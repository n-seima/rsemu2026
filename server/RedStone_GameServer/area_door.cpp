#include "cArea.h"
#include "cField.h"
#include "secretDungeon.h"
#include "debugCode.h"
#include "CGuild.h"
#include "actor_mini_pet.h"

void
cAreaDoorInfo::generate()
{
	m_bf1IsActive		=	TRUE;
	m_bf1IsOpened		=	FALSE;
	m_bf1IsDisarmed		=	TRUE;
	m_bf1IsUnlocked		=	TRUE;

	m_iHP				=	m_iDurability;
	m_wUpkeepActiveTime	=	0;
	m_wUpkeepUnlockTime	=	0;
	m_wUpkeepDisarmedTime=	0;
	m_wRemainRegenCheckTime=0;	//	비밀문 리젠체크까지 남은 시간.

	m_awTrapSkill[0]		=	m_wTrapSkill1		;
	m_awTrapActiveChance[0]	=	m_wTrapSkill1ActiveChance;
	m_awTrapSkill[1]		=	m_wTrapSkill2		;
	m_awTrapActiveChance[1]	=	m_wTrapSkill2ActiveChance;
	m_iCurrentDurability	=	m_iDurability;

	if (m_wTrapLevel)
		armedTrap();

	if (m_bUnlockLevel)
		lock();

	if (m_bSecretLevel)
		m_bf1IsHide		=	TRUE;
	else
		m_bf1IsHide		=	FALSE;

	m_wUpkeepActiveTime	=	0xffff;
}

void
cAreaDoorInfo::getDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo)
{
	int	iAttackPower	=	_lpAttacker->getPhysicalAttackPower(_lpAbility,0,FALSE,FALSE,NULL,FALSE,NULL)/100;
	int	iMagicPower		=	_lpAttacker->getMagicAttackPower(_lpAbility)/100;

	_lpAttacker->iMiniPet()->operateDestroyObjectSkill(&iAttackPower,&iMagicPower);

	int	iDamage			=	0;

	int	iPhysicalDamage	=	iAttackPower-m_iDefense;
	int	iMagicDamage	=	iMagicPower-m_iDefense;

	if	(iAttackPower)
		iPhysicalDamage	=	max(iPhysicalDamage,1);
	else
		iPhysicalDamage	=	0;

	if	(iMagicPower)
		iMagicDamage	=	max(iMagicDamage,1);
	else
		iMagicDamage	=	0;
	
	if	(_lpAttacker->m_lpField->m_bIsSiegeWarfareField)
	{
		int		iDefenseGuild	=	_lpAttacker->m_lpField->m_aBattleGuild[0].m_wSerial;
		CGuild	*lpDefenseGuild	=	g_guildManager.getBySerial(iDefenseGuild);
		CGuild	*lpAttackGuild	=	_lpAttacker->getGuild();
		int		iDamage	=	1;

		if	(lpAttackGuild	&&	random(100)	<	lpAttackGuild->m_wIncreaseGuildDoorEfficient)
			iDamage			=	2;

		if	(lpDefenseGuild	&&	random(100)	<	lpDefenseGuild->m_wIncreaseGuildDoorEfficient)
			iDamage			=	0;

		if	(iPhysicalDamage)
		{
			iPhysicalDamage	=	iDamage;
			iMagicDamage	=	0;
		}

		if	(iMagicDamage)
		{
			iMagicDamage	=	iDamage;
			iPhysicalDamage	=	0;
		}
	}

	iPhysicalDamage = max( (iPhysicalDamage*100) * (1.0f - (m_bDefensePercent/100.0f)) , 1);
	iMagicDamage	= max( (iMagicDamage*100) * (1.0f - (m_bDefensePercent/100.0f)) , 1);

	_lpHitInfo->m_dwAbsorbDamage	=	0;
	_lpHitInfo->m_dwPhysicalDamage	=	iPhysicalDamage;		// 09.08.13
	_lpHitInfo->m_dwMagicDamage		=	iMagicDamage;		// 09.08.13
	_lpHitInfo->setHit();
}
void
cAreaDoorInfo::hitDamage(cACTOR *_lpAttacker,int _iDamage)
{
	m_iCurrentDurability	-=	_iDamage;

	if (m_iCurrentDurability<=	0)
	{
		unlock();
		_lpAttacker->m_wAttackToObject	=	0xffff;
		_lpAttacker->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_UNLOCK_BY_ATTACK);
		sendDoorStatus();
	}
}

