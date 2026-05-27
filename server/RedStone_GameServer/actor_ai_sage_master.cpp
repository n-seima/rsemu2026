#include "cACTOR.H"
#include "cFIELD.H"
#include "cNPC.H"
#include "debugCode.H"
#include "CGame.H"

BOOL
cACTOR::operateSageMasterIronBall()
{
	if	(m_lpTarget	==	NULL	||	m_lpTarget->isDeath())
	{
		cACTOR	*lpTarget	=	m_lpField->findBestFarEnemy(m_wTeam,m_iLocalPart,m_pos.x,m_pos.y,3000,this);

		if	(!lpTarget)
			return	TRUE;

		setTarget(lpTarget);
	}

	if	(m_bIsMoving)
	{
		if	(isTargetInAttackRange(m_lpTarget,0))
		{
			stop();
			addPacketStop();

			setTarget(NULL);

			return	TRUE;
		}

		if	(m_dwLastMoveMessageTime	+	dMOVE_UPDATE_TIME	<=	(DWORD)m_lpField->m_iFrameCounter)
		{
			m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

			addNpcMoveMessage(TRUE);
		}
	}
	else
	{
		if	(isTargetInAttackRange(m_lpTarget,0))
		{
			setTarget(NULL);

			return	TRUE;
		}

		sageMasterPursuit();
	}

//	주기적으로 타겟의 위치를 체크하고, 이동했다면 위치를 다시 잡는다.
	if	(m_wSerchEnemyTime	>=	dAPPROACH_TARGET_REFRESH_TIME/2)
	{
		m_wSerchEnemyTime	=	0;

		if	(m_lpTarget->m_pos.x	!=	m_posTarget.x	||	m_lpTarget->m_pos.y	!=	m_posTarget.y)	//	이동 했다.
			sageMasterPursuit();
	}

	m_wSerchEnemyTime++;

	return	TRUE;
}

//
//	_lpTarget을 추적한다.
BOOL
cACTOR::sageMasterPursuit()
{
	if	(m_wTauntedToTargetTime)
	{
		if	(GetOvalRange(m_pos.x,m_pos.y,m_lpTarget->m_pos.x,m_lpTarget->m_pos.y)	>=	dMONSTER_MISSING_TAUNT_TARGET_RANGE)
			m_wTauntedToTargetTime	=	0;
	}

	if	(m_posTarget.x	!=	m_lpTarget->m_pos.x	||	m_posTarget.y	!=	m_lpTarget->m_pos.y)
		resetBlockedMoveStress();

	m_posTarget.x	=	m_lpTarget->m_pos.x;
	m_posTarget.y	=	m_lpTarget->m_pos.y;

	if	(!moveTo(m_posTarget.x,m_posTarget.y))
		return	TRUE;

	addNpcMoveMessage(FALSE);

	m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

	return	TRUE;
}

BOOL
cACTOR::operateSageMasterTraceAi()
{
	if	(m_wFleeTime || m_wInMagicBoxTime	||	m_wDanceTime	||	m_wHwabyungTime)
		return	TRUE;

	if	(m_lpTarget	==	NULL	||	m_lpTarget->isDeath()	||	m_lpTarget->m_wSerialInField	==	m_wSerialInField)
	{
		if	(m_iReaminSerchEnemyTime)
		{
			--m_iReaminSerchEnemyTime;
			return	TRUE;
		}

		cACTOR	*lpTarget	=	m_lpField->findNotUnderAttackeActor(this);

		if	(!lpTarget)
			return	TRUE;

		setTarget(lpTarget);

		lpTarget->addHostileEnemy(m_wSerialInField);
	}

	if	(m_bIsMoving)
	{
		if	(isTargetInAttackRange(m_lpTarget,0))
		{
			stop();
			addPacketStop();

			return	TRUE;
		}

		if	(m_dwLastMoveMessageTime	+	dMOVE_UPDATE_TIME	<=	(DWORD)m_lpField->m_iFrameCounter)
		{
			m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;

			addNpcMoveMessage(TRUE);
		}
	}
	else
	{
		if	(isTargetInAttackRange(m_lpTarget,0))
			return	TRUE;

		sageMasterPursuit();
	}

//	주기적으로 타겟의 위치를 체크하고, 이동했다면 위치를 다시 잡는다.
	if	(m_wSerchEnemyTime	>=	dAPPROACH_TARGET_REFRESH_TIME/2)
	{
		if	(m_lpTarget && m_lpTarget->isPlayer())
		{
			if	(!m_lpTarget->addHostileEnemy(m_wSerialInField))
				return TRUE;
		}

		m_wSerchEnemyTime	=	0;

		if	(m_lpTarget->m_pos.x	!=	m_posTarget.x	||	m_lpTarget->m_pos.y	!=	m_posTarget.y)	//	이동 했다.
			sageMasterPursuit();
	}

	m_wSerchEnemyTime++;

	return	TRUE;
}