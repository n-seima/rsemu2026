#include <MATH.H>
#include "CActor.H"
#include "cMAP.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "CFieldItem.H"
#include "cSHAKE.H"
#include "CSound.H"
#include "console.h"

#define	dJUMP_HEIGHT	160

//
//	머신을 띄운다.
void
CActor::floatMachine(int _iSkill)
{
	m_wMachineSkill	=	_iSkill;
	m_wMachineAngle	=	0xffff;
}	//	CActor::floatMachine(int _iSkill,int _iWeapon)

//
//	머신 제거
void
CActor::releasMachine()
{
	m_wMachineSkill	=	0xffff;
	m_wMachineAngle	=	0xffff;
}	//	CActor::releasMachine()

//
//	보조마법 캐스팅
void
CActor::receiveAidMagic(int _iSkill,int _iType,int _iValue)
{
	CSkill	*lpSkill	=	&g_aSkill[_iSkill];

	addSkillHitEffect(_iSkill);

	g_esm.playHitSound(lpSkill);

	if	(!isHero())
		return;
}	//	CActor::receiveAidMagic(int _iType,int _iValue)

//
//	러쉬!!
BOOL
CActor::rush(int _iX,int _iY,cAbility *_lpAbility,WORD _wResult,int _iFrame)
{

	if	(m_wUseSpecialActionSkill	==	eRS_NONE)
		return	TRUE;

	CSkill	*lpSkill	=	_lpAbility->getSkill();

	m_iFrame	=	_iFrame;
	
	if	(lpSkill->m_bf1SetRushImageByTriggerNextFrame)
		m_iFrame++;

	int	iPPS	=	m_iPPS;

	m_iPPS		=	lpSkill->m_wSpeed;


	BOOL	bMoveResult	=	moveTo(_iX,_iY,TRUE,FALSE,TRUE);

	m_iPPS		=	iPPS;

	if	(isHero())
		g_hero.m_isWalking	=	FALSE;

	if	(bMoveResult	==	FALSE)
	{
		stop(_iX,_iY);

		if (_wResult	>	eRR_ARRIVE)
			g_shaker.add(m_pos.x,m_pos.y,3,500,0);

		return	TRUE;
	}

	m_wRushResult	=	_wResult;

   	m_wRushStatus	=	eRS_RUSH;

	if(m_bIsJumpRush)
	{
		m_wRushStatus	=	eRS_JUMP;
	}


	if	(lpSkill->m_wType==	eSKILL_TYPE_ROLLING_LOG)
	{
	   	m_wRushStatus	=	eRS_ROLLING_LOG;
		addEffect(eHEOP_FOOT,g_im.m_wTransImage);
	}
	
	if	(lpSkill->m_bf1IsWhirlRunningStyleRush)
	   	m_wRushStatus	=	eRS_WHIRL_RUNNING_TYPE_RUSH;

	return TRUE;
}
//
//	스킬 기타 정보 적용
void
CActor::operateSkillEffectForCaster(cAbility *_lpAbility)
{
	CSkill	*lpSkill=	_lpAbility->getSkill();

	m_wOverlapAnm	=	lpSkill->m_wOverlapAction;
	m_wOverlapAnm2	=	lpSkill->m_wOverlapAction2;

	if (lpSkill->m_bCharacterAfterImageType != 0)
		afterImageOn(dPUT_AFTER_IMAGE,dAFTER_IMAGE_COUNT,20,1,lpSkill->m_bCharacterAfterImageDelayTime);

	m_abilityUse.copy(_lpAbility);

	if (lpSkill->m_wCasterHitImage	!=	0xffff)
		addEffect(lpSkill->m_wCasterHitImageOutputPart,lpSkill->m_wCasterHitImage);
}

//
//	스킬 기타 정보 적용
void
CActor::operateSkillEffectForTarget(cAbility *_lpAbility)
{
	CSkill	*lpSkill=	_lpAbility->getSkill();

	if (!lpSkill)	return;

//	if (lpSkill->m_wEnchantedImage	!=	0xffff)	addEnchantedSkill(_lpAbility->m_wSkill);
}	//	CActor::operateSkillEffectForTarget(cAbility *_lpAbility)

