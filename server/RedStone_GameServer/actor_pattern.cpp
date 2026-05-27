#include "cACTOR.H"
#include "pattern.H"
#include "cFIELD.H"
#include "cGAME.H"

BOOL
cACTOR::operatePattern(int _iPattern)
{
	if	(m_wProcessPatternLevel	!=	0xffff	&&	m_wProcessPatternLevel	>=	eProcessCharacterPatternCount)
	{
		ERRMSG("경고!!","패턴 오버!!");

		return	FALSE;
	}

	if	(m_wProcessPatternLevel	==	0xffff)
		m_wProcessPatternLevel	=	0;
	else
		m_wProcessPatternLevel++;

	cProcessPatternInfo		*lpProcesPattern	=	&m_aProcessPattern[m_wProcessPatternLevel];

	lpProcesPattern->m_wStep	=	0;
	lpProcesPattern->m_wPattern	=	_iPattern;

	cCharacterPattern		*lpPattern			=	m_lpField->getCharacterPattern(_iPattern);

	if	(!lpPattern)
	{
		ERRMSG("경고!!","없는 패턴 호출됨!!");
		return	FALSE;
	}

	processPattern();

	return	TRUE;
}

void
cACTOR::operateNextStepPattern()
{
	cProcessPatternInfo		*lpProcesPattern	=	&m_aProcessPattern[m_wProcessPatternLevel];
	cCharacterPattern		*lpPattern			=	m_lpField->getCharacterPattern(lpProcesPattern->m_wPattern);

	lpProcesPattern->m_wStep++;

	if	(lpProcesPattern->m_wStep	>=	lpPattern->m_wCount)
	{
		if	(m_wProcessPatternLevel	==	0)
		{
			printf("end pattern\n");
			m_wProcessPatternLevel	=	0xffff;
			m_wProcessPatternStatus	=	0xffff;

			stopBattleForNPC(TRUE);
			setAiReady();

			return;
		}

		m_wProcessPatternLevel--;
		operateNextStepPattern();
		return;
	}

	processPattern();
}

void
cACTOR::processPattern()
{
	cProcessPatternInfo		*lpProcesPattern	=	&m_aProcessPattern[m_wProcessPatternLevel];
	cCharacterPattern		*lpPattern			=	m_lpField->getCharacterPattern(lpProcesPattern->m_wPattern);
	cCharacterPatternPiece	*lpPP				=	&lpPattern->m_pPattern[lpProcesPattern->m_wStep];

	printf("process level %d-%d\n",m_wProcessPatternLevel,lpProcesPattern->m_wStep);

	switch(lpPP->m_wPattern)
	{
		case	eCP_move			:
			pcp_move(lpPP);
			break;
		case	eCP_delay_time		:
			pcp_delay_time(lpPP);
			break;
		case	eCP_use_action		:
			pcp_use_action(lpPP);
			break;
		case	eCP_use_skill		:
			pcp_use_skill(lpPP);
			break;
		case	eCP_use_local_skill	:
			pcp_use_local_skill(lpPP);
			break;
		case	eCP_set_invincible	:
			pcp_set_invicible(lpPP);
			break;
		case	eCP_call_pattern	:
			pcp_call_pattern(lpPP);
			break;
	}
}

void
cACTOR::pcp_move(cCharacterPatternPiece	*_lpPP)
{
	stopBattleForNPC(TRUE);

	cPOINT	posDest;

	posDest.x	=	_lpPP->m_aiValue[0]*dTILE_XS+dTILE_XS/2;
	posDest.y	=	_lpPP->m_aiValue[1]*dTILE_YS+dTILE_YS/2;

	moveTo(posDest.x,posDest.y);

	setAiMove();

	m_wProcessPatternStatus	=	eCP_move;
}

void
cACTOR::pcp_delay_time(cCharacterPatternPiece	*_lpPP)
{
	stopBattleForNPC(FALSE);
	setAiReady();
	setAiState(eAS_NONE);

	int	iDelayTime				=	_lpPP->m_aiValue[0];

	m_wProcessPatternStatus		=	eCP_delay_time;

	m_dwBookedActionFinishTime	=	iDelayTime/dREVISE_ACTION_SPEED_FOR_TEST+g_dwCurrentTime;
	m_isAction					=	TRUE;
}

