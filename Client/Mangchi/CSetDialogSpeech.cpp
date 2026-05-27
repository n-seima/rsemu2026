#include "resource.h"
#include "cMAP.h"
#include "cSetDialogSpeech.h"
#include "CSetDialog.h"
#include "cMAPWND.H"
#include "cDRAW.h"
#include "cTEXT.h"
#include "cMAIN.H"
#include "CSetDialogKarma.h"

HWND					CSetDialogSpeech::s_hWnd					= NULL;	
CConversationContainer	CSetDialogSpeech::s_ccEdit;
CConversationContainer	CSetDialogSpeech::s_storeCC;
cDIBWND					CSetDialogSpeech::s_dibSpeechList;
cDIBWND					CSetDialogSpeech::s_dibTriggerList;
cDIBWND					CSetDialogSpeech::s_dibContents;
cSCROLL_BAR				CSetDialogSpeech::s_sbSpeechList;
cSCROLL_BAR				CSetDialogSpeech::s_sbTriggerList;
cSCROLL_BAR				CSetDialogSpeech::s_sbContents;
CConversation			CSetDialogSpeech::s_cEdit;
CConversation			CSetDialogSpeech::s_storeConversation;
CKarma					CSetDialogSpeech::s_aStoredKarma;
HFONT					CSetDialogSpeech::s_hFont;

BOOL					CSetDialogSpeech::s_bIsEventArea;
BOOL					CSetDialogSpeech::s_bIsEdit;
BOOL					CSetDialogSpeech::s_bIsSaveAddSpeech;
BOOL					CSetDialogSpeech::s_bIsSaveAddContents;

char					CSetDialogSpeech::s_strTalkerName[64];
int						CSetDialogSpeech::s_iSelectSpeech			= 0xffff;
int						CSetDialogSpeech::s_iSelectSpeech2			= 0xffff;
int						CSetDialogSpeech::s_iSelectTrigger			= 0xffff;
int						CSetDialogSpeech::s_iSpeechListWndWidth		= 0;
int						CSetDialogSpeech::s_iTriggerWndWidth		= 0;

int						CSetDialogSpeech::s_iSelectContent			= 0xffff;
int						CSetDialogSpeech::s_iSelectContent2			= 0xffff;
int						CSetDialogSpeech::s_iContentsWndPos;

//
// 대화 편집 대화상자 생성
CConversationContainer*	
CSetDialogSpeech::Init(HWND _hwnd,char *_lpstrTalkerName,CConversationContainer *_lpccEdit,BOOL _bIsArea)
{
	s_bIsEventArea	=	_bIsArea;
	strcpy(s_strTalkerName,_lpstrTalkerName);

	if (s_ccEdit.m_wSerial	==	0xffff)
	{
		s_ccEdit.init(dMAX_CONVERSATION_CONTENTS_COUNT);
		s_ccEdit.m_wSerial	=	0;
	}

	s_ccEdit.m_wConversationCount	=	0;
	s_iSelectSpeech					=	0;
	s_iSelectTrigger				=	0;
	s_iSelectSpeech2				=	0xffff;

	s_bIsSaveAddSpeech				=	FALSE;
	s_bIsSaveAddContents			=	FALSE;
	
	if (_lpccEdit)
		s_ccEdit.copy(_lpccEdit,TRUE);

	if (DialogBox( s_hInst, "SET_DIALOG_SPEECH_EDIT",_hwnd,(DLGPROC)ProcSetDialog)	==	IDCANCEL)
		return	NULL;

	return	&s_ccEdit;
}

