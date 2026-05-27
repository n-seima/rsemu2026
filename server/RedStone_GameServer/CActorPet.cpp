#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CGuild.H"
#include "actor_mini_pet.H"


const	int	c_iObitianActionSpeed		=	4000;
const	int	c_iDashBladeActionSpeed		=	2000;
const	int	c_iAstroBowActionSpeed		=	3000;
const	int	c_iCristalWaterActionSpeed	=	5000;

const	int	c_iGuildPetDashBladeAttackSkill	=	0;
const	int	c_iGuildPetAstroBowAttackSkill	=	200;

void
cACTOR::operateGuildPetWork()
{
	int		iIncreasePetSight	=	0;
	int		iIncreaseSpeed		=	0;
	int		iAttackSpeed;
	CGuild	*lpGuild			=	getGuild();

	if	(lpGuild)
	{
		iIncreasePetSight		+=	lpGuild->m_wIncreaeGuildPetSight;
		iIncreaseSpeed			+=	lpGuild->m_wIncreaseGuildPetAttackSpeed;
	}

	if	(m_wGuildPetType	==	eGUILD_PET_OBITIAN)
	{
//	불 5+12Lv±3Lv
//	폭발 범위	120
//	공격속도	4
		iAttackSpeed		=	c_iObitianActionSpeed;
		iAttackSpeed		-=	iIncreaseSpeed*100;

		if	(m_dwLastActionTimeForGuildPet+iAttackSpeed	<	g_dwCurrentTime)
		{
//	30Lv	60Lv	80Lv	100Lv
//	폭발 범위 +10	폭발 범위 +25	폭발 범위 +40	폭발 범위 +64
			int	iRange		=	120+iIncreasePetSight;
			int	iRandDamage	=	random(300*m_wGuildPetLevel*2)-300*m_wGuildPetLevel;
			int	iDamage		=	500+1200*m_wGuildPetLevel+iRandDamage;

			if	(m_wGuildPetLevel	>=	100)
				iRange	+=	64;
			else
			if	(m_wGuildPetLevel	>=	80)
				iRange	+=	40;
			else
			if	(m_wGuildPetLevel	>=	60)
				iRange	+=	25;
			else
			if	(m_wGuildPetLevel	>=	30)
				iRange	+=	10;

			m_dwLastActionTimeForGuildPet	=	g_dwCurrentTime;

			cAbility	ability;

			ability.set(g_sm.m_iObitianSkill,1);

			m_lpField->strikeWideAreaMagicDamage(this,-1,-1,FALSE,&ability,iRange,iDamage,0,0,0,0,0);
		}
	}
	else
	if	(m_wGuildPetType	==	eGUILD_PET_DASH_BLADE)
	{
//	물리 7+9Lv±1Lv
//	사정거리	150
//	공격속도	2
//	명중률 75%

		if	(m_lpTarget	&&	!isOwnForce(m_lpTarget))
		{
			iAttackSpeed		=	c_iObitianActionSpeed;
			iAttackSpeed		-=	iIncreaseSpeed*100;

			if	(m_dwLastActionTimeForGuildPet+iAttackSpeed	<	g_dwCurrentTime)
			{
				int	iRange		=	150+iIncreasePetSight;
				int	iRandDamage	=	random(100*m_wGuildPetLevel*2)-100*m_wGuildPetLevel;
				int	iDamage		=	700+900*m_wGuildPetLevel+iRandDamage;
				int	iHitChance	=	75;

				if	(lpGuild)
					iDamage		+=	iDamage*lpGuild->m_wIncreaseDashBladeDamage/100;

				m_dwLastActionTimeForGuildPet	=	g_dwCurrentTime;

				int	iIgnoreDefensePowerChance	=	0;

				if	(m_wGuildPetLevel	>=	100)	//	방어력 무시 25%
					iIgnoreDefensePowerChance	=	25;
				else
				if	(m_wGuildPetLevel	>=	60)		//	방어력 무시 10%
					iIgnoreDefensePowerChance	=	15;

				if	(m_wGuildPetLevel	>=	80)		//	명중률 25% 증가
					iHitChance	+=	25;
				else
				if	(m_wGuildPetLevel	>=	30)		//	명중률 10% 증가
					iHitChance	+=	10;

				CHitInfo	hitInfo;

				hitInfo.reset(TRUE);

				if	(random(1000)	<	iHitChance)
				{
					hitInfo.setHit();

					int	iAttackPower	=	iDamage;
					int	iDefensivePower	=	m_lpTarget->getDefensivePower();

					if	(random(100)	<	iIgnoreDefensePowerChance)
						iDefensivePower	=	0;

					LONGLONG	llDamage		=	iAttackPower;
					LONGLONG	llDivideDamage	=	max(llDamage+iDefensivePower,1);
					int			iDamage			=	(int)(llDamage*llDamage/llDivideDamage);
					
					hitInfo.m_dwPhysicalDamage	=	iDamage;
					hitInfo.setPhysicalDamage();

					m_lpTarget->reduceHP(iDamage,this);
					m_lpTarget->addSendHitDamage(this,iDamage,0,hitInfo.m_dwResultField,g_sm.m_iDashBladeSkill);
				}
			}
		}
	}
	else
	if	(m_wGuildPetType	==	eGUILD_PET_ASTRO_BOW)
	{
		if	(m_lpTarget	&&	!isOwnForce(m_lpTarget))
		{
			int	iActionSpeed	=	c_iAstroBowActionSpeed;
			iActionSpeed		-=	iIncreaseSpeed*100;

			if	(m_wGuildPetLevel	>=	80)		//	공격속도 15% 증가
				iActionSpeed	-=	(iActionSpeed*15/100);
			else
			if	(m_wGuildPetLevel	>=	30)		//	공격속도 5% 증가
				iActionSpeed	-=	(iActionSpeed*5/100);

			if	(m_dwLastActionTimeForGuildPet+iActionSpeed	<	g_dwCurrentTime)
			{
				int	iRange		=	300+iIncreasePetSight;
				int	iRandDamage	=	random(300*m_wGuildPetLevel*2)-300*m_wGuildPetLevel;
				int	iDamage		=	300+700*m_wGuildPetLevel+iRandDamage;
				int	iHitChance	=	75;

				if	(lpGuild)
					iDamage		+=	iDamage*lpGuild->m_wIncreaseAstroBowDamage/100;

				m_dwLastActionTimeForGuildPet	=	g_dwCurrentTime;

				int	iIgnoreDefensePowerChance	=	0;

				if	(m_wGuildPetLevel	>=	100)	//	방어력 무시 25%
					iHitChance			+=	15;
				else
				if	(m_wGuildPetLevel	>=	60)		//	방어력 무시 10%
					iHitChance			+=	5;

				CHitInfo	hitInfo;

				hitInfo.reset(TRUE);

				if	(random(1000)	<	iHitChance)
				{
					hitInfo.setHit();

					int	iAttackPower	=	iDamage;
					int	iDefensivePower	=	m_lpTarget->getDefensivePower();

					LONGLONG	llDamage		=	iAttackPower;
					LONGLONG	llDivideDamage	=	max(llDamage+iDefensivePower,1);
					int			iDamage			=	(int)(llDamage*llDamage/llDivideDamage);
					
					hitInfo.m_dwPhysicalDamage	=	iDamage;
					hitInfo.setPhysicalDamage();

					m_lpTarget->reduceHP(iDamage,this);
					m_lpTarget->addSendHitDamage(this,iDamage,0,hitInfo.m_dwResultField,g_sm.m_iAstroBowSkill);
				}
			}
		}
//	명중률 75%
	}
	else
	if	(m_wGuildPetType	==	eGUILD_PET_CRISTAL_WATER)
	{
		int	iActionSpeed	=	c_iCristalWaterActionSpeed;
		iActionSpeed		-=	iIncreaseSpeed*100;

		if	(m_wGuildPetLevel	>=	100)		//	공격속도 10% 증가
			iActionSpeed	-=	(iActionSpeed*10/100);

		if	(m_dwLastActionTimeForGuildPet+iActionSpeed	<	g_dwCurrentTime)
		{
//	30Lv	60Lv	80Lv	100Lv
//	폭발 범위 +10	폭발 범위 +25	폭발 범위 +40	폭발 범위 +64
			int	iRegenHP	=	100+20*m_wGuildPetLevel;

			cAbility	ability;

			ability.set(122,1);	//	사제의 기도 스킬 타격 이펙트를 쓰자
			
			if	(m_wGuildPetLevel	>=	80)
				if	(isCurseStatus())
					if	(random(10)	==	0)
					{
						cureCurseStatus();
						addSendSkillEffect(&ability,eSE_HIT);
					}

			if	(m_wGuildPetLevel	>=	60)
				if	(isStrangeStatus())
					if	(random(10)	==	0)
					{
						cureStrangeStatus();
						addSendSkillEffect(&ability,eSE_HIT);
					}
			if	(m_wGuildPetLevel	>=	30)
				if	(isDeclinePowerStatus())
					if	(random(10)	==	0)
					{
						cureDeclinePowerStatus();
						addSendSkillEffect(&ability,eSE_HIT);
					}

			m_dwLastActionTimeForGuildPet	=	g_dwCurrentTime;

			if	(m_iHP	!=	getMaxHP())
			{
				if	(instanceHealHP(iRegenHP))
				{
					SG_REGEN_HP_BY_AID_SKILL	packet;

					packet.base.set(sizeof(SG_REGEN_HP_BY_AID_SKILL),dSG_REGEN_HP_BY_AID_SKILL);

					packet.wTarget	=	m_wSerialInField;
					
					LONGLONG	llHP=	m_iHP;
					llHP			=	llHP*10000/getMaxHP();
					packet.wRemainHP=	(WORD)llHP;
					packet.wRegenHP	=	iRegenHP/100;

					m_lpField->addSendPacket((ALL_MSG *)&packet,m_wSerialInField);
				}
			}
		}
	}
}

