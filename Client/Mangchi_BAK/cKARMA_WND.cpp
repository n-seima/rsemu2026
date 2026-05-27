#include "CKarma.H"
#include "resource.H"
#include "cMESSAGE.H"
#include "cDRAW.H"
#include "cTEXT.H"
#include "cMAP.H"
#include "cMAPWND.H"
#include "secretDungeon.H"
#include "cQuest.H"
#include "mainQuest.H"

HWND			cKARMA_WND::s_hMaster;
HINSTANCE		cKARMA_WND::s_hInst;
int				cKARMA_WND::s_iKarmaDetailWndPos	=	0;		//	카르마의 정보를 표시하는 윈도우의 스크롤바 위치
cDIBWND			cKARMA_WND::s_hKarmaDetailWnd;
BOOL			cKARMA_WND::s_isEditKarma			=	FALSE;
BOOL			cKARMA_WND::s_isEditTrigger			=	FALSE;
BOOL			cKARMA_WND::s_isEditReaction		=	FALSE;	//	에디터 모드냐?
CKarma			*cKARMA_WND::s_lpSelectKarma		=	NULL;
CKarma			cKARMA_WND::s_aStoredKarma[c_iCopyKarmaBufferSize];
BOOL			cKARMA_WND::s_isJongSung			=	FALSE;
BOOL			cKARMA_WND::s_bIsEditDialog;
BOOL			cKARMA_WND::s_bIsDrawTitle			=	TRUE;

BOOL			cKARMA_WND::s_iSelectKarmaSet		=	0;		//	선택한 탭
int				cKARMA_WND::s_iPasteReactionPos		=	0;
int				cKARMA_WND::s_iCopiedKaramaCount	=	0;
int				cKARMA_WND::s_iPasteTriggerPos		=	0;
int				cKARMA_WND::s_iDungeonvalueComboBox	=	-1;
int				cKARMA_WND::s_iQuestJobQuest		=	-1;
int				cKARMA_WND::s_iQuestArbeitJobQuest	=	-1;
int				cKARMA_WND::s_iSelectMainQuestValue	=	-1;
int				cKARMA_WND::s_iSelectMainQuestSwitch=	-1;

int				cKARMA_WND::s_iSelectDungeonValue	=	0;
int				cKARMA_WND::s_iMainQuestValueComboBox=	0;
int				cKARMA_WND::s_iMainQuestSwitchComboBox=	0;

cCOMBOBOX		cKARMA_WND::s_aCombo[10];

HWND			cKARMA_WND::s_hTab					=	NULL;	//	탭의 윈도우 핸들..
BOOL			cKARMA_WND::s_bIsFieldKarma			=	FALSE;


BOOL
cKARMA_WND::Open(HWND hwnd,HINSTANCE hinst,CKarmaContainer *_lpKC,BOOL _bIsKarma,BOOL _bIsFieldKarma)
{
	s_bIsFieldKarma	=	_bIsFieldKarma;
	g_lpAKC			=	_lpKC;
	s_hMaster		=	hwnd;
	s_hInst			=	hinst;

	if	(!s_storedKarmaForDetail.m_pReaction)
		s_storedKarmaForDetail.reset(TRUE);

	int iResult;

	s_bIsEditDialog=	1-	_bIsKarma;
		
	if	(s_bIsEditDialog)
		iResult	=	DialogBox( s_hInst, "SET_DIALOG_SPEECH_ACTION",s_hMaster,(DLGPROC)s_procKarmaMain);
	else
		iResult	=	DialogBox( s_hInst, "SET_KARMA",s_hMaster,(DLGPROC)s_procKarmaMain);
		
	g_MAPWND.Draw();
	
	if	(iResult	==	IDCANCEL)
		return	FALSE;
	return	TRUE;
}

