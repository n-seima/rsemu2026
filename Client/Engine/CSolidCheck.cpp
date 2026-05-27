#include "stdafx.h"
#include "csolidcheck.h"
#include <math.h>

CSolidCheck::CSolidCheck()
{
	m_pLines = NULL;
}

CSolidCheck::~CSolidCheck()
{
	if(m_pLines)	delete []m_pLines;
	m_pLines = NULL;
}

LPLineNode DumpSolidNode(POINT *pos, LPWORD lpIndex, int iNumNode)
{
	if(iNumNode==0)		return NULL;

	LPLineNode pNode = new LineNode;
	if(!pNode)			return NULL;

	pNode->fNX	= float(pos[ lpIndex[1] ].y-pos[ lpIndex[0] ].y);
	pNode->fNY	= float(pos[ lpIndex[0] ].x-pos[ lpIndex[1] ].x);

	float InvMag = 1/(float)sqrt( pNode->fNX*pNode->fNX + pNode->fNY*pNode->fNY );

	pNode->fNX	*= InvMag;
	pNode->fNY	*= InvMag;

	pNode->iD	= -int(pNode->fNX*pos->x + pNode->fNY*pos->y);

	WORD FrontIndex[255], BackIndex[255];
	POINT FrontPos[255], BackPos[255];

	int	iNumFront = 0, iNumBack = 0;

	POINT *FirstPos, *SecondPos;

	int FirstTest, SecondTest;

	float distance, Epsilon = 0.999999f;

	lpIndex+=2;

	POINT ClipPos;

	for(int i=1; i<iNumNode; i++, lpIndex+=2)
	{
		FirstPos	= pos+lpIndex[0];
		SecondPos	= pos+lpIndex[1];

		distance	= pNode->fNX*FirstPos->x + pNode->fNY*FirstPos->y + pNode->iD;
		FirstTest	= (distance>Epsilon) ? 1 : ( (distance<-Epsilon) ? -1 : 0 );

		distance	= pNode->fNX*SecondPos->x + pNode->fNY*SecondPos->y + pNode->iD;
		SecondTest	= (distance>Epsilon) ? 1 : ( (distance<-Epsilon) ? -1 : 0 );

		switch(FirstTest+SecondTest)
		{
		case 2:		case 1:
			FrontPos[iNumFront]		= *FirstPos;
			FrontPos[iNumFront+1]	= *SecondPos;
			FrontIndex[iNumFront]	= iNumFront;
			FrontIndex[iNumFront+1]	= iNumFront+1;
			iNumFront += 2;
			break;

		case -2:	case -1:
			BackPos[iNumBack]		= *FirstPos;
			BackPos[iNumBack+1]		= *SecondPos;
			BackIndex[iNumBack]		= iNumBack;
			BackIndex[iNumBack+1]	= iNumBack+1;
			iNumBack += 2;
			break;

		default:
			{
				float aDot = FirstPos->x*pNode->fNX + FirstPos->y*pNode->fNY;
				float bDot = SecondPos->x*pNode->fNX + SecondPos->y*pNode->fNY;
				float scale = (-(float)pNode->iD - aDot) / (bDot - aDot);
				ClipPos.x = int( FirstPos->x + (SecondPos->x-FirstPos->x) * scale );
				ClipPos.y = int( FirstPos->y + (SecondPos->y-FirstPos->y) * scale );
			}
			FrontPos[iNumFront]		= *FirstPos;
			FrontPos[iNumFront+1]	= ClipPos;
			BackPos[iNumBack]		= ClipPos;
			BackPos[iNumBack+1]		= *SecondPos;

			FrontIndex[iNumFront]	= iNumFront;
			FrontIndex[iNumFront+1]	= iNumFront+1;
			BackIndex[iNumBack]		= iNumBack;
			BackIndex[iNumBack+1]	= iNumBack+1;

			iNumFront	+= 2;
			iNumBack	+= 2;
		}
	}

	pNode->pFront	= DumpSolidNode(FrontPos, FrontIndex, iNumFront/2);
	pNode->pBack	= DumpSolidNode(BackPos, BackIndex, iNumBack/2);

	return pNode;
}

void DeleteNode(LPLineNode lpNode)
{
	if(lpNode->pFront)	DeleteNode(lpNode->pFront);
	if(lpNode->pBack)	DeleteNode(lpNode->pBack);
	delete lpNode;
}

int GetNodeCount(LPLineNode lpNode)
{
	int cot = 1;
	if(lpNode->pFront)	cot += GetNodeCount(lpNode->pFront);
	if(lpNode->pBack)	cot += GetNodeCount(lpNode->pBack);
	return cot;
}

LPLineNode CopyNode(LPLineNode *pSrc, LPLineNode pDes)
{
	LPLineNode lpSrc = *pSrc;
	(*pSrc)++;

	*lpSrc = *pDes;
	lpSrc->pFront	= NULL;
	lpSrc->pBack	= NULL;

	if(pDes->pFront)
	{
		lpSrc->pFront	= (*pSrc);
		CopyNode(pSrc, pDes->pFront);
	}

	if(pDes->pBack)
	{
		lpSrc->pBack	= (*pSrc);
		CopyNode(pSrc, pDes->pBack);
	}

	return lpSrc;
}

BOOL CSolidCheck::SetSolid(POINT *pos, int iCount)
{
	if(iCount>100)	return false;

	CSolidCheck::~CSolidCheck();

	WORD index[255], *lpIndex = index;
	for(int i=0; i<iCount; i++, lpIndex+=2)
	{	
		lpIndex[0] = i;
		lpIndex[1] = i+1;
	}
	lpIndex[-1] = 0;

	LPLineNode pSolid = DumpSolidNode(pos, index, iCount);

	if(!pSolid)		return false;

	m_wNumLine	= GetNodeCount(pSolid);

	LPLineNode lpLine = m_pLines	= new LineNode[m_wNumLine];

	if(lpLine)		CopyNode(&lpLine, pSolid);

	DeleteNode(pSolid);

	return (m_pLines) ? true : false;
}	

int TestLine(LPLineNode lpLine, POINT &pos)
{
	int distance	= int( lpLine->fNX*pos.x + lpLine->fNY*pos.y + lpLine->iD );

	if(distance>0)
	{
		if( !lpLine->pFront )	return 1;

		return TestLine( lpLine->pFront, pos );
	}
	else if(distance<0)
	{
		if( !lpLine->pBack )	return -1;

		return TestLine( lpLine->pBack, pos );
	}

	if(lpLine->pFront)	return TestLine(lpLine->pFront, pos);

	if(lpLine->pBack)	return TestLine(lpLine->pBack, pos);

	return 0;
}

BOOL CSolidCheck::IsCheckIn(POINT pos)
{
	if(!m_pLines)	return false;

	int test = TestLine(m_pLines, pos);

	return (test<=0);
}