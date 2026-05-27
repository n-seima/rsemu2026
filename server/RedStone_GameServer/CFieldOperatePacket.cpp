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
#include "cGuild.H"
#include "actor_mini_pet.H"
#include "duel.H"
#include "CEvent.H"

#include "XTrap/Xtrap_S_Interface.h"

//
//	패킷 추가
BOOL
cFIELD::addReceivePacket(ALL_MSG *_lpPacket,int _iSerial,BOOL _bIsBooking)
{
	mCS(m_csPacketManager);

	if	(_bIsBooking)
		return	m_queReceivePacket.addPacket(_lpPacket,_iSerial);

	BOOL	isResult	=	FALSE,bIsAcceptPacket = TRUE;
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		bIsAcceptPacket	=	FALSE;
	else
	if	(lpActor->m_dwPeneltyTime	||	m_bIsProcessEvent	||	lpActor->m_bIsWaitOrgAvatarData)	//	페널티 타임이다.
	{
		bIsAcceptPacket	=	FALSE;	//	클라이언트에서 날라오는 패킷을 아예 무시 -o-

		if	(_lpPacket->base.wType == dCG_ASK_PLAYER_INFO || _lpPacket->base.wType == dCG_CHECK_PLAYER_CHECK_SUM ||
			_lpPacket->base.wType == dCG_XTRAP_PACKET)
			bIsAcceptPacket	=	TRUE;	//	얘들은 무시 하면 안되는 패킷
	}

	if	(bIsAcceptPacket)
	{
		if	(_lpPacket->base.wSize	>	sizeof(ALL_MSG) || _lpPacket->base.wType < dCM_DUMMY )
		{
			lpActor->sendHackingLog(ePPPTR_PACKET_HACKING);

			return	FALSE;
		}

		isResult		=	m_queReceivePacket.addPacket(_lpPacket,_iSerial);
	}

	return	isResult;
}

void
cFIELD::receiveReportCurrentPos(CG_REPORT_CURRENT_POS *_lpPacket,int _iSerial)
{
}

//
//	플레이어의 이동
void
cFIELD::movePlayer(CG_MOVE *_lpPacket,int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	cACTOR	*lpActor	=	getActor(_iSerial);

	if	(lpActor->m_wJob	==	0xffff)
	{
		CLOG("incorrectJob","cFIELD::movePlayer - lpActor->m_wJob == 0xffff[%s/%s]",lpActor->m_strId,lpActor->m_strName);
		return;
	}

	if	(lpActor->isSealdMove())
	{
		lpActor->sendMoveFailed(eMOVE_RESULT_FAILED_BY_STATUS);
		return;
	}

	if	(lpActor->m_wSlaveActor	==	0xffff	&&	!lpActor->checkMoveCounter(_lpPacket->wPosX,_lpPacket->wPosY,"CG_MOVE"))
		return;

//	필드를 벗어나는 좌표는 아닌지 확인
	if	(isIncorrectPos(_lpPacket->wPosX,_lpPacket->wPosY)			)
		return;
	if	(isIncorrectPos(_lpPacket->wDestPosX,_lpPacket->wDestPosY)	)
		return;

	int		iMoveResult	=	lpActor->movePlayer(_lpPacket->wPosX,_lpPacket->wPosY,_lpPacket->wDestPosX,_lpPacket->wDestPosY);

	if	(iMoveResult		!=	eMOVE_RESULT_SUCCESS)
	{
		lpActor->sendMoveFailed(iMoveResult);
		return;
	}

	//	응답
	{
		SG_MOVE_RESULT	packetMoveResult;

		packetMoveResult.base.set(sizeof(SG_MOVE_RESULT),dSG_MOVE_RESULT);

		packetMoveResult.isRun		=	lpActor->m_isRunning;//	뛰냐-_-?
		packetMoveResult.wPPS		=	lpActor->m_wPPS;//	Pixel Per Second
		
		LONGLONG	llRestHP	= lpActor->m_iHP;
		llRestHP	*=	10000;
		llRestHP	/=	lpActor->getMaxHP();

		packetMoveResult.bf14HP				=	(WORD)llRestHP;
		packetMoveResult.sCP				=	lpActor->m_iCP/100;
		packetMoveResult.bf1MiniPetCharge1	=	lpActor->m_aActiveMiniPetInfo[0].m_wIsCharged;
		packetMoveResult.bf1MiniPetCharge2	=	lpActor->m_aActiveMiniPetInfo[1].m_wIsCharged;

		if	(lpActor->m_iHP)
			packetMoveResult.bf14HP	=	max(1,packetMoveResult.bf14HP);

		g_userPM.add(lpActor->m_iClientSerial,&packetMoveResult,packetMoveResult.base.wSize);
	}

	{
//		updateBorderActorPositionInfo(lpActor,_lpPacket->wPosX,_lpPacket->wPosY);
	}

	lpActor->updateLastActionTime();
	lpActor->addSendMove(lpActor->m_posLastMoved.x,lpActor->m_posLastMoved.y,_lpPacket->wDestPosX,_lpPacket->wDestPosY,FALSE);
}

//
//	플레이어가 정지한다.
void
cFIELD::stopPlayer(CG_STOP *_lpPacket,int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	cACTOR	*lpActor	=	getActor(_iSerial);

	if	(lpActor->isSealdMove())
	{
		lpActor->sendMoveFailed(eMOVE_RESULT_FAILED_BY_STATUS);

		return;
	}

	if	(lpActor->m_bf1IsJoinGuildDungeon)		// 길드던전에 입장한다~
	{
		lpActor->m_bf1IsJoinGuildDungeon	=	FALSE;
		
		++lpActor->m_bf3GuildDungeonPlayTime;

		return;
	}

	if	(m_bIsGuildPointBattleField)
	{
		CGuild	*lpGuild	=	lpActor->getGuild();
				
		if	(lpGuild && lpGuild->m_bf1IsMoveToPointBattleField)
		{
			lpGuild->m_bf1IsMoveToPointBattleField	=	FALSE;

			g_game.sendWPSimpleWork(eSWGW_INCREASE_TRY_GUILD_POINT_BATTLE_COUNT,lpGuild->m_wSerial);
		}
	}

//	if (lpActor->m_isExclusiveAction)	goto ERROR_LABEL;

//	필드를 벗어나는 좌표는 아닌지 확인
	if	(lpActor->m_wSlaveActor	!=	0xffff)
		return;

	if	(!lpActor->checkMoveCounter(_lpPacket->wPosX,_lpPacket->wPosY,"CG_STOP"))
		goto ERROR_LABEL;
	if	(isIncorrectPos(_lpPacket->wPosX,_lpPacket->wPosY))
		goto ERROR_LABEL;
	if	(!lpActor->stop(_lpPacket->wPosX,_lpPacket->wPosY))
		goto ERROR_LABEL;

	if	(m_bIsGuildBattleField)
		lpActor->checkCaughtInTrap();

	checkFieldTrap(lpActor);

	lpActor->m_wDirect	=	_lpPacket->wDirect;

	lpActor->addPacketStop();

	{
//		updateBorderActorPositionInfo(lpActor,_lpPacket->wPosX,_lpPacket->wPosY);
	}

	return;

ERROR_LABEL:

	lpActor->sendMoveFailed(eMOVE_RESULT_FAILED_BY_BLOCKED_POS);

	return;
}

//
//	필드 이동
void
cFIELD::moveField(CG_MOVE_FIELD	*_lpPacket,int _iSerial)
{
	cACTOR		*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if	(lpActor->isSealdMove())
	{
		lpActor->sendMoveFieldResult(dMOVEMAP_RESULT_CAN_NOT_MOVE_FIELD_STATUS);
		return;
	}

	if	(lpActor->getRestraintTimeByBattle())
	{
		lpActor->sendEtcWork(eEW_CAN_NOT_MOVE_FIELD_DURING_BATTLE,lpActor->getRestraintTimeByBattle());

		return;
	}

	cAreaInfo	*lpArea	=	m_pArea->getFocusArea(lpActor->m_pos.x,lpActor->m_pos.y);

	if (lpActor->m_wWaitBankDataSaveResultTime || lpActor->m_wWaitBuyCarrotShopItemResultTime)
	{
		lpActor->sendMoveFieldResult(dMOVEMAP_RESULT_CAN_NOT_MOVE_FIELD_STATUS);
		return;
	}

	if	(lpArea	==	NULL)	//	실패~~ -o-r~~
	{
// 		MessageBox(NULL,m_strFileName,"lpArea	==	NULL",MB_OK);
		lpActor->sendMoveFieldResult(dMOVEMAP_RESULT_NOTFIND);

		return;
	}

	if	(lpArea->m_wIsTestPortal && g_bIsTestServer == FALSE)
	{
		lpActor->sendMoveFieldResult(dMOVEMAP_RESULT_TEST_PORTAL);
		return;
	}

	if	(lpArea->m_gateShape	==	ePAS_ONE_WAY_GATE)
	{
		lpActor->sendMoveFieldResult(dMOVEMAP_RESULT_ONE_WAY_PORTAL);
		return;
	}

	if	(lpArea->m_gateShape	==	ePAS_INNER_PORTAL	||	lpArea->m_gateShape	==	ePAS_HIDDEN_INNER_PORTAL)
	{
		cAreaInfo	*lpTargetArea	=	getArea(lpArea->m_wMoveGate);

		if	(lpTargetArea)
		{
			cPOINT	pos;

			lpTargetArea->getCenterPos(&pos);

			g_game.sendWPMoveField(lpActor,NULL,m_iUniqueSerial,lpArea->m_wMoveGate,pos.x,pos.y,TRUE);

			return;
		}
	}

	if	(lpArea->m_gateShape	==	ePAS_LEAVE_GUILD_HALL)
	{
		g_pMoveFieldUserManager->booking(lpActor,NULL,lpActor->m_wLastVillage,0x7ffe);
		return;
	}
	
	g_pMoveFieldUserManager->booking(lpActor,lpArea->getMapName(),0xffff,lpArea->m_wMoveGate);
	lpActor->addSendVerySimpleInfo();
}

//
//	교전
void
cFIELD::engageBattle(CG_ENGAGE_BATTLE *_lpPacket,int _iSerial)
{
}

void
cFIELD::receiveActionToObject(CG_ACTION_TO_OBJECT *_lpPacket,int _iSerial)
{
	cACTOR		*lpCaster	=	getTestedActor(_iSerial);

	if (!lpCaster)
		return;

	cAbility	*lpAbility	=	lpCaster->getAbility(_lpPacket->bAbility);

	if (!lpAbility)
		return;

	int	iAR	=	lpAbility->isUseAbleToObject(_lpPacket->bObjectType);

	if (iAR	!=	eAR_OK)
	{
		lpCaster->sendUseSkillResult(iAR);
		return;
	}

	iAR	=	lpCaster->actionToObject(lpAbility,_lpPacket->bObjectType,_lpPacket->wObjectSerial);

	if (iAR	!=	eAR_OK)
	{
		lpCaster->sendUseSkillResult(iAR);
		return;
	}
}

//
//	플레이어가 캐릭터에게 액션을 한다.
int
cFIELD::actionToActor(CG_ACTION_TO_ACTOR *_lpPacket,int _iSerial,BOOL _isFirst)
{
	cACTOR		*lpCaster	=	getTestedActor(_iSerial);
	cACTOR		*lpTarget	=	getTestedActor(_lpPacket->wTargetSerial);
	cAbility	*lpAbility	=	lpCaster->getAbility(_lpPacket->bAbility);

	cAbility	ability;
	
	cAbility	transAbility;	

	if(lpCaster->m_wTransformationTime)
	{
		transAbility.m_wSkill = lpCaster->m_wTransformationSkill;	
		transAbility.m_wLevel = lpCaster->m_wTransformationLevel;
		lpAbility	=	&transAbility;
	}

	BOOL		bIsAttackShakle	=	FALSE;

	if	(!lpCaster || !lpAbility	||	lpCaster->m_wHwabyungTime)
		return	eAR_FAILED;

	if	(!lpTarget || lpTarget->m_wIsReleaseSummonBeast	||	lpTarget->m_wHwabyungTime || lpTarget->isNpc())
		return	eAR_FAILED;

	if	(m_bIsGuildBattleField)
	{
		if	(m_bIsSiegeWarfareField	==	FALSE)
		{
			if	(m_wGuildBattleStatus != eGB_BATTLE && m_wGuildBattleStatus != eGB_READY_TO_BATTLE_FINISH)
				return	eAR_FAILED;
		}
		else
		{
			if	(lpTarget	&&	lpTarget->m_iGuildGuardianType)
				return	eAR_FAILED;

			if	(lpTarget	&&	lpTarget->isOwnForce(lpCaster) && lpTarget->m_bIsGuildObject)
				return	eAR_FAILED;
		}
	}

	if	(lpCaster->isSealdAction(lpAbility->m_wSkill))
	{
		lpCaster->sendUseSkillResult(eAR_BUSY);
		return	eAR_FAILED;
	}

	if	(lpCaster->m_bf5MagicCarpetShape	!=	eMCS_GEAR)
		lpCaster->m_bf5MagicCarpetShape	=	0;

	if	(!lpAbility	||	lpAbility->m_wLevel	==	0)
	{
		lpCaster->sendUseSkillResult(eAR_NOT_LEARNED_SKILL);
		return	eAR_FAILED;
	}

	if	(lpCaster->isDeath()	)
		return	eAR_FAILED;

	if	(lpCaster->m_wLordOperator != 0xffff || lpCaster->m_wSlaveActor != 0xffff )
	{
		cACTOR	*lpSlave	=	getActor(lpCaster->m_wLordOperator);

		if	(!lpSlave)
			lpSlave	=	getActor(lpCaster->m_wSlaveActor);

		if	(!checkActor(lpSlave))
			return	eAR_FAILED;
		if	(lpSlave->isNpc())
			return	eAR_FAILED;

		if	(lpTarget->isOwnForce(lpCaster))
		{
			lpCaster->sendUseSkillResult(eAR_IS_CAN_NOT_ATTACK_TARGET);
			return	eAR_FAILED;
		}

		lpSlave->setAiFight(lpTarget);
		lpSlave->m_dwLastCommandTime	=	g_dwCurrentTime;

		return	eAR_FAILED;
	}

	if	(lpCaster->m_wOperatorLevel >= eAL_MASTER_OPERATOR)
		return	eAR_FAILED;

	ability.set(lpAbility->m_wSkill,lpAbility->m_wLevel+lpCaster->getCorrectSkillLevel(lpAbility));

	CSkill		*lpSkill	=	lpAbility->getSkill();

	if	(!lpSkill)
	{
		lpCaster->sendUseSkillResult(eAR_NOT_EXIST_SKILL);
		return	eAR_FAILED;
	}

	BOOL	bIsCorrectTarget=	lpCaster->isCastAbleTarget(lpTarget,&ability);

	if	(bIsCorrectTarget	==	FALSE	&&	lpSkill->m_bf1IsToggleSkill)
		if	(lpCaster->isSealdAction() && lpCaster->m_wUnsealAction == lpAbility->m_wSkill)
		{
			lpCaster->applySkillExtraEffects(lpCaster,lpAbility);

			return	eAR_FAILED;
		}

	if	(!bIsCorrectTarget)
	{
		lpCaster->sendUseSkillResult(eAR_NOT_APT_USED_SKILL_TO_TARGET);
		return	eAR_FAILED;
	}

	int		iResult;

	bIsAttackShakle			=	lpTarget->isAttackToShakleActor(lpCaster);

	if	(lpSkill->isAgressiveSkill())//	공격적인 스킬이다.
	{
		if	(bIsAttackShakle	==	FALSE	&&	lpCaster->isOwnForce(lpTarget) && !lpTarget->isEventMob())
		{
			lpCaster->sendUseSkillResult(eAR_IS_CAN_NOT_ATTACK_TARGET);

			return	eAR_FAILED;
		}
	}
	else
	{
		if	(lpTarget->isDummy())
		{
			lpCaster->sendUseSkillResult(eAR_NOT_APT_USED_SKILL_TO_TARGET);
			return	eAR_FAILED;
		}
	}

	if	(lpSkill->isResurrection()	||	lpSkill->isCastOnCorpseSkill())
	{
		if	(!checkPlayerAndKill(_iSerial)			)
			return	eAR_FAILED;

		if	(isValidSerial(_lpPacket->wTargetSerial)	)
		{
			cACTOR	*lpTargetActor	=	getActor(_iSerial);

			if	(lpTargetActor)
				lpTargetActor->sendRemoveActor(_lpPacket->wTargetSerial);
			return	eAR_FAILED;
		}
	}
	else
	{
		if	(!checkTargetAndKill(_iSerial,_lpPacket->wTargetSerial))
			return	eAR_FAILED;	
	}

	iResult		=	lpCaster->isUseAbleSkill(&ability);

	if	(!lpTarget)	
		iResult		=	eAR_TARGET_DEATH;
	if	(!lpSkill->isResurrection()	&&	!lpSkill->isCastOnCorpseSkill()	&&	lpTarget->isDeath())
		iResult		=	eAR_TARGET_DEATH;

	if	(lpSkill && lpSkill->m_dwTargetMethod & eSKILL_CAST_QUICK)
	{
		if	(lpCaster->m_wSerialInField != lpTarget->m_wSerialInField)
			iResult	=	eAR_IS_CAN_NOT_ATTACK_TARGET;
	}

	if	(iResult	!=	eAR_OK)
	{
		lpCaster->sendUseSkillResult(iResult);
		return	eAR_FAILED;
	}

//	공격적인 스킬이다.
	if (lpSkill->isAgressiveSkill())
	{
		cAbility	*lpEngageAbility	=	lpCaster->getAbility(_lpPacket->bEngageAbility);
		cAbility	*lpSubAbility		=	lpCaster->getAbility(_lpPacket->bSubAbility);

		lpCaster->setEngageAbility(lpEngageAbility,lpSubAbility);

#ifdef	dIS_USE_BLOCK_STEAL_SYSTEM
		{
			int	iAttckerSerial	=	lpCaster->m_wSerialInField;

			if	(lpCaster->getPartySerial()	!=	0xffff)
				iAttckerSerial	=	lpCaster->getPartySerial()+dPARTY_FLAG;

			if	(lpTarget->setFirstBeater(iAttckerSerial) == FALSE)
				lpCaster->sendRegistServerMessage(eRSM_PROTECTED_BY_PREVENT_STEAL,eSM_SYSTEM);
		}
#endif

		if (lpCaster->m_bf1IsDisableInvisibleByAttack)
		{
			if (lpCaster->m_sInvisivilityTime	>	0)
			{
				lpCaster->m_sInvisivilityTime	=	0;
				lpCaster->m_bIsHideByShadowHideSkill = FALSE;
				mDS.m_aiExValue[0]	=	10;
				lpCaster->updateEnchantedMagic();
			}
		}
	}

	int	iAttackResult	=	lpCaster->attackToActor(lpTarget,&ability);

	if	((iAttackResult > eAR_OK && iAttackResult < eAR_IS_INREGULAR_SKILL) || iAttackResult == eAR_NOT_LEARNED_SKILL)
	{
		lpCaster->sendUseSkillResult(iAttackResult);

		if	(iAttackResult == eAR_BUSY	&&	_isFirst)
			lpCaster->bookingAction(_lpPacket);

		return	iAttackResult;
	}

//JBC	스킬쓰면  미니펫 카운팅 버그 변경.	08-10-31
//	lpCaster->iMiniPet()->updateChargeCount();
	sendAttackToActorResult(iAttackResult,lpCaster,lpTarget,&ability);

	return	eAR_OK;
}

//
//	퀵 액션
void
cFIELD::actionQuick(CG_ACTION_QUICK *_lpPacket,int _iSerial,BOOL _isFirst)
{
	if	(m_bIsGuildBattleField)
	{
		if	(m_bIsSiegeWarfareField	==	FALSE)
		{
			if	(m_wGuildBattleStatus != eGB_BATTLE && m_wGuildBattleStatus != eGB_READY_TO_BATTLE_FINISH)
				return;
		}
	}

	if	(!checkPlayerAndKill(_iSerial))
		return;

	cACTOR		*lpCaster	=	getActor(_iSerial);
	cAbility	*lpAbility	=	lpCaster->getAbility(_lpPacket->wAbility);
	cAbility	ability;

	if	(!lpAbility)
		return;

	if	(lpCaster->isSealdAction(lpAbility->m_wSkill))
	{
		lpCaster->sendUseSkillResult(eAR_BUSY);
		return;
	}

	if(lpCaster->m_wTransformationTime)
		return;

	if	(lpCaster->m_bf5MagicCarpetShape	!=	eMCS_GEAR)
		lpCaster->m_bf5MagicCarpetShape	=	0;

	if	(lpCaster->m_wOperatorLevel >= eAL_MASTER_OPERATOR || lpCaster->m_wSlaveActor != 0xffff )
		return;

	if	(!lpCaster->isPlayer()										)
		return;

	int	iResult;

	if	(!lpAbility	||	lpAbility->m_wLevel	==	0)
		iResult	=	eAR_NOT_LEARNED_SKILL;
	else
	{
		ability.set(lpAbility->m_wSkill,lpAbility->m_wLevel+lpCaster->getCorrectSkillLevel(lpAbility));
		iResult	=	lpCaster->isUseAbleSkill(&ability);


		CSkill		*lpSkill	=	ability.getSkill();

		if	(!lpSkill)
			iResult	=	eAR_NOT_EXIST_SKILL;

		if	(lpSkill->m_bf1IsToggleSkill)
			if	(iResult	==	eAR_LOW_CP	||	iResult	==	eAR_IS_COOL_TIME)
				if	(lpCaster->isSealdAction() && lpCaster->m_wUnsealAction == lpAbility->m_wSkill)
					iResult	=	eAR_OK;
	}

	if	(iResult	!=	eAR_OK)
	{
		lpCaster->sendUseSkillResult(iResult);

		return;
	}

	if	(_isFirst)
		if	(!lpCaster->checkMoveCounter(_lpPacket->wPosX,_lpPacket->wPosY,"CG_ACTION_QUICK"))
			return;

	if	(!lpCaster->stop(_lpPacket->wPosX,_lpPacket->wPosY,TRUE,FALSE))
		return;

	CSkill	*lpSkill	=	ability.getSkill();

	if	(lpSkill->m_bf1IsToggleSkill)
	{
		if	(lpCaster->isSealdAction() && lpCaster->m_wUnsealAction == lpAbility->m_wSkill)
		{
			lpCaster->applySkillExtraEffects(lpCaster,&ability);

			return;
		}
	}

	int	iAttackResult	=	lpCaster->actionQuick(&ability);

	if	(iAttackResult == eAR_OK)
	{
		lpCaster->updateLastActionTime();
		lpCaster->cancelNeedState(lpSkill);	// 필요상태해제..
		sendActionQuick(lpCaster,&ability);
		lpCaster->emptyBookedAction();
		return;
	}
	if	(iAttackResult == eAR_BUSY)
	{	
		if	(_isFirst)
		{
			if	(g_bIsDuelServer)
			{		// 듀얼 서버일때...
				CActiveSkill* lpActive = lpCaster->getActiveSkill();
				
				if(lpActive)
				{// 사용중인스킬이 변신하는 스킬이면... 예약받지않는다.... 
					CSkill* lpSkill = lpActive->m_ability.getSkill() ;
					if(lpSkill)
						if(  (lpSkill->m_wSpecialEffect&eSKILL_SE_CHANGE_JOB) == eSKILL_SE_CHANGE_JOB )
							return;
				}
			}
			lpCaster->bookingQuickAction(_lpPacket);			

		}
		return;
	}

	if	(iAttackResult < eAR_BORDER_SPECIAL_SKILL)
	{
		lpCaster->sendUseSkillResult(iAttackResult);

		return;
	}

//	lpCaster->iMiniPet()->updateChargeCount();
	lpCaster->updateLastActionTime();
	lpCaster->emptyBookedAction();

	if	(lpAbility->getSkill()->isAgressiveSkill())//	공격적인 스킬이다.
		if	(lpCaster->m_bf1IsDisableInvisibleByAttack)
		{
			if	(lpCaster->m_sInvisivilityTime	>	0)
			{
				lpCaster->m_sInvisivilityTime	=	0;
				lpCaster->m_bIsHideByShadowHideSkill = FALSE;
				mDS.m_aiExValue[0]	=	11;
				lpCaster->updateEnchantedMagic();
			}
		}

	sendAttackToActorResult(iAttackResult,lpCaster,lpCaster,&ability);
}

