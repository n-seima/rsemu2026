#ifndef _classPACKET_LOGSERVER_H
#define _classPACKET_LOGSERVER_H

#include "definePacketData.h"
#include "cPACKET_BASE.h"
#include "miniPetCommon.h"

#pragma pack(1)

#define dIP_LENGTH				16
#define dPACKAGE_MAXSIZE		5120

typedef struct
{
	BYTE	yy		:	5;
	BYTE	bf1IsNewTime	:	1;

	BYTE	mm		:	4;
	BYTE	bf4Second2	:	4;

	BYTE	dd		:	5;
	BYTE	bf3Second1	:	3;

	BYTE	hour	:	5;

	BYTE	min		:	6;

	inline	int	getSecond()
	{
		if	(bf1IsNewTime	==	0)
			return	0;

		int	iSecond	=	bf3Second1*10+bf4Second2;

		return	iSecond;
	}
}tsLogTime;

class	cMSG_BASE_TYPE_FORLOG
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType;				//	패킷 타입
	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

#define dLOG_PACKAGE					0x10

#define dLOG_BEGIN						0x1c		//	로그 시작
#define dLOG_ERROR						0x1c		//	길드전 관련 로그
#define dLOG_GUILD_WORK					0x1d		//	길드 관련 로그
#define dLOG_SYSTEM						0x1e		//	시스템 로그
#define dLOG_PET_WORK					0x1f		//	펫 관련 로그
#define dLOG_GUILD_BATTLE_WORK			0x20		//	길드전 관련 로그
#define dLOG_ITEM_DROP					0x21		//	87
#define dLOG_ITEM_PICK					0x22		//	87
#define dLOG_ITEM_SELL					0x23		//	87
#define dLOG_ITEM_DEL					0x24		//	87
#define dLOG_ITEM_TRADE					0x25		//	921
#define dLOG_ITEM_BANK_IN				0x26
#define dLOG_ITEM_BANK_OUT				0x27
#define dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	0x28
#define dLOG_ITEM_CREATE_PREMIUM_ITEM	0x29
#define dLOG_CREATE_ITEM				0x2a
#define dLOG_USE_ITEM					0x2b
#define dLOG_EXPIRED_ITEM				0x2c
#define dLOG_DOUBLE_ITEM				0x2d
#define dLOG_ENCHANT_ITEM				0x2e
#define dLOG_EVENT						0x2f
#define dLOG_PITCHMAN_SHOP_TRADE		0x30

#define dLOG_AVA_CREATE					0x31		//	63
#define dLOG_AVA_DELETE					0x32		//	63
#define dLOG_AVA_LOGIN					0x33		//	63
#define dLOG_AVA_LOGOUT					0x34		//	63
#define dLOG_AVA_SAVE_LOG				0x35		//	63

#define dLOG_AVA_END_LOG				0x36		//	63
////////////////////////////////////////////////////////////
//	FROM TOOLLOG SERVER

#define dLOG_FROMTOOLLOG				0x100

struct	LOG_FROMTOOLLOG
{
	cMSG_BASE_TYPE_FORLOG	base;

	WORD	wServerIndex;
	char	binData[dPACKAGE_MAXSIZE];	//	이곳에 기록할 데이터를 담아서 보내세요. 하나씩 ~ 아래 패킷과 모양이 동일해야 합니다.
};

//	FROM TOOLLOG SERVER
////////////////////////////////////////////////////////////

struct	LOG_PACKAGE
{
	cMSG_BASE_TYPE_FORLOG	base;

	char	package[dPACKAGE_MAXSIZE];
};

enum
{
	ePW_ADD,	//	추가
	ePW_REMOVE,	//	제거
	ePW_SEAL,	//	봉인
	ePW_UNSEAL,	//	봉인 해제
	ePW_LEVEL_UP,	//	봉인 해제
};

struct	LOG_PET_WORK
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		bf2Index	:	2;
	WORD		bf6Work		:	6;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	cPetDefine	petInfo;
};

enum
{
	eERROR_LOG_QUEST_EXP_SERIAL_RECEIVE	=	1,	//	LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE	퀘스트 경험치 보상 연속으로 획득
};

struct	LOG_ERROR_BASE
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
};

struct	LOG_ERROR	: public LOG_ERROR_BASE//	
{
	char	aBuffer[256];
};

struct	LOG_ERROR_QUEST_EXP_SERIAL_RECEIVE	: public LOG_ERROR_BASE//	
{
//	=====================
	int			iQuestExp,iQuestIndex;
};


//
//	Penelty Player Play Time Reason	
enum
{
	ePPPTR_SPEED_HACK_USER,
	ePPPTR_MOVE_PACKET_HACKING,
	ePPPTR_CG_INCREASE_STATE,
	ePPPTR_CG_INCREASE_ABILITY_LEVEL,
	ePPPTR_PACKET_HACKING,