void
CActor::castIcyStalagmite(SG_ICY_STALAGMITE	*_lpPacket)
{
	if (_lpPacket	==	NULL)
	{
		if	(!isHero())
			return;

		cAbility	*lpAbility	=	g_hero.getIcyStalagmiteSkill();

		if	(!lpAbility)
			return;

		quickAction(lpAbility);

		CActiveSkill	*lpActiveSkill	=	g_sm.get(m_wUseSkill);

		if	(lpActiveSkill)
		{
			lpActiveSkill->castIcyStalagmite();
			lpActiveSkill->setIcyStalagmiteStatus(eISS_STAND);
		}

		return;
	}

	cAbility	ability;

	if	(isHero())
		g_hero.m_iCP	=	_lpPacket->sRemainCP*100;

	ability.set(_lpPacket->wSkill,_lpPacket->wLevel);

	if	(_lpPacket->wStatus	==	eRUIS_DESTROY)
	{
		if	(m_wHillSkill	==	0xffff)
			return;

		CActiveSkill	*lpActiveSkill	=	g_sm.get(m_wHillSkill);

		if	(lpActiveSkill)
			lpActiveSkill->destroyIcyStalagmite();

		return;
	}

	if	(_lpPacket->wStatus	==	eRUIS_RELEASE)
	{
		if	(m_wHillSkill	==	0xffff)
			return;

		CActiveSkill	*lpActiveSkill	=	g_sm.get(m_wHillSkill);

		if	(lpActiveSkill)
			lpActiveSkill->sinkIcyStalagmite();

		return;
	}

	quickAction(&ability);

	if	(_lpPacket->wStatus	==	eRUIS_CANCEL)	
	{
		CActiveSkill	*lpActiveSkill	=	g_sm.get(m_wUseSkill);

		if	(lpActiveSkill)
			lpActiveSkill->setReleaseIcyStalagmite();
	}

	m_wUseSkill	=	0xffff;
}

void
CActor::releaseStalagmite()
{
	if	(m_wHillSkill	==	0xffff)
		return;

	CActiveSkill	*lpActiveSkill	=	g_sm.get(m_wHillSkill);

	lpActiveSkill->sinkIcyStalagmite();
}

void
CActor::shutInMagicBox()
{
	m_wShutInMagicBoxImage			=	getFreeHitIndex();
	m_bf1IsPlayShutInMagicBoxAnm	=	TRUE;

	addEffect(eHEOP_FOOT,g_im.m_wShutInMagicBox,0);
}

void
CActor::crushMagicBox(BOOL _bIsDeath)
{
	if	(_bIsDeath)
	{
		m_wCrushMagicBoxFrame	=	0;
		addHitEffect(0,0,g_im.m_wInstanceKillEffect);
	}
	else
	{
		m_wReleaseMagicBoxAlpha	=	dSYNC_FPS*2;
		m_iHP	=	m_iMaxHP;
	}
}

//
//	러쉬 스킬 업데이트

