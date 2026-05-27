#ifndef	_classPLAYER_STATUS_H
#define	_classPLAYER_STATUS_H

#include "cNEMO.H"
#include "cItem.H"

#define	dMAXIMUM_MAGIC_RESISTANCE_VALUE			100	//	최대 마법저항
#define	dMAXIMUM_RESISTANCE_VALUE				200	//	최대 저항
#define	dMAXIMUM_ABSORB_VALUE					200
#define	dMAXIMUM_UNIQUE_ITEM_DROP_RATE			200
#define	dMAXIMUM_RARE_ITEM_DROP_RATE			200
#define	dMAXIMUM_RESURRECTION_CHANCE			100
#define	dMAXIMUM_REGEN_HP						20000
#define	dMAXIMUM_TRANSFORMATION_SPEED			200
#define	dMAXIMUM_FIRST_AID_RATE					200
#define	dMAXIMUM_RECOVER_SPEED_BY_POTION		255

#define	dMAXIMUM_RETURN_DAMAGE					200
#define	dMAXIMUM_DAMAGE_TO_CP_RATE				200
#define	dMAXIMUM_INSTANCE_KILL_CHANCE			200
#define	dMAXIMUM_MAKE_FLEE_CHANCE				200
#define	dMAXIMUM_KNOCKOUT_ATTACK_CHANCE			200
#define	dMAXIMUM_MAKE_BERSERK_CHANCE			200
#define	dMAXIMUM_MAKE_BREAK_ARMOR_CHANCE		200
#define	dMAXIMUM_MAKE_BREAK_WEAPON_CHANCE		200
#define	dMAXIMUM_MAKE_CONFUSE_CHANCE			200
#define	dMAXIMUM_MAKE_CHARM_CHANCE				200
#define	dMAXIMUM_MAKE_STUN_CHANCE				200
#define	dMAXIMUM_MAKE_COLD_CHANCE				200
#define	dMAXIMUM_MAKE_FREEZE_CHANCE				200
#define	dMAXIMUM_MAKE_BLIND_CHANCE				200
#define	dMAXIMUM_POISON_DAMAGE					255
#define	dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE		255
#define dMAXIMUM_ADDITIONAL_MAGIC_DAMAGE_DX		20000		// 09.08.07 
#define	dMAXIMUM_CORRECT_RELOAD_SPEED			90

#define	dMAXIMUM_FIRST_AID_TIME					180
#define	dMAXIMUM_CONCENTRATION_VALUE			90

#define	dMAXIMUM_MAGIC_STRENGTHENING_VALUE		120	//	최대 마법 증폭
#define	dMAXIMUM_MAGIC_STRENGTHENING_VALUE_FOR_UNIQUE	300


#define	dREDUCE_CP_RUNNING_FACTOR				10

#define	dMAXIMUM_BLOCKING_CHANCE				75		//	최고 블럭킹률
#define	dMAXIMUM_CRITICAL_HIT_CHANCE			100		//	최고 크리티컬 보정치
#define	dMAXIMUM_CRUSH_BLOW_CHANCE				100		//	최고 크러쉬샷 보정치
#define	dMAXIMUM_LIFE_STEAL_PERCENTAGE			100		//	최대 라이프 스틸 수치
#define	dMAXIMUM_BONUS_CP						100		//	최대 라이프 스틸 수치

#define	dMAXIMUM_MAKE_STONE_CHANCE				200
#define	dMAXIMUM_MAKE_SLEEP_CHANCE				200
#define	dMAXIMUM_MAKE_FIRSTTARGET_CHANCE		200
#define	dMAXIMUM_MAKE_SIMPLTONE_CHANCE			200

#define	dDECLINE_HIT_CHANCE_ATTACK_FACTOR		30		//	명중률저하 공격 인자
#define	dDECLINE_DODGE_CHANCE_ATTACK_FACTOR		30		//	회피율저하 공격 인자

////////////////////////////////////////////////////////////
//	거신의 은총 관련.
////////////////////////////////////////////////////////////
// class cACTOR_GGG_CLIENT
// {
// public:
// 	WORD				m_wGGGTimeForSkillUp;			//GGG	=	GreatGodGrace 
// 	
// 	WORD				m_wGGGValue1ForSkillUp;			//적용할 값.
// 	WORD				m_wGGGValue2ForSkillUp;			//적용할 값.
// 	
// 	inline	void		ResetGGG()
// 	{
// 		ResetGGGSkill();
// 	}
// 	
// 	inline void			ResetGGGSkill()
// 	{
// 		m_wGGGTimeForSkillUp	= 0;
// 		m_wGGGValue1ForSkillUp	= 0;
// 		m_wGGGValue2ForSkillUp	= 0;
// 	}
// };
enum		//09.08.10
{
	eADD_FIRE_DAMAGE_DX,
	eADD_WATER_DAMAGE_DX,
	eADD_WIND_DAMAGE_DX,
	eADD_LIGHT_DAMAGE_DX,
	eADD_DARK_DAMAGE_DX,
	eADD_EARTH_DAMAGE_DX,
	eADD_DAMAGE_DX_END,

};

struct sADD_NATURAL_DAMAGE_DX		//09.08.10
{
	WORD	wMin;
	WORD	wMax;
};

class	cFightEnergy
{	// 투기..
public:
	WORD	m_wTime;
	WORD	m_wState;
	int		m_iValue;
	
	cFightEnergy()
	{
		reset();
	}
	
	~cFightEnergy()
	{
	}
	
	
	void	set(WORD	_wFightEnergyTime, WORD	_wFightEnergyState, int	_iFightEnergyValue)
	{
		m_wTime	= _wFightEnergyTime;
		m_wState = _wFightEnergyState;

		if(isIncreaseAttackCount() || isDecreaseCoolTime() )
		{
			m_iValue = _iFightEnergyValue/100;
		}
		else if(isIncreaseAbsoluteCriticalPercent())
		{	
			m_iValue = _iFightEnergyValue/10;
		}
		else
		{
			m_iValue = _iFightEnergyValue;
		}
	}
	void	reset()
	{
		m_wTime	= 0;
		m_wState = 0;
		m_iValue = 0;
	}
	void	decreaseTime()
	{
		if(m_wTime)
		{
			--m_wTime;
			if(m_wTime == 0)
				reset();
		}
	}
	BOOL	isFightEnergy()
	{
		if(m_wTime)
		{
			if(m_wState)
				return TRUE;
		}
		return FALSE;
	}	
	BOOL	isAddAbsoluteAttackPower()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_ADD_ABSOLUTE_ATTACK_POWER)
				return TRUE;
		}
		return FALSE;
	}
	BOOL	isIncreaseAttackCount()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_INCREASE_ATTACK_COUNT)
				return TRUE;
		}
		return FALSE;
	}
	BOOL	isIncreaseGetCP()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_INCREASE_GET_CP)
				return TRUE;
		}
		return FALSE;
	}
	BOOL	isIncreaseAbsoluteCriticalPercent()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_INCREASE_ABSOLUTE_CRITICAL_PERCENT)
				return TRUE;
		}
		return FALSE;
	}
	BOOL	isDecreaseCoolTime()
	{
		if(m_wTime)
		{
			if(m_wState == eSKILL_FEE_DECREASE_COOL_TIME)
				return TRUE;
		}
		return FALSE;
	}
};


class	cPLAYER_STATUS
{
public:
	//
	//	가장 기본적인 스타일
	class	cACTOR_FACTOR_TYPE1
	{
	public:
		short	m_sValue;		//	장비한 상태에서의 수치
		short	m_sFixValue;

		short	m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
		WORD	m_wTime;		//	유지 시간
		WORD	m_bf1IsCanNotApplySameEffect	:	1;	//1	중복불가.
		WORD	:0;

		inline	BOOL	isWrong()
		{
			if (m_sCorrectValue				<	0)	return	TRUE;					//	더하거나 빼야 하는 수치

			return	FALSE;
		}

		inline	void	cure()
		{
			if (m_sCorrectValue	<	0)	m_sCorrectValue			=	0;
		}

		inline	BOOL	decreaseTime()	//	시간 줄여~~
		{
			if (m_wTime	==	0)	return FALSE;

			m_wTime--;
			if (m_wTime	==	0)
			{
				m_sCorrectValue	=	0;
				m_bf1IsCanNotApplySameEffect	=	FALSE;
				return	TRUE;
			}
			return	FALSE;
		}

		inline	void	resetAidData()
		{
			m_sCorrectValue	=	0;	//	더하거나 빼야 하는 수치
			m_wTime			=	0;	//	유지 시간
		}

		inline	void	resetEquipmentValue()	//	장비한 상태에서의 수치 초기화
		{
			m_sValue		=	0;	//	더하거나 빼야 하는 수치
			m_sFixValue		=	0x7fff;
		}

