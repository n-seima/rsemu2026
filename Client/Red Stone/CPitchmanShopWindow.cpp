#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "CPitchmanShop.H"
#include "cLoadTextCode.H"
#include "cLog.H"

#define	dSHOP_ITEM_HEIGHT				40
#define	dSHOP_INTERFACE_TITLE_HEIGHT	6
#define	dSHOP_INTERFACE_BOTTOM			26
#define	dSHOP_INTERFACE_HEIGHT_GAP		(dSHOP_INTERFACE_TITLE_HEIGHT+dSHOP_INTERFACE_BOTTOM+6)

#define	dMAX_ITEM_COUNT_IN_PITCHMAN_SHOP_WINDOW	9

CPitchmanShopWindow	g_gwPitchmanShop;

enum
{
	ePSM_BEGIN_SHOP,
	ePSM_STOP_SHOP,
	ePSM_CLOSE_SHOP,

	ePSM_BUY_ITEM,

	ePSM_SELECT_SIGNBOARD_SHAPE,
	ePSM_SELECT_TEXT_COLOR,
	ePSM_SELECT_BOLD,
	ePSM_SELECT_EFFECT,
	ePSM_TEXT_SHADOW,

	ePSM_NORMAL_TEXT,
	ePSM_BOLD_TEXT,
};

enum
{
	ePSDM_NONE,
	ePSDM_SELECT_SIGNBOARD,
	ePSDM_SELECT_TEXT_COLOR,
	ePSDM_SELECT_BOLD,
	ePSDM_SELECT_EFFECT,
};


void
CPitchmanShopForClient::reset()
{
	m_wSerial			=	0xffff;
	m_bf4SignboardShape	=	0;
	m_wTextColor		=	0;
	m_bf1IsBold			=	FALSE;
	m_bf1IsShadowText	=	FALSE;

	memset(m_aItem,0xff,sizeof(m_aItem));
}
	
void
CPitchmanShopWindow::openForClient(SG_PITCHMAN_SHOP_INFO *_lpPacket)
{
	m_wIsWaitPichmanShopWorkResult	=	FALSE;
	m_iDragItem						=	0xffff;

	m_shop.reset();

	memcpy(&m_shop,&_lpPacket->shopInfo,sizeof(SG_PITCHMAN_SHOP_INFO));
	memset(&m_shop.m_aItem,0xff,sizeof(m_shop.m_aItem));

	for (int i=0;i<_lpPacket->shopInfo.m_bf4ItemCount;i++)
	{
		int	iSlot	=	_lpPacket->shopInfo.m_aItem[i].m_wSlot;

		memcpy(&m_shop.m_aItem[iSlot],&_lpPacket->shopInfo.m_aItem[i],sizeof(CPitchmanShopItemInfoForClient));
	}

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	m_iOwner						=	ePS_CLIENT;
	m_iSelectShopItem				=	-1;

	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);

	s_iPopupInterface		=	ePIW_PITCHMAN_SHOP;
	m_rect.x1				=	g_iScreenWidth-iWindowWidth*2;
	m_rect.y1				=	iTitleHeight+g_iCorrectBI_Y/2;
	m_rect.x2				=	g_iScreenWidth-iWindowWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;

	SetInventoryWindowPos();
	InitPopupWindowCloseButton();

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(dMSG_BUY		,ePSM_BUY_ITEM		,m_rect.x1+130,468+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.addImageBar(dMSG_CLOSE	,ePSM_CLOSE_SHOP	,m_rect.x1+200,468+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.setClickMargin(ePSM_BUY_ITEM,1,1);
	s_bmMiddleWindowButton.setClickMargin(ePSM_CLOSE_SHOP,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);
}

//
//	아이템 추가
void
CPitchmanShopWindow::addItem(SG_ADD_PITCHMAN_SHOP_ITEM *_lpPacket)
{
	m_shop.m_aItem[_lpPacket->wShopSlot].m_dwPrice			=	_lpPacket->dwPrice;
	m_shop.m_aItem[_lpPacket->wShopSlot].m_bDenomination	=	_lpPacket->bDenomination;	// 09.08.21
	m_shop.m_aItem[_lpPacket->wShopSlot].m_item.m_wBaseItem	=	_lpPacket->wInventorySlot;
}

void
CPitchmanShopWindow::removeItem(SG_REMOVE_PITCHMAN_SHOP_ITEM *_lpPacket)
{
	m_shop.m_aItem[_lpPacket->wSlot].m_item.m_wBaseItem	=	0xffff;
}

void
CPitchmanShopWindow::changeStatus(SG_CHANGE_PITCHMAN_SHOP_INFO	*_lpPacket)
{
  	for (int i=0;i<dPITCHMAN_SHOP_NAME_LENGTH;i++)
  	{
  		if	(_lpPacket->strName[i]	==	'<')
  			_lpPacket->strName[i]	=	'[';
  	}

	m_iShopStatus					=	_lpPacket->info.m_bf2Status;
	m_shop.m_wTextColor				=	_lpPacket->info.m_wTextColor;
	m_shop.m_bf1IsBold				=	_lpPacket->info.m_bf1IsBold;
	m_shop.m_bf1IsShadowText		=	_lpPacket->info.m_bf1IsShadowText;
	m_shop.m_bf4SignboardShape		=	_lpPacket->info.m_bf6SignboardShape;

	if (m_iOwner	==	ePS_SHOP_KEEPER)
	{
		m_wIsWaitPichmanShopWorkResult	=	FALSE;

		if (m_iShopStatus	==	ePSS_READY)
		{
			s_bmMiddleWindowButton.show(ePSM_BEGIN_SHOP);
			s_bmMiddleWindowButton.hide(ePSM_STOP_SHOP);
			s_bmMiddleWindowButton.enable(ePSM_SELECT_SIGNBOARD_SHAPE	);
			s_bmMiddleWindowButton.enable(ePSM_SELECT_TEXT_COLOR		);
			s_bmMiddleWindowButton.enable(ePSM_SELECT_BOLD				);
			s_bmMiddleWindowButton.enable(ePSM_SELECT_EFFECT			);

			if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard1	==	FALSE &&	g_hero.m_bf1IsOwnedPitchmanShopSignBoard2	==	FALSE &&
				g_hero.m_bf1IsOwnedPitchmanShopSignBoard3	==	FALSE &&	g_hero.m_bf1IsOwnedPitchmanShopSignBoard4	==	FALSE &&
				g_hero.m_bf1IsOwnedPitchmanShopSignBoard5	==	FALSE)
				s_bmMiddleWindowButton.disable(ePSM_SELECT_SIGNBOARD_SHAPE	);

			if (g_hero.m_bf1IsOwnedPitchmanColorPen			==	FALSE)
				s_bmMiddleWindowButton.disable(ePSM_SELECT_TEXT_COLOR	);
			if (g_hero.m_bf1IsOwnedPitchmanShopBoldPen		==	FALSE)
				s_bmMiddleWindowButton.disable(ePSM_SELECT_BOLD			);
			if (g_hero.m_bf1IsOwnedPitchmanShopTwinkle		==	FALSE)
				s_bmMiddleWindowButton.disable(ePSM_SELECT_EFFECT		);

			if (g_hero.m_bf1IsOwnedPitchmanShopLongPen	==	TRUE)
				EnableChattingBar(TRUE,dPITCHMAN_SHOP_NAME_LENGTH-2);
			else
				EnableChattingBar(TRUE,(dPITCHMAN_SHOP_NAME_LENGTH-2)/2);
			
			s_inputChatting.setStr(0,m_shop.m_strName);
		}
		else
		{
			s_bmMiddleWindowButton.show(ePSM_STOP_SHOP);
			s_bmMiddleWindowButton.hide(ePSM_BEGIN_SHOP);
			s_bIsBookedDisableChattingBox	=	TRUE;
			s_bmMiddleWindowButton.disable(ePSM_SELECT_SIGNBOARD_SHAPE	);
			s_bmMiddleWindowButton.disable(ePSM_SELECT_TEXT_COLOR		);
			s_bmMiddleWindowButton.disable(ePSM_SELECT_BOLD				);
			s_bmMiddleWindowButton.disable(ePSM_SELECT_EFFECT			);

			strcpy(m_shop.m_strName,_lpPacket->strName);
		}
	}
	else
	{
		strcpy(m_shop.m_strName,_lpPacket->strName);

		close();
	}
}

