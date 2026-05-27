#include "cACTOR.H"
#include "cFIELD.H"
#include "cNPC.H"
#include "debugCode.H"
#include "CGame.H"
#include "packetManager.H"

CJobBasicDataDefine	l_aSummonBeastStateInfo[dSUMMON_BEAST_TYPE_COUNT]	=
{
	//	켈비
	{

		1,0,0,	//	레벨,경험치,스킬 경험치
		5000,	//	현재 체력
		50000,	//	기본 체력
		0,		//	현재 CP
		0,		//	기본 CP
		500,500,//	HP보정,최대 HP 계산할때 건강 보정

		200,		//	힘
		150,		//	민첩성
		200,		//	건강
		100,		//	지혜
		100,		//	지식
		0,		//	카리스마
		150,		//	운

		0,		//	시야

		5,		//	최소 데미지
		5,		//	최대 데미지
		0,		//	방어력

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	},
	//	스웰퍼
	{

		1,0,0,	//	레벨,경험치,스킬 경험치
		25000,	//	현재 체력
		25000,	//	기본 체력
		0,		//	현재 CP
		0,		//	기본 CP
		800,800,//	HP보정,최대 HP 계산할때 건강 보정

		150,	//	힘
		150,	//	민첩성
		150,	//	건강
		200,	//	지혜
		150,	//	지식
		0,	//	카리스마
		150,	//	운

		0,		//	시야

		6,		//	최소 데미지
		6,		//	최대 데미지
		0,		//	방어력

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	},
	//	윈디
	{

		1,0,0,	//	레벨,경험치,스킬 경험치
		2000,	//	현재 체력
		2000,	//	기본 체력
		0,		//	현재 CP
		0,		//	기본 CP
		100,100,//	HP보정,최대 HP 계산할때 건강 보정

		10,	//	힘
		20,	//	민첩성
		10,	//	건강
		25,	//	지혜
		15,	//	지식
		0,	//	카리스마
		20,	//	운

		0,		//	시야

		1,		//	최소 데미지
		1,		//	최대 데미지
		0,		//	방어력

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	},
	//	헷저
	{

		1,0,0,	//	레벨,경험치,스킬 경험치
		30000,	//	현재 체력
		30000,	//	기본 체력
		0,		//	현재 CP
		0,		//	기본 CP
		1000,1000,//	HP보정,최대 HP 계산할때 건강 보정

		150,	//	힘
		150,	//	민첩성
		20,	//	건강
		10,	//	지혜
		20,	//	지식
		0,	//	카리스마
		10,	//	운

		0,		//	시야

		1,		//	최소 데미지
		1,		//	최대 데미지
		0,		//	방어력

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	}
};

class	cSummonBeastBasicInfo
{
public:
	WORD	m_wStrength,m_wAgility,m_wConstitution,m_wIntelligence,m_wWisdom,m_wCharisma,m_wLuck;
	int		m_iBaseHP;
	WORD	m_wStrengthCorrect,m_wAgilityCorrect,m_wConstitutionCorrect,m_wIntelligenceCorrect,m_wWisdomCorrect,m_wCharismaCorrect,m_wLuckCorrect;
	WORD	m_wFire,m_wWater,m_wWind,m_wEarth,m_wLight,m_wDark;
	WORD	m_wCorrectHPPerLevel,m_wCorrectHPPerConstitution;
	WORD	m_wBasicSkill;
	float	m_fSkillLevelFactor;

	int		m_iMinDamageFactor1;
	float	m_iMinDamageFactor2;
	int		m_iMinDamageFactor3;
	int		m_iMaxDamageFactor1;
	float	m_iMaxDamageFactor2;
	int		m_iMaxDamageFactor3;

	WORD	m_wDefenseFactor;
};

