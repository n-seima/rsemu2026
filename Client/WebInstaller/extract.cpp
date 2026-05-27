#include "cMAIN.H"
#include "cMESSAGE.H"

cDOWNLOAD	l_downloadData;

//
//	다운로드
void
cMAIN::progress(cDOWNLOAD* lpInfo)
{
	memcpy(&l_downloadData,lpInfo,sizeof(cDOWNLOAD));
	
	char	strText[512];

	m_hProgressWnd	=	lpInfo->m_hWnd;

	if (m_dwLastUpdateTime+100	>	timeGetTime())
		return;
	m_dwLastUpdateTime	=	timeGetTime();

	if (m_strDownLoadFileName[0]	==	0)
		for (int i=strlen(lpInfo->m_strUrl)-1;i>=0;i--)
			if (lpInfo->m_strUrl[i] == '/')
			{
				strcpy(m_strDownLoadFileName,lpInfo->m_strUrl+i+1);
				break;
			}

	SetDlgItemText(m_hProgressWnd,IDC_INFO,dMSG_DOWNLOAD_PATCH_DATA);

	int		iReceivedSizeHigh,iReceivedSizeLow;
	int		iDataSizeHigh,iDataSizeLow;
	float	fValue;

	fValue				=	(float)lpInfo->m_dwCurrentFileReceivedSize/1024.0f/1024.0f;
	iReceivedSizeHigh	=	(int)fValue;
	iReceivedSizeLow	=	((int)(fValue*100)%100);

	fValue				=	(float)lpInfo->m_dwCurrentFileSize/1024.0f/1024.0f;
	iDataSizeHigh		=	(int)fValue;
	iDataSizeLow		=	((int)(fValue*100)%100);

	sprintf(strText,"%s(%d.%.2dMB/%d.%.2dMB)",m_strDownLoadFileName,iReceivedSizeHigh,iReceivedSizeLow,iDataSizeHigh,iDataSizeLow);
	SetDlgItemText(m_hProgressWnd,IDC_DOWNLOAD_TEXT,strText);

	SendMessage(GetDlgItem(m_hProgressWnd,IDC_PROGRESS), PBM_SETRANGE, 0, MAKELPARAM(0,1000));
	SendMessage(GetDlgItem(m_hProgressWnd,IDC_PROGRESS), PBM_SETPOS, lpInfo->m_iProgressRate, 0);

//	전송 시간 확인
	{
		int	iProgressHour			=	(int)(lpInfo->m_dwProgressTime/1000/60/60);
		int	iProgressMinute			=	(int)((lpInfo->m_dwProgressTime/1000/60)%60);
		int	iProgressSecond			=	(int)((lpInfo->m_dwProgressTime/1000)%60);

		int	iRemainFileSize			=	(int)(lpInfo->m_dwCurrentFileSize-lpInfo->m_dwCurrentFileReceivedSize);
		int	iBps					=	(int)max(lpInfo->m_dwBps,1);
		int	iForecastTime			=	(int)(iRemainFileSize/iBps);
		int	iForecastCompleteTime	=	(int)(lpInfo->m_dwProgressTime+iForecastTime);
		int	iForecastHour			=	iForecastCompleteTime/1000/60/60;
		int	iForecastMinute			=	(iForecastCompleteTime/1000/60)%60;
		int	iForecastSecond			=	(iForecastCompleteTime/1000)%60;

		sprintf(strText,"%s : %.2d:%.2d:%.2d (%.2d:%.2d:%.2d)",dMSG_SPENT_TIME,iProgressHour,iProgressMinute,iProgressSecond,iForecastHour,iForecastHour,iForecastSecond);

		SetDlgItemText(m_hProgressWnd,IDC_TIME,strText);
	}	//	전송 시간 확인

//	m_dwBps가 킬로바이트 단위이다.
	if (lpInfo->m_dwBps/1024>0)
	{
		int	iValue1	=	(int)(lpInfo->m_dwBps/1024);
		int	iValue2	=	(int)(lpInfo->m_dwBps%1000);

		sprintf(strText,"%s : %d.%.3d MB/Sec",dMSG_TRANS_SPEED,iValue1,iValue2);

		SetDlgItemText(m_hProgressWnd,IDC_DOWNLOAD_SPEED,strText);//lpInfo->m_dwBps%(1024*1024)));
	}
	else
	{
		int	iValue1	=	(int)lpInfo->m_dwBps;

		SetDlgItemText(m_hProgressWnd,IDC_DOWNLOAD_SPEED,_ms("%s : %d KB/Sec",dMSG_TRANS_SPEED,iValue1));
	}
}

//
//	압축 풀기
BOOL
cMAIN::extractFile(char *_lpstrFilename)
{
	if (cNUX::MergeExtract(_lpstrFilename,_logFolder))
	{
		SetFileAttributes(_lpstrFilename,FILE_ATTRIBUTE_ARCHIVE);
		DeleteFile(_lpstrFilename);

		return	TRUE;
	}

	return	ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,_lpstrFilename);
}

void
cMAIN::install()
{
	STARTUPINFOA		si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof( STARTUPINFO)); 
	ZeroMemory( &pi, sizeof( PROCESS_INFORMATION)); 

	si.cb			=	sizeof( STARTUPINFO); 
	si.dwFlags		=	STARTF_USESHOWWINDOW; 
	si.wShowWindow	=	SW_SHOWNORMAL;

	char	strParameter[1024];

	sprintf(strParameter,"\"%s\" %s",s_setup.m_strFileName,g_strSetupOption);

	CreateProcess( s_setup.m_strFileName,strParameter, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi); 

	if	(pi.hProcess) 
	{
		DWORD	dwExitCode	=	STILL_ACTIVE; 

		while(dwExitCode	==	STILL_ACTIVE) 
		{
			WaitForSingleObject( pi.hProcess, 1); 
			GetExitCodeProcess( pi.hProcess, &dwExitCode); 
			Sleep(1);
		}
	}

	DeleteFile(s_setup.m_strFileName);
}

//
//	다운로드
void
cMAIN::downLoad()
{
	s_setup.setBeginTime();

	m_dwLastUpdateTime	=	0;

	if (!s_setup.downLoad(m_strInstallVersionDataAddress,Progress))
	{
		ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_DOWNLOAD_PATCH_FILE);
		return;
	}

	l_downloadData.m_dwCurrentFileReceivedSize	=	l_downloadData.m_dwCurrentFileSize;

	progress(&l_downloadData);

	SendMessage(GetDlgItem(m_hProgressWnd,IDC_PROGRESS), PBM_SETPOS, 1000, 0);
	SetDlgItemText(m_hProgressWnd,IDC_INFO,dMSG_INSTALL_DOWNLOAD_PATCH_DATA);

	install();

	m_isCompleteUpdate	=	TRUE;

	SetDlgItemText(m_hProgressWnd,IDC_INFO,dMSG_COMPLETE_ALL_WORK);
	SendMessage(m_hProgressWnd,WM_COMMAND,IDC_DOWNLOAD_FINISH,0);

	return;
}