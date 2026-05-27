////////////////////////////////////////////////////////////////////
//	캐릭터의 이동에 관련된 것들..
////////////////////////////////////////////////////////////////////

#include <MATH.H>
#include "CActor.H"
#include "cMAP.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "CFieldItem.H"
#include "cSHAKE.H"
#include "CAgent.H"

//
//	Check Blocked Way Result
enum
{
	eCBWR_FREE_WAY,
	eCBWR_FAR_WAY,
	eCBWR_BLOCKED
};


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//	이동 관련 
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//
//	_iMx,_iMy으로 이동하는데 막히냐?
inline	int
CActor::checkBlockedWay(int _iMx,int _iMy)
{
	int	iAddress	=	_iMx+_iMy*g_map.m_iWidth;
	int	iCheckYValue=	g_map.m_iWidth;
	int	iCheckXValue=	1;

	if (_iMx	>=	g_map.m_iWidth	-1	||
		_iMy	>=	g_map.m_iHeight	-1	||
		_iMx	<	0	||	_iMy	<	0)
		return	eCBWR_BLOCKED;

	if (g_map.m_aBlockInfo[iAddress])
		return	eCBWR_BLOCKED;
	if	(m_pos.mx	==	_iMx	||	m_pos.my	==	_iMy		)
		return	eCBWR_FREE_WAY;

//	2칸 이상 이동하는지 체크
	{
		int	iDx	=	m_pos.mx - _iMx;
		int	iDy	=	m_pos.my - _iMy;

		if (iDx	<	0)	iDx	=	-iDx;
		if (iDy	<	0)	iDy	=	-iDy;

		if (iDx	+	iDy	>	2)
			return	eCBWR_FAR_WAY;
	}

	if	(m_pos.mx	<	_iMx)
		iCheckXValue=	-1;
	if	(m_pos.my	<	_iMy)
		iCheckYValue=	-iCheckYValue;

	if (g_map.m_aBlockInfo[iAddress+iCheckYValue	])
		return	eCBWR_BLOCKED;
	if (g_map.m_aBlockInfo[iAddress+iCheckXValue	])
		return	eCBWR_BLOCKED;

	return	eCBWR_FREE_WAY;
}	//	CActor::checkBlockedWay(int _iMx,int _iMy)

inline	BOOL
CActor::isBlockedWay(int _iPreMx,int _iPreMy,int _iMx,int _iMy)
{
	int	iAddress	=	_iMx+_iMy*g_map.m_iWidth;

	if	(g_map.m_aBlockInfo[_iPreMx+_iPreMy*g_map.m_iWidth])
		return	TRUE;
	if	(g_map.m_aBlockInfo[iAddress])
		return	TRUE;

	if	(_iPreMx	==	_iMx	||	_iPreMy	==	_iMy	)
		return	FALSE;

	int	iCheckYValue=	-g_map.m_iWidth;
	int	iCheckXValue=	-1;

	if	(_iPreMx	>	_iMx)
		iCheckXValue=	1;
	if	(_iPreMy	>	_iMy)
		iCheckYValue=	-iCheckYValue;

	if	(g_bIsTestServer)
	{
		if	(g_map.m_aBlockInfo[iAddress+iCheckYValue	]	&&	g_map.m_aBlockInfo[iAddress+iCheckXValue	])
			return	TRUE;
	}
	else
	{
		if	(g_map.m_aBlockInfo[iAddress+iCheckYValue	]	||	g_map.m_aBlockInfo[iAddress+iCheckXValue	])
			return	TRUE;
	}

	return	FALSE;
}

