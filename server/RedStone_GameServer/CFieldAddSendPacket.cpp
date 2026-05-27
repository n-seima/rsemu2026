#include "cFIELD.H"
#include "cGAME.H"
#include "Client.H"
#include "instance_guild_battle_manager.H"
#include "packetManager.H"
#include "astDoubleTarget.H"
#include "astContinuousHitOfDoppelganger.H"
#include "astContinuousBit.H"
#include "astBouncingMissile.h"
#include "astWideMissile.h"

void
FillChattingBuffer(SG_CHAT *_lpstrPacket,char *_lpstrName,char *_lpstrMessage)
{
	if	(_lpstrMessage[0] == 'a' && _lpstrMessage[1] == '*' && _lpstrMessage[2] == '3' &&
		_lpstrMessage[3] == '7' && _lpstrMessage[4] == '6' && _lpstrMessage[5] == '@')
		return;

	int	iNameLength		=	min(strlen(_lpstrName)+1,dNAME_LENGTH);
	int	iMessageLength	=	min(strlen(_lpstrMessage)+1,dCHAT_LENGTH-1);

	strcpy(_lpstrPacket->strMessage,_lpstrName);
	strncpy(_lpstrPacket->strMessage+iNameLength,_lpstrMessage,dCHAT_LENGTH-1);

	_lpstrPacket->base.wSize	=	sizeof(SG_CHAT)-sizeof(_lpstrPacket->strMessage)+iNameLength+iMessageLength;
	_lpstrPacket->base.wType	=	dSG_CHAT;
}

