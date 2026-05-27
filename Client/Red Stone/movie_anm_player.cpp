#include	"cIMAGE.H"
#include	"movie_anm_player.H"
#include	"cGAME.H"

int		cMovieAnmPlayerCommon::s_iGameFPS;

void
cMovieAnmPlayInfo::copy(cMovieAnmPlayInfo *_lpData)
{
	memcpy(this,_lpData,sizeof(cMovieAnmPlayInfo));
}

void
cMovieAnmPlayInfo::reset()
{
	m_wSerial		=	0xffff;
}

BOOL
cMovieAnmPlayInfo::init(cANM *_lpAnm,int _iAnm,int _iAnmIndex,int _iX,int _iY,int _iLayer,int _iScale,BOOL _bIsInvertPutShadowOrder)
{
	m_wFrameCount	=	_lpAnm->GetFrameCount(_iAnm);
	m_wFPS			=	_lpAnm->GetFPS(_iAnm);

	m_wAnm			=	_iAnm;
	m_wDirect		=	0;
	m_wFrame		=	0;
	m_wFrameCounter	=	0;
	m_wAnmIndex		=	_iAnmIndex;
	m_wLayer		=	_iLayer;
	m_wX			=	_iX;
	m_wY			=	_iY;
	m_wScale		=	_iScale;
	m_bIsInvertPutShadowOrder	=	_bIsInvertPutShadowOrder;
	m_wDelayFrame	=	0;

	return	TRUE;
}

BOOL
cMovieAnmPlayInfo::operate()
{
	if	(m_wDelayFrame)
	{
		m_wDelayFrame--;
		return	TRUE;
	}

	if	(m_wFrameCounter>=	s_iGameFPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	s_iGameFPS;

		m_wFrame++;

		if	(m_wFrame	>=	m_wFrameCount)
			return	FALSE;
	}

	m_wFrameCounter	+=	m_wFPS;

	return	TRUE;
}

void
cMovieAnmPlayInfo::put(cANM *_lpAnm,int _iDX,int _iDY)
{
	if	(m_bIsInvertPutShadowOrder	==	FALSE)
		_lpAnm->PutShadow(m_wX-_iDX,m_wY-_iDY,m_wAnm,m_wDirect,m_wFrame,m_wScale,m_wScale);

	_lpAnm->PutReg(m_wX-_iDX,m_wY-_iDY,m_wAnm,m_wDirect,m_wFrame,m_wScale,m_wScale);

	if	(m_bIsInvertPutShadowOrder	==	TRUE)
		_lpAnm->PutShadow(m_wX-_iDX,m_wY-_iDY,m_wAnm,m_wDirect,m_wFrame,m_wScale,m_wScale);
}

cMovieAnmPlayer::cMovieAnmPlayer()
{
	for	(int i=0;i<eMaxAnmFilecount;i++)
		m_pAnm[i]	=	NULL;

	m_pSmiImage	=	NULL;
	m_iPutImage	=	-1;

	m_denPlayList.init(eMaxAnmFilecount,TRUE);
	m_denPlayList.reset();
}

cMovieAnmPlayer::~cMovieAnmPlayer()
{
	__destroy();
}

void
cMovieAnmPlayer::__destroy()
{
	m_iPutImage	=	-1;

	for	(int i=0;i<eMaxAnmFilecount;i++)
		KILL(m_pAnm[i]);

	KILL(m_pSmiImage);
}

BOOL
cMovieAnmPlayer::loadAnm(int _iIndex,char *_lpstrFile)
{
	KILL(m_pAnm[_iIndex]);

	m_pAnm[_iIndex]	=	new cANM ();

	return	m_pAnm[_iIndex]->Load(_lpstrFile,TRUE);
}

void
cMovieAnmPlayer::addPlayAnm(int _iIndex,int _iAnm,int _iX,int _iY,int _iLayer,int _iScale,BOOL _bIsInvertPutShadowOrder)
{
	cMovieAnmPlayInfo	player;
	
	player.init(m_pAnm[_iIndex],_iAnm,_iIndex,_iX,_iY,_iLayer,_iScale,_bIsInvertPutShadowOrder);
	
	m_denPlayList.add(&player);
}

