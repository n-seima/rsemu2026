#include "cACTOR.H"
#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"
#include <math.h>
#include "secretDungeon.h"

//
//	_iMx,_iMy으로 이동하는데 막히냐?
inline	BOOL
cACTOR::isBlockedWay(int _iMx,int _iMy)
{
	BYTE	*lpMapInfo	=	m_lpField->m_pMap;
	int		iMapWidth	=	m_lpField->m_iWidth;
	int		iMapHeight	=	m_lpField->m_iHeight;
	int		iAddress	=	_iMx+_iMy*iMapWidth;
	int		iCheckXValue=	1;

	if	(_iMx <= 0 || _iMy <= 0 || _iMx >= iMapWidth-1 || _iMy >= iMapHeight -1)
		return	TRUE;

	if	(lpMapInfo[iAddress])
		return	TRUE;

	if	(m_pos.mx	==	_iMx	||	m_pos.my	==	_iMy	)
		return	FALSE;	//	이경우는 위에서 걸린다.

	int	iDx	=	m_pos.mx-	_iMx;
	int	iDy	=	m_pos.my-	_iMy;

	if	(iDx	<	0)
		iDx	=	-iDx;
	if	(iDy	<	0)
		iDy	=	-iDy;

	if	(m_pos.mx	<	_iMx)
		iCheckXValue=	-1;

	if	(m_pos.my	<	_iMy)
		iMapWidth	=	-iMapWidth;

	if	(lpMapInfo[iAddress+iMapWidth	])
		return	TRUE;

	if	(lpMapInfo[iAddress+iCheckXValue])
		return	TRUE;

	return	FALSE;
}

//
//	캐릭터의 다음 이동 지점을 얻어 온다.
inline	void
cACTOR::getNextPos(c4POINT *_lpPos)
{
	_lpPos->x	=	m_pos.x;
	_lpPos->y	=	m_pos.y;

	if	(m_pos.x	!=	m_posDest.x)	//	Way Point 근처에 왔으면 후다닥 붙어 버린다.
	{
		if	(m_pos.x < m_posDest.x)
		{	
			if	(m_pos.x + m_aPath[0].x	>= m_posDest.x)	
				_lpPos->x	=	m_posDest.x;
			else
				_lpPos->x	=	m_pos.x	+	m_aPath[m_wPathIndex].x;
		}
		else
		{	
			if	(m_pos.x + m_aPath[0].x	<= m_posDest.x)	
				_lpPos->x	=	m_posDest.x;
			else
				_lpPos->x	=	m_pos.x	+	m_aPath[m_wPathIndex].x;
		}
	}

	if	(m_pos.y	!=	m_posDest.y)
	{	
		if	(m_pos.y < m_posDest.y)
		{	
			if	(m_pos.y + m_aPath[0].y >= m_posDest.y)
				_lpPos->y	=	m_posDest.y;
			else
				_lpPos->y	=	m_pos.y	+m_aPath[m_wPathIndex].y;
		}
		else
		{	
			if	(m_pos.y + m_aPath[0].y <= m_posDest.y)	
				_lpPos->y	=	m_posDest.y;
			else
				_lpPos->y	=	m_pos.y	+ m_aPath[m_wPathIndex].y;
		}
	}

	_lpPos->mx	=	(_lpPos->x	>>	dTILE_XSIZE_SHIFT);
	_lpPos->my	=	(_lpPos->y	>>	dTILE_YSIZE_SHIFT);
}

//
//	현재 위치에서 목표위치까지 길을 만든다.
inline	BOOL
cACTOR::makePath(int _iX,int _iY)
{
	g_protractor.getPath(m_aPath,m_pos.x,m_pos.y,_iX,_iY,m_wPPS);

	m_wPathIndex	=	0;

	if (m_aPath[0].x	==	0)
	{	
		if (m_pos.x > _iX)
			m_aPath[0].x	=	-1;
		if (m_pos.x < _iX)
			m_aPath[0].x	=	1;
	}

	if (m_aPath[0].y	==	0)
	{	
		if (m_pos.y > _iY)
			m_aPath[0].y	=	-1;
		if (m_pos.y < _iY)
			m_aPath[0].y	=	1;
	}

	c4POINT	posNext;

	getNextPos(&posNext);

	if	(m_pos.mx	!=	posNext.mx	||	m_pos.my	!=	posNext.my)	
		if	(isSafeRoad(posNext.x,posNext.y)	==	FALSE)	//	다음 위치로 가는 길이 뭔가로 막혀 있다.
			return	FALSE;

	m_posDest.x		=	_iX;
	m_posDest.y		=	_iY;

	m_wAngle		=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,_iX,_iY);

	m_wDirect		=	GetDirect(m_wAngle,dDEFAULT_DIRECT_COUNT);

	return	TRUE;
