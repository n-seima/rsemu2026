#include	"cSET.H"
#include	"resource.H"
#include	"cKARMA.H"
#include	"cSKILL.H"
#include	"cJOB.H"
#include	"cDRAW.H"
#include	"cTEXT.H"
#include	"cMAIN.H"
#include	"CSetDialogKarma.h"

HWND				cSET_CHARACTER_TREE_DATA::s_hWnd;	
cTREEVIEW			cSET_CHARACTER_TREE_DATA::s_treeView;
cCHARACTER			*cSET_CHARACTER_TREE_DATA::s_lpEditCharacter;
CKarma				*cSET_CHARACTER_TREE_DATA::s_lpStoredKarma;
CKarma				*cSET_CHARACTER_TREE_DATA::s_lpSelectKarma;
CKarma				cSET_CHARACTER_TREE_DATA::s_aStoredKarma;


CKarmaContainer		cSET_CHARACTER_TREE_DATA::s_aEvent[eMECC_COUNT];
cDIBWND				cSET_CHARACTER_TREE_DATA::s_hKarmaDetailWnd;

int					cSET_CHARACTER_TREE_DATA::s_iSelectCondition	= 0;
int					cSET_CHARACTER_TREE_DATA::s_iKarmaDetailWndPos	= 0;
int					cSET_CHARACTER_TREE_DATA::s_iCopiedKaramaCount	= 0;

BOOL				cSET_CHARACTER_TREE_DATA::s_bIsSelectKarma		= FALSE;

//
// 11.24 Tree
void
cSET_CHARACTER_TREE_DATA::SetTreeEvent(HWND _hDlg,cCHARACTER *_lpEditCharacter)
{
	s_lpEditCharacter		=	_lpEditCharacter;
	s_lpStoredKarma			=	cKARMA_WND::s_aStoredKarma;
	
	s_iCopiedKaramaCount	= 0;
	s_iSelectCondition		= 0;
	s_iKarmaDetailWndPos	= 0;

	for (int i=0;i<eMECC_COUNT;i++)
		s_aEvent[i].copy(_lpEditCharacter->m_pEvent[i],TRUE);

	cKARMA_WND::s_bIsFieldKarma	=	FALSE;

	if	(DialogBox( s_hInst,"SET_CHARACTER_EVENT_TREEDATA",_hDlg,(DLGPROC)s_procSetCharacterEventTreeData)==IDOK)
		return;
}

//
// 11.24 Tree
void
cSET_CHARACTER_TREE_DATA::InitSetCharacterEventTreeData(HWND _hDlg)
{
	HWND	hTree			=	GetDlgItem(_hDlg,IDC_TREE);
			
	s_treeView.Init(hTree,MAIN.hINST,NULL);
	FillTree();
	
	UpdateKarma(_hDlg);
}

void
cSET_CHARACTER_TREE_DATA::UpdateKarma(HWND _hDlg)
{
	HWND	hKarmaDetail	=	GetDlgItem(_hDlg,IDC_KARMA_VIEW);
	g_lpAKC					=	&s_aEvent[s_iSelectCondition];	
		
	if	(!hKarmaDetail)
			return;

	SCROLLINFO	si;
	si.cbSize	=	sizeof(SCROLLINFO);
	si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
	si.nMin		=	0;
	si.nMax		=	3;
	si.nPage	=	3;
	si.nPos		=	0;
	EnableScrollBar	(hKarmaDetail,SB_VERT,ESB_ENABLE_BOTH);
	SetScrollInfo	(hKarmaDetail,SB_VERT,&si,TRUE);

	s_hKarmaDetailWnd.lpszRegister	=	"KARMA_DETAIL_WINDOW";
	s_hKarmaDetailWnd.Init(hKarmaDetail,(WNDPROC)s_ProcKarmaDetailWnd);
	
	if	(g_lpAKC)
		s_lpSelectKarma	=	g_lpAKC->getSelectKarma();
	else
		s_lpSelectKarma	=	NULL;

	s_iKarmaDetailWndPos = 0;
	DrawKarmaDetail();	
}

void
cSET_CHARACTER_TREE_DATA::FillTree()
{
	s_treeView.Clear();
	int	iIndex, jIndex, kIndex;
	
	for (iIndex=0, kIndex = 0;;iIndex++)
	{
		if (stricmp(g_strMonsterEventCheckCondition[iIndex],"end")==0)	break;

		HTREEITEM	hParents	=	s_treeView.Add(g_strMonsterEventCheckCondition[iIndex],-1);
		g_lpAKC	=	&s_aEvent[iIndex];

		if (!g_lpAKC || g_lpAKC->m_wKarmaCount == 0)
			continue;
		
		for(jIndex = 0; jIndex < g_lpAKC->m_wKarmaCount ; jIndex++, kIndex++ )
		{
			CKarma	*lpKarma	=	&g_lpAKC->m_pKarma[jIndex];
			char	strText[256];
			sprintf(strText,"#%.3d %s",jIndex,lpKarma->getTitle());

			int			iParam		=	(iIndex<<16)+jIndex;
			HTREEITEM	hCurrent	=	s_treeView.Add(hParents,strText,iParam);

			if( s_iSelectCondition == iIndex &&  g_iSelectKarma == jIndex )
			{
				s_treeView.Select(hCurrent);
			}

		}
		s_treeView.Expand(hParents);
	}
}	

