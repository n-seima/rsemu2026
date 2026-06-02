#pragma	once

#include "playerJobCommon.h"

class	cNpcData_common
{
public:
	int					m_iSerial;
	char				m_strName[28];						//	직업 이름1
	BYTE				m_abBodyShape[4];					//	몬스터 바디 쉐입

	WORD				m_wBody;							//	몸통
	WORD				m_wVariation;						//	몸통
	WORD				m_wPalette;							//	두개의 몸통에 팔레트 넘버
	WORD				m_bf1IsEnableTameMonster	:	1;
	WORD				m_bf15TameResistanceValue	:	15;
	WORD				m_wBattleInclination;				//	전투 패턴
	WORD				m_bf4AttackPart				:	4;	//	공격부위
	WORD				m_bf1IsZinBoss				:	1;	//	진짜 보스
	WORD				m_bf1IsGuildGuardian		:	1;	//	길드 가디언이다.
	WORD				m_bf1IsReviseIntCalc			:	1;	//	지능 적용 공식 정상화
	WORD				m_bf3RequireRebirthCountForTame	:	3;	//	조련을 위해 필요한 전생 횟수
	
	
	WORD				m_wType;							//	영웅,NPC,몬스터
	
	//	 몬스터를 위해 -_-추가되는 데이터들..
	WORD				m_wKind;							//	분류(언데드,인간..)
	WORD				m_wBasicPowerLevelFactor;			//	레벨에 따른 능력치 증가
	WORD				m_wMinDamageLevelFactor,m_wMaxDamageLevelFactor,m_wDefensePowerLevelFactor;
	WORD				m_wMoveSpeed,m_wAttackSpeed;
	WORD				m_wCriticalShot,m_wCrushBlow;
	short				m_sCorrectHit,m_sCorrectDodge;
	WORD				m_wBlocking;
	WORD				m_wLimitLevel;
	WORD				m_wAllignment;
	WORD				m_wMonsterLevel;					//	몬스터 등급
	short				m_sCriticalResistance,m_sCrushResistance;

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃사망후 데이터
	DWORD				m_dwDeathExp;						//	죽었을때 주는 경험치
	WORD				m_wGold;							//	가지고 있는 돈
	WORD				m_wGoldPerLevel;					//	가지고 있는 돈(레벨에 따른 증가량)
	WORD				m_wMoneyDroppingRate;				//	돈이 떨궈질 확률
	cDroppingItem		m_aDroppingItem[dDROPPING_ITEM_COUNT];	//	죽었을때 주는 아이템둘
	//┃사망후 데이터
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
	CAbilityDefine		m_aAbility[dMAX_MONSTER_ABILITY_COUNT];					//	스킬
	WORD				m_aLevelForAddSkillLevel[dMAX_MONSTER_ABILITY_COUNT];	//	스킬
};


enum
{
	eJobDataFileVersion_begin	=	0,
	eJobDataFileVersion_end,
};

const	int	c_iJobDataFileCurrentVerion	=	eJobDataFileVersion_end-1;
#define	dJobDataFileCurrentVerion	c_iJobDataFileCurrentVerion