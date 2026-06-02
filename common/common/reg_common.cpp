#include "reg_common.h"
#include "ATLBASE.h"
#include "SFC.h"
#include "version.h"
#include "server_type_define.h"

#ifdef	_FOR_USA
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for USA";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for USA";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for USA";
LPCTSTR		lpszBattleRegistry	=	"software\\L&K Logic Korea\\Red Stone(Battle)";
#elif	_FOR_INTERNATIONAL
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for International";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for International";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for International";
LPCTSTR		lpszBattleRegistry	=	"software\\L&K Logic Korea\\Red Stone(Battle)";
#elif	_FOR_THAI
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for Thai";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for Thai";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for Thai";
LPCTSTR		lpszBattleRegistry	=	"software\\L&K Logic Korea\\Red Stone(Battle)";
#elif	_FOR_JAPAN
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone for Japan";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test) for Japan";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance) for Japan";
LPCTSTR		lpszBattleRegistry	=	"software\\L&K Logic Korea\\Red Stone(Battle) for Japan";

LPCTSTR		lpszRegistryOld		=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR		lpszTestRegistryOld	=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR		lpszBalanceRegistryOld=	"software\\L&K Logic Korea\\Red Stone(balance)";
LPCTSTR		lpszBattleRegistryOld=	"software\\L&K Logic Korea\\Red Stone(Battle)";
#else
LPCTSTR		lpszRegistry		=	"software\\L&K Logic Korea\\Red Stone";
LPCTSTR		lpszTestRegistry	=	"software\\L&K Logic Korea\\Red Stone(test)";
LPCTSTR		lpszBalanceRegistry	=	"software\\L&K Logic Korea\\Red Stone(balance)";
LPCTSTR		lpszBattleRegistry	=	"software\\L&K Logic Korea\\Red Stone(Battle)";
#endif

HKEY		g_hRegKey	=	HKEY_CURRENT_USER;

char		g_strRegAddress[256]		=	"";

inline	char*
GetServerRegistryName()
{
	if	(g_strRegAddress[0]	==	NULL)
		sprintf(g_strRegAddress,"%s\\server",lpszRegistry);

	return	g_strRegAddress;
}

inline	char*
GetServerLifeSignRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_life_sign",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetServerAgentSignRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_agent_sign",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetServerVersionRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_version",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetServerStatusRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_status",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetServerPathRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_path",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetWorldServerIPRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_WorldIP",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetWorldServerIndexRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_WorldIndex",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetGameServerIndexRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_GameIndex",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetServerExeFileRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_exe",g_strServerName[_iServerType]);

	return	s_strName;
}

inline	char*
GetServerNameRegistryName(int _iServerType)
{
	static	char	s_strName[512];

	sprintf(s_strName,"%s_name",g_strServerName[_iServerType]);

	return	s_strName;
}

//	서버 레지스트리 생성
BOOL
MakeServerRegistry(CRegKey	*_lpReg)
{
	if	(_lpReg->Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
	{
		_lpReg->Create(g_hRegKey,GetServerRegistryName());

		if	(_lpReg->Open( g_hRegKey, GetServerRegistryName()) != ERROR_SUCCESS)
		{
			cMSG::Error("error","can not write registry");

			return	FALSE;
		}
	}

	return	TRUE;
}

BOOL
WriteServerNameToRegistry(int _iServerType,char *_lpstrName)
{
	CRegKey	reg;

	if	(!MakeServerRegistry(&reg))
		return	FALSE;

	reg.SetValue(_lpstrName,GetServerNameRegistryName(_iServerType));

	return	TRUE;
}

BOOL
ReadServerNameWithRegistry(int _iServerType,char *_lpstrName,int _iBufferSize)
{
	CRegKey	reg;
	DWORD	dwCount	=	_iBufferSize;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
		return	FALSE;

	if	(reg.QueryValue((LPTSTR )_lpstrName,GetServerNameRegistryName(_iServerType),&dwCount) != ERROR_SUCCESS)
	{
		_lpstrName	=	"";

		return	FALSE;
	}

	return	TRUE;
}

BOOL
ReadWorldServerIPWithRegistry(int _iServerType,char *_lpstrName,int _iBufferSize)
{
	strcpy(_lpstrName,"");

	CRegKey	reg;
	DWORD	dwCount	=	_iBufferSize;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
		return	FALSE;

	if	(reg.QueryValue((LPTSTR )_lpstrName,GetWorldServerIPRegistryName(_iServerType),&dwCount) != ERROR_SUCCESS)
		return	FALSE;

	return	TRUE;
}

BOOL
WriteWorldServerIPToRegistry(int _iServerType,char *_lpstrIP)
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
	{
		reg.Create(g_hRegKey,GetServerRegistryName());

		if	(reg.Open( g_hRegKey, GetServerRegistryName()) != ERROR_SUCCESS)
		{
			cMSG::Error("error","can not write registry");

			return	FALSE;
		}
	}

	reg.SetValue(_lpstrIP,GetWorldServerIPRegistryName(_iServerType));

	return	TRUE;
}

