#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "CGame.H"
#include "CDataSave.H"
#include "CGuild.H"
#include "secretDungeon.H"

//
//	1초에 한번 처리하는 작업 처리
void
cACTOR::operateOncePerSecondWorkForMonster()
{
//	setPoisonDamage(600,1,0);
	++m_wNonActionTime;

	if	(m_wUpkeepTargetTime)
		--m_wUpkeepTargetTime;

	if	(m_wCorrectValueByBardTitleTime)
	{
		--m_wCorrectValueByBardTitleTime;

		if	(m_wCorrectValueByBardTitleTime	==	0)
			m_wCorrectValueByBardTitle		=	0;
	}
	
	if	(m_lpTarget)
		m_wPeaceTime	=	0;
	else
	{
		if	(m_wPeaceTime	<	0xffff)
			++m_wPeaceTime;
	}

	if	(isDeath())
		return;

	if	(m_bIsGuildObject)
	{
		LONGLONG	llMaxHP	=	getMaxHP();

		if	(m_bIsGuildCrest)
			llMaxHP		=	llMaxHP*(m_lpField->m_iRemainStategyPoint+1)/(c_iGuildStategyPointCount+1);

		int	iMaxHP	=	(int )llMaxHP;

		if	(m_iHP	<	iMaxHP)
		{
			m_iHP	+=	100;
			m_iHP	=	min(m_iHP,iMaxHP);

			SG_REMAIN_HP	packet;

			packet.base.set(sizeof(packet),dSG_REMAIN_HP);

			LONGLONG	llMaxHP		=	getMaxHP();
			LONGLONG	llHP		=	m_iHP;

			llHP					=	llHP*60000/llMaxHP;
			packet.wRemainHPRate	=	(WORD)llHP;
			packet.wActor			=	m_wSerialInField;
			packet.bf1IsSyncUnionParty=	FALSE;		// 수정.
			packet.bf1IsGuildObject = TRUE;		//길드오브젝트.


			if	(m_iHP > 0)
				packet.wRemainHPRate	=	max(packet.wRemainHPRate,1);

			m_lpField->sendPacketToAllPlayer(&packet);
		}

		m_lpField->thrust(this,getBodySize(),getBodySize());
	}

	decreaseAidMagicTime();

	if	(isPureMonster() && m_lpField->m_wIsGuildHall	==	FALSE)
	{
		cNPC	*lpNpc	=	getNpc();

		if	(lpNpc)
			lpNpc->updateNonTarget();

		if	(m_wCurrentField < dINSTANCE_FIELD_FLAG && !m_lpField->checkLocalPartPlayerForMonster(m_iLocalPart))	//	주변에 플레이어도 없는체 3분이 지났다.
		{
			if	(lpNpc && lpNpc->m_iRegenerationCycle	<	60*30 && lpNpc->m_wIsBlockToAutoRegen == FALSE)	//	리젠 시간이 30분 이하인 경우만 슬립 들어감
			{
				if	(m_iNotFindPlayerCount	>=	60*3)	//	
				{
					m_iDeathCount	=	2;

					lpNpc->m_posSleep.x	=	m_pos.x;
					lpNpc->m_posSleep.y	=	m_pos.y;

					immediatelyExit(TRUE);

					return;
				}

				m_iNotFindPlayerCount++;

				return;
			}
		}

		m_iNotFindPlayerCount	=	0;

		if	(m_wRestraintTimeByBattle == 0 && m_iHP < getMaxHP())
		{
			int	iRegenHP	=	(getLevel()+getConstitution()/5);

			m_iHP			+=	iRegenHP;
			m_iHP			=	min(m_iHP,getMaxHP());
		}
	}
	else
	if	(isSummonBeast())
	{
		if	((g_iCounterBySecond%10)	==	0)
		{
			cACTOR	*lpTamer	=	getTamer();

			if	(lpTamer)
			{
				int	iRegenHP	=	(getConstitution() + getLevel()+lpTamer->getConstitution()) * 5;

				increaseHP(iRegenHP);
			}
		}
	}
	else
	if	(isPet())
	{
		if	((g_iCounterBySecond%10)	==	0)
		{
			cACTOR	*lpTamer	=	getTamer();

			if	(lpTamer)
			{
				int	iRegenHP	=	(getConstitution() + getLevel()+lpTamer->getConstitution()) * 2;
//	v펫 자동 HP Regen 기능 : (펫의 건강 + 펫의 레벨 + 조련사의 건강 ) * 0.02 /10초

				increaseHP(iRegenHP);
			}
		}
	}

	if	(g_bIsDuelServer)
	{
		if	(m_wDuelBuffTime)
		{
			m_wDuelBuffTime--;
			
			if	(m_wDuelBuffTime	==	0)
			{
				m_wDuelBuffType	=	eDUEL_BUFF_NONE;
				addSendActorMagicEffectStatus();
			}
		}
	}
	
	operateMonsterAuraSkill();

	if	(m_wIsBoss	&&	m_wInhaleTime	==	0)
		m_lpField->thrustActor(m_pos.x,m_pos.y,this,NULL,m_wTeam,NULL,getBodySize()+20,0);

	if	(m_wMirrorImageMasterNpcSerial	!=	0xffff)
	{
		if	(m_lpField->getMirrorImageActorCount()	==	0)
		{
			m_lpField->m_apNpcGenerateData[m_wMirrorImageMasterNpcSerial]->wakeUpNow(m_pos.x,m_pos.y+getBodySize()/2);

			m_wInvincibleTime			=	0;
			m_wMirrorImageMasterNpcSerial=	0xffff;
			m_bf1IsBlockToRevenge		=	FALSE;

			setAiReady();
		}
	}

	if(m_wGGGDebufTimeByWaterGGG)
	{
		m_wGGGDebufTimeByWaterGGG--;
	}
}

