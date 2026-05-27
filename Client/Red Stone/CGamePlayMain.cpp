#include "CGamePlay.H"
#include "cMAP.H"
#include "CSkill.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "cSHAKE.H"
#include "CFieldItem.H"
#include "cMESSAGE.H"
#include "CAgent.H"
#include "CHero.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CSound.H"
#include "CLog.h"
#include "CBank.h"
#include "CWindowInterface.H"
#include "CGuild.H"
#include "CShoppingCart.H"
#include "CEvent.H"
#include "CGuardianPost.H"
#include "CPitchmanShop.H"
#include "CPortal.H"
#include "trap.h"
#include "arca.h"
#include "banner.h"
#include "fireCracker.h"
#include "bm_event.h"
#include "dialog_box.h"
#include "duel.H"
#include "commonUI.H"
#include "book_MonsterDictionary.h"

BOOL		CGamePlay::s_bIsIgnoreAllFocus			=	FALSE;
BOOL		CGamePlay::s_bIsBookedDisableChattingBox=	FALSE;
CTimeInfo	CGamePlay::s_currentTime;
CTimeInfo	CGamePlay::s_serverTime;
DWORD		CGamePlay::s_dwReceiveServerTime;

DWORD		CGamePlay::s_dwRank12AgeTime = 0;
DWORD		CGamePlay::s_dwLoginTime = 0;
cKarmaDrawImage	CGamePlay::m_karmaDrawImage[dMAX_KARMA_DRAW_IMAGE];
cEventActiveManager		CGamePlay::s_eventActiveManager;
//
//	 초기화
void
CGamePlay::Init()
{
	if	(!s_sbPopupWindow.m_lpSprite)	//	초기화가 안되었으면 초기화 해라
		s_sbPopupWindow.init(&g_sprInterface,dIMAGE_SCROLL_BAR_VERTICAL,
											eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_UP_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_UP_BUTTON,0xffff,
											eSCROLLBAR_VERTICAL_NORMAL_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_DOWN_BUTTON,0xffff,
											eSCROLLBAR_VERTICAL_NORMAL_BODY_TOP,	eSCROLLBAR_VERTICAL_NORMAL_BODY,		eSCROLLBAR_VERTICAL_NORMAL_BODY_BOTTOM,
											eSCROLLBAR_VERTICAL_ACTIVE_BODY_TOP,	eSCROLLBAR_VERTICAL_ACTIVE_BODY,		eSCROLLBAR_VERTICAL_ACTIVE_BODY_BOTTOM);
	if	(!s_sbChatBox.m_lpSprite)	//	초기화가 안되었으면 초기화 해라
		s_sbChatBox.init(&g_sprInterface,dIMAGE_SCROLL_BAR_VERTICAL,
											eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_UP_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_UP_BUTTON,0xffff,
											eSCROLLBAR_VERTICAL_NORMAL_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_DOWN_BUTTON,0xffff,
											eSCROLLBAR_VERTICAL_NORMAL_BODY_TOP,	eSCROLLBAR_VERTICAL_NORMAL_BODY,		eSCROLLBAR_VERTICAL_NORMAL_BODY_BOTTOM,
											eSCROLLBAR_VERTICAL_ACTIVE_BODY_TOP,	eSCROLLBAR_VERTICAL_ACTIVE_BODY,		eSCROLLBAR_VERTICAL_ACTIVE_BODY_BOTTOM);
	if	(!s_sbMiddleWindow.m_lpSprite)	//	초기화가 안되었으면 초기화 해라
		s_sbMiddleWindow.init(&g_sprInterface,dIMAGE_SCROLL_BAR_VERTICAL,
											eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_UP_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_UP_BUTTON,0xffff,
											eSCROLLBAR_VERTICAL_NORMAL_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_DOWN_BUTTON,0xffff,
											eSCROLLBAR_VERTICAL_NORMAL_BODY_TOP,	eSCROLLBAR_VERTICAL_NORMAL_BODY,		eSCROLLBAR_VERTICAL_NORMAL_BODY_BOTTOM,
											eSCROLLBAR_VERTICAL_ACTIVE_BODY_TOP,	eSCROLLBAR_VERTICAL_ACTIVE_BODY,		eSCROLLBAR_VERTICAL_ACTIVE_BODY_BOTTOM);
  	// 10.02.16
  	if	(!s_sbStatusWindow.m_lpSprite)	//	초기화가 안되었으면 초기화 해라
  		s_sbStatusWindow.init(&g_sprInterface,dIMAGE_SCROLL_BAR_VERTICAL,
  											eSCROLLBAR_VERTICAL_NORMAL_UP_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_UP_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_UP_BUTTON,0xffff,
  											eSCROLLBAR_VERTICAL_NORMAL_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_ACTIVE_DOWN_BUTTON,	eSCROLLBAR_VERTICAL_PRESSED_DOWN_BUTTON,0xffff,
  											eSCROLLBAR_VERTICAL_NORMAL_BODY_TOP,	eSCROLLBAR_VERTICAL_NORMAL_BODY,		eSCROLLBAR_VERTICAL_NORMAL_BODY_BOTTOM,
  											eSCROLLBAR_VERTICAL_ACTIVE_BODY_TOP,	eSCROLLBAR_VERTICAL_ACTIVE_BODY,		eSCROLLBAR_VERTICAL_ACTIVE_BODY_BOTTOM);

	s_rectInterfaceSize.set(0, 0, 0, 0);
}	//	CGamePlay::Init()

//
//	리셋
void
CGamePlay::Reset()
{
	Init();

	InitNoticeBox();		//	공지 초기화
	cINPDEV::ReleaseExclusive();

	s_lpInventoryWindowItemList		=	g_hero.m_aItems;
	s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;

	s_iFrameCounter		=	0;
	s_propose.m_dwValue	=	0;
	s_bIsProgressEvent	=	FALSE;

	s_fieldTitle.m_iState=	eFIELD_TITLE_SLEEP;
	s_UpgradeFieldTitle.m_iState=	eFIELD_TITLE_SLEEP;				// 09.09.02
	g_hem.reset();
	g_eventManager.init();
	g_pitchmanShop.reset();
	g_portal.reset();
	g_trapManager.reset();
	g_arcaManager.reset();
	g_bannerManager.reset();
	g_guildBattleInfo.reset();
	g_fireCrackerManager.reset();
	g_bossMonsterEvent.close();

	if	(s_isEnterenceGame)	//	게임에 들어가는거다.
	{
		cItem::Reset();

	//	메모리 초기화
		{
			s_mcuData.reset();
			s_mcuFieldImage.reset();
			s_mcuBodyImage.reset();
		}	//	메모리 초기화

		g_carrotShop.load();
		s_bmBottomMenu.close();

		InitBottomInterface();	//	하단 인터페이스 초기화
		InitChattingBox();	//	시스템 메시지 박스 
		InitSystemMessageBox();	//시스템 메시지 박스
		InitMinimap();

		s_bmMiddleWindowButton.init(&g_sprInterface,25);//하단 메뉴
		s_bmPlusState.init(&g_sprInterface,100);

		g_gwSkill.init();
		g_gwCommonList.init();
		g_commonUI.init(&g_sprInterface);

		s_bmCarrotShop.init(&g_sprRegionInterface,20);
		s_bmButton.init(&g_sprInterface,20);
		s_bmButtonI2.init(&g_sprInterface2,20);
		s_bmToolButton.init(&g_sprInterface,10);
		// 10.02.22
		s_bmStatusButton.init(&g_sprInterface,20);
		s_bmMenu.init(&g_sprInterface,10);
		s_bmRadioBox.init(&g_sprInterface,10,dBAR_RADIO,eBTOM_BAR_RIGHT);
		s_bmCategory.init(&g_sprInterface, 3,dBAR_RADIO,eBTOM_BAR_RIGHT);
		s_bIsMoveFieldAtSameServer	=	FALSE;

		g_esm.setActive(g_config.m_bIsSoundOn);
		g_guild.reset();
		g_am.resetDenyCommunityActorList();
		g_hero.setIsRequireFullInfo(TRUE);
		g_hero.reset();

#ifndef _DEBUG
		if (g_bIsTestServer)
			CGamePlay::Notice(dMSG_TEST_SERVER_NOTICE);
#endif
	}

	s_iPlayBgmIndex			=	0;
	g_iRemainGuildBattleTime=	-1;

	g_logChat.reset();
	g_shaker.close();
	g_rapeEffect.close();
	g_standEffect.close();
	g_floatEffect.close();
	g_bank.reset();
	g_shoppingCart.close();
	g_gwPitchmanShop.m_shop.reset();

	if	(s_partyInfo.m_wMemberCount)
		s_partyInfo.m_bIsExistPreviousField	=	TRUE;
	else
		s_partyInfo.m_bIsExistPreviousField	=	FALSE;

	s_partyInfo.reset();

	s_tradeManager.reset();
	s_agent.CPacketStatus::reset();

	g_fieldItem.reset();
	g_am.reset();	//액터 초기화
	g_sm.reset();	//	스킬 초기화
	s_pmCommon.reset();
	g_gpManager.reset();

	s_rectMinimapFrame.x1	=	0xffff;	//	미니맵 사이즈 재설정을 위해..
	s_rectInterfaceSize.set(0, 0, 0, 0);
	s_iPopupInterface		=	ePIW_NONE;	//현재 팝업 되어 있는 인터페이스
	s_iDragEquipment		=	0xffff;	//	드래그 중인 장비
	s_iDragItem				=	0xffff;	//	드래그 중인 아이템
	s_iDragSkill			=	0xffff;	//	드래그 중인 기술

	g_dialogBox.reset();

	s_tlSystem.reset();
	s_inputChatting.disable(TRUE);
	s_ttCommon.setEnable(g_config.m_wIsTooltipOn);

	s_isEnterenceGame		=	FALSE;

	for(int i=0;i<dMAX_KARMA_DRAW_IMAGE;++i)
		m_karmaDrawImage[i].reset();

	s_eventActiveManager.reset();
}



//
//
void
CGamePlay::UpdateDebugMenu()
{
}

