#include "CGamePlay.H"
#include "CActor.H"
#include "CMessage.H"
#include "CWindowInterface.H"

cEasySkillTree	g_gwEasySkillTree;

#define			dEASY_SKILL_ITEM_HEIGHT		40
#define			dEASY_SKILL_COUNT_IN_WINDOW	10
#define			dEASY_SKILL_WINDOW_HEIGHT	404

cEasySkillTree::cEasySkillTree()
{
}

//
//	스킬 인터페이스 닫기
void
cEasySkillTree::close()
{
	m_iRequireAbilityCount		=	0;
	m_iFocusSkill				=	0xffff;

	m_bmPlus.reset();
}

//
//	스킬 윈도우 초기 데이터 설정
void
cEasySkillTree::init()
{
	m_bmPlus.init(&g_sprInterface,100);
}

//
//	스킬 윈도우의 버튼들을 초기화
void
cEasySkillTree::resetButtons()
{
	int	i;

	m_bmPlus.reset();

	for (i=0;i<m_iRequireAbilityCount;i++)
		m_bmPlus.addImageBar("",m_aRequireAbility[i].m_wSkill,0,0,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
}

//  
//	스킬 윈도우의 버튼들 위치 설정
void
cEasySkillTree::resetButtonsPos()
{
	int	iX	=	m_pos.x+8;
	int	iY	=	m_pos.y+56;

	int	i,iCount=0;

	for (i=0;i < m_iRequireAbilityCount;i++)
		m_bmPlus.hide(m_aRequireAbility[i].m_wSkill);

	for (i=0;i < m_iRequireAbilityCount && iCount < dEASY_SKILL_COUNT_IN_WINDOW;i++)
	{
		int			iAbility		=	m_aRequireAbility[i].m_wSkill;
		int			iRequireLevel	=	m_aRequireAbility[i].m_wLevel;
		cAbility	*lpAbility		=	&g_hero.m_aAbility[iAbility];
		CSkill		*lpSkill		=	lpAbility->getSkill();

		if (!lpSkill)
			break;

		if (i < s_sbMiddleWindow.m_iCurrentPos)
			continue;

		int		iPutX	=	iX+44;
		int		iPutY	=	iY+iCount*dEASY_SKILL_ITEM_HEIGHT+20;
		char	strText[128];

		if	(lpAbility->m_wLevel	>=	m_aRequireAbility[i].m_wLevel)
			sprintf(strText,"Lv <c:LTGREEN>%d<n>",lpAbility->m_wLevel);
		else
			sprintf(strText,"Lv <c:LTRED>%d<n>[<c:LTYELLOW>%d<n>]",lpAbility->m_wLevel,m_aRequireAbility[i].m_wLevel);

		int		iDX		=	s_sText.getPixelSize(strText);

		iPutX			+=	iDX;

		m_bmPlus.resetImageBar(iAbility,"",iAbility,iPutX ,iPutY,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED);

		if (g_hero.isLevelUpAbleAbility(iAbility) && lpAbility->m_wLevel < iRequireLevel)
		{
			if (!m_bmPlus.isShow(iAbility))
				m_bmPlus.show(iAbility);
		}
		else
			m_bmPlus.hide(iAbility);

		iCount++;
	}
}

void
cEasySkillTree::addRequireAbility(int _iAbility,int _iLevel)
{
	for (int i=0;i<m_iRequireAbilityCount;i++)
		if (m_aRequireAbility[i].m_wSkill	==	_iAbility)
		{
			m_aRequireAbility[i].m_wLevel	=	max(m_aRequireAbility[i].m_wLevel,_iLevel);

			return;
		}

	m_aRequireAbility[m_iRequireAbilityCount].m_wSkill	=	_iAbility;
	m_aRequireAbility[m_iRequireAbilityCount].m_wLevel	=	_iLevel;

	m_iRequireAbilityCount++;
}

int
cEasySkillTree::getRequireAbilityLevel(int _iAbility)
{
	for (int i=0;i<m_iRequireAbilityCount;i++)
		if (m_aRequireAbility[i].m_wSkill	==	_iAbility)
			return	m_aRequireAbility[i].m_wLevel;

	return	-1;
}

void
cEasySkillTree::getRequireAbility(int _iAbility)
{
	cAbility	*lpAbility	=	&g_hero.m_aAbility[_iAbility];
	CSkill		*lpSkill	=	lpAbility->getSkill();
	int			iCount		=	0;

	for (iCount=0;iCount<dMAX_REQUIRE_SKILL_COUNT;iCount++)
	{
		cREQUIRE_SKILL	*lpRequireSkill	=	&lpSkill->m_aRequireSkill[iCount];

		if (lpRequireSkill->m_wSkill	==	0xffff)
			break;

		int	iRequireAbility				=	g_hero.getAbilityIndexBySkill(lpRequireSkill->m_wSkill);

		addRequireAbility(iRequireAbility,lpRequireSkill->m_wLevel);

		getRequireAbility(iRequireAbility);
	}
}

//
//
void
cEasySkillTree::open(int _iObjectAbility)
{
	if (s_iPopupInterface	!=	ePIW_SKILL)
		return;

	if (_iObjectAbility	==	-1)
		_iObjectAbility		=	m_iBookedAbility;
	
	m_iObjectAbility		=	_iObjectAbility;

	int	iSkillWindowWidth	=	g_sprInterface.getSpriteWidth(eSKILL_WINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eSKILL_WINDOW);
	int	iTabWidth			=	g_sprInterface.getSpriteWidth(eACTIVE_SKILL_TAB);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);

	s_rectInterfaceSize.x1	=	g_iScreenWidth-(iSkillWindowWidth+iWindowWidth);
	s_rectInterfaceSize.x2	=	g_iScreenWidth;
	m_pos.x					=	g_iScreenWidth-(s_rectInterfaceSize.x2-s_rectInterfaceSize.x1);
	m_pos.y					=	iTitleHeight+g_iCorrectBI_Y/2;

	m_iRequireAbilityCount	=	0;

	getRequireAbility(_iObjectAbility);

	resetButtons();

	s_sbMiddleWindow.set(m_pos.x+iWindowWidth-16,m_pos.y+dEASY_SKILL_ITEM_HEIGHT+10,dEASY_SKILL_WINDOW_HEIGHT);
	s_sbMiddleWindow.setPos(0);
	s_sbMiddleWindow.setSize(m_iRequireAbilityCount,dEASY_SKILL_COUNT_IN_WINDOW);

	{
		cREQUIRE_SKILL	aRequireSkill[dABILITY_COUNT];

		memcpy(aRequireSkill,m_aRequireAbility,sizeof(m_aRequireAbility));

		int	iCount	=	0;

		for (int iLevel=1;iLevel<=5;iLevel++)
			for (int i=0;i<m_iRequireAbilityCount;i++)
			{
				int			iAbility	=	aRequireSkill[i].m_wSkill;
				cAbility	*lpAbility	=	&g_hero.m_aAbility[iAbility];
				CSkill		*lpSkill	=	lpAbility->getSkill();

				if (lpSkill->m_wDifficultyLevel	==	iLevel)
					memcpy(&m_aRequireAbility[iCount++],&aRequireSkill[i],sizeof(cREQUIRE_SKILL));
			}
	}
}

