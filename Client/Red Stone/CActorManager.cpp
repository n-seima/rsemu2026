#include "CActor.H"
#include "CFieldItem.H"
#include "CAgent.H"
#include "CMessage.H"
#include "CPitchmanShop.H"
#include "CPortal.H"
#include "CImageData.H"
#include "arca.H"
#include "CPartyWindow.H"
#include "trap.H"
#include "hero_mini_pet.H"
#include "book_MonsterDictionary.h"
#include "carrotShop.h"		// insu add
CActorManager	g_am;

//JBC 08-07-15
#define	dSCROLL_HIGH	15
#define	dSCROLL_LOW		5

int l_iSpeed = dSCROLL_LOW;

//JBC 08-07-15
BOOL	g_bObserverMode = FALSE;

the_fallowing_motion	l_axisPosSpring;

//
//	초기화
CActorManager::CActorManager()
{
	m_iDenyCommunityActorCount			=	0;

	reset();
}	//	CActorManager::CActorManager()

//
//	리셋
void
CActorManager::reset()
{
	m_screenFocus.reset();
	m_posStress.x						=	-1;
	m_posStress.y						=	-1;
	m_iSmoothScrollTime					=	0;
	m_iIsHideTimathFeelerTime			=	0;
	
	m_wFocusActor						=	0xffff;

	m_iTraceTarget						=	0xffff;
	m_wCurrentIndex						=	0;
	m_wAskInfoActorCount				=	0;
	m_wMisplacedActorCount				=	0;
	m_wObscurityActorCount				=	0;
	m_wWaitAskActorInfoResultTime		=	0;
	m_wWaitMisplacedActorResultTime		=	0;
	m_wWaitObscurityActorInfoResultTime	=	0;
	m_wOXQuizLoserCountInServer			=	0;
	m_wOXQuizSurvivorCountInServer		=	0;
	m_wOXQuizWinnerCount				=	0;
	m_wOXQuizBattleCount				=	0;
	m_iDetectingActorCount				=	0;
	m_iDetectingActorTime				=	0;

	m_iActorCount						=	0;	//	캐릭터 수
	m_iLastAskActorInfoTime				=	0;
	m_posScreenDelta.x					=	0;
	m_posScreenDelta.y					=	0;
	m_wTalker							=	0xffff;

	g_lpControlActor					=	NULL;
	g_lpHero							=	NULL;

	for (int i=0;i<dMAX_ACTOR;i++)
		g_aActor[i].reset();

	memset(m_aAskInfoActorList,0xff,sizeof(m_aAskInfoActorList));
	memset(m_aObscurityActorList,0xff,sizeof(m_aObscurityActorList));

	l_axisPosSpring.set_mode(0);
	l_axisPosSpring.set_mass( 10 );
	l_axisPosSpring.set_friction( 0.10f );
	l_axisPosSpring.set_elasticity( 7.5f );
	l_axisPosSpring.set_rotate_mass ( 5.0f );
	l_axisPosSpring.set_rotate_friction ( 0.3f );
	l_axisPosSpring.set_rotate_elasticity ( 1.5f );

	l_axisPosSpring.set_current_parent_pos(-1,-1);
}

//
//	클로즈
void
CActorManager::close()
{
	reset();
}

//
//	모든 캐릭터를 업데이트 한다.
void
CActorManager::update()
{

	if	(!g_lpHero)
		return;

	g_hero.m_wNonBattleTime++;

	if	(g_hero.m_wNonBattleTime	>=	15*dSYNC_FPS)
	{
		g_hero.m_wNonBattleTime	=	0;

		int	iIndex	=g_hero.m_wActiveMiniPet;

		if	(g_hero.m_wActiveMiniPet2	!=	c_iMiniPetCount)
		{
			if	(random(2)	==	0)
				iIndex	=	g_hero.m_wActiveMiniPet;
		}

		g_hero.iMiniPet()->playFX(eMPFX_IDLE,iIndex,FALSE);
	}

	if	(g_hero.m_wPrincessWeaponOwner	!=	0xffff)
	{
		CActor	*lpActor	=	getActor(g_hero.m_wPrincessWeaponOwner);

		if	(lpActor)
			g_lpHero->setPos(lpActor->m_pos.x,lpActor->m_pos.y);

		g_lpHero->m_enchantedImage.m_bf1IsTransToWeapon	=	TRUE;
	}

	m_rectScreen.set(0,0,g_iScreenWidth-(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1), g_iScreenHeight);
	m_screenFocus.update();

	if	(GetMMCPosToReal())
		m_rectScreen.set(-100000,-100000,-100001,-100001);

	g_hero.updateStatus();	//	영웅 상태 업데이트

	if (m_iDetectingActorTime)
	{
		m_iDetectingActorTime	--;

		if (m_iDetectingActorTime	==	0)
			m_iDetectingActorCount	=	0;
	}

	if	(g_hero.m_wWaitGuildMarkInfoTime)
		g_hero.m_wWaitGuildMarkInfoTime--;

	

	//JBC	08-07-15
	if(g_bObserverMode)
	{
		if(MoveObserverPos())
			g_bObserverMode = FALSE;
		setObserverMode();
	}
// insu add
	BOOL bInputCancel = FALSE;
	if (g_map.m_bQuestListState == TRUE)
	{
		cRECT rectTemp;
		rectTemp.set(	g_map.m_QuestListArea[0].x1,
						g_map.m_QuestListArea[0].y1 - g_sprInterface.getSpriteHeight(eWINDOW_TITLE_QUEST_OLD),
						g_map.m_QuestListArea[0].x2,
						g_map.m_QuestListArea[dMAX_PROCESS_QUEST_COUNT].y2);
		if (g_hero.getProcessQeustCount() == 0)
			rectTemp.y2 = g_map.m_QuestListArea[0].y2;
		if (s_isTouchLeftButton && rectTemp.isIn(s_posMouse.x, s_posMouse.y) || s_isLeftButtonPressed && rectTemp.isIn(s_posMouse.x, s_posMouse.y))
				bInputCancel = TRUE;
	}

	
	if (s_iPopupInterface == ePIW_CARROT_SHOP)
	{
		if (s_rectInterfaceSize.isIn(s_posMouse.x, s_posMouse.y))
			bInputCancel = TRUE;

		if (!s_rectInterfaceSize.isIn(s_posMouse.x, s_posMouse.y) && g_gwCarrotShop.m_sbScrollBar.m_isGrabbedBody == TRUE)
			bInputCancel = TRUE;
	}



	if	(g_lpHero->m_wJumpStatus	==	eJS_NONE && g_bObserverMode == FALSE && bInputCancel == FALSE)
	{
		if	(!IsOpenedEventWindow() && s_bIsObserverMode == FALSE && g_bObserverMode == FALSE)
		{
			findFocusActor();	//	마우스가 가르키고 있는 녀석을 찾는다.
			findFocusCorpse();	//	시체 찾기

			updateInpdev();		//	입력 업데이트
		}

		updateMove();		//	이동 관련 업데이트

		if	(s_iFocusArea == 0xffff && g_map.m_area.m_iFocusedArea != 0xffff)
			g_map.m_area.releaseMoveArea();
	}
	

	m_wOXQuizLoserCount		=	0;
	m_wOXQuizSurvivorCount	=	0;

	if	(m_iIsHideTimathFeelerTime)
		m_iIsHideTimathFeelerTime--;

	for (int i=0;i<m_iActorCount;)
	{
		CActor	*lpActor	=	&g_aActor[m_aActorList[i]];

		if (g_hero.isOperator())
		{
			if (!lpActor->isHighOperator() && lpActor->m_bf1IsQuizEventWinner == FALSE) // 영자도 아니고 OX퀴즈 승자도 아니다.
			{
				if (lpActor->m_bf1IsQuizEventLoser)
					m_wOXQuizLoserCount++;
				else
					m_wOXQuizSurvivorCount++;
			}
		}

		if (lpActor->update())
			i++;	//	업데 하다 죽은 넘들 있으면 카운트 안 올린다.
	}

	{
		sendAskInfoActorList();
		sendMisplacedActorList();	//	위치가 잘못된 넘들 데이터 요구
		sendObscurityActorList();	//	위치가 잘못된 넘들 데이터 요구
		sendAskActorEnchantInfoList();
	}
	
	if(g_bObserverMode == FALSE)
		setFocus();
}

int
CActorManager::getScreenActorList(CActor **_alpActorList,int _iBufferSize)
{
	int	iCount	=	0;
	
	for (int i=0;i<m_iActorCount && iCount < _iBufferSize;i++)
	{
		CActor	*lpActor	=	&g_aActor[m_aActorList[i]];

		if	(lpActor->isHero())
			continue;

		if (!g_rectViewPort.isIn(lpActor->m_pos.x,lpActor->m_pos.y))
			continue;

		_alpActorList[iCount]	=	lpActor;

		iCount++;
	}

	return	iCount;
}

//
//	데이터
void
CActorManager::setHero(CHero *_lpPlayer)
{
	g_aActor[_lpPlayer->m_iSerial].setDataByHero(_lpPlayer);
	g_lpHero	=	&g_aActor[_lpPlayer->m_iSerial];

	rebuildActorList();
}

void
CActorManager::setGroundFocus(int _iX,int _iY,int _iTime)
{
	if	(_iX	==	0)
	{
		releaseKarmaFocus();
		return;
	}

	m_screenFocus.setFocus(m_posFocus.x,m_posFocus.y,_iX,_iY,_iTime);
}

void
CActorManager::setCharacterFocus(int _iActor)
{
	m_wFocusActor	=	_iActor;

	getTestedActor(m_wFocusActor);
}

void
CActorManager::releaseKarmaFocus()
{
	m_wFocusActor		=	0xffff;
	m_screenFocus.reset();
}

void
CActorManager::getFocusPos(CPos *_lpPos)
{
	CPos	pos;

	if	(m_screenFocus.getCurrentFocus(&pos))
	{
		_lpPos->x	=	pos.x;
		_lpPos->y	=	pos.y;
		return;
	}

	if	(m_wFocusActor		!=	0xffff)
	{
		CActor	*lpFocusActor=	getTestedActor(m_wFocusActor);

		if	(lpFocusActor)
		{
			_lpPos->x	=	lpFocusActor->m_pos.x;
			_lpPos->y	=	lpFocusActor->m_pos.y;
			return;
		}
	}

	_lpPos->x	=	g_lpHero->m_pos.x;
	_lpPos->y	=	g_lpHero->m_pos.y;
}


