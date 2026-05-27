#include "CWindowInterface.H"
#include "cMessage.H"
#include "CImageData.H"
#include "cHero.H"

cGuildFeedPetWindow	g_gwFeedGuildPet;

const	int	c_iContentsHeight	=	58;

enum
{
	eGGI_SHIELD_CRAFT	=	152,
	eGGI_ELEMENTAL_POST	,
	eGGI_ETHER_SHELL	,
	eGGI_HOLLOW_KNIGHT	,

	eGPI_OBITIAN		,
	eGPI_DASH_BLADE		,
	eGPI_ASTRO_BOW		,
	eGPI_CRISTAL_WATER	,
};

cGuildFeedPetWindow::cGuildFeedPetWindow()
{
}

BOOL
cGuildFeedPetWindow::isOpened()//	열렸냐?
{
	if	(s_iPopupInterface	==	ePIW_GUILD_FEED_PET_WINDOW)
		return	TRUE;

	return	FALSE;
}

void
cGuildFeedPetWindow::open()	//	열기
{
	if	(s_iPopupInterface	==	ePIW_GUILD_FEED_PET_WINDOW)
	{
		ClosePopupWindow();
		return;
	}

	ClosePopupWindow();

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_TRADE);

	s_iPopupInterface		=	ePIW_GUILD_FEED_PET_WINDOW;
	s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2	=	g_iScreenWidth;

	m_rect.x1				=	g_iScreenWidth-iWindowWidth*2;
	m_rect.x2				=	g_iScreenWidth;

	m_rect.y1				=	g_iCorrectBI_Y/2;
	m_rect.y2				=	m_rect.y1+iWindowHeight;

	{
		s_bmButton.reset();

		s_bmButton.addImageBar(dMSG_CLOSE	,IDCANCEL	,m_rect.x1+196,472,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
		s_bmButton.setClickMargin(IDCANCEL,1,1);
		s_bmButton.setAllBarTextColor(0,FALSE);
	}

	SetInventoryWindowPos();
	InitPopupWindowCloseButton();
}

void
cGuildFeedPetWindow::close()	//창 닫기
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_FEED_PET_WINDOW)	
		return;

	cINPDEV::ReleaseExclusive();
	m_bmButton.reset();
	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
}

int
cGuildFeedPetWindow::getFocusPet()
{
	int		iX,iY,iCount	=	0;
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	cRECT	rect;

	iX		=	m_rect.x1+16;
	iY		=	m_rect.y1+16;

	for	(int iPet=0;iPet<dGUILD_PET_TYPE_COUNT;iPet++)
	{
		rect.set(iX,iY,iX+iWindowWidth,iY+40);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(0,c_iContentsHeight);
			return	iPet;
		}

		iY	+=	c_iContentsHeight;
	}
	for	(int iGuardian=0;iGuardian<dGUILD_GUARDIAN_TYPE_COUNT;iGuardian++)
	{
		rect.set(iX,iY,iX+iWindowWidth,iY+40);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(0,c_iContentsHeight);
			return	iGuardian+dGUILD_PET_TYPE_COUNT;
		}

		iY	+=	c_iContentsHeight;
	}

	return	-1;
}

