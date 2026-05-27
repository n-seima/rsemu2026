#ifndef	_class_timer_h
#define	_class_timer_h

#include	"lnk_define.h"
#pragma comment(lib, "winmm.lib")

#ifdef	WIN32
class cTimer
{
	UINT			m_uiMiliSecond;	//	밀리 세컨드
public:
	static	LONGLONG	s_llTPS;	//	tps 초당 몇번이나 튀는가!!??
	static	BOOL		s_bIsSupportQueryPerformanceFrequency;

	LARGE_INTEGER	m_iCurrentCount;				// 현재 진동수
	LARGE_INTEGER	m_iLastCount;					// 예전 진동수
	LARGE_INTEGER	m_iCountperseconds;				// 초당 진동수

	u4				m_dCurrentTime;					// 현재 시간
	u4				m_dLastTime;					// 예전 시간

	float			m_fTimeDelta;					// 현재시간 - 예전시간
	float			m_fTimeElapsed;					// 출력 간격(초단위)
	int				m_iFrameCnt;					// Frame 증가
	

public:
	void			initTimer();

	float			getSPF();
	bool			getFPS( float* fFPS );
	bool			timer( float* fDealTime );

	void			updateMiliSecond();	//	1/1000 초 시간 업데이트. timeGetTime을 사용한다.
	inline	UINT	getMiliSecond()	//	1/1000 초 시간 리턴
	{
		return	m_uiMiliSecond;
	}

//	프레임 스키핑 관련
public:
	class	cFrameSkipInfo
	{
	public:
		LONGLONG	m_llTickPerFrame;	//	프레임당 시스템 Tick
		LONGLONG	m_llOldTime;		//	프레임 스키핑을 위한.. 시간..
		LONGLONG	m_llOverTick;		//	오버된 틱(시간)
		int			m_iSkipCount,m_iWantFrame;
	};

	cFrameSkipInfo	m_frameSkipInfo;

	BOOL			setFrameCount(int _iFrame);	//	초당 몇프레임으로 돌릴까요??
	BOOL			isSkipFrame();		//	이번 프레임 스킵?
	void			resetFrameSkip();	//	프레임 스킵 관련 정보 초기화

public:
	cTimer(void);
	virtual ~cTimer(void);
};
#endif

//	시간을 체크 한다.
class	cClock
{
private:
	double			getTimeValue();
public:
	u2				m_u2Year,m_u2Month,m_u2Day,m_u2Hour,m_u2Minute,m_u2Second,m_u2MiliSecond,m_u2WeekDay,m_u2Yday;	//	연,월,일,시,분,초,요일,연일 정보

					cClock();

	inline	int		getValueS()	//	정확도가 떨어지는 초단위 수치 리턴
								//	시간간의 > , == , < 만 빠르게 체크하기 위한 함수
	{
		int	iDayValue	=	m_u2Year*372+(m_u2Month-1)*31+m_u2Day;
		int	iSecondValue=	m_u2Hour*60*60+m_u2Minute*60+m_u2Second;

		return	iDayValue*24*60*60+iSecondValue;
	}
	inline	int		getValueD()	//	정확도가 떨어지는 일단위 수치 리턴
								//	시간간의 > , == , < 만 빠르게 체크하기 위한 함수
	{
		return	m_u2Year*372+(m_u2Month-1)*31+m_u2Day;
	}


	cClock&			operator=(const cClock& _clock); 

	void			reset();
	int				getDayOfWeek();	// 1=Sun, 2=Mon, ..., 7=Sat
	void			getTime();	//	현재 시간 얻기
	void			increaseMinute(s4 _s4Minute);	//	분 단위 증가
	void			increaseHour(s4 _s4Hour,int _iHourPerDay=24);		//	시간 단위 증가
	void			increaseDay(s4 _s4Day);			//	일단위 증가
	void			increaseSecond(s4 _s4Second);	//	초 단위 증가
	void			increaseMiliSecond(int _iDeltaMili);	//	밀리초 단위 증가

	s4				getTerm(cClock *_lpTime);	//	초단위 차이 시간 얻기
	s4				getTermByMinute(cClock *_lpTime);	//	분 단위 차이 시간 얻기
	s4				getTermByHour(cClock *_lpTime);	//	시간 단위 차이 시간 얻기
	s4				getTermByDay(cClock *_lpTime);	//	일 초단위 차이 시간 얻기
};

extern	s4			GetLastDayInMonth(s4 _s4Year,s4 _s4Month);	//	_s4Year년 _s4Month월의 마지막 일 리턴

#endif