//
//	메시지 박스 처리
void
CGamePlay::OperateMessageBox()
{
//	메시지 박스 메시지 처리
	if	(s_isOperatedMsgbox)
	{
		s_iDragTradeItem	=	0xffff;
		s_iDragEquipment	=	0xffff;
		s_iDragItem			=	0xffff;
		s_iDragSkill		=	0xffff;

		cINPDEV::SetExclusive(eMOUSE_STATE_SYSTEM_DIALOG);

		if	(g_msgBox.isNothing())
			return;

		int	iResult	=	g_msgBox.getResult();

		g_msgBox.close();

		switch(iResult)
		{
			case	eMBI_ACTIVE_GUILD_MASTER_GRACE			:
				s_agent.sendEtcWork(eCEW_ACTIVE_GUILD_MASTER_GRACE);

				break;
			case	eMBI_START_GUILD_DUNGEN					:
				s_agent.sendEtcWork(eCEW_TRY_TO_GUILD_DUNGEN , g_guildManager.m_wPlayLevelForGuildDungen);
				break;

			case	eMBI_CANCEL_TRY_DUEL					:
			{
				s_agent.sendEtcWork(eCEW_CANCEL_TRY_DUEL);
				g_duelManager.reset();
				break;
			}

			case	eMBI_USE_ITEM_TO_ITEM					:
			{
				s_agent.sendUseItemToItem(g_hero.m_wUseItem,g_hero.m_wTargetItem);
				break;
			}

			case	eMBI_FEED_ITEM_TO_MINIPET		:
			{
				s_agent.sendEtcWork(eCEW_FEED_ITEM_TO_MINIPET,g_gwMiniPetStatus.m_iSelectMiniPet,g_gwMiniPetStatus.m_iFeedItem);
				break;
			}
			case	eMBI_FEED_ALL_ITEM_TO_MINIPET	:
			{
				s_agent.sendEtcWork(eCEW_FEED_ALL_ITEM_TO_MINIPET,g_gwMiniPetStatus.m_iSelectMiniPet,g_gwMiniPetStatus.m_iFeedType , FALSE);
				break;
			}
			case	eMBI_ADD_RELATED_PLACE_OF_GUILD	:
			{
				s_agent.sendEtcWork(eCEW_ADD_RELATED_PLACE_OF_GUILD,g_guild.m_wRelatedPlaceOfGuildForNewPlace);
				break;
			}
			case	eMBI_FEED_ITEM_TO_GUILD_PET_OR_GUILD_GUARDIAN		:
			{
				s_agent.sendEtcWork(eCEW_FEED_ITEM_TO_GUILD_PET,g_gwFeedGuildPet.m_iSelectPet,g_gwFeedGuildPet.m_iFeedItem);
				break;
			}

			case	eMBI_RETURN_TO_THE_GUILD_HALL_BY_END_GUILD_POINT_BATTLE	:
			{
				s_agent.sendEtcWork(eCEW_RETURN_TO_THE_GUILD_HALL_BY_END_GUILD_POINT_BATTLE);
				break;
			}

			case	eMBI_RETURN_TO_GUILD_HALL_BY_END_BATTLE	:
			{
				s_agent.sendEtcWork(eCEW_RETURN_TO_GUILD_HALL_BY_END_BATTLE);
				break;
			}
			case	eMBI_TRY_GUILD_POINT_BATTLE			:
			{
				s_agent.sendEtcWork(eCEW_TRY_TO_GUILD_POINT_BATTLE);
				break;
			}
			case	eMBI_MOVE_TO_SELECT_GUILD_HALL		:
			{
				g_gwGuildHallList.moveToSelectGuildHall();
				break;
			}

			case	eMBI_MOVE_TO_SELECT_RELATED_PLACE	:
			{
				g_gwGuildRelatedPlaceList.moveToSelectPlace();
				break;
			}

			case	eMBI_MAKE_GUILD_MARK				:
			{
				g_hero.m_bf1IsWaitGuildCommandResult	=	TRUE;
				s_agent.sendSelectGuildMark(&g_gwGuildMarkComposer.m_guildMarkInfo);
				break;
			}

			case	eMBI_REMOVE_TITLE					:
			{
				CPlayerTitleInfo	*lpTitleInfo	=	g_hero.getTitleByIndex(s_iSelectTitle);

				if	(!lpTitleInfo)
					break;

				s_agent.sendEtcWork(eCEW_REMOVE_TITLE,lpTitleInfo->m_bTitle);
				break;
			}
			case	eMBI_BUY_PIGEON_SHOP_ITEM			:
			{
				s_agent.sendEtcWork(eCEW_BUY_PIGEON_POST_ITEM,g_gwShop.m_iSelectShopItem);
				g_gwShop.close();
				break;
			}
			case	eMBI_MERGE_SELECT_ITEM				:
			{
				s_agent.sendMergeItem(g_gwMergeItem.getSelectItem(),g_gwMergeItem.m_iClientSerial);
				break;
			}

			case	eMBI_USE_GATE_GLOVE					:
			{
				if	(g_hero.m_wRestraintTimeByBattle)
				{
					char	strMessage[256];
					
					int	iRemainTime	=	max(1,g_hero.m_wRestraintTimeByBattle);
					
					sprintf(strMessage,dMSG_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE_FORM,iRemainTime);
					
					CGamePlay::WarningMessage(strMessage);
					return;
				}

				s_agent.sendUseGateGlove(g_gwWorldMap.m_iSelectFieldSerial , g_hero.m_bf1IsNetCafeUser,g_gwWorldMap.m_iUseItemSlotIndex);
				break;
			}

			case	eMBI_JUDGE_TO_SELECT_ITEM			:
			{
				s_agent.sendJudgeItem(g_gwJudgeToItem.getSelectItem(),g_gwJudgeToItem.m_iClientSerial);
				break;
			}

			case	eMBI_REMOVE_SELECT_PREFIX			:
			{
				s_agent.sendRemovePrefix(g_gwRemovePrefix.m_iSrcItem,g_gwRemovePrefix.m_iDestItem,g_gwRemovePrefix.m_iSelectPrefix);
				break;
			}

			case	eMBI_RECOVER_DEATH_PENELTY			:
			{
				s_agent.sendAskRecoverDeathPenelty();
				break;
			}

			case	eMBI_RETURN_TO_TITLE				:	//	타이틀로 돌아가자
			{
				s_agent.reset();	//	연결 바로 끊자!!
				g_msgBox.close();
				BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
				break;
			}

			case	eMBI_RETURN_TO_TITLE_BY_CANCEL		:	//	타이틀로 돌아가자
			{
				s_agent.reset();	//	연결 바로 끊자!!
				g_msgBox.close();
				BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
				s_bIsFailedToJoinGame	=	TRUE;
				break;
			}

			case	eMBI_CANCEL_TO_ENTER_GAME			:	//	게임 참가 취소
			case	eMBI_REALLY_CANCEL_TO_MOVE_FIELD	:
			{
				s_agent.reset();	//	연결 바로 끊자!!
				BookingMode(eGAME_TITLE,dSTEP_CLOSE,dFADE_OUT);
				break;
			}

			case	eMBI_CANCEL_TO_MOVE_FIELD			:	//	게임 참가 취소
			{
				g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_CANCEL_TO_MOVE_FIELD,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_REALLY_CANCEL_TO_MOVE_FIELD,eMBI_CANCEL_TO_CANCEL_TO_MOVE_FIELD);
				g_msgBox.setWaitPacket(dSG_MOVE_FIELD_NOW);
				return;
			}

			case	eMBI_CANCEL_TO_CANCEL_TO_MOVE_FIELD	:
			{
				g_msgBox.cPopup("",dMSG_WAIT_FIELD_MOVE_RESULT,dMSG_CANCEL);
				g_msgBox.setButtonId(eMBI_CANCEL_TO_MOVE_FIELD);
				g_msgBox.setWaitPacket(dSG_MOVE_FIELD_NOW);
		
				return;
			}

			case	eMBI_PERMIT_TRADE					:
				s_agent.sendPermitTrade(s_tradeManager.getCheckSum());
				break;

			case	eMBI_ACCEPT_JOIN_PARTY_APPLICATION	:
				s_agent.sendAskJoinPartyReply(s_strPartyJoinAskerName,1,TRUE);
				break;

			case	eMBI_DECLINE_JOIN_PARTY_APPLICATION	:
				s_agent.sendAskJoinPartyReply(s_strPartyJoinAskerName,0,TRUE);
				break;

			case	eMBI_ACCEPT_JOIN_PARTY_REQUEST		://	파티 참가 제안을 받아 들인다.
				s_agent.sendAskJoinPartyReply(s_strPartyJoinAskerName,1);
				break;
			case	eMBI_DECLINE_JOIN_PARTY_REQUEST		://	파티 참가 제안을 거절한다.
				s_agent.sendAskJoinPartyReply(s_strPartyJoinAskerName,0);
				break;

			case	eMBI_ACCEPT_JOIN_UNION_PARTY_REQUEST		://	연합 파티 참가 제안을 받아 들인다.
				s_agent.sendAskJoinPartyReply(s_strPartyJoinAskerName,1, FALSE, TRUE);
				break;
			case	eMBI_DECLINE_JOIN_UNION_PARTY_REQUEST		://	연합 파티 참가 제안을 거절한다.
				s_agent.sendAskJoinPartyReply(s_strPartyJoinAskerName,0, FALSE, TRUE);
				break;

			case	eMBI_ACCEPT_TRADE_REQUEST		://거래 요청을 받아 들인다.
				s_agent.sendTradeReply(s_strRequestTradeAskerName,eRTR_OK);
				break;
			case	eMBI_DECLINE_TRADE_REQUEST		://거래 요청을 거부한다.
				s_agent.sendTradeReply(s_strRequestTradeAskerName,eRTR_DENY);
				break;

			case	eMBI_LEAVE_PARTY				:
				s_agent.sendAskLeaveParty();
				break;

			case	eMBI_REMOVE_FRIEND				:
			{
				cFriendInfoDefine	*lpInfo	=	g_hero.getFriend(g_gwFriend.m_iSelectFriend);

				if	(lpInfo)
				{
					g_hero.removeFriendByName(lpInfo->m_strName);
					g_gwFriend.resetFriendList();
				}
				break;
			}

			case	eMBI_CANCEL_SELECT_QUEST		:
				s_agent.sendCancelQuest(g_gwQuest.m_iSelectQuest);
				break;

			case	eMBI_CHANGE_PREFIX_BY_20031117	:
				s_agent.sendUseItemToItem(g_hero.m_wUseItem,g_hero.m_wTargetItem);
				break;

			case	eMBI_BANK_TRANSACTION_PERMIT	:
				g_gwBank.m_wIsWaitSaveResult	=	TRUE;
				s_agent.sendFinishBankTransaction(g_bank.m_wBankSerial,g_bank.getCheckSum());
				break;

			case	eMBI_ACCEPT_ADD_FRIEND_REQUEST	:
				s_agent.sendReplyAddFriend(eRFA_OK,g_gwFriend.m_strAddFriendAskerName);
				break;

			case	eMBI_DECLINE_ADD_FRIEND_REQUEST	:
				s_agent.sendReplyAddFriend(eRFA_DENY,g_gwFriend.m_strAddFriendAskerName);
				break;

			case	eMBI_ACCEPT_JOIN_GUILD_REQUEST	:
				s_agent.sendReplyJoinGuild(eAJGR_ACCEPT,g_gwGuild.m_strJoinGuildAsker,g_gwGuild.m_wJoinGuildAskerSerial);
				break;

			case	eMBI_DECLINE_JOIN_GUILD_REQUEST	:
				s_agent.sendReplyJoinGuild(eAJGR_DENY,g_gwGuild.m_strJoinGuildAsker,g_gwGuild.m_wJoinGuildAskerSerial);
				break;

			case	eMBI_EXIT_TO_WINDOW_BY_FOCED	:	
				AskExitGame(eEGV_EXIT_TO_WINDOW,TRUE,TRUE);
				break;
				
			case	eMBI_RETURN_TO_TITLE_BY_FOCED	:
				AskExitGame(eEGV_RETURN_TO_TITLE,TRUE,TRUE);
				break;

			case	eMBI_RETURN_TO_TITLE_BY_LOGOUT	:
				AskExitGame(eEGV_RETURN_TO_TITLE,TRUE);
				break;

			case	eMBI_EXIT_TO_WINDOW				:
				AskExitGame(eEGV_EXIT_TO_WINDOW,TRUE);
				break;

			case	eMBI_BANISH_MEMBER				:
			{
				char	*lpstrName	=	s_partyInfo.getSelectMemberName();

				if (!lpstrName)
					break;

				s_agent.sendPartyLeaderWork(ePLW_BANISH_MEMBER,lpstrName);
				break;
			}

			case	eMBI_CHANGE_PARTY_LEADER			:
			{
				char	*lpstrName	=	s_partyInfo.getSelectMemberName();

				if (!lpstrName)
					break;

				s_agent.sendPartyLeaderWork(ePLW_CHANGE_LEADER,lpstrName);
				break;
			}

			case	eMBI_CHANGE_GUILD_EXP_TAX			:
				s_agent.sendChangeGuildTax(s_iBookedChangeGuildTax);
				break;

			case	eMBI_DISPLAY_NAME_WITH_SOUND_OF_LEADERS_BELL	:
				g_hero.m_bf1IsDisplayNameWithSoundOfLeadersBell	=	TRUE;
				g_hero.askUseSoundOfLeadersBell();
				break;

			case	eMBI_NOT_DISPLAY_NAME_WITH_SOUND_OF_LEADERS_BELL	:
				g_hero.m_bf1IsDisplayNameWithSoundOfLeadersBell	=	FALSE;
				g_hero.askUseSoundOfLeadersBell();
				break;

			case	eMBI_USE_SOUND_OF_LEADERS_BELL	:
				g_hero.useSoundOfLeadersBell(FALSE);
				break;

			case	eMBI_CHANGE_GUILD_NOTICE			:
				s_agent.sendChangeGuildNotice(g_guild.m_strBookedGuildNotice);
				break;

			case	eMBI_DISMISS_SUB_GUILD_MASTER		:
				s_agent.sendDismissSubGuildMaster(s_strBookedUserName);
				break;

			case	eMBI_APPOINTMENT_SUB_GUILD_MASTER	:
				s_agent.sendAppointmentSubGuildMaster(s_strBookedUserName);
				break;

			case	eMBI_APPOINTMENT_GUILD_CONGRESS		:
				s_agent.sendAppointmentGuildCongress(s_strBookedUserName);
				break;

			case	eMBI_DISMISS_GUILD_CONGRESS			:
				s_agent.sendDismissGuildCongress(s_strBookedUserName);
				break;


			case	eMBI_REGIST_REGULAR_MEMBER			:
				s_agent.sendRegistRegularGuildMember(s_strBookedUserName);
				break;

			case	eMBI_REPAIR_ITEM					:
			{
				int	iItemSlot	=	g_gwRepairItem.m_aiBreakDownItemList[g_gwRepairItem.m_iSelectItem];

				s_agent.sendRepairItem(g_hero.getItemCheckSum(),iItemSlot,g_gwRepairItem.m_iNpcSerial);
				break;
			}

			case	eMBI_BANISH_GUILD_MEMBER			:
				s_agent.sendBanishGuildMember(s_strBookedUserName);
				break;

			case	eMBI_CHANGE_GUILD_MASTER			:
				s_agent.sendChangeGuildMaster(s_strBookedUserName);
				break;

			case	eMBI_PERMIT_CART_WORK				:
				s_agent.sendWithdrawItemFromTheCart(g_shoppingCart.m_wSerial,g_shoppingCart.m_iBookedCartItem,g_shoppingCart.m_iBookedInventorySlot,g_shoppingCart.getCheckSum());
				break;

			case	eMBI_UNLOCK_DOOR_BY_USE_KEY			:
				s_agent.sendChangeDoorStatus(g_hero.m_wWorkDoor,eCDSM_UNLOCK_DOOR_BY_KEY);
				break;

			case	eMBI_UNLOCK_ARCA_BY_USE_KEY			:
				s_agent.sendArcaWork(g_arcaManager.m_wWorkArca,eAAW_UNLOCK_BY_USE_KEY);
				break;
				

			case	eMBI_DOOR_DISARM_TRAP				:
				s_agent.sendChangeDoorStatus(g_hero.m_wWorkDoor,eCDSM_DISARM_DOOR);
				break;

			case	eMBI_DESTROY_CART_ITEM				:
				s_agent.sendDestroyCartItem(g_shoppingCart.m_wSerial,g_gwCart.m_iSelectCartSlot,g_shoppingCart.getCheckSum());
				break;

			case	eMBI_CANCEL							:
				break;

			case	eMBI_TRANS_AVATAR_TO_GVG_SERVER		:
				s_agent.sendEtcWork(eCEW_TRANS_AVATAR_TO_GVG_SERVER);
				break;

			case	eMBI_TRANS_GUILD_TO_GVG_SERVER		:
				s_agent.sendEtcWork(eCEW_TRANS_GUILD_TO_GVG_SERVER);
				break;

			case	eMBI_LEAVE_GUILD					:
				s_agent.sendLeaveGuild();
				break;

			case	eMBI_DISSOLUTION_GUILD				:
				s_agent.sendDissolutionGuild();
				break;

			case	eMBI_MAKE_GUILD						:
				s_agent.sendReadyToMakeGuild();
				break;

			case	eMBI_SELECT_GUILD_MARK				:
				s_agent.sendEtcWork(eCEW_SELECT_GUILD_MARK);
				break;

			case	eMBI_OPEN_SHOPPING_CART				:
				s_agent.sendAskOpenShoppingCart();
				break;

			case	eMBI_OPEN_PITCHMAN_SHOP				:
				s_agent.sendAskOpenThePitchmanShop();
				break;

			case	eMBI_STOP_PITCHMAN_SHOP				:
				g_gwPitchmanShop.close();
				break;

			case	eMBI_ENTER_TOWN_PORTAL				:
				s_agent.sendEnterPortal(g_portal.m_iEnterPortal);
				break;

			case	eMBI_BUY_PITCHMAN_SHOP_ITEM	:
			{
				g_gwPitchmanShop.buyPitchmanShopItem();
				break;
			}

			case	eMBI_CALL_MAGIC_CARPET				:
				s_agent.sendCallMagicCarpet();
				break;

			case	eMBI_RETURN_TO_THE_VILLAGE_BY_REVIVE				:
				s_agent.sendReturnToVillage(eRTV_BY_REVIVE);
				break;

			case	eMBI_RETURN_TO_THE_VILLAGE_BY_ITEM					:
				s_agent.sendReturnToVillage(eRTV_BY_ITEM);
				break;

			case	eMBI_USE_TITLE								:
				s_agent.sendEtcWork(eCEW_USE_TITLE,s_iUseTitle);
				break;

			case	eMBI_USE_COUPLE_RING								:
				s_agent.sendEtcWork(eCEW_USE_COUPLE_RING,s_iBookedWorkItem);
				break;

			case	eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_GUILD_BATTLE	:
				s_agent.sendReturnToVillage(eRTV_BY_END_OF_GUILD_BATTLE);
				g_msgBox.cPopup("",dMSG_WAIT_MESSAGE_FROM_SERVER);
				break;

			case	eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_OX_QUIZ_EVENT	:
				s_agent.sendReturnToVillage(eRTV_BY_END_OF_EVENT);
				g_msgBox.cPopup("",dMSG_WAIT_MESSAGE_FROM_SERVER);
				break;

			case	eMBI_USE_ITEM						:
				s_agent.sendUseItemToCharacter(s_iReadyToUseItem,0xffff);
				break;

			case	eMBI_USE_POLISHER					:
				s_agent.sendEtcWork(eCEW_USE_POLISHER,s_iReadyToUseItem,g_gwMiniPetStatus.m_iSelectMiniPet,0);
				break;
			case	eMBI_USE_MASSIVE_POLISHER			:
				s_agent.sendEtcWork(eCEW_USE_POLISHER,s_iReadyToUseItem,g_gwMiniPetStatus.m_iSelectMiniPet,1);
				break;
				
			case	eMBI_RESTRAINT_MINIPET			:	//	미니펫 속박
			{
				s_agent.sendEtcWork(eCEW_RESTRAINT_MINIPET,s_iReadyToUseItem,g_gwMiniPetStatus.m_iSelectMiniPet);
				break;
			}

			case	eMBI_MIX_MINIPET				:	//	미니펫 섞기
			{
				int	aiValues[12];

				aiValues[0]	=	s_iReadyToUseItem;

				memcpy(aiValues+1,g_gwMiniPetStatus.m_aiMultiSelect,sizeof(int)*11);

				s_agent.sendEtcWork(eCEW_MIX_MINIPET,12,aiValues);
				//	사실 3개인데... 아이템 만들때 갯수도 변경 할 수 있게 했다. 그래서 걍 여유로 12개 까지.. - -
				break;
			}

			//JBC	미니펫 각성 3차		각성의 씨앗	09-01-02
			case	eMBI_USE_AWAKEN_ITME50				:
				s_agent.sendEtcWork(eCEW_USE_MINIPET_AWAKEN_ITEM,s_iReadyToUseItem,g_gwMiniPetStatus.m_iSelectMiniPet,0);
				break;
			//JBC	미니펫 각성 3차		각성의 열매	09-01-02
			case	eMBI_USE_AWAKEN_ITME100				:
				s_agent.sendEtcWork(eCEW_USE_MINIPET_AWAKEN_ITEM,s_iReadyToUseItem,g_gwMiniPetStatus.m_iSelectMiniPet,1);
				break;
			case	eMBI_CHANGE_MINIPET_NAME			:
				s_agent.sendEtcWork(eCEW_RESET_MINIPET_NAME,s_iReadyToUseItem,g_gwMiniPetStatus.m_iSelectMiniPet);
				break;

			case	eMBI_DESTROY_ITEM					:
				{
					BOOL bDestroyItem = s_agent.sendAskDestroyItem(s_iBookedWorkItem);
					
					if(bDestroyItem)
						g_hero.deleteQuickSlotItem(s_iBookedWorkItem);
					break;
				}

			case	eMBI_ACCEPT_ENCHANT_ITEM			:
				s_agent.sendUseItemToItem(g_hero.m_wUseItem,g_hero.m_wTargetItem);
				break;

			case	eMBI_REQUEST_GUILD_BATTLE			:
				s_agent.sendRequestGuildBattle(g_gwGuildBattleScheduler.m_iSelectTime,g_gwGuildBattleScheduler.m_iSelectFieldOrder);
				break;

			case	eMBI_CANCEL_GUILD_BATTLE			:
				s_agent.sendCancelGuildBattle();
				break;

			case	eMBI_PARTING_WITH_PET				:
				s_agent.sendPartingWithPet(g_gwPet.m_wSelectPetIndex);
				break;

			case	eMBI_PARTING_WITH_MINIPET			:
				s_agent.sendPartingWithPet(g_gwMiniPetStatus.m_iSelectMiniPet,2);
				break;

			case	eMBI_USE_BLOCK_LIGHT_POCKET			:
				s_agent.sendEtcWork(eCEW_USE_BLOCK_LIGHT_POCKET,g_gwMiniPetStatus.m_iSelectMiniPet);
				break;

			case	eMBI_RESET_PET_NAME					:
				s_agent.sendEtcWork(eCEW_RESET_PET_NAME,g_gwPet.m_wSelectPetIndex,g_gwPet.m_iResetPetNameItemIndex);
				break;

			case	eMBI_RELEASE_SUMMON_BEAST			:
				s_agent.sendPartingWithPet(g_gwPet.m_wSelectSummonBeast,TRUE);
				break;

			case	eMBI_WITHDRAW_MAGIC_BAG				:
				g_gwCart.askWithdrawItemFromTheCart(TRUE);
				break;

			case	eMBI_BUY_CARROT_SHOP_ITEM			:
				g_gwCarrotShop.m_wIsWaitResult	=	TRUE;
				s_agent.sendBuyCarrotShopItem(g_gwCarrotShop.m_iBuyCategory,g_gwCarrotShop.m_iBuyItem);
				break;

			case	eMBI_ARCA_DISARM_TRAP				:
				s_agent.sendArcaWork(g_arcaManager.m_wWorkArca,eAAW_DISARM_TRAP);
				break;

			case	eMBI_ARCA_UNLOCK					:
				s_agent.sendArcaWork(g_arcaManager.m_wWorkArca,eAAW_UNLOCK);
				break;

			case	eMBI_MOVE_TO_USE_GAME_CARROT_AGREEMENT_PAGE	:
			{
#ifdef	_FOR_KOREA
				SetForegroundWindow(GetDesktopWindow());
				ShellExecute(NULL,"open","http://www.redgem.co.kr/member/cig_agree.aspx",NULL,NULL,SW_SHOWNORMAL);
#endif				
				break;
			}

			case	eMBI_USE_BOTTOMLESS_BOX						:
			{
				g_gwBottomlessBoxWindow.mix();
				break;
			}
			case	eMBI_BUY_GOLD_BAR						:		// 금괴 구입할꺼다 ~ 09.08.20 
				{
					s_agent.sendEtcWork(eCEW_BUY_GOLD_BAR, g_bank.m_wGoldBarCount);				
					break;
				}
			case	eMBI_UPGRADE_ETERNAL_ITEM						:		// 강화 버튼 클릭!! ~ 09.08.26 
				{
					BOOL bUpgradePerfect = FALSE;
#ifdef _DEBUG
				bUpgradePerfect = g_gwUpgradeShop.m_bUpgradePerfect;
#endif
					s_agent.sendEtcWork(eCEW_ETERNAL_ITEM_UPGRADE, g_gwUpgradeShop.getUpgradeWeaponItemSlot(), g_gwUpgradeShop.getUpgradeItemSlot(), g_gwUpgradeShop.getUpgradeNormalItemSlot(), bUpgradePerfect);				

					break;
				}
			case		eMBI_DISJOINTING_ITEM						:				// 아이템 분해 창 메세지~~ 09.09.07
				{
					BOOL bDisJointing = s_agent.sendEtcWork(eCEW_DISJOINTING_ITEM,g_gwCommonList.m_aContentsList[g_gwCommonList.getSelected()].m_iID);
					
					if(bDisJointing)
						g_hero.deleteQuickSlotItem(g_gwCommonList.m_aContentsList[g_gwCommonList.getSelected()].m_iID);
					
					break;
				}
			case		eMBI_USE_SHOP						:				// 상점 이용하기
				{
					s_agent.sendEtcWork(eCEW_USE_SHOP);
					break;
				}
			case		eMBI_USE_BANK						:				// 은행 이용하기
				{
					s_agent.sendEtcWork(eCEW_USE_BANK);
					break;
				}
			case		eMBI_COPY_SELECT_PREFIX						:				// 접두사 복사하기.
				{
					s_agent.sendEtcWork(eCEW_COPY_PREFIX,  g_gwCopyOrCutPrefix.m_iSrcItem,g_gwCopyOrCutPrefix.m_iDestItem,g_gwCopyOrCutPrefix.m_iSelectPrefix);
					g_gwCopyOrCutPrefix.close();
					break;
				}
			case		eMBI_CUT_SELECT_PREFIX						:				// 접두사 잘라내기.
				{
					s_agent.sendEtcWork(eCEW_CUT_PREFIX,  g_gwCopyOrCutPrefix.m_iSrcItem,g_gwCopyOrCutPrefix.m_iDestItem,g_gwCopyOrCutPrefix.m_iSelectPrefix);
					g_gwCopyOrCutPrefix.close();
					break;
				}
			case	eMBI_ACCEPT_GUILD_MEMBER_JOIN_PARTY_REQUEST		:		//	길드 맴버 파티 참가 제안을 받아 들인다.
				s_agent.sendAskJoinPartyReply(s_strPartyJoinAskerName,1, FALSE, FALSE, TRUE);
				break;
			case	eMBI_DECLINE_GUILD_MEMBER_JOIN_PARTY_REQUEST		:	//		길드 맴버 파티 참가 제안을 거절한다.
				s_agent.sendAskJoinPartyReply(s_strPartyJoinAskerName,0, FALSE, FALSE, TRUE);
				break;
			case	eMBI_MINIPET_FEED			:
				s_agent.sendEtcWork(eCEW_MINIPET_FEED,g_gwMiniPetStatus.m_iSelectMiniPet,s_iReadyToUseItem);
				break;
			case	eMBI_RESET_MINIPET_SKILL			:
				s_agent.sendEtcWork(eCEW_RESET_MINIPET_SKILL,g_gwMiniPetStatus.m_iSelectMiniPet,s_iReadyToUseItem);
				break;
			case	eMBI_SET_MINIPET_TYPE			:
				s_agent.sendEtcWork(eCEW_SET_MINIPET_TYPE,g_gwMiniPetStatus.m_iSelectMiniPet,s_iReadyToUseItem);
				break;
		}

		return;
	}	//	if (s_isOperatedMsgbox)

	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_SYSTEM_DIALOG);
}

