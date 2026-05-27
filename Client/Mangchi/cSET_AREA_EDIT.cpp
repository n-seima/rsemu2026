#include	"cSET.H"
#include	"cMAIN.H"
#include	"cAREA.H"
#include	"cPROJECT.H"
#include	"resource.h"
#include	"cMAPWND.h"
#include	"cPROJECT.h"
#include	"arca.h"
#include	"trapSkill.h"
#include	"secretDungeon.h"
#include	"CSetDialog.h"
#include	"cTOOLWND.H"
#include "cSetDialogSpeech.h"

HWND					cSET_AREA_EDIT::s_hWnd				= NULL;
CConversationContainer	cSET_AREA_EDIT::s_cc;
cLISTCONTROL			cSET_AREA_EDIT::s_listArea;
int						cSET_AREA_EDIT::s_iAreaListWndPos	= 0;
int						cSET_AREA_EDIT::s_iSelectAreaList	= 0xffff;
int						cSET_AREA_EDIT::s_iAreaCount		= -1;

BOOL					cSET_AREA_EDIT::s_bEnableDialog		= FALSE;

int		l_aiSortAreaEdit[]	=	{dAREA_NAMED_AREA,dAREA_DOOR,dAREA_TRAP,dAREA_EVENT_AREA,dAREA_PORTAL,dAREA_ARCA,dAREA_START_AREA,dAREA_REVIVE_AREA,-1};
cRECT	l_SetAreaEditBoxRect;

BOOL
cSET_AREA_EDIT::SetAreaEdit(HWND hwnd)
{
	s_bEnableDialog = TRUE;

	if(s_hWnd != NULL)
		return FALSE;
			
	CreateDialog(s_hInst, "SET_AREA_EDIT", hwnd, (DLGPROC)s_procSetAreaEdit);

	RECT rt1, rt2;
	GetClientRect(g_MAPWND.hWND,&rt1);
	GetClientRect(s_hWnd, &rt2);
	
	SetWindowPos(s_hWnd, NULL, rt1.right - rt2.right + 100, 120, 0, 0, SWP_NOSIZE);

	return TRUE;
}

void
cSET_AREA_EDIT::InitAreaData(HWND hWnd)
{
	s_hWnd = hWnd;

	HWND hList = GetDlgItem(s_hWnd,IDC_LIST);
	s_listArea.Init(hList, 2, FALSE, s_procAreaListWnd);
		
	s_listArea.AddColumn("타입"			,32);
	s_listArea.AddColumn("이름"			,6*36);	
	
	ResetAreData();
	InitAreaDataBox();	
}

void				
cSET_AREA_EDIT::InitAreaDataBox()
{
	if(!s_bEnableDialog)
		return;

	HWND	hComboAreaKind	=	GetDlgItem(s_hWnd,IDC_AREA_KIND);
	HWND	hComboSeason	=	GetDlgItem(s_hWnd,IDC_COMBO_SEASON);
	HWND	hComboSwapArea	=	GetDlgItem(s_hWnd,IDC_COMBO_SWAP_AREA);

	s_cc.reset();

	int		iKind	=	0;

	for (iKind=0;;iKind++)
	{
		if (stricmp(g_strAreaKind[iKind],"end")==0)
			break;

		cCOMBOBOX::Add(hComboAreaKind,g_strAreaKind[iKind],iKind);
	}

	for (iKind=0;;iKind++)
	{
		if (stricmp(g_strSeasonKind[iKind],"end")==0)
			break;
		
		cCOMBOBOX::Add(hComboSeason,g_strSeasonKind[iKind],iKind);
	}

	cCOMBOBOX::SelectByData(hComboSeason,g_lpActmap->m_pArea->m_aArea[g_lpActmap->m_pArea->m_iSelectArea].m_wSeasonVariable);

	for (iKind=0;;iKind++)
	{
		if (stricmp(g_strSwapSlotKind[iKind],"end")==0)
			break;
		
		cCOMBOBOX::Add(hComboSwapArea,g_strSwapSlotKind[iKind],iKind);
	}
	
	//m_iSelectArea
	cCOMBOBOX::SelectByData(hComboSwapArea,0);
	
	SetDlgItemText(s_hWnd,IDC_NAME,g_lpSelectArea->m_strName);
	
	s_cc.copy(&g_lpSelectArea->m_cc);
	
	cCOMBOBOX::SelectByData(hComboAreaKind,g_lpSelectArea->m_wKind);
	
	Reset(s_hWnd,g_lpSelectArea);

	SetFocus(GetDlgItem(s_hWnd,IDC_NAME));
	SendMessage(GetDlgItem(s_hWnd,IDC_NAME),EM_SETSEL,0,-1);
}

void			
cSET_AREA_EDIT::ResetAreData()
{
	if(!s_bEnableDialog)
		return;

	cAREA *lpArea	=	g_lpActmap->m_pArea;
	int	iAreaType	=	0;
	int index,count;

	count	=	0;
	s_listArea.ResetItems();
	s_iAreaCount	= -1;
	
	while(1)
	{
		int	iCurrentType	=	l_aiSortAreaEdit[iAreaType];

		if	(iCurrentType	==	-1)
			break;

		iAreaType++;

		for (index=2;index<dMAX_AREA;index++)
		{
			cAREA_INFO	*lpAreaInfo	=	&lpArea->m_aArea[index];

			if (lpAreaInfo->m_wSerial	== 0xffff)
				continue;
			if (lpAreaInfo->m_wKind	!=	iCurrentType)
				continue;
			
			char	strAreaName[100];
			strcpy(strAreaName,lpAreaInfo->m_strName);
		
			switch(iCurrentType)
			{
				case	dAREA_DOOR			:
					s_listArea.AddItem(0,"[D]", index);
					break;
				case	dAREA_TRAP			:
					s_listArea.AddItem(0,"[T]", index);
					break;
				case	dAREA_ARCA			:
					s_listArea.AddItem(0,"[A]", index);
					break;
				case	dAREA_EVENT_AREA	:
					s_listArea.AddItem(0,"[E]", index);
					break;
				case	dAREA_PORTAL		:
					s_listArea.AddItem(0,"[G]", index);
					break;
				case	dAREA_START_AREA	:
					s_listArea.AddItem(0,"[S]", index);
					break;
				case	dAREA_NAMED_AREA	:
					s_listArea.AddItem(0,"[N]", index);
					break;
				case	dAREA_REVIVE_AREA	:
					s_listArea.AddItem(0,"[R]", index);
					break;

				case	dAREA_SECRET_DUNGEON_EXIT	:
					s_listArea.AddItem(0,"[비던출구]", index);
					break;
			}

			s_listArea.AddItem(1,strAreaName);
			s_listArea.IncreaseLineCounter();
			s_iAreaCount++;
		}
	}

	for (index=2;index<dMAX_AREA;index++)
	{
		cAREA_INFO	*lpAreaInfo	=	&lpArea->m_aArea[index];

		if (lpAreaInfo->m_wSerial	== 0xffff)
			continue;

		BOOL	bIsSpecial		=	FALSE;

		for (int j=0;;j++)
		{
			if (l_aiSortAreaEdit[j]	==	-1)
				break;
			if (l_aiSortAreaEdit[j]	==	lpAreaInfo->m_wKind)
				bIsSpecial			=	TRUE;
		}

		if (bIsSpecial)
			continue;
		
		s_listArea.AddItem(0,"", index);
		s_listArea.AddItem(1,lpAreaInfo->m_strName);
		s_listArea.IncreaseLineCounter();
		s_iAreaCount++;
	}

	if (!s_listArea.selectByData(0,s_iSelectAreaList))
	{
		if (!s_listArea.Select(0))	s_iSelectAreaList =	0xffff;
		else						s_iSelectAreaList =	s_listArea.GetData(0);
	}

	s_listArea.setHeightPos(s_iAreaListWndPos);
	s_listArea.Draw();
}

