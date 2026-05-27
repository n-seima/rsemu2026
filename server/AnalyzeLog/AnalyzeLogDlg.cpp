// AnalyzeLogDlg.cpp : implementation file
//
#include "stdafx.h"
#include "AnalyzeLog.h"
#include "AnalyzeLogDlg.h"
#include "LinkedList.h"
#include "cITEM.h"
#include "cSRVUTIL.h"
#include "CMessage.h"
#include "mmsystem.h"
#include "Tracer.h"
#include "exportLog.h" 
#include "job.h"
#include "../buildVersion.h"
#include <io.h>
#include <process.h>
#include <atlbase.h>		//	for CRegKey
#include "quest.h"
#include "cADO_AnalyzeTool.h"
#include "CFTPclient.h"
#include "log_filtering.h"
#include "duel_filter.h"
#include "ProcessInfoDlg.h"
#include "file_buffer.h"
#include "cDECODER.h"
#include "DownloadLog.h"
#include "../buildVersion.h"	//	이건 공유 안하고 쓴다.
							//	없으면 파일을 생성한다.(공유 하지 말것!!)
							//	안에는
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	와 같이 선언 되어 있다. 뒤에 이름은 프로그래머 이름을 적을것
BOOL	l_bIsExit	=	FALSE;

#ifdef	_FOR_KOREA
char	*l_strJobName[]	=	
{
	"견습기사",
	"전사",
	"마법사",
	"늑대인간",
	"사제",
	"타락천사",
	"도둑",
	"무도가",
	"마법 창병",
	"마법 궁수",
	"조련사",
	"소환사",
	"공주",
	"변신 소녀",
	"강신술사",
	"악마",
	"영술사",
	"투사",
	"광학사",
	"수인",
	""
};
#else
char	*l_strJobName[]	=	
{
	"Knight",
	"Warrior",
	"Wizard",
	"Werewolf",
	"Priest",
	"Fallen Angel",
	"Rogue",
	"Fighter",
	"Magic Lancer",
	"Magic Archer",
	"Tamer",
	"Summoner",
	"Princess",
	"Magical Girl",
	"Necromancer",
	"Devil",
	"Soul Bringer",
	"Champion",
	"OpticalList",
	"BeastMan",
	""
};
#endif

cFileBuffer	l_fb;
CAnalyzeLogDlg	*g_lpMainDlg;

#define	mRB(a,b)	if (!l_fb.read((char*)&a,b))	break

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cADO_AnalyzeTool	cADO_SERVERINFO;
DebugTracer			*cLOG	=	NULL;

UINT
PlayWavFile(char *_lpstrWaveFile,HWND _hWnd)
{
	MCI_OPEN_PARMS	open;
	MCIERROR		rc;
	UINT			uWaveDeviceId;
	MCI_PLAY_PARMS	play;
	char			strWaveFile[512];

	sprintf(strWaveFile,"%s/%s",g_strLogFolder,_lpstrWaveFile);

	open.dwCallback       = (DWORD)_hWnd;
	open.lpstrDeviceType  = "waveaudio";
	open.lpstrElementName = strWaveFile;

	rc = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,(DWORD)&open);

	if (rc == 0)
	{
		uWaveDeviceId = open.wDeviceID;
		play.dwCallback = (DWORD)_hWnd;

		rc = mciSendCommand(uWaveDeviceId, MCI_PLAY, MCI_NOTIFY,(DWORD)&play);
	}

	return	uWaveDeviceId;
}

void
__cdecl CAnalyzeLogDlg::MSGBOX(char * title,char * txt, ...)
{
	char	buffer[2048];
	va_list args;

	va_start( args, txt );
	_vstprintf( buffer, txt, args );
	va_end( args );

	MessageBox(buffer,title);
}

CAnalyzeLogDlg::CAnalyzeLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalyzeLogDlg::IDD, pParent)
{
	g_lpMainDlg	=	this;

	m_aLogNodeForSort	=	new CLogNode[dMAX_READ_LOG_COUNT];

	memset(m_strExportLogFolder,0,sizeof(m_strExportLogFolder));

	m_pProgressDlg	=	NULL;
	//{{AFX_DATA_INIT(CAnalyzeLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(g_abIsAboutItemLog,0,sizeof(g_abIsAboutItemLog));

	g_abIsAboutItemLog[dLOG_SYSTEM				]	=	TRUE;
	g_abIsAboutItemLog[dLOG_USE_ITEM			]	=	TRUE;
	g_abIsAboutItemLog[dLOG_EXPIRED_ITEM		]	=	TRUE;
	g_abIsAboutItemLog[dLOG_DOUBLE_ITEM			]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ENCHANT_ITEM		]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ITEM_DROP			]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ITEM_PICK			]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ITEM_SELL			]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ITEM_DEL			]	=	TRUE;
	g_abIsAboutItemLog[dLOG_CREATE_ITEM			]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ITEM_CREATE_PREMIUM_ITEM	]	=	TRUE;
	g_abIsAboutItemLog[dLOG_PITCHMAN_SHOP_TRADE	]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ITEM_TRADE			]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ITEM_BANK_IN		]	=	TRUE;
	g_abIsAboutItemLog[dLOG_ITEM_BANK_OUT		]	=	TRUE;

	allData = NULL;
	searchData = NULL;

	_Tracer	=	NULL;
}

CAnalyzeLogDlg::~CAnalyzeLogDlg()
{
	pKILL(m_aLogNodeForSort);
	KILL(m_pProgressDlg);
	KILL(cLOG);
	KILL(_Tracer);

	m_pProgressDlg	=	NULL;
}

void CAnalyzeLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnalyzeLogDlg)
	DDX_Control(pDX, IDC_COMMENT, m_stComment);
	DDX_Control(pDX, IDC_isDownLoadServer, m_ctlIsServer);
	DDX_Control(pDX, IDC_VALUE, m_editValue);
	DDX_Control(pDX, ID_LOG_DOWN2, m_ctlDnButton);
	DDX_Control(pDX, IDC_AND, m_checkAnd);
	DDX_Control(pDX, IDC_ITEM, m_comboItemList);
	DDX_Control(pDX, IDC_IP, m_ctlIP);
	DDX_Control(pDX, IDC_MIN2, m_ctlMin2);
	DDX_Control(pDX, IDC_MIN1, m_ctlMin1);
	DDX_Control(pDX, ID_LOG_BACK, m_ctlBack);
	DDX_Control(pDX, IDC_COMPARE, m_ctlCompare);
	DDX_Control(pDX, IDC_CB_JOB, m_ctlCB_Job);
	DDX_Control(pDX, IDC_NAME, m_ctlName);
	DDX_Control(pDX, IDC_ITEMSERIAL, m_ctlSerial);
	DDX_Control(pDX, IDC_ID, m_ctlId);
	DDX_Control(pDX, IDC_SERVERLIST, m_ctlServerList);
	DDX_Control(pDX, IDC_LOG_CALENDAR, m_ctlTime);
	DDX_Control(pDX, IDC_HOUR2, m_ctlHour2);
	DDX_Control(pDX, IDC_HOUR1, m_ctlHour1);
	DDX_Control(pDX, IDC_DD2, m_ctlDD2);
	DDX_Control(pDX, IDC_DD1, m_ctlDD1);
	DDX_Control(pDX, IDC_MM2, m_ctlMM2);
	DDX_Control(pDX, IDC_MM1, m_ctlMM1);
	DDX_Control(pDX, IDC_YY2, m_ctlYY2);
	DDX_Control(pDX, IDC_YY1, m_ctlYY1);
	DDX_Control(pDX, IDC_LISTDATA, m_ctlLogData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAnalyzeLogDlg, CDialog)
	//{{AFX_MSG_MAP(CAnalyzeLogDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_MM1, OnSelchangeMm1)
	ON_CBN_SELCHANGE(IDC_MM2, OnSelchangeMm2)
	ON_BN_CLICKED(ID_LOG_SEARCH, OnLogSearch)
	ON_BN_CLICKED(ID_LOG_DOWN, OnLogDown)
	ON_BN_CLICKED(ID_LOG_LOAD, OnLogLoad)
	ON_BN_CLICKED(ID_LOG_SAVE, OnLogSave)
	ON_BN_CLICKED(ID_LOG_ITEM_SAVE, OnLogItemSave)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_LOG_ADD_FILE, OnLogAddFile)
	ON_BN_CLICKED(IDC_LOGOPEN, OnLogopen)
	ON_BN_CLICKED(ID_LOG_DOWN2, OnLogDown2)
	ON_BN_CLICKED(IDC_FIXFILE, OnFixfile)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_YY1, OnSelchangeYy1)
	ON_CBN_SELCHANGE(IDC_YY2, OnSelchangeYy2)
	ON_BN_CLICKED(ID_SP_LOG_SEARCH, OnSpLogSearch)
	ON_BN_CLICKED(IDC_DATAFOLDER, OnDatafolder)
	ON_WM_TIMER()
	ON_NOTIFY(MCN_GETDAYSTATE, IDC_LOG_CALENDAR, OnGetdaystateLogCalendar)
	ON_NOTIFY(MCN_SELCHANGE, IDC_LOG_CALENDAR, OnSelchangeLogCalendar)
	ON_NOTIFY(MCN_SELECT, IDC_LOG_CALENDAR, OnSelectLogCalendar)
	ON_BN_CLICKED(IDC_isDownLoadServer, OnisDownLoadServer)
	ON_BN_CLICKED(IDC_NUXtoLOG, OnNUXtoLOG)
	ON_BN_CLICKED(IDC_FULL_DAY, OnFullDay)

	ON_BN_CLICKED(IDC_LASTEST_1DAY, On1Day)
	ON_BN_CLICKED(IDC_LASTEST_1WEEK, On1Week)
	ON_BN_CLICKED(IDC_LASTEST_2WEEK, On2Week)
	ON_BN_CLICKED(IDC_LASTEST_1MONTH, On1Month)
	ON_BN_CLICKED(IDC_LASTEST_6MONTH, On6Month)

	ON_BN_CLICKED(IDC_SAVE_FILTER, OnSaveFilter)
	ON_BN_CLICKED(IDC_LOAD_FILTER, OnLoadFilter)
	ON_BN_CLICKED(IDC_APPLY_EDIT_FILTER, OnApplyEditFilter)
	ON_CBN_SELCHANGE(IDC_COMPARE, OnSelchangeCompare)
	ON_BN_CLICKED(IDC_GET_EDIT_FILTER, OnGetEditFilter)
	ON_BN_CLICKED(IDC_IS_COPY_LOG_DATA, OnIsNotCopyLogData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalyzeLogDlg message handlers

BOOL CAnalyzeLogDlg::OnInitDialog()
{
	m_filter.m_iFilter	=	-1;
	strcpy(m_strExportLogFolder,g_strLogFolder);


 	if	(IsFile("do not here!!"))
	{
		cMSG::Error("Error!!","do not here!!");

		l_bIsExit	=	TRUE;

		OnCancel();			
		
		return	FALSE;
	}

	CDialog::OnInitDialog();

	{	//	reg에 등록된 로그 디렉토리를 읽어온다.
		CRegKey		reg;

		LPCTSTR		lpszDevRegistry		=	"software\\L&K Logic Korea\\Red Stone for Dev";

		g_hRegKey	=	HKEY_CURRENT_USER;

		int	iResult	=	reg.Open(HKEY_CURRENT_USER,lpszDevRegistry);

		if	(iResult==	ERROR_SUCCESS)
			g_lpszRegistry	=	lpszDevRegistry;
			
		if	(iResult!=	ERROR_SUCCESS)
			iResult	=	reg.Open(HKEY_CURRENT_USER,g_lpszRegistry);

		if	(iResult	!=	ERROR_SUCCESS)
		{
			iResult		=	reg.Open(HKEY_LOCAL_MACHINE,g_lpszRegistry);
			g_hRegKey	=	HKEY_LOCAL_MACHINE;
		}

#ifdef	_FOR_JAPAN
		if	(iResult	!=	ERROR_SUCCESS)
		{
			g_lpszRegistry	=	g_lpszRegistryOld;
			iResult			=	reg.Open(HKEY_CURRENT_USER,g_lpszRegistry);
			g_hRegKey		=	HKEY_CURRENT_USER;

			if	(iResult	!=	ERROR_SUCCESS)
			{
				iResult	=	reg.Open(HKEY_LOCAL_MACHINE,g_lpszRegistry);
				g_hRegKey	=	HKEY_LOCAL_MACHINE;
			}
		}
#endif

		if	(iResult	!=	ERROR_SUCCESS)
		{
			g_hRegKey	=	HKEY_CURRENT_USER;
			reg.Create(HKEY_CURRENT_USER,g_lpszRegistry);
		}

		DWORD size = 512;

		if (reg.QueryValue(m_strServerLogDir,"ToolLogPath",&size) != ERROR_SUCCESS)
		{	//	기존에 등록된 값이 있다면 그대로 유지한다.
			reg.SetValue("c:\\","ToolLogPath");
			strcpy(m_strServerLogDir,"c:\\");
		}
		DWORD isLogDownServer = 0;
		if (reg.QueryValue(isLogDownServer,"IsLogDownServer") != ERROR_SUCCESS)
		{
			reg.SetValue((DWORD)0,"IsLogDownServer");
			isLogDownServer = 0;
		}
		m_ctlIsServer.SetCheck(isLogDownServer);
		reg.Close();

		SetDlgItemText(IDC_DATAFOLERTXT,m_strServerLogDir);
	}

	if	(!m_ctlIsServer.GetCheck())
	{
		CRegKey		reg;

		strcpy(g_strGameFolder,"");

		int	iResult	=	reg.Open( g_hRegKey, g_lpszRegistry);

		if (iResult	== ERROR_SUCCESS)
		{
			DWORD	count	=	sizeof(g_strGameFolder);

			if (reg.QueryValue((LPTSTR )g_strGameFolder,"Path",&count) == ERROR_SUCCESS)
			{
				if (count	<=	0)
					return cMSG::Error("error","can not find red stone registry data");;
			}
			else
				return cMSG::Error("error","can not find red stone registry data");;
		}
		else
			return cMSG::Error("error","can not find red stone registry data");;

		CQuest::Load();
	}

	char txt[512];

	if	(m_ctlIsServer.GetCheck())
	{
		WSADATA wsadata;
		
		if	(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
		{	
			_log("Fail WSAStartup !! \n");
			return FALSE;
		}

		m_ctlDnButton.ShowWindow(SW_SHOW);
		sprintf(txt," SERVER : AnalyzeLog Tool %s %s built by %s",__DATE__,__TIME__,dBUILDER_NAME);
	}
	else
		sprintf(txt," AnalyzeLog Tool %s %s built by %s",__DATE__,__TIME__,dBUILDER_NAME);

	SetWindowText(txt);

#ifdef _FOR_CHINA
	//	dialog의 가로 크기를 늘려라.
	RECT rect,rectCal,rectCtlSet;
	GetClientRect(&rect);
	MoveWindow( rect.left, rect.top, rect.right+ 100, rect.bottom); 
	HWND hCalWnd;
	GetDlgItem(IDC_LOG_CALENDAR,&hCalWnd);
	::GetClientRect(hCalWnd,&rectCal);
	::GetWindowRect(hCalWnd,&rectCtlSet);
	::MoveWindow(hCalWnd,rectCal.left+rectCtlSet.left, rectCal.top+rectCtlSet.top-40, rectCal.right+ 100, rectCtlSet.bottom-40, TRUE); 
//	::MoveWindow(hCalWnd,rectCal.left+rectCtlSet.left, rectCal.top+rectCtlSet.top, 100, 100, TRUE); 
	//	IDC_LOG_CALENDAR의 가로 크기를 늘려라.
#endif

	m_dwCurListCount = m_dwSearchListCount = 0;
	isSearchList = FALSE;
	allData = new CLinkedList;
	searchData = new CLinkedList;

	scp.Read("ReadMe.txt");
	strcpy(ipLogAddr,scp.getData("LOGSERVERIP"));
	int sCount = atoi(scp.getData("SERVERCOUNT"));
	startdate = atoi(scp.getData("STARTDATE"));
	strcpy(strFtpId,scp.getData("FTP_ID"));

#ifdef	_FOR_KOREA
	strcpy(strFtpPw,"cjfvksrndl");
#else	
	strcpy(strFtpPw,scp.getData("FTP_PW"));
#endif

	FtpPort = atoi(scp.getData("FTP_PORT"));

	strcpy(g_strAccountDBIP,scp.getData("ACCOUNTDBINFO"));
	strcpy(g_strAccountTableName,scp.getData("ACCOUNTDBINFO",2));
	strcpy(g_strAccountDBID,scp.getData("ACCOUNTDBINFO",3));
	strcpy(g_strAccountDBPW,scp.getData("ACCOUNTDBINFO",4));

	GetCurrentDirectory(512,strMainDir);

	loadCofig();

	if	(m_ctlIsServer.GetCheck())
		sprintf(m_strServerLogDir,"%s\\LOGFILES",strMainDir);
	else
	{
		sprintf(m_strLocalLogDir,"%s\\LOGFILES",strMainDir);

		char	tmp[1024];
		strcpy(tmp,m_strServerLogDir);
		sprintf(m_strServerLogDir,"%s",tmp);
	}

	sprintf(strSaveDir,"%s\\SAVEFILES",strMainDir);
	sprintf(strSaveLogDir,"%s\\SAVELOG",strMainDir);
	CreateDirectory(m_strServerLogDir,NULL);
	CreateDirectory(strSaveDir,NULL);
	CreateDirectory(strSaveLogDir,NULL);
	CreateDirectory("filter",NULL);

	SetCurrentDirectory(strMainDir);

	_Tracer = new DebugTracer(DTType_ALL,"AnalyzeLog");

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SYSTEMTIME	time;
	CWnd * wnd = this->GetDlgItem(IDC_LOG_CALENDAR);
	MonthCal_GetCurSel(wnd->m_hWnd,&time);	//	현재 선택한 일자를 얻어온다.


	int tc = 0;	char	tchar[10];
	{
		for (int i=2003;i<2100;i++)
		{
			m_ctlYY1.AddString(_ms("%d",i));
			m_ctlYY2.AddString(_ms("%d",i));
		}

		int curYear = (cSRVUTIL::GetYear()+1900) - 2003;

		m_ctlYY1.SetCurSel(curYear);
		m_ctlYY2.SetCurSel(curYear);
	}

	for(tc = 1;tc <= 12 ; tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_ctlMM1.AddString(tchar);m_ctlMM2.AddString(tchar);
	}
	m_ctlMM1.SetCurSel(time.wMonth-1);m_ctlMM2.SetCurSel(time.wMonth-1);

	int aiMonthend[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if( (time.wYear%4 == 0) && (time.wYear%100 !=0) || time.wYear%400 == 0)	aiMonthend[2] = 29;
	else																	aiMonthend[2] = 28;

	for(tc = 1;tc<=aiMonthend[time.wMonth];tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_ctlDD1.AddString(tchar);
		m_ctlDD2.AddString(tchar);
	}
	m_ctlDD1.SetCurSel(time.wDay-1);m_ctlDD2.SetCurSel(time.wDay-1);

	for(tc = 0;tc<=23;tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_ctlHour1.AddString(tchar);
		m_ctlHour2.AddString(tchar);
	}
	m_ctlHour1.SetCurSel(time.wHour);
	m_ctlHour2.SetCurSel(time.wHour);
	for(tc = 0;tc<=59;tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_ctlMin1.AddString(tchar);
		m_ctlMin2.AddString(tchar);
	}
	m_ctlMin1.SetCurSel(time.wMinute);m_ctlMin2.SetCurSel(time.wMinute);

	for(tc=0;tc<sCount;tc++)
		m_ctlServerList.InsertString(tc,scp.getData("SERVERNAME",tc+1));
	m_ctlServerList.SetCurSel(0);

	{
		m_ctlCB_Job.AddString("None");

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
		{
			if	(l_strJobName[i][0]	==	NULL)
				break;

			m_ctlCB_Job.AddString(l_strJobName[i]);
		}

		m_ctlCB_Job.SetCurSel(0);
	}
//		cITEM::s_iCount
	m_ctlCompare.InsertString(eALL,dMSG_SEARCH_ALL_DATA);

	m_ctlCompare.InsertString(eITEM_ALL,dMSG_ALL_ITEM);
	m_ctlCompare.InsertString(eABOUT_GUILD_WORK,dMSG_GUILD_BATTLE_LOG);

	m_ctlCompare.InsertString(eITEM_DROP,dMSG_DROP_ITEM);
	m_ctlCompare.InsertString(eITEM_PICK,dMSG_PICK_ITEM);
	m_ctlCompare.InsertString(eITEM_BUY_SELL,dMSG_BUY_SELL_ITEM);
	m_ctlCompare.InsertString(eITEM_DEL,dMSG_REMOVE_ITEM);
	m_ctlCompare.InsertString(eITEM_TRADE,dMSG_TRADE_ITEM);
	m_ctlCompare.InsertString(eITEM_BANK,dMSG_BANK);

	m_ctlCompare.InsertString(eITEM_PREMIUM_ITEM,dMSG_PREMIUM_ITEM);
	m_ctlCompare.InsertString(eITEM_CREATE_ITEM,dMSG_CREATE_ITEM);
	m_ctlCompare.InsertString(eITEM_USE_ITEM,dMSG_USE_ITEM);

	m_ctlCompare.InsertString(eITEM_EXPI,dMSG_DESTROY_ITEM);
	m_ctlCompare.InsertString(eITEM_COPY,dMSG_COPY_ITEM);
	m_ctlCompare.InsertString(eITEM_ENCH,dMSG_ENCHANT);
	m_ctlCompare.InsertString(eITEM_PITCHMAN_SHOP,dMSG_PITCHMAN_SHOP);
	m_ctlCompare.InsertString(eITEM_FIND_INCORECT_ITEM,dMSG_INCORRECT_PREFIX);
	m_ctlCompare.InsertString(eITEM_RENEW_TERM,dMSG_RWNEW_ITEM_TERM);

	m_ctlCompare.InsertString(eAVA_ALL,dMSG_ALL_CHARACTER);
	m_ctlCompare.InsertString(eAVA_CREATE,dMSG_CREATE_CHARACTER);
	m_ctlCompare.InsertString(eAVA_DELETE,dMSG_REMOVE_CHARACTER);
	m_ctlCompare.InsertString(eAVA_LOGIN,dMSG_LOG_IN);
	m_ctlCompare.InsertString(eAVA_LOGOUT,dMSG_LOG_OUT);
	m_ctlCompare.InsertString(eAVA_SAVE,dMSG_SAVE_LOG);

	m_ctlCompare.InsertString(eABOUT_EVENT,dMSG_EVENT_LOG);
	m_ctlCompare.InsertString(eABOUT_QUEST,dMSG_QUEST_LOG);
	m_ctlCompare.InsertString(eABOUT_BACK_SERVER,dMSG_BACK_SERVER);
	m_ctlCompare.InsertString(eABOUT_PET,dMSG_ABOUT_PET);
	m_ctlCompare.InsertString(eABOUT_MINIPET,dMSG_ABOUT_MINIPET);
	m_ctlCompare.InsertString(eABOUT_BOTTOMLESS_BOX,dMSG_BOTTOMLESS_BOX);
	m_ctlCompare.InsertString(eABOUT_HACKING,"Hacking");
	m_ctlCompare.InsertString(eABOUT_CARROT_SHOP,dMSG_CARROT_SHOP);
	m_ctlCompare.InsertString(eABOUT_DUEL,dMSG_DUEL);
	m_ctlCompare.InsertString(eABOUT_REBIRTH,dMSG_REBIRTH);
	m_ctlCompare.InsertString(eABOUT_ERROR,"ERROR LOG");


	m_ctlCompare.SetCurSel(0);

	GetCurrentDirectory(512,strMainDir);

	if	(!m_ctlIsServer.GetCheck())
	{
		cITEM readItem;

		if	(!readItem.ReadItemData())
		{
			MessageBox("can not find 'item.dat' file");
			return	FALSE;
		}

		if	(!cJOB::Init())
		{
			MessageBox("can not find 'job.dat' file");
			return	FALSE;
		}

		for(int i=0;i<cITEM::s_iCount;i++)
		{
			if (g_aBasicItem[i].m_iSerial	==	0xffffffff)
				continue;

			int	iIndex	=	m_comboItemList.AddString(g_aBasicItem[i].m_strName);

			m_comboItemList.SetItemData(iIndex,i);
		}

		int	iIndex	=	m_comboItemList.AddString(" valid item");

		m_comboItemList.SetItemData(iIndex,0xffff);
		m_comboItemList.SelectString(0," valid item");
	}

	CheckDlgButton(IDC_AND,TRUE);
	CheckDlgButton(IDC_IGNORE_SAVE_LOG,FALSE);
	CheckDlgButton(IDC_ADD_SAVE_LOG,FALSE);

/*	for(i=0;i<cITEM::s_iCount+1;i++)
	{
		int	iItem	=	m_comboItemList.GetItemData(i);

		if (iItem	>=	cITEM::s_iCount)
			continue;

		if (g_aBasicItem[iItem].m_iSerial	==	0xffffffff)
			continue;

		MSGOUT("[%.3d]%s",iItem,g_aBasicItem[iItem].m_strName);
	}
*/

	SetCurrentDirectory(strMainDir);
	cLOG = new DebugTracer(DTType_File,"LOG");

	if	(m_ctlIsServer.GetCheck())
		SetTimer(1,600000,NULL);
	
	m_iSelectedDate = 0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void
CAnalyzeLogDlg::loadCofig()
{
	char	strLogFile[512];

	sprintf(strLogFile,"%s\\analyzeLogTool.cfg",g_strLogFolder);

	cFILE	file;

	if	(!file.Open(strLogFile,"rb"))
		return;

	BOOL	bIsCopyLogData;

	file.Read(m_strExportLogFolder,sizeof(m_strExportLogFolder));
	file.Read(&bIsCopyLogData,sizeof(bIsCopyLogData));

	file.Close();

	CheckDlgButton(IDC_IS_COPY_LOG_DATA,bIsCopyLogData);
}

void
CAnalyzeLogDlg::saveCofig()
{
	char	strLogFile[512];

	sprintf(strLogFile,"%s\\analyzeLogTool.cfg",g_strLogFolder);

	cFILE	file;

	if	(!file.Open(strLogFile,"wb"))
		return;

	BOOL	bIsCopyLogData	=	IsDlgButtonChecked(IDC_IS_COPY_LOG_DATA);

	file.Write(m_strExportLogFolder,sizeof(m_strExportLogFolder));
	file.Write(&bIsCopyLogData,sizeof(bIsCopyLogData));

	file.Close();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAnalyzeLogDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAnalyzeLogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAnalyzeLogDlg::OnSelchangeMm1() 
{
	// TODO: Add your control notification handler code here
	int	aiMonthend[13]=	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int	iYear		=	m_ctlYY1.GetCurSel()+2003;

	if( (iYear%4 == 0) && (iYear%100 !=0) || iYear%400 == 0)
		aiMonthend[2] = 29;
	else
		aiMonthend[2] = 28;

	for(int dc=0;dc<31;dc++)
		m_ctlDD1.DeleteString(0);

	int		selectMon = m_ctlMM1.GetCurSel()+1;
	char	tchar[10];

	for(int tc = 1;tc<=aiMonthend[selectMon];tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_ctlDD1.AddString(tchar);
	}

	m_ctlDD1.SetCurSel(0);
}

void CAnalyzeLogDlg::OnSelchangeMm2() 
{
	int	aiMonthend[13]=	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int	iYear		=	m_ctlYY2.GetCurSel()+2003;

	if( (iYear%4 == 0) && (iYear%100 !=0) || iYear%400 == 0)
		aiMonthend[2] = 29;
	else
		aiMonthend[2] = 28;

	for(int dc=0;dc<31;dc++)
		m_ctlDD2.DeleteString(0);

	int		selectMon = m_ctlMM2.GetCurSel()+1;
	char	tchar[10];

	for(int tc = 1;tc<=aiMonthend[selectMon];tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_ctlDD2.AddString(tchar);
	}

	m_ctlDD2.SetCurSel(0);
}

BOOL
CAnalyzeLogDlg::searchSettingDuelLog()
{
	OnLogSearch();

	g_duelFilter.resetDuelSerialList();

	return	TRUE;
}

void
SearchLogThread(void * _pParam)
{
	CAnalyzeLogDlg	*lpMain	= (CAnalyzeLogDlg *)_pParam;

	lpMain->searchLog();

	if	(lpMain->m_pProgressDlg)
		lpMain->m_pProgressDlg->endDlg();

	lpMain->m_ctlLogData.SetRedraw(TRUE);
}

void
CAnalyzeLogDlg::OnLogSearch()
{
	m_bIsCancelSearch	=	FALSE;

	HANDLE	hHandle		=	(HANDLE)_beginthread(SearchLogThread, 0,(LPVOID)this);

	if	(!m_pProgressDlg)
		m_pProgressDlg	=	new CProcessInfoDlg();

	m_pProgressDlg->DoModal();

	m_bIsCancelSearch	=	TRUE;

	m_ctlLogData.SetRedraw(TRUE);
}

int
CompareLogNode( const void *a, const void *b)
{
	return (((CLogNode *)a)->m_dwTimeValue-((CLogNode *)b)->m_dwTimeValue);
}

void 
CAnalyzeLogDlg::searchLog(BOOL _bIsExportFilter)
{
	// play back sample2.wav
	//......................
	DWORD		dwSelectItemIndex	=	0xffffffff;	//	선택한 아이템
	DWORD		dwFilterItemSerial	=	0xffffffff;	//	유니크 아이템 시리얼 필터
	char		strLogFileFullPath[1024];
	int			yy = 0, mm = 0, dd = 0;
	int			iReadCount = 0;
	cFILE		fileLog;
	int			iJobFilter;

	char		strValue[64];
	int			iValue				=	0xffffffff;
	CTimeInfo	beginTime,endTime;
	BOOL		bIsUseLocalData		=	FALSE;

	//	연도와 월, 일자 까지는 설정되어야 한다.
	int		yy1=0,mm1=0,dd1=0,hour1=0,min1=0;
	int		yy2=0,mm2=0,dd2=0,hour2=0,min2=0;
	int		iServerIndex=0,iFilterIndex=0;					//	서버 인덱스	, 조건
	char	strFilterId[22],strFilterName[18],strFilterSerial[20],strFilterIp[16];
	char	tmpValue[64];

	CWnd	*wnd;
	int		aiMonthend[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	BOOL	bIsCheckAndCondition,bIsIgnoreSaveLog,bIsAddSaveLog;

	int		iSkipLogCount	=	0;
	int		iFileIndex	=	0;

	//	아이템 필터 되어 있으면 인덱스를 구한다.
	{
		char	strSelectItem[128];

		m_comboItemList.GetWindowText(strSelectItem,128);

		if	(IsNumberText(strSelectItem))
		{
			dwSelectItemIndex	=	atoi(strSelectItem);

			if (dwSelectItemIndex < (DWORD)cITEM::s_iCount && (DWORD)g_aBasicItem[dwSelectItemIndex].m_iSerial != 0xffffffff)
			{
				strcpy(strSelectItem,g_aBasicItem[dwSelectItemIndex].m_strName);
			}
			else
			{
				cMSG::Error(" ",dMSG_CAN_NOT_FIND_SELECT_INDEX_ITEM_FORM,dwSelectItemIndex);

				goto label_error;
			}
		}

		{
			m_comboItemList.SelectString(0,strSelectItem);

			int	iIndex			=	m_comboItemList.GetCurSel();

			dwSelectItemIndex	=	m_comboItemList.GetItemData(iIndex);
		}

		if	(dwSelectItemIndex	>=	(DWORD)cITEM::s_iCount || (DWORD)g_aBasicItem[dwSelectItemIndex].m_iSerial == 0xffffffff)
			dwSelectItemIndex	=	0xffffffff;
	}

	//	조건 앤드냐?
	bIsCheckAndCondition	=	IsDlgButtonChecked(IDC_AND);
	bIsIgnoreSaveLog		=	IsDlgButtonChecked(IDC_IGNORE_SAVE_LOG);
	bIsAddSaveLog			=	IsDlgButtonChecked(IDC_ADD_SAVE_LOG);
	g_bIsOnlySetItem		=	IsDlgButtonChecked(IDC_IS_SET_ITEM);
	g_bIsOnlyUnique			=	IsDlgButtonChecked(IDC_IS_UNIQUE);
	g_bIsOnlyDX				=	IsDlgButtonChecked(IDC_IS_DX);
	g_bIsOnlyEternal		=	IsDlgButtonChecked(IDC_IS_ETERNAL);
	g_iEternalGrade			=	GetDlgItemInt(IDC_ETERNAL_GRADE);

	// TODO: Add your control notification handler code here
	SYSTEMTIME	time;

	wnd			=	this->GetDlgItem(IDC_LOG_CALENDAR);
	MonthCal_GetToday(wnd->m_hWnd,&time);		//	당일을 얻어온다.

	if	( (time.wYear%4 == 0) && (time.wYear%100 !=0) || time.wYear%400 == 0)
		aiMonthend[2] = 29;
	else
		aiMonthend[2] = 28;

	//	로그 검색 기간 필터
	{
		m_ctlYY1.GetWindowText(tmpValue,10);	yy1 = atoi(tmpValue);
		m_ctlYY2.GetWindowText(tmpValue,10);	yy2 = atoi(tmpValue);
		m_ctlMM1.GetWindowText(tmpValue,10);	mm1 = atoi(tmpValue);
		m_ctlMM2.GetWindowText(tmpValue,10);	mm2 = atoi(tmpValue);
		m_ctlDD1.GetWindowText(tmpValue,10);	dd1 = atoi(tmpValue);
		m_ctlDD2.GetWindowText(tmpValue,10);	dd2 = atoi(tmpValue);
		m_ctlHour1.GetWindowText(tmpValue,10);	hour1 = atoi(tmpValue);
		m_ctlHour2.GetWindowText(tmpValue,10);	hour2 = atoi(tmpValue);
		m_ctlMin1.GetWindowText(tmpValue,10);	min1 = atoi(tmpValue);
		m_ctlMin2.GetWindowText(tmpValue,10);	min2 = atoi(tmpValue);

		if	(yy1	>	time.wYear || yy2	>	time.wYear || yy1>yy2)
		{	
			MSGBOX("검색 조건 문제","연도가 잘못되었습니다.");	
			goto label_error;
		}

		if	(( yy1%100==time.wYear && mm1>time.wMonth) || ( yy2%100==time.wYear && mm2>time.wMonth) || (yy1==yy2 && mm1>mm2) )
		{	
			MSGBOX("검색 조건 문제","월이 잘못되었습니다.");	
			goto label_error;
		}

		if	(( mm1==mm2 && dd1>dd2))
		{	
			MSGBOX("검색 조건 문제","일이 잘못되었습니다.");	
			goto label_error;
		}
		
		beginTime.m_wYear	=	yy1;	
		beginTime.m_wMonth	=	mm1;	
		beginTime.m_wDay	=	dd1;	
		beginTime.m_wHour	=	hour1;	
		beginTime.m_wMinute	=	min1;	

		endTime.m_wYear		=	yy2;
		endTime.m_wMonth	=	mm2;
		endTime.m_wDay		=	dd2;
		endTime.m_wHour		=	hour2;	
		endTime.m_wMinute	=	min2;

		int	iBaseYear			=	min(yy1,yy2);

		int	iBeginTimeDayValue	=	beginTime.getDayValue(iBaseYear);
		int	iEndTimeDayValue	=	endTime.getDayValue(iBaseYear);
		int	iDayCount			=	iEndTimeDayValue-iBeginTimeDayValue;

		if	(m_pProgressDlg)
			m_pProgressDlg->setFileCount(iDayCount+1);
	}

	iServerIndex	=	m_ctlServerList.GetCurSel();	//	선택한 서버 인덱스
	iFilterIndex	=	m_ctlCompare.GetCurSel();		//	필터
	iJobFilter		=	m_ctlCB_Job.GetCurSel();		//	필터

	if	(iJobFilter	==	0)
		g_iJobFilter	=	-1;
	else
		g_iJobFilter--;

	m_ctlId.GetWindowText(strFilterId,22);		//	필터 계정
	m_ctlName.GetWindowText(strFilterName,18);	//	필터 이름
	m_ctlSerial.GetWindowText(strFilterSerial,20);//	필터 시리얼
	m_ctlIP.GetWindowText(strFilterIp,16);		//	필터 아이피

	{
		m_editValue.GetWindowText(strValue,63);	//	다용도 수치

		if	(strlen(strValue))
			iValue	=	atoi(strValue);
	}

	//	특수 필터 운영자 로그 검색
	{
		if	(stricmp(strFilterName,"#7892")==0)
		{
			iFilterIndex	=	eABOUT_OPERATOR;
			strcpy(strFilterName,"");
		}
	}

	//	초기화할 데이타
	isSearchList	=	FALSE;

	yy	=	yy1;
	mm	=	mm1;
	dd	=	dd1;

	g_bIsExportIncorrectPrefixItem	=	FALSE;
	g_bIsHideItemInfo				=	FALSE;

	//	바닥 없는 상자 관련
	if	(iFilterIndex	==	eABOUT_BOTTOMLESS_BOX)
	{
		g_bIsHideItemInfo	=	TRUE;

		if	(stricmp(strFilterId,"버그아이템")==0)
		{
			g_bIsExportIncorrectPrefixItem	=	TRUE;
			strcpy(strFilterId,"");
		}
	}

	if	(g_duelFilter.m_iDuelSerialCount)
	{
		strFilterId[0]		=	NULL;
		strFilterName[0]	=	NULL;
		strFilterSerial[0]	=	NULL;
		strFilterIp[0]		=	NULL;
		iValue				=	0xffffffff;
	}

	if	(strFilterSerial[0])
		dwFilterItemSerial		=	(DWORD)atoi(strFilterSerial);

	if	(IsDlgButtonChecked(IDC_CHECK_INCORRECT_ITEM))
	{
		iFilterIndex			=	eITEM_ALL;
		g_bIsCheckIncorrectItem	=	TRUE;
	}
	else
		g_bIsCheckIncorrectItem	=	FALSE;

	//	필터 정보 저장
	{
		m_filter.m_iFilter		=	iFilterIndex;
		m_filter.m_iServer		=	iServerIndex;
		strcpy(m_filter.m_strIP,strFilterIp);
		strcpy(m_filter.m_strValue,strValue);
		strcpy(m_filter.m_strID,strFilterId);
		strcpy(m_filter.m_strName,strFilterName);
		m_filter.m_dwItem		=	dwSelectItemIndex;
		m_filter.m_dwItemSerial	=	dwFilterItemSerial;
		m_filter.m_bIsAndCondition=	bIsCheckAndCondition;
		m_filter.m_bIsIgnoreSaveLog	=	bIsIgnoreSaveLog;
		m_filter.m_bIsAddSaveLog	=	bIsAddSaveLog;
		m_filter.m_bIsOnlyUniqueItem=	g_bIsOnlyUnique;
		m_filter.m_bIsOnlyDXItem	=	g_bIsOnlyDX;
		m_filter.m_bIsOnlySetItem	=	g_bIsOnlySetItem;
		m_filter.m_bIsOnlyEternalItem=	g_bIsOnlyEternal;

		m_filter.m_iEternalGrade	=	g_iEternalGrade;

		memcpy(&m_filter.m_dateBegin,&beginTime,sizeof(CTimeInfo));
		memcpy(&m_filter.m_dateEnd,&endTime,sizeof(CTimeInfo));
	}

	if	(_bIsExportFilter)
		return;

	m_ctlLogData.ResetContent();
	m_ctlLogData.SetRedraw(FALSE);

	allData->Reset();
	searchData->Reset();

	m_dwCurListCount		=	m_dwSearchListCount	=	0;
	m_iLogNodeCountForSort	=	0;

	if	(m_ctlIsServer.GetCheck()	==	FALSE && IsDlgButtonChecked(IDC_IS_COPY_LOG_DATA) == TRUE)	//	클라이언트 모드일경우만
		bIsUseLocalData		=	TRUE;

	GetDlgItemText(IDC_DATAFOLERTXT,m_strServerLogDir,sizeof(m_strServerLogDir));
	
	{	//	reg에 로그 디렉토리를 기록한다.
		CRegKey		reg;

		if (reg.Open(g_hRegKey,g_lpszRegistry)==ERROR_SUCCESS)
		{
			reg.SetValue(m_strServerLogDir,"ToolLogPath");
			reg.Close();
		}
	}

	g_mapLevelForRebirthLog.clear();

	while(m_dwCurListCount<dMAX_READ_LOG_COUNT)
	{
		if	(m_bIsCancelSearch)
			break;

		WORD	lastSize = 0,lastType = 0;
		DWORD	filesize		=	0;
		DWORD	readsize		=	0;
		WORD	wRetryType		=	0;
		BOOL	bIsGetByServer	=	FALSE;
		char	strLocalFileFullPath[512];

		if	(bIsUseLocalData)	//	클라이언트 모드일경우만
		{
			if	(yy == cSRVUTIL::GetTime()/10000 && mm == (cSRVUTIL::GetTime()%10000)/100 && dd==cSRVUTIL::GetTime()%100)
				sprintf(strLocalFileFullPath,"%s\\%s\\today.log",m_strLocalLogDir,scp.getData("SERVERNAME",iServerIndex+1));
			else
				sprintf(strLocalFileFullPath,"%s\\%s\\%04d%02d%02d.log",m_strLocalLogDir,scp.getData("SERVERNAME",iServerIndex+1),yy,mm,dd);

			strcpy(strLogFileFullPath,strLocalFileFullPath);
		}
		else
		{
			if	(yy == cSRVUTIL::GetTime()/10000 && mm == (cSRVUTIL::GetTime()%10000)/100 && dd==cSRVUTIL::GetTime()%100)
				sprintf(strLogFileFullPath,"%s\\%s\\today.log",m_strServerLogDir,scp.getData("SERVERNAME",iServerIndex+1));
			else
				sprintf(strLogFileFullPath,"%s\\%s\\%04d%02d%02d.log",m_strServerLogDir,scp.getData("SERVERNAME",iServerIndex+1),yy,mm,dd);
		}

		if	(!l_fb.open(strLogFileFullPath))
		{
			if	(bIsUseLocalData	==	FALSE)	//	서버 모드라면 다시 할 필요 없다.
				goto LOOPEND;

			if	(yy == cSRVUTIL::GetTime()/10000 && mm == (cSRVUTIL::GetTime()%10000)/100 && dd==cSRVUTIL::GetTime()%100)
				sprintf(strLogFileFullPath,"%s\\%s\\today.log",m_strServerLogDir,scp.getData("SERVERNAME",iServerIndex+1));
			else
				sprintf(strLogFileFullPath,"%s\\%s\\%04d%02d%02d.log",m_strServerLogDir,scp.getData("SERVERNAME",iServerIndex+1),yy,mm,dd);

			if	(!l_fb.open(strLogFileFullPath))
				goto LOOPEND;

			bIsGetByServer	=	TRUE;
		}

		if	(bIsGetByServer)
		{
			char	strFolderPath[512];

			strcpy(strFolderPath,_exportFolderName(strLocalFileFullPath));
			
			cFOLDER::Create(strFolderPath);
		}

		iFileIndex++;

		m_pProgressDlg->setCurrentFileIndex(iFileIndex);
		m_pProgressDlg->setCurrentFile(strLogFileFullPath,l_fb.m_iLength);

		while(l_fb.uploadDataToBuffer(1024*100))
		{
			if	(m_bIsCancelSearch)
				break;

			m_pProgressDlg->setUploadFileSize(l_fb.m_iReadLength);
		}

		if	(bIsGetByServer)
			l_fb.saveFile(strLocalFileFullPath);

		if	(m_bIsCancelSearch)
			break;

		ALL_LOGMSG	logMsg;
	
		iReadCount = 0;
		
		ALL_LOGMSG	lastLog;

		while(m_dwCurListCount<dMAX_READ_LOG_COUNT)
		{
			if	(m_bIsCancelSearch)
				break;

			m_pProgressDlg->setAnalLogSize(l_fb.m_iBP);

			ALL_LOGMSG	*curPos	=	&logMsg;
RETRY_FILEREAD:

			mRB(curPos->base.wSize,sizeof(WORD));
			mRB(curPos->base.wType,sizeof(WORD));

			//	로그 데이터가 깨지는 경우가 있어 이를 보정하기 위해 몇가지 작업을 한다
			//	애초에 깨지면 안되는데 왜 깨지는건지.. - -

			if	(curPos->base.wType<dLOG_BEGIN || curPos->base.wType>dLOG_AVA_END_LOG || curPos->base.wSize>sizeof(ALL_LOGMSG) || curPos->base.wSize	<=	9)
			{	
				if	(curPos->base.wSize == 0)
				{
					if	(wRetryType<2)
					{
						l_fb.addBP(-2);

						goto RETRY_FILEREAD;
					}
				}
				if	(curPos->base.wSize>0x100 && curPos->base.wSize%0x100 >0)
				{
					if	(wRetryType<2)
					{
						l_fb.addBP(-3);

						goto RETRY_FILEREAD;
					}
				}

				if	(wRetryType == 0)
				{	
					l_fb.addBP(-3);

					wRetryType++;
				}
				else
				if	(wRetryType == 1)
				{
					l_fb.addBP(-3);

					wRetryType++;
				}
				else	
				{
					l_fb.addBP(-3);
				}

				goto RETRY_FILEREAD;
			}

			wRetryType	=	0;

			iReadCount++;

			lastSize	=	curPos->base.wSize;
			lastType	=	curPos->base.wType;

			mRB(logMsg.mLOG_ITEM_DROP.logTime,sizeof(logMsg.mLOG_ITEM_DROP.logTime));	//	시간

			if	(curPos->base.wType	<	dLOG_BEGIN || curPos->base.wType	>	dLOG_AVA_END_LOG)
				wRetryType= 0;

			memcpy(&lastLog,&logMsg,logMsg.base.wSize);

			BOOL	bIsSaveLogForRebirth	=	FALSE;

			if	(logMsg.base.wType	==	dLOG_AVA_SAVE_LOG && iFilterIndex == eABOUT_REBIRTH)
				bIsSaveLogForRebirth	=	TRUE;

			BOOL	bIsSkipLog	=	IsSkipDataByCheckTimeAndSerchType(bIsIgnoreSaveLog,bIsAddSaveLog,curPos,&curPos->mLOG_ITEM_DROP.logTime,yy1,yy2,mm1,mm2,dd1,dd2,hour1,hour2,min1,min2,curPos->base.wType,iFilterIndex);

// 			if	(curPos->mLOG_AVA_SAVE_LOG.logTime.mm	==	8	&&
// 				curPos->mLOG_AVA_SAVE_LOG.logTime.dd	==	12	&&
// 				curPos->mLOG_AVA_SAVE_LOG.logTime.hour	>=	16	&&
// 				curPos->mLOG_AVA_SAVE_LOG.logTime.min	>=	15
// 				)
// 				curPos	=	curPos;

//			bIsSkipLog	=	TRUE;

			if	(bIsSkipLog && bIsSaveLogForRebirth == FALSE)
			{
				l_fb.addBP(curPos->base.wSize-9);

				continue;
			}

			if	(!l_fb.read(((char *)&logMsg)+9,curPos->base.wSize-9))
				break;

			if	(bIsSaveLogForRebirth && logMsg.base.wType	==	dLOG_AVA_SAVE_LOG)
			{
				LOG_AVA_SAVE_LOG	*lpSaveLog	=	(LOG_AVA_SAVE_LOG	*)&logMsg;

				char			strUserId[dID_LENGTH],strUserName[dNAME_LENGTH];
				BOOL			bIsCheck	=	TRUE;
				cLevelAndExp	*lpMapExp	=	NULL;

				MapLevelLog::iterator	iter	=	g_mapLevelForRebirthLog.end();

				if	(GetSaveLogIDAndName2(lpSaveLog->strIdAndName,strUserId,strUserName))
				{
					iter	=	g_mapLevelForRebirthLog.find(strUserName);

					if	(iter!=	g_mapLevelForRebirthLog.end())
						lpMapExp=	&iter->second;
				}
				else
					bIsCheck=	FALSE;

				if	(strFilterName[0] && stricmp(strFilterName,strUserName) != 0)
					bIsCheck	=	FALSE;

				if	(bIsCheck && (lpSaveLog->wLevel >= 600 || lpMapExp))
				{
					if	(lpMapExp == NULL && lpSaveLog->wLevel >= 600)	//	처음 등록
					{
						cLevelAndExp	le;

						le.m_iLevel			=	lpSaveLog->wLevel;
						le.m_iExp			=	lpSaveLog->dwExperience;
						le.m_time.m_wYear	=	lpSaveLog->logTime.yy+2000;
						le.m_time.m_wMonth	=	lpSaveLog->logTime.mm;
						le.m_time.m_wDay	=	lpSaveLog->logTime.dd;
						le.m_time.m_wHour	=	lpSaveLog->logTime.hour;
						le.m_time.m_wMinute	=	lpSaveLog->logTime.min;
						le.m_time.m_wSecond	=	lpSaveLog->logTime.getSecond();
						le.m_bIsRookie		=	FALSE;

						g_mapLevelForRebirthLog.insert(PairLevelLog(strUserName,le));
					}
					else
					if	(lpMapExp)	//	검색 결과가 있다.
					{
						if	(lpSaveLog->wLevel >= 400 && lpSaveLog->wLevel	>	lpMapExp->m_iLevel + 20)	//	갑자기 20레벨 정도 뛰었다.
							lpSaveLog->wLevel	=	0;

						if	(lpSaveLog->wLevel	>	lpMapExp->m_iLevel || 
							(lpSaveLog->wLevel	== lpMapExp->m_iLevel && lpSaveLog->dwExperience	>=  lpMapExp->m_iExp ))
						{
							lpMapExp->m_iLevel			=	lpSaveLog->wLevel;
							lpMapExp->m_iExp			=	lpSaveLog->dwExperience;
							lpMapExp->m_time.m_wYear	=	lpSaveLog->logTime.yy+2000;
							lpMapExp->m_time.m_wMonth	=	lpSaveLog->logTime.mm;
							lpMapExp->m_time.m_wDay		=	lpSaveLog->logTime.dd;
							lpMapExp->m_time.m_wHour	=	lpSaveLog->logTime.hour;
							lpMapExp->m_time.m_wMinute	=	lpSaveLog->logTime.min;
							lpMapExp->m_time.m_wSecond	=	lpSaveLog->logTime.getSecond();
							lpMapExp->m_bIsRookie		=	FALSE;
						}
					}
				}

				if	(bIsSkipLog)
					continue;
			}

			if	(!CheckIsSerchData(iFilterIndex,strValue,bIsCheckAndCondition,strFilterId,strFilterName,dwSelectItemIndex,dwFilterItemSerial,strFilterIp,curPos))
				continue;

			if	(iFilterIndex == eABOUT_REBIRTH && logMsg.base.wType	==	dLOG_SYSTEM && logMsg.mLOG_SYSTEM.wWork == eSLT_REBIRTH)
			{
				LOG_S_REBIRTH			*lpData	=	(LOG_S_REBIRTH *)&logMsg.mLOG_SYSTEM;

	 			if	(logMsg.mLOG_SYSTEM.base.wSize	!=	sizeof(LOG_S_REBIRTH) || lpData->wPreLevel < 600)
				{
					MapLevelLog::iterator	iter	=	g_mapLevelForRebirthLog.find(lpData->strName);

					if	(iter	!=	g_mapLevelForRebirthLog.end())
					{
						cLevelAndExp	*lpMapExp	=	lpMapExp	=	&iter->second;

						logMsg.mLOG_SYSTEM.base.wSize	=	sizeof(LOG_S_REBIRTH);
						lpData->wPreLevel		=	lpMapExp->m_iLevel;
						lpData->iPreExp			=	lpMapExp->m_iExp;

						lpMapExp->m_bIsRookie=	TRUE;
						lpMapExp->m_iLevel	=	1;
						lpMapExp->m_iExp	=	0;
 
 						BYTE	bYM			=	lpMapExp->m_time.m_wYear-2000;
 						bYM					<<=	4;
 						bYM					+=	lpMapExp->m_time.m_wMonth;

						lpData->wLevel		=	0;
						lpData->wLevel		+=	bYM;
						lpData->wLevel		<<=	8;
						lpData->wLevel		+=	lpMapExp->m_time.m_wDay;
						lpData->wRebirthCount|=	0x8000;
					}
				}
			}

			CNode*	lpAddNode	=	allData->AddNode(logMsg);

			{
				tsLogTime	*lpLogTime		=	&curPos->mLOG_ITEM_DROP.logTime;
				DWORD		dwSecondValue	=	(lpLogTime->yy*60*60*24*366)+(lpLogTime->mm*60*60*24*31)+(lpLogTime->dd*60*60*24)+(lpLogTime->hour*60*60)+(lpLogTime->min*60)+lpLogTime->getSecond();

				m_aLogNodeForSort[m_iLogNodeCountForSort].m_dwTimeValue	=	dwSecondValue;
				m_aLogNodeForSort[m_iLogNodeCountForSort].m_lpNode		=	lpAddNode;	

				m_iLogNodeCountForSort++;
			}

			m_dwCurListCount++;

			m_pProgressDlg->setReadLogCount(m_dwCurListCount,FALSE);

			Sleep(1);
		}

LOOPEND:
		++dd;

		if	((yy%4 == 0) && (yy%100 !=0) || yy%400 == 0)
			aiMonthend[2] = 29;
		else
			aiMonthend[2] = 28;

		if	(dd>aiMonthend[mm])
		{	
			dd = 1;
			mm++;

			if	(mm>12)
			{	
				mm = 1;
				yy++;
			}
		}

		if	(yy>yy2)
			break;

		if	(yy==yy2 && mm>mm2)
			break;

		if	(yy==yy2 && mm==mm2 && dd>dd2)
			break;
	}

	//	시간에 따라 로그를 정렬하고 리스트 박스에 추가
	{
		qsort((void *)m_aLogNodeForSort,m_iLogNodeCountForSort,sizeof(CLogNode),(int(*)(const void *,const void *))CompareLogNode);

		m_pProgressDlg->setReadLogCount(m_dwCurListCount,TRUE);
		m_ctlLogData.SetRedraw(FALSE);

		char	addText[512];

		for (int i=0;i<m_iLogNodeCountForSort;i++)
		{
			CNode	*lpNode	=	m_aLogNodeForSort[i].m_lpNode;

 			addText[0]		=	NULL;
 
 			if	(!makeLogText(&lpNode->data,addText))
 				continue;
 
 			if	(addText[0]	==	NULL)
 				continue;
 
 			m_ctlLogData.AddString(addText);
 
 			if	(i%100	==	0)
 			{
 				m_ctlLogData.SetRedraw(TRUE);
 				m_ctlLogData.RedrawWindow();
 				m_ctlLogData.SetRedraw(FALSE);
 			}
		}

		m_ctlLogData.SetRedraw(TRUE);
		m_ctlLogData.RedrawWindow();
	}

	m_stComment.SetWindowText(_ms(" found %d numbers log",m_dwCurListCount));

	m_ctlLogData.SetRedraw(TRUE);
	l_fb.close();

	if	(m_dwCurListCount)
		m_uWaveDeviceId	=	PlayWavFile("ok.wav",m_hWnd);
	else
		m_uWaveDeviceId	=	PlayWavFile("failed.wav",m_hWnd);

//	if	(m_bIsCancelSearch	==	FALSE)
//		MSGBOX(dMSG_SERCH_RESULT,dMSG_SERCH_RESULT_FORM,m_dwCurListCount);

	return;

label_error:

	m_uWaveDeviceId	=	PlayWavFile("failed.wav",m_hWnd);

	l_fb.close();
}

void CAnalyzeLogDlg::OnLogDown() 
{
	// TODO: Add your control notification handler code here
	// 선택한 날짜가 정상인가 당일날은 안된다.
/*
	WORD	wServerIndex = m_ctlServerList.GetCurSel();
	char	strServerName[32];
	strcpy(strServerName,scp.getData("SERVERNAME",wServerIndex+1));

	SYSTEMTIME	time;
	CWnd * wnd = this->GetDlgItem(IDC_LOG_CALENDAR);
	MonthCal_GetCurSel(wnd->m_hWnd,&time);	//	현재 선택한 일자를 얻어온다.
	int t = time.wYear*10000;
	t +=time.wMonth*100;
	t +=time.wDay;
	DWORD	curDate = cSRVUTIL::GetTime() , selDate = t;

	if(selDate==curDate)
	{	
		MessageBox("You must download that by hand if you want a today's file");	
		return;	
	}

	if(selDate>curDate)	
	{	
		return;																
	}

	if(selDate<startdate)
	{	
		char tmp[512];
		sprintf(tmp,"It cann't download a file before %dst %dmonth, %dyear",(startdate/10000)%100,(startdate/10000)/100,startdate/10000);
		MessageBox(tmp);
		return;	
	}

	//	선택한 날짜의 파일이 있나 검색한다.
	char strMonthDir[1024];
	sprintf(strMonthDir,"%s\\%s",strSaveLogDir,scp.getData("SERVERNAME",m_ctlServerList.GetCurSel()+1));
	CreateDirectory(strMonthDir,NULL);
	sprintf(strMonthDir,"%s\\%d_%d",strMonthDir,time.wYear,time.wMonth);
	CreateDirectory(strMonthDir,NULL);

	char	fn[1024];
	char	strTarget[1024];
	sprintf(fn,"%s/SAVELOGFILE_00_%d%02d%02d.txt",strMonthDir,time.wYear,time.wMonth,time.wDay);
	FILE * logfp = NULL;
	logfp = fopen(fn,"r");
	if(!logfp)
	{	
		sprintf(strTarget,"%d_%d/press/SAVELOGFILE_00_%d%02d%02d.txt.nux",time.wYear,time.wMonth,time.wYear,time.wMonth,time.wDay);
		CFTPClient		ftp;
		ftp.Login(strServerIp,strFtpId,strFtpPw,FALSE,FtpPort);
		if(!ftp.IsInitialized())
		{	
			MessageBox("Can not connect!!");	
			return;	
		}
		//	파일을 다운 받는다.
		bool bAbort = false;
		ftp.Download(strTarget,strMonthDir,&bAbort);
		ftp.Logoff();

		//	압축을 풀고.. 
		sprintf(fn,"%s/SAVELOGFILE_00_%d%02d%02d.txt.nux",strMonthDir,time.wYear,time.wMonth,time.wDay);
		cNUX::PressExtract(fn,strMonthDir);

		//	nux파일은 죽인다.
		DeleteFile(fn);

		m_uWaveDeviceId	=	PlayWavFile("ok.wav",m_hWnd);

		MessageBox(dMSG_DOWNLOAD_COMPLETE);
	}
	else
	{
		m_uWaveDeviceId	=	PlayWavFile("failed.wav",m_hWnd);

		MessageBox("Retry!!");

		fclose(logfp);
	}
*/
}

void CAnalyzeLogDlg::OnLogLoad() 
{
	// TODO: Add your control notification handler code here
	PSZ pszFile=new char[32767];  
	CFileDialog dlgFileOpen(TRUE,"LOG",NULL, OFN_FILEMUSTEXIST,	//OFN_ALLOWMULTISELECT | 
		"RedStone Log File(*.log)|*.log||",NULL);
	::ZeroMemory(pszFile,32767);
	dlgFileOpen.m_ofn.lpstrFile=pszFile;
    dlgFileOpen.m_ofn.nMaxFile=32767;
    dlgFileOpen.m_ofn.nFileOffset=0;

	if	(dlgFileOpen.DoModal()==IDOK)
	{
		POSITION	pos = dlgFileOpen.GetStartPosition();

		LoadLogFile((char *)(dlgFileOpen.GetNextPathName(pos).operator const char*()));
	}

	delete  [] pszFile;
}

void CAnalyzeLogDlg::OnLogSave()
{
	// TODO: Add your control notification handler code here
	char	strfn[1024];
	CFileDialog dlgFileSave(FALSE,"log",NULL, OFN_EXPLORER, "RedStone Log File(*.log)|*.log||",NULL);
	if(dlgFileSave.DoModal()==IDOK)
		strcpy(strfn,(dlgFileSave.GetFileName().operator const char*()));
	else	return;

	FILE *fp = fopen(strfn,"wb");
	if(!fp)		{	MSGBOX("ERROR FILE SAVE","파일 생성 실패");	return ;	}

	ALL_LOGMSG	*logMsg;
	if(isSearchList==TRUE && m_dwSearchListCount>0)	//	search된 내용을 저장한다.
	{
		for(DWORD i=0;i<m_dwSearchListCount;i++)
		{	logMsg = searchData->Get(i);
			fwrite(logMsg,logMsg->base.wSize,1,fp);
		}
		MSGBOX("SUCCESS FILE SAVE","[%d]의 내용을 [%s]에 저장했습니다.",m_dwSearchListCount,strfn);
	}
	else
	if(m_dwCurListCount>0 && m_dwSearchListCount==0)	//	처음 검색한 내용을 저장한다.
	{
		for(DWORD i=0;i<m_dwCurListCount;i++)
		{	logMsg = allData->Get(i);
			fwrite(logMsg,logMsg->base.wSize,1,fp);
		}
		MSGBOX("SUCCESS FILE SAVE","[%d]의 내용을 [%s]에 저장했습니다.",m_dwCurListCount,strfn);
	}
	fclose(fp);
}

void CAnalyzeLogDlg::OnLogItemSave()
{	//	중복된 아이템을 제거해야 한다.
	// TODO: Add your control notification handler code here
	char		strfn[1024];
	CItemDefine	item;

	CFileDialog dlgFileSave(FALSE,"txt",NULL, OFN_EXPLORER, "RedStone Item List File(*.txt)|*.txt||",NULL);
	if(dlgFileSave.DoModal()==IDOK)
		strcpy(strfn,(dlgFileSave.GetFileName().operator const char*()));
	else	return;

	FILE *fp = fopen(strfn,"w");

	if	(!fp)
	{
		MSGBOX("ERROR FILE SAVE","파일 생성 실패");
		return ;
	}

	ALL_LOGMSG	*logMsg;

	if(isSearchList==TRUE && m_dwSearchListCount>0)	//	search된 내용을 저장한다.
	{
		for(DWORD i=0;i<m_dwSearchListCount;i++)
		{	logMsg = searchData->Get(i);
			char	txt[10240];
			char	strComment[10240];
			strComment[0]	=	0;

			switch(logMsg->base.wType)
			{
			case dLOG_ITEM_DROP:
			case dLOG_ITEM_PICK:
			case dLOG_ITEM_DEL:
			case dLOG_ITEM_CREATE_PREMIUM_ITEM:
			case dLOG_CREATE_ITEM:
			case dLOG_USE_ITEM:
			case dLOG_EXPIRED_ITEM:
			case dLOG_DOUBLE_ITEM:
			case dLOG_ENCHANT_ITEM:
			case dLOG_ITEM_SELL:
			case dLOG_PITCHMAN_SHOP_TRADE	:
				{
					int	iQuestCharSize = 0;

					item.reset();
					memcpy(&item,&logMsg->mLOG_ITEM_DROP.cItem,sizeof(logMsg->mLOG_ITEM_DROP.cItem));

					if (logMsg->base.wType	==	dLOG_ITEM_DROP)
					{
						if (logMsg->mLOG_ITEM_DROP.wMethod == 1)
						{
							strcpy(txt,"[Q] ");
							iQuestCharSize	=	4;
						}
					}
					else
					if (logMsg->base.wType	==	dLOG_ITEM_DEL)
					{
						if (logMsg->mLOG_ITEM_DEL.bf6Method == 1)
						{
							strcpy(txt,"[Q] ");
							iQuestCharSize	=	4;
						}
					}
					else
					if (logMsg->base.wType	==	dLOG_ITEM_PICK)
					{
						if (logMsg->mLOG_ITEM_DROP.wMethod == 1)
						{
							strcpy(txt,"[Q] ");
							iQuestCharSize	=	4;
						}
					}

					exportItemInfo(txt+iQuestCharSize,&item);

					if (logMsg->base.wType	==	dLOG_ITEM_SELL)
					{
						fprintf(fp,txt);
						if (logMsg->mLOG_ITEM_SELL.dwSellPrice & 0x80000000)
							logMsg->mLOG_ITEM_SELL.dwSellPrice -= 0x80000000;

						sprintf(txt,"    %d gold\n",logMsg->mLOG_ITEM_SELL.dwSellPrice);
					}
					if (logMsg->base.wType	==	dLOG_PITCHMAN_SHOP_TRADE)
					{
						fprintf(fp,txt);
						sprintf(txt,"    %d gold\n",logMsg->mLOG_PITCHMAN_SHOP_TRADE.bf31ItemPrice);
					}
					break;
				}

			case dLOG_ITEM_TRADE:
				{
					int c = 0;

					for(c = 0;c<logMsg->mLOG_ITEM_TRADE.wTrader1ItemCount;c++)
					{
						if(logMsg->mLOG_ITEM_TRADE.cItem[c].m_wBaseItem==0xffff)
							continue;

						if(c>0)
							fprintf(fp,txt);

						item.reset();

						memcpy(&item,&logMsg->mLOG_ITEM_TRADE.cItem[c],sizeof(logMsg->mLOG_ITEM_TRADE.cItem[c]));

						exportItemInfo(txt,&item);
					}
					for(c = logMsg->mLOG_ITEM_TRADE.wTrader1ItemCount;c<logMsg->mLOG_ITEM_TRADE.wTrader1ItemCount+logMsg->mLOG_ITEM_TRADE.bf15Trader2ItemCount;c++)
					{
						if(logMsg->mLOG_ITEM_TRADE.cItem[c].m_wBaseItem==0xffff)
							continue;
						if(c>logMsg->mLOG_ITEM_TRADE.wTrader1ItemCount)
							fprintf(fp,txt);
						item.reset();
						memcpy(&item,&logMsg->mLOG_ITEM_TRADE.cItem[c],sizeof(logMsg->mLOG_ITEM_TRADE.cItem[c]));

						exportItemInfo(txt,&item);
					}
					break;
				}
			case dLOG_ITEM_BANK_IN:
				{
					int c = 0;
					if(logMsg->mLOG_ITEM_BANK_IN.iGold>0)
					{
						sprintf(txt,"gold %d\n",logMsg->mLOG_ITEM_BANK_IN.iGold);
						fprintf(fp,txt);
					}
					for(c = 0;c<logMsg->mLOG_ITEM_BANK_IN.bf15Count;c++)
					{
						if(logMsg->mLOG_ITEM_BANK_IN.cItem[c].m_wBaseItem==0xffff)	continue;
						if(c>0)		fprintf(fp,txt);
						item.reset();
						memcpy(&item,&logMsg->mLOG_ITEM_BANK_IN.cItem[c],sizeof(logMsg->mLOG_ITEM_BANK_IN.cItem[c]));

						exportItemInfo(txt,&item);
					}
					break;
				}
			case dLOG_ITEM_BANK_OUT:
				{
					int c = 0;
					if(logMsg->mLOG_ITEM_BANK_OUT.iGold>0)
					{
						sprintf(txt,"gold %d\n",logMsg->mLOG_ITEM_BANK_OUT.iGold);
						fprintf(fp,txt);
					}
					for(c = 0;c<logMsg->mLOG_ITEM_BANK_OUT.bf15Count;c++)
					{
						if(logMsg->mLOG_ITEM_BANK_OUT.cItem[c].m_wBaseItem==0xffff)	continue;
						if(c>0)		fprintf(fp,txt);
						item.reset();
						memcpy(&item,&logMsg->mLOG_ITEM_BANK_OUT.cItem[c],sizeof(logMsg->mLOG_ITEM_BANK_OUT.cItem[c]));

						exportItemInfo(txt,&item);
					}
					break;
				}
/*			case dLOG_ITEM_WITHDRAW_PREMIUM_ITEM:
				{	//	뽑을 필요 없슴
					break;
				}
*/			default:
				continue;
			}
			fprintf(fp,txt);
		}
		MSGBOX("SUCCESS ITEM LIST FILE SAVE","[%d]의 내용을 [%s]에 저장했습니다.",m_dwSearchListCount,strfn);
	}
	else
	if(m_dwCurListCount>0 && m_dwSearchListCount==0)	//	처음 검색한 내용을 저장한다.
	{
		for(DWORD i=0;i<m_dwCurListCount;i++)
		{	
			logMsg = allData->Get(i);
			char	txt[10240];

			switch(logMsg->base.wType)
			{
			case dLOG_ITEM_DROP:
			case dLOG_ITEM_PICK:
			case dLOG_ITEM_DEL:
			case dLOG_ITEM_CREATE_PREMIUM_ITEM:
			case dLOG_CREATE_ITEM:
			case dLOG_USE_ITEM:
			case dLOG_EXPIRED_ITEM:
			case dLOG_DOUBLE_ITEM:
			case dLOG_ENCHANT_ITEM:
			case dLOG_ITEM_SELL:
			case dLOG_PITCHMAN_SHOP_TRADE:
				{
					int	iQuestCharSize = 0;

					item.reset();
					memcpy(&item,&logMsg->mLOG_ITEM_DROP.cItem,sizeof(logMsg->mLOG_ITEM_DROP.cItem));

					if (logMsg->base.wType	==	dLOG_ITEM_DROP)
					{
						if (logMsg->mLOG_ITEM_DROP.wMethod == 1)
						{
							strcpy(txt,"[Q] ");
							iQuestCharSize	=	4;
						}
					}
					else
					if (logMsg->base.wType	==	dLOG_ITEM_DEL)
					{
						if (logMsg->mLOG_ITEM_DEL.bf6Method == 1)
						{
							strcpy(txt,"[Q] ");
							iQuestCharSize	=	4;
						}
					}
					else
					if (logMsg->base.wType	==	dLOG_ITEM_PICK)
					{
						if (logMsg->mLOG_ITEM_DROP.wMethod == 1)
						{
							strcpy(txt,"[Q] ");
							iQuestCharSize	=	4;
						}
					}

					exportItemInfo(txt,&item);

					if (logMsg->base.wType	==	dLOG_ITEM_SELL)
					{
						if (logMsg->mLOG_ITEM_SELL.dwSellPrice & 0x80000000)
							logMsg->mLOG_ITEM_SELL.dwSellPrice -= 0x80000000;
						fprintf(fp,txt);
						sprintf(txt,"    %s Gold\n",GetMoneyString(logMsg->mLOG_ITEM_SELL.dwSellPrice));
					}

					if (logMsg->base.wType	==	dLOG_PITCHMAN_SHOP_TRADE)
					{
						fprintf(fp,txt);
						sprintf(txt,"    %s Gold\n",GetMoneyString(logMsg->mLOG_PITCHMAN_SHOP_TRADE.bf31ItemPrice));
					}
					break;
				}


			case dLOG_ITEM_TRADE:
				{
					int c = 0;
					for(c = 0;c<logMsg->mLOG_ITEM_TRADE.wTrader1ItemCount;c++)
					{
						if(logMsg->mLOG_ITEM_TRADE.cItem[c].m_wBaseItem==0xffff)	continue;
						if(c>0)		fprintf(fp,txt);
						item.reset();
						memcpy(&item,&logMsg->mLOG_ITEM_TRADE.cItem[c],sizeof(logMsg->mLOG_ITEM_TRADE.cItem[c]));

						exportItemInfo(txt,&item);
					}
					for(c = logMsg->mLOG_ITEM_TRADE.wTrader1ItemCount;c<logMsg->mLOG_ITEM_TRADE.wTrader1ItemCount+logMsg->mLOG_ITEM_TRADE.bf15Trader2ItemCount;c++)
					{
						if(logMsg->mLOG_ITEM_TRADE.cItem[c].m_wBaseItem==0xffff)	continue;
						if(c>logMsg->mLOG_ITEM_TRADE.wTrader1ItemCount)		fprintf(fp,txt);
						item.reset();
						memcpy(&item,&logMsg->mLOG_ITEM_TRADE.cItem[c],sizeof(logMsg->mLOG_ITEM_TRADE.cItem[c]));

						exportItemInfo(txt,&item);
					}
					break;
				}
			case dLOG_ITEM_BANK_IN:
				{
					int c = 0;
					if(logMsg->mLOG_ITEM_BANK_IN.iGold>0)
					{
						sprintf(txt,"gold %d\n",logMsg->mLOG_ITEM_BANK_IN.iGold);
						fprintf(fp,txt);
					}
					for(c = 0;c<logMsg->mLOG_ITEM_BANK_IN.bf15Count;c++)
					{
						if(logMsg->mLOG_ITEM_BANK_IN.cItem[c].m_wBaseItem==0xffff)	continue;
						if(c>0)		fprintf(fp,txt);
						item.reset();
						memcpy(&item,&logMsg->mLOG_ITEM_BANK_IN.cItem[c],sizeof(logMsg->mLOG_ITEM_BANK_IN.cItem[c]));

						exportItemInfo(txt,&item);
					}
					break;
				}
			case dLOG_ITEM_BANK_OUT:
				{
					int c = 0;
					if(logMsg->mLOG_ITEM_BANK_OUT.iGold>0)
					{
						sprintf(txt,"gold %d\n",logMsg->mLOG_ITEM_BANK_OUT.iGold);
						fprintf(fp,txt);
					}
					for(c = 0;c<logMsg->mLOG_ITEM_BANK_OUT.bf15Count;c++)
					{
						if(logMsg->mLOG_ITEM_BANK_OUT.cItem[c].m_wBaseItem==0xffff)	continue;
						if(c>0)		fprintf(fp,txt);
						item.reset();
						memcpy(&item,&logMsg->mLOG_ITEM_BANK_OUT.cItem[c],sizeof(logMsg->mLOG_ITEM_BANK_OUT.cItem[c]));

						exportItemInfo(txt,&item);
					}
					break;
				}
			case dLOG_ITEM_WITHDRAW_PREMIUM_ITEM:
				{
					break;
				}
			default:
				break;
			}
			fprintf(fp,txt);
		}
		MSGBOX("SUCCESS ITEM LIST FILE SAVE","[%d]의 내용을 [%s]에 저장했습니다.",m_dwCurListCount,strfn);
	}
	fclose(fp);
}

void CAnalyzeLogDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete  allData;
	delete	searchData;
}

void CAnalyzeLogDlg::OnLogAddFile() 
{
	// 분리된 두개의 파일을 하나로 합친다.
	char	add1File[256]="",add2File[256]="",newFile[256]="";

	OPENFILENAME	ofn;
	char	dir[512];
	GetCurrentDirectory(512,dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.log";
	ofn.lpstrTitle		=	"Select the first file.";
	ofn.lpstrFile		=	add1File;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"log";
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn))		return;

	GetCurrentDirectory(512,dir);
	ofn.lpstrTitle		=	"Select the second file.";
	ofn.lpstrFile		=	add2File;
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn))		return;

	FILE * fp1,* fp2;
	//	두 파일 모두 이상이 없나?

	//	두 파일을 합친다.
	fp1 = fopen(add1File,"rb");
	if(!fp1)	{	MessageBox("First file is not complete.");	return;	}
	fp2 = fopen(add2File,"rb");
	if(!fp2)	{	MessageBox("Second file is not complete.");	return;	}
	
	int file1size = filelength(fileno(fp1));
	int file2size = filelength(fileno(fp2));

	BYTE	* data1 = new BYTE[file1size];
	BYTE	* data2 = new BYTE[file2size];

	fread(data1,file1size,1,fp1);
	fread(data2,file2size,1,fp2);
	fclose(fp1);fclose(fp2);

	char	saveFilename[256]="";
	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.log";
	ofn.lpstrTitle		=	"Save file...";
	ofn.lpstrFile		=	saveFilename;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"log";
	ofn.lpstrInitialDir	=	dir;

	if (!GetSaveFileName(&ofn)){	MessageBox("File saving is cancel.");	 return ;	}

	FILE * newFp = fopen(saveFilename,"wb");
	if(!newFp)	{	MessageBox("File Create FAIL!! File saving is cancel.");	 return ;	}

	fwrite(data1,file1size,1,newFp);
	fwrite(data2,file2size,1,newFp);
	fclose(newFp);

	MessageBox("I make merge file !!");
}

void CAnalyzeLogDlg::OnNUXtoLOG()
{
	char	add1File[256]="";

	OPENFILENAME	ofn;
	char	dir[512];
	GetCurrentDirectory(512,dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.nux";
	ofn.lpstrTitle		=	"Select target nux file";
	ofn.lpstrFile		=	add1File;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"nux";
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn))		return;

	GetCurrentDirectory(512,dir);
	cNUX::PressExtract(add1File,dir);

	MessageBox("The End!!");
}

void CAnalyzeLogDlg::OnLogopen() //	압축하기
{
	// TODO: Add your control notification handler code here
	char	add1File[256]="";

	OPENFILENAME	ofn;
	char	dir[512];
	GetCurrentDirectory(512,dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.log";
	ofn.lpstrTitle		=	"Select target Log file";
	ofn.lpstrFile		=	add1File;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"log";
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn))		return;

	char	strNuxFile[1024],strTxtFile[1024];
	sprintf(strNuxFile,"%s.nux",add1File);
	sprintf(strTxtFile,"%s",add1File);

	cNUX::PressCreate(strNuxFile);
	cNUX::PressFile(strTxtFile);
	cNUX::PressClose();

	MessageBox("The End!!");
}

