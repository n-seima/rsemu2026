#include "cFIELD.h"
#include "arca.h"
#include "pieceItem.h"
#include "itemSet.h"


void
cFIELD::moveActorToSafePosInArea(int _iArea)
{
	cAreaInfo	*lpArea	=	getArea(_iArea);

	if (!lpArea)
		return;

	for (int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
	{
		for (int	i=m_aiActorCount[iTeam];i;)
		{
			--i;

			cACTOR	*lpActor	=	getTestedActor(m_awActorList[iTeam][i]);

			if (!lpActor)
				continue;

			int	iMx		=	lpActor->m_pos.x >>	dTILE_XSIZE_SHIFT;
			int	iMy		=	lpActor->m_pos.y >>	dTILE_YSIZE_SHIFT;

			if (!isBlocked(iMx,iMy))
				continue;

			cPOINT	pos;

			if (findEnterancePos(&pos,lpArea))
			{
				if (lpActor->setActorPos(pos.x,pos.y,TRUE,TRUE))
					lpActor->sendCurrentPosition();
			}
			else
			{
				cAreaInfo	*lpStartArea	=	m_pArea->getStartArea();

				if (!lpStartArea)
					lpStartArea	=	m_pArea->getFirstPortal();

				if (findEnterancePos(&pos,lpArea))
				{
					if (lpActor->setActorPos(pos.x,pos.y,TRUE,TRUE))
						lpActor->sendCurrentPosition();
				}
			}
		}
	}
}
