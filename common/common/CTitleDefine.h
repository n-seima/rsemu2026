#ifndef _classTITLE_DEFINE_H
#define	_classTITLE_DEFINE_H

#include <winsock2.h>
#include <windows.h>

#define	dTITLE_NAME_LENGTH						32
#define	dMAX_TITLE_EFFECT_COUNT					6
#define	dMAX_TITLE_EFFECT_INFO_VALUE_COUNT		10
#define	dMAX_TITLE_LEVEL						10
#define	dMAX_TITLE_COUNT						256
#define	dMAX_PLAYER_JOB							16
#define	dTITLE_EXPLAIN_LENGTH					254
#define	dTITLE_NOT_BANK_GOLD_BAR_TAX			55		// 09.08.28   금괴 수수료.(칭호없을때~)
//
//	몇가지 타이틀 자체를 찍어야 할때 쓴다.
//	Title Name
enum
{
	eTN_BANK_CLIENT		=	1,
	eTN_PORTER			=	2,
	eTN_CITIZEN			=	3,
	eTN_DEALER			=	4,			//	거래인
	eTN_BARD			=	5,			//	음유 시인
	eTN_BERSERK			=	22,
	eTN_CARTOGRAPHER	=	25,
	eTN_CAMPING_MASTER	=	28,	//	캠핑 마스터
	eTN_DRUGGIST		=	31,	//	약사
	eTN_RED_STONE_HUNTER=	82,	//	약사
	eTN_CRUSADER		=	101,
	eTN_DUNGEON_MASTER	=	102,
	eTN_DEVIL_SLAYER	=	103,
	eTN_POACHER			=	104,
	eTN_DRAGON_SLAYER	=	105,
	eTN_FRONTIER		=	183,
	eTN_RED_STONE_AGENT	=	184,

	eTN_BONUS_STATE_POINT=	217,
	eTN_BONUS_SKILL_POINT=	218,
	eTN_BONUS_HP		=	219,
	eTN_BONUS_CP		=	220,

};
// Title Name
enum TITLE_LIST
{
	eTIN_BANK_CUSTOMER			 = 1,	// 은행고객
	eTIN_PORTER						,	// 짐꾼
	eTIN_CITIZEN					,	// 시민
	eTIN_BROKER						,	// 거래인

	eTIN_NOVICE					 = 6,	// 초보자

	eTIN_ALCHEMY				= 23,	// 연금술
	eTIN_MAPMAKER				= 25,	// 지도 제작자
	eTIN_INSTINCTFORSURVIVAL		,	// 생존 본능
	eTIN_CITYWALKER					,	// 시티 워커
	eTIN_CAMPING_MASTER				,	// 캠핑마스터
	eTIN_MONSTERSEXER				,	// 몬스터 감별사
	eTIN_CHARMING					,	// 애교
	eTIN_PHAMRMACIST				,	// 약사
	eTIN_ORPHIC						,	// 밀교

	eTIN_CAVETHEFT				= 71,	// 우수 동굴 도둑
	eTIN_CHROMATIC_HONORMEMBER		,	// 크로매틱 명예 대원
	eTIN_HONORMEMBER				,	// 명예의원

	eTIN_JEWELY_MASTER			= 81,	// 보석감정사
	eTIN_REDSTONE_HUNTER			,	// 붉은 보석 사냥꾼
	eTIN_REDEYE_MEMBER				,	// 레드아이 단원

	eTIN_PIONEER				= 183,	// 선구자

