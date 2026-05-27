#include "CSetDialog.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "cMAP.h"

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃대화 컨테이너 설정

CConversationContainer	CSetDialog::s_ccEdit;
CConversationContainer	CSetDialog::s_storeCC;
cDIBWND					CSetDialog::s_dibSpeechList;
cDIBWND					CSetDialog::s_dibTrigger;
BOOL					CSetDialog::s_bIsEventArea;
int						CSetDialog::s_iSpeechListWndWidth=	0;
int						CSetDialog::s_iTriggerWndWidth	=	0;
int						CSetDialog::s_iSelectSpeech		=	0xffff;
int						CSetDialog::s_iSelectSpeech2	=	0xffff;
int						CSetDialog::s_iSelectTrigger	=	0xffff;
char					CSetDialog::s_strTalkerName[64];
HWND					CSetDialog::s_hWnd;
cSCROLL_BAR				CSetDialog::s_sbSpeechList;
cSCROLL_BAR				CSetDialog::s_sbTriggerList;


CConversationContainer*
CSetDialog::Init(HWND hwnd,char *_lpstrTalkerName,CConversationContainer *_lpccEdit,BOOL _bIsArea)
{
	s_bIsEventArea	=	_bIsArea;

	strcpy(s_strTalkerName,_lpstrTalkerName);

	if (s_ccEdit.m_wSerial	==	0xffff)
	{
		s_ccEdit.init(dMAX_CONVERSATION_CONTENTS_COUNT);
		s_ccEdit.m_wSerial	=	0;
	}

	s_ccEdit.m_wConversationCount=	0;
	s_iSelectSpeech				=	0;
	s_iSelectTrigger			=	0;
	s_iSelectSpeech2			=	0xffff;

	if (_lpccEdit)
		s_ccEdit.copy(_lpccEdit,TRUE);

	if (DialogBox( s_hInst, "SET_DIALOG_CONTAINER",hwnd,(DLGPROC)ProcSetDialog)	==	IDCANCEL)
		return	NULL;

	if (_lpccEdit)	_lpccEdit->copy(&s_ccEdit);

	g_lpActmap->LostVirgin();

	return	&s_ccEdit;
}

//
//	대화박스 데이터를 초기화 시킨다.
void
CSetDialog::InitSetDialogBoxData(HWND _hDlg)
{
	int	i;

	s_hWnd	=	_hDlg;

	if (s_bIsEventArea	==	FALSE)
		SetWindowText(_hDlg,_ms(dMSG_TALK_BY_FORM,s_strTalkerName));
	else
	{
		SetWindowText(_hDlg,_ms("영역 [%s] 이벤트 설정",s_strTalkerName));
		SetWindowText(GetDlgItem(_hDlg,IDC_RANDOM),"랜덤하게 이벤트 선택");
		SetWindowText(GetDlgItem(_hDlg,IDC_STATIC_STATUS_LIST),"이벤트 리스트");
		SetWindowText(GetDlgItem(_hDlg,IDC_FRAME_CONDITION),"선택한 이벤트로 시작하게 되는 조건");
	}

	HWND	hTrigger	=	GetDlgItem(_hDlg,IDC_BEGIN_CONDITION);
	s_dibTrigger.lpszRegister	=	"TRIGGER_LIST_WINDOW";
	s_dibTrigger.Init(hTrigger,(WNDPROC)ProcTriggerWnd);

	HWND	hSpeechList	=	GetDlgItem(_hDlg,IDC_SPEECH_LIST);
	s_dibSpeechList.lpszRegister	=	"SPEECH_LIST_WINDOW";
	s_dibSpeechList.Init(hSpeechList,(WNDPROC)ProcSpeechListWnd);

	s_sbSpeechList.Init(hSpeechList);
	s_sbSpeechList.InitHeight();

	s_sbTriggerList.Init(hTrigger);
	s_sbTriggerList.InitHeight();

	HWND	hSizeList	=	GetDlgItem(_hDlg,IDC_SIZE_LIST);

	for (i=0;;i++)
	{
		if	(stricmp(g_strDialogWindowSizeList[i],"end")==0)
			break;

		cCOMBOBOX::Add(hSizeList,g_strDialogWindowSizeList[i],i);
	}

	cCOMBOBOX::SelectByData(hSizeList,s_ccEdit.m_wDialogBoxSize);

	CheckDlgButton(_hDlg,IDC_RANDOM,s_ccEdit.m_wIsRandomChoiceDialg);

	DrawTrigger(_hDlg);
	DrawSpeechList(_hDlg);
}

