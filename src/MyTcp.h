#pragma once
#ifndef MYSOCKET
#include "MySocketCln.h"
#endif

class CMyTcp
{
public:
	CMyTcp(void);
	~CMyTcp(void);
public:
	int StartTCPServer(DWORD &dwPort,HWND hWnd);
	CString ReadCli(SOCKET& socket);
	CString CloseCli(SOCKET& socket);
	CString AcceptCli();
	void GroupSend(SOCKET m_sockcli,CString cstrIP,CString cstrInfo);
	CString GetPeerName(SOCKET &socket);
	CList<SOCKET,SOCKET&> SockCliList;
public:
	BOOL ConnectSer();
	BOOL SendNetInfo(CString cstrSend);
	SOCKET SendComSel(CString cstrIPIn);
	void CloseSer();
public:
	SOCKET m_MyTcpServer;
	SOCKET m_MyTcpServerCom;
	SOCKET m_MyTcpClientCom;
	CMySocketCln m_MyTcpClient;
public:
	CString cstrServerIP;
	DWORD dwServerPort;
};
