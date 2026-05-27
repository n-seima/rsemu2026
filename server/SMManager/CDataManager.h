#ifndef _classDataManager_h
#define	_classDataManager_h

#include	"CSharedMemory.h"
#include	"../RedStone_DBServer/Client.h"
#include	"../RedStone_DBServer/CPlayerBuff.h"

class	CDataManager
{
public:
	CSharedMemory	m_smClient;//,m_smAvatarInfo;

	BOOL			init();

	inline	BOOL	lockCM()	{return m_smClient.beginProcess();}
	inline	BOOL	unlockCM()	{return m_smClient.endProcess();}
//	inline	BOOL	lockAI()	{return m_smAvatarInfo.beginProcess();}
//	inline	BOOL	unlockAI()	{return m_smAvatarInfo.endProcess();}
};

extern	CDataManager	g_dsm;

#endif