//
//	팝업 메뉴 처리
inline	void
CGamePlay::OperatePopupMenu()
{


	if	(s_pmCommon.isPopuped())
	{
		cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);

		int		iResult	=	s_pmCommon.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,s_isLeftButtonPressed,s_isClickedRightButton+s_aKey[KEY_ESCAPE]);

		if	(iResult	>=	ePM_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY_BEGIN	&&	iResult	<=	ePM_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY_END)
		{
			int		iRank	=	iResult	-	ePM_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY_BEGIN;
			char	strText[256];

			sprintf(strText,"@%s [%d] %s",dMSG_KW_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY,iRank,dMSG_PLEASE_INPUT_GOLD);

			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,strText);
			s_inputChatting.setSelect(0,strlen(strText)-strlen(dMSG_PLEASE_INPUT_GOLD),-1);
		}
		else
		if	(iResult	>=	ePM_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY_BEGIN	&&	iResult	<=	ePM_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY_END)
		{
			int		iRank	=	iResult	-	ePM_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY_BEGIN;
			char	strText[256];

			sprintf(strText,"@%s [%d] %s",dMSG_KW_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY,iRank,dMSG_PLEASE_INPUT_COUNT);

			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,strText);
			s_inputChatting.setSelect(0,strlen(strText)-strlen(dMSG_PLEASE_INPUT_COUNT),-1);
		}
		else
		if	(iResult	>=	ePM_CHANGE_FRIEND_GROUP	&&	iResult	<=	ePM_CHANGE_FRIEND_GROUP_END)
		{
			if	(CheckBCS())
			{
				g_hero.sendChangeFriendGroup(iResult-ePM_CHANGE_FRIEND_GROUP,g_gwFriend.m_strSelectFriendName);
				g_hero.changeFriendGroup(iResult-ePM_CHANGE_FRIEND_GROUP,g_gwFriend.m_strSelectFriendName);
				g_gwFriend.resetFriendList();
			}
		}
		else
		if	(iResult	>=	ePM_TAME_MONSTER_BEGIN	&&	iResult	<=	ePM_TAME_MONSTER_END)
		{
			int		iJob	=	iResult	-	ePM_TAME_MONSTER_BEGIN;
			char	strText[256];

			sprintf(strText,"@tameMonster %d",iJob);

			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,strText);
		}
		else
		if	(iResult	>=	ePM_CHANGE_MONSTER_JOB_BEGIN	&&	iResult	<=	ePM_CHANGE_MONSTER_JOB_END)
		{
			int		iJob	=	iResult	-	ePM_CHANGE_MONSTER_JOB_BEGIN;
			char	strText[256];

			sprintf(strText,"\\changeMonsterJob %3d %3d %s",g_am.m_iBookedTarget,iJob,dMSG_PLEASE_INPUT_LEVEL);

			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,strText);
			s_inputChatting.setSelect(0,strlen(strText)-strlen(dMSG_PLEASE_INPUT_LEVEL),-1);
		}
		else
		switch(iResult)
		{
			case	ePM_CHANGE_GUILD_INVENTORY_STORAGE_SIZE	:
			{
				char	strText[256];

				sprintf(strText,"@%s [%d,%d,%d,%d] %s",dMSG_KW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE,
						g_guild.m_abInventorySize[0],g_guild.m_abInventorySize[1],g_guild.m_abInventorySize[2],g_guild.m_abInventorySize[3],
						dMSG_PLEASE_INPUT_CHANGE_GUILD_INVENTORY_STORAGE_SIZE);

				if	(!s_inputChatting.isEnable())
					EnableChattingBar(TRUE);

				s_inputChatting.setStr(0,strText);
				s_inputChatting.setSelect(0,strlen(strText)-strlen(dMSG_PLEASE_INPUT_CHANGE_GUILD_INVENTORY_STORAGE_SIZE),-1);

				break;
			}
			case	ePM_CHANGE_FRIEND_GROUP_NAME	:
			{
				if	(!CheckBCS())
					break;

				if	(!s_inputChatting.isEnable())
					EnableChattingBar(TRUE);

				int	iLen	=	strlen(dMSG_CHANGE_FRIEND_GROUP_NAME_COMMAND)+2;

				s_inputChatting.active(0,TRUE);
				s_inputChatting.setStr(0,_ms(dMSG_CHANGE_FRIEND_GROUP_NAME_COMMAND_FORM,dMSG_CHANGE_FRIEND_GROUP_NAME_COMMAND));
				s_inputChatting.setSelect(0,iLen,-1);
				break;
			}
			case	ePCDM_ASK_FRIEND	:
				if	(g_hero.isFriend(s_strBookedUserName))
					g_lpHero->warning(dMSG_PLAYER_ARE_ALLREADY_FRIEND);
				else
					s_agent.sendRequestAddFriend(s_strBookedUserName);
				break;

			case	ePCDM_SEND_WHISPER	:
			{
				s_inputChatting.enable(TRUE);
				s_inputChatting.setStr(0,_ms("/%s ",s_strBookedUserName));
				break;
			}

			case	ePCDM_ASK_PARTY		:
			{
				if	(!g_lpHero->isPartyMember() || g_lpHero->isPartyLeader())
				{
					s_agent.sendAskJoinParty(s_strBookedUserName);
					g_lpHero->warning(_ms(dMSG_PROPOSE_PARTY_FORM,s_strBookedUserName));
				}
				else
					g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE);

				break;
			}

			case	eJPRUL_ACCEPT_JOIN_PARTY_REQUEST	:	//	파티 참가 신청 받아들임
				s_partyInfo.sendAcceptJoinPartyRequest();
				break;

			case	eJPRUL_REMOVE_USER_FROM_JOIN_PARTY_USER_LIST	:	//	파티 참가 신청 유저 제거
				s_partyInfo.m_requestJoinPartyUserList.removeSelectUser();
				break;

			case	eJPRUL_RESET_JOIN_PARTY_USER_LIST	:	//	파티 참기 신청 리스트 초기화
				s_partyInfo.m_requestJoinPartyUserList.reset();
				break;

			case	ePSBM_PS_RAISE_PARTY_MEMBER			:
				if	(s_partyInfo.m_bf1IsOpenParty	==	TRUE)
					break;

				if	(stricmp(s_partyInfo.m_strPartyProspectus," ")	==	0)
				{
					WarningMessage(dMSG_REQUIRE_SETTING_PARTY_EXPLAIN_BEFOR_OPEN_PARTY);
					break;
				}

				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,s_partyInfo.m_bf3ItemShareMethod,s_partyInfo.m_bf3PartyObject,TRUE);
				break;

			case	ePSBM_PS_DO_NOT_RAISE_PARTY_MEMBER	:
				if	(s_partyInfo.m_bf1IsOpenParty	==	FALSE)
					break;
				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,s_partyInfo.m_bf3ItemShareMethod,s_partyInfo.m_bf3PartyObject,FALSE);
				break;

			case	ePSBM_CHANGE_RECRUIT_PARTY_MEMBER_RANGE	:
				if	(!s_inputChatting.isEnable())
					EnableChattingBar(TRUE);

				s_inputChatting.setStr(0,_ms("@%s %s",dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE_KEYWORD,dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE_COMMENT));

				s_inputChatting.setSelect(0,strlen(dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE_KEYWORD)+2,-1);
				break;
				
			case	ePSBM_PO_HUNT			:	//	사냥
				if	(s_partyInfo.m_bf3PartyObject	==	ePO_HUNT)
					break;
				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,s_partyInfo.m_bf3ItemShareMethod,ePO_HUNT,s_partyInfo.m_bf1IsOpenParty);
				break;
			case	ePSBM_PO_EXPLORATION	:	//	탐험
				if	(s_partyInfo.m_bf3PartyObject	==	ePO_EXPLORATION)
					break;
				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,s_partyInfo.m_bf3ItemShareMethod,ePO_EXPLORATION,s_partyInfo.m_bf1IsOpenParty);
				break;
			case	ePSBM_PO_QUEST			:	//	퀘스트 해결
				if	(s_partyInfo.m_bf3PartyObject	==	ePO_QUEST)
					break;
				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,s_partyInfo.m_bf3ItemShareMethod,ePO_QUEST,s_partyInfo.m_bf1IsOpenParty);
				break;

			case	ePSBM_PGSM_FREE			:
				if	(s_partyInfo.m_bf3GoldShareMethod	==	ePGSM_FREE)
					break;
				s_agent.sendChangePartyStatus(ePGSM_FREE,s_partyInfo.m_bf3ItemShareMethod,s_partyInfo.m_bf3PartyObject,s_partyInfo.m_bf1IsOpenParty);
				break;
			case	ePSBM_PGSM_EQUALITY		:
				if	(s_partyInfo.m_bf3GoldShareMethod	==	ePGSM_EQUALITY)
					break;
				s_agent.sendChangePartyStatus(ePGSM_EQUALITY,s_partyInfo.m_bf3ItemShareMethod,s_partyInfo.m_bf3PartyObject,s_partyInfo.m_bf1IsOpenParty);
				break;
			case	ePSBM_PGSM_ORDER		:
				if	(s_partyInfo.m_bf3GoldShareMethod	==	ePGSM_ORDER)
					break;
				s_agent.sendChangePartyStatus(ePGSM_ORDER,s_partyInfo.m_bf3ItemShareMethod,s_partyInfo.m_bf3PartyObject,s_partyInfo.m_bf1IsOpenParty);
				break;

			case	ePSBM_PISM_FREE			:
				if	(s_partyInfo.m_bf3ItemShareMethod	==	ePISM_FREE)
					break;
				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,ePISM_FREE,s_partyInfo.m_bf3PartyObject,s_partyInfo.m_bf1IsOpenParty);
				break;
			case	ePSBM_PISM_RANDOM		:
				if	(s_partyInfo.m_bf3ItemShareMethod	==	ePISM_RANDOM)
					break;
				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,ePISM_RANDOM,s_partyInfo.m_bf3PartyObject,s_partyInfo.m_bf1IsOpenParty);
				break;
			case	ePSBM_PISM_ORDER		:
				if	(s_partyInfo.m_bf3ItemShareMethod	==	ePISM_ORDER)
					break;
				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,ePISM_ORDER,s_partyInfo.m_bf3PartyObject,s_partyInfo.m_bf1IsOpenParty);
				break;
			case	ePSBM_PISM_FIRST_JOB_RANDOM:
				if	(s_partyInfo.m_bf3ItemShareMethod	==	ePISM_FIRST_JOB_RANDOM)
					break;
				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,ePISM_FIRST_JOB_RANDOM,s_partyInfo.m_bf3PartyObject,s_partyInfo.m_bf1IsOpenParty);
				break;
