#include	"stdafx.H"
#include	"party.H"
#include	"Client.H"
#include	"operate_packet.H"

BOOL	cPartyManager::s_bIsExist	=	FALSE;

cPartyManager	g_partyManager;

const	int	c_iDefaultPartyProspectusLevelRange	=	75;

void
cPartyInfo::reset()
{
	m_wSerial				=	0xffff;
	m_dwPreferenceJobMask	=	0;		// 수정
	m_wActiveMemberCount	=	0;

	memset(m_awMemberSerial,0xff,sizeof(m_awMemberSerial));

	m_wRecruitMemberLevelRange		=	c_iDefaultPartyProspectusLevelRange;

	strcpy(m_strPartyProspectus," ");
}


void
cPartyInfo::copy(cPartyInfo *_lpInfo)
{
	memcpy(this,_lpInfo,sizeof(cPartyInfo));
}

void
cPartyInfo::getInfoForPL(cPartyInfoForPL *_lpInfo)
{
	_lpInfo->m_wSerial						=	m_wSerial;	//	파티의 고유 시리얼
	_lpInfo->m_bf3PartyObject				=	m_bf3PartyObject;	//	파티 목적
	_lpInfo->m_bf3MemberCount				=	m_bf3MemberCount;	//	파티 멤버 수
	_lpInfo->m_bf10RecruitMemberLevelRange	=	m_wRecruitMemberLevelRange;
	_lpInfo->m_dwPreferenceJobMask			=	m_dwPreferenceJobMask;		// 수정

	memcpy(_lpInfo->m_aMemberList,m_aMemberList,sizeof(m_aMemberList));

	mRCI();
	strcpy(_lpInfo->m_strMasterName,m_strMasterName);
	mRCI();
	strcpy(_lpInfo->m_strPartyProspectus,m_strPartyProspectus);
	mCRCI();
}


BOOL
cPartyInfo::isJoinAbleLevelParty(int _iLevel)
{
	if	(m_bf3MemberCount	>=	dPARTY_MEMBER_COUNT)
		return	FALSE;

	int	iLevelGab	=	getAverageLevel()-_iLevel;

	if	(iLevelGab	<	0)
		iLevelGab	=	-iLevelGab;

	if	(iLevelGab	>	m_wRecruitMemberLevelRange)
		return	FALSE;

	return	TRUE;
}

void
cPartyInfo::copyAdvancedInfo(cPartyInfo *_lpInfo)
{
	memcpy(&m_wActiveMemberCount,&_lpInfo->m_wActiveMemberCount,sizeof(cPartyInfo)-sizeof(cPartyInfoForBCS));
}

void
cPartyInfo::resetAdvancedInfo()
{
	m_wRecruitMemberLevelRange		=	c_iDefaultPartyProspectusLevelRange;
	m_dwPreferenceJobMask			=	0;
	m_wActiveMemberCount			=	0;

	memset(m_awMemberSerial,0xff,sizeof(m_awMemberSerial));

	strcpy(m_strPartyProspectus," ");
}


void
cPartyInfo::changePartyProspectusText(char *_lpstrObject)
{
	memcpy(m_strPartyProspectus,_lpstrObject,c_iPartyProspectusLength);

	memset(m_strPartyProspectus+c_iPartyProspectusLength-2,0,2);
}

BOOL
cPartyInfo::addMember(int _iSerial)
{
	for	(int i=0;i<m_wActiveMemberCount;i++)
	{
		if	(m_awMemberSerial[i]	==	_iSerial)
			return	TRUE;
	}

	if	(m_wActiveMemberCount	>=	dPARTY_MEMBER_COUNT)
	{
		checkInvalidMember();

		if	(m_wActiveMemberCount	>=	dPARTY_MEMBER_COUNT)
			return	FALSE;
	}

	m_awMemberSerial[m_wActiveMemberCount++]	=	_iSerial;

	return	FALSE;
}

void
cPartyInfo::removeMember(int _iSerial)
{
	for	(int i=0;i<m_wActiveMemberCount;i++)
	{
		if	(m_awMemberSerial[i]	==	_iSerial)
		{
			for	(int j=i;j<m_wActiveMemberCount-1;j++)
				m_awMemberSerial[j]	=	m_awMemberSerial[j+1];

			m_awMemberSerial[m_wActiveMemberCount-1]	=	0xffff;
			m_wActiveMemberCount--;

			return;
		}
	}
}

BOOL
cPartyInfo::isPartyLeader(char *_lpstrName)
{
	if	(STRICMP(m_strMasterName,_lpstrName)	==	0)
		return	TRUE;

	return	FALSE;
}

void
cPartyInfo::checkInvalidMember()
{
	int		iRemoveActorCount	=	0;
	WORD	awRemoveActorList[dPARTY_MEMBER_COUNT];
	int		i;

	for(i=m_wActiveMemberCount;i;)
	{
		--i;

		CClient	*lpUser	= mCM.GetClientPnt(m_awMemberSerial[i]);

		if	(!lpUser	||	lpUser->m_wSerial == 0xffff || lpUser->m_isGameServer)
		{
			awRemoveActorList[iRemoveActorCount++]	=	m_awMemberSerial[i];

			continue;
		}

		if	(lpUser->m_wPartySerial	!=	m_wSerial)
		{
			lpUser->m_wIsRequireAddToParty			=	TRUE;
			awRemoveActorList[iRemoveActorCount++]	=	m_awMemberSerial[i];

			continue;
		}
	}

	for(i=iRemoveActorCount;i;)
	{
		--i;

		removeMember(awRemoveActorList[i]);
	}
}

