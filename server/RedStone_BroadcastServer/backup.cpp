#include "backup.h"
#include "Client.h"

cBackupUserDataToFile	g_bud;	//	backup user data

cBackupUserDataToFile::cBackupUserDataToFile()
{
	InitializeCriticalSection(&m_cs);
}

cBackupUserDataToFile::~cBackupUserDataToFile()
{
	DeleteCriticalSection(&m_cs);
}

BOOL
cBackupUserDataToFile::save(void *_lpClient,BOOL _bIsSaved)
{
	return	TRUE;
	mCS(m_cs);

	CClient					*lpClient	=	(CClient	*)_lpClient;
	cUserCustomDataDefine	*lpInfo		=	&lpClient->m_clPersonalData;

	lpClient->m_strId[dID_LENGTH-1]		=	NULL;

	cFILE	file;

	if	(!file.Open(_ms("%s/data/%s[%d].dat",g_strLogFolder,lpClient->m_strId,lpClient->m_wAvatarIndex),"wb",TRUE))
		return	FALSE;

	DWORD	dwCrc32;

	cCRC32::BufferToCrc32(lpInfo,sizeof(cUserCustomDataDefine),dwCrc32);

	file.Write(&_bIsSaved,4);
	file.Write(&dwCrc32,4);
	file.Write(&lpClient->m_iDBIndex,4);
	file.Write(lpInfo,sizeof(cUserCustomDataDefine));
	file.Close();

	return	TRUE;
}

BOOL
cBackupUserDataToFile::checkData(void *_lpReceiveInfo,char *_lpstrId,char *_lpstrName,int _iAvatarIndex)
{
	return	TRUE;
	mCS(m_cs);

	cUserCustomDataDefine	*lpInfo		=	(cUserCustomDataDefine	*)_lpReceiveInfo;
	cUserCustomDataDefine	saveData;

	_lpstrId[dID_LENGTH-1]		=	NULL;
	_lpstrName[dNAME_LENGTH-1]	=	NULL;

	char	strFileName[512];

	sprintf(strFileName,"%s/data/%s[%d].dat",g_strLogFolder,_lpstrId,_iAvatarIndex);

	cFILE	file;

	if	(!file.Open(strFileName,"rb",TRUE))
		return	TRUE;

	BOOL	bIsSaved;
	DWORD	dwSaveFileCRC32;
	int		iDBIndex;

	file.Read(&bIsSaved,4);
	file.Read(&dwSaveFileCRC32,4);
	file.Read(&iDBIndex,4);
	file.Read(&saveData,sizeof(saveData));

	DWORD	dwCrc32;

	cCRC32::BufferToCrc32(&saveData,sizeof(cUserCustomDataDefine),dwCrc32);

	if	(dwCrc32	!=	dwSaveFileCRC32)
	{
		DeleteFile(strFileName);

		return	TRUE;
	}

	cCRC32::BufferToCrc32(_lpReceiveInfo,sizeof(cUserCustomDataDefine),dwCrc32);

	if	(dwCrc32	!=	dwSaveFileCRC32)
	{
		char	*strIsSaved[]	=	{"DB에 저장 안함","DB에 저장 함"};

		_log("!!!!!!!!!! incorrect data !!!!!!!!!!!");
		_log("[%s/%s][%d] %s",_lpstrId,_lpstrName,iDBIndex,strIsSaved[bIsSaved]);

		int		i,j,k;

		for	(i=0;i<2;i++)
			if	(lpInfo->m_aLeftAbility[i]	!=	saveData.m_aLeftAbility[i])
				_log("m_aLeftAbility[%d] %d,%d",i,lpInfo->m_aLeftAbility[i],saveData.m_aLeftAbility[i]);

		for	(i=0;i<2;i++)
			if	(lpInfo->m_aSubAbility[i]	!=	saveData.m_aSubAbility[i])
				_log("m_aLeftAbility[%d] %d,%d",i,lpInfo->m_aSubAbility[i],saveData.m_aSubAbility[i]);

		for	(i=0;i<2;i++)
			if	(lpInfo->m_aRightAbility[i]	!=	saveData.m_aRightAbility[i])
				_log("m_aLeftAbility[%d] %d,%d",i,lpInfo->m_aRightAbility[i],saveData.m_aRightAbility[i]);

		for	(i=0;i<2;i++)
			if	(lpInfo->m_aSupportAbility[i]	!=	saveData.m_aSupportAbility[i])
				_log("m_aLeftAbility[%d] %d,%d",i,lpInfo->m_aSupportAbility[i],saveData.m_aSupportAbility[i]);

		for	(i=0;i<10;i++)
			for	(j=0;j<10;j++)
				for	(k=0;k<2;k++)
					if	(lpInfo->m_aQuickSkill[i][j][k]	!=	saveData.m_aQuickSkill[i][j][k])
						_log("m_aQuickSkill[%d][%d][%d] %d,%d",i,j,k,lpInfo->m_aQuickSkill[i][j][k],saveData.m_aQuickSkill[i][j][k]);

		for	(i=0;i<2;i++)
			for	(j=0;j<dACTIVE_REACTION_COUNT;j++)
				if	(lpInfo->m_aActivePassiveSkill[i][j]	!=	saveData.m_aActivePassiveSkill[i][j])
					_log("m_aActivePassiveSkill[%d][%d] %d,%d",i,j,lpInfo->m_aActivePassiveSkill[i][j],saveData.m_aActivePassiveSkill[i][j]);

		for	(i=0;i<dFRIEND_GROUP_COUNT;i++)
		{
			lpInfo->m_strGroupName[i][dFRIEND_GROUP_NAME_LENGTH-1]		=	NULL;
			saveData.m_strGroupName[i][dFRIEND_GROUP_NAME_LENGTH-1]	=	NULL;
			if	(STRICMP(lpInfo->m_strGroupName[i],saveData.m_strGroupName[i])!=0)
				_log("m_strGroupName[%d] %s,%s",i,lpInfo->m_strGroupName[i],saveData.m_strGroupName[i]);
		}

		for	(i=0;i<dMAX_FRIEND_COUNT;i++)
		{
			lpInfo->m_aFriendList[i].m_strName[dNAME_LENGTH-2]	=	NULL;
			saveData.m_aFriendList[i].m_strName[dNAME_LENGTH-2]	=	NULL;

			if	(STRICMP(lpInfo->m_aFriendList[i].m_strName,saveData.m_aFriendList[i].m_strName)!=0)
				_log("m_aFriendList[%d].m_strName %s,%s",i,lpInfo->m_aFriendList[i].m_strName,saveData.m_aFriendList[i].m_strName);
		}

		return	FALSE;
	}

	return	TRUE;
}