//	<C2S>
}	//	cACTOR::makeLine(BOOL _isChangeDirect)

//
//makePath로 길을 찾고, 막혔을때 이넘으로 한번더 찾는다.. x,y까지 길을 만든다.
inline	BOOL
cACTOR::findOtherWay(int _iX,int _iY)
{
 	if (m_pos.x	==	_iX	||	m_pos.y	==	_iY	)
	{
		return FALSE;//	return	FALSE;	//	4방향 직선 이동이다.
	}

	//if(m_lpField->m_bIsUseFindWay)		//길찾기 사용이 체크되어 있어야 사용한다...ㅜ.ㅜ;
	{
		getMovePos(this);
		if(getAllPath(this , _iX , _iY))
			return	TRUE;
	}

	if (makePath(_iX,m_pos.y))
		return	TRUE;
	if (makePath(m_pos.x,_iY))
		return	TRUE;
	
	return	FALSE;
}

//
//	원래 이동하던대로 다시 이동
BOOL
cACTOR::moveDestPos()
{
	if (!m_bIsMoving)
		return	FALSE;

	if (moveTo(m_posDest.x,m_posDest.y))
	{
		if (!isPlayer())
		{
			m_dwLastMoveMessageTime	=	m_lpField->m_iFrameCounter;
			addNpcMoveMessage(TRUE);
		}
	}

	return	TRUE;
}

BOOL
cACTOR::moveToArea(int _iArea,BOOL _bIsToAxis,int _iMethod)
{
	cAreaInfo	*lpArea	=	m_lpField->getArea(_iArea);

	if	(!lpArea)
		return	FALSE;

	for (int iTryCount=100;iTryCount;--iTryCount)
	{
		cPOINT		posMove;

		if	(_bIsToAxis)
			lpArea->getCenterPos(&posMove);
		else
			lpArea->getRandomPos(&posMove);

		if	(_iMethod	==	2)	//	텔레포트
		{
			changePos(posMove.x,posMove.y,TRUE);	//	위치 바꾸고 본인과 다른이들한테 메시지 전송
			break;
		}
		else
		if	(moveTo(posMove.x,posMove.y))
		{
			setAiMove();
			break;
		}
	}

	if	(iTryCount	==	0)
		return	FALSE;

	return	TRUE;
}

