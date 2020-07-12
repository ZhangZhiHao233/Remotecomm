// BthListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyBlueTooth.h"
#include "RemoteComm.h"
#include "RemoteCommDlg.h"
#include "BthListDlg.h"
#include "ClientDlg.h"
#include "BthTipDlg.h"
#include "BthSucessDlg.h"
extern CRemoteCommDlg* pRemoteCommDlg;
CBthSucessDlg *pBthSucDlg = NULL;
CBthTipDlg *pBthTipDlg = NULL;
CBthListDlg *pBthListDlg = NULL;
CMyBlueTooth g_BTH_list;
extern CClientDlg* pClient; 
BOOL bThread2;
HANDLE g_hThread_BTHClientr_Receive = NULL;
// CBthListDlg �Ի���

IMPLEMENT_DYNAMIC(CBthListDlg, CDialog)

CBthListDlg::CBthListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBthListDlg::IDD, pParent)
{

}

CBthListDlg::~CBthListDlg()
{
}

void CBthListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BTH, m_list_bth);
	DDX_Control(pDX, IDC_RICHEDIT_BTH, m_richedit_bth);
	DDX_Control(pDX, IDC_EDIT_BTH, m_bth_edit_send);
	DDX_Control(pDX, IDC_BUTTON_BTH_SEND, m_btn_send_bth);
}
 

BEGIN_MESSAGE_MAP(CBthListDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BTH, &CBthListDlg::OnNMDblclkListBth)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BTH, &CBthListDlg::OnNMRClickListBth)
	ON_COMMAND(ID_BTH_32781, &CBthListDlg::OnBthConnect)
	ON_BN_CLICKED(IDC_BUTTON_BTH_SEND, &CBthListDlg::OnBnClickedButtonBthSend)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBthListDlg ��Ϣ�������

BOOL CBthListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_list_bth.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//if (pRemoteCommDlg->JudgeWindowsVersion())
	//{
		m_list_bth.InsertColumn(0,"����������",LVCFMT_CENTER,110,0);
		m_list_bth.InsertColumn(1,"������ַ",LVCFMT_CENTER,108,1);
	//}
	//else
	//	m_list_bth.InsertColumn(0,"����������",LVCFMT_CENTER,220,0);
	pBthListDlg = this;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CBthListDlg::OnNMDblclkListBth(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BTUserList RemoteBTH;
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>����������������,���Ժ�...\r\n");

	pBthTipDlg = new CBthTipDlg();
	pBthTipDlg->Create(IDD_DIALOG_BTHTIP,this);

	//ֱ�Ӿ���
	pBthTipDlg->CenterWindow();
	pBthTipDlg->ShowWindow(SW_SHOW);
	//�ػ�ؼ������̺߳�æ��
	pBthTipDlg->UpdateWindow();

	g_BTH_list.SearchRemoteBTNew(RemoteBTH);
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>�����ɹ�\r\n");

	int iCount = RemoteBTH.size();
	for (int i = 0;i < iCount;i++)
	{
		int iIndex = RemoteBTH[i].Find('(');
		CString cstrBTHName = RemoteBTH[i].Left(iIndex);
		CString cstrBTHAdress = RemoteBTH[i].Mid(iIndex);
		//if(pRemoteCommDlg->JudgeWindowsVersion())//�µĺ���win7��Ҳ������
		//{
			m_list_bth.InsertItem(i,cstrBTHName);
			m_list_bth.SetItemText(i,1,cstrBTHAdress);
		//}
		//else
		//	m_list_bth.InsertItem(i,cstrBTHName);
	}
	
	pBthTipDlg->EndDialog(IDOK);
	*pResult = 0;
}

