// RemoteComm.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRemoteCommApp:
// �йش����ʵ�֣������ RemoteComm.cpp
//

class CRemoteCommApp : public CWinApp
{
public:
	CRemoteCommApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRemoteCommApp theApp;