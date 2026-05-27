#include "CWindowInterface.H"
#include "hero_mini_pet.H"
#include "cMessage.H"

const	int	c_iMiniPetIconSize		=	28;
const	int	c_iMiniPetSkillIconSize	=	16;
const	int	c_iMiniPetSkillHeight	=	18;
const	int	c_iPutMiniPetIconWidth	=	31;
const	int	c_iPutMiniPetIconHeight	=	31;
const	int	c_iMiniPetIconBoxThick	=	2;
const	int	c_iMiniPetSkillPosX		=	14;
const	int	c_iMiniPetSkillPosY		=	253;

cMiniPetStatus	g_gwMiniPetStatus;

enum
{
	eMPWM_BEGIN		,
	eMPWM_ACTIVATE	,
	eMPWM_DISACTIVATE,
	eMPWM_CHANGE_NAME,
	eMPWM_BLOCK_LIGHT_POCKET,
	eMPWM_RELEASE,

	eMPWM_PLACE_TO_LEFT,
	eMPWM_PLACE_TO_RIGHT,
	eMPWM_PLACE_TO_UP,
	eMPWM_PLACE_TO_DOWN,
	eMPWM_ON_OFF_1,
	eMPWM_ON_OFF_2,
	eMPWM_ON_OFF_3,
	eMPWM_ON_OFF_4,
	eMPWM_ON_OFF_5,
	eMPWM_ON_OFF_6,
	eMPWM_ON_OFF_7,
	eMPWM_ON_OFF_8,
	eMPWM_ON_OFF_9,
	eMPWM_ON_OFF_10,
	eMPWM_ON_OFF_11,

	eMPWM_ONLY_ONE_MINI_PET_BORDER,	//	요 밑에 메뉴는 하나의 미니펫만 선택해야 가능하다.
};

