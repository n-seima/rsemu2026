#include "cMAPWND.H"
#include "cMAIN.H"
#include "cTOOLWND.H"

void
cMAPWND::UpdateBuilding()
{	if (g_iWorkMode	!=	dWORK_BUILDING)	return;

	if (g_iScale	!=	100)	return;

	if (TOUCHRBUTTON)
	{
		TOUCHRBUTTON	=	FALSE;

		int	iFocusBuilding	=	g_lpActmap->getFocusBuilding((Cursor.x+g_lpActmap->m_pos.x),(Cursor.y+g_lpActmap->m_pos.y));

		if (iFocusBuilding	!=	0xffff)
		{
			if (TOOLWND.m_isEditBlockOfBuilding)
			{
				TOOLWND.endEditBlockOfBuilding();

				MAIN.Draw();

				return;
			}
			else
			{
				TOOLWND.beginEditBlockOfBuilding(iFocusBuilding);

				MAIN.Draw();

				return;
			}
		}
		
		if (TOOLWND.m_isEditBlockOfBuilding)
		{
			TOOLWND.endEditBlockOfBuilding();

			MAIN.Draw();

			return;
		}

		MAIN.ChangeMode(dWORK_EDIT);
	}

	if (TOUCHLBUTTON	&&	client.isIN(Mouse.x,Mouse.y))
	{
		if (TOOLWND.m_isEditBlockOfBuilding)
		{
			int	iFocusBuilding	=	g_lpActmap->getFocusBuilding((Cursor.x+g_lpActmap->m_pos.x),(Cursor.y+g_lpActmap->m_pos.y));

			if (iFocusBuilding	!=	0xffff)
			{
				TOOLWND.beginEditBlockOfBuilding(iFocusBuilding);

				MAIN.Draw();

				return;
			}
		}
		else
		{
			if (Mouse.y	>	g_iTBSize || g_isDrawingMap)
			{
				g_lpActmap->buildBuilding((Cursor.x+g_lpActmap->m_pos.x),(Cursor.y+g_lpActmap->m_pos.y),g_lpActmap->m_iSelectBuilding);
				m_isRepaint	=	TRUE;
			}

			g_lpActmap->LostVirgin();
		}
	}

	if (!TOOLWND.m_isEditBlockOfBuilding)
		if (!Cursor.isSame(&OldCursor)		)	m_isRepaint	=	TRUE;	//	커서가 움직였냐?
}