#include "cHero.h"
#include "cActor.h"

#define	dHERO_INFO_RAND_VALUE			45024865
#define	dCHECK_CODE_OF_HERO_SAVE_DATA	0x12345678

//
//	Custom Hero Save Data
enum
{
	eCHSD_ADD_SUB_SKILL_STATUS	=	2	,
	eCHSD_RENEWEL_FRIEND_LIST			,
	eCHSD_RENEWEL_ALL_DATA				,

	eCURRENT_HERO_SAVE_DATA_VERSION	
};

void
CHero::correctCustomData()
{
	correctQuickSlotData();

	if	(m_wLeftAbility	==	0xffff)
		setLeftSkill();
}

void
CHero::setFriendListOnlineStatus(cFriendListOnlineInfoBase *_lpInfo)//	친구 온라인 상태 설정
{
	for (int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			break;

		m_aFriendList[i].m_bf1IsOnline	=	_lpInfo->isOnlineFriend(i);
	}
}

void
CHero::setCustomData(cUserCustomDataDefine *_lpData,int _iFriendCount)	//	유저 커스텀 데이터 설정
{
	WORD	aBuffer[2][dACTIVE_REACTION_COUNT];

	memcpy(aBuffer,m_aActivePassiveSkill,sizeof(aBuffer));

	cUserCustomDataDefine *lpUserData	=	getUserCustomData();

	lpUserData->copy(_lpData);

	memcpy(m_aActivePassiveSkill,aBuffer,sizeof(aBuffer));

	if	(m_aLeftAbility[0]	==	0xffff	&&	m_aLeftAbility[1]	==	0xffff)	//	데이터가 없다.
	{
		m_wLeftAbility		=	0xffff;
		m_wSubAbility		=	0xffff;
		m_wRightAbility		=	0xffff;
		m_wSupportAbility	=	0xffff;

		readOldUserData();

		int	iFriendCount	=	getFriendCount();

		CB_UPDATE_USER_ALLINFO	packet;

		packet.base.set(sizeof(packet),dCB_UPDATE_USER_ALLINFO);

		memcpy(&packet.data,getUserCustomData(),sizeof(cUserCustomDataDefine));

		packet.base.wSize	=	sizeof(packet)-sizeof(packet.data.m_aFriendList)+sizeof(cFriendInfoDefine)*iFriendCount;

		s_agent.sendBCSPacket(&packet);

		m_bf1IsSaveAllDataByClient	=	true;
	}

	m_wLeftAbility				=	m_aLeftAbility[m_wJob%2];
	m_wRightAbility				=	m_aRightAbility[m_wJob%2];
	m_wSubAbility				=	m_aSubAbility[m_wJob%2];
	m_wSupportAbility			=	m_aSupportAbility[m_wJob%2];
}

int
CHero::getFriendCountByGroup(int _iGroup,int *_lpiFriendCount,int *_lpiActiveFriendCount)
{
	int iFriendCount=0,iActiveFriendCount=0;

	for (int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			break;

		if	(m_aFriendList[i].m_bf4Group	==	_iGroup)
		{
			iFriendCount++;

			if	(m_aFriendList[i].m_bf1IsOnline)
				iActiveFriendCount++;
		}
	}

	if	(_lpiFriendCount)
		(*_lpiFriendCount)		=	iFriendCount;

	if	(_lpiActiveFriendCount)
		(*_lpiActiveFriendCount)=	iActiveFriendCount;

	return	iFriendCount;
}

cFriendInfoDefine*
CHero::getFriend(int _iIndex)
{
	if	(_iIndex < 0	||	_iIndex	>=	dMAX_FRIEND_COUNT	)
		return	NULL;
	if	(m_aFriendList[_iIndex].m_strName[0]	==	NULL	)
		return	NULL;

	return	&m_aFriendList[_iIndex];
}

cFriendInfoDefine*
CHero::getFriend(char *_lpstrName)
{
	for (int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			return	NULL;

		if	(STRICMP(_lpstrName,m_aFriendList[i].m_strName)	==	0)
			return	&m_aFriendList[i];
	}

	return	NULL;
}

