#include "CSetDialogMonster.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "cMAPWND.H"
#include <commctrl.h>
#include "cMAIN.H"
#include "cCHARACTER.H"
#include "cJOB.H"

HWND			CSetDialogMonster::s_hWnd					= NULL;
HWND			CSetDialogMonster::s_hSlider				= NULL;
HWND			CSetDialogMonster::s_hInfoTab				= NULL;
HWND			CSetDialogMonster::s_hListTab				= NULL;
HWND			CSetDialogMonster::s_hInfoTabDlg			= NULL;
HWND			CSetDialogMonster::s_hListTabDlg			= NULL;
HWND			CSetDialogMonster::s_hPageInfo				= NULL;
HWND			CSetDialogMonster::s_hPageEvent				= NULL;
HWND			CSetDialogMonster::s_hPageJob				= NULL;
HWND			CSetDialogMonster::s_hPageDeath				= NULL;
HWND			CSetDialogMonster::s_hPageList				= NULL;
HWND			CSetDialogMonster::s_hPageFindList			= NULL;

cLISTCONTROL	CSetDialogMonster::s_listMonster;
cLISTCONTROL	CSetDialogMonster::s_listFindMonster;

cDIBWND			CSetDialogMonster::s_dibMonsterBaseInfo;
cDIBWND			CSetDialogMonster::s_dibMonsterGeneralInfo;
cDIBWND			CSetDialogMonster::s_dibMonsterJob;
cDIBWND			CSetDialogMonster::s_dibMonsterDeath;
cDIBWND			CSetDialogMonster::s_hKarmaDetailWnd;

CKarmaContainer	CSetDialogMonster::s_aEvent[eMECC_COUNT];
cCHARACTER		*CSetDialogMonster::s_lpEditCharacter;
CKarma			*CSetDialogMonster::s_lpStoredKarma;
CKarma			*CSetDialogMonster::s_lpSelectKarma;
cSCROLL_BAR		CSetDialogMonster::s_sbMonsterKarma;

int				CSetDialogMonster::s_nMonsterCount			= 0;	
int				CSetDialogMonster::s_nFindMonsterCount		= 0;
int				CSetDialogMonster::s_iSelectMonsterData		= 0;
int				CSetDialogMonster::s_iSelectFindMonsterData	= 0;
int				CSetDialogMonster::s_iSpeechListWndPos		= 0;
int				CSetDialogMonster::s_iKarmaDetailWndPos		= 0;
int				CSetDialogMonster::s_iSelOldList			= 0;
int				CSetDialogMonster::s_iSelOldInfo			= 0;
int				CSetDialogMonster::s_iSelectCondition		= eMECC_KILLED_BY_PLAYER;
BOOL			CSetDialogMonster::s_bEnableDialog			= FALSE;

//
// 메뉴에서 실행. 첫 실행시 생성. 화면 지정
void					
CSetDialogMonster::Init(HWND hwnd)
{
	s_bEnableDialog = TRUE;
	
	if(s_hWnd != NULL)
		return;
	
	CreateDialog(s_hInst, "SET_MONSTER_DATA", hwnd, (DLGPROC)ProcSetDialog);

	RECT rt1, rt2;
	GetClientRect(g_MAPWND.hWND,&rt1);
	GetClientRect(s_hWnd, &rt2);
	
	SetWindowPos(s_hWnd, NULL, rt1.right - rt2.right + 100, 120, 0, 0, SWP_NOSIZE);
}

//
// 탭컨트롤 페이지 데이터 설정(LEFT 2 / RIGHT 4) 
void
CSetDialogMonster::InitPageData(HWND hwnd)
{
	// List Tab
	s_hListTab = CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, hwnd, (HMENU)0, s_hInst, NULL);

	HFONT hFont = CreateFont(11, 0, 0, 0, 0, 0, 0, 0,HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("Raised"));
	SendMessage(s_hListTab, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

	TCITEM tie;
	tie.mask = TCIF_TEXT;
	tie.pszText = "List";
	TabCtrl_InsertItem(s_hListTab, 0, &tie);
	tie.pszText = "Search";
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
	
	s_hListTabDlg = CreateDialog(s_hInst, "PAGE_LIST", hwnd,ProcPageMonsterList);
	MapDialogRect(s_hListTabDlg, &trt);
	DestroyWindow(s_hListTabDlg);
	
	TabCtrl_AdjustRect(s_hListTab, TRUE, &trt);
	
	OffsetRect(&trt, -trt.left, -trt.top);
	SetWindowPos(s_hListTab , NULL, 3, 10, 218, 800, SWP_NOZORDER);
		
	s_hListTabDlg  = CreateDialog(s_hInst, "PAGE_LIST", hwnd, ProcPageMonsterList);
	ShowWindow(s_hListTabDlg, SW_SHOW);
	
	// Info Tab
	s_hInfoTab = CreateWindow(WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, hwnd, (HMENU)0, s_hInst, NULL);
	SendMessage(s_hInfoTab, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

	tie.mask = TCIF_TEXT;
	tie.pszText = "Info";
	TabCtrl_InsertItem(s_hInfoTab, 0, &tie);
	tie.pszText = "Event";
	TabCtrl_InsertItem(s_hInfoTab, 1, &tie);
	tie.pszText = "Occupation data";
	TabCtrl_InsertItem(s_hInfoTab, 2, &tie);
	tie.pszText = "Death data";
	TabCtrl_InsertItem(s_hInfoTab, 3, &tie);

	hRc = FindResource(NULL, "PAGE_MONSTER_INFO", RT_DIALOG);
	hGlb = LoadResource(NULL, hRc);
	dTem = (DLGTEMPLATEEX*)LockResource(hGlb);
	trt.right = dTem->cx;
	trt.bottom = dTem->cy;
		
	hRc = FindResource(NULL, "PAGE_MONSTER_EVENT", RT_DIALOG);
	hGlb = LoadResource(NULL, hRc);
	dTem = (DLGTEMPLATEEX*)LockResource(hGlb);
	trt.right = max(trt.right, dTem->cx);
	trt.bottom =max(trt.bottom, dTem->cy);
		
	hRc = FindResource(NULL, "PAGE_MONSTER_JOB", RT_DIALOG);
	hGlb = LoadResource(NULL, hRc);
	dTem = (DLGTEMPLATEEX*)LockResource(hGlb);
	trt.right = max(trt.right, dTem->cx);
	trt.bottom =max(trt.bottom, dTem->cy);

	hRc = FindResource(NULL, "PAGE_MONSTER_DEATH", RT_DIALOG);
	hGlb = LoadResource(NULL, hRc);
	dTem = (DLGTEMPLATEEX*)LockResource(hGlb);
	trt.right = max(trt.right, dTem->cx);
	trt.bottom =max(trt.bottom, dTem->cy);

	s_hInfoTabDlg = CreateDialog(s_hInst, "PAGE_MONSTER_INFO", hwnd,ProcPageMonsterInfo);
	MapDialogRect(s_hInfoTabDlg, &trt);
	DestroyWindow(s_hInfoTabDlg);

	TabCtrl_AdjustRect(s_hInfoTab, TRUE, &trt);

	OffsetRect(&trt, -trt.left, -trt.top);
	SetWindowPos(s_hInfoTab , NULL, 230, 10, 440, 800, SWP_NOZORDER);

	s_hInfoTabDlg = CreateDialog(s_hInst, "PAGE_MONSTER_INFO", hwnd,ProcPageMonsterInfo);
	ShowWindow(s_hInfoTabDlg, SW_SHOW);
}

//
// 컨트롤 설정
void
CSetDialogMonster::InitSetDialogListBoxData(int sel)
{
	HWND	hList;
	
	switch(sel)
	{
	case ePAGE_LIST:
		hList = GetDlgItem(s_hPageList,IDC_LIST);
		s_listMonster.Init(hList, 2, FALSE, ProcMonsterListWnd);
		
		s_listMonster.AddColumn("No", 32);
		s_listMonster.AddColumn("Name", 6 * 36);
		break;
	case ePAGE_FIND_LIST:
		hList = GetDlgItem(s_hPageFindList, IDC_FIND_LIST);
		s_listFindMonster.Init(hList, 2, FALSE, ProcMonsterFindListWnd);

		s_listFindMonster.AddColumn("No", 32);
		s_listFindMonster.AddColumn("Name", 6 * 36);
		break;
	}
	
	ResetMonsterListBox(sel);
}

//
// 버튼 활성화 / 비활성화
void
CSetDialogMonster::EnableButton(BOOL bFlag)
{
	HWND hButton = NULL;

	if(bFlag)
	{
		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_ALLMONSTER);
		EnableWindow(hButton, TRUE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_MONSTER);
		EnableWindow(hButton, TRUE);

		hButton = GetDlgItem(s_hWnd , IDC_EDIT_BASEDATA);
		EnableWindow(hButton, TRUE);

		hButton = GetDlgItem(s_hWnd , IDC_EDIT_MONSTER_EVENT);
		EnableWindow(hButton, TRUE);
	}
	else
	{
		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_ALLMONSTER);
		EnableWindow(hButton, FALSE);

		hButton = GetDlgItem(s_hWnd , IDC_REMOVE_MONSTER);
		EnableWindow(hButton, FALSE);

		hButton = GetDlgItem(s_hWnd , IDC_EDIT_BASEDATA);
		EnableWindow(hButton, FALSE);

		hButton = GetDlgItem(s_hWnd , IDC_EDIT_MONSTER_EVENT);
		EnableWindow(hButton, FALSE);
	}
}

//
// 리스트 컨트롤(몬스터 리스트) 설정
void
CSetDialogMonster::ResetMonsterListBox(int sel)
{
	if(s_bEnableDialog == FALSE)
		return;

	if( sel == ePAGE_LIST )
	{
		cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;
		int i, iCount	= 0;
		s_nMonsterCount	= -1;
		
		s_listMonster.ResetItems();
		
		for (i=0;i<dMAX_ACTOR_COUNT;i++)
		{
			cACTOR *lpActor	=	lpActorSet->getActor(i);
			
			if (!lpActor					)		continue;
			if( lpActor->m_wType != eNPC_MONSTER)	continue;
			
			iCount++;
			s_nMonsterCount++;
			
			char	*lpText		=	lpActor->getName();
			
			s_listMonster.AddItem(0,_ms("%d",iCount),i);
			s_listMonster.AddItem(1,lpText);
			s_listMonster.IncreaseLineCounter();
		}
		
		if (!s_listMonster.selectByData(0,s_iSelectMonsterData))
		{
			if (!s_listMonster.Select(0))	s_iSelectMonsterData =	0xffff;
			else							s_iSelectMonsterData =	s_listMonster.GetData(0);
		}
		
		s_listMonster.setHeightPos(s_iSpeechListWndPos);

		if(s_nMonsterCount == -1)
			EnableButton(FALSE);
		else
			EnableButton(TRUE);
	}
	if( sel == ePAGE_FIND_LIST )
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
		s_nFindMonsterCount = -1;
		s_listFindMonster.ResetItems();
		
		for (i=0;i<dMAX_ACTOR_COUNT;i++)
		{
			cACTOR *lpActor	=	lpActorSet->getActor(i);
			
			if (!lpActor					)		continue;
			if( lpActor->m_wType != eNPC_MONSTER)	continue;
			
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
					s_nFindMonsterCount++;

					s_listFindMonster.AddItem(0,_ms("%d",iCount),i);
					s_listFindMonster.AddItem(1,lpText);
					s_listFindMonster.IncreaseLineCounter();
					break;
				}
			}			
		}
		
		if (!s_listFindMonster.selectByData(0,s_iSelectFindMonsterData))
		{
			if (!s_listFindMonster.Select(0))	s_iSelectFindMonsterData =	0xffff;
			else								s_iSelectFindMonsterData =	s_listFindMonster.GetData(0);
		}
	
		s_listFindMonster.setHeightPos(s_iSpeechListWndPos);

		if(s_nFindMonsterCount == -1)
			EnableButton(FALSE);
		else
			EnableButton(TRUE);
	}
	DrawPageList(sel);

}

