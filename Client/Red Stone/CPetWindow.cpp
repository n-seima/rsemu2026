#include "CPetWindow.H"
#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"

CPetWindow	g_gwPet;

enum
{
	ePW_CHANGE_PET_NAME_1,
	ePW_CHANGE_PET_NAME_2,
	ePW_PARTING_WITH_PET1,
	ePW_PARTING_WITH_PET2,
	ePW_PARTING_WITH_BEAST1,
	ePW_PARTING_WITH_BEAST2,
};


CPetWindow::CPetWindow()
{
}

//
//	파티창 열기
void
CPetWindow::open()
{
	if (g_hero.getPetCount()+g_hero.getSummonBeastCount() == 0)
		return;

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_PET);

	m_posPetWindow.x		=	g_iScreenWidth-iWindowWidth*2;
	m_posPetWindow.y		=	iWindowTitleHeight+g_iCorrectBI_Y/2;

	s_rectInterfaceSize.set(g_iScreenWidth-iWindowWidth*2, g_iCorrectBI_Y/2, g_iScreenWidth, iWindowTitleHeight+g_iCorrectBI_Y/2+iWindowHeight);

	{
		int	iPannelHeight	=	g_sprInterface.getSpriteHeight(eTMWI_PET_PANNEL_UNDEAD)+2;
		int	iX				=	m_posPetWindow.x+9;
		int	iY				=	m_posPetWindow.y+6;
		int	iBarHeight		=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
		int	iBarWidth		=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);

		s_bmButton.reset();

		s_bmButton.addImageBar(dCHANGE_NAME,ePW_CHANGE_PET_NAME_1,iX+iWindowWidth-iBarWidth-15,iY+iPannelHeight	-24	,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
		s_bmButton.addImageBar(dCHANGE_NAME,ePW_CHANGE_PET_NAME_2,iX+iWindowWidth-iBarWidth-15,iY+iPannelHeight*2-24	,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

		iY	-=	(iBarHeight+2);

		s_bmButton.addImageBar(dMSG_PARTING_WITH_PET,ePW_PARTING_WITH_PET1,iX+iWindowWidth-iBarWidth-15,iY+iPannelHeight	-24	,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
		s_bmButton.addImageBar(dMSG_PARTING_WITH_PET,ePW_PARTING_WITH_PET2,iX+iWindowWidth-iBarWidth-15,iY+iPannelHeight*2	-24	,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
		s_bmButton.addImageBar(dMSG_RELEASE_SUMMONED_BEAST,ePW_PARTING_WITH_BEAST1,iX+iWindowWidth-iBarWidth-15,iY+iPannelHeight*3-2	,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
		s_bmButton.addImageBar(dMSG_RELEASE_SUMMONED_BEAST,ePW_PARTING_WITH_BEAST2,iX+iWindowWidth-iBarWidth-15,iY+iPannelHeight*4-2	,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

		s_bmButton.setClicktAllBarMargin(1,1);
		s_bmButton.setAllBarTextColor(0,FALSE);
	}
}

//
//
void
CPetWindow::close()
{
	s_bmButton.reset();
}

//
//	그리기
void
CPetWindow::draw()
{
	if	(g_hero.getPetCount()+g_hero.getSummonBeastCount() == 0)
		return;

	int		iX,iY,i,iCount=0;
	cRECT	rect;
	char	strToolTip[1024];

	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iBarHeight	=	g_sprInterface.getSpriteHeight(eTMWI_PET_PANNEL_UNDEAD)+2;

	iX		=	m_posPetWindow.x;
	iY		=	m_posPetWindow.y;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+4+iWindowWidth-8,iY+iWindowHeight-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+4+iWindowWidth-8,iY+iWindowHeight-8);

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface.Put(iX,g_iCorrectBI_Y/2,eWINDOW_TITLE_PET);

	iX		=	m_posPetWindow.x+9;
	iY		=	m_posPetWindow.y+6;

	int	iDX=0,iDY=0,iPetCount=0,iSummonBeastCount=0;

	int	iOwnPetCount	=	dOWN_PET_COUNT,iOwnSummonBeastCount	=	dOWN_SUMMON_BEAST_COUNT;

	if	(g_hero.m_wJob/2	!=	dJOB_SUMMONER/2)
	{
		iOwnPetCount		=	1;
		iOwnSummonBeastCount=	0;
	}

	for (i=0;i<iOwnPetCount;i++)
	{
		cPet		*lpPet		=	g_hero.getPet(i);
		CActor		*lpPetActor	=	g_hero.getPetActor(i);
		int			iWidth		=	g_sprInterface.getSpriteWidth(eTMWI_PET_PANNEL_UNDEAD);

		if (lpPet && lpPetActor)
		{
			CJobInfo	*lpJob	=	lpPet->getJob();

			g_sprInterface.Put(iX,iY,eTMWI_PET_PANNEL_UNDEAD+lpJob->m_wKind);
			//g_sprInterface.Put(iX+iWidth/2,iY+105,eTMWI_PET_ICON_UNDEAD+lpJob->m_wKind);
			g_sprInterface.Put(iX+iWidth/2,iY+88,eTMWI_PET_ICON_UNDEAD2+lpJob->m_wKind);			// 10.02.19

			LONGLONG	llBarWidth	=	g_sprInterface.getSpriteWidth(eTMWI_GRAY_GAGUGE);
			int			iNameWidth	=	llBarWidth*lpPetActor->m_iHP/max(lpPetActor->m_iMaxHP/100,1)/100;

			iDX	=	56;
			iDY	=	6;

			g_sprInterface.Put(iX+iDX,iY+iDY,eTMWI_GRAY_GAGUGE,100,100,dPUT_HALF_BLENDING);
			g_sprInterface.putClipedImage(iX+iDX,iY+iDY,eTMWI_RED_GAGUGE,iNameWidth);
			s_text.put(iX+iDX+6,iY+iDY+3,WHITE,"%s [<c:YELLOW>%d<n>/<c:LTGREEN>%d<n>]",lpPet->getName(),lpPetActor->m_iHP/100,lpPetActor->m_iMaxHP/100);
			iDY	+=	24;

			int		iCurrentLevel	=	lpPet->m_bf10Level;
			CActor	*lpPetBody		=	g_am.getActor(lpPet->m_wBodySerial);

			if	(lpPetBody)
				iCurrentLevel	=	lpPetBody->m_wLevel;

			// 10.02.19
			g_sprInterface.Put(iX+iDX+14,iY+iDY+5,ePWI_PET_ICON_LEVEL);

			if	(iCurrentLevel	==	lpPet->m_bf10Level)
				s_text.put(iX+iDX+30,iY+iDY,RGB(255,198,0),"<c:255,238,121>%d<n>",lpPet->m_bf10Level);
			else
			if	(iCurrentLevel	>	lpPet->m_bf10Level)
				s_text.put(iX+iDX+30,iY+iDY,RGB(255,198,0),"<c:LTGREEN>%d<n>(<c:255,238,121>%d<n>)",iCurrentLevel,lpPet->m_bf10Level);
			else
			if	(iCurrentLevel	<	lpPet->m_bf10Level)
				s_text.put(iX+iDX+30,iY+iDY,RGB(255,198,0),"<c:LTRED>%d<n>(<c:255,238,121>%d<n>)",iCurrentLevel,lpPet->m_bf10Level);
			
			rect.set(iX+iDX,iY+iDY,iX+iDX+24,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_LEVEL);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}
				
			g_sprInterface.Put(iX+iDX+114,iY+iDY+5,ePWI_PET_ICON_EXP);
			s_text.put(iX+iDX+130,iY+iDY,RGB(255,238,121),"<c:255,238,121>%d<n>",lpPet->m_dwExperience);

			rect.set(iX+iDX+100,iY+iDY,iX+iDX+124,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_EXP);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}

			iDY+=16;
			g_sprInterface.Put(iX+iDX+10,iY+iDY+5,ePWI_PET_ICON_ATTACK_POWER);
			s_text.put(iX+iDX+20,iY+iDY,RGB(144,211,132),"<c:194,220,160>%d~%d<n>",lpPet->m_wMinAttackPower,lpPet->m_wMaxAttackPower);
			
			rect.set(iX+iDX,iY+iDY,iX+iDX+12,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_ATTACK_POWER);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}


			g_sprInterface.Put(iX+iDX+110,iY+iDY+5,ePWI_PET_ICON_DEFENSE_POWER);
			s_text.put(iX+iDX+120,iY+iDY,RGB(118,206,163),"<c:191,227,199>%d<n>",lpPet->m_iDefensePower);

			rect.set(iX+iDX+100,iY+iDY,iX+iDX+112,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_DEFENSE_POWER);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}

			iDY+=16;
			g_sprInterface.Put(iX+iDX+10,iY+iDY+5,ePWI_PET_ICON_LOYALTY);	
			s_text.put(iX+iDX+20,iY+iDY,RGB(86,154,255),"<c:158,197,255>%d<n>",lpPet->m_bf14Loyalty/10);

			rect.set(iX+iDX,iY+iDY,iX+iDX+12,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_LOYALTY);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}

			iDY+=16;

			if	(g_hero.m_wJob/2	==	dJOB_SUMMONER/2)
			{
				g_sprInterface.Put(iX+iDX+10,iY+iDY+5,ePWI_PET_ICON_FEEDING_RED_PEPPER);	
				s_text.put(iX+iDX+20,iY+iDY,RGB(255,80,92),"<c:254,160,167>%d<n>",lpPet->m_bf5BoostCount);
				
				rect.set(iX+iDX,iY+iDY,iX+iDX+12,iY+iDY+10);
				if (rect.isIn(s_posMouse.x,s_posMouse.y))
				{
					sprintf(strToolTip,dMSG_FEEDING_RED_PEPPER_COUNT_FORM);
					s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
				}
			}



			iDY+=16;
			
			g_sprInterface.Put(iX+iDX+10,iY+iDY+5,ePWI_PET_ICON_BREEDING_TIME);	

			if (lpPet->m_wBreedingTime	>=	43200)
				s_sText.put(iX+iDX+20,iY+iDY,RGB(255,198,0),"<c:255,238,121>%s<n>",dMSG_BREEDING_TIME_OVER_ONE_MONTH);	
			else
				s_sText.put(iX+iDX+20,iY+iDY,RGB(255,198,0),"<c:255,238,121>%s<n>",_ms(dMSG_BREEDING_TIME_FORM,lpPet->m_wBreedingTime/60/24,(lpPet->m_wBreedingTime/60)%24,lpPet->m_wBreedingTime%60));

			rect.set(iX+iDX,iY+iDY,iX+iDX+12,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_BREEDING_TIME);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}

			iPetCount++;
		}
		else
		{
			g_sprInterface.Put(iX,iY,eTMWI_PET_PANNEL_VALID);
		}

		s_text.cPut(iX+iWidth/2,iY+20,WHITE,dMSG_PET);
		g_sprInterface.Put(iX+iWidth/2,iY+60,eGBSW_1+i);	//	1,2,3,4...

		g_sprInterface.Put(iX-2,iY+iBarHeight-4,ePWI_BORDER);
		iY	+=	iBarHeight;
	}

	for (i=0;i<iOwnSummonBeastCount;i++)
	{
		cSummonBeast	*lpBeast	=	g_hero.getSummonBeast(i);
		CActor			*lpBodyActor=	g_hero.getSummonBeastActor(i);
		int				iWidth		=	g_sprInterface.getSpriteWidth(eTMWI_SUMMON_BEAST_PANNEL_FIRE);

		if (lpBeast && lpBodyActor)
		{
			g_sprInterface.Put(iX,iY,eTMWI_SUMMON_BEAST_PANNEL_FIRE+lpBeast->m_bf2Type);
			g_sprInterface.Put(iX+iWidth/2-19,iY+105-36,eTMWI_SUMMON_BEAST_ICON_FIRE+lpBeast->m_bf2Type);

			LONGLONG	llBarWidth	=	g_sprInterface.getSpriteWidth(eTMWI_GRAY_GAGUGE);
			int			iNameWidth	=	llBarWidth*lpBodyActor->m_iHP/max(lpBodyActor->m_iMaxHP/100,1)/100;

			iDX	=	56;
			iDY	=	6;

			g_sprInterface.Put(iX+iDX,iY+iDY,eTMWI_GRAY_GAGUGE,100,100,dPUT_HALF_BLENDING);
			g_sprInterface.putClipedImage(iX+iDX,iY+iDY,eTMWI_RED_GAGUGE,iNameWidth);
			s_text.put(iX+iDX+6,iY+iDY+3,WHITE,"%s [<c:YELLOW>%d<n>/<c:LTGREEN>%d<n>]",lpBodyActor->m_strName,lpBodyActor->m_iHP/100,lpBodyActor->m_iMaxHP/100);
			iDY+=24;

			int	iCurrentLevel	=	lpBodyActor->m_wLevel;

			g_sprInterface.Put(iX+iDX+14,iY+iDY+5,ePWI_PET_ICON_LEVEL);	
			
			if	(iCurrentLevel	==	lpBeast->m_bf10Level)
				s_text.put(iX+iDX+30,iY+iDY,RGB(255,198,0),"<c:255,238,121>%d<n>",lpBeast->m_bf10Level);
			else
			if	(iCurrentLevel	>	lpBeast->m_bf10Level)
				s_text.put(iX+iDX+30,iY+iDY,RGB(255,198,0),"<c:LTGREEN>%d<n>(<c:255,238,121>%d<n>)",iCurrentLevel,lpBeast->m_bf10Level);
			else
			if	(iCurrentLevel	<	lpBeast->m_bf10Level)
				s_text.put(iX+iDX+30,iY+iDY,RGB(255,198,0),"<c:LTRED>%d<n>(<c:255,238,121>%d<n>)",iCurrentLevel,lpBeast->m_bf10Level);

			rect.set(iX+iDX,iY+iDY,iX+iDX+24,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_LEVEL);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}

			g_sprInterface.Put(iX+iDX+114,iY+iDY+5,ePWI_PET_ICON_RANK);
			s_text.put(iX+iDX+130,iY+iDY,RGB(86,154,255),"<c:158,197,255>%d<n> %s",lpBeast->m_bf2Grade+1,dMSG_GRADE);

			rect.set(iX+iDX+100,iY+iDY,iX+iDX+124,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_PET_CLASS);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}

			iDY+=16;
			g_sprInterface.Put(iX+iDX+10,iY+iDY+5,ePWI_PET_ICON_ATTACK_POWER);
			s_text.put(iX+iDX+20,iY+iDY,RGB(144,211,132),"<c:194,220,160>%d~%d<n>",lpBeast->m_wMinAttackPower,lpBeast->m_wMaxAttackPower);

			rect.set(iX+iDX,iY+iDY,iX+iDX+12,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_ATTACK_POWER);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}

			g_sprInterface.Put(iX+iDX+110,iY+iDY+5,ePWI_PET_ICON_DEFENSE_POWER);
			s_text.put(iX+iDX+120,iY+iDY,RGB(118,206,163),"<c:191,227,199>%d<n>",lpBeast->m_wDefensePower);

			rect.set(iX+iDX+100,iY+iDY,iX+iDX+112,iY+iDY+10);
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				sprintf(strToolTip,dMSG_DEFENSE_POWER);
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}

			iSummonBeastCount++;
		}
		else
		{
			g_sprInterface.Put(iX,iY,eTMWI_PET_PANNEL_VALID);
		}

		s_text.cPut(iX+iWidth/2,iY+20,WHITE,dMSG_SUMMON_BEAST);
		g_sprInterface.Put(iX+iWidth/2,iY+60,eGBSW_1+i);	//	1,2,3,4...

		g_sprInterface.Put(iX-2,iY+iBarHeight-4,ePWI_BORDER);
		iY	+=	iBarHeight;
	}

	s_bmButton.draw();
}
//
//	파티창 업데이터
void
CPetWindow::update()
{
	if(g_msgBox.isPoped())
		return;

	int	iPetCount	=	g_hero.getPetCount(),i;

	if (g_hero.getPetCount()+g_hero.getSummonBeastCount() == 0)
		return;

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);

	if (s_rectInterfaceSize.x2-s_rectInterfaceSize.x1 != iWindowWidth*2)
	{
		s_rectInterfaceSize.x1 = g_iScreenWidth-iWindowWidth*2;
		s_rectInterfaceSize.x2 = g_iScreenWidth;

		open();
	}

	iPetCount		=	0;

	m_iResetPetNameItemIndex	=	g_hero.getItemIndexByOption(eIE_CHANGE_PET_NAME);

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		cPet	*lpPet			=	g_hero.getPet(i);
		BOOL	bIsHideRename	=	FALSE;

		if	(!lpPet)
			bIsHideRename	=	TRUE;
		else
		if	(strcmp(lpPet->m_strName,"noname") !=0 && m_iResetPetNameItemIndex	==	-1)
			bIsHideRename	=	TRUE;

		if	(bIsHideRename)
			s_bmButton.hide(ePW_CHANGE_PET_NAME_1+i);
		else
			s_bmButton.show(ePW_CHANGE_PET_NAME_1+i);

		if	(!lpPet)
			s_bmButton.hide(ePW_PARTING_WITH_PET1+i);
		else
		{
			s_bmButton.show(ePW_PARTING_WITH_PET1+i);

			if (bIsHideRename)
			{
				int	iBarHeight	=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);

				s_bmButton.setCorrectPos(ePW_PARTING_WITH_PET1+i,0,iBarHeight+2);
			}
			else
				s_bmButton.setCorrectPos(ePW_PARTING_WITH_PET1+i,0,0);
		}
	}

	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		cSummonBeast	*lpSummonBeast	=	g_hero.getSummonBeast(i);

		if (!lpSummonBeast)
			s_bmButton.hide(ePW_PARTING_WITH_BEAST1+i);
		else
			s_bmButton.show(ePW_PARTING_WITH_BEAST1+i);
	}

	DWORD	dwSelectMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwSelectMenu)
	{
		case	ePW_CHANGE_PET_NAME_1	:
		case	ePW_CHANGE_PET_NAME_2	:
		{
			m_wSelectPetIndex	=	(WORD)(dwSelectMenu-ePW_CHANGE_PET_NAME_1);

			cPet	*lpPet		=	g_hero.getPet(m_wSelectPetIndex);

			if	(!lpPet)
				break;

			if	(lpPet->isNoName() == FALSE)
			{
				if	(m_iResetPetNameItemIndex	==	-1)
					break;

				cItem	*lpItem	=	g_hero.getInventoryItem(m_iResetPetNameItemIndex);

				if	(!lpItem)
					break;

				g_msgBox.cPopup("",_ms(dMSG_DO_YOU_REALLY_RESET_PET_NAME_FORM,lpItem->getName(FALSE),m_wSelectPetIndex+1,lpPet->getName()),dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_RESET_PET_NAME);

				break;
			}

			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,_ms("@%s %d %s",dMSG_CHANGE_PET_NAME_KEYWORD,m_wSelectPetIndex,dMSG_PLEASE_INPUT_WANT_PET_NAME));
			s_inputChatting.setSelect(0,strlen(dMSG_CHANGE_PET_NAME_KEYWORD)+4,-1);
			break;
		}

		case	ePW_PARTING_WITH_PET1	:
		case	ePW_PARTING_WITH_PET2	:
		{
			m_wSelectPetIndex	=	(WORD)(dwSelectMenu-ePW_PARTING_WITH_PET1);

			cPet	*lpPet	=	g_hero.getPet(m_wSelectPetIndex);

			char	strName[dPET_NAME_LENGTH+2];

			memcpy(strName,lpPet->m_strName,dPET_NAME_LENGTH);

			strName[dPET_NAME_LENGTH]	=	NULL;

			g_msgBox.cPopup("",_ms(dMSG_DO_YOU_REALLY_PARTING_WITH_THAT_PET_FORM,m_wSelectPetIndex+1,strName),dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_PARTING_WITH_PET);
			break;
		}

		case	ePW_PARTING_WITH_BEAST1	:
		case	ePW_PARTING_WITH_BEAST2	:
		{
			m_wSelectSummonBeast	=	(WORD)(dwSelectMenu-ePW_PARTING_WITH_BEAST1);

			CActor	*lpBeast	=	g_hero.getSummonBeastActor(m_wSelectSummonBeast);

			g_msgBox.cPopup("",_ms(dMSG_DO_YOU_REALLY_RELEASE_THAT_SUMMON_BEAST_FORM,lpBeast->m_strName),dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_RELEASE_SUMMON_BEAST);
			break;
		}
	}
}