#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cTOOLWND.H"
#include	"cMAIN.H"
#include	"cOBJECT.H"
#include	"cBUILDING.H"
#include	"cAREA.H"
#include	"cMAPWND.H"
#include "cSetDialogNpcData.h"					//	----------------------------------	추가
#include	"CSetDialogMonster.h"					//	11.20 추가
cTOOLWND		TOOLWND;

//
//	생성자
//
cTOOLWND::cTOOLWND()
{
	TPL			=	3;
	POSITION	=	0;
	ScrollCount	=	0;
	ScrollPos	=	0;

	StaticPos	=	0;
	FreePos		=	0;
	BuildingPos	=	0;
	TilePos		=	0;
	CharacterPos=	0;
	HousePos	=	0;
	AreaPos		=	0;
	TPH			=	0;
	RoadPos		=	0;
	m_iTBSIZE	=	34;
	m_iICONSIZE	=	20;
	m_iWPOS		=	m_iTBSIZE + m_iICONSIZE +4;

	m_abInput[0] = 0;
	m_iInputCnt = 0;
	m_iPreInput = 0;
}	//	cTOOLWND::cTOOLWND()


cTOOLWND::~cTOOLWND()
{
}

//
//	초기화
//
BOOL
cTOOLWND::Init(HINSTANCE hinst,HWND hwnd,cRECT_SIZE *rect)
{
	if (!cDIBWND::Init(0,"TOOLWINDOW","TOOLWINDOW",
					WS_CHILD|WS_VSCROLL,
					rect->x,rect->y,rect->xs,rect->ys,
					hwnd,NULL,hinst, 0L,
					SW_SHOW,(WNDPROC)WNDProc
					)
		)
		return FALSE;

	m_WorkButton.Init(&RESOURCE,80,dBAR_RADIO,TRUE);
	m_ToolButton.Init(&RESOURCE,80,dBAR_CHECK,TRUE);

	for (int i=0;i<7;i++)
	{
		if(i==6)
			m_WorkButton.Add(4+i*27,3,dTB_ROAD,dBI_ROAD,0xffff,0xffff,dBI_ROAD+dBI_ACTIVE_BORDER);
		else
			m_WorkButton.Add(4+i*27,1,dTB_TILE+i,dBI_TILE+i,0xffff,0xffff,dBI_TILE+dBI_ACTIVE_BORDER+i);
	}
	

	ChangeMode(dWORK_TILE);
	
	m_WorkButton.Check(dTB_TILE);
	memset(m_strStaticCtrStr,0, sizeof(m_strStaticCtrStr));
	m_iIndex = 0;
	return TRUE;
}

//
//	작업 모드 전환
//
void
cTOOLWND::ChangeMode(DWORD mode)
{// 마지막 수정일 : 09.10.30
	if (!g_lpActmap)
		return;

	m_ToolButton.Reset();

	int	workmode	=	g_iWorkMode;

	if (workmode	==	dWORK_EDIT)
		workmode	=	g_iPreWorkMode;

	switch(workmode)
	{
		case	dWORK_TILE			:
		{
			int	x	=	6,i;

			for (i=0;i<5;i++,x+=22)
			{
				m_ToolButton.Add(x,33,dTB_SIZE1+i,dBI_SIZE1+i,dBI_SIZE1+i+dBI_ACTIVE_BORDER,0xffff,dBI_SIZE1+i+dBI_ACTIVE_BORDER);
			}
			m_ToolButton.Check(g_iBrushSize + dTB_SIZE1 - 1);

			m_ToolButton.Add(x+10,33,dTB_TILE_EXPAND,dBI_TILE_EXPAND,dBI_TILE_EXPAND+dBI_ACTIVE_BORDER,0xffff,dBI_TILE_EXPAND+dBI_ACTIVE_BORDER);

			if (g_lpActmap)
				if (g_lpActmap->m_isTileExpand)	m_ToolButton.Check(dTB_TILE_EXPAND);

			break;
		}
		case	dWORK_STATIC_OBJECT	:
		{
			break;
		}
		case	dWORK_FREE_OBJECT	:
		{
			break;
		}

		case	dWORK_BUILDING		:
		{
			endEditBlockOfBuilding();
			m_iBuildingBlockPos		=	0;
			break;
		}

		case	dWORK_AREA			:	
		{
			break;
		}

		case	dWORK_CHARACTER		:
		{
			break;
		}

		case	dWORK_EDIT			:
			{
				break;
			}
	}
	
	cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
	CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
	CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
	// 10.01.15
	cSET_AREA_EDIT::InitAreaDataBox();
	cSET_AREA_EDIT::ResetAreData();
}




