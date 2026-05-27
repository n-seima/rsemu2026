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
#include "CShoppingCart.h"
#include "packetManager.h"
#include "rscomm.h"

#ifdef	_USE_BROADCAST
//
//	
void
cGAME::operateRegClientResult(BG_REG_CLIENT_RESULT *_lpPacket)
{
	CClient	*lpClient	=	CM.mGETCLIENT(_lpPacket->wSerial);

	if	(!lpClient)
		return;

	int		iZoneSerial	=	lpClient->GetActorSerialInServer();

	cACTOR	*lpActor	=	getActor(iZoneSerial);

	if	(!lpActor	||	!lpActor->isPlayer())
		return;

	if	(_lpPacket->wIndex	==	0xfffe)	//	이미 등록되어 있음
	{
		_lpPacket->wIndex	=	lpClient->GetBCSerial();
	}
	else
	if	(_lpPacket->wIndex	==	0xffff)
	{
		lpActor->bookingReadyToExitGame();
		return;
	}

	SG_BCS_INFO	packet;
	packet.base.set(sizeof(packet),dSG_BCS_INFO);
	packet.wIndexForBCS	=	_lpPacket->wIndex;
	strcpy(packet.strBCSIp,getBCSIP());
	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cGAME::operateRegClientSerial(BG_REG_CLIENT_SERIAL *_lpPacket)
{
	CClient	*lpClient	=	CM.mGETCLIENT(_lpPacket->wSerial);

	if	(!lpClient)
		return;

	lpClient->SetBCSerial(_lpPacket->wRealIndex);
}

void
cGAME::operateAskClientInfo(BG_ASK_CLIENT_INFO *_lpPacket)
{
	for	(int i=dGAME_MAX_USER_COUNT;i;)
	{
		--i;

		CClient	*lpClient	=	CM.mGETCLIENT(_lpPacket->wSerial);

		if	(!lpClient	||	lpClient->GetBCSerial()	!=	_lpPacket->wSerial)
			continue;

		lpClient->SetBCSerial(0xffff);
	}
}

void
cGAME::operateAskItemInfo(BG_ASK_ITEM_INFO *_lpPacket)
{
	cACTOR	*lpTarget	=	g_game.getPlayer(_lpPacket->strTargetName);

	if	(!lpTarget)
		return;

	SG_OTHER_PLAYER_ITEM_DATA	packet;

	packet.base.set(sizeof(SG_OTHER_PLAYER_ITEM_DATA),dSG_OTHER_PLAYER_ITEM_DATA);
	packet.iGold				=	lpTarget->m_iGold;
	packet.bf8ItemCount				=	0;
	packet.bf6ExtraInventorySize=	lpTarget->m_bf6ExtraInventorySize;

	int		i;

	for (i=0;i<dEQUIPMENT_PART_COUNT+dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE;i++)
		packet.m_aItems[i].reset();

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		if	(lpTarget->m_aEquip[i].m_wBaseItem	==	0xffff)
			continue;
			
		memcpy(&packet.m_aItems[packet.bf8ItemCount],&lpTarget->m_aEquip[i],sizeof(CItemDefine));
		packet.m_aItems[packet.bf8ItemCount].m_wSlot	=	i+dBORDER_OF_ITEM_AND_EQUIPMENT;
		packet.bf8ItemCount++;
	}

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		if	(lpTarget->m_aItems[i].m_wBaseItem	==	0xffff)
			continue;

		memcpy(&packet.m_aItems[packet.bf8ItemCount],&lpTarget->m_aItems[i],sizeof(CItemDefine));
		packet.m_aItems[packet.bf8ItemCount].m_wSlot	=	i;
		packet.bf8ItemCount++;
	}

	for (i=0;i<lpTarget->m_bf6ExtraInventorySize;i++)
	{
		if	(lpTarget->m_aExtraInventory[i].m_wBaseItem	==	0xffff)
			continue;

		memcpy(&packet.m_aItems[packet.bf8ItemCount],&lpTarget->m_aExtraInventory[i],sizeof(CItemDefine));
		packet.m_aItems[packet.bf8ItemCount].m_wSlot	=	i+dOWN_ITEM_COUNT;
		packet.bf8ItemCount++;
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.m_aItems)+packet.bf8ItemCount*sizeof(cItemDataForRevise);

	strcpy(packet.strId,lpTarget->m_strId);
	strcpy(packet.strName,lpTarget->m_strName);

	deliverPacket(NULL,&packet,_lpPacket->wAskerBCSerial);
}

