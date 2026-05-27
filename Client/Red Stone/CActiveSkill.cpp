#include	<math.h>
#include	"CSkill.H"
#include	"cJOB.H"
#include	"CActor.H"
#include	"CImageData.H"
#include	"cSHAKE.H"
#include	"CFieldItem.H"
#include	"CSound.H"
#include	"ast_chain_lightning.h"
#include	"ast_fire_wall.h"
#include	"ast_one_bite.h"

void
CActiveSkill::operateSkillEffect()
{
	if	(m_lpCaster)
		m_lpCaster->operateSkillEffectForCaster(&m_ability);
	if	(m_lpTarget)
		m_lpTarget->operateSkillEffectForTarget(&m_ability);
}

//
//	리셋
void
CActiveSkill::reset()
{
	if	(m_lpCaster && m_lpSkill)
	{
		if	(m_lpCaster->m_wUseSkill	==	m_wSerial)
			m_lpCaster->m_wUseSkill	=	0xffff;

		if	(m_lpSkill->m_wType	==	eSKILL_TYPE_ICY_STALAGMITE)
			if	(isReleaseIcyStalagmite() == FALSE)
				m_lpCaster->m_wHillSkill	=	0xffff;

		if	(m_lpSkill->isHideShield()			)
			m_lpCaster->m_wIsHideShield	=	FALSE;

		if	(m_lpSkill->m_wType == eSKILL_TYPE_SCIMITAR_CUTTING	&&	m_lpCaster->m_wUseSpecialActionSkill	==	eSAS_SCIMATAR_CUTTING)
			m_lpCaster->m_wUseSpecialActionSkill	=	eSAS_NONE;

		if	(m_lpSkill->m_wType == eSKILL_TYPE_BOUNCING_LINEAR	||	m_lpSkill->m_wType == eSKILL_TYPE_SURPRISING_RAID)
			m_lpCaster->stopBouncingLinear();

		if	(m_lpSkill->m_wType == eSKILL_TYPE_BIG_BULLET)
		{
			CActor	*lpBullet	=	((cAS_BigBullet *)this)->m_lpBullet;

			if	(lpBullet)
				lpBullet->stopBouncingLinear();
		}

		if	(m_lpSkill->m_bf4ChangeDirect)
			m_lpCaster->bookingDirect(m_lpSkill->m_bf4ChangeDirect);
	}

	if (m_lpTarget	&&	m_lpSkill->m_wTargetMarkImage	!=	0xffff)	
		m_lpTarget->m_wTargetMark	=	0xffff;

	m_wSerial		=	0xffff;
	m_lpCaster		=	NULL;
	m_lpTarget		=	NULL;
	m_wAnm			=	0;
	m_wDirect		=	0;
	m_wFrame		=	0;
	m_wFrameCounter	=	0xffff;
	m_isGetdCP		=	FALSE;
	m_wAngle		=	1;

	m_sHeight		=	0;
	m_hitInfo.reset();
	int iHitSize = m_vecContinuousHit.size();
	for(int i=0;i<iHitSize;++i)
	{
		m_vecContinuousHit.pop_back();
	}
	memset(m_aValue,0xff,sizeof(m_aValue));
}	//	CActiveSkill::reset()

