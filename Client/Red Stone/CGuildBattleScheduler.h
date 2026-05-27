#ifndef _classGuildBattleScheduler_H_
#define	_classGuildBattleScheduler_H_

#include "CGamePlay.H"
#include "CGuild.H"

class	CGuildBattleScehduler :	public CGamePlay
{
public:
	CGuildBattleInFieldInfoForClient	m_schedule;

	WORD					m_wYear,m_wMonth,m_wDay;

	CPos					m_pos;
	cBARMENU				m_bmButton;
	int						m_iSelectTime,m_iFocusTime,m_iSelectFieldOrder;
	int						m_iGuildSkillWindowScrollBarPos;
	char					m_strJoinGuildAsker[dNAME_LENGTH];
	WORD					m_wJoinGuildAskerSerial;
	WORD					m_wBeginHour,m_wBeginMinute,m_wBattlePeriod,m_wBattleCount;

							CGuildBattleScehduler();
							
	BOOL					isOpened();	//	열렸냐?

	void					open(int _iBeginHour,int _iBeginMinute,int _iBattlePeriod,int _iBattleCount);	//	열기
	void					updateGuildBattleSchedule(WORD _wTimeOrder,CGuildBattleInFieldInfoForClient *_lpData,int _iYear,int _iMonth,int _iDay);	//	업데이트
	void					updateGuildBattleSchedule(WORD _wTimeOrder,WORD _wFieldOrder,CGuildBattleInfoForClient *_lpData);	//	업데이트

	void					close();	//창 닫기
	void					changeTab(int _iTab,BOOL _bIsCheckSameWork=TRUE);

	BOOL					getFocusTime();	//	마우스 커서가 가르키고 있는 타임을 얻는다.
	void					getBeginBattleTime(int _iTimeOrder,int _iFieldOrder,CTimeInfo *_lpTime);	//	시작 시간 얻기
	void					getTimeText(int _iIndex,int _iCurrentTime,char *_lpstrText,BOOL _bIsNoAmPm=FALSE,BOOL bIsWantPutAMPM=FALSE);

	void					resetSkillPlusButtons();	//	스킬 업 버튼들 초기화
	void					resetSkillPlusButtonsPos();	//	스킬 업 버튼들 위치 조절

	void					draw();		//그리기
	void					drawGuildInfo();	//	길드 정보 출력
	void					drawGuildSkill();	//	길드 스킬
	void					drawGuildItem();	//	길드 아이템

	int						getFocusedSkill();

	void					update();	//업데이트
	void					updateGuildInfo();
	void					updateGuildSkill();
	void					updateGuildItem();
	void					askJoinGuild(char *_lpAskerName,int _iAskerSerial,int _iLevel,int _iJob);

};

extern	CGuildBattleScehduler	g_gwGuildBattleScheduler;

#endif