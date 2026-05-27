#include "CUtil.h"
#include "cPACKET_WORLDSERVER.h"

cWorldUtil	g_worldUtil;

cWorldUtil::cWorldUtil()
{
	CTimeInfo	current;

	current.update();

	m_wYear		=	current.m_wYear;
	m_wMonth	=	current.m_wMonth;
	m_wDay		=	current.m_wDay;

	memset(m_aPlayerCount,0,sizeof(m_aPlayerCount));
	memset(m_awPlayerCountPerLevel,0,sizeof(m_awPlayerCountPerLevel));

	m_dwLastSendAccountInfoTime				=	0;
	m_wIsExportedPlayerCountPerFiledInfo	=	FALSE;

	memset(m_awPlayerCountInField,0,sizeof(m_awPlayerCountInField));
	memset(m_awPlayerCountInSecretDungeon,0,sizeof(m_awPlayerCountInSecretDungeon));
	
	InitializeCriticalSection(&m_csWork);
}

cWorldUtil::~cWorldUtil()
{
	DeleteCriticalSection(&m_csWork);
}

int
cWorldUtil::getPlayerCount()
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_GAME_SERVER_COUNT;i++)
		iCount	+=	m_aPlayerCount[i];

	return	iCount;
}

void
cWorldUtil::setPlayerCount(SERVERStoWORLD_ACCOUNT_INFO *_lpPacket)
{
	m_aPlayerCount[_lpPacket->iServerIndex]	=	_lpPacket->iPlayerCount;

	memset(m_awPlayerCountPerLevel[_lpPacket->iServerIndex],0,sizeof(m_awPlayerCountPerLevel[_lpPacket->iServerIndex]));
	memcpy(m_awPlayerCountPerLevel[_lpPacket->iServerIndex],_lpPacket->awPlayerCountPerLevel,sizeof(WORD)*_lpPacket->wMaxLevel);
}

BOOL
cWorldUtil::isSendAccountInfoTime()
{
	DWORD	dwCurrentTime	=	timeGetTime();

	if (m_dwLastSendAccountInfoTime	>	dwCurrentTime	||	m_dwLastSendAccountInfoTime+1000*60	<	dwCurrentTime)	//	1분에 한번
	{
		m_dwLastSendAccountInfoTime	=	dwCurrentTime;

		return	TRUE;
	}

	return	FALSE;
}

void
cWorldUtil::updatePlayerCountInField(int _iHour,int _iCount,WORD *_lpwList)
{
	mCS(m_csWork);

	if	(_iHour	==	0	&&	m_wIsExportedPlayerCountPerFiledInfo	==	FALSE)
	{
		m_wIsExportedPlayerCountPerFiledInfo	=	TRUE;
		cFILE	file;

		char	strFileName[256];

		sprintf(strFileName,"%s/data/playerCountLog/%s_%.2d_%.2d_%.2d.txt",g_strLogFolder,strWorldName,m_wYear,m_wMonth,m_wDay);

		if	(file.Open(strFileName,"wt"))
		{
			int	iHour,iMaxCount;
			file.writeStream("Field/Hour	max	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23\n");

			for	(int iField=0;iField<1000;iField++)
			{
				file.writeStream("%3d	",iField);

				iMaxCount	=	0;

				for	(iHour=0;iHour<24;iHour++)
				{
					if	(m_awPlayerCountInField[iHour][iField]	>	iMaxCount)
						iMaxCount	=	m_awPlayerCountInField[iHour][iField];
				}

				file.writeStream("%d	",iMaxCount);

				for	(iHour=0;iHour<24;iHour++)
					file.writeStream("%d	",m_awPlayerCountInField[iHour][iField]);

				file.writeStream("\n");
			}

			for	(int iSD=0;iSD<200;iSD++)
			{
				file.writeStream("sd %3d	",iSD);

				iMaxCount	=	0;

				for	(iHour=0;iHour<24;iHour++)
				{
					if	(m_awPlayerCountInSecretDungeon[iHour][iSD]	>	iMaxCount)
						iMaxCount	=	m_awPlayerCountInSecretDungeon[iHour][iSD];
				}

				file.writeStream("%d	",iMaxCount);

				for	(int iHour=0;iHour<24;iHour++)
					file.writeStream("%d	",m_awPlayerCountInSecretDungeon[iHour][iSD]);

				file.writeStream("\n");
			}
		}

		memset(m_awPlayerCountInField,0,sizeof(m_awPlayerCountInField));
		memset(m_awPlayerCountInSecretDungeon,0,sizeof(m_awPlayerCountInSecretDungeon));
	}

	if	(_iHour	==	23)
		m_wIsExportedPlayerCountPerFiledInfo	=	FALSE;
	
	CTimeInfo	current;

	current.update();

	m_wYear		=	current.m_wYear;
	m_wMonth	=	current.m_wMonth;
	m_wDay		=	current.m_wDay;

	for	(int i=0;i<_iCount;i++)
	{
		int	iField	=	_lpwList[i*2];
		int	iCount	=	_lpwList[i*2+1];

		if	(iField	>=	1000)
			m_awPlayerCountInSecretDungeon[_iHour][iField-1000]	+=	iCount;
		else
			m_awPlayerCountInField[_iHour][iField]	=	iCount;
	}
}