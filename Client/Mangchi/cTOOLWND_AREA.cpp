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

#define	IDM_REMOVE_AREA			2000
#define	IDM_EDIT_AREA			2001

int		l_aiSortArea[]	=	{dAREA_NAMED_AREA,dAREA_DOOR,dAREA_TRAP,dAREA_EVENT_AREA,dAREA_PORTAL,dAREA_ARCA,dAREA_START_AREA,dAREA_REVIVE_AREA,-1};

//#define	IDM_ADD_SPECIAL_AREA	2002
//
//	툴 윈도우에 영역 리스트를 업데이트 한다.
//
void
cTOOLWND::UpdateAreas()
{
	if	(!IsCorrectMode(dWORK_AREA))
		return;

	if	(!TOUCHLBUTTON	&&	!TOUCHRBUTTON		)
		return;

	if	((LBUTTON || RBUTTON) && !client.isIN(Mouse.x,Mouse.y))
	{	//09.07.10  추가... 
		return;
	}

	MAIN.ChangeMode(dWORK_AREA);

	int	width		=	(cDRAW::WIDTH - 10);
	int	height		=	12+6;
	int	iSelectArea	=	0xffff;
	int	yp			=	34;
	int index,count;

	cAREA	*lpArea	=	g_lpActmap->m_pArea;
	TPH				=	(cWND::height()-8- 30) / height;

	int	iAreaType	=	0;

	count	=	0;

	while(1)
	{
		int	iCurrentType	=	l_aiSortArea[iAreaType];

		if	(iCurrentType	==	-1)
			break;

		iAreaType++;

		for (index=2;index<dMAX_AREA;index++)
		{
			cAREA_INFO	*lpAreaInfo		=	&lpArea->m_aArea[index];

			if (lpAreaInfo->m_wSerial	== 0xffff)
				continue;
			if (lpAreaInfo->m_wKind	!=	iCurrentType)
				continue;

			if (yp +(height-6)	>=	cWND::height() - 4	)
				break;

			int	xp	=	8;

			count++;

			if (count-1 < AreaPos)
				continue;

			cRECT rect;

			rect.Set(xp,yp-3,xp+width,yp+12+3);

			if (rect.isIN(Mouse.x,Mouse.y) && index !=0)
			{	
				g_lpActmap->m_pArea->Select(index);
				iSelectArea	=	index;

				MAIN.Draw();
			}

			yp	+=	height;
		}
	}

	for (index=2;index<dMAX_AREA;index++)
	{
		cAREA_INFO	*lpAreaInfo		=	&lpArea->m_aArea[index];

		if (lpAreaInfo->m_wSerial	== 0xffff)
			continue;

		BOOL	bIsSpecial		=	FALSE;

		for (int j=0;;j++)
		{
			if (l_aiSortArea[j]	==	-1)
				break;
			if (l_aiSortArea[j]	==	lpAreaInfo->m_wKind)
				bIsSpecial		=	TRUE;
		}

		if (bIsSpecial)
			continue;

		if (yp+(height-6)>=	cWND::height() - 4	)
			break;

		int xp	=	8;

		count++;

		if (count-1 < AreaPos)
			continue;

		cRECT rect;

		rect.Set(xp,yp-3,xp+width,yp+12+3);

		if (rect.isIN(Mouse.x,Mouse.y) && index !=0)
		{	
			g_lpActmap->m_pArea->Select(index);
			iSelectArea	=	index;

			MAIN.Draw();
		}

		yp	+=	height;
	}

	if (TOUCHRBUTTON)
	{
		POINT	Pos;
		HMENU	hMenu	=	CreatePopupMenu();

		if	(!g_lpActmap->m_bf1IsLocked)					// 09.07.24   맵그리기금지....
			AppendMenu(hMenu,MFT_STRING		,IDM_REMOVE_AREA		,dMSG_REMOVE_AREA);

		AppendMenu(hMenu,MFT_STRING		,IDM_EDIT_AREA			,dMSG_EDIT_AREA);

		if	(iSelectArea	==	0xffff)
		{
			EnableMenuItem(hMenu,IDM_REMOVE_AREA	,MF_BYCOMMAND|MF_GRAYED	);
			EnableMenuItem(hMenu,IDM_EDIT_AREA	,MF_BYCOMMAND|MF_GRAYED	);
		}

		GetCursorPos(&Pos);
		TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,Pos.x,Pos.y,0,hWND,NULL);
		DestroyMenu(hMenu);
	}
}


