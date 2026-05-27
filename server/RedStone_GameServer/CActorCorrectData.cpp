#include "cACTOR.H"
#include "cFIELD.H"
#include "debugCode.H"
#include "cJOB.H"
#include "rebirth_define.H"

//
//	현재 스킬포인트의 합
int
cACTOR::getCurrentStatePointSum()
{
	int	iSum	=	m_sStrength+m_sAgility+m_sConstitution+m_sWisdom+m_sIntelligence+m_sCharisma+m_sLuck+m_wLevelPoint-90;

	return	iSum;
}

//
//	현재 스킬포인트의 합
int
cACTOR::getCorrectStatePointSum()
{
	int	iSum	=	(m_iLevel-1)*5;
	
	iSum		+=	m_bf4CH5StateBonusCount*5;
	iSum		+=	c_aBonusStatePointForRebirth[m_bf4RebirthCount];

	return	iSum;
}

//
//	데이터 체크
int
cACTOR::checkData(BOOL _bIsCheckSerial)
{
	if	(_bIsCheckSerial && isDummy())
		return	eCADR_IS_NPC2;

	int		i;

	if	(_bIsCheckSerial)
	{
		if	(m_wSerialInField	==	0xffff		)
			return	eCADR_VALID_SERIAL;

		if	(isNpc()							)
			return	eCADR_IS_NPC;
	}

	if	(strlen(m_strId)		<	4		)
		return	eCADR_IS_NPC2;
	if	(STRICMP(m_strId,"NPC")	==	0		)
		return	eCADR_IS_NPC2;
	if	(m_wJob					>=	dPLAYER_JOB_COUNT		)
		return	eCADR_INCORRECT_JOB;
	if	(m_iLevel				>	1000	)
		return	eCADR_INCORRECT_LEVEL;

	if	(m_wJob	!=	dJOB_SUMMONER	&&	m_wJob	!=	dJOB_BEAST_TAMER	&&	m_wJob	!=	dJOB_NECROMANCER	&&	m_wJob	!=	dJOB_DEVIL)
	{
		m_aPet[0].reset();
		m_aPet[1].reset();
	}

#ifndef	_FOR_KOREA
	if	(getCurrentStatePointSum()	!=	getCorrectStatePointSum())
		return	eCADR_INCORRECT_STATE_POINT;
#endif
	{
		CJobBasicDataDefine	*lpJobData	=	&g_aPlayerJobBaseInfo[m_wJob/2];
		
		m_iMaxCP	=	lpJobData->m_iMaxCP;

		if	(m_iMaxCP		<	lpJobData->m_iMaxCP				||
			m_iMaxHP		<	lpJobData->m_iMaxHP				||
			m_sStrength		<	lpJobData->m_sStrength			||		
			m_sAgility		<	lpJobData->m_sAgility			||		
			m_sConstitution	<	lpJobData->m_sConstitution		||	
			m_sWisdom		<	lpJobData->m_sWisdom			||
			m_sIntelligence	<	lpJobData->m_sIntelligence		||	
			m_sCharisma		<	lpJobData->m_sCharisma			||
			m_sLuck			<	lpJobData->m_sLuck				||			
			m_sFireResistance<	lpJobData->m_sFireResistance	||			
			m_sWaterResistance<	lpJobData->m_sWaterResistance	||			
			m_sWindResistance<	lpJobData->m_sWindResistance	||			
			m_sEarthResistance<	lpJobData->m_sEarthResistance	||			
			m_sLightResistance<	lpJobData->m_sLightResistance	||			
			m_sDarkResistance<	lpJobData->m_sDarkResistance	)
			return	eCADR_INCORRECT_STATE_POINT;
	}

	int	iAnotherJob	=	m_wJob;

	if	(m_wJob%2)
		iAnotherJob--;
	else
		iAnotherJob++;

	for (i=0;i<dABILITY_COUNT;i++)
	{
		cAbility	*lpAbility	=	&m_aAbility[i];

		if	(lpAbility->m_wSkill	==	0xffff				)
			continue;
		if	(lpAbility->m_wSkill	>	dMAX_SKILL			)
			return	eCADR_INCORRECT_SKILL_DATA;
		if	(lpAbility->m_wLevel	>	dLIMIT_SKILL_LEVEL	)
			return	eCADR_INCORRECT_SKILL_DATA;

		CSkill		*lpSkill	=	lpAbility->getSkill();

		if	(!lpSkill || (!lpSkill->isEnableJob(m_wJob) && !lpSkill->isEnableJob(iAnotherJob)))
			return	eCADR_MISMATCH_SKILL_AND_JOB;
	}

/*	if (!_bIsCheckSerial)
	{
		int	iSkillSum			=	getSkillPointSum();
		int	iSkillSumByLevel	=	getSkillPointSumByLevel();

		if (iSkillSum	!=	iSkillSumByLevel)
		{
//			LOG("cACTOR::checkData 레벨이 이상해요 [%s/%s] [%d:%d]",m_strId,m_strName,iSkillSum,iSkillSumByLevel);
//			return	eCADR_INCORRECT_LEVEL;
		}
	}
*/
	return	eCADR_ISOK;
}