BOOL
cMiniPetStatus::isIncludeActiveMiniPetInSelectedList()
{
	for (int i=m_iSelectMinipetCount;i;)
	{
		--i;

		if	(m_aiMultiSelect[i]	==	g_hero.m_wActiveMiniPet2)
			return	TRUE;
		if	(m_aiMultiSelect[i]	==	g_hero.m_wActiveMiniPet)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
cMiniPetStatus::isSelectMiniPet(int _iIndex)
{
	for (int i=m_iSelectMinipetCount;i;)
	{
		--i;

		if	(m_aiMultiSelect[i]	==	_iIndex)
			return	TRUE;
	}

	return	FALSE;
}

int
cMiniPetStatus::getFirstSelectMiniPet()
{
	if	(m_iSelectMinipetCount	==	0)
		return	-1;

	return	m_aiMultiSelect[0];
}

BOOL
cMiniPetStatus::addSelectMiniPet(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>	c_iMiniPetCount)
		return	FALSE;

	if	(isSelectMiniPet(_iIndex))
	{
		for (int i=m_iSelectMinipetCount;i;)
		{
			--i;

			if	(m_aiMultiSelect[i]	==	_iIndex)
			{
				if	(m_iSelectMinipetCount	>	1)
					m_aiMultiSelect[i]	=	m_aiMultiSelect[m_iSelectMinipetCount-1];

				m_iSelectMinipetCount--;

				break;
			}
		}

		if	(m_iSelectMinipetCount	==	0)
		{
			m_iSelectMiniPet	=	_iIndex;
			m_aiMultiSelect[m_iSelectMinipetCount++]	=	_iIndex;
		}

		return	FALSE;
	}

	m_iSelectMiniPet	=	_iIndex;
	m_aiMultiSelect[m_iSelectMinipetCount++]	=	_iIndex;

	return	TRUE;
}

BOOL
cMiniPetStatus::selectMiniPet(int _iIndex,BOOL _bIsAdd)
{
	if	(_bIsAdd	==	FALSE)
		m_iSelectMinipetCount	=	0;

	addSelectMiniPet(_iIndex);

	if	(isSelectMiniPet(m_iSelectMiniPet)	==	FALSE)
		m_iSelectMiniPet	=	getFirstSelectMiniPet();

	m_lpSelectMiniPet	=	g_hero.iMiniPet()->getMiniPet(m_iSelectMiniPet);

	return	TRUE;
}

void
cMiniPetStatus::open()
{
	if	(g_hero.iMiniPet()->getMiniPetCount() == 0)
		return;

	m_iSelectMinipetCount	=	0;

	selectMiniPet(g_hero.m_wActiveMiniPet);

	if	(m_iSelectMiniPet	==	c_iMiniPetCount)
		selectMiniPet(g_hero.m_wActiveMiniPet2);

	if	(m_iSelectMiniPet	==	c_iMiniPetCount)
		selectMiniPet(g_hero.iMiniPet()->getFistAvailMiniPetIndex());

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowTitleHeight	=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_PET);
	int	iBarWidth			=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL)+2;

	s_rectInterfaceSize.x1	=	g_iScreenWidth-(iWindowWidth*2);
	s_rectInterfaceSize.x2	=	g_iScreenWidth;

	m_rect.x1				=	g_iScreenWidth-iWindowWidth*2;
	m_rect.y1				=	iWindowTitleHeight+g_iCorrectBI_Y/2;
	m_rect.x2				=	g_iScreenWidth-iWindowWidth;
	m_rect.y2				=	iWindowTitleHeight+iWindowHeight+g_iCorrectBI_Y/2;

	if	(g_hero.m_bf6ExtraInventorySize)
	{
		s_rectInterfaceSize.x1 += 102;		
		m_rect.add(-96,0);
	}

	s_bmMiddleWindowButton.reset();

	CPos	pos;

	pos.x	=	m_rect.x1+12;
	pos.y	=	m_rect.y1+153;
	s_bmMiddleWindowButton.addImageBar(dMSG_ACTIVATE				,eMPWM_ACTIVATE				,pos.x,pos.y,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	s_bmMiddleWindowButton.addImageBar(dMSG_DISACTIVATE				,eMPWM_DISACTIVATE			,pos.x,pos.y,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	s_bmMiddleWindowButton.addImageBar(dCHANGE_NAME					,eMPWM_CHANGE_NAME			,pos.x+iBarWidth,pos.y,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	s_bmMiddleWindowButton.addImageBar(dMSG_BLOCK_LIGHT_POCKET		,eMPWM_BLOCK_LIGHT_POCKET	,pos.x+iBarWidth*2,pos.y,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);
	s_bmMiddleWindowButton.addImageBar(dMSG_RELEASE_SUMMONED_BEAST	,eMPWM_RELEASE				,pos.x+iBarWidth*3,pos.y,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

	pos.x	=	m_rect.x1+218;
	pos.y	=	m_rect.y1+79;

	s_bmMiddleWindowButton.addImageBar("",eMPWM_PLACE_TO_LEFT			,pos.x,pos.y,
											eSCROLLBAR_HORIZON_NORMAL_LEFT_BUTTON,	eSCROLLBAR_HORIZON_ACTIVE_LEFT_BUTTON,	eSCROLLBAR_HORIZON_PRESSED_LEFT_BUTTON);
	s_bmMiddleWindowButton.addImageBar("",eMPWM_PLACE_TO_RIGHT			,pos.x+26,pos.y,
											eSCROLLBAR_HORIZON_NORMAL_RIGHT_BUTTON,	eSCROLLBAR_HORIZON_ACTIVE_RIGHT_BUTTON,	eSCROLLBAR_HORIZON_PRESSED_RIGHT_BUTTON);
	s_bmMiddleWindowButton.addImageBar("",eMPWM_PLACE_TO_UP				,pos.x+14,pos.y-7,
											eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_UP_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_UP_BUTTON);
	s_bmMiddleWindowButton.addImageBar("",eMPWM_PLACE_TO_DOWN			,pos.x+14,pos.y+7,
											eSCROLLBAR_VERTICAL_NORMAL_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_DOWN_BUTTON);

	for (int i=0;i < c_iMiniPetSkillCount;i++)
	{
		s_bmMiddleWindowButton.addImageBar	("",eMPWM_ON_OFF_1+i,0,0,eREACTION_SKILL_ON,eREACTION_SKILL_ON,eREACTION_SKILL_OFF);
		s_bmMiddleWindowButton.hide(eMPWM_ON_OFF_1+i);
	}

	s_bmMiddleWindowButton.hide(eMPWM_DISACTIVATE);
	s_bmMiddleWindowButton.setClicktAllBarMargin(1,1);
	s_bmMiddleWindowButton.setAllBarTextColor(0,FALSE);

	m_bIsChangeSkillWork	=	FALSE;

	resetButtons();
	resetButtonsPos();
}

void
cMiniPetStatus::close()
{

}

void
cMiniPetStatus::modifyInfo()
{
	resetButtonsPos();
}

void
cMiniPetStatus::update()
{
	cHero_mini_pet	*lpIHeroForMiniPet	=	g_hero.iMiniPet();

	if	(lpIHeroForMiniPet->getMiniPetCount() == 0	||	g_config.m_bf1IsShowMiniPetWindow	==	FALSE)
		return;

	if	(m_bIsChangeSkillWork)
	{
		m_bIsChangeSkillWork	=	FALSE;
		resetButtonsPos();
	}

	if	(m_lpSelectMiniPet)
	{
		if	(lpIHeroForMiniPet->getBlockLightPocketCount()	==	0)
			s_bmMiddleWindowButton.disable(eMPWM_BLOCK_LIGHT_POCKET);
		else
			s_bmMiddleWindowButton.enable(eMPWM_BLOCK_LIGHT_POCKET);

		if	(stricmp(m_lpSelectMiniPet->getName(),"noname")!=0)
			s_bmMiddleWindowButton.disable(eMPWM_CHANGE_NAME);
		else
			s_bmMiddleWindowButton.enable(eMPWM_CHANGE_NAME);
	}

	if	(IsExclusive())
		return;

	m_iFocusMiniPet		=	getFocusMiniPet();

	if	(m_iFocusMiniPet!=	-1)
	{
		cMiniPet	*lpMiniPet	=	lpIHeroForMiniPet->getMiniPet(m_iFocusMiniPet);

		if	(lpMiniPet)
			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,lpMiniPet->getToolTip());
	}

	if	(!m_lpSelectMiniPet)
	{
		for	(int i=0;i<c_iMiniPetCount;i++)
		{
			if	(lpIHeroForMiniPet->m_aMiniPet[i].m_bf7Level)
			{
				selectMiniPet(i);

				resetButtonsPos();

				break;
			}
		}
	}

	if	(m_lpSelectMiniPet)
		m_iFocusSkill		=	getFocusSkill();
	else
		m_iFocusSkill		=	-1;

	if	(m_iFocusMiniPet	!=	-1)
	{
		if	(s_isTouchLeftButton)
		{
			selectMiniPet(m_iFocusMiniPet,s_aKey[KEY_LSHIFT]);

			resetButtonsPos();

			return;
		}

		if	(s_isTouchRightButton && m_iSelectMiniPet != -1 && g_iSeasonVariable > dSEASON_VARIABLE_MINIPET_FEED_INTERFACE)
		{
			int iMaxGlowType = m_lpSelectMiniPet->getMaxGrowFactor();

			if(iMaxGlowType != nsMiniPetStyle::Deault)
			{
				g_hero.getMinipetFeedTypeCount(iMaxGlowType , FALSE);

				char strMessage[512];
				
				sprintf(strMessage,_ms(dMSG_FEED_ALL_ITEM_BY_TYPE, g_pstrCommonMessage[1803 + iMaxGlowType - 1]//형태를 나타내는 텍스트.
					, m_lpSelectMiniPet->m_strName)); // 1803 : dMSG_MINIPET_FEED_TYPE_BIO 

				m_iFeedType = iMaxGlowType;
				
				g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_FEED_ALL_ITEM_TO_MINIPET);
			}
		}
	}

	if	(m_iFocusSkill		!=	-1)
	{
		cAbility	ability;

		cMiniPet	*lpMiniPet	=	lpIHeroForMiniPet->getMiniPet(m_iSelectMiniPet);

		ability.set(lpMiniPet->m_aAbility[m_iFocusSkill].m_bf11Skill,lpMiniPet->m_aAbility[m_iFocusSkill].m_bf4Level);

		lpIHeroForMiniPet->m_lpMiniPet	=	lpMiniPet;

		CSkill		*lpSkill	=	ability.getSkill();

		int		iAddLevel		=	0;

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
		{
			iAddLevel	+=	g_hero.m_bIncreaseMiniPetBasicSkillLevel;
			iAddLevel	+=	g_hero.m_bCorrectMiniPetBasicSkillLevel;
		}

		iAddLevel	+=	g_hero.m_bIncreaseMiniPetSkillLevel;
		iAddLevel	+=	g_hero.m_bCorrectMiniPetSkillLevel;

		ability.m_wLevel	+=	iAddLevel;
		char	*strComment	=	ability.getTooltip(iAddLevel , TRUE , g_hero.m_iLevel);

		s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment);
	}

	if	(m_rectReversibleFrame.isIn(s_posMouse.x,s_posMouse.y))
	{
		if	(lpIHeroForMiniPet->isAvailReversibleFrame())
		{
			char	strText[256];

			sprintf(strText,dMSG_REVERSIBLE_FRAME_COMMENT_FORM,
					g_hero.m_wReversalFrameYear+2000,g_hero.m_wReversalFrameMonth,g_hero.m_wReversalFrameDay,
					g_hero.m_wReversalFrameHour,g_hero.m_wReversalFrameMinute);

			cRECT	rect;

			rect.set(&m_rectReversibleFrame);
			rect.add(0,16);

			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strText);
		}
	}

	if	(m_rectCombiSkill.isIn(s_posMouse.x,s_posMouse.y))
	{
		if	(g_hero.m_wMiniPetCombiSkill	!=	0xffff)
		{
			char	strText[256];

			sprintf(strText,dMSG_MINIPET_COMBI_SKILLCOMMENT_FORM,
				g_aSkill[g_hero.m_wMiniPetCombiSkill].m_strName,g_aSkill[g_hero.m_wMiniPetCombiSkill].m_strComment);
		
			cRECT	rect;

			rect.set(&m_rectCombiSkill);
			rect.add(0,16);

			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strText);
		}

	}

	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	s_rectInterfaceSize.x1	=	g_iScreenWidth-(iWindowWidth*2);
	s_rectInterfaceSize.x2	=	g_iScreenWidth;

	if	(g_hero.m_bf6ExtraInventorySize)
		s_rectInterfaceSize.x1 -= 102;

	if	(m_lpSelectMiniPet	&&	m_lpSelectMiniPet->m_bf7Level	==	0)
		m_lpSelectMiniPet	=	NULL;

	if	(!m_lpSelectMiniPet)
		return;

	if	(lpIHeroForMiniPet->getBlockLightPocketCount()	==	0)
		s_bmMiddleWindowButton.disable(eMPWM_BLOCK_LIGHT_POCKET);
	else
		s_bmMiddleWindowButton.enable(eMPWM_BLOCK_LIGHT_POCKET);

	if	(stricmp(m_lpSelectMiniPet->getName(),"noname")!=0)
		s_bmMiddleWindowButton.disable(eMPWM_CHANGE_NAME);
	else
		s_bmMiddleWindowButton.enable(eMPWM_CHANGE_NAME);

	if	(lpIHeroForMiniPet->m_wActiveMiniPet	==	m_iSelectMiniPet	||	lpIHeroForMiniPet->m_wActiveMiniPet2	==	m_iSelectMiniPet)
	{
		s_bmMiddleWindowButton.show(eMPWM_DISACTIVATE);
		s_bmMiddleWindowButton.hide(eMPWM_ACTIVATE);
	}
	else
	{
		s_bmMiddleWindowButton.show(eMPWM_ACTIVATE);
		s_bmMiddleWindowButton.hide(eMPWM_DISACTIVATE);
	}

	int		iMenu			=	s_bmMiddleWindowButton.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if	(iMenu	>	eMPWM_BEGIN	&&	iMenu	<	eMPWM_ONLY_ONE_MINI_PET_BORDER)	//	미니펫을 여러개 선택한 경우 체크
	{
		if	(m_iSelectMinipetCount	>	1)
		{
			g_msgBox.cPopup("",dMSG_PLEASE_SELECT_ONLY_ONE_MINIPET,dMSG_OK);

			iMenu	=	-1;
		}
	}

	switch(iMenu)
	{
		case	eMPWM_DISACTIVATE			:
		{
			if	(m_iSelectMiniPet	!=	g_hero.m_wActiveMiniPet	&&	m_iSelectMiniPet	!=	g_hero.m_wActiveMiniPet2)
				return;

			s_agent.sendEtcWork(eCEW_ACTIVATE_MINI_PET,m_iSelectMiniPet,FALSE);

			break;
		}

		case	eMPWM_ACTIVATE				:
		{
			if	(m_iSelectMiniPet	==	g_hero.m_wActiveMiniPet	||	m_iSelectMiniPet	==	g_hero.m_wActiveMiniPet2)
				return;

			if(g_hero.m_wTransformationTime )
				return;

			s_agent.sendEtcWork(eCEW_ACTIVATE_MINI_PET,m_iSelectMiniPet,TRUE);

			break;
		}
		case	eMPWM_CHANGE_NAME			:
		{
			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,_ms("@%s %d %s",dMSG_CHANGE_MINIPET_NAME_KEYWORD,m_iSelectMiniPet,dMSG_PLEASE_INPUT_WANT_PET_NAME));
			s_inputChatting.setSelect(0,strlen(dMSG_CHANGE_MINIPET_NAME_KEYWORD)+4+m_iSelectMiniPet/10,-1);
			break;
		}

		case	eMPWM_BLOCK_LIGHT_POCKET	:
		{
			if	(lpIHeroForMiniPet->getBlockLightPocketCount()	==	0)
				break;

			g_msgBox.cPopup("",dMSG_USE_BLOCK_LIGHT_POCKET,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_USE_BLOCK_LIGHT_POCKET);
			break;
		}

		case	eMPWM_RELEASE				:
		{
			g_msgBox.cPopup("",dMSG_RELEASE_MINIPET_WARNING,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_PARTING_WITH_MINIPET);
			break;
		}

		case	eMPWM_PLACE_TO_LEFT:
				s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_PLACE,m_iSelectMiniPet,0);
			break;

		case	eMPWM_PLACE_TO_RIGHT:
			if	((m_iSelectMiniPet%8)	!=	7)
				s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_PLACE,m_iSelectMiniPet,1);
			break;
		case	eMPWM_PLACE_TO_UP:
			if	((m_iSelectMiniPet/8)	!=	0)
				s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_PLACE,m_iSelectMiniPet,2);
			break;
		case	eMPWM_PLACE_TO_DOWN:
			if	((m_iSelectMiniPet/8)	!=	1)
				s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_PLACE,m_iSelectMiniPet,3);
			break;
		case	eMPWM_ON_OFF_1:
		case	eMPWM_ON_OFF_2:
		case	eMPWM_ON_OFF_3:
		case	eMPWM_ON_OFF_4:
		case	eMPWM_ON_OFF_5:
		case	eMPWM_ON_OFF_6:
		case	eMPWM_ON_OFF_7:
		case	eMPWM_ON_OFF_8:
		case	eMPWM_ON_OFF_9:
		case	eMPWM_ON_OFF_10:
		case	eMPWM_ON_OFF_11:
			s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_SKILL_ON_OFF , m_iSelectMiniPet,iMenu-eMPWM_ON_OFF_1);

			break;
	}

	iMenu					=	s_bmPlusState.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed);

	if	(m_bIsWaitResult	==	FALSE	&&	iMenu	>=	0	&&	iMenu	<=	c_iMiniPetSkillCount)
	{
		s_agent.sendEtcWork(eCEW_INCREASE_MINIPET_SKILL_LEVEL,m_iSelectMiniPet,iMenu);
		m_bIsWaitResult	=	TRUE;
	}
}



