#include "CGamePlay.H"
#include "CActor.H"
#include "CMessage.H"
#include "CWindowInterface.H"

CSkillWindow	g_gwSkill;

#define	dSKILL_POINT_FRAME_HEIGHT		30
#define	dSKILL_ITEM_HEIGHT				43
#define	dSKILL_COUNT_IN_SKILL_WINDOW	20

cRECT	l_rectSkillWindow	=	{528,42,782,472};

//
//	Skill Window Button Menu
enum
{
	eSWBM_CHANGE_VIEW_SKILL_ICON_SIZE	=	1000,
};
// 10.01.07 함수 추가
// 아이템의 타입에 대한 장비창 인덱스 리턴

int
CSkillWindow::getSkillItemtoEquipmentIndex(int _type)
{
	
	
	int iIndex	= 0xffff;
	if(eIK_END<=_type)
		return iIndex;
	
	iIndex	= c_aItemEquipPlace[_type];
	
	if(dITEM_KIND_WASTING == iIndex)
		iIndex = 0xffff;

	return iIndex;
/*
	switch(_type)
	{
	// 무기
	case eIK_SPEAR:
	case eIK_SCYTHE:
	case eIK_ONEHANDED_SWORD:
	case eIK_STAFF:
	case eIK_MACE:
	case eIK_WING:
	case eIK_KNIFE:
	case eIK_FLUTE:
	case eIK_SLING:
	case eIK_MAGICAL_ROD:
	case eIK_WHIP:
		Index = dEQUIP_WEAPON;
		break;
	// 방패/탄환
	case eIK_ARROW:
	case eIK_SHIELD:
	case eIK_BROOCH:
	case eIK_BULLET:
	case eIK_SHOLDER_TATOO:
	case eIK_CROSS:
		Index = dEQUIP_SHIELD_BULLET;
		break;
	// 갑옷
	case eIK_ARMOR:
	case eIK_EXCLUSIVE_ARMOR:
		Index = dEQUIP_ARMOR;
		break;
	// 장갑
	case eIK_GLOVE:
	case eIK_GLOVE_STYLE:
	case eIK_BRACELET:
	case eIK_ARM_TATOO:
	case eIK_CLAW:
		Index = dEQUIP_GLOVE;
		break;
	// 헬멧 종류
	case eIK_HELM:
	case eIK_CROWN:
		Index = dEQUIP_HELM;
		break;
	// 귀걸이
	case eIK_EARING:
	case eIK_CLOAK:
		Index = dEQUIP_EARING;
		break;
	// 목걸이
	case eIK_NECKLACE:
		Index = dEQUIP_NECKLACE;
		break;
	// 벨트
	case eIK_BELT:
		Index = dEQUIP_BELT;
		break;
	// 신발
	case eIK_BOOTS:
		Index = dEQUIP_SHOES;
		break;
	// 무기2
	case eIK_BOW:
	case eIK_TWOHANDED_SWORD:
	case eIK_TOOTH:
		Index = dEQUIP_WEAPON_2;
		break;
	}
	
	return Index;
	*/
}
CSkillWindow::CSkillWindow()
{
	m_iSkillWindowScrollBarPos	=	0;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	m_iLastActionTime			=	0;
}

//
//	스킬 인터페이스 닫기
void
CSkillWindow::close()
{
	m_iSkillWindowScrollBarPos	=	s_sbPopupWindow.m_iCurrentPos;
	s_iFocusSkill				=	0xffff;

	s_bmBottomMenu.check(eBIM_MENU_SKILL,FALSE);
	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_SKILL_DRAG);

	s_bmPlusState.reset();
	m_bmReactionSkillState.reset();
	m_bmJobTab.reset();

	g_gwEasySkillTree.close();
}

//
//	스킬 윈도우 초기 데이터 설정
void
CSkillWindow::init()
{
	m_bmReactionSkillState.init(&g_sprInterface,100);
	m_bmJobTab.init(&g_sprInterface,3);
	g_gwEasySkillTree.init();
	m_iSelectJobTab				=	-1;
}

