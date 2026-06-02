// WorldScore.cpp : implementation file
//

#include "stdafx.h"
#include "AdminTool.h"
#include "WorldScore.h"
#include <io.h>
#include "cSRVUTIL.h"
#include "MSGDLG.h"
#include "AGEGRAPH.h"
#include "CMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldScore property page

BOOL				g_isViewSum;

IMPLEMENT_DYNCREATE(CWorldScore, CPropertyPage)

CWorldScore::CWorldScore() : CPropertyPage(CWorldScore::IDD)
{
	//{{AFX_DATA_INIT(CWorldScore)
	//}}AFX_DATA_INIT
	m_iSelectedServerIdx = m_iCurDay = 0;
	memset(&AC,0,sizeof(int)*24*dAGE_DEFIEN_COUNT);
	memset(&PC,0,sizeof(int)*24*dAGE_DEFIEN_COUNT);
	g_isViewSum = FALSE;
	m_bRealTime = FALSE;
}

CWorldScore::~CWorldScore()
{
}

void CWorldScore::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorldScore)
	DDX_Control(pDX, IDC_USERCOUNTBYDAY, m_ctlTotalConnectedUserCount);
	DDX_Control(pDX, IDC_GRAPH, m_ctlGraph);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ctlServerList);
	DDX_Control(pDX, IDC_SUM_TEXT, m_ctlSUM_TEXT);
	DDX_Control(pDX, IDC_SCORE_BYTIME, m_ctlScoreByTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWorldScore, CPropertyPage)
	//{{AFX_MSG_MAP(CWorldScore)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR1, OnSelchangeMonthcalendar1)
	ON_CBN_SELCHANGE(IDC_SERVER_LIST, OnSelchangeServerList)
	ON_BN_CLICKED(IDC_GRAPH, OnGraph)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SUM_TEXT, OnSumText)
	ON_BN_CLICKED(IDC_EXCEL, OnExcel)
	ON_BN_CLICKED(IDC_VIEW_REALTIME, OnRealTimeView)
	//}}AFX_DATA_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldScore message handlers

