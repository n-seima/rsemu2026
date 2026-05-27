#include "party.h"
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_BCSERVER.h"
#include "CLient.h"

void
CPartyManager::sendChatPacket(void *_lpClient,int _iPartySerial,char *_lpstrPartyName,char *_lpstrMessage,char *_lpstrMember)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_PARTY_CHAT		packet;

	packet.base.set(sizeof(WORLDtoSERVERS_PARTY_CHAT),dWORLDtoSERVERS_PARTY_CHAT);
	packet.wPartySerial	=	_iPartySerial;
	strcpy(packet.strMsg,_lpstrMessage);
	strcpy(packet.strSendID,_lpstrMember);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::operateGuildMemberJoinParty(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY	*lpPacket=	(SERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY	*)_lpPacketData;

	CClient		*lpTargetServer	=	GetClientServer(lpPacket->strTargetName);
	
	if(!lpTargetServer)
	{
		sendGuildMemberJoinPartyAnswer(_lpClient, lpPacket->strAskerName,lpPacket->strTargetName, eGMJPA_HAVE_NO_TARGET);
		return;
	}

	sendGuildMemberJoinParty(lpTargetServer,lpPacket->strAskerName,lpPacket->strTargetName,lpPacket->wAskerJob,lpPacket->wAskerLevel);
}



void
CPartyManager::sendGuildMemberJoinParty(void *_lpClient,char *_lpstrAskerName,char *_lpstrTargetName,int _iJob,int _iLevel)
{
	CClient		*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY);

	packet.wAskerLevel	=	_iLevel;
	packet.wAskerJob	=	_iJob;

	strcpy(packet.strTargetName,_lpstrTargetName);
	strcpy(packet.strAskerName,_lpstrAskerName);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::operateGuildMemberJoinPartyAnswer(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY_ANSWER	*lpPacket=	(SERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY_ANSWER	*)_lpPacketData;

	CClient		*lpAskServer	=	GetClientServer(lpPacket->strAskerName);
	CClient		*lpTargetServer	=	GetClientServer(lpPacket->strTargetName);
	if	(!lpAskServer || !lpTargetServer)
	{
		sendGuildMemberJoinPartyAnswer(_lpClient, lpPacket->strAskerName,lpPacket->strTargetName, eGMJPA_FAILED);
		return;
	}
	if(!lpPacket->wAnswer)
	{	// 거부했다..
		sendGuildMemberJoinPartyAnswer(_lpClient, lpPacket->strAskerName,lpPacket->strTargetName, eGMJPA_DENY);
		return;
	}
	Avatar_Data*	lpAskAvatar = GetAvatar(lpPacket->strAskerName);
	Avatar_Data*	lpTargetAvatar = GetAvatar(lpPacket->strTargetName);

	if	(!lpAskAvatar || !lpTargetAvatar)
	{
		sendGuildMemberJoinPartyAnswer(_lpClient, lpPacket->strAskerName,lpPacket->strTargetName, eGMJPA_FAILED);
		return;
	}
	CPartyInfo	*lpParty	=	getByPlayer(lpPacket->strAskerName);
	int iSerial				=	getPartySerial(lpPacket->strAskerName);
	CPartyInfo	*lpTargetParty	=	getByPlayer(lpPacket->strTargetName);

	if(lpTargetParty)
	{	// 대상이 파티가 있다.
		sendGuildMemberJoinPartyAnswer(_lpClient, lpPacket->strAskerName,lpPacket->strTargetName, eGMJPA_TARGET_IS_IN_PARTY);
		return;
	}

	if(!lpParty)
	{

		SERVERStoWORLD_CREATE_PARTY	creatParty;
		
		creatParty.bf10MasterLevel	=	lpAskAvatar->data.m_iLevel;
		creatParty.bf8MasterJob		=	lpAskAvatar->data.m_wJob;
		creatParty.bf8MasterRemainHP=	lpAskAvatar->data.m_iHP*100/lpAskAvatar->data.m_iMaxHP;
		creatParty.iMasterZoneSerial=	0;
		creatParty.bf4MasterSecretDungeonPlayCount	=	lpAskAvatar->data.m_bf4SecretDungeonPlayCount;
		creatParty.iMasterDuelVP	=	0;
		strcpy(creatParty.strName,lpAskAvatar->data.m_strName);
		
		creatParty.bf10TargetLevel	=	lpTargetAvatar->data.m_iLevel;
		creatParty.bf8TargetJob		=	lpTargetAvatar->data.m_wJob;
		creatParty.bf8TargetRemainHP=	lpTargetAvatar->data.m_iHP*100/lpTargetAvatar->data.m_iMaxHP;
		creatParty.iTargetZoneSerial=	0;
		creatParty.bf4TargetSecretDungeonPlayCount	=	lpTargetAvatar->data.m_bf4SecretDungeonPlayCount;
		creatParty.iTargetDuelVP	=	0;
		strcpy(creatParty.strTName,lpTargetAvatar->data.m_strName);
		
		creatParty.wFieldSerial		=	lpAskAvatar->data.m_wCurrentField;

		operateCreatePartyPacket(_lpClient,(char* )&creatParty);
	}
	else
	{
		if(lpParty->m_wMemberCount >= dPARTY_MEMBER_COUNT)
		{
			sendGuildMemberJoinPartyAnswer(_lpClient, lpPacket->strAskerName,lpPacket->strTargetName, eGMJPA_PARTY_IS_FULL);
			return;
		}

		SERVERStoWORLD_JOIN_PARTY	joinParty;
		

		joinParty.wPartySerial					=	lpParty->m_wSerial;	//	파티 시리얼
		joinParty.bf1IsReconnect				=	FALSE;
		joinParty.bf1IsByJoinPartyApplication	=	FALSE;
		
		joinParty.wFieldSerial					=	lpTargetAvatar->data.m_wCurrentField;
		joinParty.iZoneSerial					=	0;
		joinParty.bf8Job						=	lpTargetAvatar->data.m_wJob;
		joinParty.bf10Level					=	lpTargetAvatar->data.m_iLevel;
		joinParty.bf10InstanceField			=	lpTargetAvatar->data.m_bf8InstanceField;
		joinParty.bf4InstanceFieldFloor		=	lpTargetAvatar->data.m_bf4InstanceFieldFloor;
		joinParty.bf7RemainHP					=	lpTargetAvatar->data.m_iHP*100/lpAskAvatar->data.m_iMaxHP;
		joinParty.bf4SecretDungeonPlayCount	=	lpTargetAvatar->data.m_bf4SecretDungeonPlayCount;
		joinParty.iDuelVP						=	0;
		
		strcpy(joinParty.strName,lpTargetAvatar->data.m_strName);
		strcpy(joinParty.strPartyName,lpParty->m_strName);
	
		operateJoinPartyPacket(_lpClient, (char* )&joinParty);

	}
}

