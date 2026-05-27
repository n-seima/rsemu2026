#include "CGuildMarkComposer.h"
#include "CMessage.h"
#include "CHero.h"
#include "CMap.h"

//
//	Guild Mark Composer
enum
{
	eGMC_OK,
	eGMC_CANCEL,

	eGMC_ICON_TAB1,
	eGMC_ICON_TAB2,
	eGMC_ICON_TAB3,
	eGMC_ICON_TAB4,
	eGMC_ICON_TAB5,
	eGMC_ICON_TAB6,
	eGMC_ICON_TAB7,
	eGMC_ICON_TAB8,
};

//
//	Guild Mark Composer Work Tab
enum
{
	eGMCWT_SELECT_MARK_ICON,
	eGMCWT_SELECT_MARK_COLOR_PATTERN,
	eGMCWT_SELECT_MARK_BASE_SHAPE,
	eGMCWT_SELECT_MARK_BASE_COLOR_PARTITION,
	eGMCWT_SELECT_MARK_BASE_COLOR_PATTERN,
	eGMCWT_SELECT_MARK_BASE_OUTLINE_COLOR_PATTERN,
};

CGuildMarkComposer	g_gwGuildMarkComposer;

CGuildMarkComposer::CGuildMarkComposer()
{
	m_iSelectWorkTab				=	0;
	m_iSelectIconShapePart			=	0;

	m_iFocusIcon					=	0xffff;
	m_iFocusIconColorPattern		=	0xffff;
	m_iFocusBaseShape				=	0xffff;
	m_iFocusBaseColorPartition		=	0xffff;
	m_iFocusBaseColorPattern		=	0xffff;
	m_iFocusBaseOutLineColorPattern	=	0xffff;
}

//
//	열렸냐?
BOOL
CGuildMarkComposer::isOpened()
{
	if (s_iPopupInterface	==	ePIW_GUILD_MARK_COMPOSER)
		return	TRUE;

	return	FALSE;
}


void
CGuildMarkComposer::open()
{
	if (s_iPopupInterface	==	ePIW_GUILD_MARK_COMPOSER)
	{
		ClosePopupWindow();
		return;
	}

	m_guildMarkInfo.reset();
	m_guildMarkInfo.m_bBaseColor1			=	0;
	m_guildMarkInfo.m_bBaseColor2			=	0;
	m_guildMarkInfo.m_bBaseLineColor		=	0;
	m_guildMarkInfo.m_bIconColor			=	0;
	m_guildMarkInfo.m_bBaseColorPartition	=	0;

	g_hero.m_bf1IsWaitGuildCommandResult	=	FALSE;

	ClosePopupWindow();

	s_iPopupInterface			=	ePIW_GUILD_MARK_COMPOSER;
	m_iSelectWorkTab			=	eGMCWT_SELECT_MARK_ICON;
	m_iSelectIconShapePart		=	0;

	s_rectInterfaceSize.x1 = g_iScreenWidth-(g_iScreenWidth/3+12);
	s_rectInterfaceSize.x2 = g_iScreenWidth;

	{
		s_bmButton.reset();

		int	iBarWidth	=	g_sprInterface.getSpriteWidth(eBMBI_NORMAL);
		int	iBarHeight	=	g_sprInterface.getSpriteHeight(eBMBI_NORMAL);
		int	iX			=	g_iScreenWidth/3+12+258+256/2-iBarWidth/2-g_msgBox.getFrame()->m_iLeftBorderWidth,iY=63;

		cRECT	rect;

		rect.set(g_iScreenWidth/3+12-5,6,g_iScreenWidth-6,g_iFieldScreenHeight-30);

		s_bmButton.addImageBar(	dMSG_DECISION	,eGMC_OK	,
								rect.x1+rect.getWidth()/2-iBarWidth/2,
								rect.y2-23,eBMBI_NORMAL,eBMBI_ACTIVE,eBMBI_PRESSED,eBMBI_DISABLE);
//		s_bmButton.addImageBar(	dMSG_CANCEL		,eGMC_CANCEL,
//								rect.x1+236+4,
//								rect.y2-23,eBMBI_NORMAL,eBMBI_ACTIVE,eBMBI_PRESSED,eBMBI_DISABLE);

		s_bmButton.setAllBarTextColor(0,FALSE);
		s_bmButton.setClicktAllBarMargin(1,1);

		m_bmButton.init(&g_sprInterface,20,dBAR_RADIO);
		s_bmButton.setClicktAllBarMargin(1,1);
	}

	changeTab(m_iSelectWorkTab);

}