void
CActorManager::ChangeObserverMode(BOOL _isByKarma , int _iX , int _iY , int _iTime , int _iTimeForWait)
{
	CPos posNow;

	g_lpHero->getHitPoint(&posNow);

	SetCorrectPos(&posNow);

	m_posObserber.Set(posNow.x ,  posNow.y + dTILE_YS);

	_iX *= dTILE_XS;
	_iY *= dTILE_YS;

	_iX += g_iScreenWidth / 2 - dTILE_XS;
	_iY += g_iScreenHeight / 2;

	if(_isByKarma)					//스크린 이동 이라면..
	{
		m_iObserberTime		= _iTime;
		m_iObserberWaitTime	= _iTimeForWait;			
		m_fObserberSpeedX	= _iX - m_posObserber.x;
		m_fObserberSpeedX	= m_fObserberSpeedX / (m_iObserberTime) / dSYNC_FPS;
		m_fObserberSpeedY	= _iY - m_posObserber.y;		
		m_fObserberSpeedY	= m_fObserberSpeedY / (m_iObserberTime) / dSYNC_FPS;
	}
}

BOOL
CActorManager::MoveObserverPos()
{
	if(m_iObserberTime)
	{
		int iSpeedX = int(m_fObserberSpeedX + m_fObserberRemainX);
		int iSpeedY = int(m_fObserberSpeedY + m_fObserberRemainY);

		m_posObserber.x += iSpeedX;
		m_posObserber.y += iSpeedY;

		m_fObserberRemainX += m_fObserberSpeedX - iSpeedX;
		m_fObserberRemainY += m_fObserberSpeedY - iSpeedY;

		return FALSE;
	}
	else if(m_iObserberWaitTime)
	{
		return FALSE;
	}

	return TRUE;
}


//JBC 옵져버 모드로 전환 08-07-15
void
CActorManager::setObserverMode()
{
	int	iScreenHeight	=	g_iFieldScreenHeight;
	CPos	posAxis;
	
	getFocusPos(&m_posFocus);
	getFocusPos(&posAxis);
	
	if	(l_axisPosSpring.get_parent_pos().x	==	-1.0f)
	{
		l_axisPosSpring.set_current_parent_pos(posAxis.x,posAxis.y);
		l_axisPosSpring.stickToParent();
	}
	else
		l_axisPosSpring.set_current_parent_pos(posAxis.x,posAxis.y);
			td2dvector vi		=	l_axisPosSpring.get_child_pos();

	if	(s_bIsHideInterface)
		iScreenHeight	=	g_iScreenHeight;
	
	int	x	=	m_posObserber.x - (g_iScreenWidth-(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))/2;	
	int	y	=	m_posObserber.y - iScreenHeight*7/13;
	
	ClipValue(x,dTILE_XS,g_map.m_iPixelWidth	-g_iScreenWidth	-1-dTILE_XS);		//	클리핑
	ClipValue(y,dTILE_YS,g_map.m_iPixelHeight	-iScreenHeight	-1-dTILE_YS);
	
	g_map.setPos(x,y);
	g_map.m_posView.Set(x,y);

	l_axisPosSpring.update();
}

void
CActorManager::SetViewPort()
{
	g_rectViewPort.set(	m_posObserber.x - dVIEWPORT_MARGIN,
		m_posObserber.y - dVIEWPORT_MARGIN,
		m_posObserber.x + cDRAW::WIDTH	+ dVIEWPORT_MARGIN,
		m_posObserber.y + cDRAW::HEIGHT + dVIEWPORT_MARGIN);
}

void
CActorManager::SetViewPortLargeMargin()
{
	g_rectViewPort.set(	m_posObserber.x - dVIEWPORT_MARGIN * 5,
		m_posObserber.y - dVIEWPORT_MARGIN * 5,
		m_posObserber.x + cDRAW::WIDTH	+ dVIEWPORT_MARGIN * 5,
		m_posObserber.y + cDRAW::HEIGHT + dVIEWPORT_MARGIN * 5);
}

void
CActorManager::OperateObserverMouse()
{
	if	(s_isRightButtonPressed && s_tlChat.getFocusLine(s_posMouse.x,s_posMouse.y)	==	-1)
	{
		m_posObserber.x		=	s_posCursor.x;
		m_posObserber.y		=	s_posCursor.y;
		
		CPos	pos;
		
		if	(GetMMCPosToReal(&pos))
		{
			m_posObserber.x		=	pos.x;
			m_posObserber.y		=	pos.y;
		}

		SetCorrectPos(&m_posObserber);
	}
}

void
CActorManager::SetCorrectPos(CPos* _Pos)
{
	if(_Pos->x < cDRAW::WIDTH/2)
	{
		_Pos->x = cDRAW::WIDTH/2;
	}
	else if(_Pos->x > g_map.m_iPixelWidth - cDRAW::WIDTH/2 - dTILE_XS)
	{
		_Pos->x = g_map.m_iPixelWidth - cDRAW::WIDTH/2 - dTILE_XS;
	}
	else if(_Pos->y < cDRAW::HEIGHT/2)
	{
		_Pos->y = cDRAW::HEIGHT/2;
	}
	else if(_Pos->y > g_map.m_iPixelHeight - cDRAW::HEIGHT/2 - dTILE_YS)
	{
		_Pos->y = g_map.m_iPixelHeight - cDRAW::HEIGHT/2 - dTILE_YS;
	}
}

void
CActorManager::OperateObserverKeyboard()
{
	if (s_aKey[KEY_LSHIFT])
		l_iSpeed = dSCROLL_HIGH;
	else
		l_iSpeed = dSCROLL_LOW;
	
	if (s_aKey[KEY_RIGHT])
	{
		if(g_map.m_iPixelWidth > m_posObserber.x + cDRAW::WIDTH/2 + dTILE_XS)
			m_posObserber.x += l_iSpeed;
	}
	
	if (s_aKey[KEY_LEFT])
	{
		if (m_posObserber.x > cDRAW::WIDTH/2)
			m_posObserber.x-= l_iSpeed;
	}
	
	if (s_aKey[KEY_UP])
	{
		if (m_posObserber.y > cDRAW::HEIGHT/2)
			m_posObserber.y-= l_iSpeed;
	}
	
	if (s_aKey[KEY_DOWN])
	{
		if(g_map.m_iPixelHeight > m_posObserber.y + cDRAW::HEIGHT/2 + dTILE_YS)
			m_posObserber.y+= l_iSpeed;
	}
}

//
//	플레이어에게 포커스를 맞춘다.
void
CActorManager::setFocus()
{
	if	(!s_isHeroFocusOn			)
		return;

	if	(g_hero.m_bf1IsNotFocusStatus)
		return;

	if	(m_iSmoothScrollTime)
		--m_iSmoothScrollTime;

	if	(g_lpHero && s_isHeroFocusOn)	//	영웅이 있고 영웅에 포커스 맞추기가 on 상태이다.
	{
		CPos	posAxis;

		getFocusPos(&m_posFocus);
		getFocusPos(&posAxis);
		
		{
			if	(l_axisPosSpring.get_parent_pos().x	==	-1.0f)
			{
				l_axisPosSpring.set_current_parent_pos(posAxis.x,posAxis.y);
				l_axisPosSpring.stickToParent();
			}
			else
				l_axisPosSpring.set_current_parent_pos(posAxis.x,posAxis.y);
			td2dvector vi		=	l_axisPosSpring.get_child_pos();
		
			if	(m_iSmoothScrollTime)
			{
				posAxis.x			=	vi.x;
				posAxis.y			=	vi.y;
			}
		}

		int	iScreenHeight	=	g_iFieldScreenHeight;
		
		if	(s_bIsHideInterface)
			iScreenHeight	=	g_iScreenHeight;
		
		int	x	=	posAxis.x	-	(g_iScreenWidth-(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))/2;
		int	y	=	posAxis.y	-	iScreenHeight*7/13;

		if (s_iPopupInterface == ePIW_CARROT_SHOP && g_config.b_bIsNewCarrotShop)
			x = posAxis.x - g_iScreenWidth/2;
		
		ClipValue(x,dTILE_XS,g_map.m_iPixelWidth	-g_iScreenWidth	-1-dTILE_XS);		//	클리핑
		ClipValue(y,dTILE_YS,g_map.m_iPixelHeight	-iScreenHeight	-1-dTILE_YS);
		
		g_map.setPos(x,y);
		g_map.m_posView.Set(x,y);

		if	(g_bIsHalfSize)
		{
			getFocusPos(&posAxis);

			posAxis.x			/=	2;
			posAxis.y			/=	2;
			
			{
				if	(l_axisPosSpring.get_parent_pos().x	==	-1.0f)
				{
					l_axisPosSpring.set_current_parent_pos(posAxis.x,posAxis.y);
					l_axisPosSpring.stickToParent();
				}
				else
					l_axisPosSpring.set_current_parent_pos(posAxis.x,posAxis.y);

				td2dvector vi		=	l_axisPosSpring.get_child_pos();
				
				if	(m_iSmoothScrollTime)
				{
					posAxis.x			=	vi.x;
					posAxis.y			=	vi.y;
				}
			}
			
			int	iScreenHeight	=	g_iFieldScreenHeight;
			
			if	(s_bIsHideInterface)
				iScreenHeight	=	g_iScreenHeight;
			
			int	x	=	posAxis.x	-	(g_iScreenWidth-(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1))/2;
			int	y	=	posAxis.y	-	iScreenHeight*7/13;
			
			ClipValue(x,dTILE_XS,g_map.m_iPixelWidth/2	-g_iScreenWidth	-1-dTILE_XS);		//	클리핑
			ClipValue(y,dTILE_YS,g_map.m_iPixelHeight/2	-iScreenHeight	-1-dTILE_YS);
			
			s_isSmoothFocusing	=	FALSE;

			g_map.m_posView.Set(x,y);
		}
	}

	l_axisPosSpring.update();
}

//
//	액터 수등의 정보를 재 설정한다.
void
CActorManager::rebuildActorList()
{
	int		i;

	m_iActorCount	=	0;											//	캐릭터 수

	for (i=0;i<dMAX_ACTOR;i++)
		if (g_aActor[i].m_wSerial!=0xffff)
			m_aActorList[m_iActorCount++]	=	i;

}

//
//	특정 영역 안에 캐릭터를 구한다.
int
CActorManager::getViewActors(cINDEX *_lpIndexes,cRECT *_lpRectViewport)
{
	int		iCount	=	0,i;

	for (i=0;i<m_iActorCount;i++)		//	아군 캐릭터
	{
		CActor	*lpActor	=	getTestedActor(m_aActorList[i],FALSE);

		if	(!lpActor || lpActor->isCorpse())
			continue;	//	죽은 넘들은 따로 찍는다.

		int		iX	=	lpActor->m_pos.x;
		int		iY	=	lpActor->m_pos.y;

		if	(lpActor->m_wIsFeignDeath)
		{
			iX		=	lpActor->m_posFeignDeath.x;
			iY		=	lpActor->m_posFeignDeath.y;
		}

		if	(_lpRectViewport->isIn(iX,iY))
		{	
			_lpIndexes[iCount].Index	=	dBORDER_ACTOR+lpActor->m_wSerial;
			_lpIndexes[iCount].Value	=	iY;
			iCount++;
		}
	}

	return iCount;
}

