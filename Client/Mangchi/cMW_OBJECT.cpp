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
#include	"cACTOR.H"

void
cMAPWND::UpdateStaticObjects()
{
	if (g_iWorkMode	!=	dWORK_STATIC_OBJECT){
		m_bLineObjectCreate;
		return;
	}

	if (TOUCHRBUTTON)
	{
		TOUCHRBUTTON	=	FALSE;
		MAIN.ChangeMode(dWORK_EDIT);
	}

//	if (g_iScale	!=	100)	return;


	
	if(KEY[dKEY_SHIFT]){

		if(KEY[dKEY_CONTROL] && m_bLineObjectCreate == TRUE){

			int ex = Cursor.x + g_lpActmap->m_pos.x;
			int ey = Cursor.y + g_lpActmap->m_pos.y;

			int sx = m_posLineStart.x;
			int sy = m_posLineStart.y;
			
			if(sx - ex == 0){
			
				m_posSnapLineLast.x = sx;
				m_posSnapLineLast.y = ey;
			
			}else if(sy - ey == 0){
				
				m_posSnapLineLast.x = ex;
				m_posSnapLineLast.y = sy;
				

			}else{

				if(abs(ex-sx) ==  abs(ey-sy)){
					m_posSnapLineLast.x = ex;
					m_posSnapLineLast.y = ey;
				}

			}

			m_bSnapLine = TRUE;	
			
		}else {

			m_posSnapLineLast	= m_posLineStart;
			m_bSnapLine			= FALSE;	
			
		}

		if(TOUCHLBUTTON){
			
			int	x	=	Mouse.x*100/g_iScale/dTILE_WIDTH + g_lpActmap->m_pos.x;
			int	y	=	(Mouse.y-g_iTBSize)*100/g_iScale/dTILE_HEIGHT+ g_lpActmap->m_pos.y;
			if(m_bLineObjectCreate == FALSE){
				
				m_posLineStart.Set(x,y);
				m_bLineObjectCreate = TRUE;		

			}else if(m_bSnapLine == TRUE){

				lineObjectCreate(m_posLineStart.x, m_posLineStart.y, m_posSnapLineLast.x, m_posSnapLineLast.y);
				
			}else{

				lineObjectCreate(m_posLineStart.x, m_posLineStart.y, Cursor.x+g_lpActmap->m_pos.x, Cursor.y+g_lpActmap->m_pos.y);
			}			
		}

		m_isRepaint = TRUE;

	}else{
		
		if (LBUTTON && m_bLineObjectCreate == FALSE && m_bSnapLine == FALSE)
		{
			
			int	x	=	Mouse.x*100/g_iScale/dTILE_WIDTH + g_lpActmap->m_pos.x;
			int	y	=	(Mouse.y-g_iTBSize)*100/g_iScale/dTILE_HEIGHT+ g_lpActmap->m_pos.y;
			
			if (Mouse.y	>	g_iTBSize || g_isDrawingMap)
			{
				if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
				{
					g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject,x,y);
					m_isRepaint	=	TRUE;
					g_lpActmap->LostVirgin();
				}
			}
			
			
		}	


		m_bLineObjectCreate = FALSE;
		m_bSnapLine			= FALSE;
	}


	if (!Cursor.isSame(&OldCursor)		)	m_isRepaint	=	TRUE;	//	커서가 움직였냐?
}


