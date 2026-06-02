#include "CGamePlay.H"
#include "CActor.H"
#include "CWindowInterface.H"
#include "CGamePlayInterface.H"
#include "CMessage.H"
#include "CItemEffect.H"
#include "hero_mini_pet.H"
#include "secretDungeon.H"
#include "cPORTAL.H"

enum
{
	eITI_RARE,
	eITI_UNIQUE,
	eITI_SUPER,
	eITI_QUEST,
	eITI_DX,
	eITI_EXTRA_10,
	eITI_EXTRA_9,
	eITI_EXTRA_8,
	eITI_EXTRA_7,
	eITI_EXTRA_6,
	eITI_EXTRA_5,
	eITI_EXTRA_4,
	eITI_EXTRA_3,
	eITI_EXTRA_2,
	eITI_EXTRA_1,
	eITI_EXTRA_SPECIAL,
	eITI_EXTRA_ABSOLUTE,
	eITI_EXTRA_UNLIMITED,
	eITI_SET,
	eITI_ULTIMATE,
};

CPos		CGamePlay::s_posInventoryWindow;	//	인벤토리 윈도우 위치
cItem*		CGamePlay::s_lpInventoryWindowEquipmentList		=	NULL;
cItem*		CGamePlay::s_lpInventoryWindowItemList			=	NULL;
int*		CGamePlay::s_lpGold								=	NULL;
int			CGamePlay::s_iReadyToUseItem					=	0xffff;
int			CGamePlay::s_iBookedWorkItem					=	0xffff;
cBARMENU	CGamePlay::s_bmCarrotShop;

BOOL		l_bIsShowCarrotButton							=	FALSE;

#define	dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW		6
#define	dITEM_VERTICAL_COUNT_AT_INVENTORY_WINDOW	7
#define	dITEM_SLOT_WIDTH							40
#define	dITEM_SLOT_HEIGHT							37

#define	dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW	2
#define	dITEM_VERTICAL_COUNT_AT_EXTRA_INVENTORY_WINDOW	8
#define	dEXTRA_INVENTORY_ITEM_SLOT_WIDTH				40
#define	dEXTRA_INVENTORY_ITEM_SLOT_HEIGHT				37
#define	dEXTRA_INVENTORY_TITLE_WIDTH					102
#define	dEXTRA_INVENTORY_VERTICAL_POS					156

enum
{
	eEBS_BAG_FRAME	=	0,
	eEBS_BAG_INNER_FRAME,

	eIWI_CARROT_SHOP_NORMAL	=	2,
	eIWI_CARROT_SHOP_FOCUS		,
	eIWI_CARROT_SHOP_PRESSED	,
	eIWI_CARROT_SHOP_DISABLE	,

	eIWI_TOGGLE_MINIPET_WND_NORMAL	=	160,
	eIWI_TOGGLE_MINIPET_WND_FOCUS		,
	eIWI_TOGGLE_MINIPET_WND_PRESSED	,
	eIWI_TOGGLE_MINIPET_WND_DISABLE	,

	eIWI_NET_CAFE_WND_NORMAL	=	167,
	eIWI_NET_CAFE_WND_FOCUS		,
	eIWI_NET_CAFE_WND_PRESSED	,
	eIWI_NET_CAFE_WND_DISABLE	,

	eIWI_TRADE_NORMAL,
	eIWI_TRADE_ACTIVE,
	eIWI_TRADE_PRESSED,
	eIWI_TRADE_DISABLE,

	eIWI_CART_NORMAL,
	eIWI_CART_ACTIVE,
	eIWI_CART_PRESSED,
	eIWI_CART_DISABLE,

	eIWI_PITCHMAN_SHOP_NORMAL,
	eIWI_PITCHMAN_SHOP_ACTIVE	,
	eIWI_PITCHMAN_SHOP_PRESSED	,
	eIWI_PITCHMAN_SHOP_DISABLE	,

	eIWI_CALL_MAGIC_CARPET_NORMAL	,
	eIWI_CALL_MAGIC_CARPET_ACTIVE	,
	eIWI_CALL_MAGIC_CARPET_PRESSED	,
	eIWI_CALL_MAGIC_CARPET_DISABLE	,
};

CPos	l_aEquipmentPos[]	=
{
	19,25	,//	무기
	61,10	,//	방패/탄환
	139,48	,//	갑옷
	100,48	,//	장갑
	139,10	,//	헬멧 종류
	178,10	,//	귀거리
	100,10	,//	목걸이
	61,48	,//	벨트
	178,48	,//	신발
	20,88	,//	반지1
	48,88	,//	반지2
	76,88	,//	반지3
	104,88	,//	반지4
	144,88	,//	반지5
	172,88	,//	반지6
	200,88	,//	반지7
	228,88	,//	반지8
	220,25	,//	무기2
	0xffff
};

CPos	l_aQuickItemSlotPos[]	=
{
	534,524,	
	570,524,
	606,524,
	642,524,
	678,524,
	0xffff
};

//
//	Inventory Window Menu
enum
{
eIWM_TRADE,
eIWM_OPEN_SHOPPING_CART,
eIWM_OPEN_PICHMAN_SHOP,
eIWM_OPEN_CARROT_SHOP,
eIWM_TOGGLE_MINIPET_WINDOW,
eIWM_CALL_MAGIC_CARPET=987324,
eIWM_NETCAFEMENU = 100
};


int		l_aVaildWeapon[dPLAYER_JOB_COUNT][2] = 
{		//  인터페이스파일 1번이나,,2번, 무기 빈칸
		1,eIWI_WEAPON_JOB1_1,
		1,eIWI_WEAPON_JOB1_2,	// 
		1,eIWI_WEAPON_JOB2_1,
		1,eIWI_WEAPON_JOB2_2,	//
		1,eIWI_WEAPON_JOB3_1,
		1,eIWI_WEAPON_JOB3_2,
		1,eIWI_WEAPON_JOB4_1,
		1,eIWI_WEAPON_JOB4_2,
		1,eIWI_WEAPON_JOB5_1,
		1,eIWI_WEAPON_JOB5_2,
		1,eIWI_WEAPON_JOB6_1,
		1,eIWI_WEAPON_JOB6_2,
		1,eIWI_WEAPON_JOB7_1,
		1,eIWI_WEAPON_JOB7_2,
		1,eIWI_WEAPON_JOB8_1,
		1,eIWI_WEAPON_JOB8_2,
		1,eIWI_WEAPON_JOB9_1,
		2,eIWI_WEAPON_JOB9_2,
		2,eIWI_WEAPON_JOB10_1,
		1,eIWI_WEAPON_JOB8_1,
};		


//
//	인벤토리 윈도우 닫기
void
CGamePlay::CloseInventoryWindow()
{
	s_bmBottomMenu.check(eBIM_MENU_INVENTORY,FALSE);
	cINPDEV::ReleaseExclusive();

	//s_bmButton.reset();
	s_bmButtonI2.reset();
	s_bmCarrotShop.reset();

	s_rectInterfaceSize.set(0, 0, 0, 0);

	s_iDragItem			=	0xffff;
	s_iFocusItem		=	0xffff;
	s_iFocusEquipment	=	0xffff;
	s_iFocusBeltSlot	=	0xffff;
	s_propose.m_work.isProposeToTrade	=	FALSE;
}

//
//	열어!!
void
CGamePlay::OpenInventoryWindow()
{
	if (s_iPopupInterface	==	ePIW_INVENTORY)
	{
		ClosePopupWindow();

		return;
	}

#ifdef	_FOR_KOREA
	l_bIsShowCarrotButton			=	TRUE;
#ifndef	_DEBUG
	if	(g_bIsTestServer)
		l_bIsShowCarrotButton		=	FALSE;
#endif
#endif

#ifdef	_FOR_JAPAN
	l_bIsShowCarrotButton			=	TRUE;
#endif
#ifdef	_FOR_THAI
	l_bIsShowCarrotButton			=	TRUE;
#endif
#ifdef	_FOR_USA
	l_bIsShowCarrotButton			=	TRUE;
#endif

	if	(g_bIsDuelServer)
		l_bIsShowCarrotButton		=	FALSE;

	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	ClosePopupWindow();
	g_gwMiniPetStatus.open();

	s_iPopupInterface		=	ePIW_INVENTORY;

	s_bmBottomMenu.resetChecker();
	s_bmBottomMenu.check(eBIM_MENU_INVENTORY,TRUE);

	SetInventoryWindowPos();

	InitPopupWindowCloseButton();

	int	iX		=	s_posInventoryWindow.x - 10;
	int	iY		=	s_posInventoryWindow.y;

	//s_bmButton.reset();
	s_bmButtonI2.reset();
	s_bmButtonI2.addImageBar("",eIWM_TRADE				,iX+20 ,172+g_iCorrectBI_Y/2,eIWI_TRADE_NORMAL,eIWI_TRADE_ACTIVE,eIWI_TRADE_PRESSED,0xffff,eIWI_TRADE_DISABLE,dMSG_TRADE_COMMENT);
	s_bmButtonI2.addImageBar("",eIWM_OPEN_SHOPPING_CART	,iX+55 ,172+g_iCorrectBI_Y/2,eIWI_CART_NORMAL,eIWI_CART_ACTIVE,eIWI_CART_PRESSED,0xffff,eIWI_CART_DISABLE,dMSG_SHOPPING_CART_COMMENT);
	s_bmButtonI2.addImageBar("",eIWM_OPEN_PICHMAN_SHOP	,iX+90 ,172+g_iCorrectBI_Y/2,eIWI_PITCHMAN_SHOP_NORMAL,eIWI_PITCHMAN_SHOP_ACTIVE,eIWI_PITCHMAN_SHOP_PRESSED,0xffff,eIWI_PITCHMAN_SHOP_DISABLE,dMSG_PITCHMAN_SHOP_BUTTON_COMMENT);
	s_bmButtonI2.addImageBar("",eIWM_CALL_MAGIC_CARPET	,iX+125,172+g_iCorrectBI_Y/2,eIWI_CALL_MAGIC_CARPET_NORMAL,eIWI_CALL_MAGIC_CARPET_ACTIVE,eIWI_CALL_MAGIC_CARPET_PRESSED,0xffff,eIWI_CALL_MAGIC_CARPET_DISABLE,dMSG_CALL_MAGIC_CARPET_BUTTON_COMMENT);

	if	(l_bIsShowCarrotButton)
	{
		s_bmCarrotShop.reset();
		s_bmCarrotShop.addImageBar("",eIWM_OPEN_CARROT_SHOP	,iX+160,172+g_iCorrectBI_Y/2,eIWI_CARROT_SHOP_NORMAL,eIWI_CARROT_SHOP_FOCUS,eIWI_CARROT_SHOP_PRESSED,0xffff,eIWI_CARROT_SHOP_DISABLE,dMSG_CARROT_SHOP_COMMENT);
	}

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
	{
		s_bmButtonI2.disable(eIWM_TRADE);
		s_bmButtonI2.disable(eIWM_OPEN_SHOPPING_CART);
		s_bmButtonI2.disable(eIWM_OPEN_PICHMAN_SHOP);
		s_bmCarrotShop.disable(eIWM_OPEN_CARROT_SHOP);
	}

	s_bmButtonI2.addImageBar("",eIWM_TOGGLE_MINIPET_WINDOW	,iX+195,172+g_iCorrectBI_Y/2,eIWI_TOGGLE_MINIPET_WND_NORMAL,eIWI_TOGGLE_MINIPET_WND_FOCUS,eIWI_TOGGLE_MINIPET_WND_PRESSED
								,0xffff,eIWI_TOGGLE_MINIPET_WND_DISABLE,dMSG_TOGGLE_MINIPET_WINDOW_COMMENT);

//	g_hero.m_bf1IsNetCafeUser = TRUE;	//테스트용 코드.
// 	g_hero.m_bf1IsNetCafeUser = FALSE;	//테스트용 코드.

#ifdef	_FOR_JAPAN			//JBC 08-08-12
	s_bmButtonI2.addImageBar("" , eIWM_NETCAFEMENU , iX+230 , 172 +g_iCorrectBI_Y/2, eIWI_NET_CAFE_WND_NORMAL,eIWI_NET_CAFE_WND_FOCUS,eIWI_NET_CAFE_WND_PRESSED
								,0xffff,eIWI_NET_CAFE_WND_DISABLE,dMSG_USE_NET_CAFE_MENU_COMMENT);

	if	(g_hero.m_bf1IsNetCafeUser == FALSE)
		s_bmButtonI2.disable(eIWM_NETCAFEMENU);
#endif

	//if	(g_hero.m_wPreminumServiceLevel	<	ePSL_PREMIUM)

	if	(g_hero.m_bf1IsHaveSummonMagicCarpetItem == 0)
		s_bmButtonI2.disable(eIWM_CALL_MAGIC_CARPET);

	if	(g_hero.iMiniPet()->getMiniPetCount()	<=	0)
		s_bmButtonI2.disable(eIWM_TOGGLE_MINIPET_WINDOW);

	s_propose.m_work.isProposeToTrade	=	FALSE;
}

