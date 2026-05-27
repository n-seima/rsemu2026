#include "cFIELD.H"
#include "cACTOR.H"
#include "CSolidCheck.H"
#include "CGame.H"
#include "packetManager.H"
#include "actor_mini_pet.H"

const	int	c_iRushSkillSpeed	=	200;
cPOINT	l_aRoad[400];
//
//	_iDestMx,_iDestMy으로 이동하는데 막히냐?
inline	BOOL
cFIELD::isBlockedWay(int _iMx,int _iMy,int _iDestMx,int _iDestMy)
{
	int	iAddress	=	_iDestMx+_iDestMy*m_iWidth;
	int	iCheckYValue=	-m_iWidth;
	int	iCheckXValue=	-1;

	if	(iAddress < 0 || iAddress >= m_iWidth*m_iHeight || m_pMap[iAddress])	//	해당 위치
		return	TRUE;

	if	(_iMx	==	_iDestMx	||	_iMy	==	_iDestMy		)
		return	FALSE;

	if	(_iMx	>	_iDestMx)
		iCheckXValue=	1;
	if	(_iMy	>	_iDestMy)
		iCheckYValue=	-iCheckYValue;

	if	(iAddress+iCheckYValue	<	0	||	iAddress+iCheckYValue >= m_iHeight*m_iWidth)
		return	TRUE;
	if	(iAddress+iCheckXValue	<	0	||	iAddress+iCheckXValue >= m_iHeight*m_iWidth)
		return	TRUE;

	if	(m_pMap[iAddress+iCheckYValue	]	||	m_pMap[iAddress+iCheckXValue	])
		return	TRUE;

	return	FALSE;
}

BOOL
cFIELD::isMissileBlockedLine(int _iX,int _iY,int _iDestX,int _iDestY)
{
	memset(l_aRoad,0,sizeof(cPOINT)*400);
	_iX		>>=	dTILE_XSIZE_SHIFT;
	_iY		>>=	dTILE_YSIZE_SHIFT;
	_iDestX	>>=	dTILE_XSIZE_SHIFT;
	_iDestY	>>=	dTILE_YSIZE_SHIFT;
	if	(_iX <= 0 || _iY <= 0 || _iDestX <= 0 || _iDestY <= 0)
		return	TRUE;
	
	int	iPathCount	=	g_protractor.getLine(_iX,_iY,_iDestX,_iDestY,l_aRoad,400);
	
	if	(iPathCount	<	0)
		return	TRUE;
	
	iPathCount	=	min(iPathCount,400);
	
	for (int i=iPathCount-1;i;)
	{
		--i;
		
		if	(isBlockedWay(l_aRoad[i].x,l_aRoad[i].y,l_aRoad[i+1].x,l_aRoad[i+1].y))
			return	TRUE;
	}
	
	return	FALSE;
}
BOOL
cFIELD::monsterBombExplosion(cAbility *_lpAbility,cACTOR *_lpCaster,int _iRange,int _iPoisonDamage,int _iPoisonDamageTime,int _iDarkDamagePercentage)
{
	CSkill		*lpSkill	=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	CHitInfo	hitInfo;
	int			i,iTeam;
	int			iX			=	_lpCaster->m_pos.x,iY=_lpCaster->m_pos.y;
	int			iLocalPart	=	getCurrentLocalPart(iY,iY);
	int			iDamage		=	_lpCaster->m_iHP*_iDarkDamagePercentage/100;
	_iRange					+=	_lpCaster->getBodySize();
	_iRange					=	_iRange*_iRange;

	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;

	packetWideAreaDamage.bf6Count		=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;

	packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
	packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
	packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
	packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			lpActor		=	getTestedActor(iSerial);

			if	(!lpActor || lpActor->isDeath())
				continue;

			DWORD	iRange	=	GetOvalRange(iX,iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if	(iRange	>	(DWORD)_iRange)
				continue;

			hitInfo.reset(TRUE);
			lpActor->setPoisonDamage(_iPoisonDamageTime,_iPoisonDamage/100,_lpCaster->m_wSerialInField);
			lpActor->getMagicDamageForEE(_lpCaster,_lpAbility,&hitInfo,0,0,0,0,0,iDamage);
			lpActor->reduceHP(hitInfo.getDamage(),_lpCaster);

			cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

			lpInfo->wActor					=	lpActor->m_wSerialInField;
			lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
			lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
			lpInfo->dwResultField			=	hitInfo.m_dwResultField;

			if	(hitInfo.m_dwMagicDamage)
				lpInfo->wMagicDamage		=	max(lpInfo->wMagicDamage,1);
			if	(hitInfo.m_dwPhysicalDamage)
				lpInfo->wPhysicalDamage		=	max(lpInfo->wPhysicalDamage,1);

			lpInfo->wMagicDamage			=	100;

			if	(lpInfo->wPhysicalDamage == 0 && lpInfo->wMagicDamage == 0)
				lpInfo->dwResultField	=	0;

			packetWideAreaDamage.bf6Count++;

			if	(packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
			{
				packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
				packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

				addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);

				packetWideAreaDamage.bf6Count	=	0;
			}
		}
	}

	if	(packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);
	}

	_lpCaster->reduceHP(_lpCaster->m_iHP);
	_lpCaster->m_bf1IsInstanceKill	=	TRUE;
	_lpCaster->addSendSkillEffect(_lpAbility,eSE_SKILL);

	return	TRUE;
}

BOOL
cFIELD::monsterBomb(cAbility *_lpAbility,cACTOR *_lpCaster,int _iRange,int _iPoisonDamage,int _iPoisonDamageTime,int _iDarkDamagePercentage,int _iBombRange)
{
	CSkill		*lpSkill	=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	int			i,iTeam;
	int			iX=_lpCaster->m_pos.x,iY=_lpCaster->m_pos.y;
	int			iLocalPart	=	getCurrentLocalPart(iY,iY);
	_iRange					=	_iRange*_iRange;

	iTeam	= _lpCaster->m_wTeam;

	for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
	{
		int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
		lpActor		=	getTestedActor(iSerial);

		if	(iSerial	==	_lpCaster->m_wSerialInField)
			continue;

		if	(!lpActor || lpActor->isDeath())
			continue;

		DWORD	iRange	=	GetOvalRange(iX,iY,lpActor->m_pos.x,lpActor->m_pos.y);

		if	(iRange	>	(DWORD)_iRange)
			continue;

		monsterBombExplosion(_lpAbility,lpActor,_iBombRange,_iPoisonDamage,_iPoisonDamageTime,_iDarkDamagePercentage);
	}

	return	TRUE;
}

BOOL
cFIELD::wideAidAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iX,int _iY)
{
	int			i,iTeam;
	CSkill		*lpSkill	=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	BOOL		bIsGotedCP	=	FALSE;
	int			iHitRange	=	_lpAbility->getHitRange();
	int			iLocalPart	=	getCurrentLocalPart(_lpCaster->m_pos.x,_lpCaster->m_pos.y);
	int			iTargetCount=	0;

	SG_WIDE_AID_ATTACK_RESULT	packet;

	packet.base.set(sizeof(packet),dSG_WIDE_AID_ATTACK_RESULT);
	packet.bf6Count		=	0;
	packet.bf12Skill	=	_lpAbility->m_wSkill;
	packet.bf12Caster	=	_lpCaster->m_wSerialInField;

	if	(iHitRange	==	0)
	{
		if	(_lpTarget)
		{
			packet.aData[0].m_bf12Target	=	_lpTarget->m_wSerialInField;

			if	(_lpTarget->applySkillExtraEffects(_lpCaster,_lpAbility))
			{
				_lpCaster->increaseCP(_lpAbility->getGetCP());

				packet.aData[0].m_bf1IsHit	=	TRUE;
			}
			else
				packet.aData[0].m_bf1IsHit	=	FALSE;

			packet.bf6Count	=	1;

			packet.sRemainCP	=	_lpCaster->m_iCP/100;
			packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+packet.bf6Count*sizeof(cWideAideAttackResultE);

			addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);

			return	TRUE;
		}

		return	FALSE;
	}

	iHitRange	+=	_lpCaster->getBodySize();

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;)		//	적 캐릭터
		{
			--i;

			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i];

			lpActor			=	getTestedActor(iSerial);

			if	(!lpActor || lpActor->isDeath())
				continue;
			if	(!lpActor->isInAttackRange(_iX,_iY,iHitRange))
				continue;
			if	(isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;
			if	(isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			packet.aData[packet.bf6Count].m_bf12Target	=	lpActor->m_wSerialInField;

			if	(lpActor->applySkillExtraEffects(_lpCaster,_lpAbility))
			{
				if	(!bIsGotedCP)
					_lpCaster->increaseCP(_lpAbility->getGetCP());

				bIsGotedCP	=	TRUE;

				packet.aData[packet.bf6Count].m_bf1IsHit	=	TRUE;
			}
			else
				packet.aData[packet.bf6Count].m_bf1IsHit	=	FALSE;

			packet.bf6Count++;
			iTargetCount++;

			if	(packet.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
			{
				packet.sRemainCP	=	_lpCaster->m_iCP/100;

				addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);

				packet.bf6Count		=	0;
				break;
			}
		}
	}

	if	(packet.bf6Count)
	{
		packet.sRemainCP	=	_lpCaster->m_iCP/100;
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+packet.bf6Count*sizeof(cWideAideAttackResultE);

		addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
	}

	if	(iTargetCount)
		return	TRUE;

	return	FALSE;
}

//
//	회오리형 데미지!!
BOOL
cFIELD::strikeWhirlWind(cACTOR *_lpCaster,cAbility *_lpAbility,BOOL bIsWantGetCP, BOOL* _bIsContinuousGetCP)
{
	int			i,iTeam;
	CSkill		*lpSkill	=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	CHitInfo	hitInfo;
	BOOL		isGetCP		=	TRUE;
	int			iHitRange	=	_lpCaster->getHitRange(_lpAbility, TRUE) +_lpCaster->getBodySize();
	int			iLocalPart	=	getCurrentLocalPart(_lpCaster->m_pos.x,_lpCaster->m_pos.y);

	if(!_lpCaster)
		return FALSE;

	if(!_lpAbility)
		return FALSE;

	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;

	packetWideAreaDamage.bf6Count	=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			if (packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
				break;

			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];

			lpActor	=	getTestedActor(iSerial);

			if	(!lpActor || lpActor->isDeath())
				continue;
			if	(!lpActor->isTargetInAttackRange(_lpCaster,iHitRange))
				continue;
			
			if	(isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			lpActor->getDamage(_lpCaster,_lpAbility,_lpCaster->m_pos.x,_lpCaster->m_pos.y,&hitInfo);

			//	여기서 속성 대미지도 처리 할것!!
			if	(bIsWantGetCP && isGetCP && hitInfo.isHit() )
			{
				if(_bIsContinuousGetCP)
				{
					*_bIsContinuousGetCP = TRUE;
				}
				isGetCP	=	FALSE;
				_lpCaster->increaseCP(_lpAbility->getGetCP());
				
			}

			_lpCaster->m_attackInfo.m_ability.copy(_lpAbility);
			BOOL	bIsStrike				=	_lpCaster->strike(lpActor,&hitInfo,_lpAbility,TRUE,TRUE,_lpCaster->m_pos.x,_lpCaster->m_pos.y,_lpCaster->m_bIsOperateCounter);

			cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

			lpInfo->wActor					=	lpActor->m_wSerialInField;
			lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
			lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
			lpInfo->dwResultField			=	hitInfo.m_dwResultField;

			if	(hitInfo.m_dwMagicDamage)
				lpInfo->wMagicDamage		=	max(lpInfo->wMagicDamage,1);
			if	(hitInfo.m_dwPhysicalDamage)
				lpInfo->wPhysicalDamage		=	max(lpInfo->wPhysicalDamage,1);

			if	(bIsStrike	==	FALSE && lpInfo->wPhysicalDamage == 0 && lpInfo->wMagicDamage == 0)
				lpInfo->dwResultField	=	0;

			packetWideAreaDamage.bf6Count++;
		}
	}

	if (_lpAbility->getKnockBackDistance())
		thrustActor(_lpCaster->m_pos.x,_lpCaster->m_pos.y,_lpCaster,NULL,_lpCaster->m_wActorKind,_lpAbility,iHitRange,_lpAbility->getKnockBackDistance());

	if (packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		if	(bIsWantGetCP	==	FALSE)
			packetWideAreaDamage.sRemainCP	=	0x7fff;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);

		return	TRUE;
	}

	return	FALSE;
}

