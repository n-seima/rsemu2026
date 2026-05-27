// CharacterInfo.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "CharacterInfo.h"
#include "CScript.h"
#include "cADO_ADMIN.h"
#include "afxdlgs.h"
#include "cDECODER.h"
#include "atlbase.h"
#include "CSkill.h"
#include "cITEM_PREFIX.h"
#include "cPACKET_DBSERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "cPACKET_LOGSERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "SFC.h"
#include "ExpCalc.h"
#include "io.h"
#include "CMessage.h"
#include "cPACKET_TOOLLOGSERVER.h"
#include "AdminLogCode.h"
#include "CTitle.h"
#include "questList.h"
#include "quest.h"
#include "guildInfo.h"
#include "ServerTool.h"
#include "cInventory.h"
#include "cMakeItem.h"
#include "math.h"
#include "cMiniPetList.h"
#include "cPetList.h"
#include "rebirth_define.h"
#include "mess_sign.h"
#include "skill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCharacterInfo			*g_lpCharacterInfo	=	NULL;


BOOL
CheckOperatorLevelForWrite()
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return	FALSE;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return	FALSE;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return	FALSE;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return	FALSE;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return	FALSE;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return	FALSE;
#endif

	return	TRUE;
}

//CServerTool	g_serverTool;

/////////////////////////////////////////////////////////////////////////////
// CCharacterInfo dialog

cADO_ADMIN		l_DB;
cADO_ADMIN		l_UserDB;

CCharacterInfo::CCharacterInfo(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CCharacterInfo::IDD)
{
	m_iSelectWorld	=	0;
	m_iSelectAvatar	=	0;
	m_bIsFindPlayer	=	FALSE;

	setVirgin();
	//{{AFX_DATA_INIT(CCharacterInfo)
	//}}AFX_DATA_INIT
}

void CCharacterInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharacterInfo)
	DDX_Control(pDX, IDC_INCLINATION2, m_ebInclination);
	DDX_Control(pDX, IDC_OPERATOR_SHAPE, m_cbOperatorShape);
	DDX_Control(pDX, IDC_RED_STONE_POINT, m_editRedStonePoint);
	DDX_Control(pDX, IDC_EXPLORATION_COUNT, m_ebExplorationCount);
	DDX_Control(pDX, IDC_GUILD_DUNGEON_COUNT, m_ebGuildDungeonPlayTime);
	DDX_Control(pDX, IDC_EXPAND_INVENTORY_SIZE, m_ebExpandInventorySize);
	DDX_Control(pDX, IDC_WAYPOINT_POS_Y, m_editWapointYPos);
	DDX_Control(pDX, IDC_WAYPOINT_POS_X, m_editWapointXPos);
	DDX_Control(pDX, IDC_WAY_POINT_FIELD, m_editWaypoint);
	DDX_Control(pDX, IDC_WAY_POINT_INDEX, m_cbWaypoint);
	DDX_Control(pDX, IDC_DEATH_PENELTY_TIME, m_editDeathPeneltyTime);
	DDX_Control(pDX, IDC_SCRIPT, m_editScript);
	DDX_Control(pDX, IDC_PENELTY_TIME, m_editPeneltyTime);
	DDX_Control(pDX, IDC_POSITION_TY, m_editPosTy);
	DDX_Control(pDX, IDC_POSITION_TX, m_editPosTx);
	DDX_Control(pDX, IDC_LEVEL, m_editLevel);
	DDX_Control(pDX, IDC_GOLD, m_editGold);
	DDX_Control(pDX, IDC_POSITION_Y, m_editYPos);
	DDX_Control(pDX, IDC_POSITION_X, m_editXPos);
	DDX_Control(pDX, IDC_LAST_FIELD, m_editLastField);
	DDX_Control(pDX, IDC_JOB_LIST, m_cbJobList);
	DDX_Control(pDX, IDC_NAME, m_editSelectedName);
	DDX_Control(pDX, IDC_BONUS_SKILL_POINT, m_editSkillBonus);
	DDX_Control(pDX, IDC_AVTAR_INDEX, m_editAvatarIndex);
	DDX_Control(pDX, IDC_ID2, m_editSelectedId);
	DDX_Control(pDX, IDC_CHARACTER_LIST, m_cbCharacterList);
	DDX_Control(pDX, IDC_WORLD_LIST2, m_cbWorldList);
	DDX_Control(pDX, IDC_LOAD_NAME, m_editName);
	DDX_Control(pDX, IDC_LOAD_ID, m_editId);
	DDX_Control(pDX, IDC_LOAD_DB_NAME, m_editLoadDbName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharacterInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CCharacterInfo)
	ON_BN_CLICKED(IDC_SKILL_LIST, OnClickSkillList)
	ON_BN_CLICKED(IDC_CONNECT_AND_LOAD, OnConnectAndLoad)
	ON_CBN_SELCHANGE(IDC_CHARACTER_LIST, OnSelchangeCharacterList)
	ON_BN_CLICKED(IDC_SAVE_FILE, OnSaveFile)
	ON_BN_CLICKED(IDC_LOAD_FILE, OnLoadFile)
	ON_BN_CLICKED(IDC_SAVE_TO_DB, OnSaveToDb)
	ON_BN_CLICKED(IDC_RESET_SKILL_POINT, OnResetSkillPoint)
	ON_BN_CLICKED(IDC_RESET_STATE_POINT, OnResetStatePoint)
	ON_BN_CLICKED(IDC_ADD_ITEM, OnAddItem)
	ON_BN_CLICKED(IDC_CHECK_SKILL_POINT, OnCheckSkillPoint)
	ON_BN_CLICKED(IDC_REMOVE_ITEM, OnRemoveItem)
	ON_BN_CLICKED(IDC_SAVE_ALL_TO_DB, OnSaveAllToDb)
	ON_BN_CLICKED(IDC_ITEM_LIST, OnItemList)
	ON_BN_CLICKED(IDC_RESET_LEVEL, OnResetLevel)
	ON_BN_CLICKED(IDC_SEND_TO_VILLAGE, OnSendToVillage)
	ON_BN_CLICKED(IDC_SEND_TO_JAIL, OnSendToJail)
	ON_BN_CLICKED(IDC_SEND_TO_GATE, OnSendToGate)
	ON_BN_CLICKED(IDC_SCRIPTER_LAUNCHER, OnScripterLauncher)
	ON_BN_CLICKED(IDC_DATA_CONVERT, OnDataConvert)
	ON_BN_CLICKED(IDC_BANK_DATA_CONVERT, OnBankDataConvert)
	ON_BN_CLICKED(IDC_CALCULATE_EXP, OnCalculateExp)
	ON_BN_CLICKED(IDC_RESTORE_STRANGE_FAULT, OnRestoreStrangeFault)
	ON_CBN_CLOSEUP(IDC_CHARACTER_LIST, OnCloseupCharacterList)
	ON_CBN_SELCHANGE(IDC_WORLD_LIST2, OnSelchangeWorldList2)
	ON_BN_CLICKED(IDC_CHECK_STATE_POINT, OnCheckStatePoint)
	ON_BN_CLICKED(IDC_OWN_TITLE, OnOwnTitle)
	ON_BN_CLICKED(IDC_READY_TO_CREATE_GUILD, OnReadyToCreateGuild)
	ON_CBN_SELCHANGE(IDC_WAY_POINT_INDEX, OnSelchangeWayPointIndex)
	ON_BN_CLICKED(IDC_REQUITAL_PERIOD_ITEM, OnRequitalPeriodItem)
	ON_BN_CLICKED(IDC_QUEST_LIST, OnQuestList)
	ON_BN_CLICKED(IDC_GET_GUILD_NAME, OnGetGuildName)
	ON_BN_CLICKED(IDC_RESET_JOIN_GUILD_DATE, OnResetJoinGuildDate)
	ON_BN_CLICKED(IDC_ITEM_INFO, OnItemInfo)
	ON_BN_CLICKED(IDC_MINIPET_LIST, OnMinipetList)
	ON_BN_CLICKED(IDC_PET_LIST, OnPetList)
	ON_BN_CLICKED(IDC_APPLY_DUEL_RECORD, OnApplyDuelRecord)
	ON_BN_CLICKED(IDC_GET_DUEL_RECORD, OnGetDuelRecord)
	ON_EN_CHANGE(IDC_SEAL_OF_DAWN_COUNT, OnChangeSealOfDawnCount)
	ON_EN_CHANGE(IDC_REBIRTH_COUNT, OnChangeRebirthCount)
	ON_BN_CLICKED(IDC_RESET_TRANS_GVG_DATE, OnResetTransGvgDate)
	ON_BN_CLICKED(IDC_REVISE_SP, OnReviseSp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterInfo message handlers

void CCharacterInfo::OnClickSkillList() 
{
	// TODO: Add your control notification handler code here
	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)	return;

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	char	strExportFileName[512];
	int		i,iCount=0;

	SetLogFolder();
	sprintf(strExportFileName,"SKILL List - %s[%s].txt",m_am.m_strId,sList[m_iSelectWorld].strName);

	cFILE	file;

	file.Open(strExportFileName,"wb");

	cPLAYER_DATA	*lpPlayer	=	(cPLAYER_DATA *)&m_am.m_aData[m_iSelectAvatar];

	for (i=0;i<dABILITY_COUNT;i++)
	{
		CAbilityDefine	*lpAbility	=	&lpPlayer->m_aAbility[i];

		if (lpAbility->m_wSkill	==	0xffff)	continue;

		file.writeStreamCR("%s[Lv %d]",g_aSkill[lpAbility->m_wSkill].m_strName,lpAbility->m_wLevel);
	}

	file.Close();

	ShellExecute(NULL,"open",strExportFileName,NULL,NULL,SW_SHOWNORMAL);

	SetLogFolder();
}

BOOL CCharacterInfo::OnInitDialog() 
{
#ifndef _FOR_KOREA
	SetWindowText(g_strDlgText[3]);
	SetDlgItemText(IDC_STATIC1,g_strDlgText[4]);
	SetDlgItemText(IDC_STATIC2,g_strDlgText[5]);
	SetDlgItemText(IDC_STATIC5,g_strDlgText[11]);
	SetDlgItemText(IDC_STATIC6,g_strDlgText[12]);
	SetDlgItemText(IDC_STATIC7,g_strDlgText[13]);
	SetDlgItemText(IDC_CONNECT_AND_LOAD,g_strDlgText[14]);
	SetDlgItemText(IDC_STATIC8,g_strDlgText[13]);
	SetDlgItemText(IDC_SAVE_FILE,g_strDlgText[15]);
	SetDlgItemText(IDC_LOAD_FILE,g_strDlgText[16]);
	SetDlgItemText(IDC_STATIC9,g_strDlgText[17]);
	SetDlgItemText(IDC_STATIC10,g_strDlgText[18]);
	SetDlgItemText(IDC_STATIC11,g_strDlgText[19]);
	SetDlgItemText(IDC_STATIC12,g_strDlgText[20]);
	SetDlgItemText(IDC_STATIC13,g_strDlgText[21]);
	SetDlgItemText(IDC_STATIC14,g_strDlgText[22]);
	SetDlgItemText(IDC_STATIC15,g_strDlgText[23]);
	SetDlgItemText(IDC_STATIC16,g_strDlgText[24]);
	SetDlgItemText(IDC_STATIC17,g_strDlgText[25]);
	SetDlgItemText(IDC_STATIC18,g_strDlgText[26]);
	SetDlgItemText(IDC_STATIC19,g_strDlgText[27]);
	SetDlgItemText(IDC_STATIC20,g_strDlgText[28]);
	SetDlgItemText(IDC_STATIC21,g_strDlgText[29]);
	SetDlgItemText(IDC_STATIC22,g_strDlgText[30]);

	SetDlgItemText(IDC_STATIC47,"Gold bar");
	SetDlgItemText(IDC_STATIC58,"Gold bar in Bank");

	//	SetDlgItemText(IDC_STATIC23,g_strDlgText[31]);
	SetDlgItemText(IDC_STATIC24,g_strDlgText[31]);
	SetDlgItemText(IDC_STATIC25,g_strDlgText[32]);
//	SetDlgItemText(IDC_STATIC26,g_strDlgText[34]);
	SetDlgItemText(IDC_STATIC27,g_strDlgText[33]);
	SetDlgItemText(IDC_STATIC28,g_strDlgText[34]);
	SetDlgItemText(IDC_STATIC29,g_strDlgText[35]);
	SetDlgItemText(IDC_STATIC30,g_strDlgText[36]);
	SetDlgItemText(IDC_STATIC31,g_strDlgText[37]);
	SetDlgItemText(IDC_STATIC32,g_strDlgText[38]);
	SetDlgItemText(IDC_STATIC33,g_strDlgText[39]);
	SetDlgItemText(IDC_STATIC34,g_strDlgText[40]);
	SetDlgItemText(IDC_STATIC35,g_strDlgText[41]);
	SetDlgItemText(IDC_STATIC36,g_strDlgText[42]);
	SetDlgItemText(IDC_SAVE_TO_DB,g_strDlgText[43]);
	SetDlgItemText(IDC_SAVE_ALL_TO_DB,g_strDlgText[44]);
	SetDlgItemText(IDC_ITEM_LIST,g_strDlgText[47]);
	SetDlgItemText(IDC_REMOVE_ITEM,g_strDlgText[48]);
	SetDlgItemText(IDC_ADD_ITEM,g_strDlgText[49]);
	SetDlgItemText(IDC_OWN_TITLE,g_strDlgText[50]);
	SetDlgItemText(IDC_REQUITAL_PERIOD_ITEM,g_strDlgText[166]);
	SetDlgItemText(IDC_SKILL_LIST,g_strDlgText[51]);
	SetDlgItemText(IDC_RESET_SKILL_POINT,g_strDlgText[52]);
	SetDlgItemText(IDC_RESET_STATE_POINT,g_strDlgText[53]);
	SetDlgItemText(IDC_CHECK_SKILL_POINT,g_strDlgText[54]);
	SetDlgItemText(IDC_SEND_TO_VILLAGE,g_strDlgText[55]);
	SetDlgItemText(IDC_SEND_TO_JAIL,g_strDlgText[56]);
	SetDlgItemText(IDC_SEND_TO_GATE,g_strDlgText[57]);
	SetDlgItemText(IDC_STATIC37,g_strDlgText[157]);
	SetDlgItemText(IDC_STATIC23,g_strDlgText[158]);
	SetDlgItemText(IDC_READY_TO_CREATE_GUILD,g_strDlgText[159]);
	SetDlgItemText(IDC_STATIC40,g_strDlgText[160]);
	SetDlgItemText(IDC_STATIC39,g_strDlgText[161]);
	SetDlgItemText(IDC_STATIC38,g_strDlgText[162]);
	SetDlgItemText(IDC_RESET_LEVEL,g_strDlgText[163]);
	SetDlgItemText(IDC_STATIC200,g_strDlgText[165]);

	SetDlgItemText(IDC_STATIC_DAY,g_strDlgText[167]);
	SetDlgItemText(IDC_STATIC_HOUR,g_strDlgText[168]);
	SetDlgItemText(IDC_STATIC_MINUTE,g_strDlgText[169]);

#endif

	CPropertyPage::OnInitDialog();

	memset(&m_am,0,sizeof(m_am));
	SetLogFolder();
	CreateDirectory("export",NULL);
	SetLogFolder();
	CreateDirectory("Recover Item",NULL);
	SetLogFolder();
	CreateDirectory("Delete Item",NULL);
	SetLogFolder();
	CreateDirectory("Item List",NULL);
	SetLogFolder();
	CreateDirectory("Title",NULL);
	SetLogFolder();
	int		i;
/*
	cScript			scp;

	char fn[2048];
	sprintf(fn,"%s\\ReadMe.txt",g_strLogFolder);
	scp.Read(fn);
	m_iWorldCount	=	atoi(scp.getData("WORLD_COUNT"));

	char	name[dNAME_LENGTH];

	for(i=0;i<m_iWorldCount;i++)
	{
		sprintf(name,"WORLD%d",i);
		strcpy(sList[i].strName,scp.getData(name));
		if(strcmp(sList[i].strName,"")==0)
		{
			sList[i].isConnect = FALSE;
			memset(&sList[i],0,sizeof(cServerList));
			continue;
		}
		strcpy(sList[i].strIP,scp.getData(name,2));
		strcpy(sList[i].strDBIP,scp.getData(name,3));
		sList[i].wGameServerCount = atoi(scp.getData(name,4));
		for(int j=0;j<sList[i].wGameServerCount;j++)
		{
			char gsname[dNAME_LENGTH];
			sprintf(gsname,"W%dGAME%d",i,j);
			strcpy(sList[i].cGS[j].strGameIP,scp.getData(gsname));
		}
	}
*/
	SetDlgItemText(IDC_LOAD_DB_NAME,"RedStone_Avatar_Source");

	m_iWorldCount = g_iWorldCount;
	// TODO: Add extra initialization here
	for (i=0;i<m_iWorldCount;i++)
	{
		if(g_lWhereisServer== eCOUNTRY_INNERTEST)
		{
			if (sList[i].isTestServer	!=	2)
				continue;
		}
		else
		{
			if (sList[i].isTestServer	==	2)
				continue;
		}

		char	strName[50];

		sprintf(strName,"%d] %s",i,sList[i].strName);

		m_cbWorldList.AddString(strName);
	}

	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		m_iSelectWorld	=	0;

	m_cbWorldList.SetCurSel(m_iSelectWorld);

	CRegKey		reg;
	BOOL		bIsReadRegistryFailed	=	FALSE;

	int iResult	=	reg.Open( g_hRegKey, g_lpstrRegistry);

	if (iResult == ERROR_SUCCESS)
	{
		DWORD	count	=	sizeof(m_strDataFolder);
		if (reg.QueryValue((LPTSTR )m_strDataFolder,"Path",&count) == ERROR_SUCCESS)
		{	
			if (count	<=	0)	bIsReadRegistryFailed	=	TRUE;
		}
		else	bIsReadRegistryFailed	=	TRUE;
	}
	else	bIsReadRegistryFailed	=	TRUE;

	if (!SetCurrentDirectory(m_strDataFolder))
		bIsReadRegistryFailed	=	TRUE;

	if (bIsReadRegistryFailed)
	{
		MessageBox(g_strToolMessage[94]);
		return	FALSE;
	}

//	cMSG::Put("game folder",m_strDataFolder);

	LoadSkillData();

	SetCurrentDirectory(g_strLogFolder);

	cITEM::ReadItemData();
	SetCurrentDirectory(g_strLogFolder);

	g_titleManager.init();
	SetCurrentDirectory(g_strLogFolder);
	CQuest::Load();
	SetCurrentDirectory(g_strLogFolder);

	return TRUE;  // return TRUE unless you set the focus to a control
}

