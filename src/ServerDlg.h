#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"//IDD_DIALOGδ����
#include "mscomm1.h"

// CServerDlg �Ի���

class CServerDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerDlg)

public:
	CServerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg LRESULT OnTCPServer(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUDPServer(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonStartserver();
	void SetListInfo(SOCKET socketcli,CString  cstrIP,CString cstrInfo,int iMode);
	void SetListInfoUDP(CString& cstrIP,CString cstrInfo,int iMode);
	void SetListText(CString  cstrIP,int iMode);
	void ChooseSelect(CString cstrIP,int iMode);
	void ChangeEditList();
	CString InitEditText();
	void ChangeList(int iMode);
	void SetListInfoBTH(CString cstrSend,int iMode = 0);
	virtual BOOL OnInitDialog();
	void	OnInitComDate();
	void  OnSaveDate(int iMode);
	CString GetBthName(CString cstrAdress);
	void ClearAll();
	CButton m_btn_openserver;
	// //����������˵����
	CEdit m_edit_tip;
	// �����ӿͻ����б�
	CListCtrl m_list_cli;
	// �����������Ϣ��
	CRichEditCtrl m_richedit_net;
	CEdit m_edit_net_send;
	afx_msg void OnButtonNetSend();
	// ����ͨ��
	CComboBox m_comb_channel;
	afx_msg void OnchangeComboChannel();
	CComboBox m_combo_com;
	CComboBox m_comb_baud;
	CComboBox m_comb_date;
	CComboBox m_comb_stop;
	CComboBox m_comb_check;
	afx_msg void OnEnChangeRichedit22();
	CRichEditCtrl m_richedit_recv_com;
	CRichEditCtrl m_richedit_send_com;
	DECLARE_EVENTSINK_MAP()
	void OnComm();
	// ����ͨ�ſؼ�����
	CMscomm1 m_ctrlComm;

public:
	CString m_strRXData;
public:
	CString m_strTXData;
	CString m_ConnectBth;
public:
	CButton m_com_recv_mode;
	CButton m_com_send_mode;
	CButton m_com_recv_mode_hex;
	CButton m_com_send_mode_hex;

	char ConvertHexChar(char ch);
	int SringHex(CString str,CByteArray &senddate);
	afx_msg void OnBnClickedButtonOpencom();
	afx_msg void OnBnClickedButtonManualsend();
	afx_msg void OnChooseSTC();
	afx_msg void OnDownload();
	afx_msg void OnClearRecv();
	afx_msg void OnClearSend();
	afx_msg void OnSaveRecv();
	afx_msg void OnSaveSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAutoSend();
	CEdit m_edit_autosend;
	afx_msg void OnClear();
public:
	afx_msg void OnPaint();
	bool send_mode;
	bool recv_mode;
	afx_msg void OnBnClickedRadioRecvText();
	afx_msg void OnBnClickedRadioRecvHex();
	afx_msg void OnBnClickedRadioSendText();
	afx_msg void OnBnClickedRadioSendHex();
	CButton m_btn_opencom;
};
