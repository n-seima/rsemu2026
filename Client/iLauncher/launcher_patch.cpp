#include	"launcher.h"
#include	"update.h"
#include	"cText.h"
#include	"message.h"

enum
{
	eLI_progress_total	=	34-1,
	eLI_progress_current=	35-1,
	eLI_progress_back	=	36-1,
};

void
cLauncher::drawPatchStatus()
{
	cTEXT::PushShadow(FALSE);

	DWORD	dwProgressTime		=	timeGetTime() - m_dwBeginTime;

	int	iProgressHour			=	(int)(dwProgressTime/1000/60/60);
	int	iProgressMinute			=	(int)((dwProgressTime/1000/60)%60);
	int	iProgressSecond			=	(int)((dwProgressTime/1000)%60);

	sprintf(m_strSpentTime,"%s : %.2d:%.2d:%.2d",dMSG_SPENT_TIME,iProgressHour,iProgressMinute,iProgressSecond);

	g_sprPannel.Put(188,429,eLI_progress_back);

	if	(m_downloadData.m_bIsExtractFile	==	TRUE	&&	m_lpExtractInfo)
	{
		g_sprPannel.Put(188,429,eLI_progress_total,m_lpExtractInfo->m_iProgressRate/10);

		cTEXT::Put(186,440,0,m_strPatchStatusText);

		{
			float	fValue;

			fValue				=	(float)(m_dwReceivedSize)/1024.0f/1024.0f;
			int	iDataSizeHigh	=	(int)fValue;
			int	iDataSizeLow	=	((int)(fValue*100)%100);

			sprintf(m_strDownLoadInfo,"%s... %d/%d files(%d.%.2dMB Received)",
								dMSG_EXTRACT_FILE,m_iCurrentPatchFile,m_iPatchFileCount-m_iFirstPatchFile,iDataSizeHigh,iDataSizeLow);
		}

		cTEXT::Put(186,454,0,m_strDownLoadInfo);
		cTEXT::Put(540,454,0,m_strSpentTime);

		cTEXT::Put(186,466,0,_ms("> %s",m_lpExtractInfo->m_strCurrentFile));
	}
	else
	{
		g_sprPannel.Put(188,429,eLI_progress_total,m_iProgressRate/10);

		cTEXT::Put(186,440,0,m_strPatchStatusText);
		cTEXT::Put(186,454,0,m_strDownLoadInfo);
		cTEXT::Put(540,454,0,m_strSpentTime);
		cTEXT::Put(186,466,0,m_strCurrentDownLoadFileInfo);
		cTEXT::Put(540,466,0,m_strDownloadSpeed);
	}

	cTEXT::PopShadow();
}