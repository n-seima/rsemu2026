#include "CKarma.H"
#include "resource.H"
#include "cDRAW.H"
#include "cMESSAGE.H"
#include "cTEXT.H"
#include "cMAP.H"
#include "cSetDialogKarma.h"					// 09.12.03 추가

cDIBWND		cKARMA_WND::s_hTriggerWnd;
cDIBWND		cKARMA_WND::s_hReactionWnd;
int			cKARMA_WND::s_iTriggerWndPos	=	0;		//	위치
int			cKARMA_WND::s_iReactionWndPos	=	0;
int			cKARMA_WND::s_iEditType			=	0;
char		cKARMA_WND::s_strTitlebar[128];
CKarma		*cKARMA_WND::s_lpkcEdit			=	NULL;
CKarma		cKARMA_WND::s_storedKarmaForDetail;
HWND		cKARMA_WND::s_hAddKarmaDlg;
cSCROLL_BAR cKARMA_WND::s_sbReaction;
cSCROLL_BAR cKARMA_WND::s_sbTrigger;
CKarmaItem	cKARMA_WND::s_storedTrigger;
CKarmaItem	cKARMA_WND::s_storedReaction;

int			g_iSelectTerm		=	0;

#define		IDC_COPY_KARMA			1000
#define		IDC_CUT_KARMA			1001
#define		IDC_PASTE_KARMA			1002

//
//	새로운 카르마를 추가하거나 편집한다.
CKarma *
cKARMA_WND::AddKarma(HWND hdlg,char *_lpstrTitlebar,CKarma *_lpEditKarma,int _iType)
{
	s_iEditType	=	_iType;

	if (_lpstrTitlebar)
		strcpy(s_strTitlebar,_lpstrTitlebar);
	else
		strcpy(s_strTitlebar,"");

	s_lpkcEdit			=	_lpEditKarma;

	s_iTriggerWndPos	=	0;
	s_iReactionWndPos	=	0;

	g_iLastReaction		=	g_iSelectReaction;
	g_iLastTrigger		=	g_iSelectTrigger;

	int	result;

	switch	(_iType)
	{
		case	eADD_KARMA_NORMAL			:
			result	=	DialogBox( s_hInst,"KARMA_DETAIL"				,hdlg,(DLGPROC)s_procAddKarma);
			break;
// 		case	eADD_KARMA_SELECT_DIALOG	:
// 			result	=	DialogBox( s_hInst,"SET_DIALOG_SELECT_SPEECH"	,hdlg,(DLGPROC)s_procAddKarma);
// 			break;
	}

	if	(result	==	IDCANCEL)
		return NULL;

	return	&g_EditKarma;
}	//	cKARMA_WND::AddKarma(HWND hdlg)

