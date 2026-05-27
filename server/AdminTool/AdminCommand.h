// AdminCommand.h: interface for the CAdminCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADMINCOMMAND_H__17B50882_4992_4297_A672_A892DB01530B__INCLUDED_)
#define AFX_ADMINCOMMAND_H__17B50882_4992_4297_A672_A892DB01530B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RSComm.h"
#include "cADO_BASE.h"
#include "adCommandDefine.h"
//	각 서버와의 명령 전달, 현재 상황을 실시간으로 파악하기 위해 처리한다.

class CAdminCommand  :	public cADO_BASE
{
private:
	char	commanddbip[dIP_SIZE];
	char	commanddbname[64];

	char	commandID[32];
	char	commandPW[32];
	int		commandPort;
	int		commandCount;

public:
	cAdCommandData	* cAdCommandList;

	CAdminCommand();
	virtual ~CAdminCommand();

	BOOL	isCanConnect;

	BOOL	init(char * ip,char * dbname);
	BOOL	connectCommandDB();
	BOOL	disCommandDB();

	//	SP 관련
	int		GetCommandList(int _isBackUpList);	//	리스트의 갯수를 돌린다.
	int		ReserveCommand(UTime _regTime,UTime _finTime,int comType, int comTarget, int param1, int param2,char * param3,char * msg, char * who);
	int		DeleteCommand(int idx,char * whod);
	int		ModifyCommand(int idx,UTime _regTime,UTime _finTime,int comType, int comTarget, int param1, int param2,char * param3,char * msg, char * who);
	//	inline
	inline int	GetListCount(){	return commandCount;	}
};

extern CAdminCommand	g_adCommand;

#endif // !defined(AFX_ADMINCOMMAND_H__17B50882_4992_4297_A672_A892DB01530B__INCLUDED_)