	ePPPTR_NPGM_SPEED_HACK,
	ePPPTR_NPGM_FIND_GAME_HACK_AND_KILL,
	ePPPTR_NPGM_FIND_GAME_HACK,
	ePPPTR_NPGM_FIND_GAME_HACK_DOUBT,

	ePPPTR_BY_XTRAP,
};


struct	LOG_S_HACKING_USER
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	WORD		wPeneltyTime;
	WORD		wReason;

	char		strHackingProgramName[32];
};

struct	LOG_S_COMPLETE_QUEST
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	WORD		bf2Type			:	2;
	WORD		bf14QuestIndex	:	14;
};

struct	LOG_S_RECEIVE_QUEST_REWARD_EXP
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	int			iRewardExp;
	int			iRealReceiveExp;
	int			iQuest;
};

enum
{
	eTME_LOG_BOOST_EXP_IN_SERVER,
	eTME_LOG_BOOST_GOLD_IN_SERVER,
	eTME_LOG_BOOST_PLAYER_EXP,
	eTME_LOG_BOOST_PLAYER_GOLD,
};

struct	LOG_S_TRESURE_MAP_EVENT
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	WORD		wEvent;
};

struct	LOG_S_BACK_SERVER
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	DWORD		dwCurrentSaveCount;
	DWORD		dwOldSaveCount;
};

struct	LOG_S_CREATE_MINI_PET
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	CItemDefine		cItem;
	WORD			wSlot;
	cMiniPetCommon	miniPet;
};

struct	LOG_S_EXTEND_MINI_PET_TIME
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	WORD			wSlot;
	cMiniPetCommon	miniPet;
};

struct	LOG_S_SEAL_MINI_PET
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	WORD			wSlot;
	cMiniPetCommon	miniPet;
};

struct	LOG_S_UNSEAL_MINI_PET
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	WORD			wSlot;
	cMiniPetCommon	miniPet;
};

struct	LOG_S_MINI_PET_FEED_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	WORD			wSlot;
	cMiniPetCommon	miniPet;
	CItemDefine		item;
};

struct	LOG_S_RELEASE_MINI_PET
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	cMiniPetCommon	miniPet;
};

struct	LOG_S_RESTRAINT_MINI_PET
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	cMiniPetCommon	miniPet;

	WORD		m_wFireCount,m_wWaterCount,m_wWindCount,m_wEarthCount,m_wLightCount,m_wDarkCount;
};

struct	LOG_S_OPERATOR_CHAT_LOG
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;

	char		strTarget[dNAME_LENGTH];

	WORD		bf10ChatMethod	:	10;
	WORD		bf1IsListen		:	1;	//	듣는거다.
	
	char		strText[128];
};

struct	LOG_S_REMOVE_A_PLACE_RELATED_OF_GUILD
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
////////////////////////////////////////////////////////////
	char		strGuildName[dGUILD_NAME_LENGTH];
	WORD		wField;
};

struct	LOG_S_BUY_ITEM
{		// 마지막 수정일 : 09.10.27
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
////////////////////////////////////////////////////////////
	
	CItemDefine	cItem;
	DWORD		dwBuyPrice;
	WORD		wCount;
	WORD		wBuyMethod;
	WORD		wTokenItem;		// 토큰아이템.
};

struct	LOG_S_RECORD_GUILD_HONOR_POINT
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
////////////////////////////////////////////////////////////

	char		strGuildName[dGUILD_NAME_LENGTH];
	int			iHonorPoint;
};

struct	LOG_S_BEGIN_DUEL
{
	cMSG_BASE_TYPE_FORLOG	base;
	
	tsLogTime	logTime;
	
	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
	////////////////////////////////////////////////////////////
	int			iDuelSerial;
	char		astrTeamName[2][dNAME_LENGTH];
	WORD		wTeam;
};

struct	LOG_S_RESULT_OF_DUEL
{
	cMSG_BASE_TYPE_FORLOG	base;
	
	tsLogTime	logTime;
	
	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
	////////////////////////////////////////////////////////////
	int			iDuelSerial;
	int			iVP,iDP,iGetVP,iGetDP;	//	16
	int			iBattle,iWin,iDefeat,iDraw;	//	16
	WORD		wWinTeam,wTeam;
	char		astrTeamName[2][dNAME_LENGTH];	//	36
};

struct	LOG_S_DISCONNECT_IN_DUEL_FIELD
{
	cMSG_BASE_TYPE_FORLOG	base;
	