//
//	플레이어가 대지에다가 액션을 한다.
void
cFIELD::actionToGround(CG_ACTION_TO_GROUND *_lpPacket,int _iSerial,BOOL _isFirst)
{
	if	(m_bIsGuildBattleField)
	{
		if	(m_bIsSiegeWarfareField	==	FALSE)
		{
			if	(m_wGuildBattleStatus != eGB_BATTLE && m_wGuildBattleStatus != eGB_READY_TO_BATTLE_FINISH)
				return;
		}
	}

	if (!checkPlayerAndKill(_iSerial))
		return;

	cACTOR		*lpCaster		=	getActor(_iSerial);
	cAbility	*lpAbility		=	lpCaster->getAbility(_lpPacket->wAbility);
	cAbility	ability;

	if	(!lpAbility)
		return;

	if	(lpCaster->isSealdAction(lpAbility->m_wSkill))
	{
		lpCaster->sendUseSkillResult(eAR_BUSY);
		return;
	}

	if(lpCaster->m_wTransformationTime)
		return;

	if	(lpCaster->m_bf5MagicCarpetShape	!=	eMCS_GEAR)
		lpCaster->m_bf5MagicCarpetShape	=	0;

	if	(isIncorrectPos(_lpPacket->wDestPosX,_lpPacket->wDestPosY))
		return;

	if	(!lpCaster->isPlayer())
		return;
	
	if	(lpCaster->m_wOperatorLevel >= eAL_MASTER_OPERATOR)
		return;

	if	(lpCaster->m_wLordOperator != 0xffff || lpCaster->m_wSlaveActor != 0xffff )
		return;

	int	iResult;

	if	(!lpAbility	||	lpAbility->m_wLevel	==	0)
	{
		iResult	=	eAR_NOT_LEARNED_SKILL;
	}
	else
	{
		ability.set(lpAbility->m_wSkill,lpAbility->m_wLevel+lpCaster->getCorrectSkillLevel(lpAbility));

		CSkill		*lpSkill	=	ability.getSkill();

		if (!lpSkill)
			iResult	=	eAR_NOT_EXIST_SKILL;

		iResult	=	lpCaster->isUseAbleSkill(&ability);
	}

	if (iResult	!=	eAR_OK)
	{
		lpCaster->sendUseSkillResult(iResult);

		return;
	}
	
	int	iAttackResult	=	lpCaster->actionToGround(_lpPacket->wDestPosX,_lpPacket->wDestPosY,&ability);
	
	switch(iAttackResult)
	{
		case	eAR_BUSY						:	//	바빠요
			if (_isFirst)
				lpCaster->bookingGroundAction(_lpPacket);
			return;

		case	eAR_LOW_CP						:	//	CP가 모자른다.
		case	eAR_TOO_DISTANCE				:	//	너무 멀다.
		case	eAR_IS_CAN_NOT_ATTACK_TARGET	:	//	공격할 수 없는 타겟이다.
		case	eAR_FAILED						:	//	암튼 실패
		case	eAR_BLOCKED_OBJECT_EXIST		:
		{
			lpCaster->sendUseSkillResult(iAttackResult);
			return;
		}
	}

	
	if (lpAbility->getSkill()->isAgressiveSkill())//	공격적인 스킬이다.
		if (lpCaster->m_bf1IsDisableInvisibleByAttack )
		{
			if (lpCaster->m_sInvisivilityTime	>	0)
			{
				lpCaster->m_sInvisivilityTime	=	0;
				lpCaster->m_bIsHideByShadowHideSkill = FALSE;
				mDS.m_aiExValue[0]	=	12;
				lpCaster->updateEnchantedMagic();
			}
		}
	lpCaster->cancelNeedState(ability.getSkill());		// 필요상태해제
//	lpCaster->iMiniPet()->updateChargeCount();
	lpCaster->updateLastActionTime();
	sendAttackToGroundResult(iAttackResult,lpCaster,_lpPacket->wDestPosX,_lpPacket->wDestPosY,&ability);
}

//
//	걷기/뛰기 토글
void
cFIELD::setMoveAbility(CG_SET_MOVE_ABILITY *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if (!lpActor)
		return;

	if	(lpActor->isSealdMove())
		return;

	if	(lpActor->m_wSealRunningTime)
		_lpPacket->wState	=	FALSE;

	lpActor->setMoveAbility(_lpPacket->wState);
	lpActor->sendMoveState();
}

//
//	변신
void
cFIELD::transformation(CG_TRANSFORMATION *_lpPacket,int _iSerial,int _iReason,BOOL _bIsRequreAfterMotion)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;

	if	(_iReason == eTR_ASK_OF_CLIENT)
	{
		if	(lpActor->m_wRemainActionTime)
			return;
		if	(!lpActor->checkMoveCounter(_lpPacket->wPosX,_lpPacket->wPosY,"CG_TRANSFORMATION"))
			return;
	}

	if	(isIncorrectPos(_lpPacket->wPosX,_lpPacket->wPosY))
		return;

	if	(!lpActor->stop(_lpPacket->wPosX,_lpPacket->wPosY))
		return;

	SG_TRANSFORMATION	packet;

	packet.base.set(sizeof(SG_TRANSFORMATION),dSG_TRANSFORMATION);

	int	iTR	=	lpActor->isAbleToTransformation();

	if	(iTR!=	eTRANSFORMATION_SUCCESS)
	{
		SG_TRANSFORMATION_FAILED	packetFailed;

		packetFailed.base.set(sizeof(SG_TRANSFORMATION_FAILED),dSG_TRANSFORMATION_FAILED);

		packetFailed.wReason	=	iTR;	//	액션중이다.

		packetFailed.sCP		=	lpActor->m_iCP/100;

		g_userPM.add(lpActor->m_iClientSerial,&packetFailed,packetFailed.base.wSize);
		return;
	}

	int	iTransFPS					=	lpActor->getFPS(lpActor->m_wJob%2 + dACT_CHANGE1);

	packet.wSerial					=	_iSerial;
	packet.bf8TransformationJob		=	lpActor->getAnotherJob();
	packet.bf4Reason				=	_iReason;
	packet.wSpentCP					=	lpActor->getSpentCPForTransformation();
	packet.bf1IsRequreOnlyAfterMotion=	_bIsRequreAfterMotion;

	packet.wPosX				=	_lpPacket->wPosX;
	packet.wPosY				=	_lpPacket->wPosY;

	lpActor->transformation(_lpPacket->wPosX,_lpPacket->wPosY,packet.bf8TransformationJob,packet.wSpentCP);

	cItem	*lpWeapon			=	lpActor->getWeapon();
	cItem	*lpShield			=	lpActor->getShield();
	cItem	*lpArmor			=	lpActor->getArmor();

	packet.bWeapon				=	0xff;
	packet.bWeaponEffect		=	0xff;
	packet.bShield				=	0xff;
	packet.bArmor				=	0xff;

	packet.bf6FPS				=	min(dSYNC_FPS*4, iTransFPS + iTransFPS*lpActor->m_bCorrectTransformationSpeed/100);

	if (lpWeapon)
	{
		packet.bWeapon			=	(BYTE)lpWeapon->getBasicItem(TRUE)->m_wEquippedShape;
		packet.bWeaponEffect	=	lpActor->getWeaponColorizeEffect();
	}
	if	(lpShield)
	{
		packet.bShield			=	(BYTE)lpShield->getBasicItem()->m_wEquippedShape;
	}
	if	(lpArmor)
	{
		packet.bArmor			=	(BYTE)lpArmor->getBasicItem()->m_wEquippedShape;
	}

	addSendPacket((ALL_MSG*)&packet,_iSerial);
}

void
cFIELD::receiveObscurityActorList(CG_OBSCURITY_ACTOR_LIST *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;
	
	SG_VERY_SIMPLE_ACTOR_INFO_LIST	packet;

	int		iActorCount	=	0,i;
	int		iBufferPoint=	0;

	for (i=0;i<_lpPacket->wCount;i++)
	{
		int		iSerial			=	_lpPacket->aObscurityActor[i];
		cACTOR	*lpExistActor	=	getActor(iSerial);

		if (!lpExistActor)
		{
			lpActor->sendRemoveActor(iSerial);
			continue;
		}

		if (lpExistActor->m_wActorKind	==	eAK_PLAYER)
		{
			CVerySimplePlayerInfo	*lpSimpleData	=	(CVerySimplePlayerInfo *)&packet.aBuffer[iBufferPoint];

			lpExistActor->getVerySimplePlayerInfo(lpSimpleData);
			lpActor->correctInvisibleStatus(&lpSimpleData->dwEnchantedMask,lpExistActor);

			iBufferPoint	+=	sizeof(CVerySimplePlayerInfo);
		}
		else
		if (lpExistActor->m_wActorKind	==	eAK_MONSTER)
		{
			CVerySimpleMonsterInfo	*lpSimpleData	=	(CVerySimpleMonsterInfo	*)&packet.aBuffer[iBufferPoint];

			lpExistActor->getVerySimpleMonsterInfo(lpSimpleData);
			lpActor->correctInvisibleStatus(&lpSimpleData->dwEnchantedMask,lpExistActor);

			iBufferPoint	+=	sizeof(CVerySimpleMonsterInfo);
		}
		else
		{
			CVerySimpleNpcInfo	*lpSimpleData	=	(CVerySimpleNpcInfo*)&packet.aBuffer[iBufferPoint];

			lpExistActor->getVerySimpleNpcInfo(lpSimpleData);

			iBufferPoint	+=	sizeof(CVerySimpleNpcInfo);
		}

		if (iBufferPoint + sizeof(CVerySimplePlayerInfo) >= sizeof(packet.aBuffer))
			break;

		iActorCount++;
	}

	packet.wCount		=	iActorCount;
	packet.base.set(sizeof(packet)-sizeof(packet.aBuffer)+iBufferPoint,dSG_VERY_SIMPLE_ACTOR_INFO_LIST);

	addSendPacket((ALL_MSG*)&packet,_iSerial);
}

//
//	액터 데이터 보정(주로 클라이언트의 요구에 의해..)
void
cFIELD::receiveAskInfoActorList(CG_ASK_INFO_ACTOR_LIST *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	SG_SIMPLE_ACTOR_INFO_LIST	packet;

	int		iActorCount	=	0,i;
	int		iBufferPoint=	0;

	for (i=0;i<_lpPacket->wCount;i++)
	{
		int		iSerial			=	_lpPacket->aAskInfoActorList[i];
		cACTOR	*lpExistActor	=	getActor(iSerial);

		if	(!lpExistActor)
		{
			lpActor->sendRemoveActor(iSerial);
			continue;
		}
		
		if (lpExistActor->m_wActorKind	==	eAK_PLAYER)
		{
			CSimplePlayerInfo	*lpSimpleData	=	(CSimplePlayerInfo *)&packet.aBuffer[iBufferPoint];

			lpExistActor->getSimplePlayerInfo(lpSimpleData);
			lpActor->correctInvisibleStatus(&lpSimpleData->dwEnchantedMask,lpExistActor);

			iBufferPoint	=	iBufferPoint+sizeof(CSimplePlayerInfo) - sizeof(lpSimpleData->strName) + strlen(lpSimpleData->strName)+1;
		}
		else
		if (lpExistActor->m_wActorKind	==	eAK_MONSTER)
		{
			CSimpleMonsterInfo	*lpSimpleData	=	(CSimpleMonsterInfo	*)&packet.aBuffer[iBufferPoint];

			lpExistActor->getSimpleMonsterInfo(lpSimpleData);
			lpActor->correctInvisibleStatus(&lpSimpleData->dwEnchantedMask,lpExistActor);

			iBufferPoint	=	iBufferPoint+sizeof(CSimpleMonsterInfo) - sizeof(lpSimpleData->strName) + strlen(lpSimpleData->strName)+1;
		}
		else
		{
			CSimpleNpcInfo	*lpSimpleData	=	(CSimpleNpcInfo	*)&packet.aBuffer[iBufferPoint];

			lpExistActor->getSimpleNpcInfo(lpSimpleData);

			iBufferPoint	=	iBufferPoint+sizeof(CSimpleNpcInfo) - sizeof(lpSimpleData->strName) + strlen(lpSimpleData->strName)+1;
		}

		if (iBufferPoint + sizeof(CSimplePlayerInfo) >= sizeof(packet.aBuffer))
			break;

		iActorCount++;
	}

	packet.wCount		=	iActorCount;
	packet.base.set(sizeof(packet)-sizeof(packet.aBuffer)+iBufferPoint,dSG_SIMPLE_ACTOR_INFO_LIST);

	addSendPacket((ALL_MSG*)&packet,_iSerial);
}

//
//	운영자 명령 처리
void
cFIELD::receiveOperatorCommand(CG_OPERATOR_COMMAND *_lpPacket,int _iSerial)
{
	if	(!checkActor(_iSerial))
		return;

	cACTOR	*lpActor=	getActor(_iSerial);
	int		iLevel	=	lpActor->m_wOperatorLevel;

	if	(g_bIsInnerTestServer)
		iLevel	=	max(iLevel,eAL_OPERATOR);

	if	(iLevel	<	eAL_TESTER)
		return;

	switch(_lpPacket->wCommand)
	{
		case	eOC_GET_ITEM		:
		{
			g_game.sendBPGetItemInfo(lpActor,_lpPacket->strName);
			break;
		}
		case	eOC_HIDE			:
		{
			lpActor->m_bf1IsHide	=	1-lpActor->m_bf1IsHide;

			if	(lpActor->m_bf1IsHide)
				lpActor->sendServerMessage("hiding...",eSM_SYSTEM);
			else
				lpActor->sendServerMessage("cancel to hide status",eSM_SYSTEM);

			lpActor->addSendVerySimpleInfo();

			break;
		}
		case	eOC_REGEN_FIELD_MOB	:
		{
			if	(iLevel	>=	eAL_OPERATOR)
				regenAllMob();
			break;
		}

		case	eOC_SPRINKLE_GIFT			:
		{
			if	(g_bIsDuelServer)
				break;

			if	(_lpPacket->wX	==	1)
			{
				CDroppedItem	droppedItem;

				droppedItem.m_item.reset();
				droppedItem.m_item.m_wBaseItem	=	dITEM_MONEY;
				droppedItem.m_item.m_dwSerial	=	1;
				droppedItem.m_wOwner			=	0xffff;
				droppedItem.m_pos.x				=	lpActor->m_pos.x+random(40)-20;
				droppedItem.m_pos.y				=	lpActor->m_pos.y+random(40)-20;

				dropItem(&droppedItem);
			}
			else
			if	(lpActor->m_bf1IsEvilSpritOfChristmas)
				lpActor->sprinkleEventGift(10);
			break;
		}

		case	eOC_TRANS_TO_EVENT_MOB		:
		{
			if	(iLevel	>=	eAL_OPERATOR)
				lpActor->transToEventMob(_lpPacket->wX,_lpPacket->wY);
			break;
		}

		case	eOC_RETURN_TO_VILLAGE	:	//	마을로 돌아가기
		{
			g_pMoveFieldUserManager->booking(lpActor,NULL,lpActor->m_wLastVillage,0xffff);
			break;
		}

		case	eOC_CHANGE_LOCATE		:	//	위치 이동
		{
			if (isIncorrectPos(_lpPacket->wX,_lpPacket->wY) || !lpActor->stop(_lpPacket->wX,_lpPacket->wY,FALSE))
			{
				lpActor->sendCurrentPosition();
				break;
			}

			if	(lpActor->m_wLordOperator	!=	0xffff)
			{
				cACTOR	 *lpSlave	=	getTestedActor(lpActor->m_wLordOperator);

				if	(lpSlave)
				{
					lpSlave->stop();
					lpSlave->setActorPos(_lpPacket->wX,_lpPacket->wY);
					lpActor->sendCurrentPosition();
					return;
				}
			}

			lpActor->addPacketStop();
			lpActor->sendCurrentPosition();
			return;
		}

		case	eOC_MOVE_FIELD			:	//	출구로 이동
		{
			if	(lpActor->m_wCurrentField	==	_lpPacket->wX	)
				break;

			if	(_lpPacket->wX				>=	1024			)
				break;

			if	(g_game.m_aExistField[_lpPacket->wX]==	FALSE	)
			{
				lpActor->sendServerMessage("not exist field",eSM_NOTICE);
				break;
			}

			if (_lpPacket->wX	>=	990 && lpActor->m_wOperatorLevel < 4 && _lpPacket->wX	<	1000)
			{
				lpActor->sendServerMessage("can not move to guild battle field",eSM_NOTICE);
				break;
			}

			g_pMoveFieldUserManager->booking(lpActor,NULL,_lpPacket->wX,0);
			break;
		}

		case	eOC_RECALL				:	//	소환 - 누군가를 내 옆으로 소환
		{
			cACTOR	*lpTarget	=	g_game.getPlayer(_lpPacket->strName);

			if	(!lpTarget)
			{
				g_game.sendWPFindUser(lpActor->m_strName,_lpPacket->strName,lpActor->m_wCurrentField,eFUW_RECALL);
				break;
			}

			if	(lpActor->m_wCurrentField	>=	990 && lpActor->m_wCurrentField	<	1000)
			{
				lpActor->sendServerMessage("can not move to guild battle field",eSM_NOTICE);
				break;
			}

			if	(lpTarget->m_wCurrentField != lpActor->m_wCurrentField)
			{
				g_pMoveFieldUserManager->booking(lpTarget,NULL,lpActor->m_wCurrentField,0x7fff,lpActor->m_pos.x,lpActor->m_pos.y);
				break;
			}

			lpTarget->stop(lpActor->m_pos.x,lpActor->m_pos.y,FALSE);
			lpTarget->sendCurrentPosition();
			lpTarget->addPacketStop();
			break;
		}

		case	eOC_WARP				:	//	워프 - 누군가의 옆으로 이동
		{
			cACTOR	*lpTarget	=	g_game.getPlayer(_lpPacket->strName);

			if	(!lpTarget)
			{
				g_game.sendWPFindUser(lpActor->m_strName,_lpPacket->strName,lpActor->m_wCurrentField,eFUW_WARP);
				break;
			}

			if	(lpTarget->m_wCurrentField	>=	990 && lpTarget->m_wCurrentField	<	1000)
			{
				lpActor->sendServerMessage("can not move to guild battle field",eSM_NOTICE);
				break;
			}

			if	(lpTarget->m_wCurrentField != lpActor->m_wCurrentField)
			{
				g_pMoveFieldUserManager->booking(lpActor,NULL,lpTarget->m_wCurrentField,0x7fff,lpTarget->m_pos.x,lpTarget->m_pos.y);

				break;
			}

			lpActor->stop(lpTarget->m_pos.x,lpTarget->m_pos.y,FALSE);
			lpActor->sendCurrentPosition();
			lpActor->addPacketStop();
			break;
		}

		case	eOC_BANISH				:	//	추방 - 누군가를 게임 밖으로 쫏아냄
		{
#ifdef	_FOR_CHINA
			if	(iLevel	<	eAL_OPERATOR)
				break;
			printf("operate banish command - operator grade %d",iLevel);
#endif
			
			g_game.sendWPFindUser(lpActor->m_strName,_lpPacket->strName,lpActor->m_wCurrentField,eFUW_BANISH);
			break;
		}

		case	eOC_JAIL				:	//	감옥 - 누군가를 감옥으로 보냄
		{
#ifdef	_FOR_CHINA
			if	(iLevel	<	eAL_OPERATOR)
				break;
			printf("operate send jail command - operator grade %d",iLevel);
#endif
			cACTOR	*lpTarget	=	g_game.getPlayer(_lpPacket->strName);

			if	(!lpTarget)
			{
				g_game.sendWPFindUser(lpActor->m_strName,_lpPacket->strName,lpActor->m_wCurrentField,eFUW_RECALL);
				break;
			}
			g_pMoveFieldUserManager->booking(lpTarget,NULL,27,0xffff);

			break;
		}

		case	eOC_DRAWING_ROOM		:
		{
			cACTOR	*lpTarget	=	g_game.getPlayer(_lpPacket->strName);

			if	(!lpTarget)
			{
				g_game.sendWPFindUser(lpActor->m_strName,_lpPacket->strName,lpActor->m_wCurrentField,eFUW_RECALL);
				break;
			}
			g_pMoveFieldUserManager->booking(lpTarget,NULL,28,0xffff);

			break;
		}

		case	eOC_CONTROL_MONSTER		:
		{
#ifdef	_FOR_CHINA
			if (iLevel	<	eAL_OPERATOR)
				break;
#else
			if (iLevel	<	eAL_SYSTEM_OPERATOR)
				break;
#endif

			cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wX);
			cACTOR	*lpSlave	=	NULL;

			if	(!lpTarget || lpTarget->isPlayer())
			{
				if	(lpActor->m_wLordOperator!=	0xffff)
				{
					lpSlave					=	getTestedActor(lpActor->m_wLordOperator);

					SG_CONTROL_MONSTER	packet;

					packet.base.set(sizeof(SG_CONTROL_MONSTER),dSG_CONTROL_MONSTER);
					packet.wLord			=	0xffff;
					packet.wSlave			=	0xffff;

					g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

					if	(lpSlave)
					{
						lpSlave->m_wLordOperator	=	0xffff;
						lpSlave->stopBattleForNPC(TRUE);
					}

					lpActor->m_wLordOperator=	0xffff;
				}
				break;
			}

			if	(lpActor->m_wLordOperator!=	0xffff)
			{
				lpSlave		=	getTestedActor(lpActor->m_wLordOperator);

				if	(lpSlave)
					lpSlave->m_wLordOperator	=	0xffff;
			}

			lpTarget->stopBattleForNPC(TRUE);

			lpTarget->boostPower(120);
			lpTarget->m_wLordOperator	=	lpActor->m_wSerialInField;
			lpActor->m_wLordOperator	=	lpTarget->m_wSerialInField;

			SG_CONTROL_MONSTER	packet;

			packet.base.set(sizeof(SG_CONTROL_MONSTER),dSG_CONTROL_MONSTER);
			packet.wLord				=	lpActor->m_wSerialInField;
			packet.wSlave				=	lpTarget->m_wSerialInField;

			addSendPacket(&packet,lpActor->m_wSerialInField);
			lpActor->addSendSimpleInfo();

			break;
		}
	}
}