//
//	_iX,_iY로 이동
BOOL
cACTOR::moveTo(int _iX,int _iY,int _iCorrectSpeed,int _iPPS)
{
	if	(m_wInMagicBoxTime	||	m_bIsGuildObject)
		return	FALSE;

	if	(m_iGuildGuardianType	&&	m_iGuildGuardianType	<	eGUILD_GUARDIAN_HOLLOW_KNIGHT)
		return	FALSE;

	int	iX,iY,iMx,iMy;

	iX	=	max(_iX,0);
	iY	=	max(_iY,0);
	iX	=	min(_iX,m_lpField->m_iPixelWidth-1);
	iY	=	min(_iY,m_lpField->m_iPixelHeight-1);

	iMx	=	max(iX/dTILE_XS,0);
	iMy	=	max(iY/dTILE_YS,0);
	iMx	=	min(iMx,m_lpField->m_iWidth-1);
	iMy	=	min(iMy,m_lpField->m_iHeight-1);

	if	(m_wLordOperator	==	0xffff)
	{
		if	(m_wBlockedToMoveStress	>=	4 ||m_wBlockedPosValue)
			return	FALSE;
	}
	else
	{
		_iCorrectSpeed	=	200;
	}

	m_wBlockedPosValue	=	4;

	if	(m_lpField->isBlocked(iMx,iMy))
	{
		increaseBlockedMoveStress(iMx,iMy);

		return	FALSE;
	}

	if	(_iPPS)
		m_wPPS		=	_iPPS;
	else
	{
		if	(isSummonBeast())
		{
			cACTOR	*lpTamer	=	getTamer();

			if	(lpTamer)
				m_wNpcMoveSpeed	=	lpTamer->getWalkSpeed();
		}
		else
		if	(m_wJob	<	dPLAYER_JOB_COUNT)
			m_wPPS	=	getWalkSpeed();
		else
		{
			m_wPPS	=	m_wNpcMoveSpeed*_iCorrectSpeed/100*(100+getMoveSpeed())/ 100;

			if	(m_iGuildGuardianType	==	eGUILD_GUARDIAN_HOLLOW_KNIGHT)
			{
				if	(m_iLevel	>=	80)
					m_wPPS	+=	75;
				else
				if	(m_iLevel	>=	60)
					m_wPPS	+=	25;
			}
		}

		if	(m_wColdTime)
			m_wPPS	=	max(m_wPPS*dCOLD_FACTOR/100,1);
	}

	if	(m_wPPS	==	0)
	{
		if	(m_bIsMoving)
		{
			stopAction();
			addPacketStop();
		}

		return	FALSE;
	}

	if	(!makePath(_iX,_iY))
		if	(!findOtherWay(_iX,_iY))
		{
			stop();
		 
			increaseBlockedMoveStress(iMx,iMy);
		 
			return	FALSE;	//	라인 그어!!
 		}

	m_bIsMoving	=	TRUE;

	return	TRUE;
}

//
//	
BOOL
cACTOR::isSafeRoad(int _iDestX,int _iDestY)
{
	int	iDMX	=	_iDestX>>dTILE_XSIZE_SHIFT;
	int	iDMY	=	_iDestY>>dTILE_YSIZE_SHIFT;

	int	iDx		=	m_pos.mx-	iDMX;
	int	iDy		=	m_pos.my-	iDMY;

	if	(iDx	<	0)
		iDx		=	-iDx;
	if	(iDy	<	0)
		iDy		=	-iDy;

	if	(iDx	>=	2||	iDy	>=	2)
		return	1-m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,_iDestX,_iDestY);

	return	1-isBlockedWay(iDMX,iDMY);
}