BOOL
CActor::updateRushSkill()
{
	if	(m_wRushStatus	==	eRS_NONE								)
		return	FALSE;
	if	(m_wRushStatus	==	eRS_READY_FOR_WHIRL_RUNNING_TYPE_RUSH	)
		return	TRUE;

	if	(m_wRushStatus	==	eRS_WAIT_TRIGGER)
	{
		m_wWaitRushTriggerTime++;

		if	(m_wWaitRushTriggerTime	>=	dSYNC_FPS/2)
		{
			stop();

			return	FALSE;
		}
	}
	else
		m_wWaitRushTriggerTime	=	0;


	BOOL	bIsLoopAni	=	m_abilityRush.getSkill()->m_bf1IsLoopRushAni;

	if (m_wRushStatus	==	eRS_READY || m_wRushStatus	==	eRS_FINISH || m_wRushStatus	==	eRS_WHIRL_RUNNING_TYPE_RUSH)
	{
		if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_iFrameCounter	-=	dSYNC_FPS;
			m_iFrame++;

			if (m_iFrame	>=	m_iFrameCount)		//	한 에니메이션이 끝났다.
			{
				if	(m_wRushStatus	==	eRS_WHIRL_RUNNING_TYPE_RUSH)
					m_iFrame=	1;
				else
				{
					if	(isExclusiveAction())		//	단발성 에니메이션이었다면 정지한다
					{
						setExclusiveAction(FALSE);

						stop();
					}

					m_iFrame		=	0;
					m_wRushStatus	=	eRS_NONE;
				}
			}

			if (m_wRushStatus	==	eRS_READY && getBody()->IsTrigger(m_iAnm,m_iFrame))
			{
				if (m_abilityRush.getSkill()->m_bf1IsLoopRushAni)
				{
					m_iFrame--;
					m_wWaitRushTriggerTime	=	0;
					m_wRushStatus			=	eRS_WAIT_TRIGGER;
				}
			}
		}

		m_iFrameCounter	+=	m_iFPS;
	}

	if	(m_wRushStatus	==	eRS_RUSH || m_wRushStatus == eRS_WHIRL_RUNNING_TYPE_RUSH || m_wRushStatus	==	eRS_ROLLING_LOG)
	{
		if	(bIsLoopAni)
		{
			if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
			{
				m_iFrameCounter	-=	dSYNC_FPS;
				m_iFrame++;

				if (m_iFrame	>=	m_iFrameCount)		//	한 에니메이션이 끝났다.
				{
					m_iFrame	=	0;

					while(1)
					{
						if (getBody()->IsTrigger(m_iAnm,m_iFrame))
							break;

						m_iFrame++;
					}
				}
			}

			m_iFrameCounter	+=	m_iFPS;
		}
		g_rapeEffect.add(m_pos.x,m_pos.y,g_im.m_wRushDustImage);

		moveUpdate();

		if	(m_isMove	==	FALSE)	
		{
			if	(isHero())
			{
				g_hero.sendStop();	//	이걸 해줘야 주변 몹들 업뎃이 된다.

				if	(m_wRushResult	>	eRR_ARRIVE)
					g_shaker.add(m_pos.x,m_pos.y,3,500,0);
			}

			m_wRushStatus	=	eRS_FINISH;
		}
	}
	else if( m_wRushStatus== eRS_JUMP_RUSH_READY)
	{
		
		if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_iFrameCounter	-=	dSYNC_FPS;
			++m_iFrame;
		}
		m_iFrameCounter	+=	m_iFPS;

		int iAngle			=	m_dwRushFrameCounter * 2.1;
		if(iAngle> 90)
			iAngle = 90;
		float fSin = dCOS_TABLE[iAngle]/ 256.0;
		m_iHeightJumpRush  = fSin * m_wIncreaseHeightJumpRush;
		++m_dwRushFrameCounter;
		float fValue	=	 iAngle/ 90.0 * m_wJumpRushLastActionFrame  ;
		m_iFrame	=	fValue;
	}
	else if( m_wRushStatus== eRS_JUMP)
	{
		if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_iFrameCounter	-=	dSYNC_FPS;
		}
		
		m_iFrameCounter	+=	m_iFPS;

		int iAngle			=	90;
		float fSin = dCOS_TABLE[iAngle]/ 256.0;
		m_iHeightJumpRush  = fSin * m_wIncreaseHeightJumpRush;
		float fValue	=	 iAngle/ 90.0 * m_wJumpRushLastActionFrame  ;
		m_iFrame	=	fValue;	
		m_dwRushFrameCounter	=	0;
		m_wRushStatus	=	eRS_JUMP_RUSH;
		m_iJumpRushRange = sqrt(GetOvalRange(m_pos.x, m_pos.y, m_posDest.x, m_posDest.y));

	}
	else if(m_wRushStatus== eRS_JUMP_RUSH)
	{
		if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_iFrameCounter	-=	dSYNC_FPS;
		}

		m_iFrameCounter	+=	m_iFPS;
		
		int iRange			=	sqrt(GetOvalRange(m_pos.x, m_pos.y, m_posDest.x, m_posDest.y));
		int	iValue		=	((float)m_iJumpRushRange - iRange) / m_iJumpRushRange * 90;

		int	iSpeed			=	(m_iPPS/dSYNC_FPS+3);
		float fSin = dCOS_TABLE[iValue+90]/ 256.0;

		m_iHeightJumpRush  = fSin * m_wIncreaseHeightJumpRush;

		int iPPS	=	m_iPPS;
		m_iPPS	=	m_iPPS + m_dwRushFrameCounter * m_wRushAcceleration;
		makePath(m_posDest.x, m_posDest.y);
		m_iPPS	=	iPPS;
		moveUpdate();
		
		if	(m_isMove	==	FALSE)	
		{
			if	(isHero())
			{
				g_hero.sendStop();	//	이걸 해줘야 주변 몹들 업뎃이 된다.
				
				if	(m_wRushResult	>	eRR_ARRIVE)
					g_shaker.add(m_pos.x,m_pos.y,3,500,0);
			}
			
			m_wRushStatus	=	eRS_FINISH;
		}	
		++m_dwRushFrameCounter;
	}

	updateAfterImage();	//	잔상효과 업데이트
	updateEffect();

	return	TRUE;
}

