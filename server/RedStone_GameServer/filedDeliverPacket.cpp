#include <time.h>
#include "cFIELD.H"
#include "cGAME.H"
#include "cPACKET_WORLDSERVER.H"
#include "cJOB.H"
#include "debugCode.H"
#include "CGuild.H"
#include "CActor.H"
#include "CShoppingCart.H"
#include "arca.H"
#include "secretDungeon.H"
#include "packetManager.H"
#include "booking_work.H"
#include "fireWork.H"

void
cFIELD::operateDeliverPacket()
{
	return;
	if	(m_queSendPacket.getCount()	==	0	||	m_bIs1LevelGuildHall)
		return;

	m_queSendPacket.setFront();

	GB_PACKET_DELIVER	packet;

	while(1)
	{
		cPACKET_INFO		*lpPacketInfo		=	m_queSendPacket.getOne();
		ALL_MSG				*lpPacket			=	&lpPacketInfo->m_packet;
		cACTOR				*lpOwner			=	NULL;
		int					iBufferPoint		=	0;

		if	(!lpPacketInfo)
			break;	//	다 꺼냈다.

		if	(lpPacketInfo->m_iOwner		>=	0	&&	lpPacketInfo->m_iOwner	<	dMAX_ACTOR_IN_FIELD)
			lpOwner						=	getActor(lpPacketInfo->m_iOwner);

		if	(lpOwner)
			packet.wPacketOwner			=	lpOwner->getBCSerial();
		else
			packet.wPacketOwner			=	0xffff;

		switch(lpPacket->base.wType)
		{
			case	dSG_CHAT						:
			{
				if	(isSecretDungeon())
					break;

				SG_CHAT	*lpChatPacket	=	&lpPacket->mSG_CHAT;

				BOOL	bIsNormalTalk	=	FALSE;	//	주변에 말하는거다

				if	(lpChatPacket->bf6ChatType	==	eCT_TALK || lpChatPacket->bf6ChatType	==	eCT_TALK +0x20 || lpChatPacket->bf6ChatType == eCT_EVENT_CHAT)
					bIsNormalTalk		=	TRUE;

				if	(!bIsNormalTalk)
				{
					lpPacketInfo->m_iOwner		=	cPACKET_INFO::eSendedPacket;
					g_game.deliverPacket(lpOwner,lpChatPacket,0,m_iUniqueSerial,NULL);
					break;
				}

				if	(bIsNormalTalk)
					if	(!lpOwner)
						break;

				lpPacketInfo->m_iOwner			=	cPACKET_INFO::eSendedPacket;

				packet.base.set(sizeof(packet),dGB_PACKET_DELIVER);

				packet.wTargetField				=	0xffff;
				packet.wDeliverTargetCount		=	0;
				iBufferPoint					=	lpChatPacket->base.wSize;

				memcpy(packet.aBuffer,lpChatPacket,lpChatPacket->base.wSize);

				WORD	*lpwActorList			=	(WORD	*)(packet.aBuffer+lpChatPacket->base.wSize);

				for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
					for (int i=m_aiActorCount[iTeam];i;)
					{
						--i;

						cACTOR	*lpPlayer	=	getActor(m_awActorList[iTeam][i]);

						if	(!lpPlayer	||	!lpPlayer->isPlayer()	||	lpPlayer->m_wJob	>	dPLAYER_JOB_COUNT)
							continue;

						if	(bIsNormalTalk)
							if	(!lpPlayer->m_rectMoveFrame.Isin(lpOwner->m_pos.x,lpOwner->m_pos.y))
								continue;

						lpwActorList[packet.wDeliverTargetCount++]	=	lpPlayer->getBCSerial();

						iBufferPoint	+=	2;

						if	(iBufferPoint	+2	>=	c_iBCS_TransPacketBufferSize)
						{
							SendPacketToBCS((char *)&packet,packet.base.wSize);

							iBufferPoint				=	lpChatPacket->base.wSize;
							packet.wDeliverTargetCount	=	0;
						}
					}

				if	(packet.wDeliverTargetCount)
				{
					packet.base.wSize	=	sizeof(packet)-sizeof(packet.aBuffer)+iBufferPoint;
					SendPacketToBCS((char *)&packet,packet.base.wSize);
				}
				break;
			}
		}
	}
}