#include "cACTOR.H"
#include "cFIELD.H"
#include "cNPC.H"
#include "debugCode.H"
#include "CGame.H"


//
//	업데이트
void
cACTOR::updateSlave()
{
	if (updateStatusByFrameForMonster()	)
		return;

	cACTOR	*lpLord	=	getLord();

	if (!lpLord)
	{
		m_lpField->bookingDisplacement(NULL,this,TRUE);

		return;
	}

	lpLord->setActorPos(m_pos.x,m_pos.y,TRUE);

	operateAi();
}

cACTOR*
cACTOR::getGuide()
{
	if (m_wGuideActor	==	0xffff)
		return	NULL;

	cACTOR	*lpGuide	=	m_lpField->getActor(m_wGuideActor);

	if (!lpGuide|| lpGuide->m_dwNameHashCode != m_dwGuideHashCode)
	{
		m_bf1IsTraceGuide	=	FALSE;
		m_wGuideActor		=	0xffff;
		return	NULL;
	}

	return	lpGuide;
}

void
cACTOR::setTraceGuide()
{
	setAiReady();
	addPacketStop();
	
	m_bf1IsTraceGuide	=	TRUE;
}

void
cACTOR::cancelTraceGuide()
{
	setAiReady();
	addPacketStop();

	m_bf1IsTraceGuide	=	FALSE;
}

void
cACTOR::setGuide(cACTOR *_lpActor)
{
	m_bf1IsTraceGuide	=	FALSE;

	if (_lpActor		==	NULL)
	{
		setAiReady();
		addPacketStop();

		m_wGuideActor		=	0xffff;
		m_dwGuideHashCode	=	0xffffffff;
		return;
	}

	m_wGuideActor		=	_lpActor->m_wSerialInField;
	m_dwGuideHashCode	=	_lpActor->m_dwNameHashCode;
}

cACTOR*
cACTOR::getMarionetteLord()
{
	if	(m_wMarionetteCaster	==	0xffff)
		return	NULL;

	cACTOR	*lpLord	=	m_lpField->getActor(m_wMarionetteCaster);

	if	(!lpLord || lpLord->m_dwNameHashCode != m_dwMarionetteCasterNameHashCode)
		return	NULL;

	return	lpLord;
}

cACTOR*
cACTOR::getLord()
{
	if	(m_wLordActor	==	0xffff)
		return	NULL;

	cACTOR	*lpLord	=	m_lpField->getActor(m_wLordActor);

	if (!lpLord || lpLord->m_dwNameHashCode != m_dwLordHashCode)
		return	NULL;

	return	lpLord;
}

cACTOR*
cACTOR::getCopyActorLord()
{
	if	(m_wCopyActorLord	==	0xffff)
		return	NULL;

	cACTOR	*lpLord	=	m_lpField->getActor(m_wCopyActorLord);

	if	(!lpLord || lpLord->m_dwNameHashCode != m_dwCopyActorLordNameHashCode)
		return	NULL;

	return	lpLord;
}

cACTOR*
cACTOR::getCharmingActor()
{
	if (m_wCharmingActor	==	0xffff)
		return	NULL;

	cACTOR	*lpLord	=	m_lpField->getActor(m_wCharmingActor);

	if (!lpLord || lpLord->m_dwNameHashCode != m_dwCharmingActorHashCode)
		return	NULL;

	return	lpLord;
}

cACTOR*
cACTOR::getConfuseCaster()
{
	if	(m_wConfuseTime	==	0)
		return	NULL;

	cACTOR	*lpActor	=	m_lpField->getActor(m_wConfuseCaster);

	if	(lpActor	&&	lpActor->m_dwNameHashCode	!=	m_dwConfuseCasterNameHashCode)
		return	NULL;

	return	lpActor;
}

cACTOR*
cACTOR::getControlLord()
{
	cACTOR	*lpLord	=	this;

	if	(m_wLordActor != 0xffff)
		lpLord	=	getLord();
	else
	if	(m_wMarionetteTime)
		lpLord	=	getMarionetteLord();
	else
	if	(isCopyActor())
		lpLord	=	getCopyActorLord();
	else
	if	(isCharmed())
		lpLord	=	getCharmingActor();
	else
	if	(isPet() || isSummonBeast())
		lpLord	=	getTamer();
	else
	if	(m_wConfuseTime && isPureMonster())
		lpLord	=	getConfuseCaster();
	
	if	(!lpLord)
		lpLord	=	this;

	return	lpLord;
}