#endif
//
//	월드에서 온 패킷 처리
void
cGAME::operateBCSPacket()
{
#ifdef	_USE_BROADCAST
	int		iSlot,iPacketType;
	char*	lpPacket;

	beginProfile("BCS 패킷 처리");
	
	mCS(m_csReceiveBCSPacket);	//	패킷 큐를 위한 메니져. -_-a

	for	(iSlot=0;iSlot<m_iReceivedBCSPacketCount;iSlot++)
	{
		lpPacket	=	m_apBCSPacketBuffer[iSlot];
		iPacketType	=	((cMSG_BASE_TYPE *)lpPacket)->wType;

		beginProfileByID(iPacketType);

		switch(iPacketType)
		{
			case	dBG_ASK_ITEM_INFO		:
				operateAskItemInfo((BG_ASK_ITEM_INFO *)lpPacket);
				break;

			case	dBG_ASK_CLIENT_INFO		:
				operateAskClientInfo((BG_ASK_CLIENT_INFO *)lpPacket);
				break;

			case	dBG_REG_CLIENT_RESULT	:
				operateRegClientResult((BG_REG_CLIENT_RESULT *)lpPacket);
				break;

			case	dBG_REG_CLIENT_SERIAL	:
				operateRegClientSerial((BG_REG_CLIENT_SERIAL *)lpPacket);
				break;
		}

		endProfileByID(iPacketType);
	}

	m_iReceivedBCSPacketCount	=	0;

	endProfile("BCS 패킷 처리");
#endif
}

void
cGAME::addBCSPacket(char *_lpPacketData)
{
	if	(m_apBCSPacketBuffer[0] == NULL)
		return;

	int	iPacketSize	=	((cMSG_BASE_TYPE *)_lpPacketData)->wSize;

	if	(iPacketSize >	(int)m_dwMaxBCSPacketSize)
	{
//		printf("치명정인 오류!!!! in cGAME::addDBPacket - iPacketSize >= m_dwMaxBCSPacketSize\n");
		CLOG("fatalError","in cGAME::addDBPacket - iPacketSize >= m_dwMaxBCSPacketSize");

		return;
	}

	if	(m_iReceivedBCSPacketCount	>= dDB_PACKET_BUFFER_SIZE)
	{
//		printf("치명정인 오류!!!! in cGAME::addDBPacket - m_iReceivedBCSPacketCount >= dDB_PACKET_BUFFER_SIZE\n");
		CLOG("fatalError","in cGAME::addDBPacket - m_iReceivedBCSPacketCount >= dDB_PACKET_BUFFER_SIZE");
		return;
	}

	mCS(m_csReceiveBCSPacket);	//	패킷 큐를 위한 메니져. -_-a
	memcpy(m_apBCSPacketBuffer[m_iReceivedBCSPacketCount],_lpPacketData,iPacketSize);

	++m_iReceivedBCSPacketCount;
}

void
cGAME::initBCSPacketReceiver()
{
	InitializeCriticalSection(&m_csReceiveBCSPacket);	//	패킷 큐를 위한 메니져. -_-a

	m_iReceivedBCSPacketCount	=	0;

	m_dwMaxBCSPacketSize		=	128;
	m_dwMaxBCSPacketSize		=	max(m_dwMaxBCSPacketSize,sizeof(BG_REG_CLIENT_RESULT));
	m_dwMaxBCSPacketSize		=	max(m_dwMaxBCSPacketSize,sizeof(BG_REG_CLIENT_SERIAL));

	printf("Maximum BCS packet size %dbyte\n",m_dwMaxBCSPacketSize);
	printf("size of BCS packet buffer %dbyte\n",m_dwMaxBCSPacketSize*dDB_PACKET_BUFFER_SIZE);

	for (int i=0;i<dDB_PACKET_BUFFER_SIZE;i++)
		m_apBCSPacketBuffer[i]	=	new char [m_dwMaxBCSPacketSize];
}

void
cGAME::closeBCSPacketReceiver()
{
	DeleteCriticalSection(&m_csReceiveBCSPacket);	//	패킷 큐를 위한 메니져. -_-a

	printf("release DB packet buffer\n");

	for (int i=0;i<dDB_PACKET_BUFFER_SIZE;i++)
		pKILL(m_apBCSPacketBuffer[i]);
}