// AdminToolDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "AdminTool.h"
#include "AdminToolDlg.h"
#include <process.h>
#include <mmsystem.h>
#include "cPACKET_GAMESERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_TOOLLOGSERVER.h"
#include "cPLAYER_DATA.h"
#include "AdminLogCode.h"
#include "CMessage.h"
#include "cSRVUTIL.h"
#include "smtp.h"
#include "threadFunc.h"
#include "LottoEvent.h"
#include "EventTimeManager.h"

#include "buildVersion.h"	//	이건 공유 안하고 쓴다.
							//	없으면 파일을 생성한다.(공유 하지 말것!!)
							//	안에는
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	와 같이 선언 되어 있다. 뒤에 이름은 프로그래머 이름을 적을것

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int				g_iWorldCount	=	0;
cServerList		sList[dMAX_WORLD_SERVER_COUNT];
CTreeCtrl		*ptTree;
WORD			g_wMaxUserCount,g_wMaxTime;
WORD			g_wMaxYahooUserCount,g_wMaxYahooTime;
WORD			g_wMaxParanUserCount,g_wMaxParanTime;
int				g_iSelectedServer;
CAdminToolDlg	*g_lpAdminDlg	=	NULL;


CTentativeGuildManager	* l_pageTentativeGuild	=	NULL;
CServerTool				* l_lpServerTool		=	NULL;
cCarrotShop				* l_lpCarrotShop		=	NULL;
CEventManager			* l_lpEventManager		=	NULL;
cGuildInfo				* l_lpGuildInfo			=	NULL;
cPollDlg				* l_lpPollDlg			=	NULL;

/////////////////////////////////////////////////////////////////////////////
// CAdminToolDlg dialog

BOOL
IsConnectedAllServer(BOOL _bIsWantWarningMessage,BOOL _bIsIgnoreTestServer)
{
	for (int i=0;i<g_iWorldCount;i++)
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

		if (sList[i].isConnect	==	FALSE)
		{
			if (sList[i].isTestServer	&&	_bIsIgnoreTestServer && g_lWhereisServer!= eCOUNTRY_INNERTEST)
				continue;

			if (_bIsWantWarningMessage)
				cMSG::Put("",g_strToolMessage[10]);

			return	FALSE;
		}
	}

	return	TRUE;
}

BOOL
IsConnectedServer(int _iServerIndex,BOOL _bIsWantWarningMessage)
{
	if	(sList[g_iSelectedServer].worldsock.sock	==	0)
	{
		if (_bIsWantWarningMessage)
			cMSG::Put("",g_strToolMessage[10]);

		return	FALSE;
	}

	return	TRUE;
}

BOOL
IsConnectedTestServer(BOOL _bIsWantWarningMessage,int _iIndex)
{
	for (int i=0;i<g_iWorldCount;i++)
	{
		if	(_iIndex	!=	-1)
		{
			if	(i	!=	_iIndex)
				continue;
		}

		if	(g_lWhereisServer== eCOUNTRY_INNERTEST)
		{
			if (sList[i].isTestServer	!=	2)
				continue;
		}
		else
		{
			if (sList[i].isTestServer	==	2)
				continue;

			if (sList[i].isTestServer	==	0)
				continue;
		}

		if (sList[i].isConnect	==	FALSE)
		{
			if (_bIsWantWarningMessage)
				cMSG::Put("",g_strToolMessage[10]);
			return	FALSE;
		}
	}

	return	TRUE;
}

int
GetTestServerIndex()
{
	for (int i=0;i<g_iWorldCount;i++)
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

			if (sList[i].isTestServer	==	0)
				continue;
		}

		if (sList[i].isConnect	==	TRUE)
			return	i;
	}

	return	-1;
}


BOOL
CheckOperatorLevel(int _iLimitLevel)
{
#ifndef _FOR_CHINA
	if	(extLevel	<	_iLimitLevel)
	{
		cMSG::Put("",g_strToolMessage[148],_iLimitLevel);

		return FALSE;
	}
#else
	if	(extLevel & _iLimitLevel)
	{
		cMSG::Put("You have NOT a permission");
		return FALSE;
	}
#endif
	return	TRUE;
}

BOOL
SendWP_SimpleWork(int _iServerIndex,int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	SERVERStoWORLD_SIMPLE_WORK	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_SIMPLE_WORK);

	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;

	return	SendPacketToWorld(_iServerIndex,(char *)&packet);
}

BOOL
SendWP_LongSimpleWork(int _iServerIndex,int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6,
				  int _iValue7 ,int _iValue8 ,int _iValue9 ,int _iValue10 ,int _iValue11 ,int _iValue12 )
{
	SERVERStoWORLD_SIMPLE_WORK	packet;
	
	packet.base.set(sizeof(packet),dSERVERStoWORLD_SIMPLE_WORK);
	
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;
	packet.aiValue[6]	=	_iValue7;
	packet.aiValue[7]	=	_iValue8;
	packet.aiValue[8]	=	_iValue9;
	packet.aiValue[9]	=	_iValue10;
	packet.aiValue[10]	=	_iValue11;
	packet.aiValue[11]	=	_iValue12;
	
	return	SendPacketToWorld(_iServerIndex,(char *)&packet);
}


BOOL
SendPacketToWorld(int _iServerIndex,char *_lpPacket,BOOL _bSendOne,BOOL _bExceptTestServer)
{
	if	(_iServerIndex	>=0 && sList[_iServerIndex].isConnect)
	{
		sList[_iServerIndex].worldsock.SendPacket(_lpPacket,((cMSG_BASE_TYPE_FORWORLD*)_lpPacket)->wSize);	

		return	TRUE;
	}
	
	BOOL	bIsSended	=	FALSE;
	
	if (_iServerIndex	==	-1)
	{
		for (int i=0;i<g_iWorldCount;i++)
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
				if (_bExceptTestServer	&&	sList[i].isTestServer	==	1)
					continue;
			}

			if (sList[i].isConnect	==	FALSE)
				continue;

			sList[i].worldsock.SendPacket(_lpPacket,((cMSG_BASE_TYPE_FORWORLD*)_lpPacket)->wSize);	

			bIsSended	=	TRUE;

			if (_bSendOne)
				return	TRUE;
		}
	}

	if	(!bIsSended)
		return	cMSG::Error(dMSG_KRR,dMSG_IS_NOT_CONNECTED_WORLD_SERVER);

	return	TRUE;
}

BOOL
SendPacketToTestServer(char *_lpPacket)
{
	BOOL	bIsSended	=	FALSE;

	for (int i=0;i<g_iWorldCount;i++)
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
			if (sList[i].isTestServer	==	0)
				continue;
		}

		if (sList[i].isConnect	==	FALSE)
			continue;

		sList[i].worldsock.SendPacket(_lpPacket,((cMSG_BASE_TYPE_FORWORLD*)_lpPacket)->wSize);	

		bIsSended	=	TRUE;
	}

	if (!bIsSended)
		return	cMSG::Error(dMSG_KRR,dMSG_IS_NOT_CONNECTED_WORLD_SERVER);

	return	TRUE;
}

