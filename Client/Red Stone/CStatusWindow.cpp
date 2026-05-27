#include "CGamePlay.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "rebirth_define.H"


// 10.02.18	칭호타입
#define			dTITLE_COUNT_IN_WINDOW_TYPE_TEXT		11
#define			dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE		4
#define			dTITLE_CHANGE_TYPE						1000

enum 
{
	eTITLE_TYPE_TEXT,
	eTITLE_TYPE_IMAGE,
	eTITLE_TYPE_TEXTIMAGE,
	eBI_NUMBER_ZERO		= 195,
};

CPos		CGamePlay::s_posStatusWindow;		//	인벤토리 윈도우 위치
int			CGamePlay::s_iStatusWindowScrollBarPos	=	0;		//	인벤토리 윈도우 위치
int			CGamePlay::s_iFocusTitle				=	0xffff;		//	인벤토리 윈도우 위치
int			CGamePlay::s_iLastActionTime			=	0;
int			CGamePlay::s_iSelectTitle				=	0xffff;
int			CGamePlay::s_iUseTitle					=	0xffff;

int			l_iFocusStatusContents					=	0xffff;

cBARMENU	CGamePlay::s_bmStatusButton;			// 칭호타입 버튼	10.02.22	

BOOL			CGamePlay::s_bIsTitleScrollBar			= FALSE;		// 칭호 스크롤그리기 여부	10.02.18
CImageScrollBar	CGamePlay::s_sbStatusWindow;							// 칭호 스크롤바			10.02.18

//GGG 관련 정의.
//CStatusWindow.cpp 파일과 CGameBottomInterface.cpp 파일 두군데에 있음으로 두곳을 같게 유지해 주어야 한다.
//죄송합니다...ㅜ.ㅜ;


//		0xffffffff 는 공식이나 에 따른다.

int l_iaGGGBattleTable[eGGG_ATTRIBUTE_END][eINCRECE_END] = {
	{0,0,0},
	{1000,1000	,dGGG_EFFECT_ADD_DAMAGE , 500 , 0xffffffff , dGGG_EFFECT_RETURN_FIRE_DAMAGE ,0 },
	{1000,2000	,dGGG_EFFECT_DECREASE_MOVE_SPEED |  dGGG_EFFECT_DECREASE_ATTACK_SPEED , 500 , 0 , dGGG_EFFECT_IGNORE_ONE_ATTACK_SELF,5},
	{1500,30	,dGGG_EFFECT_STUN		, 500	, 22500 , dGGG_EFFECT_INCREASE_SPEED_SELF,5},
	{1000,1000	,dGGG_EFFECT_DECREASE_DEFENCE_POWER , 1000	, 3000 , dGGG_EFFECT_RETURN_DAMAGE,5},
};

int l_iaGGGSkillUpTable[eGGG_ATTRIBUTE_END][eINCRECE_END] = {
	{0,0,0},
	{0 ,30,dGGG_EFFECT_NONE, 0 ,125,dGGG_EFFECT_NONE},
	{0 ,30,dGGG_EFFECT_NONE, 0 ,125,dGGG_EFFECT_NONE},
	{0 ,30,dGGG_EFFECT_NONE, 0 ,125,dGGG_EFFECT_NONE},
	{0 ,30,dGGG_EFFECT_NONE, 0 ,125,dGGG_EFFECT_NONE},
};

// 10.02.18 타이틀 이미지
int l_iaTitleImageIndex[eTI_ICON_END] = {
	eTIN_CHIVALRY,eTIN_SWORDSMANSHIP,eTIN_WIZARDRY,eTIN_LYCANTROPHY,eTIN_FANATICISM,eTIN_ANGELRANK,eTIN_KEYMANAFACTURING,eTIN_HANDICRAFTSMAN,eTIN_RECPMMAOSSANCE,
	eTIN_TACTICS,eTIN_PALEOGRAPHY,eTIN_THREMMATOLOGY,eTIN_COURTESY,eTIN_ASTRONOMY,eTIN_SPIRITISM,eTIN_DEVIL,eTIN_PSYCHIC,eTIN_PIONEER,eTIN_CITIZEN,eTIN_NOVICE,
	eTIN_MONSTERSEXER,eTIN_BANK_CUSTOMER,eTIN_MAPMAKER,eTIN_PORTER,eTIN_INSTINCTFORSURVIVAL,eTIN_BROKER,eTIN_HONORMEMBER,eTIN_PHAMRMACIST,eTIN_CHARMING,eTIN_CAMPING_MASTER,
	eTIN_CITYWALKER,eTIN_ALCHEMY,eTIN_REDEYE_MEMBER,eTIN_ORPHIC,eTIN_JEWELY_MASTER,eTIN_REDSTONE_HUNTER,eTIN_CAVETHEFT,eTIN_CHROMATIC_HONORMEMBER, eTN_RED_STONE_AGENT,eTN_BARD,
	eTIN_INFIGHTER,eTIN_OPTICAL,
};
enum
{
	eBM_PLUS_STRENGTH		=	1000,
	eBM_PLUS_LUCK			,
	eBM_PLUS_WISDOM			,
	eBM_PLUS_CHARISMA		,
	eBM_PLUS_CONSTITUTION	,
	eBM_PLUS_AGILITY		,
	eBM_PLUS_INTELLIGENCE	
};


int
CGamePlay::GetGGGSkillUpTimeValue(int _Level)
{
	int iTime = 0;
	switch (_Level / 100)
	{
	case 0:	
		iTime = 30;					//1~99
		break;
	case 1:
		iTime = 60;					//100~199
		break;
	case 2:
		iTime = 90;					//200~299
		break;
	case 3:
		iTime = 120;					//300~399
		break;
	case 4:
		iTime = 150;					//400~499
		break;
	case 5:
		iTime = 300;					//500~599
		break;
	case 6:
		iTime = 330;					//600~699
		break;
	case 7:
		iTime = 360;					//700~799
		break;
	case 8:
		iTime = 420;					//800~899					
		break;
	case 9:
		iTime = 450;					//900~999					
		break;
	}
	
	return iTime;
}


int
CGamePlay::GetGGGSkillUpTableValue(int _attribute , int _IncreceType)
{
	if(_attribute >= eGGG_ATTRIBUTE_END)	return 0;
	if(_IncreceType >= eINCRECE_END)		return 0;
	
	return l_iaGGGSkillUpTable[_attribute][_IncreceType];
}

//
//	스테이터스 윈도우 닫기
void
CGamePlay::CloseStatusWindow()
{
	s_iStatusWindowScrollBarPos	=	s_sbPopupWindow.m_iCurrentPos;

	s_bmBottomMenu.check(eBIM_MENU_STATUS,FALSE);
	s_bmPlusState.reset();
	cINPDEV::ReleaseExclusive();
	g_gwPet.close();
}

//
//	스테이터스 창 열기
void
CGamePlay::OpenStatusWindow()
{
	if	(s_iPopupInterface	==	ePIW_STATUS)
	{
		ClosePopupWindow();

		return;
	}

	ClosePopupWindow();

	if	(g_hero.m_bf1IsWaitToReceiveDuelRecord	==	FALSE	&&	g_hero.m_bf1IsReceivedDuelRecord	==	FALSE)
	{
		s_agent.sendEtcWork(eCEW_ASK_DUEL_RECORD_INFO);
		g_hero.m_bf1IsWaitToReceiveDuelRecord	=	TRUE;
	}

	s_iPopupInterface		=	ePIW_STATUS;

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_STATUS);

	s_posStatusWindow.x		=	g_iScreenWidth-iWindowWidth;
	s_posStatusWindow.y		=	iTitleBarHeight+g_iCorrectBI_Y/2;
	s_iFocusTitle			=	0xffff;

	s_rectInterfaceSize.set(s_posStatusWindow.x, s_posStatusWindow.y-iTitleBarHeight, s_posStatusWindow.x+iWindowWidth, s_posStatusWindow.y+iWindowHeight);

	int	iX					=	s_posStatusWindow.x;
	int	iY					=	s_posStatusWindow.y;

	s_bmPlusState.reset();
	s_bmBottomMenu.resetChecker();
	s_bmBottomMenu.check(eBIM_MENU_STATUS,TRUE);

	s_sbPopupWindow.set(s_posStatusWindow.x+iWindowWidth-18,332+g_iCorrectBI_Y/2,160);
	s_sbPopupWindow.setPos(s_iStatusWindowScrollBarPos);
	s_sbPopupWindow.setSize(1,2);

	InitPopupWindowCloseButton();

	g_gwPet.open();
	// 10.02.18
	int iTypeIndex = g_config.m_bf2TitleViewType;
	SetTitleType(iTypeIndex);	

	s_bmStatusButton.reset();
	s_bmStatusButton.addImageBar("",dTITLE_CHANGE_TYPE,g_iScreenWidth-36,g_iCorrectBI_Y/2,eSW_CHANGE_ICON_SIZE_NORMAL,eSW_CHANGE_ICON_SIZE_ACTIVE,eSW_CHANGE_ICON_SIZE_PRESSED,0xffff,0xffff);	

}

