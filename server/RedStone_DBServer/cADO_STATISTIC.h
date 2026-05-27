#pragma once

#include "definePacketData.h"
#include <icrsint.h> 
#include "cADO_BASE.h"
#include "cPACKET_DBSERVER.h"
#include "RSComm.h"

class cADO_STAT	:	public cADO_BASE , public cSinglton<cADO_STAT>
{
public:
	virtual ~cADO_STAT();

	void spSend_GOLD(const char *_strServer, const char *_strID, const char *_strName, const LONGLONG _allGold, const LONGLONG _gettingGold);
	BOOL ConnectDB();

public:
	char		m_strIp[16];
	char		m_strId[32];
	char		m_strPw[32];
	char		m_strDBSource[64];
};

#define dSTATISTIC		cADO_STAT::GetInstance()