//
//	발사!!
BOOL
CActiveSkill::fire(BOOL _bIsIndependenceCasterSkill)
{
	if	(m_wType	!= eSKILL_TYPE_SPECIAL_MISSILE && m_lpCaster->isDeath() && _bIsIndependenceCasterSkill == FALSE)
		return	FALSE;	//	죽었어!!

	int	iFireResult	=	eSKILL_FIRE_RESULT_FAILED;

	switch(m_wType)
	{
		case	eSKILL_TYPE_MIRROR_IMAGE				:	iFireResult	=	fireMotionSkill();break;
		case	eSKILL_TYPE_FLORENCE_SPECIAL			:	iFireResult	=	fireMotionSkill();break;
		case	eSKILL_TYPE_DOUBLE_TARGET				:	
			iFireResult	=	fireDoubleTargetTypeSkill();break;
		case	eSKILL_TYPE_BOUNCING_LINEAR				:	iFireResult	=	fireBouncingLinear();break;//바운싱 리니어
		case	eSKILL_TYPE_IMPACT						:	iFireResult	=	fireImpactSkill();break;//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_WHIRLWIND					:	iFireResult	=	fireWhirlwindTypeSkill();break;//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_TORNADO						:	iFireResult	=	fireTornadoTypeSkill();break;//토네이도

		case	eSKILL_TYPE_ONE_BITE					:
			iFireResult	=	((cAS_OneBite *)this)->fire();
			break;
		case	eSKILL_TYPE_FIRE_WALL					:
			iFireResult	=	((cAS_FireWall *)this)->fire();
			break;
		case	eSKILL_TYPE_CHAIN_LIGHTNING				:
			iFireResult	=	((cAS_ChainLightning *)this)->fire();
			break;
		case	eSKILL_TYPE_GIGA_LIGHTNING				:
			iFireResult	=	((cAS_GigaLightning *)this)->fire();
			break;

		case	eSKILL_TYPE_BIG_BULLET					:
			iFireResult	=	((cAS_BigBullet *)this)->fire();
			break;
			
		case	eSKILL_TYPE_ROLLING_LOG					:
			iFireResult	=	((cAS_RollingLog *)this)->fire();
			break;
			
		case	eSKILL_TYPE_LIGHTNING_WINDER			:
			iFireResult	=	((cAS_LightningWinder*)this)->fire();
			break;
		case	eSKILL_TYPE_MULTI_MISSILE			:			// 멀티미사일.
			iFireResult	=	((cAS_MultiMissile*)this)->fire();
			break;	
		case	eSKILL_TYPE_BOUNCING_MISSILE			:			// 바운싱미사일
			iFireResult	=	((cAS_BouncingMissile*)this)->fire();
			break;	
		case	eSKILL_TYPE_FLY_BOUNCING_MISSILE			:			// 바운싱미사일
			iFireResult	=	((cAS_FlyBouncingMissile*)this)->fire();
			break;	
		case	eSKILL_TYPE_WIDE_MISSILE			:			// 범위미사일
			iFireResult	=	((cAS_WideMissile*)this)->fire();
			break;	
		case	eSKILL_TYPE_ARC_MISSILE			:			// 포물선미사일
			iFireResult	=	((cAS_ArcMissile*)this)->fire();
			break;	
			
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_MISSILE						:	iFireResult	=	fireMissileTypeSkill();break;
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:	iFireResult	=	fireExplosionMissile();break;
		case	eSKILL_TYPE_BOOMERANG					:	iFireResult	=	fireBoomerang();break;//부메랑
		case	eSKILL_TYPE_MACHINE_MISSILE				:	iFireResult	=	fireMachineMissile();break;
		case	eSKILL_TYPE_SPECIAL_MISSILE				:	iFireResult	=	fireSpecialMissile();break;
		case	eSKILL_TYPE_SPREAD_ARROW				:	iFireResult	=	fireSpreadArrow();break;

		case	eSKILL_TYPE_PIERCING_MISSILE			:	iFireResult	=	firePiercingMissileTypeSkill();break;
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:	iFireResult	=	fireLinerChainExplosionTypeSkill();break;
		case	eSKILL_TYPE_DROP_ON_HEAD				:	iFireResult	=	fireDropOnHeadTypeSkill();break;
		case	eSKILL_TYPE_LASER						:	iFireResult	=	fireLaserTypeSkill();break;
		case	eSKILL_TYPE_GAS							:	iFireResult	=	fireGasTypeSkill();break;
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:	iFireResult	=	fireExplosionTypeSkillDependOnImage();break;
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:	iFireResult	=	fireContinuousAttackTypeSkill();break;
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:	iFireResult	=	fireContinuousShoot();break;
		case	eSKILL_TYPE_BIT_GLIDER					:	iFireResult	=	fireBitGlider();break;
		case	eSKILL_TYPE_LAND_MARKER					:	iFireResult	=	fireLandMarker();break;
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:	iFireResult	=	fireIntervalShooter();break;
		case	eSKILL_TYPE_WATER_FALL					:	iFireResult	=	fireWaterFall();break;

		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	iFireResult	=	fireBunshine();break;
		case	eSKILL_TYPE_AWESOME_FORTRESS			:
		{
			iFireResult	=	fireAwesomeFortress();
			break;
		}
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:
		{
			iFireResult	=	fireContinuousHitOfDoppelganger();
			break;
		}
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT			:
		{
			iFireResult	=	fireContinuousBit();
			break;
		}
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:	iFireResult	=	fireScimitarCutting();break;
		case	eSKILL_TYPE_MIRROR_TOWER				:	iFireResult	=	fireMirrorTower();break;

		case	eSKILL_TYPE_RUSH						:	iFireResult	=	fireRushTypeSkill();break;

		case	eSKILL_TYPE_CHARGE						:	iFireResult	=	fireChargeTypeSkill();break;
		case	eSKILL_TYPE_FIRE_BALL					:	iFireResult	=	fireFireBallTypeSkill();break;
		case	eSKILL_TYPE_ICY_STALAGMITE				:	iFireResult	=	fireIcyStalagmite();break;

		case	eSKILL_TYPE_BITE_HANGING				:	iFireResult	=	fireBiteHanging();break;

		case	eSKILL_TYPE_PET_COMMAND					:
		case	eSKILL_TYPE_SHIMMERING_SHIELD			:
		case	eSKILL_TYPE_HEAL						:
		case	eSKILL_TYPE_AID_ATTACK					:
		case	eSKILL_TYPE_AID_MAGIC					:	iFireResult	=	fireAidSkill();break;

		case	eSKILL_TYPE_GUARDIAN_POST				:
		case	eSKILL_TYPE_MACHINE_GUN					:	iFireResult	=	fireMotionSkill();break;
		case	eSKILL_TYPE_TELEPORT					:	iFireResult	=	fireTeleport();break;
		case	eSKILL_TYPE_ALTERING_HITTER				:	iFireResult	=	fireAlteringHitter();break;
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP						:	iFireResult	=	fireJump();break;
		case	eSKILL_TYPE_JUMP_ATTACK					:	iFireResult	=	fireJumpAttack();break;
		case	eSKILL_TYPE_RADIAL_ARC					:	iFireResult	=	fireRadialArc();break;
		case	eSKILL_TYPE_WHIRLTHRUSTING				:	iFireResult	=	fireWhirlthrusting();break;
		case	eSKILL_TYPE_SURPRISING_RAID				:	iFireResult	=	fireSurprisingRaid();break;
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		:	iFireResult	=	fireContinuousWhirlwindSkill();break;		// 지속형라운딩
	}

	if (iFireResult	!=	eSKILL_FIRE_RESULT_OK)
	{
		m_wSerial	=	0xffff;

		return FALSE;	//	스킬 사용이 실패했다.!!!!!!??
	}

	if (_bIsIndependenceCasterSkill)
		return	TRUE;

	if (m_wType	!=	eSKILL_TYPE_MACHINE_MISSILE && m_wType	!= eSKILL_TYPE_SPECIAL_MISSILE)
	{
		operateSkillEffect();

		int	iRange	=	0;
		
		if	(m_lpCaster && g_lpHero)
			iRange	=	GetRange(g_lpHero->m_pos.x,g_lpHero->m_pos.y,m_lpCaster->m_pos.x,m_lpCaster->m_pos.y);

		if	(iRange	<=	g_iScreenWidth*g_iScreenWidth)
			g_esm.playCastingSound(m_lpSkill);

		m_lpCaster->setExclusiveAction(m_lpSkill->m_bf1IsExclusiveAction);
	}

	if	(m_lpSkill->m_wSpentHPPercentage)
	{
		int	iSpentHP		=	m_lpCaster->m_iMaxHP*m_lpSkill->m_wSpentHPPercentage/10000;

		m_lpCaster->reduceHP(iSpentHP);
		m_lpCaster->m_iHP	=	max(m_lpCaster->m_iHP,100);
		
		if	(m_lpCaster->isHero())
			g_hero.m_iHP	=	m_lpCaster->m_iHP;
	}

	if	(m_lpSkill->m_wSpentHPPercentageBasedBloodWing && m_hitInfo.isHit())
	{
		int	iSpentHP		=	m_hitInfo.m_dwMagicDamage*m_lpSkill->m_wSpentHPPercentageBasedBloodWing/10000;

		m_lpCaster->reduceHP(iSpentHP);
		m_lpCaster->m_iHP	=	max(m_lpCaster->m_iHP,100);

		if	(m_lpCaster->isHero())
			g_hero.m_iHP	=	m_lpCaster->m_iHP;
	}

	return	TRUE;
}

