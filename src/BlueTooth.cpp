#include "BlueTooth.h"

//初始化蓝牙套接字
BOOL InitBT()
{
	WSADATA WSAData = {0};
	if(0 != WSAStartup(MAKEWORD(2,2),&WSAData))
	{
		AfxMessageBox("初始化winsocket失败！");
		return -1;
	}
	return 0;
}

//清理套接字
void CleanBT()
{
	WSACleanup();
}

//搜索本地蓝牙
int SearchLocalBT(BTUserList& list)
{
	char ch[500];
	char ch1[500];
	DWORD dwLen = 500;

	BLUETOOTH_FIND_RADIO_PARAMS  btpara;
	HANDLE hRadio = NULL;
	HBLUETOOTH_RADIO_FIND  hFind = NULL;
	memset(&btpara,0,sizeof(BLUETOOTH_FIND_RADIO_PARAMS));
	btpara.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);
	hFind = BluetoothFindFirstRadio(&btpara,&hRadio);
	if (NULL != hFind)
	{
		do 
		{
			BLUETOOTH_RADIO_INFO  RadioInfo;
			SOCKADDR_BTH sa;
			if (hRadio)
			{
				memset(&RadioInfo,0,sizeof(BLUETOOTH_RADIO_INFO));
				RadioInfo.dwSize = sizeof(BLUETOOTH_RADIO_INFO);

				if (ERROR_SUCCESS == BluetoothGetRadioInfo(hRadio,&RadioInfo))
				{
					WideCharToMultiByte(CP_OEMCP,NULL,RadioInfo.szName,-1,ch,500,NULL,FALSE);
					memset(&sa,0,sizeof(SOCKADDR_BTH));
					sa.addressFamily = AF_BTH;
					sa.btAddr = RadioInfo.address.ullLong;
					WSAAddressToString((LPSOCKADDR)&sa,sizeof(SOCKADDR_BTH),NULL,ch1,&dwLen);
					strcat(ch,ch1);
					list.push_back(CString(ch));
					
					BluetoothEnableIncomingConnections(hRadio,TRUE);
					BluetoothEnableDiscovery(hRadio,TRUE);
				}
			}
		} while (BluetoothFindNextRadio(hFind,&hRadio));
		BluetoothFindRadioClose(hFind);
		return 0;
	}
	else
	{
		return -1;
	}
}

//搜索远程蓝牙
int SearchRemoteBT(BTUserList& list)
{
	char ch[500];
	char ch1[500];
	DWORD dwLen = 500;

	BLUETOOTH_DEVICE_SEARCH_PARAMS btpara;
	BLUETOOTH_DEVICE_INFO btInfo;
	HBLUETOOTH_DEVICE_FIND  hFind = NULL;
	memset(&btpara,0,sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS));
	btpara.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
	btpara.hRadio = NULL;
	btpara.fReturnAuthenticated = TRUE;
	btpara.fReturnRemembered = TRUE;
	btpara.fReturnUnknown = TRUE;
	btpara.fReturnConnected = TRUE;
	btpara.fIssueInquiry = TRUE;
	btpara.cTimeoutMultiplier = 30;
	memset(&btInfo,0,sizeof(BLUETOOTH_DEVICE_INFO));
	btInfo.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
	hFind = BluetoothFindFirstDevice(&btpara,&btInfo);
	if (hFind != NULL)
	{
		SOCKADDR_BTH sa;
		do 
		{
			WideCharToMultiByte(CP_OEMCP,NULL,btInfo.szName,-1,ch,500,NULL,FALSE);
			memset(&sa,0,sizeof(SOCKADDR_BTH));
			sa.addressFamily = AF_BTH;
			sa.btAddr = btInfo.Address.ullLong;
			WSAAddressToString((LPSOCKADDR)&sa,sizeof(SOCKADDR_BTH),NULL,ch1,&dwLen);
			strcat( ch,ch1);
			list.push_back(CString(ch));
		} while (BluetoothFindNextDevice(hFind,&btInfo));
		BluetoothFindRadioClose(hFind);
		return 0;
	}
	else
	{
		return -1;
	}
}

//开启本地蓝牙作为服务器监听
int StartUpLocalBT(SOCKET& sockServer)
{
	SOCKADDR_BTH	sa;
	sockServer = socket(AF_BTH,SOCK_STREAM,BTHPROTO_RFCOMM);
	if(SOCKET_ERROR == sockServer)
	{
		AfxMessageBox("蓝牙监听套接字创建失败！");
		sockServer = NULL;
		return -1;
	}

	memset(&sa,0,sizeof(SOCKADDR_BTH));
	sa.addressFamily = AF_BTH;
	sa.btAddr = 0;
	sa.port	= 20;

	if (SOCKET_ERROR == bind(sockServer,(const sockaddr*)&sa,sizeof(SOCKADDR_BTH)))
	{
		AfxMessageBox("蓝牙监听套接字绑定失败!");
		closesocket(sockServer);
		sockServer = NULL;
		return -2; 
	}

	listen(sockServer,1);
	return 0;
}

