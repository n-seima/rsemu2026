#include "cSetDialogNpcData.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "cMAPWND.H"
#include <commctrl.h>
#include "CSetDialog.h"
#include "cMAIN.H"
#include "CSetDialogSpeech.h"

HWND			cSetDialogNpcData::s_hWnd					= NULL;
HWND			cSetDialogNpcData::s_hSlider				= NULL;
HWND			cSetDialogNpcData::s_hListTab				= NULL;
HWND			cSetDialogNpcData::s_hListTabDlg			= NULL;
HWND			cSetDialogNpcData::s_hPageList				= NULL;
HWND			cSetDialogNpcData::s_hPageFindList			= NULL;

cLISTCONTROL	cSetDialogNpcData::s_listNpc;
cLISTCONTROL	cSetDialogNpcData::s_FindNpc;
cDIBWND			cSetDialogNpcData::s_dibNpcSpeech;
cDIBWND			cSetDialogNpcData::s_dibNpcInfo;
cSCROLL_BAR		cSetDialogNpcData::s_sbNpcSpeech;

int				cSetDialogNpcData::s_iSpeechListWndPos		=	0;
int				cSetDialogNpcData::s_iSelectNpcData			=	0xffff;
int				cSetDialogNpcData::s_iSelectFindNpcData		=	0xffff;
int				cSetDialogNpcData::s_nNpcCount				=	0;
int				cSetDialogNpcData::s_nFindNpcCount			=	0;
int				cSetDialogNpcData::s_iSelectSpeech			=	0xffff;
int				cSetDialogNpcData::s_iSelectSpeech2			=	0xffff;
int				cSetDialogNpcData::s_iSelOldList			=	0;
BOOL			cSetDialogNpcData::s_bEnableDialog			=	FALSE;

//
// 메뉴에서 실행. 첫 실행시 생성. 화면 지정
void 
cSetDialogNpcData::Init(HWND hwnd)
{
	s_bEnableDialog = TRUE;

	if(s_hWnd != NULL)
		return;
	
	CreateDialog(s_hInst, "SET_NPC_DATA", hwnd, (DLGPROC)ProcSetDialog);

	RECT rt1, rt2;
	GetClientRect(g_MAPWND.hWND,&rt1);
	GetClientRect(s_hWnd, &rt2);
	
	SetWindowPos(s_hWnd, NULL, rt1.right - rt2.right + 100, 120, 0, 0, SWP_NOSIZE);
}	

