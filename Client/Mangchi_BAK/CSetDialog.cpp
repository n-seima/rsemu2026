#include "CSetDialog.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "cMAP.h"



//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃대화 설정

CConversation	CSetSpeech::s_cEdit;	//	편집중인 대화 컨테이너
CConversation	CSetSpeech::s_storeConversation;
cDIBWND			CSetSpeech::s_dibContents;
cDIBWND			CSetSpeech::s_dibReaction;
cSCROLL_BAR		CSetSpeech::s_sbContents;
cSCROLL_BAR		CSetSpeech::s_sbReaction;

int				CSetSpeech::s_iContentsWndPos;
int				CSetSpeech::s_iReactionWndPos		=	0;
int				CSetSpeech::s_iSelectContent;
int				CSetSpeech::s_iSelectContent2;
char			CSetSpeech::s_strTalkerName[64];
BOOL			CSetSpeech::s_bIsEdit;
BOOL			CSetSpeech::s_bIsEventArea;
HWND			CSetSpeech::s_hWnd;
HFONT			CSetSpeech::s_hFont;

//
//	대사 추가
CConversation*
CSetSpeech::AddSpeech(HWND _hDlg,char *_lpstrTalker,CConversation* _lpcEdit,BOOL _bIsArea)
{
	if	(_lpcEdit)
		s_bIsEdit	=	TRUE;
	else
		s_bIsEdit	=	FALSE;

	s_bIsEventArea	=	_bIsArea;

	strcpy(s_strTalkerName,_lpstrTalker);

	s_iSelectContent	=	0;
	s_iSelectContent2	=	0xffff;

	if (s_cEdit.m_wSerial	==	0xffff)
	{
		s_cEdit.init(dMAX_DIALOG_TRIGGER_COUNT,dMAX_CONVERSATION_CONTENTS_COUNT);
		s_cEdit.m_wSerial	=	0;
	}

	s_cEdit.m_wContentsCount		=	0;
	s_cEdit.m_wTriggerCount			=	0;
	s_cEdit.m_wIsNoSpeech			=	FALSE;

	s_cEdit.m_isWantAndCondition	=	TRUE;
	s_cEdit.m_isWantNotComplete		=	FALSE;
	s_cEdit.m_spareValue			=	FALSE;
	s_cEdit.m_bf7ActivateTriggerCount	=	0;

	if (_lpcEdit)
		s_cEdit.copy(_lpcEdit,TRUE);
	else
	{
		if	(s_bIsEventArea	==	FALSE)
			s_cEdit.setSpeech("new speech");
		else
			s_cEdit.setSpeech("new event");
	}

	if	(DialogBox( s_hInst, "SET_DIALOG_SPEECH",_hDlg,(DLGPROC)ProcDlg)==IDCANCEL)
		return	NULL;

	return	&s_cEdit;
}

