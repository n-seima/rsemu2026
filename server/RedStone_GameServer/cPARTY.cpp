#include "cParty.H"
#include "cGAME.H"
#include "CServer.H"
#include "debugCode.H"
#include "secretDungeon.H"
#include "packetManager.H"
#include "GameDLL.H"

cPartyManager*			g_pPartyManager;

cPartyManager::cPartyManager()
{
	FPInitCS(&m_csPartyWork);
}

cPartyManager::~cPartyManager()
{
	close();
}

void
cPartyManager::operateOncePerSecond()
{
	int	iCount	=	m_den.getMaxCount();

	for(int i=0;i<iCount;i++)
	{
		cParty	*lpParty		=	m_den.get(i);

		if	(!lpParty	|| lpParty->m_wSerial == 0xffff)
			continue;

		lpParty->operateOncePerSecond();
	}
}

BOOL
cPartyManager::isNotJJUL(cACTOR *_lpChecker)
{
	if	(_lpChecker->getPartySerial()	!=	0xffff)
	{
		cParty	party;

		if	(g_pPartyManager->getParty(&party,_lpChecker->getPartySerial()))
			return	party.isNotJJUL(_lpChecker);
	}

	return	TRUE;
}

BOOL
cPartyManager::isGoodParty(cACTOR *_lpTarget,cACTOR *_lpChecker)
{
	int	iLevelRange	=	30;

	if	(_lpChecker->getPartySerial()	!=	0xffff)
	{
		cParty	party;

		if	(g_pPartyManager->getParty(&party,_lpChecker->getPartySerial()))
			return	party.isGoodParty(iLevelRange,_lpTarget,_lpChecker);
	}
	else
	if	(_lpChecker->m_iLevel	<=	_lpTarget->m_iLevel+iLevelRange)
		return	TRUE;

	return	FALSE;
}

//
//	닫기
void
cPartyManager::close()
{
	m_den.close();
	DeleteCriticalSection(&m_csPartyWork);
}


void
cPartyManager::init(int _iCount)
{
	m_den.init(_iCount);
}

int
cPartyManager::checkDisconnectPlayer(cACTOR *_lpActor,BOOL _bIsWantPartySerial)
{
	mCS(m_csPartyWork);

	DWORD				dwHashCode	=	GetHashCode(_lpActor->m_strName);
	cParty				*lpParty	=	NULL;
	cPARTY_MEMBER		*lpMember	=	NULL;

	for (int i=0,iCount=0;i<m_den.getMaxCount() && iCount < m_den.getCount();i++)
	{
		lpParty	=	m_den.get(i);

		if (!lpParty)
			continue;

		lpMember	=	lpParty->getMemberInfoByName(_lpActor->m_strName,dwHashCode);

		if (lpMember)
			break;

		lpParty		=	NULL;
		iCount++;
	}

	if (lpParty	==	NULL)
		return	0xffff;

	if (_bIsWantPartySerial || lpMember->m_bf1IsDisconnected == FALSE)
		return	lpParty->m_wSerial;

	g_game.sendWPJoinParty(lpParty,_lpActor,TRUE);

	return	lpParty->m_wSerial;
}

//
//	파티 만들기
BOOL
cPartyManager::makeParty(int _iPartyIndex,DWORD _dwUniqueSerial,char *_lpstrPartyName)
{
	cParty	party;

	party.init(_iPartyIndex,_dwUniqueSerial,_lpstrPartyName);

	if (!m_den.insert(_iPartyIndex,&party))
	{
		m_den.remove(_iPartyIndex);
		m_den.insert(_iPartyIndex,&party);

		CLOG("party","음.. 파티 생성 실패 했었었어.. -o-");
	}

	return	TRUE;
}
//
//	파티정보 업데이트
void
cPartyManager::updatePartyInfo(WORLDtoSERVERS_UPDATE_PARTYINFO* _lpPacket)
{
	int					i;
	mCS(m_csPartyWork);
	CSimplePartyInfo	*lpPartyInfo		=	&_lpPacket->partyInfo;
	cParty				*lpParty			=	NULL;
	BOOL				bIsCreateParty		=	_lpPacket->wIsCreate;

	if (bIsCreateParty)
	{
		cParty	*lpParty	=	get(lpPartyInfo->m_wSerial);

		if (lpParty)
		{
			CLOG("party","파티를 생성하려는데 이미 파티가 있음. '%s'",lpPartyInfo->m_strName);

			if (STRCMP(lpParty->m_strPartyName,lpPartyInfo->m_strName) != 0)
			{
				dissolutionParty(lpPartyInfo->m_wSerial);
				makeParty(lpPartyInfo->m_wSerial,lpPartyInfo->m_dwUniqueSerial,lpPartyInfo->m_strName);	//	생성되는 거냐?
			}
		}
		else
			makeParty(lpPartyInfo->m_wSerial,lpPartyInfo->m_dwUniqueSerial,lpPartyInfo->m_strName);	//	생성되는 거냐?
	}

	lpParty				=	get(lpPartyInfo->m_wSerial,_lpPacket->partyInfo.m_strName);

	if (!lpParty)
	{
		CLOG("party","updatePartyInfo 파티가 없어요. '%s:%d'",lpPartyInfo->m_strName,lpPartyInfo->m_wSerial);

		makeParty(lpPartyInfo->m_wSerial,lpPartyInfo->m_dwUniqueSerial,lpPartyInfo->m_strName);	//	다시 생성

		lpParty	=	get(lpPartyInfo->m_wSerial,lpPartyInfo->m_strName);
	}

	if (bIsCreateParty)
		lpParty->m_wLeaderPlayMinuteOnThisFieldTime	=	0;

	lpParty->m_wGateField		=	_lpPacket->partyInfo.m_wGateField;
	lpParty->m_bf10GateArea		=	_lpPacket->partyInfo.m_bf10GateArea;

	lpParty->m_bf3GoldShareMethod=	_lpPacket->partyInfo.m_bf3GoldShareMethod;
	lpParty->m_bf3ItemShareMethod=	_lpPacket->partyInfo.m_bf3ItemShareMethod;
	lpParty->m_bf3PartyObject	=	_lpPacket->partyInfo.m_bf3PartyObject	;	//파티 목적
	lpParty->m_bf1IsOpenParty	=	_lpPacket->partyInfo.m_bf1IsOpenParty	;	//멤버를 계속 모집한다.

	for (i=0;i<(int)lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

		if (lpActor	==	NULL)
			continue;

		lpActor->leaveParty();
	}

	lpParty->m_wMemberCount		=	0;

	int iReactionLimitCount	=	0;
	for (i=0;i<(int)lpPartyInfo->m_bf4MemberCount;i++)
	{
		lpParty->joinPartyMember(&lpPartyInfo->m_aMembers[i]);

	}

	for (i=0;i<(int)lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	g_game.getPlayer(lpParty->m_aMember[i].m_strName);

		if (!lpActor)
			continue;

		if (lpActor->m_wSerialInField	==	0xffff)
		{
			CLOG("party","cPartyManager::updatePartyInfo - lpActor->m_wSerialInField == 0xffff");
			continue;
		}
		
		lpActor->joinParty(lpParty->m_wSerial);
		lpActor->m_lpField->sendPartyInfo(lpActor,max(bIsCreateParty-i,0));
		iReactionLimitCount += lpActor->getReactionLimitCount();

		if(iReactionLimitCount > dPLAYER_ACTIVE_REACTION_LIMIT)
		{
			for(int j=0;j<dACTIVE_REACTION_COUNT;++j)
			{
				
				if(!lpActor->isReactionLimit(lpActor->m_aActiveReaction[j]))
					continue;
				CG_SET_REACTION_STATUS	packet;
				
				packet.wAbility	=	lpActor->m_aActiveReaction[j];
				packet.wIsOn	=	FALSE;
				lpActor->m_lpField->setReactionStatus(&packet, lpActor->m_wSerialInField);
				--iReactionLimitCount;
				--j;
				if(iReactionLimitCount <= dPLAYER_ACTIVE_REACTION_LIMIT)
					break;
			}

		}

	}
	
	if (bIsCreateParty)
		for (i=0;i<(int)lpParty->m_wMemberCount;i++)
		{
			cPARTY_MEMBER	*lpMember	=	&lpParty->m_aMember[i];

			changeMemberInfo(lpParty->m_wSerial,lpParty->m_dwUniqueSerial,lpMember->m_strName,lpMember->m_bf10Level,lpMember->m_bf4PlaySecretDungeonCount,lpMember->m_bf8Job,lpMember->m_bf3PlayGuildDungeonCount);
		}
}

//
//	멤버 레벨업
BOOL
cPartyManager::sendWPChangeMemberInfo(int _iPartyIndex,char *_lpstrName,int _iLevel,int _iPSC,int _iJob,int _iGDPC)
{
	mCS(m_csPartyWork);

	cParty	*lpParty	=	get(_iPartyIndex);

	if	(!lpParty	|| lpParty->m_wSerial == 0xffff)
	{
		CLOG("party","sendWPChangeMemberInfo 파티가 없어요. -o- '%d'",_iPartyIndex);

		return	FALSE;
	}

	cPARTY_MEMBER	*lpMember	=	lpParty->getMemberInfoByName(_lpstrName);

	if (!lpMember)
	{
		CLOG("party","sendWPChangeMemberInfo 파티 멤버가 없어요. -o- '%s:%s'",lpParty->m_strPartyName,_lpstrName);

		return	FALSE;
	}

	g_game.sendWPChangePartyMemberInfo(lpParty,_lpstrName,_iLevel,_iJob,_iPSC , _iGDPC);

	return	TRUE;
}

