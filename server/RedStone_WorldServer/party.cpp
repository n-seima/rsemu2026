#include "party.h"
#include "Client.h"

CPartyManager	g_partyManager;

void
CPartyInfo::reset()
{
	m_wSerial			=	0xffff;
	m_wMemberCount		=	0;
	m_wGateField		=	0xffff;
	m_bf1IsUnionParty	=	FALSE;
	m_wUnionPartyserial	=	0xffff;
}

void
CPartyInfo::create(int _iSerial,char *_lpstrPartyName,char *_lpstrLeaderName,int _iField,int _iLevel,int _iJob,int _iRemainHP,int _iMasterSPC,
				   int _iZoneSerial,int _iInstanceField,int _iInstanceFieldFloor,int _iMasterDuelVP)
{
	m_wSerial				=	_iSerial;
	strcpy(m_strName,_lpstrPartyName);
	m_dwNameHashCode		=	GetHashCode((BYTE *)_lpstrPartyName);
	m_wMemberCount			=	0;
	m_wGateField			=	0xffff;

	m_bf10GateArea			=	0;
	m_bf3GoldShareMethod	=	ePGSM_EQUALITY;
	m_bf3ItemShareMethod	=	ePISM_RANDOM;
	m_bf3PartyObject		=	ePO_HUNT;	//파티 목적
	m_bf1IsOpenParty		=	FALSE;	//멤버를 계속 모집한다.

	joinMember(_lpstrLeaderName,_iField,_iLevel,_iJob,_iRemainHP,_iMasterSPC,_iZoneSerial,_iInstanceField,_iInstanceFieldFloor,_iMasterDuelVP);
}

void
CPartyInfo::changeName(char *_lpstrChangeName)
{
	strcpy(m_strName,_lpstrChangeName);
	m_dwNameHashCode	=	GetHashCode((BYTE *)_lpstrChangeName);
}

int
CPartyInfo::updateMemberInfo(char *_lpstrName,int _iLevel,int _iPSC,int _iJob ,int _iGDPC)
{
	for (int i=0;i<m_wMemberCount;i++)
		if (STRCMP(m_aMembers[i].m_strName,_lpstrName)	==	0)
		{
			m_aMembers[i].m_bf10Level				=	_iLevel;
			m_aMembers[i].m_bf4SecretDungeonPlayCount=	_iPSC;
			m_aMembers[i].m_bf8Job					=	_iJob;
			m_aMembers[i].m_bf3GuildDungeonPlayCount	=	_iGDPC;

			return	dPWR_SUCCESS;
		}

	return	dPWR_CAN_NOT_FIND_MEMBER;
}

CPartyMemberInfo*
CPartyInfo::getPartyMember(char *_lpstrName,DWORD _dwNameHashCode)
{
	for (int i=0;i<m_wMemberCount;i++)
	{
		if (m_aMembers[i].m_dwNameHashCode	==	_dwNameHashCode)
			if (STRCMP(m_aMembers[i].m_strName,_lpstrName)	==	0)
				return	&m_aMembers[i];
	}

	return	NULL;
}

