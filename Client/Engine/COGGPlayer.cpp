#include "COGGPlayer.h"

#ifndef ABS
#define ABS(x)		( ((x) > 0) ? (x) : (-(x)) )
#endif

COGGPlayer::COGGPlayer()
{
	m_iFadeSpeed		=	dDEFAULT_FADE_SPEED;

	m_procMasterWindow	=	0;
}

COGGPlayer::~COGGPlayer(void)
{
	close();

	SetWindowLong(m_hWnd, GWL_WNDPROC,(LONG)m_procMasterWindow);

	KillTimer (m_hWnd, m_dwTimerHandler);

	m_lpSoundObject		=	NULL;
	m_dwBufferSize		=	0;
	m_dwGap				=	0;
	m_dwReadOnce		=	0;
	m_dwState			=	STREAM_STATE_NONE;
	m_procMasterWindow	=	0;
	m_lpFile			=	NULL;
}

int
COGGPlayer::init(HWND _hWnd, CSound *_lpSoundObject,DWORD _dwBufferSize,DWORD _dwGabSize,DWORD _dwReadOnceSize,DWORD _dwTimerHandle)
{
	if (!_hWnd					) 
		return STREAM_ERR_UNAVAILABLE_HWND;
	if (!_lpSoundObject			) 
		return STREAM_ERR_UNAVAILABLE_SOUND_OBJECT;
	if (_dwTimerHandle	<=	0	) 
		return STREAM_ERR_UNAVAILABLE_TIMER_INDEX;

	m_dwTimerHandler	= _dwTimerHandle;

	m_hWnd				=	_hWnd;
	m_lpSoundObject		=	_lpSoundObject;

	SetTimer(m_hWnd, m_dwTimerHandler, 100, NULL);		// 1000 / 24 = 42

	m_dwState			=	STREAM_STATE_NONE;

	if (!m_lpSoundObject->m_lpDS)
		return SOUND_ERR_CANNOT_FIND_DS;

	m_dwBufferSize		=	_dwBufferSize;
	m_dwGap				=	_dwGabSize;
	m_dwReadOnce		=	_dwReadOnceSize;

	m_iVolume			=	dMAX_SOUND_VOLUME;
	m_iFadeSpeed		=	dDEFAULT_FADE_SPEED;
	m_bIsFadeOut		=	FALSE;
	m_bIsFadeIn			=	FALSE;
	m_bIsPlaying		=	FALSE;

	m_dwState			=	STREAM_STATE_INITIALIZE;
	m_procMasterWindow	=	(WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC,(LONG)TimerCallback);

	SetWindowLong(m_hWnd,GWL_USERDATA,(LONG)this);

	return STREAM_ERR_OK;
}

BOOL
COGGPlayer::play(char *_lpstrFile,DWORD _dwOption)
{
	FILE	*fp;

	if (_dwOption	!=	0xffffffff)	m_dwOption		=	_dwOption;

	if (isPlaying())
	{
		if (stricmp(_lpstrFile,m_strLoadedFile) == 0) return	FALSE;

		if (m_bIsSmoothChange && m_bIsFadeOut == FALSE)
		{
			fp			=	fopen(_lpstrFile,"rb");

			if(!fp)	return	FALSE;

			fclose(fp);

			m_bIsFadeOut	=	TRUE;
			m_iCurrentVolume=	m_iVolume;

			strcpy(m_strBookedFile,_lpstrFile);

			return	TRUE;
		}

	}

	close();

	fp		=	fopen(_lpstrFile,"rb");

	if	(!fp)
		return	FALSE;

	strcpy(m_strLoadedFile,_lpstrFile);
	memset(m_strBookedFile,0,sizeof(m_strBookedFile));

	if (load(fp)!=	SOUND_ERR_OK)
		return	FALSE;

	m_lpFile	=	fp;

	play(m_dwOption);

	return TRUE;
}