// 대화편집 윈도우 생성 및 초기화
void
CSetDialogSpeech::InitSetDialogBoxData(HWND _hDlg)
{
	s_hWnd	= _hDlg;

	if (s_bIsEventArea	==	FALSE)
		SetWindowText(_hDlg,_ms(dMSG_TALK_BY_FORM,s_strTalkerName));

	// Speech
	{
		HWND	hSpeechList	=	GetDlgItem(s_hWnd,IDC_SPEECH_LIST);
		s_dibSpeechList.lpszRegister	=	"SPEECH_LIST_WINDOW";
		s_dibSpeechList.Init(hSpeechList,(WNDPROC)ProcSpeechListWnd);
		
		s_sbSpeechList.Init(hSpeechList);
		s_sbSpeechList.InitHeight();
	}
	
	// Trigger
	{
		HWND	hTrigger				=	GetDlgItem(s_hWnd,IDC_BEGIN_CONDITION);
		s_dibTriggerList.lpszRegister	=	"TRIGGER_LIST_WINDOW";
		s_dibTriggerList.Init(hTrigger,(WNDPROC)ProcTriggerWnd);
		
		s_sbTriggerList.Init(hTrigger);
		s_sbTriggerList.InitHeight();
	}

	// Contents
	{
		HWND	hContents	=	GetDlgItem(s_hWnd,IDC_CONTENTS_WND2);

		s_dibContents.lpszRegister	=	"CONTENTS_WINDOW";
		s_dibContents.Init(hContents,(WNDPROC)ProcContentsWnd);

		s_sbContents.Init(hContents);
		s_sbContents.InitHeight();
	}

	HWND	hSizeList	=	GetDlgItem(s_hWnd,IDC_SIZE_LIST);
	
	for (int i=0;;i++)
	{
		if	(stricmp(g_strDialogWindowSizeList[i],"end")==0)
			break;
		cCOMBOBOX::Add(hSizeList,g_strDialogWindowSizeList[i],i);
	}

	cCOMBOBOX::SelectByData(hSizeList,s_ccEdit.m_wDialogBoxSize);
	CheckDlgButton(s_hWnd,IDC_RANDOM,s_ccEdit.m_wIsRandomChoiceDialg);
	
	InitSpeech();

	DrawSpeechList();
	DrawTrigger();
	DrawContents();
	DrawSpeechKarma();
}

//
// 대화 컨텐츠 초기화
void
CSetDialogSpeech::InitSpeech()
{
	CConversation* _lpcEdit;

	if	(s_ccEdit.m_wConversationCount	<=	0)
			_lpcEdit = NULL;
	else
		_lpcEdit= &s_ccEdit.m_pConversation[s_iSelectSpeech];

	if	(_lpcEdit)
		s_bIsEdit	=	TRUE;
	else
		s_bIsEdit	=	FALSE;
	
	s_iSelectContent	=	0;
	s_iSelectContent2	=	0xffff;

	s_cEdit.init(dMAX_DIALOG_TRIGGER_COUNT,dMAX_CONVERSATION_CONTENTS_COUNT);
	s_cEdit.m_wSerial	=	0;

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
	}
	
	g_iSelectKarma	=	0;
	
	{
		HWND	hDialogText	=	GetDlgItem(s_hWnd,IDC_DIALOG);

		s_hFont				=	CreateFont(13,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN, "굴림");

		SendMessage(hDialogText,WM_SETFONT,(WPARAM)s_hFont, 0 );
	}
	
	if	(s_cEdit.m_pstrSpeech)
		SetDlgItemText(s_hWnd,IDC_DIALOG,s_cEdit.m_pstrSpeech);
	
	if	(s_cEdit.m_wIsNoSpeech)
		CheckDlgButton(s_hWnd,IDC_NO_SPEECH,TRUE);
}

//
// 대화 출력(기존 SET_DIALOG_SPEECH 부분)
void
CSetDialogSpeech::DrawSpeechList()
{
	if (!s_dibSpeechList.Active()) 
		return;

	if (s_ccEdit.m_wConversationCount<=	0)
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_SPEECH)	,FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_COPY_SPEECH)		,FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_PASTE_SPEECH)	,FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_SPEECH)	,TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_COPY_SPEECH)		,TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_PASTE_SPEECH)	,TRUE); 
	}
	
	int	x,y,i;

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
}

//
// 대화 출력(기존 SET_DIALOG_ACTION 부분)
void
CSetDialogSpeech::DrawContents()
{
	int	x,y,i,iWidth;
	
	if	(s_cEdit.m_wContentsCount	<=	0)	//	컨텐츠 카운트가 0이다.
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_CONTENTS)	,FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_COPY_CONTENTS)	,FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_PASTE_CONTENTS)	,FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_NEW)				,FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_CONTENTS)	,TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_COPY_CONTENTS)	,TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_PASTE_CONTENTS)	,TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_NEW)				,TRUE);
	}

	if	(s_storeConversation.m_wContentsCount	==	0)
		EnableWindow(GetDlgItem(s_hWnd,IDC_PASTE_CONTENTS)			,FALSE);
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

		SetDlgItemText(s_hWnd,IDC_SPEECH2,"등록된 선택문이 없습니다.");
		s_dibContents.Draw();

		return;
	}
	else
		SetDlgItemText(s_hWnd,IDC_SPEECH2,s_cEdit.m_pContents[s_iSelectContent].m_pstrTitle);

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
// 카르마 출력(기존 SET_DIALOG_SPEECH. SET_DIALOG_ACTION 부분  
void
CSetDialogSpeech::DrawSpeechKarma()
{
	cKARMA_WND::InitKarmaDlg(s_hWnd, FALSE, &s_cEdit.m_pContents[s_iSelectContent]);
}

