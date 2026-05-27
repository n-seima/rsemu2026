#include "CActiveSkill.H"
#include "cACTOR.H"
#include "CField.H"
#include "CGame.H"

//
//	회오리형 스킬 사용
int
CActiveSkill::fireGuardianPost()
{
	m_lpCaster->action(m_lpSkill->m_wAction);
	
	if	(m_lpCaster->m_wMachine	!=	0xffff)
		return	eSKILL_FIRE_RESULT_FAILED;

	int	iShotCount	=	max(m_ability.getBulletCount(),1);

	setGuardianPostBulletCount(iShotCount);
	
	setActiveGuardianPost(FALSE);
	setGuardianPostJobChangeStatus(FALSE);

	getActionTime();

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	가디언 포스트 발사?
BOOL
CActiveSkill::castGuardianPost()
{
	if	(m_lpCaster && m_lpCaster->m_wMachine	!=	0xffff)
		return	TRUE;

	cItem	*lpItem	=	m_lpCaster->getWeapon();

	int		iImage	=	0;

	if	(lpItem)
	{
		cBasicItem	*lpBasicItem	=	lpItem->getBasicItem(TRUE);

		if	(lpBasicItem)
			iImage	=	lpBasicItem->m_wEquippedShape;
		else
			return FALSE;
	}
	else
		return	FALSE;

	setActiveGuardianPost(TRUE);
	setGuardianPostImage(iImage);
	setGuardianPostStrikePeriod(m_ability.getStrikePeriod()*dSYNC_FPS/100);

	int	iCasterJob			=	m_lpCaster->m_wJob;

	m_lpCaster->m_wMachine	=	m_wSerial+0x8000;
	m_lpCaster->sendPlantGuardianPost(m_wSerial,m_pos.x,m_pos.y,iImage,m_ability.m_wSkill,m_ability.m_wLevel);

	if	(!m_lpCaster	||	m_lpCaster->getBody()	==	NULL)
		return	FALSE;

	DWORD	dwLastFrameTime			=	m_lpCaster->getFrameTime();
	DWORD	dwTriggerFrameTime		=	m_lpCaster->getTriggerTime();

	m_dwBookedActionTime			=	g_dwCurrentTime+(dwLastFrameTime-dwTriggerFrameTime)/dREVISE_ACTION_SPEED_FOR_TEST;

	if	(m_ability.isJobChange())
	{
		if	((iCasterJob%2)	==	0)
			m_lpCaster->m_wHideWeapon1Time			=	0xffff;
		else
			m_lpCaster->m_wHideWeapon2Time			=	0xffff;

		m_lpCaster->sendTransformation();
		setGuardianPostJobChangeStatus(TRUE);
	}

	return	TRUE;
}

//
//	가디언 포스트 발사?
BOOL
CActiveSkill::strikeGuardianPost()
{
	if	(m_lpCaster	==	NULL	||	m_lpCaster->m_wHideWeapon2Time)
		return	TRUE;

	cACTOR	*lpTarget	=	m_lpCaster->m_lpTarget;

	if	(lpTarget	==	NULL)
		return	FALSE;

	int	iRange		=	GetOvalRange(m_pos.x,m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y);
	int	iHitRange	=	m_ability.getHitRange();

	if	(iRange	>	iHitRange*iHitRange)
		return	FALSE;

	CHitInfo	hitInfo;

	hitInfo.reset(TRUE);

	lpTarget->getDamage(m_lpCaster,&m_ability,m_pos.x,m_pos.y,&hitInfo);

	CDamageActorInfoTrailer	info;

	info.reset();

	info.m_wCount					=	1;
	info.m_aData[0].m_wActorSerial	=	lpTarget->m_wSerialInField;
	memcpy(&info.m_aData[0].m_hitInfo,&hitInfo,sizeof(hitInfo));

	m_lpCaster->sendStrikeGuardianPostAttack(m_wSerial,m_pos.x,m_pos.y,getGuardianPostImage(),&info,m_ability.m_wSkill,m_ability.m_wLevel);

	m_lpCaster->strike(lpTarget,&hitInfo,&m_ability);

	setGuardianPostStrikePeriod(m_ability.getStrikePeriod()*dSYNC_FPS/100);

	return	decreaseGuardianPostBulletCount();
}

//
//	회오리형 업데이트
BOOL
CActiveSkill::updateGuardianPost()
{
	if	(m_lpCaster	==	NULL)
		return	TRUE;

	if	(isActiveGuardianPost())
	{
		if	(decreaseGuardianPostStrikePeriod())
			if	(strikeGuardianPost())
				return	TRUE;

		return	FALSE;
	}

	if	(isActionTime())
	{
		if	(!castGuardianPost())
			return	TRUE;

		return	FALSE;
	}

	return	FALSE;
}