//
//	스킬 윈도우의 버튼들을 초기화
void
CSkillWindow::resetButtons()
{
	int	i;

	m_bmReactionSkillState.reset();
	s_bmPlusState.reset();

	for (i=0;i < dABILITY_COUNT;i++)
	{
		s_bmPlusState.addImageBar			("",i,0,0,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		m_bmReactionSkillState.addImageBar	("",i,0,0,eREACTION_SKILL_ON,eREACTION_SKILL_ON,eREACTION_SKILL_ON);
	}

	for (i=0;i < dABILITY_COUNT;i++)
	{
		m_bmReactionSkillState.hide(i);
		s_bmPlusState.hide(i);
	}

	m_bmReactionSkillState.setClicktAllBarMargin(1,1);
}

//  
//	스킬 윈도우의 버튼들 위치 설정
void
CSkillWindow::resetButtonsPos()
{
	int	iX	=	m_posSkillWindow.x+82;
	int	iY	=	m_posSkillWindow.y+45+29;

	int	i,iCount=0,iPutCount=0;

	for (i=0;i < dABILITY_COUNT;i++)
	{
		m_bmReactionSkillState.hide(i);
		s_bmPlusState.hide(i);
	}

	if	(g_config.m_wIsViewLargeSkillIcon)
	{
		iY		-=	2;

		int	iSkipSkillCount	=	s_sbPopupWindow.m_iCurrentPos*2;

		for (i=0,iCount=0;i < dABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&g_hero.m_aAbility[i];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if	(!lpSkill)
				continue;
			if	(!lpSkill->isEnableJob(m_iSelectJobTab))
				continue;

			if	(iCount	<	iSkipSkillCount)
			{
				iCount++;
				continue;
			}

			int	iPutX	=	iX+(iPutCount%2)*125+13;
			int	iPutY	=	iY+(iPutCount/2)*dSKILL_ITEM_HEIGHT+2;

			if (iPutX < 500)
				iPutX += 0;

			s_bmPlusState.resetImageBar(i,"",i,iPutX+2 ,iPutY,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED);

			if	(g_hero.isLevelUpAbleAbility(i))
			{
				if	(!s_bmPlusState.isShow(i))
					s_bmPlusState.show(i);
			}
			else
				s_bmPlusState.hide(i);


			if	(g_hero.isReactionSkill(i))
			{
				if	(g_hero.isActiveReaction(i))	
					m_bmReactionSkillState.resetImageBar(i,"",i,iPutX+13,iPutY,eREACTION_SKILL_ON,eREACTION_SKILL_ON,eREACTION_SKILL_ON);
				else
					m_bmReactionSkillState.resetImageBar(i,"",i,iPutX+13,iPutY,eREACTION_SKILL_OFF,eREACTION_SKILL_OFF_LIGHT,eREACTION_SKILL_OFF_LIGHT);

				if	(lpAbility->m_wLevel > 0	)
					m_bmReactionSkillState.show(i);
				else
					m_bmReactionSkillState.hide(i);
			}

			iCount++;
			iPutCount++;

			if	(iPutCount	>=	dSKILL_COUNT_IN_SKILL_WINDOW)
				break;
		}
	}
	else
	{
		iX	=	m_posSkillWindow.x+32;
		iY	=	m_posSkillWindow.y+49;

		for (i=0,iCount=0;i < dABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&g_hero.m_aAbility[i];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if	(!lpSkill)
				continue;
			if	(!lpSkill->isEnableJob(m_iSelectJobTab))
				continue;

			int		iPutX			=	iX;
			int		iPutY			=	iY+iPutCount*17;
			int		iSkillNameWidth	=	0;
			BOOL	bIsShowPlusButton=	TRUE;

			if	(lpAbility->m_wLevel	==	0)
				iSkillNameWidth	=	s_text.getPixelSize(lpSkill->m_strName);
			else
			{
				if	(lpAbility->isEnableJob(g_hero.m_wJob) && g_hero.getCorrectSkillLevel(lpAbility))
					iSkillNameWidth	=	s_text.getPixelSize(_ms("%s(Lv %d/%d)",lpSkill->m_strName,lpAbility->m_wLevel+g_hero.getCorrectSkillLevel(lpAbility),lpAbility->m_wLevel));
				else
					iSkillNameWidth	=	s_text.getPixelSize(_ms("%s(Lv %d)",lpSkill->m_strName,lpAbility->m_wLevel));
			}

			s_bmPlusState.resetImageBar(i,"",i,iPutX+iSkillNameWidth,iPutY,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED);

			if	(g_hero.isLevelUpAbleAbility(i))
			{
				if	(!s_bmPlusState.isShow(i))
					s_bmPlusState.show(i);
			}
			else
				s_bmPlusState.hide(i);

			bIsShowPlusButton	=	g_hero.isLevelUpAbleAbility(i);

			if	(lpAbility->isEnableJob(g_hero.m_wJob))
				if	(g_hero.isReactionSkill(i))
				{   
					if	(g_hero.isActiveReaction(i))	
						m_bmReactionSkillState.resetImageBar(i,"",i,iPutX+iSkillNameWidth+bIsShowPlusButton*14,iPutY,eREACTION_SKILL_ON,eREACTION_SKILL_ON,eREACTION_SKILL_ON);
					else
						m_bmReactionSkillState.resetImageBar(i,"",i,iPutX+iSkillNameWidth+bIsShowPlusButton*14,iPutY,eREACTION_SKILL_OFF,eREACTION_SKILL_OFF_LIGHT,eREACTION_SKILL_OFF_LIGHT);

					if	(lpAbility->m_wLevel > 0	)
						m_bmReactionSkillState.show(i);
					else
						m_bmReactionSkillState.hide(i);
				}

			iCount++;
			iPutCount++;
		}
	}
}

//
//아이템 & 스테이터스 인터페이스 열기
void
CSkillWindow::open()
{
	if (s_iPopupInterface	==	ePIW_SKILL)
	{
		ClosePopupWindow();

		return;
	}

	ClosePopupWindow();

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eSKILL_WINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eSKILL_WINDOW);
	int	iTabWidth			=	g_sprInterface.getSpriteWidth(eACTIVE_SKILL_TAB);

	m_iSelectJobTab			=	g_hero.m_wJob;
	s_iPopupInterface		=	ePIW_SKILL;

	m_posSkillWindow.x		=	g_iScreenWidth-iWindowWidth;
	m_posSkillWindow.y		=	g_iCorrectBI_Y/2;

	s_rectInterfaceSize.set(g_iScreenWidth-iWindowWidth, m_posSkillWindow.y, g_iScreenWidth, m_posSkillWindow.y+iWindowHeight);

	s_sbPopupWindow.set(m_posSkillWindow.x+iWindowWidth-16,m_posSkillWindow.y+44,430);
	s_sbPopupWindow.setPos(0);//m_iSkillWindowScrollBarPos);

	int	iJob	=	m_iSelectJobTab/2*2;
	
	m_bmJobTab.addImageBar(g_pstrHeroJobName[iJob  ],0,m_posSkillWindow.x+6				,m_posSkillWindow.y+21,eACTIVE_SKILL_TAB,eACTIVE_SKILL_TAB);
	m_bmJobTab.addImageBar(g_pstrHeroJobName[iJob+1],1,m_posSkillWindow.x+6+iTabWidth	,m_posSkillWindow.y+21,eNORMAL_SKILL_TAB_NORMAL,eNORMAL_SKILL_TAB_LIGHT);


	if (m_iSelectJobTab%2	==	0)
	{
		m_bmJobTab.setImageBar(0,eACTIVE_SKILL_TAB,eACTIVE_SKILL_TAB);

		if(iJob == dJOB_OPTICALIST)
		{
			m_bmJobTab.setImageBar(1,eNORMAL_SKILL_TAB_NORMAL,eNORMAL_SKILL_TAB_NORMAL);
			m_bmJobTab.setTextColor(1,GRAY,TRUE);
		}
		else
			m_bmJobTab.setImageBar(1,eNORMAL_SKILL_TAB_NORMAL,eNORMAL_SKILL_TAB_LIGHT);
	}
	else
	{
		m_bmJobTab.setImageBar(0,eNORMAL_SKILL_TAB_NORMAL,eNORMAL_SKILL_TAB_LIGHT);
		m_bmJobTab.setImageBar(1,eACTIVE_SKILL_TAB,eACTIVE_SKILL_TAB);

	}

	s_bmButton.reset();
	s_bmButton.addImageBar("",eSWBM_CHANGE_VIEW_SKILL_ICON_SIZE,g_iScreenWidth-36,m_posSkillWindow.y,eSW_CHANGE_ICON_SIZE_NORMAL,eSW_CHANGE_ICON_SIZE_ACTIVE,eSW_CHANGE_ICON_SIZE_PRESSED,0xffff,0xffff);

	resetButtons();

	s_bmBottomMenu.resetChecker();
	s_bmBottomMenu.check(eBIM_MENU_SKILL,TRUE);

	InitPopupWindowCloseButton();
}

