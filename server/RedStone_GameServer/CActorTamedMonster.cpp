#include "cACTOR.H"
#include "cFIELD.H"
#include "cNPC.H"
#include "debugCode.H"
#include "CGame.H"

#define	dPET_SAFE_RANGE							160
#define	dPET_REQUIRE_TRACE_MOVE_TAMER_RANGE		200
#define	dPET_REQUIRE_FORCING_TRACE_RANGE		500
#define	dPET_CAN_NOT_FIND_TAMER_RANGE			600
#define	dPET_TRACE_AIM_RANGE					120
#define	dPET_REQUIRE_TRACE_TAMER_SPEED_UP_RANGE	230

#define	dPET_REQUIRE_TRACE_STOP_TAMER_RANGE		220
#define	dPET_REQUIRE_TRACE_TAMER_RANGE_FOR_RETURN_ITEM	100
#define	dPET_TRACE_AIM_RANGE_FOR_RETURN_ITEM			80

//
//	업데이트
void
cACTOR::updateTamedMonster()
{
	if (m_wPetReleaseCounter)
	{
		m_wPetReleaseCounter--;

		if (m_wPetReleaseCounter	==	0)
		{
			m_wPetReleaseCounter	=	1;
			immediatelyExit();
		}

		return;
	}

	cACTOR		*lpTamer=	getTamer();

	if (!lpTamer || m_dwTamerNameHashCode != lpTamer->m_dwNameHashCode || !lpTamer->isPlayer())	//	주인이 사라졌다면 사라져 버려.
	{
		immediatelyExit();

		return;
	}

	if(lpTamer && lpTamer->isDeath())		// 조련사 사망. 09.10.01
	{
		return ;
	}

	if	(m_wRidingDogTime)
		return;

	if	(m_iDeathCount)
	{
		m_iDeathCount--;

		if	(m_iDeathCount	<=	0)
		{
			m_iDeathCount	=	1;	//	이걸 설정 안해 놓으면 수치가 0이라서 살아 있는걸로 체크 된다.

			if	(isPet())	//	펫은 1분 지나면 자동 부활
			{
				if	(g_bIsDuelServer)
					m_lpField->addBookedRemoveActor(m_wSerialInField);
				else
				{
					revive();
					m_iHP	=	100;
				}
			}
			else
			{
				lpTamer->removeSummonBeast(m_wPetIndex);

				if	(m_wIsReleaseSummonBeast	==	FALSE)
					lpTamer->deathSummonBeast(m_wPetIndex,m_iLevel);

				m_iDeathCount	=	2;
				m_lpField->addBookedRemoveActor(m_wSerialInField);
			}
		}

		return;
	}

	syncTamedMonsterHP();

	if (updateStatusByFrameForMonster()	)
		return;

	checkTamedMonsterOutOfSight();	//	주어진 위치에서 너무 많이 이탈했나 확인한다.
	operateAi();	//	이동,캐릭터 액션,AI등 같이 처리
}

//	지정한 아군 주위 순찰
//	지정한 장소 방어
void
cACTOR::setAiTamedMonsterTrace()
{
	m_wAiState				=	eAS_TM_TRACE;
	resetBlockedMoveStress();
}

