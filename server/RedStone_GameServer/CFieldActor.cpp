#include <math.h>
#include "cFIELD.H"
#include "cParty.H"
#include "cGAME.H"
#include "cACTOR.H"
#include "CPitchmanShop.H"
#include "packetManager.H"
#include "secretDungeon.H"
#include "cProject.H"
#include "cEvent.H"

//	· 1~5 레벨은 자신이 다음 레벨로 올라가는데 필요한 경험치의 1/2 이상은 얻지 못한다.	
//	· 6~10 레벨은 자신이 다음 레벨로 올라가는데 필요한 경험치의 1/5 이상은 얻지 못한다.
//	· 11~20 레벨은 자신이 다음 레벨로 올라가는데 필요한 경험치의 1/10 이상은 얻지 못한다.
//	· 21~30 레벨은 자신이 다음 레벨로 올라가는데 필요한 경험치의 1/15 이상은 얻지 못한다.
//	· 31레벨 이상은 자신이 다음 레벨로 올라가는데 필요한 경험치의 1/20 이상은 얻지 못한다.

//	획득 경험치 = (파티 경험치 + 파티 보너스 경험치)									 → 몬스터 레벨이 더 높은 경우			
//	획득 경험치 = (파티 경험치 + 파티 보너스 경험치) * ((20-(자신의 레벨- 몬스터 레벨))/20)									 → 자신의 레벨이 더 높은 경우			
//	획득 경험치 = (파티 경험치 + 파티 보너스 경험치) * (1/(자신의 레벨- 몬스터 레벨))									 → 자신의 레벨이 20이상 높은 경우			

#define	dJJUL_STANDARD_LEVEL_GAB	50
/*
· 지혜 100~200		· 보너스 경험치 = 획득 경험치 * 지혜 / 1000
· 지혜 200~400		· 보너스 경험치 = 획득 경험치 * (20% + (지혜-200) / 2000)
· 지혜 400~800		· 보너스 경험치 = 획득 경험치 * (40% + (지혜-400) / 4000)
· 지혜 800~1600	· 보너스 경험치 = 획득 경험치 * (60% + (지혜-800) / 8000)
· 지혜 1600~3200	· 보너스 경험치 = 획득 경험치 * (80% + (지혜-1600) / 16000)
· 지혜 3200~		· 보너스 경험치 = 획득 경험치 * (100% + (지혜-3200) / 32000)
*/
int
getWisdomCorrectExp(int _iExp,int _iWisdom)
{
	int	iWisdomCorrectExp = 0;
	LONGLONG	llExp	=	_iExp;

	if	(_iWisdom <	100)
		llExp	=	0;
	else
	if	(_iWisdom >=	100 && _iWisdom <=	200 )
		llExp	=	llExp*_iWisdom/2000;
	else
	if	(_iWisdom >=	200 && _iWisdom <=	400 )
		llExp	=	llExp*(10+(_iWisdom-200)/40)/100;
	else
	if	(_iWisdom >=	400 && _iWisdom <=	800 )
		llExp	=	llExp*(20+(_iWisdom-400)/80)/100;
	else
	if	(_iWisdom >=	800 && _iWisdom <=	1600 )
		llExp	=	llExp*(30+(_iWisdom-800)/160)/100;
	else
	if	(_iWisdom >=	1600 && _iWisdom <=	3200 )
		llExp	=	llExp*(40+(_iWisdom-1600)/320)/100;
	else
	if	(_iWisdom >=	3200)
		llExp	=	llExp*(50+(_iWisdom-3200)/640)/100;

	iWisdomCorrectExp	=	(int)llExp;

	return	iWisdomCorrectExp;
}

int
GetCorrectExpByLevelGap(int iSharedExp,int _iPlayerLevel,int _iMonsterLevel,BOOL _bIsNewCalc=FALSE)
{
//	한국,일본을 제외한 서비스에서만 새로운 수식 사용
	BOOL	bIsNewCalc	=	TRUE;
#ifdef	_FOR_JAPAN
	bIsNewCalc	=	FALSE;
#elif	_FOR_KOREA
	bIsNewCalc	=	FALSE;
#elif	_FOR_USA
	bIsNewCalc	=	FALSE;
#endif

	if	(_bIsNewCalc)
		bIsNewCalc	=	_bIsNewCalc;

 	if	(bIsNewCalc)
 	{
 		int	iLevelRange	=	_iPlayerLevel-_iMonsterLevel-10;
 
 		if	(iLevelRange	>	0)
 		{
 			iSharedExp		-=	iSharedExp*iLevelRange*2/100;
 
 			iSharedExp		=	max(iSharedExp,1);
 		}
 
 		LONGLONG	llExp	=	iSharedExp;

 		if	(_iPlayerLevel	>=	250)
 		{
 			float	fExp	=	(float)llExp;
 			float	fLevel	=	(float)_iPlayerLevel;
 			fExp	=	fExp*(1.0f-((fLevel-245.0f)/1800.0f));
 			llExp	=	(LONGLONG)fExp;
 		}
 
 		iSharedExp	=	(int)llExp;
 
 		return	max(iSharedExp,1);
 	}

//	→ 레벨이 같거나 몬스터 레벨이 더 높다.
//	보정 없음.
	int	iLevelGap	=	_iPlayerLevel-_iMonsterLevel;

	if	(iLevelGap	<	0)
		iLevelGap	=	-iLevelGap;

	int			iCorrectValue=	75;
	LONGLONG	llExp		=	iSharedExp;

	if	(iLevelGap	>=	iCorrectValue)
		llExp		=	llExp/iLevelGap;
	else
	if	(iLevelGap	>	0)
		llExp		=	llExp*(iCorrectValue-iLevelGap)/iCorrectValue;

	if	(bIsNewCalc)		//일본과 한국을 제외한 다른 나라는 경험치 회득 공식이 다르다. -_-;
 	{
		int	iHighLevelFactor	=	_iPlayerLevel-99;
		
		if	(_iPlayerLevel >=800)
			llExp	=	llExp*(100-(iHighLevelFactor*100/910)     )*75/(_iPlayerLevel- 425)/100;
		else
		if	(_iPlayerLevel >=700)
			llExp	=	llExp*(100-(iHighLevelFactor*100/915)     )*75/(_iPlayerLevel- 425)/100;
		else
		if	(_iPlayerLevel >=600)
			llExp	=	llExp*(100-(iHighLevelFactor*100/920)     )*75/(_iPlayerLevel- 425)/100;
		else
		if	(_iPlayerLevel >=500)
			llExp	=	llExp*(100-(iHighLevelFactor*100/925)     )*75/(_iPlayerLevel- 425)/100;
		else
		if	(_iPlayerLevel >=400)
			llExp	=	llExp*(100-(iHighLevelFactor*100/930)     )/100;
		else
		if	(_iPlayerLevel >=300)
			llExp	=	llExp*(100-(iHighLevelFactor*100/940)     )/100;
		else
		if	(_iPlayerLevel >=250)
			llExp	=	llExp*(100-(iHighLevelFactor*100/990)     )/100;
								
		iSharedExp	=	(int)llExp;
	}
	else
	{
		//	전체 필드 밸런스 수정하면서 패치할 예정인 수식
		if	(_iPlayerLevel	>=	250)
		{
			float	fExp	=	(float)llExp;
			float	fLevel	=	(float)_iPlayerLevel;
			fExp	=	fExp*(1.0f-((fLevel-245.0f)/1800.0f));
			llExp	=	(LONGLONG)fExp;
		}

		iSharedExp	=	(int)llExp;
	}
	
	return	max(iSharedExp,1);
}
cACTOR *
cFIELD::findNeareastActor(int _iLocalPart, WORD* _awExceptionSerialList,int _iExceptionSerialCount,int _iX,int _iY,int _iSight,int _iTeam)
{
	int					iSerial=0xffff,iMinRange=0x7fffffff,iRange,i;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_iLocalPart];
	int					iBeginTeam=0,iEndTeam=dTEAM_COUNT;

	_iSight		*=	_iSight;

	if	(_iTeam	!=	-1)
	{
		iBeginTeam	=	_iTeam;
		iEndTeam	=	_iTeam+1;
	}

	for (int iTeam=iBeginTeam;iTeam<iEndTeam;iTeam++)
	{
		for (i=lpPart->m_aiActorCount[iTeam];i;i--)
		{
			cACTOR	*lpActor=	getTestedActor(lpPart->m_apActorList[iTeam][i-1]);

			if	(lpActor == NULL	||	lpActor->isDeath()	)
				continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if	(iRange	<=	_iSight)
			{
				if	(iRange < iMinRange)
				{	
					if	(isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
						continue;
					BOOL	bIsSameSerial	=	FALSE;
					for(int j=0;j<_iExceptionSerialCount;++j)
					{
						if(lpActor->m_wSerialInField	== _awExceptionSerialList[j])
						{
							
							bIsSameSerial	= TRUE;
							break;
						}
					}
					if(bIsSameSerial)
						continue;
					iMinRange	=	iRange;
					iSerial		=	lpActor->m_wSerialInField;
				}
			}
		}
	}

	return getActor(iSerial);
}
int
GetLimitedExpByLevelGap(int _iHitterMaxLevel,int _iSharedExp,int _iPlayerLevel,int _iMonsterLevel,BOOL _bIsNewCalc=FALSE)
{
	BOOL	bIsNewCalc	=	TRUE;
//	한국,일본,미국을 제외한 서비스에서만 새로운 수식 사용
#ifdef	_FOR_JAPAN
	bIsNewCalc	=	FALSE;
#elif	_FOR_KOREA
	bIsNewCalc	=	FALSE;
#elif	_FOR_USA
	bIsNewCalc	=	FALSE;
#endif

	if	(bIsNewCalc)
	{
 		int	iLevelRange	=	_iHitterMaxLevel	-	_iPlayerLevel;
 
 		if	(iLevelRange	>=	30)
 		{
 			float	fExp=	(float)_iSharedExp;
 			float	fLR	=	(float)iLevelRange;
 
 			_iSharedExp	=	(int)(fExp*(1.0f/9.0f - fLR/900.0f));
 		}
 		else
 		if	(iLevelRange	>=	100)
 			_iSharedExp	=	1;
//	파티원의 최고레벨과 최저레벨의 차의 절대값이 30 이상이고 99 이하이면, 모든 파티원은 최종 획득 경험치는 아래 수식과 같이 보정된다.
//	획득 경험치 * (1 / 9 - (파티원 최고 레벨  - 파티원 최저 레벨) / 900)
	}
	else
	{
	//	→ 레벨이 같거나 몬스터 레벨이 더 높다.
	//	보정 없음.

		if	(_iMonsterLevel	>	_iPlayerLevel)
		{
			int	iLevelGap		=	_iPlayerLevel-_iMonsterLevel;

			if (iLevelGap	<=	5)
				_iSharedExp	=	min(_iSharedExp,g_aExpTable[_iPlayerLevel]/2);
			else
			if (iLevelGap	<=	10)	
				_iSharedExp	=	min(_iSharedExp,g_aExpTable[_iPlayerLevel]/5);
			else
			if (iLevelGap	<=	20)	
				_iSharedExp	=	min(_iSharedExp,g_aExpTable[_iPlayerLevel]/10);
			else
			if (iLevelGap	<=	30)	
				_iSharedExp	=	min(_iSharedExp,g_aExpTable[_iPlayerLevel]/15);
			else
				_iSharedExp	=	min(_iSharedExp,g_aExpTable[_iPlayerLevel]/20);
		}
	}

	return	max(_iSharedExp,1);
}