//
//	카르마 선택
//
void
cKARMA_WND::SelectKarma(HWND hDlg)
{
	if	(g_lpAKC == NULL || g_lpAKC->getKarmaCount()	<=	0)
		return;

	HWND hList	=	GetDlgItem(hDlg,IDC_KARMA_LIST);

	int iSelect	=	SendMessage(hList,LB_GETCURSEL,0,0);

	if	(iSelect	==	LB_ERR	)
		return;

	iSelect		=	cLISTBOX::GetData(hList,iSelect);

	if	(iSelect	==	g_iSelectKarma	)
		return;

	g_lpAKC->selectKarma(iSelect);

	s_iKarmaDetailWndPos	=	0;
	s_lpSelectKarma			=	g_lpAKC->getSelectKarma();

	DrawKarmaDetail(hDlg);
}

//
//	카르마 제거
void
cKARMA_WND::RemoveKarma(HWND hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	0)
		return;

	HWND	hList				=	GetDlgItem(hDlg,IDC_KARMA_LIST);
	int		aiSelectKarma[c_iCopyKarmaBufferSize];
	int		iSelectKarmaCount	=	cLISTBOX::GetSelect(hList,aiSelectKarma,c_iCopyKarmaBufferSize);

	if	(iSelectKarmaCount	<=	0)
		return;

	if	(cMSG::YESNO(dMSG_REMOVE_SELECT_KARMA,dMSG_QUESTION_CONTINUE)	==	IDNO)
		return;

	for	(int i=0;i<iSelectKarmaCount;i++)
	{
		g_lpAKC->remove(aiSelectKarma[i]-i);

		g_iSelectKarma	=	aiSelectKarma[i]-i-1;
	}

	g_iSelectKarma	=	max(g_iSelectKarma,0);

	UpdateKarmaDlg(hDlg);

	g_lpActmap->LostVirgin();
}

//
//	선택된 카르마의 설정을 그려준다.
void
cKARMA_WND::DrawKarmaDetail(HWND hDlg)
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

	karma.copy(s_lpSelectKarma);