		inline	void	correctValue(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{

			if(m_bf1IsCanNotApplySameEffect)
			{
				if( _iValue > 0 && m_sCorrectValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffect	=	_bIsCanNotApplySameEffect;
			
			

			if (_iValue	<	0)
			{
				if (m_sCorrectValue	>	0)
				{
					if (-_iValue>=	m_sCorrectValue)	resetAidData();
				}
				else
				if (m_sCorrectValue	==	_iValue)
				{
					if (_iTime	>	m_wTime)	m_wTime	=	_iTime;
				}
				if (m_sCorrectValue	>	_iValue)
				{
					m_sCorrectValue	=	_iValue;
					m_wTime			=	_iTime;
				}

				return;
			}

			if (m_sCorrectValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectValue)	resetAidData();
			}
			else
			if (m_sCorrectValue	==	_iValue)
			{
				if (_iTime	>	m_wTime)	m_wTime	=	_iTime;
			}
			else
			if (m_sCorrectValue	<	_iValue)
			{
				m_sCorrectValue	=	_iValue;
				m_wTime			=	_iTime;
			}
		}

		inline	int		getCorrectedValue()	
		{
			if (m_sFixValue != 0x7fff)	return m_sFixValue;

			return	m_sValue+m_sCorrectValue;
		}
		inline	BOOL	isFixedValue()	
		{
			if (m_sFixValue == 0x7fff)
				return FALSE;

			return	TRUE;
		}
		inline	void	addValue(int _iValue,int _iLimit=0x7fffffff)
		{
			m_sValue	=	min(m_sValue+_iValue,_iLimit);
		}

		inline	void	fixValue(int _iFixValue)
		{
			if (m_sFixValue	!=	0x7fff)
				m_sFixValue	=	max(m_sFixValue,_iFixValue);
			else
				m_sFixValue	=	_iFixValue;
		}
	};

	//
	//	HP,CP
	class	cACTOR_FACTOR_TYPE2
	{
	public:
		int				m_iValue;							//	장비한 상태에서의 수치

		short			m_sCorrectValueByPrefix;				//	더하거나 빼야 하는 수치
		short			m_sCorrectPercentageValueByPrefix;	//	더하거나 빼야 하는 수치(퍼센티지)

		short			m_sCorrectValue;					//	더하거나 빼야 하는 수치
		short			m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
		WORD			m_wTimeForCorrectValue;				//	초
		WORD			m_wTimeForCorrectPercentageValue;	//	초
		WORD			m_bf1IsCanNotApplySameEffect		:	1;	//1	중복불가.	
		WORD			m_bf1IsCanNotApplySameEffectPercent	:	1;	//2	중복불가.
		WORD			:0;
		inline	BOOL	isWrong()
		{
			if (m_sCorrectValue				<	0)	return	TRUE;					//	더하거나 빼야 하는 수치
			if (m_sCorrectPercentageValue	<	0)	return	TRUE;			//	더하거나 빼야 하는 수치(퍼센티지)

			return	FALSE;
		}

		inline	void	cure()
		{
			if (m_sCorrectValue	<	0)
			{
				m_wTimeForCorrectValue	=	0;
				m_sCorrectValue			=	0;
			}
			if (m_sCorrectPercentageValue	<	0)
			{
				m_wTimeForCorrectPercentageValue	=	0;
				m_sCorrectPercentageValue			=	0;
			}
		}

		inline	int		getCorrectValue()			{return	m_sCorrectValueByPrefix+m_sCorrectValue;}
		inline	int		getCorrectPercentageValue()	{return	m_sCorrectPercentageValueByPrefix+m_sCorrectPercentageValue;}

		inline	BOOL	decreaseTime()	//	시간 줄여~~
		{
			if (m_wTimeForCorrectValue)
			{
				m_wTimeForCorrectValue--;
				if (m_wTimeForCorrectValue	==	0)
				{
					m_sCorrectValue	=	0;
					m_bf1IsCanNotApplySameEffect =	 FALSE;
					return	TRUE;
				}
			}
			
			if (m_wTimeForCorrectPercentageValue)
			{
				m_wTimeForCorrectPercentageValue--;
				if (m_wTimeForCorrectPercentageValue	==	0)
				{
					m_sCorrectPercentageValue	=	0;
					m_bf1IsCanNotApplySameEffectPercent	=	FALSE;
					return	TRUE;
				}
			}

			return	FALSE;
		}

		inline	void	resetAidData()
		{
			m_sCorrectValue					=	0;	//	더하거나 빼야 하는 수치
			m_sCorrectPercentageValue		=	0;	//	더하거나 빼야 하는 수치
			m_wTimeForCorrectValue			=	0;		//	유지 시간
			m_wTimeForCorrectPercentageValue=	0;		//	유지 시간
			m_bf1IsCanNotApplySameEffect		=	FALSE;	//	중복불가
			m_bf1IsCanNotApplySameEffectPercent	=	FALSE;	//	중복불가
		}

		inline	void	resetPrefixData()	//	접두사에 의해 보정되는 수치만 초기화
		{
			m_sCorrectValueByPrefix			=	0;
			m_sCorrectPercentageValueByPrefix=	0;
		}

		inline	void	correctPercentageValue(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			if(m_bf1IsCanNotApplySameEffectPercent)
			{
				if( _iValue > 0 && m_sCorrectPercentageValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectPercentageValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffectPercent	=	_bIsCanNotApplySameEffect;
			

			if (_iValue	<	0)
			{
				if (m_sCorrectPercentageValue	>	0)	m_sCorrectPercentageValue	=	0,m_wTimeForCorrectPercentageValue	=	0;
				else
				if (m_sCorrectPercentageValue	<	0)
				{
					if (-m_sCorrectPercentageValue<	-_iValue)	m_sCorrectPercentageValue		=	_iValue,m_wTimeForCorrectPercentageValue	=	_iTime;
					else
					if (m_sCorrectPercentageValue	==	_iValue)	m_wTimeForCorrectPercentageValue	=	max(m_wTimeForCorrectPercentageValue,_iTime);
				}
				else	m_sCorrectPercentageValue		=	_iValue,m_wTimeForCorrectPercentageValue	=	_iTime;
			}
			else
			if (m_sCorrectPercentageValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectPercentageValue)
				{
					m_sCorrectPercentageValue			=	0;
					m_wTimeForCorrectPercentageValue	=	0;
				}
			}
			else
			if (m_sCorrectPercentageValue	==	_iValue)
			{
				if (_iTime	>	m_wTimeForCorrectPercentageValue)	m_wTimeForCorrectPercentageValue	=	_iTime;
			}
			else
			if (m_sCorrectPercentageValue	<	_iValue)
			{
				m_sCorrectPercentageValue			=	_iValue;
				m_wTimeForCorrectPercentageValue	=	_iTime;
			}
		}

		inline	void	correctValue(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			if(m_bf1IsCanNotApplySameEffect)
			{
				if( _iValue > 0 && m_sCorrectValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffect	=	_bIsCanNotApplySameEffect;
			
			if (_iValue	<	0)
			{
				if (m_sCorrectValue	>	0)	m_sCorrectValue	=	0,m_wTimeForCorrectValue	=	0;
				else
				if (m_sCorrectValue	<	0)
				{
					if (-m_sCorrectValue<	-_iValue)	m_sCorrectValue		=	_iValue,m_wTimeForCorrectValue	=	_iTime;
					else
					if (m_sCorrectValue	==	_iValue)	m_wTimeForCorrectValue	=	max(m_wTimeForCorrectValue,_iTime);
				}
				else	m_sCorrectValue		=	_iValue,m_wTimeForCorrectValue	=	_iTime;
			}
			else
			if (m_sCorrectValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectValue)
				{
					m_sCorrectValue			=	0;
					m_wTimeForCorrectValue	=	0;
				}
			}
			else
			if (m_sCorrectValue	==	_iValue)
			{
				if (_iTime	>	m_wTimeForCorrectValue)	m_wTimeForCorrectValue	=	_iTime;
			}
			else
			if (m_sCorrectValue	<	_iValue)
			{
				m_sCorrectValue			=	_iValue;
				m_wTimeForCorrectValue	=	_iTime;
			}
		}
	};

	//
	//	AP
	class	cACTOR_FACTOR_TYPE3
	{
	public:
		short			m_sCorrectValueByPrefix;			//	더하거나 빼야 하는 수치
		short			m_sCorrectPercentageValueByPrefix;	//	더하거나 빼야 하는 수치(퍼센티지)
		short			m_sCorrectValue;					//	더하거나 빼야 하는 수치
		short			m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
		WORD			m_wTimeForCorrectValue;				//	초
		WORD			m_wTimeForCorrectPercentageValue;	//	초
		short			m_sCorrectPercentageValueForPetCommand;
		WORD			m_wTimeForCorrectPercentageValueForPetCommand;	//	초
		WORD			m_bf1IsCanNotApplySameEffect			:1	;	//1	중복불가
		WORD			m_bf1IsCanNotApplySameEffectPercent		:1	;	//2	중복불가
		WORD			m_bf1IsCanNotApplySameEffectPetCommand	:1	;	//3	중복불가
		WORD			:0;
		inline	int		getCorrectValue()			{return	m_sCorrectValueByPrefix+m_sCorrectValue;}
		inline	int		getCorrectPercentageValue()	{return	m_sCorrectPercentageValueByPrefix+m_sCorrectPercentageValue+m_sCorrectPercentageValueForPetCommand;}

		inline	BOOL	isWrong()
		{
			if (m_sCorrectValue				<	0)	return	TRUE;	//	더하거나 빼야 하는 수치
			if (m_sCorrectPercentageValue	<	0)	return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)

			return	FALSE;
		}

		inline	void	cure()
		{
			if (m_sCorrectValue	<	0)
			{
				m_wTimeForCorrectValue	=	0;
				m_sCorrectValue			=	0;
			}
			if (m_sCorrectPercentageValue	<	0)
			{
				m_wTimeForCorrectPercentageValue	=	0;
				m_sCorrectPercentageValue			=	0;
			}
			if (m_sCorrectPercentageValueForPetCommand	<	0)
			{
				m_sCorrectPercentageValueForPetCommand			=	0;
				m_wTimeForCorrectPercentageValueForPetCommand	=	0;	//	초
			}
		}

		inline	void	dispell()
		{
			if (m_sCorrectValue	>	0)
			{
				m_wTimeForCorrectValue	=	0;
				m_sCorrectValue			=	0;
			}
			if (m_sCorrectPercentageValue	>	0)
			{
				m_wTimeForCorrectPercentageValue	=	0;
				m_sCorrectPercentageValue			=	0;
			}
			if (m_sCorrectPercentageValueForPetCommand	>	0)
			{
				m_sCorrectPercentageValueForPetCommand			=	0;
				m_wTimeForCorrectPercentageValueForPetCommand	=	0;	//	초
			}
		}

		inline	BOOL	decreaseTime()	//	시간 줄여~~
		{
			if (m_wTimeForCorrectValue)
			{
				m_wTimeForCorrectValue--;
				if (m_wTimeForCorrectValue	==	0)
				{
					m_sCorrectValue	=	0;
					m_bf1IsCanNotApplySameEffect	=	FALSE;
					return	TRUE;
				}
			}
			
			if (m_wTimeForCorrectPercentageValue)
			{
				m_wTimeForCorrectPercentageValue--;
				if (m_wTimeForCorrectPercentageValue	==	0)
				{
					m_sCorrectPercentageValue	=	0;
					m_bf1IsCanNotApplySameEffectPercent	=	FALSE;
					return	TRUE;
				}
				
			}
			if (m_wTimeForCorrectPercentageValueForPetCommand)
			{
				m_wTimeForCorrectPercentageValueForPetCommand--;
				
				if (m_wTimeForCorrectPercentageValueForPetCommand	==	0)
				{
					m_sCorrectPercentageValueForPetCommand			=	0;
					m_bf1IsCanNotApplySameEffectPetCommand			=	FALSE;
					return	TRUE;
				}
			}

			return	FALSE;
		}

		inline	void	resetAidData()
		{
			m_sCorrectValue					=	0;	//	더하거나 빼야 하는 수치
			m_sCorrectPercentageValue		=	0;	//	더하거나 빼야 하는 수치
			m_wTimeForCorrectValue			=	0;		//	유지 시간
			m_wTimeForCorrectPercentageValue=	0;		//	유지 시간
			m_sCorrectPercentageValueForPetCommand			=	0;
			m_wTimeForCorrectPercentageValueForPetCommand	=	0;	//	초
			m_bf1IsCanNotApplySameEffect					=	FALSE;
			m_bf1IsCanNotApplySameEffectPercent				=	FALSE;
			m_bf1IsCanNotApplySameEffectPetCommand			=	FALSE;
		}

		inline	void	resetPrefixData()	//	접두사에 의해 보정되는 수치만 초기화
		{
			m_sCorrectValueByPrefix			=	0;
			m_sCorrectPercentageValueByPrefix=	0;
		}

		inline	void	correctValue(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			if(m_bf1IsCanNotApplySameEffect)
			{
				if( _iValue > 0 && m_sCorrectValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffect	=	_bIsCanNotApplySameEffect;

			if (m_sCorrectValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectValue)
				{
					m_sCorrectValue			=	0;
					m_wTimeForCorrectValue	=	0;
				}
			}
			else
			if (m_sCorrectValue	==	_iValue)
			{
				if (_iTime	>	m_wTimeForCorrectValue)	m_wTimeForCorrectValue	=	_iTime;
			}
			else
			if (m_sCorrectValue	<	_iValue)
			{
				m_sCorrectValue			=	_iValue;
				m_wTimeForCorrectValue	=	_iTime;
			}
		}
		inline	void	addValue(int _iValue,int _iLimit,int _iTime)
		{
			if (m_sCorrectValue	<	0)
				return;

			if (m_sCorrectValue	>=	_iLimit)
				return;

			m_sCorrectValue			+=	_iValue;
			m_sCorrectValue			=	min(_iLimit,m_sCorrectValue);
			m_wTimeForCorrectValue	=	_iTime;
		}
	
		inline	void	correctPercentageValue(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			if(m_bf1IsCanNotApplySameEffectPercent)
			{
				if( _iValue > 0 && m_sCorrectPercentageValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectPercentageValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffectPercent	=	_bIsCanNotApplySameEffect;


			if (m_sCorrectPercentageValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectPercentageValue)
				{
					m_sCorrectPercentageValue			=	0;
					m_wTimeForCorrectPercentageValue	=	0;
				}
			}
			else
			if (m_sCorrectPercentageValue	==	_iValue)
			{
				if (_iTime	>	m_wTimeForCorrectPercentageValue)	m_wTimeForCorrectPercentageValue	=	_iTime;
			}
			else
			if (m_sCorrectPercentageValue	<	_iValue)
			{
				m_sCorrectPercentageValue			=	_iValue;
				m_wTimeForCorrectPercentageValue	=	_iTime;
			}
		}

		inline	void	correctPercentageValueForPetCommand(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			if(m_bf1IsCanNotApplySameEffectPetCommand)
			{
				if( _iValue > 0 && m_sCorrectPercentageValueForPetCommand > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectPercentageValueForPetCommand < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffectPetCommand	=	_bIsCanNotApplySameEffect;

			if (m_sCorrectPercentageValueForPetCommand	<	0)
			{
				if (_iValue	>=	-m_sCorrectPercentageValueForPetCommand)
				{
					m_sCorrectPercentageValueForPetCommand			=	0;
					m_wTimeForCorrectPercentageValueForPetCommand	=	0;
				}
			}
			else
			if (m_sCorrectPercentageValueForPetCommand	==	_iValue)
			{
				if (_iTime	>	m_wTimeForCorrectPercentageValueForPetCommand)	
					m_wTimeForCorrectPercentageValueForPetCommand	=	_iTime;
			}
			else
			if (m_sCorrectPercentageValueForPetCommand	<	_iValue)
			{
				m_sCorrectPercentageValueForPetCommand			=	_iValue;
				m_wTimeForCorrectPercentageValueForPetCommand	=	_iTime;
			}
		}

		inline			resetForPetCommandValue()
		{
			m_sCorrectPercentageValueForPetCommand			=	0;
			m_wTimeForCorrectPercentageValueForPetCommand	=	0;
			m_bf1IsCanNotApplySameEffectPetCommand			=	FALSE;
		}
	};

	//
	//	DP
	class	cACTOR_FACTOR_TYPE4
	{
	public:
		int				m_iValue;							//	장비한 상태에서의 수치

		short			m_sCorrectValueByPrefix;				//	더하거나 빼야 하는 수치
		short			m_sCorrectPercentageValueByPrefix;	//	더하거나 빼야 하는 수치(퍼센티지)
		short			m_sCorrectValue;					//	더하거나 빼야 하는 수치
		short			m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
		WORD			m_wTimeForCorrectValue;				//	초
		WORD			m_wTimeForCorrectPercentageValue;	//	초

		short			m_sCorrectPercentageValueForPetCommand;
		WORD			m_wTimeForCorrectPercentageValueForPetCommand;	//	초
		WORD			m_bf1IsCanNotApplySameEffect			:1	;	//1	중복불가
		WORD			m_bf1IsCanNotApplySameEffectPercent		:1	;	//2	중복불가
		WORD			m_bf1IsCanNotApplySameEffectPetCommand	:1	;	//3	중복불가
		WORD			:0;


		inline	BOOL	isWrong()
		{
			if (m_sCorrectValue				<	0)
				return	TRUE;	//	더하거나 빼야 하는 수치
			if (m_sCorrectPercentageValue	<	0)
				return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)
			if (m_sCorrectPercentageValueForPetCommand	<	0)
				return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)

			return	FALSE;
		}

		inline	void	cure()
		{
			if (m_sCorrectValue	<	0)
			{
				m_wTimeForCorrectValue	=	0;
				m_sCorrectValue			=	0;
			}
			if (m_sCorrectPercentageValue	<	0)
			{
				m_wTimeForCorrectPercentageValue	=	0;
				m_sCorrectPercentageValue			=	0;
			}
		}

		inline	void	dispell()
		{
			if (m_sCorrectValue	>	0)
			{
				m_wTimeForCorrectValue	=	0;
				m_sCorrectValue			=	0;
			}
			if (m_sCorrectPercentageValue	>	0)
			{
				m_wTimeForCorrectPercentageValue	=	0;
				m_sCorrectPercentageValue			=	0;
			}

			if (m_sCorrectPercentageValueForPetCommand	>	0)
			{
				m_sCorrectPercentageValueForPetCommand			=	0;
				m_wTimeForCorrectPercentageValueForPetCommand	=	0;	//	초
			}
		}

		inline	BOOL	decreaseTime()	//	시간 줄여~~
		{
			if (m_wTimeForCorrectValue)
			{
				m_wTimeForCorrectValue--;
				if (m_wTimeForCorrectValue	==	0)
				{
					m_sCorrectValue	=	0;
					m_bf1IsCanNotApplySameEffect	=	FALSE;	
					return	TRUE;
				}
			}
			
			if (m_wTimeForCorrectPercentageValue)
			{
				m_wTimeForCorrectPercentageValue--;
				if (m_wTimeForCorrectPercentageValue	==	0)
				{
					m_sCorrectPercentageValue	=	0;
					m_bf1IsCanNotApplySameEffectPercent	=	FALSE;
					return	TRUE;
				}
			}
			
			if (m_wTimeForCorrectPercentageValueForPetCommand)
			{
				m_wTimeForCorrectPercentageValueForPetCommand--;
				
				if (m_wTimeForCorrectPercentageValueForPetCommand	==	0)
				{
					m_sCorrectPercentageValueForPetCommand	=	0;
					m_bf1IsCanNotApplySameEffectPetCommand	=	FALSE;
					return	TRUE;
				}
			}

			return	FALSE;
		}

		inline	void	resetAidData()
		{
			m_sCorrectValue					=	0;	//	더하거나 빼야 하는 수치
			m_sCorrectPercentageValue		=	0;	//	더하거나 빼야 하는 수치
			m_wTimeForCorrectValue			=	0;	//	유지 시간
			m_wTimeForCorrectPercentageValue=	0;	//	유지 시간
			m_sCorrectPercentageValueForPetCommand			=	0;
			m_wTimeForCorrectPercentageValueForPetCommand	=	0;	//	초
			m_bf1IsCanNotApplySameEffect					=	FALSE;
			m_bf1IsCanNotApplySameEffectPercent				=	FALSE;
			m_bf1IsCanNotApplySameEffectPetCommand			=	FALSE;
		}
		inline	void	resetPrefixData()	//	접두사에 의해 보정되는 수치만 초기화
		{
			m_sCorrectValueByPrefix			=	0;
			m_sCorrectPercentageValueByPrefix=	0;
		}

		inline	int		getCorrectValue()	{return	m_sCorrectValueByPrefix+m_sCorrectValue;}
		inline	int		getCorrectPercentageValue()	{return	m_sCorrectPercentageValueByPrefix+m_sCorrectPercentageValue+m_sCorrectPercentageValueForPetCommand;}

		inline	void	correctValue(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			if(m_bf1IsCanNotApplySameEffect)
			{
				if( _iValue > 0 && m_sCorrectValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffect	=	_bIsCanNotApplySameEffect;

			if (m_sCorrectValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectValue)
				{
					m_sCorrectValue			=	0;
					m_wTimeForCorrectValue	=	0;
				}
			}
			else
			if (m_sCorrectValue	==	_iValue)
			{
				if (_iTime	>	m_wTimeForCorrectValue)	m_wTimeForCorrectValue	=	_iTime;
			}
			else
			if (m_sCorrectValue	<	_iValue)
			{
				m_sCorrectValue			=	_iValue;
				m_wTimeForCorrectValue	=	_iTime;
			}
		}
		inline	void	correctPercentageValue(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			
			if(m_bf1IsCanNotApplySameEffectPercent)
			{
				if( _iValue > 0 && m_sCorrectPercentageValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectPercentageValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffectPercent	=	_bIsCanNotApplySameEffect;
			
			

			if (m_sCorrectPercentageValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectPercentageValue)
				{
					m_sCorrectPercentageValue			=	0;
					m_wTimeForCorrectPercentageValue	=	0;
				}
			}
			else
			if (m_sCorrectPercentageValue	==	_iValue)
			{
				if (_iTime	>	m_wTimeForCorrectPercentageValue)	m_wTimeForCorrectPercentageValue	=	_iTime;
			}
			else
			if (m_sCorrectPercentageValue	<	_iValue)
			{
				m_sCorrectPercentageValue			=	_iValue;
				m_wTimeForCorrectPercentageValue	=	_iTime;
			}
		}
		inline	void	correctPercentageValueForPetCommand(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			if(m_bf1IsCanNotApplySameEffectPetCommand)
			{
				if( _iValue > 0 && m_sCorrectPercentageValueForPetCommand > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectPercentageValueForPetCommand < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffectPetCommand	=	_bIsCanNotApplySameEffect;

			if (m_sCorrectPercentageValueForPetCommand	<	0)
			{
				if (_iValue	>=	-m_sCorrectPercentageValueForPetCommand)
				{
					m_sCorrectPercentageValueForPetCommand			=	0;
					m_wTimeForCorrectPercentageValueForPetCommand	=	0;
				}
			}
			else
			if (m_sCorrectPercentageValueForPetCommand	==	_iValue)
			{
				if (_iTime	>	m_wTimeForCorrectPercentageValueForPetCommand)	
					m_wTimeForCorrectPercentageValueForPetCommand	=	_iTime;
			}
			else
			if (m_sCorrectPercentageValueForPetCommand	<	_iValue)
			{
				m_sCorrectPercentageValueForPetCommand			=	_iValue;
				m_wTimeForCorrectPercentageValueForPetCommand	=	_iTime;
			}
		}
		
		inline			resetForPetCommandValue()
		{
			m_sCorrectPercentageValueForPetCommand			=	0;
			m_wTimeForCorrectPercentageValueForPetCommand	=	0;
			m_bf1IsCanNotApplySameEffectPetCommand			=	FALSE;
		}
	};

	class	cACTOR_FACTOR_TYPE5	//	디폴트 수치가 0이다.
	{
	public:
		short	m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
		short	m_sRange;
		WORD	m_wTime;		//	유지 시간
		WORD			m_bf1IsCanNotApplySameEffect	:	1;	//	1	중복불가
		WORD			:0;
		inline	BOOL	isWrong()
		{
			if (m_sCorrectValue				<	0)
				return	TRUE;					//	더하거나 빼야 하는 수치

			return	FALSE;
		}

		inline	void	cure()
		{
			if (m_sCorrectValue	<	0)
			{
				m_sRange		=	0;
				m_sCorrectValue	=	0;
				m_wTime			=	0;
			}
		}

		inline	void	dispell()
		{
			if (m_sCorrectValue	>	0)
			{
				m_sRange		=	0;
				m_sCorrectValue	=	0;
				m_wTime			=	0;
			}
		}

		inline	BOOL	decreaseTime()	//	시간 줄여~~
		{
			if (m_wTime	==	0)	return FALSE;

			m_wTime--;
			if (m_wTime	==	0)
			{
				m_sCorrectValue	=	0;
				m_sRange		=	0;
				m_bf1IsCanNotApplySameEffect	=	FALSE;
				return	TRUE;
			}

			return	FALSE;
		}

		inline	void	resetAidData()
		{
			m_sCorrectValue	=	0;	//	더하거나 빼야 하는 수치
			m_wTime			=	0;	//	유지 시간
			m_sRange		=	0;
			m_bf1IsCanNotApplySameEffect	=	FALSE;
		}

		inline	void	correctValue(int _iValue,int _iRange,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			
			if(m_bf1IsCanNotApplySameEffect)
			{
				if( _iValue > 0 && m_sCorrectValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffect	=	_bIsCanNotApplySameEffect;

			if (_iValue	<	0)
			{
				if (m_sCorrectValue	>	0)
				{
					if (-_iValue>=	m_sCorrectValue)
						resetAidData();
				}
				else
				if (m_sCorrectValue	==	_iValue)
				{
					if (_iTime	>	m_wTime)
						m_wTime	=	_iTime;
				}
				if (m_sCorrectValue	>	_iValue)
				{
					m_sCorrectValue	=	_iValue;
					m_wTime			=	_iTime;
					m_sRange		=	_iRange;
				}

				return;
			}

			if (m_sCorrectValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectValue)
					resetAidData();
			}
			else
			if (m_sCorrectValue	==	_iValue)
			{
				if (_iTime	>	m_wTime)
					m_wTime	=	_iTime;
			}
			else
			if (m_sCorrectValue	<	_iValue)
			{
				m_sCorrectValue	=	_iValue;
				m_wTime			=	_iTime;
				m_sRange		=	_iRange;
			}
		}

		inline	int		getCorrectedValue()		{return	m_sCorrectValue+random(m_sRange);}
	};

	class	cACTOR_FACTOR_TYPE6	//	디폴트 수치가 0이다.
	{
	public:
		short	m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
		WORD	m_wUpkeepTime;
		WORD	m_bf1IsCanNotApplySameEffect	:	1;	//	1	중복불가
		WORD	:0;
		inline	BOOL	isWrong()
		{
			if (m_sCorrectValue	<	0)
				return	TRUE;					//	더하거나 빼야 하는 수치

			return	FALSE;
		}

		inline	void	cure()
		{
			if (m_sCorrectValue	<	0)
			{
				m_sCorrectValue	=	0;
				m_wUpkeepTime	=	0;
			}
		}

		inline	void	dispell()
		{
			if (m_sCorrectValue	>	0)
			{
				m_sCorrectValue	=	0;
				m_wUpkeepTime			=	0;
			}
		}

		inline	BOOL	decreaseTime()	//	시간 줄여~~
		{
			if (m_wUpkeepTime	==	0)
				return FALSE;

			m_wUpkeepTime--;

			if (m_wUpkeepTime	==	0)
			{
				m_sCorrectValue	=	0;
				m_bf1IsCanNotApplySameEffect	=	FALSE;
				return	TRUE;
			}

			return	FALSE;
		}

		inline	void	resetAidData()
		{
			m_sCorrectValue	=	0;	//	더하거나 빼야 하는 수치
			m_wUpkeepTime			=	0;	//	유지 시간
			m_bf1IsCanNotApplySameEffect	=	FALSE;
		}

		inline	void	correctValue(int _iValue,int _iTime, BOOL _bIsCanNotApplySameEffect = FALSE)
		{
			if(m_bf1IsCanNotApplySameEffect)
			{
				if( _iValue > 0 && m_sCorrectValue > 0 )
					return;
				
				if( _iValue < 0 && m_sCorrectValue < 0 )
					return;
			}
			
			m_bf1IsCanNotApplySameEffect	=	_bIsCanNotApplySameEffect;

			if (_iValue	<	0)
			{
				if (m_sCorrectValue	>	0)
				{
					if (-_iValue>=	m_sCorrectValue)
						resetAidData();
				}
				else
				if (m_sCorrectValue	==	_iValue)
				{
					if (_iTime	>	m_wUpkeepTime)
						m_wUpkeepTime	=	_iTime;
				}
				if (m_sCorrectValue	>	_iValue)
				{
					m_sCorrectValue	=	_iValue;
					m_wUpkeepTime			=	_iTime;
				}

				return;
			}

			if (m_sCorrectValue	<	0)
			{
				if (_iValue	>=	-m_sCorrectValue)
					resetAidData();
			}
			else
			if (m_sCorrectValue	==	_iValue)
			{
				if (_iTime	>	m_wUpkeepTime)
					m_wUpkeepTime	=	_iTime;
			}
			else
			if (m_sCorrectValue	<	_iValue)
			{
				m_sCorrectValue	=	_iValue;
				m_wUpkeepTime			=	_iTime;
			}
		}

		inline	int		getCorrectedValue()		{return	m_sCorrectValue;}
	};

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃특수 능력
	short				m_sInvisivilityTime;			//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
	short				m_sFloatTime;					//	공중부양 시간(-이면 장비에 붙어 있는거다.)
	short				m_sBlurTime;					//	흐릿한 상태 시간(-이면 장비에 붙어 있는거다.)
	short				m_sNonTargetTime;				//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)
	WORD				m_wReversalPowerTime;			//	
	WORD				m_wInterruptingArmorTime,m_wInterruptingArmorLevel;
	WORD				m_wInterruptingWeaponTime;		// 무기해제.
	WORD				m_wMaintainMaxCPTime;			//	이 시간동안 CP가 max로 유지된다.
	WORD				m_wMaintainMaxWeponDamageTime;	//	이 시간동안 무기의 데미지가 항상 max가 나온다.
	int					m_iFirstAidHP;					//	응급처치한 HP
	WORD				m_wFirstAidTime;				//	응급처치한 시간
	WORD				m_wIncreaseFinalPhysicalAttackPower;

	BYTE				m_bResurrectionChance;			//	죽었을때 일정 확률로 부활한다.
	BYTE				m_bIsInfinityBullet;				//	무한 탄환
	BYTE				m_isAutoReload;					//	자동으로 벨트의 아이템을 리로드 한다.
	BYTE				m_isMagicBullet;					//	마법 탄환을 쏠 수 있는 상태
	BYTE				m_isIgnoreTargetCorrectDodgeChance;	//	적의 회피 보정치 무시
	BYTE				m_isIgnoreTargetCorrectHitChance;	//	적의 명중 보정치 무시
	BYTE				m_isPerfectHit;					//	항상 명중
	BYTE				m_bGetBonusCPPercentage;		//	CP획득시 보너스..
	WORD				m_wIncreaseSitdownHPRecoverSpeed;

	cACTOR_FACTOR_TYPE1	m_regenHPPerSecond;				//	매초 체력을 회복
	cACTOR_FACTOR_TYPE6	m_detectTrap;
	cACTOR_FACTOR_TYPE6	m_detectSecretObject;
//┃특수 능력
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치
	int					m_iWeaponMinDamage;				//	무기 최소 데미지
	int					m_iWeaponMaxDamage;				//	무기 최대 데미지
	WORD				m_wAttackSpeed,m_wAttackRange;

	WORD				m_wCorrectFinalDefensivePowerPercentageTime;
	short				m_sCorrectFinalDefensivePowerPercentage;

	cACTOR_FACTOR_TYPE3	m_attackPower;					//	공격력
	cACTOR_FACTOR_TYPE4	m_defensePower;					//	방어력

	cACTOR_FACTOR_TYPE1	m_level;
	cACTOR_FACTOR_TYPE2	m_maxHP;						//	최대 HP
	cACTOR_FACTOR_TYPE2	m_maxCP;						//	최대 CP

	cACTOR_FACTOR_TYPE1	m_weaponPower;
	cACTOR_FACTOR_TYPE1	m_armorPower;

	cACTOR_FACTOR_TYPE1	m_strength;						//	힘 
	cACTOR_FACTOR_TYPE1	m_agility;						//	민첩성
	cACTOR_FACTOR_TYPE1	m_constitution;					//	건강
	cACTOR_FACTOR_TYPE1	m_intelligence;					//	지식
	cACTOR_FACTOR_TYPE1	m_wisdom;						//	지혜
	cACTOR_FACTOR_TYPE1	m_charisma;						//	카리스마
	cACTOR_FACTOR_TYPE1	m_luck;							//	운

	cACTOR_FACTOR_TYPE1	m_actionSpeed;					//	액션 속도 보정
	cACTOR_FACTOR_TYPE1	m_moveSpeed;					//	이동 속도 보정

	cACTOR_FACTOR_TYPE1	m_hitChance;					//	명중률 보정
	cACTOR_FACTOR_TYPE1	m_dodgeChance;					//	회피 보정
	cACTOR_FACTOR_TYPE1	m_concentration;
	cACTOR_FACTOR_TYPE6	m_concentrationByStellarPeer;
	cACTOR_FACTOR_TYPE1	m_bonusItemDropChance;						//	경험치 보너스
	cACTOR_FACTOR_TYPE1	m_bonusExp;
	WORD				m_wBottomCP;					//	한계 CP 뛰기,(정지/걷기/앉기)등 CP가 떨어지는 액션에서도 CP가 떨어지지 않는다.
//┃능력치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태 이상
	WORD				m_wBlindTime;						//	장님 상태 남은 시간
	short				m_sPoisonDamagePerSec,m_wPoisonTime;//	초당 데미지/10 , 중독 상태 남은 시간
	WORD				m_wSleepTime;						//	잠이 깰때까지 남은 시간
	WORD				m_wFreezeTime;						//	프리즈 상태 남은 시간
	WORD				m_wColdTime;						//	콜드 상태 남은 시간
	WORD				m_wStunTime;						//	스턴 상태 남은 시간
	WORD				m_wStoneTime;						//	석화 남은 시간
	WORD				m_wConfuseTime;						//	혼란 상태 남은 시간
	WORD				m_wCharmedTime;						//	차밍 상태 남은 시간
//┃상태 이상
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃저주
	WORD				m_wHwabyungTime,m_wRemainImpulseTime;
	WORD				m_wAnimatePartnerTime,m_wPlotOfShadowTime;

	WORD				m_wNotificationOfDeathCounter;
	WORD				m_wFalloffMoraleTime;			//	사기 저하 상태 남은 시간
	WORD				m_wTargetedTime;				//	타겟 설정 상태 남은 시간
	WORD				m_wBreakedArmorTime;			//	갑옷 파괴 상태 남은 시간
	WORD				m_wBreakedWeaponTime;			//	무기 파괴 상태 남은 시간
	WORD				m_wSimpletonTime;				//	바보-_- 상태 남은 시간
	WORD				m_wBerserkTime;					//	버서크 상태 남은 시간
	WORD				m_wTortureRate,m_wTortureTime;	//	고문률-_-??,고문 시간
	WORD				m_wBloodDonorRate,m_wBloodDonorTime;	//	피흡혈률,피흡혈 당하는 시간
//┃저주
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공격이 성공하면 부가적으로 들어가는 효과
	BYTE				m_bInstanceKillChance;			//	즉사 시킬 확률

	BYTE				m_bTargetFleeChance;			//	대상을 도망가게 만들 확률
	BYTE				m_bTargetBerserkChance;			//	대상을 버서크로 만들 확률
	BYTE				m_bTargetArmorBreakChance;		//	대상을 갑옷 파괴 상태로 만들 확률
	BYTE				m_bTargetWeaponBreakChance;		//	대상을 무기 파괴 상태로 만들 확률
	BYTE				m_bTargetConfuseChance;			//	대상을 혼란 시킨다.
	BYTE				m_bTargetCharmingChance;		//	대상을 유혹한다.
	BYTE				m_bTargetStunChance;			//	대상을 경직 시킨다.
	BYTE				m_bTargetColdChance;			//	대상을 콜드 상태로 만든다.
	BYTE				m_bTargetFreezeChance;			//	대상을 얼린다.
	BYTE				m_bTargetBlindChance;			//	대상을 장님으로 만들 확률
	BYTE				m_bTargetStoneChance;			//	대상을 돌로 만들 확률
	BYTE				m_bTargetSleepChance;			//	대상을 재울 확률
	BYTE				m_bTargetFirstTargetChance;		//	대상을 퍼스트 타겟으로 만들 확률
	BYTE				m_bTargetSimpltoneChance;		//	대상을 AI저하 상태로 만들 확률

	BYTE				m_bAddPoisonMinDamage,m_bAddPoisonMaxDamage;	//	추가로 대지 데미지를 입힌다.

	BYTE				m_bAddDamageToUndead;			//	언데드에게 추가 데미지를 준다.
	BYTE				m_bAddDamageToDemon;			//	악마에게 추가 데미지를 준다.
	BYTE				m_bAddDamageToHuman;			//	인간에게 추가 데미지를 준다.
	BYTE				m_bAddDamageToAnimal;			//	동물에게 추가 데미지를 준다.
	BYTE				m_bAddDamageToHolyAnimal;		//	신수에게 추가 데미지를 준다.

	WORD				m_wTargetBerserkTime;			//	대상을 버서크로 만드는 시간
	WORD				m_wTargetArmorBreakTime;		//	대상을 갑옷 파괴 상태로 만들 확률
	WORD				m_wTargetWeaponBreakTime;		//	대상을 무기 파괴 상태로 만들 확률
	WORD				m_wTargetConfuseTime;			//	대상을 혼란 시킨다.
	WORD				m_wTargetCharmingTime;			//	대상을 유혹한다.
	WORD				m_wTargetStunTime;				//	대상을 경직 시킨다.
	WORD				m_wTargetColdTime;				//	대상을 콜드 상태로 만든다.
	WORD				m_wTargetFreezeTime;			//	대상을 얼린다.
	WORD				m_wTargetBlindTime;				//	대상을 중독 시킨다.
	WORD				m_wTargetStoneTime;				//	대상을 돌로 만들 확률
	WORD				m_wTargetSleepTime;				//	대상을 재울 확률
	WORD				m_wTargetFirstTargetTime;		//	대상을 퍼스트 타겟으로 만들 확률
	WORD				m_wTargetSimpltoneTime;			//	대상을 AI저하 상태로 만들 확률

	WORD				m_wTargetDeclineHitChanceTime;	//	대상을 명중률 저하 상태에 빠지게 한다.
	WORD				m_wTargetDeclineDodgeChanceTime;//	대상을 회피율 저하 상태에 빠지게 한다.
	WORD				m_wTargetCurseAmplificationTime;//	대상을 회피율 저주 증폭 상태에 빠지게 한다.

	WORD				m_wTargetPoisonedTime;			//	대상을 중독 시킨다.
	WORD				m_wLifeStealPecentage;			//	흡혈률

	cACTOR_FACTOR_TYPE5	m_addFireDamage;
	cACTOR_FACTOR_TYPE5	m_addWaterDamage;
	cACTOR_FACTOR_TYPE5	m_addWindDamage;
	cACTOR_FACTOR_TYPE5	m_addLightDamage;
	cACTOR_FACTOR_TYPE5	m_addDarkDamage;
	cACTOR_FACTOR_TYPE5	m_addEarthDamage;

	WORD				m_wAddFireMinDamage,m_wAddFireMaxDamage;	//	추가로 불꽅 데미지를 입힌다.
	WORD				m_wAddWaterMinDamage,m_wAddWaterMaxDamage;	//	추가로 물 데미지를 입힌다.
	WORD				m_wAddWindMinDamage,m_wAddWindMaxDamage;	//	추가로 바람 데미지를 입힌다.
	WORD				m_wAddLightMinDamage,m_wAddLightMaxDamage;	//	추가로 빛 데미지를 입힌다.
	WORD				m_wAddDarkMinDamage,m_wAddDarkMaxDamage;	//	추가로 어둠 데미지를 입힌다.

	//vvv 09.08.10   마법추뎀 DX	 수정.
	sADD_NATURAL_DAMAGE_DX	m_sAddNaturalDamageDX[eADD_DAMAGE_DX_END];	// 추가.
	
	inline	int			getAddFireMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_FIRE_DAMAGE_DX].wMin+m_wAddFireMinDamage+m_addFireDamage.getCorrectedValue();	}
	inline	int			getAddWaterMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_WATER_DAMAGE_DX].wMin+m_wAddWaterMinDamage+m_addWaterDamage.getCorrectedValue();	}
	inline	int			getAddWindMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_WIND_DAMAGE_DX].wMin+m_wAddWindMinDamage+m_addWindDamage.getCorrectedValue();	}
	inline	int			getAddEarthMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_EARTH_DAMAGE_DX].wMin+m_addEarthDamage.getCorrectedValue();	}
	inline	int			getAddDarkMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_DARK_DAMAGE_DX].wMin+m_wAddDarkMinDamage+m_addDarkDamage.getCorrectedValue();	}
	inline	int			getAddLightMinDamage()	{		return	m_sAddNaturalDamageDX[eADD_LIGHT_DAMAGE_DX].wMin+m_wAddLightMinDamage+m_addLightDamage.getCorrectedValue();	}