	tsLogTime	logTime;
	
	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
	////////////////////////////////////////////////////////////
	int			iDuelSerial;
	char		astrTeamName[2][dNAME_LENGTH];	//	36
};

enum
{
	eBUY_ITEM_IN_SHOP,
	eBUY_ITEM_IN_PIGEON_SHOP,
	eBUY_ITEM_IN_GUILD_SHOP,
	eBUY_ITEM_IN_BANK_GOLDBAR,
	eBUY_ITEM_IN_BADGE_OF_TRADER_SHOP,		// 거래상인의 배지 상점

};

struct	LOG_S_REBIRTH
{
	cMSG_BASE_TYPE_FORLOG	base;
	
	tsLogTime	logTime;
	
	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
	////////////////////////////////////////////////////////////
	WORD		wRebirthCount;
	WORD		wPreLevel;
	int			iPreExp;
	WORD		wLevel;
	int			iExp;
};


//
//	System Log Type
enum
{
	eSLT_HACKING,
	eSLT_RECEIVE_QUEST_EXP,
	eSLT_COMPLETE_QUEST,
	eSLT_TREASURE_MAP_EVENT,
	eSLT_BACK_SERVER,
	eSLT_OPERATOR_CHAT_LOG,
	eSLT_REMOVE_A_PLACE_RELATED_OF_GUILD,

	eSLT_CREATE_MINI_PET	,		//	미니펫 생성
	eSLT_REMOVE_MINI_PET	,		//	미니펫 제거
	eSLT_EXTEND_MINI_PET_TIME	,		//	미니펫 제거
	eSLT_SEAL_MINI_PET	,		//	미니펫 제거
	eSLT_UNSEAL_MINI_PET	,		//	미니펫 제거
	eSLT_MINI_PET_FEED_ITEM,		//	미니펫 제거
	eSLT_BUY_ITEM,
	eSLT_RECORD_GUILD_HONOR_POINT,

	eSLT_BEGIN_DUEL,
	eSLT_RESULT_OF_DUEL,
	eSLT_DISCONNECT_IN_DUEL_FIELD,
	eSLT_REBIRTH,
	eSLT_MIX_MINIPET,
	eSLT_RESTRAINT_MINIPET,
	eSLT_DIVIDE_ITEM,		// 스택 나누기..
	eSLT_TRANS_AVATAR_TO_GVG_SERVER,		// GVG서버로 아바타 전송
	eSLT_USE_LEADERS_BELL	,		// Leader's Bell Log
	eSLT_WARNING_QUEST_EXP_SERIAL_RECEIVE,
};

struct	LOG_S_LEDERS_BELL
{
	cMSG_BASE_TYPE_FORLOG	base;
	
	tsLogTime	logTime;
	
	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
	////////////////////////////////////////////////////////////

	char		strMessage[128];
};

struct	LOG_S_DIVIDE_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;
	
	tsLogTime	logTime;
	
	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
	////////////////////////////////////////////////////////////
	CItemDefine				cTargetItem;				// 나눈아이템
	WORD					wBeforDragCount;			// 나눌아이템 원래 개수.
	WORD					wAfterDragCount;			// 나누고 남은 아이템 개수
	WORD					wFocusCount;				// 나눈 아이템 개수.
};

struct	LOG_S_TRNAS_AVATAR
{
	cMSG_BASE_TYPE_FORLOG	base;
	
	tsLogTime	logTime;
	
	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wWork;
	////////////////////////////////////////////////////////////
	char		strNewAvatarName[dNAME_LENGTH];
};

struct	LOG_SYSTEM
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	WORD		wFieldUniqueIndex;
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];

	WORD		wWork;

	char		aBuffer[256];
};

enum
{
	eGBW_BOOKING,	//	예약
	eGBW_CANCEL,	//	취소
	eGBW_ENGAGE,	//	결정됨
	eGBW_RESULT,	//	길드전 결과
	
	eGBW_JOIN,		//	길드 가입
	eGBW_LEAVE,		//	길드 탈퇴

	eGBW_KILL,		//	길드전중 상대 길드원을 죽였다.
	eGBW_BEGIN,		//	길드전 시작
	eGBW_PRE_RESULT,
	eGBW_ERROR_IN_KILL,
	eGBW_ERROR_IN_CHECK_RESULT,
	eGBW_EXTERMINATE,	//	전멸 시켰다.
	eGBW_NEW_KILL,		//	길드전중 상대 길드원을 죽였다.

	eGW_GUILD_DUNGEON_PLAY,		//	길드전중 상대 길드원을 죽였다.
	eGW_ACTIVE_GUILD_MASTER_GRACE,		//	길드전중 상대 길드원을 죽였다.
	eGW_ACTIVE_GUILD_ROEN_GOOD_WILL,		//	길드 로엔의 호감도 정보

