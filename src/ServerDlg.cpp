
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "RemoteCommDlg.h"
#include "ServerDlg.h"
#include "MyTcp.h"
#include "MyUdp.h"
#include "MyBlueTooth.h"

CString cstrFileIP;
extern CRemoteCommDlg* pRemoteCommDlg;
extern CString cstrBTHName;
CMyBlueTooth g_BTH;
CMyTcp g_TCP;
CMyUdp g_UDP;
CServerDlg* pServer = NULL;
extern CString cstrFileInfo;
HANDLE g_hThread_TCPServer = NULL;
HANDLE g_hThread_TCPRecvFile = NULL;
HANDLE g_hThread_UDPRecvFile = NULL;
HANDLE g_hThread_UDPServerCom = NULL;
HANDLE g_hThread_UDPServer = NULL;
HANDLE g_hThread_BTHServer = NULL;
HANDLE g_hThread_BTHServer_Listen = NULL;
HANDLE g_hThread_BTHServer_Receive = NULL;
HANDLE g_hThread_TCPServerCom = NULL;

BOOL bThread0;
BOOL bThread1;
bool g_bflag = TRUE;
extern int g_protocol;
CString g_cstrIP;

int g_Recv_Count = 0;
int g_Send_Count = 0;


char check[3] = {'e','n','o'};
int Baurd[12] = {300,600,1200,2400,4800,9600,19200,38400,43000,56000,57600,115200};
int Date[3] = {6,7,8};
// CServerDlg �Ի���
CString strExeName;
IMPLEMENT_DYNAMIC(CServerDlg, CDialog)

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
	, m_strRXData(_T(""))
	, m_strTXData(_T(""))
{
	AfxInitRichEdit2();
}

CServerDlg::~CServerDlg()
{
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_STARTSERVER, m_btn_openserver);
	DDX_Control(pDX, IDC_EDIT_TIP, m_edit_tip);
	DDX_Control(pDX, IDC_LIST_CLI, m_list_cli);
	DDX_Control(pDX, IDC_RICHEDIT_NET, m_richedit_net);
	DDX_Control(pDX, IDC_EDIT_NET, m_edit_net_send);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_comb_channel);
	DDX_Control(pDX, IDC_COMB_BAUD, m_comb_baud);
	DDX_Control(pDX, IDC_COMBO_DATE, m_comb_date);
	DDX_Control(pDX, IDC_COMBO_STOP, m_comb_stop);
	DDX_Control(pDX, IDC_COMBO_CHECK, m_comb_check);
	DDX_Control(pDX, IDC_RICHEDIT_RECVCOM, m_richedit_recv_com);
	DDX_Control(pDX, IDC_RICHEDIT_SENDCOM, m_richedit_send_com);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);
	DDX_Text(pDX, IDC_RICHEDIT_RECVCOM, m_strRXData);
	DDX_Text(pDX, IDC_RICHEDIT_SENDCOM, m_strTXData);
	DDX_Control(pDX, IDC_COMBO_COM, m_combo_com);
	DDX_Control(pDX, IDC_RADIO_RECV_TEXT, m_com_recv_mode);
	DDX_Control(pDX, IDC_RADIO_SEND_TEXT, m_com_send_mode);
	DDX_Control(pDX, IDC_RADIO_RECV_HEX, m_com_recv_mode_hex);
	DDX_Control(pDX, IDC_RADIO_SEND_HEX, m_com_send_mode_hex);
	DDX_Control(pDX, IDC_EDIT_AUTOTIME, m_edit_autosend);
	DDX_Control(pDX, IDC_BUTTON_OPENCOM, m_btn_opencom);
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_MESSAGE(WM_SOCKET_TCP,OnTCPServer)
	ON_MESSAGE(WM_SOCKET_UDP,OnUDPServer)
	ON_BN_CLICKED(IDC_BUTTON_STARTSERVER, &CServerDlg::OnButtonStartserver)
	ON_BN_CLICKED(IDC_BUTTON_NET_SEND, &CServerDlg::OnButtonNetSend)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, &CServerDlg::OnchangeComboChannel)
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, &CServerDlg::OnBnClickedButtonOpencom)
	ON_BN_CLICKED(IDC_BUTTON_MANUALSEND, &CServerDlg::OnBnClickedButtonManualsend)
	ON_BN_CLICKED(IDC_BUTTON10, &CServerDlg::OnChooseSTC)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CServerDlg::OnDownload)
	ON_BN_CLICKED(IDC_BUTTON_CLEARRECV, &CServerDlg::OnClearRecv)
	ON_BN_CLICKED(IDC_BUTTON_CLEARSEND, &CServerDlg::OnClearSend)
	ON_BN_CLICKED(IDC_BUTTON_SAVERECV, &CServerDlg::OnSaveRecv)
	ON_BN_CLICKED(IDC_BUTTON_SAVESEND, &CServerDlg::OnSaveSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_AUTOSEND, &CServerDlg::OnAutoSend)
	ON_BN_CLICKED(IDC_BUTTON_ClEAR, &CServerDlg::OnClear)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_RECV_TEXT, &CServerDlg::OnBnClickedRadioRecvText)
	ON_BN_CLICKED(IDC_RADIO_RECV_HEX, &CServerDlg::OnBnClickedRadioRecvHex)
	ON_BN_CLICKED(IDC_RADIO_SEND_TEXT, &CServerDlg::OnBnClickedRadioSendText)
	ON_BN_CLICKED(IDC_RADIO_SEND_HEX, &CServerDlg::OnBnClickedRadioSendHex)
END_MESSAGE_MAP()


BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	pServer = this;
	g_cstrIP = InitEditText();
	
	m_list_cli.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_cli.InsertColumn(0,"�ͻ��˵�ַ",LVCFMT_CENTER,120,0);
	m_list_cli.InsertColumn(1,"״̬",LVCFMT_CENTER,100,1);

	this->OnInitComDate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


// CServerDlg ��Ϣ�������
DWORD WINAPI ThreadTCPServer(LPVOID lpParameter)
{
	DWORD dwPort = 8888;
	int iRet = g_TCP.StartTCPServer(dwPort,pServer->m_hWnd);
	return 0;//����߳�ʲôʱ���˳�
}

DWORD WINAPI ThreadUDPServer(LPVOID lpParameter)
{
	DWORD dwPort = UDP_SERVER;
	int iRet = g_UDP.StartUDPServer(dwPort,pServer->m_hWnd);

	return 0;
}

DWORD WINAPI OnListen(LPVOID  lpvThreadParam)
{
	g_BTH.ListenBth();
	return 0;
}

DWORD WINAPI OnRecieve(LPVOID lpParameter)
{
	g_BTH.RecvBth();
	return 0;
}

