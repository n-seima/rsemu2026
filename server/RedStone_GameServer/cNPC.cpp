#include "cNPC.H"
#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"

void
cNPC::reset(BOOL _bIsSleep)
{
	m_wActorSerial	=	0xffff;
	m_wBoostPower	=	100;
	m_dwLastBeatTime=	0xffffffff;
	m_wFirstBeater	=	0xffff;
	m_wFirstHitter	=	0xffff;
	m_bIsSleep		=	_bIsSleep;
	m_wNonTargetCount=	0;
	m_bWakeUpNow	=	FALSE;
	m_wHitterMaxLevel	=	0;
	
	if	(_bIsSleep)
		m_iLastTime	=	0;

	memset(m_aBeater			,0xff	,sizeof(m_aBeater));
	memset(m_aDamage			,0		,sizeof(m_aDamage));
	memset(m_aHitter			,0xff	,sizeof(m_aHitter));
	memset(m_aPersonalDamageSum	,0		,sizeof(m_aPersonalDamageSum));
	memset(m_awNonTarget		,0xff	,sizeof(m_awNonTarget));
}

BOOL
cNPC::isNonTarget(int _iSerial)
{
	for (int i=0;i<m_wNonTargetCount;i++)
	{
		if (m_awNonTarget[i][0]	==	_iSerial)
			return	TRUE;
	}

	return	FALSE;
}

void
cNPC::addNonTarget(int _iSerial,int _iTime)
{
	if (m_wNonTargetCount	>=	dMAX_NON_TARGET)
	{
		for (int i=0;i<dMAX_NON_TARGET-1;i++)
		{
			m_awNonTarget[i][0]	=	m_awNonTarget[i+1][0];
			m_awNonTarget[i][1]	=	m_awNonTarget[i+1][1];
		}

		m_awNonTarget[dMAX_NON_TARGET-1][0]	=	_iSerial;
		m_awNonTarget[dMAX_NON_TARGET-1][1]	=	_iTime;

		return;
	}

	m_awNonTarget[m_wNonTargetCount][0]	=	_iSerial;
	m_awNonTarget[m_wNonTargetCount][1]	=	_iTime;

	m_wNonTargetCount++;
}

void
cNPC::updateNonTarget()
{
	int	iNonTargetCount	=	m_wNonTargetCount;

	for (int i=0;i<iNonTargetCount;i++)
	{
		if	(m_awNonTarget[i][1]	==	0)
		{
			m_awNonTarget[i][0]	=	0xffff;

			for (int j=i;i<dMAX_NON_TARGET-1;i++)
			{
				m_awNonTarget[j][0]	=	m_awNonTarget[i+1][0];
				m_awNonTarget[j][1]	=	m_awNonTarget[i+1][1];
			}

			m_wNonTargetCount--;

			continue;
		}

		m_awNonTarget[i][1]--;
	}
}

BOOL
cNPC::setFirstBeater(int _iSerial)
{
	if	(m_wFirstBeater	==	0xffff || m_wFirstBeater == _iSerial)
	{
		m_wFirstBeater	=	_iSerial;

		return	TRUE;
	}

	return	FALSE;
}