void
cGuildFeedPetWindow::draw()		//그리기
{
	DrawInventoryWindow();

	int		iX,iY,iCount	=	0;
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_PET);

	iX		=	m_rect.x1;
	iY		=	m_rect.y1;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillAB(0,iX+8,iY+6,iX+266-4,iY+482-8+iTitleHeight,24);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8+iTitleHeight);
	
	g_sprInterface.Put(iX	,iY		,eWINDOW_TITLE_PET);
	g_sprInterface.Put(iX, iY+iTitleHeight, eWINDOW);

	iX		=	m_rect.x1+16;	
	iY		=	m_rect.y1+16+iTitleHeight;

	for	(int iPet=eGUILD_PET_OBITIAN;iPet<=eGUILD_PET_CRISTAL_WATER;iPet++)
	{
		if	(m_iFocusPet+1	==	iPet)
			cDRAW::Fill(RGB24To16(0,100,200),iX-5,iY-6,iX+iWindowWidth-24,iY+c_iContentsHeight-6);

		g_sprInterface2.Put(iX,iY+6,eGPI_OBITIAN+iPet-1);
		g_sprInterface.Put(iX,iY+6,eIWI_ITEM_SLOT_FRAME);

		s_text.put(iX+40,iY-4,WHITE,"%s <c:LTGREEN>LV %d<n>",g_lpstrGuildPetName[iPet],g_guild.getPetLevel(iPet));
		s_text.put(iX+40,iY+10,WHITE,"%s %d",dMSG_EXP,g_guild.getPetExp(iPet));
		s_text.put(iX+40,iY+24,WHITE,"%s %d",dMSG_REMAIN_EXP_FOR_LEVEL_UP,g_guild.getPetRemainExpForLevelUp(iPet));

		{
			CGuildPetInfo	*lpPet	=	g_guild.getPet(iPet);

			int	iPetLevel					=	g_guild.getPetLevel(iPet);
			int	iPetOldExp					=	max(lpPet->m_dwExperience-lpPet->m_wExpDuringWeek,0);
			int	iOldLevel					=	g_guild.getPetLevelByExp(iPetOldExp);
			int	iRequireExpForUpkeepLevel	=	iOldLevel*c_iDownGuildPetGuardianLevelFactor;
			int	iExpDuringWeek				=	lpPet->m_wExpDuringWeek;

			if	(iOldLevel	<	c_iMinLevelForCheckUpkeepGuildPetAndGuardianLevel)
				iRequireExpForUpkeepLevel	=	0;

			iExpDuringWeek					=	min(iExpDuringWeek,iRequireExpForUpkeepLevel);

			DWORD	dwTextColor				=	LTYELLOW;

			if	(iExpDuringWeek	>=	iRequireExpForUpkeepLevel)
				dwTextColor					=	LTORANGE;

			s_text.put(iX+40,iY+38,dwTextColor,"%s [%d/%d]",dMSG_EXP_FOR_UPKEEP_LEVEL,iExpDuringWeek,iRequireExpForUpkeepLevel);
		}

		iY	+=	c_iContentsHeight;
	}
	for	(int iGuardian=eGUILD_GUARDIAN_SHIELD_CRAFT;iGuardian<=eGUILD_GUARDIAN_HOLLOW_KNIGHT;iGuardian++)
	{
		if	(m_iFocusPet+1	==	iGuardian+dGUILD_PET_TYPE_COUNT)
			cDRAW::Fill(RGB24To16(0,100,200),iX-5,iY-6,iX+iWindowWidth-24,iY+c_iContentsHeight-6);

		g_sprInterface2.Put(iX,iY+6,eGGI_SHIELD_CRAFT+iGuardian-1);
		g_sprInterface.Put(iX,iY+6,eIWI_ITEM_SLOT_FRAME);

		s_text.put(iX+40,iY-4,WHITE,"%s <c:LTGREEN>LV %d<n>",g_lpstrGuildGuardianName[iGuardian],g_guild.getGuardianLevel(iGuardian));
		s_text.put(iX+40,iY+10,WHITE,"%s %d",dMSG_EXP,g_guild.getGuardianExp(iGuardian));
		s_text.put(iX+40,iY+24,WHITE,"%s %d",dMSG_REMAIN_EXP_FOR_LEVEL_UP,g_guild.getGuardianRemainExpForLevelUp(iGuardian));

		{
			CGuildPetInfo	*lpGuardian		=	g_guild.getGuardian(iGuardian);

			int	iGuardianLevel				=	g_guild.getGuardianLevel(iGuardian);
			int	iGuardianOldExp				=	max(lpGuardian->m_dwExperience-lpGuardian->m_wExpDuringWeek,0);
			int	iOldLevel					=	g_guild.getPetLevelByExp(iGuardianOldExp);
			int	iRequireExpForUpkeepLevel	=	iOldLevel*c_iDownGuildPetGuardianLevelFactor;
			int	iExpDuringWeek				=	lpGuardian->m_wExpDuringWeek;

			if	(iOldLevel	<	c_iMinLevelForCheckUpkeepGuildPetAndGuardianLevel)
				iRequireExpForUpkeepLevel	=	0;

			iExpDuringWeek					=	min(iExpDuringWeek,iRequireExpForUpkeepLevel);

			DWORD	dwTextColor				=	LTYELLOW;

			if	(iExpDuringWeek	>=	iRequireExpForUpkeepLevel)
				dwTextColor					=	LTORANGE;

			s_text.put(iX+40,iY+38,dwTextColor,"%s [%d/%d]",dMSG_EXP_FOR_UPKEEP_LEVEL,iExpDuringWeek,iRequireExpForUpkeepLevel);
		}

		iY	+=	c_iContentsHeight;
	}
}

