#include <MATH.H>
#include "CActor.H"
#include "cMAP.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "CFieldItem.H"
#include "cSHAKE.H"
#include "cMESSAGE.H"
#include "CSound.H"
#include "arca.H"
#include "BattleLog.h"

#define dGGG_HIT_EFFECT_FIRE		18

//
//	캐릭터 몸에 나타나는 히트 효과
int
CActor::getFreeHitIndex()
{
	int	i;

	for (i=0;i<dHIT_EFFECT_COUNT_ON_CHARACTER;i++)
		if (m_aHitEffect[i] == 0xffff)
			return i;

	int	iMinRemainFrame	=	1000;
	int	iHitEffect		=	0xffff;

	for (i=0;i<dHIT_EFFECT_COUNT_ON_CHARACTER;i++)
		if (m_aHitEffect[i] != 0xffff)
		{
			int	iRemainFrame	=	g_hem.getRemainFrame(m_aHitEffect[i]);

			if (iRemainFrame	<	iMinRemainFrame)
			{	iHitEffect		=	i;
				iMinRemainFrame	=	iRemainFrame;
			}
		}

	g_hem.remove(m_aHitEffect[iHitEffect]);

	return	iHitEffect;
}

//
//	아 띠바!!! 맞았다!! 아니... 아직 맞진 않았고... 언넘이 때리려고 한다... ;;;
void
CActor::hitDummy(CActor	*lpAttacker,cAbility *lpAbility,int _iDirect)
{
	CSkill	*lpSkill	=	lpAbility->getSkill();

	if(m_iAnm	!=	dACT_HIT)
		stop();

	setAnm(dACT_HIT);				//	맞았을때 에니메이션

	setExclusiveAction(TRUE);

	if	(lpSkill->m_wHitImage	!=	0xffff)
	{
		int	iHitIndex	=	getFreeHitIndex();

		int	x	=	getBody()->m_rectCrash.x1	+	random(getBody()->m_rectCrash.getWidth());
		int	y	=	getBody()->m_rectCrash.y1	+	random(getBody()->m_rectCrash.getHeight());

		m_aHitEffect[iHitIndex]	=	g_hem.addEffect(x,y,lpSkill->m_wHitImage,_iDirect);	//	타격효과 추가
	}

	//	추가 타격 이미지
	if(lpSkill->m_wAddHitImage!=	0xffff)
	{
		int	iHitIndex			=	getFreeHitIndex();
		m_aHitEffect[iHitIndex]	=	g_hem.addEffect(0,-getBody()->m_sprite.m_iHeight,lpSkill->m_wAddHitImage,_iDirect);//	타격효과 추가
	}
}

//
//	가시에 찔렸다.
void
CActor::hitThornDamage(CActor *_lpAttacker,int _iDamage,int _iRemainCP , BOOL _isDisplayMagicDamage)
{
	int	x			=	getBody()->m_rectCrash.x1	+	random(getBody()->m_rectCrash.getWidth());
	int	y			=	getBody()->m_rectCrash.y1	+	random(getBody()->m_rectCrash.getHeight());
	int	iHitEffect	=	getFreeHitIndex();

	_lpAttacker->addHitEffect(0,0,g_im.m_wDamageReturnEffect,0);

	reduceHP(_iDamage,FALSE);

	_iDamage	/=	100;
	_iDamage	=	max(_iDamage,1);

	if (_isDisplayMagicDamage)
		m_aHitEffect[iHitEffect]=	g_hem.addEffectAndInfo(x,y,g_im.m_wNormalHit,dHIT_INFO_MAGIC_NUMBER,_iDamage,0,0,isOwnTeam());//	타격효과 추가
	else
		m_aHitEffect[iHitEffect]=	g_hem.addEffectAndInfo(x,y,g_im.m_wNormalHit,dHIT_INFO_WHITE_NUMBER,_iDamage,0,0,isOwnTeam());//	타격효과 추가
	
}