//	펫 이름 변경
void
cFIELD::receiveChangePetName(CG_CHANGE_PET_NAME *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;

	lpActor->changePetName(_lpPacket->wPetIndex,_lpPacket->strPetName,TRUE,_lpPacket->wIsMiniPet);
}

void
cFIELD::receiveRemovePrefix(CG_REMOVE_PREFIX *_lpPacket,int _iSerial)
{
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);

	if	(!lpAsker)	
		return;

	lpAsker->removePrefx(_lpPacket->wUseItem,_lpPacket->wDestItem,_lpPacket->wPrefixIndex);
}

void
cFIELD::receiveJudgeToItem(CG_JUDGE_TO_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);

	if	(!lpAsker)	
		return;

	lpAsker->judgeItem(_lpPacket->wItemSlot);
}

void
cFIELD::receiveGuildMemberWork(CG_GUILD_MEMBER_WORK *_lpPacket,int _iSerial)
{
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);

	if	(!lpAsker)
		return;

	switch(_lpPacket->wWork)
	{
		case	eGMW_RENT_GUILD_PET				:
		{
			lpAsker->askRentGuildPet(_lpPacket->awValue[0],_lpPacket->strMember);
			break;
		}

		case	eGMW_RETRIEVAL_RENTED_GUILD_PET	:
		{
			lpAsker->askRetrievalRentedGuildPet(_lpPacket->strMember);
			break;
		}
	
	}
}

void
cFIELD::receiveXTrapPacket(CG_XTRAP_PACKET *_lpPacket,int _iSerial)
{
#ifdef	_USE_XTRAP
	if	(!g_bIsUseXTrap)
		return;

	cACTOR	*lpAsker	=	getTestedActor(_iSerial);
	
	if	(!lpAsker)
		return;

	CClient	*lpClient	=	lpAsker->getClient();

	if	(!lpClient)
		return;

	XTrap_CS_Step3(lpClient->m_pXtrapBuffer,_lpPacket->aBuffer);
#endif
}

void
cFIELD::receiveEtcWork(CG_ETC_WORK *_lpPacket,int _iSerial)
{		// 마지막 수정일 : 09.11.03
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);

	if	(!lpAsker)
		return;

	g_debugSign.m_iTempStep	=	_lpPacket->wWork;

	switch(_lpPacket->wWork)
	{
		case	eCEW_TRANS_AVATAR_TO_GVG_SERVER		:
		{
			lpAsker->transAvatarToGVGServer();
			break;
		}

		case	eCEW_TRANS_GUILD_TO_GVG_SERVER		:
		{
			lpAsker->transGuildToGVGServer();
			break;
		}

		case	eCEW_CANCEL_TRY_DUEL				:
		{
			g_pDuelManager->cancel(lpAsker);
			break;
		}
		case	eCEW_ASK_DUEL_RECORD_INFO			:
		{
			if	(lpAsker->m_duelRecord.m_iBattleCount	==	-1)
				g_game.sendWPAskDuelRecord(lpAsker);
			else
				lpAsker->sendDuelRecord();
			break;
		}

		case	eCEW_CHANGE_MINIPET_PLACE			:
		{
			lpAsker->iMiniPet()->changeMiniPetPlace(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		}
		case	eCEW_RESET_PET_NAME		:
		{
			lpAsker->changePetName(_lpPacket->awValue[0],"noname",TRUE,FALSE,_lpPacket->awValue[1]);
			break;
		}
		case	eCEW_RESET_MINIPET_NAME					:
		{
			lpAsker->iMiniPet()->resetMiniPetName(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		}
		case	eCEW_USE_POLISHER						:
		{
			if(lpAsker->isCanNotUseItemToGVG(_lpPacket->awValue[0]))
			{
				sendUseItemResult(lpAsker->m_iClientSerial,lpAsker->m_wSerialInField,eUIR_CAN_NOT_USE_ITEM);
				break;
			}
			lpAsker->iMiniPet()->usePolisher(_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2]);
			break;
		}
		case	eCEW_USE_MINIPET_AWAKEN_ITEM						:
		{
			lpAsker->iMiniPet()->useAwakenItem(_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2]);
			break;
		}
		case	eCEW_USE_BLOCK_LIGHT_POCKET				:
		{
			lpAsker->iMiniPet()->useBlockLightPocket(_lpPacket->awValue[0]);
			break;
		}

		case	eCEW_USE_MINIPET_CHARGE_SKILL			:
		{
			lpAsker->iMiniPet()->useChargeSkill(getTestedActor(_lpPacket->awValue[0]),_lpPacket->awValue[1]);
			break;
		}

		case	eCEW_INCREASE_MINIPET_SKILL_LEVEL		:
		{
			lpAsker->iMiniPet()->increaseMiniPetSkillLevel(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		}
		case	eCEW_CHANGE_MINIPET_SKILL_ON_OFF		:
		{
			lpAsker->iMiniPet()->changeMinipetSkillOnOff(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		}
		case	eCEW_ACTIVATE_MINI_PET					:
		{
			if(lpAsker->m_wTransformationTime)
				break;
			lpAsker->iMiniPet()->activateMiniPet(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		}
		case	eCEW_SELECT_ITEM_IN_PACK				:
		{
			lpAsker->selectItemInPack(_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2]);
			break;
		}
		case	eCEW_ASK_ITEM_LIST_IN_ITEM_PACK			:
		{
			lpAsker->askItemListInItemPackForSelect(_lpPacket->awValue[0]);
			break;
		}
		case	eCEW_ADD_RELATED_PLACE_OF_GUILD			:
		{
			CGuild	*lpGuild	=	lpAsker->getGuild();

			if	(!lpGuild || lpGuild->m_wTryAddRelatedPlaceField	!=	_lpPacket->awValue[0])
				break;

			lpAsker->addRelatedPlace(_lpPacket->awValue[0]);
			break;
		}
		
		case	eCEW_CLOSE_RELATED_PLACE_LIST			:
		{
			lpAsker->m_bf1IsOpenRelatedPlaceOfGuild	=	FALSE;
			break;
		}
		case	eCEW_SELECT_RELATED_PLACE				:
		{
			lpAsker->warpToRelatedPlaceOfGuild(_lpPacket->awValue[0]);
			break;
		}
		case	eCEW_CHANGE_OPEN_GUILD_HALL_STATUS		:
		{
			lpAsker->askToggleOpenguildHallStatus();
			break;
		}
		case	eCEW_ACTIVE_GUILD_MASTER_GRACE			:
		{
			int nResult	=	lpAsker->askActiveGuildMasterGraceEffect();

			if(nResult >= eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR1)
			{
				SG_GUILD_MASTER_GRACE_INFO		packet;

				packet.base.set(sizeof(packet) , dSG_GUILD_MASTER_GRACE_INFO);
				packet.wWork	=	nResult;

				g_userPM.add(lpAsker->m_iClientSerial , &packet , packet.base.wSize);
			}
			break;
		}
		case	eCEW_FEED_ITEM_TO_MINIPET				:
		{
			lpAsker->iMiniPet()->feedItem(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		}
		case	eCEW_FEED_ITEM_TO_GUILD_PET				:
		{
			lpAsker->feedItemToGuildPet(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		}
		case	eCEW_SUMMON_GUILD_GUARDIAN				:
		{
			lpAsker->summonGuildGuardian(_lpPacket->awValue[0]);
			break;
		}
		case	eCEW_UNSUMMON_GUILD_GUARDIAN			:
		{
			lpAsker->unsummonGuildGuardian();
			break;
		}

		case	eCEW_RETURN_TO_THE_GUILD_HALL_BY_END_GUILD_POINT_BATTLE	:
		{
			lpAsker->returnToGuildHallByEndGuildPointBattle();
			break;
		}

		case	eCEW_RETURN_TO_GUILD_HALL_BY_END_BATTLE	:
		{
			lpAsker->returnToGuildHallByEndBattle();
			break;
		}

		case	eCEW_TRY_TO_GUILD_POINT_BATTLE			:
			lpAsker->tryToGuildPointBattle();
			break;
			
		case	eCEW_TRY_TO_GUILD_DUNGEN:
			lpAsker->tryToGuildDungeon(_lpPacket->awValue[0]);
			break;

		case	eCEW_SELECT_GUILD_HALL					:
		{
			if	(lpAsker->m_bf1IsOpenGuildHallList	==	FALSE)
				return;

			lpAsker->warpToGuildHall(_lpPacket->awValue[0]);

			break;
		}
		case	eCEW_STORE_GUILD_INVENTORY_ITEM			:	//	길드 아이템 보관
		{
			int	iInventorySlot		=	_lpPacket->awValue[0];
			int	iGuildInventorySlot	=	_lpPacket->awValue[1];

			lpAsker->storeGuildInventoryItem(iInventorySlot,iGuildInventorySlot);
			break;
		}
		case	eCEW_WITHDRAW_GUILD_INVENTORY_ITEM		:	//	길드 아이템 꺼내기
		{
			int	iInventorySlot		=	_lpPacket->awValue[0];
			int	iGuildInventorySlot	=	_lpPacket->awValue[1];

			lpAsker->withdrawGuildInventoryItem(iInventorySlot,iGuildInventorySlot);
			break;
		}
		case	eCEW_STORE_GUILD_INVENTORY_GOLD			:	//	길드 인벤에 골드 보관
		{
			int		iGold1	=	_lpPacket->awValue[0];
			int		iGold2	=	_lpPacket->awValue[1];
			int		iGold	=	(iGold1<<16)+iGold2;

			lpAsker->storeGuildInventoryGold(iGold);
			break;
		}
		case	eCEW_WITHDRAW_GUILD_INVENTORY_GOLD		:	//	길드 인벤에서 골드 꺼내기
		{
			int		iGold1	=	_lpPacket->awValue[0];
			int		iGold2	=	_lpPacket->awValue[1];
			int		iGold	=	(iGold1<<16)+iGold2;

			lpAsker->withdrawGuildInventoryGold(iGold);
			break;
		}
		case	eCEW_CHANGE_GUILD_INVENTORY_ITEM_PLACE	:	//	길드 인벤에 아이템 위치 변경
		{
			int		iSlot1	=	_lpPacket->awValue[0];
			int		iSlot2	=	_lpPacket->awValue[1];

			lpAsker->changeGuildInventoryItemPlace(iSlot1,iSlot2);
			break;
		}

		case	eCEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_ITEM_COUNT:	//	인덱스:직위:개수
		{
			if	(lpAsker->isGuildMaster()	==	FALSE)
				break;

			if	(_lpPacket->awValue[0]	>	4)
				break;
			if	(_lpPacket->awValue[1]	>	255)
				break;
			g_game.sendWPChangeGuildInventoryPermitWithdrawItemCount(lpAsker->m_wGuildSerial,_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2]);
			break;
		}
		case	eCEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_GOLD:		//	직위:골드
		{
			if	(lpAsker->isGuildMaster()	==	FALSE)
				break;

			int	iGold1	=	_lpPacket->awValue[1];
			int	iGold2	=	_lpPacket->awValue[2];
			int	iGold	=	(iGold1<<16)+iGold2;

			if	(_lpPacket->awValue[0]	>	4)
				break;

			//JBC	길드원 인출 가능 자금 버그 수정... 08-09-24
			if(iGold > 2000000000)
				break;
//			이전 버젼.
// 			if	(_lpPacket->awValue[1]	>	255)
// 				break;

			g_game.sendWPChangeGuildInventoryPermitWithdrawGold(lpAsker->m_wGuildSerial,_lpPacket->awValue[0],iGold);
			break;
		}
		case	eCEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE		:		//	인덱스:사이즈
		{
			if	(lpAsker->isGuildMaster()	==	FALSE)
				break;

			CGuild	*lpGuild	=	lpAsker->getGuild();

			int	iStorageSize	=	_lpPacket->awValue[0]+_lpPacket->awValue[1]+_lpPacket->awValue[2]+_lpPacket->awValue[3];

			if	(iStorageSize	!=	lpGuild->m_wGuildInventorySize)
				break;

			g_game.sendWPChangeGuildInventoryStorageSize(lpAsker->m_wGuildSerial,_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2],_lpPacket->awValue[3]);
			break;
		}

		case	eCEW_CLOSE_GUILD_HALL_LIST			:
		{
			lpAsker->m_bf1IsOpenGuildHallList	=	FALSE;
			break;
		}
		case	eCEW_CHANGE_MONSTER_INFO			:
		{
			changeMonsterInfo(lpAsker,_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2]);
			break;
		}

		case	eCEW_DISARM_TRAP					:
		{
			lpAsker->tryDisarmTrap(_lpPacket->awValue[0]);
			break;
		}
		case	eCEW_END_EVENT						:
		{
			lpAsker->m_bf1IsProcessEvent	=	FALSE;
			break;
		}
		case	eCEW_ASK_GUILD_BATTLE_SCHEDULE		:
		{
			lpAsker->sendGuildBattleInfoForMember(TRUE);
			break;
		}

		case	eCEW_ACTION_SKILL_TO_GROUND	:
		{
			if	(lpAsker->m_wRemainBlendingTime	==	0xffff)
				lpAsker->m_wRemainBlendingTime	=	10;

			char	strCommandLine[512];
			int		iValue	=	cMessSign::MakeCheckTableResult("test");

			sprintf(strCommandLine,"%s[%d]",g_strBuildVersion,iValue);

			lpAsker->sendServerMessage(strCommandLine,eSM_NOTICE);

			break;
		}

		case	eCEW_DISCONNECT_BCS					:
		{
			CClient	*lpClient	=	lpAsker->getClient();

			if	(lpClient)
				lpClient->SetBCSerial(0xffff);
			break;
		}

		case	eCEW_SELECT_GUILD_MARK		:
			lpAsker->askOpenGuildMarkComposer();
			break;

		case	eCEW_REMOVE_TITLE			:
			lpAsker->removeTitle(_lpPacket->awValue[0],TRUE);
			break;

		case	eCEW_USE_TITLE		:
			lpAsker->useTitle(_lpPacket->awValue[0]);
			break;

		case	eCEW_BUY_PIGEON_POST_ITEM	:
			lpAsker->buyPigeonPostItem(_lpPacket->awValue[0]);
			break;

		case	eCEW_USE_COUPLE_RING		:
			lpAsker->useCoupleRing(_lpPacket->awValue[0]);
			break;

		case	eCEW_FIRE_FIRE_WORK			:
			g_fireWorkManager.fireFireWork(lpAsker,_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2],_lpPacket->awValue[3],_lpPacket->awValue[4]);
			break;

		case	eCEW_CHANGE_ISPI_SETTING	:
			lpAsker->changeISPISetting(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;

		case	eCEW_USE_BOTTOMLESS_BOX		:
			{
				if(lpAsker->isCanNotUseItemToGVG(_lpPacket->awValue[0]))
				{
					sendUseItemResult(lpAsker->m_iClientSerial,lpAsker->m_wSerialInField,eUIR_CAN_NOT_USE_ITEM);
					break;
				}
				lpAsker->useBottomlessBox(_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2]);
				break;
			}
			
		case	eCEW_INCREASE_TITLE_LEVEL_BY_CH5	:
			lpAsker->increaseTitleLevelByCH5(_lpPacket->awValue);
			break;
		case	eCEW_CH5_RELEASE_ITEM_REVERSION		:
			lpAsker->releaseItemReversion(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		case	eCEW_CH5_RESET_ONE_STATE			:
			lpAsker->resetOneState(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;
		case	eCEW_CH5_RESET_ONE_SKILL			:
			lpAsker->resetOneSkill(_lpPacket->awValue[0],_lpPacket->awValue[1]);
			break;

		case	eCEW_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP	:
			lpAsker->levelUpPrefix(_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2],FALSE);
			break;

		case	eCEW_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP	:
			lpAsker->levelUpPrefix(_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2],TRUE);
			break;

		case	eCEW_CH5_NORMAL_ITEM_UPGRADE	:
			lpAsker->upgradeItem(_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2],FALSE);
			break;
		case	eCEW_CH5_UNIQUE_ITEM_UPGRADE	:
			lpAsker->upgradeItem(_lpPacket->awValue[0],_lpPacket->awValue[1],_lpPacket->awValue[2],TRUE);
			break;

		case	eCEW_CH5_SELECT_ALLIGNMENT		:
			lpAsker->selectAllignment(_lpPacket->awValue[0],(short)_lpPacket->awValue[1]);
			break;
			
		case	eCEW_ASK_DUEL_RANKING			:
			lpAsker->sendDuelRanking(_lpPacket->awValue[0]);
			break;

		case	eCEW_DISJOINTING_ITEM	:
			lpAsker->disJointingItem(_lpPacket->awValue[0]);
			break;
		case	eCEW_FEED_ALL_ITEM_TO_MINIPET	:
			lpAsker->iMiniPet()->feedAllItem(_lpPacket->awValue[0],_lpPacket->awValue[1] ,_lpPacket->awValue[2]);
			break;
		case	eCEW_MIX_MINIPET				:
			{
				if(lpAsker->isCanNotUseItemToGVG(_lpPacket->awValue[0]))
				{
					sendUseItemResult(lpAsker->m_iClientSerial,lpAsker->m_wSerialInField,eUIR_CAN_NOT_USE_ITEM);
					break;
				}
				lpAsker->iMiniPet()->mixMiniPet(_lpPacket->awValue[0],&_lpPacket->awValue[1]);
				break;
			}
		case	eCEW_RESTRAINT_MINIPET			:
			{
				if(lpAsker->isCanNotUseItemToGVG(_lpPacket->awValue[0]))
				{
					sendUseItemResult(lpAsker->m_iClientSerial,lpAsker->m_wSerialInField,eUIR_CAN_NOT_USE_ITEM);
					break;
				}
				lpAsker->iMiniPet()->restraintMiniPet(_lpPacket->awValue[0],_lpPacket->awValue[1]);
				break;
			}
		case eCEW_BUY_GOLD_BAR				:		//   금괴구입함 ~! 09.08.28
			{
				int			iCount		=	_lpPacket->awValue[0];
				int			iTax		=	lpAsker->getTitleBankClientTax();
				LONGLONG	llGold		=	100000000 * iCount;
				LONGLONG	llTaxGold	=	(llGold * (iTax/10.0)/100.0);
				LONGLONG	llBarPrice	=	llGold + llTaxGold;

				if	(iCount > 20 || llBarPrice < 0 || llBarPrice	>	lpAsker->m_iGold || lpAsker->getRemainInventorySlotCount() == 0)
					break;	//	금괴 구매 비용이 소지금 보다 비싸거나 빈 인벤토리가 없다면 브레이크를 건다. 이런 경우가 발생한다면... 패킷 해킹이지-_-

				lpAsker->m_iGold -= llBarPrice;

				cItem	itemAddGoldBar;

				g_im.generateItemByBaseItem(&itemAddGoldBar,dITEM_INDEX_GOLD_BAR,iCount);

				lpAsker->sendBuyItemLog(&itemAddGoldBar,llBarPrice,iCount,eBUY_ITEM_IN_BANK_GOLDBAR);
				lpAsker->sendEtcWork(eEW_BUY_GOLD_BAR_MESSAGE);
				lpAsker->addGoldItemToInventory(&itemAddGoldBar,eGET_GOLD_IGNORE);
				lpAsker->immediatelySendSaveDataToDBMemory();		// 09.09.18 데이터베이스 저장..+_+;;

				lpAsker->updateGoldLog(-llTaxGold,eSPENT_GOLD_BY_MAKE_GOLDBAR_CHARGE);
				
				break;
			}
		case eCEW_ETERNAL_ITEM_UPGRADE	:		// 이터널 무기 강화 09.09.08		
			{
				int iWeaponSlot				= _lpPacket->awValue[0];
				int iUpgradeItemSlot		= _lpPacket->awValue[1];
				int iUpgradeNormalItemSlot	= _lpPacket->awValue[2];
				BOOL	iUpgradePerfect		= _lpPacket->awValue[3];

				lpAsker->upgradeEternalItem(iWeaponSlot, iUpgradeItemSlot, iUpgradeNormalItemSlot, iUpgradePerfect);
				break;
			}

		case eCEW_REALLY_ITEM_UPGRADE	:		// 강화할껀데? 재료좀.. 
			{
				int iCurrentItemLevel = _lpPacket->awValue[0];
				
				int iUpgradeMaterialsGold				=	lpAsker->getUpgradeMaterialsGold(iCurrentItemLevel) / 100000;		
				int iUpgradeMaterialsStoneOfMystery		=	lpAsker->getUpgradeMaterialsStoneOfMystery(iCurrentItemLevel);
				int iUpgradeMaterialsTantilless			=	lpAsker->getUpgradeMaterialsTantilless(iCurrentItemLevel);	
				int iUpgradeMaterialsCrystal			=	lpAsker->getUpgradeMaterialsCrystal(iCurrentItemLevel);		
#ifdef _FOR_JAPAN

				iUpgradeMaterialsGold				=	lpAsker->getUpgradeMaterialsGoldForJapan(iCurrentItemLevel) / 100000;		
				iUpgradeMaterialsStoneOfMystery		=	lpAsker->getUpgradeMaterialsStoneOfMysteryForJapan(iCurrentItemLevel);
				iUpgradeMaterialsTantilless			=	lpAsker->getUpgradeMaterialsTantillessForJapan(iCurrentItemLevel);	
				iUpgradeMaterialsCrystal			=	lpAsker->getUpgradeMaterialsCrystalForJapan(iCurrentItemLevel);		
#endif
				
				lpAsker->sendEtcWork(eEW_REALLY_ITEM_UPGRADE, iUpgradeMaterialsGold, iUpgradeMaterialsStoneOfMystery, iUpgradeMaterialsTantilless, iUpgradeMaterialsCrystal);
				break;
			}
		case eCEW_DRAG_ITEM_DIVIDE:				 // 09.09.18
			{//	아이템 나누기.
				int iDragItemSlot	= _lpPacket->awValue[0];
				int iFocusItemSlot	= _lpPacket->awValue[1];
				int iItemCount		= _lpPacket->awValue[2];
				
				
				cItem* lpDragItem	= lpAsker->getInventoryItem(iDragItemSlot);
				cItem* lpFocusItem =  lpAsker->getInventoryItem(iFocusItemSlot);

				if(!lpDragItem)
					break;		
				
				if(lpFocusItem)
					break;

				if(lpDragItem->isExceptionItem())
					break;
				
				if(lpDragItem->m_bCount<2)
					break;
				
				if(lpDragItem->m_bCount<=iItemCount)
					break;
				WORD	wBeforDragCount = lpDragItem->m_bCount;
				WORD	wAfterDragCount	= 0;
				WORD	wFocusCount		= iItemCount;
				cItem	copyItem;
				
				g_im.generateItemByBaseItem(&copyItem,lpDragItem->m_wBaseItem,iItemCount);	
				memcpy(copyItem.m_aOption, lpDragItem->m_aOption , sizeof(copyItem.m_aOption));
				memcpy(copyItem.m_aPrefix, lpDragItem->m_aPrefix , sizeof(copyItem.m_aPrefix));
				lpDragItem->decreaseCount(iItemCount);

				wAfterDragCount	=	lpDragItem->m_bCount;
				
				if	(iFocusItemSlot	>=	dOWN_ITEM_COUNT)
				{					
					lpAsker->m_aExtraInventory[iFocusItemSlot-dOWN_ITEM_COUNT].copy(&copyItem);
				}
				else
				{
					lpAsker->m_aItems[iFocusItemSlot].copy(&copyItem);
				}
				
				cBasicItem* lpBasic = lpDragItem->getBasicItem();

				if((lpBasic && lpBasic->isGoldBar()) || lpDragItem->isPremiumItem())
				{	// 금괴 이거나 유료 아이템 이면 .. 로그를 남긴다.
					lpAsker->sendDivideItemLog(lpDragItem,wBeforDragCount, wAfterDragCount, wFocusCount);
				}

				lpAsker->sendEtcWork(eEW_DRAG_ITEM_DIVIDE, iDragItemSlot, iFocusItemSlot, iItemCount);


				break;
			}
		case	eCEW_USE_SHOP :
			{
				lpAsker->sendOpenBadgeOfTraderShop();
				break;
			}
		case	eCEW_USE_BANK :
			{
				
				if	(lpAsker->m_lpField->m_bIsGuildPointBattleField	||	lpAsker->m_lpField->m_bIsSiegeWarfareField || lpAsker->m_lpField->m_bIsGuildBattleField)	//	길드전이나..포인트전.. 공성전때는 사용할수없다.
				{
					SG_USE_ITEM_RESULT	sgPacket;
					sgPacket.base.set(sizeof(sgPacket),dSG_USE_ITEM_RESULT);
					sgPacket.wTarget	=	lpAsker->m_wSerialInField;
					sgPacket.wResult	=	eUIR_CAN_NOT_USE_THIS_FIELD;
					g_userPM.add(lpAsker->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
					break;
				}
				lpAsker->askOpenBank();
				break;
			}

		case	eCEW_COPY_PREFIX :
			{
				lpAsker->copyPrefix( _lpPacket->awValue[0], _lpPacket->awValue[1], _lpPacket->awValue[2]);
				break;
			}
		case	eCEW_CUT_PREFIX :
			{
				lpAsker->cutPrefix( _lpPacket->awValue[0], _lpPacket->awValue[1], _lpPacket->awValue[2]);
				break;
			}
		case	eCEW_MINIPET_FEED :
			{
				lpAsker->iMiniPet()->feedItem(_lpPacket->awValue[0],_lpPacket->awValue[1]);
				break;
			}
		case	eCEW_RESET_MINIPET_SKILL :
			{
				if(lpAsker->isCanNotUseItemToGVG(_lpPacket->awValue[1]))
				{
					sendUseItemResult(lpAsker->m_iClientSerial,lpAsker->m_wSerialInField,eUIR_CAN_NOT_USE_ITEM);
					break;
				}
				lpAsker->iMiniPet()->resetSkill( _lpPacket->awValue[0],_lpPacket->awValue[1]);
				break;
			}	
		case	eCEW_SET_MINIPET_TYPE :
			{
				if(lpAsker->isCanNotUseItemToGVG(_lpPacket->awValue[1]))
				{
					sendUseItemResult(lpAsker->m_iClientSerial,lpAsker->m_wSerialInField,eUIR_CAN_NOT_USE_ITEM);
					break;
				}
				lpAsker->iMiniPet()->changeType( _lpPacket->awValue[0],_lpPacket->awValue[1]);
				break;
			}	
		
	}
}