//
//	현재 마우스 커서의 위치에 있는 캐릭터를 찾아서 포커스로 설정한다.
void
CActorManager::findFocusActor()
{

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_PROPOSE_TO_WORK))
		return;

	s_iFocusActor	=	0xffff;

	if	(IsExclusiveFocus() || s_bIsEventField || s_bIsIgnoreAllFocus)
		return;

	if	(	g_lpHero->m_enchantedImage.m_bf1IsFeignDeath || g_lpHero->m_enchantedImage.m_bf1IsPlump || g_lpHero->m_enchantedImage.m_bf1IsRabbit || 
			g_lpHero->m_enchantedImage.m_bf1IsFrog)
		return;

	if(g_lpHero->m_enchantedImage.m_bf1IsRushTime)		// 돌진상태
		return;

	cRECT rect;
	rect.set(s_tlChat.getRect());
	cRECT rtCheckHeight, rtCheckWidth;
	rtCheckHeight.set(0, rect.y1, 15, rect.y2);
	rtCheckWidth.set(0, rect.y2, rect.x1+135, rect.y2+15);

	if (s_iPopupInterface == ePIW_NONE && g_config.m_iMinimapWindowScale < 100  && s_rectTitle.isIn(s_posMouse.x, s_posMouse.y) || 
		s_iPopupInterface == ePIW_NONE && g_config.m_iMinimapWindowScale >= 100 && s_rectTitle.isIn(s_posMouse.x, s_posMouse.y) ||
		s_iPopupInterface == ePIW_NONE && g_config.m_iMinimapWindowScale < 100  && s_rectMapFrame.isIn(s_posMouse.x, s_posMouse.y) ||
		!(g_config.m_iChattingBoxScale & 0x8000) && rtCheckHeight.isIn(s_posMouse.x, s_posMouse.y) ||
		rtCheckWidth.isIn(s_posMouse.x, s_posMouse.y) ||
		s_iPopupInterface == ePIW_CARROT_SHOP && s_rectInterfaceSize.isIn(s_posMouse.x, s_posMouse.y))
	{		
		s_iFocusActor = 0xffff;
		return;
	}
	if (!s_rectInterfaceSize.isIn(s_posMouse.x, s_posMouse.y))
		for (int i=0;i<m_iActorCount;i++)		//	캐릭터
		{
			CActor	*lpActor	=	getTestedActor(m_aActorList[i],FALSE);

			if	(!lpActor)
				continue;
			if	(lpActor->isDummy() && lpActor->isOwnTeam())	//	아군 더미다. 스킵~
				continue;

			if	(!lpActor->isFocus(s_posCursor.x,s_posCursor.y))
				continue;

			if	(lpActor->isDeath())		//	이 녀석 시체다.
			{
				if	(lpActor->isMonster() && !lpActor->isPet() && !lpActor->isSummonBeast())
					continue;

				if	(lpActor->m_bf1IsCopyActor)
					continue;

				if	(g_bIsDuelServer)
					if	(s_iFocusActor	!=	0xffff)
						continue;
			}

			s_iFocusActor	=	m_aActorList[i];

			if	(s_iFocusActor	==	g_lpHero->m_wSerial)
			{
				if	(g_lpHero->m_wShakleSkill	!=	0xffff)
					break;

				continue;
			}

			if	(g_bIsDuelServer	&&	!lpActor->isOwnTeam())
				break;

			if	(lpActor->isPet() || lpActor->isSummonBeast() || lpActor->isPlayer())
				continue;

			if	(lpActor->m_wBody	>=	dJOB_MONSTER_Guild_Crest	&&	lpActor->m_wBody	<=	dJOB_MONSTER_GuildFlag)
				continue;

			break;
		}

	CActor	*lpActor	=	getTestedActor(s_iFocusActor,FALSE);

	if	(g_bIsDuelServer	==	FALSE)
	{
		if	(lpActor)	//	포커스 영역이 있는 상태에서 플레이어에게 포커스가 가면 영역에 우선권을 준다.
			if	(s_iFocusArea	!=	0xffff)
				if	(lpActor->isPlayer())
					s_iFocusActor	=	0xffff;
	}
	else
	{
		if	(lpActor)	//	포커스 영역이 있는 상태에서 플레이어에게 포커스가 가면 영역에 우선권을 준다.
			if	(s_iFocusArea	!=	0xffff)
				s_iFocusArea	=	0xffff;
	}

	if	(s_propose.m_dwValue != 0 && !s_propose.m_work.isReadyUseItemToItem)
	{
		CActor	*lpActor	=	g_am.getTestedActor(s_iFocusActor,FALSE);
		if (!s_propose.m_work.isReadyUseItemToCharacter	)
			if (lpActor && lpActor->isHero())
				s_iFocusActor	=	0xffff;

		if (!lpActor || !lpActor->isPlayer()			)
			s_iFocusActor	=	0xffff;
		
		if(s_propose.m_work.isReadyUseItemToEnemy)
		{
			if(lpActor  && !lpActor->isDeath())
			{
				if(g_lpHero->m_wTeam != lpActor->m_wTeam)
				{
					s_iFocusActor = lpActor->m_wSerial;
				}
				else
				{
					s_iFocusActor	=	0xffff;
				}
			}

		}
	}

	if	(s_iFocusActor	==	g_hero.m_iSerial)
		return;

	if	(s_iFocusActor	!=	0xffff)
		if (m_iLastAskActorInfoTime	+	dPERIOD_OF_ASK_ACTOR_INFO_WHEN_FOCUS	<=	s_iFrameCounter)
		{
			m_iLastAskActorInfoTime	=	s_iFrameCounter;

			s_agent.sendAskActorInfo(s_iFocusActor);
		}
}
//
//	현재 마우스 커서의 위치에 있는 시체-o-를 찾아서 포커스로 설정한다.
void
CActorManager::findFocusCorpse()
{
	if	(cINPDEV::IsExclusive(eMOUSE_STATE_PROPOSE_TO_WORK))
		return;

	if	(IsExclusiveFocus())
		return;

	if	(s_iFocusActor	!=	0xffff)
		return;

	if	(m_rectScreen.isIn(s_posMouse.x,s_posMouse.y))
		for (int i=0;i<m_iActorCount;i++)		//	캐릭터
		{
			CActor	*lpActor	=	getTestedActor(m_aActorList[i],FALSE);

			if	(!lpActor							)
				continue;
			if	(!lpActor->isDeath()					)
				continue;	//	이녀석이 몬스터이고 죽었다.
			if	(!lpActor->isPlayer() && !lpActor->isPet() && !lpActor->isSummonBeast())
				continue;
			if	(lpActor->m_bf1IsCopyActor)
				continue;

			cRECT	rect;

			cANM*	lpBody	=	lpActor->getBody();

			if	(!lpBody->getRect(lpActor->m_iAnm,lpActor->m_iDirect,lpActor->m_iFrame,&rect,lpActor->m_wVerticalScale))
			{
				cANMQ*	lpQBody	=	(cANMQ*	)lpBody;

				if	(!lpQBody->getRect(lpActor->m_iAnm,lpActor->m_iDirect,lpActor->m_iFrame,&rect,lpActor->m_wVerticalScale))
					continue;
			}

			rect.add(lpActor->m_pos.x,lpActor->m_pos.y);

			if	(rect.isIn(s_posCursor.x,s_posCursor.y))
			{
				s_iFocusActor		=	m_aActorList[i];

				if	(s_iFocusActor	==	g_lpHero->m_wSerial)
					continue;

				break;
			}
		}
}

//
//	캐릭터 초기화
void
CActorManager::resetActor(int _iSerial)
{
	g_sm.actorRemove(_iSerial);

	for (int i=0;i<m_iActorCount;i++)		//	아군 캐릭터
	{
		CActor	*lpActor	=	&g_aActor[m_aActorList[i]];

		if (lpActor->isDeath())	continue;
		if (lpActor->m_attackInfo.m_wTarget	==	_iSerial)
			lpActor->targetRemove();
	}

	if (g_hero.m_wEngageTarget	==	_iSerial)
		g_hero.m_wEngageTarget	=	0xffff;

	g_aActor[_iSerial].remove();
}

void
CActorManager::resetQuizEventStatus()
{
	for (int i=0;i<m_iActorCount;i++)		//	아군 캐릭터
	{
		CActor	*lpActor	=	&g_aActor[m_aActorList[i]];

		lpActor->m_bf1IsQuizEventWinner	=	FALSE;
		lpActor->m_bf1IsQuizEventLoser	=	FALSE;
	}
}

//
//	OX 퀴즈 패자부활
void
CActorManager::reviveOXQuizLoser()
{
	for (int i=0;i<m_iActorCount;i++)		//	아군 캐릭터
	{
		CActor	*lpActor			=	&g_aActor[m_aActorList[i]];

		if	(lpActor->m_bf1IsQuizEventWinner==	FALSE)
			lpActor->m_bf1IsQuizEventLoser	=	FALSE;
	}
}

//
//	OX 퀴즈 승자 리스트 받기
void
CActorManager::receiveOXQuizWinnerList(WORD *_lpwWinnerList,int _iCount)
{
	for (int i=0;i<_iCount;i++)		//	아군 캐릭터
	{
		CActor	*lpActor			=	&g_aActor[_lpwWinnerList[i]];

		lpActor->m_bf1IsQuizEventWinner=	TRUE;
		lpActor->m_bf1IsQuizEventLoser	=	TRUE;
	}

	if (!g_lpHero->isHighOperator() && g_lpHero->m_bf1IsQuizEventWinner == FALSE)	//	운영자도 아니고 승자도 아니다. 그럼 패자네.. -_-
	{
		g_msgBox.cPopup(dMSG_BEGIN_REVIVE_OX_QUIZ_LOSER_BATTLE,dMSG_FOR_OX_QUIZ_LOSER_WHEN_BEGIN_REVIVE_OX_QUIZ_LOSER_BATTLE,dMSG_OK);
	}

	if (!g_lpHero->isHighOperator() && g_lpHero->m_bf1IsQuizEventWinner == TRUE)	//	운영자는 아니고 승자다.
	{
		g_msgBox.cPopup(dMSG_BEGIN_REVIVE_OX_QUIZ_LOSER_BATTLE,dMSG_FOR_OX_QUIZ_WINNER_WHEN_BEGIN_REVIVE_OX_QUIZ_LOSER_BATTLE,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_OX_QUIZ_EVENT);
	}
}


