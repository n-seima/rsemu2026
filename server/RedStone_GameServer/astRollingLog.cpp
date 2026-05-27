#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include "CGame.H"
#include	"astRollingLog.h"

//
//	아이템 던지기
int
cAS_RollingLog::fire()
{
	if	(m_lpCaster->isSealdMove())
		return	eSKILL_FIRE_RESULT_FAILED;
	
	m_lpCaster->action(m_lpSkill->m_wAction);

	m_wStep			=	eRL_READY;

	setCastingTimeByCasterDamageFrame(0);

	m_dwBookedActionTime	+=	100;	//	0.1초정도 여유를 두고 터진다

	return	eSKILL_FIRE_RESULT_OK;
}

//
//
BOOL
cAS_RollingLog::trans()
{
	m_wStep	=	eRL_TRANS;


	return	TRUE;
}

BOOL
cAS_RollingLog::strike()
{
	cPOINT	posX1Y1,posX2Y1,posX1Y2,posX2Y2,posDest;

	int		iHitRange	=	m_ability.getHitRange(NULL);
	int		iAngle		=	g_protractor.getAngleToTarget(m_lpCaster->m_pos.x,m_lpCaster->m_pos.y,m_posTarget.x,m_posTarget.y);
	int		iAngle1		=	iAngle+90;
	int		iAngle2		=	iAngle-90;

	if	(iAngle1	>=	360	)
		iAngle1	-=	360;
	if	(iAngle2	<	0	)
		iAngle2	+=	360;

	posDest.x	=	m_lpCaster->m_pos.x;
	posDest.y	=	m_lpCaster->m_pos.y;

	if	(m_lpTarget)
		g_protractor.getAnglePos(&posDest,iAngle,m_wRange+m_lpCaster->getBodySize()+m_lpTarget->getBodySize());
	else
		g_protractor.getAnglePos(&posDest,iAngle,m_wRange+m_lpCaster->getBodySize());

	posX1Y1		=	posDest;
	posX2Y1		=	posDest;
	g_protractor.getAnglePos(&posX1Y1,iAngle2,iHitRange/2);
	g_protractor.getAnglePos(&posX2Y1,iAngle1,iHitRange/2);

	posX1Y2.x	=	m_lpCaster->m_pos.x;
	posX1Y2.y	=	m_lpCaster->m_pos.y;
	posX2Y2		=	posX1Y2;
	g_protractor.getAnglePos(&posX1Y2,iAngle2,iHitRange/2);
	g_protractor.getAnglePos(&posX2Y2,iAngle1,iHitRange/2);

	m_lpCaster->strikeLaser(&m_ability,&posX1Y1,&posX2Y1,&posX2Y2,&posX1Y2);
	m_lpCaster->setActorPos(m_posTarget.x,m_posTarget.y,TRUE,TRUE);

	{
		SG_RUSH	packetRush;

		packetRush.base.set(sizeof(packetRush),dSG_RUSH);
		packetRush.wResult	=	eRR_ARRIVE;
		packetRush.wActor	=	m_lpCaster->m_wSerialInField;
		packetRush.wSkill	=	m_ability.m_wSkill;
		packetRush.wLevel	=	m_ability.m_wLevel;
		packetRush.wFrame	=	0;
		packetRush.wArrivePosX	=	m_posTarget.x;
		packetRush.wArrivePosY	=	m_posTarget.y;

		m_lpCaster->m_lpField->addSendPacket((ALL_MSG*)&packetRush,m_lpCaster->m_wSerialInField);
	}

	return	TRUE;
}

//
//	회오리형 업데이트
BOOL
cAS_RollingLog::update()
{
	if	(!isActionTime())
		return	FALSE;	//	아직 발동 직전이다.

	strike();

	return	TRUE;
}