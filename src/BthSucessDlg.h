#pragma once


// CBthSucessDlg �Ի���

class CBthSucessDlg : public CDialog
{
	DECLARE_DYNAMIC(CBthSucessDlg)

public:
	CBthSucessDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBthSucessDlg();
	CBrush m_bkBrush;  
// �Ի�������
	enum { IDD = IDD_DIALOG_BTHTIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