//레벨 업에 따른 스탯 분배치
cSummonBeastBasicInfo	l_aSBBasicPowerInfo[dSUMMON_BEAST_TYPE_COUNT]	=	
{
//구분										힘		민첩성	건강	지식	지혜	매력	운		체력	불저	물저	바람	대지	빛저	어둠저	체력/건강보정	스킬/스킬레벨
	{20,	15,	20,	10,	10,	0,	15,	3000,	120,	100,	80,		60,		60,		0,		80,		10,		0,		0,		0,		0,		0,		100,	100,	613	,0.6f,200,0.5f,0  ,300,0.5f,0  , 40},//	켈비	
	{15,	15,	15,	20,	15,	0,	15,	2500,	100,	60,		80,		100,	100,	0,		60,		0,		10,		0,		0,		0,		0,		100,	100,	598	,0.6f,230,0.5f,200,240,0.5f,400, 35},	//	스웰퍼	
	{10,	20,	10,	25,	15,	0,	20,	2000,	60,		100,	60,		100,	80,		0,		100,	0,		0,		10,		0,		0,		0,		100,	100,	626	,0.6f,50 ,0.5f,0  ,550,0.5f,0  , 30},	//	윈디	
	{15,	15,	20,	10,	20,	0,	10,	3000,	80,		80,		100,	60,		100,	0,		80,		0,		0,		0,		10,		0,		0,		100,	100,	627	,0.6f,180,0.5f,0  ,360,0.5f,0  , 50},	//	헷저	
};

int
cACTOR::getSummonBeastCount()
{
	int	iCount	=	0,i;

	for (i=0;i<dMAX_SUMMON_BEAST_COUNT;i++)
	{
		if (m_aSummonBeast[i].m_bf10Level	!=	0)
			iCount++;
	}

	return	iCount;
}

BOOL
cACTOR::isExistSummonBeast(int _iType)
{
	for (int i=0;i<dMAX_SUMMON_BEAST_COUNT;i++)
	{
		if (m_aSummonBeast[i].m_bf10Level	==	0)
			continue;

		if (m_aSummonBeast[i].m_bf2Type	==	_iType)
			return	TRUE;
	}

	return	FALSE;
}

void
cACTOR::buildSummonBeast(int _iLevel,int _iRemainHP,int _iType,int _iGrade,int _iTamer,DWORD _dwTamerNameHashCode)
{
	if	(_iGrade	<	2)
		m_awSBLevel[_iType][_iGrade]	=	_iLevel;
	
	if	(_iGrade	>=	dSUMMON_BEAST_GRADE_COUNT)
		return;

	m_wTamer				=	_iTamer;
	m_dwTamerNameHashCode	=	_dwTamerNameHashCode;
	m_wIsSummonBeast		=	TRUE;
	m_wTamedMonsterCommand	=	eTMD_READY;

	m_wNpcActorSaveSerial	=	0xffff;
	m_iLevel				=	_iLevel;
	m_wJob					=	dJOB_KELBY_1+_iType*dSUMMON_BEAST_GRADE_COUNT+_iGrade;
	m_wSummonBeastType		=	_iType;
	m_wSummonBeastGrade		=	_iGrade;
	m_wTrueJob				=	m_wJob;
	m_wJobIndex				=	0xffff;

	cACTOR	*lpTamer	=	getTamer();

	if (lpTamer && m_wPetIndex < dOWN_SUMMON_BEAST_COUNT)
	{
		lpTamer->m_aSummonBeast[m_wPetIndex].m_bf2Grade	=	_iGrade;
		lpTamer->m_aSummonBeast[m_wPetIndex].m_bf2Grade	=	_iGrade;
	}

	cACTOR_STATUS::reset();
	initData();

	setAiType(eAT_TAMED_MONSTER);
	setAiReady();

	m_specialAbility.reset();

	strcpy(m_strName,"sb");
	m_iHP					=	min(getMaxHP(),_iRemainHP);
}

