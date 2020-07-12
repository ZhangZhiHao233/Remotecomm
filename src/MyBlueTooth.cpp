#include "StdAfx.h"
#include "MyBlueTooth.h"
#include "ServerDlg.h"
#include "ClientDlg.h"
#include "BthListDlg.h"

#include "Ws2bth.h"
#include "BluetoothAPIs.h"
#pragma comment(lib,"irprops.lib")
CString cstrBTHName;
extern BOOL bThread0;
extern BOOL bThread1;
extern BOOL bThread2;
extern CServerDlg* pServer;
extern CClientDlg* pClient;
extern CBthListDlg *pBthListDlg;
extern CMyBlueTooth g_BTH_list;
extern int iSendCount;
extern int iRecvCount;
CMyBlueTooth::CMyBlueTooth(void)
	:MyBthSocketSer(INVALID_SOCKET)
	,MyBthSocketSerCli(INVALID_SOCKET)
{
}

CMyBlueTooth::~CMyBlueTooth(void)
{
}

//蓝牙地址转CString
CString AddressToCString(BTH_ADDR bth)
{
	BYTE *pbAddr = (BYTE*)&bth;  
	char pAddress[20] = {0};
	_stprintf(  
		pAddress, _T("%02X:%02X:%02X:%02X:%02X:%02X"),  
		pbAddr[5], pbAddr[4], pbAddr[3],  
		pbAddr[2], pbAddr[1], pbAddr[0]  
	);  
	return CString(pAddress);
}

BOOL CMyBlueTooth::InitBT()
{
	WSADATA WSAData = {0};
	if(0 != WSAStartup(MAKEWORD(2,2),&WSAData))
	{
		AfxMessageBox("初始化winsocket失败！");
		return FALSE;
	}
	return TRUE;
}

void CMyBlueTooth::CleanBT()
{
	WSACleanup();
}

int CMyBlueTooth::SearchLocalBT(BTUserList& list)
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

int CMyBlueTooth::SearchRemoteBT(BTUserList& list)
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

int CMyBlueTooth::SearchRemoteBTNew(BTUserList& list)
{
	HBLUETOOTH_RADIO_FIND hbf = NULL;  
	HANDLE hbr = NULL;  
	HBLUETOOTH_DEVICE_FIND hbdf = NULL;  
	BLUETOOTH_FIND_RADIO_PARAMS btfrp = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };  
	BLUETOOTH_RADIO_INFO bri = { sizeof(BLUETOOTH_RADIO_INFO)};  
	BLUETOOTH_DEVICE_SEARCH_PARAMS btsp = { sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS) };  
	BLUETOOTH_DEVICE_INFO btdi = { sizeof(BLUETOOTH_DEVICE_INFO) };  
	hbf=BluetoothFindFirstRadio(&btfrp, &hbr);  
	bool brfind = hbf != NULL;  

	while (brfind)  
	{  
		if (BluetoothGetRadioInfo(hbr, &bri) == ERROR_SUCCESS)  
		{  
			btsp.hRadio = hbr;  
			btsp.fReturnAuthenticated = TRUE;  
			btsp.fReturnConnected = FALSE;  
			btsp.fReturnRemembered = TRUE;  
			btsp.fReturnUnknown = TRUE;  
			btsp.cTimeoutMultiplier = 30;  
			hbdf = BluetoothFindFirstDevice(&btsp, &btdi);  
			bool bfind = hbdf != NULL;  
			while (bfind)  
			{  
				char ch[50];
				WideCharToMultiByte(CP_OEMCP,NULL,btdi.szName,-1,ch,500,NULL,FALSE);
				CString cstrAddress;
				cstrAddress = AddressToCString(btdi.Address.ullLong);
				CString cstrShowName;
				cstrShowName.Format("(%s)",cstrAddress);
				strcat( ch,cstrShowName.GetBuffer(cstrShowName.GetLength()));
				list.push_back(CString(ch));
				bfind=BluetoothFindNextDevice(hbdf, &btdi);  
			}  
			BluetoothFindDeviceClose(hbdf);  
		}  
		CloseHandle(hbr);  
		brfind=BluetoothFindNextRadio(hbf, &hbr);  
	}  
	BluetoothFindRadioClose(hbf);  
	return 0;
}

int CMyBlueTooth::StartUpLocalBT(SOCKET& sockServer)
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

//win10设置->设备设置->更多蓝牙设置->选中“新蓝牙要连接的时候要通知我”
//win10/win7通过蓝牙代码配对连接要删除已配对/已连接蓝牙
//若第一次连接，WIN10系统
int CMyBlueTooth::ConnectRemoteBT(CString& chBTHAddr,SOCKET& sockClient)
{
	SOCKADDR_BTH sa;
	int sa_len = sizeof(SOCKADDR_BTH);

	if (SOCKET_ERROR == WSAStringToAddress(chBTHAddr.GetBuffer(chBTHAddr.GetLength()),AF_BTH,NULL,(LPSOCKADDR)&sa,&sa_len))
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
	
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>连接服务器成功！\r\n");
	

	return 0;
}