//
//	영역 리스트를 표시한다.
//
void
cTOOLWND::DrawAreas()
{
	if (!IsCorrectMode(dWORK_AREA))
		return;

	cDRAW::Fill(_SKYBLUE);
	cDRAW::Box(0	,0,0,cDRAW::WIDTH,cWND::height(),2);


	int	width		=	(cDRAW::WIDTH - 18);
	int	height		=	12+6;
	int	yp			=	34;
	int index,count;

	cAREA *lpArea	=	g_lpActmap->m_pArea;
	TPH				=	(cWND::height() - 8 - 30) / height;

	SetScrollBar(lpArea->m_iCount-2,&AreaPos);

	int	iAreaType	=	0;

	count	=	0;

	while(1)
	{
		int	iCurrentType	=	l_aiSortArea[iAreaType];

		if	(iCurrentType	==	-1)
			break;

		iAreaType++;

		for (index=2;index<dMAX_AREA;index++)
		{
			cAREA_INFO	*lpAreaInfo	=	&lpArea->m_aArea[index];

			if (lpAreaInfo->m_wSerial	== 0xffff)
				continue;
			if (lpAreaInfo->m_wKind	!=	iCurrentType)
				continue;
			if (yp+(height-6)>=	cWND::height() - 4	)
				break;

			int xp	=	8;

			count++;

			if (count-1 < AreaPos)
				continue;

			char	strAreaName[100];
			DWORD	dwColor	=	DKGREEN;

			strcpy(strAreaName,lpAreaInfo->m_strName);

			switch(iCurrentType)
			{
				case	dAREA_DOOR			:
					dwColor	=	LTPURPLE;
					sprintf(strAreaName,"[D] %s",lpAreaInfo->m_strName);
					break;
				case	dAREA_TRAP			:
					dwColor	=	LTRED;
					sprintf(strAreaName,"[T] %s",lpAreaInfo->m_strName);
					break;
				case	dAREA_ARCA			:
					dwColor	=	LTPURPLE;
					sprintf(strAreaName,"[A] %s",lpAreaInfo->m_strName);
					break;
				case	dAREA_EVENT_AREA	:
				{
					dwColor	=	LTPURPLE;

					cAreaEvent	*lpEventArea	=	(cAreaEvent	*)lpAreaInfo;

					if	(g_lpActmap->m_object.getFixedObject(lpEventArea->m_wLinkObject))
						sprintf(strAreaName,"[E] %s(L)[%d]",lpAreaInfo->m_strName,lpEventArea->m_wLinkObject);
					else
					{
						sprintf(strAreaName,"[E] %s(X)",lpAreaInfo->m_strName);
						lpEventArea->m_wLinkObject	=	0xffff;
					}
					break;
				}
				case	dAREA_PORTAL		:
					sprintf(strAreaName,"[G] %s",lpAreaInfo->m_strName);
					break;
				case	dAREA_START_AREA	:
					sprintf(strAreaName,"[S] %s",lpAreaInfo->m_strName);
					break;
				case	dAREA_NAMED_AREA	:
					dwColor	=	DKPURPLE;
					sprintf(strAreaName,"[N] %s",lpAreaInfo->m_strName);
					break;
				case	dAREA_REVIVE_AREA	:
					sprintf(strAreaName,"[R] %s",lpAreaInfo->m_strName);
					break;

				case	dAREA_SECRET_DUNGEON_EXIT	:
					sprintf(strAreaName, "[exit] %s", lpAreaInfo->m_strName);
					break;
			}

			if (index == g_lpActmap->m_pArea->m_iSelectArea)
			{
				cDRAW::Fill(_BLUE	,xp  ,yp  ,xp+width+4,yp+14+1);
				cDRAW::Fill(_LTBLUE	,xp-2,yp-2,xp+width+2,yp+12+1);

				cTEXT::Put(xp,yp,LTYELLOW,strAreaName);
			}
			else
			{
				cTEXT::PushShadow(FALSE);
				cTEXT::Put(xp,yp,dwColor	,strAreaName);
				cTEXT::PopShadow();
			}

			yp	+=	height;
		}
	}

	for (index=2;index<dMAX_AREA;index++)
	{
		cAREA_INFO	*lpAreaInfo	=	&lpArea->m_aArea[index];

		if (lpAreaInfo->m_wSerial	== 0xffff)
			continue;

		BOOL	bIsSpecial		=	FALSE;

		for (int j=0;;j++)
		{
			if (l_aiSortArea[j]	==	-1)
				break;
			if (l_aiSortArea[j]	==	lpAreaInfo->m_wKind)
				bIsSpecial		=	TRUE;
		}

		if (bIsSpecial)
			continue;
		if (yp+(height-6)>=	cWND::height() - 4	)
			break;

		int xp	=	8;

		count++;

		if (count-1 < AreaPos)
			continue;

		if (index == g_lpActmap->m_pArea->m_iSelectArea)
		{
			cDRAW::Fill(_BLUE	,xp  ,yp  ,xp+width+4,yp+14+1);
			cDRAW::Fill(_LTBLUE	,xp-2,yp-2,xp+width+2,yp+12+1);

			cTEXT::Put(xp,yp,LTYELLOW,lpAreaInfo->m_strName);
		}
		else
		{
			cTEXT::PushShadow(FALSE);
			cTEXT::Put(xp,yp,BLACK,lpAreaInfo->m_strName);
			cTEXT::PopShadow();
		}

		yp	+=	height;
	}
}

//
//	팝업 메뉴
void
cTOOLWND::UpdateAreaCommand(int menu)
{
	if (!IsCorrectMode(dWORK_AREA))
		return;

	MAIN.ChangeMode(dWORK_AREA);

	switch(menu)
	{
		case	IDM_REMOVE_AREA			:	//	기존의 영역 제거
		{
			if (cMSG::YESNO(dMSG_REMOVE_SELECT_AREA,dMSG_QUESTION_CONTINUE) == IDNO)	return;

			if (g_lpActmap->m_pArea->Del())
			{	TOOLWND.Draw();
				g_MAPWND.Draw();
				// 10.01.15
				cSET_AREA_EDIT::ResetAreData();
			}
			break;
		}

		case	IDM_EDIT_AREA			:	//	기존의 영역 수정
		{
			cSET_AREA::SetArea(hINST,hWND,NULL,TRUE);
			break;
		}
	}
}	//	cTOOLWND::UpdateHeroCommand(int menu)

//
//	스훼셜 영역 초기화
void
cTOOLWND::InitAddSpecialArea(HWND hdlg)
{
	CheckDlgButton(hdlg,IDC_POLL_HERO,TRUE);
}

LRESULT CALLBACK 
cTOOLWND::s_procAddSpecialArea(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{	switch(message)
	{	case WM_INITDIALOG	:
			TOOLWND.InitAddSpecialArea(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDOK);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
//					TOOLWND.AddSpecialArea(hDlg);
					break;

				case	IDCANCEL			:
					EndDialog(hDlg,IDCANCEL);
					break;
			}
			break;
	}

	return (0L);
}