int
CPartyInfo::joinMember(char *_lpstrName,int _iField,int _iLevel,int _iJob,int _iRemainHP,int _iSPC,int _iZoneSerial,
					int _iInstanceField,int _iInstanceFieldFloor,int _iDuelVP)
{
	int		iIndex		=	m_wMemberCount;
	BOOL	bIsReconnect=	FALSE;

	for (int i=0;i<m_wMemberCount;i++)
		if	(STRCMP(m_aMembers[i].m_strName,_lpstrName)	==	0)
		{
			iIndex		=	i;
			bIsReconnect=	TRUE;

			break;
		}

	if	(bIsReconnect	==	FALSE	&&	m_wMemberCount	>=	dPARTY_MEMBER_COUNT)
		return	dPWR_CAN_NOT_ADD_MEMBER_BY_TOO_MANY_MEMBER;

	m_aMembers[iIndex].m_bf10Level					=	_iLevel;
	m_aMembers[iIndex].m_dwLeaveFieldTime			=	0;
	m_aMembers[iIndex].m_bf8Job						=	_iJob;
	m_aMembers[iIndex].m_bf8RemainHP				=	_iRemainHP;
	m_aMembers[iIndex].m_iZoneSerial				=	_iZoneSerial;
	m_aMembers[iIndex].m_dwNameHashCode				=	GetHashCode((BYTE *)_lpstrName);
	m_aMembers[iIndex].m_wCurrentField				=	_iField;
	m_aMembers[iIndex].m_bf10InstanceField			=	_iInstanceField;
	m_aMembers[iIndex].m_bf4InstanceFieldFloor		=	_iInstanceFieldFloor;
	m_aMembers[iIndex].m_bf1IsDisconnected			=	FALSE;
	m_aMembers[iIndex].m_bf4SecretDungeonPlayCount	=	_iSPC;
	m_aMembers[iIndex].m_iDuelVP					=	_iDuelVP;

	strcpy(m_aMembers[iIndex].m_strName,_lpstrName);

	if	(bIsReconnect	==	FALSE)
		m_wMemberCount++;
	else
		return	dPWR_RECONNECT;

	return	dPWR_SUCCESS;
}

void
CPartyInfo::getSimpleInfo(CSimplePartyInfo *_lpInfo)
{
	for (int i=0;i<m_wMemberCount;i++)
		m_aMembers[i].getSimpleInfo(&_lpInfo->m_aMembers[i]);

	_lpInfo->m_wSerial				=	m_wSerial;					//	파티의 고유 시리얼
	_lpInfo->m_bf4MemberCount		=	m_wMemberCount;
	_lpInfo->m_dwUniqueSerial		=	m_dwUniqueSerial;
	_lpInfo->m_wGateField			=	m_wGateField;
	_lpInfo->m_bf10GateArea			=	m_bf10GateArea;
	_lpInfo->m_bf3GoldShareMethod	=	m_bf3GoldShareMethod;
	_lpInfo->m_bf3ItemShareMethod	=	m_bf3ItemShareMethod;
	_lpInfo->m_bf3PartyObject		=	m_bf3PartyObject;	//파티 목적
	_lpInfo->m_bf1IsOpenParty		=	m_bf1IsOpenParty;	//멤버를 계속 모집한다.

	strcpy(_lpInfo->m_strName,m_strName);
}

void
CPartyInfo::getInfoForBCS(cPartyInfoForBCS *_lpInfo)
{
	for (int i=0;i<m_wMemberCount;i++)
	{
		_lpInfo->m_aMemberList[i].m_bf10Level				=	m_aMembers[i].m_bf10Level;
		_lpInfo->m_aMemberList[i].m_bf8Job					=	m_aMembers[i].m_bf8Job;
	}

	strcpy(_lpInfo->m_strMasterName,m_aMembers[0].m_strName);

	_lpInfo->m_bf1IsValidParty		=	FALSE;
	_lpInfo->m_wSerial				=	m_wSerial;			//	파티의 고유 시리얼
	_lpInfo->m_bf3PartyObject		=	m_bf3PartyObject;	//	파티 목적
	_lpInfo->m_bf1IsOpenParty		=	m_bf1IsOpenParty;	//	멤버를 계속 모집한다.
	_lpInfo->m_bf3MemberCount		=	m_wMemberCount;		//	파티 멤버 수
}

int
CPartyInfo::getConnectedMemberCount()
{
	int	i,iAvailMemberCount=0;

	for (i=0;i<m_wMemberCount;i++)
		if (m_aMembers[i].m_bf1IsDisconnected	==	FALSE)
			iAvailMemberCount++;

	return	iAvailMemberCount;
}