	eTIN_CHIVALRY				= 201,	// 기사도
	eTIN_SWORDSMANSHIP,					// 검술
	eTIN_WIZARDRY,						// 위저드리
	eTIN_LYCANTROPHY,					// 라이칸 트로피
	eTIN_FANATICISM,					// 광신
	eTIN_ANGELRANK,						// 천사계급학
	eTIN_KEYMANAFACTURING,				// 열쇠 제조업
	eTIN_HANDICRAFTSMAN,				// 기공술
	eTIN_RECPMMAOSSANCE,				// 정찰술
	eTIN_TACTICS,						// 용병술
	eTIN_PALEOGRAPHY,					// 고문학
	eTIN_THREMMATOLOGY,					// 사육학
	eTIN_COURTESY,						// 궁정예법
	eTIN_ASTRONOMY,						// 천문학
	eTIN_SPIRITISM,						// 강신술
	eTIN_DEVIL,							// 악마학
	eTIN_PSYCHIC,						// 심령술
	eTIN_INFIGHTER,						// 인파이터
	eTIN_OPTICAL					,	//	분광학

};
//
//	Title Effect
enum
{
	eTE_SKILL_MASTER		,	//	스킬 마스터
	eTE_PORTER				,			//	짐꾼
	eTE_BANK_CLIENT			,	//	은행 고객
	eTE_FRONTIER			,		//	선구자
	eTE_DEALER				,			//	거래인
	eTE_CITIZEN				,		//	시민
	eTE_PATHFINDER			,	//	길잡이           -
	eTE_BERSERKER			,	//	버서커
	eTE_ALCHEMY				,	//	연금술
	eTE_DISTINGUISHER		,	//	선악 감별        -
	eTE_CARTOGRAPHER		,	//	지도 제작자
	eTE_SURVIVAL_INSTINCTS	,	//	생존본능
	eTE_CITY_WALKER			,	//	시티 워커
	eTE_CAMPING_MASTER		,	//	캠핑 마스터
	eTE_MONSTER_DISCERNER	,	//	몬스터 감별사
	eTE_COURTESY			,	//	애교           -
	eTE_DRUGGIST			,	//	약사
	eTE_ESOTERIC			,	//	밀교
	eTE_TRAP_EVADE			,	//	함정회피
	eTE_LOCK_MASTER			,	//	자물쇠 명인
	eTE_TREASURE_HUNTER		,	//	트레져 헌터
	eTE_ELEMENTAL_MASTER	,	//	엘레멘탈 마스터
	eTE_ADVENTURER			,	//	모험가
	eTE_INCLINE_POWER		,	//	능력치 증가
	eTE_INCLINE_MAGIC_RESISTANCE	,	//	마법 저항 증가
	eTE_NATURAL_ENEMY		,	//	천적
	eTE_HERO				,	//	영웅
	eTE_ANTI_HERO			,	//	반영웅
	eTE_BARD				,	//	음유시인
	eTE_HUNTING_BONUS		,	//	음유시인
};

//
//	Compute Require Level Method
enum
{
	eCRLM_LEVEL_VALUE		,	//	레벨 수치 자체로 계산
	eCRLM_MULTIPLE_VALUE	,	//	칭호레벨*레벨 수치
	eCRLM_SQUARE_VALUE		,	//	칭호레벨^레벨 수치
};

class	CTitleEffectInfo
{
public:
	WORD			m_wEffect;
	int				m_aiValue[dMAX_TITLE_LEVEL][dMAX_TITLE_EFFECT_INFO_VALUE_COUNT];
};

class	CTitleInfo
{
public:
	WORD				m_wSerial;
	WORD				m_wComputeRequireLevelMethod,m_wRequireLevelValue;
	
	UINT				m_uiEnableJobMask;
	UINT				m_uiDefaultOwnJobMask;

	inline	BOOL		isEnableJob(int _iJob)
	{
		UINT	uiCheckMask	=	1<<_iJob;

		if	(m_uiEnableJobMask	&	uiCheckMask)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL		isDeafultOwnJob(int _iJob)
	{
		UINT	uiCheckMask	=	1<<_iJob;

		if	(m_uiDefaultOwnJobMask	&	uiCheckMask)
			return	TRUE;

		return	FALSE;
	}

	char				m_strName[dTITLE_NAME_LENGTH];
	CTitleEffectInfo	m_aEffect[dMAX_TITLE_EFFECT_COUNT];
	char				m_strExplain[dTITLE_EXPLAIN_LENGTH];

	WORD				m_bf1IsRemoveAble	:	1;
	WORD				m_bf1ExceptCH5Reward:	1;
};

#endif