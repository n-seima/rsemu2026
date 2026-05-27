#include "CHitEffect.H"
#include "CImageData.H"
#include "CGame.H"
#include "cMAP.H"
#include "cDebugWindow.H"

CHitEffectManager	g_hem;

const	c_aHitTextZoom[dHIT_INFO_TIME+1]	=	
{	
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,110,120,130,140,150,
	160,170,180,190,200,210,220,230,240,250,
	260,270,280
};

const	c_aCriticalHitTextZoom[dHIT_INFO_TIME+1]	=	
{	
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100,100,100,
	100,100,100,100,100,120,140,160,180,200,
	220,240,260,280,300,320,340,360,380,400,
	420,440,460
};

//
//	타격효과 프레임 갱신
BOOL
CHitEffect::update()
{
	if (m_wFrame	!=	0xffff)					//	이펙트가 있다.
	{
		if (m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_wFrameCounter	-=	dSYNC_FPS;
			m_wFrame++;
		}

		m_wFrameCounter	+=	m_wFPS;

		if (m_wFrame	>=	m_wMaxFrame)	m_wFrame	=	0xffff;
	}

	m_iHitInfoTime	--;//=	4;

	if (m_iHitInfoTime	<=	0)
		m_iHitInfoKind	=	0xffff;

	if (m_wFrame	==	0xffff	&&	m_iHitInfoKind	==	0xffff)
		return FALSE;

	return	TRUE;
}

//
//	Hit	Effect를 찍어준다.
void
CHitEffect::put(int x,int y)
{
	x	=	x	+	m_pos.x;
	y	=	y	+	m_pos.y;

	int	iScaleValue	=	1;

	if	(g_bIsHalfSize)
		iScaleValue	=	2;

	if	(m_wFrame	!=	0xffff)
	{
		CPos	pos;

		pos.Set(x,y);

		if	(m_wShakeValue)
		{
			pos.x	=	pos.x+random(m_wShakeValue*2)-m_wShakeValue;
			pos.y	=	pos.y+random(m_wShakeValue*2)-m_wShakeValue;
		}

//		g_apEffect[m_wEffect]->PutReg(pos.x,pos.y,m_wAnm,m_wDirect,m_wFrame,100,100);

		g_im.putShadow(m_wEffect,pos.x,pos.y,m_wAnm,m_wDirect,m_wFrame,m_wXScale/iScaleValue,m_wYScale/iScaleValue);
		g_im.putWhichUsePalette(m_wEffect,pos.x,pos.y,m_wPaletteIndex,m_wAnm,m_wDirect,m_wFrame,m_wXScale/iScaleValue,m_wYScale/iScaleValue,m_wOutputEffect);
	}

#ifdef	_DEBUG
	if	(g_bIsHideDamageAndHideName)
		return;
#endif

	if	(m_iHitInfoKind	!=	0xffff)
	{
		int	iWidth,iZoom;

//		if(m_iHitInfoKind	==	dHIT_INFO_CRITICAL)	iZoom	=	c_aCriticalHitTextZoom[m_iHitInfoTime];
//		else										
		iZoom			=	c_aHitTextZoom[m_iHitInfoTime];

		iWidth			=	8*iZoom/100;

		y				-=	(40 + (dHIT_INFO_TIME-m_iHitInfoTime));

		int	iHitInfoKind=	m_iHitInfoKind;

		if	(m_iHitInfoKind	<=	dHIT_INFO_MAGIC_NUMBER)
		{
			int	iLength	=	strlen(m_strHitInfo);
			x			-=	iLength*iWidth/2;

			for (int i=0;i<iLength;i++)
			{
				int	iIndex	=	m_strHitInfo[i]-'0'+iHitInfoKind;

				if	(m_strHitInfo[i]==	'+')
				{
					iHitInfoKind=	dHIT_INFO_MAGIC_NUMBER;
					x			-=	6;
					y			+=	8;
					continue;
				}

				if	(m_wIsHitByMonster)
					iIndex		+=	dHIT_BY_MONSTER_BORDER;

				g_sprHitText.Put(x,y,iIndex,iZoom,iZoom,dPUT_ALPHA_BLENDING,m_iHitInfoTime);

				x	+=	iWidth;
			}
		}
		else
		{
			if	(m_wIsHitByMonster)
				iHitInfoKind		+=	dHIT_BY_MONSTER_BORDER;

			g_sprHitText.Put(x,y,iHitInfoKind,iZoom,iZoom,dPUT_ALPHA_BLENDING,m_iHitInfoTime);
		}
	}
}