//
//	
inline	BOOL
CActor::isBlockedLine(int _iX1,int _iY1,int _iX2,int _iY2)
{
	int	i;
	int	iRoadCount	=	0;		//	경유하는 포인트의 수
	int	iWidth,iHeight,iDx,iDy,iYLineValue;

    if (_iX1 > _iX2)
		iWidth	=	_iX1-_iX2,iDx=-1;
	else
		iWidth	=	_iX2-_iX1,iDx=1;

    if (_iY1 > _iY2)
		iHeight	=	_iY1-_iY2,iDy=-1,iYLineValue=-g_map.m_iWidth;
	else
		iHeight	=	_iY2-_iY1,iDy=1	,iYLineValue=g_map.m_iWidth;

	int	iPre,iPru,iPoint;
	int	iMx,iMy,iPreMx,iPreMy;

	iMx				=	_iX1>>dTILE_XSIZE_SHIFT;
	iMy				=	_iY1>>dTILE_YSIZE_SHIFT;
	iPreMx			=	iMx;
	iPreMy			=	iMy;

	int	iAddress	=	iMx+iMy*g_map.m_iWidth;

	if (g_map.m_aBlockInfo[iAddress])
		return	TRUE;

	if (_iX1>=	g_map.m_iPixelWidth	-dTILE_XS||	_iX2>=	g_map.m_iPixelWidth	-dTILE_XS)
		return	FALSE;
	if (_iY1>=	g_map.m_iPixelHeight-dTILE_YS||	_iY2>=	g_map.m_iPixelHeight-dTILE_YS)
		return	FALSE;
	if (_iX1<=	dTILE_XS					 ||	_iX2<=	dTILE_XS					 )
		return	FALSE;
	if (_iY1<=	dTILE_YS					 ||	_iY2<=	dTILE_YS					 )
		return	FALSE;

	if (_iX2	==	_iX1)
	{
		for (i=0;i<iHeight;i++)
		{
			iMy		=	_iY1>>dTILE_YSIZE_SHIFT;

			iAddress=	iMx+iMy*g_map.m_iWidth;
			if	(g_map.m_aBlockInfo[iAddress])
				return	TRUE;

			_iY1	+=	iDy;
		}

		return FALSE;
	}

	if (_iY2 == _iY1)
	{
		for (i=0;i<iWidth;i++)
		{
			iMx		=	_iX1>>dTILE_XSIZE_SHIFT;

			iAddress=	iMx+iMy*g_map.m_iWidth;
			if (g_map.m_aBlockInfo[iAddress])
				return	TRUE;

			_iX1	+=	iDx;
		}

		return FALSE;
	}

    if (iWidth >= iHeight)
    {           
        iPre 	= iHeight<<1;   						// amount to increment decision if right is chosen (always)
        iPru 	= iPre - (iWidth<<1);					// amount to increment decision if up is chosen
        iPoint 	= iPre - iWidth;						// decision variable start value

        for (; iWidth>=0; iWidth--)						// process each point in the line one at a time (just use iWidth)
        {
			iMx		=	_iX1>>dTILE_XSIZE_SHIFT;
			iMy		=	_iY1>>dTILE_YSIZE_SHIFT;

			if (iMx != iPreMy || iMy != iPreMy)
			{
				iMx = iPreMy;
				iMy = iPreMy;

				if	(isBlockedWay(iPreMy,iPreMx,iMx,iMy))
					return	FALSE;
			}

            if (iPoint > 0)                              // is the pixel going right AND up?
            {	
				_iX1	+=iDx;					// increment independent variable
                _iY1	+=iDy; 					// increment dependent variable
                iPoint	+=iPru;						// increment decision (for up)
            }
            else									// is the pixel just going right?
            {   
				_iX1	+=iDx;					// increment independent variable
                iPoint	+=iPre;						// increment decision (for right)
            }
        }
    }
    else
    {
        iPre 	= iWidth<<1;   							// amount to increment decision if right is chosen (always)
        iPru 	= iPre - (iHeight<<1);    				// amount to increment decision if up is chosen
        iPoint 	= iPre - iHeight;							// decision variable start value
        
        for (; iHeight>=0; iHeight--)							// process each point in the line one at a time (just use iHeight)
        {
			iMx		=	_iX1>>dTILE_XSIZE_SHIFT;
			iMy		=	_iY1>>dTILE_YSIZE_SHIFT;
			
			if (iMx != iPreMy || iMy != iPreMy)
			{
				iMx = iPreMy;
				iMy = iPreMy;

				if	(isBlockedWay(iPreMy,iPreMx,iMx,iMy))
					return	FALSE;
			}

            if (iPoint > 0)                              // is the pixel going up AND right?
            { 
                _iX1	+=iDx; 					// increment dependent variable
                _iY1	+=iDy;					// increment independent variable
                iPoint	+=iPru;						// increment decision (for up)
            }
            else								// is the pixel just going up?
            {
                _iY1	+=	iDy;				// increment independent variable
                iPoint	+=	iPre;				// increment decision (for right)
            }
        }		
   }

	return	FALSE;
}

