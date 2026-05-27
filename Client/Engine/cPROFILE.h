#ifndef _classPROFILE_H
#define _classPROFILE_H

#define	dMAX_PROFILE	100

#include "SFC.H"

class	CProfiler
{
public:
	class	CItem
	{
	public:
		DWORD			m_dwBeginTime,m_dwTime,m_dwAverageTime,m_dwPercent;
		LONGLONG		m_llUseTime;	//사용한 시간.
		DWORD			m_dwCounter;	//	몇번 했냐?
		char			m_strName[128];
		cTIMER			m_timer;


						CItem()
						{
							memset(this,0,sizeof(CItem));
						}

		void			begin(char *name = NULL)
						{
							if (name)
								strncpy(m_strName,name,sizeof(m_strName)-1);

							m_dwBeginTime	=	cTIMER::GetTime();
						}

		void			end(DWORD time)
						{	m_dwTime		=	time	-	m_dwBeginTime;

							if (m_dwTime<0) m_dwTime	+=	0x40000000;

							m_dwCounter++;
							m_llUseTime		+=	m_dwTime;

							m_dwAverageTime	=	(DWORD)(m_llUseTime/m_dwCounter);
						}
	};

	DWORD			m_dwBeginTime,m_dwEndTime,m_dwTime,m_dwCounter;
	LONGLONG		m_llUseTime;		//사용한 시간.
	int				m_iNumbers;
	CItem			m_aProfiles[dMAX_PROFILE];
	cTIMER			m_timer;

					CProfiler();
					
	void			begin();	//	처음..
	void			end();		//	마지막(반드시..)

	void			begin(char *name);
	void			end(char *name);
	void			build();

	char*			getAverageTime(int index);
	char*			getTime(int index);
	char*			getTotalTime(int index);
	char*			getPercent(int index);
	char*			getName(int index) { return m_aProfiles[index].m_strName;}
	int				getHitCount(int index);
};



#endif