void CAnalyzeLogDlg::OnLogDown2() 
{
	if	(m_ctlIsServer.GetCheck())
	{
		g_bIsDownloadLogTime = FALSE;
		g_bIsWaitingDownloadLog = TRUE;
		HANDLE dDNThread = (HANDLE)_beginthread(DownLoadThread, 0 ,(void*)this);
	}

	// TODO: Add your control notification handler code here
	// 선택한 날짜가 정상인가 당일날은 안된다.
}

void CAnalyzeLogDlg::OnFixfile()
{
	// TODO: Add your control notification handler code here
	FILE * fp = NULL;
	FILE * newfp = NULL;
	DWORD	filesize =0;
	DWORD	readsize =0;

	char	strFile[512]="";
	char	strNewFile[512]="";
	OPENFILENAME	ofn;
	char	dir[512];
	GetCurrentDirectory(512,dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.log";
	ofn.lpstrTitle		=	"수정할 log파일을 선택하세요.";
	ofn.lpstrFile		=	strFile;
	ofn.nMaxFile		=	512;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"log";
	ofn.lpstrInitialDir	=	dir;
	if (!GetOpenFileName(&ofn))		return;

	GetCurrentDirectory(512,dir);
	sprintf(strNewFile,"%s\\tmp%d.log",dir,cSRVUTIL::GetTimes());

	char	strTxt[512];
	sprintf(strTxt,"[%s]파일의 수정작업을 시작합니다. 계속 하시겠습니까?",strFile);
	if (cMSG::YESNO("주의",strTxt)==IDNO) return;	

	fp = fopen(strFile,"rb");
	if(!fp)	{	MSGBOX("대왕에러","지현이에게 일러주세요. - error OnFixFile 1");	return;	}
	filesize = filelength(fileno(fp));
	if(filesize == 0){	MSGBOX("대왕에러","지현이에게 일러주세요. - error OnFixFile 2");	return;	}

	//	저장할 파일 이름
	newfp = fopen(strNewFile,"ab");
	if(!newfp)	{	MSGBOX("대왕에러","지현이에게 일러주세요. - error OnFixFile 3");	fclose(fp);	return;	}

	DWORD	realreadsize = 0;

	int iReadCount = 0;
	DWORD m_dwCurListCount = 0;
	WORD wRetryType = 0;

	//	잘못된 운영자 대화 로그를 빼고 다시 저장시킨다.

	WORD wCheckSize,wCheckType;
	DWORD dwTotalSize = 0,dwAllReadSize = 0;
	DWORD file1size = filelength(fileno(fp));
	while(1)
	{	if( !fread(&wCheckSize,sizeof(WORD),1,fp))	break;
		if( !fread(&wCheckType,sizeof(WORD),1,fp)) break;

		dwAllReadSize += wCheckSize;
		if(dwAllReadSize > file1size) break;

		BYTE * tmpData = new BYTE[wCheckSize];

		if(wCheckType == dLOG_SYSTEM)
		{
			fseek(fp,-4,SEEK_CUR);
			fread(tmpData,wCheckSize,1,fp);
			//	잘못 들어간 대화가 아니면 저장한다.
			if( ((ALL_LOGMSG *)tmpData)->mLOG_SYSTEM.wWork != eSLT_OPERATOR_CHAT_LOG)
			{
				fwrite(tmpData,wCheckSize,1,newfp);
				dwTotalSize+= wCheckSize;
			}else
			{
				printf("-0-");
			}
		}else
		{
			fseek(fp,-4,SEEK_CUR);
			fread(tmpData,wCheckSize,1,fp);
			fwrite(tmpData,wCheckSize,1,newfp);
			dwTotalSize+= wCheckSize;
		}
		delete [] tmpData; tmpData = NULL;
	}


/*	while(readsize<filesize && m_dwCurListCount<dMAX_READ_LOG_COUNT)
	{
		int ret = 0;	int iSpecial = 0;
		memset(&logMsg,0,sizeof(ALL_LOGMSG));
		ALL_LOGMSG	*curPos	=	&logMsg;
RETRY_FILEREAD:
		ret = fread(&curPos->base.wSize,sizeof(WORD),1,fp);	
		if(ret != 1)	
		{	
			break;
		}
		realreadsize += sizeof(WORD);
		ret = fread(&curPos->base.wType,sizeof(WORD),1,fp);	
		if(ret != 1)
		{
			realreadsize -= sizeof(WORD);
			break;
		}
		realreadsize += sizeof(WORD);
		if(curPos->base.wType<dLOG_GUILD_BATTLE_WORK || curPos->base.wType>dLOG_AVA_END_LOG)
		{	
			if(curPos->base.wSize == 0)
			{
				fseek(fp,-2,SEEK_CUR);
				realreadsize -= 2;
				goto RETRY_FILEREAD;
			}
			if(curPos->base.wSize>0x100 && curPos->base.wSize%0x100 >0)
			{
				fseek(fp,-3,SEEK_CUR);
				goto RETRY_FILEREAD;
			}
					if(wRetryType == 0){	fseek(fp,-4,SEEK_CUR);	fread(&tmp,1,1,fp);	wRetryType++;	realreadsize -= 3;	}
			else	if(wRetryType == 1){	fseek(fp,-3,SEEK_CUR);						wRetryType++;	realreadsize -= 3;	}
			else	
			{
				fseek(fp,-4,SEEK_CUR);
				realreadsize -= 4;
				fread(&tmp,1,1,fp);
				realreadsize += 1;
//				MSGBOX("ERROR LOG FILE READ","%d번째 type [0x%x] size[%d] readsize[%d]에 문제가 발생했습니다.",m_dwCurListCount,curPos->base.wType,curPos->base.wSize,readsize);	
				wRetryType = 0;
				goto RETRY_FILEREAD;
			}
			goto RETRY_FILEREAD;
		}
		wRetryType = 0;
		iReadCount++;
		fseek(fp,-4,SEEK_CUR);
		realreadsize -= 4;
		ret = fread(&logMsg,curPos->base.wSize,1,fp);
		readsize += curPos->base.wSize;
		realreadsize += curPos->base.wSize;
		if(curPos->base.wType<dLOG_GUILD_BATTLE_WORK || curPos->base.wType>dLOG_AVA_END_LOG)
		{
			fseek(fp,-curPos->base.wSize,SEEK_CUR);
			realreadsize -= curPos->base.wSize;
			break;
		}
		fwrite(&logMsg,curPos->base.wSize,1,newfp);
	}
*/
	fclose(newfp);
	fclose(fp);
//	DeleteFile(strFile);
//	MoveFile(strNewFile,strFile);
//	DeleteFile(strNewFile);

	MSGBOX("작업완료","로그 파일 수정 작업을 완료했습니다.");
}

//void CAnalyzeLogDlg::OnMMMciNotify()
//{
//	mciSendCommand(m_uWaveDeviceId, MCI_CLOSE, 0, (DWORD)NULL);
//}

void CAnalyzeLogDlg::OnClose() 
{
	if	(l_bIsExit	==	FALSE)
		if	(cMSG::YESNO("Exit Program","Do you really exit this program?")	==	IDNO)
			return;

	l_bIsExit	=	TRUE;
	
	CDialog::OnClose();
}

void CAnalyzeLogDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CAnalyzeLogDlg::OnCancel() 
{
	if	(l_bIsExit	==	FALSE)
		if	(cMSG::YESNO("Exit Program","Do you really exit this program?")	==	IDNO)
			return;

	l_bIsExit	=	TRUE;

	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CAnalyzeLogDlg::OnSelchangeYy1() 
{
	// TODO: Add your control notification handler code here
	int	aiMonthend[13]=	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int	iYear		=	m_ctlYY1.GetCurSel()+2003;

	if( (iYear%4 == 0) && (iYear%100 !=0) || iYear%400 == 0)
		aiMonthend[2] = 29;
	else
		aiMonthend[2] = 28;

	for(int dc=0;dc<31;dc++)
		m_ctlDD1.DeleteString(0);

	m_ctlMM1.SetCurSel(0);

	int		selectMon = m_ctlMM1.GetCurSel()+1;
	char	tchar[10];

	for(int tc = 1;tc<=aiMonthend[selectMon];tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_ctlDD1.AddString(tchar);
	}

	m_ctlDD1.SetCurSel(0);
}

void CAnalyzeLogDlg::OnSelchangeYy2() 
{
	// TODO: Add your control notification handler code here
	int	aiMonthend[13]=	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int	iYear		=	m_ctlYY2.GetCurSel()+2003;

	if( (iYear%4 == 0) && (iYear%100 !=0) || iYear%400 == 0)
		aiMonthend[2] = 29;
	else
		aiMonthend[2] = 28;

	for(int dc=0;dc<31;dc++)
		m_ctlDD2.DeleteString(0);

	m_ctlMM2.SetCurSel(0);

	int		selectMon = m_ctlMM2.GetCurSel()+1;
	char	tchar[10];

	for(int tc = 1;tc<=aiMonthend[selectMon];tc++)
	{
		sprintf(tchar,"%02d",tc);
		m_ctlDD2.AddString(tchar);
	}

	m_ctlDD2.SetCurSel(0);	
}

void CAnalyzeLogDlg::OnSpLogSearch() 
{
	OnLogSearch();
	// TODO: Add your control notification handler code here

}

int CALLBACK BrowseCallbackProc2(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		{
		}
		break;
	case BFFM_SELCHANGED:
		{
			TCHAR szText[MAX_PATH] = {0};
			SHGetPathFromIDList(reinterpret_cast<LPITEMIDLIST>(lParam), szText);
			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0,
						reinterpret_cast<LPARAM>(szText));
		}
		break;
	}
         
	return 0;
}

