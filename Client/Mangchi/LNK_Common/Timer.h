#ifndef	_class_timer_h
#define	_class_timer_h

#include	"lnk_define.h"
#pragma comment(lib, "winmm.lib")

#ifdef	WIN32
class cTimer
{
public:
	bool			m_bQueryPerformance;			// TRUE: 고해상도타이머 지원, FALSE: 고해상도타이머 실패
	LARGE_INTEGER	m_iCurrentCount;				// 현재 진동수
	LARGE_INTEGER	m_iLastCount;					// 예전 진동수
	LARGE_INTEGER	m_iCountperseconds;				// 초당 진동수

	u4				m_dCurrentTime;					// 현재 시간
	u4				m_dLastTime;					// 예전 시간

	float			m_fTimeDelta;					// 현재시간 - 예전시간
	float			m_fTimeElapsed;					// 출력 간격(초단위)
	int				m_iFrameCnt;					// Frame 증가
	

public:
	void	inItTimer();

	float   getSPF();
	bool	getFPS( float* fFPS );
	bool	timer( float* fDealTime );

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
	u2				m_u2Year,m_u2Month,m_u2Day,m_u2Hour,m_u2Minute,m_u2Second,m_u2WeekDay,m_u2Yday;	//	연,월,일,시,분,초,요일,연일 정보

					cClock();

	void			reset();
	int				getDayOfWeek();	// 1=Sun, 2=Mon, ..., 7=Sat
	void			getTime();	//	현재 시간 얻기
	void			increaseMinute(s4 _s4Minute);	//	분 단위 증가
	void			increaseHour(s4 _s4Hour,int _iHourPerDay=24);		//	시간 단위 증가
	void			increaseDay(s4 _s4Day);			//	일단위 증가
	void			increaseSecond(s4 _s4Second);	//	초 단위 증가

	s4				getTerm(cClock *_lpTime);	//	초단위 차이 시간 얻기
	s4				getTermByMinute(cClock *_lpTime);	//	분 단위 차이 시간 얻기
	s4				getTermByHour(cClock *_lpTime);	//	시간 단위 차이 시간 얻기
	s4				getTermByDay(cClock *_lpTime);	//	일 초단위 차이 시간 얻기
};

extern	s4			GetLastDayInMonth(s4 _s4Year,s4 _s4Month);	//	_s4Year년 _s4Month월의 마지막 일 리턴

#endif