//
//	레벨 포인트 처리
void
CGamePlay::OperateLevelPoint()
{
	if	(g_hero.m_wLevelPoint	==	0)
	{
		if	(s_bmPlusState.isActive())
			s_bmPlusState.reset();

		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_INTERFACE);

		return;
	}

	if	(!s_bmPlusState.isActive())
	{
		int	iX	=	s_posStatusWindow.x+8;
		int	iY	=	s_posStatusWindow.y+134;

		s_bmPlusState.reset();
		s_bmPlusState.addImageBar("",eBM_PLUS_STRENGTH		,iX+19	,iY+12,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		s_bmPlusState.addImageBar("",eBM_PLUS_AGILITY		,iX+78	,iY+12,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		s_bmPlusState.addImageBar("",eBM_PLUS_CONSTITUTION	,iX+137	,iY+12,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		s_bmPlusState.addImageBar("",eBM_PLUS_LUCK			,iX+196	,iY+12,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);

		iY		+=	30;
		s_bmPlusState.addImageBar("",eBM_PLUS_INTELLIGENCE	,iX+19	,iY+12,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		s_bmPlusState.addImageBar("",eBM_PLUS_WISDOM		,iX+78	,iY+12,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		s_bmPlusState.addImageBar("",eBM_PLUS_CHARISMA		,iX+137	,iY+12,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
	}

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_INTERFACE))
		return;

	DWORD	dwMenu	=	s_bmPlusState.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);
	int		iPoint	=	1;

	if	(s_aKey[KEY_LSHIFT])
		iPoint		=	10;

	if	(s_aKey[KEY_LCONTROL])
		iPoint		=	100;

#ifdef	_DEBUG
	if	(s_aKey[KEY_LALT])
		iPoint		=	1000;
#endif

	if	(dwMenu	!=	0xffffffff)
	{
		if	(s_iLastActionTime+500	<	timeGetTime())
		{
			s_iLastActionTime	=	timeGetTime();

			switch(dwMenu)
			{
				case	eBM_PLUS_STRENGTH		:
				{
					if	(g_hero.m_wJob	==	dJOB_WEREWOLF)
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_INTELLIGENCE,iPoint);
					else
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_STRENGTH,iPoint);
					break;
				}
				case	eBM_PLUS_INTELLIGENCE	:
				{
					if	(g_hero.m_wJob	==	dJOB_WEREWOLF)
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_STRENGTH,iPoint);
					else
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_INTELLIGENCE,iPoint);
					break;
				}

				case	eBM_PLUS_WISDOM			:
				{
					if	(g_hero.m_wJob	==	dJOB_WEREWOLF)
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_AGILITY,iPoint);
					else
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_WISDOM,iPoint);
					break;
				}
				case	eBM_PLUS_AGILITY		:
				{
					if	(g_hero.m_wJob	==	dJOB_WEREWOLF)
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_WISDOM,iPoint);
					else
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_AGILITY,iPoint);
					break;
				}

				case	eBM_PLUS_CHARISMA		:
				{
					if	(g_hero.m_wJob	==	dJOB_WEREWOLF)
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_CONSTITUTION,iPoint);
					else
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_CHARISMA,iPoint);
					break;
				}
				case	eBM_PLUS_CONSTITUTION	:
				{
					if	(g_hero.m_wJob	==	dJOB_WEREWOLF)
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_CHARISMA,iPoint);
					else
						s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_CONSTITUTION,iPoint);
					break;
				}

				case	eBM_PLUS_LUCK			:
					s_agent.sendIncreaseState(eCHARACTER_BASIC_STATE_LUCK,iPoint);
					break;
			}
		}
	}

	if	(s_bmPlusState.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_INTERFACE);
	else
		cINPDEV::ReleaseExclusive();
}

//
//	아이템 인터페이스 업데이트
void
CGamePlay::UpdateStatusWindow()
{
	if (s_iPopupInterface	!=	ePIW_STATUS)
		return;

	// 10.02.18
	UpdateTitleType();
	OperateTitleScrollBar();

	g_gwPet.update();

	if (g_hero.getPetCount()+g_hero.getSummonBeastCount() == 0)
	{
		int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);

		int iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);


		s_rectInterfaceSize.set(s_posStatusWindow.x, s_posStatusWindow.y-g_sprInterface.getSpriteHeight(eWINDOW_TITLE_STATUS), s_posStatusWindow.x+iWindowWidth, s_posStatusWindow.y+iWindowHeight);

	}

	OperateLevelPoint();	//	레벨 포인트 처리

	if	(s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
		cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
	else
		cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);

	s_iStatusWindowScrollBarPos	=	s_sbPopupWindow.m_iCurrentPos;

	if	(cINPDEV::IsExclusive())
		return;

	// 10.02.18   
