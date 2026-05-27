#include "cACTOR.H"
#include "cFIELD.H"
#include "cNPC.H"
#include "debugCode.H"
#include "CGame.H"
//	비전투시 AI
	//	경계
	//	방황

//	공격 이동시 AI
	//	한놈만 패
	//	근처에 있는 넘 패
	//	

//	전투시 AI
	//	한넘만 패

BOOL
cACTOR::operateGuildGuardianAI()
{
	if	(m_iGuildGuardianType	==	eGUILD_GUARDIAN_HOLLOW_KNIGHT)	//	할로우 나이트
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::operateAi()
{
	if	(m_iGuildGuardianType)
	{
		if	(operateGuildGuardianAI())
			return	TRUE;
	}

	if	(m_lpTarget && m_lpTarget->m_wSerialInField	==	0xffff)
	{
		stop();
		setTarget(NULL);
		
		setAiReady();

		return TRUE;
	}

	if	(m_iShakleHP)
	{
		if	(!m_lpTarget	||	m_lpTarget->m_wSerialInField	!=	m_wSerialInField)
		{
			setAiFight(this);
			return	TRUE;
		}
	}

//	이동관련 업데이트 
	{
		if	(m_bIsMoving && g_iCurrentMoveNumber ==	m_iMoveNumber)
		{
			m_wStandTime	=	0;

			updateMove();
		}
		else
		{
			if	(m_wStandTime	==	0)
				m_lpField->addStandActor(m_wSerialInField);

			m_wStandTime++;
		}
	}

	if	(m_isAction		)
		updateAction();

	if	(m_bf1IsTraceGuide)
	{
		operateTraceGuide();

		return	TRUE;
	}

	if	(m_wBiteTarget	!=	0xffff)
		return	TRUE;

	if	(m_wJob	==	dJOB_MONSTER_SAGE_MASTER)
	{
		if	(m_bf1IsBlockToRevenge	==	FALSE)
		{
			if	(m_wAiState	==	eAS_SAGE_MASTER_TRACE)
				return	operateSageMasterTraceAi();

			return	operateSageMasterIronBall();
		}
	}

	//JBC			08-09-18
	if(m_lpField->m_bIsUseFindWay)
		if(m_isComeBackToRegenPlace && isMonster())
		{
			BOOL	bISResult	=	operateComeBackToRegenPlaceAi();

			if (!bISResult)
				m_isComeBackToRegenPlace = FALSE;

			return bISResult;
		}

	switch(m_wAiState)
	{
		case	eAS_CONFUSE		:
			return	operateConfuseAi();

		case	eAS_READY		:
			return	operateReadyAi();

		case	eAS_MOVE		:
			return	operateMoveAi();
			
		case	eAS_APPROACH	:
			return	operateApproachAi();

		case	eAS_FIGHT		:
			return	operateFightAi();

		case	eAS_TM_READY	:
		{
			if	(m_wCharmedTime)
				return	operateReadyAi();

			return	operateTamedMonsterReadyAi();
		}

		case	eAS_TM_TRACE	:
			if	(m_wCharmedTime)
				return	operateReadyAi();

			return	operateTamedMonsterTraceAi();
	}	//	AI 업데이트

	return	TRUE;
}

//
//AI를 대기로 설정!
void
cACTOR::setAiType(int _iType)
{
	m_iAiType	=	_iType;
}

void
cACTOR::setAiState(int _iState)
{
	m_wAiState	=	_iState;
}

void
cACTOR::setAiReady()
{
	switch(m_iAiType)
	{
		case	eAT_TAMED_MONSTER	:
			m_wAiState	=	eAS_TM_READY;
			break;

		default	:
			m_wAiState	=	eAS_READY;
			break;
	}

	m_wUseSpecialAbilityStep	=	ePUSS_NONE;

	//JBC	변수 초기화		08-09-19
	m_isComeBackToRegenPlace	=	FALSE;

	if	(m_iGuildGuardianType	==	eGUILD_GUARDIAN_HOLLOW_KNIGHT)	//	할로우 나이트
		m_dwLastUpdateTime			=	0;

	resetBlockedMoveStress();

	setTarget(NULL);
	setCurrentTarget(NULL);

	stop();
}

//
//	AI를 이동으로 설정
void
cACTOR::setAiMove()
{
	resetBlockedMoveStress();

	m_wAiState				=	eAS_MOVE;
	m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

	addNpcMoveMessage(FALSE);
}

//
//AI를 접근-_-으로 설정
void
cACTOR::setAiApproach(cACTOR *_lpTarget,int _iX,int _iY)
{
	if	(m_lpField->m_bIsBlockBattle)
		return;

	if (_lpTarget)
	{
		setTarget(_lpTarget);

		m_wIsLocalAttack	=	FALSE;
		m_posTarget.x		=	_lpTarget->m_pos.x;
		m_posTarget.y		=	_lpTarget->m_pos.y;
	}
	else
	{
		if (_iX)
		{
			m_wIsLocalAttack	=	TRUE;
			m_posTarget.x		=	_iX;
			m_posTarget.y		=	_iY;
		}
	}

	resetBlockedMoveStress();

	if	(!_lpTarget && m_wIsLocalAttack == FALSE)
	{
		setAiReady();
		return;
	}
	
	m_wAiState					=	eAS_APPROACH;
	m_wApproachTime				=	0;
	m_wAnotherTargetBitMeCount	=	0;

	if (m_isExclusiveAction)
		return;

	stop();

	if	(m_wIsLocalAttack)
	{
		if	(isInAttackRange(_iX,_iY,m_wEngageRange))//_iX,_iY부터 _iRange거리 안에 있냐?
		{
			setAiFight(NULL,&m_engageAbility,_iX,_iY);

			return;
		}
	}
	else
	if (isTargetInAttackRange(_lpTarget,m_wEngageRange))	//	사정거리 안에 있다.
	{
		setAiFight(_lpTarget);
		return;
	}

	if (moveTo(m_posTarget.x,m_posTarget.y))
	{
		m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

		addNpcMoveMessage(FALSE);
	}
}

//
//AI를 전투중으로 설정
void
cACTOR::setAiFight(cACTOR *_lpTarget,cAbility *_lpEngageAbility,int _iX,int _iY)
{
	if	(m_lpField->m_bIsBlockBattle)
		return;

	if	(!isMonster()	&&	!isCopyActor())
		return;

	if	(m_wFleeTime	||	m_wDanceTime	|| m_wHwabyungTime)
		return;

	resetBlockedMoveStress();

	m_wIsLocalAttack		=	FALSE;
	m_wAiState				=	eAS_FIGHT;

	BOOL	bIsBeginBattle	=	FALSE;

//	공격을 시작한다.
	if	(_lpTarget		==	NULL)
	{
		m_posTarget.x	=	_iX;
		m_posTarget.y	=	_iY;
		m_wIsLocalAttack=	TRUE;
	}
	else
	if	(m_lpTarget == NULL || m_lpTarget->m_wSerialInField	!=	_lpTarget->m_wSerialInField)
	{
		CSkill	*lpSkill	=	m_engageAbility.getSkill();

		if	(m_iShakleHP	==	0)
		{
			if	(lpSkill && lpSkill->isAgressiveSkill() && _lpTarget->m_wSerialInField == m_wSerialInField && m_wConfuseTime == 0 )
			{
				setAiReady();
				return;
			}

			if	(m_iAiType	==	eAT_MONSTER)
			{
				cNPC *lpNpc	=	getNpc();

				if	(lpNpc)
				{
					lpNpc->checkBeginBattleEvent(this,_lpTarget);

					if	(m_wProcessPatternLevel	!=	0xffff)	//	이동 패턴 진행
						return;
				}
			}
		}

		setTarget(_lpTarget);

		bIsBeginBattle	=	TRUE;
	}

//	적절한 공격을 찾는다.
	if	(_lpEngageAbility)
		setEngageAbility(_lpEngageAbility);
	else
	{
		if	(!findEngageAbility(_lpTarget,bIsBeginBattle))
		{
			setAiReady();
			return;
		}
	}

	operateFightAi(TRUE);
}

//
//AI를 추적-o-으로 설정.
void
cACTOR::setAiTrace(cACTOR *_lpTarget)
{
	m_wAiState	=	eAS_TRACE;
}

//
//	_lpTarget을 추적한다.
BOOL
cACTOR::pursuit(cACTOR *_lpTarget)
{
	if (m_wIsLocalAttack == FALSE)
	{
		if	(GetOvalRange(m_pos.x,m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y)	>=	dMONSTER_MISSING_TARGET_RANGE)
			return	FALSE;

		if	(m_wTauntedToTargetTime)
		{
			if	(GetOvalRange(m_pos.x,m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y)	>=	dMONSTER_MISSING_TAUNT_TARGET_RANGE)
				m_wTauntedToTargetTime	=	0;
		}

		if	(m_posTarget.x	!=	m_lpTarget->m_pos.x	||	m_posTarget.y	!=	m_lpTarget->m_pos.y)
			resetBlockedMoveStress();

		m_posTarget.x	=	m_lpTarget->m_pos.x;
		m_posTarget.y	=	m_lpTarget->m_pos.y;
	}

	if	(!moveTo(m_posTarget.x,m_posTarget.y))
		return	TRUE;

	addNpcMoveMessage(FALSE);

	m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

	return	TRUE;
}

//
//	적을 찾아라!!
cACTOR*
cACTOR::findNeareastEnemy()
{
	return	m_lpField->findNeareastEnemy(m_wTeam,m_iLocalPart,m_pos.x,m_pos.y,m_sSight,this,TRUE);
}

BOOL
cACTOR::findEnemy()
{
	if	(m_lpField->m_bIsBlockBattle)
		return	FALSE;

	if	(m_wBlindTime	||	m_wHwabyungTime)
		return	FALSE;
	
	if	(m_iReaminSerchEnemyTime)
	{
		m_iReaminSerchEnemyTime--;

		return	FALSE;
	}
	
	if	(m_lpField->isSecretDungeon())
		m_iReaminSerchEnemyTime	=	dSYNC_FPS/8;
	else
		m_iReaminSerchEnemyTime	=	dSYNC_FPS/4;

	cACTOR	*lpTarget	=	findNeareastEnemy();

	if	(lpTarget)
	{
		setAiFight(lpTarget);

		return	TRUE;
	}

	return	FALSE;
}

//
//	레뒤~상태...
BOOL
cACTOR::operateReadyAi()
{
	if	(m_wFleeTime || m_wInMagicBoxTime	||	m_wDanceTime	||	m_wHwabyungTime || m_wLordOperator != 0xffff)
		return	TRUE;
	
	BOOL	bIsAggressive	=	FALSE;

	if	(isCopyActor() || m_wCharmedTime)
		bIsAggressive	=	TRUE;
	else
	if	(isMonster() && m_wBattleInclination >= dBI_AGGRESSIVE_SOLO && m_wLordActor	==	0xffff)	//	디스플레이스먼트 상태가 아니다.
		bIsAggressive	=	TRUE;

	if	(bIsAggressive)
		if	(findEnemy())
			return	TRUE;

	if	(m_iGuildGuardianType	==	eGUILD_GUARDIAN_HOLLOW_KNIGHT)	//	할로우 나이트
	{
		if	(m_pos.x	!=	m_posFeignDeath.x	||	m_pos.y	!=	m_posFeignDeath.y)
		{
			if	(m_dwLastUpdateTime	==	0)
				m_dwLastUpdateTime	=	m_lpField->m_iFrameCounter;

			if	((int)m_dwLastUpdateTime	<	m_lpField->m_iFrameCounter-dSYNC_FPS)
				changePos(m_posFeignDeath.x,m_posFeignDeath.y,TRUE);
		}

		return	TRUE;
	}

	cNPC	*lpNpc	=	getNpc();

	if	(!lpNpc	||	lpNpc->isNotMoveNpc())	//	이동 안 한다.
		return	TRUE;

//	아직 이동 업데이트 할 시간이 안 됐다.
	if	(m_dwLastUpdateTime + lpNpc->m_wWanderCycle*dSYNC_FPS > (DWORD)m_lpField->m_iFrameCounter || m_wRallyTime)
		return	TRUE;

	m_dwLastUpdateTime	=	m_lpField->m_iFrameCounter;

	cAreaInfo	*lpArea	=	m_lpField->getArea(lpNpc->m_wMoveArea);

	if	(lpArea)
	{
		for (int iTryCount=20;iTryCount;--iTryCount)
		{
			cPOINT		posMove;

			lpArea->getRandomPos(&posMove);

			int	iCurrentRange	=	GetOvalRange(m_pos.x,m_pos.y,posMove.x,posMove.y);

			if	(iCurrentRange	>	dDEFAULT_WANDER_RANGE*dDEFAULT_WANDER_RANGE)
			{
				int	iAngle		=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,posMove.x,posMove.y);
				int	iDistance	=	random(dDEFAULT_WANDER_RANGE*2)-dDEFAULT_WANDER_RANGE;

				posMove.x		=	m_pos.x;
				posMove.y		=	m_pos.y;

				g_protractor.getAnglePos(&posMove,iAngle,iDistance);
			}

			if	(!lpArea->isIn(posMove.x,posMove.y))
				continue;
				
			if	(moveTo(posMove.x,posMove.y))
				break;
		}

		if	(iTryCount == 0)
			return	FALSE;
	}
	else
	{
		for (int iTryCount=20;iTryCount;--iTryCount)
		{
			int		iAngle	=	random(360);
			int		iDistance=	random(dDEFAULT_WANDER_RANGE*2)-dDEFAULT_WANDER_RANGE;
			cPOINT	posMove;

			posMove.x		=	m_pos.x;
			posMove.y		=	m_pos.y;

			g_protractor.getAnglePos(&posMove,iAngle,iDistance);

			if	(moveTo(posMove.x,posMove.y))
				break;
		}

		if	(iTryCount == 0)
			return	FALSE;
	}

	setAiMove();

	return	TRUE;
}