int
cACTOR::addPet(int _iJob,int _iTamedLevel,int _iTamedLoyaloty,cACTOR *_lpTarget)
{
	cPetDefine	petInfo;

	petInfo.reset();

	petInfo.setJobIndex(_iJob);

	petInfo.m_bf10Level		=	_iTamedLevel;
	petInfo.m_bf14Loyalty	=	_iTamedLoyaloty*10;
	petInfo.m_bf22CurrentHP	=	_lpTarget->getMaxHP()/100;
	petInfo.m_bf2TamerSlot	=	m_wAvatarIndex;

	strcpy(petInfo.m_strName,"noname");

	int	iPetIndex	=	addPet(&petInfo);

	if	(iPetIndex	==	0xffff)
		return	0xffff;

	m_awTamedMonsterList[iPetIndex]	=	_lpTarget->m_wSerialInField;

	return	iPetIndex;
}

void
cACTOR::revisePetJobIndex()
{
	int	i;

	for(i=0;i<dOWN_PET_COUNT;i++)
		revisePetInfo(&m_aPet[i]);

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		if	(m_aItems[i].isBreedingRecordBook())
		{
			cPetDefine	*lpPetInfo	=	(cPetDefine	*)&m_aItems[i].m_bCount;

			revisePetInfo(lpPetInfo);
		}
	}
	for (i=0;i<dEXTRA_INVENTORY_SIZE;i++)
	{
		if	(m_aExtraInventory[i].isBreedingRecordBook())
		{
			cPetDefine	*lpPetInfo	=	(cPetDefine	*)&m_aExtraInventory[i].m_bCount;

			revisePetInfo(lpPetInfo);
		}
	}
}