void
CActor::hitChainedDamage(int _iSkill,int _iShotCount,int _iMissCount,int _iGap,BOOL _bIsAttackToShakle,
						int _iPhysicalDamage,int _iMagicDamage,int _iMinPhysicalDamage,int _iMinMagicDamage,
						int _iCriticalCount,int _iDoubleCriticalCount,int _iInstanceKillCount,
						int _iBlockingCount,int _iNoActionBlockCount,int _iDancingBlockCount,int _iCaster, int _iHardBlowCount)
{
	cChanedDamageInfo*	lpInfo	=	m_chainedDamage.add(_iSkill,_iShotCount,_iGap,_iCaster);

	if	(!lpInfo)
		return;

	setContinuousAttackDamage(	_iPhysicalDamage,_iMagicDamage,_iShotCount,_iMissCount,_iCriticalCount,_iDoubleCriticalCount,_bIsAttackToShakle,
								_iMinPhysicalDamage,_iMinMagicDamage,_iBlockingCount,_iDancingBlockCount,_iNoActionBlockCount,lpInfo->m_aInfo, _iHardBlowCount);

	updateChainedDamage(lpInfo);
}

void
CActor::hitMultipleDamage(CHitInfo *_lpHitInfo,int _iCount,int _iSkill,BOOL _bIsDamageSum)
{	// 마지막 수정일 : 09.09.29
	cAbility	ability;

	ability.set(_iSkill,1);

	if(_iSkill == dSKILL_SERIAL_PLOT_OF_SHADOW)	
		_lpHitInfo->m_dwMagicDamage /= _iCount;

	if	(_bIsDamageSum	==	FALSE)
		for	(int i=0;i<_iCount;i++)
			hit(NULL,&ability,_lpHitInfo);

}