//
//	포커스 스킬을 리턴
inline	int
CSkillWindow::getFocusSkill()
{
	int		iSkipSkillCount	=	s_sbPopupWindow.m_iCurrentPos*2;
	int		iX				=	m_posSkillWindow.x+9;
	int		iY				=	m_posSkillWindow.y+47;
	int		iPutCount		=	0;
	int		i,iCount=0,iFocusSkill=0xffff;
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eSKILL_WINDOW);

	if (g_config.m_wIsViewLargeSkillIcon)
	{
		iY		-=	2;

		for (i=0,iCount=0;i < dABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&g_hero.m_aAbility[i];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if (!lpSkill)	continue;
			if (!lpSkill->isEnableJob(m_iSelectJobTab))	continue;

			if (iCount	<	iSkipSkillCount)
			{
				iCount++;
				continue;
			}

			int	iPutX	=	iX+(iPutCount%2)*125;
			int	iPutY	=	iY+(iPutCount/2)*dSKILL_ITEM_HEIGHT;

			cRECT	rect;

			rect.set(iPutX,iPutY+3,iPutX+2+dITEM_SHAPE_WIDTH+2,iPutY+3+dITEM_SHAPE_HEIGHT+3);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				s_rectTooltip.set(&rect);
				s_rectTooltip.add(-2,-4);
				iFocusSkill	=	i;
				break;
			}

			iCount++;
			iPutCount++;
			if (iPutCount	>=	dSKILL_COUNT_IN_SKILL_WINDOW)	break;
		}
	}
	else
	{
		for (i=0;i < dABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&g_hero.m_aAbility[i];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if (!lpSkill)	continue;
			if (!lpSkill->isEnableJob(m_iSelectJobTab))	continue;

			int	iPutX	=	iX;
			int	iPutY	=	iY+iPutCount*17;
			int	iSkillNameWidth	=	s_text.getPixelSize(lpSkill->m_strName);

			cRECT	rect;

			rect.set(iPutX,iPutY,iPutX+iSkillNameWidth+20,iPutY+16);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				s_rectTooltip.set(&rect);
				s_rectTooltip.add(-2,-4);
				iFocusSkill	=	i;
				break;
			}

			iPutCount++;
		}
	}

	return	iFocusSkill;
}