void
cACTOR::revisePetInfo(cPetDefine *_lpPetInfo)
{
	if	(_lpPetInfo->m_bf10Level	==	0)
		return;

	int	iJobIndex	=	_lpPetInfo->getJobIndex();

	if	(iJobIndex	>=	dMAX_JOB_COUNT	||	g_aJob[iJobIndex].m_iSerial	==	0xffff	||	g_aJob[iJobIndex].m_bf1IsEnableTameMonster	==	FALSE	||	
		g_aJob[iJobIndex].m_wMonsterLevel	>=	cJOB::eML_SEMI_BOSS1)
		_lpPetInfo->m_bf1Job	=	1-_lpPetInfo->m_bf1Job;
}

int
cACTOR::addPet(cPetDefine *_lpPetInfo)
{
	int			i,iPetIndex;
	cPetDefine	*lpPet	=	NULL;

	revisePetInfo(_lpPetInfo);

	for (i=0;i<dOWN_PET_COUNT;i++)
	{
		if	(m_aPet[i].m_bf10Level	==	0)
		{
			lpPet		=	&m_aPet[i];
			iPetIndex	=	i;

			break;
		}
	}

	if	(!lpPet)
		return	0xffff;

	memcpy(lpPet,_lpPetInfo,sizeof(cPetDefine));

	return	iPetIndex;
}