void
cACTOR::buildSummonBeastBasicPower(cACTOR *_lpTamer)
{
	if	(_lpTamer	==	NULL)
		return;
	
	int		iLevel					=	getLevel()-1;
	int		iGrade					=	m_wSummonBeastGrade+1;

	cSummonBeastBasicInfo	*lpInfo	=	&l_aSBBasicPowerInfo[m_wSummonBeastType];

	m_wCorrectMaxHPConstitutionFactor	=	lpInfo->m_wCorrectHPPerConstitution;
	m_wCorrectMaxHPFactor				=	lpInfo->m_wCorrectHPPerLevel;

	m_level.m_sValue			=	m_iLevel;

	m_sStrength					=	lpInfo->m_wStrength;
	m_sAgility					=	lpInfo->m_wAgility;
	m_sConstitution				=	lpInfo->m_wConstitution;

	m_sCharisma					=	lpInfo->m_wCharisma;
	m_sLuck						=	lpInfo->m_wLuck;
	m_strength.m_sValue			=	(int)m_sStrength	+	iLevel*lpInfo->m_wStrengthCorrect/100;
	m_agility.m_sValue			=	(int)m_sAgility		+	iLevel*lpInfo->m_wAgilityCorrect/100;
	m_constitution.m_sValue		=	(int)m_sConstitution+	iLevel*lpInfo->m_wConstitutionCorrect/100;

	m_sWisdom					=	lpInfo->m_wWisdom;
	m_sIntelligence				=	lpInfo->m_wIntelligence;
	m_intelligence.m_sValue		=	(int)m_sIntelligence+	iLevel*lpInfo->m_wIntelligenceCorrect/100;
	m_wisdom.m_sValue			=	(int)m_sWisdom		+	iLevel*lpInfo->m_wWisdomCorrect/100;
	m_charisma.m_sValue			=	(int)m_sCharisma	+	iLevel*lpInfo->m_wCharismaCorrect/100;
	m_luck.m_sValue				=	(int)m_sLuck		+	iLevel*lpInfo->m_wLuckCorrect/100;
//소환수 단계별 스탯 증가량 : (각 소환수의 단계 - 1) * (10%+소환사의 힘/10000)
	int	iCorrectGrade			=	(iGrade-1)*(10+_lpTamer->getStrength()/100);

	m_strength.m_sValue			+=	m_strength.m_sValue*iCorrectGrade/100;
	m_agility.m_sValue			+=	m_agility.m_sValue*iCorrectGrade/100;
	m_constitution.m_sValue		+=	m_constitution.m_sValue*iCorrectGrade/100;
	m_intelligence.m_sValue		+=	m_intelligence.m_sValue*iCorrectGrade/100;
	m_wisdom.m_sValue			+=	m_wisdom.m_sValue*iCorrectGrade/100;
	m_charisma.m_sValue			+=	m_charisma.m_sValue*iCorrectGrade/100;
	m_luck.m_sValue				+=	m_luck.m_sValue*iCorrectGrade/100;

	m_iMaxHP					=	lpInfo->m_iBaseHP;
	m_maxHP.m_iValue			=	lpInfo->m_iBaseHP;

	iLevel						=	getLevel();

	m_fireResistance.m_sValue	=	(1+iGrade)*lpInfo->m_wFire*iLevel/100;	//	불꽃 저항
	m_waterResistance.m_sValue	=	(1+iGrade)*lpInfo->m_wWater*iLevel/100;	//	물 저항
	m_windResistance.m_sValue	=	(1+iGrade)*lpInfo->m_wWind*iLevel/100;	//	바람 저항
	m_earthResistance.m_sValue	=	(1+iGrade)*lpInfo->m_wEarth*iLevel/100;	//	대지 저항
	m_lightResistance.m_sValue	=	(1+iGrade)*lpInfo->m_wLight*iLevel/100;	//	빛 저항
	m_darkResistance.m_sValue	=	(1+iGrade)*lpInfo->m_wDark*iLevel/100;	//	어둠 저항

	memset(m_aAbility,0xff,sizeof(m_aAbility));

//	몬스터 기술 613 인페르노 바이트 (스킬 레벨 = (켈비 레벨+소환사의 지식/3) ^0.6), max 100)
//	몬스터 기술 598 프로스트 바이트 (스킬 레벨 = (스웰퍼 레벨+소환사의 지식/3) ^0.6), max 100)
//	몬스터 기술 626 윈디 클랩 (스킬 레벨 = (윈디 레벨+소환사의 지식/3) ^0.6), max 100)
//	몬스터 기술 627 어쓰 블레이드 (스킬 레벨 = (헷저 레벨+소환사의 지식/3) ^0.6), max 100)

	m_aAbility[0].m_wSkill		=	lpInfo->m_wBasicSkill;
	m_aAbility[0].m_wLevel		=	(WORD)pow((double)(iLevel+_lpTamer->getIntelligence()/3),lpInfo->m_fSkillLevelFactor);
	m_aAbility[0].m_wLevel		=	min(m_aAbility[0].m_wLevel,100);

	m_wCorrectMaxHPConstitutionFactor	=	50+iGrade*50;
	m_wCorrectMaxHPFactor				=	100+iGrade*70;
//소환수 HP의 건강 보정치, 레벨 보정치 추가
//건강 보정치		레벨 보정치
//0.5+켈비단계*0.5	1+켈비단계*0.7
//0.5+스웰퍼단계*0.5	1+스웰퍼단계*0.7
//0.5+윈디단계*0.5	1+윈디단계*0.7
//0.5+헷저단계*0.5	1+헷저단계*0.7

	m_bFreezeResistance			=	0;	//	프리즈 저항
	m_bColdResistance			=	0;		//	콜드 저항
	m_stunResistance.m_sValue	=	0;		//	경직 저항
	m_bConfuseResistance		=	0;	//	혼란 저항
	m_charmingResistance.m_sValue=	0;//	챠밍 저항
	m_bStoneResistance			=	0;		//	석화 저항
	m_bPoisonResistance			=	0;	//	중독 저항
	m_bSleepResistance			=	0;		//	잠 저항
	m_bBlindResistance			=	0;		//	장님 저항

	m_strangeStatusResistance.m_sValue	=	0;	//	상태 이상 저항
	m_declinePowerResistance.m_sValue	=	0;//	능력치 저하 저항
	m_curseResistance.m_sValue			=	0;		//	저주 저항

	m_iWeaponMinDamage			=	(int)(lpInfo->m_iMinDamageFactor1*pow(iLevel,lpInfo->m_iMinDamageFactor2)-lpInfo->m_iMinDamageFactor3);
	m_iWeaponMaxDamage			=	(int)(lpInfo->m_iMaxDamageFactor1*pow(iLevel,lpInfo->m_iMaxDamageFactor2)+lpInfo->m_iMaxDamageFactor3);

//	0.4 * (Lv+소환사의 민첩/2)
//	0.35 * (Lv+소환사의 민첩/2)
//	0.3 * (Lv+소환사의 민첩/2)
//	0.5 * (Lv+소환사의 민첩/2)
	m_defensePower.m_iValue		=	100+(m_iLevel+_lpTamer->getAgility()/2) *lpInfo->m_wDefenseFactor;
	m_iWeaponMaxDamage			=	max(m_iWeaponMaxDamage,m_iWeaponMinDamage);

	m_correctCriticalHitChance.reset();
	m_bCorrectCrushBlowChance	=	0;
	m_bCriticalHitResistance	=	0;			//	크리티컬 저항
	m_bCrushBlowResistance		=	0;				//	크러쉬 블로우 저항

	m_hitChance.m_sValue		=	0;
	m_dodgeChance.m_sValue		=	0;
	m_blockingChance.m_sValue	=	0;
	m_wActionSpeed				=	dSYNC_FPS;
	m_wNpcMoveSpeed				=	100;

	m_wScaleX					=	100;
	m_wScaleY					=	100;

	m_strength.m_sValue			+=	_lpTamer->m_correctSummonBeastState.getCorrectedValue();
	m_agility.m_sValue			+=	_lpTamer->m_correctSummonBeastState.getCorrectedValue();
	m_constitution.m_sValue		+=	_lpTamer->m_correctSummonBeastState.getCorrectedValue();
	m_intelligence.m_sValue		+=	_lpTamer->m_correctSummonBeastState.getCorrectedValue();
	m_wisdom.m_sValue			+=	_lpTamer->m_correctSummonBeastState.getCorrectedValue();
	m_charisma.m_sValue			+=	_lpTamer->m_correctSummonBeastState.getCorrectedValue();
	m_luck.m_sValue				+=	_lpTamer->m_correctSummonBeastState.getCorrectedValue();
}