///////////////////////////////////////////////////////////////////////////
//	좌표 계산
///////////////////////////////////////////////////////////////////////////

	iHeight	=	y+18;

	for (i=0;i<karma.m_wTriggerCount;i++)
	{
		char	*lpText		=	KarmaText(&karma.m_pTrigger[i],TRUE);
		sprintf(strOutputText," <조건 #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

		iHeight				+=	iLineCount*14;
		iTriggerHeight		+=	iLineCount*14;
	}

	iHeight	+=	(21+15);

	for (i=0;i<karma.m_wReactionCount;i++)
	{
		char	*lpText		=	KarmaText(&karma.m_pReaction[i],FALSE);
		sprintf(strOutputText," <액션 #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

		iHeight				+=	iLineCount*14;
		iReactionHeight		+=	iLineCount*14;

		if (iTriggerHeight	==	0)
			iHeight += 5;
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
	{	EnableScrollBar(s_hKarmaDetailWnd.hWND,SB_VERT,ESB_DISABLE_BOTH);
		s_iKarmaDetailWndPos	=	0;
	}

	y		-=	s_iKarmaDetailWndPos;

///////////////////////////////////////////////////////////////////////////
//	조건 그려줌
///////////////////////////////////////////////////////////////////////////

	cTEXT::cPut(x+iWidth/2,y+5,SKYBLUE,_ms("발동확률[%d％] 체크주기[%d초] 발동횟수제한[%d회] 활성트리거주건수[%d회]",karma.m_wActivateChance,karma.m_wActivePeriod,karma.m_wActiveCount,karma.m_bf7ActivateTriggerCount));

	y	+=	18;

	if (iTriggerHeight	==	0)
	{
		cTEXT::cPut(x+iWidth/2,y+5,LTGREEN,"- 이 작업을 실행 시키기 위한 별다른 조건이 없습니다. -");

		y	+=	5;
	}
	else
	{
		cDRAW::Box(_LTYELLOW,x,y+5,x+iWidth,y+16+5+iTriggerHeight);
		cDRAW::Fill(0,x+iWidth/2-50,y,x+iWidth/2+50,y+12);

		if (karma.m_isWantNotComplete)
		{
			if	(karma.m_isWantAndCondition)
				cTEXT::cPut(x+iWidth/2,y,LTCYAN,"트리거 조건(and)이 불만족");
			else
				cTEXT::cPut(x+iWidth/2,y,LTCYAN,"트리거 조건(or)이 불만족");
		}
		else
		{
			if	(karma.m_isWantAndCondition)
				cTEXT::cPut(x+iWidth/2,y,LTCYAN,"트리거 조건(and)");
			else
				cTEXT::cPut(x+iWidth/2,y,LTCYAN,"트리거 조건(or)");
		}

		y	+=	16;

		for (i=0;i<karma.m_wTriggerCount;i++)
		{
			char	*lpText		=	KarmaText(&karma.m_pTrigger[i],TRUE);
			sprintf(strOutputText," <조건 #%2d> - %s",i+1,lpText);
			int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

			cTEXT::FitPut(x+5,y,iWidth,100,WHITE,strOutputText);

			y					+=	iLineCount*14;
		}

		y	+=	5;
	}

	y	+=	15;

	cDRAW::Box(_LTYELLOW,x,y+5,x+iWidth,y+16+5+iReactionHeight);
	cDRAW::Fill(0,x+iWidth/2-50,y,x+iWidth/2+50,y+12);

	cTEXT::cPut(x+iWidth/2,y,LTCYAN,"수행할 작업들");

	y	+=	16;

	for (i=0;i<karma.m_wReactionCount;i++)
	{
		char	*lpText		=	KarmaText(&karma.m_pReaction[i],FALSE);
		sprintf(strOutputText," <액션 #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

		cTEXT::FitPut(x+5,y,iWidth,100,WHITE,strOutputText);
		
		y					+=	iLineCount*14;
	}

	s_hKarmaDetailWnd.Draw();
}	//	cKARMA_WND::DrawKarmaDetail(HWND hDlg)

//	
//	등록된 트리거 출력
void
cKARMA_WND::InitKarmaDlg(HWND hwnd, BOOL _bTitle, CKarmaContainer *_lpKC)
{
	s_bIsDrawTitle	= _bTitle;
	UpdateKarmaDlg(hwnd, _lpKC);
}	

//	
//	등록된 트리거 출력
void
cKARMA_WND::UpdateKarmaDlg(HWND hwnd,CKarmaContainer *_lpKC)
{
	HWND	hKarmaList	=	GetDlgItem(hwnd,IDC_KARMA_LIST);		//	카르마 리스트 창
	HWND	hKarmaDetail=	GetDlgItem(hwnd,IDC_KARMA_VIEW);		//	카르마 디테일

	cLISTBOX::Reset(hKarmaList);									//	카르마 리스트 초기화

	if	(_lpKC)
		g_lpAKC	=	_lpKC;

	if	(!hKarmaList	||	!hKarmaDetail)
		return;

	SendMessage( hKarmaList, LB_RESETCONTENT, 0,0);				//	카르마 리스트 리셋

//	카르마의 세부 정보를 표시할 윈도우다.
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
	s_hKarmaDetailWnd.Init(hKarmaDetail,(WNDPROC)s_procKarmaDetailWnd);

	if (g_lpAKC && s_bIsDrawTitle)
	{
		SetDlgItemText(hwnd,IDC_SPEECH,g_lpAKC->m_pstrTitle);

		SetFocus(GetDlgItem(hwnd,IDC_SPEECH));
		SendMessage(GetDlgItem(hwnd,IDC_SPEECH),EM_SETSEL,0,-1);
	}

//	카르마 리스트 업~!!
	if	(s_iCopiedKaramaCount) 
		EnableWindow(GetDlgItem(hwnd,IDC_PASTE)	,TRUE);
	else
		EnableWindow(GetDlgItem(hwnd,IDC_PASTE)	,FALSE);

	if (!g_lpAKC || g_lpAKC->m_wKarmaCount == 0)	//	선택된 카르마 그룹에 등록된 카르마가 없다.
	{
		s_lpSelectKarma	=	NULL;

		DrawKarmaDetail(hwnd);
		SendMessage(hKarmaList, LB_ADDSTRING, 0,(LPARAM)dMSG_THERE_ARE_NO_KARMA);	//	등록된 카르마가 없다.
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT)	,FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_COPY)	,FALSE);
		return;
	}

	EnableWindow(GetDlgItem(hwnd,IDC_COPY)	,TRUE);
	EnableWindow(GetDlgItem(hwnd,IDC_EDIT)	,TRUE);
	EnableWindow(GetDlgItem(hwnd,IDC_REMOVE),TRUE);

	int index,pos=0;

	//	카르마 리스트 창에 카르마 리스트 출력
	for (index=0;index < g_lpAKC->m_wKarmaCount;index++)
	{
		CKarma	*lpKarma	=	&g_lpAKC->m_pKarma[index];

		char	strText[256];

		sprintf(strText,"#%.3d %s",index,lpKarma->getTitle());

		cLISTBOX::Add(hKarmaList,strText,index);
	}

	cLISTBOX::Select(hKarmaList,g_iSelectKarma);

	if	(g_lpAKC)
		s_lpSelectKarma	=	g_lpAKC->getSelectKarma();
	else
		s_lpSelectKarma	=	NULL;

	s_iKarmaDetailWndPos	=	0;	//

	DrawKarmaDetail(hwnd);
}