void
cACTOR::removePet(int _iPetIndex)
{
	m_aPet[_iPetIndex].reset();
	m_awTamedMonsterList[_iPetIndex]	=	0xffff;
}

void
cACTOR::buildAddPetPower()
{	// 마지막 수정일 : 09.10.22
	cACTOR	*lpTamer	=	getTamer();

	if	(!isPet() || !lpTamer)
		return;

	int	i;
	m_engageAbility.reset();

	cJOB	*lpJob		=	getJob();

	for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		m_aAbility[i].m_wSkill	=	lpJob->m_aAbility[i].m_wSkill;

		if	(m_aAbility[i].m_wSkill	==	0xffff)
		{
			m_aAbility[i].m_wLevel	=	0;
			continue;
		}

		if (lpJob->m_aLevelForAddSkillLevel[i]	>	0)
			m_aAbility[i].m_wLevel	=	lpJob->m_aAbility[i].m_wLevel+m_iLevel*100/lpJob->m_aLevelForAddSkillLevel[i];
		else
			m_aAbility[i].m_wLevel	=	1;

		m_aAbility[i].m_wLevel	=	max(m_aAbility[i].m_wLevel,1);
		m_aAbility[i].m_wLevel	=	min(m_aAbility[i].m_wLevel,c_aMonsterSkillLevelLimit[lpJob->m_wMonsterLevel]);

		m_aAbility[i].m_wLevel	+=	(WORD)pow(lpTamer->getIntelligence()/3,0.5f);
		m_aAbility[i].m_wLevel	=	min(m_aAbility[i].m_wLevel,100);
	}

	m_wDefaultSkillSlot	=	0;

	sortAbility();
//펫 스킬 레벨 : 펫의 각 스킬 레벨 = 원 몬스터일 때의 스킬 레벨 + (소환사의 지식/3)^0.5 (max 100)

	int	iEngageAbility	=	m_wDefaultSkillSlot,iSpecialAbility=0xffff;

	for (i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
	{
		if	(m_aAbility[i].m_wSkill	!=	0xffff && iEngageAbility	==	0xffff)
		{
			iEngageAbility	=	i;
			continue;
		}

		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if	(!lpSkill)
			break;

		if	(lpSkill->isAgressiveSkill())
		{
			iSpecialAbility	=	i;
			break;
		}
	}

	if	(iEngageAbility	==	0xffff)	//	임시
	{
		m_aAbility[0].set(500,1);

		iEngageAbility	=	0;
	}

	if	(iSpecialAbility	!=	0xffff)
		m_specialAbility.copy(&m_aAbility[iSpecialAbility]);
	else
		m_specialAbility.copy(&m_aAbility[iEngageAbility]);

	setEngageAbility(&m_aAbility[iEngageAbility]);

	int	iIncreaseStatePercentage=	m_wPetLoyaltyValue*lpTamer->getStrength()/10000;	//	천분률
//	v펫 단계별 스탯 증가량 : (펫의 충성도/100) * (조련사의 힘/10000)

	m_strength.m_sValue			+=	m_strength.m_sValue*iIncreaseStatePercentage/1000;
	m_agility.m_sValue			+=	m_agility.m_sValue*iIncreaseStatePercentage/1000;
	m_constitution.m_sValue		+=	m_constitution.m_sValue*iIncreaseStatePercentage/1000;
	m_intelligence.m_sValue		+=	m_intelligence.m_sValue*iIncreaseStatePercentage/1000;
	m_wisdom.m_sValue			+=	m_wisdom.m_sValue*iIncreaseStatePercentage/1000;
	m_charisma.m_sValue			+=	m_charisma.m_sValue*iIncreaseStatePercentage/1000;
	m_luck.m_sValue				+=	m_luck.m_sValue*iIncreaseStatePercentage/1000;

	int		iScale				=	min(dPET_MINIMUM_SCALE+m_wPetLoyaltyValue*0.0125,100);	// 09.10.22 계산식변경. 총성도 0 일땐 스케일 75% 충성도 200은... 스케일 100%
	
	m_wScaleX					=	iScale;
	m_wScaleY					=	iScale;

	int	iIncreasePetPower		=	lpTamer->getIncreasePetPowerPerLevel();

	if	(iIncreasePetPower)
	{
		m_strength.m_sValue			+=	iIncreasePetPower;
		m_agility.m_sValue			+=	iIncreasePetPower;
		m_constitution.m_sValue		+=	iIncreasePetPower;
		m_intelligence.m_sValue		+=	iIncreasePetPower;
		m_wisdom.m_sValue			+=	iIncreasePetPower;
		m_charisma.m_sValue			+=	iIncreasePetPower;
		m_luck.m_sValue				+=	iIncreasePetPower;
	}

	m_defensePower.m_iValue		+=	lpTamer->getAgility()*10;	//	최소 방어력
}

