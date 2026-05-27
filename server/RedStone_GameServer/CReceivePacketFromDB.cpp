#include "cGAME.H"
#include "cFIELD.H"
#include "cPROJECT.H"
#include "STDAFX.H"
#include "cDECODER.H"
#include "time.H"
#include "cPACKET_GAMESERVER.h"
#include "CImageData.h"
#include "Client.h"
#include "CShoppingCart.h"
#include "cEvent.h"
#include "GameDLL.H"

//
//	
void
cGAME::operateBankInfo(DWMSG_BANKOPEN *_lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->iActorSerial);

	if (!lpActor)	return;

	if (STRICMP(lpActor->m_strId,_lpPacket->cBank.m_strId) != 0) 
		return;	//	내것이 아닌디?

	lpActor->openBank(&_lpPacket->cBank);
}

//
//	저장 결과
void
cGAME::operateBankSaveResult(DWMSG_SAVEBANK* _lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->iActorSerial);

	if (!lpActor)
		return;

	switch(_lpPacket->wResult)
	{
		case	eRBDS_SUCCESS	:
			lpActor->finishBankTransaction();
			break;

		default					:
			lpActor->cancelBankTransaction(FALSE);
			lpActor->sendBankMessage(eMOB_FAILED);
			break;
	}
}

//
//	구입한 프리미엄 아이템 리스트를 받았다.
void
cGAME::operatePremiumItemList(DGMSG_BUYLIST * _lpPacket)
{
	cACTOR *lpActor	=	getActor(_lpPacket->dwSerialInServer);

	if (!lpActor)
		return;

	if (STRICMP(lpActor->m_strId,_lpPacket->strId) != 0)
		return;

	if (_lpPacket->wCount >= dMAX_BUYLIST)
	{
//		printf("_lpPacket->wCount >= dMAX_BUYLIST %d\n",_lpPacket->wCount);
		CLOG("error","in cGAME::operatePremiumItemList  - _lpPacket->wCount >= dMAX_BUYLIST %d",_lpPacket->wCount);

		_lpPacket->wCount	=	dMAX_BUYLIST;

//		lpActor->sendCartMessage(eCM_TOO_MANY_ITEM_COUNT);
//		return;
	}

	lpActor->openCart(_lpPacket->List,_lpPacket->wCount);
}

//
//	프리미엄 아이템 꺼내기 결과를 받았다.
void
cGAME::operateWithdrawPremiumItemResult(DGMSG_USEBUY* _lpPacket)
{
	cACTOR *lpActor	=	getActor(_lpPacket->dwSerialInServer);

	if	(!lpActor)
		return;

	if	(STRICMP(lpActor->m_strId,_lpPacket->strId) != 0)
		return;

	lpActor->m_wIsWaitCartMessage	=	FALSE;

	switch(_lpPacket->wResult)
	{
		case	dRESULT_USEBUYITEM_SUCCESS		:
		{
			lpActor->withdrawCartItem(_lpPacket->wCartSlot,_lpPacket->wInventorySlot);
			break;
		}
		
		case	dRESULT_USEBUYITEM_DESTROY_SUCCESS		:
		{
			lpActor->destroyCartItem(_lpPacket->wCartSlot);
			break;
		}

		case	dRESULT_USEBUYITEM_DESTROY_FAILED		:
		{
			lpActor->sendDestroyCartItemResult(eDCIR_FAILED,_lpPacket->wCartSlot);
			break;
		}

		case	dRESULT_USEBUYITEM_USED			:		//	이미 사용한건데 -_-;
		{
			printf("operateWithdrawPremiumItemResult - already use item\n");
			CLOG("cart","operateWithdrawPremiumItemResult - already use item");
			lpActor->failedCart();
			break;
		}
		case	dRESULT_USEBUYITEM_NOTFIND		:		//	그런 아이템이 없다.
		{
			printf("operateWithdrawPremiumItemResult - can not find item\n");
			CLOG("cart","operateWithdrawPremiumItemResult - can not find item");
			lpActor->failedCart();
			break;
		}

		default									:		//	뭘까?
		{
			printf("operateWithdrawPremiumItemResult - unknown error!! %d\n",_lpPacket->wResult);
			CLOG("cart","operateWithdrawPremiumItemResult - unknown error!! %d",_lpPacket->wResult);
			lpActor->failedCart();
			break;
		}
	}
}

