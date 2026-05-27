#include	"lnk_define.h"

#ifdef	dUSE_SAFE_MEMORY

#include	"safeMemoryForDebug.h"

cSafeMemoryManager	g_smm;	//	safe memory manager

const	u4	c_dwDefaultMaxSafeMemoryClientCount	=	10000;	//	세이프 메모리 관리자에 등록 가능한 최대 수

cSafeMemoryManager::cSafeMemoryManager()
{
	m_pList	=	new	cSafeMemoryItem [c_dwDefaultMaxSafeMemoryClientCount];

	m_dwMaxRegistMemoryCount=	c_dwDefaultMaxSafeMemoryClientCount;
	m_dwRegistMemoryCount	=	0;
}

cSafeMemoryManager::~cSafeMemoryManager()
{
	pKILL(m_pList);
}

//
//	최대 메모리 클라이언트 수를 재설정 한다.
bool
cSafeMemoryManager::init(int _iMaxSafeMemoryClientCount)
{
	if	(m_dwRegistMemoryCount)
	{
		g_ecm.addError("cSafeMemoryManager::init - already exist client");

		return	false;
	}

	pKILL(m_pList);

	m_pList					=	new	cSafeMemoryItem [_iMaxSafeMemoryClientCount];
	m_dwMaxRegistMemoryCount=	_iMaxSafeMemoryClientCount;
	m_dwRegistMemoryCount	=	0;

	return	true;
}

//	새로운 메모리 등록
u4
cSafeMemoryManager::add(void *_lpClient)
{
	if	(m_dwRegistMemoryCount	>=	m_dwMaxRegistMemoryCount)
	{
		g_ecm.addError("cSafeMemoryManager::add - m_dwRegistMemoryCount >= m_dwMaxRegistMemoryCount");

		return	false;
	}

	m_pList[m_dwRegistMemoryCount].m_lpReferencePoint	=	_lpClient;

	m_dwRegistMemoryCount++;

	return	m_dwRegistMemoryCount-1;
}

//	메모리가 깨진게 아닌지 검사
inline bool
cSafeMemoryManager::_inCheckMemory(int _dwIndex)
{
	return	::CheckSafeMemory(m_pList[_dwIndex].m_lpReferencePoint);
}

//	등록된 메모리 제거
bool
cSafeMemoryManager::remove(u4 _dwIndex)
{
	if	(_dwIndex	>=	m_dwRegistMemoryCount)
	{
		g_ecm.addError("cSafeMemoryManager::remove - _dwIndex >= m_dwRegistMemoryCount");

		return	false;
	}

	if	(!_inCheckMemory(_dwIndex))
		return	false;

	if	(_dwIndex	<	m_dwRegistMemoryCount-1)	//	중간에 빈 자리가 없도록 마지막 클라이언트를 빈자리에 복사한다.
	{
		int		iLastSlot	=	m_dwRegistMemoryCount-1;
		char*	lpLastSlot	=	(char*)m_pList[iLastSlot].m_lpReferencePoint;	//	마지막 슬롯

		*(u4	*)(lpLastSlot+8)				=	_dwIndex;
		//	마지막 클라이언트 메모리에 SMM 인덱스 변경
		m_pList[_dwIndex].m_lpReferencePoint	=	m_pList[iLastSlot].m_lpReferencePoint;
		//	제거 하려는 메모리에 마지막 메모리 설정
		m_pList[iLastSlot].m_lpReferencePoint	=	NULL;
		//	마지막 메모리 초기화
	}
	else
		m_pList[_dwIndex].m_lpReferencePoint				=	NULL;

	m_dwRegistMemoryCount--;

	return	true;
}

//
//	등록된 클라이언트를 모두 검사해 문제가 없는지 확인한다.
bool
cSafeMemoryManager::check()
{
	for	(int iSlot=m_dwRegistMemoryCount;iSlot;)
	{
		--iSlot;

		if	(!_inCheckMemory(iSlot))
			return	false;
	}

	return	true;
}

#endif