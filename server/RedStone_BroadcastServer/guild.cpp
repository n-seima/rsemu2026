#include	"guild.h"
#include	"operate_packet.H"

BOOL	cGuildManager::s_bIsExist	=	FALSE;

cGuildManager g_guildManager;

//	현재 친구들의 온라인 상태 변경(이전과 변동이 있다면 유저한테 전송한다.)
// void
// cGuildInfo::updateGuildOnlineStatus(int _iIndex,bool _bIsOnline)
// {
// 	if	(m_aFriendList[_iIndex].m_bf1IsOnline	!=	_bIsOnline)
// 	{
// 		m_bf1IsChangeFriendListOnlineStatus		=	TRUE;
// 		
// 		m_aFriendList[_iIndex].m_bf1IsOnline	=	_bIsOnline;
// 	}
// }


BOOL
cGuildInfo::addMember(int _iSerial , int _iDisplayIndex)
{
	for	(int i=0;i<m_wActiveMemberCount;i++)
	{
		if	(m_awMemberSerial[i]	==	_iSerial)
			return	TRUE;
	}
	
	if	(m_wActiveMemberCount	>=	dGUILD_MAX_PLAYER)
		return	FALSE;
	
	//
	//_iDisplayIndex
	if(_iDisplayIndex != 0xffff)
		m_bMemberDisplayIndex[m_wActiveMemberCount]	=	_iDisplayIndex;

	m_awMemberSerial[m_wActiveMemberCount++	]	=	_iSerial;
	m_bIsChangeConnetState	=	TRUE;
	return	FALSE;
}


void
cGuildInfo::removeMember(int _iSerial)
{
	for	(int i=m_wActiveMemberCount;i;)
	{
		--i;

		if	(m_awMemberSerial[i]	==	_iSerial)
		{
			for	(int j=i;j<m_wActiveMemberCount-1;j++)
			{
				m_awMemberSerial[j]	=	m_awMemberSerial[j+1];
			}

			m_awMemberSerial[m_wActiveMemberCount-1]	=	0xffff;
			m_wActiveMemberCount--;

			return;
		}
	}
}


void
cGuildInfo::sendGuildMemberConnectionList()
{
	BC_INFO_GUILD_MEMBER_CONNECTION_LIST		packet;
	memset(&packet, 0, sizeof(BC_INFO_GUILD_MEMBER_CONNECTION_LIST));
	
	int iCount = 0;
	for(int i=0;i<m_wActiveMemberCount;++i)
	{
		CClient* lpGuildMemberClient =  mCM.GetClientPnt(m_awMemberSerial[i]);
		if(!lpGuildMemberClient)
		{
			continue;
		}
		++iCount;
		DWORD dwHashNameCode = GetHashCode((BYTE *)lpGuildMemberClient->m_strName);
		packet.adwHashNameCode[i] = dwHashNameCode;
		
	}
	packet.wCount  = iCount;
	packet.base.set(sizeof(BC_INFO_GUILD_MEMBER_CONNECTION_LIST) - c_iMaxGuildMemberCount + (iCount*sizeof(DWORD)) , dBC_INFO_GUILD_MEMBER_CONNECTION_LIST);				
	sendPacketToClient(&packet);
}
void
cGuildInfo::reset()
{
	m_wSerial				=	0xffff;
	m_wActiveMemberCount	=	0;
	m_dwGuildMemberListUpdateGap = 0;
}

void
cGuildInfo::copy(cGuildInfo *_lpInfo)
{
	memcpy(this,_lpInfo,sizeof(cGuildInfo));
}

BOOL
cGuildInfo::changeConnetionState(int _iIndex , BYTE _ConnetionState)
{
	if(m_bMemberDisplayIndex[_iIndex] == 0xffff)
		return FALSE;

	if(m_bMemberConnetionState[(int)m_bMemberDisplayIndex[_iIndex]] != _ConnetionState)
	{
		m_bMemberConnetionState[m_bMemberDisplayIndex[_iIndex]] = _ConnetionState;
		m_bIsChangeConnetState	= TRUE;
	}

	return FALSE;
}