DWORD WINAPI ThreadBTHServer(LPVOID lpParameter)
{	
	g_BTH.InitBT();
	pServer->ChangeEditList();
	BTUserList RemoteBTH;
	pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
	pServer->m_edit_tip.ReplaceSel("I>���ڻ�ȡ��������,���Ժ�......\r\n");
	//�˴�Ӧ���ý��治�ɲٿأ����Ľ�
	g_BTH.SearchRemoteBTNew(RemoteBTH);
	pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
	pServer->m_edit_tip.ReplaceSel("I>��ȡ�ɹ�\r\n");
	int iCount = RemoteBTH.size();
	for (int i = 0;i < iCount;i++)
	{
		int iIndex = RemoteBTH[i].Find('(');
		CString cstrBTHName = RemoteBTH[i].Left(iIndex);
		CString cstrBTHAdress = RemoteBTH[i].Mid(iIndex);
		pServer->m_list_cli.InsertItem(i,cstrBTHName);
		pServer->m_list_cli.SetItemText(i,1,cstrBTHAdress);
		pServer->m_list_cli.SetItemText(i,2,"������");
	}
	int iRect = g_BTH.StartUpLocalBT(g_BTH.MyBthSocketSer);
	if (iRect == 0)
	{
		pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
		pServer->m_edit_tip.ReplaceSel("I>����������ʼ����......\r\n");
	}

	bThread0 = TRUE;
	g_hThread_BTHServer_Listen = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OnListen,NULL,0,0);
	CloseHandle(g_hThread_BTHServer_Listen);
	Sleep(500);

	bThread1 = TRUE;
	g_hThread_BTHServer_Receive = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OnRecieve,NULL,0,0);
	CloseHandle(g_hThread_BTHServer_Receive);

	return 0;
}

DWORD WINAPI ThreadTCPServerCom(LPVOID lpParameter)
{
	g_UDP.StartUDPServerCom(COM_DATE,pServer->m_hWnd);
	return 0;
}

DWORD WINAPI ThreadUDPServerCom(LPVOID lpParameter)
{
	g_UDP.StartUDPServerCom(COM_DATE,pServer->m_hWnd);
	return 0;
}

//����Э��ѡ���׽��֣����������߳�
void CServerDlg::OnButtonStartserver()
{
	CString cstrTemp;
	m_btn_openserver.GetWindowText(cstrTemp);
	if (cstrTemp == "����������")
	{
		if (g_protocol == TCP_MODE)
		{
			g_hThread_TCPServer = (HWND)::CreateThread(NULL,0,ThreadTCPServer,NULL,0,NULL);
			CloseHandle(g_hThread_TCPServer);
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel("I>�����������ɹ�!\r\n");
			g_hThread_TCPServerCom = (HWND)::CreateThread(NULL,0,ThreadTCPServerCom,NULL,0,NULL);
			CloseHandle(g_hThread_TCPServerCom);
			m_btn_openserver.SetWindowText("�رշ�����");
		}
		else if (g_protocol == UDP_MODE)
		{
			g_hThread_UDPServer = (HWND)::CreateThread(NULL,0,ThreadUDPServer,NULL,0,NULL);
			CloseHandle(g_hThread_UDPServer);
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel("I>�����������ɹ�!\r\n");
			g_hThread_UDPServerCom = (HWND)::CreateThread(NULL,0,ThreadUDPServerCom,NULL,0,NULL);
			CloseHandle(g_hThread_UDPServerCom);
			m_btn_openserver.SetWindowText("�رշ�����");
		}
		else
		{
			g_hThread_BTHServer = (HWND)::CreateThread(NULL,0,ThreadBTHServer,NULL,0,NULL);
			CloseHandle(g_hThread_BTHServer);
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel("I>�����������ɹ�!\r\n");
			m_btn_openserver.SetWindowText("�رշ�����");
		}
	}
	else
	{
		if (g_protocol == TCP_MODE)
		{
			g_TCP.CloseSer();
			m_btn_openserver.SetWindowText("����������");
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel("I>�������ѹر�!\r\n");
			ClearAll();
		}
		else if (g_protocol == UDP_MODE)
		{
			g_TCP.CloseSer();
			m_btn_openserver.SetWindowText("����������");
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel("I>�������ѹر�!\r\n");
			ClearAll();
		}
		else
		{
			g_TCP.CloseSer();
			ChangeList(0);
			m_btn_openserver.SetWindowText("����������");
			m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
			m_edit_tip.ReplaceSel("I>�������ѹر�!\r\n");
			ClearAll();
		}
	}
	
}
DWORD WINAPI ThreadTCPRecvFile(LPVOID lpParameter)
{
	AfxSocketInit();
	CSocket fSocket;
	CSocket m_MyTcpServerFile;
	if (!m_MyTcpServerFile.Socket())
	{
		char szError[256] = {0};
		sprintf_s(szError, "Create Faild: %d", GetLastError());
		AfxMessageBox(szError);
		return 1; 
	}

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);

	m_MyTcpServerFile.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);

	CString strpath("");
	CString strname("");

	if (!m_MyTcpServerFile.Bind(8880))
	{
		char szError[256] = {0};
		sprintf_s(szError, "Bind Faild: %d", GetLastError());
		AfxMessageBox(szError);
		return 1; 
	}

	if(!m_MyTcpServerFile.Listen(10))
	{   
		char szError[256] = {0};
		sprintf_s(szError, "Listen Faild: %d", GetLastError());
		AfxMessageBox(szError);
		return 1;
	}

	m_MyTcpServerFile.Accept(fSocket);

	WIN32_FIND_DATA FileInfo;
	fSocket.Receive(&FileInfo,sizeof(WIN32_FIND_DATA));

	strname.Format("%s",FileInfo.cFileName);
	strpath="E://";

	CFile file;
	if(!file.Open(strpath+strname,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("�ļ���ʧ�ܣ��Ѵ���");
		return -1;
	}

	//Receive�ļ�������
	UINT nSize = 0;
	UINT nData = 0;

	char szBuff[2048];
	while(nSize<FileInfo.nFileSizeLow)
	{
		memset(szBuff,0x00,2048);
		nData=fSocket.Receive(szBuff,2048);
		file.Write(szBuff,nData);
		nSize+=nData;
	}
	fSocket.Close();
	file.Close();
	SYSTEMTIME t; //ϵͳʱ��ṹ 
	GetLocalTime(&t);
	CString strinfo;
	pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
	strinfo.Format("I>�ļ����ճɹ�..������E����.\r\nI>ʱ�䣺%d��%d��%d�� %2d:%2d:%2d \r\n",t.wYear,t.wMonth,t.wDay,
		t.wHour,t.wMinute,t.wSecond);
	pServer->m_edit_tip.ReplaceSel(strinfo);

	m_MyTcpServerFile.Close();

	return 0;
}