int CCharacterInfo::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::DoModal();
}

void CCharacterInfo::OnOK() 
{
	// TODO: Add extra validation here
	if	(!isVirgin())
		if	(cMSG::YESNO("WARNING!!",g_strToolMessage[98])==IDNO)
			return;
	
	CPropertyPage::OnCancel();
}

void CCharacterInfo::OnConnectAndLoad() 
{
	if	(!isVirgin())
		if	(cMSG::YESNO("WARNING!!",g_strToolMessage[98])==IDNO)
			return;

#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_TESTER))
		return;
#elif	_FOR_USA
	if	(m_bIsFindPlayer	==	FALSE	&&	!CheckOperatorLevel(eAL_TESTER))
		return;
#elif	_FOR_INTER
	if	(m_bIsFindPlayer	==	FALSE	&&	!CheckOperatorLevel(eAL_TESTER))
		return;
#else
	if	(m_bIsFindPlayer	==	FALSE	&&	!CheckOperatorLevel(eAL_TESTER))
		return;
#endif

//	m_iSelectWorld	=	m_cbWorldList.GetCurSel();
	CString	str;
	m_cbWorldList.GetWindowText(str);
	m_iSelectWorld	=	atoi(str);

	if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));
	
	if	(stricmp(strDbName,"RedStone_Avatar_Source") == 0)
		l_DB.m_bIsNormalDB	=	TRUE;
	else
		l_DB.m_bIsNormalDB	=	FALSE;	

	m_editName.GetWindowText(m_am.m_strName,sizeof(m_am.m_strName));
	m_editId.GetWindowText(m_am.m_strId,sizeof(m_am.m_strId));

	char	*lpstrId	=	NULL;
	char	*lpstrName	=	NULL;

	if	(m_am.m_strName[0]	)
		lpstrName	=	m_am.m_strName;
	if	(m_am.m_strId[0]	)
		lpstrId		=	m_am.m_strId;

	BOOL	isTestServer = FALSE;

// 	if	(strcmp(sList[m_iSelectWorld].strDBIP,"211.189.40.164")==0)
// 		isTestServer = TRUE;

	{
		_log(" ip [%s] , id [%s], pw [%s], name [%s]",sList[m_iSelectWorld].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName);
		if	(!l_DB.Connect(	sList[m_iSelectWorld].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
		{
			MessageBox(g_strToolMessage[50]);
			return;
		}
	}

	{
		if	(m_bIsFindPlayer)
		{
			if	(!l_DB.loadData(NULL,NULL,&m_am,m_iFindPlayerCount,m_iFindJob,m_iFindLevel))
			{
				l_DB.Close();
				return;
			}
		}
		else
		if	(!l_DB.loadData(lpstrId,lpstrName,&m_am))
		{
			l_DB.Close();
			return;
		}
		
		m_am.checkIncorrectItemData();
	}

	m_dwLogExp				=	m_am.m_aData[0].m_iExperience;
	m_dwLogLevel			=	m_am.m_aData[0].m_iLevel;
	m_dwLastVillage			=	m_am.m_aData[0].m_wLastVillage;
	m_dwLastField			=	m_am.m_aData[0].m_wCurrentField;

	l_DB.Close();

	if	(m_am.m_iLoadDataCount	<=	0)
		MessageBox(g_strToolMessage[99]);
	else
	{
		strcpy(m_am.m_strId,m_am.m_aData[0].m_strId);

		if	(lpstrId)
			g_toolLog.loadPlayerData(m_am.m_strId,"",sList[m_iSelectWorld].strName);
		else
		if	(lpstrName)
			g_toolLog.loadPlayerData(m_am.m_strId,lpstrName,sList[m_iSelectWorld].strName);
		else
		{
			for	(int i=0;i<m_am.m_iLoadDataCount;i++)
				g_toolLog.loadPlayerData(m_am.m_aData[i].m_strId,m_am.m_aData[i].m_strName,sList[m_iSelectWorld].strName);
		}

		checkData();
	}

	memset(m_am.m_abIsLoadData,0,sizeof(m_am.m_abIsLoadData));

	if	(!g_DBAccount.spGetPvPScore(m_am.m_strId,&m_am.m_duelRecord))
		memset(&m_am.m_duelRecord,0,sizeof(m_am.m_duelRecord));

	cDuelRecord	*lpRecord			=	&m_am.m_duelRecord;

	SetDlgItemInt(IDC_DUEL_POINT	,lpRecord->m_iDuelPoint);

	SetDlgItemInt(IDC_TOTAL_BC		,lpRecord->m_iBattleCount);
	SetDlgItemInt(IDC_TOTAL_WIN_C	,lpRecord->m_iWinCount);
	SetDlgItemInt(IDC_TOTAL_LOSE_C	,lpRecord->m_iDefeatCount);
	SetDlgItemInt(IDC_TOTAL_DRAW_C	,lpRecord->m_iDrawCount);

	SetDlgItemInt(IDC_WEEKLY_BC		,lpRecord->m_iWeeklyBattleCount);
	SetDlgItemInt(IDC_WEEKLY_WIN_C	,lpRecord->m_iWeeklyWinCount);
	SetDlgItemInt(IDC_WEEKLY_LOSE_C	,lpRecord->m_iWeeklyDefeatCount);
	SetDlgItemInt(IDC_WEEKLY_DRAW_C	,lpRecord->m_iWeeklyDrawCount);
	SetDlgItemInt(IDC_WINNING_POINT	,lpRecord->m_iWeeklyWinPoint);

	// TODO: Add your control notification handler code here
	m_iSelectAvatar	=	0;

	//m_bf8ModifyDataCount;

	resetCharacterInfo();

	setVirgin();
}

void
CCharacterInfo::checkData()
{
	BOOL	bIsIncorrectTitleInfo=	FALSE,bIsIncorrectBankItemInfo=FALSE,bIsIncorrectItemData=FALSE;
	int		i,j;

	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		cITEM *lpItem	=	(cITEM *)&m_am.m_bank.m_aItems[0];

		if (lpItem->isIncorrectItem())
		{
			if (bIsIncorrectBankItemInfo	==	FALSE)
			{
				cMSG::Put("",g_strToolMessage[133]);
				m_am.m_bIsChangeBankData	=	TRUE;
			}

			bIsIncorrectBankItemInfo	=	TRUE;
		}
	}

	for (int iPlayer=0;iPlayer<m_am.m_iLoadDataCount;iPlayer++)
	{
		CAvatarData	*lpPlayer	=	&m_am.m_aData[iPlayer];

		if (lpPlayer->m_bf6ExtraInventorySize	>	dEXTRA_INVENTORY_SIZE)
		{
			lpPlayer->m_bf6ExtraInventorySize	=	0;

			for (j=0;j<dEXTRA_INVENTORY_SIZE;j++)
				((cITEM *)&lpPlayer->m_aExtraInventory[j])->reset();
		}


		WORD		awIncorrectItem[1000];
		int			iIncorrectItemCount=0;

		memset(awIncorrectItem,0xff,sizeof(awIncorrectItem));

		bIsIncorrectTitleInfo	=	FALSE;
		bIsIncorrectItemData	=	FALSE;

		for (j=0;j<dMAX_PLAYER_TITLE_COUNT;j++)
			if (lpPlayer->m_aTitle[j].m_bLevel	==	0)
			{
				lpPlayer->m_aTitle[j].m_bTitle	=	0xff;
				lpPlayer->m_aTitle[j].m_bLevel	=	0xff;

				bIsIncorrectTitleInfo			=	TRUE;
			}

		for (j=0;j<dOWN_ITEM_COUNT;j++)
		{
			cITEM	*lpItem	=	(cITEM *)&lpPlayer->m_aItems[j];

			if (lpItem->isIncorrectItem())
			{
				awIncorrectItem[iIncorrectItemCount++]	=	j;
				bIsIncorrectItemData	=	TRUE;
			}
		}

		for (j=0;j<dEQUIPMENT_PART_COUNT;j++)
		{
			cITEM	*lpItem	=	(cITEM *)&lpPlayer->m_aEquip[j];

			if (lpItem->isIncorrectItem())
			{
				awIncorrectItem[iIncorrectItemCount++]	=	j+100;
				bIsIncorrectItemData	=	TRUE;
			}
		}

		if (bIsIncorrectTitleInfo)
			cMSG::Put("",g_strToolMessage[125],lpPlayer->m_strName);

		if (bIsIncorrectItemData)
		{
			cSTRING	strText;

			strText.Set(g_strToolMessage[134],lpPlayer->m_strName);

			strText.Add("\n");

			for (int ii=0;ii<iIncorrectItemCount;ii++)
			{
				int		iItem	=	awIncorrectItem[ii];
				cITEM	*lpItem;

				if (iItem	<	100)
					lpItem	=	(cITEM *)&lpPlayer->m_aItems[iItem];
				else
					lpItem	=	(cITEM *)&lpPlayer->m_aEquip[iItem-100];

				strText.Add("\n%s",lpItem->getName());
			}

			cMSG::Put("",strText.String);

			lostVirgin();
		}

		if (lpPlayer->m_iCP	<=	-1000000)
		{
			lpPlayer->m_iCP				=	0;

			cMSG::Put("",g_strToolMessage[135],lpPlayer->m_strName);
			lostVirgin();
		}

		for (int i=0;i<dABILITY_COUNT;i++)
		{
			CAbilityDefine	*lpAbility	=	&lpPlayer->m_aAbility[i];

			if	(lpAbility->m_wSkill	>=	0xffff)
				continue;

			if (lpAbility->m_wSkill	>=	dMAX_SKILL)
			{
				lpAbility->m_wSkill	=	0xffff;
				lpAbility->m_wLevel	=	0;

				ERRMSG(g_strToolMessage[100],g_strToolMessage[101],i);

				continue;
			}

			CSkillDefine	*lpSkill	=	&g_aSkill[lpAbility->m_wSkill];
			int				iEnableJob	=	lpSkill->m_wEnableJob;
			int				iJob=lpPlayer->m_wJob,iAnotherJob=	lpPlayer->m_wJob/2*2+(1-(lpPlayer->m_wJob%2));

			if	(iEnableJob	!=	0xffff && iEnableJob != dJOB_CODE_ALL)
				if (iEnableJob	!=	iJob && iEnableJob	!=	iAnotherJob)
				{
					ERRMSG(g_strToolMessage[100],g_strToolMessage[102],i);

					lpAbility->m_wSkill	=	0xffff;
					lpAbility->m_wLevel	=	0;
					continue;
				}

			if	(lpAbility->m_wLevel > 50)
			{
				ERRMSG(g_strToolMessage[100],g_strToolMessage[103],i,lpAbility->m_wLevel);

				lpAbility->m_wSkill	=	0xffff;
				lpAbility->m_wLevel	=	0;
				continue;
			}
		}
	}
}

void
CCharacterInfo::resetCharacterInfo()
{
	int	i;
	
	m_cbCharacterList.ResetContent();

	for (i=0;i<m_am.m_iLoadDataCount;i++)	
		m_cbCharacterList.AddString(m_am.m_aData[i].m_strName);

	m_cbCharacterList.SetCurSel(m_iSelectAvatar);

	m_cbWaypoint.ResetContent();

	for (i=0;i<dMAX_PLAYER_WAY_POINT_COUNT;i++)	
		m_cbWaypoint.AddString(_ms("%d",i));

	m_cbWaypoint.SetCurSel(0);

	fillData();
}

