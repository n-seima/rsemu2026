#ifndef _classPROFILE_H
#define _classPROFILE_H

#define	dMAX_PROFILE	200

#include "SFC.H"

class	CProfiler
{
public:
	class	CItem
	{
	public:
		DWORD			m_dwBeginTime,m_dwAverageTime,m_dwPercent;
		LONGLONG		m_llUseTime;	//사용한 시간.
		DWORD			m_dwHitCounter;	//	몇번 했냐?
		DWORD			m_dwMaxCount;	//	몇번 했냐?
		DWORD			m_dwCurrentHitCount;	//	몇번 했냐?
		DWORD			m_dwCurrentWorkTime;	//	몇번 했냐?
		DWORD			m_dwMaxWorkTime;	//	몇번 했냐?
		DWORD			m_dwHashCode;
		DWORD			m_dwID;
		char			m_strName[128];
		cTIMER			m_timer;

						CItem()
						{	
							reset();
						}

		void			reset()
		{
			memset(this,0,sizeof(CItem));
		}

		void			begin(char *name = NULL)
						{	
							if (name && m_strName[0] == 0)
							{
								strncpy(m_strName,name,sizeof(m_strName)-1);
								m_dwHashCode	=	GetHashCode((BYTE*)m_strName);
							}

							m_dwBeginTime	=	cTIMER::GetTime();
						}

		void			beginByID(DWORD _dwID,char *_lpstrName=NULL)
						{	
							if (_lpstrName && m_strName[0] == NULL)
							{
								strncpy(m_strName,_lpstrName,sizeof(m_strName)-1);
								m_dwHashCode	=	GetHashCode((BYTE*)m_strName);
							}

							m_dwID			=	_dwID;

							m_dwBeginTime	=	cTIMER::GetTime();
						}

		void			end(DWORD time)
						{	
							int	iWorkTime		=	time	-	m_dwBeginTime;

							if (iWorkTime<0)
								iWorkTime		+=	0x40000000;

							m_dwHitCounter++;
							m_dwCurrentHitCount++;
							m_llUseTime			+=	iWorkTime;
							m_dwCurrentWorkTime	+=	iWorkTime;

							if (m_dwHitCounter	==	0)
							{
								memset(this,0,sizeof(CProfiler::CItem)-sizeof(m_strName)-sizeof(cTIMER));
								m_dwHitCounter		=	0;
								m_dwCurrentHitCount	=	0;
								m_llUseTime			=	0;
								m_dwCurrentWorkTime	=	0;
								m_dwAverageTime		=	0;
								return;
							}

							m_dwAverageTime		=	(DWORD)(m_llUseTime/m_dwHitCounter);
						}
	};

	DWORD			m_dwBeginTime,m_dwEndTime,m_dwCurrentWorkTime,m_dwHitCounter,m_dwSleepTime;
	LONGLONG		m_llUseTime;		//사용한 시간.
	int				m_iNumbers;
	CItem			m_aProfiles[dMAX_PROFILE];
	cTIMER			m_timer,m_timerSleep;

					CProfiler();
					
	void			reset();					
	void			begin();	//	처음..
	void			end();		//	마지막(반드시..)

	void			sleep();
	void			wakening();

	void			begin(char *name);
	void			end(char *name);
	void			beginByID(DWORD _dwSerial,char *_lpstrName=NULL);
	void			endByID(DWORD _dwSerial);
	void			begin(int _iIndex);
	void			end(int _iIndex);
	void			build();

	char*			getAverageTime(int index);
	char*			getTime(int index);
	char*			getTotalTime(int index);
	char*			getPercent(int index);
	char*			getName(int index) 
	{
		if (m_aProfiles[index].m_strName[0] == NULL)
			return	NULL;
		
		return m_aProfiles[index].m_strName;
	}
	char*			getCurrentTimeInfo(int index);
	int				getHitCount(int index);
	int				getMaxHitCount(int index);
};



#endif