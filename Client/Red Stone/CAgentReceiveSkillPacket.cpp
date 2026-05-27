#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "CSkill.H"
#include "cMESSAGE.H"
#include "CSound.H"
#include "CEvent.H"
#include "CGuardianPost.H"
#include "CWindowInterface.H"

//
//	러쉬!!
void
cPACKET_HANDLER::ReceiveRush(SERVER_PACKETS *_lpPacket)
{
	SG_RUSH		*lpPacket	=	&_lpPacket->mSG_RUSH;
	CActor		*lpActor	=	g_am.getTestedActor(lpPacket->wActor);
	cAbility	ability;

	if	(!lpActor)
		return;

	ability.set(lpPacket->wSkill,lpPacket->wLevel);

	if	(!ability.isEnableJob(lpActor->m_wJob))
	{
		lpActor->addToObscurityActorList();
		return;
	}

	lpActor->rush(lpPacket->wArrivePosX,lpPacket->wArrivePosY,&ability,lpPacket->wResult,lpPacket->wFrame);
}

//
//	리액션 스킬 발동!!
void
cPACKET_HANDLER::ReceiveActiveReactionSkill(SERVER_PACKETS *_lpPacket)
{
	SG_ACTIVE_REACTION_SKILL	*lpPacket	=	&_lpPacket->mSG_ACTIVE_REACTION_SKILL;
	CActor		*lpCaster	=	g_am.getTestedActor(lpPacket->wCaster);
	CActor		*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if (!lpCaster || !lpTarget)	return;

	if (lpCaster->isHero())	g_hero.m_iCP	=	lpPacket->sRemainCP*100;

	lpTarget->addSkillHitEffect(lpPacket->wSkill);
}


void
cPACKET_HANDLER::ReceiveActionToGroupStickedBit(SERVER_PACKETS *_lpPacket)		
{
	SG_ACTION_TO_GROUP_STICKED_BIT	*lpPacket	=	(SG_ACTION_TO_GROUP_STICKED_BIT	*)_lpPacket;
	CActor				*lpAttacker				=	g_am.getTestedActor(lpPacket->bf11Serial);
	CActor				*lpTarget				=	g_am.getTestedActor(lpPacket->aData[0].wActor);
	cAbility			ability;
	
	CSkill	*lpSkill	=	&g_aSkill[lpPacket->bf11Skill];


	if (!lpAttacker || !lpTarget)
		return;
	if (!g_am.checkDeathStatus(lpAttacker))
		return;

	ability.set(lpPacket->bf11Skill,lpPacket->bf7SkillLevel);

	if (!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	if (lpAttacker->isHero())
	{
		if (!ability.getSkill()->m_wIsNotSyncCP)
			g_hero.reduceCP(ability.getSpentCP());	//	CP 감소
	}
	else
		lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);

	lpAttacker->m_attackInfo.reset();
	lpAttacker->m_attackInfo.m_wTarget	=	lpTarget->m_wSerial;

	WORD	awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];
	for (int i=0;i<lpPacket->bf4TargetCount;i++)
	{
		cContinuousDamageInfo	*lpInfo		=	&lpPacket->aData[i];
		CHitInfo				*lpHitInfo	=	&lpAttacker->m_attackInfo.m_aInfo[i];
		CContinuousHitInfo		continuousHitInfo;
		continuousHitInfo.reset();
		continuousHitInfo.m_bf6ContinuousShotCount	=	lpInfo->m_bf6ContinuousShotCount;
		continuousHitInfo.m_bf6CriticalCount		=	lpInfo->m_bf6CriticalCount;
		continuousHitInfo.m_bf6DoubleCriticalCount	=	lpInfo->m_bf6DoubleCriticalCount;
		continuousHitInfo.m_bf6HardBlowCount		=	lpInfo->m_bf6HardBlowCount;
		continuousHitInfo.m_bf6MissCount			=	lpInfo->m_bf6MissCount;

		awTargetList[i]	=	lpInfo->wActor;
		lpHitInfo->set(lpInfo->iPhysicalDamage *100,lpInfo->iMagicDamage *100,lpInfo->dwResultField,&continuousHitInfo );

		if	(lpHitInfo->isDodge())
			lpHitInfo->set(lpInfo->iPhysicalDamage,lpInfo->iMagicDamage,lpInfo->dwResultField,&continuousHitInfo);
	}


	lpAttacker->stop();		//	정지!!

	lpAttacker->m_wUseSkill		=	g_sm.castAtGroupStickedBit(lpAttacker,lpTarget,&ability,lpPacket->wRange,awTargetList,lpPacket->bf4TargetCount);

	lpAttacker->m_abilityUse.copy(&ability);

	if (lpAttacker->isHero())
		g_hero.decreaseBullet(&ability);
	else
	{
		if (lpAttacker->m_wUseSkill	==	0xffff)	
			g_am.addObscurityActor(lpAttacker->m_wSerial);
	}


	lpAttacker->m_iFPS		=	lpPacket->bf6FPS;

	if	(lpAttacker->isHero())
	{
		if	(g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			g_hero.stopBattle();
	}

}
void
cPACKET_HANDLER::ReceiveActionToGroup(SERVER_PACKETS *_lpPacket)		
{
	SG_ACTION_TO_GROUP	*lpPacket	=	(SG_ACTION_TO_GROUP	*)_lpPacket;
	CActor				*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Serial);
	CActor				*lpTarget	=	g_am.getTestedActor(lpPacket->aData[0].wActor);
	cAbility			ability;
	
	CSkill	*lpSkill	=	&g_aSkill[lpPacket->bf11Skill];


	if (!lpAttacker || !lpTarget)
		return;
	if (!g_am.checkDeathStatus(lpAttacker))
		return;

	ability.set(lpPacket->bf11Skill,lpPacket->bf7SkillLevel);

	if (!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	if (lpAttacker->isHero())
	{
		if (!ability.getSkill()->m_wIsNotSyncCP)
			g_hero.reduceCP(ability.getSpentCP());	//	CP 감소
	}
	else
		lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);

	lpAttacker->m_attackInfo.reset();
	lpAttacker->m_attackInfo.m_wTarget	=	lpTarget->m_wSerial;

	WORD	awTargetList[dMAX_CONTINUOUS_HIT_INFO_COUNT];

	for (int i=0;i<lpPacket->bf4TargetCount;i++)
	{
		cSimpleDamageInfo	*lpInfo		=	&lpPacket->aData[i];
		CHitInfo			*lpHitInfo	=	&lpAttacker->m_attackInfo.m_aInfo[i];
		
		g_am.getTestedActor(lpInfo->wActor);

		awTargetList[i]	=	lpInfo->wActor;
		lpHitInfo->set(lpInfo->iPhysicalDamage *100,lpInfo->iMagicDamage *100,lpInfo->dwResultField);

		if	(lpHitInfo->isDodge())
			lpHitInfo->set(lpInfo->iPhysicalDamage,lpInfo->iMagicDamage,lpInfo->dwResultField);
	}

	lpAttacker->attackToGroup(lpTarget,&ability,lpPacket->wRange,awTargetList,lpPacket->bf4TargetCount,lpPacket->bf7ContinuousShotCount,
		lpPacket->bf7CriticalCount,lpPacket->bf7MissCount,lpPacket->bf7DoubleCriticalCount, lpPacket->bf7HardBlowCount);

	lpAttacker->m_iFPS		=	lpPacket->bf6FPS;

	if	(lpAttacker->isHero())
	{
		if	(g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			g_hero.stopBattle();

		g_hero.m_wIsReadyToFireChargedMiniPetSkill	=	lpPacket->bf1MiniPetCharge1;
		g_hero.m_wIsReadyToFireChargedMiniPetSkill2	=	lpPacket->bf1MiniPetCharge2;

	}

}

//
//	캐릭터에게 액션!!
void
cPACKET_HANDLER::ActionToActor(SERVER_PACKETS *_lpPacket)		
{
	SG_ACTION_TO_ACTOR	*lpPacket	=	&_lpPacket->mSG_ACTION_TO_ACTOR;
	CActor				*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Serial);
	CActor				*lpTarget	=	g_am.getTestedActor(lpPacket->bf11TargetSerial);
	cAbility			ability;

	if	(!lpTarget)
		return;
	if	(!g_am.checkDeathStatus(lpAttacker))
		return;
	
		
	ability.set(lpPacket->bf11Skill,lpPacket->bf7Level);

	if	(!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	if	(lpAttacker->isHero())
	{
		if	(!ability.getSkill()->m_wIsNotSyncCP)
			g_hero.reduceCP(ability.getSpentCP());	//	CP 감소
	}
	else
		lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);

	lpAttacker->m_attackInfo.reset();
	lpAttacker->m_attackInfo.m_wTarget	=	lpTarget->m_wSerial;
	lpAttacker->m_attackInfo.m_aInfo[0].set(lpPacket->iPhysicalDamage *100,lpPacket->iMagicDamage*100,lpPacket->dwResultField);

	if	(lpAttacker->m_attackInfo.m_aInfo[0].isDodge())
		lpAttacker->m_attackInfo.m_aInfo[0].set(lpPacket->iPhysicalDamage ,lpPacket->iMagicDamage ,lpPacket->dwResultField);

	lpAttacker->attackToActor(lpTarget,&ability,lpPacket->wRange);

	lpAttacker->m_iFPS		=	lpPacket->bf5FPS;

	if	(lpPacket->bf1FPS)
		lpAttacker->m_iFPS	+=	0x20;

	if	(lpAttacker->isHero())
		if	(g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			g_hero.stopBattle();
}


