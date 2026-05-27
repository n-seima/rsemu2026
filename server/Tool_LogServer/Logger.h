#ifndef  __Logger_H
#define  __Logger_H

#include <Windows.h>
#include <stdio.h>

#define dLOGGER_FILE_LENGTH			4096000
#define dLOGGER_DAY_CHECK_COUNT		20

class Logger
{
protected:
			FILE*		fp;
			char		fn[512];
			int		day,time,testCount;

public:
			char	strPath[512];
			CRITICAL_SECTION	cs;

			Logger();
			Logger( LPCTSTR mainPath);

	virtual	~Logger();

	BOOL	SetFileHandle();
	BOOL	MakeFile();
	BOOL	initialize( LPCTSTR mainPath);
	void	close();

	void	write(char * data);
};

#endif	//	__Logger_H
