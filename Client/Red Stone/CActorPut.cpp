#include <MATH.H>
#include "CActor.H"
#include "cMAP.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "CFieldItem.H"
#include "cSHAKE.H"
#include "CImageData.H"
#include "CGuild.H"
#include "CColorEffect.H"
#include "CPitchmanShop.H"
#include "CMessage.H"
#include "cDebugWindow.H"
#include "duel.H"

#define	dINVISIBLE_ALPHA_DEPTH		4
#define	dBLUR_ALPHA_DEPTH			8

#define	dACT_SAGE_MASTER_IRON_BALL			10//	개타고 하는 액션 수
#define	dACT_SAGE_MASTER_LIGHT_BALL			11//	개타고 하는 액션 수
#define	dACT_SAGE_MASTER_LIGHTNING_BARRIER	12//	개타고 하는 액션 수

//
//	캐릭터 보다 먼저 찍어야할 디버그 정보
inline void
CActor::putDebugInfo1(int x,int y)
{
	if (s_isPaintBlockedTile)
	{
		int	iBodySize	=	getBodySize()	-	8;

		int	iX			=	m_pos.x/dTILE_XS*dTILE_XS-g_map.m_pos.x;
		int	iY			=	m_pos.y/dTILE_YS*dTILE_YS-g_map.m_pos.y;

		cDRAW::FillHB(_BLUE	,iX,iY,iX+dTILE_XS-1,iY+dTILE_YS-1);

		cDRAW::OvalCircle(_LTRED,x,y,getBodySize());
		cDRAW::Fill(_LTRED,x-1,y-1,x+1,y+1);
	}

}

//
//	캐릭터 위에 씌울-_-? 디버그 정보
//
inline void
CActor::putDebugInfo2(int x,int y)
{
	if (s_isPutActorInfo)
	{
		s_text.cPut(x,y-getBodyHeight()	,WHITE,_ms("%s[%d]",m_strName,m_wSerial));

		if (m_wSerial	==	s_iFocusActor)
		{
			x	=	x+getBodyWidth()/2;	
			y	=	y-getBodyHeight()+12;	

			char	*aAiState[]	=	{"대기","접근","액션"};

			cDRAW::FillHB(0,x-1,y-1,x+140,y+90);

//			s_text.put(x,y    ,WHITE,_ms("HP : %d/%d",m_iHP,m_iMaxHP));
//			s_text.put(x,y+=12,WHITE,_ms("CP : %d/%d",m_iCP,m_iMaxCP));
//			s_text.put(x,y    ,WHITE,_ms("Move State   : 0x%x",m_iMoveState			));
//			s_text.put(x,y+=12,_ms("AI           : %s",aAiState[m_wAiState]);
//			s_text.put(x,y+=12,WHITE,_ms("AI State     : %s",aAiState[m_wAiState]	));
//			s_text.put(x,y+=12,WHITE,_ms("Next AI Time : %d",m_sNextAiTime			));
//			s_text.put(x,y+=12,WHITE,_ms("Target       : %d",m_wTarget				));

//			s_text.put(x,y+=12,WHITE,_ms("Stun Time    : %d",m_sStunTime			));
//			s_text.put(x,y+=12,WHITE,_ms("Freeze Time  : %d",m_sFreezeTime			));

			if(m_sConfuseTime)
				s_text.put(x,y+=12,WHITE,_ms("Confuse Time : %d",m_sConfuseTime			));
		}
		else
		{
//			s_text.cPut(x,y+14				,WHITE,_ms("%d,%d,%d",m_sStunTime,m_sFreezeTime,m_sConfuseTime));
		}
	}
//	s_text.put(x,y,RGB(255,255,255),_ms("%d,%d",m_pos.x,m_pos.y));
}


//////////////////////////////////////////////////////////////////////////////
//	팔레트 설정
//////////////////////////////////////////////////////////////////////////////

void
CActor::setSageBookPalette()
{
	static	WORD	awPlt[256];
	WORD	*lpOrgPlt	=	g_anmSageStone.getOrgPalette();

	if	(isInvincible())
	{
		EffectPalette(dPLT_EFFECT_GRAY,lpOrgPlt,awPlt,150,0);

		g_anmSageStone.setPalette(awPlt);
	}
	else
	switch(m_wSageBookColor)
	{
		case	eSage_book_color_white_blue	:
		{
			WORD	wDestColor;

			int		iRange	=	50;
			int		iValue	=	(s_iFrameCounter+m_wSerial)*16%(iRange*4);

			if	(iValue		>	iRange*2)
			{
				iValue		-=	iRange*2;
				wDestColor	=	_WHITE;
			}
			else
				wDestColor	=	_LTBLUE;

			if	(iValue		>	iRange)
				iValue		=	iRange*2-iValue;

			EffectPalette(dPLT_EFFECT_COLORIZE,lpOrgPlt,awPlt,iValue,wDestColor);

			g_anmSageStone.setPalette(awPlt);
			break;
		}

		case	eSage_book_color_red		:
		{
			int	iIntensity	=	100;
			int	iSpeed		=	400;
			int	iValue		=	((s_iFrameCounter+m_wSerial)*iSpeed/100)%(iIntensity*2);
			int	iREd		=	_RED;

			if	(iValue	>	iIntensity)
				iValue	=	iIntensity*2-iValue;

			EffectPalette(dPLT_EFFECT_RED,lpOrgPlt,awPlt,10+iValue,0);

			g_anmSageStone.setPalette(awPlt);
			break;
		}

		case	eSage_book_color_blue		:
		{
			int	iValue	=	(s_iFrameCounter%80);

			if	(iValue	>	40)
				iValue	=	40-(iValue-40);

			iValue		+=	20;
			
			EffectPalette(dPLT_EFFECT_COLORIZE,lpOrgPlt,awPlt,iValue,_LTBLUE);
			g_anmSageStone.setPalette(awPlt);
			break;
		}
	}
}

//
//	무기의 팔레트
//
inline	void
CActor::setWeaponPalette()
{
	static	WORD	s_aTempPlt[256];

	cANM	*lpWeapon	=	getWeaponAnm();

	if	(!lpWeapon)
		return;

	if	(!isVisibleWeapon())
		m_itemWeapon.m_wColorizeEffect	=	0xffff;

	WORD	*lpOrgPlt	=	lpWeapon->getOrgPalette();

	int		iValue;
	int		iRange;
	WORD	wDestColor;

	if	(m_itemWeapon.m_wColorizeEffect	!=	0xffff)
	{
		if	(m_itemWeapon.m_wColorizeEffect	==	ePOE_PRINCESS_WEAPON)
		{
			int	iValue	=	(s_iFrameCounter%80);

			if	(iValue	>	40)
				iValue	=	40-(iValue-40);

			iValue		+=	20;
			
			EffectPalette(dPLT_EFFECT_COLORIZE,lpOrgPlt,s_aTempPlt,iValue,RGB24To16(LTPINK));
		}
		else
		{
			CPrefixColorizeEffectInfo	*lpEffect	=	&g_colorEffect.m_aPrefixColorizeEffect[m_itemWeapon.m_wColorizeEffect];

			switch(lpEffect->m_wMethod)
			{
				case	ePCEM_BRIGHT	:
					iValue	=	(s_iFrameCounter+m_wSerial)*lpEffect->m_wSpeed/100%lpEffect->m_wIntensity*2;

					if (iValue	>	lpEffect->m_wIntensity)
						iValue	=	lpEffect->m_wIntensity*2-iValue;

					EffectPalette(dPLT_EFFECT_BRIGHT,lpOrgPlt,s_aTempPlt,100+iValue,0);
					break;

				case	ePCEM_COLORIZE:
					iRange		=	lpEffect->m_wIntensity;
					iValue		=	(s_iFrameCounter+m_wSerial)%(iRange*4);

					if	(iValue	>	iRange*2)
					{
						iValue		-=	iRange*2;
						wDestColor	=	lpEffect->m_wColor1;
					}
					else
						wDestColor	=	lpEffect->m_wColor2;

					if	(iValue	>	iRange)
						iValue	=	iRange*2-iValue;

					EffectPalette(dPLT_EFFECT_COLORIZE,lpOrgPlt,s_aTempPlt,iValue,wDestColor);
					break;
			}
		}

		lpWeapon->setPalette(s_aTempPlt);
	}
	else
		lpWeapon->m_sprite.restorePalette();

	if (isPlayer() && m_strName[0] && g_am.isDenyCommunityActor(m_strName))
	{
		memset(s_aTempPlt,0,sizeof(s_aTempPlt));

		lpWeapon->setPalette(s_aTempPlt);
	}
	else
	if (m_bf1IsQuizEventWinner)
	{
		memset(s_aTempPlt,0xff,sizeof(s_aTempPlt));

		lpWeapon->setPalette(s_aTempPlt);
	}
	else
	if (m_bf1IsQuizEventLoser)
	{
		memset(s_aTempPlt,0,sizeof(s_aTempPlt));

		lpWeapon->setPalette(s_aTempPlt);
	}

	if(m_enchantedImage.m_bf1IsRageSoul)
	{		// 영의 분노... 일경우.. 칼팔레트 바꾼당..
		iRange		=	80;
		iValue		=	(s_iFrameCounter+m_wSerial)%(iRange*2);
		
		if	(iValue	>	iRange*2)
			iValue		-=	iRange*2;
		else
			wDestColor	=	0;
		
		if	(iValue	>	iRange)
			iValue	=	iRange*2-iValue;
		
		EffectPalette(dPLT_EFFECT_BRIGHT,lpOrgPlt,s_aTempPlt,iValue,0);
		lpWeapon->setPalette(s_aTempPlt);
	}
/*
	{
		int		iRange	=	50;
		WORD	wDestColor;
		iValue		=	s_iFrameCounter%(iRange*4);

		if (iValue	>	iRange*2)
		{
			iValue		-=	iRange*2;
			wDestColor	=	_LTRED;
		}
		else
			wDestColor	=	RGB24To16(200,140,0);
			
		if (iValue	>	iRange)
			iValue	=	iRange*2-iValue;

		EffectPalette(dPLT_EFFECT_COLORIZE,lpOrgPlt,s_aTempPlt,iValue,wDestColor);
	}
*/	
/*
	{
		iValue	=	s_iFrameCounter%80;

		if (iValue	>	40)
			iValue	=	80-iValue;

		EffectPalette(dPLT_EFFECT_COLORIZE,lpOrgPlt,s_aTempPlt,20+iValue,_WHITE);
	}
*/

/*
	{
		iValue	=	(s_iFrameCounter*4)%800;

		if (iValue	>	200)
			memcpy(s_aTempPlt,lpOrgPlt,sizeof(s_aTempPlt));
		else
		{
			if (iValue	>	100)
				iValue	=	200-iValue;

			EffectPalette(dPLT_EFFECT_BRIGHT,lpOrgPlt,s_aTempPlt,100+iValue*150/100,0);
		}
	}
*/
	
}

//
//	방패의 팔레트
inline	void
CActor::setShieldPalette()
{
	static	WORD	s_aTempPlt[256];

	if (!isVisibleShield()	)
		return;

	int		iEquimentShape	=	m_itemShield.getBasicItem()->m_wEquippedShape;

	if (iEquimentShape	==	0xffff)
		return;

	if (isPlayer() && m_strName[0] && g_am.isDenyCommunityActor(m_strName))
	{
		memset(s_aTempPlt,0,sizeof(s_aTempPlt));

		g_aShield[m_wJob][iEquimentShape].setPalette(s_aTempPlt);
	}
	else
	if (m_bf1IsQuizEventWinner)
	{
		memset(s_aTempPlt,0xff,sizeof(s_aTempPlt));

		g_aShield[m_wJob][iEquimentShape].setPalette(s_aTempPlt);
	}
	else
	if (m_bf1IsQuizEventLoser)
	{
		memset(s_aTempPlt,0,sizeof(s_aTempPlt));

		g_aShield[m_wJob][iEquimentShape].setPalette(s_aTempPlt);
	}
	else	
		g_aShield[m_wJob][iEquimentShape].m_sprite.restorePalette();
}

//////////////////////////////////////////////////////////////////////////////
//	무기
//////////////////////////////////////////////////////////////////////////////

cANM*
CActor::getWeaponAnm()
{
	cANM	*lpWeapon	=	NULL;

	if	(!isVisibleWeapon())
	{
		if	(m_wJob	==	dJOB_FALLEN_ANGEL)
			lpWeapon	=	GetWeapon(m_wJob,0);
	}
	else
	{
		int	iEquimentShape	=	m_itemWeapon.getBasicItem()->m_wEquippedShape;

		if	(iEquimentShape	==	0xffff)
			return	NULL;

		lpWeapon	=	GetWeapon(m_wJob,iEquimentShape);
	}

	return	lpWeapon;
}


