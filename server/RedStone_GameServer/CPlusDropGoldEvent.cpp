#include "CEvent.h"
#include "CGame.h"
#include "CDecoder.h"
#include "GameDLL.H"

//
//	지금 이벤트 기간이여?
BOOL
CPlusDropGoldEvent::isEventTerm()
{
	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	m_tiBeginTime.m_wYear*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;
	DWORD	dwEndTimeValue		=	m_tiEndTime.m_wYear*60*32*12*60+   m_tiEndTime.m_wMonth*60*32*60+   m_tiEndTime.m_wDay*60*60+  m_tiEndTime.m_wHour*60+  m_tiEndTime.m_wMinute;

	if (dwCurrentTimeValue >= dwBeginTimeValue && dwCurrentTimeValue <= dwEndTimeValue)
		return	TRUE;

	return	FALSE;
}

//	아이템 가격 얻기
int
CPlusDropGoldEvent::getPlusGold()
{
	if (!isEventTerm())
		return	0;

	return	m_iGold;
}

void
CPlusDropGoldEvent::readData()
{
	cDECODER	decoder;
	char		strFolder[512];

	FPGetCurrentFolder(sizeof(strFolder),strFolder);
	SetLogFolder();

	reset();

	if (!decoder.Upload("data/event.dat",NULL))
	{
		FPSetCurrentFolder(strFolder);

		return;
	}

	while(1)
	{
		char	*strCode	=	decoder.GetWord();

		if	(!strCode)
			break;

		if (STRICMP(strCode,"event")==0)
		{
			strCode	=	decoder.GetWord();

			if (STRICMP(strCode,"control_drop_gold")!=0)
				decoder.FindCloseCode();

			continue;
		}

		if (STRICMP(strCode,"beginTime")==0)
		{
			m_tiBeginTime.m_wYear	=	decoder.GetNumber();
			m_tiBeginTime.m_wMonth	=	decoder.GetNumber();
			m_tiBeginTime.m_wDay	=	decoder.GetNumber();
			m_tiBeginTime.m_wHour	=	decoder.GetNumber();
			m_tiBeginTime.m_wMinute	=	decoder.GetNumber();

			if	(m_tiBeginTime.m_wYear>=2000)
				m_tiBeginTime.m_wYear-=	2000;

			continue;
		}
		if (STRICMP(strCode,"endTime")==0)
		{
			m_tiEndTime.m_wYear		=	decoder.GetNumber();
			m_tiEndTime.m_wMonth	=	decoder.GetNumber();
			m_tiEndTime.m_wDay		=	decoder.GetNumber();
			m_tiEndTime.m_wHour		=	decoder.GetNumber();
			m_tiEndTime.m_wMinute	=	decoder.GetNumber();

			if	(m_tiEndTime.m_wYear>=	2000)
				m_tiEndTime.m_wYear	-=	2000;
			continue;
		}

		if (STRICMP(strCode,"plus_drop_gold")==0)
		{
			m_iGold					=	decoder.GetNumber();

			continue;
		}

		if (STRICMP(strCode,"end")==0)	
			break;
	}
}

//	초기화
void
CPlusDropGoldEvent::reset()
{
	m_iGold		=	0;
	m_bIsActive	=	FALSE;
	
	memset(&m_tiBeginTime,0,sizeof(m_tiBeginTime));
	memset(&m_tiEndTime,0,sizeof(m_tiEndTime));
}

//	업데이트
void
CPlusDropGoldEvent::update()
{
	m_bIsActive	=	isEventTerm();
}