void
CPitchmanShopWindow::changeItemPlace(SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE *_lpPacket)
{
	CPitchmanShopItemInfoForClient	item;

	memcpy(&item								,&m_shop.m_aItem[_lpPacket->wSlot1]	,sizeof(CPitchmanShopItemInfoForClient));
	memcpy(&m_shop.m_aItem[_lpPacket->wSlot1]	,&m_shop.m_aItem[_lpPacket->wSlot2]	,sizeof(CPitchmanShopItemInfoForClient));
	memcpy(&m_shop.m_aItem[_lpPacket->wSlot2]	,&item								,sizeof(CPitchmanShopItemInfoForClient));
}

void
CPitchmanShopWindow::soldItem(SG_SOLD_PITCHMAN_SHOP_ITEM *_lpPacket)	
{	
	if (s_iPopupInterface	!=	ePIW_PITCHMAN_SHOP)
		return;

	if (m_iOwner			!=	ePS_SHOP_KEEPER)
		return;

	g_logChat.accuseTradeLog(g_hero.m_strName,_lpPacket->strName,TRUE);

	cItem	*lpItem		=	g_hero.getInventoryItem(m_shop.m_aItem[_lpPacket->wSlot].m_item.m_wBaseItem,TRUE);

	char	strMessage[512];
	if(_lpPacket->bDenomination)
	{
		sprintf(strMessage,dMSG_PITCHMAN_SHOP_ITEM_SOLD_FORM_GOLD_BAR,_lpPacket->strName,_lpPacket->dwPrice,lpItem->getName());
	}
	else
	{
		sprintf(strMessage,dMSG_PITCHMAN_SHOP_ITEM_SOLD_FORM,_lpPacket->strName,GetMoneyString(_lpPacket->dwPrice),lpItem->getName());
	}

	g_msgBox.cPopup("",strMessage,dMSG_OK);

	m_shop.m_aItem[_lpPacket->wSlot].m_item.m_wBaseItem	=	0xffff;
	lpItem->reset();	
	if(_lpPacket->bDenomination)
	{		
		int iStackLimit = g_aBasicItem[dITEM_INDEX_GOLD_BAR].m_wStackLimit;
		int iAddItemSlot = _lpPacket->dwPrice/iStackLimit;
		int iRemainder = 0;
		
		if(_lpPacket->dwPrice%iStackLimit)
			iRemainder = 1;


		cItem*	addItem = new cItem[iAddItemSlot + iRemainder];

		for(int i=0;i<iAddItemSlot; ++i)
		{
			cItem::GenerateItemByBaseItem(&addItem[i],dITEM_INDEX_GOLD_BAR,iStackLimit);
			cItem* lpEmptySlot	 = g_hero.getEmptyItemSlot();
			if(!lpEmptySlot)
				return ;
			lpEmptySlot->copy(&addItem[i]);
		}
		
		if(iRemainder)
		{
			cItem::GenerateItemByBaseItem(&addItem[iAddItemSlot],dITEM_INDEX_GOLD_BAR,_lpPacket->dwPrice%iStackLimit);
			cItem* lpEmptySlot	 = g_hero.getEmptyItemSlot();
			if(!lpEmptySlot)
				return ;
			lpEmptySlot->copy(&addItem[iAddItemSlot]);
		}

		pKILL(addItem);

	}
	else
	{
		g_hero.m_iGold	+=	_lpPacket->dwPrice;
	}
	
	for (int i=0;i<dPITCHMAN_SHOP_INVENTORY_SIZE;i++)
	{
		if (m_shop.m_aItem[i].m_item.m_wBaseItem	!=	0xffff)
			return;
	}

	close();

	g_msgBox.cPopup("",dMSG_CLOSE_PITCHMAN_SHOP_BY_SOLD_ALL_ITEM,dMSG_OK);
}



void
CPitchmanShopWindow::buyItem(SG_BUY_PITCHMAN_SHOP_ITEM *_lpPacket)		
{// 마지막 수정일 : 09.10.08
	if (s_iPopupInterface	!=	ePIW_PITCHMAN_SHOP)
		return;

	if (m_iOwner			!=	ePS_CLIENT)
		return;

	g_logChat.accuseTradeLog(g_hero.m_strName,_lpPacket->strOwnerName,TRUE);

	cItem	*lpItem		=	(cItem	*)&_lpPacket->item;

	char	strMessage[512];
	
	if(_lpPacket->bDenomination)
	{
		sprintf(strMessage,dMSG_PITCHMAN_SHOP_ITEM_BUY_FORM_GOLD_BAR,m_shop.m_strOwnerName,_lpPacket->dwPrice,lpItem->getName());
	}else{
		sprintf(strMessage,dMSG_PITCHMAN_SHOP_ITEM_BUY_FORM,m_shop.m_strOwnerName,GetMoneyString(_lpPacket->dwPrice),lpItem->getName());
	}
	
	g_msgBox.cPopup("",strMessage,dMSG_OK);

	m_shop.m_aItem[_lpPacket->wSlot].m_item.m_wBaseItem	=	0xffff;

	cItem	*lpValidSlot	=	g_hero.getValidInventorySlot();

	if	(lpValidSlot)
		lpValidSlot->copy(lpItem);
	if(!_lpPacket->bDenomination)
		g_hero.m_iGold	-=	_lpPacket->dwPrice;

	
}

