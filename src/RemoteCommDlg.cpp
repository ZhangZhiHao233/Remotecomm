// RemoteCommDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "RemoteCommDlg.h"
#include "WarningDlg.h"


#include "ServerDlg.h"
#include "ClientDlg.h"

CServerDlg* pServerDlg = NULL;
CClientDlg* pClientDlg = NULL;


CRemoteCommDlg* pRemoteCommDlg = NULL;

//#ifdef _DEBUG
//#define new DEBUG7_NEW
//#endif

BOOL g_mode = TRUE;
int g_protocol = TCP_MODE;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBthConnect();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

	ON_WM_PAINT()
END_MESSAGE_MAP()


// CRemoteCommDlg �Ի���


CAboutDlg* pAboutDlg = NULL;

CRemoteCommDlg::CRemoteCommDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteCommDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
}

void CRemoteCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRemoteCommDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_MSERVER, &CRemoteCommDlg::OnUpdateMserver)
	ON_UPDATE_COMMAND_UI(ID_MCLIENT, &CRemoteCommDlg::OnUpdateMclient)
	ON_COMMAND(ID_MSERVER, &CRemoteCommDlg::OnMserver)
	ON_COMMAND(ID_MCLIENT, &CRemoteCommDlg::OnMclient)
	ON_COMMAND(ID_MTCP, &CRemoteCommDlg::OnMtcp)
	ON_COMMAND(ID_MUDP, &CRemoteCommDlg::OnMudp)
	ON_COMMAND(ID_MBLUETOOTH, &CRemoteCommDlg::OnMbluetooth)
	ON_UPDATE_COMMAND_UI(ID_MTCP, &CRemoteCommDlg::OnUpdateMtcp)
	ON_UPDATE_COMMAND_UI(ID_MUDP, &CRemoteCommDlg::OnUpdateMudp)
	ON_UPDATE_COMMAND_UI(ID_MBLUETOOTH, &CRemoteCommDlg::OnUpdateMbluetooth)
	ON_COMMAND(ID_ABOUT, &CRemoteCommDlg::OnAbout)
END_MESSAGE_MAP()


// CRemoteCommDlg ��Ϣ�������

BOOL CRemoteCommDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_menu.LoadMenu(IDR_MENU);
	SetMenu(&m_menu);

	//��ʼ��ʾ������ģʽ
	OnCreateServerDlg();
	this->SetWindowText("Զ�̵��Թ���-������");
	pAboutDlg = new CAboutDlg();
	pAboutDlg->Create(IDD_ABOUTBOX);

	//ֱ�Ӿ���
	pAboutDlg->CenterWindow();
	pAboutDlg->ShowWindow(SW_SHOW);
	//�ػ�ؼ������̺߳�æ��
	pAboutDlg->UpdateWindow();

	Sleep(2000);
	pAboutDlg->EndDialog(IDOK);

	pRemoteCommDlg = this;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRemoteCommDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRemoteCommDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRemoteCommDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRemoteCommDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	if(!bSysMenu && pPopupMenu)
	{
		CCmdUI cmdUI;
		cmdUI.m_pOther = NULL;
		cmdUI.m_pMenu = pPopupMenu;
		cmdUI.m_pSubMenu = NULL;

		UINT count = pPopupMenu->GetMenuItemCount();
		cmdUI.m_nIndexMax = count;
		for(UINT i=0; i<count; i++)
		{
			UINT nID = pPopupMenu->GetMenuItemID(i);
			if(-1 == nID || 0 == nID)
			{
				continue;
			}
			cmdUI.m_nID = nID;
			cmdUI.m_nIndex = i;
			cmdUI.DoUpdate(this, FALSE);
		}
	} 
}

//�˵�ѡ����´���

//������-�ͻ���ģʽѡ��
void CRemoteCommDlg::OnUpdateMserver(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(g_mode);
}

void CRemoteCommDlg::OnUpdateMclient(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(!g_mode);
}

void CRemoteCommDlg::OnMserver()
{
	// TODO: �ڴ���������������
	g_mode = TRUE;
	if (pClientDlg)
	{
		pClientDlg->ShowWindow(SW_HIDE);
	}
	pServerDlg->ShowWindow(SW_SHOW);
	this->SetWindowText("Զ�̵��Թ���-������");
}

