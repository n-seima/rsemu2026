// guildInfo.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "guildInfo.h"
#include "cSRVUTIL.h"
#include "cPACKET_WORLDSERVER.h"
#include "cDlgChangeGMaster.h"
#include "GuildInventory.h"
#include "GuildPetAndGuardian.h"
#include "cITEM.h"
#include <atlbase.h>
#include "CDecoder.h"
#include "winuser.h"
#include "CharacterInfo.h"
#include "cMESSAGE.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cGuildInfo dialog
char	*strGuildRank[]	=
{
	"invalid",
	"Rookie",
	"Normal",
	"Congressman",
	"Sub Master",
	"Elder",
	"Master",
	"__end"
};

CGuildSkillManager	g_gsManager;

BOOL
CGuildSkillManager::init()
{
	m_iSkillCount	=	0;
	return	loadData();
}

BOOL
CGuildSkillManager::loadData()
{
	CRegKey		reg;

	char		strLogFolder[512],strDataFolder[512];

	int iResult	=	reg.Open( g_hRegKey, g_lpstrRegistry);

	if (iResult == ERROR_SUCCESS)
	{
		DWORD	count	=	sizeof(strLogFolder);
		if (reg.QueryValue((LPTSTR )strLogFolder,"Path",&count) == ERROR_SUCCESS)
		{	if (count	<=	0)
				return FALSE;
		}
		else	return FALSE;
	}
	else	return FALSE;

	cFILE	file;

	sprintf(strDataFolder,"%s/data",strLogFolder);

	if (!SetCurrentDirectory(strDataFolder))
		return	FALSE;

	if (!file.Open("guildSkill.dat","rb"))
		EXIT("error in CGuildSkillManager::loadData","can not find 'data/guildSkill.dat' file.");

	file.Read(&g_gsManager.m_iSkillCount,4);
	file.Read(g_gsManager.m_aSkills,sizeof(g_gsManager.m_aSkills));
	file.Close();

	return	TRUE;
}

CGuildSkillDefine*
CGuildSkillManager::get(int _iSkill)
{
	if (_iSkill < 0 || _iSkill >= dMAX_GUILD_SKILL_COUNT)	return	NULL;
	if (m_aSkills[_iSkill].m_wSerial	==	0xffff		)	return	NULL;

	return	&m_aSkills[_iSkill];
}


cGuildInfo::cGuildInfo()
	: CPropertyPage(cGuildInfo::IDD)
{
	//{{AFX_DATA_INIT(cGuildInfo)
	//}}AFX_DATA_INIT
}


void cGuildInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cGuildInfo)
	DDX_Control(pDX, IDC_ORGANIZE_DATE_YEAR, m_ebOrganizeDateYear);
	DDX_Control(pDX, IDC_ORGANIZE_DATE_MONTH, m_ebOrganizeDateMonth);
	DDX_Control(pDX, IDC_ORGANIZE_DATE_DAY, m_ebOrganizeDateDay);
	DDX_Control(pDX, IDC_NOTICE, m_ebNotice);
	DDX_Control(pDX, IDC_MASTER, m_ebMasterName);
	DDX_Control(pDX, IDC_GUILD_SKILL_POINT, m_ebGuildSkillPoint);
	DDX_Control(pDX, IDC_EXP_TAX, m_ebExpTax);
	DDX_Control(pDX, IDC_CHANGE_GUILD_MASTER_DATE_YEAR, m_ebChangeMasterDateYear);
	DDX_Control(pDX, IDC_CHANGE_GUILD_MASTER_DATE_MONTH, m_ebChangeMasterDateMonth);
	DDX_Control(pDX, IDC_CHANGE_GUILD_MASTER_DATE_DAY, m_ebChangeMasterDateDay);
	DDX_Control(pDX, IDC_SERIAL, m_ebSerial);
	DDX_Control(pDX, IDC_EXPERIENCE, m_ebExperience);
	DDX_Control(pDX, IDC_LEVEL, m_ebLevel);
	DDX_Control(pDX, IDC_GUILD_NAME, m_ebGuildName);
	DDX_Control(pDX, IDC_PLACE_RELATED, m_ebGuildVillage);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cGuildInfo, CPropertyPage)
	//{{AFX_MSG_MAP(cGuildInfo)
	ON_BN_CLICKED(IDC_GET_GUILD_INFO, OnGetGuildInfo)
	ON_BN_CLICKED(IDC_ASK_GUILD_LIST, OnAskGuildList)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_CHANGE_GUILDMASTER, OnChangeGuildmaster)
	ON_BN_CLICKED(IDC_GUILD_INVENTORY, OnGuildInventory)
	ON_BN_CLICKED(IDC_PET_AND_GUARDIAN, OnPetAndGuardian)
	ON_BN_CLICKED(IDC_TWINKLE_MARK, OnTwinkleMark)
	ON_BN_CLICKED(IDC_RESET_ALL_GUILD_TWINKLE_STATUS, OnResetAllGuildTwinkleStatus)
	ON_BN_CLICKED(IDC_CHANGE_HALL_LEVEL, OnChangeHallLevel)
	ON_BN_CLICKED(IDC_GET_ALL, OnGetAll)
	ON_BN_CLICKED(IDC_REWIND_LIST, OnRewindList)
	ON_BN_CLICKED(IDC_CHANGE_PLACE_REATED, OnChangePlaceReated)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_GUILD_ETC, OnButtonSaveGuildEtc)
	ON_BN_CLICKED(IDC_SAVE_COUNT_OF_GPB, OnSaveCountOfGpb)
	ON_BN_CLICKED(IDC_SYNC_LOADED_PLAYER, OnSyncLoadedPlayer)
	ON_NOTIFY(NM_RCLICK, IDC_MEMBER_LIST, OnRclickMemberList)
	ON_BN_CLICKED(IDC_SAVE_TODAY_GUILD_BATTLE_COUNT, OnSaveTodayGuildBattleCount)
	ON_BN_CLICKED(IDC_SAVE_THIS_WEEK_GUILD_BATTLE_COUNT, OnSaveThisWeekGuildBattleCount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cGuildInfo message handlers


void
cGuildInfo::fillData()
{
	SetDlgItemInt(IDC_MASTER_CONNECT_DATE_YEAR, m_guildInfo.m_timeLastConnectedMasterTime.year+2000);
	SetDlgItemInt(IDC_MASTER_CONNECT_DATE_MONTH, m_guildInfo.m_timeLastConnectedMasterTime.month);
	SetDlgItemInt(IDC_MASTER_CONNECT_DATE_DAY, m_guildInfo.m_timeLastConnectedMasterTime.day);

	SetDlgItemInt(IDC_LAST_CONNECT_DATE_YEAR, m_guildInfo.m_timeLastConnectTime.year+2000);
	SetDlgItemInt(IDC_LAST_CONNECT_DATE_MONTH, m_guildInfo.m_timeLastConnectTime.month);
	SetDlgItemInt(IDC_LAST_CONNECT_DATE_DAY, m_guildInfo.m_timeLastConnectTime.day);

	SetDlgItemInt(IDC_ORGANIZE_DATE_YEAR, m_guildInfo.m_timeOrganizeDate.year+2000);
	SetDlgItemInt(IDC_ORGANIZE_DATE_MONTH, m_guildInfo.m_timeOrganizeDate.month);
	SetDlgItemInt(IDC_ORGANIZE_DATE_DAY, m_guildInfo.m_timeOrganizeDate.day);

	SetDlgItemInt(IDC_CHANGE_GUILD_MASTER_DATE_YEAR, m_guildInfo.m_timeBeginGuildMaster.year+2000);
	SetDlgItemInt(IDC_CHANGE_GUILD_MASTER_DATE_MONTH, m_guildInfo.m_timeBeginGuildMaster.month);
	SetDlgItemInt(IDC_CHANGE_GUILD_MASTER_DATE_DAY, m_guildInfo.m_timeBeginGuildMaster.day);

	SetDlgItemInt(IDC_COUNT_OF_POINT_BATTLE	, m_guildInfo.m_bf2TryGuildPointBattleCountPerWeek);
	SetDlgItemInt(IDC_HONOR_POINT			, m_guildInfo.m_iHonorPoint);
	SetDlgItemInt(IDC_GUILD_SKILL_POINT, m_guildInfo.m_wGuildPoint);
	SetDlgItemInt(IDC_SERIAL, m_guildInfo.m_wSerial);
	SetDlgItemInt(IDC_EXPERIENCE, m_guildInfo.m_dwExp);
	SetDlgItemInt(IDC_LEVEL, m_guildInfo.m_wLevel);

	//SetDlgItemInt(IDC_GUILD_DUNGEN_PLAYTIME, m_guildInfo.m_bf3GuildDungeonPlayTime);
	SetDlgItemInt(IDC_ROEN_GOOD_WILL, m_guildInfo.m_bf3GuildGoodwillForRoen);
	SetDlgItemInt(IDC_GUILD_MASTER_GRACE, m_guildInfo.m_bf1IsUseGuildMasterGrace);

	SetDlgItemInt(IDC_TODAY_GUILD_BATTLE_COUNT,m_guildInfo.m_bf3ThisDayGuildBattleCount);
	SetDlgItemInt(IDC_THIS_WEEK_GUILD_BATTLE_COUNT,m_guildInfo.m_bf5ThisWeekGuildBattleCount);

	CheckDlgButton(IDC_TWINKLE_MARK,m_guildInfo.m_bf1IsTwinkle);

	SetDlgItemText(IDC_PLACE_RELATED,
					_ms("%d,%d,%d,%d,%d,%d,%d",
					m_guildInfo.m_awGuildVillage[0],m_guildInfo.m_awGuildVillage[1],m_guildInfo.m_awGuildVillage[2],m_guildInfo.m_awGuildVillage[3],
					m_guildInfo.m_awGuildVillage[4],m_guildInfo.m_awGuildVillage[5],m_guildInfo.m_awGuildVillage[6])
					);

	SetDlgItemInt(IDC_VP_POINT,m_guildInfo.m_iVictoryPoint);
	SetDlgItemInt(IDC_WIN,m_guildInfo.m_wVictoryCount);
	SetDlgItemInt(IDC_DRAW,m_guildInfo.m_wDrawCount);
	SetDlgItemInt(IDC_DEFEAT,m_guildInfo.m_wDefeatCount);
	SetDlgItemInt(IDC_CV_COUNT,m_guildInfo.m_wVictoryContinue);

	{
		char	strGuildMark[64];

		sprintf(strGuildMark,"%d %d %d %d %d %d %d",
				m_guildInfo.m_guildMarkInfo.m_wIcon,
				m_guildInfo.m_guildMarkInfo.m_bIconColor,
				m_guildInfo.m_guildMarkInfo.m_bBaseShape,
				m_guildInfo.m_guildMarkInfo.m_bBaseColorPartition,
				m_guildInfo.m_guildMarkInfo.m_bBaseColor1,
				m_guildInfo.m_guildMarkInfo.m_bBaseColor2,
				m_guildInfo.m_guildMarkInfo.m_bBaseLineColor);

		SetDlgItemText(IDC_GUILD_MARK,strGuildMark);
	}

	m_ebGuildName.SetWindowText(m_guildInfo.m_strName);
	m_ebMasterName.SetWindowText(m_guildInfo.m_strMasterName);
	m_ebNotice.SetWindowText(m_guildInfo.m_strNotice);
	m_ebExpTax.SetWindowText(_ms("%d.%d",m_guildInfo.m_wExpTaxRate/100,m_guildInfo.m_wExpTaxRate%100));

	cSTRING	strBattleGuildList;

	for	(int iBGL=0;iBGL<dMAX_FIGHT_ATWEEK;iBGL++)
	{
		if	(m_guildInfo.m_wFightGuild[iBGL]	>=	m_iGuildCount)
			strBattleGuildList.Add("[ ] ");
		else
			strBattleGuildList.Add("[%s] ",m_strGuildList[m_guildInfo.m_wFightGuild[iBGL]]);
	}

	SetDlgItemInt(IDC_HALL_LEVEL,m_guildInfo.m_wHallLevel);

//	m_ebLastBattleGuild.SetWindowText(strBattleGuildList.String);
//	m_ebLastBattleGuild.SetWindowText(_ms("%d,%d,%d,%d",m_guildInfo.m_wFightGuild[0],m_guildInfo.m_wFightGuild[1],m_guildInfo.m_wFightGuild[2],m_guildInfo.m_wFightGuild[3]));

	m_listMemberList.reset();

	int	iCount	=	0,i;

	for (i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if (m_aMember[i].wSerial	==	0xffff)
			continue;

		m_listMemberList.addItem(iCount,0,_ms("%.3d",iCount+1),i);
		m_listMemberList.setItem(iCount,1,m_aMember[i].m_strName);
		m_listMemberList.setItem(iCount,2,strGuildRank[m_aMember[i].m_wRank]);

		iCount++;
	}

	m_listSkillList.reset();

	iCount	=	0;
	for (i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
	{
		if	(m_guildInfo.m_abGuildSkill[i]	==	0	||	m_guildInfo.m_abGuildSkill[i]	==	0xff)
			continue;
		
		m_listSkillList.addItem(iCount,0,_ms("%.3d",iCount+1),i);
		m_listSkillList.setItem(iCount,1,g_gsManager.get(i)->m_strName);
		m_listSkillList.setItem(iCount,2,_ms("%d",m_guildInfo.m_abGuildSkill[i]));

		iCount++;
	}
}

void
cGuildInfo::receiveGuildInfo(int _iSerial,char *_lpstrName,CGuildAdvanceInfo *_lpInfo)
{
	char	strGuildName[dGUILD_NAME_LENGTH];

	m_ebGuildName.GetWindowText(strGuildName,sizeof(strGuildName)-2);

	if (_iSerial	==	0xffff)
	{
		MessageBox(_ms("can not find [%s] guild in select server",strGuildName),"error!!");

		return;
	}

	memcpy(&m_guildInfo,_lpInfo,sizeof(CGuildAdvanceInfo));

	if	(m_bIsWantAllGuildInfo)
	{
		cFILE	file;

		char	strFolder[5112];

		GetCurrentDirectory(512,strFolder);

		if	(file.Open(_ms("guild info [%s].txt",sList[m_iSelectedServer].strName),"at"))
		{
			file.writeStream("\n====================================================================================\n");
			file.writeStream("guild name : %s\n",m_guildInfo.m_strName);
			file.writeStream("> Item List\n");

			for (int	i=0;i<c_iGuildInventorySize;i++)
			{
				cITEM	*lpItem	=	(cITEM	*)&m_guildInfo.m_aInventory[i];
				
				if	(lpItem->m_wBaseItem	==	0xffff)
					continue;

				file.writeStreamCR("    Slot %2d %s IT [%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d       [%.2d/%.2d/%.2d] [%.2d:%.2d]",i,
									lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
									lpItem->m_aOption[0],lpItem->m_aOption[1],
									lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
									lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
									lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
									lpItem->getBitFieldValue()			,
									lpItem->m_year	,lpItem->m_month	,lpItem->m_day	,lpItem->m_hour	,lpItem->m_minute	);
			}
			file.writeStreamCR("\n> Member List\n");

			file.Close();
		}
	}
}

void
cGuildInfo::receiveGuildMemberInfo(int _iSerial,int _iAllCount,int _iListCount,CGuildMan *_lpMember)
{
	memset(m_aMember,0,sizeof(CGuildMan)*dGUILD_MAX_PLAYER);

	for (int i=0;i<_iListCount;i++)
		memcpy(&m_aMember[_lpMember[i].wSerial],&_lpMember[i],sizeof(CGuildMan));

	if	(m_bIsWantAllGuildInfo)
	{
		cFILE	file;

		if	(file.Open(_ms("guild info [%s].txt",sList[m_iSelectedServer].strName),"at"))
		{
			for (int	i=0;i<_iListCount;i++)
				file.writeStreamCR("    [%s/%s]	LV %d	Rank %s	bestow %d",_lpMember[i].m_strId,_lpMember[i].m_strName,_lpMember[i].m_wLevel,strGuildRank[_lpMember[i].m_wRank],_lpMember[i].m_dwBestowValue);

			file.Close();
		}
	}
	else
	{
		fillData();
		cMSG::Put("received member info");
	}
}

void
cGuildInfo::receiveGuildNameList(BOOL _bIsBegin,BOOL _bIsLast,int _iCount,char _lpstrNameList[100][dGUILD_NAME_LENGTH],int _iServerIndex,int _iLastGuild)
{
	int	i;

	if (_bIsBegin)
	{
		m_iGuildCount	=	0;
		memset(m_strGuildList,0,sizeof(m_strGuildList));
	}

	for (i=0;i<_iCount;i++)
		strcpy(m_strGuildList[m_iGuildCount+i],_lpstrNameList[i]);

	m_iGuildCount	+=	_iCount;

	if	(_bIsLast)
	{
		m_listGuildList.reset();

		for (i=0;i<m_iGuildCount;i++)
			m_listGuildList.addItem(i,0,m_strGuildList[i],i);

		cMSG::Put("complete","receive guild name list");
	}
	else
	{
		SERVERStoWORLD_ASK_GUILD_NAME_LIST	packet;
		
		packet.base.set(sizeof(SERVERStoWORLD_ASK_GUILD_NAME_LIST),dSERVERStoWORLD_ASK_GUILD_NAME_LIST);
		packet.iFirstGuild	=	_iLastGuild+1;
		packet.iServerIndex	=	_iServerIndex;
		
		sList[_iServerIndex].worldsock.SendPacket((char *)&packet,packet.base.wSize);	
	}
}

BOOL cGuildInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_listMemberList.init(GetDlgItem(IDC_MEMBER_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	m_listMemberList.addColumn(0,30,"NO");
	m_listMemberList.addColumn(1,142,"Name");
	m_listMemberList.addColumn(2,90,"Rank");

	m_listSkillList.init(GetDlgItem(IDC_SKILL_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	m_listSkillList.addColumn(0,30,"NO");
	m_listSkillList.addColumn(1,100,"Name");
	m_listSkillList.addColumn(2,30,"LV");

	m_listGuildList.init(GetDlgItem(IDC_GUILD_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	m_listGuildList.addColumn(0,148,"Name");

	m_bIsWantAllGuildInfo	=	FALSE;
	memset(m_aMember,0xff,sizeof(m_aMember));
	m_guildInfo.reset();

	g_gsManager.init();
	SetCurrentDirectory(g_strLogFolder);

	m_iSelectedServer = -1;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cGuildInfo::OnGetGuildInfo() 
{
	if	(m_iSelectedServer==-1 && !sList[m_iSelectedServer].isConnect)
	{
		MessageBox("please select world","");
		return;
	}
	
	if	(m_iGuildCount	<=	0)
	{
		MessageBox("please ask guild list","");
		return;
	}

	BOOL	bIsOk	=	FALSE;

	int	iSelectGuild	=	m_listGuildList.getSelectedItemLParam(&bIsOk);

	if (bIsOk	==	FALSE)
	{
		MessageBox("please select guild","");
		return;
	}

	SERVERStoWORLD_GET_GUILDINFO	packet;

	packet.base.set(sizeof(SERVERStoWORLD_GET_GUILDINFO),dSERVERStoWORLD_GET_GUILDINFO);
	strcpy(packet.strGuildName,m_strGuildList[iSelectGuild]);
	packet.wGuildSerial	=	0xffff;

	sList[m_iSelectedServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);
}

void cGuildInfo::OnAskGuildList() 
{
	m_iSelectedServer = g_iSelectedServer;

	if (m_iSelectedServer==-1 || !sList[m_iSelectedServer].isConnect)
	{
		MessageBox("please select world","");
		return;
	}

	SERVERStoWORLD_ASK_GUILD_NAME_LIST	packet;

	packet.base.set(sizeof(SERVERStoWORLD_ASK_GUILD_NAME_LIST),dSERVERStoWORLD_ASK_GUILD_NAME_LIST);
	packet.iFirstGuild	=	0;
	packet.iServerIndex	=	m_iSelectedServer;

	sList[m_iSelectedServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);	
}

void cGuildInfo::OnSave() 
{
	// TODO: Add your control notification handler code here
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if(m_iSelectedServer == -1)
	{
		MessageBox(" Can't Save this infomation!","ERROR");
		return;
	}

	if (!sList[m_iSelectedServer].isConnect)
	{
		MessageBox("Do not connect to the world","");
		return;
	}

	SERVERStoWORLD_CHANGE_GUILD_BATTLE_REPORT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_GUILD_BATTLE_REPORT);

	strcpy(packet.strGuildMaster,m_guildInfo.m_strMasterName);
	strcpy(packet.strGuildName,m_guildInfo.m_strName);

	packet.wGuildSerial			=	m_guildInfo.m_wSerial;
	packet.wVictoryPoint		=	GetDlgItemInt(IDC_VP_POINT);
	packet.wThisWeekBattleCount	=	0;
	packet.wWinCount			=	GetDlgItemInt(IDC_WIN);
	packet.wDrawCount			=	GetDlgItemInt(IDC_DRAW);
	packet.wDefeatCount			=	GetDlgItemInt(IDC_DEFEAT);
	packet.wCVCount				=	GetDlgItemInt(IDC_CV_COUNT);

	if (m_guildInfo.m_iVictoryPoint	==packet.wVictoryPoint&&
		m_guildInfo.m_wVictoryCount	==packet.wWinCount&&
		m_guildInfo.m_wDrawCount	==packet.wDrawCount&&
		m_guildInfo.m_wDefeatCount	==packet.wDefeatCount&&
		m_guildInfo.m_wVictoryContinue==packet.wCVCount)
		return;

	if (cMSG::YESNO("please check follow info","\nVictory Point : %d->%d\n\nWin : %d->%d\n\nDraw : %d->%d\n\nDefeat : %d->%d\n\nContinuous Victory Count : %d->%d\n\nDo you want apply this data?",
			m_guildInfo.m_iVictoryPoint,packet.wVictoryPoint,
			m_guildInfo.m_wVictoryCount,packet.wWinCount,
			m_guildInfo.m_wDrawCount,packet.wDrawCount,
			m_guildInfo.m_wDefeatCount,packet.wDefeatCount,
			m_guildInfo.m_wVictoryContinue,packet.wCVCount)	==	IDNO)
		return;

	sList[m_iSelectedServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);	
}

void cGuildInfo::OnChangeGuildmaster() 
{
	// TODO: Add your control notification handler code here
	cDlgChangeGMaster		dlgCGM;

	int	nResponse	=	dlgCGM.DoModal();
}

void cGuildInfo::OnGuildInventory() 
{
	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;

#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_TESTER))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_TESTER))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_TESTER))	return;
#else
	if	(!CheckOperatorLevel(eAL_TESTER))
		return;
#endif
	CGuildInventory	inventory;

	inventory.setGuild(&m_guildInfo);
	
	if	(inventory.DoModal()	==	IDCANCEL)
		return;	
}

void cGuildInfo::OnPetAndGuardian() 
{
	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;

#ifdef	_FOR_CHINA
	if	(!CheckOperatorLevel(eAL_MASTER_OPERATER))
		return;
#elif	_FOR_JAPAN
	if	(!CheckOperatorLevel(eAL_TESTER))
		return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_TESTER))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_TESTER))	return;
#else
	if	(!CheckOperatorLevel(eAL_TESTER))
		return;
#endif
	CGuildPetAndGuardian	pg;

	pg.setGuild(&m_guildInfo);
	
	if	(pg.DoModal()	==	IDCANCEL)
		return;	

}

void cGuildInfo::OnTwinkleMark() 
{
	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;

	if	(!CheckOperatorLevel(eAL_OPERATOR))
	{
		CheckDlgButton(IDC_TWINKLE_MARK,m_guildInfo.m_bf1IsTwinkle);
		return;
	}

	// TODO: Add your control notification handler code here
#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("길드 마크 번쩍임 상태를 변경합니다.","변경된 상태는 바로 적용이 됩니다.\n\n작업을 계속 하시겠습니까?")	==	IDNO)
	{
		CheckDlgButton(IDC_TWINKLE_MARK,m_guildInfo.m_bf1IsTwinkle);
		return;
	}
#else
	if	(cMSG::YESNO("change guild mark twinkle status","are you continue?")	==	IDNO)
	{
		CheckDlgButton(IDC_TWINKLE_MARK,m_guildInfo.m_bf1IsTwinkle);
		return;
	}
#endif

	m_guildInfo.m_bf1IsTwinkle	=	1-m_guildInfo.m_bf1IsTwinkle;

	SendWP_SimpleWork(m_iSelectedServer,eSWAW_CHANGE_GUILD_MARK_TWINKLE_STATUS,m_guildInfo.m_wSerial,m_guildInfo.m_bf1IsTwinkle);
}

void cGuildInfo::OnResetAllGuildTwinkleStatus() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("모든 길드의 길드 마크 번쩍임 상태를 초기화 합니다.","변경된 상태는 바로 적용이 됩니다.\n\n작업을 계속 하시겠습니까?")	==	IDNO)
		return;
#else
	if	(cMSG::YESNO("reset all guild's mark twinkle status","are you continue?")	==	IDNO)
		return;
#endif

	SendWP_SimpleWork(m_iSelectedServer,eSWAW_CHANGE_GUILD_MARK_TWINKLE_STATUS,0xffff);
}

void cGuildInfo::OnChangeHallLevel() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;

#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("현재 길드의 길드홀 레벨을 변경합니다.","변경 하려는 레벨에 빈 길드홀이 있어야 합니다. 변경 하시겠습니까?")	==	IDNO)
		return;
#else
	if	(cMSG::YESNO("change current guild's hall level","are you continue?")	==	IDNO)
		return;
#endif

	// TODO: Add your control notification handler code here
	int	iHallLevel	=	GetDlgItemInt(IDC_HALL_LEVEL);

	if	(iHallLevel	<	0	||	iHallLevel	>	5)
	{
#ifdef	_FOR_KOREA
		cMSG::Put("길드 홀 레벨이 이상합니다.","0~5");
#else
		cMSG::Put("Incorrect Guild hall Level","0~5");
#endif
		return;
	}

	SendWP_SimpleWork(m_iSelectedServer,eSWAW_CHANGE_GUILD_HALL_LEVEL,m_guildInfo.m_wSerial,iHallLevel);

#ifdef	_FOR_KOREA
	cMSG::Put("레벨 변경을 시도합니다.","대상 레벨 길드홀이 2레벨 이상이면 해당 레벨에 비어 있는 길드홀이 있어야만 변경이 가능합니다.\n\n길드 정보를 다시 얻어 오시면 결과를 알 수 있습니다.");
#else
	cMSG::Put("Try to change level","please reload guild info and check level of guild hall");
#endif
}