//
//	_iX,_iY로 이동
int
cACTOR::movePlayer(int _iX,int _iY,int _iDestX,int _iDestY)
{
	if	(m_bf1IsJoinGuildDungeon)		// 길드던전에 입장한다~
	{
		m_bf1IsJoinGuildDungeon	=	FALSE;
		
		++m_bf3GuildDungeonPlayTime;
	}

	if	(m_wHillSkill			!=	0xffff	)
		return	eMOVE_RESULT_FAILED_BY_ICY_STALAGMITE;

	if	(m_wPitchmanShopSerial	!=	0xffff)
		closePitchmanShop();

	if	(m_wLordOperator	!=	0xffff || m_wSlaveActor	!=	0xffff)
	{
		cACTOR	*lpSlave=	m_lpField->getActor(m_wLordOperator);

		if (!lpSlave)
			lpSlave		=	m_lpField->getActor(m_wSlaveActor);

		if (!lpSlave)
			m_lpField->bookingDisplacement(this,NULL,TRUE);
		else
		{
			lpSlave->moveTo(_iDestX,_iDestY);
			lpSlave->setAiMove();
			
			m_dwLastCommandTime	=	g_dwCurrentTime;
		}

		return	eMOVE_RESULT_MOVE_SLAVE;
	}

	stopBattle();

	if	(m_isRunning)
		action(dACT_RUN);
	else
		action(dACT_WALK);

	setMoveSpeed();

	int	iCheckPeriod	=	900;	//	일반 필드

	if	(m_lpField->m_bIsGuildBattleField)
		iCheckPeriod	=	400;	//	길드전
	else
	if	(m_lpField->m_wIsVillage)
		iCheckPeriod	=	1800;	//	마을

	{
		DWORD	dwCurrentTime	=	g_dwCurrentTime;

		if	(m_dwLastTimeForReceiveMoveMessage	<	dwCurrentTime)
		{
			DWORD	dwTimeGab		=	dwCurrentTime-m_dwLastTimeForReceiveMoveMessage;

			if	(dwTimeGab > (DWORD)iCheckPeriod)
				m_wOverSpeedCounter	=	0;
			else
			{
				m_wOverSpeedCounter++;

#ifndef	_FOR_JAPAN
//				if	(m_wOverSpeedCounter	>	5)
//				{
//					CLOG("check speed hack","[%.3d] %20s/%20s",m_wCurrentField,m_strId,m_strName);

//					setPeneltyTime(dPENELTY_TIME_FOR_SPEEDHACKUSER,ePPPTR_SPEED_HACK_USER);

//					stop();

//					return	eMOVE_RESULT_FAILED_BY_INCORRECT_POS;
//				}
#endif
			}
		}

		m_dwLastTimeForReceiveMoveMessage	=	dwCurrentTime;
	}

	if	(!setActorPos(_iX,_iY,TRUE))
		return	eMOVE_RESULT_FAILED_BY_BLOCKED_POS;

	if	(m_lpField->m_bIsGuildBattleField)
		checkCaughtInTrap();
	m_lpField->checkFieldTrap(this);

	m_wAngle		=	g_protractor.getAngleToTarget(m_pos.x,m_pos.y,_iDestX,_iDestY);

	if	(!makePath(_iDestX,_iDestY))
		return	eMOVE_RESULT_FAILED_BY_BLOCKED_POS;

	m_bIsMoving		=	TRUE;
//	moveTo(_iDestX,_iDestY);
	return	eMOVE_RESULT_SUCCESS;

#define	dCHECK_PERIOD	20

	int		iPPS			=	dCHECK_PERIOD;
	cPOINT	pos,posFind;
	BOOL	bIsBlocked		=	FALSE;
	int		iCheckCount		=	m_wPPS/dCHECK_PERIOD-1;

	posFind.x	=	-1;

	cPOINT	posOld;

	posOld.x	=	m_pos.mx;
	posOld.y	=	m_pos.my;

	for (;iCheckCount;iCheckCount--)
	{
		pos.x		=	_iX;
		pos.y		=	_iY;

		g_protractor.getAnglePos(&pos,m_wAngle,iPPS);

		if	(isBlockedWay(pos.x>>dTILE_XSIZE_SHIFT,pos.y>>dTILE_YSIZE_SHIFT))
		{
			bIsBlocked	=	TRUE;
			break;
		}

		m_pos.mx	=	pos.x>>dTILE_XSIZE_SHIFT;
		m_pos.my	=	pos.y>>dTILE_YSIZE_SHIFT;
		posFind.x	=	pos.x;
		posFind.y	=	pos.y;
		iPPS		+=	dCHECK_PERIOD;
	}

	m_pos.mx	=	posOld.x;
	m_pos.my	=	posOld.y;

	if	(bIsBlocked	==	FALSE	&&	iPPS < m_wPPS)	//	막힌것도 아닌데 끝까지 체크가 안됐다.
	{
		pos.x		=	_iX;
		pos.y		=	_iY;

		g_protractor.getAnglePos(&pos,m_wAngle,m_wPPS);

		if	(m_lpField->isMissileBlockedLine(m_pos.x,m_pos.y,pos.x,pos.y) == FALSE)
		{
			posFind.x	=	pos.x;
			posFind.y	=	pos.y;
		}
	}

	if	(posFind.x	==	-1)
		return	eMOVE_RESULT_FAILED_BY_BLOCKED_POS;

	int	iRange		=	GetOvalRange(m_pos.x,m_pos.y,_iDestX,_iDestY);
	int	iFindRange	=	GetOvalRange(m_pos.x,m_pos.y,posFind.x,posFind.y);

	if	(iRange	<=	iFindRange)
		setActorPos(_iDestX,_iDestY);
	else
		setActorPos(posFind.x,posFind.y);

	return	eMOVE_RESULT_SUCCESS;
}

