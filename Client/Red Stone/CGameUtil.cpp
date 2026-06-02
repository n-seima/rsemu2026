#include "CGame.H"
#include "speedhackchecker.h"
#include "CHero.h"
#include "CMessage.h"
#include "CMessage.h"

CTooltip	cGAME_UTIL::s_ttCommon;
CTooltip	cGAME_UTIL::s_ttDialogFrame;
CTooltip	cGAME_UTIL::s_ttFrame;
CPopupMenu	cGAME_UTIL::s_pmCommon;
BOOL		cGAME_UTIL::s_bIsSpeedHackUser			=	TRUE;
WORD		cGAME_UTIL::s_wSpeedHackUserCountdown	=	dSYNC_FPS;
char		cGAME_UTIL::s_strTimeStr[256];

BOOL		g_bIsTestServer							=	FALSE;
BOOL		g_bIsDuelServer							=	FALSE;
BOOL		g_bIsInnerTestServer					=	FALSE;

//
//	ID와 패스워드 체크
BOOL
cGAME_UTIL::CheckID(char *_strID)
{
	BYTE	*strID	=	(BYTE *)_strID;

	int	iIDLength	=	strlen(_strID),i;

	if (iIDLength < 4)	return	FALSE;

	for (i=0;i<iIDLength;i++)
	{
		if (strID[i]	>=	'a'	&&	strID[i]	<=	'z')	continue;
		if (strID[i]	>=	'A'	&&	strID[i]	<=	'Z')	continue;
		if (strID[i]	>=	'0'	&&	strID[i]	<=	'9')	continue;
		if (strID[i]	==	'_'	)	continue;
		if (strID[i]	==	'@'	)	continue;
		if (strID[i]	==	'.'	)	continue;

		return	FALSE;
	}

	return	TRUE;
}

//
//	ID와 패스워드 체크
BOOL
cGAME_UTIL::CheckName(char *_strName)
{
	BYTE	*strName=	(BYTE *)_strName;

	int	iNameLength	=	strlen(_strName),i;

	if	(iNameLength	<	4)
		return	FALSE;
#ifdef	_IS_DEV_CLIENT
	return	TRUE;
#endif
	
	{
#ifndef	_DEBUG
		if	(sfcStrstr(_strName,"GM"))
			return	FALSE;
		if	(sfcStrstr(_strName,"gm"))
			return	FALSE;
		if	(sfcStrstr(_strName,"Gm"))
			return	FALSE;
		if	(sfcStrstr(_strName,"gM"))
			return	FALSE;
#endif
	}

	if	(STRICMP(_strName,"EMPTY")==0	)
		return	FALSE;
	if	(STRICMP(_strName,"dummy!!")==0	)
		return	FALSE;
	if	(strName[0]				==	' '	)
		return	FALSE;
	if	(strName[iNameLength-1]	==	' '	)
		return	FALSE;
	if (strstr(_strName,"　"))	return	FALSE;
#ifdef	_FOR_KOREA
	#ifndef	_DEBUG
		if	(strstr(_strName,"☎"))
			return	FALSE;
		if	(strstr(_strName,"☏"))
			return	FALSE;
	#endif
#elif	_FOR_CHINA
	if	(strstr(_strName,"?"))
		return	FALSE;
	if	(strstr(_strName,"?"))
		return	FALSE;
#endif
	if	(sfcStrstr(_strName,dMSG_ADMINISTRATOR))
		return	FALSE;
	if	(sfcStrstr(_strName,dMSG_OPERATOR))
		return	FALSE;

	for (i=0;i<iNameLength;i++)
	{
#ifndef	_FOR_ENG
		if (strName[i] >= 0x80)
		{
			i++;
			continue;	//	|
		}
#endif
		if (strName[i] >= 'a' && strName[i] <= 'z'	)	continue;	//	|
		if (strName[i] >= 'A' && strName[i] <= 'Z'	)	continue;	//	|
		if (strName[i] >= '0' && strName[i] <= '9'	)	continue;	//	|
		if (strName[i] == '_'						)	continue;	//	|

		return	FALSE;
	}

#ifdef	_FOR_KOREA
	for (i=0;i<iNameLength;i++)
	{
		if ((BYTE)strName[i] >= 0x80	)
		{
			WORD	wValue1	=	(BYTE)strName[i+1];
			WORD	wValue2	=	(BYTE)strName[i];
			WORD	wValue	=	(wValue1)+(wValue2<<8);

			if(wValue < 0xb0a1 || wValue > 0xcaa0)
				return	FALSE;
			if((char)wValue2 == -55)		// 한글 2바이트 중 .. 첫번째 바이트가 char형은 -55 출력할때 공백이 보인다....
				return FALSE;
			i++;
		}
	}
#endif

	return	TRUE;
}


#define	dCHECK_CODE_OF_HERO_SAVE_DATA	0x12345678

//
//	마지막으로 플레이한 아바타 이름 얻어오기
void
cGAME_UTIL::ReadLastPlayAvatarName(char *_lpstrId,char *_lpstrName)
{
	strcpy(g_hero.m_strId,_lpstrId);

	strcpy(_lpstrName,"0");
	g_hero.readUserData(_lpstrName);
}

//
//	마지막으로 플레이한 아바타 이름 얻어오기
void
cGAME_UTIL::DrawLoadingText()
{
	int iRateX = 100;
	int iRateY = 100;
	if (g_iScreenWidth != 800)
	{
		iRateX = (int)(g_iScreenWidth*100)/800;
		iRateY = (int)(g_iScreenHeight*100)/600;
	}

	int	iFrame			=	(timeGetTime()/90)%g_sprLoading.m_iCount;

	if	(g_bIsPutSceneWhenLoading	==	FALSE)
		g_sprLoading.Put(g_iScreenWidth/2,g_iScreenHeight/2+g_iCorrectBI_Y/2,iFrame);
	else
		g_sprLoading2.Put(g_iScreenWidth/2, 300+g_iCorrectBI_Y/2, iFrame, iRateX, iRateY);
	
#ifdef	_FOR_USA
	g_sprRegionInterface.Put(g_iScreenWidth,0,10);
#endif
}