void
cMiniPetStatus::draw()
{
	cHero_mini_pet	*lpIHeroForMiniPet	=	g_hero.iMiniPet();

	if	(lpIHeroForMiniPet->getMiniPetCount() == 0	||	g_config.m_bf1IsShowMiniPetWindow	==	FALSE)
		return;

	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iX		=	m_rect.x1;
	int		iY		=	m_rect.y1;
	int		i;

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillAB(0,iX+8,iY+6,iX+266-4,iY+482-8,24);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX	,iY		,eWINDOW);
	g_sprInterface2.Put(iX	,g_iCorrectBI_Y/2		,164);

	iX		+=	12;
	iY		+=	8;

	CPos	pos;

	for	(i=0;i<c_iMiniPetCount;i++)
	{
		pos.x	=	iX+(i%8)*c_iPutMiniPetIconWidth;
		pos.y	=	iY+(i/8)*c_iPutMiniPetIconHeight;

		int		iIcon	=	g_hero.m_aMiniPet[i].m_bf5Type;

		if	(g_hero.m_aMiniPet[i].m_bf7Level	==	0)
			g_sprMiniPetIcon.Put(pos.x+1,pos.y+1,73);
		else
		if	(i	==	g_hero.m_wActiveMiniPet	||	i	==	g_hero.m_wActiveMiniPet2)
			g_sprMiniPetIcon.Put(pos.x,pos.y,iIcon+48);
		else
		if	(i	==	m_iSelectMiniPet	||	isSelectMiniPet(i))
			g_sprMiniPetIcon.Put(pos.x,pos.y,iIcon);
		else
		if	(i	==	m_iFocusMiniPet)
			g_sprMiniPetIcon.Put(pos.x,pos.y,iIcon+24,100,100);
		else
		if	(g_hero.m_aMiniPet[i].m_bf7Level)
		{
			g_sprMiniPetIcon.Put(pos.x,pos.y,iIcon+24,100,100,dPUT_ALPHA_BLENDING,24);
		}
	}

	pos.x	=	iX+2;
	pos.y	=	iY+2*c_iPutMiniPetIconWidth+4;

	{
		char	strReversibleFrameText[128];

		g_sprMiniPetIcon.Put(pos.x+1,pos.y-5,74);

		if	(lpIHeroForMiniPet->isAvailReversibleFrame())
		{
			sprintf(strReversibleFrameText,dMSG_REVERSIBLE_FRAME_VALIDATE_FORM,
					g_hero.m_wReversalFrameYear+2000,g_hero.m_wReversalFrameMonth,g_hero.m_wReversalFrameDay,
					g_hero.m_wReversalFrameHour,g_hero.m_wReversalFrameMinute);

		}
		else
			strcpy(strReversibleFrameText,dMSG_INVALID_REVERSIBLE_FRAME);

		s_text.put(iX+23,pos.y-2,WHITE,strReversibleFrameText);

		int	iWidth	=	s_text.getPixelSize(strReversibleFrameText);
		m_rectReversibleFrame.set(pos.x+1,pos.y-5,iX+23+iWidth,pos.y-5+16);
	}

	{
		char	strCombiSkillText[128];

		g_sprMiniPetIcon.Put(pos.x+2,pos.y+12,72);

		if	(g_hero.m_wMiniPetCombiSkill	==	0xffff)
			strcpy(strCombiSkillText,dMSG_INVALID_MINPET_COMBI_SKILL);
		else
			strcpy(strCombiSkillText,g_aSkill[g_hero.m_wMiniPetCombiSkill].m_strName);

		s_text.put(iX+23,pos.y+15,RGB(255,198,0),strCombiSkillText);
		int	iWidth	=	s_text.getPixelSize(strCombiSkillText);
		m_rectCombiSkill.set(pos.x+2,pos.y+12,iX+23+iWidth,pos.y+28);
	}

	pos.y	+=	33;

	if	(!m_lpSelectMiniPet)
		return;

	lpIHeroForMiniPet->operateEvolutionGradeBonus(m_iSelectMiniPet);

	int		iImage	=	93;

	g_sprInterface2.put3PieceBar(pos.x+4,pos.y-2,iImage,iWindowWidth-30,100,80,dPUT_ALPHA_BLENDING,24);

	s_text.putRF(pos.x+50,pos.y,WHITE,dMSG_NAME);
	s_text.put(pos.x+55,pos.y,LTYELLOW,m_lpSelectMiniPet->getName());

	s_text.put(pos.x+iWindowWidth/2,pos.y,WHITE,"%s [<c:LTGREEN>%d<n>]",dMSG_LEVEL,m_lpSelectMiniPet->m_bf7Level);
	pos.y	+=	19;

	s_text.putRF(pos.x+50,pos.y,WHITE,dMSG_MINIPET_EVOLUTION_GRADE);
	s_text.put(pos.x+55,pos.y,YELLOW,g_aMiniPetTypeInfo[m_lpSelectMiniPet->m_bf5Type].m_strName);
	if (m_lpSelectMiniPet->m_bf1MinipetAwaken100)
		s_text.putRF(pos.x+200,pos.y,LTYELLOW,dMSG_MINI_PET_AWAKEN_ITEM100);
	else if(m_lpSelectMiniPet->m_bf1MinipetAwaken50)
		s_text.putRF(pos.x+200,pos.y,LTYELLOW,dMSG_MINI_PET_AWAKEN_ITEM50);

	pos.y	+=	14;

	s_text.putRF(pos.x+50,pos.y,WHITE,dMSG_EXP);
	s_text.put(pos.x+55,pos.y,WHITE,"[%d/%d]",m_lpSelectMiniPet->m_bf16Exp,g_aiExpTableForMiniPet[m_lpSelectMiniPet->m_bf7Level]);

	pos.y	+=	33;

	g_sprInterface.Put(pos.x-7,pos.y,eWINDOW_SEPERATE_BAR);	//	경계
	pos.y	+=	10;

	{
		s_text.putRF(pos.x+50,pos.y,WHITE,dMSG_MINIPET_GROW_RATE);
		s_text.cPut(pos.x+90,pos.y,WHITE,dMSG_MINIPET_BIO);
		s_text.cPut(pos.x+150,pos.y,WHITE,dMSG_MINIPET_ENERGY);
		s_text.cPut(pos.x+210,pos.y,WHITE,dMSG_MINIPET_MINERAL);
		pos.y	+=	13;

		s_text.cPut(pos.x+90,pos.y,LTORANGE,"%d%s",m_lpSelectMiniPet->getGrowBioFactor(),dMSG_PERCENTAGE);
		s_text.cPut(pos.x+150,pos.y,LTORANGE,"%d%s",m_lpSelectMiniPet->getGrowEnergyFactor(),dMSG_PERCENTAGE);
		s_text.cPut(pos.x+210,pos.y,LTORANGE,"%d%s",m_lpSelectMiniPet->getGrowMineralFactor(),dMSG_PERCENTAGE);
		pos.y	+=	13;

		g_sprMiniPetIcon.Put(pos.x+2,pos.y+1,72);

		char	strText[128];
		DWORD	dwColor	=	LTGRAY;

		if	(lpIHeroForMiniPet->m_bCorrectMiniPetBasicSkillLevel)
			sprintf(strText,dMSG_INCREASE_PET_SKILL_LEVEL_FORM,lpIHeroForMiniPet->m_bCorrectMiniPetBasicSkillLevel);
		else	
		if	(lpIHeroForMiniPet->m_bCorrectMiniPetSkillLevel)
			sprintf(strText,dMSG_INCREASE_BASIC_PET_SKILL_LEVEL_FORM,lpIHeroForMiniPet->m_bCorrectMiniPetSkillLevel);
		else	
		if	(lpIHeroForMiniPet->m_bCorrectMiniPetSkillActiveChance)
			sprintf(strText,dMSG_INCREASE_PET_SKILL_ACTIVE_CAHNCE_FORM,lpIHeroForMiniPet->m_bCorrectMiniPetSkillActiveChance);
		else	
		if	(lpIHeroForMiniPet->m_bCorrectMiniPetSkillDamage)
			sprintf(strText,dMSG_INCREASE_PET_SKILL_DAMAGE_FORM,lpIHeroForMiniPet->m_bCorrectMiniPetSkillDamage);
		else
		{
			sprintf(strText,"%s %s",dMSG_EVOLUTION_BONUS,dMSG_NOTHING);
			dwColor	=	GRAY;
		}

		s_text.put(pos.x+20,pos.y+3,dwColor,strText);
	}

	pos.y	+=	17;

	g_sprInterface.Put(pos.x-7,pos.y,eWINDOW_SEPERATE_BAR);	//	경계
	pos.y	+=	8;

	g_sprInterface2.put3PieceBar(pos.x+4,pos.y,iImage,iWindowWidth-30,100,80,dPUT_ALPHA_BLENDING,24);
	s_text.put(pos.x+6,pos.y+2,WHITE,"%s  <c:LTYELLOW>%d<n>",dMSG_SKILL_POINT,m_lpSelectMiniPet->m_bf7SkillPoint);

	pos.y	+=	14;

	pos.x	=	m_rect.x1+c_iMiniPetSkillPosX;
	pos.y	=	m_rect.y1+c_iMiniPetSkillPosY;

	s_bmMiddleWindowButton.draw();

	int	iPutCount	=	0;


	for (i=0;i < c_iMiniPetSkillCount;i++)
	{
		s_bmMiddleWindowButton.resetImageBar(eMPWM_ON_OFF_1+i ,"" , eMPWM_ON_OFF_1+i , 0 , 0);
		cMiniPetAbilityInfo	*lpAbility	=	&m_lpSelectMiniPet->m_aAbility[i];
		CSkill				*lpSkill	=	GetSkill(lpAbility->m_bf11Skill);

		if	(!lpSkill	||	lpAbility->m_bf11Skill	==	0)
			continue;

		int		iPutX			=	pos.x;
		int		iPutY			=	pos.y+iPutCount*c_iMiniPetSkillHeight;
		int		iSkillNameWidth	=	0;
		BOOL	bIsShowPlusButton=	TRUE;

		char	strText[64];
		int		iAddLevel		=	0;

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
		{
			iAddLevel	+=	g_hero.m_bIncreaseMiniPetBasicSkillLevel;
			iAddLevel	+=	g_hero.m_bCorrectMiniPetBasicSkillLevel;
		}

		iAddLevel	+=	g_hero.m_bIncreaseMiniPetSkillLevel;
		iAddLevel	+=	g_hero.m_bCorrectMiniPetSkillLevel;

		if	(lpAbility->m_bf4Level	==	0)
			strcpy(strText,lpSkill->m_strName);
		else
		{
			if	(iAddLevel)
				sprintf(strText,"%s(Lv %d<c:LTGREEN>+%d<n>)",lpSkill->m_strName,lpAbility->m_bf4Level,iAddLevel);
			else
				sprintf(strText,"%s(Lv %d)",lpSkill->m_strName,lpAbility->m_bf4Level);
		}

		g_sprMiniPetIcon.Put(iPutX+2,iPutY,72);

		DWORD	dwColor	=	WHITE;

		if	(lpAbility->m_bf4Level	==	0)
			dwColor	=	LTGRAY,dwColor	=	GRAY;

		s_text.put(iPutX+c_iMiniPetSkillIconSize+4,iPutY+2,dwColor,strText);
		if(lpAbility->m_bf4Level != 0)
		{
			int	iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
			int iOnOffButtonWidth = g_sprInterface.getSpriteWidth(eREACTION_SKILL_OFF);

			int		iSkillNameWidth	=	0;
			int		iAddLevel		=	0;
			
			if	(lpSkill->m_bf1IsNormalMiniPetSkill)
			{
				iAddLevel	+=	g_hero.m_bIncreaseMiniPetBasicSkillLevel;
				iAddLevel	+=	g_hero.m_bCorrectMiniPetBasicSkillLevel;
			}
			
			iAddLevel	+=	g_hero.m_bIncreaseMiniPetSkillLevel;
			iAddLevel	+=	g_hero.m_bCorrectMiniPetSkillLevel;
			
			if	(lpAbility->m_bf4Level	==	0)
				iSkillNameWidth	=	s_text.getPixelSize(lpSkill->m_strName);
			else
			{
				if	(iAddLevel)
					iSkillNameWidth	=	s_text.getPixelSize(_ms("%s(Lv %d+%d)",lpSkill->m_strName,lpAbility->m_bf4Level,iAddLevel));
				else
					iSkillNameWidth	=	s_text.getPixelSize(_ms("%s(Lv %d)",lpSkill->m_strName,lpAbility->m_bf4Level));
			}
			
			int iAddWidth = 0;

			if(m_lpSelectMiniPet->isLevelUpAbleAbility(lpAbility))
				iAddWidth =  g_sprInterface.getSpriteWidth(eSTATE_UP_NORMAL);

			int iOnOffPosX	=	iPutX + iSkillNameWidth + iAddWidth + 22;
			int iOnOffPosY	=	iPutY + 1;

			s_bmMiddleWindowButton.setCorrectPos(eMPWM_ON_OFF_1+i, iOnOffPosX, iPutY + 1);
			s_bmMiddleWindowButton.show(eMPWM_ON_OFF_1+i);

			WORD wImage	= eREACTION_SKILL_ON;

			if(m_lpSelectMiniPet->m_aAbility[i].m_bf1IsOFF)
				wImage	= eREACTION_SKILL_OFF;
			s_bmMiddleWindowButton.setImageBar(eMPWM_ON_OFF_1+i , wImage	,wImage	,wImage	);
		}

		iPutCount++;
	}
	s_bmMiddleWindowButton.draw();
	s_bmPlusState.draw();
	pos.y	=	m_rect.y1+iWindowHeight-32;	

	g_sprInterface.Put(pos.x-7,pos.y,eWINDOW_SEPERATE_BAR);	//	경계
	pos.y	+=	4;

	CTimeInfo	currentTime,expireTime;

	currentTime			=	s_currentTime;
	expireTime.m_wYear	=	m_lpSelectMiniPet->m_bf5ExpireYear+2000;
	expireTime.m_wMonth	=	m_lpSelectMiniPet->m_bf4ExpireMonth;
	expireTime.m_wDay	=	m_lpSelectMiniPet->m_bf5ExpireDay;
	expireTime.m_wHour	=	m_lpSelectMiniPet->m_bf5ExpireHour;
	expireTime.m_wMinute=	m_lpSelectMiniPet->m_bf6ExpireMinute;

	int	iCurrentValue	=	currentTime.getMinuteValue(2000);
	int	iExpireValue	=	expireTime.getMinuteValue(2000);

	int	iRemainTime		=	max(iExpireValue-iCurrentValue,0);
	int	iDay			=	iRemainTime/24/60;
	int	iHour			=	(iRemainTime/60)%24;
	int	iMinute			=	iRemainTime%60;

	s_text.put(pos.x,pos.y+5,WHITE,dMSG_MINIPET_VALIDATE_TIME_FORM,iDay,iHour,iMinute);
}


