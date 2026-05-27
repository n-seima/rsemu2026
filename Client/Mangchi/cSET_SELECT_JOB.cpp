#include "resource.H"
#include "cSET.H"
#include "cMAPWND.H"
#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cDRAW.H"
#include "cTEXT.H"
#include "cJOB.H"

cDIBWND				cSELECT_JOB::s_JobListView;
cSCROLL_BAR			cSELECT_JOB::s_ScrollBar;

int					cSELECT_JOB::s_iWidth		=	0;
int					cSELECT_JOB::s_iHeight		=	0;
int					cSELECT_JOB::s_iJobBoxWidth	=	0;
int					cSELECT_JOB::s_iJobBoxHeight=	0;
int					cSELECT_JOB::s_iJPW			=	0;
int					cSELECT_JOB::s_iJPH			=	0;
int					cSELECT_JOB::s_iLeft		=	0;
int					cSELECT_JOB::s_iTop			=	0;
int					cSELECT_JOB::s_iMode		=	0;
int					cSELECT_JOB::s_iJobCount	=	0;
cBODY_DATA			*cSELECT_JOB::s_lpDefaultJob=	NULL;


char	l_strSelectJobTitle[256];
int		l_iSelectJob	=	0;

int
cSELECT_JOB::SelectHeroJob(HINSTANCE hinst,HWND hwnd)
{
//	s_lpDefaultJob	=	g_aHeroBodyData;
	l_iSelectJob	=	0;

	s_iMode			=	dJOB_LIST_HERO;
	s_iJobCount		=	dPLAYER_JOB_COUNT;

	strcpy(l_strSelectJobTitle,dMSG_SELECT_JOB);

	s_hInst			=	hinst;
	int	result		=	DialogBox( hinst,"JOB_LIST",hwnd,(DLGPROC)s_procListBox);

	return	l_iSelectJob;
}	//	cSELECT_JOB::Init(HINSTANCE hinst,HWND hwnd,char *title)


int
cSELECT_JOB::SelectMonsterJob(HINSTANCE hinst,HWND hwnd)
{
	s_lpDefaultJob	=	g_aMonsterBodyData;
	l_iSelectJob	=	0;

	s_iMode			=	dJOB_LIST_MONSTER;
	s_iJobCount		=	g_BodyList.m_iMonsterBodyCount;

	strcpy(l_strSelectJobTitle,dMSG_SELECT_JOB);

	s_hInst			=	hinst;
	int	result		=	DialogBox( hinst,"JOB_LIST",hwnd,(DLGPROC)s_procListBox);

	return	l_iSelectJob;
}	//	cSELECT_JOB::Init(HINSTANCE hinst,HWND hwnd,char *title)


//
//	직업 리스트박스를 초기화한다
//
void
cSELECT_JOB::InitListBox(HWND hwnd)
{
	HWND	hJobList	=	GetDlgItem(hwnd,IDC_JOB_LIST);

	SetWindowText(hwnd,l_strSelectJobTitle);

	s_ScrollBar.Init(hJobList);
	s_ScrollBar.InitHeight();

	s_JobListView.lpszRegister	=	"JOB_LIST_WND";
	s_JobListView.Init	(hJobList,(WNDPROC)s_procListWnd);

	s_iWidth			=	s_JobListView.width();
	s_iHeight			=	s_JobListView.height();
	s_iJobBoxWidth		=	(18*6 + 4);	
	s_iJobBoxHeight		=	100+4+16;	

	s_iJPW				=	s_iWidth	/s_iJobBoxWidth;
	s_iJPH				=	s_iHeight	/s_iJobBoxHeight;

	s_iLeft				=	(s_iWidth	-s_iJPW*s_iJobBoxWidth)/2;
	s_iTop				=	(s_iHeight	-s_iJPH*s_iJobBoxHeight)/2;

	int	iScrollHeight	=	s_iJobCount/s_iJPW;

	if (s_iJobCount%s_iJPW)	iScrollHeight++;
	s_ScrollBar.SetHeight(iScrollHeight,s_iJPH);

	Draw(hwnd);
}	//	cSELECT_JOB::InitListBox(HWND hwnd)