//			case	ePSBM_PISM_FIRST_JOB_ORDER:
//				if (s_partyInfo.m_bf3ItemShareMethod	==	ePISM_FIRST_JOB_ORDER)
//					break;
//				s_agent.sendChangePartyStatus(s_partyInfo.m_bf3GoldShareMethod,ePISM_FIRST_JOB_ORDER,s_partyInfo.m_bf3PartyObject,s_partyInfo.m_bf1IsOpenParty);
//				break;

			case	ePM_USE_SET_ISPI_STRENGTH		:
			case	ePM_USE_SET_ISPI_AGILITY		:
			case	ePM_USE_SET_ISPI_CONSTITUTION	:
			case	ePM_USE_SET_ISPI_CHARISMA		:
			case	ePM_USE_SET_ISPI_INTELLIGENCE	:
			case	ePM_USE_SET_ISPI_WISDOM			:
			case	ePM_USE_SET_ISPI_LUCK			:
				s_agent.sendEtcWork(eCEW_CHANGE_ISPI_SETTING,s_iBookedWorkItem,iResult-ePM_USE_SET_ISPI_STRENGTH);
				break;

			case	ePM_USE_COUPLE_RING						:
			{
				cItem	*lpItem	=	g_hero.getInventoryItem(s_iBookedWorkItem);

				if	(!lpItem)
					break;

				cCoupleRingDefine	*lpCoupleRing	=	lpItem->getCoupleRing();

				if	(!lpCoupleRing)
					break;

				char	strMessage[256];

				sprintf(strMessage,dMSG_ARE_YOU_REALLY_USE_COUPLE_RING_FROM,lpCoupleRing->getCoupleName());

				g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_USE_COUPLE_RING);

				break;
			}

			case	ePM_ATTACK_DOOR							:
			{
				s_agent.sendChangeDoorStatus(g_hero.m_wWorkDoor,eCDSM_ATTACK_DOOR);
				break;
			}
			case	ePM_ATTACK_ARCA							:
			{
				s_agent.sendArcaWork(g_arcaManager.m_wWorkArca,eAAW_ATTACK);
				break;
			}

			case	ePM_SUMMON_MAGIC_CARPET+eMCS_WILD_CAT	:
			case	ePM_SUMMON_MAGIC_CARPET+eMCS_TURTLE		:
			case	ePM_SUMMON_MAGIC_CARPET+eMCS_SQUIRREL	:
			case	ePM_SUMMON_MAGIC_CARPET+eMCS_DRAKE		:
			case	ePM_SUMMON_MAGIC_CARPET+eMCS_RED_DRAKE	:
			case	ePM_SUMMON_MAGIC_CARPET+eMCS_GEAR		:
			case	ePM_SUMMON_MAGIC_CARPET+eMCS_NORMAL		:
				s_agent.sendChangeMagicCarpetShape(iResult-ePM_SUMMON_MAGIC_CARPET);
				break;

			case	ePM_PRM_TRADE						:
			{
				if (IsOpenTradeBox())
					break;

				CActor	*lpActor	=	g_am.getActorByName(s_strBookedUserName);

				if	(!lpActor)
					break;

				s_agent.sendRequestTrade(s_strBookedUserName,lpActor->m_wSerial);
				g_lpHero->warning(_ms(dMSG_PROPOSE_TRADE_FORM,lpActor->m_strName));
				break;
			}
			
			case	ePM_PRM_ASK_JOIN_PARTY				:
			{
				if (!g_lpHero->isPartyMember() || g_lpHero->isPartyLeader())
				{
					s_agent.sendAskJoinParty(s_strBookedUserName);
					g_lpHero->warning(_ms(dMSG_PROPOSE_PARTY_FORM,s_strBookedUserName));
				}
				else
					g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE);

				break;
			}

			case	ePM_UNION_PARTY			:
				{
					if (!g_lpHero->isPartyLeader())
					{
						s_agent.sendAskJoinParty(s_strBookedUserName, FALSE , TRUE);
						g_lpHero->warning(_ms(dMSG_PROPOSE_PARTY_FORM,s_strBookedUserName));
					}
					else
						g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE);
				}
			break;
			
			case	ePM_PRM_SAY							:
			{
				s_inputChatting.enable(TRUE);
				s_inputChatting.setStr(0,_ms("/%s ",s_strBookedUserName));
				break;
			}
			case	ePM_PRM_ADD_FRIEND					:
			{
				if	(g_hero.isFriend(s_strBookedUserName))
					g_lpHero->warning(dMSG_PLAYER_ARE_ALLREADY_FRIEND);
				else
					s_agent.sendRequestAddFriend(s_strBookedUserName);
				break;
			}

			case	ePM_PRM_DENY_COMMUNITY				:
			{
				g_am.addDenyCommunityActor(s_strBookedUserName);
				break;
			}

			case	ePM_PRM_NO_MANNER_POINT				:
			{
				break;
			}

			case	ePM_REMEMBER_THIS_PLACE_TO_SLOT1	:
				s_agent.sendRememberOrbLocation(0);
				break;

			case	ePM_REMEMBER_THIS_PLACE_TO_SLOT2	:
				s_agent.sendRememberOrbLocation(1);
				break;

				//문 스톤류와의 충돌을 막기 위해 8번 슬롯을 쓴다.
			case	ePM_REMEMBER_THIS_PLACE_TO_SLOT3	:
				s_agent.sendRememberOrbLocation(8);
				break;

			case	ePM_WARP_TO_WAYPOINT_SLOT1			:
				if (g_hero.m_wRestraintTimeByBattle)
				{
					char	strMessage[256];

					int	iRemainTime	=	max(1,g_hero.m_wRestraintTimeByBattle);

					sprintf(strMessage,dMSG_CAN_NOT_USE_ORB_DURING_BATTLE_FORM,iRemainTime);
                                         
					CGamePlay::WarningMessage(strMessage);
					return;
				}
				s_agent.sendTeleportToOrbLocation(0);
				break;

			case	ePM_WARP_TO_WAYPOINT_SLOT2			:
				if (g_hero.m_wRestraintTimeByBattle)
				{
					char	strMessage[256];

					int	iRemainTime	=	max(1,g_hero.m_wRestraintTimeByBattle);

					sprintf(strMessage,dMSG_CAN_NOT_USE_ORB_DURING_BATTLE_FORM,iRemainTime);

					CGamePlay::WarningMessage(strMessage);
					return;
				}
				s_agent.sendTeleportToOrbLocation(1);
				break;
			case	ePM_WARP_TO_WAYPOINT_SLOT3			:
				if (g_hero.m_wRestraintTimeByBattle)
				{
					char	strMessage[256];
					
					int	iRemainTime	=	max(1,g_hero.m_wRestraintTimeByBattle);
					
					sprintf(strMessage,dMSG_CAN_NOT_USE_ORB_DURING_BATTLE_FORM,iRemainTime);
					
					CGamePlay::WarningMessage(strMessage);
					return;
				}
				//8로 이동하는 이유는 문스톤류 땜시...-_-;;
				s_agent.sendTeleportToOrbLocation(8);

				break;
			case	ePM_SKILL_TREE						:
			{
				g_gwEasySkillTree.open(s_iFocusSkill);
				break;
			}

			case	ePM_RETURN_TO_VILLAGE				:
			{
				if (g_hero.m_wRestraintTimeByBattle)
				{
					char	strMessage[256];

					int	iRemainTime	=	max(1,g_hero.m_wRestraintTimeByBattle);

					sprintf(strMessage,dMSG_CAN_NOT_USE_ORB_DURING_BATTLE_FORM,iRemainTime);

					CGamePlay::WarningMessage(strMessage);
					return;
				}

				g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_RETURN_TO_THE_VILLAGE,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_ITEM);
				break;
			}
			case	ePM_CALL_MAGIC_CARPET				:
			{
				g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_CALL_MAGIC_CARPET,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_CALL_MAGIC_CARPET);
				break;
			}

			case	ePM_DESTROY_ITEM					:
			{
				g_hero.askDestroyItem(s_iBookedWorkItem);				
				break;
			}

			case	ePM_ITEM_DROP						:	//	아이템 버리기
			{
				cItem	*lpItem	=	g_hero.getInventoryItem(s_iBookedWorkItem);

				if	(!lpItem)
					break;

				if	(g_aBasicItem[lpItem->m_wBaseItem].m_attr.isCanNotDropItem)
				{
					g_lpHero->warning(dMSG_CAN_NOT_DROP_ITEM);
					break;
				}

				g_hero.prepareDroppingItem(s_iBookedWorkItem);

				if (g_hero.getInventoryItem(s_iBookedWorkItem)->m_bCount	>	1)
				{
					g_itemCountWindow.popup(s_posMouse.x,s_posMouse.y,s_iBookedWorkItem,g_hero.getInventoryItem(s_iBookedWorkItem),eICWR_RETURN_DROP_ITEM_COUNT);
					cINPDEV::SetExclusive(eMOUSE_STATE_GET_ITEM_COUNT);
					break;
				}

				g_hero.sendDroppingItem(s_iBookedWorkItem,1);
				break;
			}

			case	ePM_ITEM_USE			:	//	아이템 사용하기
				g_hero.askUseItem(s_iBookedWorkItem);
				break;

			case	ePM_USE_GATE_GLOVE		:	//  게이트 글로브 사용
			{
				if	(g_hero.m_wRestraintTimeByBattle)
				{
					char	strMessage[256];

					int	iRemainTime	=	max(1,g_hero.m_wRestraintTimeByBattle);

					sprintf(strMessage,dMSG_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE_FORM,iRemainTime);

					CGamePlay::WarningMessage(strMessage);
					return;
				}

				if	(g_hero.m_wCurrentField	==	27)
					break;

				g_gwWorldMap.open(TRUE);
				break;
			}

			case ePM_NETCAFE_GATE_GLOVE		:
				if	(g_hero.m_wRemainSecondForUseGateGlove)
				{
					AddSystemMessage(WHITE,dMSG_USE_GATE_GLOVE_FAILED_BY_REMAIN_SECOND_FORM);
					AddSystemMessage(WHITE,dMSG_REMAIN_TIME_FOR_USE_GATE_GLOVE_FORM , g_hero.m_wRemainSecondForUseGateGlove);
					break;
				}

				g_gwWorldMap.open(TRUE);
				break;

			case	ePM_PUT_BANNER			:
				g_gwPutBannerWnd.open(s_iBookedWorkItem);
				break;

			case	ePM_READ_BOOK			:
				g_gwBook.open(s_iBookedWorkItem);
				break;

			case	ePM_REMOVE_TITLE		:
			{
				char	strText[256];

				CPlayerTitleInfo	*lpTitleInfo	=	g_hero.getTitleByIndex(s_iSelectTitle);

				if	(!lpTitleInfo)
					break;

				CTitle				*lpTitle		=	g_titleManager.getTitle(lpTitleInfo->m_bTitle);

				if	(!lpTitle)
					break;

				sprintf(strText,dMSG_ARE_YOU_REALLY_REMOVE_TITLE_FORM,lpTitle->m_strName);

				g_msgBox.cPopup("",strText,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_REMOVE_TITLE);
				break;
			}

			case	ePM_USE_BOTTOMLESS_BOX	:
				g_gwBottomlessBoxWindow.open(s_iBookedWorkItem);
				break;

			case	ePM_DESTROY_CART_ITEM	:
				g_gwCart.askDestroyCartItem();
				break;

			case	ePM_REGIST_REGULAR_GUILD_MEMBER	:
			case	ePM_APPOINTMENT_SUB_GUILD_MASTER:
			case	ePM_DISMISS_PARLIAMENT			:
			case	ePM_APPOINTMENT_GUILD_MASTER	:
			case	ePM_DISMISS_SUB_GUILD_MASTER	:
			case	ePM_BANISH_MEMBER				:
			case	ePM_RENT_GUILD_PET_OBITIAN		:
			case	ePM_RENT_GUILD_PET_DASH_BLADE	:
			case	ePM_RENT_GUILD_PET_ASTRO_BOW	:
			case	ePM_RENT_GUILD_PET_CRISTAL_WATER:
			case	ePM_RETRIEVAL_GUILD_PET			:
				g_gwGuild.selectMemberListPopupMenu(iResult);
				break;

			case	ePM_MINIPET_PLACE_TO_LEFT	:
				s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_PLACE,g_gwMiniPetStatus.m_iSelectMiniPet,0);
				break;
			case	ePM_MINIPET_PLACE_TO_RIGHT	:
				s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_PLACE,g_gwMiniPetStatus.m_iSelectMiniPet,1);
				break;
			case	ePM_MINIPET_PLACE_TO_UP		:
				s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_PLACE,g_gwMiniPetStatus.m_iSelectMiniPet,2);
				break;
			case	ePM_MINIPET_PLACE_TO_DOWN	:
				s_agent.sendEtcWork(eCEW_CHANGE_MINIPET_PLACE,g_gwMiniPetStatus.m_iSelectMiniPet,3);
				break;
			case	ePM_USE_SHOP		:	// 상점 이용하기
				{
				char strMessage[512] = "";
				sprintf(strMessage,dMSG_ASK_USE_SHOP);
				g_msgBox.cPopup("",strMessage,dMSG_YES, dMSG_NO);
				g_msgBox.setButtonId(eMBI_USE_SHOP);
				break;
				}
			case	ePM_USE_BANK	:		// 은행 이용하기
				{
				char strMessage[512] = "";
				sprintf(strMessage,dMSG_ASK_USE_BANK);
				g_msgBox.cPopup("",strMessage,dMSG_YES, dMSG_NO);
				g_msgBox.setButtonId(eMBI_USE_BANK);
				break;
				}
			case	ePM_PRM_ASK_GUILD_MEMBER_JOIN_PARTY				:
			{		// 길드원 리스트에 있는 길드원을 파티 초대한다.

				if(strcmp(g_lpHero->m_strName , s_strBookedUserName) == 0 )
					break;

				if (!g_lpHero->isPartyMember() || g_lpHero->isPartyLeader())
				{

					s_agent.sendAskJoinParty(s_strBookedUserName, FALSE, FALSE, TRUE);
					g_lpHero->warning(_ms(dMSG_PROPOSE_PARTY_FORM,s_strBookedUserName));
				}
				else
					g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE);

				break;

			}

		}

		return;
	}

	cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_POPUP_MENU);
}




