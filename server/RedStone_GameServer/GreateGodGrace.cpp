#include "cActor.h"
#include "cFIELD.H"
#include "packetManager.h"
#include "debugCode.h"

#define dGGG_COOLTIME_FOR_ATTACK			10
#define dGGG_COOLTIME_FOR_DEFECE			10

enum{
	eGGG_ATTRIBUTE_NONE,
	eGGG_ATTRIBUTE_FIRE,
	eGGG_ATTRIBUTE_WATER,
	eGGG_ATTRIBUTE_WIND,
	eGGG_ATTRIBUTE_EARTH,
	eGGG_ATTRIBUTE_END
};

enum{
		eINCRECE_VALUE1_ACITECHANCE,	//발동 확률. * 100 
		eINCRECE_VALUE1,				//효과 수치 * 100 
		eINCRECE_VALUE1_EFFECT,			//효과 EX>불 : 데미지 비례 추가 공격
		eINCRECE_VALUE2_ACITECHANCE,	//발동 확률. * 100 
		eINCRECE_VALUE2,				//효과 수치  * 100
		eINCRECE_VALUE2_EFFECT,			//효과
		eINCRECE_VALUE_ETC,				//기타 등등 효과... 주로 지속 시간.
		eINCRECE_END
};

#define dGGG_EFFECT_NONE					0x0000
#define dGGG_EFFECT_CP_MAX_SELF				0x0001
#define dGGG_EFFECT_RETURN_FIRE_DAMAGE		0x0002
#define dGGG_EFFECT_DECREASE_MOVE_SPEED		0x0004
#define dGGG_EFFECT_DECREASE_ATTACK_SPEED	0x0008
#define dGGG_EFFECT_STUN					0x0010
#define dGGG_EFFECT_DECREASE_DEFENCE_POWER	0x0020
#define dGGG_EFFECT_ADD_DAMAGE				0x0040
#define dGGG_EFFECT_IGNORE_ONE_ATTACK_SELF	0x0080
#define dGGG_EFFECT_INCREASE_SPEED_SELF		0x0100
#define dGGG_EFFECT_RETURN_DAMAGE			0x0200

//		0xffffffff 는 공식이나 에 따른다.

int l_iaGGGBattleTable[eGGG_ATTRIBUTE_END][eINCRECE_END] = {
	{0,0,0},
	{1000,1000	,dGGG_EFFECT_ADD_DAMAGE , 1000 , 0xffffffff , dGGG_EFFECT_RETURN_FIRE_DAMAGE ,0 },
	{1000,2000	,dGGG_EFFECT_DECREASE_MOVE_SPEED |  dGGG_EFFECT_DECREASE_ATTACK_SPEED , 500 , 0 , dGGG_EFFECT_IGNORE_ONE_ATTACK_SELF,5},
	{1500,30	,dGGG_EFFECT_STUN		, 500	, 22500 , dGGG_EFFECT_INCREASE_SPEED_SELF,5},
	{1000,1000	,dGGG_EFFECT_DECREASE_DEFENCE_POWER , 1000	, 3000 , dGGG_EFFECT_RETURN_DAMAGE,5},
};

int l_iaGGGSkillUpTable[eGGG_ATTRIBUTE_END][eINCRECE_END] = {
	{0,0,0},
	{0 ,30,dGGG_EFFECT_NONE, 0 ,125,dGGG_EFFECT_NONE},
	{0 ,30,dGGG_EFFECT_NONE, 0 ,125,dGGG_EFFECT_NONE},
	{0 ,30,dGGG_EFFECT_NONE, 0 ,125,dGGG_EFFECT_NONE},
	{0 ,30,dGGG_EFFECT_NONE, 0 ,125,dGGG_EFFECT_NONE},
};


