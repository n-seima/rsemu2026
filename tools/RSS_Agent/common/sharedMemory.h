#pragma	once

#include <windows.h>

class	CSharedMemory
{
	HANDLE		m_hMemMap,m_hMutex;
	DWORD		m_dwBufferSize;
	BYTE		*m_pBuffer;
	BOOL		m_bIsProcess;

public:
				CSharedMemory();

	void		close();
	BOOL		create(char *_lpstrName,DWORD _dwBufferSize);
	BOOL		open(char *_lpstrName);

	BOOL		beginProcess();
	BOOL		endProcess();
	BYTE*		getBuffer()	{return	m_pBuffer;}

};