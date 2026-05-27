#include	"movie.h"
#include	"cDECODER.h"
#include	"cDRAW.h"
#include	"cMap.h"
#include	"cActor.h"
#include	"cSound.h"

cMovie*		cMF_Common::s_lpMovie	=	NULL;

BOOL
cMF_LoadAnm::operate()
{
	cMovieAnmPlayer	*lpPlayer	=	&s_lpMovie->m_anmPlayer;

	BOOL	bIsSuccess	=	lpPlayer->loadAnm(m_wIndex,m_strFileName);

	CGamePlay::s_timer.resetFrameSkip();

	return	bIsSuccess;
}

BOOL
cMF_PlayAnm::operate()
{
	cMovieAnmPlayer	*lpPlayer	=	&s_lpMovie->m_anmPlayer;
	
	lpPlayer->addPlayAnm(m_iIndex,m_iAnm,m_iXPos,m_iYPos,m_iLayer,m_iScale,m_bIsInvertPutShadowOrder);
	
	return	TRUE;
}

BOOL
cMF_DelayAnm::operate()
{
	cMovieAnmPlayer	*lpPlayer	=	&s_lpMovie->m_anmPlayer;

	int	iDelayFrame	=	m_iDelayTime*dSYNC_FPS/100;
	
	lpPlayer->delayAnm(m_iIndex,iDelayFrame);
	
	return	TRUE;
}

BOOL
cMF_ChangeAnmPos::operate()
{
	cMovieAnmPlayer	*lpPlayer	=	&s_lpMovie->m_anmPlayer;
	
	lpPlayer->changePos(m_iIndex,m_iXPos,m_iYPos);
	
	return	TRUE;
}

BOOL
cMF_SetOperateScreenFilter::operate()
{
	cMovieAnmPlayer	*lpPlayer	=	&s_lpMovie->m_anmPlayer;
	cScreenFilter	*lpFilter	=	&lpPlayer->m_screenFilter;

	lpFilter->setWorkFilter(m_wBeginColor,m_wBeginDepth,m_wEndColor,m_wEndDepth,m_wOperateTime,m_wLayer);

	return	TRUE;
}

BOOL
cMF_SetFocus::operate()
{
	int	iX	=	m_wX*dTILE_XS+dTILE_XS/2;
	int	iY	=	m_wY*dTILE_YS+dTILE_YS/2;

	switch(m_wWork)
	{
		case	eSet	:
			g_am.setGroundFocus(iX,iY,0);
			break;

		case	eScroll	:
			g_am.setGroundFocus(iX,iY,m_wOperateTime);
			break;

		case	eCancel	:
			g_am.setGroundFocus(0,0,0);
			break;
	}

	return	TRUE;
}

BOOL
cMF_LoadImage::operate()
{
	cMovieAnmPlayer	*lpPlayer	=	&s_lpMovie->m_anmPlayer;

	BOOL	bIsSuccess	=	lpPlayer->loadSmi(m_strFileName);

	CGamePlay::s_timer.resetFrameSkip();

	return	bIsSuccess;
}

BOOL
cMF_PlayWav::operate()
{
	g_esm.play(m_strFileName);

	return	TRUE;
}

BOOL
cMF_PutImage::operate()
{
	cMovieAnmPlayer	*lpPlayer	=	&s_lpMovie->m_anmPlayer;

	lpPlayer->putSmi(m_wIndex,m_wX,m_wY,m_wLayer);

	return	TRUE;
}

BOOL
cMF_CancelPutImage::operate()
{
	cMovieAnmPlayer	*lpPlayer	=	&s_lpMovie->m_anmPlayer;

	lpPlayer->cancelPutSmi();

	return	TRUE;
}

BOOL
cMF_DrawMap::operate()
{
	if	(m_iOnOff	==	0)
		g_map.cancelSkippingDraw();
	else
		g_map.skippingDraw();

	return	TRUE;
}

BOOL
cMF_End::operate()
{
	s_lpMovie->m_bIsEnd	=	TRUE;

	return	TRUE;
}