//
//	포커스 스킬을 리턴
int
cEasySkillTree::getFocusAbility()
{
	int	iX	=	m_pos.x+8;
	int	iY	=	m_pos.y+56;

	int	i,iCount;
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eSKILL_WINDOW);

	for (i=0,iCount=0;i<m_iRequireAbilityCount && iCount < dEASY_SKILL_COUNT_IN_WINDOW;i++)
	{
		int			iAbility	=	m_aRequireAbility[i].m_wSkill;
		cAbility	*lpAbility	=	&g_hero.m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (!lpSkill)
			continue;

		if (i < s_sbMiddleWindow.m_iCurrentPos)
			continue;

		int	iPutX	=	iX+4;
		int	iPutY	=	iY+i*dEASY_SKILL_ITEM_HEIGHT;

		cRECT	rect;

		rect.set(iPutX,iPutY,iPutX+iWindowWidth,iPutY+dEASY_SKILL_ITEM_HEIGHT);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(-2,-4);

			return	iAbility;
		}

		iCount++;
	}

	return	-1;
}

//
//	스킬 포인트 처리
void
cEasySkillTree::operateSkillPoint()
{
	resetButtonsPos();

	if (cINPDEV::IsExclusive(eMOUSE_STATE_INTERFACE))
		return;

	DWORD	dwMenu;

//	스킬 레벨
	{
		dwMenu	=	m_bmPlus.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (dwMenu	!=	0xffffffff)
		{
			int	iPoint	=	1;

			if	(s_aKey[KEY_LSHIFT])
				iPoint		=	10;

			//JBC	스킬 포인트 50씩 찍는 기능 유저도 사용가능 하게. 08-09-14
//#ifdef dDEBUG_MODE
			if	(s_aKey[KEY_LCONTROL])
				iPoint		=	50;
//#endif
			int	iRequireLevel=	getRequireAbilityLevel(dwMenu);
			iPoint			=	min(iPoint,iRequireLevel-g_hero.m_aAbility[dwMenu].m_wLevel);

			if	(iPoint	>	0	&&	m_iLastActionTime+500	<	timeGetTime())
			{
				m_iLastActionTime	=	timeGetTime();
				s_agent.sendIncreaseAbilityLevel(dwMenu,iPoint,iRequireLevel);
			}
		}
	}
}

