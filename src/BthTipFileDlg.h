#pragma once


// CBthTipFileDlg 对话框

class CBthTipFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CBthTipFileDlg)

public:
	CBthTipFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBthTipFileDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BTHTIP_SUCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
