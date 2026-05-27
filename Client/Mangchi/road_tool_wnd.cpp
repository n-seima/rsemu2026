#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cTOOLWND.H"
#include	"cMAIN.H"
#include	"cOBJECT.H"
#include	"cBUILDING.H"
#include	"cAREA.H"
#include	"cMAPWND.H"
#include	"cMESSAGE.H"
#include	"cSET.H"
#include	"patrolRoad.H"

void
cTOOLWND::UpdateRoad()
{
	if (!IsCorrectMode(dWORK_ROAD))
		return;

	if (!TOUCHLBUTTON	&&	!TOUCHRBUTTON		)
		return;

	MAIN.ChangeMode(dWORK_ROAD);

	int	width		=	(cDRAW::WIDTH - 18);
	int	height		=	12+6;
	int	yp			=	34;
	int count;

	TPH				=	(cWND::height() - 8 - 30) / height;

	int	iAreaType	=	0;

	count	=	0;

	g_lpActmap->m_prManager.setFirst();

	while(1)
	{
		cPatrolRoad	*lpRoad	=	g_lpActmap->m_prManager.getNext();

		if (!lpRoad)
			break;

		if (count < RoadPos)
		{
			count++;
			continue;
		}

		int xp	=	8;

		cRECT rect;

		rect.Set(xp,yp-3,xp+width,yp+12+3);

		if (rect.isIN(Mouse.x,Mouse.y))
		{	
			g_lpActmap->m_prManager.m_iSelect	=	lpRoad->m_wSerial;

			if (LEFTDBLCLICK)
			{
				cPatrolRoad	*lpRoad	=	g_lpActmap->m_prManager.get(g_lpActmap->m_prManager.m_iSelect);

				if (lpRoad)
				{
					int	x	=	(lpRoad->m_aNode[0].x)/dTILE_REAL_WIDTH  - g_MAPWND.TPW/2;
					int	y	=	(lpRoad->m_aNode[0].y)/dTILE_REAL_HEIGHT - g_MAPWND.TPH/2;

					g_lpActmap->setPos(x,y);
					g_MAPWND.Draw();
				}
			}


			MAIN.Draw();
		}

		yp	+=	height;
		count++;
	}

	if (TOUCHRBUTTON && g_lpActmap->m_prManager.get())
	{
		POINT	Pos;
		HMENU	hMenu	=	CreatePopupMenu();

		AppendMenu(hMenu,MFT_STRING		,IDM_REMOVE_ROAD	,"제거");
		AppendMenu(hMenu,MFT_STRING		,IDM_EDIT_ROAD		,"수정");

		GetCursorPos(&Pos);
		TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,Pos.x,Pos.y,0,hWND,NULL);
		DestroyMenu(hMenu);
	}
}

//
//	팝업 메뉴
void
cTOOLWND::UpdateRoadCommand(int menu)
{
	if (!IsCorrectMode(dWORK_ROAD))
		return;

	MAIN.ChangeMode(dWORK_ROAD);

	switch(menu)
	{
		case	IDM_REMOVE_ROAD			:	//	기존의 영역 제거
		{
			if (cMSG::YESNO("선택한 순찰로 제거",dMSG_QUESTION_CONTINUE) == IDNO)
				return;

			if (g_lpActmap->m_prManager.remove())
			{
				Draw();
				g_MAPWND.Draw();
			}
			break;
		}

		case	IDM_EDIT_ROAD			:	//	기존의 영역 수정
		{
			if (cSetPatrolRoad::Open(hWND,g_lpActmap->m_prManager.get()))
			{
				g_lpActmap->m_prManager.replace(cSetPatrolRoad::s_patrolRoad.m_wSerial,&cSetPatrolRoad::s_patrolRoad);

				Draw();
				g_MAPWND.Draw();
			}
			break;
		}
	}
}

void
cTOOLWND::DrawRoad()
{
	if (!IsCorrectMode(dWORK_ROAD))
		return;

	cDRAW::Fill(_SKYBLUE);
	cDRAW::Box(0	,0,0,cDRAW::WIDTH,cWND::height(),2);

	int	width		=	(cDRAW::WIDTH - 18);
	int	height		=	12+6;
	int	yp			=	34;
	int count;

	TPH				=	(cWND::height() - 8 - 30) / height;

	SetScrollBar(g_lpActmap->m_prManager.getCount(),&RoadPos);

	int	iAreaType	=	0;

	count	=	0;

	g_lpActmap->m_prManager.setFirst();

	cTEXT::PushShadow(FALSE);

	while(1)
	{
		cPatrolRoad	*lpRoad	=	g_lpActmap->m_prManager.getNext();

		if (!lpRoad)
			break;

		if (count < RoadPos)
		{
			count++;
			continue;
		}

		int		 xp	=	8;

		char	*strReturn[]	=	{" ","(R)"};
		char	strName[128];

		sprintf(strName,"%s [%d초] %s",lpRoad->m_strName,lpRoad->m_wStayTime,strReturn[lpRoad->m_wIsReturnRoad]);

		if (lpRoad->m_wSerial	== g_lpActmap->m_prManager.m_iSelect)
		{
			cDRAW::Fill(_BLUE	,xp  ,yp  ,xp+width+4,yp+14+1);
			cDRAW::Fill(_LTBLUE	,xp-2,yp-2,xp+width+2,yp+12+1);

			cTEXT::Put(xp,yp,LTYELLOW,strName);
		}
		else
		{
			cTEXT::PushShadow(FALSE);
			cTEXT::Put(xp,yp,0,strName);
			cTEXT::PopShadow();
		}

		yp	+=	height;
		count++;
	}

	cTEXT::PopShadow();
}