//
//	누군가 나간다.
void
cNPC::outBeater(int _iActorSerial,int _iPartySerial,BOOL _isPartyLeave)
{
	int		iIndex;
	DWORD	dwHitDamage	=	0;

	if (m_wFirstHitter	==	_iActorSerial)
		m_wFirstHitter	=	0xffff;

	if (m_wFirstBeater	==	_iActorSerial)
		m_wFirstBeater	=	0xffff;

	if (m_wFirstBeater	==	_iPartySerial+dPARTY_FLAG && _isPartyLeave)
		m_wFirstBeater	=	0xffff;

	for (iIndex=0;iIndex<dMAX_HITTER;iIndex++)	//	히터 목록에서 제거
	{
		if	(m_aHitter[iIndex]	!=	_iActorSerial)
			continue;

		dwHitDamage					=	m_aPersonalDamageSum[iIndex];
		m_aHitter[iIndex]			=	0xffff;
		m_aPersonalDamageSum[iIndex]=	0;
	}

	for (iIndex=0;iIndex<dMAX_BEATER;iIndex++)	//	비터 목록에서 제거
	{
		int	iBeater	=	m_aBeater[iIndex];

		if (iBeater	==	0xffff)	continue;

		if (iBeater	>=	dPARTY_FLAG)
		{
			iBeater	-=	dPARTY_FLAG;

			if (iBeater	==	_iPartySerial)
			{
				if (_isPartyLeave)
				{
					m_aBeater[iIndex]		=	0xffff;
					m_aDamage[dMAX_BEATER]	+=	m_aDamage[iIndex];
					m_aDamage[iIndex]		=	0;
				}
				else
				{
					if (m_aDamage[iIndex]	>=	dwHitDamage)	m_aDamage[iIndex]	-=	dwHitDamage;
					else										m_aDamage[iIndex]	=	0;

					m_aDamage[dMAX_BEATER]	+=	dwHitDamage;
				}
			}

			continue;
		}

		if (iBeater	==	_iActorSerial)
		{
			m_aBeater[iIndex]		=	0xffff;
			m_aDamage[dMAX_BEATER]	+=	m_aDamage[iIndex];
			m_aDamage[iIndex]		=	0;
		}
	}
}

//
//언 파티가 해체된다.
void
cNPC::operateBreakParty(int _iPartySerial)
{
	_iPartySerial	+=	dPARTY_FLAG;

	if (m_wFirstBeater	==	_iPartySerial)
		m_wFirstBeater	=	0xffff;

	for (int iIndex=0;iIndex<dMAX_BEATER;iIndex++)
	{
		int	iBeater	=	m_aBeater[iIndex];

		if (iBeater	==	0xffff			)	continue;
		if (iBeater	!=	_iPartySerial	)	continue;
		{
			m_aBeater[iIndex]		=	0xffff;
			m_aDamage[dMAX_BEATER]	+=	m_aDamage[iIndex];
			m_aDamage[iIndex]		=	0;

			continue;
		}
	}
}

//
//	가장 큰 데미지를 입힌넘
int
cNPC::getWinningHitter()
{
	DWORD	dwMaxDamage	=	0;
	int		iSerial		=	0xffff,i;

	for (i=0;i<dMAX_BEATER;i++)
	{
		if (m_aBeater[i]	==	0xffff)
			continue;

		if (m_aDamage[i]	>	dwMaxDamage)
		{
			dwMaxDamage	=	m_aDamage[i];
			iSerial		=	m_aBeater[i];
		}
	}

	return	iSerial;
}

//
//	가장 큰 데미지를 입힌넘
int
cNPC::getSlugger()
{
	DWORD	dwMaxDamage	=	0;
	int		iSerial		=	0xffff,i;

	for (i=0;i<dMAX_HITTER;i++)
	{
		int	iHitter	=	m_aHitter[i];
		
		if	(iHitter	==	0xffff)
			continue;

		if	(m_aPersonalDamageSum[i]	>	dwMaxDamage)
		{
			dwMaxDamage	=	m_aPersonalDamageSum[i];
			iSerial		=	iHitter;
		}
	}

	return	iSerial;
}

void
cNPC::wakeUpNow(int _iX,int _iY)
{
	m_bWakeUpNow	=	TRUE;

	if	(_iX)
	{
		m_posSleep.x	=	_iX;
		m_posSleep.y	=	_iY;
	}
}

