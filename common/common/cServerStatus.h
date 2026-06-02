#ifndef __CSERVERSTATUS_H
#define __CSERVERSTATUS_H

#include "definePacketData.h"
#include "cADO_BASE.h"

#include <deque>

//	
//	에러 로그를 남기기 위한 특수 클래스.
//	운영자들에게 서버의 상태를 쉽게 확인할 수 있도록 하기 위해, 모든 서버에 포함된다.

//	viewer측에서는 로그를 
class cSSInfoTable
{
public:
	int	iIdx;
	UTime	utTime;
	char strName[64];
	char strIp[32];
	int iErrorType;
	int Param1_int;
	char Param2_char512[512];

	cSSInfoTable(){}
	virtual ~cSSInfoTable(){}

	int operator==(const cSSInfoTable& _info)
	{
		if( iIdx == _info.iIdx )	return TRUE;
		else						return FALSE;
	}
	void operator=(const cSSInfoTable * _info)
	{
		memcpy(this,_info,sizeof(cSSInfoTable));
	}
};


class cServerStatus	: public cADO_BASE
{
private:
	cServerStatus();

	char	m_strIp[32];

	char	m_strUserDBServerSource[128];
	char	m_strUserDBServerId[64];
	char	m_strUserDBServerPw[64];
	char	m_strUserDBIP[32];

public:
	char	m_strServerName[64];

	std::deque<cSSInfoTable>	m_listSSInfo;
	std::deque<cSSInfoTable>	m_listSSError;

	enum
	{
		//	ERROR LEVEL 5 : 일반적인 상태 전송
		eERROR_TYPE_SERVERSTATUS = 0,		//	서버의 현재 상태를 전송
		eERROR_TYPE_GAME_UPDATE_STATUS	,	//	게임 서버의 게임 업데이트 상황 전송

		//	ERROR LEVEL 2 : 문제 발생 직전..
		eERROR_TYPE_DB_DISKFULL = 701,		//	서버의 디스크가 200M이하가 된다면 경고를 보낸다.

		//	ERROR LEVEL 1 : 실제 문제 발생. 긴급 처리할 것..
		eERROR_TYPE_DB_SHUTDOWN = 901,		//	DB에 쿼리가 정상적으로 전송이 되지 않을 경우 (DB ERROR가 50회 연속으로 발생할 경우)
	};
	static cServerStatus & getInstance()
	{
		static cServerStatus ss;
		return ss;
	}
	~cServerStatus();

	//	프로젝트 시작시 실행할 것!!
	BOOL	init(char * _worldname, char * _subname);		//계정 정보를 읽고, DB에 접속한다.
	BOOL	isReady()
	{
		if(stricmp(m_strServerName,"WORLDNAME")==0)	return FALSE;
		else										return TRUE;
	}
	void	release();	//DB와의 접속 정보를 해지한다.

	//	서버를 위한
	int		update(int _iErrorType, int _iParam1, char * _strParam2 );	//에러 로그를 남겨라.
	
	//	Viewer를 위한
	int		getSS();	//	서버들의 상태를 가져온다.
	int		getError(BOOL isFirst = FALSE);	//	치명적인 에러들이 있는지 확인한다.	//	isFirst -> 막 실행한 경우?
};

#define g_SS	cServerStatus::getInstance()

#endif