//
//	멤버 레벨 업데이트
void
cPartyManager::changeMemberInfo(int _iPartyIndex,DWORD _dwUniqueSerial,char *_lpstrMemberName,int _iLevel,int _iPSC,int _iJob , int _iGDPC)
{
	mCS(m_csPartyWork);

	cParty	*lpParty	=	get(_iPartyIndex);

	if	(!lpParty || lpParty->m_dwUniqueSerial	!=	_dwUniqueSerial)
	{
		g_game.sendWPAskPartyInfo(_iPartyIndex);
		return;
	}

	cPARTY_MEMBER	*lpMember	=	lpParty->getMemberInfoByName(_lpstrMemberName);

	if	(!lpMember)
	{
		g_game.sendWPAskPartyInfo(_iPartyIndex);
		return;
	}

	lpMember->m_bf10Level					=	_iLevel;
	lpMember->m_bf4PlaySecretDungeonCount	=	_iPSC;
	lpMember->m_bf8Job						=	_iJob;
	lpMember->m_bf3PlayGuildDungeonCount	=	_iGDPC;

	lpParty->syncMemberInfo(_lpstrMemberName);
}

void
cPartyManager::moveFieldMember(int _iPartyIndex,char *_lpstrPartyName,char *_lpstrName,int _iDestField,BOOL _bIsLeave,int _iZoneSerial,int _iIFSerial,int _iIFFloor)
{
	mCS(m_csPartyWork);

	cParty	*lpParty	=	get(_iPartyIndex,_lpstrPartyName);

	if (!lpParty)
	{
		CLOG("party","moveFieldMember 파티가 없어요. -o- '%s'",_lpstrPartyName);

		g_game.sendWPAskPartyInfo(_iPartyIndex,_lpstrPartyName);
		return;
	}

	cPARTY_MEMBER	*lpMember	=	lpParty->getMemberInfoByName(_lpstrName);

	if (!lpMember)
	{
		CLOG("party","moveFieldMember 멤버가가 없어요. -o- '%s:%s'",_lpstrPartyName,_lpstrName);

		g_game.sendWPAskPartyInfo(_iPartyIndex,_lpstrPartyName);
		return;
	}

	lpMember->m_bf1IsMoveField		=	_bIsLeave;
	lpMember->m_bf10InstanceField	=	_iIFSerial;
	lpMember->m_bf4InstanceFieldFloor=	_iIFFloor;

	if (_bIsLeave	==	FALSE)
	{
		lpMember->m_wFieldSerial	=	_iDestField;
		lpMember->m_iZoneSerial		=	_iZoneSerial;
	}

	lpParty->syncMemberInfo(_lpstrName);
}

void
cPartyManager::disconnectField(int *_lpiFieldList,int _iFieldCount)
{
	mCS(m_csPartyWork);

	for (int i=0;i<dPARTY_COUNT;i++)
	{
		cParty	*lpParty	=	m_den.get(i);

		if (!lpParty)
			continue;

		lpParty->disconnectField(_lpiFieldList,_iFieldCount);

		if((int)lpParty->m_wMemberCount	<=	1)
			dissolutionParty(i);
	}
}

//
//	파티 해체
void
cPartyManager::dissolutionParty(int _iPartySerial)
{
	mCS(m_csPartyWork);

	cParty	*lpParty	=	get(_iPartySerial);

	if	(!lpParty|| lpParty->m_wSerial == 0xffff)
		return;

	int		aReferenceField[dPARTY_MEMBER_COUNT],iReferenceFieldCount=0;
	BOOL	bIsReferencedField;

	memset(aReferenceField,0xff,sizeof(aReferenceField));

	for (int i=0;i<(int)lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

		if (!lpActor			)
			continue;

		lpActor->leaveParty();
		lpActor->sendDissolutionPartyMessage();

		bIsReferencedField	=	FALSE;

		for (int j=0;j<iReferenceFieldCount;j++)
		{
			if (aReferenceField[j]	==	lpActor->m_wCurrentField)
			{
				bIsReferencedField	=	TRUE;
				break;
			}
		}

		if (bIsReferencedField)
			continue;

		aReferenceField[iReferenceFieldCount++]	=	lpActor->m_wCurrentField;
		lpActor->m_lpField->operateBreakParty(lpParty->m_wSerial);
	}

	m_den.remove(lpParty->m_wSerial);
}

//
//	파티 해체
void
cPartyManager::dissolutionParty(char *_lpstrName)
{
	mCS(m_csPartyWork);

	cParty	*lpParty	=	get(_lpstrName);

	if	(!lpParty|| lpParty->m_wSerial == 0xffff)
		return;

	int	iSerial	=	lpParty->m_wSerial;

	dissolutionParty(iSerial);
}

//
//	멤버 추가
BOOL
cPartyManager::joinPartyMember(int _iPartyIndex,char *_lpstrPartyName,CSimplePartyMemberInfo *_lpMember)
{
	mCS(m_csPartyWork);

	cParty* lpParty	=	get(_iPartyIndex,_lpstrPartyName);

	if (!lpParty|| lpParty->m_wSerial == 0xffff)
	{
		CLOG("party","joinParty 파티가 없어요. -o- '%s'",_lpstrPartyName);

		return	FALSE;
	}

	return	lpParty->joinPartyMember(_lpMember);
}

BOOL
cPartyManager::disconnectMember(int _iPartyIndex,char *_lpstrPartyName,char *_strName)
{
	mCS(m_csPartyWork);

	cParty	*lpParty=	get(_iPartyIndex,_lpstrPartyName);

	if	(lpParty	==	NULL)	//	파티가 없다. 이경우 어차피 월드에서 다시 파티 정보가 날라오니 일단 로그만 남기자.
	{
		CLOG("party","leave party 파티가 없어요. -o- '%s'",_lpstrPartyName);
		return	FALSE;
	}

	if (!lpParty->disconnectMember(_strName))
		return	FALSE;

	return	TRUE;
}

//
//멤버 제거
BOOL
cPartyManager::operatePartyWork(int _iPartyIndex,DWORD _dwUniqueSerial,int _iWork,char *_strName)
{
	mCS(m_csPartyWork);

	cParty	*lpParty=	get(_iPartyIndex);

	if	(lpParty	==	NULL	||	lpParty->m_dwUniqueSerial	!=	_dwUniqueSerial)	//	파티가 없다. 이경우 어차피 월드에서 다시 파티 정보가 날라오니 일단 로그만 남기자.
		return	FALSE;

	int		iMemberIndex,i;

	iMemberIndex		=	lpParty->getMemberIndex(_strName);

	if	(iMemberIndex	==	0xffff)
		return	FALSE;

	SG_PARTY_WORK	packet;

	packet.base.set(sizeof(packet),dSG_PARTY_WORK);
	packet.wWork		=	_iWork;
	packet.wMemberIndex	=	iMemberIndex;

	for (i=0;i<(int)lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

		if	(lpMember	==	NULL)
			continue;

		g_userPM.add(lpMember->m_iClientSerial,&packet,packet.base.wSize);
	}

	switch(_iWork)
	{
		case	ePW_LEAVE_PARTY		:
		case	ePW_BANISH_MEMBER	:
			lpParty->removeMember(_strName);
			break;

		case	ePW_CHANGE_LEADER	:
			lpParty->changeLeader(_strName);
			break;

		case	ePW_IS_CONNECT_USER	:
		{
			cACTOR	*lpActor	=	g_game.getPlayer(_strName);

			if	(lpActor)
				lpActor->m_bf1IsSendReapirPartyDisconnectedStatusMessage	=	FALSE;

			lpParty->changeMemberConnectStatus(_strName,FALSE);
			break;
		}
	}

	return	TRUE;
}

void
cPartyManager::changePartyStatus(int _iPartyIndex,DWORD _dwUniqueSerial,int _iGoldShareMethod,int _iItemShareMethod,int _iPartyObject,BOOL _bIsOpenParty,int _iLastReceiveGoldMember,int _iLastReceiveItemMember,BOOL _bIsChangeSetting)
{
	mCS(m_csPartyWork);

	cParty	*lpParty	=	get(_iPartyIndex);

	if (!lpParty)
	{
		g_game.sendWPAskPartyInfo(_iPartyIndex);

		return;
	}

	lpParty->m_bf3GoldShareMethod		=	_iGoldShareMethod;
	lpParty->m_bf3ItemShareMethod		=	_iItemShareMethod;
	lpParty->m_bf3PartyObject			=	_iPartyObject;
	lpParty->m_bf1IsOpenParty			=	_bIsOpenParty;

	lpParty->m_bf3LastReceiveGoldMember	=	_iLastReceiveGoldMember;
	lpParty->m_bf3LastReceiveItemMember	=	_iLastReceiveItemMember;

	if (_bIsChangeSetting)
	{
		SG_CHANGE_PARTY_STATUS	packet;

		packet.base.set(sizeof(packet),dSG_CHANGE_PARTY_STATUS);

		packet.bf3GoldShareMethod		=	_iGoldShareMethod;
		packet.bf3ItemShareMethod		=	_iItemShareMethod;
		packet.bf3PartyObject			=	_iPartyObject;	//파티 목적
		packet.bf1IsOpenParty			=	_bIsOpenParty;	//멤버를 계속 모집한다.

		lpParty->sendPacketToAllMember(&packet,packet.base.wSize);
	}
}