void
cAreaDoorInfo::appear(int _iUpkeepTime)
{
	if	(_iUpkeepTime	==	-1)
		_iUpkeepTime	=	m_wUpkeepPeriod;

	m_bf1IsActive		=	TRUE;
	m_bf1IsOpened		=	FALSE;
	m_bf1IsDisarmed		=	TRUE;
	m_bf1IsUnlocked		=	TRUE;

	m_iHP				=	m_iDurability;
	m_wUpkeepActiveTime	=	0xffff;
	m_wUpkeepUnlockTime	=	0;
	m_wUpkeepDisarmedTime=	0;
	m_wRemainRegenCheckTime=0;	//	비밀문 리젠체크까지 남은 시간.

	if (m_wTrapLevel)
		armedTrap();

	if (m_bUnlockLevel)
		lock();

	if (m_bSecretLevel)
		m_bf1IsHide		=	TRUE;
	else
		m_bf1IsHide		=	FALSE;

	if (m_wLinkSecretDungeon	&&	m_wLinkSecretDungeon!=	0xffff)
		m_wUpkeepActiveTime	=	0xffff;
}

void
cAreaDoorInfo::open()
{
	m_bf1IsOpened		=	TRUE;

	m_lpField->changeDoorCrashMap(this,TRUE);
}

void
cAreaDoorInfo::close()
{
	m_bf1IsOpened		=	FALSE;

	m_lpField->changeDoorCrashMap(this,FALSE);
}

void
cAreaDoorInfo::unlock(int _iUpkeepTime)
{
	if (_iUpkeepTime	==	-1)
		_iUpkeepTime	=	m_wUpkeepDismantlingTime;

	m_wUpkeepUnlockTime	=	max(_iUpkeepTime,1);
	m_bf1IsUnlocked		=	TRUE;
}

void
cAreaDoorInfo::lock(int _iUnlockLevel)
{
	close();

	if (_iUnlockLevel	!=	-1)
		m_bUnlockLevel	=	(BYTE)_iUnlockLevel;

	m_wUpkeepUnlockTime		=	0;
	m_bf1IsUnlocked			=	FALSE;
	m_iCurrentDurability	=	m_iDurability;
}

void
cAreaDoorInfo::disapper(int _iUpkeepTime)
{
	if (_iUpkeepTime	==	-1)
		_iUpkeepTime		=	m_wRegenPeriod;

	m_wRemainRegenCheckTime	=	_iUpkeepTime;
	m_wUpkeepActiveTime		=	0;
	m_bf1IsActive			=	FALSE;
	m_bf1IsOpened			=	FALSE;
	m_bf1IsDisarmed			=	FALSE;
	m_bf1IsUnlocked			=	FALSE;
	m_wUpkeepUnlockTime		=	0;
}

void
cAreaDoorInfo::disarmedTrap(int _iUpkeepTime)
{
	_iUpkeepTime			=	max(_iUpkeepTime,m_wUpkeepDismantlingTime);
	m_wUpkeepDisarmedTime	=	max(_iUpkeepTime,1);
	m_bf1IsDisarmed			=	TRUE;
}

void
cAreaDoorInfo::setFreeOpen()
{
	m_wTrapLevel			=	0;
	m_bDetectTrapLevel		=	0;
	m_bDisarmTrapLevel		=	0;	//	함정 탐지 레벨,함정 해체 레벨
}

void
cAreaDoorInfo::explosionTrap(cACTOR *_lpActor)
{
	for (int i=0;i<2;i++)
	{
		cTrapSkill*	lpTrap	=	NULL;

		if (m_awTrapActiveChance[i]	==	0)
			lpTrap	=	g_trapSkillManager.getSkill(m_awTrapSkill[i]);
		else
		{
			if (random(100) < m_awTrapActiveChance[i])
				lpTrap	=	g_trapSkillManager.getRandomSkill(i);
		}

		if (!lpTrap)
			continue;

		_lpActor->m_lpField->explosionTrapSkill(_lpActor,lpTrap,m_wTrapTarget,m_wTrapLevel);
	}

	disarmedTrap();
	sendDoorStatus();
}

void
cAreaDoorInfo::armedTrap()
{
	close();
	
	m_wUpkeepDisarmedTime	=	0;
	m_bf1IsDisarmed			=	FALSE;
}

void
cAreaDoorInfo::sendDoorStatus()
{
	m_lpField->addSendChangeDoorStatus(m_wSerial,1-m_bf1IsActive,m_bf1IsHide,1-m_bf1IsUnlocked,1-m_bf1IsDisarmed,1-m_bf1IsOpened,m_bf1IsActiveByKarma);
}

