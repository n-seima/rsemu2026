#include "CActor.h"


void	
CActroAnmInfo::reset()
{
	memset(this,0xff,sizeof(CActroAnmInfo));
}

void
CActroAnmInfo::init(CActor *_lpActor,int _iAnm,int _iAlphaDepth)
{
	m_bf14Body			=	_lpActor->m_wBody;
	m_wAnm				=	_iAnm;
	m_wFPS				=	_lpActor->getFPS(_iAnm);
	m_wMaxFrame			=	_lpActor->getFrameCount(_iAnm);
	m_wFrameCounter		=	0;
	m_wFrame			=	0;
	m_wDirect			=	0;
	m_wAlphaDepth		=	_iAlphaDepth;

	m_bf1IsDamageFrame	=	FALSE;
	m_bf1IsTriggerFrame =	FALSE;

	if	(GetBody(m_bf14Body)->IsTrigger(m_wAnm,m_wFrame))
		m_bf1IsTriggerFrame	=	TRUE;
	if	(GetBody(m_bf14Body)->IsDamage(m_wAnm,m_wFrame))
		m_bf1IsDamageFrame 	=	TRUE;
}

BOOL
CActroAnmInfo::updateFrame()
{
	if	(m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;

		m_wFrame++;

		if	(m_wFrame	>=	m_wMaxFrame)		//	한 에니메이션이 끝났다.
		{
			reset();

			return	TRUE;
		}

		if	(GetBody(m_bf14Body)->IsDamage(m_wAnm,m_wFrame))
			m_bf1IsDamageFrame	=	TRUE;
		else
			m_bf1IsDamageFrame	=	FALSE;

		if	(GetBody(m_bf14Body)->IsTrigger(m_wAnm,m_wFrame))
			m_bf1IsTriggerFrame	=	TRUE;
		else
			m_bf1IsTriggerFrame	=	FALSE;
	}

	m_wFrameCounter	+=	m_wFPS;

	return	FALSE;
}

//
//	릴리즈 포인트 구함.
void
CActor::getReleasePos(CPos *pos,int _iAnm,int _iDirect)
{
	if	(_iAnm	==	-1)
		_iAnm	=	m_iAnm;

	if	(_iDirect==	-1)
		_iDirect=	m_iDirect;

	if	(!getBody()->m_pAnmData[_iAnm].m_isRELEASE)
	{
		pos->x	=	m_pos.x;
		pos->y	=	m_pos.y;

		return;
	}

	pos->x	=	m_pos.x	+	getBody()->m_pAnmData[_iAnm].m_pReleasePos[_iDirect].x*m_wHorizonScale/g_iGameScale;
	pos->y	=	m_pos.y	+	getBody()->m_pAnmData[_iAnm].m_pReleasePos[_iDirect].y*m_wVerticalScale/g_iGameScale;
}