//
// 몬스터 정보 
void
CSetDialogMonster::ResetMonsterInfoBox(int sel)
{
	if(s_bEnableDialog == FALSE)
		return;

	if(sel == ePAGE_MONSTER_EVENT)
	{
		SetEvent();		
	}

	DrawPageInfo(sel);
}

//
// 탭컨트롤 LEFT  몬스터 목록 출력
void
CSetDialogMonster::DrawPageList(int sel)
{
	switch(sel)
	{
	case ePAGE_LIST:	
		s_listMonster.Draw();			
		break;
	case ePAGE_FIND_LIST:	
		s_listFindMonster.Draw();		
		break;
	}
}

//
// 탭 컨트롤 RIGHT  몬스터 정보 출력
void
CSetDialogMonster::DrawPageInfo(int sel)
{
	switch(sel)
	{
	case ePAGE_MONSTER_INFO:	
		DrawPageMonsterBaseInfo();
		DrawPageMonsterGeneralInfo();
		break;
	case ePAGE_MONSTER_EVENT:	
		DrawMonsterKarma();				
		break;
	case ePAGE_MONSTER_JOB:		
		DrawPageMonsterJob();			
		break;
	case ePAGE_MONSTER_DEATH:	
		DrawPageMonsterDeath();			
		break;		
	}
}

