#pragma once


// CBthTipFileDlg �Ի���

class CBthTipFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CBthTipFileDlg)

public:
	CBthTipFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBthTipFileDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BTHTIP_SUCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
