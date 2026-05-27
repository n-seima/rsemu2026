#include "cFIELD.H"
#include "CGame.H"
#include "Client.H"
#include "debugCode.H"
#include "CDataSave.H"
#include	"secretDungeon.H"

//
//	죽기로 예약된넘 처리
void
cFIELD::operateBookedDeathActor(BW_DEATH_ACTOR *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor,*lpKiller	=	NULL;

	lpActor		=	getTestedActor(_lpPacket->wSerial);

	if	(!lpActor || lpActor->m_iZoneSerial	!=	_lpPacket->iZoneSerial)
		return;

	if	(_lpPacket->wKiller	!=	0xffff)
		lpKiller	=	getActor(_lpPacket->wKiller);

	lpActor->kill(_lpPacket->bf1IsReleaseSummon);//	사형 선고를 내리고..
	kill(lpKiller,lpActor,_lpPacket->bf1IsByKarma);	//	필드에서도 죽인다. (경험치나 기타등등..)

	operateForDeathActorWork(lpActor);
}

//
//	필드 이동을위한 게임 종료
void
cFIELD::operateBookedExitGameForMoveField(BW_EXIT_GAME_FOR_MOVE_FIELD *_lpPacket,int _iSerial)
{
	cACTOR		*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(STRCMP(lpActor->m_strName,_lpPacket->strName) != 0)
	{
		lpActor	=	getPlayerByName(_lpPacket->strName);

		if (!lpActor)
			return;
	}

	BOOL	bIsNotReadyToExit	=	FALSE;

	if	(lpActor->m_wWaitBankDataSaveResultTime)
	{
		bIsNotReadyToExit	=	TRUE;
		lpActor->m_wWaitBankDataSaveResultTime--;
	}

	if	(lpActor->m_wWaitBuyCarrotShopItemResultTime)
	{
		bIsNotReadyToExit	=	TRUE;
		lpActor->m_wWaitBuyCarrotShopItemResultTime--;
	}
	
	if	(bIsNotReadyToExit)
	{
		bookingToExitGameForMoveField(_iSerial,_lpPacket->strName);

		return;
	}

	lpActor->sendMoveFieldNow();

	CClient		*lpClient	=	CM.GetClientPntByIgnoreUsedFlag(lpActor->m_iClientSerial);

	if	(lpClient && lpClient->wSerial != 0xffff)
	{
		lpClient->SetActorSerialInGame(0xffffffff);
		lpClient->SetActorSerialInField(0xffff);

//		lpClient->SetIsUsed(eISUSED_MOVE_FIELD_IN_SAME_SERVER);
	}

	removeActor(_iSerial);				//	실제로 제거

//	다른 애들한테도 나간다고 알려 줘야지..
	{
		addSendRemoveActor(_iSerial);
	}
}

//
//	게임에서 나가기
void
cFIELD::operateBookedExitGame(BW_EXIT_GAME *_lpPacket,int _iSerial)
{
	cACTOR		*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if (STRCMP(lpActor->m_strName,_lpPacket->strName) != 0)
	{
		lpActor	=	getPlayerByName(_lpPacket->strName);

		if	(!lpActor)
			return;
	}

	if (m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG	&&	lpActor->getRestraintTimeByLogout())
	{
		if	(lpActor->getRestraintTimeByLogout()	==	0xffff)
			lpActor->setRestraintTimeByLogout();
		return;
	}

	BOOL	bIsNotReadyToExit	=	FALSE;

	if	(lpActor->m_wWaitBankDataSaveResultTime)
	{
		bIsNotReadyToExit	=	TRUE;
		lpActor->m_wWaitBankDataSaveResultTime--;
	}

	if	(lpActor->m_wWaitBuyCarrotShopItemResultTime)
	{
		bIsNotReadyToExit	=	TRUE;
		lpActor->m_wWaitBuyCarrotShopItemResultTime--;
	}
	
	if	(bIsNotReadyToExit)
	{
		bookingToExitGameForMoveField(_iSerial,_lpPacket->strName);

		return;
	}

	if	(lpActor->isDummy()	==	FALSE)
	{
		g_game.operteLogoutActorWorkById(lpActor->m_strId);
// 		int	iRemoveClientCount	=	g_game.operteLogoutActorWorkById(lpActor->m_strId);
// 
// 		if (iRemoveClientCount != 1)
// 		{
// 			printf("log out error : ID [%s] %d\n",lpActor->m_strId,iRemoveClientCount);
// 			CLOG("logOutError","계정 [%s]가 %d명",lpActor->m_strId,iRemoveClientCount);
// 		}
	}

	removeActor(_iSerial);				//	실제로 제거

//	다른 애들한테도 나간다고 알려 줘야지..
	{
		addSendRemoveActor(_iSerial);
	}

	return;
}

