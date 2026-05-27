#ifndef _classGuildBattleManager_H
#define _classGuildBattleManager_H

#include "SFC.h"
#include "CGuild.h"

class	CGuildBattleManager : public CGuildBattleSchedule
{
	static	int			s_iExceptGuildBattleWeekDay;
	static	CTimeInfo	s_timeExceptGuildBattleBegin,s_timeExceptGuildBattleEnd;
public:
	BOOL				m_bIsTomorrow;

	void				init();
	void				initForTest(char *_lpGuildName1,char *_lpGuildName2,int _iTerm,int _iField);
	void				resetSchedule();	//	스케쥴 초기화

	void				operateOncePerMinute();	//	업뎃
	
	int					getCurrentBattleOrder(int _iFieldOrder=0);	//	현재 전투 순서
	int					getCurrentBattleStatus(int _iFieldOrder=0);	//	현재 전투 상태
	int					getFieldOrderByFieldSerial(int _iUniqueFieldSerial,int _iTimeOrder);
	CGuildBattleInfo*	getBattleInfoByFieldSerial(int _iUniqueFieldSerial,int _iOrder);
	CGuildBattleInfo*	getBattleInfoByGuild(int _iGuildSerial,int *_lpTimeOrder=NULL,int *_lpFieldOrder=NULL);	//	길드로 등록된 전투가 없는지 검색한다.
	CGuildBattleInfo*	getBattleInfo(int _iTimeOrder,int _iFieldOrder);	//	길드전 정보 얻기
	void				updateBattleInfo(int _iTimeOrder,int _iFieldOrder,int _iGuild1,int _iGuild2);
	void				requestGuildBattle(cACTOR *_lpActor,int _iTimeOrder,int _iFieldOrder);	//	길드전 요청
	BOOL				cancelGuildBattle(cACTOR *_lpActor);	//	길드전 취소
	void				resetGuildBattle(int _iBattleOrder);	//	특정 시간대 길드전 통째로 취소
	void				getBeginTime(int _iTimeOrder,CTimeInfo *_lpTime,int _iFieldOrder=0);	//	길드전 시작 시간 구하기
	void				resetFieldSerial();
	BOOL				isBlockToRequestBattleTime();	//	길드전에 관한 신청을 안 받는 시간이다.(자정 전후 10분)

	inline	void		setByTomorrow()	{m_bIsTomorrow	=	TRUE;}
	inline	void		setByToday()	{m_bIsTomorrow	=	FALSE;}
	inline	BOOL		isTomorrow()	{return	m_bIsTomorrow;}
	inline	BOOL		isToday()		{return	1-m_bIsTomorrow;}

	static	BOOL		IsExceptGuildBattleTime(int _iWeekDay,int _iHour,int _iMinute);
	static	void		SetMaintenanceTime(int _iWeekDay,int _iBeginHour,int _iBeginMinute,int _iEndHour,int _iEndMinute);
};

extern	CGuildBattleManager	g_gbsToday;
extern	BOOL				g_bIsReceivedGuildBattleSchedule;
extern	int					g_iGuildBattleInitTime,g_iGuildBattleInitEnterenceTime,g_iGuildBattleReadyTime,g_iGuildBattlePeriod,g_iGuildBattleTime,g_iGuildBattleBeginTime;
extern	int					g_iGuildBattleProgressTime,g_iGuildBattleFinishTime,g_iGuildBattleEndTime;


#endif