void CAnalyzeLogDlg::OnDatafolder() 
{
	// TODO: Add your control notification handler code here
	CString	m_strFolderPath;
	LPMALLOC pMalloc = NULL;
	LPITEMIDLIST pidl = NULL;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	
	// set the bi's default values
	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = _T("Current folder is:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc2;
	pidl = SHBrowseForFolder(&bi);
	if(pidl != NULL)
	{
		SHGetPathFromIDList(pidl, m_strFolderPath.GetBuffer(m_strFolderPath.GetLength()));
		UpdateData(FALSE);

/*		if(SHGetMalloc(&pMalloc) && pMalloc)
		{	
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
*/		sprintf(m_strServerLogDir,"%s",m_strFolderPath.operator const char*());
		SetDlgItemText(IDC_DATAFOLERTXT,m_strServerLogDir);

		{	//	reg에 로그 디렉토리를 기록한다.
			CRegKey		reg;
			if (reg.Open(g_hRegKey,g_lpszRegistry)==ERROR_SUCCESS)
			{
				reg.SetValue(m_strServerLogDir,"ToolLogPath");
				reg.Close();
			}
		}
	}
}

void CAnalyzeLogDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	_log(" g_bIsDownloadLogTime %d , cSRVUTIL::GetHour() %d \n",g_bIsDownloadLogTime , cSRVUTIL::GetHour());
	if(g_bIsDownloadLogTime && cSRVUTIL::GetHour() == 4)
	{
		g_bIsDownloadLogTime = FALSE;
		HANDLE dDNThread = (HANDLE)_beginthread(DownLoadThread, 0 ,(void*)this);
	}
	if(!g_bIsDownloadLogTime && !g_bIsWaitingDownloadLog && cSRVUTIL::GetHour() != 4)
	{
		g_bIsDownloadLogTime = TRUE;
	}
	CDialog::OnTimer(nIDEvent);
}