void
CPartyInfo::disconnectMember(char *_lpstrName)
{
	int	i;

	for (i=0;i<m_wMemberCount;i++)
	{
		if (STRCMP(_lpstrName,m_aMembers[i].m_strName)==0)
		{
			m_aMembers[i].m_bf1IsDisconnected	=	TRUE;

			if (i==0)
			{
				for (i=1;i<m_wMemberCount;i++)
				{
					if	(m_aMembers[i].m_bf1IsDisconnected	==	FALSE)
					{
						CPartyMemberInfo	leader;

						memcpy(&leader,&m_aMembers[0],sizeof(CPartyMemberInfo));
						memcpy(&m_aMembers[0],&m_aMembers[i],sizeof(CPartyMemberInfo));
						memcpy(&m_aMembers[i],&leader,sizeof(CPartyMemberInfo));
					}
				}
			}

			break;
		}
	}
}

int
CPartyInfo::changeConnectStatus(char *_lpstrName)
{
	int	iMember	=	0xffff,i;

	for (i=0;i<m_wMemberCount;i++)
	{
		if (STRCMP(_lpstrName,m_aMembers[i].m_strName)==0)
		{
			iMember	=	i;

			break;
		}
	}

	if	(iMember	==	0xffff)
		return	dPWR_CAN_NOT_FIND_MEMBER;

	m_aMembers[iMember].m_bf1IsDisconnected	=	FALSE;

	return	dPWR_RECONNECT;
}

int
CPartyInfo::changeLeader(char *_lpstrName)
{
	int	iNewLeader	=	0xffff,i;

	for (i=0;i<m_wMemberCount;i++)
	{
		if (STRCMP(_lpstrName,m_aMembers[i].m_strName)==0)
		{
			iNewLeader	=	i;

			break;
		}
	}

	if (iNewLeader	==	0xffff)
		return	dPWR_CAN_NOT_FIND_MEMBER;

	CPartyMemberInfo	temp;

	memcpy(&temp,&m_aMembers[0],sizeof(CPartyMemberInfo));
	memcpy(&m_aMembers[0],&m_aMembers[iNewLeader],sizeof(CPartyMemberInfo));
	memcpy(&m_aMembers[iNewLeader],&temp,sizeof(CPartyMemberInfo));

	return	dPWR_SUCCESS;
}

int
CPartyInfo::leaveMember(char *_lpstrName)
{
	int	iRemoveMember	=	0xffff,i;

	for (i=0;i<m_wMemberCount;i++)
	{
		if	(STRCMP(_lpstrName,m_aMembers[i].m_strName)==0)
		{
			iRemoveMember	=	i;

			break;
		}
	}

	if	(iRemoveMember	==	0xffff)
		return	dPWR_CAN_NOT_FIND_MEMBER;

	for (i=iRemoveMember;i<m_wMemberCount-1;i++)
		m_aMembers[i].copy(&m_aMembers[i+1]);

	m_wMemberCount--;

	if	(m_aMembers[0].m_bf1IsDisconnected)
	{
		int	iActiveMember	=	-1;

		for (i=1;i<m_wMemberCount;i++)
		{
			if	(m_aMembers[i].m_bf1IsDisconnected	==	FALSE)
			{
				iActiveMember	=	i;

				break;
			}
		}

		if	(iActiveMember	!=	-1)
		{
			CPartyMemberInfo	newLeader;

			newLeader.copy(&m_aMembers[iActiveMember]);

			m_aMembers[iActiveMember].copy(&m_aMembers[0]);
			m_aMembers[0].copy(&newLeader);
		}
	}

	return	dPWR_SUCCESS;
}

int
CPartyInfo::leaveFieldMember(char *_lpstrName,int _iDestFieldSerial,int _iIFSerial,int _iIFFloor)
{
	int	i;

	for (i=0;i<m_wMemberCount;i++)
	{
		if	(STRCMP(_lpstrName,m_aMembers[i].m_strName)==0)
		{
			m_aMembers[i].m_dwLeaveFieldTime	=	timeGetTime();
			m_aMembers[i].m_wCurrentField		=	_iDestFieldSerial;
			m_aMembers[i].m_bf10InstanceField	=	_iIFSerial;
			m_aMembers[i].m_bf4InstanceFieldFloor=	_iIFFloor;

			return	dPWR_SUCCESS;
		}
	}

	return	dPWR_CAN_NOT_FIND_MEMBER;
}