//
// SPEECH 정보 얻어오기(선택)
int
CSetDialogSpeech::GetSelectSpeechList(HWND hwnd)
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
// SPEECH 선택
int
CSetDialogSpeech::SelectSpeechList(HWND hwnd)
{
	int	 iSelectSpeech	=	GetSelectSpeechList(hwnd);

	if	(iSelectSpeech	==	0xffff)
	{
		DrawSpeechList();
		DrawTrigger();

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

		DrawSpeechList();
		DrawTrigger();
		InitSpeech();
		DrawContents();
		DrawSpeechKarma();

		return	iSelectSpeech;
	}

	return	0xffff;
}

//
// SPEECH 위로..
void
CSetDialogSpeech::UpSelectSpeechList(HWND _hDlg)
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

	DrawSpeechList();
	DrawTrigger();
}

// SPEECH 아래로..
void
CSetDialogSpeech::DownSelectSpeechList(HWND _hDlg)
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

		int	iSpeech	=	i+iSSEnd;

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
	DrawSpeechList();
	DrawTrigger();
}

//
// SPEECH 삭제
void
CSetDialogSpeech::RemoveSpeechList(HWND _hDlg)
{
	if	(s_ccEdit.m_wConversationCount	<=	0)
		return;

	s_ccEdit.removeSpeech(s_iSelectSpeech);

	if	(s_iSelectSpeech	>=	s_ccEdit.m_wConversationCount)
		s_iSelectSpeech	=	s_ccEdit.m_wConversationCount-1;

	s_cEdit.reset();
	InitSpeech();

	DrawSpeechList();
	DrawTrigger();
	DrawContents();
	DrawSpeechKarma();
}

// 
// SPEECH 복사
void
CSetDialogSpeech::CopySpeechList(HWND _hDlg)
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
//
// SPEECH 붙여넣기
void
CSetDialogSpeech::PasteSpeechList(HWND _hDlg)
{
	if	(s_storeCC.m_wConversationCount	<=	0)
		return;

	for	(int i=0;i<s_storeCC.m_wConversationCount;i++)
		s_ccEdit.addSpeech(&s_storeCC.m_pConversation[i]);

	s_iSelectSpeech	=	s_ccEdit.m_wConversationCount-1;
	s_iSelectSpeech2=	0xffff;

	DrawSpeechList();
	DrawTrigger();
}

//
// SPEECH 저장
void
CSetDialogSpeech::SaveSpeechList()
{
	if(!s_bIsSaveAddSpeech)
	{
		if (s_ccEdit.m_wConversationCount<=	0)
		{
			s_ccEdit.reset();
			InitSpeech();
			DrawSpeechList();
			DrawTrigger();
			DrawContents();
			DrawSpeechKarma();
			return;
		}
	}

	char	strSpeech[2048];
	GetDlgItemText(s_hWnd,IDC_DIALOG,strSpeech,sizeof(strSpeech));
	
	if(s_bIsSaveAddSpeech)
	{
		if (s_ccEdit.m_wConversationCount<=	0)
		{
			s_ccEdit.init(dMAX_CONVERSATION_CONTENTS_COUNT);
			s_ccEdit.m_wSerial				=	0;
			s_ccEdit.m_wConversationCount	=	0;
		}
		
		s_cEdit.reset();
		s_cEdit.setSpeech(strSpeech);
		s_ccEdit.addSpeech(&s_cEdit);
		s_iSelectSpeech	=	s_ccEdit.m_wConversationCount-1;
		InitSpeech();
		
	}
	else
	{
		s_cEdit.setSpeech(strSpeech);
		s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
	}
		
	HWND	hSizeList				=	GetDlgItem(s_hWnd,IDC_SIZE_LIST);
	s_ccEdit.m_wDialogBoxSize		=	cCOMBOBOX::GetData(hSizeList);
	s_ccEdit.m_wIsRandomChoiceDialg	=	IsDlgButtonChecked(s_hWnd,IDC_RANDOM);

	SetFocus(GetDlgItem(s_hWnd,IDC_DIALOG));
	SendMessage(GetDlgItem(s_hWnd,IDC_DIALOG),EM_SETSEL,0,-1);
	
	DrawSpeechList();
	DrawTrigger();
	DrawContents();
	DrawSpeechKarma();
	
	s_bIsSaveAddSpeech				=	FALSE;
}