void
CPartyManager::sendGuildMemberJoinPartyAnswer(void *_lpClient,char *_lpstrAskerName,char *_lpstrTargetName,int _iAnswer)
{
	CClient		*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY_ANSWER	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY_ANSWER);
	packet.wAnswer	=	_iAnswer;

	strcpy(packet.strAskerName,_lpstrAskerName);
	strcpy(packet.strTargetName	,_lpstrTargetName);


	mSEND_GSP(lpClient,packet);
}
void
CPartyManager::sendValidPartyPacket(void *_lpClient,char *_lpstrPartyName)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_VALID_PARTY	packet;

	strcpy(packet.strName,_lpstrPartyName);
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_VALID_PARTY);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendValidPartyPacket(void *_lpClient,int _iPartySerial)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_VALID_PARTY	packet;

	packet.strName[0]	=	NULL;
	packet.wPartyIndex	=	_iPartySerial;
	packet.base.set(sizeof(packet),dWORLDtoSERVERS_VALID_PARTY);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendChangePartyStatusPacket(void *_lpClient,int _iIndex,BOOL _bIsChangeSetting)
{
	CClient		*lpClient	=	(CClient	*)_lpClient;

	CPartyInfo	*lpParty	=	getBySerial(_iIndex);

	if (!lpParty)
		return;

	WORLDtoSERVERS_CHANGE_PARTY_STATUS	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_CHANGE_PARTY_STATUS);

	packet.wPartySerial			=	_iIndex;
	packet.dwUniqueSerial		=	lpParty->m_dwUniqueSerial;

	packet.bf3GoldShareMethod	=	lpParty->m_bf3GoldShareMethod;
	packet.bf3ItemShareMethod	=	lpParty->m_bf3ItemShareMethod;
	packet.bf3PartyObject		=	lpParty->m_bf3PartyObject;	//파티 목적
	packet.bf1IsOpenParty		=	lpParty->m_bf1IsOpenParty;	//멤버를 계속 모집한다.

	packet.bf3LastReceiveGoldMember	=	lpParty->m_bf3LastReceiveGoldMember;	//멤버를 계속 모집한다.
	packet.bf3LastReceiveItemMember	=	lpParty->m_bf3LastReceiveItemMember;	//멤버를 계속 모집한다.
	packet.bf1IsChangeSetting		=	_bIsChangeSetting;

	mSEND_GSP(lpClient,packet);
}


