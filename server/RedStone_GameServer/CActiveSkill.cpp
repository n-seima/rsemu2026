#include	"CActiveSkill.H"
#include	"cACTOR.H"
#include	"CGame.H"
#include	"cArea.H"
#include	"ast_mirror_image.h"
#include	"ast_giga_lightning.h"
#include	"ast_chain_lightning.h"
#include	"ast_fire_wall.h"
#include	"astBigBullet.h"
#include	"astDoubleTarget.h"
#include	"astFlorenceSpecial.h"
#include	"astLightningWinder.h"
#include	"astRollingLog.h"
#include	"astThrowItem.h"
#include	"astDependOnImageTrap.h"
#include	"astExplosionWithImage.h"
#include	"ast_gas.h"
#include	"ast_one_bite.h"
#include	"ast_ContinuousWhirlwind.h"
#include	"astMultiMissile.h"	// 멀티미사일
#include	"astContinuousHitOfDoppelganger.h"	// 분신 연속공격
#include	"astBouncingMissile.h"
#include	"astWideMissile.h"
#include	"GameDLL.h"

BOOL
cActiveSkillBaseInfo::isActionTime()
{
	return	FPIsActionTime(m_dwBookedActionTime,g_dwCurrentTime);
}

void
cActiveSkillBaseInfo::getActionTime()
{
	m_lpCaster->setActionSpeed(&m_ability);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.

	m_dwBookedActionTime	=	m_lpCaster->getFirstTriggerTime();
}

void
cActiveSkillBaseInfo::setCastingTimeByCasterDamageFrame(int _iTimes)
{
	if	(_iTimes	==	0)
		m_dwBookedActionTime	=	m_lpCaster->getDamageFrameTime(_iTimes);
	else
	{
		DWORD	dwLastTime		=	m_lpCaster->getDamageFrameTime(_iTimes-1);
		DWORD	dwDestTime		=	m_lpCaster->getDamageFrameTime(_iTimes);

		if	(dwLastTime	==	0xffffffff	||	dwDestTime	==	0xffffffff	||	dwLastTime	>	dwDestTime)
			m_dwBookedActionTime=	0xffffffff;
		else
			m_dwBookedActionTime	=	g_dwCurrentTime+(dwDestTime-dwLastTime)/dREVISE_ACTION_SPEED_FOR_TEST;
	}
}
//
//	리셋
void
CActiveSkill::reset()
{
	if	(m_lpCaster	&&	m_lpCaster->m_wSerialInField	!=	0xffff)
	{
		if	(m_ability.isHideShield()			)
			m_lpCaster->m_wIsHideShield	=	FALSE;

		if	(m_lpCaster->m_wUseSkill	==	m_wSerial)
			m_lpCaster->m_wUseSkill		=	0xffff;
		
		if	(m_lpSkill)
		{
			if	(m_lpCaster->m_wMachine	!=	0xffff)
				if	(m_lpCaster->m_wMachine	>=	0x8000)
					m_lpCaster->m_wMachine	-=	0x8000;

			if	(m_wSkillType	==	eSKILL_TYPE_CHARGE)
				m_lpCaster->sendChargeCP(m_lpSkill->m_wSerial);

			if	(m_wSkillType	==	eSKILL_TYPE_FIRE_WALL)
			{
				cAS_FireWall*	lpFW	=	(cAS_FireWall*)this;

				lpFW->removeFireWall();
			}
			else
			if	(m_wSkillType	==	eSKILL_TYPE_MACHINE_GUN)
			{
				if	(m_lpCaster->m_wMachine	==	m_wSerial)
					m_lpCaster->releaseMachine();
			}
			else
			if	(m_wSkillType	==	eSKILL_TYPE_GUARDIAN_POST)
			{
				if	(m_lpCaster->m_wMachine	==	m_wSerial)
				{
					m_lpCaster->m_wMachine			=	0xffff;;
					m_lpCaster->sendPullUpGuardianPost(m_wSerial);

					int	iJob	=	m_lpCaster->m_wJob;

					if	(!m_lpSkill->isEnableJob(m_lpCaster->m_wJob))
						iJob	=	m_lpCaster->getAnotherJob();

					if	((iJob%2)	==	0)
						m_lpCaster->m_wHideWeapon1Time			=	0;
					else
						m_lpCaster->m_wHideWeapon2Time			=	0;
				}
			}
			else
			if	(m_wSkillType	==	eSKILL_TYPE_ICY_STALAGMITE)
			{
				if	(m_lpCaster->m_wHillSkill==	m_wSerial)
					m_lpCaster->m_wHillSkill=	0xffff;
			}
			else
			if	(m_wSkillType	==	eSKILL_TYPE_BITE_HANGING)
			{
				if	(m_lpCaster	&&	m_lpCaster->m_wSerialInField	!=	0xffff)
				{
					m_lpCaster->m_wBiteTarget	=	0xffff;
					m_lpCaster->addPacketStop();
				}
				if	(m_lpTarget	&&	m_lpTarget->m_wSerialInField	!=	0xffff)
				{
					m_lpTarget->m_wBiter		=	0xffff;
					m_lpTarget->addPacketStop();
					m_lpTarget->removeEnchantedMagic(m_lpSkill->m_wSerial);
				}
			}
			else
			if	(m_wSkillType	==	eSKILL_TYPE_SHIMMERING_SHIELD)
			{
				releaseShimmeringShield();
			}				
		}
	}

	if	(m_lpTarget	&&	m_lpTarget->m_wSerialInField	!=	0xffff)
	{
		if	(m_wSkillType	==	eSKILL_TYPE_BIG_BULLET)
			m_lpTarget->m_wCaughtTime	=	0;
	}

	m_wSerial		=	0xffff;
	m_lpCaster		=	NULL;
	m_lpTarget		=	NULL;
	m_lpDoor		=	NULL;

	m_isNotSpentCP	=	FALSE;
	m_wIsGetdCP		=	FALSE;
	m_hitInfo.reset();

	memset(m_aValue,0xff,sizeof(m_aValue));
}