//
//	인벤토리 윈도우 위치 정하기
void
CGamePlay::SetInventoryWindowPos()
{
	int	iWidth				=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_INVENTORY);

	
	switch(s_iPopupInterface)
	{
	case	ePIW_INVENTORY				:
		{
			s_posInventoryWindow.x	=	g_iScreenWidth-iWidth;
			
			s_rectInterfaceSize.x1	=	g_iScreenWidth-iWidth;
			s_rectInterfaceSize.x2	=	g_iScreenWidth;
						
			if	(g_hero.m_bf6ExtraInventorySize)
			{				
				s_rectInterfaceSize.x1 -= dEXTRA_INVENTORY_TITLE_WIDTH;
			}
			if	(g_hero.iMiniPet()->getMiniPetCount() && g_config.m_bf1IsShowMiniPetWindow)
			{
				s_rectInterfaceSize.x1 -= iWidth;
			}
			break;
		}
	case	ePIW_SHOP					:
		{
			if	(g_gwShop.m_iShopKind	==	eSK_PIGEON_POST)
			{
				s_rectInterfaceSize.x1 = g_iScreenWidth-iWidth;
				s_rectInterfaceSize.x2 = g_iScreenWidth;
			}
			else
			{
				s_rectInterfaceSize.x1	=	g_iScreenWidth-iWidth*2;
				s_rectInterfaceSize.x2	=	g_iScreenWidth;
				s_posInventoryWindow.x	=	g_iScreenWidth-iWidth;
			}
			break;
		}
	case	ePIW_TRADE_BOX				:
	case	ePIW_BANK					:
	case	ePIW_SHOPPING_CART			:
	case	ePIW_PITCHMAN_SHOP			:
	case	ePIW_CARROT_SHOP			:
	case	ePIW_GUILD_INVENTORY		:
	case	ePIW_GUILD_FEED_PET_WINDOW	:
	case	ePIW_UPGRADE_SHOP			:
		{
			s_rectInterfaceSize.x1	=	g_iScreenWidth-iWidth*2;
			s_rectInterfaceSize.x2	=	g_iScreenWidth;
			s_posInventoryWindow.x	=	g_iScreenWidth-iWidth;
			break;
		}
	case	ePIW_INVENTORY_AND_STATUS	:
		{
			s_posInventoryWindow.x	=	g_iScreenWidth-iWidth*2;
			
			s_rectInterfaceSize.x1 = g_iScreenWidth-iWidth*2;
			s_rectInterfaceSize.x2 = g_iScreenWidth;
			break;
		}
	case	ePIW_BOTTOMLESS_BOX			:
		{
			s_posInventoryWindow.x	=	g_iScreenWidth-iWidth;

			s_rectInterfaceSize.x1 = g_iScreenWidth-iWidth*2;
			s_rectInterfaceSize.x2 = g_iScreenWidth;
			break;
		}
		
	}
	
	s_posInventoryWindow.y	= iTitleBarHeight+g_iCorrectBI_Y/2;
  	s_rectInterfaceSize.y1	= iTitleBarHeight+g_iCorrectBI_Y/2-g_sprInterface.getSpriteHeight(eWINDOW_TITLE_INVENTORY);
  	s_rectInterfaceSize.y2	= iTitleBarHeight+g_iCorrectBI_Y/2+g_sprInterface.getSpriteHeight(eWINDOW);

}

//
//	아이템 버리기
void
CGamePlay::OperateDropItem(int _iItemSlot)
{
	return;

#ifndef	_FOR_JAPAN
	if (s_iDragItem	==	dITEM_DRAG_GOLD)
		return;

//	g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_DESTROY_ITEM_FORM,lpItem->getName()),dMSG_YES,dMSG_NO);
//	g_msgBox.setButtonId(eMBI_DESTROY_ITEM);

	return;
#else
//	if (s_iPopupInterface	!=	ePIW_INVENTORY)	return;
	int	iDroppingCount	=	0;

	if (s_iDragItem	==	dITEM_DRAG_GOLD)
	{
		if (*s_lpGold	==	0)	return;

		g_hero.prepareDroppingItem(s_iDragItem);

		if (*s_lpGold	>	1)
		{
			cItem	item;

			item.m_wBaseItem	=	dITEM_MONEY;
			item.m_dwSerial		=	*s_lpGold;

			g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,s_iDragItem,&item,eICWR_RETURN_DROP_GOLD);
			cINPDEV::SetExclusive(eMOUSE_STATE_GET_ITEM_COUNT);
			return;
		}

		g_hero.sendDroppingItem(s_iDragItem,1);
		return;
	}

	cItem	*lpDragItem;

	if	(s_iDragItem	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		lpDragItem	=	&s_lpInventoryWindowEquipmentList[s_iDragItem-dBORDER_OF_ITEM_AND_EQUIPMENT];
	else
	if	(s_iDragItem>=dOWN_ITEM_COUNT)
		lpDragItem	=	&g_hero.m_aExtraInventory[s_iDragItem-dOWN_ITEM_COUNT];
	else
		lpDragItem	=	&s_lpInventoryWindowItemList[s_iDragItem];

	if (g_aBasicItem[lpDragItem->m_wBaseItem].m_attr.isCanNotDropItem)
	{
		g_lpHero->warning(dMSG_CAN_NOT_DROP_ITEM);
		return;
	}

	g_hero.prepareDroppingItem(s_iDragItem);

	if	(lpDragItem->m_bCount	>	1	||	!lpDragItem->isExtraItem() || !lpDragItem->isMagicCarpetEmblem() || !lpDragItem->isExceptionItem())
	{
		g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,s_iDragItem,lpDragItem,eICWR_RETURN_DROP_ITEM_COUNT);
		cINPDEV::SetExclusive(eMOUSE_STATE_GET_ITEM_COUNT);
		return;
	}

	if	(!lpDragItem->isBreedingRecordBook() && !lpDragItem->isMagicCarpetEmblem() && !lpDragItem->isExceptionItem())
		if	(lpDragItem->m_bCount	==	0)
		{
			int	iPart	=	s_iDragItem-dBORDER_OF_ITEM_AND_EQUIPMENT;

			if (iPart	>=	dEQUIP_BELT_SLOT1	&&	iPart	<=	dEQUIP_BELT_SLOT5)
				s_agent.sendStripEquipmentMessage(s_iDragItem-dBORDER_OF_ITEM_AND_EQUIPMENT);			//	장비 벗기

			return;
		}

	g_hero.sendDroppingItem(s_iDragItem,1);
#endif
}

