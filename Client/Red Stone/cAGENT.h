#ifndef _classAGENT_H
#define _classAGENT_H

#include "cNEMO.H"
#include "cPACKET.H"
#include "CGame.H"
#include "CSkill.H"
#include "ssl.h"

#pragma comment(lib, "ssl.lib")

#include <string>

#ifdef _FOR_KOREA
	#ifndef	_USE_BROADCAST
		#define _USE_BROADCAST
	#endif	
#endif

//	POPS - Period Of Packet Send
#define	dPOPS_DEFAULT					5
#define	dPOPS_BUY_ITEM					dPOPS_DEFAULT	//	æ¿€ ±∏¿‘ ∏Ω√¡E¿E€ ¡÷±E5√
#define	dPOPS_SET_REACTION_STATUS		dPOPS_DEFAULT

#define	dPORT_DUMMY_SERVER				7045

#define	dMAX_PACKET_QUE_COUNT			256

#define	dCONNECT_STATUS_TRYING			0
#define	dCONNECT_STATUS_CONNECT_OK		1
#define	dCONNECT_STATUS_CONNECT_FAILED	2
#define	dCONNECT_STATUS_TIME_OVER		3
#define	dDEFAULT_LIFE_CODE_CYCLE		4*60

#define	mINIT_PACKET(a,b) a.base.set(sizeof(a),b)

class	CActor;	//	¿EEº±æE
//
//	∂ 
class cQUE_PACKET
{
	SERVER_PACKETS		m_aPacket[dMAX_PACKET_QUE_COUNT];		//	∂E.
	int					m_iFirstQue,m_iLastQue,m_iCount;		//	√ππ∞,∏∂¡∏∑ ∂, Ωø ¿÷∂ ºE	CRITICAL_SECTION	m_csOperateQue;

public:
						cQUE_PACKET();
						~cQUE_PACKET();

	void				reset();								//	√±≠
	BOOL				addPacket(SERVER_PACKETS *_lpPacket);			//	∂ √∞°
	BOOL				isReceivedPacket(int _iPacket);			//	π¿∫ ∂¿≥?
	SERVER_PACKETS*		popPacket();							//	ø°º≠ ∂¿ª ª≥Ω.
	int					getCount()	{return	m_iCount;}
};	//	class cQUE_PACKET

//
//	ø°¿¿EÆø°º≠ ∞EÆœ øøE¿
class	CPacketStatus
{
public:
	DWORD				m_dwBeginOperateItemTime;
	BOOL				m_isOperateItemData;
	BOOL				m_bIsWaitTradeItemResult;
	int					m_iSendSetReactionPacketTime;	//	∏Ææº º≥¡§ ∂ ∫∏≥Ω Ω√∞

	inline	void		reset()
	{
		m_bIsWaitTradeItemResult	=	FALSE;
		m_isOperateItemData			=	FALSE;
		m_iSendSetReactionPacketTime=	0;	//	
	}

};	//	class	CPacketStatus

//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
//° ¡÷¿!! ‘ºE√∞°Ω√ CAgent::init(HWND _hWnd)ø° ∞∞¿ √∞° “ ∞!!
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
class	cPACKET_HANDLER	:	public CGame
{
public:
//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠∑Œ±¿Œ º≠πE	static	void				ResultLogin(SERVER_PACKETS *_lpPacket);//∑Œ±¿Œ ∞·∞E	static	void				ResultLoginOTP(SERVER_PACKETS *_lpPacket);//OTP∑Œ±¿Œ ∞·∞E	static	void				ServerList(SERVER_PACKETS *_lpPacket);//º≠π∏ÆΩ∫Æ∏¶ π.
	static	void				AvatarList(SERVER_PACKETS *_lpPacket);//æπ∏∏ÆΩ∫Æ∏¶ π.
	static	void				ResultCreateAvatar(SERVER_PACKETS *_lpPacket);// æπ∏ ª˝º∫ ∞·∞E	static	void				ResultRemoveAvatar(SERVER_PACKETS *_lpPacket);// æπ∏ ¡¶∞ ∞·∞E	static	void				ReceiveSelectAvatarResult(SERVER_PACKETS *_lpPacket);// æπ∏ ¡¶∞ ∞·∞E	static	void				ResultOfCopyAvatar(SERVER_PACKETS *_lpPacket);// æπ∏ ∫πªE∞·∞E//¶≠∑Œ±¿Œ º≠πE//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
	static	void				ReceiveSSLAuthenticationCode(SERVER_PACKETS *_lpPacket);// SSL ¿Œ¡E∂ π¿Ω

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠øπœº ∞E√
//¶≠CAgentReceiveCommunityPacket.cpp
//¶¨
	static	void				Chat(SERVER_PACKETS *_lpPacket);//√√ -o-
	static	void				ReceiveChatFailed(SERVER_PACKETS *_lpPacket);//√√ Ω
	static	void				ReceiveSayResult(SERVER_PACKETS *_lpPacket);//√√ -o-