//
//	업데이트
BOOL
CActiveSkill::update()
{
	switch(m_wType)
	{
		case	eSKILL_TYPE_BIG_BULLET					:
			return	((cAS_BigBullet *)this)->update();

		case	eSKILL_TYPE_ROLLING_LOG					:
			return	((cAS_RollingLog *)this)->update();

		case	eSKILL_TYPE_LIGHTNING_WINDER			:
			return	((cAS_LightningWinder*)this)->update();

		case	eSKILL_TYPE_MULTI_MISSILE			:	// 멀티미사일.
			return	((cAS_MultiMissile*)this)->update();

		case	eSKILL_TYPE_BOUNCING_MISSILE			:			// 바운싱미사일
			return	((cAS_BouncingMissile*)this)->update();

		case	eSKILL_TYPE_FLY_BOUNCING_MISSILE			:			// 바운싱미사일
			return	((cAS_FlyBouncingMissile*)this)->update();

		case	eSKILL_TYPE_WIDE_MISSILE			:			// 범위
			return	((cAS_WideMissile*)this)->update();

		case	eSKILL_TYPE_ARC_MISSILE			:			// 포물선
			return	((cAS_ArcMissile*)this)->update();


		case	eSKILL_TYPE_GIGA_LIGHTNING				:
			return	((cAS_GigaLightning*)this)->update();

		case	eSKILL_TYPE_CHAIN_LIGHTNING				:
			return	((cAS_ChainLightning *)this)->update();

		case	eSKILL_TYPE_ONE_BITE					:
			return	((cAS_OneBite *)this)->update();

		case	eSKILL_TYPE_FIRE_WALL					:
			return	((cAS_FireWall *)this)->update();

		case	eSKILL_TYPE_DOUBLE_TARGET				:	return	updateDoubleTargetTypeSkill();
		case	eSKILL_TYPE_BOUNCING_LINEAR				:	return	updateBouncingLinear();//바운싱 리니어
		case	eSKILL_TYPE_IMPACT						:	return	updateImpactSkill();//	때리고 주위에 충격파가 생긴다.
		case	eSKILL_TYPE_WHIRLWIND					:	return	updateWhirlwindTypeSkill();//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_TORNADO						:	return	updateTornadoTypeSkill();//토네이도
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_MISSILE						:	return	updateMissileTypeSkill();//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:	return	updateExplosionMissile();
		case	eSKILL_TYPE_BOOMERANG					:	return	updateBoomerang();//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_PIERCING_MISSILE			:	return	updatePiercingMissileTypeSkill();//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_MACHINE_MISSILE				:	return	updateMachineMissile();
		case	eSKILL_TYPE_SPECIAL_MISSILE				:	return	updateSpecialMissile();
		case	eSKILL_TYPE_SPREAD_ARROW				:	return	updateSpreadArrow();

		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:	return	updateLinerChainExplosionTypeSkill();
		case	eSKILL_TYPE_DROP_ON_HEAD				:	return	updateDropOnHeadTypeSkill();
		case	eSKILL_TYPE_LASER						:	return	updateLaserTypeSkill();
		case	eSKILL_TYPE_GAS							:	return	updateGasTypeSkill();
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:	return	updateExplosionTypeSkillDependOnImage();
		case	eSKILL_TYPE_FIRE_BALL					:	return	updateFireBallTypeSkill();
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:	return	updateContinuousAttackTypeSkill();
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:	return	updateContinuousShoot();
		case	eSKILL_TYPE_BIT_GLIDER					:	return	updateBitGlider();
		case	eSKILL_TYPE_LAND_MARKER					:	return	updateLandMarker();
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:	return	updateIntervalShooter();
		case	eSKILL_TYPE_WATER_FALL					:	return	updateWaterFall();

		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	return	updateBunshine();
		case	eSKILL_TYPE_AWESOME_FORTRESS			:	return	updateAwesomeFortress();
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER		:	return	updateContinuousHitOfDoppelganger();
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT		:	return	updateContinuousBit();

		case	eSKILL_TYPE_SCIMITAR_CUTTING			:	return	updateScimitarCutting();
		case	eSKILL_TYPE_MIRROR_TOWER				:	return	updateMirrorTower();

		case	eSKILL_TYPE_TAIL_CHASER					:	return	updateTailChaserMissile();
		case	eSKILL_TYPE_ICY_STALAGMITE				:	return	updateIcyStalagmite();

		case	eSKILL_TYPE_RUSH						:	return	updateRushTypeSkill();
		case	eSKILL_TYPE_TELEPORT					:	return	updateTeleport();
		case	eSKILL_TYPE_ALTERING_HITTER				:	return	updateAlteringHitter();
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP						:	return	updateJump();
		case	eSKILL_TYPE_JUMP_ATTACK					:	return	updateJumpAttack();
		case	eSKILL_TYPE_RADIAL_ARC					:	return	updateRadialArc();
		case	eSKILL_TYPE_WHIRLTHRUSTING				:	return	updateWhirlthrusting();
		case	eSKILL_TYPE_SURPRISING_RAID				:	return	updateSurprisingRaid();

		case	eSKILL_TYPE_AID_ATTACK					:	return	updateAidAttackSkill();

		case	eSKILL_TYPE_PET_COMMAND					:
		case	eSKILL_TYPE_HEAL						:
		case	eSKILL_TYPE_AID_MAGIC					:	return	updateAidSkill();

		case	eSKILL_TYPE_BITE_HANGING				:	return	updateBiteHanging();

		case	eSKILL_TYPE_SHIMMERING_SHIELD			:
		case	eSKILL_TYPE_GUARDIAN_POST				:
		case	eSKILL_TYPE_FLORENCE_SPECIAL			:
		case	eSKILL_TYPE_MACHINE_GUN					:	return	TRUE;
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND				:	return	updateContinuousWhirlwindSkill();
	}

	return	TRUE;
}	//	CActiveSkill::update()

