#pragma once

// CMySocketCln ����Ŀ��

class CMySocketCln : public CSocket
{
public:
	CMySocketCln();
	virtual ~CMySocketCln();
	virtual void OnReceive(int nErrorCode);
};


