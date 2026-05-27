#include "CParty.h"
#include "CActor.h"
#include "cMessage.h"
#include "CGamePlay.h"

CUnionPartyInfo		s_unionPartyInfo;

BOOL
CPatyInfo::isExistNewRequestJoinPartyUser()
{
	return	m_bIsReceiveNewJoinPartyRequest;
}

void
CPatyInfo::resetNewRequestJoinPartyUserFlag()
{
	m_bIsReceiveNewJoinPartyRequest	=	FALSE;
}

void
CPatyInfo::reset()
{
	m_wIsWaitPartyInfo				=	FALSE;
	m_wMemberCount					=	0;
	m_wSelectMember					=	0xffff;
	m_wFocusMember					=	0xffff;
	m_wSerial						=	0xffff;
	m_bIsReceiveNewJoinPartyRequest	=	FALSE;
	m_dwPreferenceJobMask			=	0;		// 수정

	m_wRecruitMemberRange			=	75;
	memset(m_strPartyProspectus,0,sizeof(m_strPartyProspectus));

	m_requestJoinPartyUserList.reset();

	resetNewRequestJoinPartyUserFlag();
	//ResetUnionParty();
}

//
//	멤버 추가
void
CPatyInfo::addMember(int _iIndex,char *_lpstrName,int _iJob,int _iLevel,int _iSerial,int _iFieldSerial,int _iIF,int _iIFFloor,int _iSDPC,int _iDuelVP)
{
	int		iIndex			=	m_wMemberCount;
	BOOL	bIsReconnect	=	FALSE;
	int		iMemberIndex	=	getMemberIndex(_lpstrName);	//	이름으로  멤버를 구한다.

	if	(iMemberIndex	!=	0xffff)
	{
		iIndex			=	iMemberIndex;
		bIsReconnect	=	TRUE;
	}

	m_requestJoinPartyUserList.remove(_lpstrName);

	if	(bIsReconnect	==	FALSE)
		m_wMemberCount++;

	strcpy(m_aMemberList[_iIndex].m_strName,_lpstrName);

	m_aMemberList[_iIndex].m_wJob					=	_iJob;
	m_aMemberList[_iIndex].m_wLevel					=	_iLevel;
	m_aMemberList[_iIndex].m_wSerial				=	_iSerial;
	m_aMemberList[_iIndex].m_wHP					=	100;
	m_aMemberList[_iIndex].m_wFieldSerial			=	_iFieldSerial;
	m_aMemberList[_iIndex].m_wNameBarSizeAtField	=	0;
	m_aMemberList[_iIndex].m_wStatusField			=	0;
	m_aMemberList[_iIndex].m_wInstanceField			=	_iIF;
	m_aMemberList[_iIndex].m_wInstanceFieldFloor	=	_iIFFloor;
	m_aMemberList[_iIndex].m_wIsDisconnected		=	FALSE;
	m_aMemberList[_iIndex].m_wPlaySecretDungeonCount=	_iSDPC;
	m_aMemberList[_iIndex].m_iDuelVP				=	_iDuelVP;

	if	(_iSerial		!=	0xffff)
	{
		CActor	*lpActor=	g_am.getTestedActor(_iSerial);

		if	(lpActor)
			lpActor->m_wLevel=	_iLevel;
	}
}

int
CPatyInfo::getAverageLevel()
{
	if	(m_wMemberCount	==	0)
		return	0;

	int	iLevelSum	=	0;

	for (int i=0;i<m_wMemberCount;i++)
		iLevelSum	+=	m_aMemberList[i].m_wLevel;

	return	iLevelSum/m_wMemberCount;
}

void
CPatyInfo::updatePartyProspectus(int _iRange,DWORD _dwMask)
{	
	m_wRecruitMemberRange	=	_iRange;
	m_dwPreferenceJobMask	=	_dwMask;		
}


void
CPatyInfo::changePartyProspectusText(char *_lpstrText)
{
	memcpy(m_strPartyProspectus,_lpstrText,c_iPartyProspectusLength);

	m_strPartyProspectus[c_iPartyProspectusLength-2]	=	NULL;
}

