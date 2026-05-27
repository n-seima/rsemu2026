#ifndef _classArcaDefine_h
#define _classArcaDefine_h

//#ifndef _WINDOWS_
//#include <windows.h>
//#endif

#include "CItemDefine.h"

#define	dMAX_ITEM_COUNT_IN_ARCA		10
#define	dMAX_ARCA_COUNT				2048
#define	dMAX_ARCA_COUNT_IN_FIELD	256

class	cArcaSimpleInfo
{
public:
	WORD	m_wSerial;
	WORD	m_wX,m_wY;
	WORD	m_bf8SecretLevel	:	8;
	WORD	m_bf1IsDestroyAble	:	1;
	WORD	m_bf1IsLocked		:	1;
};

class	cItemInArca : public cDroppingItem
{
public:

};

class	cArcaDefine
{
public:
	WORD		m_wSerial;
	char		m_strName[32];
	DWORD		m_dwRegenPeriod;
	WORD		m_wRegenChance;

	int			m_iDurability;
	int			m_iResistance;

	WORD		m_bf1IsDestroyAble		:	1;
	WORD		m_bf10DestroyAbleLevel	:	10;
	WORD		m_bf2TrapTarget			:	2;

	BYTE		m_bSecretLevel;
	BYTE		m_bLockLevel;

	WORD		m_wItemDropLevel;
	WORD		m_wItemCount;

	DWORD		m_dwOwnGold;

	WORD		m_wTrapLevel;
	WORD		m_wDetectTrapLevel;
	WORD		m_wDisarmTrapLevel;

	WORD		m_wTrap1Skill,m_wTrap1ActiveChance;
	WORD		m_wTrap2Skill,m_wTrap2ActiveChance;

	cItemInArca	m_aItem[dMAX_ITEM_COUNT_IN_ARCA];
};

#endif