int
cACTOR::correctSkill()
{
	cACTOR	temp;
	temp.initializeJobData("aaa","aaaa",m_wJob);
				
	for (int i=0;i<dABILITY_COUNT;i++)	
	{
		m_aAbility[i].m_wSkill = temp.m_aAbility[i].m_wSkill;
	}

	return TRUE;
}

//
//	체크
int
cACTOR::checkData(CClient *_lpClient,char *_lpstrWhere,BOOL _bIsCheckSerial)
{
	if	(_bIsCheckSerial	&&	isDummy())
		return	eCADR_IS_NPC;

	int	iResult		=	eCADR_ISOK;
	int	iRookieTime	=	-1;

	if (_lpClient)
	{
		iRookieTime	=	m_iRookieTime;

		if (_bIsCheckSerial && _lpClient->GetActorSerialInServer() != (DWORD)m_iZoneSerial)	//	시리얼 이상
		{
			CLOG("check","%s - _lpClient와 시리얼이 달라요 [%s/%s][%s/%s]",_lpstrWhere,m_strId,m_strName,_lpClient->GetID(),_lpClient->GetName());
//			printf("-------- %s - _lpClient와 시리얼이 달라요 [%s/%s][%s/%s]\n",_lpstrWhere,m_strId,m_strName,_lpClient->GetID(),_lpClient->GetName());

			return	eCADR_VALID_SERIAL;
		}

		if (STRCMP(_lpClient->GetName()	,m_strName)	!=	0			)	//	아이디 이상
		{
			CLOG("check","%s - 이름이 달라효 %s/%s",_lpstrWhere,_lpClient->GetName(),m_strName);
//			printf("-------- %s - 이름이 달라효 %s/%s\n",_lpstrWhere,_lpClient->GetName(),m_strName);

			return	eCADR_MISMATCH_NAME_BY_CLIENT;
		}
		if (STRICMP(_lpClient->GetID()	,m_strId)		!=	0			)					//	캐릭터 이름 이상
		{
			CLOG("check","%s - 아이디가 달라효 %s/%s",_lpstrWhere,_lpClient->GetID(),m_strId);
//			printf("-------- %s - 아이디가 달라효 %s/%s\n",_lpstrWhere,_lpClient->GetID(),m_strId);

			return	eCADR_MISMATCH_ID_BY_CLIENT;
		}
	}

	iResult		=	checkData(_bIsCheckSerial);
	
	switch(iResult)
	{
		case	eCADR_IS_NPC					:
			CLOG("actor data check","%s[%d] - NPC라는데요? -_-;;;;;;",_lpstrWhere,iRookieTime);
			break;

		case	eCADR_IS_NPC2					:
			CLOG("actor data check","%s[%d] - NPC2라는데요? -_-;;;;;;",_lpstrWhere,iRookieTime);
			break;

		case	eCADR_VALID_SERIAL				:
			CLOG("actor data check","%s - 직업 이상 - [%s/%s/%d] %d",_lpstrWhere,m_strId,m_strName,m_wJob,iRookieTime);
			break;

		case	eCADR_INCORRECT_JOB				:
			CLOG("actor data check","%s - 직업 이상 - [%s/%s/%d] %d",_lpstrWhere,m_strId,m_strName,m_wJob,iRookieTime);
//			printf("-------- %s - 직업 이상 - [%s/%s/%d]\n",_lpstrWhere,m_strId,m_strName,m_wJob);
			if	(_lpClient)
				disconnect();
			return	FALSE;

		case	eCADR_INCORRECT_PET_INFO			:
			CLOG("actor data check","%s - 펫 정보 이상 - [%s/%s/%d] %d",_lpstrWhere,m_strId,m_strName,m_iLevel,iRookieTime);
//			printf("-------- %s - 레벨 이상 - [%s/%s/%d]\n",_lpstrWhere,m_strId,m_strName,m_iLevel);
			break;

		case	eCADR_INCORRECT_LEVEL			:
			CLOG("actor data check","%s - 레벨 이상 - [%s/%s/%d] %d",_lpstrWhere,m_strId,m_strName,m_iLevel,iRookieTime);
//			printf("-------- %s - 레벨 이상 - [%s/%s/%d]\n",_lpstrWhere,m_strId,m_strName,m_iLevel);
			break;

		case	eCADR_INCORRECT_SKILL_DATA		:
			CLOG("actor data check","%s - 스킬 데이터 이상 - [%s/%s] %d",_lpstrWhere,m_strId,m_strName,iRookieTime);
//			printf("-------- %s - 스킬 데이터 이상 - [%s/%s]\n",_lpstrWhere,m_strId,m_strName);
			break;

		case	eCADR_MISMATCH_SKILL_AND_JOB	:
			CLOG("actor data check","%s - 직업과 스킬이 안맞음 - [%s/%s] %d",_lpstrWhere,m_strId,m_strName,iRookieTime);
//			printf("-------- %s - 직업과 스킬이 안맞음 - [%s/%s]\n",_lpstrWhere,m_strId,m_strName);
			break;

		case	eCADR_INCORRECT_STATE_POINT		:
			CLOG("actor data check","%s - 스탯 포인트 안 맞음 - [%s/%s] %d",_lpstrWhere,m_strId,m_strName,iRookieTime);
//			printf("-------- %s - incorrect state point [%s/%s]\n",_lpstrWhere,m_strId,m_strName);
			break;
	}
	
	return	iResult;
}
