#ifndef	_movie_anm_player_h
#define	_movie_anm_player_h

#include	"SFC.h"
#include	"screen_filter.h"

class	cANM;
class	cSMI;

class	cMovieAnmPlayerCommon
{
public:
	static	int	s_iGameFPS;
};

class	cMovieAnmPlayInfo	:	public cMovieAnmPlayerCommon
{
public:
	WORD	m_wSerial;
	WORD	m_wFrameCounter;
	WORD	m_wDelayFrame;
	WORD	m_wAnm,m_wDirect,m_wFrame,m_wFrameCount;
	WORD	m_wFPS;
	WORD	m_wAnmIndex;
	WORD	m_wX,m_wY;
	WORD	m_wScale;
	WORD	m_wLayer;
	BOOL	m_bIsInvertPutShadowOrder;

	void	copy(cMovieAnmPlayInfo *_lpData);
	void	reset();

	BOOL	operate();
	BOOL	init(cANM *_lpAnm,int _iAnm,int _iAnmIndex,int _iX,int _iY,int _iLayer,int _iScale,BOOL _bIsInvertPutShadowOrder);
	void	put(cANM *_lpAnm,int _iDX,int _iDY);
};

class	cMovieAnmPlayer	:	public cMovieAnmPlayerCommon
{
public:
	enum
	{
		eMaxAnmFilecount	=	200,
		eLayerCount			=	10,
	};

	cDen<cMovieAnmPlayInfo>	m_denPlayList;

	CPos			m_posImage;
	int				m_iPutImage;
	int				m_iPutImageLayer;
	cScreenFilter	m_screenFilter;

	cSMI			*m_pSmiImage;
	cANM			*m_pAnm[eMaxAnmFilecount];

	cMovieAnmPlayer();
	~cMovieAnmPlayer();

	void			__destroy();

	void			setGameFPS(int _iGameFPS);
	BOOL			loadAnm(int _iIndex,char *_lpstrFile);
	void			addPlayAnm(int _iIndex,int _iAnm,int _iX,int _iY,int _iLayer,int _iScale,BOOL _bIsInvertPutShadowOrder);
	void			changePos(int _iIndex,int _iX,int _iY);
	void			delayAnm(int _iIndex,int _iFrame);

	BOOL			loadSmi(char *_lpstrFile);
	void			putSmi(int _iIndex,int _iX,int _iY,int _iLayer);
	void			cancelPutSmi();
	cMovieAnmPlayInfo*	getPlayerByAnmIndex(int _iAnmIndex);

	void			operate();
	void			draw(int _iDX,int _iDY);
};

#endif