//
//	캐릭터 이동에 관련된 업데이트
BOOL
CActor::moveUpdate()
{
	c4POINT		posNext;
	CPos		posOld;

	posOld.Set(m_pos.x,m_pos.y);

	getNextPos(&posNext);				//	다음 위치를 구한다.

	int	iResult		=	checkBlockedWay(posNext.mx,posNext.my);

	if	(iResult	==	eCBWR_FAR_WAY)
		if	(isBlockedLine(m_pos.x,m_pos.y,posNext.x,posNext.y))
			iResult	=	eCBWR_BLOCKED;

	if	(iResult	==	eCBWR_BLOCKED)
	{
		if	(m_isFindStopPosition	)
			setPos(m_posDest.x,m_posDest.y);//목표위치까지 이동해야 하는데.. 막혔다.

		stop();

		return	FALSE;
	}

	int	iCurrentRange	=	GetOvalRange(m_pos.x,m_pos.y,m_posDest.x,m_posDest.y) + 5;

	int	iSpeed			=	(m_iPPS/dSYNC_FPS+3);

	iSpeed				*=	iSpeed;

	if	(iCurrentRange	<=	iSpeed)
	{
		m_pos.x		=	m_posDest.x;
		m_pos.y		=	m_posDest.y;
		m_pos.mx	=	m_pos.x/dTILE_XS;
		m_pos.my	=	m_pos.y/dTILE_YS;
	}
	else
	{
		m_pos.x		=	posNext.x;
		m_pos.y		=	posNext.y;
		m_pos.mx	=	posNext.mx;
		m_pos.my	=	posNext.my;	
	}

	if	(m_wBiter	!=	0xfff)
	{
		int		iDX			=	m_pos.x-posOld.x;
		int		iDY			=	m_pos.y-posOld.y;

		CActor	*lpBiter	=	g_am.getTestedActor(m_wBiter);

		if	(lpBiter)
		{
			lpBiter->m_pos.x+=	iDX;
			lpBiter->m_pos.y+=	iDY;
		}
	}

	m_iPathIndex++;

	if	(m_iPathIndex	>= dSYNC_FPS)
		m_iPathIndex	=	0;


	if	(m_pos.x		==	m_posDest.x	&&	m_pos.y	==	m_posDest.y	)
	{
#ifdef _DEBUG							//JBC 점프 수정 08-07-30
		if(m_bIsJumpAttack)
		{
			if (m_wUseSpecialActionSkill == eSAS_JUMP || m_wUseSpecialActionSkill ==	eSAS_BOUNCING_LINEAR || m_wUseSpecialActionSkill ==	eSAS_BIG_BULLET)
				m_isMove	=	FALSE;
			else
				stop();
		}
		else
		{
			if (m_wUseSpecialActionSkill ==	eSAS_BOUNCING_LINEAR || m_wUseSpecialActionSkill ==	eSAS_BIG_BULLET)
				m_isMove	=	FALSE;
			else
				stop();
		}
#else
		if (m_wUseSpecialActionSkill == eSAS_JUMP || m_wUseSpecialActionSkill ==	eSAS_BOUNCING_LINEAR || m_wUseSpecialActionSkill ==	eSAS_BIG_BULLET)
			m_isMove	=	FALSE;
		else
			stop();
#endif
		
			
	}

	return	TRUE;
}

//
//	길찾기
//	actor가 있는 곳으로 이동한다.
BOOL
CActor::traceActor(int actor,BOOL _isFindDetailPath)
{
	return TRUE;
}

//
//	캐릭터 쫒아가기
//	int		serial		-	캐릭터 인덱스
void
CActor::chaseActor(int serial)
{

}

