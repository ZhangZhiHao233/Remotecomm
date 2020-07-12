#pragma once
#ifndef MYSOCKET
#include "MySocketCln.h"
#endif

class CMyUdp
{
public:
	CMyUdp(void);
	~CMyUdp(void);
public:
	SOCKET m_MyUdpServer;
	SOCKET m_MyUdpServerCom;
	SOCKET m_MyUdpClientCom;
	CMySocketCln m_MyUdpClient;
	CString cstrServerIP;
	CString cstrClientIP;
	DWORD dwServerPort;
	CList<CString,CString&> SockCliIPList;
public:
	int StartUDPServer(DWORD &dwPort,HWND hWnd);
	int StartUDPServerCom(int dwPort,HWND hWnd);
	int StartUDPClientCom(int dwPort,HWND hwnd);
	CString SendComSel(CString cstrIPIn);
	void GroupSend(CString cstrIP,CString cstrInfo);
	CString ReadCli();
	CString AddDateHead(CString& cstrSend);
	BOOL SendNetInfo(CString cstrSend);
	BOOL ConnectSer();
};