//
//	박스 초기화
void
CSetSpeech::InitBox(HWND _hDlg)
{
	s_hWnd			=	_hDlg;
	g_iSelectKarma	=	0;

	{
		HWND	hDialogText	=	GetDlgItem(_hDlg,IDC_DIALOG);

		s_hFont				=	CreateFont(13,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN, "굴림");

		SendMessage(hDialogText,WM_SETFONT,(WPARAM)s_hFont, 0 );
	}

//	컨텐츠 윈도우 초기화
	{
		HWND	hContents	=	GetDlgItem(_hDlg,IDC_CONTENTS_WND);

		s_dibContents.lpszRegister	=	"CONTENTS_WINDOW";
		s_dibContents.Init(hContents,(WNDPROC)ProcContentsWnd);

		s_sbContents.Init(hContents);
		s_sbContents.InitHeight();

		DrawContentsWnd(_hDlg);
	}

//	반응 윈도우 초기화
	{
		HWND	hReaction	=	GetDlgItem(_hDlg,IDC_REACTION_WND);

		s_dibReaction.lpszRegister	=	"REACTION_WINDOW";
		s_dibReaction.Init(hReaction,(WNDPROC)ProcReactionWnd);

		s_sbReaction.Init(hReaction);
		s_sbReaction.InitHeight();

		DrawReactionWnd(_hDlg);
	}

	if (s_bIsEventArea	==	TRUE)
	{
		SetDlgItemText(_hDlg,IDC_STATIC_DIALOG,"지문");
		SetDlgItemText(_hDlg,IDC_NO_SPEECH,"지문 없이 첫번째 선택문 이 반응");
	}
		
	if	(s_cEdit.m_pstrSpeech)
	{
		SetDlgItemText(_hDlg,IDC_DIALOG,s_cEdit.m_pstrSpeech);
		SetFocus(GetDlgItem(_hDlg,IDC_DIALOG));
		SendMessage(GetDlgItem(_hDlg,IDC_DIALOG),EM_SETSEL,0,-1);
	}

	if	(s_cEdit.m_wIsNoSpeech)
		CheckDlgButton(_hDlg,IDC_NO_SPEECH,TRUE);

	if	(s_bIsEdit)
	{
		if (s_bIsEventArea	==	FALSE)
			SetWindowText(_hDlg,_ms(dMSG_EDIT_SPEECH_SET_FORM,s_strTalkerName));
		else
			SetWindowText(_hDlg,_ms("영역 [%s] 이벤트 수정",s_strTalkerName));
	}
	else
	{
		if (s_bIsEventArea	==	FALSE)
			SetWindowText(_hDlg,_ms(dMSG_ADD_SPEECH_SET_FORM,s_strTalkerName));
		else
			SetWindowText(_hDlg,_ms("영역 [%s] 이벤트 추가",s_strTalkerName));
	}
}

void
CSetSpeech::MoveUpContents(HWND _hDlg)
{
	if	(s_cEdit.m_wContentsCount	<=	0 || s_iSelectContent <= 0 || s_iSelectContent >= s_cEdit.m_wContentsCount)
		return;

	int	iSSBegin	=	s_iSelectContent,iSSEnd	=	s_iSelectContent2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectContent;
	else
	{
		if	(iSSBegin	>	iSSEnd)
			swap(iSSBegin,iSSEnd);
	}

	int	iCount	=	iSSEnd-iSSBegin+1;

	if	(iSSBegin	<=	0)
		return;

	for	(int i=0;i<iCount;i++)
	{
		int	iSpeech	=	i+iSSBegin;

		CKarmaContainer	*lpkcSpeech,*lpkcUpSpeech,temp;

		lpkcSpeech	=	&s_cEdit.m_pContents[iSpeech];
		lpkcUpSpeech=	&s_cEdit.m_pContents[iSpeech-1];

		temp.copy(lpkcSpeech);

		lpkcSpeech->copy(lpkcUpSpeech);
		lpkcUpSpeech->copy(&temp);
		lpkcSpeech->m_wSerial--;
		lpkcUpSpeech->m_wSerial++;
	}

	s_iSelectContent--;

	if	(s_iSelectContent2	!=	0xffff)
		s_iSelectContent2--;

	g_lpActmap->LostVirgin();

	DrawContentsWnd(_hDlg);
	DrawReactionWnd(_hDlg);
}

void
CSetSpeech::MoveDownContents(HWND _hDlg)
{
	if	(s_iSelectContent < 0 || s_cEdit.m_wContentsCount	<=	0 || s_iSelectContent >= s_cEdit.m_wContentsCount-1)
		return;

	int	iSSBegin	=	s_iSelectContent,iSSEnd	=	s_iSelectContent2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectContent;
	else
	{
		if	(iSSBegin	>	iSSEnd)
			swap(iSSBegin,iSSEnd);
	}

	int	iCount	=	iSSEnd-iSSBegin+1;

	if	(iSSEnd	>=	s_cEdit.m_wContentsCount-1)
		return;

	for	(int i=iCount;i;)
	{
		--i;

		int	iSpeech	=	i+iSSBegin;

		CKarmaContainer	*lpkcSpeech,*lpkcDownSpeech,temp;

		lpkcSpeech	=	&s_cEdit.m_pContents[iSpeech];
		lpkcDownSpeech=	&s_cEdit.m_pContents[iSpeech+1];

		temp.copy(lpkcSpeech);

		lpkcSpeech->copy(lpkcDownSpeech);
		lpkcDownSpeech->copy(&temp);
		lpkcSpeech->m_wSerial++;
		lpkcDownSpeech->m_wSerial--;
	}

	s_iSelectContent++;

	if	(s_iSelectContent2	!=	0xffff)
		s_iSelectContent2++;

	g_lpActmap->LostVirgin();

	DrawContentsWnd(_hDlg);
	DrawReactionWnd(_hDlg);
}

