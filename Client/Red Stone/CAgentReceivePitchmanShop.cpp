#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CTitle.H"
#include "CWindowInterface.H"
#include "CPitchmanShop.H"

void
cPACKET_HANDLER::ReceiveOpenPitchmanShop(SERVER_PACKETS *_lpPacket)
{
	SG_OPEN_PITCHMAN_SHOP	*lpPacket	=	(SG_OPEN_PITCHMAN_SHOP	*)_lpPacket;

	g_gwPitchmanShop.openForShopkeeper(lpPacket);

	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;
	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
}

void
cPACKET_HANDLER::ReceiveAddPitchmanShopItem(SERVER_PACKETS *_lpPacket)//	노점상에 아이템 추가
{
	SG_ADD_PITCHMAN_SHOP_ITEM	*lpPacket	=	(SG_ADD_PITCHMAN_SHOP_ITEM	*)_lpPacket;

	g_gwPitchmanShop.addItem(lpPacket);

	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;
	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
}

void
cPACKET_HANDLER::ReceiveRemovePitchmanShopItem(SERVER_PACKETS *_lpPacket)//	노점상에 아이템 제거
{
	SG_REMOVE_PITCHMAN_SHOP_ITEM	*lpPacket	=	(SG_REMOVE_PITCHMAN_SHOP_ITEM	*)_lpPacket;

	g_gwPitchmanShop.removeItem(lpPacket);

	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;
	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
}

void
cPACKET_HANDLER::ReceiveChangePitchmanShopInfo(SERVER_PACKETS *_lpPacket)//	노점상 상태 변경
{
	SG_CHANGE_PITCHMAN_SHOP_INFO *lpPacket	=	(SG_CHANGE_PITCHMAN_SHOP_INFO	*)_lpPacket;

	if (lpPacket->info.m_bf10Serial	==	g_gwPitchmanShop.m_shop.m_wSerial)
		g_gwPitchmanShop.changeStatus(lpPacket);

	g_pitchmanShop.setShopInfo(&lpPacket->info,lpPacket->strName);

	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;
	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
}

//	노점상 아이템 위치 변경
void
cPACKET_HANDLER::ReceiveChangePitchmanShopItemPlace(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE	*lpPacket	=	(SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE	*)_lpPacket;

	g_gwPitchmanShop.changeItemPlace(lpPacket);

	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;
	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
}

void
cPACKET_HANDLER::ReceiveBuyPitchmanShopItem(SERVER_PACKETS *_lpPacket)//	노점상에서 아이템 구입
{
	SG_BUY_PITCHMAN_SHOP_ITEM	*lpPacket	=	(SG_BUY_PITCHMAN_SHOP_ITEM	*)_lpPacket;

	g_gwPitchmanShop.buyItem(lpPacket);

	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;
	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
}

void
cPACKET_HANDLER::ReceiveSoldPitchmanShopItem(SERVER_PACKETS *_lpPacket)//	노점상에서 아이템이 팔렸다.
{
	SG_SOLD_PITCHMAN_SHOP_ITEM	*lpPacket	=	(SG_SOLD_PITCHMAN_SHOP_ITEM	*)_lpPacket;

	g_gwPitchmanShop.soldItem(lpPacket);

	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;
	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
}

void
cPACKET_HANDLER::ReceiveAddPitchmanShop(SERVER_PACKETS *_lpPacket)//	노점상 추가
{
	SG_ADD_PITCHMAN_SHOP	*lpPacket	=	(SG_ADD_PITCHMAN_SHOP	*)_lpPacket;

	g_pitchmanShop.setShopInfo(&lpPacket->info," ");
}

void
cPACKET_HANDLER::ReceiveRemovePitchmanShop(SERVER_PACKETS *_lpPacket)//	노점상 제거
{
	SG_REMOVE_PITCHMAN_SHOP	*lpPacket	=	(SG_REMOVE_PITCHMAN_SHOP	*)_lpPacket;

	g_pitchmanShop.remove(lpPacket->wSerial);

	if (lpPacket->wSerial	==	g_gwPitchmanShop.m_shop.m_wSerial)
		g_gwPitchmanShop.closeByServer();
}

void
cPACKET_HANDLER::ReceivePitchmanShopList(SERVER_PACKETS *_lpPacket)//	노점상 리스트
{
	SG_PITCHMAN_SHOP_LIST	*lpPacket	=	(SG_PITCHMAN_SHOP_LIST	*)_lpPacket;

	if (lpPacket->bf1IsFirst)
		g_pitchmanShop.reset();

	for (int i=0;i<lpPacket->bf15Count;i++)
		g_pitchmanShop.addTemplate(lpPacket->aList[i].wSerialInField,lpPacket->aList[i].wX,lpPacket->aList[i].wY);
}

//	노점상 리스트
void
cPACKET_HANDLER::ReceiveSimplePitchmanShopInfoList(SERVER_PACKETS *_lpPacket)
{
	SG_SIMPLE_PITCHMAN_SHOP_INFO_LIST	*lpPacket	=	(SG_SIMPLE_PITCHMAN_SHOP_INFO_LIST	*)_lpPacket;

	int		iDataPoint	=	0;
	char	strName[dPITCHMAN_SHOP_NAME_LENGTH];

	for (int i=0;i<lpPacket->wCount;i++)
	{
		CSimplePitchmanShopInfo	*lpInfo	=	(CSimplePitchmanShopInfo *)&lpPacket->aData[iDataPoint];

		iDataPoint	+=	sizeof(CSimplePitchmanShopInfo);

		strcpy(strName,lpPacket->aData+iDataPoint);

		for (int iP=0;iP<dPITCHMAN_SHOP_NAME_LENGTH;iP++)
			if	(strName[iP]	==	'<')
				strName[iP]	=	'[';

		iDataPoint	+=	(strlen(strName)+1);

		g_pitchmanShop.setShopInfo(lpInfo,strName);
	}
}

