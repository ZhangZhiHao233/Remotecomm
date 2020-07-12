#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CBthListDlg 对话框

class CBthListDlg : public CDialog
{
	DECLARE_DYNAMIC(CBthListDlg)

public:
	CBthListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBthListDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BTHLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_bth;
	CRichEditCtrl m_richedit_bth;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListBth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListBth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBthConnect();
	afx_msg void OnBnClickedButtonBthSend();
	CEdit m_bth_edit_send;
	afx_msg void OnPaint();
	CButton m_btn_send_bth;
};