void
cGuildInfo::sendPacketToClient(void *_lpPacket)
{
	int		iSize	=	((cMSG_BASE_TYPE *)_lpPacket)->wSize;
	int		iRemoveActorCount	=	0;
	WORD	awRemoveActorList[dGUILD_MAX_PLAYER];
	int		i;

	for(i=m_wActiveMemberCount;i;)
	{
		--i;

		CClient	*lpUser	= mCM.GetClientPnt(m_awMemberSerial[i]);

		if	(!lpUser	||	lpUser->m_wSerial == 0xffff || lpUser->m_isGameServer)
			continue;

		if	(lpUser->m_wGuildSerial	!=	m_wSerial)
		{
			lpUser->setGuildSerial(lpUser->m_wGuildSerial);

			awRemoveActorList[iRemoveActorCount++]	=	m_awMemberSerial[i];

			continue;
		}

		lpUser->AddSendPacket((char *)_lpPacket,iSize);
	}

	for(i=iRemoveActorCount;i;)
	{
		--i;

		removeMember(awRemoveActorList[i]);
	}
}

cGuildManager::cGuildManager()
{
	if	(s_bIsExist)
	{
		ERRMSG("error!!","cGuildManager::cGuildManager()");

		return;
	}

	m_denGuild.init(dGUILD_MAX_COUNT);

	//	파티 리스트 전부를 받기 끝
	s_bIsExist					=	TRUE;

	InitializeCriticalSection(&m_csGuildWork);

	reset();
}

cGuildManager::~cGuildManager()
{
	DeleteCriticalSection(&m_csGuildWork);
}

void
cGuildManager::reset()
{
	m_denGuild.reset();

	for (int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		cGuildInfo	guildInfo;

		guildInfo.reset();
		guildInfo.m_wSerial	=	i;

		m_denGuild.add(&guildInfo);
	}
}

cGuildInfo*	
cGuildManager::get(int _iSerial)
{
	if	(_iSerial	<	0	||	_iSerial	>=	dGUILD_MAX_COUNT)
		return	NULL;

	return	m_denGuild.get(_iSerial);
}

void
cGuildManager::addGuildMember(int _iGuild,int _iClient ,int _GuildDisplayIndex)
{
	CCriticalSection	cs(&m_csGuildWork);

	cGuildInfo			*lpGuild	=	get(_iGuild);

	if	(lpGuild)
	{
		lpGuild->addMember(_iClient , _GuildDisplayIndex);
	}
}

void
cGuildManager::removeGuildMember(int _iGuild,int _iClient)
{
	CCriticalSection	cs(&m_csGuildWork);

	cGuildInfo			*lpGuild	=	get(_iGuild);

	if	(lpGuild)
		lpGuild->removeMember(_iClient);
}

void
cGuildManager::sendPacketToClient(int _iGuild,void *_lpPacket)
{
	cGuildInfo			*lpGuild	=	get(_iGuild);

	if	(lpGuild)
		lpGuild->sendPacketToClient(_lpPacket);
}

// void	
// cGuildManager::getGuildMemberOnlineStateArry(int _iGuild , BYTE* _pArry)
// {
// 	cGuildInfo* lpGuild = m_denGuild->notCheckedget(_iGuild);
// 
// 
// 
// 	CClient * user = UserManager.GetClientPnt(lpGuild->m_awMemberSerial[j]);
// 	
// 	if(!user)
// 	{
// 		packet.strMessage[j] = FALSE;
// 		break;
// 	}
// 	
// 	CClient * pFriend = UserManager.GetClientPnt(user->m_strName,1);			
// 	
// 	if	(!pFriend)
// 		packet.strMessage[j] = FALSE;
// 	else
// 		packet.strMessage[j] = TRUE;
// }