//
//	레뒤~상태...
BOOL
cACTOR::operateMoveAi()
{
//	어그레시브 스타일이면 주변에 적-_-검색
	if	(m_wFleeTime	== 0 && m_wBattleInclination >= dBI_AGGRESSIVE_SOLO	&&	m_wProcessPatternStatus	==	0xffff)
		if	(findEnemy())
			return TRUE;

	if	(m_wDanceTime || m_wInMagicBoxTime || m_iShakleHP)
		return	TRUE;

	if	(m_bIsMoving)	//	이동 중이면 업데이트 타임에 맞춰서 이동 업데이트를 해 준다.
	{
		int	iMoveUpdateTime	=	dMOVE_UPDATE_TIME;

		if	(m_wCharmedTime	||	m_iAiType	>=	eAT_SLAVE)	//	챠밍되거나 디스플레이스먼트 되거나 한 몹
			iMoveUpdateTime	=	dMOVE_UPDATE_TIME/2;

		if (m_dwLastMoveMessageTime	+	iMoveUpdateTime	<=	(DWORD)m_lpField->m_iFrameCounter)
		{
			m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

			addNpcMoveMessage(TRUE);
		}

		if (m_wObjectItemForCatch	!=	0xffff)
		{
			CDroppedItem	*lpItem	=	m_lpField->m_droppedItem.get(m_wObjectItemForCatch);

			if (!lpItem || lpItem->m_item.m_dwSerial	!=	m_dwObjectItemSerialForCatch)
			{
				m_wObjectItemForCatch		=	0xffff;
				m_dwObjectItemSerialForCatch=	0xffffffff;

				stopBattleForNPC(TRUE);

				return	TRUE;
			}

			int	iRange	=	GetOvalRange(m_pos.x,m_pos.y,lpItem->m_pos.x,lpItem->m_pos.y);

			if (iRange	<	dPICK_ABLE_RANGE)
			{
				cACTOR	*lpTamer	=	getTamer();

				if (lpTamer)
				{
					BOOL	bIsDroppedItem	=	FALSE;
					cItem	itemLog;

					if (lpItem->m_wDropMethod	==	eDIM_PLAYER_DROP)
						bIsDroppedItem	=	TRUE;	//	사용자가 버린거다.

					itemLog.copy(&lpItem->m_item);

					int		iPickResult;

					if	(lpItem->m_item.isMoneyItem())
						iPickResult	=	addGoldItemToInventory(&lpItem->m_item,eGET_GOLD_BY_PICK_DROPPED_GOLD);
					else
						iPickResult	=	addItemToInventory(&lpItem->m_item);

					if (iPickResult	==	eUIR_OK)
					{
						if (lpItem->m_item.m_wBaseItem	==	0xffff)	//	아이템을 완전히 줏었다.
						{
							m_lpField->m_droppedItem.remove(m_wObjectItemForCatch);
							
							SG_REMOVE_DROPPED_ITEM	sgRemoveDroppedItem;

							sgRemoveDroppedItem.base.set(sizeof(SG_REMOVE_DROPPED_ITEM),dSG_REMOVE_DROPPED_ITEM);
							sgRemoveDroppedItem.wRemoveItem	=	m_wObjectItemForCatch;

							m_lpField->addSendPacket((ALL_MSG*)&sgRemoveDroppedItem,0);

							int	iRequireLevel	=	itemLog.getRequireLevel();

							if (bIsDroppedItem || iRequireLevel	>=	50 || itemLog.isSuperUniqueItem() || itemLog.isUniqueItem() || itemLog.isRareItem() || (itemLog.m_wBaseItem == dITEM_MONEY && itemLog.m_dwSerial >= 10000) )
								lpTamer->sendPickItemLog(&itemLog,ePIM_BEAST_PICK);

							m_wIsReturnToTransferItem	=	TRUE;
						}
					}
				}

				m_wObjectItemForCatch		=	0xffff;
				m_dwObjectItemSerialForCatch=	0xffffffff;

				stopBattleForNPC(TRUE);
			}
		}

		return	TRUE;
	}

	if	(m_wProcessPatternStatus	==	eCP_move)
		operateNextStepPattern();
	else
		setAiReady();

	m_dwLastUpdateTime	=	m_lpField->m_iFrameCounter;

	return	TRUE;
}

