#ifndef _actorDefine_h
#define	_actorDefine_h

#include "SFC.H"

#define	dMAX_ENCHANTED_MAGIC_COUNT_FOR_UPKEEP	10
#define	dMONSTER_ANIMATE_PARTNER_LIMIT_COUNT	3

#define	dPVP_GUILD_BATTLE_FIELD_MOVE_SPEED_FACTOR	80
#define	dPVP_DUEL_BATTLE_FIELD_MOVE_SPEED_FACTOR	100

#define	dPVP_FREEZE_TO_COLD_FACTOR				5
#define	dPVP_FREEZE_TO_COLD_FACTOR_NE			10
#define	dPVP_POISON_DAMAGE_FACTOR				10
#define	dPVP_POISON_DAMAGE_FACTOR_NE			20
#define	dPVP_POISON_TIME_FACTOR					50
#define	dPVP_ABSORB_DAMAGE_FACTOR				10
#define	dPVP_DAMAGE_FACTOR						10
#define	dPVP_DAMAGE_FACTOR_NE					20
#define	dPVP_CORRECT_STONE_TIME					25
#define	dPVP_CORRECT_STONE_TIME_NE				50
#define	dPVP_CORRECT_STUN_TIME					25
#define	dPVP_CORRECT_STUN_TIME_NE				50

#define	dPVP_TOP_STUN_CHANCE					50
#define	dPVP_TOP_STONE_CHANCE					50

#define	dPVP_INSTANCE_KILL_FACTOR				12
#define	dPVP_INSTANCE_KILL_FACTOR_NE			25
#define	dPVP_CORRECT_INSTANCE_KILL_CAHNCE		25
#define	dPVP_CORRECT_INSTANCE_KILL_CAHNCE_NE	50
#define	dPVP_DOUBLE_CRITICAL_DAMAGE_FACTOR		400
#define	dPVP_DOUBLE_CRITICAL_DAMAGE_FACTOR_NE	500
#define	dPVP_CRITICAL_DAMAGE_FACTOR				200
#define	dPVP_CRITICAL_DAMAGE_FACTOR_NE			250
#define	dPVP_CORRECT_SPEED_BONUS				25
#define	dPVP_DUEL_CORRECT_SPEED_BONUS			50

#define	dPVP_HARD_BLOW_DAMAGE_FACTOR			300
#define	dPVP_HARD_BLOW_DAMAGE_FACTOR_NE			375

#define	dPVP_CRUSH_BLOW_FACTOR					24
#define	dPVP_CRUSH_BLOW_FACTOR_NE				18
#define	dDUEL_CRUSH_BLOW_FACTOR					12
#define	dDUEL_CRUSH_BLOW_FACTOR_NE				9

#define	dPVP_CORRECT_CRUSH_BLOW_CAHNCE			50
#define	dPVP_CORRECT_CRUSH_BLOW_CAHNCE_NE		50
#define	dPVP_BONUS_CP_FACTOR					10
#define	dPVP_BONUS_CP_FACTOR_NE					20
#define	dCRITICAL_DAMAGE_FACTOR					2
#define	dDOUBLE_CRITICAL_DAMAGE_FACTOR			4
#define	dHARD_BLOW_DAMAGE_FACTOR				3
#define dPET_LOYALTY_MAX						2000	// 09.09.22 충성도 최대치.
class	cEnchantedMagicInfoForUpkeep
{
public:
	WORD	m_wSkill,m_wLevel,m_wRemainTime;

			cEnchantedMagicInfoForUpkeep()
			{
				m_wSkill		=	0xffff;
				m_wRemainTime	=	0;
			}
};

//
//	가장 기본적인 스타일
class	cACTOR_FACTOR_TYPE1_FOR_UPKEEP
{
public:
	short	m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	WORD	m_wTime;		//	유지 시간
	WORD	m_bf1IsCanNotApplySameEffect	:	1;	//1	중복불가.
	WORD	:0;
};


class	cACTOR_FACTOR_TYPE1
{
public:
	short			m_sValue;		//	장비한 상태에서의 수치
	short			m_sFixValue;

