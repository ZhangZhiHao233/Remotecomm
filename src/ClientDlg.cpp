// ClientDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "RemoteComm.h"
#include "ClientDlg.h"
#include "MyTcp.h"
#include "MyUdp.h"
#include "BthListDlg.h"

#include "MyBlueTooth.h"
#include "BthTipFileDlg.h"

bool g_bflagCli = TRUE;

CBthListDlg* pBthList = NULL;
CBthTipFileDlg* pBthTipFile = NULL;
CMyTcp g_TCP_Cli;
CMyUdp g_UDP_Cli;
CMyBlueTooth g_BTH_Cli;
HANDLE g_hThread_Client_Net = NULL;
HANDLE g_hThread_Client_BTH_File = NULL;
HANDLE g_hThread_Client_TCP_File = NULL;
HANDLE g_hThread_Client_UDP_File = NULL;
HANDLE g_hThread_UDPClientCom = NULL;

HANDLE g_hThread_TCPClientCom = NULL;
CClientDlg* pClient = NULL;
extern int g_protocol;
extern BOOL bThread2;
extern CMyBlueTooth g_BTH_list;
int iSendCount = 0;
int iRecvCount = 0;
// CClientDlg 对话框

IMPLEMENT_DYNAMIC(CClientDlg, CDialog)

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	, m_str_net_cli(_T(""))
	, m_richedit_recv_string(_T(""))
	, m_richedit_send_string(_T(""))
{
	AfxInitRichEdit2();
}

