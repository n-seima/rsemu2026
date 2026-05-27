#include "firecracker.h"
#include "cImageData.h"
#include "cGame.h"
#include "cSound.h"

cFireCrackerManager	g_fireCrackerManager;

void
cFireCracker::reset()
{
	m_wSerial			=	0xffff;
	m_iTriggerCounter	=	0;
}

void
cFireCracker::copy(cFireCracker *_lpData)
{
	memcpy(this,_lpData,sizeof(cFireCracker));
}

BOOL
cFireCracker::changeStep()
{
	switch(m_iStep)
	{
		case	eFCS_READY	:
			m_iCurrentImage	=	g_im.m_wSetOffFirecrackerImage;
			m_iFrame		=	0;
			m_iAnm			=	random(5);
			m_iMaxFrame		=	g_im.getEffect(m_iCurrentImage)->GetFrameCount(m_iAnm);
			m_iFPS			=	g_im.getEffect(m_iCurrentImage)->GetFPS(m_iAnm);
			m_iFrameCounter	=	0;
			m_iTriggerCounter=	0;
			m_iStep			=	eFCS_SHOOT;
			break;

		case	eFCS_SHOOT	:
			m_iStep			=	eFCS_DOWN;
			m_iDelayTime	=	dSYNC_FPS;
			break;

		case	eFCS_DOWN	:
			m_iTriggerCounter=	0;
			m_iStep			=	eFCS_FIRE;
			m_iCurrentImage	=	m_iImage;
			m_iFrame		=	0;
			m_iAnm			=	random(3);
			m_iMaxFrame		=	g_im.getEffect(m_iCurrentImage)->GetFrameCount(m_iAnm);
			m_iFPS			=	g_im.getEffect(m_iCurrentImage)->GetFPS(m_iAnm);
			m_iX			=	m_iDestX;
			m_iY			=	m_iDestY;
			m_iFrameCounter	=	0;
			break;

		case	eFCS_FIRE	:
			return	TRUE;
	}

	return	FALSE;
}

BOOL
cFireCracker::update()
{
	if	(m_iStep	==	eFCS_DOWN)
	{
		m_iDelayTime--;

		if	(m_iDelayTime	==	0)
			changeStep();
	}

	if	(m_iFrame	!=	0xffff)					//	이펙트가 있다.
	{
		if	(m_iFrameCounter>=	dSYNC_FPS)		//	프레임 갱신
		{
			m_iFrameCounter	-=	dSYNC_FPS;
			m_iFrame++;

			int	iSound		=	m_iType;

			if	(m_iStep	==	eFCS_SHOOT)
				iSound		=	c_iFireCrackerTypeCount;

			if	(m_iStep	==	eFCS_FIRE	||	m_iStep	==	eFCS_SHOOT	)
			{
				cANM	*lpFC_Image	=	g_im.getEffect(m_iCurrentImage);

				if	(lpFC_Image && lpFC_Image->IsTrigger(m_iAnm,m_iFrame))
				{
					g_esm.play(g_fireCrackerManager.m_astrSound[iSound][m_iAnm][m_iTriggerCounter]);

					m_iTriggerCounter++;
				}
			}
		}

		m_iFrameCounter	+=	m_iFPS;

		if	(m_iFrame	>=	m_iMaxFrame)
			return	changeStep();
	}

	return	FALSE;
}

void
cFireCracker::put(int _iX,int _iY)
{
	if	(m_iStep	==	eFCS_DOWN)
		return;

	cANM	*lpFC_Image	=	g_im.getEffect(m_iCurrentImage);

	if	(!lpFC_Image)
		return;

	lpFC_Image->PutReg(m_iX-_iX,m_iY-_iY,m_iAnm,0,m_iFrame);
}

BOOL
cFireCrackerManager::initForDev()
{
#ifdef	_IS_DEV_CLIENT
	CDecoder	decoder;

	if	(!decoder.Upload("dev data/scripter/fireCrackerSound.txt",NULL))
		return	FALSE;

	while(1)
	{
		int	iType	=	decoder.getNumber();

		if	(iType	==	-1)
			break;

		int	iAnm	=	decoder.getNumber();
		int	iPattern=	0;

		while(1)
		{
			char	*lpstrWord	=	decoder.getStream();

			if	(STRICMP(lpstrWord,"end")	==	0)
				break;

			strcpy(m_astrSound[iType][iAnm][iPattern],lpstrWord);

			iPattern++;
		}
	}

	cFILE	file;

	file.Open("data/fireCrackerSound.dat","wb");
	file.Write(m_astrSound,sizeof(m_astrSound));
	file.Close();
#endif
	return	TRUE;
}

