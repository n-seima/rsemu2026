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

#define	dSTATIC_OBJECT_COUNT_IN_LINE	4
#define	dFREE_OBJECT_COUNT_IN_LINE		4

//
//	고정된 오브젝트 갱신(선택 했나 본다는거다)
void
cTOOLWND::UpdateStaticObjects()
{
	if	(g_iWorkMode == dWORK_EDIT)
	{	
		if	(g_iPreWorkMode	!=	dWORK_STATIC_OBJECT)
			return;
	}
	else
	if	(g_iWorkMode		!=	dWORK_STATIC_OBJECT)
		return;

	if	(g_lpActmap->m_object.m_iFixedObjectImageCount	<=	0)
		return;

	int	gab		=	22;
	int	width	=	(client.x2-gab) / dSTATIC_OBJECT_COUNT_IN_LINE;
	int	height	=	width;
	int	index	=	StaticPos*dSTATIC_OBJECT_COUNT_IN_LINE;

	gab			/=	(dSTATIC_OBJECT_COUNT_IN_LINE+1);
	int	iTop	=	((client.y2-g_iTBSize-gab)%(height+gab))/2;

	if (TOUCHLBUTTON)
	{	
		cRECT_SIZE	rect;

		for (int i=0;index<g_lpActmap->m_object.m_iFixedObjectImageCount;i++,index++)
		{	
			int xp	=	(i%dSTATIC_OBJECT_COUNT_IN_LINE)*(width+gab)+gab;
			int yp	=	g_iTBSize+(i/dSTATIC_OBJECT_COUNT_IN_LINE)*(height+gab)+iTop;

			if	(yp +height+1 >= client.y2)
				break;

			rect.Set(xp-1,yp-1,width+2,height+2);

			if (rect.isIN(Mouse.x,Mouse.y))
			{
				g_lpActmap->m_object.selectFixedObject(index,KEY[dKEY_SHIFT],KEY[dKEY_CONTROL]);
				MAIN.ChangeMode(dWORK_STATIC_OBJECT);
				SaveScrollPos(index);
				MAIN.Draw();
				break;
			}
		}
	}

	if(KEY[dKEY_ALT] && ! g_lpActmap->m_bf1IsLocked)		// 09.07.27 오브젝트교체..
	{
		if(TOUCHLBUTTON)
		{
			int	iSelectObjectCount	=	0;
			int	iObjectCount		=	g_lpActmap->m_object.m_denFixedObject.getCount();
			int	iCount=0;

			for (int iIndex=0;iIndex<dMAX_OBJECT&& iCount < iObjectCount;iIndex++)
			{
				cFIXED_OBJECT_INFO	*lpObject	=	g_lpActmap->m_object.m_denFixedObject.get(iIndex);
				
				if	(!lpObject)
					continue;
				
				iCount++;
				
				if	(!lpObject->m_isSelected)
					continue;
				
				lpObject->m_wObject = g_lpActmap->m_object.m_iSelectFixedObject;
				MAIN.m_bBuffer = TRUE;
				
			}
		}
	}
}	//	cTOOLWND::UpdateStaticObjects()