void CAnalyzeLogDlg::OnGetdaystateLogCalendar(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CAnalyzeLogDlg::OnSelchangeLogCalendar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CAnalyzeLogDlg::OnSelectLogCalendar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	static int reHour = cSRVUTIL::GetHour()-1;	if(reHour<0)	reHour = 0;
	// TODO: Add your control notification handler code here
	NMDAYSTATE* pNMDay = (NMDAYSTATE*)pNMHDR;

	SYSTEMTIME	time;
	CWnd * wnd = this->GetDlgItem(IDC_LOG_CALENDAR);
	MonthCal_GetCurSel(wnd->m_hWnd,&time);	//	현재 선택한 일자를 얻어온다.
	int t = (cSRVUTIL::GetYear()%100)+2000;
	t=cSRVUTIL::GetMonth()+1;
	t=cSRVUTIL::GetDay();
	if(time.wYear>(cSRVUTIL::GetYear()%100)+2000){	*pResult = 0;	return;	}
	if((time.wMonth>=(cSRVUTIL::GetMonth()+1) && time.wDay>cSRVUTIL::GetDay())&& time.wYear>=(cSRVUTIL::GetYear()%100)+2000){	*pResult = 0;	return;	}

	if(cSRVUTIL::GetTimes() > 000100 || cSRVUTIL::GetTimes()<235959)
	{
		cLOG->outputf("\n\nClick Calendar : %d , %d\n",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes());
		m_iSelectedDate = time.wYear * 10000 + time.wMonth * 100 + time.wDay;
	}
	
	*pResult = 0;
}