//
//	땅에다가 액션!!

void
cPACKET_HANDLER::ActionToGround(SERVER_PACKETS *_lpPacket)
{
	cAbility			ability;
	SG_ACTION_TO_GROUND	*lpPacket	=	&_lpPacket->mSG_ACTION_TO_GROUND;
	CActor				*lpAttacker	=	g_am.getTestedActor(lpPacket->wSerial);

	if (!g_am.checkDeathStatus(lpAttacker))
		return;

	ability.set(lpPacket->bf12Skill,lpPacket->wLevel);

	if (!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	if (lpPacket->wSerial		==	g_hero.m_iSerial)
	{
		if (!ability.getSkill()->m_wIsNotSyncCP)
			g_hero.reduceCP(ability.getSpentCP());	//	CP 감소
	}
	else
		lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);

	lpAttacker->m_attackInfo.reset();

	lpAttacker->attackToGround(lpPacket->wDestPosX,lpPacket->wDestPosY,&ability,lpPacket->wRange);
	lpAttacker->m_iFPS			=	lpPacket->wFPS;

	if (lpAttacker->isHero())
	{
		if (g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpPacket->wDestPosX,lpPacket->wDestPosY))
			g_hero.stopBattle();

		g_hero.m_wIsReadyToFireChargedMiniPetSkill	=	lpPacket->bf1MiniPetCharge1;
		g_hero.m_wIsReadyToFireChargedMiniPetSkill2	=	lpPacket->bf1MiniPetCharge2;
	}
}


void
cPACKET_HANDLER::ReceiveAttackToObject(SERVER_PACKETS *_lpPacket)
{
	SG_ATTACK_TO_OBJECT	*lpPacket	=	(SG_ATTACK_TO_OBJECT	*)_lpPacket;
	CActor				*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Attacker);

	if (!g_am.checkDeathStatus(lpAttacker))	
		return;

	lpAttacker->m_attackInfo.m_ability.set(lpPacket->wSkill,1);

	if (!lpAttacker->isTransformationMonster() && !lpAttacker->m_attackInfo.m_ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	lpAttacker->m_attackInfo.reset();
	lpAttacker->m_attackInfo.m_wTarget	=	lpPacket->wObjectIndex;
	lpAttacker->m_attackInfo.m_aInfo[0].set(lpPacket->wPhysicalDamage,lpPacket->wMagicDamage,lpPacket->bf8RemainDurability);

	if (lpAttacker->isHero())
	{
		g_hero.m_bf1IsWaitArcaWorkResult=	FALSE;

		if (!lpAttacker->m_attackInfo.m_ability.getSkill()->m_wIsNotSyncCP)	
			g_hero.reduceCP(lpAttacker->m_attackInfo.m_ability.getSpentCP());	//	CP 감소
	}
	else
		lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);

	lpAttacker->attackToObject(lpPacket->wObjectIndex,lpPacket->bf4ObjectType,&lpAttacker->m_attackInfo.m_ability);
	lpAttacker->m_iFPS	=	lpPacket->bf6FPS;
}

//
//	근접공격

void
cPACKET_HANDLER::BasicAttack(SERVER_PACKETS *_lpPacket)
{
	SG_BASIC_ATTACK	*lpPacket	=	&_lpPacket->mSG_BASIC_ATTACK;
	CActor			*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Serial);
	CActor			*lpTarget	=	g_am.getTestedActor(lpPacket->bf11TargetSerial);

	if (!lpTarget)	
		return;
	if (!g_am.checkDeathStatus(lpAttacker))	
		return;

	int	iSkill	=	lpPacket->bf10Skill;

	if	(lpPacket->bf1Skill)
		iSkill	+=	0x400;

	lpAttacker->m_attackInfo.m_wTarget	=	lpTarget->m_wSerial;
	lpAttacker->m_attackInfo.m_ability.set(iSkill,lpPacket->bf7Level);


	if (!lpAttacker->isTransformationMonster() && !lpAttacker->m_attackInfo.m_ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	lpAttacker->m_attackInfo.reset();
	
	lpAttacker->m_attackInfo.m_wTarget	=	lpTarget->m_wSerial;
	lpAttacker->m_attackInfo.m_aInfo[0].set(lpPacket->iPhysicalDamage*100,lpPacket->iMagicDamage*100,lpPacket->dwResultField);

	if	(lpAttacker->m_attackInfo.m_aInfo[0].isDodge())
		lpAttacker->m_attackInfo.m_aInfo[0].set(lpPacket->iPhysicalDamage,lpPacket->iMagicDamage,lpPacket->dwResultField);

	if (lpAttacker->isHero())
	{
		if (!lpAttacker->m_attackInfo.m_ability.getSkill()->m_wIsNotSyncCP)	
			g_hero.reduceCP(lpAttacker->m_attackInfo.m_ability.getSpentCP());	//	CP 감소
	}
	else
		lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);

	lpAttacker->attackToActor(lpTarget,&lpAttacker->m_attackInfo.m_ability,0xffff);
	lpAttacker->m_iFPS	=	lpPacket->bf6FPS;

	if	(lpAttacker->isHero())
	{
		if	(g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			g_hero.stopBattle();

		g_hero.m_wIsReadyToFireChargedMiniPetSkill	=	lpPacket->bf1MiniPetCharge1;
		g_hero.m_wIsReadyToFireChargedMiniPetSkill2	=	lpPacket->bf1MiniPetCharge2;
	}
}
//
//연속타격을 입히는 공격
void
cPACKET_HANDLER::ContinuousHitAttack(SERVER_PACKETS *_lpPacket)
{
	SG_CONTINUOUS_HIT_BY_CHARACTER	*lpPacket	=	&_lpPacket->mSG_CONTINUOUS_HIT_BY_CHARACTER;
	CActor			*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Serial);
	CActor			*lpTarget	=	g_am.getTestedActor(lpPacket->bf11TargetSerial);

	if	(!lpTarget)
		return;
	if	(!g_am.checkDeathStatus(lpAttacker))
		return;

	lpAttacker->m_attackInfo.m_wTarget			=	lpTarget->m_wSerial;
	lpAttacker->m_attackInfo.m_ability.set(lpPacket->bf11Skill,lpPacket->bf7Level);

	if	(!lpAttacker->isTransformationMonster() && !lpAttacker->m_attackInfo.m_ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	lpAttacker->setContinuousAttackDamage(lpPacket->iPhysicalDamage*100,lpPacket->iMagicDamage*100,lpPacket->bf5AttackCount,lpPacket->bf4MissCount,lpPacket->bf4CriticalCount,lpPacket->bf4DoubleCriticalCount,lpPacket->bf1IsAttackToShakle,
											lpPacket->wMinPhysicalDamage*100,lpPacket->wMinMagicDamage*100,lpPacket->bf4BlockingCount,lpPacket->bf4DancingBlockCount,lpPacket->bf4NoActionBlockingCount,NULL, lpPacket->bf4HardBlowCount);

	if	(lpAttacker->isHero())
	{
		if	(!lpAttacker->m_attackInfo.m_ability.getSkill()->m_wIsNotSyncCP)
			g_hero.reduceCP(lpAttacker->m_attackInfo.m_ability.getSpentCP());	//	CP 감소
	}
	else
		lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);

	lpAttacker->attackToActor(lpTarget,&lpAttacker->m_attackInfo.m_ability,0xffff);

	lpAttacker->m_iFPS							=	lpPacket->bf6FPS;

	if	(lpAttacker->isHero())
		if	(g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			g_hero.stopBattle();
}


void
cPACKET_HANDLER::ReceiveHitChanedDamage(SERVER_PACKETS *_lpPacket)
{
	SG_HIT_CHANED_DAMAGE	*lpPacket	=	(SG_HIT_CHANED_DAMAGE	*)_lpPacket;
	CActor					*lpTarget	=	g_am.getTestedActor(lpPacket->bf11TargetSerial);

	if	(!lpTarget)	
		return;

	int		iSkill					=	lpPacket->bf11Skill;
	int		iShotCount				=	lpPacket->bf5AttackCount;	//	공격 횟수
	int		iMissCount				=	lpPacket->bf5MissCount;	//	미스 횟수
	int		iCriticalCount			=	lpPacket->bf5CriticalCount;//	크리티컬 수
	int		iDoubleCriticalCount	=	lpPacket->bf5DoubleCriticalCount;	//	더블 크리티컬 수
	int		iDancingBlockCount		=	lpPacket->bf5DancingBlockCount;
	int		iNoActionBlockCount		=	lpPacket->bf4NoActionBlockingCount;
	BOOL	bIsAttackToShakle		=	lpPacket->bf1IsAttackToShakle;
	int		iBlockingCount			=	lpPacket->bf5BlockingCount;
	int		iGap					=	lpPacket->bf5ShotGap;
	int		iInstanceKillCount		=	lpPacket->bf1IsInstanceKill;

	int		iPhysicalDamage			=	lpPacket->iPhysicalDamage*100;
	int		iMagicDamage			=	lpPacket->iMagicDamage*100;	// 대미지
	int		iMinPhysicalDamage		=	lpPacket->wMinPhysicalDamage*100;
	int		iMinMagicDamage			=	lpPacket->wMinMagicDamage*100;
	int		iHardBlowCount			=	lpPacket->bf5HardBlowCount;

	lpTarget->hitChainedDamage(iSkill,iShotCount,iMissCount,iGap,bIsAttackToShakle,
							iPhysicalDamage,iMagicDamage,iMinPhysicalDamage,iMinMagicDamage,
							iCriticalCount,iDoubleCriticalCount,iInstanceKillCount,
							iBlockingCount,iNoActionBlockCount,iDancingBlockCount,lpPacket->wCaster, iHardBlowCount);
}

