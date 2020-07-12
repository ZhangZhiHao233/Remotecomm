// RemoteCommDlg.h : ͷ�ļ�
//

#pragma once


// CRemoteCommDlg �Ի���
class CRemoteCommDlg : public CDialog
{
// ����
public:
	CRemoteCommDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REMOTECOMM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CMenu m_menu;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateMserver(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMclient(CCmdUI *pCmdUI);
	afx_msg void OnMserver();
	afx_msg void OnMclient();
	afx_msg void OnMtcp();
	afx_msg void OnMudp();
	afx_msg void OnMbluetooth();
	afx_msg void OnUpdateMtcp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMudp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMbluetooth(CCmdUI *pCmdUI);
public:
	void OnCreateServerDlg();
	void OnCreateClientDlg();
	void WriteLogText(CString m_cstrLogContent);
	BOOL JudgeWindowsVersion();
	afx_msg void OnAbout();
};