void CCharacterInfo::OnSelchangeCharacterList() 
{
	if	(!isVirgin())
		if	(cMSG::YESNO("WARNING!!",g_strToolMessage[98])==IDNO)
		{
			m_cbCharacterList.SetCurSel(m_iSelectAvatar);
			return;
		}

	m_iSelectAvatar	=	m_cbCharacterList.GetCurSel();

	fillData();
	// TODO: Add your control notification handler code here
}

void
CCharacterInfo::fillData()
{
	int	i;

	if	(m_am.m_iLoadDataCount	<=	0)	
		return;

	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];
	
	m_editSelectedId.SetWindowText(lpPlayer->m_strId);
	m_editSelectedName.SetWindowText(lpPlayer->m_strName);

//	m_editGuildRank.SetWindowText(
	
	SetDlgItemInt(IDC_INCLINATION2,lpPlayer->m_sAllignment,TRUE);
	SetDlgItemInt(IDC_EXPLORATION_COUNT,lpPlayer->m_bf4SecretDungeonPlayCount,FALSE);
	SetDlgItemInt(IDC_GUILD_DUNGEON_COUNT,lpPlayer->m_bf3GuildDungeonPlayTime,FALSE);
	SetDlgItemInt(IDC_AVTAR_INDEX	,lpPlayer->m_wAvatarIndex,FALSE);
	SetDlgItemInt(IDC_LEVEL	,lpPlayer->m_iLevel,FALSE);
	SetDlgItemInt(IDC_EXPERIENCE,lpPlayer->m_iExperience,FALSE);
	SetDlgItemInt(IDC_REBIRTH_COUNT,lpPlayer->m_bf4RebirthCount,FALSE);
	SetDlgItemInt(IDC_SEAL_OF_DAWN_COUNT,lpPlayer->m_bf4UseSealSphereOfDawnCount,FALSE);

	SetDlgItemInt(IDC_LEVEL_POINT,lpPlayer->m_wLevelPoint,FALSE);
	SetDlgItemInt(IDC_GOLD,lpPlayer->m_iGold,FALSE);
	SetDlgItemInt(IDC_BANK_GOLD	,m_am.m_bank.m_dwGold,FALSE);
	SetDlgItemInt(IDC_GOLD_BAR,lpPlayer->getItemCount(dITEM_INDEX_GOLD_BAR),FALSE);
	SetDlgItemInt(IDC_BANK_GOLD_BAR,m_am.getItemInBankCount(dITEM_INDEX_GOLD_BAR),FALSE);
	SetDlgItemInt(IDC_SKILL_POINT,lpPlayer->m_dwSkillExperience,FALSE);
	SetDlgItemInt(IDC_BONUS_SKILL_POINT,lpPlayer->m_wBonusSkillPoint,FALSE);
	SetDlgItemInt(IDC_LAST_VILLAGE,lpPlayer->m_wLastVillage,FALSE);
	SetDlgItemInt(IDC_EXPAND_INVENTORY_SIZE,lpPlayer->m_bf6ExtraInventorySize,FALSE);
	SetDlgItemInt(IDC_LAST_FIELD,lpPlayer->m_wCurrentField,FALSE);
	SetDlgItemInt(IDC_RED_STONE_POINT,lpPlayer->m_wRedStonePoint,FALSE);
	SetDlgItemInt(IDC_POSITION_X,lpPlayer->m_iXPos,TRUE);
	SetDlgItemInt(IDC_POSITION_Y,lpPlayer->m_iYPos,TRUE);
	SetDlgItemInt(IDC_POSITION_TX,lpPlayer->m_iXPos/64,TRUE);
	SetDlgItemInt(IDC_POSITION_TY,lpPlayer->m_iYPos/32,TRUE);
//	SetDlgItemInt(IDC_GUILD			,lpPlayer->,FALSE);
//	SetDlgItemInt(IDC_GUILD_RANK	,lpPlayer->,FALSE);
	SetDlgItemInt(IDC_SAVE_COUNT	,lpPlayer->m_dwSaveCount,FALSE);
	SetDlgItemInt(IDC_PENELTY_TIME	,lpPlayer->m_dwPeneltyTime,FALSE);

	SetDlgItemInt(IDC_EXTEND_DAY	,0,FALSE);
	SetDlgItemInt(IDC_EXTEND_HOUR	,0,FALSE);
	SetDlgItemInt(IDC_EXTEND_MINUTE	,0,FALSE);

	SetDlgItemInt(IDC_DEATH_PENELTY_TIME,lpPlayer->m_bf12DeathPeneltySecond,FALSE);
	SetDlgItemInt(IDC_DEATH_PENELTY_TIME,lpPlayer->m_bf12DeathPeneltySecond,FALSE);
	SetDlgItemInt(IDC_EVENT_VALUE		,lpPlayer->m_wRequitalCounter,FALSE);
	SetDlgItemInt(IDC_PLAY_TIME_HOUR	,(lpPlayer->m_dwPlayTime/60)%24,FALSE);

	SetDlgItemInt(IDC_BOOST_DAMAGE,lpPlayer->m_wIncreaseFinalDamage,FALSE);
	SetDlgItemInt(IDC_BOOST_DAMAGE_TIME,lpPlayer->m_wIncreaseFinalDamageTime,FALSE);

	SetDlgItemText(IDC_TRANS_GVG_DATE_TEXT,
					_ms("%d/%d/%d",lpPlayer->m_bf6LastTransAvatarToGVGYear,lpPlayer->m_bf4LastTransAvatarToGVGMonth,lpPlayer->m_bf5LastTransAvatarToGVGDay)
					);

	m_cbJobList.ResetContent();

	for (i=0;i<dPLAYER_JOB_COUNT;i++)	
		m_cbJobList.AddString(g_strJobName[i]);

	m_cbJobList.SetCurSel(lpPlayer->m_wJob);

	int	iSelect	=	m_cbWaypoint.GetCurSel();

	if	(lpPlayer->m_aWayPoint[iSelect].m_bf12Field	==	4095)
	{
		SetDlgItemInt(IDC_WAY_POINT_FIELD,-1,TRUE);
		SetDlgItemInt(IDC_WAYPOINT_POS_X,-1,TRUE);
		SetDlgItemInt(IDC_WAYPOINT_POS_Y,-1,TRUE);
	}
	else
	{
		SetDlgItemInt(IDC_WAY_POINT_FIELD,lpPlayer->m_aWayPoint[iSelect].m_bf12Field,FALSE);
		SetDlgItemInt(IDC_WAYPOINT_POS_X,lpPlayer->m_aWayPoint[iSelect].m_bf10XPos,FALSE);
		SetDlgItemInt(IDC_WAYPOINT_POS_Y,lpPlayer->m_aWayPoint[iSelect].m_bf10YPos,FALSE);
	}

	int	iGuildRank	=	lpPlayer->m_wGuildRank;

	if	(iGuildRank	==	65535)
		iGuildRank	=	0;

	SetDlgItemText(IDC_GUILD_RANK		,g_strToolMessage[126+iGuildRank]);
	SetDlgItemInt(IDC_GUILD				,lpPlayer->m_wGuildSerial,FALSE);
	SetDlgItemInt(IDC_GUILD_JOIN_YEAR	,lpPlayer->m_timeGuildJoinTime.year,FALSE);
	SetDlgItemInt(IDC_GUILD_JOIN_MONTH	,lpPlayer->m_timeGuildJoinTime.month,FALSE);
	SetDlgItemInt(IDC_GUILD_JOIN_DAY	,lpPlayer->m_timeGuildJoinTime.day,FALSE);

	CheckDlgButton(IDC_READY_TO_CREATE_GUILD,lpPlayer->m_bf1IsReadyToMakeGuild);
	CheckDlgButton(IDC_READY_TO_SELECT_GUILD_MARK,lpPlayer->m_bf1IsReadyToSelectGuildMark);

	setVirgin();
}

void CCharacterInfo::OnSaveFile() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(extLevel	!=	eAL_ONLY_SEARCH	&&	!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(extLevel	!=	eAL_ONLY_SEARCH	&&	!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(extLevel	!=	eAL_ONLY_SEARCH	&&	!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(m_am.m_iLoadDataCount	<=	0)
		return;

	// TODO: Add your control notification handler code here
	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	FILE	*fp;

	char	strFileName[256];
	char	dir[512];
	char	strName[256]	=	"";

	sprintf(dir,"%s\\export",g_strLogFolder);

	SetCurrentDirectory(dir);

	DWORD	dwBankLog	=	0x12345678;

	{

		sprintf(strFileName,"%s - %s[%d].export",sList[m_iSelectWorld].strName,lpPlayer->m_strId,lpPlayer->m_wAvatarIndex);
		fp	=	fopen(strFileName,"wb");
		fwrite(lpPlayer,sizeof(CAvatarData),1,fp);
		fwrite(&dwBankLog,sizeof(DWORD),1,fp);
		fwrite(&m_am.m_bank,sizeof(m_am.m_bank),1,fp);

		fclose(fp);
	}

	{
		fp	=	fopen("___________lastest export data.export","wb");
		fwrite(lpPlayer,sizeof(CAvatarData),1,fp);
		fwrite(&dwBankLog,sizeof(DWORD),1,fp);
		fwrite(&m_am.m_bank,sizeof(m_am.m_bank),1,fp);
		fclose(fp);
	}

	SetCurrentDirectory(g_strLogFolder);

	char	strMessage[512];

	sprintf(strMessage,g_strToolMessage[104],strFileName);
	MessageBox(strMessage,"SUCCESS!!",MB_OK);
}

void CCharacterInfo::OnLoadFile() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(m_am.m_iLoadDataCount	<=	0)
		return;

	if	(!isVirgin())
		if (cMSG::YESNO("WARNING!!",g_strToolMessage[98])==IDNO)
			return;

	CAvatarData		saveData,tempData;
	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	char	dir[512];
	char	strName[256]	=	"";

	sprintf(dir,"%s\\export",g_strLogFolder);

	SetCurrentDirectory(dir);

	if	(!cFILE::LoadBox(strName,"player export data {*.export}\0*.export\0\0","export",dir,"load user export data"))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	FILE	*fp;

	fp	=	fopen(strName,"rb");

	fread(&saveData,sizeof(saveData),1,fp);

	DWORD		dwBankLog;
	CBankInfo	bank;

	fread(&dwBankLog,sizeof(DWORD),1,fp);

	if	(dwBankLog	==	0x12345678)
		fread(&bank,sizeof(bank),1,fp);

	fclose(fp);

	if	(!g_toolLog.dataCopy(lpPlayer->m_strId,lpPlayer->m_strName,saveData.m_strId,saveData.m_strName,sList[m_iSelectWorld].strName))
		return;

	m_am.checkIncorrectItemData();

	m_am.m_bIsChangeBankData	=	TRUE;
	memcpy(&tempData,lpPlayer,sizeof(tempData));
	memcpy(lpPlayer,&saveData,sizeof(saveData));
	memcpy(&m_am.m_bank,&bank,sizeof(bank));

	strcpy(m_am.m_bank.m_strId	,tempData.m_strId);
	strcpy(lpPlayer->m_strId	,tempData.m_strId);
	strcpy(lpPlayer->m_strName	,tempData.m_strName);

	lpPlayer->m_wAvatarIndex	=	tempData.m_wAvatarIndex;
	lpPlayer->m_dwSaveCount		=	tempData.m_dwSaveCount+1;
	lpPlayer->m_wGuildSerial	=	tempData.m_wGuildSerial	;
	lpPlayer->m_wGuildRank		=	tempData.m_wGuildRank	;

	fillData();

	SetCurrentDirectory(g_strLogFolder);

	m_am.m_abIsLoadData[m_iSelectAvatar]	=	TRUE;
}

void CCharacterInfo::OnSaveToDb() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;
	if	(m_am.m_iLoadDataCount	<=	0)
		return;

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];
	char			strDbName[256];

	lpPlayer->m_dwSaveCount++;

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	m_editName.GetWindowText(m_am.m_strName,sizeof(m_am.m_strName));
	m_editId.GetWindowText(m_am.m_strId,sizeof(m_am.m_strId));

	cPOINT	posOld;
	cPOINT	posMapOld,posMap;
	char	strText[100];

	posOld.x	=	lpPlayer->m_iXPos;
	posOld.y	=	lpPlayer->m_iYPos;
	posMapOld.x	=	lpPlayer->m_iXPos/64;
	posMapOld.y	=	lpPlayer->m_iYPos/32;

	m_editAvatarIndex.GetWindowText(strText,sizeof(strText));

	if	(atoi(strText)<0 || atoi(strText)>dMAX_AVATAR_COUNT)
	{
		MessageBox("Avatar Index is incorrect.");
		return;	
	}

	char	*lpstrWorldName	=	sList[m_iSelectWorld].strName;

	switch(g_iToolType)
	{
		case	eTOOL_FOR_TEST			:
			if	(stricmp(lpstrWorldName,"소룬드")	!=	0)
			{
				MessageBox("테스트 버젼 어드민툴입니다. 소룬드 서버 캐릭터만 가능합니다.");
				return;
			}
			break;
		case	eTOOL_FOR_BALANCE		:
			if	(strstr(lpstrWorldName,"내부 테섭")	==	NULL && strstr(lpstrWorldName,"내부테섭")	==	NULL)
			{
				MessageBox("기획팀 버젼 어드민툴입니다. 기획팀 서버 캐릭터만 가능합니다.");
				return;
			}
			break;
		case	eTOOL_FOR_INTERNATIONAL	:
			if	(stricmp(lpstrWorldName,"인터네셔날 서버용")	!=	0)
			{
				MessageBox("인터네셔날 버젼 어드민툴입니다. 조심해서 사용하세요");
				return;
			}
			break;
	}

//	아바타 인덱스 변경?
	{
		int	iAvatarIndex		=	atoi(strText);

		if	(lpPlayer->m_wAvatarIndex	!=	iAvatarIndex)
		{
			if	(!g_toolLog.changeAvatarIndex(lpPlayer->m_strId,lpPlayer->m_strName,lpstrWorldName,lpPlayer->m_wAvatarIndex,iAvatarIndex))
				return;

			lpPlayer->m_wAvatarIndex	=	iAvatarIndex;
		}
	}

	m_editXPos.GetWindowText(strText,sizeof(strText));
	lpPlayer->m_iXPos		=	atoi(strText);
	m_editYPos.GetWindowText(strText,sizeof(strText));
	lpPlayer->m_iYPos		=	atoi(strText);

	//	전생 처리
	{
		int	iRebirthCount		=	GetDlgItemInt(IDC_REBIRTH_COUNT);
		int	iSealOfDawnCount	=	GetDlgItemInt(IDC_SEAL_OF_DAWN_COUNT);

		iRebirthCount			=	min(iRebirthCount,dLIMIT_REBIRTH_COUNT);
		iRebirthCount			=	max(iRebirthCount,0);

		iSealOfDawnCount		=	min(iSealOfDawnCount,iRebirthCount);
		iSealOfDawnCount		=	max(iSealOfDawnCount,0);

		lpPlayer->m_bf4RebirthCount				=	iRebirthCount;
		lpPlayer->m_bf4UseSealSphereOfDawnCount	=	iSealOfDawnCount;
	}