//
//	제거될 녀석 관련 작업
void
cFIELD::operateForDeathActorWork(cACTOR	*_lpOuter)
{
	_lpOuter->resetStatusForDeath();

//	이 녀석을 타겟으로 삼고 있었던 녀석들 처리
	for (int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
		for (int i=m_aiActorCount[iTeam];i;)
		{
			--i;

			cACTOR	*lpActor	=	getActor(m_awActorList[iTeam][i]);

			if	(!lpActor)
				continue;

			lpActor->actorDeath(_lpOuter->m_wSerialInField);
		}


//	이 녀석을 타겟으로 삼고 있었던 스킬들 처리
	m_skillManager.actorDeath(_lpOuter->m_wSerialInField);
}

//
//	제거될 녀석 관련 작업
void
cFIELD::readyToRemoveActorWork(int _iSerial)
{
	cACTOR	*lpOuter;
	int		i,iCount;
	BOOL	isPartyLeave		=	FALSE;
	int		iPichmanShopSerial	=	0xffff;

	lpOuter						=	getTestedActor(_iSerial);

	if	(!lpOuter	||	lpOuter->m_bf1IsOperateReadyToRemoveActorWork)
		return;

	operateForDeathActorWork(lpOuter);

	lpOuter->resetStatusForExitField();

	lpOuter->m_bf1IsOperateReadyToRemoveActorWork	=	TRUE;

//	lpOuter->kill();		//	사형 선고를 내리고..
//	플레이어에게만 있는 정보

	if	(lpOuter->isPlayer())	
	{
//	파티정보 업데이트
		if (lpOuter->getPartySerial()!=	0xffff		)
		{
			int	iPartyMemberCount		=	getPartyMemberCount(lpOuter->m_wTeam,lpOuter->getPartySerial());

			if	(iPartyMemberCount	<=	2	)
				isPartyLeave		=	TRUE;	//	파티 쫑
		}

//	필드에 떨어진 아이템 소유 변경
		m_droppedItem.outActor(lpOuter->m_wSerialInField,lpOuter->getPartySerial(),isPartyLeave,this);
		
		for (i=0;i<m_wArcaCount;i++)
		{
			cArca	*lpArca	=	g_arcaManager.get(m_awArcaList[i]);

			if (!lpArca)
				continue;

			if (lpArca->m_wOwner	==	lpOuter->m_wSerialInField)
				lpArca->m_wOwner	=	0xffff;
			else
			if (isPartyLeave && lpArca->m_wOwnerParty==	lpOuter->getPartySerial())
				lpArca->m_wOwnerParty=	0xffff;
		}
	}

//	함정 전부 해체
	disarmTrapsOwnedOutActor(_iSerial);

//	이녀석이 때리고 다닌 녀석들 정보 변경
	for (i=0,iCount=0;i<dMAX_NPC_COUNT&&iCount<m_iSavedActorCount;i++)
	{
		if (m_apNpcGenerateData[i] == NULL || m_apNpcGenerateData[i]->m_wActorSerial==0xffff)
			continue;

		iCount++;
		m_apNpcGenerateData[i]->outBeater(_iSerial,lpOuter->getPartySerial(),isPartyLeave);
	}
}

//
//	파티에게 경험치 부여
BOOL
cFIELD::giveExpToParty(int _iHitterMaxLevel,cACTOR *_lpKiller,int _iPartyBeater,BOOL _bIsExistSlugger,int _iMerit,int _iExp,cACTOR *_lpTarget)
{
	mCS(g_pPartyManager->m_csPartyWork);

	cParty	*lpParty	=	g_pPartyManager->get(_iPartyBeater);

	if	(!lpParty)
		return	FALSE;

	lpParty->checkData();

	int		iMemberCount;
	int		iMember,iInrangeMemberLevelSum;
	int		iReceiveMemberCount			=	0;
	int		aGetActor[8],iGetActorCount	=	0;
	int		iOriginalExp				=	_iExp;
	int		iCorrectLevel				=	10;
	int		iTopLevel					=	lpParty->getTopLevel();
	int		iBonusExp;

	iMemberCount			=	lpParty->getMemberCountForGiveExp(m_iUniqueSerial,&_lpTarget->m_pos,&iBonusExp);
	iInrangeMemberLevelSum	=	lpParty->getInrangeMemberLevelSum(m_iUniqueSerial,&_lpTarget->m_pos);
	iInrangeMemberLevelSum	=	iInrangeMemberLevelSum+iCorrectLevel*iMemberCount;

	if	(iMemberCount	<=	0)
		return	FALSE;

	if (_lpKiller	&&	iMemberCount	==	1)
	{
		giveExpToActor(_iHitterMaxLevel,_lpKiller->m_wSerialInField,_bIsExistSlugger,_iMerit,_iExp,_lpTarget,_lpTarget->getNpc());

		return	TRUE;
	}

	if (_bIsExistSlugger)	//	공헌도 50% 초과의 파티가 존재한다.
	{
		LONGLONG	llExp	=	_iExp;

		if (_iMerit	>	50)
			llExp	=	llExp*_iMerit/100/2+llExp/2;//	내 공헌도가 50이 넘을경우에 절반을 가져간다. 파티 보너스 경험치 = 악 보정후 경험치/2
		else
			llExp	=	llExp*_iMerit/100/2;		//	파티 경험치 = 0.5 * 파티 공헌도 * 악 보정후 경험치

		_iExp		=	(int)llExp;
	}
	else
		_iExp	=	_iMerit*_iExp/100;

	memset(aGetActor,0xff,sizeof(aGetActor));

	for (iMember=0;iMember<(int)lpParty->getMemberCount() && iReceiveMemberCount<iMemberCount;iMember++)
	{
		if	(!lpParty->isOnField(iMember,m_iUniqueSerial,m_wGateField))
			continue;

		cACTOR	*lpActor	=	getPlayerByName(lpParty->m_aMember[iMember].m_strName);

		if	(!checkActor(lpActor) || lpActor->isDeath()							)
			continue;

		if	(lpActor->getPartySerial() == 0xffff || lpActor->getPartySerial() != _iPartyBeater	)
			continue;

		{
			BOOL	bIsBug	=	FALSE;

			for (int j=0;j<iGetActorCount;j++)
			{
				if (aGetActor[j] == lpActor->m_wSerialInField)
				{
					CLOG("partExpError","---------------------------------------");
					CLOG("partExpError","[%s] 경치 두번 먹어?? [%s/%s][%d:%d] - %d",lpParty->m_strPartyName,lpActor->m_strId,lpActor->m_strName,aGetActor[j],lpActor->m_wSerialInField,iMember);

					int	k;

					for (k=0;k<(int)lpParty->getMemberCount();k++)
					{
						cPARTY_MEMBER	*lpMember		=	&lpParty->m_aMember[k];
						cACTOR			*lpMemberActor	=	lpParty->getActorByMemberIndex(k);

						if (lpMemberActor)
							CLOG("partExpError","멤버cur '%s'[%s/%s]",lpMember->m_strName,lpMemberActor->m_strId,lpMemberActor->m_strName);
						else
							CLOG("partExpError","멤버cur '%s'",lpMember->m_strName);
					}

					bIsBug	=	TRUE;
				}
			}

			if (bIsBug)
				continue;
		}

		int	iRange		=	GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,_lpTarget->m_pos.x,_lpTarget->m_pos.y);

		if	(iRange		>=	dMINIMUM_AVAIL_PARTY_RANGE || lpActor->isIdlePlayer())
			continue;

		int		iLevelGabWithTopLevel	=	iTopLevel-lpActor->m_iLevel;

		if	(iLevelGabWithTopLevel	<	0)
			iLevelGabWithTopLevel	=	-iLevelGabWithTopLevel;

		BOOL	bIsNewCalc	=	FALSE;

// 		if	(lpActor->m_bf4RebirthCount)
// 			if	(iLevelGabWithTopLevel	>=	33	&&	iLevelGabWithTopLevel	<	100)	
// 				bIsNewCalc	=	TRUE;

//	레벨차에 따른 경험치 보정
		__int64	iGainExp	=	GetCorrectExpByLevelGap(_iExp,lpActor->m_iLevel,_lpTarget->m_iLevel,bIsNewCalc);

//	최종 획득 경험치 = 획득 경험치 * (1 + 파티 내 선 캐릭터 수/100 + 파티 내 절대선 캐릭터 수/20+선구자 보정율) / (파티원수^(1/3))
//	악, 절대 악 캐릭터의 최종 획득 경험치 = 획득 경험치 * (1 + 파티 내 선 캐릭터 수/200 + 파티 내 절대선 캐릭터 수/100) / (파티원수^(1/3))
 //                                                   + 악/절대 악 캐릭터가 독점하는 경험치 * (레벨 차이에 의한 보정 (4번 항목))
		int		iLevel	=	lpActor->m_iLevel+iCorrectLevel;
		double	dValue	=	pow((double)iMemberCount,0.3333f);

//	최종 획득 경험치 = 획득 경험치 * (1 + 파티 내 선 캐릭터 수/200 + 파티 내 절대선 캐릭터 수/100) 
		iGainExp		=	iGainExp*(100+iBonusExp)/100;
		iGainExp		=	(__int64)(iGainExp*1000/dValue/1000);

		iGainExp	=	max(iGainExp,1);


//	지혜로 인한 경험치 보너스· 보너스 경험치 = 획득 경험치 * 지혜 / 1000
		{
			int	iWisdomCorrectExp	=	getWisdomCorrectExp(iGainExp,lpActor->getWisdom());
			int	iLowLevelCorrectExp	=	0;

			if	(lpActor->m_bIsApplyHuntingExpBonus	&&	lpActor->m_iLevel	<=	200 && g_pPartyManager->isNotJJUL(lpActor))
			{	
				iLowLevelCorrectExp	=	iGainExp*(201-lpActor->m_iLevel)/100;
			}
			else
			if	(lpActor->m_iLevel	<	100	&&	iTopLevel	<	lpActor->m_iLevel+dJJUL_STANDARD_LEVEL_GAB)
				iLowLevelCorrectExp	=	iGainExp*(101-lpActor->m_iLevel)/100;

			iGainExp				+=	(iLowLevelCorrectExp+iWisdomCorrectExp);
		}

//	마지막 보정 - 경험치 너무 많이 가져 가지마.. -o-
		iGainExp					=	GetLimitedExpByLevelGap(_iHitterMaxLevel,iGainExp,lpActor->m_iLevel,_lpTarget->m_iLevel);
		iGainExp					=	min(iOriginalExp,iGainExp);

		if	(lpActor->m_bf4RebirthCount)
			if	(iLevelGabWithTopLevel	>=	100)
				iGainExp	=	1;

		iReceiveMemberCount++;

		aGetActor[iGetActorCount]	=	lpActor->m_wSerialInField;
		iGetActorCount++;
			
		lpActor->increaseExperience((int)iGainExp);
	}

	return	TRUE;
}

