/********************************************************************
	created:	2002/04/18
	created:	18:4:2002   15:29
	filename: 	C:\Work\Project Red Stone\Red Stone\cPACKET_BASE.h
	file path:	C:\Work\Project Red Stone\Red Stone
	file base:	cPACKET_BASE
	file ext:	h
	author:		Young-chan LEE

	purpose:	패킷의 기본 정보들을 정의 한다.
*********************************************************************/

#ifndef _classPACKET_BASE_H
#define _classPACKET_BASE_H



#ifndef _WINDOWS_
#include <windows.h>
#endif
#include "RSComm.h"
#include "partyDefine.h"

/*
	0x1000	~ 유저  ->  링크 혹은 게임 서버  에게 
	0x2000 ~ 링크 혹은 게임 서버  -> 유저 에게 

	0x3001 ~ 채널 서버가 링크 서버에게 
	0x3101	~ 채널 서버가 DB서버에게 
	0x3401	~ DB서버가 채널서버에게 

	0x4001 ~ 채널 서버가 게임 서버에게 
	0x4301 ~ 채널 서버가 DB서버에게 
	0x4701	~ DB서버가 채널 서버에게 
	
	0x5001	~ 게임 서버가 채널 서버에게
	0x5301 ~ 게임 서버가 DB서버에게 
	0x5701	~ DB서버가 게임 서버에게 

	0x6001 ~ 각 서버들 -> 로그 서버에게...

	0x7001 ~ Broadcast 서버를 경유한다면 ~ 
*/
#define	dMESSAGE_BUFFER_SIZE		1024
#define dMAX_COMPRESS_SIZE			41960




#define	dPROTECT_NONE							0
#define	dPROTECT_ORCRYPT						1
#define	dPROTECT_SEED							2
#define	dPROTECT_COMPRESSED						3

#pragma pack(1)


class	cMSG_BASE_TYPE
{
public:
	WORD	wSize,
			wType,
			wProtectType;
	inline	void	set(WORD size,WORD type,WORD protectType=0)	{ wSize = size,wType = type,wProtectType=protectType;}
};

class	cMSG_BASE_TYPE_INNER
{
public:
	WORD	wSize,
			wType;
	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

#define	dCM_DUMMY						0x1000
#define dCM_COMPRESS					0x1fff
#define dSM_COMPLETESSL					0x1ffe
#define dCM_SYNCTIMER					0x1ffd



struct	CM_DUMMY
{
public:
	cMSG_BASE_TYPE	base;
};




class	SG_COMPLETE_SSL
{
public:
	cMSG_BASE_TYPE	base;

	WORD			wResult;
};




struct	CM_COMPRESS
{
public:
	WORD	wSize,
			wType;
	
	WORD	wSourceSize;


	unsigned char	data[dMAX_COMPRESS_SIZE];

	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

struct CM_SYNCTIMER
{
public:
	cMSG_BASE_TYPE	base;


};


class	CPartyInfoForList
{
public:
	WORD				m_wPartySerial;
	char				m_strPartyName[dPARTYNAME_LENGTH];
	char				m_strLeaderName[dNAME_LENGTH];

	DWORD				m_bf10LeaderField	:	10;
	DWORD				m_bf10LeaderLevel	:	10;
	DWORD				m_bf4LeaderJob		:	4;
	DWORD				m_bf4MemberCount	:	4;
	DWORD				m_bf3PartyObject	:	3;
};

enum
{
	ePW_LEAVE_PARTY,
	ePW_BANISH_MEMBER,
	ePW_CHANGE_LEADER,
	ePW_IS_CONNECT_USER,
};











class	CPartyMemberInfoInPacket
{
public:
	char				m_strName[dNAME_LENGTH];

	DWORD				m_bf8Job					:	8;
	DWORD				m_bf10Level					:	10;
	DWORD				m_bf9IF						:	9;
	DWORD				m_bf1IsDisconnected			:	1;
	DWORD				m_bf4IFFloor				:	4;

	WORD				m_bf4PlaySecretDungeonCount	:	4;

	int					m_iDuelVP;

	WORD				m_wFieldSerial;
	WORD				m_wSerial;
	
};

class	CPartyInfoInPacket
{
public:
	char						m_strName[dPARTYNAME_LENGTH];

	WORD						m_bf1IsCreate			:	1;
	WORD						m_bf10Serial			:	10;
	WORD						m_wFieldUniqueSerial;
	CPartyMemberInfoInPacket	m_aMemberList[dPARTY_MAXPLAYER];

	WORD						m_wGateField;
	
	DWORD						m_bf4MemberCount		:	4;
	DWORD						m_bf10GateArea			:	10;
	DWORD						m_bf3GoldShareMethod	:	3;
	DWORD						m_bf3ItemShareMethod	:	3;
	DWORD						m_bf3PartyObject		:	3;
	DWORD						m_bf1IsOpenParty		:	1;
};

#pragma pack()

#endif


/*
		MSG INDEX
		- 0x0001 ~ 0x1000		:	공통

		LOGIN SERVER 
		- 0x1001 ~ 0x1200		:	Login ~ Client

		LOGIN ~ WORLD
		- 0x1201 ~ 0x1400		:	Login ~ World

		LOGIN ~ USER DB
		- 0x1401 ~ 0x1600		:	Login ~ User DB




		WORLD SERVER
		0x8001 ~ 

		AVATAR DB SERVER 
		0x4001 ~
		
*/