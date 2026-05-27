#include "cACTOR.H"
#include "cFIELD.H"
#include "cNPC.H"
#include "debugCode.H"
#include "CGame.H"
/*
시야내아군의HP	시야 내 아군[자신 포함]의 hp	평균 hp % 이하/이상	-X /+X		
버프체크_본인	자신의 현재 스킬의 효과 여부	효과 소멸/지속 상태	FALSE/TRUE		
버프체크_타겟	타겟의 현재 스킬의 효과 여부	효과 소멸/지속 상태	FALSE/TRUE		
동료의수	범위 안에 동료의 수	이하/이상	(X,'-Y /+Y)	X가 0이면 기본 시야	
적의수	범위 안에 적의 수	이하/이상	(X,'-Y /+Y)	X가 0이면 기본 시야	
타겟주위에적의수	타겟의 A pix 내 B명 이상 있음 	이하/이상	(X,'-Y /+Y)	X가 0이면 기본 시야	
시야내아군의죽음	시야 내 아군의 죽음	유/무	TRUE/FALSE		
전투상황	전투 상황	시작/진행 중	시작/진행중		
저주로증폭가능한상태	적군에게 부정적인 효과있음	유/무	TRUE/FALSE		
타겟이펫이다	타겟이 팻이다	참/거짓	TRUE/FALSE		
발동확률	스킬 사용 확률	X%	X	(1000분률)	
현재HP	자신의 HP상태	hp % 이하/이상	-X /+X		
공격당함	공격 받을 경우 체크 하는 스킬	TRUE	TRUE		
*/

//
//	시야 안에 아군의 체력 체크
BOOL
cACTOR::checkSACS_InSightOwnerForceHP(cSkillAiPatternInfo *_lpInfo,cACTOR *_lpTarget)
{
	int		iCheckHP	=	_lpInfo->m_asValue[0];
	int		i;

	if	(m_wAiTargetCount	==	0)
		return	FALSE;

	for (i=0;i<m_wAiTargetCount;i++)
	{
		cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(m_awAiTargetList[i]);

		if	(!lpTargetActor)
		{
			m_awAiTargetList[i]	=	0xffff;

			continue;
		}

		if	(!isTargetInAttackRange(lpTargetActor,m_sSight))
		{
			m_awAiTargetList[i]	=	0xffff;
			continue;
		}

		if	(iCheckHP	<	0)
		{
			if	(lpTargetActor->getRemainHPRate()	>	-iCheckHP)	//	iCheckHP 이하인 캐릭터
				m_awAiTargetList[i]	=	0xffff;
		}
		else
		{
			if (lpTargetActor->getRemainHPRate()	<	iCheckHP)	//	iCheckHP 이상인 캐릭터
				m_awAiTargetList[i]	=	0xffff;
		}
	}

	return	orderingAI_TargetList();
}

//
//	타겟과의거리	타겟과의 거리	pix, 이하/이상	-X /+X		
BOOL
cACTOR::checkSACS_RangeToTarget(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int		iCheckRange	=	_lpInfo->m_asValue[0];
	DWORD	dwCheckRange=	iCheckRange*iCheckRange;
	int		i;

	if (m_wAiTargetCount	==	0)
		return	FALSE;

	for (i=0;i<m_wAiTargetCount;i++)
	{
		cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(m_awAiTargetList[i]);

		if (!lpTargetActor)
		{
			m_awAiTargetList[i]	=	0xffff;
			continue;
		}

		DWORD	dwCurRange	=	GetOvalRange(m_pos.x,m_pos.y,lpTargetActor->m_pos.x,lpTargetActor->m_pos.y);

		if (iCheckRange	<	0)
		{
			if (dwCurRange	>	dwCheckRange)
			{
				m_awAiTargetList[i]	=	0xffff;

				return	orderingAI_TargetList();
			}
		}
		else
		{
			if (dwCurRange	<	dwCheckRange)
			{
				m_awAiTargetList[i]	=	0xffff;

				return	orderingAI_TargetList();
			}
		}
	}

	return	TRUE;
}