	short			m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	WORD			m_wTime;		//	유지 시간
	WORD			m_bf1IsCanNotApplySameEffect	:	1;	//1	중복불가.
	WORD			:0;
	void			setUpkeepInfo(cACTOR_FACTOR_TYPE1_FOR_UPKEEP *_lpInfo);
	void			getUpkeepInfo(cACTOR_FACTOR_TYPE1_FOR_UPKEEP *_lpInfo);
	
	inline	BOOL	isWrong()
	{
		if (m_sCorrectValue				<	0)
			return	TRUE;					//	더하거나 빼야 하는 수치
		
		return	FALSE;
	}

	inline	BOOL	isGood()
	{
		if (m_sCorrectValue				>	0)	return	TRUE;					//	더하거나 빼야 하는 수치

		return	FALSE;
	}

	inline	void	cure()
	{
		if (m_sCorrectValue	<	0)
		{
			m_sCorrectValue	=	0;
			m_wTime			=	0;
		}
	}
	inline	void	reduceWrongTime(int _iTime)
	{
		int	iTime;

		if (m_sCorrectValue	<	0)
		{
			iTime				=	m_wTime	-	_iTime;
			m_wTime				=	max(iTime,0);
		}
	}

	inline	void	dispell()
	{
		if (m_sCorrectValue	>	0)
		{
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
			m_sCorrectValue					=	0;
			m_bf1IsCanNotApplySameEffect	=	FALSE;
			return	TRUE;
		}
		return	FALSE;
	}

	inline	void	reset()
	{
		memset(this,0,sizeof(cACTOR_FACTOR_TYPE1));

		m_sFixValue		=	0x7fff;
	}