//
//	스킬을 놓을 슬롯 얻기
int
CSkillWindow::getSkillDroppingSlot()
{
	int	iIndex;

	for (iIndex=0;;iIndex++)
	{
		if (s_arectSkillSlot[iIndex].x1	==	0xffff)	break;
		if (s_arectSkillSlot[iIndex].isIn(s_posMouse.x,s_posMouse.y))	return	iIndex;
	}

	return	0xffff;
}

//
//	스킬 포인트 처리
inline	void
CSkillWindow::operateSkillPoint()
{
	resetButtonsPos();

	int	iSkipSkillCount	=	s_sbPopupWindow.m_iCurrentPos*2,iPutCount=0;

	for (int i=0,iCount=0;i < dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&g_hero.m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (!lpSkill)
			continue;
		if (!lpSkill->isEnableJob(m_iSelectJobTab))
			continue;

		if (iCount	<	iSkipSkillCount)
		{
			iCount++;
			continue;
		}

		if (g_hero.isLevelUpAbleAbility(i))
		{
			if (!s_bmPlusState.isShow(i))	
				s_bmPlusState.show(i);
		}
		else
			s_bmPlusState.hide(i);

		if (g_hero.isReactionSkill(i))
		{
			if (lpAbility->m_wLevel > 0	&&	m_iSelectJobTab	==	g_hero.m_wJob)
				m_bmReactionSkillState.show(i);
			else
				m_bmReactionSkillState.hide(i);
		}

		iCount++;
		iPutCount++;

		if (iPutCount	>=	dSKILL_COUNT_IN_SKILL_WINDOW)
			break;
	}

	if (cINPDEV::IsExclusive(eMOUSE_STATE_INTERFACE))
		return;

	DWORD	dwMenu;

//	스킬 레벨
	{
		dwMenu	=	s_bmPlusState.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (dwMenu	!=	0xffffffff)
		{
			int	iPoint	=	1;

			if	(s_aKey[KEY_LSHIFT])
				iPoint		=	10;

#ifdef dDEBUG_MODE
			if (s_aKey[KEY_LCONTROL])
				iPoint		=	50;
#endif
			if (m_iLastActionTime+500	<	timeGetTime())
			{
				m_iLastActionTime	=	timeGetTime();
				s_agent.sendIncreaseAbilityLevel(dwMenu,iPoint);
			}
		}
	}

//	 리액션 스킬 상태 
	{
		dwMenu	=	m_bmReactionSkillState.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (dwMenu >= 0 && dwMenu < dHERO_ABILITY_COUNT)
		{
			if	(g_hero.isActiveReaction(dwMenu))
				s_agent.sendSetReactionStatus(dwMenu,FALSE);
			else
				s_agent.sendSetReactionStatus(dwMenu,TRUE);
		}
	}

//	직업탭 업데이트
	{
		dwMenu	=	m_bmJobTab.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (dwMenu	==	0 && m_iSelectJobTab%2	==	1)
		{
			m_iSelectJobTab--;
			
			m_bmJobTab.setImageBar(0,eACTIVE_SKILL_TAB,eACTIVE_SKILL_TAB);
			m_bmJobTab.setImageBar(1,eNORMAL_SKILL_TAB_NORMAL,eNORMAL_SKILL_TAB_LIGHT);
			s_sbPopupWindow.setPos(0);
			
			resetButtons();
		}
		
		if (dwMenu	==	1 && m_iSelectJobTab%2	==	0)
		{
			if(g_hero.m_wJob != dJOB_OPTICALIST)
			{
				m_iSelectJobTab++;
				
				m_bmJobTab.setImageBar(0,eNORMAL_SKILL_TAB_NORMAL,eNORMAL_SKILL_TAB_LIGHT);
				m_bmJobTab.setImageBar(1,eACTIVE_SKILL_TAB,eACTIVE_SKILL_TAB);
				s_sbPopupWindow.setPos(0);
				
				resetButtons();
			}
		}
		
	}

//	스킬 아이콘 사이즈
	{
		dwMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (dwMenu	==	eSWBM_CHANGE_VIEW_SKILL_ICON_SIZE)	
			g_config.m_wIsViewLargeSkillIcon	=	1-g_config.m_wIsViewLargeSkillIcon;
	}

	if (s_bmButton.isPressed() || m_bmJobTab.isPressed() || s_bmPlusState.isPressed() || m_bmReactionSkillState.isPressed())
			cINPDEV::SetExclusive(eMOUSE_STATE_INTERFACE);
	else	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_INTERFACE);
}

