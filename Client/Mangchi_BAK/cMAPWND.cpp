#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cMAPWND.H"
#include	"cMAIN.H"
#include	"cOBJECT.H"
#include	"cBUILDING.H"
#include	"cAREA.H"
#include	"cTOOLWND.H"
#include	"cMESSAGE.H"
#include	"cPROJECT.H"
#include	"cSET.H"
#include	"cJOB.H"
#include	"patrolRoad.h"
#include	"cSetDialogNpcData.h"
#include	"CSetDialogMonster.h"


cMAPWND		g_MAPWND;
cPOINT		g_posDisplayMax;
BOOL		bEDITAREA		=	FALSE;
BOOL		g_isDrawingMap	=	FALSE;
BOOL		g_iBrushSize	=	1;
int			g_iScale		=	100;
int			g_iTBSize;
cRECT		g_rectMapwnd;
cPOINT		g_posMapwnd;

cBARMENU	l_Close;

void
cMAPWND::ReBuildAll()	//	오브젝트와 건물을 다시 찍어줌
{
//	g_lpActmap->ResetBlockInfo();
//	g_lpActmap->m_object.ReBuildAll();
//	cBUILDING::ReBuildAll();
}

cMAPWND::cMAPWND()
{
	m_hMenu				=	NULL;
	g_iTBSize			=	28;
	m_isRepaint			=	TRUE;
	LineBuffer[0]		=	NULL;
	LineBuffer[1]		=	NULL;
	LineBuffer[2]		=	NULL;
	LineBuffer[3]		=	NULL;
	m_iCopyCount		=	0;
	m_bObjectCopy		=	FALSE;				// 09.07.09.
	m_bObjectCut		=	FALSE;				// 09.07.09.
	m_bLineObjectCreate	=	FALSE;				// 09.07.13.
	m_bSnapLine			=	FALSE;				// 09.07.14
	m_pSelectData		=	NULL;				// 09.07.29
	m_bObjectArea		=	FALSE;				// 09.08.03

	m_bAdditionPrintArea	=	FALSE;			// insu add
	m_bAdditionPrintActor	=	FALSE;			// insu add
}	

cMAPWND::~cMAPWND()
{
}

BOOL
cMAPWND::Init(HINSTANCE hinst,HWND hwnd,cRECT_SIZE *rect,int xs,int ys)
{
	if (!cDIBWND::Init(0,"MAPWINDOW","MAPWINDOW",
					WS_CHILD|WS_HSCROLL|WS_VSCROLL,
					rect->x,rect->y,rect->xs,rect->ys,
					hwnd,NULL,hinst, 0L,
					SW_SHOW,(WNDPROC)WNDProc
					)
		)
		return FALSE;

	Draw();

	l_Close.Init();

	l_Close.Add("close",0,10,4,40,20);	
	l_Close.InitColor(RGB(255,255,0),RGB(255,255,0),RGB(255,255,0),RGB(255,255,0),0,_GRAY,_GRAY,_GRAY);

	return TRUE;
}
int cMAPWND::getScale(){

	return g_iScale;
}
BOOL
cMAPWND::UpdateToolBar()
{
	DWORD	menu	=	g_bmMapList.Update(Mouse.x,Mouse.y,TOUCHLBUTTON);

	if	(menu	!=	0xffffffff)
	{
		if(g_isCHECK_BLOCK)
		{
			g_isCHECK_BLOCK = FALSE;
			TOOLWND.ChangeMode(dWORK_TILE);	
		}
		// 10.01.12
		cMemoWindow::SaveText();
		if(cMAP::Select((short)menu) == TRUE){
			if(MAIN.m_pMinimapWnd &&IsWindow(MAIN.m_pMinimapWnd->hWND) ){
				MAIN.CreateMiniMap();
			}
		}
		MAIN.Draw();
		TOOLWND.ChangeMode(g_iWorkMode);
		TOOLWND.Draw();
		// 10.01.12
		cMemoWindow::SetMemo(&g_lpActmap->m_sMemoInfo);
		return TRUE;
	}

	menu	=	l_Close.Update(Mouse.x,Mouse.y,TOUCHLBUTTON);

	if (menu	==	0)
	{
		l_Close.Check(0,FALSE);

		int	count	=	0;

		for (int i =0;i<dMAX_MAP;i++)
			if (g_aMap[i].m_iSerial	!=	0xffff)	count++;

		if (count	<=	1)
			return ERRMSG(dMSG_CLOSE_MAP_ERROR,dMSG_DONT_CLOSE_LAST_MAP);

		if (!g_lpActmap->IsVirgin())
		{
			switch(cMSG::YESNOCANCEL(dMSG_CLOSE_MAP_ERROR,dMSG_DO_YOU_WANT_A_SAVE))
			{
				case	IDYES	:
				{
					if (!cPROJECT::SaveMap(g_lpActmap->m_strFName)) return TRUE;
					break;
				}

				case	IDNO		:	break;
				case	IDCANCEL	:	return TRUE;
			}
		}

		cMAP::Close(g_iSelectMap);
		cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
		CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
		CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
		// 10.01.12
		cMemoWindow::SetMemo(&g_lpActmap->m_sMemoInfo);
		// 10.01.15
		cSET_AREA_EDIT::InitAreaDataBox();
		cSET_AREA_EDIT::ResetAreData();
		if(MAIN.m_pMinimapWnd){
			DestroyWindow(MAIN.m_pMinimapWnd->hWND);
			delete MAIN.m_pMinimapWnd;
			MAIN.m_pMinimapWnd = NULL;
			MAIN.CreateMiniMap();
		}
		MAIN.Draw();
		//	만약에 하나도 없으면 클로즈 못함
		//	세이브 안했음 세이브
	}

	return FALSE;
}