//
//	멤버 제거
void
CPatyInfo::removeMember(int _iIndex)
{
	if	(_iIndex	>=	m_wMemberCount)
		return;
	
	for (int i=_iIndex;i<m_wMemberCount-1;i++)
		memcpy(&m_aMemberList[i],&m_aMemberList[i+1],sizeof(CPartyMemberInfo));

	m_wMemberCount--;

	if	(m_aMemberList[0].m_wIsDisconnected)
	{
		int	iActiveMember	=	-1;

		for (i=1;i<m_wMemberCount;i++)
		{
			if	(m_aMemberList[i].m_wIsDisconnected	==	FALSE)
			{
				iActiveMember	=	i;

				break;
			}
		}

		if	(iActiveMember	!=	-1)
		{
			CPartyMemberInfo	newLeader;

			memcpy(&newLeader,&m_aMemberList[iActiveMember],sizeof(CPartyMemberInfo));
			memcpy(&m_aMemberList[iActiveMember],&m_aMemberList[0],sizeof(CPartyMemberInfo));
			memcpy(&m_aMemberList[0],&newLeader,sizeof(CPartyMemberInfo));
		}
	}

	m_wFocusMember	=	0xffff;
}

void
CPatyInfo::changeConnectStatus(int _iIndex,BOOL _bIsDisconnect)
{
	if	(_iIndex	>=	m_wMemberCount)
		return;

	m_aMemberList[_iIndex].m_wIsDisconnected	=	_bIsDisconnect;
}

void
CPatyInfo::changeLeader(int _iIndex)
{
	if	(_iIndex	>=	m_wMemberCount)
		return;

	CPartyMemberInfo	temp;

	memcpy(&temp,&m_aMemberList[0],sizeof(CPartyMemberInfo));
	memcpy(&m_aMemberList[0],&m_aMemberList[_iIndex],sizeof(CPartyMemberInfo));
	memcpy(&m_aMemberList[_iIndex],&temp,sizeof(CPartyMemberInfo));

	m_wFocusMember	=	0xffff;
}


//
//	포커스가 간 멤버 찾아서 리턴(필드 안에 시리얼)
int
CPatyInfo::getFocusMember()
{
	if (m_wFocusPet	!=	0xffff)
	{
		CActor	*lpPet			=	g_hero.getPetActor(m_wFocusPet);

		if (lpPet)
			return	lpPet->m_wSerial;
	}
	if (m_wFocusSummonBeast	!=	0xffff)
	{
		CActor	*lpSummonBeast	=	g_hero.getSummonBeastActor(m_wFocusSummonBeast);

		if (lpSummonBeast)
			return	lpSummonBeast->m_wSerial;
	}

	if (m_wFocusMember	!=	0xffff)
		return	m_aMemberList[m_wFocusMember].m_wSerial;

	return	0xffff;
}

//	_lpstrName이름을 갖는 멤버 인덱스 구하기
int
CPatyInfo::getMemberIndex(char *_lpstrName)
{
	for (int i=0;i<m_wMemberCount;i++)
	{
		if (STRICMP(m_aMemberList[i].m_strName,_lpstrName) == 0)
			return	i;
	}

	return	0xffff;
}

BOOL
CPatyInfo::checkDisconnectLeader()
{
	if (m_aMemberList[0].m_wIsDisconnected	==	FALSE)
		return	FALSE;

	for (int i=1;i<m_wMemberCount;i++)
	{
		if(m_aMemberList[i].m_wIsDisconnected	==	FALSE)
		{
			CPartyMemberInfo	leader;

			memcpy(&leader,&m_aMemberList[0],sizeof(CPartyMemberInfo));
			memcpy(&m_aMemberList[0],&m_aMemberList[i],sizeof(CPartyMemberInfo));
			memcpy(&m_aMemberList[i],&leader,sizeof(CPartyMemberInfo));
		}
	}

	return	TRUE;
}

//
//	멤버 정보 설정
void
CPatyInfo::setMemberInfo(int _iIndex,char *_lpstrName,int _iJob,int _iLevel,int _iSerial,int _iFieldSerial,BOOL _bIsFirst,int _iIF,int _iIFFloor,BOOL _bIsDisconnected,int _iSDPC,int _iDuelVP)
{
	CPartyMemberInfo	*lpMember	=	&m_aMemberList[_iIndex];

	strcpy(lpMember->m_strName,_lpstrName);		//	이름

	lpMember->m_wJob					=	_iJob;	//	직업
	lpMember->m_wLevel					=	_iLevel;	//	레벨
	lpMember->m_wSerial					=	_iSerial;	//	필드에서의 시리얼(같은 필드가 아닐때는 0xffff)
	lpMember->m_wFieldSerial			=	_iFieldSerial;	//	필드시리얼
	lpMember->m_wInstanceField			=	_iIF;
	lpMember->m_wInstanceFieldFloor		=	_iIFFloor;
	lpMember->m_wIsDisconnected			=	_bIsDisconnected;
	lpMember->m_wPlaySecretDungeonCount	=	_iSDPC;
	lpMember->m_iDuelVP					=	_iDuelVP;

	if	(_iSerial		!=	0xffff)
	{
		CActor	*lpActor=	g_am.getTestedActor(_iSerial);

		if	(lpActor)
			lpActor->m_wLevel	=	_iLevel;
	}

	if	(_bIsFirst)
	{
		lpMember->m_wHP					=	100;
		lpMember->m_wNameBarSizeAtField	=	0;
	}

	if	(g_lpHero	&&	_iSerial	==	g_lpHero->m_wSerial)
		lpMember->m_wHP	=	g_hero.m_iHP*100/g_hero.getMaxHP();

	m_wFocusMember		=	0xffff;

}

