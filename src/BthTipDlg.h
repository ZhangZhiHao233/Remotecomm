#pragma once

// CBthTipDlg �Ի���

class CBthTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CBthTipDlg)

public:
	CBthTipDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBthTipDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BTHTIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};
