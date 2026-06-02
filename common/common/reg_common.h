#pragma	once

#include <winsock2.h>
#include <windows.h>
#include "server_type_define.h"

extern	LPCTSTR		lpszRegistry;
extern	LPCTSTR		lpszTestRegistry;
extern	LPCTSTR		lpszBalanceRegistry;
extern	LPCTSTR		lpszBattleRegistry;

extern	LPCTSTR		lpszRegistryOld;
extern	LPCTSTR		lpszTestRegistryOld;
extern	LPCTSTR		lpszBalanceRegistryOld;
extern	LPCTSTR		lpszBattleRegistryOld;

extern	char*		g_alpszServerVersionRegName[dSERVER_TYPE_COUNT];

extern	BOOL		ReadServerEXENameWithRegistry(int _iServerType,char *_lpstrBuffer,int _iBufferSize);
extern	BOOL		ReadServerPathWithRegistry(int _iServerType,char *_lpstrBuffer,int _iBufferSize);

extern	BOOL		WriteServerInfoToRegistry(int _iServerType,char *_lpstrName);	//	서버 정보 모두 저장
extern	int			ReadServerVersionInRegistry(int _iServerType);	//	특정 서버의 버젼 정보 읽기
extern	void		ReadAllServerVersionInRegistry();		//	모든 서버의 버젼 정보 읽기

extern	BOOL		WriteServerNameToRegistry(int _iServerType,char *_lpstrName);		//	서버 이름 저장
extern	BOOL		ReadServerNameWithRegistry(int _iServerType,char *_lpstrName,int _iBufferSize);	//	서버 이름 읽기

extern	BOOL		WriteServerStatusToRegistry(int _iServerType,int _iStatus);	//	서버 상태 저장
extern	int			ReadServerStatusWithRegistry(int _iServerType);				//	서버 상태 읽기

extern	BOOL		WriteWorldServerIPToRegistry(int _iServerType,char *_lpstrIP);	//	월드 아이피 저장
extern	BOOL		ReadWorldServerIPWithRegistry(int _iServerType,char *_lpstrName,int _iBufferSize);	//	월드 아이피 읽기

extern	BOOL		WriteWorldServerIndexToRegistry(int _iServerType,int _iIndex);	//	월드 인덱스 저장
extern	int			ReadWorldServerIndexWithRegistry(int _iServerType);	//	월드 인덱스 읽기

extern	BOOL		WriteGameServerIndexToRegistry(int _iServerType,int _iIndex);	//	게임 서버 인덱스 저장
extern	int			ReadGameServerIndexWithRegistry(int _iServerType);	//	게임 서버 인덱스 읽기

extern	BOOL		WriteLifeSignToRegistry(int _iServerType,DWORD _dwSign);	//	라이프 사인 기록
extern	int			ReadLifeSignWithRegistry(int _iServerType);	//	라이프 사인 읽기

extern	BOOL		WriteAgentSignToRegistry(int _iServerType,DWORD _dwSign);	//	에이전트 사인 기록
extern	int			ReadAgentSignWithRegistry(int _iServerType);	//	에이전트 사인 읽기

enum
{
	eAGENT_SIGN_EMPTY,
	eAGENT_SIGN_END_SERVER,
	eAGENT_SIGN_BEGIN_BY_AGENT,
};

extern	HKEY		g_hRegKey;
extern	char		g_strRegAddress[256];