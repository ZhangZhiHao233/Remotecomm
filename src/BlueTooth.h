#ifndef BLUETOOTH
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#include "Winsock2.h"
#include "Ws2bth.h"
#include "BluetoothAPIs.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"irprops.lib")

#include <vector>
using namespace std;
typedef vector<CString> BTUserList;
#define SEND_TEXT 1
#define SEND_FILE 2
#endif


BOOL InitBT();

void CleanBT();

int SearchLocalBT(BTUserList& list);

int SearchRemoteBT(BTUserList& list);

int StartUpLocalBT(SOCKET& sockServer);

SOCKET ServerListenBT(SOCKET& sockServer);

int ConnectRemoteBT(char* chBTname,SOCKET& sockClient);

int SendMessageBT(SOCKET& destSocket,CString cstrSendMessage);

int RecvMsgAndFile(SOCKET& destsock);