//
// 탭컨트롤 페이지 데이터 설정
void
cSetDialogNpcData::InitPageData()
{
	s_hListTab = CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, s_hWnd, (HMENU)0, s_hInst, NULL);

	HFONT hFont = CreateFont(11, 0, 0, 0, 0, 0, 0, 0,HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Dotum"));
	SendMessage(s_hListTab, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

	TCITEM tie;
	tie.mask = TCIF_TEXT;
	tie.pszText = "List";
	TabCtrl_InsertItem(s_hListTab, 0, &tie);
	tie.pszText = "search";
	TabCtrl_InsertItem(s_hListTab, 1, &tie);

	HRSRC hRc;
	HGLOBAL hGlb;
	DLGTEMPLATEEX *dTem;
	RECT trt = {0,0,0, 0};

	hRc = FindResource(NULL, "PAGE_LIST", RT_DIALOG);
	hGlb = LoadResource(NULL, hRc);
	dTem = (DLGTEMPLATEEX*)LockResource(hGlb);
	trt.right = dTem->cx;
	trt.bottom = dTem->cy;

	hRc = FindResource(NULL, "PAGE_FIND_LIST", RT_DIALOG);
	hGlb = LoadResource(NULL, hRc);
	dTem = (DLGTEMPLATEEX*)LockResource(hGlb);
	trt.right = max(trt.right, dTem->cx);
	trt.bottom =max(trt.bottom, dTem->cy);

	s_hListTabDlg = CreateDialog(s_hInst, "PAGE_LIST", s_hWnd,ProcPageNpcList);
	MapDialogRect(s_hListTabDlg, &trt);
	DestroyWindow(s_hListTabDlg);
	
	TabCtrl_AdjustRect(s_hListTab, TRUE, &trt);
	
	OffsetRect(&trt, -trt.left, -trt.top);
	SetWindowPos(s_hListTab , NULL, 3, 10, 218, 800, SWP_NOZORDER);

	s_hListTabDlg  = CreateDialog(s_hInst, "PAGE_LIST", s_hWnd, ProcPageNpcList);
	ShowWindow(s_hListTabDlg, SW_SHOW);
}

//
// 컨트롤 설정
void
cSetDialogNpcData::InitSetDialogListBoxData(int sel)
{
	HWND	hList;
	HWND	hNpcSpeech	=	GetDlgItem(s_hWnd, IDC_NPC_SPEECH);
	HWND	hNpcInfo	=	GetDlgItem(s_hWnd, IDC_NPC_INFO);

	s_dibNpcSpeech.lpszRegister	=	"NPCSPEECH_WINDOW";
	s_dibNpcSpeech.Init(hNpcSpeech,(WNDPROC)ProcNpcSpeechWnd);
	s_sbNpcSpeech.Init(hNpcSpeech);
	s_sbNpcSpeech.InitHeight();
	
	s_dibNpcInfo.lpszRegister	=	"NPCINFO_WINDOW";
	s_dibNpcInfo.Init(hNpcInfo,(WNDPROC)ProcNpcInfoWnd);

	switch(sel)
	{
	case ePAGE_LIST:
		hList = GetDlgItem(s_hPageList,IDC_LIST);
		s_listNpc.Init(hList, 2, FALSE, ProcNpcListWnd);
		
		s_listNpc.AddColumn("No"			,32);
		s_listNpc.AddColumn("name"			,6*36);
		break;
	case ePAGE_FIND_LIST:
		hList = GetDlgItem(s_hPageFindList,IDC_FIND_LIST);
		s_FindNpc.Init(hList, 2, FALSE, ProcNpcFindListWnd);
		
		s_FindNpc.AddColumn("No"			,32);
		s_FindNpc.AddColumn("name"			,6*36);
		break;
	}
	
	ResetNpcListBox(sel);
}

//
// 리스트 컨트롤(NPC 리스트) 설정
void
cSetDialogNpcData::ResetNpcListBox(int sel)
{
	if(s_bEnableDialog == FALSE)
		return;
	
	if( sel == ePAGE_LIST )
	{
		cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;
		int i, iCount	= 0;
		s_nNpcCount	= -1;

		s_listNpc.ResetItems();

		for (i=0;i<dMAX_ACTOR_COUNT;i++)
		{
			cACTOR *lpActor	=	lpActorSet->getActor(i);
			
			if (!lpActor					)		continue;
			if( lpActor->m_wType == eNPC_MONSTER)	continue;
			
			iCount++;
			s_nNpcCount++;
			
			char	*lpText		=	lpActor->getName();
			
			s_listNpc.AddItem(0,_ms("%d",iCount),i);
			s_listNpc.AddItem(1,lpText);
			s_listNpc.IncreaseLineCounter();
		}
	
		if (!s_listNpc.selectByData(0,s_iSelectNpcData))
		{
			if (!s_listNpc.Select(0))	s_iSelectNpcData =	0xffff;
			else						s_iSelectNpcData =	s_listNpc.GetData(0);
		}
		
		s_listNpc.setHeightPos(s_iSpeechListWndPos);
	}
	else
	{
		char findstr[dNAME_LENGTH];
		GetDlgItemText(s_hPageFindList,IDC_EDIT_FIND_LIST, findstr, dNAME_LENGTH);

		if(strcmp(findstr, "") == 0)
		{
			DrawPageList(sel);
			return ;
		}
		
		cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;
		int i, iCount	= 0;
		s_nFindNpcCount = -1;
		s_FindNpc.ResetItems();
		
		for (i=0;i<dMAX_ACTOR_COUNT;i++)
		{
			cACTOR *lpActor	=	lpActorSet->getActor(i);
			
			if (!lpActor					)		continue;
			if( lpActor->m_wType == eNPC_MONSTER)	continue;
			
			char	*lpText		=	lpActor->getName();
			
			int iFilterNameLen	= strlen(findstr);
			int iItemNameLen	= strlen(lpText);
			int iLenGap			= iItemNameLen - iFilterNameLen;
			
			if(iLenGap<0)
				continue;
			
			for(int iName=0;iName<=iLenGap;++iName)
			{
				if(strnicmp(findstr,&lpText[iName], iFilterNameLen) == 0 )
				{
					++iCount;
					s_nFindNpcCount++;
					
					s_FindNpc.AddItem(0,_ms("%d",iCount),i);
					s_FindNpc.AddItem(1,lpText);
					s_FindNpc.IncreaseLineCounter();
					break;
				}
			}			
		}

		if (!s_listNpc.selectByData(0,s_iSelectNpcData))
		{
			if (!s_listNpc.Select(0))	s_iSelectNpcData =	0xffff;
			else						s_iSelectNpcData =	s_listNpc.GetData(0);
		}
		
		s_listNpc.setHeightPos(s_iSpeechListWndPos);

	}
	
	DrawPageList(sel);
	Draw();

}

//
// 탭 컨트롤쪽 출력(NPC LIST/ NPC FIND LIST)
void	
cSetDialogNpcData::DrawPageList(int sel)
{
	switch(sel)
	{
	case ePAGE_LIST:
		s_listNpc.Draw();
		break;
	case ePAGE_FIND_LIST:
		s_FindNpc.Draw();
		break;
	}	
}

//
// NPC 정보 / 대화 출력
void
cSetDialogNpcData::Draw()
{
	DrawNpcInfo();
	DrawNpcSpeech();
}

//
// NPC 대화 출력
void
cSetDialogNpcData::DrawNpcSpeech()
{
	int	x,y,i,iWidth;
	
	if (!s_dibNpcSpeech.Active()) 
		return;
	
	cDRAW::Fill(_SKYBLUE);
	
	x						=	5;
	y						=	5;
	iWidth					=	cDRAW::WIDTH-18;
	
	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectNpcData : selectlist = s_iSelectFindNpcData;

	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(selectlist);
	CConversationContainer	*lpConversation=NULL;
	
	if (lpActor)	
		lpConversation	=	lpActor->m_pCC;
	
	if (!lpConversation)
	{
		s_dibNpcSpeech.Draw();
		return;
	}
	
	for (i=0;i<lpConversation->m_wConversationCount;i++)
	{
		char	*lpText;
		
		if (lpConversation->m_pConversation[i].m_wIsNoSpeech)
			lpText		=	_ms("%2d)《%s》%s",i+1,dMSG_NOT_DIALOG_AND_REACT_FIRST_SELECT_SPEECH,lpConversation->m_pConversation[i].m_pstrSpeech);
		else
			lpText		=	_ms("%2d) %s",i+1,lpConversation->m_pConversation[i].m_pstrSpeech);
		
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);
		
		y					+=	iLineCount*14;
		y					+=	4+5;
	}
	
	int	iHeightPos	=	s_sbNpcSpeech.GetHeightPos();
	
	s_sbNpcSpeech.SetHeight(y,cDRAW::HEIGHT);
	s_sbNpcSpeech.SetHeightPos(iHeightPos);
	
	x		=	5;
	y		=	5-s_sbNpcSpeech.GetHeightPos();
	
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
	
	for (i=0;i<lpConversation->m_wConversationCount;i++)
	{
		char	*lpText;
		
		if (lpConversation->m_pConversation[i].m_wIsNoSpeech)
			lpText		=	_ms("%2d)《%s》%s",i+1,dMSG_NOT_DIALOG_AND_REACT_FIRST_SELECT_SPEECH,lpConversation->m_pConversation[i].m_pstrSpeech);
		else	lpText		=	_ms("%2d) %s",i+1,lpConversation->m_pConversation[i].m_pstrSpeech);
		
		ustrncpy(g_strCurrentSpeechList[i],lpText,62);
		
		if (y	>=	cDRAW::HEIGHT)	continue;
		
		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);
		
		cDRAW::Fill		(0,x,y,x+iWidth+6,y+iLineCount*14+4);
		
		if	(i>=iSSBegin	&&	i	<=	iSSEnd)
		{
			if	(i	==	s_iSelectSpeech)
				cDRAW::Fill		(_LTBLUE,x-1,y-1,x+iWidth+5,y+iLineCount*14+3);
			else
				cDRAW::Fill		(_BLUE,x-1,y-1,x+iWidth+5,y+iLineCount*14+3);
			
			if	(lpConversation->m_pConversation[i].m_wIsNoSpeech)
				cTEXT::FitPut	(x+1,y+2,iWidth,iLineCount*8*2,LTYELLOW,lpText);
			else
				cTEXT::FitPut	(x+1,y+2,iWidth,iLineCount*8*2,WHITE,lpText);
		}
		else
		{
			cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+iWidth+4,y+iLineCount*14+2);
			if (lpConversation->m_pConversation[i].m_wIsNoSpeech)
				cTEXT::FitPut	(x,y+1,iWidth,iLineCount*8*2,YELLOW,lpText);
			else	cTEXT::FitPut	(x,y+1,iWidth,iLineCount*8*2,WHITE,lpText);
		}
		
		y					+=	iLineCount*14;
		y					+=	4+5;
	}
	
	s_dibNpcSpeech.Draw();
	
}