//	칭호 처리
	{
		int	iCount			=	0;
		int iPutCount		=	0;	
		
		if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXT)
		{
			int	iY				=	334+g_iCorrectBI_Y/2;
			int	iX				=	s_posStatusWindow.x+26;
			int iStatusCount	= s_sbStatusWindow.m_iCurrentPos*2;

			s_iFocusTitle		=	0xffff;
			
			for (int i=0;i<dMAX_TITLE_COUNT ;i++)
			{
				CPlayerTitleInfo*	lpTitleInfo	=	g_hero.getTitleByIndex(i);

				if	(!lpTitleInfo)
					continue;	
				
				CTitle*				lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);
				
				if	(!lpTitle	)
					continue;
				
				if(iCount < iStatusCount)
				{
					iCount++;
					continue;
				}
				
				int	iXPos	=	iX;
				
				if	(iPutCount%2==1)
					iXPos	=	s_posStatusWindow.x+138;
				
				cRECT	rect;
				
				rect.set(iXPos,iY+(iPutCount/2)*13,iXPos+106,iY+(iPutCount/2)*13+12);
				
				if	(s_iFocusTitle == 0xffff && rect.isIn(s_posMouse.x,s_posMouse.y))
				{
					if	(s_isClickedRightButton)
					{
						if	(lpTitleInfo->m_bTitle	==	eTN_BERSERK	||	lpTitleInfo->m_bTitle	==	eTN_BARD)
						{
							if	(lpTitleInfo->m_bTitle	==	eTN_BARD)
							{
								if	(g_lpHero->isSitdown()	==	FALSE)
								{
									AddSystemMessage(LTYELLOW,dMSG_BARD_TITLE_CAN_USE_UNDER_SITDOWN);
									
									break;
								}
								
								if	(g_hero.m_bf5BardTitleCoolTime)
								{
									AddSystemMessage(LTYELLOW,dMSG_WAIT_TO_USE_TITLE_COOLTIME_FORM,g_hero.m_bf5BardTitleCoolTime);
									
									break;
								}
							}
							
							s_iUseTitle	=	lpTitleInfo->m_bTitle;
							g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_USE_TITLE_FORM,lpTitle->m_strName),dMSG_YES,dMSG_NO);
							g_msgBox.setButtonId(eMBI_USE_TITLE);
							
							break;
						}
						
						if	(lpTitle->m_bf1IsRemoveAble)
						{
							s_pmCommon.popup(iX,iY+20);
							s_pmCommon.addMenu(ePM_REMOVE_TITLE,_ms(dMSG_REMOVE_TITLE_FORM,lpTitle->m_strName));
							s_iSelectTitle	=	i;
							
							return;
						}
					}
					
					s_rectTooltip.set(&rect);
					s_rectTooltip.add(-2,-4);
					s_iFocusTitle	=	i;
					
					int iPlayerLevel = g_hero.getLevel();
					if	(g_hero.m_bf4RebirthCount)	//	전생자는 렙제 푼다
						iPlayerLevel	=	max(iPlayerLevel,600);
					
					char	*lpstrComment	=	lpTitle->getTooltip(lpTitleInfo->m_bLevel,iPlayerLevel);
					
					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,lpstrComment,eTFM_LEFT);
				}
	
				iCount++;
				iPutCount++;
	
				if(iPutCount >= dTITLE_COUNT_IN_WINDOW_TYPE_TEXT * 2)
					break;
			}
		}	//	if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXT)
		else if( g_config.m_bf2TitleViewType == eTITLE_TYPE_IMAGE)
		{
			int	iY				= 350+g_iCorrectBI_Y/2;
			int	iX				= s_posStatusWindow.x+42;
			int iStatusCount	= s_sbStatusWindow.m_iCurrentPos*8;
			s_iFocusTitle		= 0xffff;

			for (int i=0;i<dMAX_TITLE_COUNT;i++)
			{
				CPlayerTitleInfo*	lpTitleInfo	=	g_hero.getTitleByIndex(i);
				
				if	(!lpTitleInfo)
					continue;
				
				CTitle*				lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);
				
				if	(!lpTitle	)
					continue;
				
				if(iCount < iStatusCount)
				{
					iCount++;
					continue;
				}

				int	iXPos	= iX + 27 * (iPutCount % 8);
				int iYPos	= iY + 36 * (iPutCount / 8);
				
				cRECT	rect;
				rect.set(iXPos-13,iYPos-15,iXPos+10,iYPos+17);
																
				if	(s_iFocusTitle == 0xffff && rect.isIn(s_posMouse.x,s_posMouse.y))
				{
					if	(s_isClickedRightButton)
					{
						if	(lpTitleInfo->m_bTitle	==	eTN_BERSERK	||	lpTitleInfo->m_bTitle	==	eTN_BARD)
						{
							if	(lpTitleInfo->m_bTitle	==	eTN_BARD)
							{
								if	(g_lpHero->isSitdown()	==	FALSE)
								{
									AddSystemMessage(LTYELLOW,dMSG_BARD_TITLE_CAN_USE_UNDER_SITDOWN);
									
									break;
								}
								
								if	(g_hero.m_bf5BardTitleCoolTime)
								{
									AddSystemMessage(LTYELLOW,dMSG_WAIT_TO_USE_TITLE_COOLTIME_FORM,g_hero.m_bf5BardTitleCoolTime);
									
									break;
								}
							}
							
							s_iUseTitle	=	lpTitleInfo->m_bTitle;
							g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_USE_TITLE_FORM,lpTitle->m_strName),dMSG_YES,dMSG_NO);
							g_msgBox.setButtonId(eMBI_USE_TITLE);
							
							break;
						}
						
						if	(lpTitle->m_bf1IsRemoveAble)
						{
							s_pmCommon.popup(iX,iY+20);
							s_pmCommon.addMenu(ePM_REMOVE_TITLE,_ms(dMSG_REMOVE_TITLE_FORM,lpTitle->m_strName));
							s_iSelectTitle	=	i;
							return;
						}
					}
					
					s_rectTooltip.set(&rect);
					s_rectTooltip.add(-2,-4);
					s_iFocusTitle	=	i;
					
					int iPlayerLevel = g_hero.getLevel();
					if	(g_hero.m_bf4RebirthCount)	//	전생자는 렙제 푼다
						iPlayerLevel	=	max(iPlayerLevel,600);
					
					char	*lpstrComment	=	lpTitle->getTooltip(lpTitleInfo->m_bLevel,iPlayerLevel);
					
					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,lpstrComment,eTFM_LEFT);
				}
				
				iCount++;
				iPutCount++;

				if(iPutCount >= dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE * 8)
					break;
			}
		}	// else if( g_config.m_bf2TitleViewType == eTITLE_TYPE_IMAGE)
		else if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXTIMAGE)
		{
			int iY				=	350+g_iCorrectBI_Y/2;
			int iX				=	s_posStatusWindow.x+42;
			int iStatusCount	= s_sbStatusWindow.m_iCurrentPos*2;

			s_iFocusTitle		= 0xffff;


			for (int i=0;i<dMAX_TITLE_COUNT;i++)
			{
				CPlayerTitleInfo*	lpTitleInfo	=	g_hero.getTitleByIndex(i);
				
				if	(!lpTitleInfo)
					continue;
				
				CTitle*				lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);
				
				if	(!lpTitle	)
					continue;
				
				if(iCount < iStatusCount)
				{
					iCount++;
					continue;
				}
				
				int	iXPos	= iX + 113 * (iPutCount % 2);
				int iYPos	= iY + 36  * (iPutCount / 2);
																
				cRECT	rect;
				rect.set(iXPos-13,iYPos-15,iXPos+90,iYPos+17);
																
				if	(s_iFocusTitle == 0xffff && rect.isIn(s_posMouse.x,s_posMouse.y))
				{
					if	(s_isClickedRightButton)
					{
						if	(lpTitleInfo->m_bTitle	==	eTN_BERSERK	||	lpTitleInfo->m_bTitle	==	eTN_BARD)
						{
							if	(lpTitleInfo->m_bTitle	==	eTN_BARD)
							{
								if	(g_lpHero->isSitdown()	==	FALSE)
								{
									AddSystemMessage(LTYELLOW,dMSG_BARD_TITLE_CAN_USE_UNDER_SITDOWN);
									
									break;
								}
								
								if	(g_hero.m_bf5BardTitleCoolTime)
								{
									AddSystemMessage(LTYELLOW,dMSG_WAIT_TO_USE_TITLE_COOLTIME_FORM,g_hero.m_bf5BardTitleCoolTime);
									
									break;
								}
							}
							
							s_iUseTitle	=	lpTitleInfo->m_bTitle;
							g_msgBox.cPopup("",_ms(dMSG_ARE_YOU_REALLY_USE_TITLE_FORM,lpTitle->m_strName),dMSG_YES,dMSG_NO);
							g_msgBox.setButtonId(eMBI_USE_TITLE);
							
							break;
						}
						
						if	(lpTitle->m_bf1IsRemoveAble)
						{
							s_pmCommon.popup(iX,iY+20);
							s_pmCommon.addMenu(ePM_REMOVE_TITLE,_ms(dMSG_REMOVE_TITLE_FORM,lpTitle->m_strName));
							s_iSelectTitle	=	i;
							return;
						}
					}
					
					s_rectTooltip.set(&rect);
					s_rectTooltip.add(-2,-4);
					s_iFocusTitle	=	i;
					
					int iPlayerLevel = g_hero.getLevel();
					if	(g_hero.m_bf4RebirthCount)	//	전생자는 렙제 푼다
						iPlayerLevel	=	max(iPlayerLevel,600);
					
					char	*lpstrComment	=	lpTitle->getTooltip(lpTitleInfo->m_bLevel,iPlayerLevel);
					
					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,lpstrComment,eTFM_LEFT);
				}	
		
				iCount++;
				iPutCount++;

				if(iPutCount >= dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE * 2)
					break;
			}
		}	// else if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXTIMAGE)
	}
}
//
//	그려!!
void
CGamePlay::DrawStatusWindow()
{
	int		iX,iY,iPointUpWidth=0;
	DWORD	dwColor;
	int		iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);

	iX		=	s_posStatusWindow.x;
	iY		=	s_posStatusWindow.y;

	l_iFocusStatusContents	=	0xffff;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+266-4,iY+482-8);

	if	(g_hero.m_wLevelPoint)
		iPointUpWidth	=	13;

//------------------------------------------------------------------------------	
	iY		=	g_iCorrectBI_Y/2;
	iX		=	s_posStatusWindow.x;
//	g_sprInterface.Put(iX+7,iY+23	,eSW_PERSONAL_FRAME);
	g_sprRegionInterface.Put(iX+7,iY+23	,8);
	// ToolTip
	char	strToolTip[1024];
	cRECT rect = {iX + 230,0+g_iCorrectBI_Y/2,iX + 246,16+g_iCorrectBI_Y/2};
	
	if (rect.isIn(s_posMouse.x,s_posMouse.y))
	{
		sprintf(strToolTip,dMSG_CHANGE_VIEW_TYPE);
		s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
	}