//	상점 열기
void
CPitchmanShopWindow::openForShopkeeper(SG_OPEN_PITCHMAN_SHOP *_lpPacket)
{
	m_wIsWaitPichmanShopWorkResult	=	FALSE;
	m_iMenuMode						=	ePSDM_NONE;
	m_wTextColor					=	0;
	m_iSignboardShape				=	0;
	m_bIsBold						=	0;
	m_bIsShadowText					=	FALSE;
	m_iDragItem						=	0xffff;

	{
		m_shop.reset();
		m_shop.m_wSerial		=	_lpPacket->wSerial;

		CSimplePitchmanShopInfo	info;

		info.m_bf10Serial			=	_lpPacket->wSerial;
		info.m_bf6SignboardShape	=	0;
		info.m_wTextColor			=	0;
		info.m_bf1IsBold			=	FALSE;
		info.m_bf1IsShadowText		=	FALSE;
		info.m_bf2Status			=	ePSS_READY;

		info.m_wX					=	_lpPacket->wX;
		info.m_wY					=	_lpPacket->wY;

		g_pitchmanShop.setShopInfo(&info," ");
	}

	{
		g_hero.lockOff();
		g_hero.m_wPitchmanShopSerial	=	_lpPacket->wSerial;
		g_lpHero->action(g_lpHero->m_pos.x,g_lpHero->m_pos.y,dACT_SITDOWN);
		g_lpHero->setDirect(dDIRECT_S);
	}

	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	m_iOwner						=	ePS_SHOP_KEEPER;
	m_iShopStatus					=	ePSS_READY;

	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpGold						=	&g_hero.m_iGold;

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);

	s_iPopupInterface		=	ePIW_PITCHMAN_SHOP;
	m_rect.x1				=	g_iScreenWidth-iWindowWidth*2;
	m_rect.y1				=	iTitleHeight+g_iCorrectBI_Y/2;
	m_rect.x2				=	g_iScreenWidth-iWindowWidth;
	m_rect.y2				=	iTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;

	int	iX					=	m_rect.x1;
	int	iY					=	m_rect.y1+8;

	if (s_inputChatting.isEnable() == FALSE && m_iShopStatus == ePSS_READY)
	{
		if (g_hero.m_bf1IsOwnedPitchmanShopLongPen	==	TRUE)
			EnableChattingBar(TRUE,dPITCHMAN_SHOP_NAME_LENGTH-2);
		else
			EnableChattingBar(TRUE,(dPITCHMAN_SHOP_NAME_LENGTH-2)/2);
	}


	s_inputChatting.setStr(0,_ms(dMSG_PITCHMAN_SHOP_NAME_FORM,g_hero.m_strName));
	s_inputChatting.active(0,TRUE);

	SetInventoryWindowPos();

	s_bmMiddleWindowButton.reset();

	s_bmMiddleWindowButton.addImageBar(dMSG_BEGIN_PITCHMAN_SHOP			,ePSM_BEGIN_SHOP,m_rect.x1+130,472+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.addImageBar(dMSG_STOP_PITCHMAN_SHOP			,ePSM_STOP_SHOP	,m_rect.x1+130,472+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmMiddleWindowButton.addImageBar(dMSG_CLOSE						,ePSM_CLOSE_SHOP,m_rect.x1+200,472+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	
	char	strComment[128];

	strcpy(strComment,dMSG_CHANGE_PITCHMAN_SHOP_SIGNBOARD_SHAPE_COMMENT);
	if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard1	==	FALSE &&	g_hero.m_bf1IsOwnedPitchmanShopSignBoard2	==	FALSE &&
		g_hero.m_bf1IsOwnedPitchmanShopSignBoard3	==	FALSE &&	g_hero.m_bf1IsOwnedPitchmanShopSignBoard4	==	FALSE &&
		g_hero.m_bf1IsOwnedPitchmanShopSignBoard5	==	FALSE)
		sprintf(strComment,"%s\n<c:LTRED>%s<n>",dMSG_CHANGE_PITCHMAN_SHOP_SIGNBOARD_SHAPE_COMMENT,dMSG_REQUIRE_PREMIUM_ITEM_TO_USE_THIS_MENU);
	s_bmMiddleWindowButton.addImageBar("",ePSM_SELECT_SIGNBOARD_SHAPE	,m_rect.x1+14	,59+g_iCorrectBI_Y/2,ePSI_SIGNBOARD_SHAPE_NORMAL,ePSI_SIGNBOARD_SHAPE_ACTIVE,ePSI_SIGNBOARD_SHAPE_PRESSED,0xffff,ePSI_SIGNBOARD_SHAPE_DISABLE,strComment);

	strcpy(strComment,dMSG_CHANGE_PITCHMAN_SHOP_TEXT_COLOR_COMMENT);
	if (g_hero.m_bf1IsOwnedPitchmanColorPen	==	FALSE)
		sprintf(strComment,"%s\n<c:LTRED>%s<n>",dMSG_CHANGE_PITCHMAN_SHOP_TEXT_COLOR_COMMENT,dMSG_REQUIRE_PREMIUM_ITEM_TO_USE_THIS_MENU);
	s_bmMiddleWindowButton.addImageBar("",ePSM_SELECT_TEXT_COLOR		,m_rect.x1+49	,59+g_iCorrectBI_Y/2,ePSI_TEXT_COLOR_NORMAL,ePSI_TEXT_COLOR_ACTIVE,ePSI_TEXT_COLOR_PRESSED,0xffff,ePSI_TEXT_COLOR_DISABLE,strComment);

	strcpy(strComment,dMSG_CHANGE_PITCHMAN_SHOP_TEXT_BOLD_COMMENT);
	if (g_hero.m_bf1IsOwnedPitchmanShopBoldPen	==	FALSE)
		sprintf(strComment,"%s\n<c:LTRED>%s<n>",dMSG_CHANGE_PITCHMAN_SHOP_TEXT_BOLD_COMMENT,dMSG_REQUIRE_PREMIUM_ITEM_TO_USE_THIS_MENU);
	s_bmMiddleWindowButton.addImageBar("",ePSM_SELECT_BOLD				,m_rect.x1+85	,59+g_iCorrectBI_Y/2,ePSI_BOLD_NORMAL,ePSI_BOLD_ACTIVE,ePSI_BOLD_PRESSED,0xffff,ePSI_BOLD_DISABLE,strComment);

	strcpy(strComment,dMSG_CHANGE_PITCHMAN_SHOP_EFFECT_COMMENT);
	if (g_hero.m_bf1IsOwnedPitchmanShopTwinkle	==	FALSE)
		sprintf(strComment,"%s\n<c:LTRED>%s<n>",dMSG_CHANGE_PITCHMAN_SHOP_EFFECT_COMMENT,dMSG_REQUIRE_PREMIUM_ITEM_TO_USE_THIS_MENU);
	s_bmMiddleWindowButton.addImageBar("",ePSM_SELECT_EFFECT			,m_rect.x1+120	,59+g_iCorrectBI_Y/2,ePSI_EFFECT_NORMAL,ePSI_EFFECT_ACTIVE,ePSI_EFFECT_PRESSED,0xffff,ePSI_EFFECT_DISABLE,strComment);

	if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard1	==	FALSE &&	g_hero.m_bf1IsOwnedPitchmanShopSignBoard2	==	FALSE &&
		g_hero.m_bf1IsOwnedPitchmanShopSignBoard3	==	FALSE &&	g_hero.m_bf1IsOwnedPitchmanShopSignBoard4	==	FALSE &&
		g_hero.m_bf1IsOwnedPitchmanShopSignBoard5	==	FALSE)
		s_bmMiddleWindowButton.disable(ePSM_SELECT_SIGNBOARD_SHAPE	);
	if (g_hero.m_bf1IsOwnedPitchmanColorPen			==	FALSE)
		s_bmMiddleWindowButton.disable(ePSM_SELECT_TEXT_COLOR	);
	if (g_hero.m_bf1IsOwnedPitchmanShopBoldPen		==	FALSE)
		s_bmMiddleWindowButton.disable(ePSM_SELECT_BOLD			);
	if (g_hero.m_bf1IsOwnedPitchmanShopTwinkle		==	FALSE)
		s_bmMiddleWindowButton.disable(ePSM_SELECT_EFFECT		);

	s_bmMiddleWindowButton.hide(ePSM_STOP_SHOP	);
	s_bmMiddleWindowButton.setClickMargin(ePSM_BEGIN_SHOP,1,1);
	s_bmMiddleWindowButton.setClickMargin(ePSM_STOP_SHOP,1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);

	s_bmRadioBox.reset();
	s_bmRadioBox.addImageBar(dMSG_NORMAL_TEXT	,ePSM_NORMAL_TEXT	,m_rect.x1+19,96+g_iCorrectBI_Y/2,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
	s_bmRadioBox.addImageBar(dMSG_BOLD_TEXT		,ePSM_BOLD_TEXT		,m_rect.x1+99,96+g_iCorrectBI_Y/2,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);

	s_bmRadioBox.check(ePSM_NORMAL_TEXT+m_bIsBold);
	s_bmRadioBox.hide(ePSM_BOLD_TEXT);
	s_bmRadioBox.hide(ePSM_NORMAL_TEXT);

	s_bmCheckBox.reset();
	s_bmCheckBox.addImageBar(dMSG_PITCHMAN_SHOP_TEXT_SHADOW	,ePSM_TEXT_SHADOW	,m_rect.x1+160	,64+g_iCorrectBI_Y/2,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);

	if(m_bIsShadowText)	
		s_bmCheckBox.check(ePSM_TEXT_SHADOW);
}

//
//	상점 열기
void
CPitchmanShopWindow::close()
{
	if (s_iPopupInterface	!=	ePIW_PITCHMAN_SHOP)
	{
		m_shop.reset();

		return;
	}

	if (m_iOwner	==	ePS_SHOP_KEEPER)
		s_agent.sendPitchmanShopClose(m_shop.m_wSerial);

	cINPDEV::ReleaseExclusive();

	CloseInventoryWindow();

	s_iPopupInterface			=	ePIW_NONE;
	g_hero.m_wPitchmanShopSerial	=	0xffff;
	s_bIsBookedDisableChattingBox	=	TRUE;
	m_shop.reset();
}	//	CPitchmanShopWindow::close()

void
CPitchmanShopWindow::closeByServer()
{
	if (s_iPopupInterface	!=	ePIW_PITCHMAN_SHOP)
		return;

	AddSystemMessage(LTYELLOW,dMSG_CLOSE_PITCHMAN_SHOP_BY_SHOP_KEEPER);

	close();
}

		
void
CPitchmanShopWindow::drawForShopkeeper()		// 09.08.28
{
	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iX			=	m_rect.x1+10;
	int		iY			=	m_rect.y1+10,i;
	int		iPutX,iPutY;
	cRECT	rect;

	rect.set(m_rect.x1+9,22+g_iCorrectBI_Y/2,m_rect.x1+260,56+g_iCorrectBI_Y/2);
	s_ttCommon.drawBox(&rect,0,0);

	{
		s_text.scriptPutOff();	//	스크립트를 쌩까고 출력
		s_text.textMixOff();	//	스크립트를 쌩까고 출력
		s_text.cPut(rect.x1+4,rect.y1+2,rect.getWidth()-8,rect.getHeight(),0,WHITE,dFITTEXT_CENTER,m_strShopName);
		s_text.scriptPutOn();	//	스크립트를 쌩까고 출력
		s_text.textMixOn();
	}

	g_sprInterface.Put(m_rect.x1,g_iCorrectBI_Y/2		,eWINDOW_TITLE_PITCHMAN_SHOP);
	s_bmMiddleWindowButton.draw();

	iX	-=	3;

	g_sprInterface.Put(iX,116+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계
	g_sprInterface.Put(iX,466+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계

	iY	=	126;

	for (i=0;i<dMAX_ITEM_COUNT_IN_PITCHMAN_SHOP_WINDOW;i++)
	{
		iPutX	=	iX+6;
		iPutY	=	iY+i*38+g_iCorrectBI_Y/2;

		if (i	>=	g_hero.m_wPitchmanShopSize)
		{
			g_sprInterface.Put(iPutX,iPutY,eIWI_BLOCKED_ITEM_SLOT);
			continue;
		}

		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_FRAME);

		if (m_iFocusSlot == i)
			g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5));

		if (m_shop.m_aItem[i].m_item.m_wBaseItem	==	0xffff)
			continue;

		cItem	*lpItem	=	g_hero.getInventoryItem(m_shop.m_aItem[i].m_item.m_wBaseItem,TRUE);

		if (lpItem	==	NULL)
			continue;

		PutItem(lpItem,iPutX,iPutY,TRUE);

		char	strItemName[256];

		strcpy(strItemName,lpItem->getName());

		int		iLineCount	=	s_text.getLineCount(g_iScreenWidth/3-60,strItemName);
		
		char*	strDenomination = dMSG_ENG_GOLD;
		char	strPrice[64]	= "";
		strcpy(strPrice,dMSG_PRICE);	
		if(m_shop.m_aItem[i].m_bDenomination)
		{
			
			strDenomination = dMSG_COUNT;
			sprintf(strPrice,"%s %s",strPrice, dMSG_GOLD_BAR);
			
		}

		if	(iLineCount	>=	3)
		{
			char	strText[512];

		
			sprintf(strText,"%s\n%s <c:LTYELLOW>%s<n> %s",strItemName,strPrice,GetMoneyString(m_shop.m_aItem[i].m_dwPrice), strDenomination);

			s_text.setLineGap(0);
			s_text.cPut(iPutX+40,iPutY-12,g_iScreenWidth/3-60,60,0,WHITE,dFITTEXT_HEIGHT_CENTER,strText);
			s_text.setLineGap(2);
		}
		else
		if	(iLineCount	>=	2)
		{
			char	strText[512];

			sprintf(strText,"%s\n%s <c:LTYELLOW>%s<n> %s",strItemName,strPrice,GetMoneyString(m_shop.m_aItem[i].m_dwPrice), strDenomination);

			s_text.setLineGap(0);
			s_text.cPut(iPutX+40,iPutY-2,g_iScreenWidth/3-60,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,strText);
			s_text.setLineGap(2);
		}
		else
		{
			char	strText[512];
			
			sprintf(strText,"%s <c:LTYELLOW>%s<n> %s",strPrice,GetMoneyString(m_shop.m_aItem[i].m_dwPrice), strDenomination);

			s_text.put(iPutX+40,iPutY+2,g_iScreenWidth/3-60,200,0,WHITE,strItemName);
			s_text.put(iPutX+40,iPutY+18,WHITE,strText);
		}
	}

	if (m_iShopStatus	==	ePSS_READY)
	{
		DrawInventoryWindow();

		if (m_iMenuMode == ePSDM_SELECT_SIGNBOARD)
		{
			g_sprInterface.Put(m_rect.x1+19,88,ePSI_SIGNBOARD_SHAPE);

			{
				int		iX	=	m_rect.x1+18;
				int		iY	=	88;
				cRECT	rect;

				for (int i=0;i<6;i++)
				{
					rect.set(iX,iY,iX+36,iY+28);
					iX	+=	41;

					if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard1	==	FALSE	&&	i	==	1)
						cDRAW::Fill(0,rect.x1,rect.y1,rect.x2,rect.y2);
					if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard2	==	FALSE	&&	i	==	2)
						cDRAW::Fill(0,rect.x1,rect.y1,rect.x2,rect.y2);
					if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard3	==	FALSE	&&	i	==	3)
						cDRAW::Fill(0,rect.x1,rect.y1,rect.x2,rect.y2);
					if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard4	==	FALSE	&&	i	==	4)
						cDRAW::Fill(0,rect.x1,rect.y1,rect.x2,rect.y2);
					if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard5	==	FALSE	&&	i	==	5)
						cDRAW::Fill(0,rect.x1,rect.y1,rect.x2,rect.y2);
				}
			}

		}

		if (m_iMenuMode == ePSDM_SELECT_TEXT_COLOR)
			g_sprInterface.Put(m_rect.x1+16,93+g_iCorrectBI_Y/2,ePSI_TEXT_COLOR);

		if (m_iMenuMode == ePSDM_SELECT_BOLD)
			s_bmRadioBox.draw();

		s_bmCheckBox.draw();
	}
}
void
CPitchmanShopWindow::drawForClient()		//09.08.28
{
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iX				=	m_rect.x1;
	int		iY				=	m_rect.y1,i,iPutX,iPutY;

	DrawInventoryWindow();
	g_sprInterface.Put(iX	,g_iCorrectBI_Y/2		,eWINDOW_TITLE_SHOP);

	s_text.someCodeIgnoreOn();
	s_text.cPut(iX+10,iY+8,g_iScreenWidth/3-20,30,0,WHITE,dFITTEXT_HEIGHT_CENTER,"%s<n><c:LTGREEN>(%s)<n>",m_shop.m_strName,m_shop.m_strOwnerName);
	s_text.someCodeIgnoreOff();

	g_sprInterface.Put(m_rect.x1+7,iY+38,eWINDOW_SEPERATE_BAR);	//	경계

	iY		+=	50;

	for (i=0;i<dMAX_ITEM_COUNT_IN_PITCHMAN_SHOP_WINDOW;i++)
	{
		iPutX	=	iX+13;
		iPutY	=	iY+i*44;

		if	(m_iSelectShopItem	== i)
		{
			iPutY--;
			cDRAW::Fill(RGB24To16(0,100,200),iPutX-4,iPutY-3,iPutX+245,iPutY+42-6);	//	선택한 아이템이다.
		}

		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_FRAME);

		if (m_iFocusSlot == i)
			g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5));

		if (m_shop.m_aItem[i].m_item.m_wBaseItem	==	0xffff)
			continue;

		cItem	*lpItem	=	(cItem *)&m_shop.m_aItem[i].m_item;

		PutItem(lpItem,iPutX,iPutY,TRUE);

		char	strItemName[256];

		strcpy(strItemName,lpItem->getName());
		char*	strDenomination = dMSG_ENG_GOLD;
		
		char	strPrice[64]	= "";
		strcpy(strPrice,dMSG_PRICE);	
		if(m_shop.m_aItem[i].m_bDenomination)
		{
			
			strDenomination = dMSG_COUNT;
			sprintf(strPrice,"%s %s",strPrice, dMSG_GOLD_BAR);
			
		}
		if	(iLineCount	>=	3)
		{
			char	strText[512];

			sprintf(strText,"%s %s<c:LTYELLOW>%s<n> %s",strItemName,strPrice,GetMoneyString(m_shop.m_aItem[i].m_dwPrice),strDenomination);

			s_text.setLineGap(0);
			s_text.cPut(iPutX+40,iPutY-12,g_iScreenWidth/3-60,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,strText);
			s_text.setLineGap(2);
		}
		else
		if	(s_text.getLineCount(g_iScreenWidth/3-60,strItemName)	>=	2)
		{
			s_text.setLineGap(0);
			s_text.cPut(iPutX+40,iPutY-2,g_iScreenWidth/3-60,40,0,WHITE,dFITTEXT_HEIGHT_CENTER,"%s\n%s <c:LTYELLOW>%s<n> %s",strItemName,strPrice,GetMoneyString(m_shop.m_aItem[i].m_dwPrice),strDenomination);
			s_text.setLineGap(2);
		}
		else
		{
			s_text.put(iPutX+40,iPutY+2,g_iScreenWidth/3-60,200,0,WHITE,strItemName);
			s_text.put(iPutX+40,iPutY+18,WHITE,"%s <c:LTYELLOW>%s<n> %s",strPrice,GetMoneyString(m_shop.m_aItem[i].m_dwPrice),strDenomination);
		}
	}

	g_sprInterface.Put(m_rect.x1+7,458+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계

	s_bmMiddleWindowButton.draw();

}