//
//트리거 그려주기
void
CSetDialog::DrawTrigger(HWND _hDlg)
{
	int	x,y,i;

	if	(!s_dibTrigger.Active())
		return;

	if	(s_ccEdit.m_wConversationCount	<=	0)
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_TRIGGER),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_TRIGGER	),FALSE);

		EnableWindow(GetDlgItem(_hDlg,IDC_AND_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_OR_TRIGGER	),FALSE);
		SetDlgItemInt(_hDlg,IDC_ACTIVATE_TRIGGER_COUNT,0,FALSE);

		cDRAW::Fill(0);
		cTEXT::FitPut(4,2,cDRAW::WIDTH-14,100,WHITE,dMSG_NOT_EXIST_TRIGGER_FOR_SELECT_FIRST_DIALOG);
		s_dibTrigger.Draw();
		return;
	}

	EnableWindow(GetDlgItem(_hDlg,IDC_ADD_TRIGGER),TRUE);

	CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	if (lpSelectSpeech->m_wTriggerCount	<=	0)	//	트리거 카운트가 0이다.
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_TRIGGER),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_AND_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_OR_TRIGGER	),FALSE);
		SetDlgItemInt(_hDlg,IDC_ACTIVATE_TRIGGER_COUNT,0,FALSE);

		cDRAW::Fill(0);
		cTEXT::FitPut(4,2,cDRAW::WIDTH-14,100,WHITE,dMSG_NOT_EXIST_TRIGGER_FOR_SELECT_FIRST_DIALOG);
		s_dibTrigger.Draw();
		return;
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_TRIGGER),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_TRIGGER	),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_AND_TRIGGER	),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_OR_TRIGGER	),TRUE);
	}

	SetDlgItemInt(_hDlg,IDC_ACTIVATE_TRIGGER_COUNT,lpSelectSpeech->m_bf7ActivateTriggerCount,FALSE);

	CheckDlgButton(_hDlg,IDC_AND_TRIGGER,FALSE);
	CheckDlgButton(_hDlg,IDC_OR_TRIGGER,FALSE);

	if	(lpSelectSpeech->m_isWantAndCondition)
		CheckDlgButton(_hDlg,IDC_AND_TRIGGER,TRUE);
	else
		CheckDlgButton(_hDlg,IDC_OR_TRIGGER,TRUE);

	if	(lpSelectSpeech->m_isWantNotComplete)
		CheckDlgButton(_hDlg,IDC_ACTIVE_CONDITION,TRUE);
	else
		CheckDlgButton(_hDlg,IDC_ACTIVE_CONDITION,FALSE);

	x						=	5;
	y						=	5;
	s_iTriggerWndWidth		=	cDRAW::WIDTH-14-20;

	for (i=0;i<lpSelectSpeech->m_wTriggerCount;i++)
	{
		char	*lpText		=	cKARMA_WND::KarmaText(&lpSelectSpeech->m_pTrigger[i],TRUE);
		int		iLineCount	=	cTEXT::GetLines(s_iTriggerWndWidth,lpText);

		y					+=	iLineCount*14;
		y					+=	4+5;
	}

	int	iHeightPos			=	s_sbTriggerList.GetHeightPos();

	s_sbTriggerList.SetHeight(y,cDRAW::HEIGHT);
	s_sbTriggerList.SetHeightPos(iHeightPos);

	x						=	5;
	y						=	5-s_sbTriggerList.GetHeightPos();

	cDRAW::Fill(_SKYBLUE);

	for (i=0;i<lpSelectSpeech->m_wTriggerCount;i++)
	{
		char	*lpText		=	cKARMA_WND::KarmaText(&lpSelectSpeech->m_pTrigger[i],TRUE);
		int		iLineCount	=	cTEXT::GetLines(s_iTriggerWndWidth,lpText);

		cDRAW::Fill		(0,x,y,x+s_iTriggerWndWidth+6,y+iLineCount*14+4);

		if(i==s_iSelectTrigger)
		{
			cDRAW::Fill		(_LTBLUE,x-1,y-1,x+s_iTriggerWndWidth+5,y+iLineCount*14+3);
			cTEXT::FitPut	(x+1,y+2,s_iTriggerWndWidth,iLineCount*8*2,WHITE,lpText);
		}else
		{
			cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+s_iTriggerWndWidth+4,y+iLineCount*14+2);
			cTEXT::FitPut	(x,y+1,s_iTriggerWndWidth,iLineCount*8*2,WHITE,lpText);
		}

		y					+=	iLineCount*14;
		y					+=	4+5;

		if (y	>=	cDRAW::HEIGHT)	break;
	}

	s_dibTrigger.Draw();
}	//	CSetDialog::DrawTrigger(HWND _hDlg)