void CBthListDlg::OnNMRClickListBth(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu popup;                  //����һ���˵�����
	popup.LoadMenu(IDR_MENU_BTH);       //����˵���Դ
	CMenu*  pM = popup.GetSubMenu(0);  //�õ��˵���
	CPoint  p;
	GetCursorPos(&p);                    //�õ����ָ���λ��
	int count = pM->GetMenuItemCount();       //�õ��˵��ĸ���
	if (m_list_bth.GetSelectedCount() == 0)  //���û��ѡ���б��е���Ŀ
	{
		for (int i = 0; i < count; i++)       //����ÿһ���˵�
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);  //������
		}
	}
	else
	{
		int iIndex = m_list_bth.GetSelectionMark();
		CString cstrTemp  = m_list_bth.GetItemText(iIndex,1);
		g_BTH_list.cstrSelectBTHName = m_list_bth.GetItemText(iIndex,0);
		//CString ��ȡ���װ���������ͷ��β��Ҫ�ܽ�
		g_BTH_list.cstrSelectBTHAddr = cstrTemp.Mid(1,cstrTemp.GetLength()-2);
	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	*pResult = 0;
}

DWORD WINAPI OnRecieveCli(LPVOID lpParameter)
{
	g_BTH_list.RecvBthCli();
	return 0;
}


void CBthListDlg::OnBthConnect()
{
	// TODO: �ڴ���������������
	int iRet = g_BTH_list.ConnectRemoteBT(g_BTH_list.cstrSelectBTHAddr,g_BTH_list.MyBthSocketCli);
	if (iRet == 0)
	{
		bThread2 = TRUE;
		g_hThread_BTHClientr_Receive = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OnRecieveCli,NULL,0,0);
		CloseHandle(g_hThread_BTHClientr_Receive);
		pBthSucDlg = new CBthSucessDlg();
		pBthSucDlg->Create(IDD_DIALOG_BTHTIP_SUCESS,this);

		//ֱ�Ӿ���
		pBthSucDlg->CenterWindow();
		pBthSucDlg->ShowWindow(SW_SHOW);
		//�ػ�ؼ������̺߳�æ��
		pBthSucDlg->UpdateWindow();
		Sleep(2500);
		pBthSucDlg->EndDialog(IDOK);
	}
}

void CBthListDlg::OnBnClickedButtonBthSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstrSendMsg;
	m_bth_edit_send.GetWindowText(cstrSendMsg);
	g_BTH_list.SendMessageBT(cstrSendMsg);

	CHARFORMAT cf;
	ZeroMemory(&cf,sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.dwEffects = 0;
	cf.yHeight = 80;
	cf.crTextColor = RGB(255,0,0);

	CString cstrTemp;
	cstrTemp.Format("�� ˵:\n%s\n",cstrSendMsg);

	m_richedit_bth.SetSel(m_richedit_bth.GetWindowTextLength(),m_richedit_bth.GetWindowTextLength());
	m_richedit_bth.ReplaceSel(cstrTemp);

	//0~0��δѡ��
	m_richedit_bth.SetSel(0,2);
	m_richedit_bth.SetSelectionCharFormat(cf);
	//�Զ���ֱ����
	m_richedit_bth.LineScroll(m_richedit_bth.GetLineCount());
}

void CBthListDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect   rect;  
	GetClientRect(&rect);    //��ȡ�Ի��򳤿�      
	CDC   dcBmp;             //���岢����һ���ڴ��豸����
	dcBmp.CreateCompatibleDC(&dc);             //����������DC
	CBitmap   bmpBackground;   
	int a = bmpBackground.LoadBitmap(IDB_BITMAP2);    //������Դ��ͼƬ
	BITMAP   m_bitmap;                         //ͼƬ����               
	bmpBackground.GetBitmap(&m_bitmap);       //��ͼƬ����λͼ��
	//��λͼѡ����ʱ�ڴ��豸����
	CBitmap  *pbmpOld=dcBmp.SelectObject(&bmpBackground);
	//���ú�����ʾͼƬStretchBlt��ʾ��״�ɱ�
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcBmp,0,0,m_bitmap.bmWidth,m_bitmap.bmHeight,SRCCOPY);  
}