//
// 트리거 출력(기존 SET_DIALOG_CONTAINER 부분)
void
CSetDialogSpeech::DrawTrigger()
{
	if	(!s_dibTriggerList.Active())
		return;

	int	x,y,i;

	if	(s_ccEdit.m_wConversationCount	<=	0)
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_ADD_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_TRIGGER),FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_EDIT_TRIGGER	),FALSE);

		EnableWindow(GetDlgItem(s_hWnd,IDC_AND_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_OR_TRIGGER	),FALSE);
		SetDlgItemInt(s_hWnd,IDC_ACTIVATE_TRIGGER_COUNT,0,FALSE);

		cDRAW::Fill(0);
		cTEXT::FitPut(4,2,cDRAW::WIDTH-14,100,WHITE,dMSG_NOT_EXIST_TRIGGER_FOR_SELECT_FIRST_DIALOG);
		s_dibTriggerList.Draw();
		return;
	}

	EnableWindow(GetDlgItem(s_hWnd,IDC_ADD_TRIGGER),TRUE);

	CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	if (lpSelectSpeech->m_wTriggerCount	<=	0)	//	트리거 카운트가 0이다.
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_TRIGGER),FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_EDIT_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_AND_TRIGGER	),FALSE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_OR_TRIGGER	),FALSE);
		SetDlgItemInt(s_hWnd,IDC_ACTIVATE_TRIGGER_COUNT,0,FALSE);

		cDRAW::Fill(0);
		cTEXT::FitPut(4,2,cDRAW::WIDTH-14,100,WHITE,dMSG_NOT_EXIST_TRIGGER_FOR_SELECT_FIRST_DIALOG);
		s_dibTriggerList.Draw();
		return;
	}
	else
	{
		EnableWindow(GetDlgItem(s_hWnd,IDC_REMOVE_TRIGGER),TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_EDIT_TRIGGER	),TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_AND_TRIGGER	),TRUE);
		EnableWindow(GetDlgItem(s_hWnd,IDC_OR_TRIGGER	),TRUE);
	}

	SetDlgItemInt(s_hWnd,IDC_ACTIVATE_TRIGGER_COUNT,lpSelectSpeech->m_bf7ActivateTriggerCount,FALSE);

	CheckDlgButton(s_hWnd,IDC_AND_TRIGGER,FALSE);
	CheckDlgButton(s_hWnd,IDC_OR_TRIGGER,FALSE);

	if	(lpSelectSpeech->m_isWantAndCondition)
		CheckDlgButton(s_hWnd,IDC_AND_TRIGGER,TRUE);
	else
		CheckDlgButton(s_hWnd,IDC_OR_TRIGGER,TRUE);

	if	(lpSelectSpeech->m_isWantNotComplete)
		CheckDlgButton(s_hWnd,IDC_ACTIVE_CONDITION,TRUE);
	else
		CheckDlgButton(s_hWnd,IDC_ACTIVE_CONDITION,FALSE);

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

	s_dibTriggerList.Draw();		
}

//
// 트리거 정보 가져오기(선택)
int
CSetDialogSpeech::GetSelectTrigger(HWND hwnd)
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

		if (y	>=	s_dibTriggerList.HEIGHT)	break;
	}

	return	0xffff;
}

//
// 트리거 선택
int
CSetDialogSpeech::SelectTrigger(HWND hwnd)
{
	int	 iSelectTrigger	=	GetSelectTrigger(hwnd);

	if (iSelectTrigger	==	0xffff)
	{
		DrawTrigger();

		return	0xffff;
	}

	if(s_iSelectTrigger	!=	iSelectTrigger)
	{
		s_iSelectTrigger=	iSelectTrigger;

		DrawTrigger();

		return	iSelectTrigger;
	}

	return	0xffff;
}

//
// 트리거 추가
void
CSetDialogSpeech::AddTrigger(HWND _hDlg)
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

	DrawTrigger();
}