//
//	아이템 드래그 처리
void 
CGamePlay::OperateDragItem(BOOL _bIsCheckExclusiveMode)
{

	if	(s_iDragItem		==	0xffff)
		if	(s_iFocusItem	==	0xffff	&&	s_iFocusEquipment	==	0xffff	&&	s_iFocusBeltSlot	==	0xffff)
			return;

	if	(_bIsCheckExclusiveMode	&&	cINPDEV::IsExclusive(eMOUSE_STATE_ITEM_DRAG))
		return;

	///////////////////////////////////////////////////////////////////////
	///////////////////////////   KH_1   ////////////////////////////////// 
	///////////////////////////////////////////////////////////////////////
	// cINPDEV::s_posOld(private -> public)

	if	(s_bIsDragMouse	&& s_iDragItem	==	0xffff)
	{
		//포커스가 있고 1픽셀 움직이면 드래그 시작...
		if	(s_iFocusItem	!=	0xffff)
			s_iDragItem		=	s_iFocusItem;
		if	(s_iFocusEquipment!=	0xffff)
			s_iDragItem		=	s_iFocusEquipment+dBORDER_OF_ITEM_AND_EQUIPMENT;
		if	(s_iFocusBeltSlot!=	0xffff)
			s_iDragItem		=	s_iFocusBeltSlot+dEQUIP_BELT_SLOT1+dBORDER_OF_ITEM_AND_EQUIPMENT;

		cINPDEV::SetExclusive(eMOUSE_STATE_ITEM_DRAG);
	}
	else	//	l버튼 더블 클릭 설정 
	if( s_isDoubleClickLeftButton &&  (s_iPopupInterface		==	ePIW_INVENTORY ||  s_iPopupInterface == ePIW_SHOP))
	{
		//무기라면...벗어라.
		if( s_iFocusEquipment	!= 0xffff )
		{
			cItem	*lpEquip	=	g_hero.getEquipment(s_iFocusEquipment);

			if	(lpEquip && (lpEquip->getBasicItem(TRUE)->m_wKind	!=	eIK_WING || g_bIsDuelServer))
			{
				g_hero.stripEquipItem(s_iFocusEquipment + 100);

				s_iFocusItem		=	0xffff;
				s_iFocusEquipment	=	0xffff;
				s_iDragItem			=	0xffff;
			}
		}
		//무기라면...입어라. 
		else
		if	(s_iFocusItem != 0xffff )
		{
			cItem	*lpItem	=	g_hero.getInventoryItem(s_iFocusItem);

			if	(lpItem && lpItem->getBasicItem()->m_wKind == eIK_RING )
			{
				//반지는 예외처리 
				for	(int i = dEQUIP_RING1; i <= dEQUIP_RING8; i++ )
				{
					if	( g_hero.m_aEquip[i].getBasicItem() == NULL )
					{
						s_agent.sendEquipItem(s_iFocusItem,i);
						break;
					}
				}
			}	
		   	else
			{
				if (g_hero.isConsumeItem(s_iFocusItem))
				{
					if(!lpItem->isItemPack())
						g_hero.askUseItem(s_iFocusItem);
				}
				else
					g_hero.setEquipItem(s_iFocusItem);
			}

			s_iFocusItem		=	0xffff;
			s_iFocusEquipment	=	0xffff;
			s_iDragItem			=	0xffff;
		}

		s_isDoubleClickLeftButton=	FALSE;
	}
		
	if	( s_isTouchLeftButton || s_isDoubleClickLeftButton || s_isLeftButtonPressed	||	s_iDragItem	==	0xffff	)
		return;
	
	if	(s_iDragItem >= dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		cItem	*lpItem	=	g_hero.getItem(s_iDragItem);

		if	(lpItem && lpItem->getBasicItem(TRUE)->m_wKind == eIK_WING && !g_bIsDuelServer)
		{
			g_msgBox.cPopup("",dMSG_CAN_NOT_STRIP_WING,dMSG_OK);
			return;
		}
	}

	int	iDroppingArea	=	GetItemDroppingArea();

	cINPDEV::ReleaseExclusive();

	switch(iDroppingArea)
	{
		case	eDROPPING_ITEM_AREA_MINIPET				:
			g_gwMiniPetStatus.feedItem();
			// 10.01.07
			g_hero.deleteQuickSlotItem(s_iDragItem);

			break;
		case	eDROPPING_ITEM_AREA_FEED_PET			:
			g_gwFeedGuildPet.feedItem();
			// 10.01.07
			g_hero.deleteQuickSlotItem(s_iDragItem);
			break;
		case	eDROPPING_ITEM_AREA_NONE				:
			break;

		case	eDROPPING_ITEM_AREA_INVENTORY_WHEN_BANK_TRANSACTION	:
		{
			g_gwBank.moveInventoryItemWhenBankTransaction();
			break;
		}

		case	eDROPPING_ITEM_AREA_BANK			:
		{
			g_gwBank.storeItemToTheBank(s_iDragItem);
			// 10.01.07
			g_hero.deleteQuickSlotItem(s_iDragItem);
			break;
		}

		case	eDROPPING_ITEM_AREA_INVENTORY_WHEN_OPEN_GUILD_INVENTORY	:
		{
			if	(s_iDragItem	==	dITEM_DRAG_GOLD)
				break;

			int	iFocusItem	=	GetFocusItemSlot();

			if	(s_iDragItem	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)	
			{
				if	(!g_hero.getValidInventorySlot())
					break;

				// 10.01.07
				g_hero.stripEquipItem(s_iDragItem);
				break;
			}

			if	(iFocusItem	==	s_iDragItem		)
				break;	//	자기 자신이다.
			if	(iFocusItem	==	dITEM_DRAG_GOLD	)
				break;	//	돈하고 바꿀 수는 없다. -_-a
			if	(iFocusItem	==	0xffff			)
				break;	//	이상한곳이다.

			s_agent.sendChangeItemPlace(s_iDragItem,iFocusItem);//	s_iDragItem과 iItem의 위치를 바꾸거나 합친다.
			break;
		}
		case	eDROPPING_ITEM_AREA_GUILD_INVENTORY	:
		{
			g_gwGuildInventory.storeItem(s_iDragItem);
			// 10.01.07
			g_hero.deleteQuickSlotItem(s_iDragItem);

			break;
		}

		case	eDROPPING_ITEM_AREA_BOTTOMLESS_BOX	:
		{
			g_gwBottomlessBoxWindow.addItem(s_iDragItem);
			// 10.01.07
			g_hero.deleteQuickSlotItem(s_iDragItem);
			break;
		}

		case	eDROPPING_ITEM_AREA_SHOP			:
		{
			if (s_iDragItem	==	dITEM_DRAG_GOLD)
				break;

			g_gwShop.sellItem(s_iDragItem);
			// 10.01.07
			g_hero.deleteQuickSlotItem(s_iDragItem);
			break;
		}

		case	eDROPPING_ITEM_AREA_PITCHMAN_SHOP	:
		{
			g_gwPitchmanShop.operateDropItem(s_iDragItem);
			// 10.01.07
			g_hero.deleteQuickSlotItem(s_iDragItem);
			break;
		}

		case	eDROPPING_ITEM_AREA_TRADE_BOX		:
		{
			AskAddTradeItem(s_iDragItem);
			// 10.01.07
			g_hero.deleteQuickSlotItem(s_iDragItem);
			break;
		}

		case	eDROPPING_ITEM_AREA_FIELD			:
		{
			// 10.01.07
			if(s_bDragSkillItem)
				g_hero.deleteQuickSlotItem(s_iDragItem);
			else
				g_hero.askDestroyItem(s_iDragItem);
//			OperateDropItem(s_iDragItem);
			break;
		}

		case	eDROPPING_ITEM_AREA_EQUIPMENT_FRAME	:
		{
			if	(s_iDragItem	==	dITEM_DRAG_GOLD					)
				break;	
			if	(s_iDragItem	>=	dBORDER_OF_ITEM_AND_EQUIPMENT	)
				break;	

			int	iFocusEquipment	=	GetFocusEquipmentInInventoryWindow(FALSE);

			// 10.01.07
			g_hero.setEquipItem(s_iFocusItem);
			break;
		}

	case	eDROPPING_ITEM_AREA_EXTRA_INVENTORY	:
		{
			if (s_iDragItem	==	dITEM_DRAG_GOLD)
				break;

			int	iFocusItem	=	GetFocusItemSlot();

			if (s_iDragItem	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
			{
				if (!g_hero.getValidInventorySlot())
					break;

				// 10.01.07
				g_hero.stripEquipItem(s_iDragItem);
				break;
			}

			if (iFocusItem	==	s_iDragItem		)
				break;	//	자기 자신이다.
			if (iFocusItem	==	dITEM_DRAG_GOLD	)
				break;	//	돈하고 바꿀 수는 없다. -_-a
			if (iFocusItem	==	0xffff			)
				break;	//	이상한곳이다.

			if	(s_aKey[KEY_LSHIFT])		// 09.08.31
			{
				cItem* lpFocusItem = g_hero.getInventoryItem(iFocusItem);
				cItem* lpDragItem = g_hero.getInventoryItem(s_iDragItem);

				if	(!lpFocusItem)
				{	
					if	(lpDragItem->m_bCount>1 && !lpDragItem->isExceptionItem())
					{
						g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,s_iDragItem,lpDragItem,eICWR_RETURN_DROP_INVENTORY_ITEM_DIVIDE, 100, iFocusItem);
					}
				}
				break;
			}
			// 10.01.07
			int skillslot1		= g_hero.getEquipItemBySlotIndex(s_iDragItem);
			int skillslot2		= g_hero.getEquipItemBySlotIndex(iFocusItem);	
			
			if(skillslot1 != 0xffff)
			{
				int EquiItemSlot1 = (skillslot1 / 2) / 10;
				int EquiItemSlot2 = (skillslot1 / 2) % 10;
				int EquiItemSlot3 = skillslot1 % 2;
				
				g_hero.m_aQuickSkill[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3]			=	iFocusItem + dEQUIPMENT_SKILL_SLOT;
				g_hero.sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,EquiItemSlot1,EquiItemSlot2,EquiItemSlot3,iFocusItem + dEQUIPMENT_SKILL_SLOT);
			}

			if(skillslot2 != 0xffff)
			{
				int EquiItemSlot1 = (skillslot2 / 2) / 10;
				int EquiItemSlot2 = (skillslot2 / 2) % 10;
				int EquiItemSlot3 = skillslot2 % 2;

				g_hero.m_aQuickSkill[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3]			=	s_iDragItem + dEQUIPMENT_SKILL_SLOT;
				g_hero.sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,EquiItemSlot1,EquiItemSlot2,EquiItemSlot3,s_iDragItem + dEQUIPMENT_SKILL_SLOT);
			}
			s_agent.sendChangeItemPlace(s_iDragItem,iFocusItem);//	s_iDragItem과 iItem의 위치를 바꾸거나 합친다.

			break;
		}

		case	eDROPPING_ITEM_AREA_INVENTORY		:
		{
			if	(s_iDragItem	==	dITEM_DRAG_GOLD)
				break;

			int	iFocusItem	=	GetFocusItemSlot();

			if (s_iDragItem	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)	
			{
				if (!g_hero.getValidInventorySlot())	break;

				// 10.01.07
				g_hero.stripEquipItem(s_iDragItem);
				break;
			}

			if	(iFocusItem	==	s_iDragItem		)
				break;	//	자기 자신이다.
			if	(iFocusItem	==	dITEM_DRAG_GOLD	)
				break;	//	돈하고 바꿀 수는 없다. -_-a
			if	(iFocusItem	==	0xffff			)
				break;	//	이상한곳이다.

			if	(s_aKey[KEY_LSHIFT])		// 09.08.31
			{
				cItem* lpFocusItem = g_hero.getInventoryItem(iFocusItem);
				cItem* lpDragItem = g_hero.getInventoryItem(s_iDragItem);

				if	(!lpFocusItem)
				{	
					if	(lpDragItem->m_bCount>1 && !lpDragItem->isExceptionItem())
					{
						g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,s_iDragItem,lpDragItem,eICWR_RETURN_DROP_INVENTORY_ITEM_DIVIDE, 100, iFocusItem);
					}
				}
				break;
			}
			// 10.01.07
			int skillslot1		= g_hero.getEquipItemBySlotIndex(s_iDragItem);
			int skillslot2		= g_hero.getEquipItemBySlotIndex(iFocusItem);	
			
			if(skillslot1 != 0xffff)
			{
				int EquiItemSlot1 = (skillslot1 / 2) / 10;
				int EquiItemSlot2 = (skillslot1 / 2) % 10;
				int EquiItemSlot3 = skillslot1 % 2;

				g_hero.m_aQuickSkill[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3]			=	iFocusItem + dEQUIPMENT_SKILL_SLOT;
				g_hero.sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,EquiItemSlot1,EquiItemSlot2,EquiItemSlot3,iFocusItem + dEQUIPMENT_SKILL_SLOT);
			}

			if(skillslot2 != 0xffff)
			{
				int EquiItemSlot1 = (skillslot2 / 2) / 10;
				int EquiItemSlot2 = (skillslot2 / 2) % 10;
				int EquiItemSlot3 = skillslot2 % 2;

				g_hero.m_aQuickSkill[EquiItemSlot1][EquiItemSlot2][EquiItemSlot3]			=	s_iDragItem + dEQUIPMENT_SKILL_SLOT;
				g_hero.sendChangeUserSkillSetting(cUserCustomDataDefine::eUSS_QUICK_SKILL,EquiItemSlot1,EquiItemSlot2,EquiItemSlot3,s_iDragItem + dEQUIPMENT_SKILL_SLOT);
			}
			s_agent.sendChangeItemPlace(s_iDragItem,iFocusItem);//	s_iDragItem과 iItem의 위치를 바꾸거나 합친다.

			break;
		}

		case	eDROPPING_ITEM_AREA_BELT			:
		{
			if	(s_iDragItem	==	dITEM_DRAG_GOLD						)
				break;
			if	(s_iDragItem	>=	dBORDER_OF_ITEM_AND_EQUIPMENT		)
				break;

			cItem	*lpDragItem	=	g_hero.getInventoryItem(s_iDragItem);

			if	(!lpDragItem)
				break;

			if	(!lpDragItem->isBeltStackAbleItem()	)
			{
				g_lpHero->warning(dMSG_CAN_NOT_EQUIP_BELT_ITEM);
				break;
			}

			if	(g_hero.m_wBeltStackCount	==	0)
			{
				g_lpHero->warning(dMSG_BELT_NOT_FOUND);
				break;
			}

			int	iFocusBeltSlot	=	GetFocusBeltSlot();

			if	(iFocusBeltSlot	==	0xffff)
				break;

			g_hero.sendEquipItem(s_iDragItem,iFocusBeltSlot,TRUE);
			break;
		}
		case eDROPPING_ITEM_AREA_UPGRADE_SHOP :		//  강화창에 드랍했다~ 09.09.01
			{
				
				g_gwUpgradeShop.operateDropItem();
				// 10.01.07
				g_hero.deleteQuickSlotItem(s_iDragItem);
				break;
			}
		// 10.01.07  스킬창에 드롭
		case eDROPPING_ITEM_AREA_SKILL:
			{
				cItem	*lpDragItem	=	g_hero.getInventoryItem(s_iDragItem);	
				
				if	(!lpDragItem)
					break;
				cBasicItem*	lpBasicItem	=	lpDragItem->getBasicItem();
				if(!lpBasicItem)
					break;
				int equipindex	= g_gwSkill.getSkillItemtoEquipmentIndex(lpDragItem->getItemType());

				if((equipindex >= dEQUIP_WEAPON && equipindex <=dEQUIP_SHOES) || equipindex == dEQUIP_WEAPON_2 || lpBasicItem->m_bf1IsUseAfterWear)
				{
					int	iDroppingSlot = g_gwSkill.getSkillDroppingSlot();

					if (iDroppingSlot	<=	eSKILL_SLOT_QUICK10)
						g_hero.setQuickSlotItem(s_iDragItem, iDroppingSlot-eSKILL_SLOT_QUICK1);
				}

				s_iDragSkill	= 0xffff;
				cINPDEV::ReleaseExclusive();

				break;
			}
	}
	// 10.01.07
	s_bDragSkillItem = FALSE;
	s_iFocusItem		=	0xffff;
	s_iFocusEquipment	=	0xffff;
	s_iDragItem			=	0xffff;
}

