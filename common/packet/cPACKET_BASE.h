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


//#include <winsock2.h>
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
#define	dMESSAGE_BUFFER_SIZE		1024	//	메시지 버퍼 최대 사이즈
#define dMAX_COMPRESS_SIZE			41960	//	압축했을때의 최대 사이즈

//	모든 문자열은 실제 제한 길이에 +2를 해 준다. 그러니까.. 밑에것들에서 -2를 하면 실제 가능한 길이가 되겠다.


#define	dPROTECT_NONE							0			//	아무런 보호 장치가 없음
#define	dPROTECT_ORCRYPT						1			//	간단한 OR 암호화 한다.
#define	dPROTECT_SEED							2			//	SEED로 암호화 한다.
#define	dPROTECT_COMPRESSED						3			//	압축했다.

#pragma pack(1)

//	기본 패킷
class	cMSG_BASE_TYPE
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType,				//	패킷 타입
			wProtectType;		//	보안 타입
	inline	void	set(WORD size,WORD type,WORD protectType=0)	{ wSize = size,wType = type,wProtectType=protectType;}
};

class	cMSG_BASE_TYPE_INNER
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType;				//	패킷 타입
	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

#define	dCM_DUMMY						0x1000	//	더미(공통 메시지)
#define dCM_COMPRESS					0x1fff	//	암축된 패킷
#define dSM_COMPLETESSL					0x1ffe	//	ssl 인증 성공 여부
#define dCM_SYNCTIMER					0x1ffd	//	시간을 동기화 한다. 간격은 1분정도..

//	더미 패킷 - 클라이언트가 접속후에 접속 확인차 보내는 메시지(서버에서는 무시하면 된다.)
//	게임서버외에는 사용하지마.. 
struct	CM_DUMMY
{
public:
	cMSG_BASE_TYPE	base;
};

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃
//┃ssl 인증 관련
class	SG_COMPLETE_SSL
{
public:
	cMSG_BASE_TYPE	base;

	WORD			wResult;	//	0 - fail, 1 - success
};
//┃
//┃ssl 인증 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

struct	CM_COMPRESS
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType;				//	패킷 타입
	
	WORD	wSourceSize;		//	압축된 데이타의 패킷 사이즈
//			wSourceType;		//	압축된 데이타의 패킷 유형

	unsigned char	data[dMAX_COMPRESS_SIZE];

	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

struct CM_SYNCTIMER
{
public:
	cMSG_BASE_TYPE	base;


};
//
//	파티의 간단 정보
class	CPartyInfoForList
{
public:
	WORD				m_wPartySerial;	//	파티 시리얼
	char				m_strPartyName[dPARTYNAME_LENGTH];	//	파티 이름
	char				m_strLeaderName[dNAME_LENGTH];

	DWORD				m_bf10LeaderField	:	10;
	DWORD				m_bf10LeaderLevel	:	10;
	DWORD				m_bf4LeaderJob		:	4;
	DWORD				m_bf4MemberCount	:	4;
	DWORD				m_bf3PartyObject	:	3;	//파티 목적
};

enum
{
	ePW_LEAVE_PARTY,
	ePW_BANISH_MEMBER,
	ePW_CHANGE_LEADER,
	ePW_IS_CONNECT_USER,
};
// 
// enum
// {
// 	ePW_LEAVE_UNION_PARTY,
// 	ePW_BANISH_UNION_MEMBER,				//기능은 없지만 디버깅의 편의성을 위해 파티메세지와 구조를 비슷하게 한다. 메시지니까.
// 	ePW_CHANGE_UNION_LEADER,
// 	ePW_IS_CONNECT_USER,
// };

//
//	파티 멤버 정보(파티 정보 업데이트시 사용)
class	CPartyMemberInfoInPacket
{
public:
	char				m_strName[dNAME_LENGTH];	//	이름

	DWORD				m_bf8Job					:	8;
	DWORD				m_bf10Level					:	10;
	DWORD				m_bf9IF						:	9;
	DWORD				m_bf1IsDisconnected			:	1;
	DWORD				m_bf4IFFloor				:	4;

	WORD				m_bf4PlaySecretDungeonCount	:	4;

	int					m_iDuelVP;

	WORD				m_wFieldSerial;
	WORD				m_wSerial;					//	W->S 일때는 클라이언트 시리얼, S->C일때는 cACTOR의 필드 시리얼(같은 필드가 아닐때는 0xffff)
	
};

class	CPartyInfoInPacket
{
public:
	char						m_strName[dPARTYNAME_LENGTH];	//	파티 이름

	WORD						m_bf1IsCreate			:	1;	//	생성되는 거냐?
	WORD						m_bf10Serial			:	10;	//	시리얼
	WORD						m_wFieldUniqueSerial;			//	필드 고유 시리얼
	CPartyMemberInfoInPacket	m_aMemberList[dPARTY_MAXPLAYER];//	파티원들의 기본 정보 - 첫번째는 무조건 리더

	WORD						m_wGateField;
	
	DWORD						m_bf4MemberCount		:	4;
	DWORD						m_bf10GateArea			:	10;
	DWORD						m_bf3GoldShareMethod	:	3;
	DWORD						m_bf3ItemShareMethod	:	3;
	DWORD						m_bf3PartyObject		:	3;	//파티 목적
	DWORD						m_bf1IsOpenParty		:	1;	//멤버를 계속 모집한다.
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