void
cSET_CHARACTER_TREE_DATA::DrawKarmaDetail()
{
	CKarma	karma;
	char	strOutputText[1024];

	int		x=5,y=10,iWidth,iHeight,i;
	int		iTriggerHeight=0,iReactionHeight=0;

	if	(!s_hKarmaDetailWnd.Active())
		return;
	
	iWidth	=	cDRAW::WIDTH-15;
	cDRAW::Fill(0);
	
	if (!s_lpSelectKarma || s_lpSelectKarma->m_wTriggerCount + s_lpSelectKarma->m_wReactionCount <= 0)
	{
		EnableScrollBar(s_hKarmaDetailWnd.hWND,SB_VERT,ESB_DISABLE_BOTH);
		cTEXT::Put(x,y,SKYBLUE,dMSG_THERE_ARE_NO_KARMA);
		s_hKarmaDetailWnd.Draw();
		return;
	}
	
	if(s_bIsSelectKarma == FALSE)
	{
		EnableScrollBar(s_hKarmaDetailWnd.hWND,SB_VERT,ESB_DISABLE_BOTH);
		cTEXT::Put(x,y,SKYBLUE,dMSG_THERE_ARE_NO_KARMA);
		s_hKarmaDetailWnd.Draw();
		return;
	}

	karma.copy(s_lpSelectKarma);

	iHeight	=	y+18;

	for (i=0;i<karma.m_wTriggerCount;i++)
	{
		char	*lpText		=	cKARMA_WND::KarmaText(&karma.m_pTrigger[i],TRUE);
		sprintf(strOutputText," <condition #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

		iHeight				+=	iLineCount*14;
		iTriggerHeight		+=	iLineCount*14;
	}

	iHeight	+=	(21+15);

	for (i=0;i<karma.m_wReactionCount;i++)
	{
		char	*lpText		=	cKARMA_WND::KarmaText(&karma.m_pReaction[i],FALSE);
		sprintf(strOutputText," <action #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

		iHeight				+=	iLineCount*14;
		iReactionHeight		+=	iLineCount*14;
	}

	if (iHeight > cDRAW::HEIGHT)
	{	SCROLLINFO	si;

		si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	iHeight-1;
		si.nPage	=	cDRAW::HEIGHT;
		si.nPos		=	s_iKarmaDetailWndPos;

		EnableScrollBar(s_hKarmaDetailWnd.hWND,SB_VERT,ESB_ENABLE_BOTH);
		SetScrollInfo(s_hKarmaDetailWnd.hWND,SB_VERT,&si,TRUE);
	}
	else
	{	
		EnableScrollBar(s_hKarmaDetailWnd.hWND,SB_VERT,ESB_DISABLE_BOTH);
		s_iKarmaDetailWndPos	=	0;
	}

	y		-=	s_iKarmaDetailWndPos;

	///////////////////////////////////////////////////////////////////////////
//	¶° ±×·
///////////////////////////////////////////////////////////////////////////

	cTEXT::cPut(x+iWidth/2,y+5,SKYBLUE,_ms("Probability of activation [%d%] Check period [%d seconds] Limit number of activations [%d times] Number of active triggers [% d times]",karma.m_wActivateChance,karma.m_wActivePeriod,karma.m_wActiveCount,karma.m_bf7ActivateTriggerCount));
	y	+=	18;

	if (iTriggerHeight	==	0)
	{
		cTEXT::cPut(x+iWidth/2,y+5,LTGREEN,"- There are no special conditions to execute this operation. -");

		y	+=	21;
	}
	else
	{
		cDRAW::Box(_LTYELLOW,x,y+5,x+iWidth,y+16+5+iTriggerHeight);
		cDRAW::Fill(0,x+iWidth/2-50,y,x+iWidth/2+50,y+12);

		if (karma.m_isWantNotComplete)
		{
			if	(karma.m_isWantAndCondition)
				cTEXT::cPut(x+iWidth/2,y,LTCYAN,"Trigger condition (and) is not satisfied");
			else
				cTEXT::cPut(x+iWidth/2,y,LTCYAN,"Trigger condition (or) is not satisfied");
		}
		else
		{
			if	(karma.m_isWantAndCondition)
				cTEXT::cPut(x+iWidth/2,y,LTCYAN,"trigger condition (and)");
			else
				cTEXT::cPut(x+iWidth/2,y,LTCYAN,"Trigger condition (or)");
		}

		y	+=	16;

		for (i=0;i<karma.m_wTriggerCount;i++)
		{
			char	*lpText		=	cKARMA_WND::KarmaText(&karma.m_pTrigger[i],TRUE);
			sprintf(strOutputText," <condition #%2d> - %s",i+1,lpText);
			int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

			cTEXT::FitPut(x+5,y,iWidth-5,100,WHITE,strOutputText);

			y					+=	iLineCount*14;
		}
		y	+=	5;
	}

	y	+=	15;

	cDRAW::Box(_LTYELLOW,x,y+5,x+iWidth,y+16+5+iReactionHeight);
	cDRAW::Fill(0,x+iWidth/2-50,y,x+iWidth/2+50,y+12);

	cTEXT::cPut(x + iWidth / 2, y, LTCYAN, "Things to do");

	y	+=	16;

	for (i=0;i<karma.m_wReactionCount;i++)
	{
		char	*lpText		=	cKARMA_WND::KarmaText(&karma.m_pReaction[i],FALSE);
		sprintf(strOutputText," <action #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

		cTEXT::FitPut(x+5,y,iWidth-5,100,WHITE,strOutputText);
		
		y					+=	iLineCount*14;
	}

	s_hKarmaDetailWnd.Draw();
}

void
cSET_CHARACTER_TREE_DATA::RemoveEvent(HWND _hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	0 || g_iSelectKarma == 0xfffff)
		return;
	
	if	(cMSG::YESNO(dMSG_REMOVE_SELECT_KARMA,dMSG_QUESTION_CONTINUE)	==	IDNO)
		return;

	g_lpAKC->remove(g_iSelectKarma);

	g_iSelectKarma	-= 1;
	g_iSelectKarma	=	max(g_iSelectKarma,0);
	g_lpActmap->LostVirgin();

	HWND	hTree			=	GetDlgItem(s_hWnd,IDC_TREE);
	SendMessage(hTree, WM_SETREDRAW, 0, 0);
	FillTree();
	UpdateKarma(_hDlg);
	SetFocus(GetDlgItem(s_hWnd,IDC_TREE));
	SendMessage(hTree, WM_SETREDRAW, 1, 0);
}

void
cSET_CHARACTER_TREE_DATA::UpEvent(HWND _hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	0	||	g_iSelectKarma	<=	0)
		return;

	CKarma	temp;

	temp.copy(&g_lpAKC->m_pKarma[g_iSelectKarma]);
	g_lpAKC->m_pKarma[g_iSelectKarma].copy(&g_lpAKC->m_pKarma[g_iSelectKarma-1]);
	g_lpAKC->m_pKarma[g_iSelectKarma-1].copy(&temp);
	g_iSelectKarma--;

	HWND	hTree			=	GetDlgItem(s_hWnd,IDC_TREE);
	SendMessage(hTree, WM_SETREDRAW, 0, 0);
	FillTree();
	UpdateKarma(_hDlg);
	SetFocus(GetDlgItem(s_hWnd,IDC_TREE));
	SendMessage(hTree, WM_SETREDRAW, 1, 0);
}

void
cSET_CHARACTER_TREE_DATA::DownEvent(HWND _hDlg)
{
	if	(g_iSelectKarma	>=	g_lpAKC->getKarmaCount()-1)
		return;

	CKarma	temp;

	temp.copy(&g_lpAKC->m_pKarma[g_iSelectKarma]);
	g_lpAKC->m_pKarma[g_iSelectKarma].copy(&g_lpAKC->m_pKarma[g_iSelectKarma+1]);
	g_lpAKC->m_pKarma[g_iSelectKarma+1].copy(&temp);
	g_iSelectKarma++;	
	
	HWND	hTree			=	GetDlgItem(s_hWnd,IDC_TREE);
	SendMessage(hTree, WM_SETREDRAW, 0, 0);
	FillTree();
	UpdateKarma(_hDlg);
	SetFocus(GetDlgItem(s_hWnd,IDC_TREE));
	SendMessage(hTree, WM_SETREDRAW, 1, 0);
}

void
cSET_CHARACTER_TREE_DATA::CopyEvent(HWND _hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	0)
		return;

	CKarma	*lpKarma	= &g_lpAKC->m_pKarma[g_iSelectKarma];
	
	s_aStoredKarma.allocBuffer(dKARMA_COUNT);
	s_aStoredKarma.copy(lpKarma);
	s_aStoredKarma.setTitle(_ms("%s[2]",lpKarma->m_pstrTitle));

	s_iCopiedKaramaCount = 1;
}

