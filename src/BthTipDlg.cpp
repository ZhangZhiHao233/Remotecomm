// BthTipDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "BthTipDlg.h"

// CBthTipDlg 对话框

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


// CBthTipDlg 消息处理程序



BOOL CBthTipDlg::OnInitDialog()
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