//
//	액터 시리얼로 멤버를 구한다.
CPartyMemberInfo*
CPatyInfo::getMemberByActorSerial(int _iSerial)
{
	for (int i=0;i<m_wMemberCount;i++)
		if (m_aMemberList[i].m_wSerial	==	_iSerial)
			return	&m_aMemberList[i];

	return	NULL;
}

//
//	이름으로  멤버를 구한다.
CPartyMemberInfo*
CPatyInfo::getMemberByName(char *_lpstrName)
{
	for (int i=0;i<m_wMemberCount;i++)
		if (STRICMP(m_aMemberList[i].m_strName,_lpstrName)	==	0)
			return	&m_aMemberList[i];

	return	NULL;
}

//
//	파티 정보에 문제가 있다.
BOOL
CPatyInfo::isIncorrectData()
{
	if	(m_wIsWaitPartyInfo)
		return	FALSE;
	if	(m_wMemberCount	==	0)
		return	FALSE;

	if (!isPartyMember(g_lpHero->m_wSerial))
		return	TRUE;

	return	FALSE;
}

char*
CPatyInfo::getPartyStatusString(int _iMethod)
{
	if (_iMethod	==	-1)
		_iMethod	=	m_bf1IsOpenParty;

	if	(_iMethod	==	0)
		return	dMSG_DO_NOT_RAISE_PARTY_MEMBER;

	return	dMSG_RAISE_PARTY_MEMBER;
}

char*
CPatyInfo::getItemSharingMethodString(int _iMethod)
{
	if (_iMethod	==	-1)
		_iMethod	=	m_bf3ItemShareMethod;

	if (_iMethod	==	ePISM_FREE)
		return	dMSG_FREE_ACQUISITION;
	else
	if (_iMethod	==	ePISM_RANDOM)
		return	dMSG_RANDOM_ACQUISITION;
	else
	if (_iMethod	==	ePISM_ORDER)
		return	dMSG_ORDER_ACQUISITION;
	else
	if (_iMethod	==	ePISM_FIRST_JOB_RANDOM)
		return	dMSG_FIRST_JOB_RANDOM_ACQUISITION;
	else
	if (_iMethod	==	ePISM_FIRST_JOB_ORDER)
		return	dMSG_FIRST_JOB_ORDER_ACQUISITION;

	return	" ";
}

char*
CPatyInfo::getGoldSharingMethodString(int _iMethod)
{
	if (_iMethod	==	-1)
		_iMethod	=	m_bf3GoldShareMethod;

	if (_iMethod	==	ePGSM_FREE)
		return	dMSG_FREE_ACQUISITION;
	else
	if (_iMethod	==	ePGSM_EQUALITY)
		return	dMSG_EQUALITY_SHARING;
	else
	if (_iMethod	==	ePGSM_ORDER)
		return	dMSG_ORDER_ACQUISITION;
	
	return	" ";
}

char*
CPatyInfo::getPartyObjectString(int _iMethod)
{
	if (_iMethod	==	-1)
		_iMethod	=	m_bf3PartyObject;

	if (_iMethod	==	ePO_HUNT)
		return	dMSG_HUNTING;
	else
	if (_iMethod	==	ePO_EXPLORATION)
		return	dMSG_EXPLORATION_SECRET_DUNGEON;
	else
	if (_iMethod	==	ePO_QUEST)
		return	dMSG_OPERATE_QUEST;

	return	" ";
}
void
CPatyInfo::sendChangPreferenceJobMask(DWORD _dwPreferenceJobMask)
{
	if	(m_wMemberCount	==	0)
		return;

	CB_CHANGE_PARTY_PROSPECTUS	packet;

	packet.base.set(sizeof(packet),dCB_CHANGE_PARTY_PROSPECTUS);

	packet.wPartySerial					=	m_wSerial;
	packet.dwPreferenceJobMask			=	_dwPreferenceJobMask;
	packet.bf1IsChangeText				=	FALSE;
	packet.bf10RecruitMemberLevelRange	=	m_wRecruitMemberRange;

	packet.base.wSize					=	sizeof(packet)-sizeof(packet.strPartyProspectus);

	CGamePlay::s_agent.sendBCSPacket(&packet);
}
void
CPatyInfo::sendChangRangeOfRecruitPartyMemberLevel(int _iLevelRange)
{
	if	(m_wMemberCount	==	0)
		return;

	CB_CHANGE_PARTY_PROSPECTUS	packet;

	packet.base.set(sizeof(packet),dCB_CHANGE_PARTY_PROSPECTUS);

	packet.wPartySerial					=	m_wSerial;
	packet.dwPreferenceJobMask			=	m_dwPreferenceJobMask;
	packet.bf1IsChangeText				=	FALSE;
	packet.bf10RecruitMemberLevelRange	=	_iLevelRange;

	packet.base.wSize					=	sizeof(packet)-sizeof(packet.strPartyProspectus);

	CGamePlay::s_agent.sendBCSPacket(&packet);
}