int
ReadWorldServerIndexWithRegistry(int _iServerType)	//	월드 인덱스 읽기
{
	CRegKey	reg;
	DWORD	dwValue;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
		return	-1;

	if	(reg.QueryValue(dwValue,GetWorldServerIndexRegistryName(_iServerType)) != ERROR_SUCCESS)
		return	-1;

	reg.Close();

	return	(int)dwValue;
}
BOOL
WriteWorldServerIndexToRegistry(int _iServerType,int _iIndex)	//	월드 인덱스 저장
{
	CRegKey	reg;

	if	(!MakeServerRegistry(&reg))
		return	FALSE;

	reg.SetValue(_iIndex,GetWorldServerIndexRegistryName(_iServerType));

	return	TRUE;
}

int
ReadGameServerIndexWithRegistry(int _iServerType)	//	게임 서버 인덱스 읽기
{
	CRegKey	reg;
	DWORD	dwValue;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
		return	-1;

	if	(reg.QueryValue(dwValue,GetGameServerIndexRegistryName(_iServerType)) != ERROR_SUCCESS)
		return	-1;

	reg.Close();

	return	(int)dwValue;
}
BOOL
WriteGameServerIndexToRegistry(int _iServerType,int _iIndex)	//	게임 서버 인덱스 저장
{
	CRegKey	reg;

	if	(!MakeServerRegistry(&reg))
		return	FALSE;

	reg.SetValue(_iIndex,GetGameServerIndexRegistryName(_iServerType));

	return	TRUE;
}

BOOL
WriteServerInfoToRegistry(int _iServerType,int _iVersion,char *_lpstrName)
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
	{
		reg.Create(g_hRegKey,GetServerRegistryName());

		if	(reg.Open( g_hRegKey, GetServerRegistryName()) != ERROR_SUCCESS)
		{
			cMSG::Error("error","can not write registry");

			return	FALSE;
		}
	}

	reg.SetValue(_iVersion,GetServerVersionRegistryName(_iServerType));

	WriteServerStatusToRegistry(_iServerType,eSERVER_STATUS_BOOTING);

	{
		char	strCommandLine[512],strFileName[512],strFolderName[512];
		int		iLoc	=	0;

		{
			char	strText[512];

			strcpy(strText,GetCommandLine());

			BOOL	bIsFindStream	=	FALSE;
			int		iPos			=	0;

			for (int i=0;strText[i];i++)
			{
				if	(strText[i]	==	'"')
				{
					if	(bIsFindStream)
					{
						strCommandLine[iPos++]	=	NULL;

						break;
					}

					bIsFindStream	=	TRUE;
					continue;
				}

				if	(bIsFindStream)
					strCommandLine[iPos++]	=	strText[i];
			}

			if	(iPos	==	0)
				strcpy(strCommandLine,strText);
		}

		strcpy(strFileName,_exportFileName(strCommandLine));
		strcpy(strFolderName,_exportFolderName(strCommandLine));

		reg.SetValue(strFolderName,GetServerPathRegistryName(_iServerType));
		reg.SetValue(strFileName,GetServerExeFileRegistryName(_iServerType));
	}

	reg.SetValue(_lpstrName,GetServerNameRegistryName(_iServerType));

	reg.SetValue(eSERVER_STATUS_BOOTING,GetServerStatusRegistryName(_iServerType));

	reg.Close();

	return	TRUE;
}

