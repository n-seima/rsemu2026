#include "arca.h"

cArcaManager	g_arcaManager;

void
cArcaInfo::copy(cArcaInfo *_lpInfo)
{
	memcpy(this,_lpInfo,sizeof(cArcaInfo));
}

void
cArcaInfo::reset()
{
	strcpy(m_strName,"no name");

	m_wSerial				=	0xffff;

	m_dwRegenPeriod			=	60*10;
	m_wRegenChance			=	10;

	m_iDurability			=	10000;
	m_iResistance			=	10000;

	m_bf1IsDestroyAble		=	FALSE;
	m_bf10DestroyAbleLevel	=	100;

	m_bSecretLevel			=	0;
	m_bLockLevel			=	0;

	m_wItemDropLevel		=	10;
	m_wItemCount			=	1;

	m_dwOwnGold				=	100;

	m_wTrapLevel			=	0;
	m_wDetectTrapLevel		=	0;
	m_wDisarmTrapLevel		=	0;

	m_wTrap1Skill			=	0xffff;
	m_wTrap1ActiveChance	=	0;
	m_wTrap2Skill			=	0xffff;
	m_wTrap2ActiveChance	=	0;

	for (int i=0;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
		m_aItem[i].reset();
}

BOOL
cArcaInfo::isSame(cArcaInfo *_lpArca)
{
	if (stricmp(m_strName,_lpArca->m_strName)!=0)
		return	FALSE;

	if (m_dwRegenPeriod			!=	_lpArca->m_dwRegenPeriod			)
		return	FALSE;
	if (m_wRegenChance			!=	_lpArca->m_wRegenChance			)
		return	FALSE;

	if (m_iDurability			!=	_lpArca->m_iDurability			)
		return	FALSE;
	if (m_iResistance			!=	_lpArca->m_iResistance			)
		return	FALSE;

	if (m_bf1IsDestroyAble		!=	_lpArca->m_bf1IsDestroyAble		)
		return	FALSE;
	if (m_bf10DestroyAbleLevel	!=	_lpArca->m_bf10DestroyAbleLevel	)
		return	FALSE;

	if (m_bSecretLevel			!=	_lpArca->m_bSecretLevel			)
		return	FALSE;
	if (m_bLockLevel			!=	_lpArca->m_bLockLevel			)
		return	FALSE;

	if (m_wItemDropLevel		!=	_lpArca->m_wItemDropLevel		)
		return	FALSE;
	if (m_wItemCount			!=	_lpArca->m_wItemCount			)
		return	FALSE;

	if (m_dwOwnGold				!=	_lpArca->m_dwOwnGold				)
		return	FALSE;

	if (m_wTrapLevel			!=	_lpArca->m_wTrapLevel			)
		return	FALSE;
	if (m_wDetectTrapLevel		!=	_lpArca->m_wDetectTrapLevel		)
		return	FALSE;
	if (m_wDisarmTrapLevel		!=	_lpArca->m_wDisarmTrapLevel		)
		return	FALSE;

	if (m_wTrap1Skill			!=	_lpArca->m_wTrap1Skill			)
		return	FALSE;
	if (m_wTrap1ActiveChance	!=	_lpArca->m_wTrap1ActiveChance	)
		return	FALSE;
	if (m_wTrap2Skill			!=	_lpArca->m_wTrap2Skill			)
		return	FALSE;
	if (m_wTrap2ActiveChance	!=	_lpArca->m_wTrap2ActiveChance	)
		return	FALSE;

	return	TRUE;
}


int	
cArcaManager::add(cArcaInfo *_lpArca)
{
	m_den.setFirst();

	while(1)
	{
		cArcaInfo *lpArca	=	m_den.getNext();

		if (!lpArca)
			break;

		if (stricmp(lpArca->m_strName,_lpArca->m_strName) == 0)
		{
			ERRMSG("error!!","이미 같은 이름의 보물 상자가 있습니다.");

			return	0xffff;
		}
	}

	return	m_den.add(_lpArca);
}

void
cArcaManager::replace(int _iSerial,cArcaInfo *_lpArca)
{
	m_den.replace(_iSerial,_lpArca);
}

cArcaInfo*
cArcaManager::getByName(char *_lpstrName)
{
	m_den.setFirst();

	while(1)
	{
		cArcaInfo	*lpInfo	=	m_den.getNext();

		if (!lpInfo)
			break;

		if (stricmp(lpInfo->m_strName,_lpstrName) == 0)
			return	lpInfo;
	}

	return	NULL;
}