//
//	시미터 커팅 스킬 업데이트
BOOL
CActor::updateScimitarCuttingSkill()
{
	if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_iFrameCounter	-=	dSYNC_FPS;
		m_iFrame++;

		if (m_iFrame	>=	m_iFrameCount)	m_iFrame		=	0;	//	한 에니메이션이 끝났다.
	}

	m_iFrameCounter	+=	m_iFPS;

	updateAfterImage();	//	잔상효과 업데이트
	updateEffect();

	return	TRUE;
}

//
//	러쉬!!
BOOL
CActor::jump(int _iX,int _iY,int _iSkill)
{
	CSkill	*lpSkill	=	&g_aSkill[_iSkill];

	if (m_wJumpStatus	!=	eJS_WAIT_TRIGGER)
	{
		for (int i=m_iFrame;i<m_iFrameCount;i++)
		{
			if (getBody()->IsTrigger(m_iAnm,i))
			{
				m_iFrame	=	i;
				break;
			}
		}
	}

	int	iPPS	=	m_iPPS;

	m_iPPS		=	lpSkill->m_wSpeed;
	moveTo(_iX,_iY,TRUE,TRUE,TRUE);

	m_iPPS				=	iPPS;
	g_hero.m_isWalking	=	FALSE;
	m_wJumpStatus		=	eJS_JUMP;

	return TRUE;
}

void
CActor::stopBouncingLinear()
{
	m_wUseSpecialActionSkill	=	eSAS_NONE;
	
	stop();

	if	(isHero())
		g_hero.sendStop();	//	이걸 해줘야 주변 몹들 업뎃이 된다.
}

BOOL
CActor::updateBouncingLinear()
{
	if	(m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_iFrameCounter	-=	dSYNC_FPS;
		m_iFrame++;

		if	(m_iFrame	>=	m_iFrameCount)		//	한 에니메이션이 끝났다.
			m_iFrame	=	0;
	}

	m_iFrameCounter	+=	m_iFPS;

	moveUpdate();

	updateAfterImage();	//	잔상효과 업데이트
	updateEffect();

	return	TRUE;
}

BOOL
CActor::updateBigBullet()
{
	moveUpdate();

	updateAfterImage();	//	잔상효과 업데이트
	updateEffect();

	return	TRUE;
}