void
cAreaDoorInfo::operate1SecondWork()
{
	if (m_wUpkeepUnlockTime && m_wUpkeepUnlockTime != 0xffff)
	{
		m_wUpkeepUnlockTime--;

		if (m_wUpkeepUnlockTime	==	0)
		{
			lock();
			sendDoorStatus();
		}
	}

	if (m_wUpkeepDisarmedTime)
	{
		m_wUpkeepDisarmedTime--;

		if (m_wUpkeepDisarmedTime	==	0)
		{
			armedTrap();
			sendDoorStatus();
		}
	}

	if (m_bf1IsActive)
	{
		if (m_wLinkSecretDungeon	&&	m_wLinkSecretDungeon	!=	0xffff)
			m_bf1IsActive	=	m_bf1IsActive;
	}
}

void
cAreaDoorInfo::operate1MinuteWork()
{
	if	(m_wLinkSecretDungeon	&&	m_wLinkSecretDungeon	!=	0xffff)
		return;

	if (m_wRemainRegenCheckTime &&	m_wRemainRegenCheckTime != 0xffff)
	{
		if (g_bIsInnerTestServer	)
		{
			m_wRemainRegenCheckTime	=	1;
			m_wGenChance			=	100;
		}
		
		m_wRemainRegenCheckTime--;

		if (m_wRemainRegenCheckTime	==	0)
		{
			m_wRemainRegenCheckTime	=	m_wRegenPeriod;

			if (random(100)	<	m_wGenChance)
			{
				appear();
				sendDoorStatus();
			}
		}
	}

	if (m_wUpkeepActiveTime && m_wUpkeepActiveTime != 0xffff)
	{
		m_wUpkeepActiveTime--;

		if (m_wUpkeepActiveTime	==	0)
		{
			disapper();
			sendDoorStatus();
		}
	}
}

void
cAreaDoorInfo::operateMoveToTherSecretDungeon(cACTOR *_lpActor)
{
	if	(m_wLinkSecretDungeon	==	0	||	m_wLinkSecretDungeon	==	0xffff)
		return;

	cParty	*lpParty=	_lpActor->getParty();

	if	(!lpParty)
	{
		_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_REQURE_PARTY);
		return;
	}

	cSecretDungeon	*lpDungeon	=	g_pSdManager->get(m_wLinkSecretDungeon);

	if	(lpParty->checkMemberLevel(lpDungeon->m_wMinLevel,lpDungeon->m_wMaxLevel)	==	FALSE)
	{
		_lpActor->sendEtcWork(eEW_FAILED_TO_CREATE_SECRET_DUNGEON_BY_LIMIT_LEVEL,lpDungeon->m_wMinLevel,lpDungeon->m_wMaxLevel);
		_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON_BY_LIMIT_LEVEL);

		return;
	}

	cItem	*lpKey	=	_lpActor->getSecretDungeonKey(m_wLinkSecretDungeon);

	if	(!lpKey)
	{
		_lpActor->sendEtcWork(eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_NOT_EXIST_KEY,m_wLinkSecretDungeon);
		_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_DUMMY);
		return;
	}

	if	(_lpActor->isTester()	==	FALSE)
		if	(_lpActor->m_bf4SecretDungeonPlayCount	>=	dENTER_SECRET_DUNGEON_LIMIT_PER_ONE_DAY+_lpActor->m_wCorrectSecretDungeonPlayCount)
		{
			_lpActor->sendRegistServerMessage(eRSM_OVER_THE_SECRET_DUNGEON_PLAY_COUNT_PER_DAY,eSM_SYSTEM);
			_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_DUMMY);
			return;
		}

	if	(lpDungeon->m_bf1IsTestDungeon	&&	g_bIsTestServer	==	FALSE)
	{
		_lpActor->sendEtcWork(eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_NOT_EXIST_KEY,m_wLinkSecretDungeon);
		_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_DUMMY);

		lpKey->reset();
		return;
	}

	int		iIFSerial=	g_pIfManager->m_iRookie;
	int		iResult	=	g_pIfManager->create(m_wLinkSecretDungeon,lpParty,m_lpField->m_iUniqueSerial,m_wSerial);

	if	(iResult	==	eCSDR_FULL)
		_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_FULL_SECRET_DUNGEON);

	if	(iResult	==	eCSDR_ALREADY_USE_SECRET_DUNGEON)
		_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_ALREADY_USE_SECRET_DUNGEON);

	if	(iResult	==	eCSDR_SUCCESS)
	{
		_lpActor->sendRemoveItem(lpKey->m_wBaseItem,1);
		lpKey->reset();

		if	(lpDungeon->m_wActiveKeyCount)
			lpDungeon->m_wActiveKeyCount--;

		_lpActor->enterToTheInstanceField(iIFSerial,0,-1,-1);
	}
}