//
//	그리기
void
CPitchmanShopWindow::draw()
{
	if (!isOpened())
		return;

	int		iX,iY,iCount	=	0;
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX	,iY		,eWINDOW);
	
	iY		+=	dSHOP_INTERFACE_TITLE_HEIGHT;

	if (m_iOwner	==	ePS_SHOP_KEEPER)
		drawForShopkeeper();
	else
		drawForClient();
}


int
CPitchmanShopWindow::getFocusSlotForShopKeeper()
{
	int	iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iX			=	m_rect.x1+7;
	int	iY			=	126+g_iCorrectBI_Y/2,i;
	int	iPutX,iPutY;

	for (i=0;i<g_hero.m_wPitchmanShopSize;i++)
	{
		iPutX	=	iX+6;
		iPutY	=	iY+i*38;

		cRECT	rect;

		rect.set(iPutX,iPutY,iPutX+iWindowWidth,iPutY+40);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(-4,0);

			return	i;
		}
	}

	return	0xffff;
}

int
CPitchmanShopWindow::getFocusSlotForClient()
{
	int		iLineCount		=	0;
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iX				=	m_rect.x1;
	int		iY				=	m_rect.y1+50,i,iPutX,iPutY;

	for (i=0;i<dMAX_ITEM_COUNT_IN_PITCHMAN_SHOP_WINDOW;i++)
	{
		iPutX	=	iX+13;
		iPutY	=	iY+i*44;

		cRECT	rect;

		rect.set(iPutX,iPutY,iPutX+iWindowWidth,iPutY+40);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(-4,0);

			return	i;
		}
	}

	return	0xffff;
}

