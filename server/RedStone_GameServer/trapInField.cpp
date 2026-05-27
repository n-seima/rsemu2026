#include "CField.h"
#include "CExceptionHandler.h"
#include "debugCode.h"

BOOL
cFIELD::isTrapFull()
{
	if	(m_bIsReadyToClose	||	m_wTrapCount	>=	dMAX_TRAP_COUNT_IN_FIELD)
		return	TRUE;

	return	FALSE;
}

cTrap*
cFIELD::getTrap(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>=	dMAX_TRAP_COUNT)
		return	NULL;

	return	g_pTrapManager->get(_iIndex);
}

BOOL
cFIELD::isNotCrashPlaceBySpecificSkillTrap(int _iSkill,int _iRange,int _iX,int _iY,int *_lpiMinRange)
{
	if	(_lpiMinRange)
		*_lpiMinRange	=	0x7fffffff;

	for (int i=m_wTrapCount;i;)
	{
		--i;

		int		iTrapSerial	=	m_awTrapList[i];
		cTrap	*lpTrap		=	g_pTrapManager->get(iTrapSerial);

		if	(!lpTrap)
			continue;

		cAbility	*lpAbility	=	lpTrap->getAbility();

		if	(!lpAbility	||	lpAbility->m_wSkill!=	_iSkill)
			continue;

		int		iRange		=	GetOvalRange(lpTrap->m_pos.x,lpTrap->m_pos.y,_iX,_iY);

		iRange	=	(int)sqrt(iRange);

		if	(_lpiMinRange)
			if	(iRange	<	*_lpiMinRange)
				*_lpiMinRange	=	iRange;
	}

	if	(*_lpiMinRange	<	_iRange)
		return	FALSE;

	return	TRUE;
}

BOOL
cFIELD::getNotCrashPlaceForTrap(cPOINT *_lpPos,int _iSkill,int _iRange)
{
	for(int iLoop=1024;iLoop;)
	{
		--iLoop;

		_lpPos->x	=	random(m_iPixelWidth-_iRange)+_iRange;
		_lpPos->y	=	random(m_iPixelHeight-_iRange)+_iRange;

		for (int i=m_wTrapCount;i;)
		{
			--i;

			int		iTrapSerial	=	m_awTrapList[i];
			cTrap	*lpTrap		=	g_pTrapManager->get(iTrapSerial);

			if	(!lpTrap)
				continue;

			cAbility	*lpAbility	=	lpTrap->getAbility();

			if	(lpAbility->m_wSkill!=	_iSkill)
				continue;

			int		iRange		=	GetOvalRange(lpTrap->m_pos.x,lpTrap->m_pos.y,_lpPos->x,_lpPos->y);

			if	(iRange	<	_iRange*_iRange)
			{
				_lpPos->x	=	0;
				break;
			}
		}

		if	(_lpPos->x)
			return	TRUE;
	}

	return	FALSE;
}

//	함정 추가
BOOL
cFIELD::addTrap(cTrap *_lpTrap)
{
	if	(m_bIsReadyToClose	||	m_wTrapCount	>=	dMAX_TRAP_COUNT_IN_FIELD)
		return	FALSE;

	m_awTrapList[m_wTrapCount++]	=	_lpTrap->m_wSerial;
	_lpTrap->m_wLocalPart			=	getCurrentLocalPart(_lpTrap->m_pos.x,_lpTrap->m_pos.y);

	addSendAddTrap(_lpTrap);

	int	iTargetTeam	=	1;

	if	(_lpTrap->m_wTeam	>=	2)
		return	FALSE;
	
	if	(m_bIsGuildBattleField)
		iTargetTeam	=	1-_lpTrap->m_wTeam;

	if	(_lpTrap->m_wLocalPart	>=	m_iLocalPartitionCount)
		return	FALSE;
	
	for (int i=m_pLocalPartition[_lpTrap->m_wLocalPart].m_aiActorCount[iTargetTeam];i;)		//	적 캐릭터
	{
		--i;

		int		iSerial	=	m_pLocalPartition[_lpTrap->m_wLocalPart].m_apActorList[iTargetTeam][i];

		cACTOR	*lpActor=	getTestedActor(iSerial);

		if	(!lpActor || lpActor->isDeath())
			continue;
		if	(_lpTrap->isCaughtInTrap(lpActor->m_pos.x,lpActor->m_pos.y,lpActor->getBodySize())	==	FALSE)
			continue;

		lpActor->checkCaughtInTrap(_lpTrap);

		return	TRUE;
	}

	return	TRUE;
}