//
//	순찰
//	방향을 바꾸고.. 이동하고..
BOOL
cACTOR::operatePatrolAi()
{
	if (m_wBlindTime)		return	TRUE;

	if (m_wSerchEnemyTime	>=	dMONSTER_SIGHT_UPDATE_TIME)
	{
		m_wSerchEnemyTime	=	0;

		if (findEnemy())
		{
			m_wSerchEnemyTime	=	0;

			return	TRUE;
		}
	}

	m_wSerchEnemyTime++;
	m_sWaitTimeForNextMove--;

//	이동.. 어디론가.. -_-
	if (m_sWaitTimeForNextMove	<=	0)
	{
		m_sWaitTimeForNextMove	=	dDEFAULT_WAIT_TIME_FOR_NEXT_MOVE+random(24)*5;

		if (!m_bIsMoving)
		{
			int	iDx	=	random(100)+20;
			int	iDy	=	random(100)+20;

			if (iDx	&1)	iDx=-iDx;
			if (iDy	&1)	iDy=-iDy;

			moveTo(m_pos.x+iDx,m_pos.y+iDy);
		}
	}	//	이동.. 어디론가.. -_-

	return	TRUE;
}

//
//	접근
//	일정 시간마다 위치 변경 했는지 체크하고... 따라간다.
BOOL
cACTOR::operateApproachAi()
{
	if	(m_isAction	||	m_iShakleHP	)	
		return	TRUE;

	if (!m_lpTarget && !m_wIsLocalAttack)
	{
		stopBattleForNPC(TRUE);

		return	TRUE;
	}

	if	(m_wIsLocalAttack)
	{
		if	(isInAttackRange(m_posTarget.x,m_posTarget.y,m_wEngageRange))//_iX,_iY부터 _iRange거리 안에 있냐?
		{
			setAiFight(NULL,&m_engageAbility,m_posTarget.x,m_posTarget.y);

			return TRUE;
		}
	}
	else
	if (isTargetInAttackRange(m_lpTarget,m_wEngageRange))
	{
		setAiFight(m_lpTarget,&m_engageAbility);

		return TRUE;
	}

	m_wApproachTime++;

	if (m_wAnotherTargetBitMeCount	&&	m_wApproachTime	>=	dSYNC_FPS*5)
	{
		stopBattleForNPC(TRUE);

		return	TRUE;
	}

	if (m_bIsMoving)
	{
		if (m_dwLastMoveMessageTime	+	dMOVE_UPDATE_TIME	<=	(DWORD)m_lpField->m_iFrameCounter)
		{
			m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;
			addNpcMoveMessage(TRUE);
		}
	}
	else
	{
		pursuit(NULL);

		if (m_lpTarget && isTamer(m_lpTarget) && isMoveBlockOvercrowding())
			warpToTargetNearPlace(m_lpTarget);
	}

//	주기적으로 타겟의 위치를 체크하고, 이동했다면 위치를 다시 잡는다.
	if	(m_wSerchEnemyTime	>=	dAPPROACH_TARGET_REFRESH_TIME)
	{
		if	(m_lpTarget && isPureMonster() && m_lpTarget->isPlayer())
		{
			if (!m_lpTarget->addHostileEnemy(m_wSerialInField))
				return TRUE;
		}

		m_wSerchEnemyTime	=	0;

		if	(m_wIsLocalAttack == FALSE)
			if (m_lpTarget->m_pos.x	!=	m_posTarget.x	||	m_lpTarget->m_pos.y	!=	m_posTarget.y)	//	이동 했다.
			{
				if (!pursuit(NULL))
				{
					stopBattleForNPC(TRUE);

					return	TRUE;
				}
			}
	}

	m_wSerchEnemyTime++;

	return	TRUE;
}

