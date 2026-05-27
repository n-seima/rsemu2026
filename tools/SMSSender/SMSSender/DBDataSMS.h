// DabaBase 俊 包访等 单捞鸥甫 包府
//////////////////////////////////////////////////////////////////////
#pragma once

#include "AdoDB.h"



class cDBDataSMS
{
private:
	cAdoDB m_DB;
	
public:
	

public:
	cDBDataSMS(void);
	~cDBDataSMS(void);

	BOOL connect();
	void disconnect();		
	BOOL sendSMS(TCHAR *_pszRecvPhone, TCHAR *_pszSendPhone, TCHAR *_pszContent);
};