//
//	게임에서 나가기
void
cFIELD::operateReadyToExit(BW_READY_TO_EXIT *_lpPacket,int _iSerial)
{
	cACTOR		*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(STRCMP(lpActor->m_strName,_lpPacket->strName) != 0)
	{
		lpActor	=	getPlayerByName(_lpPacket->strName);

		if (!lpActor)
			return;
	}

	if	(lpActor->isDummy())
	{
		CLOG("dummy","cFIELD::operateReadyToExit");
		return;
	}

	lpActor->readyToExitGame();	//	게임 종료 준비 서버에 저장하고 결과를 기다려야 한다.
}

//
//	예약된 더미 만들기 처리
void
cFIELD::operateBookedMakeDummy(BW_MAKE_DUMMY *_lpPacket,int _iSerial)
{
	int	i,iTeam;

	if	(!checkActor(_lpPacket->wLord))
		return;

	cACTOR	*lpActor	=	getActor(_lpPacket->wLord);

	if	(!lpActor	||	lpActor->isDummy())
		return;

	cACTOR	*lpDummy	=	addActor(lpActor->m_wTeam,lpActor->m_wActorKind,lpActor->m_pos.x,lpActor->m_pos.y);

	if	(!lpDummy)
		return;

	if	(lpActor->isPlayer())
		lpDummy->setDataByJoinPlayerData((CPlayerData*)lpActor,0xffff,NULL);
	else
	{
		cNPC		*lpNpc		=	lpActor->getNpc();
		cCHARACTER	*lpCharacter=	&m_aCharacter[lpNpc->m_wCharacter];

		lpDummy->setDataByCharacter(lpCharacter,lpNpc->m_wActorKind,lpActor->getLevel());
	}

	memset(lpDummy->m_aAbility,0xff,sizeof(lpDummy->m_aAbility));
	memset(lpDummy->m_aActiveReaction,0xff,sizeof(lpDummy->m_aActiveReaction));

	for	(i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
		lpDummy->m_aEquip[i].reset();

	for	(i=0;i<dOWN_ITEM_COUNT;i++)
		lpDummy->m_aItems[i].reset();

	lpDummy->setActorPos(_lpPacket->iX,_lpPacket->iY);

	lpDummy->m_maxHP.m_iValue	+=	lpDummy->m_maxHP.m_iValue*_lpPacket->wAddHPPercentage/100;
	lpDummy->m_iHP				=	lpDummy->getMaxHP();
	lpDummy->m_wDummyTime		=	_lpPacket->wUpkeepTime;
	lpDummy->m_wDummyLord		=	_lpPacket->wLord;
	lpDummy->m_dwDummyLordNameHashCode	=	lpActor->m_dwNameHashCode;		//	난 더미다!! 주인장

	if	(lpActor->isPureMonster())
	{
		strcpy(lpDummy->m_strName,lpActor->m_strName);
		strcpy(lpDummy->m_strId,"NPC");
	}
	else
		sprintf(lpDummy->m_strName,"DUMMY %d",lpActor->m_iZoneSerial);

	lpDummy->m_dwNameHashCode	=	GetHashCode((BYTE *)lpDummy->m_strName);
	lpDummy->setPartySerial(0xffff);

	SG_CREATE_DUMMY	sJoinPlayer;

	sJoinPlayer.base.set(sizeof(SG_CREATE_DUMMY),dSG_CREATE_DUMMY);

	if	(lpActor->isPlayer())
		lpDummy->getSimplePlayerInfo(&sJoinPlayer.actorInfo);
	else
		lpDummy->getSimpleMonsterInfo((CSimpleMonsterInfo *)&sJoinPlayer.actorInfo);

	sJoinPlayer.base.wSize	=	sizeof(sJoinPlayer)-sizeof(sJoinPlayer.actorInfo.strName)+strlen(sJoinPlayer.actorInfo.strName)+1;

	addSendPacket((ALL_MSG*)&sJoinPlayer,lpDummy->m_wSerialInField);

	for (iTeam=0;iTeam<dTEAM_COUNT;iTeam++)		//	적 캐릭터
	{
		if (iTeam	==	lpActor->m_wTeam)
			continue;

		for (i=m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			cACTOR	*lpEnemy	=	getActor(m_awActorList[iTeam][i-1]);

			if (!lpEnemy || lpEnemy->isDeath())
				continue;
			if (lpEnemy->m_lpTarget	==	NULL)
				continue;

			if (lpEnemy->m_lpTarget->m_wSerialInField	==	lpActor->m_wSerialInField)
				lpEnemy->setTarget(lpDummy);
		}
	}
}

//	환영 만들기
void
cFIELD::operateAppointmentMakeIllusion(BW_MAKE_ILLUSION *_lpPacket,int _iSerial)
{
	int	i,iTeam;

	if	(!checkActor(_lpPacket->wLord))
		return;

	cACTOR	*lpActor	=	getActor(_lpPacket->wLord);

	if	(!lpActor	||	lpActor->isDummy()	||	lpActor->m_dwNameHashCode	!=	_lpPacket->dwLordHashCode)
		return;

	cACTOR	*lpDummy	=	addActor(lpActor->m_wTeam,lpActor->m_wActorKind,lpActor->m_pos.x,lpActor->m_pos.y);

	if	(!lpDummy)
		return;

	if	(lpActor->isPlayer())
		lpDummy->setDataByJoinPlayerData((CPlayerData*)lpActor,0xffff,NULL);
	else
	{
		cNPC		*lpNpc		=	lpActor->getNpc();
		
		if	(!lpNpc)
			return;

		cCHARACTER	*lpCharacter=	&m_aCharacter[lpNpc->m_wCharacter];

		lpDummy->setDataByCharacter(lpCharacter,lpNpc->m_wActorKind,lpActor->getLevel());
	}

	memset(lpDummy->m_aAbility,0xff,sizeof(lpDummy->m_aAbility));
	memset(lpDummy->m_aActiveReaction,0xff,sizeof(lpDummy->m_aActiveReaction));

	for	(i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
		lpDummy->m_aEquip[i].reset();

	for	(i=0;i<dOWN_ITEM_COUNT;i++)
		lpDummy->m_aItems[i].reset();

	lpDummy->setActorPos(_lpPacket->iX,_lpPacket->iY);

	lpDummy->m_maxHP.m_iValue			+=	_lpPacket->iHP;
	lpDummy->m_iHP						=	lpDummy->getMaxHP();
	lpDummy->m_wDummyTime				=	_lpPacket->wUpkeepTime;
	lpDummy->m_wDummyLord				=	_lpPacket->wLord;
	lpDummy->m_wIllusionLord			=	_lpPacket->wLord;
	lpDummy->m_dwDummyLordNameHashCode	=	lpActor->m_dwNameHashCode;		//	난 더미다!! 주인장
	lpDummy->m_wJob						=	_lpPacket->wJob;
	lpDummy->m_wTrueJob					=	_lpPacket->wJob;

	if	(_lpPacket->wIsFirstTarget)
		lpDummy->m_wTargetedTime		=	_lpPacket->wUpkeepTime;

	if	(lpActor->isPureMonster())
	{
		strcpy(lpDummy->m_strName,lpActor->m_strName);
		strcpy(lpDummy->m_strId,"NPC");
	}
	else
		strcpy(lpDummy->m_strName,"Illusion");

	lpDummy->m_dwNameHashCode	=	GetHashCode((BYTE *)lpDummy->m_strName);
	lpDummy->setPartySerial(0xffff);

	SG_CREATE_DUMMY	sJoinPlayer;

	sJoinPlayer.base.set(sizeof(SG_CREATE_DUMMY),dSG_CREATE_DUMMY);

	if	(lpActor->isPlayer())
		lpDummy->getSimplePlayerInfo(&sJoinPlayer.actorInfo);
	else
		lpDummy->getSimpleMonsterInfo((CSimpleMonsterInfo *)&sJoinPlayer.actorInfo);

	sJoinPlayer.base.wSize	=	sizeof(sJoinPlayer)-sizeof(sJoinPlayer.actorInfo.strName)+strlen(sJoinPlayer.actorInfo.strName)+1;

	addSendPacket((ALL_MSG*)&sJoinPlayer,lpDummy->m_wSerialInField);

	for (iTeam=0;iTeam<dTEAM_COUNT;iTeam++)		//	적 캐릭터
	{
		if	(iTeam	==	lpActor->m_wTeam)
			continue;

		for (i=m_aiActorCount[iTeam];i;i--)		//	적 캐릭터
		{
			cACTOR	*lpEnemy	=	getActor(m_awActorList[iTeam][i-1]);

			if	(!lpEnemy || lpEnemy->isDeath())
				continue;
			if	(lpEnemy->m_lpTarget	==	NULL)
				continue;
			if	(lpEnemy->m_lpTarget->m_wSerialInField	==	lpActor->m_wSerialInField)
				lpEnemy->setTarget(lpDummy);
		}
	}

	{
		cAbility	ability;

		ability.set(_lpPacket->wSkill,10);
		
		lpDummy->addSendSkillEffect(&ability,eSE_HIT);
	}
}

//
//	예약된 캐릭터 복제 처리
void
cFIELD::operateBookedCopyActor(BW_COPY_ACTOR *_lpPacket,int _iSerial)
{
	int		i;

	cACTOR	*lpLord		=	getActor(_lpPacket->wLord);
	cACTOR	*lpTarget	=	getActor(_lpPacket->wTarget);

	if	(!lpLord	||	!lpTarget)
		return;

	cACTOR	*lpCopyActor	=	addActor(lpLord->m_wTeam,lpTarget->m_wActorKind,_lpPacket->iX,_lpPacket->iY);

	if	(!lpCopyActor)
		return;

	if	(lpTarget->isPlayer())
		lpCopyActor->setDataByJoinPlayerData((CPlayerData*)lpTarget,0xffff,NULL);
	else
	if	(lpTarget->isSummonBeast() || lpTarget->isPet())
	{
		memcpy(lpCopyActor,lpTarget,sizeof(CPlayerSaveData));
		lpCopyActor->m_wScaleX	=	lpTarget->m_wScaleX;
		lpCopyActor->m_wScaleY	=	lpTarget->m_wScaleY;
		lpCopyActor->m_wTrueJob	=	lpTarget->m_wTrueJob;
		lpCopyActor->buildPower();
	}
	else
	{
		cNPC		*lpNpc		=	lpTarget->getNpc();

		if	(!lpNpc)
			return;

		cCHARACTER	*lpCharacter=	&m_aCharacter[lpNpc->m_wCharacter];

		lpCopyActor->setDataByCharacter(lpCharacter,lpNpc->m_wActorKind,lpTarget->getLevel());
	}

	for	(i=dEQUIP_BELT_SLOT1;i<=dEQUIP_BELT_SLOT5;i++)
		lpCopyActor->m_aEquip[i].reset();

	for	(i=0;i<dOWN_ITEM_COUNT;i++)
		lpCopyActor->m_aItems[i].reset();

	lpCopyActor->setActorPos(_lpPacket->iX,_lpPacket->iY);
	lpCopyActor->m_wCopyTime	=	_lpPacket->wUpkeepTime;
	lpCopyActor->m_wTargetedTime=	2;

	if	(lpTarget->isPureMonster())
	{
		strcpy(lpCopyActor->m_strName,lpTarget->m_strName);
		strcpy(lpCopyActor->m_strId,"NPC");
	}
	else
	{
		sprintf(lpCopyActor->m_strName,"C O P Y");
	}

	lpCopyActor->m_dwNameHashCode				=	GetHashCode((BYTE *)lpCopyActor->m_strName);
	lpCopyActor->setPartySerial(0xffff);
	lpCopyActor->m_wBattleInclination			=	dBI_AGGRESSIVE_SOLO;
	lpCopyActor->m_wOrgBattleInclination		=	dBI_AGGRESSIVE_SOLO;
	lpCopyActor->m_wCopyActorLord				=	lpLord->m_wSerialInField;
	lpCopyActor->m_dwCopyActorLordNameHashCode	=	lpLord->m_dwNameHashCode;
	lpCopyActor->setRemainActionTime(dSYNC_FPS);
	lpCopyActor->m_maxHP.m_iValue				=	_lpPacket->iHP;
	lpCopyActor->m_iHP							=	lpCopyActor->getMaxHP();
	lpCopyActor->m_isRunning					=	TRUE;

	{
		SG_COPY_ACTOR	sJoinPlayer;

		sJoinPlayer.base.set(sizeof(SG_COPY_ACTOR),dSG_COPY_ACTOR);

		int	iAngle	=	g_protractor.getAngleToTarget(lpLord->m_pos.x,lpLord->m_pos.y,lpCopyActor->m_pos.x,lpCopyActor->m_pos.y);

		if	(lpCopyActor->isPlayer())
		{
			lpCopyActor->getSimplePlayerInfo(&sJoinPlayer.actorInfo);
			sJoinPlayer.actorInfo.bf3Direct	=	GetDirect(iAngle,8);
		}
		else
		{
			lpCopyActor->getSimpleMonsterInfo((CSimpleMonsterInfo *)&sJoinPlayer.actorInfo);
			((CSimpleMonsterInfo *)&sJoinPlayer.actorInfo)->bf3Direct	=	GetDirect(iAngle,8);
		}

		sJoinPlayer.base.wSize	=	sizeof(sJoinPlayer)-sizeof(sJoinPlayer.actorInfo.strName)+strlen(sJoinPlayer.actorInfo.strName)+1;

		addSendPacket((ALL_MSG*)&sJoinPlayer,lpTarget->m_wSerialInField);
	}
}

void
cFIELD::operateBookedDisplacement(BW_DISPLACEMENT *_lpPacket,int _iSerial)
{
	cACTOR	*lpCaster	=	getActor(_lpPacket->wCaster);
	cACTOR	*lpTarget	=	getActor(_lpPacket->wTarget);

	if	(_lpPacket->wIsRelease)
		releaseDisplacement(lpCaster,lpTarget);
	else
	{
		if	(!lpCaster || !lpTarget)
			return;

		displacement(lpCaster,lpTarget);
	}
}

void
cFIELD::operateBookedCharming(BW_CHARMING *_lpPacket,int _iSerial)
{
	cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wTarget);

	if	(!lpTarget || STRICMP(lpTarget->m_strName,_lpPacket->strTargetName) != 0 || lpTarget->isPlayer())
		return;

	cACTOR	*lpCaster	=	getTestedActor(_lpPacket->wCaster);

	if	(_lpPacket->wIsRelease	==	FALSE)
		if	(!lpCaster || STRICMP(lpCaster->m_strName,_lpPacket->strCasterName) != 0)
			return;

	if	(_lpPacket->wIsRelease)
		lpTarget->releaseCharm(FALSE);
	else
		lpTarget->charm(lpCaster,_lpPacket->wTime,FALSE);

	stopAllTargetedAction(lpTarget);
}

void
cFIELD::operateBookedTame(BW_MONSTER_TAME *_lpPacket)
{
	cACTOR	*lpTarget	=	getActor(_lpPacket->wTargetFieldSerial);
	cACTOR	*lpCaster	=	getPlayerByName(_lpPacket->strCaster);

	if	(!lpTarget || lpTarget->m_iZoneSerial != _lpPacket->wTargetZoneSerial || lpTarget->isDeath())
		return;

	if	(lpTarget->isPlayer())
	{
		CLOG("error","cFIELD::operateBookedTame - lpTarget->isPlayer()");
		return;
	}

	if	(!lpCaster || lpCaster->m_wCurrentField	!=	m_iUniqueSerial)
		return;

	int	iPetIndex	=	lpCaster->addPet(lpTarget->m_wJobIndex,_lpPacket->wTamedLevel,_lpPacket->wTamedLoyalty,lpTarget);

	if	(iPetIndex	==	0xffff)
		return;

	cPOINT	pos;

	pos.Set(lpTarget->m_pos.x,lpTarget->m_pos.y);

	lpTarget->tamed(lpCaster);
	lpTarget->buildByPetInfo(lpCaster,&lpCaster->m_aPet[iPetIndex],iPetIndex);
	lpTarget->setActorPos(pos.x,pos.y);

	lpCaster->sendPetInfo(iPetIndex,lpTarget->m_wSerialInField);
	lpTarget->addSendSimpleInfo();

	lpCaster->sendPetWorkLog(iPetIndex,ePW_ADD);

	stopAllTargetedAction(lpTarget);
}

void
cFIELD::operateBookedGeneratePet(BW_GENERATE_PET *_lpPacket)
{
	cACTOR	*lpTamer	=	getPlayerByName(_lpPacket->strTamer);

	if (!lpTamer)
		return;

	if (lpTamer->m_wCurrentField	>=	dINSTANCE_FIELD_FLAG)
	{
		if (lpTamer->m_bf8InstanceField		!=	(DWORD)(m_iUniqueSerial/dINSTANCE_FIELD_FLAG))
			return;

		if (lpTamer->m_bf4InstanceFieldFloor!=	(DWORD)(m_iUniqueSerial%dINSTANCE_FIELD_FLAG))
			return;

		if (m_wGateField		!=	lpTamer->m_wCurrentField-dINSTANCE_FIELD_FLAG)
			return;
	}
	else
	if (lpTamer->m_wCurrentField != m_iUniqueSerial)
		return;

	generatePet(lpTamer,_lpPacket->bf4PetIndex,_lpPacket->bf1IsUnseal);
}

void
cFIELD::operateBookedGenerateSummonBeast(BW_GENERATE_SUMMON_BEAST *_lpPacket)
{
	cACTOR	*lpSummoner	=	getPlayerByName(_lpPacket->strTamer);

	if	(!lpSummoner)
		return;

	if	(lpSummoner->m_wCurrentField	>=	dINSTANCE_FIELD_FLAG)
	{
		if	(lpSummoner->m_bf8InstanceField		!=	(DWORD)(m_iUniqueSerial/dINSTANCE_FIELD_FLAG))
			return;

		if	(lpSummoner->m_bf4InstanceFieldFloor!=	(DWORD)(m_iUniqueSerial%dINSTANCE_FIELD_FLAG))
			return;

		if	(m_wGateField		!=	lpSummoner->m_wCurrentField-dINSTANCE_FIELD_FLAG)
			return;
	}
	else
	if	(lpSummoner->m_wCurrentField != m_iUniqueSerial)
		return;

	generateSummonBeast(lpSummoner,_lpPacket->bf10Level,_lpPacket->iRemainHP,_lpPacket->bf2Type,_lpPacket->bf2Grade,_lpPacket->wX,_lpPacket->wY,_lpPacket->bf1IsSummon);
}

void
cFIELD::operateBookedIntervalShooter(BW_INTERVAL_SHOOTER *_lpPacket)
{
	CActiveSkill	*lpIntervalShooter	=	m_skillManager.getActiveSkill(_lpPacket->wSkillIndex);

	if	(!lpIntervalShooter || !lpIntervalShooter->m_lpCaster)
		return;

	cACTOR	*lpCaster	=	lpIntervalShooter->m_lpCaster;
	cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wTarget);

	if	(!lpCaster || !lpTarget || lpCaster->isDeath() || lpTarget->isDeath()||
		lpCaster->m_wSerialInField	!=	_lpPacket->wCaster					||	lpTarget->m_wSerialInField	!=	_lpPacket->wTarget				||
		lpCaster->m_dwNameHashCode	!=	_lpPacket->dwCasterNameHashCode		||	lpTarget->m_dwNameHashCode	!=	_lpPacket->dwTargetNameHashCode	||
		lpCaster->m_wSerialInField	==	lpTarget->m_wSerialInField || lpTarget->m_wRabbitTime)
		return;

	lpIntervalShooter->strikeIntervalShooter(lpTarget);
}

void
cFIELD::operateMakeSkillZone(BW_MAKE_SKILL_ZONE *_lpPacket)
{
	cACTOR	*lpCaster	=	getActor(_lpPacket->wCaster);

	if	(!lpCaster	||	lpCaster->isDeath()	||	lpCaster->m_dwNameHashCode	!=	_lpPacket->dwCasterNameHashCode)
		return;

	cAbility	ability;

	ability.set(_lpPacket->wSkill,_lpPacket->wSkillLevel);

	m_skillManager.castGasTypeSkill(lpCaster,lpCaster->m_pos.x,lpCaster->m_pos.y,&ability,0);

	addSendEtcWork(_lpPacket->wCaster,eEW_MAKE_SKILL_ZONE,_lpPacket->wCaster,_lpPacket->wSkill,lpCaster->m_pos.x,lpCaster->m_pos.y);
}
