#include "cMAPWND.H"
#include "cMAIN.H"

//
//	타일 업데이트
void
cMAPWND::UpdateTiles()
{	
	if (g_iWorkMode	!=	dWORK_TILE	)	return;

	if (LBUTTON)
		if (g_lpActmap->m_bf1IsLocked	)
		{
			cMSG::Error("크악!!","맵 그리기 금지 상태여!!");

			return;
		}
		
	if (TOUCHRBUTTON)
	{
		TOUCHRBUTTON	=	FALSE;
		MAIN.ChangeMode(dWORK_EDIT);

		return;
	}
//	if (g_iScale	!=	100)	return;
	if (KEY[dKEY_SHIFT])
	{
		if (TOUCHLBUTTON)
			if (Mouse.y	>	g_iTBSize)
			{
				int x,y;

				x	=	Cursor.x+g_lpActmap->m_pos.x;
				y	=	Cursor.y+g_lpActmap->m_pos.y;

				if (g_iScale<100)
				{
					x	=	Mouse.x*100/g_iScale/dTILE_WIDTH + g_lpActmap->m_pos.x;
					y	=	(Mouse.y-g_iTBSize)*100/g_iScale/dTILE_HEIGHT+ g_lpActmap->m_pos.y;
				}

				x	=	max(0,x);
				y	=	max(0,y);
				x	=	min(x,g_lpActmap->m_iRealWidth-1);
				y	=	min(y,g_lpActmap->m_iRealHeight-1);

				int	left	=	x	-	(g_iBrushSize-1)*dSHARE_TILE;
				int	right	=	x	+	(g_iBrushSize-1)*dSHARE_TILE;
				int	top		=	y	-	(g_iBrushSize-1)*dSHARE_TILE;
				int	bottom	=	y	+	(g_iBrushSize-1)*dSHARE_TILE;

				int	tile	=	g_lpActmap->m_iSelectTile;

				for (y = top ;y <= bottom; y++)
					if (y >= 0 && y < g_lpActmap->m_iHeight)
						for (x = left ;x <= right; x++)
							if (x >= 0 && x < g_lpActmap->m_iWidth)
								g_lpActmap->BuildCliff(x,y);

				m_isRepaint	=	TRUE;

				g_lpActmap->LostVirgin();
			}
	}
	else
	if (LBUTTON)
	{
		if (Mouse.y	>	g_iTBSize || g_isDrawingMap)
		{
			int x,y;

			x	=	Cursor.x+g_lpActmap->m_pos.x;
			y	=	Cursor.y+g_lpActmap->m_pos.y;

			if (g_iScale<100)
			{
				x	=	Mouse.x*100/g_iScale/dTILE_WIDTH + g_lpActmap->m_pos.x;
				y	=	(Mouse.y-g_iTBSize)*100/g_iScale/dTILE_HEIGHT+ g_lpActmap->m_pos.y;
			}
			
			x	=	max(0,x);
			y	=	max(0,y);
			x	=	min(x,g_lpActmap->m_iRealWidth-1);
			y	=	min(y,g_lpActmap->m_iRealHeight-1);

			int	left	=	x	-	(g_iBrushSize-1)*dSHARE_TILE;
			int	right	=	x	+	(g_iBrushSize-1)*dSHARE_TILE;
			int	top		=	y	-	(g_iBrushSize-1)*dSHARE_TILE;
			int	bottom	=	y	+	(g_iBrushSize-1)*dSHARE_TILE;

			int	tile	=	g_lpActmap->m_iSelectTile;

			for (y = top ;y <= bottom; y++)
				if (y >= 0 && y < g_lpActmap->m_iHeight)
					for (x = left ;x <= right; x++)
						if (x >= 0 && x < g_lpActmap->m_iWidth)
							g_lpActmap->PutTile(x,y,tile,g_lpActmap->m_isTileExpand);

			m_isRepaint	=	TRUE;

			g_lpActmap->LostVirgin();
		}
	}

	if (!Cursor.isSame(&OldCursor)		)	m_isRepaint	=	TRUE;	//	커서가 움직였냐?
}	//	cMAPWND::UpdateTiles()

void
cMAPWND::UpdateCannotMoveTile()
{
	if (g_iWorkMode	!=	dWORK_CANNOTMOVETILE	)	return;

	if (LBUTTON)
		if (g_lpActmap->m_bf1IsLocked	)
		{
			cMSG::Error("크악!!","맵 그리기 금지 상태여!!");
			
			return;
		}

	if (TOUCHRBUTTON)
	{
		TOUCHRBUTTON	=	FALSE;
		g_isCHECK_BLOCK =	FALSE;
		MAIN.ChangeMode(dWORK_TILE);
		
		return;
	}
	//	if (g_iScale	!=	100)	return;
	if (LBUTTON)
	{
		if (Mouse.y	>	g_iTBSize || g_isDrawingMap)
		{
			int x,y;
			
			x	=	Cursor.x+g_lpActmap->m_pos.x;
			y	=	Cursor.y+g_lpActmap->m_pos.y;
			
			if (g_iScale<100)
			{
				x	=	Mouse.x*100/g_iScale/dTILE_WIDTH + g_lpActmap->m_pos.x;
				y	=	(Mouse.y-g_iTBSize)*100/g_iScale/dTILE_HEIGHT+ g_lpActmap->m_pos.y;
			}
			
			x	=	max(0,x);
			y	=	max(0,y);
			x	=	min(x,g_lpActmap->m_iRealWidth-1);
			y	=	min(y,g_lpActmap->m_iRealHeight-1);
			
			int	left	=	x	-	(g_iBrushSize-1)*dSHARE_TILE;
			int	right	=	x	+	(g_iBrushSize-1)*dSHARE_TILE;
			int	top		=	y	-	(g_iBrushSize-1)*dSHARE_TILE;
			int	bottom	=	y	+	(g_iBrushSize-1)*dSHARE_TILE;
			
			int	tile	=	g_lpActmap->m_iSelectTile;
			
			for (y = top ;y <= bottom; y++)
				if (y >= 0 && y < g_lpActmap->m_iHeight)
					for (x = left ;x <= right; x++)
						if (x >= 0 && x < g_lpActmap->m_iWidth)
						{
							if (KEY[dKEY_SHIFT])
								g_lpActmap->setCrash(x , y , 0);			//이동 가능 하게 한다.
							else
								g_lpActmap->setCrash(x , y , 1);			//이동 불가능 하게 한다.
						}
						
						m_isRepaint	=	TRUE;
		}
	}
	
	if (!Cursor.isSame(&OldCursor)		)	m_isRepaint	=	TRUE;	//	커서가 움직였냐?
}