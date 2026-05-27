#include "CShoppingCart.H"
#include "CField.H"
#include "CServer.H"
#include "CGame.H"
#include "packetManager.H"
#include "GameDLL.H"

CShoppingCartManager	g_cartManager;

//
//	정상적인 데이터?
BOOL
CShoppingCart::checkIsIncorrectData(DWORD _dwChecksum)
{
	if	(getCheckSum()	!=	_dwChecksum	)
		return	TRUE;

	return	FALSE;
}

//
//	카트 오픈
void
CShoppingCart::open(tsBuyLst *_lpCartInfo,int _iItemCount,cACTOR *_lpActor)
{
	m_wItemCount		=	_iItemCount;
	memcpy(m_aItemList,_lpCartInfo,_iItemCount*sizeof(tsBuyLst));
	m_wItemCount		=	_iItemCount;
	m_lpActor			=	_lpActor;

//   	for (int i=0;i<m_wItemCount;i++)
//   		if	(checkExpiredItem(i))
// 			break;
}

//	만들어 놓고 안쓴다. 이유가 있으니 쓰지 말자!
//	자세한건 224한테 문의
BOOL
CShoppingCart::checkExpiredItem(int _iCartSlot)
{
	if  (m_aItemList[_iCartSlot].dwItemNo	==	0xffffffff	)
		return	FALSE;

	cItem		item;
	cBasicItem	*lpBasicItem;
	UTime		endDate;
	int			iPremiumItemIndex	=	m_aItemList[_iCartSlot].dwItemNo;

	endDate.m_dwValue	=	m_aItemList[_iCartSlot].timeEndDate.m_dwValue;	//	사용 만료시간

	if	(!g_im.generatePremiumItem(&item,iPremiumItemIndex))
		return	FALSE;

	lpBasicItem			=	item.getBasicItem();

	if	(lpBasicItem->m_wIsIDPublicItem)
	{
		if	(m_aItemList[_iCartSlot].timeEndDate.m_dwValue)
		{
			item.m_year		=	m_aItemList[_iCartSlot].timeEndDate.year;
			item.m_month	=	m_aItemList[_iCartSlot].timeEndDate.month;
			item.m_day		=	m_aItemList[_iCartSlot].timeEndDate.day;
			item.m_hour		=	m_aItemList[_iCartSlot].timeEndDate.hour;
			item.m_minute	=	m_aItemList[_iCartSlot].timeEndDate.minute;
		}
	}

//	시간 제한이 있는 아이템이다. 뱃지나 오브등등..
	if	(endDate.m_dwValue == 0 && item.m_year)
	{
		endDate.year	=	item.m_year;
		endDate.month	=	item.m_month;
		endDate.day		=	item.m_day;
		endDate.hour	=	item.m_hour;
		endDate.minute	=	item.m_minute;
		endDate.second	=	0;
	}

	if	(!item.isExpiredItem())
		return	FALSE;

	{
		GDMSG_USEBUY	packet;
		packet.base.set(sizeof(GDMSG_USEBUY),dGDMSG_USEBUY);

		strcpy(packet.strId,m_lpActor->m_strId);
		strcpy(packet.strName,m_lpActor->m_strName);
		strcpy(packet.strServerName,g_strGameServerName);
		packet.iLev = m_lpActor->m_iLevel;
		packet.wJob = m_lpActor->m_wJob;

		packet.dwUseSqnList			=	m_aItemList[_iCartSlot].dwSqn;
		packet.wInventorySlot		=	0;
		packet.wCartSlot			=	_iCartSlot;
		packet.dwSerialInServer		=	m_lpActor->m_iZoneSerial;
		packet.timeEndDate.m_dwValue=	endDate.m_dwValue;
		packet.wType				=	ePIUT_REMOVE;

		g_socketDBC.SendPacket((char *)&packet,packet.base.wSize);
	}

	m_lpActor->m_wIsWaitCartMessage		=	TRUE;

	return	TRUE;
}