void
cPartyManager::changePartyName(int _iPartyIndex,char *_lpstrPartyName,char *_strChangeName)
{
	mCS(m_csPartyWork);

	cParty	*lpParty	=	get(_iPartyIndex,_lpstrPartyName);

	if (!lpParty)
	{
		g_game.sendWPAskPartyInfo(_iPartyIndex,_lpstrPartyName);

		CLOG("party","change party 파티가 없어요. [%s]",_lpstrPartyName);

		return;
	}

	strcpy(lpParty->m_strPartyName,_strChangeName);

	lpParty->m_dwNameHashCode	=	GetHashCode((BYTE *)_strChangeName);

	SG_CHANGE_PARTY_NAME	packet;

	packet.base.set(sizeof(packet),dSG_CHANGE_PARTY_NAME);
	strcpy(packet.strPartyName,_strChangeName);

	for (int i=0;i<(int)lpParty->m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	lpParty->getActorByMemberIndex(i);

		if (!lpActor)
			continue;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}
}

cParty*
cPartyManager::getByUniqueSerial(int _iPartyIndex,DWORD _dwUniqueSerial)
{
	cParty* lpParty		=	m_den.get(_iPartyIndex);

	if (lpParty && lpParty->m_dwUniqueSerial	==	_dwUniqueSerial)
		return	lpParty;

	return	NULL;
}

cParty*
cPartyManager::get(int _iPartyIndex,char *_lpstrName)
{
	cParty* lpParty		=	m_den.get(_iPartyIndex);

	if (lpParty && STRCMP(lpParty->m_strPartyName,_lpstrName)==0)
		return	lpParty;

	DWORD	dwHashCode	=	GetHashCode((BYTE *)_lpstrName);
	int		count		=	m_den.getMaxCount();

	for(int i=0;i<count;i++)
	{
		lpParty		=	m_den.get(i);

		if (!lpParty|| lpParty->m_wSerial == 0xffff)
			continue;

		if (lpParty->m_dwNameHashCode	!=	dwHashCode)
			continue;

		if (STRCMP(lpParty->m_strPartyName,_lpstrName)==0)
		{
			CLOG("party","get 시리얼 검색 실패 이름 검색함 [%s:%d]",_lpstrName,_iPartyIndex);

			return	lpParty;
		}
	}

	return	NULL;
}

cParty*
cPartyManager::get(char *_lpstrName)
{
	int		count		=	m_den.getMaxCount();
	cParty* lpParty		=	NULL;
	DWORD	dwHashCode	=	GetHashCode((BYTE *)_lpstrName);

	for(int i=0;i<count;i++)
	{
		lpParty		=	m_den.get(i);

		if (!lpParty|| lpParty->m_dwNameHashCode	!=	dwHashCode)
			continue;

		if (STRCMP(lpParty->m_strPartyName,_lpstrName)==0)
			return	lpParty;
	}

	return	NULL;
}


BOOL
cPartyManager::getParty(cParty *_lpParty,int _iPartyIndex)
{
	mCS(m_csPartyWork);

	cParty* lpParty	=	get(_iPartyIndex);

	if (!lpParty|| lpParty->m_wSerial == 0xffff)
		return	FALSE;

	memcpy(_lpParty,lpParty,sizeof(cParty));

	return	TRUE;
}

//
//	총파티의 수
int
cPartyManager::getCount()
{
	return m_den.getCount();
}

//
//	멤버의 존 시리얼 설정
void
cPartyManager::setMemberZoneSerial(int _iParty,char *_lpstrName,int _iZoneSerial)
{
	mCS(m_csPartyWork);

	cParty	*lpParty	=	get(_iParty);

	if (lpParty)
	{
		cPARTY_MEMBER	*lpMember	=	lpParty->getMemberInfoByName(_lpstrName);

		if (lpMember)
			lpMember->m_iZoneSerial	=	_iZoneSerial;
	}
}

//
//	아이템을 줏었다.
BOOL
cPartyManager::pickPartyItem(int _iPartyIndex,int _iPickMember,int _iFieldSerial,int _iX,int _iY,cItem *_lpItem)
{
	mCS(m_csPartyWork);

	cACTOR	*lpPicker	=	g_game.getActor(_iPickMember);
	cParty	*lpParty	=	get(_iPartyIndex);
	BOOL	bIsCorrect	=	FALSE;
	int		aMember[dPARTY_MAXPLAYER];
	int		iAvailMemberCount	=	0;
	int		i;

	if (!lpPicker)	
		return	FALSE;
	if (!lpParty|| lpParty->m_wSerial == 0xffff)	
		return	FALSE;

	for (i=0;i<dPARTY_MAXPLAYER;i++)
	{
		cPARTY_MEMBER	*lpMember	=	&lpParty->m_aMember[i];

//		if (lpMember->m_iZoneSerial	==	0xffffffff	)	continue;
		if (lpMember->m_wFieldSerial!=	_iFieldSerial)
			continue;

		cACTOR	*lpActor			=	g_game.getPlayer(lpMember->m_strName);

		if	(!lpActor	||	lpActor->m_wSerialInField	==	0xffff)	continue;

		int	iRange	=	GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,_iX,_iY);

		if	(iRange	>	dMINIMUM_AVAIL_PARTY_RANGE)	continue;

		aMember[iAvailMemberCount++]	=	i;
	}

	if (iAvailMemberCount	<=	0)
		return	FALSE;

	if (_lpItem->m_wBaseItem==	dITEM_MONEY)
	{
		int	iReceiveGold	=	_lpItem->m_dwSerial;

		iReceiveGold		=	max(iReceiveGold,1);

//	파티원이 주운 돈은 액수/파티원으로 정확하게 배분. 에누리는 주운 사람 몫	
//	악 캐릭터가 돈을 주웠을 경우 25%만큼 돈을 독점한 후 나머지에서 파티원들과 배분
//	절대 악 캐릭터가 돈을 주웠을 경우 50%만큼 돈을 독점한 후 나머지에서 파티원들과 배분
		SG_PCIK_PARTY_GOLD	packet;

		packet.base.set(sizeof(SG_PCIK_PARTY_GOLD),dSG_PCIK_PARTY_GOLD);
		packet.iGold		=	iReceiveGold;

		for (i=0;i<iAvailMemberCount;i++)
		{
			cPARTY_MEMBER	*lpMember	=	&lpParty->m_aMember[aMember[i]];
			cACTOR			*lpActor	=	g_game.getPlayer(lpMember->m_strName);

			if	(!lpActor	||	lpActor->m_wSerialInField==	0xffff)
				continue;

			lpActor->m_iGold			+=	iReceiveGold;
			lpActor->updateGoldLog(iReceiveGold,eGET_GOLD_BY_PICK_DROPPED_GOLD);

			g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}
	}
	else	//	줏은게 돈이 아니고 아이템이다.
	{
		int		iReceiver	=	random(iAvailMemberCount);
		cACTOR	*lpReceiver	=	g_game.getPlayer(lpParty->m_aMember[aMember[iReceiver]].m_strName);

		if (!lpReceiver)	
			return	FALSE;

		if	(_lpItem->isMoneyItem())
			CLOG("goldIOError","pickPartyItem");

		if (lpReceiver->addItemToInventory(_lpItem)	!=	eUIR_OK)
			if (lpPicker->addItemToInventory(_lpItem)	!=	eUIR_OK)
				bIsCorrect	=	FALSE;

		if (bIsCorrect)
		{
			SG_PCIK_PARTY_ITEM	packet;

			packet.base.set(sizeof(SG_PCIK_PARTY_ITEM),dSG_PCIK_PARTY_ITEM);
			memcpy(&packet.item,_lpItem,sizeof(cItem));
			packet.wReceiver	=	lpReceiver->m_wSerialInField;

			for (i=0;i<iAvailMemberCount;i++)
			{
				cPARTY_MEMBER	*lpMember	=	&lpParty->m_aMember[aMember[i]];
				cACTOR			*lpActor	=	g_game.getPlayer(lpMember->m_strName);

				if	(!lpActor	||	lpActor->m_wSerialInField==	0xffff)	continue;

				g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
			}
		}
	}

	return	TRUE;
}	//	cPartyManager::pickPartyItem(int _iPartyIndex,cItem *_lpItem)


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class cParty