//
//	아크 모양 타격
BOOL
cFIELD::strikeArcShape(cACTOR *_lpCaster,cAbility *_lpAbility,int _iX,int _iY,int _iAngle,int _iHitAngleRange)
{
	int			i,iTeam;
	CSkill		*lpSkill	=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	CHitInfo	hitInfo;
	BOOL		isGetCP		=	TRUE;
	int			iAttackRange=	_lpCaster->getAttackRange(_lpAbility);
	int			iLocalPart	=	getCurrentLocalPart(_lpCaster->m_pos.x,_lpCaster->m_pos.y);

	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;

	packetWideAreaDamage.bf6Count	=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;

	int	iMinAngle	=	(_iAngle-_iHitAngleRange/2+360)%360;
	int	iMaxAngle	=	(_iAngle+_iHitAngleRange/2+360)%360;

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			if (packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)	break;

			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];

			lpActor	=	getTestedActor(iSerial);

			if (!lpActor || lpActor->isDeath())	
				continue;
			if (!lpActor->isTargetInAttackRange(_lpCaster,iAttackRange))	
				continue;
			if (isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;
			if (isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;
			
			int	iAngle		=	g_protractor.getAngleToTarget(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if (iMinAngle > iMaxAngle)
			{
				if (iAngle<	iMinAngle && iAngle > iMaxAngle)	continue;
			}
			else
			if (iAngle	<	iMinAngle || iAngle > iMaxAngle)	continue;

			lpActor->getDamage(_lpCaster,_lpAbility,_iX,_iY,&hitInfo);

			//	여기서 속성 데미지도 처리 할것!!
			if (isGetCP && hitInfo.isHit())
			{
				isGetCP	=	FALSE;
				_lpCaster->increaseCP(_lpAbility->getGetCP());
			}

			_lpCaster->m_attackInfo.m_ability.copy(_lpAbility);
			_lpCaster->strike(lpActor,&hitInfo,_lpAbility,TRUE,TRUE,_iX,_iY);

			cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

			lpInfo->wActor					=	lpActor->m_wSerialInField;
			lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
			lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
			lpInfo->dwResultField			=	hitInfo.m_dwResultField;

			packetWideAreaDamage.bf6Count++;
		}
	}

	if (packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);

		return	TRUE;
	}

	return	FALSE;
}

//	바운싱 리니어 타겟 구한다.
void
cFIELD::getBouncingLinearTarget(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,CActiveSkillDamagedActorInfo *_lpInfo,int _iMaxCount)
{
	int			i,iTeam;
	CSkill		*lpSkill		=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	int			iAttackRange	=	_lpCaster->getAttackRange(_lpAbility)+_lpTarget->getBodySize();
	int			iMinAttackRange	=	_lpAbility->getMinimumAttackRange();
	cPOINT		posCurrent;
	int			iLocalPart		=	getCurrentLocalPart(_lpCaster->m_pos.x,_lpCaster->m_pos.y);

	_lpInfo->reset();

	posCurrent.x	=	_lpCaster->m_pos.x;
	posCurrent.y	=	_lpCaster->m_pos.y;
	
	_lpInfo->addActor(_lpTarget->m_wSerialInField);

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			if	(_lpInfo->m_wCount	>=	_iMaxCount)
				break;

			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];

			lpActor		=	getTestedActor(iSerial);

			if	(!lpActor || lpActor->isDeath())
				continue;
			if	(!lpActor->isTargetInAttackRange(_lpCaster,iAttackRange,iMinAttackRange))
				continue;
			if	(isMissileBlockedLine(posCurrent.x,posCurrent.y,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;
//			if	(_lpInfo->isDamagedActor(iSerial))
//				continue;
			posCurrent.x	=	lpActor->m_pos.x;
			posCurrent.y	=	lpActor->m_pos.y;

			_lpInfo->addActor(iSerial);
		}
	}
}

//	바운싱 리니어 타겟 구한다.
void
cFIELD::getBigBulletTarget(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,CActiveSkillDamagedActorInfo *_lpInfo,int _iMaxCount)
{
	int			i,iTeam;
	CSkill		*lpSkill		=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	int			iAttackRange	=	_lpAbility->getHitRange(NULL);
	int			iMinAttackRange	=	_lpAbility->getMinimumAttackRange();
	cPOINT		posCurrent;
	int			iLocalPart		=	getCurrentLocalPart(_lpTarget->m_pos.x,_lpTarget->m_pos.y);
	int			iLastSerial		=	0xffff;

	_lpInfo->reset();

	posCurrent.x	=	_lpTarget->m_pos.x;
	posCurrent.y	=	_lpTarget->m_pos.y;

	iTeam =	_lpTarget->m_wTeam;

	while(1)
	{
		int	iAddTarget	=	0;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			if	(_lpInfo->m_wCount	>=	_iMaxCount)
				break;

			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];

			lpActor		=	getTestedActor(iSerial);

			if	(iSerial==	iLastSerial	||	iSerial	==	_lpTarget->m_wSerialInField)
				continue;
			if	(!lpActor || lpActor->isDeath())
				continue;
			if	(!lpActor->isTargetInAttackRange(_lpCaster,iAttackRange,iMinAttackRange))
				continue;
			if	(isMissileBlockedLine(posCurrent.x,posCurrent.y,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;
	//		if	(_lpInfo->isDamagedActor(iSerial))
	//			continue;

			posCurrent.x	=	lpActor->m_pos.x;
			posCurrent.y	=	lpActor->m_pos.y;
			iLastSerial		=	iSerial;

			_lpInfo->addActor(iSerial);
			iAddTarget++;
		}

		if	(iAddTarget	==	0	||	_lpInfo->m_wCount	>=	_iMaxCount)
			break;
	}
}


//	폭발형
int
cFIELD::strikeExplosionSkill(cACTOR *_lpCaster,cACTOR *_lpTarget,int _iX,int _iY,cAbility *_lpAbility,BOOL *_bIsGetCP,
							int _iLimitCount,CActiveSkillDamagedActorInfo *_lpInfo,int _iStrikeIndex)
{
	g_debugSign.m_iExplosionSkillStep	=	0;

	int			i,iTeam;
	CSkill		*lpSkill						=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	CHitInfo	hitInfo;
	int			iHitRange						=	_lpCaster->getHitRange(_lpAbility,TRUE)+_lpCaster->getBodySize();
	BOOL		bIsApplyPhysicalDamageOnlyTarget=	_lpAbility->isApplyPhysicalDamageOnlyTarget();
	int			iWideAreaAttackDamagePercent	=	_lpAbility->getWideAreaAttackDamagePercent();
	BOOL		bIsAwesomeFortress				=	FALSE;
	int			iLocalPart						=	getCurrentLocalPart(_iX,_iY);
	BOOL		bIsTargetingToActorStickedBit	=	lpSkill->m_bf1IsTargetingToActorStickedBit;
	BOOL		bIsApplyExplosionWhenHitTargetStickedBit	=	lpSkill->m_bf1IsApplyExplosionWhenHitTargetStickedBit;	
	BOOL		bIsExplosion					=	TRUE;
	if	(lpSkill->m_wType	==	eSKILL_TYPE_AWESOME_FORTRESS)
		bIsAwesomeFortress	=	TRUE;

	
	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;

	packetWideAreaDamage.bf6Count		=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;
	
	if(bIsApplyExplosionWhenHitTargetStickedBit && _lpTarget)
	{
		int iBitCount	=	_lpTarget->getBitCount(_lpCaster->m_wSerialInField, _lpCaster->m_dwNameHashCode);
		_lpTarget->getDamage(_lpCaster,_lpAbility,_iX,_iY,&hitInfo);
		BOOL	bIsHit	=	TRUE;
		if	(!bIsAwesomeFortress)
		{
			if	(!*_bIsGetCP && hitInfo.isHit())
			{
				*_bIsGetCP	=	TRUE;
				_lpCaster->increaseCP(_lpAbility->getGetCP());
			}		
			if	(_iStrikeIndex	==	0)
				bIsHit		=	_lpCaster->strike(_lpTarget,&hitInfo,_lpAbility,TRUE,TRUE,_iX,_iY);
		}
		if(lpSkill->m_bf4SpendBit)
		{
			_lpTarget->removeBit(_lpCaster->m_wSerialInField, _lpCaster->m_dwNameHashCode, lpSkill->m_bf4SpendBit);
		}
		if(!bIsHit || !iBitCount)
			bIsExplosion = FALSE;
		cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];	
		lpInfo->wActor					=	_lpTarget->m_wSerialInField;
		lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
		lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
		lpInfo->dwResultField			=	hitInfo.m_dwResultField;
		packetWideAreaDamage.bf6Count++;
	}
	
	if	(lpSkill->isAgressiveSkill() && bIsExplosion)
		for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
		{
			if	(iTeam == _lpCaster->m_wTeam)
				continue;

			g_debugSign.m_iExplosionSkillActorCount	=	m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];
			g_debugSign.m_iExplosionSkillTeam		=	iTeam;
			g_debugSign.m_iExplosionSkillLocalPart	=	iLocalPart;

			for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
			{
				if	(packetWideAreaDamage.bf6Count	>=	_iLimitCount					)
					break;
				
				int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];

				lpActor	=	getTestedActor(iSerial);

				if (!lpActor || lpActor->isDeath())	
					continue;

				if(lpActor->m_wSlaveActor != 0xffff)			// 누구를 조정중이면 타격을 받지 않는다;
					continue;

				DWORD	iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);
				DWORD	dwHitRange	=	(iHitRange+lpActor->getBodySize())*(iHitRange+lpActor->getBodySize());

				if	(iRange	>	dwHitRange)	
					continue;
				if	(isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;
				if	(isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;
				if	(bIsTargetingToActorStickedBit)
				{
					int iBitCount	=	lpActor->getBitCount(_lpCaster->m_wSerialInField, _lpCaster->m_dwNameHashCode);
					if(iBitCount == 0)
						continue;
				}
				if(bIsApplyExplosionWhenHitTargetStickedBit)
				{
					if (_lpTarget && lpActor->m_wSerialInField == _lpTarget->m_wSerialInField && _lpTarget->m_dwNameHashCode == lpActor->m_dwNameHashCode)
						continue;
				}
				if(lpSkill->m_bf4SpendBit)
				{
					lpActor->removeBit(_lpCaster->m_wSerialInField, _lpCaster->m_dwNameHashCode, lpSkill->m_bf4SpendBit);
				}
				BOOL	bIsApplyPhysicalDamage	=	TRUE,bArrived=FALSE;

				if (bIsApplyPhysicalDamageOnlyTarget)
				{
					bIsApplyPhysicalDamage	=	FALSE;

					if (_lpTarget && lpActor->m_wSerialInField == _lpTarget->m_wSerialInField)
						bIsApplyPhysicalDamage	=	TRUE;
				}

				lpActor->getDamage(_lpCaster,_lpAbility,_iX,_iY,&hitInfo,bIsApplyPhysicalDamage,TRUE);

		//	여기서 속성 데미지도 처리 할것!!
				if	(!bIsAwesomeFortress)
					if	(!*_bIsGetCP && hitInfo.isHit())
					{
						*_bIsGetCP	=	TRUE;
						_lpCaster->increaseCP(_lpAbility->getGetCP());
					}

				if	(iWideAreaAttackDamagePercent != 655)
					if	(_lpTarget == NULL || lpActor->m_wSerialInField != _lpTarget->m_wSerialInField)
					{
						LONGLONG	llDamage;

						llDamage	=	hitInfo.m_dwMagicDamage;
						hitInfo.m_dwMagicDamage		=	(DWORD)(llDamage*iWideAreaAttackDamagePercent/100);

						llDamage	=	hitInfo.m_dwPhysicalDamage;
						hitInfo.m_dwPhysicalDamage	=	(DWORD)(llDamage*iWideAreaAttackDamagePercent/100);
					}

				BOOL	bIsStrike	=	FALSE;
				BOOL	bIsHit		=	FALSE;

				if	(!bIsAwesomeFortress)
				{
					if	(_iStrikeIndex	==	0)
						bIsHit		=	_lpCaster->strike(lpActor,&hitInfo,_lpAbility,TRUE,TRUE,_iX,_iY);
					else
					{
						lpActor->reduceHP(hitInfo.getDamage(),_lpCaster);

						if	(hitInfo.getDamage())
							bIsHit		=	TRUE;
					}
				}

				bIsStrike		=	bIsHit;

				if	(lpSkill->isCastOnEnemySkill())
					if	(_lpTarget && lpActor->m_wSerialInField == _lpTarget->m_wSerialInField)
						bIsStrike	=	TRUE;

				if	(bIsAwesomeFortress || bIsStrike	||	lpSkill->m_bf1IsRequireMissImage)
				{
					cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

					lpInfo->wActor					=	lpActor->m_wSerialInField;
					lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
					lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
					lpInfo->dwResultField			=	hitInfo.m_dwResultField;
					
					if	(hitInfo.m_dwPhysicalDamage)
						lpInfo->wPhysicalDamage		=	max(lpInfo->wPhysicalDamage,1);
					if	(hitInfo.m_dwMagicDamage)
						lpInfo->wMagicDamage		=	max(lpInfo->wMagicDamage,1);

					if	(bIsHit	==	FALSE	&&	bIsAwesomeFortress	==	FALSE)
						lpInfo->dwResultField		=	0;

					if	(_lpInfo)
						_lpInfo->addActor(lpActor->m_wSerialInField);

					if	(bIsAwesomeFortress)
						memcpy(&_lpCaster->m_attackInfo.m_aInfo[packetWideAreaDamage.bf6Count],&hitInfo,sizeof(hitInfo));

					packetWideAreaDamage.bf6Count++;
				}
				else
				{
					cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

					lpInfo->wActor					=	lpActor->m_wSerialInField;
					lpInfo->wPhysicalDamage			=	0;
					lpInfo->wMagicDamage			=	0;
					lpInfo->dwResultField			=	hitInfo.m_dwResultField;
					packetWideAreaDamage.bf6Count++;
				}
			}
		}

	g_debugSign.m_iExplosionSkillStep	=	1;

	if	(lpSkill->isGoodExtraEffectSkill())
	{
		g_debugSign.m_iExplosionSkillStep	=	2;

		BOOL	bIsWantGetCP	=	TRUE;

		if	(packetWideAreaDamage.bf6Count)
			bIsWantGetCP		=	FALSE;

		operateAuraSkill(_lpAbility,_iX,_iY,iHitRange,_lpCaster,bIsWantGetCP);
	}

	g_debugSign.m_iExplosionSkillStep	=	3;

	if	(packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.bf12Caster	=	(WORD)_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	(WORD)_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);

		g_debugSign.m_iExplosionSkillStep	=	-1;
		return	packetWideAreaDamage.bf6Count;
	}

	g_debugSign.m_iExplosionSkillStep	=	-2;

	return	0;
}
//
//	폭발형
int
cFIELD::getExplosionSkillDamage(cACTOR *_lpCaster,cACTOR *_lpTarget,int _iX,int _iY,cAbility *_lpAbility,int _iLimitCount,CDamageActorInfoTrailer *_lpDamageInfo)
{
	g_debugSign.m_iGetExplosionSkillStep	=	0;

	CSkill		*lpSkill						=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	CHitInfo	hitInfo;
	int			iHitRange						=	_lpAbility->getHitRange(_lpCaster->getWeapon())+_lpCaster->getBodySize();
	int			i,iTeam,iTargetCount			=	0;
	BOOL		bIsApplyPhysicalDamageOnlyTarget=	_lpAbility->isApplyPhysicalDamageOnlyTarget();
	int			iWideAreaAttackDamagePercent	=	_lpAbility->getWideAreaAttackDamagePercent();
	BOOL		bIsAwesomeFortress				=	FALSE;
	int			iLocalPart						=	getCurrentLocalPart(_iX,_iY);

	if (lpSkill->m_wType	==	eSKILL_TYPE_AWESOME_FORTRESS)
		bIsAwesomeFortress	=	TRUE;

	iHitRange	*=	iHitRange;
	_lpDamageInfo->m_wCount	=	0;

	g_debugSign.m_iGetExplosionSkillStep	=	1;

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam == _lpCaster->m_wTeam)
			continue;

		g_debugSign.m_iGetExplosionSkillTeam		=	iTeam;
		g_debugSign.m_iGetExplosionSkillLocalPart	=	iLocalPart;
		g_debugSign.m_iGetExplosionSkillActorCount	=	m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];

		g_debugSign.m_iGetExplosionSkillStep		=	2;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			if (iTargetCount >= _iLimitCount || iTargetCount >= dDAMAGE_ACTOR_INFO_TRAILER_SIZE	)
				break;

			g_debugSign.m_iGetExplosionSkillStep		=	3;

			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			lpActor		=	getTestedActor(iSerial);

			g_debugSign.m_iGetExplosionSkillStep		=	4;

			if (!lpActor || lpActor->isDeath())
				continue;

			DWORD	iRange	=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			g_debugSign.m_iGetExplosionSkillStep		=	5;

			if (iRange	>	(DWORD)iHitRange)
				continue;
			
			if (isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			g_debugSign.m_iGetExplosionSkillStep		=	6;

			if (_iX != _lpCaster->m_pos.x || _iY != _lpCaster->m_pos.y)
				if (isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

			g_debugSign.m_iGetExplosionSkillStep		=	7;


			BOOL	bIsApplyPhysicalDamage	=	TRUE,bArrived=FALSE;

			if (bIsApplyPhysicalDamageOnlyTarget)
			{
				bIsApplyPhysicalDamage	=	FALSE;

				if (_lpTarget && lpActor->m_wSerialInField == _lpTarget->m_wSerialInField)
					bIsApplyPhysicalDamage	=	TRUE;
			}

			g_debugSign.m_iGetExplosionSkillStep		=	8;
			lpActor->getDamage(_lpCaster,_lpAbility,_iX,_iY,&hitInfo,bIsApplyPhysicalDamage,TRUE);

			if (iWideAreaAttackDamagePercent != 655)
				if (_lpTarget == NULL || lpActor->m_wSerialInField != _lpTarget->m_wSerialInField)
				{
					LONGLONG	llDamage;

					llDamage	=	hitInfo.m_dwMagicDamage;
					hitInfo.m_dwMagicDamage		=	(DWORD)(llDamage*iWideAreaAttackDamagePercent/100);

					llDamage	=	hitInfo.m_dwPhysicalDamage;
					hitInfo.m_dwPhysicalDamage	=	(DWORD)(llDamage*iWideAreaAttackDamagePercent/100);
				}

			g_debugSign.m_iGetExplosionSkillStep=	9;
			CDamageActorInfo	*lpInfo			=	&_lpDamageInfo->m_aData[iTargetCount];

			lpInfo->m_wActorSerial				=	lpActor->m_wSerialInField;
			lpInfo->m_hitInfo.reset();
			lpInfo->m_hitInfo.m_dwPhysicalDamage=	hitInfo.m_dwPhysicalDamage;
			lpInfo->m_hitInfo.m_dwMagicDamage	=	hitInfo.m_dwMagicDamage;
			lpInfo->m_hitInfo.m_dwResultField	=	hitInfo.m_dwResultField;
			_lpDamageInfo->m_wCount++;

			iTargetCount++;
		}
	}

	g_debugSign.m_iGetExplosionSkillStep	=	-1;

	return	iTargetCount;
}