void
CActiveSkill::init()
{
	m_iCasterZoneSerial	=	m_lpCaster->m_iZoneSerial;
	m_wActiveField		=	m_lpCaster->m_wCurrentField;

	if (m_lpTarget)	
		m_iTargetZoneSerial	=	m_lpTarget->m_iZoneSerial;
	else
		m_iTargetZoneSerial	=	0xffffffff;

	m_wAnm							=	0;	//	초기화
	m_wDirect						=	0;
	m_wFrame						=	0;
	m_wFrameCounter					=	0xffff;
	m_lpCaster->m_wRemainActionTime	=	0;

}

//
//	사용!! 
BOOL
CActiveSkill::fire(int  *_lpiResult)
{
	if	(m_lpCaster->isDeath())
		return	FALSE;	//	죽었어!!

	if	(m_lpSkill->m_wLimitActiveCount	!=	0xffff)
	{
		int	iCount	=	m_lpCaster->m_lpField->m_skillManager.getActiveSkillCount(m_lpSkill->m_wSerial,m_lpCaster->m_iZoneSerial);

		if	(iCount	>=	m_lpSkill->m_wLimitActiveCount)
		{
			m_lpCaster->sendEtcWork(eEW_IS_LIMIT_ACTIVE_COUNT_SKILL,m_lpSkill->m_wSerial,m_lpSkill->m_wLimitActiveCount);

			*_lpiResult	=	eSKILL_FIRE_RESULT_FAILED;
			m_wSerial	=	0xffff;

			return	FALSE;
		}
	}

	init();

	m_wSkillType					=	m_lpSkill->m_wType;

	int	iFireResult;

	switch(m_wSkillType)
	{
		case	eSKILL_TYPE_MIRROR_IMAGE				: //	"미러이미지"			:
			iFireResult	=	((cAS_MirrorImage*)this)->fire();
			break;
		case	eSKILL_TYPE_CHAIN_LIGHTNING				: //	"체인라이트닝"			:
			iFireResult	=	((cAS_ChainLightning*)this)->fire();
			break;
		case	eSKILL_TYPE_FIRE_WALL					: //	"파이어월"				:
			iFireResult	=	((cAS_FireWall*)this)->fire();
			break;
		case	eSKILL_TYPE_GIGA_LIGHTNING				: //	"기가라이트닝"			:
			iFireResult	=	((cAS_GigaLightning*)this)->fire();
			break;
		case	eSKILL_TYPE_ONE_BITE					: //	"한입"			:
			iFireResult	=	((cAS_OneBite*)this)->fire();
			break;
		case	eSKILL_TYPE_BOUNCING_MISSILE					: //	바운싱 미사일
			iFireResult	=	((cAS_BouncingMissile*)this)->fire();
			break;	
		case	eSKILL_TYPE_WIDE_MISSILE					: //	범위 미사일
			iFireResult	=	((cAS_WideMissile*)this)->fire();
			break;		


		case	eSKILL_TYPE_DOUBLE_TARGET				:	iFireResult	=	fireDoubleTarget();break;//	바운싱 리니어
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT		:	iFireResult	=	fireContinuousBit();break;
		case	eSKILL_TYPE_BOUNCING_LINEAR				:	iFireResult	=	fireBouncingLinear();break;//	바운싱 리니어
		case	eSKILL_TYPE_IMPACT						:	iFireResult	=	fireImpactSkill();break;//	때리고 주위에 충격파가 생긴다.
		case	eSKILL_TYPE_ALTERING_HITTER				:	iFireResult	=	fireAlteringHitter();break;//연속 공격
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:	iFireResult	=	fireContinuousAttack();break;//연속 공격
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:	iFireResult	=	fireContinuousAttack();break;//연속 공격
		case	eSKILL_TYPE_BIT_GLIDER					:	iFireResult	=	fireBitGlider();break;//비트 글라이더
		case	eSKILL_TYPE_LAND_MARKER					:	iFireResult	=	fireLandMarker();break;//랜드 마커
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:	iFireResult	=	fireIntervalShooter();break;//인터벌 슈터
		case	eSKILL_TYPE_WATER_FALL					:	iFireResult	=	fireWaterFall();break;//워터 폴

		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	iFireResult	=	fireBunshineAttack();break;//연속 공격
		case	eSKILL_TYPE_AWESOME_FORTRESS			:	iFireResult	=	fireAwesomeFortress();break;//오우섬 포트리스
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:	iFireResult	=	fireContinuousHitOfDoppelganger();break;// 분신 연속 공격
			
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:	iFireResult	=	fireScimitarCutting();break;
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLWIND					:	iFireResult	=	fireWhirlwindTypeSkill();break;//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_TORNADO						:	iFireResult	=	fireTornadoTypeSkill();break;//토네이도~
		case	eSKILL_TYPE_MISSILE						:	iFireResult	=	fireMissileTypeSkill();break;//미사일

		case	eSKILL_TYPE_LIGHTNING_WINDER			:
			iFireResult	=	((cAS_LightningWinder*)this)->fire();
			break;
		case	eSKILL_TYPE_MULTI_MISSILE			:		// 멀티미사일..
			iFireResult	=	((cAS_MultiMissile*)this)->fire();
			break;

		case	eSKILL_TYPE_THROW_ITEM					:
			iFireResult	=	((cAS_ThrowItem *)this)->fire();
			break;

		case	eSKILL_TYPE_BIG_BULLET					:
			iFireResult	=	((cAS_BigBullet *)this)->fire();
			break;

		case	eSKILL_TYPE_ROLLING_LOG					:
			iFireResult	=	((cAS_RollingLog *)this)->fire();
			break;

		case	eSKILL_TYPE_EXPLOSION_MISSILE			:	iFireResult	=	fireExplosionMissile();break;//미사일
		case	eSKILL_TYPE_SPREAD_ARROW				:	iFireResult	=	fireSpreadArrow();break;//미사일

		case	eSKILL_TYPE_BOOMERANG					:	iFireResult	=	fireBoomerang();break;//미사일
		case	eSKILL_TYPE_PIERCING_MISSILE			:	iFireResult	=	firePiercingMissileTypeSkill();break;//관통형미사일
		case	eSKILL_TYPE_DROP_ON_HEAD				:	iFireResult	=	fireDropOnHeadTypeSkill();break;//머리위로 떨어지는 스킬
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
		case	eSKILL_TYPE_LASER						:	iFireResult	=	fireLaserTypeSkill();break;//레이저
		case	eSKILL_TYPE_HEAL						:	iFireResult	=	fireHealTypeSkill();break;//힐
		case	eSKILL_TYPE_FIRE_BALL					:	iFireResult	=	fireFireBallTypeSkill();break;//파이어볼

		case	eSKILL_TYPE_GAS							:	
			iFireResult	=	((cAS_Gas*)this)->fire();
			break;
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:	
			iFireResult	=	((cAS_ExplosionWithImage *)this)->fire();
			break;
		case	eSKILL_TYPE_RUSH						:	iFireResult	=	fireRushTypeSkill();break;//레이저
		case	eSKILL_TYPE_MIRROR_TOWER				:	iFireResult	=	fireMirrorTower();break;//레이저
		case	eSKILL_TYPE_GUARDIAN_POST				:	iFireResult	=	fireGuardianPost();break;//가디언 포스트

		case	eSKILL_TYPE_FLORENCE_SPECIAL			:	
			iFireResult	=	((cAS_FlorenceSpecial *)this)->fire();
			break;
//		case	eSKILL_TYPE_ILLUSION_MISSILE			:	iFireResult	=	fireIllusionMissileTypeSkill();break;//환상 미사일
//		case	eSKILL_TYPE_EXPLOSION					:	iFireResult	=	fireExplosionTypeSkill();break;//폭발형

		case	eSKILL_TYPE_SHIMMERING_SHIELD			:	iFireResult	=	fireShimmeringShield();break;//시머링 실드

		case	eSKILL_TYPE_MACHINE_GUN					:	iFireResult	=	fireMachineTypeSkill();break;//머신 
		case	eSKILL_TYPE_AID_MAGIC					:	iFireResult	=	fireAidMagic();break;	//	보조마법
		case	eSKILL_TYPE_AID_ATTACK					:	iFireResult	=	fireAidAttack();break;
		case	eSKILL_TYPE_CHARGE						:	iFireResult	=	fireChargeSkill();break;
		case	eSKILL_TYPE_ICY_STALAGMITE				:	iFireResult	=	fireIcyStalagmite();break;
		case	eSKILL_TYPE_TELEPORT					:	iFireResult	=	fireTeleport();break;
		case	eSKILL_TYPE_JUMP						:	iFireResult	=	fireJump();break;
		case	eSKILL_TYPE_WILD_STAMP					:	iFireResult	=	fireJump(TRUE);break;
		case	eSKILL_TYPE_JUMP_ATTACK					:	iFireResult	=	fireJumpAttack();break;
		
		case	eSKILL_TYPE_WHIRLTHRUSTING				:	iFireResult	=	fireWhirlthrusting();break;
		case	eSKILL_TYPE_SURPRISING_RAID				:	iFireResult	=	fireSurprisingRaid();break;
		case	eSKILL_TYPE_BITE_HANGING				:	iFireResult	=	fireBiteHanging();break;
		case	eSKILL_TYPE_PET_COMMAND					:	iFireResult	=	firePetCommand();break;
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		:	iFireResult	=	((cAS_ContinuousWhirlwind*)this)->fire();break;		// 지속형라운딩.
	}

	*_lpiResult	=	iFireResult;

	if	(iFireResult	!=	eSKILL_FIRE_RESULT_OK)
	{
		m_wSerial	=	0xffff;

		return FALSE;	//	스킬 사용이 실패했다.!!!!!!??
	}

	m_lpCaster->m_attackInfo.m_ability.copy(&m_ability);

	if	(m_ability.getDodgeChance())
		m_lpCaster->m_dodgeChance.correctValue(m_ability.getDodgeChance(),m_ability.getUpkeepTime());

	m_lpCaster->setExclusiveAction(TRUE);

	if	(m_lpSkill->m_bf4ChangeDirect)
		m_lpCaster->m_wDirect	=	(WORD)(m_lpSkill->m_bf4ChangeDirect-1);
	
	if	(m_lpSkill->m_wSpentHPPercentage)
	{
		LONGLONG	llSpentHP	=	m_lpCaster->getMaxHP();

		llSpentHP			=	llSpentHP*m_lpSkill->m_wSpentHPPercentage/10000;
		int	iSpentHP		=	min(max(m_lpCaster->m_iHP-100,0),(int)llSpentHP);

		m_lpCaster->reduceHP(iSpentHP);
		m_lpCaster->addSendSimpleInfo();
	}

	if	(m_lpSkill->m_wSpentHPPercentageBasedBloodWing && m_hitInfo.isHit())
	{
		LONGLONG	llSpentHP	=	m_hitInfo.m_dwMagicDamage;

		llSpentHP			=	llSpentHP*m_lpSkill->m_wSpentHPPercentageBasedBloodWing/10000;
		int	iSpentHP		=	min(max(m_lpCaster->m_iHP-100,0),(int)llSpentHP);

		m_lpCaster->reduceHP(iSpentHP);
		m_lpCaster->addSendSimpleInfo();
	}

	m_lpCaster->m_wIsDontHitAction	=	m_lpSkill->isNotInterrupt();

	m_lpCaster->m_wDirect			=	m_lpCaster->getDirect(m_posTarget.x,m_posTarget.y,TRUE);
	
	if	(m_lpSkill->m_bf1IsCastSelfBuff)
		m_lpCaster->applySkillExtraEffects(m_lpCaster,&m_ability);

	return	TRUE;
}