	eGW_TRANS_GUILD_TO_GVG_SERVER,		// 길드 GVG 서버에 전송
	eGW_DELETE_GUILD,					//	길드 삭제
	eGW_CHANGE_GUILD_RANK,					//	길드 랭크변경

};

struct	LOG_GUILD_BATTLE_WORK	//	길드전 관련 로그
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	char		strGuild1Name[dGUILD_NAME_LENGTH];
	char		strGuild2Name[dGUILD_NAME_LENGTH];
	WORD		wWork;

	BYTE		bTimeOrder,bFieldOrder;
	BYTE		bResult;
	BYTE		bGuild1VictoryPoint,bGuild2VictoryPoint;	//	이긴 길드 승점

	char		strKiller[dNAME_LENGTH];
	char		strTarget[dNAME_LENGTH];
	BYTE		bDummy;
};

class	cBeginGuildBattleInfo
{
public:
	WORD		m_wMemberCount;
	WORD		m_wVictory,m_wDefeat,m_wCVCount,m_wFightAtWeek;
	int			m_iVictoryPoint;
	
};

struct	LOG_GUILD_BATTLE_BEGIN_WORK	//	길드전 관련 로그
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	char		strGuild1Name[dGUILD_NAME_LENGTH];
	char		strGuild2Name[dGUILD_NAME_LENGTH];
	WORD		wWork;

	cBeginGuildBattleInfo	m_aInfo[2];
};

struct	LOG_GUILD_BATTLE_WORK_KILL	//	길드전 관련 로그
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	char		strGuildName[2][dGUILD_NAME_LENGTH];
	WORD		wWork;

	char		strKiller[dNAME_LENGTH];
	char		strTarget[dNAME_LENGTH];

	WORD		wAddKP,wKP1,wKP2,wKillerTeam;	//	이긴 길드 승점
};

struct	LOG_GUILD_BATTLE_WORK_KILL2	//	길드전 관련 로그
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	char		strGuildName[2][dGUILD_NAME_LENGTH];
	WORD		wWork;

	char		strKiller[dNAME_LENGTH];
	char		strTarget[dNAME_LENGTH];

	DWORD		bf10AddKP		:	10;
	DWORD		bf10KP1			:	10;
	DWORD		bf10KP2			:	10;	//	이긴 길드 승점
	DWORD		bf2KillerTeam	:	2;
};

struct	LOG_GUILD_BATTLE_WORK_KILL3	//	길드전 관련 로그
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	char		strGuildName[2][dGUILD_NAME_LENGTH];
	WORD		wWork;

	char		strKiller[dNAME_LENGTH];
	char		strTarget[dNAME_LENGTH];

	WORD		wKP1,wKP2;
	WORD		bf10AddKP		:	10;
	WORD		bf2KillerTeam	:	2;
};

struct	LOG_GUILD_BATTLE_WORK_RESULT	//	길드전 관련 로그
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	char		strGuild1Name[dGUILD_NAME_LENGTH];
	char		strGuild2Name[dGUILD_NAME_LENGTH];
	WORD		wWork;

	char		strGuild1MemberName[dNAME_LENGTH];
	char		strGuild2MemberName[dNAME_LENGTH];

	BYTE		bGuild1MemberCount,bGuild2MemberCount;
	WORD		wKP1,wKP2;
};

struct	LOG_GUILD_BATTLE_EXTERMINATE	
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	char		strGuild1Name[dGUILD_NAME_LENGTH];
	char		strGuild2Name[dGUILD_NAME_LENGTH];
	WORD		wWork;

	WORD		wWinner;
	WORD		wKP1,wKP2;
};

struct	LOG_GUILD_WORK
{
	cMSG_BASE_TYPE_FORLOG	base;
	
	tsLogTime	logTime;
	
	char		strId[dID_LENGTH],strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];

	WORD		wFieldUniqueIndex;
	WORD		wWork;

	char		strGuildName[dGUILD_NAME_LENGTH];
};

struct	LOG_DELETE_GUILD : public LOG_GUILD_WORK
{
	WORD		wReason;
};


struct	LOG_CHANGE_GUILD_RANK : public LOG_GUILD_WORK
{
	char		strTargetName[dNAME_LENGTH];

	WORD		wTargetRank;
	WORD		wMasterRank;
	WORD		wReason;
};

struct	LOG_ABOUT_GUILD_WORK : LOG_GUILD_WORK
{
	char		aBuffer[256];
};

struct	LOG_S_RECORD_GUILD_DUNGEON_PLAYTIME : public LOG_GUILD_WORK
{
	WORD		wPlayTime;
	WORD		wPartyMemberCount;
	WORD		wFloor;
};

