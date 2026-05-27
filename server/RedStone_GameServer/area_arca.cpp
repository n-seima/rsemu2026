#include "cArea.h"
#include "cField.h"
#include "arca.h"
#include "cGame.h"
#include "secretDungeon.h"

void
cAreaArcaInfo::operate1SecondWork()
{
	if (m_lpField->isArcaFull() || m_wIsEnable == FALSE)
		return;

	if (m_wLinkActiveArca	!=	0xffff)
	{
		cArca	*lpArca	=	g_arcaManager.get(m_wLinkActiveArca);

		if (!lpArca)
			m_wLinkActiveArca	=	0xffff;
		else
			return;
	}

	cArcaInfo	*lpArcaData	=	g_arcaManager.getArcaData(m_wLinkArca);

	if	(!lpArcaData)
		return;

	if	(m_lpField->m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
		m_wIsEnable	=	FALSE;
	else
	if	(lpArcaData->m_dwRegenPeriod	!=	0	&&	(g_iCounterBySecond%lpArcaData->m_dwRegenPeriod)	!=	0)
	{
// 		if	(g_bIsInnerTestServer)
// 			generate();

		return;
	}

	if	(random(100) >= lpArcaData->m_wRegenChance)
		return;

	generate();
}

BOOL
cAreaArcaInfo::generate()
{
	if (m_lpField->isArcaFull())
		return	FALSE;

	if (m_wLinkActiveArca	!=	0xffff)
		return	FALSE;

	cPOINT	pos;

	((cAreaInfo	*)this)->getCenterPos(&pos);

	m_wLinkActiveArca	=	g_arcaManager.generate(m_wLinkArca,m_lpField,pos.x,pos.y);

	if (m_wLinkActiveArca	!=	0xffff)
	{
		m_lpField->addActiveArca(m_wLinkActiveArca);

		return	TRUE;
	}

	return	FALSE;
}