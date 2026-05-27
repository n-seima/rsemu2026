#include "CHero.H"
#include "CActor.H"
#include "CMessage.H"
#include "cSound.H"
#include "console.H"

//
//	스킬 사용 실패 메시지 처리
void
CHero::operateUseSkillFailedMessage(int _iReason)
{
	switch(_iReason)
	{
		case	eUSR_FAULT_SKILL		:
			g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL);
			break;

		case	eUSR_WEAPON_NOT_FOUND	:
			g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_REQUIRE_WEAPON);
			break;

		case	eUSR_SHIELD_NOT_FOUND	:
			g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_REQUIRE_SHIELD);
			break;

		case	eUSR_BULLET_NOT_FOUND	:
			g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_REQUIRE_BULLET);
			break;

		case	eUSR_PET_NOT_FOUND		:
			g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_REQUIRE_PET);
			break;

		case	eUSR_SUMMON_BEAST_NOT_FOUND	:
			g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_REQUIRE_SUMMON_BEAST);
			break;

		case	eUSR_LOWER_CP			:
			g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_LOWER_CP);
			break;

		case	eUSR_CANO_NOT_CLOSE_ATTACK_ON_HILL	:
			g_lpHero->warning(dMSG_CAN_NOT_CLOSE_ATTACK_ON_THIS_PLACE);
			break;

		case	eUSR_IS_COOL_TIME					:
			g_lpHero->warning(dMSG_AR_IS_COOL_TIME);
			break;

		case	eUSR_REQUIRE_POTION	:
			g_lpHero->warning(dMSG_USR_REQUIRE_POTION);
			break;

		case	eUSR_REQUIRE_DURG	:
			g_lpHero->warning(dMSG_USR_REQUIRE_DURG);
			break;

		case	eUSR_REQUIRE_FLOWER	:
			g_lpHero->warning(dMSG_USR_REQUIRE_FLOWER);
			break;

		case	eUSR_REQUIRE_CANDY	:
			g_lpHero->warning(dMSG_USR_REQUIRE_CANDY);
			break;

		case eUSR_NOT_NEED_STATE:		// 스킬이 사용가능한 상태가 아닙니다.
			g_lpHero->warning(dMSG_USR_NOT_NEED_STATE);
			break;
		case eUSR_BINDING_WORDS:		// 언어구속상태입니다.
			g_lpHero->warning(dMSG_USR_BINDING_WORDS);
	}
}