//
//	히트 판정. 맞았다고 맞는게 아니다. ? 스킬에 따라 여러가지 작용이 있다.
void
CActor::hit(CActor *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iDirect,BOOL _bIsPlaySound,BOOL _bIsMirrorTower)		
{ // 마지막 수정일 09.09.23
//	if	(_lpAbility->m_wSkill	<	600)
//		_lpAbility->m_wSkill	=	_lpAbility->m_wSkill;
	if	((_lpAttacker && _lpAttacker->isHero()) || isHero())
		g_hero.m_wNonBattleTime	=	0;

	if	(_lpAttacker && _bIsMirrorTower == FALSE)
		if	(_lpAttacker->isHero() || g_hero.isPet(_lpAttacker->m_wSerial) || g_hero.isSummonBeastActor(_lpAttacker->m_wSerial))
			g_hero.setRestraintTimeByAttackAction();	//	공격할때

	BOOL	bIsAttackToShakle	=	_lpHitInfo->isAttackToShakle();
	BOOL	bIsHideDamageNumber	=	FALSE;

	if	(m_bf1IsBoss)
	{
		if	(_lpAttacker	==	NULL	||	_lpAttacker->isHero()	==	FALSE)
			bIsHideDamageNumber	=	TRUE;
	}

//	if	(m_wShakleSkill	!=	0xffff	&&	_lpAttacker	&&	m_wSerial	==	_lpAttacker->m_wSerial)
//		bIsAttackToShakle	=	TRUE;

	CSkill	*lpSkill	=	_lpAbility->getSkill();
	CPos	posHit,posAttacker;

	if	(_lpAttacker)
	{
		posAttacker.x	=	_lpAttacker->m_pos.x;
		posAttacker.y	=	_lpAttacker->m_pos.y;
	}
	else
	{
		posAttacker.x	=	m_pos.x;
		posAttacker.y	=	m_pos.y;
	}

	getHitPointCorrectPos(lpSkill->m_wHitImageOutputPart,&posHit);

	int		x				=	posHit.x;
	int		y				=	posHit.y;
	int		iPhysicalDamage	=	_lpHitInfo->m_dwPhysicalDamage;
	int		iMagicDamage	=	_lpHitInfo->m_dwMagicDamage;
	int		iDamage			=	_lpHitInfo->getDamage();
	int		iAttackClass	=	BattleLog::eATTACK_NORMAL;
	BOOL	bIsOwnTeam		=	isOwnTeam();

	if	(((_lpAttacker && _lpAttacker->m_wSerial	==	m_wSerial	&&	m_wShakleSkill	!=	0xffff) || m_bf1IsFreezeShakle)	&&	iDamage)
	{
		bIsOwnTeam			=	FALSE;
		m_wShakleShakeTime	=	dSYNC_FPS/4;
	}
	
	if	(_lpHitInfo->isHitTheOtherSelf())
		return;

	if	(_lpHitInfo->isImmune())
		addHitEffect(0,-getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,7,1);

	if	(_lpHitInfo->isByDamagePool())	//	미스났다.
	{
	}

	if	(_lpHitInfo->isInvincible())	//	미스났다.
	{
		m_aHitEffect[getFreeHitIndex()]=	g_hem.addEffect(x,y,lpSkill->m_wHitImage,_iDirect);	//	타격효과 추가

		int	iRange	=	GetRange(g_lpHero->m_pos.x,g_lpHero->m_pos.y,m_pos.x,m_pos.y);

		if	(iRange	<=	g_iScreenWidth*g_iScreenWidth)
			if	(_bIsPlaySound)
				g_esm.play("nondmgatt.wav");
	}
	else
	if	(_lpHitInfo->isReflection())	//	미스났다.
	{
		return;
//		m_aHitEffect[getFreeHitIndex()]=	g_hem.addEffect(x,y,lpSkill->m_wHitImage,_iDirect);	//	타격효과 추가
//
//		if (_bIsPlaySound) g_esm.play("Blocking.wav");
	}
	else
	if	(_lpHitInfo->isDodge())	//	미스났다.
	{
		cAbility	ability;
		int			iSpentCP;

		ability.m_wSkill	=	(WORD)_lpHitInfo->m_dwPhysicalDamage;
		ability.m_wLevel	=	(WORD)_lpHitInfo->m_dwMagicDamage;

		CSkill		*lpDodgeSkill	=	ability.getSkill();

		if	(!lpDodgeSkill	||	lpDodgeSkill->m_wType != eSKILL_TYPE_DODGE)
		{
			ability.m_wSkill	=	(WORD)_lpHitInfo->m_dwPhysicalDamage/100;
			ability.m_wLevel	=	(WORD)_lpHitInfo->m_dwMagicDamage/100;
		}

		lpDodgeSkill	=	ability.getSkill();

		if	(lpDodgeSkill	&&	lpDodgeSkill->m_wType == eSKILL_TYPE_DODGE)
		{
			iSpentCP			=	ability.getSpentCP()-ability.getGetCP();
			dodgeAction(_lpAttacker,iSpentCP,ability.m_wSkill);
			addHitEffect(0,-getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,2,0);

			if	(_bIsPlaySound)
				g_esm.play("dodge.wav");
		}

		iDamage					=	0;
		iAttackClass = BattleLog::eATTACK_DODGE;
	}
	else
		if	(_lpHitInfo->isMiss())	//	미스났다.
		{
			m_aHitEffect[getFreeHitIndex()]=	g_hem.addInfo(x,y,dHIT_INFO_RED_MISS,0,0,bIsOwnTeam);	//	미스~
			iDamage					=	0;
			iAttackClass = BattleLog::eATTACK_MISS;
			if	(_lpHitInfo->isLuckyDodge())	
			{
				iAttackClass = BattleLog::eATTACK_LUCKY_DODGE;
			}
		}
	else
	if	(_lpHitInfo->isBlock() || _lpHitInfo->isNoActionBlock() || _lpHitInfo->isDancingBlockerBlock())	//	블럭!!
	{
		if (_lpHitInfo->isBlock())
		{
			m_aHitEffect[getFreeHitIndex()]=	g_hem.addInfo(x,y,dHIT_INFO_BLOCK,0,0,bIsOwnTeam);	//	블럭~

			blockingAction(_lpAttacker,dDEFAULT_SHIELD_RIGIDITY_TIME);	//	블러킹 액션

			if (_bIsPlaySound)
				g_esm.play("Dancing Blocker.wav");
		}

		if	(_lpHitInfo->isNoActionBlock())
		{
			m_aHitEffect[getFreeHitIndex()]=	g_hem.addInfo(x,y,dHIT_INFO_BLOCK,0,0,bIsOwnTeam);	//	블럭~

			if	(m_wBlockerAngle	!=	0xffff)
			{
				m_wBlockerAngle			=	GetOvalAngleToTarget(m_pos.x,m_pos.y,posAttacker.x,posAttacker.y);
				m_wBlockerRigidityTime	=	dSYNC_FPS/2;

				CPos	pos;

				pos.Set(m_pos.x,m_pos.y);

				GetOvalAnglePos(&pos,m_wBlockerAngle,dBLOCKER_PUT_DISTANCE+getBodySize()+5);

				m_aHitEffect[getFreeHitIndex()]	=	g_hem.addEffect(pos.x-m_pos.x,pos.y-m_pos.y-getBody()->m_rectCrash.getHeight(),g_im.m_wNormalHit,_iDirect);	//	미스~

				if	(_bIsPlaySound)
					g_esm.play("Blocking.wav");
			}	
		}

		if	(_lpHitInfo->isDancingBlockerBlock())
		{
			CPos	pos;
			pos.Set(m_pos.x,m_pos.y);
			GetOvalTargetPos(&pos,posAttacker.x,posAttacker.y,getBodySize()+10);

			pos.x	-=	m_pos.x;
			pos.y	-=	m_pos.y;
			pos.y	-=	(getBody()->m_sprite.m_iHeight*3/4);

			cANM	*lpEffect	=	g_im.getDancingBlockerEffect();
			int		iAngle		=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,posAttacker.x,posAttacker.y,2);
			int		iDirectCount=	lpEffect->GetDirectCount(0);
			int		iDirect		=	GetDirect(iAngle,iDirectCount);

			m_aHitEffect[getFreeHitIndex()]=	g_hem.addEffectAndInfo(pos.x,pos.y,g_im.m_wDancingBlocker,dHIT_INFO_BLOCK,0,0,iDirect,bIsOwnTeam);	//	타격효과 추가

			if	 (_bIsPlaySound) 
				g_esm.play("Dancing Blocker 2.wav");
		}
		if	(iPhysicalDamage	)
			iPhysicalDamage	=	max(iPhysicalDamage/100,1);
		if	(iMagicDamage)	
			iMagicDamage	=	max(iMagicDamage/100,1);
		iAttackClass = BattleLog::eATTACK_BLOCK;
	}
	else
	{
		if	(_lpHitInfo->isCrush())
		{
			if	(g_bIsGuildBattleField)
			{
				iDamage			+=	m_iHP/24;//	크러쉬 블로우.. -o-
				iPhysicalDamage	+=	m_iHP/24;
			}
			else
			{
				iDamage			+=	m_iHP/3;//	크러쉬 블로우.. -o-
				iPhysicalDamage	+=	m_iHP/3;
			}
		}

		if	(iPhysicalDamage	)
			iPhysicalDamage	=	max(iPhysicalDamage/100,1);
		if	(iMagicDamage)	
			iMagicDamage	=	max(iMagicDamage/100,1);

		if	(m_wHillSkill == 0xffff || (m_wHillSkill != 0xffff && !_lpAbility->isCloseRangeDamageAttack()))
		{
			int		iHitInfoType;

			BOOL	bIsCritical	=	_lpHitInfo->isCritical();
			BOOL	bIsCrush	=	_lpHitInfo->isCrush();
			BOOL	bIsLuckDC	=	_lpHitInfo->isLuckyDoubleCritical();
			BOOL	bIsHardBlow	=	_lpHitInfo->isHardBlow();

			if	( bIsCritical	||	bIsCrush || bIsLuckDC ||bIsHardBlow)
				iHitInfoType	=	dHIT_INFO_RED_NUMBER;
			else
				iHitInfoType	=	dHIT_INFO_WHITE_NUMBER;

			if	(lpSkill->m_wDamageAttribute	==	0 && _bIsMirrorTower == FALSE)
				m_aHitEffect[getFreeHitIndex()]	=	g_hem.addEffect(x,y,lpSkill->m_wHitImage,_iDirect);	//	타격효과 추가
			else
			{
				if	(bIsHideDamageNumber)
				{
					iPhysicalDamage	=	0;
					iMagicDamage	=	0;
				}
				WORD	wHitImage = lpSkill->m_wHitImage;
				if(_lpHitInfo->isHitFire())
				{
						wHitImage =521;// g_im.getEffectIndex("1071_hit");// 파이어히트!	
				}
				//데미지 숫자를 출력한다.
				m_aHitEffect[getFreeHitIndex()]=	g_hem.addEffectAndInfo(x,y,wHitImage,iHitInfoType,iPhysicalDamage,iMagicDamage,_iDirect,bIsOwnTeam);	//	타격효과 추가
	
			}

			if	(_lpHitInfo->isCrush()	)
			{
				addHitEffect(0,-getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,1,1);
				iAttackClass = BattleLog::eATTACK_CRUSH;
			}
			if	(_lpAttacker && (_lpHitInfo->isLuckyHit() || _lpHitInfo->isLuckyDamage()))
			{
				_lpAttacker->addHitEffect(0,-_lpAttacker->getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,6,0);
				iAttackClass = BattleLog::eATTACK_LUCKY_HIT_OR_DAMAGE;
			}
			if	(_lpHitInfo->isCritical())
			{
				addHitEffect(0,-getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,0,1);
				iAttackClass = BattleLog::eATTACK_CRITICAL;
			}
			if	(_lpHitInfo->isLuckyDoubleCritical())
			{
				addHitEffect(0,-getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,5,0);
				iAttackClass = BattleLog::eATTACK_DOUBLE_CRITICAL;
			}
			if	(_lpHitInfo->isLuckyDodge())	
			{
				addHitEffect(0,-getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,3,0);
				iAttackClass = BattleLog::eATTACK_LUCKY_DODGE;
			}
			if	(_lpHitInfo->isLuckyDodgeCritical())
			{
				addHitEffect(0,-getBody()->m_sprite.m_iHeight+10	,g_im.m_wCriticalHitEffect,0,4,0);
				iAttackClass = BattleLog::eATTACK_LUCKY_DODGE_CRITICAL;
			}
			if	(_lpHitInfo->isHardBlow())
			{
				addHitEffect(0,-getBody()->m_sprite.m_iHeight +10	,g_im.m_wEI_Hit_Hard_Blow,0,0,0);
				iAttackClass = BattleLog::eATTACK_HARD_BLOW;
			}
			if	(_lpHitInfo->isIgnoreBlock())
			{
				addHitEffect(0,0	,g_im.m_wEI_Hit_Ignore_Block,0,0,0);
			}

			//	추가 타격 이미지
			if	(lpSkill->m_wAddHitImage	!=	0xffff)
			{
				getHitPointCorrectPos(lpSkill->m_wAddHitImageOutputPart,&posHit);
				addHitEffect(posHit.x,posHit.y,lpSkill->m_wAddHitImage,_iDirect);//	타격효과 추가
			}
			if	(lpSkill->m_bf1IsApplyFightingSpirit)
			{
				if(_lpAttacker && _lpAttacker->m_enchantedImage.m_bf1IsFightingSpirit)
					addHitEffect(0,0	,g_im.m_wEI_Hit_FightingSpirit,0,0,0);
			}
		}
		else
		{
			CActiveSkill	*lpHillSkill	=	g_sm.get(m_wHillSkill);

			if	(lpHillSkill)
			{
				addHitEffect(0,lpHillSkill->getIcyStalagmiteHeight()*2/3,lpHillSkill->m_lpSkill->m_wHitImage,0,1,1);
				setShakeTime(dSYNC_FPS/2);
				iDamage	=	0;
			}
		}

		if	(_bIsPlaySound)
			g_esm.playHitSound(lpSkill);
	}

