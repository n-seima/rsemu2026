#ifndef _classPitchmanShop_h
#define _classPitchmanShop_h

#include "SFC.H"
#include "definePacketData.h"
#include "cPACKET_WORLDSERVER.h"
#include "CItem.H"
#include "CPitchmanShopDefine.H"

#define	dPITCHMAN_SHOP_SIZE		300

class	cACTOR;

class	CPitchmanShop
{
public:
	WORD					m_wSerial;
	char					m_strName[dPITCHMAN_SHOP_NAME_LENGTH];
	CPitchmanShopItemInfo	m_aItem[dPITCHMAN_SHOP_INVENTORY_SIZE];
	WORD					m_wSize;
	int						m_iPitchmanSerial;
	WORD					m_wFieldSerial;
	DWORD					m_dwPitchmanHashCode;
	WORD					m_wActorItemChecksum;
	WORD					m_wSignboardShape;
	WORD					m_wTextColor;
	WORD					m_wIsBold,m_wIsShadowText;
	WORD					m_wStatus;

	cPOINT					m_pos;

	CPitchmanShop()
	{
		reset();
	}

	void					reset();
	void					copy(CPitchmanShop *_lpData);
	BOOL					getSimpleShopInfo(CSimplePitchmanShopInfo *_lpInfo);
	BOOL					getInfo(CPitchmanShopForClient *_lpInfo);
	int						addItem(int _iInventorySlot,int _iShopSlot,int _iPrice, BYTE _bDenomination);		// 09.08.21
	int						removeItem(int _iShopSlot);
	int						changePitchmanShopPlace(int _iShopSlot1,int _iShopSlot2);
	int						changeShopInfo(int _iStatus,char *_lpstrName,int _iSignboardShape,WORD _wTextColor,BOOL _bIsBold,BOOL _bIsShadowText);
	cACTOR*					getOwner();
};

class	CPitchmanShopManager
{
public:
	cDen<CPitchmanShop>		m_den;

	inline	void			init()
	{
		m_den.init(dMAX_PITCHMAN_SHOP_COUNT);
		reset();
	}//초기화
	void					reset();	//초기화
	inline	void			close()	{m_den.close();}//초기화
	inline	int				add(CPitchmanShop *_lpPitchmanShop)	{return m_den.add(_lpPitchmanShop);}//추가
	inline	BOOL			remove(int _iSerial)		{return m_den.remove(_iSerial);}//제거
	inline	int				getCount()					{return m_den.getCount();}//카운트
	inline	int				getMaxCount()				{return m_den.getMaxCount();}//카운트
	inline	void			rebuild()					{m_den.rebuild();}
	inline	CPitchmanShop*	get(int _iIndex)			{return	m_den.get(_iIndex);}

	BOOL					checkItemFilter(void *_lpSerchPacket,cItem *_lpItem);
	int						serchItem(void *_lpSerchPacket,void *_lpResultPacket);
	BOOL					outActor(int _iSlot,DWORD _dwHashCode);	//	액터가 나간다. 노점상 철거
	int						addShop(cACTOR *_lpActor);
	BOOL					closeShop(int _iSerial);
	BOOL					isExistNearestShop(int _iX,int _iY,int _iFieldSerial);
	BOOL					getSimpleShopInfo(CSimplePitchmanShopInfo *_lpInfo,int _iSerial);
	BOOL					getShopInfo(CPitchmanShopForClient *_lpInfo,int _iSerial);
	void					actorDeath();
};

extern	CPitchmanShopManager	g_pitchmanShopManager;

#endif