struct	LOG_S_TRANS_GUILD	: public LOG_GUILD_WORK
{
	char		strTransGuildName[dGUILD_NAME_LENGTH];
};

struct	LOG_S_RECORD_GUILD_MASTER_GRACE : public LOG_GUILD_WORK
{
	WORD		wPlayTime;
};

struct	LOG_S_RECORD_GUILD_ROEN_GOOD_WILL : public LOG_GUILD_WORK
{
	WORD		wLast;
	WORD		wNew;
};


//┏━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	ITEM LOG
struct	LOG_ITEM_DROP
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;

	char		strId[dID_LENGTH];
	char		strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wFieldUniqueIndex;
	
	CItemDefine	cItem;
	WORD		wMethod;	//	0은 그냥 ,1은 퀘스트
};

enum
{
	ePIM_NORMAL,
	ePIM_QUEST,
	ePIM_BEAST_PICK,
	ePIM_BEAST_RETURN,
	ePIM_ARCA,
	ePIM_STEAL,
	ePIM_TREASURE_MAP_EVENT,
	ePIM_MINIPET_PICK,
	ePIM_LOTTO_EVENT,
};

struct	LOG_ITEM_PICK
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;
	
	CItemDefine				cItem;

	WORD					bf6Method	:	6;	//	0은 그냥 줏은거,1은 퀘스트를 통해 얻은거
	WORD					bf1IsIncludeGuildInfo	:	1;
	WORD					bf1IsOperator			:	1;
	WORD					:	0;

	char					strGuildName[dGUILD_NAME_LENGTH];
};

struct	LOG_ITEM_SELL
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;
	
	CItemDefine				cItem;
	DWORD					dwSellPrice;
};

struct	LOG_USE_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;
	
	CItemDefine				cItem;
};

enum
{
	eCIM_LOTTERY,
	eCIM_ITEM_PACK,
	eCIM_BREEDING_RECORD_BOOK,
	eCIM_OLD,
	eCIM_USE_BOTTOMLESS_BOX,
	eCIM_WITHDRAW_FROM_GUILD_INVENTORY,
	eCIM_SELECT_ITEM_IN_PACK,
	eCIM_BLOCK_LIGHT_POCKET,
	eCIM_CH5_ITEM_COPY,
	eCIM_WITHDRAW_FROM_GUILD_INVENTORY2,
	eCIM_CUT_PREFIX,		// 접두사 잘라내기 
};

struct	LOG_CREATE_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;
	
	CItemDefine				cItem;

	WORD					wMethod;

	char					strGuildName[dGUILD_NAME_LENGTH];
};

struct	LOG_EXPIRED_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;
	
	CItemDefine				cItem;
};

//
//	Double Item Log
enum
{
	eDIL_TRADE,
	eDIL_PICK,
	eDIL_LOGIN,
	eDIL_BANK,

};

struct	LOG_DOUBLE_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;

	CItemDefine				cItem;

	WORD					wWhere;
};

//
//	Enchant Item Work
enum
{
	eEIW_SUCCESS,					//	성공
	eEIW_BROKEN,					//	부숴짐
	eEIW_DESTROY,					//	파괴됨
	eEIW_REPAIR_BROKEN,				//	부서진 상태 수리
	eEIW_REPAIR_CURSE,				//	저주 수리
	eEIW_REPAIR_CAN_NOT_ATTACH_PREFIX_STATUS,	//	접두사를 붙일 수 없는 상태 수리
	eEIW_REPAIR_AND_REMOVE_PREFIX,	//	수리는 했지만 접두사가 사라짐

	eEIW_RENEW_TERM,				//	기간 연장했다.
	eEIW_REPAIR_DX_ITEM,			//	DX 아이템 수리
	eEIW_REMOVE_PREFIX,				//	접두사 제거
	eEIW_MERGE_ITEM,				//	아이템 합체
	eEIW_JUDGE_ITEM,				//	아이템 판별
	eEIW_RELEASE_REVERSION,			//	귀속 풀기
	eEIW_BROKEN_BY_CH5_REWARD,		//	챕터 5 보상으로 뽀개짐
	eEIW_SEAL_SPHERE_OF_DAWN	,	//	새벽의 봉인구로 인해 아이템 변함
	eEIW_UPGRADE_ETERNAL_ITEM,			// 이터널 아이템 강화.		09.09.04
	eEIW_SUCCESS_COPY_PREFIX,				// 접두사 복사 성공
	eEIW_FAILED_COPY_PREFIX,				// 접두사 복사 실패
	eEIW_SUCCESS_ATTACH_PREFIX,				// 접두사 붙이기 성공
	eEIW_FAILED_ATTACH_PREFIX,				// 접두사 붙이기 실패 
	eEIW_SUCCESS_INCREASE_CARVING_LEVEL,	// 각인 레벨증가 성공
	eEIW_FAILED_INCREASE_CARVING_LEVEL,	// 각인 레벨증가 실패
	eEIW_SUCCESS_SET_CARVING_LEVEL,	// 각인 레벨설정 성공
	eEIW_FAILED_SET_CARVING_LEVEL,	// 각인 레벨설정 실패

};