//
//	최초 분기 추가
void
CSetDialog::AddTrigger(HWND _hDlg)
{
	if (s_ccEdit.m_wConversationCount	<=	0)	return;

	CConversation	tempConversation;
	CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	tempConversation.init(dMAX_DIALOG_TRIGGER_COUNT,dMAX_CONVERSATION_CONTENTS_COUNT);
	tempConversation.copy(lpSelectSpeech,TRUE);

	g_EditKarma.m_wTriggerCount	=	0;
	
	int	iIndex	=	cKARMA_WND::AddTrigger(_hDlg);

	if (iIndex	==	0xffff)	return;

	g_lpActmap->LostVirgin();

	tempConversation.addTrigger(&g_EditKarma.m_pTrigger[0]);
	lpSelectSpeech->copy(&tempConversation);

	DrawTrigger(_hDlg);
}	//	CSetDialog::AddFirstBranch(HWND _hDlg)

//
//	최초 분기 추가
void
CSetDialog::EditTrigger(HWND _hDlg)
{
	if (s_ccEdit.m_wConversationCount	<=	0)	return;

	CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	g_EditKarma.m_wTriggerCount	=	0;
	
	int	iIndex	=	cKARMA_WND::AddTrigger(_hDlg,&lpSelectSpeech->m_pTrigger[s_iSelectTrigger]);

	if (iIndex	==	0xffff)	return;

	lpSelectSpeech->m_pTrigger[s_iSelectTrigger].copy(&g_EditKarma.m_pTrigger[0]);

	DrawTrigger(_hDlg);
}	//	CSetDialog::AddFirstBranch(HWND _hDlg)

//
//	트리거 발동조건 확인
void
CSetDialog::CheckTriggerCompleteMethod(HWND _hDlg)
{
	if (s_ccEdit.m_wConversationCount	<=	0)	return;

	CConversation	*lpSelectSpeech		=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	lpSelectSpeech->m_isWantAndCondition=	IsDlgButtonChecked(_hDlg,IDC_AND_TRIGGER);
	lpSelectSpeech->m_isWantNotComplete	=	IsDlgButtonChecked(_hDlg,IDC_ACTIVE_CONDITION);

	DrawTrigger(_hDlg);
}	//	CSetDialog::AddFirstBranch(HWND _hDlg)

//
//	최초 분기 제거
void
CSetDialog::RemoveTrigger(HWND _hDlg)
{
	if (s_ccEdit.m_wConversationCount	<=	0)	return;

	CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	lpSelectSpeech->removeTrigger(s_iSelectTrigger);

	DrawTrigger(_hDlg);
}	//	CSetDialog::AddFirstBranch(HWND _hDlg)