//
//	트리거 수정
void
CSetDialogSpeech::EditTrigger(HWND _hDlg)
{
	if (s_ccEdit.m_wConversationCount	<=	0)	return;

	CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	g_EditKarma.m_wTriggerCount	=	0;
	
	int	iIndex	=	cKARMA_WND::AddTrigger(_hDlg,&lpSelectSpeech->m_pTrigger[s_iSelectTrigger]);

	if (iIndex	==	0xffff)	return;

	lpSelectSpeech->m_pTrigger[s_iSelectTrigger].copy(&g_EditKarma.m_pTrigger[0]);

	DrawTrigger();
}

//
// 트리거 삭제
void
CSetDialogSpeech::RemoveTrigger(HWND _hDlg)
{
	if (s_ccEdit.m_wConversationCount	<=	0)	return;

	CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	lpSelectSpeech->removeTrigger(s_iSelectTrigger);
	DrawTrigger();
}

//
// 트리거 체크 버튼 확인
void
CSetDialogSpeech::CheckTriggerCompleteMethod(HWND _hDlg)
{
	if (s_ccEdit.m_wConversationCount	<=	0)	return;

	CConversation	*lpSelectSpeech		=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사

	lpSelectSpeech->m_isWantAndCondition=	IsDlgButtonChecked(_hDlg,IDC_AND_TRIGGER);
	lpSelectSpeech->m_isWantNotComplete	=	IsDlgButtonChecked(_hDlg,IDC_ACTIVE_CONDITION);

	DrawTrigger();
}

//
// 컨텐츠 정보 가져오기(선택)
int
CSetDialogSpeech::GetSelectContent(HWND hwnd)
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
// 컨텐츠 선택
int
CSetDialogSpeech::SelectContent(HWND hwnd)
{
	int	 iSelectContent	=	GetSelectContent(hwnd);

	if	(iSelectContent	==	0xffff)
	{
		DrawContents();
		DrawSpeechKarma();
		
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

		DrawContents();
		DrawSpeechKarma();
		
		return	iSelectContent;
	}

	return	0xffff;
}

//
// 컨텐츠 위로..
void
CSetDialogSpeech::MoveUpContents()
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

	DrawContents();
	DrawSpeechKarma();
}

//
// 컨텐츠 아래로..
void
CSetDialogSpeech::MoveDownContents()
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

	DrawContents();
	DrawSpeechKarma();
}

//
// 컨텐츠 복사
void
CSetDialogSpeech::CopyContents()
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

	DrawContents();
	DrawSpeechKarma();
}

//
// 컨텐츠 붙여넣기
void
CSetDialogSpeech::PasteContents()
{
	if	(s_storeConversation.m_wContentsCount	<=	0)
		return;

	for	(int i=0;i<s_storeConversation.m_wContentsCount;i++)
		s_cEdit.addContent(&s_storeConversation.m_pContents[i]);

	s_iSelectContent	=	s_cEdit.m_wContentsCount-1;
	s_iSelectContent2	=	0xffff;

	DrawContents();
	DrawSpeechKarma();
}

//
// 컨텐츠 삭제
void
CSetDialogSpeech::RemoveContents()
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

	DrawContents();

	if	(s_cEdit.m_wContentsCount	<=	0)
		s_cEdit.m_pContents[s_iSelectContent].m_wKarmaCount = 0;
		
	DrawSpeechKarma();
}

//
// 컨텐츠 저장
void
CSetDialogSpeech::SaveContents()
{	
	if(!s_bIsSaveAddContents)
	{
		if	(s_cEdit.m_wContentsCount	<=	0)
		{
			s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
			DrawSpeechList();
			DrawTrigger();
			DrawContents();
			DrawSpeechKarma();
			return;
		}
	}

	char	strSpeech[2048];
	BOOL	bTemp;
	GetDlgItemText(s_hWnd,IDC_SPEECH2,strSpeech,sizeof(strSpeech));

	if(s_bIsSaveAddContents)
	{
		CKarmaContainer	kc;
		kc.allocBuffer(dKARMA_COUNT);
		kc.setTitle(strSpeech);
		
		s_cEdit.addContent(&kc);
		s_iSelectContent	=	s_cEdit.m_wContentsCount-1;
		s_iSelectContent2	=	0xffff;

		s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
	}
	else
	{
		s_cEdit.m_pContents[s_iSelectContent].setTitle(strSpeech);
		s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
	}

	s_cEdit.m_wIsNoSpeech				=	IsDlgButtonChecked(s_hWnd,IDC_NO_SPEECH);
	s_cEdit.m_bf7ActivateTriggerCount	=	GetDlgItemInt(s_hWnd,IDC_ACTIVATE_TRIGGER_COUNT,&bTemp,FALSE);

	DrawSpeechList();
	DrawTrigger();
	DrawContents();
	DrawSpeechKarma();

	s_bIsSaveAddContents = FALSE;
}