BOOL
cMiniPetStatus::isNeedOnOffButtonSkill(CSkill* _lpSkill)
{
	for (int i=0;i<dEXTRA_EFFECT_COUNT;i++)
	{
		CSkillExtraEffect		*lpExtraEffect	=	&_lpSkill->m_aExtraEffect[i];

		if(lpExtraEffect->m_wEffect == eSKILL_EE_ITEM_COLLECTOR)
			return TRUE;
	}	

	return FALSE;
}
//
//	버튼들을 초기화
void
cMiniPetStatus::resetButtons()
{
	int	i;

	s_bmPlusState.reset();


	for (i=0;i < c_iMiniPetSkillCount;i++)
	{
		s_bmPlusState.addImageBar			("",i,0,0,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		s_bmMiddleWindowButton.resetImageBar(eMPWM_ON_OFF_1+i ,"" , eMPWM_ON_OFF_1+i, 0 , 0);
		
		s_bmMiddleWindowButton.addImageBar	("",eMPWM_ON_OFF_1+i,0,0,eREACTION_SKILL_ON,eREACTION_SKILL_ON,eREACTION_SKILL_OFF);
		s_bmMiddleWindowButton.hide(eMPWM_ON_OFF_1+i);
	}
}

//  
//	버튼들 위치 설정
void
cMiniPetStatus::resetButtonsPos()
{
	int	iX	=	m_rect.x1+c_iMiniPetSkillPosX;
	int	iY	=	m_rect.y1+c_iMiniPetSkillPosY;

	iX		+=	c_iMiniPetSkillIconSize;

	int	i,iPutCount=0;

	for (i=0;i < c_iMiniPetSkillCount;i++)
		s_bmPlusState.hide(i);

	if	(!m_lpSelectMiniPet)
		return;

	for (i=0;i < c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_lpSelectMiniPet->m_aAbility[i];
		CSkill				*lpSkill	=	GetSkill(lpAbility->m_bf11Skill);

		if	(!lpSkill	||	lpAbility->m_bf11Skill	==	0)
			continue;

		int		iPutX			=	iX;
		int		iPutY			=	iY+iPutCount*c_iMiniPetSkillHeight;
		int		iSkillNameWidth	=	0;
		BOOL	bIsShowPlusButton=	TRUE;
		int		iAddLevel		=	0;

		if	(lpSkill->m_bf1IsNormalMiniPetSkill)
		{
			iAddLevel	+=	g_hero.m_bIncreaseMiniPetBasicSkillLevel;
			iAddLevel	+=	g_hero.m_bCorrectMiniPetBasicSkillLevel;
		}

		iAddLevel	+=	g_hero.m_bIncreaseMiniPetSkillLevel;
		iAddLevel	+=	g_hero.m_bCorrectMiniPetSkillLevel;

		if	(lpAbility->m_bf4Level	==	0)
			iSkillNameWidth	=	s_text.getPixelSize(lpSkill->m_strName);
		else
		{
			if	(iAddLevel)
				iSkillNameWidth	=	s_text.getPixelSize(_ms("%s(Lv %d+%d)",lpSkill->m_strName,lpAbility->m_bf4Level,iAddLevel));
			else
				iSkillNameWidth	=	s_text.getPixelSize(_ms("%s(Lv %d)",lpSkill->m_strName,lpAbility->m_bf4Level));
		}

		s_bmPlusState.resetImageBar(i,"",i,iPutX+iSkillNameWidth+6,iPutY+1,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED);

		if	(m_lpSelectMiniPet->isLevelUpAbleAbility(lpAbility))
		{
			if	(!s_bmPlusState.isShow(i))
				s_bmPlusState.show(i);
		}
		else
			s_bmPlusState.hide(i);

		iPutCount++;
	}
}

int
cMiniPetStatus::getFocusSkill()
{
	CPos	pos;

	pos.x	=	m_rect.x1+c_iMiniPetSkillPosX;
	pos.y	=	m_rect.y1+c_iMiniPetSkillPosY;

	int	iPutCount	=	0;
	int	iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);

	for (int i=0;i < c_iMiniPetSkillCount;i++)
	{
		cMiniPetAbilityInfo	*lpAbility	=	&m_lpSelectMiniPet->m_aAbility[i];
		CSkill				*lpSkill	=	GetSkill(lpAbility->m_bf11Skill);

		if	(!lpSkill	||	lpAbility->m_bf11Skill	==	0)
			continue;

		int		iPutX	=	pos.x+c_iMiniPetSkillIconSize+4;
		int		iPutY	=	pos.y+iPutCount*c_iMiniPetSkillHeight;

		cRECT	rect;

		char	strText[64];

		if	(lpAbility->m_bf4Level	==	0)
			strcpy(strText,lpSkill->m_strName);
		else
			sprintf(strText,"%s(Lv %d)",lpSkill->m_strName,lpAbility->m_bf4Level);

		int		iWidth	=	s_text.getPixelSize(strText);

		rect.set(iPutX,iPutY,iPutX+iWidth,iPutY+19);

		iWidth	+=	6;

		if	(m_lpSelectMiniPet->isLevelUpAbleAbility(lpAbility))
			iWidth	+=	10;

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(iWidth,-4);

			return	i;
		}

		iPutCount++;
	}

	return	-1;
}


