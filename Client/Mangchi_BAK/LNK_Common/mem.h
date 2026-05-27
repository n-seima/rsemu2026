//
//	제작 일시	2005.11.31
//
//	목적		메모리 관련 코드들
//
//	제작		이영찬
//
//	1. dUSE_SAFE_MEMORY 를 설정한 경우 디버그 레벨 세이프 메모리 코드가 작동한다.
//	2. dUSE_SAFE_MEMORY는 프로젝젝트에서 설정하도록 한다.
//	3. 기본적으로 디버그 모드에서는 dUSE_SAFE_MEMORY가 설정되게 되어 있다.
//	4. 디버그 모드가 아닌 경우에도 작동하게 하고 싶으면 프로젝트 설정에서 dUSE_SAFE_MEMORY를 추가 시킬것

#ifndef	_lnk_common_memory_mananger_h
#define	_lnk_common_memory_mananger_h

#include	"lnk_define.h"
#include	"memory_pool.h"
#include	"smart_pointer.h"

#ifdef	dUSE_SAFE_MEMORY
#include	"safeMemoryForDebug.h"
#else
#include	"safeMemory.h"
#endif

#endif