void
cSET_AREA_EDIT::Reset(HWND hDlg,cAREA_INFO *lpArea)
{
	cRECT	rect;

	rect.OutFrame(hDlg);

	ShowWindow(GetDlgItem(hDlg,IDS_CREATE_MONSTER	),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDS_REGEN			),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_REGEN_TIME		),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDS_REGEN_TIME		),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDS_MONSTER			),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_MONSTER_LIST		),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_LOAD_MAP			),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_LOAD_MAP_NAME	),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDS_LOAD_MAP			),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DIRECT2			),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDS_MOVE_GROUP		),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDS_CONNECT_AREA		),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_EXPLAIN			),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CLOSE			),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_OPEN				),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DIRECT			),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_GATE_SHAPE		),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DETAIL_KIND		),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT				),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_SET_EVENT		),SW_HIDE);

	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_REGEN_TIME),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_REGEN_TRAP_SECOND),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_REGEN_TRAP_SECOND2),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_DETECT_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_DETECT_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DISARM_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DISARM_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_SKILL1),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_SKILL1_LIST),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_SKILL1_CHANCE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_SKILL1_CHANCE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_SKILL2),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_SKILL2_LIST),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_SKILL2_CHANCE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_SKILL2_CHANCE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_FRAME_TRAP),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_TRAGET_TEXT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_TRAGET),SW_HIDE);


	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_LINK_SECRET_DUNGEON_TEXT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_LINK_SECRET_DUNGEON),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_REGEN_PERIOD),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_REGEN_PERIOD),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_REGEN_CHANCE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_REGEN_CHANCE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_UPKEEP_DOOR_TIME),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_UPKEEP_DOOR_TIME),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_UPKEEP_DOOR_TIME_MINUTE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_SECRET_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_SECRET_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DURABILITY),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DURABILITY),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DESTROY_MIN_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DESTROY_MIN_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DEFENSE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DEFENSE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_LOCK_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_LOCK_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TEXT_UNLOCK_UPKEEP_TIME),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_UNLOCK_UPKEEP_TIME),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DESTROY_ABLE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAGET_TEXT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAGET),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DETECT_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DETECT_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DISARM_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DISARM_TRAP_LEVEL),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1_TEXT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1_ACTIVE_CHANCE_TEXT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1_ACTIVE_CHANCE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1_ACTIVE_CHANCE_TEXT2),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2_TEXT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2_ACTIVE_CHANCE_TEXT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2_ACTIVE_CHANCE),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2_ACTIVE_CHANCE_TEXT2),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_FRAME_DOOR),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_COMBO_SEASON),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SEASON),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_COMBO_SWAP_AREA),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SWAP_AREA),SW_HIDE);

	ShowWindow(GetDlgItem(hDlg,IDC_AREA_CHECK_NPC	),SW_HIDE);		
	ShowWindow(GetDlgItem(hDlg, IDC_TEXT_DOOR_DEFENSE_PERCENT), SW_HIDE);
	ShowWindow(GetDlgItem(hDlg, IDC_DOOR_DEFENSE_PERCENT), SW_HIDE);
	// 10.02.25 오브젝트 변경
	ShowWindow(GetDlgItem(hDlg,IDC_IS_CHANGE_OBJECT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_INDEX_CHANGE_OBJECT),SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_TIME)		,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_START_YEAR)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_START_MONTH)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_START_DAY)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_START_HOUR)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_START_MINUTE)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_END_YEAR)		,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_END_MONTH)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_END_DAY)		,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_END_HOUR)		,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_CB_END_MINUTE)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SYEAR)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SMONTH)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SDAY)		,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SHOUR)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SMINUTE)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EYEAR)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EMONTH)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EDAY)		,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EHOUR)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EMINUTE)	,SW_HIDE);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_INDEX)	,SW_HIDE);
	if (lpArea->m_bf1IsTestPortal	==	TRUE && lpArea->m_bf15LinkObject != 0x7fff)
		CheckDlgButton(hDlg,IDC_IS_TEST_AREA,TRUE);
	else
		CheckDlgButton(hDlg,IDC_IS_TEST_AREA,FALSE);
	
	switch(lpArea->m_wKind)
	{
		case	dAREA_START_AREA	:
		case	dAREA_GUILD_BATTLE	:
		case	dAREA_NORMAL		:
		case	dAREA_MONSTER_ACTIVE:
		case	dAREA_NAMED_AREA	:
		case	dAREA_GUILD_CREST	:
		{
			//MoveWindow(hDlg,rect.x1,rect.y1,rect.x2-rect.x1,170,TRUE);
			break;
		}

		case	dAREA_ARCA			:
		{
			InitArcaAreaForm(hDlg,lpArea,&rect);
			break;
		}

		case	dAREA_NOTICE_BOARD	:
		{
			ShowWindow(GetDlgItem(hDlg,IDC_EXPLAIN		),SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_TEXT			),SW_SHOW);

			SetWindowText(GetDlgItem(hDlg,IDC_EXPLAIN	),dMSG_CONTENTS);

			if	(lpArea->m_pString)
				SetDlgItemText(hDlg,IDC_TEXT,lpArea->m_pString);
			else
				SetDlgItemText(hDlg,IDC_TEXT,"");
			break;
		}

		case	dAREA_DOOR		:
		{
			InitDoorAreaForm(hDlg,lpArea,&rect);
			break;
		}

		case	dAREA_PORTAL	:
		{
			InitPortalAreaForm(hDlg,lpArea,&rect);
			break;
		}
		case	dAREA_TRAP		:
		{
			InitTrapAreaForm(hDlg,lpArea,&rect);
			break;
		}

		case	dAREA_EVENT_AREA:
		{
			ShowWindow(GetDlgItem(hDlg,IDC_SET_EVENT		),SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_COMBO_SWAP_AREA	),SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SWAP_AREA	),SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_AREA_CHECK_NPC	),SW_SHOW);		// 09.08.12
			// 10.02.25 오브젝트 변경
			ShowWindow(GetDlgItem(hDlg,IDC_IS_CHANGE_OBJECT),SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_INDEX_CHANGE_OBJECT),SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_TIME)		,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_START_YEAR)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_START_MONTH)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_START_DAY)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_START_HOUR)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_START_MINUTE)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_END_YEAR)		,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_END_MONTH)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_END_DAY)		,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_END_HOUR)		,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_CB_END_MINUTE)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SYEAR)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SMONTH)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SDAY)		,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SHOUR)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SMINUTE)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EYEAR)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EMONTH)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EDAY)		,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EHOUR)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_EMINUTE)	,SW_SHOW);
			ShowWindow(GetDlgItem(hDlg,IDC_STATIC_INDEX)	,SW_SHOW);
			cAreaEvent	*lpEventArea	=	(cAreaEvent	*)lpArea;			
			CheckDlgButton(hDlg,IDC_AREA_CHECK_NPC,lpEventArea->m_bf1Npc);
			// 10.02.25
			HWND	hCombosYear		= GetDlgItem(hDlg,IDC_CB_START_YEAR);	
			HWND	hCombosMonth	= GetDlgItem(hDlg,IDC_CB_START_MONTH);	
			HWND	hCombosDay		= GetDlgItem(hDlg,IDC_CB_START_DAY);	
			HWND	hCombosHour		= GetDlgItem(hDlg,IDC_CB_START_HOUR);	
			HWND	hCombosMinute	= GetDlgItem(hDlg,IDC_CB_START_MINUTE);	
			HWND	hComboeYear		= GetDlgItem(hDlg,IDC_CB_END_YEAR);		
			HWND	hComboeMonth	= GetDlgItem(hDlg,IDC_CB_END_MONTH);	
			HWND	hComboeDay		= GetDlgItem(hDlg,IDC_CB_END_DAY);		
			HWND	hComboeHour		= GetDlgItem(hDlg,IDC_CB_END_HOUR);		
			HWND	hComboeMinute	= GetDlgItem(hDlg,IDC_CB_END_MINUTE);	
			
			CheckDlgButton(hDlg,IDC_IS_CHANGE_OBJECT,lpEventArea->m_bf1ChangeObject);
			SetDlgItemText(hDlg,IDC_INDEX_CHANGE_OBJECT	,_ms("%d", lpEventArea->m_iChangeObjectIndex));
		
			int iKind =	0;
			
			// sYear
			cCOMBOBOX::Reset(hCombosYear);
			for (iKind=0;iKind < 101;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hCombosYear,"00",iKind);
				else
					cCOMBOBOX::Add(hCombosYear,_ms("%d",2000+iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hCombosYear, lpEventArea->m_bf7StartYear);

			// sMonth
			cCOMBOBOX::Reset(hCombosMonth);
			for (iKind=0;iKind < 14;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hCombosMonth,"00",iKind);
				else
					cCOMBOBOX::Add(hCombosMonth,_ms("%d",iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hCombosMonth, lpEventArea->m_bf4StartMonth);
			
			// sDay
			cCOMBOBOX::Reset(hCombosDay);
			for (iKind=0;iKind < 33;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hCombosDay,"00",iKind);
				else
					cCOMBOBOX::Add(hCombosDay,_ms("%d",iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hCombosDay, lpEventArea->m_bf5StartDay);

			// sHour
			cCOMBOBOX::Reset(hCombosHour);
			for (iKind=0;iKind < 25;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hCombosHour,"00",iKind);
				else
					cCOMBOBOX::Add(hCombosHour,_ms("%d",iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hCombosHour, lpEventArea->m_bf5StartHour);

			// sMinute
			cCOMBOBOX::Reset(hCombosMinute);
			for (iKind=0;iKind < 61;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hCombosMinute,"00",iKind);
				else
					cCOMBOBOX::Add(hCombosMinute,_ms("%d",iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hCombosMinute, lpEventArea->m_bf6StartMinute);

			// eYear
			cCOMBOBOX::Reset(hComboeYear);
			for (iKind=0;iKind < 101;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hComboeYear,"00",iKind);
				else
					cCOMBOBOX::Add(hComboeYear,_ms("%d",2000+iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hComboeYear, lpEventArea->m_bf7EndYear);
			
			// eMonth
			cCOMBOBOX::Reset(hComboeMonth);
			for (iKind=0;iKind < 14;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hComboeMonth,"00",iKind);
				else
					cCOMBOBOX::Add(hComboeMonth,_ms("%d",iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hComboeMonth, lpEventArea->m_bf4EndMonth);
			
			// eDay
			cCOMBOBOX::Reset(hComboeDay);
			for (iKind=0;iKind < 33;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hComboeDay,"00",iKind);
				else
					cCOMBOBOX::Add(hComboeDay,_ms("%d",iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hComboeDay, lpEventArea->m_bf5EndDay);

			// eHour
			cCOMBOBOX::Reset(hComboeHour);
			for (iKind=0;iKind < 25;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hComboeHour,"00",iKind);
				else
					cCOMBOBOX::Add(hComboeHour,_ms("%d",iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hComboeHour, lpEventArea->m_bf5EndHour);

			// eMinute
			cCOMBOBOX::Reset(hComboeMinute);
			for (iKind=0;iKind < 61;iKind++)
			{
				if(iKind == 0)
					cCOMBOBOX::Add(hComboeMinute,"00",iKind);
				else
					cCOMBOBOX::Add(hComboeMinute,_ms("%d",iKind-1),iKind);
			}
			cCOMBOBOX::SelectByData(hComboeMinute, lpEventArea->m_bf6EndMinute);
			break;
		}
	}
}

void
cSET_AREA_EDIT::InitArcaAreaForm(HWND hDlg,cAREA_INFO *_lpArea,cRECT *_lpRect)
{

	cAreaArca	*lpArcaArea	=	(cAreaArca	*)_lpArea;

	ShowWindow(GetDlgItem(hDlg,IDC_GATE_SHAPE),SW_SHOW);

	HWND		hCombo		=	GetDlgItem(hDlg,IDC_GATE_SHAPE);

	cCOMBOBOX::Reset(hCombo);

	g_arcaManager.setFirst();

	while(1)
	{
		cArcaInfo	*lpArca	=	g_arcaManager.getNext();

		if (!lpArca)
			break;

		cCOMBOBOX::Add(hCombo,lpArca->m_strName,lpArca->m_wSerial);
	}

	cCOMBOBOX::SelectByData(hCombo,lpArcaArea->m_wLinkArca);

	//MoveWindow(hDlg,_lpRect->x1,_lpRect->y1,_lpRect->x2-_lpRect->x1,170,TRUE);
}


void
cSET_AREA_EDIT::InitDoorAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect)
{
	int		i;

	cAreaDoor	*lpDoor	=	(cAreaDoor	*)lpArea;

	ShowWindow(GetDlgItem(hDlg,IDC_EXPLAIN		),SW_SHOW);
//	ShowWindow(GetDlgItem(hDlg,IDC_CLOSE		),SW_SHOW);
//	ShowWindow(GetDlgItem(hDlg,IDC_OPEN			),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_SET_EVENT	),SW_SHOW);

	SetWindowText(GetDlgItem(hDlg,IDC_EXPLAIN	),dMSG_DOOR_STATUS);

	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_LINK_SECRET_DUNGEON_TEXT),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_LINK_SECRET_DUNGEON),SW_SHOW);

	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_REGEN_PERIOD),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_REGEN_PERIOD),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_REGEN_CHANCE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_REGEN_CHANCE),SW_SHOW);

	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_UPKEEP_DOOR_TIME),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_UPKEEP_DOOR_TIME),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_UPKEEP_DOOR_TIME_MINUTE),SW_SHOW);

	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_SECRET_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_SECRET_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DURABILITY),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DURABILITY),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DESTROY_MIN_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DESTROY_MIN_LEVEL),SW_SHOW);

	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DEFENSE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DEFENSE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_LOCK_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_LOCK_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TEXT_UNLOCK_UPKEEP_TIME),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_UNLOCK_UPKEEP_TIME),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DESTROY_ABLE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_TRAP_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_LEVEL),SW_SHOW);


	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAGET_TEXT),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAGET),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DETECT_TRAP_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DETECT_TRAP_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DOOR_DISARM_TRAP_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_DISARM_TRAP_LEVEL),SW_SHOW);

	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1_TEXT),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1_ACTIVE_CHANCE_TEXT),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1_ACTIVE_CHANCE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1_ACTIVE_CHANCE_TEXT2),SW_SHOW);

	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2_TEXT),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2_ACTIVE_CHANCE_TEXT),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2_ACTIVE_CHANCE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2_ACTIVE_CHANCE_TEXT2),SW_SHOW);

	ShowWindow(GetDlgItem(hDlg,IDC_FRAME_DOOR),SW_SHOW);
	
	ShowWindow(GetDlgItem(hDlg, IDC_TEXT_DOOR_DEFENSE_PERCENT), SW_SHOW);	//09.08.13
	ShowWindow(GetDlgItem(hDlg, IDC_DOOR_DEFENSE_PERCENT), SW_SHOW);	//09.08.13
	
	if	(lpArea->m_wValue	==	1)
		CheckDlgButton(hDlg,IDC_OPEN ,TRUE);
	else
	{
		lpArea->m_wValue	=	0;
		CheckDlgButton(hDlg,IDC_CLOSE,TRUE);
	}

	HWND	hLinkSecretDungeon	=	GetDlgItem(hDlg,IDC_DOOR_LINK_SECRET_DUNGEON);

	{

		cCOMBOBOX::Reset(hLinkSecretDungeon);

		cCOMBOBOX::Add(hLinkSecretDungeon,"없음",0);

		for (i=1;i<g_sdManager.getMaxCount();i++)
		{
			cSecretDungeon	*lpDungeon	=	g_sdManager.get(i);

			if (!lpDungeon)
				continue;

			cCOMBOBOX::Add(hLinkSecretDungeon,lpDungeon->m_strName,i);
		}

		cCOMBOBOX::Select(hLinkSecretDungeon,0);
	}

	HWND	hTrapSkillList1		=	GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1	);
	HWND	hTrapSkillList2		=	GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2	);
	HWND	hTrapTarget			=	GetDlgItem(hDlg,IDC_DOOR_TRAGET			);

	{
		cCOMBOBOX::Reset(hTrapTarget);

		for (i=0;;i++)
		{
			if (stricmp(g_strTrapTarget[i],"end")==0)
				break;

			cCOMBOBOX::Add(hTrapTarget,g_strTrapTarget[i],i);
		}

		cCOMBOBOX::Select(hTrapTarget,0);
	}

	{
		cCOMBOBOX::Reset(hTrapSkillList1);

		cCOMBOBOX::Add(hTrapSkillList1,"valid",0xffff);

		for (i=0;i<g_trapSkillManager.m_iCount;i++)
		{
			cTrapSkill	*lpSkill	=	&g_trapSkillManager.m_aTrapSkill[i];

			cCOMBOBOX::Add(hTrapSkillList1,lpSkill->m_strName,i);
		}

		cCOMBOBOX::Select(hTrapSkillList1,0);

		cCOMBOBOX::Reset(hTrapSkillList2);

		cCOMBOBOX::Add(hTrapSkillList2,"valid",0xffff);
		for (i=0;i<g_trapSkillManager.m_iCount;i++)
		{
			cTrapSkill	*lpSkill	=	&g_trapSkillManager.m_aTrapSkill[i];

			cCOMBOBOX::Add(hTrapSkillList2,lpSkill->m_strName,i);
		}

		cCOMBOBOX::Select(hTrapSkillList2,0);
	}

	SetDlgItemInt(hDlg,IDC_DOOR_REGEN_PERIOD,lpDoor->m_wRegenPeriod,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_REGEN_CHANCE,lpDoor->m_wGenChance,FALSE);
	SetDlgItemInt(hDlg,IDC_UPKEEP_DOOR_TIME,lpDoor->m_wUpkeepPeriod,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_SECRET_LEVEL,lpDoor->m_bSecretLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_DURABILITY,lpDoor->m_iDurability,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_DESTROY_MIN_LEVEL,lpDoor->m_wDestroyMinimumLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_DEFENSE,lpDoor->m_iDefense,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_LOCK_LEVEL,lpDoor->m_bUnlockLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_UNLOCK_UPKEEP_TIME,lpDoor->m_wUpkeepDismantlingTime,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_TRAP_LEVEL,lpDoor->m_wTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_DETECT_TRAP_LEVEL,lpDoor->m_bDetectTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_DISARM_TRAP_LEVEL,lpDoor->m_bDisarmTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_TRAP_SKILL1_ACTIVE_CHANCE,lpDoor->m_wTrapSkill1ActiveChance,FALSE);
	SetDlgItemInt(hDlg,IDC_DOOR_TRAP_SKILL2_ACTIVE_CHANCE,lpDoor->m_wTrapSkill2ActiveChance,FALSE);

	SetDlgItemInt(hDlg,IDC_DOOR_DEFENSE_PERCENT, lpDoor->m_bDefensePercent, FALSE);			// 09.08.13

	cCOMBOBOX::SelectByData(hTrapSkillList1,lpDoor->m_wTrapSkill1);
	cCOMBOBOX::SelectByData(hTrapSkillList2,lpDoor->m_wTrapSkill2);
	cCOMBOBOX::SelectByData(hTrapTarget,lpDoor->m_wTrapTarget);
	cCOMBOBOX::SelectByData(hLinkSecretDungeon,lpDoor->m_wLinkSecretDungeon);

	CheckDlgButton(hDlg,IDC_DOOR_DESTROY_ABLE	,lpDoor->m_bf1IsDestroyable);
}