//
// 몬스터 기본 정보 출력
void
CSetDialogMonster::DrawPageMonsterBaseInfo()
{
	if (!s_dibMonsterBaseInfo.Active()) 
		return;
	
	cDRAW::Fill(_DEEPGRAY);

	cTEXT::Put(5, 10, WHITE, "name: ");
	cTEXT::Put(5, 25, WHITE, "Category: ");
	cTEXT::Put(5, 40, WHITE, "Regen cycle: ");
	cTEXT::Put(5, 55, WHITE, "Action cycle: ");
	cTEXT::Put(5, 70, WHITE, "Direction of view: ");
	cTEXT::Put(5, 85, WHITE, "action area: ");
	cTEXT::Put(5, 100, WHITE, "Battle orientation: ");

	cTEXT::Put(5, 130, WHITE, "Created within action area: ");
	cTEXT::Put(5, 145, WHITE, "No automatic regeneration: ");
	cTEXT::Put(5, 160, WHITE, "Sometimes change direction: ");

	int i = 0, selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;
	
	cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
	cACTOR		*lpActor	= lpActorSet->getActor(selectlist);
	
	if (!lpActor){
		s_dibMonsterBaseInfo.Draw();
		return;	
	}

	if(s_nMonsterCount != -1)
	{

		cJOB	*lpJob = lpActor->getJob();
		cCHARACTER *lpCharacter = lpActor->getCharacter();
		
		LONGLONG	llMaxHP			=	lpJob->m_iMaxHP;
		int			iLevel	=	(lpCharacter->m_wMinLevel+lpCharacter->m_wMaxLevel)/2;
		int			iConstitution	=	(int)lpJob->m_sConstitution*((iLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		llMaxHP	=	lpJob->m_iMaxHP+iConstitution*lpJob->m_wCorrectMaxHPConstitutionFactor*10+iLevel*lpJob->m_wCorrectMaxHPFactor*10;
		llMaxHP	=	llMaxHP*lpCharacter->m_wCorrectHP/100;
		int iHP =	(int)llMaxHP/100;
		
		cTEXT::Put(80,10,WHITE, _ms("%s(%d,%d)", lpActor->getName(), (lpActor->getXpos() / dTILE_WIDTH), (lpActor->getYpos() / dTILE_HEIGHT)));
		cTEXT::Put(80,25,WHITE, g_strNpcCategorty[lpActor->m_wType]);
		cTEXT::Put(250,10,YELLOW, _ms("HP : %d", iHP));
				
		LONGLONG	llExp	=	lpJob->m_dwDeathExp;
		
		llExp		*=	(iLevel+4);
		llExp		*=	10;
		llExp		=	llExp*lpCharacter->m_wCorrectExp/100;
		int iExp	=	(int)llExp/100;
		iExp	=	GetCorrectExpByLevelGap(iExp,iLevel);
			
		cTEXT::Put(250,25,YELLOW, _ms("EP : %d", iExp));
		cTEXT::Put(80,40,WHITE, _ms("%d시간 %02d분 %02d초", lpActor->m_dwRegenerationCycle/60/60, (lpActor->m_dwRegenerationCycle/60)%60, lpActor->m_dwRegenerationCycle%60));
		cTEXT::Put(80,55,WHITE, _ms("%02d분 %02d초", lpActor->m_wWanderCycle/60, lpActor->m_wWanderCycle%60));
		cTEXT::Put(80,70,WHITE, g_str8Direct[lpActor->m_wDirect]);
		
		if(lpActor->m_wMoveArea == dMAX_AREA)
			cTEXT::Put(80,85,WHITE, "이동안함");
		else if(lpActor->m_wMoveArea < 0 || lpActor->m_wMoveArea > dMAX_AREA)
			cTEXT::Put(80,85,WHITE, " ");
		else
			cTEXT::Put(80,85,WHITE, g_lpActmap->m_pArea->GetName(lpActor->m_wMoveArea));
		
		cTEXT::Put(80,100,WHITE, g_strBattleInclination[lpActor->m_wBattleInclination]);
	
		if(lpActor->m_wGenerateMethod == dMONSTER_GENERATE_ACTIVE_AREA)
			cTEXT::Put(150,130,WHITE, "YES");	
		else
			cTEXT::Put(150,130,WHITE, "NO");	

		if(lpActor->m_wIsBlockToAutoRegen)
			cTEXT::Put(110,145,WHITE, "YES");	
		else
			cTEXT::Put(110,145,WHITE, "NO");	

		if(lpActor->m_isChangeDirect)
			cTEXT::Put(130,160,WHITE, "YES");	
		else
			cTEXT::Put(130,160,WHITE, "NO");	
	}

	s_dibMonsterBaseInfo.Draw();
}

//
// 몬스터 고유 정보 출력
// 몬스터 고유 정보 출력
void
CSetDialogMonster::DrawPageMonsterGeneralInfo()
{
	if (!s_dibMonsterGeneralInfo.Active()) 
		return;

	cDRAW::Fill(_BLACK);
	
	int ypos	= 10;
	int height	= 15;
		
	cTEXT::Put(5, ypos + height * 0, YELLOW, "Generate level: ");
	cTEXT::Put(5, ypos + height * 1, YELLOW, "Scale: ");
	cTEXT::Put(5, ypos + height * 2, YELLOW, "Skill usage frequency correction: ");
	cTEXT::Put(5, ypos + height * 3, YELLOW, "Final Damage Correction: ");
	cTEXT::Put(5, ypos + height * 4, YELLOW, "Health correction: ");
	cTEXT::Put(5, ypos + height * 5, YELLOW, "Experience Correction: ");
	cTEXT::Put(5, ypos + height * 6, YELLOW, "Attack Power Correction: ");

	cDRAW::Box(_LTYELLOW, 5, ypos + height * 8 - 5, 410, ypos + height * 15);


	cTEXT::Put(10, ypos + height * 8, YELLOW, "Physical trigger probability : "); cTEXT::Put(200, ypos + height * 8, YELLOW, "Absorption : ");
	cTEXT::Put(10, ypos + height * 9, YELLOW, " Fire Probability : "); cTEXT::Put(200, ypos + height * 9, YELLOW, "Absorption : ");
	cTEXT::Put(10, ypos + height * 10, YELLOW, "Wind trigger probability : "); cTEXT::Put(200, ypos + height * 10, YELLOW, "Absorption : ");
	cTEXT::Put(10, ypos + height * 11, YELLOW, " Light trigger probability : "); cTEXT::Put(200, ypos + height * 11, YELLOW, "Absorption : ");
	cTEXT::Put(10, ypos + height * 12, YELLOW, " Water trigger probability : "); cTEXT::Put(200, ypos + height * 12, YELLOW, "Absorption : ");
	cTEXT::Put(10, ypos + height * 13, YELLOW, "Earth trigger probability : "); cTEXT::Put(200, ypos + height * 13, YELLOW, "Absorption : ");
	cTEXT::Put(10, ypos + height * 14, YELLOW, "Darkness trigger probability : "); cTEXT::Put(200, ypos + height * 14, YELLOW, "Absorption : ");

	cDRAW::Box(_LTYELLOW, 5, ypos + height * 16 + 1, 15, ypos + height * 17 - 5); cTEXT::Put(20, ypos + height * 16, YELLOW, "Normalize intelligence formula");
	cDRAW::Box(_LTYELLOW, 5, ypos + height * 17 + 1, 15, ypos + height * 18 - 5); cTEXT::Put(20, ypos + height * 17, YELLOW, "Not trainable");
	cDRAW::Box(_LTYELLOW, 5, ypos + height * 18 + 1, 15, ypos + height * 19 - 5); cTEXT::Put(20, ypos + height * 18, YELLOW, "Don't give money");
	cDRAW::Box(_LTYELLOW, 5, ypos + height * 19 + 1, 15, ypos + height * 20 - 5); cTEXT::Put(20, ypos + height * 19, YELLOW, "No items");
	cDRAW::Box(_LTYELLOW, 5, ypos + height * 20 + 1, 15, ypos + height * 21 - 5); cTEXT::Put(20, ypos + height * 20, YELLOW, "Hide experience");
	cDRAW::Box(_LTYELLOW, 5, ypos + height * 21 + 1, 15, ypos + height * 22 - 5); cTEXT::Put(20, ypos + height * 21, YELLOW, "Invincible");
	cDRAW::Box(_LTYELLOW, 5, ypos + height * 22 + 1, 15, ypos + height * 23 - 5); cTEXT::Put(20, ypos + height * 22, YELLOW, "Tall Monster");
	cDRAW::Box(_LTYELLOW, 5, ypos + height * 23 + 1, 15, ypos + height * 24 - 5); cTEXT::Put(20, ypos + height * 23, YELLOW, "Set Boss");

	cDRAW::Box(_LTYELLOW, 185, ypos + height * 16 + 1, 195, ypos + height * 17 - 5); cTEXT::Put(200, ypos + height * 16, YELLOW, "Temptation immune");
	cDRAW::Box(_LTYELLOW, 185, ypos + height * 17 + 1, 195, ypos + height * 18 - 5); cTEXT::Put(200, ypos + height * 17, YELLOW, "Stiffness immunity");
	cDRAW::Box(_LTYELLOW, 185, ypos + height * 18 + 1, 195, ypos + height * 19 - 5); cTEXT::Put(200, ypos + height * 18, YELLOW, "Petrification immunity");
	cDRAW::Box(_LTYELLOW, 185, ypos + height * 19 + 1, 195, ypos + height * 20 - 5); cTEXT::Put(200, ypos + height * 19, YELLOW, "Heat immunity");
	cDRAW::Box(_LTYELLOW, 185, ypos + height * 20 + 1, 195, ypos + height * 21 - 5); cTEXT::Put(200, ypos + height * 20, YELLOW, "Knockback immune");
	cDRAW::Box(_LTYELLOW, 185, ypos + height * 21 + 1, 195, ypos + height * 22 - 5); cTEXT::Put(200, ypos + height * 21, YELLOW, "Magic immunity");
	cDRAW::Box(_LTYELLOW, 185, ypos + height * 22 + 1, 195, ypos + height * 23 - 5); cTEXT::Put(200, ypos + height * 22, YELLOW, "Physical Deduction");
	cDRAW::Box(_LTYELLOW, 185, ypos + height * 23 + 1, 195, ypos + height * 24 - 5); cTEXT::Put(200, ypos + height * 23, YELLOW, "Confusion Immunity");


	cTEXT::Put(5, ypos + height * 26, GREEN, "HP : ");
	cTEXT::Put(5, ypos + height * 27, GREEN, "EP: ");
	cTEXT::Put(5, ypos + height * 28, GREEN, "Agile: ");
	cTEXT::Put(5, ypos + height * 29, GREEN, "Luck: "); cTEXT::Put(250, ypos + height * 29, GREEN, "Concept: ");
	cTEXT::Put(5, ypos + height * 30, GREEN, "Min Attack Power: "); cTEXT::Put(250, ypos + height * 30, GREEN, "Skill Included: ");
	cTEXT::Put(5, ypos + height * 31, GREEN, "Max Attack Power: "); cTEXT::Put(250, ypos + height * 31, GREEN, "With skills: ");
	cTEXT::Put(5, ypos + height * 32, GREEN, "Base Armor : ");

	int i = 0, selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;
	
	cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
	cACTOR		*lpActor	= lpActorSet->getActor(selectlist);
	
	if (!lpActor){
		s_dibMonsterGeneralInfo.Draw();
		return;	
	}

	if(s_nMonsterCount != -1)
	{
		cCHARACTER *_lpEditCharacter = lpActor->getCharacter();

		cTEXT::Put(120, ypos + height * 0,YELLOW,_ms("%d ~ %d", _lpEditCharacter->m_wMinLevel, _lpEditCharacter->m_wMaxLevel));
		cTEXT::Put(120, ypos + height * 1,YELLOW,_ms("%d%% %d%%", _lpEditCharacter->m_wScaleX, _lpEditCharacter->m_wScaleY));
		cTEXT::Put(120, ypos + height * 2,YELLOW,_ms("%d%%", _lpEditCharacter->m_bf8CorrectUseSkillChance));
		cTEXT::Put(120, ypos + height * 3,YELLOW,_ms("%d / 10000", _lpEditCharacter->m_wCorrectFinalDamage));
		cTEXT::Put(120, ypos + height * 4,YELLOW,_ms("%d%%", _lpEditCharacter->m_wCorrectHP));
		cTEXT::Put(120, ypos + height * 5,YELLOW,_ms("%d%%", _lpEditCharacter->m_wCorrectExp));
		cTEXT::Put(120, ypos + height * 6,YELLOW,_ms("%d%%", _lpEditCharacter->m_wCorrectAP));
		
		cTEXT::Put(120, ypos + height * 8,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[0][0]));
		cTEXT::Put(120, ypos + height * 9,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[1][0]));
		cTEXT::Put(120, ypos + height *10,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[3][0]));
		cTEXT::Put(120, ypos + height *11,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[5][0]));
		cTEXT::Put(120, ypos + height *12,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[2][0]));
		cTEXT::Put(120, ypos + height *13,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[4][0]));
		cTEXT::Put(120, ypos + height *14,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[6][0]));
		
		cTEXT::Put(260, ypos + height * 8,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[0][1]));

		cTEXT::Put(260, ypos + height * 9,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[1][1]));
		cTEXT::Put(260, ypos + height *10,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[3][1]));
		cTEXT::Put(260, ypos + height *11,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[5][1]));
		cTEXT::Put(260, ypos + height *12,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[2][1]));
		cTEXT::Put(260, ypos + height *13,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[4][1]));
		cTEXT::Put(260, ypos + height *14,YELLOW,_ms("%d%%", _lpEditCharacter->m_asAbsorbMagicDamage[6][1]));
		
		if(_lpEditCharacter->m_bf1IsReviseIntelligence)
			cDRAW::Fill(_LTYELLOW, 5, ypos + height *16+1, 15, ypos + height *17-5);	

		if(_lpEditCharacter->m_bf1IsBlockToTame)
			cDRAW::Fill(_LTYELLOW, 5, ypos + height *17+1, 15, ypos + height *18-5);	

		if(_lpEditCharacter->m_bf1IsNotDropGold)
			cDRAW::Fill(_LTYELLOW, 5, ypos + height *18+1, 15, ypos + height *19-5);	

		if(_lpEditCharacter->m_bf1IsNotDropItem)
			cDRAW::Fill(_LTYELLOW, 5, ypos + height *19+1, 15, ypos + height *20-5);	

		if(_lpEditCharacter->m_bf1IsNotExp)
			cDRAW::Fill(_LTYELLOW, 5, ypos + height *20+1, 15, ypos + height *21-5);		
				
		if(_lpEditCharacter->m_bf1IsInvincible)
			cDRAW::Fill(_LTYELLOW, 5, ypos + height *21+1, 15, ypos + height *22-5);	

		if(_lpEditCharacter->m_bf1IsKeyMonster)
			cDRAW::Fill(_LTYELLOW, 5, ypos + height *22+1, 15, ypos + height *23-5);		

		if(_lpEditCharacter->m_bf1IsSetBoss)
			cDRAW::Fill(_LTYELLOW, 5, ypos + height *23+1, 15, ypos + height *24-5);		

		if(_lpEditCharacter->m_bf1IsImmuneCharming)
			cDRAW::Fill(_LTYELLOW, 185, ypos + height *16+1, 195, ypos + height *17-5);	

		if(_lpEditCharacter->m_bf1IsImmuneStun)
			cDRAW::Fill(_LTYELLOW, 185, ypos + height *17+1, 195, ypos + height *18-5);	

		if(_lpEditCharacter->m_bf1IsImmuneStone)
			cDRAW::Fill(_LTYELLOW, 185, ypos + height *18+1, 195, ypos + height *19-5);		

		if(_lpEditCharacter->m_bf1IsImmuneFreeze)
			cDRAW::Fill(_LTYELLOW, 185, ypos + height *19+1, 195, ypos + height *20-5);	

		if(_lpEditCharacter->m_bf1IsImmuneKnockBack)
			cDRAW::Fill(_LTYELLOW, 185, ypos + height *20+1, 195, ypos + height *21-5);
				
		if(_lpEditCharacter->m_bf1IsImmuneMagicDamage)
			cDRAW::Fill(_LTYELLOW, 185, ypos + height *21+1, 195, ypos + height *22-5);

		if(_lpEditCharacter->m_bf1IsImmunePhysicalDamage)
			cDRAW::Fill(_LTYELLOW, 185, ypos + height *22+1, 195, ypos + height *23-5);

		if(_lpEditCharacter->m_bf1IsImmuneConfuse)
			cDRAW::Fill(_LTYELLOW, 185, ypos + height *23+1, 195, ypos + height *24-5);

		// HP
		cJOB	*lpJob = lpActor->getJob();
		cCHARACTER *lpCharacter = lpActor->getCharacter();
		
		int			iMinLevel	=	lpCharacter->m_wMinLevel;
		int			iMaxLevel	=	lpCharacter->m_wMaxLevel;	

		int			iMinConstitution	=	(int)lpJob->m_sConstitution*((iMinLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;
		int			iMaxConstitution	=	(int)lpJob->m_sConstitution*((iMaxLevel-1)*(int)lpJob->m_wBasicPowerLevelFactor+100000)/100000;

		LONGLONG	llMinLevelHP = lpJob->m_iMaxHP;
		LONGLONG	llMaxLevelHP = lpJob->m_iMaxHP;
		llMinLevelHP = lpJob->m_iMaxHP+iMinConstitution*lpJob->m_wCorrectMaxHPConstitutionFactor*10+iMinLevel*lpJob->m_wCorrectMaxHPFactor*10;
		llMaxLevelHP = lpJob->m_iMaxHP+iMaxConstitution*lpJob->m_wCorrectMaxHPConstitutionFactor*10+iMaxLevel*lpJob->m_wCorrectMaxHPFactor*10;

		llMinLevelHP =	llMinLevelHP*lpCharacter->m_wCorrectHP/100;
		llMaxLevelHP =	llMaxLevelHP*lpCharacter->m_wCorrectHP/100;

		int iMinHP =	(int)llMinLevelHP/100;
		int iMaxHP =	(int)llMaxLevelHP/100;

		cTEXT::Put(90, ypos + height *26,GREEN, _ms("%d ~ %d", iMinHP, iMaxHP));
		
		// EP
		LONGLONG	llMinExp	=	lpJob->m_dwDeathExp;
		LONGLONG	llMaxExp	=	lpJob->m_dwDeathExp;

		llMinExp *= (iMinLevel+4);
		llMaxExp *= (iMaxLevel+4);

		llMinExp *=	10;
		llMaxExp *=	10;

		llMinExp = llMinExp*lpCharacter->m_wCorrectExp/100;
		llMaxExp = llMaxExp*lpCharacter->m_wCorrectExp/100;

		int iMinExp = (int)llMinExp/100;		
		int iMaxExp = (int)llMaxExp/100;		

		iMinExp = GetCorrectExpByLevelGap(iMinExp,iMinLevel);
		iMaxExp = GetCorrectExpByLevelGap(iMaxExp,iMaxLevel);
			
		cTEXT::Put(90, ypos + height *27,GREEN, _ms("%d ~ %d", iMinExp, iMaxExp));
		
		short MinDamage = lpJob->m_sMinDamage;						// C65
		WORD MinDamageFactor = lpJob->m_wMinDamageLevelFactor;		// C66
		short Strength	= lpJob->m_sStrength;						// C69
		WORD StrengthFactor = 	lpJob->m_wBasicPowerLevelFactor;	// C70
				
		// ATTACK
		double C59	= lpCharacter->m_wMinLevel;
		double C592	= lpCharacter->m_wMaxLevel;
		double B59	= lpCharacter->m_wMaxLevel;

		double C65	= lpJob->m_sMinDamage;
		double C66	= lpJob->m_wMinDamageLevelFactor * 0.01;
		double C69	= lpJob->m_sStrength;
		double C70	= lpJob->m_wBasicPowerLevelFactor * 0.001;
		
		double result1 = ((((C65*100)+(C59-1)*(C66*100))+((C65*100)+(C59-1)*(C66*100))*C69*((C59-1)*(C70*1000)+100000)/100000/200)/100);
		double result2 = ((((C65*100)+(B59-1)*(C66*100))+((C65*100)+(B59-1)*(C66*100))*C69*((B59-1)*(C70*1000)+100000)/100000/200)/100);
		
		int Min = (int)(result1);
		int Max = (int)(result2);

		cTEXT::Put(90, ypos + height *30,GREEN, _ms("%d ~ %d", Min, Max));
				
		cABILITY	*lpAbility	=	(cABILITY*)&lpJob->m_aAbility[0];
		cSKILL		*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];

		double C72	= lpSkill->m_sAttackPercentage;
		double C73	= lpSkill->m_sAttackPercentagePerLevel;
		double C75	= lpJob->m_aLevelForAddSkillLevel[0] * 0.01;
		double C74	= (int)((10000+C72+(C73*(C59 /C75)))/100);
		double C742	= (int)((10000+C72+(C73*(C592/C75)))/100);
		
		result1	=((((C65*100)+(C59 -1)*(C66*100))+((C65*100)+(C59 -1)*(C66*100))*C69*((C59 -1)*(C70*1000)+100000)/100000/200)/100)*C74 /100;
		result2	=((((C65*100)+(C592-1)*(C66*100))+((C65*100)+(C592-1)*(C66*100))*C69*((C592-1)*(C70*1000)+100000)/100000/200)/100)*C742/100;	

		Min = (int)(result1);
		Max = (int)(result2);
		cTEXT::Put(320, ypos + height *30,GREEN, _ms("%d ~ %d", Min, Max));
		
		double C67 = lpJob->m_sMaxDamage;
		double C68 = lpJob->m_wMaxDamageLevelFactor * 0.01;
		
		result1 = ((((C67*100)+(C59-1)*(C68*100))+((C67*100)+(C59-1)*(C68*100))*C69*((C59-1)*(C70*1000)+100000)/100000/200)/100);
		result2 = ((((C67*100)+(C592-1)*(C68*100))+((C67*100)+(C592-1)*(C68*100))*C69*((C592-1)*(C70*1000)+100000)/100000/200)/100);

		Min = (int)(result1);
		Max = (int)(result2);
		cTEXT::Put(90, ypos + height *31,GREEN, _ms("%d ~ %d", Min, Max));
		
		result1	=((((C67*100)+(C59 -1)*(C68*100))+((C67*100)+(C59 -1)*(C68*100))*C69*((C59 -1)*(C70*1000)+100000)/100000/200)/100)*C74  /100;
		result2	=((((C67*100)+(C592-1)*(C68*100))+((C67*100)+(C592-1)*(C68*100))*C69*((C592-1)*(C70*1000)+100000)/100000/200)/100)*C742 /100;

		Min = (int)(result1);
		Max = (int)(result2);
		cTEXT::Put(320, ypos + height *31,GREEN, _ms("%d ~ %d", Min, Max));

		// 기본방어력
		double E59	= iMinLevel;
		double E592	= iMaxLevel;
		double E65	= lpJob->m_sDefensivePower;
		double E66	= lpJob->m_wDefensePowerLevelFactor * 0.01;
		double E67	= lpJob->m_sConstitution;
		double E68	= C70;

		result1 = ((E65*100)+(E59-1)*(E66*100))*(100+(E67*((E59-1)*(E68*1000)+100000)/100000))/100/100;
		result2 = ((E65*100)+(E592-1)*(E66*100))*(100+(E67*((E592-1)*(E68*1000)+100000)/100000))/100/100;

		Min = (int)(result1);
		Max = (int)(result2);
		cTEXT::Put(90, ypos + height *32,GREEN, _ms("%d ~ %d", Min, Max));

		// 민첩
		double C85	= lpJob->m_sAgility;
		double E80	= iMinLevel;
		double E802	= iMaxLevel;
		double E84	= C70;

		result1 = ((C85*((E80-1)*E84+100)/100));
		result2 = ((C85*((E802-1)*E84+100)/100));
		Min = (int)(result1);
		Max = (int)(result2);
		cTEXT::Put(90, ypos + height *28,GREEN, _ms("%d ~ %d", Min, Max));

		// 운
		double C86 = lpJob->m_sLuck;
		result1 = ((C86*((E80-1)*E84+100)/100));
		result2 = ((C86*((E802-1)*E84+100)/100));
		Min = (int)(result1);
		Max = (int)(result2);
		cTEXT::Put(90, ypos + height *29,GREEN, _ms("%d ~ %d", Min, Max));

		// 09.12.09 추가
		{
			char	strText[20];
			
			int iMagicGrade = 0, iPhysical = 0, iDefence = 0;
			iMagicGrade	= cSET_JOB::GetMonsterMagicConcept(lpJob);
			
			if(iMagicGrade == dGRADE_MONSTER_HARD)
			{
				// 09.12.14 수정
				cABILITY	*lpAbility	=	(cABILITY*)&lpJob->m_aAbility[0];
				cSKILL		*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];

				if(lpSkill->m_wDamageAttribute % 2 == 0)
					iPhysical = dGRADE_MONSTER_WEAK;
				else
					iPhysical = cSET_JOB::GetMonsterPhysicalAttackConcpet(lpJob);
			}
			else
				iPhysical = cSET_JOB::GetMonsterPhysicalAttackConcpet(lpJob);
			
			iDefence = cSET_JOB::GetMonsterDefenceConcept(lpJob);
			sprintf(strText, "%s.%s.%s", g_strMonsterConcept[iMagicGrade+1], g_strMonsterConcept[iPhysical+1], g_strMonsterConcept[iDefence+1]);
			cTEXT::Put(320, ypos + height *29,GREEN, strText);
		}


	}
		
	s_dibMonsterGeneralInfo.Draw();
}
//
// 직업 데이터 출력
void
CSetDialogMonster::DrawPageMonsterJob()
{
	if (!s_dibMonsterJob.Active()) 
		return;

	int i = 0, selectlist = 0;
	
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;

	cDRAW::Fill(_DEEPGRAY);

	int height = 15,	ypos  = 17;
	int lxpos  = 10,	rxpos = 210;

	cDRAW::Box(_WHITE, 5, 10, 413, ypos + height * 24 + 1);	
	cDRAW::Box(_WHITE, 5, ypos + height * 25-7, 413, ypos + height * 33 + 1);	
	cDRAW::Box(_WHITE, 5, ypos + height * 34-7, 413, ypos + height * 48 - 5);	

	cTEXT::Put(lxpos, ypos + height * 0,WHITE,"직업 이름    : ");		cTEXT::Put(225, ypos + height * 0,WHITE,"지능 적용 수식 교정");	
	cTEXT::Put(lxpos, ypos + height * 1,WHITE,"팔레트       : ");				
	cTEXT::Put(lxpos, ypos + height * 2,WHITE,"레벨         : ");		cTEXT::Put(rxpos, ypos + height * 2,WHITE,"시야(반지름) : ");
	cTEXT::Put(lxpos, ypos + height * 3,WHITE,"기본 체력    : ");		cTEXT::Put(rxpos, ypos + height * 3,WHITE,"기본 마력    : ");
	cTEXT::Put(lxpos, ypos + height * 4,WHITE,"체력 계산(LV): ");		cTEXT::Put(rxpos, ypos + height * 4,WHITE,"체력계산(CON): ");
	cTEXT::Put(lxpos, ypos + height * 5,WHITE,"종류         : ");		cTEXT::Put(rxpos, ypos + height * 5,WHITE,"성향         : ");
	cTEXT::Put(lxpos, ypos + height * 6,WHITE,"등급         : ");		cTEXT::Put(rxpos, ypos + height * 6,WHITE,"외관         : ");
	cTEXT::Put(lxpos, ypos + height * 8,WHITE,"전투 성향    : ");
	cTEXT::Put(lxpos, ypos + height * 9,WHITE,"조련을 위패 필요한 전생 횟수 : ");	

	cTEXT::Put(lxpos + 15, ypos + height * 10,WHITE,"진짜 보스");		
	cTEXT::Put(lxpos + 115, ypos + height *10,WHITE,"조련 가능");
	cTEXT::Put(lxpos + 200, ypos + height *10,WHITE,"조련 저항 : ");
	
	cDRAW::Box(_WHITE, rxpos, ypos + height *0+1, rxpos + 10, ypos + height *1-5);
	cDRAW::Box(_WHITE, lxpos, ypos + height *10+1, lxpos + 10, ypos + height *11-5);
	cDRAW::Box(_WHITE, lxpos + 100, ypos + height *10+1, lxpos + 110, ypos + height *11-5);

														
	cTEXT::Put(lxpos, ypos + height * 12,WHITE,"힘          : ");		cTEXT::Put(rxpos, ypos + height * 12,WHITE,"민첩성       : ");			
	cTEXT::Put(lxpos, ypos + height * 13,WHITE,"건강        : ");		cTEXT::Put(rxpos, ypos + height * 13,WHITE,"카리스마     : ");			
	cTEXT::Put(lxpos, ypos + height * 14,WHITE,"지식        : ");		cTEXT::Put(rxpos, ypos + height * 14,WHITE,"지혜         : ");			
	cTEXT::Put(lxpos, ypos + height * 15,WHITE,"운          : ");		cTEXT::Put(rxpos, ypos + height * 15,WHITE,"레벨 팩터    : ");
	

	cTEXT::Put(lxpos, ypos + height * 17,WHITE,"최소 대미지 : ");		cTEXT::Put(rxpos, ypos + height * 17,WHITE,"레벨 팩터     : ");
	cTEXT::Put(lxpos, ypos + height * 18,WHITE,"최대 대미지 : ");		cTEXT::Put(rxpos, ypos + height * 18,WHITE,"레벨 팩터     : ");
	cTEXT::Put(lxpos, ypos + height * 19,WHITE,"기본 방어력 : ");		cTEXT::Put(rxpos, ypos + height * 19,WHITE,"레벨 팩터     : ");
	cTEXT::Put(lxpos, ypos + height * 20,WHITE,"이동 속도   : ");		cTEXT::Put(rxpos, ypos + height * 20,WHITE,"1회 공격 시간 : ");
	cTEXT::Put(lxpos, ypos + height * 21,WHITE,"크리티컬샷  : ");		cTEXT::Put(rxpos, ypos + height * 21,WHITE,"크러쉬 블로우 : ");
	cTEXT::Put(lxpos, ypos + height * 22,WHITE,"명중 보정   : ");		cTEXT::Put(rxpos, ypos + height * 22,WHITE,"회피 보정     : ");
	cTEXT::Put(lxpos, ypos + height * 23,WHITE,"블러킹 확률 : ");

	cTEXT::Put(lxpos, ypos + height * 25,WHITE,"저주 계열 :");	cTEXT::Put(lxpos+136, ypos + height * 25,WHITE,"상태 이상 :");	cTEXT::Put(lxpos+269, ypos + height * 25,WHITE,"능력치저하 :");
	cTEXT::Put(lxpos, ypos + height * 26,WHITE,"장님      :");	cTEXT::Put(lxpos+136, ypos + height * 26,WHITE,"중독      :");	cTEXT::Put(lxpos+269, ypos + height * 26,WHITE,"잠         :");
	cTEXT::Put(lxpos, ypos + height * 27,WHITE,"콜드      :");	cTEXT::Put(lxpos+136, ypos + height * 27,WHITE,"프리즈    :");	cTEXT::Put(lxpos+269, ypos + height * 27,WHITE,"경직       :");
	cTEXT::Put(lxpos, ypos + height * 28,WHITE,"석화      :");	cTEXT::Put(lxpos+136, ypos + height * 28,WHITE,"혼란      :");	cTEXT::Put(lxpos+269, ypos + height * 28,WHITE,"차밍       :");	
	cTEXT::Put(lxpos, ypos + height * 29,WHITE,"치명타    :");	cTEXT::Put(lxpos+136, ypos + height * 29,WHITE,"결정타    :");


	cTEXT::Put(lxpos, ypos + height * 31,WHITE,"불꽃      :");	cTEXT::Put(lxpos+136, ypos + height * 31,WHITE,"물        :");	cTEXT::Put(lxpos+269, ypos + height * 31,WHITE,"바람       :");
	cTEXT::Put(lxpos, ypos + height * 32,WHITE,"빛        :");	cTEXT::Put(lxpos+136, ypos + height * 32,WHITE,"대지      :");	cTEXT::Put(lxpos+269, ypos + height * 32,WHITE,"어둠       :");
	
	cTEXT::Put(lxpos, ypos + height * 34,WHITE,"스킬/마법");

	cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
	cACTOR		*lpActor	= lpActorSet->getActor(selectlist);
	
	if (!lpActor) {
		s_dibMonsterJob.Draw();
		return;	
	}

	cJOB	*_lpEditJob = lpActor->getJob();

	if(s_nMonsterCount != -1)
	{
		cTEXT::Put(lxpos + 90, ypos + height * 0,WHITE, _lpEditJob->m_strName); 
		cTEXT::Put(lxpos + 90, ypos + height * 1,WHITE, _ms("%02d", _lpEditJob->m_wPalette)); 
		cTEXT::Put(lxpos + 90, ypos + height * 2,WHITE, _ms("%d ~ %d", _lpEditJob->m_iLevel, _lpEditJob->m_wLimitLevel));
		cTEXT::Put(rxpos + 90, ypos + height * 2,WHITE, _ms("%d Pixel", _lpEditJob->m_sSight));	
		cTEXT::Put(lxpos + 90, ypos + height * 3,WHITE, _ms("%d", _lpEditJob->m_iMaxHP/100));
		cTEXT::Put(rxpos + 90, ypos + height * 3,WHITE, _ms("%d", _lpEditJob->m_iMaxCP/100));
		cTEXT::Put(lxpos + 90, ypos + height * 4,WHITE, _ms("%d.%.1d",_lpEditJob->m_wCorrectMaxHPFactor/10,_lpEditJob->m_wCorrectMaxHPFactor%10));
		cTEXT::Put(rxpos + 90, ypos + height * 4,WHITE, _ms("%d.%.1d",_lpEditJob->m_wCorrectMaxHPConstitutionFactor/10,_lpEditJob->m_wCorrectMaxHPConstitutionFactor%10));
		cTEXT::Put(lxpos + 90, ypos + height * 5,WHITE, g_strJobCategory[_lpEditJob->m_wKind]);
		cTEXT::Put(rxpos + 90, ypos + height * 5,WHITE, g_strAllignment[_lpEditJob->m_wAllignment]);
		cTEXT::Put(lxpos + 90, ypos + height * 6,WHITE, g_strMonsterGrade[_lpEditJob->m_wMonsterLevel]);
		cTEXT::Put(rxpos + 90, ypos + height * 6, WHITE, _ms("%d step", _lpEditJob->m_wVariation));

		cTEXT::Put(lxpos + 90, ypos + height * 8, WHITE, g_strBattleInclination[_lpEditJob->m_wBattleInclination]);
		cTEXT::Put(200, ypos + height * 9, WHITE, _ms("%d times", _lpEditJob->m_bf3RequireRebirthCountForTame));
		cTEXT::Put(lxpos + 270, ypos + height * 10, WHITE, _ms("%d", _lpEditJob->m_bf15TameResistanceValue));
		cTEXT::Put(lxpos + 90, ypos + height * 12, WHITE, _ms("%d", _lpEditJob->m_sStrength));
		cTEXT::Put(lxpos + 90, ypos + height * 13, WHITE, _ms("%d", _lpEditJob->m_sConstitution));
		cTEXT::Put(lxpos + 90, ypos + height * 14, WHITE, _ms("%d", _lpEditJob->m_sIntelligence));
		cTEXT::Put(lxpos + 90, ypos + height * 15, WHITE, _ms("%d", _lpEditJob->m_sLuck));

		cTEXT::Put(rxpos + 90, ypos + height * 12, WHITE, _ms("%d", _lpEditJob->m_sAgility));
		cTEXT::Put(rxpos + 90, ypos + height * 13, WHITE, _ms("%d", _lpEditJob->m_sCharisma));
		cTEXT::Put(rxpos + 90, ypos + height * 14, WHITE, _ms("%d", _lpEditJob->m_sWisdom));
		cTEXT::Put(rxpos + 90, ypos + height * 15, WHITE, _ms("%d.%.3d %%", _lpEditJob->m_wBasicPowerLevelFactor / 1000, _lpEditJob->m_wBasicPowerLevelFactor % 1000));

		cTEXT::Put(lxpos + 90, ypos + height * 17, WHITE, _ms("%d", _lpEditJob->m_sMinDamage));
		cTEXT::Put(rxpos + 90, ypos + height * 17, WHITE, _ms("%d.%.2d", _lpEditJob->m_wMinDamageLevelFactor / 100, _lpEditJob->m_wMinDamageLevelFactor % 100));
		cTEXT::Put(lxpos + 90, ypos + height * 18, WHITE, _ms("%d", _lpEditJob->m_sMaxDamage));
		cTEXT::Put(rxpos + 90, ypos + height * 18, WHITE, _ms("%d.%.2d", _lpEditJob->m_wMaxDamageLevelFactor / 100, _lpEditJob->m_wMaxDamageLevelFactor % 100));
		cTEXT::Put(lxpos + 90, ypos + height * 19, WHITE, _ms("%d", _lpEditJob->m_sDefensivePower));
		cTEXT::Put(rxpos + 90, ypos + height * 19, WHITE, _ms("%d.%.2d", _lpEditJob->m_wDefensePowerLevelFactor / 100, _lpEditJob->m_wDefensePowerLevelFactor % 100));
		cTEXT::Put(lxpos + 90, ypos + height * 20, WHITE, _ms("%d Pixel", _lpEditJob->m_wMoveSpeed));
		cTEXT::Put(rxpos + 90, ypos + height * 20, WHITE, _ms("%d.%.2d seconds", _lpEditJob->m_wAttackSpeed / 100, _lpEditJob->m_wAttackSpeed % 100));
		cTEXT::Put(lxpos + 90, ypos + height * 21,WHITE,_ms("%d %%",_lpEditJob->m_wCriticalShot));
		cTEXT::Put(rxpos + 90, ypos + height * 21,WHITE,_ms("%d %%",_lpEditJob->m_wCrushBlow));

		if (_lpEditJob->m_sCorrectHit < 0)
			cTEXT::Put(rxpos + 90, ypos + height * 22,WHITE,_ms("-%d.%.2d",-_lpEditJob->m_sCorrectHit/100,-_lpEditJob->m_sCorrectHit%100));
		else
			cTEXT::Put(rxpos + 90, ypos + height * 22,WHITE,_ms("%d.%.2d",_lpEditJob->m_sCorrectHit/100,_lpEditJob->m_sCorrectHit%100));
		
		if (_lpEditJob->m_sCorrectDodge<0)	
			cTEXT::Put(rxpos + 90, ypos + height * 22,WHITE,_ms("-%d.%.2d %%",-_lpEditJob->m_sCorrectDodge/100,-_lpEditJob->m_sCorrectDodge%100));
		else
			cTEXT::Put(rxpos + 90, ypos + height * 22,WHITE,_ms("%d.%.2d %%",_lpEditJob->m_sCorrectDodge/100,_lpEditJob->m_sCorrectDodge%100));
		

		cTEXT::Put(lxpos + 90, ypos + height * 23,WHITE,_ms("%d %%",_lpEditJob->m_wBlocking));	

		cTEXT::Put(lxpos+70 , ypos + height * 25,WHITE,_ms("%03d %%",_lpEditJob->m_sCurseResistance));
		cTEXT::Put(lxpos+206, ypos + height * 25,WHITE,_ms("%d %%",_lpEditJob->m_sBadStatusResistance));
		cTEXT::Put(lxpos+344, ypos + height * 25,WHITE,_ms("%d %%",_lpEditJob->m_sDeclinePowerResistance));
		cTEXT::Put(lxpos+70 , ypos + height * 26,WHITE,_ms("%d %%",_lpEditJob->m_sBlindResistance));
		cTEXT::Put(lxpos+206, ypos + height * 26,WHITE,_ms("%d %%",_lpEditJob->m_sPoisonResistance));		
		cTEXT::Put(lxpos+344, ypos + height * 26,WHITE,_ms("%d %%",_lpEditJob->m_sSleepResistance));
		
		cTEXT::Put(lxpos+70 , ypos + height * 27,WHITE,_ms("%d %%",_lpEditJob->m_sColdResistance));
		cTEXT::Put(lxpos+206, ypos + height * 27,WHITE,_ms("%d %%",_lpEditJob->m_sFreezeResistance));
		cTEXT::Put(lxpos+344, ypos + height * 27,WHITE,_ms("%d %%",_lpEditJob->m_sStunResistance));
		cTEXT::Put(lxpos+70 , ypos + height * 28,WHITE,_ms("%d %%",_lpEditJob->m_sStoneResistance));
		cTEXT::Put(lxpos+206, ypos + height * 28,WHITE,_ms("%d %%",_lpEditJob->m_sConfuseResistance));
		cTEXT::Put(lxpos+344, ypos + height * 28,WHITE,_ms("%d %%",_lpEditJob->m_sCharmingResistance));	
		cTEXT::Put(lxpos+70 , ypos + height * 29,WHITE,_ms("%d %%",_lpEditJob->m_sCriticalResistance));
		cTEXT::Put(lxpos+206, ypos + height * 29,WHITE,_ms("%d %%",_lpEditJob->m_sCrushResistance));	
		
		cTEXT::Put(lxpos+70 , ypos + height * 31,WHITE,_ms("%d %%",_lpEditJob->m_sFireResistance));
		cTEXT::Put(lxpos+206, ypos + height * 31,WHITE,_ms("%d %%",_lpEditJob->m_sWaterResistance));
		cTEXT::Put(lxpos+344, ypos + height * 31,WHITE,_ms("%d %%",_lpEditJob->m_sWindResistance));
		cTEXT::Put(lxpos+70 , ypos + height * 32,WHITE,_ms("%d %%",_lpEditJob->m_sLightResistance));
		cTEXT::Put(lxpos+206, ypos + height * 32,WHITE,_ms("%d %%",_lpEditJob->m_sEarthResistance));
		cTEXT::Put(lxpos+344, ypos + height * 32,WHITE,_ms("%d %%",_lpEditJob->m_sDarkResistance));
					
		int icount = 0;

		cJOB	*lpJob = lpActor->getJob();
		cCHARACTER *lpCharacter = lpActor->getCharacter();

		double C592	= lpCharacter->m_wMaxLevel;
		double C65	= lpJob->m_sMinDamage;
		double C59	= lpCharacter->m_wMinLevel;
		double C66	= lpJob->m_wMinDamageLevelFactor * 0.01;
		double C69	= lpJob->m_sStrength;

		double C75	= lpJob->m_aLevelForAddSkillLevel[0] * 0.01;
		

		double C70	= lpJob->m_wBasicPowerLevelFactor * 0.001;		
			

		for(int i = 0; i < dMONSTER_ABILITY_COUNT; ++i)
		{
			cABILITY	*lpAbility	=	(cABILITY*)&_lpEditJob->m_aAbility[i];
			if	(lpAbility->m_wSkill	==	0xffff)
				continue;

			cSKILL	*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];

			cTEXT::Put(lxpos, ypos + height * (36 + icount),WHITE,lpSkill->m_strName);

			double C72	= lpSkill->m_sAttackPercentage;
			double C73	= lpSkill->m_sAttackPercentagePerLevel;
			double C74	= (int)((10000+C72+(C73*(C59 /C75)))/100);
			double C742	= (int)((10000+C72+(C73*(C592/C75)))/100);

			double result1	=((((C65*100)+(C59 -1)*(C66*100))+((C65*100)+(C59 -1)*(C66*100))*C69*((C59 -1)*(C70*1000)+100000)/100000/200)/100)*C74 /100;
			double result2	=((((C65*100)+(C592-1)*(C66*100))+((C65*100)+(C592-1)*(C66*100))*C69*((C592-1)*(C70*1000)+100000)/100000/200)/100)*C742/100;	
			
			int Min = (int)(result1*lpCharacter->m_wCorrectAP/100);
			int Max = (int)(result2*lpCharacter->m_wCorrectAP/100);

			cTEXT::Put(lxpos + 200, ypos + height * (36 + icount),WHITE,_ms("%d+Lv/%d.%.2d, 보정%d%% %d ~ %d", lpAbility->m_sLevel,_lpEditJob->m_aLevelForAddSkillLevel[i]/100,_lpEditJob->m_aLevelForAddSkillLevel[i]%100, lpCharacter->m_wCorrectAP, Min, Max));


			icount++;
		}
		
		if(_lpEditJob->m_bf1IsReviseIntCalc)                                                                                                         
			cDRAW::Fill(_WHITE, 200, ypos + height *0+1, 210, ypos + height *1-5);
		
		if(_lpEditJob->m_bf1IsZinBoss)
			cDRAW::Fill(_WHITE, lxpos, ypos + height *10+1, lxpos + 10, ypos + height *11-5);
		
		if(_lpEditJob->m_bf1IsEnableTameMonster)
			cDRAW::Fill(_WHITE, lxpos + 100, ypos + height *10+1, lxpos + 110, ypos + height *11-5);	
	}

	s_dibMonsterJob.Draw();
}