int
cParty::getInrangeMember(int _iField,cPOINT *_lpPos,WORD *_lpwMemberList,cItem *_lpCheckJobItem)
{
	int	iCount	=	0;

	for (int i=0;i<m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if (!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;

		if	(_iField	!=	0xffff)
		{
			if	(_iField	>=	dINSTANCE_FIELD_FLAG)
			{
				if (lpMember->m_wCurrentField	<	dINSTANCE_FIELD_FLAG)
					continue;

				cFIELD	*lpIF	=	g_pIfManager->getFloor(lpMember->m_bf8InstanceField,lpMember->m_bf4InstanceFieldFloor,m_dwUniqueSerial);

				if (!lpIF)
					continue;

				if (lpMember->m_bf8InstanceField	!=	(DWORD)(_iField/dINSTANCE_FIELD_FLAG))
					continue;
				if (lpMember->m_bf4InstanceFieldFloor!=	(DWORD)(_iField%dINSTANCE_FIELD_FLAG))
					continue;
				if (lpIF->m_wGateField				!=	lpMember->m_wCurrentField-dINSTANCE_FIELD_FLAG)
					continue;
			}
			else
			if	(_iField	!=	lpMember->m_wCurrentField)
				continue;
		}

		if (lpMember->isIdlePlayer())
			continue;
		if (m_aMember[i].m_bf1IsDisconnected)
			continue;

		int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

		if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)
			continue;

		if (_lpCheckJobItem)
		{
			if (!_lpCheckJobItem->isEnableJob(lpMember->m_wJob)	&&
				!_lpCheckJobItem->isEnableJob(lpMember->getAnotherJob())	)
				continue;
		}

		_lpwMemberList[iCount]	=	i;

		iCount++;
	}

	return	iCount;
}

BOOL
cParty::isEnableToTryDuelSatus()
{
	int	iLeaderField	=	m_aMember[0].m_wFieldSerial;

	if	(m_wMemberCount	==	0)
		return	FALSE;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if	(m_aMember[i].m_bf1IsDisconnected)
			return	FALSE;

		if	(!lpMember	||	lpMember->m_wCurrentField	!=	iLeaderField)
			return	FALSE;
	}

	return	TRUE;
}

//
//특정 필드 안에 멤버의 수
int
cParty::getMemberCountForGiveExp(int _iField,cPOINT *_lpPos,int *_lpiBonusExp)
{
	*_lpiBonusExp	=	0;

	int	iCount	=	0;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if	(!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;

		if	(_iField	!=	0xffff)
		{
			if	(_iField	>=	dINSTANCE_FIELD_FLAG)
			{
				if	(lpMember->m_wCurrentField	<	dINSTANCE_FIELD_FLAG)
					continue;

				cFIELD	*lpIF	=	g_pIfManager->getFloor(lpMember->m_bf8InstanceField,lpMember->m_bf4InstanceFieldFloor,m_dwUniqueSerial);

				if	(!lpIF)
					continue;

				if	(lpMember->m_bf8InstanceField		!=	(DWORD)(_iField/dINSTANCE_FIELD_FLAG))
					continue;
				if	(lpMember->m_bf4InstanceFieldFloor	!=	(DWORD)(_iField%dINSTANCE_FIELD_FLAG))
					continue;
				if	(lpIF->m_wGateField					!=	lpMember->m_wCurrentField-dINSTANCE_FIELD_FLAG)
					continue;
			}
			else
			if	(_iField	!=	lpMember->m_wCurrentField)
				continue;
		}

		if	(lpMember->isIdlePlayer())
			continue;
		if	(m_aMember[i].m_bf1IsDisconnected)
			continue;

		if	(_lpPos)
		{
			int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

			if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)
				continue;
		}

		*_lpiBonusExp	=	max(*_lpiBonusExp,lpMember->m_wPartyExpBonus);

		iCount++;
	}

	if	(_iField	==	0xffff)
		return	(int)m_wMemberCount;

	return	iCount;
}

int
cParty::getMemberCountForMovePacket(int _iField)
{
	int	iCount	=	0;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		if (m_aMember[i].m_wFieldSerial	!=	_iField)
			continue;
		if (m_aMember[i].m_bf1IsDisconnected)
			continue;

		iCount++;
	}

	return	iCount;
}


//
//사정거리 안에 멤버들 레벨 합
int
cParty::getInrangeMemberLevelSum(int _iField,cPOINT *_lpPos)
{
	int	iLevelSum	=	0;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if	(!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;
		if	(_iField	!=	0xffff	&&	lpMember->m_wCurrentField	!=	_iField)
			continue;
		if	(lpMember->isIdlePlayer())
			continue;

		if	(_lpPos)
		{
			int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

			if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)
				continue;
		}

		iLevelSum	+=	lpMember->m_iLevel;
	}

	return	iLevelSum;
}

//
//	절대선 성향의 멤버들 수 구함
int
cParty::getAbsoluteGoodMemberCount(int _iFieldSerial,cPOINT *_lpPos)
{
	int	iCount	=	0;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if (!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;
		if (_iFieldSerial	!=	0xffff	&&	lpMember->m_wCurrentField	!=	_iFieldSerial)
			continue;
		if (lpMember->isIdlePlayer())
			continue;

		if (_lpPos)
		{
			int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

			if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)	continue;
		}

		if (lpMember->isAbsoluteGood())
			iCount++;
	}

	return	iCount;
}

//
//	선 성향의 멤버들 수 구함
int
cParty::getGoodMemberCount(int _iFieldSerial,BOOL _isIncludeAbsoluteGood,cPOINT *_lpPos)
{
	int	iCount	=	0;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if (!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;
		if (_iFieldSerial	!=	0xffff	&&	lpMember->m_wCurrentField	!=	_iFieldSerial)
			continue;
		if (lpMember->isIdlePlayer())
			continue;

		if (_lpPos)
		{
			int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

			if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)	continue;
		}

		if (lpMember->isGood())	iCount++;
	}

	return	iCount;
}

//
//	중립 성향의 멤버들 수 구함
int
cParty::getNeutralMemberCount(int _iFieldSerial,cPOINT *_lpPos)
{
	int	iCount	=	0;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if (!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;
		if (_iFieldSerial	!=	0xffff	&&	lpMember->m_wCurrentField	!=	_iFieldSerial)
			continue;
		if (lpMember->isIdlePlayer())
			continue;

		if (_lpPos)
		{
			int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

			if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)	continue;
		}

		if (lpMember->isNeutral())	iCount++;
	}

	return	iCount;
}

//
//	악 성향의 멤버들 수 구함
int
cParty::getEvilMemberCount(int _iFieldSerial,BOOL _isIncludeAbsoluteEvil,cPOINT *_lpPos)
{
	int	iCount	=	0;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if (!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;
		if (_iFieldSerial	!=	0xffff	&&	lpMember->m_wCurrentField	!=	_iFieldSerial)
			continue;
		if (lpMember->isIdlePlayer())
			continue;

		if (_lpPos)
		{
			int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

			if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)	continue;
		}

		if (lpMember->isEvil())	iCount++;
		if (_isIncludeAbsoluteEvil	&&	lpMember->isAbsoluteEvil())	iCount++;
	}

	return	iCount;
}

//
//	절대악 성향의 멤버들 수 구함
int
cParty::getAbsoluteEvilMemberCount(int _iFieldSerial,cPOINT *_lpPos)
{
	int	iCount	=	0;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if (!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;
		if (_iFieldSerial	!=	0xffff	&&	lpMember->m_wCurrentField	!=	_iFieldSerial)
			continue;
		if (lpMember->isIdlePlayer())
			continue;

		if (_lpPos)
		{
			int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

			if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)	continue;
		}

		if (lpMember->isAbsoluteEvil())	iCount++;
	}

	return	iCount;
}

//
//	멤버 액터 포인터 리턴
cACTOR*
cParty::getActorByMemberIndex(int _iMember)
{
	cACTOR	*lpActor	=	g_game.getActor(m_aMember[_iMember].m_iZoneSerial);

	if	(lpActor && STRCMP(lpActor->m_strName,m_aMember[_iMember].m_strName) == 0)
		return	lpActor;

	return	g_game.getPlayer(m_aMember[_iMember].m_strName);
}

cACTOR*
cParty::getActorByMemberNameHashCode(DWORD _dwNameHashCode)
{

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if (!lpMember	||	lpMember->m_wSerialInField	==	0xffff)
			continue;

		if(lpMember->m_dwNameHashCode == _dwNameHashCode)
			return lpMember;

	}

	return NULL;
}

void
cParty::changeLastItemPickMember(char *_lpstrName)
{
	if (m_bf3ItemShareMethod	!=	ePISM_ORDER)
		return;

	int	iMemberIndex	=	getMemberIndex(_lpstrName);

	if (iMemberIndex	==	0xffff)
		return;

	m_bf3LastReceiveItemMember	=	iMemberIndex;

	g_game.sendWPChangePartyStatus(this,FALSE);
}