//
//	카르마 추가 대화상자를 초기화한다.
void
cKARMA_WND::InitAddKarmaDlg(HWND hwnd)
{
	s_storedKarmaForDetail.allocBuffer(dTRIGGER_COUNT,dREACTION_COUNT);
	s_storedKarmaForDetail.reset(TRUE);

	HWND	hTriggerWnd		=	GetDlgItem(hwnd,IDC_TRIGGER_WND);		//	카르마 트리거 윈도우
	HWND	hReactionWnd	=	GetDlgItem(hwnd,IDC_REACTION_WND);		//	카르마 반응 윈도우
	HWND	hOkButton		=	GetDlgItem(hwnd,IDOK);					//	OK 버튼

	s_hAddKarmaDlg	=	hwnd;

	if	(s_lpkcEdit)			//	카르마를 편집하는 거다.
	{
		g_EditKarma.copy(s_lpkcEdit,TRUE);	//	복사한다.

		EnableWindow(hOkButton,TRUE);
	}
	else
	{
		g_EditKarma.reset(TRUE);	//	편집할 카르마 초기화

		if	(s_iNewKarma == 0)
			g_EditKarma.setTitle("default");
		else
			g_EditKarma.setTitle(_ms("case #%.3d",s_iNewKarma));

		EnableWindow(hOkButton,FALSE);
	}

	if	(s_strTitlebar[0])
		SetWindowText(hwnd,s_strTitlebar);

	SetDlgItemText(hwnd,IDC_NAME,g_EditKarma.getTitle());
	SetDlgItemInt(hwnd,IDC_ACTIVATE_CHANCE,g_EditKarma.m_wActivateChance,FALSE);
	SetDlgItemInt(hwnd,IDC_ACTIVATE_TRIGGER_COUNT,g_EditKarma.m_bf7ActivateTriggerCount,FALSE);
	SetDlgItemInt(hwnd,IDC_CHECK_PERIOD,g_EditKarma.m_wActivePeriod,FALSE);
	SetDlgItemInt(hwnd,IDC_LIMIT_ACTIVE_COUNT,g_EditKarma.m_wActiveCount,FALSE);
	CheckDlgButton(hwnd,IDC_IS_ACTIVE,1-g_EditKarma.m_bf1IsDisable);			//	and? or?
	CheckDlgButton(hwnd,IDC_IS_APPLY_TO_ENTER_FIELD_PLAYER,g_EditKarma.m_bf1IsForEnterFieldPlayer);			//	and? or?
	CheckRadioButton(hwnd,IDC_OR	,IDC_AND	,IDC_OR		+g_EditKarma.m_isWantAndCondition);			//	and? or?
	CheckDlgButton(hwnd	,IDC_ACTIVE_CONDITION,g_EditKarma.m_isWantNotComplete);			//	and? or?
	CheckDlgButton(hwnd	,IDC_IS_APPLY_ALL_PARTY_MEMBER,g_EditKarma.m_bf1IsApplyAllMember);			//	and? or?

	SCROLLINFO	si;

	{
		si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	3;
		si.nPage	=	3;
		si.nPos		=	0;
		EnableScrollBar	(hTriggerWnd,SB_VERT,ESB_DISABLE_BOTH);
		SetScrollInfo	(hTriggerWnd,SB_VERT,&si,TRUE);

		s_hTriggerWnd.lpszRegister	=	"TRIGGER_WINDOW";
		s_hTriggerWnd.Init(hTriggerWnd,(WNDPROC)s_procTriggerWnd);

		s_sbTrigger.Init(s_hTriggerWnd.hWND);
		s_sbTrigger.InitHeight();

		DrawTriggerWnd(hwnd);
	}

	{
		si.cbSize	=	sizeof(SCROLLINFO);
		si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
		si.nMin		=	0;
		si.nMax		=	3;
		si.nPage	=	3;
		si.nPos		=	0;
		EnableScrollBar	(hReactionWnd,SB_VERT,ESB_DISABLE_BOTH);
		SetScrollInfo	(hReactionWnd,SB_VERT,&si,TRUE);

		s_hReactionWnd.lpszRegister	=	"REACTION_WINDOW";
		s_hReactionWnd.Init(hReactionWnd,(WNDPROC)s_procReactionWnd);

		s_sbReaction.Init(s_hReactionWnd.hWND);
		s_sbReaction.InitHeight();
		s_sbReaction.SetHeightPos(s_iReactionWndPos);

		DrawReactionWnd(hwnd);
	}
}	//	cKARMA_WND::InitAddKarmaDlg(HWND hwnd)

//
//	카르마를 추가한다.
BOOL
cKARMA_WND::ReadAddKarmaDlg(HWND hdlg)
{
	if (s_iEditType	!=	eADD_KARMA_SELECT_DIALOG)
		if (g_EditKarma.m_wReactionCount<=	0)
			return	FALSE;

	char	strTitle[2048];
	BOOL	temp;

	GetDlgItemText(hdlg,IDC_NAME,strTitle,sizeof(strTitle));	//	카르마 이름

	g_EditKarma.setTitle(strTitle);

	g_EditKarma.m_wActivateChance	=	GetDlgItemInt(hdlg,IDC_ACTIVATE_CHANCE,&temp,FALSE);	//	카르마 이름

	int	iActivateTriggerCount		=	GetDlgItemInt(hdlg,IDC_ACTIVATE_TRIGGER_COUNT,&temp,FALSE);	//	카르마 이름
	g_EditKarma.m_wActivePeriod		=	GetDlgItemInt(hdlg,IDC_CHECK_PERIOD,&temp,FALSE);	//	카르마 이름
	g_EditKarma.m_wActiveCount		=	GetDlgItemInt(hdlg,IDC_LIMIT_ACTIVE_COUNT,&temp,FALSE);	//	카르마 이름

	if	(iActivateTriggerCount	<	0	||	iActivateTriggerCount	>=	128)
	{
		cMSG::Error("카르마 발동을 위한 최소 만족 트리거 수 설정 에러!!","설정 가능 개수는 0~127입니다.");

		return	FALSE;
	}

	g_EditKarma.m_bf7ActivateTriggerCount	=	iActivateTriggerCount;

	if	(IsDlgButtonChecked(hdlg,IDC_IS_ACTIVE))
		g_EditKarma.m_bf1IsDisable	=	FALSE;		//	트리거 앤드 조건
	else
		g_EditKarma.m_bf1IsDisable	=	TRUE;

	g_EditKarma.m_bf1IsApplyAllMember	=	IsDlgButtonChecked(hdlg,IDC_IS_APPLY_ALL_PARTY_MEMBER);

	if	(IsDlgButtonChecked(hdlg,IDC_IS_APPLY_TO_ENTER_FIELD_PLAYER))
		g_EditKarma.m_bf1IsForEnterFieldPlayer	=	TRUE;		//	트리거 앤드 조건
	else
		g_EditKarma.m_bf1IsForEnterFieldPlayer	=	FALSE;

	if	(IsDlgButtonChecked(hdlg,IDC_AND))
		g_EditKarma.m_isWantAndCondition	=	TRUE;		//	트리거 앤드 조건
	else
		g_EditKarma.m_isWantAndCondition	=	FALSE;

	if	(IsDlgButtonChecked(hdlg,IDC_ACTIVE_CONDITION))
		g_EditKarma.m_isWantNotComplete	=	TRUE;		//	트리거 앤드 조건
	else
		g_EditKarma.m_isWantNotComplete	=	FALSE;

	return	TRUE;
}


