#pragma once

// CMySocketCln ÃüÁîÄ¿±ê

class CMySocketCln : public CSocket
{
public:
	CMySocketCln();
	virtual ~CMySocketCln();
	virtual void OnReceive(int nErrorCode);
};