//
// 사망 데이터 출력
void
CSetDialogMonster::DrawPageMonsterDeath()
{
	if (!s_dibMonsterDeath.Active()) 
		return;

	cDRAW::Fill(_DEEPGRAY);

	int height = 15,	ypos  = 17;
	int lxpos  = 10,	rxpos = 210;

	cDRAW::Box(_WHITE, 5, 10, 413, ypos + height * 14 + 1);	
	cTEXT::Put(lxpos, ypos + height * 0,WHITE, "경험치 레벨  x ");	
	cTEXT::Put(lxpos, ypos + height * 2,WHITE, "골드  ");			cTEXT::Put(lxpos + 120, ypos + height * 2,WHITE, "+ 레벨 x ");	
	cTEXT::Put(lxpos, ypos + height * 4,WHITE, "골드 드랍 확률 ");	
	cTEXT::Put(lxpos, ypos + height * 6,WHITE, "드랍 아이템 정보");	
		
	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;
	
	cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
	cACTOR		*lpActor	= lpActorSet->getActor(selectlist);
	
	if (!lpActor){
		s_dibMonsterDeath.Draw();
		return;	
	}

	cJOB	*_lpEditJob = lpActor->getJob();
	
	if(s_nMonsterCount != -1)
	{
		cTEXT::Put(lxpos + 100, ypos + height * 0,YELLOW, _ms("%d.%.2d",_lpEditJob->m_dwDeathExp/100,_lpEditJob->m_dwDeathExp%100));
		cTEXT::Put(lxpos + 35 , ypos + height * 2,YELLOW, _ms("%d",_lpEditJob->m_wGold));
		cTEXT::Put(lxpos + 180, ypos + height * 2,YELLOW, _ms("%d.%.2d",_lpEditJob->m_wGoldPerLevel/100,_lpEditJob->m_wGoldPerLevel%100));	
		cTEXT::Put(lxpos + 100, ypos + height * 4,YELLOW, _ms("%d",_lpEditJob->m_wMoneyDroppingRate));
		
		for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
		{
			int	iItemType	=	_lpEditJob->m_aDroppingItem[i].m_wItemType;
			
			if (iItemType	!=	0xffff)
			{
				int	iValue	=	_lpEditJob->m_aDroppingItem[i].m_dwDroppingRate;

				cTEXT::Put(lxpos      , ypos + height * (7 + i),YELLOW, _ms(g_strItemCategory[iItemType]));	
				cTEXT::Put(lxpos + 100, ypos + height * (7 + i),YELLOW, _ms("%d.%.2d％",iValue/100,iValue%100));	
				cTEXT::Put(lxpos + 160, ypos + height * (7 + i),YELLOW, _ms("%d EA",_lpEditJob->m_aDroppingItem[i].m_bf8Count));	
			}
		}
	}

	s_dibMonsterDeath.Draw();
}

