#ifndef CBGPLAY_H
#define CBGPLAY_H

#include "SFC.H"
#include <dshow.h>

#pragma comment(lib,"strmiids.lib")

class CBGPlay{
public:
			CBGPlay();
			~CBGPlay();

	BOOL	m_IsFileSet;	//	Create가 성공적이었다면 true가 설정됀다.
	BOOL	m_bLoop;
	int		m_iVolume;

	HRESULT Create(char *SoundFile);
	void	Destroy();

	BOOL	PlayPosition(LONGLONG llPos);
	BOOL	Play(char *fn,BOOL _bLoop=FALSE);
	HRESULT Stop();

	BOOL	IsPlaying();
	BOOL	IsEndPosition();	//	일정한 주기로 체크해주기바람.

	HRESULT get_Balance(long *plBalance);
	HRESULT get_Volume(long *plVolume);
	HRESULT put_Balance(long lBalance);
	HRESULT put_Volume(long lVolume);

protected:
	HRESULT SetSoundFile(char *pstrFiles);
	
	IGraphBuilder *m_pGraphBuilder;
	IMediaControl *m_pMediaControl;
	IMediaSeeking *m_pMediaSeeking;
	IBasicAudio *m_pAudio;
};

#endif