//
//	친구 추가
int
CHero::addFriend(char *_lpstrName,WORD _wJob,BOOL _bIsByRecover)
{
	int	iCount	=	getFriendCount();

	if	(iCount	>=	dMAX_FRIEND_COUNT)
		return	eAFR_FAILED_BY_TOO_MANY;

	if	(getFriendCountByName(_lpstrName))
		return	eAFR_FAILED_BY_EXIST_SAME_NAME;

	cFriendInfoDefine	info;

	strcpy(info.m_strName,_lpstrName);
	info.m_bf8Job		=	_wJob;
	info.m_bf1IsOnline	=	TRUE;
	info.m_bf4Group		=	0;

	if	(_bIsByRecover)
		info.m_bf1IsOnline	=	FALSE;
	
	cUserCustomDataDefine::addFriend(&info);

	return	eAFR_SUCCESS;
}

//
//	친구 제거
BOOL
CHero::removeFriendByName(char *_lpstrName)
{
	char	strRemoveFriendName[dNAME_LENGTH];

	memcpy(strRemoveFriendName,_lpstrName,dNAME_LENGTH);

	if	(!CheckBCS())
		return	FALSE;

	if	(cUserCustomDataDefine::removeFriend(strRemoveFriendName))
	{
		sendRemoveFriend(strRemoveFriendName);

		return	TRUE;
	}

	return	FALSE;
}

//
//	친구냐?
BOOL
CHero::isFriend(char *_lpstrName)
{
	int	iCount	=	getFriendCount(),i;

	for (i=0;i<iCount;i++)
		if	(STRICMP(_lpstrName,m_aFriendList[i].m_strName) == 0)
			return	TRUE;

	return	FALSE;
}

void
CHero::disconnectFriend(char *_lpstrName)
{
	cFriendInfoDefine	*lpFriend	=	getFriend(_lpstrName);

	if	(lpFriend)
		lpFriend->m_bf1IsOnline	=	FALSE;
}

void
CHero::connectFriend(char *_lpstrName,int _iJob)
{
	cFriendInfoDefine	*lpFriend	=	getFriend(_lpstrName);

	if	(lpFriend)
	{
		lpFriend->m_bf1IsOnline	=	TRUE;
		lpFriend->m_bf8Job		=	_iJob;
	}
}

BOOL
CHero::setFriendJob(char *_lpstrName,WORD _wJob)
{
	int	iCount	=	getFriendCount(),i;

	if	(iCount	<=	0)
		return	FALSE;

	for (i=0;i<iCount;i++)
	{
		if	(STRICMP(_lpstrName,m_aFriendList[i].m_strName) == 0)
		{
			m_aFriendList[i].m_bf8Job	=	_wJob;

			return	TRUE;
		}
	}

	return	FALSE;
}


int
CHero::getFriendCountByName(char *_lpstrName)
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_FRIEND_COUNT;i++)
		if	(m_aFriendList[i].m_strName[0]	)
			if	(STRICMP(_lpstrName,m_aFriendList[i].m_strName)	==	0)
				iCount++;

	return	iCount;
}

//
//	친구 이름 얻기
char*
CHero::getFriendName(int _iIndex)
{
	if (_iIndex	==	-1	||	_iIndex	>=	dMAX_FRIEND_COUNT)
		return	NULL;

	if	(m_aFriendList[_iIndex].m_strName[0]	==	NULL)
		return	NULL;

	return	m_aFriendList[_iIndex].m_strName;
}

void
CHero::saveUserData(cFILE *_lpFile,cUserCustomData *_lpData)
{
	cUserCustomData	data;
	DWORD			dwSeedValue	=	getRandValue();

//	암호화
	{
		memcpy(&data,_lpData,sizeof(cUserCustomData));

		EncodeBuffer(dwSeedValue,(BYTE*)&data,sizeof(cUserCustomData));
	}

	_lpFile->Write(&data,sizeof(cUserCustomData));
}