//
//	타격 효과를 전부 초기화한다.
void
CHitEffectManager::reset()
{
	m_iHitCount	=	0;
	m_iRookie	=	0;

	for (int i = 0 ; i < dMAX_HIT_EFFECT ; i ++)
		m_aHitEffect[i].m_wEffect	=	0xffff;
}

BOOL
CHitEffectManager::update(int _iIndex)
{
	if	(_iIndex	>=	dMAX_HIT_EFFECT	||	_iIndex	<	0)
		return	FALSE;

	return	m_aHitEffect[_iIndex].update();
}

int
CHitEffectManager::getRemainFrame(int _iIndex)
{
	if	(_iIndex	>=	dMAX_HIT_EFFECT	||	_iIndex	<	0)
		return	0;

	return	m_aHitEffect[_iIndex].getRemainFrame();
}

CHitEffect*
CHitEffectManager::get(int _iIndex)
{
	if	(_iIndex	>=	dMAX_HIT_EFFECT	||	_iIndex	<	0)
		return	NULL;

	return &m_aHitEffect[_iIndex];
}

void
CHitEffectManager::put(int _iIndex,int _iX,int _iY)
{
	if	(_iIndex	>=	dMAX_HIT_EFFECT	||	_iIndex	<	0)
		return;

	m_aHitEffect[_iIndex].put(_iX,_iY);
}

//
//	오라 추가
int
CHitEffectManager::addAura(int _iEffect,int _iOutputEffect,int _iPalette,int _iXScale,int _iYScale)
{
	int	iIndex	=	addEffect(0,0,_iEffect,0,0,0,_iXScale,_iYScale);

	if	(iIndex	==	0xffff)	
		return	0xffff;

	m_aHitEffect[iIndex].m_wPaletteIndex	=	_iPalette;
	m_aHitEffect[iIndex].m_wOutputEffect	=	_iOutputEffect;

	return	iIndex;
}