//
//대화 리스트 그리기
void
CSetDialog::DrawSpeechList(HWND _hDlg)
{
	int	x,y,i;

	if (s_ccEdit.m_wConversationCount<=	0)	//	리액션 카운트가 0이다.
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_SPEECH)	,FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_SPEECH)	,FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_SPEECH)	,TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT_SPEECH)	,TRUE);
	}

	if (!s_dibSpeechList.Active()) return;

	x						=	5;
	y						=	5;
	s_iSpeechListWndWidth	=	cDRAW::WIDTH-18-20;

	for (i=0;i<s_ccEdit.m_wConversationCount;i++)
	{
		char	*lpText;

		if (s_ccEdit.m_pConversation[i].m_wIsNoSpeech)
			lpText		=	_ms("%2d)《%s》%s",i+1,dMSG_NOT_DIALOG_AND_REACT_FIRST_SELECT_SPEECH,s_ccEdit.m_pConversation[i].m_pstrSpeech);
		else
			lpText		=	_ms("%2d) %s",i+1,s_ccEdit.m_pConversation[i].m_pstrSpeech);

		int		iLineCount	=	cTEXT::GetLines(s_iSpeechListWndWidth,lpText);

		y					+=	iLineCount*14;
		y					+=	4+5;
	}

	int	iHeightPos	=	s_sbSpeechList.GetHeightPos();

	s_sbSpeechList.SetHeight(y,cDRAW::HEIGHT);
	s_sbSpeechList.SetHeightPos(iHeightPos);

	x		=	5;
	y		=	5-s_sbSpeechList.GetHeightPos();

	cDRAW::Fill(_SKYBLUE);

	memset(g_strCurrentSpeechList,0,sizeof(g_strCurrentSpeechList));

	int	iSSBegin	=	s_iSelectSpeech,iSSEnd	=	s_iSelectSpeech2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectSpeech;
	else
	{
		if	(iSSBegin	>	iSSEnd)
			swap(iSSBegin,iSSEnd);
	}

	for (i=0;i<s_ccEdit.m_wConversationCount;i++)
	{
		char	*lpText;

		if (s_ccEdit.m_pConversation[i].m_wIsNoSpeech)
				lpText		=	_ms("%2d)《%s》%s",i+1,dMSG_NOT_DIALOG_AND_REACT_FIRST_SELECT_SPEECH,s_ccEdit.m_pConversation[i].m_pstrSpeech);
		else	lpText		=	_ms("%2d) %s",i+1,s_ccEdit.m_pConversation[i].m_pstrSpeech);

		ustrncpy(g_strCurrentSpeechList[i],lpText,62);

		if (y	>=	cDRAW::HEIGHT)	continue;

		int		iLineCount	=	cTEXT::GetLines(s_iSpeechListWndWidth,lpText);

		cDRAW::Fill		(0,x,y,x+s_iSpeechListWndWidth+6,y+iLineCount*14+4);

		if	(i>=iSSBegin	&&	i	<=	iSSEnd)
		{
			if	(i	==	s_iSelectSpeech)
				cDRAW::Fill		(_LTBLUE,x-1,y-1,x+s_iSpeechListWndWidth+5,y+iLineCount*14+3);
			else
				cDRAW::Fill		(_BLUE,x-1,y-1,x+s_iSpeechListWndWidth+5,y+iLineCount*14+3);

			if	(s_ccEdit.m_pConversation[i].m_wIsNoSpeech)
				cTEXT::FitPut	(x+1,y+2,s_iSpeechListWndWidth,iLineCount*8*2,LTYELLOW,lpText);
			else
				cTEXT::FitPut	(x+1,y+2,s_iSpeechListWndWidth,iLineCount*8*2,WHITE,lpText);
		}
		else
		{
			cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+s_iSpeechListWndWidth+4,y+iLineCount*14+2);
			if (s_ccEdit.m_pConversation[i].m_wIsNoSpeech)
					cTEXT::FitPut	(x,y+1,s_iSpeechListWndWidth,iLineCount*8*2,YELLOW,lpText);
			else	cTEXT::FitPut	(x,y+1,s_iSpeechListWndWidth,iLineCount*8*2,WHITE,lpText);
		}

		y					+=	iLineCount*14;
		y					+=	4+5;
	}

	s_dibSpeechList.Draw();
}	//	CSetDialog::DrawSpeechList(HWND _hDlg)

void
CSetDialog::UpSelectSpeech(HWND _hDlg)
{
	if	(s_ccEdit.m_wConversationCount	<=	0 || s_iSelectSpeech <= 0 || s_iSelectSpeech >= s_ccEdit.m_wConversationCount)
		return;

	int	iSSBegin	=	s_iSelectSpeech,iSSEnd	=	s_iSelectSpeech2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectSpeech;
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

		CConversation	*lpkcSpeech,*lpkcUpSpeech,temp;

		lpkcSpeech	=	&s_ccEdit.m_pConversation[iSpeech];
		lpkcUpSpeech=	&s_ccEdit.m_pConversation[iSpeech-1];

		temp.copy(lpkcSpeech);

		lpkcSpeech->copy(lpkcUpSpeech);
		lpkcUpSpeech->copy(&temp);
		lpkcSpeech->m_wSerial--;
		lpkcUpSpeech->m_wSerial++;
	}

	s_iSelectSpeech--;

	if	(s_iSelectSpeech2	!=	0xffff)
		s_iSelectSpeech2--;

	g_lpActmap->LostVirgin();

	DrawSpeechList(_hDlg);
	DrawTrigger(_hDlg);
}