	inline	void	resetAidData()
	{
		m_sCorrectValue	=	0;	//	더하거나 빼야 하는 수치
		m_wTime			=	0;	//	유지 시간
		m_bf1IsCanNotApplySameEffect	=	FALSE;
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
	
		_iTime	=	max(_iTime,1);
		
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
			if (_iTime	>	m_wTime)	m_wTime	=	_iTime;
		}
		else
		if (m_sCorrectValue	<	_iValue)
		{
			m_sCorrectValue	=	_iValue;
			m_wTime			=	_iTime;
		}
	}

	inline	BOOL	isFixedValue()
	{
		if (m_sFixValue == 0x7fff)	
			return FALSE;

		return	TRUE;
	}

	inline	int		getCorrectedValue()	
	{
		if (m_sFixValue != 0x7fff)	return m_sFixValue;

		return	m_sValue+m_sCorrectValue;
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

class	cACTOR_FACTOR_TYPE2_FOR_UPKEEP
{
public:
	short			m_sCorrectValue;					//	더하거나 빼야 하는 수치
	short			m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
	WORD			m_wTimeForCorrectValue;				//	초
	WORD			m_wTimeForCorrectPercentageValue;	//	초
	WORD			m_bf1IsCanNotApplySameEffect		:	1;	//1	중복불가.	
	WORD			m_bf1IsCanNotApplySameEffectPercent	:	1;	//2	중복불가.
	WORD			:0;
};	

//
//	HP,CP
class	cACTOR_FACTOR_TYPE2
{
public:
	int				m_iValue;							//	장비한 상태에서의 수치

	short			m_sCorrectValueByPrefix;			//	더하거나 빼야 하는 수치
	short			m_sCorrectPercentageValueByPrefix;	//	더하거나 빼야 하는 수치(퍼센티지)

	short			m_sCorrectValue;					//	더하거나 빼야 하는 수치
	short			m_sCorrectPercentageValue;			//	더하거나 빼야 하는 수치(퍼센티지)
	WORD			m_wTimeForCorrectValue;				//	초
	WORD			m_wTimeForCorrectPercentageValue;	//	초
	WORD			m_bf1IsCanNotApplySameEffect		:	1;	//1	중복불가.	
	WORD			m_bf1IsCanNotApplySameEffectPercent	:	1;	//2	중복불가.
	WORD			:0;
	void			setUpkeepInfo(cACTOR_FACTOR_TYPE2_FOR_UPKEEP *_lpInfo);
	void			getUpkeepInfo(cACTOR_FACTOR_TYPE2_FOR_UPKEEP *_lpInfo);

	inline	BOOL	isWrong()
	{
		if (m_sCorrectValue				<	0)	return	TRUE;					//	더하거나 빼야 하는 수치
		if (m_sCorrectPercentageValue	<	0)	return	TRUE;			//	더하거나 빼야 하는 수치(퍼센티지)

		return	FALSE;
	}

	inline	BOOL	isGood()
	{
		if (m_sCorrectValue				>	0)	return	TRUE;					//	더하거나 빼야 하는 수치
		if (m_sCorrectPercentageValue	>	0)	return	TRUE;			//	더하거나 빼야 하는 수치(퍼센티지)

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
	inline	void	reduceWrongTime(int _iTime)
	{
		int	iTime;

		if (m_sCorrectValue	<	0)
		{
			iTime							=	m_wTimeForCorrectValue	-	_iTime;
			m_wTimeForCorrectValue			=	max(iTime,0);
		}

		if (m_sCorrectPercentageValue	<	0)
		{
			iTime							=	m_wTimeForCorrectPercentageValue	-	_iTime;
			m_wTimeForCorrectPercentageValue=	max(iTime,0);
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
		m_sCorrectValue						=	0;	//	더하거나 빼야 하는 수치
		m_sCorrectPercentageValue			=	0;	//	더하거나 빼야 하는 수치
		m_wTimeForCorrectValue				=	0;		//	유지 시간
		m_wTimeForCorrectPercentageValue	=	0;		//	유지 시간
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
		

		_iTime	=	max(_iTime,1);


		if (_iValue	<	0)
		{
			if (m_sCorrectPercentageValue	>	0)
				m_sCorrectPercentageValue	=	0,m_wTimeForCorrectPercentageValue	=	0;
			else
			if (m_sCorrectPercentageValue	<	0)
			{
				if (-m_sCorrectPercentageValue<	-_iValue)
					m_sCorrectPercentageValue		=	_iValue,m_wTimeForCorrectPercentageValue	=	_iTime;
				else
				if (m_sCorrectPercentageValue	==	_iValue)
					m_wTimeForCorrectPercentageValue	=	max(m_wTimeForCorrectPercentageValue,_iTime);
			}
			else
				m_sCorrectPercentageValue		=	_iValue,m_wTimeForCorrectPercentageValue	=	_iTime;
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
		
		_iTime	=	max(_iTime,1);

		if (_iValue	<	0)
		{
			if (m_sCorrectValue	>	0)
				m_sCorrectValue	=	0,m_wTimeForCorrectValue	=	0;
			else
			if (m_sCorrectValue	<	0)
			{
				if (-m_sCorrectValue<	-_iValue)
					m_sCorrectValue		=	_iValue,m_wTimeForCorrectValue	=	_iTime;
				else
				if (m_sCorrectValue	==	_iValue)
					m_wTimeForCorrectValue	=	max(m_wTimeForCorrectValue,_iTime);
			}
			else
				m_sCorrectValue		=	_iValue,m_wTimeForCorrectValue	=	_iTime;
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

class	cACTOR_FACTOR_TYPE3_FOR_UPKEEP
{
public:
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
	void			setUpkeepInfo(cACTOR_FACTOR_TYPE3_FOR_UPKEEP *_lpInfo);
	void			getUpkeepInfo(cACTOR_FACTOR_TYPE3_FOR_UPKEEP *_lpInfo);

	inline	int		getCorrectValue()			{return	m_sCorrectValueByPrefix+m_sCorrectValue;}
	inline	int		getCorrectPercentageValue()	{return	m_sCorrectPercentageValueByPrefix+m_sCorrectPercentageValue+m_sCorrectPercentageValueForPetCommand;}

	inline	BOOL	isWrong()
	{
		if (m_sCorrectValue				<	0)	return	TRUE;	//	더하거나 빼야 하는 수치
		if (m_sCorrectPercentageValue	<	0)	return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)
		if (m_sCorrectPercentageValueForPetCommand<	0)	return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)
		
		return	FALSE;
	}
	inline	BOOL	isGood()
	{
		if (m_sCorrectValue				>	0)	return	TRUE;	//	더하거나 빼야 하는 수치
		if (m_sCorrectPercentageValue	>	0)	return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)
		if (m_sCorrectPercentageValueForPetCommand>	0)	return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)

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

	inline	void	reduceWrongTime(int _iTime)
	{
		int	iTime;

		if (m_sCorrectValue	<	0)
		{
			iTime											=	m_wTimeForCorrectValue	-	_iTime;
			m_wTimeForCorrectValue							=	max(iTime,0);
		}

		if (m_sCorrectPercentageValue	<	0)
		{
			iTime											=	m_wTimeForCorrectPercentageValue	-	_iTime;
			m_wTimeForCorrectPercentageValue				=	max(iTime,0);
		}

		if (m_sCorrectPercentageValueForPetCommand	<	0)
		{
			iTime											=	m_wTimeForCorrectPercentageValueForPetCommand	-	_iTime;
			m_wTimeForCorrectPercentageValueForPetCommand	=	max(iTime,0);
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

		_iTime	=	max(_iTime,1);

		if (_iValue	<	0)
		{
			if (m_sCorrectValue	>	0)
				m_sCorrectValue	=	0,m_wTimeForCorrectValue	=	0;
			else
			if (m_sCorrectValue	<	0)
			{
				if (-m_sCorrectValue<	-_iValue)
					m_sCorrectValue		=	_iValue,m_wTimeForCorrectValue	=	_iTime;
				else
				if (m_sCorrectValue	==	_iValue)
					m_wTimeForCorrectValue	=	max(m_wTimeForCorrectValue,_iTime);
			}
			else
				m_sCorrectValue		=	_iValue,m_wTimeForCorrectValue	=	_iTime;
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

		_iTime	=	max(_iTime,1);

		if (_iValue	<	0)
		{
			if (m_sCorrectPercentageValue	>	0)
				m_sCorrectPercentageValue	=	0,m_wTimeForCorrectPercentageValue	=	0;
			else
			if (m_sCorrectPercentageValue	<	0)
			{
				if (-m_sCorrectPercentageValue<	-_iValue)
					m_sCorrectPercentageValue		=	_iValue,m_wTimeForCorrectPercentageValue	=	_iTime;
				else
				if (m_sCorrectPercentageValue	==	_iValue)
					m_wTimeForCorrectPercentageValue	=	max(m_wTimeForCorrectPercentageValue,_iTime);
			}
			else
				m_sCorrectPercentageValue		=	_iValue,m_wTimeForCorrectPercentageValue	=	_iTime;
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
			if (_iTime	>	m_wTimeForCorrectPercentageValue)
				m_wTimeForCorrectPercentageValue	=	_iTime;
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


		_iTime	=	max(_iTime,1);

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

	inline	void	resetForPetCommandValue()
	{
		m_sCorrectPercentageValueForPetCommand			=	0;
		m_wTimeForCorrectPercentageValueForPetCommand	=	0;
		m_bf1IsCanNotApplySameEffectPetCommand			=	FALSE;
	}
};

class	cACTOR_FACTOR_TYPE4_FOR_UPKEEP
{
public:
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

	void			setUpkeepInfo(cACTOR_FACTOR_TYPE4_FOR_UPKEEP *_lpInfo);
	void			getUpkeepInfo(cACTOR_FACTOR_TYPE4_FOR_UPKEEP *_lpInfo);

	inline	BOOL	isWrong()
	{
		if (m_wTimeForCorrectValue	&&	m_sCorrectValue				<	0)
			return	TRUE;	//	더하거나 빼야 하는 수치
		if (m_wTimeForCorrectPercentageValue	&&	m_sCorrectPercentageValue	<	0)
			return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)
		if (m_wTimeForCorrectPercentageValueForPetCommand	&&	m_sCorrectPercentageValueForPetCommand	<	0)
			return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)

		return	FALSE;
	}

	inline	BOOL	isGood()
	{
		if (m_wTimeForCorrectValue	&&	m_sCorrectValue				>	0)
			return	TRUE;	//	더하거나 빼야 하는 수치
		if (m_wTimeForCorrectPercentageValue	&&	m_sCorrectPercentageValue	>	0)
			return	TRUE;	//	더하거나 빼야 하는 수치(퍼센티지)
		if (m_wTimeForCorrectPercentageValueForPetCommand	&&	m_sCorrectPercentageValueForPetCommand	>	0)
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
	inline	void	reduceWrongTime(int _iTime)
	{
		int	iTime;

		if (m_sCorrectValue	<	0)
		{
			iTime							=	m_wTimeForCorrectValue	-	_iTime;
			m_wTimeForCorrectValue			=	max(iTime,0);
		}

		if (m_sCorrectPercentageValue	<	0)
		{
			iTime							=	m_wTimeForCorrectPercentageValue	-	_iTime;
			m_wTimeForCorrectPercentageValue=	max(iTime,0);
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


		_iTime	=	max(_iTime,1);

		if (_iValue	<	0)
		{
			if (m_sCorrectValue	>	0)
				m_sCorrectValue	=	0,m_wTimeForCorrectValue	=	0;
			else
			if (m_sCorrectValue	<	0)
			{
				if (-m_sCorrectValue<	-_iValue)
					m_sCorrectValue		=	_iValue,m_wTimeForCorrectValue	=	_iTime;
				else
				if (m_sCorrectValue	==	_iValue)
					m_wTimeForCorrectValue	=	max(m_wTimeForCorrectValue,_iTime);
			}
			else
				m_sCorrectValue		=	_iValue,m_wTimeForCorrectValue	=	_iTime;
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

		_iTime	=	max(_iTime,1);

		if	(_iValue	<	0)
		{
			if (m_sCorrectPercentageValue	>	0)
				m_sCorrectPercentageValue	=	0,m_wTimeForCorrectPercentageValue	=	0;
			else
			if (m_sCorrectPercentageValue	<	0)
			{
				if (-m_sCorrectPercentageValue<	-_iValue)
					m_sCorrectPercentageValue		=	_iValue,m_wTimeForCorrectPercentageValue	=	_iTime;
				else
				if (m_sCorrectPercentageValue	==	_iValue)
					m_wTimeForCorrectPercentageValue	=	max(m_wTimeForCorrectPercentageValue,_iTime);
			}
			else
				m_sCorrectPercentageValue		=	_iValue,m_wTimeForCorrectPercentageValue	=	_iTime;
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

		_iTime	=	max(_iTime,1);

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
	
	inline	void		resetForPetCommandValue()
	{
		m_sCorrectPercentageValueForPetCommand			=	0;
		m_wTimeForCorrectPercentageValueForPetCommand	=	0;
		m_bf1IsCanNotApplySameEffectPetCommand			=	FALSE;
	}
};

class	cACTOR_FACTOR_TYPE5_FOR_UPKEEP
{
public:
	short			m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	short			m_sRange;
	WORD			m_wTime;		//	유지 시간
	WORD			m_bf1IsCanNotApplySameEffect	:	1;	//	1	중복불가
	WORD			:0;
};

class	cACTOR_FACTOR_TYPE5	//	디폴트 수치가 0이다.
{
public:
	short			m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	short			m_sRange;
	WORD			m_wTime;		//	유지 시간
	WORD			m_bf1IsCanNotApplySameEffect	:	1;	//	1	중복불가
	WORD			:0;

	void			setUpkeepInfo(cACTOR_FACTOR_TYPE5_FOR_UPKEEP *_lpInfo);
	void			getUpkeepInfo(cACTOR_FACTOR_TYPE5_FOR_UPKEEP *_lpInfo);

	inline	BOOL	isWrong()
	{
		if (m_sCorrectValue				<	0)
			return	TRUE;					//	더하거나 빼야 하는 수치

		return	FALSE;
	}

	inline	BOOL	isGood()
	{
		if (m_sCorrectValue				>	0)
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
	inline	void	reduceWrongTime(int _iTime)
	{
		int	iTime;

		if (m_sCorrectValue	<	0)
		{
			iTime		=	m_wTime	-	_iTime;
			m_wTime		=	max(iTime,0);
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


		_iTime	=	max(_iTime,1);
		
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

class	cACTOR_GGG_FOR_UPKEEP
{
public:
	WORD				m_wGGGTimeForSkillUp;			//GGG	=	GreatGodGrace 
	
	WORD				m_wGGGValue1ForSkillUp;
	WORD				m_wGGGValue2ForSkillUp;
	WORD				m_wGGGAttackCoolTime;
	WORD				m_wGGGDefenceCoolTime;
};

class	cACTOR_FACTOR_TYPE6_FOR_UPKEEP	//	디폴트 수치가 0이다.
{
public:
	short			m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	WORD			m_wTime;		//	유지 시간
	WORD			m_bf1IsCanNotApplySameEffect	:	1;	//	1	중복불가
	WORD			:0;
};

class	cACTOR_FACTOR_TYPE6	//	디폴트 수치가 0이다.
{
public:
	short			m_sCorrectValue;//	일시적으로 더하거나 빼야 하는 수치
	WORD			m_wTime;		//	유지 시간
	WORD			m_bf1IsCanNotApplySameEffect	:	1;	//	1	중복불가
	WORD			:0;

	void			setUpkeepInfo(cACTOR_FACTOR_TYPE6_FOR_UPKEEP *_lpInfo);
	void			getUpkeepInfo(cACTOR_FACTOR_TYPE6_FOR_UPKEEP *_lpInfo);

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
			m_sCorrectValue	=	0;
			m_wTime			=	0;
		}
	}
	inline	void	reduceWrongTime(int _iTime)
	{
		int	iTime;

		if (m_sCorrectValue	<	0)
		{
			iTime		=	m_wTime	-	_iTime;
			m_wTime		=	max(iTime,0);
		}
	}

	inline	void	dispell()
	{
		if (m_sCorrectValue	>	0)
		{
			m_sCorrectValue	=	0;
			m_wTime			=	0;
		}
	}

	inline	BOOL	decreaseTime()	//	시간 줄여~~
	{
		if (m_wTime	==	0)
			return FALSE;

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

		_iTime	=	max(_iTime,1);

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
		}
	}

	inline	int		getCorrectedValue()		{return	m_sCorrectValue;}
};


class	cActorStatusUpkeep
{
public:
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃특수 능력
	short							m_sInvisivilityTime;			//	인비저빌러티시간(-이면 장비에 붙어 있는거다.)
	short							m_sFloatTime;					//	공중부양 시간(-이면 장비에 붙어 있는거다.)
	short							m_sBlurTime;					//	흐릿한 상태 시간(-이면 장비에 붙어 있는거다.)
	short							m_sNonTargetTime;				//	타겟에서 제외됨(-1이면 장비에 붙어 있는 것임)

	WORD							m_wReversalPowerTime;			//	지식 <-> 힘, 지혜 <-> 민첩성 변환 시간
	WORD							m_wMaintainMaxCPTime;			//	이 시간동안 CP가 max로 유지된다.
	WORD							m_wMaintainMaxWeponDamageTime;	//	이 시간동안 무기의 데미지가 항상 max가 나온다.
	int								m_iFirstAidHP;					//	응급처치한 HP
	WORD							m_wFirstAidTime;				//	응급처치한 시간
	WORD							m_wBeastBerserkerTime;
	WORD							m_wInvincibleTime;				//	무적 시간
	WORD							m_wSealRunningTime;
	WORD							m_wSealActionTime;
	WORD							m_wUnsealAction;
	WORD							m_wJumppingSkippingTime;
	WORD							m_wRabbitTime;
//┃특수 능력
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치
	cACTOR_FACTOR_TYPE3_FOR_UPKEEP	m_attackPower;					//	공격력
	cACTOR_FACTOR_TYPE4_FOR_UPKEEP	m_defensePower;					//	방어력

	cACTOR_FACTOR_TYPE2_FOR_UPKEEP	m_maxHP;						//	최대 HP
	cACTOR_FACTOR_TYPE2_FOR_UPKEEP	m_maxCP;						//	최대 CP

	cACTOR_FACTOR_TYPE2_FOR_UPKEEP	m_weaponPower;
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_armorPower;

	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_level;						//	레벨
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_strength;						//	힘 
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_agility;						//	민첩성
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_constitution;					//	건강
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_intelligence;					//	지식
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_wisdom;						//	지혜
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_charisma;						//	카리스마
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_luck;							//	운

	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_actionSpeed;					//	액션 속도 보정
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_moveSpeed;					//	이동 속도 보정
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_regenHPPerSecond;				//	체력 회복

	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_hitChance;					//	명중률 보정
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_dodgeChance;					//	회피 보정
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_concentration;
	cACTOR_FACTOR_TYPE6_FOR_UPKEEP	m_concentrationByStellarPeer;
//┃능력치
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃능력치 저하
	WORD							m_wReduceAllResistanceToHalfTime;
//┃능력치 저하
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태 이상
	WORD							m_wBlindTime;						//	장님 상태 남은 시간
	short							m_wPoisonTime;						//	초당 데미지/10 , 중독 상태 남은 시간
	int								m_iPoisonDamagePerSec;
	WORD							m_wColdTime;						//	콜드 상태 남은 시간
	WORD							m_wConfuseTime;						//	혼란 상태 남은 시간
//┃상태 이상
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃저주
	WORD							m_wFalloffMoraleTime;			//	사기 저하 상태 남은 시간
	WORD							m_wTargetedTime;				//	타겟 설정 상태 남은 시간
	WORD							m_wBreakedArmorTime;			//	갑옷 파괴 상태 남은 시간
	WORD							m_wBreakedWeaponTime;			//	무기 파괴 상태 남은 시간
	WORD							m_wSimpletonTime;				//	바보-_- 상태 남은 시간
	WORD							m_wBerserkTime;					//	버서크 상태 남은 시간
//┃저주
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	cACTOR_FACTOR_TYPE5_FOR_UPKEEP	m_addFireDamage;
	cACTOR_FACTOR_TYPE5_FOR_UPKEEP	m_addWaterDamage;
	cACTOR_FACTOR_TYPE5_FOR_UPKEEP	m_addWindDamage;
	cACTOR_FACTOR_TYPE5_FOR_UPKEEP	m_addLightDamage;
	cACTOR_FACTOR_TYPE5_FOR_UPKEEP	m_addDarkDamage;
	cACTOR_FACTOR_TYPE5_FOR_UPKEEP	m_addEarthDamage;

//┃공격이 성공하면 부가적으로 들어가는 효과
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 저항
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_stunResistance;
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_strangeStatusResistance;
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_curseResistance;
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_declinePowerResistance;

	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_fireResistance;				//	불꽃 저항
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_waterResistance;				//	물 저항
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_windResistance;				//	바람 저항
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_earthResistance;				//	대지 저항
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_lightResistance;				//	빛 저항
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_darkResistance;				//	어둠 저항
	cACTOR_FACTOR_TYPE1_FOR_UPKEEP	m_allMagicResistance;			//	모든 마법 저항
//┃보정된 마법 저항
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃기타
	WORD							m_wCurseAmplificationTime;		//	저주 증폭 상태

	cEnchantedMagicInfoForUpkeep	m_aEnchantMagic[dMAX_ENCHANTED_MAGIC_COUNT_FOR_UPKEEP];

	void							reset()
	{
		memset(this,0,sizeof(cActorStatusUpkeep));
		memset(m_aEnchantMagic,0xff,sizeof(m_aEnchantMagic));
	}
};

#endif