//
// NPC 정보 출력
void
cSetDialogNpcData::DrawNpcInfo()
{
	if (!s_dibNpcInfo.Active()) 
		return;
		
	cDRAW::Fill(_DEEPGRAY);

	cTEXT::Put(5, 10, WHITE, "name: ");
	cTEXT::Put(5, 25, WHITE, "Category: ");
	cTEXT::Put(5, 40, WHITE, "Regen cycle: ");
	cTEXT::Put(5, 55, WHITE, "Action cycle: ");
	cTEXT::Put(5, 70, WHITE, "Direction of view: ");
	cTEXT::Put(5, 85, WHITE, "action area: ");
	cTEXT::Put(5, 100, WHITE, "Battle orientation: ");

	cTEXT::Put(5, 150, WHITE, "Created within action area: ");
	cTEXT::Put(5, 165, WHITE, "No automatic regeneration: ");
	cTEXT::Put(5, 180, WHITE, "Sometimes change direction: ");

	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectNpcData : selectlist = s_iSelectFindNpcData;

	cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
	cACTOR *lpActor			= lpActorSet->getActor(selectlist);

	if (!lpActor) {
		s_dibNpcInfo.Draw();
		return;
	}

	if(s_nNpcCount != -1)
	{
		cTEXT::Put(80,10,WHITE, _ms("%s(%d,%d)", lpActor->getName(), (lpActor->getXpos() / dTILE_WIDTH), (lpActor->getYpos() / dTILE_HEIGHT)));
		cTEXT::Put(80,25,WHITE, g_strNpcCategorty[lpActor->m_wType]);
		cTEXT::Put(80,40,WHITE, _ms("%d시간 %02d분 %02d초", lpActor->m_dwRegenerationCycle/60/60, (lpActor->m_dwRegenerationCycle/60)%60, lpActor->m_dwRegenerationCycle%60));
		cTEXT::Put(80,55,WHITE, _ms("%02d분 %02d초", lpActor->m_wWanderCycle/60, lpActor->m_wWanderCycle%60));
		cTEXT::Put(80,70,WHITE, g_str8Direct[lpActor->m_wDirect]);
		
		if(lpActor->m_wMoveArea == dMAX_AREA)
			cTEXT::Put(80, 85, WHITE, "Not moving");
		else if(lpActor->m_wMoveArea < 0 || lpActor->m_wMoveArea > dMAX_AREA)
			cTEXT::Put(80,85,WHITE, " ");
		else
			cTEXT::Put(80,85,WHITE, g_lpActmap->m_pArea->GetName(lpActor->m_wMoveArea));
		
		cTEXT::Put(80,100,WHITE, g_strBattleInclination[lpActor->m_wBattleInclination]);
	
		if(lpActor->m_wGenerateMethod == dMONSTER_GENERATE_ACTIVE_AREA)
			cTEXT::Put(150,150,LTGREEN, "YES");	
		else
			cTEXT::Put(150,150,LTGREEN, "NO");	

		if(lpActor->m_wIsBlockToAutoRegen)
			cTEXT::Put(110,165,LTGREEN, "YES");	
		else
			cTEXT::Put(110,165,LTGREEN, "NO");	

		if(lpActor->m_isChangeDirect)
			cTEXT::Put(130,180,LTGREEN, "YES");	
		else
			cTEXT::Put(130,180,LTGREEN, "NO");	
	}
	s_dibNpcInfo.Draw();
}