void
cKARMA_WND::UpKarma(HWND hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	1)
		return;

	if	(g_iSelectKarma	==	0)
		return;

	int		iSelect		=	g_iSelectKarma;

	CKarma	karma;
	CKarma	*lpKarma1	=	&g_lpAKC->m_pKarma[iSelect];
	CKarma	*lpKarma2	=	&g_lpAKC->m_pKarma[iSelect-1];

	karma.copy(lpKarma1);
	lpKarma1->copy(lpKarma2);
	lpKarma2->copy(&karma);
	g_iSelectKarma--;

	cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
}

void
cKARMA_WND::DownKarma(HWND hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	1)
		return;

	if	(g_iSelectKarma	>=	g_lpAKC->m_wKarmaCount-1)
		return;

	int		iSelect		=	g_iSelectKarma;

	CKarma	karma;
	CKarma	*lpKarma1	=	&g_lpAKC->m_pKarma[iSelect];
	CKarma	*lpKarma2	=	&g_lpAKC->m_pKarma[iSelect+1];

	karma.copy(lpKarma1);
	lpKarma1->copy(lpKarma2);
	lpKarma2->copy(&karma);

	g_iSelectKarma++;

	cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
}

//
//	이벤트 복사
void
cKARMA_WND::CopyKarma(HWND hDlg)
{
	if	(g_lpAKC->getKarmaCount()	<=	0)
		return;

	int		aiSelectKarma[c_iCopyKarmaBufferSize];

	HWND	hList				=	GetDlgItem(hDlg,IDC_KARMA_LIST);
	int		iSelectKarmaCount	=	cLISTBOX::GetSelect(hList,aiSelectKarma,c_iCopyKarmaBufferSize);

	if	(iSelectKarmaCount	==	0)
		return;

	for	(int i=0;i<iSelectKarmaCount;i++)
	{
		CKarma	*lpKarma	=	&g_lpAKC->m_pKarma[aiSelectKarma[i]];

		s_aStoredKarma[i].copy(lpKarma);
		s_aStoredKarma[i].setTitle(_ms("%s[2]",lpKarma->m_pstrTitle));
	}

	s_iCopiedKaramaCount	=	iSelectKarmaCount;

	cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
}

//
//	이벤트 붙여넣기
void
cKARMA_WND::PasteKarma(HWND hDlg)
{
	if	(s_iCopiedKaramaCount	==	0)
		return;		//	조건의 수,반응의 수

	int	iAddedKarma;

	for	(int i=0;i<s_iCopiedKaramaCount;i++)
		iAddedKarma		=	g_lpAKC->add(&s_aStoredKarma[i]);

	if	(iAddedKarma	==	0xffff)
		return;

	g_lpAKC->selectKarma(iAddedKarma);	//	선택

	cKARMA_WND::UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트

	g_lpActmap->LostVirgin();	//	맵의 카르마 세트를 편집중이다.
}