//
//	파티에게 경험치 부여
void
cFIELD::giveExpToActor(int _iHitterMaxLevel,int _iSerial,BOOL _bIsExistSlugger,int _iMerit,int _iExp,cACTOR *_lpTarget,cNPC *_lpNpc)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if	(!checkActor(lpActor))
		return;
	if	(lpActor->isDeath())
		return;

	lpActor	=	lpActor->getControlLord();
	int	iOriginalExp	=	_iExp;

	if	(!lpActor)
		return;

	if (_lpNpc->m_wBeaterCount	>	1)
	{
		if (_bIsExistSlugger)	//	공헌도 50% 초과의 파티가 존재한다.
		{
			if (_iMerit	>	50)
				_iExp	=	_iMerit*_iExp/100/2+_iExp/2;//	내 공헌도가 50이 넘을경우에 절반을 가져간다. 파티 보너스 경험치 = 악 보정후 경험치/2
			else
				_iExp	=	_iMerit*_iExp/100/2;		//	파티 경험치 = 0.5 * 파티 공헌도 * 악 보정후 경험치
		}
		else
			_iExp	=	_iMerit*_iExp/100;
	}

	_iExp			=	max(1,_iExp);

//	레벨차에 따른 경험치 보정
	_iExp			=	GetCorrectExpByLevelGap(_iExp,lpActor->m_iLevel,_lpTarget->m_iLevel);

	BOOL	bIsNewCalc	=	TRUE;
#ifdef	_FOR_JAPAN
	bIsNewCalc	=	FALSE;
//#elif	_FOR_KOREA
	bIsNewCalc	=	FALSE;
#endif

// 	if	(bIsNewCalc)
// 	{
// 		_iExp	=	GetLimitedExpByLevelGap(_iHitterMaxLevel,_iExp,lpActor->m_iLevel,_lpTarget->m_iLevel);
// 		_iExp	=	min(iOriginalExp,_iExp);
// 	}

	_iExp		=	max(_iExp,1);

//	지혜로 인한 경험치 보너스· 보너스 경험치 = 획득 경험치 * 지혜 / 1000
	{
		int	iWisdomCorrectExp	=	getWisdomCorrectExp(_iExp,lpActor->getWisdom());
		int	iLowLevelCorrectExp	=	0;

		if	(lpActor->m_bIsApplyHuntingExpBonus	&&	lpActor->m_iLevel	<=	200 && g_pPartyManager->isNotJJUL(lpActor))
			iLowLevelCorrectExp	=	_iExp*(201-lpActor->m_iLevel)/100;
		else
		if	(lpActor->m_iLevel	<=	100)
			iLowLevelCorrectExp	=	_iExp*(101-lpActor->m_iLevel)/100;

		_iExp					+=	(iLowLevelCorrectExp+iWisdomCorrectExp);
	}

	lpActor->increaseExperience(_iExp);	//	경험치 증가
}

