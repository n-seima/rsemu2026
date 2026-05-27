#ifndef _classTRAP_H
#define _classTRAP_H

#include	"SFC.H"
#include	"trapDefine.H"

#define	dTRAP_RANGE					64
#define	dTRAP_DISARM_RANGE			70
#define	dBORDER_TRAP				60000

#define	dDEFAULT_BROKEN_BOX_CHANCE	75	//	운에 따라서 25%까지 떨어짐
#define	dMAX_TRAP_COUNT				10000

class	cTrap
{
public:
	WORD				m_wSerial;				//	함정 시리얼
	WORD				m_wSkill;
	CPos				m_pos;
	WORD				m_wRange;
	WORD				m_bf3Team		:	3;
	WORD				m_bf8DetectLevel:	8;
	
						cTrap()	
						{
							reset();
						}

	inline	void		reset()
	{
		m_wSerial		=	0xffff;
	}

	void				copy(cTrap *_lpTrap);
	void				draw(int _iDX,int _iDY,int _iFrameCounter,BOOL _bIsFocus=FALSE);
};

class	cTrapManager
{
public:
	cDen<cTrap>				m_den;
	int						m_iFrameCounter;
	int						m_iFocusTrap,m_iTryDisarmTrap;

							cTrapManager();
							~cTrapManager();

	inline	void			init()
	{
		m_den.init(dMAX_TRAP_COUNT,TRUE);
		reset();
	}
	void					reset();	//초기화
	inline	void			close()	{m_den.close();}//초기화
	inline	int				getCount()					{return m_den.getCount();}//카운트
	inline	int				getMaxCount()				{return m_den.getMaxCount();}//카운트
	inline	void			rebuild()					{m_den.rebuild();}
	inline	cTrap*			get(int _iIndex)			{return	m_den.get(_iIndex);}

	int						addTrap(cTrap *_lpTrap);
	BOOL					removeTrap(int _iSerial);
	int						getViewTraps(cINDEX *_lpIndexes,cRECT *_lpRectViewport);
	void					drawFlatTraps(cRECT *_lpRectViewport,int _iDX,int _iDY);
	void					drawTrap(int _iSerial,int _iDX,int _iDY);
	void					update();
	int						getFocusTrap(int _iX,int _iY);
};

extern	cTrapManager	g_trapManager;

#endif