//
//	다음 프레임에서 index캐릭터와 가까워진다. 
inline	BOOL
CActor::isApproach(int index)
{

	return FALSE;
}

//
//	x,y로 이동 가능하다.
inline	BOOL
CActor::isMoveAblePos(int mx,int my)
{
	return	TRUE;
}

//
//makePath로 길을 찾고, 막혔을때 이넘으로 한번더 찾는다.. x,y까지 길을 만든다.
BOOL
CActor::findOtherWay(int _iX,int _iY)
{
	static	CPos	posLastFind;
	BOOL	isMovedMouse	=	FALSE;
	CPos	posFind;

	if (posLastFind.x	!=	_iX	||	posLastFind.y	!=	_iY)	isMovedMouse	=	TRUE;

	if (m_pos.x	==	_iX	||	m_pos.y	==	_iY	)	return	FALSE;	//	4방향 직선 이동이다.
	if (m_isBlockedWay	&&	!isMovedMouse	)	return	FALSE;	//	막힌길인데다가.. 마우스도 안 움직였다.

	if (_iX		>	m_pos.x)	posFind.x	=	m_pos.x	+	dTILE_XS;
	if (_iX		<	m_pos.x)	posFind.x	=	m_pos.x	-	dTILE_XS;
	if (_iY		>	m_pos.y)	posFind.y	=	m_pos.y	+	dTILE_YS;
	if (_iY		<	m_pos.y)	posFind.y	=	m_pos.y	-	dTILE_YS;

	if (makePath(posFind.x,m_pos.y))	return	TRUE;
	if (makePath(m_pos.x,posFind.y))	return	TRUE;

	m_isBlockedWay	=	TRUE;

	posLastFind.x	=	_iX;
	posLastFind.y	=	_iY;

	return	FALSE;
}	//	CActor::findOtherWay(int x,int y)

//
//	현재 위치에서 목표위치까지 길을 만든다.
BOOL
CActor::makePath(int _iX,int _iY)
{
	m_posDest.x		=	_iX;
	m_posDest.y		=	_iY;

	m_iAngle	=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,_iX,_iY,2);
	m_iViewAngle=	cANGLE::GetAngleToTarget(m_pos.x,m_pos.y,_iX,_iY,1);

	GetLinerRoad(m_aPath,m_iPPS,m_iAngle,dSYNC_FPS);

	m_iPathIndex=	0;

	if (m_aPath[0].x	==	0)
	{	
		if (m_pos.x > _iX)	m_aPath[0].x	=	-1;
		if (m_pos.x < _iX)	m_aPath[0].x	=	1;
	}

	if (m_aPath[0].y	==	0)
	{	
		if (m_pos.y > _iY)	m_aPath[0].y	=	-1;
		if (m_pos.y < _iY)	m_aPath[0].y	=	1;
	}

	c4POINT	posNext;

	getNextPos(&posNext);

	int	iResult	=	checkBlockedWay(posNext.mx,posNext.my);

	if	(iResult	==	eCBWR_FAR_WAY)
		if	(isBlockedLine(m_pos.x,m_pos.y,posNext.x,posNext.y))
			iResult	=	eCBWR_BLOCKED;

	if (iResult	==	eCBWR_BLOCKED)	return FALSE;

	m_isBlockedWay	=	FALSE;

	return	TRUE;
//	<C2S>
}	//	CActor::makePath(int _iX,int _iY)