//
//	타격 정보 업데이트
BOOL
cNPC::updateBeatInfo(int _iFrame)
{
	int	i;

	if	(m_dwLastBeatTime != 0xffffffff && m_dwLastBeatTime + dRESET_BEAT_INFO_PERIOD <=	_iFrame)
	{
		m_wHitterMaxLevel	=	0;
		memset(m_aBeater			,0xff	,sizeof(m_aBeater));
		memset(m_aDamage			,0		,sizeof(m_aDamage));
		memset(m_aHitter			,0xff	,sizeof(m_aHitter));
		memset(m_aPersonalDamageSum	,0		,sizeof(m_aPersonalDamageSum));

		m_dwLastBeatTime	=	0xffffffff;
		m_wFirstBeater		=	0xffff;
		m_wFirstHitter		=	0xffff;

		return	TRUE;
	}

	for (i=0;i<dMAX_HITTER;i++)
	{
		if (m_aHitter[i]	==	0xffff)
			continue;

		if (m_aLastHitTime[i]+dRESET_BEAT_INFO_PERIOD	<	_iFrame)
			m_aHitter[i]	=	0xffff;
	}

	for (i=0;i<dMAX_BEATER;i++)
	{
		if (m_aBeater[i]	==	0xffff)
			continue;

		if (m_aLastBeatTime[i]+dRESET_BEAT_INFO_PERIOD	<	_iFrame)
		{
			m_aBeater[i]			=	0xffff;
			m_aDamage[dMAX_BEATER]	+=	m_aDamage[i];
		}
	}

	return	FALSE;
}

//
//	히터 데미지 등록
void
cNPC::addHitterDamage(int _iLevel,int _iSerial,int _iDamage,int _iFrame)
{
	int	i;

	if	(_iSerial	==	0xffff)
		return;

	m_dwLastBeatTime	=	_iFrame;	//	1분

	m_wHitterMaxLevel	=	max(m_wHitterMaxLevel,_iLevel);

	if	(m_wFirstHitter	==	0xffff)
		m_wFirstHitter	=	_iSerial;

	for (i=0;i<dMAX_HITTER;i++)
	{
		if	(m_aHitter[i]	==	_iSerial)
		{
			m_aPersonalDamageSum[i]	+=	_iDamage;
			m_aLastHitTime[i]		=	_iFrame;

			return;
		}
	}

	for (i=0;i<dMAX_HITTER;i++)
	{
		if	(m_aHitter[i]	==	0xffff)
		{
			m_aHitter[i]			=	_iSerial;
			m_aPersonalDamageSum[i]	=	_iDamage;
			m_aLastHitTime[i]		=	_iFrame;
			return;
		}
	}

	int		iOldestTime	=	0x7fffffff;
	int		iRemoveHitter	=	0;

	for (i=0;i<dMAX_HITTER;i++)
	{
		if	(m_aLastHitTime[i]	<	iOldestTime)
		{
			iOldestTime	=	m_aLastHitTime[i];
			iRemoveHitter	=	i;
		}
	}

	m_aHitter[iRemoveHitter]			=	_iSerial;
	m_aPersonalDamageSum[iRemoveHitter]	=	_iDamage;
	m_aLastHitTime[iRemoveHitter]		=	_iFrame;
}

//
//	솔로한테 맞았다. -o-
void
cNPC::addDamageBySolo(int _iLevel,int _iSerial,int _iDamage,int _iFrame)
{
	int	i;

	addHitterDamage(_iLevel,_iSerial,_iDamage,_iFrame);

#ifdef	dIS_USE_BLOCK_STEAL_SYSTEM
	if	(_iSerial	!=	m_wFirstBeater && m_wFirstBeater != 0xffff)
		_iSerial	=	0xffff;
#endif

	if	(_iSerial	==	0xffff)
	{
		m_aDamage[dMAX_BEATER]	+=	_iDamage;
		return;
	}

	for (i=0;i<dMAX_BEATER;i++)
	{
		if	(m_aBeater[i]	!=	_iSerial)
			continue;

		m_aDamage[i]		+=	_iDamage;
		m_aLastBeatTime[i]	=	_iFrame;
		return;
	}

	for (i=0;i<dMAX_BEATER;i++)
	{
		if	(m_aBeater[i]	!=	0xffff)	
			continue;

		m_aBeater[i]		=	_iSerial;
		m_aDamage[i]		=	_iDamage;
		m_aLastBeatTime[i]	=	_iFrame;
		return;
	}

//	돌림빵-_- 하는 넘들이 꽉 찼다.
	int		iOldestTime	=	0x7fffffff;
	int		iRemoveBeater	=	0;

	for (i=0;i<dMAX_BEATER;i++)
	{
		if	(m_aLastBeatTime[i]	<	iOldestTime)
		{
			iOldestTime		=	m_aLastBeatTime[i];
			iRemoveBeater	=	i;
		}
	}

	m_aBeater[iRemoveBeater]	=	_iSerial;
	m_aDamage[iRemoveBeater]	=	_iDamage;
	m_aLastBeatTime[iRemoveBeater]=	_iFrame;
}