//
//	이미지 출력
void
CActiveSkill::put()
{
	switch(m_wType)
	{
		case	eSKILL_TYPE_BIG_BULLET					:
			((cAS_BigBullet *)this)->put();
			break;

		case	eSKILL_TYPE_ROLLING_LOG					:
			((cAS_RollingLog *)this)->put();
			break;

		case	eSKILL_TYPE_GIGA_LIGHTNING				:
			((cAS_GigaLightning*)this)->put();
			break;

		case	eSKILL_TYPE_CHAIN_LIGHTNING				:
			((cAS_ChainLightning *)this)->put();
			break;
		case	eSKILL_TYPE_FLY_BOUNCING_MISSILE			:
			((cAS_FlyBouncingMissile *)this)->put();
			break;
		case	eSKILL_TYPE_ARC_MISSILE			:
			((cAS_ArcMissile *)this)->put();
			break;
		case	eSKILL_TYPE_BOUNCING_LINEAR				:	putBouncingLinear();break;//바운싱 리니어
		case	eSKILL_TYPE_IMPACT						:	putImpactSkill();break;//	때리고 주위에 충격파가 생긴다.
		case	eSKILL_TYPE_WHIRLWIND					:	putWhirlwindTypeSkill();break;//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_TORNADO						:	putTornadoTypeSkill();break;//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_LASER						:	putLaserTypeSkill();break;
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_PIERCING_MISSILE			:	putMissileTypeSkill();break;//미사일
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:	putExplosionMissile();break;
		case	eSKILL_TYPE_MACHINE_MISSILE				:	putMachineMissile();break;
		case	eSKILL_TYPE_SPECIAL_MISSILE				:	putSpecialMissile();break;
		case	eSKILL_TYPE_SPREAD_ARROW				:	putSpreadArrow();break;

		case	eSKILL_TYPE_BOOMERANG					:	putBoomerang();break;//부메랑
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:	putLinerChainExplosionTypeSkill();break;
		case	eSKILL_TYPE_DROP_ON_HEAD				:	putDropOnHeadTypeSkill();break;
		case	eSKILL_TYPE_GAS							:
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND				:			// 지속형라운딩..추가..
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:	putExplosionTypeSkillDependOnImage();break;
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:	putContinuousAttackTypeSkill();break;
		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	putBunshine();break;
		case	eSKILL_TYPE_AWESOME_FORTRESS			:	putAwesomeFortress();break;
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:	putScimitarCutting();break;

		case	eSKILL_TYPE_TAIL_CHASER					:	putTailChaserMissile();break;
		case	eSKILL_TYPE_FIRE_BALL					:	putFireBallTypeSkill();break;
		case	eSKILL_TYPE_RUSH						:	putRushTypeSkill();break;
		case	eSKILL_TYPE_TELEPORT					:	putTeleport();break;
		case	eSKILL_TYPE_ALTERING_HITTER				:	putAlteringHitter();break;
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP						:	putJump();break;
		case	eSKILL_TYPE_JUMP_ATTACK					:	putJumpAttack();break;
		case	eSKILL_TYPE_RADIAL_ARC					:	putRadialArc();break;
		case	eSKILL_TYPE_WHIRLTHRUSTING				:	putWhirlthrusting();break;
		case	eSKILL_TYPE_SURPRISING_RAID				:	putSurprisingRaid();break;
	}
}	//	CActiveSkill::put()