//
//	고정된 오브젝트 들을 그려준다.
void
cTOOLWND::DrawStaticObjects()
{	if (g_iWorkMode == dWORK_EDIT){	if (g_iPreWorkMode !=	dWORK_STATIC_OBJECT)	return;}
	else							if (g_iWorkMode	!=	dWORK_STATIC_OBJECT)	return;

	cDRAW::Fill(0);

	if (g_lpActmap->m_object.m_iFixedObjectImageCount	<=	0)	return;

	int	gab		=	22;
	int	width	=	(cDRAW::WIDTH -gab) / dSTATIC_OBJECT_COUNT_IN_LINE;
	int	height	=	width;
	int	index	=	StaticPos*dSTATIC_OBJECT_COUNT_IN_LINE;

	gab			/=	(dSTATIC_OBJECT_COUNT_IN_LINE+1);

	TPH			=	(cDRAW::HEIGHT - g_iTBSize - gab/3) / (height+gab/3)-2;

	SetScrollBar(g_lpActmap->m_object.m_iFixedObjectImageCount / dSTATIC_OBJECT_COUNT_IN_LINE,&StaticPos);

	int	iTop	=	((cDRAW::HEIGHT-g_iTBSize-gab)%(height+gab))/2;

	for (int i=0;index < g_lpActmap->m_object.m_iFixedObjectImageCount;i++,index++)
	{
		int xp	=	(i%dSTATIC_OBJECT_COUNT_IN_LINE)*(width+gab)+gab;
		int yp	=	g_iTBSize+(i/dSTATIC_OBJECT_COUNT_IN_LINE)*(height+gab)+iTop;

		if (yp +height+1 >= cDRAW::HEIGHT	)	break;

		if (index == g_lpActmap->m_object.m_iSelectFixedObject)
		{
			xp++,yp++;
			cDRAW::Fill(_LTBLUE,xp-1,yp-1,xp+width+1,yp+height+1);
		}
		else
		if (g_lpActmap->m_object.isSelectFixedObject(index))
			cDRAW::Fill(_BLUE,xp-1,yp-1,xp+width+1,yp+height+1);

		g_lpActmap->m_object.putFixedObject(index,xp,yp,width,height);

		if (index == g_lpActmap->m_object.m_iSelectFixedObject)	
			cDRAW::Box(0xffff,xp-1,yp-1,xp+width+1,yp+height+1);
		else
			cDRAW::Box(0xffff,xp-1,yp-1,xp+width+1,yp+height+1);

		if	(g_lpActmap->m_object.isDoorObject(index))
			cTEXT::Put(xp+1,yp+1,RGB(255,255,255),"DOOR");
		else
		{
			cTEXT::OutlineOn(0);
			cTEXT::Put(xp+1,yp+1,RGB(255,255,255),_ms("%d]",index));
			cTEXT::OutlineOff();
		}
	}
}

//
//	바닥에 깔리는 오브젝트들을 업데이트 한다.
void
cTOOLWND::UpdateFreeObjects()
{	if (g_iWorkMode == dWORK_EDIT){	if (g_iPreWorkMode	!=	dWORK_FREE_OBJECT)	return;}
	else							if (g_iWorkMode		!=	dWORK_FREE_OBJECT)	return;

	if (g_lpActmap->m_object.m_iTinyObjectImageCount	<=	0)	return;

	int	gab		=	32;
	int	iCount	=	g_lpActmap->m_object.m_iTinyObjectImageCount;
	int	width	=	(client.x2-gab) / dFREE_OBJECT_COUNT_IN_LINE;
	int	height	=	width;
	int	index	=	FreePos*dFREE_OBJECT_COUNT_IN_LINE;

	gab			/=	(dFREE_OBJECT_COUNT_IN_LINE+1);
	int	iTop	=	((client.y2-g_iTBSize-gab)%(height+gab))/2;

	if (TOUCHLBUTTON)		//	왼쪽 버튼이 눌렸다.
	{	cRECT_SIZE	rect;

		for (int i=0;index<iCount	;i++,index++)
		{	int xp	=	(i%dFREE_OBJECT_COUNT_IN_LINE)*(width+gab)+gab;
			int yp	=	g_iTBSize+(i/dFREE_OBJECT_COUNT_IN_LINE)*(height+gab)+iTop;

			if (yp +height+1 >= client.y2		)	break;	//	영역을 벗어난다.

			rect.Set(xp-1,yp-1,width+2,height+2);

			if (rect.isIN(Mouse.x,Mouse.y))
			{
				g_lpActmap->m_object.selectTinyObject(index,KEY[dKEY_SHIFT],KEY[dKEY_CONTROL]);

				MAIN.ChangeMode(dWORK_FREE_OBJECT);
				SaveScrollPos(index);
				MAIN.Draw();
				break;
			}
		}
	}
}	//	cTOOLWND::UpdateFreeObjects()

