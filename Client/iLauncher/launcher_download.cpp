#include	"launcher.h"
#include	"message.h"

void
cLauncher::progress(cDOWNLOAD *_lpInfo)
{
	memcpy(&m_downloadData,_lpInfo,sizeof(cDOWNLOAD));
	
	char	strText[512];

	m_dwLastUpdateTime	=	timeGetTime();

	if	(m_iPatchFileCount == 0)
	{
		strcpy(m_strPatchStatusText,dMSG_NOT_EXIST_PATCH_DATA);
//		SetDlgItemText(m_hProgressWnd,IDC_INFO,"");
//		SendMessage(GetDlgItem(m_hProgressWnd,IDC_PROGRESS), PBM_SETPOS, 0, 0);
//		SetDlgItemText(m_hProgressWnd,IDC_DOWNLOAD_TEXT,dMSG_CONFIRM_VERSION_INFO);
	}
	else
	{
		for (int i=strlen(_lpInfo->m_strUrl)-1;i>=0;i--)
			if	(_lpInfo->m_strUrl[i] == '/')
			{
				strcpy(strText,_lpInfo->m_strUrl+i+1);
				break;
			}

		strcpy(m_strPatchStatusText,dMSG_DOWNLOAD_PATCH_DATA);

		int		iReceivedSizeHigh,iReceivedSizeLow;
		int		iDataSizeHigh,iDataSizeLow;
		float	fValue;

		fValue				=	(float)_lpInfo->m_dwCurrentFileReceivedSize/1024.0f/1024.0f;
		iReceivedSizeHigh	=	(int)fValue;
		iReceivedSizeLow	=	((int)(fValue*100)%100);

		fValue				=	(float)_lpInfo->m_dwCurrentFileSize/1024.0f/1024.0f;
		iDataSizeHigh		=	(int)fValue;
		iDataSizeLow		=	((int)(fValue*100)%100);

		sprintf(m_strCurrentDownLoadFileInfo,"%s(%d.%.2dMB/%d.%.2dMB)",strText,iReceivedSizeHigh,iReceivedSizeLow,iDataSizeHigh,iDataSizeLow);

		m_iProgressRate		=	_lpInfo->m_iProgressRate;

//		iReceivedSizeHigh	=	(int)((_lpInfo->m_dwReceivedSize+_lpInfo->m_dwCurrentFileReceivedSize)/1024/1024);
//		iReceivedSizeLow	=	(int)(((_lpInfo->m_dwReceivedSize+_lpInfo->m_dwCurrentFileReceivedSize)%(1024*1024))/1024);
//		iReceivedSizeLow	=	iReceivedSizeLow*100/1024;
		fValue				=	(float)(_lpInfo->m_dwReceivedSize+_lpInfo->m_dwCurrentFileReceivedSize)/1024.0f/1024.0f;
		iDataSizeHigh		=	(int)fValue;
		iDataSizeLow		=	((int)(fValue*100)%100);

		m_dwReceivedSize	=	(DWORD)(_lpInfo->m_dwReceivedSize+_lpInfo->m_dwCurrentFileReceivedSize);

		sprintf(m_strDownLoadInfo,"%s... %d/%d files(%d.%.2dMB Received)",
							dMSG_DOWNLOADING,m_iCurrentPatchFile,m_iPatchFileCount-m_iFirstPatchFile,iDataSizeHigh,iDataSizeLow);
	}

//	m_dwBps가 킬로바이트 단위이다.
	if	(_lpInfo->m_dwBps/1024>0)
	{
		float	fValue	=	(float)_lpInfo->m_dwBps/1024.0f;
		int		iValue1	=	(int)fValue;
		int		iValue2	=	((int)(fValue*100)%100);

		sprintf(m_strDownloadSpeed,"%s : %d.%.3d MB/Sec",dMSG_TRANS_SPEED,iValue1,iValue2);
	}
	else
	{
		int	iValue1	=	(int)_lpInfo->m_dwBps;

		sprintf(m_strDownloadSpeed,"%s : %d KB/Sec",dMSG_TRANS_SPEED,iValue1);
	}
}

void
cLauncher::downLoad()
{
	char	strText[1024];

	m_patch.setBeginTime();

	m_dwLastUpdateTime	=	0;

	if	(m_iPatchFileCount)
	{
		cNUX::SetExtractFunc(ProgressExtract);

		m_iCurrentPatchFile	=	0;

		for (int i=1;i<=m_iPatchFileCount;i++)
		{
			m_iCurrentPatchFile++;

			int	iPatchVersion	=	m_iCurrentVersion+i;

#ifdef	_DEBUG
//			sprintf(strText,"ftp://211.189.122.82/redstone_patch%.4d.nux",iPatchVersion);
//			sprintf(strText,"ftp://redstone.feelamint.com/redstone/patch/redstone_patch%.4d.nux",iPatchVersion);
			sprintf(strText,"%s%.4d.nux",m_strPatchDataAddress,iPatchVersion);
#else
			sprintf(strText,"%s%.4d.nux",m_strPatchDataAddress,iPatchVersion);
#endif

			if	(!m_patch.downLoad(strText,Progress))
			{
				m_isCompleteUpdate	=	FALSE;
				m_isOperationSuccess=	FALSE;
				ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_DOWNLOAD_PATCH_FILE);

				changePatchStatus(ePS_ReadyToLaunch);
				return;
			}

			m_downloadData.m_dwCurrentFileReceivedSize	=	m_downloadData.m_dwCurrentFileSize;

			progress(&m_downloadData);

			m_iProgressRate	=	1000;

			m_downloadData.m_bIsExtractFile	=	TRUE;

			strcpy(m_strPatchStatusText,dMSG_INSTALL_DOWNLOAD_PATCH_DATA);

			if	(!extractFile(m_patch.m_strFileName))
			{
				SetFileAttributes(m_patch.m_strFileName,FILE_ATTRIBUTE_ARCHIVE);
				DeleteFile(m_patch.m_strFileName);

				m_isCompleteUpdate	=	FALSE;
				m_isOperationSuccess=	FALSE;

				changePatchStatus(ePS_ReadyToLaunch);

				return;
			}

			m_downloadData.m_bIsExtractFile	=	FALSE;
		}
	}

	m_isCompleteUpdate	=	TRUE;
	m_isOperationSuccess=	TRUE;

	changePatchStatus(ePS_ReadyToLaunch);
}

//
//	압축 풀기
BOOL
cLauncher::extractFile(char *_lpstrFilename)
{
	if	(cNUX::MergeExtract(_lpstrFilename,_logFolder))
	{
		SetFileAttributes(_lpstrFilename,FILE_ATTRIBUTE_ARCHIVE);
		DeleteFile(_lpstrFilename);

		return	TRUE;
	}

	return	ERRMSG(dMSG_ERROR_AUTO_PATCH,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,_lpstrFilename);
}