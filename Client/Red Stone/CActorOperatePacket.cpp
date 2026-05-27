#include "CActor.H"
#include "CAgent.H"

//
//	이동결과를 처리한다.
void
CActor::operateMoveResult(SG_MOVE_RESULT *_lpMoveResultPacket)
{
	if (isDeath())
		return;

	m_isRunning				=	(char)_lpMoveResultPacket->isRun;
	m_iPPS					=	_lpMoveResultPacket->wPPS;

	if (m_isMove)	//	이동 중이면..
		if (g_hero.m_bf1IsRunning	!=	m_isRunning	||	g_hero.m_iPPS	!=	m_iPPS)//지금 이동방법이나 속도와 틀리다.
		{
			g_hero.m_iPPS		=	m_iPPS;
			moveTo(m_posDest.x,m_posDest.y,FALSE,FALSE);	//	이동
		}

	m_iMaxHP	=	max(m_iMaxHP,100);

	LONGLONG	llHP=	m_iMaxHP;
	llHP			*=	_lpMoveResultPacket->bf14HP;
	llHP			/=	10000;

	g_hero.m_wIsReadyToFireChargedMiniPetSkill	=	_lpMoveResultPacket->bf1MiniPetCharge1;
	g_hero.m_wIsReadyToFireChargedMiniPetSkill2	=	_lpMoveResultPacket->bf1MiniPetCharge2;

	g_hero.m_iWaitMoveResultTime=	0;
	m_iHP						=	(int)llHP;
	g_hero.m_iHP				=	(int)llHP;
	g_hero.m_iCP				=	_lpMoveResultPacket->sCP*100;
}

//
//	이동을 한다.
void
CActor::operateMovePacket(SG_MOVE *_lpMovePacket)
{
	stopAction();	//	뭔가 액션을 하고 있으면 일단 멈춘다.
	releaseStoned();

	if (isDeath())
	{
		addToObscurityActorList();

		return;
	}

	if (m_pos.x	==	_lpMovePacket->wDestPosX	&&	m_pos.y	==	_lpMovePacket->wDestPosY)
	{
		stop();

		return;
	}

	if	(_lpMovePacket->moveInfo.wPPS	==	0 || isRockBounding() || isStunedStatus())
	{
		m_iPPS	=	0;

		moveTo(_lpMovePacket->wDestPosX,_lpMovePacket->wDestPosY,FALSE,FALSE);

		return;
	}

	int		iIncorrectRange	=	GetOvalRange(m_pos.x,m_pos.y,_lpMovePacket->wPosX,_lpMovePacket->wPosY);
	int		iClientRange	=	GetOvalRange(m_pos.x,m_pos.y,_lpMovePacket->wDestPosX,_lpMovePacket->wDestPosY);
	int		iServerRange	=	GetOvalRange(_lpMovePacket->wPosX,_lpMovePacket->wPosY,_lpMovePacket->wDestPosX,_lpMovePacket->wDestPosY);
	BOOL	isCorrectSpeed	=	FALSE;
	CPos	posWaypoint,posDest;
	int		iCorrectFactor	=	2;

	m_iPPS					=	_lpMovePacket->moveInfo.wPPS;
	m_isRunning				=	(char)_lpMovePacket->moveInfo.isRun;

	if	(isPlayer())
	{
		iCorrectFactor	=	2;
		
		if (s_bIsGuildBattleField)
			iCorrectFactor	*=	2;
	}

	if	(isPet() || isSummonBeast())
		iCorrectFactor	=	4;

//	웨이 포인트(?) 찾기
	{
		posWaypoint.Set(_lpMovePacket->wPosX,_lpMovePacket->wPosY);
		posDest.Set(_lpMovePacket->wDestPosX,_lpMovePacket->wDestPosY);

		cANGLE::GetPosToTarget(&posWaypoint,&posDest,m_iPPS*2/iCorrectFactor,2);
	}

//	차이나는 거리가 수준(?) 이상이면..
	if (iIncorrectRange	>=	dRESET_RANGE_BY_MOVE_PACKET)
	{
		stop(_lpMovePacket->wPosX,_lpMovePacket->wPosY);
		iIncorrectRange	=	0;
	}
	else	//	별루 차이가 안나면...
	if (iIncorrectRange	>=	dREFIT_RANGE_BY_MOVE_PACKET)
	{
		int	iRange	=	GetOvalRange(m_pos.x,m_pos.y,posWaypoint.x,posWaypoint.y);
		iRange		=	(int)sqrt((double)iRange);
		m_iPPS		=	iRange/2*iCorrectFactor;
	}

	if (isPlayer())
		moveTo(_lpMovePacket->wDestPosX,_lpMovePacket->wDestPosY,FALSE,FALSE);
	else
	{
		int	iAngle		=	m_iAngle;
		int	iDirect		=	m_iDirect;

		moveTo(_lpMovePacket->wDestPosX,_lpMovePacket->wDestPosY,FALSE,FALSE);

		m_wStopDirect	=	0xffff;

		int	iGap		=	abs(m_iDirect-iDirect);

		if	(iGap<=1	||	iGap==7)
			iGap	=	0;
		else
			iGap	=	1;

		if (_lpMovePacket->moveInfo.isContinue	&&	iGap == 0)
		{
			m_iAngle	=	iAngle;
			m_iDirect	=	iDirect;
		}
	}

	if	(m_wLordActor	==	g_hero.m_iSerial	&&	m_bf1IsMarionette)
		g_hero.m_dwLastReceiveCommandTime	=	timeGetTime();
}