void			
cACTOR::setGGGValueForSkillUp()
{
	if(m_bf3GreateGodGrace == 0)
		return;
	
	//증가치 설정
	m_GGG.m_wGGGValue1ForSkillUp = l_iaGGGSkillUpTable[m_bf3GreateGodGrace][eINCRECE_VALUE1];
	m_GGG.m_wGGGValue2ForSkillUp = l_iaGGGSkillUpTable[m_bf3GreateGodGrace][eINCRECE_VALUE2];
	
	//레벨에 따라 적당한 시간 값(초)을 세팅한다.
	int iTime = 0;
	switch (m_iLevel / 100)
	{
	case 0:	
		iTime = 30;					//1~99
		break;
	case 1:
		iTime = 60;					//100~199
		break;
	case 2:
		iTime = 90;					//200~299
		break;
	case 3:
		iTime = 120;					//300~399
		break;
	case 4:
		iTime = 150;					//400~499
		break;
	case 5:
		iTime = 300;					//500~599
		break;
	case 6:
		iTime = 330;					//600~699
		break;
	case 7:
		iTime = 360;					//700~799
		break;
	case 8:
		iTime = 420;					//800~899					
		break;
	case 9:
		iTime = 450;					//900~999					
		break;
	}

	m_GGG.m_wGGGTimeForSkillUp = iTime;
	
	addSendActorMagicEffectStatus();
	sendGGGSkillUp();
}


BOOL
cACTOR::operateGGG()												//일반 상황에서 버프효과를 위해 호출.
{
	if(m_wTransformationTime)
		return FALSE;

	if (m_GGG.m_wGGGTimeForSkillUp)				//스킬업 시간이 있으면.
		operateGGGEffectForSkillUp();

	return TRUE;
}


BOOL
cACTOR::operateGGG(CHitInfo* _lpHitInfo, cACTOR* _lpHitter)	//전투시 적용하는 효과들.. // _lpHitter = 피격자. 맞은엑터
{
	if(m_wTransformationTime)
		return FALSE;

	if(isActiveSurvivalInstinctTitle())
	{
		BOOL isBuild = FALSE;
		if(m_GGG.m_wGGGTimeForSkillUp)
			isBuild = TRUE;

		m_GGG.ResetGGGSkill();
		sendGGGSkillUp(TRUE);
		addSendActorMagicEffectStatus();

		if(isBuild)			//스킬 버프업 시간이 있는 상태인데 생존본능 상태에서 때렷다. 버프 날리고 케릭 다시 세팅.
			buildPower();

		return FALSE;
	}

	if(_lpHitInfo->isLuckyHit() || _lpHitInfo->isBlock())			//블럭이나 럭키 히트는 예외임..
		return FALSE;

	operateGGGBattleEffect1(_lpHitInfo , _lpHitter);	//피격 효과.
	operateGGGBattleEffect2(_lpHitInfo , _lpHitter);	//타격 효과.
	
	return TRUE;
}