//先发长度，后发文字
void CMyBlueTooth::SendMessageBT(CString& cstrMessage,int iMode)
{
	char ch[20];
	int len;
	if (iMode == 0)
	{
		len = cstrMessage.GetLength();
		ch[0] = SEND_TEXT;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketCli,ch,5,0);
		send(this->MyBthSocketCli,cstrMessage.GetBuffer(cstrMessage.GetLength()),len,0);
	}
	else if (iMode == 1)
	{
		len = 1;
		ch[0] = DIS_COM;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketCli,ch,5,0);
	}
	else if (iMode == 2)
	{
		len = cstrMessage.GetLength();
		ch[0] = SEND_COM;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketCli,ch,5,0);
		send(this->MyBthSocketCli,cstrMessage.GetBuffer(cstrMessage.GetLength()),len,0);
	}
	else if(iMode == 3)
	{
		len = cstrMessage.GetLength();
		ch[0] = SER_SEND_AUTO;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketCli,ch,5,0);
		send(this->MyBthSocketCli,cstrMessage.GetBuffer(cstrMessage.GetLength()),len,0);
	}
	else if (iMode == 4)
	{
		len = 1;
		ch[0] = STOP_SER_SEND_AUTO;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketCli,ch,5,0);
	}
}

void CMyBlueTooth::SendMessageBTSer(CString& cstrMessage,int iMode)
{
	char ch[20];
	int len;
	if (iMode == 0)
	{
		len = cstrMessage.GetLength();
		ch[0] = SEND_TEXT;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketSerCli,ch,5,0);
		send(this->MyBthSocketSerCli,cstrMessage.GetBuffer(cstrMessage.GetLength()),len,0);
	}
	else if (iMode == 1)
	{
		ch[0] = CNN_COM;
		len = 0;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketSerCli,ch,5,0);
	}
	else if (iMode == 2)
	{
		len = cstrMessage.GetLength();
		ch[0] = SER_SEND;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketSerCli,ch,5,0);
		send(this->MyBthSocketSerCli,cstrMessage.GetBuffer(cstrMessage.GetLength()),len,0);
	}
	else if (iMode == 3)
	{
		len = cstrMessage.GetLength();
		ch[0] = COM_CNT;
		sprintf(ch+1,"%04d",len);
		send(this->MyBthSocketSerCli,ch,5,0);
	    send(this->MyBthSocketSerCli,cstrMessage.GetBuffer(cstrMessage.GetLength()),len,0);
	}
}

//发文件
int CMyBlueTooth::SendFileBT(char* fn,CFile& file,long& Filelength)
{
	char ch[20];
	char data[1024];
	CString str;
	int len;
	int i;
	len = strlen(fn);
	ch[0] = SEND_FILE;
	sprintf(ch+1,"%04d",len);
	send(g_BTH_list.MyBthSocketCli,ch,5,0);

	Sleep(50);
	send(g_BTH_list.MyBthSocketCli,fn,len,0);
	Sleep(50);
	send(g_BTH_list.MyBthSocketCli,(char*)&Filelength,sizeof(long),0);

	len = Filelength;
	while(1)
	{
		i = file.Read(data,1024);
		Sleep(50);
		if (i > 0)
		{
			send(g_BTH_list.MyBthSocketCli,data,i,0);
			len = len - i;
			sprintf(fn,"%010d",len);
		}	
		if (len <= 0)
		{
			break;
		}
	}	
	file.Close();
	pClient->m_edit_tip.SetSel(pClient->m_edit_tip.GetWindowTextLength(),pClient->m_edit_tip.GetWindowTextLength());
	pClient->m_edit_tip.ReplaceSel("I>文件发送成功！\r\n");
	return 0;
}



//服务器监听
void CMyBlueTooth::ListenBth()
{
	SOCKADDR_BTH sa;
	int sa_len = sizeof(sa);
	SOCKET sock;
	while(bThread0)
	{
		memset(&sa,0,sa_len);
		sock = accept(this->MyBthSocketSer,(LPSOCKADDR)&sa,&sa_len);
		if (SOCKET_ERROR == sock)
		{

		}
		else
		{
			this->MyBthSocketSerCli = sock;
			this->ConnectName = AddressToCString(sa.btAddr);
			CString cstrTemp;
			cstrBTHName = pServer->GetBthName(ConnectName);

			int iCount = pServer->m_comb_channel.GetCount();
			pServer->m_comb_channel.InsertString(iCount,cstrBTHName);

			cstrTemp.Format("I>%s 连接成功\r\n",cstrBTHName);
			pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
			pServer->m_edit_tip.ReplaceSel(cstrTemp);
		}
	}

	pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
	pServer->m_edit_tip.ReplaceSel("I>退出服务器监听线程\r\n");
}

