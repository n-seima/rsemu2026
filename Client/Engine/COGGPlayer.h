#ifndef STREAM_HEADER
#define STREAM_HEADER

/*

	stream.h

	stream 객체.



				Fuzzface in 2003.

*/

#include "CSound.h"

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"ogg/ogg_static.lib")
#pragma comment(lib,"ogg/vorbis_static.lib")
#pragma comment(lib,"ogg/vorbisfile_static.lib")

#define STREAM_MAX					5

#define STREAM_PERMIT_NEXT_OFF		0
#define STREAM_PERMIT_NEXT_ON		1
#define STREAM_PERMIT_NEXT_NOW		2

#define STREAM_STATE_ERR			0
#define STREAM_STATE_NONE			1
#define STREAM_STATE_INITIALIZE		2
#define STREAM_STATE_LOAD			3
#define STREAM_STATE_READY			4
#define STREAM_STATE_PLAY			5
#define STREAM_STATE_PAUSE			6
#define STREAM_STATE_STOP			7
#define STREAM_STATE_CLOSE			8
#define STREAM_STATE_WAIT_FOR_END	9

#define STREAM_ERR_OK						0
#define STREAM_ERR_CANNOT_CREATE			-1
#define STREAM_ERR_CANNOT_PLAY				-2
#define STREAM_ERR_CANNOT_PAUSE				-3
#define STREAM_ERR_CANNOT_RESUME			-4
#define STREAM_ERR_CANNOT_STOP				-5
#define STREAM_ERR_CANNOT_DELETE			-6
#define STREAM_ERR_NOT_READY				-7
#define STREAM_ERR_UNAVAILABLE_SOUND_OBJECT	-8
#define STREAM_ERR_UNAVAILABLE_HWND			-9
#define STREAM_ERR_UNAVAILABLE_POSITION_SET	-10
#define	STREAM_ERR_UNAVAILABLE_TIMER_INDEX	-11

#define	dDEFAULT_FADE_SPEED					100

typedef struct tag_position_set
{

	DWORD						index;				//	포지션 정보의 인덱스 넘버
	DWORD						option;				//	옵션
	DWORD						start_pos;			//	시작 부분의 샘플 넘버
	DWORD						loop_start_pos;		//	루핑할 부분의 샘플 넘버
	DWORD						size;				//	전체 샘플 수
	struct tag_position_set		*next;				//	다음 포지션 정보의 주소

} position_set;

class COGGPlayer
{
private:
	FILE				*m_lpFile;
	IDirectSoundBuffer8	*m_pDSBuffer;			//	실제 스트림의 버퍼
	HWND				m_hWnd;						//	윈도 핸들
	CSound				*m_lpSoundObject;			//	사운드 객체의 포인터
	DWORD				m_dwBufferSize;				//	버퍼의 전체 크지 (샘플 단위)
	DWORD				m_dwGap;					//	스트림 읽기 커서와 플레이 커서간의 최소 거리
	DWORD				m_dwReadOnce;				//	한번에 읽는 최대 샘플 수
	DWORD				m_dwOption;					//	옵션
	DWORD				m_dwState;					//	현재 상태
	DWORD				m_dwSize;					//	버퍼의 전체 샘플 수
	DWORD				m_dwPlaySize;				//	현재까지 플레이한 샘플 수
	DWORD				m_dwReadSize;				//	현재까지 읽어들인 샘플 수
	DWORD				m_dwTempPlaySize;			//	스트림이 한바퀴 돌때 이전 루프에 플레이 되지 않은 샘플 수
	DWORD				m_dwCurPlay;				//	현재 플레이 커서
	DWORD				m_dwOldPlay;				//	이전 플레이 커서
	DWORD				m_dwCurWrite;				//	현재 읽기	커서
	position_set		m_pos;
	position_set		*m_lpNextPos;
	sound_handle		m_handleSound;				//	사운드 핸들
	WNDPROC				m_procMasterWindow;
	char				m_strLoadedFile[128];
	char				m_strBookedFile[128];

	BOOL				m_bIsPlaying;
	BOOL				m_bIsFadeOut;
	BOOL				m_bIsFadeIn;
	int					m_iFadeSpeed;
	int					m_iCurrentVolume;
	BOOL				m_bIsSmoothChange;
	DWORD				m_dwTimerHandler;

public:
	int					m_iVolume;

						COGGPlayer();
						~COGGPlayer(void);

	int					init(HWND _hWnd, CSound *_lpSoundObject,DWORD _dwBufferSize,DWORD _dwGabSize,DWORD _dwReadOnceSize,DWORD _dwTimerHandle);
	HRESULT				load(FILE *fp, position_set *_lpPos=NULL);
	HRESULT				play(DWORD _dwOption,int _iVolume=-1);

	BOOL				play(char *_lpstrFile,DWORD _dwOption=0xffffffff);
	void				setPlayOption(DWORD _dwOption) {m_dwOption = _dwOption;}

	HRESULT				isPlaying(void)
	{
		return ((this->m_dwState == STREAM_STATE_PLAY || this->m_dwState == STREAM_STATE_WAIT_FOR_END) ? TRUE : FALSE ); 
	}

	HRESULT				stop(void);
	HRESULT				pause(void);
	HRESULT				resume(void);
	HRESULT				setPan(long pan_v);


	HRESULT				setVolume(long _lVolume,BOOL _bIsSetvalue=TRUE);
	void				setSmoothChange(BOOL _bIsSettingValue)	
	{
		m_bIsSmoothChange	=	_bIsSettingValue;
	}

	BOOL				decreaseVolume(int _iValue);
	BOOL				increaseVolume(int _iValue);
	void				fadeOut(int _iFadeSpeed=-1)
	{
		m_iCurrentVolume=	m_iVolume;
		m_bIsFadeOut	=	TRUE;

		if	(_iFadeSpeed!=	-1)	m_iFadeSpeed	=	_iFadeSpeed;
	}
	void				fadeIn(int _iFadeSpeed=-1)
	{
		m_iCurrentVolume=	m_iVolume;
		m_bIsFadeIn		=	TRUE;

		if	(_iFadeSpeed!=	-1)	m_iFadeSpeed	=	_iFadeSpeed;
	}
	void				setFadeSpeed(int _iFadeSpeed)
	{
		m_iFadeSpeed	=	_iFadeSpeed;
	}

	HRESULT				getVolume(int *_lpiVolume);
	HRESULT				close(void);
	HRESULT				setNextPosition(position_set *p);
	HRESULT				setCurrentPosition(position_set *p);
	HRESULT				permitNextPosition(DWORD flag);
	void				update();


	static	LRESULT CALLBACK	TimerCallback(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};


#endif