//
//	카르마 메인 윈도우
LRESULT CALLBACK 
cKARMA_WND::s_procKarmaMain(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG	:
			InitKarmaDlg(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDOK);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {	
				case	IDC_NEW				:	
				{
					if (s_bIsEditDialog)
					{
						char	strTitle[1024];

						GetDlgItemText(hDlg,IDC_SPEECH,strTitle,1000);
						g_lpAKC->setTitle(strTitle);
					}

					s_isEditKarma	=	FALSE;
					s_iNewKarma		=	g_lpAKC->m_wKarmaCount;

					CKarma	*lpKarma=	AddKarma(hDlg,NULL,NULL,eADD_KARMA_NORMAL);

					if (!lpKarma)
						break;

					int	iAddedKarma	=	g_lpAKC->add(lpKarma);

					if (iAddedKarma	!=	0xffff)		//	제대로 추가됐다.
					{
						g_lpAKC->selectKarma(iAddedKarma);	//	선택
						UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트
						g_lpActmap->LostVirgin();	//	맵의 카르마 세트를 편집중이다.
					}
					break;
				}

				case	IDC_EDIT			:
				{
					if (s_bIsEditDialog)
					{
						char	strTitle[1024];

						GetDlgItemText(hDlg,IDC_SPEECH,strTitle,1000);
						g_lpAKC->setTitle(strTitle);
					}

					s_isEditKarma	=	TRUE;

					CKarma	*lpKarma	=	g_lpAKC->getSelectKarma();
					CKarma	*lpAddKarma	=	AddKarma(hDlg,NULL,lpKarma,eADD_KARMA_NORMAL);

					if (!lpAddKarma)	break;

					lpKarma->copy(lpAddKarma);
					g_lpAKC->selectKarma(lpKarma->m_wSerial);	//	선택
					UpdateKarmaDlg(hDlg);			//	카르마 윈도우 업데이트

					g_lpActmap->LostVirgin();	//	맵의 카르마 세트를 편집중이다.
					break;
				}

				case	IDC_REMOVE			:
				{
					if (s_bIsEditDialog)
					{
						char	strTitle[1024];

						GetDlgItemText(hDlg,IDC_SPEECH,strTitle,1000);
						g_lpAKC->setTitle(strTitle);
					}

					RemoveKarma(hDlg);
					break;
				}

				case	IDC_KARMA_LIST		:	//	카르마 리스트를 클릭했다.
				{
					if (s_bIsEditDialog)
					{
						char	strTitle[1024];

						GetDlgItemText(hDlg,IDC_SPEECH,strTitle,1000);
						g_lpAKC->setTitle(strTitle);
					}

					SelectKarma(hDlg);
					break;
				}

				case	IDOK				:
				{
					if (s_bIsEditDialog)
					{
						char	strTitle[1024];

						GetDlgItemText(hDlg,IDC_SPEECH,strTitle,1000);
						g_lpAKC->setTitle(strTitle);
					}

					EndDialog(hDlg,IDOK);
					break;
				}
				
				case	IDC_PASTE					:
				{
					PasteKarma(hDlg);
					break;
				}

				case	IDC_COPY					:
				{
					CopyKarma(hDlg);
					break;
				}

				case	IDC_UP_KARAM				:
				{
					UpKarma(hDlg);
					break;
				}
				case	IDC_DOWN_KARAM				:
				{
					DownKarma(hDlg);
					break;
				}

				case	IDCANCEL			:
					EndDialog(hDlg,IDCANCEL);
					break;
			}
			break;
	}

	return (0L);
}


LRESULT CALLBACK 
cKARMA_WND::s_procKarmaDetailWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	SCROLLINFO si;

	si.cbSize	=	sizeof( SCROLLINFO );
	si.fMask	=	SIF_ALL;
	GetScrollInfo( hwnd, SB_VERT, &si );
	
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_hKarmaDetailWnd.Active();
			s_hKarmaDetailWnd.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}
//		case WM_SETCURSOR	:
//			SetCursor(cMAIN::arrowCursor);
//			break;