void
cACTOR::operateGGGBattleEffect1(CHitInfo* _lpHitInfo , cACTOR* _lpHitter)		//싸울때 적용되는 버프.		_lpHitter = 피격자.
{
	int iActiveChance2 = l_iaGGGBattleTable[_lpHitter->m_bf3GreateGodGrace][eINCRECE_VALUE2_ACITECHANCE];
	DWORD dwEffectType = 0;
	BOOL bIsActiveEffect = TRUE;
	int		extraInfo = 0;	//부가 정보를 임시적으로 기록한다.// 바람은총 힐량을 기록하기 위해...

	if(_lpHitInfo->m_dwPhysicalDamage == 0 && _lpHitInfo->m_dwMagicDamage == 0)
		return;

	//레벨 차이가 50이상 나면 발동 안한다.
	int iLevelGap = _lpHitter->m_iLevel - m_iLevel;
	
	if((iLevelGap > 50 || iLevelGap < -50 ))
		return;

	if(_lpHitter->m_GGG.m_wGGGDefenceCoolTime != 0)
		bIsActiveEffect = FALSE;

//	if(g_bIsChansTestServer)
//	{
//		iActiveChance2 = 10000;
//		bIsActiveEffect = TRUE;
//	}

	//맞을때 발동하는 버프..
	if(_lpHitInfo->isHit())
	{
		switch (_lpHitter->m_bf3GreateGodGrace )				//효과에 따른 적용...
		{
			case eGGG_ATTRIBUTE_FIRE:	//피격시 20%확률로 데미지 공격
			{
				//공식에 의해서 데미지를 입힌다.(12 + 2 * 유저레벨 /2 +- 7)
				int iActiveValue = (12 + 2 * _lpHitter->m_iLevel / 2 - random(7*2));
				
				if	(!isTargetInAttackRange(_lpHitter,dRETURN_DAMAGE_RANGE))
					break;

				if	(_lpHitInfo->m_dwPhysicalDamage < _lpHitInfo->m_dwMagicDamage)
					break;
				
				if(iActiveChance2 >= random(10000))
				{
					_lpHitter->addReturnDamageRateIDEF(iActiveValue);			//방어력 적용 안하는 데미지 반사
					_lpHitter->m_GGG.m_wGGGDefenceCoolTime = dGGG_COOLTIME_FOR_DEFECE;	//쿨타임적용.
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_HIT_BY;				//이펙트 출력을 위한 변수 세팅.
				}
				
				break;
			}
			case eGGG_ATTRIBUTE_WATER:	//피격시 10%확률로 다음공격 완전 미스.
			{
				if (_lpHitter->m_ImmuneOneTime)
				{
					_lpHitter->m_ImmuneOneTime = FALSE;
					_lpHitInfo->reset(TRUE);
					break;
				}
				else if(iActiveChance2 >= random(10000) && bIsActiveEffect)
				{
					_lpHitter->SetImmuneOneTime();			//한방 무적 세팅.
					_lpHitter->m_GGG.m_wGGGDefenceCoolTime = dGGG_COOLTIME_FOR_DEFECE;
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_HIT_BY;
				}
				
				break;
			}
			case eGGG_ATTRIBUTE_WIND:	//피격시 5%확률로 이동속도 225%증가 및 HP회복 버프.
			{
				int iTimeForMoveSpeedUp = l_iaGGGBattleTable[_lpHitter->m_bf3GreateGodGrace][eINCRECE_VALUE_ETC];
				//(2000 + 2 * g_hero.m_iLevel / 2 - 1100) / 100;
				int iHealPoint			= (2000 + 2 * _lpHitter->m_iLevel / 2 + 1100 - random(2200)) / 100;

				if(iActiveChance2 >= random(10000) && bIsActiveEffect)
				{
					extraInfo = _lpHitter->getMaxHP() * iHealPoint / 100;
					_lpHitter->healHP(extraInfo);	//HP힐
					_lpHitter->m_moveSpeed.correctValue(225,iTimeForMoveSpeedUp * dSYNC_FPS);	//이속 증가.
					_lpHitter->m_GGG.m_wGGGDefenceCoolTime = dGGG_COOLTIME_FOR_DEFECE;
					_lpHitter->buildPower();
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_HIT_BY;
				}
				break;
			}
			case eGGG_ATTRIBUTE_EARTH:
			{
				int iReturnDamage	= l_iaGGGBattleTable[_lpHitter->m_bf3GreateGodGrace][eINCRECE_VALUE2] / 100;
				int iDamage			= max(_lpHitInfo->m_dwPhysicalDamage , _lpHitInfo->m_dwMagicDamage);
				
				iReturnDamage = iDamage * iReturnDamage / 100 ;

				iReturnDamage = max(iReturnDamage , 100);

				if	(!isTargetInAttackRange(_lpHitter,dRETURN_DAMAGE_RANGE))
					break;
				
				if(iActiveChance2 >= random(10000) && bIsActiveEffect)
				{
					_lpHitter->addReturnDamageRateIDEF(iReturnDamage / 100);	//뎀쥐 리턴 방어력 무시하고.
					_lpHitter->m_GGG.m_wGGGDefenceCoolTime = dGGG_COOLTIME_FOR_DEFECE;
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_HIT_BY;
				}
				break;
			}
		}
	}

	if(dwEffectType)
	{
		SendGGGEffectPlay(_lpHitter , (WORD)dwEffectType , extraInfo/100);//이펙트 출력.
	}
}