//
//	밀려나기
void
CActor::operateThrustPacket(SG_THRUST *_lpPacket)
{
	thrust(_lpPacket->wPosX,_lpPacket->wPosY,0xffff,_lpPacket->bf1IsBlockedByObject, _lpPacket->bf1IsChangeTargetDirect);
}

//
//	정지 패킷 처리
void
CActor::operateStopPacket(SG_STOP *_lpStopPacket)
{
	stopAction();	//	뭔가 액션을 하고 있으면 일단 멈춘다.

	{
		cPet	*lpPet	=	g_hero.getPet(g_hero.getPetIndex(m_wSerial));

		if (lpPet)
			lpPet->m_wEngageTarget	=	0xffff;
	}
	{
		cSummonBeast	*lpSummonBeast	=	g_hero.getSummonBeast(g_hero.getSummonBeastIndex(m_wSerial));

		if (lpSummonBeast)
			lpSummonBeast->m_wEngageTarget	=	0xffff;
	}

	if	(isDeath())
		return;

	if	(isHero())
	{
		g_hero.m_iWaitMoveResultTime	=	0;
		g_lpHero	=	g_lpHero;

		if	(_lpStopPacket->bf1IsChangeOwnerDirect)
		{
			stop(_lpStopPacket->wPosX,_lpStopPacket->wPosY);
			setDirect(_lpStopPacket->bf3Direct);
			g_shaker.add(m_pos.x,m_pos.y,3,500,0);
		}
		return;
	}

	int	iIncorrectRange	=	GetOvalRange(m_pos.x,m_pos.y,_lpStopPacket->wPosX,_lpStopPacket->wPosY);
	int	iPPS			=	m_iPPS;
	int	iDirect			=	0xffff,iAngle;

	if (iIncorrectRange	>=	dRESET_RANGE_BY_MOVE_PACKET)
	{
		stop(_lpStopPacket->wPosX,_lpStopPacket->wPosY);
		setDirect(_lpStopPacket->bf3Direct);
		return;
	}
	else
	{
		int	iRange	=	GetOvalRange(m_pos.x,m_pos.y,_lpStopPacket->wPosX,_lpStopPacket->wPosY);

		iPPS		=	(int)sqrt((double)iRange);
		iPPS		*=	8;
	}

	m_iPPS		=	iPPS;

	if (iIncorrectRange	<	10*10)
	{
		iDirect	=	m_iDirect;
		iAngle	=	m_iAngle;
	}

	setStopDirect(_lpStopPacket->bf3Direct);

	if (moveTo(_lpStopPacket->wPosX,_lpStopPacket->wPosY,FALSE,TRUE))
	{
		if (iDirect	!=	0xffff)
		{
			m_iDirect	=	iDirect;
			m_iAngle	=	iAngle;
		}
	}
	else
	{
		stop(_lpStopPacket->wPosX,_lpStopPacket->wPosY);
	}
}