// ServerTool.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "ServerTool.h"
#include "CMessage.h"
#include "CItem.h"
#include "CDecoder.h"
#include "cPACKET_WORLDSERVER.h"
#include "RenewItemExpireDate.h"
#include "cSRVUTIL.h"
#include "characterInfo.h"
#include "quest.h"
#include "cGuildDefine.h"
#include "assert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//char	*l_strServerName[10]	=	{"프란델","에립트","제노스","가디우스","바헬","end"};

HANDLE	g_hConsole	=	0;


void
Printf(char *str,...)
{	
	static	char	s_tempStr[1024];
	va_list argptr;
	va_start(argptr, str);
	vsprintf(s_tempStr,str,argptr);
	va_end(argptr);

	DWORD	dwWritten;

	WriteConsole(g_hConsole, s_tempStr, (int)strlen(s_tempStr), &dwWritten, 0);
}

// int
// GetServerIndexForEvent(char *_lpstrServerName)
// {
// 	if	(!_lpstrServerName)
// 		return	-1;
// 
// 	for (int i=0;;i++)
// 	{
// 		if	(stricmp(l_strServerName[i],"end")==0)
// 			return	-1;
// 
// 		if	(stricmp(l_strServerName[i],_lpstrServerName)==0)
// 			return	i;
// 	}
// 
// 	return	-1;
// }

int
GetServerIndexForEvent(char *_lpstrServerName)
{
	if	(!_lpstrServerName)
		return	-1;

	for (int i=0;;i++)
	{
		if	(sList[i].strName[0]	==	0)
			break;

		if	(stricmp(sList[i].strName,_lpstrServerName)==0)
			return	i;
	}

	return	-1;
}

char*
GetServerNameForEvent(int _iIndex)
{
	if	(_iIndex	<	0)
		return	"";

	return	sList[_iIndex].strName;
}

/////////////////////////////////////////////////////////////////////////////
// CServerTool dialog

CServerTool::CServerTool(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CServerTool::IDD)
{
	memset(m_abDBConnetionInfo,0,sizeof(m_abDBConnetionInfo));

	//{{AFX_DATA_INIT(CServerTool)
	//}}AFX_DATA_INIT
}


void CServerTool::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerTool)
	DDX_Control(pDX, IDC_LOAD_DB_NAME, m_editLoadDbName);
	DDX_Control(pDX, IDC_GOLD_LIMIT, m_editGoldLimit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerTool, CPropertyPage)
	//{{AFX_MSG_MAP(CServerTool)
	ON_BN_CLICKED(IDC_CHECK_RICH, OnCheckRich)
	ON_BN_CLICKED(IDC_SERCH_ITEM, OnSerchItem)
	ON_BN_CLICKED(IDC_EXPORT_UNIQUE_ITEM, OnExportUniqueItem)
	ON_BN_CLICKED(IDC_UPDATE_LIST, OnUpdateList)
	ON_BN_CLICKED(IDC_ADD_REQUITAL, OnAddRequital)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_CHECK_QUEST_050707, OnCheckQuest050707)
	ON_BN_CLICKED(IDC_CHECK_QUEST_50708, OnCheckQuest50708)
	ON_BN_CLICKED(IDC_CHECK_QUEST_50709, OnCheckQuest50709)
	ON_BN_CLICKED(IDC_CHECK_QUEST_50710, OnCheckQuest50710)
	ON_BN_CLICKED(IDC_CHECK_QUEST_50711, OnCheckQuest50711)
	ON_BN_CLICKED(IDC_CHECK_QUEST_50712, OnCheckQuest50712)
	ON_BN_CLICKED(IDC_050720, On050720)
	ON_BN_CLICKED(IDC_50721, On50721)
	ON_BN_CLICKED(IDC_50722, On50722)
	ON_BN_CLICKED(IDC_50723, On50723)
	ON_BN_CLICKED(IDC_50724, On50724)
	ON_BN_CLICKED(IDC_050818, On050818)
	ON_BN_CLICKED(IDC_50819, On50819)
	ON_BN_CLICKED(IDC_BUILD_GUILD_BATTLE_PROLOG_RESULT, OnBuildGuildBattlePrologResult)
	ON_BN_CLICKED(IDC_ROOKIE_EVENT_090720, OnRookieEvent090720)
	ON_BN_CLICKED(IDC_ROOKIE_EVENT_RUN_090720, OnRookieEventRun090720)
	ON_BN_CLICKED(IDC_100721, On100721)
	ON_BN_CLICKED(IDC_REWARD_OLD_REBIRTHER, OnRewardOldRebirther)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerTool message handlers

