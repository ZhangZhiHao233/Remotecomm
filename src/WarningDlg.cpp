// WarningDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "WarningDlg.h"


// CWarningDlg �Ի���

IMPLEMENT_DYNAMIC(CWarningDlg, CDialog)

CWarningDlg::CWarningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningDlg::IDD, pParent)
{

}

CWarningDlg::~CWarningDlg()
{
}

void CWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWarningDlg, CDialog)
END_MESSAGE_MAP()


// CWarningDlg ��Ϣ�������

BOOL CWarningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HICON icon;
	icon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);

	SetIcon(icon,true);
	SetIcon(icon,false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