void
CSetDialog::DownSelectSpeech(HWND _hDlg)
{
	if	(s_iSelectSpeech < 0 || s_ccEdit.m_wConversationCount	<=	0 || s_iSelectSpeech >= s_ccEdit.m_wConversationCount-1)
		return;

	int	iSSBegin	=	s_iSelectSpeech,iSSEnd	=	s_iSelectSpeech2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectSpeech;
	else
	{
		if	(iSSBegin	>	iSSEnd)
			swap(iSSBegin,iSSEnd);
	}

	int	iCount	=	iSSEnd-iSSBegin+1;

	if	(iSSEnd	>=	s_ccEdit.m_wConversationCount-1)
		return;

	for	(int i=iCount;i;)
	{
		--i;

		int	iSpeech	=	i+iSSEnd-1;

		CConversation	*lpkcSpeech,*lpkcDownSpeech,temp;

		lpkcSpeech	=	&s_ccEdit.m_pConversation[iSpeech];
		lpkcDownSpeech=	&s_ccEdit.m_pConversation[iSpeech+1];

		temp.copy(lpkcSpeech);

		lpkcSpeech->copy(lpkcDownSpeech);
		lpkcDownSpeech->copy(&temp);
		lpkcSpeech->m_wSerial++;
		lpkcDownSpeech->m_wSerial--;
	}

	s_iSelectSpeech++;

	if	(s_iSelectSpeech2	!=	0xffff)
		s_iSelectSpeech2++;

	g_lpActmap->LostVirgin();
	DrawSpeechList(_hDlg);
	DrawTrigger(_hDlg);
}

//
//	최초 대사 추가
void
CSetDialog::AddSpeech(HWND _hDlg)
{
	CConversation	*lpkcNewSpeech	=	CSetSpeech::AddSpeech(_hDlg,s_strTalkerName,NULL,s_bIsEventArea);

	if (!lpkcNewSpeech)
		return;

	s_ccEdit.addSpeech(lpkcNewSpeech);

	s_iSelectSpeech	=	s_ccEdit.m_wConversationCount-1;

	DrawSpeechList(_hDlg);
	DrawTrigger(_hDlg);
}

//
//	최초 대사 추가
void
CSetDialog::EditSpeech(HWND _hDlg)
{
	if (s_ccEdit.m_wConversationCount	<=	0)
		return;

	CConversation	*lpkcEditSpeech	=	CSetSpeech::AddSpeech(_hDlg,s_strTalkerName,&s_ccEdit.m_pConversation[s_iSelectSpeech],s_bIsEventArea);

	if (!lpkcEditSpeech)
		return;

	s_ccEdit.m_pConversation[s_iSelectSpeech].copy(lpkcEditSpeech);

	DrawSpeechList(_hDlg);
	DrawTrigger(_hDlg);
}

//
//	최초 대사 제거
void
CSetDialog::RemoveSpeech(HWND _hDlg)
{
	if	(s_ccEdit.m_wConversationCount	<=	0)
		return;

	s_ccEdit.removeSpeech(s_iSelectSpeech);

	if	(s_iSelectSpeech	>=	s_ccEdit.m_wConversationCount)
		s_iSelectSpeech	=	s_ccEdit.m_wConversationCount-1;

	DrawSpeechList(_hDlg);
	DrawTrigger(_hDlg);
}

void
CSetDialog::CopySpeech(HWND _hDlg)
{
	if	(s_iSelectSpeech	<	0	||	s_iSelectSpeech	>=	s_ccEdit.m_wConversationCount)
		return;

	int	iSSBegin	=	s_iSelectSpeech,iSSEnd	=	s_iSelectSpeech2;

	if	(iSSEnd	==	0xffff)
		iSSEnd	=	s_iSelectSpeech;
	else
	{
		if	(iSSBegin	>	iSSEnd)
			swap(iSSBegin,iSSEnd);
	}

	int	iCount	=	iSSEnd-iSSBegin+1;

	s_storeCC.init(iCount);

	for(int i=0;i<iCount;i++)
		s_storeCC.m_pConversation[i].copy(&s_ccEdit.m_pConversation[iSSBegin+i]);
}