void
cKARMA_WND::UpReaction(HWND _hDlg)
{
	if (g_EditKarma.m_wReactionCount<=	1)	return;
	if (g_iSelectReaction			==	0)	return;

	CKarmaItem	reaction;
	CKarmaItem	*lpSelectReaction	=	&g_EditKarma.m_pReaction[g_iSelectReaction];
	CKarmaItem	*lpDestReaction		=	&g_EditKarma.m_pReaction[g_iSelectReaction-1];

	reaction.copy(lpSelectReaction);
	lpSelectReaction->copy(lpDestReaction);
	lpDestReaction->copy(&reaction);

	g_iSelectReaction--;
	g_iLastReaction	=	g_iSelectReaction;
	DrawReactionWnd(_hDlg);
}	//	cKARMA_WND::UpReaction(HWND _hDlg)

void
cKARMA_WND::DownReaction(HWND _hDlg)
{
	if (g_EditKarma.m_wReactionCount<=	1)	return;
	if (g_iSelectReaction			>=	g_EditKarma.m_wReactionCount-1)	return;

	CKarmaItem	reaction;
	CKarmaItem	*lpSelectReaction	=	&g_EditKarma.m_pReaction[g_iSelectReaction];
	CKarmaItem	*lpDestReaction		=	&g_EditKarma.m_pReaction[g_iSelectReaction+1];

	reaction.copy(lpSelectReaction);
	lpSelectReaction->copy(lpDestReaction);
	lpDestReaction->copy(&reaction);

	g_iSelectReaction++;
	g_iLastReaction	=	g_iSelectReaction;

	DrawReactionWnd(_hDlg);
}	//	cKARMA_WND::DownReaction(HWND _hDlg)



void
cKARMA_WND::UpTrigger(HWND _hDlg)
{
	if (g_EditKarma.m_wTriggerCount <=1) return;
	if (g_iSelectTrigger == 0) return;
	
	CKarmaItem	trigger;
	CKarmaItem	*lpSelectTrigger	=	&g_EditKarma.m_pTrigger[g_iSelectTrigger];
	CKarmaItem	*lpDestTrigger		=	&g_EditKarma.m_pTrigger[g_iSelectTrigger-1];
	
	trigger.copy(lpSelectTrigger);
	lpSelectTrigger->copy(lpDestTrigger);
	lpDestTrigger->copy(&trigger);
	
	g_iSelectTrigger--;
	g_iLastTrigger	=	g_iSelectTrigger;
	
	DrawTriggerWnd(_hDlg);
}



void
cKARMA_WND::DownTrigger(HWND _hDlg)
{
	if (g_EditKarma.m_wTriggerCount <=1) return;
	if (g_iSelectTrigger >= g_EditKarma.m_wTriggerCount-1) return;
	
	CKarmaItem	trigger;
	CKarmaItem	*lpSelectTrigger	=	&g_EditKarma.m_pTrigger[g_iSelectTrigger];
	CKarmaItem	*lpDestTrigger		=	&g_EditKarma.m_pTrigger[g_iSelectTrigger+1];
	
	trigger.copy(lpSelectTrigger);
	lpSelectTrigger->copy(lpDestTrigger);
	lpDestTrigger->copy(&trigger);
	
	g_iSelectTrigger++;
	g_iLastTrigger	=	g_iSelectTrigger;
	
	DrawTriggerWnd(_hDlg);
}


