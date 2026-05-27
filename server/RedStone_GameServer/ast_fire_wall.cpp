#include	"ast_fire_wall.h"
#include	"cACTOR.h"
#include	"cGAME.h"

BOOL
cAS_FireWall::fire()
{
	m_wSmallFlameTime	=	m_lpSkill->m_aValue[0];
	m_lpField			=	m_lpCaster->m_lpField;

	m_wGrade			=	eReady;
	m_wActivePeriod		=	m_lpSkill->m_sOccurActionPeriod;
	
	m_lpCaster->action(m_lpSkill->m_wAction);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
cAS_FireWall::makeSmallFlame()
{
	m_dwBookedActionTime	=	g_dwCurrentTime+m_wSmallFlameTime*10/dREVISE_ACTION_SPEED_FOR_TEST;
	m_wGrade				=	eSmall_Flame;

	m_lpField->addSendEtcWork(m_lpCaster->m_wSerialInField,eEW_MAKE_SMALL_FLAME,m_ability.m_wSkill,m_wX1,m_wY1,m_wX2,m_wY2);

	return	TRUE;
}

BOOL
cAS_FireWall::makeFireWall()
{
	m_dwBookedActionTime	=	g_dwCurrentTime+m_ability.getUpkeepTime()*1000/dREVISE_ACTION_SPEED_FOR_TEST;
	m_wGrade				=	eWall;

	m_lpField->makeBlock(m_wX1,m_wY1,m_wX2,m_wY2);
	m_lpField->addSendEtcWork(m_lpCaster->m_wSerialInField,eEW_MAKE_FIRE_WALL,m_ability.m_wSkill,m_wX1,m_wY1,m_wX2,m_wY2);

	strike();
	
	return	TRUE;
}

BOOL
cAS_FireWall::removeFireWall()
{
	m_lpField->removeBlock(m_wX1,m_wY1,m_wX2,m_wY2);
	m_lpField->addSendEtcWork(m_lpCaster->m_wSerialInField,eEW_REMOVE_FIRE_WALL,m_ability.m_wSkill,m_wX1,m_wY1,m_wX2,m_wY2);

	return	TRUE;
}

void
cAS_FireWall::strike()
{
	m_dwEngagedStrikeTime	=	g_dwCurrentTime+m_wActivePeriod*10;

	cPOINT	aRoad[100],pos;

	int	iNodeCount	=	g_protractor.getLine(m_wX1,m_wY1,m_wX2,m_wY2,aRoad,100),i;

	SG_WIDE_AREA_DAMAGE		packet;

	packet.base.set(sizeof(packet),dSG_WIDE_AREA_DAMAGE);
	packet.bf6Count			=	0;
	packet.bf1IsDamagePool	=	FALSE;
	packet.bf12Caster		=	m_lpCaster->m_wSerialInField;
	packet.bf12Skill		=	m_ability.m_wSkill;
	packet.bf8Level			=	m_ability.m_wLevel;
	packet.sRemainCP		=	0;

	WORD	awDamagedTargetList[200],iTargetCount=0;

	for(i=iNodeCount;i;)
	{
		--i;

		pos.x		=	aRoad[i].x*dTILE_XS+dTILE_XS/2;
		pos.y		=	aRoad[i].y*dTILE_YS+dTILE_YS/2;

		int	iCount	=	m_lpField->getInRangeEnemy(pos.x,pos.y,m_wTeam,m_ability.getHitRange(),awDamagedTargetList,iTargetCount,200);

		iTargetCount+=	iCount;

		if	(iTargetCount	>=	200)
			break;
	}

	for(i=iTargetCount;i;)
	{
		--i;

		cACTOR	*lpActor	=	m_lpField->getActor(awDamagedTargetList[i]);

		if	(!lpActor)
			continue;

		CHitInfo	hitInfo;

		lpActor->getDamage(m_lpCaster,&m_ability,0,0,&hitInfo);

		cWIDE_AREA_DAMAGE_E	*lpInfo					=	&packet.aData[packet.bf6Count++];

		lpInfo->wActor								=	lpActor->m_wSerialInField;
		lpInfo->wPhysicalDamage						=	(WORD)(hitInfo.m_dwPhysicalDamage/100);
		lpInfo->wMagicDamage						=	(WORD)(hitInfo.m_dwMagicDamage/100);
		lpInfo->dwResultField						=	hitInfo.m_dwResultField;

		m_lpCaster->strike(lpActor,&hitInfo,&m_ability,TRUE,TRUE);

		if	(packet.bf6Count	>=	dMAX_TARGET_FOR_WIDE_AREA_ATTACK)
		{
			m_lpField->addSendPacket((ALL_MSG*)&packet,m_lpCaster->m_wSerialInField);

			packet.bf6Count		=	0;
		}
	}

	if	(packet.bf6Count)
	{
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+packet.bf6Count*sizeof(cWIDE_AREA_DAMAGE_E);

		m_lpField->addSendPacket((ALL_MSG*)&packet,m_lpCaster->m_wSerialInField);
	}
}

BOOL
cAS_FireWall::update()
{
	if	(m_wGrade	==	eReady) 
	{
		if	(isActionTime()	==	FALSE)
			return	FALSE;

		makeSmallFlame();

		return	FALSE;
	}

	if	(m_wGrade	==	eSmall_Flame) 
	{
		if	(isActionTime()	==	FALSE)
			return	FALSE;

		makeFireWall();

		return	FALSE;
	}

	if	(m_wGrade	==	eWall)
	{
		if	(g_dwCurrentTime	>=	m_dwEngagedStrikeTime)
			strike();

		if	(isActionTime()	==	FALSE)
			return	FALSE;

		removeFireWall();

		return	TRUE;
	}

	return	TRUE;
}