//
//_lpCaster가 관통미사일형 타격을 입힌다.
BOOL
cFIELD::strikePiercingMissile(cACTOR *_lpCaster,cAbility *_lpAbility,cACTOR *_lpTarget,WORD *_lpDamagedActorList)
{
	cPOINT					pos,aPath[200];
	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;
	CHitInfo				hitInfo;
	BOOL					isGetCP				=	TRUE;
	int						iDamagedActorCount	=	0;
	int						iLocalPart			=	getCurrentLocalPart(_lpCaster->m_pos.x,_lpCaster->m_pos.y);

	if (_lpDamagedActorList)	
		while (_lpDamagedActorList[iDamagedActorCount]!=0xffff)	
			iDamagedActorCount++;

	packetWideAreaDamage.bf6Count	=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;

	int	iRange		=	_lpCaster->getAttackRange(_lpAbility);
	int	iAngle		=	g_protractor.getAngleToTarget(_lpCaster->m_pos.x,_lpCaster->m_pos.y,_lpTarget->m_pos.x,_lpTarget->m_pos.y);
	int	iHitRange	=	_lpCaster->getHitRange(_lpAbility);
	int	iPath,i,iTeam;

	pos.x			=	_lpCaster->m_pos.x;
	pos.y			=	_lpCaster->m_pos.y;

	g_protractor.getAnglePos(&pos,iAngle,iRange);

	int		iPathCount	=	g_protractor.getLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,pos.x,pos.y,aPath,10,200);
	int		iCheck,iTargetCount	=	0;

	for (iPath=0;iPath<iPathCount;iPath++)
	{
		for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
		{
			if (iTeam == _lpCaster->m_wTeam)
				continue;

			for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
			{
				if (packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
					break;

				int		iSerial		=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
				cACTOR	*lpActor	=	getTestedActor(iSerial);
				BOOL	isDamaged	=	FALSE;

				if (!lpActor || lpActor->isDeath()	)
					continue;
				if (_lpDamagedActorList)
					for (int iIndex=0;iIndex<iDamagedActorCount;iIndex++)
					{
						if (_lpDamagedActorList[iIndex]	==	lpActor->m_wSerialInField)
						{
							isDamaged	=	TRUE;

							break;
						}
					}

				for (iCheck=0;iCheck<packetWideAreaDamage.bf6Count;iCheck++)
					if (packetWideAreaDamage.aData[iCheck].wActor	==	lpActor->m_wSerialInField)
					{
						isDamaged	=	TRUE;
						break;
					}

				if (isDamaged			)
					continue;

				if (!lpActor->isInAttackRange(aPath[iPath].x,aPath[iPath].y,iHitRange))
					continue;

				if (isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,aPath[iPath].x,aPath[iPath].y))
					continue;

				lpActor->getDamage(_lpCaster,_lpAbility,_lpCaster->m_pos.x,_lpCaster->m_pos.y,&hitInfo); 

				if (isGetCP	&&	hitInfo.isHit())
				{
					isGetCP	=	FALSE;
					_lpCaster->increaseCP(_lpAbility->getGetCP());
				}

				_lpCaster->strike(lpActor,&hitInfo,_lpAbility);

				cWIDE_AREA_DAMAGE_E	*lpInfo	=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

				lpInfo->wActor					=	lpActor->m_wSerialInField;
				lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
				lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
				lpInfo->dwResultField			=	hitInfo.m_dwResultField;

				packetWideAreaDamage.bf6Count++;
			}

			if (packetWideAreaDamage.bf6Count != iTargetCount)
			{
				iTargetCount		=	packetWideAreaDamage.bf6Count;
				int	iPiercingChance	=	_lpAbility->getPiercingChance();

				if (random(100)		>= iPiercingChance)
					break;
			}
		}
	}

	if (packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);

		return	TRUE;
	}

	return	FALSE;
}

//
//_lpCaster가 관통미사일형 타격을 입힌다.
BOOL
cFIELD::strikeLaser(cACTOR *_lpCaster,cAbility *_lpAbility,cPOINT *_lpX1Y1,cPOINT *_lpX2Y1,cPOINT *_lpX1Y2,cPOINT *_lpX2Y2)
{
	CSolidCheck				solidCheck;
	POINT					aEdge[4];
	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;
	CHitInfo				hitInfo;
	BOOL					isGetCP				=	TRUE;
	int						i,iTeam;
	int						iLocalPart	=	getCurrentLocalPart(_lpCaster->m_pos.x,_lpCaster->m_pos.y);

	aEdge[0].x	=	_lpX1Y1->x;
	aEdge[0].y	=	_lpX1Y1->y;
	aEdge[1].x	=	_lpX2Y1->x;
	aEdge[1].y	=	_lpX2Y1->y;
	aEdge[2].x	=	_lpX1Y2->x;
	aEdge[2].y	=	_lpX1Y2->y;
	aEdge[3].x	=	_lpX2Y2->x;
	aEdge[3].y	=	_lpX2Y2->y;

	solidCheck.SetSolid(aEdge,4);

	packetWideAreaDamage.bf6Count	=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			if	(packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
				break;

			int		iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			cACTOR	*lpActor=	getTestedActor(iSerial);
			POINT	pos;

			if	(!lpActor || lpActor->isDeath()	)
				continue;
			pos.x	=	lpActor->m_pos.x;
			pos.y	=	lpActor->m_pos.y;

			if	(solidCheck.IsCheckIn(pos))
				continue;

			if	(isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			lpActor->getDamage(_lpCaster,_lpAbility,_lpCaster->m_pos.x,_lpCaster->m_pos.y,&hitInfo); 

			if	(isGetCP	&&	hitInfo.isHit())
			{
				isGetCP	=	FALSE;
				_lpCaster->increaseCP(_lpAbility->getGetCP());
			}

			_lpCaster->strike(lpActor,&hitInfo,_lpAbility);

			cWIDE_AREA_DAMAGE_E	*lpInfo	=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

			lpInfo->wActor					=	lpActor->m_wSerialInField;
			lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
			lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
			lpInfo->dwResultField			=	hitInfo.m_dwResultField;

			packetWideAreaDamage.bf6Count++;
		}
	}

	if (packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);

		return	TRUE;
	}

	return	FALSE;
}