//
//	플레이어 참가.
BOOL
CActorManager::joinActor(CSimplePlayerInfo *_lpSimpleActorInfo,BOOL _bIsDummy,BOOL _bIsCopyActor)
{
	int	iSerial		=	_lpSimpleActorInfo->bf11SerialInField;

	if	(g_lpHero	&&	iSerial	==	g_lpHero->m_wSerial)
		return	TRUE;

	CActor	*lpActor=	&g_aActor[iSerial];

	if (lpActor->m_wSerial	!=	0xffff)
		resetActor(iSerial);	//	강제제거 처리

	lpActor->buildBySimplePlayerInfo(_lpSimpleActorInfo);

	if	(_bIsCopyActor	||	_bIsDummy)
		lpActor->m_wWhiteFadeStep=	1;

	if	(_bIsDummy	&&	lpActor->m_wDummyStatus	==	0xffff)
		lpActor->m_wDummyStatus	=	dALIVE_DUMMY;
	else
		lpActor->m_wDummyStatus	=	0xffff;

	rebuildActorList();

	CPartyMemberInfo	*lpMember	=	s_partyInfo.getMemberByName(lpActor->m_strName);

	if (lpMember)
		lpMember->m_wSerial	=		iSerial;

	return	TRUE;
}

//
//	플레이어 참가.
BOOL
CActorManager::joinMonster(CSimpleMonsterInfo *_lpSimpleActorInfo,BOOL _bIsDummy,BOOL _bIsCopyActor)
{
	int	iSerial		=	_lpSimpleActorInfo->bf11SerialInField;

	if	(!g_lpHero)
		return	FALSE;

	if	(iSerial	==	g_lpHero->m_wSerial)
		return	TRUE;

#ifdef	_DEBUG
	if	(g_strMinimapCheckNpc[0] && STRICMP(_lpSimpleActorInfo->strName,g_strMinimapCheckNpc) == 0)	//테스트
		WarningMessage(_ms("REGEN [%s]!!!!!!",g_strMinimapCheckNpc));
#endif

	CActor	*lpActor=	&g_aActor[iSerial];

	if (lpActor->m_wSerial	!=	0xffff)
		resetActor(iSerial);	//	강제제거 처리

	lpActor->buildBySimpleMonsterInfo(_lpSimpleActorInfo);
	lpActor->setDirect(_lpSimpleActorInfo->bf3Direct);

	rebuildActorList();

	if (lpActor->isPet())
	{
		if (_lpSimpleActorInfo->bf1IsRegen)
		{
			lpActor->m_wWhiteFadeStep	=	1;
			lpActor->addHitEffect(0,0,g_im.m_wPetUnsealImage);
		}
	}
	else
	if (lpActor->isSummonBeast())
	{
		if (_lpSimpleActorInfo->bf1IsRegen)
		{
			lpActor->m_wWhiteFadeStep	=	1;
			lpActor->addHitEffect(0,0,g_im.m_wSummonningSummonBeast);
		}
	}
	else
	if (lpActor->isMonster() && _lpSimpleActorInfo->bf1IsRegen)
	{
		int	iBodySize	=	lpActor->getBodySize();

				if (iBodySize	<	30)	lpActor->addHitEffect(0,0,g_im.m_wImageMonsterEnterance,0,0);	//	몬스터 생성 효과
		else	if (iBodySize	<	50)	lpActor->addHitEffect(0,0,g_im.m_wImageMonsterEnterance,0,1);	//	몬스터 생성 효과
		else							lpActor->addHitEffect(0,0,g_im.m_wImageMonsterEnterance,0,2);	//	몬스터 생성 효과
	}

	if	(_bIsCopyActor	||	_bIsDummy)
		lpActor->m_wWhiteFadeStep=	1;

	if	(_bIsDummy	&&	lpActor->m_wDummyStatus	==	0xffff)
		lpActor->m_wDummyStatus	=	dALIVE_DUMMY;
	else
		lpActor->m_wDummyStatus	=	0xffff;

	return	TRUE;
}


//	운영자 리스트 받음
BOOL
CActorManager::receiveOperatorList(SG_OPERATOR_LIST *_lpPacket)
{
	int	i,iCount=_lpPacket->wCount;

	for (i=0;i<iCount;i++)
	{
		int	iSerial		=	_lpPacket->aOperatorList[i].wSerialInField;

		if (iSerial < 0 || iSerial >= dMAX_ACTOR)
			break;

		CActor	*lpActor=	&g_aActor[iSerial];

		lpActor->m_wOperatorLevel	=	_lpPacket->aOperatorList[i].wOperatorLevel;
		strcpy(lpActor->m_strName,_lpPacket->aOperatorList[i].strName);
	}

	return	TRUE;
}

//
//	기존에 액터들(좌표와 시리얼만 받는다. 싱크 상태 eSSWS_VALID_ACTOR로 설정)
BOOL
CActorManager::receiveMinimumActorInfoList(SG_MINIMUM_ACTOR_INFO_LIST *_lpPacketSgExistActor)
{
	int	i,iCount=_lpPacketSgExistActor->wCount;

	for (i=0;i<iCount;i++)
	{
		CMinimumActorInfo	*lpInfo	=	&_lpPacketSgExistActor->aInfoList[i];
		int	iSerial	=	lpInfo->bf11SerialInField;

		if (iSerial<0||iSerial>=dMAX_ACTOR)
			break;
		
		if (iSerial	==	g_hero.m_iSerial)
			continue;	//	내다.. -_-a

		CActor	*lpActor	=	&g_aActor[iSerial];

		if (lpActor->m_wSerial	!=	0xffff)
			resetActor(iSerial);

		lpActor->buildByMinimumActorInfo(lpInfo);

		if	(g_hero.m_wOperatorLevel	>=	4)
			if	(g_bIsObserverModeForDuel)
				g_am.addToAskInfoActorList(iSerial);


#ifdef	_DEBUG
		if	(g_strMinimapCheckNpc[0])//테스트
			g_am.addToAskInfoActorList(iSerial);
#endif
	}

	rebuildActorList();

	return	TRUE;
}

//
//	기존에 액터들
BOOL
CActorManager::receiveSimpleActorInfoList(SG_SIMPLE_ACTOR_INFO_LIST *_lpPacket)
{
	int	i,iBufferPoint=0;

	for (i=0;i<_lpPacket->wCount;i++)
	{
		CSimplePlayerInfo	*lpPlayerInfo	=	(CSimplePlayerInfo	*)&_lpPacket->aBuffer[iBufferPoint];

		if (lpPlayerInfo->bf2Kind	==	eAK_PLAYER)
		{
			int	iSerial	=	lpPlayerInfo->bf11SerialInField;

			if	(iSerial	>=	dMAX_ACTOR)
				break;
			if	(iSerial	==	g_hero.m_iSerial)
			{
				g_lpHero->m_bf1IsHide	=	lpPlayerInfo->bf1IsHide;

				continue;	//	내다.. -_-a
			}

			CActor	*lpActor	=	&g_aActor[iSerial];

			lpActor->buildBySimplePlayerInfo(lpPlayerInfo);

			int	iSize	=	sizeof(CSimplePlayerInfo)-sizeof(lpPlayerInfo->strName)+strlen(lpPlayerInfo->strName)+1;

			iBufferPoint+=	iSize;
		}
		else
		if (lpPlayerInfo->bf2Kind	==	eAK_MONSTER)
		{
			CSimpleMonsterInfo	*lpMonsterInfo	=	(CSimpleMonsterInfo	*)&_lpPacket->aBuffer[iBufferPoint];

			int	iSerial	=	lpMonsterInfo->bf11SerialInField;

			if (iSerial	>=	dMAX_ACTOR)
				break;

			CActor	*lpActor	=	&g_aActor[iSerial];

			lpActor->buildBySimpleMonsterInfo(lpMonsterInfo);

			int	iSize	=	sizeof(CSimpleMonsterInfo)-sizeof(lpMonsterInfo->strName)+strlen(lpMonsterInfo->strName)+1;

			iBufferPoint+=	iSize;
		}
		else
		{
			CSimpleNpcInfo	*lpMonsterInfo	=	(CSimpleNpcInfo	*)&_lpPacket->aBuffer[iBufferPoint];

			int	iSerial	=	lpMonsterInfo->bf11SerialInField;

			if (iSerial	>=	dMAX_ACTOR)
				break;

			CActor	*lpActor	=	&g_aActor[iSerial];

			lpActor->buildBySimpleNpcInfo(lpMonsterInfo);

			int	iSize	=	sizeof(CSimpleNpcInfo)-sizeof(lpMonsterInfo->strName)+strlen(lpMonsterInfo->strName)+1;

			iBufferPoint+=	iSize;
		}
	}

	rebuildActorList();

	m_wWaitAskActorInfoResultTime	=	0;	//	정보 받았다.

	return	TRUE;
}

BOOL
CActorManager::receiveVerySimpleActorInfoList(SG_VERY_SIMPLE_ACTOR_INFO_LIST *_lpPacket)
{
	int	i,iBufferPoint=0;

	for (i=0;i<_lpPacket->wCount;i++)
	{
		CVerySimplePlayerInfo	*lpPlayerInfo	=	(CVerySimplePlayerInfo	*)&_lpPacket->aBuffer[iBufferPoint];

		if (lpPlayerInfo->bf2Kind	==	eAK_PLAYER)
		{
			int	iSerial	=	lpPlayerInfo->bf11SerialInField;

			if (iSerial	>=	dMAX_ACTOR)
				break;

			if	(iSerial	==	g_hero.m_iSerial)
			{
				g_lpHero->m_bf1IsHide	=	lpPlayerInfo->bf1IsHide;

				continue;	//	내다.. -_-a
			}

			CActor	*lpActor	=	&g_aActor[iSerial];

//			if (lpActor->m_wSerial	!=	0xffff)
//				resetActor(iSerial);

			lpActor->buildByVerySimplePlayerInfo(lpPlayerInfo,FALSE);
			iBufferPoint	+=	sizeof(CVerySimplePlayerInfo);
			continue;
		}
		else
		if (lpPlayerInfo->bf2Kind	==	eAK_MONSTER)
		{
			CVerySimpleMonsterInfo	*lpMonsterInfo	=	(CVerySimpleMonsterInfo	*)&_lpPacket->aBuffer[iBufferPoint];

			int	iSerial	=	lpMonsterInfo->bf11SerialInField;

			if (iSerial	>=	dMAX_ACTOR)
				break;

			CActor	*lpActor	=	&g_aActor[iSerial];

//			if (lpActor->m_wSerial	!=	0xffff)
//				resetActor(iSerial);

			lpActor->buildByVerySimpleMonsterInfo(lpMonsterInfo,FALSE);

			iBufferPoint	+=	sizeof(CVerySimpleMonsterInfo);
		}
		else
		{
			CVerySimpleNpcInfo	*lpNpcInfo	=	(CVerySimpleNpcInfo	*)&_lpPacket->aBuffer[iBufferPoint];

			int	iSerial	=	lpNpcInfo->bf11SerialInField;

			if (iSerial	>=	dMAX_ACTOR)
				break;

			CActor	*lpActor	=	&g_aActor[iSerial];

//			if (lpActor->m_wSerial	!=	0xffff)
//				resetActor(iSerial);

			lpActor->buildByVerySimpleNpcInfo(lpNpcInfo,FALSE);

			iBufferPoint	+=	sizeof(CVerySimpleNpcInfo);
		}
	}

	rebuildActorList();

	m_wWaitObscurityActorInfoResultTime	=	0;

	return	TRUE;
}