// 	if(_lpHitInfo->isHit())
// 		addHitEffect(posHit.x,posHit.y ,321);
#ifdef		_DEBUG
				if (isHero() ||  (_lpAttacker && _lpAttacker->isHero()) || g_hero.isPet(m_wSerial) || (_lpAttacker && g_hero.isPet(_lpAttacker->m_wSerial)))
				{
					g_BattleLog.WriteLog(_lpAttacker->m_strName , this->m_strName ,lpSkill , iPhysicalDamage , iMagicDamage, iAttackClass);
				}
				if(m_bIsHitAttackDoppelganger)
				{
					g_BattleLog.WriteLog(this->m_strName , this->m_strName ,lpSkill , iPhysicalDamage , iMagicDamage, iAttackClass);
				}
				else
				{
				//	g_BattleLog.WriteLog(g_hero.m_strName , this->m_strName ,lpSkill , iPhysicalDamage , iMagicDamage, iAttackClass);
				}

			if(_lpAttacker)
				g_BattleLog.showDamage(_lpAttacker->m_strName, this->m_strName, lpSkill , iPhysicalDamage , iMagicDamage, iAttackClass, _lpAttacker->isMonster() );
#endif

	if	(bIsAttackToShakle	==	FALSE)
		reduceHP(iDamage,TRUE,_lpAbility);

