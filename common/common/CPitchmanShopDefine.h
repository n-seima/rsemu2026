#ifndef _classPitchmanShopDefine_h
#define _classPitchmanShopDefine_h

#include <winsock2.h>
#include <windows.h>
#include "CItemDefine.H"
#include "definePacketData.H"


#define	dPITCHMAN_SHOP_INVENTORY_SIZE	12
#define	dPITCHMAN_SHOP_NAME_LENGTH		62
#define	dPITCHMAN_SHOP_MINIMUM_SIZE		3

#define	dMAX_PITCHMAN_SHOP_COUNT		1024

//
//	Pitchman Shop Status
enum
{
	ePSS_READY,	//	오픈 준비중이다.
	ePSS_OPEN,
	ePSS_REQUIRE_INFO,
	ePSS_WAIT_INFO,
};

class	CPitchmanShopItemInfo			// 09.08.21
{
public:
	WORD	m_wSlotIndex;
	DWORD	m_dwPrice;
	BYTE	m_bDenomination;
};

class	CMinimumPitchmanShopInfo
{
public:
	WORD			wSerialInField;			//	필드안에서 플레이어의 시리얼
	WORD			wX,wY;
};	//	class	CMinimumActorInfo

class	CSimplePitchmanShopInfo
{
public:
	WORD			m_bf10Serial		: 10;
	WORD			m_bf6SignboardShape : 6;
	WORD			m_wTextColor;

	WORD			m_bf1IsBold			: 1;
	WORD			m_bf1IsShadowText	: 1;
	WORD			m_bf2Status			: 2;
	WORD			m_bf12Owner			: 12;

	WORD			m_wX,m_wY;
};


class	CPitchmanShopItemInfoForClient		// 09.08.21
{
public:
	CItemDefine		m_item;			
	DWORD			m_dwPrice;
	WORD			m_wSlot;
	BYTE			m_bDenomination;		// 통화단위.
};

class	CPitchmanShopForClient
{
public:
	WORD							m_wSerial;
	WORD							m_wTextColor;

	WORD							m_bf4SignboardShape	: 4;
	WORD							m_bf4ItemCount		: 4;
	WORD							m_bf1IsBold			: 1;
	WORD							m_bf1IsShadowText	: 1;

	char							m_strName[dPITCHMAN_SHOP_NAME_LENGTH];
	char							m_strOwnerName[dNAME_LENGTH];
	CPitchmanShopItemInfoForClient	m_aItem[dPITCHMAN_SHOP_INVENTORY_SIZE];
	
	void							reset();
};

#endif