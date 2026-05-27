#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cTOOLWND.H"
#include	"cMAIN.H"
#include	"cOBJECT.H"
#include	"cBUILDING.H"
#include	"cAREA.H"
#include	"cMAPWND.H"
#include	"cMESSAGE.H"
#include	"cSET.H"

void
cTOOLWND::beginEditBlockOfBuilding(int _iBuilding)
{
	cBUILDING_INFO	*lpBuilding	=	g_lpActmap->getBuilding(_iBuilding);
	if (!lpBuilding)	return;

	g_lpActmap->freeSelectedAllBuilding();

	lpBuilding->m_isSelected	=	TRUE;
	m_iSelectEditBlockBuilding	=	_iBuilding;
	m_isEditBlockOfBuilding		=	TRUE;

	g_lpActmap->updateAddonObject();
}

void
cTOOLWND::endEditBlockOfBuilding()
{
	cBUILDING_INFO	*lpBuilding	=	g_lpActmap->getBuilding(m_iSelectEditBlockBuilding);
	if (!lpBuilding)	return;

	g_lpActmap->freeSelectedAllBuilding();

	m_isEditBlockOfBuilding		=	FALSE;

	g_lpActmap->updateAddonObject();
}

void
cTOOLWND::UpdateBuildings()
{	if (g_iWorkMode == dWORK_EDIT) {if (g_iPreWorkMode !=	dWORK_BUILDING)	return;}
	else							if (g_iWorkMode	!=	dWORK_BUILDING)	return;

	if (g_lpActmap->getBuildingImageCount()	<=	0)	return;

	if (m_isEditBlockOfBuilding)
	{	UpdateBuildingBlocks();
		return;
	}

	int	gab		=	20;
	int	numbers	=	1;

	int	width	=	(cWND::width()-gab) / numbers;
	int	height	=	width;
	int	index	=	BuildingPos*numbers;

	gab			/=	(numbers+1);

	int	iTop	=	((cWND::height()-g_iTBSize-gab)%(height+gab))/2;

	if (TOUCHLBUTTON)
	{	cRECT_SIZE	rect;

		for (int i=0;index<g_lpActmap->getBuildingImageCount();i++,index++)
		{	int xp	=	(i%numbers)*(width+gab)+gab;
			int yp	=	g_iTBSize+(i/numbers)*(height+gab)+iTop;

			if (yp +height+1 >= cWND::height()		)	break;

			rect.Set(xp-1,yp-1,width+2,height+2);

			if (rect.isIN(Mouse.x,Mouse.y))
			{	g_lpActmap->m_iSelectBuilding	=	index;
				MAIN.ChangeMode(dWORK_BUILDING);
				SaveScrollPos(index);
				MAIN.Draw();
				break;
			}
			
		}
	}
}

//
//	건물들을 그려준다.
void
cTOOLWND::DrawBuildings()
{
	if (g_iWorkMode == dWORK_EDIT)	{if (g_iPreWorkMode !=	dWORK_BUILDING)	return;}
	else							if (g_iWorkMode	!=	dWORK_BUILDING)	return;

	if (m_isEditBlockOfBuilding)
	{	DrawBuildingBlocks();
		return;
	}

	cDRAW::Fill(0);

	if (g_lpActmap->getBuildingImageCount()	<=	0)	return;

	int	gab		=	20;
	int	numbers	=	1;

	int	width	=	(cWND::width() -gab) / numbers;
	int	height	=	width;
	int	index	=	BuildingPos*numbers;

	gab			/=	(numbers+1);

	TPH			=	(cWND::height() - g_iTBSize - gab) / (height+gab)-2;
	int	iTop	=	((cWND::height()-g_iTBSize-gab)%(height+gab))/2;

	SetScrollBar(g_lpActmap->getBuildingImageCount()/ numbers,&BuildingPos);
	
	for (int i=0;index<g_lpActmap->getBuildingImageCount();i++,index++)
	{	int xp	=	(i%numbers)*(width+gab)+gab;
		int yp	=	g_iTBSize+(i/numbers)*(height+gab)+iTop;

		if (yp +height+1			>=	cWND::height()		)	break;

		if (index == g_lpActmap->m_iSelectBuilding)	cDRAW::Fill(_LTBLUE,xp-1,yp-1,xp+width+1,yp+height+1);
		else										cDRAW::Fill(_LTGRAY	,xp-1,yp-1,xp+width+1,yp+height+1);

		g_lpActmap->putBuilding(index,xp,yp,width,height);

		if (index == g_lpActmap->m_iSelectBuilding)	cDRAW::Box(0xffff,xp-1,yp-1,xp+width+1,yp+height+1);
		else										cDRAW::Box(0xffff,xp-1,yp-1,xp+width+1,yp+height+1);
	}
}	//	cTOOLWND::DrawBuildings()