//
//	직선형연쇄폭발
BOOL
cFIELD::strikeLinerChainExplosion(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,WORD *_lpDamagedActorList)
{
	int			i;
	CSkill		*lpSkill	=	_lpAbility->getSkill();
	cACTOR		*lpActor;
	CHitInfo	hitInfo;
	BOOL		isGetCP		=	TRUE;
	int			iHitRange	=	_lpAbility->getHitRange(_lpCaster->getWeapon());
	int			iDamagedActorCount	=	0,iTeam;
	int			iLocalPart	=	getCurrentLocalPart(_iX,_iY);

	while (_lpDamagedActorList[iDamagedActorCount]!=0xffff)	iDamagedActorCount++;

	if (iDamagedActorCount	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK	)	return	FALSE;

	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;

	packetWideAreaDamage.bf6Count	=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			if (packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK	||
				iDamagedActorCount				>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK	)	break;

			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			lpActor		=	getTestedActor(iSerial);

			if (!lpActor || lpActor->isDeath())	continue;
			BOOL	isDamagedActor	=	FALSE;

			for (int iIndex=0;iIndex<iDamagedActorCount;iIndex++)
			{
				if (_lpDamagedActorList[iIndex]	==	iSerial)
				{
					isDamagedActor	=	TRUE;

					break;
				}
			}

			if (isDamagedActor)	continue;

			if (lpActor->isInAttackRange(_iX,_iY,iHitRange))
			{
				if (isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;
				if (isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

				lpActor->getDamage(_lpCaster,_lpAbility,_iX,_iY,&hitInfo);

				_lpCaster->strike(lpActor,&hitInfo,_lpAbility,TRUE,TRUE,_iX,_iY);

				cWIDE_AREA_DAMAGE_E	*lpInfo					=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

				lpInfo->wActor								=	lpActor->m_wSerialInField;
				lpInfo->wPhysicalDamage						=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
				lpInfo->wMagicDamage						=	(WORD)(hitInfo.m_dwMagicDamage/100);
				lpInfo->dwResultField						=	hitInfo.m_dwResultField;
				_lpDamagedActorList[iDamagedActorCount++]	=	lpActor->m_wSerialInField;

				packetWideAreaDamage.bf6Count++;
			}
		}
	}

	if (packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);

		return	TRUE;
	}

	return	FALSE;
}

//
//	액터를 밀어낸다.
BOOL
cFIELD::thrustActor(int _iX,int _iY,cACTOR *_lpCaster,cACTOR *_lpTarget,int _iActorKind,cAbility *_lpAbility,int _iRange,int _iKnockBackDistance,int _iAngleToTarget)
{
	int			i,iTeam;
	cACTOR		*lpActor;
	int			iTarget	=	0xffff;
	int			iLocalPart	=	getCurrentLocalPart(_iX,_iY);
	int			iCheckRange	=	_iRange*_iRange;

	SG_THRUST_ACTORS		packet;

	packet.wCount	=	0;
	packet.wAxisXPos=	_iX;
	packet.wAxisYPos=	_iY;
	if	(_lpAbility)
		packet.wSkill		=	_lpAbility->m_wSkill;
	else
		packet.wSkill	=	0xffff;

	if	(_lpTarget)
		iTarget	=	_lpTarget->m_wSerialInField;

	for (iTeam=0;iTeam < dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			lpActor		=	getTestedActor(iSerial);

			if	(!lpActor || lpActor->m_wSerialInField == _lpCaster->m_wSerialInField)
				continue;

			if	(_iKnockBackDistance	==	0)
			{
				int	iCurRange	=	GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,_iX,_iY);

				if	(iCurRange	>=	iCheckRange)
					continue;

				int		iNockBackDistance	=	_iRange-(int)sqrt(iCurRange);
				BOOL	bIsOk				=	FALSE;

				if	(iNockBackDistance	<=	0)
					continue;

				bIsOk	=	lpActor->thrust(_iX,_iY,iNockBackDistance);

				if	(!bIsOk)
					continue;

				packet.aActor[packet.wCount].bf11SerialInField=	iSerial;
				packet.aActor[packet.wCount].bf2Team		=	lpActor->m_wTeam;
				packet.aActor[packet.wCount].wX				=	lpActor->m_pos.x;
				packet.aActor[packet.wCount].wY				=	lpActor->m_pos.y;

				packet.wCount++;
			}
			else
			{
				if	(lpActor->isDeath()	||	lpActor->m_bf1IsImmuneKnockBack)
					continue;

				if	(lpActor->isInAttackRange(_iX,_iY,_iRange))
				{
					BOOL	bIsOk	=	FALSE;

					if	(iSerial==	iTarget && _iAngleToTarget != 0xffff)
						bIsOk	=	lpActor->thrustByAngle(_iAngleToTarget,_iKnockBackDistance);
					else
						bIsOk	=	lpActor->thrust(_iX,_iY,_iKnockBackDistance);

					if	(!bIsOk)
						continue;

					packet.aActor[packet.wCount].bf11SerialInField=	iSerial;
					packet.aActor[packet.wCount].bf2Team		=	lpActor->m_wTeam;
					packet.aActor[packet.wCount].wX				=	lpActor->m_pos.x;
					packet.aActor[packet.wCount].wY				=	lpActor->m_pos.y;

					packet.wCount++;

					lpActor->applySkillExtraEffects(_lpCaster,_lpAbility,0,_iX,_iY);
				}
			}

			if	(packet.wCount	>=	dMAX_TARGET_FOR_THRUST_ATTACK)
			{
				packet.base.set(sizeof(SG_THRUST_ACTORS)-sizeof(packet.aActor),dSG_THRUST_ACTORS);
				packet.base.wSize		+=	packet.wCount*sizeof(CMinimumActorInfo);

				addSendPacket((ALL_MSG*)&packet,0xffff);

				packet.wCount			=	0;

				return	TRUE;
			}
		}
	}

	if	(packet.wCount)
	{
		packet.base.set(sizeof(SG_THRUST_ACTORS)-sizeof(packet.aActor),dSG_THRUST_ACTORS);
		packet.base.wSize		+=	packet.wCount*sizeof(CMinimumActorInfo);

		addSendPacket((ALL_MSG*)&packet,0xffff);

		return	TRUE;
	}

	return	FALSE;
}
BOOL
cFIELD::thrustCenter(cACTOR *_lpCaster,int _iThrustCenterX, int _iThrustCenterY, int _iRange,int _iMinRange/* = 0*/)
{
	int			i,iTeam;
	cACTOR		*lpActor;
	int			iTarget				=	0xffff;
	_iRange							=	min(_iRange,0x7fff);
	int			iLocalPart			=	getCurrentLocalPart(_iThrustCenterX,_iThrustCenterY);
	int			iCheckRange			=	_iRange*_iRange;
	int			iKnockBackDistance	=	0;

	SG_THRUST_ACTORS		packet;

	packet.wCount	=	0;
	packet.wAxisXPos=	_iThrustCenterX;
	packet.wAxisYPos=	_iThrustCenterY;
	packet.wSkill	=	0xffff;

	for (iTeam=0;iTeam < dTEAM_COUNT;iTeam++)
	{
		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)	//	피아 구분 없음
		{
			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			lpActor		=	getTestedActor(iSerial);


			if	(!lpActor || lpActor->m_wSerialInField == _lpCaster->m_wSerialInField	||	lpActor->isDeath())
				continue;

			if(_lpCaster->isPlayer())
			{		// 공격자가 플레이어일때...
				if(lpActor->isNpc())
					continue;
				if(_lpCaster->isOwnForce(lpActor) )
					continue;		// 아군 제외,,
			}

			if	(lpActor->isInAttackRange(	_iThrustCenterX,	_iThrustCenterY,_iRange,_iMinRange))
			{
				if	(isMissileBlockedLine(	_iThrustCenterX,_iThrustCenterY,lpActor->m_pos.x,lpActor->m_pos.y))	// 벽이 막으면 당기지 않는다..
					continue;
				BOOL	bIsOk	=	FALSE;
				BOOL	bSendInfo	=	 FALSE;
				if(isPVP(_lpCaster, lpActor))
				{
					bSendInfo	= TRUE;
				}
				iKnockBackDistance	=	GetOvalRange(_iThrustCenterX,_iThrustCenterY,lpActor->m_pos.x,lpActor->m_pos.y);
				iKnockBackDistance	=	-(int)sqrt(iKnockBackDistance);

				bIsOk	=	lpActor->thrust(	_iThrustCenterX,	_iThrustCenterY,iKnockBackDistance,bSendInfo,0,TRUE);

				if	(!bIsOk)
					continue;

				packet.aActor[packet.wCount].bf11SerialInField=	iSerial;
				packet.aActor[packet.wCount].bf2Team		=	lpActor->m_wTeam;
				packet.aActor[packet.wCount].wX				=	lpActor->m_pos.x;
				packet.aActor[packet.wCount].wY				=	lpActor->m_pos.y;

				packet.wCount++;
			}

			if	(packet.wCount	>=	dMAX_TARGET_FOR_THRUST_ATTACK)
			{
				packet.base.set(sizeof(SG_THRUST_ACTORS)-sizeof(packet.aActor),dSG_THRUST_ACTORS);
				packet.base.wSize		+=	packet.wCount*sizeof(CMinimumActorInfo);

				addSendPacket((ALL_MSG*)&packet,0xffff);

				packet.wCount			=	0;


				return	TRUE;
			}
		}
	}

	if	(packet.wCount)
	{
		packet.base.set(sizeof(SG_THRUST_ACTORS)-sizeof(packet.aActor),dSG_THRUST_ACTORS);
		packet.base.wSize		+=	packet.wCount*sizeof(CMinimumActorInfo);

		addSendPacket((ALL_MSG*)&packet,0xffff);

		return	TRUE;
	}

	return	FALSE;
}
BOOL
cFIELD::thrust(cACTOR *_lpCaster,int _iRange,int _iKnockBackDistance,int _iMinRange, BOOL _bIsKnockBackResistance)
{
	int			i,iTeam;
	cACTOR		*lpActor;
	int			iTarget	=	0xffff;
	_iRange				=	min(_iRange,0x7fff);
	int			iLocalPart	=	getCurrentLocalPart(_lpCaster->m_pos.x,_lpCaster->m_pos.y);
	int			iCheckRange	=	_iRange*_iRange;

	SG_THRUST_ACTORS		packet;

	packet.wCount	=	0;
	packet.wAxisXPos=	_lpCaster->m_pos.x;
	packet.wAxisYPos=	_lpCaster->m_pos.y;
	packet.wSkill	=	0xffff;

	for (iTeam=0;iTeam < dTEAM_COUNT;iTeam++)
	{
		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)	//	피아 구분 없음
		{
			int	iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			lpActor		=	getTestedActor(iSerial);

			if	(!lpActor || lpActor->m_wSerialInField == _lpCaster->m_wSerialInField	||	lpActor->isDeath())
				continue;

			if(_lpCaster->isPlayer())
			{		// 공격자가 플레이어일때...
				if(lpActor->isNpc())
					continue;
				if(_lpCaster->isOwnForce(lpActor) )
					continue;		// 아군 제외,,
			}

			if	(lpActor->isInAttackRange(_lpCaster->m_pos.x,_lpCaster->m_pos.y,_iRange,_iMinRange))
			{
				if	(isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))	// 벽이 막으면 당기지 않는다..
					continue;

				if(_bIsKnockBackResistance)
				{
					if	(random(100)	<	lpActor->m_bKnockBackResistance)
						continue;
					
					if	(lpActor->m_bf1IsCancelKnockBack	||	lpActor->m_bf1IsImmuneKnockBack)
					{
						lpActor->m_bf1IsCancelKnockBack	=	FALSE;
						continue;
					}
				}
				
				BOOL	bIsOk	=	FALSE;
				BOOL	bSendInfo	=	 FALSE;
				if(isPVP(_lpCaster, lpActor))
				{
					bSendInfo	= TRUE;
				}
				bIsOk	=	lpActor->thrust(_lpCaster->m_pos.x,_lpCaster->m_pos.y,_iKnockBackDistance,bSendInfo,0,TRUE);

				if	(!bIsOk)
					continue;

				packet.aActor[packet.wCount].bf11SerialInField=	iSerial;
				packet.aActor[packet.wCount].bf2Team		=	lpActor->m_wTeam;
				packet.aActor[packet.wCount].wX				=	lpActor->m_pos.x;
				packet.aActor[packet.wCount].wY				=	lpActor->m_pos.y;

				packet.wCount++;
			}

			if	(packet.wCount	>=	dMAX_TARGET_FOR_THRUST_ATTACK)
			{
				packet.base.set(sizeof(SG_THRUST_ACTORS)-sizeof(packet.aActor),dSG_THRUST_ACTORS);
				packet.base.wSize		+=	packet.wCount*sizeof(CMinimumActorInfo);

				addSendPacket((ALL_MSG*)&packet,0xffff);

				packet.wCount			=	0;


				return	TRUE;
			}
		}
	}

	if	(packet.wCount)
	{
		packet.base.set(sizeof(SG_THRUST_ACTORS)-sizeof(packet.aActor),dSG_THRUST_ACTORS);
		packet.base.wSize		+=	packet.wCount*sizeof(CMinimumActorInfo);

		addSendPacket((ALL_MSG*)&packet,0xffff);

		return	TRUE;
	}

	return	FALSE;
}



//
//	더미 제거
BOOL
cFIELD::releaseDummy(cACTOR *_lpActor)
{
	SG_RELEASE_DUMMY	packet;

	packet.base.set(sizeof(SG_RELEASE_DUMMY),dSG_RELEASE_DUMMY);
	packet.wActor	=	_lpActor->m_wSerialInField;

	addSendPacket((ALL_MSG*)&packet,packet.wActor);

	return	TRUE;
}