CAdminToolDlg::CAdminToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdminToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_iSelectedServer = -1;
	g_iWorldCount = 0;
	g_wMaxUserCount = g_wMaxTime = 0;
	m_pCtlImageList = NULL;
	g_lpAdminDlg	=	this;
}

void CAdminToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminToolDlg)
	DDX_Control(pDX, ID_CONNECT, m_ctlConnect);
	DDX_Control(pDX, IDC_SERVER_TREE, m_ctlServerTree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAdminToolDlg, CDialog)
	//{{AFX_MSG_MAP(CAdminToolDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_SERVER_TREE, OnSelchangedServerTree)
	ON_BN_CLICKED(ID_CONNECT, OnConnect)
	ON_BN_CLICKED(ID_INFO_UPDATE, OnInfoUpdate)
	ON_NOTIFY(NM_RCLICK, IDC_SERVER_TREE, OnRclickServerTree)
	ON_NOTIFY(NM_CLICK, IDC_SERVER_TREE, OnClickServerTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminToolDlg message handlers

BOOL CAdminToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::SendMessage(GetDlgItem(IDC_SERVER_TREE)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	int szie = sizeof(cServerList);

	char title[512];
	
	switch(g_iToolType)
	{
		case	eTOOL_FOR_NORMAL		:
			sprintf(title,"Admin Tool For RedStone Online (Built Time %s %s by %s)",__DATE__,__TIME__,dBUILDER_NAME);
			break;
		case	eTOOL_FOR_TEST			:
			sprintf(title,"Test Version - Admin Tool For RedStone Online (Built Time %s %s by %s)",__DATE__,__TIME__,dBUILDER_NAME);
			break;
		case	eTOOL_FOR_BALANCE		:
			sprintf(title,"For Design Team Test - Admin Tool For RedStone Online (Built Time %s %s by %s)",__DATE__,__TIME__,dBUILDER_NAME);
			break;
		case	eTOOL_FOR_INTERNATIONAL	:
			sprintf(title,"For International - Admin Tool For RedStone Online (Built Time %s %s by %s)",__DATE__,__TIME__,dBUILDER_NAME);
			break;
	}

	SetWindowText(title);

#ifndef _FOR_KOREA
	SetDlgItemText(ID_CONNECT,g_strDlgText[0]);
	SetDlgItemText(ID_INFO_UPDATE,g_strDlgText[1]);
#endif

/*	if	(!AfxSocketInit())
	{
		AfxMessageBox("Could not initialize Windows Sockets!");
		return FALSE;
	}
*/
#ifndef _FOR_INTER
	BOOL	bUseToolLog	=	(g_iToolLogPort > 0 && g_strToolLogIP[0] && stricmp(g_strToolLogIP,"0") != 0 && stricmp(g_strToolLogIP,"none") != 0);
	g_bIsRequireSendToolLog	=	bUseToolLog;

	if	(bUseToolLog && !logSock.init(g_iToolLogPort,g_strToolLogIP,"TOOL LOG SERVER",4))
	{
		g_bIsRequireSendToolLog	=	FALSE;
		bUseToolLog				=	FALSE;
	}
#else
	BOOL	bUseToolLog	=	FALSE;
#endif

	char hostname[32];
	gethostname(hostname,sizeof(hostname));
	struct hostent* he = gethostbyname(hostname);
	struct in_addr iaddr;
	memset(&iaddr,0,sizeof(iaddr));
	if	(he && he->h_addr_list[0])
		memcpy(&iaddr,he->h_addr_list[0],sizeof(iaddr));

	PACKET_LOGIN	sPacket;
	sPacket.base.set(sizeof(PACKET_LOGIN),ePACKET_LOGIN);
	strcpy(sPacket.strId,strAdminId);
	if	(he && he->h_addr_list[0])
		memcpy(&iaddr,he->h_addr_list[0],sizeof(iaddr));
	strcpy(sPacket.strIp,inet_ntoa(iaddr));

	if	(bUseToolLog && !logSock.SendPacket((char *)&sPacket,sPacket.base.wSize))
	{
		g_bIsRequireSendToolLog	=	FALSE;
	}
////	extLevel	=	eAL_SYSTEM_OPERATER;

	cPLAYER_DATA::Init();

	//	Set the icon for this dialog.  The framework does this automatically
	//	when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if	(extLevel	!=	eAL_NORMAL	||	extLevel	==	eAL_ONLY_SEARCH)
	{
	/////////////////////////////////////////////////////////////
	//
		char fn[2048];

		sprintf(fn,"%s\\ReadMe_dev.txt",strCurPath);
		if	(!IsFile(fn))
			sprintf(fn,"%s\\ReadMe.txt",strCurPath);

		scp.Read(fn);
		int count = atoi(scp.getData("WORLD_COUNT"));
		g_iWorldCount = count;
		char	name[dNAME_LENGTH];
		int realIdx = 0;
		for	(int i=0;i<count;i++)
		{
			sprintf(name,"WORLD%d",i);
			strcpy(sList[realIdx].strName,scp.getData(name));
			if(strcmp(sList[realIdx].strName,"")==0)
			{
				g_iWorldCount --;
				sList[realIdx].isConnect = FALSE;
				continue;
			}
			if(g_lWhereisServer != eCOUNTRY_INNERTEST)
			{
				if(strstr(sList[realIdx].strName,"내부"))
				{
					g_iWorldCount --;
					continue;
				}
			}else
			{
				if(strstr(sList[realIdx].strName,"내부")==0)
				{
					g_iWorldCount --;
					continue;
				}
			}

			strcpy(sList[realIdx].strIP,scp.getData(name,2));
			strcpy(sList[realIdx].strDBIP,scp.getData(name,3));
			strcpy(sList[realIdx].strDBCIP,scp.getData(name,4));

			sList[realIdx].wGameServerCount	=	atoi(scp.getData(name,5));
			sList[realIdx].startdate		=	atoi(scp.getData(name,6));
			sList[realIdx].isTestServer		=	atoi(scp.getData(name,7));

	/*		if(g_lWhereisServer== eCOUNTRY_INNERTEST && strstr(sList[i].strIP,"211.235.238")==NULL)
				continue;
			if(g_lWhereisServer!= eCOUNTRY_INNERTEST && stricmp(sList[i].strIP,"211.235.238.104")==0)
				continue;
	*/
			for(int j=0;j<sList[realIdx].wGameServerCount;j++)
			{
				char gsname[dNAME_LENGTH];

				sprintf(gsname,"W%dGAME%d",i,j);

				strcpy(sList[realIdx].cGS[j].strGameIP,scp.getData(gsname));
			}

			realIdx++;
		}
	//
	/////////////////////////////////////////////////////////////
	}
	//	sheet 설정
	switch(extLevel)
	{
	case eAL_ONLY_SEARCH	:	//	만든이들..
		m_Sheet.AddPage(&m_pageCharacterInfo);
		break;

	case eAL_SYSTEM_OPERATER:	//	만든이들..
		if	(g_bIsLimitedEdition	==	FALSE)
		{
			m_Sheet.AddPage(&m_PageNotice);
			m_Sheet.AddPage(&m_PageUserSetting);
			m_Sheet.AddPage(&m_pageCharacterInfo);
			m_Sheet.AddPage(&m_pageServerTool);
		}

		m_Sheet.AddPage(&m_pageItemPackList);

		if	(g_bIsLimitedEdition	==	FALSE)
		{
			m_Sheet.AddPage(&m_pageCarrotShop);
			m_Sheet.AddPage(&m_pageEventManager);
			m_Sheet.AddPage(&m_pageGuildInfo);
			m_Sheet.AddPage(&m_pageTentativeGuild);
			m_Sheet.AddPage(&m_pageSearch);
			m_Sheet.AddPage(&m_PageWorldScore);
			m_Sheet.AddPage(&m_PagePvPTool);
		}

		m_Sheet.AddPage(&m_pagePollDlg);
		break;

	case eAL_MASTER_OPERATER:
#ifndef _FOR_THAI
		m_Sheet.AddPage(&m_PageNotice);
		m_Sheet.AddPage(&m_PageUserSetting);
		m_Sheet.AddPage(&m_pageCharacterInfo);
		m_Sheet.AddPage(&m_pageServerTool);
		m_Sheet.AddPage(&m_pageItemPackList);
		m_Sheet.AddPage(&m_pageCarrotShop);
		m_Sheet.AddPage(&m_pageEventManager);
		m_Sheet.AddPage(&m_pageGuildInfo);
		m_Sheet.AddPage(&m_pageTentativeGuild);
		m_Sheet.AddPage(&m_pageSearch);
		m_Sheet.AddPage(&m_PageWorldScore);
		m_Sheet.AddPage(&m_PagePvPTool);
		m_Sheet.AddPage(&m_pagePollDlg);
#else
		m_Sheet.AddPage(&m_PageUserSetting);
		m_Sheet.AddPage(&m_pageCharacterInfo);
		m_Sheet.AddPage(&m_pageEventManager);
		m_Sheet.AddPage(&m_pageGuildInfo);
		m_Sheet.AddPage(&m_pageTentativeGuild);
		m_Sheet.AddPage(&m_PagePvPTool);
		m_Sheet.AddPage(&m_pagePollDlg);
#endif
		break;
	case eAL_BOSS:	//	사장님
		m_Sheet.AddPage(&m_PageNotice);
//		m_Sheet.AddPage(&m_PageUserSetting);
//		m_Sheet.AddPage(&m_pageCharacterInfo);
//		m_Sheet.AddPage(&m_pageEventManager);
//		m_Sheet.AddPage(&m_pageTentativeGuild);
//		m_Sheet.AddPage(&m_pageSearch);
		m_Sheet.AddPage(&m_PageWorldScore);
		break;
	case eAL_OPERATOR:	//	공지에 대한 권한 & 관리팀
		m_Sheet.AddPage(&m_PageNotice);
		m_Sheet.AddPage(&m_PageUserSetting);
		m_Sheet.AddPage(&m_pageCharacterInfo);
		m_Sheet.AddPage(&m_pageItemPackList);
		m_Sheet.AddPage(&m_pageCarrotShop);
		m_Sheet.AddPage(&m_pageGuildInfo);
		m_Sheet.AddPage(&m_pageEventManager);
		m_Sheet.AddPage(&m_pageTentativeGuild);
		m_Sheet.AddPage(&m_pageSearch);
		m_Sheet.AddPage(&m_PageWorldScore);
		m_Sheet.AddPage(&m_PagePvPTool);
		m_Sheet.AddPage(&m_pagePollDlg);
		break;
	case eAL_TESTER:
		m_Sheet.AddPage(&m_PageNotice);
		m_Sheet.AddPage(&m_pageCharacterInfo);
		m_Sheet.AddPage(&m_pageEventManager);
		break;
	case eAL_NORMAL:	//	월드의 기록만을 볼수 있다. 외부에 보내줄때만
		/////////////////////////////////////////////////////////////
		//
		char fn[2048];
		sprintf(fn,"%s\\ReadMe_dev.txt",strCurPath);
		if	(!IsFile(fn))
			sprintf(fn,"%s\\ReadMe.txt",strCurPath);

		scp.Read(fn);
		int count = atoi(scp.getData("WORLD_COUNT"));
		g_iWorldCount = count;
		char	name[dNAME_LENGTH];
	//	if(count>0)		g_iSelectedServer = 0;
		for(int i=0;i<count;i++)
		{
			sprintf(name,"WORLD%d",i);
			strcpy(sList[i].strName,scp.getData(name));
			if(strcmp(sList[i].strName,"")==0)
			{
				g_iWorldCount--;
				sList[i].isConnect = FALSE;
				continue;
			}
			if(g_lWhereisServer != eCOUNTRY_INNERTEST)
			{
				if(strstr(sList[i].strName,"내부"))
				{
					g_iWorldCount --;
					continue;
				}
			}else
			{
				if(strstr(sList[i].strName,"내부")==0)
				{
					g_iWorldCount --;
					continue;
				}
			}

			strcpy(sList[i].strIP,scp.getData(name,2));
			strcpy(sList[i].strDBIP,scp.getData(name,3));
			strcpy(sList[i].strDBCIP,scp.getData(name,4));
			sList[i].wGameServerCount = atoi(scp.getData(name,5));
			sList[i].startdate = atoi(scp.getData(name,6));

			for(int j=0;j<sList[i].wGameServerCount;j++)
			{
				char gsname[dNAME_LENGTH];
				sprintf(gsname,"W%dGAME%d",i,j);
				strcpy(sList[i].cGS[j].strGameIP,scp.getData(gsname));
			}

			//	결과를 토대로 트리에 등록한다.
			char	nameex[128];
			sprintf(nameex,"%s(%s)",sList[i].strName,sList[i].strIP);
			sList[i].tvStruct.hParent = 0;
			sList[i].tvStruct.hInsertAfter = TVI_LAST;
			sList[i].tvStruct.item.pszText=nameex;
			sList[i].tvStruct.item.iImage = 0;
			sList[i].tvStruct.item.iSelectedImage = 0;
			sList[i].tvStruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
			sList[i].hTreeItem = m_ctlServerTree.InsertItem(&sList[i].tvStruct);
		}
		//
		/////////////////////////////////////////////////////////////
		m_Sheet.AddPage(&m_PageWorldScore);
		break;
	}
	
	l_pageTentativeGuild	=	&m_pageTentativeGuild;
	l_lpServerTool			=	&m_pageServerTool;
	l_lpCarrotShop			=	&m_pageCarrotShop;
	l_lpEventManager		=	&m_pageEventManager;
	l_lpGuildInfo			=	&m_pageGuildInfo;
	l_lpPollDlg				=	&m_pagePollDlg;
	g_lpCharacterInfo		=	&m_pageCharacterInfo;

	m_Sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_Sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_Sheet.ModifyStyle( 0, WS_TABSTOP | WS_THICKFRAME );

	CRect rcSheet;

	GetDlgItem( IDC_SHEET )->GetWindowRect( &rcSheet );
	ScreenToClient( &rcSheet );

	m_Sheet.SetWindowPos( NULL, rcSheet.left, rcSheet.top, rcSheet.Width(), rcSheet.Height(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
//	m_Sheet.MoveWindow( rcSheet.left, rcSheet.top, rcSheet.Width(), rcSheet.Height());
	//	트리에서 사용할 이미지 리스트 
	m_pCtlImageList = new CImageList();
	m_pCtlImageList->Create(16,16,TRUE,2,1);
	CBitmap	bitmap;
	bitmap.LoadBitmap(IDB_RED_BALL);
	m_pCtlImageList->Add(&bitmap,(COLORREF)0xffffff);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_GREEN_BALL);
	m_pCtlImageList->Add(&bitmap,(COLORREF)0xffffff);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_INFO);
	m_pCtlImageList->Add(&bitmap,(COLORREF)0xffffff);
	bitmap.DeleteObject();
	//	tree 초기화 하고..
	m_ctlServerTree.SetImageList(m_pCtlImageList,TVSIL_NORMAL);
	m_ctlServerTree.DeleteAllItems();
	ptTree = &m_ctlServerTree;

	if	(extLevel	!=	eAL_NORMAL	||	extLevel	==	eAL_ONLY_SEARCH)
	{
	/////////////////////////////////////////////////////////////
	//
		char fn[2048];

		sprintf(fn,"%s\\ReadMe_dev.txt",strCurPath);
		if	(!IsFile(fn))
			sprintf(fn,"%s\\ReadMe.txt",strCurPath);

		scp.Read(fn);
		int count = atoi(scp.getData("WORLD_COUNT"));
		g_iWorldCount = count;
		char	name[dNAME_LENGTH];
		int realIdx = 0;
		for	(int i=0;i<count;i++)
		{
			sprintf(name,"WORLD%d",i);
			strcpy(sList[realIdx].strName,scp.getData(name));
			if(strcmp(sList[realIdx].strName,"")==0)
			{
				g_iWorldCount--;
				sList[realIdx].isConnect = FALSE;
				continue;
			}
			if(g_lWhereisServer != eCOUNTRY_INNERTEST)
			{
				if(strstr(sList[realIdx].strName,"내부"))
				{
					g_iWorldCount --;
					continue;
				}
			}else
			{
				if(strstr(sList[realIdx].strName,"내부")==0)
				{
					g_iWorldCount --;
					continue;
				}
			}

			strcpy(sList[realIdx].strIP,scp.getData(name,2));
			strcpy(sList[realIdx].strDBIP,scp.getData(name,3));
			strcpy(sList[realIdx].strDBCIP,scp.getData(name,4));

			sList[realIdx].wGameServerCount	=	atoi(scp.getData(name,5));
			sList[realIdx].startdate			=	atoi(scp.getData(name,6));
			sList[realIdx].isTestServer		=	atoi(scp.getData(name,7));

	/*		if(g_lWhereisServer== eCOUNTRY_INNERTEST && strstr(sList[i].strIP,"211.235.238")==NULL)
				continue;
			if(g_lWhereisServer!= eCOUNTRY_INNERTEST && stricmp(sList[i].strIP,"211.235.238.104")==0)
				continue;
	*/
			for(int j=0;j<sList[realIdx].wGameServerCount;j++)
			{
				char gsname[dNAME_LENGTH];

				sprintf(gsname,"W%dGAME%d",i,j);

				strcpy(sList[realIdx].cGS[j].strGameIP,scp.getData(gsname));
			}

			//	결과를 토대로 트리에 등록한다.

			char	nameex[128];

			sprintf(nameex,"%s(%s)",sList[realIdx].strName,sList[realIdx].strIP);

			if	(!sList[realIdx].isConnect)
			{
				sList[realIdx].tvStruct.hParent = 0;
				sList[realIdx].tvStruct.hInsertAfter = TVI_SORT;
				sList[realIdx].tvStruct.item.pszText=nameex;
				sList[realIdx].tvStruct.item.iImage = 0;
				sList[realIdx].tvStruct.item.iSelectedImage = 0;
				sList[realIdx].tvStruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
				sList[realIdx].hTreeItem = m_ctlServerTree.InsertItem(&sList[realIdx].tvStruct);
			}
			else
			{
				sList[realIdx].tvStruct.hParent = 0;
				sList[realIdx].tvStruct.hInsertAfter = TVI_SORT;
				sList[realIdx].tvStruct.item.pszText=nameex;
				sList[realIdx].tvStruct.item.iImage = 1;
				sList[realIdx].tvStruct.item.iSelectedImage = 1;
				sList[realIdx].tvStruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
				sList[realIdx].hTreeItem = m_ctlServerTree.InsertItem(&sList[realIdx].tvStruct);
			}
			realIdx++;
		}
	//
	/////////////////////////////////////////////////////////////
	}

	SetTimer(1,5000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAdminToolDlg::OnPaint()
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
HCURSOR CAdminToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void cThreadFunc::WorldCtlThread(void *tmp)
{
	int worldidx = (int)tmp;
	int time	 = timeGetTime();

	{	//	로그인 하기~ 
		SERVERStoWORLD_LOGIN	cLogin;

		cLogin.base.set((WORD)sizeof(cLogin),dSERVERStoWORLD_LOGIN);
		strcpy(cLogin.strServerName,"#admin");

		cLogin.wMapCount	=	8;	//	툴 버젼
		cLogin.wServerIndex	=	0xffff;
		cLogin.base.wSize	=	sizeof(cLogin)-sizeof(cLogin.cGSInfo);

		sList[worldidx].worldsock.SendPacket((char *)&cLogin,cLogin.base.wSize);
	}

	WORD	recvsize=0,type=0;
	char	recvpacket[dMAX_PACKET_LENGTH];
	memset(recvpacket,0,dMAX_PACKET_LENGTH);

	while(sList[worldidx].worldsock.isLoopRecv)
	{
		Sleep(1);
		while(sList[worldidx].worldsock.GetRecvPacket(recvpacket))
		{
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);
			//	데이타를 기억했다가 갱신버튼 누르면 화면에 뿌려준다.
			if(type == dCM_COMPRESS)
			{
				CM_COMPRESS	comp;

				memcpy(&comp,recvpacket,recvsize);

				cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);
				recvsize = *(WORD *)recvpacket;
				type = *(WORD *)(recvpacket+2);
			}

			switch(type)
			{
			case dWORLDtoSERVERS_CARROT_SHOP_INFO		:
				{
//					WORLDtoSERVERS_CARROT_SHOP_INFO *lpPacket	=	(WORLDtoSERVERS_CARROT_SHOP_INFO *)recvpacket;

//					l_lpCarrotShop->receiveCarrotShopInfo(lpPacket->wIndex,lpPacket->wCount,lpPacket->wStatus,(cCarrotShopCategoryInfo *)&lpPacket->data);
					break;
				}


			case dWORLDtoSERVERS_RENEW_ITEM_EXPIRE_TIME_LIST	:
				{
					WORLDtoSERVERS_RENEW_ITEM_EXPIRE_TIME_LIST	*lpPakcet	=	(WORLDtoSERVERS_RENEW_ITEM_EXPIRE_TIME_LIST	*)recvpacket;
					l_lpServerTool->receiveList(lpPakcet->aList,lpPakcet->wCount,lpPakcet->wFullCount,lpPakcet->wIsLast);
					break;
				}

			case dWORLDtoSERVERS_SENDINFOTOADMIN:
				{
					WORLDtoSERVERS_SENDINFOTOADMIN	* info;
					info = (WORLDtoSERVERS_SENDINFOTOADMIN *)recvpacket;

					sList[worldidx].wMaxAllUserCount = (WORD)info->wMaxUserCount;
					break;
				}
			case dWORLDtoSERVERS_ISCONNECT_USER:
				{
					WORLDtoSERVERS_ISCONNECT_USER	* userinfo;
					userinfo = (WORLDtoSERVERS_ISCONNECT_USER *)recvpacket;

					char	msg[512];
					if(strcmp(userinfo->strName,"EMPTY")==0)
						sprintf(msg,g_strToolMessage[1],userinfo->strId);
					else
						sprintf(msg,g_strToolMessage[2],userinfo->strId,userinfo->strName,userinfo->wfieldUniqueIndex);
					MessageBox(NULL,msg,g_strToolMessage[3],MB_OK);
					break;
				}
			case dWORLDtoSERVERS_GUILD_BATTLE_SCHEDULE:
				l_pageTentativeGuild->recvGuildData(recvpacket);
				break;
			case dWORLDtoSERVERS_GET_FIXCHECK:
				{
					WORLDtoSERVERS_GET_FIXCHECK		* rPacket;
					rPacket = (WORLDtoSERVERS_GET_FIXCHECK	*)recvpacket;

					char	strTmp[1024];

					sprintf(strTmp,g_strToolMessage[4],sList[worldidx].strName,g_strWeekName[rPacket->wWeek],rPacket->wHour,rPacket->wMin,rPacket->strNoticeMsg);
					MessageBox(NULL,strTmp,"The fixed check time",MB_OK);
					break;
				}
			case dWORLDtoSERVERS_GET_GUILDINFO:
				{
					WORLDtoSERVERS_GET_GUILDINFO	* rPacket;
					rPacket = (WORLDtoSERVERS_GET_GUILDINFO *)recvpacket;

					l_lpGuildInfo->receiveGuildInfo(rPacket->wGuildSerial,rPacket->strGuildName,&rPacket->info);
					break;
				}
			case dWORLDtoSERVERS_USER_GUILDINFO	:
				{
					WORLDtoSERVERS_USER_GUILDINFO	* rPacket;
					rPacket = (WORLDtoSERVERS_USER_GUILDINFO *)recvpacket;

					l_lpGuildInfo->receiveGuildMemberInfo(rPacket->wGuildSerial,rPacket->wAllCount,rPacket->wListCount,rPacket->List);
					break;
				}

			case	dWORLDtoSERVERS_GUILD_NAME_LIST		:
				{
					WORLDtoSERVERS_GUILD_NAME_LIST	* rPacket;
					rPacket = (WORLDtoSERVERS_GUILD_NAME_LIST *)recvpacket;

					l_lpGuildInfo->receiveGuildNameList(rPacket->bf1IsBegin,rPacket->bf1IsLast,rPacket->bf14Count,rPacket->strGuildNameList,
														rPacket->iServerIndex,rPacket->iLastGuild);
					break;
				}

/*			case	dWORLDtoSERVERS_GUILD_NAME		:
				{
					WORLDtoSERVERS_GUILD_NAME	* rPacket;
					rPacket = (WORLDtoSERVERS_GUILD_NAME	*)recvpacket;

					g_lpCharacterInfo->receiveGuildName(rPacket->wGuildSerial,rPacket->strGuildName);
					break;
				}
*/
			case	dWORLDtoSERVERS_CHANGE_GUILD_BATTLE_REPORT_RESULT	:
				{
					WORLDtoSERVERS_CHANGE_GUILD_BATTLE_REPORT_RESULT	* rPacket;
					rPacket = (WORLDtoSERVERS_CHANGE_GUILD_BATTLE_REPORT_RESULT	*)recvpacket;

					if (rPacket->wResult	==	0)
						cMSG::Put("result","success!!");
					if (rPacket->wResult	==	1)
						cMSG::Error("result","can not find guild!!");
					if (rPacket->wResult	==	2	||	rPacket->wResult	==	3)
						cMSG::Error("result","mismatch guild info!!");

					break;
				}

			case	dWORLDtoSERVERS_SOUND_OF_LEADERS_BELL	:
				{
					WORLDtoSERVERS_SOUND_OF_LEADERS_BELL	*rPacket	=	(WORLDtoSERVERS_SOUND_OF_LEADERS_BELL	*)recvpacket;

#ifndef	_FOR_KOREA
					char	*strDisplay[]	=	{"hide","show"};

					cMSG::Put("event info","correct exp : %d%%\ncorrect item drop chance : %d%%\nupkeep time : %.2dminute %.2second\ndisplay name : %s\nlast event time : %d/%d/%d %.2d:%.2d\ncool time : %.2d:%.2d\n\ncaster : %s\nmessage : %s",
							rPacket->iBoostExp,rPacket->iBoostItemDropChance,rPacket->iUpkeepTime/60,rPacket->iUpkeepTime%60,strDisplay[rPacket->wDisplayName],
							rPacket->wBeginYear,rPacket->wBeginMonth,rPacket->wBeginDay,rPacket->wBeginHour,rPacket->wBeginMinute,
							rPacket->wCoolTimeMinute/60,rPacket->wCoolTimeMinute%60,rPacket->strCaster,rPacket->strMessage);
#else
					char	*strDisplay[]	=	{"숨김","보임"};

					cMSG::Put("인도자의 종소리 정보","경험치 보정 : %d%%\n아이템 드랍 확률 보정 : %d%%\n이벤트 지속 시간 : %.2d분 %.2d초\n사용자 이름 : %s\n최근 사용 시간 : %d년 %d월 %d일 %.2d시 %.2d분\n재 사용 쿨타임 : %.2d시간 %.2d분\n\n사용자 : %s\n메시지 : %s",
							rPacket->iBoostExp,rPacket->iBoostItemDropChance,rPacket->iUpkeepTime/60,rPacket->iUpkeepTime%60,strDisplay[rPacket->wDisplayName],
							rPacket->wBeginYear,rPacket->wBeginMonth,rPacket->wBeginDay,rPacket->wBeginHour,rPacket->wBeginMinute,
							rPacket->wCoolTimeMinute/60,rPacket->wCoolTimeMinute%60,rPacket->strCaster,rPacket->strMessage);
#endif
					break;
				}

			case	dWORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE_RESULT	:
				{
					WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE_RESULT	* rPacket;
					rPacket = (WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE_RESULT	*)recvpacket;

					if (rPacket->wResult	==	0)
						cMSG::Put("result","success!!");
					if (rPacket->wResult	==	1)
						cMSG::Error("result","can not find guild!!");

					break;
				}
			case dWORLDtoSERVERS_ASK_ITEM_PACK_LIST_TEST:
				{
					WORLDtoSERVERS_ASK_ITEM_PACK_LIST_TEST	*	rPacket;
					rPacket = (WORLDtoSERVERS_ASK_ITEM_PACK_LIST_TEST *)recvpacket;

					FILE * fp = fopen("test.txt.","a+");
					if(fp)
					{
						fprintf(fp,rPacket->data);
						fprintf(fp,"\n");
						fclose(fp);
					}
					break;
				}

			case	dWORLDtoSERVERS_LOTTO_EVENT_DATA	:
				{
					g_lpLottoEventDlg->receiveLottoEventData((WORLDtoSERVERS_LOTTO_EVENT_DATA *)recvpacket);
					break;
				}
			case	dWORLDtoSERVERS_EVENT_TIME_DATA:
				{
					SERVERStoWORLD_MODIFY_EVENT_TIME* packet = (SERVERStoWORLD_MODIFY_EVENT_TIME *)recvpacket;
					if(!packet)
						break;
					g_lpEventTimeManagerDlg->receiveEventTimeData(packet);

					break;
				}
			default:
				{
					break;
				}
			}
		}
		Sleep(1);
	}
	_log("LOSE CONNECTION : world server : %d , %s",worldidx,sList[worldidx].strIP);
	sList[worldidx].isConnect = FALSE;	
	sList[worldidx].worldsock.kill();
}

// void GameCtlThread(void *tmp)
// {
// 	int	value = (int )tmp;
// 	WORD worldidx=0,gameidx=0;
// 	worldidx= (WORD)value/100;
// 	gameidx = (WORD)value%100;
// 	int time= timeGetTime();
// 
// 	_log("try connect to game server : %d, %d, %s",worldidx,gameidx,sList[worldidx].cGS[gameidx].strGameIP);
// 	if(!sList[worldidx].cGS[gameidx].cSock.init(dGAMESERVER_PORT,sList[worldidx].cGS[gameidx].strGameIP,sizeof(cMSG_BASE_TYPE)))
// 	{	_log("FAIL connect to game server : %d, %d, %s",worldidx,gameidx,sList[worldidx].cGS[gameidx].strGameIP);
// 		sList[worldidx].cGS[gameidx].isConnect = FALSE;	
// 		return;	
// 	}
// 	sList[worldidx].cGS[gameidx].isConnect = TRUE;
// 	CG_CONNECT_GAME		cLogin;
// 	cLogin.base.set(sizeof(CG_CONNECT_GAME),dCG_CONNECT_GAME);
// 	strcpy(cLogin.strId,"#admin");
// 	cLogin.wType = 8;	//	툴 버젼
// 	sList[worldidx].cGS[gameidx].cSock.SendPacket((char *)&cLogin,cLogin.base.wSize);
// 
// 	cMSG_BASE_TYPE		dummy;
// 	dummy.set(sizeof(cMSG_BASE_TYPE),dCM_DUMMY);
// 	WORD	recvsize=0,type=0;
// 	char	recvpacket[dMAX_PACKET_LENGTH];
// 
// 	while(TRUE)
// 	{
// 		if(sList[worldidx].cGS[gameidx].cSock.isLoopRecv==FALSE)
// 		{
// 			_log("LOSE CONNECTION to game server : %d, %d, %s",worldidx,gameidx,sList[worldidx].cGS[gameidx].strGameIP);
// 			sList[worldidx].cGS[gameidx].isConnect = FALSE;	
// 			sList[worldidx].cGS[gameidx].cSock.kill();
// 			break;	
// 		}
// 		
// 		if(abs(timeGetTime()-time)>60000)
// 		{	//	더미를 보내서 잘 살아 있나 검사한다.
// 			time = timeGetTime();
// 			if(!sList[worldidx].cGS[gameidx].cSock.SendPacket((char *)&dummy,dummy.wSize))
// 			{	//	연결이 안돼면 이놈은 잘린거야.. -0-
// //				sList[worldidx].cGS[gameidx].isConnect = FALSE;
// 				_log("LOSE connection 2 to game server : %d , %d, %s",worldidx,gameidx,sList[worldidx].cGS[gameidx].strGameIP);
// 				break;
// 			}
// 		}
// 
// //		memset(recvpacket,0,dMAX_PACKET_LENGTH);
// 		while(sList[worldidx].cGS[gameidx].cSock.GetRecvPacket(recvpacket))
// 		{
// 			recvsize = *(WORD *)recvpacket;
// 			type = *(WORD *)(recvpacket+2);
// 			if(type == dCM_COMPRESS)
// 			{
// 				CM_COMPRESS	comp;
// 
// 				memcpy(&comp,recvpacket,recvsize);
// 
// 				cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);
// 				recvsize = *(WORD *)recvpacket;
// 				type = *(WORD *)(recvpacket+2);
// 			}
// 			switch(type)
// 			{
// 			case dSG_SENDINFOTOADMIN:
// 				{
// 					SG_SENDINFOTOADMIN	* info;
// 					info = (SG_SENDINFOTOADMIN *)recvpacket;
// 				
// 					sList[worldidx].cGS[gameidx].dwUserCount = info->wUserCount;
// 					sList[worldidx].cGS[gameidx].dwMaxUserCount = info->wMaxUserCount;
// 
// 					sList[worldidx].cGS[gameidx].timer = timeGetTime();
// 
// 					sList[worldidx].wAllUserCount = 0;
// 					for(int j=0;j<sList[worldidx].wGameServerCount;j++)
// 						sList[worldidx].wAllUserCount += (WORD)sList[worldidx].cGS[j].dwUserCount;
// 					break;
// 				}
// 			default:
// 				{
// 					break;
// 				}
// 			}
// 		}
// 		Sleep(1);
// 	}
// }

BOOL
CAdminToolDlg::disConnectServer(int _serverIndex)
{
	if(sList[_serverIndex].worldsock.sock)
	{
		sList[_serverIndex].worldsock.kill();
		sList[_serverIndex].isConnect = FALSE;
		Sleep(500);
		m_ctlServerTree.SetItemImage( sList[_serverIndex].hTreeItem, 0, 0 );
	}
	return TRUE;
}

BOOL
CAdminToolDlg::tryConnectServer(int _serverIndex)
{
	if(strcmp(sList[_serverIndex].strName,"")==0)
		return FALSE;

	if (g_lWhereisServer	==	eCOUNTRY_INNERTEST	)
	{
		if (sList[_serverIndex].isTestServer	!=	2)
			return FALSE;
	}
	else
	{
		if(sList[_serverIndex].isTestServer	==	2)
			return FALSE;
	}

	_log("try connect to world server : %d , %s",_serverIndex,sList[_serverIndex].strIP);

	/*
	#define dDATABASEPORT					50605
	#define dUSERDBCACHE_PORT				51601
	#define dDBCACHE_PORT					52611
	#define	dWORLDSERVER_PORT				53621
	#define dGAMESERVER_PORT				54631
	#define dLOGINSERVER_PORT				55661
	#define dTOOLLOGSERVER_PORT				57651
	#define dLOGSERVER_PORT					59641
	#define dBROADCAST_PORT					56621
	*/

#ifdef __JPTEST_FOR_KR_INNER
	#undef 	dWORLDSERVER_PORT
	#define	dWORLDSERVER_PORT				53621

	#undef 	dUSERDBCACHE_PORT
	#define	dUSERDBCACHE_PORT				51601

	#undef 	dDBCACHE_PORT
	#define	dDBCACHE_PORT					52611

	#undef 	dGAMESERVER_PORT
	#define	dGAMESERVER_PORT				54631

	#undef 	dBROADCAST_PORT
	#define	dBROADCAST_PORT					56621

	#undef 	dDATABASEPORT
	#define	dDATABASEPORT					50605
#endif

	if(!sList[_serverIndex].worldsock.init(dWORLDSERVER_PORT,sList[_serverIndex].strIP,sList[_serverIndex].strIP,sizeof(cMSG_BASE_TYPE_FORWORLD)))
	{
		_log("ERROR -> connect to world server : %d , %s",_serverIndex,sList[_serverIndex].strIP);
		sList[_serverIndex].isConnect = FALSE;
		return FALSE; 	
	}

	sList[_serverIndex].isConnect = TRUE;
	sList[_serverIndex].hThread = (HANDLE)_beginthread(cThreadFunc::WorldCtlThread, 0 ,(void*)_serverIndex);
	m_ctlServerTree.SetItemImage( sList[_serverIndex].hTreeItem, 1, 1 );

	m_ctlConnect.SetWindowText("Disconnect..");

	return TRUE;
}

void CAdminToolDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	//	게임 서버와 월드 서버의 정보를 가져온다.
	if (g_lWhereisServer	==	eCOUNTRY_INNERTEST	)	return;
	try{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = g_DBSrvInfo.GetConnection();
		pCmd->CommandText = L"spSI_GETSERVERINFO";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); pRs.Release();	pRs = NULL;	return ;	}
		DEF_PARAM(pCmd,"@WORLDIDX") = _variant_t((long)0);
		pRs = pCmd->Execute(NULL,NULL,NULL);

		_variant_t v_varient;
		char	strDate[64];
		while(!pRs->adoEOF)
		{
			v_varient = pRs->Fields->Item[_variant_t("worldidx")]->Value;
			int sidx = (WORD)v_varient.intVal-1;
			v_varient = pRs->Fields->Item[_variant_t("idx")]->Value;
			int widx = (WORD)v_varient.intVal;
			int gidx = widx %100-1;
			if(gidx == -1)	//	월드 서버라면..
			{
				v_varient = pRs->Fields->Item[_variant_t("ip")]->Value;
//				strcpy(sList[sidx].strIP,(_bstr_t)v_varient);
				v_varient = pRs->Fields->Item[_variant_t("curusercount")]->Value;
				sList[sidx].wGameServerCount = (WORD)v_varient.intVal;
				v_varient = pRs->Fields->Item[_variant_t("curtime")]->Value;
				strcpy(strDate,(_bstr_t)v_varient);
				g_DBAccount.getTimeToUTime(strDate,&sList[sidx].time);
			}else
			{
				v_varient = pRs->Fields->Item[_variant_t("ip")]->Value;
				strcpy(sList[sidx].cGS[gidx].strGameIP,(_bstr_t)v_varient);
				v_varient = pRs->Fields->Item[_variant_t("curusercount")]->Value;
				sList[sidx].cGS[gidx].dwUserCount = (WORD)v_varient.intVal;
				v_varient = pRs->Fields->Item[_variant_t("curtime")]->Value;
				strcpy(strDate,(_bstr_t)v_varient);
				g_DBAccount.getTimeToUTime(strDate,&sList[sidx].cGS[gidx].time);
			}
			pRs->MoveNext();
		}
		VariantClear(&v_varient);
		pCmd.Release(); 
		pRs->Close();	pRs.Release();	pRs = NULL;
	}
	catch( _com_error &e)
	{
		return ;
	}

	//	툴 로그 서버와의 통신이 지속되는지를 확인한다.
