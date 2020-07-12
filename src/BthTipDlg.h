#pragma once

// CBthTipDlg 对话框

class CBthTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CBthTipDlg)

public:
	CBthTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBthTipDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BTHTIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};