//
//	키보드 처리
inline	void
CGamePlay::OperateKeyboard()
{
	if (s_isOperatedMsgbox)
		return;

	if (s_isLeftButtonPressed	==	FALSE)
	{
		if (s_iDragSkill	!=	0xffff)
		{
			g_gwSkill.operateDragSkill();
			s_iDragSkill	=	0xffff;
		}

		if (s_iDragItem		!=	0xffff)
		{
			OperateDragItem();
			s_iDragItem		=	0xffff;
		}

		if (s_iDragTradeItem	!=	0xffff)
		{
			OperateDragItemInTradeWindow();
			s_iDragTradeItem	=	0xffff;
		}
	}

#ifdef _DEBUG	//JBC	옵져버 모드로 전환.	08-07-15
	if(g_bObserverMode)
		g_am.OperateObserverKeyboard();
#endif

	{
		static	int	iPressedAltCounter	=	0;

		if	(s_aKey[KEY_LALT] || s_aKey[KEY_RALT])
		{
			g_fieldItem.m_bIsShowDroppedItemName	=	TRUE;
			iPressedAltCounter++;
		}
		else
		{
			if	(g_fieldItem.m_bIsShowDroppedItemName	!=	2)
			{
				if	(iPressedAltCounter	>=	dSYNC_FPS*3)
					g_fieldItem.m_bIsShowDroppedItemName	=	2;
				else
					g_fieldItem.m_bIsShowDroppedItemName	=	FALSE;

				iPressedAltCounter	=	0;
			}
		}
	}

	if (s_aKey[KEY_LCONTROL] || s_aKey[KEY_RCONTROL])
		s_bIsIgnoreAllFocus	=	TRUE;
	else
		s_bIsIgnoreAllFocus	=	FALSE;

	if (s_aKey[KEY_LSHIFT] || s_aKey[KEY_RSHIFT])
		s_ttCommon.setEnable(FALSE);
	else
		s_ttCommon.setEnable(g_config.m_wIsTooltipOn);

#ifdef	_DEBUG
	if	(g_gwWorldMap.isOpened() == FALSE && s_aTouched[KEY_F11])
		g_gwDebug.open();
#endif

#ifdef	_FOR_JAPAN
	if	(g_lpControlActor == NULL && s_aTouched[KEY_SPACE] && s_inputChatting.isEnable() == FALSE)
		g_hero.pickNearestItem();
#endif

	if	(s_inputChatting.isEnable() == FALSE)
	{
		if	(s_aTouched[KEY_V] && g_hero.m_wIsReadyToFireChargedMiniPetSkill)
			s_agent.sendEtcWork(eCEW_USE_MINIPET_CHARGE_SKILL,g_am.s_iFocusActor,0);
		if	(s_aTouched[KEY_B] && g_hero.m_wIsReadyToFireChargedMiniPetSkill2)
			s_agent.sendEtcWork(eCEW_USE_MINIPET_CHARGE_SKILL,g_am.s_iFocusActor,1);
	}

#ifdef	_DEBUG
	if (s_aTouched[KEY_U])
	{
		CG_OPERATOR_COMMAND		packet;

		packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);
		packet.wCommand	=	eOC_SPRINKLE_GIFT;

		s_agent.sendOperatorCommand(&packet);
	}
#endif