//	이름/직업
	s_text.put(iX+28,iY+30	,WHITE	,"%s <c:LTYELLOW>%s<n> [<c:LTGREEN>%s<n>]",dMSG_NAME,g_hero.m_strName,g_hero.getJobName());

//	거신의 은총 관련
	int GGGImageIconindex = g_hero.m_bf3GreateGodGrace - 1;

	if	(g_hero.m_bf3GreateGodGrace)
		g_smiIconGGG.putScale(GGGImageIconindex,iX+240,iY+26,50);

	cRECT	rectGGG;
	rectGGG.set(774+g_iCorrectBI_X , 26+g_iCorrectBI_Y/2 , 774 + 17 +g_iCorrectBI_X, 26 + 17+g_iCorrectBI_Y/2);		//17 아이콘 가로 세로...

	if	(rectGGG.isIn(s_posMouse.x,s_posMouse.y) && g_hero.m_bf3GreateGodGrace)
	{
		cSTRING	string;
		int iSkillUpTime	= GetGGGSkillUpTimeValue(g_hero.m_iLevel);
		int iActiveChance1	= l_iaGGGBattleTable[g_hero.m_bf3GreateGodGrace][eINCRECE_VALUE1_ACITECHANCE] / 100;
		int iActiveChance2	= l_iaGGGBattleTable[g_hero.m_bf3GreateGodGrace][eINCRECE_VALUE2_ACITECHANCE] / 100;
		int iValue1			= l_iaGGGBattleTable[g_hero.m_bf3GreateGodGrace][eINCRECE_VALUE1] / 100;
		int iValue2			= l_iaGGGBattleTable[g_hero.m_bf3GreateGodGrace][eINCRECE_VALUE2] / 100;
		int iTime			= l_iaGGGBattleTable[g_hero.m_bf3GreateGodGrace][eINCRECE_VALUE_ETC];
		
		string.Add(g_pstrGreateGodGrace[g_hero.m_bf3GreateGodGrace - 1]);		//이름 입력 EX>타오르는 불의 은총		
		string.Add("\n\n");		
		string.Add("<c:LTCYAN><%s><n>\n",dMSG_COMMENT);
		string.Add(g_pstrGreateGodGrace[dMSG_GGG_COMMENT_START_INDEX + g_hero.m_bf3GreateGodGrace - 1]);//설명 입력		
		string.Add("\n\n");
		string.Add("<c:LTCYAN><%s><n>\n",dMSG_GGG_EFFECT);
		string.Add("\n");

		switch (g_hero.m_bf3GreateGodGrace)		//효과에 따라 다른 은총 툴팁을 출력한다.
		{
			case eGGG_ATTRIBUTE_FIRE:			//불의 은총
			{
				int iActiveValueMin = (12 + 2 * g_hero.m_iLevel / 2 - 7) ;
				int iActiveValueMax = (12 + 2 * g_hero.m_iLevel / 2 + 7) ;
				
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_FIRE1 , iActiveChance1 , iValue1));
				string.Add("\n");
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_FIRE2 , iActiveChance2 , iActiveValueMin , iActiveValueMax));
				string.Add("\n");
				break;
			}
			case eGGG_ATTRIBUTE_WATER:			//물의 은총
			{
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_WATER1 , iActiveChance1 , iValue1 , iTime));
				string.Add("\n");
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_WATER2 , iActiveChance2 , iTime));
				string.Add("\n");
				break;
			}
			case eGGG_ATTRIBUTE_WIND:			//바람의 은총
			{
				int iActiveValueMin = (2000 + 2 * g_hero.m_iLevel / 2 - 1100) / 100;
				int iActiveValueMax = (2000 + 2 * g_hero.m_iLevel / 2 + 1100) / 100;
				
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_WIND1 , iActiveChance1 , l_iaGGGBattleTable[g_hero.m_bf3GreateGodGrace][eINCRECE_VALUE1]));
				string.Add("\n");
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_WIND2 , iActiveChance2 , iValue2 , iTime));
				string.Add("\n");
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_WIND3 , iActiveChance2 , iActiveValueMin ,  iActiveValueMax));
				string.Add("\n");
				break;
			}

			case eGGG_ATTRIBUTE_EARTH:			//대지의 은총.
			{
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_EARTH1 , iActiveChance1 , iValue1  , iTime));
				string.Add("\n");
				string.Add(_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL_EARTH2 , iActiveChance2 , iValue2 ));
				string.Add("\n");
				break;
			}
		}

		string.Add("\n");
		string.Add(dMSG_GGG_SKILLUP_EFFECT_COMMENT_VALUE1_FORM , GetGGGSkillUpTableValue(g_hero.m_bf3GreateGodGrace , eINCRECE_VALUE1) , iSkillUpTime);
		string.Add("\n");
		string.Add(dMSG_GGG_SKILLUP_EFFECT_COMMENT_VALUE2_FORM , GetGGGSkillUpTableValue(g_hero.m_bf3GreateGodGrace , eINCRECE_VALUE2) , iSkillUpTime);
		string.Add("\n");
		
		s_ttCommon.popupTip(&rectGGG,-1,-1,&s_text,WHITE,string.String,eTFM_LEFT);
		
		WORD	*lpImage	=	g_smiIconGGG.get16Image(GGGImageIconindex);
		s_ttCommon.setImage(lpImage);
	}

	//	길드명/랭크
	{
		iY	=	50+g_iCorrectBI_Y/2;
		if	(g_hero.m_wGuildSerial != 0xffff)
		{
			if	(g_hero.m_wGuildRank	==	0xffff)
				g_hero.m_wGuildRank	=	1;

			s_text.put(iX+28,iY	,WHITE	,"%s <c:CTBLUE>%s<n>[<c:LTGREEN>%s<n>]",dMSG_GUILD,g_hero.m_strGuildName,g_pstrGuildRank[g_hero.m_wGuildRank]);
		}
		else
			s_text.put(iX+28,iY	,WHITE	,"%s <c:LTGRAY>%s<n>",dMSG_GUILD,g_pstrGuildRank[0]);

	//	결투 전적
		if	(g_hero.m_bf1IsReceivedDuelRecord)
		{
			cDuelRecord	*lpRecord	=	&g_hero.m_duelRecord;

			iX		=	s_posStatusWindow.x+iWindowWidth-6;

			int	iTextWidth	=	s_text.getPixelSize(dMSG_DUEL_RECORD);
			s_text.putRF(iX,iY,WHITE,dMSG_DUEL_RECORD);

			cRECT	rect;

			rect.set(iX-iTextWidth,iY-2,iX,iY-2+16);

			if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				rect.add(-2,-4);

				char	strToolTip[1024];

				strcpy(strToolTip,_ms("<c:LTYELLOW>%s<n>\n\n",dMSG_DUEL_RECORD));

				if	(lpRecord->m_iBattleCount	==	0)
					strcat(strToolTip,_ms("<c:GRAY>%s<n>",dMSG_OUT_OF_DUEL_RECORD));
				else
				{
					//	캐릭터 정보
					{
						strcat(strToolTip,_ms("<c:LTCYAN><%s><n>\n",dMSG_DUEL_CHARACTER_INFO));
						strcat(strToolTip,_ms("- %s <c:LTYELLOW>%s<n>\n",dMSG_NAME,lpRecord->m_strName));
						strcat(strToolTip,_ms("- %s <c:LTYELLOW>%s<n>\n",dMSG_JOB,g_pstrHeroJobName[lpRecord->m_wJob]));
						strcat(strToolTip,"\n");
					}

					//	순위
					{
						strcat(strToolTip,_ms("<c:LTCYAN><%s><n>\n",dMSG_RANKING));

						//	주간 순위
						strcat(strToolTip,_ms("- %s ",dMSG_WEEKLY_RANKING));
						if	(g_hero.m_wWeeklyDuelRank	==	0xffff)
							strcat(strToolTip,_ms("<c:LTRED>%s<n>",dMSG_OUT_OF_RANKING));
						else
							strcat(strToolTip,_ms(dMSG_RANK_FORM,g_hero.m_wWeeklyDuelRank+1));
						strcat(strToolTip,"\n");

						//	오늘 순위
						strcat(strToolTip,_ms("- %s ",dMSG_DAILY_RANKING));
						if	(g_hero.m_wDailyDuelRank	==	0xffff)
							strcat(strToolTip,_ms("<c:LTRED>%s<n>",dMSG_OUT_OF_RANKING));
						else
							strcat(strToolTip,_ms(dMSG_RANK_FORM,g_hero.m_wDailyDuelRank+1));

						strcat(strToolTip,"\n\n");
					}


					//	전적
					{
						strcat(strToolTip,_ms("<c:LTCYAN><%s><n>\n- ",dMSG_MILITARY_RECORD));
						strcat(strToolTip,_ms(dMSG_WEEKLY_BATTLE_RECORD_FORM,lpRecord->m_iWeeklyBattleCount,lpRecord->m_iWeeklyWinCount,lpRecord->m_iWeeklyDefeatCount,lpRecord->m_iWeeklyDrawCount));
						strcat(strToolTip,"\n- ");
						strcat(strToolTip,_ms(dMSG_TOTAL_BATTLE_RECORD_FORM,lpRecord->m_iBattleCount,lpRecord->m_iWinCount,lpRecord->m_iDefeatCount,lpRecord->m_iDrawCount));
						strcat(strToolTip,"\n\n");
					}

					//	포인트
					{
						strcat(strToolTip,_ms("<c:LTCYAN><%s><n>\n",dMSG_POINT));
						strcat(strToolTip,_ms("- %s <c:LTYELLOW>%d<n>\n",dMSG_WINNING_POINT,lpRecord->m_iWeeklyWinPoint));
						strcat(strToolTip,_ms("- %s <c:LTYELLOW>%d<n>\n",dMSG_DUEL_POINT,lpRecord->m_iDuelPoint));					
					}
				}

				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}
		}
	}