//
//	빌딩 블럭 업데이트
void
cTOOLWND::UpdateBuildingBlocks()
{
	int	gab		=	20;
	int	numbers	=	3;

	if (g_lpActmap->getBuildingImageCount()	<=	0)	return;

	int	width	=	(cWND::width()-gab) / numbers;
	int	height	=	width;
	int	index	=	m_iBuildingBlockPos*numbers;

	gab			/=	(numbers+1);

//	int	iTop	=	((cWND::height()-g_iTBSize-gab)%(height+gab))/2;
	int	iTop	=	((cWND::height()-g_iTBSize-gab)%(height+gab))/2;

	cBUILDING_INFO	*lpBuilding	=	g_lpActmap->getBuilding(m_iSelectEditBlockBuilding);

	if (!lpBuilding)
	{
		m_isEditBlockOfBuilding	=	FALSE;
		return;
	}

	cOBJ_ANM*		lpImage		=	g_lpActmap->getBuildingImage(lpBuilding->m_wBuilding);

	int	iCount	=	lpImage->Count;

	if (TOUCHLBUTTON)
	{	cRECT_SIZE	rect;

		for (int i=0;index<iCount;i++,index++)
		{	int xp	=	(i%numbers)*(width+gab)+gab;
			int yp	=	g_iTBSize+(i/numbers)*(height+gab)+iTop;

			if (yp +height+1 >= cWND::height()		)	break;

			rect.Set(xp-1,yp-1,width+2,height+2);

			if (rect.isIN(Mouse.x,Mouse.y))
			{
				if (index	==	0)	lpBuilding->toggleShadow();
				else				lpBuilding->toggleBlock(index);

				MAIN.Draw();
				break;
			}
			
		}
	}
}	//	cTOOLWND::UpdateBuildingBlocks()

//
//	건물들을 그려준다.
void
cTOOLWND::DrawBuildingBlocks()
{
	cDRAW::Fill(0);

	if (g_lpActmap->getBuildingImageCount()	<=	0)	return;

	cBUILDING_INFO	*lpBuilding	=	g_lpActmap->getBuilding(m_iSelectEditBlockBuilding);
	
	if (!lpBuilding)
	{
		m_isEditBlockOfBuilding	=	FALSE;
		return;
	}

	cOBJ_ANM*		lpImage		=	g_lpActmap->getBuildingImage(lpBuilding->m_wBuilding);

	if (!lpBuilding)	return;

	int	gab		=	20;
	int	numbers	=	3;
	int	width	=	(cWND::width() -gab) / numbers;
	int	height	=	width;
	int	index	=	m_iBuildingBlockPos*numbers;
	int	iCount	=	lpImage->Count;

	gab			/=	(numbers+1);

	TPH			=	(cWND::height()-g_iTBSize- gab) / (height+gab)-2;

	int	iTop	=	((cWND::height()-g_iTBSize-gab)%(height+gab))/2;

	SetScrollBar(iCount/ numbers,&m_iBuildingBlockPos);

	for (int i=0;index<iCount;i++,index++)
	{
		int	iIndex	=	index;
		int xp		=	(i%numbers)*(width+gab)+gab;
		int yp		=	g_iTBSize+(i/numbers)*(height+gab)+iTop;

		if (yp +height+1			>=	cWND::height()		)	break;

		if (iIndex	==	0)
		{
			if (lpBuilding->m_isPutShadow)	cDRAW::Fill(_LTBLUE,xp-1,yp-1,xp+width+1,yp+height+1);

			cTEXT::cPut(xp+width/2,yp+height/2-6,WHITE,dMSG_SHADOW);

			if (lpBuilding->m_isPutShadow)	cDRAW::Box(0xffff,xp-1,yp-1,xp+width+1,yp+height+1);
			else							cDRAW::Box(0xffff,xp-1,yp-1,xp+width+1,yp+height+1);
		}
		else
		{
			if (lpBuilding->isCheckedBlock(iIndex))	cDRAW::Fill(_LTBLUE,xp-1,yp-1,xp+width+1,yp+height+1);

			g_lpActmap->putBuilding(lpBuilding->m_wBuilding,iIndex,xp,yp,width,height);

			if (lpBuilding->isCheckedBlock(iIndex))	cDRAW::Box(0xffff,xp-1,yp-1,xp+width+1,yp+height+1);
			else									cDRAW::Box(0xffff,xp-1,yp-1,xp+width+1,yp+height+1);
		}
	}
}	//	cTOOLWND::DrawBuildings()