BOOL
WriteServerInfoToRegistry(int _iServerType,char *_lpstrName)
{
	return	WriteServerInfoToRegistry(_iServerType,g_aiServerVersion[_iServerType],_lpstrName);
}

BOOL
ReadServerPathWithRegistry(int _iServerType,char *_lpstrBuffer,int _iBufferSize)
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
		return	FALSE;

	DWORD	dwCount	=	_iBufferSize;

	if	(reg.QueryValue((LPTSTR )_lpstrBuffer,GetServerPathRegistryName(_iServerType),&dwCount) != ERROR_SUCCESS)
		return	FALSE;

	return	TRUE;
}

BOOL
ReadServerEXENameWithRegistry(int _iServerType,char *_lpstrBuffer,int _iBufferSize)
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
		return	FALSE;

	DWORD	dwCount	=	_iBufferSize;

	if	(reg.QueryValue((LPTSTR )_lpstrBuffer,GetServerExeFileRegistryName(_iServerType),&dwCount) != ERROR_SUCCESS)
		return	FALSE;

	return	TRUE;
}

int
ReadServerStatusWithRegistry(int _iServerType)
{
	CRegKey	reg;
	DWORD	dwStatus;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
		return	eSERVER_STATUS_OFF;

	if	(reg.QueryValue(dwStatus,GetServerStatusRegistryName(_iServerType)) != ERROR_SUCCESS)
		return	eSERVER_STATUS_OFF;

	reg.Close();

	return	(int)dwStatus;
}

BOOL
WriteServerStatusToRegistry(int _iServerType,int _iStatus)
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) != ERROR_SUCCESS)
	{
		reg.Create(g_hRegKey,GetServerRegistryName());

		if	(reg.Open( g_hRegKey, GetServerRegistryName()) != ERROR_SUCCESS)
		{
			cMSG::Error("error","can not write registry");

			return	FALSE;
		}
	}

	reg.SetValue(_iStatus,GetServerStatusRegistryName(_iServerType));

	reg.Close();

	return	TRUE;
}

int
ReadServerVersionInRegistry(int _iServerType)
{
	CRegKey	reg;

	g_adwCurrentServerVersion[_iServerType]	=	0;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) == ERROR_SUCCESS)
	{
		if	(reg.QueryValue(g_adwCurrentServerVersion[_iServerType],GetServerVersionRegistryName(_iServerType)) != ERROR_SUCCESS)
			g_adwCurrentServerVersion[_iServerType]	=	0;
	}

	return	g_adwCurrentServerVersion[_iServerType];
}

void
ReadAllServerVersionInRegistry()
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) == ERROR_SUCCESS)
	{
		for (int i=0;i<dSERVER_TYPE_COUNT;i++)
		{
			if	(reg.QueryValue(g_adwCurrentServerVersion[i],GetServerVersionRegistryName(i)) != ERROR_SUCCESS)
				g_adwCurrentServerVersion[i]	=	0;
		}
	}
}

BOOL
WriteLifeSignToRegistry(int _iServerType,DWORD _dwSign)	//	라이프 사인 기록
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) == ERROR_SUCCESS)
		reg.SetValue(_dwSign,GetServerLifeSignRegistryName(_iServerType));

	return	TRUE;
}

int
ReadLifeSignWithRegistry(int _iServerType)	//	라이프 사인 읽기
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) == ERROR_SUCCESS)
	{
		DWORD	dwSign;

		if	(reg.QueryValue(dwSign,GetServerLifeSignRegistryName(_iServerType)) == ERROR_SUCCESS)
			return	dwSign;
	}

	return	0;
}

BOOL
WriteAgentSignToRegistry(int _iServerType,DWORD _dwSign)	//	에이전트 사인 기록
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) == ERROR_SUCCESS)
		reg.SetValue(_dwSign,GetServerAgentSignRegistryName(_iServerType));

	return	TRUE;
}

int
ReadAgentSignWithRegistry(int _iServerType)	//	에이전트 사인 읽기
{
	CRegKey	reg;

	if	(reg.Open( g_hRegKey,GetServerRegistryName()) == ERROR_SUCCESS)
	{
		DWORD	dwSign;

		if	(reg.QueryValue(dwSign,GetServerAgentSignRegistryName(_iServerType)) == ERROR_SUCCESS)
			return	dwSign;
	}

	return	0;
}