void
CPartyManager::sendJoinPartyApplication(void *_lpClient,char *_lpstrAskerName,char *_lpstrLeaderName,int _iJob,int _iLevel)
{
	CClient		*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_JOIN_PARTY_APPLICATION	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_JOIN_PARTY_APPLICATION);

	packet.wAskerLevel	=	_iLevel;
	packet.wAskerJob	=	_iJob;

	strcpy(packet.strLeaderName,_lpstrLeaderName);
	strcpy(packet.strAskerName,_lpstrAskerName);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendJoinPartyApplicationAnswer(void *_lpClient,char *_lpstrAskerName,char *_lpstrLeaderName,char *_lpstrPartyName,int	_iPartySerial,int _iAnswer)
{
	CClient		*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_JOIN_PARTY_APPLICATION_ANSWER	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_JOIN_PARTY_APPLICATION_ANSWER);
	packet.wAnswer	=	_iAnswer;

	strcpy(packet.strAskerName,_lpstrAskerName);

	if	(_iAnswer	==	eJPA_OK)
	{
		packet.wPartySerial	=	_iPartySerial;

		strcpy(packet.strLeaderName	,_lpstrLeaderName);
		strcpy(packet.strPartyName	,_lpstrPartyName);
	}

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendUpdatePartyInfoPacket(void *_lpClient,char *_lpstrPartyName,BOOL _bIsCreate)
{
	int	iPartySerial	=	getPartySerial(_lpstrPartyName);

	if	(iPartySerial	==	0xffff)
	{
		sendValidPartyPacket(_lpClient,_lpstrPartyName);

		return;
	}

	sendUpdatePartyInfoPacket(_lpClient,getPartySerial(_lpstrPartyName),_bIsCreate);
}