//	복사
void
CSetSpeech::CopyContents(HWND _hDlg)
{
	if	(s_iSelectContent	<	0	||	s_iSelectContent	>=	s_cEdit.m_wContentsCount)
		return;

	int	iSSBegin	=	s_iSelectContent,iSSEnd	=	s_iSelectContent2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectContent;
	else
	{
		if	(iSSBegin	>	iSSEnd)
			swap(iSSBegin,iSSEnd);
	}

	int	iCount	=	iSSEnd-iSSBegin+1;

	s_storeConversation.init(0,iCount);

	for(int i=0;i<iCount;i++)
		s_storeConversation.m_pContents[i].copy(&s_cEdit.m_pContents[iSSBegin+i]);

	DrawContentsWnd(_hDlg);
}

//	붙여넣기
void
CSetSpeech::PasteContents(HWND _hDlg)
{
	if	(s_storeConversation.m_wContentsCount	<=	0)
		return;

	for	(int i=0;i<s_storeConversation.m_wContentsCount;i++)
		s_cEdit.addContent(&s_storeConversation.m_pContents[i]);

	s_iSelectContent	=	s_cEdit.m_wContentsCount-1;
	s_iSelectContent2	=	0xffff;

	DrawContentsWnd(_hDlg);
	DrawReactionWnd(_hDlg);
}

//
//최초 분기 추가
void
CSetSpeech::AddContents(HWND _hDlg)
{
	CKarmaContainer	kc;

	kc.allocBuffer(dKARMA_COUNT);

	if	(!cKARMA_WND::Open(_hDlg,s_hInst,&kc))
	{
		DrawContentsWnd(_hDlg);
		DrawReactionWnd(_hDlg);

		return;
	}

	s_cEdit.addContent(&kc);

	s_iSelectContent	=	s_cEdit.m_wContentsCount-1;
	s_iSelectContent2	=	0xffff;

	DrawContentsWnd(_hDlg);
	DrawReactionWnd(_hDlg);
}

//
//선택된 최초 분기 편집
void
CSetSpeech::EditContents(HWND _hDlg)
{
	if	(s_cEdit.m_wContentsCount	<=	0)
		return;

	CKarmaContainer	kc;

	kc.copy(&s_cEdit.m_pContents[s_iSelectContent],TRUE);

	if	(!cKARMA_WND::Open(_hDlg,s_hInst,&kc))
	{
		DrawContentsWnd(_hDlg);
		DrawReactionWnd(_hDlg);
		return;
	}

	s_cEdit.m_pContents[s_iSelectContent].copy(&kc);

	DrawContentsWnd(_hDlg);
	DrawReactionWnd(_hDlg);
}

//
//선택된 최초 분기 제거
void
CSetSpeech::RemoveContents(HWND _hDlg)
{
	if	(s_cEdit.m_wContentsCount	<=	0)
		return;

	int	iSSBegin	=	s_iSelectContent,iSSEnd	=	s_iSelectContent2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectContent;
	else
	{
		if	(iSSBegin	>	iSSEnd)
			swap(iSSBegin,iSSEnd);
	}

	for	(int i=iSSBegin;i<=iSSEnd;i++)
		if	(!s_cEdit.removeContent(iSSBegin))
			return;

	if	(s_iSelectContent	>=	s_cEdit.m_wContentsCount)	
		s_iSelectContent	=	s_cEdit.m_wContentsCount-1;

	s_iSelectContent2		=	0xffff;

	DrawContentsWnd(_hDlg);
	DrawReactionWnd(_hDlg);
}