void
cACTOR::buildByPetInfo(cACTOR *_lpLord,cPetDefine *_lpPetInfo,int _iPetIndex)
{
	m_wNpcActorSaveSerial	=	0xffff;
	m_wJobIndex				=	_lpPetInfo->getJobIndex();

	m_wPetLoyaltyValue		=	_lpPetInfo->m_bf14Loyalty;
	m_wPetIndex				=	_iPetIndex;
	m_dwPlayTime			=	_lpPetInfo->m_wBreedingTime;

	cJOB	*lpJob			=	getJob();

	if(!lpJob)
		return;

	memcpy(&m_iLevel,&lpJob->m_iLevel,sizeof(CJobBasicDataDefine));

	m_iLevel				=	_lpPetInfo->m_bf10Level;
	m_iExperience			=	_lpPetInfo->m_dwExperience;
	m_wActorKind			=	eAK_MONSTER;

	m_wJob					=	lpJob->m_wBody;
	m_wTrueJob				=	lpJob->m_iSerial+100;
	m_allignment.m_sValue	=	lpJob->m_sAllignment;

	memcpy(m_strName,_lpPetInfo->m_strName,dPET_NAME_LENGTH);
	m_strName[dPET_NAME_LENGTH]	=	NULL;
	m_wTamer				=	_lpLord->m_wSerialInField;
	m_dwTamerNameHashCode	=	_lpLord->m_dwNameHashCode;
	m_wIsPet				=	TRUE;

	cACTOR_STATUS::reset();
	initData();

	m_iHP		=	_lpPetInfo->m_bf22CurrentHP*100;

	if	(_lpPetInfo->m_bf22CurrentHP	==	0	||	(_lpPetInfo->m_bf22CurrentHP+1)*100	>=	(DWORD)getMaxHP())
		m_iHP	=	getMaxHP();

	m_wTamedMonsterCommand	=	eTMD_READY;

	setAiType(eAT_TAMED_MONSTER);
	setAiReady();
}

cACTOR*
cACTOR::getPetActor(int _iIndex)
{
	if	(_iIndex	>=	dOWN_PET_COUNT)
		return	NULL;

	if	(m_aPet[_iIndex].m_bf10Level	== 0)
		return	NULL;

	cACTOR	*lpPet	=	m_lpField->getActor(m_awTamedMonsterList[_iIndex]);

	if	(!lpPet)
		return	NULL;

	if	(!lpPet->isPet()	||	!isOwnForce(lpPet))
		return	NULL;

	if	(lpPet->m_wTamer	==	m_wSerialInField)
		return	lpPet;

	return	NULL;
}

cPetDefine*
cACTOR::getPet(int _iIndex)
{
	if (_iIndex	>=	dOWN_PET_COUNT	||	_iIndex	<	0)
		return	NULL;
	if (m_aPet[_iIndex].m_bf10Level == 0)
		return	NULL;

	return	&m_aPet[_iIndex];
}

