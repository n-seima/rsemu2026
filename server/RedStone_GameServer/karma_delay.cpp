#include "CKarma.h"
#include "cACTOR.h"
#include "CQuest.h"
#include "debugCode.h"
#include "cGame.h"

cDelayKarmaManager	g_delayKarmaManager;

void
cDelayKarmaInfo::reset()
{
	m_dwReleaseDelayTime=	0;
	m_lpOwner			=	NULL;
	m_lpTarget			=	NULL;
	m_lpKarma			=	NULL;
	m_lpField			=	NULL;
	m_wPauseReaction	=	0;
}

BOOL
cDelayKarmaInfo::isIncorrect()
{
	if (!m_lpOwner || m_lpOwner->m_wSerialInField == 0xffff|| m_lpOwner->m_dwNameHashCode != m_dwOwnerNameHashCode)
		return	TRUE;

	if (!m_lpTarget || m_lpTarget->m_wSerialInField == 0xffff|| m_lpTarget->m_dwNameHashCode != m_dwTargetNameHashCode)
		return	TRUE;

	return	FALSE;
}


BOOL
cDelayKarmaManager::addDelayKarma(CKarma *_lpKarma,int _iPauseReaction,DWORD _dwReleaseDelayTime,cACTOR *_lpOwner,cACTOR *_lpTarget,cFIELD *_lpField)
{
	for (int i=0;i<dMAX_DELAY_KARMA_COUNT;i++)
	{
		if (m_aDelayKarma[i].m_wPauseReaction	!=	0)
			continue;
		
		m_aDelayKarma[i].m_lpKarma				=	_lpKarma;
		m_aDelayKarma[i].m_wPauseReaction		=	_iPauseReaction;
		m_aDelayKarma[i].m_dwReleaseDelayTime	=	_dwReleaseDelayTime;
		m_aDelayKarma[i].m_lpOwner				=	_lpOwner;
		m_aDelayKarma[i].m_lpField				=	_lpField;
		m_aDelayKarma[i].m_lpTarget				=	_lpTarget;
		m_aDelayKarma[i].m_dwOwnerNameHashCode	=	_lpOwner->m_dwNameHashCode;
		m_aDelayKarma[i].m_dwTargetNameHashCode	=	_lpTarget->m_dwNameHashCode;

		return	TRUE;
	}

	return	FALSE;
}

void
cDelayKarmaManager::updateDelayKarma()
{
	int		i;
	BOOL	bTempValue;

	for (i=0;i<dMAX_DELAY_KARMA_COUNT;i++)
	{
		if	(m_aDelayKarma[i].m_wPauseReaction	==	0)
			continue;

		if	(m_aDelayKarma[i].isIncorrect())
		{
			m_aDelayKarma[i].reset();
			continue;
		}

		if	(m_aDelayKarma[i].m_dwReleaseDelayTime > g_dwCurrentTime)
			continue;

		m_aDelayKarma[i].m_lpKarma->operateReaction(m_aDelayKarma[i].m_lpTarget,m_aDelayKarma[i].m_lpOwner,&bTempValue,m_aDelayKarma[i].m_wPauseReaction,m_aDelayKarma[i].m_lpField);
		m_aDelayKarma[i].reset();
	}
}