//
//	_lpKiller가 _lpTarget을 죽였다.
BOOL
cFIELD::kill(cACTOR *_lpKiller,cACTOR *_lpTarget,BOOL _bIsByKarma)
{
	if	(_lpTarget->isNpc())
		return	TRUE;

	int		i,iMerit;
	BOOL	isKilledByParty	=	FALSE,isExistSlugger=FALSE,bIsNoExpAndItem	=	m_bIsGuildPointBattleField;
	int		iExp;

	if	(_lpKiller)
		_lpKiller	=	_lpKiller->getControlLord();

	if	(_lpKiller)
	{
		BOOL	bIsCheckGuildStatue	=	TRUE;

		if	(_lpKiller->m_wLastCheckMinute	==	0	||	
			_lpKiller->m_wLastCheckMinute	==	g_currentTime.m_wHour*60+g_currentTime.m_wMinute)
			bIsCheckGuildStatue				=	FALSE;

		_lpKiller->m_wLastCheckMinute	=	g_currentTime.m_wHour*60+g_currentTime.m_wMinute;

		if	(bIsCheckGuildStatue	&&	_lpKiller	&&	_lpKiller->m_wFindGuildStatueKind+_lpKiller->m_wFindGuildStatueLevel	>=	2)
		{
			int		iTargetLevel=	_lpKiller->m_wFindGuildStatueLevel*100;
			int		iaChance[]	=	{60,120,180,240,300};
			int		iChance		=	iaChance[_lpKiller->m_wFindGuildStatueLevel-1];

			if	(_lpTarget	&&	_lpTarget->isPureMonster()	&&	m_bIsGuildBattleField	==	FALSE	&&	(_lpTarget->m_iLevel>=iTargetLevel)	)
			{
				if	(_lpKiller->m_bf1IsTestQuest)
					iChance	=	2;

				if	(random(iChance)	==	0 || _lpKiller->m_bf1DevIsGuildItemDrop)
				{
					cItem	item,itemForLog;
					int		iItem	=	g_im.m_awGuildStatueElement[_lpKiller->m_wFindGuildStatueKind-1][_lpKiller->m_wFindGuildStatueLevel-1];
					
					g_im.generateItemByBaseItem(&item,iItem,1);

					item.m_dwSerial	=	1;

					itemForLog.copy(&item);

					if	(itemForLog.isMoneyItem())
						CLOG("goldIOError","cFIELD::kill");

					if	(_lpKiller->addItemToInventory(&item)	==	eUIR_OK)
					{
						_lpKiller->sendPickItemLog(&itemForLog,ePIM_NORMAL);
						_lpKiller->sendAddItem(&itemForLog);
					}
				}
			}
		}
	}

	if	(_lpTarget)
	{
		cCHARACTER	*lpCharacter	=	_lpTarget->getCharacter();

		if	(lpCharacter	&&	lpCharacter->m_bf1IsNotExp)
		{
			bIsNoExpAndItem	=	TRUE;
	
			if	(m_iUniqueSerial!=	433)
				bIsNoExpAndItem	=	FALSE;
		}

		_lpTarget->operateSoulOath(_lpKiller);
		_lpTarget->operateBloodCompact(_lpKiller);
	}

	if	(m_bIsGuildPointBattleField && _bIsByKarma	==	FALSE)
	{
		cInstanceField	*lpIF	=	getMasterInstanceField();

		if	(_lpTarget && lpIF && !_lpTarget->isDummy()	&&	!_lpTarget->isCopyActor())
		{
			if	(_lpTarget->isPlayer()	&&	_lpTarget->m_bf1IsDeathByAnimateUndead	==	FALSE)
			{
				cParty	*lpParty	=	_lpTarget->getParty();

				int	iAverageLevel	=	_lpTarget->m_iLevel;

				if	(lpParty)
					iAverageLevel	=	lpParty->getAverageLevel();

				int	iValue1			=	iAverageLevel*100/240 + 80;

				if	(iValue1%100	>=	50)
					iValue1	+=	100;

				iValue1			/=	100;

				int	iPeneltyValue	=	_lpTarget->m_iLevel*80*iValue1;

				lpIF->addGuildHonorPoint(-iPeneltyValue);
//				(유저레벨)*(80*("유저파티평균레벨대"/240+0.8))
			}
			else
			if	(_lpTarget->isPureMonster())
			{
				int	iHonorPoint	=	_lpTarget->m_iLevel*80;

				lpIF->addGuildHonorPoint(iHonorPoint);
			}
		}
	}
	
	if	(m_bIsGuildBattleField+m_bIsSiegeWarfareField+m_bIsGuildPointBattleField	==	0)
	{
		if	(_lpKiller && _lpTarget && _lpKiller->isPlayer() && _lpTarget->isMonster())
		{
			cACTOR	*lpPlayer	=	_lpKiller;
			cParty	*lpParty	=	_lpKiller->getParty();

			if	(lpParty)
				lpPlayer	=	lpParty->getRandomInrangeMember(_lpKiller);

			if	(lpPlayer && lpPlayer->m_bf1IsCheckLottoBoxEvent == FALSE)
			{
				lpPlayer->m_bf1IsCheckLottoBoxEvent	=	TRUE;
				g_lottoEventManager.receiveLottoBox(lpPlayer,lpPlayer->m_iLevel,_lpTarget->m_iLevel);
			}
		}
	}

	if	(m_bIsSiegeWarfareField)
	{
		if	(_lpTarget)
		{
			if	(_lpTarget->m_bIsGuildCrest)
			{
				reportSuccessToInvade();

				return	TRUE;
			}

			if	(_lpTarget->m_bIsGuildObject)
			{
				destroyGuildStategyPoint();

				return	TRUE;
			}
		}
	}
	else
	if	(m_bIsGuildBattleField && m_wGuildBattleFinished == FALSE)
	{
		if	(m_wDuelType	==	0xffff)
		{
			cACTOR	*lpKiller	=	_lpKiller;

			if	(_lpTarget)
			{
				if	(_lpTarget->m_bf1IsDeathByAnimateUndead	&& _lpTarget->isPlayer())
					lpKiller	=	getTopLevelGuildMember(m_aBattleGuild[1-_lpTarget->m_wTeam].m_wSerial);

				_lpTarget->m_bf1IsDeathByAnimateUndead	=	FALSE;
			}

			if	(lpKiller && _lpTarget && _lpTarget->isPlayer() && lpKiller->isPlayer() && lpKiller->m_wTeam != _lpTarget->m_wTeam)
				addGuildBattleKillPoint(lpKiller,_lpTarget);
		}
		else
		{
			killDuelBattler(_lpKiller,_lpTarget);
		}
	}

	if	(_lpTarget->isPlayer() || _lpTarget->isPet() || _lpTarget->isSummonBeast()	||	_lpTarget->isDummy()	||	_lpTarget->isCopyActor())
	{
		if	(_lpTarget->isPlayer() && _lpKiller && _lpKiller->isMonster())
		{
			cNPC	*lpNpc	=	_lpKiller->getNpc();

			if	(lpNpc)
				lpNpc->checkKillPlayerEvent(_lpKiller,_lpTarget);
		}

		return	TRUE;
	}

	cNPC			*lpNpc			=	_lpTarget->getNpc();
	cCHARACTER		*lpCharacter	=	&m_aCharacter[lpNpc->m_wCharacter];
	cJOB			*lpJob			=	lpCharacter->getJob();
	int				iHitterMaxLevel	=	0;
	
	if	(lpNpc)
		iHitterMaxLevel	=	lpNpc->m_wHitterMaxLevel;

	if	(_lpKiller	&&	lpNpc)
	{
		lpNpc->checkKilledByPlayerEvent(_lpTarget,_lpKiller);

		g_project.m_kcMonsterDeath.operate(_lpKiller,_lpTarget,this);
	}

	if	(bIsNoExpAndItem	==	FALSE)
	{
		lpNpc->calcMerit();	//	공훈 계산

	//	몬스터 현재 경험치 = 몬스터 기본 경험치 * (몬스터 레벨+4) * 10
		LONGLONG	llExp	=	lpJob->m_dwDeathExp;

		llExp		*=	(_lpTarget->m_iLevel+4);
		llExp		*=	10;
		llExp		=	llExp*lpNpc->m_wBoostPower/100;

		if	(llExp%100	>=	50)
			llExp	=	llExp/100+1;
		else
			llExp	=	llExp/100;
		
		llExp		=	llExp*_lpTarget->m_wCorrectMonsterExp/100;
		iExp		=	(int)llExp;

		if	(lpNpc->getMaxMerit()	>	50)	
			isExistSlugger	=	TRUE;

		if	(m_bIsGuildBattleField == FALSE && _lpTarget->m_wTamer == 0xffff)
			for (i=0;i<dMAX_BEATER;i++)
			{
				int	iBeater	=	lpNpc->m_aBeater[i];

				if	(iBeater	==	0xffff)
					break;

				iMerit		=	lpNpc->getMerit(i);

				if (iBeater	>=	dPARTY_FLAG)
				{
					iBeater	-=	dPARTY_FLAG;

					if	(checkActor(_lpKiller) && _lpKiller->getPartySerial()	==	iBeater)
						giveExpToParty(iHitterMaxLevel,_lpKiller,iBeater,isExistSlugger,iMerit,iExp,_lpTarget);
					else
						giveExpToParty(iHitterMaxLevel,NULL,iBeater,isExistSlugger,iMerit,iExp,_lpTarget);
				}
				else
					giveExpToActor(iHitterMaxLevel,iBeater,isExistSlugger,iMerit,iExp,_lpTarget,lpNpc);
			}

		droppingItem(_lpTarget,_lpKiller);//아이템 떨구기..
	}

	if	(_lpKiller)
		lpNpc->killedBy(_lpKiller->m_strName);	//	킬러가 이미 죽었을 수도 있다.

	return	TRUE;
}

int
cFIELD::getInRangeEnemy(int _iX,int _iY,int _iTeam,int _iRange,WORD *_lpwList,int _iExistTargetCount,int _iBufferSize)
{
	int					iCount=0,i;
	int					iLocalPart	=	getCurrentLocalPart(_iX,_iY);
	CLocalPartionInfo	*lpPart		=	&m_pLocalPartition[iLocalPart];

	for (int iTeam=0;iTeam<2+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam	==	_iTeam)
			continue;

		for (i=lpPart->m_aiActorCount[iTeam];i;)
		{
			--i;

			int		iActor	=	lpPart->m_apActorList[iTeam][i];
			cACTOR	*lpActor=	getTestedActor(iActor);

			if	(lpActor == NULL	||	lpActor->isDeath()	)
				continue;

			for	(int iCheck=_iExistTargetCount;iCheck;)
			{
				--iCheck;

				if	(_lpwList[iCheck]	==	iActor)
				{
					lpActor	=	NULL;
					break;
				}
			}

			if	(lpActor	&&	lpActor->isInAttackRange(_iX,_iY,_iRange))
			{
				_lpwList[_iExistTargetCount+iCount]	=	iActor;

				iCount++;

				if	(_iExistTargetCount+iCount	>=	_iBufferSize)
					return	iCount;
			}
		}
	}

	return	iCount;
}

int
cFIELD::getInRangeEnemyCount(cACTOR *_lpClient,int _iRange)
{
	if	(_lpClient->m_iLocalPart	>=	m_iLocalPartitionCount || m_pLocalPartition == NULL)
		return	0;

	int					iCount=0,i;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_lpClient->m_iLocalPart];

	//예외 처리.
	if(lpPart->m_apActorList == NULL)
		return	0;

	for (int iTeam=0;iTeam<2+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam	==	_lpClient->m_wTeam)
			continue;

		for (i=lpPart->m_aiActorCount[iTeam];i;i--)
		{
			cACTOR	*lpActor=	getTestedActor(lpPart->m_apActorList[iTeam][i-1]);

			if (lpActor == NULL	||	lpActor->isDeath()	)
				continue;

			if (lpActor->isInAttackRange(_lpClient->m_pos.x,_lpClient->m_pos.y,_iRange))
				iCount++;
		}
	}

	return	iCount;
}

cACTOR *
cFIELD::findNotUnderAttackeActor(cACTOR *_lpClient)
{
	int		i,iTeam=0;
	int		iMinDistance		=	0x7fffffff;
	int		iMinDistanceTarget	=	0xffff;

	for (i=m_aiActorCount[iTeam];i;i--)	//	아군의 타겟들을 검색
	{
		int		iSerial		=	m_awActorList[iTeam][i-1];
		cACTOR	*lpActor	=	getTestedActor(iSerial);

		if	(!lpActor	||	lpActor->isDeath())
			continue;

		if	(lpActor->m_wHostileEnemyCount	==	0)
		{
			int	iRange	=	GetOvalRange(lpActor->m_pos.x,lpActor->m_pos.y,_lpClient->m_pos.x,_lpClient->m_pos.y);

			if	(iRange	<=	iMinDistance)
			{
				iMinDistance		=	iRange;
				iMinDistanceTarget	=	iSerial;
			}
		}
	}

	if	(iMinDistanceTarget	!=	0xffff)
		return	getTestedActor(iMinDistanceTarget);

	int	iMinTargeting		=	0xffff;
	int	iMinTargetingActor	=	0xffff;

	for (i=m_aiActorCount[iTeam];i;i--)	//	아군의 타겟들을 검색
	{
		int		iSerial		=	m_awActorList[iTeam][i-1];
		cACTOR	*lpActor	=	getTestedActor(iSerial);

		if	(!lpActor	||	lpActor->isDeath())
			continue;

		if	(iMinTargeting	>	lpActor->m_wHostileEnemyCount)
		{
			iMinTargeting		=	lpActor->m_wHostileEnemyCount;
			iMinTargetingActor	=	iSerial;
		}
	}

	return	getTestedActor(iMinTargetingActor);
}

//
//	가장 근접해 있는 적을 찾는다.
cACTOR *
cFIELD::findNeareastPlayer(int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient)
{
	int					iSerial=0xffff,iMinRange=0x7fffffff,iRange,i;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_iLocalPart];

	for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
	{
		for (i=lpPart->m_aiActorCount[iTeam];i;i--)
		{
			cACTOR	*lpActor=	getTestedActor(lpPart->m_apActorList[iTeam][i-1]);

			if (lpActor == NULL	||	lpActor->isDeath())
				continue;

			if (_lpClient && _lpClient->checkIsNonTarget(lpActor))
				continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if (iRange	<=	_iSight)
			{
				if (iRange < iMinRange)	//	일단은 가장 가까운 넘을 찾는데.. 나중에 기타 여러가지(?)를 추가한다.
				{	
					if	(!lpActor->m_wIsFeignDeath)
					{
						iMinRange	=	iRange;
						iSerial		=	lpActor->m_wSerialInField;
					}
				}
			}
		}
	}

	return getActor(iSerial);
}