//
//	대화 편집
void
cSetDialogNpcData::EditSpeech(HWND _hDlg)
{
	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectNpcData : selectlist = s_iSelectFindNpcData;


	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(selectlist);
	CConversationContainer	*lpConversation=NULL;
	
	if (lpActor)	
		lpConversation	=	lpActor->m_pCC;
	else
		return;				// 10.03.05 추가
	CConversationContainer*	lpData	=	CSetDialogSpeech::Init(_hDlg, lpActor->getName(),lpActor->m_pCC);

	if (!lpData)	
		return;

	if (!lpActor->m_pCC)	
		lpActor->m_pCC	=	new CConversationContainer();

	lpActor->m_pCC->copy(lpData);

	Draw(); 
}

//
// 대화 제거
void
cSetDialogNpcData::RemoveSpeech(HWND _hDlg)
{

	if (MessageBox(_hDlg, "All dialogue for the selected character will be deleted. Do you still want to delete it?", "Delete dialogue", MB_YESNO) == IDYES)
	{
		int selectlist = 0;
		s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectNpcData : selectlist = s_iSelectFindNpcData;

		cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(selectlist);
		
		if (!lpActor)	return;
		
		int	iIndex	=	g_lpActmap->m_setActor.getSpeakerIndex(s_iSelectNpcData);
		
		KILL(lpActor->m_pCC);
		
		lpActor		=	g_lpActmap->m_setActor.getSpeaker(iIndex);
		
		if (!lpActor && iIndex>0)
			lpActor	=	g_lpActmap->m_setActor.getSpeaker(iIndex-1);
		
		if (lpActor)	s_iSelectNpcData	=	lpActor->m_iSerial;
		
		s_iSpeechListWndPos	=	s_listNpc.getHeightPos();
		
		g_lpActmap->m_isVIRGIN	=	FALSE;
		
		ResetNpcListBox(s_iSelOldList);
		
		Draw();
	}
}

