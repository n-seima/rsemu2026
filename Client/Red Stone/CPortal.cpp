#include "CPortal.h"
#include "CMap.h"
#include "CActor.h"
#include "CMessage.h"
#include "CPitchmanShop.h"

CPortalManager	g_portal;

char	g_strMinimapCheckNpc[20]	=	"";

void
CPortalManager::reset()
{
	for (int i=0;i<dMAX_PORTAL_COUNT;i++)
		m_aPortal[i].m_wSerial	=	0xffff;

	m_iMovePortal			=	0xffff;
	m_iFocusPortal			=	0xffff;
	m_iWaitEnterPortalTime	=	0;
}

int	
CPortalManager::getViewPortal(cINDEX *_lpIndexes,cRECT *_lpRectViewport)
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_PORTAL_COUNT;i++)
	{
		if (m_aPortal[i].m_wSerial == 0xffff)
			continue;

		if (_lpRectViewport->isIn(m_aPortal[i].m_wX,m_aPortal[i].m_wY))
		{
			_lpIndexes[iCount].Index	=	dBORDER_PORTAL+i;
			_lpIndexes[iCount].Value	=	m_aPortal[i].m_wY;
			iCount++;
		}
	}

	return	iCount;
}

BOOL
CPortalManager::isFocus(int _iSerial,int _iX,int _iY)
{
	if (m_aPortal[_iSerial].m_wSerial	==	0xffff)
		return	FALSE;

	if (g_pitchmanShop.getFocusShop()	!=	0xffff)
		return	FALSE;

	int	iX	=	m_aPortal[_iSerial].m_wX;
	int	iY	=	m_aPortal[_iSerial].m_wY;

	cRECT	rect;

	rect.set(iX-80,iY-140,iX+80,iY);

	return	rect.isIn(_iX,_iY);
}

void
CPortalManager::update()
{
	if	(m_iMovePortal	==	0xffff)
		m_iFocusPortal	=	0xffff;

	if	(g_map.m_area.m_iMoveArea	!=	0xffff)
		return;

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_MOVE_TO_PORTAL))
		return;

	if	(IsOpenedEventWindow())
		return;

	if	(s_isTouchLeftButton)
	{
		m_iFocusPortal	=	getFocusPortal();
		m_iMovePortal	=	m_iFocusPortal;

		if	(m_iMovePortal	!=	0xffff)
			cINPDEV::SetExclusive(eMOUSE_STATE_MOVE_TO_PORTAL);
	}

	if	(s_isLeftButtonPressed	==	0)	//	마우스 버튼을 뗐다.
	{
		m_iFocusPortal	=	getFocusPortal();
		m_iMovePortal	=	0xffff;

		cINPDEV::ReleaseExclusive();
		return;
	}

	if	(m_iMovePortal	==	0xffff	||	m_iMovePortal	>=	dMAX_PORTAL_COUNT)
		return;

	if	(m_iWaitEnterPortalTime)
	{
		m_iWaitEnterPortalTime--;

		return;
	}

	if	(m_aPortal[m_iMovePortal].m_wSerial	==	0xffff)
	{
		m_iMovePortal			=	0xffff;
		m_iWaitEnterPortalTime	=	0;

		return;
	}

	if	(isFocus(m_iMovePortal,g_am.m_posFocus.x,g_am.m_posFocus.y))
	{
		m_iWaitEnterPortalTime	=	dWAIT_MOVE_AREA_RESULT_TIME;

		char	strText[256];

		if	(m_aPortal[m_iMovePortal].m_wIsTownPortal)
		{
			sprintf(strText,dMSG_MOVE_TOWN_PORTAL_QUESTION_FORM,g_aMapSimpleInfo[m_aPortal[m_iMovePortal].m_wField].m_strName);
		}
		else
		{
			sprintf(strText,dMSG_MOVE_PORTAL_QUESTION_FORM,g_aMapSimpleInfo[m_aPortal[m_iMovePortal].m_wField].m_strName);
		}

		g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_ENTER_TOWN_PORTAL);

		m_iEnterPortal	=	m_iMovePortal;

		cINPDEV::ReleaseExclusive();
	}
	else
	{
		g_hero.moveTo(m_aPortal[m_iMovePortal].m_wX,m_aPortal[m_iMovePortal].m_wY);
	}
}

int
CPortalManager::getFocusPortal()
{
	for (int i=0;i<dMAX_PORTAL_COUNT;i++)
	{
		if (m_aPortal[i].m_wSerial == 0xffff)
			continue;

		if (isFocus(i,s_posCursor.x,s_posCursor.y))
			return	i;
	}

	return	0xffff;
}

void
CPortalManager::draw(int _iSerial)
{
	if	(m_aPortal[_iSerial].m_wSerial	==	0xffff)
		return;

	int	iX	=	GetScaledXPos(m_aPortal[_iSerial].m_wX);
	int	iY	=	GetScaledYPos(m_aPortal[_iSerial].m_wY);

	cANM	*lpPortalImage	=	g_im.getEffect(g_im.m_wTownPortalImage);

	lpPortalImage->PutShadow(iX,iY,0,0,0,g_iGameScale,g_iGameScale);

	if	(m_iFocusPortal	==	_iSerial)
	{
		if	(m_aPortal[_iSerial].m_wIsTownPortal)
			g_im.putEffect(g_im.m_wTownPortalImage,iX,iY,s_iFrameCounter,1,0,g_iGameScale,g_iGameScale);
		else
			g_im.putEffect(g_im.m_wPortalImage,iX,iY,s_iFrameCounter,1,0,g_iGameScale,g_iGameScale);

		int	iSize	=	s_text.getPixelSize(g_aMapSimpleInfo[m_aPortal[_iSerial].m_wField].m_strName);

		cRECT	rect;

		rect.set(iX-iSize/2-10,iY-130,iX+iSize/2+10,iY-102);

		s_ttDialogFrame.drawTextBox(&rect,0,0xffff,g_aMapSimpleInfo[m_aPortal[_iSerial].m_wField].m_strName,FALSE,32);
	}
	else
	{
		if	(m_aPortal[_iSerial].m_wIsTownPortal)
			g_im.putEffect(g_im.m_wTownPortalImage,iX,iY,s_iFrameCounter,0,0,g_iGameScale,g_iGameScale);
		else
			g_im.putEffect(g_im.m_wPortalImage,iX,iY,s_iFrameCounter,0,0,g_iGameScale,g_iGameScale);
	}
}

BOOL
CPortalManager::remove(int _iSerial)
{
	m_aPortal[_iSerial].m_wSerial	=	0xffff;

	return	TRUE;
}

BOOL
CPortalManager::add(int _iSerial,int _iField,int _iOwner,int _iX,int _iY,BOOL _bIsTownPortal)
{
	m_aPortal[_iSerial].m_wOwner		=	_iOwner;
	m_aPortal[_iSerial].m_wField		=	_iField;
	m_aPortal[_iSerial].m_wSerial		=	_iSerial;
	m_aPortal[_iSerial].m_wX			=	_iX;
	m_aPortal[_iSerial].m_wY			=	_iY;
	m_aPortal[_iSerial].m_wIsTownPortal	=	_bIsTownPortal;

	return	TRUE;
}