//
//	아이템 윈도우에 팝업 메뉴 처리
inline	void 
CGamePlay::OperateInventoryWindowPopupMenu()
{
	if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;
	if	(s_iPopupInterface	!=	ePIW_INVENTORY		)
		return;
	if	(!s_isClickedRightButton					)
		return;
	if	(s_isLeftButtonPressed						)
		return;

	if	(s_propose.m_dwValue)
	{
		s_propose.m_dwValue		=	0;
		cINPDEV::ReleaseExclusive();
		return;
	}

	int	iFocusItem	=	GetFocusItemInInventoryWindow();

	if	(iFocusItem	>=	dOWN_ITEM_COUNT+g_hero.m_bf6ExtraInventorySize	)
	{
		iFocusItem	=	GetFocusEquipmentInInventoryWindow(TRUE);

		if	(iFocusItem	==	0xffff)
			return;

		cItem	*lpItem	=	&g_hero.m_aEquip[iFocusItem];

		cBasicItem*	lpBasic	=	lpItem->getBasicItem();

		if(lpBasic)
		{
			if(lpBasic->m_bf1IsUseAfterWear)
			{
				s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
				s_ttCommon.hide();
				s_pmCommon.addMenu(ePM_ITEM_USE			,dMSG_USE_ITEM);
				s_iBookedWorkItem	=	iFocusItem+dBORDER_OF_ITEM_AND_EQUIPMENT;
				s_iFocusItem		=	0xffff;
				cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
			}
		}

		if	(!lpItem->isCoupleRing())
			return;

		{
			cCoupleRingDefine	*lpCoupleRing	=	lpItem->getCoupleRing();

			if	(lpCoupleRing && STRICMP(lpCoupleRing->getCoupleName(),g_hero.m_strName) != 0 )
			{
				s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
				s_ttCommon.hide();

				s_pmCommon.addMenu(ePM_USE_COUPLE_RING	,dMSG_USE_COUPLE_RING);

				s_iBookedWorkItem	=	iFocusItem+dBORDER_OF_ITEM_AND_EQUIPMENT;
				s_iFocusItem		=	0xffff;
				cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
			}
		}

		return;
	}

	cItem	*lpItem				=	g_hero.getInventoryItem(iFocusItem);

	if (s_pmCommon.isPopuped())
	{
		s_pmCommon.reset();

		return;
	}

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if (!lpBasicItem)
		return;

	s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
	s_ttCommon.hide();

	if	(lpItem->isMagicCarpetEmblem())
	{
		cMagicCarpetEmblemDefine	*lpEmblem	=	(cMagicCarpetEmblemDefine	*)lpItem;

		s_pmCommon.addMenu(ePM_SUMMON_MAGIC_CARPET+eMCS_NORMAL	,dMSG_MC_DISABLE_EMBLEM);

		if (lpEmblem->m_bf1IsWildCat)
			s_pmCommon.addMenu(ePM_SUMMON_MAGIC_CARPET+eMCS_WILD_CAT,dMSG_MC_SUMMON_WILD_CAT);
		if (lpEmblem->m_bf1IsTurtleCarpet)
			s_pmCommon.addMenu(ePM_SUMMON_MAGIC_CARPET+eMCS_TURTLE	,dMSG_MC_SUMMON_TURTLE);
		if (lpEmblem->m_bf1IsSquirrelCarpet)
			s_pmCommon.addMenu(ePM_SUMMON_MAGIC_CARPET+eMCS_SQUIRREL,dMSG_MC_SUMMON_SQUIRREL);
		if (lpEmblem->m_bf1IsDrakeCarpet)
			s_pmCommon.addMenu(ePM_SUMMON_MAGIC_CARPET+eMCS_DRAKE	,dMSG_MC_SUMMON_DRAKE);
		if (lpEmblem->m_bf1IsRedDrakeCarpet)
			s_pmCommon.addMenu(ePM_SUMMON_MAGIC_CARPET+eMCS_RED_DRAKE,dMSG_MC_SUMMON_RED_DRAKE);
		if (lpEmblem->m_bf1IsGearCarpet)
			s_pmCommon.addMenu(ePM_SUMMON_MAGIC_CARPET+eMCS_GEAR	,dMSG_MC_SUMMON_GEAR);

		if	(s_pmCommon.getCount())
		{
		//	s_pmCommon.addMenu(ePM_SUMMON_MAGIC_CARPET	,dMSG_MC_RELEASE_TO_SUMMON_SCROLL);

			cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
		}
		return;
	}
	if	(iFocusItem	==	dITEM_DRAG_GOLD	)
		return;

	int	iItemPremiumLevel=	lpBasicItem->m_wPremiumLevel;

	if	(iItemPremiumLevel	>=	ePSL_LITE_PLATINUM	&&	iItemPremiumLevel	!=	ePSL_ADD_PORTAL)
	{
		if	(g_map.m_bf4FieldType	!=	eFT_VILLAGE	&&	g_hero.m_wCurrentField	!=	27)
			s_pmCommon.addMenu(ePM_RETURN_TO_VILLAGE	,dMSG_RETURN_TO_VILLAGE);
	}

	if	(lpBasicItem->m_bf1IsCanSummonMagicCarpet)
		s_pmCommon.addMenu(ePM_CALL_MAGIC_CARPET	,dMSG_CALL_MAGIC_CARPET);

	if	(g_iWorldServerType	!=	eSERVER_TYPE_GVG)
	{
		if	(lpBasicItem->m_bf1RemeberPlace0)
			s_pmCommon.addMenu(ePM_REMEMBER_THIS_PLACE_TO_SLOT1	,dMSG_REMEMBER_THIS_PLACE_TO_SLOT1);

		if	(lpBasicItem->m_bf1RemeberPlace1)
			s_pmCommon.addMenu(ePM_REMEMBER_THIS_PLACE_TO_SLOT2	,dMSG_REMEMBER_THIS_PLACE_TO_SLOT2);

		if	(lpBasicItem->m_bf1RemeberPlace2)
			s_pmCommon.addMenu(ePM_REMEMBER_THIS_PLACE_TO_SLOT3	,dMSG_REMEMBER_THIS_PLACE_TO_SLOT3);

		for (int i = 0 ; i < dMAX_PLAYER_WAY_POINT_COUNT ; i++)
		{
			BOOL		bCheck	=	FALSE;
			int			eNUMdefine	=	0;

			switch(i)
			{
				case 0:
					bCheck			=	lpBasicItem->m_bf1RemeberPlace0;
					eNUMdefine		=	ePM_WARP_TO_WAYPOINT_SLOT1;
					break;

				case 1:
					bCheck			=	lpBasicItem->m_bf1RemeberPlace1;
					eNUMdefine		=	ePM_WARP_TO_WAYPOINT_SLOT2;
					break;

				case 8:
					bCheck			=	lpBasicItem->m_bf1RemeberPlace2;
					eNUMdefine		=	ePM_WARP_TO_WAYPOINT_SLOT3;
					break;
			}

			if	(!bCheck)
				continue;

			int		iField			=	g_hero.m_aWayPoint[i].m_bf12Field;
			int		iX				=	g_hero.m_aWayPoint[i].m_bf10XPos;
			int		iY				=	g_hero.m_aWayPoint[i].m_bf10YPos;

			if	(iField	<	dINSTANCE_FIELD_FLAG)
			{
				char	*lpstrFieldName	=	cMAP::GetFieldName(iField);
				char	strText[256];
				
				sprintf(strText,dMSG_WARP_TO_WAYPOINT_FORM,lpstrFieldName,iX,iY);
				
				s_pmCommon.addMenu(eNUMdefine,strText);
			}
		}
	}	

	if	(lpItem->isGateGlove()	&&	g_hero.m_wRemainSecondForUseGateGlove == 0 && g_hero.m_wCurrentField	!=	27)
		s_pmCommon.addMenu(ePM_USE_GATE_GLOVE	,dMSG_USE_GATE);

	{
		cCoupleRingDefine	*lpCoupleRing	=	lpItem->getCoupleRing();

		if	(lpCoupleRing && STRICMP(lpCoupleRing->getCoupleName(),g_hero.m_strName) != 0 )
			s_pmCommon.addMenu(ePM_USE_COUPLE_RING	,dMSG_USE_COUPLE_RING);
	}

	if	(lpItem->isIncreaseSelectPowerItem())
	{
		BOOL bIsChange = TRUE;
		for	(int i=0;i<dGENERATE_ITEM_DATA_COUNT;++i)
		{
			if(lpBasicItem->m_aGenerateData[i].m_wEffect == eIE_INCREASE_FIX_SELECT_POWER)
			{
				if(lpItem->m_aOption[1] != 0)
				{
					bIsChange = FALSE;
				}
			}
		}
		if(bIsChange)
		{
			s_pmCommon.addMenu(ePM_USE_SET_ISPI_STRENGTH	,dMSG_ISPI_STRENGTH);
			s_pmCommon.addMenu(ePM_USE_SET_ISPI_AGILITY		,dMSG_ISPI_AGILITY);
			s_pmCommon.addMenu(ePM_USE_SET_ISPI_CONSTITUTION,dMSG_ISPI_CONSTITUTION);
			s_pmCommon.addMenu(ePM_USE_SET_ISPI_CHARISMA	,dMSG_ISPI_CHARISMA);
			s_pmCommon.addMenu(ePM_USE_SET_ISPI_INTELLIGENCE,dMSG_ISPI_INTELLIGENCE);
			s_pmCommon.addMenu(ePM_USE_SET_ISPI_WISDOM		,dMSG_ISPI_WISDOM);
			s_pmCommon.addMenu(ePM_USE_SET_ISPI_LUCK		,dMSG_ISPI_LUCK);
		}
	}

	if	(lpItem->isBook())
		s_pmCommon.addMenu(ePM_READ_BOOK		,dMSG_READ_BOOK);

	if	(lpItem->isVerticalBanner()	||	lpItem->isHorizonBanner())
		s_pmCommon.addMenu(ePM_PUT_BANNER		,dMSG_PUT_BANNER);

	if	(lpItem->isBottomlessBox() || lpItem->isExistSpecificEffect(eIE_CH5_MIX_ITEM_BOX))
		s_pmCommon.addMenu(ePM_USE_BOTTOMLESS_BOX,dMSG_USE_ITEM);

	if	(g_hero.isConsumeItem(iFocusItem) || lpBasicItem->m_attr.isUseToCorpse)
	{
		if(!lpBasicItem->m_bf1IsUseAfterWear)
			s_pmCommon.addMenu(ePM_ITEM_USE			,dMSG_USE_ITEM);
	}
	if(lpBasicItem->m_bf1IsUseShop)			// 상점 이용하기
		s_pmCommon.addMenu(ePM_USE_SHOP		,dMSG_USE_SHOP);
	if(lpBasicItem->m_bf1IsUseBank)			// 은행 이용하기
		s_pmCommon.addMenu(ePM_USE_BANK		,dMSG_USE_BANK);
	if	(lpItem->isCanNotDestroyItem()	==	FALSE)
		s_pmCommon.addMenu(ePM_DESTROY_ITEM		,dMSG_DESTROY_ITEM);

	if	(s_pmCommon.getCount())
	{
		s_iBookedWorkItem	=	iFocusItem;
		s_iFocusItem		=	0xffff;
		cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
	}
	else
		s_pmCommon.reset();
}

//
//	아이템 윈도우 처리
void 
CGamePlay::OperateInventoryWindow()
{
	OperateInventoryWindowPopupMenu();	//	아이템 윈도우 스크롤바 업데이트

	if	(g_gwCarrotShop.m_wWaitOpenResultTime	>	0)
		g_gwCarrotShop.m_wWaitOpenResultTime--;

	if	(cINPDEV::IsExclusive())
		return;

	if	(s_iDragItem	!=	0xffff)
		return;	//	드래그 하는 아이템이 있다.

	if	(s_isLeftButtonPressed	==	FALSE)
	{
		s_iFocusItem	=	0xffff;
	
		if	(IsValidFocus())
			s_iFocusItem	=	GetFocusItemInInventoryWindow();
	}

	if	(s_iFocusItem!=	0xffff	&&	s_isLeftButtonPressed	==	FALSE)
	{
		if	(s_iFocusItem!=	dITEM_DRAG_GOLD)
		{
			///////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////// KH_7 변화된 수치 //////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////
			char		*lpStrComment;
			cItem		itemFocus;

			if	(s_iFocusItem	<	dOWN_ITEM_COUNT)
			{
				lpStrComment	=	s_lpInventoryWindowItemList[s_iFocusItem].getItemTooltip(-1,-1,TRUE);
				memcpy(&itemFocus,&s_lpInventoryWindowItemList[s_iFocusItem],sizeof(cItem));
			}
			else
			{
				lpStrComment	=	g_hero.m_aExtraInventory[s_iFocusItem-dOWN_ITEM_COUNT].getItemTooltip(-1,-1,TRUE);
				memcpy(&itemFocus,&g_hero.m_aExtraInventory[s_iFocusItem-dOWN_ITEM_COUNT],sizeof(cItem));
			}

			if	(itemFocus.getBasicItem())
			{
				lpStrComment		=	g_hero.getAfterEquipInfo(lpStrComment,&itemFocus);
				
				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,lpStrComment,eTFM_LEFT);

				if	(itemFocus.isItemPiece() && itemFocus.isUnknownItem() == FALSE)
				{
					cItem	pieceItem;

					if	(itemFocus.generatePieceItem(&pieceItem))
					{
						cBasicItem	*lpBasicItem	=	pieceItem.getBasicItem();

						if	(lpBasicItem	&&	lpBasicItem->m_wIconShape	<	g_smiIconItem.m_iCount)
						{
							WORD		*lpImage		=	g_smiIconItem.get16Image(lpBasicItem->m_wIconShape);

							s_ttCommon.setImage(lpImage);
						}
					}
				}
			}
		}
	}

	s_iFocusEquipment	=	0xffff;

	if	(IsValidFocus())	
		s_iFocusEquipment	=	GetFocusEquipmentInInventoryWindow();
	
	if	(s_iFocusEquipment	!=	0xffff	&&	s_isLeftButtonPressed	==	FALSE)
	{
		cItem	*lpEquipment=	&s_lpInventoryWindowEquipmentList[s_iFocusEquipment];

		if	(lpEquipment->m_wBaseItem	!=	0xffff)
		{
			char	*strComment	=	lpEquipment->getItemTooltip();

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);
		}
	}
}