int
CPartyInfo::enterFieldMember(char *_lpstrName,int _iDestFieldSerial,int _iZoneSerial,int _iIFSerial,int _iIFFloor)
{
	int	i;

	for (i=0;i<m_wMemberCount;i++)
	{
		if (STRCMP(_lpstrName,m_aMembers[i].m_strName)==0)
		{
			m_aMembers[i].m_dwLeaveFieldTime	=	0;
			m_aMembers[i].m_wCurrentField		=	_iDestFieldSerial;
			m_aMembers[i].m_iZoneSerial			=	_iZoneSerial;
			m_aMembers[i].m_bf10InstanceField	=	_iIFSerial;
			m_aMembers[i].m_bf4InstanceFieldFloor=	_iIFFloor;

			return	dPWR_SUCCESS;
		}
	}

	return	dPWR_CAN_NOT_FIND_MEMBER;
}

DWORD
CPartyInfo::getCheckSum()
{
	DWORD	dwCheckSum	=	0;

	dwCheckSum	+=	m_wMemberCount;
	dwCheckSum	+=	m_dwNameHashCode;

	for (int i=0;i<m_wMemberCount;i++)
	{
		dwCheckSum	+=	GetHashCode(m_aMembers[i].m_strName);
		dwCheckSum	-=	m_aMembers[i].m_wCurrentField;
	}

	dwCheckSum	+=	m_dwNameHashCode;

	return	dwCheckSum;
}


char*
CPartyInfo::getTooOldTimeMemberForWaitEnterField()
{
	int	iCurrentTime	=	timeGetTime();

	for (int i=0;i<m_wMemberCount;i++)
	{
		if	(m_aMembers[i].m_wCurrentField	==	0xffff)
			m_aMembers[i].m_wCurrentField	=	0xffff;

		if	(m_aMembers[i].m_dwLeaveFieldTime	==	0)
			continue;

		if	(m_aMembers[i].m_dwLeaveFieldTime	+	60*1000	>	timeGetTime())
			continue;

		return	m_aMembers[i].m_strName;
	}

	return	NULL;
}

void
CPartyInfo::disconnectField(int *_lpiFieldList,int _iFieldCount)
{
	int	i,j,iMemberCount	=	m_wMemberCount;

	for (i=0;i<_iFieldCount;i++)
		for (j=0;j<m_wMemberCount;j++)
			if	(m_aMembers[j].m_wCurrentField	==	_lpiFieldList[i])
			{
				m_aMembers[j].m_wCurrentField	=	0xffff;
				iMemberCount--;

				//_log("파티 disconnectField EXITPARTY %s:%s",m_strName,m_aMembers[j].m_strName);
			}

	for (i=0;i<m_wMemberCount;i++)
		if (m_aMembers[i].m_wCurrentField	==	0xffff)
		{
			for (j=i+1;j<m_wMemberCount;j++)
				if	(m_aMembers[j].m_wCurrentField	!=	0xffff)
				{
					m_aMembers[i].copy(&m_aMembers[j]);
					m_aMembers[j].m_wCurrentField	=	0xffff;
				}
		}

	m_wMemberCount	=	iMemberCount;
}

BOOL		CPartyManager::s_bIsExist	=	FALSE;

CPartyManager::CPartyManager()
{
	if (s_bIsExist)
	{
		MessageBox(NULL,"CPartyManager class is singletone","error in CPartyManager class constructor",MB_OK);

		return;
	}

	s_bIsExist			=	TRUE;
	m_dwLastUpdateTime	=	0;
	m_dwCreateSerial	=	0;

	reset();

	InitializeCriticalSection(&m_cs);
	
}

CPartyManager::~CPartyManager()
{
	DeleteCriticalSection(&m_cs);
}


void
CPartyManager::reset()
{
	m_iCount	=	0;
	m_iRookie	=	0;
}

CPartyInfo*
CPartyManager::getBySerial(int _iPartySerial)
{
	CCriticalSection	cs(&m_cs);
	
	return	&m_aParty[_iPartySerial];
}