void
cTOOLWND::SetScrollBar(int count,int *pos)
{	SCROLLINFO si;

	ScrollCount	=	count;
	ScrollPos	=	*pos;

	if (ScrollCount > TPH)
	{	si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	ScrollCount-1;
		si.nPage	=	TPH;
		si.nPos		=	ScrollPos;
		EnableScrollBar(hWND,SB_VERT,ESB_ENABLE_BOTH);
		SetScrollInfo(hWND,SB_VERT,&si,TRUE);

		if (ScrollPos	> ScrollCount - TPH)  ScrollPos	=	ScrollCount - TPH;
	}
	else 
	{	EnableScrollBar(hWND,SB_VERT,ESB_DISABLE_BOTH);
		ScrollPos	=	0;
	}

	*pos	=	ScrollPos;
}

void
cTOOLWND::Run()
{
	if	(!g_lpActmap)
		return;

	Mouse.x	=	MOUSE.x	-	outFrame.x1;
	Mouse.y	=	MOUSE.y	-	outFrame.y1;

	// ┏09.07.09  단축키 기능추가.
	DWORD	menu		= 0xffffffff;
	
	if(TOUCHKEY[dKEY_F1]){
		menu		= dTB_TILE; 
	}else if(TOUCHKEY[dKEY_F2]){
		menu		= dTB_STATIC_OBJECT; 
	}else if(TOUCHKEY[dKEY_F3]){
		menu		= dTB_FREE_OBJECT;
	}else if(TOUCHKEY[dKEY_F4]){
		menu		= dTB_HOUSE;
	}else if(TOUCHKEY[dKEY_F5]){
		menu		= dTB_AREA;
	}else if(TOUCHKEY[dKEY_F6]){
		menu		= dTB_HERO;
	}else if(TOUCHKEY[dKEY_F7]){
		menu		= dTB_ROAD;
	}else if(TOUCHLBUTTON){
		menu	= m_WorkButton.Update(Mouse.x,Mouse.y,TOUCHLBUTTON);
	}

	if (menu!=0xffffffff)
	{
		if(LBUTTON == FALSE){	// 09.07.31 수정.

			m_WorkButton.Check(menu);
			
		}

		if (menu	==	dTB_ROAD){	

			MAIN.ChangeMode(dWORK_ROAD);

		}else{

			if (menu-dTB_TILE == g_iWorkMode)
				return;
			
			g_iPreWorkMode	=	g_iWorkMode;
			g_iWorkMode		=	menu-dTB_TILE;
			
			TOOLWND.ChangeMode(menu-dTB_TILE);
			MAIN.ChangeMode(dWORK_EDIT);
		}		
	}
	SelectObjects();
	UpdateTiles();
	UpdateStaticObjects();
	UpdateFreeObjects();
	UpdateBuildings();
	UpdateAreas();
	UpdateCharacter();
	UpdateRoad();
}
//
//	
//
void
cTOOLWND::DrawNPC()
{
	if (g_iWorkMode	!=	dWORK_CHARACTER	)
		return;
}