void
cSET_AREA_EDIT::InitPortalAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect)
{
	HWND	hGateCombo		=	GetDlgItem(hDlg,IDC_DIRECT2	);
	HWND	hDirectComboBox	=	GetDlgItem(hDlg,IDC_GATE_SHAPE	);
	HWND	hShapeComboBox	=	GetDlgItem(hDlg,IDC_DETAIL_KIND	);

	int		i=0;

	ShowWindow(GetDlgItem(hDlg,IDC_LOAD_MAP		),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_LOAD_MAP_NAME),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDS_LOAD_MAP		),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DIRECT2		),SW_SHOW);
	//ShowWindow(GetDlgItem(hDlg,IDS_MOVE_GROUP	),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDS_CONNECT_AREA	),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_EXPLAIN		),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DIRECT		),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_GATE_SHAPE	),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DETAIL_KIND	),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_COMBO_SEASON	),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_STATIC_SEASON),SW_SHOW);

	SetWindowText(GetDlgItem(hDlg,IDC_EXPLAIN	),dMSG_DIRECT);

	cCOMBOBOX::Reset(hDirectComboBox);

	for (i=0;;i++)
	{
		if (stricmp(g_strPortalDirect[i],"end") == 0)
			break;

		cCOMBOBOX::Add(hDirectComboBox,g_strPortalDirect[i],i);
	}

	for (i=0;;i++)
	{
		if (stricmp(g_strPortalShape[i],"end") == 0)	break;

		cCOMBOBOX::Add(hShapeComboBox,g_strPortalShape[i],i);
	}

	cCOMBOBOX::Select(hDirectComboBox,lpArea->m_gateDirect);
	cCOMBOBOX::Select(hShapeComboBox,lpArea->m_gateShape);