int
CPitchmanShopWindow::getSellingItemCount()
{
	int	i,iCount=0;

	for (i=0;i<dPITCHMAN_SHOP_INVENTORY_SIZE;i++)
	{
		if (m_shop.m_aItem[i].m_item.m_wBaseItem	!=	0xffff)
			iCount++;
	}

	return	iCount;
}

void
CPitchmanShopWindow::updateForShopkeeper()
{
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);

	if (s_inputChatting.isEnable() == FALSE && m_iShopStatus == ePSS_READY)
	{
		if (g_hero.m_bf1IsOwnedPitchmanShopLongPen	==	TRUE)
			EnableChattingBar(TRUE,dPITCHMAN_SHOP_NAME_LENGTH-2);
		else
			EnableChattingBar(TRUE,(dPITCHMAN_SHOP_NAME_LENGTH-2)/2);

		s_inputChatting.setStr(0,m_strShopName);
	}

	if (m_iShopStatus	==	ePSS_READY)
	{
		OperateInventoryWindow();
		OperateDragItem();	//	드래그 아이템 처리

		s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth*2;
		s_rectInterfaceSize.x2	=	g_iScreenWidth;
		m_rect.x1				=	g_iScreenWidth-iWindowWidth*2;
		m_rect.x2				=	g_iScreenWidth-iWindowWidth;
		s_bmMiddleWindowButton.setCorrectPos(0,0);
	}
	else
	{
		s_rectInterfaceSize.x1 = g_iScreenWidth-iWindowWidth;
		s_rectInterfaceSize.x2 = g_iScreenWidth;			//	인터페이스 넓이
		m_rect.x1				=	g_iScreenWidth-iWindowWidth;
		m_rect.x2				=	g_iScreenWidth;
		s_bmMiddleWindowButton.setCorrectPos(iWindowWidth,0);
	}
		
	if (cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON)	==	FALSE)
	{
		if (m_iShopStatus	==	ePSS_READY)
		{
			s_inputChatting.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,s_aTouched[KEY_TAB]);
			s_inputChatting.getStr(0,m_strShopName,128);

		//	각종 메뉴들의 툴팁
			{
				cBAR	*lpFocusedBar	=	s_bmMiddleWindowButton.getFocusMenu(s_posMouse.x,s_posMouse.y);

				if (lpFocusedBar)
				{
					cRECT	rect;

					rect.set(&lpFocusedBar->m_rectCrash);
					rect.add(0,30);

					char	*lpstrComment	=	lpFocusedBar->getTooltip();

					if (lpstrComment)
						s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,lpstrComment);
				}
			}	//	각종 메뉴들의 툴팁

		}

		DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (m_wIsWaitPichmanShopWorkResult)
			dwMenu		=	0xffff;

		switch(dwMenu)
		{
			case	ePSM_BEGIN_SHOP:
			{
				if (strlen(m_strShopName) < 2)
				{
					AddSystemMessage(LTYELLOW,dMSG_PITCHMAN_SHOP_NAME_RULE);
					return;
				}

				if (getSellingItemCount() <= 0)
				{
					AddSystemMessage(LTYELLOW,dMSG_SELLING_ITEM_COUNT_ZERO);
					return;
				}

				if (CheckFilter(m_strShopName,g_pstrSladerFilter,g_loaderText.m_iSlanderTextCount))
				{
					AddSystemMessage(LTYELLOW,dMSG_PLEASE_USE_GOOD_WORD);

					return;
				}
				

				s_agent.sendChangePitchmanShopInfo(m_shop.m_wSerial,ePSS_OPEN,m_strShopName,m_iSignboardShape,m_wTextColor,m_bIsBold,m_bIsShadowText);
				m_wIsWaitPichmanShopWorkResult	=	TRUE;

				break;
			}


			case	ePSM_STOP_SHOP	:
			{
				s_agent.sendChangePitchmanShopInfo(m_shop.m_wSerial,ePSS_READY,m_shop.m_strName,m_shop.m_bf4SignboardShape,m_shop.m_wTextColor,m_shop.m_bf1IsBold,m_shop.m_bf1IsShadowText);
				m_wIsWaitPichmanShopWorkResult	=	TRUE;
				break;
			}

			case	ePSM_CLOSE_SHOP	:
			{
				g_msgBox.cPopup("",dMSG_DO_YOU_STOP_PITCHMAN_SHOP,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_STOP_PITCHMAN_SHOP);
				return;
			}

			case	ePSM_SELECT_SIGNBOARD_SHAPE	:
				s_bmRadioBox.hide(ePSM_BOLD_TEXT);
				s_bmRadioBox.hide(ePSM_NORMAL_TEXT);
				m_iMenuMode	=		ePSDM_SELECT_SIGNBOARD;
				break;

			case	ePSM_SELECT_TEXT_COLOR		:
				s_bmRadioBox.hide(ePSM_BOLD_TEXT);
				s_bmRadioBox.hide(ePSM_NORMAL_TEXT);
				m_iMenuMode	=		ePSDM_SELECT_TEXT_COLOR;
				break;

			case	ePSM_SELECT_BOLD			:
				m_iMenuMode	=		ePSDM_SELECT_BOLD;
				s_bmRadioBox.show(ePSM_BOLD_TEXT);
				s_bmRadioBox.show(ePSM_NORMAL_TEXT);
				break;

			case	ePSM_SELECT_EFFECT			:
				s_bmRadioBox.hide(ePSM_BOLD_TEXT);
				s_bmRadioBox.hide(ePSM_NORMAL_TEXT);
				m_iMenuMode	=		ePSDM_SELECT_EFFECT;
				break;
		}

		if (m_iMenuMode == ePSDM_SELECT_BOLD)
		{
			dwMenu	=	s_bmRadioBox.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

			if (dwMenu	==	ePSM_BOLD_TEXT)
				m_bIsBold	=	TRUE;
			if (dwMenu	==	ePSM_NORMAL_TEXT)
				m_bIsBold	=	FALSE;
		}

		if (s_bmCheckBox.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed) ==	ePSM_TEXT_SHADOW)
			m_bIsShadowText	=	1-m_bIsShadowText;

		if (s_bmMiddleWindowButton.isPressed() + s_bmRadioBox.isPressed())
			cINPDEV::SetExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON);
		else
			cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_PITCHMAN_SHOP_BUTTON);

	}

	g_pitchmanShop.setInfoForShopKeeper(m_shop.m_wSerial,m_strShopName,m_iSignboardShape,m_wTextColor,m_bIsBold,m_bIsShadowText);

	if (cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_ITEM_DRAG)	==	FALSE)
		operateDragItem();

	if (cINPDEV::IsExclusive())
		if (!cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_ITEM_DRAG) && !cINPDEV::IsExclusive(eMOUSE_STATE_ITEM_DRAG))
			return;

	m_iFocusSlot	=	getFocusSlotForShopKeeper();

	if (m_iFocusSlot	!=	0xffff && cINPDEV::IsExclusive() == FALSE)
	{
		cItem	*lpItem	=	g_hero.getInventoryItem(m_shop.m_aItem[m_iFocusSlot].m_item.m_wBaseItem,TRUE);

		if (lpItem && lpItem->m_wBaseItem	!=	0xffff)
		{
			char	*strComment	=	lpItem->getItemTooltip(-1,-1,TRUE);

			strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);

			WORD	*lpImage	=	g_smiIconItem.get16Image(lpItem->getBasicItem(TRUE)->m_wIconShape);
			s_ttCommon.setImage(lpImage);
		}
	}

	if (cINPDEV::IsExclusive())
		return;

	if (m_iMenuMode == ePSDM_SELECT_TEXT_COLOR)
	{
		cRECT	rect;

		rect.set(m_rect.x1+17,94,m_rect.x1+16+g_iScreenWidth/3-30,93+18);

		if (rect.isIn(s_posMouse.x,s_posMouse.y) && s_isLeftButtonPressed)
		{
			m_wTextColor	=	cDRAW::p2SCREEN[s_posMouse.x+s_posMouse.y*cDRAW::WIDTH];
			m_wTextColor	=	ToRGB565(m_wTextColor);

			g_pitchmanShop.setInfoForShopKeeper(m_shop.m_wSerial,m_strShopName,m_iSignboardShape,m_wTextColor,m_bIsBold,m_bIsShadowText);
		}
	}

	if (m_iMenuMode == ePSDM_SELECT_SIGNBOARD && s_isTouchLeftButton)
	{
		int		iX	=	m_rect.x1+18;
		int		iY	=	90;
		cRECT	rect;

		for (int i=0;i<6;i++)
		{
			rect.set(iX,iY,iX+34,iY+26);
			iX	+=	41;

			if (rect.isIn(s_posMouse.x,s_posMouse.y) && s_isTouchLeftButton)
				m_iSignboardShape	=	i;

			if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard1	==	FALSE	&&	m_iSignboardShape	==	1)
				m_iSignboardShape	=	0;
			if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard2	==	FALSE	&&	m_iSignboardShape	==	2)
				m_iSignboardShape	=	0;
			if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard3	==	FALSE	&&	m_iSignboardShape	==	3)
				m_iSignboardShape	=	0;
			if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard4	==	FALSE	&&	m_iSignboardShape	==	4)
				m_iSignboardShape	=	0;
			if (g_hero.m_bf1IsOwnedPitchmanShopSignBoard5	==	FALSE	&&	m_iSignboardShape	==	5)
				m_iSignboardShape	=	0;
		}
	}
}