void CWorldScore::OnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 
{
#ifdef	_FOR_CHINA
	return ;
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	return ;
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	static int reHour = cSRVUTIL::GetHour()-1;	if(reHour<0)	reHour = 0;
	// TODO: Add your control notification handler code here
	NMDAYSTATE* pNMDay = (NMDAYSTATE*)pNMHDR;

	SYSTEMTIME	time;
	CWnd * wnd = this->GetDlgItem(IDC_MONTHCALENDAR1);
	MonthCal_GetCurSel(wnd->m_hWnd,&time);	//	현재 선택한 일자를 얻어온다.
	int t = (cSRVUTIL::GetYear()%100)+2000;
	t=cSRVUTIL::GetMonth()+1;
	t=cSRVUTIL::GetDay();
	if(m_iCurDay==time.wDay){	*pResult = 0;	return;	}
	m_iCurDay=time.wDay;

	if(time.wYear>(cSRVUTIL::GetYear()%100)+2000){	*pResult = 0;	return;	}
	if((time.wMonth>=(cSRVUTIL::GetMonth()+1) && time.wDay>cSRVUTIL::GetDay())&& time.wYear>=(cSRVUTIL::GetYear()%100)+2000){	*pResult = 0;	return;	}
	
	char	ScoreDir[1024];
	char	dir[1100];
	sprintf(ScoreDir,"%s\\Score",strCurPath);
	CreateDirectory(ScoreDir,NULL);

	CMSGDLG		msgDLG;		char msgStr[1024];
	int iMaxProgress = g_iWorldCount*50+50+100;	//	마지막 100은 DB에서 얻어오는 각종 통계자료 
	int iCurProgress = 0;
	int	iCurServer = 0;
	msgDLG.Create(IDD_MSG_DIALOG);	
	msgDLG.m_ctlProgress.SetRange(0,iMaxProgress);
	msgDLG.ShowWindow(SW_SHOWNORMAL);

	int iServer = 0;
	for(iServer = 0;iServer<dMAX_WORLD_SERVER_COUNT;iServer++)
	{
		if( strcmp(sList[iServer].strName,"")==0 || strstr(sList[iServer].strName,"내부"))	continue;
		_log("Download log file : [%d][%s]",iServer,sList[iServer].strName);
		DWORD curDate = time.wYear*10000+time.wMonth*100+time.wDay;
		if(sList[iServer].startdate>curDate)
		{	char strs[512];
			sprintf(strs,g_strToolMessage[81],sList[iServer].strName,sList[iServer].startdate);
			MessageBox(strs);
#ifdef _FOR_KOREA
			sList[iServer].wAllYahooUserCount = 0;
			sList[iServer].wMaxAllYahooUserCount = 0;
			sList[iServer].wYahooAvr = 0;
			sList[iServer].wYahooTop = 0;
			memset(sList[iServer].wYahooUserCount,0,sizeof(WORD)*dMAX_TIME_COUNT);

			sList[iServer].wAllParanUserCount = 0;
			sList[iServer].wMaxAllParanUserCount = 0;
			sList[iServer].wParanAvr = 0;
			sList[iServer].wParanTop = 0;
			memset(sList[iServer].wParanUserCount,0,sizeof(WORD)*dMAX_TIME_COUNT);
#endif
			sList[iServer].wAllUserCount = 0;
			sList[iServer].wAvr = 0;
			sList[iServer].wMaxAllUserCount = 0;
			sList[iServer].wTop = 0;
			memset(sList[iServer].wUserCount,0,sizeof(WORD)*dMAX_TIME_COUNT);
			continue;
		}

REDOWNLOAD:
		iCurProgress = 50*iCurServer;
		msgDLG.m_ctlProgress.SetPos(iCurProgress);
		iCurServer++;
		SetCurrentDirectory(ScoreDir);
		//	MSG를 보여준다. 작업이 끝나면 없어짐.
		sprintf(msgStr,g_strToolMessage[82],sList[iServer].strName,time.wMonth,time.wDay,time.wYear);
		msgDLG.m_ctlMsg.SetWindowText(msgStr);	Sleep(200);
		sprintf(dir,"%s\\Score\\%s",strCurPath,sList[iServer].strName);
		CreateDirectory(dir,NULL);

		char	fn[1024];
		sprintf(fn,"%s\\%04d%02d%02d_Score.txt",dir,time.wYear,time.wMonth,time.wDay);
		FILE * scorefp;
		if( (scorefp = fopen(fn,"r")) )
		{
			fclose(scorefp); scorefp = NULL;
			DeleteFile(fn);
		}
		{	//	파일을 다운 받는다.
			sprintf(msgStr,g_strToolMessage[83],sList[iServer].strName,time.wMonth,time.wDay,time.wYear);
			msgDLG.m_ctlMsg.SetWindowText(msgStr);	Sleep(200);

			nsFTP::CLogonInfo loginInfo(g_strLogDataFtpIp, g_iLogDataFtpPort, strLogFtpId, strLogFtpPw);
			if(ftp.Login(loginInfo))
			{
				char sfn[2049];
//#ifdef _FOR_KOREA
				sprintf(sfn,"%s/%04d%02d%02d_Score.log",sList[iServer].strName,time.wYear,time.wMonth,time.wDay);
//				sprintf(sfn,"%04d%02d%02d_Score.txt",time.wYear,time.wMonth,time.wDay);
//#else
//				sprintf(sfn,"%04d%02d%02d_Score.txt",time.wYear,time.wMonth,time.wDay);
//#endif
				_log("Download log file : [%s]->[%s]",sfn,fn);
				ftp.DownloadFile(sfn,fn);
				ftp.Logout();
				if( !(scorefp = fopen(fn,"r")) )
				{
					sprintf(msgStr,g_strToolMessage[84],sList[iServer].strName,time.wMonth,time.wDay,time.wYear);
					msgDLG.m_ctlMsg.SetWindowText(msgStr);	Sleep(200);
					continue;
				}
			}else{
				sprintf(msgStr,g_strToolMessage[85],sList[iServer].strName,time.wMonth,time.wDay,time.wYear);
				msgDLG.m_ctlMsg.SetWindowText(msgStr);	Sleep(200);
				continue;
			}
		}
		iCurProgress+=15;
		msgDLG.m_ctlProgress.SetPos(iCurProgress);
		scp.Read(fn,288);
		sprintf(msgStr,g_strToolMessage[86],sList[iServer].strName,time.wMonth,time.wDay,time.wYear);
		msgDLG.m_ctlMsg.SetWindowText(msgStr);	Sleep(200);
		iCurProgress+=5;
		msgDLG.m_ctlProgress.SetPos(iCurProgress);
		if( (reHour!=cSRVUTIL::GetHour() && (filelength( fileno(scorefp) ) == 0)) || (time.wDay != cSRVUTIL::GetDay() && atoi(scp.getData("T23"))==0xffff) )
		{
			static int	isContinue = 0;
			reHour=cSRVUTIL::GetHour();
			fclose(scorefp);
			DeleteFile(fn);
			if(isContinue)
			{
				sprintf(msgStr,g_strToolMessage[87],sList[iServer].strName,time.wMonth,time.wDay,time.wYear);
				msgDLG.m_ctlMsg.SetWindowText(msgStr);	Sleep(500);
			}else{
				isContinue = 1;
				sprintf(msgStr,g_strToolMessage[88],sList[iServer].strName,time.wMonth,time.wDay,time.wYear);
				msgDLG.m_ctlMsg.SetWindowText(msgStr);	Sleep(500);
				--iCurServer;
				goto REDOWNLOAD;
			}
		}
		//	통계작업에 30
		sList[iServer].CountReset();

		int zeroTime=0;
		int iHour = 0;
		int iMin = 0;
		for(int i=0;i<dMAX_TIME_COUNT;i++)
		{
			char value[10];
			//	read
			sprintf(value,"T%02d_%02d",iHour,iMin);
			sList[iServer].wUserCount[i] = atoi(scp.getData(value));
#ifdef _FOR_KOREA
			sList[iServer].wYahooUserCount[i] = atoi(scp.getData(value,2));
			sList[iServer].wParanUserCount[i] = atoi(scp.getData(value,3));
#endif

			// avr
			if(sList[iServer].wUserCount[i]!=0xffff)		sList[iServer].wAvr+=sList[iServer].wUserCount[i];
#ifdef _FOR_KOREA
			if(sList[iServer].wYahooUserCount[i]!=0xffff)	sList[iServer].wYahooAvr+=sList[iServer].wYahooUserCount[i];
			if(sList[iServer].wParanUserCount[i]!=0xffff)	sList[iServer].wParanAvr+=sList[iServer].wParanUserCount[i];
#endif
			if(sList[iServer].wUserCount[i]!=0xffff && sList[iServer].wTop < sList[iServer].wUserCount[i])
			{	sList[iServer].wTop = sList[iServer].wUserCount[i];
			}

			//	top
#ifdef _FOR_KOREA
			if(sList[iServer].wYahooUserCount[i]!=0xffff && sList[iServer].wYahooTop < sList[iServer].wYahooUserCount[i])
			{	sList[iServer].wYahooTop = sList[iServer].wYahooUserCount[i];
			}
			if(sList[iServer].wParanUserCount[i]!=0xffff && sList[iServer].wParanTop < sList[iServer].wParanUserCount[i])
			{	sList[iServer].wParanTop = sList[iServer].wParanUserCount[i];
			}
#endif
			WORD checkvalue = (WORD)atoi(scp.getData(value));
			if(checkvalue==0xffff || checkvalue==0)
				++zeroTime;
			iMin += 5;
			if(iMin>=60)
			{	iMin = 0;
				++iHour;
			}
			iCurProgress+=1;
			msgDLG.m_ctlProgress.SetPos(iCurProgress);
		}
		if(zeroTime==dMAX_TIME_COUNT)
			sList[iServer].wAvr = 0;
		else			
			sList[iServer].wAvr /= (dMAX_TIME_COUNT-zeroTime);
#ifdef _FOR_KOREA
		if(zeroTime==dMAX_TIME_COUNT)
		{
			sList[iServer].wYahooAvr = 0;
			sList[iServer].wParanAvr = 0;
		}else			
		{
			sList[iServer].wYahooAvr /= (dMAX_TIME_COUNT-zeroTime);
			sList[iServer].wParanAvr /= (dMAX_TIME_COUNT-zeroTime);
		}
#endif

		fclose(scorefp);
	}
	msgDLG.m_ctlProgress.SetPos(50*iCurServer);
	//	나머지 50을 하루 전체 통계에 사용한다.
	{
		int iLiveServer[dMAX_WORLD_SERVER_COUNT];
		int	iCountByHour[dMAX_TIME_COUNT],iYahooCountByHour[dMAX_TIME_COUNT],iParanCountByHour[dMAX_TIME_COUNT];
		memset(iLiveServer,0,sizeof(int)*dMAX_WORLD_SERVER_COUNT);
		memset(iCountByHour,0,sizeof(int)*dMAX_TIME_COUNT);
		memset(iYahooCountByHour,0,sizeof(int)*dMAX_TIME_COUNT);
		memset(iParanCountByHour,0,sizeof(int)*dMAX_TIME_COUNT);
		int iUsedServerCount = 0;	//	사용한 서버 갯수
		int iMaxUserCount=0,iMaxUserHour=0;		int dlCount = 0;
		int iMaxYahooUserCount=0,iMaxYahooUserHour=0;
		int iMaxParanUserCount=0,iMaxParanUserHour=0;
		for(iServer = 0;iServer<dMAX_WORLD_SERVER_COUNT;iServer++)
		{
			if( strcmp(sList[iServer].strName,"")==0)	continue;
			if(g_lWhereisServer == eCOUNTRY_KOREA && iServer>6)	continue;
			iLiveServer[iUsedServerCount] = iServer;
			iUsedServerCount++;
			for(int iHour = 0;iHour<dMAX_TIME_COUNT;iHour++)
			{
				if(sList[iServer].wUserCount[iHour]!=0xffff)
				{
					iCountByHour[iHour]+=sList[iServer].wUserCount[iHour];
#ifdef _FOR_KOREA
					iYahooCountByHour[iHour]+=sList[iServer].wYahooUserCount[iHour];
					iParanCountByHour[iHour]+=sList[iServer].wParanUserCount[iHour];
#endif
				}
			}
			//	레벨 분포 표를 얻어온다. (당일 or 테스트 서버는 할 필요 없음) - 파일에 저장까지만.. 
			//	당일 파일이 있는지 검사하고
			char	fn[1024];
			sprintf(fn,"%s\\Score\\%s\\%04d%02d%02d_LevelScore.txt",strCurPath,sList[iServer].strName,time.wYear,time.wMonth,time.wDay);
			FILE * levelscorefp;
			if( !(levelscorefp = fopen(fn,"r")) )
			//	없으면 DB에서 읽어와서 저장한다.
			{
				if( (time.wMonth!=cSRVUTIL::GetMonth()+1 || time.wDay!=cSRVUTIL::GetDay()) && !strstr(sList[iServer].strName,"소룬드") )
				{
					int day = time.wYear*10000+time.wMonth*100+time.wDay;
					char	txt[8000];
					memset(txt,0,8000);
					if(DB.Connect(sList[iServer].strDBIP,strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
					{	DB.spGetDistriLevel(day,txt);
						if(strcmp(txt,"")!=0)
						{
							levelscorefp = fopen(fn,"w");
							fprintf(levelscorefp,"%s",txt);
							fclose(levelscorefp);
						}
						DB.Close();
					}
				}
			}
			iCurProgress+=2;	msgDLG.m_ctlProgress.SetPos(iCurProgress);
		}
		msgDLG.m_ctlProgress.SetPos(50*iCurServer+25);
		g_wMaxUserCount = g_wMaxTime = 0;
		g_wMaxYahooUserCount = g_wMaxYahooTime = 0;
		g_wMaxParanUserCount = g_wMaxParanTime = 0;
		for(int iHour = 0;iHour<dMAX_TIME_COUNT;iHour++)
		{	//	최대 동접 수 , 시간 
			if(iCountByHour[iHour]>g_wMaxUserCount)
			{	g_wMaxUserCount = iCountByHour[iHour];
				g_wMaxTime = iHour;
			}
			if(iYahooCountByHour[iHour]>g_wMaxYahooUserCount)
			{	g_wMaxYahooUserCount = iYahooCountByHour[iHour];
				g_wMaxYahooTime = iHour;
			}
			if(iParanCountByHour[iHour]>g_wMaxParanUserCount)
			{	g_wMaxParanUserCount = iParanCountByHour[iHour];
				g_wMaxParanTime = iHour;
			}
		}
		msgDLG.m_ctlProgress.SetPos(50*iCurServer+20);
		char tottxt[2048];
		memset(tottxt,0,2048);
		sprintf(tottxt,g_strToolMessage[89],time.wMonth,time.wDay,time.wYear);
		for(int t = 0;t<iUsedServerCount;t++)	
		{
#ifdef	_FOR_KOREA
			sprintf(tottxt,"%s\n\r [%-15s]서버 \n\r          Max = [%05d]명, Avr = [%05d]명(AC %03d/%03d)\n\r",tottxt,sList[t].strName,sList[t].wTop,sList[t].wAvr,sList[t].wParanTop,sList[t].wParanAvr);
#else
			sprintf(tottxt,g_strToolMessage[90],tottxt,sList[t].strName,sList[t].wTop,sList[t].wAvr);
#endif
		}
#ifdef	_FOR_KOREA
		sprintf(tottxt,"%s\n\r ----------\n\r최대 동시접속자:[%05d]명 [%02d]시 [%02d]분 \n(AC %03d명, [%02d]시 [%02d]분)",tottxt,g_wMaxUserCount,g_wMaxTime/12,g_wMaxTime%12,g_wMaxParanUserCount,g_wMaxParanTime/12,g_wMaxParanTime%12);
#else
		sprintf(tottxt,g_strToolMessage[91],tottxt,g_wMaxUserCount,g_wMaxTime/12);
#endif
		m_ctlSUM_TEXT.SetWindowText(tottxt);
	}
	msgDLG.m_ctlProgress.SetPos(iMaxProgress-100);
	//	db 통계 자료 출력할 곳 초기화 
	m_ctlTotalConnectedUserCount.SetWindowText("0  person(s)");
	msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +10);
	//	DB에서 얻어오는 각종 통계 
	{	
		{	msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +15);
			int day = time.wYear*10000+time.wMonth*100+time.wDay;
			int y_result,p_result,iWeek,iWeek_y,iWeek_p,iMonth,iMonth_y,iMonth_p;
			int result = g_DBAccount.spGetTotalUserCountByDay(day,y_result,p_result,iWeek,iWeek_y,iWeek_p,iMonth,iMonth_y,iMonth_p);
			msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +20);
			char	txt[128];
#ifdef _FOR_KOREA
			sprintf(txt,"\n Today       %06d / (AC) %04d \n Last Week  %06d / (AC) %04d\n Last Month %06d / (AC) %04d ",result,p_result,iWeek,iWeek_p,iMonth,iMonth_p);
#else
			sprintf(txt,"%06d person(s)",result);
#endif
			m_ctlTotalConnectedUserCount.SetWindowText(txt);
			msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +25);
			//	연령별 접속 통계
			memset(&AC,0,sizeof(int)*24*dAGE_DEFIEN_COUNT);
			memset(&PC,0,sizeof(int)*24*dAGE_DEFIEN_COUNT);

#ifdef _FOR_KOREA
			g_DBAccount.spGetAgeConnectCount("AGE13",day,AC[0]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +30);
			g_DBAccount.spGetAgeConnectCount("AGE16",day,AC[1]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +32);
			g_DBAccount.spGetAgeConnectCount("AGE19",day,AC[2]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +35);
			g_DBAccount.spGetAgeConnectCount("AGE22",day,AC[3]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +42);
			g_DBAccount.spGetAgeConnectCount("AGE25",day,AC[4]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +45);
			g_DBAccount.spGetAgeConnectCount("AGE29",day,AC[5]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +42);
			g_DBAccount.spGetAgeConnectCount("AGE35",day,AC[6]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +47);
			g_DBAccount.spGetAgeConnectCount("AGE40",day,AC[7]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +50);

			g_DBAccount.spGetAgePlayTimeCount("AGE13",day,PC[0]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +52);
			g_DBAccount.spGetAgePlayTimeCount("AGE16",day,PC[1]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +54);
			g_DBAccount.spGetAgePlayTimeCount("AGE19",day,PC[2]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +56);
			g_DBAccount.spGetAgePlayTimeCount("AGE22",day,PC[3]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +58);
			g_DBAccount.spGetAgePlayTimeCount("AGE25",day,PC[4]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +60);
			g_DBAccount.spGetAgePlayTimeCount("AGE29",day,PC[5]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +63);
			g_DBAccount.spGetAgePlayTimeCount("AGE35",day,PC[6]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +67);
			g_DBAccount.spGetAgePlayTimeCount("AGE40",day,PC[7]);msgDLG.m_ctlProgress.SetPos(iMaxProgress-100 +70);
#endif
		}
	}	

	{	//	상세 유저 접속 통계를 DB에서 가져오고, 나중에 그래프에 그려주자.
		char	txt[128];
#ifdef _FOR_KOREA
			sprintf(txt,"DB의 통계를 가져오는 중입니다.");
#else
			sprintf(txt,"Downloading some data in SQL DB ");
#endif

		msgDLG.m_ctlMsg.SetWindowText(txt);
		for(iServer = 0;iServer<dMAX_WORLD_SERVER_COUNT;iServer++)
		{
			g_DBSrvInfo.spGetDetailUserCountStat(time.wYear*10000+time.wMonth*100+time.wDay,iServer,&m_vcDetailCC[iServer]);
			msgDLG.m_ctlProgress.SetPos(iMaxProgress- 5*(dMAX_WORLD_SERVER_COUNT - iServer));
			Sleep(1);
		}
	}
	msgDLG.m_ctlProgress.SetPos(iMaxProgress);
	msgDLG.CloseWindow();
	*pResult = 0;
}