CPartyInfo*
CPartyManager::getByUniqueSerial(DWORD _dwSerial)
{
	CCriticalSection	cs(&m_cs);

	for (int i=0,iCount=0;i<dPARTY_COUNT && iCount < m_iCount;i++)
	{
		if (m_aParty[i].m_wSerial	==	0xffff)
			continue;

		iCount++;

		if (m_aParty[i].m_dwUniqueSerial	==	_dwSerial)
			return	&m_aParty[i];
	}
	
	return	NULL;
}

CPartyInfo*
CPartyManager::getByName(char *_lpstrName)
{
	CCriticalSection	cs(&m_cs);

	DWORD	dwHashCode	=	GetHashCode((BYTE*)_lpstrName);

	for (int i=0,iCount=0;i<dPARTY_COUNT && iCount < m_iCount;i++)
	{
		if (m_aParty[i].m_wSerial	==	0xffff)
			continue;

		iCount++;

		if (m_aParty[i].m_dwNameHashCode	!=	dwHashCode)
			continue;

		if (stricmp(m_aParty[i].m_strName,_lpstrName) == 0)
			return	&m_aParty[i];
	}
	
	return	NULL;
}

int
CPartyManager::getPartySerial(char *_lpstrPartyName)
{
	CPartyInfo	*lpParty	=	getByName(_lpstrPartyName);

	if (lpParty)
		return	lpParty->m_wSerial;

	return	0xffff;	
}

int
CPartyManager::getPartySerial(DWORD _dwSerial)
{
	CPartyInfo	*lpParty	=	getByUniqueSerial(_dwSerial);

	if (lpParty)
		return	lpParty->m_wSerial;

	return	0xffff;	
}

int
CPartyManager::createParty(char *_lpstrPartyName,char *_lpstrLeaderName,int _iField,int _iLevel,int _iJob,int _iRemainHP,int _iMasterSPC,int _iMasterZoneSerial,
												char *_lpstrMemberName,int _iMemberField,int _iMemberLevel,int _iMemberJob,int _iMemberRemainHP,int _iMemberSPC,int _iMemberZoneSerial,
												int _iMasterDuelVP,int _iTargetDuelVP)
{
	if	(m_iCount	>=	dPARTY_COUNT)
		return	dPWR_PARTY_COUNT_FULL;

	if	(getByName(_lpstrPartyName))
		return	dPWR_EXIST_SAME_NAME_PARTY;

	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty=	&m_aParty[m_iRookie];

	lpParty->create(m_iRookie,_lpstrPartyName,_lpstrLeaderName,_iField,_iLevel,_iJob,_iRemainHP,_iMasterSPC,_iMasterZoneSerial,0,0,_iMasterDuelVP);
	lpParty->joinMember(_lpstrMemberName,_iMemberField,_iMemberLevel,_iMemberJob,_iMemberRemainHP,_iMemberSPC,_iMemberZoneSerial,0,0,_iTargetDuelVP);
	lpParty->m_dwUniqueSerial	=	m_dwCreateSerial;

	for (int i=m_iRookie+1;i<dPARTY_COUNT;i++)
	{
		if (m_aParty[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}
	}

	m_iCount++;

	m_dwCreateSerial++;

	sendUpdatePartyInfoToBCS(lpParty->m_wSerial);

	return	lpParty->m_wSerial;
}

int
CPartyManager::dissolutionParty(char *_lpstrPartyName)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty	=	getByName(_lpstrPartyName);

	if	(!lpParty)
		return	dPWR_CAN_NOT_FIND_PARTY;

	int	iSerial	=	lpParty->m_wSerial;

	if	(lpParty->m_wSerial	<	m_iRookie)
		m_iRookie	=	lpParty->m_wSerial;

	m_iCount--;

	lpParty->reset();

	sendUpdatePartyInfoToBCS(iSerial);

	return	dPWR_SUCCESS;
}