int
cACTOR::addSummonBeast(cACTOR *_lpActor)
{
	for (int i=0;i<dMAX_SUMMON_BEAST_COUNT;i++)
	{
		if (m_awSummonBeastList[i]	==	0xffff)
		{
			m_awSummonBeastList[i]				=	_lpActor->m_wSerialInField;
			m_aSummonBeast[i].m_bf10Level		=	_lpActor->m_iLevel;
			m_aSummonBeast[i].m_bf22CurrentHP	=	_lpActor->m_iHP/100;
			m_aSummonBeast[i].m_bf2Grade		=	_lpActor->m_wSummonBeastGrade;
			m_aSummonBeast[i].m_bf2Type			=	_lpActor->m_wSummonBeastType;

			return	i;
		}
	}

	return	0xffff;
}

void
cACTOR::removeSummonBeast(int _iIndex)
{
	m_awSummonBeastList[_iIndex]		=	0xffff;
	m_aSummonBeast[_iIndex].m_bf10Level	=	0;

	sendSummonBeastInfo(_iIndex,FALSE);
}

void
cACTOR::updateSummonBeastInfo(cACTOR *_lpActor)
{
	if	(!_lpActor->isSummonBeast())
		return;

	if	(_lpActor->m_wTamer != m_wSerialInField)
		return;
	
	if	(m_awSummonBeastList[_lpActor->m_wPetIndex]	!=	_lpActor->m_wSerialInField)
		return;

	int	iIndex	=	_lpActor->m_wPetIndex;

	if	(m_aSummonBeast[iIndex].m_bf10Level	==	0	&&	_lpActor->isDeath())
		return;

	m_aSummonBeast[iIndex].m_bf22CurrentHP	=	_lpActor->m_iHP/100;
	m_aSummonBeast[iIndex].m_bf2Grade		=	_lpActor->m_wSummonBeastGrade;
	m_aSummonBeast[iIndex].m_bf2Type		=	_lpActor->m_wSummonBeastType;
}