void
cGAME::operateProvisionRank(DGMSG_PROVISION_RANK_RET *_lpPacket)
{
	cRedStonePointRank*	lpRedStoneRank	=	g_eventManager.getRedStoneRankManager();

	lpRedStoneRank->reset();

	for	(int i=_lpPacket->wCount;i;)
	{
		--i;

		lpRedStoneRank->setRank(i,_lpPacket->list[i].wRank,_lpPacket->list[i].strName);
	}

	lpRedStoneRank->setCount(_lpPacket->wCount);

	m_bIsRequireRebuildRedStonePointTitle	=	TRUE;
}

void
cGAME::operateUseCarrotResult(DGMSG_USE_CARROT_RESULT *_lpPacket)
{
	cACTOR *lpActor	=	getActor(_lpPacket->dwSerialInServer);

	if	(!lpActor)
		return;

	if	(STRICMP(lpActor->m_strId,_lpPacket->strId) != 0)
		return;

	lpActor->m_wIsWaitCartMessage				=	FALSE;
	lpActor->m_wWaitBuyCarrotShopItemResultTime	=	0;

	switch(_lpPacket->wResult)
	{
		case	eUCR_SUCCESS			:	//	성공
			lpActor->buyCarrotShopItem(_lpPacket->itemno,_lpPacket->itcnt,_lpPacket->crcnt,_lpPacket->wPrice);
			break;
		case	eUCR_NOTFOUNDUSER		:	//	해당 유저를 찾을수 없다.
			break;
		case	eUCR_LOW_COUNT			:	//	당근 갯수가 모잘라
			lpActor->m_iCarrotCount	=	_lpPacket->crcnt;
			lpActor->sendBuyCarrotShopItemResult(eBCIR_LOW_CARROT,_lpPacket->crcnt);
			break;
		case	eUCR_AGREEMENT_FAILED	:	//	웹 에서 약관 승인 안함
			lpActor->sendEtcWork(eEW_WITHDRAW_CARROT_AGREEMENT_FAILED);
			break;
		case	eUCR_FAILED				:	//	여튼 실패	
			break;
	}
}

void
cGAME::operateGetCarrotCount(DGMSG_GET_CARROT_COUNT_RESULT	*_lpPacket)
{
	cACTOR *lpActor	=	getActor(_lpPacket->dwSerialInServer);

	if	(!lpActor)
		return;

	if	(STRICMP(lpActor->m_strId,_lpPacket->strId) != 0)
		return;

	lpActor->m_wIsWaitCartMessage	=	FALSE;

	switch(_lpPacket->wResult)
	{
		case	eGCR_SUCCESS			:
		{
			if	(_lpPacket->iRemainCarrotCount	<	-100)
			{
				lpActor->sendEtcWork(eEW_WITHDRAW_CARROT_AGREEMENT_FAILED);

				break;
			}

			if	(_lpPacket->iRemainCarrotCount	<	0)
			{
				lpActor->sendEtcWork(eEW_WITHDRAW_CARROT_AGREEMENT_FAILED);

				break;
			}

			WORD	wCarrot1	=	_lpPacket->iRemainCarrotCount>>16;
			WORD	wCarrot2	=	_lpPacket->iRemainCarrotCount&0xffff;

			lpActor->m_iCarrotCount	=	_lpPacket->iRemainCarrotCount;
			lpActor->sendEtcWork(eEW_OPEN_CARROT_SHOP,wCarrot1,wCarrot2);
			break;
		}

		default									:		//	뭘까?
		{
			printf("[%s/%s] DGMSG_GET_CARROT_COUNT_RESULT알수 없는 에러!! [%d]\n",lpActor->m_strId,lpActor->m_strName,_lpPacket->wResult);
			break;
		}
	}
}

