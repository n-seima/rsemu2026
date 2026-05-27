#ifndef _classEffectManager_H
#define _classEffectManager_H

#include "CNemo.H"

#define		dEFFECT_COUNT		256
#define		dBORDER_EFFECT		50000

class	CEffectInfo
{
public:
	WORD			m_wSerial;
	CPos			m_pos;
	WORD			m_wIsScreenPos;
	int				m_iFrameCounter;
	WORD			m_wBeginFrameConter;
	WORD			m_wDirect,m_wFrameCount,m_wAnm,m_wFrame,m_wBody,m_wIsRandomFrame,m_wIsLoopEffect,m_wImageScale;
	WORD			m_wDiscernCode;

					CEffectInfo()
					{
						m_wSerial		=	0xffff;
						m_wDiscernCode	=	0xffff;
						m_wIsScreenPos	=	FALSE;
					}

	BOOL			update();
	void			put();
	void			setAnm(int _iAnm);
};

class	CEffectManager
{
public:
	CEffectInfo		m_aEffect[dEFFECT_COUNT];

	int				m_iCount,m_iNewSerial;

					CEffectManager();

	void			close();

	void			update();
	void			put(int _iIndex);

	void			changeAnmByDiscernCode(int _iDiscernCode,int _iAnm);
	void			removeEffectByDiscernCode(int _iDiscernCode);
	CEffectInfo*	add(int _iX,int _iY,int _iImage,int _iRange=0,BOOL _bIsWantRandomFrame=FALSE,int _iDirect=0,int _iAnm=0,BOOL _bIsLoop=FALSE,int _iDiscernCode=0xffff,int _iImageScale=100,BOOL _bIsScreenPos=FALSE);
	void			remove(int _iIndex);
	void			putAll();
	int				getViewEffect(cINDEX *_lpIndexes,cRECT *_lpRectViewport);
};

extern	CEffectManager	g_rapeEffect,g_standEffect,g_floatEffect;

#endif