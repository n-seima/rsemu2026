#include	"mem.h"

#ifndef	dUSE_SAFE_MEMORY

//
//	메모리 할당
void*
cMemoryPool::alloc(u4 _dwSize,int _iStackMethod)
{
	if	(m_lpReference)
		return	m_lpReference->alloc(_dwSize,m_s4StackMethod);

	if	((_dwSize%cMemoryPool::eMemoryAllign)	!=0	)
		_dwSize+=	(cMemoryPool::eMemoryAllign-(_dwSize%cMemoryPool::eMemoryAllign));	//	메모리 정렬

	_dwSize	+=	4;		//	MCU에서 할당 받은 포인터라는 낙인찍을 공간.

	if	(_iStackMethod	==	eStackRear)
	{
		if	(m_u4FrontPointer+_dwSize >= m_u4RearPointer)
		{
			g_ecm.addError("cMemoryPool::alloc Memory allocation Error(Rear)!! [%s]",m_strName);

			return NULL;
		}

		m_u4RearPointer		-=	_dwSize;

		::PrintSafeMemoryRegistCode(m_pPool+m_u4RearPointer,cMemoryPool::eClientHead);

		return (void *)(m_pPool+m_u4RearPointer+c_iSafeMemoryUsePos);
	}

	if	(m_u4TempAlloc!=0xffffffff)
	{
		g_ecm.addError("cMemoryPool::alloc - There are Exist Template Memory [%s]",m_strName);

		return NULL;
	}

	if	((m_u4FrontPointer%cMemoryPool::eMemoryAllign)	!=0)	
		m_u4FrontPointer	+=	(cMemoryPool::eMemoryAllign-(m_u4FrontPointer%cMemoryPool::eMemoryAllign));

	if	(m_u4FrontPointer+_dwSize >= m_u4FullSize)
	{
		g_ecm.addError("cMemoryPool::alloc - Memory allocation Error(Front) [%s]",m_strName);

		return NULL;
	}

	::PrintSafeMemoryRegistCode(m_pPool+m_u4FrontPointer,cMemoryPool::eClientHead);

	m_u4FrontPointer	+=	_dwSize;

	return (void *)(m_pPool+m_u4FrontPointer-_dwSize	+	c_iSafeMemoryUsePos);
}

//
//
void*
cMemoryPool::tempAlloc(u4 _dwSize)
{
	if	(m_lpReference)
		return	NULL;

	if (m_u4TempAlloc	!=	0xffffffff)
	{
		g_ecm.addError("cMemoryPool::tempAlloc - There are Already Exist Template Memory [%s]",m_strName);
		
		return NULL;
	}

	void *mem	=	alloc(_dwSize);

	if	(!mem)
		return NULL;

	m_u4TempAlloc	=	m_u4FrontPointer;

	return mem;
}

//
//	
void
cMemoryPool::tempFree()
{	
	if	(m_u4TempAlloc	==	0xffffffff)
		return;

	m_u4FrontPointer=	m_u4TempAlloc;
	m_u4TempAlloc	=	0xffffffff;
}

bool
cMemoryPool::checkMemory(void *_lpPtr)
{
	u4		dwHead;

	char *lpPtr	=	((char*)_lpPtr);

	dwHead		=	*(u4 *)(lpPtr	);		//	헤더

	if	(dwHead	!= cMemoryPool::eClientHead)
	{	
		g_ecm.addError(eEC_Memory_DamageSafeMemoryHeader,"damaged mcu safe memory header [%s]",m_strName);

		return	false;
	}

	return	true;
}


#endif