BOOL
cACTOR::transBreedingRecordBook()
{
	cACTOR	*lpTamer	=	getTamer();

	if	(!lpTamer	||	!lpTamer->isPlayer())
		return	FALSE;

	int			iSlot	=	0;
	cItem		*lpSlot	=	lpTamer->getEmptyItemSlot(&iSlot);
	cPetDefine	*lpPet	=	&lpTamer->m_aPet[m_wPetIndex];

	if	(!lpSlot || lpPet->m_bf10Level	==	0)
	{
		lpTamer->sendRegistServerMessage(eRSM_THAT_SKILL_REQUIRE_VALID_SLOT);

		return	FALSE;
	}

	lpSlot->m_wBaseItem	=	g_im.m_iBreedingRecordItem;
	char	*lpData		=	(char *)lpSlot;

	cBreedingRecordBookDefine	*lpBook	=	(cBreedingRecordBookDefine	*)(lpData+6);

	lpSlot->m_dwSerial	=	lpBook->buildByPetInfo(&lpTamer->m_aPet[m_wPetIndex],lpTamer->m_wAvatarIndex,lpTamer->m_iLevel);
//	데이터 쪼개려니 무지 복잡해 졌다.

	lpTamer->sendUpdateItemData(iSlot,TRUE);
	lpTamer->sendMakeBreedingRecordBook(m_wPetIndex);

	lpTamer->sendPetWorkLog(m_wPetIndex,ePW_SEAL);
	lpTamer->sendCreateItemLog(lpSlot,eCIM_BREEDING_RECORD_BOOK);

	lpTamer->removePet(m_wPetIndex);
	lpTamer->sendRemovePet(m_wPetIndex);

	m_iDeathCount		=	0xffff;
	m_wPetReleaseCounter=	dSYNC_FPS/4;	//	10초후에 사라진다.

	lpTamer->immediatelySendSaveDataToDBMemory();

	m_lpField->stopAllTargetedAction(this);
	
	lpTamer->m_bf4RemainTimeForUseBreedingRecordBook	=	11;

	return	TRUE;
}

BOOL
cACTOR::unsealPet(cItem *_lpItem)
{
	if	((m_wJob/2)	!=	(dJOB_SUMMONER/2))
		return	FALSE;
	
	if	(m_lpField->m_bIsGuildBattleField)
		return	FALSE;

	if	(g_bIsChansTestServer == FALSE || isOperator() == FALSE)
		if	(m_bf4RemainTimeForUseBreedingRecordBook)
		{
			sendRegistServerMessage(eRSM_TOO_SHORT_TIME_WHEN_SEAL_PET,eSM_SYSTEM);
			return	FALSE;
		}

	if	(getPetCount()	>=	dOWN_PET_COUNT)
	{
		sendRegistServerMessage(eRSM_TOO_MANY_PET_CAN_NOT_UNSEAL_BREEDING_RECORD_BOOK,eSM_SYSTEM);
		return	FALSE;
	}

	if	(getPetCount()	>=	getMaxPetCount())
	{
		sendRegistServerMessage(eRSM_FOR_UNSEAL_PET_REQUIRE_MORE_POWERFUL_TAME_SKILL,eSM_SYSTEM);
		return	FALSE;
	}

	cPetDefine	petInfo;
	BOOL		bIsIncorrectTamer	=	FALSE;

	{
		char	*lpData		=	(char *)_lpItem;
		cBreedingRecordBookDefine	*lpBook	=	(cBreedingRecordBookDefine	*)(lpData+6);

		if	(lpBook->isAvailInfo(_lpItem->m_dwSerial))
		{
			int	iAvatarIndex	=	lpBook->getAvatarIndex();
			int	iAvatarLevel	=	lpBook->getAvatarLevel();

			if	(m_wAvatarIndex	!=	iAvatarIndex)
				bIsIncorrectTamer	=	TRUE;
			else
			if	(m_iLevel		<	iAvatarLevel)
				bIsIncorrectTamer	=	TRUE;
			else
				lpBook->makePetInfo(&petInfo,_lpItem->m_dwSerial);
		}
		else
		{
			memcpy(&petInfo,&_lpItem->m_bCount,sizeof(cPetDefine)-4);
			petInfo.m_dwExperience		=	_lpItem->m_dwSerial;

			if	(petInfo.m_bf2TamerSlot	!=	(m_wAvatarIndex%4))
				bIsIncorrectTamer	=	TRUE;
		}
	}

	int	iJob	=	petInfo.getJobIndex();

	if	(g_aJob[iJob].m_bf3RequireRebirthCountForTame	>	m_bf4RebirthCount	||	bIsIncorrectTamer)
	{
		sendRegistServerMessage(eRSM_BREEDING_RECORD_UNSEAL_ONLY_SEAL_PLAYER,eSM_SYSTEM);
		return	FALSE;
	}
	
	int	iPetIndex	=	addPet(&petInfo);

	if	(iPetIndex	==	0xffff)
		return	FALSE;

	sendPetWorkLog(iPetIndex,ePW_UNSEAL);
	sendRemoveItemLog(_lpItem,eRI_UNSEAL_BREEDING_RECORD_BOOK);

	_lpItem->reset();
	m_lpField->bookingGeneratePet(m_strName,iPetIndex,TRUE);

	immediatelySendSaveDataToDBMemory();

	return	TRUE;
}