	static	void				ReceiveAskUnionJoinParty(SERVER_PACKETS *_lpPacket);//	ø¨’ º E° ø‰√ª
	static	void				ReceiveReplayUnionJoinParty(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveUnionPartyMemberInfo(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveUnionPartyInfo(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveUnionPartyDissolution(SERVER_PACKETS *_lpPacket);

	static	void				ReceivePartyList(SERVER_PACKETS *_lpPacket);//	º∏ÆΩ∫Æ
	static	void				ReceiveChangePartyStatus(SERVER_PACKETS *_lpPacket);//	ºª π≤E	static	void				ReceivePartyInfo(SERVER_PACKETS *_lpPacket);//	º∏πø°∞‘ ∫∏≥ª º ¡§∫∏(ºøE∏ÆΩ∫Æ≥™ ±∏˚E
	static	void				ReceiveAskJoinParty(SERVER_PACKETS *_lpPacket);//	º E° ø‰√ª
	static	void				ReceiveJoinPartyMember(SERVER_PACKETS *_lpPacket);//	º ∏πE√∞°
	static	void				ReceivePartyWork(SERVER_PACKETS *_lpPacket);//	±∫∞° ºø°º≠ ªE.(∞≥™ œ∞ ≥∞≥™.. -_-a)
	static	void				ReceiveUpdatePartyMemberInfo(SERVER_PACKETS *_lpPacket);//	º ∏πE¡§∫∏∏¶ æ¿Æ(√º∑¡§..)
	static	void				ReceiveUpdatePartyMemberHP(SERVER_PACKETS *_lpPacket);//	º ∏πE¡§∫∏∏¶ æ¿Æ(√º∑¡§..)
	static	void				ReceiveChangePartyName(SERVER_PACKETS *_lpPacket);//	º ∏πE¡§∫∏∏¶ æ¿Æ(√º∑¡§..)
	static	void				ReceivePartyMessage(SERVER_PACKETS *_lpPacket);//	ºø ∞E√ ¿∑±¿˙∑± ∏Ω√¡E≥ ∏Æı∞° æ≥!!,≥ ºø° º” ¿÷¡Eææ!!,º Æ~ ˚˚E.)
	static	void				ReceiveDissolutionParty(SERVER_PACKETS *_lpPacket);//	º Eº ∏Ω√¡E
	static	void				ReceiveAskRemoveFriend(SERVER_PACKETS *_lpPacket);//	±∏ ∏∑œø°º≠ ¡¶∞ ø‰√ª ∏Ω√¡E	static	void				ReceiveAskRemoveFriendResult(SERVER_PACKETS *_lpPacket);//	±∏ ∏∑œø°º≠ ¡¶∞ ø‰√ª¿ ∞·∞E
	static	void				ReceiveRequestAddFriend(SERVER_PACKETS *_lpPacket);//	±∏ √∞° ø‰√ª ∏Ω√¡E	static	void				ReceiveReplyForAddFriend(SERVER_PACKETS *_lpPacket);//	±∏ √∞° ø‰√ªø° E EE//¶≠CAgentReceiveCommunityPacket.cpp
//¶≠øπœº ∞E√
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠≥E°ªE∞E√
//¶≠CAgentReceivePitchmanShop.cpp
	static	void				ReceiveOpenPitchmanShop(SERVER_PACKETS *_lpPacket);			//	≥E°ªEø¿
	static	void				ReceiveAddPitchmanShopItem(SERVER_PACKETS *_lpPacket);		//	≥E°ªø° æ¿€ √∞°
	static	void				ReceiveRemovePitchmanShopItem(SERVER_PACKETS *_lpPacket);	//	≥E°ªø° æ¿€ ¡¶∞
	static	void				ReceiveChangePitchmanShopInfo(SERVER_PACKETS *_lpPacket);	//	≥E°ªEª ∫Ø∞E	static	void				ReceiveChangePitchmanShopItemPlace(SERVER_PACKETS *_lpPacket);	//	≥E°ªø° æ¿€ ¡¶∞
	static	void				ReceiveBuyPitchmanShopItem(SERVER_PACKETS *_lpPacket);		//	≥E°ªø°º≠ æ¿€ ±∏¿‘
	static	void				ReceiveSoldPitchmanShopItem(SERVER_PACKETS *_lpPacket);		//	≥E°ªø°º≠ æ¿€¿ ∑.
	static	void				ReceiveAddPitchmanShop(SERVER_PACKETS *_lpPacket);			//	≥E°ªE√∞°
	static	void				ReceiveRemovePitchmanShop(SERVER_PACKETS *_lpPacket);		//	≥E°ªE¡¶∞
	static	void				ReceivePitchmanShopList(SERVER_PACKETS *_lpPacket);			//	≥E°ªE∏ÆΩ∫Æ
	static	void				ReceiveSimplePitchmanShopInfoList(SERVER_PACKETS *_lpPacket);	//	≥E°ªE∏ÆΩ∫Æ
	static	void				ReceivePitchmanShopInfo(SERVER_PACKETS *_lpPacket);			//	≥E°ªE¡§∫∏
	static	void				ReceivePitchmanShopMessage(SERVER_PACKETS *_lpPacket);		//	≥E°ªE∏Ω√¡E	static	void				ReceiveResultOfSearchItemInRelayStation(SERVER_PACKETS *_lpPacket);		//	≥E°ªE∏Ω√¡E//¶≠CAgentReceivePitchmanShop.cpp
//¶≠≥E°ªE∞E√
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠±E∞E√
//¶≠CAgentReceiveGuildPacket.cpp
//¶¨
	static	void				ReceiveBaseGuildInfo(SERVER_PACKETS *_lpPacket);		//	±E±∫ª ¡§∫∏
	static	void				ReceiveSimpleGuildInfo(SERVER_PACKETS *_lpPacket);		//	∞∑´ ±E±∫ª ¡§∫∏
	static	void				ReceiveGuildBattleInfoForMember(SERVER_PACKETS *_lpPacket);	//	∏π∏¶ ¿ß ±¿E¡§∫∏
	static	void				ReceiveGuildMarkInfo(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveGuildGuardianAndPetExp(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveSiegeWarfareNotice(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveStoreGuildInventoryItem(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveRemoveGuildInventoryItem(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveStoreGuildInventoryGold(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveRemoveGuildInventoryGold(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveChangeGuildInventoryItemPlace(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveOpenGuildHallList(SERVER_PACKETS *_lpPacket);	//	±E¶ ∏ÆΩ∫Æ
	static	void				ReceiveOpenRelatedPlaceList(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveGuildHonorPointList(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveGuildMasterGraceInfo(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveGVGWorkSuccess(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveOpenGuildInventory(SERVER_PACKETS *_lpPacket);	//	±E¿Œ∫‰∏Æ ø≠±E	static	void				ReceiveOpenGuildScheduler(SERVER_PACKETS *_lpPacket);	//	æœΩ√ ±EΩ∫…¡EØ ø¿

	static	void				ReceiveOpenGuildMarkComposer(SERVER_PACKETS *_lpPacket);	//	±E∏∂ ¡∂’ ¿Œ‰¿Ω∫ ø¿
	static	void				ReceiveGuildBattleScheduleInSameTime(SERVER_PACKETS *_lpPacket);	//	±EΩ∫…¡E	static	void				ReceiveChangeGuildSchedule(SERVER_PACKETS *_lpPacket);	//	±EΩ∫…¡E∫Ø∞
	static	void				ReceiveUpdateGuildBattleCountAtThisWeek(SERVER_PACKETS *_lpPacket);	//	¿π¡÷ø° ±¿EΩºEæ¿Æ
	static	void				ReceiveReadyToNextGuildBattle(SERVER_PACKETS *_lpPacket);	//	¿Ω ±¿E¡∫E∏Ω√¡Eπ¿Ω
	static	void				ReceiveGuildBattleResult(SERVER_PACKETS *_lpPacket);	//	±¿E∞·∞Eπ¿Ω

	static	void				ReceiveReadyToMakeGuild(SERVER_PACKETS *_lpPacket);		//	±E∏±€±E¡∫E!
	static	void				ReceiveCreateGuildResult(SERVER_PACKETS *_lpPacket);	//	±E∏±€±E∞·∞E
	static	void				ReceiveGuildDissolution(SERVER_PACKETS *_lpPacket);		//	±E√º

	static	void				ReceiveAskJoinGuildResult(SERVER_PACKETS *_lpPacket);	//	±EE° Ω√ª ∞·∞E	static	void				ReceiveAskJoinGuild(SERVER_PACKETS *_lpPacket);			//	±E∞°¿‘ Ω√ª

	static	void				ReceiveJoinGuild(SERVER_PACKETS *_lpPacket);		//	±E∞°¿‘
	static	void				ReceiveLeaveGuild(SERVER_PACKETS *_lpPacket);		//	±EªE	static	void				ReceiveUpdateGuildLevel(SERVER_PACKETS *_lpPacket);		//	±E∑π∫ß æ¿Æ
	static	void				ReceiveUpdateGuildMemberInfo(SERVER_PACKETS *_lpPacket);//	±E∏πE¡§∫∏ æ¿Æ
	static	void				ReceiveRemoveGuildMember(SERVER_PACKETS *_lpPacket);//	±E∏πE¡¶∞

	static	void				ReceiveUpdateGuildSkillLevel(SERVER_PACKETS *_lpPacket);//	±EΩ∫≥∑π∫ß ∫Øø
	static	void				ReceiveChangeGuildMark(SERVER_PACKETS *_lpPacket);	//	±∏∂ º±√.
	
	static	void				ReceiveGuildBattleNotice(SERVER_PACKETS *_lpPacket);	//	±¿E∞E√ ∞¡E	static	void				ReceiveGuildBattleInfo(SERVER_PACKETS *_lpPacket);	//	±¿E∞E√ ∞¡E	static	void				ReceiveGuildBattleKillNotice(SERVER_PACKETS *_lpPacket);	//	±¿E∞E√ ∞¡E	static	void				ReceiveGuildWorkMessage(SERVER_PACKETS *_lpPacket);		//	±E∞E√ ¿€æE∏Ω√¡E	static	void				ReceiveChangeGuildExpTax(SERVER_PACKETS *_lpPacket);	//	±Eºº¿≤ ∫Ø∞E	static	void				ReceiveChangeGuildNotice(SERVER_PACKETS *_lpPacket);	//	±E∞¡E∫Ø∞E	static	void				ReceiveChangeRankInGuild(SERVER_PACKETS *_lpPacket);	//	±E¡¿ß ∫Ø∞E
	static	void				ReceiveGuildMemberList(SERVER_PACKETS *_lpPacket);		//	±E∏π∏ÆΩ∫Æ π±E

//¶¨
//¶≠CAgentReceiveGuildPacket.cpp
//¶≠øπœº ∞E√
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠CAgentReceivePkacet.cpp
//¶≠∞‘¿” º≠πE	static	void				Disconnect(SERVER_PACKETS *_lpPacket);//Ω∫¡≥Æ ∏Ω√¡E	static	void				ResultJoin(SERVER_PACKETS *_lpPacket);//∞‘¿” E° ∞·∞E	static	void				ReceiveFailedToJoin(SERVER_PACKETS *_lpPacket);//∞‘¿” E° Ω
	static	void				ResultGameConnect(SERVER_PACKETS *_lpPacket);//∞‘¿” ø¨∞E∞·∞E	static	void				ReceiveBCSInfo(SERVER_PACKETS *_lpPacket);//∫EŒE≥Ω∫Æ º≠πE¡§∫∏
	static	void				ReceivePlayerInfo(SERVER_PACKETS *_lpPacket);//¡÷¿Œ∞E¡§∫∏
	static	void				ReceiveSuccessToJoinGame(SERVER_PACKETS *_lpPacket);//¡÷¿Œ∞E¡§∫∏
	static	void				ReceiveEnterToTherSecretDungeon(SERVER_PACKETS *_lpPacket);//∫π ¡Ø ¿‘¿E
	static	void				ReceiveJoinPlayerMessage(SERVER_PACKETS *_lpPacket);//√∑π¿æEE°
	static	void				ReceiveSimpleActorInfoList(SERVER_PACKETS *_lpPacket);//æ ∏ÆΩ∫Æ
	static	void				ReceiveSimpleActorInfo(SERVER_PACKETS *_lpPacket);//æ ∏ÆΩ∫Æ
	static	void				ReceiveVerySimpleActorInfoList(SERVER_PACKETS *_lpPacket);//√÷º“¿ ≥∏Ø ¡§∫∏ æ ∏ÆΩ∫Æ
	static	void				ReceiveMinimumActorInfoList(SERVER_PACKETS *_lpPacket);//´ø° ∫ æ ∏ÆΩ∫Æ Ω¿‘¿ª ¿ßœæ -o-
	static	void				ReceiveOperatorList(SERVER_PACKETS *_lpPacket);//Eæø° ø˚€¿ ∏ÆΩ∫Æ π¿Ω
	static	void				ReceiveRefitActorsPostion(SERVER_PACKETS *_lpPacket);//æE¿ß° ¿Eº≥¡§
	static	void				ReceiveOuterActorsPosUpdate(SERVER_PACKETS *_lpPacket);//¡÷±¿˚¿∏∑Œ ø∞˚ø° æE¡∏¶ æ¿Æ ... ∂¡∏∏, ¿∏ß¿ ¡ª.. -_-a
	static	void				ReceiveEventNpcList(SERVER_PACKETS *_lpPacket);//¿øœ¡Eæ NPC ∏ÆΩ∫Æ

	static	void				RegenMonster(SERVER_PACKETS *_lpPacket);//√∑π¿æEE°
	static	void				ReceivePlayerOutMessage(SERVER_PACKETS *_lpPacket);//√∑π¿æE≥™∞¨.

	static	void				Kill(SERVER_PACKETS *_lpPacket);//¡æ˙æE. -o-;;;;;
	static	void				UpdateExp(SERVER_PACKETS *_lpPacket);//∞° æ¿Æ
	static	void				UpdatePetExp(SERVER_PACKETS *_lpPacket);//Pet ∞° æ¿Æ
	static	void				UpdateBasicInformation(SERVER_PACKETS *_lpPacket);//√º∑ æ¿Æ
	static	void				RemoveActor(SERVER_PACKETS *_lpPacket);//æ ¡¶∞
	static	void				ReceiveVerySimpleActorInfo(SERVER_PACKETS *_lpPacket);//∏Ω∫ ¡§∫∏
	static	void				LevelUp(SERVER_PACKETS *_lpPacket);//	∑π∫ß æE	static	void				LevelUpOther(SERVER_PACKETS *_lpPacket);//	∑π∫ß æE	static	void				ChangeStatePower(SERVER_PACKETS *_lpPacket);//	Ω∫ ¡ı∞° ∞·∞E	static	void				IncreaseAbilityLevel(SERVER_PACKETS *_lpPacket);//	Ω∫ ¡ı∞° ∞·∞E
	static	void				Move(SERVER_PACKETS *_lpPacket);//¿ø
	static	void				MoveResult(SERVER_PACKETS *_lpPacket);//¿ø ∞·∞E	static	void				ReceiveMoveFailedMessage(SERVER_PACKETS *_lpPacket);//¿ø Ω
	static	void				CorrectPlayerPosition(SERVER_PACKETS *_lpPacket);//¿ø Ω
	static	void				Stop(SERVER_PACKETS *_lpPacket);//¡§¡E	static	void				SetMoveAbility(SERVER_PACKETS *_lpPacket);//¿ø …∑ ∫Ø∞E∞±E∂±Eo-)
	static	void				Transformation(SERVER_PACKETS *_lpPacket);//∫ØΩÆ -o-!!
	static	void				TransformationFailed(SERVER_PACKETS *_lpPacket);//∫ØΩÆ Ω
	static	void				ToggleSitdown(SERVER_PACKETS *_lpPacket);//æ…±E	static	void				SitdownResult(SERVER_PACKETS *_lpPacket);//æ…±E∞·∞E	static	void				ReceiveDeathPeneltySecond(SERVER_PACKETS *_lpPacket);//¡¿Ω¿ Ω√∞
	static	void				ReceiveQuestionForRecoverDeathPenelty(SERVER_PACKETS *_lpPacket);//¡¿Ω¿ Ω√∞

	static	void				ReceiveAddPortal(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveRemovePortal(SERVER_PACKETS *_lpPacket);
	static	void				ReceivePortalList(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveChangeDoorStatus(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveDoorMessage(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveDoorsStatusInField(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveRedistribute(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveStopBattle(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveAccountInfo(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveGGAuthCode(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveHSGuid(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveHSReqMsg(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveWinningPrizeLotto(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveArcaList(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveAddArca(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveAttackToObject(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveArcaWork(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveBrokenArcaItemByAttack(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveFieldTrapWork(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveSecretDungeonMessage(SERVER_PACKETS *_lpPacket);

	static	void				ReceivePlaySound(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveEtcWork(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveSetAnm(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveDuelResult(SERVER_PACKETS *_lpPacket);//	¿∫Æ
	static	void				ReceiveDuelRanking(SERVER_PACKETS *_lpPacket);//	¿∫Æ
	static	void				ReceiveDuelRecord(SERVER_PACKETS *_lpPacket);	//	∞·E¡§∫∏ π¿Ω
	static	void				ReceiveTryDuelBattle(SERVER_PACKETS *_lpPacket);	//	∞·EΩ√ª‘
	static	void				ReceiveDuelMsg(SERVER_PACKETS *_lpPacket);	//	∞·E∏Ω√¡E//¶≠CAgentReceivePacket.cpp
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠CAgentReceiveSkillPkacet.cpp
	static	void				ActionToActor(SERVER_PACKETS *_lpPacket);//≥∏Øø°∞‘ æº!!
	static	void				ReceiveActionToGroup(SERVER_PACKETS *_lpPacket);//±∑E°∞‘ æº!!
	static	void				ReceiveActionToGroupStickedBit(SERVER_PACKETS *_lpPacket);//∫Æ∫¿∫ ±∑E°∞‘ æº!!
	static	void				ActionToGround(SERVER_PACKETS *_lpPacket);//∂ø°∞° æº!!
	static	void				QuickAction(SERVER_PACKETS *_lpPacket);//∂ø°∞° æº!!
	static	void				BasicAttack(SERVER_PACKETS *_lpPacket);//±¡∞∞
	static	void				ContinuousHitAttack(SERVER_PACKETS *_lpPacket);//ø¨º”∏∞¿ª ¿‘ ∞∞
	static	void				ReceiveContinuousHitAttackByValue(SERVER_PACKETS *_lpPacket);//º°ø° ¿ ø¨º”¿˚¿∏∑Œ ∞∞œ Ω∫≥
	static	void				ReceiveLandMarkerAttack(SERVER_PACKETS *_lpPacket);//∑E∏∂ø ∞∞
	static	void				ReceiveStrikeIntervalShooter(SERVER_PACKETS *_lpPacket);//¿ŒπEΩ ∞∞!!
	static	void				ReceiveCastIntervalShooter(SERVER_PACKETS *_lpPacket);//¿ŒπEΩ πªE	static	void				ReceiveGuardianPost(SERVER_PACKETS *_lpPacket);//∞°æEΩ∫Æ
	static	void				ReceiveHitChanedDamage(SERVER_PACKETS *_lpPacket);//

	static	void				WideAreaDamage(SERVER_PACKETS *_lpPacket);//	±§ø™ ∞∞ E¡E	static	void				WideAidAttackResult(SERVER_PACKETS *_lpPacket);//	±§ø™ ∫∏¡∂ ∞∞
	static	void				WideHeal(SERVER_PACKETS *_lpPacket);	//	±§ø™ E	static	void				UseSkillResult(SERVER_PACKETS *_lpPacket);//Ω∫≥ªøE∞·∞E	static	void				ReceiveSetReactionStatus(SERVER_PACKETS *_lpPacket);//	∏Ææº º≥¡§ ∞·∞Eπ±E	static	void				ReceiveHitThornDamage(SERVER_PACKETS *_lpPacket);//	∞°Ω√ø° ∏≤
	static	void				ReceiveAbsorbDamage(SERVER_PACKETS *_lpPacket);//	E¡E˙E	static	void				ReceiveChangeSpecialSkillField(SERVER_PACKETS *_lpPacket);//Ω∫º Ω∫≥E∫Ø∞E	static	void				ReceiveRevive(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveUpdateBoostCount(SERVER_PACKETS *_lpPacket);
	static	void				ReceivePeneltyTime(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveTitleEffect(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveXtrapPacket(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveUpdatePacketEncryptCode(SERVER_PACKETS *_lpPacket);

	static	void				ReceiveSetShimmeringShield(SERVER_PACKETS *_lpPacket);//Ω√∏”∏ ΩEº≥¡§
	static	void				ReceiveReleaseShimmeringShield(SERVER_PACKETS *_lpPacket);//Ω√∏”∏ ΩE¡¶
	static	void				ReceiveCureActor(SERVER_PACKETS *_lpPacket);//°∑E
	static	void				ReceiveFloatMachineMessage(SERVER_PACKETS *_lpPacket);	//	∏”Ω ∂øEE	static	void				ReceiveReleaseMachineMessage(SERVER_PACKETS *_lpPacket);	//	∏”Ω Ææ˚÷±E	static	void				ReceiveShootMachineMessage(SERVER_PACKETS *_lpPacket);	//	∏”Ω¿ πª¿œ πªE	static	void				ReceiveTailChaser(SERVER_PACKETS *_lpPacket);	//	¿œ √º¿º≠
	static	void				ReceiveTurnOnPassiveSkill(SERVER_PACKETS *_lpPacket);	//	Ω√∫EΩ∫≥ ø
	static	void				ReceiveDisplacement(SERVER_PACKETS *_lpPacket);	//	Ω∫√∑π¿Ω∫∏’Æ
	static	void				ReceiveMiss(SERVER_PACKETS *_lpPacket);	//	Ω∫√∑π¿Ω∫∏’Æ

	static	void				ReceiveThrustActors(SERVER_PACKETS *_lpPacket);	//	æº¿ª √E.
	static	void				ReceiveMakeActionForm(SERVER_PACKETS *_lpPacket);	//	æº¿ª √E.
	static	void				ReceiveUseSkillToActor(SERVER_PACKETS *_lpPacket);	//	æº¿ª √E.
	static	void				ReceiveUseSkillToGround(SERVER_PACKETS *_lpPacket);	//	æº¿ª √E.
	static	void				ReceiveCreateDummy(SERVER_PACKETS *_lpPacket);	//	ıπ ª˝º∫
	static	void				ReceiveReleaseDummy(SERVER_PACKETS *_lpPacket);	//	ıπ ¡¶∞
	static	void				ReceiveReceiveAidMagic(SERVER_PACKETS *_lpPacket);	//	∫∏¡∂ ∏∂π˝
	static	void				ReceiveChargeCP(SERVER_PACKETS *_lpPacket);	//	CP√¿E.
	static	void				ReceiveHitAuraSkill(SERVER_PACKETS *_lpPacket);	//	ø¿∂EΩ∫≥ø° ∏æ“.
	static	void				ReceiveMessage(SERVER_PACKETS *_lpPacket);	//	ø¿∂EΩ∫≥ø° ∏æ“.
	static	void				ReceiveRemainHP(SERVER_PACKETS *_lpPacket);	//	ø¿∂EΩ∫≥ø° ∏æ“.
	static	void				ReceiveThrust(SERVER_PACKETS *_lpPacket);	//	ø¿∂EΩ∫≥ø° ∏æ“.
	static	void				ReceiveRush(SERVER_PACKETS *_lpPacket);	//	ø¿∂EΩ∫≥ø° ∏æ“.
	static	void				ReceiveActiveReactionSkill(SERVER_PACKETS *_lpPacket);	//	∏Ææº Ω∫≥ πø
	static	void				ReceiveHideEquipment(SERVER_PACKETS *_lpPacket);	//	ø¿∂EΩ∫≥ø° ∏æ“.
	static	void				ReceiveBunshineAttack(SERVER_PACKETS *_lpPacket);//æ¿€ ±≥Ø
	static	void				ReceiveScimitarCutting(SERVER_PACKETS *_lpPacket);//æ¿€ ±≥Ø
	static	void				ReceiveIcyStalagmite(SERVER_PACKETS *_lpPacket);//æ¿€ ±≥Ø
	static	void				ReceiveActorEnchantedStatus(SERVER_PACKETS *_lpPacket);//æ¿€ ±≥Ø
	static	void				ReceiveTeleport(SERVER_PACKETS *_lpPacket);//∑πÆ
	static	void				ReceiveApplyExtraSkillEffect(SERVER_PACKETS *_lpPacket);//Ω∫≥ ∫Œ∞° ø∞E¿˚øE
	static	void				ReceiveActiveSkillEffect(SERVER_PACKETS *_lpPacket);//Ω∫≥ ø∞Eπø
	static	void				ReceiveActiveMirrorTower(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveFindUserResult(SERVER_PACKETS *_lpPacket);//¿Ø¿E√±E∞·∞E	static	void				ReceiveSimpleHit(SERVER_PACKETS *_lpPacket);//
	static	void				ReceiveHitDamage(SERVER_PACKETS *_lpPacket);// E¡E¿‘æ˙.
	static	void				ReceiveSkillEffect(SERVER_PACKETS *_lpPacket);// Ω∫≥ ¿Æ πª˝
	static	void				ReceivePlaySkillSound(SERVER_PACKETS *_lpPacket);// Ω∫≥ ªø˚E√∑π¿
	static	void				ReceiveStrikeLightningWinder(SERVER_PACKETS *_lpPacket);// ∂¿Æ ø¿ŒEπªE	static	void				ReceiveStrikeMultiMissile(SERVER_PACKETS *_lpPacket);// ∏÷ºπª¿œ πªE.
	static	void				ReceiveRegenHPByAidSkill(SERVER_PACKETS *_lpPacket);//√º∑ ∏Æ¡®
	static	void				ReceiveRecoverHPByAidSkill(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveExitGameSuccess(SERVER_PACKETS *_lpPacket);//∞‘¿” ≥™∞°±E	static	void				ReceiveRegistMessage(SERVER_PACKETS *_lpPacket);//˚”œ º≠πE∏Ω√¡E	static	void				ReceiveHitAction(SERVER_PACKETS *_lpPacket);//Æ æº 
	static	void				ReceiveDetectingEvil(SERVER_PACKETS *_lpPacket);//
	static	void				ReceiveReadyToWarpToNearestVillage(SERVER_PACKETS *_lpPacket);//
//¶≠CAgentReceiveSkillPkacet.cpp
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶¨E¿ø
	static	void				MoveFieldResult(SERVER_PACKETS *_lpPacket);//	E¿ø ∞·∞E	static	void				ReceiveMoveFieldNow(SERVER_PACKETS *_lpPacket);//	¡± ¿E¿ø!!!!
	static	void				ReceiveFirstAid(SERVER_PACKETS *_lpPacket);


//¶¨æ¿€ ∞E√
	static	void				ReceiveItemWorkResult(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveOpenRepairItemWindow(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveOpenItemDisjointingWindow(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveRepairItem(SERVER_PACKETS *_lpPacket);
	static	void				DroppingItems(SERVER_PACKETS *_lpPacket);//æ¿€ ∂≥±¡ESG_DROPPING_ITEMS
	static	void				ReceiveStripEquipmentResult(SERVER_PACKETS *_lpPacket);//¿∫E¡¶ ∞·∞Eπ¿Ω
	static	void				StripEquipment(SERVER_PACKETS *_lpPacket);//æ¿€ ∂≥±¡ESG_DROPPING_ITEMS
	static	void				DroppingItem(SERVER_PACKETS *_lpPacket);//æ¿€ ∂≥±¡ESG_DROPPING_ITEM
	static	void				RemoveDroppedItems(SERVER_PACKETS *_lpPacket);//∂≥æ˚Eæ¿€E¡¶∞ dSG_REMOVE_DROPPED_ITEMS
	static	void				RemoveDroppedItem(SERVER_PACKETS *_lpPacket);//∂≥æ˚Eæ¿€ ¡¶∞ dSG_REMOVE_DROPPED_ITEM
	static	void				ReceiveUpdateItemDurability(SERVER_PACKETS *_lpPacket);//æ¿€ ≥ª±∏∑

	static	void				ChangeDroppedItemInfo(SERVER_PACKETS *_lpPacket);//æ¿€ ∂≥±¡ESG_CHANGE_DROPPED_ITEM_INFO
	static	void				PickItemResult(SERVER_PACKETS *_lpPacket);//æ¿€ ¡±E∞·∞EdSG_PICK_ITEM_RESULT
	static	void				ReceivePickPartyGold(SERVER_PACKETS *_lpPacket);//º º“¿Ø ∞∏¶ ¡æ˙.
	static	void				ReceivePickPartyItem(SERVER_PACKETS *_lpPacket);//º º“¿Ø æ¿€¿ª ¡æ˙.
	static	void				EquipItemResult(SERVER_PACKETS *_lpPacket);//æ¿€ ¿∫œ±E∞·∞˙dSG_EQUIP_ITEM_RESULT
	static	void				ReloadBeltItem(SERVER_PACKETS *_lpPacket);//æ¿€ ∏Æ∑ŒdSG_RELOAD_ITEM
	static	void				ReceiveBeltItemReloadResult(SERVER_PACKETS *_lpPacket);//æ¿€ ∏Æ∑ŒdSG_ITEM_RELOAD_RESULT
	static	void				UseItemResult(SERVER_PACKETS *_lpPacket);//æ¿€ ªøE∞·∞˙dSG_USE_ITEM_RESULT
	static	void				ReceiveUseItemToItemResult(SERVER_PACKETS *_lpPacket);//æ¿€¿ª æ¿€ø° ªøE ∞·∞E	
	static	void				ReceiveRotteryTicketPrizeWinning(SERVER_PACKETS *_lpPacket);//∫π± æ¿€ √∑dSG_LOTTERY_TICKET_PRIZE_WINNING
	static	void				UseBeltItemResult(SERVER_PACKETS *_lpPacket);//∫ßÆ æ¿€ ªøE∞·∞˙dSG_USE_BELT_ITEM_RESULT
	static	void				UseItem(SERVER_PACKETS *_lpPacket);//æ¿€ ªøE∞·∞˙dSG_USE_ITEM
	static	void				OpenSelectItemInPackWindow(SERVER_PACKETS *_lpPacket);//æ¿€ ªøE∞·∞˙dSG_USE_ITEM

	static	void				OtherPlayerItemData(SERVER_PACKETS *_lpPacket);//∏ ª∂Eæ¿€ ¿ π¿ΩdSG_OTHER_PLAYER_ITEM_DATA
	static	void				ItemData(SERVER_PACKETS *_lpPacket);//æ¿€ ¿ π¿Ω ∞·∞˙dSG_ITEM_DATA
	static	void				EquipmentData(SERVER_PACKETS *_lpPacket);//¿∫E¿ π¿ΩdSG_EQUIPMENT_DATA
	static	void				InventoryData(SERVER_PACKETS *_lpPacket);//¿Œ∫‰∏Æ ¿ π¿ΩdSG_ENVENTORY_DATA

	static	void				ChangeAppearance(SERVER_PACKETS *_lpPacket);//Ø¡§ ≥∏Ø¿ ¿∫Eø∞E° ∫Ø≠∞° ¿÷)ø° ∫Ø≠∞° ¿÷. dSG_CHANGE_EQUIPMENT
	static	void				DropItemResult(SERVER_PACKETS *_lpPacket);//æ¿€ π∏Æ±E∞·∞˙dSG_DROP_ITEM_RESULT
	static	void				ChangeItemPlaceResult(SERVER_PACKETS *_lpPacket);//æ¿€ ±≥Ø

	static	void				ReceiveDecreaseItemCount(SERVER_PACKETS *_lpPacket);//æ¿€ ±≥Ø
	static	void				ReceiveAddItem(SERVER_PACKETS *_lpPacket);//æ¿€ √∞° -o-
	static	void				ReceiveRemoveItem(SERVER_PACKETS *_lpPacket);//æ¿€ ¡¶∞ -o-
	static	void				ReceiveAddGold(SERVER_PACKETS *_lpPacket);//∑ √∞° -o-
	static	void				ReceiveSetItemUniqueSerial(SERVER_PACKETS *_lpPacket);//æ¿€¿ ¿Øœ Ω√∏ÆæEπ¿Ω
	static	void				ReceiveUpdateItemData(SERVER_PACKETS *_lpPacket);//æ¿€¿ ¿Øœ Ω√∏ÆæEπ¿Ω
	static	void				ReceiveUpdateWayPointInfo(SERVER_PACKETS *_lpPacket);//æ¿€¿ ¿Øœ Ω√∏ÆæEπ¿Ω
	static	void				ReceiveCureActorByPotion(SERVER_PACKETS *_lpPacket);//æ¿€¿ ¿Øœ Ω√∏ÆæEπ¿Ω

	static	void				ReceiveExpiredItem(SERVER_PACKETS *_lpPacket);//æ¿€ ∏Æº
	static	void				ReceiveExpiredItemList(SERVER_PACKETS *_lpPacket);//æ¿€ ∏Æº ∏ÆΩ∫Æ

	static	void				ReceiveUseOrbResult(SERVER_PACKETS *_lpPacket);//ø¿∫EªøE∞·∞E	
	static	void				ReceiveDestroyItemResult(SERVER_PACKETS *_lpPacket);	//	æ¿€ ¡¶∞
	static	void				ReceiveItemPackName(SERVER_PACKETS *_lpPacket);	//	æ¿€  ¿∏ß
	static	void				ReceivePrefix(SERVER_PACKETS *_lpPacket);	//	¡ŒªE¡¶∞
	static	void				ReceiveMiniPetInfo(SERVER_PACKETS *_lpPacket);	//	πœE¡§∫∏ æ¿Æ

	
//¶¨Æ∑π¿E∞E√
	static	void				ReceiveRequestTrade(SERVER_PACKETS *_lpPacket);	//		dSG_REQUEST_TRADE					0x1190	//	Æ∑π¿Eø‰√ª
	static	void				ReceiveRequestTradeResult(SERVER_PACKETS *_lpPacket);	//		dSG_REQUEST_TRADE_RESULT			0x1191	//	Æ∑π¿Eø‰√ª ∞·∞E	
	static	void				ReceiveBeginTrade(SERVER_PACKETS *_lpPacket);	//	dSG_BEGIN_TRADE						0x1192	//	Æ∑π¿EΩ√¿€
	static	void				ReceiveAddTradeItemByMe(SERVER_PACKETS *_lpPacket);	//	dSG_ADD_TRADE_ITEM_BY_ME			0x1193	//	Æ∑π¿Eæ¿€ √∞°
	static	void				ReceiveAddTradeItemByTradeMate(SERVER_PACKETS *_lpPacket);	//	dSG_ADD_TRADE_ITEM_BY_TRADE_MATE	0x1194	//	Æ∑π¿Eæ¿€ √∞°
	static	void				ReceiveChangeTradeItemDataByMe(SERVER_PACKETS *_lpPacket);	//	0x1193	//	Æ∑π¿Eæ¿€ √∞°
	static	void				ReceiveChangeTradeItemDataByTradeMate(SERVER_PACKETS *_lpPacket);	//	0x1194	//	Æ∑π¿Eæ¿€ √∞°

	static	void				ReceiveSetTradeGold(SERVER_PACKETS *_lpPacket);	//	dSG_SET_TRADE_GOLD					0x1195	//	Æ∑π¿Eæ¿€ √∞°
	static	void				ReceiveRemoveTradeItem(SERVER_PACKETS *_lpPacket);	//	dSG_REMOVE_TRADE_ITEM				0x1196	//	Æ∑π¿Eæ¿€ ¡¶∞ Æ∑π¿Eæ¿€ ¡¶∞ ∞·∞E	
	static	void				ReceiveReadyTrade(SERVER_PACKETS *_lpPacket);	//	dSG_READY_TRADE						0x1197	//	Æ∑π¿E¡∫E	
	static	void				ReceivePermitTrade(SERVER_PACKETS *_lpPacket);	//	dSG_PERMIT_TRADE					0x1198	//	Æ∑π¿Eº∂E	
	static	void				ReceiveTrade(SERVER_PACKETS *_lpPacket);	//	dSG_TRADE							0x1199	//	Æ∑π¿E	
	static	void				ReceiveTradeMessage(SERVER_PACKETS *_lpPacket);	//	dSG_TRADE_MESSAGE					0x119a	//	Æ∑π¿E∏Ω√¡E
//¶¨¿∫E∞E√
	static	void				ReceiveOpenBank(SERVER_PACKETS *_lpPacket);//¿∫Eø≠±E	
	static	void				ReceiveBankMessage(SERVER_PACKETS *_lpPacket);//¿∫E∞E√ ∏Ω√¡Eπ±E	
	static	void				ReceiveStoreItemToTheBank(SERVER_PACKETS *_lpPacket);//æ¿€ ¿˙¿E	
	static	void				ReceiveWithdrawItemFromTheBank(SERVER_PACKETS *_lpPacket);//æ¿€ √±E	
	static	void				ReceiveStoreGoldToTheBank(SERVER_PACKETS *_lpPacket);//∑ ¿˙¿E	
	static	void				ReceiveWithdrawGoldFromTheBank(SERVER_PACKETS *_lpPacket);//∑ √±E	
	static	void				ReceiveMoveBankItem(SERVER_PACKETS *_lpPacket);//¿∫Eæø° æ¿€ ¿ß° ∫Ø∞E	
	static	void				ReceiveMoveInventoryItemWhenBankTransaction(SERVER_PACKETS *_lpPacket);//æ¿€ ¿ß° ∫Ø∞E
//¶¨º˚Œ ´Æ
	static	void				ReceiveOpenCart(SERVER_PACKETS *_lpPacket);	//	º˚Œ ´Æ ø≠±E	
	static	void				ReceiveCartMessage(SERVER_PACKETS *_lpPacket);	//	º˚Œ ´Æ ø≠±E	
	static	void				ReceiveWithdrawItemFromCart(SERVER_PACKETS *_lpPacket);	//	º˚Œ ´Æ ø≠±E	
	static	void				ReceiveLoadFailedCartPremiumItem(SERVER_PACKETS *_lpPacket);	//	º˚Œ ´Æ ø≠±E	
	static	void				ReceiveDestroyCartItemResult(SERVER_PACKETS *_lpPacket);	//	º˚Œ ´Æø° æ¿€ ¡¶∞
	static	void				ReceiveCarrotShopInfo(SERVER_PACKETS *_lpPacket);	//	± ª¡° ¡§∫∏ π¿Ω

//¶¨¿∫Æ ∞E√
	static	void				ReceiveComplexSpeech(SERVER_PACKETS *_lpPacket);//E≠ ∞E√
	static	void				ReceiveEventFailed(SERVER_PACKETS *_lpPacket);//¿∫Æ Ω
	static	void				ReceiveEndDialog(SERVER_PACKETS *_lpPacket);//E≠ ¡æ∑E	
	static	void				ReceiveOpenShop(SERVER_PACKETS *_lpPacket);//E≠ ª¡° ø≠±E
	static	void				ReceiveOpenGuildShop(SERVER_PACKETS *_lpPacket);//±Eª¡° ø≠±E
	static	void				ReceiveBuyItem(SERVER_PACKETS *_lpPacket);//æ¿€ ±∏¿‘
	static	void				ReceiveFailedBuyItem(SERVER_PACKETS *_lpPacket);//æ¿€ ±∏¿‘ Ω	
	static	void				ReceiveSellItem(SERVER_PACKETS *_lpPacket);//æ¿€ ∏ 
	static	void				ReceiveFailedSellItem(SERVER_PACKETS *_lpPacket);//æ¿€ ∏ Ω	
	static	void				ReceiveQuestInfo(SERVER_PACKETS *_lpPacket);//æ¿€ ∏ Ω	
	static	void				ReceiveChangeQuestInfo(SERVER_PACKETS *_lpPacket);//æ¿€ ∏ Ω	
	static	void				ReceiveDamageToActorByKarma(SERVER_PACKETS *_lpPacket);//∞¿EE¡E¿‘±E	
	static	void				ReceiveEventNotice(SERVER_PACKETS *_lpPacket);//¿∫Æ ∞¡Eπ¿Ω
	static	void				ReceiveNotice(SERVER_PACKETS *_lpPacket);//∞¡Eπ¿Ω
	static	void				ReceiveGetTitle(SERVER_PACKETS *_lpPacket);//™ æ¿Ω
	static	void				ReceiveRemoveTitle(SERVER_PACKETS *_lpPacket);//™ ¡¶∞
	static	void				ReceiveCancelQuest(SERVER_PACKETS *_lpPacket);//˘Ω∫Æ √E“

	static	void				ReceiveAnnounceQuizEvent(SERVER_PACKETS *_lpPacket);//	OX ˚¡EΩ√¿€ Ω√∞ ∞¡E	
	static	void				ReceiveBeginQuizEvent(SERVER_PACKETS *_lpPacket);	//	OX ˚¡EΩ√¿€
	static	void				ReceiveOXQuizResult(SERVER_PACKETS *_lpPacket);//	OX ˚¡E∞·∞E	
	static	void				ReceiveWordQuizResult(SERVER_PACKETS *_lpPacket);//≥π∏ª ˚¡E∞·∞E	
	static	void				ReceiveCountDownStart(SERVER_PACKETS *_lpPacket);//	´ø˚Æ øEΩ√¿€
	static	void				ReceiveQuizEventQuestion(SERVER_PACKETS *_lpPacket);//	OX ˚¡E¡˙πÆ π¿Ω
	static	void				ReceiveEndQuizEvent(SERVER_PACKETS *_lpPacket);//	OX ˚¡E¡æ∑E	
	static	void				ReceiveQuizEventLoserList(SERVER_PACKETS *_lpPacket);//OX ˚¡Eπ¿ ∏ÆΩ∫Æ
	static	void				ReceiveCancelQuizEvent(SERVER_PACKETS *_lpPacket);//OX ˚¡E√E“
	static	void				ReceiveOXQuizSurvivorCount(SERVER_PACKETS *_lpPacket);//OX ˚¡Eª˝¡∏¿ ºE	
	static	void				ReceiveSoundOfLeadersBell(SERVER_PACKETS *_lpPacket);//OX ˚¡Eª˝¡∏¿ ºE
	static	void				ReceiveReviveOXQuizLoserBattle(SERVER_PACKETS *_lpPacket);//OX ˚¡E¿∫Œ∞¿E	
	static	void				ReceiveOXQuizWinnerList(SERVER_PACKETS *_lpPacket);//OX ˚¡EΩ¿ ∏ÆΩ∫Æ π±E
	static	void				ReceiveBuyCarrotShopItemResult(SERVER_PACKETS *_lpPacket);//OX ˚¡EΩ¿ ∏ÆΩ∫Æ π±E	
	static	void				ReceiveBannerInfo(SERVER_PACKETS *_lpPacket);//	π≥ ¡§∫∏
	static	void				ReceiveBannerText(SERVER_PACKETS *_lpPacket);//	π≥ Ω∫Æ
	static	void				ReceiveBannerList(SERVER_PACKETS *_lpPacket);//	π≥ ∏ÆΩ∫Æ
	static	void				ReceiveBoostPowerByEvent(SERVER_PACKETS *_lpPacket);//	¿∫Æ
	static	void				ReceiveScreenMove(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveEventTimeUpdate(SERVER_PACKETS *_lpPacket);

//¶¨E∞E√
	static	void				ReceivePartingWithPet(SERVER_PACKETS *_lpPacket);//EEE˚E	
	static	void				ReceivePetInfo(SERVER_PACKETS *_lpPacket);//E¡§∫∏ æ¿Æ
	static	void				ReceiveChangePetCommand(SERVER_PACKETS *_lpPacket);//E∏˙”… ∫Ø∞E	
	static	void				ReceiveSyncPetInfo(SERVER_PACKETS *_lpPacket);	//Eª
	static	void				ReceivePetWaitCommand(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveMakeBreedingRecordBook(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveChangePetName(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveSummonBeastInfo(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveRidingDog(SERVER_PACKETS *_lpPacket);

//¶¨∞Ω¿ ¿∫√ ∞E√
	static	void				ReceiveGGGInfo(SERVER_PACKETS *_lpPacket);		//∞Ω¿ ¿∫√ ∞E√ª¿ª æ¿Æ .
	static	void				ReceiveGGG(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveRemoveGGG(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveGGGSkillUp(SERVER_PACKETS *_lpPacket);//∞Ω¿ ¿∫√ Ω∫≥æE∫∏≥Ω∫.
	static	void				ReceiveGGGSkillUpTimeEnd(SERVER_PACKETS *_lpPacket);//∞Ω¿ ¿∫√ Ω∫≥æE∫∏≥Ω∫ ≥°.
	static	void				ReceiveGGGEffectPlay(SERVER_PACKETS *_lpPacket);

//¶¨‘¡§ ∞E√
	static	void				ReceiveRemoveTrap(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveAddTrap(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveExplosionTrap(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveExplosionTrapSkill(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveExistTrapList(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveExistFireWallList(SERVER_PACKETS *_lpPacket);
	static	void				ReceiveExistActiveFieldTrapList(SERVER_PACKETS *_lpPacket);

//¶¨±∏
	static	void				ReceiveControlMonster(SERVER_PACKETS *_lpPacket);//
	static	void				ReceiveOccurEffect(SERVER_PACKETS *_lpPacket);//
//¶≠∞‘¿” º≠πE//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
};	//	class	cLOGIN_SERVER_PACKET_HANDLER	:	public CAgent

#define	dPACKET_BUFFER_SIZE		0xffff
//
//	∞‘¿”∞Eº“œ ∏Ω√¡∏¶ ø¨∞· ¡÷ ø°¿¿EÆ
class CAgent	:	public CPacketStatus,public cPACKET_HANDLER
{
	int					m_iGameServerPort;
	char				m_strGameServerIp[20];
	DWORD				m_dwLifecodeCycle;								//	ª˝¡∏-_-∏¶ ∫∏≥ª ¡÷±E	
	DWORD				m_dwLastLifecodeTime;								//	ª˝¡∏-_-∏¶ ∫∏≥ª ¡÷±E	
	cSOCKET				m_socket;
	cSOCKET				m_socketBCS;
	cQUE_PACKET			m_quePacket;									//	∂ 
	BYTE				m_aisReceivedPacket[dMAX_SERVER_PACKET_COUNT];	//	≥Æø ¿∫Æ
	BOOL				m_isActive;
	BOOL				m_isConnected;
	BOOL				m_bIsConnected2BCS;
	WORD				m_wIsUseSSL;
	BOOL				m_bIsWaitHandShakeData,m_bIsWaitSSLAuthenticationCode;
	DWORD				m_dwLastSendPacketTime;
	DWORD				m_dwLastSendChangePosPacket;
	WORD				m_wSendChangePosPacketCounter;
	WORD				m_wIsRequireReceivePacket;

	WORD				m_wIndexForBCS;
	char				m_strUserId[dID_LENGTH],m_strUserName[dNAME_LENGTH];

	BOOL				connect(HWND _hWnd,SOCKET _socket);
	BOOL				close(SOCKET _socket);
	BOOL				receive(SOCKET _socket);				//	∏Ω√¡∏¶ π.

public:

	int					m_iLimitConnectTime,m_iTryConnectTime;			//	º≠πø ø¨∞Eø¨∞E¡¶ Ω√∞,ø¨∞EΩ√∏¶  Ω√∞.
	BOOL				m_isEventDisconnect;			//	º≠πø°º≠ ∑.
	char				m_strDisconnectText[128];		//	ø÷ ∑≥.. -_-a
	char				m_aSSLSendBuffer[dPACKET_BUFFER_SIZE];
	char				m_aSSLReceiveBuffer[dPACKET_BUFFER_SIZE];

	DWORD				m_dwSendCheckReturnPacketSpeedTime,m_dwCurrentReturnPacketSpeed,m_dwReturnPacketSpeed,m_dwSumOfReturnPacketSpeed;
	WORD				m_wReturnPacketCount;
	WORD				m_wXorCode;
	int					m_iPacketSerial;
	BYTE				m_abEncTable[dENCRYPT_TABLE_SIZE];
	int					m_iGameServerEncTable;
	std::string			m_strMacAddress;
	char				m_strClientCode[12];

	SSLSESSION			m_ssl;

						CAgent();
						~CAgent();

	void				setOperateItemData(BOOL _bStatus);
	BOOL				isOperateItemData();
	BOOL				checkChangePosPacketSendCount();
	const	char*		getMacAddress();	
	void				setPlayerName(char *_lpstrId,char *_lpstrName);

	void				enableSSL()		{m_wIsUseSSL=TRUE;}
	void				disableSSL()	{m_wIsUseSSL=FALSE;}
	void				reducePacketTime(int *_iTime,int _iPeriod=dPOPS_DEFAULT);	//	∂ ∏¿” ¡¿±E
	void				initPacketOperator();
	BOOL				init(HWND _hWnd,DWORD _dwLifecodeCycle=dDEFAULT_LIFE_CODE_CYCLE);//	¿º” √±≠œ∞E∞¡æ ∂ ¡§¿
	void				sendLifeCode();							//	ª˝¡∏ E¿E€
	void				close();								//	¿º”¿ª æø°∞E. ±∏˚E.
	void				disconnectBroadCastServer();			//	∫EŒE≥Ω∫Æ º≠πE≤±E	
	BOOL				stackPacket(SERVER_PACKETS* _lpPacket);
	void				reset();								//	√±≠
	BOOL				connectToServer(char *_strIp,int _iPortNumber=0xffff,int _iLimitTime=20,BOOL _bIsUseSSL=FALSE);	//	º≠πø° ø¨∞·.
	BOOL				connectToGameServer();					//	∞‘¿” º≠πø° ø¨∞·.
	BOOL				connectToBroadCastServer(char *_lpstrIP,int _iPortNumber,int _iIndex);
	inline	void		blocking(){m_socket.blocking();}
	inline	void		releaseBlock(){m_socket.releaseBlock();}

	inline	BOOL		isActive(){return m_isActive;}			//	ªE¿÷≥?
	inline	BOOL		isConnected()
	{
		if	(!m_socket.isConnected())
			return	FALSE;

		return	m_isConnected;
	}
	inline	BOOL		isConnectedBCServer()
	{
		if	(!m_socketBCS.isConnected())
			return	FALSE;

		return	m_bIsConnected2BCS;
	}

	BOOL				isDisconnected();

	void				disconnect();							//	ø¨∞·¿ª ¡æ∑·.

	void				sendCheckPacketSpeedPacket();
	void				receiveCheckPacketSpeedPacket();

	int					getRemianConnectServerTime();			//	º≠πEø¨∞·±˚E≥≤¿∫ Ω√∞.
	int					getConnectServerResult();						//	º≠πEø¨∞E∞·∞E	
	void				receiveSSLAuthenticationCode()
	{
		m_bIsWaitSSLAuthenticationCode	=	FALSE;
	}

	BOOL				handleWindowMessage(HWND _hWnd,int _iEvent,SOCKET _socket);	//	∂ π.

	inline	SERVER_PACKETS*	popPacket(){return	m_quePacket.popPacket();}	//	π¿∫ ∂¿ª ∏Æœ~~
	BOOL				packetHandler();							//	∂ ∑Ø
	void				registerHandler(int _iPacketType,void (*_lpFp)(SERVER_PACKETS*));
//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠¿E€ π¿∫ ∂ ¡§∫∏
//¶≠
	inline	void	checkReceivedPacket(int _iPacket){m_aisReceivedPacket[_iPacket-dPACKET_INDEX_BEGIN]=TRUE;}//æ˚“≤ ∂¿ª πæ“¡Eº≥¡§.
	inline	BOOL	isReceivedPacket(int _iPacket){return m_aisReceivedPacket[_iPacket-dPACKET_INDEX_BEGIN];}//Ø¡§ ¿∫Æ∞° ¿œæE≥¡EÆ¿Œ ∫ª.
	inline	void	clearReceivedPacketCheck(){memset(m_aisReceivedPacket,0,sizeof(m_aisReceivedPacket));}//√±≠ .
	inline	BOOL	isStoredPacket(int _iPacket){return	m_quePacket.isReceivedPacket(_iPacket);}//æ¡E√≥∏Æ æ ∂¡ø° æE¿÷.
//¶≠
//¶≠¿E€ π¿∫ ∂ ¡§∫∏
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨


//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠¨∂¿æÆ->º≠πE∏Ω√¡E¡§¿
//¶≠
	BOOL			sendPacket(char *_lpPacketData,int _iSize);
	BOOL			sendBCSPacket(void *_lpPacket);

	//	º≠πø ¡¶EŒ ¡º”¿ ≥™ Æ¿Œœ±E¿ß ∏Ω√¡E≥Ø∏≤
	BOOL			sendDummyMessage();

//¶Æ¶¨CAgentSendPacket.cpp
//¶≤¶¨¿œπ/∞‘¿” ∞E√ ∂¿E€
	BOOL			sendClientType(int _iClientType);	//	¨∂¿æÆ ∏¿‘ ¿E€
	BOOL			sendLogIn(char *_strId,char *_strPass,char *_strSelectServer,DWORD _dwClientVersion,BOOL _isDisconnectExistID,BOOL _isPortal,BOOL _bIsUseTokken);//∑Œ±¿Œ
	BOOL			sendRemoveAvatar(char *_strName);//æπ∏ ¡øEE	BOOL			sendCreateAvatar(char *_strName,int _iJob);//æπ∏ ª˝º∫œ±E	
	OOL			sendSelectAvatar(char *_strName);//æπ∏ º±√œ±E	BOOL			sendCopySelectAvatarDataToTestServer(cAVATAR_INFO *_lpInfo);
	BOOL			sendCopySelectAvatarDataToDuelServer(cAVATAR_INFO *_lpInfo,int _iWorld);
	BOOL			sendConnectGame(char *_strID,char *_strAvatarName,int _iType);//∞‘¿” ø¨∞E	
	BOOL			sendJoinGame();//∞‘¿” E°
	BOOL			sendMove(int _iXPos=-1,int _iYPos=-1);//¿ø!!
	BOOL			sendReportCurrentPos();//¿E¿ß° ∫∏∞E	
	BOOL			sendStop(BOOL _bIsCheckChangePosPacketSendCount=TRUE);//¡§¡E!
	BOOL			sendSetMoveAbility();//¿ø æ˚÷∑Øº ∫Ø∞E	
	BOOL			sendTransformation();//∫ªΩÆ!! -o-!!
	BOOL			sendSitdown();//æ…±E	
	BOOL			sendAskInfoActorList(WORD *_lpAskInfoActorList,int _iAskInfoActorCount);//Ø¡§ æø° E ¡§∫∏ ø‰±∏
	BOOL			sendObscurityActorList(WORD *_lpObscurityActorList,int _iObscurityActorCount);//∫“∫∏˙ ≥∏Øø° E ¡§∫∏ ø‰√ª
	BOOL			sendMisplacedActorList(WORD *_lpMisplacedActorList,int _iMisplacedActorCount);//Ø¡§ æ¿ ¿ß° ¡§∫∏ ø‰√ª
	BOOL			sendAskActorEnchantInfoList(WORD *_lpwActorList,int _iActorCount);//Ø¡§ æ¿ ¿ß° ¡§∫∏ ø‰√ª
	
	BOOL			sendEngageBattle(int _iTarget,int _iAbility);//æø ¿EEΩ√¿€!!
	BOOL			sendQuickAction(int _iAbility);//_iTarget¿ª ∞∞!!

	BOOL			sendUseSkillToObject(int _iTargetType,int _iTarget,int _iAbility);
	BOOL			sendUseSkillToTarget(int _iTarget,int _iAbility,int _iEngageAbility,int _iSubAbility);//_iTarget¿ª ∞∞!!
	BOOL			sendUseSkillToGround(int _iX,int _iY,int _iAbility);//_iTarget¿ª ∞∞!!
	BOOL			sendStopBattle();
	BOOL			sendIncreaseState(int _iState,int _iPoint);//Ø¡§ Ω∫ º° æE!
	BOOL			sendIncreaseAbilityLevel(int _iAbility,int _iPoint,int _iAimLevel=0);//Ø¡§ æ˚÷∑Øº¿ ∑π∫ß æE!
	BOOL			sendAskActorInfo(int _iActor);//Ø¡§ ∏Ω∫ ¡§∫∏ ø‰√ª
	BOOL			sendFindActor(char *_lpstrName);

	BOOL			sendMoveField();	//	E¿ø “≤∏!!
	BOOL			sendEnterPortal(int _iPortal);
	BOOL			sendWarpToNearVillage(int _iVillage);
	BOOL			sendReturnToVillage(int _iWork);	//	∏∂¿ª∑Œ πæ∞°¿
	BOOL			sendSetReactionStatus(int _iAbility,BOOL _isOn);//	∏Ææº ª º≥¡§
	BOOL			sendTurnOnPassiveSkill(CG_TURN_ON_PASSIVE_SKILL *_lpPacket);
	BOOL			sendExitGame(int _iType);
	BOOL			sendDebugMessage(char *_lpstrMessage);
	BOOL			sendCallMagicCarpet();
	BOOL			sendChangeMagicCarpetShape(int _iShape);
	BOOL			sendRememberOrbLocation(int _iSlot);
	BOOL			sendTeleportToOrbLocation(int _iSlot);
	BOOL			sendAskRecoverDeathPenelty();

	BOOL			sendChangePetName(int _iIndex,char *_lpstrPetName,BOOL _bIsMiniPet=FALSE);
	BOOL			sendPartingWithPet(int _iIndex,int _iType=FALSE);

	BOOL			sendAskPlayerInfo(char *_lpstrName);
	BOOL			sendCheckHeroInfo(DWORD _dwCheckSum);
	BOOL			sendHackingLog(int _iCode,char *_lpName);
	BOOL			sendGGAuthData(void *_lpData);
	BOOL			sendEtcWork(int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
	BOOL			sendEtcWork(int _iWork,int _iCount,WORD *_lpwValue);
	BOOL			sendEtcWork(int _iWork,int _iCount,int *_lpiValue);
	BOOL			sendGuildMemberWork(int _iWork,char *_lpstrName,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);

//¶≤¶¨¿œπ/∞‘¿” ∞E√ ∂¿E€
//¶±¶¨CAgentSendPacket.cpp

//¶Æ¶¨CAgentSendPichmanShopPacket.cpp
//¶≤¶¨≥E°ªE∞E√
	BOOL			sendAskOpenThePitchmanShop();	//	≥E°ªEø≠∞∂∞Eø‰√ª
	BOOL			sendAddPitchmanShopItem(int _iSerial,int _iInventorySlot,int _iShopSlot,int _iPrice ,BYTE _bDenomination);		//	≥E°ªEæ¿€ √∞°		09.08.21
	BOOL			sendRemovePitchmanShopItem(int _iSerial,int _iSlot);			//	≥E°ªø°º≠ æ¿€ ¡¶∞
	BOOL			sendChangePitchmanShopInfo(int _iSerial,int _iStatus,char *_lpstrName,int _iSignboardShape,WORD _wTextColor,BOOL _bIsBold,BOOL _bIsShadowText);			//	≥E°ªEª ∫Ø∞E(ø¿,¨∑Œ¡E¡æ∑E
	BOOL			sendChangePitchmanShopItemPlace(int _iSerial,int _iSlot1,int _iSlot2);	//	≥E°ªø°º≠ æ¿€ ¿ß° ∫Ø∞E
	BOOL			sendBuyPitchmanShopItem(int _iSerial,int _iSlot,cItem *_lpItem,DWORD _dwPrice,BYTE _bDenomination);		//	≥E°ªø°º≠ æ¿€ ±∏¿‘	09.08.21
	BOOL			sendAskPitchmanShopInfo(int _iSerial);		//	≥E°ªE¡§∫∏ ø‰√ª
	BOOL			sendAskSimplePitchmanShopInfoList(WORD *_lpList,int _iCount);
	BOOL			sendPitchmanShopClose(int _iSerial);		//	≥E°ªE±E//¶≤¶¨≥E°ªE∞E√
//¶±¶¨CAgentSendPichmanShopPacket.cpp

//¶Æ¶¨CAgentSendItemPacket.cpp
//¶≤¶¨æ¿€ ∞E√
	BOOL			sendAskItemData();//	æ¿€ ¿∏¶ ø‰±∏
	BOOL			sendAskInventoryData();//	¿Œ∫‰∏Æ æ¿€ ¿∏¶ ø‰±∏
	BOOL			sendAskEquipmentData();//	¿∫Eæ¿€ ¿∏¶ ø‰±∏
	BOOL			sendRepairItem(int _iItemCheckSum,int _iSlot,int _iNpcSerial);
	BOOL			sendPickFieldItem();//	ø° æ¿€¿ª ¡¿.
	BOOL			sendEquipItem(int _iItemSlot,int _iPart,BOOL _bIsBeltItem=FALSE);//	æ¿€¿ª ¿∫œ¿
	BOOL			sendChangeItemPlace(int _iSrc,int _iDest);//	æ¿€¿ ¿ß°∏¶ π≤€.
	BOOL			sendStripEquipmentMessage(int _iPart);	//	¿∫Eπ˛±E∏Ω√¡E¿E€
	BOOL			sendDroppingItem(int _iPosX,int _iPosY,int _iItemSlot,int _iItemCount);//	æ¿€¿ ¿ß°∏¶ π≤€.
	BOOL			sendUseItemToCharacter(int _iItemSlot,int _iDest);//	æ¿€ ªøE	
	BOOL			sendUseItemToItem(int _iItemSlot,int _iDest);//	æ¿€ ªøE	
	BOOL			sendUseBeltItem(int _iBeltSlot,int _iTarget);//	∫ßÆ æ¿€ ªøE	
	BOOL			sendReloadBeltItem();//	∫ßÆ æ¿€ ∏Æ∑ŒE	
	BOOL			sendAskDestroyItem(int _iFocusItem);
	BOOL			sendAskItemPackName(int _iCount,WORD *_lpPackIndex);	//	æ¿€  ¿∏ß ø‰√ª
	BOOL			sendRemovePrefix(int _iUseItem,int _iDestItem,int _iPrefixIndex);

	BOOL			sendMergeItem(int _iItem,int _iClient);
	BOOL			sendJudgeItem(int _iItem,int _iClient);
	BOOL			sendUseGateGlove(int _iTargetField, BOOL _bIsNetCafeUser = FALSE,int _iItemSlot=-1);
//¶≤¶¨æ¿€ ∞E√
//¶±¶¨CAgentSendItemPacket.cpp

//¶Æ¶¨CAgentSendEventPacket.cpp
//¶≤¶¨¿∫Æ ∞E√
	BOOL			sendEventToTarget(int _iTarget);//¿∫Æ .
	BOOL			sendSelectSpeech(int _iSpeaker,int _iSpeechIndex,int _iSelectSpeech,int _iEventArea);
	BOOL			sendBuyItem(int _iShopeSerial,int _iOwner,int _iSelectItem,BOOL _bIsGuildShop,int _iCount=1 , BOOL _bIsTokkenShop = FALSE, BOOL _bIsBadgeOfTraderShop = FALSE);	// æ¿€±∏¿‘
	BOOL			sendSellItem(int _iShopSerial,int _iShopKeeper,int _iSellItemSlot,int _iSellItemCount, BOOL _bIsBadgeOfTraderShop = FALSE);//æ¿€ ∏
	BOOL			sendCancelQuest(int _iSelectQuest);
	BOOL			sendBuyCarrotShopItem(int _iCategory,int _iItemIndex);
	BOOL			sendChangeDoorStatus(int _iArea,int _iMethod);
	BOOL			sendAskDoorInfo(int _iArea);
	BOOL			sendTouchEventArea(int _iArea);

	BOOL			sendArcaWork(int _iArca,int _iWork);
	BOOL			sendDisarmFieldTrap(int _iTrap);
	BOOL			sendAskArcaInfo(int _iCount,WORD *_lpPackIndex);

	BOOL			sendPutBanner(int _iX,int _iY,int _iItemSlot,char *_lpstrTitle,char *_lpstrText);
	BOOL			sendAskBannerText(int _iSerial);
//¶≤¶¨¿∫Æ ∞E√
//¶±¶¨CAgentSendEventPacket.cpp

//¶Æ¶¨CAgentSendCommunityPacket.cpp
//¶≤¶¨øπœº ∞E√
	BOOL			sendChatMessage(char *_lpstrChat,int _iChatType,BOOL _bIsShout=FALSE);//±∫ª √§√
	BOOL			sendSayMessage(char *_lpstrName,char *_lpstrMessage);	//	ºº¿ ∏Ω√¡E∫∏≥ª±E
	BOOL			sendAskParyList();//º ∏ÆΩ∫Æ ø‰√ª
	BOOL			sendAskPartyInfo();//º ¡§∫∏ ø‰√ª
	BOOL			sendAskJoinParty(char *_lpstrName,BOOL _bIsJoinPartyApplication=FALSE , BOOL _bIsForUnionParty = FALSE, BOOL _bIsGuildMemberJoinParty = FALSE);//√E	
	BOOL			sendAskJoinPartyReply(char *_lpstrAskerName,WORD _wReply,BOOL _bIsReplyForJoinPartyApplication=FALSE , BOOL _bIsUnionPartyApplication = FALSE, BOOL _bIsGuildMemberJoinParty = FALSE);//º E° ø‰√ªø° ¿¿E	
	BOOL			sendAskLeaveParty();//º ªEø‰√ª
	BOOL			sendPartyLeaderWork(int _iWork,char *_lpstrName);//ºø°º≠ œæ≥ª±Eø‰√ª
	BOOL			sendChangePartyName(char *_lpstrName);//ºø°º≠ œæ≥ª±Eø‰√ª
	BOOL			sendChangePartyStatus(int _iGoldSharingMethod,int _iItemSharingMethod,int _iPartyObject,BOOL _bIsOpenParty);//º ª ∫Ø∞E	
	BOOL			sendAccuse(char *_lpstrAccuseName=NULL);
	BOOL			sendOperatorCommand(CG_OPERATOR_COMMAND *_lpPacket);

	BOOL			sendRequestAddFriend(char *_lpstrTarget,BOOL _bIsRecoverList=FALSE,int _iServerSign=0,int _iJob=0);
	BOOL			sendRemoveMyNameInTargetFriendList(char *_lpstrTarget);
	BOOL			sendRemoveMyNameInTargetFriendListResult(char *_lpstrClient,int _iResult);
	BOOL			sendReplyAddFriend(int _iReply,char *_lpstrTarget);

	BOOL			sendChangCommunityStatus(BOOL _bIsDenyCommunity,BOOL _bIsHidePitchmanShop);
//¶≤¶¨øπœº ∞E√
//¶±¶¨CAgentSendCommunityPacket.cpp

//¶Æ¶¨CAgentSendGuildPacket.cpp
//¶≤¶¨±E∞E√
	BOOL			sendMakeGuild(char *_lpstrGuildName,char *_lpstrSubGuildManter);
	BOOL			sendAskGuildInfo(BOOL _bIsIgnoreSkill);
	BOOL			sendAskGuildMemberList(BOOL _bISInit = FALSE);	//±E∏πE∏ÆΩ∫Æ ø‰√ª
	BOOL			sendRequestJoinGuild(int _iTargetSerial);
	BOOL			sendReplyJoinGuild(int _iReply,char *_lpstrName,int _iSerial);
	BOOL			sendIncreaseGuildSkillLevel(int _iSkill);	//	±EΩ∫≥∑π∫ß ø√∏Æ±E	
	BOOL			sendAskGuildBattleInfoByTimeOrder(int _iTimeOrder);
	BOOL			sendRequestGuildBattle(int _iTimeOrder,int _iFieldOrder);	//	±¿EΩ√ª
	BOOL			sendCancelGuildBattle(int _iTimeOrder=0);	//	±¿E√E“
	BOOL			sendCloseGuildBattleScheduler();	//	±¿EΩ√ª
	BOOL			sendSelectGuildMark(CGuildMarkInfo *_lpMarkInfo);	//	±∏∂ º±√
	BOOL			sendAskGuildMarkInfo(int _iGuildSerial);	//	±∏∂ ¡§∫∏ ø‰√ª

	BOOL			sendChangeGuildNotice(char *_lpstrNotice);
	BOOL			sendChangeGuildTax(int _iTax);
	BOOL			sendDismissSubGuildMaster(char *_lpstrName);
	BOOL			sendAppointmentSubGuildMaster(char *_lpstrName);
	BOOL			sendAppointmentGuildCongress(char *_lpstrName);
	BOOL			sendDismissGuildCongress(char *_lpstrName);
	BOOL			sendRegistRegularGuildMember(char *_lpstrName);
	BOOL			sendBanishGuildMember(char *_lpstrName);
	BOOL			sendChangeGuildMaster(char *_lpstrName);
	BOOL			sendLeaveGuild();
	BOOL			sendDissolutionGuild();
	BOOL			sendReadyToMakeGuild();
	BOOL			sendGuildCheckSum(int _iCheckSum);
//¶≤¶¨±E∞E√
//¶±¶¨CAgentSendGuildPacket.cpp

//¶Æ¶¨CAgentSendBankPacket.cpp
//¶≤¶¨Æ∑π¿E∞E√
	BOOL			sendRequestTrade(char *_lpstrName,WORD _wSerial);	//	CG_REQUEST_TRADE		Æ∑π¿Eø‰√ª
	BOOL			sendTradeReply(char *_lpstrName,WORD _wReply);		//	CG_TRADE_REPLY			Æ∑π¿E¿¿E	
	BOOL			sendCancelTrade();									//	CG_CANCEL_TRADE			Æ∑π¿E√E“
	BOOL			sendAddTradeItem(int _iItemSlot,int _iItemCount);	//	CG_ADD_TRADE_ITEM		Æ∑π¿Eæ¿€ √∞°
	BOOL			sendChangeTradeItemData(int _iChangeTradeSlot,int _iItemSlot,int _iItemCount);	//	CG_CHANGE_TRADE_ITEM_DATA		Æ∑π¿Eæ¿€ ¡§∫∏ ∫Ø∞E	
	BOOL			sendRemoveTradeItem(int _iItemSlot);				//	CG_REMOVE_TRADE_ITEM	Æ∑π¿Eæ¿€ ¡¶∞
	BOOL			sendSetTradeGold(int _iGold);						//	CG_SET_TRADE_GOLD		Æ∑π¿E“ ∞Eº≥¡§
	BOOL			sendReadyTrade();									//	CG_READY_TRADE			Æ∑π¿E¡∫E	
	BOOL			sendPermitTrade(int _iCheckSum);					//	CG_PERMIT_TRADE			Æ∑π¿E∂E//¶≤¶¨Æ∑π¿E∞E√
//¶±¶¨CAgentSendBankPacket.cpp

//¶Æ¶¨CAgentSendCartPacket.cpp
//¶≤¶¨º˚Œ ´Æ ∞E√
	BOOL			sendAskOpenShoppingCart();
	BOOL			sendCloseShoppingCart();
	BOOL			sendWithdrawItemFromTheCart(int _iCartSerial,int _iCartSlot,int _iInventorySlot,DWORD _dwCheckSum);
	BOOL			sendDestroyCartItem(int _iCartSerial,int _iCartSlot,DWORD _dwCheckSum);
	BOOL			sendOpenCarrotShop(DWORD _dwCheckSum);
//¶≤¶¨º˚Œ ´Æ ∞E√
//¶±¶¨CAgentSendCartPacket.cpp


//¶Æ¶¨CAgentSendBankPacket.cpp
//¶≤¶¨¿∫E∞∑° ∞E√
	BOOL			sendStoreItemToTheBank(int _iBankSerial,int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum);
	BOOL			sendStoreGoldToTheBank(int _iBankSerial,int _iGold,DWORD _dwCheckSum);
	BOOL			sendWithdrawItemFromTheBank(int _iBankSerial,int _iInventorySlot,int _iBankSlot,DWORD _dwCheckSum);
	BOOL			sendWithdrawGoldFromTheBank(int _iBankSerial,int _iGold,DWORD _dwCheckSum);
	BOOL			sendMoveBankItem(int _iBankSerial,int _iSlot1,int _iSlot2,DWORD _dwCheckSum);
	BOOL			sendMoveInventoryItemWhenBankTransaction(int _iBankSerial,int _iSlot1,int _iSlot2,DWORD _dwCheckSum);
	BOOL			sendCancelBankTransaction();
	BOOL			sendFinishBankTransaction(int _iBankSerial,DWORD _dwCheckSum);
//¶≤¶¨¿∫E∞∑° ∞E√
//¶±¶¨CAgentSendBankPacket.cpp

//¶≠
//¶≠¨∂¿æÆ->º≠πE∏Ω√¡E¡§¿
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨


//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠¡§¿E∏πE//¶≠
	static	void	(*fpPacketHandler[dMAX_SERVER_PACKET_COUNT])(SERVER_PACKETS*);
//¶≠
//¶≠¡§¿E∏πE//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
};	//	class CAgent


#endif