void
cACTOR::operateGGGBattleEffect2(CHitInfo* _lpHitInfo , cACTOR* _lpHitter)		//싸울때 적용되는 버프.		_lpHitter = 피격자.
{
	//때릴때 발동 한다.
	DWORD	dwDamage = 0;
	BOOL	isPhysicalDamage	= FALSE;
	int		iActiveChance1		= l_iaGGGBattleTable[m_bf3GreateGodGrace][eINCRECE_VALUE1_ACITECHANCE];
	int		iActiveChance2		= l_iaGGGBattleTable[m_bf3GreateGodGrace][eINCRECE_VALUE2_ACITECHANCE];
	int		iTime				= l_iaGGGBattleTable[m_bf3GreateGodGrace][eINCRECE_VALUE_ETC];
	DWORD	dwEffectType		= 0;
	DWORD	correctEffectValue	= 100;
	BOOL	bIsActiveBattleEffect = TRUE;

	cJOB*	lpJob = getJob();

	if(lpJob == NULL)
		return;

	switch (lpJob->m_wMonsterLevel)
	{
	case cJOB::eML_SEMI_BOSS1:
	case cJOB::eML_SEMI_BOSS2:
	case cJOB::eML_SEMI_BOSS3:
		correctEffectValue = 75;
		break;
	case cJOB::eML_BOSS1:
	case cJOB::eML_BOSS2:
	case cJOB::eML_BOSS3:
		bIsActiveBattleEffect = FALSE;
		correctEffectValue = 0;
		break;
	}

	if (_lpHitter->m_wIsSetBoss)
		bIsActiveBattleEffect = FALSE;
	
	//레벨 차이가 50이상 나면 발동 안한다.
	int iLevelGap = _lpHitter->m_iLevel - m_iLevel;

	//50렙레 이상 차이나거나 쿨타임 시간이 있으면 발동 안함. 불의 은총은 타격 쿨타임이 없다.
	if(iLevelGap > 50 || iLevelGap < -50 || 
		(m_GGG.m_wGGGAttackCoolTime != 0 && m_bf3GreateGodGrace != eGGG_ATTRIBUTE_FIRE))	
		bIsActiveBattleEffect = FALSE;

	dwDamage = max(_lpHitInfo->m_dwPhysicalDamage , _lpHitInfo->m_dwMagicDamage);	//물리와 마법중에 높은값을 선택한다.

	iActiveChance1 = iActiveChance1 * correctEffectValue / 100;

	if (g_bIsChansTestServer)
	{
		iActiveChance1	=	10000;
		bIsActiveBattleEffect = TRUE;
	}

	//공격할때의 버프.	
	if(_lpHitInfo->isHit())
	{
		switch (m_bf3GreateGodGrace)				//효과에 따른 적용...
		{
			//타격시 10%확률로 데미지 비례 추가 공격 10%
			case eGGG_ATTRIBUTE_FIRE:
			{
				int iAddDamage = l_iaGGGBattleTable[m_bf3GreateGodGrace][eINCRECE_VALUE1] * correctEffectValue / 100;
				
				if(iActiveChance1 >= random(10000) && bIsActiveBattleEffect)
				{
					DWORD dwFireDamage = (dwDamage * iAddDamage / 100 ) / 100;		//뎀쥐 추가.
					m_GGG.m_wGGGAttackCoolTime = dGGG_COOLTIME_FOR_ATTACK;	
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_HIT;
					_lpHitInfo->m_dwMagicDamage += dwFireDamage;
				}

				//이걸 거치면 _lpHitInfo에 스킬업공격력 추가효과를 세팅해 준다.
				if(GetAddMagicDamage(_lpHitInfo , _lpHitter , dwDamage , m_bf3GreateGodGrace , &dwEffectType))
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_POINT_UP;
				break;
			}
			case eGGG_ATTRIBUTE_WATER:
			{
				if(!_lpHitter->m_wGGGDebufTimeByWaterGGG)
				{
					int iValueForDcreace = l_iaGGGBattleTable[m_bf3GreateGodGrace][eINCRECE_VALUE1] / 100 * correctEffectValue / 100;
					
					if(iActiveChance1 >= random(10000) && bIsActiveBattleEffect)
					{
						//디버프 시간 세팅.
						_lpHitter->m_wGGGDebufTimeByWaterGGG = l_iaGGGBattleTable[m_bf3GreateGodGrace][eINCRECE_VALUE_ETC];
						_lpHitter->m_moveSpeed.correctValue(-iValueForDcreace  , iTime * dSYNC_FPS);			//때린넘의 이속과 공속을 떨어뜨린다.
 						_lpHitter->m_actionSpeed.correctValue(-iValueForDcreace  , iTime);	
						m_GGG.m_wGGGAttackCoolTime = dGGG_COOLTIME_FOR_ATTACK;
						dwEffectType |= dGGG_EFFECT_PLAYTYPE_HIT;
						
					}
				}
					
				if(GetAddMagicDamage(_lpHitInfo , _lpHitter , dwDamage , m_bf3GreateGodGrace , &dwEffectType))
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_POINT_UP;
				break;
			}
			case eGGG_ATTRIBUTE_WIND:
			{
				int iValueForStun	= l_iaGGGBattleTable[m_bf3GreateGodGrace][eINCRECE_VALUE1] * correctEffectValue / 100;
				
				if(iActiveChance1 >= random(10000) && bIsActiveBattleEffect)
				{
					_lpHitter->m_wStunTime = max(iValueForStun , _lpHitter->m_wStunTime);	//스턴 타임 세팅.
					m_GGG.m_wGGGAttackCoolTime = dGGG_COOLTIME_FOR_ATTACK;
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_HIT;
				}

				if(GetAddMagicDamage(_lpHitInfo , _lpHitter , dwDamage , m_bf3GreateGodGrace , &dwEffectType))
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_POINT_UP;
				break;
			}
			case eGGG_ATTRIBUTE_EARTH:
			{
				int iValueForDcreaseDEF = l_iaGGGBattleTable[m_bf3GreateGodGrace][eINCRECE_VALUE1] * correctEffectValue / 100;
				
				if(iActiveChance1 >= random(10000) && bIsActiveBattleEffect)
				{
					_lpHitter->m_defensePower.correctPercentageValue(-iValueForDcreaseDEF / 100 , iTime); //방어력 하강.
					m_GGG.m_wGGGAttackCoolTime = dGGG_COOLTIME_FOR_ATTACK;
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_HIT;
				}

				if(GetAddMagicDamage(_lpHitInfo , _lpHitter , dwDamage , m_bf3GreateGodGrace , &dwEffectType))
					dwEffectType |= dGGG_EFFECT_PLAYTYPE_POINT_UP;
				break;
			}
		}
	}

	if(dwEffectType)
		SendGGGEffectPlay(_lpHitter , dwEffectType);
}

