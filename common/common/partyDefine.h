#ifndef	_classPartyDefine_H
#define	_classPartyDefine_H

#include <WINSOCK2.H>
#include <WINDOWS.H>
#include "definePacketData.h"

#define	dPARTY_MEMBER_COUNT		8
#define	dPARTY_COUNT			1024
#define dPARTYNAME_LENGTH		22	//	∆ƒ∆º¿Ã∏ß ±Ê¿Ã


///	Party Work Result
#define	dPWR_SUCCESS								-1
#define	dPWR_CAN_NOT_FIND_PARTY						-2
#define	dPWR_CAN_NOT_FIND_MEMBER					-3
#define	dPWR_DISSOLUTION_PARTY						-4
#define	dPWR_PARTY_COUNT_FULL						-5
#define	dPWR_EXIST_SAME_NAME_PARTY					-6
#define	dPWR_CAN_NOT_ADD_MEMBER_BY_TOO_MANY_MEMBER	-7
#define	dPWR_VALID_PARTY							-8
#define	dPWR_ALREADY_EXIST_SAME_NAME_MEMBER			-9
#define	dPWR_RECONNECT								-10
#define	dPWR_CAN_NOT_FIND_UNION_PARTY				-11

//	dPW_DUP = Party Work Dissolution Union Party ø¨«’ ∆ƒ∆º «ÿ√º¿« ¿Ã¿Ø...
#define	dPW_DUP_BY_UNKNOWN							0
#define	dPW_DUP_BY_LEAVE_PARTY_MEMBER				1

#define	CREATE_UNION_PARTY_RESULT_OK		0
#define	CREATE_UNION_PARTY_RESULT_FAILED	1

const	int	c_iPartyProspectusLength	=	64;

//
//	Party Object
enum
{
	ePO_HUNT,	//	ªÁ≥…
	ePO_EXPLORATION,	//	≈Ω«ÅE	
	ePO_QUEST,	//	ƒ˘Ω∫∆Æ «ÿ∞ÅE
};

//
//	Party Gold Share Method
enum
{
	ePGSM_FREE,
	ePGSM_EQUALITY,
	ePGSM_ORDER,
};

//
//	Party Item Share Method
enum
{
	ePISM_FREE,
	ePISM_RANDOM,
	ePISM_ORDER,
	ePISM_FIRST_JOB_RANDOM,
	ePISM_FIRST_JOB_ORDER,
};

//	Leave Party Cause
enum
{
	eLPC_LEAVE,
	eLPC_BANISH,
	eLPC_INCORRECT_PARTY_INFO,
};

//	Dissolution Party Cause
enum
{
	eDPC_NORMAL,
	eDPC_INCORRECT_PARTY_INFO,
};

#define	dNAME_LENGTH				18

class	CSimplePartyMemberInfo
{
public:
	char	m_strName[dNAME_LENGTH];

	WORD	m_wCurrentField;	//	«ˆ¿ÅEº“º” « µÅE	
	int		m_iZoneSerial;
	int		m_iDuelVP;

	DWORD	m_bf10InstanceField			:	10;
	DWORD	m_bf10Level					:	10;	//	∑π∫ß
	DWORD	m_bf8RemainHP				:	8;
	DWORD	m_bf4InstanceFieldFloor		:	4;

	DWORD	m_bf3GuildDungeonPlayTime	:	3;	//	31
	DWORD	m_bf1IsDisconnected			:	1;
	DWORD	m_bf8Job					:	8;	//	¡˜æÅE	
	DWORD	m_bf1IsMoveField			:	1;	//	« µÅE¿Ãµø¡ﬂ¿Ã¥Ÿ.
	DWORD	m_bf4SecretDungeonPlayCount	:	4;

};

class	CSimpleUnionPartyInfo
{
public:
	WORD					wPartySerialList[dUNION_PARTY_COUNT];		//	∆ƒ∆ºµÈ¿« Ω√∏ÆæÅE∏ÆΩ∫∆Æ.

};

class	CSimplePartyInfo
{
public:
	WORD					m_wSerial;					//	∆ƒ∆º¿« ∞˙‹Ø Ω√∏ÆæÅE	
	DWORD					m_dwUniqueSerial;
	WORD					m_wGateField;

	DWORD					m_bf4MemberCount		:	4;
	DWORD					m_bf10GateArea			:	10;
	DWORD					m_bf3GoldShareMethod	:	3;
	DWORD					m_bf3ItemShareMethod	:	3;
	DWORD					m_bf3PartyObject		:	3;	//∆ƒ∆º ∏Ò¿ÅE	
	DWORD					m_bf1IsOpenParty		:	1;	//∏‚πˆ∏¶ ∞Ëº” ∏¡˝«—¥Ÿ.

	char					m_strName[dPARTYNAME_LENGTH];

	CSimplePartyMemberInfo	m_aMembers[dPARTY_MEMBER_COUNT];
};

class	cPartyMemberInfoForBCS
{
public:
	DWORD	m_bf10Level			:	10;
	DWORD	m_bf8Job			:	8;
};

class	cPartyInfoForBCS
{
public:
	WORD				m_wSerial;					//	∆ƒ∆º¿« ∞˙‹Ø Ω√∏ÆæÅE
	WORD				m_bf1IsValidParty	:	1;	//	∆ƒ∆º ∏Ò¿ÅE	
	WORD				m_bf3PartyObject	:	3;	//	∆ƒ∆º ∏Ò¿ÅE	
	WORD				m_bf1IsOpenParty	:	1;	//	∏‚πˆ∏¶ ∞Ëº” ∏¡˝«—¥Ÿ.
	WORD				m_bf3MemberCount	:	3;	//	∆ƒ∆º ∏‚πÅEºÅE
	char				m_strMasterName[dNAME_LENGTH];

	cPartyMemberInfoForBCS	m_aMemberList[dPARTY_MEMBER_COUNT];

	int					getAverageLevel();
};

const	int	c_iPartyBufferSizeForPartyList	=	6;

class	cPartyInfoForPL	//	∆ƒ∆º ∏ÆΩ∫∆Æ∏¶ ¿ß«— ∆ƒ∆º ¡§∫∏
{
public:
	WORD	m_wSerial;					//	∆ƒ∆º¿« ∞˙‹Ø Ω√∏ÆæÅE	
	WORD	m_bf3PartyObject				:	3;	//	∆ƒ∆º ∏Ò¿ÅE	
	WORD	m_bf3MemberCount				:	3;	//	∆ƒ∆º ∏‚πÅEºÅE	
	WORD	m_bf10RecruitMemberLevelRange	:	10;

	DWORD	m_dwPreferenceJobMask;		// ºˆ¡§..

	cPartyMemberInfoForBCS	m_aMemberList[dPARTY_MEMBER_COUNT];

	char	m_strMasterName[dNAME_LENGTH];
	char	m_strPartyProspectus[c_iPartyProspectusLength];
	int		getAverageLevel();
};

#endif