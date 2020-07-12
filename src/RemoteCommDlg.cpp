// RemoteCommDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CRemoteCommDlg 对话框


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


// CRemoteCommDlg 消息处理程序

BOOL CRemoteCommDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_menu.LoadMenu(IDR_MENU);
	SetMenu(&m_menu);

	//初始显示服务器模式
	OnCreateServerDlg();
	this->SetWindowText("远程调试工具-服务器");
	pAboutDlg = new CAboutDlg();
	pAboutDlg->Create(IDD_ABOUTBOX);

	//直接居中
	pAboutDlg->CenterWindow();
	pAboutDlg->ShowWindow(SW_SHOW);
	//重绘控件，主线程很忙？
	pAboutDlg->UpdateWindow();

	Sleep(2000);
	pAboutDlg->EndDialog(IDOK);

	pRemoteCommDlg = this;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemoteCommDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

//菜单选项更新代码

//服务器-客户端模式选择
void CRemoteCommDlg::OnUpdateMserver(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(g_mode);
}

void CRemoteCommDlg::OnUpdateMclient(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(!g_mode);
}

void CRemoteCommDlg::OnMserver()
{
	// TODO: 在此添加命令处理程序代码
	g_mode = TRUE;
	if (pClientDlg)
	{
		pClientDlg->ShowWindow(SW_HIDE);
	}
	pServerDlg->ShowWindow(SW_SHOW);
	this->SetWindowText("远程调试工具-服务器");
}

void CRemoteCommDlg::OnMclient()
{
	// TODO: 在此添加命令处理程序代码
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
	this->SetWindowText("远程调试工具-客户端");
}

//TCP/UDP/BLUETOOTH 协议选择
void CRemoteCommDlg::OnUpdateMtcp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(g_protocol == TCP_MODE)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CRemoteCommDlg::OnUpdateMudp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(g_protocol == UDP_MODE)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CRemoteCommDlg::OnUpdateMbluetooth(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(g_protocol == BLUETOOTH_MODE)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CRemoteCommDlg::OnMtcp()
{
	// TODO: 在此添加命令处理程序代码
	g_protocol = TCP_MODE;
	if (g_mode)
	{
		pServerDlg->m_edit_tip.SetSel(pServerDlg->m_edit_tip.GetWindowTextLength(),pServerDlg->m_edit_tip.GetWindowTextLength());
		pServerDlg->ChangeList(1);
		pServerDlg->m_edit_tip.ReplaceSel("I>使用TCP协议\r\n");
	}
	else
	{
		pClientDlg->m_edit_tip.SetSel(pClientDlg->m_edit_tip.GetWindowTextLength(),pClientDlg->m_edit_tip.GetWindowTextLength());
		pClientDlg->m_edit_tip.ReplaceSel("I>使用TCP协议\r\n");
		pClientDlg->SetBthList(SW_SHOW);
	}
}

void CRemoteCommDlg::OnMudp()
{
	// TODO: 在此添加命令处理程序代码
	g_protocol = UDP_MODE;
	if (g_mode)
	{
		pServerDlg->m_edit_tip.SetSel(pServerDlg->m_edit_tip.GetWindowTextLength(),pServerDlg->m_edit_tip.GetWindowTextLength());
		pServerDlg->ChangeList(1);
		pServerDlg->m_edit_tip.ReplaceSel("I>使用UDP协议\r\n");
	}
	else
	{
		pClientDlg->m_edit_tip.SetSel(pClientDlg->m_edit_tip.GetWindowTextLength(),pClientDlg->m_edit_tip.GetWindowTextLength());
		pClientDlg->m_edit_tip.ReplaceSel("I>使用UDP协议\r\n");
		pClientDlg->SetBthList(SW_SHOW);
	}
}

void CRemoteCommDlg::OnMbluetooth()
{
	// TODO: 在此添加命令处理程序代码
	//CWarningDlg dlg;
	//dlg.DoModal();
	if (g_mode)
	{
		//if(JudgeWindowsVersion())
		//{
		//	AfxMessageBox("WIN10 不支持服务器模式!");
		//	return;
		//}
		g_protocol = BLUETOOTH_MODE;
		pServerDlg->m_edit_tip.SetSel(pServerDlg->m_edit_tip.GetWindowTextLength(),pServerDlg->m_edit_tip.GetWindowTextLength());
		pServerDlg->ChangeList(0);
		pServerDlg->m_edit_tip.ReplaceSel("I>使用BLUETOOTH协议\r\n");
	}
	else
	{
		g_protocol = BLUETOOTH_MODE;
		pClientDlg->SetBthList(SW_HIDE);
		pClientDlg->m_edit_tip.SetSel(pClientDlg->m_edit_tip.GetWindowTextLength(),pClientDlg->m_edit_tip.GetWindowTextLength());
		pClientDlg->m_edit_tip.ReplaceSel("I>使用BLUETOOTH协议\r\n");
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
{//利用汇编代码获取系统版本号
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
			osName = _T("当前系统版本为：Windows 2000");    
		}    
		else if((i==5)&&(j==1))    
		{     
			osName = _T("当前系统版本为：Windows XP");    
		}      
		else if((i==6)&&(j==0))    
		{    
			osName = _T("当前系统版本为：Windows Vista");    
		}    
		else if((i==6)&&(j==1))    
		{    
			osName = _T("当前系统版本为：Windows 7");    
		}  
		else if((i==6)&&(j==2))    
		{    
			osName = _T("当前系统版本为：Windows 8");    
		}  
		else if((i==6)&&(j==3))    
		{    
			osName = _T("当前系统版本为：Windows 8.1");    
		}  
		*/
}

//写日志
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
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	HICON icon;
	icon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);

	SetIcon(icon,true);
	SetIcon(icon,false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	//CPaintDC   dc(this);  
	CRect   rect;  
	GetClientRect(&rect);    //获取对话框长宽      
	CDC   dcBmp;             //定义并创建一个内存设备环境
	dcBmp.CreateCompatibleDC(&dc);             //创建兼容性DC
	CBitmap   bmpBackground;   
	int a = bmpBackground.LoadBitmap(IDB_BITMAP1);    //载入资源中图片
	BITMAP   m_bitmap;                         //图片变量               
	bmpBackground.GetBitmap(&m_bitmap);       //将图片载入位图中
	//将位图选入临时内存设备环境
	CBitmap  *pbmpOld=dcBmp.SelectObject(&bmpBackground);
	//调用函数显示图片StretchBlt显示形状可变
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcBmp,0,0,m_bitmap.bmWidth,m_bitmap.bmHeight,SRCCOPY);  
}