BOOL
cFireCrackerManager::loadSoundData()
{
	cFILE	file;

	if	(!file.Open("data/fireCrackerSound.dat","rb"))
		return	FALSE;

	file.Read(m_astrSound,sizeof(m_astrSound));
	file.Close();

	return	TRUE;
}

BOOL
cFireCrackerManager::init()
{
	m_den.init(c_iMaxFireCrackerCount);

	return	loadSoundData();
}

void
cFireCrackerManager::reset()
{
	m_den.reset();

	m_iRemainFireWorkTime	=	0;
	m_iUpkeepFireWorkTime	=	0;
	m_iRemainFireWorkCount	=	0;
	m_iFireWorkKind			=	0;
}

BOOL
cFireCrackerManager::add(int _iX,int _iY,int _iDestX,int _iDestY,int _iType)
{
	if	(_iType)
		_iType--;

	cFireCracker	fc;

	fc.m_iX		=	_iX;
	fc.m_iY		=	_iY;
	fc.m_iDestX	=	_iDestX;
	fc.m_iDestY	=	_iDestY;
	fc.m_iStep	=	eFCS_READY;
	fc.m_iType	=	_iType;
	fc.m_iImage	=	g_im.m_awFirecrackerImage[_iType];
	fc.changeStep();

	if	(m_den.add(&fc)	==	0xffff)
		return	FALSE;

	return	TRUE;
}


void
cFireCrackerManager::update(BOOL _bIsFire,int _iX,int _iY,int _iDestX,int _iDestY,BOOL _bIsChangeSecond)
{
	if	(_bIsFire	&&	isActive())
	{
		if	(m_iRemainFireWorkTime	==	0	&&	m_iUpkeepFireWorkTime)
		{
			m_iRemainFireWorkTime	=	m_iUpkeepFireWorkTime;
			m_iUpkeepFireWorkTime	=	0;
		}

		s_agent.sendEtcWork(eCEW_FIRE_FIRE_WORK,m_iServerSerial,_iX,_iY,_iDestX,_iDestY);
		m_iRemainFireWorkCount--;
	}

	if	(_bIsChangeSecond	&&	m_iRemainFireWorkTime)
	{
		m_iRemainFireWorkTime--;

		if	(m_iRemainFireWorkTime	==	0)
			m_iRemainFireWorkCount	=	0;
	}

	int	i;

	for	(i=m_den.getMaxCount();i;)
	{
		i--;
		cFireCracker	*lpCracker	=	m_den.get(i);

		if	(!lpCracker)
			continue;

		if	(lpCracker->update())
			m_den.remove(i);
	}
}

void
cFireCrackerManager::draw(int _iX,int _iY)
{
	int	i;

	for	(i=m_den.getMaxCount();i;)
	{
		i--;
		cFireCracker	*lpCracker	=	m_den.get(i);

		if	(!lpCracker)
			continue;

		lpCracker->put(_iX,_iY);
	}
}

void
cFireCrackerManager::useFireWork(int _iServerSerial,int _iKind,int _iTime)
{
	m_iServerSerial			=	_iServerSerial;
	m_iRemainFireWorkTime	=	0;
	m_iUpkeepFireWorkTime	=	_iTime;
	m_iRemainFireWorkCount	=	_iTime;
	m_iFireWorkKind			=	_iKind;
}

void
cFireCrackerManager::endFireWorkTime()
{
	m_iRemainFireWorkTime	=	0;
	m_iUpkeepFireWorkTime	=	0;
	m_iRemainFireWorkCount	=	0;
	m_iFireWorkKind			=	0;
}

BOOL
cFireCrackerManager::isActive()
{
	if	(m_iRemainFireWorkCount)
		return	TRUE;

	return	FALSE;
}
