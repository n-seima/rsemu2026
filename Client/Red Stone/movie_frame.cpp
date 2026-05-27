#include	"movie.h"
#include	"cDECODER.h"
#include	"cDRAW.h"

cMovieFrame::cMovieFrame()
{
	m_wWork		=	eMovie_none;
	m_pBuffer	=	NULL;
}

cMovieFrame::~cMovieFrame()
{
	__destroy();
}

void
cMovieFrame::alloc(int _iWork)
{
	__destroy();
	
	m_wWork			=	_iWork;
	m_wBufferSize	=	0;

	switch(_iWork)
	{
		case	eMovie_none:
			break;
		case	eMovie_load_anm:
			m_wBufferSize	=	sizeof(cMF_LoadAnm);
			break;
		case	eMovie_play_anm:
			m_wBufferSize	=	sizeof(cMF_PlayAnm);
			break;
		case	eMovie_set_operate_screen_filter:
			m_wBufferSize	=	sizeof(cMF_SetOperateScreenFilter);
			break;
		case	eMovie_set_focus	:
			m_wBufferSize	=	sizeof(cMF_SetFocus);
			break;
		case	eMovie_draw_map	:
			m_wBufferSize	=	sizeof(cMF_DrawMap);
			break;

		case	eMovie_load_image		:
			m_wBufferSize	=	sizeof(cMF_LoadImage);
			break;

		case	eMovie_play_wav			:
			m_wBufferSize	=	sizeof(cMF_PlayWav);
			break;

		case	eMovie_put_image		:
			m_wBufferSize	=	sizeof(cMF_PutImage);
			break;

		case	eMovie_cancel_put_image	:
			break;

		case	eMovie_change_anm_pos	:
			m_wBufferSize	=	sizeof(cMF_ChangeAnmPos);
			break;
		case	eMovie_delay_anm		:
			m_wBufferSize	=	sizeof(cMF_DelayAnm);
			break;

		case	eMovie_end:
			break;
	}

	if	(m_wBufferSize)
		m_pBuffer	=	new char [m_wBufferSize];
}

void
cMovieFrame::__destroy()
{
	m_wWork		=	eMovie_none;
	pKILL(m_pBuffer);
}

void
cMovieFrame::copy(const cMovieFrame *_lpFrameInfo)
{
	__destroy();

	alloc(_lpFrameInfo->m_wWork);

	if	(m_wBufferSize)
		memcpy(m_pBuffer,_lpFrameInfo->m_pBuffer,m_wBufferSize);
}

void
cMovieFrame::setPlayWav(char *_lpstrFileName)
{
	alloc(eMovie_play_wav);

	strcpy(getPlayWav()->m_strFileName,_lpstrFileName);
}

void
cMovieFrame::setLoadImage(char *_lpstrFileName)
{
	alloc(eMovie_load_image);

	strcpy(getLoadImage()->m_strFileName,_lpstrFileName);
}

void
cMovieFrame::setPutImage(int _iX,int _iY,int _iImage,int _iLayer)
{
	alloc(eMovie_put_image);

	getPutImage()->m_wIndex	=	_iImage;
	getPutImage()->m_wX		=	_iX;
	getPutImage()->m_wY		=	_iY;
	getPutImage()->m_wLayer	=	_iLayer;
}

void
cMovieFrame::setCancelPutImage()
{
	alloc(eMovie_cancel_put_image);
}

void
cMovieFrame::setLoadAnm(int _iIndex,char *_lpstrFileName)
{
	alloc(eMovie_load_anm);

	getLoadAnm()->m_wIndex	=	_iIndex;
	strcpy(getLoadAnm()->m_strFileName,_lpstrFileName);
}

void
cMovieFrame::setPlayAnm(int _iIndex,int _iAnm,int _iX,int _iY,int _iLayer,int _iScale,BOOL _bIsInvertPutShadowOrder)
{
	alloc(eMovie_play_anm);

	getPlayAnm()->m_iIndex	=	_iIndex;
	getPlayAnm()->m_iAnm	=	_iAnm;
	getPlayAnm()->m_iXPos	=	_iX;
	getPlayAnm()->m_iYPos	=	_iY;
	getPlayAnm()->m_iLayer	=	_iLayer;
	getPlayAnm()->m_iScale	=	_iScale;
	getPlayAnm()->m_bIsInvertPutShadowOrder	=	_bIsInvertPutShadowOrder;
}

void
cMovieFrame::setChangeAnmPos(int _iIndex,int _iX,int _iY)
{
	alloc(eMovie_change_anm_pos);
	
	getChangeAnmPos()->m_iIndex=	_iIndex;
	getChangeAnmPos()->m_iXPos	=	_iX;
	getChangeAnmPos()->m_iYPos	=	_iY;
}

void
cMovieFrame::setDelayAnm(int _iIndex,int _iTime)
{
	alloc(eMovie_delay_anm);
	
	getDelayAnm()->m_iIndex		=	_iIndex;
	getDelayAnm()->m_iDelayTime	=	_iTime;
}

void
cMovieFrame::setFadeScreenFilter(WORD _wBeginColor,int _iBeginDepth,WORD _wEndColor,int _iEndDepth,int _iOperateTime,int _iLayer)
{
	alloc(eMovie_set_operate_screen_filter);

	cMF_SetOperateScreenFilter*	lpFilter	=	getSetOperateScreenFilter();

	lpFilter->m_wBeginColor	=	_wBeginColor;
	lpFilter->m_wEndColor	=	_wEndColor;
	lpFilter->m_wBeginDepth	=	_iBeginDepth;
	lpFilter->m_wEndDepth	=	_iEndDepth;
	lpFilter->m_wOperateTime=	_iOperateTime;
	lpFilter->m_wLayer		=	_iLayer;
}

void
cMovieFrame::setSetFocus(int _iWork,int _iX,int _iY,int _iSpeed)
{
	alloc(eMovie_set_focus);

	getSetFocus()->m_wWork		=	_iWork;
	getSetFocus()->m_wX			=	_iX;
	getSetFocus()->m_wY			=	_iY;
	getSetFocus()->m_wOperateTime=	_iSpeed;
}

void
cMovieFrame::setDrawMap(int _iOnOff)
{
	alloc(eMovie_draw_map);

	getDrawMap()->m_iOnOff	=	_iOnOff;
}

void
cMovieFrame::setEnd()
{
	alloc(eMovie_end);
}

void
cMovieFrame::operate()
{
	switch(m_wWork)
	{
		case	eMovie_none			:
			break;
		case	eMovie_load_anm		:
			getLoadAnm()->operate();
			break;
		case	eMovie_play_anm		:
			getPlayAnm()->operate();
			break;
		case	eMovie_set_operate_screen_filter:
			getSetOperateScreenFilter()->operate();
			break;
		case	eMovie_set_focus	:
			getSetFocus()->operate();
			break;
		case	eMovie_draw_map	:
			getDrawMap()->operate();
			break;

		case	eMovie_load_image		:
			getLoadImage()->operate();
			break;

		case	eMovie_play_wav			:
			getPlayWav()->operate();
			break;

		case	eMovie_put_image		:
			getPutImage()->operate();
			break;

		case	eMovie_cancel_put_image	:
			getCancelPutImage()->operate();
			break;

		case	eMovie_change_anm_pos	:
			getChangeAnmPos()->operate();
			break;

		case	eMovie_delay_anm		:
			getDelayAnm()->operate();
			break;


		case	eMovie_end			:
			s_lpMovie->m_bIsEnd	=	TRUE;
			break;
	}
}
