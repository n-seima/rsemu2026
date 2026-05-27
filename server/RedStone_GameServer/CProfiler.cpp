#include "CProfiler.H"


CProfiler::CProfiler()
{
	reset();
}

void
CProfiler::reset()
{
	m_iNumbers		=	0;
	m_dwCurrentWorkTime	=	0;
	m_dwHitCounter	=	0;
	m_llUseTime		=	0;		//사용한 시간.

	for (int i=0;i<dMAX_PROFILE;i++)
		m_aProfiles[i].reset();
}

void
CProfiler::sleep()
{
	m_timerSleep.startTimer();
}

void
CProfiler::wakening()
{
	m_dwSleepTime	+=	m_timerSleep.stopTimer();
}

void
CProfiler::begin()
{
	m_timer.startTimer();

	m_dwSleepTime		=	0;
	m_dwCurrentWorkTime	=	0;
}

void
CProfiler::end()
{
	int		i;

	m_dwCurrentWorkTime	=	m_timer.stopTimer();
	m_dwCurrentWorkTime	-=	m_dwSleepTime;
	m_llUseTime			+=	m_dwCurrentWorkTime;
	m_dwHitCounter	++;

	if (m_dwCurrentWorkTime	>	0)
	{
		for (i=0;i<m_iNumbers;i++)
		{
			m_aProfiles[i].m_dwMaxCount		=	max(m_aProfiles[i].m_dwCurrentHitCount,m_aProfiles[i].m_dwMaxCount);
			m_aProfiles[i].m_dwMaxWorkTime	=	max(m_aProfiles[i].m_dwCurrentWorkTime,m_aProfiles[i].m_dwMaxWorkTime);

			if(m_dwHitCounter == m_aProfiles[i].m_dwHitCounter)
				m_aProfiles[i].m_dwPercent	=	(DWORD)(m_aProfiles[i].m_llUseTime*100000/m_llUseTime);
			else
				m_aProfiles[i].m_dwPercent	=	(DWORD)(m_aProfiles[i].m_dwCurrentWorkTime*100000/m_dwCurrentWorkTime);
		}
	}

	for (i=0;i<m_iNumbers;i++)
	{
		m_aProfiles[i].m_dwCurrentHitCount	=	0;
		m_aProfiles[i].m_dwCurrentWorkTime	=	0;
	}
}

void
CProfiler::begin(char *name)
{	
	if (m_iNumbers >= dMAX_PROFILE)
		return;

	DWORD	dwHashCode	=	GetHashCode((BYTE*)name);

	for (int i = 0;i < m_iNumbers; i++)
	{
		if (dwHashCode	==	m_aProfiles[i].m_dwHashCode)
			if (strcmp(name,m_aProfiles[i].m_strName) == 0)
			{
				m_aProfiles[i].begin();
				return;
			}
	}

	m_aProfiles[m_iNumbers++].begin(name);
}

void
CProfiler::beginByID(DWORD _dwSerial,char *_lpstrName)
{	
	if (m_iNumbers >= dMAX_PROFILE)
		return;

	for (int i = 0;i < m_iNumbers; i++)
	{
		if (_dwSerial	==	m_aProfiles[i].m_dwID)
		{	
			m_aProfiles[i].begin();
			return;
		}
	}

	m_aProfiles[m_iNumbers++].beginByID(_dwSerial,_lpstrName);
}

void
CProfiler::endByID(DWORD _dwSerial)
{	
	DWORD	curTime	=	cTIMER::GetTime();

	for (int i = 0;i < m_iNumbers; i++)
		if (_dwSerial	==	m_aProfiles[i].m_dwID)
		{
			m_aProfiles[i].end(curTime);

			return;
		}
}

void
CProfiler::end(char *name)
{	
	DWORD	curTime	=	cTIMER::GetTime();

	DWORD	dwHashCode	=	GetHashCode((BYTE*)name);

	for (int i = 0;i < m_iNumbers; i++)
		if (dwHashCode	==	m_aProfiles[i].m_dwHashCode)
			if (strcmp(name,m_aProfiles[i].m_strName) == 0)
			{
				m_aProfiles[i].end(curTime);

				return;
			}
}

void
CProfiler::begin(int _iIndex)
{
	if (_iIndex >= dMAX_PROFILE) return;

	m_aProfiles[_iIndex].begin();
}

void
CProfiler::end(int _iIndex)
{	
	if (_iIndex >= dMAX_PROFILE) return;

	m_aProfiles[_iIndex].end(cTIMER::GetTime());
}

char *
CProfiler::getAverageTime(int index)
{	
	static	char	tempStr[64];

	long	time	=	(long)cTIMER::getMiliSec(m_aProfiles[index].m_dwAverageTime);

	strcpy(tempStr,_ms("%3d.%.3d",time/1000,time%1000));
	
	return	tempStr;
}

char *
CProfiler::getCurrentTimeInfo(int index)
{	
	static	char	tempStr[128];

	long	time	=	(long)cTIMER::getMiliSec(m_aProfiles[index].m_dwCurrentWorkTime);
	long	maxTime	=	(long)cTIMER::getMiliSec(m_aProfiles[index].m_dwMaxWorkTime);
	strcpy(tempStr,_ms("%5d.%.3d:%5d.%.3d[%5d]",time/1000,time%1000,maxTime/1000,maxTime%1000,m_aProfiles[index].m_dwCurrentHitCount));

	return	tempStr;
}

char *
CProfiler::getTime(int index)
{	
	static	char	tempStr[64];

	long	time	=	(long)cTIMER::getMiliSec(m_aProfiles[index].m_dwCurrentWorkTime);

	strcpy(tempStr,_ms("%3d.%.3d",time/1000,time%1000));
	
	return	tempStr;
}

char *
CProfiler::getTotalTime(int index)
{	
	static	char	tempStr[64];

	LONGLONG	time	=	cTIMER::getMiliSec(m_aProfiles[index].m_llUseTime);

	strcpy(tempStr,_ms("%10d.%.3d",time/1000,time%1000));

	return	tempStr;
}

char*
CProfiler::getPercent(int index)
{
	static	char	tempStr[64];

	strcpy(tempStr,_ms("%3d.%.3d",m_aProfiles[index].m_dwPercent/1000,m_aProfiles[index].m_dwPercent%1000));

	return tempStr;
}

int
CProfiler::getHitCount(int index)
{
	return	m_aProfiles[index].m_dwHitCounter;
}
int
CProfiler::getMaxHitCount(int index)
{
	return	m_aProfiles[index].m_dwMaxCount;
}