void
cMovieAnmPlayer::changePos(int _iIndex,int _iX,int _iY)
{
	cMovieAnmPlayInfo	*lpPlayer	=	getPlayerByAnmIndex(_iIndex);
	
	if	(lpPlayer)
	{
		lpPlayer->m_wX	=	_iX;
		lpPlayer->m_wY	=	_iY;
	}
}

void
cMovieAnmPlayer::delayAnm(int _iIndex,int _iFrame)
{
	cMovieAnmPlayInfo	*lpPlayer	=	getPlayerByAnmIndex(_iIndex);
	
	if	(lpPlayer)
		lpPlayer->m_wDelayFrame	=	_iFrame;
}

BOOL
cMovieAnmPlayer::loadSmi(char *_lpstrFile)
{
	KILL(m_pSmiImage);

	m_pSmiImage		=	new cSMI ();

	return	m_pSmiImage->load(_lpstrFile);
}

void
cMovieAnmPlayer::putSmi(int _iIndex,int _iX,int _iY,int _iLayer)
{
	m_posImage.x	=	_iX;
	m_posImage.y	=	_iY;
	m_iPutImage		=	_iIndex;
	m_iPutImageLayer=	_iLayer;
}

void
cMovieAnmPlayer::cancelPutSmi()
{
	m_iPutImage		=	-1;
}

void
cMovieAnmPlayer::operate()
{
	int	aiRemoveList[eMaxAnmFilecount];
	int	iRemoveCount	=	0;

	m_screenFilter.operate();

	for	(int iPlayIndex=0;iPlayIndex<m_denPlayList.getCount();iPlayIndex++)
	{
		cMovieAnmPlayInfo	*lpPlayInfo	=	m_denPlayList.getByOrdering(iPlayIndex);

		if	(!lpPlayInfo->operate())
			aiRemoveList[iRemoveCount++]	=	lpPlayInfo->m_wSerial;
	}

	for	(int i=0;i<iRemoveCount;i++)
		m_denPlayList.remove(aiRemoveList[i]);
}

void
cMovieAnmPlayer::draw(int _iDX,int _iDY)
{
	for	(int iLayer=0;iLayer<=eLayerCount;iLayer++)
	{
		if	(m_iPutImage	!=	-1	&&	m_iPutImageLayer	==	iLayer)
			m_pSmiImage->put(m_iPutImage,m_posImage.x,m_posImage.y);

		if	(m_screenFilter.m_wCurrentDepth	&&	m_screenFilter.m_wLayer	==	iLayer)
			cDRAW::FillAB(m_screenFilter.m_wCurrentColor,m_screenFilter.m_wCurrentDepth);

		for	(int iPlayIndex=0;iPlayIndex<m_denPlayList.getCount();iPlayIndex++)
		{
			cMovieAnmPlayInfo	*lpPlayInfo	=	m_denPlayList.getByOrdering(iPlayIndex);

			if	(lpPlayInfo->m_wLayer	!=	iLayer)
				continue;

			lpPlayInfo->put(m_pAnm[lpPlayInfo->m_wAnmIndex],_iDX,_iDY);
		}
	}
}

cMovieAnmPlayInfo*
cMovieAnmPlayer::getPlayerByAnmIndex(int _iAnmIndex)
{
	for	(int iPlayIndex=0;iPlayIndex<m_denPlayList.getCount();iPlayIndex++)
	{
		cMovieAnmPlayInfo	*lpPlayInfo	=	m_denPlayList.getByOrdering(iPlayIndex);
		
		if	(lpPlayInfo->m_wAnmIndex	==	_iAnmIndex)
			return	lpPlayInfo;
	}

	return	NULL;
}

void
cMovieAnmPlayer::setGameFPS(int _iGameFPS)
{
	s_iGameFPS	=	_iGameFPS;

	m_screenFilter.init(_iGameFPS);
}