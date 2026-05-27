#include	"cFIELD.H"
#include	"cGAME.H"
#include	"Client.H"
#include	"debugCode.H"
#include	"CGuild.H"
#include	"secretDungeon.H"
#include	"cEvent.H"
#include	"cProject.H"

//
//	액터 제거
BOOL
cFIELD::removeActor(int _iSerial,BOOL _bIsSleep)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if	(!lpActor								)
	{
		CLOG("deathActor","------------------------- [%8d,%8d]액터 없음",m_iSerial,_iSerial);
		return	FALSE;
	}

	if	(lpActor->m_wSerialInField	==	0xffff	)
	{
		CLOG("deathActor","------------------------- [%8d,%8d,%d]시리얼 비어 있음",m_iSerial,_iSerial,lpActor->isPlayer());
		return	FALSE;
	}

	readyToRemoveActorWork(_iSerial);

	int	iTeam	=	lpActor->m_wTeam;

	if	(lpActor->isNpc())
	{
		CLOG("NPC","cFIELD::removeActor - 뭐꼬 이게?");
	}

//	NPC리스트에 죽었다고 통보
	if (lpActor->m_wNpcActorSaveSerial	!=	0xffff)
	{
		cNPC	*lpNpc	=	m_apNpcGenerateData[lpActor->m_wNpcActorSaveSerial];

		lpNpc->reset(_bIsSleep);
		lpNpc->m_iLastTime	=	g_iCounterBySecond;

		if (lpActor->isNpc())
			lpNpc->m_iRegenerationCycle	=	1;

		if(lpActor->m_bf1IsEventSummonMonster)
		{
			m_aCharacter[lpNpc->m_wCharacter].reset(); 
			m_apNpcGenerateData[lpActor->m_wNpcActorSaveSerial] = NULL;
			lpActor->m_lpCharacter = NULL;
			KILL(lpNpc);
			--m_iCharacterCount;
			--m_iSavedActorCount;
			--m_iSaveMonsterCount;
		}

	}

//	리스트에서 제거하고...
	removeActorInList(_iSerial,iTeam,lpActor->m_iSmallLocalPart);

//	존 전체에서 제거하고..
	if (!g_game.removeActor(lpActor->m_iZoneSerial))
		CLOG("deathActor","------------------------- [%8d]reset in cGAME::removeActor - Failed in Game",lpActor->m_iZoneSerial);

//	필드에서 제거
	{
		m_alpActor[_iSerial]	=	NULL;

		if (addReadyToRebirthTimeActor(_iSerial)	==	FALSE)
			if(_iSerial	<=	m_iRookieActor)
				m_iRookieActor=	_iSerial;
	}

	return	TRUE;
}

BOOL
cFIELD::addToReadyToRemoveActorList(int _iActor)
{
	if (m_iBookedReadyToRemoveActorCount	>=	dBOOKED_READY_TO_REMOVE_ACTOR_COUNT)
	{
		readyToRemoveActorWork(_iActor);

		return	FALSE;
	}

	m_awBookedReadyToRemoveActorList[m_iBookedReadyToRemoveActorCount++]	=	_iActor;

	return	TRUE;
}

void
cFIELD::operateReadyToRemoveActorList()
{
	for (int i=m_iBookedReadyToRemoveActorCount;i;i--)
		readyToRemoveActorWork(m_awBookedReadyToRemoveActorList[i-1]);

	m_iBookedReadyToRemoveActorCount	=	0;
}

void
cFIELD::addActorAtList(int _iIndex,int _iTeam,int _iSmallPart)
{
	m_awActorList[_iTeam][m_aiActorCount[_iTeam]++]	=	_iIndex;
	addActorAtLocalPart(_iIndex,_iTeam,_iSmallPart);

	m_iActorCount++;
}

BOOL
cFIELD::removeActorInList(int _iIndex,int _iTeam,int _iSmallPart)
{
	int	iListIndex	=	0xffff;

	for (int i=m_aiActorCount[_iTeam];i;i--)	
		if (m_awActorList[_iTeam][i-1]	==	_iIndex)
		{
			iListIndex	=	i-1;
			break;
		}

	if (iListIndex	!=	0xffff)
	{
		m_awActorList[_iTeam][iListIndex]	=	m_awActorList[_iTeam][m_aiActorCount[_iTeam]-1];

		m_aiActorCount[_iTeam]--;
	}
	else
	{
		CLOG("removeActorInList","occur!!");
		return	FALSE;
	}

	removeActorAtLocalPart(_iIndex,_iTeam,_iSmallPart);

	m_iActorCount--;

	return	TRUE;
}

int
cFIELD::getTeam(int _iGuildSerial,int _iActorKind,char *_lpstrName)
{
	if	(m_bIsGuildBattleField || m_wDuelType != 0xffff)
	{
		if	(_iActorKind	!=	eAK_PLAYER)	//	플레이어가 아니면 전부 2번팀
		{
			if	(m_bIsSiegeWarfareField)
				return	0;

			return	2;
		}

		return	getTeamInGuildBattleField(_iGuildSerial,_lpstrName);
	}

	if (_iActorKind	==	eAK_PLAYER)
		return	dTEAM_PLAYER;

	if (_iActorKind	==	eAK_MONSTER)
		return	dTEAM_MONSTER;

	return	dTEAM_NPC;
}

BOOL
cFIELD::addReadyToRebirthTimeActor(int _iSerial)
{
	if	(m_iReadyToRebirthActorCount	>=	dMAX_READY_TO_REBIRTH_ACTOR_COUNT)
		return	FALSE;

	m_aReadyToRebirthActor[m_iReadyToRebirthActorCount].m_wSerial		=	_iSerial;
	m_aReadyToRebirthActor[m_iReadyToRebirthActorCount].m_wRemainSecond	=	2;
	m_abIsReadyToRebirthActor[_iSerial]									=	TRUE;

	m_iReadyToRebirthActorCount++;

	return	TRUE;
}