	inline	int			getAddFireMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_FIRE_DAMAGE_DX].wMax+m_wAddFireMaxDamage+m_addFireDamage.getCorrectedValue();	}
	inline	int			getAddWaterMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_WATER_DAMAGE_DX].wMax+m_wAddWaterMaxDamage+m_addWaterDamage.getCorrectedValue();	}
	inline	int			getAddWindMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_WIND_DAMAGE_DX].wMax+m_wAddWindMaxDamage+m_addWindDamage.getCorrectedValue();	}
	inline	int			getAddEarthMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_EARTH_DAMAGE_DX].wMax+m_addEarthDamage.getCorrectedValue();	}
	inline	int			getAddDarkMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_DARK_DAMAGE_DX].wMax+m_wAddDarkMaxDamage+m_addDarkDamage.getCorrectedValue();	}
	inline	int			getAddLightMaxDamage()	{		return	m_sAddNaturalDamageDX[eADD_LIGHT_DAMAGE_DX].wMax+m_wAddLightMaxDamage+m_addLightDamage.getCorrectedValue();	}	
	
	//AAA 09.08.10   마법추뎀 DX

//┃공격이 성공하면 부가적으로 들어가는 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃데미지를 체력으로 흡수하거나 카운터 날림
	BYTE				m_bFirstAidPercentage;			//	입은 데미지의 일부를 치료한다.

	BYTE				m_bAbsorbFireDamage;			//	불꽃 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbWaterDamage;			//	물 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbWindDamage;			//	바람 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbEarthDamage;			//	대지 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbLightDamage;			//	빛 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbDarkDamage;			//	어둠 데미지를 흡수해서 체력으로 변환
	BYTE				m_bAbsorbMagicDamage;			//	모든 마법 데미지를 흡수해서 체력으로 변환

	cACTOR_FACTOR_TYPE1	m_returnDamageRate;			//	데미지의 일부를 공격자에게 돌려준다.
	BYTE				m_bDamageToCPRate;				//	데미지의 일부를 CP로 변환해 준다.
	BYTE				m_bReturnFreezeChance;			//	리턴 프리즈 확률
	BYTE				m_bReturnColdChance;			//	리턴 콜드 확률

	WORD				m_wReturnFreezeTime;			//	반격!! 프리즈가 성공 했을때 프리즈 되는 시간.
	WORD				m_wReturnColdTime;				//	반격!! 콜드가 성공 했을때 프리즈 되는 시간.

	cACTOR_FACTOR_TYPE1	m_aCorrectLevel[5];
	cACTOR_FACTOR_TYPE1	m_correctSummonBeastState;
	cACTOR_FACTOR_TYPE1	m_bonusPetExp;