//
//	이동 관련 업데이트
int
cACTOR::updateMove(BOOL _bIsSendStopPacket)
{
	c4POINT	posNext;

	getNextPos(&posNext);	//	다음 위치를 구한다.

	if	(m_pos.mx	!=	posNext.mx	||	m_pos.my	!=	posNext.my)	
		if	(isSafeRoad(posNext.x,posNext.y)	==	FALSE)	//	다음 위치로 가는 길이 뭔가로 막혀 있다.
		{
			stop();

			if	(_bIsSendStopPacket)
				addPacketStop();

			increaseBlockedMoveStress(m_posDest.x>>dTILE_XSIZE_SHIFT,m_posDest.y>>dTILE_YSIZE_SHIFT);
			
			return	eRUM_BLOCKED;
		}

	if	(!setActorPos(posNext.x,posNext.y))
		return	eRUM_BLOCKED;

	if	(m_wLordOperator!=	0xffff)
	{
		cACTOR	*lpLord	=	m_lpField->getTestedActor(m_wLordOperator);

		if	(lpLord)
			lpLord->setActorPos(m_pos.x,m_pos.y,TRUE,TRUE);
	}

	if	(isPureMonster() || m_lpField->m_bIsGuildBattleField)
		checkCaughtInTrap();
	if	(!isPureMonster())
		m_lpField->checkFieldTrap(this);

	if	(m_wMarionetteTime)
	{
		cACTOR	*lpLord	=	getMarionetteLord();

		if	(!lpLord)
			return	eRUM_BLOCKED;

		lpLord->setActorPos(m_pos.x,m_pos.y,TRUE,TRUE);
	}

	if	(m_pos.x	==	m_posDest.x	&&	m_pos.y	==	m_posDest.y	)
	{
		resetBlockedMoveStress();

		stop();

		if	(_bIsSendStopPacket)
			addPacketStop();

		return	eRUM_ARRIVE;
	}

	m_wPathIndex++;

	if	(m_wPathIndex	>= dUPDATE_MOVE_POS_PERIOD)
		m_wPathIndex	=	0;

	return	eRUM_MOVE;
}

//
//	이동 관련 업데이트
int
cACTOR::updateMoveOnlyPos()
{
	c4POINT	posNext;

	getNextPos(&posNext);				//	다음 위치를 구한다.

	if	(m_pos.mx	!=	posNext.mx	||	m_pos.my	!=	posNext.my)	
		if	(isBlockedWay(posNext.mx,posNext.my))	//	다음 위치로 가는 길이 뭔가로 막혀 있다.
			return	eRUM_BLOCKED;

	setActorPos(posNext.x,posNext.y);

	if	(m_pos.x	==	m_posDest.x	&&	m_pos.y	==	m_posDest.y	)
		return	eRUM_ARRIVE;

	m_wPathIndex++;

	if (m_wPathIndex	>= dUPDATE_MOVE_POS_PERIOD)
		m_wPathIndex	=	0;

	return	eRUM_MOVE;
}

//
//	정지
BOOL
cACTOR::stop(int _iX,int _iY,BOOL _bIsCheckRange,BOOL _bIsStop)
{
	m_posLastMoved.x	=	_iX;
	m_posLastMoved.y	=	_iY;

	if (_bIsStop)
		stop();

	return	setActorPos(_iX,_iY);
}


void
cACTOR::changePos(int _iX,int _iY,BOOL _bIsInnerTeleport)
{
	int		iOldLocal	=	m_iLocalPart;
	cRECT	rectMove;

	memcpy(&rectMove,&m_rectMoveFrame,sizeof(cRECT));

	if	(!setActorPos(_iX,_iY,TRUE,TRUE))
		return;

	if	(isPlayer())
		sendCorrectActorPos(_iX,_iY,_bIsInnerTeleport);

	SG_STOP		packet;

	packet.base.set(sizeof(SG_STOP),dSG_STOP);

	packet.bf12Serial	=	m_wSerialInField;
	packet.bf3Direct	=	m_wDirect;		//	이동 속도 보정
	packet.bf1IsChangeOwnerDirect	=	FALSE;
	packet.wPosX		=	m_pos.x;
	packet.wPosY		=	m_pos.y;	//	현재 위치

	m_lpField->sendPacketToSameLocalPlayer(iOldLocal,&rectMove,&packet,packet.base.wSize);

	addPacketStop();
}