//	이동할 맵의 이름
	if (lpArea->m_pString)	
		SetWindowText(GetDlgItem(hDlg,IDC_LOAD_MAP_NAME),lpArea->m_pString);

	cCOMBOBOX::Reset(hGateCombo	);

	cMAP::CPortalAreaInfo	aAreas[100];
	int						iCount=0;

	if(lpArea->m_pString)	cMAP::GetPortalAreas(lpArea->m_pString,aAreas,&iCount);

	cCOMBOBOX::Add(hGateCombo,dMSG_NOT_EXIST_CONNECT_AREA,0);

	for (i=0;i<iCount;i++)	cCOMBOBOX::Add(hGateCombo,aAreas[i].m_strName,i);

	if (lpArea->m_strMoveGateName)
	{
		if (cCOMBOBOX::Select(hGateCombo,lpArea->m_strMoveGateName)	==	CB_ERR)
			cCOMBOBOX::Select(hGateCombo,0);
	}
	else
		cCOMBOBOX::Select(hGateCombo,0);

//	MoveWindow(hDlg,_lpRect->x1,_lpRect->y1,_lpRect->x2-_lpRect->x1,256,TRUE);
}

void
cSET_AREA_EDIT::InitTrapAreaForm(HWND hDlg,cAREA_INFO *lpArea,cRECT *_lpRect)
{
	cAreaTrap	*lpTrap	=	(cAreaTrap	*)lpArea;

	int	i;

	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_REGEN_TIME),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_REGEN_TRAP_SECOND),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_REGEN_TRAP_SECOND2),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_DETECT_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_DETECT_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_DISARM_TRAP_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_DISARM_TRAP_LEVEL),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_SKILL1),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_SKILL1_LIST),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_SKILL1_CHANCE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_SKILL1_CHANCE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_SKILL2),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_SKILL2_LIST),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TEXT_TRAP_SKILL2_CHANCE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_SKILL2_CHANCE),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_FRAME_TRAP),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_TRAGET_TEXT),SW_SHOW);
	ShowWindow(GetDlgItem(hDlg,IDC_TRAP_TRAGET),SW_SHOW);
	
	HWND	hTrapSkillList1	=	GetDlgItem(hDlg,IDC_TRAP_SKILL1_LIST	);
	HWND	hTrapSkillList2	=	GetDlgItem(hDlg,IDC_TRAP_SKILL2_LIST	);
	HWND	hTrapTarget		=	GetDlgItem(hDlg,IDC_TRAP_TRAGET			);

	{
		cCOMBOBOX::Reset(hTrapTarget);

		for (i=0;;i++)
		{
			if (stricmp(g_strTrapTarget[i],"end")==0)
				break;

			cCOMBOBOX::Add(hTrapTarget,g_strTrapTarget[i],i);
		}

		cCOMBOBOX::Select(hTrapTarget,0);
	}

	{
		cCOMBOBOX::Reset(hTrapSkillList1);

		cCOMBOBOX::Add(hTrapSkillList1,"valid",0xffff);

		for (i=0;i<g_trapSkillManager.m_iCount;i++)
		{
			cTrapSkill	*lpSkill	=	&g_trapSkillManager.m_aTrapSkill[i];

			cCOMBOBOX::Add(hTrapSkillList1,lpSkill->m_strName,i);
		}

		cCOMBOBOX::Select(hTrapSkillList1,0);

		cCOMBOBOX::Reset(hTrapSkillList2);

		cCOMBOBOX::Add(hTrapSkillList2,"valid",0xffff);
		for (i=0;i<g_trapSkillManager.m_iCount;i++)
		{
			cTrapSkill	*lpSkill	=	&g_trapSkillManager.m_aTrapSkill[i];

			cCOMBOBOX::Add(hTrapSkillList2,lpSkill->m_strName,i);
		}

		cCOMBOBOX::Select(hTrapSkillList2,0);
	}

	cCOMBOBOX::SelectByData(hTrapSkillList1,lpTrap->m_wTrapSkill1);
	cCOMBOBOX::SelectByData(hTrapSkillList2,lpTrap->m_wTrapSkill2);
	cCOMBOBOX::SelectByData(hTrapTarget,lpTrap->m_wTrapTarget);

	SetDlgItemInt(hDlg,IDC_TRAP_LEVEL,lpTrap->m_wTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_REGEN_TRAP_SECOND,lpTrap->m_wTrapRefreshTime,FALSE);
	SetDlgItemInt(hDlg,IDC_TRAP_DETECT_LEVEL,lpTrap->m_wDetectTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DISARM_TRAP_LEVEL,lpTrap->m_wDisarmTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_TRAP_SKILL1_CHANCE,lpTrap->m_wTrap1ActiveChance,FALSE);
	SetDlgItemInt(hDlg,IDC_TRAP_SKILL2_CHANCE,lpTrap->m_wTrap2ActiveChance,FALSE);
}