//	맞는 동작에 걸렸다.
//	if(_lpHitInfo->isHitAction())	hitAction();
}

//
//	예약된 대미지를 입힌다.
void
CActor::strikeBasicAttackDamageToObject()
{
	if	(m_attackInfo.m_wTarget	==	0xffff)
		return;

	switch(m_wUseSkill)
	{
		case	dSKILL_BASIC_ATTACK_TO_ARCA	:
		{
			cArcaInfo	*lpArca	=	g_arcaManager.getArca(m_attackInfo.m_wTarget);

			if	(!lpArca)
				return;

			lpArca->hit(&m_attackInfo.m_ability,&m_attackInfo.m_aInfo[0]);
		
			break;
		}

		case	dSKILL_BASIC_ATTACK_TO_DOOR	:
		{
			cAreaDoorInfo	*lpDoor	=	g_map.m_area.getDoorArea(m_attackInfo.m_wTarget);

			if	(!lpDoor)
				return;

			int	iDamage		=	m_attackInfo.m_aInfo[0].getDamage()/100;
			int	iRemainHP	=	m_attackInfo.m_aInfo[0].m_dwResultField;

			lpDoor->hit(&m_attackInfo.m_ability,&m_attackInfo.m_aInfo[0]);

			if	(isHero())
				AddSystemMessage(WHITE,dMSG_ATTACK_DOOR_RESULT_FORM,iDamage,iRemainHP);
			break;
		}
	}

	if	(isHero())
		g_hero.increaseCP(m_attackInfo.m_ability.getGetCP());

	m_attackInfo.reset();
}