CClientDlg::~CClientDlg()
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_ipadress);
	DDX_Control(pDX, IDC_EDIT_TIP_CLI, m_edit_tip);
	DDX_Control(pDX, IDC_BUTTON_CNYSER, m_button_cntser);
	DDX_Control(pDX, IDC_RICHEDIT_NET_CLI, m_richedit_net_cli);
	DDX_Control(pDX, IDC_EDIT_NET_CLI, m_edit_net_cli);
	DDX_Text(pDX, IDC_EDIT_NET_CLI, m_str_net_cli);
	DDX_Control(pDX, IDC_BUTTON_SEND_NET, m_button_send_net);
	DDX_Control(pDX, IDC_RADIO_TEXT_RECV_CLI, m_radio_recvmode_cli);
	DDX_Control(pDX, IDC_RADIO_TEXT_SEND_CLI, m_radio_sendmode_cli);
	DDX_Control(pDX, IDC_RADIO_HEX_RECV_CLI, m_radio_recvmode_cli_hex);
	DDX_Control(pDX, IDC_RADIO_HEX_SEND_CLI, m_radio_sendmode_cli_hex);
	DDX_Control(pDX, IDC_COMBO_COMCLI, m_combo_com_cli);
	DDX_Control(pDX, IDC_COMBO_BAURD_CLI, m_combo_barud_cli);
	DDX_Control(pDX, IDC_COMBO_DATE_CLI, m_combo_date_cli);
	DDX_Control(pDX, IDC_COMBO_STOP_CLI, m_combo_stop_cli);
	DDX_Control(pDX, IDC_COMBO_CHECK_CLI, m_combo_check_cli);
	DDX_Control(pDX, IDC_RICHEDIT_RECV, m_richedit_recv);
	DDX_Control(pDX, IDC_RICHEDIT_SEND, m_richedit_send);
	DDX_Text(pDX, IDC_RICHEDIT_RECV, m_richedit_recv_string);
	DDX_Text(pDX, IDC_RICHEDIT_SEND, m_richedit_send_string);
	DDX_Control(pDX, IDC_BUTTON_SENDCOM, m_button_send_com);
	DDX_Control(pDX, IDC_BUTTON_AUTOSEND, m_button_auto_sendcom);
	DDX_Control(pDX, IDC_EDIT_SEND, m_edit_send);
	DDX_Control(pDX, IDC_EDIT_RECV, m_edit_recv);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CNYSER, &CClientDlg::OnBnClickedButtonCntSer)
	ON_BN_CLICKED(IDC_BUTTON_SEND_NET, &CClientDlg::OnButtonSendNet)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOADHEX, &CClientDlg::OnDownloadhex)
	ON_BN_CLICKED(IDC_BUTTON_SENDCOM, &CClientDlg::OnSendcom)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RECV, &CClientDlg::OnBnClickedButtonClearRecv)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_RECV, &CClientDlg::OnBnClickedButtonSaveRecv)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SEND, &CClientDlg::OnBnClickedButtonClearSend)
	ON_BN_CLICKED(IDC_BUTTON_AUTOSEND, &CClientDlg::OnAutosend)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, &CClientDlg::OnZeroCount)
	ON_COMMAND(ID_BTH_32782, &CClientDlg::OnBthDisCnt)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_TEXT_RECV_CLI, &CClientDlg::OnBnClickedRadioTextRecvCli)
	ON_BN_CLICKED(IDC_RADIO_HEX_RECV_CLI, &CClientDlg::OnBnClickedRadioHexRecvCli)
	ON_BN_CLICKED(IDC_RADIO_TEXT_SEND_CLI, &CClientDlg::OnBnClickedRadioTextSendCli)
	ON_BN_CLICKED(IDC_RADIO_HEX_SEND_CLI, &CClientDlg::OnBnClickedRadioHexSendCli)
	ON_BN_CLICKED(IDC_BUTTON_SACE_SEND, &CClientDlg::OnBnClickedButtonSaceSend)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pClient = this;
	m_ipadress.SetAddress(192,168,1,159);
	if(g_protocol == UDP_MODE)
		SetDlgItemInt(IDC_EDIT_PORT,UDP_SERVER);
	else if (g_protocol == TCP_MODE)
	{
		SetDlgItemInt(IDC_EDIT_PORT,8888);
	}
	//else
	//	SetBthList();
	InitEditText();

	{
		pBthList = new CBthListDlg();
		pBthList->Create(IDD_DIALOG_BTHLIST,this);
		CRect m_Rect;
		this->GetClientRect(&m_Rect);
		CRect Temp;
		Temp.left = m_Rect.left;
		Temp.top = m_Rect.top;
		Temp.right = m_Rect.left + 245;
		Temp.bottom = m_Rect.top + 305;
		pBthList->MoveWindow(&Temp);
		pBthList->ShowWindow(SW_HIDE);
	}

	OnInitComDate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CString CClientDlg::InitEditText()
{
	WSADATA wsaData;
	int err;
	err = WSAStartup(MAKEWORD(2,2),&wsaData);

	char chHostname[128];
	CString cstrHostName;
	if(gethostname(chHostname,128) == 0)
	{
		cstrHostName = CString(chHostname);
	}
	else
	{
		cstrHostName = "error";
	}

	struct hostent *pHost = gethostbyname(chHostname);
	CString cstrIP;
	for(int i = 0;pHost != NULL && pHost->h_addr_list[i] != NULL;i++)
	{
		LPCTSTR psz = inet_ntoa(*(struct in_addr *)pHost->h_addr_list[i]);
		//cstrIP += psz;
		if(CString(psz).Find("192.168.1") == -1)//过滤非局域网IP
		{
			//cstrIP = "";
		}
		else
			cstrIP = psz;
	}
	g_UDP_Cli.cstrClientIP = cstrIP;
	CString cstrTip;
	cstrTip.Format("I>本机IP地址：%s    主机名：%s\r\nI>使用TCP协议\r\n",cstrIP,cstrHostName);
	m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
	m_edit_tip.ReplaceSel(cstrTip);
	WSACleanup();

	return cstrIP;
}

DWORD WINAPI ThreadTCPClientCom(LPVOID lpParameter)
{
	g_UDP_Cli.StartUDPClientCom(COM_DATE,pClient->m_hWnd);
	return 0;
}

DWORD WINAPI ThreadUDPClientCom(LPVOID lpParameter)
{
	g_UDP_Cli.StartUDPClientCom(COM_DATE,pClient->m_hWnd);
	return 0;
}