//
//	아이시 스탈라그마이트 제거
BOOL
cFIELD::castIcyStalagmite(cACTOR *_lpActor,cAbility *_lpAbility,int _iStatus)
{
	BOOL	bIsCasting	=	TRUE;

	if	(_lpActor->m_wHillSkill != 0xffff)
		bIsCasting	=	FALSE;

	SG_ICY_STALAGMITE	packet;

	packet.base.set(sizeof(packet),dSG_ICY_STALAGMITE);
	packet.wActor	=	_lpActor->m_wSerialInField;
	packet.wSkill	=	_lpAbility->m_wSkill;
	packet.wLevel	=	_lpAbility->m_wLevel;
	packet.wStatus	=	_iStatus;
	packet.sRemainCP=	_lpActor->m_iCP/100;

	if	(_iStatus	==	-1)
	{
		if	(bIsCasting)
			packet.wStatus	=	eRUIS_CAST;
		else
			packet.wStatus	=	eRUIS_CANCEL;
	}

	addSendPacket((ALL_MSG*)&packet,packet.wActor);

	return	TRUE;
}

//
//	오러 스킬 처리
void
cFIELD::operateTitleEffect(int _iTitle,cACTOR *_lpCaster,int _iRange)
{
	int		i;
	int		iX			=	_lpCaster->m_pos.x,iY=_lpCaster->m_pos.y;
	int		iLocalPart	=	getCurrentLocalPart(iX,iY);
	cParty	*lpParty	=	_lpCaster->getParty();
	cACTOR	*lpActor,*lpLord	=	_lpCaster->getControlLord();
	int		iSerial;

	SG_TITLE_EFFECT	packet;

	packet.base.set(sizeof(packet),dSG_TITLE_EFFECT);
	packet.wTitle		=	_iTitle;
	packet.wTargetCount	=	0;

	{
		int	iMemberCount;

		if	(!lpParty)
			iMemberCount	=	1;
		else
			iMemberCount	=	lpParty->getMemberCount();

		for(int iMember=0;iMember<iMemberCount;iMember++)
		{
			cACTOR	*lpMember;

			if	(!lpParty)
				lpMember	=	lpLord;
			else
				lpMember	=	lpParty->getActorByMemberIndex(iMember);

			if	(!_lpCaster->isOnSameField(lpMember))
				continue;

			lpActor	=	lpMember;
			iSerial	=	lpActor->m_wSerialInField;

			if	(lpActor->isInAttackRange(iX,iY,_iRange))
			{
				if	(packet.wTargetCount	<	100)
					packet.awTargetList[packet.wTargetCount++]	=	iSerial;
			}

			for	(i=0;i<dOWN_PET_COUNT;i++)
			{
				lpActor	=	lpMember->getPetActor(i);

				if	(lpActor)
				{
					iSerial	=	lpActor->m_wSerialInField;

					if	(lpActor->isInAttackRange(iX,iY,_iRange))
					{
						if	(packet.wTargetCount	<	100)
							packet.awTargetList[packet.wTargetCount++]	=	iSerial;
					}
				}
			}
			for	(i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
			{
				lpActor	=	lpMember->getSummonBeastActor(i);

				if	(lpActor)
				{
					iSerial	=	lpActor->m_wSerialInField;

					if	(lpActor->isInAttackRange(iX,iY,_iRange))
					{
						if	(packet.wTargetCount	<	100)
							packet.awTargetList[packet.wTargetCount++]	=	iSerial;
					}
				}
			}
		}
	}

	CTitle	*lpTitle	=	g_titleManager.getTitle(_iTitle);

	switch(_iTitle)
	{
		case	eTN_BARD	:
		{
			for (int i=packet.wTargetCount;i;)
			{
				--i;
				lpActor	=	getTestedActor(packet.awTargetList[i]);

				if	(!lpActor)
					continue;

				int	iUpkeepTime	=	lpTitle->m_aEffect[0].m_aiValue[0][3];
				int	iValue		=	lpTitle->m_aEffect[0].m_aiValue[0][4];

				lpActor->m_wCorrectValueByBardTitle		=	iValue;
				lpActor->m_wCorrectValueByBardTitleTime	=	60*iUpkeepTime;

				lpActor->m_iHP	=	lpActor->getMaxHP();
				lpActor->m_iCP	=	lpActor->getMaxCP();
			}

			packet.base.wSize	=	sizeof(packet)-sizeof(packet.awTargetList)+sizeof(WORD)*packet.wTargetCount;

			addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);

//	"사용자가 앉아 있는 상태에서 사용시 10초간 3가지의 음악 중 1개가 랜덤으로 연주 된 후, 파티원들의 HP/CP를 모두 회복. 
//	파티원(펫,소환수 포함)의 최대 HP와 최대 CP가 20%증가. 5분 동안 지속. 쿨타임 30분. 필드 이동시 효과 사라짐."
//	모든 파티원, 펫, 소환수
//	300px
//	사용자가 앉아 있다.
			break;
		}
	}
}

//
//	오러 스킬 처리
void
cFIELD::operateAuraSkill(cAbility *_lpAbility,int _iX,int _iY,int _iRange,cACTOR *_lpCaster,BOOL _bIsIncreaseCP)
{
	g_debugSign.m_iAuraSkillStep	=	0;

	int						i,iTeam;
	WORD					aHitActor[100],iHitActorCount	=	0;
	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;
	BOOL					isGetedCP		=	FALSE;
	BOOL					bIsUsetoOwnTeamSkill	=	FALSE;
	int						iLocalPart	=	getCurrentLocalPart(_iX,_iY);
	CSkill					*lpSkill	=	_lpAbility->getSkill();
	BOOL					bIsUseAllTeam=	FALSE;

	packetWideAreaDamage.bf6Count	=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;

	if	(!lpSkill)
		return;

	if	(_iRange	==	0)
	{
		g_debugSign.m_iAuraSkillStep	=	3;

		if	(!_lpCaster->applySkillExtraEffects(_lpCaster,_lpAbility,0,_iX,_iY,TRUE))
		{
			g_debugSign.m_iAuraSkillStep	=	4;

			if	(lpSkill->m_bf1IsHideMissImage==	FALSE)
				_lpCaster->addPacketMiss(_lpAbility->m_wSkill);

			return;
		}

		_lpCaster->addSendSkillEffect(_lpAbility,eSE_CASTER_HIT);

		g_debugSign.m_iAuraSkillStep	=	-1;

		return;
	}

	if	((lpSkill->m_dwTargetMethod & eSKILL_CAST_AT_PLAYER) || lpSkill->isGoodExtraEffectSkill())
		bIsUsetoOwnTeamSkill	=	TRUE;

	if	((lpSkill->m_dwTargetMethod & eSKILL_CAST_AT_ENEMY)	&&	(lpSkill->m_dwTargetMethod & eSKILL_CAST_AT_CASTER))
		bIsUseAllTeam			=	TRUE;

	g_debugSign.m_iAuraSkillStep	=	2;

//	int	iActivateChace	=	_lpAbility->getActivateChance();
//
//	if (lpSkill->m_wActiveChanceByInRangeEnemyCount)
//		iActivateChace	+=	lpSkill->m_wActiveChanceByInRangeEnemyCount*getInRangeEnemyCount(_lpCaster,_iRange)/100;
//	if (random(100) >= iActivateChace)
//		return;

	g_debugSign.m_iAuraSkillStep		=	5;
	g_debugSign.m_iAuraSkillLocalPart	=	iLocalPart;

	if	(lpSkill->m_bf1IsApplyToPartyAura)
	{
		//JBC	08-08-28
		//cParty	*lpParty	=	_lpCaster->getParty();
		cACTOR	*lpActor,*lpLord	=	_lpCaster->getControlLord();
		cParty	*lpParty	=	lpLord->getParty();
		int		iSerial,i;

		{
			int	iMemberCount;

			if	(!lpParty)
				iMemberCount	=	1;
			else
				iMemberCount	=	lpParty->getMemberCount();

			for(int iMember=0;iMember<iMemberCount;iMember++)
			{
				cACTOR	*lpMember;

				if	(!lpParty)
					lpMember	=	lpLord;
				else
					lpMember	=	lpParty->getActorByMemberIndex(iMember);

				if	(!_lpCaster->isOnSameField(lpMember))
					continue;

				lpActor	=	lpMember;
				iSerial	=	lpActor->m_wSerialInField;

				if	(lpActor->isInAttackRange(_iX,_iY,_iRange))
				{
					if	(iHitActorCount	<	dMAX_AURA_SKILL_HIT_ACTOR_COUNT)
						if	(lpActor->applySkillExtraEffects(_lpCaster,_lpAbility,0,_iX,_iY,TRUE))
							aHitActor[iHitActorCount++]	=	iSerial;
				}

				for	(i=0;i<dOWN_PET_COUNT;i++)
				{
					lpActor	=	lpMember->getPetActor(i);

					if	(lpActor)
					{
						iSerial	=	lpActor->m_wSerialInField;

						if	(lpActor->isInAttackRange(_iX,_iY,_iRange))
						{
							if	(iHitActorCount	<	dMAX_AURA_SKILL_HIT_ACTOR_COUNT)
								if	(lpActor->applySkillExtraEffects(_lpCaster,_lpAbility,0,_iX,_iY,TRUE))
									aHitActor[iHitActorCount++]	=	iSerial;
						}
					}
				}
				for	(i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
				{
					lpActor	=	lpMember->getSummonBeastActor(i);

					if	(lpActor)
					{
						iSerial	=	lpActor->m_wSerialInField;

						if	(lpActor->isInAttackRange(_iX,_iY,_iRange))
						{
							if	(iHitActorCount	<	dMAX_AURA_SKILL_HIT_ACTOR_COUNT)
								if	(lpActor->applySkillExtraEffects(_lpCaster,_lpAbility,0,_iX,_iY,TRUE))
									aHitActor[iHitActorCount++]	=	iSerial;
						}
					}
				}
			}
		}
	}
	else
	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(bIsUseAllTeam	==	FALSE)
		{
			if	(bIsUsetoOwnTeamSkill)
			{
				if	(iTeam != _lpCaster->m_wTeam)
					continue;
			}
			else
			{
				if	(iTeam == _lpCaster->m_wTeam)
					continue;
			}
		}

		g_debugSign.m_iAuraSkillTeam		=	iTeam;
		g_debugSign.m_iAuraSkillTargetCount	=	m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			int		iSerial		=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			cACTOR	*lpActor	=	getTestedActor(iSerial);

			if	(!lpActor || lpActor->isDeath()	||	lpActor->m_bIsGuildObject)
				continue;

			if	(lpActor->isInAttackRange(_iX,_iY,_iRange))
			{
				if	(lpSkill->isDamageSkill() && lpActor->isEnemy(_lpCaster))	//	대미지 주는 스킬이고 적이다.
				{
					if	(packetWideAreaDamage.bf6Count < dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
					{
						CHitInfo	hitInfo;

						hitInfo.reset();
						hitInfo.m_dwResultField	=	0;

						lpActor->getDamage(_lpCaster,_lpAbility,_iX,_iY,&hitInfo);

						if (hitInfo.isHit() && hitInfo.getDamage())
						{
							_lpCaster->strike(lpActor,&hitInfo,_lpAbility,TRUE,TRUE,_iX,_iY);

							if (!isGetedCP)
							{
								isGetedCP	=	TRUE;
								_lpCaster->increaseCP(_lpAbility->getGetCP());
							}

							cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

							lpInfo->wActor					=	lpActor->m_wSerialInField;
							lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
							lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
							lpInfo->dwResultField			=	hitInfo.m_dwResultField;

							packetWideAreaDamage.bf6Count++;
						}
					}
				}
				else
				{
					if	(iHitActorCount	<	dMAX_AURA_SKILL_HIT_ACTOR_COUNT)
						if	(lpActor->applySkillExtraEffects(_lpCaster,_lpAbility,0,_iX,_iY,TRUE))
						{
							if	(bIsUsetoOwnTeamSkill	==	FALSE)
								lpActor->operateRevenge(_lpCaster,_lpAbility);

							aHitActor[iHitActorCount++]	=	iSerial;
						}
				}
			}
		}
	}

	g_debugSign.m_iAuraSkillStep		=	6;

	if	(iHitActorCount || packetWideAreaDamage.bf6Count)
	{
		_lpCaster->sendPlayerBasicInformation();

		g_debugSign.m_iAuraSkillStep		=	7;
		SG_HIT_AURA_SKILL	packet;

		packet.base.set(sizeof(SG_HIT_AURA_SKILL),dSG_HIT_AURA_SKILL);

		packet.wCaster		=	_lpCaster->m_wSerialInField;
		packet.wSkill		=	_lpAbility->m_wSkill;
		packet.bf10Count	=	iHitActorCount;
		packet.wRemainCP	=	0;
		
		if	(lpSkill->m_bf1IsUsHealEffectToHitImageForAuraSkill)
			packet.bf6HitImage	=	eSE_HEAL;
		else
			packet.bf6HitImage	=	eSE_HIT;

		memcpy(packet.aActorList,aHitActor,sizeof(WORD)*iHitActorCount);

		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aActorList)+sizeof(WORD)*iHitActorCount;

		addSendPacket((ALL_MSG *)&packet,_lpCaster->m_wSerialInField);

		if	(_bIsIncreaseCP	&&	lpSkill->m_bf1IsNotIncreaseByActiveAura	==	FALSE)
			if	(lpSkill->m_wType!=	eSKILL_TYPE_CHARGE)
				_lpCaster->increaseCP(_lpAbility->getGetCP());
	}

	if	(packetWideAreaDamage.bf6Count)
	{
		g_debugSign.m_iAuraSkillStep		=	8;
		packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);

	}

	g_debugSign.m_iAuraSkillStep		=	-1;
}

