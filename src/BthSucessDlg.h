#pragma once


// CBthSucessDlg 对话框

class CBthSucessDlg : public CDialog
{
	DECLARE_DYNAMIC(CBthSucessDlg)

public:
	CBthSucessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBthSucessDlg();
	CBrush m_bkBrush;  
// 对话框数据
	enum { IDD = IDD_DIALOG_BTHTIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