//
//	업데이트
//
void
cMAPWND::Run()
{
if (!g_lpActmap)
		g_lpActmap	=	&g_aMap[g_iSelectMap];

	m_isRepaint	=	FALSE;
	bDRAWCURSOR	=	FALSE;
	Mouse.x		=	MOUSE.x	-	outFrame.x1;
	Mouse.y		=	MOUSE.y	-	outFrame.y1;
	Cursor.x	=	max(Mouse.x / (dTILE_WIDTH / dSHARE_TILE),0);
	Cursor.y	=	max((Mouse.y-g_iTBSize) / (dTILE_HEIGHT / dSHARE_TILE),0);
	Cursor.x	=	min(Cursor.x,g_lpActmap->m_iWidth-1);
	Cursor.y	=	min(Cursor.y,g_lpActmap->m_iHeight-1);
	
	m_posWorld.x	=	Mouse.x+g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
	m_posWorld.y	=	Mouse.y+g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE-g_iTBSize;

	// 휠.. 이동... 누른상태에서 .. 움직..
	if(MBUTTON){
		
		int mx = LASTMOUSE.x - MOUSE.x;
		int my = LASTMOUSE.y - MOUSE.y;
		int posX = g_lpActmap->m_pos.x ;
		int posY = g_lpActmap->m_pos.y ;
		int iMove = (int)(100.0/g_iScale);
		if(mx >3){
			posX += iMove;
		}else if(mx<-3){
			posX -= iMove;
		}
		if(my >3){
			posY += iMove;
		}else if(my<-3){
			posY -= iMove;
		}
		g_lpActmap->setPos(posX, posY);
		
		LASTMOUSE = MOUSE;
		
	}

	if (!LBUTTON		)
		g_isDrawingMap	=	FALSE;

	// 09.12.16 수정 
	if (TOUCHKEY[dKEY_SPACE] )			// 09.08.03  단축키 변경.. 스케일100%만들고 ..alt + w
	{
		if (g_iScale	!=	100)
		{
			
			int	mx	=	g_lpActmap->m_pos.x+Mouse.x*100/g_iScale/dTILE_WIDTH;
			int	my	=	g_lpActmap->m_pos.y+(Mouse.y-g_iTBSize)*100/g_iScale/dTILE_HEIGHT;
			
			TPW	=	WIDTH/(dTILE_WIDTH/dSHARE_TILE)		+1;
			TPH	=	(HEIGHT-g_iTBSize)/(dTILE_HEIGHT/dSHARE_TILE)	+1;
			
			g_lpActmap->setPos(mx-TPW/2,my-TPH/2);
			g_iScale			=	100;
			
		}else{
			
			g_iScale			=	25;
			
		}
		m_isRepaint = TRUE;
	}

	if ((client.isIN(Mouse.x,Mouse.y) && Mouse.y > g_iTBSize)|| g_isDrawingMap)
	{
		if	(LBUTTON		)
		{
			g_isDrawingMap	=	TRUE;

		}

		if	(UpdateToolBar())
			return;

		if	((LBUTTON && !g_lpActmap->m_bf1IsLocked) || !LBUTTON)
		{							// 09.07.24  맵그리기 전부금지.
			UpdateTiles();
			UpdateStaticObjects();
			updateTinyObjects();
			UpdateBuilding();
			UpdateAreas();
			UpdateCharacter();
			UpdateCopyOrCut();		//			09.07.09 오브젝트 복사붙여넣기 

		}
	
		UpdateEdit();
		UpdatePatrolRoad();
		UpdateCannotMoveTile();
	}
	else
	{
		if	(LBUTTON)
		{				// 09.07.30 수정.
			resetCopyOrCut();
			m_isRepaint = TRUE;
		}

		if (UpdateToolBar())
			return;

		UpdateEdit();
	}

	if (!g_isLOCKED && !(KEY[dKEY_ALT] && KEY[0x57]))			// 09.07.31   수정 
	{
		if	(g_iScale ==	100	|| !bLININGBOX)
		{
			cRECT	rectFrame;

			rectFrame.OutFrame(hMASTER);

			if	(MOUSE.x <= rectFrame.x1+5)
				HScroll(SB_LINELEFT,FALSE);
			if	(MOUSE.y <= rectFrame.y1+5)
				VScroll(SB_LINEUP,FALSE);
			if	(MOUSE.x >= rectFrame.x2-5)
				HScroll(SB_LINERIGHT,FALSE);
			if	(MOUSE.y >= rectFrame.y2-5)
				VScroll(SB_LINEDOWN,FALSE);
		}
	}
	if (KEY[dKEY_ALT] && KEY[0x42]) // Alt + b
	{
		DialogBox(MAIN.hINST, "MOVE_FIELD", MAIN.hWND, (DLGPROC)WNDProcMovefield);
	}
	if	(g_iScale ==	100	|| !bLININGBOX)
	{
		if	(!KEY[dKEY_SHIFT])
		{
			if	(KEY[dKEY_UP])	
				VScroll(SB_LINEUP,FALSE);
			if	(KEY[dKEY_LEFT])	
				HScroll(SB_LINELEFT,FALSE);
			if	(KEY[dKEY_RIGHT])
				HScroll(SB_LINERIGHT,FALSE);
			if	(KEY[dKEY_DOWN])	
				VScroll(SB_LINEDOWN,FALSE);
		}
	}

	if	(!g_lpActmap->m_pos.isSame(&OldPos)			)
		m_isRepaint	=	TRUE;	//	맵이 움직였냐?

	OldPos		=	g_lpActmap->m_pos;
	OldCursor	=	Cursor;
	OldMouse	=	Mouse;

	if	(KEY[dKEY_ALT] || CLICKEDKEY[dKEY_ALT]){		// 09.08.03 불필요한 랜더를 줄이기위한.
		m_isRepaint	=	TRUE;
	}

	// NPC
	if (KEY[dKEY_ALT] && TOUCHKEY[0x4E])
	{
		cSetDialogNpcData::Init(hWND);
		cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
		cSetDialogNpcData::ShowDialog();						
	}
	// MONSTER
	if (KEY[dKEY_ALT] && TOUCHKEY[0x4D])
	{
		CSetDialogMonster::Init(hWND);
		CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
		CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
		CSetDialogMonster::ShowDialog();
	}
	if	(m_isRepaint)		
		Draw();
	else
	if	(bDRAWCURSOR)
		DrawCursor();
}	//	cMAPWND::Run()

void 
cMAPWND::VScroll(WORD wScroll,BOOL draw )
{	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;
	GetScrollInfo( hWND, SB_VERT, &si );

	switch( wScroll )
	{	case SB_LINEDOWN :
			g_lpActmap->m_pos.y	+=	dSHARE_TILE*100/g_iScale*2;
			break;

		case SB_LINEUP :
			g_lpActmap->m_pos.y	-=	dSHARE_TILE*100/g_iScale*2;
			break;

		case SB_THUMBTRACK :  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( hWND, SB_VERT, &si );
			g_lpActmap->m_pos.y	=	si.nTrackPos;
			break;

		case SB_PAGEUP		:
			g_lpActmap->m_pos.y	-=	(si.nPage-1);
			break;

		case SB_PAGEDOWN	:
			g_lpActmap->m_pos.y	+=	(si.nPage-1);
			break;

	}

	g_lpActmap->setPos(g_lpActmap->m_pos.x,g_lpActmap->m_pos.y);

	if (si.nPos != g_lpActmap->m_pos.y )
	{
		si.fMask = SIF_POS;
		si.nPos  = g_lpActmap->m_pos.y;
		SetScrollInfo( hWND, SB_VERT, &si, TRUE );
		if (draw)	Draw();
	}
}

void 
cMAPWND::HScroll(WORD wScroll,BOOL draw  )
{	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;
	GetScrollInfo( hWND, SB_HORZ, &si );

	switch( wScroll )
	{	case SB_LINERIGHT	:
			g_lpActmap->m_pos.x	+=	dSHARE_TILE*100/g_iScale;
			break;

		case SB_LINELEFT	:
			g_lpActmap->m_pos.x	-=	dSHARE_TILE*100/g_iScale;
			break;

		case SB_THUMBTRACK :  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( hWND, SB_HORZ, &si );
			g_lpActmap->m_pos.x	=	si.nTrackPos;
			break;

		case SB_PAGELEFT	:
			g_lpActmap->m_pos.x	-=	(si.nPage-1);
			break;

		case SB_PAGERIGHT	:
			g_lpActmap->m_pos.x	+=	(si.nPage-1);
			break;

	}

	g_lpActmap->setPos(g_lpActmap->m_pos.x,g_lpActmap->m_pos.y);

	if (si.nPos != g_lpActmap->m_pos.x )
	{
		si.fMask = SIF_POS;
		si.nPos  = g_lpActmap->m_pos.x;
		SetScrollInfo( hWND, SB_HORZ, &si, TRUE );
		if (draw)	Draw();
	}
}

void
cMAPWND::SetScrollBar()
{	SCROLLINFO si;

	if (g_lpActmap->m_iWidth	- TPW	> 0 )
	{	si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;

		if (TPW%2)	si.nPage	=	TPW;
		else		si.nPage	=	TPW-1;

		si.nMin		=	0;
		si.nMax		=	(g_lpActmap->m_iWidth-1)/dSHARE_TILE*dSHARE_TILE;

		si.nPos		=	g_lpActmap->m_pos.x;
		EnableScrollBar(hWND,SB_HORZ,ESB_ENABLE_BOTH);
		SetScrollInfo(hWND,SB_HORZ,&si,TRUE);
	}
	else 
	{	EnableScrollBar(hWND,SB_HORZ,ESB_DISABLE_BOTH);
		g_lpActmap->m_pos.x		=	0;
	}

	if (g_lpActmap->m_iHeight	- TPH	> 0 )
	{	si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;

		if (TPH%2)	si.nPage	=	TPH;
		else		si.nPage	=	TPH	-	1;

		si.nMin		=	0;
		si.nMax		=	(g_lpActmap->m_iHeight-1)/dSHARE_TILE*dSHARE_TILE;

		si.nPos		=	g_lpActmap->m_pos.y;
		EnableScrollBar(hWND,SB_VERT,ESB_ENABLE_BOTH);
		SetScrollInfo(hWND,SB_VERT,&si,TRUE);
	}
	else 
	{	EnableScrollBar(hWND,SB_VERT,ESB_DISABLE_BOTH);
		g_lpActmap->m_pos.y		=	0;
	}

	g_lpActmap->setPixelPos();
}

void
cMAPWND::DrawToolBar()
{	cDRAW::Fill(_LTGRAY	,0,0 ,cDRAW::WIDTH,g_iTBSize);		//	툴바

	cDRAW::Box(0,0,g_iTBSize,cDRAW::WIDTH,cDRAW::HEIGHT);
	cDRAW::Line(0,0,0,0,cDRAW::HEIGHT);

	g_bmMapList.Draw();
	l_Close.Draw();
}