//服务器接受信息和文件
void CMyBlueTooth::RecvBth()
{
	char buf[1024];
	int received;
	char cmd;
	int len;
	while(bThread1)
	{
		received = 0;
		memset(buf,0,1024);
		received = recv(this->MyBthSocketSerCli,buf,5,0);
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
					received = recv(this->MyBthSocketSerCli,buf,len,0);
					if (received > 0)
					{
						strcat(buf,"\r\n");
						CString cstrShow = pServer->m_ConnectBth + " 说：\r\n" + CString(buf);
						pServer->m_richedit_net.SetSel(pServer->m_richedit_net.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
						pServer->m_richedit_net.ReplaceSel(cstrShow);
					}
					break;
				}
			case DIS_COM:
				{
					pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
					CString cstrTemp;
					cstrTemp.Format("%s 断开连接！\r\n",pServer->m_ConnectBth);
					pServer->m_edit_tip.ReplaceSel(cstrTemp);
					pServer->SetListInfoBTH(NULL,	2);
					break;
				}
			case SEND_COM:
				{
					received = 0;
					memset(buf,0,1024);
					received = recv(this->MyBthSocketSerCli,buf,len,0);
					if (received > 0)
					{
						CString cstrInfo = CString(buf);
						int iIndex1 = cstrInfo.Find("#");
						int iIndex2 = cstrInfo.Find("*");
						CString cstrComText = cstrInfo.Left(iIndex1);
						bool send = _ttoi(cstrInfo.Mid(iIndex1+1,iIndex2-iIndex1-1));
						bool recv = _ttoi(cstrInfo.Mid(iIndex2+1));
						
						if (send == 1)
						{
							pServer->m_com_send_mode.SetCheck(TRUE);
							pServer->m_com_send_mode_hex.SetCheck(FALSE);
						}
						if (send == 0)
						{
							pServer->m_com_send_mode.SetCheck(FALSE);
							pServer->m_com_send_mode_hex.SetCheck(TRUE);
						}
						if (recv == 1)
						{
							pServer->m_com_recv_mode.SetCheck(TRUE);
							pServer->m_com_recv_mode_hex.SetCheck(FALSE);

						}
						if (recv == 0)
						{
							pServer->m_com_recv_mode.SetCheck(FALSE);
							pServer->m_com_recv_mode_hex.SetCheck(TRUE);
						}

						pServer->m_richedit_send_com.SetWindowText(cstrComText);

						CString cstrShow = pServer->m_ConnectBth + " 发来串口消息：" + cstrComText + "\r\n" ;
						pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
						pServer->m_edit_tip.ReplaceSel(cstrShow);
						pServer->OnBnClickedButtonManualsend();
					}
						break;
				}
			case SER_SEND_AUTO:
				{
					received = 0;
					memset(buf,0,1024);
					received = recv(this->MyBthSocketSerCli,buf,len,0);
					if (received > 0)
					{
						CString cstrInfo = CString(buf);
						int iIndex1 = cstrInfo.Find("#");
						int iIndex2 = cstrInfo.Find("*");
						CString cstrComText = cstrInfo.Left(iIndex1);
						bool send = _ttoi(cstrInfo.Mid(iIndex1+1,iIndex2-iIndex1-1));
						bool recv = _ttoi(cstrInfo.Mid(iIndex2+1,1));
						CString cstrTime = cstrInfo.Mid(iIndex2+2);

						if (send == 1)
						{
							pServer->m_com_send_mode.SetCheck(TRUE);
							pServer->m_com_send_mode_hex.SetCheck(FALSE);
						}
						if (send == 0)
						{
							pServer->m_com_send_mode.SetCheck(FALSE);
							pServer->m_com_send_mode_hex.SetCheck(TRUE);
						}
						if (recv == 1)
						{
							pServer->m_com_recv_mode.SetCheck(TRUE);
							pServer->m_com_recv_mode_hex.SetCheck(FALSE);

						}
						if (recv == 0)
						{
							pServer->m_com_recv_mode.SetCheck(FALSE);
							pServer->m_com_recv_mode_hex.SetCheck(TRUE);
						}

						pServer->m_richedit_send_com.SetWindowText(cstrComText);
						pServer->m_edit_autosend.SetWindowText(cstrTime);

						CString cstrShow = pServer->m_ConnectBth + " 发来串口消息：" + cstrComText + "\r\n" ;
						pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
						pServer->m_edit_tip.ReplaceSel(cstrShow);
						pServer->OnAutoSend();
					}
					break;
				}
			case STOP_SER_SEND_AUTO:
				{
					pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
					CString cstrTemp;
					cstrTemp.Format("%s 停止自动发送！\r\n",pServer->m_ConnectBth);
					pServer->m_edit_tip.ReplaceSel(cstrTemp);
					pServer->SetDlgItemText(IDC_BUTTON_AUTOSEND,"自动发送");
					pServer->KillTimer(1);
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
					memset(data,0,1024);
					received = recv(this->MyBthSocketSerCli,buf,len,0);
					strcpy(fn,buf);
					received = 0;
					memset(buf,0,1024);
					received = recv(this->MyBthSocketSerCli,buf,sizeof(long),0);

					filelength = *((long*)(buf));
					if (file.Open(fn,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
					{
						pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
						pServer->m_edit_tip.ReplaceSel("I>接受文件中请稍后...\r\n");
						while (1)
						{
							i = recv(this->MyBthSocketSerCli,data,1024,0);
							if (i > 0)
							{
								file.Write(data,i);
								filelength = filelength - i;
							}
							if (filelength < 0)
							{
								break;
							}
						}
						file.Close();
						pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
						pServer->m_edit_tip.ReplaceSel("I>文件接受成功！\r\n");
					}
					break;
				}
			}
		}
	}
	pServer->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
	pServer->m_edit_tip.ReplaceSel("I>退出服务器信息线程！\r\n");
}