BOOL
cACTOR::boostPetPower(int _iBoostPower,int _iUpkeepTime,int _iLimitCount)
{
	cACTOR		*lpTamer=	getTamer();

	if (!lpTamer)
		return	FALSE;

	cPetDefine	*lpPet	=	&lpTamer->m_aPet[m_wPetIndex];

	if (lpPet->m_bf5BoostCount	>=	_iLimitCount)
	{
		lpTamer->sendRegistServerMessage(eRSM_THAT_PET_ALREADY_USE_ALL_BOOST_POWER_FOR_TODAY,eSM_SYSTEM);

		return	FALSE;
	}

	lpPet->m_bf5BoostCount++;

	m_wBoostPower	=	100+_iBoostPower;
	m_wBoostTime	=	_iUpkeepTime;

	buildPower();

	addSendSimpleInfo();
	lpTamer->sendPetInfo(m_wPetIndex,m_wSerialInField,FALSE);

	return	TRUE;
}

void
cACTOR::changePetName(int _iIndex,char *_lpstrName,BOOL _bIsByClient,BOOL _bIsMiniPet,int _iUseItemIndexForReset)
{
	if	(_bIsMiniPet)
	{
		cMiniPet	*lpMiniPet	=	iMiniPet()->getMiniPet(_iIndex);

		if	(!lpMiniPet)
			return;

		if	(lpMiniPet->m_bf7Level	==	0)
			return;

		if	(_bIsByClient && STRICMP(lpMiniPet->m_strName,"noname")!=0)
		{
			sendRegistServerMessage(eRSM_ALREADY_NAMED_PET);

			return;
		}

		memcpy(lpMiniPet->m_strName,_lpstrName,dPET_NAME_LENGTH);

		sendChangePetName(_iIndex,_lpstrName,_bIsMiniPet);
	}
	else
	{
		cPetDefine	*lpPet	=	getPet(_iIndex);

		if	(!lpPet)
			return;

		cItem	*lpResetPetNameItem	=	NULL;

		if	(_bIsByClient && STRICMP(lpPet->m_strName,"noname")!=0)
		{
			if	(_iUseItemIndexForReset	==	-1)
				return;

			lpResetPetNameItem	=	getInventoryItem(_iUseItemIndexForReset);

			if	(!lpResetPetNameItem)
				return;

			if	(!lpResetPetNameItem->isOwnSpecificEffectItem(eIE_CHANGE_PET_NAME))
				return;
		}
		
		memcpy(lpPet->m_strName,_lpstrName,dPET_NAME_LENGTH);

		cACTOR	*lpPetActor	=	getPetActor(_iIndex);

		if	(lpPetActor)
		{
			strcpy(lpPetActor->m_strName,_lpstrName);

			lpPetActor->addSendSimpleInfo();
		}

		if	(lpResetPetNameItem)
			lpResetPetNameItem->decreaseCount(1);

		sendChangePetName(_iIndex,_lpstrName,FALSE,_iUseItemIndexForReset);
	}
}