BOOL
CActorManager::receiveSimpleActorInfo(SG_SIMPLE_ACTOR_INFO *_lpPacket)
{
	CSimplePlayerInfo	*lpPlayerInfo	=	(CSimplePlayerInfo	*)_lpPacket->aBuffer;

	if (lpPlayerInfo->bf2Kind	==	eAK_PLAYER)
	{
		int	iSerial	=	lpPlayerInfo->bf11SerialInField;

		if	(iSerial	>=	dMAX_ACTOR)
			return	FALSE;
		if	(lpPlayerInfo->bf1IsSyncHeroInfo	==	FALSE	&&	iSerial	==	g_hero.m_iSerial)
		{
			g_lpHero->m_bf1IsHide	=	lpPlayerInfo->bf1IsHide;

			return	FALSE;
		}

		CActor	*lpActor=	&g_aActor[iSerial];

		lpActor->buildBySimplePlayerInfo(lpPlayerInfo);
	}
	else
	if	(lpPlayerInfo->bf2Kind	==	eAK_MONSTER)
	{
		CSimpleMonsterInfo	*lpMonsterInfo	=	(CSimpleMonsterInfo	*)_lpPacket->aBuffer;

		int	iSerial	=	lpMonsterInfo->bf11SerialInField;

		if (iSerial	>=	dMAX_ACTOR)
			return	FALSE;

		CActor	*lpActor	=	&g_aActor[iSerial];

		lpActor->buildBySimpleMonsterInfo(lpMonsterInfo);
	}
	else
	{
		CSimpleNpcInfo	*lpNpcInfo	=	(CSimpleNpcInfo	*)_lpPacket->aBuffer;

		int	iSerial	=	lpNpcInfo->bf11SerialInField;

		if (iSerial	>=	dMAX_ACTOR)
			return	FALSE;

		CActor	*lpActor	=	&g_aActor[iSerial];

		int	iHP		=	lpActor->m_iHP;
		int	iMaxHP	=	lpActor->m_iMaxHP;

		lpActor->buildBySimpleNpcInfo(lpNpcInfo);

		lpActor->m_iHP		=	iHP;
		lpActor->m_iMaxHP	=	iMaxHP;
	}

	return	TRUE;
}


BOOL
CActorManager::receiveVerySimpleActorInfo(SG_VERY_SIMPLE_ACTOR_INFO *_lpPacket)
{
	CVerySimplePlayerInfo	*lpPlayerInfo	=	(CVerySimplePlayerInfo	*)_lpPacket->aBuffer;

	if	(lpPlayerInfo->bf2Kind	==	eAK_PLAYER)
	{
		int	iSerial	=	lpPlayerInfo->bf11SerialInField;

		if	(iSerial	>=	dMAX_ACTOR)
			return	FALSE;

		if	(iSerial	==	g_hero.m_iSerial )
		{
			if(!g_lpHero)
				return FALSE;
			
			g_lpHero->m_bf1IsHide		=	lpPlayerInfo->bf1IsHide;
			g_lpHero->m_wGuildPetType	=	lpPlayerInfo->bf3GuildPetType;
			g_lpHero->buildByVerySimplePlayerInfo(lpPlayerInfo,FALSE);

			if	(lpPlayerInfo->bf10Job	<	dPLAYER_JOB_COUNT)
				return	TRUE;
		}

		CActor	*lpActor	=	&g_aActor[iSerial];

		lpActor->buildByVerySimplePlayerInfo(lpPlayerInfo,FALSE);
	}
	else
	if	(lpPlayerInfo->bf2Kind	==	eAK_MONSTER)
	{
		CVerySimpleMonsterInfo	*lpMonsterInfo	=	(CVerySimpleMonsterInfo	*)_lpPacket->aBuffer;

		int	iSerial	=	lpMonsterInfo->bf11SerialInField;

		if	(iSerial	>=	dMAX_ACTOR)
			return	FALSE;

		CActor	*lpActor	=	&g_aActor[iSerial];

		lpActor->buildByVerySimpleMonsterInfo(lpMonsterInfo,FALSE);
	}
	else
	{
		CVerySimpleNpcInfo	*lpNpcInfo	=	(CVerySimpleNpcInfo	*)_lpPacket->aBuffer;

		int	iSerial	=	lpNpcInfo->bf11SerialInField;

		if	(iSerial	>=	dMAX_ACTOR)
			return	FALSE;

		CActor	*lpActor	=	&g_aActor[iSerial];

		lpActor->buildByVerySimpleNpcInfo(lpNpcInfo,FALSE);
	}

	return	TRUE;
}

//
//	액터들의 위치를 재설정 한다.
BOOL
CActorManager::operateReceivedRefitActorPostionPacket(SG_REFIT_ACTORS_POSITION *_lpPacket)
{
	for (int i=0;i<_lpPacket->wCount;i++)
	{
		CActorInfoForOuterActor	*lpInfo	=	&_lpPacket->aInfoList[i];
		CActor	*lpActor=	getTestedActor(lpInfo->wSerialInField);

		if (!lpActor || lpActor->isHero())	continue;

		if (lpActor->m_wSyncStatusWithServer==	eSSWS_WANT_SYNC	||	lpActor->m_wSyncStatusWithServer==	eSSWS_VALID_ACTOR)
		{
			lpActor->addToObscurityActorList();

			continue;
		}

		lpActor->changeSyncWithServerStatus(eSSWS_COMPLETE);

		lpActor->setPos(lpInfo->wX,lpInfo->wY);
		lpActor->m_dwEnchantedMask			=	lpInfo->dwEnchantedMask;
	}

	rebuildActorList();

	m_wWaitMisplacedActorResultTime		=	FALSE;

	return	TRUE;
}

//
//	액터들의 위치를 재설정 한다.
BOOL
CActorManager::operateReceivedOuterActorPostionPacket(SG_OUTER_ACTORS_POS_UPDATE *_lpPacket)
{
	for (int i=0;i<_lpPacket->wCount;i++)
	{
		CMinimumActorInfo	*lpInfo	=	&_lpPacket->aInfoList[i];
		CActor	*lpActor	=	g_am.getTestedActor(lpInfo->bf11SerialInField);

		if	(!lpActor	||	lpActor->isHero())	continue;

		if (lpActor->m_wSyncStatusWithServer==	eSSWS_WANT_SYNC	||	lpActor->m_wSyncStatusWithServer==	eSSWS_VALID_ACTOR)
		{
			lpActor->addToObscurityActorList();

			continue;
		}

		lpActor->setPos(lpInfo->wX,lpInfo->wY);
	}

	return	TRUE;
}

//
//	업데이트 실패(?) 처리
inline	BOOL
CActorManager::OperateFalse(int _iState)
{
	cINPDEV::ReleaseExclusive(TRUE,_iState);

	return	FALSE;
}

//
//	공격 업데이트
BOOL
CActorManager::updateActor()
{
	if	(g_lpHero->isDeath()	)
		return	OperateFalse(eMOUSE_STATE_ACTION);

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_ACTION))
		return	FALSE;

	if	(s_aKey[KEY_LSHIFT])
		return	FALSE;

	if	(g_lpHero->m_wJumpStatus)
		return	FALSE;

	if	(g_map.m_area.m_iFocusedArea	!=	0xffff)
	{
		cAreaInfo	*lpArea	=	g_map.m_area.getFocusedArea();

		if	(lpArea)
		{
			if	(lpArea->m_wKind	==	dAREA_DOOR)
			{
				cAreaDoorInfo	*lpAreaDoor	=	(cAreaDoorInfo	*)lpArea;

				if	(g_hero.isVisibleDoorTrap(lpAreaDoor))
					return	FALSE;

				if	(g_hero.isVisibleDoor(lpAreaDoor))
				{
					if	(lpAreaDoor->m_bf1IsUnlocked	==	FALSE)
						return	FALSE;
					else
					{
						if	(lpAreaDoor->m_bf1IsOpened && lpAreaDoor->m_wLinkSecretDungeon && lpAreaDoor->m_wLinkSecretDungeon !=0xffff)
							return	FALSE;
					}

					return	FALSE;
				}
			}

			if	(lpArea->m_wKind	==	dT_AREA_ARCA)
			{
				cAreaArcaInfo	*lpAreaArca	=	(cAreaArcaInfo	*)lpArea;
				cArcaInfo		*lpArca		=	g_arcaManager.getArca(lpAreaArca->m_wLinkArca);

				if	(lpArca && lpArca->m_wRemoveTimer	==	0)
				{
					if	(lpArca->m_bf1IsArmedTrap)
						return	TRUE;
					else
					if (lpArca->m_bf1IsLocked)
						return	TRUE;

					return	TRUE;
				}
			}
		}
	}

	if	(s_isLeftButtonPressed	+ s_isRightButtonPressed	==	0)	//	마우스 버튼을 뗐다.
	{
		g_hero.m_wActionToLockedTargetCount	=	0;

		if	(g_hero.isLockOn())
		{
			if	(g_hero.m_isWalking)
				g_hero.sendStop();

			g_hero.lockOff();		//	마우스 버튼이 안 눌러져 있으면, 락을 푼다.
		}

		return	OperateFalse(eMOUSE_STATE_ACTION);
	}

	if	(g_hero.isLockOn())	//	락이 걸려있고, 교전중이 아니다.
	{
		g_hero.actionToLockedTarget(g_hero.m_wActionToLockedTargetCount++);	//	락이 걸려 있는 넘을 공격!! 하거나... 공격 가능 거리로 이동.. -o-!!

		cINPDEV::SetExclusive(eMOUSE_STATE_ACTION);

		return TRUE;
	}