//
//	인벤토리 업데이트
void
CGamePlay::UpdateInventoryWindow()
{
	BOOL	isUseMagicCapetByHotKey = FALSE;

	if(s_aKey[KEY_Y] && !g_lpHero->m_wMagicCarpetType && s_inputChatting.isEnable() == FALSE && g_hero.m_bf1IsHaveSummonMagicCarpetItem)
		isUseMagicCapetByHotKey = TRUE;

#ifdef	_DEBUG
	if	(g_strMinimapCheckNpc[0] && s_isTouchLeftButton && s_aKey[KEY_LSHIFT] && s_aKey[KEY_LCONTROL] )
	{
		int	iFocusItem	=	GetFocusItemInInventoryWindow();

		if	(iFocusItem != 0xffff)
		{
			cItem*	lpItem		=	g_hero.getInventoryItem(iFocusItem);
			int		iBaseItem	=	lpItem->m_wBaseItem;

			for (int i=0;i<g_hero.m_wItemSlotCount;i++)
			{
				lpItem		=	g_hero.getInventoryItem(i);

				if	(lpItem && lpItem->m_wBaseItem == iBaseItem)
				{
					s_agent.sendEtcWork(eCEW_FEED_ITEM_TO_MINIPET,g_gwMiniPetStatus.m_iSelectMiniPet,i);
					Sleep(20);
				}
			}
		}
	}
#endif

	if	(!isUseMagicCapetByHotKey)
	{
		int	iICWR	=	g_itemCountWindow.update();

		if (iICWR)
		{
			cINPDEV::ReleaseExclusive();

			switch(iICWR)
			{
				case	eICWR_CANCEL							:
					g_hero.cancelBookedWorkWithItemCount();
					break;

				case	eICWR_RETURN_DROP_ITEM_COUNT			:
				case	eICWR_RETURN_DROP_GOLD					:
					g_hero.operateBookedWorkWithItemCount(g_itemCountWindow.m_iCount);
					break;

				case	eICWR_RETURN_SELL_ITEM_COUNT			:
					if (g_gwShop.isOpened() && g_itemCountWindow.m_iCount > 0)
						s_agent.sendSellItem(s_shop.m_wSerial,s_shop.m_wOwner,g_gwShop.m_iSellItemSlot,g_itemCountWindow.m_iCount,g_gwShop.m_bIsBadgeOfTraderShop);	// 
					break;

				case	eICWR_RETURN_TRADE_ITEM_COUNT			:
					if (IsOpenTradeBox())	s_agent.sendAddTradeItem(s_iTradeItemSlot,g_itemCountWindow.m_iCount);
					break;

				case	eICWR_RETURN_CHANGE_TRADE_ITEM_COUNT	:
					if (IsOpenTradeBox())	s_agent.sendChangeTradeItemData(s_iChangeTradeItemSlot,s_iTradeItemSlot,g_itemCountWindow.m_iCount);
					break;

				case	eICWR_RETURN_TRADE_GOLD					:
					//VIRTUALIZER_START
					if	(IsOpenTradeBox())
					{
						s_agent.sendSetTradeGold(g_itemCountWindow.m_iCount);
					}
					//VIRTUALIZER_END
					break;

				case	eICWR_RETURN_STORE_GOLD_TO_THE_BANK		:
					if	(g_gwBank.isOpened())
						g_gwBank.storeGoldToTheBank(g_itemCountWindow.m_iCount);
					break;

				case	eICWR_RETURN_WITHDRAW_GOLD_FROM_GUILD_INVENTORY	:
					if	(g_gwGuildInventory.isOpened())
						g_gwGuildInventory.withdrawGold(g_itemCountWindow.m_iCount);
					break;
				case	eICWR_RETURN_STORE_GOLD_TO_GUILD_INVENTORY		:
					if	(g_gwGuildInventory.isOpened())
						g_gwGuildInventory.storeGold(g_itemCountWindow.m_iCount);
					break;

				case	eICWR_RETURN_WITHDRAW_GOLD_FROM_THE_BANK:
					if (g_gwBank.isOpened())	g_gwBank.withdrawGoldFromTheBank(g_itemCountWindow.m_iCount);
					break;

				case	eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT	:
					if (g_gwPitchmanShop.isOpened())	
						g_gwPitchmanShop.askAddSellingItem(g_itemCountWindow.m_iItemSlot,g_itemCountWindow.m_iPrice, g_itemCountWindow.m_bDenomination); // 09.08.21
					break;
				case eICWR_RETURN_DROP_INVENTORY_ITEM_DIVIDE:
					{
						s_agent.sendEtcWork(eCEW_DRAG_ITEM_DIVIDE, g_itemCountWindow.m_iItemSlot, g_itemCountWindow.m_iFocusSlot , g_itemCountWindow.m_iCount);		// 09.09.01
						break;
					}
			}
		}

	//	if	(g_hero.m_wPreminumServiceLevel	<	ePSL_PREMIUM)
		if	(g_hero.m_bf1IsHaveSummonMagicCarpetItem == 0)
			s_bmButtonI2.disable(eIWM_CALL_MAGIC_CARPET);

		if	(g_hero.iMiniPet()->getMiniPetCount()	<=	0)
			s_bmButtonI2.disable(eIWM_TOGGLE_MINIPET_WINDOW);
		else
			s_bmButtonI2.enable(eIWM_TOGGLE_MINIPET_WINDOW);

		if	(s_iPopupInterface	!=	ePIW_INVENTORY && !isUseMagicCapetByHotKey )	
			return;

		if	(s_iPopupInterface	==	ePIW_INVENTORY)
		{
			int	iWidth				=	g_sprInterface.getSpriteWidth(eWINDOW);

			s_rectInterfaceSize.x1 = g_iScreenWidth-iWidth;
			s_rectInterfaceSize.x2 = g_iScreenWidth;

			if	(g_hero.m_bf6ExtraInventorySize)
				s_rectInterfaceSize.x1 -= dEXTRA_INVENTORY_TITLE_WIDTH;
			if	(g_hero.iMiniPet()->getMiniPetCount() && g_config.m_bf1IsShowMiniPetWindow)
				s_rectInterfaceSize.x1 -= iWidth;
		}

		g_gwMiniPetStatus.update();

		if	(cINPDEV::IsExclusive(eMOUSE_STATE_EXCLUSIVE_BY_INVENTORY_WINDOW))
		{
			if	(s_propose.m_work.isReadyUseItemToItem)
			{
				s_iFocusItem	=	GetFocusItemInInventoryWindow();

				cItem	*lpFocusItem	=	NULL;

				if	(s_iFocusItem	>=	dITEM_DRAG_GOLD)
					lpFocusItem	=	NULL;
				else
				if	(s_iFocusItem	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
					lpFocusItem	=	&s_lpInventoryWindowEquipmentList[s_iFocusItem-dBORDER_OF_ITEM_AND_EQUIPMENT];
				else
				if	(s_iFocusItem	>=	dOWN_ITEM_COUNT)
					lpFocusItem	=	&g_hero.m_aExtraInventory[s_iFocusItem-dOWN_ITEM_COUNT];
				else
				if	(s_iFocusItem	>=	0)
					lpFocusItem	=	&s_lpInventoryWindowItemList[s_iFocusItem];

				if	(lpFocusItem)
				{
					char	*strComment	=	lpFocusItem->getName();
					char	strText[128];

					if	(lpFocusItem->isExtraItem(TRUE))
					{
						cExtraItemDefine	*lpExItem	=	(cExtraItemDefine	*)lpFocusItem;

						sprintf(strText,"%s [<c:LTYELLOW>%d%s<n>]",strComment,lpExItem->getRemainTimeEfficency(),dMSG_PERCENTAGE);
					}
					else
						strcpy(strText,strComment);

					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strText,eTFM_LEFT);
				}
			}
			return;
		}

	//	각종 메뉴들의 툴팁
		{
			cBAR	*lpFocusedBar	=	s_bmButtonI2.getFocusMenu(s_posMouse.x,s_posMouse.y);

			if	(l_bIsShowCarrotButton)
				if	(!lpFocusedBar)
					lpFocusedBar	=	s_bmCarrotShop.getFocusMenu(s_posMouse.x,s_posMouse.y);

			//if	(!lpFocusedBar)
			//	lpFocusedBar		=	s_bmButtonI2.getFocusMenu(s_posMouse.x,s_posMouse.y);

			if	(lpFocusedBar)
			{
				cRECT	rect;

				rect.set(&lpFocusedBar->m_rectCrash);
				rect.add(0,-40);

				char	*lpstrComment	=	lpFocusedBar->getTooltip();

				if	(lpstrComment)
				{
					s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,lpstrComment);
				}
			}

		}	//	각종 메뉴들의 툴팁

	}

	DWORD	dwMenu	=	s_bmButtonI2.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);	

	if(isUseMagicCapetByHotKey)
		dwMenu	=	eIWM_CALL_MAGIC_CARPET;

	//if	(dwMenu	==	0xffffffff)
	//	dwMenu	=	s_bmButtonI2.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if	(dwMenu	==	0xffffffff	&&	l_bIsShowCarrotButton)
		dwMenu	=	s_bmCarrotShop.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	eIWM_TRADE				:
			s_propose.m_work.isProposeToTrade	=	TRUE;
			cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);
			break;

		case	eIWM_OPEN_SHOPPING_CART	:
		{
#ifndef _DEBUG
			if	(g_bIsTestServer || g_bIsDuelServer)
				break;
#endif
			g_msgBox.cPopup("",dMSG_ARE_YOU_OPEN_SHOPPING_CART,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_OPEN_SHOPPING_CART);
			break;
		}

		case	eIWM_CALL_MAGIC_CARPET	:
		{
			g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_CALL_MAGIC_CARPET,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_CALL_MAGIC_CARPET);
			break;
		}

		case	eIWM_OPEN_PICHMAN_SHOP		:
		{
			if(g_map.m_bf1IsNotOpenPitchManShop)
			{
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_OPEN_PITCHMAN_SHOP_THIS_FIELD);
				break;
			}
			if (g_hero.m_bf1IsHidePitchmanShop)
				g_msgBox.cPopup("",dMSG_CAN_NOT_OPEN_PITCHMAN_SHOP_WHEN_HIDE_PICHMAN_SHOP_STATUS,dMSG_OK);
			else
			{
				g_msgBox.cPopup("",dMSG_DO_YOU_OPEN_PITCHMAN_SHOP,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_OPEN_PITCHMAN_SHOP);
			}
			break;
		}
		case	eIWM_TOGGLE_MINIPET_WINDOW		:
		{
			g_config.m_bf1IsShowMiniPetWindow	=	1-g_config.m_bf1IsShowMiniPetWindow;
			break;
		}

		case	eIWM_OPEN_CARROT_SHOP				:
		{
			if	(g_gwCarrotShop.isOpened())
				break;

			if	(g_gwCarrotShop.m_wWaitOpenResultTime)
			{
				CGamePlay::AddSystemMessage(LTYELLOW,dMSG_WAIT_FOR_CART_DATA);
				break;
			}

			g_gwCarrotShop.m_wWaitOpenResultTime	=	0;
			g_gwCarrotShop.open();
			break;
		}
#ifdef	_FOR_JAPAN					//JBC 08-08-12
		case	eIWM_NETCAFEMENU					:
		{
			if	(!g_hero.m_bf1IsNetCafeUser)
				break;
	

			if (s_pmCommon.isPopuped())
			{
				s_pmCommon.reset();	
				return;
			}

			s_pmCommon.popup(s_posMouse.x,s_posMouse.y);

			if(g_hero.m_wCurrentField	!=	27)
				s_pmCommon.addMenu(ePM_NETCAFE_GATE_GLOVE	,dMSG_USE_GATE);
		
			break;
		}
#endif
	}

	if	(l_bIsShowCarrotButton)
	{
		if	(!s_bmCarrotShop.isPressed() && !s_bmButtonI2.isPressed())
			cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_EXCLUSIVE_BY_INVENTORY_WINDOW);
		else
		{
			cINPDEV::SetExclusive(eMOUSE_STATE_EXCLUSIVE_BY_INVENTORY_WINDOW);
			return;
		}
	}
	else
	{
		if	(!s_bmButtonI2.isPressed())
			cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_EXCLUSIVE_BY_INVENTORY_WINDOW);
		else
		{
			cINPDEV::SetExclusive(eMOUSE_STATE_EXCLUSIVE_BY_INVENTORY_WINDOW);
			return;
		}
	}

	//s_bmButtonI2.setTooltop(DWORD _dwMenu,char *_lpstrTooltip);
	
	OperateInventoryWindow();
	OperateDragItem();
}