int
cACTOR::GetDecreaseByWaterGGGValue()
{
	return l_iaGGGBattleTable[eGGG_ATTRIBUTE_WATER][eINCRECE_VALUE1] / 100;
}

void
cACTOR::operateGGGEffectForSkillUp()
{
	switch (m_bf3GreateGodGrace)				//효과에 따른 적용...
	{
	case eGGG_ATTRIBUTE_FIRE:
		m_moveSpeed.addValue(m_GGG.m_wGGGValue2ForSkillUp);				//이속 증가.
		break;
	case eGGG_ATTRIBUTE_WATER:
		m_moveSpeed.addValue(m_GGG.m_wGGGValue2ForSkillUp);				//이속 증가.
		break;
	case eGGG_ATTRIBUTE_WIND:
		m_moveSpeed.addValue(m_GGG.m_wGGGValue2ForSkillUp);				//이속 증가.
		break;
	case eGGG_ATTRIBUTE_EARTH:
		m_moveSpeed.addValue(m_GGG.m_wGGGValue2ForSkillUp);				//이속 증가.
		break;
	}
}


//엑터에 은총을 부여 한다. -_-; 머 그냥 값을 세팅한다.
void
cACTOR::addGGG(int _iGrace)
{
	if(!_iGrace)
		return;

	m_bf3GreateGodGrace = _iGrace;
	m_GGG.ResetGGG();
	sendGetGGG();
	addSendActorMagicEffectStatus();
	sendGGGSkillUp(TRUE);
}

