// cADO_COMMAND.h: interface for the cADO_COMMAND class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADO_COMMAND_H__DD8477E4_8F73_4229_BF2E_D4764B8F4DDA__INCLUDED_)
#define AFX_CADO_COMMAND_H__DD8477E4_8F73_4229_BF2E_D4764B8F4DDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "definePacketData.h"
#include "cPACKET_USERDBSERVER.h"
#include "cADO_BASE.h"
#include "adCommandDefine.h"

class cADO_COMMAND		:	public cADO_BASE
{
public:
	cADO_COMMAND();
	virtual ~cADO_COMMAND();

	int		GetCommand(cAdCommandData * _command);	//	먼저 실행할 명령을 하나 DB로 부터 가져온다.
#ifdef _FOR_KOREA
	int		UpdateServerInfo(SERVERINFO * _info, DWORD *);
#else
	int		UpdateServerInfo(SERVERINFO * _info);
#endif

	//////////////////////////////////////////////////////////////////////////
	//	LOG 처리
	int		logInOut(WU_LOG_INOUT* _data);
	int		logQuest(WU_LOG_QUEST* _data);
	//	LOG 처리
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_CADO_COMMAND_H__DD8477E4_8F73_4229_BF2E_D4764B8F4DDA__INCLUDED_)