//
//	바닥에 깔리는 오브젝트 들을 그려준다.
void
cTOOLWND::DrawFreeObjects()
{
	if (g_iWorkMode == dWORK_EDIT)
	{
		if (g_iPreWorkMode !=	dWORK_FREE_OBJECT)	
			return;
	}
	else
	if (g_iWorkMode	!=	dWORK_FREE_OBJECT)
		return;

	cDRAW::Fill(0);

	if (g_lpActmap->m_object.m_iTinyObjectImageCount	<=	0)	return;

	int	gab		=	32;
	int	iCount	=	g_lpActmap->m_object.m_iTinyObjectImageCount;
	int	width	=	(cDRAW::WIDTH -gab) / dFREE_OBJECT_COUNT_IN_LINE;
	int	height	=	width;
	int	index	=	FreePos*dFREE_OBJECT_COUNT_IN_LINE;

	gab			/=	(dFREE_OBJECT_COUNT_IN_LINE+1);
	int	iTop	=	((cDRAW::HEIGHT-g_iTBSize-gab)%(height+gab))/2;

	TPH			=	(cDRAW::HEIGHT - g_iTBSize - gab) / (height+gab)-2;

	SetScrollBar(iCount / dFREE_OBJECT_COUNT_IN_LINE,&FreePos);

	for (int i=0;index<iCount;i++,index++)
	{	int xp	=	(i%dFREE_OBJECT_COUNT_IN_LINE)*(width+gab)+gab;
		int yp	=	g_iTBSize+(i/dFREE_OBJECT_COUNT_IN_LINE)*(height+gab)+iTop;

		if (yp +height+1		>=	cDRAW::HEIGHT		)	break;

		if (index == g_lpActmap->m_object.m_iSelectTinyObject)
		{
			xp	++;
			yp	++;
			cDRAW::Fill(_LTBLUE,xp-1,yp-1,xp+width+1,yp+height+1);
		}
		else
		if (g_lpActmap->m_object.isSelectTinyObject(index))
			cDRAW::Fill(_BLUE,xp-1,yp-1,xp+width+1,yp+height+1);

		g_lpActmap->m_object.putTinyObject(index,xp+1,yp+1,width-2,height-2);

		if (index == g_lpActmap->m_object.m_iSelectTinyObject)
			cDRAW::Box(_LTGREEN	,xp-1,yp-1,xp+width+1,yp+height+1);
		else
			cDRAW::Box(0xffff	,xp-1,yp-1,xp+width+1,yp+height+1);

		{
			cTEXT::OutlineOn(0);
			cTEXT::Put(xp+1,yp+1,RGB(255,255,255),_ms("%d]",index));
			cTEXT::OutlineOff();
		}
//		cTEXT::cPut(xp+(width+1)/2,yp+height-14,0,RGB(255,255,255),OBJ_DATA[i+g_lpActmap->m_object.m_iFixedObjectImageCount].Name);
	}
}


void 
cTOOLWND::SelectObjects()
{	
	// dTB_STATIC_OBJECT, dTB_FREE_OBJECT, dTB_HOUSE 일때만 실행되도록
	if (m_WorkButton.IsCheck(dTB_STATIC_OBJECT) == TRUE)
	{		
		InputKeypad(dTB_STATIC_OBJECT);		
	}

	if ( m_WorkButton.IsCheck(dTB_FREE_OBJECT) == TRUE )
	{
		InputKeypad(dTB_FREE_OBJECT);

	}

	if (m_WorkButton.IsCheck(dTB_HOUSE) == TRUE)
	{	
		InputKeypad(dTB_HOUSE);
	}
}