//
//연속타격을 입히는 공격

void
cPACKET_HANDLER::ReceiveContinuousHitAttackByValue(SERVER_PACKETS *_lpPacket)
{
	SG_CONTINUOUS_HIT_BY_VALUE	*lpPacket	=	&_lpPacket->mSG_CONTINUOUS_HIT_BY_VALUE;

	CActor			*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Serial);
	CActor			*lpTarget	=	g_am.getTestedActor(lpPacket->bf11TargetSerial);
	cAbility		ability;

	ability.set(lpPacket->bf11Skill,lpPacket->bf7Level);
	CSkill			*lpSkill	=	ability.getSkill();
	
	if (lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_HIT_VALUE	||
		lpSkill->m_wType	==	eSKILL_TYPE_CONTINUOUS_SHOOT		||
		lpSkill->m_wType	==	eSKILL_TYPE_BIT_GLIDER				)
	{
		if (!lpTarget)	
			return;
	}

	if (!g_am.checkDeathStatus(lpAttacker))	
		return;

	if (!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	lpAttacker->attackToActorByContinuousAttack(lpPacket);

	if (lpAttacker->isHero())
		if (g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			g_hero.stopBattle();
}

//
//	랜드 마커 공격
void
cPACKET_HANDLER::ReceiveLandMarkerAttack(SERVER_PACKETS *_lpPacket)
{
	SG_LAND_MARKER	*lpPacket	=	&_lpPacket->mSG_LAND_MARKER;
	CActor			*lpAttacker	=	g_am.getTestedActor(lpPacket->wSerial);

	if (!g_am.checkDeathStatus(lpAttacker))	return;

	lpAttacker->landMarkerAttack(lpPacket);

	if (lpAttacker->isHero())
		if (g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpPacket->wTargetPosX,lpPacket->wTargetPosY))
			g_hero.stopBattle();
}

//
//	인터벌 슈터 작렬!!
void
cPACKET_HANDLER::ReceiveStrikeIntervalShooter(SERVER_PACKETS *_lpPacket)
{
	SG_STRIKE_INTERVAL_SHOOTER	*lpPacket	=	&_lpPacket->mSG_STRIKE_INTERVAL_SHOOTER;
	CActor						*lpTarget	=	g_am.getTestedActor(lpPacket->wTargetSerial);
	cAbility					ability;
	int							iPhycalDamage,iMagicDamage;

	if (!lpTarget)	return;

	ability.set(lpPacket->wSkill,10);

	iPhycalDamage	=	lpPacket->iPhysicalDamage*100;
	iMagicDamage	=	lpPacket->iMagicDamage*100;

	g_sm.strikeIntervalShooter(lpTarget,&ability,iPhycalDamage,iMagicDamage,lpPacket->wShotCount);
}

//
//	인터벌 슈터 발사
void
cPACKET_HANDLER::ReceiveCastIntervalShooter(SERVER_PACKETS *_lpPacket)
{
	SG_CAST_INTERVAL_SHOOTER	*lpPacket	=	&_lpPacket->mSG_CAST_INTERVAL_SHOOTER;

	CActor			*lpCaster	=	g_am.getTestedActor(lpPacket->wSerial);

	if (!g_am.checkDeathStatus(lpCaster))	return;

	lpCaster->castIntervalShooter(lpPacket);
}

void
cPACKET_HANDLER::ReceiveGuardianPost(SERVER_PACKETS *_lpPacket)
{
	SG_GUARDIAN_POST	*lpPacket	=	&_lpPacket->mSG_GUARDIAN_POST;
	CActor				*lpCaster	=	g_am.getTestedActor(lpPacket->bf12Caster);
	BOOL				bIsPlaySound=	TRUE;

	if	(lpPacket->wImage	==	0xffff)	// 포스트 제거
	{
		g_gpManager.removePost(lpPacket->wRegSerial);

		return;
	}

	if	(lpPacket->wTargetCount	==	0)		//	포스트 추가
	{
		g_gpManager.addPost(lpPacket->wX,lpPacket->wY,lpPacket->wImage,lpPacket->wRegSerial);

		return;
	}

	g_gpManager.addPost(lpPacket->wX,lpPacket->wY,lpPacket->wImage,lpPacket->wRegSerial);

	cAbility	ability;

	ability.set(lpPacket->bf12Skill,lpPacket->bf8Level);

	mBeginTLog();
	g_eh.addLog("ReceiveGuardianPost %d",lpPacket->wTargetCount);

	for (int i=0;i<lpPacket->wTargetCount;i++)
	{
		if	(i	>=	2)
			bIsPlaySound	=	FALSE;

		cWIDE_AREA_DAMAGE_E	*lpInfo	=	&lpPacket->aData[i];
		CHitInfo			hitInfo;

		hitInfo.m_dwPhysicalDamage	=	lpInfo->wPhysicalDamage*100;		//	얼마나? 아파? ;;
		hitInfo.m_dwMagicDamage		=	lpInfo->wMagicDamage*100;		//	얼마나? 아파? ;;
		hitInfo.m_dwResultField		=	lpInfo->dwResultField;	//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션

		CActor	*lpTarget			=	g_am.getTestedActor(lpInfo->wActor);

		if	(!lpTarget)
			continue;

		g_sm.castSpecialMissile(NULL,lpTarget,lpPacket->wX,lpPacket->wY,&hitInfo,&ability);
//		lpTarget->hit(lpCaster,&ability,&hitInfo,0,bIsPlaySound);
	}

	if	(lpCaster->isHero())
		g_hero.m_iCP	=	lpPacket->sCP*100;
}

//
//연속타격을 입히는 공격