void CClientDlg::OnBnClickedButtonCntSer()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstrTemp;
	m_button_cntser.GetWindowText(cstrTemp);
	if(cstrTemp == "断开连接")
	{
		if (g_protocol == TCP_MODE)
		{
			//若用Close()的话客户端捕捉不到FD_CLOSE信息
			g_TCP_Cli.m_MyTcpClient.ShutDown();
			g_TCP_Cli.m_MyTcpClient.Close();
			g_bflagCli = FALSE;
			closesocket(g_UDP_Cli.m_MyUdpClientCom);
			WSACleanup();
		}
		if (g_protocol == UDP_MODE)
		{
			g_bflagCli = FALSE;
			CString cstrCmd = "DISCONN";
			g_UDP_Cli.SendNetInfo(cstrCmd);
			g_UDP_Cli.m_MyUdpClient.Close();
			closesocket(g_UDP_Cli.m_MyUdpClientCom);
			WSACleanup();
		}
		pClient->m_button_send_com.EnableWindow(FALSE);
		pClient->m_button_auto_sendcom.EnableWindow(FALSE);
		pClient->m_button_send_net.EnableWindow(TRUE);
		m_button_cntser.SetWindowText("连接服务器");
		return;
	}

	if(m_ipadress.IsBlank())
	{
		AfxMessageBox("IP地址不能为空！");
		return;
	}

	if(0 == GetDlgItemInt(IDC_EDIT_PORT))
	{
		AfxMessageBox("PORT不能为空！");
		return;
	}

	BYTE field1,field2,field3,field4;
	m_ipadress.GetAddress(field1,field2,field3,field4);
	g_TCP_Cli.cstrServerIP.Format("%d.%d.%d.%d",field1,field2,field3,field4);
	g_TCP_Cli.dwServerPort = GetDlgItemInt(IDC_EDIT_PORT);

	g_UDP_Cli.cstrServerIP.Format("%d.%d.%d.%d",field1,field2,field3,field4);
	g_UDP_Cli.dwServerPort = GetDlgItemInt(IDC_EDIT_PORT);

	if (g_protocol == TCP_MODE)
	{
		if (g_TCP_Cli.ConnectSer())
		{
			CString cstrTemp;
			cstrTemp.Format("I>连接服务器：%s:%d 成功!\r\n",g_TCP_Cli.cstrServerIP,g_TCP_Cli.dwServerPort);
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel(cstrTemp);
			m_button_cntser.SetWindowText("断开连接");
			g_hThread_TCPClientCom = (HWND)::CreateThread(NULL,0,ThreadTCPClientCom,NULL,0,NULL);
			CloseHandle(g_hThread_TCPClientCom);
			m_richedit_net_cli.Clear();
		}
	}
	else if (g_protocol == UDP_MODE)
	{
		//UDP不需要连接，因此该函数主要是发送数据包确认服务器可以接受数据
		if(g_UDP_Cli.ConnectSer())
		{
			CString cstrTemp;
			cstrTemp.Format("I>连接服务器：%s:%d 成功!\r\n",g_UDP_Cli.cstrServerIP,g_UDP_Cli.dwServerPort);
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel(cstrTemp);
			m_button_cntser.SetWindowText("断开连接");
			g_hThread_UDPClientCom = (HWND)::CreateThread(NULL,0,ThreadUDPClientCom,NULL,0,NULL);
			CloseHandle(g_hThread_UDPClientCom);
			m_richedit_net_cli.Clear();
		}
	}
}

//网络信息发送线程
DWORD WINAPI ThreadClientNet(LPVOID lpParameter)
{
	pClient->m_edit_net_cli.GetWindowText(pClient->m_str_net_cli);
	CString cstrSend = pClient->m_str_net_cli;
	if(cstrSend.IsEmpty())
	{
		AfxMessageBox("输入内容不能为空！");
		return -1;
	}
	
	if (g_protocol == TCP_MODE)
	{
		if (g_TCP_Cli.SendNetInfo(cstrSend))
		{
			pClient->m_edit_net_cli.SetWindowText("");
			pClient->SetListInfo(cstrSend,0);
			return 0;
		}
	}
	else if(g_protocol == UDP_MODE)
	{
		if (g_UDP_Cli.SendNetInfo(cstrSend))
		{
			pClient->m_edit_net_cli.SetWindowText("");
			pClient->SetListInfo(cstrSend,0);
			return 0;
		}
	}
	

	return -1;
}


void CClientDlg::OnButtonSendNet()
{
	// TODO: 在此添加控件通知处理程序代码
	g_hThread_Client_Net= (HWND)CreateThread(NULL,0,ThreadClientNet,NULL,0,NULL);
	CloseHandle(g_hThread_Client_Net);
}