BOOL
cACTOR::checkSACS_SelfBuffed(cAbility *_lpAbility,cSkillAiPatternInfo *_lpInfo,cACTOR *_lpTarget)
{
	BOOL	bCheckCondition	=	_lpInfo->m_asValue[0];

	if	(getBuffedEffectCount(_lpAbility))
	{
		if	(bCheckCondition	==	FALSE)
			return	FALSE;
	}
	else
	{
		if	(bCheckCondition	==	TRUE)
			return	FALSE;
	}

	return	TRUE;
}

BOOL
cACTOR::checkSACS_TargetBuffed(cAbility *_lpAbility,cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int		i;
	BOOL	bCheckCondition	=	_lpInfo->m_asValue[0];

	for (i=0;i<m_wAiTargetCount;i++)
	{
		cACTOR	*lpTargetActor	=	m_lpField->getActor(m_awAiTargetList[i]);

		if	(!lpTargetActor)
		{
			m_awAiTargetList[i]	=	0xffff;
			continue;
		}

		if	(lpTargetActor->getBuffedEffectCount(_lpAbility))
		{
			if	(bCheckCondition	==	FALSE)
				m_awAiTargetList[i]	=	0xffff;
		}
		else
		{
			if	(bCheckCondition	==	TRUE)
				m_awAiTargetList[i]	=	0xffff;
		}
	}

	return	orderingAI_TargetList();;
}