BOOL CWorldScore::OnInitDialog() 
{
#ifndef _FOR_KOREA
	SetDlgItemText(IDC_STATIC1,g_strDlgText[145]);
	SetDlgItemText(IDC_STATIC2,g_strDlgText[146]);
	SetDlgItemText(IDC_STATIC3,g_strDlgText[147]);
	SetDlgItemText(IDC_STATIC4,g_strDlgText[148]);
#endif

	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	int iServer = 0;
	for(;iServer<dMAX_WORLD_SERVER_COUNT;iServer++)
	{
		if( strcmp(sList[iServer].strName,"")==0)	continue;
		m_ctlServerList.AddString(sList[iServer].strName);
	}
	m_ctlServerList.SetCurSel(0);

	iconGraph = AfxGetApp()->LoadIcon(IDI_GRAPH);
	m_ctlGraph.SetIcon(iconGraph);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorldScore::OnSelchangeServerList() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_KOREA
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#endif

	// TODO: Add your control notification handler code here
	char strSelectedName[24];
	m_ctlServerList.GetLBText(m_ctlServerList.GetCurSel(),strSelectedName);

	if(strcmp(strSelectedName,"")==0)	return;
	for(int iServer = 0;iServer<dMAX_WORLD_SERVER_COUNT;iServer++)
	{
		if(strcmp(strSelectedName,sList[iServer].strName)==0)
		{
			// 기존을 내용을 싹 지우구.. 
			for(int dc=0;dc<24;dc++)
				m_ctlScoreByTime.DeleteString(0);
			//	선택된 서버의 시간당 접속수를 등록한다. 
			char txt[128];
			for(int t=0;t<24;t++)
			{
				sprintf(txt,g_strToolMessage[92],t,sList[iServer].wUserCount[t]);
				m_ctlScoreByTime.InsertString(t,txt);
			}
			m_ctlScoreByTime.SetCurSel(0);
			break;
		}
	}
	if(iServer==dMAX_WORLD_SERVER_COUNT)
		MessageBox(g_strToolMessage[93]);
}