void CClientDlg::SetListInfo(CString cstrInfo,int iMode)
{
	m_richedit_net_cli.SetSel(m_richedit_net_cli.GetWindowTextLength(),
		m_richedit_net_cli.GetWindowTextLength());
	CString cstrTemp;

	CHARFORMAT cf;
	ZeroMemory(&cf,sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.dwEffects = 0;
	cf.yHeight = 80;

	int iLineStart,iLineEnd;
	int iCount = m_richedit_net_cli.GetLineCount();

	switch(iMode)
	{
	case 0:
		{  
			cstrTemp.Format("我 发出普通消息：\n%s\n",cstrInfo);

			iLineStart = m_richedit_net_cli.LineIndex(iCount - 1) ;
			iLineEnd = m_richedit_net_cli.LineIndex(iCount -1) + 2;
			cf.crTextColor = RGB(0,255,0);
			break;
		}
	case 1:
		{
			cstrTemp = cstrInfo;
			int iPos = cstrTemp.Find(' ');//获取IP地址长度

			iLineStart = m_richedit_net_cli.LineIndex(iCount - 1) ;
			iLineEnd = m_richedit_net_cli.LineIndex(iCount -1) + iPos;
			cf.crTextColor = RGB(255,0,0);
			break;
		}
	case 2:
		{	
			cstrTemp = "我 进入串口通道...\n";
			iLineStart = m_richedit_net_cli.LineIndex(iCount - 1) ;
			iLineEnd = m_richedit_net_cli.LineIndex(iCount -1) + 2;
			m_button_send_net.EnableWindow(FALSE);
			m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel("I>进入串口通道,可以发送串口数据\r\n");
			cf.crTextColor = RGB(0,255,0);
			break;	
		}
	}
	m_richedit_net_cli.ReplaceSel(cstrTemp);

	m_richedit_net_cli.SetSel(iLineStart,iLineEnd);
	m_richedit_net_cli.SetSelectionCharFormat(cf);

	m_richedit_net_cli.LineScroll(m_richedit_net_cli.GetLineCount());
}

//改变客户端蓝牙模式界面，先改协议在进入客户端未测试
void CClientDlg::SetBthList(int iMode)
{
	if (iMode == SW_HIDE)
	{
		m_ipadress.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(SW_HIDE);
		m_button_cntser.ShowWindow(SW_HIDE);
		m_richedit_net_cli.ShowWindow(SW_HIDE);
		m_edit_net_cli.ShowWindow(SW_HIDE);
		m_button_send_net.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow(SW_HIDE);

		pBthList->ShowWindow(SW_SHOW);
	
		BTUserList LocalBTHList;
		g_BTH_Cli.SearchLocalBT(LocalBTHList);
		int iCount = LocalBTHList.size();
		for (int i = 0;i < iCount;i++)
		{
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			int iIndex = LocalBTHList[i].Find('(');
			CString cstrBTHName = LocalBTHList[i].Left(iIndex);
			CString cstrBTHAdress = LocalBTHList[i].Mid(iIndex);
			CString cstrTemp;
			cstrTemp.Format("I>本地蓝牙名称:%s \r\nI>本地蓝牙地址:%s\r\n",cstrBTHName,cstrBTHAdress);
			m_edit_tip.ReplaceSel(cstrTemp);
		}
	}
	else if(iMode == SW_SHOW)
	{
		pBthList->ShowWindow(SW_HIDE);

		m_ipadress.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(SW_SHOW);
		m_button_cntser.ShowWindow(SW_SHOW);
		m_richedit_net_cli.ShowWindow(SW_SHOW);
		m_edit_net_cli.ShowWindow(SW_SHOW);
		m_button_send_net.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow(SW_SHOW);
	}
}

//单独创建文件套接字
DWORD WINAPI OnFileTCPSend(LPVOID lpvThreadParam)
{
	WIN32_FIND_DATA FindFileData;
	CString strPathName;
	CSocket m_MyTcpClientFile;
	CString csTemp = "SENDFILE";
	AfxSocketInit();//在线程中单独初始化！！
	m_MyTcpClientFile.Create();
	g_TCP_Cli.m_MyTcpClient.Send(csTemp,csTemp.GetLength());

	m_MyTcpClientFile.Connect(g_TCP_Cli.cstrServerIP,8880);

	CFileDialog fileDialog(TRUE,0,0,4|2,"MyHexFile(*.hex) |*.hex||");//后缀加“||”解决下拉类型乱码
	if(fileDialog.DoModal() == IDOK)
	{
		strPathName = fileDialog.GetPathName();
	} 
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	CString cstrTemp;
	cstrTemp.Format("I>已选择HEX文件:\r\n%s",strPathName);
	pClient->m_edit_tip.ReplaceSel(cstrTemp);

	FindClose(FindFirstFile(strPathName,&FindFileData));

	m_MyTcpClientFile.Send(&FindFileData,sizeof(WIN32_FIND_DATA));

	CFile file;
	if (!file.Open(strPathName,CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("文件不存在！");
		return -1;
	}
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>下载中...\r\n");

	UINT nSize = 0;
	UINT nSend = 0;
	char szBuff[2048];
	while(nSize<FindFileData.nFileSizeLow)
	{
		memset(szBuff,0,2048);
		nSend = file.Read(szBuff,2048);
		m_MyTcpClientFile.Send(szBuff,nSend);//发送数据
		nSize += nSend;
	}

	m_MyTcpClientFile.Close();
	file.Close();

	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>文件下载成功!\r\n");

	return 0;
}

DWORD WINAPI OnFileUDPSend(LPVOID lpvThreadParam)
{
	sockaddr_in RemoteAddr;  
	RemoteAddr.sin_family = AF_INET;  
	RemoteAddr.sin_port = htons(UDP_SERVER);  
	RemoteAddr.sin_addr.s_addr=inet_addr(g_UDP_Cli.cstrServerIP);  
	CString csTemp = "SENDFILE";
	csTemp = g_UDP_Cli.AddDateHead(csTemp);
	g_UDP_Cli.m_MyUdpClient.SendTo(csTemp,csTemp.GetLength(),(sockaddr*)&RemoteAddr,sizeof(RemoteAddr));

	WIN32_FIND_DATA FindFileData;
	CString strPathName;
	CSocket m_MyTcpClientFile;
	
	AfxSocketInit();//在线程中单独初始化！！
	m_MyTcpClientFile.Create(8886,SOCK_DGRAM,0);



	RemoteAddr.sin_port = htons(UDP_FILE);  

	CFileDialog fileDialog(TRUE,0,0,4|2,"MyHexFile(*.hex) |*.hex||");//后缀加“||”解决下拉类型乱码
	if(fileDialog.DoModal() == IDOK)
	{
		strPathName = fileDialog.GetPathName();
	} 
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	CString cstrTemp;
	cstrTemp.Format("I>已选择HEX文件:\r\n%s\r\n",strPathName);
	pClient->m_edit_tip.ReplaceSel(cstrTemp);

	FindClose(FindFirstFile(strPathName,&FindFileData));

	m_MyTcpClientFile.SendTo(&FindFileData,sizeof(WIN32_FIND_DATA),(sockaddr*)&RemoteAddr,sizeof(RemoteAddr));

	CFile file;
	if (!file.Open(strPathName,CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("文件不存在！");
		return -1;
	}
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>下载中...\r\n");

	UINT nSize = 0;
	UINT nSend = 0;
	char szBuff[2048];
	while(nSize<FindFileData.nFileSizeLow)
	{
		memset(szBuff,0,2048);
		nSend = file.Read(szBuff,2048);
		m_MyTcpClientFile.SendTo(szBuff,nSend,(sockaddr*)&RemoteAddr,sizeof(RemoteAddr));//发送数据
		nSize += nSend;
	}

	m_MyTcpClientFile.Close();
	file.Close();

	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>文件下载成功!\r\n");
	return 0;
}

DWORD WINAPI OnFileBTHSend(LPVOID lpvThreadParam)
{
	CFileDialog fdDlg(TRUE,0,0,4|2,"MyHexFile(*.hex) |*.hex||");//后缀加“||”解决下拉类型乱码
	CString cstrFilePathName;
	char fn[300];
	CFile file;
	long Filelength;
	if (IDOK == fdDlg.DoModal())
	{
		cstrFilePathName = fdDlg.GetPathName();
		if (!file.Open(cstrFilePathName.GetBuffer(),CFile::modeRead | CFile::typeBinary))
		{
			AfxMessageBox("打开文件错误!");
			return -1;
		}

		cstrFilePathName = fdDlg.GetFileName();
		strcpy(fn,cstrFilePathName.GetBuffer());
		Filelength = file.GetLength();
	}
	
	//pBthTipFile = new CBthTipFileDlg();
	//pBthTipFile->Create(IDD_DIALOG_BTHTIP_SUCESS_FILE);

	//直接居中
	//pBthTipFile->CenterWindow();
	//pBthTipFile->ShowWindow(SW_SHOW);
	//重绘控件，主线程很忙？
	//pBthTipFile->UpdateWindow();

	g_BTH_Cli.SendFileBT(fn,file,Filelength);
	//pBthTipFile->EndDialog(IDOK);

	return 0;
}

void CClientDlg::OnDownloadhex()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_protocol == TCP_MODE)
	{
		g_hThread_Client_TCP_File = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OnFileTCPSend,NULL,0,0);
		CloseHandle(g_hThread_Client_TCP_File);
	}
	else if (g_protocol == UDP_MODE)
	{
		g_hThread_Client_UDP_File = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OnFileUDPSend,NULL,0,0);
		CloseHandle(g_hThread_Client_UDP_File);
	}
	else
	{
		g_hThread_Client_BTH_File = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OnFileBTHSend,NULL,0,0);
		//g_TCP_Cli.m_MyTcpClientFile.Create();//放在线程会失败？
		CloseHandle(g_hThread_Client_BTH_File);
	}
}


