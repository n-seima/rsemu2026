#include "CHero.H"
#include "CActor.H"
#include "CMessage.h"

void
CHero::setPetInfo(int _iIndex,cPetDefine *_lpPet,int _iSerial,int _iMinAttackPower,int _iMaxAttackPower,int _iDefensePower)
{
	memcpy(&m_aPet[_iIndex],_lpPet,sizeof(cPetDefine));

	m_aPet[_iIndex].m_wBodySerial		=	_iSerial;
	m_aPet[_iIndex].m_wProtectActor		=	g_lpHero->m_wSerial;
	m_aPet[_iIndex].m_wMinAttackPower	=	_iMinAttackPower;
	m_aPet[_iIndex].m_wMaxAttackPower	=	_iMaxAttackPower;
	m_aPet[_iIndex].m_iDefensePower		=	_iDefensePower;

	CActor	*lpPet	=	getPetActor(_iIndex);

	if (lpPet)
	{
		lpPet->m_wIsPet			=	TRUE;
		lpPet->m_wTamer			=	g_lpHero->m_wSerial;
		lpPet->m_wCommand		=	eTMD_READY;
	}
}

void
CHero::ridingDog(BOOL _bIsRiding,int _iGrade,int _iX,int _iY)
{
	cSummonBeast	*lpBeast	=	getKelby();

	if (!lpBeast)
		return;

	CActor			*lpBeastActor=	g_am.getTestedActor(lpBeast->m_wBodySerial);

	if (_bIsRiding == FALSE)
	{
		g_lpHero->m_wRideDog	=	0xffff;

		if	(lpBeastActor)
			lpBeastActor->m_wIsRideTamer=	FALSE;

		g_lpHero->stop();
		g_lpHero->setAnm(dACT_READY);

		return;
	}

	g_lpHero->m_wRideDog		=	_iGrade;
	g_lpHero->stop(_iX,_iY);
	g_lpHero->setAnm(dACT_READY);

	if (lpBeastActor)
		lpBeastActor->m_wIsRideTamer=	TRUE;
}

//펫과 헤어짐(parting).
void
CHero::partingWithPet(int _iIndex,BOOL _bIsMiniPet,BOOL _bIsRequireMessage)
{
	if	(_bIsMiniPet)
	{
		if	(_bIsRequireMessage)
			g_msgBox.cPopup(NULL,_ms(dMSG_PARTING_WITH_PET_FORM,m_aMiniPet[_iIndex].getName()),dMSG_OK);

		m_aMiniPet[_iIndex].m_bf7Level	=	0;

		if	(g_hero.m_wActiveMiniPet==	_iIndex)
		{
			g_hero.m_wActiveMiniPet	=	c_iMiniPetCount;
			syncHeroApprearance();
		}
		if	(g_hero.m_wActiveMiniPet2==	_iIndex)
		{
			g_hero.m_wActiveMiniPet2	=	c_iMiniPetCount;
			syncHeroApprearance();
		}
	}
	else
	if	(m_aPet[_iIndex].m_bf10Level)
	{
		if	(_bIsRequireMessage)
			g_msgBox.cPopup(NULL,_ms(dMSG_PARTING_WITH_PET_FORM,m_aPet[_iIndex].getName()),dMSG_OK);

		m_aPet[_iIndex].reset();
	}

	buildPower();
}

void
CHero::setSummonBeastInfo(int _iIndex,int _iSerial,int _iLevel,int _iGrade,int _iType)
{
	if	(_iLevel	==	0)
	{
		m_aSummonBeast[_iIndex].reset();

		return;
	}

	m_aSummonBeast[_iIndex].m_bf22CurrentHP		=	0;
	m_aSummonBeast[_iIndex].m_wIndex			=	_iIndex;	
	m_aSummonBeast[_iIndex].m_wBodySerial		=	_iSerial;	
	m_aSummonBeast[_iIndex].m_wEngageTarget		=	0xffff;
	m_aSummonBeast[_iIndex].m_bf10Level			=	_iLevel;
	m_aSummonBeast[_iIndex].m_bf2Grade			=	_iGrade;
	m_aSummonBeast[_iIndex].m_bf2Type			=	_iType;

	if	(g_lpHero->m_wRideDog	!=	0xffff	&&	_iType	==	dSBT_KELBY)
	{
		g_lpHero->m_wRideDog	=	_iGrade;
		g_lpHero->setAnm(dACT_READY);
	}
}

