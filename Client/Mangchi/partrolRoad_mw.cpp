#include "cMAPWND.H"
#include "cMAIN.H"
#include "patrolRoad.H"
#include "cSet.H"
#include "cTOOLWND.H"

void
cMAPWND::UpdatePatrolRoad()
{
	if (g_iWorkMode	!=	dWORK_ROAD)
		return;

	if (!client.isIN(Mouse.x,Mouse.y))
		return;

	int x=Mouse.x+g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
	int y=Mouse.y+g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE-g_iTBSize;

	x	=	Mouse.x*100/g_iScale+g_lpActmap->m_pos.x*dTILE_WIDTH/dSHARE_TILE;
	y	=	(Mouse.y-g_iTBSize)*100/g_iScale+g_lpActmap->m_pos.y*dTILE_HEIGHT/dSHARE_TILE;

	if (TOUCHLBUTTON)
	{
		g_editRoad.addNode(x,y);

		m_isRepaint	=	TRUE;
	}

	if (TOUCHRBUTTON)
	{
		m_isRepaint	=	TRUE;
		g_editRoad.reset();
	}

	if (TOUCHKEY[dKEY_ENTER])
	{
		if (g_editRoad.m_wNodeCount)
		{
			m_isRepaint	=	TRUE;
			
			if (cSetPatrolRoad::Open(hWND,&g_editRoad))
			{
				g_lpActmap->m_prManager.add(&cSetPatrolRoad::s_patrolRoad);

				TOOLWND.Draw();
			}

			g_editRoad.reset();
		}
	}

	if (KEY[dKEY_DELETE])
		if (g_lpActmap->m_prManager.get())
			if (cMSG::YESNO("선택한 순찰로 제거",dMSG_QUESTION_CONTINUE) == IDYES)
			{
				if (g_lpActmap->m_prManager.remove())
				{
					m_isRepaint	=	TRUE;
					g_lpActmap->LostVirgin();
					TOOLWND.Draw();
				}
			}
}
