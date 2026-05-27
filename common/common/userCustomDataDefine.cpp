#include "userCustomDataDefine.h"
#include "playerJobCommon.h"

void
cUserCustomDataDefine::copy(cUserCustomDataDefine *_lpData)
{
	memcpy(this,_lpData,sizeof(cUserCustomDataDefine));

	int	iFriendCount	=	getFriendCount(),i;

	for	(i=0;i<dMAX_FRIEND_COUNT;i++)
		m_aFriendList[i].m_strName[dNAME_LENGTH-1]	=	NULL;
	
	for	(i=iFriendCount;i<dMAX_FRIEND_COUNT;i++)
		memset(&m_aFriendList[i],0,sizeof(cFriendInfoDefine));
}


#pragma pack(2)

class	cFriendInfoDefine_old
{
public:
	WORD	m_bf1IsOnline		:	1;	//	온라인이냐?
	WORD	m_bf4Group			:	4;	//	친구 그룹
	WORD	m_bf4Job			:	4;	//	직업
};

#pragma pack()
//	현재 친구들의 온라인 상태 전부 오프라인으로 초기화
void
cUserCustomDataDefine::resetCurrentFriendListOnlineStatus()
{
	int	i;

	for	(i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			break;

		m_aFriendList[i].m_bf1IsOnline		=	FALSE;

		if	(m_aFriendList[i].m_bf8Job	>=	dPLAYER_JOB_COUNT)
		{
			cFriendInfoDefine_old	*lpOld	=	(cFriendInfoDefine_old	*)&m_aFriendList[i];

			m_aFriendList[i].m_bf8Job	=	lpOld->m_bf4Job;
		}
	}

	m_bf1IsChangeFriendListOnlineStatus		=	FALSE;

	for	(i=0;i<dFRIEND_GROUP_COUNT;i++)
		if	(m_strGroupName[i][0]	==	NULL)
			sprintf(m_strGroupName[i],"Group #%.2d",i+1);
}

//	현재 친구들의 온라인 상태 변경(이전과 변동이 있다면 유저한테 전송한다.)
void
cUserCustomDataDefine::updateFriendOnlineStatus(int _iIndex,bool _bIsOnline)
{
	if	(_iIndex	>=	dMAX_FRIEND_COUNT)
		return;

	if	(m_aFriendList[_iIndex].m_bf1IsOnline	!=	_bIsOnline)
	{
		m_bf1IsChangeFriendListOnlineStatus		=	TRUE;

		m_aFriendList[_iIndex].m_bf1IsOnline	=	_bIsOnline;
	}
}

void
cUserCustomDataDefine::getFriendListOnlineStatus(cFriendListOnlineInfoBase *_lpInfo)
{
	for	(int	i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			break;

		if	(m_aFriendList[i].m_bf1IsOnline)
			_lpInfo->setOnlineStatus(i);
	}
}

//	친구 목록의 온라인 상태 업데이트가 끝났다.
bool
cUserCustomDataDefine::isChangedFriendListOnlineStatus()
{
	return	m_bf1IsChangeFriendListOnlineStatus;
}

BOOL
cUserCustomDataDefine::getUserCustomDataCRC32(DWORD &_dwResult)
{
	DWORD	dwCRC32_1,dwCRC32_2;

	int		iSkillCRC32DataSize	=	sizeof(m_aLeftAbility)*4+sizeof(m_aQuickSkill);
	int		iFriendCRC32DataPos	=	iSkillCRC32DataSize+sizeof(m_aActivePassiveSkill)+sizeof(DWORD);
	int		iFriendCRC32DataSize=	sizeof(m_strGroupName)+sizeof(m_aFriendList);

	char	*lpFriendData		=	((char *)this)+iFriendCRC32DataPos;

	BOOL	bIsOk1	=	cCRC32::BufferToCrc32(this,iSkillCRC32DataSize,dwCRC32_1);
	BOOL	bIsOk2	=	cCRC32::BufferToCrc32(lpFriendData,iFriendCRC32DataSize,dwCRC32_2);

	_dwResult	=	dwCRC32_1+dwCRC32_2;

	if	(!bIsOk1	||	!bIsOk2)
		return	FALSE;

	return	TRUE;
}