void
CPartyManager::sendUpdatePartyInfoPacket(void *_lpClient,int _iIndex,BOOL _bIsCreate)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_UPDATE_PARTYINFO	packet;

	int	iResult			=	getSimpleInfoBySerial(_iIndex,&packet.partyInfo);

	if	(iResult	==	dPWR_CAN_NOT_FIND_PARTY)
		return;

	packet.wIsCreate	=	_bIsCreate;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.partyInfo.m_aMembers)+sizeof(CSimplePartyMemberInfo)*packet.partyInfo.m_bf4MemberCount;
	packet.base.wType	=	dWORLDtoSERVERS_UPDATE_PARTYINFO;

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendFailedToCreateParty(void *_lpClient,char *_lpstrAsker,int _iReason)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_FAILED_TO_CREATE_PARTY	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_FAILED_TO_CREATE_PARTY);

	strcpy(packet.strName,_lpstrAsker);

	packet.sReason	=	_iReason;

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendJoinPartyResult(void *_lpClient,int _iResult,char *_lpstrPartyName,char *_lpstrName,int _iLevel,int _iJob,int _iRemainHP,int _iSPC,
									int _iFieldSerial,int _iZoneSerial,int _iIFSerial,int _iIFFloor,int _iDuelVP)
{
	CClient	*lpClient	=	(CClient *)_lpClient;

	WORLDtoSERVERS_JOIN_PARTY	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_JOIN_PARTY);

	strcpy(packet.strPartyName,_lpstrPartyName);
	strcpy(packet.strName,_lpstrName);

	packet.wPartySerial				=	getPartySerial(_lpstrPartyName);
	packet.sResult					=	_iResult;
	packet.bf10Level				=	_iLevel;
	packet.bf8Job					=	_iJob;
	packet.wRemainHP				=	_iRemainHP;
	packet.wFieldSerial				=	_iFieldSerial;
	packet.iZoneSerial				=	_iZoneSerial;
	packet.bf10InstanceField		=	_iIFSerial;
	packet.bf4InstanceFieldFloor	=	_iIFFloor;
	packet.bf4SecretDungeonPlayCount=	_iSPC;
	packet.iDuelVP					=	_iDuelVP;

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendDisconnectPartyMemberPacket(void *_lpClient,char *_lpstrPartyName,int _iPartySerial,char *_lpstrMember)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_DISCONNECT_PARTY_MEMBER	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_DISCONNECT_PARTY_MEMBER);

	if (_iPartySerial	==	-1)
		_iPartySerial	=	getPartySerial(_lpstrPartyName);

	packet.wPartySerial	=	_iPartySerial;
	strcpy(packet.strPartyName,_lpstrPartyName);
	strcpy(packet.strName,_lpstrMember);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendPartyWorkPacket(void *_lpClient,DWORD _dwSerial,char *_lpstrMember,int _iWork)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_PARTY_WORK	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_PARTY_WORK);

	packet.wPartySerial	=	getPartySerial(_dwSerial);
	packet.wWork		=	_iWork;
	packet.dwUniqueSerial=	_dwSerial;

	strcpy(packet.strName,_lpstrMember);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendDissolutionPartyPacket(void *_lpClient,char *_lpstrPartyName)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_DISSOLUTION_PARTY	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_DISSOLUTION_PARTY);

	strcpy(packet.strPartyName,_lpstrPartyName);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendChangePartyNameResultPacket(void *_lpClient,int _iPartySerial,char *_lpstrPartyName,char *_lpstrChangePartyName,int _iResult)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_RENAME_PARTYNAME		packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_RENAME_PARTYNAME);
	packet.sResult		=	_iResult;
	packet.wPartySerial	=	_iPartySerial;

	strcpy(packet.strName,_lpstrPartyName);
	strcpy(packet.strChangeName,_lpstrChangePartyName);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::sendUpdateMemberInfoPacket(void *_lpClient,DWORD _dwSerial,char *_lpstrMember,int _iLevel,int _iPSC,int _iJob,int _iGDPC)
{
	CClient	*lpClient	=	(CClient	*)_lpClient;

	WORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO);
	packet.wPartySerial		=	getPartySerial(_dwSerial);
	packet.dwUniqueSerial	=	_dwSerial;
	packet.bf10Level		=	_iLevel;
	packet.bf4PSC			=	_iPSC;
	packet.bf8Job			=	_iJob;
	packet.bf3GDPC			=	_iGDPC;

	strcpy(packet.strName,_lpstrMember);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::operateJoinPartyApplication(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_JOIN_PARTY_APPLICATION	*lpPacket=	(SERVERStoWORLD_JOIN_PARTY_APPLICATION	*)_lpPacketData;

	CClient		*lpClient		=	(CClient	*)_lpClient;
	CClient		*lpDestServer	=	GetClientServer(lpPacket->strLeaderName);

	if	(!lpDestServer)
	{
		sendJoinPartyApplicationAnswer(_lpClient,lpPacket->strAskerName,lpPacket->strLeaderName,NULL,0xffff,eJPA_FAILED);
		return;
	}

	sendJoinPartyApplication(lpDestServer,lpPacket->strAskerName,lpPacket->strLeaderName,lpPacket->wAskerJob,lpPacket->wAskerLevel);
}