int
CPartyManager::dissolutionParty(int _iPartyIndex)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty	=	&m_aParty[_iPartyIndex];

	if	(lpParty->m_wSerial	==	0xffff)
		return	dPWR_CAN_NOT_FIND_PARTY;

	if (lpParty->m_wSerial	<	m_iRookie)
		m_iRookie	=	lpParty->m_wSerial;

	m_iCount--;

	lpParty->reset();

	sendUpdatePartyInfoToBCS(_iPartyIndex);

	return	dPWR_SUCCESS;
}

int
CPartyManager::joinPartyMember(char *_lpstrPartyName,char *_lpstrName,int _iField,int _iLevel,int _iJob,int _iRemainHP,int _iSPC,int _iZoneSerial,int _iIFSerial,int _iIFFloor,int _iVP)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty=	getByName(_lpstrPartyName);

	if	(!lpParty)
		return	dPWR_CAN_NOT_FIND_PARTY;

	int	iResult	=	lpParty->joinMember(_lpstrName,_iField,_iLevel,_iJob,_iRemainHP,_iSPC,_iZoneSerial,_iIFSerial,_iIFFloor,_iVP);

	if	(iResult	==	dPWR_SUCCESS)
		sendUpdatePartyInfoToBCS(lpParty->m_wSerial);

	return	iResult;
}

int
CPartyManager::operatePartyWork(int _iSerial,int _iWork,char *_lpstrName)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty=	getBySerial(_iSerial);

	if (!lpParty)
		return	dPWR_CAN_NOT_FIND_PARTY;

	switch(_iWork)
	{
		case	ePW_LEAVE_PARTY		:
		case	ePW_BANISH_MEMBER	:
		{
			int	iLeaveResult	=	lpParty->leaveMember(_lpstrName);

			if	(iLeaveResult	!=	dPWR_SUCCESS)
				return	iLeaveResult;

			if	(lpParty->m_wMemberCount	<=	1)
				return	dPWR_DISSOLUTION_PARTY;

			sendUpdatePartyInfoToBCS(_iSerial);

			return	dPWR_SUCCESS;
		}

		case	ePW_CHANGE_LEADER	:
		{
			int	iResult	=	lpParty->changeLeader(_lpstrName);

			if	(iResult	==	dPWR_SUCCESS)
				sendUpdatePartyInfoToBCS(_iSerial);

			return	iResult;
		}
			
		case	ePW_IS_CONNECT_USER	:
			return	lpParty->changeConnectStatus(_lpstrName);
	}

	return	dPWR_SUCCESS;
}

int
CPartyManager::changePartyName(char *_lpstrPartyName,char *_lpstrChangePartyName)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty=	getByName(_lpstrPartyName);

	if	(!lpParty)
		return	dPWR_CAN_NOT_FIND_PARTY;

	if	(getByName(_lpstrChangePartyName))
		return	dPWR_EXIST_SAME_NAME_PARTY;

	lpParty->changeName(_lpstrChangePartyName);

	sendUpdatePartyInfoToBCS(lpParty->m_wSerial);

	return	dPWR_SUCCESS;
}

int
CPartyManager::updateMemberInfo(DWORD _dwSerial,char *_lpstrName,int _iLevel,int _iPSC,int _iJob ,int _iGDPC)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty=	getByUniqueSerial(_dwSerial);

	if	(!lpParty)
		return	dPWR_CAN_NOT_FIND_PARTY;

	int	iResult	=	lpParty->updateMemberInfo(_lpstrName,_iLevel,_iPSC,_iJob,_iGDPC);

	if	(iResult	==	dPWR_SUCCESS)
		sendUpdatePartyInfoToBCS(lpParty->m_wSerial);

	return	iResult;
}

int
CPartyManager::leaveField(char *_lpstrPartyName,char *_lpstrName,int _iDestFieldSerial,int _iIFSerial,int _iIFFloor)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty=	getByName(_lpstrPartyName);

	if	(!lpParty)
		return	dPWR_CAN_NOT_FIND_PARTY;

	return	lpParty->leaveFieldMember(_lpstrName,_iDestFieldSerial,_iIFSerial,_iIFFloor);
}