void CAnalyzeLogDlg::OnisDownLoadServer() 
{
	// TODO: Add your control notification handler code here
	DWORD check = m_ctlIsServer.GetCheck();

	if	(m_ctlIsServer.GetCheck())
	{
		if	(MessageBox("Do you wanna change the mode for download server? ","Mode Change",MB_YESNO)==IDNO)
		{
			m_ctlIsServer.SetCheck(0);
			return;
		}
	}
	else
	{
		if(MessageBox("Do you wanna change the mode for log search tool? ","Mode Change",MB_YESNO)==IDNO)
		{
			m_ctlIsServer.SetCheck(1);
			return;
		}
	}

	CRegKey		reg;
	if (reg.Open(g_hRegKey,g_lpszRegistry)==ERROR_SUCCESS)
	{
		reg.SetValue((DWORD)check,"IsLogDownServer");
		reg.Close();
	}
	MessageBox(" It'll be restart this program!!");
	exit(0);
}

void
CAnalyzeLogDlg::setLastestDate(int _iDay)
{
	CTimeInfo	beginTime,endTime;

	beginTime.update();
	endTime.update();

	beginTime.decreaseDay(_iDay);
	endTime.decreaseDay(1);

	m_ctlYY1.SelectString(0,_ms("%d",beginTime.m_wYear));
	m_ctlMM1.SelectString(0,_ms("%02d",beginTime.m_wMonth));
	m_ctlDD1.SelectString(0,_ms("%02d",beginTime.m_wDay));
	m_ctlHour1.SelectString(0,_ms("%02d",0));
	m_ctlMin1.SelectString(0,_ms("%02d",0));

	m_ctlYY2.SelectString(0,_ms("%d",endTime.m_wYear));
	m_ctlMM2.SelectString(0,_ms("%02d",endTime.m_wMonth));
	m_ctlDD2.SelectString(0,_ms("%02d",endTime.m_wDay));
	m_ctlHour2.SelectString(0,_ms("%02d",23));
	m_ctlMin2.SelectString(0,_ms("%02d",59));
}