void
cSET_CHARACTER_TREE_DATA::PasteEvent(HWND _hDlg)
{
	if	(s_iCopiedKaramaCount	==	0)
		return;

	int iAddKarma = g_lpAKC->add(&s_aStoredKarma);

	if	(iAddKarma	==	0xffff)
		return;
	
	g_lpAKC->selectKarma(iAddKarma);

	HWND	hTree			=	GetDlgItem(s_hWnd,IDC_TREE);
	SendMessage(hTree, WM_SETREDRAW, 0, 0);
	FillTree();
	UpdateKarma(_hDlg);
	SetFocus(GetDlgItem(s_hWnd,IDC_TREE));
	SendMessage(hTree, WM_SETREDRAW, 1, 0);
	g_lpActmap->LostVirgin();
}

void
cSET_CHARACTER_TREE_DATA::CompleteWork(HWND _hDlg)
{
	int	i;

	for (i=0;i<eMECC_COUNT;i++)
	{
		KILL(s_lpEditCharacter->m_pEvent[i]);

		if (s_aEvent[i].m_wKarmaCount == 0) continue;

		s_lpEditCharacter->m_pEvent[i]	=	new CKarmaContainer();
		s_lpEditCharacter->m_pEvent[i]->copy(&s_aEvent[i]);
		s_lpEditCharacter->m_pEvent[i]->setTitle(g_strMonsterEventCheckCondition[i]);
	}
}