void 
cTOOLWND::InputKeypad(int _iType)
{
	if (m_iInputCnt == 0)
	{
		memset(m_strStaticCtrStr,0 ,sizeof(m_strStaticCtrStr));
	}
	if (m_iInputCnt == 4)	// enter 입력 없이 숫자만 입력했을 경우 4자리가 되면 자리수를 1로!
	{
		m_iInputCnt = 1;
		m_abInput[0] = m_abInput[3];
		m_strStaticCtrStr[0] = m_strStaticCtrStr[3];
		for(int i=1 ; i<4 ; i++)
			m_strStaticCtrStr[i] = ' ';
		SetWindowText(g_lpcToolBar->m_hStatic2, m_strStaticCtrStr);
	}


	for(int i=0 ; i<=9 ; i++)
	{
		if(TOUCHKEY[dKEY_NUMPAD0+i])
		{
			m_abInput[m_iInputCnt] = i;
			m_iInputCnt++;
			itoa(i, &m_strStaticCtrStr[m_iInputCnt-1], 10);
			SetWindowText(g_lpcToolBar->m_hStatic2, m_strStaticCtrStr);
		}
	}

	if (TOUCHKEY[0x0D])
	{
		// Enter 입력시 작업 시행
		m_iIndex = 0;

		m_iIndex = atoi(m_strStaticCtrStr);
		
		if (_iType == dTB_STATIC_OBJECT)
		{
			if (m_iIndex > g_lpActmap->m_object.m_iFixedObjectImageCount-1)
			return;

			g_lpActmap->m_object.selectFixedObject(m_iIndex, KEY[dKEY_SHIFT],KEY[dKEY_CONTROL]);
			MAIN.ChangeMode(dWORK_STATIC_OBJECT);
			MAIN.Draw();
		}else if (_iType == dTB_FREE_OBJECT)
		{
			if (m_iIndex > g_lpActmap->m_object.m_iTinyObjectImageCount-1)
				return;

			g_lpActmap->m_object.selectTinyObject(m_iIndex, KEY[dKEY_SHIFT], KEY[dKEY_CONTROL]);
			MAIN.ChangeMode(dWORK_FREE_OBJECT);
			MAIN.Draw();
		}else if (_iType == dTB_HOUSE)
		{
			if (m_iIndex > g_lpActmap->getBuildingImageCount())
			{
				return;
			}

			g_lpActmap->selectBuilding(m_iIndex);
			MAIN.ChangeMode(dWORK_BUILDING);
			MAIN.Draw();
		}
		
		// 건물 오브젝트의 경우 한줄에 나타나는 오브젝트수가 다르므로 다르게 처리한다.
		if (_iType == dTB_STATIC_OBJECT || _iType == dTB_FREE_OBJECT)
		{
			int iCnt = m_iIndex - (m_iIndex % dSTATIC_OBJECT_COUNT_IN_LINE);
			iCnt = (iCnt-m_iPreInput)/4;
			
			if (iCnt > 0)
			{
				for(int i=0 ; i < iCnt ; i++)
				{
					VScroll(SB_LINEDOWN);
				}
			}else
			{
				for(int i=0 ; i > iCnt ; i--)
				{
					VScroll(SB_LINEUP);
				}
			}
			m_iPreInput = m_iIndex;	// 이전 선택된 오브젝트 확인을 위해 저장을 해 둔다.
			m_iPreInput = m_iPreInput - (m_iPreInput % dSTATIC_OBJECT_COUNT_IN_LINE);
		}else if (_iType == dTB_HOUSE)
		{
			int iCnt = m_iIndex - m_iPreInput;

			if (iCnt > 0)
			{
				for(int i=0 ; i < iCnt ; i++)
				{
					VScroll(SB_LINEDOWN);
				}
			}else
			{
				for(int i=0 ; i > iCnt ; i--)
				{
					VScroll(SB_LINEUP);
				}
			}
			m_iPreInput = m_iIndex;	// 이전 선택된 오브젝트 확인을 위해 저장을 해 둔다.
		}

		// 완료후엔 m_iInputCnt 0으로 m_strStaticCtrStr 공백으로 초기화
		m_iInputCnt = 0;
		//strcpy(m_strStaticCtrStr, "   ");
		memset(m_strStaticCtrStr, 0, sizeof(m_strStaticCtrStr));
	}
}




void
cTOOLWND::SaveScrollPos(int _iScrollPos)
{
				// 마우스를 클릭해서 오브젝트를 선택해도 static 내용이 바뀌어야 하므로 ..				
	memset(m_strStaticCtrStr, 0, sizeof(m_strStaticCtrStr));
	wsprintf(m_strStaticCtrStr, "%d", ScrollPos);	
	SetWindowText(g_lpcToolBar->m_hStatic2, m_strStaticCtrStr);
	
	// 몇번째 줄의 오브젝트를 선택했는지 계산해 둔다.
	// 그래야 마우스나 키보드로 오브젝트를 선택할 때 정확하게 스크롤 된다.
	m_iIndex = _iScrollPos;
	m_iPreInput = _iScrollPos;
	m_iPreInput = m_iPreInput - (m_iPreInput % dSTATIC_OBJECT_COUNT_IN_LINE);
}