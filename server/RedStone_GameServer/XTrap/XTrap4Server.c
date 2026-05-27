// ***************************************************************
//  X-Trap Interface Library For Server
//  -------------------------------------------------------------
//  Copyright (C)WiseLogic 2005 - All Rights Reserved
// ***************************************************************

// If "stdafx.h" is being used, "XTrap4Server.c" need to be renamed as "XTrap4Server.cpp"
//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32)
#include <io.h>
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>

#define _access access

#endif

#include "XTrap4Server.h"

PFN_XTrap_S_Start			XTrap_S_Start			= 0;
PFN_XTrap_S_SessionInit		XTrap_S_SessionInit		= 0;
PFN_XTrap_CS_Step1			XTrap_CS_Step1			= 0;
PFN_XTrap_CS_Step3			XTrap_CS_Step3			= 0;
PFN_XTrap_S_SessionReset	XTrap_S_SessionReset	= 0;
PFN_XTrap_S_SetModuleOn		XTrap_S_SetModuleOn		= 0;
PFN_XTrap_S_SetModuleOff	XTrap_S_SetModuleOff	= 0;
PFN_XTrap_S_SetOption		XTrap_S_SetOption		= 0;
PFN_XTrap_S_SaveLog			XTrap_S_SaveLog			= 0;
PFN_XTrap_CS_Loading		XTrap_CS_Loading		= 0;
PFN_XTrap_S_SendGamePacket	XTrap_S_SendGamePacket	= 0;
PFN_XTrap_S_RecvGamePacket	XTrap_S_RecvGamePacket	= 0; 

#if defined(_WIN32) || defined(WIN32)

HINSTANCE hXTrap4Server = 0;

//extern unsigned long	XTrap_S_FreeDll	();		// Dll 기본 해제함수 예제


unsigned long XTrap_S_LoadDll()
{
	unsigned long ulError = 0;

	char sDllBinFile[]	="XTrap4Server.Dll";

	hXTrap4Server = LoadLibrary(sDllBinFile);

	if (hXTrap4Server == NULL)
	{	
		ulError = GetLastError(); return 0x10000000|ulError; }

	XTrap_S_Start			= (PFN_XTrap_S_Start)			GetProcAddress(hXTrap4Server, "XTrap_S_Start");
	XTrap_S_SessionInit		= (PFN_XTrap_S_SessionInit)		GetProcAddress(hXTrap4Server, "XTrap_S_SessionInit");
	XTrap_CS_Step1			= (PFN_XTrap_CS_Step1)			GetProcAddress(hXTrap4Server, "XTrap_CS_Step1");
	XTrap_CS_Step3			= (PFN_XTrap_CS_Step3)			GetProcAddress(hXTrap4Server, "XTrap_CS_Step3");
	XTrap_S_SessionReset	= (PFN_XTrap_S_SessionReset)	GetProcAddress(hXTrap4Server, "XTrap_S_SessionReset");
	XTrap_S_SetModuleOn		= (PFN_XTrap_S_SetModuleOn)		GetProcAddress(hXTrap4Server, "XTrap_S_SetModuleOn");
	XTrap_S_SetModuleOff	= (PFN_XTrap_S_SetModuleOff)	GetProcAddress(hXTrap4Server, "XTrap_S_SetModuleOff");
	XTrap_S_SetOption		= (PFN_XTrap_S_SetOption)		GetProcAddress(hXTrap4Server, "XTrap_S_SetOption");
	XTrap_S_SaveLog			= (PFN_XTrap_S_SaveLog)			GetProcAddress(hXTrap4Server, "XTrap_S_SaveLog");
	XTrap_CS_Loading		= (PFN_XTrap_CS_Loading)		GetProcAddress(hXTrap4Server, "XTrap_CS_Loading");
	XTrap_S_SendGamePacket  = (PFN_XTrap_S_SendGamePacket)	GetProcAddress(hXTrap4Server, "XTrap_S_SendGamePacket");
	XTrap_S_RecvGamePacket  = (PFN_XTrap_S_RecvGamePacket)	GetProcAddress(hXTrap4Server, "XTrap_S_RecvGamePacket");

	if(	(XTrap_S_Start			== NULL	)	||
		(XTrap_S_SessionInit	== NULL	)	||
		(XTrap_CS_Step1			== NULL	)	||
		(XTrap_CS_Step3			== NULL	)	||
		(XTrap_S_SessionReset	== NULL	)	||
		(XTrap_S_SetModuleOn	== NULL	)	||
		(XTrap_S_SetModuleOff	== NULL	)	||
		(XTrap_S_SetModuleOff	== NULL	)	||
		(XTrap_S_SetOption		== NULL )	||
		(XTrap_CS_Loading		== NULL	)	||
		(XTrap_S_SendGamePacket	== NULL	)	||
		(XTrap_S_RecvGamePacket	== NULL	)	)
	{
		return	0x20000000;
	}

	return XTRAP_API_RETURN_OK;
}