//
//	캐릭터를 x,y로 이동 시킨다.
BOOL
CActor::moveTo(int _iX,int _iY,BOOL _isFindAnyWay,BOOL _isFindStopPosition,BOOL _bIsThrust)
{
	if	(isStunedStatus())
	{
		stop();
		return	FALSE;
	}

	if (m_pos.x	==	_iX	&&	m_pos.y	==	_iY	)
		return	FALSE;

	m_isFindStopPosition	=	_isFindStopPosition;

	if (m_iPPS	==	0)
	{
		if (m_isRunning)
			m_iPPS	=	getPPS(dACT_RUN);
		else
			m_iPPS	=	getPPS(dACT_WALK);
	}

	if (!makePath(_iX,_iY))
		if (_isFindAnyWay)
		{
			if (!findOtherWay(_iX,_iY))
				return	FALSE;	//	다른길 찾아봐
		}
		else
			return	FALSE;	//	-o-

	c4POINT		posNext;

	getNextPos(&posNext);				//	다음 위치를 구한다.

	if	(posNext.x	==	m_posDest.x	&&	posNext.y	==	m_posDest.y	)
	{
		return	FALSE;
	}

	if	(!_bIsThrust)
	{
		setExclusiveAction(FALSE);

		if	(m_enchantedImage.m_bf1IsJumppingSkipping && m_wJob == dJOB_PRINCESS)
			setAnm(dACT_PRINCE_JUMPPING_SKIPPING);
		else
		{
			if	(m_isRunning)
				setAnm(dACT_RUN);
			else
				setAnm(dACT_WALK);
		}
	}

	m_isMove		=	TRUE;

	if	(isHero())
		g_hero.m_isWalking	=	TRUE;

	return	TRUE;
}

//
//	캐릭터의 다음 이동 지점을 얻어 온다.
void
CActor::getNextPos(c4POINT *pos)
{
	pos->x	=	m_pos.x;
	pos->y	=	m_pos.y;

	if (m_pos.x	!=	m_posDest.x)	//	Way Point 근처에 왔으면 후다닥 붙어 버린다.
	{
		if (m_pos.x < m_posDest.x)
		{	
			if (m_pos.x + m_aPath[0].x	>= m_posDest.x)
				pos->x	=	m_posDest.x;
			else
				pos->x	=	m_pos.x	+	m_aPath[m_iPathIndex].x;
		}
		else
		{	
			if (m_pos.x + m_aPath[0].x	<= m_posDest.x)
				pos->x	=	m_posDest.x;
			else
				pos->x	=	m_pos.x	+	m_aPath[m_iPathIndex].x;
		}
	}

	if (m_pos.y	!=	m_posDest.y)
	{	
		if (m_pos.y < m_posDest.y)
		{	
			if (m_pos.y + m_aPath[0].y >= m_posDest.y)
				pos->y	=	m_posDest.y;
			else
				pos->y	=	m_pos.y	+m_aPath[m_iPathIndex].y;
		}
		else
		{	
			if (m_pos.y + m_aPath[0].y <= m_posDest.y)
				pos->y	=	m_posDest.y;
			else
				pos->y	=	m_pos.y	+ m_aPath[m_iPathIndex].y;
		}
	}

	pos->mx	=	(pos->x	>>	dTILE_XSIZE_SHIFT);
	pos->my	=	(pos->y	>>	dTILE_YSIZE_SHIFT);
}

//
//	어거지로 잔상을 만들어 준다. 
//	pos 를 기준으로 anm,direct,frame,과 똑같은 angle 방향으로 pps 단위로 reverse일 경우 거꾸로 해서... dust에 따라 먼지를 찍어준다.
void
CActor::makeAfterImage(CPos *pos,int anm,int direct,int frame,int angle,int pps,BOOL reverse,BOOL dust)
{
	if (m_wUseSkill	==	0xffff)
		return;

/*	CSkill	*lpSkill	=	g_aSM[m_wUseSkill].GetSkill();

	CPos	TempPos;

	TempPos.x	=	pos->x;
	TempPos.y	=	pos->y;

	for (int i=0;i<m_cAfterImageCount;i++)
	{
		m_aPreImage[i][0]	=	anm;
		m_aPreImage[i][1]	=	direct;
		m_aPreImage[i][2]	=	frame;

		m_aPreviousPos[i].x	=	TempPos.x;
		m_aPreviousPos[i].y	=	TempPos.y;

		if (reverse)	m_aAfterImageAlpha[i]	=	16-i;
		else			m_aAfterImageAlpha[i]	=	8+i;

		if (dust)	cDUST::ADD(TempPos.x,TempPos.y,lpSkill->m_wDustImage);

		GetOvalAnglePos(&TempPos,angle,pps);

		if (m_iRush	==	dRUSH_SPIN)		//	스핀 러쉬이다.
		{
			frame--;						//	프레임 변화가 있다.
			if (frame < 0 ) frame = m_iFrameCount-1;
		}
	}

	while(1 && dust)
	{
		int	range	=	GetOvalRange(TempPos.x,TempPos.y,m_pos.x,m_pos.y);

		if (range > pps*pps)	return;

		cDUST::ADD(TempPos.x,TempPos.y,lpSkill->m_wDustImage);

		GetOvalAnglePos(&TempPos,angle,pps);
	}
	*/
}