void
CPitchmanShopWindow::buyPitchmanShopItem()		// 09.08.21
{
	cItem	*lItem	=	(cItem *)&m_shop.m_aItem[m_iBookingBuyItem].m_item;

	if	(lItem->m_wBaseItem	==	0xffff)
		return;

	s_agent.sendBuyPitchmanShopItem(m_shop.m_wSerial,m_iBookingBuyItem,lItem,m_shop.m_aItem[m_iBookingBuyItem].m_dwPrice,m_shop.m_aItem[m_iBookingBuyItem].m_bDenomination);

	m_wIsWaitPichmanShopWorkResult	=	TRUE;
}

void
CPitchmanShopWindow::updateForClient()	// 09.08.21
{
	if (cINPDEV::IsExclusive()	==	FALSE)
	{
		DWORD	dwMenu	=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (m_wIsWaitPichmanShopWorkResult)
			dwMenu		=	0xffff;

		if (dwMenu		==	ePSM_BUY_ITEM)
		{
			if	(m_iSelectShopItem >= 0 && m_iSelectShopItem < dMAX_ITEM_COUNT_IN_PITCHMAN_SHOP_WINDOW)
			{
				cItem	*lItem	=	(cItem *)&m_shop.m_aItem[m_iSelectShopItem].m_item;

				if	(lItem->m_wBaseItem	!=	0xffff)
				{
					m_iBookingBuyItem	=	m_iSelectShopItem;
					if(m_shop.m_aItem[m_iSelectShopItem].m_bDenomination)
					{
						g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_BUY_SELECT_ITEM_FORM_GOLD_BAR,m_shop.m_aItem[m_iSelectShopItem].m_dwPrice,lItem->getName()),dMSG_YES,dMSG_NO);
					}
					else
					{
						g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_BUY_SELECT_ITEM_FORM,GetMoneyString(m_shop.m_aItem[m_iSelectShopItem].m_dwPrice),lItem->getName()),dMSG_YES,dMSG_NO);
					}
					
					g_msgBox.setButtonId(eMBI_BUY_PITCHMAN_SHOP_ITEM,eMBI_CANCEL);
				}
			}

			return;
		}

		if (dwMenu		==	ePSM_CLOSE_SHOP)
		{
			close();
			return;

		}
	}

	if (cINPDEV::IsExclusive())
		return;

	OperateInventoryWindow();

	m_iFocusSlot	=	getFocusSlotForClient();

	if(s_isTouchLeftButton && m_iFocusSlot != 0xffff)
		m_iSelectShopItem	=	m_iFocusSlot;

	if (m_iSelectShopItem >= 0 && m_iSelectShopItem < dMAX_ITEM_COUNT_IN_PITCHMAN_SHOP_WINDOW)
	{
		if (m_shop.m_aItem[m_iSelectShopItem].m_item.m_wBaseItem	==	0xffff)
			m_iSelectShopItem	=	-1;
	}

	if (m_iFocusSlot	!=	0xffff)
	{
		cItem	*lpItem	=	(cItem *)&m_shop.m_aItem[m_iFocusSlot].m_item;

		if (lpItem->m_wBaseItem	!=	0xffff)
		{
			char	*strComment	=	lpItem->getItemTooltip(-1,-1,TRUE);
			
			strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);

			WORD	*lpImage	=	g_smiIconItem.get16Image(lpItem->getBasicItem(TRUE)->m_wIconShape);
			s_ttCommon.setImage(lpImage);
		}
	}
}
void
CPitchmanShopWindow::askAddSellingItem(int _iItemSlot,int _iPrice, BYTE	_bDenomination)		// 09.08.21
{
	
	if (_iPrice < 0 || _iPrice > 1000000000 && _bDenomination == FALSE)
	{
		if(_iPrice > 1000000000)
			AddSystemMessage(LTGREEN,dMSG_SELL_ITEM_GOLD_OVER);
		return;
	}
	if (_iItemSlot < 0 || _iItemSlot >= g_hero.m_wItemSlotCount)
		return;

	s_agent.sendAddPitchmanShopItem(m_shop.m_wSerial,_iItemSlot,m_iSellItemSlot,_iPrice,_bDenomination);
}