#ifndef _FOR_INTER
	if(g_bIsRequireSendToolLog && logSock.sock == NULL)
	{	//	툴을 종료한다.
		SendMessage(WM_CLOSE);
	}
#endif
	CDialog::OnTimer(nIDEvent);
}

void CAdminToolDlg::OnActivate(UINT _uiMessage,CWnd *_lpWnd, BOOL _bIsActive)
{
}

void CAdminToolDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	for(int i=0;i<dMAX_WORLD_SERVER_COUNT;i++)
	{
		if	(sList[i].isConnect)
			sList[i].worldsock.isLoopRecv = FALSE;

		Sleep(10);
	}

	if	(m_pCtlImageList)
		delete m_pCtlImageList;
}

void CAdminToolDlg::OnSelchangedServerTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString tmp; 
	HTREEITEM	nitem = NULL;
	nitem = m_ctlServerTree.GetSelectedItem();
	tmp = m_ctlServerTree.GetItemText(nitem);
	if(tmp.GetLength()>0)
	{
		char	nameex[128];
		for(int i=0;i<dMAX_WORLD_SERVER_COUNT;i++)
		{
			sprintf(nameex,"%s(%s)",sList[i].strName,sList[i].strIP);
			if(strcmp(sList[i].strName,"")==0)	continue;
			if(strcmp(nameex,tmp.operator const char*())==0)
			{	g_iSelectedServer = i;
				if(sList[g_iSelectedServer].worldsock.sock != NULL)
				{
					m_ctlConnect.SetWindowText("Disconnect..");
				}else
				{
					m_ctlConnect.SetWindowText("Connect..");
				}
				break;
			}
		}
	}else
	{
		g_iSelectedServer = -1;
	}
	*pResult = 0;
}

