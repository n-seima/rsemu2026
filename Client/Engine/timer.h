#include <winsock2.H>
#include <windows.h>
#include <stdio.h>
#include <dxerr8.h>

#pragma	once

#pragma comment(lib,"winmm.lib")

class cTimer
{
public:
	double				m_dblTPS;
	static	LONGLONG	s_llTps,s_llLastTime;			//	tps 초당 몇번이나 튀는가!!??
	static	int			s_iFps,s_iFrameCounter;	//	FPS 측정을 위한 수치

	LONGLONG			m_llTick;		//	이름 그대로.. Frame Per Second
	LONGLONG			m_llOldTime;		//	프레임 스키핑을 위한.. 시간..
	LONGLONG			m_llNewTime;
	LONGLONG			m_llOverTick;
	LONGLONG			m_llTimerStartTime;
	double				m_dblTick;

	int					m_iSkipCount;	//	프레임 스킵한 횟수.

						cTIMER();

	void				startTimer();
	int					stopTimer();
	BOOL				setFPS(int fps);	//	초당 몇프레임으로 돌릴까요??
	BOOL				isSkipFrame();
	void				resetFrameSkip();

	static	inline	int	getMiliSec(LONGLONG _llTime)
	{
		_llTime	=	_llTime*1000000/s_llTps;

		return (int)_llTime;
	}

	static	inline	int	GetTime()
	{
		LARGE_INTEGER	time;

		QueryPerformanceCounter( &time );

		return (int)(time.LowPart>>2);	//	32비트 풀로 받으면 계산하기 짱나니까.. 30비트로 바꾼다..
										//	사인드 플래기가 어쩌구 저쩌구.. 궁시렁..
	}
	static	inline	int	GetFPS(){return s_iFps;}
	static	int			UpdateFPS();
};