void CRemoteCommDlg::OnMclient()
{
	// TODO: �ڴ���������������
	g_mode = FALSE;
	if (pServerDlg)
	{
		pServerDlg->ShowWindow(SW_HIDE);
	}
	if (pClientDlg == NULL)
	{
		OnCreateClientDlg();
	}
	else
	{
		pClientDlg->ShowWindow(SW_SHOW);
	}
	this->SetWindowText("Զ�̵��Թ���-�ͻ���");
}

//TCP/UDP/BLUETOOTH Э��ѡ��
void CRemoteCommDlg::OnUpdateMtcp(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(g_protocol == TCP_MODE)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CRemoteCommDlg::OnUpdateMudp(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(g_protocol == UDP_MODE)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CRemoteCommDlg::OnUpdateMbluetooth(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(g_protocol == BLUETOOTH_MODE)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CRemoteCommDlg::OnMtcp()
{
	// TODO: �ڴ���������������
	g_protocol = TCP_MODE;
	if (g_mode)
	{
		pServerDlg->m_edit_tip.SetSel(pServerDlg->m_edit_tip.GetWindowTextLength(),pServerDlg->m_edit_tip.GetWindowTextLength());
		pServerDlg->ChangeList(1);
		pServerDlg->m_edit_tip.ReplaceSel("I>ʹ��TCPЭ��\r\n");
	}
	else
	{
		pClientDlg->m_edit_tip.SetSel(pClientDlg->m_edit_tip.GetWindowTextLength(),pClientDlg->m_edit_tip.GetWindowTextLength());
		pClientDlg->m_edit_tip.ReplaceSel("I>ʹ��TCPЭ��\r\n");
		pClientDlg->SetBthList(SW_SHOW);
	}
}

void CRemoteCommDlg::OnMudp()
{
	// TODO: �ڴ���������������
	g_protocol = UDP_MODE;
	if (g_mode)
	{
		pServerDlg->m_edit_tip.SetSel(pServerDlg->m_edit_tip.GetWindowTextLength(),pServerDlg->m_edit_tip.GetWindowTextLength());
		pServerDlg->ChangeList(1);
		pServerDlg->m_edit_tip.ReplaceSel("I>ʹ��UDPЭ��\r\n");
	}
	else
	{
		pClientDlg->m_edit_tip.SetSel(pClientDlg->m_edit_tip.GetWindowTextLength(),pClientDlg->m_edit_tip.GetWindowTextLength());
		pClientDlg->m_edit_tip.ReplaceSel("I>ʹ��UDPЭ��\r\n");
		pClientDlg->SetBthList(SW_SHOW);
	}
}

void CRemoteCommDlg::OnMbluetooth()
{
	// TODO: �ڴ���������������
	//CWarningDlg dlg;
	//dlg.DoModal();
	if (g_mode)
	{
		//if(JudgeWindowsVersion())
		//{
		//	AfxMessageBox("WIN10 ��֧�ַ�����ģʽ!");
		//	return;
		//}
		g_protocol = BLUETOOTH_MODE;
		pServerDlg->m_edit_tip.SetSel(pServerDlg->m_edit_tip.GetWindowTextLength(),pServerDlg->m_edit_tip.GetWindowTextLength());
		pServerDlg->ChangeList(0);
		pServerDlg->m_edit_tip.ReplaceSel("I>ʹ��BLUETOOTHЭ��\r\n");
	}
	else
	{
		g_protocol = BLUETOOTH_MODE;
		pClientDlg->SetBthList(SW_HIDE);
		pClientDlg->m_edit_tip.SetSel(pClientDlg->m_edit_tip.GetWindowTextLength(),pClientDlg->m_edit_tip.GetWindowTextLength());
		pClientDlg->m_edit_tip.ReplaceSel("I>ʹ��BLUETOOTHЭ��\r\n");
	}
}

void CRemoteCommDlg::OnCreateServerDlg()
{
	pServerDlg = new CServerDlg();
	pServerDlg->Create(IDD_DIALOG_SERVER,this);

	CRect m_Rect;
	this->GetClientRect(&m_Rect);
	CRect Temp;
	Temp.left = m_Rect.left;
	Temp.top = m_Rect.top;
	Temp.right = m_Rect.right;
	Temp.bottom = m_Rect.bottom;
	pServerDlg->MoveWindow(&Temp);
	pServerDlg->ShowWindow(SW_SHOW);

}

void CRemoteCommDlg::OnCreateClientDlg()
{
	pClientDlg = new CClientDlg();
	pClientDlg->Create(IDD_DIALOG_CLIENT,this);

	CRect m_Rect;
	this->GetClientRect(&m_Rect);
	CRect Temp;
	Temp.left = m_Rect.left;
	Temp.top = m_Rect.top;
	Temp.right = m_Rect.right;
	Temp.bottom = m_Rect.bottom;
	pClientDlg->MoveWindow(&Temp);
	pClientDlg->ShowWindow(SW_SHOW);
}

/************************************************************************/
/* Operating system Version number 
Windows 10 10.0* 
Windows Server 2016 10.0* 
Windows 8.1 6.3* 
Windows Server 2012 R2 6.3* 
Windows 8 6.2 
Windows Server 2012 6.2 
Windows 7 6.1 

Windows Server 2008 R2 6.1 
Windows Server 2008 6.0 
Windows Vista 6.0 
Windows Server 2003 R2 5.2 
Windows Server 2003 5.2 
Windows XP 64-Bit Edition 5.2 
Windows XP 5.1 
Windows 2000 5.0                                                                     */
/************************************************************************/
BOOL CRemoteCommDlg::JudgeWindowsVersion()
{//���û������ȡϵͳ�汾��
	int a=0,b=0,i=0,j=0;    
	_asm    
	{    
		pushad    
			mov ebx,fs:[0x18] ; get self pointer from TEB    
			mov eax,fs:[0x30] ; get pointer to PEB / database    
			mov ebx,[eax+0A8h] ; get OSMinorVersion    
			mov eax,[eax+0A4h] ; get OSMajorVersion    
			mov j,ebx    
			mov i,eax    
			popad    
	}    
	if((i==10)&&(j==0))    
	{    
		return TRUE;
	}  
	else
		return FALSE;
		/*if((i==5)&&(j==0))    
		{    
			osName = _T("��ǰϵͳ�汾Ϊ��Windows 2000");    
		}    
		else if((i==5)&&(j==1))    
		{     
			osName = _T("��ǰϵͳ�汾Ϊ��Windows XP");    
		}      
		else if((i==6)&&(j==0))    
		{    
			osName = _T("��ǰϵͳ�汾Ϊ��Windows Vista");    
		}    
		else if((i==6)&&(j==1))    
		{    
			osName = _T("��ǰϵͳ�汾Ϊ��Windows 7");    
		}  
		else if((i==6)&&(j==2))    
		{    
			osName = _T("��ǰϵͳ�汾Ϊ��Windows 8");    
		}  
		else if((i==6)&&(j==3))    
		{    
			osName = _T("��ǰϵͳ�汾Ϊ��Windows 8.1");    
		}  
		*/
}

//д��־
void CRemoteCommDlg::WriteLogText(CString m_cstrLogContent)  
{  
	CString strFileName;  
	GetModuleFileName(AfxGetInstanceHandle(),strFileName.GetBuffer(_MAX_PATH),_MAX_PATH);  
	strFileName.ReleaseBuffer();  

	strFileName = strFileName.Left(strFileName.ReverseFind('\\') + 1);  
 
	CString cstrLogFilePath = strFileName + "MyLogName.log";  

	FILE* pLogFileObj = fopen(cstrLogFilePath,"a+");  
	
	m_cstrLogContent = m_cstrLogContent + "\r\n*******\r\n";
	fprintf(pLogFileObj,"%s",m_cstrLogContent);  

	fclose(pLogFileObj);  
}  

void CRemoteCommDlg::OnAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg dlg;
	dlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HICON icon;
	icon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);

	SetIcon(icon,true);
	SetIcon(icon,false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	//CPaintDC   dc(this);  
	CRect   rect;  
	GetClientRect(&rect);    //��ȡ�Ի��򳤿�      
	CDC   dcBmp;             //���岢����һ���ڴ��豸����
	dcBmp.CreateCompatibleDC(&dc);             //����������DC
	CBitmap   bmpBackground;   
	int a = bmpBackground.LoadBitmap(IDB_BITMAP1);    //������Դ��ͼƬ
	BITMAP   m_bitmap;                         //ͼƬ����               
	bmpBackground.GetBitmap(&m_bitmap);       //��ͼƬ����λͼ��
	//��λͼѡ����ʱ�ڴ��豸����
	CBitmap  *pbmpOld=dcBmp.SelectObject(&bmpBackground);
	//���ú�����ʾͼƬStretchBlt��ʾ��״�ɱ�
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcBmp,0,0,m_bitmap.bmWidth,m_bitmap.bmHeight,SRCCOPY);  
}