//
// 모든 대화 제거
void
cSetDialogNpcData::RemoveAllSpeech(HWND _hDlg)
{
	if(g_lpActmap->m_bf1IsLocked)
	{
		MessageBox(_hDlg, "Drawing is prohibited.", "Notification", MB_OK);
		return;
	}

	if (MessageBox(_hDlg, "All characters' dialogue will be deleted. Do you want to delete?", "Delete all dialogue", MB_YESNO) == IDYES)
	{
		cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;
		g_lpActmap->m_isVIRGIN	=	FALSE;
		
		for (int i=0;i<dMAX_ACTOR_COUNT;i++)
		{
			cACTOR *lpActor	=	lpActorSet->getActor(i);
			
			if (!lpActor)	continue;
			
			KILL(lpActor->m_pCC);
		}
		
		Draw();
	}
}

//
// 선택된 NPC 삭제
void 
cSetDialogNpcData::RemmoveNpc(HWND _hDlg)
{
	if(g_lpActmap->m_bf1IsLocked)
	{
		MessageBox(_hDlg, "Drawing is prohibited.", "Notification", MB_OK);
		return;
	}

	if(MessageBox(_hDlg, "The selected character will be deleted. Do you want to delete it?", "Delete character", MB_YESNO) == IDYES)
	{
		int selectlist = 0;
		s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectNpcData : selectlist = s_iSelectFindNpcData;

		cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;
		lpActorSet	->remove(selectlist);
		g_MAPWND.Draw();
	}
	
}