//
//	이미지 출력
void
CActiveSkill::putBottomImage()
{
	switch(m_wType)
	{
		case	eSKILL_TYPE_GAS							:
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:
			putBottomExplosionTypeSkillDependOnImage();
			break;

		case	eSKILL_TYPE_GIGA_LIGHTNING				:
			((cAS_GigaLightning*)this)->putBottom();
			break;
//		case	eSKILL_TYPE_IMPACT						:	break;//	때리고 주위에 충격파가 생긴다.
/*		case	eSKILL_TYPE_WHIRLWIND					:	putWhirlwindTypeSkill();break;//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_TORNADO						:	putTornadoTypeSkill();break;//회오리형(자기를 중심으로 일정 영역을 공격한다.)
		case	eSKILL_TYPE_LASER						:	putLaserTypeSkill();break;
		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_PIERCING_MISSILE			:	putMissileTypeSkill();break;//미사일
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:	putExplosionMissile();break;

		case	eSKILL_TYPE_BOOMERANG					:	putBoomerang();break;//부메랑
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:	putLinerChainExplosionTypeSkill();break;
		case	eSKILL_TYPE_DROP_ON_HEAD				:	putDropOnHeadTypeSkill();break;
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:	putContinuousAttackTypeSkill();break;
		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:	putBunshine();break;
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:	putScimitarCutting();break;
		case	eSKILL_TYPE_MACHINE_MISSILE				:	putMachineMissile();break;
		case	eSKILL_TYPE_SPECIAL_MISSILE				:	putSpecialMissile();break;

		case	eSKILL_TYPE_TAIL_CHASER					:	putTailChaserMissile();break;
		case	eSKILL_TYPE_FIRE_BALL					:	putFireBallTypeSkill();break;
		case	eSKILL_TYPE_RUSH						:	putRushTypeSkill();break;
		case	eSKILL_TYPE_TELEPORT					:	putTeleport();break;*/
	}
}	//	CActiveSkill::putBottomImage()