struct	LOG_ENCHANT_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;

	CItemDefine				cItem;	//	작업후 결과(단, eEIW_DESTROY 의 경우는 원본 아이템 정보이다.)

	WORD					wWork;
	WORD					wRenewTerm;
	DWORD					dwPrice;
};
struct	LOG_UPGRADE_ETERNAL_ITEM		// 09.09.01
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;

	CItemDefine				cTargetItem;				// 강화할 아이템
	
	WORD					wWork;
	WORD					wUpgradeItemSerial;				//  강화 후  아이템
	DWORD					dwGold;							// 재료 금..
	WORD					wStoneOfMesteryCount : 5;			// 신비석
	WORD					wTantillessCount	 : 5;				// 탄틸리스
	WORD					wCrystalCount		 : 5;					// 결정석
	WORD					:0;
	WORD					wUpgradeMaterialsItemSerial;			// 일반강화제 시리얼
	WORD					wUpgradePremiumMaterialsItemSerial;		// 프리미엄아이템 강화제 시리얼

};
struct	LOG_CARVE_ITEM		//
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;

	CItemDefine				cTargetItem;				// 강화할 아이템
	
	WORD					wWork;
	WORD					wUseItem;								//	 사용한 아이템
	WORD					bf8BeforCarvingLevel : 8;				//  강화 전  레벨
	WORD					bf8AfterCarvingLevel : 8;				//  강화 후  레벨
	DWORD					dwGold;							// 재료 금..
	WORD					bf5CrystalCount		 : 5;		// 결정석
	WORD					:0;

};

struct	LOG_PITCHMAN_SHOP_TRADE			// 09.08.21
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;

	CItemDefine				cItem;

	char					strOwnerId[dID_LENGTH];
	char					strOwnerName[dNAME_LENGTH];
	char					strOwnerIP[dIP_LENGTH];
	DWORD					bf31ItemPrice	:	31;
	DWORD					bf1IsOperator	:	1;
	BYTE					bDenominationType;
};


//
//	Event Work
enum
{
	eEW_OXQUIZ_WINNER		,	//	OX 퀴즈 승자 리스트
	eEW_OXQUIZ_COMER_LIST	,	//	OX 퀴즈 참가자 리스트
	eEW_OXQUIZ_LOSER_LIST	,	//	OX 퀴즈 탈락자 리스트
	eEW_OXQUIZ_QUESTION		,	//	OX 퀴즈 문제

	eEW_WORDQUIZ_WINNER		,	//	낱말 퀴즈 승자 리스트
	eEW_WORDQUIZ_COMER_LIST	,	//	낱말 퀴즈 참가자 리스트
	eEW_WORDQUIZ_LOSER_LIST	,	//	낱말 퀴즈 탈락자 리스트
	eEW_WORDQUIZ_QUESTION	,	//	낱말 퀴즈 문제
};

//
//	로그 패킷을 위한 OX 퀴즈 질문 
class	COXQuizQuesionForLogPacket
{
public:
	WORD		m_wSolution;
	WORD		m_wQuestionCounter;

	char		m_strQuestion[256];
};

class	cWordQuizQuesionForLogPacket
{
public:
	WORD		m_wQuestionCounter;

	char		m_strSolution[32];
	char		m_strQuestion[256];
};


struct	LOG_EVENT
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	WORD					wCount;
	WORD					wType;
	WORD					wValue;
	char					astrName[100][dNAME_LENGTH];
};

struct	OLD_LOG_EVENT
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	WORD					wCount;
	char					astrName[100][dNAME_LENGTH];
};