BOOL
cACTOR::checkSACS_OwnerForceCountInSight(cSkillAiPatternInfo *_lpInfo,cACTOR *_lpTarget)
{
	int		iCheckRange	=	_lpInfo->m_asValue[0];
	int		iCheckCount	=	_lpInfo->m_asValue[1];

	if	(iCheckRange	==	0)
		iCheckRange		=	m_sSight;

	DWORD	dwCheckRange=	iCheckRange*iCheckRange;
	int		iCount		=	m_lpField->getInSightOwnerTeamActorCount(m_pos.x,m_pos.y,this,iCheckRange,FALSE,FALSE);

	if (iCheckCount	<	0)
	{
		iCheckCount	=	-iCheckCount;

		if (iCount	<=	iCheckCount)
			return	TRUE;
	}
	else
	{
		if (iCount	>=	iCheckCount)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
cACTOR::checkSACS_EnemyForceCountInSight(cSkillAiPatternInfo *_lpInfo,cACTOR *_lpTarget)
{
	int		iCheckRange	=	_lpInfo->m_asValue[0];
	int		iCheckCount	=	_lpInfo->m_asValue[1];

	if	(iCheckRange	==	0)
		iCheckRange		=	m_sSight;

	DWORD	dwCheckRange=	iCheckRange*iCheckRange;
	int		iCount		=	m_lpField->getInSightEnemyActorCount(m_pos.x,m_pos.y,m_iLocalPart,m_wTeam,iCheckRange);

	if (iCheckCount	<	0)
	{
		iCheckCount	=	-iCheckCount;

		if (iCount	<=	iCheckCount)
			return	TRUE;
	}
	else
	{
		if (iCount	>=	iCheckCount)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
cACTOR::checkSACS_EnemyForceCountInTargetAround(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int		iCheckRange	=	_lpInfo->m_asValue[0];
	int		iCheckCount	=	_lpInfo->m_asValue[1];

	if (m_wAiTargetCount	==	0)
		return	FALSE;

	int		iTarget		=	random(m_wAiTargetCount);

	cACTOR	*lpTarget	=	m_lpField->getTestedActor(m_awAiTargetList[iTarget]);

	if (!lpTarget)
		return	FALSE;

	m_awAiTargetList[0]	=	m_awAiTargetList[iTarget];
	m_wAiTargetCount	=	1;

	int	iCount		=	m_lpField->getInSightEnemyActorCount(lpTarget->m_pos.x,lpTarget->m_pos.y,lpTarget->m_iLocalPart,m_wTeam,iCheckRange);

	if (iCheckCount	<	0)
	{
		iCheckCount	=	-iCheckCount;

		if (iCount	>	iCheckCount)
			return	FALSE;
	}
	else
	{
		if (iCount	<	iCheckCount)
			return	FALSE;
	}

	return	TRUE;
}

BOOL
cACTOR::checkSACS_DeathOwnerForceInSight(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int		iCheckStatus	=	_lpInfo->m_asValue[0];
	int		iCheckRange		=	m_sSight;

	DWORD	dwCheckRange	=	iCheckRange*iCheckRange;
	int		i;
	BOOL	bExistLiveTarget=	FALSE;

	if	(m_wAiTargetCount	==	0)
		return	FALSE;

	for (i=0;i<m_wAiTargetCount;i++)
	{
		cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(m_awAiTargetList[i]);

		if (!lpTargetActor || !lpTargetActor->isDeath())
		{
			m_awAiTargetList[i]	=	0xffff;
			bExistLiveTarget	=	TRUE;
			continue;
		}
	}

	if	(bExistLiveTarget)
		orderingAI_TargetList();

	if	(iCheckStatus	==	TRUE)
	{
		if (m_wAiTargetCount)
			return	TRUE;

		return	FALSE;
	}

	if	(m_wAiTargetCount)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::checkSACS_OnBattle(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget,BOOL _bIsFirst)
{
	int		iCheckStatus	=	_lpInfo->m_asValue[0];

//	0	이면 전투중
//	1	이면 전투중이 아님

	if	(_bIsFirst)
	{
		if (iCheckStatus)
			return	TRUE;

		return	FALSE;
	}

	if (iCheckStatus)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::checkSACS_EnableCurseAmplify(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int		i;
	int		iCheckStatus	=	_lpInfo->m_asValue[0];

	if (m_wAiTargetCount	==	0)
		return	FALSE;

	for (i=0;i<m_wAiTargetCount;i++)
	{
		cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(m_awAiTargetList[i]);

		if (!lpTargetActor || !lpTargetActor->isWrongStatus())
		{
			m_awAiTargetList[i]	=	0xffff;
			continue;
		}
	}

	orderingAI_TargetList();

	if (iCheckStatus	==	TRUE)
	{
		if (m_wAiTargetCount)
			return	TRUE;

		return	FALSE;
	}

	if (m_wAiTargetCount)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::checkSACS_TargetIsPet(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int		i;
	int		iCheckStatus	=	_lpInfo->m_asValue[0];

	if (m_wAiTargetCount	==	0)
		return	FALSE;

	for (i=0;i<m_wAiTargetCount;i++)
	{
		cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(m_awAiTargetList[i]);

		if (!lpTargetActor || (!lpTargetActor->isPet() && !lpTargetActor->isSummonBeast()))
		{
			m_awAiTargetList[i]	=	0xffff;
			continue;
		}
	}

	orderingAI_TargetList();

	if (iCheckStatus	==	TRUE)
	{
		if (m_wAiTargetCount)
			return	TRUE;

		return	FALSE;
	}

	if (m_wAiTargetCount)
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::checkSACS_ActiveChance(cAbility *_lpAbility,cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int	iChance	=	_lpInfo->m_asValue[0];
	int	aCorrectChanceBySkillGrade[6]	=	{	10,10,1000	,1300	,1800	,2500	};
	int	iCorrectChanceBySkillGrade		=	aCorrectChanceBySkillGrade[_lpAbility->getSkill()->m_wDifficultyLevel];

	if	(m_wIsBoss	==	FALSE)
		iChance							=	iChance	+ (getWisdom()*100/iCorrectChanceBySkillGrade);
	
	if	(m_wSimpletonTime)
		iChance	/=	2;
	
//	스킬 발동 확률 = 설정된 스킬의 발동확률 + (몬스터의 지혜/인수)

	int	iRandValue						=	random(10000);

	if	(g_iOutputStatus	==	1)
		printf("skill active chance %d.%.2d[check chance %d.%.2d]\n",iChance/100,iChance%100,iRandValue/100,iRandValue%100);

	if	(iRandValue < iChance)
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::checkSACS_CurrentHP(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int	iCheckHP	=	_lpInfo->m_asValue[0];
	int	iCurrentHP	=	getRemainHPRate();

	if	(g_iOutputStatus	==	1)
		printf("check Current HP %s %d/%d\n",m_strName,iCurrentHP,iCheckHP);

	if	(iCheckHP	<	0)
	{
		iCheckHP	=	-iCheckHP;

		if (iCurrentHP	>	iCheckHP)
			return	FALSE;
	}
	else
	{
		if (iCurrentHP	<	iCheckHP)
			return	FALSE;
	}

	return	TRUE;
}

BOOL
cACTOR::checkSACS_Attacked(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	return	TRUE;
}

BOOL
cACTOR::checkSACS_CheckSwallowCorpesCount(cSkillAiPatternInfo	*_lpInfo)
{
	if	(_lpInfo->m_asValue[0]	>	0)
	{
		if	(m_wSwallowCorpesCount	>=	_lpInfo->m_asValue[0])
			return	TRUE;
	}
	else
	{
		if	(m_wSwallowCorpesCount	<=	-_lpInfo->m_asValue[0])
			return	TRUE;
	}
	
	return	FALSE;
}

BOOL
cACTOR::checkSACS_CheckTrapCount(cSkillAiPatternInfo	*_lpInfo)
{
	int	iExistTrapCount	=	m_lpField->getExistTrapCount(this,_lpInfo->m_asValue[0]);

	if	(iExistTrapCount	>=	_lpInfo->m_asValue[1])
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::checkSACS_CheckStatus(cSkillAiPatternInfo	*_lpInfo,cACTOR *_lpTarget)
{
	int	_iCheckStatus	=	_lpInfo->m_asValue[0],i;

	if (m_wAiTargetCount	==	0)
		return	FALSE;

	for (i=0;i<m_wAiTargetCount;i++)
	{
		cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(m_awAiTargetList[i]);

		if (!lpTargetActor)
		{
			m_awAiTargetList[i]	=	0xffff;
			continue;
		}

		switch(_iCheckStatus)
		{
			case	eSTATUS_INVISIBLE	:
				if (lpTargetActor->m_sInvisivilityTime	==	0)
					m_awAiTargetList[i]	=	0xffff;
				break;
		}
	}

	orderingAI_TargetList();

	if (m_wAiTargetCount)
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::checkSACS_AnimatePartnerCount()
{
	int	i;

	if	(m_wAiTargetCount	==	0)
		return	FALSE;

	BOOL	bIsCutTarget	=	FALSE;

	for (i=0;i<m_wAiTargetCount;i++)
	{
		cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(m_awAiTargetList[i]);

		if	(!lpTargetActor	||	lpTargetActor->m_bf2AnimatePartnerCount	>=	dMONSTER_ANIMATE_PARTNER_LIMIT_COUNT)
		{
			bIsCutTarget		=	TRUE;
			m_awAiTargetList[i]	=	0xffff;
			continue;
		}
	}

	if	(bIsCutTarget)
		orderingAI_TargetList();

	if	(m_wAiTargetCount)
		return	TRUE;

	return	FALSE;
}

//
//	교전 스킬 찾기
BOOL
cACTOR::findEngageAbility(cACTOR *_lpTarget,BOOL _bIsFirst)
{
	if	(isPet())
		_bIsFirst	=	_bIsFirst;

	if	(g_bIsTestServer)
		sortAbility();

	g_bIsChansTestServer=	g_bIsChansTestServer;

	if	(isCopyActor() && m_wJob < dPLACENAME_COUNT)
	{
		for (int i=0;i<dABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&m_aAbility[i];
			CSkill		*lpSkill	=	m_aAbility[i].getSkill();

			if	(lpSkill	==	NULL)
				continue;

			if	(!lpAbility->isEnableJob(m_wJob)	||	!lpSkill->isAgressiveSkill())
				continue;

			if	(lpAbility->getSpentCP())
				continue;

			if	(lpSkill->m_wDifficultyLevel	>	1)
				continue;

			setCurrentTarget(_lpTarget);
			setEngageAbility(lpAbility);

			return	TRUE;
		}

		return	FALSE;
	}

	BOOL	bIsUseDeaultSkill	=	FALSE;

	if	(m_wIsZinMonster	==	FALSE)
		bIsUseDeaultSkill	=	TRUE;

	if	(isPet()	&&	m_wUseSpecialAbilityTime)
	{
		if	(m_wUseSpecialAbilityStep	==	ePUSS_NONE)
			bIsUseDeaultSkill	=	FALSE;
		else
		{
			setEngageAbility(&m_specialAbility);
			return	TRUE;
		}
	}

//	if	(_lpTarget	&&	_lpTarget->m_wSerialInField	==	m_wSerialInField)
//		bIsUseDeaultSkill	=	TRUE;

	if(m_bEngageReactionAbility)		// 09.08.13
	{
		m_bEngageReactionAbility = FALSE;
		return TRUE;
	}

	if	(bIsUseDeaultSkill)
	{
		setCurrentTarget(_lpTarget);
		setEngageAbility(&m_aAbility[m_wDefaultSkillSlot]);

		return	TRUE;
	}

	int	i;

	if	(m_wBerserkTime	==	0)
		for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
		{
			CSkill	*lpSkill	=	m_aAbility[i].getSkill();

			if	(lpSkill	==	NULL)
				break;

			if	(lpSkill->isPassiveSkill())
				continue;

			if	(g_iOutputStatus	==	1)
				printf("skill check %s : 우선순위 %d\n",lpSkill->m_strName,lpSkill->getOrderingInSkillAi());

			if	(checkIsSuitableSkill(&m_aAbility[i],_lpTarget,_bIsFirst))
			{
				int	iTarget	=	random(m_wAiTargetCount);
				iTarget		=	m_awAiTargetList[iTarget];

				setCurrentTarget(m_lpField->getActor(iTarget));
				setEngageAbility(&m_aAbility[i]);

				return	TRUE;
			}
		}

	setCurrentTarget(_lpTarget);
	setEngageAbility(&m_aAbility[m_wDefaultSkillSlot]);

	return	TRUE;
}

void
cACTOR::setMonsterReaction(BOOL _bIsOn)
{
	if	(_bIsOn	==	FALSE)
	{
		memset(m_aActiveReaction,0xff,sizeof(m_aActiveReaction));

		return;
	}

	int	iCount				=	0;

	for (int	i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill)
			continue;

		if	(lpSkill->isPassiveSkill())
		{
			m_aAuraSkillTimer[iCount]	=	0;
			m_aActiveReaction[iCount]	=	i;

			iCount++;

			if	(iCount	>=	dACTIVE_REACTION_COUNT)
				break;
		}
	}
	
	operateInstanceApplyAura();
}

//
//	교전 스킬 찾기
void
cACTOR::sortAbility()
{
	if	(!isPet())
	{
		if	(!isPureMonster())
			return;

		if	(m_wIsZinMonster	==	FALSE)
			return;
	}

	cAbility	defaultSkill;

	defaultSkill.copy(&m_aAbility[m_wDefaultSkillSlot]);

	int			i,iOrder,iCount=0;
	cAbility	aSortSkill[dMAX_MONSTER_ABILITY_COUNT];

	memset(aSortSkill,0xff,sizeof(aSortSkill));

	for (iOrder=100;iOrder>=0;iOrder--)
	{
		for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
		{
			cAbility	*lpAbility	=	&m_aAbility[i];
			CSkill		*lpSkill	=	lpAbility->getSkill();

			if	(lpSkill && iOrder	==	lpSkill->getOrderingInSkillAi())
				memcpy(&aSortSkill[iCount++],lpAbility,sizeof(cAbility));
		}
	}

	memcpy(m_aAbility,aSortSkill,sizeof(aSortSkill));

	iCount				=	0;
	m_wDefaultSkillSlot	=	0;

	for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];
		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill)
			continue;

		if	(lpAbility->m_wSkill==	defaultSkill.m_wSkill	&&	lpAbility->m_wLevel	==	defaultSkill.m_wLevel)
		{
			m_wDefaultSkillSlot	=	i;
			break;
		}
	}

	if	(isPet() && m_wUseSpecialAbilityTime	==	0)
		setMonsterReaction(FALSE);
	else
		setMonsterReaction(TRUE);
}

void
cACTOR::resetAiTargetList()
{
	m_wAiTargetCount	=	0;
	memset(m_awAiTargetList,0xff,sizeof(m_awAiTargetList));
}

BOOL
cACTOR::addAiTargetList(int _iSerial)
{
	if	(m_wAiTargetCount	>=	dMAX_AI_TARGET_COUNT)
		return	FALSE;

	for	(int i=0;i<m_wAiTargetCount;i++)
	{
		if (m_awAiTargetList[i]	==	_iSerial)
			return	TRUE;
	}

	m_awAiTargetList[m_wAiTargetCount++]	=	_iSerial;

	return	TRUE;
}

BOOL
cACTOR::orderingAI_TargetList()
{
	int		i,iTargetCount=0;

	WORD	awAiTargetList[dMAX_AI_TARGET_COUNT];

	memset(awAiTargetList,0xff,sizeof(awAiTargetList));

	for (i=0;i<m_wAiTargetCount;i++)
	{
		if	(m_awAiTargetList[i]	==	0xffff)
			continue;

		awAiTargetList[iTargetCount++]	=	m_awAiTargetList[i];
	}

	m_wAiTargetCount	=	iTargetCount;

	memcpy(m_awAiTargetList,awAiTargetList,sizeof(awAiTargetList));

	if	(m_wAiTargetCount)
		return	TRUE;

	return	FALSE;
}

void
cACTOR::findAiTargetList(cAbility *_lpAbility,cACTOR *_lpTarget)
{
	int	iSkillRange	=	_lpAbility->getSkillRangeForAI();

	resetAiTargetList();

	WORD	awOwnForceList[100];
	int		iOwnForceCount	=	0;
	CSkill	*lpSkill		=	_lpAbility->getSkill();
	int		iAI_Target		=	lpSkill->m_wAI_Target;

	switch(iAI_Target)
	{
		case	eSAT_OWN_FORCE			:
		{
			if	(isPet())
			{
				iOwnForceCount	=	m_lpField->getPetForceList(this,awOwnForceList,100);

				for (int i=0;i<iOwnForceCount;i++)
				{
					cACTOR	*lpMember	=	m_lpField->getActor(awOwnForceList[i]);

					if	(!lpMember	||	lpMember->isDeath())
						continue;

					if	(!isTargetInAttackRange(lpMember,iSkillRange))
						continue;

					addAiTargetList(lpMember->m_wSerialInField);
				}

				break;
			}

			addAiTargetList(m_wSerialInField);
			
			cNPC	*lpNpc		=	getNpc();

			if	(!lpNpc)
				break;

			for (int i=0;i<dMAX_LINK_MONSTER_COUNT;i++)
			{
				int	iNpc	=	lpNpc->m_awLinkMonster[i];

				if	(iNpc	>=	dMAX_LINK_MONSTER_COUNT)
					break;

				cNPC	*lpTargetNpc	=	m_lpField->m_apNpcGenerateData[iNpc];

				if	(!lpTargetNpc || lpTargetNpc->m_wActorSerial	==	0xffff)
					continue;

				cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(lpTargetNpc->m_wActorSerial);

				if	(!lpTargetActor	||	lpTargetActor->isDeath())
					continue;

				if	(!isTargetInAttackRange(lpTargetActor,iSkillRange))
					continue;

				addAiTargetList(lpTargetActor->m_wSerialInField);
			}
			break;
		}

		case	eSAT_ENEMY				:
			if	(_lpTarget	&&	!_lpTarget->isDeath())
				addAiTargetList(_lpTarget->m_wSerialInField);
			break;

		case	eSAT_SELF				:
			addAiTargetList(m_wSerialInField);
			break;

		case	eSAT_OWN_FORCE_CORPSE	:
		{
			if	(isPet())
			{
				iOwnForceCount	=	m_lpField->getPetForceList(this,awOwnForceList,100);

				for (int i=0;i<iOwnForceCount;i++)
				{
					cACTOR	*lpMember	=	m_lpField->getActor(awOwnForceList[i]);

					if	(!lpMember	||	!lpMember->isDeath())
						continue;

					if	(!isTargetInAttackRange(lpMember,iSkillRange))
						continue;

					addAiTargetList(lpMember->m_wSerialInField);
				}

				break;
			}

			cNPC	*lpNpc		=	getNpc();

			if	(!lpNpc)
				break;

			for	(int i=0;i<dMAX_LINK_MONSTER_COUNT;i++)
			{
				int	iNpc	=	lpNpc->m_awLinkMonster[i];

				if (iNpc	>=	dMAX_LINK_MONSTER_COUNT)
					break;

				cNPC	*lpTargetNpc	=	m_lpField->m_apNpcGenerateData[iNpc];

				if	(!lpTargetNpc || lpTargetNpc->m_wActorSerial	==	0xffff)
					continue;

				cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(lpTargetNpc->m_wActorSerial);

				if	(!lpTargetActor || !lpTargetActor->isDeath())
					continue;

				if	(!isTargetInAttackRange(lpTargetActor,iSkillRange))
					continue;

				addAiTargetList(lpTargetActor->m_wSerialInField);
			}
			break;
		}

		case	eSAT_OWN_FORCE_SAME_ORDER_JOB_CORPSE	:
		{
			if	(isPet())
			{
				iOwnForceCount	=	m_lpField->getPetForceList(this,awOwnForceList,100);

				for (int i=0;i<iOwnForceCount;i++)
				{
					cACTOR	*lpMember	=	m_lpField->getActor(awOwnForceList[i]);

					if	(!lpMember	||	!lpMember->isDeath())
						continue;

					if	(lpMember->m_wJob	!=	m_wJob)
						continue;

					if	(!isTargetInAttackRange(lpMember,iSkillRange))
						continue;

					addAiTargetList(lpMember->m_wSerialInField);
				}

				break;
			}

			cNPC	*lpNpc		=	getNpc();

			if	(!lpNpc)
				break;

			for	(int i=0;i<dMAX_LINK_MONSTER_COUNT;i++)
			{
				int	iNpc	=	lpNpc->m_awLinkMonster[i];

				if (iNpc	>=	dMAX_LINK_MONSTER_COUNT)
					break;

				cNPC	*lpTargetNpc	=	m_lpField->m_apNpcGenerateData[iNpc];

				if	(!lpTargetNpc || lpTargetNpc->m_wActorSerial	==	0xffff)
					continue;

				cACTOR	*lpTargetActor	=	m_lpField->getTestedActor(lpTargetNpc->m_wActorSerial);

				if	(!lpTargetActor || !lpTargetActor->isDeath())
					continue;

				if	(lpTargetActor->m_wJob	!=	m_wJob)
					continue;

				if	(!isTargetInAttackRange(lpTargetActor,iSkillRange))
					continue;

				addAiTargetList(lpTargetActor->m_wSerialInField);
			}
			break;
		}
	}

	if	(m_wAiTargetCount	&&	lpSkill->m_aValue[dSKILL_SPARE_VALUE_COUNT-1]	==	12345)	//	티아메쓰다. 직업 체크 후 액션 결정
	{
		int iActor;

		for (iActor=0;iActor<m_wAiTargetCount;iActor++)
		{
			cACTOR	*lpTarget	=	m_lpField->getActor(m_awAiTargetList[iActor]);

			if	(!lpTarget)
				continue;

			int		iAngle	=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y,FALSE);
			BOOL	bIsOK	=	FALSE;

			for	(int i=0;i<4;i++)
			{
				int	iSlot	=	i*4;

				cJOB	*lpJob	=	getJob();

				if	(!lpJob	||	lpJob->m_wBody	!=	lpSkill->m_aValue[iSlot])
					continue;

				if	(lpSkill->m_aValue[iSlot]	==	-1)
					break;

				int	iAngle1	=	lpSkill->m_aValue[iSlot+1];
				int	iAngle2	=	lpSkill->m_aValue[iSlot+2];

				if	(iAngle2	<	iAngle1)
				{
					if	(iAngle	<	iAngle1	&&	iAngle	>	iAngle2)
						continue;
				}
				else
				{
					if	(iAngle	<	iAngle1	||	iAngle	>	iAngle2)
						continue;
				}

				bIsOK	=	TRUE;

				break;
			}

			if	(bIsOK	==	FALSE)
				m_awAiTargetList[iActor]	=	0xffff;
		}

		int	iTargetCount	=	m_wAiTargetCount;

		for (iActor=0;iActor<m_wAiTargetCount;iActor++)
		{
			if	(m_awAiTargetList[iActor]	!=	0xffff)
				continue;

			for	(int i=iActor;i<m_wAiTargetCount-1;i++)
				m_awAiTargetList[iActor]	=	m_awAiTargetList[iActor+1];

			m_awAiTargetList[m_wAiTargetCount-1]	=	0xffff;

			m_wAiTargetCount--;
		}
	}
}

void
cACTOR::operateMonsterCounterSkill(cACTOR *_lpAttacker)
{
	for	(int i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if	(lpSkill	==	NULL)
			break;

		if	(lpSkill->m_bf1IsMonsterCounter)
		{
			if	(checkIsSuitableSkill(&m_aAbility[i],_lpAttacker,FALSE))
			{

			}
			return;
		}
	}
}

BOOL
cACTOR::checkIsSuitableSkill(cAbility *_lpAbility,cACTOR *_lpTarget,BOOL _bIsFirst)
{
	findAiTargetList(_lpAbility,_lpTarget);

	if	(m_wAiTargetCount	==	0)
		return	FALSE;

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	int		i;

	for (i=0;i<dMAX_SKILL_AI_PATTERN;i++)
	{
		cSkillAiPatternInfo	*lpInfo	=	&lpSkill->m_aAi[i];

		switch(lpInfo->m_wTrigger)
		{
			case	0xffff	:
				return	TRUE;

			case	eSACS_IN_SIGHT_OWNER_FORCE_HP		:	//	시야내아군의HP
			{
				if	(checkSACS_InSightOwnerForceHP(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_RANGE_TO_TARGET				:	//	타겟과의거리
			{
				if	(checkSACS_RangeToTarget(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_BUFF_ON_OWNER					:	//	자신에게버프됨
			{
				if	(checkSACS_SelfBuffed(_lpAbility,lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_BUFF_ON_TARGET				:	//	타겟에게버프됨
			{
				if (checkSACS_TargetBuffed(_lpAbility,lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_OWNER_FORCE_COUNT_IN_SIGHT	:	//	시야내동료의수
			{
				if (checkSACS_OwnerForceCountInSight(lpInfo,this)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_ENEMY_COUNT_IN_SIGHT			:	//	시야내적의수
			{
				if (checkSACS_EnemyForceCountInSight(lpInfo,this)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_ENEMY_COUNT_IN_TARGET_AROUND	:	//	타겟주위에적의수
			{
				if (checkSACS_EnemyForceCountInTargetAround(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_DEATH_OWNER_FORCE_IN_SIGHT	:	//	시야내아군의죽음
			{
				if	(checkSACS_DeathOwnerForceInSight(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_ON_BATTLE						:	//	전투상황
			{
				if	(checkSACS_OnBattle(lpInfo,_lpTarget,_bIsFirst)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_ENABLE_CURSE_AMPLIFY			:	//	저주로증폭가능
			{
				if	(checkSACS_EnableCurseAmplify(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_TARGET_IS_PET					:	//	타겟이펫이다
			{
				if	(checkSACS_TargetIsPet(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_ACTIVE_CHANCE					:	//	발동확률
			{
				if	(checkSACS_ActiveChance(_lpAbility,lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_CURRENT_HP					:	//	현재HP
			{
				if	(checkSACS_CurrentHP(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_ATTACKED						:	//	공격당함
			{
				if	(checkSACS_Attacked(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_CHECK_STATUS					:	//	상태체크
			{
				if	(checkSACS_CheckTrapCount(lpInfo)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_CHECK_SWALLOW_CORPES_COUNT	:	//	삼킨시체수체크
			{
				if	(checkSACS_CheckSwallowCorpesCount(lpInfo)	==	FALSE)
					return	FALSE;
				break;
			}

			case	eSACS_CHECK_TRAP_COUNT				:	//	함정개수체크
			{
				if	(checkSACS_CheckStatus(lpInfo,_lpTarget)	==	FALSE)
					return	FALSE;
				break;
			}
			case	eSACS_CHECK_ANIMATE_PARTNER_COUNT	:
			{
				if	(checkSACS_AnimatePartnerCount()	==	FALSE)
					return	FALSE;
				break;
			}
		}
	}

	return	TRUE;
}