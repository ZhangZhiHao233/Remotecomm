// BthSucessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "BthSucessDlg.h"


// CBthSucessDlg 对话框

IMPLEMENT_DYNAMIC(CBthSucessDlg, CDialog)

CBthSucessDlg::CBthSucessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBthSucessDlg::IDD, pParent)
{

}

CBthSucessDlg::~CBthSucessDlg()
{
}

void CBthSucessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBthSucessDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CBthSucessDlg 消息处理程序

BOOL CBthSucessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON icon;

	icon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);

	SetIcon(icon,true);
	SetIcon(icon,false);

	// 创建一把黄色的背景刷子   Modify:5.24黄色难看！改用Windows标准白色
	m_bkBrush.CreateSolidBrush(RGB(217,217,219));   

	::SetWindowLong( m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);  
	::SetLayeredWindowAttributes( m_hWnd, 0, 200, LWA_ALPHA); // 120是透明度，范围是0～255  
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CBthSucessDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor == CTLCOLOR_DLG)   // 判断是否是对话框  
	{  
		return   m_bkBrush; // 返回刚才创建的背景刷子  
	}  
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
