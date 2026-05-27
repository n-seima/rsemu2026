#include "cPROFILE.H"


CProfiler::CProfiler()
{
	m_iNumbers		=	0;
	m_dwTime		=	0;
	m_dwCounter		=	0;
	m_llUseTime		=	0;		//사용한 시간.
}

void
CProfiler::begin()
{	m_timer.startTimer();
}

void
CProfiler::end()
{	m_dwTime		=	m_timer.stopTimer();
	m_llUseTime		+=	m_dwTime;
	m_dwCounter		++;

	for (int i=0;i<m_iNumbers;i++)
	{	if(m_dwCounter == m_aProfiles[i].m_dwCounter)
				m_aProfiles[i].m_dwPercent	=	(DWORD)(m_aProfiles[i].m_llUseTime*100000/m_llUseTime);
		else	m_aProfiles[i].m_dwPercent	=	(DWORD)(m_aProfiles[i].m_dwTime*100000/m_dwTime);
	}
}

void
CProfiler::begin(char *name)
{	if (m_iNumbers >= dMAX_PROFILE) return;

	for (int i = 0;i < m_iNumbers; i++)
		if (strcmp(name,m_aProfiles[i].m_strName) == 0)
		{	m_aProfiles[i].begin();
			return;
		}

	m_aProfiles[m_iNumbers++].begin(name);
}

void
CProfiler::end(char *name)
{	
	DWORD	curTime	=	cTIMER::GetTime();

	for (int i = 0;i < m_iNumbers; i++)
		if (strcmp(name,m_aProfiles[i].m_strName) == 0)
		{	m_aProfiles[i].end(curTime);
			return;
		}
}

char *
CProfiler::getAverageTime(int index)
{	static	char	tempStr[64];

	long	time	=	cTIMER::getMiliSec(m_aProfiles[index].m_dwAverageTime);

	strcpy(tempStr,_ms("%.3d.%.3d",time/1000,time%1000));
	
	return	tempStr;
}

char *
CProfiler::getTime(int index)
{	static	char	tempStr[64];

	long	time	=	cTIMER::getMiliSec(m_aProfiles[index].m_dwTime);

	strcpy(tempStr,_ms("%.3d.%.3d",time/1000,time%1000));
	
	return	tempStr;
}

char *
CProfiler::getTotalTime(int index)
{	static	char	tempStr[64];

	long	time	=	cTIMER::getMiliSec(m_aProfiles[index].m_llUseTime);

	strcpy(tempStr,_ms("%.7d.%.3d",time/1000,time%1000));
	
	return	tempStr;
}

char*
CProfiler::getPercent(int index)
{
	static	char	tempStr[64];

	strcpy(tempStr,_ms("%.3d.%.3d",m_aProfiles[index].m_dwPercent/1000,m_aProfiles[index].m_dwPercent%1000));

	return tempStr;
}

int
CProfiler::getHitCount(int index)
{
	return	m_aProfiles[index].m_dwCounter;
}