//
//	전투
BOOL
cACTOR::operateFightAi(BOOL _bIsFirst)
{
	int		iAttackResult;

	if	(!isMonster() && !isCopyActor())
	{
		CLOG("incorrect NPC","NPC가 전투를? %s/%s",m_lpField->m_strName,m_strName);
		setAiReady();

		return FALSE;
	}

	if	(m_isExclusiveAction)
		return	TRUE;	//	행동중 이잖어!!

	if	(_bIsFirst	==	FALSE)
	{
		if	(m_wProcessPatternStatus	==	eCP_use_local_skill	||	m_wProcessPatternStatus	==	eCP_use_skill)
		{
			operateNextStepPattern();
			return	TRUE;
		}

		if	(isPureMonster() || isCopyActor())
			findEngageAbility(m_lpTarget);
		else
		if	(isPet() && m_wUseSpecialAbilityTime)
			findEngageAbility(m_lpTarget);
	}

	CSkill	*lpSkill	=	m_engageAbility.getSkill();
	BOOL	bIsUseQuick	=	FALSE;

	if	(lpSkill==	NULL)
	{
		memcpy(&m_engageAbility,&m_aAbility[0],sizeof(m_engageAbility));

		lpSkill	=	m_engageAbility.getSkill();

		if	(lpSkill	==	NULL)
		{
			m_lpField->addBookedRemoveActor(m_wSerialInField);
			return	FALSE;
		}
	}

	if	(m_wIsLocalAttack == FALSE && m_lpTarget)
	{
		if	(lpSkill && lpSkill->isCastOnCasterSkill()	==	FALSE && !isTargetInAttackRange(m_lpTarget,m_wEngageRange))	//	사정거리 밖이다.
		{
			setAiApproach(m_lpTarget);

			return	TRUE;
		}
	}
	else
	{
		if	(lpSkill->m_bf1IsCastOnDestPosSkill)
		{
			m_wEngageRange	=	1;

			if	(!isInAttackRange(m_posTarget.x,m_posTarget.y,m_wEngageRange))
			{
				setAiApproach(NULL,m_posTarget.x,m_posTarget.y);
				return	TRUE;
			}
		}
		else
		if	(lpSkill->isCastOnCasterSkill()	== FALSE && !isInAttackRange(m_posTarget.x,m_posTarget.y,m_wEngageRange))	//	사정거리 밖이다.
		{
			setAiApproach(NULL,m_posTarget.x,m_posTarget.y);

			return	TRUE;
		}
	}

	if	(m_bIsMoving)	//	이동중이면 정지
	{
		stop();
		addPacketStop();
	}

	if	(m_wIsLocalAttack	==	FALSE	&&	lpSkill->isRequireTargetSkill()	==	FALSE)
	{
		if	(lpSkill->m_wShootRange ==	0	&&	lpSkill->m_wShootRangePerLevel	==	0)
			bIsUseQuick	=	TRUE;
	}

	if	(m_wUseSpecialAbilityStep	==	ePUSS_FIRE)
	{
		m_wUseSpecialAbilityStep	=	ePUSS_NONE;

		cACTOR	*lpTamer			=	getTamer();

		if	(lpTamer && isSummonBeast())
		{
			if	(lpTamer->m_iCP	<	0	&&	lpTamer->m_wLastCommandSummonedBeast	==	m_wSerialInField)
			{
				downGradeSummonedBeast();

				return	TRUE;
			}
		}

		if	(m_wConfuseTime	==	0 && m_lpTarget)
			if (isOwnForce(m_lpTarget) || m_lpTarget->isNpc())
			{
				stopBattleForNPC(TRUE);

				return	TRUE;
			}
		
		if	(m_lpTarget)
			setAiFight(m_lpTarget);
		else
			setAiReady();

		return	TRUE;
	}

	if	(m_lpTarget && isPureMonster() && m_lpTarget->isPlayer())
	{
		if (!m_lpTarget->addHostileEnemy(m_wSerialInField))
			return TRUE;
	}

	if	(m_iShakleHP	==	0	&&	bIsUseQuick	==	FALSE	&&	m_engageAbility.getSkill()->isAgressiveSkill()	&&	isOwnForce(m_lpTarget)	&&	m_wConfuseTime	==	0)
	{
		stop();
		setTarget(NULL);
		setAiReady();
		addPacketStop();

		return	TRUE;
	}

	if	(bIsUseQuick)
		iAttackResult	=	actionQuick(&m_engageAbility);
	else
	if	(m_wIsLocalAttack)
		iAttackResult	=	actionToGround(m_posTarget.x,m_posTarget.y,&m_engageAbility);
	else
		iAttackResult	=	attackToActor(m_lpTarget,&m_engageAbility);

	if	((iAttackResult > eAR_OK && iAttackResult < eAR_IS_INREGULAR_SKILL) || iAttackResult == eAR_NOT_LEARNED_SKILL)
		return TRUE;

	if	(g_bIsWantPrintTriggerCondition == 1)
		printf("[%s] use skill '%s'\n",m_strName,m_engageAbility.getSkill()->m_strName);

	if	(bIsUseQuick)
		m_lpField->sendAttackToGroundResult(iAttackResult,this,m_pos.x,m_pos.y,&m_engageAbility);
	else
	if	(m_wIsLocalAttack)
		m_lpField->sendAttackToGroundResult(iAttackResult,this,m_posTarget.x,m_posTarget.y,&m_engageAbility);
	else
		m_lpField->sendAttackToActorResult(iAttackResult,this,m_lpTarget,&m_engageAbility);	//	C2S

	if (m_wUseSpecialAbilityStep	==	ePUSS_READY)
		m_wUseSpecialAbilityStep	=	ePUSS_FIRE;

//	cACTOR	*lpTamer	=	getTamer();

//	if	(lpTamer)
//		lpTamer->iMiniPet()->updateChargeCount();

	if	(m_lpCurrentTarget)
	{
		setTarget(m_lpCurrentTarget);
		m_lpCurrentTarget			=	NULL;
	}

	return	TRUE;
}
// 
BOOL
cACTOR::operateComeBackToRegenPlaceAi()
{
	cPOINT	posComeBack;

	cAreaInfo	*lpArea	=	m_lpField->m_pArea->getArea(getNpc()->m_wMoveArea);
	
	if (lpArea)
	{
		if (!m_lpField->findEnterancePos(&posComeBack,lpArea))
			return	FALSE;

		if(!moveTo(posComeBack.x , posComeBack.y))
			return FALSE;
	}

	return TRUE;
}