//
//	캐스가 정지했다.
BOOL
CActiveSkill::casterStop()
{
	switch(m_wType)
	{
		case	eSKILL_TYPE_WHIRLWIND					:
		case	eSKILL_TYPE_IMPACT						:
			break;

		case	eSKILL_TYPE_TAIL_CHASER					:
		case	eSKILL_TYPE_MACHINE_MISSILE				:
		case	eSKILL_TYPE_SPECIAL_MISSILE				:
		case	eSKILL_TYPE_AID_ATTACK					:
		case	eSKILL_TYPE_AID_MAGIC					:
		case	eSKILL_TYPE_JUMP						:
		case	eSKILL_TYPE_JUMP_ATTACK					:
		case	eSKILL_TYPE_ALTERING_HITTER				:
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLTHRUSTING				:
		case	eSKILL_TYPE_BOUNCING_LINEAR				:
		case	eSKILL_TYPE_DOUBLE_TARGET				:
		case	eSKILL_TYPE_BIG_BULLET					:
		case	eSKILL_TYPE_ROLLING_LOG					:
		case	eSKILL_TYPE_GIGA_LIGHTNING				:
		case	eSKILL_TYPE_CHAIN_LIGHTNING				:
		case	eSKILL_TYPE_FIRE_WALL					:
		case	eSKILL_TYPE_ONE_BITE					:
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND				:		//  지속형라운딩..추가..
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT		:
		case	eSKILL_TYPE_FLY_BOUNCING_MISSILE			:			// 바운싱미사일
		case	eSKILL_TYPE_ARC_MISSILE			:			// 포물선미사일
			return	FALSE;

		case	eSKILL_TYPE_LASER						:
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:
		case	eSKILL_TYPE_PIERCING_MISSILE			:
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
		case	eSKILL_TYPE_DROP_ON_HEAD				:
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:
		case	eSKILL_TYPE_GAS							:
		case	eSKILL_TYPE_BOOMERANG					:
		case	eSKILL_TYPE_TORNADO						:
		case	eSKILL_TYPE_TELEPORT					:
		case	eSKILL_TYPE_SPREAD_ARROW				:
			if	(m_wFrameCounter	!=	0xffff	)
				return	FALSE;
			return	TRUE;

		case	eSKILL_TYPE_FIRE_BALL					:
		{
			if	(getFireBallStatus()	!=	eFBTSS_FIRE)
				return	TRUE;

			return	FALSE;
		}

		case	eSKILL_TYPE_WATER_FALL					:
		{
			if	(m_wFrameCounter	!=	0xffff)
				return	FALSE;

			return	TRUE;
		}

		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:
		case	eSKILL_TYPE_BIT_GLIDER					:
		case	eSKILL_TYPE_LAND_MARKER					:
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:
		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:
		case	eSKILL_TYPE_SURPRISING_RAID				:
		case	eSKILL_TYPE_RUSH						:	
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:
		case	eSKILL_TYPE_MIRROR_TOWER				:
		case	eSKILL_TYPE_BITE_HANGING				:
		case	eSKILL_TYPE_LIGHTNING_WINDER			:
		case	eSKILL_TYPE_MULTI_MISSILE			:	// 멀티미사일.
		case	eSKILL_TYPE_BOUNCING_MISSILE			:			// 바운싱미사일
		case	eSKILL_TYPE_WIDE_MISSILE				:			// 범위
			return	TRUE;

		case	eSKILL_TYPE_AWESOME_FORTRESS			:
			if (isTriggerAwesomeFortress())
				return	TRUE;
			return	FALSE;
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:
			if (isTriggerContinuousHitOfDoppelganger())
				return	TRUE;
			return	FALSE;

	}

	return	TRUE;
}	//	CActiveSkill::casterStop()

//
//	캐스가 죽었다. -_-a
BOOL
CActiveSkill::casterDeath()
{
	switch(m_wType)
	{
		case	eSKILL_TYPE_WHIRLWIND					:
		case	eSKILL_TYPE_IMPACT						:
		case	eSKILL_TYPE_FIRE_BALL					:
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:
		case	eSKILL_TYPE_BIT_GLIDER					:
		case	eSKILL_TYPE_LAND_MARKER					:
		case	eSKILL_TYPE_INTERVAL_SHOOTER			:
		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:
		case	eSKILL_TYPE_AWESOME_FORTRESS			:
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:
		case	eSKILL_TYPE_RUSH						:	
		case	eSKILL_TYPE_AID_ATTACK					:
		case	eSKILL_TYPE_AID_MAGIC					:
		case	eSKILL_TYPE_MIRROR_TOWER				:
		case	eSKILL_TYPE_BITE_HANGING				:
		case	eSKILL_TYPE_BOUNCING_LINEAR				:
		case	eSKILL_TYPE_DOUBLE_TARGET				:
		case	eSKILL_TYPE_BIG_BULLET					:
		case	eSKILL_TYPE_ROLLING_LOG					:
		case	eSKILL_TYPE_LIGHTNING_WINDER			:
		case	eSKILL_TYPE_MULTI_MISSILE			:	// 멀티미사일.
		case	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT				:	
		case	eSKILL_TYPE_BOUNCING_MISSILE			:			// 바운싱미사일
		case	eSKILL_TYPE_WIDE_MISSILE				:			// 범위
			return	TRUE;

		case	eSKILL_TYPE_WATER_FALL					:
		{
			if (m_wFrameCounter	!=	0xffff)	return	FALSE;

			return	TRUE;
		}

		case	eSKILL_TYPE_MACHINE_MISSILE				:
			if (getMachineSkillUpdateStatus()	==	0)
			{
				if (m_lpCaster)
				{
					m_lpCaster->m_wMachineAngle	=	0xffff;
					m_lpCaster->m_wMachineFrame	=	0;
				}
				return	TRUE;
			}
			return	FALSE;

		case	eSKILL_TYPE_SPECIAL_MISSILE				:
		case	eSKILL_TYPE_BOOMERANG					:
		case	eSKILL_TYPE_TORNADO						:
		case	eSKILL_TYPE_SCIMITAR_CUTTING			:
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:
		case	eSKILL_TYPE_TELEPORT					:
		case	eSKILL_TYPE_ALTERING_HITTER				:
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP						:
		case	eSKILL_TYPE_JUMP_ATTACK					:
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLTHRUSTING				:
		case	eSKILL_TYPE_SURPRISING_RAID				:
			return	TRUE;

		case	eSKILL_TYPE_LASER						:
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_PIERCING_MISSILE			:
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
		case	eSKILL_TYPE_DROP_ON_HEAD				:
		case	eSKILL_TYPE_GAS							:
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		:		//  지속형라운딩..추가..
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:
		case	eSKILL_TYPE_GIGA_LIGHTNING				:
		case	eSKILL_TYPE_CHAIN_LIGHTNING				:
		case	eSKILL_TYPE_TAIL_CHASER					:
		case	eSKILL_TYPE_SPREAD_ARROW				:
		case	eSKILL_TYPE_FIRE_WALL					:
		case	eSKILL_TYPE_ONE_BITE					:
		case	eSKILL_TYPE_FLY_BOUNCING_MISSILE		:
		case	eSKILL_TYPE_ARC_MISSILE		:

			if (m_wFrameCounter	!=	0xffff	)
			{
				m_lpCaster	=	NULL;	//	동작은 마저 하지만, 대미지 처리만 무시된다.
				return	FALSE;
			}
			return	TRUE;
	}

	return	TRUE;
}	//	CActiveSkill::casterDeath()

