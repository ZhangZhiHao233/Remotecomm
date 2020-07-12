// BthListDlg.cpp : 实现文件
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
// CBthListDlg 对话框

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


// CBthListDlg 消息处理程序

BOOL CBthListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_bth.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//if (pRemoteCommDlg->JudgeWindowsVersion())
	//{
		m_list_bth.InsertColumn(0,"附近蓝牙名",LVCFMT_CENTER,110,0);
		m_list_bth.InsertColumn(1,"蓝牙地址",LVCFMT_CENTER,108,1);
	//}
	//else
	//	m_list_bth.InsertColumn(0,"附近蓝牙名",LVCFMT_CENTER,220,0);
	pBthListDlg = this;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBthListDlg::OnNMDblclkListBth(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	BTUserList RemoteBTH;
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>正在搜索附近蓝牙,请稍后...\r\n");

	pBthTipDlg = new CBthTipDlg();
	pBthTipDlg->Create(IDD_DIALOG_BTHTIP,this);

	//直接居中
	pBthTipDlg->CenterWindow();
	pBthTipDlg->ShowWindow(SW_SHOW);
	//重绘控件，主线程很忙？
	pBthTipDlg->UpdateWindow();

	g_BTH_list.SearchRemoteBTNew(RemoteBTH);
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>搜索成功\r\n");

	int iCount = RemoteBTH.size();
	for (int i = 0;i < iCount;i++)
	{
		int iIndex = RemoteBTH[i].Find('(');
		CString cstrBTHName = RemoteBTH[i].Left(iIndex);
		CString cstrBTHAdress = RemoteBTH[i].Mid(iIndex);
		//if(pRemoteCommDlg->JudgeWindowsVersion())//新的函数win7下也可以用
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
	// TODO: 在此添加控件通知处理程序代码
	CMenu popup;                  //声明一个菜单变量
	popup.LoadMenu(IDR_MENU_BTH);       //载入菜单资源
	CMenu*  pM = popup.GetSubMenu(0);  //得到菜单项
	CPoint  p;
	GetCursorPos(&p);                    //得到鼠标指针的位置
	int count = pM->GetMenuItemCount();       //得到菜单的个数
	if (m_list_bth.GetSelectedCount() == 0)  //如果没有选中列表中的条目
	{
		for (int i = 0; i < count; i++)       //遍历每一个菜单
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);  //该项变灰
		}
	}
	else
	{
		int iIndex = m_list_bth.GetSelectionMark();
		CString cstrTemp  = m_list_bth.GetItemText(iIndex,1);
		g_BTH_list.cstrSelectBTHName = m_list_bth.GetItemText(iIndex,0);
		//CString 截取到底包不包括开头结尾需要总结
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
	// TODO: 在此添加命令处理程序代码
	int iRet = g_BTH_list.ConnectRemoteBT(g_BTH_list.cstrSelectBTHAddr,g_BTH_list.MyBthSocketCli);
	if (iRet == 0)
	{
		bThread2 = TRUE;
		g_hThread_BTHClientr_Receive = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OnRecieveCli,NULL,0,0);
		CloseHandle(g_hThread_BTHClientr_Receive);
		pBthSucDlg = new CBthSucessDlg();
		pBthSucDlg->Create(IDD_DIALOG_BTHTIP_SUCESS,this);

		//直接居中
		pBthSucDlg->CenterWindow();
		pBthSucDlg->ShowWindow(SW_SHOW);
		//重绘控件，主线程很忙？
		pBthSucDlg->UpdateWindow();
		Sleep(2500);
		pBthSucDlg->EndDialog(IDOK);
	}
}

void CBthListDlg::OnBnClickedButtonBthSend()
{
	// TODO: 在此添加控件通知处理程序代码
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
	cstrTemp.Format("我 说:\n%s\n",cstrSendMsg);

	m_richedit_bth.SetSel(m_richedit_bth.GetWindowTextLength(),m_richedit_bth.GetWindowTextLength());
	m_richedit_bth.ReplaceSel(cstrTemp);

	//0~0即未选中
	m_richedit_bth.SetSel(0,2);
	m_richedit_bth.SetSelectionCharFormat(cf);
	//自动垂直滚动
	m_richedit_bth.LineScroll(m_richedit_bth.GetLineCount());
}

void CBthListDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect   rect;  
	GetClientRect(&rect);    //获取对话框长宽      
	CDC   dcBmp;             //定义并创建一个内存设备环境
	dcBmp.CreateCompatibleDC(&dc);             //创建兼容性DC
	CBitmap   bmpBackground;   
	int a = bmpBackground.LoadBitmap(IDB_BITMAP2);    //载入资源中图片
	BITMAP   m_bitmap;                         //图片变量               
	bmpBackground.GetBitmap(&m_bitmap);       //将图片载入位图中
	//将位图选入临时内存设备环境
	CBitmap  *pbmpOld=dcBmp.SelectObject(&bmpBackground);
	//调用函数显示图片StretchBlt显示形状可变
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcBmp,0,0,m_bitmap.bmWidth,m_bitmap.bmHeight,SRCCOPY);  
}