void
cMAPWND::DrawCursor()
{
	if	(!m_isRepaint)	
		if	(!Active())	
			return;

	if	(KEY[dKEY_ALT]){		
		cDRAW::Box(0xffff,Mouse.x,Mouse.y,Mouse.x+800,Mouse.y+515);
	}
	switch(g_iWorkMode)
	{	
		case	dWORK_TILE		:
		{
			int	x		=	Cursor.x/dSHARE_TILE*dSHARE_TILE*dTILE_REAL_WIDTH;
			int	y		=	Cursor.y/dSHARE_TILE*dSHARE_TILE*dTILE_REAL_HEIGHT+g_iTBSize;

			cDRAW::FillHB(0,x,y,x+dTILE_WIDTH,y+dTILE_HEIGHT);

			int left	=	x	-	(g_iBrushSize-1)*dTILE_WIDTH;
			int	top		=	y	-	(g_iBrushSize-1)*dTILE_HEIGHT;
			int right	=	x	+	g_iBrushSize*dTILE_WIDTH;
			int	bottom	=	y	+	g_iBrushSize*dTILE_HEIGHT;

			for (y=top;y<=bottom;y+=dTILE_HEIGHT)	cDRAW::XLineHB(_WHITE,left,right,y);
			for (x=left;x<=right;x+=dTILE_WIDTH)	cDRAW::YLineHB(_WHITE,top,bottom,x);

//			if (KEY[dKEY_CONTROL])
//			{
//				cDRAW::Box(_LTGREEN,left,top,left+800,top+600);
//				cDRAW::Box(_GREEN,left-1,top-1,left+801,top+601);
//			}
			break;
		}

		case	dWORK_STATIC_OBJECT	:				// 09.07.03 수정.
		{

			if	(g_lpActmap->m_object.m_iFixedObjectImageCount	<=	0)
				break;


			if(m_bLineObjectCreate == TRUE){

				if(m_bSnapLine){	
					DrawLineObjectCursor(m_posLineStart.x, m_posLineStart.y, m_posSnapLineLast.x, m_posSnapLineLast.y);					
				}else{
					DrawLineObjectCursor(m_posLineStart.x, m_posLineStart.y, Cursor.x+g_lpActmap->m_pos.x, Cursor.y+g_lpActmap->m_pos.y);
				}

			}else{				
				
				int	x	=	Cursor.x*dTILE_WIDTH/dSHARE_TILE + dTILE_WIDTH/dSHARE_TILE /2;
				int	y	=	Cursor.y*dTILE_HEIGHT/dSHARE_TILE+ dTILE_HEIGHT/dSHARE_TILE /2+g_iTBSize;								
				
				BOOL	crash	=	g_lpActmap->isCrashFixedObject(g_lpActmap->m_object.m_iSelectFixedObject,Cursor.x+g_lpActmap->m_pos.x,Cursor.y+g_lpActmap->m_pos.y);
				
				g_lpActmap->m_object.putFixedObject(g_lpActmap->m_object.m_iSelectFixedObject,x,y,crash);
			}
			if (g_isCHECK_BLOCK)		//	지나갈수 없는 영역을 표시한다.
			{
				BYTE	*lpCrash	=	g_lpActmap->m_object.getFixedObjectCrashInfo(g_lpActmap->m_object.m_iSelectFixedObject);
				g_lpActmap->PutCrashBlock(Cursor.x,Cursor.y,0,-g_iTBSize,lpCrash,_LTYELLOW);
			}


			break;
		}

		case	dWORK_FREE_OBJECT	:
		{
			int	x	=	Mouse.x,y	=	Mouse.y;

			if (g_lpActmap->m_object.isFixedObject(g_lpActmap->m_object.m_iSelectTinyObject))
			{	
				x	=	Cursor.x*dTILE_WIDTH/dSHARE_TILE + dTILE_WIDTH /dSHARE_TILE /2;
				y	=	Cursor.y*dTILE_HEIGHT/dSHARE_TILE+ dTILE_HEIGHT/dSHARE_TILE /2+g_iTBSize;
			}

			BOOL	crash	=	FALSE;

			if	(g_lpActmap->m_object.m_iTinyObjectImageCount	>	0)
				crash	=	g_lpActmap->m_object.isCrashTinyObject(g_lpActmap->m_object.m_iSelectTinyObject,Mouse.x+g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE,Mouse.y+g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE-g_iTBSize);

			g_lpActmap->m_object.putTinyObject(g_lpActmap->m_object.m_iSelectTinyObject,x,y,crash);
			break;
		}

		case	dWORK_BUILDING		:
		{
			if (TOOLWND.m_isEditBlockOfBuilding)	break;

			int	x	=	Cursor.x*dTILE_WIDTH/dSHARE_TILE + dTILE_WIDTH/dSHARE_TILE/2;
			int	y	=	Cursor.y*dTILE_HEIGHT/dSHARE_TILE+ dTILE_HEIGHT/dSHARE_TILE/2+g_iTBSize;

			BOOL	crash	=	g_lpActmap->isCrashBuilding(Cursor.x+g_lpActmap->m_pos.x,Cursor.y+g_lpActmap->m_pos.y,g_lpActmap->m_iSelectBuilding);

			g_lpActmap->putBuilding(g_lpActmap->m_iSelectBuilding,x,y,crash);
			break;
		}

		case	dWORK_CHARACTER		:
		{
			int	x	=	Mouse.x,y	=	Mouse.y;

			if (g_lpActmap->m_setCharacter.m_iSelectCharacter	==	0xffff)	break;

			BOOL	isCrash	=	g_lpActmap->m_setActor.isCrash(m_posWorld.x,m_posWorld.y);

			cCHARACTER	*lpCharacter	=	g_lpActmap->m_setCharacter.getSelectCharacter();		//	선택된 직업을 리턴
			cJOB		*lpJob			=	lpCharacter->getJob();
			int			iBody			=	g_lpActmap->m_setCharacter.getSelectCharacterBody();
			cANMQ		*lpBody			=	g_pBody[iBody];

			if (g_aMonsterPalette[iBody].m_wCount)
				memcpy(lpBody->pPLT,&g_aMonsterPalette[iBody].m_pPalette[lpJob->m_wPalette],512);

			if	(isCrash)
				lpBody->PutRed(x,y,4);
			else
				lpBody->Put(x,y,4);

			if	(lpJob->m_wVariation)
			{
				if	(isCrash)
					g_apMonsterVariation[iBody][lpJob->m_wVariation-1]->PutRed(x,y,4);
				else
					g_apMonsterVariation[iBody][lpJob->m_wVariation-1]->Put(x,y,4);
			}

			break;
		}

		case	dWORK_AREA			:
		case	dWORK_EDIT			:
		{	
			
			if(bLININGBOX){	
				
				BufferRect.Move(-g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE,-g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE);

				if (LineBuffer[0])
				{	cDRAW::PutImage(BufferRect.x1,BufferRect.y1,LineBuffer[0]);
					cDRAW::PutImage(BufferRect.x1,BufferRect.y1,LineBuffer[1]);
					cDRAW::PutImage(BufferRect.x1,BufferRect.y2,LineBuffer[2]);
					cDRAW::PutImage(BufferRect.x2,BufferRect.y1,LineBuffer[3]);
				}

				BufferRect		=	LiningBox;
				BufferRect.Move(-g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE,-g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE);
				BufferRect.Refit();

				for (int i=0;i<4;i++)	pKILL(LineBuffer[i]);
				LineBuffer[0]	=	cDRAW::GetImage(BufferRect.x1,BufferRect.y1,BufferRect.width(),1);
				LineBuffer[1]	=	cDRAW::GetImage(BufferRect.x1,BufferRect.y1,1				  ,BufferRect.height());
				LineBuffer[2]	=	cDRAW::GetImage(BufferRect.x1,BufferRect.y2,BufferRect.width(),1);
				LineBuffer[3]	=	cDRAW::GetImage(BufferRect.x2,BufferRect.y1,1				  ,BufferRect.height());
				BufferRect.Move(g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE,g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE);

				cDRAW::Box(_LTGREEN,LiningBox.x1-g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE	,LiningBox.y1-g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE,
									LiningBox.x2-g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE	,LiningBox.y2-g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE);

			}
			
			break;
		}
		case dWORK_COPYORCUT:				// 09.07.09  복사붙여넣기.
			if	(m_bObjectCopy || m_bObjectCut)
				drawSelectDataCursor();

			break;
	}

	if (!m_isRepaint)
	{	
		DrawToolBar();
		Flip();
	}
}

cINDEX	l_aSortObject[dMAX_OBJECT];