//
//	스킬 윈도우의 스크롤바 업데이트
inline	void 
CSkillWindow::operateScrollBar()
{
	if (cINPDEV::IsExclusive(eMOUSE_STATE_SCROLL_BAR))
		return;

	if (!g_config.m_wIsViewLargeSkillIcon)
		return;

	int	iPos	=	s_sbPopupWindow.m_iCurrentPos;

	if (l_rectSkillWindow.isIn(s_posMouse.x,s_posMouse.y))
	{
		if (s_iWheelValue<0)
			s_sbPopupWindow.setPos(iPos+1);
		if (s_iWheelValue>0)
			s_sbPopupWindow.setPos(iPos-1);
	}

	if (s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
		cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
	else
		cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);

	if	(iPos	!=	s_sbPopupWindow.m_iCurrentPos)
		resetButtons();
}

//
//	드래그 스킬 처리
void 
CSkillWindow::operateDragSkill()
{
	if	(g_hero.IsExclusive(eMOUSE_STATE_SKILL_DRAG)	)
		return;

	if	(s_iFocusSkill	==	0xffff	&&	s_iDragSkill	==	0xffff)
		return;
	
	if (s_isLeftButtonPressed	&&	s_iDragSkill	==	0xffff	)
	{
		if (g_hero.m_aAbility[s_iFocusSkill].m_wLevel != 0)
		{
			s_iDragSkill	=	s_iFocusSkill;

			g_hero.SetExclusive(eMOUSE_STATE_SKILL_DRAG);
		}
	}

	if (s_isLeftButtonPressed	||s_iDragSkill	==	0xffff	)
		return;

	int	iDroppingSlot	=	getSkillDroppingSlot();

	if (iDroppingSlot	<=	eSKILL_SLOT_QUICK10)
	{
		BOOL	bIsWorkSucss=	FALSE;
		int		iAbility	=	s_iDragSkill;

		if	(s_iDragSkill	>=	dBORDER_OF_BOTTOM_SKILL)
			iAbility		=	g_hero.getQuickSlotSkill(s_iDragSkill-dBORDER_OF_BOTTOM_SKILL);

		cAbility	*lpAbility	=	&g_hero.m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpAbility->m_wLevel	<=	0)
			g_lpHero->warning(dMSG_NOT_LEARNED_SKILL);
		else
		if	(!lpAbility->isEnableJob(g_hero.m_wJob))
			g_lpHero->warning(dMSG_NOT_THIS_JOB_SKILL);
		else if(lpSkill && lpSkill->m_wType ==	eSKILL_TYPE_PASSIVE)
		{
			g_lpHero->warning(dMSG_CAN_DO_NOT_PASSIVE_SKILL);
		}		
		else
		switch(iDroppingSlot)
		{
			case	eSKILL_SLOT_LEFT		:
				if	(lpSkill->isEnableLeftSkill())
				{
					if	(s_iDragSkill	>=	dBORDER_OF_BOTTOM_SKILL)
						g_hero.setQuickSlotSkill(s_iDragSkill-dBORDER_OF_BOTTOM_SKILL,0xffff);

					g_hero.setLeftAbility(iAbility);
					bIsWorkSucss	=	TRUE;
				}
				else
					g_lpHero->warning(dMSG_THAT_SKILL_NOT_LEFT_BUTTON_SKILL);
				break;

			case	eSKILL_SLOT_RIGHT		:
				if	(lpSkill->isEnableRightSkill())
				{
					if	(s_iDragSkill	>=	dBORDER_OF_BOTTOM_SKILL)
						g_hero.setQuickSlotSkill(s_iDragSkill-dBORDER_OF_BOTTOM_SKILL,0xffff);

					g_hero.setRightAbility(iAbility);
					bIsWorkSucss	=	TRUE;
				}
				else
					g_lpHero->warning(dMSG_THAT_SKILL_NOT_RIGHT_BUTTON_SKILL);
				break;

			case	eSKILL_SLOT_SUB				:
				if	(lpSkill->isEnableSubSkill())
				{
					if	(s_iDragSkill	>=	dBORDER_OF_BOTTOM_SKILL)
						g_hero.setQuickSlotSkill(s_iDragSkill-dBORDER_OF_BOTTOM_SKILL,0xffff);

					g_hero.setSubAbility(iAbility);
					bIsWorkSucss	=	TRUE;
				}
				else
					g_lpHero->warning(dMSG_THAT_SKILL_NOT_SUB_SKILL);
				break;

			case	eSKILL_SLOT_SUPPORT_MAGIC	:
				if (lpSkill->isSupportSkill())
				{
					if (s_iDragSkill	>=	dBORDER_OF_BOTTOM_SKILL)
						g_hero.setQuickSlotSkill(s_iDragSkill-dBORDER_OF_BOTTOM_SKILL,0xffff);

					g_hero.setSupportAbility(iAbility);
					bIsWorkSucss	=	TRUE;
				}
				else
					g_lpHero->warning(dMSG_THAT_SLOT_ARE_SUPPORT_SKILL_SLOT);
				break;

			case	eSKILL_SLOT_QUICK1		:
			case	eSKILL_SLOT_QUICK2		:
			case	eSKILL_SLOT_QUICK3		:
			case	eSKILL_SLOT_QUICK4		:
			case	eSKILL_SLOT_QUICK5		:
			case	eSKILL_SLOT_QUICK6		:
			case	eSKILL_SLOT_QUICK7		:
			case	eSKILL_SLOT_QUICK8		:
			case	eSKILL_SLOT_QUICK9		:
			case	eSKILL_SLOT_QUICK10		:
				if	(s_iDragSkill	>=	dBORDER_OF_BOTTOM_SKILL)
					g_hero.setQuickSlotSkill(s_iDragSkill-dBORDER_OF_BOTTOM_SKILL,0xffff);

				g_hero.setQuickSlotAbility(iAbility,iDroppingSlot-eSKILL_SLOT_QUICK1);
				break;
		}

		if	(bIsWorkSucss)
			iDroppingSlot	=	0xffff;
	}
	else
	{
		if	(s_iDragSkill	>=	1004)
			g_hero.setQuickSlotSkill(s_iDragSkill-dBORDER_OF_BOTTOM_SKILL,0xffff);
		else
		if	(s_iDragSkill	>=	dBORDER_OF_BOTTOM_SKILL)
			g_hero.setQuickSlotSkill(s_iDragSkill-dBORDER_OF_BOTTOM_SKILL,0xffff);
	}

	s_iDragSkill	=	0xffff;

	cINPDEV::ReleaseExclusive();
}