//	함정 제거
BOOL
cFIELD::removeTrap(int _iTrapSerial)
{
	g_pTrapManager->disarmTrap(_iTrapSerial);

	for (int i=0;i<m_wTrapCount;i++)
		if (m_awTrapList[i]	==	_iTrapSerial)
		{
			addSendRemoveTrap(m_awTrapList[i]);

			m_awTrapList[i]	=	m_awTrapList[m_wTrapCount-1];

			m_wTrapCount--;

			return	TRUE;
		}

	return	FALSE;
}

BOOL
cFIELD::disarmTrapsOwnedOutActor(int _iOutActor)	//	액터가 나간다.
{
	int		iDisarmedTrapCount	=	0,iCount=0;

	for (int i=0;i<m_wTrapCount;)
	{
		int		iTrapSerial	=	m_awTrapList[i];
		cTrap	*lpTrap		=	g_pTrapManager->get(iTrapSerial);

		if (lpTrap	==	NULL)
		{
			removeTrap(iTrapSerial);

			continue;
		}
		else
		if (lpTrap->m_wOwner==	_iOutActor)
		{
			removeTrap(iTrapSerial);

			continue;
		}

		i++;
	}

	return	FALSE;
}

void
cFIELD::updateTrap()
{
	for (int i=0;i<m_wTrapCount;)
	{
		int		iTrapSerial	=	m_awTrapList[i];
		cTrap	*lpTrap		=	g_pTrapManager->get(iTrapSerial);

		if	(lpTrap	==	NULL || m_iUniqueSerial != lpTrap->m_wFieldSerial)
		{
			removeTrap(iTrapSerial);

			continue;
		}

		switch(lpTrap->update())
		{
			case	eRUT_LIVE		:
				break;

			case	eRUT_END		:
				removeTrap(iTrapSerial);
				continue;

			case	eRUT_EXPLOSION	:
			{
				cACTOR	*lpOwner	=	getTestedActor(lpTrap->m_wOwner);

				if	(lpOwner	==	NULL)
				{
					lpTrap->m_wUpkeepTime	=	1;
					break;
				}

				explosionTrap(lpOwner,lpTrap,NULL);
				break;
			}
		}

		i++;
	}
}

int
cFIELD::getExistTrapCount(cACTOR *_lpCaster,int _iSkill)
{
	int	iCount	=	0;

	for (int i=0;i<m_wTrapCount;i++)
	{
		int		iTrapSerial	=	m_awTrapList[i];
		cTrap	*lpTrap		=	g_pTrapManager->get(iTrapSerial);

		if (lpTrap	==	NULL || lpTrap->m_wOwner != _lpCaster->m_wSerialInField	|| lpTrap->getAbility()->m_wSkill != _iSkill)
			continue;

		iCount++;
	}

	return	iCount;
}

cTrap*
cFIELD::getCaughtTrap(cACTOR *_lpActor)
{
	for (int i=0;i<m_wTrapCount;i++)
	{
		int		iTrapSerial	=	m_awTrapList[i];
		cTrap	*lpTrap		=	g_pTrapManager->get(iTrapSerial);

		if	(lpTrap	==	NULL || lpTrap->m_wLocalPart != _lpActor->m_iLocalPart || lpTrap->m_wTeam == _lpActor->m_wTeam)
			continue;

		if	(lpTrap->isCaughtInTrap(_lpActor->m_pos.x,_lpActor->m_pos.y,_lpActor->getBodySize()))
			return	lpTrap;
	}

	return	NULL;
}