void
cUserCustomDataDefine::changeSkillInfo(eUSER_SKILL_SET	_info,int _iValue1,int _iValue2,int _iValue3,int _iValue4)
{
	switch(_info)
	{
		case	eUSS_LEFT_ABILITY	:
			if	(_iValue1	>=	2)
				return;
			m_aLeftAbility[_iValue1]					=	_iValue2;
			break;
		case	eUSS_SUB_ABILITY	:
			if	(_iValue1	>=	2)
				return;
			m_aSubAbility[_iValue1]						=	_iValue2;
			break;
		case	eUSS_RIGHT_ABILITY	:
			if	(_iValue1	>=	2)
				return;
			m_aRightAbility[_iValue1]					=	_iValue2;
			break;
		case	eUSS_SUPPORT_ABILITY:
			if	(_iValue1	>=	2)
				return;
			m_aSupportAbility[_iValue1]					=	_iValue2;
			break;

		case	eUSS_QUICK_SKILL	:
			if	(_iValue1	>=	10	||	_iValue2	>=	10	||	_iValue3	>=	2)
				return;
			m_aQuickSkill[_iValue1][_iValue2][_iValue3]	=	_iValue4;
			break;
	}
}

void
cUserCustomDataDefine::addFriend(cFriendInfoDefine *_lpInfo)
{
	int	iFriendCount=	getFriendCount(),i;

	if	(iFriendCount	>=	dMAX_FRIEND_COUNT)
		return;

	for	(i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			break;

		int	iCheckResult	=	STRICMP(m_aFriendList[i].m_strName,_lpInfo->m_strName);

		if	(iCheckResult	==	0)
			return;

		if	(iCheckResult	>	0)
		{
			for (int j=iFriendCount-1;j>=i;j--)
				memcpy(&m_aFriendList[j+1],&m_aFriendList[j],sizeof(cFriendInfoDefine));

			memset(m_aFriendList[i].m_strName,0,sizeof(m_aFriendList[i].m_strName));
			memcpy(&m_aFriendList[i],_lpInfo,sizeof(cFriendInfoDefine));

			return;
		}
	}

	memset(m_aFriendList[iFriendCount].m_strName,0,sizeof(m_aFriendList[iFriendCount].m_strName));
	memcpy(&m_aFriendList[iFriendCount],_lpInfo,sizeof(cFriendInfoDefine));
}

BOOL
cUserCustomDataDefine::removeFriend(char *_lpstrName)
{
	_lpstrName[dNAME_LENGTH-1]	=	NULL;
	
	for	(int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			break;

		if	(STRICMP(m_aFriendList[i].m_strName,_lpstrName)	==	0)
		{
			for	(int j=i;j<dMAX_FRIEND_COUNT-1;j++)
			{
				if	(m_aFriendList[j].m_strName[0]	==	NULL)
					break;

				memcpy(&m_aFriendList[j],&m_aFriendList[j+1],sizeof(cFriendInfoDefine));
			}

			m_aFriendList[dMAX_FRIEND_COUNT-1].reset();

			return	TRUE;
		}
	}

	return	FALSE;
}

void
cUserCustomDataDefine::changeFriendGroupName(int _iGroup,char *_lpstrName)
{
	if	(_iGroup	>=	dFRIEND_GROUP_COUNT)
		return;

	memset(_lpstrName+dFRIEND_GROUP_NAME_LENGTH-2,0,2);

	strcpy(m_strGroupName[_iGroup],_lpstrName);
}


void
cUserCustomDataDefine::changeFriendGroup(int _iGroup,char *_lpstrName)
{
	if	(_iGroup	>=	dFRIEND_GROUP_COUNT)
		return;

	memset(_lpstrName+dFRIEND_GROUP_NAME_LENGTH-2,0,2);

	for	(int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			return;

		if	(STRICMP(m_aFriendList[i].m_strName,_lpstrName)	==	0)
		{
			m_aFriendList[i].m_bf4Group	=	_iGroup;
			return;
		}
	}
}

//
//	등록된 친구의 수
int
cUserCustomDataDefine::getFriendCount()
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(m_aFriendList[i].m_strName[0]	==	NULL)
			break;

		iCount++;
	}

	return	iCount;
}