void
cMAPWND::drawMapToFile(char *_lpstrFileName,int _iScale)
{
	cRECT	rect;

	rect.Set(0,0,0,0);

	g_lpActmap->select(&rect,&rect);

	int		x,y,i=0;
	WORD	*pScreen;

	if (!g_lpActmap)	g_lpActmap	=	&g_aMap[g_iSelectMap];

	__int64	llBuferSize	=	g_lpActmap->m_iWidth*dTILE_WIDTH;
	llBuferSize	*=	g_lpActmap->m_iHeight*dTILE_HEIGHT;
	llBuferSize	*=	_iScale;
	llBuferSize	/=	100;

	llBuferSize	+=	2;

	if (llBuferSize	>=	0xffffffff)	
	{
		ERRMSG("미니맵 생성 에러!!","너무 크잖아효!!");
		return;
	}

	pScreen		=	new WORD [(int)llBuferSize];

	pScreen[0]		=	g_lpActmap->m_iWidth*dTILE_WIDTH*_iScale/100;
	pScreen[1]		=	g_lpActmap->m_iHeight*dTILE_HEIGHT*_iScale/100;

	cDRAW::Active(pScreen+2,g_lpActmap->m_iWidth*dTILE_WIDTH*_iScale/100,g_lpActmap->m_iHeight*dTILE_HEIGHT*_iScale/100);

	g_rectMapwnd.x1	=	0;
	g_rectMapwnd.y1	=	0;
	g_rectMapwnd.x2	=	g_lpActmap->m_iWidth*dTILE_WIDTH/dSHARE_TILE;
	g_rectMapwnd.y2	=	g_lpActmap->m_iHeight*dTILE_WIDTH/dSHARE_TILE;

	g_lpActmap->drawWholeImage(_iScale);		//	일단 타일을 다 그려준다.

	//	바닥에 깔리는 오브젝트를 그냥-_- 그려준다.
	int	objectCount	=	0,iIobjectIndex	=	0;

	g_lpActmap->m_object.drawAllRapeObjectScale(&g_rectMapwnd,0,0,_iScale);

	objectCount	+=	g_lpActmap->m_object.getStandObjects(l_aSortObject);				//	뷰포트 안에 오브젝트들을 구한다.

	qsort((void *)l_aSortObject,objectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

	l_aSortObject[objectCount].Value	=	0x7fffffff;

	for (y=0;y<g_lpActmap->m_iHeight;y++)
	{
		int y_border	=	y*dTILE_REAL_HEIGHT+dTILE_REAL_HEIGHT/2;

		while(l_aSortObject[iIobjectIndex].Value	< y_border)
		{
			int	index	=	l_aSortObject[iIobjectIndex].Index;

			g_lpActmap->m_object.putScale(0,0,index,_iScale);

			iIobjectIndex++;
		}

		for (x=0;x<g_lpActmap->m_iWidth;x++)
		{
			int ty		=	y*dTILE_HEIGHT/dSHARE_TILE+dTILE_HEIGHT/2/dSHARE_TILE;
			int	tx		=	x*dTILE_WIDTH/dSHARE_TILE+dTILE_WIDTH/2/dSHARE_TILE;
			tx			=	tx*_iScale/100;
			ty			=	ty*_iScale/100;

			int object	=	g_lpActmap->getObject(x,y);

			if (object	!=	0xffff)
			{	
				cFIXED_OBJECT_INFO	*lpObject	=	g_lpActmap->m_object.getFixedObject(object);

				g_lpActmap->m_object.putFixedObjectScale(lpObject,tx,ty,_iScale);
			}

			int	iBuilding	=	g_lpActmap->getBuilding(x,y);

			if (iBuilding	!=	0xff)	g_lpActmap->putRegisteredBuildingScale(iBuilding,tx,ty,_iScale);
		}
	}

	cTGA::Save(_lpstrFileName,pScreen,TRUE);

	//	떠있는 오브젝트를 그냥-_- 그려준다.
	pKILL(pScreen);
//	if (isDrawObject)	g_lpActmap->m_object.drawAllFloatObject(&g_rectMapwnd,g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize);
}

void
cMAPWND::DrawScaledScreen()
{
	int		x,y,i=0;

	if (!Active())		return;
	if (!g_lpActmap)	g_lpActmap	=	&g_aMap[g_iSelectMap];

	int	iVPP	=	g_lpActmap->m_pos.y;

	VScroll(SB_LINEUP,FALSE);
	VScroll(SB_LINEDOWN,FALSE);

	g_lpActmap->m_pos.y	=	iVPP;

	SetScrollBar();

	g_bmMapList.SetVirgin(g_iSelectMap,g_lpActmap->IsVirgin());		//	툴바 설정

	m_isRepaint		=	TRUE;

	BOOL	isDrawBlock			=	g_isCHECK_BLOCK;
	BOOL	isDrawBuilding		=	g_bIsOnBuildingLayer;
	BOOL	isDrawObject		=	g_bIsOnObjectLayer;
	BOOL	isDrawArea			=	g_bIsOnAreaLayer;
	BOOL	isDrawUnit			=	g_bIsOnUnitLayer;
	BOOL	isDrawDoor			=	g_bIsOnDoorLayer;

	isDrawArea	+=	isDrawDoor;

	if(IsCorrectMode(dWORK_CHARACTER))		isDrawUnit		=	TRUE;
	if(IsCorrectMode(dWORK_AREA))			isDrawArea		=	TRUE;
	if(IsCorrectMode(dWORK_BUILDING))		isDrawBuilding	=	TRUE;
	if(IsCorrectMode(dWORK_STATIC_OBJECT))	isDrawObject	=	TRUE;
	if(IsCorrectMode(dWORK_FREE_OBJECT))	isDrawObject	=	TRUE;

	TPW	=	WIDTH*100*dSHARE_TILE/(dTILE_WIDTH*g_iScale)			-1;
	TPH	=	(HEIGHT-g_iTBSize)*100*dSHARE_TILE/(dTILE_HEIGHT*g_iScale)	-1;

	g_rectMapwnd.x1	=	g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE	-	200;
	g_rectMapwnd.y1	=	g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE-	200;
	g_rectMapwnd.x2	=	g_rectMapwnd.x1+WIDTH*100/g_iScale				+	200;
	g_rectMapwnd.y2	=	g_rectMapwnd.y1+HEIGHT*100/g_iScale			+	200;

	g_posMapwnd.x	=	g_lpActmap->m_pos.x * dTILE_WIDTH/dSHARE_TILE;
	g_posMapwnd.y	=	g_lpActmap->m_pos.y * dTILE_HEIGHT/dSHARE_TILE;

	SetScrollBar();

	g_lpActmap->draw(g_iScale);		//	일단 타일을 다 그려준다.

	//	바닥에 깔리는 오브젝트를 그냥-_- 그려준다.
	int	objectCount	=	0,iIobjectIndex	=	0;

	if (isDrawObject)	g_lpActmap->m_object.drawAllRapeObjectScale(&g_rectMapwnd,g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y,g_iTBSize,g_iScale);

	objectCount	+=	g_lpActmap->m_object.getStandObjects(l_aSortObject,&g_rectMapwnd);				//	뷰포트 안에 오브젝트들을 구한다.
	objectCount	+=	g_lpActmap->m_setActor.getActors(&l_aSortObject[objectCount],&g_rectMapwnd);	//	뷰포트 안에 캐릭터들을 구한다.

	qsort((void *)l_aSortObject,objectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

	l_aSortObject[objectCount].Value	=	0x7fffffff;

	for (y=max(g_lpActmap->m_pos.y-40*dSHARE_TILE,0);y<min(TPH+g_lpActmap->m_pos.y+40*dSHARE_TILE,g_lpActmap->m_iHeight);y++)
	{
		int y_border	=	y*dTILE_REAL_HEIGHT+dTILE_REAL_HEIGHT/2;

		while(l_aSortObject[iIobjectIndex].Value	< y_border)
		{
			int	index	=	l_aSortObject[iIobjectIndex].Index;

			if	((index	<	dBORDER_ACTOR) &&  isDrawObject)
			{
				g_lpActmap->m_object.putScale(g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y,(int)index,g_iTBSize,g_iScale);
			}

			iIobjectIndex++;
		}

		for (x=max(g_lpActmap->m_pos.x-10*dSHARE_TILE,0);x<TPW+g_lpActmap->m_pos.x+10*dSHARE_TILE;x++)
		{	if (x < g_lpActmap->m_iWidth)
			{
				int ty		=	g_iTBSize+(y-g_lpActmap->m_pos.y)*dTILE_HEIGHT*g_iScale/100/dSHARE_TILE+dTILE_HEIGHT*g_iScale/100/2/dSHARE_TILE;
				int	tx		=	(x-g_lpActmap->m_pos.x)*dTILE_WIDTH*g_iScale/100/dSHARE_TILE+dTILE_WIDTH*g_iScale/100/2/dSHARE_TILE;

				int object	=	g_lpActmap->getObject(x,y);

				if (object	!=	0xffff	&&	isDrawObject)
				{	
					cFIXED_OBJECT_INFO	*lpObject	=	g_lpActmap->m_object.getFixedObject(object);

					g_lpActmap->m_object.putFixedObjectScale(lpObject,tx,ty,g_iTBSize,g_iScale);
				}

				int	iBuilding	=	g_lpActmap->getBuilding(x,y);

				if (iBuilding	!=	0xff	&&	isDrawBuilding)
					g_lpActmap->putRegisteredBuildingScale(iBuilding,tx,ty,g_iScale);
			}
			else
				x	=	TPW+g_lpActmap->m_pos.x + 10*dSHARE_TILE;
		}
	}

	iIobjectIndex	=	0;
	for (y=max(g_lpActmap->m_pos.y-40*dSHARE_TILE,0);y<min(TPH+g_lpActmap->m_pos.y+40*dSHARE_TILE,g_lpActmap->m_iHeight);y++)
	{
		int y_border	=	y*dTILE_REAL_HEIGHT+dTILE_REAL_HEIGHT/2;

		while(l_aSortObject[iIobjectIndex].Value	< y_border)
		{
			int	index	=	l_aSortObject[iIobjectIndex].Index;

			if(index	>=	dBORDER_ACTOR)
			{
				if (isDrawUnit)
				{
					index	-=	dBORDER_ACTOR;
					g_lpActmap->m_setActor.m_aActor[index].putScale(g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y,g_iTBSize,g_iScale);
				}
			}

			iIobjectIndex++;
		}
	}

	//	떠있는 오브젝트를 그냥-_- 그려준다.

	if (isDrawObject)	g_lpActmap->m_object.drawAllFloatObject(&g_rectMapwnd,g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize);

	for (y=g_lpActmap->m_pos.y;y<TPH+g_lpActmap->m_pos.y;y++)
	{	if (y >= g_lpActmap->m_iHeight)	break;

		for (x=g_lpActmap->m_pos.x;x<TPW+g_lpActmap->m_pos.x;x++)
		{	if (x < g_lpActmap->m_iWidth)
			{
				if (isDrawBlock)		//	지나갈수 없는 영역을 표시한다.
				{
					int	xp	=	(x-g_lpActmap->m_pos.x)*dTILE_WIDTH*g_iScale/100/dSHARE_TILE;
					int	yp	=	g_iTBSize+(y-g_lpActmap->m_pos.y)*dTILE_HEIGHT*g_iScale/100/dSHARE_TILE;

					if (g_lpActmap->getCrash(x,y) != 0)
					{
						cDRAW::FillHB(_RED,xp,yp,xp+dTILE_WIDTH*g_iScale/100/dSHARE_TILE-1,yp+dTILE_HEIGHT*g_iScale/100/dSHARE_TILE-1);
					}
					else
					{
						if(g_lpActmap->isBlockedTile(x,y)) 
							cDRAW::FillHB(_RED,xp,yp,xp+dTILE_WIDTH*g_iScale/100/dSHARE_TILE-1,yp+dTILE_HEIGHT*g_iScale/100/dSHARE_TILE-1);
					}
				}
			}	else	x	=	TPW+g_lpActmap->m_pos.x;
		}
	}

	if (g_isGRID)
	{
		int	iWidth	=	dTILE_WIDTH*g_iScale/100;
		int	iHeight	=	dTILE_HEIGHT*g_iScale/100;

		if (iWidth*100/g_iScale	!=	dTILE_WIDTH)	iWidth++;
		if (iHeight*100/g_iScale!=	dTILE_HEIGHT)	iHeight++;

		for (y	=	g_lpActmap->m_pos.y/dSHARE_TILE;y<TPH/dSHARE_TILE+2+g_lpActmap->m_pos.y;y++)
		{
			int		yp=g_iTBSize+(y-g_lpActmap->m_pos.y/dSHARE_TILE)*dTILE_HEIGHT*g_iScale/100;

			if (y < g_lpActmap->m_iRealHeight)
			{
				cDRAW::XLine(0,0,cDRAW::WIDTH,yp);

				if (y%10 == 0)
					cDRAW::FillHB(0,0,yp,cDRAW::WIDTH,yp+iHeight-1);
			}
		}

		for (x=g_lpActmap->m_pos.x/dSHARE_TILE;x<TPW/dSHARE_TILE+2+g_lpActmap->m_pos.x;x++)
		{
			int		xp=(x-g_lpActmap->m_pos.x/dSHARE_TILE)*dTILE_WIDTH*g_iScale/100;

			if (x < g_lpActmap->m_iRealWidth)
			{
				cDRAW::YLine(0,0,cDRAW::HEIGHT,xp);

				if (x%10 == 0)
					cDRAW::FillHB(0,xp,0,xp+iWidth-1,cDRAW::HEIGHT);
			}
		}
	}
	if	(KEY[dKEY_ALT]){		
		cDRAW::Box(0xffff,Mouse.x,Mouse.y,Mouse.x + ((800) * g_iScale/100),Mouse.y +((515) * g_iScale/100));
	}
	if (g_iWorkMode	==	dWORK_AREA || isDrawArea)
	{
		BOOL	bIsDrawOnlyDoor	=	FALSE;

		if	(g_bIsOnAreaLayer	==	FALSE	&&	g_bIsOnDoorLayer	==	TRUE)
			bIsDrawOnlyDoor	=	TRUE;

		g_lpActmap->m_pArea->DrawAllByScale(g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y,g_iTBSize,g_iScale,bIsDrawOnlyDoor);
	}

	if (g_iWorkMode	==	dWORK_ROAD)
		g_editRoad.draw(TRUE);

	g_lpActmap->m_prManager.draw();

	DrawToolBar();

	Flip();
}

void
cMAPWND::Draw()
{
	if	(g_iScale	<	100)
	{
		DrawScaledScreen();
		return;
	}

	int		x,y,i=0;

	if	(!Active())
		return;
	if	(!g_lpActmap)
		g_lpActmap	=	&g_aMap[g_iSelectMap];
	
	g_bmMapList.SetVirgin(g_iSelectMap,g_lpActmap->IsVirgin());		//	툴바 설정

	m_isRepaint		=	TRUE;

	BOOL	isDrawBlock			=	g_isCHECK_BLOCK;
	BOOL	isDrawBuilding		=	g_bIsOnBuildingLayer;
	BOOL	isDrawObject		=	g_bIsOnObjectLayer;
	BOOL	isDrawArea			=	g_bIsOnAreaLayer;
	BOOL	isDrawUnit			=	g_bIsOnUnitLayer;
	BOOL	isDrawDoor			=	g_bIsOnDoorLayer;
	
	isDrawArea	+=	isDrawDoor;


	TPW	=	WIDTH/(dTILE_WIDTH/dSHARE_TILE)				;//+1;
	TPH	=	(HEIGHT-g_iTBSize)/(dTILE_HEIGHT/dSHARE_TILE)	;//+1;

	g_rectMapwnd.x1	=	g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE	-	200;
	g_rectMapwnd.y1	=	g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE-	200;
	g_rectMapwnd.x2	=	g_rectMapwnd.x1+WIDTH					+	200;
	g_rectMapwnd.y2	=	g_rectMapwnd.y1+HEIGHT					+	200;

	g_posMapwnd.x	=	g_lpActmap->m_pos.x * dTILE_WIDTH/dSHARE_TILE;
	g_posMapwnd.y	=	g_lpActmap->m_pos.y * dTILE_HEIGHT/dSHARE_TILE;

	SetScrollBar();

	g_lpActmap->draw();		//	일단 타일을 다 그려준다.

	//	바닥에 깔리는 오브젝트를 그냥-_- 그려준다.
	int	objectCount	=	0,iIobjectIndex	=	0;

	if	(isDrawObject)
		g_lpActmap->m_object.drawAllRapeObject(&g_rectMapwnd,g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize);

	objectCount	+=	g_lpActmap->m_object.getStandObjects(l_aSortObject,&g_rectMapwnd);				//	뷰포트 안에 오브젝트들을 구한다.
	objectCount	+=	g_lpActmap->m_setActor.getActors(&l_aSortObject[objectCount],&g_rectMapwnd);	//	뷰포트 안에 캐릭터들을 구한다.

	qsort((void *)l_aSortObject,objectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);

	l_aSortObject[objectCount].Value	=	0x7fffffff;

	for (y=max(g_lpActmap->m_pos.y-40*dSHARE_TILE,0);y<min(TPH+g_lpActmap->m_pos.y+40*dSHARE_TILE,g_lpActmap->m_iHeight);y++)
	{
		int y_border	=	y*dTILE_REAL_HEIGHT+dTILE_REAL_HEIGHT/2;

		while(l_aSortObject[iIobjectIndex].Value	< y_border)
		{
			int	index	=	l_aSortObject[iIobjectIndex].Index;

			if(index	>=	dBORDER_ACTOR)
			{
				if (isDrawUnit)
				{
					index	-=	dBORDER_ACTOR;
					g_lpActmap->m_setActor.m_aActor[index].put(g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize);
				}
			}
			else
			if (isDrawObject)
				g_lpActmap->m_object.put(g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize,index);

			iIobjectIndex++;
		}

		for (x=max(g_lpActmap->m_pos.x-10*dSHARE_TILE,0);x<TPW+g_lpActmap->m_pos.x+10*dSHARE_TILE;x++)
		{	if (x < g_lpActmap->m_iWidth)
			{
				int ty		=	g_iTBSize+(y-g_lpActmap->m_pos.y)*dTILE_HEIGHT/dSHARE_TILE+dTILE_HEIGHT/2/dSHARE_TILE;
				int	tx		=	(x-g_lpActmap->m_pos.x)*dTILE_WIDTH/dSHARE_TILE+dTILE_WIDTH/2/dSHARE_TILE;

				int object	=	g_lpActmap->getObject(x,y);

				if (object	!=	0xffff	&&	isDrawObject)
				{	
					cFIXED_OBJECT_INFO	*lpObject	=	g_lpActmap->m_object.getFixedObject(object);

					g_lpActmap->m_object.putFixedObject(lpObject,tx,ty);
				}

				int	iBuilding	=	g_lpActmap->getBuilding(x,y);

				if (iBuilding	!=	0xff	&&	isDrawBuilding)
					g_lpActmap->putRegisteredBuilding(iBuilding,tx,ty);
			}
			else
				x	=	TPW+g_lpActmap->m_pos.x + 10*dSHARE_TILE;
		}
	}



//	오브젝트 선택 표시
	{
		m_bIsWantDrawSelectBox	=	TRUE;

		if (isDrawObject)
		{
			g_lpActmap->m_object.drawAllRapeObject(&g_rectMapwnd,g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize);

			for (y=max(g_lpActmap->m_pos.y-40*dSHARE_TILE,0);y<min(TPH+g_lpActmap->m_pos.y+40*dSHARE_TILE,g_lpActmap->m_iHeight);y++)
			{
				int y_border	=	y*dTILE_REAL_HEIGHT+dTILE_REAL_HEIGHT/2;

				while(l_aSortObject[iIobjectIndex].Value	< y_border)
				{
					int	index	=	l_aSortObject[iIobjectIndex].Index;

					if(index	>=	dBORDER_ACTOR)
					{
					}
					else
					if (isDrawObject)
						g_lpActmap->m_object.put(g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize,index);

					iIobjectIndex++;
				}

				for (x=max(g_lpActmap->m_pos.x-10*dSHARE_TILE,0);x<TPW+g_lpActmap->m_pos.x+10*dSHARE_TILE;x++)
				{	if (x < g_lpActmap->m_iWidth)
					{
						int ty		=	g_iTBSize+(y-g_lpActmap->m_pos.y)*dTILE_HEIGHT/dSHARE_TILE+dTILE_HEIGHT/2/dSHARE_TILE;
						int	tx		=	(x-g_lpActmap->m_pos.x)*dTILE_WIDTH/dSHARE_TILE+dTILE_WIDTH/2/dSHARE_TILE;

						int object	=	g_lpActmap->getObject(x,y);

						if (object	!=	0xffff	&&	isDrawObject)
						{	
							cFIXED_OBJECT_INFO	*lpObject	=	g_lpActmap->m_object.getFixedObject(object);

							if (g_lpActmap->m_object.isDoorObject(lpObject->m_wObject))
							{
								cDRAW::Fill(0,tx+1-14,ty,tx+30-16,ty+12);
								// 09.12.18
								cTEXT::cPut(tx+1,ty+1,cSET_FONT::s_OldColorOption[eCOLOR_DOOR],"DOOR");
								//cTEXT::cPut(tx+1,ty+1,WHITE,"DOOR");
							}

							g_lpActmap->m_object.putFixedObject(lpObject,tx,ty);
						}
					}
					else
						x	=	TPW+g_lpActmap->m_pos.x + 10*dSHARE_TILE;
				}
			}
		}
		m_bIsWantDrawSelectBox	=	FALSE;
	}

	//JBC	참고	08-09-10
	//	떠있는 오브젝트를 그냥-_- 그려준다.

	if	(isDrawObject)
		g_lpActmap->m_object.drawAllFloatObject(&g_rectMapwnd,g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize);

	for (y=g_lpActmap->m_pos.y;y<TPH+g_lpActmap->m_pos.y;y++)
	{	if (y >= g_lpActmap->m_iHeight)	break;

		for (x=g_lpActmap->m_pos.x;x<TPW+g_lpActmap->m_pos.x;x++)
		{	if (x < g_lpActmap->m_iWidth)
			{
				if (isDrawBlock)		//	지나갈수 없는 영역을 표시한다.
				{
					int		xp			=	(x-g_lpActmap->m_pos.x)*dTILE_WIDTH/dSHARE_TILE;
					int		yp			=	g_iTBSize+(y-g_lpActmap->m_pos.y)*dTILE_HEIGHT/dSHARE_TILE;

					int		iCrashValue					=	g_lpActmap->getCrash(x,y);
					BOOL	bIsMissileBlockedObjectTile	=	g_lpActmap->isMisslieBlockedObjectTile(x,y);
					BOOL	bIsBlockedTile				=	g_lpActmap->isMissileBlockedTile(x,y);
					BOOL	bIsCrashedTile				=	g_lpActmap->isBlockedTile(x,y);

/*					if ((iCrashValue==1 || bIsCrashedTile) && (bIsBlockedTile || iCrashValue >= 2))
					{
						cDRAW::FillHB(_PURPLE,xp,yp,xp+dTILE_WIDTH/dSHARE_TILE-1,yp+dTILE_HEIGHT/dSHARE_TILE-1);
					}
					else*/
					if (bIsBlockedTile)
					{
						cDRAW::FillHB(_BLUE,xp,yp,xp+dTILE_WIDTH/dSHARE_TILE-1,yp+dTILE_HEIGHT/dSHARE_TILE-1);
					}
					else
					if (iCrashValue)
					{
						WORD	wFillColor	=	_RED;

						if (bIsMissileBlockedObjectTile)	
							wFillColor	=	_BLUE;

						cDRAW::FillHB(wFillColor,xp,yp,xp+dTILE_WIDTH/dSHARE_TILE-1,yp+dTILE_HEIGHT/dSHARE_TILE-1);
					}
					else
					{
						if(g_lpActmap->isBlockedTile(x,y)) 
							cDRAW::FillHB(_RED,xp,yp,xp+dTILE_WIDTH/dSHARE_TILE-1,yp+dTILE_HEIGHT/dSHARE_TILE-1);

					}
				}

				if (g_isGRID)			//	격자를 표시한다.
				{
					int		xp	=	(x-g_lpActmap->m_pos.x)*dTILE_WIDTH/dSHARE_TILE;
					int		yp	=	g_iTBSize+(y-g_lpActmap->m_pos.y)*dTILE_HEIGHT/dSHARE_TILE;

					if (x%10	==	0	||	y	%	10	==	0)
							cDRAW::FillHB(0,xp+1,yp+1,xp+dTILE_WIDTH/dSHARE_TILE,yp+dTILE_HEIGHT/dSHARE_TILE);
//					else	cDRAW::Box(_WHITE,xp,yp,xp+dTILE_WIDTH/dSHARE_TILE,yp+dTILE_HEIGHT/dSHARE_TILE);
					
					// 09.12.18
					//cTEXT::Put(xp+1,yp+1,WHITE,_ms("%d,%d",x,y));
					cTEXT::Put(xp+1,yp+1,cSET_FONT::s_OldColorOption[eCOLOR_GIRD],_ms("%d,%d",x,y));
					
				}

			}	else	x	=	TPW+g_lpActmap->m_pos.x;
		}
	}

	iIobjectIndex	=	0;

	if (isDrawUnit)
		for (y=max(g_lpActmap->m_pos.y-40*dSHARE_TILE,0);y<min(TPH+g_lpActmap->m_pos.y+40*dSHARE_TILE,g_lpActmap->m_iHeight);y++)
		{
			int y_border	=	y*dTILE_REAL_HEIGHT+dTILE_REAL_HEIGHT/2;

			while(l_aSortObject[iIobjectIndex].Value	< y_border)
			{
				int	index	=	l_aSortObject[iIobjectIndex].Index;

				if(index	>=	dBORDER_ACTOR)
				{
					index	-=	dBORDER_ACTOR;
					g_lpActmap->m_setActor.m_aActor[index].putName(g_lpActmap->m_posPixel.x,g_lpActmap->m_posPixel.y-g_iTBSize);
				}

				iIobjectIndex++;
			}
		}

	if	(g_lpActmap->m_iHeight	<	TPH)
		cDRAW::Fill(0,0,g_lpActmap->m_iHeight*dTILE_REAL_HEIGHT+g_iTBSize,cDRAW::WIDTH,cDRAW::HEIGHT);
	if	(g_lpActmap->m_iWidth	<	TPW)
		cDRAW::Fill(0,g_lpActmap->m_iWidth*dTILE_REAL_WIDTH,0,cDRAW::WIDTH,cDRAW::HEIGHT);

	BOOL	bIsDrawOnlyDoor	=	FALSE;

	if	(g_bIsOnAreaLayer	==	FALSE	&&	g_bIsOnDoorLayer	==	TRUE)
		bIsDrawOnlyDoor	=	TRUE;

	if (g_iWorkMode	==	dWORK_AREA)
		g_lpActmap->m_pArea->DrawAll(-g_lpActmap->m_pos.x * dTILE_WIDTH/dSHARE_TILE,-g_lpActmap->m_pos.y * dTILE_HEIGHT/dSHARE_TILE+g_iTBSize,TRUE,TRUE,bIsDrawOnlyDoor);
	else
	{
		if (isDrawArea)
			g_lpActmap->m_pArea->DrawAll(-g_lpActmap->m_pos.x * dTILE_WIDTH/dSHARE_TILE,-g_lpActmap->m_pos.y * dTILE_HEIGHT/dSHARE_TILE+g_iTBSize,FALSE,FALSE,bIsDrawOnlyDoor);
	}

	if (g_iWorkMode	==	dWORK_ROAD)
		g_editRoad.draw(TRUE);

	g_lpActmap->m_prManager.draw();

	DrawCursor();
	DrawToolBar();

	int	yp	=	g_iTBSize+1;

	cDRAW::FillHB(0,0,yp,300,yp+76+50);
	yp+=4;
	cTEXT::Put(2,yp,cSET_FONT::s_ColorOption[eCOLOR_FILENAME],_ms("LastSaveIP [%s]",g_lpActmap->getLastSaveIP()));
	// 09.12.18
	yp+=14;
	cTEXT::Put(2,yp,cSET_FONT::s_ColorOption[eCOLOR_FILENAME],_ms("%s [%dx%d][%d]",g_lpActmap->m_strFName,g_lpActmap->m_iWidth,g_lpActmap->m_iHeight,g_lpActmap->m_object.getFixedObjectCount()));
	//cTEXT::Put(2,yp,LTYELLOW,_ms("%s [%dx%d][%d]",g_lpActmap->m_strFName,g_lpActmap->m_iWidth,g_lpActmap->m_iHeight,g_lpActmap->m_object.getFixedObjectCount()));
	yp+=14;
	cTEXT::Put(2,yp,cSET_FONT::s_ColorOption[eCOLOR_MAPNAME],_ms("%s",g_lpActmap->m_strName));
	//cTEXT::Put(2,yp,LTGREEN,_ms("%s",g_lpActmap->m_strName));
	yp+=14;
	cTEXT::Put(2,yp,cSET_FONT::s_ColorOption[eCOLOR_AREA_COUNT],_ms("%-16s - %d",dMSG_AREA_COUNT,g_lpActmap->m_pArea->m_iCount-2));
	//cTEXT::Put(2,yp,LTYELLOW,_ms("%-16s - %d",dMSG_AREA_COUNT,g_lpActmap->m_pArea->m_iCount-2));
	yp+=14;
	cTEXT::Put(2,yp,cSET_FONT::s_ColorOption[eCOLOR_CHARACTER_COUNT],_ms("%-16s - %d",dMSG_NPC_COUNT,g_lpActmap->m_setActor.m_iCount));
	//cTEXT::Put(2,yp,LTGREEN,_ms("%-16s - %d",dMSG_NPC_COUNT,g_lpActmap->m_setActor.m_iCount));

	yp	+=	14;

	if	(g_isAutoSave == TRUE)
	{
		cTEXT::Put(2,yp,cSET_FONT::s_ColorOption[eCOLOR_SAVE_YESNO],_ms("%-16s","임시파일저장중"));
		//cTEXT::Put(2,yp,LTGREEN,_ms("%-16s","임시파일저장중"));
	}
	else
	{
		cTEXT::Put(2,yp,cSET_FONT::s_ColorOption[eCOLOR_SAVE_YESNO],_ms("%-16s","임시파일저장X"));
		//cTEXT::Put(2,yp,LTGREEN,_ms("%-16s","임시파일저장X"));
	}

	if (g_lpActmap->m_bf1IsLocked)
	{
		yp+=14;
		cTEXT::Put(2,yp,cSET_FONT::s_ColorOption[eCOLOR_MAP_DRAW],"맵 그리기 금지 상태다!!");
		//cTEXT::Put(2,yp,LTRED,"맵 그리기 금지 상태다!!");
	}
	// insu add	
	if(m_bAdditionPrintArea)
	{
		cTEXT::Put(2,yp+14,cSET_FONT::s_ColorOption[eCOLOR_SAVE_YESNO],"미니맵 영역 Show!");
	}
	else
	{
		cTEXT::Put(2,yp+14,cSET_FONT::s_ColorOption[eCOLOR_SAVE_YESNO],"미니맵 영역 Hide!");
	}
	
	if(m_bAdditionPrintActor)
	{
		cTEXT::Put(2,yp+28,cSET_FONT::s_ColorOption[eCOLOR_SAVE_YESNO],"미니맵 액터 Show!");
	}
	else
	{
		cTEXT::Put(2,yp+28,cSET_FONT::s_ColorOption[eCOLOR_SAVE_YESNO],"미니맵 액터 Hide!");
	}
	// insu add end
	if (!g_bIsDisplayTileIndex && !g_isGRID)		
	{
		Flip();
		return;
	}

	for (y=g_lpActmap->m_pos.y;y<TPH+g_lpActmap->m_pos.y;y+=dSHARE_TILE)
	{
		if	(y >= g_lpActmap->m_iHeight)
			break;

		for (x=g_lpActmap->m_pos.x;x<TPW+g_lpActmap->m_pos.x;x+=dSHARE_TILE)
		{	
			if (x < g_lpActmap->m_iWidth)
			{
				int		xp=(x-g_lpActmap->m_pos.x)*dTILE_WIDTH/dSHARE_TILE;
				int		yp=g_iTBSize+(y-g_lpActmap->m_pos.y)*dTILE_HEIGHT/dSHARE_TILE;

				int		iIndex			=	g_lpActmap->m_pPattern->GetMapIndex(x/dSHARE_TILE,y/dSHARE_TILE);
				LPPattn	lpTilePattern	=	g_lpActmap->m_pPattern->GetPattn(iIndex);

				if	(g_bIsDisplayTileIndex)
					cTEXT::cPut(xp+32	,yp+10	,cSET_FONT::s_ColorOption[eCOLOR_TILE_INDEX],_ms("%d",iIndex));
					//cTEXT::cPut(xp+32	,yp+10	,WHITE,_ms("%d",iIndex));
				cDRAW::Box(0,xp,yp,xp+dTILE_WIDTH,yp+dTILE_HEIGHT);
			}
			else
				x	=	TPW+g_lpActmap->m_pos.x;
		}
	}

	Flip();
}

void
cMAPWND::Draw(int x,int y,int xs,int ys)
{	
	if	(!Active())
		return;

	Flip(x,y,xs,ys);
}


void
cMAPWND::UpdateCursorShape()
{
	if (!client.isIN(Mouse.x,Mouse.y) || Mouse.y < g_iTBSize)
	{
		if	(GetCursor()	!=	g_hNormalCursor)
			SetCursor(g_hNormalCursor);

		return;
	}

	if (g_iWorkMode == dWORK_AREA)
	{
		if	(m_hMenu)
		{
			if	(GetCursor()	!=	g_hNormalCursor)
				SetCursor(g_hNormalCursor);
		}
		else
		if	(GetCursor()	!=	g_hAreaCursor)
			SetCursor(g_hAreaCursor);

		return;
	}

	if	(g_iWorkMode == dWORK_EDIT)
	{
		if	(GetCursor()	!=	g_hNormalCursor)
			SetCursor(g_hNormalCursor);

		return;
	}

	if	(GetCursor()	!=	g_hNormalCursor)
		SetCursor(g_hNormalCursor);
}

void
cMAPWND::UpdateCommand(int iMenu)
{
	switch(iMenu)
	{
		case	IDM_EDIT_AREA	:
			cSET_AREA::SetArea(hINST,hWND,NULL,TRUE);
			break;

		case	IDM_REMOVE_AREA	:
			if (cMSG::YESNO(dMSG_REMOVE_SELECT_AREA,dMSG_QUESTION_CONTINUE) == IDNO)	return;

			if (g_lpActmap->m_pArea->Del())
			{	TOOLWND.Draw();
				g_MAPWND.Draw();
			}
			break;

		case	IDM_SELECT_CHARACTER	:
		{
			g_lpActmap->m_setActor.selectAll();
			MAIN.Draw();
			break;
		}
		case	IDM_SELECT_CHARACTER_SAME_KIND	:
		{
			g_lpActmap->m_setActor.selectSameKind();
			MAIN.Draw();
			break;
		}
		case	IDM_PREVIOUS_WORK		:
			MAIN.ChangeMode(g_iPreWorkMode);
			break;

		case	IDM_REMOVE				:
			g_lpActmap->m_object.removeSelectedAll();
			g_lpActmap->removeSelectedBuilding();
			g_lpActmap->m_setActor.removeSelectedActors();
			MAIN.Draw();
			TOOLWND.Draw();
			break;

		case	IDM_EDIT_ACTOR			:
		{
			SettingCharacter();
			Draw();
			break;
		}

		case	IDM_LINK_MONSTER		:
		{
			g_lpActmap->m_setActor.linkSelectedMonster();
			g_lpActmap->LostVirgin();
			Draw();
			break;
		}

		case	IDM_SET_LEADER			:
			SetLeader();
			break;

		case	IDM_SET_WARD			:
			SetWard();
			break;

		case	IDM_CHECK_SHADOW		:
			g_lpActmap->m_object.checkSelectFixedObjectShadow(TRUE);
			g_lpActmap->LostVirgin();
			Draw();
			// insu add
			MAIN.m_bBuffer = TRUE;
			MAIN.undoBufferUpdate();
			// insu add end
			break;

		case	IDM_REMOVE_SHADOW		:
			g_lpActmap->m_object.checkSelectFixedObjectShadow(FALSE);
			g_lpActmap->LostVirgin();
			Draw();
			// insu add
			MAIN.m_bBuffer = TRUE;
			MAIN.undoBufferUpdate();
			// insu add end
			break;

		case	IDM_LINK_TO_EVENT_AREA	:
		{
			int					iObject		=	g_lpActmap->m_object.getSelectedObject(FALSE);			//선택된 오브젝트

			if	(iObject	==	0xffff	||	iObject	<	0x8000)
			{
				cMSG::Put("이벤트 영역과 링크","링크 실패 - 선택된 오브젝트 없음");
				break;
			}

			if	(iObject-0x8000	==	0)
			{
				cMSG::Put("이벤트 영역과 링크","링크 실패 - 선택된 오브젝트 시리얼이 0번입니다.");
				break;
			}

			cFIXED_OBJECT_INFO	*lpObject	=	g_lpActmap->m_object.getFixedObject(iObject-0x8000);

			if	(!lpObject)
			{
				cMSG::Put("이벤트 영역과 링크","링크 실패 - 선택된 오브젝트 없음");
				break;
			}

			int	iLastIndex	=	2;

			while(1)
			{
				int	iArea		=	g_lpActmap->m_pArea->GetFocusArea(lpObject->m_pos.x,lpObject->m_pos.y,iLastIndex);

				iLastIndex		=	iArea+1;

				if	(iArea	==	0xffff)
				{
					cMSG::Put("이벤트 영역과 링크","링크 실패");
					break;
				}

				cAREA_INFO		*lpArea		=	&g_lpActmap->m_pArea->m_aArea[iArea];

				if	(lpArea->isEventArea()	||	lpArea->isGuildCrest())
				{
					cAreaEvent	*lpEventArea	=	(cAreaEvent	*)lpArea;

					lpEventArea->m_wLinkObject	=	iObject-0x8000;

					cMSG::Put("이벤트 영역과 링크","'%s' 영역과 링크 성공",lpEventArea->m_strName);

					MAIN.Draw();

					break;
				}
			}
			break;
		}

		case	IDM_SELECT_SAME_OBJECT_IN_FIELD		:
		{
			cRECT	rect;

			rect.Set(0,0,0x7fffffff,0x7fffffff);
			g_lpActmap->m_object.select(&rect,TRUE);

			Draw();
			break;
		}

		case	IDM_SELECT_SAME_OBJECT_IN_SCREEN	:
		{
			cRECT	screen;
			screen.x1	=	g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
			screen.y1	=	g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE;
			screen.x2	=	screen.x1+WIDTH;
			screen.y2	=	screen.y1+HEIGHT-g_iTBSize;

			g_lpActmap->m_object.select(&screen,TRUE);
			Draw();

			break;
		}

		case	IDM_REMOVE_LINK			:
		{
			g_lpActmap->m_setActor.removeSelectedMonstersLink();
			g_lpActmap->LostVirgin();
			Draw();
			break;
		}
	}
}	//	cMAPWND::UpdateCommand(int iMenu)


LRESULT CALLBACK 
cMAPWND::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hWnd, &ps );

			g_MAPWND.Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hWnd, &ps);
			break;
		}

		case WM_SETCURSOR	:
			g_MAPWND.UpdateCursorShape();
			break;

		case WM_CLOSE		:
		case WM_DESTROY		:
			return (0L);

		case WM_VSCROLL		:
			g_MAPWND.VScroll(wParam);
			break;

		case WM_HSCROLL		:
			g_MAPWND.HScroll(wParam);
			break;

		case WM_COMMAND		:
			g_MAPWND.UpdateCommand(LOWORD( wParam ));
			break;

		default :
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}