void cGuildInfo::OnGetAll() 
{
	if	(m_iSelectedServer==-1 && !sList[m_iSelectedServer].isConnect)
	{
		MessageBox("please select world","");
		return;
	}
	
	if	(m_iGuildCount	<=	0)
	{
		MessageBox("please ask guild list","");
		return;
	}

	if	(m_bIsWantAllGuildInfo	==	FALSE)
	{
		MessageBox("you must click BEGIN button","");
		return;
	}

	SERVERStoWORLD_GET_GUILDINFO	packet;

	packet.base.set(sizeof(SERVERStoWORLD_GET_GUILDINFO),dSERVERStoWORLD_GET_GUILDINFO);

	packet.wGuildSerial	=	0xffff;

	strcpy(packet.strGuildName,m_strGuildList[m_iFirstGuild]);
	sList[m_iSelectedServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);
	sList[m_iSelectedServer].worldsock.ReallySend();

	m_iFirstGuild++;

	{
		m_listMemberList.reset();

		m_listMemberList.addItem(0,0," ",0);
		m_listMemberList.setItem(0,1,_ms("%d/%d",m_iFirstGuild,m_iGuildCount));
	}

	if	(m_iFirstGuild	>=	m_iGuildCount)
	{
		m_bIsWantAllGuildInfo	=	FALSE;
		MessageBox("complete to receive list","and export guild info");

		return;
	}

}