int
CPartyManager::enterField(char *_lpstrPartyName,char *_lpstrName,int _iDestFieldSerial,int _iZoneSerial,int _iIFSerial,int _iIFFloor)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty=	getByName(_lpstrPartyName);

	if	(!lpParty)
		return	dPWR_CAN_NOT_FIND_PARTY;

	return	lpParty->enterFieldMember(_lpstrName,_iDestFieldSerial,_iZoneSerial,_iIFSerial,_iIFFloor);
}

int
CPartyManager::getInfoForBCS(int _iSerial,cPartyInfoForBCS *_lpInfo)
{
	_lpInfo->m_wSerial	=	_iSerial;

	if	(_iSerial	==	0xffff)
		return	dPWR_CAN_NOT_FIND_PARTY;

	CPartyInfo	*lpParty=	&m_aParty[_iSerial];

	if	(lpParty->m_wSerial	==	0xffff)
	{
		_lpInfo->m_bf1IsValidParty	=	TRUE;

		return	dPWR_CAN_NOT_FIND_PARTY;
	}

	lpParty->getInfoForBCS(_lpInfo);

	return	dPWR_SUCCESS;
}

int
CPartyManager::getSimpleInfoBySerial(int _iSerial,CSimplePartyInfo *_lpInfo)
{
	if	(_iSerial	==	0xffff)
		return	dPWR_CAN_NOT_FIND_PARTY;

	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty=	&m_aParty[_iSerial];

	if (lpParty->m_wSerial	==	0xffff)
		return	dPWR_CAN_NOT_FIND_PARTY;

	lpParty->getSimpleInfo(_lpInfo);

	return	dPWR_SUCCESS;
}

int
CPartyManager::getSimpleInfoByName(char *_lpstrPartyName,CSimplePartyInfo *_lpInfo)
{
	CCriticalSection	cs(&m_cs);

	CPartyInfo	*lpParty	=	getByName(_lpstrPartyName);

	if (!lpParty)
		return	dPWR_CAN_NOT_FIND_PARTY;

	lpParty->getSimpleInfo(_lpInfo);

	return	dPWR_SUCCESS;
}

void
CPartyManager::update()
{
	if	(m_dwLastUpdateTime	+	10*1000	>	timeGetTime() && m_dwLastUpdateTime	<	timeGetTime())
		return;

	m_dwLastUpdateTime	=	timeGetTime();

	CCriticalSection	cs(&m_cs);
	char				strExpiredMember[dNAME_LENGTH];

	for (int i=0;i<dMAX_PARTY_COUNT;i++)
	{
		CPartyInfo	*lpParty	=	&m_aParty[i];

		if	(lpParty->m_wSerial	==	0xffff)
			continue;

		char	*lpstrExpiredMember	=	lpParty->getTooOldTimeMemberForWaitEnterField();

		if	(lpstrExpiredMember)
		{
			strcpy(strExpiredMember,lpstrExpiredMember);

			lpParty->disconnectMember(strExpiredMember);

			sendDisconnectPartyMemberPacket(NULL,lpParty->m_strName,i,strExpiredMember);
		}

		if	(lpParty->m_wMemberCount	<=	1)
		{
			char	strPartyName[dPARTYNAME_LENGTH];

			strcpy(strPartyName,lpParty->m_strName);
//	_log("파티 update 한명짜리 파티라 해체 [%s]",lpParty->m_strName);

			if	(lpParty->m_wSerial	<	m_iRookie)
				m_iRookie	=	lpParty->m_wSerial;

			m_iCount--;

			lpParty->reset();

			sendUpdatePartyInfoToBCS(i);

			sendDissolutionPartyPacket(NULL,strPartyName);
		}
	}
}

//
//	모든 파티 정보를 업데이트 시켜준다. (갓 들어온 겜 서버에게 전송해줌)
void
CPartyManager::updateAllPartyInfo(void *_lpServer)
{
	CCriticalSection	cs(&m_cs);

	for (int i=0;i<dPARTY_COUNT;i++)
	{
		if	(m_aParty[i].m_wSerial	==	0xffff)
			continue;

		sendUpdatePartyInfoPacket(_lpServer,i,TRUE);
	}
}