cACTOR*
cParty::getThisTimePicker(int _iField,cPOINT *_lpPos,cACTOR *_lpPicker,cItem	*_lpPickItem)
{
	WORD	awMemberList[dPARTY_MAXPLAYER];
	int		iCount;

	switch(m_bf3ItemShareMethod)
	{
		case	ePISM_FREE				:
			return	_lpPicker;

		case	ePISM_RANDOM			:
		{
			iCount	=	getInrangeMember(_iField,_lpPos,awMemberList);

			if (iCount	==	0)
				return	_lpPicker;

			cACTOR	*lpActor	=	getActorByMemberIndex(awMemberList[random(iCount)]);

			if	(!lpActor->getValidInventorySlot())
				return	_lpPicker;

			return	lpActor;
		}

		case	ePISM_ORDER				:
		{
			iCount		=	getInrangeMember(_iField,_lpPos,awMemberList);

			if (iCount	==	0)
				return	_lpPicker;

			int	iReceiveMember	=	-1;

			for (int i=0;i<iCount;i++)
			{
				if (awMemberList[i]	>	m_bf3LastReceiveItemMember)
				{
					iReceiveMember	=	awMemberList[i];
					break;
				}
			}

			if (iReceiveMember	==	-1)
				iReceiveMember	=	awMemberList[0];

			m_bf3LastReceiveItemMember	=	iReceiveMember;

			g_game.sendWPChangePartyStatus(this,FALSE);

			cACTOR	*lpActor	=	getActorByMemberIndex(iReceiveMember);

			if (!lpActor)
				return	_lpPicker;

			return	lpActor;
		}

		case	ePISM_FIRST_JOB_RANDOM	:
		{
			iCount	=	getInrangeMember(_iField,_lpPos,awMemberList,_lpPickItem);

			if (iCount	==	0)
				iCount	=	getInrangeMember(_iField,_lpPos,awMemberList);

			if (iCount	==	0)
				return	_lpPicker;

			cACTOR	*lpActor	=	getActorByMemberIndex(awMemberList[random(iCount)]);

			if (!lpActor->getValidInventorySlot())
				return	_lpPicker;

			return	lpActor;
		}

		case	ePISM_FIRST_JOB_ORDER	:
			break;
	}

	return	_lpPicker;
}

void
cParty::sharingGold(int _iField,cPOINT *_lpPos,cACTOR *_lpPicker,cItem	*_lpPickItem)
{
	cACTOR	*lpPicker		=	_lpPicker;
	WORD	awMemberList[dPARTY_MAXPLAYER];
	cItem	itemLog;
	int		iGold			=	_lpPickItem->m_dwSerial;
	int		iCount,iPickResult;

	itemLog.copy(_lpPickItem);

	SG_PICK_ITEM_RESULT	sgPacket;

	sgPacket.base.set(sizeof(sgPacket),dSG_PICK_ITEM_RESULT);
	sgPacket.wResult	=	eUIR_OK;

	memcpy(&sgPacket.item,_lpPickItem,sizeof(cItem));

	switch(m_bf3GoldShareMethod)
	{
		case	ePGSM_FREE		:
		{
			if	(_lpPickItem->isMoneyItem())
				iPickResult	=	lpPicker->addGoldItemToInventory(_lpPickItem,eGET_GOLD_BY_PICK_DROPPED_GOLD);
			else
				iPickResult	=	lpPicker->addItemToInventory(_lpPickItem);

			itemLog.m_dwSerial	=	iGold-_lpPickItem->m_dwSerial;

			if (itemLog.m_dwSerial)
				sendPickItemMessage(lpPicker,&itemLog);
			break;
		}

		case	ePGSM_EQUALITY	:
		{
			iCount		=	getInrangeMember(_iField,_lpPos,awMemberList);

			if	(iCount	==	0	||	_lpPickItem->m_dwSerial	<	(DWORD)iCount)
			{
				if	(_lpPickItem->isMoneyItem())
					iPickResult	=	lpPicker->addGoldItemToInventory(_lpPickItem,eGET_GOLD_BY_PICK_DROPPED_GOLD);
				else
					iPickResult	=	lpPicker->addItemToInventory(_lpPickItem);

				itemLog.m_dwSerial	=	iGold-_lpPickItem->m_dwSerial;

				if (itemLog.m_dwSerial)
					sendPickItemMessage(lpPicker,&itemLog);

				break;
			}

			int		iShareGold	=	_lpPickItem->m_dwSerial	/	iCount;
			int		iModGold	=	_lpPickItem->m_dwSerial	%	iCount;
			int		iRemainGold	=	0;
			cItem	itemAdd;

			for (int i=0;i<iCount;i++)
			{
				itemAdd.copy(_lpPickItem);
				itemAdd.m_dwSerial		=	iShareGold;

				cACTOR	*lpReceiver		=	getActorByMemberIndex(awMemberList[i]);

				if	(itemAdd.isMoneyItem())
					iPickResult	=	lpReceiver->addGoldItemToInventory(&itemAdd,eGET_GOLD_BY_PICK_DROPPED_GOLD);
				else
					iPickResult	=	lpReceiver->addItemToInventory(&itemAdd);

				memcpy(&sgPacket.item,_lpPickItem,sizeof(cItem));

				sgPacket.item.m_dwSerial	=	iShareGold;

				if (itemAdd.m_wBaseItem		!=	0xffff)
				{
					iRemainGold				+=	itemAdd.m_dwSerial;
					sgPacket.item.m_dwSerial-=	itemAdd.m_dwSerial;
				}

				sgPacket.wResult			=	iPickResult;

				g_userPM.add(lpReceiver->m_iClientSerial,(void *)&sgPacket,sgPacket.base.wSize);
			}

			iModGold			+=	iRemainGold;

			if (iModGold)
			{
				itemAdd.copy(_lpPickItem);
				memcpy(&sgPacket.item,_lpPickItem,sizeof(cItem));
				itemAdd.m_dwSerial		=	iModGold;
				sgPacket.item.m_dwSerial=	iModGold;

				if	(itemAdd.isMoneyItem())
					iPickResult	=	lpPicker->addGoldItemToInventory(&itemAdd,eGET_GOLD_BY_PICK_DROPPED_GOLD);
				else
					iPickResult	=	lpPicker->addItemToInventory(&itemAdd);

				if (itemAdd.m_wBaseItem	!=	0xffff)
					sgPacket.item.m_dwSerial-=	itemAdd.m_dwSerial;	

				sgPacket.wResult			=	iPickResult;

				g_userPM.add(lpPicker->m_iClientSerial,(void *)&sgPacket,sgPacket.base.wSize);
			}
			else
				itemAdd.reset();

			if (itemAdd.m_wBaseItem	!=	0xffff)
				_lpPickItem->m_dwSerial	=	itemAdd.m_dwSerial;
			else
				_lpPickItem->reset();

			return;
		}

		case	ePGSM_ORDER		:
		{
			iCount	=	getInrangeMember(_iField,_lpPos,awMemberList);

			int	iReceiveMember	=	-1;

			if (iCount)
			{
				for (int i=0;i<iCount;i++)
				{
					if (awMemberList[i]	>	m_bf3LastReceiveGoldMember)
					{
						iReceiveMember	=	awMemberList[i];
						break;
					}
				}

				if (iReceiveMember	==	-1)
					iReceiveMember			=	awMemberList[0];

				lpPicker		=	getActorByMemberIndex(iReceiveMember);
			}
			else
				iReceiveMember	=	getMemberIndex(lpPicker->m_strName);

			m_bf3LastReceiveGoldMember	=	iReceiveMember;

			g_game.sendWPChangePartyStatus(this,FALSE);

			if	(_lpPickItem->isMoneyItem())
				iPickResult	=	lpPicker->addGoldItemToInventory(_lpPickItem,eGET_GOLD_BY_PICK_DROPPED_GOLD);
			else
				iPickResult	=	lpPicker->addItemToInventory(_lpPickItem);

			itemLog.m_dwSerial			=	iGold-_lpPickItem->m_dwSerial;

			if (itemLog.m_dwSerial)
				sendPickItemMessage(lpPicker,&itemLog);

			break;
		}
	}

	sgPacket.wResult	=	iPickResult;

	if (_lpPickItem->m_wBaseItem	!=	0xffff)	//	돈이 남았다.
		sgPacket.item.m_dwSerial	=	sgPacket.item.m_dwSerial-_lpPickItem->m_dwSerial;

	g_userPM.add(lpPicker->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

	sgPacket.wResult	=	eUPR_PICK_ITEM_BY_PARTY_MEMBER;
	sgPacket.base.wSize	=	sizeof(sgPacket)-sizeof(cItem);

	g_userPM.add(_lpPicker->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
}

//
//	멤버 정보 리턴~
cPARTY_MEMBER*
cParty::getMemberInfoByIndex(int _iMember)
{
	if (_iMember < 0 || _iMember >= (int)m_wMemberCount)
		return	NULL;

	return	&m_aMember[_iMember];
}

//
//	멤버 정보 리턴~
cPARTY_MEMBER*
cParty::getMemberInfoByName(char *_lpstrName)
{
	for (int i=0;i<(int)m_wMemberCount;i++)
		if (STRCMP(m_aMember[i].m_strName,_lpstrName)==0)
			return	&m_aMember[i];

	return	NULL;
}

cPARTY_MEMBER*
cParty::getMemberInfoByName(char *_lpstrName,DWORD _dwNameHashcode)
{
	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		if (m_aMember[i].m_dwNameHashCode	!=	_dwNameHashcode)
			continue;

		if (STRCMP(m_aMember[i].m_strName,_lpstrName)==0)
			return	&m_aMember[i];
	}

	return	NULL;
}

BOOL
cParty::changeMemberConnectStatus(char *_strMemberName,BOOL _bIsDisconnect)
{
	int	iIndex	=	getMemberIndex(_strMemberName);

	if	(iIndex	==	0xffff	)
		return	FALSE;

	m_aMember[iIndex].m_bf1IsDisconnected	=	_bIsDisconnect;

	return	TRUE;
}

//
//	멤버 제거
BOOL
cParty::changeLeader(char *_strMemberName)
{
	int	iIndex	=	getMemberIndex(_strMemberName);

	if	(iIndex	==	0xffff	)
		return	FALSE;

	cPARTY_MEMBER	temp;

	memcpy(&temp,&m_aMember[0],sizeof(cPARTY_MEMBER));
	memcpy(&m_aMember[0],&m_aMember[iIndex],sizeof(cPARTY_MEMBER));
	memcpy(&m_aMember[iIndex],&temp,sizeof(cPARTY_MEMBER));
	
	return	TRUE;
}

//
//	멤버 제거
BOOL
cParty::removeMember(char *_strMemberName)
{
	int	iIndex	=	getMemberIndex(_strMemberName);

	if (iIndex	==	0xffff	)
		return	FALSE;

	cACTOR	*lpMember	=	g_game.getPlayer(m_aMember[iIndex].m_strName);

	if (lpMember && lpMember->getPartySerial() != 0xffff)
	{
		cParty	*lpParty	=	lpMember->getParty();

		if (lpMember->getPartySerial()	==	m_wSerial || lpParty	==	NULL)
		{
			if (lpParty	==	NULL)
				CLOG("party","removeMember");

			lpMember->leaveParty();
		}
	}

	for (int i=iIndex;i<(int)m_wMemberCount-1;i++)
		memcpy(&m_aMember[i],&m_aMember[i+1],sizeof(cPARTY_MEMBER));

	m_wMemberCount--;
	
	return	TRUE;
}

BOOL
cParty::disconnectMember(char *_strMemberName)
{
	int	iIndex	=	getMemberIndex(_strMemberName);

	if (iIndex	==	0xffff	)
		return	FALSE;

	m_aMember[iIndex].m_bf1IsDisconnected	=	TRUE;

	syncMemberInfo(_strMemberName);

	if (iIndex==0)
	{
		for (int i=1;i<(int)m_wMemberCount;i++)
		{
			if(m_aMember[i].m_bf1IsDisconnected	==	FALSE)
			{
				cPARTY_MEMBER	leader;

				memcpy(&leader,&m_aMember[0],sizeof(cPARTY_MEMBER));
				memcpy(&m_aMember[0],&m_aMember[i],sizeof(cPARTY_MEMBER));
				memcpy(&m_aMember[i],&leader,sizeof(cPARTY_MEMBER));
			}
		}
	}

	return	TRUE;
}

//
//	멤버들의 상태 싱크 시킴
void
cParty::syncMemberStatus(cACTOR *_lpActor)
{
	cPARTY_MEMBER	*lpMemberInfo	=	getMemberInfoByName(_lpActor->m_strName);

	if	(!lpMemberInfo)
		return;

	lpMemberInfo->m_bf8RestHP				=	_lpActor->m_iHP*100/_lpActor->getMaxHP();
	lpMemberInfo->m_wFieldSerial			=	_lpActor->m_wCurrentField;
	lpMemberInfo->m_bf4PlaySecretDungeonCount=	_lpActor->m_bf4SecretDungeonPlayCount;
	lpMemberInfo->m_bf8Job					=	_lpActor->m_wJob;
	lpMemberInfo->m_bf10Level				=	_lpActor->m_iLevel;
	lpMemberInfo->m_iDuelVP					=	_lpActor->m_duelRecord.m_iWeeklyWinPoint;
}

void
cParty::syncMemberHP(cACTOR *_lpActor)
{
	cPARTY_MEMBER	*lpMemberInfo	=	getMemberInfoByName(_lpActor->m_strName);

	if	(!lpMemberInfo)
		return;

	if(m_bf1IsUnionParty)
	{
		cUnionParty *lpUnionParty  = g_pUnionpartyManager->getDenIndex(m_wUnionPartyserial);

		if(!lpUnionParty)
			return;

		lpUnionParty->syncUnionPartyMemberHP(_lpActor);
		return;
	}

	lpMemberInfo->m_bf8RestHP				=	_lpActor->m_iHP*100/_lpActor->getMaxHP();
	lpMemberInfo->m_wFieldSerial			=	_lpActor->m_wCurrentField;
	lpMemberInfo->m_bf4PlaySecretDungeonCount=	_lpActor->m_bf4SecretDungeonPlayCount;
	lpMemberInfo->m_bf8Job					=	_lpActor->m_wJob;

	LONGLONG	llMaxHP		=	_lpActor->getMaxHP();
	LONGLONG	llHP		=	_lpActor->m_iHP;

	llHP					=	llHP*60000/llMaxHP;

	SG_UPDATE_PARTY_MEMBER_HP	packet;

	packet.base.set(sizeof(packet),dSG_UPDATE_PARTY_MEMBER_HP);

	packet.wRemainHPRate	=	(WORD)llHP;
	packet.wActor			=	_lpActor->m_wSerialInField;
	packet.dwMaxHP			=	_lpActor->getMaxHP();

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if	(!lpMember || STRICMP(lpMember->m_strName,_lpActor->m_strName)==0 || lpMember->m_wCurrentField != _lpActor->m_wCurrentField)
			continue;

		g_userPM.add(lpMember->m_iClientSerial,&packet,packet.base.wSize);	//	전송!!
	}
}