void
CSetDialog::PasteSpeech(HWND _hDlg)
{
	if	(s_storeCC.m_wConversationCount	<=	0)
		return;

	for	(int i=0;i<s_storeCC.m_wConversationCount;i++)
		s_ccEdit.addSpeech(&s_storeCC.m_pConversation[i]);

	s_iSelectSpeech	=	s_ccEdit.m_wConversationCount-1;
	s_iSelectSpeech2=	0xffff;

	DrawSpeechList(_hDlg);
	DrawTrigger(_hDlg);
}

//
//	선택한 리액션을 얻는다.
int
CSetDialog::GetSelectSpeech(HWND hwnd)
{
	int		x,y,i;
	cPOINT	Pos;

	Pos.MousePos(hwnd);

	x		=	5;
	y		=	5-s_sbSpeechList.GetHeightPos();

	for (i	=	0;i<s_ccEdit.m_wConversationCount;i++)
	{
		char	*lpText;

		if (s_ccEdit.m_pConversation[i].m_wIsNoSpeech)
				lpText		=	_ms("%2d)《%s》%s",i+1,dMSG_NOT_DIALOG_AND_REACT_FIRST_SELECT_SPEECH,s_ccEdit.m_pConversation[i].m_pstrSpeech);
		else	lpText		=	_ms("%2d) %s",i+1,s_ccEdit.m_pConversation[i].m_pstrSpeech);

		int		iLineCount	=	cTEXT::GetLines(s_iSpeechListWndWidth,lpText);

		if (y	>=	y	+	iLineCount*14+10)	break;

		cRECT	rect;

		rect.Set(0,y,s_iSpeechListWndWidth,y+iLineCount*14+4+5);

		if (rect.isIN(Pos.x,Pos.y))	return i;

		y					+=	iLineCount*14;
		y					+=	4+5;

		if (y	>=	s_dibSpeechList.HEIGHT)	break;
	}

	return	0xffff;
}

//
//	액션을 선택한다.
int
CSetDialog::SelectSpeech(HWND hwnd)
{
	int	 iSelectSpeech	=	GetSelectSpeech(hwnd);

	if	(iSelectSpeech	==	0xffff)
	{
		DrawSpeechList(s_hWnd);
		DrawTrigger(s_hWnd);

		return	0xffff;
	}

	cINPDEV::Update();

	if	(KEY[dKEY_SHIFT])
		s_iSelectSpeech2	=	s_iSelectSpeech;
	else
		s_iSelectSpeech2	=	0xffff;

	if	(s_iSelectSpeech	!=	iSelectSpeech)
	{
		s_iSelectSpeech		=	iSelectSpeech;

		DrawSpeechList(s_hWnd);
		DrawTrigger(s_hWnd);

		return	iSelectSpeech;
	}

	return	0xffff;
}

//
//	선택한 리액션을 얻는다.
int
CSetDialog::GetSelectTrigger(HWND hwnd)
{
	int		x,y,i;
	cPOINT	Pos;

	if (s_ccEdit.m_wConversationCount	<=	0)	return	0xffff;

	CConversation	*lpConversation	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];

	Pos.MousePos(hwnd);

	x		=	5;
	y		=	5-s_sbTriggerList.GetHeightPos();

	for (i	=	0;i<lpConversation->m_wTriggerCount;i++)
	{
		char	*lpText		=	cKARMA_WND::KarmaText(&lpConversation->m_pTrigger[i],TRUE);
		int		iLineCount	=	cTEXT::GetLines(s_iTriggerWndWidth,lpText);

		if (y	>=	y	+	iLineCount*14+10)	break;

		cRECT	rect;

		rect.Set(0,y,s_iTriggerWndWidth,y+iLineCount*14+4+5);

		if (rect.isIN(Pos.x,Pos.y))	return i;

		y					+=	iLineCount*14;
		y					+=	4+5;

		if (y	>=	s_dibTrigger.HEIGHT)	break;
	}

	return	0xffff;
}

//
//	액션을 선택한다.
int
CSetDialog::SelectTrigger(HWND hwnd)
{
	int	 iSelectTrigger	=	GetSelectTrigger(hwnd);

	if (iSelectTrigger	==	0xffff)
	{
		DrawTrigger(s_hWnd);

		return	0xffff;
	}

	if(s_iSelectTrigger	!=	iSelectTrigger)
	{
		s_iSelectTrigger=	iSelectTrigger;

		DrawTrigger(s_hWnd);

		return	iSelectTrigger;
	}

	return	0xffff;
}