//
// 카르마 복사
void
CSetDialogSpeech::CopyKarma()
{
	if	(g_lpAKC->getKarmaCount()	<=	0)
		return;
	
	CKarma	*lpKarma	=	g_lpAKC->getSelectKarma();

	s_aStoredKarma.allocBuffer(dKARMA_COUNT);
	s_aStoredKarma.copy(lpKarma);
	s_aStoredKarma.setTitle(_ms("%s[2]",lpKarma->m_pstrTitle));

	cKARMA_WND::s_iCopiedKaramaCount	=	1;

	DrawSpeechKarma();
}

//
// 카르마 붙여넣기
void
CSetDialogSpeech::PasteKarma()
{	
	if	(cKARMA_WND::s_iCopiedKaramaCount	==	0)
		return;

	char strBuffer[2048];
	strcpy(strBuffer, s_cEdit.m_pContents[s_iSelectContent].m_pstrTitle);

	CKarmaContainer *_lpKc = &s_cEdit.m_pContents[s_iSelectContent];
	
	CKarmaContainer *_lpKc1 = new CKarmaContainer;
	_lpKc1->allocBuffer(dKARMA_COUNT);
	_lpKc1->copy(_lpKc);
	
	_lpKc->allocBuffer(dKARMA_COUNT);
	int iAddedKarma= _lpKc->add(&s_aStoredKarma);
	_lpKc->selectKarma(iAddedKarma);

	for(int i=0; i<iAddedKarma; ++i)
		_lpKc->m_pKarma[i].copy(&_lpKc1->m_pKarma[i]);
	
	s_cEdit.m_pContents[s_iSelectContent].setTitle(strBuffer);	

	DrawSpeechKarma();
}

