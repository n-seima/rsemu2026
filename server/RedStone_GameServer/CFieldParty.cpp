#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"
#include "CGuild.H"
#include "secretDungeon.H"
#include "packetManager.H"

//
//	파티 정보 전송
void
cFIELD::sendPartyInfo(cACTOR *_lpActor,BOOL _bIsCreate)
{		// 마지막 수정일 : 09.10.01
	if (!checkActor(_lpActor))
		return;

	if (_lpActor->getPartySerial()	==	0xffff)
		return;

	mCS(g_pPartyManager->m_csPartyWork);

	cParty	*lpParty	=	_lpActor->getParty();

	if (!lpParty) //	파티 정보가 이상하다.
	{
		CLOG("party","sendPartyInfo");
		return;
	}

	lpParty->checkMember();	//	멤버 체크

	SG_PARTY_INFO	packet;

	packet.base.set(sizeof(packet),dSG_PARTY_INFO);

	CPartyInfoInPacket	*lpPartyInfo	=	&packet.partyInfo;

	strcpy(lpPartyInfo->m_strName,lpParty->m_strPartyName);

	if (g_pIfManager->getByUniquePartySerial(lpParty->m_dwUniqueSerial)	==	NULL)
	{
		lpParty->m_wGateField	=	0xffff;
		lpParty->m_bf10GateArea	=	0;
	}

	lpPartyInfo->m_bf1IsCreate			=	_bIsCreate;
	lpPartyInfo->m_bf10Serial			=	lpParty->m_wSerial;
	lpPartyInfo->m_bf4MemberCount		=	lpParty->getMemberCount();
	lpPartyInfo->m_wFieldUniqueSerial	=	m_iUniqueSerial;
	lpPartyInfo->m_wGateField			=	lpParty->m_wGateField;
	lpPartyInfo->m_bf10GateArea			=	lpParty->m_bf10GateArea;

	lpPartyInfo->m_bf3GoldShareMethod	=	lpParty->m_bf3GoldShareMethod;
	lpPartyInfo->m_bf3ItemShareMethod	=	lpParty->m_bf3ItemShareMethod;
	lpPartyInfo->m_bf3PartyObject		=	lpParty->m_bf3PartyObject;
	lpPartyInfo->m_bf1IsOpenParty		=	lpParty->m_bf1IsOpenParty;

	for (int i=0;i<(int)lpParty->getMemberCount();i++)
	{
		CPartyMemberInfoInPacket	*lpMemberInfoInPacket	=	&lpPartyInfo->m_aMemberList[i];
		cPARTY_MEMBER				*lpMemberInfo			=	&lpParty->m_aMember[i];

		lpMemberInfoInPacket->m_wSerial			=	0xffff;
		lpMemberInfoInPacket->m_bf8Job			=	lpMemberInfo->m_bf8Job;		//	직업
		lpMemberInfoInPacket->m_bf10Level		=	lpMemberInfo->m_bf10Level;		//	레벨
		lpMemberInfoInPacket->m_wFieldSerial	=	lpMemberInfo->m_wFieldSerial;

		lpMemberInfoInPacket->m_bf9IF			=	lpMemberInfo->m_bf10InstanceField;
		lpMemberInfoInPacket->m_bf4IFFloor		=	lpMemberInfo->m_bf4InstanceFieldFloor;
		lpMemberInfoInPacket->m_bf1IsDisconnected=	lpMemberInfo->m_bf1IsDisconnected;
		lpMemberInfoInPacket->m_bf4PlaySecretDungeonCount=	lpMemberInfo->m_bf4PlaySecretDungeonCount;
		lpMemberInfoInPacket->m_iDuelVP			=	lpMemberInfo->m_iDuelVP;

		strcpy(lpMemberInfoInPacket->m_strName,lpMemberInfo->m_strName);//	이름

		if (m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
		{
			if ((lpMemberInfo->m_wFieldSerial&(dINSTANCE_FIELD_FLAG-1))	!=	m_wGateField)
				continue;

			if (lpMemberInfo->m_bf10InstanceField		!=	(m_iUniqueSerial/dINSTANCE_FIELD_FLAG))
				continue;

			if (lpMemberInfo->m_bf4InstanceFieldFloor	!=	(m_iUniqueSerial%dINSTANCE_FIELD_FLAG))
				continue;
		}
		else
		{
			if (lpMemberInfo->m_wFieldSerial	!=	m_iUniqueSerial)
				continue;
		}

		cACTOR				*lpMember			=	getPlayerByName(lpMemberInfo->m_strName);

		if	(!lpMember)
			continue;

		if	(m_bIs1LevelGuildHall)
		{
			if	(_lpActor->m_wGuildSerial != lpMember->m_wGuildSerial)
				continue;
		}

		lpPartyInfo->m_aMemberList[i].m_wSerial	=	lpMember->m_wSerialInField;
	}

	g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
}