//初始化串口数据
void	CClientDlg::OnInitComDate()
{
	m_combo_com_cli.SetCurSel(2);
	m_combo_barud_cli.SetCurSel(5);
	m_combo_date_cli.SetCurSel(2);
	m_combo_stop_cli.SetCurSel(0);
	m_combo_check_cli.SetCurSel(1);

	m_radio_recvmode_cli.SetCheck(TRUE);
	m_radio_sendmode_cli.SetCheck(TRUE);

	m_button_send_com.EnableWindow(FALSE);
	m_button_auto_sendcom.EnableWindow(FALSE);

	m_edit_recv.SetWindowText("0");
	m_edit_send.SetWindowText("0");
}

//分割字符串
CStringArray* CClientDlg::DivString(CString cstrText)
{
	CStringArray* m_result = new CStringArray;
	while(TRUE)
	{
		int index = cstrText.Find(_T("&"));
		if(index == -1)
		{
			m_result->Add(cstrText);
			return m_result;
		}
		CString test1 = cstrText.Left(index);
		m_result->Add(test1);
		cstrText = cstrText.Right(cstrText.GetLength()-index-1);
	}
}

void CClientDlg::UpdateComInfo(CString cstrInfo)
{
	CStringArray* result = DivString(cstrInfo);
	CString cstrComIndex = result->GetAt(0);
	CString cstrBaurdIndex = result->GetAt(1);
	CString cstrDateIndex = result->GetAt(2);
	CString cstrStopIndex = result->GetAt(3);
	CString cstrCheckIndex = result->GetAt(4);
	CString cstrRecvMode = result->GetAt(5);
	CString cstrSendMode = result->GetAt(6);

	m_combo_com_cli.SetCurSel(_ttoi(cstrComIndex));
	m_combo_barud_cli.SetCurSel(_ttoi(cstrBaurdIndex));
	m_combo_date_cli.SetCurSel(_ttoi(cstrDateIndex));
	m_combo_stop_cli.SetCurSel(_ttoi(cstrStopIndex));
	m_combo_check_cli.SetCurSel(_ttoi(cstrCheckIndex));

	if (_ttoi(cstrSendMode) == 1)
	{
		m_radio_sendmode_cli.SetCheck(TRUE);
		m_radio_sendmode_cli_hex.SetCheck(FALSE);
	}
	if (_ttoi(cstrSendMode) == 0)
	{
		m_radio_sendmode_cli.SetCheck(FALSE);
		m_radio_sendmode_cli_hex.SetCheck(TRUE);
	}
	if (_ttoi(cstrRecvMode) == 1)
	{
		m_radio_recvmode_cli.SetCheck(TRUE);
		m_radio_recvmode_cli_hex.SetCheck(FALSE);

	}
	if (_ttoi(cstrRecvMode) == 0)
	{
		m_radio_recvmode_cli.SetCheck(FALSE);
		m_radio_recvmode_cli_hex.SetCheck(TRUE);
	}

}