void
CPatyInfo::sendChangPartyProspectusText(char *_lpText)
{
	if	(m_wMemberCount	==	0)
		return;

	CB_CHANGE_PARTY_PROSPECTUS	packet;

	packet.base.set(sizeof(packet),dCB_CHANGE_PARTY_PROSPECTUS);

	packet.wPartySerial					=	m_wSerial;
	packet.dwPreferenceJobMask			=	m_dwPreferenceJobMask;
	packet.bf1IsChangeText				=	TRUE;
	packet.bf10RecruitMemberLevelRange	=	m_wRecruitMemberRange;

	memcpy(packet.strPartyProspectus,_lpText,c_iPartyProspectusLength);
	packet.strPartyProspectus[c_iPartyProspectusLength-2]	=	NULL;

	CGamePlay::s_agent.sendBCSPacket(&packet);
}


void
CPatyInfo::sendAskPartyProspectus()
{
	if	(m_wMemberCount	==	0)
		return;

	CB_ASK_PARTY_PROSPECTUS	packet;

	packet.base.set(sizeof(packet),dCB_ASK_PARTY_PROSPECTUS);

	CGamePlay::s_agent.sendBCSPacket(&packet);
}

void
CUnionPartyInfo::updateActorList()
{
	WORD wPartyIndex	= 0xffff;
	WORD wPlayerIndex	= 0xffff;

	for (int i = 0 ; i < 128 ; i++)
	{
		int iWantUpdateIndex			= getupdateActorListIndexWantupdate();
		WORD wIndex						= m_waWantupdateActor[iWantUpdateIndex];
		//int	iIsexistInList		= getPlaceInupdateActorList(iFreeIndex);

		m_waWantupdateActor[i] = 0xffff;

		if(iWantUpdateIndex == 0xffffffff)
			break;

		CActor*		lpActor	=	&g_aActor[wIndex];

		if(!lpActor)
			continue;

		getByName(lpActor->m_strName, &wPartyIndex , &wPlayerIndex);

		if(wPartyIndex == 0xffff || wPlayerIndex == 0xffff)
			continue;

		m_wUnionPartyInfo[wPartyIndex][wPlayerIndex].m_wHP		= (lpActor->m_iHP / (float)lpActor->m_iMaxHP)*100;

		//m_wUnionPartyInff[wPartyIndex][wPlayerIndex].m_dwState	= lpActor->
	}
}

void 
CUnionPartyInfo::getByName(char* _strName , WORD* _wpPartyindex , WORD* _wpPlayerindex)
{
	for (int i =0 ; i < dUNION_PARTY_COUNT ;i++)
	{
		for (int j = 0 ; j < dPARTY_MAXPLAYER ; j++)
		{
			if(m_wUnionPartyInfo[i][j].m_strName == NULL)
				return;

			if(STRICMP(m_wUnionPartyInfo[i][j].m_strName , _strName) == 0)
			{
				*_wpPartyindex	= i;
				*_wpPlayerindex	= j;
				return;
			}
		}
	}
}

int
CUnionPartyInfo::isUnionPartyMember(WORD wIndex)
{
 	char* strName = g_aActor[wIndex].m_strName;
 
	for (int i = 0 ; i < dUNION_PARTY_COUNT ; i++)
	{
	 	for (int j = 0 ; j < dPARTY_MAXPLAYER ; j++)
	 	{
	 		if(STRICMP(strName , m_wUnionPartyInfo[i][j].m_strName) == 0)
	 			return (i * 10 + j);
	 	}
	}

	return 0xffffffff;
}