//
//	타겟이 죽었다.
BOOL
CActiveSkill::targetDeath()
{
//	유도탄처럼 매 프레임 타겟의 위치에 영향을 받는건 예외 처리함
	switch(m_wType)
	{
		case	eSKILL_TYPE_WHIRLWIND					:
		case	eSKILL_TYPE_GAS							:
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		:		//  지속형라운딩..추가..		
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:
		case	eSKILL_TYPE_TORNADO						:
		case	eSKILL_TYPE_RUSH						:
		case	eSKILL_TYPE_TELEPORT					:
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP						:
		case	eSKILL_TYPE_SPREAD_ARROW				:
		case	eSKILL_TYPE_WATER_FALL					:
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLTHRUSTING				:
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
			m_lpTarget	=	NULL;
			return	FALSE;

		case	eSKILL_TYPE_INTERVAL_SHOOTER			:
		case	eSKILL_TYPE_BIT_GLIDER					:
		case	eSKILL_TYPE_ALTERING_HITTER				:
		case	eSKILL_TYPE_JUMP_ATTACK					:
		case	eSKILL_TYPE_SURPRISING_RAID				:
		case	eSKILL_TYPE_ATTACK_BY_BUNSHIN			:
		case	eSKILL_TYPE_AWESOME_FORTRESS			:
		case	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER			:
		case	eSKILL_TYPE_BOUNCING_LINEAR				:
		case	eSKILL_TYPE_DOUBLE_TARGET				:
		case	eSKILL_TYPE_BIG_BULLET					:
		case	eSKILL_TYPE_ROLLING_LOG					:
		case	eSKILL_TYPE_LIGHTNING_WINDER			:
		case	eSKILL_TYPE_MULTI_MISSILE			:	// 멀티미사일.
		case	eSKILL_TYPE_GIGA_LIGHTNING				:
		case	eSKILL_TYPE_CHAIN_LIGHTNING				:
		case	eSKILL_TYPE_FIRE_WALL					:
		case	eSKILL_TYPE_ONE_BITE					:
		case	eSKILL_TYPE_SPECIAL_MISSILE				:	//	이미 날아가는중.. -o-
		case	eSKILL_TYPE_BOUNCING_MISSILE			:			// 바운싱미사일
		case	eSKILL_TYPE_FLY_BOUNCING_MISSILE		:			// 바운싱미사일
		case	eSKILL_TYPE_WIDE_MISSILE				:
		case	eSKILL_TYPE_ARC_MISSILE					:	
			return	FALSE;

		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:
		case	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		:
		case	eSKILL_TYPE_LAND_MARKER					:
		case	eSKILL_TYPE_AID_ATTACK					:
		case	eSKILL_TYPE_AID_MAGIC					:
		case	eSKILL_TYPE_MIRROR_TOWER				:
		case	eSKILL_TYPE_IMPACT						:
		case	eSKILL_TYPE_BITE_HANGING				:
			return	TRUE;

		case	eSKILL_TYPE_SCIMITAR_CUTTING			:
			m_lpTarget	=	NULL;
			endScimitarCutting();
			return	FALSE;

		case	eSKILL_TYPE_BOOMERANG					:
			if (getBoomerangStatus()==eBS_FLY_TO_TARGET)	setBoomerangStatus(eBS_DECREASE_SPEED);
			return	FALSE;

		case	eSKILL_TYPE_FIRE_BALL					:
			if (getFireBallStatus()	!=	eFBTSS_FIRE)
			{
				m_lpCaster->stop();
				return	TRUE;
			}
			return	FALSE;

		case	eSKILL_TYPE_MACHINE_MISSILE				:
			if (getMachineSkillUpdateStatus()	==	0)
			{
				if (m_lpCaster)
				{
					m_lpCaster->m_wMachineFrame	=	0;
					m_lpCaster->m_wMachineAngle	=	0xffff;
				}

				return	TRUE;
			}
			return	FALSE;

		case	eSKILL_TYPE_LASER						:
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_PIERCING_MISSILE			:
		case	eSKILL_TYPE_DROP_ON_HEAD				:
		case	eSKILL_TYPE_TAIL_CHASER					:
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:
			if	(m_wFrameCounter	!=	0xffff	)
				return	FALSE;
			return	TRUE;
	}

	return	TRUE;
}	//	CActiveSkill::targetDeath()