//
//	체크 섬
DWORD
CShoppingCart::getCheckSum()
{
	int		i;
	BYTE	*lpCartBuffer	=	(BYTE *)m_aItemList;
	int		iCartBufferSize	=	m_wItemCount*sizeof(tsBuyLst);

	m_dwCheckSum			=	0;

	for (i=0;i<iCartBufferSize;i++)
	{
		if (i%2)
			m_dwCheckSum	+=	lpCartBuffer[i];
		else
			m_dwCheckSum	-=	lpCartBuffer[i];
	}

	return	m_dwCheckSum;
}

//
//	아이템을 카트에서 찾아옴 
BOOL
CShoppingCart::askWithdrawItemFromTheCart(int _iInventorySlot,int _iCartSlot,DWORD _dwChecksum)
{	// 마지막 수정일 : 09.10.06
	if	(checkIsIncorrectData(_dwChecksum) || _iCartSlot >= m_wItemCount || _iCartSlot <  0)
		return	eCIM_FAILED;

	if  (m_aItemList[_iCartSlot].dwItemNo	==	0xffffffff)
		return	eCIM_FAILED;

	if	(_iInventorySlot < 0 || _iInventorySlot >= m_lpActor->getItemSlotCount())
	{
		m_lpActor->sendCartMessage(eCM_CAN_NOT_LOAD_THAT_PLACE);

		return	eCIM_FAILED;
	}

	if	(m_lpActor->m_wIsWaitCartMessage)
	{
		m_lpActor->sendCartMessage(eCM_WAIT_FOR_PREVIOUS_WORK_RESULT);

		return	eCIM_SUCCESS;
	}

	cItem		*lpInventorySlot	=	&m_lpActor->m_aItems[_iInventorySlot];
	cItem		item;
	UTime		endDate;
	int			iPremiumItemIndex;

	endDate.m_dwValue	=	m_aItemList[_iCartSlot].timeEndDate.m_dwValue;	//	사용 만료시간
	iPremiumItemIndex	=	m_aItemList[_iCartSlot].dwItemNo;

	if	(!g_im.generatePremiumItem(&item,m_aItemList[_iCartSlot].dwItemNo))
	{
		m_lpActor->sendCartMessage(eCM_FAILED_BY_VALID_ITEM_INFO);

		return	eCIM_FAILED;
	}

	int			iExpandInventorySize=	item.getExpandInventorySize();
	cBasicItem	*lpBasicItem		=	item.getBasicItem();
	
	if(lpBasicItem->isCosmicPower())
	{	// 찾으려고 하는 아이템이 코스믹류 아이템이라면...		09.10.06
		if	(m_lpActor->getInventoryCosmicPowerUpgradeChanceMax())
		{	// 인벤토리에 코스믹아이템 강화 찬스를 체크하여 찬스가 없으면 .. 코스믹아이템이 없는거다...+_+; 왜냐면 코스믹아이템에 확률이 붙어있다는 가정하에.. 체크하는거기때문에!+_+;
			m_lpActor->sendCartMessage(eCM_ONLY_ONE_ITEM_IN_INVENTORY);
			return eCIM_FAILED;	 	
		}
	}

	if (m_lpActor->m_bf6ExtraInventorySize	+	iExpandInventorySize	>	dEXTRA_INVENTORY_SIZE)
		return	eCIM_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL;

	if	(iExpandInventorySize	==	0	&&	lpInventorySlot->m_wBaseItem	!=	0xffff)
	{
		m_lpActor->sendCartMessage(eCM_CAN_NOT_LOAD_THAT_PLACE);

		return	eCIM_SUCCESS;
	}
	
	if (item.isMergeEmblemItem())
	{
		cMagicCarpetEmblemDefine	*lpEmblem	=	(cMagicCarpetEmblemDefine	*)m_lpActor->getMagicCarpetEmblem();

		if (lpEmblem)
		{
			BOOL	bIsAlreadyExist	=	FALSE;

			switch(lpBasicItem->m_aGenerateData[0].m_wEffect)
			{
				case	eIE_REGEN_HP_ON_MAGIC_CARPET				:
					if (lpEmblem->m_bf1IsTurtleCarpet)
						bIsAlreadyExist	=	TRUE;
					break;
				case	eIE_NOT_DOWN_CP_ON_MAGIC_CARPET				:
					if (lpEmblem->m_bf1IsSquirrelCarpet)
						bIsAlreadyExist	=	TRUE;
					break;
				case	eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET		:
					if (lpEmblem->m_bf1IsDrakeCarpet)
						bIsAlreadyExist	=	TRUE;
					break;
				case	eIE_INCREASE_MOVE_SPEED_ON_MAGIC_CARPET_RED	:
					if (lpEmblem->m_bf1IsRedDrakeCarpet)
						bIsAlreadyExist	=	TRUE;
					break;
				case	eIE_FIGHT_MAGIC_CARPET						:
					if (lpEmblem->m_bf1IsActiveGearCarpet)
						bIsAlreadyExist	=	TRUE;
					break;
			}

			if (bIsAlreadyExist)
			{
				m_lpActor->sendCartMessage(eCM_ALREADY_EXIST_SAME_EFFECT_IN_EMBLEM);

				return	eCIM_FAILED;
			}
		}
	}

	if (lpBasicItem)
	{
		if (lpBasicItem->m_attr.isCanNotOwnSameItem)
			if (m_lpActor->getItemByBasicItem(item.m_wBaseItem))
				return	eCIM_CAN_NOT_OWN_SAME_ITEM;

		if (lpBasicItem->m_attr.isBadge)
			if (m_lpActor->getBadgeCount()	>=	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY)
				return	eCIM_TOO_MANY_BADGE_ITEM;
	}

//	프리미엄 오브를 꺼내려 하는데 이미 프리미엄 오브를 가지고 있다.
	if	(lpBasicItem->m_wPremiumLevel)
	{
		if	(m_lpActor->m_wPremiumServiceGrade > lpBasicItem->m_wPremiumItemLevelForWithdraw)
		{
			m_lpActor->sendCartMessage(eCM_ALREADY_EXIST_MORE_HIGH_LEVEL_ORB);

			return	eCIM_SUCCESS;
		}

		if (m_lpActor->getItemByBasicItem(item.m_wBaseItem))
			return	eCIM_CAN_NOT_OWN_SAME_ITEM;
	}

//	시간 제한이 있는 아이템이다. 뱃지나 오브등등..
	if	(endDate.m_dwValue == 0 && item.m_year)
	{
		endDate.year	=	item.m_year;
		endDate.month	=	item.m_month;
		endDate.day		=	item.m_day;
		endDate.hour	=	item.m_hour;
		endDate.minute	=	item.m_minute;
		endDate.second	=	0;
	}

	{
		GDMSG_USEBUY	packet;
		packet.base.set(sizeof(GDMSG_USEBUY),dGDMSG_USEBUY);

		strcpy(packet.strId,m_lpActor->m_strId);
		strcpy(packet.strName,m_lpActor->m_strName);
		strcpy(packet.strServerName,g_strGameServerName);
		packet.iLev = m_lpActor->m_iLevel;
		packet.wJob = m_lpActor->m_wJob;

		packet.dwUseSqnList			=	m_aItemList[_iCartSlot].dwSqn;
		packet.wInventorySlot		=	_iInventorySlot;
		packet.wCartSlot			=	_iCartSlot;
		packet.dwSerialInServer		=	m_lpActor->m_iZoneSerial;
		packet.timeEndDate.m_dwValue=	endDate.m_dwValue;
		packet.wType				=	ePIUT_WITHDRAW;

// 		memcpy(&m_lpActor->m_itemFailedPremiumItem,&item,sizeof(cItem));	//	혹시 모를 사태를 대비해 백업.. 했었으나 아이템 복제의 가능성이 있어 바꾸다!
		g_socketDBC.SendPacket((char *)&packet,packet.base.wSize);
	}

	m_lpActor->m_wIsWaitCartMessage		=	TRUE;

	return	eCIM_SUCCESS;
}