void
CPartyManager::operateJoinPartyApplicationAnswer(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_JOIN_PARTY_APPLICATION_ANSWER	*lpPacket=	(SERVERStoWORLD_JOIN_PARTY_APPLICATION_ANSWER	*)_lpPacketData;

	CClient		*lpDestServer	=	GetClientServer(lpPacket->strAskerName);

	if	(!lpDestServer)
		return;

	sendJoinPartyApplicationAnswer(lpDestServer,lpPacket->strAskerName,lpPacket->strLeaderName,lpPacket->strPartyName,lpPacket->wPartySerial,lpPacket->wAnswer);
}

void
CPartyManager::operateMoveFieldPartyMemberPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_MOVE_FIELD_PARTY_MEMBER	*lpPacket	=	(SERVERStoWORLD_MOVE_FIELD_PARTY_MEMBER	*)_lpPacketData;

	CClient	*lpClient	=	NULL;//(CClient	*)_lpClient;

	int	iWorkResult;

	if	(lpPacket->bf1IsLeave)
		iWorkResult	=	leaveField(lpPacket->strPartyName,lpPacket->strName,lpPacket->wDestFieldSerial,lpPacket->bf10InstanceField,lpPacket->bf4InstanceFieldFloor);
	else
		iWorkResult	=	enterField(lpPacket->strPartyName,lpPacket->strName,lpPacket->wDestFieldSerial,lpPacket->iZoneSerial,lpPacket->bf10InstanceField,lpPacket->bf4InstanceFieldFloor);

	switch(iWorkResult)
	{
		case	dPWR_CAN_NOT_FIND_PARTY		:
			sendValidPartyPacket((CClient *)_lpClient,lpPacket->strPartyName);
			//_log("파티 MOVE_FIELD_PARTY_MEMBER @@ 파티 못찾음");
			return;

		case	dPWR_CAN_NOT_FIND_MEMBER	:
			sendUpdatePartyInfoPacket(lpClient,lpPacket->strPartyName);
			//_log("파티 MOVE_FIELD_PARTY_MEMBER @@ 멤버 못찾음");
			return;
	}

	WORLDtoSERVERS_MOVE_FIELD_PARTY_MEMBER	packet;

	packet.base.set(sizeof(packet),dWORLDtoSERVERS_MOVE_FIELD_PARTY_MEMBER);

	packet.wPartySerial			=	getPartySerial(lpPacket->strPartyName);
	packet.wDestFieldSerial		=	lpPacket->wDestFieldSerial;
	packet.bf1IsLeave			=	lpPacket->bf1IsLeave;
	packet.iZoneSerial			=	lpPacket->iZoneSerial;
	packet.bf10IFSerial			=	lpPacket->bf10InstanceField;
	packet.bf4IFFloor			=	lpPacket->bf4InstanceFieldFloor;

	strcpy(packet.strPartyName,lpPacket->strPartyName);
	strcpy(packet.strName,lpPacket->strName);

	mSEND_GSP(lpClient,packet);
}

void
CPartyManager::operateCreatePartyPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_CREATE_PARTY	*lpPacket	=	(SERVERStoWORLD_CREATE_PARTY	*)_lpPacketData;

	CClient	*lpClient		=	(CClient	*)_lpClient;

	char	strPartyName[dPARTYNAME_LENGTH];

	for(int i=0;i<1000;i++)
	{
		sprintf(strPartyName,"%s_%.3d",lpPacket->strName,i);

		if	(getByName(strPartyName)	==	NULL)
			break;
	}

	int		iWorkResult		=	
		createParty(strPartyName,lpPacket->strName,lpPacket->wFieldSerial,lpPacket->bf10MasterLevel,lpPacket->bf8MasterJob,lpPacket->bf8MasterRemainHP,
								lpPacket->bf4MasterSecretDungeonPlayCount,lpPacket->iMasterZoneSerial,
								lpPacket->strTName,lpPacket->wFieldSerial,lpPacket->bf10TargetLevel,lpPacket->bf8TargetJob,lpPacket->bf8TargetRemainHP,
								lpPacket->bf4TargetSecretDungeonPlayCount,lpPacket->iTargetZoneSerial,lpPacket->iMasterDuelVP,lpPacket->iTargetDuelVP);

	switch(iWorkResult)
	{
		case	dPWR_PARTY_COUNT_FULL		:
		case	dPWR_EXIST_SAME_NAME_PARTY	:
			//_log("파티 CREATEPARTY 생성 실패");
			sendFailedToCreateParty(lpClient,lpPacket->strName,iWorkResult);
			return;
	}

	lpClient	=	NULL;

	sendUpdatePartyInfoPacket(lpClient,strPartyName,TRUE);
}