void
cPACKET_HANDLER::ReceivePitchmanShopInfo(SERVER_PACKETS *_lpPacket)//	노점상 정보
{
	SG_PITCHMAN_SHOP_INFO	*lpPacket	=	(SG_PITCHMAN_SHOP_INFO	*)_lpPacket;

	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;

	g_gwPitchmanShop.openForClient(lpPacket);
}

//	노점상 메시지
void
cPACKET_HANDLER::ReceiveResultOfSearchItemInRelayStation(SERVER_PACKETS *_lpPacket)
{
}

void
cPACKET_HANDLER::ReceivePitchmanShopMessage(SERVER_PACKETS *_lpPacket)//	노점상 메시지
{
	SG_PITCHMAN_SHOP_MESSAGE	*lpPacket	=	(SG_PITCHMAN_SHOP_MESSAGE	*)_lpPacket;

	g_pitchmanShop.m_wIsWaitWorkResult				=	FALSE;
	g_gwPitchmanShop.m_wIsWaitPichmanShopWorkResult	=	FALSE;

	switch(lpPacket->wMessage)
	{
		case	ePSM_SETUP_FAILED_BY_REQUIRE_DEALER_TITLE		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_REQUIRE_DEALER_TITLE_FOR_OPEN_PITCHMAN_SHOP);
			break;
		case	ePSM_SETUP_FAILED_BY_EXIT_ANOTHER_PITCHMAN_SHOP	:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_EXIST_ANOTHER_PITCHMAN_SHOP);
			break;
		case	ePSM_SETUP_FAILED_BY_TOO_MANY_PITCHMAN_SHOP		:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TOO_MANY_PITCHMAN_SHOP);
			break;
		case	ePSM_SETUP_FAILED								:
			g_gwPitchmanShop.close();
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_OPEN_PITCHMAN_SHOP);
			break;
		case	ePSM_FAILED_REGIST_ITEM_BY_ALREADY_REGISTED		:
			g_gwPitchmanShop.close();
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_ADD_ITEM_BY_ALREADY_ADDED_PITCHMAN_SHOP_ITEM);
			break;
		case	ePSM_FAILED_REGIST_ITEM_BY_VALID_INVENTORY_SLOT	:
			g_gwPitchmanShop.close();
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_ADD_ITEM_BY_VALID_INVENTORY_SLOT);
			break;
		case	ePSM_FAILED_REMOVE_ITEM_BY_VALID_SLOT			:
			g_gwPitchmanShop.close();
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_REMOVE_ITEM_BY_VALID_SLOT);
			break;

		case	ePSM_FAILED_SHOP_NOT_OPENED						:
		case	ePSM_FAILED_CLOSE_SHOP_BY_NOT_OPENED			:
			g_gwPitchmanShop.close();
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_NOT_OPENED_PITCHMAN_SHOP);
			break;
		case	ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO		:			//	슬롯이 비었다.
			g_gwPitchmanShop.close();
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO);
			break;
		case	ePSM_FAILED_BUY_ITEM_BY_INSUFFICIENT_GOLD		:	//	돈이 모자라서 실패
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_INSUFFICIENT_GOLD_TO_BY_PITCHMAN_SHOP_ITEM);
			break;
		case	ePSM_FAILED_BUY_ITEM_BY_FULL_GOLD				:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_TOO_MANY_GOLD);
			break;
		case	ePSM_FAILED_BUY_ITEM_BY_FULL_INVENTORY			:
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_FAILED_BUY_ITEM_BY_FULL_INVENTORY);
			break;
		case	ePSM_FAILED_OPEN_SHOP_BY_NOT_OPENED				://	아직 오픈 안했다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_READY_TO_OPEN_PITCHMAN_SHOP);
			break;

		case	ePSM_FAILED_BY_CAN_NOT_TRADE_ITEM				:	//	거래할 수 없는 아이템이다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_TRADE_ITEM);
			break;

		case	ePSM_FAILED_BY_INCORRECT_NAME					:	//	거래할 수 없는 아이템이다.
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_PSM_FAILED_BY_INCORRECT_NAME);
			break;
		case ePSM_FAILED_BUY_ITEM_BY_INSUFFICIENT_GOLD_BAR		:	// 금괴가 부족해서 실패 09.08.21
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_INSUFFICIENT_GOLD_BAR_TO_BY_PITCHMAN_SHOP_ITEM);
			break;
		case ePSM_FAILED_BUY_ITEM_BY_OWNER_NOT_SLOT		:	// 판매자의 인벤토리 부족 09.08.21
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_PITCHMAN_SHOP_OWNER_NOT_SLOT);
			break;
		case ePSM_FAILED_BUY_ITEM_BY_MANY_GOLD_BAR		:	// 자신의 금괴가 너무 많아용
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_POSSESS_NO_MORE_THE_GOLD);
			break;
		case ePSM_CAN_NOT_OPEN_THIS_FIELD		:	// 현재필드에서는 할수 없어요
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAN_NOT_OPEN_PITCHMAN_SHOP_THIS_FIELD);
			break;
	}
}