//
//	Remove Item Work
enum
{
	eRI_DESTROY,	//	부순거다.
	eRI_QUEST,		//	퀘스트에 의한거
	eRI_OX_QUIZ,	//	Ox 퀴즈에서
	eRI_RECOVER_DEATH_PENELTY,		//	퀘스트에 의한거
	eRI_UNSEAL_BREEDING_RECORD_BOOK,		//	퀘스트에 의한거
	eRI_GUILD_BATTLE_ITEM_BY_NOT_GUILD_BATTLE_FIELD	,	//	길드전 필드가 아니라서 길드전 아이템 제거 했다.
	eRI_REPAIR_ITEM_CAHRGE,	//	아이템 수리하면서 돈이 나감
	eRI_REVISE_INCORRECT_PREFIX,	//	잘못된 접두사 수리
	eRI_LOTTERY,	//	복권 꽝~
	eRI_BY_MOVE_FIELD,	//	복권 꽝~
	eRI_BY_USE_BOTTOMLESS_BOX,	//	바닥 없는 상자 사용
	eRI_BY_KARMA,	//	바닥 없는 상자 사용
	eRI_BY_STORE_GUILD,	//	길드 인벤에 보관
	eRI_BY_FEED_TO_GUILD_PET,	//	길드 인벤에 보관
	eRI_BY_ADD_RELATED_PLACE,	//	길드 연고지 추가
	eRI_BY_SEAL_MINIPET,	//	길드 연고지 추가
	eRI_BY_UNSEAL_MINIPET,	//	길드 연고지 추가
	eRI_BY_USE_POLISHER,	//	길드 연고지 추가
	eRI_BY_FEED_TO_MINIPET,	//	길드 인벤에 보관
	eRI_BY_USE_AWAKEN_ITEM,	//	각성 아이템 사용.
	eRI_BY_DISJOINTING_ITEM,	//	각성 아이템 사용.
	eRI_BY_TOKEN,	//	토큰으로 사용
	eRI_BY_ETERNAL,	//	이터널때메
	eRI_BY_PITCHMANSHOP,	//	노점상에서
};

struct	LOG_ITEM_DEL
{
	cMSG_BASE_TYPE_FORLOG	base;

	tsLogTime	logTime;
	char		strId[dID_LENGTH];
	char		strName[dNAME_LENGTH];
	char		strIP[dIP_LENGTH];
	WORD		wFieldUniqueIndex;
	CItemDefine	cItem;

	WORD		bf6Method				:	6;	//	0은 그냥 ,1은 퀘스트
	WORD		bf1IsIncludeGuildInfo	:	1;	//	로그에 길드 정보가 포함되어 있다.
	WORD		bf8Spare				:	8;	//	로그에 길드 정보가 포함되어 있다.
	WORD		bf1IsOperator			:	1;
	int			iCharge;	//	아이템 수리 비용

//	090716 추가
	char		strGuildName[dGUILD_NAME_LENGTH];
};

struct	LOG_ITEM_TRADE
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char	strSenderId[dID_LENGTH];
	char	strSenderName[dNAME_LENGTH];
	char	strSenderIP[dIP_LENGTH];
	WORD	wFieldUniqueIndexSender;
	DWORD	dwGoldSender;

	char	strRecverId[dID_LENGTH];
	char	strRecverName[dNAME_LENGTH];
	char	strRecverIP[dIP_LENGTH];
	WORD	wFieldUniqueIndexRecver;
	DWORD	dwGoldRecver;
	WORD	wTrader1ItemCount;
	WORD	bf15Trader2ItemCount	:	15;
	WORD	bf1IsOperator			:	1;

	CItemDefine	cItem[dTRADE_ITEM_COUNT+dTRADE_ITEM_COUNT];
};

struct	LOG_ITEM_BANK_IN
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strIP[dIP_LENGTH];
	WORD	wFieldUniqueIndex;
	int		iGold;
	WORD	bf15Count				:	15;
	WORD	bf1IsOperator			:	1;

	CItemDefine	cItem[dMAX_STORE_ITEM_COUNT_BY_LOG];
};

struct	LOG_ITEM_BANK_OUT
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strIP[dIP_LENGTH];
	WORD	wFieldUniqueIndex;
	int		iGold;

	WORD	bf15Count				:	15;
	WORD	bf1IsOperator			:	1;
	
	CItemDefine	cItem[dMAX_STORE_ITEM_COUNT_BY_LOG];
};

enum
{
	eWPIM_WITHDRAW_CART_ITEM,
	eWPIM_WITHDRAW_CARROT,
};

struct	LOG_ITEM_WITHDRAW_PREMIUM_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strIP[dIP_LENGTH];
	WORD	wFieldUniqueIndex;

	DWORD	dwItemNo;
	DWORD	dwSqnNumber;

	WORD	wMethod;
};

enum
{
	eCPIM_WITHDRAW_CART_ITEM,
	eCPIM_BUY_BY_CARROT,
};

struct	LOG_ITEM_CREATE_PREMIUM_ITEM
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char					strId[dID_LENGTH];
	char					strName[dNAME_LENGTH];
	char					strIP[dIP_LENGTH];
	WORD					wFieldUniqueIndex;
	
	CItemDefine				cItem;
	
	WORD					wMethod;
	WORD					wCarrotPrice;
};