void
cACTOR::pcp_use_action(cCharacterPatternPiece	*_lpPP)
{
	stopBattleForNPC(FALSE);
	setAiReady();
	setAiState(eAS_NONE);

	int	iAction			=	_lpPP->m_aiValue[0];
	int	iDirect			=	_lpPP->m_aiValue[1];
	int	iActionCount	=	_lpPP->m_aiValue[2];
	int	iFPS			=	_lpPP->m_aiValue[3];

	if	(m_wProcessPatternStatus!=	eCP_use_action)
		m_awProcessValue[0]		=	iActionCount;

	BOOL	bIsChangeFPS=	FALSE;

	if	(iFPS			!=	-1)
	{
		m_iFPS			=	iFPS;
		bIsChangeFPS	=	TRUE;
	}

	action(iAction,bIsChangeFPS);
	setExclusiveAction(TRUE);
	bookingActionFinishTime();

	m_lpField->addSendEtcWork(m_wSerialInField,eEW_USE_ACTION,m_wSerialInField,m_pos.x,m_pos.y,iAction,iDirect,m_iFPS);

	m_wProcessPatternStatus	=	eCP_use_action;
}

void
cACTOR::pcp_use_local_skill(cCharacterPatternPiece	*_lpPP)
{
	int	iSkill	=	_lpPP->m_aiValue[0];
	int	iX1		=	_lpPP->m_aiValue[1];
	int	iY1		=	_lpPP->m_aiValue[2];
	int	iX2		=	_lpPP->m_aiValue[3];
	int	iY2		=	_lpPP->m_aiValue[4];

	cAbility	ability;

	cAbility	*lpAbility	=	getAbilityBySkill(iSkill);//_iSkill인 어빌러티를 찾는다.

	if	(!lpAbility)
	{
		operateNextStepPattern();
		return;
	}

	memcpy(&ability,lpAbility,sizeof(cAbility));

	setLastUseSkill(ability.m_wSkill);

	m_wUseSkill				=	m_lpField->m_skillManager.castFireWall(this,&ability,iX1,iY1,iX2,iY2);
	m_wProcessPatternStatus	=	eCP_use_local_skill;

	m_lpField->sendActionQuick(this,&ability);

	setActionSpeed(&ability);	//fps와 현재 액션이 끝나는데 필요한 프레임을 얻어서 설정한다.
	decreaseBullet(&ability);

	setCoolTime(&ability);
}

void
cACTOR::pcp_use_skill(cCharacterPatternPiece	*_lpPP)
{
	int	iSkill	=	_lpPP->m_aiValue[0];
	int	iLevel	=	_lpPP->m_aiValue[1];

	cAbility	ability;

	ability.m_wSkill	=	iSkill;
	ability.m_wLevel	=	iLevel;

	if	(iLevel	==	0)
	{
		cAbility	*lpAbility	=	getAbilityBySkill(iSkill);//_iSkill인 어빌러티를 찾는다.

		if	(!lpAbility)
		{
			operateNextStepPattern();
			return;
		}

		ability.m_wLevel	=	lpAbility->m_wLevel;
	}

	stop(TRUE);

	setEngageAbility(&ability);

	int	iAttackResult	=	attackToActor(m_lpTarget,&m_engageAbility);

	if	((iAttackResult > eAR_OK && iAttackResult < eAR_IS_INREGULAR_SKILL) || iAttackResult == eAR_NOT_LEARNED_SKILL)
	{
		operateNextStepPattern();
		return;
	}

	m_lpField->sendAttackToActorResult(iAttackResult,this,m_lpTarget,&m_engageAbility);	//	C2S

	m_wProcessPatternStatus	=	eCP_use_skill;

	setAiState(eAS_FIGHT);
}

void
cACTOR::pcp_set_invicible(cCharacterPatternPiece	*_lpPP)
{
	int	iInvincible	=	_lpPP->m_aiValue[0];

	if	(iInvincible	==	0)
		m_wInvincibleTime	=	0xffff;
	else
		m_wInvincibleTime	=	0;

	operateNextStepPattern();
}

void
cACTOR::pcp_call_pattern(cCharacterPatternPiece	*_lpPP)
{
	int	iChance			=	_lpPP->m_aiValue[0];
	int	iPattern		=	_lpPP->m_aiValue[1];

	if	(random(100)	>	iChance)
	{
		operateNextStepPattern();
		return;
	}

	operatePattern(iPattern);
}