void
CAnalyzeLogDlg::On1Day()
{
	setLastestDate(1);
}

void
CAnalyzeLogDlg::On1Week()
{
	setLastestDate(8);
}

void
CAnalyzeLogDlg::On2Week()
{
	setLastestDate(15);
}

void
CAnalyzeLogDlg::On1Month()
{
	setLastestDate(32);
}

void
CAnalyzeLogDlg::On6Month()
{
	setLastestDate(31*6+1);
}

void
CAnalyzeLogDlg::OnFullDay() 
{
	m_ctlHour1.SetCurSel(0);
	m_ctlMin1.SetCurSel(0);
	m_ctlHour2.SetCurSel(23);
	m_ctlMin2.SetCurSel(59);	
}

void
CAnalyzeLogDlg::OnSaveFilter() 
{
	searchLog(TRUE);

	char	strName[512]	=	"";

	if	(!cFILE::SaveBox(strName,"filter file {*.fil}\0*.fil\0\0","fil",_ms("%s\\filter",g_strLogFolder),"Export filter"))
		return;

	cFILE	file;

	file.Open(strName,"wb");

	file.Write(&m_filter,sizeof(m_filter));

	file.Close();
}

void
CAnalyzeLogDlg::OnLoadFilter() 
{
	char	strName[512]	=	"";

	if	(!cFILE::LoadBox(strName,"filter file {*.fil}\0*.fil\0\0","fil",_ms("%s\\filter",g_strLogFolder),"Load filter"))
		return;

	cFILE	file;

	if	(file.Open(strName,"rb"))
	{
		memset(&m_filter,0,sizeof(m_filter));

		file.Read(&m_filter,file.Length);

		file.Close();
	}

	applyFilter();
}

