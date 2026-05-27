#include "MinimapWindow.h"



POINT			MinimapWindow::m_posMouse;
cMAPWND*		MinimapWindow::m_pMapWnd		= NULL;
MinimapWindow*	MinimapWindow::m_pMinimapWnd	= NULL;


MinimapWindow::MinimapWindow()
:m_iScale(100),
bLbuttondown(FALSE)
{
	m_posMinimap.x = 0;
	m_posMinimap.y = 0;
	
}


MinimapWindow::~MinimapWindow(){
	
}

BOOL MinimapWindow::Init(HINSTANCE _hInst, HWND _hWnd,  int _xP, int _yP, int _xWidth, int _yHeight, int _iScale){
	if (!cDIBWND::Init(0,"MINIMAPWINDOW","MINIMAPWINDOW",
		WS_POPUP|WS_VISIBLE|WS_CAPTION |WS_SYSMENU,
		_xP,_yP,_xWidth, _yHeight,
		_hWnd,NULL,_hInst, 0L,
		SW_SHOW,(WNDPROC)WNDProc
		)
		)
		return FALSE;
	
	m_posMouse.x = 0;
	m_posMouse.y = 0;

	m_iScale = _iScale;

	m_dwCreateTime = GetTickCount();

	Draw();
	
	return TRUE;
	
}
BOOL MinimapWindow::Init(HINSTANCE _hInst, HWND _hWnd,cMAPWND* _mapWnd, int _xP, int _yP, int _xWidth, int _yHeight,int _iScale){
	if (!cDIBWND::Init(0,"MINIMAPWINDOW","MINIMAPWINDOW",
		WS_POPUP|WS_VISIBLE|WS_CAPTION |WS_SYSMENU,
		_xP,_yP,_xWidth, _yHeight,
		_hWnd,NULL,_hInst, 0L,
		SW_SHOW,(WNDPROC)WNDProc
		)
		)
		return FALSE;


	m_posMinimap.x = _xP;
	m_posMinimap.y = _yP;
	m_posMouse.x = 0;
	m_posMouse.y = 0;

	m_pMapWnd = _mapWnd;
	m_pMinimapWnd = this;
	m_iScale = _iScale;
	m_dwCreateTime = GetTickCount();
	Draw();
	

	
	return TRUE;
	
}
void 
MinimapWindow::Draw()
{


	int		x,y,i=0;
	
	if	(!Active())
		return;


	if	(!g_lpActmap)
		g_lpActmap	=	&g_aMap[g_iSelectMap];
	
	cINDEX		cObjectIndex[dMAX_OBJECT];

	cRECT rectMapwnd ;
	rectMapwnd.x1	=	0;
	rectMapwnd.y1	=	0;
	rectMapwnd.x2	=	g_lpActmap->m_iWidth*dTILE_WIDTH/dSHARE_TILE;
	rectMapwnd.y2	=	g_lpActmap->m_iHeight*dTILE_WIDTH/dSHARE_TILE;
	
	g_lpActmap->drawWholeImage(m_iScale);		//	일단 타일을 다 그려준다.
	
	//	바닥에 깔리는 오브젝트를 그냥-_- 그려준다.
	int	objectCount	=	0,iIobjectIndex	=	0;

	g_lpActmap->m_object.drawAllRapeObjectScale(&rectMapwnd,0,0,m_iScale);
	objectCount	+=	g_lpActmap->m_object.getStandObjects(cObjectIndex);				//	뷰포트 안에 오브젝트들을 구한다.

	
	qsort((void *)cObjectIndex,objectCount,sizeof(cINDEX),(int(*)(const void *,const void *))Compare);
	
	cObjectIndex[objectCount].Value	=	0x7fffffff;
	
	for (y=0;y<g_lpActmap->m_iHeight;y++)
	{
		int y_border	=	y*dTILE_REAL_HEIGHT+dTILE_REAL_HEIGHT/2;
		
		while(cObjectIndex[iIobjectIndex].Value	< y_border)
		{
			int	index	=	cObjectIndex[iIobjectIndex].Index;
			
			g_lpActmap->m_object.putScale(0,0,index,m_iScale);
			
			iIobjectIndex++;
		}
		
		for (x=0;x<g_lpActmap->m_iWidth;x++)
		{
			int ty		=	y*dTILE_HEIGHT/dSHARE_TILE+dTILE_HEIGHT/2/dSHARE_TILE;
			int	tx		=	x*dTILE_WIDTH/dSHARE_TILE+dTILE_WIDTH/2/dSHARE_TILE;
			tx			=	tx*m_iScale/100;
			ty			=	ty*m_iScale/100;
			
			int object	=	g_lpActmap->getObject(x,y);
			
			if (object	!=	0xffff)
			{	
				cFIXED_OBJECT_INFO	*lpObject	=	g_lpActmap->m_object.getFixedObject(object);
				
				g_lpActmap->m_object.putFixedObjectScale(lpObject,tx,ty,m_iScale);
			}
			
			int	iBuilding	=	g_lpActmap->getBuilding(x,y);
			
			if (iBuilding	!=	0xff)	g_lpActmap->putRegisteredBuildingScale(iBuilding,tx,ty,m_iScale);
		}
	}

	
	// Area  영역을 그려준다
	if(g_MAPWND.m_bAdditionPrintArea)
		g_lpActmap->m_pArea->DrawAllByScale(0, 0, g_iTBSize-25, m_iScale, false);

	
	// Actor를 그려준다.
	if(g_MAPWND.m_bAdditionPrintActor)
	{
		for(i=0 ; i<g_lpActmap->m_setActor.m_iCount ; i++)
		{
			g_lpActmap->m_setActor.m_aActor[i].putScale(0, 0, g_iTBSize, m_iScale);
		}
	}
 
	Flip();
	

}

void MinimapWindow::Run(){

	if(bLbuttondown == TRUE){
		if(LBUTTON == FALSE){
			Draw();
			bLbuttondown = FALSE;
		}
	}else{
		if(LBUTTON == TRUE){
			bLbuttondown = TRUE;
		}
		if( TOUCHKEY[dKEY_DELETE] == TRUE){
			Draw();
		}
		
	}	

}

LRESULT CALLBACK MinimapWindow::WNDProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){	
	
	switch(uMsg)
	{

	case WM_LBUTTONDOWN :
	
		m_posMouse.y	= HIWORD(lParam);
		m_posMouse.x	= LOWORD(lParam);

		break;
	case WM_PAINT :
/*		
		if(m_pMinimapWnd)
			m_pMinimapWnd->Draw();
*/
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case 0x4d:
			
		break;	
		}
	case WM_KEYUP:

		if(GetTickCount() - m_pMinimapWnd->m_dwCreateTime<=1000)
			break;

		switch(wParam)
		{
		case 0x4d:

			DestroyWindow(hWnd);
		}
		break;
	case WM_LBUTTONUP:

		if(m_pMapWnd && m_pMinimapWnd){
			int iTpw = m_pMapWnd->TPW;
			int iTph = m_pMapWnd->TPH;
			RECT rt;
			GetClientRect(hWnd, &rt);

			g_lpActmap->setPos( (int)((m_posMouse.x / (1.0 * rt.right  / g_lpActmap->m_iWidth )) - iTpw/2),
								(int)((m_posMouse.y / (1.0 * rt.bottom / g_lpActmap->m_iHeight)) - iTph/2) );
			m_pMapWnd->Draw();
		}
		break;

	case WM_DESTROY:		
		m_pMapWnd		= NULL;
		m_pMinimapWnd	= NULL;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