//
//	특정 멤버들의 정보를 싱크 시킴
void
cParty::syncMemberInfo(char *_lpstrName)
{
	SG_UPDATE_PARTY_MEMBER_INFO		packet;
	cPARTY_MEMBER					*lpMemberInfo	=	getMemberInfoByName(_lpstrName);
	int								iIndex			=	getMemberIndex(_lpstrName);

	if	(iIndex	==	0xffff)
		return;

	packet.base.set(sizeof(packet),dSG_UPDATE_PARTY_MEMBER_INFO);
	packet.bIndex					=	iIndex;
	packet.bRestHp					=	lpMemberInfo->m_bf8RestHP;
	packet.wLevel					=	lpMemberInfo->m_bf10Level;
	packet.wField					=	lpMemberInfo->m_wFieldSerial;
	packet.bf10InstanceField		=	lpMemberInfo->m_bf10InstanceField;
	packet.bf4InstanceFieldFloor	=	lpMemberInfo->m_bf4InstanceFieldFloor;
	packet.bf1IsDisconnected		=	lpMemberInfo->m_bf1IsDisconnected;
	packet.bf4SecretDungeonPlayCount=	lpMemberInfo->m_bf4PlaySecretDungeonCount;
	packet.bf8Job					=	lpMemberInfo->m_bf8Job;

	packet.iDuelVP					=	lpMemberInfo->m_iDuelVP;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if (!lpMember)
			continue;

		g_userPM.add(lpMember->m_iClientSerial,&packet,packet.base.wSize);	//	전송!!
	}
}

//
//	멤버들의 정보에 문제가 없나 확인
BOOL
cParty::checkMember()
{
	int	i;

	for (i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR			*lpActor	=	g_game.getPlayer(m_aMember[i].m_strName);

		if (!lpActor	)
			continue;

		m_aMember[i].m_bf10Level				=	lpActor->m_iLevel;
		m_aMember[i].m_wFieldSerial				=	lpActor->m_wCurrentField;
		m_aMember[i].m_bf8RestHP				=	lpActor->m_iHP*100/lpActor->getMaxHP();
		m_aMember[i].m_bf8Job					=	lpActor->m_wJob;
		m_aMember[i].m_bf4PlaySecretDungeonCount=	lpActor->m_bf4SecretDungeonPlayCount;
		m_aMember[i].m_bf3PlayGuildDungeonCount	=	lpActor->m_bf3GuildDungeonPlayTime;
	}

	return	TRUE;
}

BOOL
cParty::addMember(cPARTY_MEMBER *_lpNewMember)//멤버추가
{
	if (m_wSerial		==	0xffff			)
		return	FALSE;
	if ((int)m_wMemberCount	>=	dPARTY_MAXPLAYER)
		return	FALSE;

	if (getMemberIndex(_lpNewMember->m_strName) != 0xffff)
	{
		CLOG("party","addMember 이미존재하는 멤버 [%s]",_lpNewMember->m_strName);

		return	FALSE;
	}

	memcpy(&m_aMember[(int)m_wMemberCount],_lpNewMember,sizeof(cPARTY_MEMBER));
	m_aMember[(int)m_wMemberCount].m_bf8RestHP	=	100;

	m_wMemberCount++;

	return	TRUE;
}