//
//	파티한테 맞았다. -o-
void
cNPC::addDamageByParty(int _iLevel,int _iPartySerial,int _iActorSerial,int _iDamage,int _iFrame)
{
	int	i;

	addHitterDamage(_iLevel,_iActorSerial,_iDamage,_iFrame);

	_iPartySerial	+=	dPARTY_FLAG;

#ifdef	dIS_USE_BLOCK_STEAL_SYSTEM
	if (_iPartySerial	!=	m_wFirstBeater && m_wFirstBeater != 0xffff)
		_iPartySerial	=	0xffff;
#endif

	if (_iPartySerial	==	0xffff)
	{
		m_aDamage[dMAX_BEATER]	+=	_iDamage;
		return;
	}

	for (i=0;i<dMAX_BEATER;i++)
	{
		if (m_aBeater[i]	!=	_iPartySerial)	continue;

		m_aDamage[i]		+=	_iDamage;
		m_aLastBeatTime[i]	=	_iFrame;
		return;
	}

	for (i=0;i<dMAX_BEATER;i++)
	{
		if (m_aBeater[i]	!=	0xffff)	continue;

		m_aBeater[i]		=	_iPartySerial;
		m_aDamage[i]		=	_iDamage;
		m_aLastBeatTime[i]	=	_iFrame;
		return;
	}

//	돌림빵-_- 하는 넘들이 꽉 찼다.
	int		iOldestTime		=	0x7fffffff;
	int		iRemoveBeater	=	0;

	for (i=0;i<dMAX_BEATER;i++)
	{
		if (m_aLastBeatTime[i]	<	iOldestTime)
		{
			iOldestTime		=	m_aLastBeatTime[i];
			iRemoveBeater	=	i;
		}
	}

	m_aBeater[iRemoveBeater]	=	_iPartySerial;
	m_aDamage[iRemoveBeater]	=	_iDamage;
	m_aLastBeatTime[iRemoveBeater]=	_iFrame;
}

//
//	공훈 계산
void
cNPC::calcMerit()
{
	int	iTotalDamage	=	0,i;

	m_wBeaterCount		=	0;

	for (i=0;i<dMAX_BEATER+1;i++)
	{
		if	(m_aBeater[i]==	0xffff)
			break;

		iTotalDamage	+=	m_aDamage[i];
	}

	iTotalDamage	+=	m_aDamage[dMAX_BEATER];

	if	(iTotalDamage<=	0)
	{
		m_aDamage[0]	=	1;
		iTotalDamage	=	1;
	}

	for (i=0;i<dMAX_BEATER;i++)
	{
		if	(m_aBeater[i]==	0xffff)
			break;
		
		LONGLONG	llDamage	=	m_aDamage[i];

		llDamage				=	llDamage*100/iTotalDamage;	//	데미지가 공훈으로 바뀐다(100%기준..)

		m_aDamage[i]			=	(DWORD)llDamage;
		m_wBeaterCount	++;
	}

	if	(m_aDamage[dMAX_BEATER])
		m_wBeaterCount	++;
}

//
//	최대 공훈!!
int
cNPC::getMaxMerit()
{
	DWORD	dwMaxMerit	=	0,i;

	for (i=0;i<dMAX_BEATER;i++)
	{
		if	(m_aBeater[i]==	0xffff)
			break;

		if	(m_aDamage[i]	>	dwMaxMerit)
			dwMaxMerit	=	m_aDamage[i];
	}

	return	dwMaxMerit;
}

//
//누군가 한테 죽었다. -o-!!
void
cNPC::killedBy(char *_strKillerName)
{
	if (STRCMP(m_strKiller,_strKillerName)	==	0)	
		m_wKilledBySameCharacterCount++;
	else											
		m_wKilledBySameCharacterCount=1;

	strcpy(m_strKiller,_strKillerName);
}

