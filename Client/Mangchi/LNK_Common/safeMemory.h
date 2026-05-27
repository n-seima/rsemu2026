//
//	제작 일시	2005.11.30
//
//	목적		보다 안전한 메모리 관리를 가능하게 해준다.
//
//	제작		이영찬
//
//	
//	디버그 모드가 아닌 경우에는 사실상 더미이다.

#ifndef	_lnk_common_safe_memory_h
#define	_lnk_common_safe_memory_h

#include	"lnk_define.h"
#include	"memory_pool.h"
#include	"error_code.h"
#include	<malloc.h>

#ifndef	dUSE_SAFE_MEMORY

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class	cSafeMemoryManager
{
public:
	bool			check();
};

extern	cSafeMemoryManager	g_smm;	//	safe memory manager

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	타입,개수,메모리 풀,이름
#define	mNew(type,numbers,mp,name)		(type *)SafeMalloc((numbers)*sizeof(type),mp)
#define	mFree(ptr)						SafeFree(ptr)

const	u4		c_dwSafeMemoryPointHead		=	0x4d912e3b;
const	u4		c_dwSafeMemoryPointTail		=	0xb492de13;
const	int		c_iSafeMemoryUsePos			=	4;

inline	void
PrintSafeMemoryRegistCode(char *_lpPtr,u4 _dwHead)
{
	*(u4 *)(_lpPtr	)	=	_dwHead;	//	MCU 클라이언트와 구분하기 위한 최소한의 장치
}

inline void*  
SafeMalloc(u4 _dwSize,cMemoryPool *_lpMcu=NULL)
{
	if	(_lpMcu)
	{	
		void	*ptr	=	_lpMcu->alloc(_dwSize);

		return	ptr;
	}

	char	*ptr	=	(char *)malloc(_dwSize+4);

	::PrintSafeMemoryRegistCode(ptr,c_dwSafeMemoryPointHead);

	return	ptr+c_iSafeMemoryUsePos;
}

//	메모리 해제
inline	bool	
SafeFree(void *_lpPtr)
{
	if	(!_lpPtr)
		return	true;

	char	*lpPtr	=	((char *)_lpPtr)-c_iSafeMemoryUsePos;

	if	(*(u4 *)lpPtr	==	cMemoryPool::eClientHead)	//	메모리 풀에서 설정된 것은 해제 해주면 안된다.
		return	true;

	if	(*(u4 *)lpPtr	!=	c_dwSafeMemoryPointHead)	//	메모리 풀도 아니고, 헤더도 맞지 않는다.
	{
		g_ecm.addError(eEC_Memory_DamageSafeMemoryHeader,"Damage Safe Memory Header");

		return	false;
	}

	free(lpPtr);
	
	_lpPtr	=	NULL;

	return	true;
}

#endif


#endif