//
//	이동 솔도 결정
void
cACTOR::setMoveSpeed(BOOL _bIsSendToPlayer)
{
	int	iMoveSpeed	=	getMoveSpeed();
	int	iPPS		=	0;

	if	(m_lpField->m_wIsGBS_Field && iMoveSpeed > 0)
	{
		if (g_bIsDuelServer)
		{
			if(g_iSeasonVariable >= dSEASON_VARIABLE_PVP_REFORM)
				iMoveSpeed	=	iMoveSpeed*dPVP_DUEL_CORRECT_SPEED_BONUS/100;
		}
		else
			iMoveSpeed	=	iMoveSpeed*dPVP_CORRECT_SPEED_BONUS/100;
	}

	if	(m_isRunning)
		iPPS	=	getBody()->getPPS(dACT_RUN);
	else
		iPPS	=	getBody()->getPPS(dACT_WALK);

	iPPS		=	iPPS*(iMoveSpeed+100)/ 100;

	if	(m_wColdTime			)
		iPPS	=	iPPS*dCOLD_FACTOR/100;

	if	(m_bf5MagicCarpetShape)
		iPPS	=	iPPS*m_wBoostSpeedByMagicCarpet/100;

	if	(m_lpField->m_wIsGBS_Field)
	{
		if (g_bIsDuelServer)
		{
			if(g_iSeasonVariable >= dSEASON_VARIABLE_PVP_REFORM)
				iPPS	=	iPPS*dPVP_DUEL_BATTLE_FIELD_MOVE_SPEED_FACTOR/100;
		}
		else
			iPPS	=	iPPS*dPVP_GUILD_BATTLE_FIELD_MOVE_SPEED_FACTOR/100;
	}

	if	(m_bf1IsExistDrakeCarpet)
		iPPS	=	min(iPPS,dLIMIT_ACTOR_MOVE_SPEED+200);
	else
		iPPS	=	min(iPPS,dLIMIT_ACTOR_MOVE_SPEED);

	m_wPPS		=	iPPS;

	if	(_bIsSendToPlayer)
		sendMoveState();
}

//
//	이동 솔도 결정
int
cACTOR::getWalkSpeed()
{
	int	iMoveSpeed	=	getMoveSpeed();
	int	iPPS;

	if	(m_lpField->m_wIsGBS_Field)
		iMoveSpeed	=	iMoveSpeed*dPVP_CORRECT_SPEED_BONUS/100;

	if	(m_wJob	<	dPLAYER_JOB_COUNT)
	{
		if	(m_isRunning)
			iPPS	=	getBody()->getPPS(dACT_RUN);
		else
			iPPS	=	getBody()->getPPS(dACT_WALK);
	}
	else
		iPPS	=	m_wNpcMoveSpeed;

	iPPS		=	iPPS*(iMoveSpeed+100)/ 100;

	if	(m_wColdTime			)
		iPPS	=	m_wPPS*dCOLD_FACTOR/100;

	if	(m_bf5MagicCarpetShape	)
		iPPS	=	iPPS*m_wBoostSpeedByMagicCarpet/100;

	if	(m_lpField->m_wIsGBS_Field)
		iPPS	=	iPPS*dPVP_GUILD_BATTLE_FIELD_MOVE_SPEED_FACTOR/100;

	iPPS	=	min(iPPS,dLIMIT_ACTOR_MOVE_SPEED);

	return	iPPS;
}


//
//	걷기/뛰기 전환
void
cACTOR::setMoveAbility(BOOL _isRunning)
{
	m_isRunning				=	_isRunning;

	setMoveSpeed();
}


BOOL
cACTOR::getRandomNearPlace(cPOINT *_lpPos,int _iPartRange,int _iAngle)
{
	int	iAngle	=	_iAngle;

	if (iAngle	==	0xffff)
		iAngle	=	random(360);

	for (int iTryCount=100;iTryCount;--iTryCount)
	{
		int		iRange	=	random(40)+getBodySize()/2+_iPartRange;
		cPOINT	posMove;

		posMove.Set(m_pos.x,m_pos.y);

		g_protractor.getAnglePos(&posMove,iAngle,iRange,FALSE);

		if (!m_lpField->isBlocked(posMove.x/dTILE_XS,posMove.y/dTILE_YS))
		{
			_lpPos->Set(posMove.x,posMove.y);

			return	TRUE;
		}

		iAngle	=	random(360);
	}

	_lpPos->Set(m_pos.x,m_pos.y);

	return	TRUE;
}