//
//	예약된 대미지를 입힌다.
void
CActor::strikeBasicAttackDamage()
{
	if	(m_attackInfo.m_wTarget	==	0xffff)
		return;

	CSkill	*lpSkill	=	m_attackInfo.m_ability.getSkill();
	CActor	*lpTarget	=	&g_aActor[m_attackInfo.m_wTarget];
	BOOL	bIsHit		=	FALSE;

	int iAttackCount = 0;
	iAttackCount = lpSkill->m_wReiterationDamageCountSyncWithOverlapAction;
	int iCount =0;

	if(m_enchantedImage.m_bf1IsIncreaseAttackCount)
	{
		for(int j= 0; j<dMAX_CONTINUOUS_HIT_INFO_COUNT;++j)
		{
			if(m_attackInfo.m_aInfo[j].m_dwResultField  == 0xffffffff)
				break;
			++iCount;
		}
	}

	if(iCount)
		iAttackCount = iCount;
	if	(iAttackCount > 1)
	{
		for (int i=0;i<iAttackCount;i++)
		{
			lpTarget->hit(this,&m_attackInfo.m_ability,&m_attackInfo.m_aInfo[i]);

			if	(m_attackInfo.m_aInfo[i].isHit())
				bIsHit	=	TRUE;
		}
	}
	else
	{
		lpTarget->hit(this,&m_attackInfo.m_ability,&m_attackInfo.m_aInfo[0]);

		if	(m_attackInfo.m_aInfo[0].isHit())
		{
			bIsHit	=	TRUE;
		}
	}
	
	if	(bIsHit	&& isHero())
	{
		g_hero.increaseCP(m_attackInfo.m_ability.getGetCP());
	}

	operateGGGEffectByHit(lpTarget);
	operateGGGEffectHit(lpTarget);
	operateGGGEffectPointHit(lpTarget);

	m_attackInfo.reset();
}