//
//	사용!!
BOOL
CActiveSkill::update()
{
	if	(m_lpCaster	==	NULL)
		return	TRUE;
	if	(m_lpCaster->isDeath())
		return	TRUE;
	if	(m_lpCaster->m_wSerialInField == 0xffff)
		return	TRUE;

	if	(m_iCasterZoneSerial			!=	m_lpCaster->m_iZoneSerial)
		return	TRUE;
	if	(m_lpCaster->m_wCurrentField	!=	m_wActiveField)
		return	TRUE;
	if	(m_iCasterZoneSerial			!= m_lpCaster->m_iZoneSerial)
		return	TRUE;
	if	(m_lpTarget && (m_lpTarget->m_wSerialInField	==	0xffff || m_lpTarget->m_wCurrentField != m_lpCaster->m_wCurrentField))
		return	TRUE;

	switch(m_wSkillType)
	{
		case	eSKILL_TYPE_MIRROR_IMAGE				: //	"미러이미지"			:
			return	((cAS_MirrorImage*)this)->update();

		case	eSKILL_TYPE_CHAIN_LIGHTNING				: //	"체인라이트닝"			:
			return	((cAS_ChainLightning*)this)->update();

		case	eSKILL_TYPE_FIRE_WALL					: //	"파이어월"				:
			return	((cAS_FireWall*)this)->update();

		case	eSKILL_TYPE_GIGA_LIGHTNING				: //	"기가라이트닝"			:
			return	((cAS_GigaLightning*)this)->update();

		case	eSKILL_TYPE_ONE_BITE						: //	"한입"			:
			return	((cAS_OneBite*)this)->update();

		case	eSKILL_TYPE_BOUNCING_MISSILE						: //	"바운싱미사일"			:
			return	((cAS_BouncingMissile*)this)->update();

		case	eSKILL_TYPE_WIDE_MISSILE						: //	"범위 미사일"			:
			return	((cAS_WideMissile*)this)->update();

		case	eSKILL_TYPE_DOUBLE_TARGET				:	return	updateDoubleTarget();	//	더블 타겟
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT		:	return	updateContinuousBit();	//	비트 그룹연타
		case	eSKILL_TYPE_BOUNCING_LINEAR				:	return	updateBouncingLinear();//	바운싱 리니어
		case	eSKILL_TYPE_IMPACT						:	return	updateImpactSkill();//	때리고 주위에 충격파가 생긴다.
		case	eSKILL_TYPE_ALTERING_HITTER				:	return	updateAlteringHitter();//연속 공격
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:	return	updateContinuousAttack();//연속 공격
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:	return	updateContinuousAttack();//연속 공격
		case	eSKILL_TYPE_BIT_GLIDER					:	return	updateBitGlider();//비트 글라이더
		case	eSKILL_TYPE_LAND_MARKER					:	return	updateLandMarker();//랜드 마커
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:	return	updateIntervalShooter();//인터벌 슈터
		case	eSKILL_TYPE_WATER_FALL					:	return	updateWaterFall();//워터 폴

		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	return	updateBunshineAttack();//연속 공격
		case	eSKILL_TYPE_AWESOME_FORTRESS			:	return	updateAwesomeFortress();//오우섬 포트리스
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:	return	updateContinuousHitOfDoppelganger();// 분신 연속 공격
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:	return	updateScimitarCutting();
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLWIND					:	return	updateWhirlwindTypeSkill();//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_TORNADO						:	return	updateTornadoTypeSkill();//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_MISSILE						:	return	updateMissileTypeSkill();//미사일

		case	eSKILL_TYPE_LIGHTNING_WINDER			:
			return	((cAS_LightningWinder*)this)->update();
		case	eSKILL_TYPE_MULTI_MISSILE			:		// 멀티미사일..
			return	((cAS_MultiMissile*)this)->update();

		case	eSKILL_TYPE_THROW_ITEM					:
			return	((cAS_ThrowItem *)this)->update();

		case	eSKILL_TYPE_BIG_BULLET					:
			return	((cAS_BigBullet *)this)->update();

		case	eSKILL_TYPE_ROLLING_LOG					:
			return	((cAS_RollingLog *)this)->update();

		case	eSKILL_TYPE_EXPLOSION_MISSILE			:	return	updateExplosionMissile();//미사일
		case	eSKILL_TYPE_SPREAD_ARROW				:	return	updateSpreadArrow();//미사일

		case	eSKILL_TYPE_BOOMERANG					:	return	updateBoomerang();//부메랑
		case	eSKILL_TYPE_PIERCING_MISSILE			:	return	updatePiercingMissileTypeSkill();//관통형미사일
		case	eSKILL_TYPE_DROP_ON_HEAD				:	return	updateDropOnHeadTypeSkill();//머리위로 떨어지는 스킬
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
		case	eSKILL_TYPE_LASER						:	return	updateLaserTypeSkill();//레이저
		case	eSKILL_TYPE_HEAL						:	return	updateHealTypeSkill();//힐타입 스킬
		case	eSKILL_TYPE_FIRE_BALL					:	return	updateFireBallTypeSkill();//파이어볼

		case	eSKILL_TYPE_TRAP						:	return	updateDefendOnImageTrap();
		case	eSKILL_TYPE_GAS							:	
			return	((cAS_Gas*)this)->update();					

		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:	
			return	((cAS_ExplosionWithImage *)this)->update();

		case	eSKILL_TYPE_RUSH						:	return	updateRushTypeSkill();//러쉬 타입 스킬
		case	eSKILL_TYPE_MIRROR_TOWER				:	return	updateMirrorTower();//미러타워
		case	eSKILL_TYPE_GUARDIAN_POST				:	return	updateGuardianPost();//가디언 포스트

		case	eSKILL_TYPE_FLORENCE_SPECIAL			:	
			return	((cAS_FlorenceSpecial *)this)->update();

//		case	eSKILL_TYPE_ILLUSION_MISSILE			:	return	updateIllusionMissileTypeSkill();//환상 미사일
//		case	eSKILL_TYPE_EXPLOSION					:	updateExplosionTypeSkill();break;//폭발형
//		case	eSKILL_TYPE_SHOOT						:	break;
		case	eSKILL_TYPE_SHIMMERING_SHIELD			:	return	updateShimmeringShield();//시머링 실드

		case	eSKILL_TYPE_MACHINE_GUN					:	return	updateMachineTypeSkill();//머신건
		case	eSKILL_TYPE_AID_MAGIC					:	return	updateAidMagic();	//	보조마법
		case	eSKILL_TYPE_AID_ATTACK					:	return	updateAidAttack();	//	보조마법
		case	eSKILL_TYPE_CHARGE						:	return	updateChargeSkill();
		case	eSKILL_TYPE_ICY_STALAGMITE				:	return	updateIcyStalagmite();
		case	eSKILL_TYPE_TELEPORT					:	return	updateTeleport();
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP						:	return	updateJump();
		case	eSKILL_TYPE_JUMP_ATTACK					:	return	updateJumpAttack();

		case	eSKILL_TYPE_WHIRLTHRUSTING				:	return	updateWhirlthrusting();
		case	eSKILL_TYPE_SURPRISING_RAID				:	return	updateSurprisingRaid();
		case	eSKILL_TYPE_BITE_HANGING				:	return	updateBiteHanging();
		case	eSKILL_TYPE_PET_COMMAND					:	return	updatePetCommand();
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND				:	return ((cAS_ContinuousWhirlwind*)this)->update();		// 지속형라운딩
	}

	return	TRUE;
}