void
cFIELD::updateReadyToRebirthTimeActor()
{
	for (int i=0;i<m_iReadyToRebirthActorCount;)
	{
		cReadyToRebirthActorInfo	*lpInfo	=	&m_aReadyToRebirthActor[i];

		lpInfo->m_wRemainSecond--;

		if	(lpInfo->m_wRemainSecond)
			i++;
		else
		{
			m_iReadyToRebirthActorCount--;
			m_abIsReadyToRebirthActor[lpInfo->m_wSerial]	=	FALSE;

			if	(lpInfo->m_wSerial	<	m_iRookieActor)
			{
				if (m_alpActor[lpInfo->m_wSerial]	==	NULL)
					m_iRookieActor	=	lpInfo->m_wSerial;
				else
					CLOG("critical","m_alpActor[lpInfo->m_wSerial] != NULL");
			}

			memcpy(lpInfo,&m_aReadyToRebirthActor[m_iReadyToRebirthActorCount],sizeof(cReadyToRebirthActorInfo));
		}
	}
}

//
//	액터를 추가한다.
cACTOR*
cFIELD::addActor(int _iTeam,int _iActorKind,int _iX,int _iY)
{
	BOOL	bIsFailedTeam	=	FALSE;
	cACTOR	*lpActor		=	NULL;

	if (_iTeam	>=	dTEAM_COUNT)
	{
		bIsFailedTeam	=	TRUE;
		_iTeam			=	0;
	}

	if	(m_aiActorCount[_iTeam]	>=	dMAX_PLAYER_IN_FIELD)
		return	NULL;

	lpActor	=	g_game.getFreeActor();

	if	(!lpActor)
		return	NULL;

	int	iLocalPart	=	getCurrentSmallLocalPart(_iX,_iY);

	if (_iTeam	==	2	&&	_iActorKind == eAK_PLAYER)
	{
		CLOG("npc","cFIELD::addActor is npc team!!");
	}

	addActorAtList(m_iRookieActor,_iTeam,iLocalPart);	//	몬스터/플레이어 리스트에 추가

	m_alpActor[m_iRookieActor]	=	lpActor;
	lpActor->m_wSerialInField	=	m_iRookieActor;
	lpActor->m_wTeam			=	_iTeam;
	lpActor->m_wOldTeam			=	_iTeam;
	lpActor->m_wActorKind		=	_iActorKind;
	lpActor->m_iSmallLocalPart	=	iLocalPart;
	lpActor->m_iLocalPart		=	getCurrentLocalPart(_iX,_iY);;
	lpActor->m_iMoveNumber		=	lpActor->m_iZoneSerial%dUPDATE_UPDATE_MOVE_POS_PERIOD;

	if (bIsFailedTeam)
		lpActor->m_wIsIncorrectGuildBattleFieldJoin	=	TRUE;

	if (m_aiActorCount[0] + m_aiActorCount[1] + m_aiActorCount[2] != m_iActorCount)
	{
		CLOG("AddPlayer","액터 카운트가 엉켰다. %d+%d+%d!=%d",m_aiActorCount[0],m_aiActorCount[1], m_aiActorCount[2],m_iActorCount);

		rebuildActorList();
	}

	for (int i=m_iRookieActor+1;i<dMAX_ACTOR_IN_FIELD;i++)
		if (m_alpActor[i]	==	NULL	&& m_abIsReadyToRebirthActor[i]	==	FALSE)
		{
			m_iRookieActor	=	i;
			break;
		}

	lpActor->m_lpField			=	this;

	if (m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		lpActor->m_wCurrentField		=	m_iUniqueSerial;
	else
	{
		lpActor->m_wCurrentField		=	m_wGateField+dINSTANCE_FIELD_FLAG;
		lpActor->m_bf8InstanceField		=	m_iUniqueSerial/dINSTANCE_FIELD_FLAG;
		lpActor->m_bf4InstanceFieldFloor=	m_iUniqueSerial%dINSTANCE_FIELD_FLAG;
	}

	return	lpActor;
}

//
//	분신-_-? 없애기
void
cFIELD::checkConnectedPlayer(char *_lpstrID,BOOL *_lpbIsCannotDisconnect)
{
	*_lpbIsCannotDisconnect	=	FALSE;

	cACTOR	*lpActor		=	g_game.getPlayerByID(_lpstrID);

	if	(!lpActor) 
		return;

	if	(lpActor->isCanNotCloseGameStatus())
	{
		*_lpbIsCannotDisconnect	=	TRUE;

		return;
	}

	CClient	*lpClient	=	lpActor->getClient();

	if	(lpClient && lpActor->isCorrectClient(lpClient))
	{
		DisUser(lpActor->m_iClientSerial,"cFIELD::checkConnectedPlayer");
		CLOG("remove Valid Player","cFIELD::checkConnectedPlayer - 똑같은 계정이 있어서 제거한다. [DisUser] %s",_lpstrID);
	}
	else
	{
		lpActor->bookingReadyToExitGame();
		CLOG("remove Valid Player","cFIELD::checkConnectedPlayer - 똑같은 계정이 있어서 제거한다. [bookingExitPlayer] %s",_lpstrID);
	}
}

//
//	플레이어참가
cACTOR*
cFIELD::joinPlayer(CPlayerData *_lpData,int _iClientSerial,BOOL _isFirst,cActorStatusUpkeep *_lpUpkeepData)
{
	BOOL			bIsCannotDisconnect,bIsReconnectGuildBattleField=FALSE;
	SG_RESULT_JOIN	sJoinResult;
	cACTOR			*lpActor;
	int				iTeam;
	int				iOperatorLevel	=	0;

	if	(m_aiActorCount[0]	>=	m_iLimitPlayerCount)
	{
		sJoinResult.wResult	=	dRESULT_JOIN_FIELD_FULL;

		goto	ERROR_LABEL;
	}

	if	(_lpData->m_wJob	>=	dPLAYER_JOB_COUNT)
	{
		sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;

		CLOG("joinPlayer","[%s]error in cFIELD::joinPlayer 직업이 이상해 %s-%s",m_strFileName,_lpData->m_strId,_lpData->m_strName);

		goto	ERROR_LABEL;
	}

	if	(!g_bIsInnerTestServer)
	{
		if	(g_aPlayer1LvSetting[_lpData->m_wJob].m_wStatus	==	0)
		{
			sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;
			printf("is banished by block job\n");
			goto	ERROR_LABEL;
		}

		if	(g_aPlayer1LvSetting[_lpData->m_wJob].m_wStatus	==	2)
		{
			if	(!g_bIsTestServer)
			{
				sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;

				printf("is banished by block job\n");

				goto	ERROR_LABEL;
			}
		}
	}

	checkConnectedPlayer(_lpData->m_strId,&bIsCannotDisconnect);

	if	(bIsCannotDisconnect)
	{
		sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED_BY_EXIST_PLAYER;

		goto	ERROR_LABEL;
	}

	if	(_lpData->m_bf1IsTeleportToCouple)
	{
		_lpData->m_bf1IsTeleportToCouple	=	FALSE;
		_lpData->m_iXPos=	-1;
		_lpData->m_iYPos=	-1;

		cItem	*lpItem	=	((cACTOR *)_lpData)->getInventoryItemInData(_lpData->m_bf7UseCoupleRingSlot);

		if	(lpItem)
		{
			cCoupleRingDefine	*lpCoupleRing	=	lpItem->getCoupleRing();

			if	(lpCoupleRing)
			{
				cACTOR	*lpCouple	=	getPlayerByName(lpCoupleRing->getCoupleName());

				if	(lpCouple)
				{
					_lpData->m_iXPos=	lpCouple->m_pos.x;
					_lpData->m_iYPos=	lpCouple->m_pos.y;
				}
			}
		}
	}

	if	(m_bIsSiegeWarfareField)
	{
		if	(_lpData->m_bf5GuildBattleDay	!=	m_iSiegeWarfareValue)
		{
			_lpData->m_bf5GuildBattleDay	=	m_iSiegeWarfareValue;
			_lpData->m_iXPos				=	-1;
			_lpData->m_iYPos				=	0;
		}
	}
//	캐릭터 생성하고 처음으로 로그인 한거다. 이 맵의 시작 포인트로 보내자.
	if	(_lpData->m_iXPos	==	-1)
	{
		if	(_lpData->m_iYPos	==	-1 || _lpData->m_iYPos	==	0xffff)
			return	startPlayer(_lpData,_iClientSerial,_lpUpkeepData);
		else
		{
			cAreaInfo	*lpArea;
			
			if	(m_bIsGuildBattleField	||	m_wDuelType != 0xffff)
			{
				int	iTeam	=	getTeamInGuildBattleField(_lpData->m_wGuildSerial,_lpData->m_strName);

				if	(iTeam	==	0xffff)
					iTeam	=	0;

				if	(m_bIsSiegeWarfareField)
				{
					if	(iTeam	==	0)
						lpArea	=	m_pArea->getGuildBattleArea(0);
					else
						lpArea	=	m_pArea->getStartArea();
				}
				else
					lpArea	=	m_pArea->getGuildBattleArea(iTeam);
			}
			else
			{
				lpArea	=	m_pArea->getPortal(_lpData->m_iYPos);
			}

			if	(lpArea		==	NULL)
			{
				lpArea		=	m_pArea->getFirstPortal();

				if	(lpArea	==	NULL)
				{
					CLOG("joinPlayer","cFIELD::joinPlayer - 입구를 못 찾겠어 -o-");
					sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;

					goto	ERROR_LABEL;
				}
			}

			cPOINT	pos;

			pos.x	=	_lpData->m_iXPos;
			pos.y	=	_lpData->m_iYPos;

			lpArea->getCenterPos(&pos);
		
			if	(isBlocked(pos.x/dTILE_XS,pos.y/dTILE_YS))
				findEnterancePos(&pos,lpArea);

			_lpData->m_iXPos	=	pos.x;
			_lpData->m_iYPos	=	pos.y;
		}
	}
	
	if	(_lpData->m_iXPos<0 || _lpData->m_iYPos<0 || _lpData->m_iXPos>=m_iPixelWidth || _lpData->m_iYPos>=m_iPixelHeight) // 뭔가에 의해 좌표가 엉망이 되었다.
	{
		cAreaInfo	*lpArea;

		if	(_lpData->m_iXPos	==	-2)	//	길드 홀 입구로 이동
			lpArea	=	m_pArea->getGuildHallGate();
		else
		if	(m_bIsGuildBattleField == FALSE)
			lpArea	=	m_pArea->getFirstPortal();
		else
			lpArea	=	m_pArea->getGuildBattleArea(0);

		if	(!lpArea)
			lpArea	=	m_pArea->getFirstPortal();

		if	(!lpArea)
		{
			ERRMSG("joinPlayer","입구 없음\n\n%s[%d]",m_strFileName,m_iUniqueSerial);
			sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;

			goto	ERROR_LABEL;
		}

		cPOINT	pos;

		pos.x	=	_lpData->m_iXPos;
		pos.y	=	_lpData->m_iYPos;

		lpArea->getCenterPos(&pos);

		_lpData->m_iXPos	=	pos.x;
		_lpData->m_iYPos	=	pos.y;
	}

	if(_lpData->m_bf1IsTeleportToPointAround)
	{
		cPOINT	pos = {_lpData->m_iXPos,_lpData->m_iYPos};

		findEnterancePosToPointAroundTile(&pos,1);
		_lpData->m_iXPos	=	pos.x;
		_lpData->m_iYPos	=	pos.y;

		_lpData->m_bf1IsTeleportToPointAround	=	FALSE;
	}

	if	(isBlocked(_lpData->m_iXPos/dTILE_XS,_lpData->m_iYPos/dTILE_YS))
	{
		cAreaInfo	*lpArea;

		if	(m_bIsGuildBattleField == FALSE)
			lpArea	=	m_pArea->getFirstPortal();
		else
			lpArea	=	m_pArea->getGuildBattleArea(0);

		if	(!lpArea)
		{
			ERRMSG("joinPlayer","입구 없음\n\n%s[%d]",m_strFileName,m_iUniqueSerial);
			sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;

			goto	ERROR_LABEL;
		}

		cPOINT	pos;

		pos.x	=	_lpData->m_iXPos;
		pos.y	=	_lpData->m_iYPos;
		lpArea->getCenterPos(&pos);
		_lpData->m_iXPos	=	pos.x;
		_lpData->m_iYPos	=	pos.y;
	}

	{
		CClient	*lpClient	=	CM.mGETCLIENT(_iClientSerial);
		
		if	(lpClient)
			iOperatorLevel	=	lpClient->GetIsOper();
	}

	if	(m_wDuelType	!=	0xffff	&&	iOperatorLevel	>=	eAL_BOSS)
		iTeam	=	0;
	else
		iTeam	=	getTeam(_lpData->m_wGuildSerial,eAK_PLAYER,_lpData->m_strName);

	if	(iTeam	==	-1)
	{
		sJoinResult.wResult	=	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;

		goto	ERROR_LABEL;
	}

	lpActor		=	addActor(iTeam,eAK_PLAYER,_lpData->m_iXPos,_lpData->m_iYPos);

//	게임 참가 실패
	if	(!lpActor)
	{
		sJoinResult.wResult	=	dRESULT_JOIN_FIELD_FULL;

		goto	ERROR_LABEL;
	}

	if	(m_bIsGuildBattleField == FALSE)
	{
		lpActor->setDataByJoinPlayerData(_lpData,_iClientSerial,_lpUpkeepData);
		lpActor->m_bf1IsPlayOnGuildBattle	=	FALSE;
	}
	else
		lpActor->setDataByJoinPlayerData(_lpData,_iClientSerial,NULL);

	if	(lpActor->m_dwPlayTime	==	0)
	{
		lpActor->m_iHP	=	lpActor->getMaxHP();
		_lpData->m_iHP	=	lpActor->getMaxHP();
	}

	lpActor->m_wReceiveFieldActorData	=	0;
	lpActor->m_wReceiveFieldItemData	=	0;
	lpActor->m_iRookieTime				=	1;

//	필드에 다른 플레이들 한테, 언넘이 들어 왔다고 전해 준다.
	{
		SG_JOIN_PLAYER	sJoinPlayer;

		sJoinPlayer.base.set(sizeof(SG_JOIN_PLAYER),dSG_JOIN_PLAYER);

		lpActor->getSimplePlayerInfo(&sJoinPlayer.actorInfo);

		sJoinPlayer.bf1IsByDeath			=		lpActor->m_bf1IsMoveFieldByDeath;
		sJoinPlayer.bf1IsByTeleport			=		lpActor->m_bf1IsMoveFieldByTeleport;

		lpActor->m_bf1IsMoveFieldByDeath	=	FALSE;
		lpActor->m_bf1IsMoveFieldByTeleport	=	FALSE;

		if	(_lpData->m_wJob	>=	dPLAYER_JOB_COUNT)
		{
			CLOG("joinPlayer","[%s]error in cFIELD::joinPlayer SG_JOIN_PLAYER 직업이 이상해 %s-%s",m_strFileName,_lpData->m_strId,_lpData->m_strName);
			sJoinResult.wResult	= dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA;

			goto	ERROR_LABEL;
		}

		sJoinPlayer.base.wSize	=	sizeof(sJoinPlayer)-sizeof(sJoinPlayer.actorInfo.strName)+strlen(sJoinPlayer.actorInfo.strName)+1;

		addSendPacket(&sJoinPlayer,lpActor->m_wSerialInField);
	}

	lpActor->m_wIsIncorrectGuildBattleFieldJoin		=	bIsReconnectGuildBattleField;

	return	lpActor;

ERROR_LABEL:

	CClient	*lpClient	=	CM.mGETCLIENT(_iClientSerial);

	sJoinResult.base.set(sizeof(cMSG_BASE_TYPE)+2,dSG_RESULT_JOIN);

	CM.SEND_PACKET2(lpClient,&sJoinResult,sJoinResult.base.wSize);

	return	NULL;	//	처리 -_-a
}

//
//	시작 영역에 플래이어를 추가한다.
cACTOR*
cFIELD::startPlayer(CPlayerData *_lpData,int _iClientSerial,cActorStatusUpkeep *_lpUpkeepData)
{
	cAreaInfo	*lpStartArea	=	m_pArea->getStartArea();
	cPOINT		pos;

	if	(!lpStartArea)
	{
		lpStartArea	=	m_pArea->getFirstPortal();

		if	(!lpStartArea)
		{
			lpStartArea	=	m_pArea->getGuildBattleArea(0);

			if	(!lpStartArea)
			{
				ERRMSG("error in add Player!!","not exist start point");

				return	NULL;
			}
		}
	}

	lpStartArea->getCenterPos(&pos);

	if	(!findEnterancePos(&pos,lpStartArea))
	{
		ERRMSG("error in add Player!!","incorrect field start point");

		return	NULL;
	}

	if	(_lpData->m_dwPlayTime	==	0	&&	m_iUniqueSerial	==	0)
	{
		pos.x	=	31*64;
		pos.y	=	63*32;
	}

	_lpData->m_iXPos	=	pos.x;
	_lpData->m_iYPos	=	pos.y;

	return	joinPlayer(_lpData,_iClientSerial,TRUE,_lpUpkeepData);
}

int
cFIELD::getLinkAreaMonsterCount(int _iArea)
{
	int			iMonsterCount	=	0;
	cAreaInfo	*lpArea			=	m_pArea->getArea(_iArea);

	if (!lpArea)
		return	0;

	for (int i=0,iCount=0;i<dMAX_NPC_COUNT && iCount<m_iSavedActorCount;i++)
	{
		cNPC	*lpMonster	=	m_apNpcGenerateData[i];

		if	(lpMonster	==	NULL)
			continue;

		iCount++;

		if	(lpMonster->m_wActorKind	!=	eAK_MONSTER	||	lpMonster->m_wMoveArea	!=	_iArea)
			continue;

		cACTOR	*lpActor	=	getActor(lpMonster->m_wActorSerial);

		if	(!lpActor	||	lpActor->isDeath())
			continue;

		iMonsterCount++;
	}

	return	iMonsterCount;
}

//
//	몬스터 리젠
cACTOR*
cFIELD::generateNpc(cNPC *_lpNpc,BOOL _isFirst,BOOL _bIsByKarma,int _iX,int _iY)
{
	int			iRegenNpc		=	_lpNpc->m_wCharacter;
	BOOL		bIsAwakeOrMirror=	_lpNpc->m_bIsSleep;
	cCHARACTER	*lpCharacter	=	&m_aCharacter[iRegenNpc];

	if	(lpCharacter->m_bf1IsKeyMonster)
	{
		_lpNpc->m_iLastTime				=	0;
		_lpNpc->m_iRegenerationCycle	=	0;

		if	(_isFirst || _bIsByKarma)
			return	NULL;

		if	(getLinkAreaMonsterCount(_lpNpc->m_wMoveArea))
			return	NULL;

		lpCharacter->m_bf1IsKeyMonster	=	FALSE;
	}
	else
	{
		if	(_bIsByKarma	==	FALSE	&&	m_wIsRegenMonster	==	FALSE)
			return	NULL;
	}

	cPOINT		posGenerate;

	posGenerate.x	=	_lpNpc->m_pos.x;
	posGenerate.y	=	_lpNpc->m_pos.y;

	if	(_iX)
	{
		posGenerate.x	=	_iX;
		posGenerate.y	=	_iY;
		bIsAwakeOrMirror=	TRUE;
	}
	else
	if	(_lpNpc->m_bIsSleep	||	_lpNpc->m_bWakeUpNow)
	{
		_lpNpc->m_bIsSleep	=	FALSE;

		posGenerate.x		=	_lpNpc->m_posSleep.x;
		posGenerate.y		=	_lpNpc->m_posSleep.y;
	}
	else				//JBC		몬스터 젠자리를 구한다.		08-09-19
	if	(_lpNpc->m_wGenerateMethod	==	dMONSTER_GENERATE_ACTIVE_AREA)
	{
		cAreaInfo	*lpArea	=	m_pArea->getArea(_lpNpc->m_wMoveArea);

		if (lpArea)
		{
			if (!findEnterancePos(&posGenerate,lpArea))
			{
				posGenerate.x	=	_lpNpc->m_pos.x;
				posGenerate.y	=	_lpNpc->m_pos.y;
			}
		}
	}
	else if(_lpNpc->m_wGenerateMethod	==	dMONSTER_GENERATE_AROUND)
	{

		cRECT	rect;
		rect.x1		= min(_lpNpc->m_wGenerateAroundPosX1,_lpNpc->m_wGenerateAroundPosX2) * dTILE_XS;
		rect.y1		= min(_lpNpc->m_wGenerateAroundPosY1,_lpNpc->m_wGenerateAroundPosY2) * dTILE_YS;
		rect.x2		= max(_lpNpc->m_wGenerateAroundPosX1,_lpNpc->m_wGenerateAroundPosX2) * dTILE_XS;
		rect.y2		= max(_lpNpc->m_wGenerateAroundPosY1,_lpNpc->m_wGenerateAroundPosY2) * dTILE_YS;

		if (!findEnterancePos(&posGenerate,rect))
		{
			posGenerate.x	=	_lpNpc->m_pos.x;
			posGenerate.y	=	_lpNpc->m_pos.y;
		}
	}
	if	(_lpNpc->m_bWakeUpNow)
	{
		bIsAwakeOrMirror	=	TRUE;
		_lpNpc->m_bWakeUpNow=	FALSE;
	}

	if	(m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG && _bIsByKarma == FALSE)
		if (_lpNpc->m_wActorKind	==	eAK_MONSTER	&&	m_bIsGuildBattleField	==	FALSE	&&	m_wIsGuildHall == FALSE)
		{
			int	iPart	=	getCurrentLocalPart(posGenerate.x,posGenerate.y);

			if	(!checkLocalPartPlayerForMonster(iPart))
				return	NULL;
		}

	cACTOR		*lpActor	=	addActor(getTeam(0xffff,_lpNpc->m_wActorKind,_lpNpc->m_strName),_lpNpc->m_wActorKind,posGenerate.x,posGenerate.y);

	if	(!lpActor)
		return	NULL;

	_lpNpc->m_wActorSerial	=	lpActor->m_wSerialInField;

	lpActor->setDataByCharacter(lpCharacter,_lpNpc->m_wActorKind,lpCharacter->getGenerateLevel(),posGenerate.x,posGenerate.y,this);

	lpActor->m_bIsGuildCrest	=	_lpNpc->m_bf1IsGuildCrest;
	lpActor->m_bIsGuildObject	=	_lpNpc->m_bf1IsGuildObject;

	if	(lpActor->m_bIsGuildCrest)
		lpActor->m_bIsGuildObject	=	TRUE;

	if	(lpActor->m_bIsGuildObject)
	{
		if	(lpActor->m_bIsGuildCrest)
		{
			lpActor->m_maxHP.m_iValue	=	c_iGuildCrestHP;
#ifdef	_FOR_JAPAN			
			lpActor->m_maxHP.m_iValue	=	1000000;
#endif
		}
		else
		{
			lpActor->m_maxHP.m_iValue	=	c_iGuildObjectHP;
		}
		lpActor->m_iHP				=	lpActor->m_maxHP.m_iValue;
	}

	strcpy(lpActor->m_strName,_lpNpc->m_strName);
	strcpy(lpActor->m_strId,"NPC");

	lpActor->m_wDirect				=	_lpNpc->m_wDirect;
	lpActor->m_wNpcActorSaveSerial					=	_lpNpc->m_iSerial;
	lpActor->m_lpCharacter			=	lpCharacter;
	lpActor->m_dwLastUpdateTime		=	m_iFrameCounter-random(_lpNpc->m_wWanderCycle*dSYNC_FPS+1);
	lpActor->m_wBattleInclination	=	_lpNpc->m_wBattleInclination;
	lpActor->m_wOrgBattleInclination=	lpActor->m_wBattleInclination;

	if	(m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		lpActor->m_wCurrentField	=	m_iUniqueSerial;
	else
	{
		lpActor->m_wCurrentField		=	m_wGateField+dINSTANCE_FIELD_FLAG;
		lpActor->m_bf8InstanceField		=	m_iUniqueSerial/dINSTANCE_FIELD_FLAG;
		lpActor->m_bf4InstanceFieldFloor=	m_iUniqueSerial%dINSTANCE_FIELD_FLAG;
	}

	if	(lpActor->m_wDirect	==	dDIRECT_RANDOM)
		lpActor->m_wDirect	=	random(8);

	_lpNpc->m_wBrithCount++;

	if	(m_bIsGuildBattleField	==	FALSE)
	{
		if	(lpActor->isPureMonster())
			g_project.m_kcMonsterGen.operate(lpActor,lpActor,this);
	}

	if	(_isFirst)
		return	lpActor;

	SG_REGEN_MONSTER	packetRegenMonster;

	packetRegenMonster.base.set(sizeof(packetRegenMonster),dSG_REGEN_MONSTER);

	mDS.m_aiExValue[0]	=	8;
	lpActor->updateEnchantedMagic();
	lpActor->getSimpleMonsterInfo(&packetRegenMonster.monsterInfo);
	
	packetRegenMonster.monsterInfo.bf1IsRegen	=	1-bIsAwakeOrMirror;
	if	(lpActor->isNpc())
		packetRegenMonster.monsterInfo.bf1IsRegen=	FALSE;

	packetRegenMonster.base.wSize	=	sizeof(packetRegenMonster)-sizeof(packetRegenMonster.monsterInfo.strName)+strlen(packetRegenMonster.monsterInfo.strName)+1;

	addSendPacket((ALL_MSG*)&packetRegenMonster,lpActor->m_wSerialInField);

	if	(lpActor->isNpc())
		lpActor->addSendSimpleInfo();

	return	lpActor;
}

//
//	몬스터 리젠
cACTOR*
cFIELD::generateMonsterByCharacter(cCHARACTER *_lpCharacter,int _iLevel,int _iX,int _iY)
{
	cPOINT		posGenerate;

	posGenerate.x	=	_iX;
	posGenerate.y	=	_iY;

	if	(m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		if (m_bIsGuildBattleField	==	FALSE)
		{
			int	iPart	=	getCurrentLocalPart(posGenerate.x,posGenerate.y);

			if	(!checkLocalPartPlayerForMonster(iPart))
				return	NULL;
		}

	cACTOR		*lpActor	=	addActor(getTeam(0xffff,eAK_MONSTER,_lpCharacter->m_strName),eAK_MONSTER,posGenerate.x,posGenerate.y);

	if	(!lpActor)
		return	NULL;

	lpActor->setDataByCharacter(_lpCharacter,eAK_MONSTER,_iLevel,posGenerate.x,posGenerate.y);

	strncpy(lpActor->m_strName,_lpCharacter->m_strName,17);
	strcpy(lpActor->m_strId,"NPC");

	lpActor->m_dwLastUpdateTime		=	m_iFrameCounter;
	lpActor->m_wBattleInclination	=	dBI_PASSIVE_SOLO;
	lpActor->m_wOrgBattleInclination=	lpActor->m_wBattleInclination;

	if	(m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		lpActor->m_wCurrentField	=	m_iUniqueSerial;
	else
	{
		lpActor->m_wCurrentField		=	m_wGateField+dINSTANCE_FIELD_FLAG;
		lpActor->m_bf8InstanceField		=	m_iUniqueSerial/dINSTANCE_FIELD_FLAG;
		lpActor->m_bf4InstanceFieldFloor=	m_iUniqueSerial%dINSTANCE_FIELD_FLAG;
	}

	if	(lpActor->m_wDirect	>	8)
		lpActor->m_wDirect	=	random(8);

	SG_REGEN_MONSTER	packetRegenMonster;

	packetRegenMonster.base.set(sizeof(packetRegenMonster),dSG_REGEN_MONSTER);

	mDS.m_aiExValue[0]	=	9;
	lpActor->updateEnchantedMagic();
	lpActor->getSimpleMonsterInfo(&packetRegenMonster.monsterInfo);
	
	packetRegenMonster.monsterInfo.bf1IsRegen	=	TRUE;

	if	(lpActor->isNpc())
		packetRegenMonster.monsterInfo.bf1IsRegen=	FALSE;

	packetRegenMonster.base.wSize	=	sizeof(packetRegenMonster)-sizeof(packetRegenMonster.monsterInfo.strName)+strlen(packetRegenMonster.monsterInfo.strName)+1;

	addSendPacket((ALL_MSG*)&packetRegenMonster,lpActor->m_wSerialInField);

	if	(lpActor->isNpc())
		lpActor->addSendSimpleInfo();

	return	lpActor;
}

//
//	펫 생성
BOOL
cFIELD::generatePet(cACTOR *_lpTamer,int _iPetIndex,BOOL _bIsUnseal)
{
	if	(isEventField())
		return	FALSE;

	cPetDefine *lpPetInfo	=	&_lpTamer->m_aPet[_iPetIndex];

	cPOINT	posGenerate;
	BOOL	bIsDeath	=	FALSE;

	if	(lpPetInfo->m_bf22CurrentHP	==	0)
		bIsDeath	=	TRUE;

	int	iJobIndex	=	lpPetInfo->getJobIndex();

	if	(iJobIndex	>=	dMAX_JOB_COUNT	||	g_aJob[iJobIndex].m_iSerial	==	0xffff)
	{
		CLOG("___incorrectPetData","[%s/%s] pet data failed",_lpTamer->m_strId,_lpTamer->m_strName);
		return	FALSE;
	}

	_lpTamer->getRandomNearPlace(&posGenerate,g_aBody[g_aJob[iJobIndex].m_wBody].m_iCrashSize*2/3);

	cACTOR		*lpActor	=	addActor(_lpTamer->m_wTeam,eAK_MONSTER,posGenerate.x,posGenerate.y);

	if	(!lpActor)
		return	FALSE;

	lpActor->buildByPetInfo(_lpTamer,lpPetInfo,_iPetIndex);
	lpActor->setActorPos(posGenerate.x,posGenerate.y,TRUE);
	_lpTamer->m_awTamedMonsterList[_iPetIndex]	=	lpActor->m_wSerialInField;

	strcpy(lpActor->m_strId,"pet");

	lpActor->m_dwLastUpdateTime		=	m_iFrameCounter;

	if	(m_iUniqueSerial			<	dINSTANCE_FIELD_FLAG)
		lpActor->m_wCurrentField	=	m_iUniqueSerial;
	else
	{
		lpActor->m_wCurrentField		=	m_wGateField+dINSTANCE_FIELD_FLAG;
		lpActor->m_bf8InstanceField		=	m_iUniqueSerial/dINSTANCE_FIELD_FLAG;
		lpActor->m_bf4InstanceFieldFloor=	m_iUniqueSerial%dINSTANCE_FIELD_FLAG;
	}

	lpActor->m_wDirect	=	random(8);

	SG_REGEN_MONSTER	packetRegenMonster;

	packetRegenMonster.base.set(sizeof(packetRegenMonster),dSG_REGEN_MONSTER);

	lpActor->getSimpleMonsterInfo(&packetRegenMonster.monsterInfo);
	packetRegenMonster.monsterInfo.bf1IsRegen	=	_bIsUnseal;
	packetRegenMonster.base.wSize				=	sizeof(packetRegenMonster)-sizeof(packetRegenMonster.monsterInfo.strName)+strlen(packetRegenMonster.monsterInfo.strName)+1;

	addSendPacket((ALL_MSG*)&packetRegenMonster,lpActor->m_wSerialInField);

	if (bIsDeath)
	{
		lpActor->m_iHP			=	0;
		lpActor->m_iDeathCount	=	dDEFAULT_DEATH_COUNT;	//	죽었다고 표시
	}

	_lpTamer->sendPetInfo(_iPetIndex,lpActor->m_wSerialInField,FALSE);

	return	TRUE;
}

BOOL
cFIELD::generateSummonBeast(cACTOR *_lpSummoner,int _iLevel,int _iRemainHP,int _iType,int _iGrade,int _iX,int _iY,BOOL _bIsSummon)
{
	if	(isEventField())
		return	FALSE;

	if	(_bIsSummon && _lpSummoner->getSummonBeastCount()	>=	dMAX_SUMMON_BEAST_COUNT)
		return	FALSE;

	cPOINT	posGenerate;

	posGenerate.Set(_iX,_iY);

	if	(_bIsSummon	==	FALSE)
		_lpSummoner->getRandomNearPlace(&posGenerate,100);

	if	(isBlocked(posGenerate.x/dTILE_XS,posGenerate.y/dTILE_YS))
		posGenerate.Set(_lpSummoner->m_pos.x,_lpSummoner->m_pos.y);

	cACTOR		*lpActor	=	addActor(_lpSummoner->m_wTeam,eAK_MONSTER,posGenerate.x,posGenerate.y);

	if	(!lpActor)
		return	FALSE;

	int	iSummonBeastIndex	=	_lpSummoner->addSummonBeast(lpActor);
	
	if	(iSummonBeastIndex	==	0xffff)
		return	FALSE;

	strcpy(lpActor->m_strId,"sb");

	lpActor->m_wPetIndex			=	iSummonBeastIndex;
	lpActor->m_dwLastUpdateTime		=	m_iFrameCounter;

	lpActor->buildSummonBeast(_iLevel,_iRemainHP,_iType,_iGrade,_lpSummoner->m_wSerialInField,_lpSummoner->m_dwNameHashCode);
	lpActor->setActorPos(posGenerate.x,posGenerate.y,TRUE);

	if	(m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		lpActor->m_wCurrentField	=	m_iUniqueSerial;
	else
	{
		lpActor->m_wCurrentField		=	m_wGateField+dINSTANCE_FIELD_FLAG;
		lpActor->m_bf8InstanceField		=	m_iUniqueSerial/dINSTANCE_FIELD_FLAG;
		lpActor->m_bf4InstanceFieldFloor=	m_iUniqueSerial%dINSTANCE_FIELD_FLAG;
	}

	lpActor->m_wDirect	=	random(8);

	SG_REGEN_MONSTER	packetRegenMonster;

	packetRegenMonster.base.set(sizeof(packetRegenMonster),dSG_REGEN_MONSTER);

	lpActor->getSimpleMonsterInfo(&packetRegenMonster.monsterInfo);
	packetRegenMonster.base.wSize				=	sizeof(packetRegenMonster)-sizeof(packetRegenMonster.monsterInfo.strName)+strlen(packetRegenMonster.monsterInfo.strName)+1;
	packetRegenMonster.monsterInfo.bf1IsRegen	=	_bIsSummon;

	addSendPacket((ALL_MSG*)&packetRegenMonster,lpActor->m_wSerialInField);

	_lpSummoner->sendSummonBeastInfo(iSummonBeastIndex,FALSE);
	lpActor->syncTamedMonsterHP(TRUE);

	return	TRUE;
}

//
//	모든 몬스터 제거
void
cFIELD::killTeam(int _iTeam)
{
	for (int i=0;i<m_aiActorCount[_iTeam];i++)
		addBookedRemoveActor(m_awActorList[_iTeam][i]);
}

void
cFIELD::linkMonsterInSameArea()
{
	int	iArea,i,j,iLinkMonsterCount=0,iCount;
	int	aiMonsterList[dMAX_LINK_MONSTER_COUNT];

	for (iArea=2;iArea<dMAX_AREA;iArea++)
	{
		if	(m_pArea->m_pArea[iArea]	==	NULL	||	m_pArea->m_pArea[iArea]->m_wSerial	==	0xffff)
			continue;

		iLinkMonsterCount	=	0;

		for (i=0,iCount=0;i<dMAX_NPC_COUNT && iCount<m_iSavedActorCount;i++)
		{
			if	(m_apNpcGenerateData[i]	==	NULL)
				continue;

			iCount++;

			if	(m_apNpcGenerateData[i]->m_wActorKind	!=	eAK_MONSTER)
				continue;

			if	(m_apNpcGenerateData[i]->m_wMoveArea	==	iArea)
			{
				aiMonsterList[iLinkMonsterCount++]	=	i;

				if	(iLinkMonsterCount	>=	dMAX_LINK_MONSTER_COUNT)
					break;
			}
		}

		for (i=0;i<iLinkMonsterCount;i++)
		{
			int		iNpc				=	aiMonsterList[i];
			cNPC	*lpNpc				=	m_apNpcGenerateData[iNpc];

			lpNpc->m_wLinkMonsterCount	=	0;

			for (j=0;j<iLinkMonsterCount;j++)
			{
				if	(iNpc==aiMonsterList[j])
					continue;

				lpNpc->m_awLinkMonster[lpNpc->m_wLinkMonsterCount++]	=	aiMonsterList[j];
			}
		}
	}
}

//
//	모든 NPC를 생성합니다.
void
cFIELD::generateAllNPC()
{
	int	i,iCount;
	int	iTime	=	g_dwCurrentTime;
	
	for (i=0,iCount=0;i<dMAX_NPC_COUNT && iCount<m_iSavedActorCount;i++)
	{
		if	(m_apNpcGenerateData[i]	==	NULL	||	m_apNpcGenerateData[i]->m_wIsBlockToAutoRegen)
			continue;

		if	(m_apNpcGenerateData[i]->m_wActorKind	==	eAK_NPC || m_apNpcGenerateData[i]->m_wActorKind > eAK_MONSTER || m_bIsGuildBattleField	||	m_wIsGuildHall)
		{
			if	(generateNpc(m_apNpcGenerateData[i],TRUE)	==	NULL)
				ERRMSG("too many character count","please edit 'server_config.txt' file's  max_actor_count value");
		}
		else
		if	(m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
			generateNpc(m_apNpcGenerateData[i],TRUE,FALSE);

		iCount++;
	}
}

//
//	모든 mob을 리젠한다.
void
cFIELD::regenAllMob()
{
	int	i,iCount;
	int	iTime	=	g_dwCurrentTime;

	for (i=0,iCount=0;i<dMAX_NPC_COUNT && iCount<m_iSavedActorCount;i++)
	{
		if (m_apNpcGenerateData[i]	==	NULL)
			continue;

		if (m_apNpcGenerateData[i]->m_wActorKind	!=	eAK_MONSTER)
			continue;

		if (m_apNpcGenerateData[i]->m_wActorSerial	!=	0xffff)
			continue;

		if	(m_apNpcGenerateData[i]->m_wIsBlockToAutoRegen)
			continue;

		generateNpc(m_apNpcGenerateData[i],FALSE);

		iCount++;
	}
}

void
cFIELD::regenMonsterInArea(int _iArea,int _iCharacter)
{
	int		i,iCount;
	BOOL	bIsTiamath	=	FALSE;
	cPOINT	posTiamath;

	for (i=0,iCount=0;i<dMAX_NPC_COUNT && iCount<m_iSavedActorCount;i++)
	{
		cNPC	*lpNpc	=	m_apNpcGenerateData[i];

		if	(lpNpc	==	NULL)
			continue;

		if	(lpNpc->m_wActorKind	!=	eAK_MONSTER)
			continue;

		if	(lpNpc->m_wActorSerial	!=	0xffff)
			continue;

		if	(lpNpc->m_wMoveArea		!=	_iArea)
			continue;

		if	(_iCharacter	!=	dCHARACTER_ALL)
			if	(lpNpc->m_wCharacter!=	_iCharacter)
				continue;

		generateNpc(lpNpc,FALSE,TRUE);

		if	(g_aJob[m_aCharacter[lpNpc->m_wCharacter].m_wBaseJob].m_wBody	==	dJOB_MONSTER_TIAMATH)	//	티아메쓰 일 경우,좌/우 촉수를 강제로 찍어 준다.
		{
			bIsTiamath		=	TRUE;
			posTiamath.x	=	lpNpc->m_pos.x;
			posTiamath.y	=	lpNpc->m_pos.y;
		}
	}

	if	(bIsTiamath		==	TRUE)
		for (i=0,iCount=0;i<dMAX_NPC_COUNT && iCount<m_iSavedActorCount;i++)
		{
			cNPC	*lpNpc	=	m_apNpcGenerateData[i];

			if	(lpNpc	==	NULL)
				continue;

			if	(g_aJob[m_aCharacter[lpNpc->m_wCharacter].m_wBaseJob].m_wBody	==	dJOB_MONSTER_TIAMATH_FEELER)	//	티아메쓰 일 경우,좌/우 촉수를 강제로 찍어 준다.
			{
				lpNpc->m_pos.x	=	posTiamath.x;
				lpNpc->m_pos.y	=	posTiamath.y+1;

				generateNpc(lpNpc,FALSE,TRUE);
			}

			if	(g_aJob[m_aCharacter[lpNpc->m_wCharacter].m_wBaseJob].m_wBody	==	dJOB_MONSTER_TIAMATH_BOTTOM_FEELER)	//	티아메쓰 일 경우,좌/우 촉수를 강제로 찍어 준다.
			{
				lpNpc->m_pos.x	=	posTiamath.x;
				lpNpc->m_pos.y	=	posTiamath.y-1;

				generateNpc(lpNpc,FALSE,TRUE);
			}
		}
}

//
//NPC 생성 타임!!
void
cFIELD::operateRegenNpc()
{
	int		i,iCount,iRegenCount=0;
 
	if	(m_bIsGuildBattleField == FALSE && m_aiActorCount[0] == 0)
		return;
	
	for (i=0,iCount=0;i<dMAX_NPC_COUNT && iCount<m_iSavedActorCount && iRegenCount < dMAX_REGEN_COUNT_BY_ONE_TIME;i++)
	{
		cNPC	*lpNpc	=	m_apNpcGenerateData[i];

		if	(lpNpc == NULL || lpNpc->m_iSerial	==	0xffffffff)
			continue;

		if	(getActor(lpNpc->m_wActorSerial)	==	NULL)
			lpNpc->m_wActorSerial	=	0xffff;

		if	(lpNpc->isGenerateTime(g_iCounterBySecond))
			if	(generateNpc(lpNpc))
				iRegenCount++;

		iCount++;
	}
}

//
//	액터 리스트 재 설정
void
cFIELD::rebuildActorList()
{
	int	i;
	int	aiActorCount[dTEAM_COUNT];

	memcpy(aiActorCount,m_aiActorCount,sizeof(aiActorCount));

	m_aiActorCount[0]	=	0;	//	플레이어의 수
	m_aiActorCount[1]	=	0;	//	몬스터의 수
	m_aiActorCount[2]	=	0;	//	NPC의 수
	m_iActorCount		=	0;

	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR	*lpActor	=	getTestedActor(i);

		if (lpActor	==	NULL)
			continue;

		m_awActorList[lpActor->m_wTeam][m_aiActorCount[lpActor->m_wTeam]++]	=	i;

		m_iActorCount++;
	}

	if (aiActorCount[0]!=m_aiActorCount[0] || aiActorCount[1]!=m_aiActorCount[1] || aiActorCount[2]!=m_aiActorCount[2] )
	{
		int	iValidActorInfoCount	=	removeNpcValidBeaterInfo();

		CLOG("joinPlayer","error in cFIELD::rebuildActorList() - 이런 경우가 있네.. -_-;; %d명",iValidActorInfoCount);

		if (iValidActorInfoCount)
		{
			//	어쩌구 저쩌구... 
		}
	}
}