//
//	카르마 추가 프로콜
LRESULT CALLBACK 
cKARMA_WND::s_procAddKarma(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{	
	switch(message)
	{
		case WM_INITDIALOG	:
			InitAddKarmaDlg(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDC_UP_TRIGGER		:
				{
					UpTrigger(hDlg);
				}
				break;

				case	IDC_DOWN_TRIGGER		:
				{
					DownTrigger(hDlg);
				}
				break;
				case	IDC_NEW_TRIGGER		:
				{
//					strcpy(g_strGroupName,s_lpSelectGroup->m_strName);
					GetDlgItemText(hDlg,IDC_NAME,g_strKarmaName,127);

					s_isEditTrigger	=	FALSE;
					int	trigger		=	AddTrigger(hDlg);

					if(trigger	!=	0xffff)
					{
						g_iLastTrigger	=	g_iSelectTrigger;

						DrawTriggerWnd(hDlg);

						if (g_EditKarma.m_wReactionCount	>	0	)
							EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
						else
							EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
					}
					break;
				}

				case	IDC_EDIT_TRIGGER	:	//	트리거 편집
				{
//					strcpy(g_strGroupName,s_lpSelectGroup->m_strName);
					GetDlgItemText(hDlg,IDC_NAME,g_strKarmaName,127);

					s_isEditTrigger	=	TRUE;
					int	trigger		=	AddTrigger(hDlg);

					if(trigger	!=	0xffff)
					{
						g_iLastReaction	=	g_iSelectTrigger;

						DrawTriggerWnd(hDlg);

						if (g_EditKarma.m_wReactionCount	>	0	)	EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
						else											EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
					}
					break;
				}

				case	IDC_REMOVE_TRIGGER	:	//	트리거 제거
					RemoveTrigger(hDlg);
					break;

				case	IDC_NEW_REACTION	:
				{
//					strcpy(g_strGroupName,s_lpSelectGroup->m_strName);
					GetDlgItemText(hDlg,IDC_NAME,g_strKarmaName,127);

					s_isEditReaction	=	FALSE;
					int	reaction		=	AddReaction(hDlg);

					if(reaction	!=	0xffff)
					{
						g_iLastReaction	=	g_iSelectReaction;

						DrawReactionWnd(hDlg);

						if (g_EditKarma.m_wReactionCount	>	0	)
								EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
						else	EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
	
					}
					break;
				}

				case	IDC_EDIT_REACTION	:		//	리액션 편집
				{
//					strcpy(g_strGroupName,s_lpSelectGroup->m_strName);
					GetDlgItemText(hDlg,IDC_NAME,g_strKarmaName,127);

					s_isEditReaction	=	TRUE;
					int	reaction		=	AddReaction(hDlg);

					if(reaction	!=	0xffff)
					{
						g_iLastReaction	=	g_iSelectReaction;

						DrawReactionWnd(hDlg);

						if (g_EditKarma.m_wReactionCount	>	0	)
								EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
						else	EnableWindow(GetDlgItem(hDlg,IDOK),FALSE);
					}
					break;
				}


				case	IDC_UP_REACTION		:
					UpReaction(hDlg);
					break;

				case	IDC_DOWN_REACTION		:
					DownReaction(hDlg);
					break;


				case	IDC_REMOVE_REACTION	:		//	리액션 제거
					RemoveReaction(hDlg);
					break;

				case	IDOK				:		//	카르마 추가 혹은 편집 완료
					if (ReadAddKarmaDlg(hDlg))	EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL			:		//	카르마 추가 혹은 편집 취소
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_AND				:
				case	IDC_OR				:
					CheckRadioButton(hDlg,IDC_OR	,IDC_AND	,LOWORD( wParam ));	//	독점 실행이냐?
					break;

				case	IDC_GATHER			:
				case	IDC_SOLO			:
					CheckRadioButton(hDlg,IDC_GATHER,IDC_SOLO	,LOWORD( wParam ));	//	독점 실행이냐?
					break;

				case	IDC_COPY_TRIGGER	:
					CopyTrigger(hDlg);
					break;

				case	IDC_PASTE_TRIGGER	:
					PasteTrigger(hDlg);
					break;

				case	IDC_COPY_REACTION	:
					CopyReaction(hDlg);
					break;

				case	IDC_PASTE_REACTION	:
					PasteReaction(hDlg);
					break;

			}
			break;
	}

	return (0L);
}

///////////////////////////////////////////////////////////////////////////////////////
//	트리거 관련
///////////////////////////////////////////////////////////////////////////////////////
//
//	선택한 리액션을 제거한다.
void
cKARMA_WND::CopyTrigger(HWND hdlg)
{
	int	iBegin,iEnd,i;

	if	(!s_storedKarmaForDetail.m_pReaction)
		s_storedKarmaForDetail.reset(TRUE);

	if	(g_iLastTrigger	>	g_iSelectTrigger)
		iBegin	=	g_iSelectTrigger	,iEnd	=	g_iLastTrigger;
	else	
		iBegin	=	g_iLastTrigger		,iEnd	=	g_iSelectTrigger;

	s_storedKarmaForDetail.m_wTriggerCount	=	iEnd-iBegin+1;

	for (i=iBegin;i<=iEnd;i++)
		s_storedKarmaForDetail.m_pTrigger[i-iBegin].copy(&g_EditKarma.m_pTrigger[i]);

	DrawTriggerWnd(hdlg);
}

//
//	선택한 리액션을 잘라낸다.
void
cKARMA_WND::CutTrigger(HWND hdlg)
{
	CopyTrigger(hdlg);
	RemoveTrigger(hdlg,FALSE);
}

//
//	선택한 리액션을 붙여 넣는다.
void
cKARMA_WND::PasteTrigger(HWND hdlg)
{
	g_EditKarma.PasteTrigger(s_iPasteTriggerPos,&s_storedKarmaForDetail);

	DrawTriggerWnd(hdlg);
}

//
//	선택한 리액션을 제거한다.
void
cKARMA_WND::RemoveTrigger(HWND hdlg,BOOL safe)
{
	int	iBegin,iEnd,iCount;

	if (g_iLastTrigger	>	g_iSelectTrigger)	iBegin	=	g_iSelectTrigger	,iEnd	=	g_iLastTrigger;
	else										iBegin	=	g_iLastTrigger		,iEnd	=	g_iSelectTrigger;

	iCount	=	iEnd	-	iBegin+1;

	if (iCount > 1 && safe)
		if (cMSG::YESNO(dMSG_REMOVE_SELECT_TRIGGER,dMSG_QUESTION_CONTINUE)==IDNO)	
			return;

	g_EditKarma.RemoveTrigger(iBegin,iCount);	//	iBegin부터 iCount개를 지운다.

	if (g_iSelectTrigger	>=	g_EditKarma.m_wTriggerCount && g_EditKarma.m_wTriggerCount	>	0)
		g_iSelectTrigger	=	g_EditKarma.m_wTriggerCount-1;

	g_iLastTrigger	=	g_iSelectTrigger;

	DrawTriggerWnd(hdlg);
}	//	cKARMA_WND::RemoveTrigger(HWND hdlg)


//
//	선택된 트리거냐?
BOOL
cKARMA_WND::IsSelectTrigger(int index)
{
	if (g_iLastTrigger	>	g_iSelectTrigger)
	{
		if (index <= g_iLastTrigger && index >= g_iSelectTrigger)	return	TRUE;
	}
	else	if (index >= g_iLastTrigger && index <= g_iSelectTrigger)	return	TRUE;

	return	FALSE;
}	//	cKARMA_WND::IsSelectTrigger(int index)

//
//	트리거를 선택한다.
int
cKARMA_WND::GetSelectTrigger(HWND hwnd)
{
	int		x,y,i,iWidth;
	cPOINT	Pos;

	Pos.MousePos(hwnd);

	x		=	5;
	y		=	5;
	iWidth	=	s_hTriggerWnd.WIDTH-24;

	for (i	=	s_iTriggerWndPos;i<g_EditKarma.m_wTriggerCount;i++)
	{
		char	*lpText		=	KarmaText(&g_EditKarma.m_pTrigger[i],TRUE);
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		if (y	>=	y	+	iLineCount*14+10)	break;

		cRECT	rect;

		rect.Set(0,y,iWidth,y+iLineCount*14+4+4);

		if (rect.isIN(Pos.x,Pos.y))	return i;

		y					+=	iLineCount*14;
		y					+=	4+4;

		if (y	>=	cDRAW::HEIGHT)	break;
	}

	return	0xffff;
}	//	cKARMA_WND::SelectTrigger(HWND hwnd)

//
//	트리거를 선택한다.
int
cKARMA_WND::SelectTrigger(HWND hwnd)
{
	int	 iSelectTrigger	=	GetSelectTrigger(hwnd);

	if (iSelectTrigger	==	0xffff)
	{
		g_iLastTrigger	=	g_iSelectTrigger;
		DrawTriggerWnd(s_hAddKarmaDlg);
		return	0xffff;
	}

	cINPDEV::Update();

	if(g_iSelectTrigger	!=	iSelectTrigger || KEY[dKEY_SHIFT])
	{
		if (KEY[dKEY_SHIFT])	g_iLastTrigger	=	iSelectTrigger;
		else
		{	g_iSelectTrigger=	iSelectTrigger;
			g_iLastTrigger	=	iSelectTrigger;
		}

		DrawTriggerWnd(s_hAddKarmaDlg);

		return	iSelectTrigger;
	}

	return	0xffff;
}	//	cKARMA_WND::SelectTrigger(HWND hwnd)

//
//	트리거 팝업
void
cKARMA_WND::PopupTrigger(HWND hdlg)
{
	if	(g_EditKarma.m_wTriggerCount	<=	0)
		return;

	int	iSelectTrigger	=	GetSelectTrigger(hdlg),iPasteMask=0;

	POINT	Pos;
	HMENU	hMenu	=	CreatePopupMenu();

	s_iPasteTriggerPos	=	iSelectTrigger;

	if	(s_storedKarmaForDetail.m_wTriggerCount ==	0	)
		iPasteMask	=	MF_GRAYED;	//	복사해 둔게 없다.

	AppendMenu(hMenu,MFT_STRING				,IDC_COPY_KARMA		,dMSG_COPY);
	AppendMenu(hMenu,MFT_STRING				,IDC_CUT_KARMA		,dMSG_CUT);
	AppendMenu(hMenu,MFT_STRING|iPasteMask	,IDC_PASTE_KARMA		,dMSG_PASTE);

	GetCursorPos(&Pos);
	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,Pos.x,Pos.y,0,hdlg,NULL);
	DestroyMenu(hMenu);
}