void
cGAME::operateLogoutResult(DGMSG_LOGOUTAVATAR* _lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->iActorSerialInServer);

	if (!lpActor)
	{
//		printf("로그아웃 버그 : 시리얼로 이름 찾으니까 없다 [%s]\n",_lpPacket->strName);
		CLOG("logOutError","시리얼로 이름 찾으니까 없다 [%s]",_lpPacket->strName);

		lpActor	=	getPlayer(_lpPacket->strName);

		if (!lpActor)
		{
//			printf("로그아웃 버그 : 클라이언트를 이름으로 검색해서 제거 시도 [%s]\n",_lpPacket->strName);
			CLOG("logOutError","클라이언트를 이름으로 검색해서 제거 시도 [%s]",_lpPacket->strName);

			int	iRemoveClientCount	=	operteLogoutActorWorkByName(_lpPacket->strName);

//			printf("로그아웃 버그 : 이름이 [%s]인 클라이언트 %d명 제거\n",_lpPacket->strName,iRemoveClientCount);
			CLOG("logOutError","이름이 [%s]인 클라이언트 %d명 제거",_lpPacket->strName,iRemoveClientCount);
			return;
		}

		if (lpActor->m_dwSaveCount > _lpPacket->dwSaveCount)
		{
//			printf("로그아웃 버그 : 거기다 세이브 카운트가 기존의것이 커서 다시 로그아웃 시도\n");
			CLOG("logOutError","거기다 세이브 카운트가 기존의것이 커서 다시 로그아웃 시도");

			if	(lpActor->isDummy())
			{
				CLOG("dummy","cGAME::operateLogoutResult");
				return;
			}

			lpActor->readyToExitGame();
			return;
		}
	}

	if (STRCMP(lpActor->m_strName,_lpPacket->strName) != 0)
	{
//		printf("로그아웃 버그 : 시리얼로 이름 찾으니까 엉뚱한 녀석이 나옴 [%s] ->[%s/%s]\n",_lpPacket->strName,lpActor->m_strId,lpActor->m_strName);
		CLOG("logOutError","시리얼로 이름 찾으니까 엉뚱한 녀석이 나옴 [%s] ->[%s/%s]",_lpPacket->strName,lpActor->m_strId,lpActor->m_strName);

		lpActor	=	getPlayer(_lpPacket->strName);

		if (!lpActor)
		{
//			printf("로그아웃 버그 : 클라이언트를 이름으로 검색해서 제거 시도 [%s]\n",_lpPacket->strName);
			CLOG("logOutError","클라이언트를 이름으로 검색해서 제거 시도 [%s]",_lpPacket->strName);

			int	iRemoveClientCount	=	operteLogoutActorWorkByName(_lpPacket->strName);

//			printf("로그아웃 버그 : 이름이 [%s]인 클라이언트 %d명 제거\n",_lpPacket->strName,iRemoveClientCount);
			CLOG("logOutError","이름이 [%s]인 클라이언트 %d명 제거",_lpPacket->strName,iRemoveClientCount);
			return;
		}

		if (lpActor->m_dwSaveCount > _lpPacket->dwSaveCount)
		{
//			printf("로그아웃 버그 : 거기다 세이브 카운트가 기존의것이 커서 다시 로그아웃 시도\n");
			CLOG("logOutError","거기다 세이브 카운트가 기존의것이 커서 다시 로그아웃 시도");

			if	(lpActor->isDummy())
			{
				CLOG("dummy","cGAME::operateLogoutResult");
				return;
			}

			lpActor->readyToExitGame();
			return;
		}
	}

	switch(_lpPacket->wResult)
	{
		case	eSAVE_SUCCESS_DB		:
			break;

		case	eSAVE_FAIL_LITTLECOUNT	:
//			printf("로그아웃 저장 실패 eSAVE_FAIL_LITTLECOUNT %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("logOutError","로그아웃 저장 실패 eSAVE_FAIL_LITTLECOUNT %s/%s",lpActor->m_strId,lpActor->m_strName);
			break;

		case	eSAVE_CAN_NOT_FIND_DATA	:
//			printf("로그아웃 저장 실패 eSAVE_CAN_NOT_FIND_DATA %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("logOutError","로그아웃 저장 실패 eSAVE_CAN_NOT_FIND_DATA %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;

		case	eSAVE_DB_FAILED			:
//			printf("로그아웃 저장 실패 eSAVE_DB_FAILED %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("logOutError","로그아웃 저장 실패 eSAVE_DB_FAILED %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;

		case	eSAVE_DB_INCORRECT_DATA	:
//			printf("로그아웃 저장 실패 eSAVE_DB_INCORRECT_DATA %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("logOutError","로그아웃 저장 실패 eSAVE_DB_INCORRECT_DATA %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;

		case	eSAVE_DB_UNKNOWN_ERROR	:
//			printf("로그아웃 저장 실패 eSAVE_DB_UNKNOWN_ERROR %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("logOutError","로그아웃 저장 실패 eSAVE_DB_UNKNOWN_ERROR %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;

		case	eSAVE_DB_DATA_NULL		:
//			printf("로그아웃 저장 실패 eSAVE_DB_DATA_NULL %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("logOutError","로그아웃 저장 실패 eSAVE_DB_DATA_NULL %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;

		default							:
//			printf("로그아웃 저장 실패 default?? %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("logOutError","로그아웃 저장 실패 default?? %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;
	}

	lpActor->m_wWaitLogoutResultTime	=	0;
	lpActor->bookingToExitGame();
}

