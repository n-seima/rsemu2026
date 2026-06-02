#pragma once

#include	"SFC.H"
#include	"definePacketData.H"

#define		dMAX_EVENT_TIME_COUNT	128
#define		dEVENT_TIME_BUFFER_BIT	32
#define		dEVENT_TIME_BUFFER_SIZE	dMAX_EVENT_TIME_COUNT/dEVENT_TIME_BUFFER_BIT // 32비트를 나눈다..
//
//	Event Type
enum
{
	eET_OX_QUIZ,
	eET_WORD_QUIZ,
};

//
//	Red Stone Receiver
enum
{
	eRSRT_HEAVEN,
	eRSRT_HELL,
	eRSRT_RED_DEVIL,
};

enum
{
	eEVENT_TIME_CHRISTMAS,	//	크리스마스
	eEVENT_TIME_VAL,	//	발렌타인데이
	eEVENT_TIME_WHITE_DAY,	//	화이트 데이
	eEVENT_TIME_THE_FIRST_OF_THE_YEAR_LUNAR,	//	설날
	eEVENT_TIME_THE_FIRST_OF_THE_YEAR,	//	신년

	eEVENT_TIME_JANUARY_FULL_MOON,	//	정월대보름
	eEVENT_TIME_DAY_OF_CHILDREN,	//	어린이날
	eEVENT_TIME_RESTORATION_OF_NATURE,	//	자연의 복원(식목일)
	eEVENT_TIME_KOREAN_THANKSGIVING_DAY,	//	추석
	eEVENT_TIME_CHILDRENS_DAY,					//	 어린이날

	eEVENT_TIME_PARENTS_DAY,					// 어버이날
	eEVENT_TIME_TEACHERS_DAY,					// 스승의날
	eEVENT_TIME_A_RAINY_DAY,					// 비오는 날
	eEVENT_TIME_DAY_OF_SEA,						// 바다의날
	eEVENT_TIME_INDEPENDENCE_OF_NACRIEMA,		// 나크리에마의 독립

	eEVENT_TIME_KOREAN_THANKSGIVING_DAY2,	//	추석2
	eEVENT_TIME_HALLOWEEN,					//	할로윈
	eEVENT_TIME_DAY_OF_FIREWORKS,			//	불꽃놀이의 날
	eEVENT_TIME_CHRISTMAS2,					//	크리스마스2
	eEVENT_TIME_NEW_YEAR_EVENT_OF_DECEMBER,	//	12월새해이벤트
		
	eEVENT_TIME_DELEATION_OF_NATURE,//	자연의 사절단 이벤트
	eEVENT_TIME_DELEATION_OF_NATURE_EXP_BONUS,//	자연의 사절단 이벤트(경험치 보너스)
	eEVENT_TIME_7TH_ANNIVERSARY_EVENT,					//	 7주년 이벤트
	eEVENT_TIME_REDISTRIBUTION_EVENT_OF_FIGHTER_AND_SOULBRINGER,	// 투사 영술사 재분배 이벤트
	eEVENT_END,
};



class	cEventTimeDefine
{
public:
	CTimeInfo	m_timeBegin,m_timeEnd;
	int			m_iBeginTimeMinuteValue,m_iEndTimeMinuteValue;
	BOOL		m_bIsActive;
};

extern	char	*g_strEventTimeName[];

class	cEventWordDefine
{		// 마지막 수정일 : 09.10.09
public:
	BOOL			m_bIsActiveWordCompareEvent;
	char			m_strEventWord[256];
	DWORD			m_bf16EventKind		: 16;		// 이벤트 종류
	DWORD			m_bf16BuffSerial	: 16;		// 이벤트 버프 !
	DWORD			m_dwTimeMinite;					//분단위 시간...
	WORD			m_wBeginTimeYear;				//시작 시간...
	WORD			m_wBeginTimeMonth;
	WORD			m_wBeginTimeDay;
	WORD			m_wBeginTimeHour;
	WORD			m_wBeginTimeMinite;
	WORD			m_wEndTimeYear;				//종료 시간...
	WORD			m_wEndTimeMonth;
	WORD			m_wEndTimeDay;
	WORD			m_wEndTimeHour;
	WORD			m_wEndTimeMinite;
	char			m_strBeginComment[256];
	char			m_strEndComment[256];
	char			m_strActiveComment[256];
	WORD			m_wIsSendBeginNotice;
	WORD			m_wIsSendEndNotice;
};

class	cOXQuizDefine
{
public:
	WORD			m_wYear,m_wMonth,m_wDay,m_wHour,m_wMinute,m_wSecond;
	WORD			m_wLimitPlayerCount;
	WORD			m_wIsTest;

	void			reset()
	{
		memset(this,0,sizeof(cOXQuizDefine));
	}
};

class	cWordQuizDefine
{
public:
	WORD			m_wYear,m_wMonth,m_wDay,m_wHour,m_wMinute,m_wSecond;
	WORD			m_wLimitPlayerCount;
	WORD			m_wIsTest;

	void			reset()
	{
		memset(this,0,sizeof(cWordQuizDefine));
	}
};

class	cGoldRushDefine
{
public:
	UTime			m_begin,m_end;
	WORD			m_wBoostExperience,m_wCorrectItemDropChance;
	char			m_strBeginComment[256],m_strEndComment[256];
	
	void			reset()
	{
		memset(this,0,sizeof(cGoldRushDefine));

		m_wBoostExperience		=	100;
		m_wCorrectItemDropChance=	100;
	}
};

#define	dLOTTO_EVENT_NAME_LENGTH	64
#define	dLOTTO_EVENT_COUNT			256

class	cLottoItemDropEvent
{
public:
	int			m_iSerial;
	char		m_strName[dLOTTO_EVENT_NAME_LENGTH];
	int			m_iBeginTimeValue,m_iEndTimeValue;
	BOOL		m_bIsActiveEvent;
	UTime		m_utBegin,m_utEnd;
	int			m_iLottoBoxSerial;
	int			m_iDropChance;	//	10만
	int			m_iMinMobLevel,m_iMaxMobLevel;
	int			m_iLevelRange;
	int			m_iCriticalChance,m_iCriticalCount;

	cLottoItemDropEvent()
	{
		memset(this,0,sizeof(cLottoItemDropEvent));

		m_bIsActiveEvent	=	FALSE;
		m_iDropChance		=	1;	//	0.0001
		m_iMinMobLevel		=	1;
		m_iMaxMobLevel		=	2000;
		m_iLevelRange		=	-10;
		m_iCriticalChance	=	0;
		m_iCriticalCount	=	1;
	}
};