/*
//	if (g_lpHero->m_isDontAttack	)	return	FALSE;	//	공격 할 수 있는 상황이 아니다.
//	if (g_lpHero->m_iBerserkerTime)	return	FALSE;
	if (g_lpHero->isLockOn())	//	락이 걸려 있으면 락 걸린 넘 공격한다.
	{
		if (m_iLastActionTime	+	dSYNC_FPS/4	<	s_iFrameCounter)	//	1초에 4번 공격 업데이트
		{
			m_iLastActionTime	=	s_iFrameCounter;

			g_hero->attackLockedTarget(s_posCursor.x,s_posCursor.y);
		}

		return TRUE;
	}

	if (g_hero.m_iState != 0	&&	g_hero.m_iState != eMOUSE_STATE_ACTION)	return	FALSE;	//	락 걸린 상태도 아니고.. 딴걸하고 있다.
*/
	BOOL	bIsAttackShakle	=	FALSE;

	if	(s_iFocusActor	==	g_hero.m_iSerial	&&	g_lpHero->m_wShakleSkill	!=	0xffff)
		bIsAttackShakle	=	TRUE;

	if	(s_iFocusActor	!=	0xffff)
		if	(g_aActor[s_iFocusActor].m_bf1IsFreezeShakle)
			bIsAttackShakle	=	TRUE;

	if (s_isTouchLeftButton)
	{
		if	(s_iFocusActor	!=	0xffff)
		{
			if	(s_iFocusActor	!=	g_hero.m_iSerial	||	bIsAttackShakle)
			{
				if	(!g_hero.isOperator() && g_aActor[s_iFocusActor].m_wTeam	==	g_hero.m_wTeam	&&	bIsAttackShakle	==	FALSE)
					return	FALSE;

				s_bIsBookedDisableChattingBox	=	TRUE;

				g_hero.lockOn(s_iFocusActor,g_hero.m_wLeftAbility);

				cINPDEV::SetExclusive(eMOUSE_STATE_ACTION);

				return TRUE;
			}
		}

		return OperateFalse(eMOUSE_STATE_ACTION);
	}

	if (s_isTouchRightButton	&&	!s_rectInterfaceSize.isIn(s_posMouse.x, s_posMouse.y))
	{
		int	iFocusActor	=	s_iFocusActor;

		if	(iFocusActor	==	0xffff)
			iFocusActor	=	s_partyInfo.getFocusMember();

		if	(iFocusActor	!=	0xffff)
		{
			if	(g_aActor[iFocusActor].m_wTeam	==	g_hero.m_wTeam)
			{
				if	(g_hero.m_wSupportAbility	==	0xffff)
					return	OperateFalse(eMOUSE_STATE_ACTION);	//	다른거...

				g_hero.lockOn(iFocusActor,g_hero.m_wSupportAbility);

				cINPDEV::SetExclusive(eMOUSE_STATE_ACTION);

				return TRUE;
			}

			cAbility	*lpRightAbility	=	g_hero.getRightAbility();
			
			if	(lpRightAbility && !lpRightAbility->isQuickCastSkill())
			{
				if	(g_hero.m_wRightAbility	==	0xffff	||	s_iFocusActor	==	0xffff)
					return	OperateFalse(eMOUSE_STATE_ACTION);

				g_hero.lockOn(s_iFocusActor,g_hero.m_wRightAbility);

				cINPDEV::SetExclusive(eMOUSE_STATE_ACTION);

				return TRUE;
			}
		}

		g_hero.useSkillToGround(s_posCursor.x,s_posCursor.y,g_hero.m_wRightAbility);

		return OperateFalse(eMOUSE_STATE_ACTION);
	}

	return	OperateFalse(eMOUSE_STATE_ACTION);
}

//
//	아이템 줍기 업데이트
BOOL
CActorManager::updateFieldItem()
{
	if	(cINPDEV::IsExclusive(eMOUSE_STATE_PICK_FIELD_ITEM))
		return	FALSE;

	int	iOldFocusArea	=	s_iFocusArea;

	s_iFocusArea		=	0xffff;

	g_fieldItem.findFocusItem(s_posCursor.x,s_posCursor.y,m_rectScreen.isIn(s_posMouse.x,s_posMouse.y));	//	포커스 아이템을 찾는다.

	if	(s_iFocusFieldItem	==	0xffff)
		s_iFocusArea	=	iOldFocusArea;

	if	(s_isLeftButtonPressed	==	0)	//	마우스 버튼을 뗐다.
	{
		if (g_fieldItem.m_iPickItem	!=	0xffff)	//	줏으려는 아이템이 있었다.
		{
			if (g_hero.m_isWalking)
				g_hero.sendStop();	//	이동 중이었으면 정지

			g_fieldItem.releasePickItem();	//	줍기로 했던거 풀어주기
		}

		return	OperateFalse(eMOUSE_STATE_PICK_FIELD_ITEM);
	}

	if (g_hero.pickFieldItem())	//	줍자.
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_PICK_FIELD_ITEM);

		return	TRUE;
	}

	return	FALSE;//OperateFalse(eMOUSE_STATE_PICK_FIELD_ITEM);
}

BOOL
CActorManager::updateTrap()
{
	s_iFocusTrap				=	0xffff;
	g_trapManager.m_iFocusTrap	=	0xffff;

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_DISARM_TRAP) || s_bIsEventField || s_bIsIgnoreAllFocus)
		return	FALSE;

	if	(g_trapManager.m_iTryDisarmTrap	!=	0xffff	&&	g_trapManager.get(g_trapManager.m_iTryDisarmTrap)	&&	s_isLeftButtonPressed)
	{
		s_iFocusTrap				=	g_trapManager.m_iTryDisarmTrap;
		g_trapManager.m_iFocusTrap	=	g_trapManager.m_iTryDisarmTrap;
	}
	else
	{
		if	(m_rectScreen.isIn(s_posMouse.x,s_posMouse.y))
		{
			int	iFocusTrap	=	g_trapManager.getFocusTrap(s_posCursor.x,s_posCursor.y);

			if	(iFocusTrap	!=	0xffff	&&	g_trapManager.get(iFocusTrap)->m_bf3Team	==	g_hero.m_wTeam)
				iFocusTrap	=	0xffff;

			s_iFocusTrap				=	iFocusTrap;
			g_trapManager.m_iFocusTrap	=	s_iFocusTrap;
		}
	}

	if	(s_isLeftButtonPressed	==	0)	//	마우스 버튼을 뗐다.
	{
		if	(g_trapManager.m_iTryDisarmTrap	!=	0xffff)	//	줏으려는 아이템이 있었다.
		{
			if	(g_hero.m_isWalking)
				g_hero.sendStop();	//	이동 중이었으면 정지

			g_trapManager.m_iTryDisarmTrap	=	0xffff;
		}

		return	OperateFalse(eMOUSE_STATE_DISARM_TRAP);
	}

	if	(g_hero.tryDisarmTrap())	//	줍자.
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_DISARM_TRAP);

		return	TRUE;
	}

	return	TRUE;
}

//
//	영역 관련
BOOL
CActorManager::updateArea()
{
	g_map.m_area.update();

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_MOVE_TO_GATE))
		return	FALSE;

	s_iFocusArea			=	g_map.m_area.m_iMoveArea;

	BOOL	bIsIgnoreFocus	=	s_bIsIgnoreAllFocus;

	if (s_iFocusActor != 0xffff)
	{
		CActor	*lpActor	=	g_am.getActor(s_iFocusActor);

		if (lpActor)
			if (lpActor->isOwnTeam())	
				bIsIgnoreFocus	=	TRUE;
	}

	if	(IsValidFocus() || bIsIgnoreFocus && g_hero.m_wWaitMoveAreaResultTime	==	0)
	{
		BOOL	bIsIgnoreSpecialArca	=	FALSE;

		for	(int i=2;i;)
		{
			--i;
			s_iFocusArea	=	g_map.m_area.getFocusArea(s_posCursor.x,s_posCursor.y,m_rectScreen.isIn(s_posMouse.x,s_posMouse.y),bIsIgnoreSpecialArca);	//	

			if	(s_iFocusArea	==	0xffff)
				break;

			cAreaInfo	*lpFocusArea	=	g_map.m_area.getFocusedArea();

			if	(!lpFocusArea)
				break;

			if	(lpFocusArea->m_wKind	==	dAREA_TRAP)
			{
				cAreaTrapInfo	*lpAreaTrap	=	(cAreaTrapInfo	*)lpFocusArea;

				if	(g_hero.isVisibleTrap(lpAreaTrap)	==	FALSE)
				{
					s_iFocusArea			=	0xffff;
					g_map.m_area.releaseMoveArea();

					bIsIgnoreSpecialArca	=	TRUE;
				}

				continue;
			}

			if	(lpFocusArea->m_wKind	==	dAREA_EVENT_AREA)
			{
				cAreaEvent	*lpEventArea	=	(cAreaEvent	*)lpFocusArea;

				if	(g_map.isEventArea(lpEventArea,s_posMouse.x,s_posMouse.y)	==	FALSE)
				{
					s_iFocusArea	=	0xffff;
					g_map.m_area.releaseMoveArea();
				}

				break;
			}

			if	(lpFocusArea->m_wKind	==	dAREA_DOOR)
			{
				cAreaDoorInfo	*lpAreaDoor	=	(cAreaDoorInfo	*)lpFocusArea;
				BOOL			bIsEnable	=	TRUE;

				if	(g_map.isFocusDoor(lpAreaDoor,s_posMouse.x,s_posMouse.y)	==	FALSE	||	lpAreaDoor->m_bf1IsActiveByKarma)
					bIsEnable	=	FALSE;

				if	(lpAreaDoor->m_bf1IsOpened	&&	lpAreaDoor->m_bf1IsOpenedByHero	==	FALSE)
					if	(lpAreaDoor->m_wLinkSecretDungeon	==	0xffff	||	lpAreaDoor->m_wLinkSecretDungeon	==	0)
						bIsEnable	=	FALSE;

				if	(bIsEnable	==	FALSE)
				{
					s_iFocusArea	=	0xffff;
					g_map.m_area.releaseMoveArea();
				}

				break;
			}

			if	(lpFocusArea->m_wKind	==	dT_AREA_ARCA)
			{
				cAreaArcaInfo	*lpAreaArca	=	(cAreaArcaInfo	*)lpFocusArea;

				if	(g_arcaManager.isVisible(lpAreaArca->m_wLinkArca)	==	FALSE)
				{
					s_iFocusArea			=	0xffff;
					g_map.m_area.releaseMoveArea();
					bIsIgnoreSpecialArca	=	TRUE;
				}
				continue;
			}
		}
	}

	g_map.operateDoorWork();
	g_map.operateEventObjectWork();

	if	(s_iFocusArea	==	0xffff)
		return	OperateFalse(eMOUSE_STATE_MOVE_TO_GATE);

	if	(s_isTouchLeftButton && g_map.m_area.m_iMoveArea == 0xffff)
	{
		g_map.m_area.m_iMoveArea	=	s_iFocusArea;
		
		cINPDEV::SetExclusive(eMOUSE_STATE_MOVE_TO_GATE);
	}

	if	(s_isLeftButtonPressed	==	0)	//	마우스 버튼을 뗐다.
	{
		if (g_map.m_area.m_iMoveArea	!=	0xffff)
		{
			if (g_hero.m_isWalking)
				g_hero.sendStop();

			g_map.m_area.releaseMoveArea();

			g_map.m_area.getFocusArea(s_posCursor.x,s_posCursor.y,m_rectScreen.isIn(s_posMouse.x,s_posMouse.y));
		}

		return	OperateFalse(eMOUSE_STATE_MOVE_TO_GATE);
	}

	if	(g_hero.moveToArea())	
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_MOVE_TO_GATE);

		return	TRUE;
	}

	return	FALSE;
}

