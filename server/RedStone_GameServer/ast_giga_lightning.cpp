#include	"ast_giga_lightning.h"
#include	"cACTOR.h"
#include	"cFIELD.h"
#include	"debugCode.h"
#include	"cGame.h"

BOOL
cAS_GigaLightning::fire()
{
	m_wTrapSkill			=	m_lpSkill->m_aValue[0];

	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wCastingTime			=	m_lpSkill->m_aValue[1]*10;
	m_wSerchTime			=	m_lpSkill->m_aValue[2]*10;
	m_wWarningTime			=	m_lpSkill->m_aValue[3]*10;
	int	iRange				=	m_lpSkill->m_aValue[6];

	m_dwBookedActionTime	=	g_dwCurrentTime	+m_wCastingTime/dREVISE_ACTION_SPEED_FOR_TEST;

	if	(m_lpSkill->m_aValue[5])
		m_lpCaster->m_lpField->addSendEtcWork(m_lpCaster->m_wSerialInField,eEW_CHANGE_SAGE_BOOK_COLOR,m_lpCaster->m_wSerialInField,m_lpSkill->m_aValue[5]);
	
	{
		cFIELD		*lpField	=	m_lpCaster->m_lpField;
		cAbility	trapAbility;

		trapAbility.set(m_wTrapSkill,m_ability.m_wLevel);

		m_wTrapRange			=	trapAbility.getHitRange();

		int	iFitRegularPattern	=	0xffff;
		int	iMoreFarDistance	=	0;
		int	iClientFPS			=	64;

		m_wFinishStep			=	iClientFPS*m_wSerchTime/1000;
		m_wSerchTime			+=	200;	//	0.2초 더해준다.
		int	iBeginPattern		=	random(g_regularRandom.m_iPatternCount);
		int	iDistance;

		for	(int iSerchPlace=g_regularRandom.m_iPatternCount;iSerchPlace;)	//	100번정도 뒤져서 빈자리를 찾아 본다.
		{
			--iSerchPlace;

			int	iRegularPattern	=	(iSerchPlace+iBeginPattern)%g_regularRandom.m_iPatternCount;

			m_spLine.setRegularRandomPattern(iRegularPattern);
			m_spLine.init(m_lpCaster->m_pos.x-iRange,m_lpCaster->m_pos.y-iRange,m_lpCaster->m_pos.x+iRange,m_lpCaster->m_pos.y+iRange,0.02f);

			for	(int i=m_wFinishStep;i;)
			{
				--i;
				m_spLine.update();
			}

			if	(m_lpCaster->m_lpField->isNotCrashPlaceBySpecificSkillTrap(m_wTrapSkill,m_wTrapRange*2,m_spLine.m_pos.x,m_spLine.m_pos.y,&iDistance))
			{
				m_posTarget.x	=	m_spLine.m_pos.x;
				m_posTarget.y	=	m_spLine.m_pos.y;
				m_wRandomPattern=	iRegularPattern;
				break;
			}

			if	(iDistance	>	iMoreFarDistance)
			{
				m_posTarget.x	=	m_spLine.m_pos.x;
				m_posTarget.y	=	m_spLine.m_pos.y;

				m_wRandomPattern=	iRegularPattern;
				iMoreFarDistance=	iDistance;
			}
		}

		m_lpCaster->m_lpField->isNotCrashPlaceBySpecificSkillTrap(m_wTrapSkill,m_wTrapRange*2,m_posTarget.x,m_posTarget.y,&iDistance);
	}

	m_wGrade	=	eStep_ReadyMakeCrossTarget;

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
cAS_GigaLightning::casting()
{
	cAbility	trapAbility;
	cFIELD		*lpField		=	m_lpCaster->m_lpField;

	trapAbility.set(m_wTrapSkill,m_ability.m_wLevel);

	int			iTrapEE_Index	=	trapAbility.getSkill()->getExtraEffectIndexByEffect(eSKILL_EE_SET_TRAP);

	if	(iTrapEE_Index	==	0xffff)
		return	FALSE;

	CSkillExtraEffectInfo	skillEE;

	trapAbility.getExtraEffect(iTrapEE_Index,&skillEE);

	int		iUpkeepTime		=	skillEE.m_iUpkeepTime/100;
	int		iSkillLevel		=	trapAbility.m_wLevel;
	int		iRange			=	(skillEE.m_aiValue[0]+skillEE.m_aiValue[1]*iSkillLevel)/100;
	int		iDisarmLevel	=	(skillEE.m_aiValue[2]+skillEE.m_aiValue[3]*iSkillLevel)/100;
	int		iDetectLevel	=	(skillEE.m_aiValue[4]+skillEE.m_aiValue[5]*iSkillLevel)/100;

	iDisarmLevel	=	min(iDisarmLevel,100);
	iDetectLevel	=	min(iDetectLevel,100);

	if	(iUpkeepTime		==	0)
		iUpkeepTime			=	trapAbility.getUpkeepTime();

	cTrap	trap;

	int		iLimitTrapCount	=	trapAbility.getShotCount();
	int		iExistTrapCount	=	lpField->getExistTrapCount(m_lpCaster,trapAbility.m_wSkill);

	if	(lpField->isTrapFull()	||	iExistTrapCount	>=	iLimitTrapCount)
		return	FALSE;

	trap.build(m_lpCaster,&trapAbility,m_posTarget.x,m_posTarget.y,iRange,iDetectLevel,iDisarmLevel,iUpkeepTime);

	int	iAddTrap		=	g_pTrapManager->addTrap(&trap);

	if	(iAddTrap		==	0xffff)
		return	TRUE;

	lpField->addTrap(&trap);

	return	TRUE;
}

BOOL
cAS_GigaLightning::update()
{
	if	(!isActionTime())
		return	FALSE;

	if(!m_lpCaster)
		return FALSE;

	m_lpCaster->setRemainActionTime(1);

	if	(m_wGrade	==	eStep_ReadyMakeCrossTarget)
	{
		m_dwBookedActionTime	=	g_dwCurrentTime	+m_wSerchTime/dREVISE_ACTION_SPEED_FOR_TEST;

		m_wGrade	=	eStep_SerchRandomLocation;

		m_lpCaster->m_lpField->addSendEtcWork(0xffff,eEW_BEGIN_SERCH_RANDOM_PLACE_BY_GIGA_LIGHTNING,
					m_ability.m_wSkill,m_lpCaster->m_wSerialInField,m_wRandomPattern,m_wFinishStep,m_wTrapRange);
		return	FALSE;
	}
	if	(m_wGrade	==	eStep_SerchRandomLocation)
	{
		m_dwBookedActionTime	=	g_dwCurrentTime	+m_wWarningTime/dREVISE_ACTION_SPEED_FOR_TEST;

		m_wGrade	=	eStep_LockOn;

		return	FALSE;
	}

	if	(m_wGrade	==	eStep_LockOn)
	{
		m_wGrade	=	eStep_Fire;
		m_lpCaster->m_lpField->addSendEtcWork(0xffff,eEW_FIRE_GIGA_LIGHTNING,m_ability.m_wSkill,m_lpCaster->m_wSerialInField);

		casting();

		return	TRUE;
	}

	return	TRUE;
}
