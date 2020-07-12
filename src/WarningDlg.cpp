// WarningDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "WarningDlg.h"


// CWarningDlg 对话框

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


// CWarningDlg 消息处理程序

BOOL CWarningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HICON icon;
	icon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);

	SetIcon(icon,true);
	SetIcon(icon,false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