BOOL
cSET_AREA_EDIT::GetAreaData(HWND hDlg)
{
	cAREA_INFO	areaInfo;

	areaInfo.m_wSerial	=	g_lpSelectArea->m_wSerial;

	{
		HWND	hComboAreaKind	=	GetDlgItem(hDlg,IDC_AREA_KIND);

		areaInfo.m_wKind	=	cCOMBOBOX::GetSelect(hComboAreaKind);
		
		if (IsDlgButtonChecked(hDlg,IDC_IS_TEST_AREA	))
		{
			areaInfo.m_bf1IsTestPortal	=	TRUE;
			areaInfo.m_bf15LinkObject	=	0x7000;
		}
		else												
		{
			areaInfo.m_bf1IsTestPortal	=	FALSE;
			areaInfo.m_bf15LinkObject	=	0x7000;
		}

		areaInfo.m_cc.reset();

		switch(areaInfo.m_wKind)
		{
			case	dAREA_NORMAL		:
			case	dAREA_MONSTER_ACTIVE:
			case	dAREA_NAMED_AREA	:
			case	dAREA_GUILD_CREST	:
			{
				areaInfo.emptyBuffer();
				break;
			}

			case	dAREA_NOTICE_BOARD	:
			{
				char	text[2048];
				int size = GetDlgItemText(hDlg,IDC_TEXT,text,2048);
				if (size == 0) strcpy(text," ");

				areaInfo.setString(text);
				break;
			}

			case	dAREA_ARCA			:
			{
				HWND		hCombo		=	GetDlgItem(hDlg,IDC_GATE_SHAPE);

				cAreaArca	*lpArca		=	(cAreaArca	*)&areaInfo;

				lpArca->m_wLinkArca		=	cCOMBOBOX::GetData(hCombo);

				break;
			}

			case	dAREA_DOOR			:
			{
				areaInfo.emptyBuffer();

				BOOL	bIsOk					=	FALSE;
				HWND	hTrapSkillList1			=	GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL1);
				HWND	hTrapSkillList2			=	GetDlgItem(hDlg,IDC_DOOR_TRAP_SKILL2);
				HWND	hTrapTarget				=	GetDlgItem(hDlg,IDC_DOOR_TRAGET		);
				HWND	hLinkSecretDungeon		=	GetDlgItem(hDlg,IDC_DOOR_LINK_SECRET_DUNGEON);

				cAreaDoor	*lpDoor				=	(cAreaDoor	*)&areaInfo;

				lpDoor->m_wRegenPeriod			=	GetDlgItemInt(hDlg,IDC_DOOR_REGEN_PERIOD,&bIsOk,FALSE);
				lpDoor->m_wGenChance			=	GetDlgItemInt(hDlg,IDC_DOOR_REGEN_CHANCE,&bIsOk,FALSE);
				lpDoor->m_wUpkeepPeriod			=	GetDlgItemInt(hDlg,IDC_UPKEEP_DOOR_TIME,&bIsOk,FALSE);
				lpDoor->m_bSecretLevel			=	GetDlgItemInt(hDlg,IDC_DOOR_SECRET_LEVEL,&bIsOk,FALSE);
				lpDoor->m_iDurability			=	GetDlgItemInt(hDlg,IDC_DOOR_DURABILITY,&bIsOk,FALSE);
				lpDoor->m_wDestroyMinimumLevel	=	GetDlgItemInt(hDlg,IDC_DOOR_DESTROY_MIN_LEVEL,&bIsOk,FALSE);
				lpDoor->m_iDefense				=	GetDlgItemInt(hDlg,IDC_DOOR_DEFENSE,&bIsOk,FALSE);
				lpDoor->m_bUnlockLevel			=	GetDlgItemInt(hDlg,IDC_DOOR_LOCK_LEVEL,&bIsOk,FALSE);
				lpDoor->m_wUpkeepDismantlingTime=	GetDlgItemInt(hDlg,IDC_DOOR_UNLOCK_UPKEEP_TIME,&bIsOk,FALSE);
				lpDoor->m_wTrapLevel			=	GetDlgItemInt(hDlg,IDC_DOOR_TRAP_LEVEL,&bIsOk,FALSE);
				lpDoor->m_bDetectTrapLevel		=	GetDlgItemInt(hDlg,IDC_DOOR_DETECT_TRAP_LEVEL,&bIsOk,FALSE);
				lpDoor->m_bDisarmTrapLevel		=	GetDlgItemInt(hDlg,IDC_DOOR_DISARM_TRAP_LEVEL,&bIsOk,FALSE);
				lpDoor->m_wTrapSkill1ActiveChance=	GetDlgItemInt(hDlg,IDC_DOOR_TRAP_SKILL1_ACTIVE_CHANCE,&bIsOk,FALSE);
				lpDoor->m_wTrapSkill2ActiveChance=	GetDlgItemInt(hDlg,IDC_DOOR_TRAP_SKILL2_ACTIVE_CHANCE,&bIsOk,FALSE);
				
				lpDoor->m_bDefensePercent			= min(GetDlgItemInt(hDlg,IDC_DOOR_DEFENSE_PERCENT, &bIsOk, FALSE), 100);	// 09.08.13

				lpDoor->m_bf1IsDestroyable		=	IsDlgButtonChecked(hDlg,IDC_DOOR_DESTROY_ABLE);

				lpDoor->m_wTrapSkill1			=	cCOMBOBOX::GetData(hTrapSkillList1);
				lpDoor->m_wTrapSkill2			=	cCOMBOBOX::GetData(hTrapSkillList2);
				lpDoor->m_wTrapTarget			=	cCOMBOBOX::GetData(hTrapTarget);
				lpDoor->m_wLinkSecretDungeon	=	cCOMBOBOX::GetData(hLinkSecretDungeon);
				
				areaInfo.m_cc.copy(&s_cc);

				break;
			}

			case	dAREA_EVENT_AREA	:		//09.08.12
				{
					areaInfo.m_cc.copy(&s_cc);
					cAreaEvent	*lpEventArea	=	(cAreaEvent	*)&areaInfo;		
					lpEventArea->m_bf1Npc		=	IsDlgButtonChecked(hDlg,IDC_AREA_CHECK_NPC);
					// 10.02.25
					lpEventArea->m_bf1ChangeObject	=	IsDlgButtonChecked(hDlg,IDC_IS_CHANGE_OBJECT);
					
					HWND	hCombosYear		= GetDlgItem(hDlg,IDC_CB_START_YEAR);				
					
					HWND	hCombosMonth	= GetDlgItem(hDlg,IDC_CB_START_MONTH);				
					HWND	hCombosDay		= GetDlgItem(hDlg,IDC_CB_START_DAY);				
					HWND	hCombosHour		= GetDlgItem(hDlg,IDC_CB_START_HOUR);				
					HWND	hCombosMinute	= GetDlgItem(hDlg,IDC_CB_START_MINUTE);				
					
					HWND	hComboeYear		= GetDlgItem(hDlg,IDC_CB_END_YEAR);					
					HWND	hComboeMonth	= GetDlgItem(hDlg,IDC_CB_END_MONTH);				
					HWND	hComboeDay		= GetDlgItem(hDlg,IDC_CB_END_DAY);					
					HWND	hComboeHour		= GetDlgItem(hDlg,IDC_CB_END_HOUR);					
					HWND	hComboeMinute	= GetDlgItem(hDlg,IDC_CB_END_MINUTE);				
					
					HWND	hEditIndex		= GetDlgItem(hDlg,IDC_INDEX_CHANGE_OBJECT);			
					
					lpEventArea->m_bf7StartYear		= cCOMBOBOX::GetData(hCombosYear);
					
					lpEventArea->m_bf4StartMonth	= cCOMBOBOX::GetData(hCombosMonth);
					lpEventArea->m_bf5StartDay		= cCOMBOBOX::GetData(hCombosDay);
					lpEventArea->m_bf5StartHour		= cCOMBOBOX::GetData(hCombosHour);
					lpEventArea->m_bf6StartMinute	= cCOMBOBOX::GetData(hCombosMinute);
					
					lpEventArea->m_bf7EndYear		= cCOMBOBOX::GetData(hComboeYear);
					lpEventArea->m_bf4EndMonth		= cCOMBOBOX::GetData(hComboeMonth);
					lpEventArea->m_bf5EndDay		= cCOMBOBOX::GetData(hComboeDay);
					lpEventArea->m_bf5EndHour		= cCOMBOBOX::GetData(hComboeHour);
					lpEventArea->m_bf6EndMinute		= cCOMBOBOX::GetData(hComboeMinute);
					
					BOOL	bTemp;
					lpEventArea->m_iChangeObjectIndex =	GetDlgItemInt(hDlg,IDC_INDEX_CHANGE_OBJECT,&bTemp,FALSE);
					break;
				}

			case	dAREA_TRAP			:
			{
				cAreaTrap	*lpTrap			=	(cAreaTrap	*)&areaInfo;
				BOOL		bIsOk			=	FALSE;
				HWND		hTrapSkillList1	=	GetDlgItem(hDlg,IDC_TRAP_SKILL1_LIST	);
				HWND		hTrapSkillList2	=	GetDlgItem(hDlg,IDC_TRAP_SKILL2_LIST	);
				HWND		hTrapTarget		=	GetDlgItem(hDlg,IDC_TRAP_TARGET			);

				lpTrap->m_wTrapSkill1		=	cCOMBOBOX::GetData(hTrapSkillList1);
				lpTrap->m_wTrapSkill2		=	cCOMBOBOX::GetData(hTrapSkillList2);
				lpTrap->m_wTrapTarget		=	cCOMBOBOX::GetData(hTrapTarget);

				lpTrap->m_wTrapLevel		=	GetDlgItemInt(hDlg,IDC_TRAP_LEVEL,&bIsOk,FALSE);
				lpTrap->m_wTrapRefreshTime	=	GetDlgItemInt(hDlg,IDC_REGEN_TRAP_SECOND,&bIsOk,FALSE);
				lpTrap->m_wDetectTrapLevel	=	GetDlgItemInt(hDlg,IDC_TRAP_DETECT_LEVEL,&bIsOk,FALSE);
				lpTrap->m_wDisarmTrapLevel	=	GetDlgItemInt(hDlg,IDC_DISARM_TRAP_LEVEL,&bIsOk,FALSE);
				lpTrap->m_wTrap1ActiveChance=	GetDlgItemInt(hDlg,IDC_TRAP_SKILL1_CHANCE,&bIsOk,FALSE);
				lpTrap->m_wTrap2ActiveChance=	GetDlgItemInt(hDlg,IDC_TRAP_SKILL2_CHANCE,&bIsOk,FALSE);

				break;
			}
			
			case	dAREA_PORTAL	:
			{
				char	strMoveMapName[512];
				
				areaInfo.emptyBuffer();

				HWND	hDirectComboBox	=	GetDlgItem(hDlg,IDC_GATE_SHAPE	);
				HWND	hShapeComboBox	=	GetDlgItem(hDlg,IDC_DETAIL_KIND	);
				HWND	hGateCombo		=	GetDlgItem(hDlg,IDC_DIRECT2);
				HWND	hComboSeason	=	GetDlgItem(hDlg,IDC_COMBO_SEASON);
				char	*lpSelectArea	=	cCOMBOBOX::GetString(hGateCombo);
				int		iDirect			=	cCOMBOBOX::GetData(hDirectComboBox);
				int		iShape			=	cCOMBOBOX::GetData(hShapeComboBox);
				int		size			=	GetDlgItemText(hDlg,IDC_LOAD_MAP_NAME,strMoveMapName,512);
				areaInfo.m_wSeasonVariable	=	cCOMBOBOX::GetData(hComboSeason);

				areaInfo.m_gateDirect	=	iDirect;
				areaInfo.m_gateShape	=	iShape;

				if	(size	!=	0)
					areaInfo.setString(strMoveMapName);

				if	(stricmp(lpSelectArea,dMSG_NOT_EXIST_CONNECT_AREA)!=0)
					strcpy(areaInfo.m_strMoveGateName,lpSelectArea);
				else
					areaInfo.m_strMoveGateName[0]	=	NULL;

				break;
			}

			case	dAREA_START_AREA	:
			{
				areaInfo.emptyBuffer();

				int		iArea			=	g_lpActmap->m_pArea->getStartArea();

				if(iArea	!=	0xffff	&&	areaInfo.m_wSerial	!=	iArea)
					return	ERRMSG(dMSG_ERROR_ADD_AREA,dMSG_ALREADY_EXIST_START_AREA);

				break;
			}
		}

	}	//	영역 정보 얻어옴

	char name[32];

	GetDlgItemText(hDlg,IDC_NAME,name,sizeof(name));

	//	중복되는 이름의 영역이 있는지 확인
	{
		if(stricmp(name,"")==0)	
			return	ERRMSG(dMSG_ERROR_ADD_AREA,dMSG_VALID_AREA_NAME);
		
		int area	=	g_lpActmap->m_pArea->GetArea(name);
		
		if (area	!=	0xffff	&&	area	!=	g_lpSelectArea->m_wSerial)
			return	ERRMSG(dMSG_ERROR_SET_AREA,dMSG_EXIST_SAME_NAME_AREA);
		
		strcpy(g_lpSelectArea->m_strName,name);
		
	}

	g_lpSelectArea->m_wKind				=	areaInfo.m_wKind;
	g_lpSelectArea->m_wValue			=	areaInfo.m_wValue;