//	전생 정보
	{
		iX		=	s_posStatusWindow.x;
		iY		=	68+g_iCorrectBI_Y/2;

		s_text.put(iX+28 ,iY,RGB(255,255,255),dMSG_REBIRTH_COUNT_FORM,g_hero.m_bf4RebirthCount,g_hero.m_bf4UseSealSphereOfDawnCount);

		cRECT	rect;

		rect.x1	=	iX;
		rect.y1	=	iY-2;
		rect.x2	=	g_iScreenWidth;
		rect.y2	=	iY+16;

		if	(g_hero.m_bf4RebirthCount && rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			char	strToolTip[1024];
			char	strPenelty[128];

			int		iPenelty	=	0;

			if	(g_hero.m_bf4RebirthCount && g_hero.m_iLevel <= dGAIN_EXP_PENELTY_LEVEL_FOR_REBIRTH)
				iPenelty	=	c_aiGainExpPeneltyForRebirth[g_hero.m_bf4RebirthCount];

			if	(iPenelty	==	0)
				sprintf(strPenelty,"<c:LTGREEN>%s<n>",dMSG_NOTHING);
			else
				sprintf(strPenelty,dMSG_REBIRTH_EXP_PENELTY_FORM,dGAIN_EXP_PENELTY_LEVEL_FOR_REBIRTH,iPenelty);

			sprintf(strToolTip,dMSG_REBIRTH_TOOLTIP_FORM,
						c_aBonusStatePointForRebirth[g_hero.m_bf4RebirthCount],c_aBonusSkillPointForRebirth[g_hero.m_bf4RebirthCount]);
#ifdef	_FOR_JAPAN			// 09.09.28
			sprintf(strToolTip,"%s\n%s : %s"	,strToolTip, dMSG_EXP_PENALTY, strPenelty);
#else

#endif

			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
		}
	}

	{
		iX		=	s_posStatusWindow.x;
		iY		=	92+g_iCorrectBI_Y/2;
	//	레벨
#ifdef	_FOR_JAPAN
	s_text.put(iX+28 ,iY,RGB(255,198,0),"Lv");
#else
		s_text.put(iX+28 ,iY,RGB(255,198,0),dMSG_LEVEL);
#endif

		if	(g_hero.getLevel()	>	g_hero.m_iLevel	)
			s_text.put(iX+56 ,iY,RGB(255,238,121),"<c:LTGREEN>%d<n>",g_hero.getLevel());
		else
		if	(g_hero.getLevel()	<	g_hero.m_iLevel	)
			s_text.put(iX+56 ,iY,RGB(255,238,121),"<c:LTRED>%d<n>",g_hero.getLevel());
		else
			s_text.put(iX+56 ,iY,RGB(255,238,121),"%d",g_hero.getLevel());	

	//	경험치
		s_text.put(iX+146,iY,RGB(255,198,0),dMSG_EXP);

		if (g_hero.m_iExperience	>=	0)
			s_text.put(iX+186,iY,RGB(255,238,121),"%d",g_hero.m_iExperience);
		else
			s_text.put(iX+186,iY,LTRED,"%d",g_hero.m_iExperience);
	}

	{
		iY		=	112+g_iCorrectBI_Y/2;

	//	체력
		{
			int	iMaxHP	=	g_hero.getMaxHP();
			int	iHP		=	max(g_hero.m_iHP/100,1);

			if (g_hero.m_iHP==	0)	iHP	=	0;

			s_text.put(iX+28,iY,RGB(255,80,92),dMSG_HP);
			s_text.put(iX+56,iY,RGB(254,160,167),"%d/%d",iHP,iMaxHP/100);
		}

	//	공격력
		s_text.put(iX+146,iY,RGB(144,211,132),dMSG_ATTACK_POWER);
		s_text.put(iX+186,iY,RGB(194,220,160),"%d~%d",g_hero.getAttackPower(NULL,TRUE)/100,g_hero.getAttackPower(NULL,FALSE)/100);
	}


	iY	=	126+g_iCorrectBI_Y/2;

//	CP
	s_text.put(iX+28,iY,RGB(86,154,255),dMSG_CP);
	s_text.put(iX+56,iY,RGB(158,197,255),"%d/%d",g_hero.m_iCP/100,g_hero.getMaxCP()/100);

//	방어력
	{
		int	iDefenseValue	=	g_hero.getDefensivePower();
		int	iBaseValue		=	g_hero.getDefensivePower(TRUE);

		s_text.put(iX+146,iY,RGB(118,206,163),dMSG_DEFENSE_POWER);

		if (g_hero.isPlusDefense())	
			s_text.put(iX+186,iY,RGB(191,227,199),"%d[<c:LTGREEN>%d<n>]",iBaseValue/100,iDefenseValue/100);
		else
		if (g_hero.isMinusDefense())	
			s_text.put(iX+186,iY,RGB(191,227,199),"%d[<c:LTRED>%d<n>]"  ,iBaseValue/100,iDefenseValue/100);
		else
			s_text.put(iX+186,iY,RGB(191,227,199),"%d",iDefenseValue/100);
	}