//
//	가장 근접해 있는 적을 찾는다.
cACTOR *
cFIELD::findNeareastEnemy(int _iOwnTeam,int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient,BOOL _bIsIgnoreSleepyActor)
{
	int					iSerial		=	0xffff,iMinRange=0x7fffffff,iRange,i,iTeam;
	int					iSleepyActor=	0xffff,iSleepyActorMinRange=0x7fffffff;
	CLocalPartionInfo	*lpPart		=	&m_pLocalPartition[_iLocalPart];

	_iSight	*=	_iSight;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam	==	_iOwnTeam)
			continue;

		for (i=lpPart->m_aiActorCount[iTeam];i;)
		{
			--i;

			cACTOR	*lpActor=	getTestedActor(lpPart->m_apActorList[iTeam][i]);

			if	(lpActor == NULL	||	lpActor->isDeath())
				continue;
			
			if	(_lpClient && _lpClient->checkIsNonTarget(lpActor))
				continue;
			
			if	(lpActor->isSummonBeast() || lpActor->isPet())
				if	(lpActor->m_wAiState	<=	eAS_APPROACH)
					continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if	(iRange	<=	_iSight)
			{
				if	(isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

				if	(_bIsIgnoreSleepyActor	&&	lpActor->m_wSleepTime)
				{
					if	(iRange < iSleepyActorMinRange)	//	일단은 가장 가까운 넘을 찾는데.. 나중에 기타 여러가지(?)를 추가한다.
					{
						iSleepyActorMinRange=	iRange;
						iSleepyActor		=	lpActor->m_wSerialInField;
					}
				}
				else
				if	(iRange < iMinRange)	//	일단은 가장 가까운 넘을 찾는데.. 나중에 기타 여러가지(?)를 추가한다.
				{	
					if	(!lpActor->m_wIsFeignDeath)
					{
						iMinRange	=	iRange;
						iSerial		=	lpActor->m_wSerialInField;
					}
					
					if	(lpActor->m_wTargetedTime)
					{
						iSerial		=	lpActor->m_wSerialInField;
						break;
					}
				}
			}
		}
	}

	if	(iSerial	!=	0xffff)
		return getActor(iSerial);

	return getActor(iSleepyActor);
}

//
//	가장 멀리 있는 적을 찾는다.
cACTOR *
cFIELD::findBestFarEnemy(int _iOwnTeam,int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient)
{
	int					iSerial		=	0xffff,iFarRange=0,iRange,i,iTeam;
	CLocalPartionInfo	*lpPart		=	&m_pLocalPartition[_iLocalPart];

	_iSight	*=	_iSight;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam	==	_iOwnTeam)
			continue;

		for (i=lpPart->m_aiActorCount[iTeam];i;)
		{
			--i;

			cACTOR	*lpActor=	getTestedActor(lpPart->m_apActorList[iTeam][i]);

			if	(lpActor == NULL	||	lpActor->isDeath())
				continue;

			if	(_lpClient && _lpClient->checkIsNonTarget(lpActor))
				continue;
			
			if	(lpActor->isSummonBeast() || lpActor->isPet())
				if	(lpActor->m_wAiState	<=	eAS_APPROACH)
					continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if	(iRange	<=	_iSight)
			{
				if	(iRange > iFarRange)	//	일단은 가장 가까운 넘을 찾는데.. 나중에 기타 여러가지(?)를 추가한다.
				{	
					iFarRange	=	iRange;
					iSerial		=	lpActor->m_wSerialInField;
				}
			}
		}
	}

	return getActor(iSerial);
}

//
//	가장 근접해 있는 적을 찾는다.
cACTOR *
cFIELD::findNeareastEnemyForChainLightning(int _iOwnTeam,int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient,int _iExceptActorCount,WORD *_lpwExceptActorList)
{
	int					iSerial		=	0xffff,iMinRange=0x7fffffff,iRange,i,iTeam;
	CLocalPartionInfo	*lpPart		=	&m_pLocalPartition[_iLocalPart];

	_iSight	*=	_iSight;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if	(iTeam	==	_iOwnTeam)
			continue;

		for (i=lpPart->m_aiActorCount[iTeam];i;)
		{
			--i;

			int		iActor	=	lpPart->m_apActorList[iTeam][i];

			for (int iExceptActor=_iExceptActorCount;iExceptActor;)
			{
				--iExceptActor;

				if	(iActor	==	_lpwExceptActorList[iExceptActor])
				{
					iActor	=	0xffff;

					break;
				}
			}

			if	(iActor	==	0xffff)
				continue;

			cACTOR	*lpActor=	getTestedActor(iActor);

			if	(lpActor == NULL	||	lpActor->isDeath()	||	lpActor->m_wIsFeignDeath)
				continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if	(iRange	<=	_iSight)
			{
				if	(isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

				if	(iRange < iMinRange)	//	일단은 가장 가까운 넘을 찾는데.. 나중에 기타 여러가지(?)를 추가한다.
				{	
					iMinRange	=	iRange;
					iSerial		=	lpActor->m_wSerialInField;
				}
			}
		}
	}

	return getActor(iSerial);
}

cACTOR*
cFIELD::findRandomTargetInInsightEnemy(cACTOR *_lpClient)
{
	return	findRandomTargetInInsightEnemy(_lpClient->m_wTeam,_lpClient->m_iLocalPart,_lpClient->m_pos.x,_lpClient->m_pos.y,_lpClient->m_sSight,_lpClient);
}

//
//	가장 근접해 있는 적을 찾는다.
cACTOR *
cFIELD::findRandomTargetInInsightEnemy(int _iOwnTeam,int _iLocalPart,int _iX,int _iY,int _iSight,cACTOR *_lpClient)
{
	int					iSerial	=	0xffff,iMinRange=0x7fffffff,iRange,i,iTeam;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_iLocalPart];
	WORD				awList[256];
	int					iCount	=	0;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam	==	_iOwnTeam)
			continue;

		for (i=lpPart->m_aiActorCount[iTeam];i;i--)
		{
			cACTOR	*lpActor=	getTestedActor(lpPart->m_apActorList[iTeam][i-1]);

			if (lpActor == NULL	||	lpActor->isDeath())
				continue;

			if (_lpClient && _lpClient->checkIsNonTarget(lpActor))
				continue;
			
			if (lpActor->isSummonBeast() || lpActor->isPet())
				if (lpActor->m_wAiState	<=	eAS_APPROACH)
					continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if (iRange	<=	_iSight)
			{
				if (!lpActor->m_wIsFeignDeath)
					awList[iCount++]	=	lpActor->m_wSerialInField;
			}
		}
	}

	if (iCount	==	0)
		return	NULL;

	iSerial	=	awList[random(iCount)];

	return getActor(iSerial);
}

//
//	가장 근접해 있는 누군가를 찾는다.
cACTOR *
cFIELD::findNeareastActor(int _iLocalPart,int _iExceptionSerial,int _iX,int _iY,int _iSight,int _iTeam)
{
	int					iSerial=0xffff,iMinRange=0x7fffffff,iRange,i;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_iLocalPart];
	int					iBeginTeam=0,iEndTeam=dTEAM_COUNT;

	_iSight		*=	_iSight;

	if	(_iTeam	!=	-1)
	{
		iBeginTeam	=	_iTeam;
		iEndTeam	=	_iTeam+1;
	}

	for (int iTeam=iBeginTeam;iTeam<iEndTeam;iTeam++)
	{
		for (i=lpPart->m_aiActorCount[iTeam];i;i--)
		{
			if	(lpPart->m_apActorList[iTeam][i-1]	==	_iExceptionSerial)
				continue;

			cACTOR	*lpActor=	getTestedActor(lpPart->m_apActorList[iTeam][i-1]);

			if	(lpActor == NULL	||	lpActor->isDeath()	)
				continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if	(iRange	<=	_iSight)
			{
				if	(iRange < iMinRange)
				{	
					iMinRange	=	iRange;
					iSerial		=	lpActor->m_wSerialInField;
				}
			}
		}
	}

	return getActor(iSerial);
}

int
cFIELD::getInSightOwnerTeamActorList(WORD *_lpwActorList,int _iBufferSize,int _iX,int _iY,cACTOR *_lpActor,int _iSight,BOOL _bIsExceptOwner,BOOL _bIsWantDeathActor)
{
	int					iSerial=0xffff,iMinRange=0x7fffffff,iRange,i,iCount=0,iExceptionSerial=-1;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_lpActor->m_iLocalPart];

	_iSight	*=	_iSight;

	if (_bIsExceptOwner)
		iExceptionSerial	=	_lpActor->m_wSerialInField;

	if (_iX	==	-1)
	{
		_iX	=	_lpActor->m_pos.x;
		_iY	=	_lpActor->m_pos.y;
	}

	int iTeam=_lpActor->m_wTeam;

	for (i=lpPart->m_aiActorCount[iTeam];i;i--)
	{
		int	iSerial	=	lpPart->m_apActorList[iTeam][i-1];

		if (iSerial	==	iExceptionSerial)
			continue;

		cACTOR	*lpActor=	getTestedActor(iSerial);

		if (!lpActor)
			continue;

		if (lpActor->isDeath())
		{
			if (!_bIsWantDeathActor)
				continue;
			else
				continue;
		}

		iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

		if (iRange	<=	_iSight)
		{
			_lpwActorList[iCount++]	=	iSerial;

			if (iCount	>=	_iBufferSize)
				return	iCount;
		}
	}

	return iCount;
}