void cGuildInfo::OnRewindList() 
{
	m_iFirstGuild			=	0;
	m_bIsWantAllGuildInfo	=	TRUE;

	cFILE	file;

	file.Open(_ms("guild info [%s].txt",sList[m_iSelectedServer].strName),"wt");
	file.Close();
	// TODO: Add your control notification handler code here

}

void cGuildInfo::OnChangePlaceReated() 
{
	// TODO: Add your control notification handler code here

	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
	
	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;
	
#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("현재 길드의 연고지를 변경합니다.","변경 하시겠습니까?")	==	IDNO)
		return;
#else
	if	(cMSG::YESNO("change current guild's hall Place reated","are you continue?")	==	IDNO)
		return;
#endif

	char strGuildVillage[256];

	m_ebGuildVillage.GetWindowText(strGuildVillage , 256);

	cDECODER	decoder;

	if(!decoder.UploadBuffer(strGuildVillage,NULL))
	{
#ifdef	_FOR_KOREA
		cMSG::Put("정보가 이상합니다.","Check text");
#else
		cMSG::Put("Error","Check text");
#endif
		return;
	}

	BOOL		bIsSuccess;

	for (int i = 0 ; i < 7 ; i++)
	{
		m_guildInfo.m_awGuildVillage[i] = decoder.GetNumber(&bIsSuccess);
	}

	SendWP_LongSimpleWork(m_iSelectedServer,eSWAW_CHANGE_GUILD_VILLAGE_INFO,m_guildInfo.m_wSerial , m_guildInfo.m_awGuildVillage[0],
		m_guildInfo.m_awGuildVillage[1],m_guildInfo.m_awGuildVillage[2],m_guildInfo.m_awGuildVillage[3],
		m_guildInfo.m_awGuildVillage[4],m_guildInfo.m_awGuildVillage[5],m_guildInfo.m_awGuildVillage[6], 0 , 0 , 0 , 0);
	
#ifdef	_FOR_KOREA
	cMSG::Put("길드 연고지를 변경합니다.","길드 정보를 다시 로드하셔서 확인해 주세요.");
#else
	cMSG::Put("Try to change Guild Place Reated","please reload guild info and check Guild Place Reated");
#endif
	
}