LRESULT CServerDlg::OnTCPServer(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
	case FD_ACCEPT:
		{
			CString cstrIP = g_TCP.AcceptCli();
			ChooseSelect(cstrIP,1);
			SetListText(cstrIP,0);
			SetListInfo(wParam,cstrIP,NULL,0);
			break;
		} 
	case FD_READ:
		{
			//�������ݺ����IPͷ	
			CString cstrTemp = g_TCP.ReadCli(wParam);
			int iIndex1 = cstrTemp.Find("%");
			int iIndex4 = cstrTemp.Find("@");
			int iIndexStop = cstrTemp.Find("ASC");
			CString cstrIP,cstrInfo;
			if(iIndex1 != -1)
			{
				int iIndex3 = cstrTemp.Find('$');
				cstrIP = cstrTemp.Left(iIndex3);
				cstrInfo = cstrTemp.Mid(iIndex1+1);
				
				int iIndeX1 = cstrInfo.Find("#");
				CString cstrText = cstrInfo.Left(iIndeX1);
				int iSend = _ttoi(cstrInfo.Mid(iIndeX1+1,1));
				int iRecv = _ttoi(cstrInfo.Right(1));
				
				if (iSend == 1)
				{
					m_com_send_mode.SetCheck(TRUE);
					m_com_send_mode_hex.SetCheck(FALSE);
				}
				if (iSend == 0)
				{
					m_com_send_mode.SetCheck(FALSE);
					m_com_send_mode_hex.SetCheck(TRUE);
				}
				if (iRecv == 1)
				{
					m_com_recv_mode.SetCheck(TRUE);
					m_com_recv_mode_hex.SetCheck(FALSE);

				}
				if (iRecv == 0)
				{
					m_com_recv_mode.SetCheck(FALSE);
					m_com_recv_mode_hex.SetCheck(TRUE);
				}


				m_richedit_send_com.SetWindowText(cstrText);
				SetListInfo(wParam,cstrIP,cstrText,6);
				this->OnBnClickedButtonManualsend();
				return 0;
			}
			else if (iIndex4 != -1)
			{
				int iIndex3 = cstrTemp.Find('$');
				cstrIP = cstrTemp.Left(iIndex3);
				cstrInfo = cstrTemp.Mid(iIndex4+1);
				m_richedit_send_com.SetWindowText(cstrInfo);
				CString cstrCount = cstrTemp.Mid(iIndex3+1,iIndex4-iIndex3-1);

				SetListInfo(wParam,cstrIP,cstrInfo,6);
				m_edit_autosend.SetWindowText(cstrCount);
				m_strRXData.Empty();
				this->OnAutoSend();
				return 0;

			}
			else if(iIndexStop != -1)
			{
				KillTimer(1);
				SetDlgItemText(IDC_BUTTON_AUTOSEND,"�Զ�����");
				return 0;
			}
			else
			{
				int iIndex2 = cstrTemp.Find('$');
				cstrIP = cstrTemp.Left(iIndex2);
				cstrInfo = cstrTemp.Mid(iIndex2+1);
			}
			SetListText(cstrIP,1);
			if ("SENDFILE" == cstrInfo)
			{
				g_hThread_TCPRecvFile = (HWND)::CreateThread(NULL,0,ThreadTCPRecvFile,NULL,0,NULL);
				CloseHandle(g_hThread_TCPRecvFile);
				SetListInfo(wParam,cstrIP,NULL,5);
			}
			else
				SetListInfo(wParam,cstrIP,cstrInfo,1);
			break;
		}
	case FD_CLOSE:
		{
			CString cstrIP = g_TCP.CloseCli(wParam);
			ChooseSelect(cstrIP,0);
			SetListText(cstrIP,2);
			SetListInfo(wParam,cstrIP,NULL,2);
			break;
		}
	}
	return 0;
}

//��ʼ��ʾ��������IP
CString CServerDlg::InitEditText()
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
		if(((CString)psz).Find("192.168.1") == -1)//���˷Ǿ�����IP
		{
			//cstrIP = "";
		}
		else
			cstrIP = psz;
	}
	
	g_UDP.cstrServerIP = cstrIP;
	CString cstrTip;
	cstrTip.Format("I>����IP��ַ��%s    ��������%s\r\nI>ʹ��TCPЭ��\r\n",cstrIP,cstrHostName);
	m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
	m_edit_tip.ReplaceSel(cstrTip);
	WSACleanup();

	return cstrIP;
}

//�����б���Ϣ
void CServerDlg::SetListText(CString  cstrIP,int iMode)
{
	int iCount = m_list_cli.GetItemCount();
	int i = 0;
	while(i < iCount)
	{
		if( cstrIP == m_list_cli.GetItemText(i,0) )
		{
			switch(iMode)
			{
			case 0:
				m_list_cli.SetItemText(i,1,"������");
				break;
			case 1:
				m_list_cli.SetItemText(i,1,"������");
				break;
			case 2:
				m_list_cli.DeleteItem(i);
				break;
			}			
			return;
		}
		i++;
	}

	if(2 != iMode)
	{
		m_list_cli.InsertItem(i,cstrIP);
		m_list_cli.SetItemText(i,1,"������");
	}
}