//┃데미지를 체력으로 흡수하거나 카운터 날림
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 수치 보정
	cACTOR_FACTOR_TYPE1	m_correctAllSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctRightPunchSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctLeftPunchSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctRightKickSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctLeftKickSkillLevel;
	cACTOR_FACTOR_TYPE6	m_correctDownKickSkillLevel;

	int					m_iCorrectRecoverBlocking;		//	블럭킹 회복 시간 보정
	BYTE				m_bCorrectRareItemDropRate;		//	레어 아이템이 떨어질 확률을 보정한다.
	BYTE				m_bCorrectUniqueItemDropRate;	//	유니크 아이템이 떨어질 확률을 보정한다.
	BYTE				m_bCorrectCriticalHitChance;	//	크리티컬 히트 날릴 기회를 보정한다.
	BYTE				m_bCorrectCrushBlowChance;		//	크러쉬 블로우 날릴 기회를 보정한다.
	BYTE				m_bCorrectKnockoutAttackChance;	//	녹아웃 공격을 시킬 기회를 보정한다.
	BYTE				m_bCorrectReloadSpeed;			//	벨트 채우는 속도 보정
	BYTE				m_bCorrectTransformationSpeed;	//	변신 속도 보정
	BYTE				m_bCorrectHPRecoverSpeed;		//	체력 회복 속도 보정

	BYTE				m_bCorrectFieldFirePenelty;
	BYTE				m_bCorrectFieldWaterPenelty;
	BYTE				m_bCorrectFieldWindPenelty;
	BYTE				m_bCorrectFieldEarthPenelty;
	BYTE				m_bCorrectFieldLightPenelty;
	BYTE				m_bCorrectFieldDarkPenelty;

	cACTOR_FACTOR_TYPE1	m_blockingChance;

	cACTOR_FACTOR_TYPE1	m_weakningTargetFireResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetWaterResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetWindResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetEarthResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetLightResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetDarkResistance;
	cACTOR_FACTOR_TYPE1	m_weakningTargetMagicResistance;

	cACTOR_FACTOR_TYPE1	m_strengtheningFireDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningWaterDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningWindDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningEarthDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningDarkDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningLightDamage;
	cACTOR_FACTOR_TYPE1	m_strengtheningMagicDamage;