void
cACTOR::warpToTargetNearPlace(cACTOR *_lpActor)
{
	cPOINT	pos;

	int		iAngle	=	g_protractor.getAngleToTarget(_lpActor->m_pos.x,_lpActor->m_pos.y,m_pos.x,m_pos.y);

	_lpActor->getRandomNearPlace(&pos,getBodySize()*2/3,iAngle);

	setActorPos(pos.x,pos.y,TRUE,TRUE);
	sendCurrentPosition();
}

void
cACTOR::increaseBlockedMoveStress(int _iMX,int _iMY)
{
	if (m_wBlockedStressedTime)
		m_wBlockedToMoveStress++;

	m_wBlockedStressedTime	=	dSYNC_FPS;
}

void
cACTOR::resetBlockedMoveStress()
{
	m_wBlockedStressedTime	=	0;
	m_wBlockedToMoveStress	=	0;
}

BOOL
cACTOR::isMoveBlockOvercrowding()
{
	if (isPureMonster())
		return	FALSE;

	if (m_wBlockedToMoveStress	>	3)
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::checkIsNonTarget(cACTOR *_lpTarget)
{
	if	(_lpTarget->m_iRookieTime)
		return	TRUE;

	if	(_lpTarget->isSummonBeast() && m_wRidingDogTime)
		return	TRUE;

	if	(_lpTarget->m_bf1IsHide || _lpTarget->m_wTransToWeaponTime || _lpTarget->m_wRabbitTime	||	_lpTarget->m_wFeignDeathTime)
		return	TRUE;

	if	(_lpTarget->m_sInvisivilityTime ==0 &&  _lpTarget->m_sNonTargetTime == 0 && _lpTarget->m_sBlurTime == 0)
		return	FALSE;

	BOOL	bIsSuccessStealth	=	FALSE;

	cNPC	*lpNpc	=	getNpc();
	cJOB	*lpJob	=	getJob();

	if	(!lpJob || !lpNpc)
		return	FALSE;

	if	(lpNpc->isNonTarget(_lpTarget->m_wSerialInField))
		return	TRUE;

	if	(lpJob->m_wMonsterLevel	>=	4)	//	세미 보스 이상
		return	FALSE;
//	하이딩류 아이템 규칙에 적 캐릭터에게 발각될 조건 추가 : (숨은 캐릭터 레벨 + 숨기 레벨 <= 적 캐릭터 레벨 + 적 캐릭터 숨기 탐지 레벨) 일 경우 발각. 발각된 캐릭터는 반투명으로 표시됨
	int	iHideLevel		=	_lpTarget->getHidingLevel();
	int	iDetectLevel	=	getDetectingLevel();

	if	(iDetectLevel	>=	iHideLevel || _lpTarget->m_wNonActionTime	<=	15)
		return	FALSE;

	if	(_lpTarget->m_bIsMoving	==	FALSE)	//	정지 상태이거나 걷기상태일때..
		if	(random(100) >= 2)
		{
			lpNpc->addNonTarget(_lpTarget->m_wSerialInField,10);
			return	TRUE;
		}

	if (_lpTarget->m_sInvisivilityTime)	//	몹의 레벨이 타겟레벨+100레벨 이하일때
	{
		int	iDetectionChance	=	m_wFoundOutChanceInInvisible;

		if	(m_bIsMoving		==	FALSE)
			iDetectionChance	=	m_wFoundOutChanceInInvisibleForStand;

		if	(random(10000)	>=	iDetectionChance)
		{
			lpNpc->addNonTarget(_lpTarget->m_wSerialInField,10);

			return	TRUE;
		}

		return	FALSE;
	}

	if	(_lpTarget->m_sBlurTime)	//	몹의 레벨이 타겟레벨+150레벨 이하일때
	{
		if	(_lpTarget->m_isRunning	==	FALSE && random(100) >= 5)	//	걷기상태일때..
		{
			lpNpc->addNonTarget(_lpTarget->m_wSerialInField,10);

			return	TRUE;
		}

		return	FALSE;
	}

	if (_lpTarget->m_sNonTargetTime)	//	몹의 레벨이 타겟레벨+50레벨 이하일때
	{
		if	(random(100) >= 10)
		{
			lpNpc->addNonTarget(_lpTarget->m_wSerialInField,10);

			return	TRUE;
		}

		return	FALSE;
	}

	return	FALSE;
}

BOOL
cACTOR::checkMoveCounter(int _iX,int _iY,char *_lpstrWhere)
{
	setMoveSpeed();
	
	if	(m_wIsSendedCorrectPosPacket)
	{
		if	(_iX	!=	m_pos.x	||	_iY	!=	m_pos.y)
			return	FALSE;

		m_wIsSendedCorrectPosPacket	=	FALSE;
	}

	m_wReceiveMovePacketCounter++;

	if	(m_dwLastCheckMoveSpeedTime+1000	<	g_dwCurrentTime)
	{
#ifndef	_FOR_JAPAN
//		if	(m_wReceiveMovePacketCounter	>=	6 + m_lpField->m_bIsGuildBattleField*2) 
//			setPeneltyTime(m_wReceiveMovePacketCounter-4,ePPPTR_MOVE_PACKET_HACKING);
#endif
		m_wReceiveMovePacketCounter		=	0;
		m_dwLastCheckMoveSpeedTime		=	g_dwCurrentTime;
	}

	if	(!g_bIsInnerTestServer && m_wOperatorLevel < eAL_TESTER)
	{
		int	iRange	=	GetOvalRange(_iX,_iY,m_posLastMoved.x,m_posLastMoved.y);
		int	iPPS	=	m_wPPS;
		int	iBadRange=	2000;

		if	(m_lpField->m_bIsGuildBattleField)
		{
			iPPS	/=	2;
			iBadRange/=	2;
		}
		else
		if (m_lpField->m_wIsVillage)
		{
			iPPS	*=	2;
			iBadRange*=	2;
		}

		iPPS		=	iPPS*200/100;
		iPPS		=	iPPS*iPPS;

#ifndef	_FOR_JAPAN
//		if (iRange	>=	iBadRange*iBadRange)
//		{
//			CLOG("packetHacking","%s [%.3d] [%s/%s] iBadRange*iBadRange %d>%d",_lpstrWhere,m_wCurrentField,m_strId,m_strName,iRange,iBadRange*iBadRange);

//			setPeneltyTime(60*10,ePPPTR_MOVE_PACKET_HACKING);
//		}
#endif
		if (iRange	>=	iPPS)
		{
			stop();

			sendMoveFailed(eMOVE_RESULT_FAILED_BY_INCORRECT_POS);

			return	FALSE;
		}
	}

	return	TRUE;
}

BOOL
cACTOR::checkMoveReport(int _iX,int _iY)
{
	if (m_wIsSendedCorrectPosPacket)
	{
		if (_iX	!=	m_pos.x	||	_iY	!=	m_pos.y)
			return	FALSE;

		m_wIsSendedCorrectPosPacket	=	FALSE;
	}

	if	(!g_bIsInnerTestServer && m_wOperatorLevel < eAL_TESTER)
	{
		int	iRange	=	GetOvalRange(_iX,_iY,m_posLastMoved.x,m_posLastMoved.y);
		int	iPPS	=	m_wPPS;
		int	iBadRange=	2000;

		if	(m_lpField->m_bIsGuildBattleField)
		{
			iPPS	/=	2;
			iBadRange/=	2;
		}
		else
		if (m_lpField->m_wIsVillage)
		{
			iPPS	*=	2;
			iBadRange*=	2;
		}

		iPPS		=	iPPS*150/100;
		iPPS		=	iPPS*iPPS;

#ifndef	_FOR_JAPAN
///		if (iRange	>=	iBadRange*iBadRange)
//		{
//			CLOG("packetHacking","[%.3d] [%s/%s] iBadRange*iBadRange %d>%d",m_wCurrentField,m_strId,m_strName,iRange,iBadRange*iBadRange);

//			setPeneltyTime(60*10,ePPPTR_MOVE_PACKET_HACKING);
//		}
#endif

		if (iRange	>=	iPPS)
		{
			stop();

			sendMoveFailed(eMOVE_RESULT_FAILED_BY_INCORRECT_POS);

			return	FALSE;
		}
	}

	return	TRUE;
}