// BthTipFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "BthTipFileDlg.h"


// CBthTipFileDlg 对话框

IMPLEMENT_DYNAMIC(CBthTipFileDlg, CDialog)

CBthTipFileDlg::CBthTipFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBthTipFileDlg::IDD, pParent)
{

}

CBthTipFileDlg::~CBthTipFileDlg()
{
}

void CBthTipFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBthTipFileDlg, CDialog)
END_MESSAGE_MAP()


// CBthTipFileDlg 消息处理程序
