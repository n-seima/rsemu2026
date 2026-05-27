#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include "CGame.H"
#include	"astBigBullet.h"

//
//	아이템 던지기
int
cAS_BigBullet::fire()
{
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wStep			=	eBBS_READY;

	setCastingTimeByCasterDamageFrame(0);

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	
BOOL
cAS_BigBullet::catchTarget()
{
	if	(m_lpCaster	==	NULL	||	m_lpTarget	==	NULL)
		return	TRUE;

	CHitInfo	hitInfo;

	m_lpTarget->getDamage(m_lpCaster,&m_ability,m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,&hitInfo);
	m_lpCaster->strike(m_lpTarget,&hitInfo,&m_ability);
	
	if	(m_lpTarget	==	NULL)
		return	TRUE;

	m_lpTarget->addSendHitDamage(m_lpCaster,hitInfo.m_dwPhysicalDamage,hitInfo.m_dwMagicDamage,hitInfo.m_dwResultField,m_ability.m_wSkill);

	int	iActiveChance	=	m_ability.getActivateChance();
	iActiveChance		=	iActiveChance*(100-m_lpTarget->getCurseResistance())/100;
	int	iRandValue		=	random(100);
	m_dwLastHitDamage	=	hitInfo.getDamage();

	if	(iRandValue	>=	iActiveChance)
		return	FALSE;

	m_lpCaster->m_lpField->addSendEtcWork(m_lpTarget->m_wSerialInField,eEW_CAUGHT,m_lpTarget->m_wSerialInField);

	m_wStep			=	eBBS_CAUGHT;

	setCastingTimeByCasterDamageFrame(1);

	int	iX	=	m_lpCaster->m_pos.x;
	int	iY	=	m_lpCaster->m_pos.y-20;

	if	(m_lpCaster->m_lpField->isBlocked(iX/dTILE_XS,iY/dTILE_YS))
		iY	=	m_lpCaster->m_pos.y;

	m_lpTarget->setActorPos(iX,iY);
	m_lpTarget->sendCurrentPosition();
	m_lpCaster->increaseCP(m_ability.getGetCP());

	m_lpTarget->m_wCaughtTime	=	6;

	return	TRUE;
}

//
//
BOOL
cAS_BigBullet::aimingNextTarget()
{
	cACTOR		*lpTarget;

	while(1)
	{
		lpTarget	=	m_lpTarget->m_lpField->getActor(m_awTargetList[m_wTargetIndex]);

		if	(lpTarget)
			break;

		m_wTargetIndex++;

		if	(m_wTargetIndex	>=	m_wTargetCount)
			return	FALSE;
	}

	int	iRange		=	GetOvalRange(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y);

	iRange			=	(int)sqrt(iRange);

	if	(iRange		>=	m_wAttackRange)
		return	FALSE;

	return	TRUE;
}

BOOL
cAS_BigBullet::hit()
{
	cAttackInfoForBigBullet	*lpAttackInfo	=	(cAttackInfoForBigBullet	*)&m_lpTarget->m_attackInfo;
	cACTOR					*lpTarget		=	m_lpTarget->m_lpField->getActor(m_awTargetList[m_wTargetIndex]);

	if	(lpTarget)
	{
		cMinHitInfoForBigBullet	*lpInfo		=	&lpAttackInfo->m_aInfo[m_wTargetIndex];
		CHitInfo				hitInfo;

		hitInfo.reset(TRUE);
		hitInfo.m_dwPhysicalDamage	=	lpInfo->m_wPhysicalDamage*100;
		hitInfo.m_dwMagicDamage		=	lpInfo->m_wMagicDamage*100;

		if	(lpInfo->m_wMagicDamage+lpInfo->m_wPhysicalDamage)
			hitInfo.setHit();

		m_lpCaster->strike(lpTarget,&hitInfo,&m_ability);

		m_lpTarget->setActorPos(lpTarget->m_pos.x,lpTarget->m_pos.y,TRUE,TRUE);
	}

	m_wTargetIndex++;

	if	(m_wTargetIndex	>=	m_wTargetCount)
		return	FALSE;

	return	aimingNextTarget();
}

BOOL
cAS_BigBullet::throwTarget()
{
	CActiveSkillDamagedActorInfo	info;

	int	iMaxCount	=	m_ability.getContinuousShotCount(FALSE),iHitCount=0;

	m_lpCaster->m_lpField->getBigBulletTarget(m_lpCaster,m_lpTarget,&m_ability,&info,iMaxCount);

	cAttackInfoForBigBullet	*lpAttackInfo	=	(cAttackInfoForBigBullet	*)&m_lpTarget->m_attackInfo;

	for (int i=0;i<info.m_wCount;i++)
	{
		cMinHitInfoForBigBullet	*lpInfo		=	&lpAttackInfo->m_aInfo[i];
		cACTOR					*lpTarget	=	m_lpCaster->m_lpField->getActor(info.m_awActorList[i]);

		lpInfo->reset();

		if	(lpTarget)
		{
			int			iDefensivePower	=	lpTarget->getDefensivePower();
			LONGLONG	llDamage		=	m_dwLastHitDamage;
			LONGLONG	llDivedeValue	=	max(llDamage+iDefensivePower,1);

			llDamage					=	llDamage*llDamage/llDivedeValue;

			int			iDamage			=	(int)llDamage;

			if	(iDamage	==	0)
				break;

			iDamage						=	max(iDamage,100);
			m_dwLastHitDamage			=	iDamage;

			m_awTargetList[iHitCount]	=	info.m_awActorList[i];
			lpAttackInfo->m_aInfo[iHitCount].m_wPhysicalDamage	=	iDamage/100;
			iHitCount++;

			lpTarget->operateRevenge(m_lpCaster,&m_ability);
		}
	}

	if	(iHitCount	==	0)
		return	FALSE;

	m_wAttackRange	=	m_ability.getHitRange(NULL)+20;
	m_wTargetIndex	=	0;
	m_wTargetCount	=	iHitCount;
	m_wStep			=	eBBS_THROW;

	{
		SG_WIDE_AREA_DAMAGE	packet;

		packet.base.set(sizeof(packet),dSG_WIDE_AREA_DAMAGE);

		packet.bf6Count		=	m_wTargetCount;
		packet.bf12Caster	=	m_lpCaster->m_wSerialInField;
		packet.bf12Skill	=	m_ability.m_wSkill;
		packet.bf8Level		=	m_ability.m_wLevel;
		packet.sRemainCP	=	m_lpCaster->m_iCP/100;
		packet.bf1IsDamagePool	=	FALSE;

		for (int i=0;i<packet.bf6Count;i++)
		{
			packet.aData[i].wActor			=	m_awTargetList[i];
			packet.aData[i].wPhysicalDamage	=	lpAttackInfo->m_aInfo[i].m_wPhysicalDamage;
			packet.aData[i].wMagicDamage	=	0;
			packet.aData[i].dwResultField	=	0;
		}

		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWIDE_AREA_DAMAGE_E)*packet.bf6Count;

		m_lpCaster->m_lpField->addSendPacket((ALL_MSG*)&packet,m_lpCaster->m_wSerialInField);
	}

	return	TRUE;
}

//
//	회오리형 업데이트
BOOL
cAS_BigBullet::update()
{
	if	(m_wStep	==	eBBS_THROW)
	{
		if	(!hit())
			return	TRUE;

		return	FALSE;
	}

	if	(!isActionTime())
		return	FALSE;	//	아직 발동 직전이다.

	if	(m_wStep	==	eBBS_READY)
	{
		if	(!catchTarget())
			return	TRUE;

		return	FALSE;
	}

	if	(m_wStep	==	eBBS_CAUGHT)
	{
		if	(!throwTarget())
			return	TRUE;

		return	FALSE;
	}

	return	TRUE;
}