//
// 기본 Proc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK
CSetDialogSpeech::ProcSetDialog(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	
	HWND hAddButton;	// insu add
	
	
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			InitSetDialogBoxData(hwnd);
			
			// insu add
			hAddButton = GetDlgItem(hwnd, IDC_SAVE_SPEECH);
			EnableWindow(hAddButton, false);
			ShowWindow(hAddButton, SW_HIDE);
			hAddButton = GetDlgItem(hwnd, IDC_SAVE_CONTENTS_SPEECH);
			EnableWindow(hAddButton, false);
			ShowWindow(hAddButton, SW_HIDE);
			hAddButton = GetDlgItem(hwnd, IDC_SAVE_KARMA);
			EnableWindow(hAddButton, false);
			ShowWindow(hAddButton, SW_HIDE);	
			
			// 좌상측 대화상자
			s_bIsSaveAddSpeech				=	TRUE;
			SetDlgItemText(hwnd,IDC_DIALOG,"");	
			
			if (s_ccEdit.m_wConversationCount == 0)
			{
				hAddButton = GetDlgItem(hwnd, IDC_ADD_CONTENTS);
				EnableWindow(hAddButton, false);
			}
			
			// 우상측 대화상자
			s_bIsSaveAddContents	= TRUE;
			SetDlgItemText(hwnd,IDC_SPEECH2,"");	
			
			// 포커스를 왼쪽 텍스트 박스로 이동시켜준다.
			SetFocus(GetDlgItem(s_hWnd,IDC_DIALOG));
			// insu add end
			break;
		}
		
	case WM_CLOSE		:
		EndDialog(hwnd,IDCANCEL);
		break;	
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDC_UP		:
			UpSelectSpeechList(hwnd);
			break;
		case IDC_DOWN	:
			DownSelectSpeechList(hwnd);
			break;
		case IDC_ADD_SPEECH		:
			{
				// insu add .. add 
				
				SaveSpeechList();
				
				s_bIsSaveAddSpeech				=	TRUE;
				SetDlgItemText(hwnd,IDC_DIALOG,"");	
				SetFocus(GetDlgItem(s_hWnd,IDC_DIALOG));
				
				hAddButton = GetDlgItem(hwnd, IDC_ADD_CONTENTS);
				EnableWindow(hAddButton, true);			
				// insu add end
				break;
			}
		case IDC_SAVE_SPEECH	:
			// save
			// insu add .. 일단 저장 기능은 막는다.
			//SaveSpeechList();
			break;
		case IDC_COPY_SPEECH	:
			CopySpeechList(hwnd);			
			break;
		case IDC_PASTE_SPEECH	:
			PasteSpeechList(hwnd);
			break;
		case IDC_REMOVE_SPEECH	:
			RemoveSpeechList(hwnd);
			break;
		case IDC_ADD_TRIGGER	:
			AddTrigger(hwnd);	
			break;
		case IDC_EDIT_TRIGGER	:
			EditTrigger(hwnd);	
			break;
		case IDC_REMOVE_TRIGGER	:
			RemoveTrigger(hwnd);	
			break;
		case	IDC_AND_TRIGGER			:
			CheckTriggerCompleteMethod(hwnd);
			break;
		case	IDC_OR_TRIGGER			:
			CheckTriggerCompleteMethod(hwnd);
			break;
		case	IDC_ACTIVATE_TRIGGER_COUNT	:
			if	(HIWORD(wParam)	==	EN_CHANGE)
			{
				if	(s_ccEdit.m_wConversationCount	<=	0)
					break;
				
				CConversation	*lpSelectSpeech	=	&s_ccEdit.m_pConversation[s_iSelectSpeech];	//	선택한 대사
				
				BOOL	bTemp;
				int		iCount	=	GetDlgItemInt(hwnd,IDC_ACTIVATE_TRIGGER_COUNT,&bTemp,FALSE);
				
				if	(bTemp)
				{
					iCount		=	min(lpSelectSpeech->m_wTriggerCount,iCount);
					lpSelectSpeech->m_bf7ActivateTriggerCount	=	iCount;
				}
				break;
			}
			break;
		case IDC_UP2		:
			MoveUpContents();
			break;
		case IDC_DOWN2		:
			MoveDownContents();
			break;
		case IDC_COPY_CONTENTS	:
			CopyContents();
			break;	
		case IDC_PASTE_CONTENTS	:
			PasteContents();
			s_bIsSaveAddContents = FALSE;
			SaveContents();
			break;
		case IDC_REMOVE_CONTENTS	:
			RemoveContents();
			s_bIsSaveAddContents = FALSE;
			SaveContents();
			break;
		case IDC_ADD_CONTENTS		:

			s_bIsSaveAddContents	= TRUE;
			SaveContents();
			SetDlgItemText(hwnd,IDC_SPEECH2,"");	
			SetFocus(GetDlgItem(hwnd,IDC_SPEECH2));
			
			break;
		case IDC_SAVE_CONTENTS_SPEECH	:
			SaveContents();
			break;
		case IDC_UP_KARMA	:
			cKARMA_WND::UpKarma(hwnd);
			break;
		case IDC_DOWN_KARMA	:
			cKARMA_WND::DownKarma(hwnd);
			break;
		case	IDC_KARMA_LIST		:
			cKARMA_WND::SelectKarma(hwnd);
			break;
		case	IDC_REMOVE	:
			cKARMA_WND::RemoveKarma(hwnd);
			s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
			break;
		case	IDC_COPY	:
			CopyKarma();
			break;
		case	IDC_PASTE	:
			PasteKarma();	
			s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
			break;
		case IDC_SAVE_KARMA:
			// insu add
			//s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
			// insu add end
			break;
		case IDC_NEW	:
			{	
				CKarma	*lpKarma=	CSetDialogKarma::AddKarma(hwnd,NULL,eADD_KARMA_NORMAL);
				
				if(!lpKarma)
					break;

				CKarmaContainer *_lpKc = &s_cEdit.m_pContents[s_iSelectContent];
				
				CKarmaContainer *_lpKc1 = new CKarmaContainer;
				_lpKc1->allocBuffer(dKARMA_COUNT);
				_lpKc1->copy(_lpKc);
				
				char	strSpeech[2048];
				//GetDlgItemText(s_hWnd,IDC_SPEECH2,strSpeech,sizeof(strSpeech));
				strcpy(strSpeech, s_cEdit.m_pContents[s_iSelectContent].m_pstrTitle);	
				
				_lpKc->allocBuffer(dKARMA_COUNT);
				_lpKc->setTitle(strSpeech);

				int iAddedKarma= _lpKc->add(lpKarma);
				_lpKc->selectKarma(iAddedKarma);
				
				for(int i=0; i<iAddedKarma; ++i)
					_lpKc->m_pKarma[i].copy(&_lpKc1->m_pKarma[i]);
				
				if (iAddedKarma	!=	0xffff)
				{
					s_cEdit.m_pContents[s_iSelectContent].selectKarma(iAddedKarma);

					
					cKARMA_WND::UpdateKarmaDlg(hwnd);
					g_lpActmap->LostVirgin();
				}
				// insu add
				s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
				// insu add end
				break;
			}
		case IDC_EDIT	:
			{
				CKarma	*lpKarma	=	g_lpAKC->getSelectKarma();
				CKarma	*lpAddKarma	=	CSetDialogKarma::AddKarma(hwnd,lpKarma,eADD_KARMA_NORMAL);

				if(!lpAddKarma)
					break;
				
				s_cEdit.m_pContents[s_iSelectContent].m_pKarma[g_iSelectKarma].copy(lpAddKarma);
				cKARMA_WND::UpdateKarmaDlg(hwnd);
				g_lpActmap->LostVirgin();
				s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
			}
			break;
		case IDC_MODIFY_CONTENTS:
			char	strSpeech[2048];
			GetDlgItemText(s_hWnd,IDC_SPEECH2,strSpeech,sizeof(strSpeech));
			s_cEdit.m_pContents[s_iSelectContent].setTitle(strSpeech);			
			s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
			DrawContents();
			break;
		case IDC_MODIFY_SPEECH:			
			GetDlgItemText(s_hWnd,IDC_DIALOG,strSpeech,sizeof(strSpeech));
			s_cEdit.setSpeech(strSpeech);
			s_ccEdit.m_pConversation[s_iSelectSpeech].copy(&s_cEdit);
			DrawSpeechList();
			break;
		case IDC_OK		:
			EndDialog(hwnd,IDOK);
			break;
		case IDC_CANCEL	:
			EndDialog(hwnd,IDCANCEL);
			break;

		}
		break;
	}

	return (0L);
}