void CServerDlg::SetListInfo(SOCKET socketcli,CString  cstrIP,CString cstrInfo,int iMode)
{
	m_richedit_net.SetSel(m_richedit_net.GetWindowTextLength(),
		m_richedit_net.GetWindowTextLength());
	CString cstrTemp;

	CHARFORMAT cf;
	ZeroMemory(&cf,sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.dwEffects = 0;
	cf.yHeight = 80;
	cf.crTextColor = RGB(255,0,0);

	int iCount = m_richedit_net.GetLineCount();
	int iCount_ip = cstrIP.GetLength();
	int iLineStart = m_richedit_net.LineIndex(iCount - 1) ;
	int iLineEnd = m_richedit_net.LineIndex(iCount -1) + iCount_ip;

	if(iMode == 0)
	{
		cstrTemp.Format("%s ������\n",cstrIP);
		g_TCP.GroupSend(socketcli,cstrIP,cstrTemp);
	}
	else if(iMode == 1)
	{
		cstrTemp.Format("%s ������ͨ��Ϣ��\n%s\n",cstrIP,cstrInfo);
		g_TCP.GroupSend(socketcli,cstrIP,cstrTemp);
	}
	else if(iMode == 2)
	{
		cstrTemp.Format("%s �Ͽ�����\n",cstrIP);
		g_TCP.GroupSend(socketcli,cstrIP,cstrTemp);
	}
	else if(iMode == 3)
	{
		cstrTemp.Format("�� ������ͨ��Ϣ��\n%s\n",cstrInfo);
		CString cstrTemp1;
		cstrTemp1.Format("%s ������ͨ��Ϣ��\n%s\n",g_cstrIP,cstrInfo);
		g_TCP.GroupSend(g_TCP.m_MyTcpServer,g_cstrIP,cstrTemp1);
		iLineStart = m_richedit_net.LineIndex(iCount - 1) ;
		iLineEnd = m_richedit_net.LineIndex(iCount -1) + 2;
		cf.crTextColor = RGB(0,255,0);
	}
	else if(iMode == 4)
	{
		cstrTemp.Format("%s ���봮��ͨ��\n",cstrIP);
		g_TCP.GroupSend(socketcli,cstrIP,cstrTemp);
		cf.crTextColor = RGB(0,255,0);
	}
	else if(iMode	== 5)
	{
		cstrTemp.Format("%s ���ڷ����ļ�\n",cstrIP);
		g_TCP.GroupSend(socketcli,cstrIP,cstrTemp);
		cf.crTextColor = RGB(0,255,0);
	}
	else if (iMode == 6)
	{
		cstrTemp.Format("%s ���ʹ�������\n  %s\n",cstrIP,cstrInfo);
		g_TCP.GroupSend(socketcli,cstrIP,cstrTemp);
		cf.crTextColor = RGB(0,255,0);
	}
	m_richedit_net.ReplaceSel(cstrTemp);

	//0~0��δѡ��
	m_richedit_net.SetSel(iLineStart,iLineEnd);
	m_richedit_net.SetSelectionCharFormat(cf);
	//�Զ���ֱ����
	m_richedit_net.LineScroll(m_richedit_net.GetLineCount());

}

void CServerDlg::SetListInfoUDP(CString& cstrIP,CString cstrInfo,int iMode)
{
	m_richedit_net.SetSel(m_richedit_net.GetWindowTextLength(),
		m_richedit_net.GetWindowTextLength());
	CString cstrTemp;

	CHARFORMAT cf;
	ZeroMemory(&cf,sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.dwEffects = 0;
	cf.yHeight = 80;
	cf.crTextColor = RGB(255,0,0);

	int iCount = m_richedit_net.GetLineCount();
	int iCount_ip = cstrIP.GetLength();
	int iLineStart = m_richedit_net.LineIndex(iCount - 1) ;
	int iLineEnd = m_richedit_net.LineIndex(iCount -1) + iCount_ip;

	switch(iMode)
	{
	case 0:
		{
			cstrTemp.Format("%s ������\n",cstrIP);
			g_UDP.GroupSend(cstrIP,cstrTemp);
			break;
		}
	case 1:
		{
			cstrTemp.Format("%s ������ͨ��Ϣ��\n%s\n",cstrIP,cstrInfo);
			g_UDP.GroupSend(cstrIP,cstrTemp);
			break;
		}
	case 2:
		{
			cstrTemp.Format("%s �����ļ�\n",cstrIP);
			g_UDP.GroupSend(cstrIP,cstrTemp);
			break;
		}
	case 3:
		{
			cstrTemp.Format("%s ���봮��ͨ��\n",cstrIP);
			g_UDP.GroupSend(cstrIP,cstrTemp);
			break;
		}
	case 4:
		{
			cstrTemp.Format("%s �Ͽ�����\n",cstrIP);
			g_UDP.GroupSend(cstrIP,cstrTemp);
			break;
		}
	case 5:
		{
			cstrTemp.Format("%s ���ʹ�������\n  %s\n",cstrIP,cstrInfo);
			g_UDP.GroupSend(cstrIP,cstrTemp);
		}
	case 6:
		{
			cstrTemp.Format("%s ���ʹ�������\n  %s\n",cstrIP,cstrInfo);
			g_UDP.GroupSend(cstrIP,cstrTemp);
		}
	}

	m_richedit_net.ReplaceSel(cstrTemp);

	//0~0��δѡ��
	m_richedit_net.SetSel(iLineStart,iLineEnd);
	m_richedit_net.SetSelectionCharFormat(cf);
	//�Զ���ֱ����
	m_richedit_net.LineScroll(m_richedit_net.GetLineCount());
}


void CServerDlg::SetListInfoBTH(CString cstrSend,int iMode)
{
	g_BTH.SendMessageBTSer(cstrSend);

	CHARFORMAT cf;
	ZeroMemory(&cf,sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.dwEffects = 0;
	cf.yHeight = 80;
	cf.crTextColor = RGB(255,0,0);

	CString cstrTemp;
	m_richedit_net.SetSel(m_richedit_net.GetWindowTextLength(),m_richedit_net.GetWindowTextLength());

	if (iMode  == 0)
	{
		cstrTemp.Format("�� ˵:\n%s\n",cstrSend);
	}
	else if(iMode == 1)
	{
		cstrTemp.Format("%s ���봮��ͨ��\r\n",cstrBTHName);
	}
	else if (iMode == 2)
	{
		cstrTemp.Format("%s �Ͽ�����\r\n",cstrBTHName);
	}
	m_richedit_net.ReplaceSel(cstrTemp);

	//0~0��δѡ��
	m_richedit_net.SetSel(0,2);
	m_richedit_net.SetSelectionCharFormat(cf);
	//�Զ���ֱ����
	m_richedit_net.LineScroll(m_richedit_net.GetLineCount());

}

void CServerDlg::OnButtonNetSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstrSend;
	m_edit_net_send.GetWindowText(cstrSend);
	if(cstrSend.IsEmpty())
	{
		AfxMessageBox("�������ݲ���Ϊ�գ�");
		return;
	}
	if(g_protocol == TCP_MODE)
		this->SetListInfo(g_TCP.m_MyTcpServer,g_TCP.cstrServerIP,cstrSend,3);
	else if (g_protocol ==UDP_MODE)
	{
		this->SetListInfoUDP(g_UDP.cstrServerIP,cstrSend,1);
	}
	else
		this->SetListInfoBTH(cstrSend);

	m_edit_net_send.SetWindowText("");
}


void CServerDlg::ChooseSelect(CString cstrIP,int iMode)
{
	if(0 == iMode)
	{
		int iItem = 0;
		while((iItem = m_comb_channel.FindString(iItem,cstrIP)) != CB_ERR)
		{
			m_comb_channel.DeleteString(iItem);
		}
	}
	else if(1 == iMode)
	{
		int iCount = m_comb_channel.GetCount();
		m_comb_channel.InsertString(iCount,cstrIP);
	}
}
void CServerDlg::OnchangeComboChannel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iSel = m_comb_channel.GetCurSel();
	int iLength = m_comb_channel.GetLBTextLen(iSel);
	CString cstrSel;
	m_comb_channel.GetLBText(iSel,cstrSel.GetBuffer(iLength));
	
	if (g_protocol == TCP_MODE)
	{
		SOCKET socktemp = g_TCP.SendComSel(cstrSel);
		SetListInfo(socktemp,cstrSel,NULL,4);
	}
	else	if (g_protocol == UDP_MODE)
	{
		CString cstrTempIP = g_UDP.SendComSel(cstrSel);
		SetListInfoUDP(cstrTempIP,NULL,3);
	}
	else
	{
		g_BTH.SendMessageBTSer(CString("a"),1);
		SetListInfoBTH(NULL,1);
	}

}

DWORD WINAPI ThreadUDPRecvFile(LPVOID lpParameter)
{
	AfxSocketInit();
	CSocket fSocket;
	
	if (!fSocket.Create(UDP_FILE,SOCK_DGRAM))
	{
		char szError[256] = {0};
		sprintf_s(szError, "Create Faild: %d", GetLastError());
		AfxMessageBox(szError);
		return 1; 
	}

	CString strpath("");
	CString strname("");
	
	UINT port = (unsigned int)UDP_FILE;
	WIN32_FIND_DATA FileInfo;
	fSocket.ReceiveFrom(&FileInfo,sizeof(WIN32_FIND_DATA),cstrFileIP,port);

	strname.Format("%s",FileInfo.cFileName);
	strpath="D://";

	CFile file;
	if(!file.Open(strpath+strname,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("�ļ���ʧ�ܣ��Ѵ���");
		return -1;
	}

	//Receive�ļ�������
	UINT nSize = 0;
	UINT nData = 0;

	char szBuff[2048];
	while(nSize<FileInfo.nFileSizeLow)
	{
		memset(szBuff,0x00,2048);
		nData=fSocket.ReceiveFrom(szBuff,2048,CString("192.168.1.194"),port);
		file.Write(szBuff,nData);
		nSize+=nData;
	}
	fSocket.Close();
	file.Close();
	SYSTEMTIME t; //ϵͳʱ��ṹ 
	GetLocalTime(&t);
	CString strinfo;
	pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
	strinfo.Format("I>�ļ����ճɹ�..������E����.\r\nI>ʱ�䣺%d��%d��%d�� %2d:%2d:%2d \r\n",t.wYear,t.wMonth,t.wDay,
		t.wHour,t.wMinute,t.wSecond);
	pServer->m_edit_tip.ReplaceSel(strinfo);

	return 0;
}

//UDPģʽ�޷��жϿͻ��˵�״̬������or�Ͽ������ֻ���¼�FD_READ
LRESULT CServerDlg::OnUDPServer(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
	case FD_READ://��ִ�������ܽ�case,UDP���ܸ���wParam���IP
		{
			CString cstrRecv = g_UDP.ReadCli();
			int index = cstrRecv.Find('$');
			CString cstrIP = cstrRecv.Left(index);
			CString cstrInfo = cstrRecv.Mid(index+1);

			int iIndex1 = cstrInfo.Find("%");
			int iIndex4 = cstrInfo.Find("@");
			int iIndexStop = cstrInfo.Find("ASC");

			if ( g_UDP.SockCliIPList.IsEmpty())
			{
				 g_UDP.SockCliIPList.AddTail(cstrIP);
			}
			else
			{
				POSITION pos = g_UDP.SockCliIPList.GetHeadPosition();
				while( pos != NULL)
				{
					 CString cstrTemp =  g_UDP.SockCliIPList.GetNext(pos);
					 if (cstrTemp != cstrIP)
					 {
						 g_UDP.SockCliIPList.AddTail(cstrIP);
					 }
				}
			}

			if ( cstrInfo == "TS")
			{
				SetListText(cstrIP,0);
			    SetListInfoUDP(cstrIP,NULL,0);
				ChooseSelect(cstrIP,1);
			}
			else if(cstrInfo == "SENDFILE")
			{
				cstrFileIP = cstrIP;
				g_hThread_UDPRecvFile = (HWND)::CreateThread(NULL,0,ThreadUDPRecvFile,NULL,0,NULL);
				CloseHandle(g_hThread_UDPRecvFile);
				SetListInfoUDP(cstrIP,NULL,2);
			}
			else if (cstrInfo == "DISCONN")
			{
				SetListText(cstrIP,2);
				SetListInfoUDP(cstrIP,NULL,4);
				ChooseSelect(cstrIP,0);
			}
			else if(iIndex1 != -1)
			{
				cstrInfo = cstrInfo.Mid(iIndex1+1);

				int iIndeX1 = cstrInfo.Find("#");
				CString cstrText = cstrInfo.Left(iIndeX1);
				int iSend = _ttoi(cstrInfo.Mid(iIndeX1+1,1));
				int iRecv = _ttoi(cstrInfo.Right(1));

				if (iSend == 1)
				{
					m_com_send_mode.SetCheck(TRUE);
					m_com_send_mode_hex.SetCheck(FALSE);
				}
				if (iSend == 0)
				{
					m_com_send_mode.SetCheck(FALSE);
					m_com_send_mode_hex.SetCheck(TRUE);
				}
				if (iRecv == 1)
				{
					m_com_recv_mode.SetCheck(TRUE);
					m_com_recv_mode_hex.SetCheck(FALSE);

				}
				if (iRecv == 0)
				{
					m_com_recv_mode.SetCheck(FALSE);
					m_com_recv_mode_hex.SetCheck(TRUE);
				}

				m_richedit_send_com.SetWindowText(cstrText);
				SetListInfoUDP(cstrIP,cstrText,5);
				this->OnBnClickedButtonManualsend();
				return 0;
			}
			else if (iIndex4 != -1)
			{
				cstrInfo = cstrInfo.Mid(iIndex4+1);
				m_richedit_send_com.SetWindowText(cstrInfo);
				CString cstrCount = cstrInfo.Left(iIndex4);

				SetListInfoUDP(cstrIP,cstrInfo,6);
				m_edit_autosend.SetWindowText(cstrCount);
				m_strRXData.Empty();
				this->OnAutoSend();
				return 0;
			}
			else if(iIndexStop != -1)
			{
				KillTimer(1);
				SetDlgItemText(IDC_BUTTON_AUTOSEND,"�Զ�����");
				return 0;
			}
			else
			{
				SetListText(cstrIP,1);
				SetListInfoUDP(cstrIP,cstrInfo,1);
			}
			break;
		}
	}
	return 0;
}

void CServerDlg::ChangeList(int iMode)
{
	if (iMode == 0)
	{	//ɾ��������ͷ������
		int nCols = m_list_cli.GetHeaderCtrl()->GetItemCount();
		for (int j = 0;j < nCols;j++)
		{
			m_list_cli.DeleteColumn(0);
		}
		SetDlgItemText(IDC_STATIC_TEXT,"���������б�(�����绰������)");
		m_list_cli.DeleteAllItems();
		m_list_cli.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	//	if (pRemoteCommDlg->JudgeWindowsVersion())
	//	{
			m_list_cli.InsertColumn(0,"����������ַ",LVCFMT_CENTER,116,0);
			m_list_cli.InsertColumn(1,"������",LVCFMT_CENTER,113,1);
	//		m_list_cli.InsertColumn(2,"״̬",LVCFMT_CENTER,60,1);
	//	}
	//	else
	//	{
	//		m_list_cli.InsertColumn(0,"����������(���������ƶ��豸)",LVCFMT_CENTER,170,0);
	//		m_list_cli.InsertColumn(1,"״̬",LVCFMT_CENTER,60,1);
	//	}
	}
	if(iMode == 1)
	{
		int nCols = m_list_cli.GetHeaderCtrl()->GetItemCount();
		for (int j = 0;j < nCols;j++)
		{
			m_list_cli.DeleteColumn(0);
		}
		SetDlgItemText(IDC_STATIC_TEXT,"�����ӿͻ���");
		m_list_cli.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
		m_list_cli.InsertColumn(0,"�ͻ��˵�ַ",LVCFMT_CENTER,120,0);
		m_list_cli.InsertColumn(1,"״̬",LVCFMT_CENTER,100,1);
	}
}

void CServerDlg::ChangeEditList()
{
	BTUserList LocalBTHList;
	g_BTH.SearchLocalBT(LocalBTHList);
	int iCount = LocalBTHList.size();
	for (int i = 0;i < iCount;i++)
	{
		m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
		int iIndex = LocalBTHList[i].Find('(');
		CString cstrBTHName = LocalBTHList[i].Left(iIndex);
		CString cstrBTHAdress = LocalBTHList[i].Mid(iIndex);
		CString cstrTemp;
		cstrTemp.Format("I>������������:%s \r\nI>����������ַ:%s\r\n",cstrBTHName,cstrBTHAdress);
		m_edit_tip.ReplaceSel(cstrTemp);
	}
}


BEGIN_EVENTSINK_MAP(CServerDlg, CDialog)
	ON_EVENT(CServerDlg, IDC_MSCOMM1, 1, CServerDlg::OnComm, VTS_NONE)
END_EVENTSINK_MAP()

//�������ݻ�������������
void CServerDlg::OnComm()
{
	// TODO: �ڴ˴������Ϣ����������
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len,k;
	BYTE rxdata[2048]; //����BYTE���� An 8-bit integerthat is not signed.
	CString strtemp;
	if(m_ctrlComm.get_CommEvent()==2) //�¼�ֵΪ2��ʾ���ջ����������ַ�
	{       
		variant_inp=m_ctrlComm.get_Input(); //��������
		safearray_inp=variant_inp; //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		len=safearray_inp.GetOneDimSize(); //�õ���Ч���ݳ���
		g_Recv_Count += len;
		SetDlgItemInt(IDC_EDIT_RECVCOUNT,g_Recv_Count);
		for(k=0;k<len;k++)
			safearray_inp.GetElement(&k,rxdata+k);//ת��ΪBYTE������
		for(k=0;k<len;k++) //������ת��ΪCstring�ͱ���
		{
			BYTE bt=*(char*)(rxdata+k); //�ַ���
			if (m_com_recv_mode.GetCheck() == TRUE)
			{
				strtemp.Format("%c",bt);//תΪ�ַ����ͱ���
				m_strRXData += strtemp;//�ַ�������ܱ༭��
			}
			else
			{
				strtemp.Format("%02X ",bt); 
				m_strRXData += strtemp;//�ַ�������ܱ༭��
			}
		}
		if (g_protocol == BLUETOOTH_MODE)
		{
			g_BTH.SendMessageBTSer(m_strRXData,3);
		}
	}
	m_richedit_recv_com.SetWindowText(m_strRXData); //���±༭������
	if (m_strRXData.GetLength() > 800)//Ĭ�϶�ʱ����
	{
		m_strRXData.Empty();
	}
}

//��ʼ����������
void	CServerDlg::OnInitComDate()
{

	m_combo_com.SetCurSel(0);
	m_comb_baud.SetCurSel(5);
	m_comb_date.SetCurSel(2);
	m_comb_stop.SetCurSel(0);
	m_comb_check.SetCurSel(1);

	m_com_recv_mode.SetCheck(TRUE);
	m_com_send_mode.SetCheck(TRUE);

	SetDlgItemInt(IDC_EDIT_SENDCOUNT,g_Send_Count);
	SetDlgItemInt(IDC_EDIT_RECVCOUNT,g_Recv_Count);

	m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
	m_edit_tip.ReplaceSel("I>�������ݳ�ʼ��....\r\n");
}


//�򿪴���
void CServerDlg::OnBnClickedButtonOpencom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstrText;
	m_btn_opencom.GetWindowText(cstrText);
	if (cstrText == "�򿪴���")
	{
		int iComIndex = m_combo_com.GetCurSel();
		int iBarudIndex = m_comb_baud.GetCurSel();
		int iDateIndex = m_comb_date.GetCurSel();
		int iStopIndex = m_comb_stop.GetCurSel();
		int iCheckIndex = m_comb_check.GetCurSel();

		int iCom = iComIndex + 1;
		int iBarud = Baurd[iBarudIndex];
		int iDate = Date[iDateIndex];
		int iStop = iStopIndex + 1;

		BOOL bRecvMode = recv_mode;
		BOOL bSendMode = send_mode;

		//E - 0 N - 1 O - 2
		if (m_ctrlComm.get_PortOpen())
			m_ctrlComm.put_PortOpen(FALSE);

		m_ctrlComm.put__CommPort(iCom);
		m_ctrlComm.put_InputMode(1);
		m_ctrlComm.put_InBufferSize(1024);
		m_ctrlComm.put_OutBufferSize(512);

		CString cstrSettings;
		cstrSettings.Format("%d,%c,%d,%d",iBarud,check[iCheckIndex],iDate,iStop);
		m_ctrlComm.put_Settings(cstrSettings);
		if (!m_ctrlComm.get_PortOpen())
			m_ctrlComm.put_PortOpen(TRUE);
		else
			AfxMessageBox("���ܴ򿪸�COM!");

		m_ctrlComm.put_RThreshold(1);
		m_ctrlComm.put_InputLen(0);
		m_ctrlComm.get_Input();

		m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
		m_edit_tip.ReplaceSel("I> ���ڴ򿪳ɹ�!\r\n");
		
		m_btn_opencom.SetWindowText("�رմ���");

		CString cstrSendComInfo;
		cstrSendComInfo.Format("$%d&%d&%d&%d&%d&%d&%d",iComIndex,iBarudIndex,iDateIndex,iStopIndex,iCheckIndex,bRecvMode,bSendMode);	
		//�򿪴���ʱת����������
		if(g_protocol == TCP_MODE)
		{
			g_TCP.GroupSend(g_TCP.m_MyTcpServer,g_cstrIP,cstrSendComInfo);
		}
		else if (g_protocol == UDP_MODE)
		{
			g_UDP.GroupSend(g_UDP.cstrServerIP,cstrSendComInfo);
		}
	}
	else if (cstrText == "�رմ���")
	{
		if (m_ctrlComm.get_PortOpen())
		{
			m_ctrlComm.put_PortOpen(FALSE);
		}
		m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
		m_edit_tip.ReplaceSel("I> �����ѹر�!\r\n");
		m_btn_opencom.SetWindowText("�򿪴���");
	}	
}