//
//	아이템을 어디다 드롭 했냐?
int
CGamePlay::GetItemDroppingArea()
{
	cRECT	rectFrame;

	int		iX	=	s_posInventoryWindow.x;
	int		iY	=	s_posInventoryWindow.y;

	if	(s_iPopupInterface	==	ePIW_SHOP	&&	g_gwShop.m_iShopKind	==	eSK_NORMAL)
	{
		if	(g_gwShop.m_rect.isIn(s_posMouse.x,s_posMouse.y))	
			return	eDROPPING_ITEM_AREA_SHOP;

		//JBC	샵에서도 아이템을 장비할수 있게함.	08-09-01
		rectFrame.set(iX+12,iY+8,iX+260,iY+122);
		if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_EQUIPMENT_FRAME;

		rectFrame.set(298,238,769,918);	
		if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_INVENTORY;

		return	0xffff;
	}

	if	(s_iPopupInterface	==	ePIW_GUILD_FEED_PET_WINDOW	)
	{
		if	(g_gwFeedGuildPet.m_rect.isIn(s_posMouse.x,s_posMouse.y))	
			return	eDROPPING_ITEM_AREA_FEED_PET;

		return	0xffff;
	}

	if	(s_iPopupInterface	==	ePIW_BOTTOMLESS_BOX)
	{
		if	(g_gwBottomlessBoxWindow.m_rect.isIn(s_posMouse.x,s_posMouse.y))	
			return	eDROPPING_ITEM_AREA_BOTTOMLESS_BOX;

		return	0xffff;
	}

	if	(s_iPopupInterface	==	ePIW_PITCHMAN_SHOP)
	{
		if	(g_gwPitchmanShop.m_rect.isIn(s_posMouse.x,s_posMouse.y))	
			return	eDROPPING_ITEM_AREA_PITCHMAN_SHOP;

		return	0xffff;
	}

	if (s_iPopupInterface	==	ePIW_TRADE_BOX)
	{
		cRECT	rect;

		rect.set(139,129,254,393);
		rect.add(s_rectTradeBox.x1,0);
		
		if	(rect.isIn(s_posMouse.x,s_posMouse.y))	
			return	eDROPPING_ITEM_AREA_TRADE_BOX;

		return	0xffff;
	}

	if	(s_iPopupInterface	==	ePIW_BANK)
	{
		cRECT	rect;
		rect.set(280+g_iCorrectBI_X,32+g_iCorrectBI_Y,516+g_iCorrectBI_X,450+g_iCorrectBI_Y);
		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_BANK;

		rect.set(iX+18,iY+206,iX+252,iY+468);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_INVENTORY_WHEN_BANK_TRANSACTION;

		return	0xffff;
	}

	if	(s_iPopupInterface	==	ePIW_GUILD_INVENTORY)
	{
		cRECT	rect;

		int	iTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);
		int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

		rect.set(280+g_iCorrectBI_X, g_iCorrectBI_Y/2+iTitleHeight, 516+g_iCorrectBI_X, g_iCorrectBI_Y/2+iWindowHeight);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_GUILD_INVENTORY;

		rect.set(iX+18,iY+206,iX+252,iY+468);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_INVENTORY_WHEN_OPEN_GUILD_INVENTORY;

		return	0xffff;
	}

	if (s_iPopupInterface	==	ePIW_SHOPPING_CART)
	{
		cRECT	rect;

		rect.set(iX+18,iY,iX+252,iY+468);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_INVENTORY_WHEN_CART_TRANSACTION;

		return	0xffff;
	}

	if (s_iPopupInterface	==	ePIW_INVENTORY || s_iPopupInterface	==	ePIW_SHOP || s_bDragSkillItem == TRUE)
	{
//	아이템 창
		rectFrame.set(iX+18,iY+206,iX+252,iY+468);
		if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_INVENTORY;

//	장비창
		rectFrame.set(iX+12,iY+8,iX+260,iY+122);
		if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_EQUIPMENT_FRAME;

		if (g_hero.m_bf6ExtraInventorySize)
		{
			int	iYPos	=	s_posInventoryWindow.y+dEXTRA_INVENTORY_VERTICAL_POS-2;

			rectFrame.set(iX-dEXTRA_INVENTORY_TITLE_WIDTH,iYPos+8,iX,iYPos+342);

			if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
				return	eDROPPING_ITEM_AREA_EXTRA_INVENTORY;
		}

		if	(s_iPopupInterface == ePIW_INVENTORY && g_hero.iMiniPet()->getMiniPetCount() && g_config.m_bf1IsShowMiniPetWindow)
		{
			if	(g_gwMiniPetStatus.m_rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				return	eDROPPING_ITEM_AREA_MINIPET;
			}
		}

		rectFrame.set(g_iScreenWidth - 714,g_iScreenHeight - 76,g_iScreenWidth - 536,g_iScreenHeight -  3);
		if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
			return	eDROPPING_ITEM_AREA_SKILL;
	}
	if(s_iPopupInterface == ePIW_UPGRADE_SHOP)		// 강화창에 드래그~ 09.09.01
	{
		cRECT* rt = g_gwUpgradeShop.getUpgradeWindowRect();
		if(rt->isIn(s_posMouse.x, s_posMouse.y))
		{
			return eDROPPING_ITEM_AREA_UPGRADE_SHOP;
		}
		
	}
//	필드
	rectFrame.set(0,0,g_iScreenWidth-(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1),498);
	if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
		return	eDROPPING_ITEM_AREA_FIELD;

//	벨트
	rectFrame.set(g_iScreenWidth - 266,g_iScreenHeight - 76,g_iScreenWidth - 88,g_iScreenHeight - 42);
	if (rectFrame.isIn(s_posMouse.x,s_posMouse.y))
		return	eDROPPING_ITEM_AREA_BELT;

	return	eDROPPING_ITEM_AREA_NONE;
}


//
//	포커스 아이템
int
CGamePlay::GetFocusItemInInventoryWindow()
{
	int		iX,iY,i,j;
	int		iFocusItem	=	0xffff;
	cRECT	rect;
	CPos	pos;

	iX		=	s_posInventoryWindow.x;
	iY		=	s_posInventoryWindow.y;

	for (i=0;i<dITEM_VERTICAL_COUNT_AT_INVENTORY_WINDOW;i++)
	{
		for (j=0;j<dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW;j++)
		{
			pos.x	=	iX+19 +dITEM_SLOT_WIDTH*j;
			pos.y	=	iY+214+dITEM_SLOT_HEIGHT*i;

			int			iItemSlot	=	i*dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW+j;
			cItem		*lpItem		=	NULL;
			cBasicItem	*lpBaseItem	=	NULL;

			if	(iItemSlot	>=	g_hero.getItemSlotCount())
				continue;

			lpItem		=	&s_lpInventoryWindowItemList[iItemSlot];

			if (!lpItem->getBasicItem())
				continue;

			if	(IsInvisibleItem(iItemSlot))
			{
				if (IsOpenTradeBox())
				{
					cItem	tradeItem;
					GetTradeItem(iItemSlot,&tradeItem);

					if (tradeItem.m_bCount	>=	lpItem->m_bCount)
						continue;
				}
				else
					continue;
			}

			if(s_iPopupInterface == ePIW_UPGRADE_SHOP)	//  09.09.01
			{
				if(g_gwUpgradeShop.getUpgradeWeaponItemSlot() == iItemSlot)
					continue;
				if(g_gwUpgradeShop.getUpgradeItemSlot() == iItemSlot)
					continue;
				if(g_gwUpgradeShop.getUpgradeNormalItemSlot() == iItemSlot)
					continue;
				
			}
			rect.set(pos.x-3,pos.y-1,pos.x+dITEM_SHAPE_WIDTH+2,pos.y+dITEM_SHAPE_HEIGHT+1);

			if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				iFocusItem			=	iItemSlot;
				s_rectTooltip.set(&rect);
				s_rectTooltip.add(-4,0);
				break;
			}
		}
	}

	if	(s_iPopupInterface	==	ePIW_INVENTORY)
	{
		int	iYPos	=	s_posInventoryWindow.y+dEXTRA_INVENTORY_VERTICAL_POS-2;

		for (i=0;i<dITEM_VERTICAL_COUNT_AT_EXTRA_INVENTORY_WINDOW;i++)
		{
			for (j=0;j<dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW;j++)
			{
				int			iItemSlot	=	i*dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW+j;

				if (iItemSlot	>=	g_hero.m_bf6ExtraInventorySize)
					break;

				if (g_hero.m_aExtraInventory[iItemSlot].m_wBaseItem	==	0xffff)
					continue;

				cItem		tradeItem,putItem;
				cBasicItem	*lpBaseItem	=	NULL;

				pos.x	=	iX		+19+dEXTRA_INVENTORY_ITEM_SLOT_WIDTH*j-dEXTRA_INVENTORY_TITLE_WIDTH;
				pos.y	=	iYPos	+21+dEXTRA_INVENTORY_ITEM_SLOT_HEIGHT*i;

				rect.set(pos.x-3,pos.y-1,pos.x+dITEM_SHAPE_WIDTH+2,pos.y+dITEM_SHAPE_HEIGHT+1);

				if (rect.isIn(s_posMouse.x,s_posMouse.y))
				{
					iFocusItem			=	dOWN_ITEM_COUNT+iItemSlot;
					s_rectTooltip.set(&rect);
					s_rectTooltip.add(-4,0);
					break;
				}
			}
		}
	}

	rect.set(17,187,17+27,187+27);
	rect.add(iX,iY);

	if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		return	dITEM_DRAG_GOLD;

	return	iFocusItem;
}

//
//	포커스 아이템
int
CGamePlay::GetFocusItemSlot()
{
	if (s_pmCommon.getCount())
		return 0xffff;

	int		iX,iY,i,j;
	int		iFocusItem	=	0xffff;
	cRECT	rect;
	CPos	pos;

	iX		=	s_posInventoryWindow.x;
	iY		=	s_posInventoryWindow.y;

	for (i=0;i<dITEM_VERTICAL_COUNT_AT_INVENTORY_WINDOW;i++)
	{
		for (j=0;j<dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW;j++)
		{
			pos.x	=	iX+19 +dITEM_SLOT_WIDTH*j;
			pos.y	=	iY+214+dITEM_SLOT_HEIGHT*i;

			int	iItemSlot	=	i*dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW+j;

			if	(IsInvisibleItem(iItemSlot))
				continue;

			if (iItemSlot	>=	g_hero.getItemSlotCount())
				continue;

			rect.set(pos.x-3,pos.y-1,pos.x+dITEM_SHAPE_WIDTH+2,pos.y+dITEM_SHAPE_HEIGHT+1);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
				return	iItemSlot;
		}
	}

	if (s_iPopupInterface	==	ePIW_INVENTORY)
	{
		int	iYPos	=	s_posInventoryWindow.y+dEXTRA_INVENTORY_VERTICAL_POS-2;

		for (i=0;i<dITEM_VERTICAL_COUNT_AT_EXTRA_INVENTORY_WINDOW;i++)
		{
			for (j=0;j<dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW;j++)
			{
				int			iItemSlot	=	i*dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW+j;

				if (iItemSlot	>=	g_hero.m_bf6ExtraInventorySize)
					break;

				cItem		*lpItem		=	NULL;
				cItem		tradeItem,putItem;
				cBasicItem	*lpBaseItem	=	NULL;

				pos.x	=	iX	 +19+dEXTRA_INVENTORY_ITEM_SLOT_WIDTH*j-dEXTRA_INVENTORY_TITLE_WIDTH;
				pos.y	=	iYPos+21+dEXTRA_INVENTORY_ITEM_SLOT_HEIGHT*i;

				rect.set(pos.x-3,pos.y-1,pos.x+dITEM_SHAPE_WIDTH+2,pos.y+dITEM_SHAPE_HEIGHT+1);

				if (rect.isIn(s_posMouse.x,s_posMouse.y))
					return	iItemSlot+dOWN_ITEM_COUNT;
			}
		}
	}

	return	0xffff;
}

//
//	초점이된 장비
int
CGamePlay::GetFocusEquipmentInInventoryWindow(BOOL _isWantOnlyAvailItem)
{
	int		iFocusPart	=	0xffff;
	int		iX			=	s_posInventoryWindow.x;
	int		iY			=	s_posInventoryWindow.y;
	CPos	pos;
	cRECT	rect;

	int		iPart	=	0;

	for (iPart=0;;iPart++)
	{
		if	(l_aEquipmentPos[iPart].x	==	0xffff)
			break;

		cItem		*lpItem				=	&s_lpInventoryWindowEquipmentList[iPart];

		if	(IsOpenTradeBox() && iPart>=dEQUIP_BELT_SLOT1 && iPart<=dEQUIP_BELT_SLOT5	)
			continue;

		if	(IsInvisibleItem(iPart+dBORDER_OF_ITEM_AND_EQUIPMENT)	)
		{
			if (IsOpenTradeBox())
			{
				cItem	tradeItem;

				GetTradeItem(iPart+dBORDER_OF_ITEM_AND_EQUIPMENT,&tradeItem);

				if (tradeItem.m_bCount	>=	lpItem->m_bCount)
					continue;
			}
		}

		pos.x	=	iX+l_aEquipmentPos[iPart].x;
		pos.y	=	iY+l_aEquipmentPos[iPart].y;

		if	(_isWantOnlyAvailItem)
		{
			cBasicItem	*lpBaseItem		=	lpItem->getBasicItem();

			if	(!lpBaseItem)
				continue;
		}

		if	(iPart	>=	dEQUIP_RING1 && iPart	<=	dEQUIP_RING8)
			rect.set(pos.x,pos.y,pos.x+dITEM_SMALL_SHAPE_WIDTH	,pos.y+dITEM_SMALL_SHAPE_HEIGHT);
		else
			rect.set(pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH		,pos.y+dITEM_SHAPE_HEIGHT);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(-4,0);

			iFocusPart	=	iPart;
			break;
		}
	}

	if	(iFocusPart	== dEQUIP_WEAPON	&& g_hero.m_bf1IsHideWeapon1)	 
		iFocusPart	=	0xffff;
	if	(iFocusPart	== dEQUIP_WEAPON_2	&& g_hero.m_bf1IsHideWeapon2)	 
		iFocusPart	=	0xffff;

	return	iFocusPart;
}

