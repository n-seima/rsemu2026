#include "CGamePlayInterface.H"
#include "CHero.H"
#include "CMessage.H"
#include "CWindowInterface.H"

CGamePlayInterfaceItemCount	g_itemCountWindow;

CGamePlayInterfaceItemCount::CGamePlayInterfaceItemCount()
{
	m_bIsOpend		=	FALSE;
	m_iItemSlot		=	0xffff;
	m_iFocusSlot	=	0xffff;		// 09.09.01
}
//
//	닫기
BOOL
CGamePlayInterfaceItemCount::close()
{
	m_bIsOpend		=	FALSE;
	m_inputGold.close();

	return	FALSE;
}

//
//	닫기
BOOL
CGamePlayInterfaceItemCount::reset()
{
	m_bIsOpend		=	FALSE;

	m_inputGold.close();

	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_GET_ITEM_COUNT);

	return	FALSE;
}

//
//
void
CGamePlayInterfaceItemCount::init()		
{
	m_sbCount.init(&g_sprInterface,dIMAGE_SCROLL_BAR_HORIZON,
									eSCROLLBAR_HORIZON_NORMAL_LEFT_BUTTON,eSCROLLBAR_HORIZON_ACTIVE_LEFT_BUTTON,eSCROLLBAR_HORIZON_PRESSED_LEFT_BUTTON,0xffff,
									eSCROLLBAR_HORIZON_NORMAL_RIGHT_BUTTON,eSCROLLBAR_HORIZON_ACTIVE_RIGHT_BUTTON,eSCROLLBAR_HORIZON_PRESSED_RIGHT_BUTTON,0xffff,
									eSCROLLBAR_HORIZON_NORMAL_BODY_TOP,eSCROLLBAR_HORIZON_NORMAL_BODY,eSCROLLBAR_HORIZON_NORMAL_BODY_BOTTOM,
									eSCROLLBAR_HORIZON_ACTIVE_BODY_TOP,eSCROLLBAR_HORIZON_ACTIVE_BODY,eSCROLLBAR_HORIZON_ACTIVE_BODY_BOTTOM);

	m_bmButton.init(&g_sprInterface,10);
	m_bmRadioButton.reset();
	m_bmRadioButton.init(&g_sprInterface,2,dBAR_RADIO,eBTOM_BAR_RIGHT);
	m_bmButton.addImageBar(dMSG_OK		,IDOK		,16,68,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	m_bmButton.addImageBar(dMSG_CANCEL	,IDCANCEL	,88,68,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	m_bmButton.setClickMargin(IDOK,1,1);
	m_bmButton.setClickMargin(IDCANCEL,1,1);

	m_bmButton.addImageBar("",dBM_ICW_1				,47,53,eITEM_DROP_1_NORMAL,eITEM_DROP_1_ACTIVE,eITEM_DROP_1_ACTIVE,0xffff,0xffff);
	m_bmButton.addImageBar("",dBM_ICW_5				,69,53,eITEM_DROP_5_NORMAL,eITEM_DROP_5_ACTIVE,eITEM_DROP_5_ACTIVE,0xffff,0xffff);
	m_bmButton.addImageBar("",dBM_ICW_10			,91,53,eITEM_DROP_10_NORMAL,eITEM_DROP_10_ACTIVE,eITEM_DROP_10_ACTIVE,0xffff,0xffff);
	m_bmButton.addImageBar("",dBM_ICW_HALF			,113,53,eITEM_DROP_HALF_NORMAL,eITEM_DROP_HALF_ACTIVE,eITEM_DROP_HALF_ACTIVE,0xffff,0xffff);
	m_bmButton.addImageBar("",dBM_ICW_ALL			,135,53,eITEM_DROP_ALL_NORMAL,eITEM_DROP_ALL_ACTIVE,eITEM_DROP_ALL_ACTIVE,0xffff,0xffff);
	m_bmButton.setClickImageMargin(dBM_ICW_1,1,1);
	m_bmButton.setClickImageMargin(dBM_ICW_5,1,1);
	m_bmButton.setClickImageMargin(dBM_ICW_10,1,1);
	m_bmButton.setClickImageMargin(dBM_ICW_HALF,1,1);
	m_bmButton.setClickImageMargin(dBM_ICW_ALL,1,1);

	m_bmButton.setAllBarTextColor(0,FALSE);



	m_bmRadioButton.addImageBar(dMSG_ENG_GOLD,	eBM_RADIO_GOLD,45,26,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);			//	골드버튼
	m_bmRadioButton.addImageBar(dMSG_GOLD_BAR,	eBM_RADIO_GOLD_BAR,93,26,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);		//	금괴버튼


}


//
//	드랍 아이템 윈도우 열기

BOOL
CGamePlayInterfaceItemCount::popup(int _iX,int _iY,int _iItemSlot, cItem *_lpItem,int _iWorkResult,int _iPriceFactor, int _iFocusSlot,cTALK* _lpInput,int _iIndex)	
{
	m_lpRefInput	=	_lpInput;
	m_iRefInputIndex=	_iIndex;

	if	(m_lpRefInput	==	NULL)
		m_lpRefInput	=	&m_inputGold;

	m_iWorkResult	=	_iWorkResult;
	m_iPriceFactor	=	_iPriceFactor;
	m_iItemSlot		=	_iItemSlot;		
	m_iFocusSlot	=	_iFocusSlot;		// 09.09.01
	m_bDenomination = 0;

	m_bmButton.correctPos(IDOK		,16,68);
	m_bmButton.correctPos(IDCANCEL	,88,68);

	if	(_lpItem)
		memcpy(&m_item,_lpItem,sizeof(cItem));

	m_bIsOpend		=	TRUE;
	m_iCount		=	1;
	m_iItemCount	=	min(getItemCount(),0x7fffffff);

	if	(_lpItem)
		if	(_lpItem->isExtraItem() || _lpItem->isMagicCarpetEmblem())
			m_iItemCount=	1;

	int	iWidth		=	g_sprInterface.getSpriteWidth(eITEM_DROP_FRAME);
	int	iHeight		=	g_sprInterface.getSpriteHeight(eITEM_DROP_FRAME);

	if	(_iWorkResult	!=	eICWR_RETURN_MOTP_CODE)
	{
		_iX				-=	iWidth/2;
		_iY				-=	iHeight/2;
		_iX				=	max(_iX,0);
		_iY				=	max(_iY,0);

		if (_iX+iWidth	>	g_iScreenWidth		)
			_iX	=	g_iScreenWidth-iWidth;
		if (_iY+iHeight	>	g_iFieldScreenHeight)
			_iY	=	g_iFieldScreenHeight-iHeight;
	}

	m_pos.Set(_iX,_iY);
	m_bmButton.setCorrectPos(m_pos.x,m_pos.y);

	if	(_iWorkResult!=eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT && _iWorkResult!=eICWR_RETURN_MOTP_CODE)
	{
		if	(_iWorkResult==eICWR_RETURN_SELL_ITEM_COUNT )
			m_sbCount.set(m_pos.x+46,m_pos.y+38,76);
		else
			m_sbCount.set(m_pos.x+46,m_pos.y+38,111);
	}

	int	iCount		=	m_iItemCount;

	m_sbCount.setSize(iCount,1);
	m_sbCount.setPos(m_iCount-1);

	s_inputChatting.disable(TRUE);

	if	(m_iWorkResult	>=	eICWR_RETURN_TRADE_GOLD && m_iWorkResult	<=	eICWR_RETURN_WITHDRAW_GOLD_FROM_GUILD_INVENTORY )
	{
 		m_inputGold.init(s_hWnd,s_hInst,&s_text,1);
		m_inputGold.set(_iX+52,_iY+23,0,_iX+52,_iY+23,100,13,12,"1",ES_NUMBER);
		m_inputGold.setColor(0,WHITE);//,FALSE);
		m_inputGold.setStr(0,"1");
		m_inputGold.active(0,TRUE);
	}

	if	(m_iWorkResult	==	eICWR_RETURN_MOTP_CODE)
	{

		strcpy(m_strTitle,dMSG_PLEASE_INPUT_CODE);
		
		int	iFrameWidth	=	g_iScreenWidth/3;
		int	iY	=	m_pos.y+14;

		m_rect.set(_iX-iFrameWidth/2,_iY-4,_iX+iFrameWidth,_iY+82);

		m_bmButton.setCorrectPos(dBM_ICW_1		,1000,1000);
		m_bmButton.setCorrectPos(dBM_ICW_5		,1000,1000);
		m_bmButton.setCorrectPos(dBM_ICW_10		,1000,1000);
		m_bmButton.setCorrectPos(dBM_ICW_HALF	,1000,1000);
		m_bmButton.setCorrectPos(dBM_ICW_ALL	,1000,1000);

		int	iSize		=	s_text.getPixelSize(m_strTitle);
		int	iWidth		=	g_sprInterface.getSpriteWidth(eITEM_PRICE_FRAME);
		int	iInputWidth	=	66;

		m_iInputPosForItemPrice	=	m_pos.x-iInputWidth/2;

		iWidth			=	m_iInputPosForItemPrice+iWidth+50+32;
		m_rect.x2		=	m_rect.x1+iFrameWidth;

		m_iInputPosForItemPrice	=	iInputWidth/2;

		m_lpRefInput->set(_iX-m_iInputPosForItemPrice	,iY+18,m_iRefInputIndex,
						_iX-m_iInputPosForItemPrice		,iY+18,iInputWidth,13,6,"",ES_NUMBER);

		m_lpRefInput->active(m_iRefInputIndex,TRUE);

		int	iButtonWidth=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
		m_bmButton.setCorrectPos(IDOK		,0,0);
		m_bmButton.setCorrectPos(IDCANCEL	,0,0);

		m_bmButton.correctPos(IDOK		,m_pos.x-iButtonWidth-2	,m_pos.y+52);
		m_bmButton.correctPos(IDCANCEL	,m_pos.x+2				,m_pos.y+52);
	}

	if	(m_iWorkResult	==	eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT)
	{
		m_rect.set(_iX,_iY-4,_iX+g_iScreenWidth/3,_iY+105);//92);

		int	iY	=	m_pos.y+28;

		m_iItemCount	=	0x7fffffff;
		
		m_bmButton.setCorrectPos(dBM_ICW_1		,1000,1000);
		m_bmButton.setCorrectPos(dBM_ICW_5		,1000,1000);
		m_bmButton.setCorrectPos(dBM_ICW_10		,1000,1000);
		m_bmButton.setCorrectPos(dBM_ICW_HALF	,1000,1000);
		m_bmButton.setCorrectPos(dBM_ICW_ALL	,1000,1000);

		int	iSize		=	s_text.getPixelSize(dMSG_SELL_PRICE);
		int	iWidth		=	g_sprInterface.getSpriteWidth(eITEM_PRICE_FRAME);
		int iAddHeight	=	0;
		int	iItemNameSize=	s_text.getPixelSize(m_item.getName());

		m_iInputPosForItemPrice	=	iSize;
		
		iWidth			=	m_iInputPosForItemPrice+iWidth+50+32;

		if(iItemNameSize/2>=iWidth)
		{
			iWidth			=	iItemNameSize/2;
		}
		if(iItemNameSize>iWidth)
		{
			iAddHeight	=	15;
			iWidth		+=	4;
		}
		
		

		m_iPrice		=	1;

		m_rect.x2		=	m_rect.x1+iWidth;
		m_rect.y2		+=	iAddHeight;
 		m_inputGold.init(s_hWnd,s_hInst,&s_text,1);
		m_inputGold.set(_iX+50+m_iInputPosForItemPrice,iY+18 +iAddHeight	,0,_iX+50+m_iInputPosForItemPrice+27,iY+18+iAddHeight,60,13,12,"1",ES_NUMBER);
		m_inputGold.setColor(0,WHITE);
		m_inputGold.setStr(0,"1");
		m_inputGold.active(0,TRUE);
		m_inputGold.enable();

		int	iButtonWidth=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
		m_bmButton.setCorrectPos(IDOK		,m_pos.x-iButtonWidth+iWidth/2-16	,m_pos.y+12 +iAddHeight);
		m_bmButton.setCorrectPos(IDCANCEL	,m_pos.x-iButtonWidth+iWidth/2-16-8	,m_pos.y+12+ iAddHeight);

		m_bmRadioButton.setCorrectPos(eBM_RADIO_GOLD		,m_pos.x	,m_pos.y+iAddHeight);
		m_bmRadioButton.setCorrectPos(eBM_RADIO_GOLD_BAR	,m_pos.x	,m_pos.y+iAddHeight);
		m_bmRadioButton.check(eBM_RADIO_GOLD + m_bDenomination);

	}

	update();

	return	TRUE;
}


//
//	아이템 윈도우 업데이트
int
CGamePlayInterfaceItemCount::update()		// 09.08.21
{
	if (!m_bIsOpend)
		return	eICWR_NONE;

	if (m_iWorkResult	>=	eICWR_RETURN_TRADE_GOLD && m_iWorkResult	<=	eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT)
	{
		char	strValue[20];
		int		iValue;

 		m_inputGold.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,FALSE);
		m_inputGold.getStr(0,strValue,20);
		
		iValue	=	atoi(strValue);

		if	(iValue	<	0)
			iValue	=	2000000000;

		iValue		=	min(iValue,2000000000);

		if (iValue	!=	m_iCount)
		{
			m_iCount	=	iValue;
			m_sbCount.setPos(m_iCount-1);

 			if (iValue	>=	0)
 				m_inputGold.setStr(0,_ms("%d",m_iCount));
		}
	}

	if (m_iWorkResult	==	eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT)
	{
		char	strValue[20];
		int		iValue;

		m_inputGold.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,s_aTouched[KEY_TAB]);

		m_inputGold.getStr(0,strValue,20);

		iValue		=	atoi(strValue);

		if	(iValue	<	0)
			iValue	=	2000000000;

		iValue		=	min(iValue,2000000000);

		m_iPrice	=	iValue;
	}

	cINPDEV::SetExclusive(eMOUSE_STATE_GET_ITEM_COUNT);

	int	iOldPos	=	m_sbCount.m_iCurrentPos;

	m_sbCount.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (iOldPos	!=	m_sbCount.m_iCurrentPos)
	{
		m_iCount=	m_sbCount.m_iCurrentPos+1;
		m_inputGold.setStr(0,_ms("%d",m_iCount));
	}

	DWORD	dwMenu	=	m_bmButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (s_aTouched[KEY_RETURN] + s_aTouched[KEY_NUMPADENTER])
		dwMenu		=	IDOK;

	switch(dwMenu)
	{
		case	dBM_ICW_1		:
			m_iCount	=	1;
			if (m_iWorkResult	>=	eICWR_RETURN_TRADE_GOLD)	m_inputGold.setStr(0,_ms("%d",m_iCount));
			break;
		case	dBM_ICW_5		:
			m_iCount	=	min(5,m_iItemCount);
			if (m_iWorkResult	>=	eICWR_RETURN_TRADE_GOLD)	m_inputGold.setStr(0,_ms("%d",m_iCount));
			break;
		case	dBM_ICW_10		:
			m_iCount	=	min(10,m_iItemCount);
			if (m_iWorkResult	>=	eICWR_RETURN_TRADE_GOLD)	m_inputGold.setStr(0,_ms("%d",m_iCount));
			break;
		case	dBM_ICW_HALF	:
			m_iCount	=	max(m_iItemCount/2,1);
			if (m_iWorkResult	>=	eICWR_RETURN_TRADE_GOLD)	m_inputGold.setStr(0,_ms("%d",m_iCount));
			break;
		case	dBM_ICW_ALL		:
			m_iCount	=	m_iItemCount;
			if (m_iWorkResult	>=	eICWR_RETURN_TRADE_GOLD)	m_inputGold.setStr(0,_ms("%d",m_iCount));
			break;
		case	IDOK			:
		{
			reset();

			if	(m_iWorkResult	==	eICWR_RETURN_MOTP_CODE)
				return	m_iWorkResult;

			if	(m_iCount	>	m_iItemCount	||	m_iCount	<=	0)
			{
				m_iCount	=	0;
				return	eICWR_CANCEL;
			}

			return	m_iWorkResult;
		}

		case	IDCANCEL		:
			reset();
			m_iCount	=	0;
			return	eICWR_CANCEL;
	}


	DWORD	dwMenuRadio	=	m_bmRadioButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenuRadio)
	{

	case eBM_RADIO_GOLD:
		m_bDenomination	= eBM_RADIO_GOLD;
		break;

	case eBM_RADIO_GOLD_BAR:
		m_bDenomination	= eBM_RADIO_GOLD_BAR;
		break;

	}


	switch(m_iWorkResult)
	{
		case	eICWR_RETURN_SELL_ITEM_COUNT	:
		{
			DWORD	dwPrice	=	m_item.getPrice(m_iCount);

			if (!m_item.isExtraItem())
			{
				dwPrice	=	dwPrice*m_iPriceFactor/100;
				dwPrice	=	min(dwPrice,s_shop.m_dwLimitPrice*m_iCount);
			}

			sprintf(m_strTitle,"%s %s G",dMSG_PRICE,GetMoneyString(dwPrice));
			break;
		}

		case	eICWR_RETURN_DROP_ITEM_COUNT			:
		case	eICWR_RETURN_TRADE_ITEM_COUNT			:
		case	eICWR_RETURN_CHANGE_TRADE_ITEM_COUNT	:
		case	eICWR_RETURN_DROP_INVENTORY_ITEM_DIVIDE	:		//09.09.01
		{
			m_iCount	=	m_iCount;
			strcpy(m_strTitle,dMSG_MOVE_ITEM_COUNT);
			break;
		}

		case	eICWR_RETURN_TRADE_GOLD						:
		case	eICWR_RETURN_DROP_GOLD						:
		case	eICWR_RETURN_STORE_GOLD_TO_THE_BANK			:
		case	eICWR_RETURN_STORE_GOLD_TO_GUILD_INVENTORY	:
		case	eICWR_RETURN_WITHDRAW_GOLD_FROM_THE_BANK	:
		case	eICWR_RETURN_WITHDRAW_GOLD_FROM_GUILD_INVENTORY	:
		case	eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT	:
		{
			m_strDenomination = dMSG_ENG_GOLD;
			if(m_bDenomination)
				m_strDenomination = dMSG_GOLD_BAR;
			
			sprintf(m_strTitle,"<c:LTYELLOW>%s %s<n>",GetMoneyString(m_iCount), m_strDenomination);
			break;
		}
	}

	m_sbCount.setPos(m_iCount-1);

	return	eICWR_NONE;
}