//
//	러쉬 스킬 처리
BOOL
cFIELD::operateRushSkill(cACTOR *_lpCaster,cACTOR *_lpTarget,int _iX,int _iY,cAbility *_lpAbility,cPOINT *_lpPosArrive,CActiveSkillDamagedActorInfo *_lpInfo,BOOL _bIsIllusion)
{
	g_debugSign.m_iExplosionSkillStep			=	0;

	int						iPPS		=	_lpCaster->m_wPPS,iTeam,i;
	CSkill					*lpSkill	=	_lpAbility->getSkill();
	CHitInfo				hitInfo;
	BOOL					isGotedCP	=	_lpInfo->m_wIsGotedCP,bIsFindVictim=	TRUE;
	BOOL					bIsAttackOnlyTargetRush	=	lpSkill->m_bf1IsAttackOnlyTargetRush;
	SG_RUSH					packetRush;
	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;
	SG_THRUST_ACTORS		packetThrustActor;
	BOOL					bIsApplyPhysicalDamageOnlyTarget =	_lpAbility->isApplyPhysicalDamageOnlyTarget();
	WORD					wTargetSerial;
	int						iNockBackDistance	=	_lpAbility->getKnockBackDistance();
	int						iHitRange			=	_lpAbility->getHitRange(_lpCaster->getWeapon())+_lpCaster->getBodySize();
	BOOL					bIsPenetrationRush	=	lpSkill->m_bf1IsPenetrationRush;
	int						iWideAreaAttackDamagePercent	=	_lpAbility->getWideAreaAttackDamagePercent();
	int						iLocalPart			=	getCurrentLocalPart(_iX,_iY);
	cPOINT					posBegin;
	int						iAngleToTarget		=	g_protractor.getAngleToTarget(_lpCaster->m_pos.x,_lpCaster->m_pos.y,_iX,_iY);

	posBegin.x			=	_lpCaster->m_pos.x;
	posBegin.y			=	_lpCaster->m_pos.y;

	if (_lpTarget)	
		wTargetSerial	=	_lpTarget->m_wSerialInField;

	g_debugSign.m_iExplosionSkillStep			=	1;
	_lpCaster->m_wPPS	=	c_iRushSkillSpeed;
	_lpCaster->makePath(_iX,_iY);

	g_debugSign.m_iExplosionSkillStep			=	2;

	packetWideAreaDamage.bf6Count	=	0;
	packetWideAreaDamage.bf1IsDamagePool=	FALSE;

	packetRush.base.set(sizeof(packetRush),dSG_RUSH);
	packetRush.wResult	=	eRR_ARRIVE;
	packetRush.wActor	=	_lpCaster->m_wSerialInField;
	packetRush.wSkill	=	_lpAbility->m_wSkill;
	packetRush.wLevel	=	_lpAbility->m_wLevel;
	packetRush.wFrame	=	_lpCaster->m_iFrame;

	if	(_lpCaster->m_iShakleHP)
	{
		packetRush.wResult		=	eRR_BLOCKED;
		packetRush.wArrivePosX	=	_lpCaster->m_pos.x;
		packetRush.wArrivePosY	=	_lpCaster->m_pos.y;

		addSendPacket((ALL_MSG*)&packetRush,_lpCaster->m_wSerialInField);

		_lpCaster->stop();
		_lpCaster->m_wPPS			=	iPPS;

		g_debugSign.m_iExplosionSkillStep			=	-1;
		return	TRUE;
	}

	packetThrustActor.wCount	=	0;

	g_debugSign.m_iExplosionSkillStep			=	3;

	for	(int iLoop=1000;iLoop;)
	{
		--iLoop;

		cPOINT	posOld;

		posOld.x	=	_lpCaster->m_pos.x;
		posOld.y	=	_lpCaster->m_pos.y;

		for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
		{
			if	(iTeam == _lpCaster->m_wTeam)
				continue;

			for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
			{
				if	(packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
					continue;

				int		iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
				cACTOR *lpActor	=	getTestedActor(iSerial);

				if	(_lpTarget && bIsAttackOnlyTargetRush && iSerial != _lpTarget->m_wSerialInField)
					continue;

				if	(!lpActor || lpActor->isDeath() || lpActor->m_wSerialInField == _lpCaster->m_wSerialInField)
					continue;

				BOOL	bIsDamagedActor	=	FALSE;

				if	(!lpActor->isTargetInAttackRange(_lpCaster,iHitRange))
					continue;

				int	iAngle	=	g_protractor.getAngleToTarget(posBegin.x,posBegin.y,lpActor->m_pos.x,lpActor->m_pos.y);

				if	(iAngle+360	<	iAngleToTarget+306-90	||	iAngle+360	>	iAngleToTarget+306+90)
				{
					if	(!_lpTarget ||	_lpTarget->m_wSerialInField	!=	iSerial)
						continue;
				}

				for (int j=0;j<packetWideAreaDamage.bf6Count;j++)
				{
					if	(packetWideAreaDamage.aData[j].wActor == iSerial)
					{
						bIsDamagedActor	=	TRUE;
						break;
					}
				}

				if	(bIsDamagedActor)
					break;

				if	(_lpInfo->isDamagedActor(iSerial))
					continue;

				_lpInfo->addActor(iSerial);

				BOOL	bIsApplyPhysicalDamage	=	TRUE,bArrived=FALSE;

				if	(bIsApplyPhysicalDamageOnlyTarget)
				{
					if	(lpActor->m_wSerialInField != wTargetSerial)
						bIsApplyPhysicalDamage	=	FALSE;
					else
						bArrived				=	TRUE;
				}

				lpActor->getDamage(_lpCaster,_lpAbility,_iX,_iY,&hitInfo,bIsApplyPhysicalDamage,TRUE);

				//	여기서 속성 데미지도 처리 할것!!
				if	(isGotedCP ==	FALSE && hitInfo.isHit())
				{
					isGotedCP				=	TRUE;
					_lpInfo->m_wIsGotedCP	=	TRUE;
					_lpCaster->increaseCP(_lpAbility->getGetCP());
				}

				if	(iWideAreaAttackDamagePercent != 655)
					if	(_lpTarget == NULL || lpActor->m_wSerialInField != _lpTarget->m_wSerialInField)
					{
						LONGLONG	llDamage;

						llDamage	=	hitInfo.m_dwMagicDamage;
						hitInfo.m_dwMagicDamage		=	(DWORD)(llDamage*iWideAreaAttackDamagePercent/100);

						llDamage	=	hitInfo.m_dwPhysicalDamage;
						hitInfo.m_dwPhysicalDamage	=	(DWORD)(llDamage*iWideAreaAttackDamagePercent/100);
					}

				if	(hitInfo.getDamage())
					_lpCaster->strike(lpActor,&hitInfo,_lpAbility,TRUE,TRUE,_iX,_iY);

				if	(iNockBackDistance && hitInfo.isHit()	&&	lpActor->m_bf1IsImmuneKnockBack	==	FALSE)
				{
					if	(lpActor->thrust(_lpCaster->m_pos.x,_lpCaster->m_pos.y,iNockBackDistance))
					{
						packetThrustActor.aActor[packetThrustActor.wCount].bf11SerialInField=	iSerial;
						packetThrustActor.aActor[packetThrustActor.wCount].bf2Team			=	lpActor->m_wTeam;
						packetThrustActor.aActor[packetThrustActor.wCount].wX				=	lpActor->m_pos.x;
						packetThrustActor.aActor[packetThrustActor.wCount].wY				=	lpActor->m_pos.y;

						packetThrustActor.wCount++;
					}

				}

				cWIDE_AREA_DAMAGE_E	*lpInfo		=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

				lpInfo->wActor					=	lpActor->m_wSerialInField;
				lpInfo->wPhysicalDamage			=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
				lpInfo->wMagicDamage			=	(WORD)(hitInfo.m_dwMagicDamage/100);
				lpInfo->dwResultField			=	hitInfo.m_dwResultField;

				packetWideAreaDamage.bf6Count++;

				if	(_bIsIllusion == FALSE)
					if	(!bIsPenetrationRush || bArrived)
					{
						bIsFindVictim				=	FALSE;
						packetRush.wResult			=	eRR_CRUSH;
						break;
					}
			}
		}

		int	iMoveResult	=	_lpCaster->updateMove(FALSE);

		if	(iMoveResult	==	eRUM_BLOCKED)
		{
			if	(packetWideAreaDamage.bf6Count)
				packetRush.wResult	=	eRR_CRUSH_AND_BLOCKED;
			else
				packetRush.wResult	=	eRR_BLOCKED;
			break;
		}

		if	(iMoveResult	==	eRUM_ARRIVE)
			break;

		if	(packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
			break;

		if	(posOld.x	==	_lpCaster->m_pos.x	&&	posOld.y	==	_lpCaster->m_pos.y)
			break;
	}

	g_debugSign.m_iExplosionSkillStep			=	4;

	if (_lpCaster && _lpTarget)
	{
		int	iCurRange	=	GetOvalRange(_lpTarget->m_pos.x,_lpTarget->m_pos.y,_lpCaster->m_pos.x,_lpCaster->m_pos.y);
		int	iRange		=	_lpTarget->getBodySize()+_lpCaster->getBodySize();

		iCurRange		=	(int)sqrt(iCurRange);

		if (iCurRange	<	iRange)
		{
			cPOINT	pos;

			pos.x		=	_lpTarget->m_pos.x;
			pos.y		=	_lpTarget->m_pos.y;

			g_protractor.getTargetPos(&pos,posBegin.x,posBegin.y,iRange);

			_lpCaster->m_pos.x	=	pos.x;
			_lpCaster->m_pos.y	=	pos.y;
		}
	}

	g_debugSign.m_iExplosionSkillStep			=	5;

	_lpCaster->stop();

	packetRush.wArrivePosX			=	_lpCaster->m_pos.x;
	packetRush.wArrivePosY			=	_lpCaster->m_pos.y;

	_lpPosArrive->x					=	_lpCaster->m_pos.x;
	_lpPosArrive->y					=	_lpCaster->m_pos.y;

	if (_bIsIllusion == FALSE)
	{
		addSendPacket((ALL_MSG*)&packetRush,_lpCaster->m_wSerialInField);
		_lpCaster->setActorPos(_lpCaster->m_pos.x,_lpCaster->m_pos.y,TRUE,TRUE);
	}

	_lpCaster->m_wPPS			=	iPPS;

	g_debugSign.m_iExplosionSkillStep			=	6;

	if (packetThrustActor.wCount)
	{
		packetThrustActor.wAxisXPos	=	_lpCaster->m_pos.x;
		packetThrustActor.wAxisYPos	=	_lpCaster->m_pos.y;
		packetThrustActor.wSkill	=	_lpAbility->m_wSkill;
		packetThrustActor.base.set(sizeof(SG_THRUST_ACTORS)-sizeof(packetThrustActor.aActor),dSG_THRUST_ACTORS);
		packetThrustActor.base.wSize+=	packetThrustActor.wCount*sizeof(CMinimumActorInfo);

		addSendPacket((ALL_MSG*)&packetThrustActor,0xffff);
	}

	if (packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);
	}

	g_debugSign.m_iExplosionSkillStep			=	-7;

	return	TRUE;
}

//
//	러쉬 스킬 처리
BOOL
cFIELD::getRushPoint(cACTOR *_lpCaster,cACTOR *_lpTarget,int _iX,int _iY,cAbility *_lpAbility,cPOINT *_lpPosArrive)
{
	int		iPPS				=	_lpCaster->m_wPPS,iTeam,i;
	CSkill	*lpSkill			=	_lpAbility->getSkill();
	BOOL	bIsFindVictim		=	TRUE;
	BOOL	bIsAttackOnlyTargetRush	=	lpSkill->m_bf1IsAttackOnlyTargetRush;
	SG_RUSH	packetRush;
	BOOL	bIsApplyPhysicalDamageOnlyTarget =	_lpAbility->isApplyPhysicalDamageOnlyTarget();
	WORD	wTargetSerial;
	int		iHitRange			=	_lpAbility->getHitRange(_lpCaster->getWeapon())+_lpCaster->getBodySize();
	BOOL	bIsPenetrationRush	=	lpSkill->m_bf1IsPenetrationRush;
	int		iVitimCount			=	0;
	cPOINT	posRemember;
	int		iLocalPart			=	getCurrentLocalPart(_iX,_iY);

	posRemember.x	=	_lpCaster->m_pos.x;
	posRemember.y	=	_lpCaster->m_pos.y;

	if	(_lpTarget)
		wTargetSerial	=	_lpTarget->m_wSerialInField;

	_lpCaster->m_wPPS	=	c_iRushSkillSpeed;
	_lpCaster->makePath(_iX,_iY);


	packetRush.base.set(sizeof(packetRush),dSG_RUSH);
	packetRush.wResult	=	eRR_ARRIVE;
	packetRush.wActor	=	_lpCaster->m_wSerialInField;
	packetRush.wSkill	=	_lpAbility->m_wSkill;
	packetRush.wLevel	=	_lpAbility->m_wLevel;
	packetRush.wFrame	=	_lpCaster->m_iFrame;

	while(bIsFindVictim)
	{
		int	iMoveResult	=	_lpCaster->updateMove(FALSE);

		if	(iMoveResult	==	eRUM_BLOCKED)
		{
			if	(iVitimCount)
				packetRush.wResult	=	eRR_CRUSH_AND_BLOCKED;
			else
				packetRush.wResult	=	eRR_BLOCKED;
			break;
		}

		if	(iMoveResult	==	eRUM_ARRIVE)
			break;

		if	(bIsAttackOnlyTargetRush)
			continue;

		for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
		{
			if	(iTeam == _lpCaster->m_wTeam)
				continue;

			for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
			{
				int		iSerial	=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
				cACTOR *lpActor	=	getTestedActor(iSerial);

				if	(!lpActor || lpActor->isDeath() || lpActor->m_wSerialInField == _lpCaster->m_wSerialInField)
					continue;

				BOOL	bIsDamagedActor	=	FALSE;

				if	(!lpActor->isTargetInAttackRange(_lpCaster,iHitRange))
					continue;
				BOOL	bArrived=FALSE;

				if	(bIsApplyPhysicalDamageOnlyTarget)
					if	(lpActor->m_wSerialInField == wTargetSerial)
						bArrived	=	TRUE;

				iVitimCount++;

				if	(!bIsPenetrationRush || bArrived)
				{
					bIsFindVictim				=	FALSE;
					packetRush.wResult			=	eRR_CRUSH;
					break;
				}
			}
		}
	}

	packetRush.wArrivePosX			=	_lpCaster->m_pos.x;
	packetRush.wArrivePosY			=	_lpCaster->m_pos.y;
	_lpPosArrive->x					=	_lpCaster->m_pos.x;
	_lpPosArrive->y					=	_lpCaster->m_pos.y;

	addSendPacket((ALL_MSG*)&packetRush,_lpCaster->m_wSerialInField);

	_lpCaster->setActorPos(posRemember.x,posRemember.y,TRUE,TRUE);

	_lpCaster->m_wPPS				=	iPPS;

	return	TRUE;
}

BOOL
cFIELD::strikeWideAreaMagicDamage(cACTOR *_lpCaster,int _iX,int _iY,BOOL _bIsWantApplyEE,cAbility *_lpAbility,
								  int _iRange,int _iFireDamage,int _iWaterDamage,int _iWindDamage,int _iEarthDamage,int _iLightDamage,int _iDarkDamage,int _iTeam,
								  int *_lpiDeathTargetCount,BOOL _bIsByMiniPet)
{
	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;
	CHitInfo				hitInfo;
	int						iX=_lpCaster->m_pos.x,iY=_lpCaster->m_pos.y;

	if	(_iX	!=	-1)
	{
		iX	=	_iX;
		iY	=	_iY;
	}

	int						iLocalPart	=	getCurrentLocalPart(iX,iY);
	int						i,iDamagedActorCount	=	0,iTeam;

	packetWideAreaDamage.bf6Count			=	0;
	packetWideAreaDamage.bf1IsDamagePool	=	FALSE;

	packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
	packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
	packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
	packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

	packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
	packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

	if	(_iTeam	==	-1)
		_iTeam	=	_lpCaster->m_wTeam;

	if	(_lpiDeathTargetCount)
		*_lpiDeathTargetCount	=	0;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam == _iTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			int		iSerial		=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			cACTOR	*lpActor	=	getTestedActor(iSerial);

			if	(!lpActor || lpActor->isDeath()	)
				continue;

			if	(lpActor->m_bIsGuildObject	||	lpActor->m_iGuildGuardianType	||	lpActor->m_wInMagicBoxTime	||	lpActor->m_wInvincibleTime)
				continue;

			if	(!lpActor->isInAttackRange(iX,iY,_iRange))
				continue;

			if	(isMissileBlockedLine(iX,iY,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			hitInfo.reset(TRUE);

			int iFireDamage		=	_iFireDamage;
			int iWaterDamage	=	_iWaterDamage;
			int iWindDamage		=	_iWindDamage;
			int iEarthDamage	=	_iEarthDamage;
			int iLightDamage	=	_iLightDamage;
			int iDarkDamage		=	_iDarkDamage;

			int iCorrectResistance	=	0;

			_lpCaster->iMiniPet()->operateVSBossCorrectMagicPower(lpActor,&iFireDamage	,&iWaterDamage,&iWindDamage,&iEarthDamage,&iLightDamage,&iDarkDamage);
			iCorrectResistance	=	_lpCaster->iMiniPet()->getVSBossCorrectTargetMagicResistance(lpActor);

			if	(iFireDamage)
				iFireDamage	-=	_iFireDamage*lpActor->getFireResistance(iCorrectResistance)/100;
			if	(iWaterDamage)
				iWaterDamage-=	_iWaterDamage*lpActor->getWaterResistance(iCorrectResistance)/100;
			if	(iWindDamage)
				iWindDamage	-=	_iWindDamage*lpActor->getWindResistance(iCorrectResistance)/100;
			if	(iEarthDamage)
				iEarthDamage-=	_iEarthDamage*lpActor->getEarthResistance(iCorrectResistance)/100;
			if	(iLightDamage)
				iLightDamage-=	_iLightDamage*lpActor->getLightResistance(iCorrectResistance)/100;
			if	(iDarkDamage)
				iDarkDamage	-=	_iDarkDamage*lpActor->getDarkResistance(iCorrectResistance)/100;

			int	iDamage	=	iFireDamage+iWaterDamage+iWindDamage+iEarthDamage+iLightDamage+iDarkDamage;

			if	(isPVP(_lpCaster,lpActor))
				iDamage	=	GetP2PCorrectDamage(iDamage);

			if	(lpActor->m_wCorrectFinalDamage	!=	10000)
			{
				LONGLONG	llDamage;
				llDamage	=	iDamage;
				llDamage	=	llDamage*lpActor->m_wCorrectFinalDamage/10000;
				iDamage		=	(DWORD)llDamage;
			}

			lpActor->reduceHP(iDamage,_lpCaster,0,FALSE,_bIsByMiniPet);
			lpActor->operateRevenge(_lpCaster,_lpAbility);

			if	(_lpiDeathTargetCount	&&	lpActor->isDeath())
				*_lpiDeathTargetCount++;

			if	(_bIsWantApplyEE)
				lpActor->applySkillExtraEffects(_lpCaster,_lpAbility);

			hitInfo.setHit();
			hitInfo.m_dwMagicDamage	=	iDamage;

			cWIDE_AREA_DAMAGE_E	*lpInfo					=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

			lpInfo->wActor								=	lpActor->m_wSerialInField;
			lpInfo->wPhysicalDamage						=	0;
			lpInfo->wMagicDamage						=	(WORD)(iDamage/100);
			lpInfo->dwResultField						=	hitInfo.m_dwResultField;

			packetWideAreaDamage.bf6Count++;
			iDamagedActorCount++;

			if	(packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
			{
				addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);
				packetWideAreaDamage.bf6Count	=	0;
			}
		}
	}

	if	(packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);
	}

	if	(iDamagedActorCount)
		return	TRUE;

	return	FALSE;
}

