#include	"CActiveSkill.H"
#include	"cACTOR.H"
#include	"CField.H"
#include	"CGame.H"
#include	"ast_mirror_image.h"

BOOL
cAS_MirrorImage::fire()
{
	m_wMirrorJob			=	m_lpSkill->m_aValue[0];
	m_wGeneratePeriod		=	m_lpSkill->m_aValue[1];
	m_wAwakenTime			=	m_lpSkill->m_aValue[2];
	m_wEnchantSkill			=	m_lpSkill->m_aValue[3];
	m_wImageCount			=	max(m_ability.getBulletCount(),1);
	m_wGeneratedImageCount	=	0;

	m_lpCaster->action(m_lpSkill->m_wAction);
	m_lpCaster->m_bf1IsBlockToRevenge	=	TRUE;
	m_lpCaster->setAiState(eAS_NONE);
	m_lpCaster->m_wInvincibleTime		=	0xffff;

	getActionTime();

	m_wGrade				=	eCasting;

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
cAS_MirrorImage::casting()
{
	m_wGrade						=	eGenerate;

	cFIELD	*lpField				=	m_lpCaster->m_lpField;

	cACTOR	*lpTarget				=	lpField->getActorByJob(m_wMirrorJob);

	if	(!lpTarget)
		return	FALSE;

	m_wMasterNpcSerial				=	lpTarget->getNpc()->m_iSerial;

	lpTarget->stopBattleForNPC(TRUE);
	lpTarget->setAiReady();
	lpTarget->setAiState(eAS_NONE);
	lpTarget->m_wInvincibleTime			=	0;

	lpTarget->cureWrongStatus();
	lpTarget->m_iHP						=	lpTarget->getMaxHP();
	lpTarget->m_bf1IsBlockToRevenge		=	TRUE;

	m_lpImageTarget						=	lpTarget;

	return	TRUE;
}

void
cAS_MirrorImage::actionNow()
{
	if	(m_lpCaster	==	NULL	||	m_lpCaster->m_wSerialInField	==	0xffff)
		return;

	m_lpCaster->m_wMirrorImageMasterNpcSerial	=	m_wMasterNpcSerial;

	for	(int i=0;i<m_wImageCount;i++)
	{
		cACTOR		*lpTarget	=	m_lpCaster->m_lpField->getActorByJob(m_wMirrorJob,i);

		if	(lpTarget	==	NULL	||	lpTarget->isDeath())
			continue;

		cAbility	*lpAbility	=	lpTarget->getAbilityBySkill(m_wEnchantSkill);

		if	(lpAbility)
			lpTarget->attackToActor(lpTarget,lpAbility);

		lpTarget->m_bf1IsBlockToRevenge		=	FALSE;
		lpTarget->m_wBattleInclination		=	dBI_AGGRESSIVE_SOLO;
		lpTarget->setAiState(eAS_SAGE_MASTER_TRACE);
		lpTarget->m_iReaminSerchEnemyTime	=	dSYNC_FPS;	//	즉시 적을 찾는다. 스킬 사용후 약간의 시간
	}
}

BOOL
cAS_MirrorImage::update()
{
	if	(m_wGrade	==	eCasting)
	{
		if	(!isActionTime())
			return	FALSE;	//	아직 발동 직전이다.

		if	(!casting())
			return	TRUE;

		return	FALSE;
	}

	if	(m_wGrade	==	eWaitNextTime)
	{
		if	(!isActionTime())
			return	FALSE;	//	아직 발동 직전이다.

		m_wGrade						=	eGenerate;
	}

	if	(m_wGrade	==	eAwakenTime)
	{
		if	(!isActionTime())
			return	FALSE;

		actionNow();

		return	TRUE;
	}

	if	(m_wGrade	==	eGenerate)
	{
		if	(m_lpImageTarget	==	NULL	||	m_lpImageTarget->m_wSerialInField	==	0xffff	||	m_lpImageTarget->isDeath())
		{
			m_wGrade				=	eAwakenTime;
			m_dwBookedActionTime	=	g_dwCurrentTime	+	m_wAwakenTime*10/dREVISE_ACTION_SPEED_FOR_TEST;
			return	FALSE;
		}

		int		iAngle					=	(m_wGeneratedImageCount*360/m_wImageCount)%360;
		int		iDistance				=	m_lpImageTarget->getBodySize()*3;

		cACTOR	*lpTarget				=	NULL;

		if	(m_wGeneratedImageCount+1	==	m_wImageCount)
			lpTarget			=	m_lpImageTarget;
		else
		{
			cFIELD	*lpField	=	m_lpImageTarget->m_lpField;
			cNPC	*lpNpc		=	lpField->getValidNpcByJob(m_wMirrorJob);

			if	(!lpNpc)
			{
				m_wImageCount		=	m_wGeneratedImageCount;
				m_dwBookedActionTime=	g_dwCurrentTime	+	m_wAwakenTime*10/dREVISE_ACTION_SPEED_FOR_TEST;
				m_wGrade			=	eAwakenTime;
				return	FALSE;
			}

			lpTarget			=	lpField->generateNpc(lpNpc,FALSE,FALSE,m_lpImageTarget->m_pos.x,m_lpImageTarget->m_pos.y);

			if	(!lpTarget)
			{
				m_wImageCount		=	m_wGeneratedImageCount;
				m_wGrade			=	eAwakenTime;
				m_dwBookedActionTime=	g_dwCurrentTime	+	m_wAwakenTime*10/dREVISE_ACTION_SPEED_FOR_TEST;
				return	FALSE;
			}
		}

		cPOINT	movePos;

		movePos.x	=	m_lpImageTarget->m_pos.x;
		movePos.y	=	m_lpImageTarget->m_pos.y;

		g_protractor.getAnglePos(&movePos,iAngle,iDistance);

		lpTarget->moveTo(movePos.x,movePos.y);
		lpTarget->setAiMove();
		lpTarget->m_bf1IsMirrorImage		=	TRUE;
		lpTarget->m_bf1IsBlockToRevenge		=	TRUE;		//	반격 안하게
		lpTarget->m_wInvincibleTime			=	0;

		m_wGeneratedImageCount++;

		if	(m_wGeneratedImageCount	>=	m_wImageCount)
		{
			m_wGrade				=	eAwakenTime;
			m_dwBookedActionTime	=	g_dwCurrentTime	+	m_wAwakenTime*10/dREVISE_ACTION_SPEED_FOR_TEST;
			return	FALSE;
		}

		m_dwBookedActionTime		=	g_dwCurrentTime	+	m_wGeneratePeriod*10/dREVISE_ACTION_SPEED_FOR_TEST;

		m_wGrade	=	eWaitNextTime;

		return	FALSE;
	}

	return	TRUE;
}