//
//	컨텐츠 윈도우 그려주기
void
CSetSpeech::DrawContentsWnd(HWND _hDlg)
{
	int	x,y,i,iWidth;

	if	(s_cEdit.m_wContentsCount	<=	0)	//	컨텐츠 카운트가 0이다.
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_SPEECH),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_SPEECH)	,FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_COPY)			,FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_SPEECH),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_SPEECH)	,TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_COPY)			,TRUE);
	}

	if	(s_storeConversation.m_wContentsCount	==	0)
		EnableWindow(GetDlgItem(_hDlg,IDC_PASTE_CONTENTS)			,FALSE);
	else
		EnableWindow(GetDlgItem(s_hWnd,IDC_PASTE_CONTENTS)			,TRUE);

	if	(!s_dibContents.Active())
		return;

	cDRAW::Fill(_SKYBLUE);

	x		=	5;
	y		=	5;
	iWidth	=	s_dibContents.WIDTH-32;

	if (s_cEdit.m_wContentsCount	<=	0)
	{
		cTEXT::PushShadow(FALSE);
		cTEXT::Put(x,y,0,"등록된 선택문이 없습니다.");
		cTEXT::PopShadow();

		s_dibContents.Draw();

		return;
	}

	s_iContentsWndPos		=	s_sbContents.GetHeightPos();

	for (i = 0;i<s_cEdit.m_wContentsCount;i++)
	{
		char	*lpText		=	_ms("%2d) %s",i+1,s_cEdit.m_pContents[i].m_pstrTitle);
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		y					+=	iLineCount*14;
		y					+=	4+4;
	}

	s_sbContents.SetHeight(y,cDRAW::HEIGHT);
	s_sbContents.SetHeightPos(s_iContentsWndPos);

	y		=	5-s_iContentsWndPos;

	int	iSSBegin	=	s_iSelectContent,iSSEnd	=	s_iSelectContent2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectContent;
	else
	{
		if	(iSSBegin	>	iSSEnd)
			swap(iSSBegin,iSSEnd);
	}

	for (i=0;i<s_cEdit.m_wContentsCount;i++)
	{
		char	*lpText		=	_ms("%2d) %s",i+1,s_cEdit.m_pContents[i].m_pstrTitle);
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		cDRAW::Fill		(0,x,y,x+iWidth+6,y+iLineCount*14+4);

		if	(i	>=	iSSBegin	&&	i	<=	iSSEnd)
		{
			if	(i	==	s_iSelectContent)
				cDRAW::Fill		(_LTBLUE,x-1,y-1,x+iWidth+5,y+iLineCount*14+3);
			else
				cDRAW::Fill		(_BLUE,x-1,y-1,x+iWidth+5,y+iLineCount*14+3);

			cTEXT::FitPut	(x+1,y+2,iWidth,iLineCount*8*2,WHITE,lpText);
		}
		else
		{
			cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);

			cTEXT::FitPut	(x,y+1,iWidth,iLineCount*8*2,WHITE,lpText);
		}

		y					+=	iLineCount*14;
		y					+=	4+4;

		if	(y	>=	cDRAW::HEIGHT)
			break;
	}

	s_dibContents.Draw();
}

//
//	현재 선택한 컨탠츠를 선택했을때의 액션 정보를 그려주기
void
CSetSpeech::DrawReactionWnd(HWND _hDlg)
{
	CKarmaContainer	*lpSelectSpeech	=	NULL;

	if	(s_iSelectContent	>=	s_cEdit.m_wContentsCount)
	{
		s_iSelectContent	=	s_cEdit.m_wContentsCount-1;
		s_iSelectContent2	=	0xffff;
	}

	if	(s_iSelectContent >= 0)
		lpSelectSpeech	=	&s_cEdit.m_pContents[s_iSelectContent];

	g_lpAKC	=	lpSelectSpeech;

	cKARMA_WND::UpdateKarmaDlg(_hDlg);
}