//┃각종 수치 보정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 저항
	BYTE				m_bFreezeResistance;		//	상태 이상 저항
	BYTE				m_bColdResistance;			//	상태 이상 저항
	BYTE				m_bBlindResistance;			//	상태 이상 저항
	BYTE				m_bStunResistance;			//	상태 이상 저항
	BYTE				m_bConfuseResistance;		//	상태 이상 저항
	BYTE				m_bCharmingResistance;		//	상태 이상 저항
	BYTE				m_bStoneResistance;			//	상태 이상 저항
	BYTE				m_bPoisonResistance;		//	상태 이상 저항
	BYTE				m_bSleepResistance;			//	상태 이상 저항

	BYTE				m_bCriticalHitResistance;			//	크리티컬 저항
	BYTE				m_bCrushBlowResistance;				//	크러쉬 블로우 저항
	BYTE				m_bKnockoutResistance;			//	녹아웃 저항
	BYTE				m_bInstanceKillResistance;		//	즉사 저항

	cACTOR_FACTOR_TYPE1	m_strangeStatusResistance;
	cACTOR_FACTOR_TYPE1	m_curseResistance;
	cACTOR_FACTOR_TYPE1	m_declinePowerResistance;

	cACTOR_FACTOR_TYPE1	m_fireResistance;				//	불꽃 저항
	cACTOR_FACTOR_TYPE1	m_waterResistance;				//	물 저항
	cACTOR_FACTOR_TYPE1	m_windResistance;				//	바람 저항
	cACTOR_FACTOR_TYPE1	m_earthResistance;				//	대지 저항
	cACTOR_FACTOR_TYPE1	m_lightResistance;				//	빛 저항
	cACTOR_FACTOR_TYPE1	m_darkResistance;				//	어둠 저항
	cACTOR_FACTOR_TYPE1	m_allMagicResistance;			//	모든 마법 저항