BOOL CAdminToolDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==13)
		pMsg->wParam = 0;

	return CDialog::PreTranslateMessage(pMsg);
}

void CAdminToolDlg::connectWorld(int _iWorld) 
{
	// TODO: Add your control notification handler code here
	if(extLevel>eAL_NORMAL)
	{
		if(_iWorld == -1)
		{
			if(MessageBox("Try Connect to All server",g_strToolMessage[5],MB_YESNO)==IDNO)
				return;
			for(int worldidx = 0;worldidx< dMAX_WORLD_SERVER_COUNT; worldidx++)
			{
				if(strcmp(sList[worldidx].strName,"")==0)	continue;
//				if(g_lWhereisServer != eCOUNTRY_INNERTEST)
				if(strstr(sList[worldidx].strName,"내부"))	continue;

				if(!sList[worldidx].worldsock.sock)
				{
					BOOL	ret = tryConnectServer(worldidx);
					if(!ret)
					{
						MessageBox("Fail to connect to the server","ERROR!!",MB_OK);
					}else
					{
						m_ctlConnect.SetWindowText("Disconnect..");
					}
				}else
				{	//	접속 종료하기
					disConnectServer(worldidx);
					m_ctlConnect.SetWindowText("Connect..");
				}
			}
		}else
		{
			if(!sList[_iWorld].worldsock.sock)
			{
				if(MessageBox("Try Connect to the server..",g_strToolMessage[5],MB_YESNO)==IDNO)
					return;
				BOOL	ret = tryConnectServer(_iWorld);
				if(!ret)
				{
					MessageBox("Fail to connect to the server","ERROR!!",MB_OK);
				}else
				{
					m_ctlConnect.SetWindowText("Disconnect..");
				}
			}else
			{	//	접속 종료하기
				disConnectServer(_iWorld);
				m_ctlConnect.SetWindowText("Connect..");
			}
		}
	}
}