void CClientDlg::OnSendcom()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_protocol == TCP_MODE)
	{
		m_richedit_send.GetWindowText(m_richedit_send_string);
		m_richedit_send_string = "%" + m_richedit_send_string;
		CString cstrSend;
		int iIndex1 = m_radio_sendmode_cli.GetCheck();
		int iIndex2 = m_radio_recvmode_cli.GetCheck();
		cstrSend.Format("%s#%d*%d",m_richedit_send_string,iIndex1,iIndex2);
		g_TCP_Cli.SendNetInfo(cstrSend);
		m_richedit_send.SetWindowText("");
		pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
		pClient->m_edit_tip.ReplaceSel("I>串口数据发送成功!\r\n");
	}
	else if (g_protocol == UDP_MODE)
	{
		m_richedit_send.GetWindowText(m_richedit_send_string);
		m_richedit_send_string = "%" + m_richedit_send_string;
		CString cstrSend;
		int iIndex1 = m_radio_sendmode_cli.GetCheck();
		int iIndex2 = m_radio_recvmode_cli.GetCheck();
		cstrSend.Format("%s#%d*%d",m_richedit_send_string,iIndex1,iIndex2);
		g_UDP_Cli.SendNetInfo(cstrSend);
		m_richedit_send.SetWindowText("");
		pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
		pClient->m_edit_tip.ReplaceSel("I>串口数据发送成功!\r\n");
	}
	else if (g_protocol == BLUETOOTH_MODE)
	{
		m_richedit_send.GetWindowText(m_richedit_send_string);
		int iIndex1 = m_radio_sendmode_cli.GetCheck();
		int iIndex2 = m_radio_recvmode_cli.GetCheck();
		CString cstrSend;
		cstrSend.Format("%s#%d*%d",m_richedit_send_string,iIndex1,iIndex2);
		
		iSendCount = m_richedit_send_string.GetLength() + iSendCount;
		CString cstrSendCount;
		cstrSendCount.Format("%d",iSendCount);
		m_edit_send.SetWindowText(cstrSendCount);

		g_BTH_list.SendMessageBT(cstrSend,2);
		m_richedit_send.SetWindowText("");
		pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
		pClient->m_edit_tip.ReplaceSel("I>串口数据发送成功!\r\n");
	}
}