//
//	등록된 트리거들을 그려준다.
void
cKARMA_WND::DrawTriggerWnd(HWND hwnd)
{
	int	x,y,i,iWidth;

	if (g_EditKarma.m_wTriggerCount	<=	0)	//	리액션 카운트가 0이다.
	{
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_COPY_TRIGGER	),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE_TRIGGER	),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT_TRIGGER	),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDC_COPY_TRIGGER	),TRUE);
	}

	if (!s_hTriggerWnd.Active()) return;

	x		=	5;
	y		=	5;
	iWidth	=	cDRAW::WIDTH-24;

	cDRAW::Fill(_SKYBLUE);

	if (g_EditKarma.m_wTriggerCount <= 0)
	{
		cTEXT::PushShadow(FALSE);
		cTEXT::Put(x+1,y+2,0,"특별히 설정된 상황이 없습니다. 상황에 관계없이 작업이 실행 됩니다.");
		cTEXT::PopShadow();
	}

	for (i	=	s_iTriggerWndPos;i<g_EditKarma.m_wTriggerCount;i++)
	{
		char	*lpText		=	KarmaText(&g_EditKarma.m_pTrigger[i],TRUE);
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		if (y	>=	y	+	iLineCount*14+10)	break;

		cDRAW::Fill		(0,x,y,x+iWidth+6,y+iLineCount*14+4);

		if(i==g_iSelectTrigger)
		{
			cDRAW::Fill		(_BLUE,x-1,y-1,x+iWidth+5,y+iLineCount*14+3);
			cTEXT::FitPut	(x+1,y+2,iWidth,iLineCount*8*2,WHITE,lpText);
		}else
		{
			if (IsSelectTrigger(i))
					cDRAW::Fill		(_DEEPBLUE,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);
			else	cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);

			cTEXT::FitPut	(x,y+1,iWidth,iLineCount*8*2,WHITE,lpText);
		}

		y					+=	iLineCount*14;
		y					+=	4+4;

		if (y	>=	cDRAW::HEIGHT)	break;
	}

	s_hTriggerWnd.Draw();
}