//
//	초점이된 벨트 슬롯
int
CGamePlay::GetFocusBeltSlot()
{
	int		iFocusItem	=	0xffff,iIndex=0;

	for (iIndex=0;s_arectBeltSlot[iIndex].x1	!=	0xffff;iIndex++)
	{
		if (s_arectBeltSlot[iIndex].isIn(s_posMouse.x,s_posMouse.y))
		{
			cItem	*lpItem		=	g_hero.getBeltItem(iIndex);

			iFocusItem	=	iIndex+dEQUIP_BELT_SLOT1;

			break;
		}
	}

	return	iFocusItem;
}

//
//	레어 심볼 찍기
void
CGamePlay::PutItem(cItem *_lpItem,int _iX,int _iY,BOOL _bIsPutCount,BOOL _bIsSmallSize,int _iTwinkleFrame,BOOL _bIsRed)
{
	if	(!_lpItem)
		return;

	cBasicItem	*lpBasicItem	=	NULL;

	lpBasicItem	=	_lpItem->getBasicItem(TRUE);

	if	(!lpBasicItem)
		return;

	if	(lpBasicItem->m_wIconShape	>=	g_smiIconItem.m_iCount)
	{
		if	(_bIsSmallSize)
			g_sprInterface.Put(_iX,_iY,eIWI_BROKEN_ITEM,70,70);
		else
			g_sprInterface.Put(_iX,_iY,eIWI_BROKEN_ITEM);

		return;
	}

	if	(_bIsSmallSize)
	{
		g_smiIconItem.putScale(lpBasicItem->m_wIconShape,_iX,_iY,70);

		if	(_lpItem->isExceptionItem() == FALSE && _lpItem->m_isBroken)
			g_sprInterface.Put(_iX,_iY,eIWI_BROKEN_ITEM,70,70);

		BOOL	isBroken	=	_lpItem->m_isBroken;
		_lpItem->m_isBroken	=	FALSE;

		if	(!g_hero.isUseAbleItem(_lpItem) || _bIsRed)
			cDRAW::FillAB(_LTRED,_iX,_iY,_iX+dITEM_SMALL_SHAPE_WIDTH,_iY+dITEM_SMALL_SHAPE_HEIGHT,12);

		if	(_lpItem->isUltimateItem())
		{
			int		iFrame	=	(s_iFrameCounter*g_anmItemType.GetFPS(eITI_ULTIMATE)/dSYNC_FPS)%g_anmItemType.GetFrameCount(eITI_ULTIMATE);
			g_anmItemType.PutReg(_iX+8,_iY+12,eITI_ULTIMATE,0,iFrame);
		}
		else
		if	(_lpItem->isDXItem())
		{
			int		iFrame	=	(s_iFrameCounter*g_anmItemType.GetFPS(eITI_DX)/dSYNC_FPS)%g_anmItemType.GetFrameCount(eITI_DX);
			g_anmItemType.PutReg(_iX+8,_iY+12,eITI_DX,0,iFrame);
		}

		_lpItem->m_isBroken	=	isBroken;
	}
	else
	{
		g_smiIconItem.put(lpBasicItem->m_wIconShape,_iX,_iY);

		if	(_lpItem->isBroken())
			g_sprInterface.Put(_iX,_iY,eIWI_BROKEN_ITEM);

		BOOL	isBroken	=	_lpItem->m_isBroken;

		if	(_lpItem->isExceptionItem() ==	FALSE)
			_lpItem->m_isBroken	=	FALSE;
		
		if	(!g_hero.isUseAbleItem(_lpItem) || _bIsRed)
			cDRAW::FillAB(_LTRED,_iX,_iY,_iX+dITEM_SHAPE_WIDTH,_iY+dITEM_SHAPE_HEIGHT,12);

		if	(_lpItem->isUltimateItem())
		{
			int		iFrame	=	(s_iFrameCounter*g_anmItemType.GetFPS(eITI_ULTIMATE)/dSYNC_FPS)%g_anmItemType.GetFrameCount(eITI_ULTIMATE);
			g_anmItemType.PutReg(_iX+18,_iY+24,eITI_ULTIMATE,0,iFrame);
		}
		else
		if	(_lpItem->isDXItem())
		{
			int		iFrame	=	(s_iFrameCounter*g_anmItemType.GetFPS(eITI_DX)/dSYNC_FPS)%g_anmItemType.GetFrameCount(eITI_DX);
			g_anmItemType.PutReg(_iX+18,_iY+24,eITI_DX,0,iFrame);
		}

		if	(_lpItem->isExceptionItem() ==	FALSE)
			_lpItem->m_isBroken	=	isBroken;

		if	(_iTwinkleFrame	!=	-1)
		{
			_iTwinkleFrame	=	_iTwinkleFrame%64;

			if	(_iTwinkleFrame	>=	32)
				_iTwinkleFrame	-=	(_iTwinkleFrame%32)*2;

			cDRAW::FillAB(_WHITE,_iX,_iY,_iX+dITEM_SHAPE_WIDTH,_iY+dITEM_SHAPE_HEIGHT,_iTwinkleFrame);
		}
	}

	if	(!_lpItem->isExceptionItem())
	{
		if	(lpBasicItem->m_wKind	!=	eIK_PREMIUM_ITEM)
		{
			int		iAnm	=	-1;

			if	(_lpItem->isQuestItem()	)
				iAnm	=	eITI_QUEST;
			else
			if	(_lpItem->isSuperUniqueItem()	)
				iAnm	=	eITI_SUPER;
			else
			if	(_lpItem->isUniqueItem()	)
				iAnm	=	eITI_UNIQUE;
			else
			if	(_lpItem->isRareItem() || _lpItem->isLittleGoodItem())
				iAnm	=	eITI_RARE;
			else
			if	(_lpItem->isSetItem())
				iAnm	=	eITI_SET;
			else
			if	(_lpItem->isExtraItem())
				iAnm	=	eITI_EXTRA_10+lpBasicItem->m_wExtraGrade;

			if	(iAnm	!=	-1)
			{
				int		iFrame	=	(s_iFrameCounter*g_anmItemType.GetFPS(iAnm)/dSYNC_FPS)%g_anmItemType.GetFrameCount(iAnm);

				g_anmItemType.PutReg(_iX,_iY,iAnm,0,iFrame);
			}
		}

		if (!_lpItem->isMagicCarpetEmblem() && !_lpItem->isExtraItem() && !_lpItem->isExceptionItem() && _bIsPutCount)	//	갯수
		{
			int	iCount	=	_lpItem->m_bCount;

			if	(lpBasicItem->m_attr.isDurabilityToUseCount)
				iCount	=	_lpItem->m_bDurability;
				
			if	(_bIsPutCount	==	1	&&	iCount <=	1)
				return;

			int	iWidth	=	s_sText.getPixelSize(_ms("%d",iCount));
			cDRAW::Fill(0,	_iX+dITEM_SHAPE_WIDTH-4-iWidth ,_iY+22,_iX + dITEM_SHAPE_WIDTH-3+1,_iY+dITEM_SHAPE_HEIGHT-3+1);
			s_sText.putRF(	_iX+dITEM_SHAPE_WIDTH-3        ,_iY+22,WHITE,"%d",iCount);
		}
	}
}

void
CGamePlay::DrawExtraInventoryWindow()
{
	int	iExtranInventorySize	=	g_hero.m_bf6ExtraInventorySize;

	if (iExtranInventorySize	==	0	||	s_iPopupInterface	!=	ePIW_INVENTORY)
		return;

	int		iX,iY,iCount=0,i,j;
	CPos	pos;

	iX		=	s_posInventoryWindow.x;
	iY		=	s_posInventoryWindow.y+dEXTRA_INVENTORY_VERTICAL_POS-2;

	if	(g_config.m_bIsInfoWindowHalfblending)
		g_sprInterface2.Put(iX,dEXTRA_INVENTORY_VERTICAL_POS+g_iCorrectBI_Y/2		,eEBS_BAG_INNER_FRAME,100,100,dPUT_HALF_BLENDING);
	else
		g_sprInterface2.Put(iX,dEXTRA_INVENTORY_VERTICAL_POS+g_iCorrectBI_Y/2		,eEBS_BAG_INNER_FRAME);

	g_sprInterface2.Put(iX,dEXTRA_INVENTORY_VERTICAL_POS+g_iCorrectBI_Y/2		,eEBS_BAG_FRAME);

	for (i=0;i<dITEM_VERTICAL_COUNT_AT_EXTRA_INVENTORY_WINDOW;i++)
	{
		for (j=0;j<dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW;j++)
		{
			pos.x	=	iX+19 +dEXTRA_INVENTORY_ITEM_SLOT_WIDTH*j-dEXTRA_INVENTORY_TITLE_WIDTH;
			pos.y	=	iY+21+dEXTRA_INVENTORY_ITEM_SLOT_HEIGHT*i;

			int			iItemSlot	=	i*dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW+j;
			cItem		*lpItem		=	NULL;
			cItem		tradeItem,putItem;
			cBasicItem	*lpBaseItem	=	NULL;

			if (iItemSlot	>=	iExtranInventorySize)
			{
				g_sprInterface.Put(pos.x,pos.y,eIWI_BLOCKED_ITEM_SLOT);
				continue;
			}

			lpItem	=	&g_hero.m_aExtraInventory[iItemSlot];

			if (lpItem && lpItem->m_wBaseItem != 0xffff)
			{
				putItem.copy(lpItem);

				if	(putItem.m_bCount > 0 || putItem.isExtraItem() || putItem.isMagicCarpetEmblem()|| putItem.isExceptionItem())
				{
					PutItem(&putItem,pos.x,pos.y,TRUE,FALSE,g_hero.getTwinkleFrame(iItemSlot+dOWN_ITEM_COUNT));

					continue;
				}
			}

			if (g_config.m_bIsInfoWindowHalfblending)
				g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_VALID_SLOT,100,100,dPUT_HALF_BLENDING);
			else
				g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_VALID_SLOT);
		}
	}

	if (s_iDragItem	!=	0xffff || g_gwBank.m_iDragBankItem	!=	0xffff || s_propose.m_work.isReadyUseItemToItem)
	{
		int	iFocusItemSlot	=	GetFocusItemSlot();

		if	(s_propose.m_work.isReadyUseItemToItem)
			iFocusItemSlot	=	GetFocusItemInInventoryWindow();

		if	(iFocusItemSlot	!=	0xffff && s_iDragItem != dITEM_DRAG_GOLD && g_gwBank.m_iDragBankItem != dITEM_DRAG_GOLD)
		{
			if	(iFocusItemSlot	>=	dOWN_ITEM_COUNT)
			{
				iFocusItemSlot	-=	dOWN_ITEM_COUNT;

				pos.x	=	iX+19+dEXTRA_INVENTORY_ITEM_SLOT_WIDTH*(iFocusItemSlot%dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW)-dEXTRA_INVENTORY_TITLE_WIDTH;
				pos.y	=	iY+21+dEXTRA_INVENTORY_ITEM_SLOT_HEIGHT*(iFocusItemSlot/dITEM_HORIZON_COUNT_AT_EXTRA_INVENTORY_WINDOW);

				g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5));
			}
		}
	}
}

