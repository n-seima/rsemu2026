#ifndef _classGuildBattleManager_H
#define _classGuildBattleManager_H

#include "SFC.h"
#include "CGuildDefine.h"
#include "CGuildManager.h"

class	CGuildBattleManager
{
public:
	CGuildBattleSchedule		m_gbsToday;

	CRITICAL_SECTION			m_csWork;
	CTimeInfo					m_timeLastSiegeWarfareScheduleUpdate;

	void					init();			//	초기화
	void					close();	//	끝장~

	void					reset();	//	초기화
	void					resetTodaySchedule();	//	오늘 스케쥴만 초기화

	void					load();
	void					save();

	int						bookingBattle(int _iGuildSerial,int _iTimeOrder,int _iFieldOrder,int _iCurrentTimeOrder,CGuildBattleInfo *_lpReturnInfo);	//	전투 예약
	int						changeBattle(int _iGuildSerial1, int _iGuildSerial2,int _iTimeOrder,WORD _wField,CGuildBattleInfo * _lpReturnInfo,CGuildBattleInfo * _lpCancelInfo);
	int						cancelBookedBattle(int _iGuildSerial,CGuildBattleInfo *_lpReturnInfo);	//	예약된거 취소
	int						cancelBattles(int _iBattleOrder);	//	전부 취소
	CGuildBattleInfo*		getBookedScheduleWithAll(int _iGuildSerial);
	CGuildBattleInfo*		getBookedSchedule(int _iGuildSerial,int _iCurrentTimeOrder);
	int						getBookedScheduleCount(int _iGuildSerial);
	void					correctGuildBattle();

	BOOL					update(BOOL _bIsCheckTime=FALSE);	//	업데이트
	BOOL					updateUserTestSiegeWarfare();
	
	void					updateSiegeWarfareSchedule();	//	공성전 스케쥴 업데이트
	void					buildProgramOfSiegeWarfare();

};

extern	void	SaveGuildBaseData(cGuild * guild);

extern	CGuildBattleManager	g_guildBattleManager;

#endif