//
//	액션을 선택한다.
BOOL
CSetDialog::GetDialogData(HWND _hDlg)
{
	HWND	hSizeList	=	GetDlgItem(_hDlg,IDC_SIZE_LIST);

	s_ccEdit.m_wDialogBoxSize		=	cCOMBOBOX::GetData(hSizeList);
	s_ccEdit.m_wIsRandomChoiceDialg	=	IsDlgButtonChecked(_hDlg,IDC_RANDOM);

	return	TRUE;
}

//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
CSetDialog::ProcSetDialog(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{

	int zDelta = 0;

	switch(message)
	{	case WM_INITDIALOG	:
			InitSetDialogBoxData(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		case 0x020A:	// 마우스 휠			
			zDelta = ((short)HIWORD(wParam));

			if (zDelta <= 0)
			{
				s_sbSpeechList.VScroll(SB_LINEDOWN, 14);
			}
			if (zDelta > 0)
			{
				s_sbSpeechList.VScroll(SB_LINEUP, 14);
			}
			DrawSpeechList(hDlg);

			break;
		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case	IDC_ACTIVATE_TRIGGER_COUNT	:
					if	(HIWORD(wParam)	==	EN_CHANGE)
                    {
						if	(s_ccEdit.m_wConversationCount	<=	0)
							break;

						CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

						BOOL	bTemp;
						int		iCount	=	GetDlgItemInt(hDlg,IDC_ACTIVATE_TRIGGER_COUNT,&bTemp,FALSE);

						if	(bTemp)
						{
							iCount		=	min(lpSelectSpeech->m_wTriggerCount,iCount);
							lpSelectSpeech->m_bf7ActivateTriggerCount	=	iCount;
						}
						break;
					}
					break;

				case	IDOK					:
					if (GetDialogData(hDlg))
					{
						EndDialog(hDlg,IDOK);
						return TRUE;
					}
					return TRUE;

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					return TRUE;

				case	IDC_AND_TRIGGER			:
					CheckTriggerCompleteMethod(hDlg);
					break;

				case	IDC_OR_TRIGGER			:
					CheckTriggerCompleteMethod(hDlg);
					break;

				case	IDC_ADD_TRIGGER			:
					AddTrigger(hDlg);
					break;

				case	IDC_EDIT_TRIGGER		:
					EditTrigger(hDlg);
					break;

				case	IDC_REMOVE_TRIGGER		:
					RemoveTrigger(hDlg);
					break;

				case	IDC_ADD_SPEECH			:
					AddSpeech(hDlg);
					break;

				case	IDC_EDIT_SPEECH			:
					EditSpeech(hDlg);
					break;

				case	IDC_REMOVE_SPEECH		:
					RemoveSpeech(hDlg);
					break;

				case	IDC_COPY_SPEECH			:
					CopySpeech(hDlg);
					break;
				case	IDC_PASTE_SPEECH		:
					PasteSpeech(hDlg);
					break;

				case	IDC_UP					:
					UpSelectSpeech(hDlg);
					break;

				case	IDC_DOWN					:
					DownSelectSpeech(hDlg);
					break;
			}
			break;
	}

	return (0L);
}


//
//	컨디션 윈도우
LRESULT CALLBACK 
CSetDialog::ProcTriggerWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case	WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibTrigger.Active();
			s_dibTrigger.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case	WM_LBUTTONDOWN	:
			SelectTrigger(hwnd);
			break;

		case	WM_VSCROLL		:
		{
			if	(s_sbTriggerList.VScroll(wParam))
				DrawTrigger(s_hWnd);
			break;
		}

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//
//	리액션 윈도우
LRESULT CALLBACK 
CSetDialog::ProcSpeechListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case	WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibSpeechList.Active();
			s_dibSpeechList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case	WM_LBUTTONDOWN	:
			SelectSpeech(hwnd);
			break;

		case	WM_VSCROLL		:
		{
			if (s_sbSpeechList.VScroll(wParam))	DrawSpeechList(s_hWnd);
			break;
		}

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//┃대화 컨테이너 설정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━