HRESULT	
COGGPlayer::load(FILE *fp, position_set *_lpPos)
{
	int	iErr = 0;

	if (!m_lpSoundObject->m_lpDS)
		return SOUND_ERR_CANNOT_FIND_DS;

	//	set COGGPlayer
	if (m_dwState == STREAM_STATE_INITIALIZE)
	{
		m_dwState = STREAM_STATE_LOAD;

		if ((iErr = m_lpSoundObject->openFromFile (fp, &m_handleSound)) != SOUND_ERR_OK)
			return iErr;

		// create m_pDSBuffer
		m_dwSize			=	m_handleSound.desc.dwBufferBytes / m_handleSound.fmt.nBlockAlign;
		m_dwReadSize	=	0;
		m_dwPlaySize	=	0;
		m_dwOldPlay		=	0;
		m_dwTempPlaySize=	0;

		if (_lpPos)
		{
			setCurrentPosition (_lpPos);
		}
		else 
		{
			m_pos.index			=	0;
			m_pos.start_pos		=	0;
			m_pos.loop_start_pos=	0;
			m_pos.option=	STREAM_PERMIT_NEXT_OFF;
			m_pos.size	=	m_handleSound.desc.dwBufferBytes / m_handleSound.fmt.nBlockAlign;
			m_lpNextPos	=	NULL;
		}

		m_dwCurPlay	=	0;
		m_dwCurWrite=	0;

		// insert m_pDSBuffer m_dwSize
		m_handleSound.desc.dwBufferBytes = m_dwBufferSize * m_handleSound.fmt.nBlockAlign;

		if ((iErr = m_lpSoundObject->createBuffer ((IDirectSoundBuffer **)&m_pDSBuffer, &m_handleSound.desc)) != SOUND_ERR_OK)
			return iErr;

		if ((iErr = m_lpSoundObject->readFromFile (m_pDSBuffer, &m_handleSound, m_dwCurWrite, min(m_dwBufferSize, m_dwSize), 0, &m_dwReadSize)) != SOUND_ERR_OK) 
			return iErr;

		m_dwCurWrite=	(m_dwCurWrite + m_dwReadSize) % m_dwBufferSize;

		m_dwState	=	STREAM_STATE_READY;

		return SOUND_ERR_OK;

	}

	return SOUND_ERR_CANNOT_CREATE_STREAM;
}


HRESULT
COGGPlayer::play(DWORD _dwOption,int _iVolume)
{
	if	(!m_lpSoundObject->m_lpDS			)	
		return SOUND_ERR_CANNOT_FIND_DS;

	if	(m_dwState	!=	STREAM_STATE_READY	)	
		return STREAM_ERR_NOT_READY;

	m_dwOption	=	_dwOption;

	m_pDSBuffer->SetCurrentPosition(0);
	m_pDSBuffer->Play(0,0,SOUND_PLAY_REPEAT);

	if (_iVolume	==	-1	)	_iVolume	=	m_iVolume;

	setVolume(_iVolume);

	m_dwState	=	STREAM_STATE_PLAY;

	return SOUND_ERR_OK;
}

HRESULT
COGGPlayer::stop(void)
{
	if (!m_lpSoundObject->m_lpDS)
		return SOUND_ERR_CANNOT_FIND_DS;

	if (!isPlaying())
		return STREAM_ERR_CANNOT_STOP;

	m_pDSBuffer->Stop();
	m_dwState	=	STREAM_STATE_STOP;

	return SOUND_ERR_OK;
}

HRESULT
COGGPlayer::pause(void)
{
	if (!m_lpSoundObject->m_lpDS) return SOUND_ERR_CANNOT_FIND_DS;

	if (!isPlaying()) return STREAM_ERR_CANNOT_PAUSE;

	m_pDSBuffer->Stop();
	m_dwState	=	STREAM_STATE_PAUSE;

	return SOUND_ERR_OK;
}

HRESULT
COGGPlayer::resume(void)
{
	if (!m_lpSoundObject->m_lpDS) return SOUND_ERR_CANNOT_FIND_DS;

	if (m_dwState != STREAM_STATE_PAUSE) return STREAM_ERR_CANNOT_RESUME;

	m_pDSBuffer->Play(0,0,SOUND_PLAY_REPEAT);

	m_dwState	=	STREAM_STATE_PLAY;

	return SOUND_ERR_OK;
}