void CAdminToolDlg::OnConnect() 
{
	connectWorld(g_iSelectedServer) ;
}

void CAdminToolDlg::OnInfoUpdate() 
{
	int	iTotalCount	=	0;

	// TODO: Add your control notification handler code here
	for(int worldidx = 0;worldidx< dMAX_WORLD_SERVER_COUNT; worldidx++)
	{
		if	(strcmp(sList[worldidx].strName,"")==0)
			continue;
		if	(strstr(sList[worldidx].strName,"내부"))
			continue;

		char	nameex[512];
		sprintf(nameex,"%s(%s)",sList[worldidx].strName,sList[worldidx].strIP);
		ptTree->DeleteItem(sList[worldidx].hTreeItem);
		sList[worldidx].tvStruct.hParent = 0;
		sList[worldidx].tvStruct.hInsertAfter = TVI_LAST;
		sList[worldidx].tvStruct.item.pszText=nameex;
		sList[worldidx].tvStruct.item.iImage = sList[worldidx].isConnect;
		sList[worldidx].tvStruct.item.iSelectedImage = sList[worldidx].isConnect;
		sList[worldidx].tvStruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		sList[worldidx].hTreeItem = ptTree->InsertItem(&sList[worldidx].tvStruct);

		char	txt[512];
		TV_INSERTSTRUCT	tvStruct;

		sList[worldidx].wAllUserCount = 0;
		//	게임서버 리스트...
		for(int j=0;j<sList[worldidx].wGameServerCount;j++)
		{
			char	gList[512];
			sprintf(gList,"%s(%d)",sList[worldidx].cGS[j].strGameIP,sList[worldidx].cGS[j].dwUserCount);
			tvStruct.item.pszText=gList;
			tvStruct.hParent = sList[worldidx].hTreeItem;
			tvStruct.hInsertAfter = TVI_LAST;

			//	얻어온 시간을 비교해서 표시한다.
			tvStruct.item.iImage = 1;
			tvStruct.item.iSelectedImage = 1;

			tvStruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
			ptTree->InsertItem(&tvStruct);
			sList[worldidx].wAllUserCount+=sList[worldidx].cGS[j].dwUserCount;
			iTotalCount				+=	sList[worldidx].cGS[j].dwUserCount;
		}

		sprintf(txt,"User Count : %d",sList[worldidx].wAllUserCount);
		tvStruct.hParent = sList[worldidx].hTreeItem;
		tvStruct.hInsertAfter = TVI_LAST;
		tvStruct.item.pszText=txt;
		tvStruct.item.iImage = 2;
		tvStruct.item.iSelectedImage = 2;
		tvStruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		ptTree->InsertItem(&tvStruct);

		sprintf(txt,"Game Server Count : %d  ",sList[worldidx].wGameServerCount);
		tvStruct.item.pszText=txt;
		ptTree->InsertItem(&tvStruct);

		ptTree->SetItemState(sList[worldidx].hTreeItem,TVIS_EXPANDED,TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT);
	}

	UpdateData(FALSE);

	cMSG::Put("Total connect count","%d",iTotalCount);
}