#ifdef	_DEBUG
	#ifdef	_FOR_KOREA
		if	(s_aTouched[KEY_SPACE] && s_inputChatting.isEnable() == FALSE)
		{
			if	(g_lpControlActor)
			{
				CG_OPERATOR_COMMAND		packet;

				packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);
				packet.wCommand	=	eOC_SPRINKLE_GIFT;
				packet.wX		=	1;

				s_agent.sendOperatorCommand(&packet);
			}
			else
				g_hero.pickNearestItem();
		}
	#endif
#endif

	BOOL	bIsRightButtonWorkComplete	=	FALSE;
	CActor	*lpFocusActor				=	g_am.getTestedActor(s_iFocusActor,FALSE);

	if	(s_aKey[KEY_LSHIFT]	&& s_isClickedRightButton)
	{
		if	(lpFocusActor	&&	lpFocusActor->isPlayer() && !lpFocusActor->isHero())
		{
			s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
			s_ttCommon.hide();

			s_pmCommon.addMenu(ePM_PRM_TRADE			,dMSG_TRADE);
			s_pmCommon.addMenu(ePM_PRM_ASK_JOIN_PARTY	,dMSG_ASK_JOIN_PARTY);
			s_pmCommon.addMenu(ePM_PRM_SAY				,dMSG_SAY);
			s_pmCommon.addMenu(ePM_PRM_ADD_FRIEND		,dMSG_ADD_FRIEND);
			s_pmCommon.addMenu(ePM_PRM_DENY_COMMUNITY	,dMSG_DENY_COMMUNITY);
#ifdef	_FOR_JAPAN
			s_pmCommon.addMenu(ePM_PRM_NO_MANNER_POINT	,dMSG_NO_MANER_POINT);
#endif
			strcpy(s_strBookedUserName,lpFocusActor->m_strName);
			cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
		}
	}

#ifdef	_DEBUG
	if	(s_aKey[KEY_LALT]	&& s_isClickedRightButton)
		if	(lpFocusActor	&&	lpFocusActor->isMonster()	&&	g_hero.m_wOperatorLevel >= 2	)
		{
			CJobInfo	*lpBaseJob	=	lpFocusActor->getJob();

			g_am.m_iBookedTarget	=	lpFocusActor->m_wSerial;

			if	(lpBaseJob)
			{
				int			iBody		=	lpBaseJob->m_iBody;
				int			iVariation	=	lpBaseJob->m_iVariation;

				s_pmCommon.popup(s_posMouse.x,s_posMouse.y);
				s_ttCommon.hide();

				for	(int i=0;i<dMAX_JOB_COUNT;i++)
				{
					CJobInfo	*lpJob	=	g_jm.getJob(i);

					if	(lpJob->m_wSerial	==	0xffff)
						continue;

					if	(lpJob->m_iBody	!=	iBody	||	lpJob->m_iVariation	!=	iVariation)
						continue;

					char	strText[256];

					sprintf(strText,"%s[%s]",lpJob->m_strName,g_pstrMonsterGrade[lpJob->m_wRank]);

					s_pmCommon.addMenu(ePM_CHANGE_MONSTER_JOB_BEGIN+i,strText);
				}

				s_pmCommon.addMenu(ePM_TAME_MONSTER_BEGIN+lpFocusActor->m_wSerial,"Tame");

				cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);

				bIsRightButtonWorkComplete	=	TRUE;
			}
		}
#endif

	//g_am.OperateObserverMouse();

	if	((lpFocusActor	==	NULL	||	lpFocusActor->isPlayer() == FALSE)	&&	bIsRightButtonWorkComplete	==	FALSE	&&	
			s_aKey[KEY_LSHIFT]	&& s_isTouchRightButton && g_hero.m_wOperatorLevel >= 2	&&	s_tlChat.getFocusLine(s_posMouse.x,s_posMouse.y)	==	-1)
	{
		CG_OPERATOR_COMMAND	packet;

		packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);

		packet.wCommand	=	eOC_CHANGE_LOCATE;
		packet.wX		=	s_posCursor.x;
		packet.wY		=	s_posCursor.y;

		CPos	pos;

		if	(GetMMCPosToReal(&pos))
		{
			packet.wX		=	pos.x;
			packet.wY		=	pos.y;
		}

		g_lpHero->stop(packet.wX,packet.wY);

		s_agent.sendOperatorCommand(&packet);
	}

	if	(s_aTouched[KEY_UP]	&& s_inputChatting.isEnable() && s_bIsChattingCompositionState == FALSE)
	{
		s_inputChatting.getStr(0,s_strChatMessage,sizeof(s_strChatMessage));

		if	(s_strChatMessage[0] == '/')
		{
			char	*lpstrName	=	GetNextWhisperName();

			if	(lpstrName)
				s_inputChatting.setStr(0,_ms("/%s ",lpstrName));
		}

		if	(s_strChatMessage[0] == '\\' && g_hero.m_wOperatorLevel >= 2)
		{
			CDecoder	dec;

			dec.UploadBuffer(s_strChatMessage+1,NULL);

			char	*lpstrWord	=	dec.getWord();

			if	(lpstrWord)
			{
				char	*lpstrName	=	g_am.getNextPlayerName();

				if (lpstrName)	s_inputChatting.setStr(0,_ms("\\%s %s",lpstrWord,lpstrName));
			}
		}
	}

	if	(s_aTouched[KEY_DOWN] && s_inputChatting.isEnable()  && s_bIsChattingCompositionState == FALSE)
	{
		s_inputChatting.getStr(0,s_strChatMessage,sizeof(s_strChatMessage));

		if	(s_strChatMessage[0] == '/')
		{
			char	*lpstrName	=	GetPreviousWhisperName();

			if (lpstrName)
				s_inputChatting.setStr(0,_ms("/%s ",lpstrName));
		}

		if	(s_strChatMessage[0] == '\\' && g_hero.m_wOperatorLevel >= 2)
		{
			CDecoder	dec;

			dec.UploadBuffer(s_strChatMessage+1,NULL);

			char	*lpstrWord		=	dec.getWord();

			if	(lpstrWord)
			{
				char	*lpstrName	=	g_am.getPreviousPlayerName();

				if (lpstrName)	s_inputChatting.setStr(0,_ms("\\%s %s",lpstrWord,lpstrName));
			}
		}
	}

#ifdef	_DEBUG
		operateMoveByKey();
