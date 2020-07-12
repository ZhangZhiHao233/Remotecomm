// MySocketCln.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "MySocketCln.h"
#include "ClientDlg.h"
#include "MyUdp.h"
extern CClientDlg* pClient;
extern int g_protocol;
extern CMyUdp g_UDP_Cli;

// CMySocketCln

CMySocketCln::CMySocketCln()
{
}

CMySocketCln::~CMySocketCln()
{
}


// CMySocketCln ��Ա����

void CMySocketCln::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	char chRecv[1024] = {0};
	if (g_protocol == TCP_MODE)
	{
		Receive(chRecv,1024,0);
	}
	else if (g_protocol == UDP_MODE)
	{
		UINT port;
		CString cstrIP;
		ReceiveFrom(chRecv,1024,cstrIP,port,0); 
	}

	CString cstrRecv = CString(chRecv);
	if (_ttoi(chRecv) == SELECT_COM)
	{
		//���봮��ͨ��
		pClient->SetListInfo(NULL,2);
		pClient->m_button_send_com.EnableWindow(TRUE);
		pClient->m_button_auto_sendcom.EnableWindow(TRUE);
		pClient->m_button_send_net.EnableWindow(FALSE);
	}
	else if (cstrRecv.Find('$') == 0)
	{
		pClient->UpdateComInfo(cstrRecv.Mid(1));
	}
	else
		pClient->SetListInfo(cstrRecv,1);

	CSocket::OnReceive(nErrorCode);
}