//
//	이동 업데이트
inline	BOOL
CActorManager::updateMove()
{
	if (g_map.m_area.m_iMoveArea	!=	0xffff )
		return	TRUE;

	BOOL	bIsIgnoreKey	=	IsOpenedEventWindow();

	g_hero.checkStop();

#ifdef	_DEBUG
	if (m_iTraceTarget	!=	0xffff)
		if (s_iFrameCounter % 8 == 0)
		{
			CActor	*lpActor	=	getTestedActor(m_iTraceTarget);

			if (!lpActor)
				m_iTraceTarget	=	0xffff;
			else
			{
				g_lpHero->setPos(lpActor->m_pos.x,lpActor->m_pos.y);
				s_agent.sendStop();
			}
		}
#endif

	if (s_iFocusActor				!=	0xffff	)	
		bIsIgnoreKey	=	TRUE;
	if (g_pitchmanShop.m_wFocusShop	!=	0xffff	)
		bIsIgnoreKey	=	TRUE;
	if (g_portal.m_iFocusPortal		!=	0xffff	)
		bIsIgnoreKey	=	TRUE;

	if	(s_rectInterfaceSize.isIn(s_posMouse.x, s_posMouse.y))
		bIsIgnoreKey	=	TRUE;
	else
		bIsIgnoreKey	+=	g_gwParty.isIn();

	if (g_hero.walking(bIsIgnoreKey))	
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_MOVE);
		return	TRUE;
	}

	return	OperateFalse(eMOUSE_STATE_MOVE);
}

//
//	입력 처리
BOOL
CActorManager::updateInpdev()
{
	if	(s_isOperatedMsgbox)
		return	FALSE;

//	키보드 관련 업데이트
	if	(!s_inputChatting.isEnable())
	{

#ifdef _DEBUG			//몬스터 도감 열기..
		if(s_aClicked[KEY_MULTIPLY])
			g_gwBookMonDic.open();
		
		//JBC 08-07-15
		if(s_aClicked[KEY_O])
		{
			g_bObserverMode = !g_bObserverMode;
			g_am.ChangeObserverMode();
		}
#endif

	}	//	키보드 관련 업데이트

	if	(!g_lpHero)
		return	FALSE;

//	마우스 관련 업데이트
	{
		updateActor();		//	액션 관련 업데이트
		updateFieldItem();	//	아이템 줍기 관련 업데이트
		updateArea();		//	게이트로 이동하는것 관련
		updateTrap();
	}	//	마우스 관련 업데이트

	if (s_bIsIgnoreAllFocus)
	{
		g_fieldItem.m_iFocusItem	=	0xffff;
		s_iFocusActor				=	0xffff;
		s_iFocusFieldItem			=	0xffff;
		s_iFocusArea				=	0xffff;
	}

	return	TRUE;
}

//
//	비어있는 액터 데이터 리스트 추가
BOOL
CActorManager::addToAskInfoActorList(int _iActorSerial)
{
	if (m_wWaitAskActorInfoResultTime							)	return FALSE;
	if (m_wAskInfoActorCount>=	dMAX_REQUEST_ACTOR_DATA_COUNT	)	return FALSE;
	if (_iActorSerial	<	0	||	_iActorSerial		>=	dMAX_ACTOR			)
		return FALSE;

	for (int i=0;i<m_wAskInfoActorCount;i++)
		if (m_aAskInfoActorList[i]	==	_iActorSerial)
			return FALSE;	//	이미 등록 되어 있다.

	m_aAskInfoActorList[m_wAskInfoActorCount++]	=	_iActorSerial;

	return	TRUE;
}

//	매우 간단한 캐릭터 정보를 얻기위한 리스트에 추가
BOOL
CActorManager::addObscurityActor(int _iActorSerial)
{
	if (m_wWaitObscurityActorInfoResultTime							)
		return FALSE;

	if (m_wObscurityActorCount	>=	dMAX_REQUEST_ACTOR_DATA_COUNT	)
		return FALSE;

	if (_iActorSerial	<	0	||	_iActorSerial	>=	dMAX_ACTOR	)
		return FALSE;

	for (int i=0;i<m_wObscurityActorCount;i++)
		if (m_aObscurityActorList[i]	==	_iActorSerial)
			return FALSE;	//	이미 등록 되어 있다.

	m_aObscurityActorList[m_wObscurityActorCount]	=	_iActorSerial;

	m_wObscurityActorCount++;

	return	TRUE;
}

//
//	비어있는 액터 데이터 리스트 추가
BOOL
CActorManager::addMisplacedActor(int _iActorSerial)
{
	if (m_wWaitMisplacedActorResultTime							)	return FALSE;	//	결과를 기다리는 중이다.
	if (m_wMisplacedActorCount	>=	dMAX_MISPLACED_ACTOR_COUNT	)	return FALSE;	//	너무 많이 추가되었다.
	if (_iActorSerial			>=	dMAX_ACTOR					)	return FALSE;	//	시리얼이 이상해

	for (int i=0;i<m_wMisplacedActorCount;i++)
		if (m_aMisplacedActorList[i]==_iActorSerial)
			return FALSE;	//	이미 등록 되어 있다.

	m_aMisplacedActorList[m_wMisplacedActorCount++]	=	_iActorSerial;

	return	TRUE;
}

//
//	비어있는 액터 데이터 리스트 추가
BOOL
CActorManager::addAskActorEnchantEffectInfo(int _iActorSerial,int _iWork)
{
	if	(m_wAskActorEnchantEffectInfoResultTime								)
		return FALSE;	//	결과를 기다리는 중이다.
	if	(m_wAskActorEnchantEffectInfoCount	>=	dMAX_MISPLACED_ACTOR_COUNT	)
		return FALSE;	//	너무 많이 추가되었다.
	if	(_iActorSerial						>=	dMAX_ACTOR					)
		return FALSE;	//	시리얼이 이상해

	for (int i=0;i<m_wAskActorEnchantEffectInfoCount;i++)
		if (m_awAskActorEnchantEffectInfoList[i][0]	==	_iActorSerial)
			return FALSE;	//	이미 등록 되어 있다.

	m_awAskActorEnchantEffectInfoList[m_wAskActorEnchantEffectInfoCount][0]	=	_iActorSerial;
	m_awAskActorEnchantEffectInfoList[m_wAskActorEnchantEffectInfoCount][1]	=	_iWork;

	m_wAskActorEnchantEffectInfoCount++;	

	return	TRUE;
}

//
//	서버에 잘못된 액터 데이터의 교정을 요구한다.
void
CActorManager::sendAskInfoActorList()
{
	if (m_wWaitAskActorInfoResultTime	)	//	정보를 기다리는 중이다.
	{
		m_wWaitAskActorInfoResultTime	--;
		return;
	}

	if (m_wAskInfoActorCount	<=	0		)	return;	//	요청 정보가 없다.

	m_wWaitAskActorInfoResultTime	=	dSYNC_FPS;	//	1초간 기다리고 안오면 다시 요청한다.

	s_agent.sendAskInfoActorList(m_aAskInfoActorList,m_wAskInfoActorCount);

	m_wAskInfoActorCount		=	0;
}

//
//	서버에 잘못된 액터 데이터의 교정을 요구한다.
void
CActorManager::sendObscurityActorList()
{
	if (m_wWaitObscurityActorInfoResultTime	)	//	정보를 기다리는 중이다.
	{
		m_wWaitObscurityActorInfoResultTime	--;
		return;
	}

	if (m_wObscurityActorCount	<=	0		)	return;	//	요청 정보가 없다.

	m_wWaitObscurityActorInfoResultTime	=	dSYNC_FPS;	//	1초간 기다리고 안오면 다시 요청한다.

	s_agent.sendObscurityActorList(m_aObscurityActorList,m_wObscurityActorCount);

	m_wObscurityActorCount		=	0;
}

//
//	서버에 잘못된 액터 데이터의 교정을 요구한다.
void
CActorManager::sendMisplacedActorList()
{
	if (m_wWaitMisplacedActorResultTime							)
	{
		m_wWaitMisplacedActorResultTime--;
		return;
	}

	if (m_wMisplacedActorCount	<=	0							)	return;

	WORD	aMisplacedActorList[dMAX_MISPLACED_ACTOR_COUNT],wMisplacedActorCount=0;

	for (int i=0;i<m_wMisplacedActorCount;i++)
	{
		CActor	*lpActor=	getTestedActor(m_aMisplacedActorList[i]);

		if (!lpActor ||	lpActor->m_wSyncStatusWithServer != eSSWS_WAIT_MINIMUM_ACTOR_INFO)
			continue;

		aMisplacedActorList[wMisplacedActorCount++]	=	m_aMisplacedActorList[i];
	}

	m_wMisplacedActorCount	=	0;

	if (wMisplacedActorCount<=	0)	return;

	m_wWaitMisplacedActorResultTime	=	dSYNC_FPS;	//	정보 기다린다.

	s_agent.sendMisplacedActorList(aMisplacedActorList,wMisplacedActorCount);
}

//
//	서버에 잘못된 액터 데이터의 교정을 요구한다.
void
CActorManager::sendAskActorEnchantInfoList()
{
	if	(m_wAskActorEnchantEffectInfoResultTime		)
	{
		m_wAskActorEnchantEffectInfoResultTime--;
		return;
	}

	if	(m_wAskActorEnchantEffectInfoCount	<=	0	)
		return;

	WORD	awActorList[dMAX_MISPLACED_ACTOR_COUNT*2],wActorCount=0;

	for (int i=0;i<m_wAskActorEnchantEffectInfoCount;i++)
	{
		CActor	*lpActor=	getTestedActor(m_awAskActorEnchantEffectInfoList[i][0]);

		if	(!lpActor)
			continue;

		awActorList[wActorCount*2	]	=	m_awAskActorEnchantEffectInfoList[i][0];
		awActorList[wActorCount*2+1]	=	m_awAskActorEnchantEffectInfoList[i][1];
		wActorCount++;
	}

	m_wAskActorEnchantEffectInfoCount	=	0;

	if	(wActorCount<=	0)
		return;

	m_wAskActorEnchantEffectInfoResultTime	=	dSYNC_FPS;	//	정보 기다린다.

	s_agent.sendAskActorEnchantInfoList(awActorList,wActorCount);
}


