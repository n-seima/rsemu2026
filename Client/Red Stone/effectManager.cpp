#include "effectManager.H"
#include "CImageData.H"
#include "cMAP.H"

CEffectManager	g_rapeEffect,g_standEffect,g_floatEffect;

//
//	업데이트 하기
BOOL
CEffectInfo::update()
{
	int	iFPS		=	g_apEffect[m_wBody]->GetFPS(m_wAnm);

	m_iFrameCounter++;
	m_wFrame		=	(m_iFrameCounter*iFPS/dSYNC_FPS);

	if	(m_wIsLoopEffect)
		return	FALSE;

	if	(m_wFrame	>=	m_wFrameCount)
	{
		m_wFrame	=	0;
		return	TRUE;
	}

	return	FALSE;
}

void
CEffectInfo::setAnm(int _iAnm)
{
	m_wAnm			=	_iAnm;
	m_wFrame		=	0;
	m_iFrameCounter	=	m_wBeginFrameConter;
	m_wFrameCount	=	g_apEffect[m_wBody]->GetFrameCount(m_wAnm);
}

//
//	찍어주기
void
CEffectInfo::put()
{
	if	(m_wSerial	==	0xffff)
		return;

	if	(m_wIsScreenPos	==	FALSE)
	{
		int	iX		=	GetScaledXPos(m_pos.x);
		int	iY		=	GetScaledYPos(m_pos.y);
		int	iFrame	=	m_wFrame%m_wFrameCount;

		if	(m_wIsRandomFrame)
		{
			iFrame	=	random(m_wFrameCount);

			iX		=	iX+random(4)-2;
			iY		=	iY+random(4)-2;
		}

		int	iScale	=	g_iGameScale*m_wImageScale/100;

		g_apEffect[m_wBody]->PutShadow(iX,iY,m_wAnm,m_wDirect,iFrame,iScale,iScale);
		g_apEffect[m_wBody]->PutReg(iX,iY,m_wAnm,m_wDirect,iFrame,iScale,iScale);
	}
	else
	{
		int	iX		=	m_pos.x;
		int	iY		=	m_pos.y;
		int	iFrame	=	m_wFrame%m_wFrameCount;

		if	(m_wIsRandomFrame)
		{
			iFrame	=	random(m_wFrameCount);

			iX		=	iX+random(4)-2;
			iY		=	iY+random(4)-2;
		}

		int	iScale	=	g_iGameScale*m_wImageScale/100;

		g_apEffect[m_wBody]->PutShadow(iX,iY,m_wAnm,m_wDirect,iFrame,iScale,iScale);
		g_apEffect[m_wBody]->PutReg(iX,iY,m_wAnm,m_wDirect,iFrame,iScale,iScale);
	}
}

CEffectManager::CEffectManager()
{
	close();
}

//
//	초기화
void
CEffectManager::close()
{
	m_iCount	=	0;
	m_iNewSerial=	0;

	for (int i=dEFFECT_COUNT;i;i)
	{
		--i;

		m_aEffect[i].m_wSerial	=	0xffff;
	}
}


///	정적 멤버
CEffectInfo*
CEffectManager::add(int x,int y,int body,int range,BOOL _bIsWantRandomFrame,int _iDirect,int _iAnm,BOOL _bIsLoop,int _iDiscernCode,int _iImageScale,BOOL _bIsScreenPos)
{
	if	(m_iCount	>=	dEFFECT_COUNT	)
		return NULL;
	if	(body		==	0xffff			)
		return NULL;

	if	(_iDirect	>=	g_apEffect[body]->GetDirectCount(_iAnm))
		_iDirect	=	g_apEffect[body]->GetDirectCount(_iAnm)	-	1;

	CEffectInfo	*lpInfo	=	&m_aEffect[m_iNewSerial];
	
	lpInfo->m_pos.x			=	x + random(range) -(range/2);
	lpInfo->m_pos.y			=	y + random(range) -(range/2);
	lpInfo->m_wBody			=	body;
	lpInfo->m_wSerial		=	m_iNewSerial;
	lpInfo->m_wDirect		=	_iDirect;
	lpInfo->m_wIsRandomFrame=	_bIsWantRandomFrame;
	lpInfo->m_wIsLoopEffect	=	_bIsLoop;
	lpInfo->m_wDiscernCode	=	_iDiscernCode;
	lpInfo->m_wImageScale	=	_iImageScale;
	lpInfo->m_wBeginFrameConter	=	0;
	lpInfo->m_wIsScreenPos	=	_bIsScreenPos;

	lpInfo->setAnm(_iAnm);

	m_iCount++;

	for (int i=m_iNewSerial+1;i<dEFFECT_COUNT;i++)
	{
		if	(m_aEffect[i].m_wSerial	==	0xffff)
		{
			m_iNewSerial	=	i;

			break;
		}
	}

	return lpInfo;
}

void
CEffectManager::remove(int index)
{
	if	(m_aEffect[index].m_wSerial	==	0xffff)
		return;

	m_aEffect[index].m_wSerial	=	0xffff;
	m_iCount--;

	if (index < m_iNewSerial)	m_iNewSerial	=	index;
}

void
CEffectManager::removeEffectByDiscernCode(int _iDiscernCode)
{
	int	i;

	for (i=dEFFECT_COUNT;i;)
	{
		--i;

		if	(m_aEffect[i].m_wSerial==	0xffff)
			continue;

		if	(m_aEffect[i].m_wDiscernCode	==	_iDiscernCode)
			remove(i);
	}
}

void
CEffectManager::changeAnmByDiscernCode(int _iDiscernCode,int _iAnm)
{
	int	i;

	for (i=dEFFECT_COUNT;i;)
	{
		--i;

		if	(m_aEffect[i].m_wSerial==	0xffff)
			continue;

		if	(m_aEffect[i].m_wDiscernCode	==	_iDiscernCode)
			m_aEffect[i].setAnm(_iAnm);
	}
}

void
CEffectManager::update()
{	
	int	i;

	for (i=0;i<dEFFECT_COUNT; i++)
	{
		if	(m_aEffect[i].m_wSerial==	0xffff)
			continue;

		if	(m_aEffect[i].update())
			remove(i);
	}
}

//
//	전부 그려!!
void
CEffectManager::putAll()
{	
	int	i;

	for (i=0;i<dEFFECT_COUNT; i++)
	{
		if (m_aEffect[i].m_wSerial==	0xffff)	continue;

		m_aEffect[i].put();
	}
}

//
void
CEffectManager::put(int _iIndex)
{	
	if (m_aEffect[_iIndex].m_wSerial==	0xffff)	
		return;

	m_aEffect[_iIndex].put();
}

int	
CEffectManager::getViewEffect(cINDEX *_lpIndexes,cRECT *_lpRectViewport)
{
	int	iCount	=	0;

	for (int i=0;i<dEFFECT_COUNT;i++)
	{
		if (m_aEffect[i].m_wSerial == 0xffff)
			continue;

		if (_lpRectViewport->isIn(m_aEffect[i].m_pos.x,m_aEffect[i].m_pos.y))
		{
			_lpIndexes[iCount].Index	=	dBORDER_EFFECT+i;
			_lpIndexes[iCount].Value	=	m_aEffect[i].m_pos.y;
			iCount++;
		}
	}

	return	iCount;
}