//	능력
	{
		iX		=	s_posStatusWindow.x+8;
		iY		=	149+g_iCorrectBI_Y/2;

		if (g_hero.m_wDeathPeneltySecond)
		{
			cRECT	rect;

			rect.set(iX,iY,g_iScreenWidth,iY+56);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				rect.add(-2,-4);

				char	strToolTip[1024];

				sprintf(strToolTip,dMSG_DEATH_PENELTY_FORM,g_hero.m_wDeathPeneltySecond/60,g_hero.m_wDeathPeneltySecond%60,g_hero.m_wDeathPenelty);

				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}
		}
		else
		for (int i=0;i<8;i++)
		{
			cRECT	rect;

			rect.x1	=	iX+17+59*(i%4);
			rect.y1	=	iY + i/4*29;
			rect.x2	=	iX+75+59*(i%4);
			rect.y2	=	iY+26+ i/4*29;

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				l_iFocusStatusContents	=	i;

				rect.add(-2,-4);

				char	strToolTip[1024];
				char	strValue[100]	=	"";
				char	strColor[20]	=	"";
				char	strPlus[5]		=	"";
				int		iStatus			=	i+1;

				if (i	==	3)
					iStatus	=	7;
				else
				if (i	==	6)
					iStatus	=	4;

				int		iValue1,iValue2;
				BOOL	bIsFixedValue	=	FALSE;

				if (i==	0)
				{
					iValue1			=	g_hero.getStrength();
					iValue2			=	g_hero.getStrength(TRUE);
					bIsFixedValue	=	g_hero.isFixedStrength();
				}
				if (i==	1)
				{
					iValue1			=	g_hero.getAgility();
					iValue2			=	g_hero.getAgility(TRUE);
					bIsFixedValue	=	g_hero.isFixedAgility();
				}
				if (i==	2)
				{
					iValue1			=	g_hero.getConstitution();
					iValue2			=	g_hero.getConstitution(TRUE);
					bIsFixedValue	=	g_hero.isFixedConstitution();
				}
				if (i==	3)
				{
					iValue1			=	g_hero.getLuck();
					iValue2			=	g_hero.getLuck(TRUE);
					bIsFixedValue	=	g_hero.isFixedLuck();
				}
				if (i==	4)
				{
					iValue1			=	g_hero.getIntelligence();
					iValue2			=	g_hero.getIntelligence(TRUE);
					bIsFixedValue	=	g_hero.isFixedIntelligence();
				}
				if (i==	5)
				{
					iValue1			=	g_hero.getWisdom();
					iValue2			=	g_hero.getWisdom(TRUE);
					bIsFixedValue	=	g_hero.isFixedWisdom();
				}
				if (i==	6)
				{
					iValue1			=	g_hero.getCharisma();
					iValue2			=	g_hero.getCharisma(TRUE);
					bIsFixedValue	=	g_hero.isFixedCharisma();
				}

				if	(bIsFixedValue)
				{
					if (iValue1-iValue2	>=	0)
						strcpy(strColor,"LTPINK");
					else
						strcpy(strColor,"LTRED");

					sprintf(strValue,"<c:%s>%d<n><<c:219,192,101>%d<n>>",strColor,iValue1,iValue2);
				}
				else
				if (iValue1	-	iValue2	== 0)
					sprintf(strValue,"<c:219,192,101>%d<n>",iValue1);
				else
				{
					if (iValue1-iValue2	>	0)
					{
						strcpy(strColor,"LTGREEN");
						strcpy(strPlus,"+");
					}
					else
					{
						strcpy(strColor,"LTRED");
						strcpy(strPlus,"-");
					}

					sprintf(strValue,"<c:%s>%d<n><<c:219,192,101>%d<n>%s<c:%s>%d<n>>",strColor,iValue1,iValue2,strPlus,strColor,abs(iValue1-iValue2));
				}

				if	(i	==	7)
				{
					if	(g_hero.m_bf4CH5StateBonusCount)
					{
						sprintf(strToolTip,dMSG_RECEIVE_CH5_STATE_POINT_COUONT_FORM,g_hero.m_bf4CH5StateBonusCount,g_hero.m_bf4CH5StateBonusCount*5);
					}
					else
						continue;
				}
				else
				{
					if	(strlen(g_pstrStatusTooltip[7+7*g_hero.m_wJob+i]) > 2)
						sprintf(strToolTip," <c:LTCYAN>%s<n> %s\n- %s\n- %s",g_pstrEquipUseLimitContents[iStatus],strValue,g_pstrStatusTooltip[i],g_pstrStatusTooltip[7+7*g_hero.m_wJob+i]);
					else
						sprintf(strToolTip," <c:LTCYAN>%s<n> %s\n- %s",g_pstrEquipUseLimitContents[iStatus],strValue,g_pstrStatusTooltip[i]);
				}

				rect.x1		=	iX;
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strToolTip,eTFM_LEFT);
			}
		}

		dwColor	=	RGB(219,192,101);
		if (g_hero.getRevisionStrength()	>	0)
		{
			dwColor	=	LTGREEN;
			if (g_hero.isFixedStrength())
				dwColor	=	LTPINK;
		}
		if (g_hero.getRevisionStrength()	<	0)	dwColor	=	LTRED;

		if (l_iFocusStatusContents	==	0)
			s_sText.put(iX+19				,iY   ,RGB(255,255,100),dMSG_STRENGTH);
		else
			s_sText.put(iX+19				,iY   ,RGB(187,142,76),dMSG_STRENGTH);

		s_sText.put(iX+19+iPointUpWidth	,iY+15,dwColor,"%d",g_hero.getStrength());

//	민첩성
		dwColor	=	RGB(219,192,101);
		if (g_hero.getRevisionAgility() > 0)
		{
			dwColor	=	LTGREEN;
			if (g_hero.isFixedAgility())
				dwColor	=	LTPINK;
		}
		if (g_hero.getRevisionAgility() < 0)	dwColor	=	LTRED;
		if (l_iFocusStatusContents	==	1)
			s_sText.put(iX+78				,iY   ,RGB(255,255,100),dMSG_AGILITY);
		else
			s_sText.put(iX+78				,iY   ,RGB(187,142,76),dMSG_AGILITY);

		s_sText.put(iX+78+iPointUpWidth	,iY+15,dwColor,"%d",g_hero.getAgility());

//	건강
		dwColor	=	RGB(219,192,101);
		if (g_hero.getRevisionConstitution() > 0)
		{
			dwColor	=	LTGREEN;
			if (g_hero.isFixedConstitution())
				dwColor	=	LTPINK;
		}
		if (g_hero.getRevisionConstitution() < 0)	dwColor	=	LTRED;
		if (l_iFocusStatusContents	==	2)
			s_sText.put(iX+137				,iY   ,RGB(255,255,100),dMSG_HEALTH);
		else
			s_sText.put(iX+137				,iY   ,RGB(187,142,76),dMSG_HEALTH);
		s_sText.put(iX+137+iPointUpWidth,iY+15,dwColor,"%d",g_hero.getConstitution());

//	운
		dwColor	=	RGB(219,192,101);
		if (g_hero.getRevisionLuck() > 0)
		{
			dwColor	=	LTGREEN;
			if (g_hero.isFixedLuck())
				dwColor	=	LTPINK;
		}
		if (g_hero.getRevisionLuck() < 0)	dwColor	=	LTRED;
		if (l_iFocusStatusContents	==	3)
			s_sText.put(iX+196				,iY   ,RGB(255,255,100),dMSG_LUCK);
		else
			s_sText.put(iX+196				,iY   ,RGB(187,142,76),dMSG_LUCK);
		s_sText.put(iX+196+iPointUpWidth,iY+15,dwColor,"%d",g_hero.getLuck());

//------------------------------------------------------------------------------	
		iY		=	178+g_iCorrectBI_Y/2;

//	지식
		dwColor	=	RGB(219,192,101);
		if (g_hero.getRevisionIntelligence() > 0)
		{
			dwColor	=	LTGREEN;
			if (g_hero.isFixedIntelligence())
				dwColor	=	LTPINK;
		}
		if (g_hero.getRevisionIntelligence() < 0)	dwColor	=	LTRED;
		if (l_iFocusStatusContents	==	4)
			s_sText.put(iX+19				,iY   ,RGB(255,255,200),dMSG_INTELLIGENCE);
		else
			s_sText.put(iX+19				,iY   ,RGB(187,142,76),dMSG_INTELLIGENCE);
		s_sText.put(iX+19+iPointUpWidth	,iY+15,dwColor,"%d",g_hero.getIntelligence());

//	지혜
		dwColor	=	RGB(219,192,101);
		if (g_hero.getRevisionWisdom() > 0)
		{
			dwColor	=	LTGREEN;
			if (g_hero.isFixedWisdom())
				dwColor	=	LTPINK;
		}
		if (g_hero.getRevisionWisdom() < 0)	dwColor	=	LTRED;
		if (l_iFocusStatusContents	==	5)
			s_sText.put(iX+78				,iY   ,RGB(255,255,200),dMSG_WISDOM);
		else
			s_sText.put(iX+78				,iY   ,RGB(187,142,76),dMSG_WISDOM);
		s_sText.put(iX+78+iPointUpWidth	,iY+15,dwColor,"%d",g_hero.getWisdom());

//	카리스마
		dwColor	=	RGB(219,192,101);
		if (g_hero.getRevisionCharisma() > 0)
		{
			dwColor	=	LTGREEN;
			if (g_hero.isFixedCharisma())
				dwColor	=	LTPINK;
		}
		if (g_hero.getRevisionCharisma() < 0)	dwColor	=	LTRED;
		if (l_iFocusStatusContents	==	6)
			s_sText.put(iX+137					,iY   ,RGB(255,255,200),dMSG_CHARISMA);
		else
			s_sText.put(iX+137					,iY   ,RGB(187,142,76),dMSG_CHARISMA);
		s_sText.put(iX+137+iPointUpWidth	,iY+15,dwColor,"%d",g_hero.getCharisma());

