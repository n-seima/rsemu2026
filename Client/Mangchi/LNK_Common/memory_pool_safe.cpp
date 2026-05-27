#ifdef	dUSE_SAFE_MEMORY

#include	"safeMemoryForDebug.h"

//
//	메모리 할당
void*
cMemoryPool::alloc(u4 _u4Size,char *_lpstrName,s4 _s4StackMethod)
{
	if	(m_lpReference)
		return	m_lpReference->alloc(_u4Size,_lpstrName,m_s4StackMethod);

	_u4Size	+=	c_s4SafeMemoryNameLength+4+4+4+4;

	if	((_u4Size%eMemoryAllign)	!=	0)
		_u4Size	+=	(eMemoryAllign-(_u4Size%eMemoryAllign));

	if	(_s4StackMethod	==	eStackRear)
	{
		if	(m_u4FrontPointer+_u4Size	>= m_u4RearPointer)
		{
			g_ecm.addError("cMemoryPool::alloc Memory allocation Error(Rear)!! [%s:%s]",m_strName,_lpstrName);

			return NULL;
		}

		m_u4RearPointer					-=	_u4Size;

		::PrintSafeMemoryRegistCode(m_pPool+m_u4RearPointer,_u4Size,_lpstrName,eClientHead,eClientTail);

		return (void *)(m_pPool+m_u4RearPointer+c_s4SafeMemoryUsePos);
	}

	if	(m_u4TempAlloc	!=	0xffffffff)
	{
		g_ecm.addError("cMemoryPool::alloc - There are Exist Template Memory [%s:%s]",m_strName,_lpstrName);

		return NULL;
	}

	if	((m_u4FrontPointer%eMemoryAllign)	!=	0)
		m_u4FrontPointer	+=	eMemoryAllign	-	(m_u4FrontPointer%eMemoryAllign);

	if	(m_u4FrontPointer+_u4Size	>=	m_u4RearPointer)
	{
		g_ecm.addError("cMemoryPool::alloc - Memory allocation Error(Front) [%s:%s]",m_strName,_lpstrName);

		return NULL;
	}

	::PrintSafeMemoryRegistCode(m_pPool+m_u4FrontPointer,_u4Size,_lpstrName,eClientHead,eClientTail);

	m_u4FrontPointer		+=	_u4Size;

	return (void *)(m_pPool+m_u4FrontPointer-_u4Size+c_s4SafeMemoryUsePos);
}

//
//	메모리 임시 할당(이동안 아무것도 못한다.)
void*
cMemoryPool::tempAlloc(u4 _u4Size,char *_lpstrName)
{
	if	(m_lpReference)
		return	NULL;

	if	(m_u4TempAlloc	!=	0xffffffff)
	{
		g_ecm.addError("cMemoryPool::tempAlloc - There are Already Exist Template Memory[%s:%s]",m_strName,_lpstrName);

		return NULL;
	}

	void *mem	=	alloc(_u4Size,_lpstrName);

	if	(!mem)
		return NULL;

	m_u4TempAlloc	=	m_u4FrontPointer;

	return mem;
}

//
//	임시로 할당된 메모리 해제
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
	u4		u4Head,u4Tail,u4TailPos;

	char *lpPtr	=	((char*)_lpPtr);

	u4Head		=	*(u4 *)(lpPtr+c_s4SafeMemoryHeadPos	);		//	헤더
	u4TailPos	=	*(u4 *)(lpPtr+c_s4SafeMemoryTailPosPos);		//	꼬리 위치
	u4Tail		=	*(u4 *)(lpPtr+u4TailPos				);		//	꼬리

	if	(u4Head	!= eClientHead)
	{	
		g_ecm.addError(eEC_Memory_DamageSafeMemoryHeader,"damaged mcu safe memory header : '%s'",(char *)(lpPtr+c_s4SafeMemoryNamePos));

		return	false;
	}

	if	(u4Tail	!=	eClientTail)
	{
		g_ecm.addError(eEC_Memory_DamageSafeMemoryTail,"damaged mcu safe memory tail : '%s'",(char *)(lpPtr+c_s4SafeMemoryNamePos));
		return	false;
	}

	return	true;
}

#endif