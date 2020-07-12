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
		AfxMessageBox("�����׽���ʧ��!");
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
		AfxMessageBox("���׽���ʧ��!");
		WSACleanup();
		return -1;
	}
	
	if(SOCKET_ERROR == WSAAsyncSelect(m_MyUdpServer,hWnd,WM_SOCKET_UDP,FD_ACCEPT | FD_READ |FD_CLOSE))
	{
		closesocket(m_MyUdpServer);
		AfxMessageBox("ע�������¼��׽���ʧ��!");
		WSACleanup();
		return -1;
	}
	return 0;
}

//�������ݷ����߳�
int CMyUdp::StartUDPServerCom(int dwPort,HWND hWnd)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	this->m_MyUdpServerCom = socket(AF_INET,SOCK_DGRAM,0);

	if(INVALID_SOCKET == m_MyUdpServerCom)
	{
		closesocket(m_MyUdpServerCom);
		AfxMessageBox("�����׽���ʧ��!");
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
		AfxMessageBox("���׽���ʧ��!");
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
		{//����ͻ����б�Ϊ��
			if (!pServer->m_strRXData.IsEmpty())
			{
				if(g_TCP.SockCliList.GetCount() > 0)
				{
					POSITION pos = g_TCP.SockCliList.GetHeadPosition();
					while( pos != NULL)
					{
						//��ȡ�ÿͻ���IP��ַ
						SOCKET socketTemp;
						socketTemp =  g_TCP.SockCliList.GetAt(pos);
						CString cstrIPtemp = g_TCP.GetPeerName(socketTemp);
						CString sendCount,recvCount;
						sendCount.Format("%d",g_Send_Count);
						recvCount.Format("%d",g_Recv_Count);
						//���˷���ˣ��ͷ���
						if(cstrIPtemp != g_TCP.cstrServerIP)
						{
							//addrTo.sin_addr.S_un.S_addr=htonl(inet_addr(cstrIPtemp.GetBuffer(cstrIPtemp.GetLength())));
							addrTo.sin_addr.S_un.S_addr = inet_addr(cstrIPtemp);//����Ҫ��ǰ���htol!!!
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
		{//����ͻ����б�Ϊ��
			if (!pServer->m_strRXData.IsEmpty())
			{
				if(this->SockCliIPList.GetCount() > 0)
				{
					POSITION pos =this->SockCliIPList.GetHeadPosition();
					while( pos != NULL)
					{
						//��ȡ�ÿͻ���IP��ַ
						CString cstrTempIP;
						cstrTempIP = this->SockCliIPList.GetAt(pos);
					
						CString sendCount,recvCount;
						sendCount.Format("%d",g_Send_Count);
						recvCount.Format("%d",g_Recv_Count);
						//���˷���ˣ��ͷ���
						if(cstrTempIP != this->cstrServerIP)
						{
							//addrTo.sin_addr.S_un.S_addr=htonl(inet_addr(cstrIPtemp.GetBuffer(cstrIPtemp.GetLength())));
							addrTo.sin_addr.S_un.S_addr = inet_addr(cstrTempIP);//����Ҫ��ǰ���htol!!!
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


//�������ݽ����߳�
int CMyUdp::StartUDPClientCom(int dwPort,HWND hWnd)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	this->m_MyUdpClientCom = socket(AF_INET,SOCK_DGRAM,0);

	if(INVALID_SOCKET == m_MyUdpClientCom)
	{
		closesocket(m_MyUdpClientCom);
		AfxMessageBox("�����̴߳����׽���ʧ��!");
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
		AfxMessageBox("�����̰߳��׽���ʧ��!");
		WSACleanup();
		return -1;
	}

	struct sockaddr_in  sockaddr;
	int iAddrLen = sizeof(SOCKADDR);
	char buffer[1024] = {0};//���ջ�����Ҫ�󣬷�����ܲ�ȫ�����������λ��
	//ѭ������
	g_bflagCli = TRUE;
	while(g_bflagCli)
	{
		 recvfrom(this->m_MyUdpClientCom,buffer,1024,0,(SOCKADDR*)&sockaddr,&iAddrLen);
		//������ܻ�������Ϊ�գ�����ʾ�ڽ���
		 CString cstrRecv = CString(buffer);
		 //������
		if(!cstrRecv.IsEmpty())
		{
			int iIndex1 = cstrRecv.Find("$");
			int iIndex2 = cstrRecv.Find("#");
			CString cstrTemp = cstrRecv.Left(iIndex1);
			CString cstrText;
			pClient->m_richedit_recv.GetWindowText(cstrText);
			if ((cstrTemp != cstrText) && (cstrTemp != pClient->m_richedit_recv_string) && (iIndex1 != -1) && iIndex2 != -1)
			{//�ж�����2����������
				pClient->m_richedit_recv.SetWindowText(cstrTemp);
				pClient->m_richedit_recv_string = cstrTemp;//ͬʱ��ֵ������������������
				pClient->m_edit_send.SetWindowText(cstrRecv.Mid(iIndex1+1,iIndex2-iIndex1-1));
				pClient->m_edit_recv.SetWindowText(cstrRecv.Mid(iIndex2+1));
			}
		}
		ZeroMemory(buffer,1024);//����memset
	}

	return 0;
}

BOOL CMyUdp::ConnectSer()
{
	this->m_MyUdpClient.Create(UDP_CLIENT,SOCK_DGRAM,this->cstrClientIP);
	//this->m_MyUdpClient.Bind((UINT)UDP_CLIENT); //port����Ҳ��8888,����ʱͬһ�������ͻ
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
		AfxMessageBox("����ʧ�ܣ�");
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
	//UDPҲ���Բ�ȡ���·�ʽ��ȡIP�����β��ÿͻ����ṩ
	//CString cstrIP = inet_ntoa(sockaddr.sin_addr);
	return Recv;
}

//UDP������Ⱥ��������Ϣ
void CMyUdp::GroupSend(CString cstrIP,CString cstrInfo)
{
	sockaddr_in	addrTo;
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons((u_short)UDP_CLIENT);

	//�����������˸��׽����⣬�����յ�����Ϣ���ͳ�ȥ
	POSITION pos = this->SockCliIPList.GetHeadPosition();
	while( pos != NULL)
	{
		CString cstrTemp = this->SockCliIPList.GetAt(pos);

		if(cstrTemp != cstrIP)
		{	
			addrTo.sin_addr.S_un.S_addr=inet_addr(cstrTemp.GetBuffer(cstrTemp.GetLength()));
			int iReT = sendto(m_MyUdpServer,cstrInfo.GetBuffer(cstrInfo.GetLength()),cstrInfo.GetLength(),0,(sockaddr*)&addrTo,sizeof(sockaddr));//����Ҫ��ǰ���htol!!!
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
			int iReT = sendto(m_MyUdpServer,cstrCmd.GetBuffer(cstrCmd.GetLength()),cstrCmd.GetLength(),0,(sockaddr*)&addrTo,sizeof(sockaddr));//����Ҫ��ǰ���htol!!!
		}
		this->SockCliIPList.GetNext(pos);
	}
	return cstrIPClient;
}