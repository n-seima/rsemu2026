#ifndef _classJOB_H
#define _classJOB_H

#include "SFC.H"
#include "cIMAGE.H"
#include "cITEM.H"
#include "cSKILL.H"
#include "cAI.H"
#include "cBODY.H"
#include "jobDefine.H"
#include "jobCommon.H"

enum
{
	eNPC_PLAYER,			//	플레이어
	eNPC_NORMAL,			//	보통 NPC
	eNPC_MONSTER,			//	몬스터

	eNPC_WEAPON_MERCHANT,	//	무기 상인
	eNPC_ARMOR_MERCHANT,	//	방어구 상인
	eNPC_ETC_ITEM_MERCHANT,	//	잡화 상인
	eNPC_ACCESSORY_MERCHANT,//	도구 상인
	eNPC_PITCHMAN,			//	노점상
	eNPC_BANK,
	eNPC_SKILL_MASTER,

	eNPC_NORMAL_QUEST,
	eNPC_TITLE_QUEST,
	eNPC_GUILD_QUEST,
	eNPC_MAJOR_QUEST,
	eNPC_DOUMI,

	eNPC_TELEPORTER,

};	//	NPC 종류

#define	dJOB_GROUP_HERO								0
#define	dJOB_GROUP_MONSTER							1
#define	dJOB_GROUP_NPC								2

#define	dMAX_JOB_COUNT								2048
#define	dMAX_MONSTER_ABILITY_COUNT					10

#define	dATTACK_PART_ALL							0		//	전체
#define	dATTACK_PART_UPPER							1		//	상단 (헬멧)
#define	dATTACK_PART_MIDDLE							2		//	중단 (갑옷,건틀릿,벨트)
#define	dATTACK_PART_LOWER							3		//	하단 (갑옷,슈즈)

#define	dJOB_CHANGE_CORRECT_DATA_COUNT				6

#define	dALLIGNMENT_ABSOLUTE_EVIL					1		//	절대 악
#define	dALLIGNMENT_EVIL							2		//	악
#define	dALLIGNMENT_NEUTRAL							3		//	중립
#define	dALLIGNMENT_GOOD							4		//	선
#define	dALLIGNMENT_ABSOLUTE_GOOD					5		//	절대 선

#define	dJOB_SET_HERO								0
#define	dJOB_SET_CHARACTER							1

#define	dDEFAULT_JOB_COUNT							256

#define	dJOB_COUNT									128


#define	dJOB_VALID									0x7fff

const	DWORD	dJOB_NPC			=	128;					//	128~255까지 NPC다.


class	cJOB_BASE_DATA	//	기본 직업에 필요한 데이터
{
public:
	int				m_iLevel;							//	레벨
	DWORD			m_dwExperience;						//	경험치
	DWORD			m_dwSkillExperience;				//	기술 경험치

	int				m_iHP;								//	현재 체력			10.00	10,000.00  
	int				m_iMaxHP;							//	최대 체력			10.00	10,000.00  
	int				m_iCP;								//	현재   파워 게이지   1.00	 5,000.00  
	int				m_iMaxCP;							//	맥시멈 파워 게이지	10.00	10,000.00  
	WORD			m_wCorrectMaxHPFactor;				//	최대 HP 계산식에 필요한 HP 인자
	WORD			m_wCorrectMaxHPConstitutionFactor;	//	최대 HP 계산식에 필요한 건강 인자

	short			m_sStrength;						//	힘			5 1,000
	short			m_sAgility;							//	민첩성		5 1,000
	short			m_sConstitution;					//	건강		5 1,000
	short			m_sWisdom;							//	지혜		5 1,000
	short			m_sIntelligence;					//	지식		5 1,000
	short			m_sCharisma;						//	카리스마	5 1,000
	short			m_sLuck;							//	운			5 1,000
	short			m_sSight;							//	시야

	short			m_sMinDamage;						//	최소 데미지
	short			m_sMaxDamage;						//	최대 데미지
	short			m_sDefensivePower;					//	방어력

	short			m_sAllignment;						//	성향 -100.00~100.00(absoulte evil,evil,neutral,good,absoulte good)

	short			m_sFireResistance;					//	불 저항력 0% 100.00%  
	short			m_sWaterResistance;					//	물 저항력 0% 100.00%  
	short			m_sWindResistance;					//	바람 저항력 0% 100.00%  
	short			m_sEarthResistance;					//	대지 저항력 0% 100.00%  
	short			m_sLightResistance;					//	빛 저항력 0% 100.00%
	short			m_sDarkResistance;					//	어둠 저항력 0% 100.00%

	short			m_sBlindResistance;					//	장님 저항
	short			m_sPoisonResistance;				//	중독 저항
	short			m_sSleepResistance;					//	잠 저항
	short			m_sColdResistance;					//	콜드 저항
	short			m_sFreezeResistance;				//	프리즈 저항
	short			m_sStunResistance;					//	경직 저항
	short			m_sStoneResistance;					//	석화 저항
	short			m_sConfuseResistance;				//	혼란 저항
	short			m_sCharmingResistance;				//	챠밍 저항

	short			m_sBadStatusResistance;				//	상태 이상공격에 대한 저항 0% 100.00%
	short			m_sDeclinePowerResistance;			//	능력치 저하계에 대한 저항 0% 100.00%
	short			m_sCurseResistance;					//	저주에 대한 저항 0% 100.00%

