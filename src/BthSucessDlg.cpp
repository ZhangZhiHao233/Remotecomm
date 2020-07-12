// BthSucessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteComm.h"
#include "BthSucessDlg.h"


// CBthSucessDlg �Ի���

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


// CBthSucessDlg ��Ϣ�������

BOOL CBthSucessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON icon;

	icon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);

	SetIcon(icon,true);
	SetIcon(icon,false);

	// ����һ�ѻ�ɫ�ı���ˢ��   Modify:5.24��ɫ�ѿ�������Windows��׼��ɫ
	m_bkBrush.CreateSolidBrush(RGB(217,217,219));   

	::SetWindowLong( m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);  
	::SetLayeredWindowAttributes( m_hWnd, 0, 200, LWA_ALPHA); // 120��͸���ȣ���Χ��0��255  
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CBthSucessDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(nCtlColor == CTLCOLOR_DLG)   // �ж��Ƿ��ǶԻ���  
	{  
		return   m_bkBrush; // ���ظղŴ����ı���ˢ��  
	}  
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