//멤버추가
BOOL
cParty::joinPartyMember(CSimplePartyMemberInfo *_lpMember)
{	
	if (m_wSerial		==	0xffff			)
		return	FALSE;
	if ((int)m_wMemberCount	>=	dPARTY_MAXPLAYER)
		return	FALSE;

	int		iSlot			=	(int)m_wMemberCount;
	int		iMemberIndex	=	getMemberIndex(_lpMember->m_strName);
	BOOL	bIsReconnect	=	FALSE;

	if (iMemberIndex	!= 0xffff)
	{
		iSlot			=	iMemberIndex;
		bIsReconnect	=	TRUE;
	}

	m_aMember[iSlot].copy(_lpMember);

	if (bIsReconnect	==	FALSE)
		m_wMemberCount++;

	if (m_bf1IsUnionParty)
		g_pUnionpartyManager->sendUnionPartyMemberInfoToAllUnionParty(m_wUnionPartyserial , m_wUnionPartyIndex , iSlot);

	return	TRUE;
}


//
//	데이터 체크
BOOL
cParty::checkData()
{
	if	(m_wSerial	==	0xffff	)
		return	TRUE;

	if	((int)m_wMemberCount	<=	1)
	{
		g_game.sendWPAskPartyInfo(m_wSerial,m_strPartyName);

		return	FALSE;
	}

	int	i;

	for (i=0;i<(int)m_wMemberCount;i++)
	{
		cPARTY_MEMBER	*lpMember	=	&m_aMember[i];

		if	(lpMember->m_bf1IsMoveField || lpMember->m_bf1IsDisconnected)
			continue;

		char	strLeaveMemberName[dNAME_LENGTH];

		strcpy(strLeaveMemberName,lpMember->m_strName);

		cFIELD	*lpField	=	g_game.getFieldByUniqueSerial(lpMember->m_wFieldSerial);

		if	(!lpField)
			continue;

		cACTOR	*lpActor	=	lpField->getPlayerByName(strLeaveMemberName);

		if	(!lpActor)
		{
			CLOG("party","checkData : leave member by member not on info field [%s]",lpMember->m_strName);
			removeMember(strLeaveMemberName);
			g_game.sendWPPartyWork(m_wSerial,ePW_LEAVE_PARTY,strLeaveMemberName);
			i--;

			continue;
		}

		if	(lpActor->getPartySerial()	==	m_wSerial)
			continue;

		cParty	*lpParty	=	lpActor->getParty();

		if	(lpParty)
		{
			g_game.sendWPPartyWork(m_wSerial,ePW_LEAVE_PARTY,strLeaveMemberName);
			CLOG("party","checkData : leave member by mismatch party serial [%s]",lpMember->m_strName);
		}
		else
		{
			CLOG("party","checkData : leave member by mismatch party serial [%s]",lpMember->m_strName);

			g_game.sendWPAskPartyInfo(m_wSerial,m_strPartyName);
			removeMember(strLeaveMemberName);
			i--;
		}
	}

	return	TRUE;
}

void
cParty::disconnectField(int *_lpiFieldList,int _iFieldCount)
{
	int	i,j,iMemberCount=(int)m_wMemberCount;
	int	aLeaveMember[dPARTY_MAXPLAYER],iLeaveMemberCount=0;

	for (i=0;i<_iFieldCount;i++)
		for (j=0;j<(int)m_wMemberCount;j++)
			if	(m_aMember[j].m_wFieldSerial==	_lpiFieldList[i])
				aLeaveMember[iLeaveMemberCount++]	=	j;

	if (m_wMemberCount	-	iLeaveMemberCount	<=	1)	//	뽀게믄 된다.
	{
		m_wMemberCount	-=	iLeaveMemberCount;
		return;
	}

	char	astrMemberName[dPARTY_MAXPLAYER][dNAME_LENGTH];

	for (i=0;i<iLeaveMemberCount;i++)
		strcpy(astrMemberName[i],m_aMember[aLeaveMember[i]].m_strName);

	for (i=0;i<iLeaveMemberCount;i++)
	{
		CLOG("party","leavePartyBy disconnectField [%s]",astrMemberName[i]);
		g_pPartyManager->operatePartyWork(m_wSerial,m_dwUniqueSerial,ePW_LEAVE_PARTY,astrMemberName[i]);
	}
}

int
cParty::getAverageLevel(int _iField,cPOINT	*_lpPos)
{
	int	iLevelSum	=	0;
	int	iCount		=	0;

	if	(m_wMemberCount	<=	0)
		return	1;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		if	(_iField	!=	0xffff)
		{
			cACTOR	*lpMember	=	getActorByMemberIndex(i);

			if	(!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
				continue;
			if	(_iField	!=	0xffff	&&	lpMember->m_wCurrentField	!=	_iField)
				continue;
			if	(lpMember->isIdlePlayer())
				continue;

			if	(_lpPos)
			{
				int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPos->x,_lpPos->y);

				if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)
					continue;
			}
		}

		iLevelSum	+=	m_aMember[i].m_bf10Level;
		iCount		++;
	}

	if	(iCount)
		return	iLevelSum/iCount;

	return	1;
}

BOOL
cParty::checkMemberLevel(int _iMinLevel,int _iMaxLevel)// 레벨 체크 수정. ~~ 09.09.22
{
	if	(_iMaxLevel	==	0)
		return	TRUE;

	for(int i=0;i<(int)m_wMemberCount;++i)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);
		
		if(!lpMember)
			return FALSE;

		if(lpMember->m_iLevel <	(DWORD)_iMinLevel)
			return FALSE;
		
		if (lpMember->m_iLevel	>	(DWORD)_iMaxLevel)
			return	FALSE;				
	}

	return	TRUE;
}

BOOL
cParty::checkSecretDungeonLimitLevel(int _iLevel)
{
	if	(m_wGateField	==	0xffff)
		return	TRUE;

	cInstanceField	*lpIF		=	g_pIfManager->getByUniquePartySerial(m_dwUniqueSerial);

	if	(!lpIF)
		return	TRUE;

	cSecretDungeon	*lpDungeon	=	g_pSdManager->get(lpIF->m_wSecretDungeonSerial);

	if	(lpDungeon->m_wMaxLevel	==	0	)
		return	TRUE;

	if	(_iLevel	<	lpDungeon->m_wMinLevel)
		return	FALSE;

	if	(_iLevel	>	lpDungeon->m_wMaxLevel)
		return	FALSE;

	return	TRUE;
}

BOOL
cParty::isOnField(int _iMember,int _iFieldSerial,int _iGateField)
{
	int	iCount	=	1;

	if	(_iMember	==	-1)
	{
		iCount		=	m_wMemberCount;
		_iMember	=	0;
	}

	for	(int i =0;i<iCount;i++)
	{
		int	iMember	=	_iMember+i;

		if	(_iFieldSerial	<	dINSTANCE_FIELD_FLAG)
		{
			if	(m_aMember[iMember].m_wFieldSerial	==	_iFieldSerial)
				continue;

			return	FALSE;
		}

		if	(m_aMember[iMember].m_bf10InstanceField					!=	_iFieldSerial/dINSTANCE_FIELD_FLAG)
			return	FALSE;
		if	(m_aMember[iMember].m_bf4InstanceFieldFloor				!=	_iFieldSerial%dINSTANCE_FIELD_FLAG)
			return	FALSE;
		if	(m_aMember[iMember].m_wFieldSerial-dINSTANCE_FIELD_FLAG	!=	_iGateField)
			return	FALSE;
		//if	(m_aMember[iMember].m_wFieldSerial-dINSTANCE_FIELD_FLAG	!=	m_wGateField)
		//	return	FALSE;
	}

	return	TRUE;
}

DWORD
cParty::getCheckSum()
{
	DWORD	dwCheckSum	=	0;

	dwCheckSum	+=	m_wMemberCount;
	dwCheckSum	+=	m_dwNameHashCode;

	for (int i=0;i<(int)m_wMemberCount;i++)
	{
		dwCheckSum	+=	GetHashCode(m_aMember[i].m_strName);
		dwCheckSum	-=	m_aMember[i].m_wFieldSerial;
	}
	
	dwCheckSum	+=	m_dwNameHashCode;

	return	dwCheckSum;
}

void
cParty::sendPacketToAllMember(void *_lpPacket,int _iSize)
{
	for (int i=0;i<m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	getActorByMemberIndex(i);

		if	(!lpActor)
			continue;

		g_userPM.add(lpActor->m_iClientSerial,_lpPacket,_iSize);
	}
}

void
cParty::sendPacketToSameFieldMember(cACTOR *_lpSender,void *_lpPacket,int _iSize,int _iRange)
{
	if	(!_lpSender)
		return;

	for (int i=0;i<m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	getActorByMemberIndex(i);

		if	(!lpActor	||	_lpSender->isOnSameField(lpActor)	==	FALSE)
			continue;

		if	(_iRange	!=	-1)
			if	(!lpActor->isInAttackRange(_lpSender->m_pos.x,_lpSender->m_pos.y,_iRange))
				continue;

		g_userPM.add(lpActor->m_iClientSerial,_lpPacket,_iSize);
	}
}