//
//	혼란 상태 처리
BOOL
cACTOR::operateConfuseAi()
{
	if	(isPlayer())
		return	FALSE;

	if	(m_wConfuseTime	==	0)
	{
		stopBattleForNPC(TRUE);

		return	TRUE;
	}

	if	(!m_lpTarget || m_lpTarget->m_wSerialInField	!=	m_wSerialInField)
	{
		revenge(this);

		return	TRUE;
	}

	return	FALSE;
}

//
//	도망
void
cACTOR::flee(cACTOR *_lpAttacker,int _iUpkeepTime)
{
	if	(m_wFleeTime)
		return;

	setAiReady();

	m_wFleeTime			=	max(_iUpkeepTime,m_wFleeTime);

	int		iAngle		=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,_lpAttacker->m_pos.x,_lpAttacker->m_pos.y);
	int		iDistance	=	dDEFAULT_WANDER_RANGE;
	cPOINT	posMove;

	posMove.Set(m_pos.x,m_pos.y);
	g_protractor.getAnglePos(&posMove,(iAngle+180)%360,iDistance);

	if	(!moveTo(posMove.x,posMove.y))
	{
		for (int iTryCount=0;;iTryCount++)
		{
			if	(iTryCount >= 40)
				return;

			posMove.x		=	m_pos.x+random(iDistance)+iDistance/4;
			posMove.y		=	m_pos.y+random(iDistance)+iDistance/4;
				
			if (moveTo(posMove.x,posMove.y))
				break;
		}
	}

	m_wRemainActionTime	=	0;
	setExclusiveAction(FALSE);
	if(isPlayer())
		sendEtcWork(eEW_CHANGE_FLEE,_iUpkeepTime, m_posDest.x, m_posDest.y);
	setAiMove();
}

