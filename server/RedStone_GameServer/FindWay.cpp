#include "cACTOR.h"
#include "cFIELD.h"

// 클레스 선언은 cACTOR.h 에 있습니다.
// JBC	4방향 간단한 길찾기 입니다.		08-09-17
// 4방향을 체크해서 가장 가까운 경로를 찾아 이동하고 왔던 길은 될돌아가지 않습니다.

BOOL
cFindOtherWayByBC::getAllPath(cACTOR* _Actor , int _iX , int _iY , int iNowPath)
{
	int MinValue = 1000000;				//큰수를 의미.
	int MinPlace = 0 ;

	//GetOvalRange로 부하 걸릴까봐 한거임..
	for (int i = 0 ; i < d4WAY_DIRECT_COUNT ; i++)
	{
		if(!m_aisCheck[i])
			m_aiDistance[i] = GetOvalRange(m_aPos[i].x , m_aPos[i].y , _iX , _iY);
		else
			m_aiDistance[i] = 1000000;
	}

	//체크리스트랑 거리를 체크해서 가장작은 거리의 값과 거리를 구한다.
	for (i = 0 ; i < d4WAY_DIRECT_COUNT ; i++)
	{
		if(!m_aisCheck[i])
		{
			if(MinValue > m_aiDistance[i])
			{
				MinPlace	=	i ;	
				MinValue	=	m_aiDistance[i];
			}
		}
	}

	if(MinValue == 1000000)
		return FALSE;

	if((m_PosLast.x == m_aPos[MinPlace].x)&&(m_PosLast.y == m_aPos[MinPlace].y))
		return FALSE;

	m_iLastDirecion = MinPlace;
		
 	_Actor->m_posDest.x		=	m_aPos[MinPlace].x;
 	_Actor->m_posDest.y		=	m_aPos[MinPlace].y;

	m_PosLast.x = m_aPos[MinPlace].x;
	m_PosLast.y = m_aPos[MinPlace].y;

	g_protractor.getPath(_Actor->m_aPath,_Actor->m_pos.x,_Actor->m_pos.y,_Actor->m_posDest.x,_Actor->m_posDest.y,_Actor->m_wPPS);
		
	_Actor->m_wAngle		=	g_protractor.getAngleToTarget(_Actor->m_pos.x,_Actor->m_pos.y,m_aPos[MinPlace].x,m_aPos[MinPlace].y);
		
	_Actor->m_wDirect		=	GetDirect(_Actor->m_wAngle,dDEFAULT_DIRECT_COUNT);

	return TRUE;
}

void
cFindOtherWayByBC::getMovePos(cACTOR* _Actor)
{
	BYTE	*lpMapInfo	=	_Actor->m_lpField->m_pMap;
	int		iMapWidth	=	_Actor->m_lpField->m_iWidth;
	int		iMapHeight	=	_Actor->m_lpField->m_iHeight;
	int		tapX		=	_Actor->m_wPPS/dUPDATE_MOVE_POS_PERIOD;
	int		tapY		=	_Actor->m_wPPS/dUPDATE_MOVE_POS_PERIOD;

	Reset();
	
	m_aPos[d4WAY_DIRECTION_N].x	=	_Actor->m_pos.x ;
	m_aPos[d4WAY_DIRECTION_N].y	=	_Actor->m_pos.y - tapY ;
	
	m_aPos[d4WAY_DIRECTION_E].x	=	_Actor->m_pos.x + tapX;
	m_aPos[d4WAY_DIRECTION_E].y	=	_Actor->m_pos.y ;
	
	m_aPos[d4WAY_DIRECTION_S].x	=	_Actor->m_pos.x ;
	m_aPos[d4WAY_DIRECTION_S].y	=	_Actor->m_pos.y + tapY ;
	
	m_aPos[d4WAY_DIRECTION_W].x	=	_Actor->m_pos.x - tapX ;
	m_aPos[d4WAY_DIRECTION_W].y	=	_Actor->m_pos.y ;
	
	for (int i =0 ; i < d4WAY_DIRECT_COUNT ; i++)
	{
		m_aPos[i].mx	=	min((m_aPos[i].x >>	dTILE_XSIZE_SHIFT),iMapWidth-1);
		m_aPos[i].my	=	min((m_aPos[i].y >>	dTILE_YSIZE_SHIFT),iMapHeight-1);

		if	(m_aPos[i].mx < 0 || m_aPos[i].my < 0)
			continue;

		int iAddress	=	m_aPos[i].my * iMapWidth + m_aPos[i].mx;

		if	(lpMapInfo[iAddress])
			m_aisCheck[i] = TRUE;
	}

	if(m_iLastDirecion != dDIRECT_RANDOM)
		m_aisCheck[(m_iLastDirecion + 2) % 4 ] =  TRUE;
}

cFindOtherWayByBC::cFindOtherWayByBC()
{
	memset(m_aPos , 0 , sizeof(c4POINT) * d4WAY_DIRECT_COUNT);
	memset(m_aisCheck , 0 , sizeof(BOOL) * d4WAY_DIRECT_COUNT);
	memset(m_aiDistance , 0 , sizeof(int) * d4WAY_DIRECT_COUNT);
	memset(&m_iLastDirecion , 0 , sizeof(int));
	memset(&m_PosLast , 0 , sizeof(cPOINT));

	m_iLastDirecion	= dDIRECT_RANDOM;
	
}

cFindOtherWayByBC::~cFindOtherWayByBC()
{

}