//
//	함정이 터졌어요. -o-
BOOL
cFIELD::explosionTrap(cACTOR *_lpCaster,cTrap *_lpTrap,cACTOR *_lpTarget,int _iType)
{
	cAbility	*lpAbility	=	_lpTrap->getAbility();
	CSkill		*lpSkill	=	lpAbility->getSkill();
	int			iHitRange	=	lpAbility->getHitRange();
	int			iLocalPart	=	_lpTrap->m_wLocalPart;

	if	(iLocalPart	>=	m_iLocalPartitionCount	||	_lpTrap->m_wFieldSerial	!=	m_iUniqueSerial)
	{
		removeTrap(_lpTrap->m_wSerial);
		return	FALSE;
	}

	CHitInfo	hitInfo;
	cACTOR		*lpActor;
	int			i,iTeam;

	if	(lpSkill->isGoodExtraEffectSkill() && lpAbility->getHitRange())
		operateAuraSkill(lpAbility,_lpTrap->m_pos.x,_lpTrap->m_pos.y,lpAbility->getHitRange(),_lpCaster);

	SG_EXPLOSION_TRAP	packet;

	packet.bf12Skill		=	lpAbility->m_wSkill;
	packet.bf8Level			=	lpAbility->m_wLevel;
	packet.bf6TargetCount	=	0;
	packet.wPosX			=	_lpTrap->m_pos.x;
	packet.wPosY			=	_lpTrap->m_pos.y;
	packet.bf1IsFirst		=	TRUE;

	if	(_iType	==	eETT_DEFEND_ON_IMAGE)
		packet.bf1IsFirst	=	FALSE;

	packet.base.set(sizeof(packet),dSG_EXPLOSION_TRAP);

	if	(_iType	==	eETT_DEFEND_ON_IMAGE_FIRST_STEP)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData);

		addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
		removeTrap(_lpTrap->m_wSerial);

		return	TRUE;
	}

	if	(iHitRange	==	0)
	{
		if	(_lpTarget	&&	_lpTarget->m_wSerialInField	!=	0xffff)
		{
			_lpTarget->getTrapDamage(_lpCaster,_lpTrap,lpAbility,&hitInfo);
			_lpTarget->damagedByTrap(_lpCaster,&hitInfo,lpAbility,_lpTrap->m_pos.x,_lpTrap->m_pos.y);

			cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packet.aData[packet.bf6TargetCount];

			lpInfo->wActor					=	_lpTarget->m_wSerialInField;
			lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
			lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
			lpInfo->dwResultField			=	hitInfo.m_dwResultField;

			packet.bf6TargetCount++;
		}
	}
	else
	{
		for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
		{
			if	(iTeam == _lpTrap->m_wTeam)
				continue;

			for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
			{
				int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];

				lpActor	=	getTestedActor(iSerial);
				if	(!lpActor || lpActor->isDeath())
					continue;

				if	(!lpActor->isInAttackRange(_lpTrap->m_pos.x,_lpTrap->m_pos.y,iHitRange))
					continue;

				if	(isMissileBlockedLine(_lpTrap->m_pos.x,_lpTrap->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

				lpActor->getTrapDamage(_lpCaster,_lpTrap,lpAbility,&hitInfo);
				lpActor->damagedByTrap(_lpCaster,&hitInfo,lpAbility,_lpTrap->m_pos.x,_lpTrap->m_pos.y);

				if	(lpSkill->m_wDamageAttribute	==	0)
					hitInfo.setHit();

				cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packet.aData[packet.bf6TargetCount];

				lpInfo->wActor					=	lpActor->m_wSerialInField;
				lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
				lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
				lpInfo->dwResultField			=	hitInfo.m_dwResultField;

				packet.bf6TargetCount++;

				if	(packet.bf6TargetCount	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
				{
					addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
					packet.bf1IsFirst			=	FALSE;
					packet.bf6TargetCount		=	0;
				}
			}
		}
	}

	if	(lpAbility->getKnockBackDistance())
		thrustActor(_lpTrap->m_pos.x,_lpTrap->m_pos.y,_lpCaster,NULL,_lpCaster->m_wActorKind,lpAbility,iHitRange,lpAbility->getKnockBackDistance());

	if	(packet.bf6TargetCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+packet.bf6TargetCount*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
	}

	if	(_iType	==	eETT_NORMAL)
	{
		if	(_lpTrap->m_wRange)
			removeTrap(_lpTrap->m_wSerial);
	}

	return	TRUE;
}

void
cFIELD::explosionTrapSkill(cACTOR *_lpTarget,cTrapSkill *_lpTrap,int _iTargetMethod,int _iTrapLevel)
{
	int			iLocalPart	=	_lpTarget->m_iLocalPart;
	int			iHitRange	=	dWIDE_AREA_DAMAGE_TRAP_SKILL_RANGE;
	cPOINT		posExplosion;
	CHitInfo	hitInfo;

	SG_EXPLOSION_TRAP_SKILL	packet;

	packet.base.set(sizeof(packet),dSG_EXPLOSION_TRAP_SKILL);

	packet.bf6TargetCount	=	0;
	packet.bf11Skill		=	_lpTrap->m_wSerial;

	if (_iTargetMethod		==	eTT_TARGET)
	{
		if	(_lpTrap->explosion(_lpTarget,&hitInfo,_iTrapLevel))
		{
			packet.bf6TargetCount			=	1;
			packet.aData[0].wActor			=	_lpTarget->m_wSerialInField;
			packet.aData[0].wMagicDamage	=	(WORD)hitInfo.m_dwMagicDamage/100;
			packet.aData[0].wPhysicalDamage	=	(WORD)hitInfo.m_dwPhysicalDamage/100;

			if	(hitInfo.m_dwMagicDamage)
				packet.aData[0].wMagicDamage=	max(packet.aData[0].wMagicDamage,1);
			if	(hitInfo.m_dwPhysicalDamage)
				packet.aData[0].wPhysicalDamage=	max(packet.aData[0].wPhysicalDamage,1);
		}
	}
	else
	{
		posExplosion.x			=	_lpTarget->m_pos.x;
		posExplosion.y			=	_lpTarget->m_pos.y;

		for (int iTeam=0;iTeam < dTEAM_COUNT-2+m_bIsGuildBattleField;iTeam++)
		{
			for (int i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
			{
				int		iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];

				cACTOR	*lpActor=	getTestedActor(iSerial);

				if	(!lpActor || lpActor->isDeath())
					continue;
				if	(!lpActor->isInAttackRange(posExplosion.x,posExplosion.y,iHitRange))
					continue;
				if	(isMissileBlockedLine(posExplosion.x,posExplosion.y,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;
				if	(!_lpTrap->explosion(lpActor,&hitInfo,_iTrapLevel))
					continue;

				cTrapSkillDamageE	*lpInfo		=	&packet.aData[packet.bf6TargetCount];

				lpInfo->wActor					=	lpActor->m_wSerialInField;
				lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
				lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);

				if (hitInfo.m_dwMagicDamage)
					lpInfo->wMagicDamage		=	max(lpInfo->wMagicDamage,1);
				if (hitInfo.m_dwPhysicalDamage)
					lpInfo->wPhysicalDamage		=	max(lpInfo->wPhysicalDamage,1);

				packet.bf6TargetCount++;

				if	(packet.bf6TargetCount	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
				{
					addSendPacket((ALL_MSG*)&packet,_lpTarget->m_wSerialInField);
					packet.bf6TargetCount		=	0;
				}
			}
		}
	}

	if	(packet.bf6TargetCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cTrapSkillDamageE)*packet.bf6TargetCount;

		addSendPacket((ALL_MSG*)&packet,_lpTarget->m_wSerialInField);
	}
}

void
cFIELD::checkFieldTrap(cACTOR *_lpActor)
{
	cAreaTrapInfo	*lpTrap	=	m_pArea->getActiveTrap(_lpActor->m_pos.x,_lpActor->m_pos.y);

	if	(!lpTrap)
		return;

	lpTrap->explosionTrap(_lpActor);
}

BOOL
cFIELD::operateTrapWorkInAreaByKarma(int _iArea,int _iWork)
{
	cAreaInfo	*lpWorkArea	=	getArea(_iArea);

	if	(!lpWorkArea)
		return	FALSE;

	cRECT		*lpWorkRect	=	(cRECT	*)&lpWorkArea->x1;

	for (int i=0;i<dMAX_AREA;i++)
	{
		cAreaInfo	*lpArea	=	getArea(_iArea);

		if	(!lpArea || lpArea->m_wKind	!=	dAREA_DOOR)
			continue;

		cRECT	*lpRect		=	(cRECT	*)&lpArea->x1;

		if	(!lpWorkRect->Crash(lpRect))
			continue;

		cAreaTrapInfo	*lpTrapArea	=	(cAreaTrapInfo	*)lpArea;

		if	(lpTrapArea->m_wIsActive	==	FALSE)
		{
			if	(_iWork	==	eADTWK_REMOVE)
				continue;

			if	(_iWork	==	eADTWK_REGEN)
			{
				lpTrapArea->initTrap();
				addSendFieldTrapWork(lpTrapArea->m_wSerial,eFTW_ACTIVE);
				continue;
			}
		}

		if	(_iWork	==	eADTWK_REGEN)
			continue;

		if	(_iWork	==	eADTWK_REMOVE)
		{
			lpTrapArea->disarm(0xffff);
			addSendFieldTrapWork(lpTrapArea->m_wSerial,eFTW_DISACTIVE);
			continue;
		}
	}

	return	TRUE;
}