//
//	타켓 재 설정.
BOOL
cACTOR::reaimTarget(cACTOR *_lpActor)
{
	short	sNonTargetTime		=	0;

	if  (_lpActor)
	{
		sNonTargetTime				=	_lpActor->m_sNonTargetTime;
		_lpActor->m_sNonTargetTime	=	1;
	}

	cACTOR	*lpNextTarget			=	findNeareastEnemy();

	if	(lpNextTarget)
		revenge(lpNextTarget);

	if  (_lpActor)
		_lpActor->m_sNonTargetTime	=	sNonTargetTime;

	return	TRUE;
}

//
//전투중지
void
cACTOR::stopBattleForNPC(BOOL _bIsRequireSendPacket)
{
	stop();
	setAiReady();

	if (_bIsRequireSendPacket)
		addPacketStop();
}

#define	dTRACE_GUIDE_SAFE_RANGE							64
#define	dCAN_NOT_FIND_TRACE_GUIDE_RANGE					400
#define	dTRACE_GUIDE_MOVE_UPDATE_TIME					(dSYNC_FPS/4)
#define	dTRACE_GUIDE_REQUIRE_TRACE_TAMER_SPEED_UP_RANGE	180
#define	dTRACE_GUIDE_AIM_RANGE							32

//
//	위치나 주인에게로 돌아가기
BOOL
cACTOR::operateTraceGuide()
{
	BOOL	bIsRequireWarp	=	FALSE;
	cACTOR	*lpGuide		=	getGuide();

	if	(!lpGuide)
		return	FALSE;

	if	(m_wBlockedPosValue)
		m_wBlockedPosValue--;

	int	iCurrentRange	=	GetRange(lpGuide->m_pos.x,lpGuide->m_pos.y,m_pos.x,m_pos.y);
	int	iSafeRange;

	iSafeRange			=	(dTRACE_GUIDE_SAFE_RANGE+getBodySize()/2+lpGuide->getBodySize()/2)*(dTRACE_GUIDE_SAFE_RANGE+getBodySize()/2+lpGuide->getBodySize()/2);

	if	(iCurrentRange	>=	dCAN_NOT_FIND_TRACE_GUIDE_RANGE*dCAN_NOT_FIND_TRACE_GUIDE_RANGE)	//	시야를 벗어 나면 워프 시킨다.
		bIsRequireWarp	=	TRUE;
	else
	{
		if (m_bIsMoving)	//	이동중이다.
		{
			if (m_dwLastMoveMessageTime	+	dTRACE_GUIDE_MOVE_UPDATE_TIME	<=	(DWORD)m_lpField->m_iFrameCounter)
			{
				m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

				addNpcMoveMessage(TRUE);
			}
		}
		else	//	하여간 목적지까지 왔다.
		{
			if (iCurrentRange	<=	iSafeRange)	//	일정 거리 안으로 다가 갔으면 중지한다.
			{
				setAiReady();
				addPacketStop();

				return	TRUE;
			}

			if	(traceGuide())
				return	TRUE;

			if	(isMoveBlockOvercrowding())
				bIsRequireWarp	=	TRUE;
		}
	}

	if	(m_wSerchEnemyTime	>=	dAPPROACH_TARGET_REFRESH_TIME)	//	일정 주기로 상대가 이동하거나 했는지 확인한다.
	{
		if	(lpGuide->m_bIsMoving)	//	이동 했다.
		{
			if	(traceGuide())
				return	TRUE;
			else
				bIsRequireWarp	=	TRUE;
		}
	}

	if	(bIsRequireWarp)
	{
		warpToTargetNearPlace(lpGuide);

		setAiReady();
		
		return	TRUE;
	}

	m_wSerchEnemyTime++;

	return	TRUE;
}