void cGuildInfo::OnButtonSaveGuildEtc() 
{
	// TODO: Add your control notification handler code here
	
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;
	
	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;
	
#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("길드 마스터 은총 , 길드 던젼 플레이 횟수 , 로엔의 호감도를 변경하시겠습니까?." , "변경 하시겟습니까?")	==	IDNO)
		return;
#else
	if	(cMSG::YESNO("change current guild's Master Grace , Dungen PlayTime ROEN`s GOOD WILL reated","are you continue?")	==	IDNO)
		return;
#endif
	
	SendWP_SimpleWork(m_iSelectedServer , eSWAW_CHANGE_GUILD_INFO_ETC_BY_ADMINTOOL , m_guildInfo.m_wSerial , GetDlgItemInt(IDC_GUILD_MASTER_GRACE) , GetDlgItemInt(IDC_ROEN_GOOD_WILL));
	
#ifdef	_FOR_KOREA
	cMSG::Put("길드 연고지를 변경합니다.","길드 정보를 다시 로드하셔서 확인해 주세요.");
#else
	cMSG::Put("Try to change Guild Place Reated","please reload guild info and check Guild Place Reated");
#endif
	
}

void cGuildInfo::OnSaveCountOfGpb() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;

#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("현재 길드의 길드 포인트전 횟수를 변경합니다.","변경 하시겠습니까?")	==	IDNO)
		return;