//	마지막 필드 위치 변경
	{
		m_editLastField.GetWindowText(strText,sizeof(strText));
		int		iCurrentField		=	atoi(strText);

		if	(lpPlayer->m_wCurrentField	!=	iCurrentField)
		{
			if	(!g_toolLog.changeCurrentField(lpPlayer->m_strId,lpPlayer->m_strName,lpstrWorldName,lpPlayer->m_wCurrentField,iCurrentField))
				return;

			lpPlayer->m_wCurrentField	=	iCurrentField;
			lpPlayer->m_iXPos	=	-1;
			lpPlayer->m_iYPos	=	-1;
		}
	}

//	길드 생성 실패
	{
		lpPlayer->m_bf1IsReadyToMakeGuild		=	IsDlgButtonChecked(IDC_READY_TO_CREATE_GUILD);
		lpPlayer->m_bf1IsReadyToSelectGuildMark	=	IsDlgButtonChecked(IDC_READY_TO_SELECT_GUILD_MARK);
	}

//	붉은 보석 포인트
	{
		m_editRedStonePoint.GetWindowText(strText,sizeof(strText));
		int	iPoint	=	atoi(strText);

		if	(lpPlayer->m_wRedStonePoint	!=	iPoint)
			lpPlayer->m_wRedStonePoint	=	iPoint;
	}
	
//	비밀 던젼 탐험 횟수 변경
	{
		m_ebExplorationCount.GetWindowText(strText,sizeof(strText));
		int	iSecretDungeonPlayCount	=	min(atoi(strText),15);
		iSecretDungeonPlayCount		=	max(iSecretDungeonPlayCount,0);

		if	(lpPlayer->m_bf4SecretDungeonPlayCount	!=	(DWORD)iSecretDungeonPlayCount)
		{
			if	(!g_toolLog.changeSecretDungeonPlayCount(lpPlayer->m_strId,lpPlayer->m_strName,lpstrWorldName,lpPlayer->m_bf4SecretDungeonPlayCount,iSecretDungeonPlayCount))
				return;

			lpPlayer->m_bf4SecretDungeonPlayCount	=	iSecretDungeonPlayCount;
		}
	}

//	길드 던젼 탐험 횟수 변경
	{
		m_ebGuildDungeonPlayTime.GetWindowText(strText,sizeof(strText));
		int	iGuildDungeonPlayCount	=	min(atoi(strText),15);
		iGuildDungeonPlayCount		=	max(iGuildDungeonPlayCount,0);
		
		if	(lpPlayer->m_bf3GuildDungeonPlayTime	!=	(DWORD)iGuildDungeonPlayCount)
		{
			if	(!g_toolLog.changeSecretDungeonPlayCount(lpPlayer->m_strId,lpPlayer->m_strName,lpstrWorldName,lpPlayer->m_bf4SecretDungeonPlayCount,iGuildDungeonPlayCount))
				return;
			
			lpPlayer->m_bf3GuildDungeonPlayTime	=	iGuildDungeonPlayCount;
		}
	}

	{
		m_ebInclination.GetWindowText(strText,sizeof(strText));

		int	iValue	=	atoi(strText);

		iValue		=	max(iValue,-100);
		iValue		=	min(iValue,100);

		lpPlayer->m_sAllignment	=	iValue;
	}

//	마법 배낭 수 변경
	{
		m_ebExpandInventorySize.GetWindowText(strText,sizeof(strText));
		int	iExtraInventorySize		=	min(atoi(strText),dEXTRA_INVENTORY_SIZE);

		if	(lpPlayer->m_bf6ExtraInventorySize	!=	iExtraInventorySize)
		{
			if	(!g_toolLog.changeExtraInventorySize(lpPlayer->m_strId,lpPlayer->m_strName,lpstrWorldName,lpPlayer->m_bf6ExtraInventorySize,iExtraInventorySize))
				return;

			lpPlayer->m_bf6ExtraInventorySize	=	iExtraInventorySize;
		}
	}

	{
		m_editPeneltyTime.GetWindowText(strText,sizeof(strText));
		DWORD	dwPeneltyTime	=	atoi(strText);

		if	(lpPlayer->m_dwPeneltyTime	!=	dwPeneltyTime)
		{
			if	(!g_toolLog.changePeneltyTime(lpPlayer->m_strId,lpPlayer->m_strName,lpstrWorldName,lpPlayer->m_dwPeneltyTime,dwPeneltyTime))
				return;

			lpPlayer->m_dwPeneltyTime	=	dwPeneltyTime;
		}
	}

	{
		lpPlayer->m_wIncreaseFinalDamage	=	GetDlgItemInt(IDC_BOOST_DAMAGE);
		lpPlayer->m_wIncreaseFinalDamageTime=	GetDlgItemInt(IDC_BOOST_DAMAGE_TIME);
	}

	{
		m_editDeathPeneltyTime.GetWindowText(strText,sizeof(strText));
		int		iDeathPeneltyTime	=	atoi(strText);
		
		if	(lpPlayer->m_bf12DeathPeneltySecond	!=	(DWORD)iDeathPeneltyTime)
		{
			if	(!g_toolLog.changeDeathPeneltyTime(lpPlayer->m_strId,lpPlayer->m_strName,lpstrWorldName,lpPlayer->m_bf12DeathPeneltySecond,iDeathPeneltyTime))
				return;

			lpPlayer->m_bf12DeathPeneltySecond	=	iDeathPeneltyTime;
		}
	}

	m_editPosTx.GetWindowText(strText,sizeof(strText));
	posMap.x				=	atoi(strText);
	m_editPosTy.GetWindowText(strText,sizeof(strText));
	posMap.y				=	atoi(strText);

	if (posMapOld.x != posMap.x || posMapOld.y != posMap.y)
	{
		lpPlayer->m_iXPos	=	posMap.x*64+32;
		lpPlayer->m_iYPos	=	posMap.y*32+16;
	}

	char	l_strSelectServer[32];
	char	*lpstrName		=	NULL;

	m_cbWorldList.GetWindowText(l_strSelectServer,32);

	if	(m_am.m_strName[0])
		lpstrName	=	m_am.m_strName;

	BOOL	isTestServer = FALSE;

	if	(sList[m_iSelectWorld].isTestServer)
		isTestServer = TRUE;

	{
		if	(!l_DB.Connect(	sList[m_iSelectWorld].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
		{
			MessageBox(g_strToolMessage[50]);
			return;
		}
	}

//	lpPlayer->m_wGuildSerial = 891;
//	lpPlayer->m_wGuildRank =2;
	if	(!g_toolLog.savePlayer(lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
		return;

	if	(!l_DB.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer,isTestServer,TRUE))
	{
		g_toolLog.failedLog();
		l_DB.Close();
		return;
	}

	if	(m_am.m_abIsLoadData[m_iSelectAvatar])
	{
		if(MessageBox("Do you wanna set this user for operator?","Check!!",MB_YESNO)==IDYES)
		{
			l_DB.spSetOper(lpPlayer->m_strName,eAL_TESTER);
		}
		m_am.m_abIsLoadData[m_iSelectAvatar]	=	FALSE;
	}

	setVirgin();

	if	(m_am.m_bIsChangeBankData || strcmp(m_am.m_bank.m_strId,"")==0)
	{
		strcpy(m_am.m_bank.m_strId,lpPlayer->m_strId);

		if	(!g_toolLog.saveBankData(lpPlayer->m_strId,sList[m_iSelectWorld].strName))
			return;
		
		l_DB.spSaveBank(&m_am.m_bank);
		m_am.m_bIsChangeBankData	=	FALSE;
	}

	l_DB.Close();
	_log(g_strToolMessage[107],strUserName,lpPlayer->m_strId,lpPlayer->m_strName);
	fillData();
	MessageBox("Success Save !!");
}

void CCharacterInfo::OnMoveToGate() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))	
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	// TODO: Add your control notification handler code here
	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)	return;
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	lpPlayer->m_iXPos	=	-1;
	lpPlayer->m_iYPos	=	-1;

	fillData();

	lostVirgin();
}

void CCharacterInfo::OnMoveToJail() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)	return;
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	lpPlayer->m_wCurrentField	=	27;
	lpPlayer->m_iXPos			=	-1;
	lpPlayer->m_iYPos			=	-1;

	fillData();
	// TODO: Add your control notification handler code here
	lostVirgin();
}

void CCharacterInfo::OnMoveToVillage() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif


	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)	return;
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	lpPlayer->m_wCurrentField	=	0;
	lpPlayer->m_iXPos			=	-1;
	lpPlayer->m_iYPos			=	-1;

	fillData();
	// TODO: Add your control notification handler code here
}

void CCharacterInfo::OnResetSkillPoint() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	CString	str;
	m_cbWorldList.GetWindowText(str);
	m_iSelectWorld	=	atoi(str);
	
	if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;
	if	(m_am.m_iLoadDataCount	<=	0)
		return;

	if	(!g_toolLog.resetSkillPoint(m_am.m_aData[m_iSelectAvatar].m_strId,m_am.m_aData[m_iSelectAvatar].m_strName,sList[m_iSelectWorld].strName))
		return;

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	((cPLAYER_DATA *)lpPlayer)->resetSkillPoint();
	_log(g_strToolMessage[108],lpPlayer->m_strId,lpPlayer->m_strName);
	fillData();

	lostVirgin();
}


void CCharacterInfo::OnResetStatePoint() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	if	(!g_toolLog.resetStatePoint(m_am.m_aData[m_iSelectAvatar].m_strId,m_am.m_aData[m_iSelectAvatar].m_strName,sList[m_iSelectWorld].strName))
		return;

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	((cPLAYER_DATA *)lpPlayer)->resetStatePoint();

	fillData();
	
	_log(g_strToolMessage[109],lpPlayer->m_strId,lpPlayer->m_strName);
	lostVirgin();
}

void CCharacterInfo::OnCheckSkillPoint() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#else
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#endif

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	int	iCurrentSkillPoint	=	lpPlayer->getCurrentSkillPointSum();
	int	iCorrectSkillPoint	=	lpPlayer->getCurrentSkillPointByLevel();
	int	iCurrentStatePoint	=	lpPlayer->getCurrentStatePointSum();
	int	iCorrectStatePoint	=	lpPlayer->getCurrentStatePointSumByLevel();

	char	strMessage[256];

	sprintf(strMessage,g_strToolMessage[124],iCurrentSkillPoint,iCorrectSkillPoint,iCurrentStatePoint,iCorrectStatePoint);

	MessageBox(strMessage);
}