void
CSkillWindow::operatePopupMenu()
{
	if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;

	if	(!s_isTouchRightButton)
		return;

	if	(s_iFocusSkill	<	0	||	s_iFocusSkill	>=	dBORDER_OF_BOTTOM_SKILL	)
		return;

	g_gwEasySkillTree.open(s_iFocusSkill);

	return;
/*
	if (s_pmCommon.isPopuped())
	{
		s_pmCommon.reset();

		return;
	}

	s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
	s_ttCommon.hide();

	s_pmCommon.addMenu(ePM_SKILL_TREE	,dMSG_OPEN_SKILL_TREE);
	g_gwEasySkillTree.m_iBookedAbility	=	s_iFocusSkill;

	if (s_pmCommon.getCount())
		cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
	else
		s_pmCommon.reset();
*/
}

//
//	아이템 인터페이스 업데이트
void
CSkillWindow::update()
{
	if	(s_iPopupInterface	!=	ePIW_SKILL)
		return;

	g_gwEasySkillTree.update();

	operatePopupMenu();	//	아이템 윈도우 스크롤바 업데이트

	{
		int	iCount	=	0,i,iLineCount;

		for (i=0;i < dABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&g_hero.m_aAbility[i];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if (!lpSkill)
				continue;
			if (!lpSkill->isEnableJob(m_iSelectJobTab))
				continue;

			iCount++;
		}

		iLineCount	=	iCount/2+iCount%2;

		s_sbPopupWindow.setSize(iLineCount,10);
	}

	operateSkillPoint();
	operateScrollBar();
	operateDragSkill();

	if	(cINPDEV::IsExclusive())
		return;

//	포커스 스킬을 구한다.
	if	(s_iDragSkill	==	0xffff)
	{
		s_iFocusSkill	=	0xffff;

		if	(IsValidFocus())
			s_iFocusSkill	=	getFocusSkill();

		if	(s_iFocusSkill	!=	0xffff)
		{
			cAbility	ability;

			ability.copy(&g_hero.m_aAbility[s_iFocusSkill]);

			if	(ability.isEnableJob(g_hero.m_wJob))
			{
				int			iCorrectLevel=	0;

				if	(ability.isEnableJob(g_hero.m_wJob))
					iCorrectLevel		=	g_hero.getCorrectSkillLevel(&ability);

				ability.m_wLevel	+=	iCorrectLevel;

				char	*strComment	=	ability.getTooltip(iCorrectLevel);

				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);

				if	(g_config.m_wIsViewLargeSkillIcon == FALSE)
				{
					WORD	*lpImage	=	g_smiIconSkill.get16Image(ability.getSkill()->m_wIconIndex);
					s_ttCommon.setImage(lpImage);
				}
			}
			else
			{
				g_hero.m_bIsEmulation=	TRUE;
				g_hero.m_wJob		=	g_hero.getAnotherJob();
				g_hero.buildPower(FALSE, FALSE, FALSE);

				int	iCorrectLevel	=	g_hero.getCorrectSkillLevel(&ability);
				ability.m_wLevel	+=	iCorrectLevel;

				char	*strComment	=	ability.getTooltip(iCorrectLevel);

				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);

				if	(g_config.m_wIsViewLargeSkillIcon == FALSE)
				{
					WORD	*lpImage	=	g_smiIconSkill.get16Image(ability.getSkill()->m_wIconIndex);
					s_ttCommon.setImage(lpImage);
				}

				g_hero.m_wJob	=	g_hero.getAnotherJob();
				g_hero.buildPower();
				g_hero.m_bIsEmulation=	FALSE;
			}
		}

		if	(s_iFocusSkill	!=	0xffff	&&	s_isLeftButtonPressed)
			if	(g_hero.m_aAbility[s_iFocusSkill].m_wLevel != 0)
			{
				s_iDragSkill	=	s_iFocusSkill;

				g_hero.SetExclusive(eMOUSE_STATE_SKILL_DRAG);
			}
	}

