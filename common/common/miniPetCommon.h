#pragma	once

#include	<windows.h>

const	int	c_iMiniPetCount			=	16;
const	int	c_iMiniPetSkillCount	=	11;
const	int	c_iMiniPetNameLength	=	10;
const	int	c_iMiniPetSkillMaxLevel	=	5;
const	int	c_iMiniPetEvolutionLevel=	30;

const	int	c_iMiniPetEvolutionToVital		=	6;
const	int	c_iMiniPetEvolutionToEnergy		=	12;
const	int	c_iMiniPetEvolutionToMineral	=	18;

namespace	nsMiniPetType
{
	enum	
	{
	SpritOfFire,
	SpritOfWater,
	SpritOfWind,
	SpritOfEarth,
	SpritOfLight,
	SpritOfDark,

	Salamander,
	Undine,
	Silf,
	Nom,
	Wisp,
	Shadow,

	Hwa,
	Soo,
	Poong,
	Ji,
	Vit,
	Am,

	Haoring,
	Mioring,
	Sooring,
	Daoring,
	Chaoring,
	Goring,
	Count,
	};
};

const	int	c_iMiniPetSpareSkillType		=	nsMiniPetType::Count+1;

namespace	nsMiniPetStyle
{
	enum	
	{
	Deault,
	Bio,
	Energy,
	Mineral,
	};
};

namespace	nsMiniPetST
{
	enum		//	Mini Pet Skill Type
	{
	None,	//	전투중인 상대 공격
	AttackUnderBattleEnemy,	//	전투중인 상대 공격
	ActionWhenDamaged,	//	맞을때발동
	LordPowerUp,		//	주인파워업
	AidToLord,		//	주인돕기
	PowerUpUnderBattleToBoss,	//	파워업_보스전
	MiniPetSkillPowerUp,	//	미니펫스킬파워업
	AidToUnderBattleLord,	//	전투중주인보조
	PickItem,	//	아이템줍기
	AvoidTrap,	//	함정회피
	PickGold,	//	골드획득
	DecreaseResurrectionPenelty,	//	부활페널티감소
	Resurrection,	//	부활페널티감소
	IncreaeUnlockLevel,	//	잠금해체레벨증가
	UseHealPotion,	//	포션효율증가
	UseChargePotion,	//	CP포션효율증가
	RecoverUseCP,	//	사용CP회복
	HealBonusByHealSkill,	//	힐스킬회복체력증가
	IncreaseDropItemChance,	//	아이템드랍률증가
	IncreaseDropGoldChance,	//	골드드랍률증가
	AttackTheObject,
	IgnoreTargetMagicResistanceUnderBossBattle,	//	타겟마법저항무시
	ChargeAndFire,	//	스킬누적공격
	DecreaseChargeCount,	//	스킬누적공격
	IncreaseActiveChanceUnderBossBattle,
	};
}

#pragma	pack(1)

class	cMiniPetAbilityInfo
{
public:
	WORD	m_bf11Skill	:	11;
	WORD	m_bf4Level	:	4;
	WORD	m_bf1IsOFF	:	1;
};

class	cMiniPetCommon
{
public:
	UINT	m_bf7Level		:	7;	//	레벨
	UINT	m_bf5Type		:	5;	//	타입
	UINT	m_bf16Exp		:	16;	//	경험치
	UINT	m_bf4ExpireMonth:	4;	//	32

	UINT	m_bf5ExpireYear	:	5;
	UINT	m_bf5ExpireDay	:	5;
	UINT	m_bf5ExpireHour	:	5;
	UINT	m_bf6ExpireMinute:	6;
	UINT	m_bf7SkillPoint	:	7;	//	28
	UINT	m_bf1MinipetAwaken50	:	1;	//	29
	UINT	m_bf1MinipetAwaken100	:	1;	//	30

	int		m_iVitalPoint,m_iEnergyPoint,m_iMineralPoint;

	char	m_strName[c_iMiniPetNameLength];
	cMiniPetAbilityInfo	m_aAbility[c_iMiniPetSkillCount];

	inline	int		getStyle()
	{
		return	m_bf5Type/6;
	}
	inline	int		getAttr()
	{
		return	m_bf5Type%6;
	}
};

#define	dSTATE_POINT_FOR_EVERY_RESTRAINTED_MINIPET	10
#define	dLIMIT_EAT_SAME_TYPE_MINIPET_COUNT			10		//	동일 종류 미니펫 먹는게 가능한 횟수

#pragma	pack()