//	포인트
		if	(g_hero.m_wLevelPoint	>	0	||	g_hero.m_bf4CH5StateBonusCount)
		{
			if (l_iFocusStatusContents	==	7)
				s_sText.put(iX+196	,iY   ,RGB(255,255,200),dMSG_POINT);
			else
				s_sText.put(iX+196	,iY   ,RGB(187,142,76),dMSG_POINT);
			
			s_text.put(iX+196	,iY+15,RGB(210,221,230),"+%d",g_hero.m_wLevelPoint);
			g_sprInterface.Put(iX+195,iY+14,eWINDOW_DOT_SEPERATE_BAR);
		}
	}

//	저항
	{
		iY		=	248+g_iCorrectBI_Y/2;
		
		s_sText.putRF(iX+34	+8	,iY,WHITE,"<c:LTYELLOW>%d<n>",g_hero.getFireResistance());
		s_sText.putRF(iX+73	+8	,iY,WHITE,"<c:LTYELLOW>%d<n>",g_hero.getWaterResistance());
		s_sText.putRF(iX+112+8	,iY,WHITE,"<c:LTYELLOW>%d<n>",g_hero.getWindResistance());
		s_sText.putRF(iX+151+8	,iY,WHITE,"<c:LTYELLOW>%d<n>",g_hero.getEarthResistance());
		s_sText.putRF(iX+190+8	,iY,WHITE,"<c:LTYELLOW>%d<n>",g_hero.getLightResistance());
		s_sText.putRF(iX+229+8	,iY,WHITE,"<c:LTYELLOW>%d<n>",g_hero.getDarkResistance());
	}

//	성향
	{
		iY		=	287+g_iCorrectBI_Y/2;

		int	iPos	=	115*(g_hero.m_sAllignment+100)/200;
		int	iFrame	=	20*(g_hero.m_sAllignment+100)/200;

		iFrame		=	20-iFrame;

		s_text.cPut(iX+80+iPos,iY,WHITE,"%d",g_hero.m_sAllignment);

		g_sprInterface.Put(iX+76+iPos-32,iY+14,eSW_ALLIGNMENT_GAUGE_ABSOLUTE_GOOD+iFrame);
		g_sprInterface.Put(iX+22,iY,eSW_ALLIGNMENT_FRAME);

		s_rectTooltip.set(iX+22,iY,iX+iWindowWidth-20,iY+30);

		if	(g_iHeavenRedStoneCount+g_iHellRedStoneCount+g_iRedDevilRedStoneCount && s_rectTooltip.isIn(s_posMouse.x,s_posMouse.y))
		{
			int	iExpBonus			=	g_hero.getExpBonusByA();

			int	iFireBonus			=	g_hero.getFireResistanceBonusByA();
			int	iWaterBonus			=	g_hero.getWaterResistanceBonusByA();
			int	iWindBonus			=	g_hero.getWindResistanceBonusByA();
			int	iEarthBonus			=	g_hero.getEarthResistanceBonusByA();
			int	iLightBonus			=	g_hero.getLightResistanceBonusByA();
			int	iDarkBonus			=	g_hero.getDarkResistanceBonusByA();

			int	iTradeBonus			=	g_hero.getTradeBonusByA();

			int	iItemDropChanceBonus=	g_hero.getItemDropChanceBonusByA();
			int	iItemDropLevelBonus	=	g_hero.getItemDropLevelBonusByA();

			cSTRING	string;

			if	(iExpBonus	>	0)
			{
				string.Add(dMSG_EXP_BONUS_FORM,iExpBonus/100,iExpBonus%100);
				string.Add("\n");
			}

			if	(iExpBonus	<	0)
			{
				string.Add(dMSG_EXP_PENELTY_FORM,iExpBonus/100,-iExpBonus%100);
				string.Add("\n");
			}

			if	(iTradeBonus	>	0)
			{
				string.Add(dMSG_TRADE_PENELTY_FORM,iTradeBonus);
				string.Add("\n");
			}
			if	(iTradeBonus	<	0)
			{
				string.Add(dMSG_TRADE_BONUS_FORM,iTradeBonus);
				string.Add("\n");
			}

			if	(iItemDropChanceBonus	>	0)
			{
				string.Add(dMSG_ITEM_DROP_CHANCE_BONUS_FORM,iItemDropChanceBonus);
				string.Add("\n");
			}

			if	(iItemDropLevelBonus	>	0)
			{
				string.Add(dMSG_ITEM_DROP_LEVEL_BONUS_FORM,iItemDropLevelBonus);
				string.Add("\n");
			}

			for (int i=0;i<6;i++)
			{
				int		iBonus		=	g_hero.getResistanceBonusByA(i);
				char	*lpstrAttr	=	g_pstrCommonMessage[dWM_ATTR_MESSAGE_BEGIN+i];

				if	(iBonus	>	0)
				{
					string.Add(dMSG_RESISTANCE_BONUS_FORM,lpstrAttr,iBonus);
					string.Add("\n");
				}
				if	(iBonus	<	0)
				{
					string.Add(dMSG_RESISTANCE_PENELTY_FORM,lpstrAttr,iBonus);
					string.Add("\n");
				}
			}

			if	(string.Length)
			{
				string.Insert("%s\n",dMSG_ALLIGNMENT_BONUS_INFO);
				string.Insert("\n");
			}

			string.Insert("\n");
			string.Insert(dMSG_RED_STONE_COUNT_FORM,g_iHeavenRedStoneCount,g_iHellRedStoneCount,g_iRedDevilRedStoneCount);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,string.String,eTFM_LEFT);
		}
	}

	// 10.02.18   