void
CHero::readUserData(cFILE *_lpFile,cUserCustomData *_lpData)
{
	cUserCustomData	data;

	_lpFile->Read(&data,sizeof(cUserCustomData));

	DWORD	dwSeedValue	=	getRandValue();

//	암호화
	{
		DecodeBuffer(dwSeedValue,(BYTE*)&data,sizeof(cUserCustomData));
	}

	memcpy(_lpData,&data,sizeof(cUserCustomData));

	for	(int iFriend=0;iFriend<dMAX_FRIEND_COUNT;iFriend++)
	{
		cFriendInfoDefine	*lpFriendInfo	=	&_lpData->m_aFriendList[iFriend];

		if	(strlen(lpFriendInfo->m_strName)	>	dNAME_LENGTH-1)
		{
			for	(int i=0;i<dMAX_FRIEND_COUNT;i++)
				_lpData->m_aFriendList[i].reset();

			break;
		}
	}

	for	(int iGroup=0;iGroup<dFRIEND_GROUP_COUNT;iGroup++)
	{
		if	(strlen(_lpData->m_strGroupName[iGroup])	>	dFRIEND_GROUP_NAME_LENGTH-2)
		{
			sprintf(_lpData->m_strGroupName[iGroup],"Group #%.2d",iGroup);

			if	(s_agent.isConnectedBCServer())
				sendChangeFriendGroupName(iGroup,_lpData->m_strGroupName[iGroup]);
		}
	}

	_lpData->resetCurrentFriendListOnlineStatus();
}

//
//	유저 데이터 기록
void
CHero::saveUserData()
{
	DWORD	dwCheckData		=	dCHECK_CODE_OF_HERO_SAVE_DATA;
	DWORD	dwDataVersion	=	eCURRENT_HERO_SAVE_DATA_VERSION-1;

	SetLogFolder();
	SetCurrentDirectory("data/user");

	if	(!SetCurrentDirectory(s_strSelectServerName))
	{
		SetLogFolder();
		SetCurrentDirectory("data");
		cFOLDER::Create("user",TRUE);
		cFOLDER::Create(s_strSelectServerName,TRUE);
	}

	cFILE	file;

	char	strEncodeName[dNAME_LENGTH];

	memcpy(strEncodeName,m_strName,dNAME_LENGTH);

//	이름 암호화
	{
		DWORD	dwSeedValue	=	getRandValue();

		EncodeBuffer(dwSeedValue,(BYTE*)strEncodeName,dNAME_LENGTH);
	}


	if	(IsFile(_ms("%s.ud8",m_strId)))
	{
		CopyFile(_ms("%s.ud8",m_strId),"~~temp.tmp",TRUE);

		if	(!file.Open("~~temp.tmp","rb+"))
			return;
	}
	else
	{
		cUserCustomData	saveData;

		saveData.reset();

		SetFileAttributes("~~temp.tmp",FILE_ATTRIBUTE_ARCHIVE);

		if	(!file.Open("~~temp.tmp","wb"))
			return;

		file.Write(&dwCheckData,4);
		file.Write(&dwDataVersion,4);
		file.Write(strEncodeName,sizeof(strEncodeName));

		saveUserData(&file,&saveData);
		saveUserData(&file,&saveData);
		saveUserData(&file,&saveData);
		saveUserData(&file,&saveData);

		file.Seek(0,SEEK_SET);
	}

	file.Write(&dwCheckData,4);
	file.Write(&dwDataVersion,4);
	file.Write(strEncodeName,sizeof(strEncodeName));	//	마지막으로 플레이한 아바타 이름
	file.Skip(s_iSelectAvatar*sizeof(cUserCustomData));

	saveUserData(&file,getUserCustomData());

	file.Close();

	SetFileAttributes(_ms("%s.ud8",g_hero.m_strId),FILE_ATTRIBUTE_ARCHIVE);
	DeleteFile(_ms("%s.ud8",g_hero.m_strId));
	MoveFile("~~temp.tmp",_ms("%s.ud8",g_hero.m_strId));

	SetLogFolder();
}

void
CHero::readLastPlayAvatarName(char *_lpstrName)
{
	readUserData(_lpstrName);
}