LRESULT CALLBACK
cMAPWND::WNDProcMovefield(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rtMain, rtDialog;
	static BOOL bGrid = FALSE;
	int iTileXCnt=0, iTileYCnt=0;
	char str[30];
	int iGetDlgItem = 0;
	memset(str, '\0', 30);	

	switch(uMsg)
	{
		case WM_INITDIALOG:
			// 윈도우 생성 위치 설정
			GetClientRect(MAIN.hWND, &rtMain);
			GetClientRect(hwnd, &rtDialog);
			SetWindowPos(hwnd, HWND_TOP, (rtMain.right-rtDialog.right)/2, (rtMain.bottom-rtDialog.bottom)/2, 0, 0, SWP_NOSIZE);
			bGrid = g_isGRID;
			g_isGRID = TRUE;
			if (!bGrid)
				g_MAPWND.Draw();
			SetFocus(hwnd);
			break;
		case WM_CLOSE		:
		case WM_DESTROY		:
			return (0L);
		case WM_COMMAND		:
			 
			switch( LOWORD( wParam ) )
            {
				case IDOK:
					iGetDlgItem = GetDlgItemInt(hwnd, IDC_POS_X, NULL, FALSE);
					if (iGetDlgItem > g_lpActmap->m_iWidth-1)
					{
						wsprintf(str, "The Area of Map is %d, %d", g_lpActmap->m_iWidth-1, g_lpActmap->m_iHeight-1);
						MessageBox(hwnd, str, "Error!", 0);
						break;
					}					
					g_lpActmap->m_pos.x = iGetDlgItem;
					iGetDlgItem = GetDlgItemInt(hwnd, IDC_POS_Y, NULL, FALSE);
					if (iGetDlgItem > g_lpActmap->m_iHeight-1)
					{
						wsprintf(str, "The Area of Map is %d, %d", g_lpActmap->m_iWidth-1, g_lpActmap->m_iHeight-1);
						MessageBox(hwnd, str, "Error!", 0);
						break;
					}
					g_lpActmap->m_pos.y = iGetDlgItem;

					// 상변 넓이 대략 100 으로 계산
					iTileXCnt = (rtMain.right-rectTOOLWND.xs)/dTILE_WIDTH/2;		// x축에 보일 타일 수/2
					iTileYCnt = (rtMain.bottom-100)/dTILE_HEIGHT/2;		// y축에 보일 타일 수/2				
					if (g_lpActmap->m_pos.x-iTileXCnt >= 0)
						g_lpActmap->m_pos.x -= iTileXCnt;
					if (g_lpActmap->m_pos.y-iTileYCnt >= 0)
						g_lpActmap->m_pos.y -= iTileYCnt/2;
					g_MAPWND.Draw();
					break;
				case IDCANCEL:
					g_isGRID = bGrid;
					EndDialog(hwnd,IDCANCEL);
					g_MAPWND.Draw();
					break;					
			}
			break;
	}
	return (0L);
}