//
//	 드랍 아이템 윈도우 그려주기
void
CGamePlayInterfaceItemCount::draw()		// 09.08.21
{
	if (!m_bIsOpend)
		return;

	switch(m_iWorkResult)
	{
		case	eICWR_RETURN_MOTP_CODE	:
		{
			s_ttCommon.drawBox(&m_rect,0,32);

			int	iY			=	m_pos.y+14;
			int	iFrameWidth	=	g_iScreenWidth/3;
			int	iInputWidth	=	66;

			s_text.cPut(m_pos.x,m_pos.y+7,WHITE,m_strTitle);

			cRECT	rect;

			rect.x1	=	m_pos.x-m_iInputPosForItemPrice-4;
			rect.y1	=	iY+14;
			rect.x2	=	rect.x1+iInputWidth+8;
			rect.y2	=	rect.y1+18;

			s_ttCommon.drawBox(&rect,0,32);
		
			m_lpRefInput->draw(BLACK);

			break;
		}

		case	eICWR_RETURN_SELL_ITEM_COUNT	:
		{
			g_sprInterface.Put(m_pos.x,m_pos.y,eITEM_DROP_FRAME);
			s_text.pushShadowStatus(FALSE);
			s_text.put	(m_pos.x+50		,m_pos.y+24	,BLACK,m_strTitle);
			s_text.putRF(m_pos.x+46+109 ,m_pos.y+39	,WHITE,dMSG_ITEM_COUNT_FORM,m_iCount);
			s_text.put	(m_pos.x+6		,m_pos.y+7	,WHITE,dMSG_PREPARE_SELL_ITEM);
			s_text.popShadowStatus();
			m_sbCount.draw();
			m_bmButton.draw();
			PutItem(&m_item,m_pos.x+7,m_pos.y+26,FALSE,FALSE);
			break;
		}

		case	eICWR_RETURN_DROP_ITEM_COUNT			:
		case	eICWR_RETURN_TRADE_ITEM_COUNT			:
		case	eICWR_RETURN_CHANGE_TRADE_ITEM_COUNT	:
		case	eICWR_RETURN_DROP_INVENTORY_ITEM_DIVIDE:			// 09.09.01
		{
			g_sprInterface.Put(m_pos.x,m_pos.y,eITEM_DROP_FRAME);
			s_text.pushShadowStatus(FALSE);
			s_text.cPut(m_pos.x+100,m_pos.y+24,BLACK,"%d",m_iCount);
			s_text.popShadowStatus();
			s_text.put(m_pos.x+6,m_pos.y+7,WHITE,m_strTitle);
			m_sbCount.draw();
			m_bmButton.draw();
			PutItem(&m_item,m_pos.x+7,m_pos.y+26,FALSE,FALSE);
			break;
		}

		case	eICWR_RETURN_TRADE_GOLD						:
		case	eICWR_RETURN_DROP_GOLD						:
		case	eICWR_RETURN_STORE_GOLD_TO_THE_BANK			:
		case	eICWR_RETURN_WITHDRAW_GOLD_FROM_GUILD_INVENTORY	:
		case	eICWR_RETURN_STORE_GOLD_TO_GUILD_INVENTORY	:
		case	eICWR_RETURN_WITHDRAW_GOLD_FROM_THE_BANK	:
		{
			g_sprInterface.Put(m_pos.x,m_pos.y,eITEM_DROP_FRAME);
			s_text.pushShadowStatus(FALSE);
			m_inputGold.draw(BLACK);
			s_text.popShadowStatus();
			s_text.put(m_pos.x+6,m_pos.y+7,WHITE,m_strTitle);
			m_sbCount.draw();
			PutItem(&m_item,m_pos.x+7,m_pos.y+26,FALSE,FALSE);
			break;
		}

		case	eICWR_RETURN_PITCHMAN_SHOP_SELL_ITEM_COUNT	:
		{
			drawPitchmanShopSellItemCount();
			break;
		}
	}

	m_bmButton.draw();
}