void
cFIELD::addSendAttackToObject(cACTOR *_lpCaster,int _iTarget,int _iObjectType,int _iRemainDurability)
{
	SG_ATTACK_TO_OBJECT	packet;

	packet.base.set(sizeof(packet),dSG_ATTACK_TO_OBJECT);

	packet.wObjectIndex		=	_iTarget;
	packet.bf11Attacker		=	_lpCaster->m_wSerialInField;
	packet.bf4ObjectType	=	_iObjectType;
	packet.wPhysicalDamage	=	(WORD)(_lpCaster->m_attackInfo.m_aInfo[0].m_dwPhysicalDamage);	//	얼마나?
	packet.wMagicDamage		=	(WORD)(_lpCaster->m_attackInfo.m_aInfo[0].m_dwMagicDamage);		//	얼마나?
	packet.bf8RemainDurability=	_iRemainDurability;
	packet.bf6FPS			=	(WORD)_lpCaster->m_iFPS;		//	위치

	packet.wSkill			=	_lpCaster->m_attackInfo.m_ability.m_wSkill;
	packet.wPosX			=	_lpCaster->m_pos.x;
	packet.wPosY			=	_lpCaster->m_pos.y;


	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	누군가-_-를 근접 공격한다.
void
cFIELD::sendBasicAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility)
{
	if (!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	SG_BASIC_ATTACK	packet;

	packet.base.set(sizeof(SG_BASIC_ATTACK),dSG_BASIC_ATTACK);
	
	packet.bf11Serial		=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11TargetSerial	=	_lpTarget->m_wSerialInField;	//	누가-_-?
	packet.bf10Skill		=	_lpAbility->m_wSkill&0x3ff;		//	뭘로.. -o-
	if	(_lpAbility->m_wSkill	>=	0x400)
		packet.bf1Skill		=	1;		//	뭘로.. -o-
	else
		packet.bf1Skill		=	0;
	packet.bf7Level			=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.wPosX			=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY			=	_lpCaster->m_pos.y;		//	위치
	packet.iPhysicalDamage =	(_lpCaster->m_attackInfo.m_aInfo[0].m_dwPhysicalDamage/100);	//	얼마나?
	packet.iMagicDamage 	=	(_lpCaster->m_attackInfo.m_aInfo[0].m_dwMagicDamage/100);		//	얼마나?
	packet.dwResultField		=	(DWORD)(_lpCaster->m_attackInfo.m_aInfo[0].m_dwResultField);
	packet.bf6FPS			=	(WORD)_lpCaster->m_iFPS;		//	위치
	packet.bf1MiniPetCharge1	=	_lpCaster->m_aActiveMiniPetInfo[0].m_wIsCharged;
	packet.bf1MiniPetCharge2	=	_lpCaster->m_aActiveMiniPetInfo[1].m_wIsCharged;

	if	(_lpCaster->m_attackInfo.m_aInfo[0].m_dwPhysicalDamage)
		packet.iPhysicalDamage	=	max(packet.iPhysicalDamage,1);

	if	(_lpCaster->m_attackInfo.m_aInfo[0].m_dwMagicDamage)
		packet.iMagicDamage		=	max(packet.iMagicDamage,1);

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	랜드 마커 어택 전송
void
cFIELD::sendLandMarkerAttack(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iShotCount)
{
	if (!checkActor(_lpCaster))	return;

	SG_LAND_MARKER		packet;

	packet.base.set(sizeof(SG_LAND_MARKER),dSG_LAND_MARKER);

	packet.wSerial				=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.wSkill				=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.wLevel				=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.wTargetPosX			=	_iX;					//	목표 위치
	packet.wTargetPosY			=	_iY;					//	목표 위치
	packet.bFPS					=	_lpCaster->m_iFPS;		//	FPS
	packet.bShotCount			=	_iShotCount;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

void
cFIELD::sendStrikeIntervalShooter(cACTOR *_lpTarget,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,int _iShotCount)
{
	if (!checkActor(_lpTarget))	return;

	SG_STRIKE_INTERVAL_SHOOTER	packet;

	packet.base.set(sizeof(SG_STRIKE_INTERVAL_SHOOTER),dSG_STRIKE_INTERVAL_SHOOTER);

	packet.wTargetSerial		=	_lpTarget->m_wSerialInField;	//	누구를-_-?
	packet.wSkill				=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.wPosX				=	_lpTarget->m_pos.x;		//	위치
	packet.wPosY				=	_lpTarget->m_pos.y;		//	위치
	packet.iPhysicalDamage		=	_iPhysicalDamage/100;
	packet.iMagicDamage			=	_iMagicDamage/100;
	packet.wShotCount			=	_iShotCount;

	if	(_iPhysicalDamage)	
		packet.iPhysicalDamage	=	max(packet.iPhysicalDamage,1);
	if	(_iMagicDamage)		
		packet.iMagicDamage		=	max(packet.iMagicDamage,1);

	addSendPacket((ALL_MSG*)&packet,_lpTarget->m_wSerialInField);
}

void
cFIELD::sendIntervaslShooterAttack(cACTOR *_lpCaster,cAbility *_lpAbility,int _iShotCount)
{
	if (!checkActor(_lpCaster))	return;

	SG_CAST_INTERVAL_SHOOTER		packet;

	packet.base.set(sizeof(SG_CAST_INTERVAL_SHOOTER),dSG_CAST_INTERVAL_SHOOTER);

	packet.wSerial				=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.wSkill				=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.wLevel				=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.bFPS					=	_lpCaster->m_iFPS;
	packet.bShotCount			=	_iShotCount;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	연속 히트 시키는 공격
void
cFIELD::sendActionToGroup(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility)
{
	if (!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	cAS_DoubleTarget	*lpActiveSkill	=	(cAS_DoubleTarget	*)_lpCaster->getActiveSkill();

	if	(!lpActiveSkill)
		return;

	SG_ACTION_TO_GROUP	packet;

	packet.base.set(sizeof(SG_ACTION_TO_GROUP),dSG_ACTION_TO_GROUP);

	packet.bf11Serial			=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11Skill			=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.bf6FPS				=	_lpCaster->m_iFPS;		//	FPS
	packet.bf4TargetCount		=	lpActiveSkill->m_wTargetCount;
	packet.wRange				=	_lpCaster->getAttackRange(_lpAbility);
	packet.bf7ContinuousShotCount=	min(lpActiveSkill->m_bContinuousShotCount,31);
	packet.bf7MissCount			=	min(lpActiveSkill->m_bMissCount,31);
	packet.bf7CriticalCount		=	min(lpActiveSkill->m_bCriticalCount,31);
	packet.bf7DoubleCriticalCount=	min(lpActiveSkill->m_bDoubleCriticalCount,31);
	packet.bf7SkillLevel		=	_lpAbility->m_wLevel;
	packet.bf1MiniPetCharge1	=	_lpCaster->m_aActiveMiniPetInfo[0].m_wIsCharged;
	packet.bf1MiniPetCharge2	=	_lpCaster->m_aActiveMiniPetInfo[1].m_wIsCharged;
	packet.bf7HardBlowCount		=	min(lpActiveSkill->m_bHardBlowCount,31);

	for (int i=0;i<lpActiveSkill->m_wTargetCount;i++)
	{
		CHitInfo			*lpHitInfo	=	&_lpCaster->m_attackInfo.m_aInfo[i];

		packet.aData[i].wActor			=	lpActiveSkill->m_awTargetList[i];
		packet.aData[i].iPhysicalDamage 	=	(lpHitInfo->m_dwPhysicalDamage/100);
		packet.aData[i].iMagicDamage 	=	(lpHitInfo->m_dwMagicDamage/100);
		packet.aData[i].dwResultField	=	lpHitInfo->m_dwResultField;

		if (lpHitInfo->m_dwPhysicalDamage)
			packet.aData[i].iPhysicalDamage	=	max(packet.aData[i].iPhysicalDamage,1);
		if (lpHitInfo->m_dwMagicDamage)
			packet.aData[i].iMagicDamage	=	max(packet.aData[i].iMagicDamage,1);
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cSimpleDamageInfo)*lpActiveSkill->m_wTargetCount;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

void
cFIELD::sendContinuousActionToGroup(cACTOR *_lpCaster,cAbility *_lpAbility)
{

	if (!checkActor(_lpCaster))
		return;

	CActiveSkill*	lpActiveSkill	=	_lpCaster->getActiveSkill();

	if	(!lpActiveSkill)
		return;

	CSkill* lpSkill = lpActiveSkill->getSkill();

	if(!lpSkill || lpSkill->m_wType != eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER)
		return;
	
	cAS_ContinuousHitOfDoppelganger*	lpActiveCHODSkill	=	(cAS_ContinuousHitOfDoppelganger	*)lpActiveSkill;
		
	SG_ACTION_TO_GROUP	packet;

	packet.base.set(sizeof(SG_ACTION_TO_GROUP),dSG_ACTION_TO_GROUP);

	packet.bf11Serial			=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11Skill			=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.bf6FPS				=	_lpCaster->m_iFPS;		//	FPS
	packet.bf4TargetCount		=	lpActiveCHODSkill->m_wTargetCount;
	packet.wRange				=	_lpCaster->getAttackRange(_lpAbility);
	packet.bf7ContinuousShotCount=	min(lpActiveCHODSkill->m_wContinuousShotCount,127);
	packet.bf7MissCount			=	min(lpActiveCHODSkill->m_bMissCount,127);
	packet.bf7CriticalCount		=	min(lpActiveCHODSkill->m_bCriticalCount,127);
	packet.bf7DoubleCriticalCount=	min(lpActiveCHODSkill->m_bDoubleCriticalCount,127);
	packet.bf7SkillLevel		=	_lpAbility->m_wLevel;
	packet.bf1MiniPetCharge1	=	_lpCaster->m_aActiveMiniPetInfo[0].m_wIsCharged;
	packet.bf1MiniPetCharge2	=	_lpCaster->m_aActiveMiniPetInfo[1].m_wIsCharged;
	packet.bf7HardBlowCount		=	min(lpActiveCHODSkill->m_bHardBlowCount,127);

	if(lpActiveCHODSkill->m_wTargetCount == 0xffff)
		return;

	for (int i=0;i<lpActiveCHODSkill->m_wTargetCount && i <dMAX_CONTINUOUS_HIT_INFO_COUNT;i++)
	{
		CHitInfo			*lpHitInfo	=	&_lpCaster->m_attackInfo.m_aInfo[i];

		packet.aData[i].wActor			=	lpActiveCHODSkill->m_awTargetList[i];
		packet.aData[i].iPhysicalDamage 	=	(lpHitInfo->m_dwPhysicalDamage/100);
		packet.aData[i].iMagicDamage 	=	(lpHitInfo->m_dwMagicDamage/100);
		packet.aData[i].dwResultField	=	lpHitInfo->m_dwResultField;

		if (lpHitInfo->m_dwPhysicalDamage)
			packet.aData[i].iPhysicalDamage	=	max(packet.aData[i].iPhysicalDamage,1);
		if (lpHitInfo->m_dwMagicDamage)
			packet.aData[i].iMagicDamage	=	max(packet.aData[i].iMagicDamage,1);
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cSimpleDamageInfo)*lpActiveCHODSkill->m_wTargetCount;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);

}
//
//	연속 히트 시키는 공격
void
cFIELD::sendContinuousHitAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility)
{
	if (!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	SG_CONTINUOUS_HIT_BY_CHARACTER	packet;

	packet.base.set(sizeof(SG_CONTINUOUS_HIT_BY_CHARACTER),dSG_CONTINUOUS_HIT_BY_CHARACTER);

	packet.bf11Serial			=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11TargetSerial		=	_lpTarget->m_wSerialInField;	//	누구를-_-?
	packet.bf11Skill			=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.bf7Level				=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.bf6FPS				=	_lpCaster->m_iFPS;		//	FPS
	packet.iPhysicalDamage		=	0;
	packet.iMagicDamage			=	0;
	packet.bf5AttackCount		=	0;
	packet.bf4MissCount			=	0;
	packet.bf4CriticalCount		=	0;
	packet.bf4DoubleCriticalCount=	0;
	packet.bf1IsCrushBlow		=	FALSE;
	packet.bf1IsAttackToShakle	=	_lpCaster->isAttackToShakleActor(_lpTarget);
	packet.wMinPhysicalDamage	=	0xffff;
	packet.wMinMagicDamage		=	0xffff;
	packet.bf4BlockingCount		=	0;
	packet.bf4NoActionBlockingCount	=	0;
	packet.bf4DancingBlockCount	=	0;
	packet.bf4HardBlowCount		=	0;
	packet.bf1IsInstanceKill	=	FALSE;

	DWORD	dwPhysicalDamage	=	0;
	DWORD	dwMagicDamage		=	0;

	for (int i=0,iCount=0;i<dMAX_CONTINUOUS_HIT_INFO_COUNT;i++)
	{
		CHitInfo	*lpInfo		=	&_lpCaster->m_attackInfo.m_aInfo[i];

		if	(lpInfo->m_dwResultField	==	0xffffffff)
			break;

		if	(lpInfo->isMiss()	&&	packet.bf4MissCount<15)	
			packet.bf4MissCount	++;
		else
		if	(lpInfo->isBlock())
			packet.bf4BlockingCount++;
		else
		if	(lpInfo->isNoActionBlock())
			packet.bf4NoActionBlockingCount++;
		else
		if	(lpInfo->isDancingBlockerBlock())
			packet.bf4DancingBlockCount++;

		dwPhysicalDamage	+=	lpInfo->m_dwPhysicalDamage;
		dwMagicDamage		+=	lpInfo->m_dwMagicDamage;

		packet.wMinPhysicalDamage	=	min(packet.wMinPhysicalDamage,(WORD)(lpInfo->m_dwPhysicalDamage/100));
		packet.wMinMagicDamage		=	min(packet.wMinPhysicalDamage,(WORD)(lpInfo->m_dwMagicDamage/100));

		if	(lpInfo->isInstanceKill())
			packet.bf1IsInstanceKill	=	TRUE;

		if	(lpInfo->isCrush())
			packet.bf1IsCrushBlow		=	TRUE;

		if	(lpInfo->isCritical()	&&	packet.bf4CriticalCount	<	15)
			packet.bf4CriticalCount++;

		if	(lpInfo->isLuckyDoubleCritical()	&&	packet.bf4DoubleCriticalCount	<	15)
			packet.bf4DoubleCriticalCount++;

		if	(lpInfo->isHardBlow())
			++packet.bf4HardBlowCount;

		packet.bf5AttackCount++;
	}

	packet.iPhysicalDamage	=	(dwPhysicalDamage/100);
	packet.iMagicDamage		=	(dwMagicDamage/100);

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}


//
//	연속 히트 시키는 공격
void
cFIELD::sendContinuousHitAttackByValue(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,int _iShotCount,int _iMissCount,
										int _iCriticalCount,int _iDoubleCriticalCount,int _iMinPhysicalDamage,int _iMinMagicDamage,int _iBlockingCount,int _iInstanceKillCount,
										int _iNoActionBlockingCount,int _iDancingBlockCount, int _iHardBlowCount)
{
	if	(!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	SG_CONTINUOUS_HIT_BY_VALUE	packet;

	packet.base.set(sizeof(SG_CONTINUOUS_HIT_BY_VALUE),dSG_CONTINUOUS_HIT_BY_VALUE);

	packet.bf11Serial			=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11TargetSerial		=	_lpTarget->m_wSerialInField;	//	누구를-_-?
	packet.bf11Skill			=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.bf7Level				=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.bf6FPS				=	_lpCaster->m_iFPS;		//	FPS
	packet.bf5AttackCount		=	_iShotCount;
	packet.bf4MissCount			=	min(_iMissCount,15);
	packet.iPhysicalDamage		=	_iPhysicalDamage/100;
	packet.iMagicDamage			=	_iMagicDamage/100;
	packet.wMinPhysicalDamage	=	_iMinPhysicalDamage;
	packet.wMinMagicDamage		=	_iMinMagicDamage;
	packet.bf4BlockingCount		=	_iBlockingCount;
	packet.bf4NoActionBlockingCount	=_iNoActionBlockingCount;
	packet.bf4DancingBlockCount	=	_iDancingBlockCount;
	packet.bf4HardBlowCount		=	_iHardBlowCount;

	packet.bf1IsInstanceKill	=	FALSE;

	packet.bf4CriticalCount		=	min(_iCriticalCount,15);
	packet.bf4DoubleCriticalCount=	min(_iDoubleCriticalCount,15);
	packet.bf1IsCrushBlow		=	FALSE;
	packet.bf1IsAttackToShakle	=	_lpCaster->isAttackToShakleActor(_lpTarget);

	if	(_iInstanceKillCount)
		packet.bf1IsInstanceKill=	TRUE;
	if	(_iPhysicalDamage)
		packet.iPhysicalDamage	=	max(packet.iPhysicalDamage,1);
	if	(_iMagicDamage)
		packet.iMagicDamage	=	max(packet.iMagicDamage,1);

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	분신 공격
void
cFIELD::sendBunshineAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,int _iShotCount,int _iMissCount,
						   int _iCriticalCount,int _iDoubleCriticalCount,int _iMinPhysicalDamage,int _iMinMagicDamage,int _iBlockingCount,int _iInstanceKillCount,
						   int _iNoActionBlockingCount,int _iDancingBlockCount, int _iHardBlowCount)
{
	if (!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	SG_BUNSHINE_ATTACK	packet;

	packet.base.set(sizeof(SG_BUNSHINE_ATTACK),dSG_BUNSHINE_ATTACK);

	packet.bf11Serial			=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11TargetSerial		=	_lpTarget->m_wSerialInField;	//	누구를-_-?
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.iPhysicalDamage		=	_iPhysicalDamage;
	packet.iMagicDamage			=	_iMagicDamage;
	packet.bf11Skill			=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.bf7Level				=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.bf5AttackCount		=	_iShotCount;
	packet.bf4MissCount			=	min(_iMissCount,15);
	packet.bf4CriticalCount		=	min(_iCriticalCount,15);
	packet.bf4DoubleCriticalCount=	min(_iDoubleCriticalCount,15);
	packet.bf1IsCrushBlow		=	FALSE;
	packet.bf1IsAttackToShakle	=	_lpCaster->isAttackToShakleActor(_lpTarget);
	packet.wMinPhysicalDamage	=	_iMinPhysicalDamage;
	packet.wMinMagicDamage		=	_iMinMagicDamage;
	packet.bf4BlockingCount		=	_iBlockingCount;
	packet.bf1IsInstanceKill	=	FALSE;
	packet.bf4NoActionBlockingCount	=_iNoActionBlockingCount;
	packet.bf4DancingBlockCount	=	_iDancingBlockCount;
	packet.bf4HardBlowCount		=	_iHardBlowCount;
	if	(_iInstanceKillCount)
		packet.bf1IsInstanceKill=	TRUE;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

void
cFIELD::sendBouncingMissile(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility)
{
	if (!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	CActiveSkill*	lpActiveSkill	=	_lpCaster->getActiveSkill();
	
	if	(!lpActiveSkill)
		return;

	if(lpActiveSkill->m_wSkillType != eSKILL_TYPE_BOUNCING_MISSILE)
		return;
	
	cAS_BouncingMissile*	lpActiveBCSkill	=	(cAS_BouncingMissile	*)lpActiveSkill;
	
	SG_WIDE_AREA_DAMAGE	packet;
	memset(&packet,0, sizeof(packet));
	
	packet.base.set(sizeof(packet),dSG_WIDE_AREA_DAMAGE);
	
	packet.bf6Count		=	lpActiveBCSkill->m_bTargetCount;
	packet.bf12Caster	=	_lpCaster->m_wSerialInField;
	packet.bf12Skill	=	lpActiveBCSkill->m_ability.m_wSkill;
	packet.bf8Level		=	lpActiveBCSkill->m_ability.m_wLevel;
	packet.sRemainCP	=	_lpCaster->m_iCP/100;
	packet.bf1IsDamagePool=	FALSE;
	packet.bf7FPS			=	_lpCaster->m_iFPS;

	for (int i=0;i<packet.bf6Count;i++)
	{
		packet.aData[i].wActor			=	lpActiveBCSkill->m_awTargetList[i];
		packet.aData[i].wPhysicalDamage	=	(WORD)(_lpCaster->m_attackInfo.m_aInfo[i].m_dwPhysicalDamage/100);
		packet.aData[i].wMagicDamage	=	(WORD)(_lpCaster->m_attackInfo.m_aInfo[i].m_dwMagicDamage/100);
		packet.aData[i].dwResultField	=	_lpCaster->m_attackInfo.m_aInfo[i].m_dwResultField;
		
		if (packet.aData[i].wPhysicalDamage	==	0 && _lpCaster->m_attackInfo.m_aInfo[i].m_dwPhysicalDamage)
			packet.aData[i].wPhysicalDamage	=	1;
		if (packet.aData[i].wMagicDamage	==	0 && _lpCaster->m_attackInfo.m_aInfo[i].m_dwMagicDamage)
			packet.aData[i].wMagicDamage	=	1;
	}
	
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWIDE_AREA_DAMAGE_E)*packet.bf6Count;
	
	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}
//
//	시미터 커팅
void
cFIELD::sendScimitarCuttingAttack(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iPhysicalDamage,int _iMagicDamage,int _iShotCount,int _iMissCount,int _iRange,
								int _iCriticalCount,int _iDoubleCriticalCount,int _iMinPhysicalDamage,int _iMinMagicDamage,int _iBlockingCount,int _iInstanceKillCount,
								int _iNoActionBlockingCount,int _iDancingBlockCount, int _iHardBlowCount)
{
	if	(!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	SG_SCIMITAR_CUTTING	packet;

	packet.base.set(sizeof(SG_SCIMITAR_CUTTING),dSG_SCIMITAR_CUTTING);

	packet.bf11Serial				=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11TargetSerial		=	_lpTarget->m_wSerialInField;	//	누구를-_-?
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.iPhysicalDamage		=	_iPhysicalDamage;
	packet.iMagicDamage			=	_iMagicDamage;
	packet.wRange				=	_iRange;
	packet.bf11Skill			=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.bf7Level				=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.bf5AttackCount 		=	_iShotCount;
	packet.bf4MissCount			=	min(_iMissCount,15);
	packet.bf4CriticalCount		=	min(_iCriticalCount,15);
	packet.bf4DoubleCriticalCount=	min(_iDoubleCriticalCount,15);
	packet.bf1IsCrushBlow		=	FALSE;
	packet.bf1IsAttackToShakle	=	_lpCaster->isAttackToShakleActor(_lpTarget);
	packet.wMinPhysicalDamage	=	_iMinPhysicalDamage;
	packet.wMinMagicDamage		=	_iMinMagicDamage;
	packet.bf4BlockingCount		=	_iBlockingCount;
	packet.bf1IsInstanceKill	=	FALSE;
	packet.bf4NoActionBlockingCount	=_iNoActionBlockingCount;
	packet.bf4DancingBlockCount	=	_iDancingBlockCount;
	packet.bf4HardBlowCount		=	_iHardBlowCount;

	if	(_iInstanceKillCount)
		packet.bf1IsInstanceKill=	TRUE;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	누군가-_-를 공격한다.
void
cFIELD::sendActionToActor(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	if	(_lpAbility->getSkill()->isCastOnCorpseSkill())
	{
		if	(!_lpTarget								)
			return;
		if	(_lpTarget->m_wSerialInField == 0xffff	)
			return;
	}
	else	
	if	(!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	switch(_lpAbility->getSkill()->m_wType)
	{
		case	eSKILL_TYPE_ILLUSION_MISSILE	:				//	환상미사일
			sendContinuousHitAttack(_lpCaster,_lpTarget,_lpAbility);
			return;
	}

	CHitInfo	*lpHitInfo	=	&_lpCaster->m_attackInfo.m_aInfo[0];

	if	(_lpHitInfo)
		lpHitInfo			=	_lpHitInfo;

	SG_ACTION_TO_ACTOR	packet;

	int	iRange				=	_lpCaster->getAttackRange(_lpAbility);

	packet.base.set(sizeof(SG_ACTION_TO_ACTOR),dSG_ACTION_TO_ACTOR);

	packet.bf11Serial		=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11TargetSerial	=	_lpTarget->m_wSerialInField;	//	누구를-_-?
	packet.bf11Skill		=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.bf7Level			=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.wPosX			=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY			=	_lpCaster->m_pos.y;		//	위치
	packet.bf5FPS			=	_lpCaster->m_iFPS&0x1F;

	if	(_lpCaster->m_iFPS	>=	0x20)
		packet.bf1FPS		=	1;		//	FPS
	else
		packet.bf1FPS		=	0;		//	FPS

	packet.wRange			=	iRange;					//	사정거리
	packet.iPhysicalDamage =	(lpHitInfo->m_dwPhysicalDamage/100);	//	얼마나?
	packet.iMagicDamage 		=	(lpHitInfo->m_dwMagicDamage/100);		//	얼마나?
	packet.dwResultField		=	lpHitInfo->m_dwResultField;

	if	(lpHitInfo->m_dwPhysicalDamage)
		packet.iPhysicalDamage	=	max(packet.iPhysicalDamage,1);

	if	(lpHitInfo->m_dwMagicDamage)
		packet.iMagicDamage		=	max(packet.iMagicDamage,1);

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	어딘가-_-를 공격한다.
void
cFIELD::sendActionToGround(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility)
{
	if (!checkPlayerAndKill(_lpCaster))
		return;

	SG_ACTION_TO_GROUND	packet;

	int	iRange			=	_lpCaster->getAttackRange(_lpAbility);

	packet.base.set(sizeof(SG_ACTION_TO_GROUND),dSG_ACTION_TO_GROUND);

	packet.wSerial		=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf12Skill	=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.wLevel		=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.wPosX		=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY		=	_lpCaster->m_pos.y;		//	위치
	packet.wDestPosX	=	_iX;					//	목표 위치
	packet.wDestPosY	=	_iY;					//	목표 위치
	packet.wFPS			=	_lpCaster->m_iFPS;		//	FPS
	packet.wRange		=	iRange;					//	사정거리
	packet.bf1MiniPetCharge1	=	_lpCaster->m_aActiveMiniPetInfo[0].m_wIsCharged;
	packet.bf1MiniPetCharge2	=	_lpCaster->m_aActiveMiniPetInfo[1].m_wIsCharged;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	퀵 액션을 사용했다.
void
cFIELD::sendActionQuick(cACTOR *_lpCaster,cAbility *_lpAbility,int _iDirect)
{
	if (!checkPlayerAndKill(_lpCaster))
		return;

	SG_QUICK_ACTION	packet;

	packet.base.set(sizeof(SG_QUICK_ACTION),dSG_QUICK_ACTION);

	packet.bf11Serial	=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11Skill	=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.bf7Level		=	_lpAbility->m_wLevel;	//	뭘로.. -o-
	packet.wPosX		=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY		=	_lpCaster->m_pos.y;		//	위치
	packet.bf4Direct	=	_iDirect;
	packet.bf6FPS		=	_lpCaster->m_iFPS;
	packet.bf1MiniPetCharge1	=	_lpCaster->m_aActiveMiniPetInfo[0].m_wIsCharged;
	packet.bf1MiniPetCharge2	=	_lpCaster->m_aActiveMiniPetInfo[1].m_wIsCharged;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	스파이크 데미지 전송
void
cFIELD::sendHitThornDamage(int _iAttacker,int _iTarget,int _iDamage,int _iRemainCP ,BOOL _isDisplayMagicDamage)
{
	SG_HIT_THORN_DAMAGE	packet;

	packet.base.set(sizeof(SG_HIT_THORN_DAMAGE),dSG_HIT_THORN_DAMAGE);

	packet.wDamage	=	_iDamage/100;
	packet.wTarget	=	_iTarget;
	packet.wAttacker=	_iAttacker;
	packet.sRemainCP=	_iRemainCP/100;
	packet.bIsDiplayMagicDamage = _isDisplayMagicDamage;
	
	if (packet.wDamage	>	20000)
		packet.wDamage	=	packet.wDamage;


	addSendPacket((ALL_MSG*)&packet,_iAttacker);
}

//
//	데미지 흡수
void
cFIELD::sendAbsorbDamage(int _iActorSerial,int _iAbsorbDamage)
{
	SG_ABSORB_DAMAGE	packet;

	packet.base.set(sizeof(packet),dSG_ABSORB_DAMAGE);

	packet.wActor	=	_iActorSerial;
	packet.iAbsorbHp=	_iAbsorbDamage;

	addSendPacket((ALL_MSG*)&packet,_iActorSerial);
}

//
//	시머링 실드 해제
void
cFIELD::sendReleaseShimmeringShield(int _iCaster,int _iReceiver)
{
	SG_RELEASE_SHIMMERING_SHIELD	packet;

	packet.base.set(sizeof(SG_RELEASE_SHIMMERING_SHIELD),dSG_RELEASE_SHIMMERING_SHIELD);

	packet.wCaster	=	_iCaster;
	packet.wReceiver=	_iReceiver;


	addSendPacket((ALL_MSG*)&packet,_iCaster);
}

//
//	시머링 실드 설정
void
cFIELD::sendSetShimmeringShield(int _iCaster,int _iReceiver,int _iRemainCP,int _iShieldShape,BOOL _bIsShield)
{
	SG_SET_SHIMMERING_SHIELD	packet;

	packet.base.set(sizeof(SG_SET_SHIMMERING_SHIELD),dSG_SET_SHIMMERING_SHIELD);

	packet.wCaster		=	_iCaster;
	packet.wReceiver	=	_iReceiver;
	packet.sRemainCP	=	_iRemainCP/100;
	packet.bf6ShieldShape=	_iShieldShape;
	packet.bf1IsShield	=	_bIsShield;

	addSendPacket((ALL_MSG*)&packet,_iCaster);
}

//
//	치유 메시지 전송
void
cFIELD::sendCureActorMessage(int _iTarget,int _iSkill,int _iHealPoint)
{
	SG_CURE_ACTOR	packet;

	packet.base.set(sizeof(SG_CURE_ACTOR),dSG_CURE_ACTOR);

	packet.iHealPoint	=	_iHealPoint;
	packet.wActor		=	_iTarget;
	packet.wSkill		=	_iSkill;

	addSendPacket((ALL_MSG*)&packet,_iTarget);
}

//
//	머신을 띄운다.
void
cFIELD::sendFloatMachine(int _iTarget,int _iSkill)
{
	SG_FLOAT_MACHINE	packet;

	packet.base.set(sizeof(SG_FLOAT_MACHINE),dSG_FLOAT_MACHINE);

	packet.wTarget	=	_iTarget;
	packet.wSkill	=	_iSkill;
	
	addSendPacket((ALL_MSG*)&packet,_iTarget);
}

//
//	머신을 제거한다.
void
cFIELD::sendReleaseMachine(int _iTarget)
{
	SG_RELEASE_MACHINE	packet;

	packet.base.set(sizeof(SG_RELEASE_MACHINE),dSG_RELEASE_MACHINE);

	packet.wTarget	=	_iTarget;

	addSendPacket((ALL_MSG*)&packet,_iTarget);
}

//
//미사일 쐈다!!
void
cFIELD::sendMachineMissile(int _iCaster,int _iTarget,int _iSkill,CHitInfo *_lpHitInfo,int _iPetOrder)
{
	SG_SHOOT_MACHINE_MISSILE	packet;

	packet.base.set(sizeof(SG_SHOOT_MACHINE_MISSILE),dSG_SHOOT_MACHINE_MISSILE);
	
	packet.wCaster			=	_iCaster;
	packet.wTarget			=	_iTarget;
	packet.bf12Skill		=	_iSkill;
	packet.m_bf2MinpetOrder	=	_iPetOrder;
	packet.hitInfo.m_wPhysicalDamage	=	(WORD)(_lpHitInfo->m_dwPhysicalDamage/100);
	packet.hitInfo.m_wMagicDamage		=	(WORD)(_lpHitInfo->m_dwMagicDamage/100);
	packet.hitInfo.m_dwResultField		=	_lpHitInfo->m_dwResultField;

	addSendPacket((ALL_MSG*)&packet,_iCaster);
}

//
//	테일 체이서 발사!!
void
cFIELD::sendFireTailChaser(int _iCaster,int _iTarget,int _iSkill,int _iShotCount,int _iShotPeriod,int _iDamage,int _iCorrectDamage)
{
	if (!checkActor(_iCaster))
		return;

	SG_FIRE_TAIL_CHASER	packet;

	packet.base.set(sizeof(SG_FIRE_TAIL_CHASER),dSG_FIRE_TAIL_CHASER);

	packet.wSkill			=	_iSkill;
	packet.wCaster			=	_iCaster;
	packet.wTarget			=	_iTarget;
	packet.wDamage			=	_iDamage/100;
	packet.wShotCount		=	_iShotCount;
	packet.wShotPeriod		=	_iShotPeriod;
	packet.wCorrectDamage	=	_iCorrectDamage;
	packet.sRemainCP		=	getActor(_iCaster)->m_iCP/100;

	addSendPacket((ALL_MSG*)&packet,_iCaster);
}

//
//	액터를 타격정보로 공격.. -_-;;
void
cFIELD::sendMakeActionForm(int _iCaster,int _iTarget,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	if	(!checkActor(_iCaster))
		return;

	SG_MAKE_ACTION_FORM	packet;

	packet.base.set(sizeof(SG_MAKE_ACTION_FORM),dSG_MAKE_ACTION_FORM);

	packet.wCaster	=	_iCaster;
	packet.wTarget	=	_iTarget;
	packet.sRemainCP=	getActor(_iCaster)->m_iCP/100;
	packet.wPosX	=	getActor(_iCaster)->m_pos.x;
	packet.wPosY	=	getActor(_iCaster)->m_pos.y;

	if	(_lpHitInfo)
	{
		packet.hitInfo.m_wPhysicalDamage	=	(WORD)(_lpHitInfo->m_dwPhysicalDamage/100);
		packet.hitInfo.m_wMagicDamage		=	(WORD)(_lpHitInfo->m_dwMagicDamage/100);
		packet.hitInfo.m_dwResultField		=	_lpHitInfo->m_dwResultField;

		if	(_lpHitInfo->m_dwPhysicalDamage)
			packet.hitInfo.m_wPhysicalDamage=	max(packet.hitInfo.m_wPhysicalDamage,1);
		if	(_lpHitInfo->m_dwMagicDamage)
			packet.hitInfo.m_wMagicDamage	=	max(packet.hitInfo.m_wMagicDamage,1);
	}

	memcpy(&packet.ability,_lpAbility,sizeof(cAbility));
	
	addSendPacket((ALL_MSG*)&packet,packet.wCaster);
}

//
//	_iCaster가 _iTarget에게 _lpAbility를 한다.
void
cFIELD::sendUseSkillToActor(int _iCaster,int _iTarget,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	if(!checkActor(_iCaster))	return;

	SG_USE_SKILL_TO_ACTOR	packet;
	packet.base.set(sizeof(SG_USE_SKILL_TO_ACTOR),dSG_USE_SKILL_TO_ACTOR);

	packet.wActor	=	_iCaster;
	packet.wTarget	=	_iTarget;
	packet.sRemainCP=	getActor(_iCaster)->m_iCP/100;
	packet.wPosX	=	getActor(_iCaster)->m_pos.x;
	packet.wPosY	=	getActor(_iCaster)->m_pos.y;
	packet.hitInfo.m_wPhysicalDamage	=	(WORD)(_lpHitInfo->m_dwPhysicalDamage/100);
	packet.hitInfo.m_wMagicDamage		=	(WORD)(_lpHitInfo->m_dwMagicDamage/100);
	packet.hitInfo.m_dwResultField		=	_lpHitInfo->m_dwResultField;

	memcpy(&packet.ability,_lpAbility,sizeof(cAbility));

	addSendPacket((ALL_MSG*)&packet,_iCaster);
}

//
//	_iCaster가 _iTarget에게 _lpAbility를 한다.
void
cFIELD::sendUseSkillToGround(int _iCaster,int _iPosX,int _iPosY,cAbility *_lpAbility)
{
	SG_USE_SKILL_TO_GROUND	packet;
	packet.base.set(sizeof(SG_USE_SKILL_TO_GROUND),dSG_USE_SKILL_TO_GROUND);

	packet.wActor	=	_iCaster;
	packet.wPosX	=	_iPosX;
	packet.wPosY	=	_iPosY;

	memcpy(&packet.ability,_lpAbility,sizeof(cAbility));

	addSendPacket((ALL_MSG*)&packet,_iCaster);
}


void
cFIELD::sendWideMissile(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility)
{
	if (!checkTargetAndKill(_lpCaster,_lpTarget))
		return;

	CActiveSkill*	lpActiveSkill	=	_lpCaster->getActiveSkill();
	
	if	(!lpActiveSkill)
		return;

	if(lpActiveSkill->m_wSkillType != eSKILL_TYPE_WIDE_MISSILE)
		return;
	
	cAS_WideMissile*	lpActiveBCSkill	=	(cAS_WideMissile	*)lpActiveSkill;
	
	SG_WIDE_AREA_DAMAGE	packet;
	memset(&packet,0, sizeof(packet));
	
	packet.base.set(sizeof(packet),dSG_WIDE_AREA_DAMAGE);
	
	packet.bf6Count		=	lpActiveBCSkill->m_bTargetCount;
	packet.bf12Caster	=	_lpCaster->m_wSerialInField;
	packet.bf12Skill	=	lpActiveBCSkill->m_ability.m_wSkill;
	packet.bf8Level		=	lpActiveBCSkill->m_ability.m_wLevel;
	packet.sRemainCP	=	_lpCaster->m_iCP/100;
	packet.bf1IsDamagePool=	FALSE;
	packet.bf7FPS			=	_lpCaster->m_iFPS;

	for (int i=0;i<packet.bf6Count;i++)
	{
		packet.aData[i].wActor			=	lpActiveBCSkill->m_awTargetList[i];
		packet.aData[i].wPhysicalDamage	=	(WORD)(_lpCaster->m_attackInfo.m_aInfo[i].m_dwPhysicalDamage/100);
		packet.aData[i].wMagicDamage	=	(WORD)(_lpCaster->m_attackInfo.m_aInfo[i].m_dwMagicDamage/100);
		packet.aData[i].dwResultField	=	_lpCaster->m_attackInfo.m_aInfo[i].m_dwResultField;
		
		if (packet.aData[i].wPhysicalDamage	==	0 && _lpCaster->m_attackInfo.m_aInfo[i].m_dwPhysicalDamage)
			packet.aData[i].wPhysicalDamage	=	1;
		if (packet.aData[i].wMagicDamage	==	0 && _lpCaster->m_attackInfo.m_aInfo[i].m_dwMagicDamage)
			packet.aData[i].wMagicDamage	=	1;
	}
	
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWIDE_AREA_DAMAGE_E)*packet.bf6Count;
	
	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

//
//	리액션스킬이 발동했다.
void
cFIELD::addActiveReactionSkillMessage(cACTOR *_lpActor,cACTOR *_lpTarget,cAbility *_lpAbility)
{
	SG_ACTIVE_REACTION_SKILL	packet;
	
	packet.base.set(sizeof(SG_ACTIVE_REACTION_SKILL),dSG_ACTIVE_REACTION_SKILL);

	packet.wCaster	=	_lpActor->m_wSerialInField;
	packet.wTarget	=	_lpTarget->m_wSerialInField;
	packet.sRemainCP=	_lpActor->m_iCP/100;
	packet.wSkill	=	_lpAbility->m_wSkill;

	addSendPacket((ALL_MSG*)&packet,packet.wTarget);
}
void
cFIELD::sendActionToGroupStickedBit(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility)
{

	if (!checkTargetAndKill(_lpCaster,_lpTarget))
		return;


	
	CActiveSkill*	lpActiveSkill	=	_lpCaster->getActiveSkill();
	
	if	(!lpActiveSkill)
		return;

	if(lpActiveSkill->m_wSkillType != eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT)
		return;
	
	cAS_ContinuousBit*	lpActiveCBSkill	=	(cAS_ContinuousBit	*)lpActiveSkill;

	SG_ACTION_TO_GROUP_STICKED_BIT	packet;
	memset(&packet,0, sizeof(packet));
	packet.base.set(sizeof(SG_ACTION_TO_GROUP_STICKED_BIT),dSG_ACTION_TO_GROUP_STICKED_BIT);
	
	packet.bf11Serial			=	_lpCaster->m_wSerialInField;	//	누가-_-?
	packet.bf11Skill			=	_lpAbility->m_wSkill;	//	뭘로.. -o-
	packet.wPosX				=	_lpCaster->m_pos.x;		//	위치
	packet.wPosY				=	_lpCaster->m_pos.y;		//	위치
	packet.bf6FPS				=	_lpCaster->m_iFPS;		//	FPS
	packet.bf4TargetCount		=	lpActiveCBSkill->m_bTargetCount;
	packet.wRange				=	_lpCaster->getAttackRange(_lpAbility);

	for (int i=0;i<lpActiveCBSkill->m_bTargetCount;i++)
	{
		CHitInfo* lpHitInfo							=	&_lpCaster->m_attackInfo.m_aInfo[i];

		packet.aData[i].wActor						=	lpActiveCBSkill->m_awTargetList[i];
		packet.aData[i].iPhysicalDamage				=	(lpHitInfo->m_dwPhysicalDamage/100);
		packet.aData[i].iMagicDamage				=	(lpHitInfo->m_dwMagicDamage/100);
		packet.aData[i].dwResultField				=	lpHitInfo->m_dwResultField;
		packet.aData[i].m_bf6ContinuousShotCount	=	lpHitInfo->continuousHitInfo.m_bf6ContinuousShotCount;
		packet.aData[i].m_bf6CriticalCount			=	lpHitInfo->continuousHitInfo.m_bf6CriticalCount;
		packet.aData[i].m_bf6DoubleCriticalCount	=	lpHitInfo->continuousHitInfo.m_bf6DoubleCriticalCount;
		packet.aData[i].m_bf6HardBlowCount			=	lpHitInfo->continuousHitInfo.m_bf6HardBlowCount;
		packet.aData[i].m_bf6MissCount				=	lpHitInfo->continuousHitInfo.m_bf6MissCount;

		if (lpHitInfo->m_dwPhysicalDamage)
			packet.aData[i].iPhysicalDamage	=	max(packet.aData[i].iPhysicalDamage,1);
		if (lpHitInfo->m_dwMagicDamage)
			packet.aData[i].iMagicDamage	=	max(packet.aData[i].iMagicDamage,1);
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cContinuousDamageInfo)*lpActiveCBSkill->m_bTargetCount;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);


}

//
//	공격 결과 전성
void
cFIELD::sendAttackToActorResult(int _iResult,cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility)
{
	CSkill *lpSkill		=	_lpAbility->getSkill();
	BOOL	bIsSuccess	=	TRUE;

	switch(_iResult)
	{
		case	eAR_OK							:	//	OK
		{
			if (lpSkill->isAgressiveSkill())
				if (!_lpCaster->m_lpTarget)
					_lpCaster->setTarget(_lpTarget,lpSkill);

			switch(_lpCaster->m_wUseSkill)
			{
				case	dSKILL_BASIC_ATTACK				:
				{
					int iAttackCount = lpSkill->m_wReiterationDamageCountSyncWithOverlapAction;
					if( _lpCaster->m_fightEnergy.isIncreaseAttackCount())
						iAttackCount += _lpCaster->m_fightEnergy.m_iValue;
					if	(iAttackCount > 1)
						sendContinuousHitAttack(_lpCaster,_lpTarget,_lpAbility);
					else
						sendBasicAttack(_lpCaster,_lpTarget,_lpAbility);
					break;
				}

				case	dSKILL_CONTINUOUS_HIT_ATTACK	:
				case	dSKILL_ILLUSION_ATTACK			:
					sendContinuousHitAttack(_lpCaster,_lpTarget,_lpAbility);
					break;

				default	:
					sendActionToActor(_lpCaster,_lpTarget,_lpAbility);
					break;
			}
			_lpCaster->emptyBookedAction();
			break;
		}

		case	eAR_INTERVAL_SHOOTER			:
		{
			CActiveSkill	*lpActiveSkill	=	_lpCaster->getActiveSkill();
			
			if (lpActiveSkill)
				sendIntervaslShooterAttack(_lpCaster,_lpAbility,lpActiveSkill->getIntervalShooterAttackCount());

			_lpCaster->emptyBookedAction();
			break;
		}
		case	eAR_DOUBLE_TARGET				:
		{
			sendActionToGroup(_lpCaster,_lpTarget,_lpAbility);

			_lpCaster->emptyBookedAction();
			break;
		}
		case	eAR_GROUP_CONTINUOUS_BY_BIT		:
		{
			sendActionToGroupStickedBit(_lpCaster,_lpTarget,_lpAbility);

			_lpCaster->emptyBookedAction();
			break;
		}
		case	eAR_BOUNCING_MISSILE			:
			{
				sendBouncingMissile(_lpCaster,_lpTarget,_lpAbility);		
				_lpCaster->emptyBookedAction();
				break;
			}
		case	eAR_WIDE_MISSILE			:
			{
				sendWideMissile(_lpCaster,_lpTarget,_lpAbility);		
				_lpCaster->emptyBookedAction();
				break;
			}


		case	eAR_CONTINUOUS_ATTACK_BY_VALUE	:
		{
			if	(!_lpCaster->m_lpTarget)
				_lpCaster->setTarget(_lpTarget,lpSkill);

			CActiveSkill	*lpActiveSkill	=	_lpCaster->getActiveSkill();

			if (lpActiveSkill)
			{
				int	iPhysicalDamage		=	lpActiveSkill->getContinuousAttackDamageSumOfPhysicalDamage();
				int	iMagicDamage		=	lpActiveSkill->getContinuousAttackDamageSumOfMagicDamage();
				int	iShotCount			=	lpActiveSkill->getContinuousAttackCount();
				int	iMissCount			=	lpActiveSkill->getContinuousAttackMissCount();
				int	iCriticalCount		=	lpActiveSkill->getContinuousAttackCriticalCount();
				int	iDoubleCriticalCount=	lpActiveSkill->getContinuousAttackDoubleCriticalCount();
				int	iMinPhysicalDamage	=	lpActiveSkill->getContinuousAttackMinPhysicalDamage();
				int	iMinMagicDamage		=	lpActiveSkill->getContinuousAttackMinMagicDamage();
				int	iBlockingCount		=	lpActiveSkill->getContinuousAttackBlockingCount();
				int	iInstanceKillCount	=	lpActiveSkill->getContinuousAttackInstanceKillCount();
				int	iNoActionBlockCount	=	lpActiveSkill->getContinuousAttackNoActionBlockCount();
				int	iDancingBlockCount	=	lpActiveSkill->getContinuousAttackDancingBlockCount();
				int iHardBlowCount		=	lpActiveSkill->getContinuousAttackHardBlowCount();
				sendContinuousHitAttackByValue(_lpCaster,_lpTarget,_lpAbility,iPhysicalDamage,iMagicDamage,iShotCount,iMissCount,iCriticalCount,iDoubleCriticalCount,iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount,iHardBlowCount);
			}

			_lpCaster->emptyBookedAction();
			break;
		}
		case	eAR_BUNSHINE_ATTACK				:
		{
			if	(!_lpCaster->m_lpTarget)	
				_lpCaster->setTarget(_lpTarget,lpSkill);

			CActiveSkill	*lpActiveSkill	=	_lpCaster->getActiveSkill();

			if (lpActiveSkill)
			{
				int	iPhysicalDamage		=	lpActiveSkill->getBunshineAttackDamageSumOfPhysicalDamage();
				int	iMagicDamage		=	lpActiveSkill->getBunshineAttackDamageSumOfMagicDamage();
				int	iShotCount			=	lpActiveSkill->getBunshineAttackCount();
				int	iMissCount			=	lpActiveSkill->getBunshineAttackMissCount();
				int	iCriticalCount		=	lpActiveSkill->getBunshineAttackCriticalCount();
				int	iDoubleCriticalCount=	lpActiveSkill->getBunshineAttackDoubleCriticalCount();
				int	iMinPhysicalDamage	=	lpActiveSkill->getBunshineAttackMinPhysicalDamage();
				int	iMinMagicDamage		=	lpActiveSkill->getBunshineAttackMinMagicDamage();
				int	iBlockingCount		=	lpActiveSkill->getBunshineAttackBlockingCount();
				int	iInstanceKillCount	=	lpActiveSkill->getBunshineAttackInstanceKillCount();
				int	iNoActionBlockCount	=	lpActiveSkill->getBunshineAttackNoActionBlockCount();
				int	iDancingBlockCount	=	lpActiveSkill->getBunshineAttackDancingBlockCount();
				int iHardBlowCount		=	lpActiveSkill->getBunshineAttackHardBlowCount();

				sendBunshineAttack(_lpCaster,_lpTarget,_lpAbility,iPhysicalDamage,iMagicDamage,iShotCount,iMissCount,iCriticalCount,iDoubleCriticalCount,iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount, iHardBlowCount);
			}
			_lpCaster->emptyBookedAction();
			break;
		}
		case	eAR_SCIMITAR_CUTTING			:
		{
			if	(!_lpCaster->m_lpTarget)	
				_lpCaster->setTarget(_lpTarget,lpSkill);

			CActiveSkill	*lpActiveSkill	=	_lpCaster->getActiveSkill();

			if (lpActiveSkill)
			{
				int	iPhysicalDamage		=	lpActiveSkill->getScimitarCuttingPhysicalDamage();
				int	iMagicDamage		=	lpActiveSkill->getScimitarCuttingMagicDamage();
				int	iShotCount			=	lpActiveSkill->getScimitarCuttingRoundCount();
				int	iRange				=	lpActiveSkill->getScimitarCuttingRange();
				int	iMissCount			=	lpActiveSkill->getScimitarCuttingMissCount();
				int	iCriticalCount		=	lpActiveSkill->getScimitarCuttingCriticalCount();
				int	iDoubleCriticalCount=	lpActiveSkill->getScimitarCuttingDoubleCriticalCount();
				int	iMinPhysicalDamage	=	lpActiveSkill->getScimitarCuttingMinPhysicalDamage();
				int	iMinMagicDamage		=	lpActiveSkill->getScimitarCuttingMinMagicDamage();
				int	iBlockingCount		=	lpActiveSkill->getScimitarCuttingBlockingCount();
				int	iInstanceKillCount	=	lpActiveSkill->getScimitarCuttingInstanceKillCount();
				int	iNoActionBlockCount	=	lpActiveSkill->getScimitarCuttingNoActionBlockCount();
				int	iDancingBlockCount	=	lpActiveSkill->getScimitarCuttingDancingBlockCount();
				int iHardBlowCount		=	lpActiveSkill->getScimitarCuttingHardBlowCount();
				sendScimitarCuttingAttack(_lpCaster,_lpTarget,_lpAbility,iPhysicalDamage,iMagicDamage,iShotCount,iMissCount,iRange,iCriticalCount,iDoubleCriticalCount,iMinPhysicalDamage,iMinMagicDamage,iBlockingCount,iInstanceKillCount,iNoActionBlockCount,iDancingBlockCount, iHardBlowCount);
			}

			_lpCaster->emptyBookedAction();
			break;
		}

		case	eAR_BOUNCING_LINEAR				:	//	바운싱 리니어
		{
			CActiveSkill		*lpActiveSkill	=	_lpCaster->getActiveSkill();

			if (lpActiveSkill)
			{
				SG_WIDE_AREA_DAMAGE	packet;

				packet.base.set(sizeof(packet),dSG_WIDE_AREA_DAMAGE);

				packet.bf6Count		=	lpActiveSkill->getBouncingLinearTargetCount();
				packet.bf12Caster	=	_lpCaster->m_wSerialInField;
				packet.bf12Skill	=	lpActiveSkill->m_ability.m_wSkill;
				packet.bf8Level		=	lpActiveSkill->m_ability.m_wLevel;
				packet.sRemainCP	=	_lpCaster->m_iCP/100;
				packet.bf1IsDamagePool=	FALSE;

				for (int i=0;i<packet.bf6Count;i++)
				{
					packet.aData[i].wActor			=	lpActiveSkill->m_aValue[i];
					packet.aData[i].wPhysicalDamage	=	(WORD)(_lpCaster->m_attackInfo.m_aInfo[i].m_dwPhysicalDamage/100);
					packet.aData[i].wMagicDamage	=	(WORD)(_lpCaster->m_attackInfo.m_aInfo[i].m_dwMagicDamage/100);
					packet.aData[i].dwResultField	=	_lpCaster->m_attackInfo.m_aInfo[i].m_dwResultField;

					if (packet.aData[i].wPhysicalDamage	==	0 && _lpCaster->m_attackInfo.m_aInfo[i].m_dwPhysicalDamage)
						packet.aData[i].wPhysicalDamage	=	1;
					if (packet.aData[i].wMagicDamage	==	0 && _lpCaster->m_attackInfo.m_aInfo[i].m_dwMagicDamage)
						packet.aData[i].wMagicDamage	=	1;
				}

				packet.base.wSize	=	sizeof(packet)-sizeof(packet.aData)+sizeof(cWIDE_AREA_DAMAGE_E)*packet.bf6Count;

				addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
			}

			_lpCaster->emptyBookedAction();
			break;
		}

		case	eAR_ICY_STALAGMITE				:
		{
			_lpCaster->castIcyStalagmite(_lpAbility);
			_lpCaster->emptyBookedAction();
			break;
		}

		case	eAR_SET_SPECIAL_SKILL_FIELD		:
		{
			sendChangeSpecialSkillField(_lpCaster,_lpAbility);
			break;
		}
		
		case	eAR_LAND_MARKER					:
		{
			CActiveSkill	*lpActiveSkill	=	_lpCaster->getActiveSkill();

			if	(!_lpCaster->m_lpTarget)						//  09.09.10
				_lpCaster->setTarget(_lpTarget,lpSkill);

			if (lpActiveSkill)
				sendLandMarkerAttack(_lpCaster,_lpTarget->m_pos.x,_lpTarget->m_pos.y,_lpAbility,lpActiveSkill->getContinuousAttackCount());
			_lpCaster->emptyBookedAction();
			break;
		}

		default	:
		{
			bIsSuccess	=	FALSE;
			break;
		}
	}

	if	(bIsSuccess)
	{
		if	(_lpCaster->isPlayer())
			_lpCaster->updateLastActionTime();
	}
	else
		_lpCaster->sendUseSkillResult(_iResult);
}

//
//	공격 결과 전송
void
cFIELD::sendAttackToGroundResult(int _iResult,cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility)
{
	switch(_iResult)
	{
		case	eAR_OK							:	//	OK
			sendActionToGround(_lpCaster,_iX,_iY,_lpAbility);
			_lpCaster->emptyBookedAction();
			break;


		case	eAR_LAND_MARKER					:
		{
			CActiveSkill	*lpActiveSkill	=	_lpCaster->getActiveSkill();

			if (lpActiveSkill)
				sendLandMarkerAttack(_lpCaster,_iX,_iY,_lpAbility,lpActiveSkill->getContinuousAttackCount());
			_lpCaster->emptyBookedAction();
			break;
		}
	}
}

//
//	특정 스킬 효과가 발동!!
void
cFIELD::sendActiveSkillEffect(int _iCaster,int _iTarget,int _iSkill,int _iCP,int _iPhysicalDamage,int _iMagicDamage)
{
	SG_ACTIVE_SKILL_EFFECT	packet;

	packet.base.set(sizeof(SG_ACTIVE_SKILL_EFFECT),dSG_ACTIVE_SKILL_EFFECT);

	packet.wCaster			=	_iCaster;
	packet.wTarget			=	_iTarget;
	packet.wSkill			=	_iSkill;
	packet.sRemainCP		=	_iCP/100;
	packet.wPhysicalDamage	=	_iPhysicalDamage/100;
	packet.wMagicDamage		=	_iMagicDamage/100;

	if (_iPhysicalDamage)
		packet.wPhysicalDamage	=	max(packet.wPhysicalDamage,1);
	if (_iMagicDamage)
		packet.wMagicDamage		=	max(packet.wMagicDamage,1);

	addSendPacket((ALL_MSG *)&packet,_iCaster);
}

void
cFIELD::addPacketRemovePortal(int _iPortalSerial)
{
	SG_REMOVE_PORTAL	packet;

	packet.base.set(sizeof(packet),dSG_REMOVE_PORTAL);
	packet.wPortalSerial	=	_iPortalSerial;

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::addSendChangeDoorStatus(int _iArea,BOOL _bIsSleep,BOOL _bIsHide,BOOL _bIsLocked,BOOL _bIsTrap,BOOL _bIsClose,BOOL _bIsActiveByKarma)
{
	SG_CHANGE_DOOR_STATUS	packet;

	packet.base.set(sizeof(packet),dSG_CHANGE_DOOR_STATUS);

	packet.bf10Area				=	_iArea;
	packet.bf1IsSleep			=	_bIsSleep;
	packet.bf1IsHide			=	_bIsHide;
	packet.bf1IsLocked			=	_bIsLocked;
	packet.bf1IsTrap			=	_bIsTrap;
	packet.bf1IsClose			=	_bIsClose;
	packet.bf1IsActiveByKarma	=	_bIsActiveByKarma;

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::addSendStrikeLightningWinder(cACTOR *_lpCaster,cACTOR *_lpTarget,CHitInfo *_lpHitInfo,cAbility *_lpAbility)
{
	SG_STRIKE_LIGHTNING_WINDER	packet;

	packet.base.set(sizeof(packet),dSG_STRIKE_LIGHTNING_WINDER);

	packet.wCaster			=	_lpCaster->m_wSerialInField;
	packet.wTarget			=	_lpTarget->m_wSerialInField;
	packet.wPhysicalDamage	=	(WORD)_lpHitInfo->m_dwPhysicalDamage/100;
	packet.wMagicDamage		=	(WORD)_lpHitInfo->m_dwMagicDamage/100;
	packet.dwResultField		=	_lpHitInfo->m_dwResultField;
	packet.wAbsorbDamage	=	(WORD)_lpHitInfo->m_dwAbsorbDamage/100;
	packet.wSkill			=	_lpAbility->m_wSkill;

	LONGLONG	llMaxHP		=	_lpTarget->getMaxHP();
	LONGLONG	llHP		=	_lpTarget->m_iHP;

	llHP					=	llHP*60000/llMaxHP;
	packet.wRemainTargetHP	=	(WORD)llHP;

	if	(_lpHitInfo->m_dwPhysicalDamage)
		packet.wPhysicalDamage	=	max(packet.wPhysicalDamage,1);
	if	(_lpHitInfo->m_dwMagicDamage)
		packet.wMagicDamage	=	max(packet.wMagicDamage,1);
	if	(_lpHitInfo->m_dwAbsorbDamage)
		packet.wAbsorbDamage	=	max(packet.wAbsorbDamage,1);

	addSendPacket(&packet,_lpCaster->m_wSerialInField);
}
void
cFIELD::addSendStrikeMultiMissile(cACTOR *_lpCaster,cACTOR *_lpTarget,CHitInfo *_lpHitInfo,cAbility *_lpAbility)
{		// 멀티미사일..
	SG_STRIKE_MULTI_MISSILE	packet;

	packet.base.set(sizeof(packet),dSG_STRIKE_MULTI_MISSILE);

	packet.wCaster			=	_lpCaster->m_wSerialInField;
	packet.wTarget			=	_lpTarget->m_wSerialInField;
	packet.dwPhysicalDamage	=	_lpHitInfo->m_dwPhysicalDamage/100;
	packet.dwMagicDamage	=	_lpHitInfo->m_dwMagicDamage/100;
	packet.dwResultField		=	_lpHitInfo->m_dwResultField;
	packet.wAbsorbDamage	=	(WORD)_lpHitInfo->m_dwAbsorbDamage/100;
	packet.wSkill			=	_lpAbility->m_wSkill;

	LONGLONG	llMaxHP		=	_lpTarget->getMaxHP();
	LONGLONG	llHP		=	_lpTarget->m_iHP;

	llHP					=	llHP*60000/llMaxHP;
	packet.wRemainTargetHP	=	(WORD)llHP;

	if	(_lpHitInfo->m_dwPhysicalDamage)
		packet.dwPhysicalDamage	=	max(packet.dwPhysicalDamage,1);
	if	(_lpHitInfo->m_dwMagicDamage)
		packet.dwMagicDamage	=	max(packet.dwMagicDamage,1);
	if	(_lpHitInfo->m_dwAbsorbDamage)
		packet.wAbsorbDamage	=	max(packet.wAbsorbDamage,1);

	addSendPacket(&packet,_lpCaster->m_wSerialInField);
}
void
cFIELD::addSendFieldTrapWork(int _iTrapIndex,int _iWork,int _iSpareValue)
{
	SG_FIELD_TRAP_WORK	packet;

	packet.base.set(sizeof(packet),dSG_FIELD_TRAP_WORK);
	packet.wTrapIndex	=	_iTrapIndex;
	packet.wWork		=	_iWork;
	packet.wValue1		=	_iSpareValue;

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::addSendDoorMessage(int _iDoor,int _iKey,int _iSkill,int _iMessage,cACTOR *_lpCaster)
{
	SG_DOOR_MESSAGE	packet;

	packet.base.set(sizeof(packet),dSG_DOOR_MESSAGE);

	packet.wArea	=	_iDoor;
	packet.wKey		=	_iKey;
	packet.wSkill	=	_iSkill;
	packet.wMessage	=	_iMessage;

	if (_lpCaster)
		strcpy(packet.strCasterName,_lpCaster->m_strName);
	else
		strcpy(packet.strCasterName,"");

	packet.base.wSize=	sizeof(packet)-sizeof(packet.strCasterName)+strlen(packet.strCasterName)+1;

	addSendPacket((ALL_MSG*)&packet,_lpCaster->m_wSerialInField);
}

void
cFIELD::addPacketAddPortal(int _iPortalSerial,int _iX,int _iY,int _iOwnerSerial,int _iField,BOOL _bIsTownPortal)
{
	SG_ADD_PORTAL	packet;

	packet.base.set(sizeof(packet),dSG_ADD_PORTAL);

	packet.wPortalSerial=	_iPortalSerial;
	packet.wX			=	_iX;
	packet.wY			=	_iY;
	packet.wField		=	_iField;
	packet.bf12Owner	=	_iOwnerSerial;
	packet.bf1IsTownPortal=	_bIsTownPortal;

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::addSendAddTrap(cTrap *_lpTrap)
{
	SG_ADD_TRAP		packet;

	packet.base.set(sizeof(packet),dSG_ADD_TRAP);

	packet.wSerial	=	_lpTrap->m_wSerial;
	packet.wPosX	=	_lpTrap->m_pos.x;
	packet.wPosY	=	_lpTrap->m_pos.y;
	packet.wRange	=	_lpTrap->m_wRange;
	packet.bf3Team	=	_lpTrap->m_wTeam;
	packet.bf8DetectLevel=	_lpTrap->m_wDetectLevel;
	packet.wSkill	=	_lpTrap->getAbility()->m_wSkill;
	
	if	(packet.wRange	==	0)
		packet.wRange	=	_lpTrap->getAbility()->getHitRange();

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::addSendRemoveTrap(int _iSerial)
{
	SG_REMOVE_TRAP		packet;

	packet.base.set(sizeof(packet),dSG_REMOVE_TRAP);
	packet.wSerial	=	_iSerial;

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::addSendUpdateArcaInfo(cArca *_lpArca)
{
	SG_ADD_ARCA	packet;

	packet.base.set(sizeof(packet),dSG_ADD_ARCA);

	packet.arcaInfo.m_wSerial			=	_lpArca->m_wSerial;
	packet.arcaInfo.m_wX				=	_lpArca->m_pos.x;
	packet.arcaInfo.m_wY				=	_lpArca->m_pos.y;
	packet.arcaInfo.m_bf8SecretLevel	=	_lpArca->m_bSecretLevel;
	packet.arcaInfo.m_bf1IsDestroyAble	=	_lpArca->m_bf1IsDestroyAble;
	packet.arcaInfo.m_bf1IsLocked		=	FALSE;

	if	(_lpArca->m_bLockLevel)
		packet.arcaInfo.m_bf1IsLocked	=	TRUE;

	addSendPacket((ALL_MSG*)&packet,0xffff);
}

void
cFIELD::sendEtcWorkInArea(int _iArea,int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;

	sendPacketToInAreaActor((ALL_MSG*)&packet,_iArea);
}
void
cFIELD::addSendEtcWork(int _iSerial,int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ETC_WORK);

	if	(_iSerial	!=	0xffff)
		packet.base.wType=	dSG_CHECK_RANGE_ETC_WORK;

	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;

	addSendPacket((ALL_MSG*)&packet,_iSerial);
}

void
cFIELD::sendSimpleInfoToInAreaActor(int _iArea , cACTOR* _lpAsker)
{
	cAreaInfo	*lpArea	=	getArea(_iArea);
	
	if	(!lpArea)
		return;

	if(!_lpAsker)
		return;
	
	for (int i=0;i < dMAX_ACTOR_IN_FIELD;i++)
	{
		cACTOR *lpActor		=	m_alpActor[i];
		
		if	(!lpActor)
			continue;
		
		if	(!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
			continue;
		
		lpActor->addSendVerySimpleInfo(_lpAsker);
	}
}

void
cFIELD::sendPacketToInAreaActor(ALL_MSG *_lpMsg,int _iArea)
{
	cAreaInfo	*lpArea	=	getArea(_iArea);

	if	(!lpArea)
		return;
		
	for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		for (int i=m_aiActorCount[iTeam];i;i--)
		{
			cACTOR *lpActor		=	getActor(m_awActorList[iTeam][i-1]);

			if	(!lpActor || !lpActor->isPlayer())
				continue;

			if	(!lpArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			g_userPM.add(lpActor->m_iClientSerial,(void *)_lpMsg,_lpMsg->base.wSize);
		}
}

void
cFIELD::addSendRemoveActor(int _iSerial,BOOL _bIsHide)
{
	SG_REMOVE_ACTOR	packetRemoveActor;

	packetRemoveActor.base.set(sizeof(SG_REMOVE_ACTOR),dSG_REMOVE_ACTOR);
	packetRemoveActor.bf15Actor	=	_iSerial;
	packetRemoveActor.bf1IsHide	=	_bIsHide;

	addSendPacket((ALL_MSG*)&packetRemoveActor,0xffff);
}


void
cFIELD::addSendGuildBattleInfo(int _iTarget)
{
	SG_GUILD_BATTLE_INFO	packet;

	packet.base.set(sizeof(packet),dSG_GUILD_BATTLE_INFO);
	packet.wIsTrialGame	=	m_bIsTrialGameBattle;
	
	if	(m_wDuelType	==	0xffff)
	{
		CGuild	*lpGuild1		=	g_guildManager.getBySerial(m_aBattleGuild[0].m_wSerial);
		CGuild	*lpGuild2		=	g_guildManager.getBySerial(m_aBattleGuild[1].m_wSerial);

		if	(lpGuild1 == NULL ||  lpGuild2 == NULL)
			return;

		strcpy(packet.strTeam1Name,lpGuild1->m_strName);
		strcpy(packet.strTeam2Name,lpGuild2->m_strName);
		packet.iGuild1KillPoint	=	m_aBattleGuild[0].m_iKillPoint;
		packet.iGuild2KillPoint	=	m_aBattleGuild[1].m_iKillPoint;
	}
	else
	{
		if(STRICMP(m_aDuelTeam[0].m_strTeamName ,"" ) == NULL)
			return;

		if(STRICMP(m_aDuelTeam[1].m_strTeamName ,"" ) == NULL)
			return;

		strcpy(packet.strTeam1Name,m_aDuelTeam[0].m_strTeamName);
		strcpy(packet.strTeam2Name,m_aDuelTeam[1].m_strTeamName);

		packet.iGuild1KillPoint	=	m_awKillCount[0];
		packet.iGuild2KillPoint	=	m_awKillCount[1];
	}

	if	(_iTarget	!=	0xffff)
	{
		cACTOR	*lpTarget		=	getActor(_iTarget);

		if	(!lpTarget)
			return;

		g_userPM.add(lpTarget->m_iClientSerial,&packet,packet.base.wSize);
	}
	else
		addSendPacket((ALL_MSG*)&packet,0xffff);
}