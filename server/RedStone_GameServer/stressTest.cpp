#include "stressTest.h"
#include "Client.h"
#include "cPACKET_GAMESERVER.h"
#include "cGame.h"

cStressTestManager	*g_pStressTestManager	=	NULL;

cStressTestManager::cStressTestManager()
{
	reset();
}

void
cStressTestManager::reset()
{
	memset(m_aiRepoterList,0xff,sizeof(m_aiRepoterList));
}

BOOL
cStressTestManager::addReporter(int _iSerial)
{
	for	(int	i=0;i<1024;i++)
	{
		if	(m_aiRepoterList[i]	==	0xffffffff)
		{
			m_aiRepoterList[i]	=	_iSerial;

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL
cStressTestManager::addClient(int _iSerial)
{
	for	(int	i=0;i<1024*10;i++)
	{
		if	(m_aiClient[i][0]	==	0xffffffff)
		{
			m_aiClient[i][0]	=	_iSerial;
			m_aiClient[i][1]	=	0;

			return	TRUE;
		}
	}

	return	FALSE;
}

void
cStressTestManager::sendReport()
{
	for	(int	i=0;i<1024;i++)
	{
		if	(m_aiRepoterList[i]	==	0xffffffff)
			continue;

		CClient	*lpClient	=	CM.mGETCLIENT(m_aiRepoterList[i]);

		if	(lpClient	==	NULL)
		{
			m_aiRepoterList[i]	=	0xffffffff;
			continue;
		}

		SG_STRESS_SERVER_INFO	packet;

		packet.base.set(sizeof(packet),dSG_STRESS_SERVER_INFO);

		packet.iConnectUserCount	=	CM.Size();
		packet.iUpdateTime			=	g_game.m_dw1SecondTime;

		lpClient->AddSendPacket((char *)&packet,packet.base.wSize);
	}
}

void
cStressTestManager::updateJoin()
{
	int	iAddUserCount	=	0;

	for	(int	i=0;i<1024*10;i++)
	{
		if	(m_aiClient[i][0]	==	0xffffffff)
			continue;

		CClient	*lpClient	=	CM.mGETCLIENT(m_aiClient[i][0]);

		if	(lpClient	==	NULL)
		{
			m_aiClient[i][0]	=	0xffffffff;
			continue;
		}

		if	(m_aiClient[i][1]	==	0xffffffff)
			continue;

		m_aiClient[i][1]++;

		if	(m_aiClient[i][1]	<	dSYNC_FPS)
			continue;

		cACTOR				actor;
		CUpkeepPlayerData	upkeepData;
		cActorStatusUpkeep	upkeepStatusData;

		upkeepData.reset();
		upkeepStatusData.reset();

		char	strId[dID_LENGTH],strName[dNAME_LENGTH];

		sprintf(strId,"ID_%d",timeGetTime());
		sprintf(strName,"Name_%d",timeGetTime());
		lpClient->SetName(strName);
		lpClient->SetID(strId,"");

		actor.initializeJobData(strId,strName,random(4));

		int	iField	=	g_game.m_pField[CM.Size()/100].m_iUniqueSerial;

		actor.m_wCurrentField	=	iField;
		actor.m_iYPos			=	0;
//		if (lpClient && lpClient->GetActorSerialInServer() == (DWORD)m_iZoneSerial)	return	lpClient;

		g_game.bookingJoinPlayer(&actor,&upkeepData,&upkeepStatusData,lpClient->wSerial,TRUE);

		iAddUserCount++;

		m_aiClient[i][1]	=	0xffffffff;

		break;
	}
}

/*
void
cStressTestManager::sendPacket(int _iSize)
{
	for	(int	i=0;i<1024*10;i++)
	{
		if	(m_aiClient[i]	==	0xffffffff)
			continue;

		CClient	*lpClient	=	CM.mGETCLIENT(m_aiClient[i]);

		if	(lpClient	==	NULL)
		{
			m_aiClient[i]	=	0xffffffff;
			continue;
		}

		SG_STRESSCLIENT	packet;

		packet.base.set(sizeof(packet),dSG_STRESS_SERVER_INFO);

		packet.iConnectUserCount	=	CM.Size();
		packet.iUpdateTime			=	g_game.m_dw1SecondTime;

		lpClient->AddSendPacket((char *)&packet,packet.base.wSize);
	}
}*/