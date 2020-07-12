#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CClientDlg 对话框

class CClientDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientDlg)

public:
	CClientDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClientDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 服务器IP控件
	CIPAddressCtrl m_ipadress;
	virtual BOOL OnInitDialog();
	// 客户端提示信息编辑框
	CEdit m_edit_tip;
public:
	CString InitEditText();
	afx_msg void OnBnClickedButtonCntSer();
	// 连接服务器按钮
	CButton m_button_cntser;
	// 客户端网络信息编辑框
	CRichEditCtrl m_richedit_net_cli;
	afx_msg void OnButtonSendNet();
	void SetListInfo(CString cstrInfo,int iMode);
	void SetBthList(int iMode);
	void OnInitComDate();
	void UpdateComInfo(CString cstrInfo);
	void OnSaveDate(int iMode);
	CStringArray* DivString(CString test);
	CEdit m_edit_net_cli;
	CString m_str_net_cli;
	// 网络信息发送按钮
	CButton m_button_send_net;
	afx_msg void OnDownloadhex();
	CButton m_radio_recvmode_cli;
	CButton m_radio_sendmode_cli;
	CButton m_radio_recvmode_cli_hex;
	CButton m_radio_sendmode_cli_hex;
	CComboBox m_combo_com_cli;
	CComboBox m_combo_barud_cli;
	CComboBox m_combo_date_cli;
	CComboBox m_combo_stop_cli;
	CComboBox m_combo_check_cli;
	afx_msg void OnSendcom();
	afx_msg void OnBnClickedButtonClearRecv();
	afx_msg void OnBnClickedButtonSaveRecv();
	afx_msg void OnBnClickedButtonClearSend();
	CRichEditCtrl m_richedit_recv;
	CRichEditCtrl m_richedit_send;
	CString m_richedit_recv_string;
	CString m_richedit_send_string;
	CButton m_button_send_com;
	CButton m_button_auto_sendcom;
	afx_msg void OnAutosend();
	CEdit m_edit_send;
	CEdit m_edit_recv;
	BOOL send_mode_cli;
	BOOL recv_mode_cli;
	afx_msg void OnZeroCount();
	afx_msg void OnBthDisCnt();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRadioTextRecvCli();
	afx_msg void OnBnClickedRadioHexRecvCli();
	afx_msg void OnBnClickedRadioTextSendCli();
	afx_msg void OnBnClickedRadioHexSendCli();
	afx_msg void OnBnClickedButtonSaceSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