void cMAPWND::lineObjectCreate(){		// 09.07.13 라인에 따른 오브젝트 생성
	
	int	x		=	Cursor.x+g_lpActmap->m_pos.x;
	int	y		=	Cursor.y+g_lpActmap->m_pos.y; 
	
	int x1		=	x - m_posLineStart.x;
	int y1		=	y - m_posLineStart.y;



	if( x1 == 0 ){
		
		for(int i =  min(y, m_posLineStart.y) ; i<=max(y,m_posLineStart.y); ++i){

			if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
			{
				g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject, x , i);
				m_isRepaint	=	TRUE;
				g_lpActmap->LostVirgin();
				
			}		
		}

	}else if( y1 == 0){

		for(int i =  min(x, m_posLineStart.x) ; i<=max(x,m_posLineStart.x); ++i){
			if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
			{
				g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject, i , y);
				m_isRepaint	=	TRUE;
				g_lpActmap->LostVirgin();
				
			}		
		}

	}else{

		float fSlope =  float(y1)/x1;

		int ix = (x> m_posLineStart.x)? -1 : 1;
		int iy = (y> m_posLineStart.y)? -1 : 1;
		int i,j;
		if(fSlope <0 ) {
			fSlope*=-1;
		}

		for(i = 0 ;i<=abs(x-m_posLineStart.x) ; ++i){
			for(j = 0 ;j<=abs(y - m_posLineStart.y) ; ++j){
				
				if( i*fSlope <= j + 0.5f && i*fSlope>= j-0.5f){


					if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
					{
						g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject, x + i*ix, j*iy + y);
						m_isRepaint	=	TRUE;
						g_lpActmap->LostVirgin();
						
					}
				}
			}
		}

		fSlope = float(x1)/y1;

		if(fSlope <0 ) {
			fSlope*=-1;
		}
		
		for(i = 0 ;i<=abs(y-m_posLineStart.y) ; ++i){
			for(j = 0 ;j<=abs(x - m_posLineStart.x) ; ++j){
				
				if( i*fSlope <= j + 0.5f && i*fSlope>= j-0.5f){


					if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
					{
						g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject, x + j*ix, i*iy + y);
						m_isRepaint	=	TRUE;
						g_lpActmap->LostVirgin();
						
					}
				}
			}
		}

	}
	
	m_bLineObjectCreate	= FALSE;

}
void cMAPWND::lineObjectCreate(int _sx, int _sy, int _ex, int _ey){		// 09.07.14 라인에 따른 오브젝트 생성
	
	
	int x1		=	_ex - _sx;
	int y1		=	_ey - _sy;



	if( x1 == 0 ){
		
		for(int i =  min(_ey, _sy) ; i<=max(_ey, _sy); ++i){

			if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
			{
				g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject, _ex , i);
				m_isRepaint	=	TRUE;
				g_lpActmap->LostVirgin();
				
			}		
		}

	}else if( y1 == 0){

		for(int i =  min(_ex, _sx) ; i<=max(_ex, _sx); ++i){
			if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
			{
				g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject, i , _ey);
				m_isRepaint	=	TRUE;
				g_lpActmap->LostVirgin();
				
			}		
		}

	}else{

		float fSlope =  float(y1)/x1;

		int ix = (_ex> _sx)? -1 : 1;
		int iy = (_ey> _sy)? -1 : 1;
		int i,j;
		if(fSlope <0 ) {
			fSlope*=-1;
		}

		for(i = 0 ;i<=abs(_ex - _sx) ; ++i){
			for(j = 0 ;j<=abs(_ey - _sy) ; ++j){
				
				if( i*fSlope <= j + 0.5f && i*fSlope>= j-0.5f){


					if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
					{
						g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject, _ex + i*ix, j*iy + _ey);
						m_isRepaint	=	TRUE;
						g_lpActmap->LostVirgin();
						
					}
				}
			}
		}

		fSlope = float(x1)/y1;

		if(fSlope <0 ) {
			fSlope*=-1;
		}
		
		for(i = 0 ;i<=abs(_ey-_sy) ; ++i){
			for(j = 0 ;j<=abs(_ex - _sx) ; ++j){
				
				if( i*fSlope <= j + 0.5f && i*fSlope>= j-0.5f){


					if (g_lpActmap->m_object.getFixedObjectCount()	<	dMAX_OBJECT_COUNT)
					{
						g_lpActmap->buildObject(g_lpActmap->m_object.m_iSelectFixedObject, _ex + j*ix, i*iy + _ey);
						m_isRepaint	=	TRUE;
						g_lpActmap->LostVirgin();
						
					}
				}
			}
		}

	}
	
	m_bLineObjectCreate	= FALSE;

}
//
//
void
cMAPWND::updateTinyObjects()
{	if (g_iWorkMode	!=	dWORK_FREE_OBJECT)	return;

	if (TOUCHRBUTTON)
	{
		TOUCHRBUTTON	=	FALSE;
		MAIN.ChangeMode(dWORK_EDIT);
	}

//	if (g_iScale	!=	100)	return;

	if (TOUCHKEY[dKEY_TAB])
	{
		g_lpActmap->cycleSelectAddonObject();
		m_isRepaint	=	TRUE;
	}

	if (KEY[dKEY_SHIFT])
	{
		if (TOUCHKEY[dKEY_DELETE])
		{	g_lpActmap->removeSelectAddonObject();
			m_isRepaint	=	TRUE;
		}
		if (TOUCHKEY[dKEY_LEFT])
		{	g_lpActmap->moveSelectAddonObject(-1,0);
			m_isRepaint	=	TRUE;
		}
		if (TOUCHKEY[dKEY_RIGHT])
		{	g_lpActmap->moveSelectAddonObject(1,0);
			m_isRepaint	=	TRUE;
		}
		if (TOUCHKEY[dKEY_DOWN])
		{	g_lpActmap->moveSelectAddonObject(0,1);
			m_isRepaint	=	TRUE;
		}
		if (TOUCHKEY[dKEY_UP])
		{	g_lpActmap->moveSelectAddonObject(0,-1);
			m_isRepaint	=	TRUE;
		}
	}

	if (LBUTTON)
	{
		if (Mouse.y	>	g_iTBSize || g_isDrawingMap)
		{
			int	x				=	m_posWorld.x-g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
			int	y				=	m_posWorld.y-g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE;

			x					=	g_lpActmap->m_pos.x*dTILE_WIDTH /dSHARE_TILE+x*100/g_iScale;
			y					=	g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE+y*100/g_iScale;
			
			if (KEY[dKEY_SHIFT])
			{
				if (TOUCHLBUTTON)
					if (g_lpActmap->addAddonObject(g_lpActmap->m_object.m_iSelectTinyObject,x,y))
						m_isRepaint	=	TRUE;
			}
			else
			if (g_lpActmap->addTinyObject(g_lpActmap->m_object.m_iSelectTinyObject,x,y))
				m_isRepaint	=	TRUE;
		}
	}

	if (!Mouse.isSame(&OldMouse)		)	m_isRepaint	=	TRUE;	//	커서가 움직였냐?
}	//	cMAPWND::updateTinyObjects()