//
//	캐스가 정지했다.
BOOL
CActiveSkill::casterStop()
{
	if	(m_wSerial	==	0xffff)
		return TRUE;

	switch(m_wSkillType)
	{
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLWIND					:
		case	eSKILL_TYPE_IMPACT						:
		case	eSKILL_TYPE_LASER						:
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
		case	eSKILL_TYPE_PIERCING_MISSILE			:
		case	eSKILL_TYPE_HEAL						:
		case	eSKILL_TYPE_AID_MAGIC					:
		case	eSKILL_TYPE_AID_ATTACK					:
		case	eSKILL_TYPE_FIRE_BALL					:
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:
		case	eSKILL_TYPE_BIT_GLIDER					:
		case	eSKILL_TYPE_LAND_MARKER					:
		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	//연속 공격
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:
		case	eSKILL_TYPE_TELEPORT					:
		case	eSKILL_TYPE_MIRROR_TOWER				:
		case	eSKILL_TYPE_SPREAD_ARROW				:
		case	eSKILL_TYPE_BITE_HANGING				:
		case	eSKILL_TYPE_WIDE_MISSILE				:
			reset();
			return	TRUE;

		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_DROP_ON_HEAD				:
		case	eSKILL_TYPE_GAS							:

		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:
		case	eSKILL_TYPE_MACHINE_GUN					:
		case	eSKILL_TYPE_BOOMERANG					:
		case	eSKILL_TYPE_TORNADO						:
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:
		case	eSKILL_TYPE_THROW_ITEM					:
			if	(m_wFrameCounter	!=	0xffff	)
				return	FALSE;
			reset();
			return	TRUE;
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		:	//지속형라운딩
			return FALSE;
		case	eSKILL_TYPE_BIG_BULLET					:
		{
			cAS_BigBullet *lpAS	=	(cAS_BigBullet *)this;

			if	(lpAS->m_wStep	==	eBBS_READY)
			{
				reset();
				return	TRUE;
			}

			return	FALSE;
		}

		case	eSKILL_TYPE_GUARDIAN_POST				:
		{
			if	(isActiveGuardianPost()	==	FALSE)
			{
				reset();
				return	TRUE;
			}
			return	FALSE;
		}

		case	eSKILL_TYPE_ICY_STALAGMITE				:
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP						:
		case	eSKILL_TYPE_JUMP_ATTACK					:
		case	eSKILL_TYPE_ALTERING_HITTER				:
		case	eSKILL_TYPE_WHIRLTHRUSTING				:
		case	eSKILL_TYPE_SURPRISING_RAID				:
		case	eSKILL_TYPE_RUSH						:	
		case	eSKILL_TYPE_FLORENCE_SPECIAL			:	
		case	eSKILL_TYPE_BOUNCING_LINEAR				:
		case	eSKILL_TYPE_PET_COMMAND					:
		case	eSKILL_TYPE_DOUBLE_TARGET				:
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT		:
		case	eSKILL_TYPE_TRAP						:
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:
		case	eSKILL_TYPE_ROLLING_LOG					:
		case	eSKILL_TYPE_LIGHTNING_WINDER			:
		case	eSKILL_TYPE_MULTI_MISSILE				:	// 멀티미사일.
		case	eSKILL_TYPE_MIRROR_IMAGE				: //	"미러이미지"			:
		case	eSKILL_TYPE_CHAIN_LIGHTNING				: //	"체인라이트닝"			:
		case	eSKILL_TYPE_FIRE_WALL					: //	"파이어월"				:
		case	eSKILL_TYPE_GIGA_LIGHTNING				: //	"기가라이트닝"			:
		case	eSKILL_TYPE_ONE_BITE						: //	"한입"			:
			return	FALSE;

		case	eSKILL_TYPE_WATER_FALL					:	//워터 폴
			if (isWaterFallCasting())
				return	FALSE;
			return	TRUE;

		case	eSKILL_TYPE_AWESOME_FORTRESS			:
			if (isAwesomeFortressTriggerAction())
				return	TRUE;
			return	FALSE;
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:
			if (isContinuousHitOfDoppelgangerTriggerAction())
				return	TRUE;
			return	FALSE;
	}

	reset();

	return	TRUE;
}