//��������
void CServerDlg::OnBnClickedButtonManualsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_ctrlComm.get_PortOpen())
		AfxMessageBox("��򿪴��ڣ�");
	else
	{
		m_richedit_send_com.GetWindowText(m_strTXData);
		g_Send_Count += m_strTXData.GetLength();
		SetDlgItemInt(IDC_EDIT_SENDCOUNT,g_Send_Count);
		if(m_com_send_mode.GetCheck() == TRUE)
			m_ctrlComm.put_Output(COleVariant(m_strTXData));
		else
		{
			CByteArray hexdate;//�յ��ֽ�����
			int len=SringHex(m_strTXData,hexdate);
			m_ctrlComm.put_Output(COleVariant(hexdate));
		}
	}
	if (g_protocol == TCP_MODE)
	{
		CString cstrSend;
		cstrSend.Format("���������ʹ�������: %s\r\n",m_strTXData);
		g_TCP.GroupSend(NULL,g_TCP.cstrServerIP,cstrSend);
	}
	else if (g_protocol == UDP_MODE)
	{
		CString cstrSend;
		cstrSend.Format("���������ʹ�������: %s\r\n",m_strTXData);
		g_UDP.GroupSend(g_UDP.cstrServerIP,cstrSend);
	}
	else if (g_protocol == BLUETOOTH_MODE)
	{
		CString cstrSend;
		cstrSend.Format("���������ʹ�������: %s\r\n",m_strTXData);
		g_BTH.SendMessageBTSer(cstrSend,2);
	}
	m_strTXData.Empty();
	m_richedit_send_com.SetWindowText("");
}	

