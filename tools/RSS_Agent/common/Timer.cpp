#include "stdafx.h"
#include "./timer.h"
#include "time.h"
#include <math.h>

s4 
GetLastDayInMonth(s4 _s4Year,s4 _s4Month)
{
	if	(_s4Year	<	2000)
		_s4Year	+=	2000;

	s4	as4LastDay[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if	(_s4Month==2)
	{
		if	((_s4Year%4 == 0) && (_s4Year%100 !=0) || _s4Year%400 == 0)
			return	29;
		else
			return	28;
	}

	return	as4LastDay[_s4Month];
}

cClock::cClock()
{
	memset(this,0,sizeof(cClock));
}

cClock& 
cClock::operator=(const cClock& _clock)
{
	memcpy(this,&_clock,sizeof(cClock));

	return *this;
}

void
cClock::reset()
{
	memset(this,0,sizeof(cClock));
}

int	l_aiMonthDays[13] =	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

double
cClock::getTimeValue()
{
	// Validate year and month (ignore day of week and milliseconds)
	if	(m_u2Year > 9999 || m_u2Month < 1 || m_u2Month > 12)
		return FALSE;

	//  Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((m_u2Year & 3) == 0) &&
		((m_u2Year % 100) != 0 || (m_u2Year % 400) == 0);

	int nDaysInMonth =	l_aiMonthDays[m_u2Month] - l_aiMonthDays[m_u2Month-1] +((bLeapYear && m_u2Day == 29 && m_u2Month == 2) ? 1 : 0);

	// Finish validating the date
	if	(m_u2Day < 1 || m_u2Day > nDaysInMonth ||m_u2Hour > 23 || m_u2Minute > 59 ||	m_u2Second > 59)
		return 0;

	// Cache the date in days and time in fractional days
	long	nDate;
	double	dblTime;

	//It is a valid date; make Jan 1, 1AD be 1
	nDate = m_u2Year*365L + m_u2Year/4 - m_u2Year/100 + m_u2Year/400 +	l_aiMonthDays[m_u2Month-1] + m_u2Day;

	//  If leap year and it's before March, subtract 1:
	if	(m_u2Month <= 2 && bLeapYear)
		--nDate;

	//  Offset so that 12/30/1899 is 0
	nDate -= 693959L;

	dblTime = (((long)m_u2Hour * 3600L) +  // hrs in seconds
		((long)m_u2Minute * 60L) +  // mins in seconds
		((long)m_u2Second)) / 86400.;

	return (double) nDate + ((nDate >= 0) ? dblTime : -dblTime);
}

#ifndef	HALF_SECOND 
#define HALF_SECOND  (1.0/172800.0)
#endif

int
cClock::getDayOfWeek()
{
	double	dtSrc	=	getTimeValue();
	struct	tm	tmDest;

	long nDays;             // Number of days since Dec. 30, 1899
	long nDaysAbsolute;     // Number of days since 1/1/0
	long nSecsInDay;        // Time in seconds since midnight
	long nMinutesInDay;     // Minutes in day

	long n400Years;         // Number of 400 year increments since 1/1/0
	long n400Century;       // Century within 400 year block (0,1,2 or 3)
	long n4Years;           // Number of 4 year increments since 1/1/0
	long n4Day;             // Day within 4 year block
							//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	long n4Yr;              // Year within 4 year block (0,1,2 or 3)
	BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year

	double dblDate = dtSrc; // tempory serial date

	// If a valid date, then this conversion should not overflow
	nDays = (long)dblDate;

	// Round to the second
	dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	// Calculate the day of week (sun=1, mon=2...)
	//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
	tmDest.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (long)(nDaysAbsolute / 146097L);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097L;

	// -1 because first century has extra day
	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = FALSE;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	// n4Day is now 0-based day of year. Save 1-based day of year, year number
	tmDest.tm_yday = (int)n4Day + 1;
	tmDest.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

	// Handle leap year: before, on, and after Feb. 29.
	if (n4Yr == 0 && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			/* Feb. 29 */
			tmDest.tm_mon = 2;
			tmDest.tm_mday = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
			--n4Day;
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time */
	for (tmDest.tm_mon = (n4Day >> 5) + 1;
		n4Day > l_aiMonthDays[tmDest.tm_mon]; tmDest.tm_mon++);

	tmDest.tm_mday = (int)(n4Day - l_aiMonthDays[tmDest.tm_mon-1]);

DoTime:
	if (nSecsInDay == 0)
		tmDest.tm_hour = tmDest.tm_min = tmDest.tm_sec = 0;
	else
	{
		tmDest.tm_sec = (int)nSecsInDay % 60L;
		nMinutesInDay = nSecsInDay / 60L;
		tmDest.tm_min = (int)nMinutesInDay % 60;
		tmDest.tm_hour = (int)nMinutesInDay / 60;
	}

	return TRUE;
}

void
cClock::getTime()
{
	time_t tTime=	0;
	struct tm tmTime;

	time(&tTime);

	localtime_s(&tmTime,&tTime);

	m_u2Year	=	1900+tmTime.tm_year;
	m_u2Month	=	tmTime.tm_mon+1;
	m_u2Day		=	tmTime.tm_mday;
	m_u2Hour	=	tmTime.tm_hour;
	m_u2Minute	=	tmTime.tm_min;
	m_u2Second	=	tmTime.tm_sec;
	m_u2WeekDay	=	tmTime.tm_wday;
	m_u2Yday	=	tmTime.tm_yday;
	m_u2MiliSecond=	min(m_u2MiliSecond,999);
}

//
//	_lpTime과 현재 시간의 초 단위 차이를 리턴한다.
s4
cClock::getTerm(cClock *_lpTime)
{
	cClock	earlyTime;
	cClock	lateTime;

	memcpy(&earlyTime,this,sizeof(cClock));
	memcpy(&lateTime,_lpTime,sizeof(cClock));

	if	(lateTime.getValueS()	<	earlyTime.getValueS())
		Swap(earlyTime,lateTime);

	u4	u4Days	=	0;

	if	(lateTime.m_u2Month	>=	earlyTime.m_u2Month && lateTime.m_u2Day	>=	earlyTime.m_u2Day)
	{
		int	iGabDays	=	(lateTime.m_u2Year-earlyTime.m_u2Year)*365;
		u4Days			+=	iGabDays;
		earlyTime.m_u2Year=	lateTime.m_u2Year;
	}
	else
	{
		int	iGabDays	=	(lateTime.m_u2Year-1-earlyTime.m_u2Year)*365;

		if	(iGabDays	>	0)
		{
			earlyTime.m_u2Year	=	lateTime.m_u2Year-1;
			u4Days			+=	iGabDays;
		}
	}

	while(1)
	{
		if	(lateTime.getValueD() == earlyTime.getValueD())
		{
			s4	s4CurrentSecondValue	=	earlyTime.m_u2Hour*60*60 + earlyTime.m_u2Minute*60+earlyTime.m_u2Second;
			s4	s4CheckSecondValue		=	lateTime.m_u2Hour*60*60 + lateTime.m_u2Minute*60+lateTime.m_u2Second;
			s4	s4SecondTerm			=	s4CheckSecondValue-s4CurrentSecondValue;
			s4	s4Term					=	u4Days*24*60*60+s4SecondTerm;

			return	s4Term;
		}

		if	(lateTime.m_u2Year	==	earlyTime.m_u2Year &&	lateTime.m_u2Month	==	earlyTime.m_u2Month)
		{
			u4Days			+=	(lateTime.m_u2Day-earlyTime.m_u2Day);
		}
		else
		{
			int	iLastDay	=	GetLastDayInMonth(earlyTime.m_u2Year,earlyTime.m_u2Month);
			int	iRemainDays	=	iLastDay-earlyTime.m_u2Day+1;
			earlyTime.m_u2Day=	1;
			u4Days			+=	iRemainDays;
			earlyTime.m_u2Month++;

			if	(earlyTime.m_u2Month	>	12)
			{
				earlyTime.m_u2Month	=	1;
				earlyTime.m_u2Year++;
			}
		}
	}

	return	u4Days;
}

//	분 단위 차이 시간 얻기
s4
cClock::getTermByMinute(cClock *_lpTime)
{
	s4	s4Term	=	getTerm(_lpTime);

	return	s4Term/60;
}

//	시간 단위 차이 시간 얻기
s4
cClock::getTermByHour(cClock *_lpTime)
{
	s4	s4Term	=	getTerm(_lpTime);

	return	s4Term/60/60;
}

//	일 초단위 차이 시간 얻기
s4
cClock::getTermByDay(cClock *_lpTime)
{
	s4	s4Term	=	getTerm(_lpTime);

	return	s4Term/60/60/24;
}

//
//	시간 증가
void
cClock::increaseDay(s4 _s4Day)
{
	while(_s4Day)
	{
		int	iLastDay	=	GetLastDayInMonth(m_u2Year,m_u2Month);
		int	iReaminDays	=	iLastDay-m_u2Day+1;

		if	(iReaminDays>=	_s4Day)
		{
			m_u2Day		=	1;
			_s4Day		-=	iReaminDays;

			m_u2Month++;

			if	(m_u2Month	>	12)
			{
				m_u2Month	=	1;
				m_u2Year++;
			}
		}
		else
		{
			m_u2Day		+=	_s4Day;
			_s4Day		=	0;
		}
	}
}

//
//	시간 증가
void
cClock::increaseHour(s4 _s4Hour,int _iHourPerDay)
{
	int	iIncreaseDay	=	_s4Hour/_iHourPerDay;
	int	iIncreaseHour	=	_s4Hour-(iIncreaseDay*_iHourPerDay);

	if	(iIncreaseDay)
		increaseDay(iIncreaseDay);

	for (;iIncreaseHour;)
	{
		int	iRemainHour		=	_iHourPerDay-m_u2Hour;

		if	(iIncreaseHour	>=	iRemainHour)
		{
			iIncreaseHour	-=	iRemainHour;
			m_u2Hour		=	0;
			increaseDay(1);
		}
		else
		{
			m_u2Hour		+=	iIncreaseHour;
			iIncreaseHour	=	0;
		}
	}
}

//
//	분단위 증가 증가
void
cClock::increaseMinute(s4 _s4Minute)
{
	int	iIncreaseHour	=	_s4Minute/60;
	int	iIncreaseMinute	=	_s4Minute-iIncreaseHour*60;

	if	(iIncreaseHour)
		increaseHour(iIncreaseHour);

	for (;iIncreaseMinute;)
	{
		int	iRemainMinute	=	60-m_u2Minute;

		if	(iIncreaseMinute>=	iRemainMinute)
		{
			iIncreaseMinute	-=	iRemainMinute;
			m_u2Minute		=	0;
			increaseHour(1);
		}
		else
		{
			m_u2Minute		+=	iIncreaseMinute;
			iIncreaseMinute	=	0;
		}
	}
}

//
//	초 단위 증가
void
cClock::increaseSecond(s4 _s4Second)
{
	int	iIncreaseMinute	=	_s4Second/60;
	int	iIncreaseSecond	=	_s4Second-iIncreaseMinute*60;

	if	(iIncreaseMinute)
		increaseMinute(iIncreaseMinute);

	for (;iIncreaseSecond;)
	{
		int	iRemainSecond	=	60-m_u2Second;

		if	(iIncreaseSecond>=	iRemainSecond)
		{
			iIncreaseSecond	-=	iRemainSecond;
			m_u2Second		=	0;

			increaseMinute(1);
		}
		else
		{
			m_u2Second		+=	iIncreaseSecond;
			iIncreaseSecond	=	0;
		}
	}
}

void
cClock::increaseMiliSecond(int _iDeltaMili)
{
	int	iIncreaseSecond	=	_iDeltaMili/1000;
	int	iIncreaseMili	=	_iDeltaMili-iIncreaseSecond*1000;

	if	(iIncreaseSecond)
		increaseSecond(iIncreaseSecond);

	for (;iIncreaseMili;)
	{
		int	iRemainMiliSecond	=	1000-m_u2MiliSecond;

		if	(iIncreaseMili	>=	iRemainMiliSecond)
		{
			iIncreaseMili	-=	iRemainMiliSecond;
			m_u2MiliSecond	=	0;

			increaseSecond(1);
		}
		else
		{
			m_u2MiliSecond	+=	iIncreaseMili;
			iIncreaseMili	=	0;
		}
	}
}

#ifdef	WIN32
#include <Mmsystem.h>

LONGLONG	cTimer::s_llTPS	=	0;
BOOL		cTimer::s_bIsSupportQueryPerformanceFrequency	=	FALSE;

cTimer::cTimer(void)
{
	m_iCurrentCount.QuadPart	= 0;
	m_iLastCount.QuadPart		= 0;

	m_dCurrentTime				= 0;
	m_dLastTime					= 0;

	m_fTimeDelta				= 0.0f;
	m_fTimeElapsed				= 0.0f;
	m_iFrameCnt					= 0;

	memset(&m_frameSkipInfo,0,sizeof(m_frameSkipInfo));

	if	(s_llTPS	==	0)
	{	
		LARGE_INTEGER time;

		time.LowPart	=	0;
		time.HighPart	=	0;

		if	(!QueryPerformanceCounter( &time ))
		{
			s_bIsSupportQueryPerformanceFrequency	=	FALSE;
			return;
		}

		s_bIsSupportQueryPerformanceFrequency		=	TRUE;

		QueryPerformanceFrequency((LARGE_INTEGER*)&s_llTPS);
	}
}

cTimer::~cTimer(void)
{
}

void cTimer::initTimer()
{
	//if	(s_bIsSupportQueryPerformanceFrequency	==	FALSE)
	//{
		m_dCurrentTime	=	timeGetTime();
		m_dLastTime		=	m_dCurrentTime;
	//}
	//else
	//{
	//	QueryPerformanceCounter( &m_iCurrentCount );

	//	m_iLastCount.QuadPart	=	m_iCurrentCount.QuadPart;
	//}
}

float cTimer::getSPF()
{
//	if	(s_bIsSupportQueryPerformanceFrequency	==	FALSE) // timeGetTime
//	{
		m_dCurrentTime	=	timeGetTime();
		m_fTimeDelta	=	(float)(m_dCurrentTime - m_dLastTime) / 1000.0f;
		m_dLastTime		=	m_dCurrentTime;

//	}
//	else					 // 고해상도
//	{	
//		QueryPerformanceCounter( &m_iCurrentCount );
//		m_iCurrentCount.QuadPart	=	timeGetTime();
//		m_fTimeDelta	=	(float)( m_iCurrentCount.QuadPart - m_iLastCount.QuadPart ) / (float)1000;
//		m_iLastCount.QuadPart = m_iCurrentCount.QuadPart;
//	}

	return m_fTimeDelta;
}

bool cTimer::getFPS( float* fFPS )
{
	m_iFrameCnt++;

	m_fTimeElapsed += getSPF();
	
	if	(m_fTimeElapsed >= 1.0f )
	{
		*fFPS = (float)m_iFrameCnt / m_fTimeElapsed;

		m_fTimeElapsed	= 0.0f;
		m_iFrameCnt		= 0;

		return TRUE;
	}
	else
		return FALSE;
}

bool cTimer::timer( float* fDealTime )
{
	m_fTimeElapsed += getSPF();
	
	if	(m_fTimeElapsed >= *fDealTime )
	{
		m_fTimeElapsed = 0.0f;

		return TRUE;
	}
	else
		return FALSE;
}

//
//	프레임 스킵 설정
BOOL
cTimer::setFrameCount(int _iFrameCount)
{
	if	(_iFrameCount <= 0 )
	{	
		m_frameSkipInfo.m_llTickPerFrame	=	0;
		return TRUE;
	}

	m_frameSkipInfo.m_llTickPerFrame	=	1000/ _iFrameCount;
	m_frameSkipInfo.m_iWantFrame		=	_iFrameCount;

	resetFrameSkip();

	return TRUE;
}

void
cTimer::resetFrameSkip()
{
	m_frameSkipInfo.m_llOldTime	=	timeGetTime();
//	QueryPerformanceCounter((LARGE_INTEGER*) &m_frameSkipInfo.m_llOldTime);
}

//	1/1000 초 시간 업데이트. timeGetTime을 사용한다.
void
cTimer::updateMiliSecond()
{
	m_uiMiliSecond	=	timeGetTime();
}

//
//	이번 프레임 스킵할까?
BOOL 
cTimer::isSkipFrame()
{
	if	(m_frameSkipInfo.m_llTickPerFrame	==	0)	
	{
		Sleep(1);

		return FALSE;
	}

	LONGLONG	llNewTime,llTimeGab;

	llNewTime					=	timeGetTime();
	llTimeGab					=	llNewTime-m_frameSkipInfo.m_llOldTime;
	m_frameSkipInfo.m_llOverTick=	llTimeGab-m_frameSkipInfo.m_llTickPerFrame+m_frameSkipInfo.m_llOverTick;

	if	(m_frameSkipInfo.m_llOverTick < 0 )	//	너무 빠르다. 여유 좀 부리자.
	{
		while(m_frameSkipInfo.m_llOverTick < 0 )
		{
			m_frameSkipInfo.m_llOldTime		=	llNewTime;

			Sleep(1);

			llNewTime					=	timeGetTime();
		//	QueryPerformanceCounter( (LARGE_INTEGER*)&llNewTime);

			llTimeGab						=	llNewTime - m_frameSkipInfo.m_llOldTime;	//	흐른 시간.
			m_frameSkipInfo.m_llOverTick	+=	llTimeGab;
			m_frameSkipInfo.m_llOldTime		=	llNewTime;
		}

		m_frameSkipInfo.m_iSkipCount	=	0;

		return FALSE;	//	이번 프레임은 프레임 스킵 할 필요가 없다.
	}

	++m_frameSkipInfo.m_iSkipCount;

	if	(m_frameSkipInfo.m_iSkipCount	>	m_frameSkipInfo.m_iWantFrame/2)		//	최소 1초에 두번은 보이게 하자!
	{
		m_frameSkipInfo.m_llOldTime		=	llNewTime;
		m_frameSkipInfo.m_llOverTick	=	0;
		m_frameSkipInfo.m_iSkipCount	=	0;

		return FALSE;
	}

	m_frameSkipInfo.m_llOldTime	=	llNewTime;

	return TRUE;
}

//
//	FPS측정

#endif