BOOL
cSELECT_JOB::SelectJob(HWND hwnd,int xp,int yp)
{
	int	i,j,x,y;
	int	index			=	s_ScrollBar.m_iHeightPos*s_iJPW;

	y					=	s_iTop;

	for (i=0;i<s_iJPH;i++)
	{
		x	=	s_iLeft;

		for (j=0;j<s_iJPW;j++)
		{
			if(s_lpDefaultJob[index].m_iBody	!=	0xffff)
			{
				cRECT	rect;

				rect.Set(x,y,x+s_iJobBoxWidth-1,y+s_iJobBoxHeight-1);

				if (rect.isIN(xp,yp))
				{
					l_iSelectJob	=	index;
					return TRUE;
				}
			}

			index	++;

			if (index >= s_iJobCount)	break;

			x	+=	s_iJobBoxWidth;
		}

		y	+=	s_iJobBoxHeight;

		if (index >= s_iJobCount)	break;
	}

	return	FALSE;
}

//
//	직업 리스트 창에 직업이미지와 이름을 표시해준다.
//
void
cSELECT_JOB::Draw(HWND hwnd)
{
	if (!s_JobListView.Active())	return;

	cDRAW::Fill(0);

	int	i,j,x,y;
	int	index			=	s_ScrollBar.m_iHeightPos*s_iJPW;

	y					=	s_iTop;

	for (i=0;i<s_iJPH;i++)
	{
		x	=	s_iLeft;

		for (j=0;j<s_iJPW;j++)
		{
			if(s_lpDefaultJob[index].m_iBody	!=	0xffff)
			{
				if (index	==	l_iSelectJob)
				{
					x+=1,y+=1;
					cDRAW::Fill(_LTBLUE,x+3,y+3,x+s_iJobBoxWidth-6,y+s_iJobBoxHeight-22);
					cDRAW::Box(_WHITE,x+3,y+3,x+s_iJobBoxWidth-6,y+s_iJobBoxHeight-22);
					g_pBody[s_lpDefaultJob[index].m_iBody]->FitPut(x+3,y+3,s_iJobBoxWidth-10,s_iJobBoxHeight-25,4);
					cDRAW::Box(_LTGREEN,x,y,x+s_iJobBoxWidth-3,y+s_iJobBoxHeight-3);

					cTEXT::cPut(x+s_iJobBoxWidth/2,y+s_iJobBoxHeight - 17,LTYELLOW,s_lpDefaultJob[index].m_strName);
					x-=1,y-=1;
				}
				else
				{
					cDRAW::Fill(_GRAY,x+3,y+3,x+s_iJobBoxWidth-6,y+s_iJobBoxHeight-22);
					cDRAW::Box(_LTGRAY,x+3,y+3,x+s_iJobBoxWidth-6,y+s_iJobBoxHeight-22);
					g_pBody[s_lpDefaultJob[index].m_iBody]->FitPut(x+3,y+3,s_iJobBoxWidth-10,s_iJobBoxHeight-25,4);
					cDRAW::Box(_LTGRAY,x,y,x+s_iJobBoxWidth-3,y+s_iJobBoxHeight-3);

					cTEXT::cPut(x+s_iJobBoxWidth/2,y+s_iJobBoxHeight - 17,WHITE,s_lpDefaultJob[index].m_strName);
				}
			}

			index	++;

			if (index >= s_iJobCount)	break;

			x	+=	s_iJobBoxWidth;
		}

		y	+=	s_iJobBoxHeight;

		if (index >= s_iJobCount)	break;
	}

	s_JobListView.Draw();
}	//	cSELECT_JOB::Draw(HWND hwnd)

LRESULT CALLBACK
cSELECT_JOB::s_procListBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			InitListBox(hwnd);
			break;

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			l_iSelectJob	=	0xffff;	//	선택한 직업이 없다.
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					EndDialog(hwnd,IDOK);
					break;
				}

				case	IDCANCEL			:
				{
					EndDialog(hwnd,IDOK);
					l_iSelectJob	=	0xffff;	//	선택한 직업이 없다.

					break;
				}
			}
			break;
	}

	return (0L);
}


LRESULT CALLBACK
cSELECT_JOB::s_procListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_JobListView.Active();
			s_JobListView.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN		:
		{
			if(SelectJob(hwnd,LOWORD(lParam),HIWORD(lParam)))	Draw(hwnd);

			break;
		}

		case WM_VSCROLL			:
		{
			if (s_ScrollBar.VScroll(wParam))	Draw(hwnd);
			break;
		}

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}