//
//
void
cGAME::operateSaveDataToFileResult(DWMSG_SAVEAVA_TO_FILE* _lpPacket)
{
	cACTOR	*lpActor	=	getActor(_lpPacket->iSerialInServer);

	if (!lpActor)
	{
//		printf("필드이동 버그 : 시리얼로 이름 찾으니까 없다!! [%s]\n",_lpPacket->strName);
		CLOG("moveFieldError","시리얼로 이름 찾으니까 없다!! [%s]",_lpPacket->strName);

		lpActor	=	getPlayer(_lpPacket->strName);

		if	(lpActor)
		{
			if	(lpActor->isDummy())
			{
				CLOG("dummy","cGAME::operateSaveDataToFileResult");
				return;
			}

			lpActor->readyToExitGame();
		}

		return;
	}

	if (STRCMP(lpActor->m_strName,_lpPacket->strName) != 0)
	{
		printf("moveFieldError : mismatch serial and name [%s] ->%s/%s\n",_lpPacket->strName,lpActor->m_strId,lpActor->m_strName);
		CLOG("moveFieldError","시리얼로 이름 찾으니까 엉뚱한 녀석이 나옴 [%s] ->[%s/%s]",_lpPacket->strName,lpActor->m_strId,lpActor->m_strName);

		lpActor	=	getPlayer(_lpPacket->strName);

		if	(lpActor)
		{
			if	(lpActor->isDummy())
			{
				CLOG("dummy","cGAME::operateSaveDataToFileResult");
				return;
			}
			
			lpActor->readyToExitGame();
		}

		return;
	}

	CClient	*lpClient		=lpActor->getClient();

//	if	(lpClient)
//		lpClient->SetIsUsed(eISUSED_DWMSG_SAVEAVA_TO_FILE);

	lpActor->m_wWaitMoveFieldResultTime	=	0;

	switch(_lpPacket->wResult)
	{
		case	eSAVE_SUCCESS_SAVEFILE			:	//	성공
			if (lpActor->m_wIsMoveToDifferentServer)
				g_game.sendWPUserLogoutForMoveField(lpActor);
			else
				lpActor->bookingToExitGameForMoveField();
			return;

		case	eSAVE_CAN_NOT_FIND_LOGIN_USER	:
//			printf("필드 이동 실패!! 로그인 정보가 없댜. %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("moveFieldError","필드 이동 실패!! 로그인 정보가 없댜. %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;

		case	eSAVE_MEMORY_FAIL_LITTLECOUNT	:
//			printf("필드 이동 실패!! 세이브 카운트가 적댜 %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("moveFieldError","필드 이동 실패!! 세이브 카운트가 적댜 %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;

		default		:
//			printf("필드 이동 실패!! default가 왠말이냐!!!! %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			CLOG("moveFieldError","필드 이동 실패!! default가 왠말이냐!!!! %s/%s\n",lpActor->m_strId,lpActor->m_strName);
			break;
	}

	lpActor->bookingToExitGame();
}

void
cGAME::operateKillAvatar(GDMSG_KILLAVATAR* _lpPacket)
{
	cACTOR	*lpActor	=	getPlayer(_lpPacket->strId);

	if (!lpActor)	return;

	lpActor->bookingToExitGame();
}

//
//	월드에서 온 패킷 처리
void
cGAME::operateDBPacket()
{
	int		iSlot,iPacketType;
	char*	lpPacket;

	beginProfile("DB 패킷 처리");
	
	mCS(m_csReceiveDBPacket);	//	패킷 큐를 위한 메니져. -_-a

	for	(iSlot=0;iSlot<m_iReceivedDBPacketCount;iSlot++)
	{
		lpPacket	=	m_apDBPacketBuffer[iSlot];
		iPacketType	=	((cMSG_BASE_TYPE_FORDB *)lpPacket)->wType;

		mDS_PK.m_iLastUpdateDBPacket	=	iPacketType;

		beginProfileByID(iPacketType);

		switch(iPacketType)
		{
			case	dDGMSG_PROVISION_RANK_RET	:
				operateProvisionRank((DGMSG_PROVISION_RANK_RET *)lpPacket);
				break;

			case	dDGMSG_USE_CARROT_RESULT	:
				operateUseCarrotResult((DGMSG_USE_CARROT_RESULT *)lpPacket);
				break;
				
			case	dDGMSG_GET_CARROT_COUNT_RESULT	:
				operateGetCarrotCount((DGMSG_GET_CARROT_COUNT_RESULT *)lpPacket);
				break;
			
			case	dDWMSG_BANKOPEN			:
				operateBankInfo((DWMSG_BANKOPEN *)lpPacket);
				break;

			case	dDWMSG_SAVEBANK			:
				operateBankSaveResult((DWMSG_SAVEBANK*)lpPacket);
				break;

			case	dDGMSG_BUYLIST			:
				operatePremiumItemList((DGMSG_BUYLIST *)lpPacket);
				break;

			case	dDGMSG_USEBUY			:
				operateWithdrawPremiumItemResult((DGMSG_USEBUY*)lpPacket);
				break;

			case	dDGMSG_LOGOUTAVATAR		:
				operateLogoutResult((DGMSG_LOGOUTAVATAR*)lpPacket);
				break;

			case	dDWMSG_SAVEAVA_TO_FILE	:
				operateSaveDataToFileResult((DWMSG_SAVEAVA_TO_FILE*)lpPacket);
				break;

			case	dGDMSG_KILLAVATAR		:
				operateKillAvatar((GDMSG_KILLAVATAR*)lpPacket);
				break;
		}

		endProfileByID(iPacketType);
	}

	mDS_PK.m_iLastUpdateDBPacket	=	-1;

	m_iReceivedDBPacketCount	=	0;

	endProfile("DB 패킷 처리");
}