HRESULT
COGGPlayer::close(void)
{
	if (m_dwState	!=	STREAM_STATE_INITIALIZE && m_lpSoundObject)
	{
		if (!m_lpSoundObject->m_lpDS)
			return SOUND_ERR_CANNOT_FIND_DS;

		if (isPlaying())
			stop();

		m_lpSoundObject->closeFromFile (&m_handleSound);

		m_dwState	=	STREAM_STATE_INITIALIZE;

		if (m_lpFile)
		{
			fclose(m_lpFile);
			m_lpFile	=	NULL;
		}
	}

	return SOUND_ERR_OK;
}

HRESULT
COGGPlayer::setPan(long pan_v)
{

	if (!m_lpSoundObject->m_lpDS) return SOUND_ERR_CANNOT_FIND_DS;

	pan_v	=	max(pan_v,SOUND_PAN_LEFT);
	pan_v	=	min(pan_v,SOUND_PAN_RIGHT);

	if (!isPlaying() || FAILED(m_pDSBuffer->SetPan (pan_v)))
		return SOUND_ERR_CANNOT_SET_PAN;

	return SOUND_ERR_OK;

}

HRESULT
COGGPlayer::setVolume(long _lVolume,BOOL _bIsSetvalue)
{
	if (!m_lpSoundObject->m_lpDS) return SOUND_ERR_CANNOT_FIND_DS;

	if (_lVolume	>=	dMAX_SOUND_VOLUME/2)
		_lVolume	=	_lVolume;

	_lVolume	=	max(_lVolume,dMIN_SOUND_VOLUME);
	_lVolume	=	min(_lVolume,dMAX_SOUND_VOLUME);

	if (_bIsSetvalue)	m_iVolume	=	_lVolume;

	_lVolume	-=	SOUND_VOLUME_RANGE;

	if (!isPlaying() || FAILED(m_pDSBuffer->SetVolume(_lVolume)))
		return SOUND_ERR_CANNOT_SET_VOLUME;

	return SOUND_ERR_OK;
}

BOOL
COGGPlayer::decreaseVolume(int _iValue)
{
	m_iVolume	-=	_iValue;
	m_iVolume	=	min(m_iVolume,dMAX_SOUND_VOLUME);
	m_iVolume	=	max(m_iVolume,dMIN_SOUND_VOLUME);

	if (setVolume(m_iVolume) == SOUND_ERR_OK)	return	TRUE;

	return	FALSE;
}

BOOL
COGGPlayer::increaseVolume(int _iValue)
{
	m_iVolume	+=	_iValue;
	m_iVolume	=	min(m_iVolume,dMAX_SOUND_VOLUME);
	m_iVolume	=	max(m_iVolume,dMIN_SOUND_VOLUME);

	if (setVolume(m_iVolume) == SOUND_ERR_OK)	return	TRUE;

	return	FALSE;
}

HRESULT
COGGPlayer::getVolume(int *_lpiVolume)
{
	if (!m_lpSoundObject->m_lpDS) return SOUND_ERR_CANNOT_FIND_DS;

	LONG	lVolume;

	if (!isPlaying() || FAILED(m_pDSBuffer->GetVolume(&lVolume)))
		return SOUND_ERR_CANNOT_SET_VOLUME;

	*_lpiVolume	=	(lVolume+SOUND_VOLUME_RANGE);

	return SOUND_ERR_OK;
}

HRESULT 
COGGPlayer::setNextPosition(position_set *_lpPos)
{
	if (!m_lpSoundObject->m_lpDS) return SOUND_ERR_CANNOT_FIND_DS;

	if (_lpPos)	m_lpNextPos = _lpPos;
	else		STREAM_ERR_UNAVAILABLE_POSITION_SET;

	return STREAM_ERR_OK;
}

HRESULT
COGGPlayer::setCurrentPosition(position_set *_lpPos)
{
	if (!m_lpSoundObject->m_lpDS)
		return SOUND_ERR_CANNOT_FIND_DS;

	if (_lpPos)
	{
		CopyMemory (&m_pos, _lpPos, sizeof(position_set));
		m_lpNextPos = m_pos.next;
	}
	else
		STREAM_ERR_UNAVAILABLE_POSITION_SET;

	return STREAM_ERR_OK;

}

HRESULT
COGGPlayer::permitNextPosition(DWORD flag)
{
	if (!m_lpSoundObject->m_lpDS)
		return SOUND_ERR_CANNOT_FIND_DS;

	m_pos.option = flag;

	return STREAM_ERR_OK;
}