//
//	스피드해r
BOOL CALLBACK
cGAME_UTIL::OperateSpeedHackUser(int _iType)
{
	return	FALSE;

	s_bIsSpeedHackUser			=	TRUE;
	s_wSpeedHackUserCountdown	=	random(dSYNC_FPS)+dSYNC_FPS;
/*
	struct PList pList;

	pList = EnumProcs();

	if (pList.num > 0)
		for (int i=0;i<pList.num;i++)
			ERRMSG(pList.pname[i],pList.mname[i]);
*/
	return	TRUE;
}

void
cGAME_UTIL::BeginSpeedHackChecker()
{
	s_bIsSpeedHackUser			=	FALSE;
	s_wSpeedHackUserCountdown	=	0;
	return;

	int		iResult	=	InitChecker("Version=1.3",CLOCK_METHOD);//BOTH_METHOD);
//#define PATTERN_METHOD		1
//#define CLOCK_METHOD		2

	if (iResult	!=	0)
	{
				if	(iResult	==	-1) g_eh.addStaticLog("List file was not found");
		else	if	(iResult	==	-2) g_eh.addStaticLog("MAC values are invalid");
		else	if	(iResult	==	-3) g_eh.addStaticLog("Version is not matched");
		else	if	(iResult	==	-4) g_eh.addStaticLog("List decryption was failed");

		s_wSpeedHackUserCountdown	=	random(dSYNC_FPS)+dSYNC_FPS;

		return;
	}

	SetCallback(OperateSpeedHackUser);

	s_bIsSpeedHackUser			=	FALSE;
	s_wSpeedHackUserCountdown	=	0;

	SetChecker(0, 0, 100*1000);	//	10초에 한번씩 검사
}

void
cGAME_UTIL::CloseSpeedHackChecker()
{
	s_bIsSpeedHackUser			=	FALSE;
	s_wSpeedHackUserCountdown	=	0;
}

//
//	스피드핵 체크
BOOL
cGAME_UTIL::CheckSpeedHack()
{
	s_bIsSpeedHackUser			=	FALSE;
	s_wSpeedHackUserCountdown	=	0;
	return	FALSE;

	struct PList pList;

	pList = EnumProcs();

	if (pList.num > 0)
	{
		for (int i=0;i<pList.num;i++)
			ERRMSG(pList.pname[i],pList.mname[i]);

		s_bIsSpeedHackUser			=	TRUE;
		s_wSpeedHackUserCountdown	=	random(dSYNC_FPS)+dSYNC_FPS;

		return	TRUE;
	}

	return	FALSE;
}

//
//	패스워드 인코드
void
cGAME_UTIL::EncodePassword(char *_lpstrPassword)
{
	int		iPassLength	=	strlen(_lpstrPassword),i;
	DWORD	dwSeedValue	=	60000;

	for (i=0;i<iPassLength;i++)	dwSeedValue		+=	(BYTE)_lpstrPassword[i];

	srand(dwSeedValue);

	for (i=0;i<iPassLength;i++)	_lpstrPassword	+=	random(100);

	*(DWORD *)(_lpstrPassword+iPassLength+1)	=	dwSeedValue;
}

//
//	패스위드 디코드
void
cGAME_UTIL::DecodePassword(char *_lpstrPassword)
{
	int		iPassLength	=	strlen(_lpstrPassword)-4,i;
	DWORD	dwSeedValue	=	*(DWORD *)(_lpstrPassword+iPassLength);

	srand(dwSeedValue);

	for (i=0;i<iPassLength;i++)	_lpstrPassword	-=	random(100);
}


//
//	시간 문자열 얻기
char*
cGAME_UTIL::GetTimeStringHM(int _iHour,int _iMinute)
{
	if (_iMinute)
		sprintf(s_strTimeStr,"%d%s %d%s",_iHour,dMSG_HOUR,_iMinute,dMSG_MINUTE);
	else
		sprintf(s_strTimeStr,"%d%s",_iHour,dMSG_HOUR);
	
	return	s_strTimeStr;

	if (_iHour >= 12)
	{
		if (_iHour	>	12)
			_iHour	-=	12;

		if (_iMinute)
			sprintf(s_strTimeStr,"%s %d%s %d%s",dMSG_PM,_iHour,dMSG_HOUR,_iMinute,dMSG_MINUTE);
		else
			sprintf(s_strTimeStr,"%s %d%s",dMSG_PM,_iHour,dMSG_HOUR);
	}
	else
	{
		if (_iMinute)
			sprintf(s_strTimeStr,"%s %d%s %d%s",dMSG_AM,_iHour,dMSG_HOUR,_iMinute,dMSG_MINUTE);
		else
			sprintf(s_strTimeStr,"%s %d%s",dMSG_AM,_iHour,dMSG_HOUR);
	}

	return	s_strTimeStr;
}


BOOL
cGAME_UTIL::ImeUpdate(int _iX,int _iY,BOOL _bIsClickedLeftButton,BOOL _bIsClickedRightButton)
{
	return	TRUE;
}

BOOL
cGAME_UTIL::ImeDraw(HDC _hDC)
{
	return	TRUE;
}

int
cGAME_UTIL::ImeMessageProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return	TRUE;
}