//
//	캐스가 죽었다.
BOOL
CActiveSkill::casterDeath()
{
	switch(m_wSkillType)
	{
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLWIND					:
		case	eSKILL_TYPE_IMPACT						:
		case	eSKILL_TYPE_LASER						:
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
		case	eSKILL_TYPE_PIERCING_MISSILE			:
		case	eSKILL_TYPE_HEAL						:
		case	eSKILL_TYPE_MACHINE_GUN					:
		case	eSKILL_TYPE_AID_MAGIC					:
		case	eSKILL_TYPE_AID_ATTACK					:
		case	eSKILL_TYPE_CHARGE						:
		case	eSKILL_TYPE_FIRE_BALL					:
		case	eSKILL_TYPE_ALTERING_HITTER				:
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:
		case	eSKILL_TYPE_BIT_GLIDER					:
		case	eSKILL_TYPE_LAND_MARKER					:
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:
		case	eSKILL_TYPE_WATER_FALL					:	//워터 폴
		case	eSKILL_TYPE_RUSH						:	
		case	eSKILL_TYPE_BOOMERANG					:	
		case	eSKILL_TYPE_TORNADO						:
		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	//연속 공격
		case	eSKILL_TYPE_AWESOME_FORTRESS			:
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:
		case	eSKILL_TYPE_ICY_STALAGMITE				:
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:
		case	eSKILL_TYPE_TELEPORT					:
		case	eSKILL_TYPE_JUMP						:
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP_ATTACK					:
		case	eSKILL_TYPE_MIRROR_TOWER				:
		case	eSKILL_TYPE_SPREAD_ARROW				:
		case	eSKILL_TYPE_WHIRLTHRUSTING				:
		case	eSKILL_TYPE_SURPRISING_RAID				:
		case	eSKILL_TYPE_GUARDIAN_POST				:
		case	eSKILL_TYPE_FLORENCE_SPECIAL			:	
		case	eSKILL_TYPE_BITE_HANGING				:
		case	eSKILL_TYPE_BOUNCING_LINEAR				:
		case	eSKILL_TYPE_PET_COMMAND					:
		case	eSKILL_TYPE_DOUBLE_TARGET				:
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT		:
		case	eSKILL_TYPE_TRAP						:
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:
		case	eSKILL_TYPE_GAS							:
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		:	//지속형라운딩.
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_DROP_ON_HEAD				:
		case	eSKILL_TYPE_BIG_BULLET					:
		case	eSKILL_TYPE_ROLLING_LOG					:
		case	eSKILL_TYPE_LIGHTNING_WINDER			:
		case	eSKILL_TYPE_MULTI_MISSILE				:	// 멀티미사일.
		case	eSKILL_TYPE_MIRROR_IMAGE				: //	"미러이미지"			:
		case	eSKILL_TYPE_CHAIN_LIGHTNING				: //	"체인라이트닝"			:
		case	eSKILL_TYPE_FIRE_WALL					: //	"파이어월"				:
		case	eSKILL_TYPE_GIGA_LIGHTNING				: //	"기가라이트닝"			:
		case	eSKILL_TYPE_ONE_BITE						: //	"한입"			:
		case	eSKILL_TYPE_BOUNCING_MISSILE						: //	"바운싱미사일"			:
		case	eSKILL_TYPE_WIDE_MISSILE						: //	"범위미사일"			:
			reset();
			return	TRUE;

		case	eSKILL_TYPE_SHIMMERING_SHIELD		:
			if	(m_wFrameCounter	!=	0xffff	)
				releaseShimmeringShield();

			reset();
			return	TRUE;
	}

	reset();

	return	TRUE;
}