int
cMiniPetStatus::getFocusMiniPet()
{
	cHero_mini_pet	*lpIHeroForMiniPet	=	g_hero.iMiniPet();

	if	(lpIHeroForMiniPet->getMiniPetCount() == 0)
		return	-1;

	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iX		=	m_rect.x1;
	int		iY		=	m_rect.y1;
	int		i;

	iX		+=	12;
	iY		+=	8;

	CPos	pos;

	for	(i=0;i<c_iMiniPetCount;i++)
	{
		pos.x	=	iX+(i%8)*c_iPutMiniPetIconWidth;
		pos.y	=	iY+(i/8)*c_iPutMiniPetIconWidth;

		cRECT	rect;

		rect.set(pos.x,pos.y,pos.x+c_iMiniPetIconSize,pos.y+c_iMiniPetIconSize);

		if	(lpIHeroForMiniPet->getMiniPet(i)->m_bf7Level	==	0)
			continue;

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(0,rect.getHeight()+4);

			resetButtonsPos();
			return	i;
		}
	}

	return	-1;
}

void
cMiniPetStatus::feedItem()
{
	m_iFeedItem	=	s_iDragItem;

	if	(s_iDragItem	<	0)
		return;

	if	(s_iDragItem	>=	g_hero.m_wItemSlotCount	)
	{
		if	(s_iDragItem	<	dOWN_ITEM_COUNT)
			return;

		if	(s_iDragItem	>=	dOWN_ITEM_COUNT+g_hero.m_bf6ExtraInventorySize)
			return;
	}

	if	(m_iSelectMiniPet	>=	c_iMiniPetCount	||	m_iSelectMiniPet	<	0	)
		return;

	cMiniPet	*lpMiniPet	=	&g_hero.m_aMiniPet[m_iSelectMiniPet];

	if	(lpMiniPet->m_bf7Level	==	0	||	lpMiniPet->isExpired())
		return;

	if	(lpMiniPet->m_bf7Level	>=	100)
	{
		AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_GROUW_UP_MINIPET);
		return;
	}

	cItem	*lpItem	=	g_hero.getInventoryItem(s_iDragItem);

	if	(!lpItem)
		return;
		
	int		iExp	=	lpItem->getExpForMiniPet();

	if	(iExp	==	0	||	lpItem->isCanNotFeedToPetItem() || lpItem->isExpirationData())
	{
		g_msgBox.cPopup("",dMSG_INCORRECT_ITEM_FOR_FEED_MINIPET,dMSG_OK);
		return;
	}

	cItem		*lpExpBoostItem	=	g_hero.getItemByOption(eIE_INCREASE_GAIN_MINIPET_EXP);

	if	(lpExpBoostItem)
	{
		int	iBoost	=	lpExpBoostItem->m_aOption[0];

		iExp		=	iExp*iBoost;
	}

	char	*lpstrName	=	NULL;

	char	strQ[512];

	sprintf(strQ,dMSG_DO_YOU_WANT_FEED_ITEM_TO_MINI_PET_FORM,iExp);

	g_msgBox.cPopup("",strQ,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_FEED_ITEM_TO_MINIPET);
}