#include	"stdafx.h"
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

BOOL	g_bIsDownloadLogTime	=	TRUE;
BOOL	g_bIsWaitingDownloadLog	=	FALSE;

char	g_strAccountDBIP[dIP_LENGTH];//	ACCOUNTDBINFO , 1
char	g_strAccountTableName[128];	//	ACCOUNTDBINFO , 2
char	g_strAccountDBID[32];
char	g_strAccountDBPW[32];

void
DownLoadThread(void *tmp)
{
	DWORD curDate = cSRVUTIL::GetTime() , selDate;
	vector<cADO_AnalyzeTool::stUserStatsPack>		_pVector_UserStats[dMAX_WORLD_SERVER_COUNT];
	DWORD t = cSRVUTIL::GetPrevTime();
	selDate = t;

	if	(g_lpMainDlg->m_iSelectedDate)
	{
		selDate = t = g_lpMainDlg->m_iSelectedDate;
		g_lpMainDlg->m_iSelectedDate = 0;
	}

	nsFTP::CFTPClient ftp;
	nsFTP::CLogonInfo loginInfo(g_lpMainDlg->scp.getData("LOGSERVERIP"), g_lpMainDlg->FtpPort, g_lpMainDlg->strFtpId, g_lpMainDlg->strFtpPw);

	if	(!ftp.Login(loginInfo))
	{
		cLOG->outputf("Can not connect to log server's ftp \n");
		g_bIsDownloadLogTime = TRUE;
		return ;
	}

	char	strMonthDir[1024];
	char	strMonthDirFile[1024];
	char	fn[1024];
	char	strTarget[1024];
	bool isdn;

	cLOG->outputf("========================================\n");
	cLOG->outputf("DownLoad Date : %d %d\n",curDate,cSRVUTIL::GetTimes()); 
	cLOG->outputf("========================================\n");

	int sCount = atoi(g_lpMainDlg->scp.getData("SERVERCOUNT"));

	for(int i=0; i<sCount; i++)
	{
		sprintf(strMonthDir,"%s\\%s",g_lpMainDlg->m_strServerLogDir,g_lpMainDlg->scp.getData("SERVERNAME",i+1));
		CreateDirectory(strMonthDir,NULL);
		sprintf(strMonthDirFile,"%s\\%d.log.nux",strMonthDir,selDate);
		sprintf(fn,"%s/%d.log",strMonthDir,selDate);
		cLOG->outputf("Save Folder : %s\n",strMonthDir);
		cLOG->outputf("Filename : %s\n",fn);
		cLOG->outputf("========================================\n");

		FILE * logfp = NULL;
		FILE * testfp = NULL;
		int retry_count = 0;

		///////////////////////////////////////////////////////////
		//	paran, yahoo를 위해 통계정보를 가져온다. 
		//	Account Sql DB에 있는 RedStone_Info의 UserStats 테이블
		cLOG->outputf("DB [%s] Table Name [%s] \n",g_strAccountDBIP,g_strAccountTableName);
	
		if	(cADO_SERVERINFO.Connect(g_strAccountDBIP,g_strAccountDBID, g_strAccountDBPW,g_strAccountTableName,1))
		{
			//	가져올 날짜, 서버 인덱스, 
			cLOG->outputf("detail user count statistic DB [%s] Table Name [%s] ",g_strAccountDBIP,g_strAccountTableName);
			int ret = cADO_SERVERINFO.spGetDetailUserCountStat((int)selDate,i,&_pVector_UserStats[i]);
			cLOG->outputf(" stat ret [%d], seldate [%d],  count [ %d]\n ",ret,selDate, _pVector_UserStats[i].size());
			cADO_SERVERINFO.Close();
		}
		else
		{
			cLOG->outputf("Can not connect to REDSTONE_INFO DB!\n");
		}
#ifdef _FOR_KOREA
		if(i == sCount-1)	//	소룬드
		{
			goto JUMP_SORUND;
		}
#endif
		///////////////////////////////////////////////////////////
		//	 로그 다운
		logfp = fopen(fn,"r");
		//	다운 받자 !!

		if	(!logfp && selDate!=curDate)
		{
			retry_count = 0;
			cLOG->outputf(" DownLoad Start -> %d\n",cSRVUTIL::GetTimes());
			sprintf(strTarget,"/%s/press/%d.log.nux",g_lpMainDlg->scp.getData("SERVERNAME",i+1),selDate);

			isdn = ftp.DownloadFile(strTarget, strMonthDirFile);
			cLOG->outputf("Download Info :%s ,%s\n",strTarget,strMonthDirFile);
			sprintf(fn,"%s/%d.log.nux",strMonthDir,selDate);

			testfp = fopen(fn,"r");

			if	(!testfp || filelength(fileno(testfp))==0)
			{
				if(testfp)	fclose(testfp);
				cLOG->outputf("Retry download file...\n");
				Sleep(5000);
				ftp.DownloadFile(strTarget, strMonthDirFile);
				testfp = fopen(fn,"r");
				if(!testfp || filelength(fileno(testfp))==0)
				{
					if(testfp)	fclose(testfp);
					cLOG->outputf("failed to download....\n\n");
					continue;
				}else
				{
					cLOG->outputf("Extract nux file : [%s], [%s]\n",fn,strMonthDir);
					cNUX::PressExtract(fn,strMonthDir);
				}
			}
			else
			{
				if	(testfp)
					fclose(testfp);

				cLOG->outputf("Extract nux file : [%s], [%s]\n",fn,strMonthDir);
				cNUX::PressExtract(fn,strMonthDir);
			}

			//	nux파일은 죽인다.
			DeleteFile(fn);
#ifdef _FOR_KOREA
JUMP_SORUND:
#endif
			cLOG->outputf("Download complete!!\n");
			cLOG->outputf("========================================\n");
		}else
		{
			fclose(logfp);
			cLOG->outputf("The file is exist!!\n");
			cLOG->outputf("========================================\n");
		}
	}

	ftp.Logout();

	//	각 서버 이름별로 파일들을 정리
	for(i = 0;i<sCount;i++)
	{
		sprintf(strMonthDir,"%s\\%s",g_lpMainDlg->m_strServerLogDir,g_lpMainDlg->scp.getData("SERVERNAME",i+1));
		CreateDirectory(strMonthDir,NULL);
		sprintf(fn,"%s/%d_Score.log",strMonthDir,selDate);
		FILE * _srvFile = NULL;
		_srvFile = fopen(fn,"w");

		if	(_srvFile && _pVector_UserStats[i].size() > 0)
		{
			int allmax=0,allavr=0,ymax=0,yavr=0,pmax=0,pavr=0;

			for(int dc=0;dc<1440;dc+=5)
			{
				cADO_AnalyzeTool::stUserStatsPack _pack = _pVector_UserStats[i].at(dc);

				fprintf(_srvFile,"T%02d_%02d = %d | %d | %d; \n",dc/60,dc%60,_pack.iAllUserCount,_pack.iReserve1UserCount,_pack.iReserve2UserCount);
				if(allmax<_pack.iAllUserCount)	allmax = _pack.iAllUserCount;
				if(ymax<_pack.iReserve1UserCount)	ymax = _pack.iReserve1UserCount;
				if(pmax<_pack.iReserve2UserCount)	pmax = _pack.iReserve2UserCount;
				allavr += _pack.iAllUserCount;
				yavr += _pack.iReserve1UserCount;
				pavr += _pack.iReserve2UserCount;
			}
			allavr /= 288;//(1440/5)
			yavr /= 288;
			pavr /= 288;
			fprintf(_srvFile,"MAXUSER = %d | %d | %d | %d | %d | %d; \n",allmax,allavr,ymax,yavr,pmax,pavr);
			fclose(_srvFile); _srvFile = NULL;
		}
		else
		{
			cLOG->outputf("Can not create the Score file [%s]!",fn);
			if(_srvFile)
			{
				fclose(_srvFile); _srvFile = NULL;
			}
		}
	}

#ifdef _FOR_KOREA
	//	해당 일자의 yahoo관련 내용을 정리한다.
	sprintf(strMonthDir,"%s\\_YAHOO",g_lpMainDlg->m_strServerLogDir);
	CreateDirectory(strMonthDir,NULL);
	sprintf(fn,"%s/%d_Yahoo.log",strMonthDir,selDate);
	FILE * _srvFile = fopen(fn,"w");
	if(_srvFile)
	{
		for(i=0; i<sCount; i++)
		{
#ifdef _FOR_KOREA
		if(i == sCount-1)	//	소룬드
		{
			break;
		}
#endif
			for(int dc=0;dc<1440;dc+=5)
			{
				if(_pVector_UserStats[i].size() ==0 )	continue;
				cADO_AnalyzeTool::stUserStatsPack _pack = _pVector_UserStats[i].at(dc);
				fprintf(_srvFile,"702|~~|%d|~~|1|~~|redgem.co.kr|~~|0000|~~|%s|~~|%d|~~|%02d-%02d-%02d %02d:%02d:00\n",i,g_lpMainDlg->scp.getData("SERVERNAME",i+1),_pack.iReserve1UserCount,selDate/10000,((selDate%10000)/100),(selDate%100),dc/60,dc%60);
			}
		}
		fclose(_srvFile); _srvFile = NULL;
	}else
	{
		cLOG->outputf("Can not create the YAHOO log file [%s]!",fn);
	}

	//	해당 일자의 paran관련 내용을 정리한다.
	sprintf(strMonthDir,"%s\\_PARAN",g_lpMainDlg->m_strServerLogDir);
	CreateDirectory(strMonthDir,NULL);
	sprintf(fn,"%s/%d_Paran.log",strMonthDir,selDate);
	_srvFile = fopen(fn,"w");

	if	(_srvFile)
	{
		for(i=0; i<sCount; i++)
		{
#ifdef _FOR_KOREA
			if(i == sCount-1)	//	소룬드
			{
				break;
			}
#endif
			for(int dc=0;dc<1440;dc+=5)
			{
				if	(_pVector_UserStats[i].size() ==0 )
					continue;

				cADO_AnalyzeTool::stUserStatsPack _pack = _pVector_UserStats[i].at(dc);
				fprintf(_srvFile,"702|~~|%d|~~|1|~~|redgem.co.kr|~~|0000|~~|%s|~~|%d|~~|%02d-%02d-%02d %02d:%02d:00\n",i,g_lpMainDlg->scp.getData("SERVERNAME",i+1),_pack.iReserve2UserCount,selDate/10000,(selDate%10000)/100,(selDate%100),dc/60,dc%60);
			}
		}
		fflush(_srvFile);
		fclose(_srvFile); _srvFile = NULL;
		//	Paran은 보내는 과정이 필요하다.
		int iPort = atoi(g_lpMainDlg->scp.getData("PARANFTP_INFO",2));
		char ip[16];
		strcpy(ip,g_lpMainDlg->scp.getData("PARANFTP_INFO"));
		char id[32],pw[32];
		strcpy(id,g_lpMainDlg->scp.getData("PARANFTP_INFO",3));
		strcpy(pw,g_lpMainDlg->scp.getData("PARANFTP_INFO",4));

		Sleep(1000);

		nsFTP::CFTPClient paranftp;
		nsFTP::CLogonInfo loginInfo(ip, iPort, id, pw);
		if(paranftp.Login(loginInfo))
		{
			char sfn[1024];
			sprintf(sfn,"%02d%04d_CUser_702.log",(selDate%1000000)/10000,(selDate%10000));

			bool ret = paranftp.UploadFile(fn, sfn);	//	파일이 없을 때는 생성만 되고 ..마네?

			cLOG->outputf("Send Paran Data File : ret %d [%s -> %s]!\n\n",ret,fn,sfn);
			paranftp.Logout();
		}else
			cLOG->outputf("Can not Send PARAN log file\n\n");
	}else
	{
		cLOG->outputf("Can not create the PARAN log file [%s]!\n\n",fn);
	}
#endif
	g_bIsWaitingDownloadLog = FALSE;
	return ;
}