//
//	타겟이 죽었다.
BOOL
CActiveSkill::targetDeath()
{
	switch(m_wSkillType)
	{
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLWIND					:
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
		case	eSKILL_TYPE_GAS							:
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		:	//지속형라운딩
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:
		case	eSKILL_TYPE_RUSH						:	
		case	eSKILL_TYPE_TORNADO						:
		case	eSKILL_TYPE_ICY_STALAGMITE				:
		case	eSKILL_TYPE_TELEPORT					:
		case	eSKILL_TYPE_JUMP						:
		case	eSKILL_TYPE_JUMP_ATTACK					:
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_ALTERING_HITTER				:
		case	eSKILL_TYPE_WHIRLTHRUSTING				:
		case	eSKILL_TYPE_SURPRISING_RAID				:
		case	eSKILL_TYPE_DOUBLE_TARGET				:
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT		:
		case	eSKILL_TYPE_TRAP						:
		case	eSKILL_TYPE_ROLLING_LOG					:
		case	eSKILL_TYPE_LIGHTNING_WINDER			:
		case	eSKILL_TYPE_MULTI_MISSILE				:	// 멀티미사일.
		case	eSKILL_TYPE_MIRROR_IMAGE				: //	"미러이미지"			:
		case	eSKILL_TYPE_CHAIN_LIGHTNING				: //	"체인라이트닝"			:
		case	eSKILL_TYPE_FIRE_WALL					: //	"파이어월"				:
		case	eSKILL_TYPE_GIGA_LIGHTNING				: //	"기가라이트닝"			:
		case	eSKILL_TYPE_ONE_BITE						: //	"한입"			:
		case	eSKILL_TYPE_BOUNCING_MISSILE						: //	"바운싱 미사일"			:
		case	eSKILL_TYPE_WIDE_MISSILE						: //	"범위 미사일
			m_lpTarget	=	NULL;
			return	FALSE;

		case	eSKILL_TYPE_LASER						:
		case	eSKILL_TYPE_HEAL						:
		case	eSKILL_TYPE_AID_MAGIC					:
		case	eSKILL_TYPE_AID_ATTACK					:
			if (m_lpTarget)
			{
				reset();
				return	TRUE;
			}
			return	FALSE;

		case	eSKILL_TYPE_PIERCING_MISSILE			:
		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_BIG_BULLET					:
		case	eSKILL_TYPE_IMPACT						:
		case	eSKILL_TYPE_DROP_ON_HEAD				:
		case	eSKILL_TYPE_FIRE_BALL					:
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:
		case	eSKILL_TYPE_BIT_GLIDER					:
		case	eSKILL_TYPE_LAND_MARKER					:
		case	eSKILL_TYPE_BOOMERANG					:	
		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	//연속 공격
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:
		case	eSKILL_TYPE_MIRROR_TOWER				:
		case	eSKILL_TYPE_BITE_HANGING				:
			reset();
			return	TRUE;

		case	eSKILL_TYPE_MACHINE_GUN					:
		case	eSKILL_TYPE_SPREAD_ARROW				:
		case	eSKILL_TYPE_WATER_FALL					:	//워터 폴
		case	eSKILL_TYPE_AWESOME_FORTRESS			:
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:

		case	eSKILL_TYPE_GUARDIAN_POST				:
		case	eSKILL_TYPE_FLORENCE_SPECIAL			:	
		case	eSKILL_TYPE_BOUNCING_LINEAR				:
		case	eSKILL_TYPE_PET_COMMAND					:
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:
			m_lpTarget	=	NULL;
			return	FALSE;

		case	eSKILL_TYPE_SHIMMERING_SHIELD			:
			if (m_wFrameCounter	!=	0xffff	)
				releaseShimmeringShield();
			reset();
			return	TRUE;
	}

	reset();

	return	TRUE;
}