void CClientDlg::OnBnClickedButtonClearRecv()
{
	// TODO: 在此添加控件通知处理程序代码
	m_richedit_recv.SetWindowText("");
}

void CClientDlg::OnBnClickedButtonSaveRecv()
{
	// TODO: 在此添加控件通知处理程序代码
	OnSaveDate(0);
}

void CClientDlg::OnBnClickedButtonClearSend()
{
	// TODO: 在此添加控件通知处理程序代码
	m_richedit_send.SetWindowText("");
	m_richedit_send_string.Empty();
}


void CClientDlg::OnSaveDate(int iMode)
{
	CString cstrDateTime;  
	CTime m_time = CTime::GetCurrentTime();             //获取当前时间日期  
	cstrDateTime = m_time.Format("%Y%m%d _%H%M%S");   //格式化日期时间  
	CString cstrName;
	CString cstrDate;
	CString cstrTemp;
	if (iMode == 0)
	{
		cstrName.Format("接受数据%s.txt",cstrDateTime);
		m_richedit_recv.GetWindowText(cstrDate);
		cstrTemp.Format("I>保存%s数据成功!\r\n","接收");
	}
	else
	{
		cstrName.Format("发送数据%s.txt",cstrDateTime);
		m_richedit_send.GetWindowText(cstrDate);
		cstrTemp.Format("I>保存%s数据成功!\r\n","发送");
	}


	CFile file(cstrName, CFile::modeReadWrite | CFile::modeCreate);
	file.Write(cstrDate, cstrDate.GetLength());

	file.Close();
	m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
	m_edit_tip.ReplaceSel(cstrTemp);
}

