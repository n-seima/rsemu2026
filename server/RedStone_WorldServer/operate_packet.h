#ifndef	_ws_operate_packet_h
#define	_ws_operate_packet_h

#include <winsock2.h>
#include "cPACKET_WORLDSERVER.h"
#include "cPACKET_USERDBSERVER.h"
#include "cPACKET_DBSERVER.h"

namespace	nsReceiveFromGS
{
	void	addGuildGold(SERVERStoWORLD_ADD_GUILD_GOLD* _lpPacket);
};

class	CClient;
class	cGuild;
class	cSOCKET;

extern	void	SendChangeGVGServerGuildMasterPacketToLoginServer(char *_lpstrGuildName,char *_lpstrNewMasterID,CClient *_lpLoginServer);
extern	void	sendRenewPremiumItemInfo(CClient *_lpUser,BOOL _bIsNow);
extern	void	sendGSPCalling(CClient *_lpUser,SERVERStoWORLD_CALLING *_lpPacket);
extern	void	sendGSPValidGuild(CClient *_lpUser,int _iGuildSerial);	//	게임 서버에 빈 서버라고 통보한다.
extern	void	sendGSPChangeDay(CClient *_lpUser);	//	날짜가 바뀜
extern	void	sendGSPGuildBattleSchedule(CClient *_lpUser);	//	길드전 스케쥴 전송
extern	void	sendGSPResetWeekFightGuildInfo(CClient *_lpUser);
extern	void	sendGSPUpdateGuildInfo(CClient *_lpUser,int _iGuildSerial,BOOL _bIsRequireResetMemberInfo);	//	길드 정보 전송
extern	void	sendGSPUpdateGuildMemberInfo(CClient *_lpUser,int _iGuildSerial);	//	길드 멤버 정보 전송
extern	void	sendGSPGuildNameList(CClient *_lpUser,int _iFirstGuild,int _iServerIndex);
extern	void	sendGSPDuelRankerInfo(CClient *_lpUser,BOOL _bIsDailyRanking);
extern	void	sendGSPDuelTeamInfo(CClient *_lpServer);


extern	void	operateBeginTestGuildBattle(CClient *_lpUser,SERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE *_lpPacket);
extern	void	sendGSPGuildName(CClient *_lpUser,int _iSerial);
extern	void	sendGSPGuildBattleInfo(CClient *_lpUser,int _iTimeOrder,int _iFieldOrder);	//	특정 시간대 길드전 정보
extern	void	operateGuildBattleResult(CClient *_lpUser,SERVERStoWORLD_GUILD_BATTLE_RESULT *_lpPacket);	//	길드전 결과 처리
extern	void	operateSelectGuildMark(CClient *_lpUser,SERVERStoWORLD_SELECT_GUILD_MARK *_lpPacket);
extern	void	operateUpdateGuildMemberLevel(CClient *_lpUser,SERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL *_lpPacket);
extern	void	forceSetTodayGuildBattleSchedule(CClient *_lpUser,int _iTimeOrder,int _iFieldOrder,int _iGuild1,int _iGuild2,BOOL _bIsSendData=FALSE);
extern	void	operateBanishUser(CClient *_lpUser,SERVERStoWORLD_BANISH_USER* _lpPacket);
extern	void	operateAppointmentQuizEvent(CClient *_lpUser,SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT *_lpPacket);
extern	void	sendAppointmentQuizEventInfo(CClient *_lpUser);
extern	void	sendWorldValueInfo(CClient *_lpUser);

extern	void	operateSetQuizEventVillage(CClient *_lpUser,SERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE *_lpPacket);
extern	void	operateOXQuizLoserReviveBattle(CClient *_lpUser);	//	패자 부활전
extern	void	operateProduceOxQuiz(CClient *_lpUser,SERVERStoWORLD_PRODUCE_OX_QUIZ *_lpPacket);
extern	void	operateProduceWordQuiz(CClient *_lpUser,SERVERStoWORLD_PRODUCE_WORD_QUIZ *_lpPacket);
extern  void	operateChangeEventWord(CClient *_lpUser,SERVERStoWORLD_SET_EVENT_WORD *_lpPacket , BOOL _isStart = FALSE);
extern	void	operateChangeWordQuizRound(CClient *_lpUser,SERVERStoWORLD_CHANGE_WORD_QUIZ_ROUND *_lpPacket);
extern	void	OperateBuildGuildBattleProlog(BOOL _bIsForTestGuild);
extern	void	OperateSimpleWork(SERVERStoWORLD_SIMPLE_WORK *_lpPacket,CClient *_lpUser,CClient *_lpLoginServer,cSOCKET *_lpUSerDbc);

