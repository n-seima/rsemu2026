#include "cMAPWND.H"
#include "cMAIN.H"

void
cMAPWND::UpdateAreas()
{
	if (g_iWorkMode	!=	dWORK_AREA)
		return;

	if (!client.isIN(Mouse.x,Mouse.y))
		return;

	int x=Mouse.x+g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
	int y=Mouse.y+g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE-g_iTBSize;

	x	=	Mouse.x*100/g_iScale+g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
	y	=	(Mouse.y-g_iTBSize)*100/g_iScale+g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE;


	if (KEY[dKEY_CONTROL])
	{
		if	(TOUCHLBUTTON)	
			g_lpActmap->m_pArea->Select(x,y);
		
		return;
	}

	if (LBUTTON)
	{
		if(!g_lpSelectArea)
		{
			if (g_lpActmap->m_pArea->Select(x,y)  && KEY[dKEY_SHIFT] == FALSE)
			{
				g_lpActmap->m_pArea->Update(x,y,g_iScale,g_iTBSize);
				m_isRepaint	=	TRUE;
			}
			return;
		}
	}

	if (TOUCHRBUTTON)
	{
		if (g_lpActmap->m_pArea->GetFocusArea(x,y)	!=	0xffff)
		{
			if (g_lpActmap->m_pArea->Select(x,y))
			{	g_lpActmap->m_pArea->Update(x,y,g_iScale,g_iTBSize);
				g_MAPWND.Draw();
			}

			PopupArea();
			return;
		}
		else
		{
			TOUCHRBUTTON	=	FALSE;
			MAIN.ChangeMode(dWORK_EDIT);
		}
	}

	if(TOUCHKEY[0x43] && g_lpSelectArea){		// 09.08.03 
		
		g_iWorkMode = dWORK_COPYORCUT;	
		g_iPreWorkMode = dWORK_AREA;
		m_bObjectCopy = TRUE;
		m_bObjectArea = TRUE;
	}

	if (!bLININGBOX)
	{
		m_isRepaint	=	g_lpActmap->m_pArea->Update(x,y,g_iScale,g_iTBSize);

		if (LBUTTON)
			if (!m_isRepaint)
				g_lpActmap->m_pArea->Select(x,y);
	}

//	UpdateEdit();
}

//
//	영역 관련 팝업 메뉴 띄움
void
cMAPWND::PopupArea()
{
	HMENU	hMenu	=	CreatePopupMenu();
	POINT	pos;

	AppendMenu(hMenu,MFT_STRING,IDM_EDIT_AREA	,dMSG_EDIT_AREA);

	if	(!g_lpActmap->m_bf1IsLocked)					// 09.07.24 맵그리기 금지~. 지우기메뉴 보이기/안보이기
		AppendMenu(hMenu,MFT_STRING,IDM_REMOVE_AREA	,dMSG_REMOVE_AREA);

	GetCursorPos(&pos);
	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,pos.x,pos.y,0,hWND,NULL);
	DestroyMenu(hMenu);
}	//	cMAPWND::PopupArea()