void CClientDlg::OnAutosend()
{
	// TODO: 在此添加控件通知处理程序代码
	m_richedit_send.GetWindowText(m_richedit_send_string);
	CString cstrTime;
	GetDlgItemText(IDC_EDIT_TIME,cstrTime);
	if (m_richedit_send_string.IsEmpty() || cstrTime.IsEmpty())
	{
		AfxMessageBox("发送内容或者时间不能为空!");
		return;
	}

	CString cstrTemp;
	m_button_auto_sendcom.GetWindowText(cstrTemp);
	if (cstrTemp == "自动发送")
	{
		if (g_protocol == TCP_MODE)
		{
			int iTime = GetDlgItemInt(IDC_EDIT_TIME);
			CString cstrSend;//CString自己赋给自己会造成不可预计的错误
			cstrSend.Format("%d@%s",iTime,m_richedit_send_string);
			g_TCP_Cli.SendNetInfo(cstrSend);
			m_button_auto_sendcom.SetWindowText("停止发送");
		}
		else if (g_protocol == UDP_MODE)
		{
			int iTime = GetDlgItemInt(IDC_EDIT_TIME);
			CString cstrSend;
			cstrSend.Format("%d@%s",iTime,m_richedit_send_string);
			g_UDP_Cli.SendNetInfo(cstrSend);
			m_button_auto_sendcom.SetWindowText("停止发送");
		}
		else if (g_protocol == BLUETOOTH_MODE)
		{
			int iTime = GetDlgItemInt(IDC_EDIT_TIME);
			m_richedit_send.GetWindowText(m_richedit_send_string);
			int iIndex1 = m_radio_sendmode_cli.GetCheck();
			int iIndex2 = m_radio_recvmode_cli.GetCheck();
			CString cstrSend;
			cstrSend.Format("%s#%d*%d%d",m_richedit_send_string,iIndex1,iIndex2,iTime);
			SetTimer(1,iTime,NULL);//设置系统定时器
			g_BTH_list.SendMessageBT(cstrSend,3);
			pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
			pClient->m_edit_tip.ReplaceSel("I>串口数据发送成功!\r\n");
			m_button_auto_sendcom.SetWindowText("停止发送");
		}
	}
	else if (cstrTemp == "停止发送")
	{
		if(g_protocol == TCP_MODE)
		{
			g_TCP_Cli.SendNetInfo("ASC");
			m_button_auto_sendcom.SetWindowText("自动发送");
		}
		else if(g_protocol == UDP_MODE)
		{
			g_UDP_Cli.SendNetInfo("ASC");
			m_button_auto_sendcom.SetWindowText("自动发送");
		}
		else if(g_protocol == BLUETOOTH_MODE)
		{
			g_BTH_list.SendMessageBT(CString("A"),4);
			KillTimer(1);
			m_button_auto_sendcom.SetWindowText("自动发送");
		}
	}
}


void CClientDlg::OnZeroCount()
{
	// TODO: 在此添加控件通知处理程序代码
	m_edit_send.SetWindowText("0");
	iSendCount = 0;
	iRecvCount = 0;
	m_edit_recv.SetWindowText("0");
}


void CClientDlg::OnBthDisCnt()
{
	// TODO: 在此添加命令处理程序代码
	bThread2 = FALSE;
	g_BTH_list.SendMessageBT(CString("A"),1);
	shutdown(g_BTH_Cli.MyBthSocketCli,1);
	closesocket(g_BTH_Cli.MyBthSocketCli);
	m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
	m_edit_tip.ReplaceSel("I>与服务器断开连接！\n");
    pBthList->m_richedit_bth.Clear();
	pClient->m_button_send_com.EnableWindow(FALSE);
	pClient->m_button_auto_sendcom.EnableWindow(FALSE);
	pClient->m_button_send_net.EnableWindow(TRUE);
}

void CClientDlg::OnPaint()
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

void CClientDlg::OnBnClickedRadioTextRecvCli()
{
	// TODO: 在此添加控件通知处理程序代码
	recv_mode_cli = 1;
}

void CClientDlg::OnBnClickedRadioHexRecvCli()
{
	// TODO: 在此添加控件通知处理程序代码
	recv_mode_cli = 0;
}

void CClientDlg::OnBnClickedRadioTextSendCli()
{
	// TODO: 在此添加控件通知处理程序代码
	send_mode_cli = 1;
}

void CClientDlg::OnBnClickedRadioHexSendCli()
{
	// TODO: 在此添加控件通知处理程序代码
	send_mode_cli = 0;
}

void CClientDlg::OnBnClickedButtonSaceSend()
{
	// TODO: 在此添加控件通知处理程序代码
	OnSaveDate(1);
}


void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	iSendCount = m_richedit_send_string.GetLength() + iSendCount;
	CString cstrSendCount;
	cstrSendCount.Format("%d",iSendCount);
	m_edit_send.SetWindowText(cstrSendCount);
	CDialog::OnTimer(nIDEvent);
}


