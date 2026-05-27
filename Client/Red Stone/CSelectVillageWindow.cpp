#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CMessage.H"
#include "CWindowInterface.H"

CSelectVillageWindow	g_gwSelectVillage;

//	열기
void
CSelectVillageWindow::open(WORD *_lpwList,int _iCount,BOOL _bIsCreatePortal)
{
	ClosePopupWindow();

	g_hero.lockOff();
	cINPDEV::ReleaseExclusive();
	ResetAllFocus();
	ClosePopupWindow();

	for (int i=0;i<_iCount;i++)
		m_aiVillageList[i]	=	_lpwList[i];
	
	m_iVillageCount			=	_iCount;

	m_iSelectVillage		=	0;

	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_SHOP);

	s_rectInterfaceSize.x1	=	g_iScreenWidth-iWindowWidth;
	s_rectInterfaceSize.x2	=	g_iScreenWidth;
	s_iPopupInterface		=	ePIW_SELECT_VILLAGE;
	m_pos.x					=	g_iScreenWidth-iWindowWidth;
	m_pos.y					=	iTitleHeight+g_iCorrectBI_Y/2;

	s_bmButton.reset();

	s_bmButton.addImageBar(dMSG_SELECT	,IDOK		,m_pos.x+130,470+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmButton.addImageBar(dMSG_CLOSE	,IDCANCEL	,m_pos.x+196,470+g_iCorrectBI_Y/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);
	s_bmButton.setClickMargin(IDOK,1,1);
	s_bmButton.setClickMargin(IDCANCEL,1,1);
	s_bmButton.setAllBarTextColor(0,FALSE);

	m_bIsReadyToCreatePortal=	_bIsCreatePortal;

	InitPopupWindowCloseButton();
}

//
//	딛기
void
CSelectVillageWindow::close()
{
	cINPDEV::ReleaseExclusive();

	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
}


//
//	그리기
void
CSelectVillageWindow::draw()
{
	if	(!isOpened())
		return;

	int	iX				=	m_pos.x;
	int	iY				=	m_pos.y;

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill  (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface2.Put(iX	,g_iCorrectBI_Y/2	,66);
	g_sprInterface.Put(iX	,iY	,eWINDOW);

	iX				=	m_pos.x+12;
	iY				=	m_pos.y+8;

	s_text.put(iX,iY,WHITE,dMSG_PLEASE_SELECT_MOVE_FIELD);

	iY	=	m_pos.y+26;

	for (int i=0;i<m_iVillageCount;i++)
	{
		int	iMap	=	m_aiVillageList[i];

		if	(g_aMapSimpleInfo[iMap].m_wSerial	==	0xffff)
			continue;

		DWORD	dwColor	=	LTGRAY;

		if	(i == m_iSelectVillage)
			dwColor	=	LTGREEN;
		else
		if	(i == m_iSelectVillage)
			dwColor	=	WHITE;

		s_sText.put(iX,iY,dwColor,"%s",g_aMapSimpleInfo[iMap].m_strName);

		iY	+=	14;
	}

	g_sprInterface.Put(m_pos.x+7,461,eWINDOW_SEPERATE_BAR);	//	경계

	s_bmButton.draw();
}

int
CSelectVillageWindow::getFocusVillage()
{
	int	iX				=	m_pos.x+12;
	int	iY				=	m_pos.y+26;

	for (int i=0;i<m_iVillageCount;i++)
	{
		int	iMap	=	m_aiVillageList[i];

		if (g_aMapSimpleInfo[iMap].m_wSerial	==	0xffff)
			continue;

		cRECT	rect;

		rect.set(iX,iY-2,g_iScreenWidth,iY+12);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i;

		iY	+=	14;
	}

	return	0xffff;
}

void
CSelectVillageWindow::update()
{
	if (!isOpened())
		return;
	
	if (cINPDEV::IsExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON))
		return;

	DWORD	dwMenu		=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	IDOK		:
		{
			if	(m_iSelectVillage	<	0	||	m_iSelectVillage	>=	m_iVillageCount)
				break;

			s_agent.sendWarpToNearVillage(m_aiVillageList[m_iSelectVillage]);

			close();
			return;
		}

		case	IDCANCEL	:
			close();
			return;
	}

	if (s_bmButton.isPressed())
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_PITCHMAN_SHOP_BUTTON);
		return;
	}

	if (cINPDEV::IsExclusive())
	{
		m_iFocusVillage	=	0xffff;

		return;
	}

	m_iFocusVillage	=	getFocusVillage();

	if (s_isTouchLeftButton && m_iFocusVillage	!=	0xffff)
		m_iSelectVillage	=	m_iFocusVillage;
}

BOOL
CSelectVillageWindow::isOpened()
{
	if (s_iPopupInterface	==	ePIW_SELECT_VILLAGE)
		return	TRUE;

	return	FALSE;
}