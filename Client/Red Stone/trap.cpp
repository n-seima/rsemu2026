#include "trap.H"
#include "cActor.H"

cTrapManager	g_trapManager;

void
cTrap::draw(int _iDX,int _iDY,int _iFrameCounter,BOOL _bIsFocus)
{
	int		iX			=	GetScaledXPos(m_pos.x);
	int		iY			=	GetScaledYPos(m_pos.y);

	cANM	*lpTrapImage=	g_im.getEffect(g_aSkill[m_wSkill].m_wSkillImage);

	int		iMaxFrame	=	lpTrapImage->GetFrameCount(0);
	int		iFPS		=	lpTrapImage->GetFPS(0);
	int		iFrame		=	(_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame;

	lpTrapImage->PutShadow(iX,iY,0,0,iFrame,g_aSkill[m_wSkill].m_wImageScale*g_iGameScale/100,g_aSkill[m_wSkill].m_wImageScale*g_iGameScale/100);

	WORD	*lpPlt		=	lpTrapImage->getOrgPalette();
	WORD	awNewPalette[256];

	if	(_bIsFocus)
	{
		EffectPalette(dPLT_EFFECT_RED,lpTrapImage->getOrgPalette(),awNewPalette,140,0);
		lpPlt	=	awNewPalette;
	}

	lpTrapImage->setPalette(lpPlt);
	lpTrapImage->PutReg(iX,iY,0,0,iFrame,g_aSkill[m_wSkill].m_wImageScale*g_iGameScale/100,g_aSkill[m_wSkill].m_wImageScale*g_iGameScale/100);
	lpTrapImage->restorePalette();
}

void
cTrap::copy(cTrap *_lpTrap)
{
	memcpy(this,_lpTrap,sizeof(cTrap));
}

cTrapManager::cTrapManager()
{
}

cTrapManager::~cTrapManager()
{
	m_den.close();
}

void
cTrapManager::reset()
{
	m_iFrameCounter	=	0;
	m_den.reset();
}

int
cTrapManager::addTrap(cTrap *_lpTrap)
{
	return	m_den.insert(_lpTrap->m_wSerial,_lpTrap);
}

BOOL
cTrapManager::removeTrap(int _iSerial)
{
	return	m_den.remove(_iSerial);
}

int	
cTrapManager::getViewTraps(cINDEX *_lpIndexes,cRECT *_lpRectViewport)
{
	int	iCount	=	0;

	for (int i=0;i<getCount();i++)
	{
		cTrap	*lpTrap	=	m_den.getByOrdering(i);

		if	(lpTrap == NULL)
			continue;

		if	(g_aSkill[lpTrap->m_wSkill].m_bf1IsFlatTrap)
			continue;

		if	(lpTrap->m_bf3Team	!=	g_hero.m_wTeam)
			if	(g_hero.getDetectTrapLevel()	<	lpTrap->m_bf8DetectLevel)
				continue;

		if	(_lpRectViewport->isIn(lpTrap->m_pos.x,lpTrap->m_pos.y))
		{
			_lpIndexes[iCount].Index	=	dBORDER_TRAP+lpTrap->m_wSerial;
			_lpIndexes[iCount].Value	=	lpTrap->m_pos.y;
			iCount++;
		}
	}

	return	iCount;
}

int	
cTrapManager::getFocusTrap(int _iX,int _iY)
{
	int	iCount	=	0;

	if	(g_hero.getDisarmTrapAbility()	==	NULL)
		return	0xffff;

	for (int i=0;i<getCount();i++)
	{
		cTrap	*lpTrap	=	m_den.getByOrdering(i);

		if	(lpTrap == NULL)
			continue;

		if	(lpTrap->m_bf3Team	!=	g_hero.m_wTeam)
			if	(g_hero.getDetectTrapLevel()	<	lpTrap->m_bf8DetectLevel)
				continue;

		int	iRange	=	lpTrap->m_wRange;

		if	(GetOvalRange(_iX,_iY,lpTrap->m_pos.x,lpTrap->m_pos.y)	<=	iRange*iRange)
			return	lpTrap->m_wSerial;
	}

	return	0xffff;
}

void
cTrapManager::drawFlatTraps(cRECT *_lpRectViewport,int _iDX,int _iDY)
{
	int	iCount	=	0;

	for (int i=0;i<getCount();i++)
	{
		cTrap	*lpTrap	=	m_den.getByOrdering(i);

		if	(lpTrap == NULL)
			continue;

		if	(g_aSkill[lpTrap->m_wSkill].m_bf1IsFlatTrap	==	FALSE)
			continue;

		if	(lpTrap->m_bf3Team	!=	g_hero.m_wTeam)
			if	(g_hero.getDetectTrapLevel()	<	lpTrap->m_bf8DetectLevel)
				continue;

		if	(_lpRectViewport->isIn(lpTrap->m_pos.x,lpTrap->m_pos.y))
		{
			BOOL	bIsFocus	=	FALSE;

			if	(lpTrap->m_wSerial	==	m_iFocusTrap)
				bIsFocus	=	TRUE;

			lpTrap->draw(_iDX,_iDY,m_iFrameCounter+i,bIsFocus);
		}
	}
}

void
cTrapManager::drawTrap(int _iSerial,int _iDX,int _iDY)
{
	cTrap	*lpTrap	=	get(_iSerial);

	if	(lpTrap == NULL)
		return;

	BOOL	bIsFocus	=	FALSE;

	if	(_iSerial	==	m_iFocusTrap)
		bIsFocus	=	TRUE;

	if	(lpTrap->m_bf3Team	!=	g_hero.m_wTeam)
		if	(g_hero.getDetectTrapLevel()	<	lpTrap->m_bf8DetectLevel)
			return;

	lpTrap->draw(_iDX,_iDY,m_iFrameCounter,bIsFocus);
}

void
cTrapManager::update()
{
	m_iFrameCounter++;

	cTrap	*lpTrap1	=	get(6);
	cTrap	*lpTrap2	=	get(1);

	if	(lpTrap1	&&	lpTrap2)
	{
		int	iRange		=	GetOvalRange(lpTrap1->m_pos.x,lpTrap1->m_pos.y,lpTrap2->m_pos.x,lpTrap2->m_pos.y);

		iRange			=	(int)sqrt(iRange);
	}
}