void
cFIELD::receiveMergeItem(CG_MERGE_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);

	if	(!lpAsker)	
		return;

	lpAsker->mergeItem(_lpPacket->wItemSlot);
}

void
cFIELD::warpFieldByGateGlove(CG_WARP_FIELD_BY_GATE_GLOVE *_lpPacket,int _iSerial , BOOL _isNetCafeUser)
{
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);

	if	(!lpAsker	||	lpAsker->m_wCurrentField	==	27 || lpAsker->m_lpField->m_wIsBlockToWarpField)	
		return;

	if	(lpAsker->getRestraintTimeByBattle())
	{
		lpAsker->sendEtcWork(eEW_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE,lpAsker->getRestraintTimeByBattle());

		return;
	}

	lpAsker->warpFieldByGateGlove(_lpPacket->wDestField , _isNetCafeUser,_lpPacket->iItemSlot);
}

void
cFIELD::receiveAskActorEnchantInfo(CG_ASK_ACTOR_ENCHANT_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpAsker	=	getTestedActor(_iSerial);

	if	(!lpAsker)	
		return;
	
	for	(int	i=0;i<_lpPacket->wCount;i++)
	{
		int	iSerial			=	_lpPacket->awData[i][0];
		int	iWork			=	_lpPacket->awData[i][1];

		switch(iWork)
		{
			case	eAEI_SHAKLE_SKILL			:
			{
				cACTOR	*lpTarget	=	getActor(iSerial);

				if	(!lpTarget)
				{
					lpAsker->sendRemoveActor(iSerial);
					return;
				}

				if	(lpTarget->m_iShakleHP	==	0)
					break;

				lpAsker->sendEtcWork(eEW_SHAKLE_SKILL,iSerial,lpTarget->m_bf10ShakleSkill);
				break;
			}

			case	eAEI_BITE_HANGER			:
			{
				cACTOR	*lpTarget	=	getActor(iSerial);

				if	(!lpTarget)
				{
					lpAsker->sendRemoveActor(iSerial);
					return;
				}

				cACTOR	*lpBiter	=	getActor(lpTarget->m_wBiter);

				if	(!lpBiter)
					break;

				lpAsker->sendEtcWork(eEW_BITE_HANGER,iSerial,lpBiter->m_wBiter);
				break;
			}

			case	eAEI_THE_OTHER_SELF_COUNT	:
			{
				cACTOR	*lpTarget	=	getActor(iSerial);

				if	(!lpTarget)
				{
					lpAsker->sendRemoveActor(iSerial);
					return;
				}

				lpAsker->sendEtcWork(eEW_THE_OTHER_SELF_COUNT,iSerial,lpTarget->m_wTheOtherSelfCount);
				break;
			}
		}
	}
}

void
cFIELD::receiveGGAuthData(CG_GG_AUTH_DATA *_lpPacket,int _iSerial)
{
#ifdef	_USE_NPGL
	if (g_bIsUseNPGL	==	FALSE)
		return;

	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)	
		return;

	lpActor->m_iLastSendGGAuthCodeTime	=	g_iCounterBySecond;

	CClient	*lpClient	=	lpActor->getClient();

	if	(!lpClient)
		return;

	memcpy(&lpClient->m_csa.m_AuthAnswer,_lpPacket->aBuffer,sizeof(GG_AUTH_DATA));

	DWORD	dwGGErrorCode	=	lpClient->m_csa.CheckAuthAnswer();

	if (dwGGErrorCode	!=	ERROR_SUCCESS)
	{
		if (g_bIsDisconnectGGAuthFailedUser)
		{
			printf("Disconnect GG Auth Failed User [%s/%s] \n",lpActor->m_strId,lpActor->m_strName);
			printf("    by Error Code [#%.3d]\n",dwGGErrorCode);
			CLOG("GG Auth","Entry Step : Disconnect GG Auth Failed User [%s/%s] by Error Code [#%.3d]",lpActor->m_strId,lpActor->m_strName,dwGGErrorCode);

			lpActor->bookingReadyToExitGame();
		}
		//	짤라
	}
#endif
}

void
cFIELD::receivePartingWithPet(CG_PARTING_WITH_PET *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)	
		return;

	if	(lpActor->isSealdAction())
		return;

	lpActor->partingWithPet(_lpPacket->bf10PetIndex,_lpPacket->bf6Type);
}

//
//	마을로 돌아가기
void
cFIELD::receiveReturnToVillage(CG_RETURN_TO_VILLAGE *_lpPacket,int _iSerial)
{
	cACTOR		*lpActor		=	getTestedActor(_iSerial);

	if	(!lpActor)	
		return;

	if	(m_bIsGuildBattleField == FALSE && lpActor->isSealdAction())
		return;
	
	if	(m_iUniqueSerial	==	27	&&	lpActor->m_wOperatorLevel	<	2)
		return;

	if	(m_wDuelType	!=	0xffff && m_wDuelMatchStep == eDUEL_MATCH_STEP_BATTLE)
		return;

	if	(isCanNotUseTeleportEffectField()	&&	_lpPacket->wWork	!=	eRTV_BY_REVIVE)
	{
		lpActor->sendRegistServerMessage(eRSM_CAN_NOT_TELEPORT_FIELD,eSM_SYSTEM);

		return;
	}
	
	if	(lpActor->isDeath())
		lpActor->m_bf1IsMoveFieldByDeath	=	TRUE;
	else
	if (_lpPacket->wWork	==	eRTV_BY_END_OF_GUILD_BATTLE)
		lpActor->m_iHP				=	lpActor->getMaxHP();
	else
	if	(lpActor->m_wPremiumServiceLevel==	ePSL_NORMAL && _lpPacket->wWork	!=	eRTV_BY_END_OF_EVENT && lpActor->m_bf1IsExistExceptExpPortal	==	FALSE)
		lpActor->m_iHP				=	1;

	if	(_lpPacket->wWork	==	eRTV_BY_ITEM)
	{
		if	(lpActor->m_wPremiumServiceLevel < ePSL_LITE_PLATINUM && lpActor->m_bf1IsExistExceptExpPortal	==	FALSE)
			return;

		if	(lpActor->getRestraintTimeByBattle())
		{
			lpActor->sendEtcWork(eEW_CAN_NOT_RETURN_TO_VILLAGE_DURING_BATTLE,lpActor->getRestraintTimeByBattle());

			return;
		}

		lpActor->m_bf1IsMoveFieldByTeleport	=	TRUE;
	}

	if	(_lpPacket->wWork	==	eRTV_BY_END_OF_EVENT)
	{
		if	(isEventField()	==	FALSE)
			return;

		lpActor->m_bf1IsMoveFieldByTeleport	=	TRUE;
	}

	if	(m_bIsGuildBattleField)
	{
		if	(_lpPacket->wWork	==	eRTV_BY_ITEM)
		{
			lpActor->sendUseOrbResult(eUOR_CAN_NOT_USE_AT_THIS_FIELD);

			return;
		}

		if	(m_wDuelType	==	0xffff	&&	lpActor->getPartySerial()	!=	0xffff)
		{
			cParty	*lpParty	=	lpActor->getParty();

			if	(lpParty)
			{
				CLOG("party","leave party by receiveReturnToVillage [%s]",lpActor->m_strName);
				g_game.sendWPPartyWork(lpActor->getPartySerial(),ePW_LEAVE_PARTY,lpActor->m_strName);
			}
		}

		//JBC	엑터가 죽으면 리셋시켜주는 부분. 08-09-01
		lpActor->cureWrongStatus();
// 		lpActor->m_wReversalPowerTime = 0;
// 		lpActor->m_bf1IsMoveFieldByTeleport	=	TRUE;
	}

	if	(lpActor->isDeath()	&&	lpActor->m_wLastVillage	==	m_iUniqueSerial)
	{
		//JBC	엑터가 죽으면 리셋시켜주는 부분. 08-09-01
		lpActor->cureWrongStatus();
// 		lpActor->m_wReversalPowerTime = 0;
// 		lpActor->m_bf12DeathPeneltySecond	=	0;
		lpActor->m_iHP	=	lpActor->getMaxHP();
		lpActor->m_iDeathCount	=	0;
		lpActor->addSendRevive();
	}

	//lpActor->m_wReversalPowerTime = 0;
	g_pMoveFieldUserManager->booking(lpActor,NULL,lpActor->m_wLastVillage);
}

//
//	위치가 잘못된(?) 액터 리스트를 받았다.
void
cFIELD::receiveMisplacedActorList(CG_MISPLACED_ACTOR_LIST *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	SG_REFIT_ACTORS_POSITION	packet;
	int		i;

	packet.base.set(0,dSG_REFIT_ACTORS_POSITION);
	packet.wCount	=	0;

	if	(_lpPacket->base.wSize	!=	dCG_MISPLACED_ACTOR_LIST)
	{
		if	(g_wBuffUpkeepTime	==	0xffff)
			lpActor->m_wRemainShakeTime	=	10;

		return;
	}

	for (i=0;i<_lpPacket->wCount;i++)
	{
		int	iSerial	=	_lpPacket->aMisplacedActorList[i];

		cACTOR	*lpExistActor	=	getActor(iSerial);

		if (!lpExistActor)
		{
			lpActor->sendRemoveActor(iSerial);
			continue;
		}

		CActorInfoForOuterActor	*lpInfo	=	&packet.aInfoList[packet.wCount];

		lpExistActor->getActorInfoForOuterActor(lpInfo);

		packet.wCount++;

		if (packet.wCount	>=	20)
		{
			addSendPacket((ALL_MSG*)&packet,_iSerial);

			packet.wCount	=	0;
			break;
		}
	}

	if(packet.wCount)
	{
		int	iSize		=	sizeof(SG_REFIT_ACTORS_POSITION)-sizeof(packet.aInfoList)+packet.wCount*sizeof(CActorInfoForOuterActor);

		packet.base.set(iSize,dSG_REFIT_ACTORS_POSITION);

		addSendPacket((ALL_MSG*)&packet,_iSerial);
	}
}

void
cFIELD::receiveAskPlayerInfo(CG_ASK_PLAYER_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->sendPlayerInfo(lpActor);
}

void
cFIELD::receiveCheckPlayerCheckSum(CG_CHECK_PLAYER_CHECK_SUM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(_lpPacket->dwCheckSum	!=	lpActor->getCheckSumForClientData())
	{
#ifdef	_FOR_KOREA
		printf("mismatch check sum %s/%s\n",lpActor->m_strId,lpActor->m_strName);
#endif
		lpActor->sendPlayerInfo(lpActor);
	}

	lpActor->sendSuccesToJoin();
}

void
cFIELD::receiveBuyCarrotShopItem(CG_BUY_CARROT_SHOP_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	lpActor->askBuyCarrotShopItem(_lpPacket->wCategory,_lpPacket->wItemIndex);
}