//
//	범위 데미지 입힌다.
BOOL
cFIELD::strikeWideAreaDamageSkill(cACTOR *_lpCaster,cAbility *_lpAbility,cPOINT *_lpPos,int _iRange,WORD *_lpDamagedActorList,int _iListSize,BOOL *_bIsGetedCp,CHitInfo *_lpDamageSum)
{
	SG_WIDE_AREA_DAMAGE		packetWideAreaDamage;
	CHitInfo				hitInfo;
	int						iLocalPart	=	getCurrentLocalPart(_lpCaster->m_pos.x,_lpCaster->m_pos.y);

	int		i,iDamagedActorCount	=	0,iTeam;
	BOOL	bIsApplyPhysicalDamageOnlyTarget =	_lpAbility->isApplyPhysicalDamageOnlyTarget();
	int		iMinRange				=	_lpAbility->getMinimumHitRange();

	CSkill *lpSkill = _lpAbility->getSkill();

	if	(_lpDamagedActorList)
		while (_lpDamagedActorList[iDamagedActorCount]!=0xffff)
			iDamagedActorCount++;

	packetWideAreaDamage.bf6Count			=	0;
	packetWideAreaDamage.bf1IsDamagePool	=	FALSE;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam == _lpCaster->m_wTeam)
			continue;

		for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			if	(packetWideAreaDamage.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
				break;
			if	(_lpDamagedActorList	&&	iDamagedActorCount		>=	_iListSize		)
				break;

			int		iSerial		=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
			cACTOR	*lpActor	=	getTestedActor(iSerial);
			BOOL	isDamaged	=	FALSE;

			if	(!lpActor || lpActor->isDeath()	)
				continue;

			if	(lpActor->m_wJumppingSkippingTime	||	lpActor->m_wTransToFrogTime	||	lpActor->m_wHwabyungTime)
				continue;

			if	(!lpActor->isInAttackRange(_lpPos->x,_lpPos->y,_iRange))
				continue;

			if	(iMinRange)
				if	(lpActor->isInAttackRange(_lpPos->x,_lpPos->y,iMinRange))
					continue;

			if	(_lpDamagedActorList)
				for (int iIndex=0;iIndex<iDamagedActorCount;iIndex++)
				{
					if	(_lpDamagedActorList[iIndex]	==	iSerial)
					{
						isDamaged	=	TRUE;

						break;
					}
				}

			if (isDamaged	)
				continue;

			if (isMissileBlockedLine(_lpCaster->m_pos.x,_lpCaster->m_pos.y,lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			lpActor->getDamage(_lpCaster,_lpAbility,0,0,&hitInfo); 

			if	(_lpDamageSum)
			{
				_lpDamageSum->m_dwMagicDamage	+=	hitInfo.m_dwMagicDamage;
				_lpDamageSum->m_dwPhysicalDamage+=	hitInfo.m_dwPhysicalDamage;
			}

			if (!*_bIsGetedCp && hitInfo.isHit())
			{
				*_bIsGetedCp	=	TRUE;
				_lpCaster->increaseCP(_lpAbility->getGetCP());
			}

			_lpCaster->strike(lpActor,&hitInfo,_lpAbility,TRUE,TRUE,_lpPos->x,_lpPos->y);

			cWIDE_AREA_DAMAGE_E	*lpInfo					=	&packetWideAreaDamage.aData[packetWideAreaDamage.bf6Count];

			lpInfo->wActor								=	lpActor->m_wSerialInField;
			lpInfo->wPhysicalDamage						=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
			lpInfo->wMagicDamage						=	(WORD)(hitInfo.m_dwMagicDamage/100);
			lpInfo->dwResultField						=	hitInfo.m_dwResultField;

			if (_lpDamagedActorList)
				_lpDamagedActorList[iDamagedActorCount++]	=	lpActor->m_wSerialInField;

			packetWideAreaDamage.bf6Count++;
		}
	}

	if (packetWideAreaDamage.bf6Count)
	{
		packetWideAreaDamage.bf12Caster	=	_lpCaster->m_wSerialInField;
		packetWideAreaDamage.bf12Skill	=	_lpAbility->m_wSkill;
		packetWideAreaDamage.bf8Level	=	_lpAbility->m_wLevel;
		packetWideAreaDamage.sRemainCP	=	_lpCaster->m_iCP/100;

		packetWideAreaDamage.base.set(sizeof(packetWideAreaDamage),dSG_WIDE_AREA_DAMAGE);
		packetWideAreaDamage.base.wSize	=	sizeof(packetWideAreaDamage)-sizeof(packetWideAreaDamage.aData)+packetWideAreaDamage.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		addSendPacket((ALL_MSG*)&packetWideAreaDamage,_lpCaster->m_wSerialInField);
	}

	if	(_lpDamagedActorList	&&	iDamagedActorCount	>=	_iListSize	)
		return	TRUE;

	return	FALSE;
}

