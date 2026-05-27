#pragma	once

#include <windows.h>

extern "C" {

#ifdef _GSDLL_EXPORTS
#define GSDLL_API __declspec(dllexport)
#else
#define GSDLL_API __declspec(dllimport)
#endif

GSDLL_API char*		GetHeroFileName(int _iIndex);
GSDLL_API char*		GetMonsterFileName(int _iIndex);
GSDLL_API char*		GetNpcFileName(int _iIndex);
GSDLL_API int		IsActionTime(unsigned int _dwBookedTime,unsigned int _dwCurrnetTime);
GSDLL_API int		SetCurrentFolder(char *_lpstrDestFolder);
GSDLL_API int		GetCurrentFolder(int _iSize,char *_lpstrResult);
GSDLL_API void		InitCS(void *_lpData);
GSDLL_API int		RemoveAS(int _iSerial,int *_lpiRookie,unsigned short *_lpwSerial,int *_lpiASCount);
}

typedef		char*	(*GetHeroFileNameProc)(int);
typedef		char*	(*GetMonsterFileNameProc)(int);
typedef		char*	(*GetNpcFileNameProc)(int);
typedef		int		(*IsActionTimeProc)(unsigned int,unsigned int );
typedef		int		(*SetCurrentFolderProc)(char *);
typedef		int		(*GetCurrentFolderProc)(int,char*);
typedef		void	(*InitCSProc)(void *);
typedef		int		(*RemoveASProc)(int,int*,unsigned short*,int*);

extern	GetHeroFileNameProc		FPGetHeroFileName;
extern	GetMonsterFileNameProc	FPGetMonsterFileName;
extern	GetNpcFileNameProc		FPGetNpcFileName;
extern	IsActionTimeProc		FPIsActionTime;
extern	SetCurrentFolderProc	FPSetCurrentFolder;
extern	GetCurrentFolderProc	FPGetCurrentFolder;
extern	InitCSProc				FPInitCS;
extern	RemoveASProc			FPRemoveAS;