void
cACTOR::checkTamedMonsterOutOfSight()
{
	if	(m_wMarionetteTime	||	m_wFleeTime || m_wAiState	==	eAS_TM_TRACE)
		return;

	cACTOR	*lpTamer	=	getTamer();

	if	(!lpTamer)
		return;

	int	iCurrentRange	=	GetRange(lpTamer->m_pos.x,lpTamer->m_pos.y,m_pos.x,m_pos.y);

	if	(iCurrentRange	>=	dPET_REQUIRE_FORCING_TRACE_RANGE*dPET_REQUIRE_FORCING_TRACE_RANGE)	//	뭘하고 있든지 강제로 조련사를 따라가는 범위
	{
		stop();
		setTarget(NULL);
		setAiTamedMonsterTrace();

		if (m_wIsReturnToTransferItem	==	FALSE)
			m_wObjectItemForCatch	=	0xffff;

		return;
	}

	if	(m_wObjectItemForCatch	!=	0xffff)
	{
		CDroppedItem	*lpItem	=	m_lpField->m_droppedItem.get(m_wObjectItemForCatch);

		if	(!lpItem || lpItem->m_item.m_dwSerial	!=	m_dwObjectItemSerialForCatch)
		{
			m_wObjectItemForCatch		=	0xffff;
			m_dwObjectItemSerialForCatch=	0xffffffff;
			m_wIsReturnToTransferItem	=	FALSE;

			stopBattleForNPC(TRUE);
		}
		else
			m_wIsReturnToTransferItem	=	FALSE;
	}

	if (m_wIsReturnToTransferItem)
	{
		if (m_iGold	<=	0	&&	getItemCount(0xffff)	==	0)
		{
			m_wIsReturnToTransferItem	=	FALSE;

			stopBattleForNPC(TRUE);
		}
		else
		{
			if (GetRange(m_pos.x,m_pos.y,lpTamer->m_pos.x,lpTamer->m_pos.y)	<	dPICK_ABLE_RANGE)
				returnBeastItem();
		}
	}
	else
	{
		if	(m_iGold	>	0	&&	getItemCount(0xffff))
			m_wIsReturnToTransferItem	=	TRUE;
	}

	if (m_wAiState != eAS_FIGHT && m_wAiState != eAS_APPROACH)
		if (m_lpTarget == NULL &&	m_wObjectItemForCatch	==	0xffff)	//	싸우는중이 아니면 일정 범위를 벗어나면 추적을 시작한다.
		{
			int	iTraceRange;

			if (m_wIsReturnToTransferItem)
				iTraceRange		=	(dPET_REQUIRE_TRACE_TAMER_RANGE_FOR_RETURN_ITEM)*(dPET_REQUIRE_TRACE_TAMER_RANGE_FOR_RETURN_ITEM);
			else
			if (lpTamer->m_bIsMoving)
				iTraceRange		=	(dPET_REQUIRE_TRACE_MOVE_TAMER_RANGE+getBodySize()/2+lpTamer->getBodySize()/2)*(dPET_REQUIRE_TRACE_MOVE_TAMER_RANGE+getBodySize()/2+lpTamer->getBodySize()/2);
			else
				iTraceRange		=	(dPET_REQUIRE_TRACE_STOP_TAMER_RANGE+getBodySize()/2+lpTamer->getBodySize()/2)*(dPET_REQUIRE_TRACE_STOP_TAMER_RANGE+getBodySize()/2+lpTamer->getBodySize()/2);

			if (iCurrentRange	>=	iTraceRange)
			{
				setAiTamedMonsterTrace();

				return;
			}
		}
}

//
//	레뒤~상태...
BOOL
cACTOR::operateTamedMonsterReadyAi()
{
	if (m_wFleeTime)
		return	TRUE;

	cACTOR	*lpTamer	=	getTamer();

	if (!lpTamer)
		return	TRUE;

//	아직 이동 업데이트 할 시간이 안 됐다.
	if (m_dwLastUpdateTime + dTAMED_MONSTER_WANDER_CYCLE*dSYNC_FPS > (DWORD)m_lpField->m_iFrameCounter)
		return	TRUE;

	return	TRUE;

	m_dwLastUpdateTime	=	m_lpField->m_iFrameCounter+random(10);

	int	iAngle			=	g_protractor.getAngleToTarget(lpTamer->m_pos.x,lpTamer->m_pos.y,m_pos.x,m_pos.y)+360;
	int	iMoveAngle		=	random(20)+10;

	if (random(2) == 0)
		iMoveAngle		=	-iMoveAngle;

	iAngle				+=	iMoveAngle;
	iAngle				%=	360;

	for (int iTryCount=20;iTryCount;--iTryCount)
	{
		int		iRange	=	random(40)+dPET_SAFE_RANGE*2/3+getBodySize()/2;
		cPOINT	posMove;

		posMove.Set(lpTamer->m_pos.x,lpTamer->m_pos.y);

		g_protractor.getAnglePos(&posMove,iAngle,iRange,FALSE);

		if (moveTo(posMove.x,posMove.y))
		{
			setAiMove();

			break;
		}
	}

	return	TRUE;
}

