#include	"lnk_define.h"

#ifndef	dUSE_SAFE_MEMORY

#include	"safeMemory.h"

cSafeMemoryManager	g_smm;	//	safe memory manager

bool
cSafeMemoryManager::check()
{
	return	true;
}

#endif