//
// 모든 NPC 삭제
void
cSetDialogNpcData::RemoveAllNpc(HWND _hDlg)
{
	if(g_lpActmap->m_bf1IsLocked)
	{
		MessageBox(_hDlg, "Drawing is prohibited.", "Notification", MB_OK);
		return;
	}

	if(MessageBox(_hDlg, "All characters will be deleted. Do you want to delete them?", "Delete all characters", MB_YESNO) == IDYES)
	{
		cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;
		lpActorSet->removeAllNpc();
		
		ResetNpcListBox(s_iSelOldList);
		g_MAPWND.Draw();
	}
}

//
// NPC 정보 수정
void
cSetDialogNpcData::EditNpcInfo(HWND _hDlg)
{
	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectNpcData : selectlist = s_iSelectFindNpcData;

	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(selectlist);
	// 10.03.05
	if(!lpActor)
		return;
	cSET_FIELD_CHARACTER::Init(_hDlg, lpActor);
	g_MAPWND.Draw();
}

//
// 선택한 대화정보 받아옴
int
cSetDialogNpcData::GetSelectSpeech(HWND hwnd)
{
	int		x,y,i, iWidth;
	cPOINT	Pos;

	Pos.MousePos(hwnd);

	x		=	5;
	y		=	5-s_sbNpcSpeech.GetHeightPos();
	iWidth	=	cDRAW::WIDTH-18;

	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectNpcData : selectlist = s_iSelectFindNpcData;

	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(selectlist);
	CConversationContainer	*lpConversation=NULL;
		
	if (lpActor)	
		lpConversation	=	lpActor->m_pCC;

	if (!lpConversation)
		return FALSE;
	
	for (i	=	0;i<lpConversation->m_wConversationCount;i++)
	{
		char	*lpText;

		if (lpConversation->m_pConversation[i].m_wIsNoSpeech)
				lpText		=	_ms("%2d)《%s》%s",i+1,dMSG_NOT_DIALOG_AND_REACT_FIRST_SELECT_SPEECH,lpConversation->m_pConversation[i].m_pstrSpeech);
		else	lpText		=	_ms("%2d) %s",i+1,lpConversation->m_pConversation[i].m_pstrSpeech);

		int		iLineCount	=	cTEXT::GetLines(iWidth,lpText);

		if (y	>=	y	+	iLineCount*14+10)	break;

		cRECT	rect;

		rect.Set(0,y,iWidth	,y+iLineCount*14+4+5);

		if (rect.isIN(Pos.x,Pos.y))	return i;

		y					+=	iLineCount*14;
		y					+=	4+5;

		if (y	>=	s_dibNpcSpeech.HEIGHT)	break;
	}
	
	return	0xffff;
}

// 
// 대화를 선택
int
cSetDialogNpcData::SelectSpeech(HWND hwnd)
{
	
	int	 iSelectSpeech	=	GetSelectSpeech(hwnd);

	if	(iSelectSpeech	==	0xffff)
	{
		DrawNpcSpeech();
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

		DrawNpcSpeech();
	
		return	iSelectSpeech;
	}
	
	return	0xffff;
}

//
// 버튼 활성화 / 비활성화
void
cSetDialogNpcData::EnableButton(BOOL bFlag)
{
	HWND hButton = NULL;
	if(bFlag)
	{
		hButton = GetDlgItem(s_hWnd , IDC_EDIT_ABILITY);
		EnableWindow(hButton, TRUE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_NPC);
		EnableWindow(hButton, TRUE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_ALLNPC);
		EnableWindow(hButton, TRUE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_ALLSPEECH);
		EnableWindow(hButton, TRUE);

		hButton = GetDlgItem(s_hWnd , IDC_EDIT_NPCSPEECH);
		EnableWindow(hButton, TRUE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_NPCSPEECH);
		EnableWindow(hButton, TRUE);
	}
	else
	{	
		hButton = GetDlgItem(s_hWnd , IDC_EDIT_ABILITY);
		EnableWindow(hButton, FALSE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_NPC);
		EnableWindow(hButton, FALSE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_ALLNPC);
		EnableWindow(hButton, FALSE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_ALLSPEECH);
		EnableWindow(hButton, FALSE);

		hButton = GetDlgItem(s_hWnd , IDC_EDIT_NPCSPEECH);
		EnableWindow(hButton, FALSE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_NPCSPEECH);
		EnableWindow(hButton, FALSE);
	}
}

