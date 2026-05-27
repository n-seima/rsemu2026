#ifndef _classSharedMemory_h
#define _classSharedMemory_h

#include "SFC.H"

class	CSharedMemory
{
	HANDLE		m_hMemMap,m_hMutex;
	DWORD		m_dwBufferSize;
	BYTE		*m_pBuffer;
	BOOL		m_bIsProcess;

public:
				CSharedMemory();

	BOOL		create(char *_lpstrName,DWORD _dwBufferSize);
	BOOL		open(char *_lpstrName);
	void		close();

	BOOL		beginProcess();
	BOOL		endProcess();
	BYTE*		getBuffer()	{return	m_pBuffer;}

};

#endif