void
cPACKET_HANDLER::ReceiveBunshineAttack(SERVER_PACKETS *_lpPacket)
{
	SG_BUNSHINE_ATTACK	*lpPacket	=	&_lpPacket->mSG_BUNSHINE_ATTACK;

	CActor			*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Serial);
	CActor			*lpTarget	=	g_am.getTestedActor(lpPacket->bf11TargetSerial);
	cAbility		ability;

	if (!lpTarget)	return;
	if (!g_am.checkDeathStatus(lpAttacker))	return;

	ability.set(lpPacket->bf11Skill,lpPacket->bf7Level);

	if (!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	lpAttacker->attackToActorByBunshine(lpPacket);

	if (lpAttacker->isHero())
		if (g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			g_hero.stopBattle();
}


//
//연속타격을 입히는 공격

void
cPACKET_HANDLER::ReceiveScimitarCutting(SERVER_PACKETS *_lpPacket)
{
	SG_SCIMITAR_CUTTING	*lpPacket	=	&_lpPacket->mSG_SCIMITAR_CUTTING;

	CActor				*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Serial);
	CActor				*lpTarget	=	g_am.getTestedActor(lpPacket->bf11TargetSerial);
	cAbility			ability;

	if	(!lpTarget)
		return;
	if	(!g_am.checkDeathStatus(lpAttacker))
		return;

	ability.set(lpPacket->bf11Skill,lpPacket->bf7Level);

	if (!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	lpAttacker->attackToActorByScimitarCutting(lpPacket);
}

//
//	퀵 액션

void
cPACKET_HANDLER::QuickAction(SERVER_PACKETS *_lpPacket)
{
	cAbility		ability;
	SG_QUICK_ACTION	*lpPacket	=	&_lpPacket->mSG_QUICK_ACTION;
	CActor			*lpAttacker	=	g_am.getTestedActor(lpPacket->bf11Serial);

	if (!g_am.checkDeathStatus(lpAttacker))
		return;

	ability.set(lpPacket->bf11Skill,lpPacket->bf7Level);

	if (!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	if (lpAttacker->isHero())
	{
		if (!ability.getSkill()->m_wIsNotSyncCP)	
			g_hero.reduceCP(ability.getSpentCP());	//	CP 감소

		g_hero.m_wIsReadyToFireChargedMiniPetSkill	=	lpPacket->bf1MiniPetCharge1;
		g_hero.m_wIsReadyToFireChargedMiniPetSkill2	=	lpPacket->bf1MiniPetCharge2;
	}

	lpAttacker->setDirect(lpPacket->bf4Direct);
	lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);
	lpAttacker->quickAction(&ability);

	lpAttacker->m_iFPS		=	lpPacket->bf6FPS;
}

void
cPACKET_HANDLER::WideAidAttackResult(SERVER_PACKETS *_lpPacket)
{
	SG_WIDE_AID_ATTACK_RESULT	*lpPacket	=	(SG_WIDE_AID_ATTACK_RESULT	*)_lpPacket;
	CActor						*lpCaster	=	g_am.getTestedActor(lpPacket->bf12Caster);
	CSkill						*lpSkill	=	&g_aSkill[lpPacket->bf12Skill];

	for	(int i=lpPacket->bf6Count;i;)
	{
		--i;

		cWideAideAttackResultE	*lpInfo		=	&lpPacket->aData[i];
		CActor					*lpTarget	=	g_am.getTestedActor(lpInfo->m_bf12Target);

		if	(!lpTarget)
			continue;

		if	(lpInfo->m_bf1IsHit)
		{
			if	(lpSkill->m_wAidAttackImage	!=	0xffff)
				lpTarget->addEffect(lpSkill->m_wAidAttackImageOutputPart,lpSkill->m_wAidAttackImage);
		}
		else
		{
			if	(lpSkill->m_wMissImage	==	0xffff)
				lpTarget->addHitInfo(dHIT_INFO_RED_MISS);
			else
				lpTarget->addEffect(lpSkill->m_wHitImageOutputPart,lpSkill->m_wMissImage);
		}

		if	(i <= 2 && lpSkill->m_bf1PlayCastSoundByEnchantAidSkill)
			g_esm.playCastingSound(lpSkill);
	}

	if	(lpCaster	&&	lpCaster->isHero())
		g_hero.m_iCP	=	lpPacket->sRemainCP*100;
}

//
//	광역 공격
void
cPACKET_HANDLER::WideAreaDamage(SERVER_PACKETS *_lpPacket)
{
	SG_WIDE_AREA_DAMAGE	*lpPacket	=	&_lpPacket->mSG_WIDE_AREA_DAMAGE;
	CActor				*lpCaster	=	g_am.getTestedActor(lpPacket->bf12Caster);

	if	(!lpCaster)
		return;

	if	(lpPacket->bf1IsDamagePool)
	{
		lpCaster->operateWideAreaDamage(lpPacket);
		return;
	}

	CSkill	*lpSkill	=	&g_aSkill[lpPacket->bf12Skill];

	switch(lpSkill->m_wType)
	{

	case	eSKILL_TYPE_CHAIN_LIGHTNING:
		lpCaster->operateChainLightningSkill(lpPacket);
		break;
	case	eSKILL_TYPE_AWESOME_FORTRESS:
		lpCaster->operateAwesomeFortressAttack(lpPacket);
		break;
	case	eSKILL_TYPE_BOUNCING_LINEAR:
		lpCaster->operateBouncingLinear(lpPacket);
		break;
	case	eSKILL_TYPE_BIG_BULLET:
		lpCaster->operateBigBullet(lpPacket);
		break;
	case	eSKILL_TYPE_BOUNCING_MISSILE:
		lpCaster->operateBouncingMissileSkill(lpPacket);
		break;
	case	eSKILL_TYPE_WIDE_MISSILE:
		lpCaster->operateWideMissileSkill(lpPacket);
		break;
	default:
		lpCaster->operateWideAreaDamage(lpPacket);
	}
}

//
//	광역 힐
void
cPACKET_HANDLER::WideHeal(SERVER_PACKETS *_lpPacket)
{
	SG_WIDE_HEAL	*lpPacket	=	(SG_WIDE_HEAL	*)_lpPacket;

	for	(int i = 0;i<lpPacket->wCount;i++)
	{
		cWideHealE	*lpInfo	=	&lpPacket->aData[i];
		CActor		*lpActor=	g_am.getActor(lpInfo->wActor);

		if	(lpActor)
		{
			lpActor->cure(lpPacket->wSkill,lpInfo->wHealPoint*100);

			if	(i	<=	2)
				g_esm.playHitSound(&g_aSkill[lpPacket->wSkill]);
		}
	}
}

//
//	스킬 사용 결과
void
cPACKET_HANDLER::UseSkillResult(SERVER_PACKETS *_lpPacket)
{
	SG_USE_SKILL_RESULT	*lpPacket	=	&_lpPacket->mSG_USE_SKILL_RESULT;

	g_hero.m_iCP	=	lpPacket->sRemainCP*100;

	switch(lpPacket->wResult)
	{
		case	eAR_CAN_NOT_CLOSE_ATTACK_ON_HILL:
			g_lpHero->warning(dMSG_CAN_NOT_CLOSE_ATTACK_ON_THIS_PLACE);
			break;

		case	eAR_NOT_APT_USED_SKILL_TO_TARGET	:
			g_lpHero->warning(dMSG_NOT_APT_USED_SKILL_TO_TARGET);
			break;

		case	eAR_BLOCKED_OBJECT_EXIST				:
			g_lpHero->warning(dMSG_BLOCKED_OBJECT_EXIST);
			break;

		case	eAR_IS_UNDER_TARGET_ULTIMATE_BARRIER	:
			CGamePlay::WarningMessage(dMSG_CAN_NOT_ATTACK_TARGET_BY_IS_UNDER_TARGET_ULTIMATE_BARRIER);
			break;

		case	eAR_REQUIRE_TARGET_SKILL		:
			g_lpHero->warning(dMSG_IS_ACTION_TO_TARGET_SKILL);
			break;

		case	eAR_NOT_LEARNED_SKILL			: 	//	배우지 않는 스킬
			break;
		case	eAR_BUSY						:	//	바빠요
			break;
		case	eAR_LOW_CP						:	//	CP가 모자른다.
			g_lpHero->warning(dMSG_CAN_NOT_USE_SKILL_LOWER_CP);
			break;
		case	eAR_TOO_DISTANCE				:	//	너무 멀다.
			break;
		case	eAR_IS_CAN_NOT_ATTACK_TARGET	:	//	공격할 수 없는 녀석이다.
			break;
		case	eAR_FAILED						:	//	걍 실패
			break;
		case	eAR_SET_SPECIAL_SKILL_FIELD		:	//	특수 스킬 필드를 설정했다.
			break;
		case	eAR_REQUIRE_WEAPON				:	//	무기가 필요한 스킬
			break;
		case	eAR_REQUIRE_SHIELD				:	//	방패가 필요한 스킬
			break;
		case	eAR_REQUIRE_BULLET				:	//	탄환이 필요한 스킬
			break;
		case	eAR_NOT_EXIST_SKILL				: 	//	존재하지 않는 스킬
			break;
		case	eAR_CONTINUOUS_ATTACK_BY_VALUE	:	//	배우지 않는 스킬
			break;
		case	eAR_TARGET_DEATH				:	//	배우지 않는 스킬
			break;

		case	eAR_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE	:
			CGamePlay::WarningMessage(dMSG_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE	);
			break;

		case	eAR_CAN_NOT_APT_SKILL_TO_THIS_STATUS	:
			CGamePlay::WarningMessage(dMSG_CAN_NOT_APT_SKILL_TO_THIS_STATUS	);
			break;

		case	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_CONFUSE	:
			CGamePlay::WarningMessage(dMSG_CAN_NOT_ACTION_BY_CONFUSE);
			break;

		case	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_BERSERK	:
			CGamePlay::WarningMessage(dMSG_CAN_NOT_ACTION_BY_BERSERK);
			break;

		case	eAR_IS_COOL_TIME							:
			CGamePlay::WarningMessage(dMSG_AR_IS_COOL_TIME);
			break;

		case	eAR_CAN_NOT_USE_IN_DUEL_FIELD_SKILL	:
			CGamePlay::WarningMessage(dMSG_AR_CAN_NOT_USE_IN_DUEL_FIELD_SKILL);
			break;
	}
}

void
cPACKET_HANDLER::ReceiveMiss(SERVER_PACKETS *_lpPacket)
{
	SG_MISS	*lpPacket	=	(SG_MISS	*)_lpPacket;

	CActor	*lpTarget	=	g_am.getTestedActor(lpPacket->wActor);
	CSkill	*lpSkill	=	&g_aSkill[lpPacket->wSkill];

	if (lpTarget	==	NULL)
		return;

	if	(lpSkill->m_wMissImage	==	0xffff)
		lpTarget->addHitInfo(dHIT_INFO_RED_MISS);
	else
		lpTarget->addEffect(lpSkill->m_wHitImageOutputPart,lpSkill->m_wMissImage);
}

//
//	리액션 켜기
void
cPACKET_HANDLER::ReceiveSetReactionStatus(SERVER_PACKETS *_lpPacket)
{
	s_agent.m_iSendSetReactionPacketTime	=	0;

	SG_SET_REACTION_STATUS	*lpPacket	=	&_lpPacket->mSG_SET_REACION_STATUS;

	switch(lpPacket->wResult)
	{
		case eRRS_OK					:	//	성공
			g_hero.setReactionStatus(lpPacket->wAbility,lpPacket->wIsOn);
			break;
		case eRRS_TOO_MANY_REACTION		:	//	이미 너무 많은 리액션이 설정되어 있다.
			g_lpHero->warning(dMSG_ACTIVE_REACTION_FULL);
			break;
		case eRRS_FAILED				:	//	아무튼 실패다.
			g_lpHero->warning(dMSG_FAILED);
			break;
		case eRRS_NOT_USE_REACTION_SKILL_TIME				:	//	리액션스킬을 사용할 수 없는 시간이다.
			g_lpHero->warning(dMSG_NOT_USE_REACTION_SKILL_TIME);
			break;
	}
}

//
//	가시에 찔림
void
cPACKET_HANDLER::ReceiveHitThornDamage(SERVER_PACKETS *_lpPacket)
{
	SG_HIT_THORN_DAMAGE	*lpPacket	=	&_lpPacket->mSG_HIT_THORN_DAMAGE;

	CActor		*lpAttacker,*lpTarget;

	lpAttacker	=	g_am.getTestedActor(lpPacket->wAttacker);
	lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if (!lpTarget)	return;
	if (!g_am.checkDeathStatus(lpAttacker))	return;

	lpTarget->hitThornDamage(lpAttacker,lpPacket->wDamage*100,lpPacket->sRemainCP*100, lpPacket->bIsDiplayMagicDamage);
}

//
//	대미지 흡수
void
cPACKET_HANDLER::ReceiveAbsorbDamage(SERVER_PACKETS *_lpPacket)
{
	SG_ABSORB_DAMAGE	*lpPacket	=	&_lpPacket->mSG_ABSORB_DAMAGE;

	CActor		*lpActor;

	lpActor	=	g_am.getTestedActor(lpPacket->wActor);

	if (!g_am.checkDeathStatus(lpActor))	return;

	lpActor->absorbDamage(lpPacket->iAbsorbHp);
	
	if (lpActor->isHero())
		g_hero.absorbDamage(lpPacket->iAbsorbHp);
}

//
//	스훼셜 스킬필드 변경
void
cPACKET_HANDLER::ReceiveChangeSpecialSkillField(SERVER_PACKETS *_lpPacket)
{
	SG_CHANGE_SPECIAL_SKILL_FIELD	*lpPacket	=	&_lpPacket->mSG_CHANGE_SPECIAL_SKILL_FIELD;

	WORD	wOld	=	g_hero.m_wSpecialSkillField;
	WORD	wNew	=	lpPacket->wSpecialSkillField;

	if ((wOld & dSSF_COMPLETE_PROTECTION) != (wNew & dSSF_COMPLETE_PROTECTION))
	{
		if (wNew & dSSF_COMPLETE_PROTECTION)
		{
			cAbility	ability;

			memcpy(&ability,&lpPacket->ability,sizeof(cAbility));

			if	(!ability.getSkill()->m_wIsNotSyncCP)
				g_hero.reduceCP(ability.getSpentCP());	//	CP 감소

			g_lpHero->warning(dMSG_TURN_ON_COMPLETE_PROTECTION);
		}
		else
		{
			g_lpHero->warning(dMSG_TURN_OFF_COMPLETE_PROTECTION);
		}
	}

	g_hero.m_wSpecialSkillField	=	wNew;
}

//
//	시머링 실드 설정
void
cPACKET_HANDLER::ReceiveSetShimmeringShield(SERVER_PACKETS *_lpPacket)
{
	SG_SET_SHIMMERING_SHIELD		*lpPacket	=	&_lpPacket->mSG_SET_SHIMMERING_SHIELD;

	CActor	*lpCaster	=	g_am.getTestedActor(lpPacket->wCaster);
	CActor	*lpReceiver	=	g_am.getTestedActor(lpPacket->wReceiver);

	if (!lpReceiver)
		return;
	if (!g_am.checkDeathStatus(lpCaster))
		return;

	if	(lpCaster->isHero())	
		g_hero.m_iCP	=	lpPacket->sRemainCP*100;

	lpReceiver->m_wBlockerShape			=	0xffff;
	lpReceiver->m_wSpearBlockerShape	=	0xffff;

	if	(lpPacket->bf1IsShield)
	{
		lpCaster->m_wIsCastBlocker		=	TRUE;
		lpReceiver->m_wBlockerShape		=	lpPacket->bf6ShieldShape;
	}
	else
		lpReceiver->m_wSpearBlockerShape=	lpPacket->bf6ShieldShape;

	lpReceiver->m_wBlockerAngle			=	random(360);
	lpReceiver->m_wBlockerRigidityTime	=	0;
}

void
cPACKET_HANDLER::ReceiveIcyStalagmite(SERVER_PACKETS *_lpPacket)
{
	SG_ICY_STALAGMITE	*lpPacket	=	&_lpPacket->mSG_ICY_STALAGMITE;

	CActor				*lpCaster	=	g_am.getTestedActor(lpPacket->wActor);

	if (!lpCaster)	return;

	lpCaster->castIcyStalagmite(lpPacket);
}

void
cPACKET_HANDLER::ReceiveActorEnchantedStatus(SERVER_PACKETS *_lpPacket)
{
	SG_ACTOR_ENCHANTED_STATUS	*lpPacket	=	(SG_ACTOR_ENCHANTED_STATUS	*)_lpPacket;

	CActor						*lpActor	=	g_am.getTestedActor(lpPacket->wActor);

	if	(!lpActor)
		return;

	lpActor->m_dwEnchantedMask	=	lpPacket->dwMask;
	lpActor->m_enchantedImage.copy(&lpPacket->enchantedImage);
	lpActor->m_wItemEffect		=	lpPacket->wItemEffect;

	if	(lpPacket->dwMask&dAMEM_INVISIBLE)
		lpActor->m_wRevealTime	=	0;

		lpActor->m_wMagicCarpetType	=	lpPacket->bf7MagicCarpetShape;

	if	(g_bIsObserverModeForDuel && lpActor->m_wDuelBuffType	!=	lpPacket->bf3CurrentDuelBuff && lpPacket->bf3CurrentDuelBuff)
	{
		char *lpstrBuffName	=	g_alpDuelBuffItem[lpPacket->bf3CurrentDuelBuff-1]->m_strName;

		CGamePlay::AddSystemMessage(WHITE,dMSG_USE_BUFF_FORM,lpActor->m_strName,lpstrBuffName);
	}

	lpActor->m_wDuelBuffType				=	lpPacket->bf3CurrentDuelBuff;
	lpActor->m_wGGGBuffType					=	lpPacket->bf3CurrentGGGBuff;

	lpActor->checkEquipmentShape();
}

void
cPACKET_HANDLER::ReceiveTeleport(SERVER_PACKETS *_lpPacket)
{
	SG_TELEPORT	*lpPacket	=	&_lpPacket->mSG_TELEPORT;
	CActor		*lpActor	=	g_am.getTestedActor(lpPacket->wActor);

	if (!lpActor)
		return;

	if (lpPacket->wMethod	==	eTM_TELEPORT)
	{
		lpActor->stop(lpPacket->wX,lpPacket->wY);
		lpActor->addSkillHitEffect(lpPacket->wSkill);

		if (lpActor->isHero())
			g_hero.sendStop();	//	이걸 해줘야 주변 몹들 업뎃이 된다.
	}

	if (lpPacket->wMethod	==	eTM_JUMP)
	{
//		lpActor->jump(lpPacket->wX,lpPacket->wY,lpPacket->wSkill);
	}
}

void
cPACKET_HANDLER::ReceiveApplyExtraSkillEffect(SERVER_PACKETS *_lpPacket)
{		// 마지막 수정일 : 09.10.14
	SG_APPLY_SKILL_EXTRA_EFFECT	*lpPacket	=	&_lpPacket->mSG_APPLY_SKILL_EXTRA_EFFECT;

	CActor		*lpActor	=	g_am.getTestedActor(lpPacket->wActor);
	cAbility	ability;

	if	(!lpActor)
		return;

	ability.set(lpPacket->wSkill,lpPacket->wLevel);

	g_hero.applySkillExtraEffects(lpActor,&ability, lpPacket->dwKeepTime);	//	스킬 부가효과를 적용한다.
}

//
//스킬 효과 발동
void
cPACKET_HANDLER::ReceiveActiveMirrorTower(SERVER_PACKETS *_lpPacket)
{
	SG_ACTIVE_MIRROR_TOWER	*lpPacket	=	(SG_ACTIVE_MIRROR_TOWER	*)_lpPacket;

	CActor		*lpCaster	=	g_am.getTestedActor(lpPacket->wCaster);
	CActor		*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);
	cAbility	ability;
	CSkill		*lpSkill;

	if (!lpCaster)	return;
	if (!lpTarget)	return;

	ability.set(lpPacket->wSkill,10);

	lpSkill		=	ability.getSkill();

	if (lpSkill->m_wSkillImage	!=	0xffff	)
		lpTarget->addHitEffect(0,0,lpSkill->m_wSkillImage);

	CHitInfo	hitInfo;

	hitInfo.m_dwResultField		=	0;
	hitInfo.m_dwPhysicalDamage	=	0;
	hitInfo.m_dwMagicDamage		=	lpPacket->wMagicDamage*100;

	if (hitInfo.getDamage())	hitInfo.setHit();

	lpCaster->hit(lpCaster,&ability,&hitInfo,0,TRUE,TRUE);

	if (lpCaster->isHero()					)	
		g_hero.m_iCP	=	lpPacket->sRemainCP*100;

	lpTarget->addHealEffectAndInfo(0xffff,max(lpPacket->wTargetRecoverHP/100,1),eHEOP_FOOT);
}