//
//	유저 데이터 읽기
void
CHero::readUserData(char *_lpstrName)
{
	cUserCustomData::reset();

	DWORD	dwCheckData;
	DWORD	dwDataVersion;
	char	strFileName[64];

	strcpy(strFileName,_ms("%s.ud8",m_strId));

	SetLogFolder();
	SetCurrentDirectory("data/user");

	if	(!SetCurrentDirectory(s_strSelectServerName))
	{
		SetLogFolder();
		SetCurrentDirectory("data");
		cFOLDER::Create("user",TRUE);
		cFOLDER::Create(s_strSelectServerName,TRUE);
	}

	cFILE	file;

	while(file.Open(strFileName,"rb"))
	{
		file.Read(&dwCheckData,4);

		if	(dwCheckData	!=	dCHECK_CODE_OF_HERO_SAVE_DATA)
		{
			file.Close();
			DeleteFile(strFileName);
			break;
		}

		file.Read(&dwDataVersion,4);

		if	(dwDataVersion	<	eCHSD_RENEWEL_ALL_DATA)
		{
			file.Close();
			break;
		}

		if	(_lpstrName)	//	마지막으로 플래이한녀석 이름만 얻어오기
		{
			file.Read(_lpstrName,sizeof(m_strName));	//	마지막으로 플레이한 아바타 이름
			DWORD	dwSeedValue	=	getRandValue();
			DecodeBuffer(dwSeedValue,(BYTE *)_lpstrName,dNAME_LENGTH);
			file.Close();
			break;
		}
		file.Skip(dNAME_LENGTH);	//	마지막으로 플레이한 아바타 이름
		file.Skip(s_iSelectAvatar*sizeof(cUserCustomData));
		readUserData(&file,getUserCustomData());

		file.Close();
		break;
	}
	// 10.01.07 스킬슬롯 아이템 초기화
	initQuickSlotItem();
	SetLogFolder();
}

DWORD
CHero::getRandValue()
{
	DWORD	dwSeedValue	=	0;
	int		iLength		=	strlen(m_strId),i;

	srand(dHERO_INFO_RAND_VALUE);

	for (i=0;i<iLength;i++)
	{
		int	iValue		=	(BYTE)m_strId[i];

		if (iValue	<	'a')
			iValue	+=	'a'-'A';

		dwSeedValue		+=	iValue+random(100);
	}

	return	dwSeedValue+dHERO_INFO_RAND_VALUE;
}

void
CHero::sendChangeUserSkillSetting(cUserCustomDataDefine::eUSER_SKILL_SET _iType,int _iValue1,int _iValue2,int _iValue3,int _iValue4)
{
	CB_CHANGE_USER_SKILL_SETTING	packet;

	packet.base.set(sizeof(packet),dCB_CHANGE_USER_SKILL_SETTING);

	packet.wWhat	=	_iType;
	packet.wValue1	=	_iValue1;
	packet.wValue2	=	_iValue2;
	packet.wValue3	=	_iValue3;
	packet.wValue4	=	_iValue4;

	s_agent.sendBCSPacket(&packet);
}

void
CHero::sendRemoveFriend(char *_lpstrName)
{
	CB_REMOVE_FRIEND	packet;

	packet.base.set(sizeof(packet),dCB_REMOVE_FRIEND);

	strcpy(packet.strName,_lpstrName);

	s_agent.sendBCSPacket(&packet);
}

void
CHero::sendChangeFriendGroupName(int _iGroup,char *_lpstrName)
{
	CB_CHANGE_FRIEND_GROUP_NAME	packet;

	packet.base.set(sizeof(packet),dCB_CHANGE_FRIEND_GROUP_NAME);

	packet.wGroup	=	_iGroup;
	strcpy(packet.strName,_lpstrName);

	s_agent.sendBCSPacket(&packet);
}

void
CHero::sendChangeFriendGroup(int _iGroup,char *_lpstrName)
{
	CB_CHANGE_FRIEND_GROUP	packet;

	packet.base.set(sizeof(packet),dCB_CHANGE_FRIEND_GROUP);

	packet.wChangeGroup	=	_iGroup;
	strcpy(packet.strName,_lpstrName);

	s_agent.sendBCSPacket(&packet);
}