int
cFIELD::getInSightOwnerTeamActorCount(int _iX,int _iY,cACTOR *_lpActor,int _iSight,BOOL _bIsExceptOwner,BOOL _bIsWantDeathActor)
{
	int					iSerial=0xffff,iMinRange=0x7fffffff,iRange,i,iCount=0,iExceptionSerial=-1;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_lpActor->m_iLocalPart];

	_iSight	*=	_iSight;

	if (_bIsExceptOwner)
		iExceptionSerial	=	_lpActor->m_wSerialInField;

	if (_iX	==	-1)
	{
		_iX		=	_lpActor->m_pos.x;
		_iY		=	_lpActor->m_pos.y;
	}

	int iTeam	=	_lpActor->m_wTeam;

	for (i=lpPart->m_aiActorCount[iTeam];i;i--)
	{
		int	iSerial	=	lpPart->m_apActorList[iTeam][i-1];

		if (iSerial	==	iExceptionSerial)
			continue;

		cACTOR	*lpActor=	getTestedActor(iSerial);

		if (!lpActor)
			continue;

		if (lpActor->isDeath())
		{
			if (!_bIsWantDeathActor)
				continue;
			else
				continue;
		}

		iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

		if (iRange	<=	_iSight)
			iCount++;
	}

	return iCount;
}

int
cFIELD::getInSightEnemyActorList(WORD *_lpwActorList,int _iBufferSize,int _iX,int _iY,cACTOR *_lpActor,int _iSight,BOOL _bIsWantDeathActor,BOOL _bIsCheckBlock,cAbility *_lpAbility)
{
	int					iSerial=0xffff,iMinRange=0x7fffffff,iRange,i,iCount=0,iExceptionSerial=-1,iTeam;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_lpActor->m_iLocalPart];
	BOOL				bIsTagetingToBattleWithPartyMember	=	FALSE;
	BOOL				bIsTargetingToActorStickedBit		=	FALSE;
	if	(_lpAbility)
	{
		CSkill	*lpSkill	=	_lpAbility->getSkill();

		if	(lpSkill)
		{
			bIsTagetingToBattleWithPartyMember	=	lpSkill->m_bf1IsTagetingToBattleWithPartyMember;
			bIsTargetingToActorStickedBit		=	lpSkill->m_bf1IsTargetingToActorStickedBit;
		}
	}

	_iSight	*=	_iSight;

	if (_iX	==	-1)
	{
		_iX	=	_lpActor->m_pos.x;
		_iY	=	_lpActor->m_pos.y;
	}

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam	==	_lpActor->m_wTeam)
			continue;

		for (i=lpPart->m_aiActorCount[iTeam];i;i--)
		{
			int	iSerial	=	lpPart->m_apActorList[iTeam][i-1];

			cACTOR	*lpActor=	getTestedActor(iSerial);

			if	(!lpActor)
				continue;

			if	(bIsTagetingToBattleWithPartyMember)
				if	(lpActor->isBattleWithTargetParty(_lpActor)==FALSE)
					continue;

			if	(bIsTargetingToActorStickedBit)
			{
				if(lpActor->getBitCount(_lpActor->m_wSerialInField, _lpActor->m_dwNameHashCode) == 0)
					continue;
			}
			if	(lpActor->isDeath())
				if (_bIsWantDeathActor	==	FALSE)
					continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if (iRange	<=	_iSight)
			{
				if (_bIsCheckBlock)
					if (isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
						continue;

				_lpwActorList[iCount++]	=	iSerial;

				if (iCount	>=	_iBufferSize)
					return	iCount;
			}
		}
	}

	return iCount;
}

int
cFIELD::getEnemyActorListForChainLightning(WORD *_lpwActorList,cACTOR *_lpClient,int _iSight,int _iWantCount)
{
	int	iCount=0;
	int	iX	=	_lpClient->m_pos.x;
	int	iY	=	_lpClient->m_pos.y;

	_lpwActorList[0]	=	_lpClient->m_wSerialInField;
	iCount				=	1;

	for	(;_iWantCount;)
	{
		--_iWantCount;

		cACTOR	*lpTarget=	findNeareastEnemyForChainLightning(_lpClient->m_wTeam,_lpClient->m_iLocalPart,iX,iY,_iSight,_lpClient,iCount,_lpwActorList);

		if	(!lpTarget)
			return	iCount;

		_lpwActorList[iCount]	=	lpTarget->m_wSerialInField;
		iCount++;

		iX	=	lpTarget->m_pos.x;
		iY	=	lpTarget->m_pos.y;
	}

	return	iCount;
}

int
cFIELD::getInSightEnemyActorCount(int _iX,int _iY,int _iLocalPart,int _iTeam,int _iSight)
{
	int					iSerial=0xffff,iMinRange=0x7fffffff,iRange,i,iCount=0,iExceptionSerial=-1,iTeam;
	CLocalPartionInfo	*lpPart	=	&m_pLocalPartition[_iLocalPart];

	_iSight	*=	_iSight;

	for (iTeam=0;iTeam<dTEAM_COUNT-1+m_bIsGuildBattleField;iTeam++)
	{
		if (iTeam	==	_iTeam)
			continue;

		for (i=lpPart->m_aiActorCount[iTeam];i;i--)
		{
			int	iSerial	=	lpPart->m_apActorList[iTeam][i-1];

			cACTOR	*lpActor=	getTestedActor(iSerial);

			if (!lpActor	||	lpActor->isDeath())
				continue;

			iRange		=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

			if (iRange	<=	_iSight)
				iCount++;
		}
	}

	return iCount;
}

//_lpPos를 중심으로 빈자리 찾기
BOOL
cFIELD::findValidPos(cPOINT *_lpPos)
{
	_lpPos->x	/=	dTILE_XS;
	_lpPos->y	/=	dTILE_YS;
	
	int	mx	=	_lpPos->x;
	int	my	=	_lpPos->y;

	for	(int iDist=1;iDist<10;iDist++)
	{
		cPOINT	pos;
		int		iX,iY;

		//	x1,y2->x2,y2
		for (iX = -iDist; iX<=iDist;iX++)
		{
			pos.Set(mx+iX,my+iDist);

			if	(!isBlocked(pos.x,pos.y))
			{
				_lpPos->x	=	pos.x*dTILE_XS + dTILE_XS/2;
				_lpPos->y	=	pos.y*dTILE_YS + dTILE_YS/2;
				return	TRUE;
			}
		}

		//	x1,y1->x2,y1
		for (iX = -iDist; iX<=iDist;iX++)
		{
			pos.Set(mx+iX,my-iDist);

			if	(!isBlocked(pos.x,pos.y))
			{
				_lpPos->x	=	pos.x*dTILE_XS + dTILE_XS/2;
				_lpPos->y	=	pos.y*dTILE_YS + dTILE_YS/2;
				return	TRUE;
			}
		}

		//	x1,y1->x1,y2
		for (iY = -iDist; iY<=iDist;iY++)
		{
			pos.Set(mx-iDist,my+iY);

			if	(!isBlocked(pos.x,pos.y))
			{
				_lpPos->x	=	pos.x*dTILE_XS + dTILE_XS/2;
				_lpPos->y	=	pos.y*dTILE_YS + dTILE_YS/2;
				return	TRUE;
			}
		}

		//	x2,y1->x2,y2
		for (iY = -iDist; iY<=iDist;iY++)
		{
			pos.Set(mx+iDist,my+iY);

			if	(!isBlocked(pos.x,pos.y))
			{
				_lpPos->x	=	pos.x*dTILE_XS + dTILE_XS/2;
				_lpPos->y	=	pos.y*dTILE_YS + dTILE_YS/2;
				return	TRUE;
			}
		}
	}

	return	FALSE;
}

//
//	_lpAreaInfo안에 입장 가능한 위치를 찾는다.
BOOL
cFIELD::findEnterancePos(cPOINT *_lpPos,cAreaInfo *_lpAreaInfo)
{

	cRECT	rect;

	rect.x1	=	_lpAreaInfo->x1;
	rect.y1	=	_lpAreaInfo->y1;
	rect.x2	=	_lpAreaInfo->x2;
	rect.y2	=	_lpAreaInfo->y2;

	return	findEnterancePos(_lpPos,rect);
}

void
cFIELD::findEnterancePosToPointAround(cPOINT *_lpPos,int _iRange)
{
	int iWidth	=	_iRange;
	int iHeight	=	iWidth/2;	

	cRECT	rect = {_lpPos->x - iWidth, _lpPos->y - iHeight,_lpPos->x + iWidth,_lpPos->y + iHeight};

	cPOINT	pos;

	if(findEnterancePos(&pos,rect))
	{
		_lpPos->x	=	pos.x;
		_lpPos->y	=	pos.y;
	}
}

void
cFIELD::findEnterancePosToPointAroundTile(cPOINT *_lpPos,int _iTileRange)
{
	// 일정 좌표에서 타일범위 주위에 임의 점을 구한다. 

	cPOINT	pos		=	{_lpPos->x/dTILE_XS,_lpPos->y/dTILE_YS};

	cRECT	rect	= {pos.x - _iTileRange, pos.y - _iTileRange,pos.x + _iTileRange,pos.y + _iTileRange};

	rect.x1			=	max(rect.x1,2);
	rect.y1			=	max(rect.y1,2);
	rect.x2			=	min(rect.x2,m_iWidth-2);
	rect.y2			=	min(rect.y2,m_iHeight-2);

	for(int i=0;i<100;++i)
	{
		
		rect.getRandomPosInRect(&pos.x,&pos.y);

		if	(!isBlocked(pos.x,pos.y))
			break;
	}

	_lpPos->x	=	pos.x*dTILE_XS+dTILE_XS/2;
	_lpPos->y	=	pos.y*dTILE_YS+dTILE_YS/2;
}