//은총 관련 정보를 보낸다.[3]번부터 쓴거는 중간에 레벨 은총이 삭제 되서...-_-  미안;
void 
cACTOR::SendGGGInfo()
{
	SG_GGG_INFO	packet;
	ZeroMemory(&packet , sizeof(packet));

	if	(!isPlayer())
		return;

	packet.base.set(sizeof(packet),dSG_GGG_INFO);
	packet.wValue[3] =	m_GGG.m_wGGGTimeForSkillUp;			
	packet.wValue[4] =	m_GGG.m_wGGGValue1ForSkillUp;			
	packet.wValue[5] =	m_GGG.m_wGGGValue2ForSkillUp;					
}


//스킬업 햇다고 클라이언트에게 알려준다.
void
cACTOR::sendGGGSkillUp(BOOL _isEndMessage)
{
	SG_GGG_INFO	packet;
	ZeroMemory(&packet , sizeof(packet));
	
	if	(!isPlayer())
		return;
	
	if(_isEndMessage)
	{
		packet.base.set(sizeof(packet),dSG_GGG_SKILL_UP_TIME_END);
		buildPower();
		addSendActorMagicEffectStatus(TRUE , _isEndMessage);
	}
	else
	{	
		packet.base.set(sizeof(packet),dSG_ADD_GGG_SKILL_UP);
		packet.wValue[0] =	m_GGG.m_wGGGTimeForSkillUp;			//time
		packet.wValue[1] =	m_GGG.m_wGGGValue1ForSkillUp;			
		packet.wValue[2] =	m_GGG.m_wGGGValue2ForSkillUp;			

		addSendActorMagicEffectStatus(TRUE);
	}
	
	addSendActorMagicEffectStatus();
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//은총을 획득햇다고 클라이언트에 통보한다.
void					
cACTOR::sendGetGGG()
{
	SG_GGG_INFO	packet;
	ZeroMemory(&packet , sizeof(packet));

	if	(!isPlayer())
		return;

	packet.base.set(sizeof(packet),dSG_ADD_GGG);
	packet.wValue[0] = m_bf3GreateGodGrace;

	addSendActorMagicEffectStatus();
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//은총을 제거할거라고 통보.
void					
cACTOR::SendRemoveGGG()
{
	SG_GGG_INFO	packet;
	ZeroMemory(&packet , sizeof(packet));

	if	(!isPlayer())
		return;

	packet.base.set(sizeof(packet),dSG_REMOVE_GGG);
	
	addSendActorMagicEffectStatus();
	g_userPM.add(m_iClientSerial,&packet,packet.base.wSize);
}

//이펙트를 뿌리라고 통보한다.
void
cACTOR::SendGGGEffectPlay(cACTOR* _lpHitter , WORD _wType , int _iHealPoint)
{
	SG_GGG_INFO	packet;
	ZeroMemory(&packet , sizeof(packet));
	//0 타입 비트 마스크 - 어떤 종류의 이펙트를 플레이 할거냐?
	//1 피격자의 필드 시리얼
	//2 공격자의 은총 
	//3 공격자의 필드 시리얼.
	//4 피격자의 은총.
	//5 부가 정보

	packet.base.set(sizeof(packet),dSG_PLAY_GGG_EFFECT);

	packet.wValue[0]	=	_wType;											//비트 마스크한 정보를 보낸다.
	packet.wValue[1]	=	_lpHitter->m_wSerialInField;
	packet.wValue[2]	=	m_bf3GreateGodGrace;
	packet.wValue[3]	=	m_wSerialInField;
	packet.wValue[4]	=	_lpHitter->m_bf3GreateGodGrace;
	if(_iHealPoint)
		packet.wValue[5]	=	_iHealPoint;
	
	m_lpField->addSendPacket((ALL_MSG*)&packet,m_wSerialInField);
}

//이런 저런 조건을 검사해서 _lpHitInfo에 스킬포인트 업시 데미지 증가 효과를 부여하던지 말던지 한다.
BOOL
cACTOR::GetAddMagicDamage(CHitInfo* _lpHitInfo , cACTOR* _lpHitter , DWORD _dwDamge , int _attribute , DWORD* _dwEffectType)
{
	if(!m_GGG.m_wGGGTimeForSkillUp)
		return FALSE;

	//추가 데미지
	int iMagicDamage	 =	_dwDamge *  m_GGG.m_wGGGValue1ForSkillUp / 100;
	int	iLevelCorrectDamagePercentage = 0;
	BOOL bIsBossBattle = FALSE;
	
	if	(iMagicDamage	==	0)
		return FALSE;
	
	if	(_lpHitter->m_wIsBoss	||	m_wIsBoss)
		bIsBossBattle	=	TRUE;
	
	if	(bIsBossBattle)
		iLevelCorrectDamagePercentage	=	(_lpHitter->m_iLevel-m_iLevel)*30/25;

	int	iCorrectDamage = 0;

	switch (_attribute)
	{
	case eGGG_ATTRIBUTE_FIRE:
		iCorrectDamage = _lpHitter->getStrengtheningFireDamage(m_wAlchemyTitleFactor,TRUE)+iLevelCorrectDamagePercentage;
		break;
	case eGGG_ATTRIBUTE_WATER:
		iCorrectDamage = _lpHitter->getStrengtheningWaterDamage(m_wAlchemyTitleFactor,TRUE)+iLevelCorrectDamagePercentage;
		break;
	case eGGG_ATTRIBUTE_WIND:
		iCorrectDamage = _lpHitter->getStrengtheningWindDamage(m_wAlchemyTitleFactor,TRUE)+iLevelCorrectDamagePercentage;
		break;
	case eGGG_ATTRIBUTE_EARTH:
		iCorrectDamage = _lpHitter->getStrengtheningEarthDamage(m_wAlchemyTitleFactor,TRUE)+iLevelCorrectDamagePercentage;
		break;
	default:
		return FALSE;
		break;
	}
	
	if	(iLevelCorrectDamagePercentage)
	{
		int	iRealDamage	=	iMagicDamage+iMagicDamage*(iCorrectDamage-iLevelCorrectDamagePercentage)/100;
		iMagicDamage		+=	iMagicDamage*iCorrectDamage/100;
		
		if	(iMagicDamage<=	iRealDamage*40/100)
			iMagicDamage	=	iRealDamage*40/100;
		if	(iMagicDamage>=	iRealDamage*180/100)
			iMagicDamage	=	iRealDamage*180/100;
	}
	else
		iMagicDamage		+=	iMagicDamage*iCorrectDamage/100;	

	switch (_attribute)
	{
	case eGGG_ATTRIBUTE_FIRE:
		iMagicDamage		-=	iMagicDamage*_lpHitter->getFireResistance(getWeakningTargetFireResistance())/100;
		break;
	case eGGG_ATTRIBUTE_WATER:
		iMagicDamage		-=	iMagicDamage*_lpHitter->getWaterResistance(getWeakningTargetWaterResistance())/100;
		break;
	case eGGG_ATTRIBUTE_WIND:
		iMagicDamage		-=	iMagicDamage*_lpHitter->getWindResistance(getWeakningTargetWindResistance())/100;
		break;
	case eGGG_ATTRIBUTE_EARTH:
		iMagicDamage		-=	iMagicDamage*_lpHitter->getEarthResistance(getWeakningTargetEarthResistance())/100;
		break;
	default:
		return FALSE;
		break;
	}
	
	_lpHitInfo->m_dwMagicDamage += iMagicDamage;

	if(_lpHitInfo->m_dwMagicDamage > dMAX_MAGICAL_DAMAGE)
		_lpHitInfo->m_dwMagicDamage = dMAX_MAGICAL_DAMAGE;

	return TRUE;
}