void CAdminToolDlg::OnRclickServerTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	g_iSelectedServer = -1;

	HTREEITEM	nitem = NULL;
	nitem = m_ctlServerTree.GetSelectedItem();
	m_ctlServerTree.SetItemState(nitem,0,TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT);
	
	*pResult = 0;
}

void CAdminToolDlg::OnClickServerTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString tmp; 
	HTREEITEM	nitem = NULL;
	nitem = m_ctlServerTree.GetSelectedItem();
	tmp = m_ctlServerTree.GetItemText(nitem);
	if(tmp.GetLength()>0)
	{
		char	nameex[128];
		for(int i=0;i<dMAX_WORLD_SERVER_COUNT;i++)
		{
			sprintf(nameex,"%s(%s)",sList[i].strName,sList[i].strIP);
			if(strcmp(sList[i].strName,"")==0)	continue;
			if(strcmp(nameex,tmp.operator const char*())==0)
			{	g_iSelectedServer = i;
				if(sList[g_iSelectedServer].worldsock.sock != NULL)
				{
					m_ctlConnect.SetWindowText("Disconnect..");
				}else
				{
					m_ctlConnect.SetWindowText("Connect..");
				}
				break;
			}
		}
	}else
	{
		g_iSelectedServer = -1;
	}

	*pResult = 0;
}