//	g_lpSelectArea->m_bGate				=	areaInfo.m_bGate;
	g_lpSelectArea->m_gateDirect		=	areaInfo.m_gateDirect;
	g_lpSelectArea->m_gateShape			=	areaInfo.m_gateShape;
	g_lpSelectArea->m_bf1IsTestPortal	=	areaInfo.m_bf1IsTestPortal;
	g_lpSelectArea->m_bf15LinkObject	=	areaInfo.m_bf15LinkObject;
	g_lpSelectArea->m_wSeasonVariable	=	areaInfo.m_wSeasonVariable;

	g_lpSelectArea->m_cc.copy(&areaInfo.m_cc);
	
	((cAreaEvent*)g_lpSelectArea)->m_bf1Npc =((cAreaEvent*)&areaInfo)->m_bf1Npc;		// 09.08.12 추가
	// 10.02.25
	((cAreaEvent*)g_lpSelectArea)->m_bf1ChangeObject	= ((cAreaEvent*)&areaInfo)->m_bf1ChangeObject;

	((cAreaEvent*)g_lpSelectArea)->m_bf7StartYear		= ((cAreaEvent*)&areaInfo)->m_bf7StartYear;
	((cAreaEvent*)g_lpSelectArea)->m_bf4StartMonth		= ((cAreaEvent*)&areaInfo)->m_bf4StartMonth;
	((cAreaEvent*)g_lpSelectArea)->m_bf5StartDay		= ((cAreaEvent*)&areaInfo)->m_bf5StartDay;
	((cAreaEvent*)g_lpSelectArea)->m_bf5StartHour		= ((cAreaEvent*)&areaInfo)->m_bf5StartHour;
	((cAreaEvent*)g_lpSelectArea)->m_bf6StartMinute		= ((cAreaEvent*)&areaInfo)->m_bf6StartMinute;

	((cAreaEvent*)g_lpSelectArea)->m_bf7EndYear			= ((cAreaEvent*)&areaInfo)->m_bf7EndYear;
	((cAreaEvent*)g_lpSelectArea)->m_bf4EndMonth		= ((cAreaEvent*)&areaInfo)->m_bf4EndMonth;
	((cAreaEvent*)g_lpSelectArea)->m_bf5EndDay			= ((cAreaEvent*)&areaInfo)->m_bf5EndDay;
	((cAreaEvent*)g_lpSelectArea)->m_bf5EndHour			= ((cAreaEvent*)&areaInfo)->m_bf5EndHour;
	((cAreaEvent*)g_lpSelectArea)->m_bf6EndMinute		= ((cAreaEvent*)&areaInfo)->m_bf6EndMinute;

	((cAreaEvent*)g_lpSelectArea)->m_iChangeObjectIndex	= ((cAreaEvent*)&areaInfo)->m_iChangeObjectIndex;
	switch(g_lpSelectArea->m_wKind)
	{
		case	dAREA_NOTICE_BOARD	:
			g_lpSelectArea->setString(areaInfo.m_pString);
			break;

		case	dAREA_PORTAL		:
		{
			g_lpSelectArea->setString(areaInfo.m_pString);
			strcpy(g_lpSelectArea->m_strMoveGateName,areaInfo.m_strMoveGateName);

			if	(g_lpSelectArea->m_gateShape	==	ePAS_ENTER_GUILD_HALL)
				strcpy(g_lpSelectArea->m_strName,"길드 홀 입구");

			if	(!areaInfo.m_pString)
				g_lpSelectArea->m_strMoveGateName[0]	=	NULL;

			break;
		}

		case	dAREA_DOOR			:
		{
			memcpy(g_lpSelectArea->m_strMoveGateName,areaInfo.m_strMoveGateName,sizeof(areaInfo.m_strMoveGateName));
			if	(g_lpSelectArea->m_wValue	>	1)
				g_lpSelectArea->m_wValue	=	0;

			break;
		}
		case	dAREA_TRAP			:
		case	dAREA_ARCA			:
		{
			memcpy(g_lpSelectArea->m_strMoveGateName,areaInfo.m_strMoveGateName,sizeof(areaInfo.m_strMoveGateName));
			break;
		}
		case	dAREA_START_AREA	:
		case	dAREA_OX_AREA_O		:
		case	dAREA_OX_AREA_X		:
		{
			strcpy(g_lpSelectArea->m_strName,g_strAreaKind[g_lpSelectArea->m_wKind]);
			break;
		}
	}

	g_lpActmap->LostVirgin();
	MAIN.Draw();

	return	TRUE;
}

