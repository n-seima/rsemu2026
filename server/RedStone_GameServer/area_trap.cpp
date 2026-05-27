#include "cArea.h"
#include "cField.h"
#include "arca.h"
#include "cGame.h"

void
cAreaTrapInfo::initTrap()
{
	m_wIsActive				=	TRUE;
	m_dwBookedRegenTime		=	0;

	m_awTrapSkill[0]		=	m_wTrapSkill1;
	m_awTrapSkill[1]		=	m_wTrapSkill2;

	m_awTrapActiveChance[0]	=	m_wTrap1ActiveChance;
	m_awTrapActiveChance[1]	=	m_wTrap2ActiveChance;

	if (m_wTrap1ActiveChance)
	{
		if (random(100) < m_wTrap1ActiveChance)
			m_awTrapActiveChance[0]	=	100;
		else
			m_awTrapActiveChance[0]	=	0xffff;
	}
	if (m_wTrap2ActiveChance)
	{
		if (random(100) < m_wTrap2ActiveChance)
			m_awTrapActiveChance[1]	=	100;
		else
			m_awTrapActiveChance[1]	=	0xffff;
	}

	if	(m_awTrapActiveChance[0]	==	0xffff	&&	m_awTrapActiveChance[1]	==	0xffff)
		disarm();
}

void
cAreaTrapInfo::disarm(int _iUpkeepTime)
{
	m_wIsActive			=	FALSE;

	_iUpkeepTime		=	max(_iUpkeepTime,m_wTrapRefreshTime);

	if	(_iUpkeepTime	==	0xffff)
		m_dwBookedRegenTime	=	0xffffffff;
	else
		m_dwBookedRegenTime	=	g_dwCurrentTime+_iUpkeepTime*1000;
}

void
cAreaTrapInfo::operate1SecondWork()
{
	if (m_wIsActive	)
		return;

	if (m_dwBookedRegenTime	==	0xffffffff	||	g_dwCurrentTime	<	m_dwBookedRegenTime)
		return;

	if	(!m_lpField)
		return;

	initTrap();

	m_lpField->addSendFieldTrapWork(m_wSerial,eFTW_ACTIVE);
}

void
cAreaTrapInfo::explosionTrap(cACTOR *_lpActor)
{
	if	(random(100)	>=	_lpActor->getTrapEvadeCahnceByTitle())
		for (int i=0;i<2;i++)
		{
			cTrapSkill*	lpTrap	=	NULL;

			if (m_awTrapActiveChance[i]	==	0)
				lpTrap	=	g_trapSkillManager.getSkill(m_awTrapSkill[i]);
			else
			{
				if (random(100) < m_awTrapActiveChance[i])
					lpTrap	=	g_trapSkillManager.getRandomSkill(i);
			}

			if	(!lpTrap)
				continue;

			_lpActor->m_lpField->explosionTrapSkill(_lpActor,lpTrap,m_wTrapTarget,m_wTrapLevel);
		}

	disarm();
	m_lpField->addSendFieldTrapWork(m_wSerial,eFTW_DISACTIVE);
}

void
cAreaTrapInfo::tryDisarm(cACTOR *_lpActor)
{
	cRECT	*lpRect	=	(cRECT *)&x1;

	int		iRange	=	lpRect->getRange(_lpActor->m_pos.x,_lpActor->m_pos.y,TRUE);

	if (m_wIsActive	==	FALSE)
	{
		_lpActor->sendFieldTrapWork(m_wSerial,eFTW_FAILED_BY_IS_DISACTIVE);

		return;
	}

	if (iRange	>	dRANGE_OF_DISARM_ABLE_TRAP*dRANGE_OF_DISARM_ABLE_TRAP)
	{
		_lpActor->sendFieldTrapWork(m_wSerial,eFTW_FAILED_DISARM_BY_RANGE);

		return;
	}

	if (!_lpActor->isVisibleTrap(m_wDetectTrapLevel))
	{
		_lpActor->sendFieldTrapWork(m_wSerial,eFTW_FAILED_BY_IS_CAN_NOT_DETECT);

		return;
	}

	cAbility	*lpDisarmSkill	=	_lpActor->getDisarmTrapAbility();

	if (!lpDisarmSkill)
	{
		_lpActor->sendFieldTrapWork(m_wSerial,eFTW_FAILED_BY_NOT_USE_ABLE_SKILL);

		return;
	}

	int	iAR		=	_lpActor->actionToObject(lpDisarmSkill,eTARGET_OBJECT_FIELD_TRAP,m_wSerial);

	if	(iAR	==	eAR_OK)
	{
		_lpActor->sendFieldTrapWork(m_wSerial,eFTW_TRY_DISARM);
	}
	else
	if	(iAR	==	eAR_LOW_CP)
	{
		_lpActor->sendFieldTrapWork(m_wSerial,eFTW_TRY_DISARM_FAILED_BY_IS_LOW_CP);
	}
	else
	{
		_lpActor->sendFieldTrapWork(m_wSerial,eFTW_TRY_DISARM_FAILED_BY_BUSY);
	}
}