//┃보정된 마법 저항
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타
	WORD				m_wResurrectionCount;			//	부활 카운트
	WORD				m_wCurseAmplificationTime;		//	저주 증폭 상태
	WORD				m_wImpulseTime;
	int					m_iRecoverHP;					//	회복될 HP
	int					m_iForcedRecoverHP;
	WORD				m_wBeltStackCount;				//	벨트에 들어가는 아이템 수
	int					m_iSaveExperience;						//	레벨업 전의 경험치를 세이브 하는곳.
	int					m_iCorrectMinimumDamage,m_iCorrectMaximumDamage;
	WORD				m_wBlackScreenCount , m_wTVScreenCount;			//	화면 암전 효과 시간.

//┃기타
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태
	WORD				m_wConsensusTime;		// 교감상태.
	WORD				m_wMadnessTime;			// 광란.
	// 언어구속
	WORD	m_wBindingWordsTime, m_wBindingWordsOverLimitLevel;
	//도망
	WORD	m_wFleeTime; 
	//  리액션스킬사용금지..
	WORD	m_wNotUseReactionSkillTime;	
	// 투기
	cFightEnergy	m_fightEnergy;
	// 투기
	WORD			m_wRushTime;		// 돌진시간.

	WORD			m_aPassiveSkill[dPASSIVE_SKILL_COUNT];	//	패시브 스킬..
	WORD			m_wFightingSpiritTime;		// 투지
	WORD			m_wIgnoreNeedStateTime;		// 필요상태시간
	int				m_iCarvingOptionMaxHP;	
	// 즉시힐효과
	WORD			m_wInstanceHealEffectTime;			//	시간
	WORD			m_wInstanceHealEffectPercent;		// 효과퍼센트
