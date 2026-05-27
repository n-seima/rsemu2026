#include	"tool.h"

//cHackingToolUserList	tool;

void
cGetQuestExperienceInfo::reset()
{
}

void
cHackingToolUserInfo::reset()
{
	m_wIsSuspected	=	FALSE;
	m_iCount		=	0;
}


BOOL
cHackingToolUserInfo::addInfo(CTimeInfo *_lpTime,int _iIncreaseExp)
{
	if (m_iCount	>=	dMAX_EXPERIENCE_INFO_COUNT)
		return	FALSE;

	int		i,iIndex	=	m_iCount;

	DWORD	dwAddTimeValue	=	_lpTime->m_wYear*60*60*24*31*12+_lpTime->m_wMonth*60*60*24*31+_lpTime->m_wDay*60*60*24+_lpTime->m_wHour*60*60+_lpTime->m_wMinute*60+_lpTime->m_wSecond;

	for (i=0;i<m_iCount;i++)
	{
		cGetQuestExperienceInfo	*lpInfo		=	&m_aList[i];
		DWORD					dwTimeValue	=	lpInfo->m_time.m_wYear*60*60*24*31*12+lpInfo->m_time.m_wMonth*60*60*24*31+lpInfo->m_time.m_wDay*60*60*24+lpInfo->m_time.m_wHour*60*60+lpInfo->m_time.m_wMinute*60+lpInfo->m_time.m_wSecond;

		if (dwAddTimeValue	<	dwTimeValue)
		{
			iIndex	=	i;
			break;
		}
	}

	for (i=m_iCount-1;i>=iIndex;i--)
		memcpy(&m_aList[i+1],&m_aList[i],sizeof(cGetQuestExperienceInfo));

	memcpy(&m_aList[iIndex].m_time,_lpTime,sizeof(CTimeInfo));
	m_aList[iIndex].m_dwGetExperience	=	_iIncreaseExp;

	m_iCount++;

	return	TRUE;
}

void
cHackingToolUserList::reset()
{
	m_iCount		=	0;

	for (int i=0;i<1024*10;i++)
		m_aUser[i].reset();

}

BOOL
cHackingToolUserList::addInfo(char *_lpstrId,char *_lpstrName,CTimeInfo *_lpTime,int _iIncreaseExp)
{
	int	iIndex	=	0xffff,i;

	for (i=0;i<m_iCount;i++)
	{
		if (STRICMP(m_aUser[i].m_strId,_lpstrId)==0 && STRICMP(m_aUser[i].m_strName,_lpstrName)==0)
		{
			iIndex	=	i;
			break;
		}
	}

	if (iIndex	==	0xffff)
	{
		iIndex	=	m_iCount;
		m_iCount++;
	}

	strcpy(m_aUser[iIndex].m_strId,_lpstrId);
	strcpy(m_aUser[iIndex].m_strName,_lpstrName);

	return	m_aUser[iIndex].addInfo(_lpTime,_iIncreaseExp);
}

void
cHackingToolUserList::run()
{
	CDecoder	decoder;

	char		*strServer[]	=	{"프란델.txt","에립트.txt","가디우스.txt","제노스.txt","end"};
	char		*strResult[]	=	{"프란델list.txt","에립트list.txt","가디우스list.txt","제노스list.txt","end"};

	for (int iServer=0;;iServer++)
	{
		if (STRICMP(strServer[iServer],"end")==0)
			break;

		if	(!decoder.Upload(strServer[iServer],NULL))
		{
			ERRMSG("없어!!",strServer[iServer]);

			return;
		}

		reset();

		while(1)
		{
			char		strId[20];
			char		strName[20];
			CTimeInfo	timeInfo;
			int			iExperience;

			char	*lpstrStream=	decoder.getStream();

			if (STRICMP(lpstrStream,"stream not Found!!")	==	0)
				break;

			timeInfo.m_wYear	=	decoder.getNumber();
			timeInfo.m_wMonth	=	decoder.getNumber();
			timeInfo.m_wDay		=	decoder.getNumber();

			timeInfo.m_wHour	=	decoder.getNumber();
			timeInfo.m_wMinute	=	decoder.getNumber();
			timeInfo.m_wSecond	=	decoder.getNumber();

			char	*lpstrId	=	decoder.getStream();
			strcpy(strId,lpstrId);
			char	*lpstrName	=	decoder.getStream();
			strcpy(strName,lpstrName);

			iExperience			=	decoder.getNumber();

			addInfo(strId,strName,&timeInfo,iExperience);
		}


		cFILE	file;

		file.Open(strResult[iServer],"wb");

		for (int i=0;i<m_iCount;i++)
		{
			cHackingToolUserInfo	*lpUser	=	&m_aUser[i];

			file.writeStream("---------------------------------------\n");
			file.writeStream("[%s/%s]\n",lpUser->m_strId,lpUser->m_strName);

			int	iGap	=	0;

			for (int j=0;j<lpUser->m_iCount;j++)
			{
				file.writeStream("    [%.2d %.2d:%.2d:%.2d] %8d [%d 초 걸림]\n",lpUser->m_aList[j].m_time.m_wDay,lpUser->m_aList[j].m_time.m_wHour,lpUser->m_aList[j].m_time.m_wMinute,lpUser->m_aList[j].m_time.m_wSecond,
									lpUser->m_aList[j].m_dwGetExperience,iGap);

				int	iNextTimeTime	=	lpUser->m_aList[j+1].m_time.m_wDay*60*60*24+lpUser->m_aList[j+1].m_time.m_wHour*60*60+lpUser->m_aList[j+1].m_time.m_wMinute*60+lpUser->m_aList[j+1].m_time.m_wSecond;
				int	iCurrentTime	=	lpUser->m_aList[j].m_time.m_wDay*60*60*24+lpUser->m_aList[j].m_time.m_wHour*60*60+lpUser->m_aList[j].m_time.m_wMinute*60+lpUser->m_aList[j].m_time.m_wSecond;
				iGap				=	iNextTimeTime-iCurrentTime;
			}

			file.writeStream("\n");
		}
		file.Close();
	}
}