//客户端接受信息
void CMyBlueTooth::RecvBthCli()
{
	CHARFORMAT cf;
	ZeroMemory(&cf,sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.dwEffects = 0;
	cf.yHeight = 80;
	cf.crTextColor = RGB(255,0,0);

	char buf[1024];
	int received;
	char cmd;
	int len;
	while(bThread2)
	{
		received = 0;
		memset(buf,0,1024);
		received = recv(this->MyBthSocketCli,buf,5,0);
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
					received = recv(this->MyBthSocketCli,buf,len,0);
					if (received > 0)
					{
						strcat(buf,"\r\n");
						CString cstrTemp(buf);
						CString cstrShow = this->cstrSelectBTHName + " 说：\r\n" + cstrTemp	 ;
						pBthListDlg->m_richedit_bth.SetSel(pBthListDlg->m_richedit_bth.GetWindowTextLength(),pBthListDlg->m_richedit_bth.GetWindowTextLength());
						pBthListDlg->m_richedit_bth.ReplaceSel(cstrShow);

						//0~0即未选中
						pBthListDlg->m_richedit_bth.SetSel(0,2);
						pBthListDlg->m_richedit_bth.SetSelectionCharFormat(cf);
						//自动垂直滚动
						pBthListDlg->m_richedit_bth.LineScroll(pBthListDlg->m_richedit_bth.GetLineCount());
					}
					break;
				}
			case CNN_COM:
				{
					pBthListDlg->m_richedit_bth.SetSel(pBthListDlg->m_richedit_bth.GetWindowTextLength(),pBthListDlg->m_richedit_bth.GetWindowTextLength());
					pBthListDlg->m_richedit_bth.ReplaceSel("我 进入串口通道！\r\n");
					pClient->m_edit_tip.SetSel(pServer->m_edit_tip.GetWindowTextLength(),pServer->m_edit_tip.GetWindowTextLength());
					pClient->m_edit_tip.ReplaceSel("I>进入串口通道,可以发送串口数据\r\n");

					pClient->m_button_send_com.EnableWindow(TRUE);
					pClient->m_button_auto_sendcom.EnableWindow(TRUE);
					pBthListDlg->m_btn_send_bth.EnableWindow(FALSE);
					break;
				}
			case SER_SEND:
				{
					received = 0;
					memset(buf,0,1024);
					received = recv(this->MyBthSocketCli,buf,len,0);
					if (received > 0)
					{
						strcat(buf,"\r\n");
						pBthListDlg->m_richedit_bth.SetSel(pBthListDlg->m_richedit_bth.GetWindowTextLength(),pBthListDlg->m_richedit_bth.GetWindowTextLength());
						pBthListDlg->m_richedit_bth.ReplaceSel(CString(buf));
					}
					break;
				}
			case COM_CNT:
				{
					received = 0;
					memset(buf,0,1024);
					received = recv(this->MyBthSocketCli,buf,len,0);

					iRecvCount += received;
					pClient->SetDlgItemInt(IDC_EDIT_RECV,iRecvCount);
					if (received > 0)
					{
						pClient->m_richedit_recv.SetWindowText(CString(buf));
					}
					break;
				}
			}
		}
	}
}