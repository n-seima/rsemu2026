#include "cGAME.H"
#include "cFIELD.H"
#include "cPROJECT.H"
#include "STDAFX.H"
#include "cDECODER.H"
#include "time.H"
#include "cPACKET_GAMESERVER.h"
#include "CImageData.h"
#include "Client.h"
#include "CGuild.h"
#include "instance_guild_battle_manager.h"
#include "debugCode.h"
#include "CEvent.h"
#include "packetManager.H"

//
//	파티 정보 패킷 처리
void
cGAME::operatePartyInfoPacket(WORLDtoSERVERS_UPDATE_PARTYINFO* _lpPacket)
{
	g_pPartyManager->updatePartyInfo(_lpPacket);	//	생성되는 거냐?
}

//
//	파티 참가 패킷 처리
void
cGAME::operateJoinPartyPacket(WORLDtoSERVERS_JOIN_PARTY *_lpPacket)
{
	cACTOR	*lpTarget				=	getPlayer(_lpPacket->strName);
	cParty	*lpParty				=	g_pPartyManager->get(_lpPacket->wPartySerial,_lpPacket->strPartyName);
	int		iAddMemberFieldSerial	=	0xffff;

	if	(lpParty	==	NULL)	//	파티가 없다. 이경우 어차피 월드에서 다시 파티 정보가 날라오니 일단 로그만 남기자.
	{
		CLOG("party","join party 파티가 없어요. -o- '%s'",_lpPacket->strPartyName);
		return;
	}

	switch(_lpPacket->sResult)
	{
		case	dPWR_CAN_NOT_ADD_MEMBER_BY_TOO_MANY_MEMBER	:
			if	(lpTarget)
				lpTarget->sendPartyMessage(ePM_ALREADY_FULL_PARTY);

			CLOG("party","풀 파티래요. '%s'",_lpPacket->strName);
			return;

		case	dPWR_ALREADY_EXIST_SAME_NAME_MEMBER			:
			if	(lpTarget)
				lpTarget->sendPartyMessage(ePM_FAILED_TO_JOIN_PARTY);
			CLOG("party","같은 이름의 멤버가 이미 있데요. '%s'",_lpPacket->strName);
			return;
	}

//	파티에 추가했다고 이 서버 안에 있는 인간들한테 알려준다.
	SG_JOIN_PARTY_MEMBER	packet;

	packet.base.set(sizeof(packet),dSG_JOIN_PARTY_MEMBER);

	strcpy(packet.strName,_lpPacket->strName);
	packet.wLevel			=	_lpPacket->bf10Level;
	packet.wJob				=	_lpPacket->bf8Job;
	packet.wIndex			=	lpParty->getMemberCount();
	packet.wField			=	_lpPacket->wFieldSerial;
	packet.bf10IFSerial		=	_lpPacket->bf10InstanceField;
	packet.bf4IFFloor		=	_lpPacket->bf4InstanceFieldFloor;
	packet.bf4SPC			=	_lpPacket->bf4SecretDungeonPlayCount;
	packet.iDuelVP			=	_lpPacket->iDuelVP;

	if	(lpTarget)
		iAddMemberFieldSerial	=	lpTarget->m_wCurrentField;

	CSimplePartyMemberInfo	memberInfo;

	memberInfo.m_bf10Level					=	_lpPacket->bf10Level;
	memberInfo.m_bf8RemainHP				=	_lpPacket->wRemainHP;
	memberInfo.m_bf8Job						=	_lpPacket->bf8Job;
	memberInfo.m_wCurrentField				=	_lpPacket->wFieldSerial;
	memberInfo.m_bf1IsMoveField				=	FALSE;
	memberInfo.m_bf1IsDisconnected			=	FALSE;
	memberInfo.m_bf10InstanceField			=	_lpPacket->bf10InstanceField;
	memberInfo.m_bf4InstanceFieldFloor		=	_lpPacket->bf4InstanceFieldFloor;
	memberInfo.m_bf4SecretDungeonPlayCount	=	_lpPacket->bf4SecretDungeonPlayCount;
	memberInfo.m_iDuelVP					=	_lpPacket->iDuelVP;
	memberInfo.m_bf3GuildDungeonPlayTime	=	_lpPacket->bf3GuildDungeonPlayCount;

	if	(lpTarget)
		memberInfo.m_iZoneSerial		=	lpTarget->m_iZoneSerial;
	else
		memberInfo.m_iZoneSerial		=	0xffffffff;

	strcpy(memberInfo.m_strName,_lpPacket->strName);
	
	lpParty->joinPartyMember(&memberInfo);

	for (int i=0;i<lpParty->getMemberCount()-1;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

		if	(!lpActor)
			continue;

		packet.wSerialInField		=	0xffff;

		if	(lpActor->m_wCurrentField==	iAddMemberFieldSerial)
			packet.wSerialInField	=	lpTarget->m_wSerialInField;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}

	if(lpTarget)
	{
		g_pPartyManager->changeMemberInfo(lpParty->m_wSerial,lpParty->m_dwUniqueSerial,lpTarget->m_strName,lpTarget->m_iLevel,lpTarget->m_bf4SecretDungeonPlayCount,lpTarget->m_wJob,lpTarget->m_bf3GuildDungeonPlayTime);
	}
}
//
//	파티 탈퇴 패킷 처리
void
cGAME::operatePartyWorkPacket(WORLDtoSERVERS_PARTY_WORK *_lpPacket)
{
	g_pPartyManager->operatePartyWork(_lpPacket->wPartySerial,_lpPacket->dwUniqueSerial,_lpPacket->wWork,_lpPacket->strName);
}