void
cSET_AREA_EDIT::MoveArea()
{
	int x = (g_lpSelectArea->x1 + (g_lpSelectArea->x2-g_lpSelectArea->x1)/2)/dTILE_REAL_WIDTH  - g_MAPWND.TPW/2;
	int y = (g_lpSelectArea->y1 + (g_lpSelectArea->y2-g_lpSelectArea->y1)/2)/dTILE_REAL_HEIGHT - g_MAPWND.TPH/2;
	
	if (g_lpSelectArea->m_wKind	==	dAREA_RANGE)
	{
		cAREA_INFO	*lpArea	=	&g_lpActmap->m_pArea->m_aArea[g_lpSelectArea->x1];
		
		x	=	(lpArea->x1 + (lpArea->x2-lpArea->x1)/2)/dTILE_REAL_WIDTH - g_MAPWND.TPW/2;
		y	=	(lpArea->y1 + (lpArea->y2-lpArea->y1)/2)/dTILE_REAL_HEIGHT- g_MAPWND.TPH/2;
	}
	
	if (g_lpSelectArea->m_wKind	<	dAREA_HERO_RANGE)
		g_lpActmap->setPos(x,y);

	TOOLWND.Draw();
	MAIN.Draw();
}

LRESULT CALLBACK 
cSET_AREA_EDIT::s_procSetAreaEdit(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		InitAreaData(hDlg);
		break;
	case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {	
				case IDC_BT_AREASAVE:
					if(s_iAreaCount == -1)
						break;

					GetAreaData(hDlg);
					ResetAreData();
					break;
				case IDC_BT_CLOSE:
					s_bEnableDialog = FALSE;
					ShowWindow(hDlg, SW_HIDE);
					break;
				case IDC_BT_AREADELETE:
					if(s_iAreaCount == -1)
						break;

					if (g_lpActmap->m_pArea->Del())
					{
						ResetAreData();
						TOOLWND.Draw();
						g_MAPWND.Draw();
					}
					break;
				case IDC_SET_EVENT:
					{
						char	strName[64];
						
						GetDlgItemText(hDlg,IDC_NAME,strName,60);
						CConversationContainer*	lpData	=	CSetDialogSpeech::Init(hDlg,strName,&s_cc,TRUE);
						
						if (lpData)
							s_cc.copy(lpData);
					}
					break;
				case	IDC_AREA_KIND		:
					{
						switch (HIWORD(wParam))
						{
						case	CBN_SELCHANGE	:
							{
								cAREA_INFO	Area;
								
								Area.m_wKind	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_AREA_KIND));
								
								Area.resetForSpecialArea();
								
								Reset(hDlg,&Area);
								break;
							}
						}
						break;
					}
				case	IDC_LOAD_MAP	:
				{
					char	*lpstrMapName	=	cPROJECT::GetMapName(MAIN.hINST,hDlg);

					if(lpstrMapName)
					{
						HWND					hGateCombo		=	GetDlgItem(hDlg,IDC_DIRECT2	);
						cMAP::CPortalAreaInfo	aAreas[100];
						int						iCount=0,i;

						SetWindowText(GetDlgItem(hDlg,IDC_LOAD_MAP_NAME),lpstrMapName);

						cMAP::GetPortalAreas(lpstrMapName,aAreas,&iCount);

						cCOMBOBOX::Reset(hGateCombo);
						cCOMBOBOX::Add(hGateCombo,dMSG_NOT_EXIST_CONNECT_AREA,0);

						for (i=0;i<iCount;i++)
							cCOMBOBOX::Add(hGateCombo,aAreas[i].m_strName,i);

						cCOMBOBOX::Select(hGateCombo,0);
					}

					break;
				}
			}
			break;
	case WM_CLOSE:
		s_bEnableDialog = FALSE;
		ShowWindow(hDlg, SW_HIDE);
		break;
	}
	
	return (0L);
}

LRESULT CALLBACK
cSET_AREA_EDIT::s_procAreaListWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			s_iSelectAreaList = s_listArea.GetSelectData();
			g_lpActmap->m_pArea->Select(s_iSelectAreaList);
			InitAreaDataBox();		
			MAIN.Draw();
			break;
		}
	case WM_LBUTTONDBLCLK:
		s_iSelectAreaList = s_listArea.GetSelectData();
		if(s_iSelectAreaList != 0xffff)
			MoveArea();
		
		break;
	}
	return (0L);
}