void
cGAME::addDBPacket(char *_lpPacketData)
{
	if (m_apDBPacketBuffer[0] == NULL)
		return;

	int	iPacketSize	=	((cMSG_BASE_TYPE_FORDB*)_lpPacketData)->wSize;

	if (iPacketSize >	(int)m_dwMaxDBPacketSize)
	{
//		printf("치명정인 오류!!!! in cGAME::addDBPacket - iPacketSize >= m_dwMaxDBPacketSize\n");
		CLOG("fatalError","in cGAME::addDBPacket - iPacketSize >= m_dwMaxDBPacketSize");

		return;
	}

	if (m_iReceivedDBPacketCount	>= dDB_PACKET_BUFFER_SIZE)
	{
//		printf("치명정인 오류!!!! in cGAME::addDBPacket - m_iReceivedDBPacketCount >= dDB_PACKET_BUFFER_SIZE\n");
		CLOG("fatalError","in cGAME::addDBPacket - m_iReceivedDBPacketCount >= dDB_PACKET_BUFFER_SIZE");
		return;
	}

	mCS(m_csReceiveDBPacket);	//	패킷 큐를 위한 메니져. -_-a
	memcpy(m_apDBPacketBuffer[m_iReceivedDBPacketCount],_lpPacketData,iPacketSize);

	++m_iReceivedDBPacketCount;
}

void
cGAME::initDBPacketReceiver()
{
	FPInitCS(&m_csReceiveDBPacket);	//	패킷 큐를 위한 메니져. -_-a

	m_iReceivedDBPacketCount=	0;

	m_dwMaxDBPacketSize		=	0;
	m_dwMaxDBPacketSize		=	max(m_dwMaxDBPacketSize,sizeof(DWMSG_BANKOPEN));
	m_dwMaxDBPacketSize		=	max(m_dwMaxDBPacketSize,sizeof(DGMSG_BUYLIST));
	m_dwMaxDBPacketSize		=	max(m_dwMaxDBPacketSize,sizeof(DGMSG_USEBUY));
	m_dwMaxDBPacketSize		=	max(m_dwMaxDBPacketSize,sizeof(DGMSG_LOGOUTAVATAR));
	m_dwMaxDBPacketSize		=	max(m_dwMaxDBPacketSize,sizeof(DWMSG_SAVEAVA_TO_FILE));
	m_dwMaxDBPacketSize		=	max(m_dwMaxDBPacketSize,sizeof(GDMSG_KILLAVATAR));

	printf("Maximum DB packet size %dbyte\n",m_dwMaxDBPacketSize);
	printf("size of DB packet buffer %dbyte\n",m_dwMaxDBPacketSize*dDB_PACKET_BUFFER_SIZE);

	for (int i=0;i<dDB_PACKET_BUFFER_SIZE;i++)
		m_apDBPacketBuffer[i]	=	new char [m_dwMaxDBPacketSize];
}

void
cGAME::closeDBPacketReceiver()
{
	DeleteCriticalSection(&m_csReceiveDBPacket);	//	패킷 큐를 위한 메니져. -_-a

	printf("release DB packet buffer\n");

	for (int i=0;i<dDB_PACKET_BUFFER_SIZE;i++)	pKILL(m_apDBPacketBuffer[i]);
}