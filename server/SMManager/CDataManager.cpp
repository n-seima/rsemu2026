#include "CDataManager.h"

CDataManager	g_dsm;

BOOL
CDataManager::init()
{
	if (!m_smClient.create("UDB ClientData",sizeof(CClientDataManager)+1024*10))		return	FALSE;	//	클라이언트 정보 메모리 메니져 + 10k정도의 여유
//	if (!m_smAvatarInfo.create("UDB AvatarData",sizeof(CPlayerBuffManager)+1024*10))	return	FALSE;	//	아마타 정보 메모리 메니져 + 10k정도의 여유

	g_lpCdm	=	(CClientDataManager*)m_smClient.getBuffer();
//	g_lpAbm	=	(CPlayerBuffManager*)m_smAvatarInfo.getBuffer();

	g_lpCdm->Init();
//	g_lpAbm->initForSharedMemoryManager();

	return	TRUE;
}	//	CDataManager::init()