//
//	트리거 윈도우
LRESULT CALLBACK 
cKARMA_WND::s_procTriggerWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_hTriggerWnd.Active();
			s_hTriggerWnd.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}
		
		case	WM_LBUTTONDOWN	:
			SelectTrigger(hwnd);
			if(CSetDialogKarma::s_bIsEnableDialog)
			{
				BOOL temp;
				temp = CSetDialogKarma::ChangeKarmaTab(0);
				
				if(!temp)
					CSetDialogKarma::InitTrigger(CSetDialogKarma::s_hTrigger);
			}
			break;

		case	WM_RBUTTONDOWN	:
			PopupTrigger(hwnd);
			break;

		case	WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {
				case	IDC_COPY_KARMA		:
					CopyTrigger(hwnd);
					break;

				case	IDC_CUT_KARMA		:
					CutTrigger(hwnd);
					break;

				case	IDC_PASTE_KARMA	:
					PasteTrigger(hwnd);
					break;
			}
			break;

//		case WM_SETCURSOR	:
//			SetCursor(cMAIN::arrowCursor);
//			break;

//		case WM_VSCROLL			:
//			_TRGDLG.VScroll(wParam);
//			break;

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}	//	cKARMA_WND::s_procTriggerWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	반응관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	선택한 리액션을 제거한다.
void
cKARMA_WND::CopyReaction(HWND hdlg)
{
	int	iBegin,iEnd,i;

	if	(g_iLastReaction	>	g_iSelectReaction)
		iBegin	=	g_iSelectReaction	,iEnd	=	g_iLastReaction;
	else
		iBegin	=	g_iLastReaction		,iEnd	=	g_iSelectReaction;

	s_storedKarmaForDetail.m_wReactionCount	=	iEnd-iBegin+1;

	for (i=iBegin;i<=iEnd;i++)	
		s_storedKarmaForDetail.m_pReaction[i-iBegin].copy(&g_EditKarma.m_pReaction[i]);
}	//	cKARMA_WND::CopyReaction(HWND hdlg)

//
//	선택한 리액션을 잘라낸다.
//
void
cKARMA_WND::CutReaction(HWND hdlg)
{
	CopyReaction(hdlg);
	RemoveReaction(hdlg,FALSE);
}	//	cKARMA_WND::CutReaction(HWND hdlg)

//
//	선택한 리액션을 붙여 넣는다.
void
cKARMA_WND::PasteReaction(HWND hdlg)
{
	g_EditKarma.PasteReaction(s_iPasteReactionPos,&s_storedKarmaForDetail);

	DrawReactionWnd(hdlg);
}	//	cKARMA_WND::PasteReaction(HWND hdlg)