unsigned long XTrap_S_FreeDll()
{
	if (hXTrap4Server)
		FreeLibrary((HMODULE)hXTrap4Server);

	return XTRAP_API_RETURN_OK;
}

#else

void *hXTrap4Server	= 0;

unsigned long XTrap_S_LoadDll()
{
	unsigned long	ulError			= 0;

	char sDllBinFile[]	="./libXTrap4Server.so";

	hXTrap4Server = dlopen(sDllBinFile, RTLD_LAZY);

	if (hXTrap4Server == 0) {

		printf("dlopen error %s\n", dlerror());

		return 0x10000000|(unsigned long)ulError;	
	}

	XTrap_S_Start		= (PFN_XTrap_S_Start		)	dlsym(hXTrap4Server, "XTrap_S_Start"		);
	XTrap_S_SessionInit	= (PFN_XTrap_S_SessionInit	)	dlsym(hXTrap4Server, "XTrap_S_SessionInit"	);
	XTrap_CS_Step1		= (PFN_XTrap_CS_Step1		)	dlsym(hXTrap4Server, "XTrap_CS_Step1"		);
	XTrap_CS_Step3		= (PFN_XTrap_CS_Step3		)	dlsym(hXTrap4Server, "XTrap_CS_Step3"		);
	XTrap_S_SessionReset= (PFN_XTrap_S_SessionReset	)	dlsym(hXTrap4Server, "XTrap_S_SessionReset"	);
	XTrap_S_SetModuleOn	= (PFN_XTrap_S_SetModuleOn	)	dlsym(hXTrap4Server, "XTrap_S_SetModuleOn"	);
	XTrap_S_SetModuleOff= (PFN_XTrap_S_SetModuleOff	)	dlsym(hXTrap4Server, "XTrap_S_SetModuleOff"	);
	XTrap_S_SetOption	= (PFN_XTrap_S_SetOption	)	dlsym(hXTrap4Server, "XTrap_S_SetOption"	);
	XTrap_S_SaveLog		= (PFN_XTrap_S_SaveLog		)	dlsym(hXTrap4Server, "XTrap_S_SaveLog"		);
	XTrap_CS_Loading	= (PFN_XTrap_CS_Loading		)	dlsym(hXTrap4Server, "XTrap_CS_Loading"		);

	XTrap_S_SendGamePacket		= (PFN_XTrap_S_SendGamePacket	)	dlsym(hXTrap4Server, "XTrap_S_SendGamePacket");
	XTrap_S_RecvGamePacket		= (PFN_XTrap_S_RecvGamePacket	)	dlsym(hXTrap4Server, "XTrap_S_RecvGamePacket");

	if ((XTrap_S_Start			== 0)	||
		(XTrap_S_SessionInit	== 0)	||
		(XTrap_CS_Step1			== 0)	||
		(XTrap_CS_Step3			== 0)	||
		(XTrap_S_SessionReset	== 0)	||
		(XTrap_S_SetModuleOn	== 0)	||
		(XTrap_S_SetModuleOff	== 0)	||
		(XTrap_S_SetOption		== 0) )
	{
		return	0x20000000;
	}

	return XTRAP_API_RETURN_OK;
}

unsigned long XTrap_S_FreeDll()
{
	if(hXTrap4Server)
		dlclose(hXTrap4Server);

	return XTRAP_API_RETURN_OK;
}

#endif

unsigned long XTrap_S_LoadCMF(void* pBuf, unsigned long ulSize, unsigned long ulCount, char* pFileName, unsigned long ulMax)
{
	unsigned long	ulfcnt	= 0;
	unsigned long	ulbcnt	= 0;
	unsigned long	ulRead	= 0;

	FILE	*fi = 0;

	char	sFileName[1024]	= {0,};

	for (ulfcnt = ulMax; ulfcnt >= 0; ulfcnt--)
	{
		sprintf(sFileName, pFileName, ulfcnt);

		if (_access(sFileName, 0) == 0)
		{
			fi = fopen(sFileName, "rb");
			if (fi == NULL) return -1;

			ulRead = fread((unsigned char*)((unsigned char*)pBuf + ulbcnt*ulSize), ulSize, 1, fi);
			if (ulRead != 1) return -1;

			if (fclose(fi))	return -1;

			ulbcnt++;

			if (ulbcnt == ulCount)
				break;
		}	
	}

	return 0;
}

// ex )
//
// XTrap_S_LoadCMF(g_sMapBuffer, XTRAP_CS4_BUFSIZE_MAP, 2, "map%d.cs3", 255);
//
// LOAD  map1.cs3 ~ map255.cs3 