//
//	예약된 대미지를 입힌다.
void
CActor::strike(CActor *_lpTarget,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iDirect,BOOL _isGetCP,BOOL _bIsPlayHitSound)
{
	_lpTarget->hit(this,_lpAbility,_lpHitInfo,_iDirect,_bIsPlayHitSound);

	if (_isGetCP && _lpHitInfo->isHit()	&&	isHero())
		g_hero.increaseCP(_lpAbility->getGetCP());

	operateGGGEffectByHit(_lpTarget);
	operateGGGEffectHit(_lpTarget);
	operateGGGEffectPointHit(_lpTarget);
}

//
//	예약된 대미지를 입힌다.
void
CActor::strikeDamage(CActor *_lpTarget,cAbility *_lpAbility,int _iDamage)
{
	CHitInfo	hitInfo;

	hitInfo.m_dwPhysicalDamage	=	_iDamage;
	hitInfo.m_dwResultField		=	0;
	hitInfo.m_dwMagicDamage		=	0;

	hitInfo.setHit();
	hitInfo.setPhysicalDamage();

	_lpTarget->hit(this,_lpAbility,&hitInfo);

	operateGGGEffectByHit(_lpTarget);
}

void
CActor::updateChainedDamage(cChanedDamageInfo *_lpInfo)
{
	if	(_lpInfo->isValid())
		return;

	if	(_lpInfo->m_dwLastDamagedFrame+_lpInfo->m_wGap	>	s_iFrameCounter)
		return;

	CHitInfo	*lpHitInfo	=	&_lpInfo->m_aInfo[_lpInfo->m_wCurrentCount];
	cAbility	ability;

	ability.set(_lpInfo->m_wSkill,1);

	CActor		*lpCaster	=	g_am.getTestedActor(_lpInfo->m_wCaster);

	hit(lpCaster,&ability,lpHitInfo);

	_lpInfo->m_wCurrentCount++;

	if	(_lpInfo->m_wCurrentCount	>=	_lpInfo->m_wCount)
		_lpInfo->reset();

	_lpInfo->m_dwLastDamagedFrame	=	s_iFrameCounter;
}

void
CActor::updateChainedDamage()
{
	for	(int i=eMaxChanedDamageCount;i;)
	{
		--i;

		if	(m_chainedDamage.m_aData[i].isValid())
			continue;

		updateChainedDamage(&m_chainedDamage.m_aData[i]);
	}
}

void
CActor::cChanedDamageInfoStack::reset()
{
	for	(int i=eMaxChanedDamageCount;i;)
	{
		--i;
		m_aData[i].reset();
	}
}

CActor::cChanedDamageInfo*
CActor::cChanedDamageInfoStack::add(int _iSkill,int _iShotCount,int _iGap,int _iCaster)
{
	if	(_iShotCount	<=	0)
		return	NULL;

	cChanedDamageInfo	*lpInfo	=	NULL;

	for	(int i=eMaxChanedDamageCount;i;)
	{
		--i;

		if	(m_aData[i].isValid())
		{
			lpInfo	=	&m_aData[i];
			break;
		}
	}

	if	 (!lpInfo)
		return	NULL;

	lpInfo->m_wCount			=	_iShotCount;
	lpInfo->m_wCurrentCount		=	0;
	lpInfo->m_wGap				=	_iGap*dSYNC_FPS/dSYNC_FPS_IN_SERVER;
	lpInfo->m_wSkill			=	_iSkill;
	lpInfo->m_dwLastDamagedFrame=	0;
	lpInfo->m_wCaster			=	_iCaster;

	return	lpInfo;
}