//
//	프레임에 의한 시간 업데이트
BOOL
cACTOR::updateStatusByFrameForMonster()
{
	if (m_wBlockedStressedTime)
		m_wBlockedStressedTime--;
	if (m_wUpdateOuterActorInfoPeriod	)
		m_wUpdateOuterActorInfoPeriod--;
	if (m_wChargeTime					)
		m_wChargeTime--;
	if (m_wBeastBerserkerTime			)
		m_wBeastBerserkerTime--;
	if (m_wBlockedPosValue)
		m_wBlockedPosValue--;

	mDS.m_aiExValue[0]	=	1;
	mDS.m_aiExValue[1]	=	0;
	mDS.m_aiExValue[2]	=	0;
	mDS.m_aiExValue[3]	=	0;

	updateEnchantedMagic();
	operateRecoverHP();	//	HP 회복
	reduceCP();

	BOOL	isFreezed	=	FALSE;

	if	(m_wStunTime+m_wFreezeTime+m_wBlockingRigidityTime+m_wHitActionTime+m_wCaughtTime)
		isFreezed	=	TRUE;

	if	(m_moveSpeed.decreaseTime())	
		setMoveAbility(m_isRunning);

	if	(m_wInvincibleTime && m_wInvincibleTime	!=	0xffff	)
		m_wInvincibleTime--;	//	무적 상태
	if	(m_wSealActionTime									)
		m_wSealActionTime--;
	if	(m_wSealRunningTime									)
		m_wSealRunningTime--;

	if	(m_wStunTime					)
		m_wStunTime--;
	if	(m_wCurseAmplificationTime		)
		m_wCurseAmplificationTime--;	//	저주 증폭 상태
	if	(m_wFreezeTime				)
	{
		m_wFreezeTime--;				//	프리즈 상태 남은 시간

		if	(m_wFreezeTime	==	0)
			setExclusiveAction(FALSE);
	}

	if	(m_wBlockingRigidityTime		)	// 09.09.09
	{	
		if	(m_wRemainActionTime	<	m_wBlockingRigidityTime)
			m_wRemainActionTime	=	m_wBlockingRigidityTime+2;

		m_wBlockingRigidityTime--;
	}

	if	(m_wHitActionTime			)
		m_wHitActionTime--;
	if	(m_wRemainActionTime			)
	{
		setExclusiveAction(TRUE);
		m_wRemainActionTime--;

		if	(m_wRemainActionTime	==	0	&&	!m_isAction)
			stopAction();
	}

	if	(m_wStunTime+m_wFreezeTime+m_wBlockingRigidityTime+m_wHitActionTime+m_wStoneTime+m_wSleepTime+m_wCaughtTime)
	{
		setExclusiveAction(TRUE);
		return	TRUE;
	}
	
	if	(m_wProhibitActionByEnchantedMagic	!=	0xffff)
		return	TRUE;

	return	FALSE;
}

//
//	업데이트
void
cACTOR::updateMonster()
{
	if	(isDummy())
	{
		operateDummy();
		return;
	}

	if(m_wAttackDoppelgangerCount)
		operateAttackDoppelganger();

	if	(isPet())
	{
		cACTOR		*lpTamer=	getTamer();

		if	(m_wPetReleaseCounter	||	!lpTamer || m_dwTamerNameHashCode != lpTamer->m_dwNameHashCode || !lpTamer->isPlayer())	//	주인이 사라졌다면 사라져 버려.
		{
			immediatelyExit();

			return;
		}
	}

	if	(m_iDeathCount)
	{
		m_iDeathCount--;

		if (m_iDeathCount	<=	0)
		{
			m_iDeathCount	=	2;

			m_lpField->addBookedRemoveActor(m_wSerialInField);
		}

		return;
	}

	if	(updateStatusByFrameForMonster()	)
		return;

	if	(m_bIsGuildObject	==	FALSE	&&	m_wBlockToActionTime	==	0)
		operateAi();

	return;
}

BOOL
cACTOR::isTiamath()
{
	if	(g_aJob[m_wJobIndex].m_wBody	==	dJOB_MONSTER_TIAMATH)	//	티아메쓰 일 경우,좌/우 촉수를 강제로 찍어 준다.
		return	TRUE;

	return	FALSE;
}
