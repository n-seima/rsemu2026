#include	"ast_fire_wall.h"
#include	"cACTOR.h"

int
cAS_FireWall::fire()
{
	m_lpCaster->action(m_posTarget.x,m_posTarget.y,m_lpSkill->m_wAction);

	m_wFrameCounter	=	0xffff;
	m_wGrade		=	eReady;

	m_wX1			=	0xffff;
	m_wY1			=	0xffff;
	m_wX2			=	0xffff;
	m_wY2			=	0xffff;

	return	eSKILL_FIRE_RESULT_OK;
}

BOOL
cAS_FireWall::makeSmallFlame(int _iX1,int _iY1,int _iX2,int _iY2)	//	작은 불꽃 만들기
{
	m_wX1	=	_iX1;
	m_wY1	=	_iY1;
	m_wX2	=	_iX2;
	m_wY2	=	_iY2;

	CPos	aRoad[100];

	int		iNodeCount	=	GetLinerPath(_iX1,_iY1,_iX2,_iY2,aRoad,100);

	for(int i=iNodeCount;i;)
	{
		--i;

		CPos	pos;

		pos.x	=	aRoad[i].x*dTILE_XS+dTILE_XS/2;
		pos.y	=	aRoad[i].y*dTILE_YS+dTILE_YS/2;

		CEffectInfo*	lpEffect		=	g_standEffect.add(pos.x,pos.y,m_lpSkill->m_wSkillImage,0,FALSE,0,1,TRUE,m_wSerial);
		if(lpEffect)
		{
			lpEffect->m_wBeginFrameConter	=	random(10000);
			lpEffect->m_iFrameCounter		=	lpEffect->m_wBeginFrameConter;
			lpEffect->m_wImageScale			=	m_lpSkill->m_wImageScale;
		}
	}

	return	TRUE;
}

BOOL
cAS_FireWall::makeFireWall(int _iX1,int _iY1,int _iX2,int _iY2)		//	불꽃 장벽 만들기
{
	m_wX1	=	_iX1;
	m_wY1	=	_iY1;
	m_wX2	=	_iX2;
	m_wY2	=	_iY2;

	g_map.makeBlock(m_wX1,m_wY1,m_wX2,m_wY2);
	g_standEffect.changeAnmByDiscernCode(m_wSerial,0);

	return	TRUE;
}

BOOL
cAS_FireWall::removeFireWall()	//	불꽃 장벽 제거
{
	g_map.removeBlock(m_wX1,m_wY1,m_wX2,m_wY2);
	g_standEffect.removeEffectByDiscernCode(m_wSerial);

	return	TRUE;
}

BOOL
cAS_FireWall::update()
{
	return	FALSE;
}