void
cAreaDoorInfo::operatePlayerRequest(int _iWork,cACTOR *_lpActor)
{
	if	(m_bf1IsActiveByKarma)
	{
		_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_DUMMY);
		return;
	}

	if (_iWork	==	eCDSM_ENTER_TO_THE_SECRET_DUNGEON)
	{
		if	(_lpActor && _lpActor->m_lpField)
			if	(_lpActor->m_lpField->m_wIsExistSecretDungeonGate	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON);
				return;
			}
		
		BOOL	bIsFailed	=	TRUE;
		cParty	*lpParty	=	_lpActor->getParty();

		if (lpParty)
		{
			cInstanceField	*lpIF	=	g_pIfManager->getByUniquePartySerial(lpParty->m_dwUniqueSerial);

			if	(lpIF && lpIF->m_wLinkField	==	m_lpField->m_iUniqueSerial	&& lpIF->m_wLinkGate	==	m_wSerial)
			{
				cSecretDungeon	*lpDungeon	=	g_pSdManager->get(m_lpField->m_wLinkSecretDungeon);
				
				if(lpDungeon)
				{
					if(_lpActor->m_iLevel < lpDungeon->m_wMinLevel ||_lpActor->m_iLevel > lpDungeon->m_wMaxLevel)
					{// 비밀던전 레벨제한 체크
						_lpActor->sendEtcWork(eEW_FAILED_TO_CREATE_SECRET_DUNGEON_BY_LIMIT_LEVEL,lpDungeon->m_wMinLevel,lpDungeon->m_wMaxLevel);
						_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON_BY_LIMIT_LEVEL);
						return;
					}


				}
				if (_lpActor->enterToTheInstanceField(lpIF->m_wSerial,0,-1,-1)	!=	eESDE_SUCCESS)
					bIsFailed	=	TRUE;
			}
			else
				bIsFailed	=	FALSE;
		}

		if (bIsFailed)
		{
			_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON);
			return;
		}
		else
		{
			operateMoveToTherSecretDungeon(_lpActor);
			return;
		}
	}

	if (m_bf1IsActive == FALSE)
	{
		_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_SLEEP_HIDE);

		return;
	}

	if (m_bf1IsOpened	==	FALSE	&&	m_bSecretLevel)
	{
		if (!_lpActor->isVisibleDoor(this))
		{
			_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_HIDE_DOOR);
			return;
		}
	}

	if (m_bf1IsDisarmed	==	FALSE && !_lpActor->isVisibleTrap(m_bDetectTrapLevel))
	{
		explosionTrap(_lpActor);
		m_lpField->addSendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_EXPLOSION_TRAP_BY_CAN_NOT_DETECT_TRAP,_lpActor);
		return;
	}

	switch(_iWork)
	{
		case	eCDSM_OPEN_DOOR				:
		{
			if (m_bf1IsOpened)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_OPENED);

				return;
			}

			if (m_bf1IsUnlocked	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_LOCKED);

				return;
			}

			if (m_bf1IsDisarmed	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_ARMED_TRAP);
				return;
			}

			m_lpField->addSendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_OPEN,_lpActor);

			open();
			sendDoorStatus();
			break;
		}

		case	eCDSM_ENTER_TO_THE_SECRET_DUNGEON	:
		{
			if (m_bf1IsOpened	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_CLOSED);

				return;
			}
			if (m_bf1IsUnlocked	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_LOCKED);

				return;
			}
			if (m_bf1IsDisarmed	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_ARMED_TRAP);
				return;
			}
			operateMoveToTherSecretDungeon(_lpActor);