void CWorldScore::OnGraph() 
{
#ifdef	_FOR_CHINA
	if (!CheckOperatorLevel(eAL_MASTER_OPERATER))	return;
#elif	_FOR_JAPAN
	if (!CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_USA
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#elif	_FOR_INTER
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#else
	if (extLevel != eAL_NORMAL && !CheckOperatorLevel(eAL_OPERATOR))	return;
#endif
	// TODO: Add your control notification handler code here
	if(!m_iCurDay)
		return;
	SYSTEMTIME	time;
	CWnd * wnd = this->GetDlgItem(IDC_MONTHCALENDAR1);
	MonthCal_GetCurSel(wnd->m_hWnd,&time);	//	현재 선택한 일자를 얻어온다.

#ifdef _FOR_KOREA
	CAGEGRAPH		grapDLG;
	grapDLG.Set((int **)&AC,(int **)&PC,time.wYear*10000+time.wMonth*100+time.wDay,m_vcDetailCC);
	grapDLG.DoModal();
#endif
//	msgDLG.Create(IDD_MSG_DIALOG);
//	msgDLG.ShowWindow(SW_SHOWNORMAL);
//	msgDLG.CloseWindow();
}

void CWorldScore::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(g_isViewSum)		g_isViewSum = FALSE;
	else				g_isViewSum = TRUE;
}