BOOL
cACTOR::traceGuide()
{
	cACTOR	*lpGuide	=	getGuide();

	if	(!lpGuide)
		return	FALSE;

	cPOINT	pos;
	int		iPPS				=	lpGuide->getWalkSpeed()*9/10;
	int		iAngle				=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,lpGuide->m_pos.x,lpGuide->m_pos.y,FALSE)+360;
	int		iFollowMonsterCount	=	lpGuide->getFollowMonsterCount()+1;
	int		iIndex				=	iFollowMonsterCount-1;

	int		iCurrentRange		=	GetRange(lpGuide->m_pos.x,lpGuide->m_pos.y,m_pos.x,m_pos.y);
	int		iLimitRange			=	(dTRACE_GUIDE_REQUIRE_TRACE_TAMER_SPEED_UP_RANGE+getBodySize()/2+lpGuide->getBodySize()/2)*(dTRACE_GUIDE_REQUIRE_TRACE_TAMER_SPEED_UP_RANGE+getBodySize()/2+lpGuide->getBodySize()/2);

	if	(iCurrentRange	>=	iLimitRange)	//	강제로 조련사를 따라가는 범위
		iPPS	=	iPPS*120/100;

	iAngle	+=	180;
	int	iDA	=	0;

	if	(iFollowMonsterCount%2	==	0)
		iDA		=	(iIndex/2*10)+8;
	else
		iDA		=	(iIndex+1)/2*10;

	if	(iIndex	%2	==	0)
		iAngle	-=	iDA;
	else
		iAngle	+=	iDA;

	iAngle		%=	360;

	int	iTraceRange	=	dTRACE_GUIDE_AIM_RANGE+getBodySize()/2+lpGuide->getBodySize()/2;

	pos.Set(lpGuide->m_pos.x,lpGuide->m_pos.y);
	g_protractor.getAnglePos(&pos,iAngle,iTraceRange,FALSE);

	return	tamedMonsterMoveForTrace(pos.x,pos.y,iPPS);
}