//
// 이벤트 설정
void
CSetDialogMonster::SetEvent()
{
	HWND	hEventConditionList	=	GetDlgItem(s_hPageEvent, IDC_EVENT_CONDITION_MONSTERLIST);

	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;

	cACTOR_SET *lpActorSet = &g_lpActmap->m_setActor;
	cACTOR *lpActor   = lpActorSet->getActor(selectlist);
     
	if (!lpActor) 
		return;

	cCHARACTER *_lpEditCharacter = lpActor->getCharacter();

	cKARMA_WND::s_bIsFieldKarma	=	FALSE;
	cCOMBOBOX::SelectByData(hEventConditionList,s_iSelectCondition);
	
	g_lpAKC	=	_lpEditCharacter->m_pEvent[s_iSelectCondition];
	
	InitSetCharacterEventData();
}

// 
// 이벤트 출력
void
CSetDialogMonster::InitSetCharacterEventData()
{

	HWND	hEventConditionList	=	GetDlgItem(s_hPageEvent, IDC_EVENT_CONDITION_MONSTERLIST);
	cCOMBOBOX::Reset(hEventConditionList);

	int	iIndex;
	for (iIndex=0;;iIndex++)
	{
		if (stricmp(g_strMonsterEventCheckCondition[iIndex],"end")==0)	break;

		cCOMBOBOX::Add(hEventConditionList,g_strMonsterEventCheckCondition[iIndex],iIndex);
	}

	cCOMBOBOX::SelectByData(hEventConditionList,s_iSelectCondition);
}

