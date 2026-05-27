#ifndef	_guild_define_in_world_h
#define	_guild_define_in_world_h

#include "CGuildDefine.h"
#include "CGuildManager.h"

class cGuildForWS : public cGuild
{
public:
	BOOL	operate1MinuteWork();
	BOOL	checkGuildMasterLastConnectDate();	//	길마의 마지막 접속일을 체크해 너무 오래 됐으면 길마를 변경한다.
	BOOL	checkGuildMemberLastConnectDate();
	BOOL	checkLastGVGBattleDate();		//	GVG 서버일 경우 마지막 전투일을 체크해 삭제 
	void	sendGuildBattleScore();
};

class	cGuildManagerForWS	:	public cGuildManager
{
public:
	void	operate1MinuteWork();
	void	operatePeriodicallyResetInfoWork(WORD _wWeekDay);

	void	updateLastConnectTime(int _iGuild,int _iMemberRnak);
	void	changeTodayGuildBattleCount(int _iGuild,int _iCount);
	void	changeThisWeekGuildBattleCount(int _iGuild,int _iCount);
};

extern	cGuildManagerForWS		g_guildManager;

extern	void	SaveGuildBaseData(cGuild * guild);
extern	void	SaveGuildSkill(cGuild * guild,DWORD	_dwSerial = 0xffffffff);
extern	void	SaveGuildPet(cGuild * guild,DWORD	_dwSerial = 0xffffffff);
extern	void	SaveGuildGuardian(cGuild * guild,DWORD	_dwSerial = 0xffffffff);
extern	void	SaveGuildStructure(cGuild * guild,DWORD	_dwSerial = 0xffffffff);
extern	void	SaveGuildInven(cGuild * guild,DWORD	_dwSerial = 0xffffffff);
extern	void	SaveGuildMarkData(cGuild * guild,DWORD	_dwSerial = 0xffffffff);
extern	void	LoadGuildMarkInFile(cGuild * _lpGuild);

extern	BOOL	g_bIsTestSiegeWarfareOfGuild;
extern	cGuild	g_guildForCreate; 

#endif