//┃상태
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	inline	void		resetEquipmentData()
	{
	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃특수 능력
		if (m_sInvisivilityTime	==	-1)	m_sInvisivilityTime	=	0;	//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
		if (m_sFloatTime		==	-1)	m_sFloatTime		=	0;	//	공중부양 시간(-이면 장비에 붙어 있는거다.)
		if (m_sBlurTime			==	-1)	m_sBlurTime			=	0;	//	흐릿한 상태 시간(-이면 장비에 붙어 있는거다.)
		if (m_sNonTargetTime	==	-1)	m_sNonTargetTime	=	0;	//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)

		m_regenHPPerSecond.resetEquipmentValue();
		m_level.resetEquipmentValue();

		m_bResurrectionChance				=	0;			//	죽었을때 일정 확률로 부활한다.
		m_bIsInfinityBullet					=	FALSE;		//	탄환 리젠 주기
		m_isAutoReload						=	FALSE;		//	자동으로 벨트의 아이템을 리로드 한다.
		m_isMagicBullet						=	FALSE;		//	마법 탄환을 쏠 수 있는 상태
		m_isIgnoreTargetCorrectDodgeChance	=	FALSE;		//	적의 회피 보정치 무시
		m_isIgnoreTargetCorrectHitChance	=	FALSE;		//	적의 명중 보정치 무시
		m_isPerfectHit						=	FALSE;					//	항상 명중
		m_bGetBonusCPPercentage				=	FALSE;		//	CP획득시 보너스..
		m_wIncreaseSitdownHPRecoverSpeed	=	0;
	//┃특수 능력
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃능력치
		m_iWeaponMinDamage	=	0;
		m_iWeaponMaxDamage	=	0;

		m_attackPower.resetPrefixData();
		m_defensePower.resetPrefixData();
		m_maxHP.resetPrefixData();
		m_maxCP.resetPrefixData();

		m_weaponPower.resetEquipmentValue();
		m_armorPower.resetEquipmentValue();

		m_strength.resetEquipmentValue();			//	힘 
		m_agility.resetEquipmentValue();			//	민첩성
		m_constitution.resetEquipmentValue();		//	건강
		m_intelligence.resetEquipmentValue();		//	지식
		m_wisdom.resetEquipmentValue();				//	지혜
		m_charisma.resetEquipmentValue();			//	카리스마
		m_luck.resetEquipmentValue();				//	운

		m_actionSpeed.resetEquipmentValue();		//	액션 속도 보정
		m_moveSpeed.resetEquipmentValue();			//	이동 속도 보정

		m_hitChance.resetEquipmentValue();			//	명중률 보정
		m_dodgeChance.resetEquipmentValue();		//	회피 보정
		m_concentration.resetEquipmentValue();
		m_bonusItemDropChance.resetEquipmentValue();						//	경험치 보너스
		m_bonusExp.resetEquipmentValue();

		m_wBottomCP			=	0;					//	한계 CP 뛰기,(정지/걷기/앉기)등 CP가 떨어지는 액션에서도 CP가 떨어지지 않는다.
	//┃능력치
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃공격이 성공하면 부가적으로 들어가는 효과
		m_bInstanceKillChance		=	0;			//	즉사 시킬 확률

		m_bCorrectFieldFirePenelty	=	0;
		m_bCorrectFieldWaterPenelty	=	0;
		m_bCorrectFieldWindPenelty	=	0;
		m_bCorrectFieldEarthPenelty	=	0;
		m_bCorrectFieldLightPenelty	=	0;
		m_bCorrectFieldDarkPenelty	=	0;

		m_bTargetFleeChance			=	0;			//	대상을 도망가게 만들 확률
		m_bTargetBerserkChance		=	0;			//	대상을 버서크로 만들 확률
		m_bTargetArmorBreakChance	=	0;		//	대상을 갑옷 파괴 상태로 만들 확률
		m_bTargetWeaponBreakChance	=	0;		//	대상을 무기 파괴 상태로 만들 확률
		m_bTargetConfuseChance		=	0;			//	대상을 혼란 시킨다.
		m_bTargetCharmingChance		=	0;		//	대상을 유혹한다.
		m_bTargetStunChance			=	0;			//	대상을 경직 시킨다.
		m_bTargetColdChance			=	0;			//	대상을 콜드 상태로 만든다.
		m_bTargetFreezeChance		=	0;			//	대상을 얼린다.
		m_bTargetBlindChance		=	0;			//	대상을 장님으로 만들 확률
		m_bTargetStoneChance		=	0;			//	대상을 돌로 만들 확률
		m_bTargetSleepChance		=	0;			//	대상을 재울 확률
		m_bTargetFirstTargetChance	=	0;		//	대상을 퍼스트 타겟으로 만들 확률
		m_bTargetSimpltoneChance	=	0;		//	대상을 AI저하 상태로 만들 확률

		m_bAddPoisonMinDamage	=	0;
		m_bAddPoisonMaxDamage	=	0;	//	추가로 대지 데미지를 입힌다.

		m_bAddDamageToUndead	=	0;			//	언데드에게 추가 데미지를 준다.
		m_bAddDamageToDemon		=	0;			//	악마에게 추가 데미지를 준다.
		m_bAddDamageToHuman		=	0;			//	인간에게 추가 데미지를 준다.
		m_bAddDamageToAnimal	=	0;			//	동물에게 추가 데미지를 준다.
		m_bAddDamageToHolyAnimal=	0;		//	신수에게 추가 데미지를 준다.

		m_wTargetBerserkTime	=	0;			//	대상을 버서크로 만드는 시간
		m_wTargetArmorBreakTime	=	0;		//	대상을 갑옷 파괴 상태로 만들 확률
		m_wTargetWeaponBreakTime=	0;		//	대상을 무기 파괴 상태로 만들 확률
		m_wTargetConfuseTime	=	0;			//	대상을 혼란 시킨다.
		m_wTargetCharmingTime	=	0;			//	대상을 유혹한다.
		m_wTargetStunTime		=	0;				//	대상을 경직 시킨다.
		m_wTargetColdTime		=	0;				//	대상을 콜드 상태로 만든다.
		m_wTargetFreezeTime		=	0;			//	대상을 얼린다.
		m_wTargetBlindTime		=	0;				//	대상을 중독 시킨다.
		m_wTargetStoneTime		=	0;				//	대상을 돌로 만들 확률
		m_wTargetSleepTime		=	0;				//	대상을 재울 확률
		m_wTargetFirstTargetTime	=	0;		//	대상을 퍼스트 타겟으로 만들 확률
		m_wTargetSimpltoneTime		=	0;			//	대상을 AI저하 상태로 만들 확률

		m_wTargetDeclineHitChanceTime	=	0;	//	대상을 명중률 저하 상태에 빠지게 한다.
		m_wTargetDeclineDodgeChanceTime	=	0;//	대상을 회피율 저하 상태에 빠지게 한다.
		m_wTargetCurseAmplificationTime	=	0;//	대상을 회피율 저주 증폭 상태에 빠지게 한다.

		m_wTargetPoisonedTime	=	0;			//	대상을 중독 시킨다.
		m_wLifeStealPecentage	=	0;			//	흡혈률

		m_wAddFireMinDamage		=	0;
		m_wAddFireMaxDamage		=	0;	//	추가로 불꽅 데미지를 입힌다.
		m_wAddWaterMinDamage	=	0;
		m_wAddWaterMaxDamage	=	0;	//	추가로 물 데미지를 입힌다.
		m_wAddWindMinDamage		=	0;
		m_wAddWindMaxDamage		=	0;	//	추가로 바람 데미지를 입힌다.
		m_wAddLightMinDamage	=	0;
		m_wAddLightMaxDamage	=	0;	//	추가로 빛 데미지를 입힌다.
		m_wAddDarkMinDamage		=	0;
		m_wAddDarkMaxDamage		=	0;	//	추가로 어둠 데미지를 입힌다.
		//vvv 09.08.10   마법추뎀 DX
		memset(m_sAddNaturalDamageDX,0,sizeof(sADD_NATURAL_DAMAGE_DX)*eADD_DAMAGE_DX_END);
		//AAA 09.08.10   마법추뎀 DX

	//┃공격이 성공하면 부가적으로 들어가는 효과
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃데미지를 체력으로 흡수하거나 카운터 날림
		m_bFirstAidPercentage	=	0;			//	입은 데미지의 일부를 치료한다.

		m_bAbsorbFireDamage		=	0;			//	불꽃 데미지를 흡수해서 체력으로 변환
		m_bAbsorbWaterDamage	=	0;			//	물 데미지를 흡수해서 체력으로 변환
		m_bAbsorbWindDamage		=	0;			//	바람 데미지를 흡수해서 체력으로 변환
		m_bAbsorbEarthDamage	=	0;			//	대지 데미지를 흡수해서 체력으로 변환
		m_bAbsorbLightDamage	=	0;			//	빛 데미지를 흡수해서 체력으로 변환
		m_bAbsorbDarkDamage		=	0;			//	어둠 데미지를 흡수해서 체력으로 변환
		m_bAbsorbMagicDamage	=	0;			//	모든 마법 데미지를 흡수해서 체력으로 변환

		m_returnDamageRate.resetEquipmentValue();			//	데미지의 일부를 공격자에게 돌려준다.
		m_bDamageToCPRate		=	0;				//	데미지의 일부를 CP로 변환해 준다.
		m_bReturnFreezeChance	=	0;			//	리턴 프리즈 확률
		m_bReturnColdChance		=	0;			//	리턴 콜드 확률

		m_wReturnFreezeTime		=	0;			//	반격!! 프리즈가 성공 했을때 프리즈 되는 시간.
		m_wReturnColdTime		=	0;				//	반격!! 콜드가 성공 했을때 프리즈 되는 시간.

		m_aCorrectLevel[0].resetEquipmentValue();
		m_aCorrectLevel[1].resetEquipmentValue();
		m_aCorrectLevel[2].resetEquipmentValue();
		m_aCorrectLevel[3].resetEquipmentValue();
		m_aCorrectLevel[4].resetEquipmentValue();
		m_correctSummonBeastState.resetEquipmentValue();
		m_bonusPetExp.resetEquipmentValue();

	//┃데미지를 체력으로 흡수하거나 카운터 날림
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃각종 수치 보정
		m_correctAllSkillLevel.resetEquipmentValue();

		m_iCorrectRecoverBlocking		=	0;		//	블럭킹 회복 시간 보정
		m_bCorrectRareItemDropRate		=	0;		//	레어 아이템이 떨어질 확률을 보정한다.
		m_bCorrectUniqueItemDropRate	=	0;	//	유니크 아이템이 떨어질 확률을 보정한다.
		m_bCorrectCriticalHitChance		=	0;	//	크리티컬 히트 날릴 기회를 보정한다.
		m_bCorrectCrushBlowChance		=	0;		//	크러쉬 블로우 날릴 기회를 보정한다.
		m_bCorrectKnockoutAttackChance	=	0;	//	녹아웃 공격을 시킬 기회를 보정한다.
		m_bCorrectReloadSpeed			=	0;			//	벨트 채우는 속도 보정
		m_bCorrectTransformationSpeed	=	0;	//	변신 속도 보정
		m_bCorrectHPRecoverSpeed		=	0;		//	체력 회복 속도 보정
		m_blockingChance.resetEquipmentValue();				//	블럭킹률

		m_weakningTargetFireResistance.resetEquipmentValue();
		m_weakningTargetWaterResistance.resetEquipmentValue();
		m_weakningTargetWindResistance.resetEquipmentValue();
		m_weakningTargetEarthResistance.resetEquipmentValue();
		m_weakningTargetLightResistance.resetEquipmentValue();
		m_weakningTargetDarkResistance.resetEquipmentValue();
		m_weakningTargetMagicResistance.resetEquipmentValue();

		m_strengtheningFireDamage.resetEquipmentValue();
		m_strengtheningWaterDamage.resetEquipmentValue();
		m_strengtheningWindDamage.resetEquipmentValue();
		m_strengtheningEarthDamage.resetEquipmentValue();
		m_strengtheningDarkDamage.resetEquipmentValue();
		m_strengtheningLightDamage.resetEquipmentValue();
		m_strengtheningMagicDamage.resetEquipmentValue();

	//┃각종 수치 보정
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃각종 저항
		m_bFreezeResistance		=	0;		//	상태 이상 저항
		m_bColdResistance		=	0;			//	상태 이상 저항
		m_bBlindResistance		=	0;			//	상태 이상 저항
		m_bStunResistance		=	0;			//	상태 이상 저항
		m_bConfuseResistance	=	0;		//	상태 이상 저항
		m_bCharmingResistance	=	0;		//	상태 이상 저항
		m_bStoneResistance		=	0;			//	상태 이상 저항
		m_bPoisonResistance		=	0;		//	상태 이상 저항
		m_bSleepResistance		=	0;			//	상태 이상 저항

		m_bCriticalHitResistance	=	0;			//	크리티컬 저항
		m_bCrushBlowResistance		=	0;				//	크러쉬 블로우 저항
		m_bKnockoutResistance		=	0;			//	녹아웃 저항
		m_bInstanceKillResistance	=	0;		//	즉사 저항

		m_strangeStatusResistance.resetEquipmentValue();//
		m_declinePowerResistance.resetEquipmentValue();//
		m_curseResistance.resetEquipmentValue();//

		m_fireResistance.resetEquipmentValue();//	"불저항"
		m_earthResistance.resetEquipmentValue();//	"대지저항"
		m_windResistance.resetEquipmentValue();//	"바람저항"
		m_waterResistance.resetEquipmentValue();//	"물저항"
		m_lightResistance.resetEquipmentValue();//	"빛저항"
		m_darkResistance.resetEquipmentValue();//	"어둠저항"
		m_allMagicResistance.resetEquipmentValue();//	"마법 속성 공격에 대한 저항"
	//┃보정된 마법 저항
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃기타
		m_wBeltStackCount			=	0;				//	벨트에 들어가는 아이템 수
		m_iCorrectMinimumDamage		=	0;
		m_iCorrectMaximumDamage		=	0;
	//┃기타
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		
		
		//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//┃상태
		m_iCarvingOptionMaxHP		=	0;
		//┃상태
		//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	}

	inline	void		reset()
	{
		memset(this,0,sizeof(cPLAYER_STATUS));
	}

	inline	char*		getStatusPointer(){return	(char*)this;}

};

class	CPlayerDeathPeneltyInfo
{
public:
	int		m_iBeginLevel,m_iEndLevel;
	int		m_iPeneltySecond,m_iPeneltyValue;
	int		m_iRecoverPriceValue;
};

extern	CPlayerDeathPeneltyInfo	g_aPlayerDeathPeneltyInfo[20];

#endif