int
CHero::getPetIndex(int _iSerial)
{
	for (int i=0;i<dOWN_PET_COUNT;i++)
	{
		if (m_aPet[i].m_bf10Level == 0)
			continue;
		
		if (m_aPet[i].m_wBodySerial	==	_iSerial)
			return	i;
	}

	return	0xffff;
}


CActor*
CHero::getPetActor(int _iIndex)
{
	if (_iIndex	>=	dOWN_PET_COUNT	||	m_aPet[_iIndex].m_bf10Level == 0)
		return	NULL;

	return	g_am.getTestedActor(m_aPet[_iIndex].m_wBodySerial);
}

cPet*
CHero::getPet(int _iIndex)
{
	if (_iIndex	>=	dOWN_PET_COUNT	||	m_aPet[_iIndex].m_bf10Level == 0)
		return	NULL;

	return	&m_aPet[_iIndex];
}

cSummonBeast*
CHero::getSummonBeast(int _iIndex)
{
	if (_iIndex	>=	dOWN_SUMMON_BEAST_COUNT ||	m_aSummonBeast[_iIndex].m_bf10Level == 0)
		return	NULL;

	return	&m_aSummonBeast[_iIndex];
}

cSummonBeast*
CHero::getKelby()
{
	for (int i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		if (m_aSummonBeast[i].m_bf10Level == 0)
			continue;
		
		if (m_aSummonBeast[i].m_bf2Type	==	dSBT_KELBY)
			return	&m_aSummonBeast[i];
	}

	return	NULL;
}

cSummonBeast*
CHero::getSummonBeastByType(int _iType,int _iLimitGrade)
{
	for (int i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		if (m_aSummonBeast[i].m_bf10Level == 0)
			continue;

		if (m_aSummonBeast[i].m_bf2Type	==	_iType)
		{
			if (_iLimitGrade	!=	-1)
			{
				if (m_aSummonBeast[i].m_bf2Grade	<	_iLimitGrade-1)
					return	NULL;
			}

			return	&m_aSummonBeast[i];
		}
	}

	return	NULL;
}

CActor*
CHero::getSummonBeastActor(int _iIndex)
{
	if (_iIndex	>=	dOWN_SUMMON_BEAST_COUNT ||	m_aSummonBeast[_iIndex].m_bf10Level == 0)
		return	NULL;

	return	g_am.getTestedActor(m_aSummonBeast[_iIndex].m_wBodySerial);
}

int
CHero::getSummonBeastIndex(int _iSerial)
{
	for (int i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		if (m_aSummonBeast[i].m_bf10Level == 0)
			continue;
		
		if (m_aSummonBeast[i].m_wBodySerial	==	_iSerial)
			return	i;
	}

	return	0xffff;
}

void
CHero::changeCommand(int _iPetIndex,int _iCommand,int _iX,int _iY,int _iProtectActor)
{
	m_aPet[_iPetIndex].m_wCommand		=	_iCommand;
	m_aPet[_iPetIndex].m_posProtect.x	=	_iX;
	m_aPet[_iPetIndex].m_posProtect.y	=	_iY;

	if (_iProtectActor	!=	1023)
		m_aPet[_iPetIndex].m_wProtectActor	=	_iProtectActor;

	CActor	*lpPet	=	getPetActor(_iPetIndex);

	m_dwLastReceiveCommandTime	=	timeGetTime();

	if	(!lpPet)
		return;

	lpPet->m_wCommand		=	_iCommand;
	lpPet->m_wPetWaitCommand=	FALSE;
	lpPet->m_wTalkTime		=	0;
}

