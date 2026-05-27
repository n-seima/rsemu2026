#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"

//
//기본 채팅
BOOL
CAgent::sendChatMessage(char *_lpstrChat,int _iChatType,BOOL _bIsShout)
{
	CG_CHAT	packet;

	packet.base.set(sizeof(CG_CHAT),dCG_CHAT);

	if	(_bIsShout)
		packet.wChatType	=	dCHAT_SHOUT;	//	메시지 타입
	else
	{
		if	(_iChatType	==	eCMF_NORMAL_CHAT)
			packet.wChatType	=	dCHAT_CHAT;		//	메시지 타입
		if	(_iChatType	==	eCMF_PARTY_CHAT)
			packet.wChatType	=	dCHAT_PARTY;		//	메시지 타입
		if	(_iChatType	==	eCMF_GUILD_CHAT)
			packet.wChatType	=	dCHAT_GUILD;
	}

	strcpy(packet.strMessage,_lpstrChat);	//	메시지

	packet.base.wSize	=	sizeof(packet)-dCHAT_LENGTH+strlen(packet.strMessage)+1;

	if	(packet.base.wSize	>	sizeof(packet))
		return	FALSE;	

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	세이 메시지 보내기
BOOL
CAgent::sendSayMessage(char *_lpstrName,char *_lpstrMessage)
{
	CG_CHAT	packet;

	packet.base.set(sizeof(CG_CHAT),dCG_CHAT);

	packet.wChatType	=	dCHAT_SAY;		//	메시지 타입
	strcpy(packet.strMessage,_lpstrMessage);	//	메시지
	strcpy(packet.strDestName,_lpstrName);	//	메시지

	packet.base.wSize	=	sizeof(packet)-dCHAT_LENGTH+strlen(packet.strMessage)+1;

	if (packet.base.wSize	>	sizeof(packet))	return	FALSE;	

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//파티 리스트 요청
BOOL
CAgent::sendAskParyList()
{
	CG_ASK_PARTY_LIST	packet;

	packet.base.set(sizeof(CG_ASK_PARTY_LIST),dCG_ASK_PARTY_LIST);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//파티 정보 요청
BOOL
CAgent::sendAskPartyInfo()
{
	CG_ASK_PARTY_INFO	packet;

	packet.base.set(sizeof(CG_ASK_PARTY_INFO),dCG_ASK_PARTY_INFO);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//초대
BOOL
CAgent::sendAskJoinParty(char *_lpstrName,BOOL _bIsForJoinParty,BOOL _bIsForUnionParty, BOOL _bIsGuildMemberJoinParty /* = FALSE */)
{
	CG_ASK_JOIN_PARTY	packet;

	packet.base.set(sizeof(CG_ASK_JOIN_PARTY),dCG_ASK_JOIN_PARTY);
	packet.bf1IsJoinPartyApplication	=	_bIsForJoinParty;
	packet.bf1IsUnionPartyApplication	=	_bIsForUnionParty;
	packet.bf1IsGuildMemberJoinParty 	=	_bIsGuildMemberJoinParty;

	strcpy(packet.strName,_lpstrName);	//	초대할 유저의 이름

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//파티 참가 요청에 응답
BOOL
CAgent::sendAskJoinPartyReply(char *_lpstrAskerName,WORD _wReply,BOOL _bIsReplyForJoinPartyApplication , BOOL _bIsUnionPartyApplication, BOOL _bIsGuildMemberJoinParty /* = FALSE */)
{
	CG_ASK_JOIN_PARTY_REPLY	packet;

	packet.base.set(sizeof(CG_ASK_JOIN_PARTY_REPLY),dCG_ASK_JOIN_PARTY_REPLY);

	packet.bf1IsForJoinPartyApplication	=	_bIsReplyForJoinPartyApplication;
	packet.bf1IsUnionPartyApplication	=	_bIsUnionPartyApplication;
	packet.bf1IsGuildMemberJoinParty 	=	_bIsGuildMemberJoinParty;
	packet.wReply						=	_wReply;	//	응답
	strcpy(packet.strAskerName,_lpstrAskerName);	//	초대한 유저의 이름

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//파티 탈퇴 요청
BOOL
CAgent::sendAskLeaveParty()
{
	CG_LEAVE_PARTY			packet;

	packet.base.set(sizeof(CG_LEAVE_PARTY),dCG_LEAVE_PARTY);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//파티에서 쫏아내기 요청
BOOL
CAgent::sendPartyLeaderWork(int _iWork,char *_lpstrName)
{
	CG_PARTY_LEADER_WORK	packet;

	packet.base.set(sizeof(CG_PARTY_LEADER_WORK),dCG_PARTY_LEADER_WORK);
	packet.wWork	=	_iWork;

	strcpy(packet.strName,_lpstrName);	//	초대한 유저의 이름

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	파티이름 변경 하기
BOOL
CAgent::sendChangePartyName(char *_lpstrName)
{
	CG_CHANGE_PARTY_NAME	packet;

	packet.base.set(sizeof(CG_CHANGE_PARTY_NAME),dCG_CHANGE_PARTY_NAME);

	strcpy(packet.strName,_lpstrName);	//	초대한 유저의 이름

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//파티 상태 변경
BOOL
CAgent::sendChangePartyStatus(int _iGoldSharingMethod,int _iItemSharingMethod,int _iPartyObject,BOOL _bIsOpenParty)
{
	CG_CHANGE_PARTY_STATUS	packet;

	packet.base.set(sizeof(packet),dCG_CHANGE_PARTY_STATUS);

	packet.bf3GoldShareMethod	=	_iGoldSharingMethod;
	packet.bf3ItemShareMethod	=	_iItemSharingMethod;
	packet.bf3PartyObject		=	_iPartyObject;	//파티 목적
	packet.bf1IsOpenParty		=	_bIsOpenParty;	//멤버를 계속 모집한다.

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	신고하기
BOOL
CAgent::sendAccuse(char *_lpstrAccuseName)
{
	return	TRUE;

	CG_ACCUSE	packet;

	packet.base.set(sizeof(CG_ACCUSE),dCG_ACCUSE);

	packet.dwCheckSum1	=	random(10000);
	packet.dwCheckSum2	=	random(10000);
	packet.dwSerial		=	12345678;

	return	sendPacket((char *)&packet,packet.base.wSize);
}