//
//	대화 데이터 얻기
BOOL
CSetSpeech::GetSpeechData(HWND _hDlg)
{
	char	strSpeech[2048];
	BOOL	bTemp;

	GetDlgItemText(_hDlg,IDC_DIALOG,strSpeech,sizeof(strSpeech));	//	카르마 이름

	s_cEdit.m_wIsNoSpeech				=	IsDlgButtonChecked(_hDlg,IDC_NO_SPEECH);
	s_cEdit.m_bf7ActivateTriggerCount	=	GetDlgItemInt(_hDlg,IDC_ACTIVATE_TRIGGER_COUNT,&bTemp,FALSE);

	s_cEdit.setSpeech(strSpeech);

	return	TRUE;
}

//
//	선택한 리액션을 얻는다.
int
CSetSpeech::GetSelectContent(HWND hwnd)
{
	int		x,y,i,iWidth;
	cPOINT	Pos;

	Pos.MousePos(hwnd);

	x		=	5;
	y		=	5-s_sbContents.GetHeightPos();
	iWidth	=	s_dibContents.WIDTH-14;

	for (i	=	0;i<s_cEdit.m_wContentsCount;i++)
	{
		char	*lpText		=	s_cEdit.m_pContents[i].m_pstrTitle;
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		cRECT	rect;

		rect.Set(0,y-2,iWidth,y+iLineCount*14+4);

		if	(rect.isIN(Pos.x,Pos.y))
			return i;

		y		+=	iLineCount*14;
		y		+=	4+4;

		if	(y	>=	s_dibContents.HEIGHT)
			break;
	}

	return	0xffff;
}

//
//	액션을 선택한다.
int
CSetSpeech::SelectContent(HWND hwnd)
{
	int	 iSelectContent	=	GetSelectContent(hwnd);

	if	(iSelectContent	==	0xffff)
	{
		DrawContentsWnd(s_hWnd);
		DrawReactionWnd(s_hWnd);

		return	0xffff;
	}

	cINPDEV::Update();

	if	(KEY[dKEY_SHIFT])
		s_iSelectContent2	=	s_iSelectContent;
	else
		s_iSelectContent2	=	0xffff;

	if	(s_iSelectContent	!=	iSelectContent)
	{
		s_iSelectContent	=	iSelectContent;

		DrawContentsWnd(s_hWnd);
		DrawReactionWnd(s_hWnd);

		return	iSelectContent;
	}

	return	0xffff;
}

//
//	다얄로그 프로시져
LRESULT CALLBACK
CSetSpeech::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	case WM_INITDIALOG	:
			InitBox(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;

		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case	IDC_KARMA_LIST		:	//	카르마 리스트를 클릭했다.
					cKARMA_WND::SelectKarma(hDlg);
					break;

				case	IDC_UP					:
					MoveUpContents(hDlg);
					break;
				case	IDC_DOWN				:
					MoveDownContents(hDlg);
					break;

				case	IDC_COPY				:
					CopyContents(hDlg);
					break;
				case	IDC_PASTE_CONTENTS		:
					PasteContents(hDlg);
					break;

				case	IDC_ADD_SPEECH			:
					AddContents(hDlg);
					break;

				case	IDC_EDIT_SPEECH			:
					EditContents(hDlg);
					break;

				case	IDC_REMOVE_SPEECH		:
					RemoveContents(hDlg);
					break;

				case	IDOK					:
					if (!GetSpeechData(hDlg))	break;
					EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;
			}
			break;
	}

	return (0L);
}

//
//	컨텐츠 리스트 윈도우 프로시져
LRESULT CALLBACK
CSetSpeech::ProcContentsWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibContents.Active();
			s_dibContents.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_VSCROLL			:
			s_sbContents.VScroll(wParam);
			DrawContentsWnd(hwnd);
			break;

		case	WM_LBUTTONDOWN	:
			SelectContent(hwnd);
			break;

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}	//	CSetSpeech::ProcContestsListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

//
//	반응 윈도우 프로시져
LRESULT CALLBACK
CSetSpeech::ProcReactionWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibReaction.Active();
			s_dibReaction.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}	//	CSetSpeech::ProcReactionWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

//┃대화 설정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━