#include "CEvent.h"
#include "CGame.h"
#include "CDecoder.h"
#include "GameDLL.H"

//
//	아이템 추가
void
CItemDropEvent::addItem(int _iItem,int _iChance)
{
	if (m_iItemCount	>=	dMAX_DROP_ITEM_EVENT_ITEM_COUNT)
	{
		ERRMSG("Too Many Sale Event Item");

		return;
	}

	m_aItemInfo[m_iItemCount].m_wItemIndex	=	_iItem;
	m_aItemInfo[m_iItemCount].m_iDropwChance=	_iChance;

	m_iItemCount++;
}

//
//	지금 아이템 세일 기간이냐?
BOOL
CItemDropEvent::isActiveTerm()
{
	DWORD	dwCurrentTimeValue	=	(g_currentTime.m_wYear-2000)*60*32*12*60+ g_currentTime.m_wMonth*60*32*60+ g_currentTime.m_wDay*60*60+g_currentTime.m_wHour*60+g_currentTime.m_wMinute;
	DWORD	dwBeginTimeValue	=	m_tiBeginTime.m_wYear*60*32*12*60+ m_tiBeginTime.m_wMonth*60*32*60+ m_tiBeginTime.m_wDay*60*60+m_tiBeginTime.m_wHour*60+m_tiBeginTime.m_wMinute;
	DWORD	dwEndTimeValue		=	m_tiEndTime.m_wYear*60*32*12*60+   m_tiEndTime.m_wMonth*60*32*60+   m_tiEndTime.m_wDay*60*60+  m_tiEndTime.m_wHour*60+  m_tiEndTime.m_wMinute;

	if (dwCurrentTimeValue >= dwBeginTimeValue && dwCurrentTimeValue <= dwEndTimeValue)
		return	TRUE;

	return	FALSE;
}

void
CItemDropEvent::readData()
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

			if (STRICMP(strCode,"control_drop_item")!=0)
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

		if (STRICMP(strCode,"drop_item")==0)
		{
			char		*lpstrItemName	=	decoder.GetStream();
			cBasicItem	*lpItem			=	g_im.getItem(lpstrItemName);
			int			iChance			=	decoder.GetNumber();

			if (!lpItem)
			{
				printf("Can not find follow Item : %s\n",lpstrItemName);

				continue;
			}

			addItem(lpItem->m_iSerial,iChance);

			continue;
		}

		if (STRICMP(strCode,"end")==0)	
			break;
	}

	g_game.rebuildAllShopInfo();
}

//	초기화
void
CItemDropEvent::reset()
{
	m_iItemCount	=	0;
	
	memset(&m_tiBeginTime,0,sizeof(m_tiBeginTime));
	memset(&m_tiEndTime,0,sizeof(m_tiEndTime));
}

//	업데이트
void
CItemDropEvent::update()
{
	BOOL	bIsActive	=	isActiveTerm();

	if (m_bIsActive	!=	bIsActive)
		m_bIsActive	=	bIsActive;
}

//	업데이트
int
CItemDropEvent::getDropItem()
{
	if (m_bIsActive	==	FALSE)
		return	0xffff;

	for (int i=0;i<m_iItemCount;i++)
	{
		int	iRandValue	=	largeRandom(1000000);

		if (iRandValue	<	m_aItemInfo[i].m_iDropwChance)
			return	m_aItemInfo[i].m_wItemIndex;
	}

	return	0xffff;
}