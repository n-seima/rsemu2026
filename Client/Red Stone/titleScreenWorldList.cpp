#include "cGAME_TITLE.H"
#include "cJOB.H"
#include "CActor.H"
#include "CAgent.H"
#include "CSound.H"
#include "CGamePlayInterface.H"
#include "CGamePlay.H"
#include "CMessage.H"
#include "CWindowInterface.h"
#include "itemSet.h"

enum
{
	eCSWI_CARROT	=	33,
	eCSWI_TITLE		=	67,
	eCSWI_FIRST_BAR	=	72,
	eSWLI_FIRST_BAR	=	93,
	eSWLI_ZERO		=	105,
	eSWLI_TEST_SERVER=	115,
	eSWLI_DUEL		=	166,
};

int
cGAME_TITLE::GetSelectWorld()
{
	int	iBarHeight	=	g_sprInterface2.getSpriteHeight(eSWLI_FIRST_BAR)+2;
	int	iX			=	545+g_iCorrectBI_X,i,j;
	int	iY			=	s_iLoginServerLineCount*iBarHeight+86;
	int	iWorldCount	=	0;
	int	iRectHeight	=	s_rectServerListBorder.y2-iY;
	int	iWindowWidth=	s_rectServerListBorder.getWidth()-12;
	int	iBarWidth	=	iWindowWidth-8;
	int	iPutBarWidth=	iBarWidth-4;
	int	iCoulums	=	1,iSelect=-1;
	int	iPannelDeltaWidth	=	(g_iScreenWidth - 517+g_iCorrectBI_X)*(32-g_canvas.getFadeScreenValue())/32;
	CPos	pos;

	if	((iRectHeight+2)/iBarHeight	<	s_iWorldServerCount)
	{
		iX			=	544+g_iCorrectBI_X;
		iCoulums	=	2;
		iBarWidth	=	iWindowWidth/2;
		iPutBarWidth=	iBarWidth-10;
	}

	iX				+=	iPannelDeltaWidth;

	for	(i=0;i<100;i++)
	{
		for (j=0;j<s_iWorldServerCount;j++)
		{
			if	(s_aServerList[j].m_wServerPlaceInList	!=	i)
				continue;
#ifndef _DEBUG
			if	(STRICMP(s_aServerList[j].m_strName,"")	==	0	)
				continue;
#endif
			cRECT	rect;

			pos.x	=	iX+(iWorldCount%iCoulums)*iBarWidth;
			pos.y	=	iY+(iWorldCount/iCoulums)*iBarHeight;

			rect.set(pos.x,pos.y,pos.x+iBarWidth,pos.y+iBarHeight);

			BOOL	bIsEnable	=	TRUE;

			int		iServerType	=	s_aServerList[j].m_wType;

#ifndef _DEBUG
			if	(iServerType	==	eSERVER_TYPE_INNER_TEST || iServerType	==	eSERVER_TYPE_OFF)
				bIsEnable	=	FALSE;
#else
			if	(iServerType	==	eSERVER_TYPE_OFF)	
				bIsEnable	=	FALSE;
#endif

			if	(bIsEnable	&&	rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				iSelect	=	j;
				strcpy(s_strFocusServerName,s_aServerList[j].m_strName);

				s_rectFocusWorld.set(&rect);
			}

			iWorldCount++;
		}
	}

//	s_iWorldServerCount	=	iWorldCount;

	return	iSelect;
}

void
cGAME_TITLE::DrawWorldList()
{
	int	iBarHeight	=	g_sprInterface2.getSpriteHeight(eSWLI_FIRST_BAR)+2;
	int	iX			=	545+g_iCorrectBI_X,i,j;
	int	iY			=	s_iLoginServerLineCount*iBarHeight+86;
	int	iWorldCount	=	0;
	int	iRectHeight	=	s_rectServerListBorder.y2-iY;
	int	iWindowWidth=	s_rectServerListBorder.getWidth()-12;
	int	iBarWidth	=	iWindowWidth-8;
	int	iPutBarWidth=	iBarWidth-4;
	int	iCoulums	=	1;
	int	iPannelDeltaWidth	=	(g_iScreenWidth - 517+g_iCorrectBI_X)*(32-g_canvas.getFadeScreenValue())/32;
	CPos	pos;

	if	((iRectHeight+2)/iBarHeight	<	s_iWorldServerCount)
	{
		iX			=	544+g_iCorrectBI_X;
		iCoulums	=	2;
		iBarWidth	=	iWindowWidth/2;
		iPutBarWidth=	iBarWidth-10;
	}

	iX				+=	iPannelDeltaWidth;

	for	(i=0;i<100;i++)
	{
		for (j=0;j<s_iWorldServerCount;j++)
		{
			if	(s_aServerList[j].m_wServerPlaceInList	!=	i)
				continue;

			BOOL	bIsInnerOpen	=	FALSE;

			if	(s_aServerList[j].m_bf1IsInnerOpen || s_aServerList[j].m_wType	==	eSERVER_TYPE_INNER_TEST)
				bIsInnerOpen	=	TRUE;

#ifndef _DEBUG
			if	(bIsInnerOpen	||	STRICMP(s_aServerList[j].m_strName,"")	==	0	)
				continue;
#endif

			int		iImage		=	eSWLI_FIRST_BAR;
			DWORD	dwColor		=	0;
			int		iServerType	=	s_aServerList[j].m_wType;

#ifndef _DEBUG
			if	(iServerType	==	eSERVER_TYPE_INNER_TEST || iServerType	==	eSERVER_TYPE_OFF)
				iImage	+=	9;
#else
			if	(iServerType	==	eSERVER_TYPE_OFF)	
				iImage	+=	9;
#endif
			else
			if	(s_aServerList[j].m_wServerPlaceInList	==	s_iSelectServer)
			{
				iImage	+=	3;
				dwColor	=	LTBLUE;
			}
			else
			if	(j	==	s_iFocusServer)
				iImage	+=	3;

			int	iFirstNumber	=	iWorldCount/10;
			int	iSecondNumber	=	iWorldCount%10;

			pos.x	=	iX+(iWorldCount%iCoulums)*iBarWidth;
			pos.y	=	iY+(iWorldCount/iCoulums)*iBarHeight;

			g_sprInterface2.put3PieceBar(pos.x,pos.y,iImage,iPutBarWidth);
			g_sprInterface2.Put(pos.x+1,pos.y+10,eSWLI_ZERO+iFirstNumber);
			g_sprInterface2.Put(pos.x+7,pos.y+10,eSWLI_ZERO+iSecondNumber);

			if	(s_aServerList[j].m_wType	==	eSERVER_TYPE_TEST)
				g_sprInterface2.Put(pos.x+iPutBarWidth-36,pos.y+5,eSWLI_TEST_SERVER);
			if	(s_aServerList[j].m_wType	==	eSERVER_TYPE_DUEL)
				g_sprInterface2.Put(pos.x+iPutBarWidth-36,pos.y+4,eSWLI_DUEL);

			s_text.pushShadowStatus(FALSE);

			if	(bIsInnerOpen)
				s_text.put(pos.x+18,pos.y+5,dwColor,"IO_%s",s_aServerList[j].m_strName);
			else
			if	(iServerType	==	eSERVER_TYPE_GVG)
				s_text.put(pos.x+18,pos.y+5,dwColor,"%s[G.V.G]",s_aServerList[j].m_strName);
			else
				s_text.put(pos.x+18,pos.y+5,dwColor,s_aServerList[j].m_strName);
			s_text.popShadowStatus();

			iWorldCount++;
		}
	}
}