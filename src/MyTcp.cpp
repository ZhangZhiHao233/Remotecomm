#include "StdAfx.h"
#include "MyTcp.h"
#include "MyUdp.h"
#include "MyBlueTooth.h"
extern CMyUdp g_UDP;
extern CMyBlueTooth  g_BTH;
extern BOOL bThread0;
extern BOOL bThread1;

#pragma comment(lib,"ws2_32.lib")

extern bool g_bflag;
extern int g_protocol;
CMyTcp::CMyTcp(void)
	:m_MyTcpServer(INVALID_SOCKET)
{
}

CMyTcp::~CMyTcp(void)
{
}

//创建TCP套接字并监听
int CMyTcp::StartTCPServer(DWORD &dwPort,HWND hWnd)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	m_MyTcpServer = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,0);

	if(INVALID_SOCKET == m_MyTcpServer)
	{
		closesocket(m_MyTcpServer);
		AfxMessageBox("创建套接字失败!");
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN addrSer;
	addrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons((u_short)dwPort);

	int iRet1;
	iRet1= bind(m_MyTcpServer,(SOCKADDR*)&addrSer,sizeof(SOCKADDR));

	if(iRet1 == SOCKET_ERROR)
	{
		closesocket(m_MyTcpServer);
		AfxMessageBox("绑定套接字失败!");
		WSACleanup();
		return -1;
	}

	int iRet2;
	iRet2 = listen(m_MyTcpServer,5);

	if(iRet2 == SOCKET_ERROR)
	{
		closesocket(m_MyTcpServer);
		AfxMessageBox("监听套接字失败!");
		WSACleanup();
		return -1;
	}

	if(SOCKET_ERROR == WSAAsyncSelect(m_MyTcpServer,hWnd,WM_SOCKET_TCP,FD_ACCEPT | FD_READ |FD_CLOSE))
	{
		closesocket(m_MyTcpServer);
		AfxMessageBox("注册网络事件套接字失败!");
		WSACleanup();
		return -1;
	}
	return 0;
}



//由套接字获取IP信息
CString CMyTcp::GetPeerName(SOCKET &socket)
{
	struct sockaddr_in  sockaddr;
	int iAddrLen = sizeof(SOCKADDR);
	int iErr = getpeername(socket,(struct sockaddr*)&sockaddr,&iAddrLen);
	if (iErr != 0)
	{
		AfxMessageBox("获取IP地址失败！");
		return NULL;
	}
	CString cstrIP = inet_ntoa(sockaddr.sin_addr);
	return cstrIP;
}

//接受客户端连接，并放入套接字列表
CString  CMyTcp::AcceptCli()
{
	SOCKADDR addr;
	int iLen;
	iLen = sizeof(SOCKADDR);
	struct sockaddr_in  sockaddr;

	SOCKET *m_psocket = new SOCKET();
	(*m_psocket)	= accept(this->m_MyTcpServer,(SOCKADDR*)&addr,&iLen);
	int iErr = getpeername(*m_psocket,(struct sockaddr*)&sockaddr,&iLen);
	CString cstrIP = inet_ntoa(sockaddr.sin_addr);
	
	this->SockCliList.AddTail(*m_psocket);
	return cstrIP;
}

//接受客户端数据
CString  CMyTcp::ReadCli(SOCKET& socket)
{
	POSITION pos = this->SockCliList.GetHeadPosition();
	while( pos != NULL)
	{
		if(socket == this->SockCliList.GetAt(pos))
			break;
		else
			this->SockCliList.GetNext(pos);
	}

	char buf[100] = "";
	SOCKET m_temp = this->SockCliList.GetAt(pos);

	CString cstrIP = GetPeerName(m_temp);
	recv(m_temp,buf,100,0);

	CString cstrRet = cstrIP + '$' + CString(buf);
	return cstrRet;
}