BOOL
cACTOR::tamedMonsterMoveForTrace(int _iX,int _iY,int _iPPS)
{
	m_posTarget.x	=	_iX;
	m_posTarget.y	=	_iY;

	if	(!moveTo(m_posTarget.x,m_posTarget.y,100,_iPPS))
		return	FALSE;

	m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;
	m_wSerchEnemyTime		=	0;

	addNpcMoveMessage(FALSE);

	return	TRUE;
}

BOOL
cACTOR::traceTamer()
{
	cACTOR	*lpTamer	=	getTamer();

	if	(!lpTamer)
		return	FALSE;

	cPOINT	pos;
	int		iPPS				=	lpTamer->getWalkSpeed()*95/100;
	int		iAngle				=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,lpTamer->m_pos.x,lpTamer->m_pos.y,FALSE)+360;
	int		iFollowMonsterCount	=	lpTamer->getFollowMonsterCount();
	int		iIndex				=	lpTamer->getFollowMonsterIndex(m_wSerialInField);

	if	(iIndex	==	0xffff)
		return	FALSE;

	int		iCurrentRange		=	GetRange(lpTamer->m_pos.x,lpTamer->m_pos.y,m_pos.x,m_pos.y);

	if	(iCurrentRange	>=	(dPET_REQUIRE_TRACE_TAMER_SPEED_UP_RANGE+getBodySize()/2+lpTamer->getBodySize()/2)*(dPET_REQUIRE_TRACE_TAMER_SPEED_UP_RANGE+getBodySize()/2+lpTamer->getBodySize()/2))	//	강제로 조련사를 따라가는 범위
		iPPS	=	iPPS*120/100;

	iAngle	+=	180;
	int	iDA	=	0;

	if (iFollowMonsterCount%2	==	0)
		iDA		=	(iIndex/2*10)+8;
	else
		iDA		=	(iIndex+1)/2*10;

	if (iIndex	%2	==	0)
		iAngle	-=	iDA;
	else
		iAngle	+=	iDA;

	iAngle		%=	360;

	int	iTraceRange	=	dPET_TRACE_AIM_RANGE+getBodySize()/2+lpTamer->getBodySize()/2;

	if (m_wIsReturnToTransferItem)
		iTraceRange	=	dPET_TRACE_AIM_RANGE_FOR_RETURN_ITEM-10;

	pos.Set(lpTamer->m_pos.x,lpTamer->m_pos.y);
	g_protractor.getAnglePos(&pos,iAngle,iTraceRange,FALSE);

	return	tamedMonsterMoveForTrace(pos.x,pos.y,iPPS);
}

