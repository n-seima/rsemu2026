//	actor 와의 거리를 구한다.
inline	int
CActor::getRange(CActor *_lpActor)
{
	if (!_lpActor)	return	0x7fffffff;

	return	GetOvalRange(m_pos.x,m_pos.y,_lpActor->m_pos.x,_lpActor->m_pos.y);
}

//
//	거리를 구한다.
//	CActor	*actor	-	목표 캐릭터
inline	int
CActor::getRange(CActor *_lpActor,int _iAddLength)
{
	int	xs	=	m_pos.x	-	_lpActor->m_pos.x;
	int	ys	=	m_pos.y	-	_lpActor->m_pos.y;

	if (xs < 0 ) xs = -xs;
	if (ys < 0 ) ys = -ys;

	ys	*=	2;
	xs += _iAddLength;
	ys += _iAddLength;

	if (xs >= 0xafff)	return	0x7ffffff;
	if (ys >= 0xafff)	return	0x7ffffff;

	return	(xs*xs+ys*ys);
}


//
//	pos에서 충돌하는 캐릭터 구함
inline int
CActor::getCrashActor(c4POINT *pos)
{
	int		actor	=	0;

//	actor		=	GetTileActor(pos->mx,pos->my);

	return actor;
}

//
//	공격할 수 없는 타겟
inline	BOOL
CActor::isCanNotAttack()
{
	if	(m_isSkillControl)
		return	TRUE;	//	스킬 컨트롤 상태다.
	if	(m_wIsFeignDeath)
		return	TRUE;	//	죽은척 하고 있다.
	if	(isDeath())	
		return	TRUE;	//	... 죽었다.

	return	FALSE;
}

//
//	방패가 보이냐?
inline	BOOL
CActor::isVisibleShield()
{
	if	(m_wJob	>=	dPLAYER_JOB_COUNT)
		return	FALSE;
	if	(m_wIsHideShield						)
		return	FALSE;
	if	(!c_aUseShieldJob[m_wJob]				)
		return	FALSE;	//	방패를 장비 할 수 있는 직업이 아니다.(보이는 -_-)
	if	(m_itemShield.m_wBaseItem	==	0xffff	)
		return	FALSE;
	if	(m_wIsCastBlocker						)
		return	FALSE;

	return TRUE;
}


//	무기를 장비하고 있냐?
inline	BOOL
CActor::isVisibleWeapon()
{
	if	(m_wJob	>=	dPLAYER_JOB_COUNT)
		return	FALSE;
	if	(!c_aVisibleWeaponJob[m_wJob]			)
		return	FALSE;	//	방패를 장비 할 수 있는 직업이 아니다.(보이는 -_-)
	if	(m_itemWeapon.m_wBaseItem	==	0xffff	)
		return	FALSE;

	return TRUE;
}