void
CAnalyzeLogDlg::applyFilter()
{
	m_ctlServerList.SetCurSel(m_filter.m_iServer);	//	선택한 서버 인덱스
	m_ctlCompare.SetCurSel(m_filter.m_iFilter);		//	필터

	m_ctlId.SetWindowText(m_filter.m_strID);		//	필터 계정
	m_ctlName.SetWindowText(m_filter.m_strName);	//	필터 이름
	m_editValue.SetWindowText(m_filter.m_strValue);	//	다용도 수치
	m_ctlIP.SetWindowText(m_filter.m_strIP);		//	필터 아이피

	if	(m_filter.m_dwItemSerial	!=	0xffffffff)
		m_ctlSerial.SetWindowText(_ms("%d",m_filter.m_dwItemSerial));//	필터 시리얼
	else
		m_ctlSerial.SetWindowText("");//	필터 시리얼

	if	(m_filter.m_dwItem	!=	0xffffffff)
	{
		char	strSelectItem[64];

		strcpy(strSelectItem,g_aBasicItem[m_filter.m_dwItem].m_strName);

		m_comboItemList.SelectString(0,strSelectItem);
	}
	else
		m_comboItemList.SelectString(0," valid item");

	m_ctlYY1.SelectString(0,_ms("%d",m_filter.m_dateBegin.m_wYear));
	m_ctlMM1.SelectString(0,_ms("%02d",m_filter.m_dateBegin.m_wMonth));
	m_ctlDD1.SelectString(0,_ms("%02d",m_filter.m_dateBegin.m_wDay));
	m_ctlHour1.SelectString(0,_ms("%02d",m_filter.m_dateBegin.m_wHour));
	m_ctlMin1.SelectString(0,_ms("%02d",m_filter.m_dateBegin.m_wMinute));

	m_ctlYY2.SelectString(0,_ms("%d",m_filter.m_dateEnd.m_wYear));
	m_ctlMM2.SelectString(0,_ms("%02d",m_filter.m_dateEnd.m_wMonth));
	m_ctlDD2.SelectString(0,_ms("%02d",m_filter.m_dateEnd.m_wDay));
	m_ctlHour2.SelectString(0,_ms("%02d",m_filter.m_dateEnd.m_wHour));
	m_ctlMin2.SelectString(0,_ms("%02d",m_filter.m_dateEnd.m_wMinute));

	CheckDlgButton(IDC_AND,m_filter.m_bIsAndCondition);
	CheckDlgButton(IDC_IGNORE_SAVE_LOG,m_filter.m_bIsIgnoreSaveLog);
	CheckDlgButton(IDC_ADD_SAVE_LOG,m_filter.m_bIsAddSaveLog);

	CheckDlgButton(IDC_IS_SET_ITEM,m_filter.m_bIsOnlySetItem);
	CheckDlgButton(IDC_IS_UNIQUE,m_filter.m_bIsOnlyUniqueItem);
	CheckDlgButton(IDC_IS_DX,m_filter.m_bIsOnlyDXItem);
	CheckDlgButton(IDC_IS_ETERNAL,m_filter.m_bIsOnlyEternalItem);
	SetDlgItemInt(IDC_ETERNAL_GRADE,m_filter.m_iEternalGrade);

	OnSelchangeCompare();
}

void CAnalyzeLogDlg::OnApplyEditFilter() 
{
	char	strFilter[512];

	GetDlgItemText(IDC_EDIT_FILTER,strFilter,512);

	CDecoder	decoder;
	cFilterData	filter;

	memset(&filter,0,sizeof(filter));

	decoder.UploadBuffer(strFilter,NULL);

	{
		char	*lpstrFilter=	decoder.getWord();

		if	(strcmp(lpstrFilter,"filter")	!=	0)
		{
			cMSG::Error("filter error!!","incorrect filter info!");
			return;
		}

		filter.m_iFilter	=	decoder.getNumber();
	}

	{
		decoder.getWord();

		int	iYear	=	decoder.getNumber();
		int	iMonth	=	decoder.getNumber();
		int	iDay	=	decoder.getNumber();
		int	iHour	=	decoder.getNumber();
		int	iMinute	=	decoder.getNumber();

		filter.m_dateBegin.m_wYear	=	iYear;
		filter.m_dateBegin.m_wMonth	=	iMonth;
		filter.m_dateBegin.m_wDay	=	iDay;
		filter.m_dateBegin.m_wHour	=	iHour;
		filter.m_dateBegin.m_wMinute=	iMinute;
	}

	{
		decoder.getWord();

		int	iYear	=	decoder.getNumber();
		int	iMonth	=	decoder.getNumber();
		int	iDay	=	decoder.getNumber();
		int	iHour	=	decoder.getNumber();
		int	iMinute	=	decoder.getNumber();

		filter.m_dateEnd.m_wYear	=	iYear;
		filter.m_dateEnd.m_wMonth	=	iMonth;
		filter.m_dateEnd.m_wDay		=	iDay;
		filter.m_dateEnd.m_wHour	=	iHour;
		filter.m_dateEnd.m_wMinute	=	iMinute;
	}

	{
		decoder.getWord();
		filter.m_iServer	=	decoder.getNumber();
	}

	{
		decoder.getWord();
		char	*lpstrIP	=	decoder.getStream();

		if	(strcmp(lpstrIP,"isValidCode")!=0)
			strcpy(filter.m_strIP,lpstrIP);
	}

	{
		decoder.getWord();
		char	*lpstrValue	=	decoder.getStream();

		if	(strcmp(lpstrValue,"isValidCode")!=0)
			strcpy(filter.m_strValue,lpstrValue);
	}

	{
		decoder.getWord();
		char	*lpstrID	=	decoder.getStream();

		if	(strcmp(lpstrID,"isValidCode")!=0)
			strcpy(filter.m_strID,lpstrID);
	}

	{
		decoder.getWord();
		char	*lpstrName	=	decoder.getStream();

		if	(strcmp(lpstrName,"isValidCode")!=0)
			strcpy(filter.m_strName,lpstrName);
	}

	{
		decoder.getWord();
		int		iItem		=	decoder.getNumber();

		filter.m_dwItem		=	iItem;
	}

	{
		decoder.getWord();
		int		iSerial		=	decoder.getNumber();

		filter.m_dwItemSerial=	iSerial;
	}

	{
		decoder.getWord();
		int		iAnd		=	decoder.getNumber();

		filter.m_bIsAndCondition=	iAnd;
	}

	{
		decoder.getWord();
		int		iIgnoreSaveLog	=	decoder.getNumber();
		
		filter.m_bIsIgnoreSaveLog=	iIgnoreSaveLog;
	}
	
	{
		decoder.getWord();
		int		iAddSaveLog	=	decoder.getNumber();
		
		filter.m_bIsAddSaveLog	=	iAddSaveLog;
	}

	{
		filter.m_bIsOnlySetItem		=	decoder.getNumber();
		filter.m_bIsOnlyUniqueItem	=	decoder.getNumber();
		filter.m_bIsOnlyDXItem		=	decoder.getNumber();
		filter.m_bIsOnlyEternalItem	=	decoder.getNumber();
		filter.m_iEternalGrade		=	decoder.getNumber();
	}

	memcpy(&m_filter,&filter,sizeof(filter));

	applyFilter();
}

void
CAnalyzeLogDlg::OnSelchangeCompare() 
{
	int	iFilterIndex	=	m_ctlCompare.GetCurSel();		//	필터

	if	(iFilterIndex	==	eABOUT_GUILD_WORK)
		SetDlgItemText(IDC_ST_NAME,"Guild Name");
	else
		SetDlgItemText(IDC_ST_NAME,"Player Name");

	if	(iFilterIndex	==	eABOUT_QUEST)
		SetDlgItemText(IDC_IT_SERIAL,"Reward Exp");
	else
		SetDlgItemText(IDC_IT_SERIAL,"IT Serial");
}

char*
CAnalyzeLogDlg::getEditFilterCode() 
{
	cFilterData	*lpFilter	=	&m_filter;

	sprintf(m_strEditFilterCode,"[filter %d] [begin %d/%d/%d %d:%d] [end %d/%d/%d %d:%d] [server %d] [IP \"%s\"] [Value \"%s\"] [ID \"%s\"] [Name \"%s\"] [Item %d] [Serial %d] [And %d] [IgnoreSaveLog %d] [AddSaveLog %d] [%d %d %d %d %d]",
		lpFilter->m_iFilter,
		lpFilter->m_dateBegin.m_wYear,lpFilter->m_dateBegin.m_wMonth,lpFilter->m_dateBegin.m_wDay,lpFilter->m_dateBegin.m_wHour,lpFilter->m_dateBegin.m_wMinute,
		lpFilter->m_dateEnd.m_wYear,lpFilter->m_dateEnd.m_wMonth,lpFilter->m_dateEnd.m_wDay,lpFilter->m_dateEnd.m_wHour,lpFilter->m_dateEnd.m_wMinute,
		lpFilter->m_iServer,lpFilter->m_strIP,lpFilter->m_strValue,lpFilter->m_strID,lpFilter->m_strName,
		lpFilter->m_dwItem,lpFilter->m_dwItemSerial,lpFilter->m_bIsAndCondition,lpFilter->m_bIsIgnoreSaveLog,lpFilter->m_bIsAddSaveLog,
		lpFilter->m_bIsOnlySetItem,lpFilter->m_bIsOnlyUniqueItem,lpFilter->m_bIsOnlyDXItem,lpFilter->m_bIsOnlyEternalItem,lpFilter->m_iEternalGrade);

	return	m_strEditFilterCode;
}

void CAnalyzeLogDlg::OnGetEditFilter() 
{
	searchLog(TRUE);

	char	*lpstrFilter	=	getEditFilterCode();

	SetDlgItemText(IDC_EDIT_FILTER,lpstrFilter);
}

void CAnalyzeLogDlg::OnIsNotCopyLogData() 
{
	saveCofig();
	// TODO: Add your control notification handler code here
	
}