cACTOR*
cACTOR::getSummonBeastActor(int _iIndex)
{
	int	iBeastSerial	=	m_awSummonBeastList[_iIndex];

	if	(iBeastSerial	==	0xffff)
		return	NULL;

	cACTOR	*lpActor	=	m_lpField->getTestedActor(iBeastSerial);

	if	(!lpActor || lpActor->m_wTamer	!= m_wSerialInField) 
	{
		m_awSummonBeastList[_iIndex]	=	0xffff;

		return	NULL;
	}

	return	lpActor;
}

void
cACTOR::deathSummonBeast(int _iIndex,int _iLevel)
{
	int iSummonBeastType		=	m_aSummonBeast[_iIndex].m_bf2Type;

	removeSummonBeast(_iIndex);
//	소환수 부활 딜레이 타임 : 소환수의 HP가 0 이하로 떨어지는 순간 소환은 해제되며 [소환수의 레벨-소환사의 카리스마/2]초간 그 소환수는 소환 불가(최소 60초). 이 경우는 행동 중인 소환수도 행동 취소하고 바로 소환 해제
	
	m_awRemainTimeForRecallSummonBeast[iSummonBeastType]=	max(dDEFAULT_TIME_FOR_RECALL_SUMMON_BEAST,(_iLevel-getCharisma()/2));

	switch(iSummonBeastType)
	{
		case	eSBT_KELBY		:
			m_bf1WaitForRecallForKelby		=	TRUE;
			break;
		case	eSBT_SWELLFER	:
			m_bf1WaitForRecallForSwellfer	=	TRUE;
			break;
		case	eSBT_WINDY		:
			m_bf1WaitForRecallForWindy		=	TRUE;
			break;
		case	eSBT_HEDGER		:
			m_bf1WaitForRecallForHedger		=	TRUE;
			break;
	}
}