//
//	락 걸린 타겟을 공격
void
CHero::useSkillToLockedTarget()
{
	CActor		*lpTarget			=	&g_aActor[m_wLockedTarget];
	cAbility	*lpLockedAbility	=	getLockedAbility();
	CSkill		*lpSkill			=	lpLockedAbility->getSkill();
	cAbility	transAbility;	

	if(m_wTransformationTime)
	{
		
		transAbility.m_wSkill = m_wTransformationSkill;
		transAbility.m_wLevel = m_wTransformationLevel;
		lpLockedAbility	=	&transAbility;
	}

	if	(g_lpControlActor)
	{
		if (m_iLastActionTime+dSEND_HERO_ACTION_PERIOD	>	s_iFrameCounter	)
			return;

		if (s_agent.sendUseSkillToTarget(m_wLockedTarget,m_wLockedAbility,m_wLeftAbility,m_wSubAbility))
			m_iLastActionTime	=	s_iFrameCounter;

		return;
	}

	if	(!lpLockedAbility	||	!lpTarget	||	!lpSkill)	
	{
		releaseEngagedTarget();
		return;
	}

	if (g_lpHero->m_isExclusiveAction				)
	{
		if(s_isRightButtonPressed)
		{
			int	iRange		=	getAttackRange(lpLockedAbility);
			int	iMinRange	=	lpLockedAbility->getMinimumAttackRange();
			
			if	(g_lpHero->isTouchAbleTarget(lpTarget,iRange,iMinRange))	//	타겟이 공격거리 안에 있으면
			{
				if	(s_agent.sendUseSkillToTarget(m_wLockedTarget,m_wLockedAbility,m_wLeftAbility,m_wSubAbility))
				{
					m_iLastActionTime	=	s_iFrameCounter;
					engageTarget(m_wLockedTarget);
				}
			}
		}
		return;
	}

	if (!lpTarget->isCastAbleSkill(lpLockedAbility)	)
	{
		g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_TARGET);	//	선택한 스킬을 사용하기에 적절한 대상이 아닙니다.
		releaseEngagedTarget();
		return;
	}

	if (m_iLastActionTime+dSEND_HERO_ACTION_PERIOD	>	s_iFrameCounter	)
		return;

	if (lpLockedAbility->isQuickCastSkill())	//	퀵 캐스트 가능 스킬
	{
		useSkillToGround(0,0,m_wLockedAbility);
		return;
	}

	if	(g_map.isMissileBlockedLine(g_lpHero->m_pos.x,g_lpHero->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
	{
		releaseEngagedTarget();
		return;
	}

	int	iUseAble	=	isUseAbleSkill(m_wLockedAbility);

	if	(lpSkill->m_bf1IsToggleSkill)
		if	(iUseAble	==	eUSR_IS_COOL_TIME)
			iUseAble	=	eUSR_SUCCESS;

	if	(iUseAble	!=	eUSR_SUCCESS)
	{
		if (m_isWalking)
			sendStop();	//	이동 중이면 정지!! -o-

		operateUseSkillFailedMessage(iUseAble);

		releaseEngagedTarget();
//		lockOff();
//		m_wEngageTarget	=	0xffff;

		return;
	}

	int	iRange		=	getAttackRange(lpLockedAbility);
	int	iMinRange	=	lpLockedAbility->getMinimumAttackRange();

	if	(g_lpHero->isTouchAbleTarget(lpTarget,iRange,iMinRange))	//	타겟이 공격거리 안에 있으면
	{
		if	(isEngage()							)
		{
			sendStop();	//	실제 정지보다 캐릭터 현재 좌표 갱신의 의미가 더 크다
			positiveAttack(m_wLockedTarget,m_wLockedAbility);
			return;	//	이미 교전중이면
		}

		if	(m_isWalking)
		{
			dprint("send stop packet!");
			sendStop();	//	이동 중이면 정지!! -o-
		}

		if	(s_agent.sendUseSkillToTarget(m_wLockedTarget,m_wLockedAbility,m_wLeftAbility,m_wSubAbility))
		{
			dprint("send attack packet!");
			m_iLastActionTime	=	s_iFrameCounter;
			engageTarget(m_wLockedTarget);
		}
	}
	else
	{
		releaseEngagedTarget();
		moveTo(lpTarget->m_pos.x,lpTarget->m_pos.y);
	}
}

//
//	공격
BOOL
CHero::useSkillToTarget(int _iTarget,int _iAbility)
{
	if	(!isAvailAbility(_iAbility))
		return	FALSE;

	cAbility	*lpAbility	=	&m_aAbility[_iAbility];
	CActor		*lpTarget	=	&g_aActor[_iTarget];
	CSkill		*lpSkill	=	lpAbility->getSkill();

	if	(!lpSkill)
		return	FALSE;

	if	(g_map.isMissileBlockedLine(g_lpHero->m_pos.x,g_lpHero->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
	{
		g_lpHero->warning(dMSG_BLOCKED_OBJECT_EXIST);	//	선택한 스킬을 사용하기에 적절한 대상이 아닙니다.

		return	FALSE;
	}

	if	(lpSkill->m_bf1IsToggleSkill	== FALSE && !lpTarget->isCastAbleSkill(lpAbility))
	{
		if	(lpSkill->isQuickCastSkill())
			return	FALSE;
		if	(lpSkill->isCastOnGroundSkill())
			return	FALSE;
		
		g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_TARGET);	//	선택한 스킬을 사용하기에 적절한 대상이 아닙니다.

		return	FALSE;
	}

	if	(m_iLastActionTime+dSEND_HERO_ACTION_PERIOD	>	s_iFrameCounter	)
		return	FALSE;

	int	iUseAble	=	isUseAbleSkill(_iAbility);

	if	(lpSkill->m_bf1IsToggleSkill)
		if	(iUseAble	==	eUSR_IS_COOL_TIME)
			iUseAble	=	eUSR_SUCCESS;

	if	(iUseAble	!=	eUSR_SUCCESS			)
		return	FALSE;

	m_iLastActionTime	=	s_iFrameCounter;

	cItem	*lpWeapon	=	getWeapon();
	int		iAttackRange=	getAttackRange(lpAbility) + g_lpHero->getBodySize();	//	공격범위 체크
	int		iMinRange	=	lpAbility->getMinimumAttackRange();	//	공격범위 체크

	if	(!lpAbility->isQuickCastSkill())
		if	(!g_lpHero->isTouchAbleTarget(lpTarget,iAttackRange,iMinRange))
		{
			g_lpHero->warning(dMSG_OUT_OF_SKILL_RANGE_TARGET);

			return	FALSE;	//	공격거리 밖이다!!
		}

	if	(m_isWalking)
		sendStop();	//	이동 중이면 정지!! -o-

	if	(lpAbility->isAgressiveSkill())
		if	(m_wEngageTarget	==	0xffff)	
			engageTarget(_iTarget);	//	교전중인 상대가 없으면 이녀석을 교전 상대로 정한다.

	dprint("send sendUseSkillToTarget");

	return	s_agent.sendUseSkillToTarget(_iTarget,_iAbility,m_wLeftAbility,m_wSubAbility);//_lpAbility,&m_abilityLeft);
}

//
//	락이 걸린 대상에게 액숀!!
void
CHero::actionToLockedTarget(int _iActionToLockedTargetCount)
{
	if	(g_lpHero->isDeath())
		return;

	CActor		*lpTarget	=	&g_aActor[m_wLockedTarget];
	cAbility	*lpAbility	=	getLockedAbility();

	if (!lpTarget							)	
	{
		lockOff();
		return;
	}

	BOOL	bIsAttackShakle	=	FALSE;

	if	(lpTarget->isHero()	&&	lpTarget->m_wShakleSkill	!=	0xffff)
		bIsAttackShakle	=	TRUE;

	if	(lpTarget->m_bf1IsFreezeShakle)
		bIsAttackShakle	=	TRUE;

	if	(!lpTarget->isNpc())
	{
		if	(!lpTarget->isCastAbleSkill(lpAbility,_iActionToLockedTargetCount))
			return;

		if	(!lpAbility->isQuickCastSkill())	//	퀵 캐스트 가능 스킬
		{
			if (g_map.isMissileBlockedLine(g_lpHero->m_pos.x,g_lpHero->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			{
				moveTo(lpTarget->m_pos.x,lpTarget->m_pos.y);
				g_lpHero->warning(dMSG_BLOCKED_OBJECT_EXIST);	//	선택한 스킬을 사용하기에 적절한 대상이 아닙니다.

				return;
			}
		}

		if	(lpAbility->isQuickCastSkill())	//	퀵 캐스트 가능 스킬
			useSkillToGround(0,0,m_wLockedAbility);

		useSkillToLockedTarget();
		return;
	}

	if	(g_lpHero->m_isExclusiveAction		)
		return;

	int	iRange	=	dEVENT_RANGE_WITH_NPC;

	if	(lpTarget->isNpc() && lpTarget->m_bf1IsTalker && g_map.isRoom())
		if	(g_map.isMissileBlockedLine(g_lpHero->m_pos.x,g_lpHero->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			iRange	=	dEVENT_RANGE_WITH_MERCHANT;

	if	(g_lpHero->isTouchAbleTarget(lpTarget,iRange))	//	타겟이 공격거리 안에 있으면
	{
		if	(m_isWalking)
			sendStop();	//	이동 중이면 정지!! -o-

		if	(m_iLastActionTime+dSEND_HERO_ACTION_PERIOD	>	s_iFrameCounter	)
			return;

		if	(s_agent.sendEventToTarget(m_wLockedTarget))
			m_iLastActionTime	=	s_iFrameCounter;
	}
	else
		moveTo(lpTarget->m_pos.x,lpTarget->m_pos.y);
}

//
//	공격
BOOL
CHero::useSkillToGround(int _iX,int _iY,int _iAbility)
{
	if	(!isAvailAbility(_iAbility))
		return	FALSE;

	if	(g_lpHero->isDeath())
		return	FALSE;

	cAbility	*lpAbility	=	&m_aAbility[_iAbility];
	CSkill		*lpSkill	=	lpAbility->getSkill();

	if	(!lpAbility	||	!lpSkill)
		return FALSE;

	if	(!lpAbility->isQuickCastSkill())	//	퀵 캐스트 가능 스킬
		if	(g_map.isMissileBlockedLine(g_lpHero->m_pos.x,g_lpHero->m_pos.y,_iX,_iY))
		{
			g_lpHero->warning(dMSG_BLOCKED_OBJECT_EXIST);	//	선택한 스킬을 사용하기에 적절한 대상이 아닙니다.

			return	FALSE;
		}

	if	(m_iLastActionTime+dSEND_HERO_ACTION_PERIOD	>	s_iFrameCounter	)
		return	FALSE;

	int	iUseAble		=	isUseAbleSkill(_iAbility);

	if	(lpSkill->m_bf1IsToggleSkill)
		if	(iUseAble	==	eUSR_IS_COOL_TIME)
			iUseAble	=	eUSR_SUCCESS;

	if	(iUseAble	!=	eUSR_SUCCESS			)
	{
		operateUseSkillFailedMessage(iUseAble);
		return	FALSE;
	}

	m_iLastActionTime	=	s_iFrameCounter;

	if	(lpAbility->isQuickCastSkill() || lpAbility->getSkill()->m_bf1IsToggleSkill)	//	퀵 캐스트 가능 스킬
	{
		if (m_isWalking	)
			sendStop(FALSE);	//	이동 중이면 정지!! -o-

		return	s_agent.sendQuickAction(_iAbility);
	}

	if	(lpAbility->isCastGroundSkill())//	필드 공격 가능 스킬
	{
		cItem	*lpWeapon	=	getWeapon();
		int		iAttackRange=	getAttackRange(lpAbility) + g_lpHero->getBodySize();	//	공격범위 체크

		iAttackRange		*=	iAttackRange;

		if	(GetOvalRange(g_lpHero->m_pos.x,g_lpHero->m_pos.y,_iX,_iY)	<=	iAttackRange)
		{
			if (m_isWalking)
				sendStop();	//	이동 중이면 정지!! -o-

			return s_agent.sendUseSkillToGround(_iX,_iY,_iAbility);
		}

		g_lpHero->warning(dMSG_THERE_IS_OUT_OF_ATTACK_RANGE);

		return	FALSE;
	}

	g_lpHero->warning(dMSG_IS_ACTION_TO_TARGET_SKILL);

	return FALSE;
}

BOOL
CHero::isUseSkillState(CSkill* _lpSkill)
{

	if(m_wIgnoreNeedStateTime)
		return TRUE;

	switch(_lpSkill->m_bf6NeedState)
	{
	case dNEED_STATE_EMPTY:
		return TRUE;

	case eNEED_STATE_CONSENSUS :

		if(m_wConsensusTime)
			return TRUE;
		break;
	case eNEED_STATE_FIGHTING_SPIRIT :

		if(m_wFightingSpiritTime)
			return TRUE;
		break;
	}

	return FALSE;
}

//
//	퀵 스킬 사용
BOOL
CHero::useQuickSlotSkill(int _iSlot)
{
	// insu add
	if(s_iPopupInterface == ePIW_CARROT_SHOP)
		return TRUE;
	// insu add end

	if	(g_lpHero->isDeath())	
		return	FALSE;

	int			iAbility	=	m_aQuickSkill[m_wCurrentQuickSkillSlot][_iSlot][(m_wJob)%2];
	// 10.01.07
  	// 스킬슬롯에 있는 아이템 사용
	if( iAbility >= dEQUIPMENT_SKILL_SLOT && iAbility <= dEQUIPMENT_SKILL_SLOT * 2)
	{
		s_iFocusItem = iAbility - dEQUIPMENT_SKILL_SLOT;
		BOOL bIsOk = 	setEquipItem(s_iFocusItem, _iSlot, TRUE);

		if(bIsOk)
			g_lpHero->stop(TRUE);

		s_iFocusItem = 0xffff;
		return bIsOk;
	}

	if(g_lpHero->isSitdown())
		return FALSE;

	if(g_lpHero->isTransformationMonster())
		return FALSE;

	cAbility	*lpAbility	=	getAbility(iAbility);

	if	(!isAvailAbility(iAbility))	
		return	FALSE;

	CSkill		*lpSkill	=	lpAbility->getSkill();

	if	(!lpSkill)
		return	FALSE;

	if (lpSkill->isPassiveSkill())
	{
		int	iAbility	=	getAbilityIndexBySkill(lpAbility->m_wSkill);
		
		if (iAbility	==	0xffff)	
			return	FALSE;

		if (g_hero.isActiveReaction(iAbility))
		{
			s_agent.sendSetReactionStatus(iAbility,FALSE);
			return	TRUE;
		}
		else
		{
			s_agent.sendSetReactionStatus(iAbility,TRUE);
			return	TRUE;
		}
	}

	if	(lpSkill->m_bf1IsOnOffSkill)
	{
		switch(lpSkill->m_wType)
		{
			case	eSKILL_TYPE_COMPLETE_PROTECTION	:
				if (isActiveCompleteProtection())	
				{
					useSkillToTarget(g_lpHero->m_wSerial,iAbility);
					return	TRUE;
				}
				break;

			case	eSKILL_TYPE_SHIMMERING_SHIELD	:
				if (g_lpHero->m_wIsCastBlocker)
				{
					useSkillToTarget(g_lpHero->m_wSerial,iAbility);
					return	TRUE;
				}
				break;
		}
	}

	int	iUseAble	=	isUseAbleSkill(iAbility);

	if	(lpSkill->m_bf1IsToggleSkill)
		if	(iUseAble	==	eUSR_IS_COOL_TIME)
			iUseAble	=	eUSR_SUCCESS;

	if	(iUseAble	!=	eUSR_SUCCESS)
	{
		operateUseSkillFailedMessage(iUseAble);
		return	FALSE;
	}

	int	iFocusedPartyMember	=	CGamePlay::s_partyInfo.getFocusMember();

	if	(lpSkill->isQuickCastSkill())
		return	useSkillToGround(s_posCursor.x,s_posCursor.y,iAbility);

	if	(iFocusedPartyMember	!=	0xffff)
	{
		if	(useSkillToTarget(iFocusedPartyMember,iAbility)				)
			return	TRUE;

		if(!lpSkill->isQuickCastSkill() && !lpSkill->isCastOnGroundSkill())	
			return	FALSE;
	}

	if	(s_iFocusActor	!=	0xffff)
	{
		if	(useSkillToTarget(s_iFocusActor,iAbility))
			return	TRUE;

		if	(!lpSkill->isQuickCastSkill() && !lpSkill->isCastOnGroundSkill())	
			return	FALSE;
	}
	else
	if	 (m_wEngageTarget		!=	0xffff	)
	{
		if (lpSkill->isCastOnEnemySkill() && !lpSkill->isCastOnGroundSkill())
			return	useSkillToTarget(m_wEngageTarget,iAbility);
	}

	return	useSkillToGround(s_posCursor.x,s_posCursor.y,iAbility);
}