//
//	
BOOL
CActiveSkill::isCaster(int _iIndex)
{
	if (m_lpCaster == NULL)
		return	FALSE;

	if (m_lpCaster->m_wSerialInField	==	_iIndex)
		return	TRUE;

	return	FALSE;
}

//	정상적인 타겟이다.
BOOL
CActiveSkill::isCorrectTarget()
{
	if (!m_lpTarget)
		return	FALSE;

	if (m_lpTarget->isDeath())
		return	FALSE;

	if (m_lpTarget->m_iZoneSerial	!= m_iTargetZoneSerial)
		return	FALSE;

	return	TRUE;
}

//
//	스킬을 액터한테 사용한다.
WORD
CActiveSkillManager::castAtActor(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility,int _iRange)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dMAX_ACTIVE_SKILL)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

//	printf("skill Count = %d\n",m_iActiveSkillCount);
	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpTarget;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wTeam		=	_lpCaster->m_wTeam;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_posTarget.x	=	_lpTarget->m_pos.x;
	lpAS->m_posTarget.y	=	_lpTarget->m_pos.y;
	lpAS->m_wRange		=	_iRange;

	if	(lpSkill->m_dwTargetMethod == eSKILL_CAST_QUICK)
	{
		lpAS->m_posTarget.x		=	_lpCaster->m_pos.x;
		lpAS->m_posTarget.y		=	_lpCaster->m_pos.y;
	}

	int	iResult;

	if	(!lpAS->fire(&iResult))
	{
		m_iLastError	=	iResult;
		return 0xffff;
	}

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dMAX_ACTIVE_SKILL;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	if (m_iActiveSkillCount	>	m_iMaxSkillCount)
		m_iMaxSkillCount	=	m_iActiveSkillCount;

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	퀵 캐스트
WORD
CActiveSkillManager::castQuick(cACTOR *_lpCaster,cAbility *_lpAbility,int _iRange,int *_lpiResult)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

//	printf("active skill count : %d\n",m_iActiveSkillCount);

	if (m_iActiveSkillCount	>=	dMAX_ACTIVE_SKILL)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpTarget	=	_lpCaster;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wTeam		=	_lpCaster->m_wTeam;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_posTarget.x	=	_lpCaster->m_pos.x;
	lpAS->m_posTarget.y	=	_lpCaster->m_pos.y;
	lpAS->m_wRange		=	_iRange;

	if	(!lpAS->fire(_lpiResult))
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dMAX_ACTIVE_SKILL;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	if (m_iActiveSkillCount	>	m_iMaxSkillCount)
		m_iMaxSkillCount	=	m_iActiveSkillCount;

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