//
// 이벤트 콤보박스
void
CSetDialogMonster::SelectCondition()
{
	HWND	hKarmaGroup	=	GetDlgItem(s_hPageEvent,IDC_EVENT_CONDITION_MONSTERLIST);
	
	int	iSelectGroup	=	cCOMBOBOX::GetData(hKarmaGroup);
	if	(iSelectGroup	==	s_iSelectCondition)
		return;

	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;

	cACTOR_SET *lpActorSet = &g_lpActmap->m_setActor;
	cACTOR *lpActor   = lpActorSet->getActor(selectlist);
     
	if (!lpActor) 
		return;

	cCHARACTER *_lpEditCharacter = lpActor->getCharacter();

	s_iSelectCondition	=	iSelectGroup;
	g_lpAKC	=	_lpEditCharacter->m_pEvent[s_iSelectCondition];

	DrawPageInfo(ePAGE_MONSTER_EVENT);
}

//
// 이벤트 선택
void
CSetDialogMonster::SelectKarma()
{
	if	(g_lpAKC == NULL || g_lpAKC->getKarmaCount()	<=	0)
		return;

	HWND hList	=	GetDlgItem(s_hPageEvent,IDC_KARMA_MONSTERLIST);

	int iSelect	=	SendMessage(hList,LB_GETCURSEL,0,0);

	if	(iSelect	==	LB_ERR	)
		return;

	iSelect		=	cLISTBOX::GetData(hList,iSelect);

	if	(iSelect	==	g_iSelectKarma	)
		return;

	g_lpAKC->selectKarma(iSelect);

	s_iKarmaDetailWndPos	=	0;
	s_lpSelectKarma			=	g_lpAKC->getSelectKarma();

	DrawKarmaDetail();
}