//	 리액션 스킬 상태
	for (int i=0;i<m_bmReactionSkillState.m_wCount;i++)
	{
		cBAR		*lpBar		=	&m_bmReactionSkillState.m_pMenu[i];
		int			iAbility	=	lpBar->m_dwMenu;
		cAbility	*lpAbility	=	&g_hero.m_aAbility[iAbility];

		if	(!lpAbility->getSkill())
			continue;

		if	(g_hero.isActiveReaction(iAbility))
			m_bmReactionSkillState.setImageBar(iAbility,eREACTION_SKILL_ON,eREACTION_SKILL_ON,eREACTION_SKILL_ON);
		else
			m_bmReactionSkillState.setImageBar(iAbility,eREACTION_SKILL_OFF,eREACTION_SKILL_OFF_LIGHT,eREACTION_SKILL_OFF_LIGHT);
	}
}

//
//	스킬 정보
void
CSkillWindow::draw()
{
	int		iX,iY,i,iCount=0;

	int		iTabHeight	=	g_sprInterface.getSpriteHeight(eACTIVE_SKILL_TAB);
	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eSKILL_WINDOW);
	int		iWindowHeight=	g_sprInterface.getSpriteHeight(eSKILL_WINDOW);

	iX		=	m_posSkillWindow.x;
	iY		=	m_posSkillWindow.y;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+42,iX+4+iWindowWidth-8,iY+iWindowHeight-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+42,iX+4+iWindowWidth-8,iY+iWindowHeight-8);

	g_sprInterface.Put(iX,iY,eSKILL_WINDOW);

	// ToolTip
	char	strToolTip[1024];
	cRECT	rect = {iX + 240,g_iCorrectBI_Y/2,iX + 256,16+g_iCorrectBI_Y/2};
	
	if (rect.isIn(s_posMouse.x,s_posMouse.y))
	{
		sprintf(strToolTip,dMSG_CHANGE_VIEW_TYPE);
		s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
	}

