#include "patrolRoad.h"
#include "cMAPWND.h"
#include "cDRAW.h"
#include "cTEXT.h"

cPatrolRoad	g_editRoad;

cPatrolRoad::cPatrolRoad()
{
	reset();
}

void
cPatrolRoad::reset()
{
	m_wSerial		=	0xffff;
	m_wNodeCount	=	0;
	m_wStayTime		=	0;
	m_wIsReturnRoad	=	FALSE;

	strcpy(m_strName,"no name");
}

void
cPatrolRoad::copy(cPatrolRoad *_lpRoad)
{
	memcpy(this,_lpRoad,sizeof(cPatrolRoad));
}

BOOL
cPatrolRoad::addNode(int _iX,int _iY)
{
	if (m_wNodeCount	>=	dMAX_PATROL_ROAD_NODE_COUNT)
		return	FALSE;

	m_aNode[m_wNodeCount].x	=	_iX;
	m_aNode[m_wNodeCount].y	=	_iY;

	m_wNodeCount++;

	return	TRUE;
}

void
cPatrolRoad::putNode(int _iIndex,BOOL _bIsEditRoad,BOOL _bIsSelectRoad)
{
	int		iSize	=	1;
	int		iX		=	m_aNode[_iIndex].x	-	g_posMapwnd.x;
	int		iY		=	m_aNode[_iIndex].y	-	g_posMapwnd.y;

	iX	=	iX*g_iScale/100;
	iY	=	iY*g_iScale/100;

	iY	+=	g_iTBSize;

	if (_iIndex	==	0	||	_iIndex	==	m_wNodeCount-1)
		iSize		=	2;

	int	iSize2		=	iSize+1;

	WORD	wColor,wBoxColor;

	if (_iIndex	==	0)
		wColor		=	_LTYELLOW;
	else
		wColor		=	_LTGREEN;

	if (_bIsSelectRoad	||	_bIsEditRoad)
		wBoxColor	=	_LTBLUE;
	else
		wBoxColor	=	0;

	cDRAW::Fill(wColor,iX-iSize,iY-iSize,iX+iSize,iY+iSize);
	cDRAW::Box(0,iX-iSize2,iY-iSize2,iX+iSize2,iY+iSize2);

	cTEXT::OutlineOn();

	if (_iIndex	==	0	&&	_bIsEditRoad	==	FALSE)
	{
		char	strName[64];

		sprintf(strName,"#%.2d %s[%d 초]",_iIndex+1,m_strName,m_wStayTime);
		int		xs	=	cTEXT::GetSize(strName);

		cDRAW::FillHB(0,iX - xs/2-2 ,iY-2+7 ,iX + xs/2+2,iY+12+7);
		cTEXT::cPut(iX,iY+7,WHITE,strName);
	}
	else
	if (_iIndex	==	m_wNodeCount-1	&&	_bIsEditRoad	==	FALSE	&&	m_wIsReturnRoad	==	TRUE)
	{
		char	strName[64];

		strcpy(strName,"(R)");
		int		xs	=	cTEXT::GetSize(strName);

		cDRAW::FillHB(0,iX - xs/2-2 ,iY-2+7 ,iX + xs/2+2,iY+12+7);
		cTEXT::cPut(iX,iY+7,WHITE,strName);
	}
	else
	{
		cTEXT::cPut(iX,iY+7,WHITE,_ms("#%.2d",_iIndex+1));
	}

	cTEXT::OutlineOff();
}

