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


//
//	에디트 모드다.
void
cMAPWND::UpdateEdit()		
{
	if (g_iWorkMode	!=	dWORK_EDIT && g_iWorkMode	!=	dWORK_AREA)
		return;

	if (g_iWorkMode	==	dWORK_EDIT &&	TOUCHRBUTTON	&&	client.isIN(Mouse.x,Mouse.y))
	{
		int	iFocusBuilding	=	0xffff;

		if (g_iPreWorkMode	==	dWORK_BUILDING)
		{
			iFocusBuilding	=	g_lpActmap->getFocusBuilding((Cursor.x+g_lpActmap->m_pos.x),(Cursor.y+g_lpActmap->m_pos.y));

			if (iFocusBuilding	!=	0xffff)
			{
				if (!TOOLWND.m_isEditBlockOfBuilding)
				{
					TOOLWND.beginEditBlockOfBuilding(iFocusBuilding);

					MAIN.Draw();

					return;
				}
				else
				{
					if (TOOLWND.m_iSelectEditBlockBuilding	==	iFocusBuilding)
					{
						TOOLWND.endEditBlockOfBuilding();

						MAIN.Draw();

						return;
					}

					TOOLWND.beginEditBlockOfBuilding(iFocusBuilding);

					MAIN.Draw();

					return;
				}
			}
		}

		if (iFocusBuilding	==	0xffff)
		{
			if (TOOLWND.m_isEditBlockOfBuilding)
			{
				TOOLWND.endEditBlockOfBuilding();

				MAIN.Draw();

				return;
			}

			popEditMenu();
		}

		return;
	}

	if	(g_iWorkMode	==	dWORK_AREA )
		if (g_lpActmap->m_pArea->m_isMoveArea + g_lpActmap->m_pArea->m_isResizeArea > 0)
		{
			bLININGBOX	=	FALSE;
			return;
		}

	if	(TOUCHKEY[dKEY_TAB])
	{
		g_lpActmap->cycleSelectAddonObject();
		m_isRepaint	=	TRUE;
	}

	if (KEY[dKEY_SHIFT])
	{

		if (KEY[dKEY_UP])
			m_isRepaint	+=	g_lpActmap->moveSelectActor(0,-1);
		if (KEY[dKEY_LEFT])
			m_isRepaint	+=	g_lpActmap->moveSelectActor(1,0);
		if (KEY[dKEY_RIGHT])
			m_isRepaint	+=	g_lpActmap->moveSelectActor(-1,0);
		if (KEY[dKEY_DOWN])	
			m_isRepaint	+=	g_lpActmap->moveSelectActor(0,1);

		if	(TOUCHKEY[dKEY_DELETE])
		{	
			g_lpActmap->removeSelectAddonObject();
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
		if (client.isIN(Mouse.x,Mouse.y))
			if (TOOLWND.m_isEditBlockOfBuilding)
			{
				TOOLWND.endEditBlockOfBuilding();
				TOOLWND.Draw();
			}

		if (bLININGBOX)
		{	if (LiningBox.x2	!=	Mouse.x)	bDRAWCURSOR	=	TRUE;
			if (LiningBox.y2	!=	Mouse.y)	bDRAWCURSOR	=	TRUE;

			LiningBox.x2	=	Mouse.x+g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
			LiningBox.y2	=	Mouse.y+g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE;
		}
		else
		if (client.isIN(Mouse.x,Mouse.y))
		{	LiningBox.x1	=	Mouse.x+g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
			LiningBox.y1	=	Mouse.y+g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE;
			LiningBox.x2	=	LiningBox.x1;
			LiningBox.y2	=	LiningBox.y1;
			BufferRect		=	LiningBox;
			BufferRect.Refit();

			bLININGBOX		=	TRUE;
			bDRAWCURSOR		=	TRUE;
			m_isRepaint		=	TRUE;
		}
	}
	else
	{
		if (bLININGBOX)
		{
			LiningBox.y1	-=	g_iTBSize;
			LiningBox.y2	-=	g_iTBSize;

			if	(LiningBox.x1 > LiningBox.x2)
				swap(LiningBox.x1,LiningBox.x2);
			if	(LiningBox.y1 > LiningBox.y2)
				swap(LiningBox.y1,LiningBox.y2);

			bLININGBOX	=	FALSE;
			m_isRepaint	=	TRUE;

			for (int i=0;i<4;i++)
				pKILL(LineBuffer[i]);

			cRECT	screen;
			screen.x1	=	g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
			screen.y1	=	g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE;
			screen.x2	=	screen.x1+WIDTH;
			screen.y2	=	screen.y1+HEIGHT-g_iTBSize;

			if	(g_iWorkMode==	dWORK_EDIT)
			{
				cRECT	rect;

				rect.Set(LiningBox.x1,LiningBox.y1,LiningBox.x2,LiningBox.y2);

				rect.x1			-=	g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
				rect.y1			-=	(g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE);
				rect.x2			-=	g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
				rect.y2			-=	(g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE);

				LiningBox.x1	=	g_lpActmap->m_pos.x*dTILE_WIDTH /dSHARE_TILE+rect.x1*100/g_iScale;
				LiningBox.y1	=	g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE+rect.y1*100/g_iScale;
				LiningBox.x2	=	g_lpActmap->m_pos.x*dTILE_WIDTH /dSHARE_TILE+rect.x2*100/g_iScale;
				LiningBox.y2	=	g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE+rect.y2*100/g_iScale;

//				if (g_iScale	==	100)	
				g_lpActmap->select(&LiningBox,&screen);
			}

			if	(g_iWorkMode==	dWORK_AREA)
				if	(LiningBox.width() > 5 && LiningBox.height() > 5)
					cSET_AREA::SetArea(hINST,hWND,&LiningBox);
		}

		if	(!KEY[dKEY_SHIFT])
			if	(KEY[dKEY_DELETE] && !g_lpActmap->m_bf1IsLocked)
			{	
				if (g_iWorkMode==	dWORK_EDIT)
				{
					g_lpActmap->removeSelectedObject();
					g_lpActmap->removeSelectedBuilding();
					g_lpActmap->m_setActor.removeSelectedActors();
					m_isRepaint	=	TRUE;

					g_lpActmap->LostVirgin();
				}

				if (g_iWorkMode==	dWORK_AREA)
				{
					if (cMSG::YESNO(dMSG_REMOVE_SELECT_AREA,dMSG_QUESTION_CONTINUE) == IDYES)
					{
						if (g_lpActmap->m_pArea->Del())
						{
							m_isRepaint	=	TRUE;
							g_lpActmap->LostVirgin();
						}
					}
				}

				TOOLWND.Draw();
			}
	}

	if	(TOUCHKEY[0x43] || TOUCHKEY[0x58])
	{			// 09.07.09    c키.  복사키~~~~ x키 잘라내기.
  		saveSelectData();				// 09.07.28

  		if	(m_iCopyCount>0)
  		{
  			g_iWorkMode = dWORK_COPYORCUT;
  
  			if	(TOUCHKEY[0x43])
  			{
  				m_bObjectCopy = TRUE;
  			}
  			else
  			{
  				m_bObjectCut = TRUE;
  			}			
  		}
	}
}

//
//	편집 팝업 메뉴를 띄운다.
void
cMAPWND::popEditMenu()
{
	m_hMenu	=	CreatePopupMenu();
//	m_hMenu	hSelectPopup=	CreatePopupMenu();
//	AppendMenu(hSelectPopup, MF_STRING, IDM_SELECT_ALL			,dMSG_SELECT_ALL);
//	AppendMenu(hSelectPopup, MF_STRING, IDM_SELECT_CHARACTER	,dMSG_SELECT_CHARACTER);
//	AppendMenu(hSelectPopup, MF_STRING, IDM_SELECT_OBJECT		,dMSG_SELECT_ALL_OBJECT);
//	AppendMenu(hSelectPopup, MF_STRING, IDM_SELECT_BUILDING		,dMSG_SELECT_ALL_BUILDING);

	POINT	pos;

	AppendMenu(m_hMenu,MFT_STRING			,IDM_PREVIOUS_WORK		,dMSG_CANCEL_EDIT);
	AppendMenu(m_hMenu,MFT_SEPARATOR		,0						,NULL);
	AppendMenu(m_hMenu,MF_STRING			,IDM_SELECT_CHARACTER	,dMSG_SELECT_CHARACTER);
	//AppendMenu(m_hMenu,MF_POPUP			,(UINT) hSelectPopup	,dMSG_SELECT);


//	캐릭터 편집 관련
	{
		int	iCount	=	g_lpActmap->m_setActor.getSelectedActor();

		if (iCount	>=	1)
		{
			
			AppendMenu(m_hMenu,MF_STRING			,IDM_SELECT_CHARACTER_SAME_KIND	,dMSG_SELECT_SAME_KIND);
			AppendMenu(m_hMenu,MFT_SEPARATOR		,0					,NULL);
			if (iCount	>=	2)
				AppendMenu(m_hMenu,MFT_STRING		,IDM_LINK_MONSTER	,dMSG_LINK_MONSTER);

			AppendMenu(m_hMenu,MFT_STRING			,IDM_REMOVE_LINK	,dMSG_REMOVE_LINK);

			AppendMenu(m_hMenu,MFT_SEPARATOR		,0					,NULL);

			
			if (iCount	==	1)
				AppendMenu(m_hMenu,MFT_STRING		,IDM_SET_LEADER		,dMSG_SET_LEADER);

			AppendMenu(m_hMenu,MFT_STRING			,IDM_SET_WARD		,dMSG_SET_WARD);

			AppendMenu(m_hMenu,MFT_SEPARATOR		,0					,NULL);
			AppendMenu(m_hMenu,MFT_STRING			,IDM_EDIT_ACTOR		,dMSG_ACTOR_SETTING);
			
		}
	}

//	고정 오브젝트 편집 관련
	{
		int	iCount	=	g_lpActmap->m_object.getSelectedFixedObjectCount();

		if (iCount	>=	1)
		{
			AppendMenu(m_hMenu,MFT_SEPARATOR		,0						,NULL);
			AppendMenu(m_hMenu,MFT_STRING			,IDM_CHECK_SHADOW		,dMSG_CHECK_OBJECT_SHADOW);
			AppendMenu(m_hMenu,MFT_STRING			,IDM_REMOVE_SHADOW		,dMSG_REMOVE_OBJECT_SHADOW);
			AppendMenu(m_hMenu,MFT_STRING			,IDM_LINK_TO_EVENT_AREA	,"이벤트 영역과 연결하기");


		}

		if (g_lpActmap->m_object.getSelectedObjectCount()	==	1)
		{
			AppendMenu(m_hMenu,MFT_SEPARATOR		,0						,NULL);
			AppendMenu(m_hMenu,MFT_STRING			,IDM_SELECT_SAME_OBJECT_IN_FIELD	,dMSG_SELECT_SAME_OBJECT_IN_FIELD);
			AppendMenu(m_hMenu,MFT_STRING			,IDM_SELECT_SAME_OBJECT_IN_SCREEN	,dMSG_SELECT_SAME_OBJECT_IN_SCREEN);
		}
	}

	GetCursorPos(&pos);
	SetCursor(g_hNormalCursor);
	TrackPopupMenu(m_hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,pos.x,pos.y,0,hWND,NULL);
	DestroyMenu(m_hMenu);

	m_hMenu	=	NULL;
}	//	cMAPWND::popEditMenu()