//
//스킬 효과 발동
void
cPACKET_HANDLER::ReceiveActiveSkillEffect(SERVER_PACKETS *_lpPacket)
{
	SG_ACTIVE_SKILL_EFFECT		*lpPacket	=	&_lpPacket->mSG_ACTIVE_SKILL_EFFECT;

	CActor		*lpCaster	=	g_am.getTestedActor(lpPacket->wCaster);
	CActor		*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);
	cAbility	ability;
	CSkill		*lpSkill;

	if (!lpCaster)
		return;
	if (!lpTarget && lpPacket->wTarget != 0xffff)
		return;

	ability.set(lpPacket->wSkill,10);

	lpSkill		=	ability.getSkill();

	if (lpSkill->m_wSkillImage	!=	0xffff	)
	{
		CPos	pos;

		lpCaster->getHitPointCorrectPos(lpSkill->m_wSkillImageOutputPart,&pos);
		lpCaster->addHitEffect(pos.x,pos.y,lpSkill->m_wSkillImage);
	}

	if	(lpSkill->isAgressiveSkill()	||	lpSkill->m_bf1IsPutHitDamage)
	{
		CHitInfo	hitInfo;

		hitInfo.m_dwResultField		=	0;
		hitInfo.m_dwPhysicalDamage	=	lpPacket->wPhysicalDamage*100;
		hitInfo.m_dwMagicDamage		=	lpPacket->wMagicDamage*100;

		if (hitInfo.getDamage())
			hitInfo.setHit();

		lpTarget->hit(lpCaster,&ability,&hitInfo);

		if (lpCaster->isHero()					)
			g_hero.m_iCP	=	lpPacket->sRemainCP*100;
	}
}

