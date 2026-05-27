#include "CSharedMemory.h"


CSharedMemory::CSharedMemory()
{
	m_hMemMap	=	NULL;
	m_hMutex	=	NULL;
	m_pBuffer	=	NULL;
	m_bIsProcess=	FALSE;
}	//	CSharedMemory::CSharedMemory()


BOOL
CSharedMemory::create(char *_lpstrName,DWORD _dwBufferSize)
{
	m_dwBufferSize	=	_dwBufferSize;

	char	strName[128];

	sprintf(strName,"SMMap_%s",_lpstrName);	//	Shared Memory Map

	m_hMemMap	=	::CreateFileMapping(INVALID_HANDLE_VALUE,	// 파일 맵의 핸들, 초기에 INVALID_HANDLE_VALUE를 설정한다.
										NULL,					// 보안 속성
										PAGE_READWRITE,			// 접근 속성
										0,						// 64비트 어드레스를 사용한다. 상위 32비트 - 메모리의 크기
										_dwBufferSize,			// 하위 32비트 - 여기선LPBYTE 타입.
										strName
										);

	if (!m_hMemMap)
		return	FALSE;

	m_pBuffer	=	(LPBYTE)::MapViewOfFile(m_hMemMap,FILE_MAP_ALL_ACCESS,0, 0, 0 );

	if (!m_pBuffer)
		return	FALSE;

	sprintf(strName,"SMMutex_%s",_lpstrName);	//	Shared Memory Mutex

	m_hMutex	=	::CreateMutex(NULL, FALSE,strName);

	if ( NULL == m_hMutex )
		return	FALSE;

	return	TRUE;
}	//	CSharedMemory::create(char *_lpstrName,DWORD _dwBufferSize,DWORD _dwAccessLevel)

void
CSharedMemory::close()
{
	if	(m_pBuffer)
	{
		::UnmapViewOfFile(m_pBuffer);

        m_pBuffer	=	NULL;
	}

	if	(m_hMemMap)
	{
        ::CloseHandle(m_hMemMap);

        m_hMemMap	=	NULL;
	}
}

BOOL
CSharedMemory::open(char *_lpstrName)
{
	char	strName[128];

    sprintf(strName, "SMMap_%s",_lpstrName);

// open the file mapping
    m_hMemMap	=	::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,strName);

    if	(!m_hMemMap)
		return FALSE;

// get the map buffer
    m_pBuffer	=	(LPBYTE)::MapViewOfFile(m_hMemMap,FILE_MAP_ALL_ACCESS, 0, 0, 0 );

// if getting the map buffer is failed

    if	(!m_pBuffer)
	{
		::CloseHandle(m_hMemMap);
		m_hMemMap	=	NULL;

        return FALSE;
    }

// get the handle of mutex
    sprintf(strName,"SMMutex_%s",_lpstrName);

	m_hMutex	=	::OpenMutex(MUTEX_ALL_ACCESS, FALSE,strName);

// can't open the mutex
    if(!m_hMutex)
	{
		::UnmapViewOfFile(m_pBuffer);

        m_pBuffer	=	NULL;

        ::CloseHandle(m_hMemMap);

        m_hMemMap	=	NULL;

		return	FALSE;
    }

	return	TRUE;
}	//	CSharedMemory::open(char *_lpstrName)

BOOL
CSharedMemory::beginProcess()
{
	return	TRUE;

	DWORD	dwWaitFuncRes	=	::WaitForSingleObject(m_hMutex, INFINITE );
//	ERROR_WAIT_NO_CHILDREN
	if	(WAIT_OBJECT_0		==	dwWaitFuncRes)
	{
		m_bIsProcess	=	TRUE;

		return	TRUE;
	}

	return	FALSE;
}	//	CSharedMemory::beginProcess()

BOOL
CSharedMemory::endProcess()
{
	return	TRUE;

	if (FALSE	==	m_bIsProcess)	return	FALSE;

	::ReleaseMutex(m_hMutex);

	m_bIsProcess	=	FALSE;
	
	return	TRUE;
}	//	CSharedMemory::endProcess()