int
CCharacterInfo::removeGold(DWORD _dwSerial)
{
	int		iRemoveGold = 0;

	for (int i=0;i<m_am.m_iLoadDataCount;i++)
	{
		CAvatarData	*lpPlayer	=	&m_am.m_aData[i];

		if	((int)_dwSerial		>	lpPlayer->m_iGold)
		{	
			if	(!g_toolLog.removeGoldFromPlayer(lpPlayer->m_iGold,lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
				return	-1;
				
			iRemoveGold			+=	lpPlayer->m_iGold;
			lpPlayer->m_iGold	=	0;
			_dwSerial			-=	iRemoveGold;
		}
		else
		{
			if	(!g_toolLog.removeGoldFromPlayer(_dwSerial,lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
				return	-1;

			lpPlayer->m_iGold	-=	_dwSerial;
			iRemoveGold			+=	_dwSerial;
			break;
		}
	}

	return	iRemoveGold;
}

void CCharacterInfo::OnAddItem() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData		*lpPlayer		=	&m_am.m_aData[m_iSelectAvatar];

	if	(!g_toolLog.workMessage(m_am.m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName,"============= try add item list ============="))
		return;

	cDECODER		decoder;
	OPENFILENAME	ofn;

	char			dir[512];
	char			strName[256]	=	"";
	char			strMessage[256];

	sprintf(dir,"%s\\ITEM RECOVERY",g_strLogFolder);

	SetCurrentDirectory(dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.txt";
	ofn.lpstrTitle		=	"ITEM RECOVERY";
	ofn.lpstrFile		=	strName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"txt";
	ofn.lpstrInitialDir	=	dir;

	if	(!GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	if	(!decoder.Upload(strName,NULL))
		return;

	int			iProcessIndex	=	0,iCount=0;
	cITEM		item;
	DWORD		dwRecoverGold = 0;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		if (stricmp(lpstrWord,"IT")	==	0 || stricmp(lpstrWord,"item")	==	0)
		{
			item.reset();

			for (iProcessIndex =0;iProcessIndex<22;iProcessIndex++)
			{
				BOOL	bIsSuccess;
				DWORD	dwValue		=	decoder.GetNumber(&bIsSuccess);

				if	(!bIsSuccess)
					break;

				if	(iProcessIndex	==	0)
					item.m_wBaseItem				=	(WORD)dwValue;
				if	(iProcessIndex	==	1)
				{
					item.m_dwSerial	=	(DWORD)dwValue;

					if	(item.m_dwSerial	<=	1)
						item.m_dwSerial		=	largeRandom(0x7fffffff);
				}
				if (iProcessIndex	==	2)	item.m_bCount					=	(BYTE)dwValue;
				if (iProcessIndex	==	3)	item.m_bDurability				=	(BYTE)dwValue;

				if (iProcessIndex	==	4)	item.m_aOption[0]				=	(BYTE)dwValue;
				if (iProcessIndex	==	5)	item.m_aOption[1]				=	(BYTE)dwValue;

				if (iProcessIndex	==	6)	item.m_aPrefix[0].m_wPrefix		=	(WORD)dwValue;
				if (iProcessIndex	==	7)	item.m_aPrefix[0].m_aValue[0]	=	(BYTE)dwValue;
				if (iProcessIndex	==	8)	item.m_aPrefix[0].m_aValue[1]	=	(BYTE)dwValue;

				if (iProcessIndex	==	9)	item.m_aPrefix[1].m_wPrefix		=	(WORD)dwValue;
				if (iProcessIndex	==	10)	item.m_aPrefix[1].m_aValue[0]	=	(BYTE)dwValue;
				if (iProcessIndex	==	11)	item.m_aPrefix[1].m_aValue[1]	=	(BYTE)dwValue;

				if (iProcessIndex	==	12)	item.m_aPrefix[2].m_wPrefix		=	(WORD)dwValue;
				if (iProcessIndex	==	13)	item.m_aPrefix[2].m_aValue[0]	=	(BYTE)dwValue;
				if (iProcessIndex	==	14)	item.m_aPrefix[2].m_aValue[1]	=	(BYTE)dwValue;

				if (iProcessIndex	==	15)	item.setBitFieldValue((DWORD)dwValue);
			}

			BOOL	bIsAddToBank=	FALSE;

			CItemDefine	*lpSlot	=	lpPlayer->getValidItem();

			if	(!lpSlot)
			{
				_log(g_strToolMessage[137],strUserName,m_am.m_strId,iCount,dwRecoverGold);

				sprintf(strMessage,g_strToolMessage[138],iCount,dwRecoverGold);

				MessageBox(strMessage);

				return;
			}

			if	(m_am.getItem(item.m_wBaseItem,item.m_dwSerial) || m_am.getItemInBank(item.m_wBaseItem,item.m_dwSerial))
			{
				sprintf(strMessage,g_strToolMessage[139],item.m_wBaseItem,item.m_dwSerial);

				MessageBox(strMessage);
				return;
			}

			if	(!g_toolLog.addItemToPlayer(&item,lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
				return;

			memcpy(lpSlot,&item,sizeof(item));
			lostVirgin();

			iCount++;

			continue;
		}

		if (stricmp(lpstrWord,"GD")	==	0 || stricmp(lpstrWord,"Gold")	==	0|| stricmp(lpstrWord,"골드")	==	0)
		{
			BOOL	bIsSuccess;
			DWORD	dwValue	=	decoder.GetNumber(&bIsSuccess);

			if (!bIsSuccess)
			{
				ERRMSG("ERROR ITEM RECOVERY!!","Incorrect gold data");

				break;
			}

			if	(lpPlayer->m_iGold	+	dwValue > 200000000)
			{
				ERRMSG("ERROR ITEM RECOVERY!!",dMSG_TOO_ADD_MANY_GOLD_FOR_PLAYER);

				break;				
			}

			if	(!g_toolLog.addGoldToPlayer(dwValue,lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
				return;

			dwRecoverGold		+=	dwValue;
			lpPlayer->m_iGold	+=	dwValue;
			lostVirgin();
			continue;
		}

		if (stricmp(lpstrWord,"isEndCode")	==	0)
			break;
	}

	if	(!g_toolLog.workMessage(m_am.m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName,"============= end add item list ============="))
		return;

	_log(g_strToolMessage[140],strUserName,m_am.m_strId,iCount,dwRecoverGold);

	sprintf(strMessage,g_strToolMessage[142],m_am.m_strId,iCount,dwRecoverGold);

	MessageBox(strMessage);
}

void CCharacterInfo::OnRemoveItem() 
{
#ifndef _FOR_KOREA
	return;
#endif
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	// TODO: Add your control notification handler code here
	if	(m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	cDECODER	decoder;

	OPENFILENAME	ofn;

	char	dir[512];
	char	strName[256]	=	"";

	sprintf(dir,"%s\\Item Recovery",g_strLogFolder);

	SetCurrentDirectory(dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.txt";
	ofn.lpstrTitle		=	"Item Deletion";
	ofn.lpstrFile		=	strName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"txt";
	ofn.lpstrInitialDir	=	dir;

	if	(!GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	if (!decoder.Upload(strName,NULL))	return;

	int	iProcessIndex=0,iCount=0,iRemoveItemCount=0;

	cITEM		item;
	cFILE		file;

	char	strFileName[512];

	sprintf(dir,"%s\\ITEM RECOVERY",g_strLogFolder);

	SetCurrentDirectory(dir);

	sprintf(strFileName,"__REMOVE - %s",_exportFileName(strName));

	file.Open(strFileName,"wb",TRUE);

	SetCurrentDirectory(g_strLogFolder);               

	LONGLONG	llRemoveGold	= 0;	//	로그에 기록할 빼앗은 골드 

	if	(!g_toolLog.workMessage(m_am.m_strId,"",sList[m_iSelectWorld].strName,"============= try remove item list ============="))
		return;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		if (stricmp(lpstrWord,"IT")	==	0 || stricmp(lpstrWord,"item")	==	0)
		{
			item.reset();

			for (iProcessIndex =0;iProcessIndex<22;iProcessIndex++)
			{
				BOOL	bIsSuccess;
				DWORD	dwValue		=	decoder.GetNumber(&bIsSuccess);

				if (!bIsSuccess)	break;

				if (iProcessIndex	==	0)	item.m_wBaseItem				=	(WORD)dwValue;
				if (iProcessIndex	==	1)	item.m_dwSerial					=	(DWORD)dwValue;

				if (iProcessIndex	==	2)	item.m_bCount					=	(BYTE)dwValue;
				if (iProcessIndex	==	3)	item.m_bDurability				=	(BYTE)dwValue;

				if (iProcessIndex	==	4)	item.m_aOption[0]				=	(BYTE)dwValue;
				if (iProcessIndex	==	5)	item.m_aOption[1]				=	(BYTE)dwValue;

				if (iProcessIndex	==	6)	item.m_aPrefix[0].m_wPrefix		=	(WORD)dwValue;
				if (iProcessIndex	==	7)	item.m_aPrefix[0].m_aValue[0]	=	(BYTE)dwValue;
				if (iProcessIndex	==	8)	item.m_aPrefix[0].m_aValue[1]	=	(BYTE)dwValue;

				if (iProcessIndex	==	9)	item.m_aPrefix[1].m_wPrefix		=	(WORD)dwValue;
				if (iProcessIndex	==	10)	item.m_aPrefix[1].m_aValue[0]	=	(BYTE)dwValue;
				if (iProcessIndex	==	11)	item.m_aPrefix[1].m_aValue[1]	=	(BYTE)dwValue;

				if (iProcessIndex	==	12)	item.m_aPrefix[2].m_wPrefix		=	(WORD)dwValue;
				if (iProcessIndex	==	13)	item.m_aPrefix[2].m_aValue[0]	=	(BYTE)dwValue;
				if (iProcessIndex	==	14)	item.m_aPrefix[2].m_aValue[1]	=	(BYTE)dwValue;

				if (iProcessIndex	==	15)	item.setBitFieldValue((DWORD)dwValue);
			}

			iCount++;

			BOOL	bIsBankItem		=	FALSE;
			char	strOwner[dNAME_LENGTH];

			CItemDefine	*lpRemoveItem=	m_am.getItem(item.m_wBaseItem,item.m_dwSerial,strOwner);

			if (!lpRemoveItem)
			{
				lpRemoveItem	=	m_am.getItemInBank(item.m_wBaseItem,item.m_dwSerial);

				if	(lpRemoveItem)
				{
					m_am.m_bIsChangeBankData	=	TRUE;
					bIsBankItem					=	TRUE;
				}
			}

			if (lpRemoveItem)
			{
				file.writeStream("item [%d:%d] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d       [%.2d/%.2d/%.2d %.2d:%.2d]\n",
									lpRemoveItem->m_wBaseItem,lpRemoveItem->m_dwSerial,lpRemoveItem->m_bCount,lpRemoveItem->m_bDurability,
									lpRemoveItem->m_aOption[0],lpRemoveItem->m_aOption[1],
									lpRemoveItem->m_aPrefix[0].m_wPrefix,lpRemoveItem->m_aPrefix[0].m_aValue[0],lpRemoveItem->m_aPrefix[0].m_aValue[1],
									lpRemoveItem->m_aPrefix[1].m_wPrefix,lpRemoveItem->m_aPrefix[1].m_aValue[0],lpRemoveItem->m_aPrefix[1].m_aValue[1],
									lpRemoveItem->m_aPrefix[2].m_wPrefix,lpRemoveItem->m_aPrefix[2].m_aValue[0],lpRemoveItem->m_aPrefix[2].m_aValue[1],
									lpRemoveItem->getBitFieldValue()			,
									lpRemoveItem->m_year						,
									lpRemoveItem->m_month						,
									lpRemoveItem->m_day							,
									lpRemoveItem->m_hour						,
									lpRemoveItem->m_minute	);

				if	(bIsBankItem)
				{
					if	(!g_toolLog.removeItemFromBank(lpRemoveItem,m_am.m_strId,sList[m_iSelectWorld].strName))
						return;
				}
				else
				{
					if	(!g_toolLog.removeItemFromPlayer(lpRemoveItem,m_am.m_strId,strOwner,sList[m_iSelectWorld].strName))
						return;
				}

				lpRemoveItem->reset();
				iRemoveItemCount++;
				continue;
			}

		}

		if	(stricmp(lpstrWord,"gold")	==	0 || stricmp(lpstrWord,"GD")	==	0 || stricmp(lpstrWord,"골드")	==	0 )
		{
			BOOL	bIsSuccess;
			DWORD	dwValue	=	decoder.GetNumber(&bIsSuccess);

			if	(!bIsSuccess)
			{
				ERRMSG("ERROR ITEM DELETION!!","Incorrect Gold Data");

				break;
			}

			int iRemoveGold		=	removeGold(dwValue);

			if	(iRemoveGold	==	-1	||	iRemoveGold	>	200000000)
				return;

			DWORD	dwRemoveGold=	0;
			
			dwRemoveGold		+=	iRemoveGold; 
			llRemoveGold		+=	iRemoveGold; 

			if	((int)dwValue	>	iRemoveGold)
			{
				DWORD	dwRemainGoldToRemove	=	dwValue	-	iRemoveGold;

				if	(m_am.m_bIsInvalidBankData	==	FALSE)
				{
					if	(dwRemainGoldToRemove	>	m_am.m_bank.m_dwGold)
					{
						if	(!g_toolLog.removeGoldFromBank(m_am.m_bank.m_dwGold,m_am.m_strId,sList[m_iSelectWorld].strName))
							return;

						dwRemoveGold			+=	m_am.m_bank.m_dwGold;
						m_am.m_bank.m_dwGold	=	0;
						m_am.m_bIsChangeBankData=	TRUE;
					}
					else
					{
						if	(!g_toolLog.removeGoldFromBank(dwValue,m_am.m_strId,sList[m_iSelectWorld].strName))
							return;

						dwRemoveGold			+=	dwRemainGoldToRemove;
						m_am.m_bank.m_dwGold	-=	dwRemainGoldToRemove;
						m_am.m_bIsChangeBankData=	TRUE;
					}
				}
			}

			if	(iRemoveGold	>	0)
				file.writeStream("gold [%ud]\n",dwRemoveGold);

			if	((int)dwValue	>	dwRemoveGold)
				file.writeStream("// CURRENT GOLD : [%d]Gold , FINDED GOLD : [%ud]Gold\n",dwValue,dwRemoveGold);
			continue;
		}

		if	(stricmp(lpstrWord,"isEndCode")	==	0)
			break;
	}

	if	(!g_toolLog.workMessage(m_am.m_strId,"",sList[m_iSelectWorld].strName,"============= end remove item list ============="))
		return;

	file.Close();

	lostVirgin();

	_log(g_strToolMessage[141],strUserName,m_am.m_strId,iCount,iRemoveItemCount,llRemoveGold);
	char	strMessage[256];

	sprintf(strMessage,g_strToolMessage[142],iCount,iRemoveItemCount,llRemoveGold,strFileName);

	MessageBox(strMessage);
}

void CCharacterInfo::OnSaveAllToDb()
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	
		return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	
		return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	
		return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	if	(m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	BOOL	isTestServer = FALSE;

// 	if(strcmp(sList[m_iSelectWorld].strDBIP,"211.189.40.164")==0)	
// 		isTestServer = TRUE;

	if	(!l_DB.Connect(	sList[m_iSelectWorld].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
	{
		MessageBox(g_strToolMessage[50]);
		return;
	}

	for (int i=0;i<m_am.m_iLoadDataCount;i++)
	{
		CAvatarData		*lpPlayer	=	&m_am.m_aData[i];

		lpPlayer->m_dwSaveCount++;

		if	(!g_toolLog.savePlayer(lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
			return;

		if	(!l_DB.saveData((cP_PLAYER_SAVE_DATA *)lpPlayer))
		{
			g_toolLog.failedLog();
			l_DB.Close();
			return;
		}

		if	(m_am.m_abIsLoadData[i])
		{
			m_am.m_abIsLoadData[i]	=	FALSE;
			l_DB.spSetOper(lpPlayer->m_strName,eAL_TESTER);
		}

		_log(g_strToolMessage[107],strUserName,lpPlayer->m_strId,lpPlayer->m_strName);
	}

	setVirgin();

	if	(m_am.m_bIsChangeBankData)
	{
		if	(!g_toolLog.saveBankData(m_am.m_strId,sList[m_iSelectWorld].strName))
			return;
		l_DB.spSaveBank(&m_am.m_bank);
		m_am.m_bIsChangeBankData	=	FALSE;
	}

	l_DB.Close();


	fillData();

	MessageBox("Success All Avatar !!");
}

void CCharacterInfo::OnItemList() 
{
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	cInventory	inventory;

	inventory.init(&m_am,m_iSelectAvatar,m_iSelectWorld);
	
	if	(inventory.DoModal()	==	IDCANCEL)
		return;
}

void CCharacterInfo::OnResetLevel() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	char	l_strSelectServer[32];
	m_cbWorldList.GetWindowText(l_strSelectServer,32);
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	char	strText[256];
	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];
	m_editLevel.GetWindowText(strText,sizeof(strText));

	int	iLevel	=	max(min(atoi(strText),999),1);

	if	(!g_toolLog.changeLevel(lpPlayer->m_strId,lpPlayer->m_strName,m_dwLogLevel,iLevel,sList[m_iSelectWorld].strName))
		return;

	lpPlayer->m_iLevel		=	max(min(atoi(strText),999),1);
	lpPlayer->m_iExperience	=	0;
	lpPlayer->resetSkillPoint();
	lpPlayer->resetStatePoint();
	fillData();
	// TODO: Add your control notification handler code here
	_log(g_strToolMessage[123],lpPlayer->m_strId,lpPlayer->m_strName,lpPlayer->m_iLevel);
}

void CCharacterInfo::OnSendToVillage() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData	*lpPlayer		=	&m_am.m_aData[m_iSelectAvatar];

	if	(!g_toolLog.moveTo(lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName,0))
		return;

	lpPlayer->m_wCurrentField	=	lpPlayer->m_wLastVillage;
	lpPlayer->m_iXPos			=	-1;
	lpPlayer->m_iYPos			=	-1;

	SetDlgItemInt(IDC_POSITION_X,lpPlayer->m_iXPos,TRUE);
	SetDlgItemInt(IDC_POSITION_Y,lpPlayer->m_iYPos,TRUE);
	SetDlgItemInt(IDC_POSITION_TX,lpPlayer->m_iXPos/64,TRUE);
	SetDlgItemInt(IDC_POSITION_TY,lpPlayer->m_iYPos/32,TRUE);
	SetDlgItemInt(IDC_LAST_FIELD,lpPlayer->m_wCurrentField,FALSE);
}

void CCharacterInfo::OnSendToJail() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData	*lpPlayer		=	&m_am.m_aData[m_iSelectAvatar];

	if	(!g_toolLog.moveTo(lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName,1))
		return;

	lpPlayer->m_wCurrentField	=	27;
	lpPlayer->m_iXPos			=	-1;
	lpPlayer->m_iYPos			=	-1;

	SetDlgItemInt(IDC_POSITION_X,lpPlayer->m_iXPos,TRUE);
	SetDlgItemInt(IDC_POSITION_Y,lpPlayer->m_iYPos,TRUE);
	SetDlgItemInt(IDC_POSITION_TX,lpPlayer->m_iXPos/64,TRUE);
	SetDlgItemInt(IDC_POSITION_TY,lpPlayer->m_iYPos/32,TRUE);
	SetDlgItemInt(IDC_LAST_FIELD,lpPlayer->m_wCurrentField,FALSE);
}

void CCharacterInfo::OnSendToGate() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData	*lpPlayer		=	&m_am.m_aData[m_iSelectAvatar];

	if	(!g_toolLog.moveTo(lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName,2))
		return;

	lpPlayer->m_iXPos			=	-1;
	lpPlayer->m_iYPos			=	-1;

	SetDlgItemInt(IDC_POSITION_X,lpPlayer->m_iXPos,TRUE);
	SetDlgItemInt(IDC_POSITION_Y,lpPlayer->m_iYPos,TRUE);
	SetDlgItemInt(IDC_POSITION_TX,lpPlayer->m_iXPos/64,TRUE);
	SetDlgItemInt(IDC_POSITION_TY,lpPlayer->m_iYPos/32,TRUE);
}


void CCharacterInfo::OnBankDataConvert()
{
	return ;
}

void CCharacterInfo::OnDataConvert() 
{
	return ;
}

void CCharacterInfo::OnScripterLauncher() 
{
	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];
	char		*lpstrKeyWord;	
	char		strScript[1024];
	BOOL		bIsFindPlayer	=	FALSE;

	m_editScript.GetWindowText(strScript,sizeof(strScript));

	cDECODER	decoder;

	if	(!decoder.UploadBuffer(strScript,NULL))
	{
		goto	error_label;
		return;
	}

	lpstrKeyWord	=	decoder.GetWord();

#ifdef 	_FOR_CHINA
	return;
#endif

	if	(stricmp(lpstrKeyWord,"findPlayer")	==	0	||	stricmp(lpstrKeyWord,"getGuildSkill")	==	0)
		bIsFindPlayer	=	TRUE;

	if	(bIsFindPlayer	==	FALSE	&&	m_am.m_iLoadDataCount	<=	0)
		goto	error_label;

#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(bIsFindPlayer	==	FALSE	&&	!CheckOperatorLevel(eAL_OPERATOR))
		goto	error_label;
#elif	_FOR_INTER
	if	(bIsFindPlayer	==	FALSE	&&	!CheckOperatorLevel(eAL_OPERATOR))
		goto	error_label;
#else
	if	(bIsFindPlayer	==	FALSE	&&	!CheckOperatorLevel(eAL_OPERATOR))
		goto	error_label;
#endif

	decoder.rewind();

	while(1)
	{
		lpstrKeyWord	=	decoder.GetWord();

		if	(stricmp(lpstrKeyWord,"getGuildSkill")	==	0)
		{
			char	*lpstrGuild	=	decoder.GetStream();
		//	m_iSelectWorld	=	m_cbWorldList.GetCurSel();
			CString	str;
			m_cbWorldList.GetWindowText(str);
			m_iSelectWorld	=	atoi(str);

			if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
				return;

			char	strDbName[256];

			m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));
			
			if	(stricmp(strDbName,"RedStone_Avatar_Source") == 0)
				l_DB.m_bIsNormalDB	=	TRUE;
			else
				l_DB.m_bIsNormalDB	=	FALSE;	

			if	(!l_DB.Connect(	sList[m_iSelectWorld].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
			{
				MessageBox(g_strToolMessage[50]);
				return;
			}

			CGuildSaveInfoSkill	gSkill;

			l_DB.spGetGuildSkillData(lpstrGuild,&gSkill);

			g_gsManager.loadData();

			SetCurrentDirectory(g_strLogFolder);

			cFILE	file;

			file.Open("길드스킬.txt","wt");

	
			for(int i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
			{
				if	(gSkill.m_aGuildSkill[i])
				{
					CGuildSkillDefine	*lpGuildSkill	=	g_gsManager.get(i);

					if	(!lpGuildSkill)
						ERRMSG("에러!","길드 스킬 없음");
					else
					{
						file.writeStream("%s : %d\n",lpGuildSkill->m_strName,gSkill.m_aGuildSkill[i]);
					}
				}
			}

			file.Close();

			return;
		}


		if	(stricmp(lpstrKeyWord,"findPlayer")	==	0)
		{
			char	*lpstrJob	=	decoder.GetStream();
			int		iJob		=	-1;
			
			for (int i=0;i<dPLAYER_JOB_COUNT;i++)	
			{
				if	(stricmp(g_strJobName[i],lpstrJob)	==	0)
				{
					iJob	=	i;
					break;
				}
			}

			if	(iJob	==	-1)
			{
				cMSG::Put("error!!","'%s' 라는 직업은 없습니다. 띄어 쓰기 없이 입력해 주세요.",lpstrJob);
				break;
			}

			BOOL	bIsOK;
			int		iLevel		=	decoder.GetNumber(&bIsOK);

			if	(bIsOK	==	FALSE)
			{
				cMSG::Put("error!!","레벨을 입력해 주세요.");
				break;
			}

			if	 (iLevel	<	10	||	iLevel	>	999)
			{
				cMSG::Put("error!!","레벨 수치는 10~999로 입력해 주세요.");
				break;
			}

			m_bIsFindPlayer		=	TRUE;
			m_iFindJob			=	iJob;
			m_iFindLevel		=	iLevel;
			m_iFindPlayerCount	=	dMAX_AVATAR_DATA_COUNT;

			OnConnectAndLoad();

			m_bIsFindPlayer	=	FALSE;
			
			return;
		}

		if	(stricmp(lpstrKeyWord,"setQuestStatus")	==	0)
		{
			BOOL	bTemp;
			int		iQuestIndex		=	decoder.GetNumber(&bTemp);

			if	(!bTemp)
			{
				cMSG::Put("error","ex> setQuestStatus [questIndex] [0 or 1]");
				return;
			}
			int		iOnOff			=	decoder.GetNumber(&bTemp);

			if	(!bTemp	||	(iOnOff	!=	0	&&	iOnOff	!=	1))
			{
				cMSG::Put("error","ex> setQuestStatus [questIndex] [0 or 1]");
				return;
			}

			char	*lpstrQuestName	=	g_quest.getName(iQuestIndex	);

			if	(lpstrQuestName	==	NULL)
			{
				cMSG::Put("error!","Invalid quest Index %d",iQuestIndex);
				return;
			}

			if	(cMSG::YESNO("change quest status","Do you want follow quest status by %d\n\n%s",iOnOff,lpstrQuestName)	==	IDNO)
				return;

			lpPlayer->setCompleteQuestStatus(eQT_NORMAL,iQuestIndex,iOnOff);
			lostVirgin();

			break;
		}
	
		if	(stricmp(lpstrKeyWord,"resetTitle")	==	0)
		{
			if	(cMSG::YESNO("Reset all title info","really?")	==	IDNO)
				return;

			memset(lpPlayer->m_aTitle,0,sizeof(lpPlayer->m_aTitle));
			lostVirgin();

			cMSG::Put("Reset title Info","complete");

			break;
		}
		if	(stricmp(lpstrKeyWord,"resetPetInfo")	==	0)
		{
			if	(cMSG::YESNO("Reset all pet info","really?")	==	IDNO)
				return;

			lpPlayer->m_aPet[0].reset();
			lpPlayer->m_aPet[1].reset();
			lostVirgin();
			cMSG::Put("Reset pet Info","complete");

			break;
		}
		if	(stricmp(lpstrKeyWord,"resetMainQuestSwitch")	==	0)
		{
			if	(cMSG::YESNO("Reset main quest switch info","really?")	==	IDNO)
				return;

			memset(lpPlayer->m_awMainQuestValue,0,sizeof(lpPlayer->m_awMainQuestValue));
			memset(lpPlayer->m_abMainQuestSwitchStatus,0,sizeof(lpPlayer->m_abMainQuestSwitchStatus));

			lostVirgin();
			cMSG::Put("Reset main quest switch info","complete");
			break;
		}
		if	(stricmp(lpstrKeyWord,"resetQuest")	==	0)
		{
			if	(cMSG::YESNO("Reset all quest complete info","really?")	==	IDNO)
				return;

			lpPlayer->m_bf1IsAddAboutMainQuestInfo	=	TRUE;

			lpPlayer->m_wProcessChapterIndex		=	0xffff;
			lpPlayer->m_wProcessHighQuestIndex		=	0xffff;
			memset(lpPlayer->m_awMainQuestValue,0,sizeof(lpPlayer->m_awMainQuestValue));
			memset(lpPlayer->m_abMainQuestSwitchStatus,0,sizeof(lpPlayer->m_abMainQuestSwitchStatus));

			memset(lpPlayer->m_aCompleteQuest,0,sizeof(lpPlayer->m_aCompleteQuest));
			memset(lpPlayer->m_aCompleteQuest2,0,sizeof(lpPlayer->m_aCompleteQuest2));
			memset(lpPlayer->m_aCompleteArbeit,0,sizeof(lpPlayer->m_aCompleteArbeit));
			memset(lpPlayer->m_aCompleteArbeit2,0,sizeof(lpPlayer->m_aCompleteArbeit2));
			lostVirgin();

			cMSG::Put("Reset all quest complete info","complete");
			break;
		}
		
		if	(stricmp(lpstrKeyWord,"addTitle") == 0)
		{
			BOOL	bTemp;
			char	*lpTitleName	=	decoder.GetStream();
			int		iTitleLevel		=	decoder.GetNumber(&bTemp);
			CTitle	*lpTitle		=	g_titleManager.getTitleByName(lpTitleName);

			if	(!lpTitle)
			{
				ERRMSG("command line","incorrect title name [%s]",lpTitleName);

				return;
			}

			if	(cMSG::YESNO("COMMAND","Do you want add title [%s:lv%d] to [%s]?",lpTitleName,iTitleLevel,lpPlayer->m_strName) == IDNO)
				return;


			if	(!lpPlayer->addTitle(lpTitle->m_wSerial,iTitleLevel))
				ERRMSG("command line","can not find valid title slot in player data",lpTitleName);
			else
			{
				lostVirgin();
				cMSG::Put("work complete","add title [%s:lv%d] to [%s]",lpTitleName,iTitleLevel,lpPlayer->m_strName);
			}

			break;
		}
		if	(stricmp(lpstrKeyWord,"removePet") == 0)
		{
			BOOL		temp;
			int			iPetIndex		=	decoder.GetNumber(&temp);
			cPetDefine	*lpPetInfo		=	&lpPlayer->m_aPet[iPetIndex];

			if	(lpPetInfo->m_bf10Level	==	0)
			{
				cMSG::Put("error in remove pet","invalid pet index %d",iPetIndex);
				break;
			}

			char	strPetName[dNAME_LENGTH];

			memcpy(strPetName,lpPetInfo->m_strName,dPET_NAME_LENGTH);
			strPetName[dPET_NAME_LENGTH]	=	NULL;

			if	(cMSG::YESNO("COMMAND","Remove Pet [%s]\n\nContinue?",strPetName) == IDNO)
				return;

			lpPetInfo->reset();
			lostVirgin();

			cMSG::Put("work complete","remove pet [%s]",strPetName);

			break;
		}

		if	(stricmp(lpstrKeyWord,"removeTitle") == 0)
		{
			char	*lpTitleName	=	decoder.GetStream();
			CTitle	*lpTitle		=	g_titleManager.getTitleByName(lpTitleName);

			if	(!lpTitle)
			{
				ERRMSG("command line","incorrect title name [%s]",lpTitleName);

				return;
			}

			if	(cMSG::YESNO("COMMAND","Do you want remove title [%s] from [%s]?",lpTitleName,lpPlayer->m_strName) == IDNO)
				return;

			if	(!lpPlayer->removeTitle(lpTitle->m_wSerial))
				ERRMSG("command line","can not find title [%s] in player data",lpTitleName);
			else
			{
				lostVirgin();
				cMSG::Put("work complete","remove title [%s] from [%s]",lpTitleName,lpPlayer->m_strName);
			}

			break;
		}

		if	(stricmp(lpstrKeyWord,"addPetExp") == 0)
		{
			BOOL		temp;
			int			iPetIndex		=	decoder.GetNumber(&temp);
			DWORD		dwExp			=	decoder.GetNumber(&temp);
			cPetDefine	*lpPetInfo	=	&lpPlayer->m_aPet[iPetIndex];

			if	(lpPetInfo->m_bf10Level	==	0)
			{
				cMSG::Put("error in add pet exp","invalid pet index %d",iPetIndex);
				break;
			}

			char	strPetName[dNAME_LENGTH];

			memcpy(strPetName,lpPetInfo->m_strName,dPET_NAME_LENGTH);

			strPetName[dPET_NAME_LENGTH]	=	NULL;

			if	(cMSG::YESNO("COMMAND","Add Exp [%d] to Pet [%s]\n\nContinue?",dwExp,strPetName) == IDNO)
				return;

			LONGLONG	llExp	=	lpPetInfo->m_dwExperience;

			llExp	+=	dwExp;

			if	(llExp	<	0xffffffff)
			{
				lostVirgin();
				lpPetInfo->m_dwExperience		+=	dwExp;
			}
			else
			{
				lostVirgin();
				lpPetInfo->m_bf10Level++;
				lpPetInfo->m_dwExperience		=	dwExp-(0xffffffff-lpPetInfo->m_dwExperience);
			}
			break;
		}

		if (stricmp(lpstrKeyWord,"addexp") == 0 || stricmp(lpstrKeyWord,"ae") == 0)
		{
			BOOL	temp;
			DWORD	dwExp			=	decoder.GetNumber(&temp);

			if	(cMSG::YESNO("COMMAND","Add Exp [%d].\n\nContinue?",dwExp) == IDNO)
				return;

			if	(!g_toolLog.addExp(lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName,dwExp))
				return;

			lpPlayer->m_iExperience	+=	dwExp;
			lostVirgin();

			SetDlgItemInt(IDC_EXPERIENCE,lpPlayer->m_iExperience,FALSE);
			break;
		}

		if (stricmp(lpstrKeyWord,"minusexp") == 0 || stricmp(lpstrKeyWord,"me") == 0)
		{
			BOOL	temp;
			DWORD	dwExp			=	decoder.GetNumber(&temp);

			if	(cMSG::YESNO("COMMAND","Minus Exp [%d].\n\nContinue?",dwExp) == IDNO)
				return;

			lpPlayer->decreaseExp(dwExp);

			lostVirgin();

			SetDlgItemInt(IDC_EXPERIENCE,lpPlayer->m_iExperience,FALSE);
			SetDlgItemInt(IDC_LEVEL,lpPlayer->m_iLevel,FALSE);
			break;
		}

		if (stricmp(lpstrKeyWord,"removeExp2") == 0)
		{
			BOOL	temp;
			int		iOldLevel		=	decoder.GetNumber(&temp);
			int		iOldExp			=	decoder.GetNumber(&temp);
			int		iLevel			=	decoder.GetNumber(&temp);
			int		iExp			=	decoder.GetNumber(&temp);

			if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
				break;

			int		iExpGap			=	ExpCalc::GetExp(iOldLevel,iOldExp,iLevel,iExp);

			if	(cMSG::YESNO("COMMAND","Remove Exp [%d].\n\nContinue?",iExpGap) == IDNO)
				return;

			lostVirgin();
			lpPlayer->decreaseExp(iExpGap);

			SetDlgItemInt(IDC_LEVEL,lpPlayer->m_iLevel,FALSE);
			SetDlgItemInt(IDC_EXPERIENCE,lpPlayer->m_iExperience,FALSE);
			break;
		}

		if (stricmp(lpstrKeyWord,"removeExp") == 0)
		{
			BOOL	temp;
			DWORD	dwExp			=	decoder.GetNumber(&temp);

			if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
				break;

			int		iBoostExp		=	100;//lpPlayer->getBoostExperienceValue();

			if	(iBoostExp		>	100)
			{
				LONGLONG	llExp	=	dwExp;
				llExp				=	llExp*iBoostExp/100;
				dwExp				=	(DWORD)llExp;

				if	(cMSG::YESNO("COMMAND","Remove Exp [%d]. by Sphere %d percent\n\nContinue?",dwExp,iBoostExp) == IDNO)
					return;
			}
			else
			if	(cMSG::YESNO("COMMAND","Remove Exp [%d].\n\nContinue?",dwExp) == IDNO)
				return;

			lpPlayer->decreaseExp(dwExp);

			lostVirgin();
			SetDlgItemInt(IDC_LEVEL,lpPlayer->m_iLevel,FALSE);
			SetDlgItemInt(IDC_EXPERIENCE,lpPlayer->m_iExperience,FALSE);
			break;
		}

		if (stricmp(lpstrKeyWord,"removeExpSP") == 0)
		{
			BOOL	temp;
			DWORD	dwExp			=	decoder.GetNumber(&temp);

			if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
				break;

			int		iBoostExp		=	lpPlayer->getBoostExperienceValue();

			if	(iBoostExp		>	100)
			{
				LONGLONG	llExp	=	dwExp;
				llExp				=	llExp*iBoostExp/100;
				dwExp				=	(DWORD)llExp;

				if	(cMSG::YESNO("COMMAND","Remove Exp [%d]. by Sphere %d percent\n\nContinue?",dwExp,iBoostExp) == IDNO)
					return;
			}
			else
			if	(cMSG::YESNO("COMMAND","Remove Exp [%d].\n\nContinue?",dwExp) == IDNO)
				return;

			cFILE	file;

			if	(!file.Open("add remove Exp Player List.txt","rb+"))
				file.Open("add remove Exp Player List.txt","wb");

			file.Seek(0,SEEK_END);

			file.writeStream("%s ",sList[m_iSelectWorld].strName);

			lpPlayer->operateCorrectExpWork050706(dwExp,&file,NULL);

			file.Close();
			lostVirgin();

			SetDlgItemInt(IDC_LEVEL,lpPlayer->m_iLevel,FALSE);
			SetDlgItemInt(IDC_EXPERIENCE,lpPlayer->m_iExperience,FALSE);
			break;
		}

		goto	error_label;
	}

	return;
error_label:

#ifdef _FOR_KOREA
		ERRMSG("command line","incorrect command [%s] \n\n%s\n",lpstrKeyWord,
"경험치 추가 : addexp [경험치]\n\
경험치 빼기 : minusexp [경험치]\n\
퀘스트 완료 상태 설정 : setQuestStatus [퀘스트 번호] [0 or 1]\n\
메인 퀘스트 초기화 : resetMainQuestSwitch\n\
칭호 추가 : addTitle [\"칭호 이름\"] [레벨]\n\
칭호 제거 : removeTitle [\"칭호 이름\"]\n\
펫 제거 : removePet [펫 인덱스]\n\
펫 경험치 추가 : addPetExp [펫 인덱스] [exp]\n\
플레이어 검색 : findPlayer [\"직업이름\"] [레벨]"
);

#else
		ERRMSG("command line","incorrect command [%s] \n\nae,addexp - Addition Exp\n",lpstrKeyWord);
#endif

	
}

void CCharacterInfo::OnCalculateExp() 
{
	ExpCalc	dlg;

	cPLAYER_DATA	*lpPlayer	=	NULL;
	
	if (m_am.m_iLoadDataCount)
	{
		lpPlayer	=	(cPLAYER_DATA *)&m_am.m_aData[m_iSelectAvatar];
	}

	if	(dlg.open(lpPlayer))
	{
#ifdef	_FOR_CHINA
		if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
		if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_THAI
		if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
		if (!CheckOperatorLevel(eAL_OPERATOR))			return;
#elif	_FOR_INTER
		if (!CheckOperatorLevel(eAL_OPERATOR))			return;
#else
		if (!CheckOperatorLevel(eAL_OPERATOR))			return;
#endif

		SetDlgItemInt(IDC_LEVEL	,lpPlayer->m_iLevel,FALSE);
		SetDlgItemInt(IDC_EXPERIENCE,lpPlayer->m_iExperience,FALSE);

		if	(!g_toolLog.changeLevel(lpPlayer->m_strId,lpPlayer->m_strName,m_dwLogLevel,lpPlayer->m_iLevel,sList[m_iSelectWorld].strName))
			return;
	}
}

void CCharacterInfo::OnRestoreStrangeFault() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_SYSTEM_OPERATER))	
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_SYSTEM_OPERATER))	
		return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_SYSTEM_OPERATER))	
		return;
#else
	if (!CheckOperatorLevel(eAL_SYSTEM_OPERATER))	
		return;
#endif
	if (m_am.m_iLoadDataCount	<=	0)	return;

	int		iAvatarIndex;

	cPLAYER_DATA	*lpPlayer	=	(cPLAYER_DATA *)&m_am.m_aData[m_iSelectAvatar];
	
	{
		// TODO: Add your control notification handler code here
		char	strDbName[256],strName[22],strId[18];
		m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));
		m_editSelectedName.GetWindowText(strName,sizeof(strName));
		m_editSelectedId.GetWindowText(strId,sizeof(strId));

		if(!l_DB.Connect(	sList[m_iSelectWorld].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
		{
			MessageBox("FAIL CONNECT TO DB SERVER.");		return;
		}
		iAvatarIndex	=	l_DB.spGetAvatarIndexOnly(strId,strName);
		l_DB.Close();
	}

	lpPlayer->m_dwPeneltyTime		=	0;
	lpPlayer->m_wAvatarIndex		=	iAvatarIndex;
	lpPlayer->m_wRequitalCounter	=	0;
	lpPlayer->m_dwSaveCount			+=	2;
	lpPlayer->m_dwPlayTime			=	60*24*10;
	lpPlayer->m_itemFailedPremiumItem.reset();
	memset(lpPlayer->m_aProcessQuest,0,sizeof(lpPlayer->m_aProcessQuest));
	memset(lpPlayer->m_aTitle,0,sizeof(lpPlayer->m_aTitle));
	memset(lpPlayer->m_aWayPoint,0xff,sizeof(lpPlayer->m_aWayPoint));
	memset(lpPlayer->m_aCompleteQuest,0,sizeof(lpPlayer->m_aCompleteQuest));

	lpPlayer->m_aTitle[0].m_bLevel	=	1;
	lpPlayer->m_aTitle[0].m_bTitle	=	1;

	OnResetSkillPoint() ;
}

void CCharacterInfo::OnCloseupCharacterList() 
{
	// TODO: Add your control notification handler code here
	int iSelectedList = m_cbCharacterList.GetCurSel();

	if(m_am.m_iLoadDataCount<=iSelectedList)
	{
		MessageBox("PLEASE, CONNECT TO LnkLogicKorea, in Korea");
		return;
	}
	m_dwLogExp = (DWORD)m_am.m_aData[iSelectedList].m_iExperience;
	m_dwLogLevel = m_am.m_aData[iSelectedList].m_iLevel;
	m_dwLastVillage = m_am.m_aData[iSelectedList].m_wLastVillage;
	m_dwLastField = m_am.m_aData[iSelectedList].m_wCurrentField;
}

void CCharacterInfo::OnSelchangeWorldList2() 
{
	CString	str;
	m_cbWorldList.GetWindowText(str);
	m_iSelectWorld	=	atoi(str);
	// TODO: Add your control notification handler code here
}



void CCharacterInfo::OnCheckStatePoint() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	int	iCurrentStatePoint	=	lpPlayer->getCurrentStatePointSum();
	int	iCorrectStatePoint	=	lpPlayer->getCurrentStatePointSumByLevel();

	char	strMessage[256];

	sprintf(strMessage,g_strToolMessage[124],iCurrentStatePoint,iCorrectStatePoint);

	MessageBox(strMessage);
}

void CCharacterInfo::OnOwnTitle() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#else
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#endif
	UpdateData(true);

	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	char	strExportDir[512],strExportFileName[512];
	int		i,j,iCount=0;

	sprintf(strExportDir,"%s\\Title",g_strLogFolder);
	sprintf(strExportFileName,"Title List - %s[%s].txt",m_am.m_strId,sList[m_iSelectWorld].strName);
	SetCurrentDirectory(strExportDir);

	cFILE	file;

	file.Open(strExportFileName,"wt");

	for (i=0;i<m_am.m_iLoadDataCount;i++)
	{
		file.writeStreamCR("◆ Character : %s",m_am.m_aData[i].m_strName);

		iCount	=	0;

		for (j=0;j<dMAX_PLAYER_TITLE_COUNT;j++)
		{
			int	iTitle		=	m_am.m_aData[i].m_aTitle[j].m_bTitle;
			int	iLevel		=	m_am.m_aData[i].m_aTitle[j].m_bLevel;

			if (iTitle	==	255 || iLevel >= 50 || iLevel == 0)
			{
				m_am.m_aData[i].m_aTitle[j].m_bTitle	=	255;
				m_am.m_aData[i].m_aTitle[j].m_bLevel	=	255;
				continue;
			}

			CTitle*	lpTitle	=	&g_titleManager.m_aTitles[iTitle];

			if (lpTitle->m_wSerial	==	0xffff)
			{
				m_am.m_aData[i].m_aTitle[j].m_bTitle	=	255;
				m_am.m_aData[i].m_aTitle[j].m_bLevel	=	0;
				continue;
			}

			file.writeStreamCR("    Title %s[Lv %d]",lpTitle->m_strName,iLevel);

			iCount++;
		}

		file.writeStreamCR("   ");
	}

	file.Close();

	ShellExecute(NULL,"open",strExportFileName,NULL,NULL,SW_SHOWNORMAL);

	SetLogFolder();
}