void
COGGPlayer::update()
{
	DWORD		dwRead		=	0;

	if (m_bIsFadeOut)
	{
		m_iCurrentVolume	-=	m_iFadeSpeed;

		if (m_iCurrentVolume<=	dMIN_SOUND_VOLUME+SOUND_VOLUME_RANGE/2)
		{
			m_iCurrentVolume=	dMIN_SOUND_VOLUME;
			m_bIsFadeOut	=	FALSE;

			if (m_bIsSmoothChange)
			{
				if (m_strBookedFile[0])
				{
					close();
					play(m_strBookedFile);
					setVolume(m_iVolume);

					return;
				}
			}

			stop();
			return;
		}

		setVolume(m_iCurrentVolume,FALSE);
	}

	if (m_dwState != STREAM_STATE_CLOSE || m_dwState != STREAM_STATE_LOAD)
	{
		if (isPlaying() )
		{
			if (m_lpSoundObject->m_bIsStreamStop == TRUE)
			{
				stop();
				m_lpSoundObject->m_bIsStreamStop = FALSE;
			}

			m_dwOldPlay	=	m_dwCurPlay;
			m_pDSBuffer->GetCurrentPosition(&m_dwCurPlay, NULL);
			m_dwCurPlay	/=	m_handleSound.fmt.nBlockAlign;

			if (m_dwOldPlay > m_dwCurPlay)
				m_dwPlaySize	+=	m_dwBufferSize - m_dwOldPlay + m_dwCurPlay;
			else
				m_dwPlaySize	+=	m_dwCurPlay - m_dwOldPlay;

			if (m_dwState == STREAM_STATE_WAIT_FOR_END && m_dwPlaySize >= m_pos.size)
				stop();
			else
			if(ABS((long)((m_dwPlaySize + m_dwTempPlaySize) - m_dwReadSize)) < (long)m_dwGap && m_dwState != STREAM_STATE_WAIT_FOR_END)
			{
				m_lpSoundObject->readFromFile (m_pDSBuffer, &m_handleSound, m_dwCurWrite, min(m_dwReadOnce, m_pos.size - m_dwReadSize), 0, &dwRead);
				m_dwReadSize += dwRead;
				m_dwCurWrite = (m_dwCurWrite + dwRead) % m_dwBufferSize;
			}

			if ((m_pos.option == STREAM_PERMIT_NEXT_NOW || m_dwReadSize >= m_pos.size) && m_dwState != STREAM_STATE_WAIT_FOR_END )
			{
				switch (m_dwOption)
				{
					case SOUND_PLAY_ONCE:
						m_dwState	=	STREAM_STATE_WAIT_FOR_END;
						break;

					case SOUND_PLAY_REPEAT:
						// rewind
						m_dwTempPlaySize = (long)((long)(m_dwPlaySize+m_dwTempPlaySize) - m_dwReadSize);
						m_dwReadSize = 0;
						m_dwPlaySize = 0;

						if (m_lpNextPos != NULL && (m_pos.option == STREAM_PERMIT_NEXT_ON || m_pos.option == STREAM_PERMIT_NEXT_NOW))
							// 새 포지션 정보를 갱신한다.
						{
							setCurrentPosition (m_lpNextPos);
						} 
						else // 현재 포지션 정보를 수정해서 loop
						{
							m_pos.start_pos = m_pos.loop_start_pos;
							m_pos.size -= (m_pos.loop_start_pos - m_pos.start_pos);
						}

						m_lpSoundObject->moveFromFile(&m_handleSound, m_pos.start_pos);//-(long)(m_dwReadSize), SOUND_MOVE_FROM_CURRENT);
						break;

					default:
						break;
				}
			}
		}
	}
}

LRESULT CALLBACK
COGGPlayer::TimerCallback(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	COGGPlayer	*lpStream	=	(COGGPlayer *)GetWindowLong(hWnd,GWL_USERDATA);

	switch( uMsg )
	{
		case	WM_TIMER	:
		{
			if(lpStream && wParam == lpStream->m_dwTimerHandler)
				lpStream->update();
			break;
		}
	}

	return CallWindowProc((WNDPROC)lpStream->m_procMasterWindow,hWnd,uMsg,wParam,lParam);
}
