#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CGuild.H"
#include "CEvent.H"
#include "arca.H"

void
cACTOR::hitObject()
{
	CHitInfo	*lpHitInfo	=	m_attackInfo.getInfo(0);

	switch(m_wUseSkill)
	{
		case	dSKILL_BASIC_ATTACK_TO_ARCA	:
		{
			cArca	*lpArca	=	g_arcaManager.get(m_attackInfo.m_wTarget);

			if (lpArca)
				lpArca->hitDamage(this,lpHitInfo->getDamage());

			break;
		}

		case	dSKILL_BASIC_ATTACK_TO_DOOR	:
		{
			cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)m_lpField->getArea(m_attackInfo.m_wTarget);

			if (lpDoor)
				lpDoor->hitDamage(this,lpHitInfo->getDamage());

			break;
		}
	}
}