	void			reset()
					{
						memset(this,0,sizeof(cJOB_BASE_DATA));

						m_wCorrectMaxHPFactor				=	100;		//	최대 HP 계산식에 필요한 HP 보정치
						m_wCorrectMaxHPConstitutionFactor	=	100;		//	최대 HP 계산식에 필요한 건강 보정치

						m_iLevel				=	1;			//	레벨
						m_sAllignment			=	0;
						m_sSight				=	100;

						m_iHP					=	1000;		//	현재 체력			10.00	10,000.00  
						m_iMaxHP				=	1000;		//	최대 체력			10.00	10,000.00  
						m_iCP					=	100;		//	현재   파워 게이지   1.00	 5,000.00  
						m_iMaxCP				=	1000;		//	맥시멈 파워 게이지	10.00	10,000.00  

						m_sMinDamage			=	1;
						m_sMaxDamage			=	2;

						m_sStrength				=	5;			//	힘			5 1,000
						m_sAgility				=	5;			//	민첩성		5 1,000
						m_sConstitution			=	5;			//	건강		5 1,000
						m_sWisdom				=	5;			//	지혜		5 1,000
						m_sIntelligence			=	5;			///	지식		5 1,000
						m_sCharisma				=	5;			//	카리스마	5 1,000
						m_sLuck					=	5;			//	운			5 1,000
					}
};

#define	dJOB_CATEGORY_UNDEAD		0
#define	dJOB_CATEGORY_HUMAN			1
#define	dJOB_CATEGORY_DEVIL			2
#define	dJOB_CATEGORY_ANIMAL		3
#define	dJOB_CATEGORY_HOLY_BEAST	4

class	cJOB_DATA :  public cNpcData_common
{
public:
						cJOB_DATA()
						{
							m_iSerial		=	0xffff;
							m_wBody			=	0xffff;
						}

	void				reset()
	{

		int	i;

		memset(this,0,sizeof(cJOB_DATA));

		m_iSerial					=	0xffff;
		m_wBody						=	0xffff;

		m_dwDeathExp				=	100;				//	죽었을때 주는 경험치
		m_wMoneyDroppingRate		=	90;					//	돈이 떨궈질 확률

		m_wMoveSpeed				=	100;
		m_wAttackSpeed				=	200;
//		m_wKind						=	0;
		m_wLimitLevel				=	50;
		m_wAllignment				=	dALLIGNMENT_NEUTRAL;

		m_wBasicPowerLevelFactor	=	100;		//	레벨에 따른 능력치 증가
		m_wMinDamageLevelFactor		=	100;
		m_wMaxDamageLevelFactor		=	100;
		m_wDefensePowerLevelFactor	=	100;
		m_bf1IsEnableTameMonster	=	FALSE;
		m_bf15TameResistanceValue	=	1;

		for (i=0;i<dDROPPING_ITEM_COUNT			;i++)	m_aDroppingItem[i].reset();
		for (i=0;i<dMAX_MONSTER_ABILITY_COUNT	;i++)
		{	
			m_aAbility[i].m_wLevel	=	0;
			m_aAbility[i].m_wSkill	=	0xffff;
			m_aLevelForAddSkillLevel[i]	=	100;
		}

		strcpy(m_strName,"");
	}

	int				getAbilityIndex(int skill)
	{
		if (skill	==	0xffff)	return	0xffff;

		for (int i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
			if (m_aAbility[i].m_wSkill==skill)	return	i;

		return	0xffff;
	}

	int				getValidAbilityIndex()
	{
		for (int i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
			if (m_aAbility[i].m_wSkill==0xffff)	return	i;

		return	0xffff;
	}

	BOOL			removeSkill(int _iSkill)			//		
	{
		for (int i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
		{
			if (m_aAbility[i].m_wSkill	==	_iSkill)
			{
				m_aAbility[i].m_wSkill	=	0xffff;

				return	TRUE;
			}
		}

		return	FALSE;
	}
};

class	cJOB		:	public cJOB_DATA,public cJOB_BASE_DATA
{
public:
					cJOB()
					{
						reset();
					}

	void			reset()
					{
						cJOB_BASE_DATA::reset();
						cJOB_DATA::reset();
					}

	void			setBody(int _iBody)	{m_wBody	=	_iBody;}
	void			copy(cJOB *_lpJob);

//////////////////////////////////////////////////////////////////////////////////////
//	정적 멤버
//////////////////////////////////////////////////////////////////////////////////////
	static	BOOL	Init();

	static	BOOL	IsAvailJob(int job);
	static	cJOB	*GetJob(int job);
	static	char	*GetName(int job);					//		job의 직업 이름을 얻는다.

	static	BOOL	AddHero(cJOB *lpJob);
	static	BOOL	AddCharacter(cJOB *lpJob);

	static	int		GetCharacterCount();
};


class	cJOB_DEN
{
public:
	cJOB				m_aJobList[dMAX_JOB_COUNT];		//	리스트
	int					m_iCount,m_iRookie;				//	카운트,루키

						cJOB_DEN();
						~cJOB_DEN();

	void				init();							//	초기화
	void				reset();							//	초기화
	void				rebuild();
	cJOB*				getJob(int _iIndex);
	cJOB*				getJob(char *_strName);
	cJOB*				addJob(cJOB *_lpJob);
	BOOL				remove(int _iIndex);
	BOOL				remove(char *_strName);
	BOOL				saveData(char *_lpstrFileName);
};

extern	cJOB_DEN			g_denJob;

extern	char				*g_aDefaultJobName[];	//	기본 직업 이름
extern	char				*g_strAllignment[];		//	성향
extern	char				*g_strJobCategory[];	//	직업 카테 고리
extern	char				*g_strNpcCategorty[];
extern	char				*g_strMonsterLevel[];	//	몬스터 등급
extern	char				*g_strMonsterConcept[];	//	몬스터 컨셉

extern	int					g_aiEnableJobIDCList[];

#endif