WORD
CActiveSkillManager::castFireWall(cACTOR *_lpCaster,cAbility *_lpAbility,int _iX1,int _iY1,int _iX2,int _iY2)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if	(m_iActiveSkillCount	>=	dMAX_ACTIVE_SKILL)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wTeam		=	_lpCaster->m_wTeam;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_lpTarget	=	NULL;

	cAS_FireWall	*lpFireWall	=	(cAS_FireWall	*)lpAS;

	lpFireWall->m_wX1	=	_iX1;
	lpFireWall->m_wY1	=	_iY1;
	lpFireWall->m_wX2	=	_iX2;
	lpFireWall->m_wY2	=	_iY2;

	int	iResult;

	if	(!lpAS->fire(&iResult))
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dMAX_ACTIVE_SKILL;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	if	(m_iActiveSkillCount	>	m_iMaxSkillCount)
		m_iMaxSkillCount	=	m_iActiveSkillCount;

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	땅을 향해 기술 사용
WORD
CActiveSkillManager::castAtGround(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iRange)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if	(m_iActiveSkillCount	>=	dMAX_ACTIVE_SKILL)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wTeam		=	_lpCaster->m_wTeam;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_posTarget.x	=	_iX;
	lpAS->m_posTarget.y	=	_iY;
	lpAS->m_wRange		=	_iRange;
	lpAS->m_lpTarget	=	NULL;

	int	iResult;

	if	(!lpAS->fire(&iResult))
		return 0xffff;

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dMAX_ACTIVE_SKILL;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	if (m_iActiveSkillCount	>	m_iMaxSkillCount)
		m_iMaxSkillCount	=	m_iActiveSkillCount;

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

int
CActiveSkillManager::findActiveSkill(int _iX,int _iY,int _iSkill,int _iRange)
{
	_iRange	*=	_iRange;

	for (int i=0,iCount=0;i<dMAX_ACTIVE_SKILL && iCount<m_iActiveSkillCount;i++)
	{
		CActiveSkill	*lpAS	=	&m_aActiveSkill[i];

		if	(lpAS->m_wSerial		==	0xffff)
			continue;

		if	(lpAS->m_lpSkill->m_wSerial	==	_iSkill	)
		{
			int	iCurrentRage	=	GetOvalRange(_iX,_iY,lpAS->m_pos.x,lpAS->m_pos.y);

			if	(iCurrentRage	<=	_iRange)
				return	i;
		}

		iCount++;
	}

	return	0xffff;
}

int
CActiveSkillManager::getActiveSkillCount(int _iSkill,int _iCasterZoneSerial)
{
	int	iASCount	=	0;

	for (int i=0,iCount=0;i<dMAX_ACTIVE_SKILL && iCount<m_iActiveSkillCount;i++)
	{
		CActiveSkill	*lpAS	=	&m_aActiveSkill[i];

		if	(lpAS->m_wSerial			==	0xffff)
			continue;

		if	(lpAS->m_lpSkill->m_wSerial	!=	_iSkill	)
			continue;

		if	(lpAS->m_iCasterZoneSerial	==	_iCasterZoneSerial)
			iASCount++;

		iCount++;
	}

	return	iASCount;
}

//
//	땅을 향해 기술 사용
WORD
CActiveSkillManager::castGasTypeSkill(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility,int _iRange)
{
	CSkill		*lpSkill=	_lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dMAX_ACTIVE_SKILL)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,_lpAbility	,sizeof(cAbility));
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wTeam		=	_lpCaster->m_wTeam;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_posTarget.x	=	_iX;
	lpAS->m_posTarget.y	=	_iY;
	lpAS->m_wRange		=	_iRange;
	lpAS->m_lpTarget	=	NULL;

	lpAS->init();
	lpAS->m_wSkillType	=	eSKILL_TYPE_GAS;
	((cAS_Gas*)lpAS)->cast();

	lpAS->m_wSerial		=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dMAX_ACTIVE_SKILL;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	if (m_iActiveSkillCount	>	m_iMaxSkillCount)
		m_iMaxSkillCount	=	m_iActiveSkillCount;

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}


WORD
CActiveSkillManager::castDefendOnImageTrap(cACTOR *_lpCaster,cTrap *_lpTrap)
{
	cAbility	*lpAbility	=	_lpTrap->getAbility();
	CSkill		*lpSkill	=	lpAbility->getSkill();

	if (m_iActiveSkillCount	>=	dMAX_ACTIVE_SKILL)
		return 0xffff;

	CActiveSkill	*lpAS	=	&m_aActiveSkill[m_iRookie];

	memcpy(&lpAS->m_ability,lpAbility	,sizeof(cAbility));
	lpAS->m_lpCaster	=	_lpCaster;
	lpAS->m_lpSkill		=	lpSkill;
	lpAS->m_wTeam		=	_lpCaster->m_wTeam;
	lpAS->m_pos.x		=	_lpCaster->m_pos.x;
	lpAS->m_pos.y		=	_lpCaster->m_pos.y;
	lpAS->m_lpTarget	=	NULL;

	cAS_DefendOnImageTrap	*lpTrap	=	(cAS_DefendOnImageTrap	*)lpAS;

	lpTrap->cast(_lpTrap);
	lpAS->m_wSerial				=	m_iRookie;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.
	lpAS->m_iCasterZoneSerial	=	_lpCaster->m_iZoneSerial;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.
	lpAS->m_wActiveField		=	_lpCaster->m_wCurrentField;	//	이전에 기술을 사용하는데 어떠한 결함(?)이 발견될수도 있다.

	for (int i=m_iRookie + 1;i<dMAX_ACTIVE_SKILL;i++)	//	다음 스킬 메니져 인덱스 설정
		if (m_aActiveSkill[i].m_wSerial	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	m_iActiveSkillCount++;	//	카운터 증가

	if (m_iActiveSkillCount	>	m_iMaxSkillCount)
		m_iMaxSkillCount	=	m_iActiveSkillCount;

	return lpAS->m_wSerial;	//	스킬 인덱스 리턴
}

//
//	캐릭터에게 퀵 캐스트~~ -o-;;
WORD
CActiveSkillManager::quickCastAtActor(cACTOR *_lpCaster,cACTOR *_lpTarget,cAbility *_lpAbility)
{
	return	0xffff;
}

WORD
CActiveSkillManager::quickCastAtGround(cACTOR *_lpCaster,int _iX,int _iY,cAbility *_lpAbility)
{
	return	0xffff;
}