cItem*
cParty::getItemByBasicItem(int _iBaseItem)
{
	int	i;

	for (i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	g_game.getActor(m_aMember[i].m_iZoneSerial);

		if	(!lpActor)
			continue;

		cItem	*lpItem		=	lpActor->getItemByBasicItem(_iBaseItem);

		if	(lpItem)
			return	lpItem;
	}

	return	NULL;
}

void
cParty::sendPickItemMessage(cACTOR *_lpPicker,cItem *_lpItem,BOOL _bIsSharingGold)
{
	int	i;

	int	iMemberIndex	=	getMemberIndex(_lpPicker->m_strName);

	if	(iMemberIndex	==	0xffff)
		return;

	for (i=0;i<(int)m_wMemberCount;i++)
	{
		if	(i	==	iMemberIndex)
			continue;

		if	(m_aMember[i].m_wFieldSerial!=	_lpPicker->m_wCurrentField)
			continue;

		cACTOR	*lpActor	=	g_game.getActor(m_aMember[i].m_iZoneSerial);

		if	(!lpActor)
			continue;

		if	(m_aMember[i].m_wFieldSerial>=	dINSTANCE_FIELD_FLAG)
			if	(!lpActor->isOnSameField(_lpPicker))
				continue;

		if	(STRCMP(lpActor->m_strName,m_aMember[i].m_strName)	!=	0)
			continue;

		lpActor->sendPickPartyItemMessage(_lpPicker->m_strName,_lpItem,_bIsSharingGold);
	}
}

void
cParty::sendEnterToTherSecretDungeonMessage(int _iField,int _iArea)
{
	int	i;

	g_game.sendWPEnterIFParty(m_wSerial,m_strPartyName,_iField,_iArea);

	SG_ENTER_TO_THE_SECRET_DUNGEON	packet;

	packet.base.set(sizeof(packet),dSG_ENTER_TO_THE_SECRET_DUNGEON);
	packet.wGateArea	=	_iArea;
	packet.wGateField	=	_iField;

	for (i=0;i<(int)m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	g_game.getActor(m_aMember[i].m_iZoneSerial);

		if (!lpActor)
			continue;

		if (STRCMP(lpActor->m_strName,m_aMember[i].m_strName)	!=	0)
			continue;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}
}

int
cParty::getSecretDungeonScore()
{
	int		i;
	int		iPoint					=	0;
	int		iPlaySecretDungeonCount	=	0;

	iPoint	=	m_wMemberCount	*	2;

	for (i=0;i<m_wMemberCount;i++)
	{
		if	(m_aMember[i].m_bf8Job	==	dJOB_PRIEST			||
			m_aMember[i].m_bf8Job	==	dJOB_FALLEN_ANGEL	||
			m_aMember[i].m_bf8Job	==	dJOB_FIGHTER		||
			m_aMember[i].m_bf8Job	==	dJOB_ROGUE)
			iPoint	+=	2;

		iPlaySecretDungeonCount		+=	m_aMember[i].m_bf4PlaySecretDungeonCount;
		
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if	(lpMember)
			iPlaySecretDungeonCount		+=	lpMember->m_wCorrectSecretDungeonPlayCount;
	}

	int	iPlayCountScore	=	iPlaySecretDungeonCount-dPARTY_MAXPLAYER*dENTER_SECRET_DUNGEON_LIMIT_PER_ONE_DAY;
	iPlayCountScore		=	-iPlayCountScore;

	iPoint				+=	iPlayCountScore;
	iPoint				=	max(iPoint,1);

	return	iPoint;
}

void
cParty::operateOncePerSecond()
{
}

int
cParty::getCountOfMemberOwnedItem(WORD _wSerial, BOOL _bIsIncludEquippedItem)
{

	int iCount = 0;

	for	(int i=0;i<m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	getActorByMemberIndex(i);
		
		if	(!lpActor)
			continue;
		if(_wSerial == 0xffff)
			break;
		if(lpActor->getItemCount(_wSerial,_bIsIncludEquippedItem))
			++iCount;
	}

	return iCount;
}


int	
cParty::getItemCountInParty(WORD _wSerial, BOOL _bIsIncludEquippedItem)
{

	int iCount = 0;

	for	(int i=0;i<m_wMemberCount;i++)
	{
		cACTOR	*lpActor	=	getActorByMemberIndex(i);
		
		if	(!lpActor)
			continue;

		iCount += lpActor->getItemCount(_wSerial,_bIsIncludEquippedItem);
	}

	return iCount;

}
DWORD 
cPARTY_MEMBER::getInfoForUnionPartyState()
{
	DWORD	dwState = 0;

	if(m_bf1IsMoveField)
		dwState |= dUNION_PARTY_MEMBER_STATE_DISCONNECT;

	if(m_bf1IsMoveField)
		dwState |=	dUNION_PARTY_MEMBER_STATE_OTHERFILED;

	return	dwState;
}


BOOL
cPARTY_MEMBER::isInSameField(int _iField,int _iIF,int _iIFFloor)
{
	if	(_iField	==	m_wFieldSerial)
		return	TRUE;
	
	if	(_iField	<	dINSTANCE_FIELD_FLAG)
		return	FALSE;

	if	(m_bf4InstanceFieldFloor	!=	_iIFFloor)
		return	FALSE;

	if	(m_bf10InstanceField		!=	_iIF)
		return	FALSE;

	return	TRUE;
}

BOOL
cParty::isNotJJUL(cACTOR *_lpChecker)
{
	if	(!_lpChecker)
		return	TRUE;

	int iMinLevel		=	1000;
	int	iMaxLevel		=	0;

	for	(int iMember=0;iMember<m_wMemberCount;iMember++)
	{
		if	(iMinLevel	>	(int)m_aMember[iMember].m_bf10Level)
			iMinLevel	=	(int)m_aMember[iMember].m_bf10Level;
		if	(iMaxLevel	<	(int)m_aMember[iMember].m_bf10Level)
			iMaxLevel	=	(int)m_aMember[iMember].m_bf10Level;
	}

//	①	파티내 최고렙의 레벨이 [내 레벨+50] 보다 작거나 같다.
	if	(iMaxLevel	<=	_lpChecker->m_iLevel+50)
		return	TRUE;

	return	FALSE;
}

BOOL
cParty::isGoodParty(int _iLevelRange,cACTOR *_lpTarget,cACTOR *_lpChecker)
{
	if	(!_lpChecker || !_lpTarget)
		return	FALSE;

	int	iAverageLevel	=	getAverageLevel(0xffff,NULL);

	if	(iAverageLevel	<=	_lpTarget->m_iLevel+_iLevelRange)
	{
		int iMinLevel		=	1000;
		int	iMaxLevel		=	0;

		for	(int iMember=0;iMember<m_wMemberCount;iMember++)
		{
			if	(iMinLevel	>	(int)m_aMember[iMember].m_bf10Level)
				iMinLevel	=	(int)m_aMember[iMember].m_bf10Level;
			if	(iMaxLevel	<	(int)m_aMember[iMember].m_bf10Level)
				iMaxLevel	=	(int)m_aMember[iMember].m_bf10Level;
		}

		if	(iMaxLevel	<=	iMinLevel+50)
			return	TRUE;
	}

	return	FALSE;
}


cACTOR *
cParty::getRandomInrangeMember(cACTOR *_lpPicker)
{
	int		iCount	=	0;
	WORD	awInrangeMember[8];
	int		iField	=	_lpPicker->m_wCurrentField;

	for (int i=0;i<m_wMemberCount;i++)
	{
		cACTOR	*lpMember	=	getActorByMemberIndex(i);

		if	(!lpMember	||	lpMember->m_wSerialInField	==	0xffff	||	lpMember->isDeath())
			continue;

		if	(iField	>=	dINSTANCE_FIELD_FLAG)
		{
			if	(lpMember->m_wCurrentField	<	dINSTANCE_FIELD_FLAG)
				continue;

			cFIELD	*lpIF	=	g_pIfManager->getFloor(lpMember->m_bf8InstanceField,lpMember->m_bf4InstanceFieldFloor,m_dwUniqueSerial);

			if	(!lpIF)
				continue;

			if	(lpMember->m_bf8InstanceField	!=	(DWORD)(iField/dINSTANCE_FIELD_FLAG))
				continue;
			if	(lpMember->m_bf4InstanceFieldFloor!=	(DWORD)(iField%dINSTANCE_FIELD_FLAG))
				continue;
			if	(lpIF->m_wGateField				!=	lpMember->m_wCurrentField-dINSTANCE_FIELD_FLAG)
				continue;
		}
		else
		if	(iField	!=	lpMember->m_wCurrentField)
			continue;

		if	(lpMember->isIdlePlayer())
			continue;
		if	(m_aMember[i].m_bf1IsDisconnected)
			continue;

		int	iRange		=	GetOvalRange(lpMember->m_pos.x,lpMember->m_pos.y,_lpPicker->m_pos.x,_lpPicker->m_pos.y);

		if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE)
			continue;

		awInrangeMember[iCount]	=	i;

		iCount++;
	}

	if	(iCount	==	0)
		return	NULL;

	int	iIndex	=	random(iCount);

	return	getActorByMemberIndex(iIndex);
}