#else
	if	(cMSG::YESNO("change current guild's guild point battle count","are you continue?")	==	IDNO)
		return;
#endif

	// TODO: Add your control notification handler code here
	int	iCount	=	GetDlgItemInt(IDC_COUNT_OF_POINT_BATTLE);

	if	(iCount	<	0	||	iCount	>	2)
	{
#ifdef	_FOR_KOREA
		cMSG::Error("변경하려는 길드 포인트전 횟수가 이상합니다.","0~2");
#else
		cMSG::Error("Incorrect Count","0~2");
#endif
		return;
	}

	SendWP_SimpleWork(m_iSelectedServer,eSWAW_CHANGE_GUILD_POINT_BATTLE_COUNT,m_guildInfo.m_wSerial,iCount);

#ifdef	_FOR_KOREA
	cMSG::Put("횟수 변경을 시도합니다.","길드 정보를 다시 얻어 오시면 결과를 알 수 있습니다.");
#else
	cMSG::Put("Try to change count","please reload guild info and check count");
#endif
	
}

void cGuildInfo::OnSyncLoadedPlayer() 
{
	// TODO: Add your control notification handler code here
	if	(g_lpCharacterInfo->m_iSelectWorld	>=	g_lpCharacterInfo->m_iWorldCount || g_lpCharacterInfo->m_iSelectWorld < 0	||	g_lpCharacterInfo->m_am.m_iLoadDataCount	<=	0)
	{
#ifdef	_FOR_KOREA
		cMSG::Error("로딩된 캐릭터 길드 정보 싱크","먼저 캐릭터 탭에 캐릭터를 로딩해 주세요.");
#else
		cMSG::Error("Try to sync member info","please load character data in character tab");
#endif
		return;
	}

	if	(m_guildInfo.m_wSerial	==	0xffff)
	{
#ifdef	_FOR_KOREA
		cMSG::Error("로딩된 캐릭터 길드 정보 싱크","먼저 길드 정보를 로딩해 주세요.");
#else
		cMSG::Error("Try to sync member info","please load guild info");
#endif
		return;
	}

	CAvatarData		*lpPlayer	=	&g_lpCharacterInfo->m_am.m_aData[g_lpCharacterInfo->m_iSelectAvatar];

	for (int i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if	(m_aMember[i].wSerial	==	0xffff)
			continue;

		if	(strcmp(m_aMember[i].m_strName,lpPlayer->m_strName) == 0)
		{
			lpPlayer->m_wGuildSerial	=	m_guildInfo.m_wSerial;
			lpPlayer->m_wGuildSerial	=	m_aMember[i].m_wRank;

			g_lpCharacterInfo->SetDlgItemText(IDC_GUILD_RANK		,g_strToolMessage[126+m_aMember[i].m_wRank]);
			g_lpCharacterInfo->SetDlgItemInt(IDC_GUILD				,lpPlayer->m_wGuildSerial,FALSE);
			g_lpCharacterInfo->SetDlgItemInt(IDC_GUILD_JOIN_YEAR	,lpPlayer->m_timeGuildJoinTime.year,FALSE);
			g_lpCharacterInfo->SetDlgItemInt(IDC_GUILD_JOIN_MONTH	,lpPlayer->m_timeGuildJoinTime.month,FALSE);
			g_lpCharacterInfo->SetDlgItemInt(IDC_GUILD_JOIN_DAY		,lpPlayer->m_timeGuildJoinTime.day,FALSE);

#ifdef	_FOR_KOREA
			cMSG::Put("로딩된 캐릭터 길드 정보 싱크","멤버 정보를 발견해 길드 정보를 싱크 시켰습니다.\n\n캐릭터 탭에서 해당 캐릭터 데이터를 저장해야 적용됩니다.");
#else
			cMSG::Put("Try to sync member info","sync member info");
#endif
			return;
		}
	}

#ifdef	_FOR_KOREA
	cMSG::Error("로딩된 캐릭터 길드 정보 싱크","현재 길드에서 해당 캐릭터를 찾을 수 없습니다.");
#else
	cMSG::Error("Try to sync member info","can not find loaded character info");
#endif

//	m_guildInfo

}