//
//중독 대미지
void
cPACKET_HANDLER::ReceiveSimpleHit(SERVER_PACKETS *_lpPacket)
{
	SG_SIMPLE_HIT	*lpPacket	=	&_lpPacket->mSG_SIMPLE_HIT;
	CActor			*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if	(!lpTarget)
		return;

	LONGLONG		llMaxHp		=	lpTarget->m_iMaxHP;
	lpTarget->m_iHP				=	(int)(llMaxHp * lpPacket->bf15RemainHP/10000);
	lpTarget->m_iHP				=	max(lpTarget->m_iHP,1);

	switch(lpPacket->bf7AddHitType)
	{
	case eAHT_POISON:
		lpTarget->addHitEffectAndInfo(g_im.m_wHitPoisonEffect,lpPacket->iPhysicalDamage,lpPacket->iMagicDamage);
		break;
	case eAHT_DARK:
		lpTarget->addHitEffectAndInfo(g_im.m_wHitDarkDamageEffect,lpPacket->iPhysicalDamage,lpPacket->iMagicDamage);
		break;
	case eAHT_BLEEDING:
		lpTarget->addHitEffectAndInfo(g_im.m_wHitBleedingEffect,lpPacket->iPhysicalDamage,lpPacket->iMagicDamage);
		break;
	case eAHT_ELECTRIC_SHOCK:
		lpTarget->addHitEffectAndInfo(g_im.m_wHitElectricShockEffect,lpPacket->iPhysicalDamage,lpPacket->iMagicDamage);
		break;
	}
}


// 대미지 입었다.
void
cPACKET_HANDLER::ReceiveHitDamage(SERVER_PACKETS *_lpPacket)
{
	SG_HIT_DAMAGE	*lpPacket	=	(SG_HIT_DAMAGE *)_lpPacket;
	CActor			*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);
	CActor			*lpCaster	=	g_am.getTestedActor(lpPacket->wCaster);
	cAbility		ability;
	CHitInfo		hitInfo;

	if	(!lpTarget)
		return;

	ability.set(lpPacket->bf11Skill,1);

	hitInfo.m_dwPhysicalDamage	=	lpPacket->iPhysicalDamage;		//	얼마나? 아파? ;;
	hitInfo.m_dwMagicDamage		=	lpPacket->iMagicDamage;		//	얼마나? 아파? ;;
	hitInfo.m_dwResultField		=	lpPacket->dwResultField;	//	1 히트,2 크리티컬,4 크러쉬 블로우,8 블럭,16 히트 액션

	lpTarget->hit(lpCaster,&ability,&hitInfo);

	LONGLONG		llMaxHp		=	lpTarget->m_iMaxHP;
	lpTarget->m_iHP				=	(int)(llMaxHp * lpPacket->wRemainHP/10000);
	lpTarget->m_iHP				=	max(lpTarget->m_iHP,1);
}
// 라이트닝 와인더 발사
void
cPACKET_HANDLER::ReceiveStrikeLightningWinder(SERVER_PACKETS *_lpPacket)
{
	SG_STRIKE_LIGHTNING_WINDER	*lpPacket	=	(SG_STRIKE_LIGHTNING_WINDER	*)_lpPacket;

//	CActiveSkill	*lpAS	=	g_sm.getActiveSkill(lpPacket->wSkill,lpPacket->wCaster);
	CActor			*lpTarget=	g_am.getActor(lpPacket->wTarget);
	CActor			*lpCaster=	g_am.getActor(lpPacket->wCaster);

	if	(!lpTarget	||	!lpCaster)
		return;

	CActiveSkill	*lpAS	=	lpCaster->getActiveSkill();

	if	(!lpAS	||	lpAS->m_lpSkill->m_wSerial	!=	lpPacket->wSkill)
		return;

	cAS_LightningWinder	*lpLW	=	(cAS_LightningWinder	*)lpAS;

	lpLW->shoot(lpTarget,lpPacket->wPhysicalDamage*100,lpPacket->wMagicDamage*100,lpPacket->dwResultField);
}
void
cPACKET_HANDLER::ReceiveStrikeMultiMissile(SERVER_PACKETS *_lpPacket)
{
	SG_STRIKE_MULTI_MISSILE	*lpPacket	=	(SG_STRIKE_MULTI_MISSILE	*)_lpPacket;

//	CActiveSkill	*lpAS	=	g_sm.getActiveSkill(lpPacket->wSkill,lpPacket->wCaster);
	CActor			*lpTarget=	g_am.getActor(lpPacket->wTarget);
	CActor			*lpCaster=	g_am.getActor(lpPacket->wCaster);

	if	(!lpTarget	||	!lpCaster)
		return;
//	lpCaster->m_wUseSkill = 255;

	CActiveSkill	*lpAS	=	lpCaster->getActiveSkill();



	if	(!lpAS	||	lpAS->m_lpSkill->m_wSerial	!=	lpPacket->wSkill)
	{
		return;

	}
	cAS_MultiMissile	*lpMM	=	(cAS_MultiMissile	*)lpAS;

	lpMM->shoot(lpTarget,lpPacket->dwPhysicalDamage*100,lpPacket->dwMagicDamage*100,lpPacket->dwResultField);
}
//
// 스킬 이펙트 발생
void
cPACKET_HANDLER::ReceiveSkillEffect(SERVER_PACKETS *_lpPacket)
{
	SG_SKILL_EFFECT	*lpPacket	=	(SG_SKILL_EFFECT	*)_lpPacket;

	CActor			*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);
	cAbility		ability;

	if (!lpTarget)
		return;

	ability.set(lpPacket->wSkill,lpPacket->bf8Level);
	CSkill	*lpSkill	=	ability.getSkill();

	if (!lpSkill)
		return;

	switch(lpPacket->bf8ImageType)
	{
		case	eSE_HIT		:
			g_esm.playHitSound(lpSkill);
			lpTarget->addEffect(lpSkill->m_wHitImageOutputPart,lpSkill->m_wHitImage);
			break;
		case	eSE_SKILL	:
			lpTarget->addEffect(lpSkill->m_wSkillImageOutputPart,lpSkill->m_wSkillImage,0xffff,lpSkill->m_wImageScale,lpSkill->m_wImageScale);
			break;
		case	eSE_HEAL	:
			lpTarget->addEffect(lpSkill->m_wHealImageOutputPart,lpSkill->m_wHealImage);
			break;
		case	eSE_CAST	:
			g_esm.playCastingSound(lpSkill);
			lpTarget->addEffect(lpSkill->m_wCastImageOutputPart,lpSkill->m_wCastImage);
			break;
		case	eSE_AID_SKILL	:
			lpTarget->addEffect(lpSkill->m_wAidSkillCastingImageOutputPart,lpSkill->m_wAidSkillCastingImage);
			break;
		case	eSE_CASTER_HIT	:
			lpTarget->addEffect(lpSkill->m_wCasterHitImageOutputPart,lpSkill->m_wCasterHitImage);
			break;
		case	eSE_EXPLOSION	:
			lpTarget->addEffect(eHEOP_FOOT,lpSkill->m_wExplosionImage);
			break;
	}
}

void
cPACKET_HANDLER::ReceivePlaySkillSound(SERVER_PACKETS *_lpPacket)
{
	SG_PLAY_SKILL_SOUND	*lpPacket	=	(SG_PLAY_SKILL_SOUND	*)_lpPacket;

	CSkill	*lpSkill	=	&g_aSkill[lpPacket->wSkill];

	switch(lpPacket->wSound)
	{
		case	eSSD_CASTING	:
			g_esm.playCastingSound(lpSkill);
			break;
		case	eSSD_HIT		:
			g_esm.playHitSound(lpSkill);
			break;
		case	eSSD_ACTION		:
			g_esm.playActionSound(lpSkill);
			break;
		case	eSSD_CREATE		:
			g_esm.playCreateSound(lpSkill);
			break;
		case	eSSD_EXPLOSION	:
			g_esm.playExplosionSound(lpSkill);
			break;
		case	eSSD_MISS	:
			g_esm.playMissSound(lpSkill);
			break;

		case	eSSD_EE_SUCCESS		:
			g_esm.playSEE_SuccessSound(lpSkill);
			break;

		case	eSSD_EE_MISS		:
			g_esm.playSEE_MissSound(lpSkill);
			break;
	}
}

void
cPACKET_HANDLER::ReceiveRegenHPByAidSkill(SERVER_PACKETS *_lpPacket)
{
	SG_REGEN_HP_BY_AID_SKILL	*lpPacket	=	&_lpPacket->mSG_REGEN_HP_BY_AID_SKILL;
	CActor						*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if	(!lpTarget)
		return;

	LONGLONG		llMaxHp		=	lpTarget->m_iMaxHP;
	lpTarget->m_iHP				=	(int)(llMaxHp * lpPacket->wRemainHP/10000);
	lpTarget->m_iHP				=	max(lpTarget->m_iHP,1);

	lpTarget->addHealEffectAndInfo(g_im.m_wRegenHPImage,lpPacket->wRegenHP,eHEOP_FOOT);
}