void
cACTOR::operateCommandSkill(int _iSkill,int _iLevel,cACTOR *_lpTarget,int _iX,int _iY)
{
	cAbility	ability;

	ability.set(_iSkill,_iLevel);

	CSkill		*lpSkill	=	ability.getSkill();

	if	(!lpSkill)
		return;

	if	(_lpTarget && isOwnForce(_lpTarget) && lpSkill->isAgressiveSkill())
		return;

	cACTOR	*lpTamer	=	getTamer();

	if	(lpTamer)
		lpTamer->m_dwLastCommandTime=	g_dwCurrentTime;

	m_wUseSpecialAbilityStep		=	ePUSS_READY;

	setAiFight(_lpTarget,&ability,_iX,_iY);
}

BOOL
cACTOR::callSummonBeast(int _iX,int _iY,int _iType,int _iLevel)
{
	_iLevel	=	min(_iLevel,999);

	if	(getSummonBeastCount()	>=	dMAX_SUMMON_BEAST_COUNT)
		return	FALSE;

	if	(isExistSummonBeast(_iType))
		return	FALSE;

	BOOL	bIsExistEmptySlot	=	FALSE;

	for	(int i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		if	(m_awSummonBeastList[i]	==	0xffff)
			bIsExistEmptySlot	=	TRUE;
	}

	if	(!bIsExistEmptySlot)
		return	FALSE;

	if (m_awRemainTimeForRecallSummonBeast[_iType])
	{
		sendRegistServerMessage(eRSM_CAN_NOT_SUMMON_BY_DEATH_RULE,eSM_SYSTEM);

		return	FALSE;
	}

	m_lpField->bookingGenerateSummonBeast(_iX,_iY,m_strName,_iType,0,_iLevel,0x7fffffff,TRUE);

	return	TRUE;
}

BOOL
cACTOR::powerUpSummonBeast(int _iGrade,int _iLevel)
{
	_iLevel	=	min(_iLevel,999);

	cACTOR	*lpTamer	=	getTamer();

	if (!lpTamer)
		return	FALSE;

	buildSummonBeast(_iLevel,m_iHP,m_wSummonBeastType,_iGrade-1,m_wTamer,m_dwTamerNameHashCode);
	lpTamer->updateSummonBeastInfo(this);
	lpTamer->sendSummonBeastInfo(m_wPetIndex,TRUE);
	syncTamedMonsterHP(TRUE);
	addSendSimpleInfo(NULL,TRUE);

	return	TRUE;
}

BOOL
cACTOR::rideTamer(int _iUpkeepTime,int _iCorrectConcentration,int _iCorrectMoveSpeed)
{
	cACTOR	*lpTamer	=	getTamer();

	if (!lpTamer)
		return	FALSE;

	lpTamer->m_concentration.correctValue(_iCorrectConcentration,_iUpkeepTime);

	lpTamer->m_wCorrectMoveSpeedByRidingDog	=	_iCorrectMoveSpeed;
	lpTamer->m_wRidingDogTime				=	_iUpkeepTime;
	m_wRidingDogTime						=	_iUpkeepTime;
	m_sNonTargetTime						=	-1;

	lpTamer->setActorPos(m_pos.x,m_pos.y,TRUE);
	lpTamer->sendRidingDog(TRUE,m_wSummonBeastGrade,m_pos.x,m_pos.y);

	addSendSimpleInfo();
	lpTamer->addSendSimpleInfo();

	return	TRUE;
}

cACTOR*
cACTOR::getKelby()
{
	for (int i=0;i<dMAX_SUMMON_BEAST_COUNT;i++)
	{
		if (m_awSummonBeastList[i]	==	0xffff)
			continue;

		if (m_aSummonBeast[i].m_bf2Type	==	dSBT_KELBY)
			return	getSummonBeastActor(i);
	}

	return	NULL;
}

