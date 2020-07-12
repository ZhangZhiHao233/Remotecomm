#include "StdAfx.h"
#include "MyUdp.h"
#include "MyTcp.h"
#include "ServerDlg.h"
#include "ClientDlg.h"
#include "RemoteCommDlg.h"
extern CMyTcp g_TCP;
extern CServerDlg* pServer;
extern CClientDlg* pClient;
extern CRemoteCommDlg* pRemoteCommDlg;

#pragma comment(lib,"ws2_32.lib")

extern int g_Recv_Count;
extern int g_Send_Count;
extern bool g_bflag;
extern bool g_bflagCli;
extern int g_protocol;
CMyUdp::CMyUdp(void)
	:m_MyUdpServer(INVALID_SOCKET)
{
}

CMyUdp::~CMyUdp(void)
{
}

int CMyUdp::StartUDPServer(DWORD &dwPort,HWND hWnd)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	this->m_MyUdpServer = WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,0);

	if(INVALID_SOCKET == m_MyUdpServer)
	{
		closesocket(m_MyUdpServer);
		AfxMessageBox("创建套接字失败!");
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN addrSer;
	addrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons((u_short)dwPort);

	int iRet1;
	iRet1= bind(m_MyUdpServer,(SOCKADDR*)&addrSer,sizeof(SOCKADDR));
	if(iRet1 == SOCKET_ERROR)
	{
		closesocket(m_MyUdpServer);
		AfxMessageBox("绑定套接字失败!");
		WSACleanup();
		return -1;
	}
	
	if(SOCKET_ERROR == WSAAsyncSelect(m_MyUdpServer,hWnd,WM_SOCKET_UDP,FD_ACCEPT | FD_READ |FD_CLOSE))
	{
		closesocket(m_MyUdpServer);
		AfxMessageBox("注册网络事件套接字失败!");
		WSACleanup();
		return -1;
	}
	return 0;
}

//串口数据发送线程
int CMyUdp::StartUDPServerCom(int dwPort,HWND hWnd)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	this->m_MyUdpServerCom = socket(AF_INET,SOCK_DGRAM,0);

	if(INVALID_SOCKET == m_MyUdpServerCom)
	{
		closesocket(m_MyUdpServerCom);
		AfxMessageBox("创建套接字失败!");
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN addrSer;
	addrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons((u_short)dwPort);

	int iRet1;
	iRet1= bind(m_MyUdpServerCom,(SOCKADDR*)&addrSer,sizeof(SOCKADDR));
	if(iRet1 == SOCKET_ERROR)
	{
		closesocket(m_MyUdpServerCom);
		AfxMessageBox("绑定套接字失败!");
		WSACleanup();
		return -1;
	}

	sockaddr_in	addrTo;
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons((u_short)dwPort);
	g_bflag = TRUE;

	if (g_protocol == TCP_MODE)
	{
		while(g_bflag)
		{//如果客户端列表不为空
			if (!pServer->m_strRXData.IsEmpty())
			{
				if(g_TCP.SockCliList.GetCount() > 0)
				{
					POSITION pos = g_TCP.SockCliList.GetHeadPosition();
					while( pos != NULL)
					{
						//获取该客户端IP地址
						SOCKET socketTemp;
						socketTemp =  g_TCP.SockCliList.GetAt(pos);
						CString cstrIPtemp = g_TCP.GetPeerName(socketTemp);
						CString sendCount,recvCount;
						sendCount.Format("%d",g_Send_Count);
						recvCount.Format("%d",g_Recv_Count);
						//除了服务端，就发送
						if(cstrIPtemp != g_TCP.cstrServerIP)
						{
							//addrTo.sin_addr.S_un.S_addr=htonl(inet_addr(cstrIPtemp.GetBuffer(cstrIPtemp.GetLength())));
							addrTo.sin_addr.S_un.S_addr = inet_addr(cstrIPtemp);//不需要在前面加htol!!!
							CString cstrSend = pServer->m_strRXData + '$' + sendCount + '#' + recvCount;
							sendto(m_MyUdpServerCom,cstrSend.GetBuffer(cstrSend.GetLength()),cstrSend.GetLength(),0,(sockaddr*)&addrTo,sizeof(sockaddr));
						}
						g_TCP.SockCliList.GetNext(pos);
					}
				}
			}
			Sleep(1);
		}
	}
	else if(g_protocol == UDP_MODE)
	{
		while(g_bflag)
		{//如果客户端列表不为空
			if (!pServer->m_strRXData.IsEmpty())
			{
				if(this->SockCliIPList.GetCount() > 0)
				{
					POSITION pos =this->SockCliIPList.GetHeadPosition();
					while( pos != NULL)
					{
						//获取该客户端IP地址
						CString cstrTempIP;
						cstrTempIP = this->SockCliIPList.GetAt(pos);
					
						CString sendCount,recvCount;
						sendCount.Format("%d",g_Send_Count);
						recvCount.Format("%d",g_Recv_Count);
						//除了服务端，就发送
						if(cstrTempIP != this->cstrServerIP)
						{
							//addrTo.sin_addr.S_un.S_addr=htonl(inet_addr(cstrIPtemp.GetBuffer(cstrIPtemp.GetLength())));
							addrTo.sin_addr.S_un.S_addr = inet_addr(cstrTempIP);//不需要在前面加htol!!!
							CString cstrSend = pServer->m_strRXData + '$' + sendCount + '#' + recvCount;
							sendto(m_MyUdpServerCom,cstrSend.GetBuffer(cstrSend.GetLength()),cstrSend.GetLength(),0,(sockaddr*)&addrTo,sizeof(sockaddr));
						}
						this->SockCliIPList.GetNext(pos);
					}
				}
			}
			Sleep(1);
		}
	}
	return 0;
}