//
//	다음 녀석!!
char*
CActorManager::getNextPlayerName()
{
	int	i;

	for (i=m_wCurrentIndex;i<dMAX_ACTOR;i++)		//	아군 캐릭터
	{
		CActor	*lpActor	=	getActor(i);

		if (lpActor->m_wSerial	==	0xffff				)	continue;
		if (lpActor->m_wSerial	==	g_hero.m_iSerial	)	continue;
		if (strlen(lpActor->m_strName) < 4				)	continue;
		if (!lpActor->isPlayer()						)	continue;

		i++;

		if (i	>=	dMAX_ACTOR)	i	=	0;
		m_wCurrentIndex	=	i;

		return	lpActor->m_strName;
	}

	for (i=0;i<m_wCurrentIndex;i++)		//	아군 캐릭터
	{
		CActor	*lpActor	=	getActor(i);

		if (lpActor->m_wSerial	==	0xffff				)	continue;
		if (lpActor->m_wSerial	==	g_hero.m_iSerial	)	continue;
		if (strlen(lpActor->m_strName) < 4				)	continue;
		if (!lpActor->isPlayer()						)	continue;

		i++;

		if (i	>=	dMAX_ACTOR)
			i	=	0;

		m_wCurrentIndex	=	i;

		return	lpActor->m_strName;
	}

	return	NULL;
}

void
CActorManager::addDenyCommunityActor(char *_lpstrName)
{
	if (m_iDenyCommunityActorCount	>=	256)
		return;

	for (int i=0;i<m_iDenyCommunityActorCount;i++)
	{
		if (STRICMP(m_astrDenyCommunityActor[i],_lpstrName)==0)
		{
			memcpy(m_astrDenyCommunityActor+i,m_astrDenyCommunityActor+i+1,dNAME_LENGTH*(m_iDenyCommunityActorCount-i-1));

			m_iDenyCommunityActorCount--;

			return;
		}
	}

	strcpy(m_astrDenyCommunityActor[m_iDenyCommunityActorCount],_lpstrName);

	m_iDenyCommunityActorCount++;
}

BOOL
CActorManager::isDenyCommunityActor(char *_lpstrName)
{
	for (int i=0;i<m_iDenyCommunityActorCount;i++)
	{
		if (STRICMP(m_astrDenyCommunityActor[i],_lpstrName)==0)
			return TRUE;
	}

	return	FALSE;
}

void
CActorManager::resetDenyCommunityActorList()
{
	m_iDenyCommunityActorCount	=	0;
}

//
//	다음 녀석!!
char*
CActorManager::getPreviousPlayerName()
{
	int	i;

	for (i=m_wCurrentIndex;i>=0;i--)		//	아군 캐릭터
	{
		CActor	*lpActor	=	getActor(i);

		if (lpActor->m_wSerial	==	0xffff				)	continue;
		if (lpActor->m_wSerial	==	g_hero.m_iSerial	)	continue;
		if (strlen(lpActor->m_strName) < 4				)	continue;
		if (!lpActor->isPlayer()						)	continue;

		if (i	==	0)	m_wCurrentIndex	=	dMAX_ACTOR-1;
		else			m_wCurrentIndex	=	i-1;

		return	lpActor->m_strName;
	}

	for (i=dMAX_ACTOR-1;i>=m_wCurrentIndex;i--)		//	아군 캐릭터
	{
		CActor	*lpActor	=	getActor(i);

		if (lpActor->m_wSerial	==	0xffff				)	continue;
		if (lpActor->m_wSerial	==	g_hero.m_iSerial	)	continue;
		if (strlen(lpActor->m_strName) < 4				)	continue;
		if (!lpActor->isPlayer()						)	continue;

		if (i	==	0)	m_wCurrentIndex	=	dMAX_ACTOR-1;
		else			m_wCurrentIndex	=	i-1;

		return	lpActor->m_strName;
	}

	return	NULL;
}

//
//	죽여!!
void
CActorManager::killActor(int _iSerial,BOOL _bIsInstanceKill,int _iDeathPeneltyTime,BOOL _bIsReleaseSummon)
{
	CActor	*lpDeathActor	=	getTestedActor(_iSerial);

	int		i;

	if (!lpDeathActor			)
		return;

	if (lpDeathActor->m_wBiter != 0xffff)
	{
		CActor	*lpTarget	=	getTestedActor(lpDeathActor->m_wBiter);

		if (lpTarget)
		{
			lpTarget->stop();
			lpTarget->m_wBiteTarget	=	0xffff;
		}

		lpDeathActor->m_wBiter		=	0xffff;
	}

	if (lpDeathActor->m_wBiteTarget != 0xffff)
	{
		CActor	*lpTarget	=	getTestedActor(lpDeathActor->m_wBiteTarget);

		if (lpTarget)
		{
			lpTarget->stop();
			lpTarget->m_wBiter		=	0xffff;
		}

		lpDeathActor->m_wBiteTarget	=	0xffff;
	}

	if (lpDeathActor->isHero()	)
	{
		g_hero.m_wDeathPeneltySecond	=	_iDeathPeneltyTime;
		s_pmCommon.reset();
		g_hero.syncHeroApprearance();
	}

	if (lpDeathActor->isDeath())
		return;

	for (i=0;i<m_iActorCount;i++)		//	아군 캐릭터
	{
		CActor	*lpActor	=	getTestedActor(m_aActorList[i],FALSE);

		if (!lpActor		)
			continue;
		if (lpActor->isDeath())
			continue;

		if (lpActor->m_attackInfo.m_wTarget	==	_iSerial)
			lpActor->targetDeath();
	}

	if (g_hero.m_wEngageTarget	==	_iSerial)
		g_hero.m_wEngageTarget	=	0xffff;

	lpDeathActor->kill(FALSE,_bIsReleaseSummon);	//	죽어!! -o-

	if (_bIsInstanceKill)
		lpDeathActor->addHitEffect(0,-lpDeathActor->getBody()->m_sprite.m_iHeight,g_im.m_wInstanceKillEffect);

	if (_bIsReleaseSummon)
		lpDeathActor->addHitEffect(0,0,g_im.m_wSoulOutEffect);

	g_sm.actorDeath(_iSerial);
}

//
//캐릭터 제거
void
CActorManager::removeActor(int _iSerial,BOOL _bIsHide)
{
	CPartyMemberInfo	*lpPartyMember	=	s_partyInfo.getMemberByActorSerial(_iSerial);

	if	(lpPartyMember	)
		lpPartyMember->m_wSerial	=	0xffff;

	if	(g_hero.isSummonBeastActor(_iSerial))
	{
		int	iIndex	=	g_hero.getSummonBeastIndex(_iSerial);

		if	(iIndex	!=	0xffff)
			g_hero.m_aSummonBeast[iIndex].m_bf10Level	=	0;
	}

	if	(_bIsHide	&&	g_lpHero->m_wSerial	==	_iSerial)
		return;

	resetActor(_iSerial);
	rebuildActorList();
}

//
//	_iWho에게 _lpItem 사용
void
CActorManager::attatchItemUseEffect(int _iWho,cItem *_lpItem)
{
	CActor		*lpActor	=	getTestedActor(_iWho);
	cBasicItem	*lpBasicItem=	_lpItem->getBasicItem();

	if	(!lpBasicItem || !lpActor)
		return;

	if	(lpBasicItem->m_bf1IsOccurEatPotionEffect	||	lpBasicItem->m_wKind	==	eIK_HEAL_POTION		||	lpBasicItem->m_wKind	==	eIK_CHARGING_POTION	||	lpBasicItem->m_wKind	==	eIK_FOOD_AND_ETC)
		lpActor->addEffect(eHEOP_HIT_ZONE,g_im.m_wEatPotionImage);
}

//
//	외관이 변했다.
void
CActorManager::changeAppearance(int _iSerial,SG_CHANGE_APPEARANCE *_lpPacket)
{
	CActor	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)	return;

	if (_lpPacket->bf4Weapon	!=	dVAILD_EQUIPMENT_VALUE_FOR_SAI)	
			lpActor->m_itemWeapon.m_wBaseItem	=	cBasicItem::GetItemByEquippedShape(_lpPacket->bf4Weapon,lpActor->m_wJob,dEQUIP_WEAPON);
	else	lpActor->m_itemWeapon.m_wBaseItem	=	0xffff;

	if (_lpPacket->bf4Shield	!=	dVAILD_EQUIPMENT_VALUE_FOR_SAI)	
			lpActor->m_itemShield.m_wBaseItem	=	cBasicItem::GetItemByEquippedShape(_lpPacket->bf4Shield,lpActor->m_wJob,dEQUIP_SHIELD_BULLET);
	else	lpActor->m_itemShield.m_wBaseItem	=	0xffff;

	if (_lpPacket->bf4Armor		!=	dVAILD_EQUIPMENT_VALUE_FOR_SAI)	
			lpActor->m_itemArmor.m_wBaseItem	=	cBasicItem::GetItemByEquippedShape(_lpPacket->bf4Armor,lpActor->m_wJob,dEQUIP_ARMOR);
	else	lpActor->m_itemArmor.m_wBaseItem	=	0xffff;

	if (_lpPacket->bf8WeaponEffect	!=	0xff)
			lpActor->m_itemWeapon.m_wColorizeEffect	=	_lpPacket->bf8WeaponEffect;
	else	lpActor->m_itemWeapon.m_wColorizeEffect	=	0xffff;

	lpActor->m_itemArmor.m_wColorizeEffect		=	_lpPacket->bf3ArmorPalette;

	lpActor->getBodyIndex();

	lpActor->checkEquipmentShape();
}


//
//	특정 영역 안에 죽은 캐릭터를 그린다. -_-a
void
CActorManager::drawDeathActors(cRECT *_lpRectViewport)
{
	int		iCount	=	0,i;

	CActor	*lpActor;

	for (i=0;i<m_iActorCount;i++)		//	아군 캐릭터
	{
		lpActor	=	getTestedActor(m_aActorList[i],FALSE);

		if (!lpActor			)
			continue;
		if (!lpActor->isCorpse())
			continue;

		if (_lpRectViewport->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
			lpActor->put();
	}
}

//
//	죽었으면 잘못된 액터 리스트에 추가
BOOL
CActorManager::checkDeathStatus(CActor *_lpActor)
{
	if	(!_lpActor)	
		return	FALSE;

	if	(_lpActor->isDeath())
	{
		addObscurityActor(_lpActor->m_wSerial);
		return	FALSE;
	}

	return	TRUE;
}

CActor*	
CActorManager::getActorByName(char *_lpstrName)
{
	for (int i=0;i<m_iActorCount;i++)
	{
		CActor	*lpActor	=	&g_aActor[m_aActorList[i]];

		if (STRICMP(lpActor->m_strName,_lpstrName) == 0)
			return	lpActor;
	}

	return	NULL;
}