//
//	아이템 드래그 처리
void 
CPitchmanShopWindow::operateDragItem()
{
	int	iFocusItem	=	getFocusSlotForShopKeeper();

	if	(m_iOwner	==	ePS_CLIENT)
		return;

	if	(m_iShopStatus==	ePSS_OPEN)
		return;

	if	(iFocusItem	==	0xffff && m_iDragItem	==	0xffff)
		return;

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_ITEM_DRAG))
		return;

	if	(s_isTouchLeftButton	&&	m_iDragItem	==	0xffff)
	{
		m_iDragItem	=	iFocusItem;

		cINPDEV::SetExclusive(eMOUSE_STATE_PITCHMAN_SHOP_ITEM_DRAG);
	}

	if	(s_isLeftButtonPressed	||	m_iDragItem	==	0xffff	)
		return;

	int		iX	=	s_posInventoryWindow.x;
	int		iY	=	s_posInventoryWindow.y;

	cRECT	rectFrame;

//	아이템 창
	rectFrame.set(iX+18,iY,iX+252,iY+468);

	if	(rectFrame.isIn(s_posMouse.x,s_posMouse.y))
		s_agent.sendRemovePitchmanShopItem(m_shop.m_wSerial,m_iDragItem);
	else
	{
		if	(m_iFocusSlot	!=	0xffff)
			s_agent.sendChangePitchmanShopItemPlace(m_shop.m_wSerial,m_iDragItem,m_iFocusSlot);
	}

	cINPDEV::ReleaseExclusive();

	m_iDragItem	=	0xffff;
}	//	CGamePlay::OperateDragItem()