LRESULT CALLBACK		
CSetDialogSpeech::ProcSpeechListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case	WM_PAINT		:
		{	
			PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibSpeechList.Active();
			s_dibSpeechList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}
		case	WM_VSCROLL		:
		{
			if (s_sbSpeechList.VScroll(wParam, 5))			// 09.12.17 수정
				DrawSpeechList();
			break;
		}
		case WM_LBUTTONDOWN:
			SetFocus(hwnd);						// 09.12.17 수정
			SelectSpeechList(hwnd);
			break;
		case WM_MOUSEWHEEL :
		{			
			int zDelta = ((short)HIWORD(wParam));
			
			if	(zDelta>=0)
				s_sbSpeechList.VScroll(SB_LINEUP, 5);
			else
				s_sbSpeechList.VScroll(SB_LINEDOWN, 5);

			DrawSpeechList();
						
			break;
		}
		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}
	
	return (0L);
}

LRESULT CALLBACK		
CSetDialogSpeech::ProcTriggerWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case	WM_PAINT		:
		{	
			PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibTriggerList.Active();
			s_dibTriggerList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}
		
		case	WM_VSCROLL		:
		{
			if	(s_sbTriggerList.VScroll(wParam))
				DrawTrigger();
			break;
		}
		case WM_LBUTTONDOWN:
			SelectTrigger(hwnd);
			break;
		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}
	return (0L);
}

LRESULT CALLBACK		
CSetDialogSpeech::ProcContentsWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	
			PAINTSTRUCT ps;
			BeginPaint( hwnd, &ps );

			s_dibContents.Active();
			s_dibContents.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_VSCROLL			:
			s_sbContents.VScroll(wParam, 5);			// 09.12.17 수정
			DrawContents();
			break;

		case WM_LBUTTONDOWN	:
			SetFocus(hwnd);						// 09.12.17 수정
			SelectContent(hwnd);
			break;
		case WM_MOUSEWHEEL :
		{		
			int zDelta = ((short)HIWORD(wParam));
			
			if	(zDelta>=0)
				s_sbContents.VScroll(SB_LINEUP, 5);
			else
				s_sbContents.VScroll(SB_LINEDOWN, 5);

			DrawContents();
			break;
		}		
		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}
	return (0L);
}