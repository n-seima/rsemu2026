#include "CGuardianPost.h"
#include "CImageData.h"
#include "CMap.h"

CGuardianPostManager	g_gpManager;

void
CGuardianPost::init(int _iSerial,int _iRegSerial,int _iImage,int _iX,int _iY)
{
	reset();

	cANM	*lpImage=	g_im.getEffect(g_im.m_wGuardianPostImage);

	m_wRegSerial	=	_iRegSerial;
	m_wSerial		=	_iSerial;
	m_wImage		=	_iImage;

	m_pos.x			=	_iX;
	m_pos.y			=	_iY;
	
	m_wAnm			=	_iImage;
	m_wDirect		=	0;
	m_wFrame		=	0;
	m_wFrameCounter	=	0;
	m_wMaxFrame		=	lpImage->GetFrameCount(_iImage);
	m_wFPS			=	lpImage->GetFPS(_iImage);
}

void
CGuardianPost::reset()
{
	m_wSerial		=	0xffff;
	m_wRegSerial	=	0xffff;
	m_wAnm			=	0;
	m_wDirect		=	0;
	m_wFrame		=	0;
	m_wFrameCounter	=	0;
	m_wMaxFrame		=	0;
	m_wFPS			=	0;
}

void
CGuardianPost::update()
{
	if (m_wFrameCounter	>=	dSYNC_FPS)		//	프레임 갱신
	{
		m_wFrameCounter	-=	dSYNC_FPS;

		m_wFrame++;

		if (m_wFrame	>=	m_wMaxFrame)		//	한 에니메이션이 끝났다.
			m_wFrame	=	0;
	}

	m_wFrameCounter	+=	m_wFPS;
}

void
CGuardianPost::put()
{
	cANM	*lpImage=	g_im.getEffect(g_im.m_wGuardianPostImage);
	int		iX		=	GetScaledXPos(m_pos.x);
	int		iY		=	GetScaledYPos(m_pos.y);

	lpImage->PutShadow(iX,iY,m_wImage,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);
	lpImage->PutReg(iX,iY,m_wImage,m_wDirect,m_wFrame,g_iGameScale,g_iGameScale);
}

void
CGuardianPostManager::reset()
{
	m_wCount	=	0;

	for (int i=0;i<dMAX_GUARDIAN_POST_COUNT;i++)
		m_aPost[i].reset();
}

int
CGuardianPostManager::getValidSlotIndex()
{
	for (int i=0;i<dMAX_GUARDIAN_POST_COUNT;i++)
		if (m_aPost[i].m_wSerial == 0xffff)
			return	i;

	return	0xffff;
}

int
CGuardianPostManager::getPostIndexByRegSerial(int _iRegSerial)
{
	for (int i=0;i<dMAX_GUARDIAN_POST_COUNT;i++)
		if (m_aPost[i].m_wRegSerial == _iRegSerial)
			return	i;

	return	0xffff;
}

int
CGuardianPostManager::getViewPost(cINDEX *_lpIndexes,cRECT *_lpRectViewport)
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_GUARDIAN_POST_COUNT;i++)
	{
		if (m_aPost[i].m_wSerial == 0xffff)
			continue;

		if (_lpRectViewport->isIn(m_aPost[i].m_pos.x,m_aPost[i].m_pos.y))
		{
			_lpIndexes[iCount].Index	=	dBORDER_GUARDIAN_POST+i;
			_lpIndexes[iCount].Value	=	m_aPost[i].m_pos.y;
			iCount++;
		}
	}

	return	iCount;
}

BOOL
CGuardianPostManager::addPost(int _iX,int _iY,int _iImage,int _iRegSerial)
{
	int	iExistIndex	=	getPostIndexByRegSerial(_iRegSerial);
	int	iIndex		=	getValidSlotIndex();

	if (iExistIndex	!=	0xffff)
		iIndex		=	iExistIndex;

	if (iIndex		==	0xffff)
		return	FALSE;

	CGuardianPost	*lpPost	=	&m_aPost[iIndex];

	lpPost->init(iIndex,_iRegSerial,_iImage,_iX,_iY);

	if (iExistIndex	==	0xffff)
		m_wCount++;

	return	TRUE;
}

BOOL
CGuardianPostManager::removePost(int _iRegSerial)
{
	for (int i=0;i<dMAX_GUARDIAN_POST_COUNT;i++)
		if (m_aPost[i].m_wRegSerial == _iRegSerial)
		{
			m_aPost[i].reset();
			m_wCount--;

			return	TRUE;
		}

	return	FALSE;
}

void
CGuardianPostManager::update()
{
	for (int i=0;i<dMAX_GUARDIAN_POST_COUNT;i++)
	{
		if (m_aPost[i].m_wSerial	==	0xffff)
			continue;

		m_aPost[i].update();
	}
}