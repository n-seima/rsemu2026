#include "CSkill.H"
#include "CActor.H"
#include "CSound.H"

//
//	보조마법
int
cAS_LightningWinder::fire()
{
	m_lpCaster->action(m_lpTarget->m_pos.x,m_lpTarget->m_pos.y,m_lpSkill->m_wAction);

	m_wIsFinished	=	FALSE;

	return	eSKILL_FIRE_RESULT_OK;
}

//
//	캐스팅
void
cAS_LightningWinder::shoot(CActor *_lpTarget,int _iPhysicalDamage,int _iMagicDamage,DWORD _dwResult)
{
	CHitInfo	hitInfo;

	hitInfo.m_dwPhysicalDamage	=	_iPhysicalDamage;
	hitInfo.m_dwMagicDamage		=	_iMagicDamage;
	hitInfo.m_dwResultField		=	_dwResult;

	int	iCasterArrowHeight		=	m_lpCaster->getHeight()+m_lpCaster->getBodyHeight()*2/3;
//	g_sm.castExplosionMissile(m_lpCaster,_lpTarget,_lpTarget->m_pos.x,_lpTarget->m_pos.y,&m_ability);
	g_sm.castSpecialMissile(m_lpCaster,_lpTarget,m_pos.x,m_pos.y-iCasterArrowHeight,&hitInfo,&m_ability);
}

//
//	보조마법
BOOL
cAS_LightningWinder::update()
{
	return	m_wIsFinished;
}