//
//	업데이트
void
CGuildMarkComposer::update()
{
	if (s_iPopupInterface	!=	ePIW_GUILD_MARK_COMPOSER)	return;

	DWORD	dwSelectMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);


	switch(dwSelectMenu)
	{
		case	eGMC_OK		:
		{
			if (g_hero.m_bf1IsWaitGuildCommandResult)
				break;

			if (m_guildMarkInfo.m_wIcon == 0xffff && m_guildMarkInfo.m_bBaseShape == 0xff)
			{
				g_msgBox.cPopup("",dMSG_GUILD_MARK_BASIC_RULE,dMSG_OK);
				break;
			}

			if (m_guildMarkInfo.m_wIcon != 0xffff)
				if (g_guildMarkPackManager.isEnableSymbol(m_guildMarkInfo.m_wIcon,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
				{
					g_msgBox.cPopup("",dMSG_GUILD_MARK_SELECT_CAN_NOT_SELECT_ABLE_SYMBOL,dMSG_OK);
					break;
				}

			if (m_guildMarkInfo.m_bBaseShape != 0xff)
				if (g_guildMarkPackManager.isEnableBase(m_guildMarkInfo.m_bBaseShape,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
				{
					g_msgBox.cPopup("",dMSG_GUILD_MARK_SELECT_CAN_NOT_SELECT_ABLE_BASE_SHAPE,dMSG_OK);
					break;
				}

			if (g_guildMarkPackManager.isEnableDivision(m_guildMarkInfo.m_bBaseColorPartition,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
			{
				g_msgBox.cPopup("",dMSG_GUILD_MARK_SELECT_CAN_NOT_SELECT_ABLE_BASE_PARTITION,dMSG_OK);
				break;
			}

			g_msgBox.cPopup(dMSG_MAKE_GUILD_MARK,dMSG_MAKE_GUILD_BY_SELECT_CONTENTS_CONTINUE,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_MAKE_GUILD_MARK);
			break;
		}

//		case	eGMC_CANCEL	:
//		{
//			close();
//			break;
//		}
	}

	if (s_bmButton.isPressed())
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_GUILD_MARK_COMPOSER);
		return;
	}
	else	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_GUILD_MARK_COMPOSER);

	m_iFocusWorkTab		=	getFocusWorkTab();

	if (m_iFocusWorkTab	!= 0xffff)
	{
		if (s_isTouchLeftButton)
		{
			if (m_iSelectWorkTab!=	m_iFocusWorkTab)
				changeTab(m_iFocusWorkTab);
		}
	}


	dwSelectMenu	=	m_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwSelectMenu)
	{
		case	eGMC_ICON_TAB1	:
		case	eGMC_ICON_TAB2	:
		case	eGMC_ICON_TAB3	:
		case	eGMC_ICON_TAB4	:
		case	eGMC_ICON_TAB5	:
		case	eGMC_ICON_TAB6	:
		case	eGMC_ICON_TAB7	:
		case	eGMC_ICON_TAB8	:
			m_iSelectIconShapePart	=	dwSelectMenu-eGMC_ICON_TAB1;
			break;
	}

	if (s_bmButton.isPressed())
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_GUILD_MARK_COMPOSER);
		return;
	}
	else	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_GUILD_MARK_COMPOSER);

	switch(m_iSelectWorkTab)
	{
		case	eGMCWT_SELECT_MARK_ICON					:
			m_iFocusIcon					=	getFocusIcon();
			if(s_isTouchLeftButton && m_iFocusIcon != 0xfffe)
				m_guildMarkInfo.m_wIcon		=	m_iFocusIcon;
			break;

		case	eGMCWT_SELECT_MARK_COLOR_PATTERN		:
			m_iFocusIconColorPattern		=	getFocusIconColorPattern();
			if(s_isTouchLeftButton && m_iFocusIconColorPattern != 0xffff)
				m_guildMarkInfo.m_bIconColor=	m_iFocusIconColorPattern;
			break;

		case	eGMCWT_SELECT_MARK_BASE_SHAPE			:
			m_iFocusBaseShape				=	getFocusBaseShape();
			if(s_isTouchLeftButton && m_iFocusBaseShape!= 0xfffe)
				m_guildMarkInfo.m_bBaseShape=	m_iFocusBaseShape;
			break;

		case	eGMCWT_SELECT_MARK_BASE_COLOR_PARTITION	:
			m_iFocusBaseColorPartition		=	getFocusBaseColorPartition();
			if(s_isTouchLeftButton && m_iFocusBaseColorPartition != 0xffff)
				m_guildMarkInfo.m_bBaseColorPartition=	m_iFocusBaseColorPartition;
			break;

		case	eGMCWT_SELECT_MARK_BASE_COLOR_PATTERN	:
			m_iFocusBaseColorPattern		=	getFocusBaseColorPattern();
			if(s_isLeftButtonPressed && m_iFocusBaseColorPattern != 0xffff)
			{
				if (m_iFocusBaseColorPattern	<	1000)
					m_guildMarkInfo.m_bBaseColor1=	m_iFocusBaseColorPattern;
				else
					m_guildMarkInfo.m_bBaseColor2=	m_iFocusBaseColorPattern-1000;
			}
			break;

		case	eGMCWT_SELECT_MARK_BASE_OUTLINE_COLOR_PATTERN			:
			m_iFocusBaseOutLineColorPattern	=	getFocusBaseOutLineColorPattern();
			if(s_isTouchLeftButton && m_iFocusBaseOutLineColorPattern != 0xffff)
				m_guildMarkInfo.m_bBaseLineColor	=	m_iFocusBaseOutLineColorPattern;
			break;
	}
}