void
CPitchmanShopWindow::operateDropItem(int _iInventorySlot)
{
	if	(m_iFocusSlot	==	0xffff)
		return;
	
	if (m_iOwner	==	ePS_CLIENT)
		return;

	if (m_iShopStatus==	ePSS_OPEN)
	{
		g_msgBox.cPopup("",dMSG_PLEASE_STOP_SHOP_FOR_ADD_SELLING_ITEM,dMSG_OK);
		return;
	}

	if (_iInventorySlot	==	dITEM_DRAG_GOLD)
		return;

	if (_iInventorySlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
	{
		AddSystemMessage(LTGREEN,dMSG_CAN_NOT_SELL_EQUIPPED_ITEM);
		return;
	}

	cItem	*lpItem	=	&g_hero.m_aItems[_iInventorySlot];

	if	(lpItem->isCanNotTradeItem())
	{
		g_lpHero->warning(dMSG_CAN_NOT_TRADE_ITEM);
		return;
	}

	g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,_iInventorySlot,&g_hero.m_aItems[_iInventorySlot],eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT,s_shop.m_wPriceFactor);

	m_iSellItemSlot	=	m_iFocusSlot;

	cINPDEV::SetExclusive(eMOUSE_STATE_GET_ITEM_COUNT);
}

BOOL
CPitchmanShopWindow::isSellingInventorySlot(int _iSlot)
{
	if (m_iOwner	!=	ePS_SHOP_KEEPER)
		return	FALSE;

	for (int i=0;i<dPITCHMAN_SHOP_INVENTORY_SIZE;i++)
	{
		if (m_shop.m_aItem[i].m_item.m_wBaseItem	==	_iSlot)
			return	TRUE;
	}

	return	FALSE;		
}

//
//	업데이트
void
CPitchmanShopWindow::update()
{
	if (!isOpened())
		return;

	if (g_itemCountWindow.m_bIsOpend)
		return;

	if (m_iOwner	==	ePS_SHOP_KEEPER)
		updateForShopkeeper();
	else
		updateForClient();
}
