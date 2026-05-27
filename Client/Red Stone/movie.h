#ifndef	_movie_h
#define	_movie_h

#include	"SFC.h"
#include	"movie_anm_player.h"
#include	"screen_filter.h"
#include	"screen_focus.h"

enum
{
	eMovie_none,
	eMovie_load_anm,
	eMovie_play_anm,

	eMovie_load_image,
	eMovie_put_image,
	eMovie_cancel_put_image,

	eMovie_set_operate_screen_filter,

	eMovie_draw_map,
	eMovie_set_focus,

	eMovie_play_wav,
	eMovie_change_anm_pos,
	eMovie_delay_anm,

	eMovie_end	=	10000,
};

class	cMovie;

class	cMF_Common
{
public:
	static	cMovie	*s_lpMovie;
};

class	cMF_DrawMap	:	public cMF_Common
{
public:
	int		m_iOnOff;

	BOOL	operate();
};

class	cMF_LoadAnm	:	public cMF_Common
{
public:
	enum
	{
		eFileNameLength	=	256,
	};

	WORD	m_wIndex;
	char	m_strFileName[eFileNameLength];

	BOOL	operate();
};

class	cMF_PlayAnm	:	public cMF_Common
{
public:
	int		m_iIndex;
	int		m_iAnm,m_iXPos,m_iYPos,m_iLayer;
	int		m_iScale;
	BOOL	m_bIsInvertPutShadowOrder;
	
	BOOL	operate();
};

class	cMF_ChangeAnmPos	:	public cMF_Common
{
public:
	int		m_iIndex;
	int		m_iXPos,m_iYPos;
	
	BOOL	operate();
};

class	cMF_DelayAnm	:	public cMF_Common
{
public:
	int		m_iIndex;
	int		m_iDelayTime;
	
	BOOL	operate();
};

class	cMF_LoadImage	:	public cMF_Common
{
public:
	enum
	{
		eFileNameLength	=	256,
	};

	char	m_strFileName[eFileNameLength];

	BOOL	operate();
};

class	cMF_PlayWav	:	public cMF_Common
{
public:
	enum
	{
		eFileNameLength	=	256,
	};

	char	m_strFileName[eFileNameLength];

	BOOL	operate();
};


class	cMF_PutImage	:	public cMF_Common
{
public:
	WORD	m_wX,m_wY;
	WORD	m_wIndex,m_wLayer;

	BOOL	operate();
};

class	cMF_CancelPutImage	:	public cMF_Common
{
public:
	BOOL	operate();
};

class	cMF_End	:	public cMF_Common
{
public:
	BOOL	operate();
};

class	cMF_SetOperateScreenFilter	:	public cMF_Common
{
public:
	WORD	m_wBeginColor,m_wBeginDepth;
	WORD	m_wEndColor,m_wEndDepth;
	WORD	m_wOperateTime,m_wLayer;

	BOOL	operate();
};

class	cMF_SetFocus	:	public cMF_Common
{
public:
	enum
	{
		eSet,
		eScroll,
		eCancel,
	};
	WORD	m_wWork;
	WORD	m_wX,m_wY;
	WORD	m_wOperateTime;

	BOOL	operate();
};

class	cMovieFrame	:	public cMF_Common
{
public:
	WORD	m_wWork;
	WORD	m_wBufferSize;
	char	*m_pBuffer;

	cMovieFrame();
	~cMovieFrame();

	void			__destroy();
	void			alloc(const int _iWork);
	void			copy(const cMovieFrame *_lpFrameInfo);

	cMF_LoadAnm*	getLoadAnm()
	{
		return	(cMF_LoadAnm*)m_pBuffer;
	}

	cMF_PlayAnm*	getPlayAnm()
	{
		return	(cMF_PlayAnm*)m_pBuffer;
	}
	cMF_DelayAnm*	getDelayAnm()
	{
		return	(cMF_DelayAnm*)m_pBuffer;
	}
	
	cMF_ChangeAnmPos*	getChangeAnmPos()
	{
		return	(cMF_ChangeAnmPos*)m_pBuffer;
	}

	cMF_SetOperateScreenFilter*	getSetOperateScreenFilter()
	{
		return	(cMF_SetOperateScreenFilter*)m_pBuffer;
	}

	cMF_SetFocus*		getSetFocus()
	{
		return	(cMF_SetFocus*)m_pBuffer;
	}

	cMF_LoadImage*		getLoadImage()
	{
		return	(cMF_LoadImage*)m_pBuffer;
	}

	cMF_PlayWav*		getPlayWav()
	{
		return	(cMF_PlayWav*)m_pBuffer;
	}

	cMF_PutImage*		getPutImage()
	{
		return	(cMF_PutImage*)m_pBuffer;
	}
	cMF_CancelPutImage*	getCancelPutImage()
	{
		return	(cMF_CancelPutImage*)m_pBuffer;
	}
	cMF_DrawMap*		getDrawMap()
	{
		return	(cMF_DrawMap*)m_pBuffer;
	}
	cMF_End*			getEnd()
	{
		return	(cMF_End*)m_pBuffer;
	}

	void	setPlayWav(char *_lpstrFileName);
	void	setLoadImage(char *_lpstrFileName);
	void	setPutImage(int _iX,int _iY,int _iImage,int _iLayer);
	void	setCancelPutImage();
	
	void	setLoadAnm(int _iIndex,char *_lpstrFileName);
	void	setPlayAnm(int _iIndex,int _iAnm,int _iX,int _iY,int _iLayer,int _iScale,BOOL _bIsInvertPutShadowOrder);
	void	setChangeAnmPos(int _iIndex,int _iX,int _iY);
	void	setDelayAnm(int _iIndex,int _iTime);

	void	setFadeScreenFilter(WORD _wBeginColor,int _iBeginDepth,WORD _wEndColor,int _iEndDepth,int _iOperateTime,int _iLayer);
	void	setSetFocus(BOOL _bIsScroll,int _iX,int _iY,int _iSpeed);
	void	setDrawMap(int _iOnOff);
	void	setEnd();

	void	operate();
};

class	cMovieEvent	:	public cMF_Common
{
public:
	WORD			m_wFrame;
	WORD			m_wWorkCount;
	cMovieFrame		*m_pWork;

	cMovieEvent();
	~cMovieEvent();

	void			__destroy();

	BOOL			init(int _iWorkCount);
	void			copy(cMovieEvent *_lpSrc);
	void			operate();
};

class	cMovie	:	public cMF_Common
{
public:
	WORD			m_wEventCount;
	int				m_iFPS;
	cMovieEvent		*m_pEvents;

	int				m_iFrame;
	WORD			m_wLastEventIndex;

	BOOL			m_bIsEnd;

	cMovieAnmPlayer	m_anmPlayer;

	cMovie();
	~cMovie();

	void			__destroy();
	void			operate();
	void			draw();
	void			init();

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃디코드/저장/불러오기
//┃movie_io.cpp
	BOOL			load(char *_lpstrFile);
	BOOL			save(char *_lpstrFile);
	BOOL			loadScript(char *_lpstrScriptFile,int _iFPS);
//┃movie_io.cpp
//┃디코드/저장/불러오기
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

#endif