//
//	타겟이 제거 되었다.
BOOL
CActiveSkill::targetRemove()
{
	switch(m_wType)
	{
		case	eSKILL_TYPE_WHIRLWIND					:
		case	eSKILL_TYPE_GAS							:
		case	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		:		//  지속형라운딩..추가..
		case	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	:
		case	eSKILL_TYPE_GIGA_LIGHTNING				:
		case	eSKILL_TYPE_CHAIN_LIGHTNING				:
		case	eSKILL_TYPE_FIRE_WALL					:
		case	eSKILL_TYPE_TORNADO						:
		case	eSKILL_TYPE_RUSH						:	
		case	eSKILL_TYPE_TELEPORT					:
		case	eSKILL_TYPE_ALTERING_HITTER				:
		case	eSKILL_TYPE_WILD_STAMP					:
		case	eSKILL_TYPE_JUMP						:
		case	eSKILL_TYPE_JUMP_ATTACK					:
		case	eSKILL_TYPE_SPREAD_ARROW				:
		case	eSKILL_TYPE_RADIAL_ARC					:
		case	eSKILL_TYPE_WHIRLTHRUSTING				:
		case	eSKILL_TYPE_SURPRISING_RAID				:
		case	eSKILL_TYPE_BOUNCING_LINEAR				:
		case	eSKILL_TYPE_DOUBLE_TARGET				:
		case	eSKILL_TYPE_BIG_BULLET					:
		case	eSKILL_TYPE_ROLLING_LOG					:
		case	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		:
		case	eSKILL_TYPE_LIGHTNING_WINDER			:
		case	eSKILL_TYPE_MULTI_MISSILE			:	// 멀티미사일.
		case	eSKILL_TYPE_ONE_BITE					:
		case	eSKILL_TYPE_BOUNCING_MISSILE					:
		case	eSKILL_TYPE_WIDE_MISSILE					:

			m_lpTarget	=	NULL;
			return	FALSE;

		case	eSKILL_TYPE_MACHINE_MISSILE				:
			if (getMachineSkillUpdateStatus()	==	0)
			{
				if (m_lpCaster)
				{
					m_lpCaster->m_wMachineFrame	=	0;
					m_lpCaster->m_wMachineAngle	=	0xffff;
				}
				return	TRUE;
			}
			return	FALSE;

		case	eSKILL_TYPE_SPECIAL_MISSILE				:
		case	eSKILL_TYPE_AID_ATTACK					:
		case	eSKILL_TYPE_AID_MAGIC					:
		case	eSKILL_TYPE_MIRROR_TOWER				:
		case	eSKILL_TYPE_BIT_GLIDER					:
		case	eSKILL_TYPE_IMPACT						:
		case	eSKILL_TYPE_BITE_HANGING				:
		case	eSKILL_TYPE_CONTINUOUS_SHOOT			:
			return	TRUE;

		case	eSKILL_TYPE_BOOMERANG					:
			if (getBoomerangStatus()==eBS_FLY_TO_TARGET)	setBoomerangStatus(eBS_DECREASE_SPEED);
			return	TRUE;

		case	eSKILL_TYPE_FIRE_BALL					:
			m_lpCaster->stop();
			return	TRUE;

		case	eSKILL_TYPE_MISSILE						:
		case	eSKILL_TYPE_THROW_ITEM					:
		case	eSKILL_TYPE_LASER						:
		case	eSKILL_TYPE_PIERCING_MISSILE			:
		case	eSKILL_TYPE_DROP_ON_HEAD				:
		case	eSKILL_TYPE_TAIL_CHASER					:
		case	eSKILL_TYPE_EXPLOSION_MISSILE			:
		case	eSKILL_TYPE_FLY_BOUNCING_MISSILE		:
		case	eSKILL_TYPE_ARC_MISSILE					:
			if (m_wFrameCounter	!=	0xffff	)	
			{
				m_lpTarget	=	NULL;
				return	FALSE;
			}
			return	TRUE;
	}

	return	TRUE;
}