//
//	위치나 주인에게로 돌아가기
BOOL
cACTOR::operateTamedMonsterTraceAi()
{
	if	(m_wFleeTime)
		return	TRUE;

	BOOL	bIsRequireWarp	=	FALSE;
	cACTOR	*lpTamer		=	getTamer();

	int	iCurrentRange	=	GetRange(lpTamer->m_pos.x,lpTamer->m_pos.y,m_pos.x,m_pos.y);
	int	iSafeRange;

	if	(m_wIsReturnToTransferItem)
		iSafeRange		=	dPET_TRACE_AIM_RANGE_FOR_RETURN_ITEM*dPET_TRACE_AIM_RANGE_FOR_RETURN_ITEM;
	else
		iSafeRange		=	(dPET_SAFE_RANGE+getBodySize()/2+lpTamer->getBodySize()/2)*(dPET_SAFE_RANGE+getBodySize()/2+lpTamer->getBodySize()/2);

	if	(iCurrentRange	>=	dPET_CAN_NOT_FIND_TAMER_RANGE*dPET_CAN_NOT_FIND_TAMER_RANGE)	//	시야를 벗어 나면 워프 시킨다.
		bIsRequireWarp	=	TRUE;
	else
	{
		if	(m_bIsMoving)	//	이동중이다.
		{
			if	(m_dwLastMoveMessageTime	+	dTAMED_MONSTER_MOVE_UPDATE_TIME	<=	(DWORD)m_lpField->m_iFrameCounter)
			{
				m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

				addNpcMoveMessage(TRUE);
			}
		}
		else	//	하여간 목적지까지 왔다.
		{
			if	(iCurrentRange	<=	iSafeRange)	//	일정 거리 안으로 다가 갔으면 중지한다.
			{
				setAiReady();
				addPacketStop();

				return	TRUE;
			}

			if	(traceTamer())
				return	TRUE;

			if	(isMoveBlockOvercrowding())
				bIsRequireWarp	=	TRUE;
		}
	}

	if	(m_wSerchEnemyTime	>=	dAPPROACH_TARGET_REFRESH_TIME)	//	일정 주기로 상대가 이동하거나 했는지 확인한다.
	{
		if	(lpTamer->m_bIsMoving)	//	이동 중이다.
		{
			if	(traceTamer())
				return	TRUE;
			else
				bIsRequireWarp	=	TRUE;
		}
	}

	if (bIsRequireWarp)
	{
		warpToTargetNearPlace(lpTamer);

		setAiReady();
		
		return	TRUE;
	}

	m_wSerchEnemyTime++;

	return	TRUE;
}

void
cACTOR::commandTamedMonster(int _iCommand,cACTOR *_lpTarget,int _iX,int _iY)
{
	if	(isDeath())
		return;

	cACTOR	*lpTamer	=	getTamer();

	if	(!lpTamer)
		return;

	lpTamer->sendChangeTamerCommand(m_wPetIndex,m_wTamedMonsterCommand,_lpTarget,_iX,_iY);

	if	(!_lpTarget || _lpTarget->m_wSerialInField	==	0xffff || lpTamer->isOwnForce(_lpTarget))
		return;

	lpTamer->m_dwLastCommandTime	=	g_dwCurrentTime;

	switch(_iCommand) 
	{
		case	eTMD_ATTACK_ALL	:
		{
			if (m_wTamedMonsterCommand	==	eTMD_READY)
				m_wTamedMonsterCommand	=	eTMD_ATTACK_ALL;
			else
				m_wTamedMonsterCommand	=	eTMD_READY;
			break;
		}

		case	eTMD_READY		:
			m_wTamedMonsterCommand	=	_iCommand;
			break;

		case	eTMD_ATTACK_ONE	:
			m_wTamedMonsterCommand	=	_iCommand;
			setAiFight(_lpTarget);
			break;

		case	eTMD_PATROL		:
			m_wTamedMonsterCommand	=	_iCommand;
			break;

		case	eTMD_DEFENSE_POSITION:
			m_wTamedMonsterCommand	=	_iCommand;
			break;
	}
}

cACTOR*
cACTOR::getTamer()
{
	cACTOR	*lpTamer	=	m_lpField->getTestedActor(m_wTamer);

	if	(lpTamer)
	{
		if	(m_wPetIndex	==	0xffff)
			return	NULL;

		if	(m_wPetIndex	!=	0xffff	&&	isSummonBeast())
		{
			cACTOR	*lpBeast=	lpTamer->getSummonBeastActor(m_wPetIndex);

			if	(!lpBeast)
				return	NULL;

			if	(lpBeast->m_iZoneSerial	!=	m_iZoneSerial)
				return	NULL;
		}
	}
	
	return	lpTamer;
}