//
//	정지
void
CActor::stop(BOOL _bIsCancelUsedSkill)
{
	if	(m_iAnm	==	dACT_CHANGE1)
		return;

	if	(m_wBiteTarget	!=	0xffff)
	{
		CActor	*lpTarget	=	g_am.getTestedActor(m_wBiteTarget);

		if	(lpTarget)
			lpTarget->m_wBiter	=	0xffff;
		
		m_wBiteTarget	=	0xffff;
	}

	setInvincibleSkill(FALSE);

	if	(isStunedStatus()	==	FALSE)
		setExclusiveAction(FALSE);

	if (m_wStopDirect	!=	0xffff)
		setDirect(m_wStopDirect);

	m_wUseSpecialActionSkill=	eSAS_NONE;
	m_isMove				=	FALSE;
	m_isBlockedWay			=	FALSE;;			//	막힌길이라니까!!
	m_wIsDontHitAction		=	FALSE;

	m_wRushStatus			=	eRS_NONE;
	m_wJumpHeight			=	0;
	m_wJumpStatus			=	eJS_NONE;
	m_bIsJumpRush			=	FALSE;		
	m_wRushAcceleration		=	0;	
	m_iHeightJumpRush		=	0;	
	m_wIncreaseHeightJumpRush	=	0;	
	m_dwRushFrameCounter	=	0;
	m_wJumpRushLastActionFrame	=	0;				//	점프러쉬 최종액션프레임
	m_iJumpRushRange		=	0;
	m_isSkillControl		=	FALSE;
	m_iPathIndex			=	0;
	m_wLoopState			=	dLOOP_NONE;
	m_wBlockingRigidityTime	=	0;
	m_wPitchmanShopSerial	=	0xffff;
	m_wOverlapAnm			=	0xffff;
	m_wOverlapAnm2			=	0xffff;

	if	(isHero())
		g_hero.offNotFocusStatus();

	m_wIsFreezeAtTriggerFrame=	FALSE;

	if	(m_wChargeImage		!=	0xffff)
		cancelChargeEffect();//	챠지 캔슬?

	if	(isHero())
	{
//		g_hero.m_iWaitMoveResultTime=	0;
//		g_hero.m_wEngageTarget		=	0xffff;
	}

	m_posDest.x				=	m_pos.x;
	m_posDest.y				=	m_pos.y;
	m_posDest.mx			=	m_pos.mx;
	m_posDest.my			=	m_pos.my;
	m_wStopDirect			=	0xffff;

	if (m_cAfterImageStatus)
		afterImageOff();

	if (m_wParallelBunshineCount)
	{
		afterImageOn(dFADE_AFTER_IMAGE,dAFTER_IMAGE_COUNT,20,1);

		if	(m_wAwesomeFortressAction	==	0xffff)
			m_wParallelBunshineCount	=	0;
	}
	if (m_wDoppelgangerCount)
	{
		afterImageOn(dFADE_AFTER_IMAGE,dAFTER_IMAGE_COUNT,20,1);

		if	(m_wContinuousHitOfDoppelgangerAction[0]	==	0xffff)
			m_wDoppelgangerCount	=	0;
	}
	if (_bIsCancelUsedSkill	&&	m_wUseSkill	!=	0xffff)
	{
		if (m_wUseSkill	<	dACTIVE_SKILL_COUNT)
			g_sm.casterStop(m_wUseSkill);

		m_wUseSkill		=	0xffff;
	}

	m_abilityUse.reset();

	m_iPostureOfDefense	=	0;

	if (!isDeath())
		setAnm(dACT_READY);
	else
		setExclusiveAction(TRUE);

}