void
CGamePlay::EnableChattingBar(BOOL _bIsEnable,int _iTextLength)
{
	if (g_gwMakeGuild.isOpened())
		return;

	if (g_rectEditControlList.size() == 0)
	{
		int	iInputBarPos=492,iTextCorrectPos=0;

		if	(s_bIsHideInterface)
			iInputBarPos=576,iTextCorrectPos=84;

		int	iCorrectBI_X	=	g_iScreenWidth-800;
		int	iCorrectBI_Y	=	g_iScreenHeight-600;

		s_inputChatting.set(79+14+26+iCorrectBI_X,iInputBarPos+6+iCorrectBI_Y-1,
							0,
							79+14+26+iCorrectBI_X,iInputBarPos+6+iCorrectBI_Y-1,
							574,13,dCHAT_LENGTH-dNAME_LENGTH-2,"");
	}

	if (_iTextLength	==	-1)
		_iTextLength	=	dCHAT_LENGTH-dNAME_LENGTH-2;

	s_inputChatting.enable(_bIsEnable);
	s_inputChatting.setLimit(0,_iTextLength);
	s_inputChatting.setColor(0,WHITE);
}

void
CGamePlayInterfaceItemCount::drawPitchmanShopSellItemCount()		
{

	s_ttCommon.drawBox(&m_rect,0,32);
	

	
	int	iY	=	m_pos.y+28;
	int iAddHeight = 0;
	cSTRING strItemName = m_item.getName();
	
	int iNamePixelSize = s_text.getPixelSize(strItemName.String);
	
	if(iNamePixelSize> m_rect.x2 - m_rect.x1)
	{
		int iNameLen	= strlen(strItemName.String);
		int iNameCenter = iNameLen-1;
		
		for(int i=0;i<iNameLen;++i)
		{
			if(strItemName.String[i]	== ' ')
			{
				if(abs(iNameLen/2 - i) <= abs(iNameLen/2 - iNameCenter))
				{
					iNameCenter = i;
				}
			}	
		}
		
		char strFirstName[256] = "";
		char strLastName[256] = "";
		
		strncpy(strFirstName,strItemName.String,iNameCenter);
		BOOL chk = FALSE;
		
		for(int j=strlen(strFirstName)-3; j>=0; --j)
		{
			if(strnicmp("<n>", &strFirstName[j],3) == 0) 
				break;
			
			if(strnicmp("<c:", &strFirstName[j],3) == 0) 
			{
				chk = TRUE;
				for(int k=j;k<strlen(strFirstName);++k)
				{
					
					strLastName[k-j] = strFirstName[k];
					if(strFirstName[k] == '>')
						break;
				}
				break;
			}				
		}
		strcat(strLastName,&strItemName.String[iNameCenter+1]);
		
		s_text.put(m_pos.x+6,m_pos.y+7,WHITE,strFirstName);
		s_text.put(m_pos.x+6,m_pos.y+20,WHITE,strLastName);
		
		iAddHeight = 15;
	}
	else
	{
		s_text.put(m_pos.x+6,m_pos.y+7,WHITE,strItemName.String);
	}
	//			cDRAW::FillHB(0xffff,m_rect.x1+6,m_rect.y1+6,m_rect.x2-6,m_rect.y1+22);
	//			s_text.put(m_pos.x+12 ,m_pos.y+4 ,WHITE,dMSG_ITEM_COUNT_FOR_SELL);
	g_sprInterface.Put(m_pos.x+7,m_pos.y+26+iAddHeight,eIWI_ITEM_SLOT_FRAME);
	PutItem(&m_item,m_pos.x+7,m_pos.y+26+iAddHeight,TRUE,FALSE);
	
	int iPixelSize = s_text.getPixelSize(m_strTitle);
	s_text.put(m_pos.x+198-iPixelSize,m_pos.y+65 + iAddHeight,WHITE,m_strTitle);
	
	s_text.putRF(m_pos.x+7+38+m_iInputPosForItemPrice+27,iY+19 + iAddHeight,WHITE,dMSG_SELL_PRICE);
	g_sprInterface.Put(m_pos.x+m_iInputPosForItemPrice+48+27,iY+17 + iAddHeight,eITEM_PRICE_FRAME);
	m_inputGold.draw(BLACK);
	
	int	iWidth		=	g_sprInterface.getSpriteWidth(eITEM_PRICE_FRAME);

	m_bmRadioButton.draw();
	
	
}