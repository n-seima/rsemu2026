#include	"event.h"
#include	"common.h"
#include	"cPACKET_USERDBSERVER.h"

cEventManager	*g_pEventManager	=	NULL;

cEventManager::cEventManager()
{
	m_iUpdateWeeklyRankingDay	=	3;
	memset(&m_timeLastUpdate,0,sizeof(m_timeLastUpdate));
}

void
cEventManager::sendResetWeeklyDuelRecordMessage()
{
	UW_RESET_WEEKLY_DUEL_INFO	packet;
	
	packet.base.set(sizeof(packet),dUW_RESET_WEEKLY_DUEL_INFO);
	
	SendPacket(NULL,&packet);
}

void
cEventManager::update()
{
	CTimeInfo	current;
	
	current.update();

	if	(m_timeLastUpdate.m_wDay !=	current.m_wDay && current.m_wHour == 3)
	{
		if	(current.m_wWeekDay	==	m_iUpdateWeeklyRankingDay)
		{
			//	주간 랭킹 정보 초기화 메시지 월드에 전송
			sendResetWeeklyDuelRecordMessage();	//	일간 전적도 없애 버릴것

			//	주간 랭킹 산정 하고 모든 계정의 주간 전적을 초기화 시킨다.
			InitWeeklyPvPInfo();

			//	주간 랭킹 정보 전송
			SendWeeklyDuelRankingInfo();

			//	일간 랭킹 정보 초기화
			InitDailyPvPInfo();
		}
		else
		{
			//	일간 랭킹/WCG 랭킹 산정
			MakeDailyDeulRankingInfo();

			//	일간/WCG 랭킹 정보 전송
			SendDailyDuelRankingInfo();
		}

		memcpy(&m_timeLastUpdate,&current,sizeof(current));
	}
}