//
//	툴바의 내용을 그려준다.
void
cTOOLWND::Draw()
{
	if (!Active())
		return;

	if (!g_lpActmap)
		return;

	client.x2	=	cDRAW::WIDTH;
	client.y2	=	cDRAW::HEIGHT;

	DrawTiles();
	DrawStaticObjects();
	DrawFreeObjects();
	DrawBuildings();
	DrawAreas();
	DrawCharacter();
	DrawRoad();

	cDRAW::Fill(_LTGRAY		,0,0	,cDRAW::WIDTH	,28);
	cDRAW::Line(_DEEPGRAY	,0,28	,cDRAW::WIDTH	,28);
	m_WorkButton.Draw();
	m_ToolButton.Draw();

	cDRAW::Line(_LTGRAY		,0,0	,0				,cDRAW::HEIGHT);

	Flip();
}

void
cTOOLWND::Draw(int x,int y,int xs,int ys)
{
	if (!Active())
		return;

	Flip(x,y,xs,ys);
}

void 
cTOOLWND::VScroll(WORD wScroll)
{
	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;
	GetScrollInfo( hWND, SB_VERT, &si );

	switch( wScroll )
	{	case SB_LINEDOWN	:
			ScrollPos++;
			break;

		case SB_LINEUP		:
			ScrollPos--;
			break;

		case SB_THUMBTRACK	:  // 스크롤바를 잡구 움직였을때..
			GetScrollInfo( hWND, SB_VERT, &si );
			ScrollPos	=	si.nTrackPos;
			break;

		case SB_PAGEUP		:
			ScrollPos	=	ScrollPos- si.nPage+1;
			break;

		case SB_PAGEDOWN	:
			ScrollPos	=	ScrollPos+si.nPage-1;
			break;
	}

	if (ScrollPos	< 0								)	ScrollPos	=	0;
	if (ScrollPos	> (int)(si.nMax - si.nPage+1)	)	ScrollPos	=	si.nMax - si.nPage+1;


	switch(g_iWorkMode)
	{	case	dWORK_TILE			:
			TilePos		=	ScrollPos;
			break;
		case	dWORK_STATIC_OBJECT	:
			StaticPos	=	ScrollPos;
			break;
		case	dWORK_FREE_OBJECT	:
			FreePos		=	ScrollPos;
			break;
		case	dWORK_BUILDING		:
			if (m_isEditBlockOfBuilding)	m_iBuildingBlockPos	=	ScrollPos;
			else							BuildingPos			=	ScrollPos;
			break;
		case	dWORK_CHARACTER		:
			CharacterPos=	ScrollPos;
			break;

		case	dWORK_AREA			:
			AreaPos		=	ScrollPos;
			break;

		case	dWORK_EDIT			:
			switch(g_iPreWorkMode)
			{	case	dWORK_TILE			:
					TilePos		=	ScrollPos;
					break;
				case	dWORK_STATIC_OBJECT	:
					StaticPos	=	ScrollPos;
					break;
				case	dWORK_FREE_OBJECT	:
					FreePos		=	ScrollPos;
					break;
				case	dWORK_BUILDING		:
					BuildingPos	=	ScrollPos;
					break;
				case	dWORK_CHARACTER		:
					CharacterPos=	ScrollPos;
					break;

				case	dWORK_AREA			:
					AreaPos		=	ScrollPos;
					break;
			}
			break;
	}

	if (si.nPos != ScrollPos )
	{
		si.fMask = SIF_POS;
		si.nPos  = ScrollPos;
		SetScrollInfo( hWND, SB_VERT, &si, TRUE );
		Draw();
	}
}

void
cTOOLWND::UpdateCommand(int menu)
{
	UpdateAreaCommand(menu);
	UpdateCharacterCommand(menu);
	UpdateRoadCommand(menu);
}

LRESULT CALLBACK 
cTOOLWND::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
//		case WM_PAINT		:
//			TOOLWND.Active();
//			TOOLWND.Flip();
//			return (0L);

		case WM_VSCROLL		:
			TOOLWND.VScroll(wParam);
			break;

		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hWnd, &ps );

			TOOLWND.Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hWnd, &ps);
			break;
		}

		case WM_COMMAND		:
			TOOLWND.UpdateCommand(LOWORD( wParam ));
			break;

		case WM_CLOSE		:
		case WM_DESTROY		:
			return (0L);

		default :
			return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}