void
cPatrolRoad::draw(BOOL _bIsEditRoad,BOOL _bIsSelectRoad)
{
	int		i;
	BOOL	bIsVisible	=	FALSE;

	for (i=0;i<m_wNodeCount;i++)
	{
		if (g_rectMapwnd.isIN(m_aNode[i].x,m_aNode[i].y))
			bIsVisible	=	TRUE;
	}

	if (!bIsVisible)
		return;

	cPOINT	posBegin,posEnd;

	if	(m_wNodeCount	==	1)
	{
		putNode(0,_bIsEditRoad,_bIsSelectRoad);
		return;
	}

	WORD	wLineColor;

	for (i=0;i<m_wNodeCount-1;i++)
	{
		posBegin.x	=	(m_aNode[i].x-g_posMapwnd.x)*g_iScale/100;
		posBegin.y	=	(m_aNode[i].y-g_posMapwnd.y)*g_iScale/100;
		posEnd.x	=	(m_aNode[i+1].x-g_posMapwnd.x)*g_iScale/100;
		posEnd.y	=	(m_aNode[i+1].y-g_posMapwnd.y)*g_iScale/100;

		posBegin.y	+=	g_iTBSize;
		posEnd.y	+=	g_iTBSize;

		wLineColor	=	_WHITE;

		if	(_bIsEditRoad	&&	i == m_wNodeCount-2)
			wLineColor	=	_LTCYAN;

		if	(_bIsSelectRoad)
			wLineColor	=	_LTGREEN;

		cDRAW::Line(wLineColor,posBegin.x,posBegin.y,posEnd.x,posEnd.y);

		if (abs(posBegin.x-posEnd.x) > abs(posBegin.y-posEnd.y))
			cDRAW::Line(wLineColor,posBegin.x,posBegin.y+1,posEnd.x,posEnd.y+1);
		else
			cDRAW::Line(wLineColor,posBegin.x+1,posBegin.y,posEnd.x+1,posEnd.y);
	}

	if(_bIsEditRoad	==	FALSE	&&	m_wIsReturnRoad	==	FALSE)
	{
		posBegin.x	=	(m_aNode[m_wNodeCount-1].x-g_posMapwnd.x)*g_iScale/100;
		posBegin.y	=	(m_aNode[m_wNodeCount-1].y-g_posMapwnd.y)*g_iScale/100;
		posEnd.x	=	(m_aNode[0].x-g_posMapwnd.x)*g_iScale/100;
		posEnd.y	=	(m_aNode[0].y-g_posMapwnd.y)*g_iScale/100;

		posBegin.y	+=	g_iTBSize;
		posEnd.y	+=	g_iTBSize;

		wLineColor	=	_WHITE;

		if	(_bIsSelectRoad)
			wLineColor	=	_LTGREEN;

		cDRAW::Line(wLineColor,posBegin.x,posBegin.y,posEnd.x,posEnd.y);

		if (abs(posBegin.x-posEnd.x) > abs(posBegin.y-posEnd.y))
			cDRAW::Line(wLineColor,posBegin.x,posBegin.y+1,posEnd.x,posEnd.y+1);
		else
			cDRAW::Line(wLineColor,posBegin.x+1,posBegin.y,posEnd.x+1,posEnd.y);
	}

	for (i=0;i<m_wNodeCount;i++)
		putNode(i,_bIsEditRoad,_bIsSelectRoad);
}


cPatrolRoadManager::cPatrolRoadManager()
{
	init();
	reset();
	m_iSelect	=	0;
}

void
cPatrolRoadManager::build()
{
	m_den.rebuild();
}

cPatrolRoad*
cPatrolRoadManager::get(int _iIndex)
{
	if (_iIndex	==	-1)
		return	m_den.get(m_iSelect);

	return	m_den.get(_iIndex);
}

BOOL
cPatrolRoadManager::remove(int _iSerial)
{
	if (_iSerial	==	-1)
		return	m_den.remove(m_iSelect);

	return	m_den.remove(_iSerial);
}

void
cPatrolRoadManager::draw()
{
	m_den.setFirst();

	while(1)
	{
		cPatrolRoad	*lpInfo	=	m_den.getNext();

		if (!lpInfo)
			break;

		BOOL	bIsSelect	=	FALSE;

		if	(m_iSelect	==	lpInfo->m_wSerial)
			bIsSelect	=	TRUE;

		lpInfo->draw(FALSE,bIsSelect);
	}
}

int
cPatrolRoadManager::add(cPatrolRoad *_lpRoad,BOOL _bIsCheckName)
{
	m_den.setFirst();

	while(1)
	{
		cPatrolRoad	*lpInfo	=	m_den.getNext();

		if (!lpInfo	||	lpInfo->m_wSerial	==	_lpRoad->m_wSerial)
			break;

		if (stricmp(lpInfo->m_strName,_lpRoad->m_strName) == 0)
		{
			ERRMSG("error!!","이미 같은 이름의 순찰 루트가 있습니다.");

			return	0xffff;
		}
	}

	if	(_bIsCheckName)
		return	0;

	return	m_den.add(_lpRoad);

}

BOOL
cPatrolRoadManager::replace(int _iSerial,cPatrolRoad *_lpRoad)
{
	m_den.setFirst();

	while(1)
	{
		cPatrolRoad	*lpInfo	=	m_den.getNext();

		if (!lpInfo || _iSerial == lpInfo->m_wSerial)
			break;

		if (stricmp(lpInfo->m_strName,_lpRoad->m_strName) == 0)
			return	ERRMSG("error!!","이미 같은 이름의 순찰 루트가 있습니다.");;
	}

	return	m_den.replace(_iSerial,_lpRoad);	

}