//客户端断开连接
CString  CMyTcp::CloseCli(SOCKET& socket)
{
	CString cstrIP;
	POSITION pos = this->SockCliList.GetHeadPosition();
	while( pos != NULL)
	{
		if(socket == this->SockCliList.GetAt(pos))
		{
			SOCKET m_temp = this->SockCliList.GetAt(pos);
			cstrIP = GetPeerName(m_temp);

			m_temp = INVALID_SOCKET;
			this->SockCliList.RemoveAt(pos);
			break;
		}
		else
			this->SockCliList.GetNext(pos);
	}

	return cstrIP;
}

//TCP服务器群发网络消息
void CMyTcp::GroupSend(SOCKET m_sockcli,CString cstrIP,CString cstrInfo)
{
	//遍历链表，除了该套接字外，将接收到的信息发送出去
	POSITION pos = this->SockCliList.GetHeadPosition();
	while( pos != NULL)
	{
		SOCKET socketTemp;
		socketTemp = this->SockCliList.GetAt(pos);
		CString cstrIPtemp = GetPeerName(socketTemp);
		if(cstrIPtemp != cstrIP)
		{
			send(socketTemp,cstrInfo.GetBuffer(cstrInfo.GetLength()),cstrInfo.GetLength(),0);
		}
		this->SockCliList.GetNext(pos);
	}
}


//////////////////////////////////////////////////////////////////////////
//客户端实现代码
//////////////////////////////////////////////////////////////////////////
BOOL CMyTcp::ConnectSer()
{
	this->m_MyTcpClient.Create();
	int iErr = this->m_MyTcpClient.Connect(this->cstrServerIP,this->dwServerPort);
	if( 1 ==  iErr)
	{
		return TRUE;
	}
	else
	{
		this->m_MyTcpClient.Close();
		AfxMessageBox("网络线程连接失败！");
		return FALSE;
	}
}

BOOL CMyTcp::SendNetInfo(CString cstrSend)
{
	int iRet = this->m_MyTcpClient.Send(cstrSend.GetBuffer(cstrSend.GetLength()),cstrSend.GetLength(),0);
	if (iRet >= 0)
		return TRUE;
	else
	{
		AfxMessageBox("发送失败！");
		return FALSE;
	}
}

SOCKET CMyTcp::SendComSel(CString cstrIPIn)
{
	POSITION pos = this->SockCliList.GetHeadPosition();
	while( pos != NULL)
	{
		SOCKET socketTemp;
		socketTemp = this->SockCliList.GetAt(pos);

		CString cstrIP = this->GetPeerName(socketTemp);

		CString cstrCmd;
		cstrCmd.Format("%d",SELECT_COM);
		if(cstrIP == cstrIPIn)
		{
			send(socketTemp,cstrCmd.GetBuffer(cstrCmd.GetLength()),cstrCmd.GetLength(),0);
			return socketTemp;
		}
		this->SockCliList.GetNext(pos);
	}
	return NULL;
}

void CMyTcp::CloseSer()
{
	if (g_protocol == TCP_MODE)
	{
		//shutdown(m_MyTcpServer);
		//关闭TCP服务器网络套接字、各客户端套接字
		POSITION pos = this->SockCliList.GetHeadPosition();
		while( pos != NULL)
		{
			SOCKET socketTemp;
			socketTemp = this->SockCliList.GetAt(pos);
			if (socketTemp != INVALID_SOCKET)
			{
				closesocket(socketTemp);
			}
			this->SockCliList.GetNext(pos);
		}

		closesocket(m_MyTcpServer);
		g_bflag = FALSE;
		//shutdown(m_MyTcpServerCom);
		//关闭UDP串口数据套接字
		closesocket(g_UDP.m_MyUdpServerCom);
		WSACleanup();
	}
	else if (g_protocol == UDP_MODE)
	{
		closesocket(g_UDP.m_MyUdpServer);
		g_bflag = FALSE;
		closesocket(g_UDP.m_MyUdpServerCom);
		WSACleanup();
	}
	else if(g_protocol == BLUETOOTH_MODE)
	{
	    bThread0 = FALSE;
	    bThread1 = FALSE;
		closesocket(g_BTH.MyBthSocketSer);
		closesocket(g_BTH.MyBthSocketSerCli);
		WSACleanup();
	}
}