void
cGuildFeedPetWindow::update()	//업데이트
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_FEED_PET_WINDOW)
		return;

	if	(g_msgBox.isPoped())
		return;

	m_iFocusPet	=	getFocusPet();

	if	(m_iFocusPet	!=	-1)
	{
		cSTRING	string;
		char	*lpstrTip	=	NULL;

		if	(m_iFocusPet	>=	dGUILD_PET_TYPE_COUNT)
		{
			lpstrTip	=	g_guild.getGuildGuardianToolTip(m_iFocusPet-dGUILD_PET_TYPE_COUNT+1);
			string.Add("%s\n\n",g_lpstrGuildGuardianName[m_iFocusPet-dGUILD_PET_TYPE_COUNT+1]);
		}
		else
		{
			lpstrTip	=	g_guild.getGuildPetToolTip(m_iFocusPet+1);
			string.Add("%s\n\n",g_lpstrGuildPetName[m_iFocusPet+1]);
		}

		string.Add("<c:LTCYAN><%s><n>\n",dMSG_INFORMATION);
		string.Add(lpstrTip);

		s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,string.String,eTFM_NONE);
	}

	OperateInventoryWindow();
	OperateDragItem();	//	인벤토리 창에 드래그 아이템 처리
}

void
cGuildFeedPetWindow::feedItem()
{
	m_iFeedItem	=	s_iDragItem;

	if	(s_iDragItem	<	0	||	s_iDragItem	>=	g_hero.m_wItemSlotCount	||	m_iFocusPet	==	-1)
		return;

	m_iSelectPet	=	m_iFocusPet;

	cItem	*lpItem	=	&g_hero.m_aItems[s_iDragItem];
	int		iExp	=	lpItem->getExpForGuildPet();

	if	(iExp	==	0	||	lpItem->isCanNotFeedToPetItem() || lpItem->isExpirationData())
	{
		g_msgBox.cPopup("",dMSG_INCORRECT_ITEM_FOR_FEED_GUILD_PET_FORM,dMSG_OK);
		return;
	}

	char	*lpstrName	=	NULL;

	if	(m_iFocusPet	>=	dGUILD_PET_TYPE_COUNT)
		lpstrName	=	g_lpstrGuildGuardianName[m_iFocusPet-dGUILD_PET_TYPE_COUNT+1];
	else
		lpstrName	=	g_lpstrGuildPetName[m_iFocusPet+1];

	char	strQ[512];

	sprintf(strQ,dMSG_DO_YOU_WANT_FEED_ITEM_TO_GUILD_PET_FORM,lpstrName,iExp);

	g_msgBox.cPopup("",strQ,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_FEED_ITEM_TO_GUILD_PET_OR_GUILD_GUARDIAN);
}
