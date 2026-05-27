//
//	제작 일시	2005.11.30
//
//	목적		보다 안전한 메모리 관리를 가능하게 해준다.
//
//	제작		이영찬
//
//	
#ifndef	_lnk_common_safe_memory_for_debug_h
#define	_lnk_common_safe_memory_for_debug_h

#include	"lnk_define.h"

#ifdef	dUSE_SAFE_MEMORY

#include	"memory_pool.h"
#include	"error_code.h"
#include	<memory.h>
#include	<stdlib.h>

//	ex>
//	char *pBuffer	=	mNew(char,100,NULL,"테스트 메모리");
//	mFree(pBuffer);
#define	mNew(type,numbers,mcu,name)			(type *)::SafeMalloc((numbers)*sizeof(type),name,mcu)

//	세이프 메모리 매크로
//	char *pBuffer	=	(char *)g_smm.alloc(256);
//	g_smm.release(pBuffer);
//	여기 최대 문제는 생성자와 소멸자가 작동하지 않는 다는것.
#define	mFree(ptr)							if (::SafeFree(ptr)) ptr = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//	세이프 메모리
//	메모리를 등록한 후 해당 메모리에 문제가 없는지 체크가 가능하게 해준다.
//	디폴트로 최대 10000개의 메모리를 관리 할 수 있다.(메모리만 참조하기 때문에 개당 4바이트만 필요)
//
//	관리 메모리 개수 변경 g_smm.init(수치);
//
//	나머지는 mNew,mFree를 통해 사용하는것으로 직접 사용하지 않는게 좋다.

class	cSafeMemoryManager
{
protected:
	inline	bool	_inCheckMemory(s4 _dwIndex);	//	인덱스 번째 메모리의 무결성 검사

public:
	class	cSafeMemoryItem
	{
	public:
		void		*m_lpReferencePoint;
	};

	u4				m_dwMaxRegistMemoryCount;	//	등록된 가능한 메모리 수
	u4				m_dwRegistMemoryCount;		//	등록된 메모리 수
	s4				m_iMaxHitCount;				//	프로세스 돌면서 최대 몇개의 메모리가 생겼는지 체크해 준다.
	cSafeMemoryItem	*m_pList;	//	등록된 메모리 들

					cSafeMemoryManager();
	virtual			~cSafeMemoryManager();

	bool			init(s4 _iMaxSafeMemoryClientCount);
	bool			check();
	//	루프 안에 넣어서 주기적으로 체크해 준다.
	//	만약 등록된 메모리들에 문제가 생기면 에러 코드를 남기고 false 값을 리턴한다.

//
	u4				add(void *_lpClient);					//	새로운 메모리 등록
	bool			remove(u4 _dwIndex);					//	등록된 메모리 제거

};

extern	cSafeMemoryManager	g_smm;	//	safe memory manager

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const	u4		c_u4SafeMemoryPointHead		=	0xaf93ecd5;
const	u4		c_u4SafeMemoryPointTail		=	0xe3af5c9d;
const	s4		c_s4SafeMemoryNameLength		=	32;
const	s4		c_s4SafeMemoryNamePos		=	8;
const	s4		c_s4SafeMemorySMM_IndexPos	=	0;
const	s4		c_s4SafeMemoryTailPosPos		=	4;

const	s4		c_s4SafeMemoryUsePos			=	c_s4SafeMemoryNamePos+c_s4SafeMemoryNameLength+4;
const	s4		c_s4SafeMemoryHeadPos		=	c_s4SafeMemoryNamePos+c_s4SafeMemoryNameLength;
const	s4		c_s4SafeMemoryTailPos		=	c_s4SafeMemoryHeadPos+4;