//�ַ�תʮ������
char CServerDlg::ConvertHexChar(char ch)
{
	if((ch >= '0')&&(ch <' 9'))
	{
		return ch - 0x30;
	}
	else if((ch >= 'A')&&(ch = 'F'))
	{
		return ch - 'A' + 10;
	}
	else if((ch >= 'a')&&(ch <= 'f'))
	{
		return ch - 'a' + 10;
	}
	else
		return -1;
}

//ת����ʮ�������ַ���
int CServerDlg::SringHex(CString str,CByteArray &senddate)
{
	int hexdate,lowhexdate;
	int hexdatelen=0;
	int len=str.GetLength();
	senddate.SetSize(len/2);
	for(int i=0;i<len;)
	{
		char lstr,hstr=str[i];
		if(hstr == ' ')
		{
			i++;
			hstr=str[i];
			continue;       
		}                  
		i++;                 
		if(i>=len)
			break;
		lstr=str[i];
		hexdate=ConvertHexChar(hstr);
		lowhexdate=ConvertHexChar(lstr);
		if((hexdate == 16)||(lowhexdate == 16))
			break;
		else
			hexdate=hexdate*16+lowhexdate;
		i++;
		senddate[hexdatelen]=(char)hexdate;
		hexdatelen++;
	}
	senddate.SetSize(hexdatelen);
	return hexdatelen;
}