//
//	챠지 스킬 부가효과 적용
BOOL
cFIELD::operateSkillExtraEffectForOwnTeam(int _iX,int _iY,cACTOR *_lpCaster,cAbility *_lpAbility)
{
	int		iRange		=	_lpAbility->getHitRange();
	int		iLocalPart	=	getCurrentLocalPart(_iX,_iY);

	if (iRange	==	0)
	{
		if (!_lpCaster->applySkillExtraEffects(_lpCaster,_lpAbility,0,_iX,_iY))
		{
			_lpCaster->addPacketMiss(_lpAbility->m_wSkill);

			return	FALSE;
		}

		return	TRUE;
	}

	int		i,iTeam=_lpCaster->m_wTeam;

	for (i=m_pLocalPartition[iLocalPart].m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
	{
		int		iSerial		=	m_pLocalPartition[iLocalPart].m_apActorList[iTeam][i-1];
		cACTOR	*lpActor	=	getTestedActor(iSerial);

		if (!checkActor(lpActor) || lpActor->isDeath()	)
			continue;

		if (!lpActor->isInAttackRange(_lpCaster->m_pos.x,_lpCaster->m_pos.y,iRange))
			continue;

		if (lpActor->applySkillExtraEffects(_lpCaster,_lpAbility,0,_iX,_iY))
		{
			}
	}

	return	TRUE;
}

//
//	_iSerial한테 미러 타워를 받은 넘들 수
int
cFIELD::getMirrorTowerReceiverCount(cACTOR *_lpCaster)
{
	int		i,iReceiverCount=0,iTeam;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam != _lpCaster->m_wTeam)
			continue;

		for (i=m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			int		iSerial		=	m_awActorList[iTeam][i-1];
			cACTOR	*lpActor	=	getTestedActor(iSerial);

			if (!checkActor(lpActor))	continue;

			if (lpActor->m_wMirrorTowerCaster == _lpCaster->m_wSerialInField)
				iReceiverCount++;
		}
	}

	return	iReceiverCount;
}

//
//	미러타워 처리
int
cFIELD::operateMirrorTower(cACTOR *_lpAttacker,cACTOR *_lpReceiver,cAbility *_lpAbility,int _iDamage)
{
	cACTOR	*lpCaster	=	getTestedActor(_lpReceiver->m_wMirrorTowerCaster);

	if (!checkActor(lpCaster) || lpCaster->isDeath())
	{
		_lpReceiver->m_wMirrorTowerCaster		=	0xffff;
		_lpReceiver->m_wMirrorTowerUpkeepTime	=	0;

		return	_iDamage;
	}

	if (!lpCaster->isTargetInAttackRange(_lpReceiver,_lpReceiver->m_wMirrorTowerRange))
		return	_iDamage;	//	거리가 안된다.

	int		iHitDamage	=	_iDamage;
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	int	iReturnDamage	=	_iDamage*(100-_lpReceiver->m_wMirrorTowerFactor)/100;
	int	iSharedDamage	=	_iDamage*_lpReceiver->m_wMirrorTowerFactor/100;
	int	iMagicResistance=	min(lpCaster->getAllMagicResistance(),90);

	if (lpSkill->isFireDamageSkill())
		iMagicResistance	=	lpCaster->getFireResistance();
	else
	if (lpSkill->isWaterDamageSkill())
		iMagicResistance	=	lpCaster->getWaterResistance();
	else
	if (lpSkill->isWindDamageSkill())
		iMagicResistance	=	lpCaster->getWindResistance();
	else
	if (lpSkill->isEarthDamageSkill())
		iMagicResistance	=	lpCaster->getEarthResistance();
	else
	if (lpSkill->isLightDamageSkill())
		iMagicResistance	=	lpCaster->getLightResistance();
	else
	if (lpSkill->isDarkDamageSkill())
		iMagicResistance	=	lpCaster->getDarkResistance();

	if (_lpReceiver->m_wMirrorTowerCaster	==	_lpReceiver->m_wSerialInField)
		iMagicResistance	=	0;

	iSharedDamage		=	iSharedDamage*(100-iMagicResistance)/100;
	int	iIncreaseCP		=	iReturnDamage*_lpReceiver->m_wMirrorTowerCPValue/100;

	lpCaster->damagedBy(_lpAttacker,_lpAbility,-1,iSharedDamage,FALSE,FALSE);
	lpCaster->increaseCP(iIncreaseCP);

	SG_ACTIVE_MIRROR_TOWER	packet;

	packet.base.set(sizeof(SG_ACTIVE_MIRROR_TOWER),dSG_ACTIVE_MIRROR_TOWER);

	packet.wCaster			=	lpCaster->m_wSerialInField;
	packet.wTarget			=	_lpReceiver->m_wSerialInField;
	packet.wTargetRecoverHP	=	iHitDamage-iSharedDamage;
	packet.wSkill			=	_lpReceiver->m_wMirrorTowerSkill;
	packet.sRemainCP		=	lpCaster->m_iCP/100;
	packet.wMagicDamage		=	iSharedDamage/100;

	if (iSharedDamage)
		packet.wMagicDamage		=	max(packet.wMagicDamage,1);

	addSendPacket((ALL_MSG *)&packet,packet.wCaster);

	if (lpSkill->isFireDamageSkill())
		iReturnDamage	=	iReturnDamage*(100-_lpReceiver->getFireResistance())/100;
	else
	if (lpSkill->isWaterDamageSkill())
		iReturnDamage	=	iReturnDamage*(100-_lpReceiver->getWaterResistance())/100;
	else
	if (lpSkill->isWindDamageSkill())
		iReturnDamage	=	iReturnDamage*(100-_lpReceiver->getWindResistance())/100;
	else
	if (lpSkill->isEarthDamageSkill())
		iReturnDamage	=	iReturnDamage*(100-_lpReceiver->getEarthResistance())/100;
	else
	if (lpSkill->isLightDamageSkill())
		iReturnDamage	=	iReturnDamage*(100-_lpReceiver->getLightResistance())/100;
	else
	if (lpSkill->isDarkDamageSkill())
		iReturnDamage	=	iReturnDamage*(100-_lpReceiver->getDarkResistance())/100;

	return	iReturnDamage;
}

void
cFIELD::changeTeam(cACTOR *_lpActor,int _iTeam)
{
	if	(_lpActor->m_wTeam	==	_iTeam)
		return;

	if	(m_iUniqueSerial < 900)
		if (_iTeam	==	2 || _lpActor->m_wTeam	==	2)
		{
			CLOG("npc","cFIELD::changeTeam is npc team!!");

			return;
		}

	if	(!removeActorInList(_lpActor->m_wSerialInField,_lpActor->m_wTeam,_lpActor->m_iSmallLocalPart))
		return;

	addActorAtList(_lpActor->m_wSerialInField,_iTeam,_lpActor->m_iSmallLocalPart);

	_lpActor->m_wTeam	=	_iTeam;
	_lpActor->stop();
	_lpActor->addPacketStop();
	_lpActor->setTarget(NULL);
}

void
cFIELD::displacement(cACTOR *_lpCaster,cACTOR *_lpTarget)
{
	if	(_lpTarget->m_wCharmedTime)
		return;

	if	(_lpTarget->m_wLordActor	!=	0xffff	||	_lpTarget->m_wLordOperator	!=	0xffff)
		return;

	_lpCaster->sendDisplacement(_lpTarget,TRUE);

	_lpCaster->m_wIsFeignDeath	=	TRUE;
	_lpCaster->m_wSlaveActor	=	_lpTarget->m_wSerialInField;
	_lpTarget->m_wLordActor		=	_lpCaster->m_wSerialInField;
	_lpTarget->m_dwLordHashCode	=	_lpCaster->m_dwNameHashCode;
	_lpCaster->m_posFeignDeath.x=	_lpCaster->m_pos.x;
	_lpCaster->m_posFeignDeath.y=	_lpCaster->m_pos.y;
	_lpCaster->setActorPos(_lpTarget->m_pos.x,_lpTarget->m_pos.y,TRUE,TRUE);
	_lpCaster->sendCurrentPosition();

	stopAllTargetedAction(_lpCaster);
	stopAllTargetedAction(_lpTarget);

	_lpCaster->stop();
	_lpTarget->setAiReady();

	_lpCaster->m_wRemainActionTime	=	0;
	_lpTarget->m_wRemainActionTime	=	0;

	changeTeam(_lpTarget,_lpCaster->m_wTeam);
}

void
cFIELD::releaseDisplacement(cACTOR *_lpCaster,cACTOR *_lpTarget)
{
	if (_lpCaster)
	{
		_lpCaster->m_wIsFeignDeath	=	FALSE;
		_lpCaster->m_wSlaveActor	=	0xffff;
		_lpCaster->stop();
		_lpCaster->stopBattle();

		stopAllTargetedAction(_lpCaster);

		_lpCaster->sendDisplacement(_lpTarget,FALSE);
		_lpCaster->setActorPos(_lpCaster->m_posFeignDeath.x,_lpCaster->m_posFeignDeath.y,TRUE,TRUE);
		_lpCaster->sendCurrentPosition();
	}

	if (_lpTarget)
	{
		_lpTarget->m_wLordActor		=	0xffff;
		_lpTarget->setAiReady();

		stopAllTargetedAction(_lpTarget);
		changeTeam(_lpTarget,_lpTarget->m_wOldTeam);
	}
}

void
cFIELD::stopAllTargetedAction(cACTOR *_lpTarget,BOOL _bIsByInvisible,BOOL _bIsReleaseFocus)
{
	int	i,iTeam;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		for (i=m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			int		iSerial		=	m_awActorList[iTeam][i-1];
			cACTOR	*lpActor	=	getTestedActor(iSerial);

			if	(!lpActor)
				continue;

			cACTOR	*lpTarget	=	lpActor->m_lpTarget;

			if	(lpTarget == NULL)
				continue;

			if	(_bIsByInvisible)
				if	(lpActor->getDetectingLevel() >= _lpTarget->getHidingLevel())
					continue;

			if	(lpTarget->m_wSerialInField == _lpTarget->m_wSerialInField)
			{
				lpActor->stop();
				lpActor->setTarget(NULL);
				lpActor->setAiReady();

				if	(lpActor->isPlayer())
					lpActor->sendStopBattle(_bIsReleaseFocus);
			}
		}
	}
}

void
cFIELD::stopAllHostileAction(cACTOR *_lpTarget)
{
	int	i,iTeam;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam == _lpTarget->m_wTeam)
			continue;

		for (i=m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			int		iSerial		=	m_awActorList[iTeam][i-1];;
			cACTOR	*lpActor	=	getTestedActor(iSerial);

			if (!lpActor)
				continue;

			cACTOR	*lpTarget	=	lpActor->m_lpTarget;

			if (lpTarget == NULL)
				continue;

			if (lpTarget->m_wSerialInField == _lpTarget->m_wSerialInField)
			{
				lpActor->stop();
				lpActor->setTarget(NULL);

				if (lpActor->isPlayer())
					lpActor->sendStopBattle();
			}
		}
	}
}

int
cFIELD::detectingEvil(cACTOR *_lpCaster,int _iRange,int _iLimitCount,int _iLimitLevel,int _iLimitType)
{
	SG_DETECTING_EVIL	packet;

	int	iLocal		=	_lpCaster->m_iLocalPart;
	packet.wCount	=	0;
	packet.base.wType	=	dSG_DETECTING_EVIL;
	packet.base.wSize	=	sizeof(packet);

	if	(iLocal	>=	m_iLocalPartitionCount || !m_pLocalPartition)
		return	0;

	for (int	iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam == _lpCaster->m_wTeam)
			continue;

		CLocalPartionInfo	*lpPartion		=	&m_pLocalPartition[iLocal];
		int					iCheckActorCount=	lpPartion->m_aiActorCount[iTeam];

		iCheckActorCount	=	min(iCheckActorCount,lpPartion->m_aiBufferSize[iTeam]);

		for (int i=iCheckActorCount;i;i--)		//	적 캐릭터
		{
			int		iSerial		=	lpPartion->m_apActorList[iTeam][i-1];
			cACTOR	*lpActor	=	getTestedActor(iSerial);

			if	(!lpActor)
				continue;

			if	(lpActor->isMonster())
				if	(g_aJob[lpActor->m_wJobIndex].m_wMonsterLevel	>	_iLimitType)
					continue;

			if	(lpActor->getLevel()	>	_iLimitLevel)
				continue;

			if	(!lpActor->isTargetInAttackRange(_lpCaster,_iRange))
				continue;

			packet.aTarget[packet.wCount].bf16X			=	lpActor->m_pos.x;
			packet.aTarget[packet.wCount].bf15Y			=	lpActor->m_pos.y;
			packet.aTarget[packet.wCount].bf1IsMonster	=	lpActor->isMonster();

			packet.wCount++;

			if	(packet.wCount	>=	dMAX_DETECTING_EVIL_TARGET_COUNT)
			{
				g_userPM.add(_lpCaster->m_iClientSerial,(ALL_MSG*)&packet,packet.base.wSize);

				packet.wCount	=	0;
			}

			if (packet.wCount	>=	_iLimitCount)
				break;
		}
	}

	if	(packet.wCount)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aTarget)+sizeof(CDetectingEvilTargetInfo)*packet.wCount;

		g_userPM.add(_lpCaster->m_iClientSerial,(ALL_MSG*)&packet,packet.base.wSize);
	}

	return	packet.wCount;
}