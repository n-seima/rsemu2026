#include "CGuild.H"
#include "instance_guild_battle_manager.H"
#include "CActor.H"
#include "CGame.H"
#include "CCompress.H"
#include "packetManager.H"
#include "secretDungeon.H"

//
//	멤버 업데이트
void
CGuild::sendUpdateMemberInfo(CGuildMan *_lpMember)
{
	if	(IsAvailBCConnect())
		return;

	SG_UPDATE_GUILD_MEMBER	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_GUILD_MEMBER);

	packet.wGuildSerial			=	m_wSerial;
	packet.bMemberCount			=	(BYTE)m_wMemberCount;
	packet.bSubMasterCount		=	(BYTE)m_wSubMasterCount;
	packet.bCongressManCount	=	(BYTE)m_wCongressManCount;

	packet.m_bf11Level			=	_lpMember->m_wLevel;
	packet.m_bf4Rank			=	_lpMember->m_wRank;
	packet.m_bf8Job				=	(BYTE)_lpMember->m_wJob;
	packet.m_bf4ReservePet		=	(BYTE)_lpMember->m_bf3ReservePet;
	packet.m_bf1IsConnected		=	FALSE;
	strcpy(packet.strMemberName,_lpMember->m_strName);

	addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);
}
//
//	멤버 제거
void
CGuild::sendRemoveMember(char *_lpstrName)
{
	if	(IsAvailBCConnect())
		return;

	SG_REMOVE_GUILD_MEMBER	packet;

	packet.base.set(sizeof(packet),dSG_REMOVE_GUILD_MEMBER);

	packet.wGuildSerial			=	m_wSerial;
	packet.bMemberCount			=	(BYTE)m_wMemberCount;
	packet.bSubMasterCount		=	(BYTE)m_wSubMasterCount;
	packet.bCongressManCount	=	(BYTE)m_wCongressManCount;
	strcpy(packet.strMemberName,_lpstrName);

	addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);
}

//
//	길드 멤버 리스트 
BOOL
CGuild::sendGuildMemberList(cACTOR *_lpActor)
{
	int						i;
	SG_GUILD_MEMBER_LIST	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_MEMBER_LIST);
	packet.bf15Count	=	0;
	packet.bf1IsAppend	=	FALSE;

	for (i=0;i<dGUILD_MAX_PLAYER;i++)
	{
		if	(m_aMembers[i].wSerial	==	0xffff)
			continue;

		CSimpleGuildMemberInfo	*lpMember	=	&packet.m_aMember[packet.bf15Count];

		lpMember->setName(m_aMembers[i].m_strName);

		lpMember->m_bf11Level		=	m_aMembers[i].m_wLevel;
		lpMember->m_bf4Rank			=	m_aMembers[i].m_wRank;
		lpMember->m_bf8Job			=	(BYTE)m_aMembers[i].m_wJob;
		lpMember->m_bf4ReservePet	=	m_aMembers[i].m_bf3ReservePet;
		lpMember->m_bf1IsConnected	=	FALSE;

		packet.bf15Count++;

		if	(packet.bf15Count		>=	dGUILD_MEMBER_COUNT_FOR_MEMBER_LIST_PACKET)
		{
			g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);

			packet.bf15Count	=	0;
			packet.bf1IsAppend	=	TRUE;
		}
	}

	if	(packet.bf15Count || packet.bf1IsAppend	==	FALSE)
	{

		packet.base.wSize	=	sizeof(packet) - sizeof(packet.m_aMember) + sizeof(CSimpleGuildMemberInfo)*packet.bf15Count;

		g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}

	return	TRUE;
}

void
CGuild::sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ETC_WORK);

	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;

	addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);
}

//
//	다음 전투를 준비하라고 멤버들에게 메시지 전송
void
CGuild::sendReadyToNextGuildBattleMessageToMember(char *_lpTargetGuildName,int _iHour,int _iMinute)
{
	SG_READY_TO_NEXT_GUILD_BATTLE	packet;

	packet.base.set(sizeof(packet),dSG_READY_TO_NEXT_GUILD_BATTLE);
	strcpy(packet.strTargetGuildName,_lpTargetGuildName);
	packet.wGuildSerial	=	m_wSerial;
	packet.bHour		=	_iHour;
	packet.bMinute		=	_iMinute;

	addSendPacketToAllMember((cMSG_BASE_TYPE *)&packet);
}

//
//	모든 멤버에게 패킷 전송
void
CGuild::addSendPacketToAllMember(void *_lpPacket)
{
	if	(IsAvailBCConnect())
		if	(g_game.sendBPGuildPacket(_lpPacket,m_wSerial))
			return;
	
	int			iSize		=	((cMSG_BASE_TYPE *)_lpPacket)->wSize;

	for (int iField=g_game.m_iFieldCount;iField;)
	{
		--iField;

		cFIELD	*lpField	=	&g_game.m_pField[iField];

		for	(int iTeam=0;iTeam<1+lpField->m_bIsGuildBattleField;iTeam++)
		{
			for	(int iPlayer=lpField->m_aiActorCount[iTeam];iPlayer;)
			{
				--iPlayer;

				cACTOR	*lpPlayer	=	lpField->getActor(lpField->m_awActorList[iTeam][iPlayer]);

				if	(!lpPlayer	||	lpPlayer->m_wGuildSerial	!=	m_wSerial)
					continue;
				
				g_userPM.add(lpPlayer->m_iClientSerial,_lpPacket,iSize);
			}
		}
	}

	for (int i =dMAX_IF_COUNT;i;)
	{
		--i;

		cInstanceField*	lpIF	=	&g_pIfManager->m_aInstanceField[i];

		if	(lpIF->m_wSerial	==	0xffff)
			continue;

		for (int iFloor=0;iFloor<lpIF->m_wFloorCount;iFloor++)
		{
			cFIELD		*lpField=	&lpIF->m_pFloor[iFloor];

			for	(int iPlayer=lpField->m_aiActorCount[0];iPlayer;)
			{
				--iPlayer;

				cACTOR	*lpPlayer	=	lpField->getActor(lpField->m_awActorList[0][iPlayer]);

				if	(!lpPlayer	||	lpPlayer->m_wGuildSerial	!=	m_wSerial)
					continue;

				g_userPM.add(lpPlayer->m_iClientSerial,_lpPacket,iSize);
			}
		}
	}
}