void
cPACKET_HANDLER::ReceiveRecoverHPByAidSkill(SERVER_PACKETS *_lpPacket)
{
	SG_RECOVER_HP_BY_AID_SKILL	*lpPacket	=	(SG_RECOVER_HP_BY_AID_SKILL	*)_lpPacket;
	CActor						*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if	(!lpTarget)
		return;

	CSkill	*lpSkill	=	&g_aSkill[lpPacket->wSkill];

	if (lpTarget->isHero())
	{
		g_hero.m_iHP		=	lpPacket->iRemainHP;

		if (lpSkill->m_bf1IsInstanceHeal)
		{
			g_hero.m_iHP	+=	lpPacket->wRecoverHP*100;
			g_hero.m_iHP	=	min(g_hero.m_iHP,g_hero.getMaxHP());
		}
		else
			g_hero.healHP(lpPacket->wRecoverHP*100);
	}
	else
	{
		lpTarget->m_iHP		=	lpPacket->iRemainHP;

		if (lpSkill->m_bf1IsInstanceHeal)
			lpTarget->m_iHP	+=	lpPacket->wRecoverHP*100;
	}

	lpTarget->addHealEffectAndInfo(lpSkill->m_wHealImage,lpPacket->wRecoverHP,lpSkill->m_wHealImageOutputPart);
}

void
cPACKET_HANDLER::ReceiveHitAction(SERVER_PACKETS *_lpPacket)
{
#ifdef	_IS_DEV_CLIENT
	return;
#endif

	SG_HIT_ACTION			*lpPacket	=	&_lpPacket->mSG_HIT_ACTION;

	CActor	*lpActor	=	g_am.getTestedActor(lpPacket->wActor);

	if (!lpActor)
		return;

	lpActor->hitAction();
}

void
cPACKET_HANDLER::ReceiveDetectingEvil(SERVER_PACKETS *_lpPacket)
{
	SG_DETECTING_EVIL	*lpPacket	=	(SG_DETECTING_EVIL	*)_lpPacket;

	memcpy(g_am.m_aDetetingActorList,lpPacket->aTarget,sizeof(CDetectingEvilTargetInfo)*lpPacket->wCount);

	g_am.m_iDetectingActorCount		=	lpPacket->wCount;
	g_am.m_iDetectingActorTime		=	dSYNC_FPS*10;
}

void
cPACKET_HANDLER::ReceiveReadyToWarpToNearestVillage(SERVER_PACKETS *_lpPacket)
{
	SG_READY_TO_WARP_TO_NEAREST_VILLAGE	*lpPacket	=	(SG_READY_TO_WARP_TO_NEAREST_VILLAGE	*)_lpPacket;

	g_gwSelectVillage.open(lpPacket->awVillage,lpPacket->wVillageCount,lpPacket->wIsCreatePortal);

	CSkill	*lpSkill	=	&g_aSkill[lpPacket->wSkill];

	g_lpHero->addEffect(lpSkill->m_wHitImageOutputPart,lpSkill->m_wHitImage);
}

//
//	시머링 실드 해제
void
cPACKET_HANDLER::ReceiveReleaseShimmeringShield(SERVER_PACKETS *_lpPacket)
{
	SG_RELEASE_SHIMMERING_SHIELD	*lpPacket	=	&_lpPacket->mSG_RELEASE_SHIMMERING_SHIELD;

	CActor	*lpCaster			=	g_am.getTestedActor(lpPacket->wCaster);
	CActor	*lpReceiver			=	g_am.getTestedActor(lpPacket->wReceiver);

	if (lpCaster)
		lpCaster->m_wIsCastBlocker	=	FALSE;
	if (lpReceiver)
	{
		lpReceiver->m_wBlockerAngle	=	0xffff;
		
		if	(lpReceiver->m_dwEnchantedMask	&	dAMEM_SHIMMERING_SHIELD)
			lpReceiver->m_dwEnchantedMask	-=	dAMEM_SHIMMERING_SHIELD;
	}
}

//
//치료
void
cPACKET_HANDLER::ReceiveCureActor(SERVER_PACKETS *_lpPacket)
{
	SG_CURE_ACTOR	*lpPacket	=	&_lpPacket->mSG_CURE_ACTOR;
	CActor			*lpActor	=	g_am.getTestedActor(lpPacket->wActor);

	if	(!lpActor)
		return;

	lpActor->cure(lpPacket->wSkill,lpPacket->iHealPoint);

	if	(lpActor->isHero())
		g_hero.cure(lpPacket->wSkill,lpPacket->iHealPoint);
}

//
//	머신 띄우기
void
cPACKET_HANDLER::ReceiveFloatMachineMessage(SERVER_PACKETS *_lpPacket)
{
	SG_FLOAT_MACHINE	*lpPacket	=	&_lpPacket->mSG_FLOAT_MACHINE;

	CActor	*lpActor	=	g_am.getTestedActor(lpPacket->wTarget);

	if (!g_am.checkDeathStatus(lpActor))	return;

	lpActor->floatMachine(lpPacket->wSkill);
}	//	cPACKET_HANDLER::ReceiveFloatMachineMessage(SERVER_PACKETS *_lpPacket)

//
//	머신 풀어주기
void
cPACKET_HANDLER::ReceiveReleaseMachineMessage(SERVER_PACKETS *_lpPacket)
{
	SG_RELEASE_MACHINE	*lpPacket	=	&_lpPacket->mSG_RELEASE_MACHINE;

	CActor	*lpActor	=	g_am.getTestedActor(lpPacket->wTarget);

	if (!g_am.checkDeathStatus(lpActor))	return;

	lpActor->releasMachine();
}	//	cPACKET_HANDLER::ReceiveReleaseMachineMessage(SERVER_PACKETS *_lpPacket)

//
//	머신이 미사일 발사
void
cPACKET_HANDLER::ReceiveShootMachineMessage(SERVER_PACKETS *_lpPacket)
{
	SG_SHOOT_MACHINE_MISSILE	*lpPacket	=	&_lpPacket->mSG_SHOOT_MACHINE_MISSILE;

	CActor	*lpCaster	=	g_am.getTestedActor(lpPacket->wCaster);
	CActor	*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if	(!lpTarget)
		return;
	if	(!g_am.checkDeathStatus(lpCaster))
		return;
	if	(lpCaster->m_wMachineSkill	!=	lpPacket->bf12Skill	&&	g_aSkill[lpPacket->bf12Skill].isMiniPetSkill()	==	FALSE)
		lpCaster->floatMachine(lpPacket->bf12Skill);

	CHitInfo	hitInfo;

	hitInfo.m_dwPhysicalDamage	=	lpPacket->hitInfo.m_wPhysicalDamage*100;
	hitInfo.m_dwMagicDamage		=	lpPacket->hitInfo.m_wMagicDamage*100;
	hitInfo.m_dwResultField		=	lpPacket->hitInfo.m_dwResultField;

	g_sm.castMachineMissile(lpCaster,lpTarget,&hitInfo,lpPacket->bf12Skill,lpPacket->m_bf2MinpetOrder);	//	발솨!!
}

//
//	테일 체이서
void
cPACKET_HANDLER::ReceiveTailChaser(SERVER_PACKETS *_lpPacket)
{	
	SG_FIRE_TAIL_CHASER	*lpPacket	=	&_lpPacket->mSG_FIRE_TAIL_CHASER;

	CActor	*lpCaster	=	g_am.getTestedActor(lpPacket->wCaster);
	CActor	*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if	(!lpTarget)
		return;
	if	(!g_am.checkDeathStatus(lpCaster))
		return;

	g_sm.castTailChaserMissiles(lpCaster,lpTarget,lpPacket->wDamage,lpPacket->wShotCount,lpPacket->wShotPeriod,lpPacket->wSkill,lpPacket->wCorrectDamage);	//	발솨!!

	if	(lpCaster->isHero())
		g_hero.m_iCP	=	lpPacket->sRemainCP*100;
}	//	cPACKET_HANDLER::ReceiveTailChaser(SERVER_PACKETS *_lpPacket)

//
//	패시브 스킬들 켜기
void
cPACKET_HANDLER::ReceiveTurnOnPassiveSkill(SERVER_PACKETS *_lpPacket)
{
	SG_TURN_ON_PASSIVE_SKILL	*lpPacket	=	&_lpPacket->mSG_TURN_ON_PASSIVE_SKILL;

	g_hero.turnOnPassiveSkill(lpPacket->aPassiveSkill);
}

void
cPACKET_HANDLER::ReceiveDisplacement(SERVER_PACKETS *_lpPacket)
{
	SG_DISPLACEMENT			*lpPacket	=	(SG_DISPLACEMENT	*)_lpPacket;

	CActor	*lpCaster	=	g_lpHero;
	CActor	*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if (lpPacket->wIsDisplacement)
	{
		if (lpTarget	==	NULL)
			return;

		lpCaster->m_wSlaveActor		=	lpPacket->wTarget;
		lpCaster->m_wIsFeignDeath	=	TRUE;
		lpCaster->m_posFeignDeath.x	=	lpCaster->m_pos.x;
		lpCaster->m_posFeignDeath.y	=	lpCaster->m_pos.y;
		lpTarget->m_wLordActor		=	g_lpHero->m_wSerial;

		lpTarget->stop();
		lpCaster->stop();

		g_lpControlActor			=	lpTarget;

		lpTarget->addHitEffect(0,-lpTarget->getBodyHeight()*5/7,g_im.m_wCastDisplacementImage);
	}
	else
	{
		lpCaster->m_wSlaveActor		=	0xffff;
		lpCaster->m_wIsFeignDeath	=	FALSE;
		lpCaster->addHitEffect(0,-lpCaster->getBodyHeight()*5/7,g_im.m_wCastDisplacementImage);

		if (lpTarget)
		{
			lpTarget->m_wLordActor	=	0xffff;
			lpTarget->stop();
		}

		g_lpControlActor			=	NULL;
	}
}