void
cFIELD::receiveTouchEventArea(CG_TOUCH_EVENT_AREA *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;

	cAreaInfo	*lpEventArea	=	getArea(_lpPacket->wArea);

	if	(!lpEventArea || lpEventArea->m_wKind	!=	dAREA_EVENT_AREA	||	!lpEventArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
	{
		lpActor->sendRegistServerMessage(eRSM_TOUCH_EVENT_AREA_FAILED);
		return;
	}
	
	lpActor->sendRegistServerMessage(eRSM_TOUCH_EVENT_AREA_SUCCESS);

	lpActor->m_wCurrentClickObject	=	lpEventArea->m_wSerial;

	lpEventArea->operateKarma(lpActor);

	lpActor->m_wLastClickObject		=	lpEventArea->m_wSerial;
}

void
cFIELD::receiveAskArcaInfo(CG_ASK_ARCA_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	SG_ARCA_LIST		packet;

	packet.base.set(sizeof(packet),dSG_ARCA_LIST);
	packet.bf8Count		=	0;
	packet.bf1IsFirst	=	FALSE;

	for (int i=0;i<_lpPacket->wCount;i++)
	{
		int		iArcaSerial	=	m_awArcaList[i];

		if	(iArcaSerial	==	0xffff)
			continue;

		cArca	*lpArca	=	g_arcaManager.get(iArcaSerial);

		packet.aArcaList[packet.bf8Count].m_wSerial				=	iArcaSerial;

		if	(!lpArca	||	lpArca->getField()	!=	this)
			packet.aArcaList[packet.bf8Count].m_wX	=	0;
		else
		{
			packet.aArcaList[packet.bf8Count].m_wX				=	lpArca->m_pos.x;
			packet.aArcaList[packet.bf8Count].m_wY				=	lpArca->m_pos.y;
			packet.aArcaList[packet.bf8Count].m_bf8SecretLevel	=	lpArca->m_bSecretLevel;
			packet.aArcaList[packet.bf8Count].m_bf1IsDestroyAble=	lpArca->m_bf1IsDestroyAble;
			packet.aArcaList[packet.bf8Count].m_bf1IsLocked		=	FALSE;

			if (lpArca->m_bLockLevel)
				packet.aArcaList[packet.bf8Count].m_bf1IsLocked	=	TRUE;
		}

		packet.bf8Count++;
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aArcaList)+sizeof(cArcaSimpleInfo)*packet.bf8Count;

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cFIELD::receiveSearchTradeStationItem(CG_SEARCH_RELAY_STATION_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	SG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION	packet;

	packet.base.set(sizeof(packet),dSG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION);

	g_pitchmanShopManager.serchItem(_lpPacket,&packet);

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.m_aItemList)+sizeof(CPitchmanShopItemInfoForClient)*packet.wCount;

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cFIELD::receiveAskBannerText(CG_ASK_BANNER_TEXT *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	cBanner	*lpBanner	=	g_bannerManager.get(_lpPacket->wBannerSerial);

	if	(!lpBanner)
	{
		lpActor->sendEtcWork(eEW_REMOVE_BANNER,_lpPacket->wBannerSerial);

		return;
	}

	if	(lpBanner->m_lpField->m_iUniqueSerial	!=	m_iUniqueSerial)
	{
		lpActor->sendEtcWork(eEW_REMOVE_BANNER,_lpPacket->wBannerSerial);

		return;
	}

	lpActor->sendBannerText(_lpPacket->wBannerSerial,lpBanner->m_strText);
}

void
cFIELD::receivePutBanner(CG_PUT_BANNER *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	cItem	*lpItem		=	lpActor->getInventoryItem(_lpPacket->wBannerItemSlot);

	if	(!lpItem)
	{
		lpActor->sendEtcWork(eEW_FAILED_TO_PUT_BANNER);
		return;
	}

	BOOL	bIsHorizonBanner	=	lpItem->isHorizonBanner();
	BOOL	bIsVerticalBanner	=	lpItem->isVerticalBanner();

	if	(!bIsHorizonBanner	&&	!bIsVerticalBanner)
	{
		lpActor->sendEtcWork(eEW_FAILED_TO_PUT_BANNER);
		return;
	}

	if	(isExistCrushBanner(_lpPacket->wX,_lpPacket->wY))
	{
		lpActor->sendEtcWork(eEW_FAILED_TO_PUT_BANNER_BY_TO_CLOSE);
		return;
	}

	if	(isSecretDungeon()	||	m_bIsGuildBattleField)
	{
		lpActor->sendEtcWork(eEW_FAILED_TO_PUT_BANNER_BY_INCORRECT_FIELD);
		return;
	}

	int	iSerial	=	g_bannerManager.generate(this,_lpPacket->wX,_lpPacket->wY,lpItem->m_aOption[0],bIsVerticalBanner,_lpPacket->strTitle,_lpPacket->strComment);

	if	(iSerial	==	0xffff)
	{
		lpActor->sendEtcWork(eEW_FAILED_TO_BANNER_FULL);
		return;
	}

	lpItem->decreaseCount(1);
	lpActor->sendEtcWork(eEW_SUCCESS_TO_PUT_BANNER,_lpPacket->wBannerItemSlot);
}

void
cFIELD::receiveDisarmFieldTrap(CG_DISARM_FIELD_TRAP *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
	{
		lpActor->sendFieldTrapWork(_lpPacket->wFieldTrap,eFTW_FAILED_BY_BAD_STATUS);
		return;
	}

	cAreaTrapInfo	*lpTrapArea	=	(cAreaTrapInfo	*)getArea(_lpPacket->wFieldTrap);

	if	(!lpTrapArea)
	{
		lpActor->sendFieldTrapWork(_lpPacket->wFieldTrap,eFTW_FAILED_BY_VALID_TRAP);
		return;
	}

	lpTrapArea->tryDisarm(lpActor);
}

void
cFIELD::receiveAskDoorInfo(CG_ASK_DOOR_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	cAreaDoorInfo	*lpDoorArea	=	(cAreaDoorInfo	*)getArea(_lpPacket->wDoorArea);

	if (!lpDoorArea || lpDoorArea->m_wKind	!=	dAREA_DOOR)
		return;

	lpDoorArea->sendDoorStatus();
}

void
cFIELD::receiveArcaWork(CG_ARCA_WORK *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if (lpActor->isSealdAction())
	{
		lpActor->sendArcaWork(_lpPacket->wArca,eAW_FAILED_BY_BAD_STATUS);
		return;
	}

	if	(_lpPacket->wArca	==	0x7fff)
	{
		lpActor->openTreasureBox();

		return;
	}

	cArca	*lpArca	=	g_arcaManager.get(_lpPacket->wArca);

	if	(!lpArca)
	{
		removeActiveArca(_lpPacket->wArca);

		lpActor->sendArcaWork(_lpPacket->wArca,eAW_IS_FAILED);

		return;
	}

	if	(lpArca->getField()	!=	this)
	{
		lpActor->sendArcaWork(_lpPacket->wArca,eAW_IS_FAILED);

		return;
	}

	if	(lpArca->m_wTrapLevel)
	{
		if (_lpPacket->wWork	==	eAAW_ATTACK || !lpActor->isVisibleTrap(lpArca->m_wDetectTrapLevel))
		{
			lpArca->explosionTrap(lpActor);

			lpActor->addSendCloseRangeArcaWork(_lpPacket->wArca,eAW_EXPLOSION_TRAP_BY_NOT_DETECTED,0xffff,lpActor->m_strName);

			return;
		}

		if (_lpPacket->wWork	!=	eAAW_DISARM_TRAP)
		{
			lpActor->sendArcaWork(_lpPacket->wArca,eAW_IS_TRAP);

			return;
		}
	}

	switch(_lpPacket->wWork)
	{
		case	eAAW_OPEN		:
			g_arcaManager.open(_lpPacket->wArca,lpActor);
			break;
			
		case	eAAW_DISARM_TRAP:
			g_arcaManager.disarmTrap(_lpPacket->wArca,lpActor);
			break;

		case	eAAW_UNLOCK		:
			g_arcaManager.tryUnlock(_lpPacket->wArca,lpActor,FALSE);
			break;

		case	eAAW_UNLOCK_BY_USE_KEY:
			g_arcaManager.tryUnlock(_lpPacket->wArca,lpActor,TRUE);
			break;
			
		case	eAAW_ATTACK		:
			g_arcaManager.attack(_lpPacket->wArca,lpActor);
			break;
	}
}

void
cFIELD::receiveChangeDoorStatus(CG_CHANGE_DOOR_STATUS* _lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if (_lpPacket->wArea	>=	dMAX_AREA)
		return;

	if (_lpPacket->wMethod	>=	eCDSM_COUNT)
		return;

	if (lpActor->isSealdAction())
		return;

	cAreaInfo	*lpArea		=	getArea(_lpPacket->wArea);

	if	(!lpArea || lpArea->m_wSerial	==	0xffff	||	!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
		return;

	if	(lpArea->m_wKind		!=	dAREA_DOOR)
		return;

	if	(lpArea->m_cc.m_wConversationCount)
	{
		lpArea->operateKarma(lpActor);
		
		lpActor->sendDoorMessage(lpArea->m_wSerial,0xffff,0xffff,eDWM_DUMMY);

		return;
	}

	cAreaDoorInfo	*lpDoor	=	(cAreaDoorInfo	*)lpArea;

	lpDoor->operatePlayerRequest(_lpPacket->wMethod,lpActor);
}

//
//	전투 종료
void
cFIELD::stopBattle(CG_STOP_BATTLE *_lpPacket,int _iSerial)
{
	if (!checkPlayerAndKill(_iSerial))
		return;

	getActor(_iSerial)->stopBattle();
}

//
//	스탯 증가
void
cFIELD::increaseCharacterState(CG_INCREASE_STATE *_lpPacket,int _iSerial)
{
	if (!checkPlayerAndKill(_iSerial))
		return;

	cACTOR	*lpActor		=	getActor(_iSerial);
	SG_CHANGE_STATE_RESULT	packet;

	if	(lpActor->isSealdAction())
		return;
	
	if (lpActor->increaseState(_lpPacket->wState,_lpPacket->wPoint))
	{
		switch(_lpPacket->wState)
		{
			case	eCHARACTER_BASIC_STATE_STRENGTH		:	
				packet.sValue				=	lpActor->m_sStrength;
				break;
			case	eCHARACTER_BASIC_STATE_CONSTITUTION	:
				packet.sValue				=	lpActor->m_sConstitution;
				break;
			case	eCHARACTER_BASIC_STATE_AGILITY		:
				packet.sValue				=	lpActor->m_sAgility;
				break;
			case	eCHARACTER_BASIC_STATE_INTELLIGENCE	:
				packet.sValue				=	lpActor->m_sIntelligence;
				break;
			case	eCHARACTER_BASIC_STATE_WISDOM		:
				packet.sValue				=	lpActor->m_sWisdom;
				break;
			case	eCHARACTER_BASIC_STATE_CHARISMA		:
				packet.sValue				=	lpActor->m_sCharisma;
				break;
			case	eCHARACTER_BASIC_STATE_LUCK			:
				packet.sValue				=	lpActor->m_sLuck;
				break;
		}

		packet.wType		=	_lpPacket->wState;
		packet.wLevelPoint	=	lpActor->m_wLevelPoint;

	}
	else
	{
		packet.wType		=	eCHARACTER_BASIC_STATE_FAILED;
		packet.wLevelPoint	=	lpActor->m_wLevelPoint;
	}

	packet.base.set(sizeof(SG_CHANGE_STATE_RESULT),dSG_CHANGE_STATE_RESULT);

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

//
//	어빌러티 레벨 증가
void
cFIELD::increaseAbilityLevel(CG_INCREASE_ABILITY_LEVEL *_lpPacket,int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	cACTOR	*lpActor		=	getActor(_iSerial);
	SG_INCREASE_ABILITY_LEVEL	sgIncreaseAbilityLevel;

	for (int i=0;i<_lpPacket->wPoint;i++)
	{
		if	(_lpPacket->wAimLevel)
		{
			if	(lpActor->m_aAbility[_lpPacket->wAbility].m_wLevel	>=	_lpPacket->wAimLevel)
				break;
		}
		if	(!lpActor->increaseAbilityLevel(_lpPacket->wAbility))
		{
			if	(i == 0)
				return;
			else
				break;
		}
	}

	{
		cAbility	*lpAbility	=	&lpActor->m_aAbility[_lpPacket->wAbility];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(lpSkill	&&	lpAbility->m_wLevel	&&	lpSkill->m_sIncreasePetPowerPerLevel)
		{
			for (int	i=0;i<dOWN_PET_COUNT;i++)
			{
				cACTOR	*lpPet	=	lpActor->getPetActor(i);

				if	(!lpPet	|| lpPet->isDeath())
					continue;

				lpPet->rebuildForUpdateLevel();
			}
		}
	}

	sgIncreaseAbilityLevel.base.set(sizeof(SG_INCREASE_ABILITY_LEVEL),dSG_INCREASE_ABILITY_LEVEL);
	sgIncreaseAbilityLevel.wAbility				=	_lpPacket->wAbility;
	sgIncreaseAbilityLevel.wLevel				=	lpActor->m_aAbility[_lpPacket->wAbility].m_wLevel;
	sgIncreaseAbilityLevel.dwSkillExperience	=	lpActor->m_dwSkillExperience;	//	남은 스킬 경험치

	g_userPM.add(lpActor->m_iClientSerial,&sgIncreaseAbilityLevel,sgIncreaseAbilityLevel.base.wSize);
}

//
//	몬스터 정보 요청
void
cFIELD::receiveAskActorInfo(CG_ASK_ACTOR_INFO *_lpPacket,int _iSerial)
{
	SG_SIMPLE_ACTOR_INFO	packet;

	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	cACTOR	*lpExistActor	=	getActor(_lpPacket->wTarget);
	int		iBufferSize		=	0;

	if	(!lpExistActor)
	{
		lpActor->sendRemoveActor(_lpPacket->wTarget);
		return;
	}

	if (lpExistActor->m_wActorKind	==	eAK_PLAYER)
	{
		CSimplePlayerInfo	*lpSimpleData	=	(CSimplePlayerInfo *)packet.aBuffer;

		lpExistActor->getSimplePlayerInfo(lpSimpleData);
		lpActor->correctInvisibleStatus(&lpSimpleData->dwEnchantedMask,lpExistActor);

		iBufferSize	=	sizeof(CSimplePlayerInfo);
	}
	else
	if (lpExistActor->m_wActorKind	==	eAK_MONSTER)
	{
		CSimpleMonsterInfo	*lpSimpleData	=	(CSimpleMonsterInfo	*)packet.aBuffer;

		lpExistActor->getSimpleMonsterInfo(lpSimpleData);
		lpActor->correctInvisibleStatus(&lpSimpleData->dwEnchantedMask,lpExistActor);

		iBufferSize	=	sizeof(CSimpleMonsterInfo);
	}
	else
	{
		CSimpleNpcInfo	*lpSimpleData	=	(CSimpleNpcInfo*)packet.aBuffer;

		lpExistActor->getSimpleNpcInfo(lpSimpleData);

		iBufferSize	=	sizeof(CSimpleNpcInfo);
	}

	packet.base.set(sizeof(packet)-sizeof(packet.aBuffer)+iBufferSize,dSG_SIMPLE_ACTOR_INFO);

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

void
cFIELD::receiveAskUseSoundOfLeadersBell(CG_USE_SOUND_OF_LEADERS_BELL *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	cItem	*lpItem	=	lpActor->getInventoryItem(_lpPacket->iItemSlot);

	if	(!lpItem)
		return;

	if	(!lpItem->isOwnSpecificEffectItem(eIE_SOUND_OF_LEADERS_BELL))
		return;

	if	(strlen(_lpPacket->strMessage) > 60)
		return;

	int	iMinute	=	lpItem->m_aOption[0];
	int	iBoost	=	lpItem->m_aOption[1];

	iMinute		=	iMinute*60;
	iBoost		=	iBoost*100;

	if	(!g_eventManager.checkTwoMouthOneSoundEventTime(iMinute))
	{
		lpActor->sendEtcWork(eEW_UNDER_EFFECT_TWO_MOUTH_ONE_SOUND_EVENT);
		//	이구 동성 이벤트 유효시간이다.

		return;
	}
	if	(!g_eventManager.checkGoldRushEventTime(iMinute))
	{
		lpActor->sendEtcWork(eEW_UNDER_EFFECT_GOLD_RUSH_EVENT);
		//	골드 러쉬 이벤트 유효시간이다.

		return;
	}
	//	골드러쉬 : 전 1시간~ 이벤트 시간 동안 사용 금지 체크
	//	이구동성 : 전 1시간~ 이벤트 종료 후 1시간 사용 금지 체크

	SERVERStoWORLD_SOUND_OF_LEADERS_BELL	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_SOUND_OF_LEADERS_BELL);

	packet.iBoostExp	=	iBoost;
	packet.iBoostItemDropChance=	iBoost;
	packet.iUpkeepMinute	=	iMinute;
	packet.wDisplayName		=	_lpPacket->wIsDisplayName;
	packet.iActorSerial		=	lpActor->m_iZoneSerial;
	packet.wItemIndex		=	_lpPacket->iItemSlot;
	packet.wIsTest			=	_lpPacket->wTest;


	strcpy(packet.strCaster,lpActor->m_strName);
	strcpy(packet.strMessage,_lpPacket->strMessage);

	SEND_WOLRD_PACKET(&packet);
}

void
cFIELD::receiveAskItemPackName(CG_ASK_ITEM_PACK_NAME *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	for (int i=0;i<_lpPacket->wCount;i++)
		lpActor->sendItemPackName(_lpPacket->awPackIndex[i]);
}

//
//	필드에 떨어진 아이템 줍기
BOOL
cFIELD::pickItem(int _iItemSerial,int _iCheckSum,int _iSerial,BOOL _bIsByServer)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return	FALSE;

	SG_PICK_ITEM_RESULT	sgPacket;

	sgPacket.base.set(sizeof(sgPacket),dSG_PICK_ITEM_RESULT);

	if	(lpActor->isSealdAction())
	{
		if	(_bIsByServer	==	FALSE)
		{
			sgPacket.wResult	=	eUPR_FAILED_BY_BAD_STATUS;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		}
		return	FALSE;
	}

	CDroppedItem	*lpDroppedItem	=	m_droppedItem.get(_iItemSerial);

	if (!lpDroppedItem || lpDroppedItem->m_item.m_wBaseItem	==	0xffff)
	{
		if	(_bIsByServer	==	FALSE)
		{
			sgPacket.wResult	=	eUPR_PICK_ITEM_IS_VALID_ITEM;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		}
		return	FALSE;
	}

	if	(lpActor->isDeath())
	{
		if	(_bIsByServer	==	FALSE)
		{
			sgPacket.wResult	=	eUPR_PICK_ITEM_IS_VALID_ITEM;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		}
		return	FALSE;
	}

	cBasicItem	*lpBasicItem	=	lpDroppedItem->m_item.getBasicItem();
	int			iItemOwnerParty	=	lpDroppedItem->getOwnerParty();

	if	(lpBasicItem	==	NULL)
		return	FALSE;

	if	(_bIsByServer	==	FALSE)
	{
		int	iRange	=	GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,lpDroppedItem->m_pos.x,lpDroppedItem->m_pos.y);

		if	(iRange	>	dPICK_ABLE_RANGE)
		{
			sgPacket.wResult	=	eUIR_TOO_FAR;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			return	FALSE;
		}
	}

	if	(lpBasicItem->m_questIndex!=	dNOT_QUEST_ITEM && lpBasicItem->m_questIndex	<	dNOT_QUEST_ITEM )	//	퀘스트 아이템이다.
		if	(lpActor->getProcessQuest(lpBasicItem->m_questKind,lpBasicItem->m_questIndex) == NULL)
		{
			if	(_bIsByServer	==	FALSE)
			{
				sgPacket.wResult	=	eUPR_QUEST_ITEM;

				g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			}
			return	FALSE;
		}

	memcpy(&sgPacket.item,&lpDroppedItem->m_item,sizeof(cItem));

	if	((_iCheckSum	!=	-1	&&	_iCheckSum	!=	lpActor->getItemCheckSum())	||	!lpDroppedItem)	//	얼~ 체크섬이 안 맞아!!
	{
		if	(_bIsByServer	==	FALSE)
		{
			sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		}
		return	FALSE;
	}

	if	(!lpDroppedItem->isOwner(_iSerial,lpActor->getPartySerial()))	//	니것이 아니잖어!!
	{
		if	(_bIsByServer	==	FALSE)
		{
			sgPacket.wResult	=	eUPR_PICK_ITEM_IS_PRIVATE_ITEM;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		}
		return	FALSE;
	}

	if	(lpDroppedItem->m_wDropMethod == eDIM_DUMMY)
	{
		sgPacket.wResult				=	eUIR_OK;

		if (lpActor->m_aEquip[dEQUIP_WEAPON].m_wBaseItem	== lpDroppedItem->m_item.m_wBaseItem && 
			lpActor->m_aEquip[dEQUIP_WEAPON].m_dwSerial		== lpDroppedItem->m_item.m_dwSerial)
		{
			lpActor->m_wHideWeapon1Time	=	FALSE;
			sgPacket.wResult			=	eUPR_PICK_THROW_WEAPON1;
		}

		if (lpActor->m_aEquip[dEQUIP_WEAPON_2].m_wBaseItem	== lpDroppedItem->m_item.m_wBaseItem && 
			lpActor->m_aEquip[dEQUIP_WEAPON_2].m_dwSerial	== lpDroppedItem->m_item.m_dwSerial)
		{
			lpActor->m_wHideWeapon2Time	=	FALSE;
			sgPacket.wResult			=	eUPR_PICK_THROW_WEAPON2;
		}

		m_droppedItem.remove(_iItemSerial);

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

		SG_REMOVE_DROPPED_ITEM	sgRemoveDroppedItem;

		sgRemoveDroppedItem.base.set(sizeof(SG_REMOVE_DROPPED_ITEM),dSG_REMOVE_DROPPED_ITEM);

		sgRemoveDroppedItem.wRemoveItem	=	_iItemSerial;

		addSendPacket((ALL_MSG*)&sgRemoveDroppedItem,0);
		return	TRUE;
	}

	BOOL	bIsDroppedItem	=	FALSE;

	if	(lpDroppedItem->m_wDropMethod	==	eDIM_PLAYER_DROP)
		bIsDroppedItem		=	TRUE;	//	사용자가 버린거다.

	cACTOR		*lpPicker	=	lpActor;

	cParty		*lpParty	=	lpActor->getParty();

	if	(lpParty	&&	lpDroppedItem->m_item.isQuestItem() == FALSE)
	{
		if	(lpDroppedItem->m_item.m_wBaseItem	==	dITEM_MONEY)
		{
			lpParty->sharingGold(m_iUniqueSerial,&lpDroppedItem->m_pos,lpActor,&lpDroppedItem->m_item);

			lpPicker	=	NULL;
		}
		else
			lpPicker	=	lpParty->getThisTimePicker(m_iUniqueSerial,&lpDroppedItem->m_pos,lpActor,&lpDroppedItem->m_item);
	}

	cItem	itemLog;

	itemLog.copy(&lpDroppedItem->m_item);

	if	(lpPicker)
	{
		int		iResult;

		if	(lpDroppedItem->m_item.isMoneyItem())
			iResult	=	lpPicker->addGoldItemToInventory(&lpDroppedItem->m_item,eGET_GOLD_BY_PICK_DROPPED_GOLD);
		else
			iResult	=	lpPicker->addItemToInventory(&lpDroppedItem->m_item,FALSE);

		sgPacket.wResult	=	iResult;

		if	(lpDroppedItem->m_item.m_wBaseItem	!=	0xffff)	//	아이템이 쪼매 남았다.
		{
			if (lpDroppedItem->m_item.m_wBaseItem	==	dITEM_MONEY)
				sgPacket.item.m_dwSerial=	sgPacket.item.m_dwSerial-lpDroppedItem->m_item.m_dwSerial;
			else
				sgPacket.item.m_bCount	=	sgPacket.item.m_bCount-lpDroppedItem->m_item.m_bCount;
		}

		g_userPM.add(lpPicker->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

		if	(lpPicker->m_wSerialInField	!=	lpActor->m_wSerialInField)
		{
			sgPacket.wResult	=	eUPR_PICK_ITEM_BY_PARTY_MEMBER;
			sgPacket.base.wSize	=	sizeof(sgPacket)-sizeof(cItem);

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		}

		if	(iResult	!=	eUIR_OK)
			return	FALSE;	//	아이템 추가 할라고 했는데 뭔가 문제가 있다.
	}

	if	(lpDroppedItem->m_item.m_wBaseItem	==	0xffff)	//	아이템을 완전히 줏었다.
	{
		m_droppedItem.remove(_iItemSerial);

		SG_REMOVE_DROPPED_ITEM	sgRemoveDroppedItem;

		sgRemoveDroppedItem.base.set(sizeof(SG_REMOVE_DROPPED_ITEM),dSG_REMOVE_DROPPED_ITEM);

		sgRemoveDroppedItem.wRemoveItem	=	_iItemSerial;

		addSendPacket((ALL_MSG*)&sgRemoveDroppedItem,0);

		int	iRequireLevel	=	itemLog.getRequireLevel();

		if	(lpPicker)
			if	(bIsDroppedItem || iRequireLevel	>=	50 || itemLog.isUniqueItem() || itemLog.isRareItem() || (itemLog.m_wBaseItem == dITEM_MONEY && itemLog.m_dwSerial >= 10000) )
				lpPicker->sendPickItemLog(&itemLog);
	}
	else
	{
		//JBC	참조	08-09-02
		//	빈자리는 없고... 중복되는게 있어서 몇개-_- 줏었다.
		SG_CHANGE_DROPPED_ITEM_INFO	sgChangeDroppedItemInfo;

		sgChangeDroppedItemInfo.base.set(sizeof(SG_CHANGE_DROPPED_ITEM_INFO),dSG_CHANGE_DROPPED_ITEM_INFO);

		sgChangeDroppedItemInfo.wFieldItem	=	_iItemSerial;

		if	(lpDroppedItem->m_item.m_wBaseItem	==	dITEM_MONEY)
			sgChangeDroppedItemInfo.dwCount		=	lpDroppedItem->m_item.m_dwSerial;
		else
			sgChangeDroppedItemInfo.dwCount		=	lpDroppedItem->m_item.m_bCount;

		addSendPacket((ALL_MSG*)&sgChangeDroppedItemInfo,0);
	}

	if	(lpPicker	&&	iItemOwnerParty	!=	0xffff)
	{
		cParty	*lpParty	=	lpActor->getParty();

		if	(lpParty)
			lpParty->sendPickItemMessage(lpPicker,&itemLog);
	}

	return	TRUE;
}

//
//	필드에 떨어진 아이템 줍기
void
cFIELD::pickItem(CG_PICK_ITEM *_lpPacket,int _iSerial)
{
	pickItem(_lpPacket->wItemSerial,_lpPacket->wCheckSum,_iSerial);
}

void
cFIELD::repairItem(CG_REPAIR_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);
	cACTOR	*lpNpc		=	getTestedActor(_lpPacket->wNpcSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction()	||	lpActor->m_wPitchmanShopSerial	!=	0xffff)
		return;

	if (!lpNpc)
	{
		lpActor->sendItemWorkResult(eRIWR_FAILED);
		return;
	}

	if (!lpActor->isTargetInAttackRange(lpNpc,dEVENT_RANGE_WITH_NPC))
	{
		lpActor->sendItemWorkResult(eRIWR_FAILED);
		return;
	}

	if (_lpPacket->wCheckSum	!=	lpActor->getItemCheckSum())
	{
		lpActor->sendItemWorkResult(eRIWR_ITEM_SYNC_FAILED);
		return;
	}

	cItem	*lpItem	=	lpActor->getInventoryItem(_lpPacket->wSlot);

	if (!lpItem)
	{
		lpActor->sendItemWorkResult(eRIWR_ITEM_SYNC_FAILED);
		return;
	}

	if (!lpItem->isBreakdownItem())
	{
		lpActor->sendItemWorkResult(eRIWR_ITEM_SYNC_FAILED);
		return;
	}

	int			iRepairPrice=	lpItem->getFullRepairPrice();
	LONGLONG	llPrice		=	iRepairPrice;
	
	llPrice					-=	llPrice*lpActor->m_wCourtesyTitleValue/10000;
	iRepairPrice			=	(int)llPrice;

	if	(g_bIsDuelServer)
		iRepairPrice		=	0;

	if (lpActor->m_iGold < iRepairPrice)
	{
		lpActor->sendItemWorkResult(eRIWR_ITEM_SYNC_FAILED);
		return;
	}

	lpActor->sendRepairItemLog(lpItem,iRepairPrice);

	lpActor->m_iGold -=	iRepairPrice;

	lpActor->updateGoldLog(-iRepairPrice,eSPENT_GOLD_BY_REPAIR_ITEM);

	lpItem->fullRepairDurability();

	SG_REPAIR_ITEM	packet;

	packet.base.set(sizeof(packet),dSG_REPAIR_ITEM);

	packet.bDurability		=	lpItem->m_bDurability;
	packet.bPart			=	(BYTE)_lpPacket->wSlot;
	packet.dwRepairPrice	=	iRepairPrice;

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

//
//	아이템 장비
void
cFIELD::equipItem(CG_EQUIP_ITEM *_lpPacket,int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	cACTOR		*lpActor		=	getActor(_iSerial);
	cItem		*lpItem			=	lpActor->getInventoryItem(_lpPacket->wItemSlot);
	
	SG_EQUIP_ITEM_RESULT	sgPacket;

	sgPacket.base.set(sizeof(cMSG_BASE_TYPE)+2,dSG_EQUIP_ITEM_RESULT);

	if	(!lpItem || (lpItem->m_wBaseItem!=_lpPacket->wBaseItem) || _lpPacket->wCheckSum!=lpActor->getItemCheckSum() )	//	얼~ 체크섬이 안 맞아!!
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if	(lpActor->isSealdAction()	||	lpActor->m_wPitchmanShopSerial	!=	0xffff)
	{
		sgPacket.wResult	=	eUIR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem(TRUE);

	if ((_lpPacket->wItemSlot	>=	lpActor->getItemSlotCount() && _lpPacket->wItemSlot < dOWN_ITEM_COUNT)	||
		_lpPacket->wItemSlot	>=	dOWN_ITEM_COUNT+lpActor->m_bf6ExtraInventorySize	)
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if (lpActor->m_isExclusiveAction)
	{
		sgPacket.wResult		=	eUER_CAN_NOT_EQUIP_EQUIPMENT_WHEN_ACTION;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

		return;
	}

	int	iEquipPart		=	_lpPacket->wPart;
	int	iEuqipResult	=	lpActor->equipItem(lpItem,_lpPacket->wPart,&iEquipPart);

	if (iEuqipResult	!=	eUIR_OK)
	{
		sgPacket.wResult	=	iEuqipResult;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

//	당사자에게 장비 교환 결과를 리턴해 준다.
	{
		sgPacket.base.set(sizeof(SG_EQUIP_ITEM_RESULT),dSG_EQUIP_ITEM_RESULT);
		sgPacket.wResult	=	eUIR_OK;
		sgPacket.wItemSlot	=	_lpPacket->wItemSlot;
		sgPacket.wPart		=	iEquipPart;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
	}

//	장비 교환으로 인해 외관의 변화가 있었다... 라고 추측된다. -_-
	if (lpBasicItem->isAppearanceChangeItem())
		lpActor->sendAppearanceInfo();
}

//
//	아이템 리로드
void
cFIELD::reloadBeltItem(CG_RELOAD_BELT_ITEM *_lpPacket,int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	cACTOR	*lpActor	=	getActor(_iSerial);

	SG_BELT_ITEM_RELOAD_RESULT	sgPacket;

	sgPacket.base.set(sizeof(sgPacket),dSG_BELT_ITEM_RELOAD_RESULT);

	if	(lpActor->isSealdAction()		||	lpActor->m_wPitchmanShopSerial	!=	0xffff)
	{
		sgPacket.wResult	=	eUIR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if (_lpPacket->wCheckSum	!=	lpActor->getItemCheckSum() )	//	얼~ 체크섬이 안 맞아!!
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if (lpActor->reloadBeltSlot())
	{
		sgPacket.wResult	=	eUIR_OK;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	{
		sgPacket.wResult	=	eURR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
	}
}

//
//	아이템 사용
void
cFIELD::useItem(CG_USE_ITEM *_lpPacket,int _iSerial)
{
	cACTOR		*lpActor	=	getTestedActor(_iSerial);
	cACTOR		*lpTarget	=	getTestedActor(_lpPacket->wWho);
	
	if (!lpActor || !lpTarget)
		return;

	cItem		*lpItem		=	lpActor->getInventoryItem(_lpPacket->wItemSlot);

	SG_USE_ITEM_RESULT	sgPacket;
	SG_USE_ITEM			sgUseItem;

	sgPacket.base.set(sizeof(sgPacket),dSG_USE_ITEM_RESULT);
	sgUseItem.base.set(sizeof(SG_USE_ITEM),dSG_USE_ITEM);
	sgUseItem.bf1IsUseEffectToHero	=	FALSE;
	sgUseItem.bf1IsDisplayMsg		=	FALSE;
	sgUseItem.base.wSize			=	sizeof(sgUseItem)-sizeof(sgUseItem.strCaster);

	sgPacket.wTarget				=	_lpPacket->wWho;

	if	(lpActor->isSealdAction()	||	lpActor->m_wPitchmanShopSerial	!=	0xffff)
	{
		sgPacket.wResult	=	eUIR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if	(!lpItem || (lpItem->m_wBaseItem!=_lpPacket->wBaseItem) || _lpPacket->wCheckSum	!=	lpActor->getItemCheckSum())
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

//	얼~ 체크섬이 안 맞아!!
	if	(!lpItem->checkFirstEffect(eIE_MINIPET_POUCH) && lpItem->m_bCount < 1)
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	cBasicItem	*lpBasicItem=	lpItem->getBasicItem();

	if	(m_bIsGuildBattleField)
		if (lpBasicItem->m_wKind	==	eIK_PREMIUM_ITEM || lpBasicItem->m_aGenerateData[0].m_wEffect == 238)
		{
			sgPacket.wResult	=	eUIR_CAN_NOT_USE_THIS_FIELD;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			return;
		}

//	너무 멀어~~
	if (GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y)	>	dRANGE_OF_USE_ITEM)
	{
		sgPacket.wResult	=	eUIR_TOO_FAR;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}
	if(lpBasicItem->m_bf1IsUseAfterWear)
	{// 착용후 기능 적용...
			if(_lpPacket->wItemSlot<dOWN_ITEM_COUNT+lpActor->m_bf6ExtraInventorySize)	// 인벤토리 아이템일 경우 기능 적용 안함.. 
				return;	
	}
	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG && lpBasicItem->m_bf1IsNotUseToGVG)
	{
		sgPacket.wResult	=	eUIR_CAN_NOT_USE_ITEM;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}
	if(lpBasicItem->m_bf1IsUseToEnemy )
	{
		if(!lpActor->isEnemy(lpTarget))
			return;
	}
	if (lpActor->isDeath())
	{
		if (!lpBasicItem->m_attr.isCanUseCorpseStatus)
		{
			sgPacket.wResult=	eUIR_IS_NOT_CORRECT_STATUS;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			return;
		}

		if (_iSerial		!=	_lpPacket->wWho)
		{
			sgPacket.wResult=	eUIR_INCORRECT_TARGET;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			return;
		}
	}
	else
	{
		if (lpTarget->isDeath())
		{
			if (!lpBasicItem->m_attr.isUseToCorpse)
			{
				sgPacket.wResult=	eUIR_INCORRECT_TARGET;

				g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
				return;
			}
		}
	}

	cItem	itemForLog;

	memcpy(&sgUseItem.item,lpItem,sizeof(sgUseItem.item));	//	아이템 복 사 해두기..
	itemForLog.copy(lpItem);

//	사용 실패~~
	int	iUseItemResult		=	lpActor->useItemToCharacter(lpItem,lpTarget,FALSE);

//	프리미엄 아이템은 사용 로그도 남긴다.
	if ((lpBasicItem->m_wKind==	eIK_PREMIUM_ITEM && lpItem->m_bCount != sgUseItem.item.m_bCount) || lpBasicItem->m_attr.isRequireSaveUseLog)
		lpActor->sendUseItemLog((cItem*)&itemForLog);

	sgPacket.wResult	=	iUseItemResult;
	sgPacket.wItemSlot	=	_lpPacket->wItemSlot;

	switch(iUseItemResult)
	{
//	복권 당첨
		case	eUIR_ROTTERY_TICKET_OK	:
		{
			SG_LOTTERY_TICKET_PRIZE_WINNING		packetPrize;

			packetPrize.base.set(sizeof(packetPrize),dSG_LOTTERY_TICKET_PRIZE_WINNING);
			packetPrize.wItemSlot			=	_lpPacket->wItemSlot;
			packetPrize.bf1IsBySelect		=	FALSE;

			cItem	*lpItem	=	lpTarget->getInventoryItem(_lpPacket->wItemSlot);

			if	(!lpItem)
				break;

			memcpy(&packetPrize.itemPrize,lpItem,sizeof(cItem));

			g_userPM.add(lpActor->m_iClientSerial,&packetPrize,packetPrize.base.wSize);

			break;
		}

//	복권 꽝 -o-
		case	eUIR_ROTTERY_TICKET_KWANG	:
		{
			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			break;
		}

//	아이템 사용 성공
		case	eUIR_OK						:
		{
			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

			sgUseItem.wWho		=	_lpPacket->wWho;	//	누구? 한테

			addSendPacket((ALL_MSG*)&sgUseItem,lpActor->m_wSerialInField);
			break;
		}

//	기타 다른 문제가 있음
		default								:
		{
			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

			break;
		}
	}
}

//
//	아이템을 아이템에 사용
void
cFIELD::useItemToItem(CG_USE_ITEM_TO_ITEM *_lpPacket,int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	cACTOR	*lpActor		=	getActor(_iSerial);
	cItem	*lpItem			=	lpActor->getInventoryItem(_lpPacket->bItemSlot);
	cItem	*lpTargetItem	=	lpActor->getInventoryItem(_lpPacket->bTargetItem);
	cItem	itemUse;

	SG_USE_ITEM_TO_ITEM_RESULT	sgPacket;

	sgPacket.base.set(sizeof(sgPacket),dSG_USE_ITEM_TO_ITEM_RESULT);
	sgPacket.bItemSlot		=	_lpPacket->bItemSlot;
	sgPacket.bTargetItemSlot=	_lpPacket->bTargetItem;

//	얼~ 체크섬이 안 맞아!!
	WORD	wCheckSum		=	lpActor->getItemCheckSum();

	if	(lpActor->isSealdAction()	||	lpActor->m_wPitchmanShopSerial	!=	0xffff)
	{
		sgPacket.wResult	=	eUIR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if (!lpItem || !lpTargetItem || lpItem->m_wBaseItem!=_lpPacket->wBaseItem || _lpPacket->wCheckSum	!=	wCheckSum)
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if	(!lpItem->isExceptionItem()	&&	lpItem->m_bCount < 1)
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	memcpy(&itemUse,lpItem,sizeof(itemUse));

//	사용 실패~~
	int			iUseItemResult		=	lpActor->useItemToItem(lpItem,lpTargetItem,_lpPacket->bItemSlot,_lpPacket->bTargetItem);
	cBasicItem	*lpBasicItem		=	itemUse.getBasicItem();

//	프리미엄 아이템은 사용 로그도 남긴다.
	if	((lpBasicItem->m_wKind==	eIK_PREMIUM_ITEM && lpItem->m_bCount != itemUse.m_bCount) || lpBasicItem->m_attr.isRequireSaveUseLog)
		lpActor->sendUseItemLog(&itemUse);

	switch(iUseItemResult)
	{
//	아이템 사용 성공
		case	eUIR_OK						:
		{
			sgPacket.wResult		=	iUseItemResult;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			break;
		}

//	기타 다른 문제가 있음
		default								:
		{
			sgPacket.wResult		=	iUseItemResult;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

			break;
		}
	}
}

//
//	벨트에 아이템을 사용한다.
void
cFIELD::useBeltItem(CG_USE_BELT_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);
	cACTOR	*lpTarget	=	getTestedActor(_lpPacket->wTarget);
	cItem	*lpItem		=	lpActor->getBeltItem(_lpPacket->wBeltSlot);

	if (!lpActor || !lpTarget)
		return;

	SG_USE_BELT_ITEM_RESULT	sgPacket;
	SG_USE_ITEM				sgUseItem;

	sgPacket.base.set(sizeof(SG_USE_BELT_ITEM_RESULT),dSG_USE_BELT_ITEM_RESULT);
	sgUseItem.base.set(sizeof(SG_USE_ITEM),dSG_USE_ITEM);

	sgPacket.wTarget=	_lpPacket->wTarget;
	sgUseItem.wWho	=	_lpPacket->wTarget;
	sgUseItem.bf1IsUseEffectToHero	=	FALSE;
	sgUseItem.bf1IsDisplayMsg		=	FALSE;
	sgUseItem.base.wSize			=	sizeof(sgUseItem)-sizeof(sgUseItem.strCaster);

	if	(lpActor->isSealdAction())
	{
		sgPacket.wResult	=	eUIR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

//	얼~ 체크섬이 안 맞아!!
	if (!lpItem)
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	cBasicItem	*lpBasicItem	=	lpItem->getBasicItem();

	if	(!lpBasicItem	||	(lpItem->m_wBaseItem!=_lpPacket->wBaseItem) || _lpPacket->wCheckSum	!=	lpActor->getItemCheckSum()  ||	lpItem->m_bCount < 1)
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if	(_lpPacket->wTarget != _iSerial)
	{
		if	(lpBasicItem->m_attr.isUseToCorpse	==	FALSE)
			if	(!lpTarget->isPet() || lpTarget->m_wTamer != _iSerial)
			{
				sgPacket.wTarget=	_iSerial;
				sgUseItem.wWho	=	_iSerial;
				lpTarget		=	lpActor;
			}
	}

//	너무 멀어~~
	if	(GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y)	>	400*400)
	{
		sgPacket.wResult	=	eUIR_TOO_FAR;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	{
		memcpy(&sgUseItem.item,lpItem,sizeof(sgUseItem.item));	//	아이템 복 사 해두기..
	}


	if	(m_bIsGuildBattleField)
		if	(lpBasicItem->m_wKind	==	eIK_PREMIUM_ITEM)
		{
			sgPacket.wResult	=	eUIR_CAN_NOT_USE_THIS_FIELD;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			return;
		}

	if	(lpActor->isDeath())
	{
		if	(!lpBasicItem->m_attr.isCanUseCorpseStatus)
		{
			sgPacket.wResult=	eUIR_IS_NOT_CORRECT_STATUS;

			g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
			return;
		}
	}

//	사용 실패~~
	int	iResult	=	lpActor->useItemToCharacter(lpItem,lpTarget,TRUE);

//	프리미엄 아이템은 사용 로그도 남긴다.
	if	((lpBasicItem->m_wKind==	eIK_PREMIUM_ITEM && lpItem->m_bCount != sgUseItem.item.m_bCount) || lpBasicItem->m_attr.isRequireSaveUseLog)
		lpActor->sendUseItemLog((cItem*)&sgUseItem.item);

	if	(iResult >= eUIR_FAILED)
	{
		sgPacket.wResult	=	iResult;
		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

//	잘 썼어~
	{
		sgPacket.wResult	=	eUIR_OK;
		sgPacket.wItemSlot	=	_lpPacket->wBeltSlot;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
	}

	addSendPacket((ALL_MSG*)&sgUseItem,lpActor->m_wSerialInField);
}

//
//	아이템 버리기
void
cFIELD::dropItem(CG_DROP_ITEM *_lpPacket,int _iSerial)
{
}

//
//	뭔가 잘못 되었다!! 아이템 데이터를 요청한다.
void
cFIELD::askItemData(int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	lpActor->sendItemData();
}

//
//	뭔가 잘못 되었다!! 아이템 데이터를 요청한다.
void
cFIELD::askInventoryData(int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	SG_INVENTORY_DATA	sgPacket;
	cACTOR				*lpActor	=	getActor(_iSerial);

	sgPacket.base.set(sizeof(SG_INVENTORY_DATA),dSG_INVENTORY_DATA);

	memcpy(sgPacket.m_aItems,lpActor->m_aItems,sizeof(sgPacket.m_aItems));

	g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
}


//
//	뭔가 잘못 되었다!! 아이템 데이터를 요청한다.
void
cFIELD::askEquipmentData(int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	SG_EQUIPMENT_DATA	sgPacket;
	cACTOR				*lpActor	=	getActor(_iSerial);

	sgPacket.base.set(sizeof(SG_EQUIPMENT_DATA),dSG_EQUIPMENT_DATA);

	memcpy(sgPacket.m_aEquip,lpActor->m_aEquip,sizeof(sgPacket.m_aEquip));

	g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
}

//
//	장비 해제
void
cFIELD::stripEquipment(CG_STRIP_EQUIPMENT *_lpPacket,int _iSerial)
{
	if (!checkPlayerAndKill(_iSerial))
		return;

	SG_STRIP_EQUIPMENT_RESULT	packet;
	cACTOR	*lpActor	=	getActor(_iSerial);

	packet.base.set(sizeof(SG_STRIP_EQUIPMENT_RESULT),dSG_STRIP_EQUIPMENT_RESULT);

	if	(lpActor->isSealdAction()	||	lpActor->m_wPitchmanShopSerial	!=	0xffff)
	{
		packet.wResult	=	eUIR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

//	얼~ 체크섬이 안 맞아!!
	if (_lpPacket->wCheckSum	!=	lpActor->getItemCheckSum())
	{
		packet.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

	if (lpActor->m_isExclusiveAction)
	{
		packet.wResult	=	eUSR_CAN_NOT_STRIP_EQUIPMENT_WHEN_ACTION;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);

		return;
	}

	int	iResult	=	lpActor->stripEquipment(_lpPacket->wPart);

	if (iResult	!=	eUIR_OK)
	{
		packet.wResult	=	iResult;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

	packet.wResult		=	eUIR_OK;
	packet.wPart		=	_lpPacket->wPart;

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

//
//	아이템 교환
void
cFIELD::changeItemPlace(CG_CHANGE_ITEM_PLACE *_lpPacket,int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	SG_CHANGE_ITEM_PLACE_RESULT	sgPacket;
	cACTOR		*lpActor	=	getActor(_iSerial);

	sgPacket.base.set(sizeof(SG_CHANGE_ITEM_PLACE_RESULT),dSG_CHANGE_ITEM_PLACE_RESULT);

	if	(lpActor->isSealdAction()	||	lpActor->m_wPitchmanShopSerial	!=	0xffff)
	{
		sgPacket.wResult	=	eUIR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

//	얼~ 체크섬이 안 맞아!!
	if (_lpPacket->wCheckSum	!=	lpActor->getItemCheckSum())
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	if	(!lpActor->changeItemPlace(_lpPacket->wSrcItemSlot,_lpPacket->wDestItemSlot))
	{
		sgPacket.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
		return;
	}

	sgPacket.wResult		=	eUIR_OK;
	sgPacket.wSrcSlot		=	_lpPacket->wSrcItemSlot;
	sgPacket.wDestSlot		=	_lpPacket->wDestItemSlot;

	g_userPM.add(lpActor->m_iClientSerial,&sgPacket,sgPacket.base.wSize);
	return;
}

//
//	아이템 파괴
void
cFIELD::destroyItem(CG_DESTROY_ITEM *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	SG_DESTROY_ITEM_RESULT	packet;

	packet.wSlot	=	_lpPacket->wSlot;
	packet.base.set(sizeof(SG_DESTROY_ITEM_RESULT),dSG_DESTROY_ITEM_RESULT);

	if	(lpActor->isSealdAction()	||	lpActor->m_wPitchmanShopSerial	!=	0xffff)
	{
		packet.wResult	=	eUIR_FAILED;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

//	얼~ 체크섬이 안 맞아!!
	if (_lpPacket->wCheckSum	!=	lpActor->getItemCheckSum())
	{
		packet.wResult	=	eUIR_WRONG_ITEM_DATA;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

	packet.wResult	=	lpActor->destroyItem(_lpPacket->wSlot);

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

//
//앉기 토글
void
cFIELD::toggleSitdown(CG_TOGGLE_SITDOWN *_lpPacket,int _iSerial)
{
	cACTOR		*lpActor	=	getTestedActor(_iSerial);

	SG_SITDOWN_RESULT	packetResult;

	packetResult.base.set(sizeof(SG_SITDOWN_RESULT),dSG_SITDOWN_RESULT);

	if	(!lpActor)
		return;
	if(lpActor->m_wRushTime)		// 돌진중에는 앉지못한다.
		return;

	if	(lpActor->isSealdAction()	||	lpActor->m_wSlaveActor	!=	0xffff)
	{
		packetResult.wResult	=	2;

		g_userPM.add(lpActor->m_iClientSerial,&packetResult,packetResult.base.wSize);
		return;
	}

	if	(lpActor->m_wRidingDogTime)
	{
		lpActor->getOffFromDog();
		lpActor->setMoveSpeed(TRUE);
		return;
	}

	if (lpActor->m_bf5MagicCarpetShape	&&	lpActor->isSitdown()	==	FALSE)
	{
		lpActor->m_bf5MagicCarpetShape	=	0;

		lpActor->addSendActorMagicEffectStatus();
		lpActor->setMoveSpeed(TRUE);
		return;
	}

	LONGLONG	llRestHP	= lpActor->m_iHP;
	llRestHP	*=	255;
	llRestHP	/=	lpActor->getMaxHP();

	packetResult.wHP	=	(WORD)llRestHP;
	packetResult.sCP	=	lpActor->m_iCP/100;

	if (lpActor->m_iHP)
		packetResult.wHP	=	max(packetResult.wHP,1);

	if	(!lpActor->checkMoveCounter(_lpPacket->wPosX,_lpPacket->wPosY,"CG_TOGGLE_SITDOWN"))
		return;
	if	(lpActor->toggleSitdown(_lpPacket->wPosX,_lpPacket->wPosY))
	{
		SG_TOGGLE_SITDOWN	packet;

		packet.base.set(sizeof(SG_TOGGLE_SITDOWN),dSG_TOGGLE_SITDOWN);

		packet.wPosX		=	lpActor->m_pos.x;
		packet.wPosY		=	lpActor->m_pos.y;
		packet.bf15Serial	=	_iSerial;

		packet.bf1IsSitDown	=	lpActor->isSitdown();

		addSendPacket((ALL_MSG*)&packet,_iSerial);

		packetResult.wResult=	eSITDOWN_RESULT_OK;
		packetResult.wValue	=	lpActor->isSitdown();
	}
	else
	{
		packetResult.wResult=	eSITDOWN_RESULT_FAILED_BY_BATTLE;
		packetResult.wValue	=	lpActor->getRestraintTimeByBattle();//	HP/CP
	}

	g_userPM.add(lpActor->m_iClientSerial,&packetResult,packetResult.base.wSize);
}

//
//	리액션 상태 설정
void
cFIELD::setReactionStatus(CG_SET_REACTION_STATUS *_lpPacket,int _iSerial)
{
	if (!checkPlayerAndKill(_iSerial))
		return;

	cACTOR		*lpActor	=	getActor(_iSerial);
	WORD 		wIsResult;
	SG_SET_REACTION_STATUS	packet;

	if(lpActor)
	{
		if(lpActor->m_wTransformationTime)
			return;
		
		if(m_bIsNotUseSkillBeforeTrans)
			if(!lpActor->m_wTransformationTime)
				return;
	}

	if (_lpPacket->wIsOn)
		wIsResult	=	lpActor->reactionOn(_lpPacket->wAbility);
	else
		wIsResult	=	lpActor->reactionOff(_lpPacket->wAbility);

	packet.base.set(sizeof(SG_SET_REACTION_STATUS),dSG_SET_REACTION_STATUS);

	packet.wAbility	=	_lpPacket->wAbility;
	packet.wIsOn	=	lpActor->isActiveReaction(packet.wAbility);

	packet.wResult	=	eRRS_OK;

	if (wIsResult)
		packet.wResult	=	wIsResult;
	else
		lpActor->operateInstanceApplyAura();

	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

//
//	리액션들 상태 설정
void
cFIELD::setPassiveSkillStatus(CG_TURN_ON_PASSIVE_SKILL *_lpPacket,int _iSerial)
{
	if	(!checkPlayerAndKill(_iSerial))
		return;

	cACTOR	*lpActor	=	getActor(_iSerial);
	int		i,iCount=0;

	if	(lpActor->isSealdAction())
		return;

	SG_TURN_ON_PASSIVE_SKILL	packet;

	packet.base.set(sizeof(SG_TURN_ON_PASSIVE_SKILL),dSG_TURN_ON_PASSIVE_SKILL);
	memset(packet.aPassiveSkill,0xff,sizeof(packet.aPassiveSkill));

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	_lpPacket->aPassiveSkill[i];

		if (iAbility	==	0xff)
			continue;

		int iResult = lpActor->reactionOn(iAbility);
		if(iResult !=eRRS_OK)
		{		// 결과가 성공이 아니면.. 리액션을 끈다...
			lpActor->reactionOff(iAbility);
		}
	}

	for (i=0;i<dACTIVE_REACTION_COUNT;i++)
	{
		int	iAbility	=	lpActor->m_aActiveReaction[i];

		if (iAbility	==	0xffff	)
			continue;

		packet.aPassiveSkill[iCount++]	=	iAbility;
	}

	if (iCount	==	0)
		return;

	lpActor->operateInstanceApplyAura();
	
	g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

//
//	게임 종료
void
cFIELD::exitGame(CG_EXIT_GAME *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(m_wDuelType	!=	0xffff && m_wDuelMatchStep == eDUEL_MATCH_STEP_BATTLE)
		return;

	if	(m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
	{
		if	(lpActor->getRestraintTimeByLogout()	==	0xffff)
			lpActor->setRestraintTimeByLogout();

		lpActor->sendEtcWork(eEW_TRY_LOGOUT,lpActor->getRestraintTimeByLogout(),dLOGOUT_WAIT_TIMER , m_bIsGuildDungeonMap);

		lpActor->resetRestraintTimeByBattle();

		return;
	}

	if	(lpActor->getRestraintTimeByBattle())
	{
		lpActor->sendEtcWork(eEW_CAN_NOT_EXIT_GAME_DURING_BATTLE,lpActor->getRestraintTimeByBattle());
		return;
	}

	lpActor->m_bf1IsPlayOnGuildBattle	=	FALSE;

	lpActor->sendEtcWork(eEW_PERMISSION_LOGOUT);
}

//
//	레벨업!!
void
cFIELD::levelUp(int _iSerial)
{
	if (!checkPlayerAndKill(_iSerial))
		return;

	SG_LEVEL_UP_OTHER	packetLevelupOther;

	packetLevelupOther.base.set(sizeof(SG_LEVEL_UP_OTHER),dSG_LEVEL_UP_OTHER);
	packetLevelupOther.wWho		=	_iSerial;
	packetLevelupOther.wLevel	=	getActor(_iSerial)->m_iLevel;

	addSendPacket((ALL_MSG*)&packetLevelupOther,_iSerial);
}

//
//	디버그 메시지 받음
void
cFIELD::receiveDebugMessage(CG_DEBUG_MESSAGE *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	if (strlen(_lpPacket->strMessage) >= dDEBUG_MESSAGE_LENGTH)	return;

	CLOG("client message","[%2d/%2d %.2d:%.2d:%.2d] '%s' by %s/%s",
					g_currentTime.m_wMonth,g_currentTime.m_wDay,g_currentTime.m_wHour,g_currentTime.m_wMinute,g_currentTime.m_wSecond,
					_lpPacket->strMessage,lpActor->m_strId,lpActor->m_strName);
}

void
cFIELD::receiveCallMagicCarpet(CG_CALL_MAGIC_CARPET *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;

	if	(lpActor->m_bf1IsHaveSummonMagicCarpetItem == 0)
		return;

	lpActor->callMagicCarpet();
}

void
cFIELD::receiveChangeEmblemShape(CG_CHANGE_EMBLEM_SHAPE *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;

	cItem	*lpItem		=	lpActor->getMagicCarpetEmblem();

	if (!lpItem)
		return;

	lpItem->m_aOption[0]		=	(BYTE)_lpPacket->wShape;
	lpActor->m_wMagicCarpetShape=	lpItem->m_aOption[0];
	int	iSlot					=	lpActor->getMagicCarpetEmblemIndex();

	if (lpActor->m_bf5MagicCarpetShape)
	{
		lpActor->callMagicCarpet();
		lpActor->addSendVerySimpleInfo();
	}

	lpActor->sendUpdateItemData(iSlot,TRUE);
}


void
cFIELD::receiveRememberOrbLocation(CG_REMEMBER_ORB_LOCATION *_lpPacket,int _iSerial)
{
	if (m_iUniqueSerial	==	27	||	m_iUniqueSerial	==	28	)
		return;
	
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;

	if	(m_wIsCanNotMemoryField)
	{
		lpActor->sendUseOrbResult(eUOR_CAN_NOT_WRITTEN_FIELD);
		return;
	}

	lpActor->rememberOrbLocation(_lpPacket->wSlot);
}

void
cFIELD::receiveWarpToNearVillage(CG_WARP_TO_NEAR_VILLAGE *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;

	if	(g_game.isCloseMap(_lpPacket->wVillage) || g_fieldInfo.isVillage(_lpPacket->wVillage) == FALSE)
	{
		CLOG("packetHacking","[%.3d] [%s/%s] hacking CG_WARP_TO_NEAR_VILLAGE [%d]",lpActor->m_wCurrentField,lpActor->m_strId,lpActor->m_strName,_lpPacket->wVillage);

		lpActor->setPeneltyTime(60,ePPPTR_PACKET_HACKING);

		return;
	}

	int	iDistance	=	g_fieldInfo.getDistance(lpActor->m_wCurrentField,_lpPacket->wVillage);

	if	(iDistance	>	lpActor->m_wWarpAbleRange)
	{
		CLOG("packetHacking","[%.3d] [%s/%s] hacking CG_WARP_TO_NEAR_VILLAGE [%d>%d]",lpActor->m_wCurrentField,lpActor->m_strId,lpActor->m_strName,iDistance,lpActor->m_wWarpAbleRange);

		lpActor->setPeneltyTime(60,ePPPTR_PACKET_HACKING);

		return;
	}

	if	(lpActor->m_bf1IsReadyToWarpToNearestVillage)
	{
		g_pMoveFieldUserManager->booking(lpActor,"",_lpPacket->wVillage,0xffff);

		lpActor->m_bf1IsReadyToWarpToNearestVillage	=	FALSE;
	}

    if	(lpActor->m_bf1IsReadyToCreateTownPortal)
	{
		addPortal(lpActor,_lpPacket->wVillage,lpActor->m_wPortalTime);

		lpActor->m_bf1IsReadyToCreateTownPortal		=	FALSE;
	}
}

void
cFIELD::receiveEnterPortal(CG_ENTER_PORTAL *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;

	if	(_lpPacket->wPortal	>=	dMAX_PORTAL_IN_FIELD)
		return;

	CPortalInfo	*lpPortal	=	&m_portal.m_aPortal[_lpPacket->wPortal];

	if	(lpPortal->m_wOwner	==	0xffff)
	{
		lpActor->sendRegistServerMessage(eRSM_REMOVE_PORTAL);

		return;
	}

	int	iRange	=	GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,lpPortal->m_wX,lpPortal->m_wY);

	if	(iRange	>=	200*200)
	{
		lpActor->sendRegistServerMessage(eRSM_TOO_FAR_PORTAL);

		return;
	}

	if	(lpPortal->m_wTargetX	!=	0xffff	)
	{
		if	(lpPortal->m_dwPartyUniqueSerial	!=	0xffffffff)
		{
			cParty	*lpParty	=	lpActor->getParty();

			if	(!lpParty	||	lpPortal->m_dwPartyUniqueSerial	!=	lpParty->m_dwUniqueSerial)
			{
				lpActor->sendRegistServerMessage(eRSM_CAN_NOT_ENTER_TO_PORTAL_BY_IS_OWNED_DIFFERENT_PARTY);
				return;
			}
		}
		else
		if	(lpPortal->m_dwCreatorNameHashCode	!=	lpActor->m_dwNameHashCode)
		{
			lpActor->sendRegistServerMessage(eRSM_CAN_NOT_ENTER_TO_PORTAL_BY_IS_OWNED_DIFFERENT_PARTY);
			return;
		}

		lpActor->teleportByInregularMethod(lpPortal->m_wField,lpPortal->m_wTargetX*dTILE_XS,lpPortal->m_wTargetY*dTILE_YS);
	}
	else
		g_pMoveFieldUserManager->booking(lpActor,"",lpPortal->m_wField,0xffff);
}

void
cFIELD::receiveTeleportToOrbLocation(CG_TELEPORT_TO_ORB_LOCATION *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(isCanNotUseTeleportEffectField())
	{
		lpActor->sendRegistServerMessage(eRSM_CAN_NOT_TELEPORT_FIELD,eSM_SYSTEM);

		return;
	}

	if	(lpActor->getRestraintTimeByBattle())
	{
		lpActor->sendEtcWork(eEW_CAN_NOT_TELEPORT_DURING_BATTLE,lpActor->getRestraintTimeByBattle());

		return;
	}

	if	(lpActor->isSealdAction())
		return;

	if	(m_bIsGuildBattleField)
	{
		lpActor->sendUseOrbResult(eUOR_CAN_NOT_USE_AT_THIS_FIELD);
		return;
	}

	lpActor->teleportToOrbLocation(_lpPacket->wSlot);
}

void
cFIELD::receiveAskRecoverDeathPenelty(CG_RECOVER_DEATH_PENELTY *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	if	(lpActor->isSealdAction())
		return;
	
	lpActor->recoverDeathPenelty();
}

cPACKET_INFO	l_aPacket[dMAX_STORE_PACKET_COUNT];		//	패킷들..
#define	dCG_MISPLACED_ACTOR	0x2537

void
cFIELD::uploadPacketToWorkBuffer()
{
	mCS(m_csPacketManager);

	m_iWorkPacketCount	=	0;

	while(1)
	{
		cPACKET_INFO	*lpPacket	=	m_queReceivePacket.popPacket();

		if	(!lpPacket)
			break;

		memcpy(&l_aPacket[m_iWorkPacketCount++],lpPacket,lpPacket->m_packet.base.wSize+8);
	}
}

//
//	저장된 패킷 처리
void
cFIELD::operateStoredPacket()
{
	uploadPacketToWorkBuffer();

	int	i;

	for (i=0;i<m_iWorkPacketCount;i++)
	{
		cPACKET_INFO	*lpPacket	=	&l_aPacket[i];
		cACTOR			*lpOwner	=	getTestedActor(lpPacket->m_iOwner);

		g_game.beginProfileByID(lpPacket->m_packet.base.wType);
		mDS_PK.m_iLastUpdateClientPacket	=	lpPacket->m_packet.base.wType;

		switch(lpPacket->m_packet.base.wType)
		{
			case	dBW_EXIT_GAME				:
				operateBookedExitGame(&lpPacket->m_packet.mBW_EXIT_GAME,lpPacket->m_iOwner);
				break;

			case	dBW_EXIT_GAME_FOR_MOVE_FIELD:
				operateBookedExitGameForMoveField(&lpPacket->m_packet.mBW_EXIT_GAME_FOR_MOVE_FIELD,lpPacket->m_iOwner);
				break;

			case	dBW_READY_TO_EXIT			:
				operateReadyToExit(&lpPacket->m_packet.mBW_READY_TO_EXIT,lpPacket->m_iOwner);
				break;

			case	dBW_DEATH_ACTOR				:
				operateBookedDeathActor(&lpPacket->m_packet.mBW_DEATH_ACTOR,lpPacket->m_iOwner);
				break;

			case	dBW_COPY_ACTOR				:
				operateBookedCopyActor((BW_COPY_ACTOR *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dBW_MAKE_DUMMY				:
				operateBookedMakeDummy(&lpPacket->m_packet.mBW_MAKE_DUMMY,lpPacket->m_iOwner);
				break;

			case	dBW_MAKE_ILLUSION			:
				operateAppointmentMakeIllusion((BW_MAKE_ILLUSION *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dBW_DISPLACEMENT			:
				operateBookedDisplacement(&lpPacket->m_packet.mBW_DISPLACEMENT,lpPacket->m_iOwner);
				break;

			case	dBW_CHARMING				:
				operateBookedCharming(&lpPacket->m_packet.mBW_CHARMING,lpPacket->m_iOwner);
				break;
				
			case	dBW_MONSTER_TAME			:
				operateBookedTame((BW_MONSTER_TAME *)&lpPacket->m_packet);
				break;
				
			case	dBW_GENERATE_PET			:
				operateBookedGeneratePet((BW_GENERATE_PET *)&lpPacket->m_packet);
				break;
			case	dBW_GENERATE_SUMMON_BEAST	:
				operateBookedGenerateSummonBeast((BW_GENERATE_SUMMON_BEAST *)&lpPacket->m_packet);
				break;
			case	dBW_INTERVAL_SHOOTER		:
				operateBookedIntervalShooter((BW_INTERVAL_SHOOTER *)&lpPacket->m_packet);
				break;
			case	dBW_MAKE_SKILL_ZONE		:
				operateMakeSkillZone((BW_MAKE_SKILL_ZONE *)&lpPacket->m_packet);
				break;
		}

		g_game.endProfileByID(lpPacket->m_packet.base.wType);
	}

	for (i=0;i<m_iWorkPacketCount;i++)
	{
		cPACKET_INFO	*lpPacket	=	&l_aPacket[i];
		cACTOR			*lpOwner	=	getTestedActor(lpPacket->m_iOwner);

		mDS_PK.m_iLastUpdateClientPacket	=	lpPacket->m_packet.base.wType;
		g_game.beginProfileByID(lpPacket->m_packet.base.wType);

		if	(!lpOwner || lpOwner->m_bIsReadyToExit)
			continue;

		switch(lpPacket->m_packet.base.wType)
		{
			case	dCM_DUMMY				:
				break;

			case	dCG_XTRAP_PACKET		:
				receiveXTrapPacket((CG_XTRAP_PACKET *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_ETC_WORK			:
				receiveEtcWork((CG_ETC_WORK *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_JUDGE_TO_ITEM		:
				receiveJudgeToItem((CG_JUDGE_TO_ITEM *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_MERGE_ITEM			:
				receiveMergeItem((CG_MERGE_ITEM *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_REMOVE_PREFIX		:
				receiveRemovePrefix((CG_REMOVE_PREFIX *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_WARP_FIELD_BY_GATE_GLOVE	:
				warpFieldByGateGlove((CG_WARP_FIELD_BY_GATE_GLOVE *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			//JBC	넷카페를 이용한 이동. 	08-08-12	
			case	dCG_WARP_FIELD_BY_NET_CAFE		:
				warpFieldByGateGlove((CG_WARP_FIELD_BY_GATE_GLOVE *)&lpPacket->m_packet,lpPacket->m_iOwner , TRUE);
				break;
			case	dCG_ASK_ACTOR_ENCHANT_INFO		:
				receiveAskActorEnchantInfo((CG_ASK_ACTOR_ENCHANT_INFO *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_GG_AUTH_DATA		:
				receiveGGAuthData((CG_GG_AUTH_DATA *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_CHANGE_PET_NAME		:
				receiveChangePetName((CG_CHANGE_PET_NAME *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_PARTING_WITH_PET	:
				receivePartingWithPet((CG_PARTING_WITH_PET *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_OPERATOR_COMMAND	:
				receiveOperatorCommand(&lpPacket->m_packet.mCG_OPERATOR_COMMAND,lpPacket->m_iOwner);
				break;

			case	dCG_RETURN_TO_VILLAGE	:
				receiveReturnToVillage(&lpPacket->m_packet.mCG_RETURN_TO_VILLAGE,lpPacket->m_iOwner);
				break;

			case	dCG_MOVE					:	
				movePlayer(&lpPacket->m_packet.mCG_MOVE,lpPacket->m_iOwner);
				break;

			case	dCG_REPORT_CURRENT_POS		:
				receiveReportCurrentPos((CG_REPORT_CURRENT_POS *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_ASK_INFO_ACTOR_LIST		:
				receiveAskInfoActorList(&lpPacket->m_packet.mCG_ASK_INFO_ACTOR_LIST,lpPacket->m_iOwner);
				break;

			case	dCG_OBSCURITY_ACTOR_LIST	:
				receiveObscurityActorList(&lpPacket->m_packet.mCG_OBSCURITY_ACTOR_LIST,lpPacket->m_iOwner);
				break;

			case	dCG_MISPLACED_ACTOR_LIST	:
				receiveMisplacedActorList(&lpPacket->m_packet.mCG_MISPLACED_ACTOR_LIST,lpPacket->m_iOwner);
				break;

			case	dCG_ASK_PLAYER_INFO			:
				receiveAskPlayerInfo((CG_ASK_PLAYER_INFO*)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_CHECK_PLAYER_CHECK_SUM	:
				receiveCheckPlayerCheckSum((CG_CHECK_PLAYER_CHECK_SUM *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;
				
			case	dCG_CHANGE_DOOR_STATUS				:
				receiveChangeDoorStatus((CG_CHANGE_DOOR_STATUS*)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_TOUCH_EVENT_AREA			:
				receiveTouchEventArea((CG_TOUCH_EVENT_AREA *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_MISPLACED_ACTOR	:
				receiveMisplacedActorList(&lpPacket->m_packet.mCG_MISPLACED_ACTOR_LIST,lpPacket->m_iOwner);
				break;

			case	dCG_ARCA_WORK					:
				receiveArcaWork((CG_ARCA_WORK *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;
				
			case	dCG_ASK_ARCA_INFO				:
				receiveAskArcaInfo((CG_ASK_ARCA_INFO *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_DISARM_FIELD_TRAP			:
				receiveDisarmFieldTrap((CG_DISARM_FIELD_TRAP *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_PUT_BANNER					:
				receivePutBanner((CG_PUT_BANNER *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_ASK_BANNER_TEXT				:
				receiveAskBannerText((CG_ASK_BANNER_TEXT *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_SEARCH_RELAY_STATION_ITEM	:
				receiveSearchTradeStationItem((CG_SEARCH_RELAY_STATION_ITEM *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_ASK_DOOR_INFO				:
				receiveAskDoorInfo((CG_ASK_DOOR_INFO *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_BUY_CARROT_SHOP_ITEM		:
				receiveBuyCarrotShopItem((CG_BUY_CARROT_SHOP_ITEM *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_SET_MOVE_ABILITY		:
				setMoveAbility(&lpPacket->m_packet.mCG_SET_MOVE_ABILITY,lpPacket->m_iOwner);
				break;

			case	dCG_TRANSFORMATION		:
				transformation(&lpPacket->m_packet.mCG_TRANSFORMATION,lpPacket->m_iOwner,eTR_ASK_OF_CLIENT);
				break;

			case	dCG_STOP				:
				stopPlayer(&lpPacket->m_packet.mCG_STOP,lpPacket->m_iOwner);
				break;

			case	dCG_TOGGLE_SITDOWN		:
				toggleSitdown(&lpPacket->m_packet.mCG_TOGGLE_SITDOWN,lpPacket->m_iOwner);
				break;

			case	dCG_MOVE_FIELD			:
				moveField(&lpPacket->m_packet.mCG_MOVE_FIELD,lpPacket->m_iOwner);
				break;

			case	dCG_ACTION_TO_OBJECT	:
				receiveActionToObject((CG_ACTION_TO_OBJECT *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case	dCG_ACTION_TO_ACTOR		:
				actionToActor(&lpPacket->m_packet.mCG_ACTION_TO_ACTOR,lpPacket->m_iOwner);
				break;

			case	dCG_ACTION_TO_GROUND	:
				actionToGround(&lpPacket->m_packet.mCG_ACTION_TO_GROUND,lpPacket->m_iOwner);
				break;

			case	dCG_ACTION_QUICK		:
				actionQuick(&lpPacket->m_packet.mCG_ACTION_QUICK,lpPacket->m_iOwner,TRUE);
				break;

			case	dCG_ENGAGE_BATTLE		:
				engageBattle(&lpPacket->m_packet.mCG_ENGAGE_BATTLE,lpPacket->m_iOwner);
				break;

			case	dCG_STOP_BATTLE			:
				stopBattle(&lpPacket->m_packet.mCG_STOP_BATTLE,lpPacket->m_iOwner);
				break;

			case	dCG_INCREASE_STATE		:
				increaseCharacterState(&lpPacket->m_packet.mCG_INCREASE_STATE,lpPacket->m_iOwner);
				break;

			case	dCG_INCREASE_ABILITY_LEVEL	:
				increaseAbilityLevel(&lpPacket->m_packet.mCG_INCREASE_ABILITY_LEVEL,lpPacket->m_iOwner);
				break;

			case	dCG_ASK_ACTOR_INFO	:
				receiveAskActorInfo(&lpPacket->m_packet.mCG_ASK_ACTOR_INFO,lpPacket->m_iOwner);
				break;

			case dCG_SET_REACTION_STATUS	:	//	리액션 상태 설정
				setReactionStatus(&lpPacket->m_packet.mCG_SET_REACTION_STATUS,lpPacket->m_iOwner);
				break;

			case dCG_TURN_ON_PASSIVE_SKILL	:
				setPassiveSkillStatus(&lpPacket->m_packet.mCG_TURN_ON_PASSIVE_SKILL,lpPacket->m_iOwner);
				break;

			case dCG_EXIT_GAME				:
				exitGame(&lpPacket->m_packet.mCG_EXIT_GAME,lpPacket->m_iOwner);
				break;

			case dCG_CALL_MAGIC_CARPET					:
				receiveCallMagicCarpet(&lpPacket->m_packet.mCG_CALL_MAGIC_CARPET,lpPacket->m_iOwner);
				break;

			case dCG_CHANGE_EMBLEM_SHAPE				:
				receiveChangeEmblemShape((CG_CHANGE_EMBLEM_SHAPE *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_REMEMBER_ORB_LOCATION			:
				receiveRememberOrbLocation(&lpPacket->m_packet.mCG_REMEMBER_ORB_LOCATION,lpPacket->m_iOwner);
				break;

			case dCG_TELEPORT_TO_ORB_LOCATION		:
				receiveTeleportToOrbLocation(&lpPacket->m_packet.mCG_TELEPORT_TO_ORB_LOCATION,lpPacket->m_iOwner);
				break;
				
			case dCG_RECOVER_DEATH_PENELTY			:
				receiveAskRecoverDeathPenelty((CG_RECOVER_DEATH_PENELTY *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_WARP_TO_NEAR_VILLAGE				:
				receiveWarpToNearVillage((CG_WARP_TO_NEAR_VILLAGE *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_ENTER_PORTAL						:
				receiveEnterPortal((CG_ENTER_PORTAL *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;


//━━━CFieldCartPacket.cpp
			case dCG_DESTROY_CART_ITEM					:	//	카트오픈
				receiveAskDestroyCartItem(&lpPacket->m_packet.mCG_DESTROY_CART_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_CART_OPEN							:	//	카트오픈
				receiveAskCartOpen(&lpPacket->m_packet.mCG_CART_OPEN,lpPacket->m_iOwner);
				break;

			case dCG_WITHDRAW_ITEM_FROM_THE_CART		:	//	카트에서 아이템 꺼내기
				receiveWithdrawItemFromTheCart(&lpPacket->m_packet.mCG_WITHDRAW_ITEM_FROM_THE_CART,lpPacket->m_iOwner);
				break;

			case dCG_CLOSE_CART							:
				receiveCloseCart(&lpPacket->m_packet.mCG_CLOSE_CART,lpPacket->m_iOwner);
				break;
				
			case dCG_OPEN_CARROT_SHOP	:
				receiveOpenCarrotShop((CG_OPEN_CARROT_SHOP *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

//━━━CFieldItemPacket.cpp
			case dCG_USE_SOUND_OF_LEADERS_BELL	:
				receiveAskUseSoundOfLeadersBell((CG_USE_SOUND_OF_LEADERS_BELL *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;
				
			case dCG_ASK_ITEM_PACK_NAME		:
				receiveAskItemPackName((CG_ASK_ITEM_PACK_NAME *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_PICK_ITEM				:	//	필드에 아이템을 줏었다!
				pickItem(&lpPacket->m_packet.mCG_PICK_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_REPAIR_ITEM			:	//	필드에 아이템을 줏었다!
				repairItem((CG_REPAIR_ITEM*)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_EQUIP_ITEM				:	//	아이템 장비
				equipItem(&lpPacket->m_packet.mCG_EQUIP_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_RELOAD_BELT_ITEM			:	//	아이템 리로드
				reloadBeltItem(&lpPacket->m_packet.mCG_RELOAD_BELT_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_USE_ITEM				:	//	아이템 사용
				useItem(&lpPacket->m_packet.mCG_USE_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_USE_ITEM_TO_ITEM		:
				useItemToItem(&lpPacket->m_packet.mCG_USE_ITEM_TO_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_USE_BELT_ITEM			:	//	벨트에 아이템을 사용한다.
				useBeltItem(&lpPacket->m_packet.mCG_USE_BELT_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_ASK_ITEM_DATA			:	//	아이템 정보를 요청한다.
				askItemData(lpPacket->m_iOwner);
				break;

			case dCG_ASK_INVENTORY_DATA		:	//	뭔가 잘못 되었다!! 인벤토리 데이터를 요청한다.
				askInventoryData(lpPacket->m_iOwner);
				break;
			case dCG_ASK_EQUIPMENT_DATA		:	//	뭔가 잘못 되었다!! 장비 데이터를 요청한다.
				askEquipmentData(lpPacket->m_iOwner);
				break;

			case dCG_DROP_ITEM				:	//	아이템 버리기
				dropItem(&lpPacket->m_packet.mCG_DROP_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_CHANGE_ITEM_PLACE		:	//	아이템 버리기
				changeItemPlace(&lpPacket->m_packet.mCG_CHANGE_ITEM_PLACE,lpPacket->m_iOwner);
				break;

			case dCG_STRIP_EQUIPMENT		:	//	장비 벗기
				stripEquipment(&lpPacket->m_packet.mCG_STRIP_EQUIPMENT,lpPacket->m_iOwner);
				break;

			case dCG_DESTROY_ITEM			:	//	장비 벗기
				destroyItem(&lpPacket->m_packet.mCG_DESTROY_ITEM,lpPacket->m_iOwner);
				break;

//━━━CFieldTradePacket.cpp
			case dCG_REQUEST_TRADE			:	//	트레이드 요청
				operateRequestTradePacket(&lpPacket->m_packet.mCG_REQUEST_TRADE,lpPacket->m_iOwner);
				break;
			case dCG_TRADE_REPLY			:	//	트레이드 수락
				operateAcceptTradePacket(&lpPacket->m_packet.mCG_TRADE_REPLY,lpPacket->m_iOwner);
				break;
			case dCG_CANCEL_TRADE			:	//	트레이드 취소
				operateCancelTradePacket(&lpPacket->m_packet.mCG_CANCEL_TRADE,lpPacket->m_iOwner);
				break;
			case dCG_ADD_TRADE_ITEM			:	//	트레이드 아이템 추가
				operateAddTradeItemPacket(&lpPacket->m_packet.mCG_ADD_TRADE_ITEM,lpPacket->m_iOwner);
				break;
			case dCG_CHANGE_TRADE_ITEM_DATA			:
				operateSetTradeItemPacket(&lpPacket->m_packet.mCG_CHANGE_TRADE_ITEM_DATA,lpPacket->m_iOwner);
				break;
			case dCG_REMOVE_TRADE_ITEM		:	//	트레이드 아이템 제거
				operateRemoveTradeItemPacket(&lpPacket->m_packet.mCG_REMOVE_TRADE_ITEM,lpPacket->m_iOwner);
				break;
			case dCG_SET_TRADE_GOLD			:	//	트레이드 할 골드 설정
				operateSetTradeGoldPacket(&lpPacket->m_packet.mCG_SET_TRADE_GOLD,lpPacket->m_iOwner);
				break;
			case dCG_READY_TRADE			:	//	트레이드 준비
				operateReadyTradePacket(&lpPacket->m_packet.mCG_READY_TRADE,lpPacket->m_iOwner);
				break;
			case dCG_PERMIT_TRADE			:	//	트레이드 허락
				operatePermitTradePacket(&lpPacket->m_packet.mCG_PERMIT_TRADE,lpPacket->m_iOwner);
				break;

//━━━CFieldEventPacket.cpp
			case dCG_SELECT_NPC				:
				selectNpc(&lpPacket->m_packet.mCG_SELECT_NPC,lpPacket->m_iOwner);
				break;

			case dCG_SELECT_SPEECH			:
				selectSpeech(&lpPacket->m_packet.mCG_SELECT_SPEECH,lpPacket->m_iOwner);
				break;

			case dCG_BUY_ITEM				:
				buyItem(&lpPacket->m_packet.mCG_BUY_ITEM,lpPacket->m_iOwner);
				break;

			case dCG_SELL_ITEM				:
				sellItem(&lpPacket->m_packet.mCG_SELL_ITEM,lpPacket->m_iOwner);
				break;
				
			case dCG_CANCEL_QUEST			:
				cancelQuest(&lpPacket->m_packet.mCG_CANCEL_QUEST,lpPacket->m_iOwner);
				break;
			case dCG_SUMMON_EVENT_MONSTER			:
				{
					for(int i=0;i<lpPacket->m_packet.mCG_SUMMON_EVENT_MONSTER.bf5Count ;++i)
						summonEventMonster(&lpPacket->m_packet.mCG_SUMMON_EVENT_MONSTER,lpPacket->m_iOwner);
					break;
				}
//━━━CFieldBankPacket.cpp
			case dCG_STORE_ITEM_TO_THE_BANK		://	은행에 아이템 저장 요청
				storeItemToTheBank(&lpPacket->m_packet.mCG_STORE_ITEM_TO_THE_BANK,lpPacket->m_iOwner);
				break;
			case dCG_STORE_GOLD_TO_THE_BANK		://	은행에 골드 저장 요청
				storeGoldToTheBank(&lpPacket->m_packet.mCG_STORE_GOLD_TO_THE_BANK,lpPacket->m_iOwner);
				break;
			case dCG_WITHDRAW_ITEM_FROM_THE_BANK	://	은행에서 아이템 꺼내기
				withdrawItemFromTheBank(&lpPacket->m_packet.mCG_WITHDRAW_ITEM_FROM_THE_BANK,lpPacket->m_iOwner);
				break;
			case dCG_WITHDRAW_GOLD_FROM_THE_BANK	://	은행에서 골드꺼내기
				withdrawGoldFromTheBank(&lpPacket->m_packet.mCG_WITHDRAW_GOLD_FROM_THE_BANK,lpPacket->m_iOwner);
				break;
			case dCG_MOVE_BANK_ITEM				://	은행 아이템 위치 변경
				moveBankItem(&lpPacket->m_packet.mCG_MOVE_BANK_ITEM,lpPacket->m_iOwner);
				break;
			case dCG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	:
				moveInventoryItemWhenBankTransaction(&lpPacket->m_packet.mCG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION,lpPacket->m_iOwner);
				break;
			case dCG_FINISH_BANK_TRANSACTION	://	은행 거래 종료
				finishBankTransaction(&lpPacket->m_packet.mCG_FINISH_BANK_TRANSACTION,lpPacket->m_iOwner);
				break;
			case dCG_CANCEL_BANK_TRANSACTION	://	은행 거래 취소
				cancelBankTransaction(&lpPacket->m_packet.mCG_CANCEL_BANK_TRANSACTION,lpPacket->m_iOwner);
				break;
				
//━━━CFieldCommunityPacket.cpp
			case dCG_CHAT					:
				chat(&lpPacket->m_packet.mCG_CHAT,lpPacket->m_iOwner);
				break;

			case dCG_CHANGE_PARTY_STATUS	:		//	파티 참가 요청을 한다.
				receiveChangePartyStatus((CG_CHANGE_PARTY_STATUS *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;
				
			case dCG_ASK_JOIN_PARTY			:		//	파티 참가 요청을 한다.
				receiveAskJoinPartyMessage(&lpPacket->m_packet.mCG_ASK_JOIN_PARTY,lpPacket->m_iOwner);
				break;
			case dCG_ASK_JOIN_PARTY_REPLY	:	//	파티 & 연합 파티 참가 요청 결과
				receiveAskJoinPartyReplyMessage(&lpPacket->m_packet.mCG_ASK_JOIN_PARTY_REPLY,lpPacket->m_iOwner);
				break;		
			case dCG_ASK_PARTY_LIST			:	//	파티 리스트 요청
				receiveAskPartyListMessage(&lpPacket->m_packet.mCG_ASK_PARTY_LIST,lpPacket->m_iOwner);
				break;
			case dCG_LEAVE_PARTY			:	//	파티에서 탈퇴하기
				receiveLeavePartyMessage(&lpPacket->m_packet.mCG_LEAVE_PARTY,lpPacket->m_iOwner);
				break;
			case dCG_PARTY_LEADER_WORK	:	//	파티에서 쫓아내기(리더만 가능)
				receivePartyLeaderWork(&lpPacket->m_packet.mCG_PARTY_LEADER_WORK,lpPacket->m_iOwner);
				break;
			case dCG_CHANGE_PARTY_NAME		:	//	파티 이름 바꾸기
				receiveChangePartyNameMessage(&lpPacket->m_packet.mCG_CHANGE_PARTY_NAME,lpPacket->m_iOwner);
				break;
			case dCG_ASK_PARTY_INFO			:	//	자신이 속한 파티의 정보 요청
				receiveAskPartyInfoMessage(&lpPacket->m_packet.mCG_ASK_PARTY_INFO,lpPacket->m_iOwner);
				break;
			case dCG_FIND_USER				:	//	자신이 속한 파티의 정보 요청
				receiveFindUserMessage(&lpPacket->m_packet.mCG_FIND_USER,lpPacket->m_iOwner);
				break;
			case dCG_REQUEST_ADD_FRIEND		:
				receiveRequestAddFriendMessage(&lpPacket->m_packet.mCG_REQUEST_ADD_FRIEND,lpPacket->m_iOwner);
				break;

			case dCG_REMOVE_NAME_IN_TARGET_FRIEND_LIST	:
				receiveRemoveNameInTargetFriendList((CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST*)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT:
				receiveRemoveNameInTargetFriendListResult((CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_REPLY_FOR_ADD_FRIEND	:
				receiveReplyForAddFriendMessage(&lpPacket->m_packet.mCG_REPLY_FOR_ADD_FRIEND,lpPacket->m_iOwner);
				break;

			case dCG_CHANGE_COMMUNITY_STATUS:
				receiveChangeCommunityStatus(&lpPacket->m_packet.mCG_CHANGE_COMMUNITY_STATUS,lpPacket->m_iOwner);
				break;

			case dCG_DEBUG_MESSAGE			:
				receiveDebugMessage(&lpPacket->m_packet.mCG_DEBUG_MESSAGE,lpPacket->m_iOwner);
				break;

//━━━CFieldGuildPacket.cpp
			case dCG_CREATE_GUILD			:
				receiveCreateGuild(&lpPacket->m_packet.mCG_CREATE_GUILD,lpPacket->m_iOwner);
				break;

			case dCG_ASK_GUILD_INFO			:	
				receiveAskGuildInfo(&lpPacket->m_packet.mCG_ASK_GUILD_INFO,lpPacket->m_iOwner);
				break;

			case dCG_ASK_GUILD_MEMBER_LIST	:	
				receiveAskGuildMemberList(&lpPacket->m_packet.mCG_ASK_GUILD_MEMBER_LIST,lpPacket->m_iOwner);
				break;

			case dCG_ASK_DISSOLUTION_GUILD	:
				receiveAskDissoulutionGuild(lpPacket->m_iOwner);
				break;

			case dCG_ASK_JOIN_GUILD			:
				receiveAskJoinGuild(&lpPacket->m_packet.mCG_ASK_JOIN_GUILD,lpPacket->m_iOwner);
				break;

			case dCG_REPLY_ASK_JOIN_GUILD	:
				receiveReplyAskJoinGuild(&lpPacket->m_packet.mCG_REPLY_ASK_JOIN_GUILD,lpPacket->m_iOwner);
				break;

			case dCG_ASK_CHANGE_GUILD_NOTICE			:	//	길드 공지 변경 요청
				receiveAskChangeGuildNotice(&lpPacket->m_packet.mCG_ASK_CHANGE_GUILD_NOTICE,lpPacket->m_iOwner);
				break;
			case dCG_ASK_CHANGE_GUILD_EXP_TAX			:	//	길드 경험치 세율 변경 요청
				receiveAskChangeGuildExpTax(&lpPacket->m_packet.mCG_ASK_CHANGE_GUILD_EXP_TAX,lpPacket->m_iOwner);
				break;

			case	dCG_GUILD_MEMBER_WORK	:
				receiveGuildMemberWork((CG_GUILD_MEMBER_WORK*)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_ASK_DISMISS_SUB_GUILD_MASTER		:	//	서브 길드 마스터 해임 건의?
				receiveAskDismissSubGuildMaster(&lpPacket->m_packet.mCG_ASK_DISMISS_SUB_GUILD_MASTER,lpPacket->m_iOwner);
				break;
			case dCG_ASK_APPOINTMENT_SUB_GUILD_MASTER	:	//	서브 길드 마스터 임명 요청
				receiveAskAppointmentSubGuildMaster(&lpPacket->m_packet.mCG_ASK_APPOINTMENT_SUB_GUILD_MASTER,lpPacket->m_iOwner);
				break;
			case dCG_ASK_DISMISS_GUILD_CONGRESS			:
				receiveAskDismissGuildCongress(&lpPacket->m_packet.mCG_ASK_DISMISS_GUILD_CONGRESS,lpPacket->m_iOwner);
				break;
			case dCG_ASK_APPOINTMENT_GUILD_CONGRESS		:
				receiveAskAppoitmentGuildCongress(&lpPacket->m_packet.mCG_ASK_APPOINTMENT_GUILD_CONGRESS,lpPacket->m_iOwner);
				break;
				
			case dCG_ASK_BANISH_GUILD_MEMBER			:	//	길드 멤버 추방 요청
				receiveAskBanishGuildMember(&lpPacket->m_packet.mCG_ASK_BANISH_GUILD_MEMBER,lpPacket->m_iOwner);
				break;
			case dCG_ASK_LEAVE_GUILD					:	//	길드 탈퇴 요청
				receiveAskLeaveGuild(&lpPacket->m_packet.mCG_ASK_LEAVE_GUILD,lpPacket->m_iOwner);
				break;

			case dCG_ASK_CHANGE_GUILD_MASTER			:
				receiveAskChangeGuildMaster(&lpPacket->m_packet.mCG_ASK_CHANGE_GUILD_MASTER,lpPacket->m_iOwner);
				break;

			case dCG_READY_TO_MAKE_GUILD				:
				receiveReadyToMakeGuild(&lpPacket->m_packet.mCG_READY_TO_MAKE_GUILD,lpPacket->m_iOwner);
				break;

			case dCG_REGIST_REGULAR_GUILD_MEMBER		:
				receiveAskRegistRegularMember(&lpPacket->m_packet.mCG_REGIST_REGULAR_GUILD_MEMBER,lpPacket->m_iOwner);
				break;

			case dCG_INCREASE_GUILD_SKILL_LEVEL			:
				receiveIncreaseGuildSkillLevel(&lpPacket->m_packet.mCG_INCREASE_GUILD_SKILL_LEVEL,lpPacket->m_iOwner);
				break;

			case dCG_ASK_GUILD_BATTLE_SCHEDULE			:
				receiveAskGuildBattleSchedule(&lpPacket->m_packet.mCG_ASK_GUILD_BATTLE_SCHEDULE,lpPacket->m_iOwner);
				break;

			case dCG_REQUEST_GUILD_BATTLE				:	//	길드전 요청
				receiveRequestGuildBattle(&lpPacket->m_packet.mCG_REQUEST_GUILD_BATTLE,lpPacket->m_iOwner);
				break;

			case dCG_CANCEL_GUILD_BATTLE				:
				receiveCancelGuildBattle(&lpPacket->m_packet.mCG_CANCEL_GUILD_BATTLE,lpPacket->m_iOwner);
				break;

			case dCG_CLOSE_GUILD_BATTLE_SCHEDULER		:
				receiveCloseGuildBattleScheduler(lpPacket->m_iOwner);
				break;

			case dCG_SELECT_GUILD_MARK					:
				receiveSelectGuildMark((CG_SELECT_GUILD_MARK *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_ASK_GUILD_MARK_INFO				:
				receiveAskGuildMarkInfo((CG_ASK_GUILD_MARK_INFO *)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_ASK_SIMPLE_GUILD_INFO				:
				receiveAskSimpleGuildInfo((CG_ASK_SIMPLE_GUILD_INFO*)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

			case dCG_GUILD_CHECK_SUM					:
				receiveGuildCheckSum((CG_GUILD_CHECK_SUM*)&lpPacket->m_packet,lpPacket->m_iOwner);
				break;

//━━━CFieldPitchmanShopPacket.cpp
				
			case dCG_ASK_OPEN_THE_PITCHMAN_SHOP			:	//	노점상 열거라고 요청
				receiveAskOpenThePitchmanShop(&lpPacket->m_packet.mCG_ASK_OPEN_THE_PITCHMAN_SHOP,lpPacket->m_iOwner);
				break;
			case dCG_ADD_PITCHMAN_SHOP_ITEM			:	//	노점상 아이템 등록
				receiveAddPitchmanShopItem(&lpPacket->m_packet.mCG_ADD_PITCHMAN_SHOP_ITEM,lpPacket->m_iOwner);
				break;
			case dCG_REMOVE_PITCHMAN_SHOP_ITEM		:	//	노점상에서 아이템 제거
				receiveRemovePitchmanShopItem(&lpPacket->m_packet.mCG_REMOVE_PITCHMAN_SHOP_ITEM,lpPacket->m_iOwner);
				break;
			case dCG_CHANGE_PITCHMAN_SHOP_INFO			:	//	노점상 상태 변경 (오픈,클로즈,종료)
				receiveChangePitchmanShopInfo(&lpPacket->m_packet.mCG_CHANGE_PITCHMAN_SHOP_INFO,lpPacket->m_iOwner);
				break;
			case dCG_BUY_PITCHMAN_SHOP_ITEM			:	//	노점상에서 아이템 구입
				receiveBuyPitchmanShopItem(&lpPacket->m_packet.mCG_BUY_PITCHMAN_SHOP_ITEM,lpPacket->m_iOwner);
				break;
			case dCG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE	:
				receiveChangePitchmanShopItemPlace(&lpPacket->m_packet.mCG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE,lpPacket->m_iOwner);
				break;

			case dCG_CLOSE_PITCHMAN_SHOP				:
				receiveClosePitchmanShop(&lpPacket->m_packet.mCG_CLOSE_PITCHMAN_SHOP,lpPacket->m_iOwner);
				break;

			case dCG_ASK_PITCHMAN_SHOP_INFO				:	//	노점상 정보 요청
				receiveAskPitchmanShopInfo(&lpPacket->m_packet.mCG_ASK_PITCHMAN_SHOP_INFO,lpPacket->m_iOwner);
				break;

			case dCG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST	:
				receiveAskSimplePithmanShopInfoList(&lpPacket->m_packet.mCG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST,lpPacket->m_iOwner);
				break;
		}

		g_game.endProfileByID(lpPacket->m_packet.base.wType);
	}

	mDS_PK.m_iLastUpdateClientPacket	=	-1;

	if	(g_bIsTestBCS_Deliver	&&	IsAvailBCConnect())
		operateDeliverPacket();

	for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		for (i=m_aiActorCount[iTeam];i;)
		{
			--i;

			cACTOR	*lpPlayer	=	getActor(m_awActorList[iTeam][i]);

			if	(!lpPlayer	||	!lpPlayer->isPlayer()	||	lpPlayer->m_wJob	>=	dPLAYER_JOB_COUNT)
				continue;

			sendStoredPacketsToPlayer(lpPlayer);
		}

	m_queSendPacket.reset();	//	센드 패킷을 초기화 한다.

	m_iWorkPacketCount					=	0;

	mDS_PK.m_iLastUpdateClientPacket	=	-2;
}