void CCharacterInfo::OnReadyToCreateGuild() 
{
	return;
	// TODO: Add your control notification handler code here
	
}

void CCharacterInfo::OnSelchangeWayPointIndex() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	fillData();
}

void CCharacterInfo::OnRequitalPeriodItem() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	int	iExtendDay		=	GetDlgItemInt(IDC_EXTEND_DAY	);
	int	iExtendHour		=	GetDlgItemInt(IDC_EXTEND_HOUR	);
	int	iExtendMinute	=	GetDlgItemInt(IDC_EXTEND_MINUTE	);

	int	iExtendTime	=	iExtendDay*24*60+iExtendHour*60+iExtendMinute;

	if	(iExtendTime	<=	0	||	iExtendTime	>	60*24*30)
	{
		cMSG::Put("renew item preiod","please input time\n\nlimit 1 month");
		return;
	}

	if	(cMSG::YESNO("renew item period","do you want id [%s]'s all item(include bank item) period\n\n%d day,%d hour,%d minute",m_am.m_strId,iExtendDay,iExtendHour,iExtendMinute)	==	IDNO)
		return;

	int	iRequitalItemCount	=	0,iPlayer,i,j;
	// TODO: Add your control notification handler code here
	for (iPlayer=0;iPlayer<m_am.m_iLoadDataCount;iPlayer++)
	{
		CAvatarData	*lpPlayer	=	&m_am.m_aData[iPlayer];

		for (j=0;j<dOWN_ITEM_COUNT;j++)
		{
			cITEM	*lpItem	=	(cITEM *)&lpPlayer->m_aItems[j];

			if	(lpItem->m_wBaseItem	==	0xffff || !lpItem->isPremiumItem()	||	lpItem->m_year	==	0)
				continue;

			if	(!g_toolLog.renewPlayItemPeriod(lpItem,iExtendDay,iExtendHour,iExtendMinute,lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
				return;

			lpItem->increaseMinute(iExtendTime);

			iRequitalItemCount++;
		}

		for (j=0;j<dEQUIPMENT_PART_COUNT;j++)
		{
			cITEM	*lpItem	=	(cITEM *)&lpPlayer->m_aEquip[j];

			if	(lpItem->m_wBaseItem	==	0xffff || !lpItem->isPremiumItem()	||	lpItem->m_year	==	0)
				continue;

			if	(!g_toolLog.renewPlayItemPeriod(lpItem,iExtendDay,iExtendHour,iExtendMinute,lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
				return;

			lpItem->increaseMinute(iExtendTime);
			iRequitalItemCount++;
		}
		for (j=0;j<lpPlayer->m_bf6ExtraInventorySize;j++)
		{
			cITEM	*lpItem	=	(cITEM *)&lpPlayer->m_aExtraInventory[j];

			if	(lpItem->m_wBaseItem	==	0xffff || !lpItem->isPremiumItem()	||	lpItem->m_year	==	0)
				continue;

			if	(!g_toolLog.renewPlayItemPeriod(lpItem,iExtendDay,iExtendHour,iExtendMinute,lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName))
				return;

			lpItem->increaseMinute(iExtendTime);
			iRequitalItemCount++;
		}
	}	

	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		cITEM	*lpItem	=	(cITEM *)&m_am.m_bank.m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff || !lpItem->isPremiumItem()	||	lpItem->m_year	==	0)
			continue;

		if	(!g_toolLog.renewBankItemPeriod(lpItem,iExtendDay,iExtendHour,iExtendMinute,m_am.m_strId,sList[m_iSelectWorld].strName))
			return;

		lpItem->increaseMinute(iExtendTime);
		m_am.m_bIsChangeBankData	=	TRUE;
	}

	if	(iRequitalItemCount)
		lostVirgin();

	cMSG::Put("complete",g_strToolMessage[149],iRequitalItemCount);
}

void CCharacterInfo::OnQuestList() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#else
	if (!CheckOperatorLevel(eAL_TESTER))	return;
#endif

	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	cQuestList	questDlg;

	memcpy(questDlg.m_aProcessQuest,lpPlayer->m_aProcessQuest,sizeof(questDlg.m_aProcessQuest));
	memcpy(questDlg.m_aCompleteQuest,lpPlayer->m_aCompleteQuest,sizeof(questDlg.m_aCompleteQuest));
	memcpy(questDlg.m_aCompleteQuest2,lpPlayer->m_aCompleteQuest2,sizeof(questDlg.m_aCompleteQuest2));
	memcpy(questDlg.m_aCompleteArbeit,lpPlayer->m_aCompleteArbeit,sizeof(questDlg.m_aCompleteArbeit));
	memcpy(questDlg.m_aCompleteArbeit2,lpPlayer->m_aCompleteArbeit2,sizeof(questDlg.m_aCompleteArbeit2));
	memcpy(questDlg.m_awMainQuestValue,lpPlayer->m_awMainQuestValue,sizeof(questDlg.m_awMainQuestValue));

	questDlg.setPlayer(lpPlayer);

	if	(questDlg.DoModal()	==	IDCANCEL)
		return;

	memcpy(lpPlayer->m_aProcessQuest,questDlg.m_aProcessQuest,sizeof(questDlg.m_aProcessQuest));
	memcpy(lpPlayer->m_awMainQuestValue,questDlg.m_awMainQuestValue,sizeof(questDlg.m_awMainQuestValue));

	memcpy(lpPlayer->m_aCompleteQuest,questDlg.m_aCompleteQuest,sizeof(questDlg.m_aCompleteQuest));
	memcpy(lpPlayer->m_aCompleteQuest2,questDlg.m_aCompleteQuest2,sizeof(questDlg.m_aCompleteQuest2));
	memcpy(lpPlayer->m_aCompleteArbeit,questDlg.m_aCompleteArbeit,sizeof(questDlg.m_aCompleteArbeit));
	memcpy(lpPlayer->m_aCompleteArbeit2,questDlg.m_aCompleteArbeit2,sizeof(questDlg.m_aCompleteArbeit2));
	
	lostVirgin();
}