//창 닫기
void
CGuildMarkComposer::close()
{
	if (s_iPopupInterface	!=	ePIW_GUILD_MARK_COMPOSER)	
		return;

	cINPDEV::ReleaseExclusive();

	s_bmButton.reset();

	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
}

//그리기
void
CGuildMarkComposer::draw()
{
	if (s_iPopupInterface	!=	ePIW_GUILD_MARK_COMPOSER)	return;

	int		iX=g_iScreenWidth/3+12,iY=0,i;
	cRECT	rect;

	rect.set(g_iScreenWidth/3+12,6,g_iScreenWidth-6,g_iFieldScreenHeight-30);
	g_msgBox.drawFrame(&rect,FALSE,dMSG_GUILD_MARK_COMPOSE);

	iX	-=	5;

	s_textBold14.setShadowMargin(2);

	s_textBold14.cPut(iX+231/2,iY+72,WHITE,dMSG_MIXED_GUILD_MARK_SHAPE);

	for (i=0;i<6;i++)
	{
		if (m_iSelectWorkTab==	i)
			cDRAW::Fill(RGB24To16(198,157,33),iX+7,iY+219+39*i,iX+232,iY+219+38+39*i);
		else
		if (m_iFocusWorkTab	==	i)
			cDRAW::Fill(RGB24To16(100,80,10),iX+7,iY+219+39*i,iX+232,iY+219+38+39*i);

		cDRAW::XLine(RGB24To16(71,35,9),iX+5,iX+235,iY+218+39*i);

		g_sprInterface.Put(iX+19,iY+248+39*i,eGBSW_1+i);

		if (i==0)	s_textBold14.put(iX+30,iY+232,WHITE,dMSG_GUILD_MARK_ICON_SHAPE);
		if (i==1)	s_textBold14.put(iX+30,iY+232+39*i,WHITE,dMSG_GUILD_MARK_ICON_COLOR_PATTERN);
		if (i==2)	s_textBold14.put(iX+30,iY+232+39*i,WHITE,dMSG_GUILD_MARK_BASE_SHAPE);
		if (i==3)	s_textBold14.put(iX+30,iY+232+39*i,WHITE,dMSG_GUILD_MARK_BASE_COLOR_PARTITION);
		if (i==4)	s_textBold14.put(iX+30,iY+232+39*i,WHITE,dMSG_GUILD_MARK_BASE_COLOR_PARTTERN);
		if (i==5)	s_textBold14.put(iX+30,iY+232+39*i,WHITE,dMSG_GUILD_MARK_OUTLINE_COLOR_PARTTERN);
	}

	cDRAW::YLine(RGB24To16(159,92,66),iY+39,iY+452,iX+236,2);

	switch(m_iSelectWorkTab)
	{
		case	eGMCWT_SELECT_MARK_ICON					:
			drawMarkIconList();
			break;

		case	eGMCWT_SELECT_MARK_COLOR_PATTERN		:
			drawMarkIconColorPatternList();
			break;

		case	eGMCWT_SELECT_MARK_BASE_SHAPE			:
			drawMarkBaseShapeList();
			break;

		case	eGMCWT_SELECT_MARK_BASE_COLOR_PARTITION	:
			drawMarkBaseShapeColorPartitionList();
			break;

		case	eGMCWT_SELECT_MARK_BASE_COLOR_PATTERN	:
			drawMarkBaseShapeColorPatternList();
			break;

		case	eGMCWT_SELECT_MARK_BASE_OUTLINE_COLOR_PATTERN			:
			drawMarkBaseShapeOutLineColorPatternList();
			break;
	}

	s_bmButton.draw();
	m_bmButton.draw();

	WORD	aImageBuffer[42*42+2];
	CPos	pos;

	pos.x	=	iX+114;
	pos.y	=	iY+155;

	g_guildMark.putMark(pos.x,pos.y,
						m_guildMarkInfo.m_wIcon,m_guildMarkInfo.m_bIconColor,
						m_guildMarkInfo.m_bBaseShape,m_guildMarkInfo.m_bBaseColorPartition,m_guildMarkInfo.m_bBaseColor1,m_guildMarkInfo.m_bBaseColor2,m_guildMarkInfo.m_bBaseLineColor,
						0,0);

	pos.x	-=	20;
	pos.y	-=	20;

	cDRAW::GetImage(pos.x,pos.y,40,40,aImageBuffer);
	cDRAW::PutImageScale(pos.x-40,pos.y-50,aImageBuffer,300);

	cDRAW::PutImage(iX+4,pos.y+40,aImageBuffer);
}