extern	void	operateEndQuizEvent(CClient *_lpUser,SERVERStoWORLD_END_QUIZ_EVENT *_lpPacket);
extern	void	operateCancelQuizEvent(CClient *_lpUser,SERVERStoWORLD_CANCEL_QUIZ_EVENT *_lpPacket);
extern	void	operateIsDenyCommunityTargetPacket(SERVERStoWORLD_IS_DENY_COMMUNITY_TARGET *_lpPacket);
extern	void	operateMakeGuildLevelPacket(SERVERStoWORLD_MAKE_GUILD_LEVEL *_lpPacket);
extern	void	operateSimpleWork(SERVERStoWORLD_SIMPLE_WORK *_lpPacket);
extern	void	OperateDisconnectGameServer(CClient *_lpServer,int _iCause=-1);
extern	void	CheckGuildCheckSum(CClient *_lpServer,SERVERStoWORLD_CHECK_GUILD_CHECKSUM *_lpPacket);
extern	void	operateAddRenewItemExpireTime(CClient *_lpServer,SERVERStoWORLD_ADD_RENEW_ITEM_EXPIRE_TIME *_lpPacket);
extern	void	operateAskRenewItemExpireTimeList(CClient *_lpServer);
extern	void	sendItemPackList(CClient *_lpServer,int _iPackSerial=0xffff);
extern	void	sendCarrotShopInfo(CClient *_lpServer);
extern	void	sendGRInfo(CClient *_lpServer);
extern	void	operateUpdateCarrotShopInfo(CClient *_lpServer,UW_ASK_CARROTSHOPINFO_RESULT* _lpPacket);
extern	void	operateBoostPowerByEvent(CClient *_lpServer,SERVERStoWORLD_BOOST_POWER_BY_EVENT* _lpPacket);
extern	void	operateUpdateItemPackInfo(CClient *_lpServer,DWMSG_ITEMPACK_RESULT* _lpPacket);
extern	void	operateItemPackInfoCheckSum(CClient *_lpServer,SERVERStoWORLD_ITEM_PACK_INFO_CHECKSUM* _lpPacket);

extern	void	operateRemoveFriendInTargetList(char *_lpstrClientName,char *_lpstrTargetName);
extern	void	operateRemoveFriendInTargetListResult(char *_lpstrClientName,char *_lpstrTargetName,int _iResult);
extern	void	SendGuildMemberUpdateInfoToBCS(cGuild *_lpGuild,CGuildMan *_lpMember);

extern	void	SendUpdateGuildHallInfo(CClient *_lpServer=NULL,BOOL _bIsBuildSiegeWarfareSchedule=FALSE);		//	길드 홀 정보 날리기
extern	void	SendNoticeBeginSiegeWarfare(CClient *_lpServer,int _iBattleOrder);	//	공성전 시작 예고
extern	void	SendBeginSiegeWarfare(CClient *_lpServer,int _iBattleOrder);		//	공성전 시작
extern	void	SendEndSiegeWarfare(CClient *_lpServer,int _iBattleOrder);			//	공성전 종료
extern	void	SendNoticeEndSiegeWarfare(CClient *_lpServer,int _iBattleOrder);	//	공성전 종료 x분전

extern	void	SendSimpleWork(CClient *_lpServer,int _iWork,int _iValue0=0,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,
															int _iValue6=0,int _iValue7=0,int _iValue8=0,int _iValue9=0,int _iValue10=0,int _iValue11=0);

extern	void	operateTransAvatarToGVGServerPacket(CClient *_lpLoginServer,CPlayerSaveDataForServerDefine *_lpSaveData,CClient *_lpGS,int _iServerIndex,cSOCKET *_lpDBCSocket);
extern	void	operateTransAvatarToGVGResultPacket(CClient *_lpGameServer,SERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT	*_lpPacket);
extern	void	operateTransAvatarToGVGResultDBPacket(CClient *_lpLoginServer,DWMSG_TRANS_AVATAR_TO_GVG_RESULT *_lpPacket);

extern	void	operateTransGuildToGVGServerPacket(CClient *_lpLoginServer,SERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER *_lpPacket,CClient *_lpGS,cSOCKET *_lpDBCSocket);
extern	void	operateTransGuildToGVGResultPacket(CClient *_lpGameServer,SERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT	*_lpPacket);
extern	void	operateChangeGVGServerGuildMasterPacket(SERVERStoWORLD_CHANGE_GVG_SERVER_GUILD_MASTER *_lpPacket,cSOCKET *_lpDBCSocket);

extern	void	operateG2W_ASK_GVG_AVATAR_ORG_DATA(CClient *_lpLoginServer,CClient *_lpGameServer,G2W_ASK_GVG_AVATAR_ORG_DATA *_lpPacket);
extern	void	operateL2W_TOSS_ASK_GVG_AVATAR_ORG_DATA(CClient *_lpLoginServer,L2W_TOSS_ASK_GVG_AVATAR_ORG_DATA *_lpPacket);
extern	void	operateD2W_GVG_AVATAR_ORG_DATA(CClient *_lpLoginServer,D2W_GVG_AVATAR_ORG_DATA *_lpPacket);
extern	void	operateD2W_SIMPLE_WORK(cSOCKET *_lpUser,CClient *_lpLoginServer,D2W_SIMPLE_WORK* _lpPacket);
extern	void	operateD2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(CClient *_lpLoginServer,D2W_ASK_GVG_AVATAR_ORG_DATA_RESULT* _lpPacket);
extern	void	operateL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT(CClient *_lpGameServer,L2W_ASK_GVG_AVATAR_ORG_DATA_RESULT *_lpPacket=NULL,char *_lpstrName=NULL,int _iResult=0);
extern	void	operateL2W_GVG_AVATAR_ORG_DATA(CClient *_lpGameServer,L2W_GVG_AVATAR_ORG_DATA *_lpPacket);


//	->DBC
extern	cSOCKET*	GetADBSocket();		//	server.cpp 에 선언되어 있음.
extern	cSOCKET*	GetIDDBSocket();	//	server.cpp 에 선언되어 있음.

extern	void	SendAskDuelRankingInfoMsg();
extern	void	SendAskDuelTeamInfoMsg();

#endif