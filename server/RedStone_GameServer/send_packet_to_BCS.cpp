#include "cGAME.H"
#include "cFIELD.H"
#include "cPROJECT.H"
#include "STDAFX.H"
#include "cDECODER.H"
#include "time.H"
#include "cPACKET_GAMESERVER.h"
#include "cPACKET_BCSERVER.h"
#include "CImageData.h"
#include "Client.h"
#include "CGuild.h"
#include "CDataSave.h"
#include "debugCode.h"
#include "secretDungeon.h"
#include "packetManager.h"

void
cGAME::sendBPGetItemInfo(cACTOR *_lpActor,char *_lpstrTargetName)
{
	GB_ASK_ITEM_INFO	packet;

	packet.base.set(sizeof(packet),dGB_ASK_ITEM_INFO);

	strcpy(packet.strAskerName,_lpActor->m_strName);
	memcpy(packet.strTargetName,_lpstrTargetName,dNAME_LENGTH);

	packet.strTargetName[dNAME_LENGTH-1]	=	NULL;

	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

void
cGAME::sendBPActorInfo(cACTOR *_lpActor)
{
	GB_REG_CLIENT	packet;

	packet.base.set(sizeof(packet),dGB_REG_CLIENT);

	strcpy(packet.strId,_lpActor->m_strId);
	strcpy(packet.strName,_lpActor->m_strName);

	packet.wLevel		=	_lpActor->m_iLevel;
	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	packet.wGuildRank	=	_lpActor->m_wGuildRank;
	packet.wOper		=	_lpActor->m_wOperatorLevel;					//	운영자 레벨
	packet.wSerial		=	_lpActor->m_iClientSerial;	//	돌려받을때 사용할 게임서버내의 client serial
	packet.wCurrentField=	_lpActor->m_wCurrentField;
	packet.wPartySerial	=	_lpActor->getPartySerial();
	packet.wAvatarIndex	=	_lpActor->m_wAvatarIndex;

	CClient * _lpclient = _lpActor->getClient();

	if	(_lpclient)
		_lpclient->SetBCSerial(0xfffe);

	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

//	클라이언트 정보 갱신
void
cGAME::sendBPUpdatePlayerInfo(cACTOR *_lpActor)
{
	CClient	*lpClient	=	_lpActor->getClient();

	if	(!lpClient)
		return;

	GB_UPDATE_INFO	packet;

	packet.base.set(sizeof(packet),dGB_UPDATE_INFO);

	strcpy(packet.strName,_lpActor->m_strName);

	packet.wBCSerial	=	lpClient->GetBCSerial();

	packet.wLevel		=	_lpActor->m_iLevel;
	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	packet.wGuildRank	=	_lpActor->m_wGuildRank;
	packet.wOper		=	_lpActor->m_wOperatorLevel;					//	운영자 레벨
	packet.wCurrentField=	_lpActor->m_wCurrentField;
	packet.wAvatarIndex	=	_lpActor->m_wAvatarIndex;
	packet.wPartySerial	=	_lpActor->getPartySerial();

	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

void
cGAME::sendBPAddFriend(cACTOR *_lpActor1,cACTOR *_lpActor2,char *_lpstrTargetName,int _iJob)
{
	GB_ADD_FRIEND	packet;

	packet.base.set(sizeof(packet),dGB_ADD_FRIEND);

	packet.aInfo[0].m_bf1IsOnline	=	TRUE;
	packet.aInfo[0].m_bf4Group		=	0;
	packet.aInfo[0].m_bf8Job		=	_lpActor1->m_wJob;
	strcpy(packet.aInfo[0].m_strName,_lpActor1->m_strName);

	if	(_lpActor2)
	{
		packet.wIsRecoverList			=	FALSE;
		packet.aInfo[1].m_bf1IsOnline	=	TRUE;
		packet.aInfo[1].m_bf4Group		=	0;
		packet.aInfo[1].m_bf8Job		=	_lpActor2->m_wJob;
		strcpy(packet.aInfo[1].m_strName,_lpActor2->m_strName);
	}
	else
	{
		packet.aInfo[0].m_bf1IsOnline	=	FALSE;
		packet.aInfo[1].m_bf1IsOnline	=	FALSE;
		packet.aInfo[1].m_bf4Group		=	0;
		packet.aInfo[1].m_bf8Job		=	_iJob;
		strcpy(packet.aInfo[1].m_strName,_lpstrTargetName);

		packet.wIsRecoverList			=	TRUE;
	}

	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

void
cGAME::sendBPRemoveFriend(char *_lpstrOwner,char *_lpstrRemoveFriendName)
{
	GB_FORCED_REMOVE_FRIEND	packet;

	packet.base.set(sizeof(packet),dGB_FORCED_REMOVE_FRIEND);

	strcpy(packet.strOwnerName,_lpstrOwner);
	strcpy(packet.strFriendName,_lpstrRemoveFriendName);

	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

void
cGAME::sendBPGuildChat(cACTOR *_lpActor,char *_lpstrMessage)
{
	GB_GUILD_CHAT	packet;

	CClient			*lpClient	=	_lpActor->getClient();

	if	(!lpClient)
		return;

	packet.base.set(sizeof(packet),dGB_GUILD_CHAT);

	packet.wBCSerial					=	lpClient->GetBCSerial();
	memcpy(packet.strName,_lpActor->m_strName,dNAME_LENGTH);
	packet.strName[dNAME_LENGTH-1]		=	NULL;
	memcpy(packet.strMessage,_lpstrMessage,dCHAT_LENGTH);
	packet.strMessage[dCHAT_LENGTH-2]	=	NULL;

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMessage)+strlen(packet.strMessage)+1;

	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

BOOL
cGAME::sendBPPartyChat(cACTOR *_lpActor,char *_lpstrMessage)
{
	GB_PARTY_CHAT	packet;

	CClient			*lpClient	=	_lpActor->getClient();

	if	(!lpClient)
		return	FALSE;

	packet.base.set(sizeof(packet),dGB_PARTY_CHAT);

	packet.wBCSerial					=	lpClient->GetBCSerial();
	memcpy(packet.strName,_lpActor->m_strName,dNAME_LENGTH);
	packet.strName[dNAME_LENGTH-1]		=	NULL;
	memcpy(packet.strMessage,_lpstrMessage,dCHAT_LENGTH);
	packet.strMessage[dCHAT_LENGTH-2]	=	NULL;

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMessage)+strlen(packet.strMessage)+1;

	return	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

BOOL
cGAME::sendBPGuildPacket(void *_lpPacket,int _iGuild)
{
	int	iPacketSize	=	((cMSG_BASE_TYPE *)_lpPacket)->wSize;

	GB_GUILD_WORK_BUS	packet;

	packet.base.set(sizeof(packet),dGB_GUILD_WORK_BUS);
	packet.wGuildSerial	=	_iGuild;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aBuffer)+iPacketSize;

	memcpy(packet.aBuffer,_lpPacket,iPacketSize);

	return	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

BOOL
cGAME::deliverPartyPacket(cACTOR *_lpOwner,void *_lpPacket,int _iField)
{
	int	iPacketSize	=	((cMSG_BASE_TYPE *)_lpPacket)->wSize;

	GB_PARTY_PACKET_DELIVER	packet;

	packet.base.set(sizeof(packet),dGB_PARTY_PACKET_DELIVER);
	packet.wPartySerial	=	_lpOwner->getPartySerial();
	packet.wFieldSerial	=	_iField;

	memcpy(packet.aBuffer,_lpPacket,iPacketSize);

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aBuffer)+iPacketSize;

	return	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

BOOL
cGAME::deliverPacket(cACTOR *_lpOwner,void *_lpPacket,int _iClinetCount,int _iField,WORD *_lpwClientList)
{
	int	iPacketSize	=	((cMSG_BASE_TYPE *)_lpPacket)->wSize;

	GB_PACKET_DELIVER	packet;

	packet.base.set(sizeof(packet),dGB_PACKET_DELIVER);
	packet.wDeliverTargetCount	=	_iClinetCount;
	packet.wTargetField			=	_iField;

	if	(_lpOwner)
		packet.wPacketOwner		=	_lpOwner->getBCSerial();
	else
		packet.wPacketOwner		=	0xffff;

	memcpy(packet.aBuffer,_lpPacket,iPacketSize);

	if	(_iClinetCount)
		memcpy(packet.aBuffer+iPacketSize,_lpwClientList,_iClinetCount*sizeof(WORD));

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aBuffer)+_iClinetCount*sizeof(WORD)+iPacketSize;

	return	SendPacketToBCS((char *)&packet,packet.base.wSize);
}

BOOL
cGAME::deliverPacket(cACTOR *_lpOwner,void *_lpPacket,int _iTargetClient)
{
	WORD	awList[2];

	awList[0]	=	_iTargetClient;

	return	deliverPacket(_lpOwner,_lpPacket,1,0xffff,awList);
}