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

void cMAPWND::resetCopyOrCut(){		// 09.08.03

	if(g_iWorkMode != dWORK_COPYORCUT)
		return;
	
	g_lpActmap->m_object.freeSelect();
	g_lpActmap->m_setActor.freeSelect();
	g_iWorkMode = dWORK_EDIT;
	
	m_bObjectCut	= FALSE;
	m_bObjectCopy	= FALSE;
	m_bObjectArea	= FALSE;
	resetSelectData();

}


void cMAPWND::UpdateCopyOrCut(){		//	09.08.03 오브젝트복사,붙여넣기 업데이트함수  수정

	if(g_iWorkMode != dWORK_COPYORCUT && m_bObjectArea == FALSE)
		return;
	
	if (!Mouse.isSame(&OldMouse))
		m_isRepaint	=	TRUE;	//	커서가 움직였냐?
	
	
	
	
	if(RBUTTON){
		g_iWorkMode = dWORK_EDIT;
		m_bObjectCopy	= FALSE;	
		m_bObjectCut	= FALSE;	
		m_isRepaint		= TRUE;
		m_bObjectArea	= FALSE;
		resetSelectData();
	}
	
	if(LBUTTON)
	{


		if(m_iCopyCount==0  && m_bObjectArea == FALSE) 
			return;
		if(m_bObjectCut == TRUE){
			
			m_bObjectCut	= FALSE;
			m_bObjectCopy	= FALSE;
			g_iWorkMode = dWORK_EDIT;
			g_lpActmap->removeSelectedObject();
			cutSelectData();
			resetSelectData();
			
			
		}else{
			
			copySelectData();
			TOOLWND.Draw();
		}
		g_lpActmap->m_object.freeSelect();
		g_lpActmap->m_setActor.freeSelect();
		m_isRepaint	=	TRUE;
	}
}

void				
cMAPWND::saveSelectData()				// 09.07.31
{

	

	int iFixedCount = 	g_lpActmap->m_object.getSelectedFixedObjectCount();

	int iTinyCount	=	g_lpActmap->m_object.getSelectedTinyObjectCount();
	
	int iActorCount	=	g_lpActmap->m_setActor.getSelectedActor();

	m_iCopyCount	= iFixedCount + iTinyCount + iActorCount;

	if(m_iCopyCount == 0)
		return;
	m_pSelectData = new sSELECT_DATA[m_iCopyCount];
	for(int iCopyIndex=0;iCopyIndex<m_iCopyCount;++iCopyIndex)
	{
		memset(m_pSelectData[iCopyIndex].aAddonObject,0, sizeof(cADD_ON_OBJECT) * dMAX_ADD_ON_OBJECT_COUNT);
	}

	int i = 0;
	int iIndex = 0;
	int iCount = 0;

	for (i=0,iCount=0;i<dMAX_OBJECT && iCount <iFixedCount ;++i)
	{

		cFIXED_OBJECT_INFO	*lpObject	=  g_lpActmap->m_object.m_denFixedObject.get(i);
		
		if(!lpObject)	
			continue;
		
		if(lpObject->m_isSelected == TRUE)
		{
			m_pSelectData[iIndex].iIndex	=	i;
			m_pSelectData[iIndex].iX		=	lpObject->m_pos.x;
			m_pSelectData[iIndex].iY		=	lpObject->m_pos.y;
			m_pSelectData[iIndex].iClass	=	eSELECT_FIXED_OBJECT;
			m_pSelectData[iIndex].iObject	=	lpObject->m_wObject;
			memcpy(m_pSelectData[iIndex].aAddonObject,lpObject->m_aAddonObject,sizeof(cADD_ON_OBJECT) * dMAX_ADD_ON_OBJECT_COUNT);

			++iIndex;
			++iCount;
		}		
	}

	for (i=0,iCount=0;i<dMAX_OBJECT && iCount <iTinyCount ;++i)
	{
		cTINY_OBJECT_INFO	*lpObject	=  g_lpActmap->m_object.m_denTinyObject.get(i);
		
		if(!lpObject)	
			continue;
		
		if(lpObject->m_isSelected == TRUE)
		{
			m_pSelectData[iIndex].iIndex	=	i;
			m_pSelectData[iIndex].iX		=	lpObject->m_pos.x;
			m_pSelectData[iIndex].iY		=	lpObject->m_pos.y;
			m_pSelectData[iIndex].iClass	=	eSELECT_TINY_OBJECT;
			m_pSelectData[iIndex].iObject	=	lpObject->m_wObject;
			++iIndex;
			++iCount;
		}		
	}
	
	for(i=0;i<iActorCount;++i)
	{
		int iI = g_lpActmap->m_setActor.m_aSelectedActor[i];
		cACTOR* pAc = g_lpActmap->m_setActor.getActor(iI); 
		
		if(!pAc)
			continue;

		m_pSelectData[iIndex].iIndex	=	iI;
		m_pSelectData[iIndex].iX		=	pAc->m_pos.x;
		m_pSelectData[iIndex].iY		=	pAc->m_pos.y;
		m_pSelectData[iIndex].iClass	=	eSELECT_ACTOR_OBJECT;
		m_pSelectData[iIndex].iObject	=	pAc->m_wCharacter;
		++iIndex;
		
	}
}




