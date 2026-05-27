#ifndef _classFirecracker_h
#define	_classFirecracker_h

#include "cGamePlay.H"

enum
{
	eFCS_READY,	//	발사~	
	eFCS_SHOOT,	//	발사~	
	eFCS_DOWN,	//	내려 오기
	eFCS_FIRE,	//	터졌다.
};

const	int		c_iMaxFireCrackerCount			=	1024;
const	int		c_iFireCrackerTypeCount			=	10;
const	int		c_iFireCrackerAnmCount			=	3;
const	int		c_iFireCrackerMaxVariationCount	=	10;

class	cFireCracker
{
public:
	WORD				m_wSerial;
	int					m_iX,m_iY,m_iDestX,m_iDestY;
	int					m_iStep;
	int					m_iType;
	int					m_iAnm,m_iFrame,m_iFrameCounter,m_iMaxFrame,m_iFPS;
	int					m_iImage;
	int					m_iCurrentImage;
	int					m_iDelayTime;
	int					m_iTriggerCounter;

	void				reset();
	void				copy(cFireCracker *_lpData);
	BOOL				update();
	BOOL				changeStep();
	void				put(int _iX,int _iY);
};

class	cFireCrackerManager : public CGamePlay
{
public:
	cDen<cFireCracker>	m_den;

	int					m_iRemainFireWorkTime;
	int					m_iUpkeepFireWorkTime;
	int					m_iRemainFireWorkCount;
	int					m_iFireWorkKind;
	int					m_iServerSerial;
	char				m_astrSound[c_iFireCrackerTypeCount+1][c_iFireCrackerAnmCount][c_iFireCrackerMaxVariationCount][32];

	BOOL				loadSoundData();
	BOOL				initForDev();
	BOOL				init();
	void				reset();
	BOOL				add(int _iX,int _iY,int _iDestX,int _iDestY,int _iType);
	void				useFireWork(int _iServerSerial,int _iKind,int _iTime);
	void				fire(BOOL _bIsFire);
	void				endFireWorkTime();
	BOOL				isActive();

	void				update(BOOL _bIsFire,int _iX,int _iY,int _iDestX,int _iDestY,BOOL _bIsChangeSecond);

	void				draw(int _iX,int _iY);
};

extern	cFireCrackerManager	g_fireCrackerManager;

#endif