BOOL
cFIELD::findEnterancePos(cPOINT *_lpPos,cRECT _rect)
{
	int	mx,my;
	int	iX,iY,i=100;

	cRECT	rect	=	_rect;	
	do
	{
		rect.getRandomPosInRect(&_lpPos->x,&_lpPos->y);

		mx	=	_lpPos->x/dTILE_XS;
		my	=	_lpPos->y/dTILE_YS;

		--i;

		if	(!isBlocked(mx,my))
			return	TRUE;
	}	while(i);

	rect.x1	=	max(_rect.x1/dTILE_XS,2);
	rect.y1	=	max(_rect.y1/dTILE_YS,2);
	rect.x2	=	min(_rect.x2/dTILE_XS,m_iWidth-2);
	rect.y2	=	min(_rect.y2/dTILE_YS,m_iHeight-2);

	for (iY = rect.y1; iY<=rect.y2;iY++)
	{
		for (iX = rect.x1; iX<=rect.x2;iX++)
		{
			if	(!isBlocked(mx+iX,my+iY))
			{
				_lpPos->x	=	iX*dTILE_XS+dTILE_XS/2;
				_lpPos->y	=	iY*dTILE_YS+dTILE_YS/2;
				return	TRUE;
			}
		}
	}

	return	FALSE;
}
//
//	NPC에 잘못된 비터-_-정보 처리
int
cFIELD::removeNpcValidBeaterInfo()
{
	int		i,iCount;
	int		iIncorrectDataCount	=	0;

	for (i=0,iCount=0;i<dMAX_NPC_COUNT&&iCount<m_iSavedActorCount;i++)
	{
		if (m_apNpcGenerateData[i] == NULL || m_apNpcGenerateData[i]->m_wActorSerial	==	0xffff)
			continue;

		cNPC	*lpNpc	=	m_apNpcGenerateData[i];

		for (int j=dMAX_BEATER;j;--j)
		{
			int	iSerial	=	lpNpc->m_aBeater[j-1];

			if (iSerial	!=	0xffff)
			{
				cACTOR	*lpActor	=	getTestedActor(iSerial);

				if (lpActor	==	NULL)
				{
					lpNpc->outBeater(iSerial,0xffff,FALSE);
					iIncorrectDataCount++;
				}
			}
		}

		iCount++;
	}

	return	iIncorrectDataCount;
}

//
//	외곽에 액터들 위치 업데이트
void
cFIELD::updateBorderActorPositionInfo(cACTOR *_lpPlayer,int _iX,int _iY)
{
	if (g_game.m_bIsUpdateSightBorderActorInfo == FALSE) return;

	if (!_lpPlayer->isPlayer())
		return;

	SG_OUTER_ACTORS_POS_UPDATE packet;

	packet.wCount	=	0;
	
	if (_lpPlayer->m_wUpdateOuterActorInfoPeriod)	return;

	_lpPlayer->m_wUpdateOuterActorInfoPeriod	=	dPOSITION_UPDATE_PERIOD;

	cRECT	rectOuterFrame,rectInnerFrame;
	int		iCheckSum	=	0;

	rectOuterFrame.Set(_iX-dVISIBLE_OUT_ACTOR_WIDTH,_iY-dVISIBLE_OUT_ACTOR_HEIGHT,_iX+dVISIBLE_OUT_ACTOR_WIDTH,_iY+dVISIBLE_OUT_ACTOR_HEIGHT);
	if (rectOuterFrame.x1	<	0				)	{rectOuterFrame.x2	-=	rectOuterFrame.x1;rectOuterFrame.x1	=	0;}
	if (rectOuterFrame.y1	<	0				)	{rectOuterFrame.y2	-=	rectOuterFrame.y1;rectOuterFrame.y1	=	0;}
	if (rectOuterFrame.x2	>	m_iPixelWidth	)	{rectOuterFrame.x1	-=	(rectOuterFrame.x2-m_iPixelWidth);rectOuterFrame.x2	=	m_iPixelWidth;}
	if (rectOuterFrame.y2	>	m_iPixelHeight	)	{rectOuterFrame.y1	-=	(rectOuterFrame.y2-m_iPixelHeight);rectOuterFrame.y2=	m_iPixelHeight;}

	rectOuterFrame.expand(100,50);

	rectInnerFrame.Set(_iX-dFORECAST_VISIBLE_ACTOR_WIDTH_FOR_MOVE_ACTION,_iY-dFORECAST_VISIBLE_ACTOR_HEIGHT_FOR_MOVE_ACTION,_iX+dFORECAST_VISIBLE_ACTOR_WIDTH_FOR_MOVE_ACTION,_iY+dFORECAST_VISIBLE_ACTOR_HEIGHT_FOR_MOVE_ACTION);
	if (rectInnerFrame.x1	<	0				)	{rectInnerFrame.x2	-=	rectInnerFrame.x1;rectInnerFrame.x1	=	0;}
	if (rectInnerFrame.y1	<	0				)	{rectInnerFrame.y2	-=	rectInnerFrame.y1;rectInnerFrame.y1	=	0;}
	if (rectInnerFrame.x2	>	m_iPixelWidth	)	{rectInnerFrame.x1	-=	(rectInnerFrame.x2-m_iPixelWidth);rectInnerFrame.x2	=	m_iPixelWidth;}
	if (rectInnerFrame.y2	>	m_iPixelHeight	)	{rectInnerFrame.y1	-=	(rectInnerFrame.y2-m_iPixelHeight);rectInnerFrame.y2=	m_iPixelHeight;}
	rectOuterFrame.expand(-50,-25);

	for (int i=m_iStandActorCount;i;--i)
	{
		int		iSerial	=	m_awStandActorList[i-1];
		cACTOR *lpActor	=	getActor(iSerial);

		if (!lpActor)
			continue;
		if (lpActor->isMonster() && lpActor->isDeath())
			continue;

		if (rectOuterFrame.Isin(lpActor->m_pos.x,lpActor->m_pos.y))	//	외곽 프레임 안에는 있고
		{
			if (!rectInnerFrame.Isin(lpActor->m_pos.x,lpActor->m_pos.y))	//	내부 프레임 안에는 없다.
			{
				packet.aInfoList[packet.wCount].bf11SerialInField	=	iSerial;			//	필드안에서 플레이어의 시리얼
				packet.aInfoList[packet.wCount].bf2Team				=	lpActor->m_wTeam;			//	필드안에서 플레이어의 시리얼
				packet.aInfoList[packet.wCount].wX					=	lpActor->m_pos.x;
				packet.aInfoList[packet.wCount].wY					=	lpActor->m_pos.y;
				packet.wCount++;

				iCheckSum	+=	iSerial;
			}
		}

		if (packet.wCount >= dMAX_UPDATE_ACTOR_POS_COUNT)	break;
	}

	iCheckSum	+=	packet.wCount;

	if (packet.wCount)	//	있다!!
	{
		if (_lpPlayer->m_iVisibleActorCheckSum	==	iCheckSum)	//	그런데 좀 전에도 보냈다. -o-
		{
			packet.wCount	=	0;
			return;
		}

		int	iSize	=	sizeof(SG_OUTER_ACTORS_POS_UPDATE)-sizeof(packet.aInfoList)+packet.wCount*sizeof(CMinimumActorInfo);
		packet.base.set(iSize,dSG_OUTER_ACTORS_POS_UPDATE);

		g_userPM.add(_lpPlayer->m_iClientSerial,&packet,packet.base.wSize);
	}

	_lpPlayer->m_iVisibleActorCheckSum	=	iCheckSum;
}

//
//	같은 직업의 수 구하기
int
cFIELD::getSameJobCount(cACTOR *_lpCaster,int _iRange)
{
	int	iSameJobCount=0,iTeam = _lpCaster->m_wTeam;
	int	iActorCount	=	m_aiActorCount[iTeam];

	for (;iActorCount;--iActorCount)		//	적 캐릭터
	{
		cACTOR *lpActor	=	getActor(m_awActorList[iTeam][iActorCount-1]);

		if (!lpActor || lpActor->isDeath() || lpActor->m_wSerialInField == _lpCaster->m_wSerialInField)
			continue;

		if (lpActor->m_wJob	==	_lpCaster->m_wJob)
			if (lpActor->isTargetInAttackRange(_lpCaster,_iRange))	
				iSameJobCount++;
	}

	return	iSameJobCount;
}