//ѡ��STC-ISP·��
void CServerDlg::OnChooseSTC()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDialog(TRUE);
	if(fileDialog.DoModal() == IDOK)
	{
		strExeName = fileDialog.GetPathName();
	} 
	CString cstrTemp;
	cstrTemp.Format("I>��ѡ�����¼���Ϊ��\r\nI>%s\r\n",strExeName);
	m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
	m_edit_tip.ReplaceSel(cstrTemp);
}

//����¼���
void CServerDlg::OnDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstrAppName = "STC-ISP (V6.86O) (���۵绰: 0513-55012928) ����:www.STCMCUDATA.com  (����֧��QQ:800003751) ���������: 6000Ԫ(��������) -- STC: ȫ������8051��Ƭ����ƹ�˾ (Ҧ��ƽ)";
	CWnd * pWnd = FindWindow(NULL,cstrAppName);
	if(pWnd == NULL)
	{
		::ShellExecute(NULL,"open",strExeName,NULL,NULL,SW_SHOW);
		Sleep(6000);
		pWnd = FindWindow(NULL,cstrAppName);
	}
/*
	if(pWnd != GetForegroundWindow())                      
	{
		pRemoteCommDlg->ShowWindow(SW_MINIMIZE);
		pWnd->ShowWindow(SW_RESTORE);                      
		HWND hCurWnd = NULL; 
		DWORD lMyID; 
		DWORD lCurID; 
		hCurWnd = ::GetForegroundWindow(); 
		lMyID = ::GetCurrentThreadId(); 
		lCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
		::AttachThreadInput(lMyID, lCurID, TRUE);          
		pWnd->SetForegroundWindow();                       
		::AttachThreadInput(lMyID, lCurID, FALSE);         
	}*/
	//pRemoteCommDlg->ShowWindow(SW_MINIMIZE);
//	if(!pWnd->IsZoomed())
	//	pWnd->ShowWindow(SW_SHOWMAXIMIZED);
	//int iResult = ::SetForegroundWindow(pWnd->GetSafeHwnd());


	//��ԭ
	if (::IsIconic(pWnd->GetSafeHwnd()))
	{
		::SendMessage(pWnd->GetSafeHwnd(), WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	// ����
	if (::GetActiveWindow() != pWnd->GetSafeHwnd())
	{
		::SetForegroundWindow(pWnd->GetSafeHwnd());
	}
	
	CWnd * pButtonWnd = FindWindowEx(pWnd->GetSafeHwnd(),NULL,"Button","�򿪳����ļ�");
	int iID = ::GetDlgCtrlID(pButtonWnd->m_hWnd);
	//ChangeWindowMessageFilter(BM_CLICK,MSGFLT_ADD);Win7�²���Ҫ���Ȩ��
	int iErr = ::PostMessage(pButtonWnd->GetSafeHwnd(),BM_CLICK,0,0);//ֱ�ӽ�����ȡ�ù���ԱȨ��
	CString cstrSend;
	cstrSend.Format("%d",iErr);
	TRACE(cstrSend);
}

void CServerDlg::OnClearRecv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_richedit_recv_com.SetWindowText("");
	m_strRXData.Empty();
}

void CServerDlg::OnClearSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_richedit_send_com.SetWindowText("");
	m_strTXData.Empty();
}

//������ܿ���Ϣ
void CServerDlg::OnSaveRecv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnSaveDate(0);

}

//���淢�Ϳ���Ϣ
void CServerDlg::OnSaveSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnSaveDate(1);
}

