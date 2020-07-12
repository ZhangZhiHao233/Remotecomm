#pragma once

#ifndef HEAD
#include <vector>
using namespace std;
typedef vector<CString> BTUserList;
#define SEND_TEXT 1
#define SEND_FILE 2
#define  CNN_COM 3
#define DIS_COM 4
#define SEND_COM 5
#define SER_SEND 6
#define SER_SEND_AUTO 7
#define STOP_SER_SEND_AUTO 8
#define COM_CNT 9
#endif

class CMyBlueTooth
{
public:
	CMyBlueTooth(void);
	~CMyBlueTooth(void);
public:
	SOCKET MyBthSocketSer;
	SOCKET MyBthSocketSerCli;//服务端Accept到的客户端SOCKET
	SOCKET MyBthSocketCli;
	CString cstrSelectBTHAddr;
	CString cstrSelectBTHName;
	CString ConnectName;
public:
	BOOL InitBT();
	void CleanBT();
	int SearchLocalBT(BTUserList& list);
	int SearchRemoteBT(BTUserList& list);
	int SearchRemoteBTNew(BTUserList& list);
	int StartUpLocalBT(SOCKET& sockServer);
	int ConnectRemoteBT(CString& chBTHAddr,SOCKET& sockClient);
	void SendMessageBT(CString& cstrMessage,int iMode = 0);
	void SendMessageBTSer(CString& cstrMessage,int iMode = 0);
	int SendFileBT(char* fn,CFile& file,long& Filelength);
	void ListenBth();
	void RecvBth();
	void RecvBthCli();
};