void CWorldScore::OnSumText() 
{
	// TODO: Add your control notification handler code here
	HGLOBAL         hMem; 
	
	if ( !OpenClipboard() )
	{
		AfxMessageBox( "Fail to copy the text");
		return;
	}
	if( !EmptyClipboard() )
	{
		AfxMessageBox( "Cannot empty the Clipboard" );
		return;
	}
	hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, 1024 );

    if( hMem )
	{
		LPTSTR pClipData = (LPTSTR) GlobalLock(hMem);
		m_ctlSUM_TEXT.GetWindowText(pClipData,1024);
		if ( ::SetClipboardData( CF_TEXT , hMem ) == NULL )
		{
			AfxMessageBox( "Unable to set Clipboard data" );
			GlobalUnlock( hMem );
			CloseClipboard();
			return;
		}
		GlobalUnlock( hMem );
	}
	
	CloseClipboard();
}

void CWorldScore::OnExcel() 
{
	// TODO: Excel 파일로 모든 통계를 출력하자.
	if(!m_iCurDay)	return;
#ifdef _FOR_KOREA
	char	strName[256]	=	"";
	//	시간대별 접속사 평균..
	SetCurrentDirectory(strCurPath);
	if	(!cFILE::SaveBox(strName,"csv {*.csv}\0*.csv\0\0","csv",strCurPath,"Select save file"))
	{	SetCurrentDirectory(strCurPath);
		return;
	}

	SYSTEMTIME	time;
	CWnd * wnd = this->GetDlgItem(IDC_MONTHCALENDAR1);
	MonthCal_GetCurSel(wnd->m_hWnd,&time);	//	현재 선택한 일자를 얻어온다.

	FILE	*fp	=	fopen(strName,"w");
	if	(fp)
	{
		//	접속자 통계
		for(int iServer = 0;iServer<g_iWorldCount-1;iServer++)
		{
			int iHour=0,iMin=0;
			fprintf(fp,"%d년 %d월 %d일 - 붉은보석 접속자 통계 : %s\n",time.wYear,time.wMonth,time.wDay,sList[iServer].strName);
			fprintf(fp,"=======================================\n");
			fprintf(fp,"시,분,전체 접속자,  Anycall 접속자\n");
			for(int i=0;i<dMAX_TIME_COUNT;i++)
			{
				iMin += 5;
				if(iMin>=60){	++iHour; iMin = 0;	}

				fprintf(fp,"%d,%d,%d,%d\n",iHour,iMin,sList[iServer].wUserCount[i],sList[iServer].wParanUserCount[i]);
			}
			fprintf(fp,"=======================================\n");
			fprintf(fp,"AVERAGE, %d, MAX, %d, Anycall AVERAGE, %d, MAX, %d \n",sList[iServer].wAvr,sList[iServer].wTop,sList[iServer].wParanAvr,sList[iServer].wParanTop);
			fprintf(fp,"=======================================\n\n\n");
		}

		char	fn[1024];

		int iDL[dMAX_WORLD_SERVER_COUNT][200];
		memset(iDL,0,sizeof(int)*dMAX_WORLD_SERVER_COUNT*200);

		char txt[2000];	
		memset(txt,0,2000);

		for(iServer = 0;iServer<g_iWorldCount-1;iServer++)
		{
			if	( strcmp(sList[iServer].strName,"")==0)
				continue;

			sprintf(fn,"%s\\Score\\%s\\%04d%02d%02d_LevelScore.txt",strCurPath,sList[iServer].strName,time.wYear,time.wMonth,time.wDay);
			FILE * levelscorefp;

			if	((levelscorefp = fopen(fn,"r")) )
			{
				memset(txt,0,2000);
				fread(txt,2000,1,levelscorefp);
				fclose(levelscorefp);

				int count = strlen(txt)-2;
				char tmp[10];

				for(int j=0;j<1000/5;j++)
				{
					memset(tmp,0,10);
					int tmpCount = 0;
					
					while(1)
					{
						if	(txt[count]!='|')
						{
							if	(txt[count]=='*')	
								txt[count] = 0;	//	너무 많을 경우 자리수가 모자란다.

							tmp[tmpCount]=txt[count];
							tmpCount++;
							--count;

							if	(count<0)
								break;
						}
						else
						{
							--count;
							break;
						}
					}

					if	(tmp=="")
					{
						printf("-0- BREAK!!");
					}

					if	(count<=0)
						break;

					char	strNumber[32];
					int		iLen	=	strlen(tmp);

					for(int iS=0;iS<iLen;iS++)
						strNumber[iLen-iS-1]	=	tmp[iS];

					strNumber[iLen]	=	NULL;
					iDL[iServer][j] = atoi(strNumber);
				}
			}
		}

		//	레벨 통계
		for(iServer = 0;iServer<g_iWorldCount-1;iServer++)
		{
			fprintf(fp,"%d년 %d월 %d일 - 붉은보석 레벨 통계 : %s\n",time.wYear,time.wMonth,time.wDay,sList[iServer].strName);
			fprintf(fp,"=======================================\n");
			fprintf(fp,"레벨, 인원수\n");
			for(int l=0;l<200;l++)
			{
				fprintf(fp,"%d~%d,%d\n",l*5+1,l*5+5,iDL[iServer][l]);
			}
			fprintf(fp,"=======================================\n\n");
		}
		fclose(fp);
	}
#endif
}

void
CWorldScore::OnRealTimeView()
{
	if(m_bRealTime)		stopRealTime();
	else				startRealTime();
}

void CWorldScore::stopRealTime()
{
	m_bRealTime = false;
	KillTimer(eREALTIME_TIMER_EVENT_ID);
}

void CWorldScore::startRealTime()
{
	m_bRealTime = true;
	SetTimer(eREALTIME_TIMER_EVENT_ID, eREALTIME_TIMER_INTERVAL, NULL);

	//	통계 DB로 접속한다.
	
	//	각 서버의 동접을 가져온다.
	
	//	
}


