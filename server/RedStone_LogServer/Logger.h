#ifndef  __Logger_H
#define  __Logger_H

#include <Windows.h>
#include <stdio.h>

#define dLOGGER_FILE_LENGTH			4096000
#define dLOGGER_DAY_CHECK_COUNT		20

class Logger
{
protected:
			FILE*		m_pFP;
			char		m_strLogFileName[512];
			int			day,time,testCount;
			int			m_iLastFP;
			
			BOOL		makeFile(char *_lpstrFileName);

public:
			char		m_strLogPath[512];
			CRITICAL_SECTION	cs;

			Logger();
			Logger( LPCTSTR mainPath);

	virtual	~Logger();

	BOOL	CreateFileHandle();
	BOOL	SetFileHandle();
	BOOL	MakeFile();
	BOOL	initialize( LPCTSTR mainPath);
	void	close();

	void	write(void *_lpData);
};

#endif	//	__Logger_H