//
//	선택한 리액션을 제거한다.
void
cKARMA_WND::RemoveReaction(HWND hdlg,BOOL safe)
{
	int	iBegin,iEnd,iCount;

	if (g_iLastReaction	>	g_iSelectReaction)	iBegin	=	g_iSelectReaction	,iEnd	=	g_iLastReaction;
	else										iBegin	=	g_iLastReaction		,iEnd	=	g_iSelectReaction;

	iCount	=	iEnd	-	iBegin+1;

	if (iCount > 1 && safe)
		if (cMSG::YESNO(dMSG_REMOVE_SELECT_REACTION,dMSG_QUESTION_CONTINUE)==IDNO)	
			return;

	g_EditKarma.RemoveReaction(iBegin,iCount);	//	iBegin부터 iCount개를 지운다.

	if (g_iSelectReaction	>=	g_EditKarma.m_wReactionCount && g_EditKarma.m_wReactionCount	>	0)
		g_iSelectReaction	=	g_EditKarma.m_wReactionCount-1;

	g_iLastReaction	=	g_iSelectReaction;

	DrawReactionWnd(hdlg);
}	//	cKARMA_WND::RemoveReaction(HWND hdlg)


//
//	선택된 리액션이냐?
BOOL
cKARMA_WND::IsSelectReaction(int index)
{
	if (g_iLastReaction	>	g_iSelectReaction)
	{
		if (index <= g_iLastReaction && index >= g_iSelectReaction)	return	TRUE;
	}
	else	if (index >= g_iLastReaction && index <= g_iSelectReaction)	return	TRUE;

	return	FALSE;
}	//	cKARMA_WND::IsSelectReaction(int index)

//
//	선택한 리액션을 얻는다.
int
cKARMA_WND::GetSelectReaction(HWND hwnd)
{
	int		x,y,i,iWidth;
	cPOINT	Pos;

	Pos.MousePos(hwnd);

	x		=	5;
	y		=	5-s_sbReaction.GetHeightPos();
	iWidth	=	s_hReactionWnd.WIDTH-24;

	for (i	=	0;i<g_EditKarma.m_wReactionCount;i++)
	{
		char	*lpText		=	KarmaText(&g_EditKarma.m_pReaction[i],FALSE);
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		if (y	>=	y	+	iLineCount*14+10)	break;

		cRECT	rect;

		rect.Set(0,y,iWidth,y+iLineCount*14+4);

		if (rect.isIN(Pos.x,Pos.y))	return i;

		y					+=	iLineCount*14;
		y					+=	4+4;

		if (y	>=	cDRAW::HEIGHT)	break;
	}

	return	0xffff;

}	//	cKARMA_WND::SelectReaction(HWND hwnd)

//
//	액션을 선택한다.
int
cKARMA_WND::SelectReaction(HWND hwnd)
{
	int	 iSelectReaction	=	GetSelectReaction(hwnd);

	if (iSelectReaction	==	0xffff)
	{
		g_iLastReaction	=	g_iSelectReaction;

		DrawReactionWnd(s_hAddKarmaDlg);
		return	0xffff;
	}

	cINPDEV::Update();

	if(g_iSelectReaction	!=	iSelectReaction || KEY[dKEY_SHIFT])
	{
		if (KEY[dKEY_SHIFT])	g_iLastReaction	=	iSelectReaction;
		else
		{	g_iSelectReaction=	iSelectReaction;
			g_iLastReaction	=	iSelectReaction;
		}

		DrawReactionWnd(s_hAddKarmaDlg);

		return	iSelectReaction;
	}

	return	0xffff;
}	//	cKARMA_WND::SelectReaction(HWND hwnd)

//
//	리액션 팝업
void
cKARMA_WND::PopupReaction(HWND hdlg)
{
	if	(g_EditKarma.m_wReactionCount	<=	0)
		return;

	int	iSelectReaction	=	GetSelectReaction(hdlg),iPasteMask=0;

	POINT	Pos;
	HMENU	hMenu	=	CreatePopupMenu();

	s_iPasteReactionPos	=	iSelectReaction;

	if	(s_storedKarmaForDetail.m_wReactionCount	==	0	)
		iPasteMask	=	MF_GRAYED;	//	복사해 둔게 없다.

	AppendMenu(hMenu,MFT_STRING				,IDC_COPY_KARMA		,dMSG_COPY);
	AppendMenu(hMenu,MFT_STRING				,IDC_CUT_KARMA		,dMSG_CUT);
	AppendMenu(hMenu,MFT_STRING|iPasteMask	,IDC_PASTE_KARMA		,dMSG_PASTE);

	GetCursorPos(&Pos);
	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,Pos.x,Pos.y,0,hdlg,NULL);
	DestroyMenu(hMenu);
}	//	cKARMA_WND::PopupReaction(HWND hdlg)