//		case WM_VSCROLL			:
//			_TRGDLG.VScroll(wParam);
//			break;
		case WM_VSCROLL			:
			switch (LOWORD(wParam)) 
			{
				case SB_LINEUP:
					if (s_iKarmaDetailWndPos > 0)
						s_iKarmaDetailWndPos--;
					break;
				case SB_LINEDOWN:
					if (si.nMax > s_iKarmaDetailWndPos)
						s_iKarmaDetailWndPos++;
					break;
				case SB_PAGEUP:
					if (s_iKarmaDetailWndPos > 0)
						s_iKarmaDetailWndPos-=5;
					if (s_iKarmaDetailWndPos < 0)
						s_iKarmaDetailWndPos = 0;
					break;
				case SB_PAGEDOWN:
					if (si.nMax > s_iKarmaDetailWndPos)
						s_iKarmaDetailWndPos+=5;
					if (si.nMax < s_iKarmaDetailWndPos)
						s_iKarmaDetailWndPos=si.nMax;
					break;
				case SB_THUMBTRACK:
					s_iKarmaDetailWndPos = HIWORD(wParam);
					break;
			}
			DrawKarmaDetail(hwnd);
			break;
		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

void
cKARMA_WND::OperateChangeComboBoxText(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	int		command	=	LOWORD( wParam );
	char	strText[512];
	GetDlgItemText(hDlg,command,strText,512);

	if	(s_iDungeonvalueComboBox	==	command)
	{
		cSecretDungeonValue	*lpString	=	g_sdManager.getString(g_lpActmap->m_wLinkSecretDungeon);

		if	(lpString)
		{
			strncpy(lpString->m_strDungeonValue[s_iSelectDungeonValue],strText,63);
			g_lpActmap->LostVirgin();

			HWND	hCombo	=	GetDlgItem(hDlg,command);

			cCOMBOBOX::RemoveByData(hCombo,s_iSelectDungeonValue);
			cCOMBOBOX::Insert(hCombo,s_iSelectDungeonValue,lpString->m_strDungeonValue[s_iSelectDungeonValue],s_iSelectDungeonValue);
		}
	}
	else
	if	(s_iMainQuestValueComboBox	==	command)
	{
		strncpy(g_strMainQuestValue[s_iSelectMainQuestValue],strText,30);

		HWND	hCombo	=	GetDlgItem(hDlg,command);

		cCOMBOBOX::RemoveByData(hCombo,s_iSelectMainQuestValue);
		cCOMBOBOX::Insert(hCombo,s_iSelectMainQuestValue,g_strMainQuestValue[s_iSelectMainQuestValue],s_iSelectMainQuestValue);
	}
	else
	if	(s_iMainQuestSwitchComboBox	==	command)
	{
		strncpy(g_strMainQuestSwitch[s_iSelectMainQuestSwitch],strText,30);

		HWND	hCombo	=	GetDlgItem(hDlg,command);

		cCOMBOBOX::RemoveByData(hCombo,s_iSelectMainQuestSwitch);
		cCOMBOBOX::Insert(hCombo,s_iSelectMainQuestSwitch,g_strMainQuestSwitch[s_iSelectMainQuestSwitch],s_iSelectMainQuestSwitch);
	}
}

void
cKARMA_WND::OperateSelectChangeComboBox(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam,BOOL _bIsForReactioin)
{
	int	command	=	LOWORD( wParam );

	if	(s_iDungeonvalueComboBox	==	command	||	s_iMainQuestValueComboBox	==	command	||	s_iMainQuestSwitchComboBox	==	command)
	{
		if	(_bIsForReactioin)
		{
			ReadKarmaText(hDlg,g_aReaction[g_iSelectTerm].m_wSerial,g_aReaction[g_iSelectTerm].m_strReferenceText,(void *)&g_EditReaction);
			WriteKarmaText(hDlg,g_aReaction[g_iSelectTerm].m_strReferenceText,g_EditReaction.m_aValue,g_EditReaction.m_strString);
		}
		else
		{
			ReadKarmaText(hDlg,g_aTrigger[g_iSelectTerm].m_wSerial,g_aTrigger[g_iSelectTerm].m_strReferenceText,(void *)&g_EditTrigger);
			WriteKarmaText(hDlg,g_aTrigger[g_iSelectTerm].m_strReferenceText,g_EditTrigger.m_aValue,NULL);
		}
	}
}