void cGuildInfo::OnRclickMemberList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HMENU	hMenu	=	CreatePopupMenu();
	HMENU	hRank	=	CreatePopupMenu();

	AppendMenu(hRank, MF_STRING, 10000+dGUILD_CLASS_ROOKIE	,"Rookie");
	AppendMenu(hRank, MF_STRING, 10000+dGUILD_CLASS_MEMBER	,"Member");
	AppendMenu(hRank, MF_STRING, 10000+dGUILD_CLASS_PARLIAMENT,"Congress");
	AppendMenu(hRank, MF_STRING, 10000+dGUILD_CLASS_SUBMASTER	,"Sub Master");
	AppendMenu(hRank, MF_STRING, 10000+dGUILD_CLASS_ELDER		,"Elder");
	AppendMenu(hRank, MF_STRING, 10000+dGUILD_CLASS_MASTER		,"Master");

	POINT	pos;

// 	AppendMenu(m_hMenu,MFT_STRING			,IDM_PREVIOUS_WORK		,dMSG_CANCEL_EDIT);
// 	AppendMenu(m_hMenu,MFT_SEPARATOR		,0						,NULL);
// 	AppendMenu(m_hMenu,MF_STRING		,IDM_SELECT_CHARACTER	,dMSG_SELECT_CHARACTER);
	AppendMenu(hMenu,MF_POPUP			,(UINT) hRank	,"Change Member Rank");

	GetCursorPos(&pos);
	TrackPopupMenu(hMenu,TPM_RIGHTBUTTON|TPM_TOPALIGN|TPM_LEFTALIGN,pos.x,pos.y,0,m_hWnd,NULL);
	DestroyMenu(hMenu);
		
	*pResult = 0;
}

