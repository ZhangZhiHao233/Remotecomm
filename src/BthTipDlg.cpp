// BthTipDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "BthTipDlg.h"

// CBthTipDlg �Ի���

IMPLEMENT_DYNAMIC(CBthTipDlg, CDialog)

CBthTipDlg::CBthTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBthTipDlg::IDD, pParent)
{

}

CBthTipDlg::~CBthTipDlg()
{
}

void CBthTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBthTipDlg, CDialog)
END_MESSAGE_MAP()


// CBthTipDlg ��Ϣ�������



BOOL CBthTipDlg::OnInitDialog()
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