void
CHero::syncPetInfo(int _iPetIndex,int _iRemainHP,int _iMaxHP,int _iMaxAttackPower,int _iMinAttackPower,int _iDefensePower,int _iStrength,int _iIntelligence,BOOL _bIsSummonBeast)
{	
	CActor	*lpActor;
	
	if (_bIsSummonBeast	==	FALSE)
		lpActor	=	getPetActor(_iPetIndex);
	else
		lpActor	=	getSummonBeastActor(_iPetIndex);

	if (lpActor)
	{
		lpActor->m_iMaxHP		=	_iMaxHP*100;
		LONGLONG	llHP		=	_iMaxHP*100;
		llHP					=	llHP*_iRemainHP/10000;
		lpActor->m_iHP			=	(int)llHP;
	}

	if (_bIsSummonBeast)
	{
		m_aSummonBeast[_iPetIndex].m_wMaxAttackPower	=	_iMaxAttackPower;
		m_aSummonBeast[_iPetIndex].m_wMinAttackPower	=	_iMinAttackPower;
		m_aSummonBeast[_iPetIndex].m_wDefensePower		=	_iDefensePower;
		m_aSummonBeast[_iPetIndex].m_wStrength			=	_iStrength;
		m_aSummonBeast[_iPetIndex].m_wIntelligence		=	_iIntelligence;
	}
	else
	{
		m_aPet[_iPetIndex].m_wMaxAttackPower	=	_iMaxAttackPower;
		m_aPet[_iPetIndex].m_wMinAttackPower	=	_iMinAttackPower;
		m_aPet[_iPetIndex].m_iDefensePower		=	_iDefensePower;
	}
}

void
CHero::petWaitCommand(int _iPetIndex)
{
	CActor	*lpPet	=	getPetActor(_iPetIndex);

	if (lpPet)
		lpPet->m_wPetWaitCommand	=	1;
}

void
CHero::makeBreedingRecordBook(int _iPetIndex)
{
	AddSystemMessage(LTGREEN,dMSG_MAKE_BREEDING_RECORD_BOOK);

	m_aPet[_iPetIndex].reset();
}

BOOL
CHero::isPet(int _iSerial)
{
	if (getPetIndex(_iSerial)!=0xffff)
		return	TRUE;

	return	FALSE;
}

BOOL
CHero::isSummonBeastActor(int _iSerial,int _iType)
{
	int	iIndex	=	getSummonBeastIndex(_iSerial);

	if (iIndex==0xffff)
		return	FALSE;

	if (_iType	!=	0xffff)
		if (m_aSummonBeast[iIndex].m_bf2Type	!=	_iType)
			return	FALSE;

	return	TRUE;
}


int
CHero::getPetCount()
{
	int	iPetCount	=	0;

	for (int i=0;i<dOWN_PET_COUNT;i++)
	{
		if (m_aPet[i].m_bf10Level)
			iPetCount++;
	}

	return	iPetCount;
}

int
CHero::getSummonBeastCount()
{
	int	iCount	=	0;

	for (int i=0;i<dOWN_SUMMON_BEAST_COUNT;i++)
	{
		if (m_aSummonBeast[i].m_bf10Level)
			iCount++;
	}

	return	iCount;
}

void
CHero::changePetName(int _iPetIndex,char *_lpstrName,BOOL _bIsMiniPet,int _iResetItemIndex)
{
	if	(_bIsMiniPet)
		memcpy(m_aMiniPet[_iPetIndex].m_strName,_lpstrName,dPET_NAME_LENGTH);
	else
	{
		memcpy(m_aPet[_iPetIndex].m_strName,_lpstrName,dPET_NAME_LENGTH);

		if	(_iResetItemIndex	!=	-1)	//	아이템을 이용해 이름을 초기하 시킨거다. 
		{
			cItem	*lpItem	=	getInventoryItem(_iResetItemIndex);

			if	(lpItem)
				lpItem->decreaseCount(1);

		}

		if	(STRCMP(_lpstrName,"noname")==0)
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_RESETED_PET_NAME);
		else
			CGamePlay::AddSystemMessage(LTYELLOW,dMSG_CAHNGED_PET_NAME);
	}
}