//
//	아이템을 카트에서 찾아옴 
BOOL
CShoppingCart::withdrawItemFromTheCart(int _iInventorySlot,int _iCartSlot)
{
	m_lpActor->m_wIsWaitCartMessage	=	FALSE;

	if	(_iCartSlot >= m_wItemCount || _iCartSlot <  0		)
	{
		printf("withdrawItemFromTheCart - _iCartSlot >= m_wItemCount || _iCartSlot < 0\n");
		CLOG("cart","in CShoppingCart::withdrawItemFromTheCart - _iCartSlot >= m_wItemCount || _iCartSlot < 0 [%d,%d]",_iInventorySlot,_iCartSlot);
		m_lpActor->sendCartMessage(eCM_CART_CLOSE_BY_INCORRECT_INFO);

		return	FALSE;
	}

	if  (m_aItemList[_iCartSlot].dwItemNo	==	0xffffffff	)
	{
		printf("withdrawItemFromTheCart - in CShoppingCart::withdrawItemFromTheCart - m_aItemList[_iCartSlot].dwItemNo == 0xffffffff\n");
		CLOG("cart","in CShoppingCart::withdrawItemFromTheCart - m_aItemList[_iCartSlot].dwItemNo == 0xffffffff");
		m_lpActor->sendCartMessage(eCM_CART_CLOSE_BY_INCORRECT_INFO);

		return	FALSE;
	}

//	로그 서버에 로그 전송
	{
		LOG_ITEM_WITHDRAW_PREMIUM_ITEM	logPacket;

		logPacket.base.set(sizeof(logPacket),dLOG_ITEM_WITHDRAW_PREMIUM_ITEM);

		logPacket.logTime.yy		=	g_currentTime.m_wYear%100;
		logPacket.logTime.mm		=	g_currentTime.m_wMonth;
		logPacket.logTime.dd		=	g_currentTime.m_wDay;
		logPacket.logTime.hour		=	g_currentTime.m_wHour;
		logPacket.logTime.min		=	g_currentTime.m_wMinute;

		strcpy(logPacket.strId,m_lpActor->m_strId);
		strcpy(logPacket.strName,m_lpActor->m_strName);

		CClient	*lpClient	=	m_lpActor->getClient();

		if (lpClient)
			memcpy(logPacket.strIP,lpClient->ip,dIP_LENGTH);

		logPacket.dwItemNo			=	m_aItemList[_iCartSlot].dwItemNo;
		logPacket.dwSqnNumber		=	m_aItemList[_iCartSlot].dwSqn;
		logPacket.wFieldUniqueIndex	=	m_lpActor->m_lpField->m_iUniqueSerial;

		g_LogQue.addPacket((ALL_LOGMSG *)&logPacket);	
	}

	cItem		item;
	cItem		*lpInventorySlot;
	cBasicItem	*lpBasicItem;
	int			iPremiumItemIndex	=	m_aItemList[_iCartSlot].dwItemNo;

	if	(!g_im.generatePremiumItem(&item,iPremiumItemIndex))
	{
		CLOG("error","in CShoppingCart::withdrawItemFromTheCart - !g_im.generatePremiumItem");

		return	FALSE;
	}

	lpInventorySlot		=	&m_lpActor->m_aItems[_iInventorySlot];
	lpBasicItem			=	item.getBasicItem();

	if	(lpBasicItem->m_wIsIDPublicItem)
	{
		if	(m_aItemList[_iCartSlot].timeEndDate.m_dwValue)
		{
			item.m_year		=	m_aItemList[_iCartSlot].timeEndDate.year;
			item.m_month	=	m_aItemList[_iCartSlot].timeEndDate.month;
			item.m_day		=	m_aItemList[_iCartSlot].timeEndDate.day;
			item.m_hour		=	m_aItemList[_iCartSlot].timeEndDate.hour;
			item.m_minute	=	m_aItemList[_iCartSlot].timeEndDate.minute;
		}
	}
	else
	{
		m_aItemList[_iCartSlot].dwItemNo	=	0xffffffff;	

		if	(lpBasicItem->m_attr.isExtraItem)
			item.updateExtraItemEfficency();
	}

//	로그 서버에 프리미엄 아이템 생성 로그 전송
	int	iExpandInventorySize	=	item.getExpandInventorySize();

	if	(iExpandInventorySize)
	{
		m_lpActor->m_bf6ExtraInventorySize	+=	iExpandInventorySize;

		SG_WITHDRAW_ITEM_FROM_THE_CART	packet;

		packet.base.set(sizeof(packet),dSG_WITHDRAW_ITEM_FROM_THE_CART);

		packet.wCartSlot		=	_iCartSlot;
		packet.wInventorySlot	=	_iInventorySlot;

		memcpy(&packet.item,&item,sizeof(cItem));

		g_userPM.add(m_lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}
	else
	{
		if	(item.isExpiredItem())
		{
			m_lpActor->m_itemFailedPremiumItem.reset();
			m_lpActor->sendCartMessage(eCM_FAILED_BY_EXPIRED_ITEM);

			return	FALSE;
		}

		if	(lpInventorySlot->m_wBaseItem	!=	0xffff)
		{
			m_lpActor->sendCartMessage(eCM_FAILED_BY_INVENTORY_FULL);

			return	FALSE;
		}

		memcpy(lpInventorySlot,&item,sizeof(cItem));
		m_lpActor->sendWithdrawItemFromCart(_iInventorySlot,_iCartSlot);
	}

	m_lpActor->sendCreatePremiumItemLog(&item,eCPIM_WITHDRAW_CART_ITEM);
	m_lpActor->m_itemFailedPremiumItem.reset();

	m_lpActor->buildPower();
	m_lpActor->immediatelySendSaveDataToDBMemory();

	return	TRUE;
}

//
//	카트에 아이템 제거 요청
BOOL
CShoppingCart::askDestroyCartItem(int _iCartSlot,DWORD _dwChecksum)
{
	if	(checkIsIncorrectData(_dwChecksum) || _iCartSlot >= m_wItemCount || _iCartSlot <  0)	return	FALSE;
	if  (m_aItemList[_iCartSlot].dwItemNo	==	0xffffffff)	return	FALSE;
	if	(m_lpActor->m_wIsWaitCartMessage)
	{
		m_lpActor->sendCartMessage(eCM_WAIT_FOR_PREVIOUS_WORK_RESULT);

		return	TRUE;
	}

	cItem	item;

	if (!g_im.generatePremiumItem(&item,m_aItemList[_iCartSlot].dwItemNo))
	{
		m_lpActor->sendCartMessage(eCM_FAILED_BY_VALID_ITEM_INFO);

		return	FALSE;
	}

	cBasicItem	*lpBasicItem	=	item.getBasicItem();

	if(!lpBasicItem)
	{
		m_lpActor->sendCartMessage(eCM_FAILED_BY_VALID_ITEM_INFO);

		return	FALSE;
	}

	if (m_aItemList[_iCartSlot].timeEndDate.m_dwValue == 0)
	{
		m_lpActor->sendDestroyCartItemResult(eDCIR_CAN_NOT_DESTROY_NOT_USED_ITEM,_iCartSlot);

		return TRUE;
	}

	{
		GDMSG_USEBUY	packet;
		packet.base.set(sizeof(GDMSG_USEBUY),dGDMSG_USEBUY);

		strcpy(packet.strId,m_lpActor->m_strId);
		strcpy(packet.strName,m_lpActor->m_strName);
		strcpy(packet.strServerName,g_strGameServerName);
		packet.iLev = m_lpActor->m_iLevel;
		packet.wJob = m_lpActor->m_wJob;

		packet.dwUseSqnList			=	m_aItemList[_iCartSlot].dwSqn;
		packet.wCartSlot			=	_iCartSlot;
		packet.dwSerialInServer		=	m_lpActor->m_iZoneSerial;
		packet.wType				=	ePIUT_REMOVE;

		g_socketDBC.SendPacket((char *)&packet,packet.base.wSize);
	}

	m_lpActor->m_wIsWaitCartMessage		=	TRUE;

	return	TRUE;
}

//
//	카트에 아이템 제거
BOOL
CShoppingCart::destroyCartItem(int _iCartSlot)
{
	m_lpActor->m_wIsWaitCartMessage		=	FALSE;

	if	(_iCartSlot >= m_wItemCount || _iCartSlot <  0		)
	{
		printf("destroyCartItem - _iCartSlot >= m_wItemCount || _iCartSlot < 0\n");
		CLOG("cart","in CShoppingCart::destroyCartItem - _iCartSlot >= m_wItemCount || _iCartSlot < 0 [%d,%d]",_iCartSlot);
		m_lpActor->sendCartMessage(eCM_CART_CLOSE_BY_INCORRECT_INFO);

		return	FALSE;
	}

	if  (m_aItemList[_iCartSlot].dwItemNo	==	0xffffffff	)
	{
		printf("destroyCartItem - m_aItemList[_iCartSlot].dwItemNo == 0xffffffff\n");
		CLOG("error","in CShoppingCart::destroyCartItem - m_aItemList[_iCartSlot].dwItemNo == 0xffffffff");
		m_lpActor->sendCartMessage(eCM_CART_CLOSE_BY_INCORRECT_INFO);

		return	FALSE;
	}

	m_aItemList[_iCartSlot].dwItemNo	=	0xffffffff;	
	m_lpActor->sendDestroyCartItemResult(eDCIR_SUCCESS,_iCartSlot);

	return	TRUE;
}


//
//
CShoppingCartManager::CShoppingCartManager()
{
	m_pCart	=	NULL;
}

CShoppingCartManager::~CShoppingCartManager()
{
	if (m_pCart)
	{
		pKILL(m_pCart);

		DeleteCriticalSection(&m_cs);
	}
}

//
//	초기화
void
CShoppingCartManager::init(int _iCartCount)
{
	m_iCartSize				=	_iCartCount;
	m_iCartCount			=	0;

	m_pCart					=	new CShoppingCart [m_iCartSize];

	FPInitCS(&m_cs);
}

//
//	연다.
int
CShoppingCartManager::open(tsBuyLst *_lpCartInfo,int _iItemCount,cACTOR *_lpActor)
{
	mCS(m_cs);

	int	iFreeCartIndex	=	getFreeCartIndex();

	if (iFreeCartIndex	==	0xffff)
		return	0xffff;

	CShoppingCart	*lpCart	=	&m_pCart[iFreeCartIndex];

	lpCart->open(_lpCartInfo,_iItemCount,_lpActor);
	lpCart->m_wSerial	=	iFreeCartIndex;

	return	iFreeCartIndex;
}

//
//	빈 카트 찾기
int
CShoppingCartManager::getFreeCartIndex()
{
	for (int i=0;i<m_iCartSize;i++)
		if (m_pCart[i].m_wSerial	==	0xffff)	return i;

	return	0xffff;
}

//
//	카트 닫기
void
CShoppingCartManager::closeCart(int _iCartIndex,BOOL _bIsForced)
{
	CShoppingCart	*lpCart	=	&m_pCart[_iCartIndex];

	if (lpCart->m_wSerial	==	0xffff)	return;

	mCS(m_cs);

	lpCart->reset();
	m_iCartCount--;
}