//			sendDoorStatus();
			break;
		}

		case	eCDSM_CLOSE_DOOR			:
		{
			if (m_bf1IsOpened	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_CLOSED);

				return;
			}

			if	(m_bf1IsDisarmed	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_ARMED_TRAP);
				return;
			}

			close();
			sendDoorStatus();
			break;
		}

		case	eCDSM_ATTACK_DOOR			:
		{
			if	(m_bf1IsDestroyable	==	FALSE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_CAN_NOT_DESTROY_DOOR);

				return;
			}
			if	(m_bf1IsUnlocked		==	TRUE)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_UNLOCKED);

				return;
			}

			if	(m_bf1IsDisarmed		==	FALSE)
			{
				explosionTrap(_lpActor);
				m_lpField->addSendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_EXPLOSION_TRAP_BY_ATTACK,_lpActor);
				return;
			}

			_lpActor->attackToObject(eATTACK_OBJECT_DOOR,m_wSerial);

			break;
		}

		case	eCDSM_DISARM_DOOR			:
		{
			if (m_bf1IsOpened)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_OPENED);
				return;
			}

			if (m_bf1IsDisarmed)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_DISARMED);
				return;
			}

			cAbility	*lpDisarmTrapAbility	=	_lpActor->getDisarmTrapAbility();

			if (!lpDisarmTrapAbility)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_CAN_NOT_DISARM_TRAP);
				return;
			}

			int	iAR	=	_lpActor->actionToObject(lpDisarmTrapAbility,eTARGET_OBJECT_DOOR,m_wSerial);

			if (iAR	==	eAR_OK)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_TRY_DISARM_BY_SKILL);
			}
			else
			if (iAR	==	eAR_LOW_CP)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_TRY_DISARM_FAILED_BY_LOW_CP);
			}
			else
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_TRY_DISARM_FAILED_BY_BUSY);
			}
			break;
		}

		case	eCDSM_UNLOCK_DOOR_BY_KEY	:
		{
			if (m_bf1IsOpened)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_OPENED);
				return;
			}

			if (m_bf1IsUnlocked)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_UNLOCKED);
				return;
			}

			if (!_lpActor->isUnlockAbleDoorByKey(m_bUnlockLevel))
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_CAN_NOT_UNLOCK_DOOR_BY_NOT_EXIST_AVAIL_KEY);
				return;
			}

			int		iKey	=	_lpActor->getUnlockDoorKeySlot(m_bUnlockLevel);
			cItem	*lpKey	=	_lpActor->getInventoryItem(iKey);

			unlock();
			m_lpField->addSendDoorMessage(m_wSerial,iKey,0xffff,eDWM_UNLOCK_BY_KEY,_lpActor);
			lpKey->decreaseCount(1);

			break;
		}

		case	eCDSM_UNLOCK_DOOR	:
		{
			if	(m_bf1IsOpened)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_OPENED);
				return;
			}

			if	(m_bf1IsUnlocked)
			{
				_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_IS_UNLOCKED);
				return;
			}

			if	(m_lpField->m_bIsSiegeWarfareField)
			{
				if	(m_wRegenPeriod	==	2	&&	_lpActor->m_wTeam	==	0)
				{
					m_lpField->addSendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_OPEN,_lpActor);

					open();
					sendDoorStatus();
					return;
				}
			}

			cAbility	*lpUnlockAbility	=	_lpActor->getUnlockDoorSkill();
		
			if	(lpUnlockAbility)
			{
				cAbility	abilityUnlock;
				int			iCorrectUnlockLevel	=	_lpActor->getCorrectUnlockLevelByTitle();
				int			iCorrectSkillLevel	=	_lpActor->getCorrectSkillLevel(lpUnlockAbility);

				abilityUnlock.copy(lpUnlockAbility);
				abilityUnlock.m_wLevel			+=	iCorrectSkillLevel;

				if	(abilityUnlock.getUnlockDoorLevel()+iCorrectUnlockLevel	>=	m_bUnlockLevel)
				{
					int	iAR	=	_lpActor->actionToObject(lpUnlockAbility,eTARGET_OBJECT_DOOR,m_wSerial);

					if	(iAR	==	eAR_OK)
					{
						_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_TRY_UNLOCK_BY_SKILL);
					}
					else
					if	(iAR	==	eAR_LOW_CP)
					{
						_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_TRY_UNLOCK_FAILED_BY_LOW_CP);
					}
					else
					{
						_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_TRY_UNLOCK_FAILED_BY_BUSY);
					}

					return;
				}
			}

			int		iKey	=	_lpActor->getUnlockDoorKeySlot(m_bUnlockLevel);

			if	(iKey	!=	0xffff)
			{
				_lpActor->sendDoorMessage(m_wSerial,iKey,0xffff,eDWM_DO_YOU_WANT_UNLOCK_DOOR_USE_KEY);
				return;
			}

			_lpActor->sendDoorMessage(m_wSerial,0xffff,0xffff,eDWM_NOT_EXIST_AVAIL_KEY_OR_SKILL_FOR_UNLOCK_DOOR);

			break;
		}
	}
}