//
//	아이템 인터페이스 업데이트
void
cEasySkillTree::update()
{
	if (m_iRequireAbilityCount	==	0)
		return;

	if (s_iPopupInterface	!=	ePIW_SKILL)
		return;

	if (!cINPDEV::IsExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR))
	{
		if (s_sbMiddleWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			
			cINPDEV::SetExclusive(eMOUSE_STATE_SHOP_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SHOP_SCROLL_BAR);
	}

	operateSkillPoint();

	if (cINPDEV::IsExclusive())
		return;
}

//
//	스킬 정보
void
cEasySkillTree::draw()
{
	if (m_iRequireAbilityCount	==	0)
		return;

	int		iX,iY,i,iCount=0;

	int		iTabHeight	=	g_sprInterface.getSpriteHeight(eACTIVE_SKILL_TAB);
	int		iWindowWidth=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight=	g_sprInterface.getSpriteHeight(eWINDOW);

	iX		=	m_pos.x;
	iY		=	m_pos.y;

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX	,g_iCorrectBI_Y/2		,eWINDOW_TITLE_SKILL_TREE);
	g_sprInterface.Put(iX	,iY		,eWINDOW);

	iX		=	m_pos.x+8;
	iY		=	m_pos.y+8;

	{
		cAbility	*lpAbility	=	&g_hero.m_aAbility[m_iObjectAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (!lpSkill)
			return;

		int	iPutX	=	iX+4;
		int	iPutY	=	iY;

		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_FRAME);
		g_smiIconSkill.put(lpSkill->m_wIconIndex,iPutX,iPutY);

		s_sText.put(iPutX+38,iPutY+2,WHITE,lpSkill->m_strName);

		if (g_hero.isLevelUpAbleAbility(m_iObjectAbility) || lpAbility->m_wLevel)
			s_sText.put(iPutX+38,iPutY+20,RGB(219,192,100),"Lv <c:LTGREEN>%d<n>",lpAbility->m_wLevel);
		else
			s_sText.put(iPutX+38,iPutY+20,RGB(219,192,100),"Lv <c:LTRED>%d<n>",lpAbility->m_wLevel);

		g_sprInterface.Put(m_pos.x+7,iY+36		,eWINDOW_SEPERATE_BAR);	//	경계

		g_sprInterface.Put(m_pos.x+7,m_pos.y+iWindowHeight-30	,eWINDOW_SEPERATE_BAR);	//	경계
		iY		+=	48;
	}

	s_sText.disableFitRightEdge();
	s_sText.setLineGap(0);

	for (i=0,iCount=0;i < m_iRequireAbilityCount	&&	iCount < dEASY_SKILL_COUNT_IN_WINDOW;i++)
	{
		int			iAbility	=	m_aRequireAbility[i].m_wSkill;
		cAbility	*lpAbility	=	&g_hero.m_aAbility[iAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if (!lpSkill)
			break;

		if (i < s_sbMiddleWindow.m_iCurrentPos)
			continue;

		int	iPutX	=	iX+4;
		int	iPutY	=	iY+iCount*dEASY_SKILL_ITEM_HEIGHT;

		g_sprInterface.Put(iPutX,iPutY,eIWI_ITEM_SLOT_FRAME);
		g_smiIconSkill.put(lpSkill->m_wIconIndex,iPutX,iPutY);

		s_sText.put(iPutX+38,iPutY+2,WHITE,lpSkill->m_strName);

		if	(lpAbility->m_wLevel	>=	m_aRequireAbility[i].m_wLevel)
			s_sText.put(iPutX+38,iPutY+20,RGB(219,192,100),"Lv <c:LTGREEN>%d<n>[<c:LTYELLOW>%d<n>]",lpAbility->m_wLevel,m_aRequireAbility[i].m_wLevel);
		else
			s_sText.put(iPutX+38,iPutY+20,RGB(219,192,100),"Lv <c:LTRED>%d<n>[<c:LTYELLOW>%d<n>]",lpAbility->m_wLevel,m_aRequireAbility[i].m_wLevel);

		iCount++;
	}

	s_text.put(iX+10,m_pos.y+iWindowHeight-21,WHITE,dMSG_PLUS_SKILL_TIP_ABOUT_SHIFT);

	s_sText.setLineGap(2);
#ifndef	_FOR_ENG
	s_sText.enableFitRightEdge();
#endif

	m_bmPlus.draw();
	s_sbMiddleWindow.draw();
}