#ifndef	_backup_community_server_data_h
#define	_backup_community_server_data_h

#include "SFC.H"

class cBackupUserDataToFile
{
public:
	CRITICAL_SECTION	m_cs;

	cBackupUserDataToFile();
	~cBackupUserDataToFile();

	BOOL				save(void *_lpClient,BOOL _bIsSaved);
	BOOL				checkData(void *_lpReceiveInfo,char *_lpstrId,char *_lpstrName,int _iAvatarIndex);
};

extern	cBackupUserDataToFile	g_bud;	//	backup user data

#endif