cACTOR*
cACTOR::getSummonBeastByType(int _iType)
{
	for (int i=0;i<dMAX_SUMMON_BEAST_COUNT;i++)
	{
		if (m_awSummonBeastList[i]	==	0xffff)
			continue;

		if (m_aSummonBeast[i].m_bf2Type	==	_iType)
			return	getSummonBeastActor(i);
	}

	return	NULL;
}

BOOL
cACTOR::getOffFromDog(BOOL _bIsWantSendToUser)
{
	m_wRidingDogTime	=	0;
	m_sNonTargetTime	=	0;
	m_wCorrectMoveSpeedByRidingDog	=	0;

	m_concentration.dispell();

	if	(_bIsWantSendToUser)
	{
		addSendVerySimpleInfo();
		sendRidingDog(FALSE);

		cACTOR	*lpKelby	=	getKelby();

		if	(lpKelby)
			lpKelby->getOffTamer();
	}

	setMoveSpeed();

	return	TRUE;
}

BOOL
cACTOR::getOffTamer()
{
	cACTOR	*lpTamer	=	getTamer();

	if	(!lpTamer)
		return FALSE;

	setActorPos(lpTamer->m_pos.x,lpTamer->m_pos.y);
	m_wRidingDogTime	=	0;
	m_sNonTargetTime	=	0;

	addSendVerySimpleInfo();

	return TRUE;
}

BOOL
cACTOR::catchItem(CDroppedItem *_lpItem)
{
	if (!moveTo(_lpItem->m_pos.x,_lpItem->m_pos.y,100,400))
		return	FALSE;

	m_wObjectItemForCatch		=	_lpItem->m_wSerial;
	m_dwObjectItemSerialForCatch=	_lpItem->m_item.m_dwSerial;
	m_posTarget.x				=	_lpItem->m_pos.x;
	m_posTarget.y				=	_lpItem->m_pos.y;
	m_wIsReturnToTransferItem	=	FALSE;

	setAiMove();

	return	TRUE;
}