void CCharacterInfo::OnGetGuildName() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	if (m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];
	cADO_ADMIN	DB;
	if(!DB.Connect(	sList[m_iSelectWorld].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
	{	
		MessageBox("Fail Connect!!");	
		return;	
	}

	//	DB백업하고 할것!!
	{
		_RecordsetPtr  pRs = NULL;
		char	       str_Query[1024];
		int i_RecCount;
		HRESULT hr = CoInitialize(0);

		sprintf(str_Query,"SELECT strName FROM GUILD_INFO WHERE iIndex = %d",lpPlayer->m_wGuildSerial);
		printf(str_Query);
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)DB.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;

		if(!FAILED(hr) && i_RecCount>0)
		{ 
			_variant_t     v_bufferdata;
			VariantInit(&v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			char strGuildName[64];
			strcpy(strGuildName,(_bstr_t)v_bufferdata);
			SetDlgItemText(IDC_GUILD,_ms("%s[%d]",strGuildName,lpPlayer->m_wGuildSerial));
		}else
		{
			MessageBox("Fail to get the infomation!","ERROR",MB_OK);
		}
		pRs->Close();
	}

	DB.Close();
}

void CCharacterInfo::OnResetJoinGuildDate() 
{
#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_THAI
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#else
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
#endif

	if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	if	(m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	if	(!g_toolLog.workMessage(lpPlayer->m_strId,lpPlayer->m_strName,sList[m_iSelectWorld].strName,"reset join guild date"))
		return;

	lpPlayer->m_timeGuildJoinTime.year	=	0;
	lpPlayer->m_timeGuildJoinTime.month	=	0;
	lpPlayer->m_timeGuildJoinTime.day	=	0;

	SetDlgItemInt(IDC_GUILD_JOIN_YEAR	,lpPlayer->m_timeGuildJoinTime.year,FALSE);
	SetDlgItemInt(IDC_GUILD_JOIN_MONTH	,lpPlayer->m_timeGuildJoinTime.month,FALSE);
	SetDlgItemInt(IDC_GUILD_JOIN_DAY	,lpPlayer->m_timeGuildJoinTime.day,FALSE);
}

void CCharacterInfo::OnItemInfo() 
{
	cMakeItem	dlg;

	dlg.m_makeItem.reset();

	dlg.DoModal();
	
}

void CCharacterInfo::OnPetList() 
{
	if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;
	
	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];
	
 	cPetList		PetList;
 	
 	PetList.setPetInfo(lpPlayer->m_aPet);

	if	(PetList.DoModal()	==	IDCANCEL)
		return;
	
	/*
	cMiniPetList	miniPetList;

	miniPetList.setMiniPet(lpPlayer->m_aMiniPet,
		lpPlayer->m_reversalFrameYear	,lpPlayer->m_reversalFrameMonth	,lpPlayer->m_reversalFrameDay	,
		lpPlayer->m_reversalFrameHour	,lpPlayer->m_reversalFrameMinute	);
	
	if	(miniPetList.DoModal()	==	IDCANCEL)
		return;
	
	memcpy(lpPlayer->m_aMiniPet,miniPetList.getMiniPet(),sizeof(lpPlayer->m_aMiniPet));
	
	lpPlayer->m_reversalFrameYear	=	miniPetList.m_editReversalFrameValidate.m_wYear-2000;
	lpPlayer->m_reversalFrameMonth	=	miniPetList.m_editReversalFrameValidate.m_wMonth;
	lpPlayer->m_reversalFrameDay	=	miniPetList.m_editReversalFrameValidate.m_wDay;
	lpPlayer->m_reversalFrameHour	=	miniPetList.m_editReversalFrameValidate.m_wHour;
	lpPlayer->m_reversalFrameMinute	=	miniPetList.m_editReversalFrameValidate.m_wMinute;
	*/
	
	lostVirgin();

}


void CCharacterInfo::OnMinipetList() 
{
	if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	cMiniPetList	miniPetList;

	miniPetList.setMiniPet(lpPlayer,lpPlayer->m_aMiniPet,
							lpPlayer->m_reversalFrameYear	,lpPlayer->m_reversalFrameMonth	,lpPlayer->m_reversalFrameDay	,
							lpPlayer->m_reversalFrameHour	,lpPlayer->m_reversalFrameMinute	);

	if	(miniPetList.DoModal()	==	IDCANCEL)
		return;

	memcpy(lpPlayer->m_aMiniPet,miniPetList.getMiniPet(),sizeof(lpPlayer->m_aMiniPet));

	lpPlayer->m_reversalFrameYear	=	miniPetList.m_editReversalFrameValidate.m_wYear-2000;
	lpPlayer->m_reversalFrameMonth	=	miniPetList.m_editReversalFrameValidate.m_wMonth;
	lpPlayer->m_reversalFrameDay	=	miniPetList.m_editReversalFrameValidate.m_wDay;
	lpPlayer->m_reversalFrameHour	=	miniPetList.m_editReversalFrameValidate.m_wHour;
	lpPlayer->m_reversalFrameMinute	=	miniPetList.m_editReversalFrameValidate.m_wMinute;

	lostVirgin();
}

void CCharacterInfo::OnApplyDuelRecord() 
{
	if	(!CheckOperatorLevelForWrite())
		return;

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	cDuelRecord	record;

	memcpy(&record,&m_am.m_duelRecord,sizeof(cDuelRecord));

	record.m_iDuelPoint			=	GetDlgItemInt(IDC_DUEL_POINT);

	record.m_iBattleCount		=	GetDlgItemInt(IDC_TOTAL_BC);
	record.m_iWinCount			=	GetDlgItemInt(IDC_TOTAL_WIN_C);
	record.m_iDefeatCount		=	GetDlgItemInt(IDC_TOTAL_LOSE_C);
	record.m_iDrawCount			=	GetDlgItemInt(IDC_TOTAL_DRAW_C);

	record.m_iWeeklyBattleCount	=	GetDlgItemInt(IDC_WEEKLY_BC);
	record.m_iWeeklyWinCount	=	GetDlgItemInt(IDC_WEEKLY_WIN_C);
	record.m_iWeeklyDefeatCount	=	GetDlgItemInt(IDC_WEEKLY_LOSE_C);
	record.m_iWeeklyDrawCount	=	GetDlgItemInt(IDC_WEEKLY_DRAW_C);
	record.m_iWeeklyWinPoint	=	GetDlgItemInt(IDC_WINNING_POINT);

	if	(record.m_iBattleCount	!=	record.m_iWinCount+record.m_iDefeatCount+record.m_iDrawCount)
	{
		cMSG::Error("incorrect battle count","%d+%d+%d = %d ??",record.m_iWinCount,record.m_iDefeatCount,record.m_iDrawCount,record.m_iBattleCount);
		return;
	}
	if	(record.m_iWeeklyBattleCount	!=	record.m_iWeeklyWinCount+record.m_iWeeklyDefeatCount+record.m_iWeeklyDrawCount)
	{
		cMSG::Error("incorrect weekly battle count","%d+%d+%d = %d ??",record.m_iWeeklyWinCount,record.m_iWeeklyDefeatCount,record.m_iWeeklyDrawCount,record.m_iWeeklyBattleCount);
		return;
	}

	g_DBAccount.spModifyPvPScore(&record);

	if	(!g_DBAccount.spGetPvPScore(m_am.m_strId,&m_am.m_duelRecord))
		memset(&m_am.m_duelRecord,0,sizeof(m_am.m_duelRecord));

	cDuelRecord	*lpRecord			=	&m_am.m_duelRecord;

	SetDlgItemInt(IDC_DUEL_POINT	,lpRecord->m_iDuelPoint);

	SetDlgItemInt(IDC_TOTAL_BC		,lpRecord->m_iBattleCount);
	SetDlgItemInt(IDC_TOTAL_WIN_C	,lpRecord->m_iWinCount);
	SetDlgItemInt(IDC_TOTAL_LOSE_C	,lpRecord->m_iDefeatCount);
	SetDlgItemInt(IDC_TOTAL_DRAW_C	,lpRecord->m_iDrawCount);

	SetDlgItemInt(IDC_WEEKLY_BC		,lpRecord->m_iWeeklyBattleCount);
	SetDlgItemInt(IDC_WEEKLY_WIN_C	,lpRecord->m_iWeeklyWinCount);
	SetDlgItemInt(IDC_WEEKLY_LOSE_C	,lpRecord->m_iWeeklyDefeatCount);
	SetDlgItemInt(IDC_WEEKLY_DRAW_C	,lpRecord->m_iWeeklyDrawCount);
	SetDlgItemInt(IDC_WINNING_POINT	,lpRecord->m_iWeeklyWinPoint);

	cMSG::Put("work complete.","check changed duel record");
}

void CCharacterInfo::OnGetDuelRecord() 
{
	// TODO: Add your control notification handler code here
	
}

CItemDefine*
CAvatarData::getExistItem(CItemDefine* _lpItem)
{
	int	i;

	int	iInventorySize	=	getInventorySize();

	for (i=m_bf6ExtraInventorySize;i;)
	{
		--i;

		cITEM	*lpItem	=	(cITEM	*)&m_aExtraInventory[i];

		if	(lpItem->isSameItem((cITEM	*)_lpItem))
			return	lpItem;
	}

	for (i=iInventorySize;i;)
	{
		--i;

		cITEM	*lpItem	=	(cITEM	*)&m_aItems[i];

		if	(lpItem->isSameItem((cITEM	*)_lpItem))
			return	lpItem;
	}
		
	return	NULL;
}

void CCharacterInfo::OnChangeSealOfDawnCount() 
{
	lostVirgin();
}

void CCharacterInfo::OnChangeRebirthCount() 
{
	lostVirgin();
}

CItemDefine*
CAvatarData::getExistItem(int _iBaseItem,int _iIndex,BOOL _bEquip)
{
	int	i=0;
	int iCount = 0;
	int	iInventorySize		=	getInventorySize();
	
	for (i=0;i<iInventorySize;++i)
	{
		if	(m_aItems[i].m_wBaseItem == _iBaseItem)
		{	
			if	(iCount == _iIndex)
				return	&m_aItems[i];
			iCount ++;
		}
	}
	
	for (i=0;i<m_bf6ExtraInventorySize;++i)
	{
		if	(m_aExtraInventory[i].m_wBaseItem == _iBaseItem)
		{
			if	(iCount == _iIndex)
				return	&m_aItems[i];
			iCount ++;
		}
	}
	
	if(_bEquip)
	{
		for (i=0;i<dEQUIPMENT_PART_COUNT;++i)
		{
			if	(m_aEquip[i].m_wBaseItem == _iBaseItem)
			{
				if	(iCount == _iIndex)
					return	&m_aItems[i];
				iCount ++;
			}
		}	
	}	

	return	NULL;
}

int
CAvatarData::getItemCount(int _iBaseItem, BOOL _bEquip)
{
	int	i=0;
	int iCount = 0;
	int	iInventorySize		=	getInventorySize();
	
	for (i=0;i<iInventorySize;++i)
	{
		if	(m_aItems[i].m_wBaseItem == _iBaseItem)
		{		
			iCount += m_aItems[i].m_bCount;
		}
	}
	
	for (i=0;i<m_bf6ExtraInventorySize;++i)
	{
		if	(m_aExtraInventory[i].m_wBaseItem == _iBaseItem)
		{
			iCount += m_aExtraInventory[i].m_bCount;
		}
	}
	
	if(_bEquip)
	{
		for (i=0;i<dEQUIPMENT_PART_COUNT;++i)
		{
			if	(m_aEquip[i].m_wBaseItem == _iBaseItem)
			{
				
				iCount += m_aEquip[i].m_bCount;
			}
		}	
	}	

	return	iCount;
}

int
CAvatarData::getItemSlotCount(int _iBaseItem, BOOL _bEquip)
{
	int	i=0;
	int iCount = 0;
	int	iInventorySize		=	getInventorySize();
	
	for (i=0;i<iInventorySize;++i)
	{
		if	(m_aItems[i].m_wBaseItem == _iBaseItem)
		{		
			iCount ++;
		}
	}
	
	for (i=0;i<m_bf6ExtraInventorySize;++i)
	{
		if	(m_aExtraInventory[i].m_wBaseItem == _iBaseItem)
		{
			iCount ++;
		}
	}
	
	if(_bEquip)
	{
		for (i=0;i<dEQUIPMENT_PART_COUNT;++i)
		{
			if	(m_aEquip[i].m_wBaseItem == _iBaseItem)
			{
				iCount ++;
			}
		}	
	}	

	return	iCount;
}

void CCharacterInfo::OnResetTransGvgDate() 
{
	if	(m_iSelectWorld	>=	m_iWorldCount || m_iSelectWorld < 0)
		return;

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData		*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	lpPlayer->m_bf6LastTransAvatarToGVGYear		=	0;	//	마지막으로 전송한 연도
	lpPlayer->m_bf4LastTransAvatarToGVGMonth	=	0;
	lpPlayer->m_bf5LastTransAvatarToGVGDay		=	0;

	SetDlgItemText(IDC_TRANS_GVG_DATE_TEXT,
					_ms("%d/%d/%d",lpPlayer->m_bf6LastTransAvatarToGVGYear,lpPlayer->m_bf4LastTransAvatarToGVGMonth,lpPlayer->m_bf5LastTransAvatarToGVGDay)
					);

	lostVirgin();
}

void CCharacterInfo::OnReviseSp() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_TESTER))
		return;
#elif	_FOR_USA
	if (!CheckOperatorLevel(eAL_TESTER))
		return;
#elif	_FOR_INTER
	if (!CheckOperatorLevel(eAL_TESTER))
		return;
#else
	if (!CheckOperatorLevel(eAL_TESTER))
		return;
#endif

	if (m_am.m_iLoadDataCount	<=	0)
	{
		MessageBox(g_strToolMessage[106]);
		return;
	}

	CAvatarData	*lpPlayer	=	&m_am.m_aData[m_iSelectAvatar];

	int	iCurrentSkillPoint	=	lpPlayer->getCurrentSkillPointSum();
	int	iCorrectSkillPoint	=	lpPlayer->getCurrentSkillPointByLevel();

	int	iAddSP				=	iCorrectSkillPoint-iCurrentSkillPoint;

	if	(iAddSP	>	0)
	{
		lpPlayer->m_dwSkillExperience	+=	iAddSP;

		char	strMessage[256];

		sprintf(strMessage,"revise skill point\n\nadd %d SP");

		MessageBox(strMessage);	
	}
	else
	{
		MessageBox("is correct skill point");	
	}
}