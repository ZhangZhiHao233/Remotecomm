#pragma once


// CWarningDlg 对话框

class CWarningDlg : public CDialog
{
	DECLARE_DYNAMIC(CWarningDlg)

public:
	CWarningDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWarningDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