void
CPartyManager::operateJoinPartyPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_JOIN_PARTY	*lpPacket	=	(SERVERStoWORLD_JOIN_PARTY	*)_lpPacketData;

	CClient	*lpClient		=	NULL;//(CClient	*)_lpClient;

	int		iWorkResult		=	joinPartyMember(lpPacket->strPartyName,lpPacket->strName,lpPacket->wFieldSerial,lpPacket->bf10Level,lpPacket->bf8Job,lpPacket->bf7RemainHP,
												lpPacket->bf4SecretDungeonPlayCount,lpPacket->iZoneSerial,lpPacket->bf10InstanceField,lpPacket->bf4InstanceFieldFloor,lpPacket->iDuelVP);

	switch(iWorkResult)
	{
		case	dPWR_CAN_NOT_FIND_PARTY		:
			//_log("파티 JOINPARTY 파티 못찾음");
			sendValidPartyPacket((CClient *)_lpClient,lpPacket->strPartyName);
			return;
	}

	sendJoinPartyResult(lpClient,iWorkResult,lpPacket->strPartyName,lpPacket->strName,lpPacket->bf10Level,lpPacket->bf8Job,lpPacket->bf7RemainHP,lpPacket->bf4SecretDungeonPlayCount,
						lpPacket->wFieldSerial,lpPacket->iZoneSerial,lpPacket->bf10InstanceField,lpPacket->bf4InstanceFieldFloor,lpPacket->iDuelVP);

	if	(iWorkResult	==	dPWR_SUCCESS || iWorkResult	==	dPWR_RECONNECT)
		sendUpdatePartyInfoPacket(lpClient,lpPacket->strPartyName,FALSE);
}

void
CPartyManager::operatePartyWorkPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_PARTY_WORK	*lpPacket	=	(SERVERStoWORLD_PARTY_WORK	*)_lpPacketData;

	CClient		*lpClient	=	NULL;
	CPartyInfo	*lpParty	=	getBySerial(lpPacket->wPartySerial);

	if	(!lpParty	||	lpParty->m_dwUniqueSerial	!=	lpPacket->dwUniqueSerial)
	{
		sendValidPartyPacket((CClient *)_lpClient,lpPacket->wPartySerial);

		return;
	}

	int	iWorkResult	=	operatePartyWork(lpPacket->wPartySerial,lpPacket->wWork,lpPacket->strName);


	switch(iWorkResult)
	{
		case	dPWR_CAN_NOT_FIND_PARTY		:
			sendValidPartyPacket((CClient	*)_lpClient,lpPacket->wPartySerial);
			return;

		case	dPWR_CAN_NOT_FIND_MEMBER	:
			sendUpdatePartyInfoPacket(lpClient,lpPacket->wPartySerial);
			return;

		case	dPWR_DISSOLUTION_PARTY		:
//			if(!g_UnionpartyManager.isCorrectUnionParty(lpPacket->wPartySerial))
//				g_UnionpartyManager.dissolutionUnionParty(lpPacket->wPartySerial , dPW_DUP_BY_LEAVE_PARTY_MEMBER);
			
			sendPartyWorkPacket(lpClient,lpPacket->dwUniqueSerial,lpPacket->strName,lpPacket->wWork);
			sendDissolutionPartyPacket(lpClient,lpParty->m_strName);
			dissolutionParty(lpParty->m_strName);
			return;

		case	dPWR_RECONNECT				:
			sendPartyWorkPacket(NULL,lpPacket->dwUniqueSerial,lpPacket->strName,lpPacket->wWork);
			return;
	}

	sendPartyWorkPacket(lpClient,lpPacket->dwUniqueSerial,lpPacket->strName,lpPacket->wWork);
	sendUpdatePartyInfoPacket(lpClient,lpPacket->wPartySerial);
}