BOOL
cACTOR::isTamer(cACTOR *_lpActor)
{
	if	(_lpActor->m_wSerialInField == m_wTamer && _lpActor->m_dwNameHashCode == m_dwTamerNameHashCode)
		return	TRUE;

	return	FALSE;
}

void
cACTOR::syncTamedMonsterHP(BOOL _bIsNow)
{
	cACTOR	*lpTamer	=	getTamer();

	if (!lpTamer)
		return;

	if (isPet())	//	 펫이라면 계속해서 체력을 싱크 시켜 준다.
	{
		cPetDefine	*lpPet			=	lpTamer->getPet(m_wPetIndex);

		if (lpPet)
			lpPet->m_bf22CurrentHP	=	max(m_iHP/100,1);
	}

	lpTamer->updateSummonBeastInfo(this);
	lpTamer->updatePetInfo(this);

	if (m_wRemainSyncTime==0 || _bIsNow)
	{
		int	iMaxPower	=	getPhysicalAttackPower(TRUE)/100;
		int	iMinPower	=	getPhysicalAttackPower(FALSE)/100;
		int	iDefensePower=	getDefensivePower()/100;
		int	iMaxHP		=	getMaxHP()/100;

		if (m_iLastSyncHP			!=	m_iHP		||	m_iLastSyncMaxHP		!=	iMaxHP		||
			m_wLastMaxAttackPower	!=	iMaxPower	||	m_wLastMinAttackPower	!=	iMinPower	||	m_iDefensePower	!=	iDefensePower)
		{
			m_iLastSyncHP			=	m_iHP;
			m_wLastMaxAttackPower	=	iMaxPower;
			m_wLastMinAttackPower	=	iMinPower;
			m_iDefensePower			=	iDefensePower;
			m_iLastSyncMaxHP		=	iMaxHP;

			lpTamer->sendSyncPetInfo(m_wPetIndex,m_iHP/100,iMaxHP,iMaxPower,iMinPower,iDefensePower,getStrength(),getIntelligence(),isSummonBeast());
		}

		m_wRemainSyncTime	=	dSYNC_PLAYER_STATUS_PERIOD_IN_BATTLE/2;	//	1초에 한번씩
	}
	else	m_wRemainSyncTime--;
}

void
cACTOR::operateTamedMonsterReactionForUnderAttackLord(cACTOR *_lpEnemy)
{
	int	i;

	if	(m_dwLastCommandTime	+	1000*60*5	<	g_dwCurrentTime)
		return;

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		cACTOR	*lpPet	=	getPetActor(i);

		if	(!lpPet)
			continue;

		if	(lpPet->m_lpTarget	==	NULL)
		{
			lpPet->revenge(_lpEnemy);
			continue;
		}
	}
	for (i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		cACTOR	*lpBeast	=	getSummonBeastActor(i);

		if	(!lpBeast || lpBeast->m_wRidingDogTime)
			continue;

		if	(lpBeast->m_lpTarget	==	NULL)
		{
			lpBeast->revenge(_lpEnemy);
			continue;
		}
	}
}

void
cACTOR::setPetUseSpecialAttack(int _iTime)
{
/*	if	(isPet())
	{
		m_wUseSpecialAbilityTime	=	_iTime;
		setMonsterReaction(TRUE);

		setAiFight(m_lpTarget);
		return;
	}
*/
	if	(isPet())
	{
		m_wUseSpecialAbilityStep	=	ePUSS_READY;

		setAiFight(m_lpTarget,&m_specialAbility);

		m_wUseSpecialAbilityTime	=	_iTime;
		setMonsterReaction(TRUE);
		return;
	}

	if	(m_specialAbility.m_wSkill	==	0xffff)
		return;

	m_wUseSpecialAbilityStep	=	ePUSS_READY;

	setAiFight(m_lpTarget,&m_specialAbility);
}