void
cACTOR::partingWithPet(int _iIndex,int _iTpye)
{
	if	(_iTpye	==	2)
	{
		cMiniPet	*lpMiniPet	=	iMiniPet()->getMiniPet(_iIndex);

		if	(!lpMiniPet	||	lpMiniPet->m_bf7Level	==	0)
			return;

		sendReleaseMiniPetLog(lpMiniPet);

		lpMiniPet->reset();

		if	((int)m_bf5ActiveMiniPet	==	_iIndex)
		{
			m_bf5ActiveMiniPet	=	m_bf5ActiveMiniPet2;
			m_bf5ActiveMiniPet2	=	c_iMiniPetCount;
		}

		if	((int)m_bf5ActiveMiniPet2	==	_iIndex)
			m_bf5ActiveMiniPet2	=	c_iMiniPetCount;
		
		buildPower();

		sendRemovePet(_iIndex,TRUE);
	}
	if	(_iTpye	==	1)
	{
		cACTOR	*lpBeast	=	getSummonBeastActor(_iIndex);

		if	(!lpBeast)
			return;

		if	(m_wRidingDogTime)
		{
			if	(lpBeast->m_wSummonBeastType	==	eSBT_KELBY)
				getOffFromDog();
		}

		lpBeast->bookingDeath(NULL,TRUE);
		removeSummonBeast(_iIndex);

		if	(lpBeast->isDeath())
			deathSummonBeast(_iIndex,lpBeast->m_iLevel);

		return;
	}

	if	(_iTpye	==	0)
	{
		cPetDefine	*lpPet	=	getPet(_iIndex);

		if (!lpPet)
			return;

		cACTOR	*lpPetActor	=	NULL;

		if	(_iIndex	<	dOWN_PET_COUNT	&&	m_aPet[_iIndex].m_bf10Level)
		{
			lpPetActor	=	m_lpField->getActor(m_awTamedMonsterList[_iIndex]);

			if	(lpPetActor)
			{
				if	(!lpPetActor->isPet())
					lpPetActor	=	NULL;
				else
				if	(lpPetActor->m_wTamer	!=	m_wSerialInField)
					lpPetActor	=	NULL;
			}
		}

		sendPetWorkLog(_iIndex,ePW_REMOVE);
		removePet(_iIndex);

		if	(lpPetActor)
		{
			lpPetActor->m_iDeathCount		=	0xffff;
			lpPetActor->m_wPetReleaseCounter=	1;	//	1프레임후에 사라진다.

			m_lpField->stopAllTargetedAction(lpPetActor);
		}
		
		immediatelySendSaveDataToDBMemory();
		sendRemovePet(_iIndex,FALSE);
		return;
	}
}

int
cACTOR::getFollowMonsterCount()
{
	int	iCount=0,i;

	for (i=0;i<dMAX_TAME_MONSTER_COUNT;i++)
	{
		if (m_awTamedMonsterList[i]	!=	0xffff)
			iCount++;
	}

	for (i=0;i<dMAX_SUMMON_BEAST_COUNT;i++)
	{
		if (m_aSummonBeast[i].m_bf10Level	!=	0)
			iCount++;
	}

	return	iCount;
}

int
cACTOR::getFollowMonsterIndex(int _iSerial)
{
	int	iCount=0,i;

	for (i=0;i<dMAX_TAME_MONSTER_COUNT;i++)
	{
		if (m_awTamedMonsterList[i]	!=	0xffff)
		{
			if (m_awTamedMonsterList[i]	==	_iSerial)
				return	iCount;
			
			iCount++;
		}
	}

	for (i=0;i<dMAX_SUMMON_BEAST_COUNT;i++)
	{
		if (m_awSummonBeastList[i]	!=	0xffff)
		{
			if (m_awSummonBeastList[i]	==	_iSerial)
				return	iCount;

			iCount++;
		}
	}

	return	0xffff;
}

//
//	펫 능력치를 올려주는 정보
int
cACTOR::getIncreasePetPowerPerLevel()
{
	int	IncreasePetPowerPerLevel	=	0;

	for (int i=dHERO_ABILITY_COUNT;i;)
	{
		--i;

		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if	(!lpSkill	||	m_aAbility[i].m_wLevel	==	0)
			continue;

		if	(lpSkill->m_sIncreasePetPowerPerLevel	==	0)
			continue;

		IncreasePetPowerPerLevel	+=	lpSkill->m_sIncreasePetPowerPerLevel*m_aAbility[i].m_wLevel;
	}

	return	IncreasePetPowerPerLevel/100;
}

int
cACTOR::getPetCount()
{
	int	iPetCount	=	0;

	for (int i=0;i<dOWN_PET_COUNT;i++)
	{
		if	(m_aPet[i].m_bf10Level)
			iPetCount++;
	}

	return	iPetCount;
}

int
cACTOR::getMaxPetCount()
{
	int	iMaxPetCount	=	0;

	for (int i=0;i<dHERO_ABILITY_COUNT;i++)
	{
		CSkill	*lpSkill	=	m_aAbility[i].getSkill();

		if (!lpSkill || m_aAbility[i].m_wLevel	==	0)
			continue;

		iMaxPetCount	=	max(lpSkill->m_bf3MaxPetCount,(DWORD)iMaxPetCount);
	}

	return	iMaxPetCount;
}

void
cACTOR::updatePetInfo(cACTOR *_lpActor)
{
	if (!isPet())
		return;

	if (_lpActor->m_wTamer != m_wSerialInField)
		return;

	m_aPet[_lpActor->m_wPetIndex].m_bf22CurrentHP	=	_lpActor->m_iHP/100;
}