/*
Format�����ĸ�ʽ���ż������壺  
%a: ��д��������������Sat����Saturday  
%A: ȫд���ڣ�Ӣ�ģ�����Friday  
%b: ��д������������Mar����March  
%B: �·ݣ�ȫдӢ�ģ�����October  
%c: ��/��/�� ʱ:��:�룬��1/3/2012 21:19:11  
%d: ���е�������ֵΪ01��31  
%H: 24Сʱ��ʽ��Сʱ����ֵΪ00��23  
%I: ͨ����12Сʱ��ʽ��Сʱ����ֵΪ01��12  
%j: һ�굱�еĵڼ��죬ֵΪ001��336  
%m: ������ֵΪ01��12  
%M: ��������ֵΪ00��59  
%p: ��12Сʱ��ʽ��ʱ��ָʾ��am/pm(����/����)  
%S: ������ֵΪ00��59  
%U: ���е�������ֵΪ00��53��������Ϊһ�ܵĵ�һ��  
%w: ���е���������ֵΪ0��6������0Ϊ����  
%W: ���е�������ֵΪ00��53������һΪһ�ܵĵ�һ��  
%x: ��/��/�꣬%c��ǰ���  
%X: ʱ/��/�룬%c�ĺ���  
%y: �������ǰ׺��������ֵΪ00��99  
%Y: ��ݣ������ͣ���2006��  
%z: ��д��ʱ������  
%Z: ʱ�����Ƶ�ȫ�ƣ����ʱ��δ֪�����ַ�Ϊ�գ��硰�й���׼ʱ�䡱  
*/
//�����ļ�
void CServerDlg::OnSaveDate(int iMode)
{
	CString cstrDateTime;  
	CTime m_time = CTime::GetCurrentTime();             //��ȡ��ǰʱ������  
	cstrDateTime = m_time.Format("%Y%m%d _%H%M%S");   //��ʽ������ʱ��  
	CString cstrName;
	CString cstrDate;
	CString cstrTemp;
	if (iMode == 0)
	{
		 cstrName.Format("��������%s.txt",cstrDateTime);
		 m_richedit_recv_com.GetWindowText(cstrDate);
		 cstrTemp.Format("I>����%s���ݳɹ�!\r\n","����");
	}
	else
	{
		 cstrName.Format("��������%s.txt",cstrDateTime);
		 m_richedit_send_com.GetWindowText(cstrDate);
		 cstrTemp.Format("I>����%s���ݳɹ�!\r\n","����");
	}
	

	CFile file(cstrName, CFile::modeReadWrite | CFile::modeCreate);
	file.Write(cstrDate, cstrDate.GetLength());
	
	file.Close();
	m_edit_tip.SetSel(m_edit_tip.GetWindowTextLength(),m_edit_tip.GetWindowTextLength());
	m_edit_tip.ReplaceSel(cstrTemp);
}

void CServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_richedit_send_com.GetWindowText(m_strTXData);
	if (g_protocol == TCP_MODE)
	{

	}
	g_Send_Count += m_strTXData.GetLength();
	SetDlgItemInt(IDC_EDIT_SENDCOUNT,g_Send_Count);
	if(m_com_send_mode.GetCheck() == TRUE)
		m_ctrlComm.put_Output(COleVariant(m_strTXData));
	else
	{
		CByteArray hexdate;//�յ��ֽ�����
		int len=SringHex(m_strTXData,hexdate);
		m_ctrlComm.put_Output(COleVariant(hexdate));
	}
	m_strTXData.Empty();
	CDialog::OnTimer(nIDEvent);
}

void CServerDlg::OnAutoSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cstrTemp;
	GetDlgItemText(IDC_BUTTON_AUTOSEND,cstrTemp);
	if (cstrTemp == "�Զ�����")
	{
		int iTime = GetDlgItemInt(IDC_EDIT_AUTOTIME);
		if(iTime<=0)
			AfxMessageBox("�������Զ�����ʱ������");
		else if(!m_ctrlComm.get_PortOpen())
			AfxMessageBox("��򿪴��ڣ�");
		else
		{
			SetDlgItemText(IDC_BUTTON_AUTOSEND,"ֹͣ����");
			SetTimer(1,iTime,NULL);//����ϵͳ��ʱ��
		
			m_richedit_send_com.GetWindowText(m_strTXData);

			if (g_protocol == TCP_MODE)
			{
				CString cstrSend;
				cstrSend.Format("�������Զ����ʹ�������: %s,\r\n�����%d\r\n",m_strTXData,iTime);
				g_TCP.GroupSend(NULL,g_TCP.cstrServerIP,cstrSend);
			}
			else if (g_protocol == UDP_MODE)
			{
				CString cstrSend;
				cstrSend.Format("�������Զ����ʹ�������: %s,\r\n�����%d\r\n",m_strTXData,iTime);
				g_UDP.GroupSend(g_UDP.cstrServerIP,cstrSend);
			}
			else if (g_protocol == BLUETOOTH_MODE)
			{
				CString cstrSend;
				cstrSend.Format("�������Զ����ʹ�������: %s,\r\n�����%d\r\n",m_strTXData,iTime);
				g_BTH.SendMessageBTSer(cstrSend,2);
			}

		}
	}
	else if(cstrTemp == "ֹͣ����")
	{
		KillTimer(1);
		SetDlgItemText(IDC_BUTTON_AUTOSEND,"�Զ�����");
	}
	
}

void CServerDlg::OnClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_Recv_Count = 0;
	g_Send_Count = 0;
	SetDlgItemInt(IDC_EDIT_RECVCOUNT,g_Recv_Count);
	SetDlgItemInt(IDC_EDIT_SENDCOUNT,g_Send_Count);
}


void CServerDlg::OnPaint()
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


CString CServerDlg::GetBthName(CString cstrAdress)
{
	CString str;
	for(int i=0; i<m_list_cli.GetItemCount(); i++ )
	{
		str = m_list_cli.GetItemText(i,1);
		int iCount = str.GetLength();
		str = str.Mid(1,iCount-2);
		if (str == cstrAdress)
		{
			m_ConnectBth = m_list_cli.GetItemText(i,0);
			return m_list_cli.GetItemText(i,0);
		}
	}
}
void CServerDlg::OnBnClickedRadioRecvText()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	recv_mode = 1;
}

void CServerDlg::OnBnClickedRadioRecvHex()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	recv_mode = 0;
}

void CServerDlg::OnBnClickedRadioSendText()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	send_mode = 1;
}

void CServerDlg::OnBnClickedRadioSendHex()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	send_mode = 0;
}

void CServerDlg::ClearAll()
{
	this->m_comb_channel.SetCurSel(-1);
	m_comb_channel.ResetContent();
	pServer->m_richedit_net.Clear();
}