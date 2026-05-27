#include "CSetDialogList.h"
#include "cDRAW.h"
#include "resource.h"
#include "cTEXT.h"
#include "CSetDialog.h"



HWND			CSetDialogList::s_hWnd;
cLISTCONTROL	CSetDialogList::s_listSpeaker;	//	접두사 리스트
cDIBWND			CSetDialogList::s_dibSpeech;
int				CSetDialogList::s_iSpeechListWndPos	=	0;
int				CSetDialogList::s_iSelectSpeaker	=	0xffff;

void
CSetDialogList::Init(HWND hwnd)
{
	DialogBox( s_hInst, "SET_DIALOG_LIST",hwnd,(DLGPROC)ProcSetDialog);
}	//	CSetDialogList::Init(HWND hwnd)

//
//
void
CSetDialogList::InitSetDialogListBoxData(HWND _hDlg)
{
	HWND	hSpeech	=	GetDlgItem(_hDlg,IDC_SPEECH);

	s_dibSpeech.lpszRegister	=	"SPEECH_WINDOW";
	s_dibSpeech.Init(hSpeech,(WNDPROC)ProcSpeechWnd);

//	컨텐츠 윈도우 초기화
	{
		HWND	hSpeechList	=	GetDlgItem(_hDlg,IDC_SPEECH_LIST);

		s_listSpeaker.Init(hSpeechList			,2,FALSE,ProcSpeechListWnd);		//	3개의 컬럼
		s_listSpeaker.AddColumn("No"			,32);
		s_listSpeaker.AddColumn("이름"			,6*36);

		ResetSpeakerListBox(_hDlg);
	}

	Draw(_hDlg);
}	//	CSetDialogList::InitSetDialogListBoxData(HWND _hDlg)

//
//	기본 아이템 목록 설정
void
CSetDialogList::ResetSpeakerListBox(HWND hwnd)
{
	cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;

	int			iCount	=	0,i;

	s_listSpeaker.ResetItems();

	for (i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		cACTOR *lpActor	=	lpActorSet->getActor(i);

		if (!lpActor						)	continue;
		if (lpActor->m_pCC==NULL	)	continue;
		iCount++;

		char	*lpText		=	lpActor->getCharacter()->m_strName;

		s_listSpeaker.AddItem(0,_ms("%d",iCount),i);
		s_listSpeaker.AddItem(1,lpText);
		s_listSpeaker.IncreaseLineCounter();
	}

	if (!s_listSpeaker.selectByData(0,s_iSelectSpeaker))
	{
		if (!s_listSpeaker.Select(0))	s_iSelectSpeaker	=	0xffff;
		else							s_iSelectSpeaker	=	s_listSpeaker.GetData(0);
	}

	s_listSpeaker.setHeightPos(s_iSpeechListWndPos);
}	//	CSetDialogList::ResetSpeakerListBox(HWND hwnd)

//대화 리스트 그리기
void
CSetDialogList::Draw(HWND _hDlg)
{
	s_listSpeaker.Draw();

	if (!s_dibSpeech.Active()) return;

	cDRAW::Fill(_SKYBLUE);

	int	x,y,i,iWidth;

	x		=	5;
	y		=	5;
	iWidth	=	cDRAW::WIDTH-18;

	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(s_iSelectSpeaker);
	CConversationContainer	*lpConversation=NULL;

	if (lpActor)	lpConversation	=	lpActor->m_pCC;

	if (!lpConversation)
	{
		s_dibSpeech.Draw();
		return;
	}

	for (i=0;i<lpConversation->m_wConversationCount;i++)
	{
		char	*lpText		=	_ms("%2d) %s",i+1,lpConversation->m_pConversation[i].m_pstrSpeech);

		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		cDRAW::Fill		(0,x,y,x+iWidth+6,y+iLineCount*14+4);

		cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);
		cTEXT::FitPut	(x,y+1,iWidth,iLineCount*8*2,WHITE,lpText);

		y					+=	iLineCount*14;
		y					+=	4+5;

		if (y	>=	cDRAW::HEIGHT)	break;
	}

	s_dibSpeech.Draw();
}

//
//	대화 편집
void
CSetDialogList::EditSpeech(HWND _hDlg)
{
	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(s_iSelectSpeaker);
	CConversationContainer	*lpConversation=NULL;
	if (lpActor)	lpConversation	=	lpActor->m_pCC;

	if (!lpConversation)	return;

	CConversationContainer*	lpData	=	CSetDialog::Init(_hDlg,lpActor->getName(),lpActor->m_pCC);

	if (!lpData)	return;

	if (!lpActor->m_pCC)	lpActor->m_pCC	=	new CConversationContainer();

	lpActor->m_pCC->copy(lpData);
}	//	CSetDialogList::EditSpeech(HWND _hDlg)

//
//대화 제거
void
CSetDialogList::RemoveSpeech(HWND _hDlg)
{
	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(s_iSelectSpeaker);

	if (!lpActor)	return;

	int	iIndex	=	g_lpActmap->m_setActor.getSpeakerIndex(s_iSelectSpeaker);

	KILL(lpActor->m_pCC);

	lpActor		=	g_lpActmap->m_setActor.getSpeaker(iIndex);

	if (!lpActor && iIndex>0)
		lpActor	=	g_lpActmap->m_setActor.getSpeaker(iIndex-1);

	if (lpActor)	s_iSelectSpeaker	=	lpActor->m_iSerial;

	s_iSpeechListWndPos	=	s_listSpeaker.getHeightPos();

	g_lpActmap->m_isVIRGIN	=	FALSE;

	ResetSpeakerListBox(_hDlg);

	Draw(_hDlg);
}	//	CSetDialogList::RemoveSpeech(HWND _hDlg)

//
//대화 제거
void
CSetDialogList::RemoveAllSpeech(HWND _hDlg)
{
	cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;

	s_listSpeaker.ResetItems();

	g_lpActmap->m_isVIRGIN	=	FALSE;

	for (int i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		cACTOR *lpActor	=	lpActorSet->getActor(i);

		if (!lpActor)	continue;

		KILL(lpActor->m_pCC);
	}

	Draw(_hDlg);
}	//	CSetDialogList::RemoveAllSpeech(HWND _hDlg)

LRESULT CALLBACK
CSetDialogList::ProcSetDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	case WM_INITDIALOG	:
			InitSetDialogListBoxData(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;

		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case	IDC_REMOVE_ALL_SPEECH	:
					RemoveAllSpeech(hDlg);
					break;

				case	IDC_EDIT_SPEECH			:
					EditSpeech(hDlg);
					break;

				case	IDC_REMOVE_SPEECH		:
					RemoveSpeech(hDlg);
					break;

				case	IDOK					:
					s_iSpeechListWndPos	=	s_listSpeaker.getHeightPos();
					EndDialog(hDlg,IDOK);
					break;
			}
			break;
	}

	return (0L);
}

LRESULT CALLBACK
CSetDialogList::ProcSpeechListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN	:
		{
			s_iSelectSpeaker	=	s_listSpeaker.GetSelectData();
			Draw(hwnd);
			break;
		}
	}

	return (0L);
}	//	cSET_ITEM_PREFIX::s_procItemPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)


//
//	컨디션 윈도우
LRESULT CALLBACK 
CSetDialogList::ProcSpeechWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibSpeech.Active();
			s_dibSpeech.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}	//	CSetDialog::ProcSpeechWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)