void
cMAPWND::resetSelectData()				// 09.07.28
{
	pKILL(m_pSelectData);
}
void
cMAPWND::copyFixedSelectData(sSELECT_DATA* _pSelectData )				// 09.07.31
{
	if (g_lpActmap->m_object.getFixedObjectCount() > dMAX_OBJECT_COUNT)
		return;

	cPOINT pos = getLeftSelectDataPoint();			
		
	int x	= (_pSelectData->iX-pos.x)/dTILE_WIDTH*g_iScale/100;
	int y	= (_pSelectData->iY-pos.y)/dTILE_HEIGHT*g_iScale/100;
	
	x		=	x + Cursor.x + g_lpActmap->m_pos.x;
	y		=	y + Cursor.y + g_lpActmap->m_pos.y;
	
	cFIXED_OBJECT_INFO* lpOb = g_lpActmap->m_object.m_denFixedObject.getRookie();
	g_lpActmap->buildObject(_pSelectData->iObject,x,y);
	
	memcpy(lpOb->m_aAddonObject, _pSelectData->aAddonObject, sizeof(cADD_ON_OBJECT) * dMAX_ADD_ON_OBJECT_COUNT);
	g_lpActmap->LostVirgin();
}

void
cMAPWND::copyActorSelectData(sSELECT_DATA* _pSelectData)				// 09.07.28
{
	cPOINT pos = getLeftSelectDataPoint();
				
				
	cACTOR		actor;
	cACTOR*		pAc = g_lpActmap->m_setActor.getActor(_pSelectData->iIndex);
	if(!pAc)
		return;
	
	int x	= _pSelectData->iX-pos.x + m_posWorld.x;
	int y	= _pSelectData->iY-pos.y + m_posWorld.y;
				

	
	memcpy(&actor, pAc, sizeof(cACTOR));
	strcpy(actor.m_strName,pAc->getCharacter()->m_strName);
	actor.m_wJob		=	pAc->getCharacter()->m_wBaseJob;	//	직업 번호

	actor.m_isSelected	=	FALSE;
	actor.m_pos.x		=	x;
	actor.m_pos.y		=	y;
	actor.m_pCC = NULL;	
				
	int	iFocusArea		=	g_lpActmap->m_pArea->GetFocusArea(x,y);
				
	if (iFocusArea	==	0xffff)
		iFocusArea	=	0;

	actor.m_wMoveArea	=	iFocusArea;
			
	int iIndex = g_lpActmap->m_setActor.add(&actor);
	cACTOR* pAcc = g_lpActmap->m_setActor.getActor(iIndex);
	if(pAcc)		
		pAcc->deepCopy(pAc);

	g_lpActmap->LostVirgin();
				
}