//	스킬 포인트
	s_text.setShadowMargin(1);

	if	(g_hero.m_bf4CH5SkillBonusCount)
	{
		cRECT	rect;

		rect.set(iX+10,iY+iWindowHeight-20,iX+iWindowWidth-20,iY+iWindowHeight-4);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			char	strTT[256];

			sprintf(strTT,dMSG_RECEIVE_CH5_SKILL_POINT_COUONT_FORM,g_hero.m_bf4CH5SkillBonusCount,g_hero.m_bf4CH5SkillBonusCount*100);

			rect.x1	-=	10;
			rect.y1	-=	5;
			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strTT,eTFM_LEFT);
		}

	}
	s_text.put(iX+10,iY+iWindowHeight-20,WHITE,"%s <c:LTYELLOW>%d<n>",dMSG_SKILL_POINT,g_hero.m_dwSkillExperience);
	s_text.putRF(iX+iWindowWidth-6,iY+iWindowHeight-20,WHITE,dMSG_OPEN_SKILL_TREE_HELP);
	s_text.setShadowMargin(2);

	iX		=	m_posSkillWindow.x+9;
	iY		=	m_posSkillWindow.y+47;

	s_sText.disableFitRightEdge();
	s_sText.setLineGap(0);

	int	iSkipSkillCount	=	s_sbPopupWindow.m_iCurrentPos*2,iPutCount=0;

	if (g_config.m_wIsViewLargeSkillIcon)
	{
		iY		-=	2;
		for (i=0;i < dABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&g_hero.m_aAbility[i];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if	(!lpSkill)
				continue;
			if	(!lpSkill->isEnableJob(m_iSelectJobTab))
				continue;
			if	(iCount	<	iSkipSkillCount)
			{
				iCount++;
				continue;
			}

			int	iPutX	=	iX+(iPutCount%2)*125;
			int	iPutY	=	iY+(iPutCount/2)*dSKILL_ITEM_HEIGHT;

			g_sprInterface.Put(iPutX,iPutY,eWINDOW_SKILL_SLOT_RED+lpSkill->m_wCastMethod);
			g_smiIconSkill.put(lpSkill->m_wIconIndex,iPutX+2,iPutY+5);

			if	(lpAbility->m_wLevel	==	0)
			{
				s_sText.put(iPutX+44,iPutY+7,82,42,0,GRAY,lpSkill->m_strName);
				cDRAW::FillAB(0,iPutX+2,iPutY+5,iPutX+2+dITEM_SHAPE_WIDTH-1,iPutY+5+dITEM_SHAPE_HEIGHT-1,20);
			}
			else
				s_sText.put(iPutX+44,iPutY+7,82,42,0,WHITE,lpSkill->m_strName);

			int	iCorrectLevel	=	0;

			if	(lpAbility->isEnableJob(g_hero.m_wJob))
				iCorrectLevel	=	g_hero.getCorrectSkillLevel(lpAbility);

			if	(iCorrectLevel && lpAbility->m_wLevel)
				s_sText.put(iPutX+44,iPutY+30,RGB(219,192,100),"Lv <c:LTGREEN>%d<n>/%d",lpAbility->m_wLevel+iCorrectLevel,lpAbility->m_wLevel);
			else
				s_sText.put(iPutX+44,iPutY+30,RGB(219,192,100),"Lv <c:WHITE>%d<n>",lpAbility->m_wLevel);

			iCount++;
			iPutCount++;
			if	(iPutCount	>=	(iWindowHeight-21-46)/dSKILL_ITEM_HEIGHT*2)
				break;
		}
	}
	else
	{
		for (i=0;i < dABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&g_hero.m_aAbility[i];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if	(!lpSkill)
				continue;
			if	(!lpSkill->isEnableJob(m_iSelectJobTab))
				continue;

			int		iPutX	=	iX;
			int		iPutY	=	iY+iPutCount*17;
			DWORD	dwColor;

			g_smiIconSkill.putScale(lpSkill->m_wIconIndex,iPutX+2,iPutY,45);
//			cDRAW::XLineHB(0,iPutX+20,g_iScreenWidth-10,iPutY+17-1);

			if (lpAbility->m_wLevel	==	0)
			{
				dwColor	=	GRAY;

				if	(i==s_iFocusSkill)
					dwColor	=	YELLOW;

				s_text.put(iPutX+20,iPutY+2,dwColor	,lpSkill->m_strName);
			}
			else
			{
				dwColor	=	WHITE;

				if	(i==s_iFocusSkill)
					dwColor	=	LTYELLOW;

				int	iCorrectLevel	=	0;

				if	(lpAbility->isEnableJob(g_hero.m_wJob))
					iCorrectLevel	=	g_hero.getCorrectSkillLevel(lpAbility);

				if	(iCorrectLevel)
					s_text.put(iPutX+20,iPutY+2,dwColor	,"%s(Lv <c:LTGREEN>%d<n>/%d)",lpSkill->m_strName,lpAbility->m_wLevel+iCorrectLevel,lpAbility->m_wLevel);
				else
					s_text.put(iPutX+20,iPutY+2,dwColor	,"%s(Lv %d)",lpSkill->m_strName,lpAbility->m_wLevel);
			}

			iCount++;
			iPutCount++;
		}
	}

	s_sText.setLineGap(2);
#ifndef	_FOR_ENG
	s_sText.enableFitRightEdge();
#endif

	s_bmPlusState.draw();
	m_bmReactionSkillState.draw();
	m_bmJobTab.draw();
	s_bmButton.draw();

	if	(g_config.m_wIsViewLargeSkillIcon)
		s_sbPopupWindow.draw();
}