void CServerTool::OnCheckRich() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	char	strText[256];

	m_editGoldLimit.GetWindowText(strText,sizeof(strText));

	if	(strlen(strText) <= 2)
		return;

	DWORD	dwGold	=	atoi(strText);
	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	if	(!m_db.Connect(	sList[g_iSelectedServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
	{
		MessageBox(g_strToolMessage[50]);

		return;
	}

	char	strFileName[256];
	sprintf(strFileName,"[%s]checkGold.txt",sList[g_iSelectedServer].strName);

	int	iCount	=	m_db.checkRich(strFileName,dwGold);

	m_db.Close();

	sprintf(strText,"exist '%d' numbers player in server '%s'\n\ncheck '%s' file",iCount,sList[g_iSelectedServer].strName,strFileName);

	MessageBox(strText);

	// TODO: Add your control notification handler code here
	
}

BOOL
CServerTool::ConnectAllServerDB()
{
	char	strDbName[256];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

// 	cMSG::Put("connect all DB server","try");

	for	(int iServer=0;iServer<20;iServer++)
	{
// 		if	(stricmp(GetServerNameForEvent(iServer),"end")==0)
// 			break;

		if	(m_abDBConnetionInfo[iServer])
			continue;

// 		cMSG::Put("connect all DB server","[%d][%s:%s]",iServer,sList[iServer].strName,sList[iServer].strDBIP);

		if	(sList[iServer].strName[0]	==	0)
			break;
		
		if	(!m_aDb[iServer].Connect(sList[iServer].strDBIP,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
			return	FALSE;

		m_abDBConnetionInfo[iServer]	=	TRUE;
	}

// 	cMSG::Put("connect all DB server","end");

	return	TRUE;
}

BOOL CServerTool::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SetDlgItemText(IDC_LOAD_DB_NAME,"RedStone_Avatar_Source");

	int size=  0;
	size = sizeof(cRenewItemExpireTimeInfo)*1024;
	size = sizeof(cQuestOperatorInfo050708)*1024;
	size = sizeof(cQuestOperatorInfo050708ForQuest)*1024;
#ifndef _FOR_KOREA
	SetDlgItemText(IDC_ADD_REQUITAL,g_strDlgText[170]);
	SetDlgItemText(IDC_STATIC_REQUITAL,g_strDlgText[171]);
	SetDlgItemText(IDC_UPDATE_LIST,g_strDlgText[172]);
	SetDlgItemText(IDC_CHECK_RICH,g_strDlgText[173]);
	SetDlgItemText(IDC_SERCH_ITEM,g_strDlgText[174]);
	SetDlgItemText(IDC_EXPORT_UNIQUE_ITEM,g_strDlgText[175]);
#endif

	m_listExtendItemDeathTime.init(GetDlgItem(IDC_REQUITAL_LIST)->m_hWnd,0,0);
	::SendMessage(GetDlgItem(IDC_REQUITAL_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );
	

	m_listExtendItemDeathTime.addColumn(0,60 ,"Index");
	m_listExtendItemDeathTime.addColumn(1,120,"Date");
	m_listExtendItemDeathTime.addColumn(2,240,"Extend Time");
#ifdef	_FOR_KOREA
	m_listExtendItemDeathTime.addColumn(3,78,"미니펫만");
	m_listExtendItemDeathTime.addColumn(4,78,"우리회원만");
#endif
	m_listExtendItemDeathTime.reset();

	CQuest::Load();

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerTool::OnSerchItem() 
{
	if (g_iSelectedServer	>=	g_iWorldCount || g_iSelectedServer < 0)
	{
		MessageBox("please select world","");
		return;
	}

#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	OPENFILENAME	ofn;

	char	dir[512];
	char	strName[256]	=	"";

	sprintf(dir,"%s\\Item List",g_strLogFolder);

	SetCurrentDirectory(dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.txt";
	ofn.lpstrTitle		=	"Find Item Text";
	ofn.lpstrFile		=	strName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"txt";
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	cDECODER	decoder;

	if (!decoder.Upload(strName,NULL))	return;

	#define	dMAX_FIND_ITEM_COUNT	100

	int			iProcessIndex	=	0,iCount=0,iFindItemCount=0;
	cITEM		aFindItemList[dMAX_FIND_ITEM_COUNT];
	DWORD		dwRecoverGold = 0;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		if (stricmp(lpstrWord,"IT")	==	0)
		{
			cITEM	*lpItem	=	&aFindItemList[iFindItemCount];
			lpItem->reset();

			for (iProcessIndex =0;iProcessIndex<22;iProcessIndex++)
			{
				BOOL	bIsSuccess;
				DWORD	dwValue		=	decoder.GetNumber(&bIsSuccess);

				if (!bIsSuccess)	break;

				if (iProcessIndex	==	0)	lpItem->m_wBaseItem				=	(WORD)dwValue;
				if (iProcessIndex	==	1)	lpItem->m_dwSerial				=	(DWORD)dwValue;
				if (iProcessIndex	==	2)	lpItem->m_bCount				=	(BYTE)dwValue;
				if (iProcessIndex	==	3)	lpItem->m_bDurability			=	(BYTE)dwValue;

				if (iProcessIndex	==	4)	lpItem->m_aOption[0]			=	(BYTE)dwValue;
				if (iProcessIndex	==	5)	lpItem->m_aOption[1]			=	(BYTE)dwValue;

				if (iProcessIndex	==	6)	lpItem->m_aPrefix[0].m_wPrefix	=	(WORD)dwValue;
				if (iProcessIndex	==	7)	lpItem->m_aPrefix[0].m_aValue[0]=	(BYTE)dwValue;
				if (iProcessIndex	==	8)	lpItem->m_aPrefix[0].m_aValue[1]=	(BYTE)dwValue;

				if (iProcessIndex	==	9)	lpItem->m_aPrefix[1].m_wPrefix	=	(WORD)dwValue;
				if (iProcessIndex	==	10)	lpItem->m_aPrefix[1].m_aValue[0]=	(BYTE)dwValue;
				if (iProcessIndex	==	11)	lpItem->m_aPrefix[1].m_aValue[1]=	(BYTE)dwValue;

				if (iProcessIndex	==	12)	lpItem->m_aPrefix[2].m_wPrefix	=	(WORD)dwValue;
				if (iProcessIndex	==	13)	lpItem->m_aPrefix[2].m_aValue[0]=	(BYTE)dwValue;
				if (iProcessIndex	==	14)	lpItem->m_aPrefix[2].m_aValue[1]=	(BYTE)dwValue;

				if (iProcessIndex	==	15)	lpItem->setBitFieldValue((DWORD)dwValue);
			}

			iFindItemCount++;

			if (iFindItemCount	>=	dMAX_FIND_ITEM_COUNT)
				break;

			continue;
		}

		if (stricmp(lpstrWord,"isEndCode")	==	0)
			break;
	}

	char	strDbName[256],strText[128];

	m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

	if(!m_db.Connect(	sList[g_iSelectedServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
	{
		MessageBox(g_strToolMessage[50]);
		return;
	}

	sprintf(strText,"Check Item in server '%s'.txt",sList[g_iSelectedServer].strName);

	m_db.findItem(aFindItemList,iFindItemCount,strText);

	m_db.Close();

	ShellExecute(NULL,"open",strText,NULL,NULL,SW_SHOWNORMAL);
}

void CServerTool::OnExportUniqueItem() 
{
#ifdef _FOR_CHINA
	return;
#endif
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	if (g_iSelectedServer	>=	g_iWorldCount || g_iSelectedServer < 0)
	{
		MessageBox("please select world","");
		return;
	}

	_RecordsetPtr	pRs = NULL; 	
	_variant_t		v_bufferdata;
	char			str_Query[1024];
	int				i_RecCount;
	int				i,j;
	char			*pBuf = NULL;
	HRESULT			hr = CoInitialize(0);
	char			strExportDir[512],strExportFileName[512];

	sprintf(strExportDir,"Export good item list in server '%s'",sList[g_iSelectedServer].strName);

	if (cMSG::YESNO(strExportDir,"Continue?") == IDNO) return;

	//	DB에 연결
	{
		char	strDbName[512];

		m_editLoadDbName.GetWindowText(strDbName,sizeof(strDbName));

		if(!m_db.Connect(	sList[g_iSelectedServer].strDBIP	,strAvatarDBCId,strAvatarDBCPw,strDbName,1))
		{
			MessageBox(g_strToolMessage[50]);
			return;
		}
	}

	sprintf(str_Query,"SELECT data FROM Avatar_Current");

	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)m_db.GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	SetCurrentDirectory(g_strLogFolder);

	sprintf(strExportDir,"%s\\Item List",g_strLogFolder);
	sprintf(strExportFileName,"Good Item List - %s.txt",sList[g_iSelectedServer].strName);
	SetCurrentDirectory(strExportDir);

	cFILE	file,file2;
	int		iItemCount	=	0;

	file.Open(strExportFileName,"wb",TRUE);
	sprintf(strExportFileName,"Good Item List - %s.dat",sList[g_iSelectedServer].strName);
	file2.Open(strExportFileName,"wb",TRUE);

	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			cPLAYER_DATA	data;
			cITEM			*lpItem;

			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			m_db.VariantToBlob(v_bufferdata,&data);

			BOOL	bIsFirst			=	TRUE;
			BOOL	bIsIncorrectPrefix	=	FALSE;

			for (j=0;j<dEQUIPMENT_PART_COUNT;j++)
			{
				lpItem	=	(cITEM *)&data.m_aEquip[j];

				if (lpItem->m_wBaseItem	>=	dBASIC_ITEM_COUNT)	continue;
				if (!lpItem->isHighLevelMagicOptionItem() && !lpItem->isUniqueItem())	continue;

				if (bIsFirst)	file.writeStreamCR("[%-20s/%-20s]",data.m_strId,data.m_strName);

				bIsFirst	=	FALSE;

				iItemCount++;
				file2.Write(lpItem,sizeof(cITEM));

				file.writeStreamCR("    [%-30s] [%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d       [%.2d/%.2d/%.2d] [%.2d:%.2d]",
									lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
									lpItem->m_aOption[0],lpItem->m_aOption[1],
									lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
									lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
									lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
									lpItem->getBitFieldValue()			,
									lpItem->m_year	,lpItem->m_month	,lpItem->m_day	,lpItem->m_hour	,lpItem->m_minute	);
			}

			for (j=0;j<dOWN_ITEM_COUNT;j++)
			{
				lpItem	=	(cITEM *)&data.m_aItems[j];

				if (lpItem->m_wBaseItem	>=	dBASIC_ITEM_COUNT)	continue;
				if (!lpItem->isHighLevelMagicOptionItem() && !lpItem->isUniqueItem())	continue;

				if (bIsFirst)	file.writeStreamCR("[%-20s/%-20s]",data.m_strId,data.m_strName);

				bIsFirst	=	FALSE;

				iItemCount++;
				file2.Write(lpItem,sizeof(cITEM));
				file.writeStreamCR("    [%-30s] [%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d       [%.2d/%.2d/%.2d] [%.2d:%.2d]",
									lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
									lpItem->m_aOption[0],lpItem->m_aOption[1],
									lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
									lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
									lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
									lpItem->getBitFieldValue()			,
									lpItem->m_year	,lpItem->m_month	,lpItem->m_day	,lpItem->m_hour	,lpItem->m_minute	);
			}
			pRs->MoveNext();
		}
	}

	file2.Write(&iItemCount,4);
	file.Close();
	file2.Close();

	pRs->Close();
	m_db.Close();

	sprintf(strExportDir,"Export Good item List in server '%s'",sList[g_iSelectedServer].strName);

	MessageBox(strExportDir);

	return;

}

void CServerTool::OnUpdateList() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	if (g_iSelectedServer	>=	g_iWorldCount || g_iSelectedServer < 0)
	{
		MessageBox("please select world","");
		return;
	}

	if (sList[g_iSelectedServer].isConnect	==	FALSE)
	{
		MessageBox(g_strToolMessage[10]);
		return;
	}

	m_listExtendItemDeathTime.reset();

	m_iRenewItemExpireTimeInfoCount	=	0;

	SERVERStoWORLD_ASK_RENEW_ITEM_EXPIRE_TIME_LIST	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ASK_RENEW_ITEM_EXPIRE_TIME_LIST);

	sList[g_iSelectedServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);	
}

void CServerTool::OnAddRequital() 
{

#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#endif

	if (g_iSelectedServer	>=	g_iWorldCount || g_iSelectedServer < 0)
	{
		MessageBox("please select world","");
		return;
	}

	if (sList[g_iSelectedServer].isConnect	==	FALSE)
	{
		MessageBox(g_strToolMessage[10]);
		return;
	}

	RenewItemExpireDate	dlg;

	if (dlg.open(sList[g_iSelectedServer].strName)	==	IDCANCEL)
		return;

	SERVERStoWORLD_ADD_RENEW_ITEM_EXPIRE_TIME	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ADD_RENEW_ITEM_EXPIRE_TIME);

	memcpy(&packet.time,&dlg.m_time,sizeof(UTime));
	packet.dwRenewMinute	=	dlg.m_dwRenewMunute;
	packet.wIsForOnlyYahoo	=	dlg.m_wIsForOnlyYahoo;
	packet.wIsForOnlyRedgem	=	dlg.m_wIsForOnlyRedgem;

	sList[g_iSelectedServer].worldsock.SendPacket((char *)&packet,packet.base.wSize);	
}

void
CServerTool::receiveList(cRenewItemExpireTimeInfo *_lpList,int _iCount,int _iFullCount,BOOL _bIsLast)
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	int	i;
	for (i=0;i<_iCount;i++)
	{
		cRenewItemExpireTimeInfo	*lpDest,*lpSrc;

		lpSrc	=	&_lpList[i];
		lpDest	=	&m_aRenewItemExpireTimeInfoList[lpSrc->m_bf14Index-1];

		memcpy(lpDest,lpSrc,sizeof(cRenewItemExpireTimeInfo));
	}

	if (_bIsLast	==	FALSE)
		return;

	m_iRenewItemExpireTimeInfoCount	=	_iFullCount;

	m_listExtendItemDeathTime.reset();

	if (m_iRenewItemExpireTimeInfoCount	==	0)
	{
		m_listExtendItemDeathTime.addItem(0,0," ");
		m_listExtendItemDeathTime.setItem(0,1,"invalid list");
	}

	for (i=0;i<m_iRenewItemExpireTimeInfoCount;i++)
	{
		cRenewItemExpireTimeInfo	*lpInfo	=	&m_aRenewItemExpireTimeInfoList[i];

		m_listExtendItemDeathTime.addItem(i,0,_ms("%3d",i));
		m_listExtendItemDeathTime.setItem(i,1,_ms("%.2d/%.2d/%.2d %.2d:%.2d",lpInfo->m_time.year,lpInfo->m_time.month,lpInfo->m_time.day,lpInfo->m_time.hour,lpInfo->m_time.minute));

		char	strText[60];
		int		iDays	=	lpInfo->m_dwRenewMinuite/60/24;
		int		iHours	=	(lpInfo->m_dwRenewMinuite/60)%24;
		int		iMinutes=	lpInfo->m_dwRenewMinuite%60;
#ifdef	_FOR_KOREA
		if (iDays)
		{
			if (iHours && iMinutes)
				sprintf(strText,"%d 일 %d 시간 %d 분",iDays,iHours,iMinutes);
			else
			if (iHours)
				sprintf(strText,"%d 일 %d 시간",iDays,iHours);
			else
			if (iMinutes)
				sprintf(strText,"%d 일 %d 분",iDays,iMinutes);
			else
				sprintf(strText,"%d 일",iDays);
		}
		else
		if (iHours)
		{
			if (iMinutes)
				sprintf(strText,"%d 시간 %d 분",iHours,iMinutes);
			else
				sprintf(strText,"%d 시간",iHours);
		}
		else
			sprintf(strText,"%d 분",iMinutes);
#else
		if (iDays)
		{
			if (iHours && iMinutes)
				sprintf(strText,"%d Days %d Hours %d Minutes",iDays,iHours,iMinutes);
			else
			if (iHours)
				sprintf(strText,"%d Days %d Hours",iDays,iHours);
			else
			if (iMinutes)
				sprintf(strText,"%d Days %d Minutes",iDays,iMinutes);
			else
				sprintf(strText,"%d Days",iDays);
		}
		else
		if (iHours)
		{
			if (iMinutes)
				sprintf(strText,"%d Hours %d Minutes",iHours,iMinutes);
			else
				sprintf(strText,"%d Hours",iHours);
		}
		else
			sprintf(strText,"%d Minutes",iMinutes);
#endif
		m_listExtendItemDeathTime.setItem(i,2,strText);

#ifdef	_FOR_KOREA
		char	*strOX[]	=	{"X","O"};

		m_listExtendItemDeathTime.setItem(i,3,strOX[lpInfo->m_bf1IsForOnlyYahoo]);
		m_listExtendItemDeathTime.setItem(i,4,strOX[lpInfo->m_bf1IsForOnlyRedGem]);
#endif
	}
}

void CServerTool::OnButton3()
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if	(!CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	FILE * fpGuildMarkStat = NULL;	
	fpGuildMarkStat = fopen("logChangeGuildInfo.txt","w+");
	if(!fpGuildMarkStat)	{	MessageBox("ERROR !");	return;}


	CGuildAdvanceInfo		newGuildInfo;

	for(int i=0;i<dMAX_WORLD_SERVER_COUNT;i++)
	{
		if(strcmp(sList[i].strDBIP,"")==0)	continue;
	
		_log("idx [%d] -> ip [%s] , id [%s], pw [%s] ",i,sList[i].strDBIP,strAvatarDBCId,strAvatarDBCPw);
		if(!m_db.Connect(sList[i].strDBIP,strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
		{
			fprintf(fpGuildMarkStat, "Connet failed to SQL DB [%s] ",sList[i].strDBIP);
			_log("Connet failed to SQL DB [%s] ",sList[i].strDBIP);
			continue;
		}

/*		cADO_ADMIN	backupDB;
		if(!backupDB.Connect(sList[i].strDBIP,strAvatarDBCId,strAvatarDBCPw,"Redstone_Avatar_Source",1))
		{
			continue;
		}
*/	
		GuildListDBOld	* plist = new GuildListDBOld[dGUILD_MAX_COUNT];
		int count = m_db.spGetAllOldGuildInfo(plist);

		fprintf(fpGuildMarkStat, "[%s][%s]Total Guild Count [%d] \n",sList[i].strName,sList[i].strDBIP,count);
		fprintf(fpGuildMarkStat, "==========================================================================\n");



		for(int j=0;j<count;j++)
		{


		//	저장한다.
			CGuildSaveInfoForColumn	ColumnInfo;
			CGuildSaveInfoForBinary	BinaryInfo;
			CGuildSaveInfoMark		MarkInfo;
			CGuildSaveInfoBank		BankInfo;
			CGuildSaveInfoRelShip	RelShipInfo;
			CGuildSaveInfoSkill		SkillInfo;
			CGuildSaveInfoPet		PetInfo;
			CGuildSaveInfoGuardian	GuardianInfo;
			CGuildSaveInfoInventory	InventoryInfo;
			CGuildSaveInfoStructure	StructureInfo;

			newGuildInfo.getSaveData_Column(&ColumnInfo);
			newGuildInfo.getSaveData_Binary(&BinaryInfo);
			newGuildInfo.getSaveData_Mark(&MarkInfo);
			newGuildInfo.getSaveData_Bank(&BankInfo);
			newGuildInfo.getSaveData_RelShip(&RelShipInfo);
			newGuildInfo.getSaveData_Skill(&SkillInfo);
			newGuildInfo.getSaveData_Pet(&PetInfo);
			newGuildInfo.getSaveData_Guardian(&GuardianInfo);
			newGuildInfo.getSaveData_Inventory(&InventoryInfo);
			newGuildInfo.getSaveData_Structure(&StructureInfo);

			int ret =0;
			ret = m_db.spSaveGuildInfo(&ColumnInfo,&BinaryInfo,&MarkInfo,&BankInfo,&RelShipInfo);
			ret = m_db.spInsertGuildInfo(&ColumnInfo,&BinaryInfo,&MarkInfo,&BankInfo,&RelShipInfo,&SkillInfo);
			ret = m_db.spInsertGuildData(newGuildInfo.m_wSerial,newGuildInfo.m_strName,&PetInfo,&GuardianInfo,&InventoryInfo,&StructureInfo);
		}
		fprintf(fpGuildMarkStat, "==========================================================================\n");

		m_db.Close();
		delete [] plist;
	}
	fclose(fpGuildMarkStat);
}

void CServerTool::OnBuildGuildBattlePrologResult() 
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	if	(g_iSelectedServer	>=	g_iWorldCount || g_iSelectedServer < 0)
	{
		MessageBox("please select world","");
		return;
	}

	if	(sList[g_iSelectedServer].isConnect	==	FALSE)
	{
		MessageBox(g_strToolMessage[10]);
		return;
	}

	if	(cMSG::YESNO("Build result of guild battle prolog","%s\n\nContinue?",sList[g_iSelectedServer].strName)	==	IDNO)
		return;
	
	SERVERStoWORLD_BUILD_RESULT_OF_GUILD_BATTLE_PROLOG	packet;
	packet.base.set(sizeof(SERVERStoWORLD_BUILD_RESULT_OF_GUILD_BATTLE_PROLOG),dSERVERStoWORLD_BUILD_RESULT_OF_GUILD_BATTLE_PROLOG);

	SendPacketToWorld(g_iSelectedServer,(char *)&packet);

	cMSG::Put("","Complete request Work\n\n%s",sList[g_iSelectedServer].strName);
}

class	c100721
{
public:
	char		m_strID[dID_LENGTH];
	char		m_strName[dNAME_LENGTH];
	CTimeInfo	m_receiveTime;
	CTimeInfo	m_useTime;
	int			m_iServer;
};

void CServerTool::On100721() 
{
	BOOL		bSaveToDB	=	FALSE;
	cDECODER	decoder;

	if	(bSaveToDB)
	{
		if	(cMSG::YESNO("신규 유저 아이템 체크!!","이거 DB 저장하는건데 진짜 할껴?")	==	IDNO)
			return;
	}

	if	(!decoder.Upload("20100721.txt",NULL))	
		return;

	c100721		aList[2000];
	int			iListCount	=	0;

	while(1)
	{
		c100721		*lpData		=	&aList[iListCount];
		char		*lpstrReadText;
		BOOL		bIsSuccess	=	FALSE;

		lpstrReadText	=	decoder.GetStream(9);

		if	(!lpstrReadText)
			break;

		strcpy(lpData->m_strID,lpstrReadText);

		lpData->m_receiveTime.m_wYear	=	decoder.GetNumber(&bIsSuccess);
		lpData->m_receiveTime.m_wMonth	=	decoder.GetNumber(&bIsSuccess);
		lpData->m_receiveTime.m_wDay	=	decoder.GetNumber(&bIsSuccess);
		lpData->m_receiveTime.m_wHour	=	decoder.GetNumber(&bIsSuccess);
		lpData->m_receiveTime.m_wMinute	=	decoder.GetNumber(&bIsSuccess);

		lpData->m_useTime.m_wYear		=	decoder.GetNumber(&bIsSuccess);
		lpData->m_useTime.m_wMonth		=	decoder.GetNumber(&bIsSuccess);
		lpData->m_useTime.m_wDay		=	decoder.GetNumber(&bIsSuccess);
		lpData->m_useTime.m_wHour		=	decoder.GetNumber(&bIsSuccess);
		lpData->m_useTime.m_wMinute		=	decoder.GetNumber(&bIsSuccess);

		decoder.GetStream(9);	//	스킵

		lpstrReadText	=	decoder.GetStream(9);	//	서버명
		lpData->m_iServer	=	-1;

		for	(int iServer=0;iServer<g_iWorldCount;iServer++)
		{
			if	(stricmp(sList[iServer].strName,lpstrReadText)==0)
			{
				lpData->m_iServer	=	iServer;

				if	(iServer	==	5)
					iServer	=	iServer;
				break;
			}
		}

		if	(lpData->m_iServer	==	-1)
			lpData->m_iServer	=	lpData->m_iServer;

		lpstrReadText	=	decoder.GetStream(9);
		strcpy(lpData->m_strName,lpstrReadText);	//	캐릭명

		lpstrReadText	=	decoder.GetStream(10);
		iListCount++;
	}

	if	(!ConnectAllServerDB())
	{
		cMSG::Error("DB 연결 실패!!","DB 연결에 실패 했습니다.");

		return;
	}

	int	iIncorrectDrake	=	3620;
	int	iCorrectDrake	=	4506;

	int	iCoupleRing1	=	1884;
	int	iCoupleRing2	=	1885;
	
	if	(AllocConsole())
	{
		// Create the actual console
		g_hConsole	= CreateFile(_T("CONOUT$"), GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

		if	(g_hConsole != INVALID_HANDLE_VALUE)
		{
			SetConsoleMode(g_hConsole	, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

			SetConsoleTitle("On100721");
		}
	}

	cFILE	logFile;
	
	logFile.Open("100721_log.txt","wt",TRUE);

	for (int i=0;i<iListCount;i++)
	{
		c100721		*lpData		=	&aList[i];

		if	(m_abDBConnetionInfo[lpData->m_iServer]	==	FALSE)
		{
			Printf("[%d/%d] [%s] skip by invalid server %s\n",i,iListCount,lpData->m_strName,sList[lpData->m_iServer].strName);
			continue;
		}

		if	(!m_aDb[lpData->m_iServer].loadData(lpData->m_strID,NULL,&m_commonAm) || m_commonAm.m_iLoadDataCount == 0)
		{
			Printf("[%d/%d] not found [%s]\n",i,iListCount,lpData->m_strName);

			continue;
		}

		int	iCheckDay	=	7*31+7;
		int	iCurDay		=	lpData->m_useTime.m_wMonth*31+lpData->m_useTime.m_wDay;
		int	iDrakeCount=0,iCoupleRing1Count=0,iCoupleRing2Count=0;

		if	(iCurDay	<=	iCheckDay)
		{
			iDrakeCount	=	m_commonAm.getItemInBankCount(iIncorrectDrake);

			if	(iDrakeCount)
				iDrakeCount	=	iDrakeCount;

			for (int iA=0;iA<m_commonAm.m_iLoadDataCount;iA++)
			{
				CItemDefine*	lpDrake	=	m_commonAm.m_aData[iA].getExistItem(iIncorrectDrake,0);

				if	(!lpDrake)
					continue;

				if	(lpDrake->m_year)
					continue;

				lpDrake->m_wBaseItem	=	iCorrectDrake;

				lpDrake->m_year	=	10;
				lpDrake->m_month=	8;
				lpDrake->m_day	=	28;
				lpDrake->m_hour	=	0;
				lpDrake->m_minute=	0;

				if	(bSaveToDB)
				{
					m_commonAm.m_aData[iA].m_dwSaveCount++;
					m_aDb[lpData->m_iServer].saveData((cP_PLAYER_SAVE_DATA *)&m_commonAm.m_aData[iA]);
				}

				iDrakeCount++;
			}
		}

		if	(stricmp(lpData->m_strName,"초승달사구")==0)
			lpData	=	lpData;
		

		{
			iCoupleRing1Count	=	m_commonAm.getItemInBankCount(iCoupleRing1);
			iCoupleRing2Count	=	m_commonAm.getItemInBankCount(iCoupleRing2);

			if	(iCoupleRing1Count+iCoupleRing2Count)
				iCoupleRing2Count	=	iCoupleRing2Count;

			for (int iA=0;iA<m_commonAm.m_iLoadDataCount;iA++)
			{
				int iC,iCheckItem=iCoupleRing1;
				
				for (int iType=0;iType<2;iType++)
				{
					if	(iType	==	1)
						iCheckItem	=	iCoupleRing2;

					for (iC=0;;iC++)
					{
						CItemDefine*	lpRing	=	m_commonAm.m_aData[iA].getExistItem(iCheckItem,iC);

						if	(!lpRing)
							break;

						cCoupleRingDefine	*lpCRing	=	(cCoupleRingDefine	*)lpRing;

						if	(lpCRing->m_year)
							continue;

 						lpCRing->m_year	=	10;
 						lpCRing->m_month=	8;
 						lpCRing->m_day	=	28;

						if	(bSaveToDB)
						{
							m_commonAm.m_aData[iA].m_dwSaveCount++;
							m_aDb[lpData->m_iServer].saveData((cP_PLAYER_SAVE_DATA *)&m_commonAm.m_aData[iA]);
						}

						if	(iType	==	0)
							iCoupleRing1Count++;
						if	(iType	==	1)
							iCoupleRing2Count++;
					}
				}
			}
		}

		logFile.writeStreamCR("%d/%d/%d %d:%d	%s	%s	%s	%d	%d	%d",
			lpData->m_useTime.m_wYear,lpData->m_useTime.m_wMonth,lpData->m_useTime.m_wDay,lpData->m_useTime.m_wHour,lpData->m_useTime.m_wMinute,
			sList[lpData->m_iServer].strName,
			lpData->m_strID,lpData->m_strName,iDrakeCount,iCoupleRing1Count,iCoupleRing2Count);

		if	(iDrakeCount+iCoupleRing1Count+iCoupleRing2Count)
		{
		}
		
		Printf("[%d/%d] [%s] %d/%d/%d\n",i,iListCount,lpData->m_strName,iDrakeCount,iCoupleRing1Count,iCoupleRing2Count);

	}

	cMSG::Put("complete");

	FreeConsole();
}


class	c101022RebirthAvatar
{
public:
	char		m_strID[dID_LENGTH];
	char		m_strName[dNAME_LENGTH];
	int			m_iLevel,m_iExp;
	int			m_iCurLevel,m_iCurExp;
	int			m_iResultLevel,m_iResultExp;
};


void 
operateOldRebirthAvatarLine(char *_lpstrLine,int _iServerIndex,cFILE *_lpLogFile,CServerTool *_lpDlg,int _iServerNumber,char *_lpstrServerName,int _iAvatarCount)
{
	cDECODER	decoder;

	decoder.UploadBuffer(_lpstrLine,NULL);

	decoder.m_bIsUseComment	=	FALSE;

	decoder.FindCharOver('[');
	decoder.FindCharOver('[');

	c101022RebirthAvatar	avatar;

	char	*lpstrId	=	decoder.getLongWord('/');

	if	(!lpstrId)
	{
		cMSG::Error("decode line error!!","can not find ID\n\n%s",_lpstrLine);

		return;
	}

	strcpy(avatar.m_strID,lpstrId);

	if	(strstr(_lpstrLine,"@^^*@"))
		_lpstrLine	=	_lpstrLine;
		
	char	*lpstrName	=	decoder.getLongWord(']');

	if	(!lpstrName)
	{
		cMSG::Error("decode line error!!","can not find Name\n\n%s",_lpstrLine);

		return;
	}

	strcpy(avatar.m_strName,lpstrName);

	decoder.FindCharOver('[');	//	[IP 218.157.12.122]
	decoder.FindCharOver('[');	//	[340]
	decoder.FindCharOver('[');	//	[Rebirth count 1]
	decoder.FindCharOver('[');	//	[Rebirth Level 693:85285203->1:0]
	decoder.GetWord();	//	Rebirth
	decoder.GetWord();	//	Level

	BOOL	bIsSuccess;
	avatar.m_iLevel	=	decoder.GetNumber(&bIsSuccess);	//	

	if	(!bIsSuccess)
	{
		cMSG::Error("decode line error!!","can not read Level\n\n%s",_lpstrLine);

		return;
	}

	avatar.m_iExp	=	decoder.GetNumber(&bIsSuccess);	//	

	if	(!bIsSuccess)
	{
		cMSG::Error("decode line error!!","can not read Exp\n\n%s",_lpstrLine);

		return;
	}

	int	iResultLevel	=	decoder.GetNumber(&bIsSuccess);	//	

	if	(!bIsSuccess)
	{
		cMSG::Error("decode line error!!","can not read result Level\n\n%s",_lpstrLine);

		return;
	}

	decoder.m_bIsUseComment	=	TRUE;

	Printf("server %d:%s : #%.4d  [%s/%s]",_iServerNumber,_lpstrServerName,_iAvatarCount,avatar.m_strID,avatar.m_strName);

//	file.writeStream("ID	Name	Old Level	Old Exp	Cur Level	Cur Exp	Result Level	Result Exp\n");

	if	(!_lpDlg->m_aDb[_iServerIndex].loadData(avatar.m_strID,NULL,&_lpDlg->m_commonAm) || _lpDlg->m_commonAm.m_iLoadDataCount == 0)
	{
		Printf("avatar not exist\n");
		_lpLogFile->writeStream("%s	%s	%d	%d	??	??	??	??	avatar not exist\n",avatar.m_strID,avatar.m_strName,avatar.m_iLevel,avatar.m_iExp);

		return;
	}

	CAvatarData*	lpPlayer	=	_lpDlg->m_commonAm.getByName(avatar.m_strName);

	if	(!lpPlayer)
	{
		Printf("avatar not exist\n");
		_lpLogFile->writeStream("%s	%s	%d	%d	??	??	??	??	avatar not exist\n",avatar.m_strID,avatar.m_strName,avatar.m_iLevel,avatar.m_iExp);

		return;
	}

	if	(iResultLevel	!=	1)
	{
		Printf("new rebirth avatar\n");
		_lpLogFile->writeStream("%s	%s	%d	%d	%d	%d	%d	%d	new rebirth avatar\n",avatar.m_strID,avatar.m_strName,lpPlayer->m_iLevel,lpPlayer->m_iExperience,lpPlayer->m_iLevel,lpPlayer->m_iExperience);

		return;
	}

//	스킬 포인트 버그 있어서 복구를 위해 임시로 사용했던 코드
/*	int	iCorrectSP	=	lpPlayer->getCurrentSkillPointByLevel();
	int	iCurrentSP	=	lpPlayer->getCurrentSkillPointSum();

	if	(iCorrectSP	!=	iCurrentSP)
	{
		lpPlayer->resetSkillPoint();

  		_lpDlg->m_aDb[_iServerIndex].saveData((cP_PLAYER_SAVE_DATA *)lpPlayer);

		_lpLogFile->writeStream("reset SP	[%s/%s]\n",lpPlayer->m_strId,lpPlayer->m_strName);

		iCorrectSP	=	lpPlayer->getCurrentSkillPointByLevel();
		iCurrentSP	=	lpPlayer->getCurrentSkillPointSum();

		if	(iCorrectSP	!=	iCurrentSP)
			iCorrectSP	=	iCorrectSP;
	}
	else
		_lpLogFile->writeStream("correct SP	[%s/%s]\n",lpPlayer->m_strId,lpPlayer->m_strName);	*/


 	{
 		avatar.m_iCurLevel	=	lpPlayer->m_iLevel;
		avatar.m_iCurExp	=	lpPlayer->m_iExperience;

		for (int i=600;i<avatar.m_iLevel;++i)
			lpPlayer->increaseExp(g_aExpTable[i]);

		lpPlayer->increaseExp(avatar.m_iExp);

		int	iCorrectSP	=	lpPlayer->getCurrentSkillPointByLevel();
		int	iCurrentSP	=	lpPlayer->getCurrentSkillPointSum();

		if	(iCorrectSP	==	iCurrentSP)
		{
			avatar.m_iResultLevel	=	lpPlayer->m_iLevel;
			avatar.m_iResultExp		=	lpPlayer->m_iExperience;

			_lpLogFile->writeStream("%s	%s	%d	%d	%d	%d	%d	%d	success!\n",avatar.m_strID,avatar.m_strName,
									avatar.m_iLevel,avatar.m_iExp,
									avatar.m_iCurLevel,avatar.m_iCurExp,
									avatar.m_iResultLevel,avatar.m_iResultExp);

   			_lpDlg->m_aDb[_iServerIndex].saveData((cP_PLAYER_SAVE_DATA *)lpPlayer);
			Printf("success\n");
		}
		else
		{
			_lpLogFile->writeStream("%s	%s	%d	%d	%d	%d	%d	%d	failed by mistmatch skill point\n",avatar.m_strID,avatar.m_strName,
									avatar.m_iLevel,avatar.m_iExp,
									avatar.m_iCurLevel,avatar.m_iCurExp,
									avatar.m_iCurLevel,avatar.m_iCurExp);
			Printf("failed by mistmatch skill point\n");
		}
	}
}


void 
readOldRebirthFile(char *_lpstrFile,CServerTool *_lpDlg,int _iServerNumber)
{
	cDECODER	decoder;

	if	(!decoder.Upload(_ms("script/%s",_lpstrFile),NULL))
	{
		cMSG::Error("file open error","can not find follow file\n\nscript/%s",_lpstrFile);

		return;
	}

	char	*lpstrServerName	=	decoder.GetStream();
	int		iServerIndex		=	GetServerIndexForEvent(lpstrServerName);

	char	strServerName[64];

	if	(!lpstrServerName || iServerIndex	==	-1)
	{
		cMSG::Error("decode error!!","can not find follow server\n\n%s",lpstrServerName);

		return;
	}

	strcpy(strServerName,lpstrServerName);

	cFILE	file;

	cFOLDER::Create(_ms("%s/script/result",g_strLogFolder));

	char	*lpstrFileName	=	_ms("%s/script/result/%s.txt",g_strLogFolder,lpstrServerName);

	file.Open(lpstrFileName,"wt",TRUE);
	file.writeStream("ID	Name	Old Level	Old Exp	Cur Level	Cur Exp	Result Level	Result Exp	Work Result\n");

	int		iCount	=	0;
	
	while(1)
	{
		char*	lpstrOneLine	=	decoder.GetStream(0x0d,0x0a,TRUE);

		if	(lpstrOneLine	==	NULL)
			break;

		if	(strlen(lpstrOneLine) < 5)
			continue;

		operateOldRebirthAvatarLine(lpstrOneLine,iServerIndex,&file,_lpDlg,_iServerNumber,strServerName,iCount++);
	}
}

void CServerTool::OnRewardOldRebirther() 
{
	if	(!CheckOperatorLevel(eAL_SYSTEM_OPERATER))
		return;

	cDECODER	decoder;

 	if	(!ConnectAllServerDB())
 	{
 		cMSG::Error("Failed to Connect DB!!","Failed to Connect DB");
 
 		return;
 	}

	if	(!decoder.Upload("script/old_rebirth_avatar.txt",NULL))
	{
		cMSG::Error("file open error","can not find follow file\n\nscript/old_rebirth_avatar.txt");

		return;
	}

	if	(AllocConsole())
	{
		// Create the actual console
		g_hConsole	= CreateFile(_T("CONOUT$"), GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

		if	(g_hConsole != INVALID_HANDLE_VALUE)
		{
			SetConsoleMode(g_hConsole	, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

			SetConsoleTitle("OnRewardOldRebirther");
		}
	}

	int	iServerIndex	=	1;

	while(1)
	{
		char	*lpstrFile	=	decoder.GetStream();

		if	(lpstrFile	==	NULL)
			break;

		readOldRebirthFile(lpstrFile,this,iServerIndex++);
	}

	cMSG::Put("work complete");

	FreeConsole();
}