void
cPartyInfo::sendPacketToClient(void *_lpPacket,int _iFieldSerial)
{
	int		iSize	=	((cMSG_BASE_TYPE *)_lpPacket)->wSize;
	int		iIncorrectMemberrCount	=	0;
	int		i;

	for(i=m_wActiveMemberCount;i;)
	{
		--i;

		CClient	*lpUser	= mCM.GetClientPnt(m_awMemberSerial[i]);

		if	(!lpUser	||	lpUser->m_wSerial == 0xffff || lpUser->m_isGameServer	||	lpUser->m_wPartySerial	!=	m_wSerial)
		{
			iIncorrectMemberrCount++;

			continue;
		}

		if	(_iFieldSerial	!=	0xffff	&&	_iFieldSerial	!=	lpUser->m_wCurrentField)
			continue;

		lpUser->AddSendPacket((char *)_lpPacket,iSize);
	}

	if	(iIncorrectMemberrCount)
		checkInvalidMember();
}

cPartyManager::cPartyManager()
{
	if	(s_bIsExist)
	{
		ERRMSG("error!!","cPartyManager::cPartyManager()");

		return;
	}

	m_denParty.init(dPARTY_COUNT);

	//	파티 리스트 전부를 받기 끝
	s_bIsExist					=	TRUE;
	m_bIsProcessUpdataPartyList	=	FALSE;
	m_bIsReceivePartyInfo		=	FALSE;

	InitializeCriticalSection(&m_csPartyWork);

	reset();
}

cPartyManager::~cPartyManager()
{
	DeleteCriticalSection(&m_csPartyWork);
}

void
cPartyManager::reset()
{
	m_denParty.reset();
}

cPartyInfo*	
cPartyManager::get(int _iSerial)
{
	if	(_iSerial	<	0	||	_iSerial	>=	dPARTY_COUNT)
		return	NULL;

	return	m_denParty.get(_iSerial);
}

BOOL
cPartyManager::addPartyMember(int _iParty,void *_lpClient)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	CCriticalSection	cs(&m_csPartyWork);

	cPartyInfo	*lpParty	=	get(_iParty);

	if	(!lpParty)
		return	FALSE;

	if	(!lpParty->addMember(lpClient->m_wSerial))
		return	FALSE;

	BC_UPDATE_PARTY_PROSPECTUS	packet;

	packet.base.set(sizeof(packet),dBC_UPDATE_PARTY_PROSPECTUS);

	memcpy(packet.strPartyProspectus,lpParty->m_strPartyProspectus,c_iPartyProspectusLength);
	packet.strPartyProspectus[c_iPartyProspectusLength-2]	=	NULL;
	packet.wPartySerial					=	lpParty->m_wSerial;
	packet.bf1IsIncludeText				=	TRUE;
	packet.dwPreferenceJobMask			=	lpParty->m_dwPreferenceJobMask;
	packet.bf10RecruitMemberLevelRange	=	lpParty->m_wRecruitMemberLevelRange;

	packet.base.wSize	=	sizeof(packet)-c_iPartyProspectusLength + strlen(packet.strPartyProspectus)+1;
	lpClient->AddSendPacket((char *)&packet,packet.base.wSize);

	return	TRUE;
}

void
cPartyManager::removePartyMember(int _iParty,int _iClient)
{
	CCriticalSection	cs(&m_csPartyWork);

	cPartyInfo	*lpParty	=	get(_iParty);

	if	(lpParty)
		lpParty->removeMember(_iClient);
}


//	파티 리스트 전부를 받기 시작
void
cPartyManager::beginReceivePartyList()
{
	m_bIsProcessUpdataPartyList	=	TRUE;
	
	reset();
}

//	파티 리스트 전부를 받기 끝
void
cPartyManager::endReceivePartyList()
{
	m_bIsProcessUpdataPartyList	=	FALSE;
	m_bIsReceivePartyInfo		=	TRUE;
}

void
cPartyManager::updatePartyInfo(cPartyInfo *_lpInfo,BOOL _bIsByUpdateListPacket)
{
	if	(_lpInfo->m_wSerial	==	0xffff)
		return;

	if	(_bIsByUpdateListPacket	==	FALSE)
	{
		if	(m_bIsProcessUpdataPartyList)
			return;

		if	(!m_bIsReceivePartyInfo)
			return;
	}

	CCriticalSection	cs(&m_csPartyWork);

	if	(_lpInfo->m_bf1IsValidParty)
		m_denParty.remove(_lpInfo->m_wSerial);
	else
	{
		cPartyInfo	info;

		info.copy(_lpInfo);

		cPartyInfo	*lpInfo	=	m_denParty.get(_lpInfo->m_wSerial);

		if	(lpInfo)
			info.copyAdvancedInfo(lpInfo);
		else
			info.resetAdvancedInfo();

		m_denParty.replace(info.m_wSerial,&info);
	}
}