//本地蓝牙接收远程蓝牙连接
SOCKET ServerListenBT(SOCKET& sockServer)
{	
	SOCKADDR_BTH sa;
	int sa_len = sizeof(sa);
	SOCKET sock;

	memset(&sa,0,sa_len);
	sock = accept(sockServer,(LPSOCKADDR)&sa,&sa_len);

	if (SOCKET_ERROR == sock)
	{
		return NULL;
	}
	else
	{
		return sock;
	}
}

//客户端连接远程蓝牙设备
int ConnectRemoteBT(char* chBTname,SOCKET& sockClient)
{
	char *ch1;
	SOCKADDR_BTH sa;
	int sa_len = sizeof(SOCKADDR_BTH);

	ch1 = chBTname + strlen(chBTname) - 18;
	ch1[17] = 0;
	memset(&sa,0,sa_len);

	if (SOCKET_ERROR == WSAStringToAddress(ch1,AF_BTH,NULL,(LPSOCKADDR)&sa,&sa_len))
	{
		AfxMessageBox("远程设备地址转换失败！");
		return -1;
	}
	sa.addressFamily = AF_BTH;
	sa.port = 20;
	sockClient = socket(AF_BTH,SOCK_STREAM,BTHPROTO_RFCOMM);

	if(SOCKET_ERROR == connect(sockClient,(LPSOCKADDR)&sa,sa_len))
	{
		AfxMessageBox("连接失败！");
		closesocket(sockClient);
		sockClient = NULL;
		return -2;
	}
	
	return 0;
}

//发送信息到对方蓝牙设备
int SendMessageBT(SOCKET& destSocket,CString cstrSendMessage)
{
	char ch[20];
	int len;
	len = cstrSendMessage.GetLength();
	ch[0] = SEND_TEXT;

	sprintf(ch+1,"%04d",len);
	send(destSocket,ch,5,0);
	send(destSocket,cstrSendMessage.GetBuffer(),len,0);
	return 0;
}

//发送文件至对方蓝牙设备
int SendFileBT(SOCKET& destsocket)
{
	CFileDialog fdDlg(TRUE);
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

		//获取文件长度
		{
			char ch[20];
			char data[1024];
			CString str;
			int len;
			int i;
			len = strlen(fn);
			ch[0] = SEND_FILE;
			sprintf(ch+1,"%04d",len);
			send(destsocket,ch,5,0);
			send(destsocket,fn,len,0);
			send(destsocket,(char*)&Filelength,sizeof(long),0);

			len = Filelength;
			while(1)
			{
				i = file.Read(data,1024);
				Sleep(50);
				if (i > 0)
				{
					send(destsocket,data,i,0);
					len = len - i;
					sprintf(fn,"%010d",len);
				}	
				if (len <= 0)
				{
					break;
				}
			}	
		}
		file.Close();
	}
	return 0;
}

//接收对方蓝牙设备信息与文件
int RecvMsgAndFile(SOCKET& destsock)
{
	char buf[1024];
	int received;
	char cmd;
	int len;

	received = 0;
	memset(buf,0,1024);
	received = recv(destsock,buf,5,0);
	if (received  > 0)
	{
		cmd = buf[0];
		len = atoi(buf+1);
		switch(cmd)
		{
		case  SEND_TEXT:
			{
				received = 0;
				memset(buf,0,1024);
				received = recv(destsock,buf,len,0);
				if (received > 0)
				{
					strcat(buf,"\r\n");
				}
				break;
			}
		case SEND_FILE:
			{
				char fn[300];
				long filelength;
				CFile file;
				int i;
				char data[1024];
				received = 0;
				memset(buf,0,1024);
				received = recv(destsock,buf,len,0);
				strcpy(fn,buf);
				received = 0;
				memset(buf,0,1024);
				received = recv(destsock,buf,sizeof(long),0);

				filelength = *((long*)(buf));
				if (file.Open(fn,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
				{
					while (1)
					{
						i = recv(destsock,data,1024,0);
						if (i > 0)
						{
							file.Write(data,i);
							filelength = filelength - i;
							sprintf(buf,"%010d",filelength);
						}
						if (filelength <= 0)
						{
							break;
						}
					}
					file.Close();
				}
				break;
			}
		}
	}
	return 0;
}