//	칭호
	{
		int	iCount			=	0;
		int iPutCount		=	0;
		
		if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXT)
		{
			iY					=	334+g_iCorrectBI_Y/2;
			iX					=	s_posStatusWindow.x+26;

			int iStatusCount = s_sbStatusWindow.m_iCurrentPos*2;
			
			for (int i=0;i<dMAX_TITLE_COUNT;i++)
			{
				CPlayerTitleInfo*	lpTitleInfo	=	g_hero.getTitleByIndex(i);
				
				if	(!lpTitleInfo)
					continue;
				
				CTitle*				lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);
				
				if	(!lpTitle	)
					continue;
				
				if(iCount < iStatusCount)
				{
					iCount++;
					continue;
				}
				
				int	iXPos	=	iX;
				
				if	(iPutCount%2==1)
					iXPos	=	s_posStatusWindow.x+138;
				
				char	strTitleName[dTITLE_NAME_LENGTH];
				
				strcpy(strTitleName,lpTitle->m_strName);
				
				if	(strlen(strTitleName)	>	14)
				{
					ustrncpy(strTitleName,lpTitle->m_strName,14);
					sprintf(strTitleName,"%s..",strTitleName);
				}
				
				if	(i	==	s_iFocusTitle)
					s_sText.put(iXPos,iY+(iPutCount/2)*13,LTYELLOW,"%s(<c:LTGREEN>%d<n>)",strTitleName,lpTitleInfo->m_bLevel);
				else
					s_sText.put(iXPos,iY+(iPutCount/2)*13,BTPURPLE,"%s(<c:GREEN>%d<n>)",strTitleName,lpTitleInfo->m_bLevel);
				
				iCount++;
				iPutCount++;
				
				if(iPutCount >= dTITLE_COUNT_IN_WINDOW_TYPE_TEXT * 2)
					break;
			}
		}	//	if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXT)
		else if( g_config.m_bf2TitleViewType == eTITLE_TYPE_IMAGE)
		{
			iY					=	350+g_iCorrectBI_Y/2;
			iX					=	s_posStatusWindow.x+42;

			int iStatusCount = s_sbStatusWindow.m_iCurrentPos*8;

			for (int i=0;i<dMAX_TITLE_COUNT;i++)
			{
				CPlayerTitleInfo*	lpTitleInfo	=	g_hero.getTitleByIndex(i);
				
				if	(!lpTitleInfo)
					continue;
				
				CTitle*				lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);
				
				if	(!lpTitle	)
					continue;
				
				if(iCount < iStatusCount)
				{
					iCount++;
					continue;
				}
				
				int	iXPos	= iX + 27 * (iPutCount % 8);
				int iYPos	= iY + 36 * (iPutCount / 8);

				int iImage = GetTitleImageIndex(i);
				
				if(iImage != 0xffff)
					g_sprTitle.Put(iXPos,iYPos,iImage);
												
				int iNumber = lpTitleInfo->m_bLevel;
								
				if(iNumber >= 10)
				{
					g_sprInterface.Put(iXPos-8 ,iYPos-8,eSN_0+1);
					g_sprInterface.Put(iXPos-5,iYPos-8,eSN_0);
				}
				else
					g_sprInterface.Put(iXPos-8,iYPos-8,eSN_0 + iNumber);
							
				iCount++;
				iPutCount++;

				if(iPutCount >= dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE * 8)
					break;
			}
		}
		else if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXTIMAGE)
		{
			iY					=	350+g_iCorrectBI_Y/2;
			iX					=	s_posStatusWindow.x+42;
			
			int iStatusCount = s_sbStatusWindow.m_iCurrentPos*2;
			
			for (int i=0;i<dMAX_TITLE_COUNT;i++)
			{
				CPlayerTitleInfo*	lpTitleInfo	=	g_hero.getTitleByIndex(i);
				
				if	(!lpTitleInfo)
					continue;
				
				CTitle*				lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);
				
				if	(!lpTitle	)
					continue;
				
				if(iCount < iStatusCount)
				{
					iCount++;
					continue;
				}
				
				int	iXPos	= iX + 113 * (iPutCount % 2);
				int iYPos	= iY + 36  * (iPutCount / 2);
				
				int iImage = GetTitleImageIndex(i);
				
				if(iImage != 0xffff)
					g_sprTitle.Put(iXPos,iYPos,iImage);

				char	strTitleName[dTITLE_NAME_LENGTH];
				strcpy(strTitleName,lpTitle->m_strName);

				if	(strlen(strTitleName)	>	12)
				{
					ustrncpy(strTitleName,lpTitle->m_strName,14);
					sprintf(strTitleName,"%s..",strTitleName);
				}
				
				if	(i	==	s_iFocusTitle)
				{
					s_sText.put(iXPos+14,iYPos-12,LTYELLOW,"%s",strTitleName);
					s_sText.put(iXPos+14,iYPos+2,LTYELLOW,"(<c:LTGREEN>%d<n>)",lpTitleInfo->m_bLevel);
				}
				else
				{
					s_sText.put(iXPos+14,iYPos-12,BTPURPLE,"%s",strTitleName);
					s_sText.put(iXPos+14,iYPos+2,BTPURPLE,"(<c:GREEN>%d<n>)",lpTitleInfo->m_bLevel);
				}
				
				iCount++;
				iPutCount++;
				
				if(iPutCount >= dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE * 2)
					break;
			}
		}
	}

	if (s_sbPopupWindow.isVisible())	
		s_sbPopupWindow.draw();

	s_bmPlusState.draw();

	g_sprInterface.Put(s_posStatusWindow.x,s_posStatusWindow.y	,eWINDOW);
	g_sprInterface.Put(s_posStatusWindow.x,g_iCorrectBI_Y/2,eWINDOW_TITLE_STATUS);

	g_gwPet.draw();
	// 10.02.18	
	s_bmStatusButton.draw();			
	
	if(s_bIsTitleScrollBar)
		s_sbStatusWindow.draw();
}
int
CGamePlay::GetTitleImageIndex(int _iIndex)
{
	int Image = 0xffff;

	for(int i=0; i<eTI_ICON_END; i++)
		if(l_iaTitleImageIndex[i] == _iIndex)
		{
			Image = i;
			break;
		}

	return Image;
}

int
CGamePlay::CheckTitleIndex()
{
	int iCount = 0;

	for (int i=0;i<dMAX_TITLE_COUNT;i++)
	{
		int	iTitleIndex	=	i+s_iStatusWindowScrollBarPos*2;
		
		CPlayerTitleInfo*	lpTitleInfo	=	g_hero.getTitleByIndex(iTitleIndex);
		
		if	(!lpTitleInfo)
			continue;
		
		CTitle*				lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);
		
		if	(!lpTitle	)
			continue;
		
		iCount++;
	}
	
	return iCount;
}

void
CGamePlay::SetTitleType(int _iType)
{
	int	iX					= s_posStatusWindow.x;
	int	iY					= s_posStatusWindow.y;
	int iCheckTitleIndex	= CheckTitleIndex();
	int iLineIndex			= 0;
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	s_sbStatusWindow.setPos(0);		// 추가
	g_config.m_bf2TitleViewType	= _iType;
	s_bIsTitleScrollBar			= FALSE;
		
	if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXT)
	{
		if( iCheckTitleIndex/2 > dTITLE_COUNT_IN_WINDOW_TYPE_TEXT || (iCheckTitleIndex/2 == dTITLE_COUNT_IN_WINDOW_TYPE_TEXT && iCheckTitleIndex%2!=0) )
		{
			s_bIsTitleScrollBar = TRUE;
			iLineIndex			= iCheckTitleIndex/2;

			if(iCheckTitleIndex%2 != 0)
				iLineIndex += 1;

			s_sbStatusWindow.set(iX+iWindowWidth-16,332+g_iCorrectBI_Y/2,160);
			s_sbStatusWindow.setPos(0);
			s_sbStatusWindow.setSize(iLineIndex,dTITLE_COUNT_IN_WINDOW_TYPE_TEXT);
		}
	}
	else if( g_config.m_bf2TitleViewType == eTITLE_TYPE_IMAGE)
	{
		if( iCheckTitleIndex/8 > dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE || (iCheckTitleIndex/8 == dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE && iCheckTitleIndex%8!=0) )
		{
			s_bIsTitleScrollBar = TRUE;
			iLineIndex			= iCheckTitleIndex/8;
			
			if(iCheckTitleIndex%8 != 0)
				iLineIndex += 1;

			s_sbStatusWindow.set(iX+iWindowWidth-16,332+g_iCorrectBI_Y/2,160);
			s_sbStatusWindow.setPos(0);
			s_sbStatusWindow.setSize(iLineIndex,dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE);
		}
	}
	else if( g_config.m_bf2TitleViewType == eTITLE_TYPE_TEXTIMAGE)
	{
		if( iCheckTitleIndex/2 > dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE || (iCheckTitleIndex/2 == dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE && iCheckTitleIndex%2!=0) )
		{
			s_bIsTitleScrollBar = TRUE;
			iLineIndex			= iCheckTitleIndex/2;
			
			if(iCheckTitleIndex%2 != 0)
				iLineIndex += 1;

			s_sbStatusWindow.set(iX+iWindowWidth-16,332+g_iCorrectBI_Y/2,160);
			s_sbStatusWindow.setPos(0);
			s_sbStatusWindow.setSize(iLineIndex,dTITLE_COUNT_IN_WINDOW_TYPE_IMAGE );
			
		}
	}	
}


void
CGamePlay::UpdateTitleType()
{
	DWORD dwMenu = s_bmStatusButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);
	
	if (dwMenu	==	dTITLE_CHANGE_TYPE)	
	{
		int iTypeIndex = g_config.m_bf2TitleViewType;
		iTypeIndex + 1 > eTITLE_TYPE_TEXTIMAGE ? iTypeIndex = eTITLE_TYPE_TEXT : iTypeIndex += 1; 
	
		SetTitleType(iTypeIndex);
	}
	
	if (s_bmStatusButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_INTERFACE);
	else	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_INTERFACE);
}

void
CGamePlay::OperateTitleScrollBar()
{
	if(!s_bIsTitleScrollBar)
		return;

	if (cINPDEV::IsExclusive(eMOUSE_STATE_SCROLL_BAR))
		return;

	int	iPos	=	s_sbStatusWindow.m_iCurrentPos;
	
	cRECT rect = {560, 334, 784, 500};
		
	if (rect.isIn(s_posMouse.x,s_posMouse.y))
	{
		if (s_iWheelValue<0)
			s_sbStatusWindow.setPos(iPos+1);
		if (s_iWheelValue>0)
			s_sbStatusWindow.setPos(iPos-1);
	}

	if (s_sbStatusWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
		cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
	else
		cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);
}