//
// 이벤트 목록 출력
void
CSetDialogMonster::DrawMonsterKarma()
{
	HWND	hKarmaList	=	GetDlgItem(s_hPageEvent,IDC_KARMA_MONSTERLIST);		//	카르마 리스트 창
	HWND	hKarmaDetail=	GetDlgItem(s_hPageEvent,IDC_KARMA_MONSTERVIEW);		//	카르마 디테일

	cLISTBOX::Reset(hKarmaList);

	if	(!hKarmaList	||	!hKarmaDetail)
		return;

	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;

	cACTOR_SET *lpActorSet = &g_lpActmap->m_setActor;
	cACTOR *lpActor   = lpActorSet->getActor(selectlist);
     
	if (!lpActor) {
		DrawKarmaDetail();
		return;
	}

	SendMessage( hKarmaList, LB_RESETCONTENT, 0,0);

	SCROLLINFO	si;
	si.cbSize	=	sizeof(SCROLLINFO);
	si.fMask	=	SIF_POS | SIF_RANGE | SIF_PAGE ;
	si.nMin		=	0;
	si.nMax		=	3;
	si.nPage	=	3;
	si.nPos		=	0;
	EnableScrollBar	(hKarmaDetail,SB_VERT,ESB_ENABLE_BOTH);
	SetScrollInfo	(hKarmaDetail,SB_VERT,&si,TRUE);

	s_hKarmaDetailWnd.lpszRegister	=	"MONSTERKARMA_DETAIL_WINDOW";
	s_hKarmaDetailWnd.Init(hKarmaDetail,(WNDPROC)ProcMonsterKarmaDetailWnd);
	
	if (!g_lpAKC || g_lpAKC->m_wKarmaCount == 0)
	{
		s_lpSelectKarma	=	NULL;

		DrawKarmaDetail();
		SendMessage(hKarmaList, LB_ADDSTRING, 0,(LPARAM)dMSG_THERE_ARE_NO_KARMA);
		return;
	}

	int index,pos=0;
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

	s_iKarmaDetailWndPos	=	0;
	
	DrawKarmaDetail();
}

//
// 이벤트 내용 출력
void
CSetDialogMonster::DrawKarmaDetail()
{
	CKarma	karma;
	char	strOutputText[1024];

	int		x=5,y=10,iWidth,iHeight,i;
	int		iTriggerHeight=0,iReactionHeight=0;

	if	(!s_hKarmaDetailWnd.Active())
		return;
	
	iWidth	=	cDRAW::WIDTH-10;
	cDRAW::Fill(0);
	
	if (!s_lpSelectKarma || s_lpSelectKarma->m_wTriggerCount + s_lpSelectKarma->m_wReactionCount <= 0)
	{
		EnableScrollBar(s_hKarmaDetailWnd.hWND,SB_VERT,ESB_DISABLE_BOTH);
		cTEXT::Put(x,y,SKYBLUE,dMSG_THERE_ARE_NO_KARMA);
		s_hKarmaDetailWnd.Draw();
		return;
	}

	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;

	cACTOR_SET *lpActorSet = &g_lpActmap->m_setActor;
	cACTOR *lpActor   = lpActorSet->getActor(selectlist);
     
	if (!lpActor) {
		s_hKarmaDetailWnd.Draw();		
		return;
	}

	karma.copy(s_lpSelectKarma);

	int	iHeightPos	= s_sbMonsterKarma.GetHeightPos();
	
	s_sbMonsterKarma.SetHeight(y,cDRAW::HEIGHT);
	s_sbMonsterKarma.SetHeightPos(iHeightPos);

	iHeight	=	y+18;

	for (i=0;i<karma.m_wTriggerCount;i++)
	{
		char	*lpText		=	cKARMA_WND::KarmaText(&karma.m_pTrigger[i],TRUE);
		sprintf(strOutputText," <조건 #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth - 10,strOutputText);

		iHeight				+=	iLineCount*14;
		iTriggerHeight		+=	iLineCount*14;
	}

	iHeight	+=	(21+15);

	for (i=0;i<karma.m_wReactionCount;i++)
	{
		char	*lpText		=	cKARMA_WND::KarmaText(&karma.m_pReaction[i],FALSE);
		sprintf(strOutputText," <액션 #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth- 10,strOutputText);

		iHeight				+=	iLineCount*14;
		iReactionHeight		+=	iLineCount*14;
	}

	if (iHeight > cDRAW::HEIGHT)
	{	
		SCROLLINFO	si;

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
	
	cTEXT::cPut(208,y,SKYBLUE,_ms("발동확률[%d％]체크주기[%d초]발동횟수제한[%d회]활성트리거주건수[%d회]",karma.m_wActivateChance,karma.m_wActivePeriod,karma.m_wActiveCount,karma.m_bf7ActivateTriggerCount));

	y	+=	18;

	if (iTriggerHeight	==	0)
	{
		cTEXT::cPut(x+iWidth/2,y+5,LTGREEN,"- 이 작업을 실행 시키기 위한 별다른 조건이 없습니다. -");

		y	+=	21;
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
			char	*lpText		=	cKARMA_WND::KarmaText(&karma.m_pTrigger[i],TRUE);
			sprintf(strOutputText," <조건 #%2d> - %s",i+1,lpText);
			int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

			cTEXT::FitPut(x+5,y,iWidth - 5,100,WHITE,strOutputText);

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
		sprintf(strOutputText," <액션 #%2d> - %s",i+1,lpText);
		int		iLineCount	=	cTEXT::GetLines(iWidth,strOutputText);

		cTEXT::FitPut(x+5,y,iWidth-5,100,WHITE,strOutputText);
		
		y					+=	iLineCount*14;
	}
	s_hKarmaDetailWnd.Draw();
}

//
// 몬스터 삭제
void
CSetDialogMonster::RemoveMonster(HWND _hDlg)
{
	if(g_lpActmap->m_bf1IsLocked)
	{
		MessageBox(_hDlg, "그리기 금지 상태입니다.", "알림", MB_OK);
		return;
	}
	if(MessageBox(_hDlg, "선택된 몬스터가 삭제됩니다. 삭제 하시겠습니까?", "몬스터 삭제", MB_YESNO) == IDYES)
	{
		int selectlist = 0;
		s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;

		cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;
		lpActorSet	->remove(selectlist);
		
		ResetMonsterListBox(s_iSelOldList);
		ResetMonsterInfoBox( s_iSelOldInfo);

		g_MAPWND.Draw();
	}
}

//
// 모든 몬스터 삭제
void
CSetDialogMonster::RemoveAllMonster(HWND _hDlg)
{
	if(g_lpActmap->m_bf1IsLocked)
	{
		MessageBox(_hDlg, "그리기 금지 상태입니다.", "알림", MB_OK);
		return;
	}

	if(MessageBox(_hDlg, "모든 몬스터가 삭제됩니다. 모두 삭제하시겠습니까?", "모든 몬스터 삭제", MB_YESNO) == IDYES)
	{
		cACTOR_SET	*lpActorSet	=	&g_lpActmap->m_setActor;
		lpActorSet->removeAllMonster();
		
		ResetMonsterListBox(s_iSelOldList);
		ResetMonsterInfoBox(s_iSelOldInfo);

		g_MAPWND.Draw();
	}
}

//
// 몬스터 데이터 정보 편집
void
CSetDialogMonster::EditMonsterBaseData(HWND _hDlg)
{
	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;
	
	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(selectlist);

	cSET_FIELD_CHARACTER::Init(_hDlg, lpActor);

	ResetMonsterListBox(s_iSelOldList);
	ResetMonsterInfoBox(s_iSelOldInfo);
	g_MAPWND.Draw();
}

//
// 몬스터 이벤트 편집
void
CSetDialogMonster::EditMonsterEvent(HWND _hDlg)
{
	int selectlist = 0;
	s_iSelOldList == ePAGE_LIST ? selectlist = s_iSelectMonsterData : selectlist = s_iSelectFindMonsterData;
	
	cACTOR *lpActor	=	g_lpActmap->m_setActor.getActor(selectlist);
	cCHARACTER *_lpEditCharacter = lpActor->getCharacter();
	
	cSET_CHARACTER_TREE_DATA::SetTreeEvent(_hDlg, _lpEditCharacter);		// 09.11.30 수정

	ResetMonsterListBox(s_iSelOldList);
	ResetMonsterInfoBox(s_iSelOldInfo);
	g_MAPWND.Draw();
}

//
// 화면에 몬스터 편집 대화상자 출력
void
CSetDialogMonster::ShowDialog()
{
	ShowWindow(s_hWnd, SW_SHOW);
}


//
// 베이스 Proc
LRESULT CALLBACK
CSetDialogMonster::ProcSetDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int SelList = 0, SelInfo = 0;
	
	switch(uMsg)
	{
	case WM_INITDIALOG:
		s_hWnd = hDlg;
		s_hSlider = GetDlgItem(hDlg ,IDC_SLIDER_MONSTER_DATA);
		
		SendMessage(s_hSlider, TBM_SETRANGE, FALSE, MAKELONG(DLG_ALPHAMIN,DLG_ALPHAMAX));
		SendMessage(s_hSlider, TBM_SETPOS, TRUE, g_iDlgMonsterAlpha);
		{
			SLWA pSetLayeredWindowAttributes = NULL;  
			HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL"); 
			pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
			SetWindowLong(hDlg, GWL_EXSTYLE,GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
			pSetLayeredWindowAttributes(hDlg, 0, g_iDlgMonsterAlpha, LWA_ALPHA);
		}

		InitPageData(hDlg);
		InitSetDialogListBoxData(SelList);
		break;
	
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			
			SelList = TabCtrl_GetCurSel(s_hListTab);
			SelInfo = TabCtrl_GetCurSel(s_hInfoTab);
			
			if(s_iSelOldList != SelList)
			{
				if(s_hListTabDlg)	
					DestroyWindow(s_hListTabDlg);

				if(SelList  == ePAGE_LIST)
					s_hListTabDlg = CreateDialog(s_hInst, "PAGE_LIST", hDlg,ProcPageMonsterList);
				if(SelList  == ePAGE_FIND_LIST)
					s_hListTabDlg = CreateDialog(s_hInst, "PAGE_FIND_LIST", hDlg,ProcPageMonsterFind);

				ShowWindow(s_hListTabDlg, SW_SHOW);

				s_iSelectFindMonsterData = 0xffffff;
				s_iSelOldList = SelList;

				DrawPageInfo(s_iSelOldInfo);
			}

			if(s_iSelOldInfo != SelInfo)
			{
				if(s_hInfoTabDlg)
					DestroyWindow(s_hInfoTabDlg);

				if(SelInfo == ePAGE_MONSTER_INFO)
					s_hInfoTabDlg = CreateDialog(s_hInst, "PAGE_MONSTER_INFO", hDlg,ProcPageMonsterInfo);
				if(SelInfo == ePAGE_MONSTER_EVENT)
					s_hInfoTabDlg = CreateDialog(s_hInst, "PAGE_MONSTER_EVENT", hDlg,ProcPageMonsterEvent);
				if(SelInfo == ePAGE_MONSTER_JOB)
					s_hInfoTabDlg = CreateDialog(s_hInst, "PAGE_MONSTER_JOB", hDlg,ProcPageMonsterJob);
				if(SelInfo == ePAGE_MONSTER_DEATH)
					s_hInfoTabDlg = CreateDialog(s_hInst, "PAGE_MONSTER_DEATH", hDlg,ProcPageMonsterDeath);
				
				ShowWindow(s_hInfoTabDlg, SW_SHOW);
			}
						
			s_iSelOldInfo = SelInfo;

			break;
		}
		break;
	case WM_HSCROLL:
		if ((HWND)lParam == s_hSlider)
		{
			g_iDlgMonsterAlpha	= SendMessage(s_hSlider, TBM_GETPOS,0,0);
			
			SLWA pSetLayeredWindowAttributes = NULL;  
			HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL"); 
			pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
			SetWindowLong(hDlg, GWL_EXSTYLE,GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
			pSetLayeredWindowAttributes(hDlg, 0, g_iDlgMonsterAlpha, LWA_ALPHA);
		}
		break;
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDC_REMOVE_ALLMONSTER:	
			RemoveAllMonster(hDlg);		
			break;
		case IDC_REMOVE_MONSTER:			
			RemoveMonster(hDlg);		
			break;
		case IDC_EDIT_BASEDATA:			
			EditMonsterBaseData(hDlg);	
			break;
		case IDC_EDIT_MONSTER_EVENT:		
			EditMonsterEvent(hDlg);		
			break;	
		case IDC_MONSTER_NPC_DATA:									
			cJOB_LIST::Init(hDlg, s_hInst);
			break;	
		case IDC_CLOSE:
			s_bEnableDialog = FALSE;
			ShowWindow(hDlg, SW_HIDE);
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
// 기본 정보
LRESULT CALLBACK
CSetDialogMonster::ProcMonsterBaseInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT		:
		{	
			PAINTSTRUCT ps;
			BeginPaint( hDlg, &ps );
			
			s_dibMonsterBaseInfo.Active();
			s_dibMonsterBaseInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);
			
			EndPaint(hDlg, &ps);
			break;
		}
	}
	
	return (0L);
}

//
// 고유 정보
LRESULT CALLBACK
CSetDialogMonster::ProcMonsterGeneralInfoWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT		:
		{	
			PAINTSTRUCT ps;
			BeginPaint( hDlg, &ps );
			
			s_dibMonsterGeneralInfo.Active();
			s_dibMonsterGeneralInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);
			
			EndPaint(hDlg, &ps);
			break;
		}

	}
	return (0L);
}

