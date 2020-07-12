// RemoteCommDlg.h : 头文件
//

#pragma once


// CRemoteCommDlg 对话框
class CRemoteCommDlg : public CDialog
{
// 构造
public:
	CRemoteCommDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REMOTECOMM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CMenu m_menu;
	// 生成的消息映射函数
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