void
cACTOR::returnBeastItem()
{
	cACTOR	*lpTamer		=	getTamer();

	if (!lpTamer)
		return;

	m_wIsReturnToTransferItem=	FALSE;

	cItem				item,itemForLog;
	SG_PICK_ITEM_RESULT	sgPacket;

	sgPacket.base.set(sizeof(sgPacket),dSG_PICK_ITEM_RESULT);

	cParty		*lpParty	=	lpTamer->getParty();

	if (m_iGold	>	0)
	{
		item.reset();
		item.m_wBaseItem	=	dITEM_MONEY;
		item.m_dwSerial		=	m_iGold;

		itemForLog.copy(&item);

		if (lpTamer->addGoldItemToInventory(&item,eGET_GOLD_BY_PICK_DROPPED_GOLD)	==	eUPR_CAN_NOT_OWN_MORE_GOLD)
		{
			CDroppedItem	droppedItem;

			droppedItem.m_item.m_wBaseItem	=	dITEM_MONEY;
			droppedItem.m_item.m_dwSerial	=	m_iGold;
			droppedItem.m_wOwner			=	lpTamer->m_wSerialInField;
			droppedItem.m_pos.x				=	m_pos.x+random(40)-20;
			droppedItem.m_pos.y				=	m_pos.y+random(40)-20;
			droppedItem.m_wSealedTime		=	6000;
			droppedItem.m_wDropMethod		=	eDIM_PLAYER_DROP;

			m_lpField->dropItem(&droppedItem);

			lpTamer->sendRegistServerMessage(eRSM_OWN_GOLD_LIMIT,eSM_SYSTEM);

			itemForLog.m_dwSerial			-=	m_iGold;

			if	(lpParty)
				lpParty->sendPickItemMessage(lpTamer,&itemForLog);
		}
		else
		{
			item.m_wBaseItem	=	dITEM_MONEY;
			item.m_dwSerial		=	m_iGold;

			if (m_iGold >= 10000)
				lpTamer->sendPickItemLog(&item,ePIM_BEAST_RETURN);

			memcpy(&sgPacket.item,&item,sizeof(cItem));
			sgPacket.wResult	=	eUIR_OK;

			g_userPM.add(lpTamer->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

			if	(lpParty)
				lpParty->sendPickItemMessage(lpTamer,&itemForLog);
		}
			
		m_iGold	=	0;
	}

	int	iItemCount	=	getItemCount(0xffff);

	if	(iItemCount)
		for (int i=0;i<dOWN_ITEM_COUNT;i++)
		{
			cItem	*lpItem			=	&m_aItems[i];

			if (lpItem->m_wBaseItem	==	0xffff)
				continue;

			cItem	*lpEmptySlot	=	getEmptyItemSlot();

			item.copy(lpItem);
			itemForLog.copy(&item);

			if	(lpItem->isMoneyItem())
				CLOG("goldIOError","returnBeastItem");

			if	(!lpEmptySlot || lpTamer->addItemToInventory(lpItem)	!=	eUIR_OK)
			{
				for (;i<dOWN_ITEM_COUNT;i++)
				{
					CDroppedItem	droppedItem;

					lpItem		=	&m_aItems[i];

					if (lpItem->m_wBaseItem	==	0xffff)
						continue;

					droppedItem.m_item.copy(lpItem);
					droppedItem.m_wOwner			=	lpTamer->m_wSerialInField;
					droppedItem.m_pos.x				=	lpTamer->m_pos.x+random(40)-20;
					droppedItem.m_pos.y				=	lpTamer->m_pos.y+random(40)-20;
					droppedItem.m_wSealedTime		=	6000;
					droppedItem.m_wDropMethod		=	eDIM_PLAYER_DROP;

					m_lpField->dropItem(&droppedItem);
					lpItem->reset();
				}

				lpTamer->sendRegistServerMessage(eRSM_CAN_NOT_RECEIVE_ITEM_BY_INVENTORY_FULL,eSM_SYSTEM);

				break;
			}
			else
			{
				int	iRequireLevel	=	itemForLog.getRequireLevel();

				if (iRequireLevel	>=	50 || itemForLog.isUniqueItem() || itemForLog.isRareItem())
					lpTamer->sendPickItemLog(&itemForLog,ePIM_BEAST_RETURN);

				memcpy(&sgPacket.item,&itemForLog,sizeof(cItem));
				sgPacket.wResult	=	eUIR_OK;

				g_userPM.add(lpTamer->m_iClientSerial,&sgPacket,sgPacket.base.wSize);

				if	(lpParty)
					lpParty->sendPickItemMessage(lpTamer,&itemForLog);
			}
		}
}

void
cACTOR::downGradeSummonedBeast()
{
	cACTOR		*lpTamer=	getTamer();

	if	(!lpTamer)
		return;

	if	(m_wSummonBeastGrade	<	1)
	{
		bookingDeath(NULL,TRUE);
		lpTamer->sendRegistServerMessage(eRSM_RELEASE_SUMMON_BEAST_BY_LOW_CP,eSM_SYSTEM);

		lpTamer->m_aSummonBeast[m_wPetIndex].m_bf10Level	=	0;

		return;
	}

	m_wSummonBeastGrade--;

	buildSummonBeast(m_awSBLevel[m_wSummonBeastType][m_wSummonBeastGrade],m_iHP,m_wSummonBeastType,m_wSummonBeastGrade,m_wTamer,m_dwTamerNameHashCode);

	lpTamer->updateSummonBeastInfo(this);
	lpTamer->sendSummonBeastInfo(m_wPetIndex,TRUE);
	syncTamedMonsterHP(TRUE);
	addSendSimpleInfo(NULL,TRUE);

	lpTamer->sendRegistServerMessage(eRSM_SUMMON_BEAST_GRADE_DOWN_BY_LOW_CP,eSM_SYSTEM);
}