//
//	밀려나기!!
void
cPACKET_HANDLER::ReceiveThrust(SERVER_PACKETS *_lpPacket)
{
	SG_THRUST	*lpPacket	=	&_lpPacket->mSG_THRUST;
	CActor		*lpTarget	=	g_am.getTestedActor(lpPacket->bf15Serial);

	if	(!lpTarget)
		return;

	lpTarget->thrust(lpPacket->wPosX,lpPacket->wPosY,0xffff,lpPacket->bf1IsBlockedByObject,lpPacket->bf1IsChangeTargetDirect );
}

//
//	얼라들을 밀어낸다.
void
cPACKET_HANDLER::ReceiveThrustActors(SERVER_PACKETS *_lpPacket)
{
	SG_THRUST_ACTORS			*lpPacket	=	&_lpPacket->mSG_THRUST_ACTORS;

	for (int i=0;i<lpPacket->wCount;i++)
	{
		CActor *lpActor	=	g_am.getTestedActor(lpPacket->aActor[i].bf11SerialInField);

		if	(!lpActor)
			continue;

		lpActor->thrust(lpPacket->aActor[i].wX,lpPacket->aActor[i].wY,lpPacket->wSkill,FALSE);
	}
}

//
//	액션을 취한다.
void
cPACKET_HANDLER::ReceiveMakeActionForm(SERVER_PACKETS *_lpPacket)
{
	SG_MAKE_ACTION_FORM		*lpPacket			=	&_lpPacket->mSG_MAKE_ACTION_FORM;
	cAbility				ability;
	BOOL					bIsQuickSkill		=	FALSE;
	CActor					*lpActor,*lpTarget	=	NULL;

	ability.set(lpPacket->ability.m_wSkill,lpPacket->ability.m_wLevel);

	if	(lpPacket->wTarget	==	0xffff)
		bIsQuickSkill	=	TRUE;

	lpActor	=	g_am.getTestedActor(lpPacket->wCaster);

	if	(!bIsQuickSkill)
		lpTarget=	g_am.getTestedActor(lpPacket->wTarget);

	if	(!bIsQuickSkill	&&	!lpTarget	)
		return;
	if	(!g_am.checkDeathStatus(lpActor))
		return;

	if	(!ability.isEnableJob(lpActor->m_wJob))
	{
		lpActor->addToObscurityActorList();
		return;
	}

	if	(lpActor->isHero())
	{
		g_hero.m_iCP=	lpPacket->sRemainCP*100;
		lpActor->stop();
	}
	else
		lpActor->stop(lpPacket->wPosX,lpPacket->wPosY);

	if	(bIsQuickSkill)
		lpActor->action(lpActor->m_pos.x,lpActor->m_pos.y,ability.getSkill()->m_wAction);
	else
		lpActor->action(lpTarget->m_pos.x,lpTarget->m_pos.y,ability.getSkill()->m_wAction);

	if	(lpTarget)
	{
		CHitInfo	hitInfo;

		hitInfo.m_dwPhysicalDamage	=	lpPacket->hitInfo.m_wPhysicalDamage*100;
		hitInfo.m_dwMagicDamage		=	lpPacket->hitInfo.m_wMagicDamage*100;
		hitInfo.m_dwResultField		=	lpPacket->hitInfo.m_dwResultField;

		lpTarget->hit(lpActor,&ability,&hitInfo);
	}

	lpActor->setExclusiveAction(TRUE);
}	//	cPACKET_HANDLER::ReceiveMakeActionForm(SERVER_PACKETS *_lpPacket)

//
//	얼라에게 스킬 사용

void
cPACKET_HANDLER::ReceiveUseSkillToActor(SERVER_PACKETS *_lpPacket)
{
	SG_USE_SKILL_TO_ACTOR	*lpPacket	=	&_lpPacket->mSG_USE_SKILL_TO_ACTOR;
	CActor					*lpAttacker	=	g_am.getTestedActor(lpPacket->wActor);
	CActor					*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);
	cAbility				ability;

	if (!lpTarget)	return;
	if (!g_am.checkDeathStatus(lpAttacker))	return;

	ability.set(lpPacket->ability.m_wSkill,lpPacket->ability.m_wSkill);

	if (!lpAttacker->isTransformationMonster() && !ability.isEnableJob(lpAttacker->m_wJob))
	{
		lpAttacker->addToObscurityActorList();
		return;
	}

	if (lpAttacker->isHero())
	{
		g_hero.m_iCP	=	lpPacket->sRemainCP*100;
		lpAttacker->stop();
	}
	else	lpAttacker->stop(lpPacket->wPosX,lpPacket->wPosY);

	lpAttacker->m_attackInfo.reset();
	lpAttacker->m_attackInfo.m_wTarget	=	lpPacket->wTarget;

	CHitInfo	hitInfo;

	hitInfo.m_dwPhysicalDamage	=	lpPacket->hitInfo.m_wPhysicalDamage*100;
	hitInfo.m_dwMagicDamage		=	lpPacket->hitInfo.m_wMagicDamage*100;
	hitInfo.m_dwResultField		=	lpPacket->hitInfo.m_dwResultField;

	lpAttacker->m_attackInfo.m_aInfo[0].copy(&hitInfo);

	lpAttacker->attackToActor(lpTarget,&ability,0);
	lpAttacker->setExclusiveAction(TRUE);

	if (lpAttacker->isHero())
		if (g_map.isMissileBlockedLine(lpAttacker->m_pos.x,lpAttacker->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
			g_hero.stopBattle();
//	lpAttacker->m_iFPS		=	lpPacket->wFPS;
}

//
//	땅에다 스킬 사용
void
cPACKET_HANDLER::ReceiveUseSkillToGround(SERVER_PACKETS *_lpPacket)
{
	SG_USE_SKILL_TO_GROUND	*lpPacket	=	&_lpPacket->mSG_USE_SKILL_TO_GROUND;
}

//
//
void
cPACKET_HANDLER::ReceiveCreateDummy(SERVER_PACKETS *_lpPacket)
{
	SG_CREATE_DUMMY		*lpPacket	=	&_lpPacket->mSG_CREATE_DUMMY;

	BOOL	bIsDummy=FALSE,bIsCopyActor=FALSE;

	if	(lpPacket->base.wType==dSG_CREATE_DUMMY)
		bIsDummy	=	TRUE;
	if	(lpPacket->base.wType==dSG_COPY_ACTOR)
		bIsCopyActor=	TRUE;

	if	(lpPacket->actorInfo.bf2Kind	==	eAK_PLAYER)
		g_am.joinActor(&lpPacket->actorInfo,bIsDummy,bIsCopyActor);
	else
		g_am.joinMonster((CSimpleMonsterInfo *)&lpPacket->actorInfo,bIsDummy,bIsCopyActor);
}

//
//	더미 제거
void
cPACKET_HANDLER::ReceiveReleaseDummy(SERVER_PACKETS *_lpPacket)
{
	SG_RELEASE_DUMMY	*lpPacket	=	&_lpPacket->mSG_RELEASE_DUMMY;
	CActor				*lpDummy	=	g_am.getTestedActor(lpPacket->wActor,FALSE);

	if	(!lpDummy)
	{
		g_am.removeActor(lpPacket->wActor);
		return;
	}

	lpDummy->m_wDummyStatus	=	dDISAPEAR_DUMMY;
}

//
//	보조마법
void
cPACKET_HANDLER::ReceiveReceiveAidMagic(SERVER_PACKETS *_lpPacket)
{
	SG_RECEIVE_AID_MAGIC	*lpPacket	=	&_lpPacket->mSG_RECEIVE_AID_MAGIC;
	CActor					*lpTarget	=	g_am.getTestedActor(lpPacket->wTarget);

	if	(!lpTarget)
		return;

	lpTarget->receiveAidMagic(lpPacket->wSkill,lpPacket->wType,lpPacket->sValue);
}

//
//	오라 스킬에 맞았다.
void
cPACKET_HANDLER::ReceiveHitAuraSkill(SERVER_PACKETS *_lpPacket)
{
	SG_HIT_AURA_SKILL	*lpPacket	=	&_lpPacket->mSG_HIT_AURA_SKILL;
	CActor				*lpActor	=	g_am.getTestedActor(lpPacket->wCaster);
	CSkill				*lpSkill	=	&g_aSkill[lpPacket->wSkill];

	if	(random(10)	==	0)
		g_esm.playCastingSound(lpSkill);

	g_esm.playHitSound(lpSkill);

	if	(!g_am.checkDeathStatus(lpActor))
		return;

	for (int i=0;i<lpPacket->bf10Count;i++)
	{
		CActor	*lpTarget	=	g_am.getTestedActor(lpPacket->aActorList[i]);

		if	(!lpTarget)
			continue;

		lpTarget->addSkillEffect(lpSkill->m_wSerial,lpPacket->bf6HitImage);
	}

	lpActor->addAuraImage(lpPacket->wSkill);

	if	(lpSkill->m_wCasterHitImage	!=	0xffff)
		lpActor->addEffect(lpSkill->m_wCasterHitImageOutputPart,lpSkill->m_wCasterHitImage);
}