inline	bool
CheckSafeMemory(void *_lpMemory)
{
	u4		dwHead,dwTail,dwTailPos;

	char *lpPtr	=	((char*)_lpMemory);

	dwHead		=	*(u4 *)(lpPtr+c_s4SafeMemoryHeadPos	);		//	헤더
	dwTailPos	=	*(u4 *)(lpPtr+c_s4SafeMemoryTailPosPos);		//	꼬리 위치
	dwTail		=	*(u4 *)(lpPtr+dwTailPos				);		//	꼬리

	if	(dwHead	!= c_u4SafeMemoryPointHead)
	{	
		if	(dwHead	==	cMemoryPool::eClientHead)	//	MCU 클라이언트다
			return	((cMemoryPool *)(lpPtr+4))->checkMemory(lpPtr);

		g_ecm.addError(eEC_Memory_DamageSafeMemoryHeader,"damaged safe memory header : '%s'",(char *)(lpPtr+c_s4SafeMemoryNamePos));

		return	false;
	}

	if	(dwTail	!=	c_u4SafeMemoryPointTail)
	{
		g_ecm.addError(eEC_Memory_DamageSafeMemoryTail,"damaged safe memory tail : '%s'",(char *)(lpPtr+c_s4SafeMemoryNamePos));
		return	false;
	}

	return	true;
}

inline	void
PrintSafeMemoryRegistCode(char *_lpPtr,u4 _dwSize,char *_lpstrName,u4 _dwHead,u4 _dwTail)
{
	u4	dwSMM_Index	=	g_smm.add(_lpPtr);

	*(u4 *)(_lpPtr+c_s4SafeMemorySMM_IndexPos	)	=	dwSMM_Index;	//	헤더
	*(u4 *)(_lpPtr+c_s4SafeMemoryTailPosPos		)	=	(_dwSize+12+c_s4SafeMemoryNameLength);					//	꼬리 위치
	*(u4 *)(_lpPtr+c_s4SafeMemoryHeadPos			)	=	_dwHead;		//	스마트 메모리 메니져 인덱스
	*(u4 *)(_lpPtr+c_s4SafeMemoryTailPos+_dwSize	)	=	_dwTail;		//	꼬리

	if	(_lpstrName)
	{
		memcpy(_lpPtr+c_s4SafeMemoryNamePos,_lpstrName,c_s4SafeMemoryNameLength);
		_lpPtr[c_s4SafeMemoryNamePos+c_s4SafeMemoryNameLength-1]			=	NULL;
	}
	else
	{
		memcpy(_lpPtr+c_s4SafeMemoryNamePos,"noname",c_s4SafeMemoryNameLength);
		_lpPtr[c_s4SafeMemoryNamePos+c_s4SafeMemoryNameLength-1]			=	NULL;
	}
}

inline	void*  
SafeMalloc(u4 _dwSize,char *_lpstrName,cMemoryPool *_lpMcu=NULL)
{
	if	(_lpMcu)
	{
		void	*ptr	=	_lpMcu->alloc(_dwSize,_lpstrName);

		return	ptr;
	}

	char	*ptr		=	new char [_dwSize+c_s4SafeMemoryNameLength+4+4+4+4];	//	size+name(32)+ smm index(4) + head(4) + tail pos(4) + tail(4)

	if	(!ptr)
		return	NULL;

	::PrintSafeMemoryRegistCode(ptr,_dwSize,_lpstrName,c_u4SafeMemoryPointHead,c_u4SafeMemoryPointTail);

	ptr	+=	c_s4SafeMemoryUsePos;

	return ptr;
}

//	메모리 해제
inline	bool
SafeFree(void *_lpPtr)
{
	if	(!_lpPtr)
		return	true;

	char	*lpPtr	=	((char *)_lpPtr)-c_s4SafeMemoryUsePos;

	if	(*(u4 *)(lpPtr+c_s4SafeMemoryHeadPos)	==	cMemoryPool::eClientHead)	//	MCU 클라이언트다
		return	false;

	u4	dwIndex	=	*(u4 *)(lpPtr+c_s4SafeMemorySMM_IndexPos);

	if	(dwIndex	!=	0xffffffff)
		g_smm.remove(*(u4 *)(lpPtr+c_s4SafeMemorySMM_IndexPos));
	else
		CheckSafeMemory(lpPtr);

	free(lpPtr);

	return	true;
}

#endif


#endif