//串口数据接受线程
int CMyUdp::StartUDPClientCom(int dwPort,HWND hWnd)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	this->m_MyUdpClientCom = socket(AF_INET,SOCK_DGRAM,0);

	if(INVALID_SOCKET == m_MyUdpClientCom)
	{
		closesocket(m_MyUdpClientCom);
		AfxMessageBox("串口线程创建套接字失败!");
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN addrSer;
	addrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons((u_short)dwPort);

	int iRet1;
	iRet1= bind(m_MyUdpClientCom,(SOCKADDR*)&addrSer,sizeof(SOCKADDR));
	if(iRet1 == SOCKET_ERROR)
	{
		closesocket(m_MyUdpClientCom);
		AfxMessageBox("串口线程绑定套接字失败!");
		WSACleanup();
		return -1;
	}

	struct sockaddr_in  sockaddr;
	int iAddrLen = sizeof(SOCKADDR);
	char buffer[1024] = {0};//接收缓冲区要大，否则接受不全，导致乱码错位！
	//循环接受
	g_bflagCli = TRUE;
	while(g_bflagCli)
	{
		 recvfrom(this->m_MyUdpClientCom,buffer,1024,0,(SOCKADDR*)&sockaddr,&iAddrLen);
		//如果接受缓冲区不为空，则显示在接受
		 CString cstrRecv = CString(buffer);
		 //待测试
		if(!cstrRecv.IsEmpty())
		{
			int iIndex1 = cstrRecv.Find("$");
			int iIndex2 = cstrRecv.Find("#");
			CString cstrTemp = cstrRecv.Left(iIndex1);
			CString cstrText;
			pClient->m_richedit_recv.GetWindowText(cstrText);
			if ((cstrTemp != cstrText) && (cstrTemp != pClient->m_richedit_recv_string) && (iIndex1 != -1) && iIndex2 != -1)
			{//判断条件2解决清空问题
				pClient->m_richedit_recv.SetWindowText(cstrTemp);
				pClient->m_richedit_recv_string = cstrTemp;//同时赋值关联变量解决清空问题
				pClient->m_edit_send.SetWindowText(cstrRecv.Mid(iIndex1+1,iIndex2-iIndex1-1));
				pClient->m_edit_recv.SetWindowText(cstrRecv.Mid(iIndex2+1));
			}
		}
		ZeroMemory(buffer,1024);//不用memset
	}

	return 0;
}