//
//	그려!!
void
CGamePlay::DrawInventoryWindow()
{
	int		iX,iY,iCount=0,i,j;
	CPos	pos;

	iX		=	s_posInventoryWindow.x;
	iY		=	s_posInventoryWindow.y;
	
	if (g_config.m_bIsInfoWindowHalfblending)
		g_sprInterface.Put(iX,iY,eWINDOW_INVENTORY_PANNEL,100,100,dPUT_HALF_BLENDING);
	else
		g_sprInterface.Put(iX,iY,eWINDOW_INVENTORY_PANNEL);
		
	for (i=0;i<dITEM_VERTICAL_COUNT_AT_INVENTORY_WINDOW;i++)
	{
		for (j=0;j<dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW;j++)
		{
			pos.x	=	iX+19 +dITEM_SLOT_WIDTH*j;
			pos.y	=	iY+214+dITEM_SLOT_HEIGHT*i;

			int			iItemSlot	=	i*dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW+j;
			cItem		*lpItem		=	NULL;
			cItem		tradeItem,putItem;
			cBasicItem	*lpBaseItem	=	NULL;

			if (iItemSlot	>=	g_hero.getItemSlotCount())
				continue;
			
			if	(s_iPopupInterface	==	ePIW_BANK)
				lpItem	=	&s_lpInventoryWindowItemList[iItemSlot];
			else
				lpItem	=	g_hero.getInventoryItem(iItemSlot,TRUE);

			if (lpItem && lpItem->m_wBaseItem != 0xffff)
			{
				putItem.copy(lpItem);

				if	(GetTradeItem(iItemSlot,&tradeItem))
					putItem.m_bCount	-=	tradeItem.m_bCount;

				if (IsInvisibleItem(iItemSlot) && !IsOpenTradeBox())
					putItem.m_bCount	=	0;
				BOOL bUpgradeDragItem  = FALSE;
				if(s_iPopupInterface == ePIW_UPGRADE_SHOP)	//  09.09.01
				{
					if(g_gwUpgradeShop.getUpgradeWeaponItemSlot() == iItemSlot)
						bUpgradeDragItem = TRUE;
					if(g_gwUpgradeShop.getUpgradeItemSlot() == iItemSlot)
						bUpgradeDragItem = TRUE;
					if(g_gwUpgradeShop.getUpgradeNormalItemSlot() == iItemSlot)
						bUpgradeDragItem = TRUE;
					
					
				}
				if ( ( putItem.m_bCount > 0 || putItem.isExtraItem() || putItem.isMagicCarpetEmblem() || putItem.isExceptionItem() ) && bUpgradeDragItem == FALSE)
				{
					PutItem(&putItem,pos.x,pos.y,TRUE,FALSE,g_hero.getTwinkleFrame(iItemSlot));
					continue;
				}
			}

			if (g_config.m_bIsInfoWindowHalfblending)
				g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_VALID_SLOT,100,100,dPUT_HALF_BLENDING);
			else
				g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_VALID_SLOT);
		}
	}


	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface.Put(iX,g_iCorrectBI_Y/2,eWINDOW_TITLE_INVENTORY);
	g_sprInterface.Put(iX,iY,eWINDOW_INVENTORY_FRAME);

	if (s_iPopupInterface	!=	ePIW_INVENTORY)
	{
		g_sprInterface2.Put(iX+22,172+g_iCorrectBI_Y/2,eIWI_TRADE_DISABLE);
		g_sprInterface2.Put(iX+59,172+g_iCorrectBI_Y/2,eIWI_CART_DISABLE);
		g_sprInterface2.Put(iX+96,172+g_iCorrectBI_Y/2,eIWI_PITCHMAN_SHOP_DISABLE);
		g_sprInterface2.Put(iX+133,172+g_iCorrectBI_Y/2,eIWI_CALL_MAGIC_CARPET_DISABLE);

		if	(l_bIsShowCarrotButton)
			g_sprRegionInterface.Put(iX+170,172+g_iCorrectBI_Y/2,eIWI_CARROT_SHOP_DISABLE);
	}

	for (i=0;i<dITEM_VERTICAL_COUNT_AT_INVENTORY_WINDOW;i++)
	{
		for (j=0;j<dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW;j++)
		{
			pos.x	=	iX+19 +dITEM_SLOT_WIDTH*j;
			pos.y	=	iY+214+dITEM_SLOT_HEIGHT*i;

			int	iItemSlot	=	i*dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW+j;

			if (iItemSlot	<	g_hero.getItemSlotCount())
				continue;

			g_sprInterface.Put(pos.x,pos.y,eIWI_BLOCKED_ITEM_SLOT);
		}
	}

	if	(s_iDragItem	!=	0xffff || g_gwBank.m_iDragBankItem	!=	0xffff || s_propose.m_work.isReadyUseItemToItem)
	{
		int	iFocusItemSlot	=	GetFocusItemSlot();

		if	(s_propose.m_work.isReadyUseItemToItem)
			iFocusItemSlot	=	GetFocusItemInInventoryWindow();

		if	(iFocusItemSlot	!=	0xffff && s_iDragItem != dITEM_DRAG_GOLD && g_gwBank.m_iDragBankItem != dITEM_DRAG_GOLD)
		{
			if	(iFocusItemSlot	<	dOWN_ITEM_COUNT)
			{
				pos.x	=	iX+19 +dITEM_SLOT_WIDTH*(iFocusItemSlot%dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW);
				pos.y	=	iY+214+dITEM_SLOT_HEIGHT*(iFocusItemSlot/dITEM_HORIZON_COUNT_AT_INVENTORY_WINDOW);

				g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5));
			}
		}
	}

	int	iPart	=	0;

	for (iPart=0;;iPart++)
	{
		if	(l_aEquipmentPos[iPart].x	==	0xffff)
			break;

		pos.x	=	iX+l_aEquipmentPos[iPart].x;
		pos.y	=	iY+l_aEquipmentPos[iPart].y;

		cItem		*lpItem;
		int			iValidSlot		=	0xffff;
		int			iJob			=	g_hero.m_wJob;
		int			iCharacter		=	iJob/2;
		int			iInterfaceFileIndex = 1;		
		if	(s_iPopupInterface	==	ePIW_BANK)
			lpItem	=	&s_lpInventoryWindowEquipmentList[iPart];
		else
			lpItem	=	g_hero.getInventoryItem(iPart+dBORDER_OF_ITEM_AND_EQUIPMENT,TRUE);

		if	(iPart == dEQUIP_WEAPON		&& g_hero.m_bf1IsHideWeapon1)
			lpItem	=	NULL;
		if	(iPart == dEQUIP_WEAPON_2	&& g_hero.m_bf1IsHideWeapon2)
			lpItem	=	NULL;

		if	(lpItem && lpItem->m_wBaseItem != 0xffff)
		{
			cItem	putItem,tradeItem;

			putItem.copy(lpItem);

			if	(GetTradeItem(iPart+dBORDER_OF_ITEM_AND_EQUIPMENT,&tradeItem))	
				putItem.m_bCount	-=	tradeItem.m_bCount;

			if	(putItem.m_bCount	>	0	||	putItem.isExtraItem()	|| putItem.isMagicCarpetEmblem() || putItem.isExceptionItem())
			{
				BOOL	bIsRed	=	FALSE;

				if	(g_hero.m_wJob/2	!=	dJOB_BEAST_TAMER/2)
				{
					if	(iPart	==	dEQUIP_WEAPON	&&	(g_hero.m_wJob	%2))
						bIsRed	=	TRUE;
					if	(iPart	==	dEQUIP_WEAPON_2	&&	(g_hero.m_wJob	%2)==0)
						bIsRed	=	TRUE;
				}

				if	(iPart>=dEQUIP_RING1	&& iPart<=dEQUIP_RING8)
					PutItem(&putItem,pos.x,pos.y,TRUE,TRUE,0xffff,bIsRed);
				else
					PutItem(&putItem,pos.x,pos.y,TRUE,FALSE,0xffff,bIsRed);

				continue;
			}
		}

		if	(iPart	==	dEQUIP_WEAPON)
		{
			iInterfaceFileIndex	=	l_aVaildWeapon[iCharacter*2][0];
			iValidSlot			=	l_aVaildWeapon[iCharacter*2][1];	

		}
		else
		if	(iPart	==	dEQUIP_SHIELD_BULLET)
		{
			iValidSlot	=	eIWI_SHIELD;

			if (iCharacter	==	dJOB_MAGIC_LANCER/2)
				iValidSlot	=	eIWI_BULLET;
			else
			if (iCharacter	==	dJOB_WIZARD/2 || iCharacter	==	dJOB_PRINCESS/2 || iCharacter == dJOB_OPTICALIST/2	)
				iValidSlot	=	eIWI_BROOCH;
			else
			if (iJob		==	dJOB_FALLEN_ANGEL || iJob	==	dJOB_DEVIL/2 )
				iValidSlot	=	eIWI_CROSS;
			else
			if (iCharacter	==	dJOB_SUMMONER/2 || iCharacter	==	dJOB_SOUL_BRINGER/2)		// 영술사 추가..
				iValidSlot	=	eIWI_SHOULDER_TATOO;
		}
		else
		if	(iPart	==	dEQUIP_WEAPON_2)
		{
			iInterfaceFileIndex	=	l_aVaildWeapon[iCharacter*2 + 1][0];
			iValidSlot			=	l_aVaildWeapon[iCharacter*2 + 1][1];
		}
		else
		if	(iPart	==	dEQUIP_EARING)
		{
			if	(c_aIsMale[iJob])
				iValidSlot	=	eIWI_CAPE;
			else
				iValidSlot	=	eIWI_EAR;
		}
		
		if	(iValidSlot	!=	0xffff)
		{
			if	(g_config.m_bIsInfoWindowHalfblending)
			{

				if(iInterfaceFileIndex == 1)
				{
					g_sprInterface.Put(pos.x ,pos.y,iValidSlot,100,100,dPUT_HALF_BLENDING);
				}
				else if(iInterfaceFileIndex == 2)
				{
					g_sprInterface2.Put(pos.x ,pos.y,iValidSlot,100,100,dPUT_HALF_BLENDING);
				}
				
			}
			else
			{
				if(iInterfaceFileIndex == 1)
				{
						g_sprInterface.Put(pos.x ,pos.y,iValidSlot);
				}
				else if(iInterfaceFileIndex == 2)
				{
						g_sprInterface2.Put(pos.x ,pos.y,iValidSlot);
				}				
			}
		}
	}

	if	(s_iPopupInterface	==	ePIW_INVENTORY)
	{
		//s_bmButton.draw();
		s_bmButtonI2.draw();

		if	(l_bIsShowCarrotButton)
			s_bmCarrotShop.draw();
	}

//	HP
	{
		int	iHP	=	g_hero.m_iHP,iMaxHP	=	g_hero.getMaxHP()/100;

		if	(iHP	!=	0)	iHP	=	max(iHP/100,1);

		s_text.put(iX+20,iY+120,RGB(255,80,92),"%s <c:254,160,167>%d/%d<n>",dMSG_HP,iHP,iMaxHP);
	}

//	CP
	s_text.put(iX+ 20,iY+134,RGB(86,154,255),"%s <c:158,197,255>%d/%d<n>",dMSG_CP,g_hero.m_iCP/100,g_hero.getMaxCP()/100);

//	방어력
	{
		int	iDefenseValue	=	g_hero.getDefensivePower();
		int	iBaseValue		=	g_hero.getDefensivePower(TRUE);

		if (g_hero.isPlusDefense())	
			s_text.put(iX+140,iY+134,RGB(118,206,163),"%s <c:191,227,199>%d[<c:LTGREEN>%d<n>]",dMSG_DEFENSE_POWER,iBaseValue/100,iDefenseValue/100);
		else
		if (g_hero.isMinusDefense())
			s_text.put(iX+140,iY+134,RGB(118,206,163),"%s <c:191,227,199>%d[<c:LTRED>%d<n>]",dMSG_DEFENSE_POWER,iBaseValue/100,iDefenseValue/100);
		else
			s_text.put(iX+140,iY+134,RGB(118,206,163),"%s <c:191,227,199>%d",dMSG_DEFENSE_POWER,iDefenseValue/100);
	}

//	공격력
	s_text.put(iX+140,iY+120,RGB(144,211,132),"%s <c:194,220,160>%d~%d<n>",dMSG_ATTACK_POWER,g_hero.getAttackPower(NULL,TRUE)/100,g_hero.getAttackPower(NULL,FALSE)/100);

	s_text.put(iX+47,iY+193,WHITE,"<c:LTYELLOW>%s<n> Gold",GetMoneyString((DWORD)*s_lpGold));

	DrawExtraInventoryWindow();

	if	(s_iPopupInterface	==	ePIW_INVENTORY)	
		g_gwMiniPetStatus.draw();

	if	(g_hero.m_bf1IsReceiveOtherPlayerItemData)
	{
		iX		=	s_posInventoryWindow.x+10;
		iY		=	4;

		cRECT	rect;

		rect.set(iX,iY,g_iScreenWidth-24,iY+10);

		char	strToolTip[256];

		sprintf(strToolTip,"[%s/%s]",g_hero.m_strOtherPlayerId,g_hero.m_strOtherPlayerName);

		s_ttCommon.drawBoxAddBorderSize(&rect,0,32);
		s_text.cPut(rect.x1+rect.getWidth()/2,iY,WHITE,strToolTip);
	}
}


//
//	거래중인 아이템이다.
BOOL
CGamePlay::IsInvisibleItem(int _iSlot)
{
	if (IsOpenTradeBox() && s_tradeManager.isTradeItem(_iSlot))
		return	TRUE;

	if (g_gwPitchmanShop.isOpened() && g_gwPitchmanShop.isSellingInventorySlot(_iSlot))
		return	TRUE;

	return	FALSE;
}