//
// 화면에 NPC 편집 대화상자 출력
void 
cSetDialogNpcData::ShowDialog()
{
	ShowWindow(s_hWnd, SW_SHOW);
}

//
// 베이스 Proc
LRESULT CALLBACK
cSetDialogNpcData::ProcSetDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int SelList = 0;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		s_hWnd	= hDlg;
		
		s_hSlider = GetDlgItem(hDlg ,IDC_SLIDER_NPC_DATA);
		SendMessage(s_hSlider, TBM_SETRANGE, FALSE, MAKELONG(DLG_ALPHAMIN,DLG_ALPHAMAX));
		SendMessage(s_hSlider, TBM_SETPOS, TRUE, g_iDlgAlpha);
		{
			SLWA pSetLayeredWindowAttributes = NULL;  
			HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL"); 
			pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
			SetWindowLong(hDlg, GWL_EXSTYLE,GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
			pSetLayeredWindowAttributes(hDlg, 0, g_iDlgAlpha, LWA_ALPHA);
		}

		InitPageData();
		break;
	case WM_HSCROLL:
		if ((HWND)lParam == s_hSlider)
		{
			g_iDlgAlpha	= SendMessage(s_hSlider, TBM_GETPOS,0,0);
			
			SLWA pSetLayeredWindowAttributes = NULL;  
			HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL"); 
			pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
			SetWindowLong(hDlg, GWL_EXSTYLE,GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
			pSetLayeredWindowAttributes(hDlg, 0, g_iDlgAlpha, LWA_ALPHA);
		}
		break;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			SelList = TabCtrl_GetCurSel(s_hListTab);
			
			if(s_iSelOldList != SelList)
			{
				if(s_hListTabDlg)	
					DestroyWindow(s_hListTabDlg);
				
				if(SelList  == ePAGE_LIST)
					s_hListTabDlg = CreateDialog(s_hInst, "PAGE_LIST", hDlg,ProcPageNpcList);
				if(SelList  == ePAGE_FIND_LIST)
					s_hListTabDlg = CreateDialog(s_hInst, "PAGE_FIND_LIST", hDlg,ProcPageNpcFindList);

				ShowWindow(s_hListTabDlg, SW_SHOW);
			}
			
			s_iSelectFindNpcData = 0xfffff;
			s_iSelOldList = SelList;

			Draw();
			break;
		}
		break;
		case WM_COMMAND:
			switch( LOWORD( wParam ) )
			{
			case IDC_EDIT_NPCSPEECH:
				EditSpeech(hDlg);
				break;
			case IDC_REMOVE_NPCSPEECH:
					RemoveSpeech(hDlg);
				break;
			case IDC_REMOVE_ALLSPEECH:
				RemoveAllSpeech(hDlg);
				break;
			case IDC_REMOVE_ALLNPC:
				RemoveAllNpc(hDlg);
				break;
			case IDC_REMOVE_NPC	:
				RemmoveNpc(hDlg);
				break;
			case IDC_EDIT_ABILITY:
				EditNpcInfo(hDlg);
				break;
			case IDC_CLOSE_NPCEDIT:
				s_bEnableDialog = FALSE;
				ShowWindow(hDlg, SW_HIDE);
				break;
			}
			break;
	case WM_CLOSE:
		s_bEnableDialog = FALSE;
		ShowWindow(hDlg, SW_HIDE);
		break;
	case WM_DESTROY:
		EndDialog(hDlg, IDOK);
		break;
	}
	
	return (0L);
}