LRESULT CALLBACK	
cSET_CHARACTER_TREE_DATA::s_procSetCharacterEventTreeData(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG	:
			s_hWnd = hDlg;
			InitSetCharacterEventTreeData(hDlg);
			break;
		
		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			break;

		case WM_LBUTTONDOWN:

			break;
		case WM_NOTIFY		:
			if( wParam == IDC_TREE)
			{
				NM_TREEVIEW	*lpNM	=	(NM_TREEVIEW *)lParam;

				switch(lpNM->hdr.code)
				{
					case	TVN_SELCHANGED		:
					{
						int	iSerial	=	lpNM->itemNew.lParam;
						
						if	(lpNM->itemNew.lParam	==	-1)
						{
							s_bIsSelectKarma = FALSE;
							UpdateKarma(hDlg);
							break;
						}

						s_bIsSelectKarma	= TRUE;												
						s_iSelectCondition	= iSerial>>16;
						g_iSelectKarma		= iSerial&0xffff;
						
						UpdateKarma(hDlg);
						
						break;
					}
				}
			}
			break;

		case WM_COMMAND:
			switch( LOWORD( wParam ) )
			{
			case IDC_REMOVE	:
				RemoveEvent(hDlg);
				break;
			case IDC_UP		:
				UpEvent(hDlg);
				break;
			case IDC_DOWN	:
				DownEvent(hDlg);
				break;
			case IDC_COPY	:
				CopyEvent(hDlg);
				break;
			case IDC_PASTE	:
				PasteEvent(hDlg);
				break;
			case IDC_NEW	:
				{
					CKarma	*lpNewKarma =	CSetDialogKarma::AddKarma(hDlg, NULL, 0);
					
					if (!lpNewKarma)
						break;
					
					int	iAddedKarma		=	g_lpAKC->add(lpNewKarma);
					if (iAddedKarma		==	0xffff)		break;

					g_lpAKC->selectKarma(iAddedKarma);
					FillTree();
					UpdateKarma(hDlg);
					SetFocus(GetDlgItem(s_hWnd,IDC_TREE));
					g_lpActmap->LostVirgin();
					break;
				}
			case IDC_EDIT	:
				{
					CKarma	*lpKarma =	CSetDialogKarma::AddKarma(hDlg, &g_lpAKC->m_pKarma[g_iSelectKarma], 1);
					
					if (!lpKarma)
						break;
					
					g_lpAKC->m_pKarma[g_iSelectKarma].copy(lpKarma);
					
					FillTree();
					UpdateKarma(hDlg);
					SetFocus(GetDlgItem(s_hWnd,IDC_TREE));
					g_lpActmap->LostVirgin();
					
					break;
				}
			case	IDOK						:
				{
					CompleteWork(hDlg);
					EndDialog(hDlg,IDOK);
					break;
				}

			case	IDCANCEL					:
					EndDialog(hDlg,IDCANCEL);
					break;
			
			}

			break;
	}

	return (0L);
}

LRESULT CALLBACK
cSET_CHARACTER_TREE_DATA::s_ProcKarmaDetailWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	
			PAINTSTRUCT ps;
			BeginPaint( hDlg, &ps );

			s_hKarmaDetailWnd.Active();
			s_hKarmaDetailWnd.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}
	}
	
	return (0L);
}