void
cMAPWND::cutActorSelectData(sSELECT_DATA* _pSelectData)				// 09.07.28
{

	cPOINT pos = getLeftSelectDataPoint();
	
	cACTOR*		pAc = g_lpActmap->m_setActor.getActor(_pSelectData->iIndex);
	if(!pAc)
		return;
	int x	= _pSelectData->iX-pos.x + m_posWorld.x;
	int y	= _pSelectData->iY-pos.y + m_posWorld.y;
		
	pAc->m_pos.x		=	x;
	pAc->m_pos.y		=	y;
	
	g_lpActmap->LostVirgin();
}


void
cMAPWND::copyTinySelectData(sSELECT_DATA* _pSelectData)				// 09.07.28
{
	cPOINT pos = getLeftSelectDataPoint();
	
	if (g_lpActmap->m_object.getTinyObjectCount() > dMAX_OBJECT_COUNT)
		return;
				
				
	int x	= _pSelectData->iX-pos.x + m_posWorld.x;
	int y	= _pSelectData->iY-pos.y + m_posWorld.y;
				
	
	g_lpActmap->addTinyObject(_pSelectData->iObject,x,y);
	g_lpActmap->LostVirgin();


}


void
cMAPWND::copySelectData()				// 09.08.03
{
	

	if(m_bObjectArea == TRUE)
	{
		

		int x1 = g_lpSelectArea->x1;
		int x2 = g_lpSelectArea->x2;
		int y1 = g_lpSelectArea->y1;
		int y2 = g_lpSelectArea->y2;

		cRECT rt;
		
		char strName[64] = "";

		sprintf(strName, "%s - Copy", g_lpSelectArea->m_strName);

		int iIndex = g_lpActmap->m_pArea->Add(strName,&rt);
		
		g_lpActmap->m_pArea->m_aArea[iIndex].copy(g_lpSelectArea);
		g_lpActmap->m_pArea->m_aArea[iIndex].x1 = m_posWorld.x;
		g_lpActmap->m_pArea->m_aArea[iIndex].x2 = m_posWorld.x +x2-x1;
		g_lpActmap->m_pArea->m_aArea[iIndex].y1 = m_posWorld.y;
		g_lpActmap->m_pArea->m_aArea[iIndex].y2 = m_posWorld.y + y2-y1;

		m_isRepaint = TRUE;
		g_iWorkMode = dWORK_EDIT;
		m_bObjectCopy	= FALSE;	
		m_bObjectCut	= FALSE;	
		m_isRepaint		= TRUE;
		m_bObjectArea	= FALSE;

		return;
	}



	for(int i=0;i<m_iCopyCount;++i)
	{
		
		switch(m_pSelectData[i].iClass)
		{
			
		case eSELECT_FIXED_OBJECT:

			copyFixedSelectData(&m_pSelectData[i]);	
			
			break;
		case eSELECT_TINY_OBJECT:
			
			copyTinySelectData(&m_pSelectData[i]);
			
			break;
			
		case eSELECT_ACTOR_OBJECT:
			
			copyActorSelectData(&m_pSelectData[i]);			
			
			break;
		}	
		
	}

}