BOOL cGuildInfo::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL cGuildInfo::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	BOOL	bIsSelected		=	FALSE;
	int		iSelectMember	=	m_listMemberList.getSelectedItemLParam(&bIsSelected);

	if	(bIsSelected)
	{
		SERVERStoWORLD_USER_CHANGERANK		packet;

		packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);
		packet.wGuildSerial		=	m_guildInfo.m_wSerial;

		strcpy(packet.strMaster,"#admin");
		strcpy(packet.strTarget,m_aMember[iSelectMember].m_strName);

		packet.wType	=	dGUILD_APPONT;

		char	astrMemberRank[10][32]	=	
		{
			"","rookie","member","congress","sub master","elder","master",
		};

		switch( LOWORD( wParam ) )
		{
			case	10000+dGUILD_CLASS_ROOKIE		:
			case	10000+dGUILD_CLASS_MEMBER		:
			case	10000+dGUILD_CLASS_PARLIAMENT	:
			case	10000+dGUILD_CLASS_SUBMASTER	:
			case	10000+dGUILD_CLASS_ELDER		:
			case	10000+dGUILD_CLASS_MASTER		:
			{
				if	(m_iSelectedServer==-1 && !sList[m_iSelectedServer].isConnect)
				{
					MessageBox("please select world","");
					break;
				}
				
				if	(m_iGuildCount	<=	0)
				{
					MessageBox("please ask guild list","");
					break;
				}

				packet.wRank	=	LOWORD( wParam )-10000;

				if	(cMSG::YESNO("do you want change member rank?","target member : %s\n\ntarget rank : %s",
						m_aMember[iSelectMember].m_strName,astrMemberRank[packet.wRank])	==	IDNO)
					break;

				sList[m_iSelectedServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);
				sList[m_iSelectedServer].worldsock.ReallySend();

				cMSG::Put("ask to world change member rank","please re-ask guild info and check result");
				break;
			}
		}
	}
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnCommand(wParam, lParam);
}

void cGuildInfo::OnSaveTodayGuildBattleCount() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;

#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("현재 길드의 길드 포인트전 횟수를 변경합니다.","변경 하시겠습니까?")	==	IDNO)
		return;
#else
	if	(cMSG::YESNO("change current guild's guild point battle count","are you continue?")	==	IDNO)
		return;
#endif

	// TODO: Add your control notification handler code here
	int	iCount	=	GetDlgItemInt(IDC_TODAY_GUILD_BATTLE_COUNT);

	SendWP_SimpleWork(m_iSelectedServer,eSWAW_CHANGE_TODAY_GUILD_BATTLE_COUNT,m_guildInfo.m_wSerial,iCount);

#ifdef	_FOR_KOREA
	cMSG::Put("횟수 변경을 시도합니다.","길드 정보를 다시 얻어 오시면 결과를 알 수 있습니다.");
#else
	cMSG::Put("Try to change count","please reload guild info and check count");
#endif
}

void cGuildInfo::OnSaveThisWeekGuildBattleCount() 
{
	if	(!CheckOperatorLevel(eAL_OPERATOR))
		return;

	if	(m_guildInfo.m_wSerial	==	0xffff)
		return;

#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("현재 길드의 길드 포인트전 횟수를 변경합니다.","변경 하시겠습니까?")	==	IDNO)
		return;
#else
	if	(cMSG::YESNO("change current guild's guild point battle count","are you continue?")	==	IDNO)
		return;
#endif

	// TODO: Add your control notification handler code here
	int	iCount	=	GetDlgItemInt(IDC_THIS_WEEK_GUILD_BATTLE_COUNT);

	SendWP_SimpleWork(m_iSelectedServer,eSWAW_CHANGE_THIS_WEEK_GUILD_BATTLE_COUNT,m_guildInfo.m_wSerial,iCount);

#ifdef	_FOR_KOREA
	cMSG::Put("횟수 변경을 시도합니다.","길드 정보를 다시 얻어 오시면 결과를 알 수 있습니다.");
#else
	cMSG::Put("Try to change count","please reload guild info and check count");
#endif	
}