#endif

	if	(s_aTouched[KEY_ESCAPE]	)
	{
		BOOL	isCloseInterface	=	FALSE;

		resetKarmaDrawImage();

		if (s_propose.m_dwValue)
		{
			s_propose.m_dwValue		=	0;
			isCloseInterface		=	TRUE;
			cINPDEV::ReleaseExclusive();
		}
		else
		if (g_lpHero->m_wTalkTime)
		{
			g_lpHero->m_wTalkTime	=	0;
			isCloseInterface		=	TRUE;
		}
		else
		if	(s_inputChatting.isEnable() && g_gwPitchmanShop.isOpened()	==	FALSE)
		{
			s_bIsBookedDisableChattingBox	=	TRUE;
			isCloseInterface				=	TRUE;
		}
		else
		if	(g_itemCountWindow.m_bIsOpend)
		{
			g_itemCountWindow.reset();
			isCloseInterface	=	TRUE;
		}
		else
		if	(g_dialogBox.isTalking())
		{
			if(g_dialogBox.m_bDoNotCloseDialogBox)
			{
				g_dialogBox.m_bDoNotCloseDialogBox = FALSE;
				ClosePopupWindow();
			}
			g_dialogBox.closeComplexDialog();
			isCloseInterface	=	TRUE;
		}
		else
		if	(ClosePopupWindow())
			isCloseInterface	=	TRUE;

		if	(!isCloseInterface)
			PopupSystemMenu();
	}

	if( s_aKey[KEY_LCONTROL])  
	{
		for(int i=KEY_A;i<=KEY_F;++i)
		{
			if( s_aTouched[i])
			{
				changeChattingFilter(i-KEY_A);
			}
		}
	}

	if	(s_inputChatting.isEnable())
	{
		if(s_aKey[KEY_SPACE])
		{
			if(strlen(s_strChatMessage)<=4)
			{
				if(STRICMP(s_strChatMessage,"/a ") == 0)
				{
					changeChattingFilter(eCMF_NORMAL_CHAT);
					s_inputChatting.reset();
				}
				if(STRICMP(s_strChatMessage,"/p ") == 0)
				{
					changeChattingFilter(eCMF_PARTY_CHAT);
					s_inputChatting.reset();
				}
				if(STRICMP(s_strChatMessage,"/g ") == 0)
				{
					changeChattingFilter(eCMF_GUILD_CHAT);
					s_inputChatting.reset();
				}
				
				if(STRICMP(s_strChatMessage,"/w ") == 0)
				{
					changeChattingFilter(eCMF_WHISPER);
					s_inputChatting.reset();
				}
				
#ifdef _FOR_KOREA
				if(STRICMP(s_strChatMessage,"/ㅁ ") == 0)
				{
					changeChattingFilter(eCMF_NORMAL_CHAT);
					s_inputChatting.reset();
				}
				if(STRICMP(s_strChatMessage,"/ㅔ ") == 0)
				{
					changeChattingFilter(eCMF_PARTY_CHAT);
					s_inputChatting.reset();
				}
				if(STRICMP(s_strChatMessage,"/ㅎ ") == 0)
				{
					changeChattingFilter(eCMF_GUILD_CHAT);
					s_inputChatting.reset();
				}
				if(STRICMP(s_strChatMessage,"/ㅈ ") == 0)
				{
					changeChattingFilter(eCMF_WHISPER);
					s_inputChatting.reset();
				}			
#endif
			}
		}
		return;
	}
	if	(s_aTouched[KEY_Z]	)
		g_hero.changeMoveAbility();	//	걷거나 뛰는 중에도 전환 가능하게 하려구..

	if	(s_aTouched[KEY_TAB]	&&	s_iDragSkill	==	0xffff)
		g_hero.rotateQuickSlot();

	//JBC 월드맵 단축키로 키기.				08-07-03
	if	(s_aTouched[KEY_M])
	{
		if	(g_gwWorldMap.isOpened())
			ClosePopupWindow();
		else
			s_dwResultPopupwindow = eMMTB_WORLD_MAP;
	}
	if (s_aTouched[KEY_J])
	{
		if (g_map.m_tgaMinimap.m_p16IMAGE)
			g_map.m_bFogmapState = (g_map.m_bFogmapState)? FALSE : TRUE;
	}

	if (s_aTouched[KEY_N])
	{
		g_map.m_bQuestListState = (g_map.m_bQuestListState)? FALSE : TRUE;
	}
	if (s_aTouched[KEY_O])
	{
		g_map.m_bOptionState = (g_map.m_bOptionState)? FALSE : TRUE;
		g_map.m_rtOptionWnd.set(0xffff, 0xffff, 0xffff, 0xffff);
	}
	if (s_aTouched[KEY_SLASH])
	{
		s_iChatFilter++;
		if (s_iChatFilter > eCMF_WHISPER)
			s_iChatFilter = eCMF_NORMAL_CHAT;
		if (s_iChatFilter == eCMF_GUILD_CHAT && !g_hero.isGuildMember())
			s_iChatFilter = eCMF_WHISPER;
		if (g_config.m_iChattingBoxScale & 0x8000)
			ToggleChattingBoxScale();
		else
			RebuildChatMessageBox();
	}
	if (s_aTouched[KEY_COMMA])
	{
		if	(g_config.m_iChattingBoxScale & 0x8000)
			ToggleChattingBoxScale();
		
		g_config.m_iChattingBoxScale++;
		
		if	(g_config.m_iChattingBoxScale	>	dCHAT_BOX_MAX_SCALE-1)
			g_config.m_iChattingBoxScale	=	0;
		
		RebuildChatMessageBoxFrame(FALSE);
	}
	if (s_aTouched[KEY_PERIOD])
	{
		if	(g_config.m_iChattingBoxScale & 0x8000)
			ToggleChattingBoxScale();
		
		g_config.m_iChattingBoxScale--;
		
		if	(g_config.m_iChattingBoxScale	<	0)
			g_config.m_iChattingBoxScale	=	dCHAT_BOX_MAX_SCALE-1;
		
		RebuildChatMessageBoxFrame(FALSE);
	}
	//JBC 미니맵 축소 확대 단축키로 하기.	08-07-03
	if	(s_aTouched[KEY_PAD_PLUS] || s_aTouched[KEY_PLUS])
		s_dwResultPopupwindow = eMMTB_ZOOM_PLUS;
	if	(s_aTouched[KEY_PAD_MINUS] || s_aTouched[KEY_MINUS])
		s_dwResultPopupwindow = eMMTB_ZOOM_MINUS;
		// 미니맵 최대 / 최소 단축키. 09.06.24 
	if	(s_aTouched[KEY_LBRACKET])
		s_dwResultPopupwindow = eMMTB_ZOOM_MAX;
	if	(s_aTouched[KEY_RBRACKET])
		s_dwResultPopupwindow = eMMTB_ZOOM_MIN;

	////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////KH_2//////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////	

	if	(s_aKey[KEY_LSHIFT]	&& s_aKey[KEY_LCONTROL] && s_aTouched[KEY_SPACE])
	{
		g_config.m_bIsDoubleSizeTargetting	=	1-g_config.m_bIsDoubleSizeTargetting;

		if	(g_config.m_bIsDoubleSizeTargetting)
			WarningMessage("Active Double Size Targetting!!");
		else
			WarningMessage("Dis-active Double Size Targetting!!");
			


	}
	
	if( s_aKey[KEY_LCONTROL])  //lctrl+key1~10 + QuickSkilSlot 
	{
		for( int i = 0; i < 10; i++ )
		{
			if( s_aKey[KEY_1+i] )
			{	
				g_hero.m_wCurrentQuickSkillSlot = i;
				break;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////
}

void
CGamePlay::operateMoveByKey()
{
	if(IsOpenedEventWindow()  || s_inputChatting.isEnable())
		return;

	if( !(s_aKey[KEY_DOWN] + s_aKey[KEY_UP] + s_aKey[KEY_LEFT] + s_aKey[KEY_RIGHT]) ) 
		return;

	int iHeroMoveX			=	g_lpHero->m_pos.x;
	int iHeroMoveY			=	g_lpHero->m_pos.y;

	if(s_aKey[KEY_DOWN] && s_aKey[KEY_LEFT])
	{
		iHeroMoveX	-=	dTILE_XS;
		iHeroMoveY	+=	dTILE_YS;
	}
	else if(s_aKey[KEY_DOWN] && s_aKey[KEY_RIGHT])
	{
		iHeroMoveX	+=	dTILE_XS;
		iHeroMoveY	+=	dTILE_YS;
	}
	else if(s_aKey[KEY_UP] && s_aKey[KEY_LEFT])
	{
		iHeroMoveX	-=	dTILE_XS;
		iHeroMoveY	-=	dTILE_YS;
	}
	else if(s_aKey[KEY_UP] && s_aKey[KEY_RIGHT])
	{
		iHeroMoveX	+=	dTILE_XS;
		iHeroMoveY	-=	dTILE_YS;
	}
	else if	(s_aKey[KEY_DOWN])
	{
		iHeroMoveY	+=	dTILE_YS;
	}
	else if	(s_aKey[KEY_UP])
	{
		iHeroMoveY	-=	dTILE_YS;
	}
	else if	(s_aKey[KEY_LEFT])
	{
		iHeroMoveX	-=	dTILE_XS;
	}
	else if	(s_aKey[KEY_RIGHT])
	{
		iHeroMoveX	+=	dTILE_XS;
	}

	g_hero.moveTo(iHeroMoveX,iHeroMoveY);
	
}


//
//	패킷 처리
inline	void
CGamePlay::OperatePacket()
{
	if (s_agent.isDisconnected())
	{
		g_msgBox.cPopup(NULL,s_agent.m_strDisconnectText,dMSG_OK);
		g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
		s_agent.reset();

		return;
	}

	s_agent.packetHandler();
}

//
//	프로포즈*-_-* 작업들을 처리한다.
inline	void
CGamePlay::OperateProposeWorks()
{
	if	(s_propose.m_dwValue	==	0)
		return;

	CActor	*lpActor	=	g_am.getTestedActor(s_iFocusActor,FALSE);

	if	(s_isClickedRightButton)
	{
		s_propose.m_dwValue		=	0;

		cINPDEV::ReleaseExclusive();
		
		s_isClickedRightButton	=	FALSE;
	}

	if	(!s_isClickedLeftButton)
		return;

	if	(	s_propose.m_work.isProposeToParty+s_propose.m_work.isReadyToAddFriend+s_propose.m_work.isProposeToTrade+s_propose.m_work.isReadyUseItemToCharacter+
			s_propose.m_work.isReadyAskJoinGuild + s_propose.m_work.isProposeToUnionParty + s_propose.m_work.isReadyUseItemToEnemy)
	{
		if	(lpActor == NULL && s_isClickedLeftButton)
			return;
	}

	if	(s_propose.m_work.isProposeToUnionParty)
	{
		if	(g_lpHero->isPartyLeader())
		{
			s_agent.sendAskJoinParty(lpActor->m_strName , FALSE , TRUE);
			g_lpHero->warning(_ms(dMSG_PROPOSE_PARTY_FORM,lpActor->m_strName));
		}
		else
			g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE);
	}

	
	if	(s_propose.m_work.isProposeToParty)
	{
		if	(!g_lpHero->isPartyMember() || g_lpHero->isPartyLeader())
		{
			s_agent.sendAskJoinParty(lpActor->m_strName);
			g_lpHero->warning(_ms(dMSG_PROPOSE_PARTY_FORM,lpActor->m_strName));
		}
		else
			g_lpHero->warning(dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE);
	}

	if	(s_propose.m_work.isReadyToAddFriend)
	{
		if (g_hero.isFriend(lpActor->m_strName))
			g_lpHero->warning(dMSG_PLAYER_ARE_ALLREADY_FRIEND);
		else
			s_agent.sendRequestAddFriend(lpActor->m_strName);
	}

	if	(s_propose.m_work.isReadyAskJoinGuild)
		s_agent.sendRequestJoinGuild(lpActor->m_wSerial);

	if	(s_propose.m_work.isProposeToTrade)
	{
		if (!IsOpenTradeBox())
		{
			s_agent.sendRequestTrade(lpActor->m_strName,lpActor->m_wSerial);
			g_lpHero->warning(_ms(dMSG_PROPOSE_TRADE_FORM,lpActor->m_strName));
		}
	}

	if	(s_propose.m_work.isReadyUseItemToCharacter || s_propose.m_work.isReadyUseItemToCorpse ||  s_propose.m_work.isReadyUseItemToEnemy)
		g_hero.askUseItemToCharacter(s_iReadyToUseItem,s_iFocusActor);
	
	if	(s_propose.m_work.isReadyUseItemToItem)
	{
		int	iFocusItem	=	GetFocusItemInInventoryWindow();
		g_hero.askUseItemToItem(s_iReadyToUseItem,iFocusItem);
	}

	s_propose.m_dwValue			=	0;

	cINPDEV::ReleaseExclusive();
}

void
CGamePlay::OperateSystemMessageAboutHero()
{
	if	(g_hero.m_bf1IsMoveByEndOfGuildBattle)
	{
		g_hero.m_bf1IsMoveByEndOfGuildBattle	=	FALSE;

		if	(g_bIsDuelServer)
			AddSystemMessage(LTYELLOW,dMSG_IS_MOVE_BY_END_OF_DUEL);
		else
			AddSystemMessage(LTYELLOW,dMSG_IS_MOVE_BY_END_OF_GUILD_BATTLE);
	}
	if	(g_hero.m_bf1IsMoveByNotGuildBattleTime)
	{
		g_hero.m_bf1IsMoveByNotGuildBattleTime	=	FALSE;
		AddSystemMessage(LTYELLOW,dMSG_IS_MOVE_BY_NOT_GUILD_BATTLE_TIME);
	}
	if	(g_hero.m_bf1IsMoveByOXQuizProcess)
	{
		g_hero.m_bf1IsMoveByOXQuizProcess	=	FALSE;
		AddSystemMessage(LTYELLOW,dMSG_IS_MOVE_BY_ALREADY_OX_QUIZ_PROCESS);
	}
	if	(g_hero.m_bf1IsMoveByNotOXQuizTime)
	{
		g_hero.m_bf1IsMoveByNotOXQuizTime	=	FALSE;
		AddSystemMessage(LTYELLOW,dMSG_IS_MOVE_BY_NOT_OX_QUIZ_TIME);
	}
	if	(g_hero.m_bf1IsMoveByEndOfOXQuiz)
	{
		g_hero.m_bf1IsMoveByEndOfOXQuiz		=	FALSE;
		AddSystemMessage(LTYELLOW,dMSG_IS_MOVE_BY_END_OF_OX_QUIZ);
	}
	if	(g_hero.m_bf1IsBannedSecretDungeon)
	{
		g_hero.m_bf1IsBannedSecretDungeon	=	FALSE;
		AddSystemMessage(LTYELLOW,dMSG_FAILED_TO_JOIN_GAME_BY_NOT_SECRET_DUNGEON_MEMBER);
	}
}

void
CGamePlay::OperateEventWork()
{
	if	(s_bIsProgressEvent	==	FALSE)
	{
		g_countDown.update();
		g_eventCountDown.update();
	}

	if	(g_eventOxQuiz.m_wIsCountDown)
	{
		char	strQuestion[512];

		sprintf(strQuestion,"%s %s",g_eventOxQuiz.m_strQuestion,dMSG_OX_QUIZ_TAIL);

		Notice(strQuestion);
	}

	if	(g_eventWordQuiz.m_wIsCountDown)
		Notice(g_eventWordQuiz.m_strQuestion);

	if	(s_bIsOXQuizField	&& g_eventOxQuiz.m_bIsWaitBegin)
		Notice(g_eventOxQuiz.m_strBeginTimeAnnounce);
	if	(s_bIsWordQuizField	&& g_eventWordQuiz.m_bIsWaitBegin)
		Notice(g_eventWordQuiz.m_strBeginTimeAnnounce);
}

void
CGamePlay::HideTooltip()
{
	if (s_iDragItem	!=	0xffff)
		s_ttCommon.hide();
	if (s_iDragSkill!=	0xffff)
		s_ttCommon.hide();
	if (s_iDragTradeItem	!=	0xffff	&&	IsOpenTradeBox())
		s_ttCommon.hide();
	if (g_gwPitchmanShop.isOpened()	&& g_gwPitchmanShop.m_iDragItem	!=	0xffff)
		s_ttCommon.hide();
	if (g_gwBank.m_iDragBankItem != 0xffff && s_iPopupInterface == ePIW_BANK)
		s_ttCommon.hide();
	if (g_gwCart.m_iDragCartItem != 0xffff && s_iPopupInterface == ePIW_SHOPPING_CART)
		s_ttCommon.hide();
}

BOOL
CGamePlay::CheckIncorrectStatus()
{
	if (s_partyInfo.isIncorrectData())
	{
		s_partyInfo.m_wIsWaitPartyInfo	=	TRUE;
		s_agent.sendAskPartyInfo();
	}

	if (!g_msgBox.isPoped() && s_iStep == dSTEP_MAIN && s_iBookingStep != dSTEP_FINISH)
		if (g_lpHero->isIncorrectName())
		{
			g_msgBox.cPopup(NULL,dMSG_INCORRECT_CHARACTER_NAME,dMSG_OK);
			g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
			s_agent.reset();
			return	TRUE;
		}

	return	FALSE;
}

void
CGamePlay::OperateMenuWindow()
{
	if (s_iPopupInterface	==	ePIW_NONE)
	{
		s_lpInventoryWindowItemList		=	g_hero.m_aItems;
		s_lpInventoryWindowEquipmentList=	g_hero.m_aEquip;
	}

	if	(g_lpHero && g_lpHero->isDeath() && g_bIsDuelServer	== FALSE)
	{
		if	(s_iPopupInterface	!=	ePIW_SYSTEM)
			PopupSystemMenu();

		UpdateSystemMenu();
	}
	else if(s_bIsCanNotControlInterface == FALSE)
	{

		UpdateBottomInterface();	//	하단 인터페이스 업데이트
		UpdateInventoryWindow();
		UpdateStatusWindow();
		UpdateSystemMenu();
		UpdateTradeBox();

		g_gwFriend.update();
		g_gwQuest.update();
		g_gwShop.update();	//	상점 업데이트
		g_gwCarrotShop.update();	//	상점 업데이트
		g_gwSelectItemInPack.update();	//	상점 업데이트
		g_gwBottomlessBoxWindow.update();
		g_gwBook.update();	
		g_gwBookMonDic.update();	
		g_gwRepairItem.update();	//	상점 업데이트
		g_gwSelectVillage.update();
#ifdef	_DEBUG
		g_gwDebug.update();
#endif
		g_gwParty.update();
		g_gwRemovePrefix.update();
		g_gwMergeItem.update();
		g_gwJudgeToItem.update();
		g_gwSkill.update();
		g_gwCommonList.update();
		g_commonUI.update();
		g_gwBank.update();
		g_gwGuild.update();
		g_gwPitchmanShop.update();
		g_gwWorldMap.update();
		g_gwMainQuestEvent.update();
		g_gwHelp.update();
		g_gwMakeGuild.update();
		g_gwCart.update();
		g_gwPutBannerWnd.update();
		g_gwGuildBattleScheduler.update();
		g_gwGuildHallList.update();
		g_gwGuildRelatedPlaceList.update();
		g_gwGuildMarkComposer.update();
		g_gwGuildInventory.update();
		g_gwFeedGuildPet.update();
		g_gwHonorPointList.update();
		g_gwCopyOrCutPrefix.update();		// 복사, 잘라내기 창 업데이트
		if	(s_iPopupInterface	!=	ePIW_NONE)
			UpdatePopupWindowCloseButton();
		g_gwUpgradeShop.update();		// 강화 창 업데이트 ~~~~09.08.25
		OperateDragItem();
	}
}

//
//	업데이트
BOOL
CGamePlay::UpdateMain()
{
	if	(s_bIsProgressEvent)
		cINPDEV::Reset();

#ifdef dDEBUG_MODE
	UpdateDebugMenu();
#endif

	if	(g_bIsHalfSize)
	{
		s_posCursor.x	=	s_posMouse.x*2	+ g_map.m_posView.x*2;
		s_posCursor.y	=	s_posMouse.y*2	+ g_map.m_posView.y*2;
	}
	else
	{
		s_posCursor.x	=	s_posMouse.x	+ g_map.m_pos.x;
		s_posCursor.y	=	s_posMouse.y	+ g_map.m_pos.y;
	}

	s_posCursor.mx	=	s_posCursor.x	>>	dTILE_XSIZE_SHIFT;
	s_posCursor.my	=	s_posCursor.y	>>	dTILE_YSIZE_SHIFT;

	if	(g_msgBox.isPoped())
	{
		s_propose.m_dwValue	=	0;
		s_pmCommon.reset();
	}

	if	(g_map.m_object.m_bIsCheckObjectInfoFailed)
	{
		g_map.m_object.m_bIsCheckObjectInfoFailed	=	FALSE;
#ifdef	_FOR_KOREA
		g_msgBox.cPopup("","오브젝트 데이터에 문제가 있습니다. 일부 오브젝트가 보이지 않거나 이상하게 나올 수 있습니다. 홈페이지에서 오브젝트 패치를 다운 받으시기 바랍니다.",dMSG_OK);
#else
	#ifdef	_DEBUG
		g_msgBox.cPopup("","Incorrect Object Data!! Please Exit Game. and Check log.txt File",dMSG_OK);
	#endif
#endif
	}
	
//	s_unionPartyInfo.updateActorList();
	OperateSystemMessageAboutHero();
	OperateProposeWorks();

	OperateChatMessageBox();
	OperatePopupMenu();
	OperateMessageBox();
	OperatePacket();

	OperateKeyboard();
	OperateSystemMessageBox();
	g_duelManager.update();
	g_logChat.update();	//	채팅 로그 업데이트
	g_pitchmanShop.update();
	g_portal.update();
	g_arcaManager.update();
	g_bannerManager.update();
	g_bossMonsterEvent.update();

	g_dialogBox.updateComplexDialog();		//	대화 업데이트
	OperateMenuWindow();
	UpdateMinimap();
	UpdateMemberNameBarAtField();	//	파티 멤버 네임 업데이트
	s_fieldTitle.update();			//	필드 타이틀 업데이트
	s_UpgradeFieldTitle.update();	// 강화 성공실패 타이틀 보기 09.09.02	
	g_am.update();		//	액터들 업데이트(키보드/마우스 입력 포함)
	g_fieldItem.update();
	g_trapManager.update();
	g_sm.update();					//	스킬 업데이트
	g_gpManager.update();
	g_rapeEffect.update();			//	필드 이펙트
	g_standEffect.update();			//	필드 이펙트
	g_floatEffect.update();			//	필드 이펙트
	g_fireCrackerManager.update(s_aTouched[KEY_SPACE],g_lpHero->m_pos.x,g_lpHero->m_pos.y,s_posCursor.x,s_posCursor.y,s_bIs1Second);	//	불꽃 놀이

	s_agent.clearReceivedPacketCheck();	//	패킷 받은거 정보 초기화
	ResetNetworkEvent();				//	네트워크 이벤트 초기화

	if	(g_iCountDownForSelfDestruction && (s_iFrameCounter % dSYNC_FPS) == 0)
	{
		g_iCountDownForSelfDestruction--;

		if	(g_iCountDownForSelfDestruction	==	0)
		{
			while(1)
			{
				g_shaker.update();
				Sleep(1);
			}
		}
	}
	
	if	(s_iFrameCounter%2==0)
		g_shaker.update();

	if	(CheckIncorrectStatus())
		return	TRUE;

	OperateEventWork();
	HideTooltip();
	UpdateCursorShape();
	SendAskItemPackName();
	SendAskArcaInfo();
	updateKarmaDrawImage();
#ifdef	_USE_NPGL
	if (g_bIsUseNPGL)
		if (random(dSYNC_FPS*10) == 0)
			if (g_npgl->Check()	!=	NPGAMEMON_SUCCESS)
			{
				s_isRunApplication	=	FALSE;

				g_eh.outputStaticLog(_ms("%s/log.txt",g_strLogFolder));
			}
#endif

	return TRUE;
}

BOOL				
CGamePlay::addKarmaDrawImage(WORD	_wIndex,  int _iX, int _iY, WORD	_wTime, WORD _wDrawingMethod, WORD _wDistance)
{
	for(int i=0;i<dMAX_KARMA_DRAW_IMAGE;++i)
	{
		if(m_karmaDrawImage[i].m_wIndex == 0xffff)
		{
			if(_wTime == 0)
				_wTime = -1;	// -1은 무한이다.
			m_karmaDrawImage[i].m_wIndex		=	_wIndex;
			m_karmaDrawImage[i].m_pos.Set( _iX,_iY  );
			m_karmaDrawImage[i].m_posCurrent.Set( _iX,_iY );
			m_karmaDrawImage[i].m_wTime			=	_wTime;	
			m_karmaDrawImage[i].m_wDrawingMethod	=	_wDrawingMethod;
			m_karmaDrawImage[i].m_wDistance		=	_wDistance;
			m_karmaDrawImage[i].m_wFrameCounter	=	0;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL				
CGamePlay::updateKarmaDrawImage()
{	
	for(int i=0;i<dMAX_KARMA_DRAW_IMAGE;++i)
	{
		if(m_karmaDrawImage[i].m_wIndex	== 0xffff)
			continue;
		if(m_karmaDrawImage[i].m_wFrameCounter >=0xfffe)
			m_karmaDrawImage[i].m_wFrameCounter = 0;
		if(s_bIs1Second)
		{
			if(m_karmaDrawImage[i].m_wTime > 0)
				--m_karmaDrawImage[i].m_wTime;	
		}
		if(m_karmaDrawImage[i].m_wTime == 0 )
		{
			m_karmaDrawImage[i].reset();
		}
		else
		{
			float fX = 0;
			float fY = 0;

			switch(m_karmaDrawImage[i].m_wDrawingMethod)
			{
			case eDM_NONE					:
				break;	
			case eDM_LOOP_UP_AND_DOWN		:
				{
					float fCosValue	= dCOS_TABLE[m_karmaDrawImage[i].m_wFrameCounter*5%360]/256.0 *   m_karmaDrawImage[i].m_wDistance/2.0 +m_karmaDrawImage[i].m_wDistance/2.0;
					fY = fCosValue;
					
					break;
				}
			case eDM_LOOP_LEFT_AND_RIGHT	:
				{
					float fCosValue	= dCOS_TABLE[m_karmaDrawImage[i].m_wFrameCounter*5%360]/256.0 *  m_karmaDrawImage[i].m_wDistance/2.0 +m_karmaDrawImage[i].m_wDistance/2.0;
					fX = fCosValue;
					break;
				}
			}
			m_karmaDrawImage[i].m_posCurrent.Set(m_karmaDrawImage[i].m_pos.x +fX ,m_karmaDrawImage[i].m_pos.y + fY);
		}
		++m_karmaDrawImage[i].m_wFrameCounter;
	}
	return TRUE;
}

BOOL				
CGamePlay::offKarmaDrawImage(WORD	_wIndex,  int _iX, int _iY)
{
	for(int i=0;i<dMAX_KARMA_DRAW_IMAGE;++i)
	{
		if(m_karmaDrawImage[i].m_wIndex == _wIndex)
		{
			if(m_karmaDrawImage[i].m_pos.x == _iX && m_karmaDrawImage[i].m_pos.y == _iY)
			{
				m_karmaDrawImage[i].reset();
				return TRUE;
			}
		}
	}

	return FALSE;
}


BOOL				
CGamePlay::isExistKarmaDrawImage()
{
	for(int i=0;i<dMAX_KARMA_DRAW_IMAGE;++i)
	{
		if(m_karmaDrawImage[i].m_wIndex != 0xffff)
		{
			return TRUE;
		}
	}	
	return FALSE;
}



BOOL				
CGamePlay::resetKarmaDrawImage()
{
	for(int i=0;i<dMAX_KARMA_DRAW_IMAGE;++i)
	{
		m_karmaDrawImage[i].reset();
	}	
	return TRUE;
}

void
CGamePlay::changeEventObject(BOOL _bIsEvent)
{
/*	
	if(_bIsEvent)
	{
// 이벤트 오브젝트 로딩		
	}
	else
	{
// 기존 오브젝트 로딩		
	}
	
*/	  
}



void
CGamePlay::UpdateCursorShape()
{
	if	(g_msgBox.isPoped() || s_pmCommon.isPopuped() || g_gwBook.isOpened()||g_gwBookMonDic.isOpened())
	{
		ChangeCursorShape(s_hNormalCursor);
		return;
	}

	if	(s_bIsProgressEvent)
	{
		ChangeCursorShape(s_hEventCursor);
		return;
	}

	if	(g_gwMainQuestEvent.isOpened() || g_bossMonsterEvent.isAvail())
	{
		if	(g_gwMainQuestEvent.m_bIsHideCursor || g_bossMonsterEvent.isAvail())
			ChangeCursorShape(NULL);
		else
			ChangeCursorShape(s_hNormalCursor);
		return;
	}

	if	(s_iFocusActor	!=	0xffff)
	{
		CActor	*lpActor	=	g_am.getTestedActor(s_iFocusActor,FALSE);

		if	(lpActor)
		{
			if	(lpActor->isNpc() && lpActor->m_bf1IsTalker)
			{
				ChangeCursorShape(s_hDialogCursor);
				return;
			}
			else
			if	(!lpActor->isOwnTeam() && lpActor->isNpc() == FALSE)
			{
				ChangeCursorShape(s_hAttackCursor);
				return;
			}
			else
			if	(lpActor->isHero() && lpActor->m_wShakleSkill != 0xffff)
			{
				ChangeCursorShape(s_hAttackCursor);
				return;
			}
			else
			if	(lpActor->m_bf1IsFreezeShakle)
			{
				ChangeCursorShape(s_hAttackCursor);
				return;
			}
		}
	}

	if	(g_fieldItem.m_iFocusItem	!=	0xffff)
	{
		ChangeCursorShape(s_hPickCursor);
		return;
	}

	if	(g_dialogBox.m_iFocusSpeech	!=	-1 || g_pitchmanShop.m_wFocusShop	!=	0xffff)
	{
		ChangeCursorShape(s_hPointDialogCursor);
		return;
	}

	if	(s_iFocusTrap	!=	0xffff)
	{
		ChangeCursorShape(s_hDisarmCursor);
		return;
	}

	if	(g_map.m_area.m_iFocusedArea	!=	0xffff)
	{
		cAreaInfo	*lpArea	=	g_map.m_area.getFocusedArea();

		if	(!lpArea)
		{
			g_map.m_area.m_iFocusedArea	=	0xffff;

			return;
		}

		if	(lpArea->m_wKind	==	dAREA_DOOR)
		{
			cAreaDoorInfo	*lpAreaDoor	=	(cAreaDoorInfo	*)lpArea;

			if	(g_hero.isVisibleDoorTrap(lpAreaDoor))
			{
				ChangeCursorShape(s_hDisarmCursor);
				return;
			}

			if	(g_hero.isVisibleDoor(lpAreaDoor))
			{
				if	(lpAreaDoor->m_bf1IsUnlocked	==	FALSE)
				{
					if	(lpAreaDoor->m_bf1IsOpened)
						ChangeCursorShape(s_hOpenDoorCursor);
					else
						ChangeCursorShape(s_hUnlockCursor);
				}
				else
				{
					if	(lpAreaDoor->m_bf1IsOpened && lpAreaDoor->m_wLinkSecretDungeon && lpAreaDoor->m_wLinkSecretDungeon !=0xffff)
						ChangeCursorShape(s_hMoveFieldCursor);
					else
						ChangeCursorShape(s_hOpenDoorCursor);
				}
				return;
			}
		}

		if	(lpArea->m_wKind	==	dAREA_EVENT_AREA)		// 09.08.12
		{	
			if(	((cAreaEvent*)lpArea)->m_bf1Npc	){
				ChangeCursorShape(s_hDialogCursor);
			}else{
				ChangeCursorShape(s_hViewCursor);
			}
			return;
		}


		if	(lpArea->m_wKind	==	dAREA_TRAP)
		{
			cAreaTrapInfo	*lpAreaTrap	=	(cAreaTrapInfo	*)lpArea;

			if	(g_hero.isVisibleTrap(lpAreaTrap))
			{
				ChangeCursorShape(s_hDisarmCursor);
				return;
			}
		}

		if (lpArea->m_wKind	==	dT_AREA_ARCA)
		{
			cAreaArcaInfo	*lpAreaArca	=	(cAreaArcaInfo	*)lpArea;
			cArcaInfo		*lpArca		=	g_arcaManager.getArca(lpAreaArca->m_wLinkArca);

			if (lpArca && lpArca->m_wRemoveTimer	==	0)
			{
				if (lpArca->m_bf1IsArmedTrap)
					ChangeCursorShape(s_hDisarmCursor);
				else
				if (lpArca->m_bf1IsLocked)
					ChangeCursorShape(s_hUnlockCursor);
				else
					ChangeCursorShape(s_hOpenBoxCursor);
				return;
			}
		}
	}


	ChangeCursorShape(s_hNormalCursor);
}

//
//	게임 종료
void
CGamePlay::ExitGame(int _iType)
{
	switch(_iType)
	{
		case	eEGV_RETURN_TO_TITLE			:
			s_agent.reset();	//	연결 바로 끊자!!
			BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
			ClosePopupWindow();
			break;

		case	eEGV_EXIT_TO_WINDOW				:
			s_agent.reset();	//	연결 바로 끊자!!
			BookingMode(eGAME_CLOSE,dSTEP_FINISH,dFADE_OUT);
			ClosePopupWindow();
			break;
	}
}	//	CGamePlay::ExitGame(int _iType)

//
//	게임 종료
void
CGamePlay::ExitGame()
{
	g_msgBox.cPopup(NULL,dMSG_ARE_YOU_REALLY_EXIT_GAME,dMSG_YES,dMSG_NO);

	g_msgBox.setButtonId(eMBI_EXIT_TO_WINDOW);
}