void
CActor::stopAction()
{
	if	(m_wUseSkill	==	0xffff	&&	m_wBiteTarget	==	0xffff	)
		return;	//	액션을 사용중이 아니다.

	stop();

	return;
	if (m_iAnm		==	dACT_CHANGE1)	return;

	setInvincibleSkill(FALSE);
	setExclusiveAction(FALSE);

	m_isMove				=	FALSE;
	m_isBlockedWay			=	FALSE;;			//	막힌길이라니까!!
	m_wUseSpecialActionSkill=	eSAS_NONE;

	m_wRushStatus			=	eRS_NONE;
	m_wJumpHeight			=	0;
	m_wJumpStatus			=	eJS_NONE;
	m_isSkillControl		=	FALSE;
	m_iPathIndex			=	0;
	m_wLoopState			=	dLOOP_NONE;
	m_wBlockingRigidityTime	=	0;
	m_bIsJumpRush			=	FALSE;		
	m_wRushAcceleration		=	0;	
	m_iHeightJumpRush		=	0;	
	m_wIncreaseHeightJumpRush	=	0;	
	m_dwRushFrameCounter	=	0;
	m_iJumpRushRange		=	0;
	m_wJumpRushLastActionFrame	=	0;				//	점프러쉬 최종액션프레임
	m_wOverlapAnm			=	0xffff;
	m_wOverlapAnm2			=	0xffff;

	if (isHero())
		g_hero.offNotFocusStatus();

	m_posDest.x				=	m_pos.x;
	m_posDest.y				=	m_pos.y;
	m_posDest.mx			=	m_pos.mx;
	m_posDest.my			=	m_pos.my;
	m_wStopDirect			=	0xffff;

	m_wIsFreezeAtTriggerFrame=	FALSE;

	if (m_wChargeImage		!=	0xffff)	cancelChargeEffect();//	챠지 캔슬?

	if (isHero())
	{
//		g_hero.m_iWaitMoveResultTime=	0;
//		g_hero.m_wEngageTarget		=	0xffff;
	}

	if (m_cAfterImageStatus)	afterImageOff();

	if (m_wParallelBunshineCount)
	{
		afterImageOn(dFADE_AFTER_IMAGE,dAFTER_IMAGE_COUNT,20,1);
		
		if (m_wAwesomeFortressAction	==	0xffff)
			m_wParallelBunshineCount	=	0;
	}
	if (m_wDoppelgangerCount)
	{
		afterImageOn(dFADE_AFTER_IMAGE,dAFTER_IMAGE_COUNT,20,1);
		
		if (m_wContinuousHitOfDoppelgangerAction[0]	==	0xffff)
			m_wParallelBunshineCount	=	0;

	}


	if (m_wUseSkill	!=	0xffff)
	{
		if (m_wUseSkill	<	dACTIVE_SKILL_COUNT)	g_sm.casterStop(m_wUseSkill);

		m_wUseSkill		=	0xffff;
	}

	m_abilityUse.reset();

	m_iPostureOfDefense	=	0;
}


//
//	주변에 얼라들을 밀어낸다.
void
CActor::secureArea()
{
}


void
CActor::setPos(int x,int y)
{
	CPos		posOld;

	posOld.Set(m_pos.x,m_pos.y);

	m_pos.x			=	x;
	m_pos.y			=	y;

	m_pos.mx		=	m_pos.x>>dTILE_XSIZE_SHIFT;
	m_pos.my		=	m_pos.y>>dTILE_YSIZE_SHIFT;

	m_posDest.x		=	m_pos.x;
	m_posDest.y		=	m_pos.y;
	m_posDest.mx	=	m_pos.mx;
	m_posDest.my	=	m_pos.my;

	if	(m_wBiter	!=	0xffff)
	{
		int		iDX			=	m_pos.x-posOld.x;
		int		iDY			=	m_pos.y-posOld.y;

		CActor	*lpBiter	=	g_am.getTestedActor(m_wBiter);

		if	(lpBiter)
		{
			lpBiter->m_pos.x+=	iDX;
			lpBiter->m_pos.y+=	iDY;
		}
	}
}