inline void
CActor::putWeapon(int x,int y,cANM *_lpWeapon, int _iDirect,WORD _wAnm,int _iAlphaDepth)
{
	if	(m_wBody	>=	100 || m_bIsTransOther || isTransformationMonster() )		//  무기해제상태이면.. 뿌리지않는다...
		return;

	int		iDirect	= _iDirect;
	WORD	wAnm	= _wAnm;
	if	(isInvisivle())
	{
		if	(_iAlphaDepth == 32)
			if	(isRequireAddPaint())
				return;

		_iAlphaDepth	=	dINVISIBLE_ALPHA_DEPTH;

		if	(isOwnTeam())
			_iAlphaDepth=	16;
	}
	else
	if	(isBlur())
	{
		if	(_iAlphaDepth == 32)
			if	(isRequireAddPaint())
				return;

		_iAlphaDepth	=	dBLUR_ALPHA_DEPTH;

		if	(isOwnTeam())
			_iAlphaDepth=	16;
	}

	if	(_iAlphaDepth <= 0)
		return;

	if	(wAnm	>=	_lpWeapon->m_iAnmCount)
		return;

	if	(_iAlphaDepth >= 32)
	{
		_lpWeapon->PutReg(x,y,wAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);

		if (m_wOverlapAnm != 0xffff)
			_lpWeapon->PutReg(x,y,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
		if (m_wOverlapAnm2!= 0xffff)
			_lpWeapon->PutReg(x,y,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);

		return;
	}

	if (_iAlphaDepth == 16)
	{
		_lpWeapon->Put(x,y,wAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);

		if (m_wOverlapAnm != 0xffff)
			_lpWeapon->Put(x,y,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);
		if (m_wOverlapAnm2!= 0xffff)
			_lpWeapon->Put(x,y,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);

		return;
	}

	_lpWeapon->Put(x,y,wAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,_iAlphaDepth);
	
	if (m_wOverlapAnm != 0xffff)
		_lpWeapon->Put(x,y,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,_iAlphaDepth);
	if (m_wOverlapAnm2!= 0xffff)
		_lpWeapon->Put(x,y,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,_iAlphaDepth);
}




//
//	캐릭터 뒷쪽에 무기 출력
inline	void
CActor::putBackWeapon(int x,int y, int _iDirect,WORD _wAnm,BOOL _bIsWantIgnorePos)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	cANM	*lpWeapon	=	getWeaponAnm();

	if	(!lpWeapon)
		return;

	if	(m_iAnm	>=	lpWeapon->m_iAnmCount)
		return;

	if	(_bIsWantIgnorePos == FALSE  && lpWeapon->IsBack(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putWeapon(x,y,lpWeapon,_iDirect,_wAnm);
}

//
//	캐릭터 앞쪽에 무기 출력
inline	void
CActor::putFrontWeapon(int x,int y, int _iDirect,WORD _wAnm,BOOL _bIsWantIgnorePos)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	cANM	*lpWeapon	=	getWeaponAnm();

	if	(!lpWeapon)
		return;

	if	(m_iAnm	>=	lpWeapon->m_iAnmCount)
		return;

	if	(_bIsWantIgnorePos == FALSE  && lpWeapon->IsFront(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putWeapon(x,y,lpWeapon,_iDirect,_wAnm);
}
//
//	캐릭터 뒷쪽에 무기 알파블렌딩으로 출력
inline	void
CActor::putBackWeaponAB(int x,int y, int _iDirect,WORD _wAnm,int _iAlpha,BOOL _bIsWantIgnorePos)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	cANM	*lpWeapon	=	getWeaponAnm();

	if	(!lpWeapon)
		return;

	if	(m_iAnm	>=	lpWeapon->m_iAnmCount)
		return;

	if	(_bIsWantIgnorePos == FALSE && lpWeapon->IsBack(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putWeapon(x,y,lpWeapon,_iDirect,_wAnm,_iAlpha);
}

//
//	캐릭터 앞쪽에 무기 알파블렌딩으로 출력
inline	void
CActor::putFrontWeaponAB(int x,int y, int _iDirect,WORD _wAnm, int _iAlpha,BOOL _bIsWantIgnorePos)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	cANM	*lpWeapon	=	getWeaponAnm();

	if	(!lpWeapon)
		return;

	if	(m_iAnm	>=	lpWeapon->m_iAnmCount)
		return;

	if	(_bIsWantIgnorePos == FALSE  && lpWeapon->IsFront(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putWeapon(x,y,lpWeapon,_iDirect,_wAnm,_iAlpha);
}


//
//	캐릭터 뒷쪽에 무기 반투명으로 출력
inline	void
CActor::putBackWeaponHB(int x,int y, int _iDirect,WORD _wAnm,BOOL _bIsWantIgnorePos)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	cANM	*lpWeapon	=	getWeaponAnm();

	if	(!lpWeapon)
		return;

	if	(m_iAnm	>=	lpWeapon->m_iAnmCount)
		return;

	if	(_bIsWantIgnorePos == FALSE  && lpWeapon->IsBack(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putWeapon(x,y,lpWeapon,_iDirect,_wAnm,16);
}



//
//	캐릭터 앞쪽에 무기 반투명으로 출력
inline	void
CActor::putFrontWeaponHB(int x,int y, int _iDirect,WORD _wAnm,BOOL _bIsWantIgnorePos)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	cANM	*lpWeapon	=	getWeaponAnm();

	if	(!lpWeapon)
		return;

	if	(m_iAnm	>=	lpWeapon->m_iAnmCount)
		return;

	if	(_bIsWantIgnorePos == FALSE  && lpWeapon->IsFront(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putWeapon(x,y,lpWeapon,_iDirect,_wAnm,16);
}

//////////////////////////////////////////////////////////////////////////////
//	방패
//////////////////////////////////////////////////////////////////////////////

inline	void
CActor::putShield(int x,int y,int _iEquimentShape, int _iDirect,WORD _wAnm,int _iAlphaDepth)
{
	if	(m_wBody	>=	100|| m_bIsTransOther ||isTransformationMonster() )	
		return;

	int		iDirect	= _iDirect;
	WORD	wAnm	= _wAnm;
	if (isInvisivle())
	{
		if (_iAlphaDepth == 32)
			if (isRequireAddPaint())
				return;

		_iAlphaDepth	=	dINVISIBLE_ALPHA_DEPTH;

		if (isOwnTeam() )
			_iAlphaDepth=	16;
	}
	else
	if (isBlur())
	{
		if (_iAlphaDepth == 32)
			if (isRequireAddPaint())
				return;

		_iAlphaDepth	=	dBLUR_ALPHA_DEPTH;

		if (isOwnTeam() )
			_iAlphaDepth=	16;
	}

	if (_iAlphaDepth <= 0)
		return;

	if (wAnm	>=	g_aShield[m_wJob][_iEquimentShape].m_iAnmCount)
		return;

	if (_iAlphaDepth >= 32)
	{
		g_aShield[m_wJob][_iEquimentShape].PutReg(x,y,wAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
		if (m_wOverlapAnm != 0xffff) g_aShield[m_wJob][_iEquimentShape].PutReg(x,y,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
		if (m_wOverlapAnm2!= 0xffff) g_aShield[m_wJob][_iEquimentShape].PutReg(x,y,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);

		return;
	}

	if (_iAlphaDepth == 16)
	{
		g_aShield[m_wJob][_iEquimentShape].Put(x,y,wAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);
		if (m_wOverlapAnm != 0xffff) g_aShield[m_wJob][_iEquimentShape].Put(x,y,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);
		if (m_wOverlapAnm2!= 0xffff) g_aShield[m_wJob][_iEquimentShape].Put(x,y,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);

		return;
	}

	g_aShield[m_wJob][_iEquimentShape].Put(x,y,wAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,_iAlphaDepth);
	if (m_wOverlapAnm != 0xffff) g_aShield[m_wJob][_iEquimentShape].Put(x,y,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,_iAlphaDepth);
	if (m_wOverlapAnm2!= 0xffff) g_aShield[m_wJob][_iEquimentShape].Put(x,y,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,_iAlphaDepth);
}


//
//	캐릭터 뒷쪽에 방패 출력
inline	void
CActor::putBackShield(int x,int y, int _iDirect, WORD _wAnm)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	if (!isVisibleShield())	
		return;

	int	iEquimentShape	=	m_itemShield.getBasicItem()->m_wEquippedShape;

	if (iEquimentShape	==	0xffff)
		return;

	if (m_iAnm	>=	g_aShield[m_wJob][iEquimentShape].m_iAnmCount)
		return;

	if (g_aShield[m_wJob][iEquimentShape].IsBack(m_iAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putShield(x,y,iEquimentShape,_iDirect,_wAnm);
}


//
//	캐릭터 앞쪽에 방패 출력
inline	void
CActor::putFrontShield(int x,int y, int _iDirect, WORD _wAnm)
{
	if (!isVisibleShield())	
		return;

	int	iEquimentShape	=	m_itemShield.getBasicItem()->m_wEquippedShape;
	
	if (iEquimentShape	==	0xffff)
		return;

	if (m_iAnm	>=	g_aShield[m_wJob][iEquimentShape].m_iAnmCount)
		return;

	if (g_aShield[m_wJob][iEquimentShape].IsFront(m_iAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putShield(x,y,iEquimentShape, _iDirect, _wAnm);
}


//
//	캐릭터 뒷쪽에 방패 알파 블렌딩으로 출력
inline	void
CActor::putBackShieldAB(int x,int y, int _iDirect, WORD _wAnm,int _iAlpha)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	if (!isVisibleShield())
		return;

	int	iEquimentShape	=	m_itemShield.getBasicItem()->m_wEquippedShape;
	
	if (iEquimentShape	==	0xffff)
		return;

	if (m_iAnm	>=	g_aShield[m_wJob][iEquimentShape].m_iAnmCount)
		return;

	if (g_aShield[m_wJob][iEquimentShape].IsBack(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putShield(x,y,iEquimentShape,_iDirect,_wAnm, _iAlpha);
}


//
//	캐릭터 앞쪽에 방패 알파 블렌딩으로 출력
inline	void
CActor::putFrontShieldAB(int x,int y, int _iDirect,WORD _wAnm,int _iAlpha)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	if (!isVisibleShield())
		return;

	int	iEquimentShape	=	m_itemShield.getBasicItem()->m_wEquippedShape;
	
	if (iEquimentShape	==	0xffff)
		return;

	if (m_iAnm	>=	g_aShield[m_wJob][iEquimentShape].m_iAnmCount)
		return;

	if (g_aShield[m_wJob][iEquimentShape].IsFront(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putShield(x,y,iEquimentShape,_iDirect,_wAnm,_iAlpha);
}

//
//	캐릭터 뒷쪽에 방패 반투명으로 출력
inline	void
CActor::putBackShieldHB(int x,int y, int _iDirect,WORD _wAnm)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	if (!isVisibleShield())
		return;

	int	iEquimentShape	=	m_itemShield.getBasicItem()->m_wEquippedShape;
	
	if (iEquimentShape	==	0xffff)
		return;

	if (m_iAnm	>=	g_aShield[m_wJob][iEquimentShape].m_iAnmCount)
		return;

	if (g_aShield[m_wJob][iEquimentShape].IsBack(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putShield(x,y,iEquimentShape,_iDirect,_wAnm,16);
}


//
//	캐릭터 앞쪽에 방패 반투명으로 출력
inline	void
CActor::putFrontShieldHB(int x,int y, int _iDirect,WORD _wAnm)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	if	(!isVisibleShield())	
		return;

	int	iEquimentShape	=	m_itemShield.getBasicItem()->m_wEquippedShape;

	if	(iEquimentShape	==	0xffff)
		return;

	if	(m_iAnm	>=	g_aShield[m_wJob][iEquimentShape].m_iAnmCount)
		return;

	if	(g_aShield[m_wJob][iEquimentShape].IsFront(_wAnm,_iDirect,m_iFrame)	==	FALSE)
		return;

	putShield(x,y,iEquimentShape,_iDirect,_wAnm,16);
}
//
//	캐릭터보다 먼저 장비를 찍는다.

inline void
CActor::putEquipmentBack(int x,int y, int _iDirect, WORD _wAnm)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	putBackShield(x,y,_iDirect, _wAnm);
	putBackWeapon(x,y,_iDirect, _wAnm);
}



void
CActor::putImageSyncWithActor(int _iImage)
{
	int	x		=	GetScaledXPos(m_pos.x);
	int	y		=	GetScaledYPos(m_pos.y);

	y			-=	getHeight();

	cANM	*lpImage	=	(cANM *)g_im.getEffect(_iImage);

	if	(lpImage)
		lpImage->PutReg(x,y,0,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
}

//
//	캐릭터보다 나중에(남쪽) 장비를 찍는다.
inline void
CActor::putEquipmentFront(int x,int y, int _iDirect, WORD _wAnm)
{
	if	(m_wBody	>=	100|| m_bIsTransOther)
		return;

	putFrontShield(x,y,_iDirect,_wAnm);
	putFrontWeapon(x,y,_iDirect,_wAnm);

	putCastImage(x,y);
}
//
//	내몸에 붙은 블로커를 찍어준다.
void
CActor::putBlocker(int x,int y,BOOL _bIsFrontOfCharacter)
{
	if	(m_wBlockerAngle	>	360)
		return;

	int		iAngle		=	m_wBlockerAngle;
	int		iDistance	=	0,iAnm	=	0;
	CPos	pos;

	if	(_bIsFrontOfCharacter)
	{
		if(iAngle< 180)
			return;
	}
	else
	{	
		if(iAngle>=180)
			return;
	}

	pos.x	=	x;
	pos.y	=	y+dSIN_TABLE[(s_iFrameCounter+m_wSerial)*6%360]*5/256*m_wVerticalScale/100;

	if (m_wBlockerRigidityTime)
	{
		pos.y	=	y;

		if ((s_iFrameCounter+m_wSerial) % 4 < 2)
			iDistance	=	2;
	}

	GetOvalAnglePos(&pos,iAngle,dBLOCKER_PUT_DISTANCE+getBodySize()+iDistance*m_wHorizonScale/100);

	cANM	*lpBlocker;

	if	(m_wSpearBlockerShape	!=	0xffff)
	{
		lpBlocker		=	g_im.getEffect(g_im.m_wSpearBlockerImage);
		iAnm			=	m_wSpearBlockerShape;
	}
	else
	if (m_wBlockerShape	!=	0xffff)
	{
		if	(m_wBody	!=	dJOB_MONSTER_ARCH_DEVIL)
			lpBlocker	=	g_im.getEffect(g_im.m_wBlockerImage+m_wBlockerShape);
		else
			return;
	}
	else
		return;

	int		iDirectCount=	lpBlocker->GetDirectCount(0);
	int		iDirect		=	GetDirect(iAngle,iDirectCount);
	int		iAlphaDepth	=	32;

	if (isInvisivle())
	{
		iAlphaDepth	=	dINVISIBLE_ALPHA_DEPTH;

		if (isOwnTeam())
			iAlphaDepth=	16;
	}
	else
	if (isBlur())
	{
		iAlphaDepth	=	dBLUR_ALPHA_DEPTH;

		if (isOwnTeam())
			iAlphaDepth=	16;
	}

	lpBlocker->Put(pos.x,pos.y,iAnm,iDirect,0,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlphaDepth);
}

inline	void
CActor::putEnchantedImage(int x,int y,int _iMask,BOOL _bIsFront)
{
	int		iImage	=	g_enchantedImage.getImage(_iMask);

	if	(iImage		==	0xffff)	
		return;

	cANM	*lpEffect=	g_im.getEffect(iImage);

	if	(!lpEffect)
		return;

	int		iAnm	=	0;

	if	(!_bIsFront)
		iAnm	=	1;

	if	(iAnm	>=	lpEffect->m_iAnmCount)
		return;

	int	iMaxFrame	=	lpEffect->GetFrameCount(iAnm);
	int	iFPS		=	lpEffect->GetFPS(0);

	if	(iMaxFrame	==	0)
		return;

	if	(_iMask	==	dAMEM_EVANGELISM)
	{
		int		iHeight	=	0;

		{
			cANM	*lpBody		=	getBody();
			iHeight		=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100+20;

			if	(m_wRideDog	!=	0xffff)
				iHeight		=	GetBody(dBODY_KELBY_1+m_wRideDog)->m_sprite.m_iHeight*m_wVerticalScale/100+40;

			if	(m_iAnm	==	dACT_SITDOWN)
			{
				iHeight				=	lpBody->getSpriteHeight(m_iAnm,m_iDirect,m_iFrame);
				iHeight				=	iHeight*m_wVerticalScale/100+5;
			}
		}

		y		-=	iHeight;
	}

	lpEffect->PutReg(x,y,iAnm,0,((s_iFrameCounter+m_wSerial)*iFPS/dSYNC_FPS)%iMaxFrame,m_wHorizonScale,m_wVerticalScale);
}

void
CActor::putCounter(int _iImage,int _iTime,int _iX,int _iY)
{
	int	i3Digit	=	(_iTime/100)%10;
	int	i2Digit	=	(_iTime%100)/10;
	int	i1Digit	=	_iTime%10;

	if	(i3Digit)
	{
		g_im.putEffect(_iImage,_iX-15,_iY,s_iFrameCounter+m_wSerial,10,0);
		g_im.putEffect(_iImage,_iX-5,_iY,s_iFrameCounterFor1SecondWork,i3Digit,0);
		g_im.putEffect(_iImage,_iX+5,_iY,s_iFrameCounterFor1SecondWork,i2Digit,0);
		g_im.putEffect(_iImage,_iX+15,_iY,s_iFrameCounterFor1SecondWork,i1Digit,0);
	}
	else
	if	(i2Digit)
	{
		g_im.putEffect(_iImage,_iX-10,_iY,s_iFrameCounterFor1SecondWork,10,0);
		g_im.putEffect(_iImage,_iX,_iY,s_iFrameCounterFor1SecondWork,i2Digit,0);
		g_im.putEffect(_iImage,_iX+10,_iY,s_iFrameCounterFor1SecondWork,i1Digit,0);
	}
	else
	{
		g_im.putEffect(_iImage,_iX-5,_iY,s_iFrameCounterFor1SecondWork,10,0);
		g_im.putEffect(_iImage,_iX+5,_iY,s_iFrameCounterFor1SecondWork,i1Digit,0);
	}
}

void
CActor::putAttatchedFlatImage(int x,int y,BOOL _bIsFront)
{
	if(isInvisivle())
	{
		if( g_lpHero->m_wTeam != m_wTeam)
			return;
	}

	#define	dREBIRTH_EFFET_FACTOR		2
	#define	dREBIRTH_EFFET_ANGLE_VALUE	10
	#define	dREBIRTH_EFFET_RADIUS		55

	if	(m_bf4RebirthCount)
	{
		cANM*	lpBall			=	g_im.getEffect(g_im.m_awRebirthMarkImage[1]);
		int		iValue			=	CGamePlay::s_iFrameCounter;
		int		iFrameCount		=	lpBall->GetFrameCount(0);
		int		iCorrentLength	=	(5-m_bf4RebirthCount)*3;
		int		iPaletteIndex	=	m_bf4RebirthCount-1;

		lpBall->setPalette(&g_pRebirthBallPalette[iPaletteIndex*256]);

		for (int iFrame=iCorrentLength;iFrame<iFrameCount*dREBIRTH_EFFET_FACTOR;iFrame++)
		{
			int		iAngle	=	(iValue*dREBIRTH_EFFET_FACTOR+iFrame*dREBIRTH_EFFET_ANGLE_VALUE)%360;
			iAngle			=	360-iAngle-1;
			CPos	posLight;

			if	(_bIsFront)
			{
				if	(iAngle	<	180)
					continue;
			}
			else
			{
				if	(iAngle	>=	180)
					continue;
			}

			posLight.Set(x,y-getHeight());

			GetOvalAnglePos(&posLight,iAngle,dREBIRTH_EFFET_RADIUS);
			lpBall->PutReg(posLight.x,posLight.y,0,0,(iFrame-iCorrentLength)/dREBIRTH_EFFET_FACTOR,g_iGameScale,g_iGameScale);
		}

		lpBall->restorePalette();
	}

	if	(_bIsFront	==	FALSE)
	{
		if	(m_bf4RebirthCount)
		{
			int		iPaletteIndex=	m_bf4RebirthCount-1;
			cANM	*lpBaseEffect=	g_im.getEffect(g_im.m_awRebirthMarkImage[0]);

			lpBaseEffect->setPalette(&g_pRebirthPalette[iPaletteIndex*256]);
			g_im.putEffect(g_im.m_awRebirthMarkImage[0],x,y-getHeight(),s_iFrameCounter+m_wSerial,0,0,g_iGameScale*100/100,g_iGameScale*100/100);
			lpBaseEffect->restorePalette();
		}
	}
}

//
//	인챈트 이미지 모두 츌력

inline	void
CActor::putAllEnchantedImage(int x,int y,BOOL _bIsFront)
{

	if(g_config.m_bf1IsBodyEffectOff)
		return;

	int			iHeight,i;
	LONGLONG	dwMask	=	1;

	{
		if	(isPlayer())
			dwMask	=	1;

		cANM	*lpBody		=	getBody();
		iHeight		=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100+20;

		if	(m_wRideDog	!=	0xffff)
			iHeight		=	GetBody(dBODY_KELBY_1+m_wRideDog)->m_sprite.m_iHeight*m_wVerticalScale/100+40;

		if	(m_iAnm	==	dACT_SITDOWN)
		{
			iHeight				=	lpBody->getSpriteHeight(m_iAnm,m_iDirect,m_iFrame);
			iHeight				=	iHeight*m_wVerticalScale/100+5;
		}
	}

	if	(isInvisivle() || isBlur())
	{
		if	(!isOwnTeam())
			return;
	}

	for (i=0;i<32;i++)
	{
		if (m_dwEnchantedMask & dwMask)
			if (dwMask != dAMEM_BERSERK)
				putEnchantedImage(x,y,dwMask,_bIsFront);

		dwMask	<<=	1;
	}

// 수정
	if	(_bIsFront == TRUE)	//	캐릭터 전면
	{
		if (m_enchantedImage.isExitEffect())
		{
			if	(m_enchantedImage.m_bf1IsSmellOfDeath)
				g_im.putEffect(g_im.m_wEI_SmellOfDeath,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1EIFlameRing)
				g_im.putEffect(g_im.m_wEI_FlameRing,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1Incinerate)
				g_im.putEffect(g_im.m_wEI_Incinerate,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1VaccumPoint)
				g_im.putEffect(g_im.m_wEI_VaccumPoint,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
//			if	(m_enchantedImage.m_bf1IsLightningBarrier)
//				g_im.putEffect(g_im.m_wEI_LightningBarrier,x,y,s_iFrameCounter+m_wSerial,0,0,m_wHorizonScale,m_wVerticalScale);
			if	(m_enchantedImage.m_bf1IsTaunted)
				g_im.putEffect(g_im.m_wEI_Taunt,x,y-iHeight,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1HotSkin)
				g_im.putEffect(g_im.m_wEI_HotSkin,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1DarkWeapon)
				g_im.putEffect(g_im.m_wEI_DarkWeapon,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsLevelDown)
				g_im.putEffect(g_im.m_wLevelDown,x,y-iHeight*5/7,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1CancerHall)
				g_im.putEffect(g_im.m_wEI_CancerHall,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsRabbitRush)
				g_im.putEffect(g_im.m_wEI_RabbitRush,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsNoMoreFight)
				g_im.putEffect(g_im.m_wEI_NoMoreFight,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);

			if	(m_enchantedImage.m_bf1IsPlotOfShadow)
			{
				if	(isHero() && g_hero.m_wPlotOfShadowTime)
					putCounter(g_im.m_wDeathCounter,g_hero.m_wPlotOfShadowTime,x,y-iHeight-40);
			}
			
			if	(m_enchantedImage.m_bf1IsImpulse)
			{
				if	(isHero() && g_hero.m_wImpulseTime)
					putCounter(g_im.m_wDeathCounter,g_hero.m_wImpulseTime,x,y-iHeight-40);
			}

			if	(m_enchantedImage.m_bf1IsHwabyung)
			{
				if	(isHero() && g_hero.m_wHwabyungTime)
					putCounter(g_im.m_wDeathCounter,g_hero.m_wHwabyungTime,x,y-iHeight/2-40);
//				else
//					g_im.putEffect(g_im.m_wDeathCounter,x,y-iHeight-40,s_iFrameCounter+m_wSerial,10,0);
			}
			if	(m_enchantedImage.m_bf1IsSoulBurn)		 // 소울번..
				g_im.putEffect(g_im.m_wEI_SS_Soul_Blaze,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsConsensus)		 // 교감
				g_im.putEffect(g_im.m_wEI_SS_Soul_Consensus,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			
			if	(m_enchantedImage.m_bf1IsIncreaseAllStatus) // 모든능력치상태.
				g_im.putEffect(g_im.m_wEI_SS_Soul_Release,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsNotUseActivityReactionSkill) // 발동형리액션사용금지.
				g_im.putEffect(g_im.m_wEI_SS_Soul_NotUseReaction,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);

			if	(m_enchantedImage.m_bf1IsRushTime) // 돌진
				g_im.putEffect(g_im.m_wEI_NoMoreFight,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsReturnMissileDamage)		 // 미사일대미지리턴
				g_im.putEffect(g_im.m_wEI_SS_Soul_NotUseReaction,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsIncreaseHardBlow)		 // 강타효과
				g_im.putEffect(g_im.m_wEI_SS_Hard_Blow,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsGreamTant)
				g_im.putEffect(g_im.m_wEI_GleamTant,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);

			if	(m_enchantedImage.m_bf1IsPowerOfVitalization)
				g_im.putEffect(g_im.m_wEI_PowerOfVitalization,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsMakeTypeUndead)
				g_im.putEffect(g_im.m_wEI_MakeTypeUndead,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
			if	(m_enchantedImage.m_bf1IsElectricShock)
				g_im.putEffect(g_im.m_wEI_ElectricShock,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
		}

		if(m_wItemEffect != 0xffff && !isInvisivle() && g_lpHero->m_wTeam == m_wTeam)
			g_im.putEffect(m_wItemEffect,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);

		putShakle(x,y);
	}
	else
	{
		if	(isDisplacement()			)
			g_im.putEffect(g_im.m_wDisplacementImage,x,y,s_iFrameCounter,0,0,m_wHorizonScale,m_wVerticalScale);

		if	(m_enchantedImage.isExitEffect())
		{
			if	(m_enchantedImage.m_bf1UltimateBarrier)
				g_im.putEffect(g_im.m_wEI_UntimateBarrier,x,y,s_iFrameCounter+m_wSerial,0,0,500*m_wHorizonScale/100,500*m_wVerticalScale/100);

			if	(m_enchantedImage.m_bf1IsRabbitRush)
				g_im.putEffect(g_im.m_wEI_RabbitRush,x,y,s_iFrameCounter+m_wSerial,1,0,g_iGameScale,g_iGameScale);
		}
		return;
	}

	if	(isConfuse()						)
		g_im.putEffect(g_im.m_wConfuseImage,x,y-iHeight,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
	if	(isBlind()	|| isNewBlindVerySmall()|| isNewBlindSmall() || isNewBlindMiddle() || isNewBlindLarge()|| isNewBlindVeryLarge())
		g_im.putEffect(g_im.m_wBlindImage,x,y-iHeight,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
	if	(isBerserk()						)
		g_im.putEffect(g_im.m_wBerserkImage,x,y-iHeight*5/7,s_iFrameCounter,0,0,g_iGameScale,g_iGameScale);

	if	(isPlayer()	&&	(m_dwEnchantedMask &dAMEM_SANCTUARY) == 0 && isInvincible())
		g_im.putEffect(g_im.m_wInvincible,x,y,s_iFrameCounter,0,0,g_iGameScale,g_iGameScale);

	if	(isNightMare())
		g_im.putEffect(g_im.m_wSleepImage,x,y-iHeight-20,s_iFrameCounter+m_wSerial,1,0,g_iGameScale,g_iGameScale);
	else
	if	(isSleep())
		g_im.putEffect(g_im.m_wSleepImage,x,y-iHeight-20,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);

	if	(isParalysis())
		g_im.putEffect(g_im.m_wStunImage,x,y-iHeight,s_iFrameCounter+m_wSerial,1,0,g_iGameScale,g_iGameScale);
	else
	if	(isStuned())
	{
		if	((m_dwEnchantedMask & dAMEM_HOLD_MONSTER) == 0 || (m_dwEnchantedMask & dAMEM_HOLD_PERSON == 0))
			g_im.putEffect(g_im.m_wStunImage,x,y-iHeight,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
	}

	if	(isCharmed() && ((m_dwEnchantedMask & dAMEM_EVANGELISM) == 0))
		g_im.putEffect(g_im.m_wCharmedImage,x,y-iHeight-10,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);

	if	(m_bf1IsSoundOfLedersBellsLeader)
		g_im.putEffect(g_im.m_wSoundOfLeadersBellMarkImage,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
}






inline	void
CActor::putChargeImage(int _iX,int _iY,BOOL _bIsFront)
{
	if (m_wChargeImage	==	0xffff)
		return;

	cANM	*lpEffect	=	g_im.getEffect(m_wChargeImage);
	int		iAnm		=	m_bChargeAnm;

	if (iAnm	>=	lpEffect->m_iAnmCount)
		iAnm	=	0;

	if (_bIsFront)
	{
		if (lpEffect->IsFront(iAnm,0,m_bChargeFrame))
			lpEffect->PutReg(_iX,_iY,iAnm,0,m_bChargeFrame,g_iGameScale,g_iGameScale);
	}
	else
	{
		if (lpEffect->IsBack(iAnm,0,m_bChargeFrame))
			lpEffect->PutReg(_iX,_iY,iAnm,0,m_bChargeFrame,g_iGameScale,g_iGameScale);
	}
}

inline	void
CActor::putHitEffect(int x,int y)
{
	for (int i=0;i<dHIT_EFFECT_COUNT_ON_CHARACTER;i++)
	{
		if (m_aHitEffect[i] == 0xffff)
			continue;

		g_hem.put(m_aHitEffect[i],x,y);
	}
}

enum
{
	eQNM_PROPRIETY_QUEST_NPC		=	1,
	eQNM_PROCESS_QUEST_CLIENT		=	3,
	eQNM_NPC_ABOUT_PROCESS_QUEST	=	7,
	eQNM_MONSTER_ABOUT_PROCESS_QUEST=	9,
};
//
//	이름과 기타 자잘한 정보 출력
inline	void
CActor::putActorSimpleInfo(int x,int y)
{

	int		iHeight	=	0;

	{
		cANM	*lpBody		=	getBody();
		iHeight		=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100+20;

		if	(m_wRideDog	!=	0xffff)
			iHeight		=	GetBody(dBODY_KELBY_1+m_wRideDog)->m_sprite.m_iHeight*m_wVerticalScale/100+40;

		if	(m_iAnm	==	dACT_SITDOWN)
		{
			iHeight				=	lpBody->getSpriteHeight(m_iAnm,m_iDirect,m_iFrame);
			iHeight				=	iHeight*m_wVerticalScale/100+5;
		}
	}

	int		iHeadPos	=	y-iHeight-30+dCOS_TABLE[(s_iFrameCounter+m_wSerial)*2%360]*4/256;
	BOOL	bIsQuestNpc	=	FALSE;

	if	(isNpc() && m_strName[0] && m_wActorKind != eNPC_FIRST_HELPER)
	{
		bIsQuestNpc	=	TRUE;

		if (g_hero.isProcessQeustClient(m_strName,m_wNpcActorSerial))
			g_anmQuestNpcMark.PutReg(x,iHeadPos,eQNM_PROCESS_QUEST_CLIENT,0,0);
		else
		if (g_hero.isNpcAboutProcessQuest(m_strName,m_wNpcActorSerial))
			g_anmQuestNpcMark.PutReg(x,iHeadPos,eQNM_NPC_ABOUT_PROCESS_QUEST,0,0);
		else
		if (g_hero.isMonsterAboutProcessQuest(m_strName,m_wNpcActorSerial))
			g_anmQuestNpcMark.PutReg(x,iHeadPos,eQNM_MONSTER_ABOUT_PROCESS_QUEST,0,0);
		else
		if (g_hero.getProprietyQuest(m_strName,m_wNpcActorSerial))
			g_anmQuestNpcMark.PutReg(x,iHeadPos,eQNM_PROPRIETY_QUEST_NPC,0,0);
		else
			bIsQuestNpc	=	FALSE;
	}

	if	(bIsQuestNpc == FALSE)
		switch(m_wActorKind)
		{
			case	eNPC_WEAPON_MERCHANT	:	//	무기 상인
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_WEAPON_SHOP,0,0);
				break;
			case	eNPC_ARMOR_MERCHANT		:	//	방어구 상인
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_ARMOR_SHOP,0,0);
				break;
			case	eNPC_ETC_ITEM_MERCHANT	:	//	잡화 상인
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_ETC_ITEM_MERCHANT,0,0);
				break;
			case	eNPC_ACCESSORY_MERCHANT	:	//	도구 상인
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_TOOL_SHOP,0,0);
				break;
			case	eNPC_PITCHMAN			:	//	노점상
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_PITCHMAN,0,0);
				break;
			case	eNPC_BANK				:	//	은행
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_BANK,0,0);
				break;
			case	eNPC_TELEPORTER			:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_TELEPORTER,0,0);
				break;

			case	eNPC_HEALER				:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_HEALER,0,0);
				break;

			case	eNPC_QUEST_MANAGER		:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_QUEST_MANAGER,0,0);
				break;

			case	eNPC_SMITH				:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_SMITH,0,0);
				break;

			case	eNPC_FIRST_HELPER		:
			{
				int	iMaxFrame	=	g_anmNpcType.GetFrameCount(eNPC_TYPE_FIRST_HELPER);
				int	iFPS		=	g_anmNpcType.GetFPS(eNPC_TYPE_FIRST_HELPER);

				if	(iMaxFrame	<=	0)
					break;

				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_FIRST_HELPER,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame);
				break;
			}

			case	eNPC_SKILL_MASTER		:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_SKILL_MASTER,0,0);
				break;
			case	eNPC_NORMAL_QUEST		:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_QUEST_PROVIDER,0,0);
				break;
			case	eNPC_TITLE_QUEST		:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_QUEST_PROVIDER,0,0);
				break;
			case	eNPC_GUILD_QUEST		:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_QUEST_PROVIDER,0,0);
				break;
			case	eNPC_MAJOR_QUEST		:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_QUEST_PROVIDER,0,0);
				break;
			case	eNPC_DOUMI				:
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_DOUMI,0,0);
				break;

			case	eNPC_WEAPON_MERCHANT_KNIGHT_WARRIOR			:	//	"견습기사/전사 무기상":
			case	eNPC_WEAPON_MERCHANT_WIZARD_WEREWOLF		:	//	"마법사/늑대인간 무기상":
			case	eNPC_WEAPON_MERCHANT_PRIEST_FALLEN_ANGEL	:	//	"사제/타락천사 무기상":
			case	eNPC_WEAPON_MERCHANT_ROGUE_FIGHTER			:	//	"도둑/무도가 무기상":
			case	eNPC_WEAPON_MERCHANT_LANCER_ARCHER			:	//	"마법 창검사/마법 궁수 무기상":
			case	eNPC_WEAPON_MERCHANT_TAMER_SUMMONER			:	//	"조련사/소환사 무기상":
			case	eNPC_WEAPON_MERCHANT_PRINCESS_MAGICAL_GIRL	:	//	"공주/변신 소녀 무기상":
			case	eNPC_WEAPON_MERCHANT_NECROMANCER_DEVIL		:	//	"강신술사/악마 무기상":
			case	eNPC_WEAPON_MERCHANT_SOUL_BRINGER			:	//	"영술사 무기상":
			case	eNPC_WEAPON_MERCHANT_OPTICALIST				:	//	"광학사 무기상":
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_WEAPON_SHOP,0,0);
				break;
			case	eNPC_GUILD_HALL_TELEPORTER				:		// 길드홀텔레포터
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_GUILD_HALL_TELEPORTER,0,0);
				break;
			case	eNPC_EVENT_HELPER				:				// 이벤트도우미
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_EVENT_HELPER,0,0);
				break;
			case	eNPC_ADVENTURER_SOCIETY				:				// "모험가 협회"
				g_anmNpcType.PutReg(x,iHeadPos,eNPC_TYPE_EVENT_ADVENTURER_SOCIETY,0,0);
				break;

		}

#ifndef	_DEBUG
	if	(isNpc())
#endif
		putName(x,y);

	//JBC 이동중 도착점 X표시 08-06-30
// #ifdef _DEBUG
// 	if(g_hero.isWalking() && isHero())
// 		DrawCross(g_hero.GetposClick().x - GetCorrectViewXPos() , g_hero.GetposClick().y - GetCorrectViewYPos());
// #endif
}

inline	void
CActor::putBackAdditionalEffect(int _iX,int _iY)
{
	if	(m_wRideDog	!=	0xffff)
	{
		int	iAnm	=	m_iAnm;

		iAnm		-=	m_wRideDog*dACT_RIDE_DOG_ACTION_COUNT;

		switch(iAnm)
		{
			case	dACT_RIDE_DOG_MOVE		:	//	개타고 이동
				iAnm	=	dACT_RUN;
				break;
			case	dACT_RIDE_DOG_READY		:	//	개타고 정지
			case	dACT_RIDE_DOG_ATTACK	:	//	개타고 공격
				iAnm	=	dACT_READY;
				break;
		}

		int	iBody	=	dBODY_KELBY_1+m_wRideDog;

//		GetBody(iBody)->setPalette(GetBody(iBody)->getOrgPalette());
		GetBody(iBody)->PutShadow(_iX,_iY,iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
		GetBody(iBody)->PutReg(_iX,_iY,iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
	}

	if (isHero() && g_config.m_bf1IsViewSkillRange)	//	스킬 사정거리 표시
		if (g_hero.m_wLeftAbility	!=	0xffff)
		{
			int	iRange	=	g_hero.getAttackRange(g_hero.m_wLeftAbility)+g_lpHero->getBodySize();

			if (iRange && g_config.m_bf1IsViewSkillRange)
				cDRAW::DrawParticleGradationOvalCircle(_WHITE,_iX,_iY,iRange*m_wVerticalScale/100,(s_iFrameCounter+m_wSerial)*2%360,80,2,1,5);
		}
}


inline	void
CActor::putFrontAdditionalEffect(int _iX,int _iY)
{
	int		iHeight	=	0;

	{
		cANM	*lpBody		=	getBody();
		iHeight		=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100+20;

		if	(m_wRideDog	!=	0xffff)
			iHeight		=	GetBody(dBODY_KELBY_1+m_wRideDog)->m_sprite.m_iHeight*m_wVerticalScale/100+40;

		if	(m_iAnm	==	dACT_SITDOWN)
		{
			iHeight				=	lpBody->getSpriteHeight(m_iAnm,m_iDirect,m_iFrame);
			iHeight				=	iHeight*m_wVerticalScale/100+5;
		}
	}

	putMachine(_iX,_iY);	//	머쉰을 찍어준다.

	if	(m_wUseSkill == dSKILL_ILLUSION_ATTACK)	//	딜레이 어택중이다.
	{
		int	iFrame		=	m_bIllusionAttackFrame;

		for (int i=0;i<m_attackInfo.m_ability.getContinuousShotCount();i++)
		{
			if (iFrame	<	0				)
				break;
			if (iFrame	<	m_iFrameCount-1	)
			{
				CSkill* lpSkill =m_attackInfo.m_ability.getSkill();
				if(!lpSkill)
					continue;
		
				if(lpSkill->m_aValue[0] )
				{		// 딜레이 공격중. value 0의 값은.. 분신형태이다.
					CActor	*lpTarget	= g_am.getTestedActor(m_attackInfo.m_wTarget);
					if(lpTarget)
					{
						int		iAngle	=	360/m_attackInfo.m_ability.getContinuousShotCount();
						int		iRange = lpTarget->getBodySize()+getBodySize()+40;

						switch(lpSkill->m_aValue[0])
						{
						case eBUNSHIN_RUSH_ATTACK:

							iAngle	=(180+m_iAngle)%360;
							iRange	=	sqrt(pow(lpTarget->m_pos.x - m_pos.x,2) + pow(lpTarget->m_pos.y - m_pos.y,2));
							iRange -= pow((iFrame%m_iFrameCount),2)*2+ s_iFrameCounter%20*3;
							iRange = max(30, iRange);
							

							if( m_iFrameCount/3 <= (iFrame%m_iFrameCount))
								iRange = 30;
							break;
						case eBUNSHIN_ROUND_ATTACK:
							iAngle = (((180+m_iAngle)/iAngle + i) * iAngle)%360 + m_iAngle%iAngle;
							break;
						}

						int		iDirect	=	m_iDirect;
						CPos	pos;
						pos.x = lpTarget->m_pos.x;
						pos.y = lpTarget->m_pos.y;
						
						GetOvalAnglePos(&pos,iAngle,iRange);
						pos.x		=	GetScaledXPos(pos.x);
						pos.y		=	GetScaledYPos(pos.y);
						iDirect		=	GetDirect((iAngle+180)%360,m_iDirectCount);
						putBodyEffect(pos.x,pos.y,iFrame,iDirect,dPUT_SOFT_DODGE,dACT_ACTION_1, FALSE);
						
					}
				}
				else
				{
					putBodyHalf(_iX,_iY,iFrame, m_iDirect);
				}

			}
			iFrame	-=	m_attackInfo.getContinuousShotPeriod();
		}
	}

	if	(!isVisibleName())
		putHitEffect(_iX,_iY);

	putActorSimpleInfo(_iX,_iY);

	if	(m_wTargetMark	!=	0xffff)
		g_im.putEffect(m_wTargetMark,_iX,_iY-iHeight*2/3,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);

	if (isHighOperator())
	{
		int	iFPS		=	g_anmGameManager.GetFPS(0);
		int	iFrameCount	=	g_anmGameManager.GetFrameCount(0);
		int	iFrame		=	(((s_iFrameCounter+m_wSerial)/iFPS)%iFrameCount);

		g_anmGameManager.PutReg(_iX,_iY,0,0,iFrame,m_wHorizonScale,m_wVerticalScale);
	}

	if(m_wAttackDoppelgangerCount)
	{
		CPos pos;
		pos.Set(m_pos.x,m_pos.y);
		int iAngle = (180+m_iAngle)%360;
		int	iRange = getBodySize()+40 * g_iGameScale/100;
		GetOvalAnglePos(&pos,iAngle,iRange);
		pos.x		=	GetScaledXPos(pos.x);
		pos.y		=	GetScaledYPos(pos.y) - getHeight();
		putBodyEffect(pos.x,pos.y,(int)m_fAttackDoppelgangerFrame,m_iDirect,dPUT_SOFT_DODGE,m_wAttackDoppelgangerAnm, FALSE);
	}
}




//
//	타격 이미지를 찍어준다.
inline	void
CActor::putAdditionalEffect(int x,int y,BOOL _bIsFront)
{
	putChargeImage(x,y,_bIsFront);
	putAllEnchantedImage(x,y,_bIsFront);
	putBlocker(x,y,_bIsFront);			//	블로커를 찍어준다.
	putAfterImage(_bIsFront);			//	캐릭터 잔상 효과
	putTheOtherSelf(x,y,_bIsFront);

	if	(m_wBody	==	dJOB_MONSTER_SAGE)
	{
		int	iAnm		=	1;

		if	(_bIsFront)
		{
			iAnm		=	0;

			if	(isRequireAddPaint())
				return;
		}

		int	iMaxFrame	=	g_anmSageStone.GetFrameCount(iAnm);
		int	iFPS		=	g_anmSageStone.GetFPS(iAnm);

		if	(_bIsFront	==	FALSE)
			g_anmSageStone.PutShadow(x,y,0,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame,m_wHorizonScale,m_wVerticalScale);

		setSageBookPalette();

		g_anmSageStone.PutReg(x,y,iAnm,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame,m_wHorizonScale,m_wVerticalScale);
		g_anmSageStone.PutReg(x,y,iAnm+2,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame,m_wHorizonScale,m_wVerticalScale);

		g_anmSageStone.restorePalette();
	}
}

//
//	캐릭터의 뒤에 잔상을 찍어준다.


inline void
CActor::putAfterImage(BOOL _bIsFront)
{
	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_bf1IsHide || m_enchantedImage.m_bf1IsTransToWeapon)//	||	isInvisivle()	||	isBlur())
		return;

	int	iOldAnm		=	m_iAnm;
	int	iOldDirect	=	m_iDirect;
	int	iOldFrame	=	m_iFrame;
	int	iCorrectAlpha=	1;

	if	(isInvisivle()	||	isBlur())
		iCorrectAlpha	=	2;

	for (int i=0;i<dAFTER_IMAGE_COUNT;i++)
	{
		int	iAlphaDepth	=	m_aAfterImageList[i].m_bAlphaDepth;

		m_iAnm			=	m_aAfterImageList[i].m_bAnm;
		m_iDirect		=	m_aAfterImageList[i].m_bDirect;
		m_iFrame		=	m_aAfterImageList[i].m_bFrame;

		if	(iAlphaDepth/iCorrectAlpha	<=	0)
			continue;

		int	_iX			=	GetScaledXPos(m_aAfterImageList[i].m_pos.x);
		int	_iY			=	GetScaledYPos(m_aAfterImageList[i].m_pos.y);

		if (m_wAwesomeFortressAction == 0xffff || m_wContinuousHitOfDoppelgangerAction[0] == 0xffff)
		{
			if (_bIsFront)
			{
				if (m_aAfterImageList[i].m_pos.y < m_pos.y)
					continue;
			}
			else
			{
				if (m_aAfterImageList[i].m_pos.y >= m_pos.y)
					continue;
			}
		}

		lpBody->Put(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlphaDepth/iCorrectAlpha);

		if (_bIsFront)
			putFrontWeaponAB(_iX,_iY,m_iDirect,m_iAnm,iAlphaDepth/iCorrectAlpha,TRUE);
		else
			putBackWeaponAB(_iX,_iY,m_iDirect, m_iAnm,iAlphaDepth/iCorrectAlpha,TRUE);
//		if (m_wOverlapAnm != 0xffff)	lpBody->Put(_iX,_iY,m_wOverlapAnm,iDirect,iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlphaDepth);	//	더미
	}

	m_iAnm		=	iOldAnm;
	m_iDirect	=	iOldDirect;
	m_iFrame	=	iOldFrame;
}
//
//	오버랩되는 이미지 출력
void
CActor::putOverLap(int x,int y)
{
	if	(m_bf1IsHide || m_enchantedImage.m_bf1IsTransToWeapon)
		return;

	if (m_wSyncStatusWithServer)
		return;
}

//
//	머쉰을 찍어준다.
inline	void
CActor::putMachine(int x,int y)
{
	td2dvector vi,fly_pos ;

	if	(m_wMachineSkill	==	0xffff)
		return;

	CSkill	*lpSkill	=	&g_aSkill[m_wMachineSkill];
	cANM	*lpMachine	=	g_im.getEffect(lpSkill->m_wMachineImage);
	int		iDirect;

	if (m_wMachineAngle	==	0xffff)
		iDirect		=	GetDirect(m_iAngle,lpMachine->GetDirectCount(0));
	else
		iDirect		=	GetDirect(m_wMachineAngle,lpMachine->GetDirectCount(0));

	int	iAngle			=	(m_iAngle+180)%360;

	m_flying_bow.set_current_parent_pos(m_pos.x, m_pos.y);
	m_flying_bow.set_target_degree((float)(-iAngle));

	vi		=	m_flying_bow.get_child_pos();
	vi.x	=	GetScaledXPos(vi.x);
	vi.y	=	GetScaledYPos(vi.y);

	fly_pos = vi - m_flying_bow.get_fly_pos();

	int	iAlpha	=	32;
  
	if (isInvisivle())
	{
		iAlpha	=	dINVISIBLE_ALPHA_DEPTH;

		if	(isOwnTeam() )
			iAlpha	=	12;
	}
	else
	if (isBlur())
	{
		iAlpha	=	dBLUR_ALPHA_DEPTH;

		if	(isOwnTeam() )
			iAlpha	=	12;
	}

	if	(isHero())
	{
		iAlpha	=	iAlpha;

	}
	lpMachine->Put(vi.x,vi.y,0,iDirect,m_wMachineFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);
	
	if	(iAlpha	==	32)
		lpMachine->PutShadow(vi.x,vi.y,0,iDirect,m_wMachineFrame,m_wHorizonScale,m_wVerticalScale);
//	cDRAW::FillHB(0,fly_pos.x-5,fly_pos.y-5+10, fly_pos.x+5, fly_pos.y+5+10 );
}

//
//	블럭커의 그림자를 찍는다.
inline void
CActor::putBlockerShadow(int x,int y)
{
	if (m_wBlockerAngle	>	360)
		return;

	int		iAngle	=	m_wBlockerAngle;
	int		iAnm	=	0;
	int		iAlpha	=	16;

	if (isInvisivle())
	{
		iAlpha	=	dINVISIBLE_ALPHA_DEPTH;

		if (isOwnTeam() )
			iAlpha	=	12;
	}
	else
	if (isBlur())
	{
		iAlpha	=	dBLUR_ALPHA_DEPTH;

		if (isOwnTeam() )
			iAlpha	=	12;
	}

	CPos	pos;

	pos.x	=	x;
	pos.y	=	y;

	GetOvalAnglePos(&pos,iAngle,dBLOCKER_PUT_DISTANCE+getBodySize());

	cANM	*lpBlocker;

	if	(m_wSpearBlockerShape	!=	0xffff)
	{
		lpBlocker	=	g_im.getEffect(g_im.m_wSpearBlockerImage);
		iAnm		=	m_wSpearBlockerShape;
	}
	else
	if	(m_wBlockerShape	!=	0xffff)
	{
		if	(m_wBody	!=	dJOB_MONSTER_ARCH_DEVIL)
			lpBlocker	=	g_im.getEffect(g_im.m_wBlockerImage+m_wBlockerShape);
		else
			return;
	}
	else
		return;

	int		iDirectCount=	lpBlocker->GetDirectCount(0);
	int		iDirect		=	GetDirect(iAngle,iDirectCount);

	lpBlocker->PutShadow(pos.x,pos.y,iAnm,iDirect,0,m_wHorizonScale,m_wVerticalScale,iAlpha);
}

//
//	그림자들을 찍는다.
inline void
CActor::putShadow(int x,int y)
{
	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_wHillSkill!=	0xffff)
		return;
	if	(m_wRideDog	!=	0xffff)
		return;

	int	iAlpha	=	16;

	if	(isInvisivle())
	{
		iAlpha	=	dINVISIBLE_ALPHA_DEPTH;

		if	(isOwnTeam() )
			iAlpha	=	12;
	}
	else
	if	(isBlur())
	{
		iAlpha	=	dBLUR_ALPHA_DEPTH;

		if	(isOwnTeam() )
			iAlpha	=	12;
	}

	if	(m_wBody	!=	dJOB_MONSTER_ARCH_DEVIL	&&	isEnchantedLevitate()	&&	isDeath()	==	FALSE)
	{
		cANM *lpShadow	=	g_im.getEffect(g_im.m_wLevitateShadow);

		if	(lpShadow)
		{
			if	(iAlpha	==	16)
				lpShadow->PutReg(x,y,0,0,0,m_wHorizonScale,m_wVerticalScale);
			else
				lpShadow->Put(x,y,0,0,0,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);
		}

		return;
	}

//		if (isVisibleShield())	g_aSHIELDS[m_wJob].PutShadow	(x,y,m_iAnm,m_iDirect,m_iFrame);	//	무기가 있다.
//		if (isVisibleWeapon())	g_apWeapon[m_wJob].PutShadow	(x,y,m_iAnm,m_iDirect,m_iFrame);	//	방패가 있다.

	putBlockerShadow(x,y);

	lpBody->PutShadow(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,iAlpha);	//	캐릭터의 그림자
}

//
//	더미 출력

inline	BOOL
CActor::putDummy()
{
	if	(m_wDummyStatus	==	0xffff)
		return	FALSE;

	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return	TRUE;

	if	(m_wDummyStatus	==	0		)
		return	TRUE;

	setPalette();

	int	x,y;

	x	=	m_pos.x;
	y	=	m_pos.y-getHeight();
	x	=	GetScaledXPos(x);
	y	=	GetScaledYPos(y);

	int	iAlpha	=	min((m_wDummyStatus-32)/2,32);

	putBackShieldAB(x,y,m_iDirect,m_iAnm,iAlpha);
	putBackWeaponAB(x,y,m_iDirect,m_iAnm,iAlpha);

//	본체다!!
	if	(iAlpha	>=	32)
		lpBody->PutShadow(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	캐릭터의 그림자

	lpBody->Put(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	더미

	putFrontShieldAB(x,y,m_iDirect,m_iAnm,iAlpha);
	putFrontWeaponAB(x,y,m_iDirect,m_iAnm,iAlpha);

	putName(x,y);

	return	TRUE;
}

inline	void
CActor::putBodyEffect(int _iX,int _iY,int _iFrame,int _iDirect, int _iEffect, int _iAnm, BOOL _bIsUseEnchantedMask)
{
	cANM	*lpBody	=	getBody();
	cANM	*lpBodyVariation	=	NULL;

	int	iAnm = m_iAnm;
	if(_iAnm != 0xffff)
	{	
		iAnm = _iAnm;
	}
	if	(m_wBodyVariation)
	{
		lpBodyVariation	=	GetMonsterVariation(m_wBody,m_wBodyVariation);

		if	(!lpBodyVariation)
		{
			g_im.loadIMonsterBody(m_wBody,m_wBodyVariation);

			lpBodyVariation	=	GetMonsterVariation(m_wBody,m_wBodyVariation);
		}
	}

	if	(!lpBody	||	iAnm	>=	lpBody->m_iAnmCount)
		return;

	if	(m_bf1IsHide || m_enchantedImage.m_bf1IsTransToWeapon)
		return;

	if	(m_wIsReceivedBasicData	==	FALSE)
		return;

	int	iSaveFrame	=	m_iFrame;

	int iDirect = _iDirect;

	m_iFrame	=	_iFrame;
	putBackWeaponHB(_iX,_iY, _iDirect, iAnm);
	putBackShieldHB(_iX,_iY, _iDirect,iAnm);



	DWORD	dwEnchantedMask	=	0;
	if(m_dwEnchantedMask && !_bIsUseEnchantedMask)
	{
		dwEnchantedMask	=	m_dwEnchantedMask;	
		m_dwEnchantedMask = 0;
		setPalette();
	}
	lpBody->Put(_iX,_iY,iAnm,iDirect,_iFrame,m_wHorizonScale,m_wVerticalScale,_iEffect);	//	캐릭터
	if	(lpBodyVariation)
		lpBodyVariation->Put(_iX,_iY,iAnm,iDirect,_iFrame,m_wHorizonScale,m_wVerticalScale,_iEffect);	//	캐릭터
	
	if	(m_wOverlapAnm != 0xffff)
	{
		lpBody->Put(_iX,_iY,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,_iEffect);	//	더미
		
		if	(lpBodyVariation)
			lpBodyVariation->Put(_iX,_iY,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,_iEffect);	//	더미
	}
	
	if	(m_wOverlapAnm2!= 0xffff)
	{
		lpBody->Put(_iX,_iY,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,_iEffect);	//	더미
		
		if	(lpBodyVariation)
			lpBodyVariation->Put(_iX,_iY,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,_iEffect);	//	더미
	}
	
	putFrontWeaponHB(_iX,_iY, _iDirect, iAnm);
	putFrontShieldHB(_iX,_iY, _iDirect, iAnm);

	if(dwEnchantedMask && !_bIsUseEnchantedMask)
	{
		m_dwEnchantedMask = dwEnchantedMask;
		setPalette();
	}
	m_iFrame	=	iSaveFrame;
}


//
//	장비와 캐릭터의 팔레트를 설정한다.
//

inline	void
CActor::setPalette()
{
	cANM	*lpBody	=	getBody();
	cANM	*lpBodyVariation	=	NULL;

	if	(m_wBodyVariation)
	{
		lpBodyVariation	=	GetMonsterVariation(m_wBody,m_wBodyVariation);

		if	(!lpBodyVariation)
		{
			g_im.loadIMonsterBody(m_wBody,m_wBodyVariation);

			lpBodyVariation	=	GetMonsterVariation(m_wBody,m_wBodyVariation);
		}
	}

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_bIsTransOther)
		if	(m_enchantedImage.m_bf1IsZombieBomb	==	FALSE)
			return;

	int	iBody	=	m_wBody;

	if	(m_enchantedImage.m_bf1IsZombieBomb)
		iBody	=	dJOB_MONSTER_ZOMBIE;

	static	WORD	saTempPlt[256];

	setWeaponPalette();
	setShieldPalette();

	int		i=0;
	int		iPaletteIndex=	0;

	if	(iBody	>=	dBODY_HERO_JOB_END)	//	몬스터나 NPC의 경우..
		iPaletteIndex	=	g_jm.getPalettIndex(m_wTrueJob);
	else	//	플레이어의 경우..
		iPaletteIndex	=	m_itemArmor.m_wColorizeEffect;

	if	(m_enchantedImage.m_bf1IsZombieBomb)
		iPaletteIndex	=	0;

	if	(iPaletteIndex	==	0xffff)
		iPaletteIndex	=	0;

	WORD	*lpPalette		=	g_aPalette[iBody].m_apPalette[iPaletteIndex];
	
	if	(m_enchantedImage.m_bf1IsTransformationHunter)
	{
		lpPalette		=		g_aPalette[dJOB_MONSTER_DARK_ELF].m_apPalette[0];
	}
	else if(m_enchantedImage.m_bf1IsTransformationThief)
	{
		lpPalette		=		g_aPalette[dJOB_MONSTER_ASSASSIN].m_apPalette[0];
	}
	else if(m_enchantedImage.m_bf1IsTransformationMagician)
	{
		lpPalette		=		g_aPalette[dJOB_MONSTER_FALLEN_WIZARD].m_apPalette[0];
	}
	else if(m_enchantedImage.m_bf1IsTransformationReptile)
	{
		lpPalette		=		g_aPalette[dJOB_MONSTER_REPTILE].m_apPalette[0];
	}

	if	(lpPalette	==	NULL)
		lpPalette	=	lpBody->m_sprite.m_pPlt;

	if	(isHighOperator())
	{
		int	iIntensity	=	100;
		int	iSpeed		=	400;
		int	iValue		=	((s_iFrameCounter+m_wSerial)*iSpeed/100)%(iIntensity*2);

		if (iValue	>	iIntensity)
			iValue	=	iIntensity*2-iValue;

		EffectPalette(dPLT_EFFECT_BRIGHT,lpPalette,saTempPlt,100+iValue,0);

		lpPalette	=	saTempPlt;
	}
	else
	if	(isBoost())
	{
		int	iIntensity	=	100;
		int	iSpeed		=	400;
		int	iValue		=	((s_iFrameCounter+m_wSerial)*iSpeed/100)%(iIntensity*2);

		if (iValue	>	iIntensity)
			iValue	=	iIntensity*2-iValue;

//		if (isPet())
		EffectPalette(dPLT_EFFECT_RED,lpPalette,saTempPlt,120+iValue,0);
//		else
//			EffectPalette(dPLT_EFFECT_YELLOW,lpPalette,saTempPlt,200+iValue,0);

		lpPalette	=	saTempPlt;
	}
	else
	if	(isIgnoreAttackByGGG())
	{
		int	iIntensity	=	100;
		int	iSpeed		=	400;
		int	iValue		=	((s_iFrameCounter+m_wSerial)*iSpeed/100)%(iIntensity*2);
		
		if (iValue	>	iIntensity)
			iValue	=	iIntensity*2-iValue;

		EffectPalette(dPLT_EFFECT_BLUE,lpPalette,saTempPlt,120+iValue,0);

		lpPalette	=	saTempPlt;
	}
	else
	if	(m_wWhiteFadeStep)
	{
		int	iIntensity	=	m_wWhiteFadeStep;

		if (m_wWhiteFadeStep>=		100)
			iIntensity	=	100-(m_wWhiteFadeStep-100);

		EffectPalette(dPLT_EFFECT_COLORIZE,lpPalette,saTempPlt,iIntensity,_WHITE);

		if (s_iFocusActor	==	m_wSerial)				//	마우스가 올라가 있는넘
			EffectPalette(dPLT_EFFECT_BRIGHT,saTempPlt,saTempPlt,150,0);

		lpPalette	=	saTempPlt;
	}
	else
	if	(isPlayer() && m_strName[0] && g_am.isDenyCommunityActor(m_strName))
	{
		memset(saTempPlt,0,sizeof(saTempPlt));
		lpPalette	=	saTempPlt;
	}
	else
	if	(m_bf1IsQuizEventWinner)
	{
		memset(saTempPlt,0xff,sizeof(saTempPlt));
		lpPalette	=	saTempPlt;
	}
	else
	if	(m_bf1IsQuizEventLoser)
	{
		memset(saTempPlt,0,sizeof(saTempPlt));
		lpPalette	=	saTempPlt;
	}
	else
	if	(isStoned())
	{
		EffectPalette(dPLT_EFFECT_GRAY,lpPalette,saTempPlt,70,0);

		lpPalette	=	saTempPlt;
	}
	else
	if	(isCold() || isFreeze() || m_enchantedImage.m_bf1IsConversionIce || m_bf1IsFreezeShakle)
	{
		EffectPalette(dPLT_EFFECT_BLUE,lpPalette,saTempPlt,190,0);

		lpPalette	=	saTempPlt;
	}
	else
	if	(isMadness())
	{		// 광란 상태..
		EffectPalette(dPLT_EFFECT_BLACK,lpPalette,saTempPlt,190,0);

		lpPalette	=	saTempPlt;
	}

	if	(s_iFocusActor	==	m_wSerial)				//	마우스가 올라가 있는넘
	{
		static	WORD	saBrightPlt[256];

		EffectPalette(dPLT_EFFECT_BRIGHT,lpPalette,saBrightPlt,150,0);

		lpBody->m_sprite.setPalette(saBrightPlt);

		if	(lpBodyVariation)
			lpBodyVariation->m_sprite.setPalette(saBrightPlt);
	}
	else
	{
		lpBody->m_sprite.setPalette(lpPalette);

		if	(lpBodyVariation)
			lpBodyVariation->m_sprite.setPalette(lpPalette);
	}
}

enum
{
	eMD_GRADE1		=	0,
	eMD_GRADE_X		=	10,
	eMD_KIND_UNDEAD	=	11,
	eMD_KIND_UNKNOWN=	16,
	eMD_ZIN_MONSTER	=	17,
};
//
//	캐릭터 이름 출력
void
CActor::putName(int x,int y,BOOL _bIsOnlyGauge)
{
	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_bf1IsHideName)
		return;

#ifdef	_DEBUG
	if	(g_bIsHideDamageAndHideName)
	{
		if	(!isDeath())
			putHitEffect(x,y);

		return;
	}
#endif

	if	(m_bf1IsHide || m_enchantedImage.m_bf1IsTransToWeapon)
		return;

	if	(m_wIsRideTamer)
		return;

	if	(isHero())
		m_wIsRideTamer	=	m_wIsRideTamer;

	if	(m_wTalkTime	&&	!m_wIsWarningMessage)
		return;

	if	(m_strName[0] == NULL)
	{
		addToAskInfoActorList();

		return;
	}

	if	(m_wIsPutName)
		return;

	if	(m_wBody	==	dJOB_MONSTER_SAGE_MASTER	&&	isInvincible())
		return;

	if	(m_wBody	==	dJOB_MONSTER_TIAMATH_FEELER	||	m_wBody	==	dJOB_MONSTER_TIAMATH_BOTTOM_FEELER)
	{
		putHitEffect(x,y);
		return;
	}

	char	strName[64],strBaseName[32];

	strcpy(strBaseName,m_strName);

	if	(m_wIsEventMob)
	{
		CJobInfo	*lpJob	=	g_jm.getBaseJob(m_wJob-dJOB_MONSTER_START);

		if	(lpJob)
			strcpy(strBaseName,lpJob->m_strName);
	}

	if	(isSummonBeast())
	{
		if (m_strTamerName[0]	==	NULL)
		{
			CActor	*lpTamer;

			if (g_hero.isSummonBeastActor(m_wSerial))
				lpTamer	=	g_lpHero;
			else
				lpTamer	=	g_am.getTestedActor(m_wTamer);

			if (lpTamer)
			{
				if (lpTamer->m_strName[0])
					strcpy(m_strTamerName,lpTamer->m_strName);
				else
					lpTamer->addToAskInfoActorList();
			}
		}

		if (m_strTamerName[0])
			sprintf(strBaseName,dMSG_SUMMON_BEAST_NAME_FORM,m_strTamerName,m_strName);
	}

	int	iLevel=-1,iType=-1,iGrade=-1;

	if	(isMonster() && !isPet() && !isSummonBeast())
		g_hero.discermentMonster(this,&iLevel,&iType,&iGrade);

	if	(s_bIsGuildBattleField && isOwnTeam())
		iLevel	=	m_wLevel;
	if	(isHero() || isPartyMember() || g_hero.isPet(m_wSerial) || g_hero.isOperator() || g_hero.isSummonBeastActor(m_wSerial))
		iLevel	=	m_wLevel;

	if	(g_hero.m_wTitleDistinguisherValue)
	{
		int	iLimitLevel	=	g_hero.getLevel()*g_hero.m_wTitleDistinguisherValue/100;

		if	(m_wLevel	<=	iLimitLevel)
			iLevel	=	m_wLevel;
	}

	if	(m_wBody	>=	dJOB_MONSTER_Guild_Crest	&&	m_wBody	<=	dJOB_MONSTER_GuildFlag)
		iLevel	=	-1;
	
	if	(g_bIsObserverModeForDuel)
		iLevel	=	-1;

	if	(iLevel	==	0 && !isNpc())
	{
		if (isHero())
		{
			m_wLevel	=	g_hero.getLevel();
			iLevel		=	m_wLevel;
		}
		else
			addToAskInfoActorList();

		putHitEffect(x,y);

		return;
	}

	if	(iLevel	!=	-1)
		sprintf(strName,"%s(%d)",strBaseName,m_wLevel);
	else
	if	(m_wBody	==	dJOB_MONSTER_Guild_Crest	||	m_wBody	==	dJOB_MONSTER_GuildFlag)
	{
		int	iRemainHP	=	max(m_iHP*100/m_iMaxHP,1);

		sprintf(strName,"%s [%d%s]",strBaseName,iRemainHP,dMSG_PERCENTAGE);
	}
	else
		strcpy(strName,strBaseName);

	s_sText.scriptPutOff();	//	스크립트를 쌩까고 출력

	BOOL	bIsZinMonster	=	FALSE;

	if	(isMonster())
	{
		CJobInfo*	lpJob	=	getJob();

		if (lpJob)
		{
			if	(lpJob->m_bf1IsZinMonster)
				bIsZinMonster	=	TRUE;

			if	(lpJob->m_bf1IsExMonster)
			{
#ifdef	_DEBUG
				strcpy(strName+strlen(strName)," Ex");
#endif
			}
		}
	}

#ifdef	_DEBUG
	if	(bIsZinMonster)
		strcpy(strName+strlen(strName)," Zin");
#endif
	
	//sprintf(strName,"%d",m_wSerial);

	int		iNameWidth		=	max(s_sText.getPixelSize(strName),40);
	int		iHeight			=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100+20;

	if	(m_wRideDog	!=	0xffff)
		iHeight		=	GetBody(dBODY_KELBY_1+m_wRideDog)->m_sprite.m_iHeight*m_wVerticalScale/100+40;

	if	(m_wBody	>=	dJOB_MONSTER_SAGE	&&	m_wBody	<=	dJOB_MONSTER_Exist_Variation_begin)
	{
		if	(m_wBody==	dJOB_MONSTER_SAGE)
			iNameWidth	=	max(140,iNameWidth);
		if	(m_wBody==	dJOB_MONSTER_ARCH_DEVIL)
			iNameWidth	=	max(140,iNameWidth);
		if	(m_wBody==	dJOB_MONSTER_WATER_DRAGON)
			iNameWidth	=	max(140,iNameWidth);
		if	(m_wBody==	dJOB_MONSTER_TIAMATH)
			iNameWidth	=	max(140,iNameWidth);
		if	(m_wBody==	dJOB_MONSTER_DRACO_LICH)
			iNameWidth	=	max(140,iNameWidth);
	}

	int		iNameBarIndex	=	0;
	LONGLONG	llBarWidth	=	m_iHP;
	llBarWidth				*=	(iNameWidth+10);
	llBarWidth				/=	m_iMaxHP;
	int		iBarWidth		=	(int)llBarWidth;

	if	(m_iAnm	==	dACT_SITDOWN	&&	m_wJob	<	dPLAYER_JOB_COUNT)
	{
		int		iSitSpriteIndex	=	lpBody->GetSpriteIndex(m_iAnm,m_iDirect,0);
		BYTE	*lpSprite		=	lpBody->m_sprite.Get8(iSitSpriteIndex);

		if	(lpSprite)
		{
			int	iSitHeight		=	*(short *)(lpSprite+6)+lpBody->m_posRefit.y+15;

			iHeight				=	iSitHeight*m_wVerticalScale/100+15;
		}
	}

	if	(m_wBody	==	dJOB_MONSTER_GuildFlag)
		iHeight		=	120;

	if	(m_wBody	>=	dJOB_MONSTER_SAGE	&&	m_wBody	<=	dJOB_MONSTER_Exist_Variation_begin)
	{
		if	(m_wBody==	dJOB_MONSTER_SAGE)
			iHeight		=	0;
		if	(m_wBody==	dJOB_MONSTER_ARCH_DEVIL)
			iHeight		=	-40*g_iGameScale/100;
		if	(m_wBody==	dJOB_MONSTER_WATER_DRAGON)
			iHeight		=	-40*g_iGameScale/100;
		if	(m_wBody==	dJOB_MONSTER_TIAMATH)
			iHeight		=	-40*g_iGameScale/100;
		if	(m_wBody==	dJOB_MONSTER_DRACO_LICH)
			iHeight		=	-40*g_iGameScale/100;
	
	}

	if	(isDeath()	&&	isNpc()	==	FALSE)
	{
		cRECT	rect;

		rect.set(0,0,0,0);
		lpBody->getRect(m_iAnm,m_iDirect,m_iFrame,&rect,m_wVerticalScale);
		rect.add(x,y);

		x		=	rect.x1+rect.getWidth()/2;
		iHeight	=	y-(rect.y1-20);
	}

	if	(m_bf1IsCopyActor)
		m_bf1IsCopyActor	=	m_bf1IsCopyActor;
	
	if	(s_bIsGuildBattleField)
	{
				if (!isOwnTeam()				)	iNameBarIndex	=	eNBI_RED_LEFT;
		else	if (m_bf1IsCopyActor && isOwnTeam()	)iNameBarIndex	=	eNBI_BLUE_LEFT;
		else	if (isPet()						)	iNameBarIndex	=	eNBI_GREEN_LEFT;
		else	if (isSummonBeast()				)	iNameBarIndex	=	eNBI_GREEN_LEFT;
		else	if (isDummy()					)	iNameBarIndex	=	eNBI_GRAY_LEFT;
		else	if (isHero() || isPartyMember()	)	iNameBarIndex	=	eNBI_GREEN_LEFT;
		else	if (isOwnTeam()					)	iNameBarIndex	=	eNBI_BLUE_LEFT;
		else	if (isNpc()						)	iNameBarIndex	=	eNBI_GRAY_LEFT;
	}
	else
	{
				if (isHighOperator()			)	iNameBarIndex	=	eNBI_RED_LEFT;
		else	if (m_bf1IsCopyActor && isOwnTeam()	)iNameBarIndex	=	eNBI_BLUE_LEFT;
		else	if (isNpc()						)	iNameBarIndex	=	eNBI_GRAY_LEFT;
		else	if (!isOwnTeam()				)	iNameBarIndex	=	eNBI_RED_LEFT;
		else	if (isPet()						)	iNameBarIndex	=	eNBI_GREEN_LEFT;
		else	if (isSummonBeast()				)	iNameBarIndex	=	eNBI_GREEN_LEFT;
		else	if (isDummy()					)	iNameBarIndex	=	eNBI_GRAY_LEFT;
		else	if (isHero()||isPartyMember()	)	iNameBarIndex	=	eNBI_GREEN_LEFT;
		else	if (isMonster()					)	iNameBarIndex	=	eNBI_RED_LEFT;
		else	if (isPlayer()					)	iNameBarIndex	=	eNBI_BLUE_LEFT;
	}
	
	int	iNameBarWidth	=	iBarWidth;
	
	BOOL	bIsMovieMode	=	FALSE;

#ifdef	_DEBUG
	bIsMovieMode	=	g_bIsMovieMode;
#endif

	if(!bIsMovieMode)
	{
		g_sprNameBar.Put(x-iNameWidth/2           ,y-iHeight,0,100,100,dPUT_HALF_BLENDING);		//5
		g_sprNameBar.Put(x-iNameWidth/2+iNameWidth,y-iHeight,1,100,100,dPUT_HALF_BLENDING);	//5
		g_sprNameBar.putClipedImage(x-iNameWidth/2,y-iHeight,2,iNameWidth,100,100,dPUT_HALF_BLENDING);
		
		if (m_iFirstAidHP)
		{
			m_iFirstAidHP	=	min(m_iFirstAidHP,m_iMaxHP-m_iHP);
			llBarWidth		=	m_iHP+m_iFirstAidHP;
			llBarWidth		*=	(iNameWidth+10);
			llBarWidth		/=	m_iMaxHP;
			int	iFirstAidBarWidth	=	(int)llBarWidth;
			int	iFirstBarIndex		=	eNBI_RED_LEFT;
			
			int	iAlpha		=	s_iFrameCounter/2%30;
			
			if (iAlpha	>	15)
				iAlpha	=	10+(30-iAlpha);
			else
				iAlpha	+=	10;
			
			g_sprNameBar.putClipedImage(x-iNameWidth/2				,y-iHeight,iFirstBarIndex	,iFirstAidBarWidth-5,100,100,dPUT_ALPHA_BLENDING,iAlpha);
			g_sprNameBar.putClipedImage(x-iNameWidth/2+iNameWidth	,y-iHeight,iFirstBarIndex+1	,iFirstAidBarWidth-(iNameWidth+5),100,100,dPUT_ALPHA_BLENDING,iAlpha);
			g_sprNameBar.putClipedImage(x-iNameWidth/2				,y-iHeight,iFirstBarIndex+2	,min(iFirstAidBarWidth-5,iNameWidth),100,100,dPUT_ALPHA_BLENDING,iAlpha);
		}
		
		g_sprNameBar.putClipedImage(x-iNameWidth/2				,y-iHeight,iNameBarIndex  ,iBarWidth);		//5
		g_sprNameBar.putClipedImage(x-iNameWidth/2+iNameWidth	,y-iHeight,iNameBarIndex+1,iBarWidth-(iNameWidth+5));
		g_sprNameBar.putClipedImage(x-iNameWidth/2				,y-iHeight,iNameBarIndex+2,min(iBarWidth-5,iNameWidth));
	}
	DWORD	dwTextColor	=	WHITE;
	
	if	(isHighOperator())
		dwTextColor	=	LTYELLOW;
	
	if	(isHero() && g_hero.m_level.m_sCorrectValue	<	0)
		dwTextColor	=	LTRED;
	if(!bIsMovieMode)
	{		
		s_sText.cPut(x,y-iHeight+3-1,dwTextColor,strName);
	}	
	s_sText.scriptPutOn();	//	스크립트를 디코드해서 출력
	if(!bIsMovieMode)
	{		
		if	(m_wBody	<	dJOB_MONSTER_SAGE	||	m_wBody	>	dJOB_MONSTER_Exist_Variation_begin)
			if	(isMonster() && !isPet() && !isSummonBeast())
			{
				if	(m_wBody	<	dJOB_MONSTER_Guild_Crest	||	m_wBody	>	dJOB_MONSTER_GuildFlag)
				{
					int	iX	=	x-(iNameWidth/2+14);
					int	iY	=	y-(iHeight-9);
					
					if	(iType	!=	-1)
						g_sprMonsterDiscerner.Put(iX,iY,iType+eMD_KIND_UNDEAD);
					else
						g_sprMonsterDiscerner.Put(iX,iY,eMD_KIND_UNKNOWN);
					
					if	(bIsZinMonster)
						g_sprMonsterDiscerner.Put(iX,iY,eMD_ZIN_MONSTER);
					
					iX	+=	4;
					if	(iGrade	!=	-1)
						g_sprMonsterDiscerner.Put(iX,iY,iGrade+eMD_GRADE1);
				}
			}
	}	
	
	if	(isHero() && g_guild.m_wSerial != 0xffff)
	{
		CGuildMarkInfo	*lpInfo	=	&g_guild.m_guildMarkInfo;

		g_guildMark.putMark(x-iNameWidth/2-20,y-iHeight+7,
							lpInfo->m_wIcon,lpInfo->m_bIconColor,
							lpInfo->m_bBaseShape,lpInfo->m_bBaseColorPartition,lpInfo->m_bBaseColor1,lpInfo->m_bBaseColor2,lpInfo->m_bBaseLineColor,
							g_guild.m_wLevel,g_guild.m_wHallLevel,100,g_guild.m_bf1IsTwinkle);
	}
	else
	if	(isPlayer() && m_wGuildSerial	!=	0xffff)
	{
		CGuildMarkInfo2	*lpInfo	=	g_guildManager.getGuildMark(m_wGuildSerial);

		if (lpInfo	==	NULL)
		{
			if (g_hero.m_wWaitGuildMarkInfoTime	==	0)
			{
				g_hero.m_wWaitGuildMarkInfoTime	=	dSYNC_FPS*2;
				s_agent.sendAskGuildMarkInfo(m_wGuildSerial);
			}
		}
		else
		{
			g_guildMark.putMark(x-iNameWidth/2-20,y-iHeight+7,lpInfo->m_wIcon,lpInfo->m_bIconColor,
								lpInfo->m_bBaseShape,lpInfo->m_bBaseColorPartition,lpInfo->m_bBaseColor1,lpInfo->m_bBaseColor2,lpInfo->m_bBaseLineColor,
								lpInfo->m_bf8Level,lpInfo->m_bf3HallLevel,100,lpInfo->m_bf1IsTwinkle);
		}
	}

#ifdef	_FOR_JAPAN
	else
	if	(isPlayer() && m_wLevel < 20)
		g_anmNpcType.PutReg(x-iNameWidth/2-14,y-iHeight+9,eNPC_TYPE_BEGINNER,0,0);
#endif

	if (isPet())
	{
		iHeight	-=	7;

		if	(g_hero.isPet(m_wSerial) && m_wPetWaitCommand)
			g_im.putEffect(g_im.m_wPetAnalCommandImage,x-iNameWidth/2-8,y-iHeight,s_iFrameCounter);
		else
		if	(m_enchantedImage.m_bf1IsUseSpecialAttack)
			g_im.putEffect(g_im.m_wUseSpecialAttack,x-iNameWidth/2,y-iHeight,s_iFrameCounter);
		else
			g_im.putEffect(g_im.m_wPetMark,x-iNameWidth/2,y-iHeight,s_iFrameCounter);

		if	(bIsZinMonster)
			g_sprMonsterDiscerner.Put(x-iNameWidth/2-16,y-iHeight+3,eMD_ZIN_MONSTER);

		iHeight	+=	7;
	}

	if	(m_enchantedImage.m_bf1IsContract)
		g_im.putEffect(g_im.m_wEI_Contract,x,y-iHeight+26,s_iFrameCounter);
	if	(m_enchantedImage.m_bf1IsBloodCompact)
		g_im.putEffect(g_im.m_wEI_BloodCompact,x,y-iHeight+26,s_iFrameCounter);
	if	(m_enchantedImage.m_bf1IsSoulOath)
		g_im.putEffect(g_im.m_wEI_SoulOath,x,y-iHeight+26,s_iFrameCounter);
	if	(m_enchantedImage.m_bf1IsInterruptingArmor)
		g_im.putEffect(g_im.m_wEI_InterruptingArmor,x,y-iHeight+26,s_iFrameCounter);
	if	(m_bf1IsMarionette)
		g_im.putEffect(g_im.m_wEI_Marionette,x,y-iHeight-20,s_iFrameCounter);
	if	(m_enchantedImage.m_bf1IsHwabyung)
		g_im.putEffect(g_im.m_wEI_HwaByung,x+30,y-iHeight+25,s_iFrameCounter);
	if	(m_enchantedImage.m_bf1IsImpulse)
		g_im.putEffect(g_im.m_wEI_Impulse,x-30,y-iHeight+25,s_iFrameCounter);
	//	
	//	

	if	(m_enchantedImage.m_bf1IsBindingWords) // 언어구속
		g_im.putEffect(g_im.m_wEI_SS_Soul_BindingWords,x,y-iHeight+26,s_iFrameCounter);
	if	(m_enchantedImage.m_bf1IsFlee) // 공포..
		g_im.putEffect(g_im.m_wConfuseImage,x,y-iHeight+26,s_iFrameCounter);
	if	(m_enchantedImage.isExitEffect())
	{
		int		iEffectHeight	=	iHeight+10,i;
		int		iCount			=	0,iWidth=0;
		int		iEffectWidth	=	14,iCorrectWidth=0;
		LONGLONG	dwMask			=	0;
		if	(m_enchantedImage.m_bf1IsInterruptingWeapon) // 무기해제
		{
			++iCount;
		}
		if(m_enchantedImage.m_bf1IsStrengtheningCriticalDamage)
		{
			++iCount;
		}
		for (i=0;i<22;i++)
		{
			dwMask	=	1<<i;

			if	(m_enchantedImage.m_value1	&	dwMask)
				iCount++;
		}

		if	(iCount%2 == 1)
			iCount			=	1;
		else
		{
			iCount			=	0;
			iCorrectWidth	=	7;
		}

		for (i=0;i<22;i++)
		{
			dwMask	=	1<<i;

			if	(m_enchantedImage.m_value1	&	dwMask)
			{
				if (iCount % 2 == 0)
					iWidth	=	(iCount/2*iEffectWidth+iCorrectWidth);
				else
					iWidth	=	-(iCount/2*iEffectWidth+iCorrectWidth);

				if	(i==21)
					g_im.putEffect(g_im.m_wBreakWeapon,x+iWidth,y-iEffectHeight,s_iFrameCounter);
				else
				if	(i==20)
					g_im.putEffect(g_im.m_wBreakArmor,x+iWidth,y-iEffectHeight,s_iFrameCounter);
				else
				if	(i==19)
					g_im.put(g_im.m_wEI_PlotOfShadow,x+iWidth,y-iEffectHeight,0,0,0);
				else
				if	(i==18)
					g_im.put(g_im.m_wEI_ReversalPower,x+iWidth,y-iEffectHeight,0,0,0);
				else
				if	(i==17)
					g_im.put(g_im.m_wEI_BloodDrain,x+iWidth,y-iEffectHeight,0,0,0);
				else
				if	(i==16)
					g_im.put(g_im.m_wEI_Torture,x+iWidth,y-iEffectHeight,0,0,0);
				else
				if	(i>=8)
					g_im.put(g_im.m_wEI_DeclineStatus,x+iWidth,y-iEffectHeight,0,0,i-8);
				else
					g_im.put(g_im.m_wEI_InclineStatus,x+iWidth,y-iEffectHeight,0,0,i);

				iCount++;
			}
		}
		if	(m_enchantedImage.m_bf1IsInterruptingWeapon) // 무기해제
		{
			if (iCount % 2 == 0)
				iWidth	=	(iCount/2*iEffectWidth+iCorrectWidth);
			else
				iWidth	=	-(iCount/2*iEffectWidth+iCorrectWidth);
			g_im.putEffect(g_im.m_wEI_SS_Soul_InterruptingWeapon,x+iWidth,y-iEffectHeight,s_iFrameCounter);
			++iCount;
			
		}
		if	(m_enchantedImage.m_bf1IsStrengtheningCriticalDamage) // 치명타대미지증가..
		{
			if (iCount % 2 == 0)
				iWidth	=	(iCount/2*iEffectWidth+iCorrectWidth);
			else
				iWidth	=	-(iCount/2*iEffectWidth+iCorrectWidth);
			g_im.putEffect(g_im.m_wEI_SS_Soul_CruelSoul,x+iWidth,y-iEffectHeight,s_iFrameCounter);
			++iCount;

		}


		if	(m_enchantedImage.m_bf1IsNotificationOfDeath	||	m_enchantedImage.m_bf1IsZombieBomb)
		{
			if	(isHero() && g_hero.m_wNotificationOfDeathCounter)
				putCounter(g_im.m_wDeathCounter,g_hero.m_wNotificationOfDeathCounter,x,y-iHeight-40*m_wVerticalScale/100);
			else
				g_im.putEffect(g_im.m_wDeathCounter,x,y-iHeight-40*m_wVerticalScale/100,s_iFrameCounter+m_wSerial,10,0);
		}
		
		if	(m_enchantedImage.m_bf1IsAnimatePartner)
		{
			if	(isHero() && g_hero.m_wAnimatePartnerTime)
				putCounter(g_im.m_wDeathCounter,g_hero.m_wAnimatePartnerTime,x,y-iHeight-40*m_wVerticalScale/100);
			else
				g_im.putEffect(g_im.m_wDeathCounter,x,y-iHeight-40*m_wVerticalScale/100,s_iFrameCounter+m_wSerial,10,0);
		}
	}

	
	if	(m_enchantedImage.m_bf1IsAddAbsoluteAttackPower)		 // 절대공격력증가
	{
		g_im.putEffect(g_im.m_wEI_SS_Cham_Bull,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
	}
	else if	(m_enchantedImage.m_bf1IsIncreaseAttackCount)		 // 공격횟수증가
	{
		g_im.putEffect(g_im.m_wEI_SS_Cham_Hawk,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
	}
	else if	(m_enchantedImage.m_bf1IsIncreaseGetCP) // 획득CP증가
	{
		g_im.putEffect(g_im.m_wEI_SS_Cham_Bear,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
	}
	else if	(m_enchantedImage.m_bf1IsIncreaseAbsoluteCriticalPercent) // 절대크리티컬증가
	{
		g_im.putEffect(g_im.m_wEI_SS_Cham_Puma,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
	}
	else if	(m_enchantedImage.m_bf1IsDecreaseCoolTime) // 쿨타임감소
	{
		g_im.putEffect(g_im.m_wEI_SS_Cham_Snake,x,y,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);
	}

	BOOL	bIsLaziness	=	FALSE;

	if	(s_bIsVillage	==	FALSE)
		if	(g_hero.isPet(m_wSerial) || g_hero.isSummonBeastActor(m_wSerial))
		{
			int	iIdleTime	=	timeGetTime()	-	g_hero.m_dwLastReceiveCommandTime;

			if	(iIdleTime	>=	1000*60*5)
				bIsLaziness	=	TRUE;
		}

	if	(bIsLaziness	|| m_enchantedImage.m_bf1IsLaziness	)
		g_im.putEffect(g_im.m_wLaziness,x,y-iHeight-30,s_iFrameCounter+m_wSerial,0,0,g_iGameScale,g_iGameScale);

	m_wIsPutName	=	TRUE;

	if	(!isDeath())
		putHitEffect(x,y);
}

//
//	몸통만 찍기

void
CActor::putBody(int _iX,int _iY)
{ // 마지막 수정일 : 09.09.25
	cANM	*lpBody	=	getBody();
	cANM	*lpBodyVariation	=	NULL;

	if	(m_wBodyVariation)
	{
		lpBodyVariation	=	GetMonsterVariation(m_wBody,m_wBodyVariation);

		if	(!lpBodyVariation)
		{
			g_im.loadIMonsterBody(m_wBody,m_wBodyVariation);

			lpBodyVariation	=	GetMonsterVariation(m_wBody,m_wBodyVariation);
		}
	}

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_bf1IsHide || m_enchantedImage.m_bf1IsTransToWeapon)
		return;

	if	(m_wIsReceivedBasicData	==	FALSE)
		return;

//	코드 이런식으로 짜면 안되요. ㅠㅜ
	if	(m_wBody	==	dJOB_MONSTER_ARCH_DEVIL)
	{
		if	(m_wBlockerShape	!=	0xffff)
		{
			int	iFPS		=	g_anmMagicalShield.GetFPS(1);
			int	iFrameCount	=	g_anmMagicalShield.GetFrameCount(1);
			int	iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
			int	iCorrectHeight=	80*g_iGameScale/100;
			int	iScale		=	200*g_iGameScale/100;

			g_anmMagicalShield.PutReg(_iX,_iY-iCorrectHeight,1,0,iFrame,iScale,iScale);
		}
	}

	putEquipmentBack(_iX,_iY, m_iDirect, m_iAnm);							//	장비들을 찍어준다.

	if	(m_enchantedImage.m_bf1IsFogForm)
	{
		if	(isRequireAddPaint())
			return;

		lpBody->Put(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_SOFT);	//	캐릭터
	}
	else
	if	(isInvisivle())
	{
		putGGGBuffType(_iX,_iY);

		if	(isRequireAddPaint())
			return;

		int	iAlpha	=	dINVISIBLE_ALPHA_DEPTH;

		if	(isOwnTeam())
			iAlpha	=	16;

		lpBody->Put(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	캐릭터

		if	(lpBodyVariation)
			lpBodyVariation->Put(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	캐릭터
	}
	else
	if	(isBlur())
	{
		putGGGBuffType(_iX,_iY);

		if	(isRequireAddPaint())
			return;

		int	iAlpha	=	dBLUR_ALPHA_DEPTH;

		if	(isOwnTeam())
			iAlpha	=	16;

		lpBody->Put(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	캐릭터

		if	(lpBodyVariation)
			lpBodyVariation->Put(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	캐릭터
	}
	else
	{
		lpBody->PutReg(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	캐릭터

		if	(lpBodyVariation)
			lpBodyVariation->PutReg(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	캐릭터

		if	(m_wBody	==	dJOB_MONSTER_SAGE_MASTER	&&	isDeath()	==	FALSE)
		{
			if	(isInvincible())
				lpBody->PutReg(_iX,_iY,dACT_SAGE_MASTER_IRON_BALL	,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	캐릭터
			else
			{
				lpBody->PutReg(_iX,_iY,dACT_SAGE_MASTER_LIGHT_BALL	,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	캐릭터

				if	(m_enchantedImage.m_bf1IsLightningBarrier)
				{
					int	iAnm		=	dACT_SAGE_MASTER_LIGHTNING_BARRIER;
					int	iMaxFrame	=	lpBody->GetFrameCount(iAnm);
					int	iFPS		=	lpBody->GetFPS(iAnm);

					lpBody->PutReg(_iX,_iY,iAnm,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame,m_wHorizonScale,m_wVerticalScale);
				}
			}
		}
	}

	if	(m_wDuelBuffType)
	{
		cANM	*lpAnm	=	&g_aDuelBuff[m_wDuelBuffType-1];

		int	iAnm		=	0;
		int	iMaxFrame	=	lpAnm->GetFrameCount(iAnm);
		int	iFPS		=	lpAnm->GetFPS(iAnm);
		int	iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame;

		lpAnm->PutReg(_iX,_iY,0,0,iFrame);
	}
	
	

	if	(m_wOverlapAnm != 0xffff)
	{
		lpBody->Put(_iX,_iY,m_wOverlapAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	더미

		if	(lpBodyVariation)
			lpBodyVariation->Put(_iX,_iY,m_wOverlapAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	더미
	}

	if	(m_wOverlapAnm2!= 0xffff)
	{
		lpBody->Put(_iX,_iY,m_wOverlapAnm2,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	더미

		if	(lpBodyVariation)
			lpBodyVariation->Put(_iX,_iY,m_wOverlapAnm2,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	더미
	}

//	코드 이런식으로 짜면 안되요. ㅠㅜ
	if	(m_wBody	==	dJOB_MONSTER_ARCH_DEVIL)
	{
		if	(m_wBlockerShape	!=	0xffff)
		{
			int	iFPS		=	g_anmMagicalShield.GetFPS(0);
			int	iFrameCount	=	g_anmMagicalShield.GetFrameCount(0);
			int	iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iFrameCount;
			int	iCorrectHeight=	80*g_iGameScale/100;
			int	iScale		=	200*g_iGameScale/100;

			g_anmMagicalShield.PutReg(_iX,_iY-iCorrectHeight,0,0,iFrame,iScale,iScale);
		}
	}

	if	(m_wBody	==	dJOB_MONSTER_GuildFlag)
	{
		int	iMaxFrame	=	g_anmGuildFlag.GetFrameCount(0);
		int	iFPS		=	g_anmGuildFlag.GetFPS(0);
		
		g_anmGuildFlag.Put(_iX,_iY,0,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame);
	}

	//if	(isRequireAddPaint()	==	FALSE	||	isHero())	//09.09.25 주석 처리
		putEquipmentFront(_iX,_iY, m_iDirect, m_iAnm);							//	장비들을 찍어준다.

	putGGGBuffType(_iX,_iY);

	if(m_enchantedImage.m_bf1IsFightingSpirit)
	{
		if(lpBody->m_sprite.m_isLayer)
		{

			WORD	wRed	=	144;
			WORD	wGreen	=	254;
			WORD	wBlue	=	46;
/*
			int iRange		=	200;
			int iValue		=	122;
			
			int iColorFrame = (s_iFrameCounter+1)/5 %iRange;
					
			iColorFrame = iColorFrame%iRange/2;
			iColorFrame -= iRange/4;

			iValue += abs(iColorFrame);
*/			
			DWORD	dwColor = RGB( wRed,wGreen,wBlue);


			lpBody->PutLayer(_iX,_iY,m_iAnm,m_iDirect,m_iFrame,dwColor,m_wHorizonScale,m_wVerticalScale);	
		}
	}
}

//JBC	08-10-15	거신의 은총 듀얼 버프를 쓴다....-_-;;
inline void
CActor::putGGGBuffType(int _iX , int _iY)
{
	if	(m_wGGGBuffType)
	{
		cANM	*lpAnm	=	&g_aGGGBuff[m_wGGGBuffType-1];
		
		int	iAnm		=	0;
		int	iMaxFrame	=	lpAnm->GetFrameCount(iAnm);
		int	iFPS		=	lpAnm->GetFPS(iAnm);
		int	iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame;
		
		lpAnm->PutReg(_iX,_iY,0,0,iFrame);
	}
}

//
//	몸통만 찍기

inline	void
CActor::putBodyHalf(int _iX,int _iY,int _iFrame,int _iDirect)
{
	cANM	*lpBody	=	getBody();
	cANM	*lpBodyVariation	=	NULL;

	if	(m_wBodyVariation)
	{
		lpBodyVariation	=	GetMonsterVariation(m_wBody,m_wBodyVariation);

		if	(!lpBodyVariation)
		{
			g_im.loadIMonsterBody(m_wBody,m_wBodyVariation);

			lpBodyVariation	=	GetMonsterVariation(m_wBody,m_wBodyVariation);
		}
	}

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_bf1IsHide || m_enchantedImage.m_bf1IsTransToWeapon)
		return;

	if	(m_wIsReceivedBasicData	==	FALSE)
		return;

	int	iSaveFrame	=	m_iFrame;

	int iDirect = _iDirect;

	m_iFrame	=	_iFrame;
	putBackWeaponHB(_iX,_iY, _iDirect , m_iAnm);
	putBackShieldHB(_iX,_iY, _iDirect, m_iAnm);

	lpBody->Put(_iX,_iY,m_iAnm,iDirect,_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);	//	캐릭터
	if	(lpBodyVariation)
		lpBodyVariation->Put(_iX,_iY,m_iAnm,iDirect,_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);	//	캐릭터

	if	(m_wOverlapAnm != 0xffff)
	{
		lpBody->Put(_iX,_iY,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);	//	더미

		if	(lpBodyVariation)
			lpBodyVariation->Put(_iX,_iY,m_wOverlapAnm,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);	//	더미
	}

	if	(m_wOverlapAnm2!= 0xffff)
	{
		lpBody->Put(_iX,_iY,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);	//	더미

		if	(lpBodyVariation)
			lpBodyVariation->Put(_iX,_iY,m_wOverlapAnm2,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);	//	더미
	}

	putFrontWeaponHB(_iX,_iY, _iDirect, m_iAnm);
	putFrontShieldHB(_iX,_iY, _iDirect, m_iAnm);

	m_iFrame	=	iSaveFrame;
}


//
//	바닥에 깔리는 이미지 출력
void
CActor::putBottomImage()
{
	if	(m_wChargeImage	==	0xffff)
		return;
//	m_wChargeFrame	=	0;			//	충전 이미지
}

void
CActor::putFeignDeath()
{
	int	iAnm	=	m_iAnm;
	int	iDirect	=	m_iDirect;
	int	iFrame	=	m_iFrame;

	m_iAnm		=	dACT_DEAD;
	m_iDirect	=	0;
	m_iFrame	=	getFrameCount(dACT_DEAD)-1;

	int	x		=	GetScaledXPos(m_posFeignDeath.x);
	int	y		=	GetScaledYPos(m_posFeignDeath.y);

	putShadow(x,y);
	putBody(x,y);

	m_iAnm		=	iAnm;
	m_iDirect	=	iDirect;
	m_iFrame	=	iFrame;
}

void
CActor::putShakle(int _iX,int _iY)
{
	if	(m_wShakleSkill	==	0xffff)
		return;

	CSkill	*lpSkill	=	&g_aSkill[m_wShakleSkill];
	cANM	*lpShakle	=	g_im.getEffect(lpSkill->m_wSkillImage);

	if	(!lpShakle)
		return;

	int	iDX	=	0;

	if	(m_wShakleShakeTime)
		iDX	=	(random(10)-5)*g_iGameScale/100;

	int	iHorizonScale	=	m_wHorizonScale*lpSkill->m_wImageScale/100;
	int	iVerticalScale	=	m_wVerticalScale*lpSkill->m_wImageScale/100;

	lpShakle->PutReg(_iX+iDX,_iY,m_wShakleAnm,0,m_wShakleFrame,iHorizonScale,iVerticalScale);
}

void
CActor::putTheOtherSelf(int _iX,int _iY,BOOL _bIsFront)
{
	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_wTheOtherSelfCount	==	0)
		return;

	int		iAngleGap	=	30;
	int		iBeginAngle	=	m_iAngle+180;
	int		iAngle;
	CPos	pos;

	if	(m_wTheOtherSelfCount%2	==	0)
		iBeginAngle	+=	iAngleGap/2;

	for (int i=0;i<m_wTheOtherSelfCount;i++)
	{
		int	iAddAngle	=	iAngleGap*((i+1)/2);

		if (i%2)
			iAddAngle	=	-iAddAngle;

		iAngle			=	(iBeginAngle+iAddAngle)%360;

		pos.x			=	_iX;
		pos.y			=	_iY;

		if (_bIsFront)
			if (iAngle	<	180)
				continue;

		GetOvalAnglePos(&pos,iAngle,60*m_wHorizonScale/100);

		lpBody->Put(pos.x,pos.y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_NORMAL,20);
	}
}

void
CActor::putBit(int _iX,int _iY)
{

	for(int i=0;i<m_bitInfoManager.m_iCount;++i)
	{
		int iX	=	GetScaledXPos(m_bitInfoManager.m_bitInfo[i].m_pos.x);
		int iY	=	GetScaledYPos(m_bitInfoManager.m_bitInfo[i].m_pos.y) + getHeight();
		if(g_hero.m_iSerial == m_bitInfoManager.m_bitInfo[i].m_wTargetSerial &&  m_bitInfoManager.m_bitInfo[i].m_dwNameHashCode == GetHashCode(g_hero.m_strName))
		{
			g_im.putEffect(g_im.m_wEI_RedBit, iX, iY, s_iFrameCounter, 0,0,g_iGameScale,g_iGameScale);	
		}
		else
		{
			g_im.putEffect(g_im.m_wEI_WhiteBit, iX, iY, s_iFrameCounter, 0,0,g_iGameScale,g_iGameScale);
		}
	}
}

//
//	출력
void
CActor::put()
{
	if	(m_wBody	==	dJOB_MONSTER_TIAMATH_FEELER	||	m_wBody	==	dJOB_MONSTER_TIAMATH_BOTTOM_FEELER)
	{
		if	(g_am.m_iIsHideTimathFeelerTime)
			return;
	}

	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_bf1IsHide || m_enchantedImage.m_bf1IsTransToWeapon)
		return;

	if	(m_wIsRideTamer)
	{
		setPalette();
		return;
	}

	if	(g_lpHero->m_bf1IsQuizEventLoser == FALSE && g_hero.isOperator() == FALSE)
		if	(m_bf1IsQuizEventLoser && !isHero() )
			return;

	if	(m_wIsReceivedBasicData	==	FALSE)
		return;

	if	(!checkEquipmentShape())
		return;

	if	(putDummy())
		return;		//	이넘.. 더미다-_-

	if	(isHero() && g_hero.m_wSlaveMonster != 0xffff)
		return;

	if	(m_wIsFeignDeath	||	m_enchantedImage.m_bf1IsFeignDeath)
	{
		if	(m_enchantedImage.m_bf1IsFeignDeath)
		{
			m_posFeignDeath.x	=	m_pos.x;
			m_posFeignDeath.y	=	m_pos.y;
		}
		putFeignDeath();

		return;
	}

	int		x,y,iDx=0,iDy=0;

	x		=	GetScaledXPos(m_pos.x);
	y		=	GetScaledYPos(m_pos.y);

	if	(m_wRushStatus	==	eRS_ROLLING_LOG)
	{
		cANM	*lpRollingLog	=	g_im.getEffect(g_im.m_wRollinggLog);

		if	(lpRollingLog)
		{
			int	iDirect		=	GetDirect(m_iAngle,lpRollingLog->GetDirectCount(0));

			g_im.putEffect(g_im.m_wRollinggLog,x,y,s_iFrameCounter,0,iDirect,m_wHorizonScale,m_wVerticalScale);
//			lpRollingLog->PutReg(x,y,0,iDirect,0,m_wHorizonScale,m_wVerticalScale);
			putHitEffect(x,y);	//	타격 이미지 출력
		}

		return;
	}

	//	매직 박스 출력
	{
		if	(m_wReleaseMagicBoxAlpha!=	0xffff)	//	매직 박스가 그냥 사라진다.
		{
		}
		else
		if	(m_wCrushMagicBoxFrame	!=	0xffff)	//	매직 박스가 뽀개진다.
		{
			cANM	*lpMagicBox	=	g_im.getEffect(g_im.m_wShutInMagicBox);

			if	(!lpMagicBox)
				return;

			g_im.putEffect(g_im.m_wShutInMagicBox,x,y,m_wCrushMagicBoxFrame,2,0,m_wHorizonScale,m_wVerticalScale);
		}
		else
		if	(m_wShutInMagicBoxImage	==	0xffff	&&	m_enchantedImage.m_bf1IsShutInMagicBox)
		{
			cANM	*lpMagicBox	=	g_im.getEffect(g_im.m_wShutInMagicBox);

			if	(!lpMagicBox)
				return;

			lpMagicBox->PutShadow(x,y,3,0,0,m_wHorizonScale,m_wVerticalScale);
			lpMagicBox->Put(x,y,3,0,0,m_wHorizonScale,m_wVerticalScale);
			return;
		}
	}

	int	iAnm=m_iAnm,iFrame=m_iFrame;

	if (m_wTheOtherSelfCount)
	{
		int		iAngleGap	=	30;
		int		iBeginAngle	=	m_iAngle+180;
		int		iAngle;
		CPos	pos;

		if (m_wTheOtherSelfCount%2	==	0)
			iBeginAngle	+=	iAngleGap/2;

		for (int i=0;i<m_wTheOtherSelfCount;i++)
		{
			int	iAddAngle	=	iAngleGap*((i+1)/2);

			if	(i%2)
				iAddAngle	=	-iAddAngle;

			iAngle			=	(iBeginAngle+iAddAngle)%360;

			pos.x			=	x;
			pos.y			=	y;

			GetOvalAnglePos(&pos,iAngle,60*m_wHorizonScale/100);

			lpBody->Put(pos.x,pos.y-m_sLevitateHeight,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_NORMAL,24);
		}
	}

	putHill(iDx,iDy);

	if (m_wMagicCarpetType)
	{
		int		iCarpetAnm		=	0;

		if	(m_isMove	==	FALSE)
			iCarpetAnm			=	1;

		int	iMagicCarpetShape	=	m_wMagicCarpetType;

		cANM	*lpCarpet		=	NULL;

		if	(m_wMagicCarpetType	==	eMCS_RED_DRAKE)
		{
			lpCarpet		=	&g_aAnmMagicCarpet[eMCS_DRAKE-1];
			lpCarpet->setPalette(g_pRedDrakePalette);
		}
		else
		{
			lpCarpet		=	&g_aAnmMagicCarpet[m_wMagicCarpetType-1];
			lpCarpet->restorePalette();
		}
		if(lpCarpet)
		{
			int		iCarpetFrame	=	(s_iFrameCounter+m_wSerial)*lpCarpet->GetFPS(iCarpetAnm)/dSYNC_FPS;
			iCarpetFrame			=	iCarpetFrame%lpCarpet->GetFrameCount(iCarpetAnm);
			lpCarpet->PutShadow(x,y,iCarpetAnm,m_iDirect,iCarpetFrame,m_wHorizonScale,m_wVerticalScale);
			
			y						=	y+dSIN_TABLE[(s_iFrameCounter+m_wSerial)*5%360]*3/256*m_wVerticalScale/100-8*m_wVerticalScale/100;//-getHeight();
			
			lpCarpet->PutReg(x,y,iCarpetAnm,m_iDirect,iCarpetFrame,m_wHorizonScale,m_wVerticalScale);
			
			if	(m_wMagicCarpetType	==	eMCS_GEAR)
			{
				if	(m_iAnm	==	dACT_WALK	||	m_iAnm	==	dACT_RUN)
				{
					m_iAnm			=	dACT_READY;	
					
					int	iFrameCount	=	lpBody->GetFrameCount(m_iAnm);
					
					if	(iFrameCount	<=	0)
						return;
					
					m_iFrame		=	(s_iFrameCounter*lpBody->GetFPS(m_iAnm)/dSYNC_FPS)%iFrameCount;
				}
			}
			else
			{
				m_iAnm				=	dACT_SITDOWN;
				m_iFrame			=	0;
			}
		}
	}

	if	(m_wShakeTime)	//	방패 막기 효과
		if	((s_iFrameCounter+m_wSerial) % 4 < 2)
		{
			CPos	pos;

			pos.Set(x,y);

			GetOvalAnglePos(&pos,(m_iAngle+180)%360,2);

			iDx	=	x-pos.x;
			iDy	=	y-pos.y;
			x	=	pos.x;
			y	=	pos.y;
		}

	if	(m_wAuraImage	!=	0xffff)
	{
		g_hem.put(m_wAuraImage,x,y);	//	오라 이미지
	}

	setPalette();

	if	(isDeath())
	{
		if	(m_wCorpseTime	!=	0xffff)
		{
			int	iAlpha		=	m_wCorpseTime/4;

			if	(iAlpha	>	32)
				lpBody->PutShadow(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	캐릭터의 그림자
			else
				lpBody->PutShadow(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,iAlpha/2);	//	캐릭터의 그림자

			if	(lpBody->m_pAnmData[m_iAnm].m_iALPHA	==	dPUT_NORMAL)
				lpBody->Put(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	캐릭터
			else
				lpBody->PutReg(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);	//	캐릭터

			putHitEffect(x,y);											//	타격 이미지 출력

			return;
		}

		putHitEffect(x,y);											//	타격 이미지 출력
	}

#ifdef	dDEBUG_MODE
	putDebugInfo1(x,y);
#endif

	putAttatchedFlatImage(x,y,FALSE);
//	그림자다!!
	{
		int	iCorrectX	=	getHeight(FALSE)/2;
		int	iCorrectY	=	getHeight(FALSE);

		if	(m_wBody	==	dJOB_MONSTER_ARCH_DEVIL	&&	isEnchantedLevitate())
			iCorrectY	=	-m_sLevitateHeight;

		putShadow(x- iCorrectX,y + iCorrectY);//	그림자들
	}

	y	-=	getHeight();

	putBackAdditionalEffect(x,y);
	putAdditionalEffect(x,y,FALSE);							//	추가 이미지들 출력

//	장비나 기타 등등 캐릭터 몸에 달리는 것들!!! 그넘들을 중에 캐릭터보다 먼저 찍어야 할 넘들만 골라 찍는다!!!

	putBody(x,y);
	putBit(x,y);
	putAttatchedFlatImage(x,y+getHeight(),TRUE);

	if	(m_wGuildPetType)
	{
		int	iFPS		=	g_aGuildPet[m_wGuildPetType-1].GetFPS(0);
		int	iMaxFrame	=	max(g_aGuildPet[m_wGuildPetType-1].GetFrameCount(0),1);
		int	iFrame		=	(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame;
		int	iHeight		=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100-10;
		int	iDX			=	20;

		if	(m_wMiniPetType2	<	nsMiniPetType::Count)
			iDX	+=	20;

		g_aGuildPet[m_wGuildPetType-1].PutReg(x+iDX,y-iHeight,0,0,iFrame);
	}

	if(!isInvisivle() ||  (isInvisivle() && g_lpHero->m_wTeam == m_wTeam))
	{
		if	(m_wMiniPetType	<	nsMiniPetType::Count)
		{
			cANM	*lpMiniPet	=	&g_aMiniPetBody[m_wMiniPetType];
			int		iHeight		=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100 - 25;
			int		iWidth		=	40*m_wHorizonScale/100 - 30;		
			int	x		=	GetScaledXPos(m_posPet.x);
			int y		=	GetScaledYPos(m_posPet.y);			
			x	-=	iWidth;
			y	-=	iHeight;
			int iSinHeight	= dSIN_TABLE[(s_iFrameCounter+m_wSerial)*5%360]*8/256;
			y+= iSinHeight;
			lpMiniPet->PutReg(x,y,m_iMiniPetAnm,m_iDirect,m_iMiniPetFrame,m_wHorizonScale,m_wVerticalScale);


		}
		
		if	(m_wMiniPetType2	<	nsMiniPetType::Count)
		{
			cANM	*lpMiniPet	=	&g_aMiniPetBody[m_wMiniPetType2];
			int		iHeight		=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100-25;
			int		iWidth		=	40*m_wHorizonScale/100 - 30;
			int	x		=	GetScaledXPos(m_posPet2.x);
			int y		=	GetScaledYPos(m_posPet2.y);	
			x	-=	iWidth;
			y	-=	iHeight;
			int iSinHeight	= dCOS_TABLE[(s_iFrameCounter+m_wSerial)*5%360]*8/256;
			y+= iSinHeight;
			lpMiniPet->PutReg(x,y,m_iMiniPetAnm2,m_iDirect,m_iMiniPetFrame2,m_wHorizonScale,m_wVerticalScale);
		}
	}

	putFrontAdditionalEffect(x,y);
	putAdditionalEffect(x,y,TRUE);											//	타격 이미지 출력

#ifdef	dDEBUG_MODE
//	s_bText.cPut(x,y,LTYELLOW,"%d",m_wSerial);
//	if (m_bContinuousHitCounter)	s_bText.cPut(x,y,LTYELLOW,"Combo %d",m_bContinuousHitCounter);
	putDebugInfo2(x,y);
#endif

	if	(m_wReleaseMagicBoxAlpha	!=	0xffff)	//	매직 박스가 그냥 사라진다.
	{
		cANM	*lpMagicBox	=	g_im.getEffect(g_im.m_wShutInMagicBox);

		if	(!lpMagicBox)
			return;

		int	iAlpha		=	m_wReleaseMagicBoxAlpha/2;

		if	(iAlpha	>	32)
			lpMagicBox->PutShadow(x,y,3,0,0,m_wHorizonScale,m_wVerticalScale);	//	캐릭터의 그림자
		else
			lpMagicBox->PutShadow(x,y,3,0,0,m_wHorizonScale,m_wVerticalScale,iAlpha/2);	//	캐릭터의 그림자

		lpMagicBox->Put(x,y,3,0,0,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);
	}

	m_iAnm		=	iAnm;
	m_iFrame	=	iFrame;
}

void
CActor::drawPetMark()
{
	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(!isPet())
		return;

	if	(m_wIsPutName)
		return;

	int	x		=	m_pos.x	-	g_map.m_pos.x;
	int	y		=	m_pos.y	-	g_map.m_pos.y;
	int	iHeight	=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100+20;

	if	(isDeath())
	{
		cRECT	rect;

		rect.set(0,0,0,0);
		lpBody->getRect(m_iAnm,m_iDirect,m_iFrame,&rect,m_wVerticalScale);
		rect.add(x,y);

		x		=	rect.x1+rect.getWidth()/2;
		iHeight	=	y-(rect.y1-20);
	}

	g_im.putEffect(g_im.m_wPetMark,x+11,y-iHeight+7,s_iFrameCounter);
}

//
//	출력
void
CActor::putForModel(int _iX,int _iY,BOOL _bIsFocus)
{
	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	int	x,y;

	x	=	_iX;
	y	=	_iY;

	WORD	saTempPlt[256];

	if	(_bIsFocus)	//	마우스가 올라가 있는넘
	{
		EffectPalette(dPLT_EFFECT_NORMAL,lpBody->m_sprite.m_pPlt,saTempPlt,150,0);

		lpBody->m_sprite.setPalette(saTempPlt);
	}
	else
	{
		EffectPalette(dPLT_EFFECT_GRAY,lpBody->m_sprite.m_pPlt,saTempPlt,150,0);

		lpBody->m_sprite.setPalette(saTempPlt);
	}

	m_wIsReceivedBasicData	=	TRUE;

//	그림자다!!
	putShadow(x- getHeight()/2,y + getHeight());//	그림자들

//	장비나 기타 등등 캐릭터 몸에 달리는 것들!!! 그넘들을 중에 캐릭터보다 먼저 찍어야 할 넘들만 골라 찍는다!!!
	putBody(x,y);
}

void
CActor::putCastImage(int x,int y)
{
	CSkill	*lpSkill	=	m_abilityUse.getSkill();

	if	(!lpSkill)
		return;

	if	(lpSkill->m_wSwingImage	!=	0xffff)
	{
		g_apEffect[lpSkill->m_wSwingImage]->PutReg(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);

		if	(m_wOverlapAnm != 0xffff)
			g_apEffect[lpSkill->m_wSwingImage]->PutReg(x,y,m_wOverlapAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);

		if	(m_wOverlapAnm2!= 0xffff)
			g_apEffect[lpSkill->m_wSwingImage]->PutReg(x,y,m_wOverlapAnm2,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
	}

	if	(lpSkill->m_wCastImage	!=	0xffff)
	{
		cANM	*lpCastImage	=	(cANM *)g_im.getEffect(lpSkill->m_wCastImage);

		if	(lpCastImage)
		{
			int		iDirect		=	m_iDirect;

			iDirect				=	min(iDirect,lpCastImage->GetDirectCount(0)-1);
			iDirect				=	max(iDirect,0);

			if	(lpSkill->m_bf1IsCastImageOnReleasPos)
			{
				CPos	pos;
				
				getReleasePos(&pos);

				int	iX	=	GetScaledXPos(pos.x);
				int	iY	=	GetScaledYPos(pos.y);

				lpCastImage->PutReg(iX,iY,0,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
			}
			else
			{
				CPos	pos;

				getHitPointCorrectPos(lpSkill->m_wCastImageOutputPart,&pos);

				x		+=	pos.x;
				y		+=	pos.y;

				lpCastImage->PutReg(x,y,0,iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale);
			}
		}
	}
}

//
//	덮어찍기 -_-... 덮밥이 아니란 말이다!!!
void
CActor::addPaint()
{
	// 마지막 수정일 : 09.09.25
	if	(m_wBody	==	dJOB_MONSTER_TIAMATH_FEELER	||	m_wBody	==	dJOB_MONSTER_TIAMATH_BOTTOM_FEELER)
	{
		if	(g_am.m_iIsHideTimathFeelerTime)
			return;
	}

	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_bf1IsHide || m_enchantedImage.m_bf1IsTransToWeapon)
		return;

	if	(m_wShutInMagicBoxImage	==	0xffff	&&	m_enchantedImage.m_bf1IsShutInMagicBox)
		return;

	if	(m_wRushStatus	==	eRS_ROLLING_LOG)
		return;

	if	(isDummy())
		return;

	if	(isPet())
		m_wIsRideTamer	=	m_wIsRideTamer;

	if	(m_wIsRideTamer)
		return;

	if	(g_lpHero->m_bf1IsQuizEventLoser == FALSE && g_hero.isOperator() == FALSE)
		if (m_bf1IsQuizEventLoser && !isHero())
			return;

	if	(m_wIsReceivedBasicData == FALSE || m_wIsFeignDeath || m_wIsAddPaint)
		return;

	m_wIsAddPaint	=	TRUE;

	int	x,y,i=0;

	x		=	GetScaledXPos(m_pos.x);
	y		=	GetScaledYPos(m_pos.y)-getHeight();

/*	if ((isInvisivle() || isBlur()) && !isOwnTeam())
	{
		if (isVisibleName())
			putName(x,y);	//	우리팀

		return;
		}
	*/
	int	iAnm	=	m_iAnm;
	int	iDirect	=	m_iDirect;
	int	iFrame	=	m_iFrame;

	if	(m_enchantedImage.m_bf1IsFeignDeath)
	{
		m_iAnm		=	dACT_DEAD;
		m_iDirect	=	0;
		m_iFrame	=	getFrameCount(dACT_DEAD)-1;
	}

//	return;

	if	(m_wMagicCarpetType)
	{
		int		iAnm			=	0;

		if	(m_isMove	==	FALSE)
			iAnm				=	1;

		cANM	*lpCarpet		=	NULL;

		if	(m_wMagicCarpetType	==	eMCS_RED_DRAKE)
		{
			lpCarpet		=	&g_aAnmMagicCarpet[eMCS_DRAKE-1];
			lpCarpet->setPalette(g_pRedDrakePalette);
		}
		else
		{
			lpCarpet		=	&g_aAnmMagicCarpet[m_wMagicCarpetType-1];
			lpCarpet->restorePalette();
		}

		int		iCarpetFrame	=	(s_iFrameCounter+m_wSerial)*lpCarpet->GetFPS(iAnm)/dSYNC_FPS;
		iCarpetFrame			=	iCarpetFrame%lpCarpet->GetFrameCount(iAnm);

		y						=	y+dSIN_TABLE[(s_iFrameCounter+m_wSerial)*5%360]*3/256*m_wVerticalScale/100-8*m_wVerticalScale/100;
		lpCarpet->Put(x,y+getHeight(),iAnm,m_iDirect,iCarpetFrame,m_wHorizonScale,m_wVerticalScale,dPUT_HALF_BLENDING);

		if (m_wMagicCarpetType	==	eMCS_GEAR)
		{
			if (m_iAnm	==	dACT_WALK	||	m_iAnm	==	dACT_RUN)
			{
				m_iAnm			=	dACT_READY;	
				int	iFrameCount	=	lpBody->GetFrameCount(m_iAnm);

				if	(iFrameCount<=	0)
					return;

				m_iFrame		=	(s_iFrameCounter*lpBody->GetFPS(m_iAnm)/dSYNC_FPS)%iFrameCount;
			}
		}
		else
		{
			m_iAnm				=	dACT_SITDOWN;
			m_iFrame			=	0;
		}
	}

	setPalette();

	putBackWeaponHB(x,y, iDirect,m_iAnm);
	putBackShieldHB(x,y, iDirect,m_iAnm);

	int	iAlpha	=	16;

	if	(isInvisivle())
	{
		if (!isOwnTeam())
			iAlpha	=	dINVISIBLE_ALPHA_DEPTH;
	}
	else
	if	(isBlur())
	{
		if (!isOwnTeam())
			iAlpha	=	dBLUR_ALPHA_DEPTH;
	}

	if	(m_wBody	==	dJOB_MONSTER_SAGE_MASTER	&&	isInvincible())
	{
		lpBody->Put(x,y,dACT_SAGE_MASTER_IRON_BALL	,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	캐릭터
		putHitEffect(x,y);
	}
	else
	{
		int	iLinkAnm	=	lpBody->m_pAnmData[m_iAnm].m_iLinkAnm;

		if	(lpBody->m_pAnmData[m_iAnm].m_iALPHA	!=	dPUT_NORMAL	&&	iLinkAnm	!=	0xffff)
		{
			if	(lpBody->m_pAnmData[iLinkAnm].m_iALPHA	==	dPUT_NORMAL)
				lpBody->Put(x,y,iLinkAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	캐릭터
		}
		else
		{
			if	(lpBody->m_pAnmData[m_iAnm].m_iALPHA	==	dPUT_NORMAL)
				lpBody->Put(x,y,m_iAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);	//	캐릭터
		}

		if	(m_wOverlapAnm != 0xffff)
			lpBody->Put(x,y,m_wOverlapAnm,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);
		if	(m_wOverlapAnm2!= 0xffff)
			lpBody->Put(x,y,m_wOverlapAnm2,m_iDirect,m_iFrame,m_wHorizonScale,m_wVerticalScale,dPUT_ALPHA_BLENDING,iAlpha);

		if	(isVisibleName())
		{
			if	(isDeath()	&&	m_wCorpseTime	==	0xffff)
				m_wIsPutName	=	TRUE;

// 			if (g_lpHero && (g_lpHero->isBlind() || g_lpHero->isNewBlindVerySmall()|| g_lpHero->isNewBlindSmall() || g_lpHero->isNewBlindMiddle() || 
// 				g_lpHero->isNewBlindLarge() || g_lpHero->isNewBlindVeryLarge())&& g_lpHero->m_wOperatorLevel < 4)
// 					m_wIsPutName = FALSE;
				
			putName(x,y);	//	전투중인 녀석
		}
		else
		if	(s_bIsGuildBattleField)
			putName(x,y,TRUE);	//	전투중인 녀석
// 		else
// 			putName(x,y);	

		putFrontWeaponHB(x,y, iDirect,m_iAnm);		
		putFrontShieldHB(x,y, iDirect,m_iAnm);		

		if	(m_wBody	==	dJOB_MONSTER_SAGE)
		{
			int	iAnm		=	0;

			int	iMaxFrame	=	g_anmSageStone.GetFrameCount(iAnm);
			int	iFPS		=	g_anmSageStone.GetFPS(iAnm);

			setSageBookPalette();

			g_anmSageStone.PutReg(x,y,iAnm,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame,m_wHorizonScale,m_wVerticalScale);
			g_anmSageStone.PutReg(x,y,iAnm+2,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame,m_wHorizonScale,m_wVerticalScale);
			g_anmSageStone.restorePalette();
		}
	}

	if	(m_wBody	==	dJOB_MONSTER_GuildFlag)
	{
		int	iMaxFrame	=	g_anmGuildFlag.GetFrameCount(0);
		int	iFPS		=	g_anmGuildFlag.GetFPS(0);
		
		g_anmGuildFlag.Put(x,y,0,0,(s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame);
	}

	putDialog(x,y);

	m_iAnm		=	iAnm;
	m_iDirect	=	iDirect;
	m_iFrame	=	iFrame;

	putCastImage(x,y);

//	if	(m_bf1IsHide)
//		s_text.cPut(x,y,WHITE,"Hide");
}


//	잔상 효과를 켠다.
void
CActor::afterImageOn(int _iEffect,int _iCount,int _iDepth,int _iReduce,int _iDelayPeriod)
{
	if (_iEffect	== 0xffff)
		m_cAfterImageStatus		=	dPUT_AFTER_IMAGE;
	else
		m_cAfterImageStatus		=	_iEffect;

	m_cAfterImageCount			=	min(_iCount,dAFTER_IMAGE_COUNT);

	m_cAfterImageDepth			=	min(max(_iDepth,10),32);		//	캐릭터 잔상 수
	m_cAfterImageDisappearSpeed	=	min(max(_iReduce,1),10);		//	캐릭터 잔상 수
	m_wAfterImageDelayPeriod	=	_iDelayPeriod;
}

//
//	대사 출력
void
CActor::putDialog(int _iX,int _iY)
{
	if	(g_bIsObserverModeForDuel)
		return;

	cANM	*lpBody	=	getBody();

	if	(!lpBody	||	m_iAnm	>=	lpBody->m_iAnmCount	)
		return;

	if	(m_wSyncStatusWithServer)
		return;
	if	(m_wTalkTime	==	0)
		return;
#ifdef	_DEBUG
	if	(g_bIsHideDamageAndHideName	&&	isPlayer())
		return;
	if	(g_bIsMovieMode	&&	isPlayer())
		return;
#endif

	if	(isPlayer() && g_pitchmanShop.isExistCrashShop(m_pos.x,m_pos.y))
		return;

//	if	(isMonster()	&&	!isPet() && !isSummonBeast())
//	{
//		m_wTalkTime	=	0;
//		return;
//	}

	if	(isPlayer()	==	FALSE)
		if	(g_lpHero)
			setDirect(getDirect(g_lpHero->m_pos.x,g_lpHero->m_pos.y,TRUE));

	char	strText[512];
	CPos	pos;
	cRECT	rect;

	int		iBodyHeight	=	lpBody->m_sprite.m_iHeight*m_wVerticalScale/100+10;

	if	(m_iAnm	==	dACT_SITDOWN)
		iBodyHeight	=	iBodyHeight*3/5;

	pos.x	=	_iX;
	pos.y	=	_iY-iBodyHeight;

//	if (m_wIsWarningMessage)
	strcpy(strText,m_strTalk);
//	else	sprintf(strText,"<b>%s<b>\n%s",m_strName,m_strTalk);

	int		iWidth		=	120;
	int		iHeight		=	s_text.getTextHeight(iWidth,strText);
	int		iArrowPos	=	20;

	{
		iWidth	=	min(s_text.getPixelSize(strText),iWidth);
		iWidth	=	max(iWidth,iArrowPos*2);

		rect.set(pos.x-iArrowPos,pos.y-iHeight,pos.x-iArrowPos+iWidth,pos.y);

		s_ttDialogFrame.drawBoxAddBorderSize(&rect,_WHITE,32);
		g_sprInterface.Put(rect.x1+iArrowPos,rect.y2+1,eDIALOG_FRAME_ARROW);
	}

	s_text.disableFitRightEdge();
	s_text.pushShadowStatus(FALSE);
	if	(isPlayer())
		s_text.someCodeIgnoreOn();
	s_text.put(rect.x1,rect.y1,iWidth,iHeight,0,BLACK,strText);
	if	(isPlayer())
		s_text.someCodeIgnoreOff();
	s_text.popShadowStatus();
#ifndef	_FOR_ENG
	s_text.enableFitRightEdge();
#endif
}

//
//	힐 효과
void
CActor::makeHealEffect(int _iSkill,int _iHealPoint)
{
	int	iHitEffect		=	getFreeHitIndex();

	CSkill	*lpSkill	=	&g_aSkill[_iSkill];
	CPos	pos;

	getHitPointCorrectPos(lpSkill->m_wHealImageOutputPart,&pos);
	
	_iHealPoint	/=	100;
	_iHealPoint	=	max(_iHealPoint,1);

	if(_iHealPoint)	
		m_aHitEffect[iHitEffect]	=	g_hem.addEffectAndInfo(pos.x,pos.y,lpSkill->m_wHealImage,dHIT_INFO_GREEN_NUMBER,_iHealPoint,0,0,isOwnTeam());	//	타격효과 추가
	else
		m_aHitEffect[iHitEffect]	=	g_hem.addEffect(pos.x,pos.y,lpSkill->m_wHealImage,0,0,isOwnTeam());	//	미스~
}

//
//	힐 효과
void
CActor::makeAbsorbDamageEffect(int _iHealPoint)
{
	int		iHitEffect	=	getFreeHitIndex();
	CPos	pos;

	getHitPointCorrectPos(eHEOP_HIT_ZONE,&pos);

	_iHealPoint		/=	100;
	_iHealPoint		=	max(_iHealPoint,1);

	if(_iHealPoint)
		m_aHitEffect[iHitEffect]	=	g_hem.addEffectAndInfo(pos.x,pos.y,g_im.m_wDamageAbsorbEffect,dHIT_INFO_GREEN_NUMBER,_iHealPoint,0,0,isOwnTeam());	//	타격효과 추가
	else
		m_aHitEffect[iHitEffect]	=	g_hem.addEffect(pos.x,pos.y,g_im.m_wDamageAbsorbEffect,0,0,isOwnTeam());	//	미스~
}

void
CActor::putHill(int _iX,int _iY)
{
	if	(m_wHillSkill	==	0xffff)	
		return;

	CActiveSkill	*lpSkill	=	g_sm.get(m_wHillSkill);

	if	(lpSkill	==	NULL)
		return;

	lpSkill->putIcyStalagmite(_iX,_iY);
}