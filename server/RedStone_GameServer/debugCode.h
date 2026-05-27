#ifndef _definValueForDebug_h
#define _definValueForDebug_h

#include "SFC.h"
#include "CExceptionHandler.h"

class	cOldDebugValue_050330
{
public:
	int			m_iCurrentPlayer,m_iCurrentMonster,m_iCurrentNpc;
	int			m_iCurrentUpdateField,m_iCurrentGameStep,m_iCurrentFieldStep;
	int			m_iCurrentGameUpdateStep;
	int			m_iLastSkill,m_iLastSkillLevel;
	int			m_iLastUpdateClientPacket,m_iLastUpdateWorldPacket,m_iLastUpdateDBPacket;
	int			m_iActiveSkillStep;
	
	int			m_iExplosionSkillStep,m_iExplosionSkillLocalPart,m_iExplosionSkillTeam,m_iExplosionSkillActorCount;
	int			m_iGetExplosionSkillStep,m_iGetExplosionSkillLocalPart,m_iGetExplosionSkillTeam,m_iGetExplosionSkillActorCount;
	int			m_iAuraSkillStep,m_iAuraSkillTeam,m_iAuraSkillTargetCount,m_iAuraSkillLocalPart;
	int			m_iTempStep;
};

class	cDebugValueAboutActiveSkill
{
public:
	int			m_iCurrentSkill,m_iCurrentSkillLevel,m_iActiveSkillManagerStep;
	int			m_iActiveSkillInnerStep;
};

class	cDebugValueAboutPacket
{
public:
	int			m_iLastUpdateClientPacket,m_iLastUpdateWorldPacket,m_iLastUpdateDBPacket;
};

class	cDebugValueAboutActor
{
public:
	int			m_iStep;
};

class	cDebugValue
{
public:
	int			m_iCurrentGameStep,m_iCurrentFieldStep;

	cDebugValueAboutActiveSkill	m_activeSkill;
	cDebugValueAboutPacket		m_packet;
	cDebugValueAboutActor		m_player,m_monster;

	int			m_iCurrentUpdateField,m_iCurrentPlayer,m_iCurrentMonster,m_iCurrentNpc;
	
	int			m_iExplosionSkillStep,m_iExplosionSkillLocalPart,m_iExplosionSkillTeam,m_iExplosionSkillActorCount;
	int			m_iGetExplosionSkillStep,m_iGetExplosionSkillLocalPart,m_iGetExplosionSkillTeam,m_iGetExplosionSkillActorCount;
	int			m_iAuraSkillStep,m_iAuraSkillTeam,m_iAuraSkillTargetCount,m_iAuraSkillLocalPart;
	int			m_iTempStep , m_iTrigerStep , m_iTrigerStepError , m_iTrigerErrorDetail , m_iTrigerErrorDetailValue;
	cFILE		m_file;
	int			m_aiExValue[10];

	void		reset();
	void		init();
	void		writeSign(char *_lpstrText);

};

extern	cDebugValue	g_debugSign;
extern	BOOL		g_bIsBlockSetItem;
extern	BOOL		g_bIsTestBCS_Deliver;

extern	BOOL		g_bIsTestGuildBattle;
extern	BOOL		g_bIsStressTestServer;
extern	BOOL		g_bIsEnableGuildChat;
extern	BOOL		g_bIsEnablePVPMagicDamageRule;
extern	BOOL		g_bIsDuelTestPC;
extern	BOOL		g_bIsIgnoreSSInfo;
extern	BOOL		g_bIsStopInstanceGuildBattle;
extern	BOOL		g_bIsWantPrintTimeKarmaInfo;

extern	int			g_bIsChansTestServer;
extern	int			g_bIsInnerTestServer;
extern	int			g_bIsTestServer;
extern	int			g_bIsTestSiegeWarfare;
extern	int			g_iOutputStatus;
extern	BOOL		g_bIsMainTestServer;	//	본서버랑 설정이 똑같은 테섭. 결재나 몇가지 게임 외적인것만 테스트다.

extern	BOOL		g_bIsUseXTrap;

extern	BOOL		g_bIsOperatorServer;	//	 개발자 서버다..

#define	mDSEXValue(a)		g_debugSign.m_aiExValue[a]
#define	mDS		g_debugSign
#define	mDS_AS	g_debugSign.m_activeSkill
#define	mDS_PK	g_debugSign.m_packet
#define	mDS_P	g_debugSign.m_player
#define	mDS_M	g_debugSign.m_monster

///////////////////////////////////////////////////////////////////////////////////////////////
//	얼티미트 아이템 드랍과 관련된 설정
#ifdef	_FOR_JAPAN	//	일본 버젼이면 얼티미트 아이템 안나오게 한다.
	#define	dIS_DROP_ULTIMATE_ITEM_ONLY_TEST_SERVER		1
	#define	dIS_DROP_ULTIMATE_PREFIX_ONLY_TEST_SERVER	1
	#define	dBLOCKED_TO_DROP_ULTIMATE_ITEM				1
#endif


#define	dBLOCKED_TO_DROP_DX_UNIQUE_ITEM	1

//	한국은 얼티미트 아이템은 나오고 얼티미트 접두사는 테섭에서만 나온다
#ifndef	dIS_DROP_ULTIMATE_PREFIX_ONLY_TEST_SERVER
	#define	dIS_DROP_ULTIMATE_PREFIX_ONLY_TEST_SERVER	1
#endif

#define	dIS_USE_BLOCK_STEAL_SYSTEM	1

#ifdef	_FOR_KOREA
	#undef	dIS_USE_BLOCK_STEAL_SYSTEM
#elif	_FOR_JAPAN
	#undef	dIS_USE_BLOCK_STEAL_SYSTEM
#endif

#endif