// 이벤트 세부 내용
LRESULT CALLBACK
CSetDialogMonster::ProcMonsterKarmaDetailWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

//
// 직업 데이터
LRESULT CALLBACK
CSetDialogMonster::ProcMonsterJobWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT		:
		{	
			PAINTSTRUCT ps;
			BeginPaint( hDlg, &ps );
			
			s_dibMonsterJob.Active();
			s_dibMonsterJob.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);
			
			EndPaint(hDlg, &ps);
			break;
		}

	}
	return (0L);
}

//
// 사망 데이터
LRESULT CALLBACK
CSetDialogMonster::ProcMonsterDeathWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_PAINT		:
		{	
			PAINTSTRUCT ps;
			BeginPaint( hDlg, &ps );
			
			s_dibMonsterDeath.Active();
			s_dibMonsterDeath.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);
			
			EndPaint(hDlg, &ps);
			break;
		}
	}
	return (0L);
}

//
// 정보
BOOL CALLBACK
CSetDialogMonster::ProcPageMonsterInfo(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;
	HWND	hMonsterInfo;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(s_hInfoTab, &prt);
		TabCtrl_AdjustRect(s_hInfoTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);

		hMonsterInfo =	GetDlgItem(hDlg, IDC_MONSTER_BASEDATA);
		s_dibMonsterBaseInfo.lpszRegister	=	"MONSTERBASEINFO_WINDOW";
		s_dibMonsterBaseInfo.Init(hMonsterInfo,(WNDPROC)ProcMonsterBaseInfoWnd);

		hMonsterInfo =	GetDlgItem(hDlg, IDC_MONSTE_GENERALDATA);

		s_dibMonsterGeneralInfo.lpszRegister	=	"MONSTERGENERALINFO_WINDOW";
		s_dibMonsterGeneralInfo.Init(hMonsterInfo,(WNDPROC)ProcMonsterGeneralInfoWnd);

		ResetMonsterInfoBox(ePAGE_MONSTER_INFO);
		break;
	}
	
	return (0L);
}

//
// 이벤트
BOOL CALLBACK
CSetDialogMonster::ProcPageMonsterEvent(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;
	HWND	hKarmaDetail;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(s_hInfoTab, &prt);
		TabCtrl_AdjustRect(s_hInfoTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);

		s_hPageEvent = hDlg;

		hKarmaDetail = GetDlgItem(hDlg,IDC_KARMA_MONSTERVIEW);
		s_hKarmaDetailWnd.lpszRegister	=	"MONSTERKARMA_DETAIL_WINDOW";
		s_hKarmaDetailWnd.Init(hKarmaDetail,(WNDPROC)ProcMonsterKarmaDetailWnd);

		s_sbMonsterKarma.Init(hKarmaDetail);
		s_sbMonsterKarma.InitHeight();

		SetEvent();
		DrawPageInfo(ePAGE_MONSTER_EVENT);
		break;
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDC_EVENT_CONDITION_MONSTERLIST:
			SelectCondition();
			break;
		case IDC_KARMA_MONSTERLIST:
			SelectKarma();
			break;
		}
	}

	return (0L);
}

//
// 직업 데이터
BOOL CALLBACK
CSetDialogMonster::ProcPageMonsterJob(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(s_hInfoTab, &prt);
		TabCtrl_AdjustRect(s_hInfoTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);
		
		HWND hMonsterJob =	GetDlgItem(hDlg, IDC_MONSTER_JOB);
		s_dibMonsterJob.lpszRegister	=	"MONSTERJOB_WINDOW";
		s_dibMonsterJob.Init(hMonsterJob,(WNDPROC)ProcMonsterJobWnd);

		DrawPageInfo(ePAGE_MONSTER_JOB);
		break;
	}

	return (0L);
}

//
// 사망 데이터
BOOL CALLBACK
CSetDialogMonster::ProcPageMonsterDeath(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(s_hInfoTab, &prt);
		TabCtrl_AdjustRect(s_hInfoTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);
		
		HWND hMonsterDeath = GetDlgItem(hDlg, IDC_MONSTER_DEATH);
		s_dibMonsterDeath.lpszRegister	=	"MONSTERDEATH_WINDOW";
		s_dibMonsterDeath.Init(hMonsterDeath,(WNDPROC)ProcMonsterDeathWnd);
				
		DrawPageInfo(ePAGE_MONSTER_DEATH);
		break;
	}

	return (0L);
}

//
// 목록
BOOL CALLBACK
CSetDialogMonster::ProcPageMonsterList(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
// 목록 컨트롤
LRESULT CALLBACK		
CSetDialogMonster::ProcMonsterListWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			SetFocus(hDlg);		// 09.12.17 추가
			s_iSelectMonsterData	= s_listMonster.GetSelectData();
						
			cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
			cACTOR *lpActor			= lpActorSet->getActor(s_iSelectMonsterData);
						
			if (!lpActor)	break;
						
			ResetMonsterInfoBox(s_iSelOldInfo);
			
			lpActorSet->freeSelect();
			lpActor->m_isSelected	= TRUE;
			
			int x = (int)((lpActor->getXpos() / dTILE_WIDTH)  - (g_MAPWND.TPW / 2));
			int y = (int)((lpActor->getYpos() / dTILE_HEIGHT) - (g_MAPWND.TPH / 2));
			
			g_lpActmap->setPos( x+2,y);
			g_MAPWND.Draw();
			break;
		}
		case WM_MOUSEWHEEL :
		{
			int zDelta = ((short)HIWORD(wParam));
			
			if	(zDelta>=0)
				s_listMonster.VScroll(SB_LINEUP);
			else
				s_listMonster.VScroll(SB_LINEDOWN);

			DrawPageList(ePAGE_LIST);		
			break;
		}
	}
	return (0L);
}

//
// 검색 목록
BOOL CALLBACK
CSetDialogMonster::ProcPageMonsterFind(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT prt;
	HWND hEdit;

	switch(uMsg)
	{
	case WM_INITDIALOG:
		GetWindowRect(s_hListTab, &prt);
		TabCtrl_AdjustRect(s_hListTab, FALSE, &prt);
		ScreenToClient(hDlg, (LPPOINT)&prt);
		SetWindowPos(hDlg, HWND_TOP, prt.left, prt.top, 0, 0, SWP_NOSIZE);

		hEdit = GetDlgItem(hDlg, IDC_EDIT_FIND_LIST);
		SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)16, 0);

		s_hPageFindList	= hDlg;
		InitSetDialogListBoxData(ePAGE_FIND_LIST);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_EDIT_FIND_LIST:
			switch(HIWORD(wParam))
			{
			case EN_CHANGE:
				ResetMonsterListBox(ePAGE_FIND_LIST);
				break;
			}
			break;
		}
		break;
	}
	
	return (0L);
}

// 
// 검색 목록 컨트롤
LRESULT CALLBACK		
CSetDialogMonster::ProcMonsterFindListWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{

			SetFocus(hDlg);			// 09.12.17 추가
			s_iSelectFindMonsterData	= s_listFindMonster.GetSelectData();
			
			cACTOR_SET	*lpActorSet	= &g_lpActmap->m_setActor;
			cACTOR *lpActor			= lpActorSet->getActor(s_iSelectFindMonsterData);
			
			if (!lpActor)	break;
						
			ResetMonsterInfoBox(s_iSelOldInfo);
			
			lpActorSet->freeSelect();
			lpActor->m_isSelected	= TRUE;
			
			int x = (int)((lpActor->getXpos() / dTILE_WIDTH)  - (g_MAPWND.TPW / 2));
			int y = (int)((lpActor->getYpos() / dTILE_HEIGHT) - (g_MAPWND.TPH / 2));
			
			g_lpActmap->setPos( x+2,y);
			
			g_MAPWND.Draw();
			break;
		}
	case WM_MOUSEWHEEL :
		{
			int zDelta = ((short)HIWORD(wParam));
			
			if	(zDelta>=0)
				s_listFindMonster.VScroll(SB_LINEUP);
			else
				s_listFindMonster.VScroll(SB_LINEDOWN);

			DrawPageList(ePAGE_FIND_LIST);		
			break;
		}
	}
	return (0L);
}

