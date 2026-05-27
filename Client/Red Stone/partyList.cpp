#include "CParty.h"
#include "CActor.h"
#include "cMessage.h"

void
CPatyInfo::resetPartyList()
{
	m_iPartyCount		=	0;
	m_iHeadPartySerial	=	dPARTY_COUNT-1;
	m_iLastPartySerial	=	0;
	m_iSelectParty		=	-1;

	resetNewRequestJoinPartyUserFlag();
}

void
CPatyInfo::receivePartyList(int _iPartyCount,cPartyInfoForPL *_lpList)
{
	m_iPartyCount		=	_iPartyCount;
	m_iSelectParty		=	-1;

	if	(_iPartyCount	<=	0)
	{
		resetPartyList();
		m_iPartyCount	=	-1;
		return;
	}

	memcpy(m_aPartyList,_lpList,sizeof(cPartyInfoForPL)*_iPartyCount);

	m_iLastPartySerial	=	m_aPartyList[_iPartyCount-1].m_wSerial+1;
	m_iHeadPartySerial	=	m_aPartyList[0].m_wSerial-1;

	if	(m_iHeadPartySerial	<	0)
		m_iHeadPartySerial	=	dPARTY_COUNT-1;
}

void
CPatyInfo::cRequestJoinParty::reset()
{
	m_iUserCount	=	0;
	m_iFocusUser	=	-1;
	m_strWorkUser[0]=	NULL;
	
	memset(m_aList,0,sizeof(m_aList));
}

void
CPatyInfo::cRequestJoinParty::update()
{
	for	(int i=0;i<m_iUserCount;)
	{
		if	(m_aList[i].m_dwAskTime	+ 60*10*1000 <=	timeGetTime() )
		{
			remove(i);

			continue;
		}

		i++;
	}
}

BOOL
CPatyInfo::cRequestJoinParty::add(char *_lpstrName,int _iJob,int _iLevel)
{
	int	iIndex	=		getIndexByName(_lpstrName);

	if	(iIndex	!=	0xffff)
		remove(iIndex);

	if	(m_iUserCount	>=	eRequestJoinPartyUserCount)
		remove(0);

	m_aList[m_iUserCount].m_dwAskTime	=	(DWORD)timeGetTime();
	m_aList[m_iUserCount].m_wJob		=	_iJob;
	m_aList[m_iUserCount].m_wLevel		=	_iLevel;

	memcpy(m_aList[m_iUserCount].m_strName,_lpstrName,dNAME_LENGTH);

	m_aList[m_iUserCount].m_strName[dNAME_LENGTH-1]	=	NULL;

	m_iUserCount++;

	return	TRUE;
}

BOOL
CPatyInfo::cRequestJoinParty::remove(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>=	eRequestJoinPartyUserCount)
		return	FALSE;

	if	(m_iUserCount	<=	0)
		return	FALSE;

	m_aList[m_iUserCount-1].reset();

	if	(m_iUserCount-_iIndex-1	>	0)
		memcpy(&m_aList[_iIndex],&m_aList[_iIndex+1],sizeof(cRequestJoinPartyUserInfo)*(m_iUserCount-_iIndex-1));

	m_iUserCount--;

	return	TRUE;
}

BOOL
CPatyInfo::cRequestJoinParty::remove(char *_lpstrName)
{
	int	iIndex	=	getIndexByName(_lpstrName);

	if	(iIndex	==	0xffff)
		return	FALSE;

	return	remove(iIndex);
}

BOOL
CPatyInfo::cRequestJoinParty::removeSelectUser()
{
	return	remove(m_strWorkUser);
}

int
CPatyInfo::cRequestJoinParty::getIndexByName(char *_lpstrName)
{
	for	(int i=m_iUserCount;i;)
	{
		--i;

		if	(STRICMP(m_aList[i].m_strName,_lpstrName)	==	0)
			return	i;
	}

	return	0xffff;
}

CPatyInfo::cRequestJoinPartyUserInfo*
CPatyInfo::cRequestJoinParty::getByName(char *_lpstrName)
{
	int	iIndex	=	getIndexByName(_lpstrName);

	if	(iIndex	==	0xffff)
		return	NULL;

	return	&m_aList[iIndex];
}

void
CPatyInfo::sendAcceptJoinPartyRequest()
{
	CGamePlay::s_agent.sendAskJoinPartyReply(m_requestJoinPartyUserList.m_strWorkUser,1,TRUE);
}