void
CPartyManager::operateRenamePartyPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_RENAME_PARTYNAME	*lpPacket	=	(SERVERStoWORLD_RENAME_PARTYNAME			*)_lpPacketData;

	CClient	*lpClient	=	NULL;

	int	iWorkResult		=	changePartyName(lpPacket->strPartyName,lpPacket->strName);

	sendChangePartyNameResultPacket(lpClient,lpPacket->wPartySerial,lpPacket->strPartyName,lpPacket->strName,iWorkResult);
}

void
CPartyManager::operateAskPartyListPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_ASK_PARTY_LIST	*lpPacket	=	(SERVERStoWORLD_ASK_PARTY_LIST	*)_lpPacketData;
	
	CClient	*lpClient	=	(CClient	*)_lpClient;
}

void
CPartyManager::operateAskPartyInfoPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_ASK_PARTYINFO	*lpPacket	=	(SERVERStoWORLD_ASK_PARTYINFO	*)_lpPacketData;
	
	CClient	*lpClient	=	(CClient	*)_lpClient;

	if	(lpPacket->strPartyName[0])
		sendUpdatePartyInfoPacket(lpClient,lpPacket->strPartyName);
	else
		sendUpdatePartyInfoPacket(lpClient,lpPacket->wPartySerial);
}

void
CPartyManager::operateChangePartyStatusPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_CHANGE_PARTY_STATUS	*lpPacket	=	(SERVERStoWORLD_CHANGE_PARTY_STATUS	*)_lpPacketData;

	CClient		*lpClient	=	(CClient	*)_lpClient;

	CPartyInfo	*lpParty	=	getBySerial(lpPacket->wPartySerial);

	if (!lpParty	||	lpParty->m_dwUniqueSerial	!=	lpPacket->dwUniqueSerial)
		return;

	lpParty->m_bf1IsOpenParty			=	lpPacket->bf1IsOpenParty;
	lpParty->m_bf3GoldShareMethod		=	lpPacket->bf3GoldShareMethod;
	lpParty->m_bf3ItemShareMethod		=	lpPacket->bf3ItemShareMethod;
	lpParty->m_bf3PartyObject			=	lpPacket->bf3PartyObject;
	lpParty->m_bf3LastReceiveGoldMember	=	lpPacket->bf3LastReceiveGoldMember;
	lpParty->m_bf3LastReceiveItemMember	=	lpPacket->bf3LastReceiveItemMember;

	sendChangePartyStatusPacket(lpClient,lpPacket->wPartySerial,lpPacket->bf1IsChangeSetting);
	sendUpdatePartyInfoToBCS(lpPacket->wPartySerial);
}

void
CPartyManager::operateUpdatePartyMemberInfoPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_CHANGE_PARTY_MEMBER_INFO	*lpPacket	=	(SERVERStoWORLD_CHANGE_PARTY_MEMBER_INFO	*)_lpPacketData;

	CClient	*lpClient	=	NULL;//(CClient	*)_lpClient;

	int		iWorkResult	=	updateMemberInfo(lpPacket->dwUniqueSerial,lpPacket->strName,lpPacket->bf10Level,lpPacket->bf4PSC,lpPacket->bf8Job ,lpPacket->bf3GDPC);

	switch(iWorkResult)
	{
		case	dPWR_CAN_NOT_FIND_PARTY		:
			sendValidPartyPacket((CClient *)_lpClient,lpPacket->wPartySerial);
			return;

		case	dPWR_CAN_NOT_FIND_MEMBER	:
			sendUpdatePartyInfoPacket((CClient *)_lpClient,lpPacket->wPartySerial);
			break;
	}

	sendUpdateMemberInfoPacket(lpClient,lpPacket->dwUniqueSerial,lpPacket->strName,lpPacket->bf10Level,lpPacket->bf4PSC,lpPacket->bf8Job, lpPacket->bf3GDPC);
}