BOOL CMyUdp::ConnectSer()
{
	this->m_MyUdpClient.Create(UDP_CLIENT,SOCK_DGRAM,this->cstrClientIP);
	//this->m_MyUdpClient.Bind((UINT)UDP_CLIENT); //port不能也是8888,调试时同一主机会冲突
	CString cstrTemp("TS");//TEST-STRING
	CString cstrSend;
	cstrSend = AddDateHead(cstrTemp);
	m_MyUdpClient.SendTo(cstrSend.GetBuffer(cstrSend.GetLength()),cstrSend.GetLength(),dwServerPort,cstrServerIP);
	return TRUE;
}

BOOL CMyUdp::SendNetInfo(CString cstrSend)
{
	cstrSend = AddDateHead(cstrSend);
	int iRet = m_MyUdpClient.SendTo(cstrSend.GetBuffer(cstrSend.GetLength()),cstrSend.GetLength(),dwServerPort,cstrServerIP);
	if (iRet >= 0)
		return TRUE;
	else
	{
		AfxMessageBox("发送失败！");
		return FALSE;
	}
}

CString CMyUdp::AddDateHead(CString& cstrSend)
{
	CString cstrInfo;
	cstrInfo = this->cstrClientIP + '$' + cstrSend;
	return cstrInfo;
}

CString CMyUdp::ReadCli()
{
	struct sockaddr_in  sockaddr;
	int iAddrLen = sizeof(SOCKADDR);
	char buffer[1024] = {0};
	recvfrom(this->m_MyUdpServer,buffer,1024,0,(SOCKADDR*)&sockaddr,&iAddrLen);
	CString Recv = CString(buffer);
	//UDP也可以采取如下方式获取IP，本次采用客户端提供
	//CString cstrIP = inet_ntoa(sockaddr.sin_addr);
	return Recv;
}

//UDP服务器群发网络消息
void CMyUdp::GroupSend(CString cstrIP,CString cstrInfo)
{
	sockaddr_in	addrTo;
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons((u_short)UDP_CLIENT);

	//遍历链表，除了该套接字外，将接收到的信息发送出去
	POSITION pos = this->SockCliIPList.GetHeadPosition();
	while( pos != NULL)
	{
		CString cstrTemp = this->SockCliIPList.GetAt(pos);

		if(cstrTemp != cstrIP)
		{	
			addrTo.sin_addr.S_un.S_addr=inet_addr(cstrTemp.GetBuffer(cstrTemp.GetLength()));
			int iReT = sendto(m_MyUdpServer,cstrInfo.GetBuffer(cstrInfo.GetLength()),cstrInfo.GetLength(),0,(sockaddr*)&addrTo,sizeof(sockaddr));//不需要在前面加htol!!!
		}
		this->SockCliIPList.GetNext(pos);
	}
}

CString CMyUdp::SendComSel(CString cstrIPIn)
{
	sockaddr_in	addrTo;
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons((u_short)UDP_CLIENT);
	
	CString cstrIPClient;
	POSITION pos = this->SockCliIPList.GetHeadPosition();
	while( pos != NULL)
	{
		cstrIPClient = this->SockCliIPList.GetAt(pos);

		CString cstrCmd;
		cstrCmd.Format("%d",SELECT_COM);
		if(cstrIPClient == cstrIPIn)
		{
			addrTo.sin_addr.S_un.S_addr=inet_addr(cstrIPClient);
			int iReT = sendto(m_MyUdpServer,cstrCmd.GetBuffer(cstrCmd.GetLength()),cstrCmd.GetLength(),0,(sockaddr*)&addrTo,sizeof(sockaddr));//不需要在前面加htol!!!
		}
		this->SockCliIPList.GetNext(pos);
	}
	return cstrIPClient;
}