//┃	ITEM LOG
//┗━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	AVATAR LOG
struct	LOG_AVA_CREATE
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strIP[dIP_LENGTH];
};

struct	LOG_AVA_DELETE
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strIP[dIP_LENGTH];
};

struct	LOG_AVA_LOGIN
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strIP[dIP_LENGTH];
};

struct	LOG_AVA_LOGOUT
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
	char	strIP[dIP_LENGTH];
};

struct	LOG_AVA_SAVE_LOG
{
	cMSG_BASE_TYPE_FORLOG	base;
	tsLogTime				logTime;

	WORD	wLevel;
	DWORD	dwExperience;
	DWORD	dwGold;
	DWORD	dwSaveCount;
	WORD	wField;

	char	strIdAndName[dID_LENGTH+dNAME_LENGTH];
};

//┃	AVATAR LOG
//┗━━━━━━━━━━━━━━━━━━━━━━━━━

union	ALL_LOGMSG
{
	cMSG_BASE_TYPE_FORLOG			base;

	LOG_ERROR						mLOG_ERROR;
	LOG_GUILD_WORK					mLOG_GUILD_WORK;
	LOG_PET_WORK					mLOG_PET_WORK;
	LOG_GUILD_BATTLE_WORK			mLOG_GUILD_BATTLE_WORK;
	LOG_ITEM_DROP					mLOG_ITEM_DROP;
	LOG_ITEM_PICK					mLOG_ITEM_PICK;
	LOG_ITEM_SELL					mLOG_ITEM_SELL;
	LOG_ITEM_DEL					mLOG_ITEM_DEL;
	LOG_CREATE_ITEM					mLOG_CREATE_ITEM;
	LOG_USE_ITEM					mLOG_USE_ITEM;
	LOG_ITEM_TRADE					mLOG_ITEM_TRADE;
	LOG_PITCHMAN_SHOP_TRADE			mLOG_PITCHMAN_SHOP_TRADE;
	LOG_ITEM_BANK_IN				mLOG_ITEM_BANK_IN;
	LOG_ITEM_BANK_OUT				mLOG_ITEM_BANK_OUT;
	LOG_ITEM_WITHDRAW_PREMIUM_ITEM	mLOG_ITEM_WITHDRAW_PREMIUM_ITEM;
	LOG_ITEM_CREATE_PREMIUM_ITEM	mLOG_ITEM_CREATE_PREMIUM_ITEM;
	LOG_EXPIRED_ITEM				mLOG_EXPIRED_ITEM;
	LOG_DOUBLE_ITEM					mLOG_DOUBLE_ITEM;
	LOG_ENCHANT_ITEM				mLOG_ENCHANT_ITEM;

	LOG_AVA_CREATE					mLOG_AVA_CREATE;
	LOG_AVA_DELETE					mLOG_AVA_DELETE;
	LOG_AVA_LOGIN					mLOG_AVA_LOGIN;
	LOG_AVA_LOGOUT					mLOG_AVA_LOGOUT;
	LOG_SYSTEM						mLOG_SYSTEM;
	LOG_EVENT						mLOG_EVENT;
	LOG_ABOUT_GUILD_WORK			mLOG_ABOUT_GUILD_WORK;

	LOG_AVA_SAVE_LOG				mLOG_AVA_SAVE_LOG;
};

#define	dMAX_MSG_COUNT		10000

class cLOG_QUE
{
	ALL_LOGMSG	m_aPacket[dMAX_MSG_COUNT];		//	패킷들..
	int			m_iFirstQue,m_iLastQue,m_iCurrentQue,m_iCount,m_iCounter;		//	첫번째,마지막 패킷, 쌓여 있는패킷 수
	CRITICAL_SECTION	cs;

public:
	DWORD		m_dwTotalPacketSize;
					cLOG_QUE()	{	InitializeCriticalSection(&cs);reset();	}
					virtual ~cLOG_QUE()	{	DeleteCriticalSection(&cs);	}
	void			reset();									//	초기화
	inline	int		getCount(){return	m_iCount;}
	BOOL			addPacket(ALL_LOGMSG *_lpPacket);				//	패킷 추가
	BOOL			isReceivedPacket(int _iPacket);				//	받은 패킷이냐?
	ALL_LOGMSG	*	popPacket();								//	큐에서 패킷을 빼낸다.

	ALL_LOGMSG	*	getOne();									//	큐에서 패킷 하나를 얻어낸다.
	void			setFront()
	{	
		m_iCounter		=	m_iCount;
		m_iCurrentQue	=	m_iFirstQue;
	}
};	//	class cPACKET_QUE

extern cLOG_QUE		g_LogQue;

#pragma pack()

#endif	//	_classPACKET_LOGSERVER_H

