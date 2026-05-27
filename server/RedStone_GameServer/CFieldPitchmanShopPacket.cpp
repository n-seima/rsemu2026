#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"
#include "CPitchmanShop.H"
#include "packetManager.H"

//
//	노점 개설
void
cFIELD::receiveAskOpenThePitchmanShop(CG_ASK_OPEN_THE_PITCHMAN_SHOP *_lpPacket,int _iSerial)
{
	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		return;

	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	int		iResult	=	lpActor->openPitchmanShop();
}

void
cFIELD::receiveAddPitchmanShopItem(CG_ADD_PITCHMAN_SHOP_ITEM *_lpPacket,int _iSerial)		// 09.08.21
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->addPitchmanShopItem(_lpPacket->wSerial,_lpPacket->wInventorySlot,_lpPacket->wShopSlot,_lpPacket->dwPrice, _lpPacket->bDenomination);
}

void
cFIELD::receiveRemovePitchmanShopItem(CG_REMOVE_PITCHMAN_SHOP_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;
	
	lpActor->removePitchmanShopItem(_lpPacket->wSerial,_lpPacket->wSlot);
}

void
cFIELD::receiveChangePitchmanShopInfo(CG_CHANGE_PITCHMAN_SHOP_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;
	
	lpActor->changePitchmanShopInfo(_lpPacket->wSerial,_lpPacket->bf2Status,_lpPacket->strName,_lpPacket->bf10SignboardShape,_lpPacket->wTextColor,_lpPacket->bf1IsBold,_lpPacket->bf1IsShadowText);
}

void
cFIELD::receiveChangePitchmanShopItemPlace(CG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->changePitchmanShopPlace(_lpPacket->wSerial,_lpPacket->wSlot1,_lpPacket->wSlot2);
}

void
cFIELD::receiveClosePitchmanShop(CG_CLOSE_PITCHMAN_SHOP *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->closePitchmanShop(_lpPacket->wSerial);
}

void
cFIELD::receiveBuyPitchmanShopItem(CG_BUY_PITCHMAN_SHOP_ITEM *_lpPacket,int _iSerial)		// 09.08.21
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;
	
	lpActor->buyPitchmanShopItem(_lpPacket->wPitchmanSerial,_lpPacket->wSlot,(cItem *)&_lpPacket->item,_lpPacket->dwPrice, _lpPacket->bDenomination);
}

void
cFIELD::receiveAskPitchmanShopInfo(CG_ASK_PITCHMAN_SHOP_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->askPitchmanShopInfo(_lpPacket->wPitchmanSerial);
}

void
cFIELD::receiveAskSimplePithmanShopInfoList(CG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	SG_SIMPLE_PITCHMAN_SHOP_INFO_LIST	packet;
	int		iDataPoint	=	0;

	packet.base.set(sizeof(packet),dSG_SIMPLE_PITCHMAN_SHOP_INFO_LIST);
	packet.wCount	=	0;

	if (_lpPacket->wCount	>	dMAX_ASK_SIMPLE_PITCHMAN_SHOP_INFO_COUNT)
	{
		CLOG("packetHacking","[%.3d] [%s/%s] cFIELD::receiveAskSimplePithmanShopInfoList [%d]",lpActor->m_wCurrentField,lpActor->m_strId,lpActor->m_strName,_lpPacket->wCount);

		lpActor->setPeneltyTime(dPENELTY_TIME_FOR_SPEEDHACKUSER,ePPPTR_PACKET_HACKING);

		return;
	}

	for (int i=0;i<_lpPacket->wCount;i++)
	{
		int				iSerial	=	_lpPacket->awList[i];
		CPitchmanShop	*lpShop	=	g_pitchmanShopManager.get(iSerial);

		CSimplePitchmanShopInfo	*lpInfo	=	(CSimplePitchmanShopInfo *)&packet.aData[iDataPoint];

		if (!lpShop || !lpShop->getSimpleShopInfo(lpInfo))
		{
			SG_REMOVE_PITCHMAN_SHOP	packetRemove;

			packetRemove.base.set(sizeof(packet),dSG_REMOVE_PITCHMAN_SHOP);
			packetRemove.wSerial	=	iSerial;

			g_userPM.add(lpActor->m_iClientSerial,&packetRemove,packetRemove.base.wSize);

			continue;
		}

		iDataPoint	+=	sizeof(CSimplePitchmanShopInfo);
		
		if (lpInfo->m_bf2Status	==	ePSS_READY)
		{
			strcpy(packet.aData+iDataPoint," ");
			iDataPoint	+=	2;
		}
		else
		{
			strcpy(packet.aData+iDataPoint,lpShop->m_strName);
			iDataPoint	+=	(strlen(lpShop->m_strName)+1);
		}
		packet.wCount++;

		if (iDataPoint+dPITCHMAN_SHOP_NAME_LENGTH+10 >=	sizeof(packet.aData))
		{
			packet.base.wSize	=	sizeof(packet) - sizeof(packet.aData)+iDataPoint;

			g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

			packet.wCount		=	0;
			iDataPoint			=	0;
		}
	}

	if (packet.wCount)
	{
		packet.base.wSize	=	sizeof(packet) - sizeof(packet.aData)+iDataPoint;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}
}