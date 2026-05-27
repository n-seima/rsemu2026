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

void
cTOOLWND::UpdateTiles()
{	
	if (!IsCorrectMode(dWORK_TILE))
		return;
	if (!g_lpActmap)
		return;

	DWORD	menu	=	m_ToolButton.Update(Mouse.x,Mouse.y,TOUCHLBUTTON);

	if(TOUCHKEY[0x31]){			// 1
		menu = dTB_SIZE1;
	}else if(TOUCHKEY[0x32]){	// 2
		menu = dTB_SIZE2;
	}else if(TOUCHKEY[0x33]){	// 3
		menu = dTB_SIZE3;
	}else if(TOUCHKEY[0x34]){	// 4
		menu = dTB_SIZE4;
	}else if(TOUCHKEY[0x35]){	// 5
		menu = dTB_SIZE5;
	}else if(TOUCHKEY[0x45]){	// e °
		menu = dTB_TILE_EXPAND;
	}

	switch(menu)
	{
		case		dTB_SIZE1					:
		case		dTB_SIZE2					:
		case		dTB_SIZE3					:
		case		dTB_SIZE4					:
		case		dTB_SIZE5					:
		{
			for (int i = 0;i<5;i++)	m_ToolButton.Check(dTB_SIZE1+i,FALSE);

			g_iBrushSize	=	menu	-	dTB_SIZE1+1;
			m_ToolButton.Check(dTB_SIZE1+g_iBrushSize-1,TRUE);
			MAIN.ChangeMode(dWORK_TILE);
			MAIN.Draw();
			break;
		}

		case		dTB_TILE_EXPAND				:
		{
			g_lpActmap->m_isTileExpand	=	1-g_lpActmap->m_isTileExpand;

			m_ToolButton.Check(dTB_TILE_EXPAND,g_lpActmap->m_isTileExpand);

			MAIN.ChangeMode(dWORK_TILE);
			MAIN.Draw();
			break;
		}

		case		dTB_PAINT					:
		{
			break;
		}

		case		dTB_ROAD					:
		{
			break;
		}
		case		dTB_FREQ1					:
		{
			break;
		}
		case		dTB_FREQ2					:
		{
			break;
		}
		case		dTB_FREQ3					:
		{
			break;
		}
	}

	if (TOUCHLBUTTON){
		
		cRECT_SIZE	rect;

		for (int i=0;i<g_lpActmap->m_iPatternCount;i++)
		{	if (m_iWPOS+(i/3)*36 >= client.y2)	break;

			rect.Set(7+((i%TPL)*68),m_iWPOS+(i/TPL)*36,64,32);
			if (rect.isIN(Mouse.x,Mouse.y))
			{	g_lpActmap->m_iSelectTile	=	i+TilePos*3;
				MAIN.ChangeMode(dWORK_TILE);
				Draw();
				break;
			}
		}

	}else if(TOUCHKEY[0x51]){		// q °.

		if(g_lpActmap->m_iSelectTile+1< g_lpActmap->m_iPatternCount){
			g_lpActmap->m_iSelectTile+=1;
			MAIN.ChangeMode(dWORK_TILE);
			Draw();		
		}

	}else if(TOUCHKEY[dKEY_TAB]){		// °.

		if(g_lpActmap->m_iSelectTile-1>= 0){
			g_lpActmap->m_iSelectTile-=1;
			MAIN.ChangeMode(dWORK_TILE);
			Draw();		
		}
	}
}



void
cTOOLWND::DrawTiles()
{
	if (!IsCorrectMode(dWORK_TILE))
		return;
	if (!g_lpActmap)
		return;

	cDRAW::Fill(0);

//			m_pPattern->GetNormalFlag(m_aTilePattern,m_iPatternCount);
	TPH			=	(cDRAW::HEIGHT - m_iWPOS) / 36;

	SetScrollBar(g_lpActmap->m_iPatternCount / TPL + 1,&TilePos);

	for (int i=0;;i++)
	{	if (m_iWPOS+(i/3)*36	>=	cDRAW::HEIGHT				)	break;
		if (i+TilePos*3			>=	g_lpActmap->m_iPatternCount	)	break;

		int	x	=	7+((i%TPL)*68);
		int	y	=	m_iWPOS+(i/TPL)*36;

		if (i+TilePos*3 == g_lpActmap->m_iSelectTile)
		{
			cDRAW::Fill(_LTYELLOW	,x-2	,y-2	,x+2+63	,y+2+31 );
			cDRAW::Fill(_YELLOW		,x-2	,y+32	,x+2+63	,y+2+31 );
			cDRAW::Fill(_YELLOW		,x+64	,y-2	,x+63	,y+2+31	);
			cDRAW::Fill(_YELLOW		,x+2+62	,y-2	,x+2+63	,y+2+31	);
		}

		LPPattn		lpPattern	=	g_lpActmap->m_pPattern->GetFlagFromPattn(g_lpActmap->m_aTilePattern[i]);
		cDRAW::PutImage(x,y,dTILE_WIDTH,dTILE_HEIGHT,(WORD *)(lpPattern->lpImage));
	}
}