//
//	등록된 반응들을 그려준다.
void
cKARMA_WND::DrawReactionWnd(HWND hwnd)
{
	int	x,y,i,iWidth;

	if (g_EditKarma.m_wReactionCount	<=	0)	//	리액션 카운트가 0이다.
	{
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE_REACTION),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT_REACTION	),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDOK				),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_UP_REACTION	),FALSE);
		EnableWindow(GetDlgItem(hwnd,IDC_DOWN_REACTION	),FALSE);
		
	}
	else
	{
		EnableWindow(GetDlgItem(hwnd,IDC_REMOVE_REACTION),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDC_EDIT_REACTION	),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDOK				),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDC_UP_REACTION	),TRUE);
		EnableWindow(GetDlgItem(hwnd,IDC_DOWN_REACTION	),TRUE);

		if (g_EditKarma.m_wReactionCount	<=	1)
		{
			EnableWindow(GetDlgItem(hwnd,IDC_UP_REACTION	),FALSE);
			EnableWindow(GetDlgItem(hwnd,IDC_DOWN_REACTION	),FALSE);
		}
		else
		{
			if (g_iSelectReaction	==	0)
				EnableWindow(GetDlgItem(hwnd,IDC_UP_REACTION	),FALSE);

			if (g_iSelectReaction	>=	g_EditKarma.m_wReactionCount-1)
				EnableWindow(GetDlgItem(hwnd,IDC_DOWN_REACTION	),FALSE);
		}
	}

	if (!s_hReactionWnd.Active()) return;

	x		=	5;
	y		=	5;
	iWidth	=	cDRAW::WIDTH-24;

	cDRAW::Fill(_SKYBLUE);

	if (g_EditKarma.m_wReactionCount <= 0)
	{
		cTEXT::PushShadow(FALSE);
		cTEXT::Put(x+1,y+2,0,"설정된 작업이 없습니다.");
		cTEXT::PopShadow();
	}

	s_iReactionWndPos	=	s_sbReaction.GetHeightPos();

	for (i = 0;i<g_EditKarma.m_wReactionCount;i++)
	{
		char	*lpText		=	KarmaText(&g_EditKarma.m_pReaction[i],FALSE);
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		y					+=	iLineCount*14;
		y					+=	4+4;
	}

	s_sbReaction.SetHeight(y,cDRAW::HEIGHT);
	s_sbReaction.SetHeightPos(s_iReactionWndPos);

	y		=	5-s_sbReaction.GetHeightPos();

	for (i	=	0;i<g_EditKarma.m_wReactionCount;i++)
	{
		char	*lpText		=	KarmaText(&g_EditKarma.m_pReaction[i],FALSE);
		char	strText[1024];

		sprintf(strText," #%.2d]%s",i,lpText);

		int		iLineCount	=	cTEXT::GetLines(iWidth,strText);

		cDRAW::Fill		(0,x,y,x+iWidth+6,y+iLineCount*14+4);

		if	(i==g_iSelectReaction)
		{
			cDRAW::Fill		(_BLUE,x-1,y-1,x+iWidth+5,y+iLineCount*14+3);
			cTEXT::FitPut	(x+1,y+2,iWidth,iLineCount*8*2,WHITE,strText);
		}
		else
		{
			if (IsSelectReaction(i))
					cDRAW::Fill		(_DEEPBLUE,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);
			else	cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);

			cTEXT::FitPut	(x,y+1,iWidth,iLineCount*8*2,WHITE,strText);
		}

		y					+=	iLineCount*14;
		y					+=	4+4;

		if (y	>=	cDRAW::HEIGHT)	break;
	}

	s_hReactionWnd.Draw();
}	//	cKARMA_WND::DrawReactionWnd(HWND hwnd)

//
//	리액션 윈도우
LRESULT CALLBACK 
cKARMA_WND::s_procReactionWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_hReactionWnd.Active();
			s_hReactionWnd.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case	WM_LBUTTONDOWN	:

			SelectReaction(hwnd);
			if(CSetDialogKarma::s_bIsEnableDialog)
			{
				BOOL temp;
				temp = CSetDialogKarma::ChangeKarmaTab(1);
				
				if(!temp)
					CSetDialogKarma::InitReaction(CSetDialogKarma::s_hReaction);
			}
			break;

		case	WM_RBUTTONDOWN	:
			PopupReaction(hwnd);
			break;

		case	WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {
				case	IDC_COPY_KARMA		:
					CopyReaction(hwnd);
					break;

				case	IDC_CUT_KARMA		:
					CutReaction(hwnd);
					break;

				case	IDC_PASTE_KARMA		:
					PasteReaction(hwnd);
					break;
			}
			break;

//		case WM_SETCURSOR	:
//			SetCursor(cMAIN::arrowCursor);
//			break;

		case WM_VSCROLL			:
			s_sbReaction.VScroll(wParam);
			DrawReactionWnd(hwnd);
			break;

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}	//	cKARMA_WND::s_procReactionWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)