//
//	해당 필드에 있다고 되어 있는 멤버들 전부 파탈 시킴. (서버가 접속이 끊겼을때 사용)
void
CPartyManager::disconnectField(int *_lpiFieldList,int _iFieldCount)
{
	CCriticalSection	cs(&m_cs);

	//_log("파티 disconnectField");
	
	for (int i=0;i<dPARTY_COUNT;i++)
	{
		if	(m_aParty[i].m_wSerial	==	0xffff)
			continue;

		m_aParty[i].disconnectField(_lpiFieldList,_iFieldCount);

		if	(m_aParty[i].m_wMemberCount	<=	1)
		{
			dissolutionParty(i);

			sendUpdatePartyInfoToBCS(i);
		}
	}
}

CPartyInfo*
CPartyManager::getByPlayer(char *_lpstrName)
{
	CCriticalSection	cs(&m_cs);
	DWORD				dwHashCode	=	GetHashCode(_lpstrName);

	for (int i=0,iCount=0;i<dPARTY_COUNT && iCount < m_iCount;i++)
	{
		if	(m_aParty[i].m_wSerial	==	0xffff)
			continue;

		if	(m_aParty[i].getPartyMember(_lpstrName,dwHashCode))
			return	&m_aParty[i];

		iCount++;
	}

	return	NULL;
}

void
CPartyManager::disconnectMember(char *_lpstrName)
{
	CCriticalSection	cs(&m_cs);
	DWORD				dwHashCode	=	GetHashCode(_lpstrName);

	for (int i=0,iCount=0;i<dPARTY_COUNT && iCount < m_iCount;i++)
	{
		if	(m_aParty[i].m_wSerial	==	0xffff)
			continue;

		if	(m_aParty[i].getPartyMember(_lpstrName,dwHashCode))
		{
			m_aParty[i].disconnectMember(_lpstrName);

			if	(m_aParty[i].getConnectedMemberCount()	<=	0)
			{
				char	strPartyName[32];

				strcpy(strPartyName,m_aParty[i].m_strName);

				dissolutionParty(strPartyName);
				sendDissolutionPartyPacket(NULL,strPartyName);
			}
			else
				sendDisconnectPartyMemberPacket(NULL,m_aParty[i].m_strName,i,_lpstrName);

			sendUpdatePartyInfoToBCS(i);

			return;
		}

		iCount++;
	}
}

void
CPartyManager::removePartyMember(char *_lpstrName)
{
	CCriticalSection	cs(&m_cs);
	DWORD				dwHashCode	=	GetHashCode(_lpstrName);

	for (int i=0,iCount=0;i<dPARTY_COUNT && iCount < m_iCount;i++)
	{
		if	(m_aParty[i].m_wSerial	==	0xffff)
			continue;

		iCount++;
		
		if	(!m_aParty[i].getPartyMember(_lpstrName,dwHashCode))
			continue;

		DWORD	dwPartyUniqueSerial	=	m_aParty[i].m_dwUniqueSerial;
		int		iPartySerial		=	m_aParty[i].m_wSerial;
		char	strPartyName[dPARTYNAME_LENGTH];
		strcpy(strPartyName,m_aParty[i].m_strName);

		int		iWorkResult			=	operatePartyWork(iPartySerial,ePW_LEAVE_PARTY,_lpstrName);

		switch(iWorkResult)
		{
			case	dPWR_DISSOLUTION_PARTY		:
				sendPartyWorkPacket(NULL,dwPartyUniqueSerial,_lpstrName,ePW_LEAVE_PARTY);
				sendDissolutionPartyPacket(NULL,strPartyName);
				dissolutionParty(strPartyName);
				return;
		}
 
		sendPartyWorkPacket(NULL,dwPartyUniqueSerial,_lpstrName,ePW_LEAVE_PARTY);
		sendUpdatePartyInfoPacket(NULL,iPartySerial);

		return;
	}
}