//	아이콘 리스트
void
CGuildMarkComposer::drawMarkIconList()
{
	int	iX=g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	int	iSelectIconColorPattern	=	m_guildMarkInfo.m_bIconColor;

	if (iSelectIconColorPattern	>=	dGUILD_MARK_COLOR_PATTERN_COUNT)
		iSelectIconColorPattern	=	0;

	WORD	*lpPalette	=	g_guildMarkPalette.getGradiationPalette(iSelectIconColorPattern);
	int		iSelectKind	=	m_guildMarkInfo.m_wIcon/1000;
	int		iSelectIndex=	m_guildMarkInfo.m_wIcon%1000;
	CPos	pos;

	for (iLine=0;iLine<10;iLine++)
		for (iColumn=0;iColumn<10;iColumn++)
		{
			int	iIndex	=	iColumn+iLine*10;

			pos.x		=	iX+246+8+iColumn*28;
			pos.y		=	iY+96+iLine*34;

			if	(m_iSelectIconShapePart	== iSelectKind && iColumn+iLine*10 == iSelectIndex)
			{
				cDRAW::Fill(RGB24To16(198,157,33),pos.x-13,pos.y-15,pos.x+13,pos.y+15);
				cDRAW::Box(RGB24To16(100,80,10),pos.x-13,pos.y-15,pos.x+13,pos.y+15);
			}

			g_sprGuildIconShape[m_iSelectIconShapePart].setPalette(lpPalette);

			if	(m_iSelectIconShapePart	>=	6)
				g_sprGuildIconShape[m_iSelectIconShapePart].restorePalette();

			g_sprGuildIconShape[m_iSelectIconShapePart].Put(pos.x,pos.y,iIndex);

			if (g_guildMarkPackManager.isEnableSymbol(iIndex+m_iSelectIconShapePart*1000,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
			{
				g_sprInterface.Put(pos.x-13,pos.y-16,eGMCW_SYMBOL_BLOCK,100,100,dPUT_HALF_BLENDING);
			}
		}

	pos.x	=	iX+245;
	pos.y	=	iY+424;

	if (m_guildMarkInfo.m_wIcon	!=	0xffff)
		if (g_guildMarkPackManager.isEnableSymbol(m_guildMarkInfo.m_wIcon,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
		{
			char	*lpstrPartString	=	NULL;
			int		iPart				=	g_guildMarkPackManager.getSymbolPart(m_guildMarkInfo.m_wIcon);

			switch(iPart)
			{
				case	eGMIP_EXPAND	:
					lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_EXPAND_PACK;
					break;
				case	eGMIP_SPECIAL	:
					lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_SPECIAL_PACK;
					break;
				case	eGMIP_LUXURY	:
					lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_LUXURY_PACK;
					break;
			}

			switch(iSelectKind)
			{
				case	0	:	//	한글
					if (lpstrPartString)
						s_text.put(pos.x,pos.y,274,100,0,WHITE," %s %s %s",dMSG_GMC_RQUIRE_GUILD_MARK_HANGUL_PACK,dMSG_GMC_OR,lpstrPartString);
					else
						s_text.put(pos.x,pos.y,274,100,0,WHITE,dMSG_GMC_RQUIRE_GUILD_MARK_HANGUL_PACK);
					break;

				case	1	:	//	영어
					if (lpstrPartString)
						s_text.put(pos.x,pos.y,274,100,0,WHITE," %s %s %s",dMSG_GMC_RQUIRE_GUILD_MARK_ENGLISH_PACK,dMSG_GMC_OR,lpstrPartString);
					else
						s_text.put(pos.x,pos.y,274,100,0,WHITE,dMSG_GMC_RQUIRE_GUILD_MARK_ENGLISH_PACK);
					break;

				case	2	:	//	한문
					if (lpstrPartString)
						s_text.put(pos.x,pos.y,274,100,0,WHITE," %s %s %s",dMSG_GMC_RQUIRE_GUILD_MARK_HANMUN_PACK,dMSG_GMC_OR,lpstrPartString);
					else
						s_text.put(pos.x,pos.y,274,100,0,WHITE,dMSG_GMC_RQUIRE_GUILD_MARK_HANMUN_PACK);
					break;

				case	3	:	//	아이콘
					if (lpstrPartString)
						s_text.put(pos.x,pos.y,274,100,0,WHITE," %s %s %s",dMSG_GMC_RQUIRE_GUILD_MARK_ITEM_PACK,dMSG_GMC_OR,lpstrPartString);
					else
						s_text.put(pos.x,pos.y,274,100,0,WHITE,dMSG_GMC_RQUIRE_GUILD_MARK_ITEM_PACK);
					break;

				case	4	:	//	동물
					if (lpstrPartString)
						s_text.put(pos.x,pos.y,274,100,0,WHITE," %s %s %s",dMSG_GMC_RQUIRE_GUILD_MARK_ANIMAL_PACK,dMSG_GMC_OR,lpstrPartString);
					else
						s_text.put(pos.x,pos.y,274,100,0,WHITE,dMSG_GMC_RQUIRE_GUILD_MARK_ANIMAL_PACK);
					break;
					
				case	5	:
					break;
				case	6	:
					break;
				case	7	:
					break;
			}
		}
}

//	아이콘 컬러 패턴 리스트
void
CGuildMarkComposer::drawMarkIconColorPatternList()
{
	int		iX					=	g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	int		iPaletteSpearIndex	=	g_sprGuildIconOutLine.m_iCount-1;
	int		iGab				=	((s_iFrameCounter/6)%5);
	int		iSelectIconKind		=	m_guildMarkInfo.m_wIcon/1000;
	int		iSelectIconIndex	=	m_guildMarkInfo.m_wIcon%1000;
	CPos	pos;

	if (m_guildMarkInfo.m_wIcon	==	0xffff)
		iSelectIconKind		=	0xffff;

	for (iLine=0;iLine<10;iLine++)
		for (iColumn=0;iColumn<3;iColumn++)
		{
			pos.x	=	iX+277+8+iColumn*85;
			pos.y	=	iY+65+iLine*40;

			WORD	*lpPalette	=	g_guildMarkPalette.getGradiationPalette(iColumn+iLine*3);

			g_sprGuildIconOutLine.setPalette(lpPalette);
			g_sprGuildIconOutLine.Put(pos.x,pos.y,iPaletteSpearIndex);

			if (iSelectIconKind	!=	0xffff)
			{
				g_sprGuildIconShape[iSelectIconKind].setPalette(lpPalette);

				if	(iSelectIconKind	>=	6)
					g_sprGuildIconShape[iSelectIconKind].restorePalette();

				g_sprGuildIconShape[iSelectIconKind].Put(pos.x+30,pos.y,iSelectIconIndex);
			}

			if (m_guildMarkInfo.m_bIconColor	==	iColumn+iLine*3)
				g_sprInterface.Put(pos.x-11-iGab,pos.y-8,eQW_LEFT_HAND);
		}
}

//	베이스 모양 리스트
void
CGuildMarkComposer::drawMarkBaseShapeList()
{
	int		iX			=	g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	WORD	*lpPalette	=	g_guildMarkPalette.getGradiationPalette(1);
	int		iGab		=	((s_iFrameCounter/6)%5);
	CPos	pos;

	for (iLine=0;iLine<5;iLine++)
		for (iColumn=0;iColumn<2;iColumn++)
		{
			int	iIndex	=	iColumn+iLine*2;
			pos.x		=	iX+314+iColumn*96;
			pos.y		=	iY+66+iLine*75;

			g_guildMark.putBase(pos.x+10,pos.y+19,iIndex,m_guildMarkInfo.m_bBaseColorPartition,m_guildMarkInfo.m_bBaseColor1,m_guildMarkInfo.m_bBaseColor2);
			g_guildMark.putBaseOutLine(pos.x+10,pos.y+19,iIndex,m_guildMarkInfo.m_bBaseLineColor);

			if (g_guildMarkPackManager.isEnableBase(iIndex,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
			{
				g_sprInterface.Put(pos.x-3,pos.y+1,eGMCW_BASE_BLOCK,100,100,dPUT_HALF_BLENDING);
			}

			if (m_guildMarkInfo.m_bBaseShape	==	iColumn+iLine*2)
				g_sprInterface.Put(pos.x-15-iGab+5,pos.y+10,eQW_LEFT_HAND);
		}

	pos.x	=	iX+245;
	pos.y	=	iY+424;

	if (m_guildMarkInfo.m_bBaseShape	!=	0xff)
		if (g_guildMarkPackManager.isEnableBase(m_guildMarkInfo.m_bBaseShape,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
		{
			char	*lpstrPartString	=	NULL;
			int		iPart				=	g_guildMarkPackManager.m_base.getPart(m_guildMarkInfo.m_bBaseShape);

			switch(iPart)
			{
				case	eGMIP_EXPAND	:
					lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_EXPAND_PACK;
					break;
				case	eGMIP_SPECIAL	:
					lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_SPECIAL_PACK;
					break;
				case	eGMIP_LUXURY	:
					lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_LUXURY_PACK;
					break;
			}

			if (lpstrPartString)
				s_text.put(pos.x,pos.y,274,100,0,WHITE,lpstrPartString);
		}

}

//	베이스 모양 컬러 분할형태 리스트
void
CGuildMarkComposer::drawMarkBaseShapeColorPartitionList()
{
	int		iX			=	g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	WORD	*lpPalette	=	g_guildMarkPalette.getGradiationPalette(1);
	int		iGab		=	((s_iFrameCounter/6)%5);
	CPos	pos;

	g_smiGuildIconBaseColorPartition.restorePalette();

	for (iLine=0;iLine<8;iLine++)
		for (iColumn=0;iColumn<5;iColumn++)
		{
			int	iIndex	=	iColumn+iLine*5;

			pos.x		=	iX+270+iColumn*55;
			pos.y		=	iY+56+iLine*46;

			g_smiGuildIconBaseColorPartition.put(iIndex,pos.x,pos.y);
			
			if (g_guildMarkPackManager.isEnableDivision(iIndex,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
			{
				g_sprInterface.Put(pos.x-3,pos.y-2,eGMCW_DIVISION_BLOCK,100,100,dPUT_HALF_BLENDING);
			}

			if (m_guildMarkInfo.m_bBaseColorPartition	==	iColumn+iLine*5)
				g_sprInterface.Put(pos.x-iGab-3,pos.y,eQW_LEFT_HAND);
		}

	pos.x	=	iX+245;
	pos.y	=	iY+424;

	if (g_guildMarkPackManager.isEnableDivision(m_guildMarkInfo.m_bBaseColorPartition,&g_guild.m_guildMarkInfo.m_ownedMarkItem) == FALSE)
	{
		char	*lpstrPartString	=	NULL;
		int		iPart				=	g_guildMarkPackManager.m_division.getPart(m_guildMarkInfo.m_bBaseColorPartition);

		switch(iPart)
		{
			case	eGMIP_EXPAND	:
				lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_EXPAND_PACK;
				break;
			case	eGMIP_SPECIAL	:
				lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_SPECIAL_PACK;
				break;
			case	eGMIP_LUXURY	:
				lpstrPartString	=	dMSG_GMC_RQUIRE_GUILD_MARK_LUXURY_PACK;
				break;
		}

		if (lpstrPartString)
			s_text.put(pos.x,pos.y,274,100,0,WHITE,lpstrPartString);
	}
}

//	베이스 모양 컬러 패턴 리스트
void
CGuildMarkComposer::drawMarkBaseShapeColorPatternList()
{
	int		iX		=	g_iScreenWidth/3+12-5,iY=0,iColumn,iLine,iPattern;
	int		iGab	=	((s_iFrameCounter/6)%5);
	CPos	pos;

	s_text.cPut(iX+367,iY+48,WHITE,"<b>Color 1<b>");
	s_text.cPut(iX+367,iY+254,WHITE,"<b>Color 2<b>");

	for (iPattern=0;iPattern<2;iPattern++)
		for (iLine=0;iLine<16;iLine++)
			for (iColumn=0;iColumn<16;iColumn++)
			{
				pos.x	=	iX+252+iColumn*16;
				pos.y	=	iY+64+iLine*11+iPattern*206;

				cDRAW::Fill(g_guildMarkPalette.m_awBaseColor[iLine*16+iColumn],pos.x,pos.y,pos.x+15,pos.y+10);

				if ((iPattern == 0 && m_guildMarkInfo.m_bBaseColor1 == iLine*16+iColumn) || 
					(iPattern == 1 && m_guildMarkInfo.m_bBaseColor2 == iLine*16+iColumn))
				{
					int	iValue		=	s_iFrameCounter*8%512;

					if (iValue	>=	255)
						iValue	=	255-(iValue%256);

					cDRAW::Box(RGB24To16(iValue,iValue,iValue),pos.x,pos.y,pos.x+15,pos.y+10,2);
				}
			}
}

//	베이스 모양 외곽선 컬러 패턴 리스트
void
CGuildMarkComposer::drawMarkBaseShapeOutLineColorPatternList()
{
	int		iX=g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	int		iPaletteSpearIndex	=	g_sprGuildIconOutLine.m_iCount-1;
	int		iGab	=	((s_iFrameCounter/6)%5);
	CPos	pos;

	for (iLine=0;iLine<10;iLine++)
		for (iColumn=0;iColumn<3;iColumn++)
		{
			pos.x	=	iX+277+8+iColumn*85;
			pos.y	=	iY+65+iLine*40;

			WORD	*lpPalette	=	g_guildMarkPalette.getGradiationPalette(iColumn+iLine*3);

			g_sprGuildIconOutLine.setPalette(lpPalette);
			g_sprGuildIconOutLine.Put	(pos.x		,pos.y,iPaletteSpearIndex);
			g_guildMark.putBaseOutLine	(pos.x+30	,pos.y,m_guildMarkInfo.m_bBaseShape,iColumn+iLine*3);

			if (m_guildMarkInfo.m_bBaseLineColor == iColumn+iLine*3)
				g_sprInterface.Put(pos.x-11-iGab,pos.y-8,eQW_LEFT_HAND);
		}
}

//	현재 포커스가 있는 워크 탭
int
CGuildMarkComposer::getFocusWorkTab()
{
	int		iX=g_iScreenWidth/3+12,iY=0,i;
	cRECT	rect;

	rect.set(g_iScreenWidth/3+12,6,g_iScreenWidth-6,g_iFieldScreenHeight-30);
	iX	-=	5;

	for (i=0;i<6;i++)
	{
		rect.set(iX+7,iY+219+i*39,iX+232,iY+219+38+i*39);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;
	}

	return	0xffff;
}

int
CGuildMarkComposer::getFocusIconShapeTab()
{
	return	0xffff;
}

//	포커스 가 있는 아이콘
int
CGuildMarkComposer::getFocusIcon()
{
	int		iX=g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	cRECT	rect;
	CPos	pos;

	rect.set(iX+238,iY+76,iX+520,iY+454);

	if (!rect.isIn(s_posMouse.x,s_posMouse.y))
		return	0xfffe;

	for (iLine=0;iLine<10;iLine++)
		for (iColumn=0;iColumn<10;iColumn++)
		{
			pos.x	=	iX+246+8+iColumn*28;
			pos.y	=	iY+96+iLine*34;

			rect.set(pos.x-15,pos.y-15,pos.x+15,pos.y+15);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
				return	m_iSelectIconShapePart*1000+iColumn+iLine*10;
		}

	return	0xffff;
}

int
CGuildMarkComposer::getFocusIconColorPattern()
{
	int		iX=g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	cRECT	rect;
	CPos	pos;

	for (iLine=0;iLine<10;iLine++)
		for (iColumn=0;iColumn<3;iColumn++)
		{
			pos.x	=	iX+277+8+iColumn*85;
			pos.y	=	iY+65+iLine*40;

			rect.set(pos.x-15,pos.y-15,pos.x+40,pos.y+15);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
				return	iColumn+iLine*3;
		}

	return	0xffff;
}

int
CGuildMarkComposer::getFocusBaseShape()
{
	int		iX		=	g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	cRECT	rect;
	CPos	pos;

	rect.set(iX+238,iY+76,iX+520,iY+454);

	if (!rect.isIn(s_posMouse.x,s_posMouse.y))
		return	0xfffe;

	for (iLine=0;iLine<5;iLine++)
		for (iColumn=0;iColumn<2;iColumn++)
		{
			pos.x	=	iX+314+iColumn*96;
			pos.y	=	iY+66+iLine*75;

			rect.set(pos.x-4,pos.y-10,pos.x+29,pos.y+45);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
				return	iColumn+iLine*2;
		}

	return	0xffff;
}

int
CGuildMarkComposer::getFocusBaseColorPartition()
{
	int		iX	=	g_iScreenWidth/3+12-5,iY=0,iColumn,iLine;
	cRECT	rect;
	CPos	pos;

	for (iLine=0;iLine<8;iLine++)
		for (iColumn=0;iColumn<5;iColumn++)
		{
			pos.x	=	iX+270+iColumn*55;
			pos.y	=	iY+56+iLine*46;

			rect.set(pos.x-4,pos.y-4,pos.x+25+4,pos.y+38+4);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
				return	iColumn+iLine*5;
		}

	return	0xffff;
}

int
CGuildMarkComposer::getFocusBaseColorPattern()
{
	int		iX	=	g_iScreenWidth/3+12-5,iY=0,iColumn,iLine,iPattern;
	cRECT	rect;
	CPos	pos;

	for (iPattern=0;iPattern<2;iPattern++)
		for (iLine=0;iLine<16;iLine++)
			for (iColumn=0;iColumn<16;iColumn++)
			{
				pos.x	=	iX+252+iColumn*16;
				pos.y	=	iY+64+iLine*11+iPattern*206;

				rect.set(pos.x,pos.y,pos.x+15,pos.y+10);

				if (rect.isIn(s_posMouse.x,s_posMouse.y))
					return	iLine*16+iColumn+iPattern*1000;

				cDRAW::Fill(g_guildMarkPalette.m_awBaseColor[iLine*16+iColumn],pos.x,pos.y,pos.x+15,pos.y+10);
			}

	return	0xffff;
}

int
CGuildMarkComposer::getFocusBaseOutLineColorPattern()
{
	return	getFocusIconColorPattern();
}

void
CGuildMarkComposer::changeTab(int _iWorkTab)
{
	m_bmButton.reset();

	m_iFocusIcon					=	0xffff;
	m_iFocusIconColorPattern		=	0xffff;
	m_iFocusBaseShape				=	0xffff;
	m_iFocusBaseColorPartition		=	0xffff;
	m_iFocusBaseColorPattern		=	0xffff;
	m_iFocusBaseOutLineColorPattern	=	0xffff;

	if (_iWorkTab	==	eGMCWT_SELECT_MARK_ICON)
	{
		int	iX		=	g_iScreenWidth/3+12,iY	=	0;

		m_bmButton.addImageBar(	dMSG_HANGUL_INITIAL	,eGMC_ICON_TAB1,
								iX+239	,iY+40		,
								eGBSW_NORMAL_TIME_BAR,eGBSW_ACTIVE_TIME_BAR,eGBSW_SELECT_TIME_BAR,eGBSW_SELECT_TIME_BAR);
		m_bmButton.addImageBar(	dMSG_ENGLISH_INITIAL,eGMC_ICON_TAB2,
								iX+308	,iY+40		,
								eGBSW_NORMAL_TIME_BAR,eGBSW_ACTIVE_TIME_BAR,eGBSW_SELECT_TIME_BAR,eGBSW_SELECT_TIME_BAR);
		m_bmButton.addImageBar(	dMSG_HANJA_INITIAL	,eGMC_ICON_TAB3,
								iX+377	,iY+40		,
								eGBSW_NORMAL_TIME_BAR,eGBSW_ACTIVE_TIME_BAR,eGBSW_SELECT_TIME_BAR,eGBSW_SELECT_TIME_BAR);
		m_bmButton.addImageBar(	dMSG_ITEM_SHAPE		,eGMC_ICON_TAB4,
								iX+446	,iY+40		,
								eGBSW_NORMAL_TIME_BAR,eGBSW_ACTIVE_TIME_BAR,eGBSW_SELECT_TIME_BAR,eGBSW_SELECT_TIME_BAR);
		m_bmButton.addImageBar(	dMSG_ANIMAL_SHAPE	,eGMC_ICON_TAB5,
								iX+239	,iY+58		,
								eGBSW_NORMAL_TIME_BAR,eGBSW_ACTIVE_TIME_BAR,eGBSW_SELECT_TIME_BAR,eGBSW_SELECT_TIME_BAR);
		m_bmButton.addImageBar(	dMSG_SYMBOL_PATTERN	,eGMC_ICON_TAB6,
								iX+308	,iY+58		,
								eGBSW_NORMAL_TIME_BAR,eGBSW_ACTIVE_TIME_BAR,eGBSW_SELECT_TIME_BAR,eGBSW_SELECT_TIME_BAR);
		m_bmButton.addImageBar(	dMSG_UNIQUE1		,eGMC_ICON_TAB7,
								iX+377	,iY+58		,
								eGBSW_NORMAL_TIME_BAR,eGBSW_ACTIVE_TIME_BAR,eGBSW_SELECT_TIME_BAR,eGBSW_SELECT_TIME_BAR);
		m_bmButton.addImageBar(	dMSG_UNIQUE2		,eGMC_ICON_TAB8,
								iX+446	,iY+58		,
								eGBSW_NORMAL_TIME_BAR,eGBSW_ACTIVE_TIME_BAR,eGBSW_SELECT_TIME_BAR,eGBSW_SELECT_TIME_BAR);

		m_bmButton.check(m_iSelectIconShapePart+eGMC_ICON_TAB1);
	}

	m_iSelectWorkTab=	_iWorkTab;
}