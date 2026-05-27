#include	"ast_chain_lightning.h"
#include	"cACTOR.h"
#include	"cFIELD.h"

BOOL
cAS_ChainLightning::fire()
{
	m_wRelayJob		=	m_lpSkill->m_aValue[0];

	m_lpCaster->action(m_lpSkill->m_wAction);

	if	(m_lpSkill->m_aValue[1])
		m_lpCaster->m_lpField->addSendEtcWork(m_lpCaster->m_wSerialInField,eEW_CHANGE_SAGE_BOOK_COLOR,m_lpCaster->m_wSerialInField,m_lpSkill->m_aValue[1]);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
cAS_ChainLightning::casting()
{
	cFIELD	*lpField			=	m_lpCaster->m_lpField;
	cACTOR	*lpRelayTarget		=	lpField->getActorByJob(m_wRelayJob);

	if	(!lpRelayTarget)	//	세이지 마스터가 없다.
		return	FALSE;

	int		iShotCount			=	m_ability.getBulletCount(),i;
	int		iRange				=	m_ability.getAttackRange(NULL,0);

	WORD	awTargetList[100];

	int		iTargetCount		=	lpField->getEnemyActorListForChainLightning(awTargetList,lpRelayTarget,iRange,iShotCount+1);

	if	(iTargetCount	==	1)	//	세이지 마스터 밖에 없다.
		return	FALSE;

	SG_WIDE_AREA_DAMAGE	packet;

	packet.base.set(sizeof(packet),dSG_WIDE_AREA_DAMAGE);
	packet.bf12Caster		=	m_lpCaster->m_wSerialInField;
	packet.bf6Count			=	1;
	packet.bf1IsDamagePool	=	FALSE;
	packet.bf12Skill		=	m_ability.m_wSkill;
	packet.bf8Level			=	m_ability.m_wLevel;
	packet.sRemainCP		=	m_lpCaster->m_iCP/100;

	{
		cWIDE_AREA_DAMAGE_E	*lpInfo	=	&packet.aData[0];

		lpInfo->wActor				=	lpRelayTarget->m_wSerialInField;
		lpInfo->wPhysicalDamage		=	0;
		lpInfo->wMagicDamage		=	0;
		lpInfo->dwResultField		=	1;
	}

	for (i=1;i<iTargetCount;i++)
	{
		cACTOR	*lpTarget	=	lpField->getTestedActor(awTargetList[i]);

		if	(!lpTarget || lpTarget->isDeath())
			continue;

		CHitInfo			hitInfo;

		lpTarget->getDamage(m_lpCaster,&m_ability,m_pos.x,m_pos.y,&hitInfo);

		m_lpCaster->strike(lpTarget,&hitInfo,&m_ability,TRUE,FALSE);

		cWIDE_AREA_DAMAGE_E	*lpInfo	=	&packet.aData[packet.bf6Count];

		lpInfo->wActor				=	lpTarget->m_wSerialInField;
		lpInfo->wPhysicalDamage		=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
		lpInfo->wMagicDamage		=	(WORD)(hitInfo.m_dwMagicDamage/100);
		lpInfo->dwResultField		=	hitInfo.m_dwResultField;

		packet.bf6Count++;

		if	(packet.bf6Count		>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
			break;
	}

	lpField->addSendPacket((ALL_MSG*)&packet,m_lpCaster->m_wSerialInField);
	
	return	TRUE;
}

BOOL
cAS_ChainLightning::update()
{
	if	(!isActionTime())
		return	FALSE;

	casting();

	return	TRUE;
}