void
cGAME::operateDisconnectPartyMemberPacket(WORLDtoSERVERS_DISCONNECT_PARTY_MEMBER	*_lpPacket)
{
	g_pPartyManager->disconnectMember(_lpPacket->wPartySerial,_lpPacket->strPartyName,_lpPacket->strName);
}

//
//	파티의 이름 변경
void
cGAME::operateChangePartyNamePacket(WORLDtoSERVERS_RENAME_PARTYNAME *_lpPacket)
{
	g_pPartyManager->changePartyName(_lpPacket->wPartySerial,_lpPacket->strName,_lpPacket->strChangeName);
}

void
cGAME::operateChangePartyStatus(WORLDtoSERVERS_CHANGE_PARTY_STATUS *_lpPacket)
{
	g_pPartyManager->changePartyStatus(	_lpPacket->wPartySerial,_lpPacket->dwUniqueSerial,_lpPacket->bf3GoldShareMethod,_lpPacket->bf3ItemShareMethod,_lpPacket->bf3PartyObject,
								_lpPacket->bf1IsOpenParty,_lpPacket->bf3LastReceiveGoldMember,_lpPacket->bf3LastReceiveItemMember,_lpPacket->bf1IsChangeSetting);
}

void
cGAME::operateJoinPartyApplication(WORLDtoSERVERS_JOIN_PARTY_APPLICATION *_lpPacket)
{
	cACTOR	*lpPlayer	=	getPlayer(_lpPacket->strLeaderName);

	int		iResult		=	eJPA_OK;

	if	(!lpPlayer)
		iResult	=	eJPA_FAILED;
	else
	{
		cParty	*lpParty	=	lpPlayer->getParty();

		if	(!lpParty || !lpPlayer->isPartyLeader() || lpParty->isFullParty() || lpParty->m_bf1IsOpenParty == FALSE)
			iResult	=	eJPA_FAILED;
	}

	if	(iResult	==	eJPA_FAILED)
	{
		sendWPJoinPartyApplicationAnswer(_lpPacket->strLeaderName,_lpPacket->strAskerName,iResult,NULL);
		return;
	}

	lpPlayer->sendJoinParty(_lpPacket->wAskerLevel,_lpPacket->wAskerJob,_lpPacket->strAskerName,TRUE);
}

void
cGAME::operateJoinPartyApplicationAnswer(WORLDtoSERVERS_JOIN_PARTY_APPLICATION_ANSWER *_lpPacket)
{
	cACTOR	*lpPlayer	=	getPlayer(_lpPacket->strAskerName);

	int		iResult		=	eJPA_OK;

	if	(!lpPlayer	||	lpPlayer->getPartySerial()	!=	0xffff)	//	없거나 이미 가입해 있다.
		return;

	switch	(_lpPacket->wAnswer)
	{
		case	eJPA_OK						:
			lpPlayer->receivePermitJoinPartyApplication(_lpPacket->strPartyName,_lpPacket->wPartySerial);
			break;
		case	eJPA_DENY					:
			lpPlayer->sendPartyMessage(ePM_DENY_INVITE_MESSAGE);
			break;
		case	eJPA_FAILED	:
			lpPlayer->sendPartyMessage(ePM_FAILED_TO_JOIN_PARTY);
			break;
	}
}

void
cGAME::operateUpdateUnionPartyInfoPacket(WORLDtoSERVERS_UPDATE_UNIONPARTYINFO *_lpPacket)
{
	g_pUnionpartyManager->updateUnionPartyInfo(_lpPacket);
}

void
cGAME::operateUpdateUnionPartyDissolutionPacket(SG_UNION_PARTY_WORK *_lpPacket)
{
	g_pUnionpartyManager->dissolutionUnionParty(_lpPacket->wValue[0] , _lpPacket->wValue[1]);
}