void 
cMAPWND::DrawLineObjectCursor(int _sx, int _sy, int _ex, int _ey)		// 09.07.30 추가
{
	
	if(!m_bLineObjectCreate || g_iWorkMode != dWORK_STATIC_OBJECT)
		return;

	int x1		=	_ex - _sx;
	int y1		=	_ey - _sy;	

	if( x1 == 0 ){
		
		for(int i =  min(_ey, _sy) ; i<=max(_ey, _sy); ++i){
			
			BOOL	crash	=	g_lpActmap->isCrashFixedObject(g_lpActmap->m_object.m_iSelectFixedObject,_ex,i);

			int iPutX = (_ex - g_lpActmap->m_pos.x) * dTILE_WIDTH/dSHARE_TILE+ dTILE_WIDTH/dSHARE_TILE /2;
			int iPutY = (i - g_lpActmap->m_pos.y)*dTILE_HEIGHT/dSHARE_TILE + dTILE_HEIGHT/dSHARE_TILE /2+g_iTBSize;

			g_lpActmap->m_object.putFixedObject(g_lpActmap->m_object.m_iSelectFixedObject,iPutX,iPutY,crash);
			
		}
		
	}else if( y1 == 0){
		
		for(int i =  min(_ex, _sx) ; i<=max(_ex, _sx); ++i){
			
			BOOL	crash	=	g_lpActmap->isCrashFixedObject(g_lpActmap->m_object.m_iSelectFixedObject,i,_ey);

			int iPutX = (i - g_lpActmap->m_pos.x)*dTILE_WIDTH/dSHARE_TILE + dTILE_WIDTH/dSHARE_TILE /2;
			int iPutY = (_ey - g_lpActmap->m_pos.y)*dTILE_HEIGHT+ dTILE_HEIGHT/dSHARE_TILE /2+g_iTBSize;

			g_lpActmap->m_object.putFixedObject(g_lpActmap->m_object.m_iSelectFixedObject,iPutX,iPutY,crash);
			
		}
		
	}else{
		float fSlope =  float(y1)/x1;

		int ix = (_ex> _sx)? -1 : 1;
		int iy = (_ey> _sy)? -1 : 1;
		int i,j;
		if(fSlope <0 ) {
			fSlope*=-1;
		}

		for(i = 0 ;i<=abs(_ex - _sx) ; ++i){
			for(j = 0 ;j<=abs(_ey - _sy) ; ++j){
				
				if( i*fSlope <= j + 0.5f && i*fSlope>= j-0.5f){
		
					BOOL	crash	=	g_lpActmap->isCrashFixedObject(g_lpActmap->m_object.m_iSelectFixedObject, _ex + i*ix, j*iy + _ey);
					int iPutX = (_ex + i*ix- g_lpActmap->m_pos.x) * dTILE_WIDTH/dSHARE_TILE+ dTILE_WIDTH/dSHARE_TILE /2;
					int iPutY = (j*iy + _ey- g_lpActmap->m_pos.y) * dTILE_HEIGHT/dSHARE_TILE + dTILE_HEIGHT/dSHARE_TILE /2+g_iTBSize;
					g_lpActmap->m_object.putFixedObject(g_lpActmap->m_object.m_iSelectFixedObject,iPutX,iPutY,crash);
					
				}
				
			}
		}
		fSlope = float(x1)/y1;

		if(fSlope <0 ) {
			fSlope*=-1;
		}
		
		for(i = 0 ;i<=abs(_ey-_sy) ; ++i){
			for(j = 0 ;j<=abs(_ex - _sx) ; ++j){
				
				if( i*fSlope <= j + 0.5f && i*fSlope>= j-0.5f){
					
					BOOL	crash	=	g_lpActmap->isCrashFixedObject(g_lpActmap->m_object.m_iSelectFixedObject, _ex + j*ix, i*iy + _ey);
					int iPutX =  (_ex + j*ix- g_lpActmap->m_pos.x) * dTILE_WIDTH/dSHARE_TILE+ dTILE_WIDTH/dSHARE_TILE /2;
					int iPutY =  (i*iy + _ey- g_lpActmap->m_pos.y) * dTILE_HEIGHT/dSHARE_TILE + dTILE_HEIGHT/dSHARE_TILE /2+g_iTBSize;			
					
					g_lpActmap->m_object.putFixedObject(g_lpActmap->m_object.m_iSelectFixedObject, iPutX,iPutY,crash);
					
				}
			}
		}
		
	}
}