BOOL
cFIELD::changeActorsPos(int _iArea1,int _iArea2,int _iTarget,int _iWhere,cACTOR	*_lpAsker)
{
	cAreaInfo	*lpSrcArea	=	m_pArea->getArea(_iArea1);
	cAreaInfo	*lpDestArea	=	m_pArea->getArea(_iArea2);

	if	(!lpSrcArea || !lpDestArea)
		return	FALSE;

	int	i;

	if	(_iTarget	==	0)	//	플레이어
	{
		if	(_lpAsker	&&	lpSrcArea->isIn(_lpAsker->m_pos.x,_lpAsker->m_pos.y))
		{
			cPOINT	pos;

			lpDestArea->getRandomPos(&pos);

			_lpAsker->changePos(pos.x,pos.y,TRUE);	//	위치 바꾸고 본인과 다른이들한테 메시지 전송
		}
	}
	else
	if	(_iTarget	==	1)	//	NPC
	{
		for (i=m_aiActorCount[2];i;i--)
		{
			cACTOR *lpActor	=	getActor(m_awActorList[2][i-1]);

			if (!lpActor)
				continue;

			if (!lpSrcArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			cPOINT	pos;

			lpDestArea->getRandomPos(&pos);

			lpActor->changePos(pos.x,pos.y,TRUE);	//	위치 바꾸고 본인과 다른이들한테 메시지 전송
		}

	}
	else
	if	(_iTarget	==	2)	//	몬스터
	{
		int	iTeam	=	1+m_bIsGuildBattleField;

		for (i=m_aiActorCount[iTeam];i;i--)
		{
			cACTOR *lpActor	=	getActor(m_awActorList[iTeam][i-1]);

			if (!lpActor)
				continue;

			if (!lpSrcArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
				continue;

			cPOINT	pos;

			lpDestArea->getRandomPos(&pos);

			lpActor->changePos(pos.x,pos.y,TRUE);	//	위치 바꾸고 본인과 다른이들한테 메시지 전송
		}
	}
	else
	if	(_iTarget	==	3)	//	운영자
	{
		for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
			for (i=m_aiActorCount[iTeam];i;i--)
			{
				cACTOR *lpActor	=	getActor(m_awActorList[iTeam][i-1]);

				if (!lpActor || lpActor->m_wOperatorLevel < eAL_TESTER)
					continue;

				if (!lpSrcArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

				cPOINT	pos;

				lpDestArea->getRandomPos(&pos);

				lpActor->changePos(pos.x,pos.y,TRUE);	//	위치 바꾸고 본인과 다른이들한테 메시지 전송
			}
	}
	else
	if	(_iTarget	==	4)	//	운영자를 제외한 모든 플레이어
	{
		for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
			for (i=m_aiActorCount[iTeam];i;i--)
			{
				cACTOR *lpActor	=	getActor(m_awActorList[iTeam][i-1]);

				if (!lpActor || lpActor->m_wOperatorLevel >= eAL_TESTER)
					continue;

				if (!lpSrcArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

				cPOINT	pos;

				lpDestArea->getRandomPos(&pos);

				lpActor->changePos(pos.x,pos.y,TRUE);	//	위치 바꾸고 본인과 다른이들한테 메시지 전송
			}
	}
	else
	if	(_iTarget	==	5)	//	모든 플레이어
	{
		for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
			for (i=m_aiActorCount[iTeam];i;i--)
			{
				cACTOR *lpActor	=	getActor(m_awActorList[iTeam][i-1]);

				if (!lpActor)
					continue;

				if (!lpSrcArea->isIn(lpActor->m_pos.x,lpActor->m_pos.y))
					continue;

				cPOINT	pos;

				lpDestArea->getRandomPos(&pos);

				lpActor->changePos(pos.x,pos.y,TRUE);	//	위치 바꾸고 본인과 다른이들한테 메시지 전송
			}
	}

	return	TRUE;
}

BOOL
cFIELD::isPVP(cACTOR *_lpAttacker,cACTOR *_lpTarget)
{
	if	(!_lpTarget)
		return	FALSE;

	if	(m_bIsGuildBattleField	&&	(_lpTarget->isPlayer() || _lpTarget->isDummy()))
		return	TRUE;
	
	return	FALSE;
}

int
cFIELD::getCopyActorCount(cACTOR *_lpCaster)
{
	int	iCount	=	0;
	int	iTeam	=	_lpCaster->m_wTeam;

	for	(int i=m_aiActorCount[iTeam];i;)
	{
		--i;

		int		iSerial		=	m_awActorList[iTeam][i];
		cACTOR	*lpActor	=	getActor(iSerial);

		if	(!lpActor)
			continue;

		if	(lpActor->m_wCopyActorLord	==	_lpCaster->m_wSerialInField	&&	lpActor->m_dwCopyActorLordNameHashCode	==	_lpCaster->m_dwNameHashCode)
			iCount++;
	}

	return	iCount;
}

int
cFIELD::getShutInMagicBoxTargetCount(cACTOR *_lpCaster)
{
	int	iCount	=	0;

	for	(int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
	{
		if	(iTeam	==	_lpCaster->m_wTeam)
			continue;

		for	(int i=m_aiActorCount[iTeam];i;)
		{
			--i;

			int		iSerial		=	m_awActorList[iTeam][i];
			cACTOR	*lpActor	=	getActor(iSerial);

			if	(!lpActor	||	lpActor->m_wInMagicBoxTime	==	0)
				continue;

			if	(lpActor->m_wMagicBoxCasterSerial	==	_lpCaster->m_wSerialInField	&&	lpActor->m_dwMagicBoxCasterNameHashCode	==	_lpCaster->m_dwNameHashCode)
				iCount++;
		}
	}

	return	iCount;
}

int
cFIELD::getIllusionCount(cACTOR *_lpCaster)
{
	int	iCount	=	0;
	int iTeam	=_lpCaster->m_wTeam;

	for	(int i=m_aiActorCount[iTeam];i;)
	{
		--i;

		int		iSerial		=	m_awActorList[iTeam][i];
		cACTOR	*lpActor	=	getActor(iSerial);

		if	(!lpActor || lpActor->m_wIllusionLord	==	0xffff)
			continue;

		if	(lpActor->m_wIllusionLord	==	_lpCaster->m_wSerialInField	&&	lpActor->m_dwDummyLordNameHashCode	==	_lpCaster->m_dwNameHashCode)
			iCount++;
	}

	return	iCount;
}

int
cFIELD::getPOSTargetCount(cACTOR *_lpCaster)
{
	int	iCount	=	0;

	for	(int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
	{
		if	(iTeam	==	_lpCaster->m_wTeam)
			continue;

		for	(int i=m_aiActorCount[iTeam];i;)
		{
			--i;

			int		iSerial		=	m_awActorList[iTeam][i];
			cACTOR	*lpActor	=	getActor(iSerial);

			if	(!lpActor || lpActor->m_wPOS_Time	==	0 || lpActor->isDeath())
				continue;

			if	(lpActor->m_wPOS_Caster	==	_lpCaster->m_wSerialInField	&&	lpActor->m_dwPOS_CasterNameHashCode	==	_lpCaster->m_dwNameHashCode)
				iCount++;
		}
	}

	return	iCount;
}

int
cFIELD::getPetForceList(cACTOR *_lpPet,WORD *_lpwList,int _iSize)
{
	int		iCount		=	0;
	cACTOR	*lpTamer	=	_lpPet->getTamer();

	if	(!lpTamer)
		return	0;

	cParty	*lpParty	=	lpTamer->getParty();

	if	(lpParty)
	{
		for (int i=0;i<lpParty->m_wMemberCount;i++)
		{
			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(i);

			if	(!lpMember	||	!lpMember->isOnSameField(lpTamer))
				continue;

			_lpwList[iCount++]	=	lpMember->m_wSerialInField;

			cACTOR	*lpPet1		=	lpMember->getPetActor(0);
			cACTOR	*lpPet2		=	lpMember->getPetActor(1);

			cACTOR	*lpSB1		=	lpMember->getSummonBeastActor(0);
			cACTOR	*lpSB2		=	lpMember->getSummonBeastActor(1);

			if	(lpPet1)
				_lpwList[iCount++]	=	lpPet1->m_wSerialInField;
			if	(lpPet2)
				_lpwList[iCount++]	=	lpPet2->m_wSerialInField;
			if	(lpSB1)
				_lpwList[iCount++]	=	lpSB1->m_wSerialInField;
			if	(lpSB2)
				_lpwList[iCount++]	=	lpSB2->m_wSerialInField;
		}
	}
	else
	{
		_lpwList[iCount++]	=	lpTamer->m_wSerialInField;

		cACTOR	*lpPet1		=	lpTamer->getPetActor(0);
		cACTOR	*lpPet2		=	lpTamer->getPetActor(1);

		cACTOR	*lpSB1		=	lpTamer->getSummonBeastActor(0);
		cACTOR	*lpSB2		=	lpTamer->getSummonBeastActor(1);

		if	(lpPet1)
			_lpwList[iCount++]	=	lpPet1->m_wSerialInField;
		if	(lpPet2)
			_lpwList[iCount++]	=	lpPet2->m_wSerialInField;
		if	(lpSB1)
			_lpwList[iCount++]	=	lpSB1->m_wSerialInField;
		if	(lpSB2)
			_lpwList[iCount++]	=	lpSB2->m_wSerialInField;
	}

	return	iCount;
}

cNPC*
cFIELD::getValidNpcByJob(int _iJob)
{
	for (int i=dMAX_NPC_COUNT;i;)
	{
		--i;

		cNPC	*lpNpc	=	m_apNpcGenerateData[i];

		if	(!lpNpc	||	lpNpc->m_wActorSerial	!=	0xffff	||	lpNpc->m_bIsSleep)
			continue;

		if	(m_aCharacter[lpNpc->m_wCharacter].m_wBaseJob	!=	_iJob)
			continue;

		return	lpNpc;
	}

	return	NULL;
}

cACTOR*
cFIELD::getActorByBody(int _iBody)
{
	for (int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
		for (int i=m_aiActorCount[iTeam];i;)
		{
			--i;
			
			cACTOR	*lpActor	=	getActor(m_awActorList[iTeam][i]);
			
			if	(!lpActor)
				continue;
			
			if	(g_aJob[lpActor->m_wJobIndex].m_wBody	==	_iBody)
				return	lpActor;
		}
		
	return	NULL;
}

cACTOR*
cFIELD::getActorByJob(int _iJob,int _iIndex)
{
	int	iCount	=	0;

	for (int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
		for (int i=m_aiActorCount[iTeam];i;)
		{
			--i;

			cACTOR	*lpActor	=	getActor(m_awActorList[iTeam][i]);

			if	(!lpActor)
				continue;

			if	(lpActor->m_wJobIndex	==	_iJob)
			{
				if	(iCount	==	_iIndex)
					return	lpActor;

				iCount++;
			}
		}

	return	NULL;
}

int
cFIELD::getMirrorImageActorCount()
{
	int	iCount	=	0;

	for (int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
		for (int i=m_aiActorCount[iTeam];i;)
		{
			--i;

			cACTOR	*lpActor	=	getActor(m_awActorList[iTeam][i]);

			if	(!lpActor	||	lpActor->m_bf1IsMirrorImage	==	FALSE)
				continue;

			iCount++;
		}

	return	iCount;
}

void
cFIELD::changeMonsterInfo(cACTOR *_lpAsker,int _iSerial,int _iJob,int _iLevel)
{
	if	(g_bIsTestServer == FALSE)
		return;
	
	if	(!_lpAsker->isOperator())
		return;

	cACTOR	*lpMonster	=	getActor(_iSerial);

	if	(!lpMonster	||	!lpMonster->isPureMonster())
		return;

	_iJob	-=	100;

	if	(_iJob	<	0	||	_iJob	>=	dMAX_JOB_COUNT)
		return;

	cJOB	*lpJob		=	lpMonster->getJob();
	cJOB	*lpChangeJob=	&g_aJob[_iJob];

	if	(!lpJob	||	lpJob->m_wBody	!=	lpChangeJob->m_wBody	||	lpJob->m_wVariation	!=	lpChangeJob->m_wVariation)
	{
		_lpAsker->sendServerMessage("정보가 잘못 되었습니다.");
		return;
	}

	lpMonster->setDataByJob(lpChangeJob,lpMonster->m_wActorKind,_iLevel);
	lpMonster->addSendSimpleInfo();

	char	strText[512];

	sprintf(strText,"몬스터의 직업과 레벨을 변경했습니다.");

	_lpAsker->sendServerMessage(strText);
}