//
//	캐릭터에는 파티가 있는데 파티에는 캐릭터가 없다.
void
CPartyManager::operateCheckPartyInfoByActorPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_CHECK_PARTY_INFO_BY_ACTOR	*lpPacket	=	(SERVERStoWORLD_CHECK_PARTY_INFO_BY_ACTOR	*)_lpPacketData;

	CClient		*lpClient	=	(CClient	*)_lpClient;
	CPartyInfo	*lpParty	=	getBySerial(lpPacket->wPartySerial);
	
	if	(!lpParty || stricmp(lpParty->m_strName,lpPacket->strName) != 0)	//	진짜로 없다.
	{
		sendUpdatePartyInfoPacket(NULL,lpPacket->strName);
		return;
	}

	//	있는게 정상이다. 게임 서버가 잘못된 정보를 가지고 있다.
	sendUpdatePartyInfoPacket(NULL,lpParty->m_strName);
}

void
CPartyManager::operatePartyChatPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_PARTY_CHAT *lpPacket	=	(SERVERStoWORLD_PARTY_CHAT *)_lpPacketData;

	CPartyInfo	*lpParty			=	getBySerial(lpPacket->wPartySerial);

	if (!lpParty)
	{
		sendValidPartyPacket((CClient *)_lpClient,lpPacket->wPartySerial);
		return;
	}

	CPartyMemberInfo*	lpMember	=	lpParty->getPartyMember(lpPacket->strName,GetHashCode(lpPacket->strName));

	if (!lpMember)
	{
		sendUpdatePartyInfoPacket((CClient *)_lpClient,lpParty->m_strName);
		return;
	}

	sendChatPacket(NULL,lpParty->m_wSerial,lpParty->m_strName,lpPacket->strMsg,lpPacket->strName);
}

void
CPartyManager::operateEnterIFPartyPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_PARTY_ENTER_IF	*lpPacket	=	(SERVERStoWORLD_PARTY_ENTER_IF *)_lpPacketData;

	CPartyInfo	*lpParty			=	getBySerial(lpPacket->wPartySerial);

	if (!lpParty || stricmp(lpParty->m_strName,lpPacket->strPartyName) != 0)
	{
		sendValidPartyPacket((CClient *)_lpClient,lpPacket->wPartySerial);
		return;
	}

	lpParty->m_wGateField	=	lpPacket->wGateField;
	lpParty->m_bf10GateArea	=	lpPacket->wGateArea;

	sendUpdatePartyInfoPacket(NULL,lpParty->m_strName);
}

void
CPartyManager::sendUpdatePartyInfoToBCS(int _iIndex)
{
	WB_UPDATE_PARTY_INFO	packet;

	packet.base.set(sizeof(packet),dWB_UPDATE_PARTY_INFO);

	getInfoForBCS(_iIndex,&packet.partyInfo);

	if	(packet.partyInfo.m_bf1IsValidParty)
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.partyInfo.m_aMemberList)-dNAME_LENGTH;

	SendPacketToBCS(&packet);
}

void
CPartyManager::sendAllPartyInfoToBCS()
{
	WB_PARTY_LIST			packet;

	packet.bf1IsFirst		=	TRUE;
	packet.bf1IsLast		=	FALSE;
	packet.bf8CurrentCount	=	0;
	packet.base.set(sizeof(packet),dWB_PARTY_LIST);

	packet.base.wSize		=	sizeof(packet)-sizeof(packet.aList);

	SendPacketToBCS(&packet);

	packet.bf1IsFirst		=	FALSE;

	for (int i=0;i<dPARTY_COUNT;i++)
	{
		if	(m_aParty[i].m_wSerial	==	0xffff)
			continue;

		m_aParty[i].getInfoForBCS(&packet.aList[packet.bf8CurrentCount++]);

		if	(packet.bf8CurrentCount	>=	c_iPartyCountForWBPartyListPacket)
		{
			packet.base.wSize		=	sizeof(packet);

			SendPacketToBCS(&packet);

			packet.bf8CurrentCount	=	0;
		}
	}

	if	(packet.bf8CurrentCount)
	{
		packet.base.wSize		=	sizeof(packet)-sizeof(packet.aList)+sizeof(cPartyInfoForBCS)*packet.bf8CurrentCount;
		SendPacketToBCS(&packet);
		packet.bf8CurrentCount	=	0;
	}

	packet.bf1IsLast	=	TRUE;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aList);

	SendPacketToBCS(&packet);
}
