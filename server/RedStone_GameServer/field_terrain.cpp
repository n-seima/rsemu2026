#include	"cFIELD.H"

BOOL
cFIELD::makeBlock(int _iX1,int _iY1,int _iX2,int _iY2)
{
	cPOINT	*pRoad	=	new cPOINT [100];

	int	iNodeCount	=	g_protractor.getLine(_iX1,_iY1,_iX2,_iY2,pRoad,100);

	for(int i=iNodeCount;i;)
	{
		--i;

		int	iAddress	=	pRoad[i].x+pRoad[i].y*m_iWidth;

		m_pMap[iAddress]=	1;
	}

	for (int iTeam=0;iTeam<2;iTeam++)
	{
		for (int iActor=m_aiActorCount[iTeam];iActor;)
		{
			--iActor;

			cACTOR	*lpActor	=	m_alpActor[m_awActorList[iTeam][iActor]];

			if	(lpActor	==	NULL)
				continue;

			if	(!isBlocked(lpActor->m_pos.mx,lpActor->m_pos.my))
				continue;

			cPOINT	pos;

			pos.x	=	lpActor->m_pos.x;
			pos.y	=	lpActor->m_pos.y;

			if	(!findValidPos(&pos))
				continue;

			lpActor->changePos(pos.x,pos.y);
		}
	}

	delete [] pRoad;

	return	TRUE;
}

BOOL
cFIELD::removeBlock(int _iX1,int _iY1,int _iX2,int _iY2)
{
	cPOINT	*pRoad	=	new cPOINT [100];

	int	iNodeCount	=	g_protractor.getLine(_iX1,_iY1,_iX2,_iY2,pRoad,100);

	for(int i=iNodeCount;i;)
	{
		--i;

		int	iAddress	=	pRoad[i].x+pRoad[i].y*m_iWidth;

		m_pMap[iAddress]=	0;
	}

	delete [] pRoad;

	return	TRUE;
}