void
cGAME::operateUpdateUnionPartyCreateResultPacket(SG_UNION_PARTY_WORK *_lpPacket)
{
	if(_lpPacket->wValue[0] == CREATE_UNION_PARTY_RESULT_OK)
		g_pUnionpartyManager->CreateUnionPartyResultWork(_lpPacket->wValue[1] , g_pPartyManager->get(_lpPacket->wValue[2]) , g_pPartyManager->get(_lpPacket->wValue[3]));
}

void
cGAME::operateMoveFieldPartyMemberPacket(WORLDtoSERVERS_MOVE_FIELD_PARTY_MEMBER *_lpPacket)
{
	g_pPartyManager->moveFieldMember(_lpPacket->wPartySerial,_lpPacket->strPartyName,_lpPacket->strName,_lpPacket->wDestFieldSerial,_lpPacket->bf1IsLeave,_lpPacket->iZoneSerial,_lpPacket->bf10IFSerial,_lpPacket->bf4IFFloor);
}

void
cGAME::operateUpdatePartyInfoPacket(WORLDtoSERVERS_UPDATE_PARTYINFO *_lpPacket)
{
	g_pPartyManager->updatePartyInfo(_lpPacket);
}

void
cGAME::operateValidPartyPacket(WORLDtoSERVERS_VALID_PARTY *_lpPacket)
{
	if (_lpPacket->strName[0])
		g_pPartyManager->dissolutionParty(_lpPacket->strName);
	else
		g_pPartyManager->dissolutionParty(_lpPacket->wPartyIndex);
}

void
cGAME::operateFailedToCreatePartyPacket(WORLDtoSERVERS_FAILED_TO_CREATE_PARTY *_lpPacket)
{
	cACTOR	*lpActor	=	getPlayer(_lpPacket->strName);

	if (!lpActor)
		return;

	switch(_lpPacket->sReason)
	{
		case	dPWR_PARTY_COUNT_FULL		:
		case	dPWR_EXIST_SAME_NAME_PARTY	:
			lpActor->sendPartyMessage(ePM_FAILED_TO_CREATE_PARTY);
			break;
	}
}

void
cGAME::operateDissolutionPartyPacket(WORLDtoSERVERS_DISSOLUTION_PARTY *_lpPacket)
{
	g_pPartyManager->dissolutionParty(_lpPacket->strPartyName);
}

//
//	파티 리스트가 날아 왔다.
void
cGAME::operatePartyListPacket(WORLDtoSERVERS_PARTY_LIST *_lpPacket)
{
	CClient	*lpClient	=	CM.mGETCLIENT(_lpPacket->dwSerial);

	if	(!lpClient)
		return;

	SG_PARTY_LIST	packet;

	packet.base.set(sizeof(packet),dSG_PARTY_LIST);

	packet.wCount		=	_lpPacket->wPartyCount;//	넘어갈 갯수 
	packet.wPartySIndex	=	_lpPacket->wPartySIndex;
	packet.wPartyEIndex	=	_lpPacket->wPartyEIndex;
	memcpy(packet.aPartyList,_lpPacket->pInfo,sizeof(CPartyInfoForList)*packet.wCount);

	g_userPM.add(_lpPacket->dwSerial,&packet,packet.base.wSize);
}

//
//	파티 멤버 레벨업 패킷 처리
void
cGAME::operateChangePartyMemberInfoPacket(WORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO* _lpPacket)
{
	g_pPartyManager->changeMemberInfo(_lpPacket->wPartySerial,_lpPacket->dwUniqueSerial,_lpPacket->strName,_lpPacket->bf10Level,_lpPacket->bf4PSC,_lpPacket->bf8Job,_lpPacket->bf3GDPC);
}

//
//	파티챗 처리
void
cGAME::operatePartyChat(WORLDtoSERVERS_PARTY_CHAT* _lpPacket)
{
	SG_CHAT	packet;
	cParty	party;

	FillChattingBuffer(&packet,_lpPacket->strSendID,_lpPacket->strMsg);

	if (!g_pPartyManager->getParty(&party,_lpPacket->wPartySerial))
		return;

	packet.bf11PGSerial	=	party.m_wSerial;
	packet.bf6ChatType	=	eCT_PARTY;

	int		aSendFieldSerial[20],iSendFieldCount=0;

	for (int i=0;i<party.getMemberCount();i++)
	{
		cACTOR	*lpMember	=	party.getActorByMemberIndex(i);

		if (!lpMember)	
			continue;

		int		iFieldSerial=	lpMember->m_lpField->m_iUniqueSerial;

		for (int j=0;j<iSendFieldCount;j++)
		{
			if(aSendFieldSerial[j]	==	iFieldSerial)
			{
				iFieldSerial	=	-1;
				break;
			}
		}

		if	(iFieldSerial	!=	-1)
		{
			aSendFieldSerial[iSendFieldCount++]	=	iFieldSerial;

			lpMember->m_lpField->addSendPacket((ALL_MSG *)&packet,0xffff);
		}
	}
}