//
//	타격 효과를 추가한다.
int
CHitEffectManager::addEffect(int _iX,int _iY,int _iEffect,int _iDirect,int _iAnm,int _iShakeValue,int _iXScale,int _iYScale,BOOL _isHalfFaram)
{
	if	(m_iHitCount >= dMAX_HIT_EFFECT	)
		return 0xffff;
	if	(_iEffect	==	0xffff			)
		return 0xffff;

	CHitEffect	*lpHitEffect	=	&m_aHitEffect[m_iRookie];
	int			iAnm			=	random(g_apEffect[_iEffect]->m_iAnmCount);
	int			iDirectCount	=	g_apEffect[_iEffect]->m_pAnmData[iAnm].m_iDirectCount;

	if (g_apEffect[_iEffect]->m_isFlip)
		iAnm	=	0;
	if (_iAnm	!=	0xffff)
		iAnm	=	_iAnm;

	_iDirect					=	min(_iDirect,iDirectCount-1);
	lpHitEffect->m_wEffect		=	_iEffect;
	lpHitEffect->m_pos.x		=	_iX;
	lpHitEffect->m_pos.y		=	_iY;

	lpHitEffect->m_wFrame		=	0;
	lpHitEffect->m_wFrameCounter=	0;
	lpHitEffect->m_wPaletteIndex=	0xffff;
	lpHitEffect->m_wAnm			=	iAnm;
	lpHitEffect->m_wDirect		=	_iDirect;
	if(_isHalfFaram)
		lpHitEffect->m_wFPS			=	g_apEffect[_iEffect]->GetFPS(iAnm) / 2;
	else 
		lpHitEffect->m_wFPS			=	g_apEffect[_iEffect]->GetFPS(iAnm);
	lpHitEffect->m_wMaxFrame	=	g_apEffect[_iEffect]->GetFrameCount(iAnm);
	lpHitEffect->m_iHitInfoKind	=	0xffff;
	lpHitEffect->m_iHitInfoTime	=	dHIT_INFO_TIME;
	lpHitEffect->m_wShakeValue	=	_iShakeValue;
	lpHitEffect->m_wXScale		=	_iXScale;
	lpHitEffect->m_wYScale		=	_iYScale;
	lpHitEffect->m_wOutputEffect=	0xffff;

	m_iHitCount++;

	_iEffect	=	m_iRookie;

	for (int i = m_iRookie+1 ; i < dMAX_HIT_EFFECT ; i ++)
		if (m_aHitEffect[i].m_wEffect	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	return	_iEffect;
}

//
//	타격 효과와 타격 정보 모두 나타낸다.
int
CHitEffectManager::addEffectAndInfo(int _iX,int _iY,int _iEffect,int _iHitInfo,int _iPhysicalDamage,int _iMagicDamage,int _iDirect,BOOL _bIsOwnTeam,int _iAnm)
{
	if (_iPhysicalDamage	>	20000)
		_iPhysicalDamage	=	_iPhysicalDamage;

	if (_iEffect	==	0xffff)
		return	addInfo(_iX,_iY,_iHitInfo,_iPhysicalDamage,_iMagicDamage,_bIsOwnTeam);

	if (m_iHitCount >= dMAX_HIT_EFFECT)
		return 0xffff;

	CHitEffect	*lpHitEffect	=	&m_aHitEffect[m_iRookie];

	lpHitEffect->m_wIsHitByMonster=	_bIsOwnTeam;

	if (_iMagicDamage	>	20000)
		_iMagicDamage	=	_iMagicDamage;

	if (_iMagicDamage)
	{
		if (_iPhysicalDamage)
			strcpy(lpHitEffect->m_strHitInfo,_ms("%d+%d",_iPhysicalDamage,_iMagicDamage));
		else
		{
			_iHitInfo	=	dHIT_INFO_MAGIC_NUMBER;
			strcpy(lpHitEffect->m_strHitInfo,_ms("%d",_iMagicDamage));
		}
	}
	else
	{
		if (_iPhysicalDamage)
			strcpy(lpHitEffect->m_strHitInfo,_ms("%d",_iPhysicalDamage));
		else
		{
			if (_iHitInfo	<	dHIT_INFO_RED_MISS)
			{
				lpHitEffect->m_iHitInfoKind	=	dHIT_INFO_NO_DAMAGE;
				strcpy(lpHitEffect->m_strHitInfo,"");
			}
		}
	}

	_iEffect	=	addEffect(_iX,_iY,_iEffect,_iDirect,_iAnm);

	lpHitEffect->m_iHitInfoKind	=	_iHitInfo;

	return	_iEffect;
}

//
//	타격 정보만 나타낸다.
int
CHitEffectManager::addInfo(int x,int y,int _iHitInfo,int _iPhysicalDamage,int _iMagicDamage,BOOL _bIsOwnTeam)
{
	if (_iPhysicalDamage	>	20000)
		_iPhysicalDamage	=	_iPhysicalDamage;

	if	(m_iHitCount >= dMAX_HIT_EFFECT)
		return 0xffff;

	if	(_iHitInfo	==	dHIT_INFO_RED_MISS)
		_iHitInfo	=	dHIT_INFO_RED_MISS;

	CHitEffect	*lpHitEffect	=	&m_aHitEffect[m_iRookie];

	lpHitEffect->m_wEffect		=	0;
	lpHitEffect->m_pos.x		=	x;
	lpHitEffect->m_pos.y		=	y;
	lpHitEffect->m_wFrame		=	0xffff;
	lpHitEffect->m_wPaletteIndex=	0xffff;
	lpHitEffect->m_wIsHitByMonster=	_bIsOwnTeam;

	lpHitEffect->m_iHitInfoKind	=	_iHitInfo;
	lpHitEffect->m_iHitInfoTime	=	dHIT_INFO_TIME;
	lpHitEffect->m_wOutputEffect=	0xffff;

/*	if (_iHitInfo	<=	dHIT_INFO_MAGIC_NUMBER)
	{
		if (_iPhysicalDamage)	strcpy(lpHitEffect->m_strHitInfo,_ms("%d+%d",_iPhysicalDamage,_iMagicDamage));
		else
		{
			strcpy(lpHitEffect->m_strHitInfo,_ms("%d",_iMagicDamage));
			lpHitEffect->m_iHitInfoKind		=	dHIT_INFO_WHITE_NUMBER;
		}
	}
	else	strcpy(lpHitEffect->m_strHitInfo,"");
*/
	if (_iMagicDamage)
	{
		if (_iPhysicalDamage)	strcpy(lpHitEffect->m_strHitInfo,_ms("%d+%d",_iPhysicalDamage,_iMagicDamage));
		else
		{
			strcpy(lpHitEffect->m_strHitInfo,_ms("%d",_iMagicDamage));
			lpHitEffect->m_iHitInfoKind	=	dHIT_INFO_MAGIC_NUMBER;
		}
	}
	else
	{
		if (_iPhysicalDamage)	strcpy(lpHitEffect->m_strHitInfo,_ms("%d",_iPhysicalDamage));
		else
		{
			if (_iHitInfo	<	dHIT_INFO_RED_MISS)
			{
				lpHitEffect->m_iHitInfoKind	=	dHIT_INFO_NO_DAMAGE;
				strcpy(lpHitEffect->m_strHitInfo,"");
			}
		}
	}

	m_iHitCount++;

	_iHitInfo	=	m_iRookie;

	for (int i = m_iRookie+1 ; i < dMAX_HIT_EFFECT ; i ++)
		if (m_aHitEffect[i].m_wEffect	==	0xffff)
		{
			m_iRookie	=	i;
			break;
		}

	return	_iHitInfo;
}

//
//	타격 효과를 제거한다.
BOOL
CHitEffectManager::remove(int index)
{
	if	(index < 0 || index >= dMAX_HIT_EFFECT)
		return	FALSE;

	m_aHitEffect[index].m_wEffect	=	0xffff;

	if	(index < m_iRookie)
		m_iRookie	=	index;

	m_iHitCount--;

	return TRUE;
}

cHitInfoManager::cHitInfoManager()
{
	m_pHitEffect	=	NULL;
}

cHitInfoManager::~cHitInfoManager()
{
	pKILL(m_pHitEffect);
}

void
cHitInfoManager::init(int _iCount)
{
	m_wCount		=	_iCount;
	m_pHitEffect	=	new WORD [_iCount];

	memset(m_pHitEffect,0xff,sizeof(WORD)*_iCount);
}

int
cHitInfoManager::getFreeHitIndex()
{
	int	i;

	for (i=0;i<m_wCount;i++)
		if (m_pHitEffect[i] == 0xffff)
			return i;

	int	iMinRemainFrame	=	1000;
	int	iHitEffect		=	0xffff;

	for (i=0;i<m_wCount;i++)
		if (m_pHitEffect[i] != 0xffff)
		{
			int	iRemainFrame	=	g_hem.getRemainFrame(m_pHitEffect[i]);

			if (iRemainFrame	<	iMinRemainFrame)
			{	
				iHitEffect		=	i;
				iMinRemainFrame	=	iRemainFrame;
			}
		}

	g_hem.remove(m_pHitEffect[iHitEffect]);

	return	iHitEffect;
}

void
cHitInfoManager::addHitEffect(int _iEffect)
{
	int	iFreeHitEffectIndex	=	getFreeHitIndex();

	m_pHitEffect[iFreeHitEffectIndex]	=	_iEffect;
}