//
// NPC LIST 컨트롤
LRESULT CALLBACK
cSetDialogNpcData::ProcNpcListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			SetFocus(hwnd);			// 09.12.17 추가
			s_iSelectNpcData		= s_listNpc.GetSelectData();
						
			cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
			cACTOR *lpActor			= lpActorSet->getActor(s_iSelectNpcData);
						
			if (!lpActor)	break;
			
			lpActorSet->freeSelect();
			lpActor->m_isSelected	= TRUE;
			
			int x = (int)((lpActor->getXpos() / dTILE_WIDTH)  - (g_MAPWND.TPW / 2));
			int y = (int)((lpActor->getYpos() / dTILE_HEIGHT) - (g_MAPWND.TPH / 2));
			
			g_lpActmap->setPos( x+2,y);
						
			g_MAPWND.Draw();
			DrawPageList(ePAGE_LIST);
			Draw();
			break;
		}
	case WM_LBUTTONDBLCLK:
		EditNpcInfo(hwnd);			
		break;
	case WM_MOUSEWHEEL :
		{
			int zDelta = ((short)HIWORD(wParam));
			
			if	(zDelta>=0)
				s_listNpc.VScroll(SB_LINEUP);
			else
				s_listNpc.VScroll(SB_LINEDOWN);
			
			DrawPageList(ePAGE_LIST);
			Draw();
			break;
		}
	}
	return (0L);
}

//
// NPC FIND LIST 
LRESULT CALLBACK
cSetDialogNpcData::ProcNpcFindListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{

			SetFocus(hwnd);			// 09.12.17 추가
			s_iSelectFindNpcData	= s_FindNpc.GetSelectData();
			
			cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
			cACTOR *lpActor			= lpActorSet->getActor(s_iSelectFindNpcData);
			
			if (!lpActor)	break;
						
			lpActorSet->freeSelect();
			lpActor->m_isSelected	= TRUE;
			
			int x = (int)((lpActor->getXpos() / dTILE_WIDTH)  - (g_MAPWND.TPW / 2));
			int y = (int)((lpActor->getYpos() / dTILE_HEIGHT) - (g_MAPWND.TPH / 2));
			
			g_lpActmap->setPos( x+2,y);
			
			g_MAPWND.Draw();
			DrawPageList(ePAGE_FIND_LIST);
			Draw();
			break;
		}
	case WM_LBUTTONDBLCLK:
			EditNpcInfo(hwnd);			
		break;
	case WM_MOUSEWHEEL :
		{
			int zDelta = ((short)HIWORD(wParam));
			
			if	(zDelta>=0)
				s_FindNpc.VScroll(SB_LINEUP);
			else
				s_FindNpc.VScroll(SB_LINEDOWN);
			
			DrawPageList(ePAGE_LIST);
			Draw();
			break;
		}
	}
	return (0L);
}

//
// NPC 대화 
LRESULT CALLBACK 
cSetDialogNpcData::ProcNpcSpeechWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT		:
		{	PAINTSTRUCT ps;
		
		BeginPaint( hwnd, &ps );
		
		s_dibNpcSpeech.Active();
		s_dibNpcSpeech.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);
		
		EndPaint(hwnd, &ps);
		break;
		}
	case	WM_VSCROLL		:
		{
			if (s_sbNpcSpeech.VScroll(wParam))	
				DrawNpcSpeech();

			break;
		}
	case WM_LBUTTONDOWN:
		SelectSpeech(hwnd);
		break;
	default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}
	return (0L);
}

//  
//	NPC 정보
LRESULT CALLBACK 
cSetDialogNpcData::ProcNpcInfoWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibNpcInfo.Active();
			s_dibNpcInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}
		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}
	
	return (0L);
}

//
// 탭컨트롤 NPC LIST
BOOL CALLBACK
cSetDialogNpcData::ProcPageNpcList(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(s_hListTab, &prt);
		TabCtrl_AdjustRect(s_hListTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);

		s_hPageList = hDlg;
		InitSetDialogListBoxData(ePAGE_LIST);
		break;
	}
	return (0L);
}

//
// 탭컨트롤 NPC FIND LIST
BOOL CALLBACK
cSetDialogNpcData::ProcPageNpcFindList(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(s_hListTab, &prt);
		TabCtrl_AdjustRect(s_hListTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);

		s_hPageFindList = hDlg;
		InitSetDialogListBoxData(ePAGE_FIND_LIST);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_EDIT_FIND_LIST:
			switch(HIWORD(wParam))
			{
			case EN_CHANGE:
				ResetNpcListBox(ePAGE_FIND_LIST);
				break;
			}
			break;
		}
		break;
	}
	return (0L);
}