//
//	이런 시리얼이 붙어 있는 상점을 가지고 있냐?(대화 중에..)
BOOL
cNPC::isOwnedShop(int _iSerial)
{
	if	(!m_pSpeech)
		return	FALSE;

	return	m_pSpeech->isExistShop(_iSerial);
}

void
cNPC::checkBeginBattleEvent(cACTOR *_lpOwner,cACTOR *_lpTarget)
{
	cFIELD			*lpField		=	_lpOwner->m_lpField;
	cCHARACTER		*lpCharaceter	=	&lpField->m_aCharacter[m_wCharacter];
	CKarmaContainer	*lpEvent		=	lpCharaceter->m_pEvent[eMECC_BEGIN_BATTLE];

	if	(lpEvent	==	NULL)
		return;

	if	(g_bIsWantPrintTriggerCondition)
		printf("\n◆ 전투 시작 이벤트 체크 시작\n");

	cACTOR	*lpActor	=	_lpTarget->getTamer();

	if	(lpActor		==	NULL)
		lpActor			=	_lpTarget;

	if	(!lpActor)
		return;

	if	(_lpOwner->m_lpTarget	&&	lpActor->m_wSerialInField	==	_lpOwner->m_lpTarget->m_wSerialInField)
		return;

	_lpOwner->setTarget(lpActor);

	lpEvent->operate(lpActor,_lpOwner,lpField);
}

void
cNPC::checkKilledByPlayerEvent(cACTOR *_lpOwner,cACTOR *_lpKiller)
{
	cFIELD			*lpField		=	_lpOwner->m_lpField;
	cCHARACTER		*lpCharaceter	=	&lpField->m_aCharacter[m_wCharacter];
	CKarmaContainer	*lpEvent		=	lpCharaceter->m_pEvent[eMECC_KILLED_BY_PLAYER];

	if	(lpEvent	==	NULL)
		return;

	cACTOR	*lpActor	=	_lpKiller->getTamer();

	if	(lpActor	==	NULL)
		lpActor		=	_lpKiller;

	if	(!lpActor	||	lpActor->isDeath())
		return;

	if	(g_bIsWantPrintTriggerCondition)
		printf("\n◆ 죽음 이벤트 체크 시작\n");

	lpEvent->operate(_lpKiller,_lpOwner,lpField);
}

void
cNPC::checkKillPlayerEvent(cACTOR *_lpOwner,cACTOR *_lpTarget)
{
	cFIELD			*lpField		=	_lpOwner->m_lpField;
	cCHARACTER		*lpCharaceter	=	&lpField->m_aCharacter[m_wCharacter];
	CKarmaContainer	*lpEvent		=	lpCharaceter->m_pEvent[eMECC_KILL_PLAYER];

	if (lpEvent	==	NULL)
		return;

	if (g_bIsWantPrintTriggerCondition)
		printf("\n◆ 죽임 이벤트 체크 시작\n");

	cACTOR	*lpActor	=	_lpTarget->getTamer();

	if	(lpActor	==	NULL)
		lpActor	=	_lpTarget;

	lpEvent->operate(_lpTarget,_lpOwner,lpField);
}

void
cNPC::operateAlwaysEvent(cFIELD *_lpField)
{
	cACTOR			*lpOwner		=	_lpField->getActor(m_wActorSerial);

	if (lpOwner	==	NULL)
		return;

	cCHARACTER		*lpCharaceter	=	&_lpField->m_aCharacter[m_wCharacter];
	CKarmaContainer	*lpEvent		=	lpCharaceter->m_pEvent[eMECC_ALWAYS];
	cACTOR			*lpTarget		=	lpOwner->m_lpTarget;

	if (lpEvent	==	NULL)
		return;

	if	(lpTarget	==	lpOwner)
		lpTarget	=	lpOwner->m_lpLastAttacker;

	if	(lpTarget	==	NULL)
		lpTarget	=	lpOwner;

	lpEvent->operate(lpTarget,lpOwner,lpOwner->m_lpField);
}