void
cMAPWND::drawSelectDataCursor()				// 09.08.03
{
	
	if(m_bObjectArea && g_lpSelectArea){
		
		int x1 = g_lpSelectArea->x1;
		int x2 = g_lpSelectArea->x2;
		int y1 = g_lpSelectArea->y1;
		int y2 = g_lpSelectArea->y2;

		cDRAW::Box(_LTGREEN, Mouse.x	,Mouse.y,
					Mouse.x +x2-x1	,Mouse.y + y2-y1);
		
		return;
	}

	cPOINT pos = getLeftSelectDataPoint();

	
	for(int i=0;i<m_iCopyCount;++i)
	{
		
		switch(m_pSelectData[i].iClass)
		{
			
		case eSELECT_FIXED_OBJECT:
			{
				
				int ox = (m_pSelectData[i].iX-pos.x)/dTILE_WIDTH;
				int oy = (m_pSelectData[i].iY-pos.y)/dTILE_HEIGHT;
				
				int	x	=	(ox+Cursor.x)*dTILE_WIDTH/dSHARE_TILE + dTILE_WIDTH/dSHARE_TILE /2;
				int	y	=	(oy+Cursor.y)*dTILE_HEIGHT/dSHARE_TILE+ dTILE_HEIGHT/dSHARE_TILE /2+g_iTBSize;
				
				int iCrashX = ox + Cursor.x + g_lpActmap->m_pos.x;
				int iCrashY = oy + Cursor.y + g_lpActmap->m_pos.y;
				BOOL	crash	=	g_lpActmap->isCrashFixedObject(m_pSelectData[i].iObject,iCrashX,iCrashY);
				g_lpActmap->m_object.putFixedObject(m_pSelectData[i].iObject,x,y,crash);		
				
				
			}
			break;

		case eSELECT_TINY_OBJECT:
			{
				
				int x	= m_pSelectData[i].iX-pos.x;
				int y	= m_pSelectData[i].iY-pos.y;

				int iCrashX		= x + m_posWorld.x;
				int iCrashY		= y + m_posWorld.y;

				x	=	x + Mouse.x;
				y	=	y + Mouse.y;

				BOOL	crash	=	FALSE;
				
				if	(g_lpActmap->m_object.m_iTinyObjectImageCount	>	0)
					crash	=	g_lpActmap->m_object.isCrashTinyObject(m_pSelectData[i].iObject ,iCrashX,iCrashY);
				
				g_lpActmap->m_object.putTinyObject(m_pSelectData[i].iObject,x,y,crash);				

			}	
			break;

		case eSELECT_ACTOR_OBJECT:
			{



				
				cACTOR*		pAc = g_lpActmap->m_setActor.getActor(m_pSelectData[i].iIndex);
				
				int x	= m_pSelectData[i].iX-pos.x;
				int y	= m_pSelectData[i].iY-pos.y;

				int iCrashX		= x + m_posWorld.x;
				int iCrashY		= y + m_posWorld.y;
							

				x	=	x + Mouse.x;
				y	=	y + Mouse.y;

				BOOL	isCrash	=	g_lpActmap->m_setActor.isCrash(iCrashX,iCrashY);
				
				cCHARACTER	*lpCharacter	=	pAc->getCharacter();		//	선택된 직업을 리턴
				cJOB		*lpJob			=	lpCharacter->getJob();
				int			iBody			=	pAc->getBody();
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
			}
			break;
			
		}	
		
	}


}
void				
cMAPWND::cutSelectData()				// 09.07.28
{
	for(int i=0;i<m_iCopyCount;++i)
	{
		
		switch(m_pSelectData[i].iClass)
		{
			case eSELECT_FIXED_OBJECT:
				copyFixedSelectData(&m_pSelectData[i]);	
				break;

			case eSELECT_TINY_OBJECT:
				copyTinySelectData(&m_pSelectData[i]);
				break;
			
			case eSELECT_ACTOR_OBJECT:
				cutActorSelectData(&m_pSelectData[i]);			
				break;
		}	
		
		
	}
}

cPOINT
cMAPWND::getLeftSelectDataPoint()	// 09.07.28
{
	
	int iLeftX = 100000;
	int iLeftY = 0;
	for(int iCount=0;iCount<m_iCopyCount;++iCount)
	{
		if(iLeftX>m_pSelectData[iCount].iX)
		{
			iLeftX = m_pSelectData[iCount].iX;
			iLeftY = m_pSelectData[iCount].iY;
		}
	}
	
	return cPOINT(iLeftX, iLeftY);
}