#include "cGAME.h"

BOOL
cGAME::sendUseCarrotLog(cACTOR *_lpActor,int _iPremiumItemIndex,int _iCount,int _iPrice,BOOL _bIsIDPublicItem,
						char *_lpstrItemName,char *_lpstrReceiverID,char *_lpstrEtcBuffer)
{
	CClient	*lpClient	=	_lpActor->getClient();

	GDMSG_USE_CARROT_LOG	packet;

	packet.base.set(sizeof(packet),dGDMSG_USE_CARROT_LOG);

	strcpy(packet.strServerName,g_strGameServerName);
	strcpy(packet.strId,_lpActor->m_strId);
	strcpy(packet.strName,_lpActor->m_strName);

	packet.dwSerialInServer	=	_lpActor->m_iZoneSerial;
	packet.job				=	_lpActor->m_wJob;
	packet.lev				=	_lpActor->m_iLevel;
	packet.itemno			=	_iPremiumItemIndex;	//	구매한 아이템 번호
	packet.itcnt			=	_iCount;			//	구매한 아이템 갯수
	packet.sPrice			=	_iPrice;		//	당근수
	packet.wIsPublicItem	=	_bIsIDPublicItem;
	packet.strIP[0]			=	NULL;

	if	(_lpstrReceiverID)
		strcpy(packet.strReceiverId,_lpstrReceiverID);
	else
		strcpy(packet.strReceiverId,_lpActor->m_strId);

	if	(lpClient)
		strcpy(packet.strIP,lpClient->ip);

#ifdef	_OGP_SERVICE
	packet.ogpServiceKey.m_wGameServiceID	=	dOGP_GAME_SERVICE_ID;

	if	(_lpstrReceiverID)
		packet.ogpServiceKey.m_wUsedType		=	eOGP_BuyItem_UsedType_gift;
	else
		packet.ogpServiceKey.m_wUsedType		=	eOGP_BuyItem_UsedType_purchase;

	if	(_lpstrItemName)
		strcpy(packet.ogpServiceKey.m_strItemName,_lpstrItemName);
	else
		packet.ogpServiceKey.m_strItemName[0]	=	NULL;

	if	(_lpstrEtcBuffer)
		strcpy(packet.ogpServiceKey.m_strEtcInfo,_lpstrEtcBuffer);
	else
		packet.ogpServiceKey.m_strEtcInfo[0]	=	NULL;

	if	(lpClient)
		strcpy(packet.ogpServiceKey.m_strToken,lpClient->m_strToken);
#endif

	return	g_socketDBC.SendPacket((char *)&packet,packet.base.wSize);
}

BOOL
cGAME::sendGetCarrotCount(cACTOR *_lpActor)
{
	if	(_lpActor->m_wWaitDBMessageSecond)
		return	FALSE;

	CClient	*lpClient	=	_lpActor->getClient();
	
	GDMSG_GET_CARROT_COUNT	packet;
	packet.base.set(sizeof(GDMSG_GET_CARROT_COUNT),dGDMSG_GET_CARROT_COUNT);

	strcpy(packet.strId,_lpActor->m_strId);

	packet.dwSerialInServer		=	_lpActor->m_iZoneSerial;
	packet.time.year			=	g_currentTime.m_wYear%100;
	packet.time.month			=	g_currentTime.m_wMonth;
	packet.time.day				=	g_currentTime.m_wDay;
	packet.time.hour			=	g_currentTime.m_wHour;
	packet.time.minute			=	g_currentTime.m_wMinute;
	packet.time.second			=	g_currentTime.m_wSecond;

	_lpActor->m_wWaitDBMessageSecond	=	30;

	if	(lpClient)
		strcpy(packet.strIP,lpClient->ip);

#ifdef	_OGP_SERVICE
	packet.ogpServiceKey.m_wGameServiceID	=	dOGP_GAME_SERVICE_ID;

	if	(lpClient)
		strcpy(packet.ogpServiceKey.m_strToken,lpClient->m_strToken);
#endif

	return	g_socketDBC.SendPacket((char *)&packet,packet.base.wSize);
}