//
//	점프 스킬 업데이트
BOOL
CActor::updateJumpSkill()
{
	if (m_wJumpStatus	==	eJS_NONE)
		return	FALSE;

	if(m_wJumpStatus	==	eJS_FINISH)
		m_iFrame++;

	if (m_wJumpStatus	==	eJS_READY || m_wJumpStatus	==	eJS_FINISH)
	{
		if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_iFrameCounter	-=	dSYNC_FPS;
			m_iFrame++;

			if (m_iFrame	>=	m_iFrameCount)		//	한 에니메이션이 끝났다.
			{
				if (isExclusiveAction())		//	단발성 에니메이션이었다면 정지한다
				{
					setExclusiveAction(FALSE);
					stop();

					if	(isHero())
						g_hero.sendStop();	//	이걸 해줘야 주변 몹들 업뎃이 된다.
				}

				m_iFrame		=	0;
				m_wJumpStatus	=	eJS_NONE;

				return	TRUE;
			}

			if (m_wJumpStatus	==	eJS_READY && getBody()->IsTrigger(m_iAnm,m_iFrame))
				m_wJumpStatus	=	eJS_WAIT_TRIGGER;
		}

		m_iFrameCounter	+=	m_iFPS;
	}

	if (m_wJumpStatus	==	eJS_JUMP)
	{
		s_iFrameCounter	=	s_iFrameCounter;
//		if ((s_iFrameCounter % (dSYNC_FPS/4)) == 0)
//			makePath(m_posDest.x,m_posDest.y);

		if	(moveUpdate())
		{
			int		iRange	=	GetOvalRange(m_pos.x,m_pos.y,m_posDest.x,m_posDest.y);
			iRange			=	(int )sqrt((double)iRange);			//	남은 거리를 계산한다.
			int	iIndex		=	(m_wJumpTargetRange-iRange)*180/m_wJumpTargetRange;				//	코사인 테이블에 인덱스를 계산
			int	iMaxRange	=	m_wJumpMaxRange;
			int	iHeight		=	min(m_wJumpTargetRange*dJUMP_HEIGHT / iMaxRange, dJUMP_HEIGHT);	//	클라이막스 높이 계산

#ifdef _DEBUG			//JBC 수정 부분 점프 자연스럽게 해보기... 음 차이없으려나. 08-07-30
			m_wJumpMaxRangeSave = iRange;

			if(iRange > 0)
				m_wJumpMaxRangeSave = max(m_wJumpMaxRangeSave , iRange);
			else
				m_wJumpMaxRangeSave = 0;
			
			int nTemp = iIndex;

			if(nTemp >= 90)	
				nTemp = 180 - nTemp;

			int nPlusValue;

			if(m_wJumpMaxRangeSave > 300)
				nPlusValue = 0.7f * nTemp;
			else if(m_wJumpMaxRangeSave > 250)
				nPlusValue = 0.6f * nTemp;
			else if(m_wJumpMaxRangeSave > 200)
				nPlusValue = 0.5f * nTemp;
			else if(m_wJumpMaxRangeSave > 150)
				nPlusValue = 0.4f * nTemp;
			else if(m_wJumpMaxRangeSave > 100)
				nPlusValue = 0.3f * nTemp;
			else if(m_wJumpMaxRangeSave > 50)
				nPlusValue = 0.2f * nTemp;
			else
				nPlusValue = 0.1f * nTemp;

				m_wJumpHeight = dCOS_TABLE[nTemp]*iHeight/256 + nPlusValue ; 
			//dprint("높이 = %d , 인덱스 = %d , 거리 = %d" , m_wJumpHeight , nTemp , iRange);

			//if (m_isMove	==	FALSE)	
			//	m_wJumpStatus	=	eJS_FINISH;
			
			
			if(m_bIsJumpAttack)
			{
				if (m_isMove	==	FALSE)	
				{
					m_wJumpStatus	=	eJS_FINISH;
					m_bIsJumpAttack = FALSE;
				}
			}	
			else
			{
				if (nTemp	==	0)
				{
					m_wJumpStatus	=	eJS_FINISH;
					m_isMove = FALSE;
					moveUpdate();
					m_bIsJumpAttack = FALSE;
				}				
			}
			
			//여기 까지.
#else			
			//원본 소스
			m_wJumpHeight = dCOS_TABLE[iIndex]*iHeight/256;

			if (m_isMove	==	FALSE)	
				m_wJumpStatus	=	eJS_FINISH;
#endif
			
		}
		else
		{
			setExclusiveAction(FALSE);
			stop();

			if	(isHero())
				g_hero.sendStop();	//	이걸 해줘야 주변 몹들 업뎃이 된다.
		}
	}

	updateAfterImage();	//	잔상효과 업데이트
	updateEffect();

	return	TRUE;
}
/*
#define	dJUMP_HEIGHT	150
//
//	쩌어어어엄뿌~~~~!!!
void
CActor::jump()
{
	m_isTriggerFrame	=	FALSE;
	m_isDamageFrame		=	FALSE;

	if (m_iFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_iFrameCounter	-=	dSYNC_FPS;

		if (m_wJumpHeight == 0)	
			m_iFrame++;

		if (m_iFrame >=	m_iFrameCount)	//	한 에니메이션이 끝났다.
		{
			m_iFrame			=	0;

			setExclusiveAction(FALSE);
			afterImageOff();

			m_pos.mx			=	m_pos.x >> dTILE_XSIZE_SHIFT;
			m_pos.my			=	m_pos.y >> dTILE_YSIZE_SHIFT;

			stop();
		}

		if (m_wJumpHeight == 0)		//	공중에 있을때는 필요 없으..
		{
			if (g_apBody[m_iBodySlot].IsTrigger(m_iAnm,m_iFrame))	m_isTriggerFrame	=	TRUE;
			if (g_apBody[m_iBodySlot].IsDamage(m_iAnm,m_iFrame))		m_isDamageFrame		=	TRUE;
		}
	}

	updateAfterImage();	//	잔상효과 업데이트

	m_iFrameCounter	+=	m_iFPS;
}	//	cACTOR::jump()


//
//	날아가는 중이다.. 훨훨~~~
//
void
CActor::jumpFly()
{
	int		iRange;

	CSkill	*lpSkill=	m_abilityUse.getSkill();

	iRange			=	GetOvalRange(m_pos.x,m_pos.y,m_posDest.x,m_posDest.y);
	iRange			=	(int )sqrt((double)iRange);								//	남은 거리를 계산한다.

	int	iIndex		=	(m_wTargetRange-iRange)*180/m_wTargetRange;				//	코사인 테이블에 인덱스를 계산
	int	iHeight		=	min(m_wTargetRange*dJUMP_HEIGHT/m_abilityUse.getAttackRange(),dJUMP_HEIGHT);	//	클라이막스 높이 계산

	m_wJumpHeight	=	dCOS_TABLE[iIndex]*iHeight/256;							//	현재 높이 계산

	if (iRange	==	0)															//	도착했다.
	{
		g_aSM[m_wUseSkill].m_cStatus=	dJUMP_LAND;								//	착륙~~

		if (lpSkill->m_dwEffect)	m_iFPS	=	32;		//	공격이라면 빨리 돌려 버린다.

		m_iDirect		=	getDirect(g_aSM[m_wUseSkill].m_wAngle,TRUE);		//	방향 조정

		m_wJumpHeight	=	0;		//	강제로 착륙 시키고..
		m_iFrameCounter	=	0;		//	프레임 카운터 초기화
		m_iFrame++;					//	프레임 증가

		if (g_apBody[m_iBodySlot].IsTrigger(m_iAnm,m_iFrame))	m_isTriggerFrame	=	TRUE;
		if (g_apBody[m_iBodySlot].IsDamage(m_iAnm,m_iFrame))	m_isDamageFrame		=	TRUE;

		return;
	}

	jump();																		//	기본 업데이트

	moveUpdate();
}	//	cACTOR::jumpFly()*/