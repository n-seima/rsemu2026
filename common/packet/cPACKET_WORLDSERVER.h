#ifndef _classPACKET_WORLDSERVER_H
#define _classPACKET_WORLDSERVER_H

#include "cPACKET_BASE.h"
#include "definePacketData.h"
#include "CGuildDefine.h"
#include "actorDefine.h"
#include "carrotShopDefine.h"
#include "eventDefine.h"
#include "cPACKET_GAMESERVER.h"
//	∏µÅE¿Ø¿˙¿« µ•¿Ã≈∏∏¶ ∞°¡ˆ∞ÅE¿÷¥Ÿ.

//	∞‘¿” º≠πˆø°º≠ ø‰√ª«œ¥¬ ¿€æ˜¿ª «ÿ¡ÿ¥Ÿ.
//	∞‘¿” º≠πˆ¿« ¡§∫∏, æ∆¿Ã≈€ ª˝º∫, ¿Ø¿˙µÈ∞£¿« ƒøπ¬¥œ∆º(ºº¿Ã, ±ÊµÅE ƒ£±∏), º≠πˆ∞£ ¿Ãµø

//	∆–≈∂¿« ±‚∫ª±∏º∫
//	type,size

#pragma pack(1)

class	cMSG_BASE_TYPE_FORWORLD
{
public:
	WORD	wSize,				//	∆–≈∂ ªÁ¿Ã¡ÅE			
			wType;				//	∆–≈∂ ≈∏¿‘
	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

/////////////////////////////////////////////////////////
//	PACKET TYPE
/////////////////////////////////////////////////////////
//	About In / Out
#define	dSERVERStoWORLD_LOGIN				0x8001//	¿⁄Ω≈(∞‘¿” º≠πÅE¿« ¡§∫∏∏¶ ≥—∞‹¡÷∞ÅEø¨∞·¿ª ¿Ø¡ˆ«—¥Ÿ.
#define	dSERVERStoWORLD_GETAVAS				0x8002//	æ∆πŸ≈∏¿« ¡§∫∏µÈ¿ª ∫∏≥ª¥ﬁ∂ÅE
#define	dSERVERStoWORLD_CREATEAVA			0x8003//	æ∆πŸ≈∏∏¶ ªı∑”∞‘ ∏∏µÈ¥Ÿ.
#define	dSERVERStoWORLD_DELETEAVA			0x8004//	æ∆πŸ≈∏∏¶ ªË¡¶«—¥Ÿ.
#define	dSERVERStoWORLD_LOADAVA				0x8005//	¥©±∫∞° µÈæ˚Ã•∞≈¥œ±ÅE. æ∆πŸ≈∏ ¡§∫∏ ∫“∑Øµ÷∂ÅE
#define	dSERVERStoWORLD_SAVEAVA				0x8006//	¿˙¿Â«—¥Ÿ. µ•¿Ã≈∏... -0-
#define	dSERVERStoWORLD_INFO				0x8007//	æ˚–¿ º“ƒœ¿∏∑Œ πﬁ¿ª¡ÅEæÀ∑¡¡ÿ¥Ÿ.
#define	dSERVERStoWORLD_LOGOUT				0x8008//	∞ÅE¡æ∑·«“ øπ¡§¿Ã¥œ±ÅE.. ¥ı¿ÃªÅE∆–≈∂¿ª ∫∏≥ª¡ÅE∏∂º≈¿Ø ~~ 
#define	dSERVERStoWORLD_JOIN_COMPLETE		0x8009//	¡∂¿Œ øœ∑ÅE- ¡§∫∏∏¶ ¡§ªÛ¿˚¿∏∑Œ ªÁøÅEœººøÅE

#define	dSERVERStoWORLD_USER_LOGIN			0x8011//	¿Ã ¿Ø¿˙∞° ¿⁄Ω≈ø°∞‘ µÈæ˚€‘¥Ÿ.
#define	dSERVERStoWORLD_USER_LOGOUT			0x8012//	¿Ã ¿Ø¿˙∞° ¿⁄Ω≈ø°∞‘º≠ ≥™∞¨¥Ÿ.
#define	dSERVERStoWORLD_GET_AVADATA			0x8013//	æ∆πŸ≈∏ µ•¿Ã≈∏∏¶ ø‰√ª.. µŒ∞°¡ÅE≈∏¿‘¿Ã ¡∏¿Á«—¥Ÿ.
#define	dSERVERStoWORLD_COPY_DATA			0x8014//	≈◊Ω∫∆Æ º≠πˆø° ªÁøÅE“ ƒ≥∏Ø≈Õ∏¶ ∫πªÁ«—¥Ÿ. πŸ∑Œ DBCø° ≥÷¿ª∞Õ!!

#define	dSERVERStoWORLD_ACCOUNT_INFO		0x8015
#define	dSERVERStoWORLD_CONFIRM_ENTER_FIELD	0x8016//
#define	dSERVERStoWORLD_PLAYER_COUNT		0x8017//
#define	dSERVERStoWORLD_LIFE_SIGN			0x8018//


#define	dWORLDtoSERVERS_LOGIN				0x8701//	guild¡§∫∏±˚›ÅE¥Ÿ ≥—∞‹¡·¿∏¥œ ¿Ã¡¶ æÀæ∆º≠«ÿ!!
#define	dWORLDtoSERVERS_GETAVAS				0x8702//	æ∆πŸ≈∏¿« ¡§∫∏µÈ¿ª ∫∏≥Ω¥Ÿ.
#define	dWORLDtoSERVERS_CREATEAVA			0x8703//	æ∆πŸ≈∏ ∏∏µÅE∞·∞ÅE
#define	dWORLDtoSERVERS_DELETEAVA			0x8704//	æ∆πŸ≈∏ ªË¡¶ ∞·∞ÅE
#define	dWORLDtoSERVERS_LOADAVA				0x8705//	æ∆πŸ≈∏ ¿Œ¡ÅE∞·∞ÅE
#define	dWORLDtoSERVERS_USER_LOGIN			0x8711//	¿Œ¡ı∏∏ »Æ¿Œ«œ∞ÅE
#define	dWORLDtoSERVERS_DIS					0x8712
#define	dWORLDtoSERVERS_GET_AVADATA			0x8713//	µ•¿Ã≈∏∏∏ ø‰√ª.. - ¿œπ›¿˚¿Œ ø‰√ª∞ÅE√π ∑Œ±◊¿Œ¿ª ¿ß«— ø‰√ª
#define	dWORLDtoSERVERS_USER_LOGOUT			0x8714//	¡§ªÛ¿˚¿∏∑Œ ∑Œ±◊æ∆øÅE«ﬂ¥Ÿ.
#define	dWORLDtoSERVERS_DISCONNECT_SERVER	0x8715//	∞‘¿”º≠πˆ∞° ¡◊æ˙¥Ÿ.
#define	dWORLDtoSERVERS_RENEW_PREMIUM_ITEM	0x8716

#define	dWORLDtoSERVERS_ACCOUNT_INFO		0x8717
#define	dWORLDtoSERVERS_CONFIRM_ENTER_FIELD	0x8718	//
#define	dWORLDtoSERVERS_SIMPLE_WORK			0x8719	//

#define	dWORLDtoSERVERS_DUEL_MATCH_INFO		0x871a
#define	dWORLDtoSERVERS_LIFE_SIGN			0x871b
#define	dD2W_SIMPLE_WORK					0x871c


//	About Map & User
#define	dSERVERStoWORLD_USER_MOVEMAP		0x8101//	¿⁄Ω≈¿Ã ∞°¡ˆ∞ÅE¿÷¡ÅEæ ¿∫ ∏ ¿∏∑Œ ¿Ø¿˙∞° ¿Ãµø«œ∑¡ «“∂ß
#define	dSERVERStoWORLD_USER_MOVEMAP_INFO	0x8104//	∏ ¿Ãµøø° « ø‰«— ¡§∫∏∏¶ ø˘µÂø°∞‘ æÀ∏∞¥Ÿ.
#define	dSERVERStoWORLD_USER_RECALL			0x8107//	¿Ã¬ ¿∏∑Œ ¿ÃµøΩ√ƒ—¡÷~~~
#define	dSERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE		0x8108//	OX ƒ˚¡ÅEΩ√¿€ ∏∂¿ª
#define	dSERVERStoWORLD_CALLING				0x810a
#define	dSERVERStoWORLD_BOOST_POWER_BY_EVENT	0x8128
#define	dSERVERStoWORLD_CHECK_USER_MOVEMAP	0x8129	//	¿⁄Ω≈¿Ã ∞°¡ˆ∞ÅE¿÷¡ÅEæ ¿∫ ∏ ¿∏∑Œ ¿Ø¿˙∞° ¿Ãµø«œ∑¡ «“∂ß
													//	SERVERStoWORLD_USER_MOVEMAP ø° ≈∏¿‘∏∏ πŸ≤„º≠ ªÁøÅE—¥Ÿ.
#define	dSERVERStoWORLD_SIMPLE_WORK			0x812a	//
#define	dSERVERStoWORLD_SOUND_OF_LEADERS_BELL	0x812c	
#define	dSERVERStoWORLD_CHANGE_MSG_OF_LEDERS_BELL	0x812d


#define	dWORLDtoSERVERS_USER_MOVEMAP		0x8801
#define	dWORLDtoSERVERS_USER_RECALL			0x8807//	æÀ±ﬂ¥Ÿ. ∞°∂ÅE~~~ 
#define	dWORLDtoSERVERS_CALLING				0x8808
#define	dWORLDtoSERVERS_BOOST_POWER_BY_EVENT				0x8809
#define	dWORLDtoSERVERS_CHANGE_DAY			0x880a
#define	dWORLDtoSERVERS_CHECK_USER_MOVEMAP	0x882d	//	« µÅE¿Ãµø √º≈© ∞·∞˙∏¶ ≥Ø∏∞¥Ÿ.
													//	SERVERStoWORLD_USER_MOVEMAP ∆–≈∂ø° ≈∏¿‘∏∏ πŸ≤ÅEªÁøÅE—¥Ÿ.
#define	dWORLDtoSERVERS_SOUND_OF_LEADERS_BELL	0x882e
#define dWORLDtoSERVERS_SYNC_CHANGEDNAME	0x882f


//	About Admin
#define	dSERVERStoWORLD_ADMIN_LOGIN			0x8102//	Login ¡§∫∏∏¶ πﬁæ∆º≠ Admin¿œ∞ÊøÅEΩ√Ω∫≈€ ∏˙”…æ˚‘¶ ªÁøÅE°¥…«œ∞‘..
#define	dSERVERStoWORLD_ALL_MSG				0x8103//	∞ÅEÆ¿⁄ ∏µÂ∞° WORLDø°∞‘ .. ∏µÅE¿Ø¿˙µÈø°∞‘ ¿ÅEﬁµ… ∏ﬁºº¡ÅE
#define	dSERVERStoWORLD_ISCONNECT_USER		0x8105//	¿Ã ∞Ë¡§¿« ¿Ø¿˙∞° «ˆ¿ÅE¿÷≥ƒ?
#define	dSERVERStoWORLD_OFFSERVER			0x8109//	ø˘µÂ∏¶ ¡æ∑·«—¥Ÿ. (∏µÅE∞‘¿”º≠πˆ∞° ¡æ∑·µ…∂ß±˚›ÅE±‚¥Ÿ∑»¥Ÿ∞°..)
#define	dSERVERStoWORLD_APPOINTMENT_QUIZ_EVENT		0x810b//	OX ƒ˚¡ÅEøπæÅE
#define	dSERVERStoWORLD_PRODUCE_OX_QUIZ		0x810c
#define	dSERVERStoWORLD_END_QUIZ_EVENT		0x810d
#define	dSERVERStoWORLD_CANCEL_QUIZ_EVENT		0x810e//	OX ƒ˚¡ÅE√ÅE“
#define	dSERVERStoWORLD_OX_QUIZ_LOSER_REVIVE_BATTLE	0x810f//	OX ƒ˚¡ÅE∆–¿⁄ ∫Œ»∞¿ÅE
#define dSERVERStoWORLD_CHANGE_FIXCHECK		0x8110
#define dSERVERStoWORLD_GET_FIXCHECK		0x8111
#define dSERVERStoWORLD_GET_GUILDLIST		0x8112
#define dSERVERStoWORLD_GET_GUILDINFO		0x8113
#define dSERVERStoWORLD_GET_GUILDMEMBERLIST				0x8114
#define dSERVERStoWORLD_BANISH_USER						0x8115
#define dSERVERStoWORLD_ADD_RENEW_ITEM_EXPIRE_TIME		0x8116
#define dSERVERStoWORLD_ASK_RENEW_ITEM_EXPIRE_TIME_LIST	0x8117
#define dSERVERStoWORLD_ASK_ITEM_PACK_LIST				0x8118
#define dSERVERStoWORLD_UPDATE_ITEM_PACK_INFO			0x8119
#define dSERVERStoWORLD_ITEM_PACK_INFO_CHECKSUM			0x811a
#define dSERVERStoWORLD_ASK_GR_INFO						0x811b
#define dSERVERStoWORLD_UPDATE_GR_INFO					0x811c
#define dSERVERStoWORLD_ASK_GUILD_NAME_LIST				0x811d
#define dSERVERStoWORLD_ASK_GUILD_NAME					0x811e
#define dSERVERStoWORLD_CHANGE_GUILD_BATTLE_REPORT		0x811f
#define dSERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE			0x8120
#define	dSERVERStoWORLD_RESET_DROP_PACK_ITEM_COUNT		0x8121
#define dSERVERStoWORLD_ASK_NEXT_ITEM_PACK_INFO			0x8122
#define dSERVERStoWORLD_ASK_CARROT_SHOP_INFO			0x8123
#define dSERVERStoWORLD_UPDATE_CARROT_SHOP_INFO			0x8124
#define dSERVERStoWORLD_PRODUCE_WORD_QUIZ				0x8125
#define dSERVERStoWORLD_CHANGE_WORD_QUIZ_ROUND			0x8127
#define dSERVERStoWORLD_CHANGE_EVENT_WORD				0x8130
#define dSERVERStoWORLD_WORD_EVENT_END					0x8131
#define dSERVERStoWORLD_WORD_EVENT_START				0x8132
#define dSERVERStoWORLD_BUILD_RESULT_OF_GUILD_BATTLE_PROLOG		0x8601
//#define dSERVERStoWORLD_END_WORD_QUIZ					0x8129
#define dSERVERStoWORLD_SYNC_CHANGEDNAME				0x8133
#define dSERVERStoWORLD_MODIFY_LOTTO_EVENT_DATA			0x8134
#define dSERVERStoWORLD_MODIFY_EVENT_TIME				0x8135



#define	dWORLDtoSERVERS_SENDINFOTOADMIN		0x8802//	Adminø°∞‘ ∫∏≥ª¡Ÿ ∞¢¡æ ∞‘¿” º≠πÅE¡§∫∏ 
#define	dWORLDtoSERVERS_ALL_MSG				0x8803//	WORLD∞° ∞¢ ∞‘¿” º≠πˆø°∞‘ ¿ÅEﬁ 
#define	dWORLDtoSERVERS_DELETE_AVATAR		0x8804//	ªı∑ŒøÅEƒ≥∏Ø≈Õ∞° ª˝º∫µ«∏ÅE¿Ã ƒ≥∏Ø≈Õ¿« save∆ƒ¿œ¿ª ªË¡¶Ω√≈≤¥Ÿ.
#define	dWORLDtoSERVERS_ISCONNECT_USER		0x8805//	¡¢º”¡ﬂ¿Œ ƒ≥∏Ø≈Õ ¿Ã∏ß∞ÅE∞∞¿Ã µπ∑¡¡ÿ¥Ÿ. ∏  ¿Ã∏ß¿Ã∂ÅE π¯»£∂ÅE
#define	dWORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT		0x880b//	OX ƒ˚¡ÅEøπæÅE
#define	dWORLDtoSERVERS_PRODUCE_OX_QUIZ		0x880c//	OX ƒ˚¡ÅE¡¶√ÅE
#define	dWORLDtoSERVERS_END_QUIZ_EVENT			0x880d//	OX ƒ˚¡ÅE¡æ∑ÅE
#define	dWORLDtoSERVERS_CANCEL_QUIZ_EVENT		0x880e//	OX ƒ˚¡ÅE√ÅE“
#define	dWORLDtoSERVERS_OX_QUIZ_LOSER_REVIVE_BATTLE	0x880f//	OX ƒ˚¡ÅE∆–¿⁄ ∫Œ»∞¿ÅE
#define dWORLDtoSERVERS_CHAGEGUILDBATTLE	0x8810//	admin≈¯ø°º≠ ±ÊµÂ¿ÅEøπæ‡ªÛ»≤¿ª πŸ≤€¥Ÿ.
#define dWORLDtoSERVERS_GET_FIXCHECK		0x8811
#define dWORLDtoSERVERS_GET_GUILDLIST		0x8812
#define dWORLDtoSERVERS_GET_GUILDINFO		0x8813
#define dWORLDtoSERVERS_BANISH_USER			0x8814	//	WORLDtoSERVERS_GET_GUILDMEMBERLIST
#define dWORLDtoSERVERS_BANISH_USER_RESULT	0x8815
#define dWORLDtoSERVERS_RENEW_ITEM_EXPIRE_TIME_LIST	0x8816
#define dWORLDtoSERVERS_ITEM_PACK_INFO		0x8817
#define dWORLDtoSERVERS_ASK_ALL_ITEM_PACK_INFO	0x8818
#define dWORLDtoSERVERS_GR_INFO					0x8819
#define dWORLDtoSERVERS_GUILD_NAME_LIST			0x881a
#define dWORLDtoSERVERS_GUILD_NAME				0x881b
#define dWORLDtoSERVERS_CHANGE_GUILD_BATTLE_REPORT_RESULT	0x881c
#define dWORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE_RESULT		0x881d
#define dWORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE				0x881e
#define dWORLDtoSERVERS_RESET_DROP_PACK_ITEM_COUNT			0x881f
#define dWORLDtoSERVERS_CARROT_SHOP_INFO					0x8820
#define dWORLDtoSERVERS_PRODUCE_WORD_QUIZ					0x8821
#define dWORLDtoSERVERS_CHANGE_WORD_QUIZ_ROUND				0x8822
#define dWORLDtoSERVERS_ITEM_PACK_LIST_SERIAL				0x8823
#define	dWORLDtoSERVERS_UPDATE_WORD_COMPARE					0x8360		//ADMINø°º≠ ∫∏≥Ω¡§∫∏∏¶ ø˘µÂ∏¶ ∞≈√ƒº≠ ∞‘¿” º≠πˆ∑Œ ∫∏≥Ω¥Ÿ.
#define dWORLDtoSERVERS_LOTTO_EVENT_DATA					0x8361
#define dWORLDtoSERVERS_EVENT_TIME_DATA						0x8362

//	About Community
#define	dSERVERStoWORLD_USER_SAY					0x8201
//#define	dSERVERStoWORLD_USER_GUILDCHAT				0x8202
#define	dSERVERStoWORLD_PARTY_CHAT					0x8203
#define	dSERVERStoWORLD_USER_FIND					0x8204
#define	dSERVERStoWORLD_EVENT_MESSAGE				0x8205
#define	dSERVERStoWORLD_IS_DENY_COMMUNITY_TARGET	0x8206
#define	dSERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST	0x8207
#define	dSERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST_RESULT	0x8208
#define	dSERVERStoWORLD_INCREASE_DB_EVENT_VALUE		0x8209
#define	dSERVERStoWORLD_ASK_DUEL_RECORD				0x820a
#define	dSERVERStoWORLD_UPDATE_DUEL_RECORD			0x820b
#define	dSERVERStoWORLD_UPDATE_DUEL_TEAM_RECORD		0x820c


#define	dWORLDtoSERVERS_USER_SAY					0x8901
#define	dWORLDtoSERVERS_USER_SAY_RESULT				0x8902
//#define	dWORLDtoSERVERS_USER_GUILDCHAT				0x8903
#define	dWORLDtoSERVERS_PARTY_CHAT					0x8904
#define	dWORLDtoSERVERS_USER_FIND					0x8905
#define	dWORLDtoSERVERS_CHAT_ADMIN					0x8906
#define	dWORLDtoSERVERS_EVENT_MESSAGE				0x8907
#define	dWORLDtoSERVERS_IS_DENY_COMMUNITY_TARGET	0x8908
#define	dWORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST	0x8909
#define	dWORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST_RESULT	0x890a
#define	dWORLDtoSERVERS_RESULT_INCREASE_DB_EVENT_VALUE	0x890b
#define	dWORLDtoSERVERS_DUEL_RANKER_INFO			0x890c
#define	dWORLDtoSERVERS_DUEL_RECORD					0x890d
#define	dWORLDtoSERVERS_DUEL_TEAM					0x890e

//	About Guild
#define	dSERVERStoWORLD_USER_CREATEGUILD					0x8301
#define	dSERVERStoWORLD_USER_JOINGUILD						0x8302
#define	dSERVERStoWORLD_USER_EXITGUILD						0x8303
#define	dSERVERStoWORLD_USER_DELETEGUILD					0x8304
#define	dSERVERStoWORLD_USER_CHANGERANK						0x8306
#define	dSERVERStoWORLD_UPDATE_SERVER_EXP					0x8315
#define	dSERVERStoWORLD_UPDATE_NOTICE						0x8316
#define	dSERVERStoWORLD_UPDATE_EXPRATE						0x8317
#define	dSERVERStoWORLD_INCREASE_GUILD_SKILL_LEVEL			0x8318
#define	dSERVERStoWORLD_CHANGE_GUILD_SKILL_STATUS			0x8319
#define	dSERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL			0x8322
#define	dSERVERStoWORLD_CHECK_GUILD_CHECKSUM				0x8323
#define	dSERVERStoWORLD_BOOKING_GUILD_BATTLE				0x831a
#define	dSERVERStoWORLD_CANCEL_GUILD_BATTLE					0x831b
#define	dSERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE			0x831c
#define	dSERVERStoWORLD_ASK_GUILD_INFO						0x831d
#define	dSERVERStoWORLD_ASK_GUILD_BATTLE_INFO				0x831e
#define	dSERVERStoWORLD_GUILD_BATTLE_RESULT					0x831f
#define	dSERVERStoWORLD_SELECT_GUILD_MARK					0x8320
#define	dSERVERStoWORLD_MAKE_GUILD_LEVEL					0x8321
#define	dSERVERStoWORLD_ADD_GUILD_GOLD						0x8325
#define	dSERVERStoWORLD_SET_GUILD_HALL_LEVEL				0x8327
#define	dSERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_GOLD		0x8328	
#define	dSERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_ITEM		0x8329
#define	dSERVERStoWORLD_CHANGE_GI_STORAGE_SIZE				0x832a

#define	dSERVERStoWORLD_STORE_GUILD_INVENTORY_ITEM			0x832b
#define	dSERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_ITEM		0x832c
#define	dSERVERStoWORLD_STORE_GUILD_INVENTORY_GOLD			0x832d
#define	dSERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_GOLD		0x832e
#define	dSERVERStoWORLD_CHANGE_GUILD_INVENTORY_ITEM_PLACE	0x832f
#define	dSERVERStoWORLD_ADD_RELATED_PLACE_OF_GUILD			0x8330
#define	dSERVERStoWORLD_RECORD_GUILD_HONOR_POINT			0x8331

#define	dWORLDtoSERVERS_USER_CREATEGUILD					0x8332
#define	dWORLDtoSERVERS_USER_JOINGUILD						0x8333
#define	dWORLDtoSERVERS_USER_EXITGUILD						0x8334
#define	dWORLDtoSERVERS_USER_DELETEGUILD					0x8335
#define	dWORLDtoSERVERS_DELETEGUILD							0x8336
#define	dWORLDtoSERVERS_USER_CHANGERANK						0x8337
#define	dWORLDtoSERVERS_USER_CHANGERANK_RESULT				0x8338
#define	dWORLDtoSERVERS_USER_LISTGUILD						0x8339
#define	dWORLDtoSERVERS_USER_GUILDINFO						0x833a
#define	dWORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL			0x833b
#define	dWORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE	0x833c
#define	dWORLDtoSERVERS_BOOKING_GUILD_BATTLE_RESULT			0x833d
#define	dWORLDtoSERVERS_VALID_GUILD							0x833e
#define	dWORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO			0x833f
#define	dWORLDtoSERVERS_UPDATE_GUILD_INFO					0x8340
#define	dWORLDtoSERVERS_GUILD_BATTLE_SCHEDULE				0x8341
#define	dWORLDtoSERVERS_GUILD_BATTLE_RESULT					0x8342
#define	dWORLDtoSERVERS_GUILD_BATTLE_TODAY_INFO				0x8343
#define	dWORLDtoSERVERS_SELECT_GUILD_MARK					0x8344
#define	dWORLDtoSERVERS_UPDATE_GUILD_MEMBER_LEVEL			0x8345
#define	dWORLDtoSERVERS_RESET_WEEK_FIGHT_GUILD_INFO			0x8346
#define	dWORLDtoSERVERS_UPDATE_GUILD_GOLD					0x8347
#define	dWORLDtoSERVERS_SET_GUILD_HALL_LEVEL				0x8348

#define	dWORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD		0x8349
#define	dWORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_ITEM		0x834a
#define	dWORLDtoSERVERS_CHANGE_GI_STORAGE_SIZE				0x834b

#define	dWORLDtoSERVERS_STORE_GUILD_INVENTORY_ITEM			0x834c
#define	dWORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_ITEM		0x834d
#define	dWORLDtoSERVERS_STORE_GUILD_INVENTORY_GOLD			0x834e
#define	dWORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_GOLD		0x834f
#define	dWORLDtoSERVERS_CHANGE_GUILD_INVENTORY_ITEM_PLACE	0x8350
#define	dWORLDtoSERVERS_ADD_RELATED_PLACE_OF_GUILD			0x8351
#define	dWORLDtoSERVERS_RECORD_GUILD_HONOR_POINT			0x8352

#define	dWORLDtoSERVERS_GUILD_CREATE						0x8353
#define	dWORLDtoSERVERS_GUILD_JOIN							0x8354
#define	dWORLDtoSERVERS_GUILD_EXIT							0x8355
#define	dWORLDtoSERVERS_GUILD_UPDATE_EXP					0x8356
#define	dWORLDtoSERVERS_GUILD_NOTICE						0x8357
#define	dWORLDtoSERVERS_GUILD_EXPRATE						0x8358

#define	dWORLDtoSERVERS_UPDATE_GUILD_HALL_INFO				0x8359

#define	dSERVERStoWORLD_SET_GUILD_HALL_ROEN_GOOD_WILL		0x8463
#define	dWORLDtoSERVERS_SET_GUILD_HALL_ROEN_GOOD_WILL		0x8464
#define	dWORLDtoSERVERS_UPDATE_WORLD_VALUE_INFO				0x8462
#define	dWORLDtoSERVERS_UPDATE_WORLD_VALUE_INFO_ALL			0x8461
#define	dSERVERStoWORLD_UPDATE_WORLD_VALUE_INFO				0x8460
#define	dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER			0x8465
#define	dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_SERVER			0x8467
#define	dWORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT			0x8466
#define	dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT			0x8468

#define	dSERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER			0x8469
#define	dWORLDtoSERVERS_TRANS_GUILD_TO_GVG_SERVER			0x846a
#define	dWORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT			0x846b
#define	dSERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT			0x846c
#define	dWORLDtoSERVERS_CHANGE_GVG_SERVER_GUILD_MASTER		0x846d
#define	dSERVERStoWORLD_CHANGE_GVG_SERVER_GUILD_MASTER		0x846e

#define	dG2W_ASK_GVG_AVATAR_ORG_DATA						0x846f	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª
#define	dW2L_TOSS_ASK_GVG_AVATAR_ORG_DATA					0x8470	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂ ≈‰Ω∫
#define	dL2W_TOSS_ASK_GVG_AVATAR_ORG_DATA					0x8471	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂ ≈‰Ω∫
#define	dW2D_ASK_GVG_AVATAR_ORG_DATA						0x8472	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
#define	dD2W_GVG_AVATAR_ORG_DATA							0x8473	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
#define	dW2L_GVG_AVATAR_ORG_DATA							0x8474	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
#define	dL2W_GVG_AVATAR_ORG_DATA							0x8475	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
#define	dW2G_GVG_AVATAR_ORG_DATA							0x8476	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂

#define	dW2G_ASK_GVG_AVATAR_ORG_DATA_RESULT					0x8477
#define	dL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT					0x8478
#define	dW2L_ASK_GVG_AVATAR_ORG_DATA_RESULT					0x8479
#define	dD2W_ASK_GVG_AVATAR_ORG_DATA_RESULT					0x847a

//	About Party
#define	dSERVERStoWORLD_CREATE_PARTY				0x8401
#define	dSERVERStoWORLD_JOIN_PARTY					0x8402
#define	dSERVERStoWORLD_PARTY_WORK					0x8403
#define	dSERVERStoWORLD_RENAME_PARTYNAME			0x8404	//	∆ƒ∆º ¿Ã∏ß πŸ≤Ÿ±ÅE
#define	dSERVERStoWORLD_ASK_PARTY_LIST				0x8405
#define	dSERVERStoWORLD_ASK_PARTYINFO				0x8406
#define	dSERVERStoWORLD_CHANGE_PARTY_STATUS			0x8407
#define	dSERVERStoWORLD_CHANGE_PARTY_MEMBER_INFO	0x8408
#define	dSERVERStoWORLD_CHECK_PARTY_INFO_BY_ACTOR	0x8409
#define	dSERVERStoWORLD_MOVE_FIELD_PARTY_MEMBER		0x840a
#define	dSERVERStoWORLD_DISCONNECT_PARTY_MEMBER		0x840b
#define	dSERVERStoWORLD_PARTY_ENTER_IF				0x840c
#define	dSERVERStoWORLD_JOIN_PARTY_APPLICATION			0x840d
#define	dSERVERStoWORLD_JOIN_PARTY_APPLICATION_ANSWER	0x840e
#define	dSERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY			0x840f
#define	dSERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY_ANSWER			0x84a1

#define	dWORLDtoSERVERS_JOIN_PARTY					0x8b01
#define	dWORLDtoSERVERS_PARTY_WORK					0x8b02
#define	dWORLDtoSERVERS_RENAME_PARTYNAME			0x8b03	//	∆ƒ∆º ¿Ã∏ß πŸ≤Ÿ±ÅE- « ø‰«— ∞‘¿” º≠πˆø°∞‘∏∏.
#define	dWORLDtoSERVERS_PARTY_LIST					0x8b04
#define	dWORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO	0x8b05
#define	dWORLDtoSERVERS_MOVE_FIELD_PARTY_MEMBER		0x8b06
#define	dWORLDtoSERVERS_UPDATE_PARTYINFO			0x8b07
#define	dWORLDtoSERVERS_VALID_PARTY					0x8b08
#define	dWORLDtoSERVERS_FAILED_TO_CREATE_PARTY		0x8b09
#define	dWORLDtoSERVERS_DISSOLUTION_PARTY			0x8b0a
#define	dWORLDtoSERVERS_DISCONNECT_PARTY_MEMBER		0x8b0b
#define	dWORLDtoSERVERS_CHANGE_PARTY_STATUS			0x8407
#define	dWORLDtoSERVERS_JOIN_PARTY_APPLICATION			0x8408
#define	dWORLDtoSERVERS_JOIN_PARTY_APPLICATION_ANSWER	0x8409
#define	dWORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY			0x840a
#define	dWORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY_ANSWER		0x840b

//	∆ƒ∆º ø¨«’ ∞ÅE√.
#define	dSERVERStoWORLD_CREATE_UNION_PARTY			0x84a0
#define	dWORLDtoSERVERS_UPDATE_UNION_PARTYINFO		0x84a1
#define	dWORLDtoSERVERS_DISSOLUTION_UNION_PARTY		0x84a2
#define	dWORLDtoSERVERS_CREATE_UNION_PARTY_RESULT	0x84a3

//	æ∆¿Ã≈€ √≥∏Æ
#define	dSERVERStoWORLD_CREATE_ITEM					0x8501	//	« µÂø° ∂≥±º æ∆¿Ã≈€ ª˝º∫
#define	dSERVERStoWORLD_CREATE_SELL_ITEM			0x8502	//	∆«∏≈«“ æ∆¿Ã≈€ ª˝º∫
#define	dSERVERStoWORLD_ADD_ITEM					0x8503	//	
#define	dSERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL		0x8504	//	¿Ø¥œ≈© Ω√∏ÆæÅEø‰±∏
#define	dSERVERStoWORLD_INCREASE_PACK_ITEM_COUNT	0x8505


#define	dWORLDtoSERVERS_CREATE_ITEM_RESULT		0x8c01	//	« µÂø° ∂≥±º æ∆¿Ã≈€ ª˝º∫ ∞·∞ÅE
#define	dWORLDtoSERVERS_CREATE_SELL_ITEM_RESULT	0x8c02	//	∆«∏≈«“ æ∆¿Ã≈€ ª˝º∫ ∞·∞ÅE
#define	dWORLDtoSERVERS_ADD_ITEM_RESULT			0x8c03	//	∆«∏≈«“ æ∆¿Ã≈€ ª˝º∫ ∞·∞ÅE
#define	dWORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL	0x8c04	//	¿Ø¥œ≈© Ω√∏ÆæÅEø‰±∏
#define	dWORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT	0x8c05	

//	º≠πˆ∞£ √≥∏Æ 
#define	dWORLDtoSERVERS_CHECKTIME				0x8d01	//	º≠πÅE¡°∞À Ω√∞£
#define dWORLDtoSERVERS_CHEKCPACKET				0x8d02	//	≈◊Ω∫∆ÆøÅE.
#define dWORLDtoSERVERS_WORLDNAME				0x8d03	//	∞¢ º≠πˆ∑Œ ø˘µÅE¿Ã∏ß¿ª ∫∏≥Ω¥Ÿ.

//////////////////////////////////////////////////////////////////////////
//	≈ÅEË∏¶ ¿ß«— ∑Œ±◊ ±‚∑œ
typedef  enum
{
	dSERVERStoWORLD_LOG_INOUT				=	0x8f11,
	dSERVERStoWORLD_LOG_QUEST,
};
//	≈ÅEË∏¶ ¿ß«— ∑Œ±◊ ±‚∑œ
//////////////////////////////////////////////////////////////////////////

//	æ˚—ÂπŒ≈ÅE¿Ã∫•∆Æ ¥‹æ˚‹« ∫Ò∆Æ ∏∂Ω∫≈©.

#define dWORD_EVENT_EFFECT_ATK200				0x00000001
#define dWORD_EVENT_EFFECT_ATK300				0x00000002
#define dWORD_EVENT_EFFECT_DEF200				0x00000004
#define dWORD_EVENT_EFFECT_DEF300				0x00000008
#define dWORD_EVENT_EFFECT_ITEM200				0x00000010
#define dWORD_EVENT_EFFECT_EXP200				0x00000020
#define dWORD_EVENT_EFFECT_DOTGE200				0x00000040
#define dWORD_EVENT_EFFECT_HP200				0x00000080
#define dWORD_EVENT_EFFECT_CP200				0x00000100
#define dWORD_EVENT_EFFECT_MOVESPD200			0x00000200
#define dWORD_EVENT_EFFECT_CPMAX				0x00000400

struct WORLDtoSERVERS_CHEKCPACKET
{
	cMSG_BASE_TYPE_FORWORLD		base;
	char						msg[4196];
};

struct WORLDtoSERVERS_WORLDNAME
{
	cMSG_BASE_TYPE_FORWORLD		base;
	char						strWorldName[64];
	WORD						wServerType;
};

/////////////////////////////////////////////////////////
//	PACKET s : Server -> World Server
/////////////////////////////////////////////////////////

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠	PACKET In / Out

#define	dMAPNAME_LENGTH			32

//
//	Field Type
enum
{
	eFT_NORMAL_FIELD,
	eFT_PREMIUM_ZONE,
	eFT_GUILD_BATTLE_FIELD,
};

class	cGameServerType
{
public:
	WORD		wIndex;							//∏ ¿« ∞˙‹Ø ¿Œµ¶Ω∫
	WORD		wType;							//∏ ¿« ¡æ∑ÅE	
	char		strMapName[dMAPNAME_LENGTH];	//¿⁄Ω≈¿Ã ∞ÅEÆ«œ¥¬ ∏ ¿Ã∏ß¿ª µ˚”œ«—¥Ÿ.
};

struct	SERVERStoWORLD_LOGIN
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strServerName[64];
	WORD			wNICount;
	char			strIp[2][dIP_SIZE];
	WORD			wMapCount;
	WORD			wServerIndex;
	WORD			isReconnect;
	cGameServerType	cGSInfo[dGAME_MAX_FIELD_COUNT];
};

struct	SERVERStoWORLD_CONFIRM_ENTER_FIELD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wField;
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_PLAYER_COUNT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wHour,wCount;
	WORD	awPlayerCount[1000*2];
};

struct	SERVERStoWORLD_LOGOUT
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

struct	SERVERStoWORLD_JOIN_COMPLETE
{
	cMSG_BASE_TYPE_FORWORLD		base;
	char		strId[dID_LENGTH];	//	¿ÃªÁ∂˜¿« µ•¿Ã≈∏∏¶ ¡§ªÛ¿˚¿∏∑Œ ªÁøÅEœººøÅE
};

struct SERVERStoWORLD_GETAVAS
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD		loginSerial;			//	login º≠πˆø°º≠¿« Ω√∏ÆæÅE
	DWORD		worldSerial;			//	world º≠πˆ¿« ¿Ø¿ÅEΩ√∏ÆæÅE	
	char		strId[dID_LENGTH];
};

struct SERVERStoWORLD_CREATEAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD		loginSerial;
	DWORD		worldSerial;			//	world º≠πˆ¿« ¿Ø¿ÅEΩ√∏ÆæÅE	
	char		strID[dID_LENGTH];
	char		strName[dNAME_LENGTH];
	char		strIP[dIP_SIZE];
	WORD		wJob;	//	¡˜æÅE};
};

struct SERVERStoWORLD_DELETEAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD		loginSerial;
	char		strID[dID_LENGTH];
	char		strNAME[dNAME_LENGTH];
	char		strIP[dIP_SIZE];
};

struct SERVERStoWORLD_LOADAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD		loginSerial;
	char		strID[dID_LENGTH];
	char		strNAME[dNAME_LENGTH];
	char		strIp[dIP_SIZE];
	DWORD		dwCode;
};

struct SERVERStoWORLD_SAVEAVA
{
	cMSG_BASE_TYPE_FORWORLD			base;

	CPlayerSaveDataForServerDefine	aInfo;
};

struct	SERVERStoWORLD_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wSocketIndex;		//	0∞ÅE1
	WORD						wCount;				//	index∑Œ ∏Ùæ∆≥÷¿ª ¿Ø¿˙ºÅE
};

struct	SERVERStoWORLD_USER_LOGIN			//	¿Ã ¿Ø¿˙∞° ¿⁄Ω≈ø°∞‘ µÈæ˚€‘¥Ÿ.
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			type;				//	0 - ¿œπ›¿˚¿Œ ∑Œ±◊¿Œ , 1 - ∏ ¿« ¿Ãµø 
	DWORD		serial;
	char		strID[dID_LENGTH];
	DWORD		dwCode;
	char		strName[dNAME_LENGTH];
	char		strIP[dIP_SIZE];
	char		strMacAddress[64];
	char		strClientCode[12];
};

struct SERVERStoWORLD_GET_AVADATA
{
	cMSG_BASE_TYPE_FORWORLD		base;
	WORD			type;					//	0 - ¿œπ›¿˚¿Œ µ•¿Ã≈∏∏∏ ø‰√ª , 1 - ∑Œ±◊¿Œ¿ª ¿ß«— ø‰√ª
	DWORD			serial;
	DWORD			dwIndex;
	char			strId[dID_LENGTH];
	WORD			badtime;

#ifdef	_OGP_SERVICE
	char			strToken[dTOKEN_LENGTH];
#endif
};

struct SERVERStoWORLD_COPY_DATA				//	≈◊Ω∫∆Æ º≠πˆø° ªÁøÅE“ ƒ≥∏Ø≈Õ∏¶ ∫πªÁ«—¥Ÿ. πŸ∑Œ DBCø° ≥÷¿ª∞Õ!!
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strId[dID_LENGTH];
	cAVATAR_INFO	info;
	
	char			strOrgName[dNAME_LENGTH];
	WORD			wOrgWorld;
	WORD			wType;
};

struct SERVERStoWORLD_ACCOUNT_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iServerIndex,iPlayerCount;
	WORD		wMaxLevel;
	WORD		awPlayerCountPerLevel[2000];
};

struct WORLDtoSERVERS_USER_LOGIN
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD		serial;						//	∞‘¿”º≠πˆ∞° ∫∏≥Ω ¿Ø¿˙¿« Ω√∏ÆæÅE- ±◊¥ÅEŒ µπ∑¡¡÷∏ÅEµ»¥Ÿ.
	int			iIndex;						//	µ˚”œµ» Ω√∏ÆæÛ¿ª µπ∑¡¡ÿ¥Ÿ. ∏∏æÅE-1¿Ã∏ÅEµ˚”œ Ω«∆–∑Œ ¿Ø¿ÅE¿⁄∏•¥Ÿ.
	WORD		wFieldidx;
	WORD		bf10IFSerial	:	10;
	WORD		bf4IFFloor		:	4;
	WORD		wPartyIndex;
	WORD		badtime;
	WORD		oper;						//	ø˚€µ¿⁄¿Œ∞°??
	WORD		isOperInAccount;			//	∞Ë¡§¿Ã ø˚€µ¿⁄∑Œ º≥¡§µ«æÅE¿÷≥™?
	WORD		isNetCafeUser;
	WORD		bf1IsIncorrectConnectByGuildBattle : 1;
	//	cP_PLAYER_DATA	aInfo;
};

struct WORLDtoSERVERS_USER_LOGOUT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wResult;
	int			iSerialInServer;
	char		strName[dNAME_LENGTH];
};

struct WORLDtoSERVERS_DISCONNECT_SERVER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wServerIndex;
	WORD						wCause;
	char						strWorldName[40];
};

struct WORLDtoSERVERS_DIS
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	char			strId[dID_LENGTH];
	char			strCause[128];		//	¡◊¿Ã¥¬ ¿Ã¿Ø -_x
};

struct WORLDtoSERVERS_GET_AVADATA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD							type;					//	0 - ¿œπ›¿˚¿Œ µ•¿Ã≈∏∏∏ ø‰√ª , 1 - ∑Œ±◊¿Œ¿ª ¿ß«— ø‰√ª
	DWORD							serial;
	CPlayerSaveDataForServerDefine	aInfo;
	CUpkeepPlayerData				upkeepPlayerData;		//	« µÂ∏¶ ¿Ãµø«ÿµµ ¿Ø¡ˆ«ÿæﬂ «œ¥¬ µ•¿Ã≈Õ
	cActorStatusUpkeep				upkeepStatus;
};

typedef union 
{
	struct{
		DWORD	bf12MTWeekday	:	12;
		DWORD	bf4MTStartHour	:	4;
		DWORD	bf6MTStartMin	:	6;
		DWORD	bf4MTEndHour	:	4;
		DWORD	bf6MTEndMin		:	6;
	};
	DWORD dwMtValue;
}stMainterenceTime;

struct WORLDtoSERVERS_LOGIN
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wBoostGoldByTreasureMapEventCount,wBoostExpByTreasureMapEventCount;
	WORD	wTrialGameBeginYear,wTrialGameBeginMonth,wTrialGameBeginDay,wTrialGameBeginHour,wTrialGameBeginMinute;
	WORD	wTrialGameEndYear,wTrialGameEndMonth,wTrialGameEndDay,wTrialGameEndHour,wTrialGameEndMinute;
	WORD	wIsSendTrialGameResultMail,wTrialGameTeamSize;
	
	WORD	wOfflineTournamentBeginYear,wOfflineTournamentBeginMonth,wOfflineTournamentBeginDay,wOfflineTournamentBeginHour,wOfflineTournamentBeginMinute;
	WORD	wOfflineTournamentEndYear,wOfflineTournamentEndMonth,wOfflineTournamentEndDay,wOfflineTournamentEndHour,wOfflineTournamentEndMinute;

	int		iHeavenRedStoneCount,iHellRedStoneCount,iRedDevilRedStoneCount;
	int		iBoostExpByLeadersSoundofBell,iBoostItemDropChanceByLeadersSoundofBell,iRemainSoundofBellTime;
	WORD	wDisplayNameWithSoundOfLeadersBell;
	char	strMessageOfSoundOfLeadersBell[128];
	char	strOwnerOfSoundOfLeadersBell[dNAME_LENGTH];

	int		iServerType;
	char	strServerName[dWORLD_NAME_LENGTH];
	char	strTrialGameName[64];
	char	strOfflineTournamentComment[256];
	stMainterenceTime	stMtTime;
	SYSTEMTIME			systemTime;
	int		aiEventValue[c_iDBControlledEvnetCount];
	WORD	wSeasonVariable;
	WORD	wWorldServerIndex;
};

struct WORLDtoSERVERS_DUEL_MATCH_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wTrialGameBeginYear,wTrialGameBeginMonth,wTrialGameBeginDay,wTrialGameBeginHour,wTrialGameBeginMinute;
	WORD	wTrialGameEndYear,wTrialGameEndMonth,wTrialGameEndDay,wTrialGameEndHour,wTrialGameEndMinute;
	WORD	wIsSendTrialGameResultMail,wTrialGameTeamSize;;
	
	WORD	wOfflineTournamentBeginYear,wOfflineTournamentBeginMonth,wOfflineTournamentBeginDay,wOfflineTournamentBeginHour,wOfflineTournamentBeginMinute;
	WORD	wOfflineTournamentEndYear,wOfflineTournamentEndMonth,wOfflineTournamentEndDay,wOfflineTournamentEndHour,wOfflineTournamentEndMinute;

	char	strTrialGameName[64];
	char	strOfflineTournamentComment[256];
};

struct WORLDtoSERVERS_CONFIRM_ENTER_FIELD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wField;
	char	strId[dID_LENGTH];
	char	strName[dNAME_LENGTH];
};

enum
{
//	ø˘µÂø°º≠ ∞‘¿”º≠πˆ∑Œ ¿ÅE€
	eSWWG_BEGIN		=	0x0,
	eSWWG_NOTICE_BEGIN_SIEGE_WARFARE,
	eSWWG_NOTICE_END_SIEGE_WARFARE,
	eSWWG_BEGIN_SIEGE_WARFARE,
	eSWWG_END_SIEGE_WARFARE,
	eSWWG_SIEGE_WARFARE_RESULT_SUCCESS_TO_DEFENSE	,
	eSWWG_SIEGE_WARFARE_RESULT_SUCCESS_TO_INVADE	,
	eSWWG_RENT_GUILD_PET				,
	eSWWG_RETRIEVAL_RENTED_GUILD_PET	,
	eSWWG_UPDATE_GUILD_PET_EXP			,
	eSWWG_CHANGE_GUILD_HALL_OPEN_STATUS	,
	eSWWG_RESET_GUILD_HONOR_POINT		,
	eSWWG_RESET_GUILD_PROLOG_POINT		,
	eSWWG_WARNING_FOR_LEVEL_DOWN_ABLE_GUILD_PET_AND_GUARDIAN	,
	eSWWG_CHANGE_GUILD_MARK_TWINKLE_STATUS	,
	eSWWG_CHANGE_TRY_GUILD_POINT_BATTLE_COUNT	,
	eSWWG_UPDATE_RED_STONE_COUNT,
	eSWWG_INCREASE_BONUS_GUILD_POINT,
	eSWWG_RECOVER_BONUS_GUILD_POINT_PENELTY	,
	eSWWG_RESET_WEEKLY_DUEL_RECORD,
	eSWWG_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL,
	eSWWG_CHANGE_SYSTEM_TIME,
	eSWWG_CONFIRM_USE_SOUND_OF_LEADERS_BELL,	//	¿Œµµ¿⁄¿« ¡æº“∏Æ ªÁøÅE∞°¥…
	eSWWG_CHANGE_TRY_GUILD_DUNGEN_COUNT	,
	eSWWG_SMS_ALARM_ERROR	,
	eSWWG_UPDATE_GUILD_BATTLE_SCORE,
	eSWWG_RESET_GUILD_SIEGEWARFARE_INFO,	//	±ÊµÅE∞¯º∫¿ÅE¡§∫∏ √ ±‚»≠
	eSWWG_RESET_1DAY_GUILD_INFO		,
	eSWWG_RESET_1WEEK_GUILD_INFO	,

//	∞‘¿” º≠πˆø°º≠ ø˘µÂ∑Œ ¿ÅE€
	eSWGW_BEGIN		=	0x1000,
	eSWGW_SIEGE_WARFARE_RESULT_SUCCESS_TO_DEFENSE	,
	eSWGW_SIEGE_WARFARE_RESULT_SUCCESS_TO_INVADE	,
	eSWGW_RENT_GUILD_PET				,
	eSWGW_RETRIEVAL_RENTED_GUILD_PET	,
	eSWGW_INCREASE_PET_EXP				,
	eSWGW_TOGGLE_GUILD_HALL_OPEN_STATUS	,
	eSWGW_RESET_GUILD_HONOR_POINT		,
	eSWGW_SET_GUILD_HONOR_POINT			,
	eSWGW_RESET_GUILD_PROLOG_POINT		,
	eSWGW_SET_GUILD_PROLOG_POINT		,
	eSWGW_END_GUILD_PROLOG				,
	eSWGW_BEGIN_TEST_SIEGE_WARFARE_OF_GUILD	,
	eSWGW_CHANGE_GUILD_HALL_LEVEL,
	eSWGW_INCREASE_TRY_GUILD_POINT_BATTLE_COUNT	,
	eSWGW_UPDATE_SECRET_DUNGEON_INFO,
	eSWGW_INCREASE_RED_STONE_COUNT,
	eSWGW_INCREASE_BONUS_GUILD_POINT,	//	∫∏≥ Ω∫ ±ÊµÅE∆˜¿Œ∆Æ ¡ı∞°
	eSWGW_RECOVER_BONUS_GUILD_POINT_PENELTY	,
	eSWGW_NAME_STONE_INDEX,
	eSWGW_ACTIVE_GUILD_MASTER_GRACE,
	eSWGW_INCREASE_TRY_GUILD_DUNGEON_COUNT	,
	eSWGW_CHANGE_SYSTEM_TIME	,
	eSWGW_CHANGE_GUILD_PET_EXP			,		// ±ÊµÂ∆ÅEπ∫ß∫Ø∞ÅE	
	eSWGW_RESET_GUILD_BATTLE_SCHEDULE	,		// ±ÊµÂ¿ÅEΩ∫ƒ…¡ÅE√ ±‚»≠ 

	eSWAW_BEGIN		=	0x2000,	//	æ˚—ÂπŒ≈ÅE>ø˘µÅE	
	eSWAW_CHANGE_GUILD_MARK_TWINKLE_STATUS,
	eSWAW_CHANGE_GUILD_HALL_LEVEL,
	eSWAW_CHANGE_GUILD_VILLAGE_INFO,
	eSWAW_CHANGE_GUILD_MASTER_GRACE,
	eSWAW_CHANGE_GUILD_INFO_ETC_BY_ADMINTOOL,
	eSWAW_RESET_SOUND_OF_LEDERS_BELL_COOLTIME	,	//	¿Œµµ¿⁄¿« ¡æº“∏Æ √ ±‚»≠
	eSWAW_ASK_SOUND_OF_LEDERS_BELL_EVENT_INFO,
	eSWAW_CHANGE_GUILD_POINT_BATTLE_COUNT	,
	eSWAW_ASK_LOTTO_EVENT_DATA,
	eSWAW_ASK_EVENT_TIME_DATA,
	eSWAW_UPDATE_CARROT_SHOP_INFO,
	eSWAW_CHANGE_TODAY_GUILD_BATTLE_COUNT,
	eSWAW_CHANGE_THIS_WEEK_GUILD_BATTLE_COUNT,

	eSWLW_BEGIN		=	0x3000,						//	∑Œ±◊¿Œ º≠πˆø°º≠ ø˘µÂ∑Œ
	eSWLW_SMS_ALARM_ERROR	,
	eSWLW_UPDATE_CARROT_SHOP_INFO	,

	eSWWA_BEGIN		=	0x4000,		//	ø˘µÅE>æ˚—ÂπŒ≈ÅE	
	eSWWA_CAN_NOT_FOUND_LOGIN_SERVER,

	eSWWL_BEGIN		=	0x5000,						//	ø˘µÅEº≠πˆø°º≠ ∑Œ±◊¿Œ º≠πˆ∑Œ
	eSWWL_UPDATE_CARROT_SHOP_INFO	,
	
	eSWDW_BEGIN		=	0x6000,		//	DBC->ø˘µÅE
};

struct D2W_SIMPLE_WORK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wWork;
	int		aiValue[6];
};

struct WORLDtoSERVERS_SIMPLE_WORK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wWork;
	int		aiValue[12];
};

struct SERVERStoWORLD_SIMPLE_WORK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wWork;
	int		aiValue[12];
};

class	cSimpleRenewInfo
{
public:
	WORD	bf14Index			:	14;
	WORD	bf1IsForOnlyYahoo	:	1;
	WORD	bf1IsForOnlyRedGem	:	1;

	WORD	wTerm;
};

struct WORLDtoSERVERS_RENEW_PREMIUM_ITEM	//	«¡∏ÆπÃæÅEæ∆¿Ã≈€ ∫∏ªÅE¡§∫∏
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD				wRenewCount,wIsNow;
	cSimpleRenewInfo	aRenewInfo[100];
};

struct WORLDtoSERVERS_ACCOUNT_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int		aiPlayerCount[10];
};

struct WORLDtoSERVERS_GETAVAS
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strID[dID_LENGTH];
	DWORD			loginSerial;			//	login º≠πˆø°º≠¿« Ω√∏ÆæÅE	
	DWORD			worldSerial;			//	world º≠πˆ¿« ¿Ø¿ÅEΩ√∏ÆæÅE	
	cAVATAR_INFO	aAvatarList[dMAX_AVATAR_COUNT];
};

struct WORLDtoSERVERS_CREATEAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strID[dID_LENGTH];
	DWORD			loginSerial;
	DWORD			worldSerial;			//	world º≠πˆ¿« ¿Ø¿ÅEΩ√∏ÆæÅE	
	short			wResult;		//	∞·∞ÅE	
	WORD			wIndex;			//	∏˚’¯¬∞ æ∆πŸ≈∏ø° √ﬂ∞°µ«æ˙¥¬¡ÅE..
	cAVATAR_INFO	aCharacter;		//	µ•¿Ã≈∏µµ ∫∏≥ª¡ÿ¥Ÿ. 
};

struct WORLDtoSERVERS_DELETEAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD			loginSerial;
	char			strID[dID_LENGTH];
	short			wResult;		//	∞·∞ÅE
};

enum
{
	eBeginLifeSign,
	eUpdateLifeSign,
	eIncorrectStatusSign,
	eEndLifeSign,
	eNotUseLifeSign,
};

struct WORLDtoSERVERS_LIFE_SIGN
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wSign;	
	int		iServerIndex;	//	-1¿Ã∏ÅEø˘µÅE	
	int		iGameServerCount;	//	øˆµÂø°º≠∏∏ æ≤¥¬ ∞™. -1¿Ã∏ÅEø˘µÂ∞° æ∆¥—∞˜ø°º≠ ∫∏≥Ω ∞™¿∏∑Œ ¿ŒΩƒ«œ∞ÅEπ´Ω√«ÿ πˆ∏∞¥Ÿ.

	char	strIP[32];
	char	strMsg[64];
};

struct SERVERStoWORLD_LIFE_SIGN
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wSign;	
	int		iServerIndex;
	char	strIP[32];
	char	strMsg[64];
};

enum
{
	eLOADAVA_SUCCESS	,
	eLOADAVA_FAIL
};

struct WORLDtoSERVERS_LOADAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strID[dID_LENGTH];
	char			strNAME[dNAME_LENGTH];
	DWORD			loginSerial;
	WORD			wResult;
};

struct	SERVERStoWORLD_USER_LOGOUT			//	¿Ã ¿Ø¿˙∞° ¿⁄Ω≈ø°∞‘º≠ ≥™∞¨¥Ÿ.
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		type;						//	 0 -logout , 1 - ¥Ÿ∏• º≠πˆ∑Œ ¿Ãµø
	int			iSerialInServer;
	char		strID[dID_LENGTH];
	char		strName[dNAME_LENGTH];
	char		strIP[dIP_SIZE];
};

//¶≠	PACKET In / Out
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨


//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠	PACKET Map & User

struct	SERVERStoWORLD_USER_MOVEMAP
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iSerialInServer;	//	«ÿ¥ÅE∞‘¿” º≠πˆø°º≠¿« Ω√∏ÆæÅE
	int			iWorldSerial;	//	ø˘µÂø°º≠ ∞ÅEÆ«œ¥¬ ¿Ø¿ÅEΩ√∏ÆæÅE- ∞‘¿”º≠πˆø° ∑Œ±◊¿Œ¿ª º∫∞¯«œ∏ÅEø˘µÂ∞° ∫∏≥ª¡ÿ¥Ÿ. 
	WORD		wFieldSerial;	//	« µÂ¿« ∞˙‹Ø Ω√∏ÆæÅE	
	WORD		wIsPremiumMember;
	char		strMapName[dMAPNAME_LENGTH];
	WORD		wGateIndex;
	WORD		wXPos,wYPos;
	WORD		wCheckResult;	//	∆–≈∂ ≈∏¿‘¿Ã dSERVERStoWORLD_CHECK_USER_MOVEMAP¿Œ ∞ÊøÅE¿Ã ∞™ø° ∞·∞˙∏¶ ¥„æ∆
								//	∆–≈∂ ≈∏¿‘¿ª dWORLDtoSERVERS_CHECK_USER_MOVEMAP∑Œ πŸ≤ÅE∞‘¿” º≠πˆø° ¿ÅE€«—¥Ÿ.
};

struct SERVERStoWORLD_USER_MOVEMAP_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	DWORD						dwSerial;				//	«ÿ¥ÅE≈¨∂Û¿Ãæ∆Æ¿« ø˘µÅEΩ√∏ÆæÅE
	CUpkeepPlayerData			upkeepPlayerData;		//	« µÂ∏¶ ¿Ãµø«ÿµµ ¿Ø¡ˆ«ÿæﬂ «œ¥¬ µ•¿Ã≈Õ
	cActorStatusUpkeep			upkeepStatus;
	WORD		wIsNetCafeUser;
};

struct WORLDtoSERVERS_SOUND_OF_LEADERS_BELL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int		iBoostExp,iBoostItemDropChance;
	int		iUpkeepTime;
	int		iActorSerial;

	WORD	wItemIndex,wDisplayName;
	WORD	wWork;	//	0 ¿Ã∫•∆Æ Ω√¿€,1 ¿ÅEº ¥ÅEÅE¿Ã∫•∆Æ ¡æ∑ÅE2 ∏Æ¥ÅE¥ÅEÅE¿Ã∫•∆Æ ¡æ∑ÅE3 ∏ﬁΩ√¡ÅE∫Ø∞ÅE
	WORD	wBeginYear,wBeginMonth,wBeginDay,wBeginHour,wBeginMinute;	//	Ω√¿€«— Ω√∞£(æ˚—ÂπŒ≈¯ø°º≠ ∏∏ æ∏. ∞‘¿” º≠πˆø°¥¬ æ≤∑π±ÅE∞™)
	WORD	wCoolTimeMinute;	//	≥≤¿∫ ƒ≈∏¿”(æ˚—ÂπŒ≈¯ø°º≠ ∏∏ æ∏. ∞‘¿” º≠πˆø°¥¬ æ≤∑π±ÅE∞™)

	char	strCaster[dNAME_LENGTH];
	char	strMessage[128];
};

struct SERVERStoWORLD_SOUND_OF_LEADERS_BELL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int		iBoostExp,iBoostItemDropChance,iUpkeepMinute;
	int		iActorSerial;
	WORD	wDisplayName,wItemIndex,wIsTest;
	char	strCaster[dNAME_LENGTH];
	char	strMessage[128];
};

struct SERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wOXQuizVillage;
	WORD						wWordQuizVillage;
};

struct SERVERStoWORLD_USER_RECALL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wFieldIndex;
	WORD						wX,wY;
	char						strName[dNAME_LENGTH];
};

struct WORLDtoSERVERS_USER_RECALL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wFieldIndex;
	WORD						wX,wY;
	char						strName[dNAME_LENGTH];
};

#define	dMOVEMAP_RESULT_SUCCESS			0x01	//	¿Ãµø∞°¥…
#define	dMOVEMAP_RESULT_NOTFIND			0x02	//	¡∏¿Á«œ¡ÅEæ ∞≈≥™.. «ˆ¿ÅE¡¢º” ∫“∞°¥… ¡ˆø™¿‘¥œ¥Ÿ.
#define	dMOVEMAP_RESULT_FULL			0x03	//	¥ı¿ÃªÅE¿‘¿Â¿Ã ∫“∞°¥…«’¥œ¥Ÿ.
#define	dMOVEMAP_RESULT_USEDNAME		0x04	//	∞∞¿∫ ¿Ã∏ß¿« æ∆¿Ãµ∞° ¿÷¥Ÿ -_-;
#define	dMOVEMAP_RESULT_UNKNOWN_ERROR	0x05	//	æÀºÅEæ¯¥¬ πÆ¡¶ πﬂª˝.. -_-a
#define	dMOVEMAP_RESULT_PREMIUM_ZONE	0x06	//	«¡∏ÆπÃæÅE¡∏¿Œµ• ¥ÁΩ≈¿∫ «¡∏ÆπÃæÅEªÁøÅE⁄∞° æ∆≥ƒ!!
#define	dMOVEMAP_RESULT_ONE_WAY_PORTAL	0x07	//	¿œπÊ≈ÅEÅE∆˜≈ª¿Ã¥Ÿ.
#define	dMOVEMAP_RESULT_TEST_PORTAL		0x08	//	≈◊Ω∫∆Æ ∆˜≈ª¿‘¥œ¥Ÿ.
#define	dMOVEMAP_RESULT_MOVE_TO_IF		0x09	//	¿ŒΩ∫≈œΩ∫ ¥¯¡Ø¿∏∑Œ ¿‘¿ÅE
#define	dMOVEMAP_RESULT_CAN_NOT_MOVE_FIELD_STATUS	0x0a	//	« µÅE¿Ãµø¿ª «“ ºÅEæ¯¥¬ ªÛ≈¬¿Ã¥Ÿ.

	//	±‚≈∏ « ø‰«“¡ˆµµ ∏∏£¥¬ ∞·∞ÅE
#define	dMOVEMAP_RESULT_NOTGUILD		0x11	//	∆Ø¡§±ÊµÂ∏∏¿Ã ¿‘¿ÅE∞°¥…«’¥œ¥Ÿ.


struct WORLDtoSERVERS_USER_MOVEMAP
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		Result;
	int			iSerialInServer;
	WORD		wUniqueFieldSerial;	//	« µÂ¿« ∞˙‹Ø Ω√∏ÆæÅE	
	char		strServerAddr[dIP_SIZE];
	char		strMapName[dMAPNAME_LENGTH];
	WORD		wGateIndex;
	WORD		wXPos,wYPos;
};

//¶≠	PACKET Map & User
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨



//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠	PACKET Community

struct	SERVERStoWORLD_USER_SAY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD		dwSenderSerial;
	WORD		wIsByOperator;
	char		strSendName[dNAME_LENGTH];
	char		strRecvName[dNAME_LENGTH];
	char		strMsg[dCHAT_LENGTH];
};

struct	SERVERStoWORLD_PARTY_CHAT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wPartySerial;

	char		strName[dNAME_LENGTH];
	char		strMsg[dCHAT_LENGTH];
};


//
//	Find User Work
enum
{
	eFUW_WARP,
	eFUW_RECALL,
	eFUW_FIND,
	eFUW_JAIL,
	eFUW_DRAWING_ROOM,
	eFUW_BANISH,
	eFUW_USE_COUPLE_RING,
	eFUW_CHECK_COUPLE_RING,
	eFUW_CHECK_COUPLE_RING_RESULT_OK,
	eFUW_CHECK_COUPLE_RING_RESULT_FAILED,
	eFUW_CHECK_COUPLE_RING_RESULT_TARGET_EXIST_CAN_NOT_TELEPORT_FIELD,
	
};

struct	SERVERStoWORLD_USER_FIND
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char		strName[dNAME_LENGTH];
	char		strFindName[dNAME_LENGTH];
	WORD		wWork;
	WORD		wField;
};

struct	WORLDtoSERVERS_USER_SAY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wIsByOperator;
	char	strSendName[dNAME_LENGTH];
	char	strRecvName[dNAME_LENGTH];
	char	strMsg[dCHAT_LENGTH];
};

struct SERVERStoWORLD_ASK_DUEL_RECORD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strId[dID_LENGTH];
};

struct WORLDtoSERVERS_DUEL_RECORD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strId[dID_LENGTH];
	cDuelRecord	record;
};

struct WORLDtoSERVERS_DUEL_TEAM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD			wTotalTeamCount;
	WORD			wTeamCount;
	cDuelTeamInfo	aTeams[100];
};

struct SERVERStoWORLD_UPDATE_DUEL_RECORD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char		strId[dID_LENGTH];
	cDuelRecord	record;
};

struct SERVERStoWORLD_UPDATE_DUEL_TEAM_RECORD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strTeamName[c_iDuelTeamNameLength];
	cDuelTeamRecord	record;
};


struct SERVERStoWORLD_INCREASE_DB_EVENT_VALUE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strUserID[dNAME_LENGTH];
	char	strUserName[dNAME_LENGTH];
	WORD	wEventIndex;
	BOOL	bReset;
	int		iMaxCount;
	BOOL	bIsAllowMulipleID;
};

struct WORLDtoSERVERS_DUEL_RANKER_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD				wIsDailyRanking;	//	0 : ¡÷∞£ ∑©≈∑,1 : ¿œ∞£ ∑©≈∑
	WORD				wWeeklyRankerCount,wTrialGameRankerCount;	//	¡÷∞£ ∑©ƒø ºÅEøπº±¿ÅE∑©ƒø ºÅE	
	cDuelRankingInfo	aList[200];	//	øπº±¿ÅE∑©≈∑ + ¡÷∞£ ∑©≈∑
};

struct WORLDtoSERVERS_RESULT_INCREASE_DB_EVENT_VALUE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wResult;
	char	strUserID[dNAME_LENGTH];
	char	strUserName[dNAME_LENGTH];
	WORD	wEventIndex;
	BOOL	bReset;
	int		iValue;
};

struct	SERVERStoWORLD_EVENT_MESSAGE
{
	cMSG_BASE_TYPE_FORWORLD		base;
	char						strMessage[dCHAT_LENGTH];
	WORD						wType;
};

struct	WORLDtoSERVERS_EVENT_MESSAGE
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	char						strMessage[dCHAT_LENGTH];
	WORD						wType;
};

struct	SERVERStoWORLD_IS_DENY_COMMUNITY_TARGET
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strSenderName[dNAME_LENGTH];
	char	strReceiverName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_IS_DENY_COMMUNITY_TARGET
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strSenderName[dNAME_LENGTH];
	char	strReceiverName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strClientName[dNAME_LENGTH];
	char	strTargetName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strClientName[dNAME_LENGTH];
	char	strTargetName[dNAME_LENGTH];
};

enum
{
	eRFITLR_SUCCESS,
	eRFITLR_NOT_EXIST_NAME_IN_TARGET_FRIEND_LIST,
	eRFITLR_NOT_CONNECTED,
};

struct	SERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strClientName[dNAME_LENGTH];
	char	strTargetName[dNAME_LENGTH];
	WORD	wResult;
};

struct	WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strClientName[dNAME_LENGTH];
	char	strTargetName[dNAME_LENGTH];
	WORD	wResult;
};



struct	SERVERStoWORLD_CALLING
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wCasterIFField	:	10;
	WORD	wCasterIFFloor	:	6;
	WORD	wCasterField,wParty;
	WORD	wCaterXPos,wCaterYPos;
	WORD	wLimitRange;
	WORD	wSkill;
	DWORD	dwNameHashCode;
};

struct	WORLDtoSERVERS_CALLING
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wCasterIFField	:	10;
	WORD	wCasterIFFloor	:	6;
	WORD	wCasterField,wParty;
	WORD	wCaterXPos,wCaterYPos;
	WORD	wLimitRange;
	WORD	wSkill;
	DWORD	dwNameHashCode;
};

enum
{
	eBPBE_TM_GOLD,	//	∫∏π∞ ¡ˆµµ ¿Ã∫•∆Æ∏¶ ¿Œ«— ∞ÒµÅE4πÅE	
	eBPBE_TM_EXP,	//	∫∏π∞ ¡ˆµµ ¿Ã∫•∆Æ∑Œ ¿Œ«— ∞Ê«Ëƒ° 2πÅE
};

struct SERVERStoWORLD_BOOST_POWER_BY_EVENT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wWork;
	char		strName[dNAME_LENGTH];
};

struct WORLDtoSERVERS_BOOST_POWER_BY_EVENT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wWork;
	WORD		wBoostGoldByTreasureMapEventCount;
	WORD		wBoostExpByTreasureMapEventCount;

	char		strName[dNAME_LENGTH];
};

struct WORLDtoSERVERS_CHANGE_DAY
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

//
//	ROFS - Reason Of Failed Say
enum
{
	eROFS_SUCCESS					,
	eROFS_CAN_NOT_FIND_TARGET		,	//	≈∏∞Ÿ¿ª ∏¯√£∞⁄¥¬µ¿Ø.. -o-;;
	eROFS_TARGET_DENIED_SAY_MESSAGE	,	//	ºˆΩ≈ ∞≈∫Œ ªÛ≈¬
	eROFS_TARGET_IS_OPERATOR		,	//	¥ÅEÛ¿Ã ø˚€µ¿⁄¥Ÿ
};

struct	WORLDtoSERVERS_USER_SAY_RESULT 
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD	dwSenderSerial;
	char	strDestName[dNAME_LENGTH];
	char	strMsg[dCHAT_LENGTH];
	WORD	wReason;
};

struct	WORLDtoSERVERS_PARTY_CHAT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;
	char	strSendID[dID_LENGTH];
	char	strMsg[dCHAT_LENGTH];
};

#define	dFIND_RESULT_NOTFIND		0x00	//	¡¢º”«œ¡ÅEæ æ“Ω¿¥œ¥Ÿ.
#define	dFIND_RESULT_SUCCESS		0x01
#define	dFIND_NOT_EXIST_COUPLE_RING	0x02	//	¡¢º”«œ¡ÅEæ æ“Ω¿¥œ¥Ÿ.

struct	WORLDtoSERVERS_USER_FIND
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	Result;
	WORD	wWork;
	WORD	wField;
	char	strName[dNAME_LENGTH];
	char	strFindName[dNAME_LENGTH];
	WORD	wMapUniqueIndex;
	WORD	wJob;

	WORD	bf1IsPremiumZone	:	1;
};

struct WORLDtoSERVERS_CHAT_ADMIN
{
	cMSG_BASE_TYPE_FORWORLD		base;
	char	strMsg[dCHAT_LENGTH];
};

//¶≠	PACKET Community
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠	PACKET Guild

struct	SERVERStoWORLD_USER_CREATEGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char		strName[dGUILD_NAME_LENGTH];
	char		strMasterId[dID_LENGTH];
	char		strMasterName[dNAME_LENGTH];
	char		strSubMasterName[dNAME_LENGTH];
	WORD		wMasterJob;
	WORD		wMasterLevel;
};

struct	SERVERStoWORLD_SET_GUILD_HALL_LEVEL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wLevel;
	char		strName[dGUILD_NAME_LENGTH];
};

struct	SERVERStoWORLD_SET_GUILD_HALL_ROEN_GOOD_WILL
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD		wGoodWill;
	char		strName[dGUILD_NAME_LENGTH];
};


struct	SERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER
{
	cMSG_BASE_TYPE_FORWORLD			base;

	cGuildInfoForTransToGVG			guildData;
	char							strMasterName[dNAME_LENGTH];	//	±ÊµÅE∏∂Ω∫≈Õ ¿Ã∏ß
	char							strMasterId[dID_LENGTH];
	int								iWorldIndex;
	WORD	wMasterLevel;
	WORD	wMasterJob;
};

struct	SERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER
{
	cMSG_BASE_TYPE_FORWORLD			base;

	CPlayerSaveDataForServerDefine	avatarData;
	int								iWorldIndex;
};

struct	WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_SERVER
{
	cMSG_BASE_TYPE_FORWORLD			base;

	CPlayerSaveDataForServerDefine	avatarData;
	int								iWorldIndex;
};

struct	WORLDtoSERVERS_TRANS_GUILD_TO_GVG_SERVER
{
	cMSG_BASE_TYPE_FORWORLD			base;

	cGuildInfoForTransToGVG			guildData;
	char							strMasterName[dNAME_LENGTH];	//	±ÊµÅE∏∂Ω∫≈Õ ¿Ã∏ß
	char							strMasterId[dID_LENGTH];
	int								iWorldIndex;
	WORD	wMasterLevel;
	WORD	wMasterJob;
};

struct SERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wResult;
	char	strName[dNAME_LENGTH];
	char	strNewName[dNAME_LENGTH];
};

struct SERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wResult;
	char	strMasterName[dNAME_LENGTH];
	char	strNewMasterName[dNAME_LENGTH];
	char	strNewGuildName[dGUILD_NAME_LENGTH];
	char	strGuildName[dGUILD_NAME_LENGTH];
};

struct WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wResult;
	WORD	wWorldServerIndex;
	char	strName[dNAME_LENGTH];
	char	strNewName[dNAME_LENGTH];
};

struct WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wResult;
	WORD	wWorldServerIndex;
	char	strMasterName[dNAME_LENGTH];
	char	strNewMasterName[dNAME_LENGTH];
	char	strNewGuildName[dGUILD_NAME_LENGTH];
	char	strGuildName[dGUILD_NAME_LENGTH];
};

struct WORLDtoSERVERS_CHANGE_GVG_SERVER_GUILD_MASTER
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strGuildName[dGUILD_NAME_LENGTH];
	char	strNewMasterID[dID_LENGTH];
};

struct SERVERStoWORLD_CHANGE_GVG_SERVER_GUILD_MASTER
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strGuildName[dGUILD_NAME_LENGTH];
	char	strNewMasterID[dID_LENGTH];
};

struct G2W_ASK_GVG_AVATAR_ORG_DATA						//	0x846f	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wOrgServer;
	WORD	wNotUseUnderBarName;	//	¿Ã∏ß¿Ã ±ÊæÅEπÿ¡Ÿ(_)¿ª ∏ÅE∫Ÿ¿Ã∞ÅEπŸ∑Œ æÀ∆ƒ∫™¿ª ∫Ÿ¿Œ ¿Ã∏ß¿Ã¥Ÿ.
	char	strName[dNAME_LENGTH];	//	
	char	strId[dID_LENGTH];	//	
};

struct W2L_TOSS_ASK_GVG_AVATAR_ORG_DATA					//	0x8470	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂ ≈‰Ω∫
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wOrgServer;
	WORD	wNotUseUnderBarName;	//	¿Ã∏ß¿Ã ±ÊæÅEπÿ¡Ÿ(_)¿ª ∏ÅE∫Ÿ¿Ã∞ÅEπŸ∑Œ æÀ∆ƒ∫™¿ª ∫Ÿ¿Œ ¿Ã∏ß¿Ã¥Ÿ.
	char	strName[dNAME_LENGTH];	//	
	char	strId[dID_LENGTH];	//	
};

struct L2W_TOSS_ASK_GVG_AVATAR_ORG_DATA					//	0x8471	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂ ≈‰Ω∫
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wOrgServer;
	WORD	wNotUseUnderBarName;	//	¿Ã∏ß¿Ã ±ÊæÅEπÿ¡Ÿ(_)¿ª ∏ÅE∫Ÿ¿Ã∞ÅEπŸ∑Œ æÀ∆ƒ∫™¿ª ∫Ÿ¿Œ ¿Ã∏ß¿Ã¥Ÿ.
	char	strName[dNAME_LENGTH];	//	
	char	strId[dID_LENGTH];	//	
};

struct W2D_ASK_GVG_AVATAR_ORG_DATA						//	0x8472	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wNotUseUnderBarName;	//	¿Ã∏ß¿Ã ±ÊæÅEπÿ¡Ÿ(_)¿ª ∏ÅE∫Ÿ¿Ã∞ÅEπŸ∑Œ æÀ∆ƒ∫™¿ª ∫Ÿ¿Œ ¿Ã∏ß¿Ã¥Ÿ.
	char	strId[dID_LENGTH];	//	
	char	strName[dNAME_LENGTH];	//	
};

struct D2W_GVG_AVATAR_ORG_DATA							//	0x8473	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strName[dNAME_LENGTH];	//	
	CPlayerSaveDataForServerDefine	avatarData;
};

struct W2L_GVG_AVATAR_ORG_DATA							//	0x8474	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strName[dNAME_LENGTH];	//	
	CPlayerSaveDataForServerDefine	avatarData;
};

struct L2W_GVG_AVATAR_ORG_DATA							//	0x8475	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strName[dNAME_LENGTH];	//	
	CPlayerSaveDataForServerDefine	avatarData;
};

struct W2G_GVG_AVATAR_ORG_DATA							//	0x8476	//	G.V.G º≠πˆø°º≠ ∫ªº≠πÅE∫ª∑° ƒ≥∏Ø≈Õ µ•¿Ã≈Õ ø‰√ª ∆–≈∂
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strName[dNAME_LENGTH];	//	
	CPlayerSaveDataForServerDefine	avatarData;
};


struct W2G_ASK_GVG_AVATAR_ORG_DATA_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strName[dNAME_LENGTH];
	WORD	wResult;
};

struct L2W_ASK_GVG_AVATAR_ORG_DATA_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strName[dNAME_LENGTH];
	WORD	wResult;
};

struct W2L_ASK_GVG_AVATAR_ORG_DATA_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strName[dNAME_LENGTH];
	WORD	wResult;
};

struct D2W_ASK_GVG_AVATAR_ORG_DATA_RESULT
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strName[dNAME_LENGTH];
	WORD	wResult;
};

struct	SERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_GOLD
{
	cMSG_BASE_TYPE_FORWORLD			base;

	int		iGuild;
	WORD	wRank;
	int		iGold;
};

struct	SERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_ITEM
{
	cMSG_BASE_TYPE_FORWORLD			base;

	int		iGuild;
	WORD	wRank,wStorage;
	WORD	wItemCount;
};

struct	SERVERStoWORLD_CHANGE_GI_STORAGE_SIZE
{
	cMSG_BASE_TYPE_FORWORLD			base;

	int		iGuild;
	BYTE	abStorageSize[4];
};

struct	SERVERStoWORLD_USER_JOINGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	char		strName[dGUILD_NAME_LENGTH];
	char		strMasterName[dNAME_LENGTH];
	char		strTargetId[dID_LENGTH];
	char		strTargetName[dNAME_LENGTH];
	WORD		wRank;
	WORD		wJob;
	WORD		wLevel;
};

//
//	Exit Guild Asker
enum
{
	eEGA_SELF,
	eEGA_GUILD_MASTER,
	eEGA_SUB_GUILD_MASTER,
};


struct	SERVERStoWORLD_USER_EXITGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strMasterName[dNAME_LENGTH];
	char		strTargetName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_CHECK_GUILD_CHECKSUM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildCount;
	DWORD		dwTodayGBS_CheckSum;
	DWORD		dwGuildHallCheckSum;
	WORD		awCheckGuild[10];
	int			aiCheckSum[10];
};

struct	SERVERStoWORLD_STORE_GUILD_INVENTORY_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iGuild;
	int			iGuildInventorySlot;
	WORD		wInventorySlot;
	CItemDefine	item;
	char		strMemberName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_ADD_RELATED_PLACE_OF_GUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iGuild,iFieldSerial,iGold;
	WORD		wIsAdd;
	char		strMemberName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_RECORD_GUILD_HONOR_POINT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iGuild,iGuildHonorPoint;
};

struct	SERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iGuild;
	int			iGuildInventorySlot;

	CItemDefine	item;
	char		strMemberName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_STORE_GUILD_INVENTORY_GOLD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iGuild;
	int			iGold;
	char		strMemberName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_GOLD
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	int			iGuild;
	int			iGold;
	char		strMemberName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_CHANGE_GUILD_INVENTORY_ITEM_PLACE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iGuild,iSlot1,iSlot2;
	char		strMemberName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_USER_DELETEGUILD	//	±ÊµÅE∏∂Ω∫≈Õ & «ˆ¿ÅE¡¢º”¡ﬂ¿Œ ±ÊµÂøÅE& ∏µÅE±ÊµÂø¯¿« ¡§∫∏ ∞ªΩ≈
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strName[dGUILD_NAME_LENGTH];
	char		strRequestMan[dNAME_LENGTH];
};

#define	dGUILD_APPONT							1
#define	dGUILD_DISMISSAL						2
#define	dGUILD_CHANGERANK_MASTER_APPONT_SPECIAL	3

struct	SERVERStoWORLD_USER_CHANGERANK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	WORD		wType;
	char		strMaster[dNAME_LENGTH];
	char		strTarget[dNAME_LENGTH];
	WORD		wRank;
};

#define	dUPDATE_GUILD_EXP_BUFFER_SIZE	dGUILD_MAX_COUNT

class	CGuildExpInfo
{
public:
	WORD		m_wGuildSerial;
	DWORD		m_dwUpdateExp;
};

struct	SERVERStoWORLD_UPDATE_SERVER_EXP
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD			wCount;
	CGuildExpInfo	aExpData[dUPDATE_GUILD_EXP_BUFFER_SIZE];
};

struct SERVERStoWORLD_UPDATE_NOTICE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strMaster[dNAME_LENGTH];
	WORD		wType;
	char		strNotice[dGUILD_NOTICE_LENGTH];
};

struct SERVERStoWORLD_UPDATE_EXPRATE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strMaster[dNAME_LENGTH];
	WORD		wRateExp;
};

struct	SERVERStoWORLD_MAKE_GUILD_LEVEL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strMaster[dNAME_LENGTH];
	WORD		wLevel;
};

struct	SERVERStoWORLD_INCREASE_GUILD_SKILL_LEVEL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	WORD		wSkill;
};

struct	SERVERStoWORLD_CHANGE_GUILD_SKILL_STATUS
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	WORD		wSkill;
	WORD		wIsDisable;
};

struct	SERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strName[dNAME_LENGTH];
	WORD		wLevel;
};

//
//	Result of Booking Guild Battle
enum
{
	eRBGB_SUCCESS					,
	eRBGB_ALREADY_EXIST_SCHEDULE	,	//	¿ÃπÃ Ω≈√ª «ﬂ¿›æ∆!!
	eRBGB_ALREADY_BOOKED			,	//	¿ÃπÃ øπæ‡µ«æÅE¿÷¥Ÿ.
	eRBGB_OVER_BATTLE_FOR_THIS_WEEK	,	//	¿Ãπ¯¡÷ ¿ÅEı¥¬ ≥°≥µ¥Ÿ.

	eRBGB_NOT_EXIST_GUILD			,	//	¡∏¿Á«œ¡ÅEæ ¥¬ ±ÊµÂ¥Ÿ.
	eRBGB_INCORRECT_DATA			,	//	µ•¿Ã≈Õ∞° ¿ﬂ∏ÅEµ«æ˙¥Ÿ.
	eRBGB_FULL_REQUEST_IN_THAT_TIME	,	//	±◊ Ω√∞£ø° Ω≈√ª¿Ã ¡æ∑·µ«æ˙¥Ÿ.

	eRBGB_OVER_BATTLE_FOR_THIS_DAY	,	//	ø¿¥√ ¿ÅEı¥¬ ≥°≥µ¥Ÿ.
};

struct	SERVERStoWORLD_BOOKING_GUILD_BATTLE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	WORD		wTimeOrder,wCurrentBattleOrder;
	WORD		wFieldOrder;
};

struct	SERVERStoWORLD_ADD_GUILD_GOLD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	int			iGold;
};

struct	SERVERStoWORLD_WORLD_VALUE_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD		wWorldValueSerial;
	int			iValue;
	int			iAddRemoveSet;
};


//
//	Result of Cancel Guild Battle
enum
{
	eRCGB_SUCCESS									,	//	º∫∞ÅE	
	eRCGB_VALID										,	//	øπæ‡µ»∞≈ æ¯¥Ÿ.
	eRCGB_CAN_NOT_CANCEL_BATTLE_BY_ALREADY_ENGAGED	,	//	√ÅE“«“ ºÅEæ¯¥Ÿ.
};

struct	SERVERStoWORLD_CANCEL_GUILD_BATTLE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	WORD		wTimeOrder;
	WORD		wFieldOrder;
};

struct	SERVERStoWORLD_ASK_GUILD_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wGuildSerial;
};

struct	SERVERStoWORLD_ASK_GUILD_BATTLE_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wTimeOrder,wFieldOrder;
};

struct	SERVERStoWORLD_GUILD_BATTLE_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;
	WORD						awGuildSerial[2];
	BYTE						abWin[2];
};

struct	SERVERStoWORLD_SELECT_GUILD_MARK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wGuildSerial;
	CGuildMarkInfo				guildMarkInfo;
	WORD						bf1IsUseGuildMarkPack	:	1;
};

struct	SERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE
{
	cMSG_BASE_TYPE_FORWORLD		base;
};


struct	WORLDtoSERVERS_USER_LISTGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wCount;
	WORD	wAllCount;
	CGuildAdvanceInfo	info;	//dGUILD_COUNT_FORPACKET
};

//
//	±ÊµÅE«œ≥™¿« ¡§∫∏ æ˜µ•¿Ã∆Æ
struct	WORLDtoSERVERS_UPDATE_GUILD_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wGuildSerial;
	WORD						wIsRequireResetMemberInfo;
	CGuildAdvanceInfo			info;	//dGUILD_COUNT_FORPACKET
};

//
//	±ÊµÂ¿ÅE¡§∫∏
struct	WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	CGuildBattleSchedule		todaySchedule;
};

//
//	±ÊµÂ¿ÅE∞·∞ÅE
struct	WORLDtoSERVERS_GUILD_BATTLE_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	CGuildBattleResultInfo		aBattleGuild[2];
};

struct	WORLDtoSERVERS_GUILD_BATTLE_TODAY_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	CGuildBattleSchedule		todaySchedule;
	WORD						wIsChangeDay;
};

struct	WORLDtoSERVERS_SELECT_GUILD_MARK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wGuildSerial;
	CGuildMarkInfo				guildMarkInfo;
	WORD						bf1IsUseGuildMarkPack	:	1;
};

struct	WORLDtoSERVERS_UPDATE_GUILD_MEMBER_LEVEL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wGuildSerial;
	char						strName[dNAME_LENGTH];
	WORD						wLevel;
};

struct	WORLDtoSERVERS_RESET_WEEK_FIGHT_GUILD_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

struct	WORLDtoSERVERS_SET_GUILD_HALL_LEVEL
{
	cMSG_BASE_TYPE_FORWORLD			base;

	int			iGuildSerial;
	int			iHallLevel;
};

struct	WORLDtoSERVERS_SET_GUILD_HALL_ROEN_GOOD_WILL
{
	cMSG_BASE_TYPE_FORWORLD			base;
	
	int			iGuildSerial;
	int			iGoodWill;
};

struct	WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD
{
	cMSG_BASE_TYPE_FORWORLD			base;

	int		iGuild;
	WORD	wRank;
	int		iGold;
};

struct	WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_ITEM
{
	cMSG_BASE_TYPE_FORWORLD			base;

	int		iGuild;
	WORD	wRank,wStorage;
	WORD	wItemCount;
};

struct	WORLDtoSERVERS_STORE_GUILD_INVENTORY_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wIsFailed;
	WORD		wInventorySlot;
	int			iGuild;
	int			iGuildInventorySlot;
	CItemDefine	item;
	char		strMemberName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wIsFailed;
	int			iGuild;
	int			iGuildInventorySlot;
	CItemDefine	item;
	char		strMemberName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_STORE_GUILD_INVENTORY_GOLD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wIsFailed;
	int			iGuild;
	int			iGold;
	char		strMemberName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_GOLD
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD		wIsFailed;
	int			iGuild;
	int			iGold;
	char		strMemberName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_CHANGE_GUILD_INVENTORY_ITEM_PLACE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wIsFailed;
	int			iGuild,iSlot1,iSlot2;
	char		strMemberName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_ADD_RELATED_PLACE_OF_GUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iGuild,iFieldSerial,iGold;
	WORD		wIsAdd,wIsSuccess;
	char		strMemberName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_RECORD_GUILD_HONOR_POINT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iGuild,iGuildHonorPoint;
	WORD		bf1IsVictoryPoint					:	1;
	WORD		bf2TryGuildPointBattleCountPerWeek	:	2;
};

struct	WORLDtoSERVERS_GUILD_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	int			iGuild,iGuildHonorPoint;
	WORD		bf1IsVictoryPoint					:	1;
	WORD		bf2TryGuildPointBattleCountPerWeek	:	2;
};



struct	WORLDtoSERVERS_CHANGE_GI_STORAGE_SIZE
{
	cMSG_BASE_TYPE_FORWORLD			base;

	int		iGuild;
	BYTE	abStorageSize[4];
};


struct	WORLDtoSERVERS_UPDATE_GUILD_GOLD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	LONGLONG	llGuildGold;
};

//
//	¡∏¿Á«œ¡ÅEæ ¥¬ ±ÊµÂ¥Ÿ.
struct	WORLDtoSERVERS_VALID_GUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wGuildSerial;
};


struct	WORLDtoSERVERS_USER_GUILDINFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	WORD		wAllCount;
	WORD		wListCount;
	CGuildMan	List[dGUILD_MAX_PLAYER];
};


#define	dCREATEGUILD_RESULT_SUCCESS		0x01
#define	dCREATEGUILD_RESULT_FULL		0x02		//	¥ı¿ÃªÅE∏∏µÈºÅEæ¯Ω¿¥œ¥Ÿ.
#define	dCREATEGUILD_RESULT_SAMENAME	0x03		//	∞∞¿∫ ¿Ã∏ß¿« ±ÊµÂ∞° ¡∏¿Á«—¥Ÿ.
#define	dCREATEGUILD_RESULT_HAVEGUILD	0x04		//	∏∂Ω∫≈Õ∞° ¥Ÿ∏• ±ÊµÂ∞° ¿÷¿›æ∆ -_-a
#define	dCREATEGUILD_RESULT_FAIL		0x05		//	¥Ÿ∏• ¿Ã¿Ø∑Œ Ω«∆–

struct	WORLDtoSERVERS_USER_CREATEGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	Result;
	WORD	wGuildSerial;
	char	strMasterName[dNAME_LENGTH];
	char	strSubMasterName[dNAME_LENGTH];
	char	strGuildName[dGUILD_NAME_LENGTH];
};

#define	dJOINGUILD_RESULT_SUCCESS		0x01
#define	dJOINGUILD_RESULT_NOTFIND		0x02	//	±◊∑± ±ÊµÂ¥¬ æ¯¥¬µ• .. ;
#define	dJOINGUILD_RESULT_FULL			0x03	//	«™ ~~ øÅE
#define	dJOINGUILD_RESULT_JOINED		0x04	//	¿ÃπÃ ¥Ÿ∏• ±ÊµÂø° ¡∂¿Œµ«æÅE¿÷¥Ÿ.
#define	dJOINGUILD_RESULT_NEEDRANK		0x05
#define	dJOINGUILD_RESULT_FAIL			0x06	//	πª±˚õ~~

struct	WORLDtoSERVERS_USER_JOINGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wResult;
	WORD	wRank;
	char	strName[dGUILD_NAME_LENGTH];
	char	strMasterName[dNAME_LENGTH];
	char	strTargetName[dNAME_LENGTH];
};

#define	dEXITGUILD_RESULT_SUCCESS		0x01
#define	dEXITGUILD_RESULT_NOTFIND		0x02	//	±◊∑± ±ÊµÂ¥¬ æ¯¥¬µ• .. ;
#define	dEXITGUILD_RESULT_NOTJOIN		0x03	//	≥Õ ±ÊµÅEæ¯Ω∫ ~~
#define	dEXITGUILD_RESULT_NOTACOUNT		0x04	//	¡§∫∏∞° ¿œƒ°«œ¡ÅEæ Ω¿¥œ¥Ÿ.
#define	dEXITGUILD_RESULT_FAIL			0x05	//	πª±˚õ~~
#define dEXITGUILD_RESULT_NEEDRANK		0x06	//	πª±˚õ~~
#define dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_GUILD_MASTER	0x07	//	πª±˚õ~~
#define dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_ELDER			0x08
#define dEXITGUILD_RESULT_FAILED_BY_IT_IS_ABLE_ONLY_MASTER	0x09

struct	WORLDtoSERVERS_USER_EXITGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wResult;
	WORD	wGuildSerial;
	char	strMasterName[dNAME_LENGTH];
	char	strTargetName[dNAME_LENGTH];
};

#define	dDELETEGUILD_RESULT_SUCCESS		0x01		
#define	dDELETEGUILD_RESULT_FAIL		0x02	//	π∫ ¿Ã¿Ø∞° ¿÷¿ª±ÅE-_-a

struct	WORLDtoSERVERS_USER_DELETEGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wResult;
	WORD	wGuildSerial;
	char	strRequestMan[dNAME_LENGTH];
	char	strGuildName[dGUILD_NAME_LENGTH];
};

struct	WORLDtoSERVERS_DELETEGUILD
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wGuildSerial;
	char	strGuildName[dGUILD_NAME_LENGTH];
};

#define	dCHANGERANK_RESULT_SUCCESS		0x01
#define	dCHANGERANK_RESULT_NOTFIND		0x02	//	±◊∑± ±ÊµÂ¥¬ æ¯¥¬µ• .. ;
#define	dCHANGERANK_RESULT_NOTACOUNT	0x03	//	¡§∫∏∞° ¿œƒ°«œ¡ÅEæ Ω¿¥œ¥Ÿ.
#define	dCHANGERANK_RESULT_FAIL			0x04	//	πª±˚õ~~

struct WORLDtoSERVERS_USER_CHANGERANK			//	º∫∞¯«ﬂ¿ª ∞ÊøÅE¿ÅEº≠πˆø°∞‘...
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wResult;
	WORD		wGuildSerial;
	WORD		wType;
	char		strMaster[dNAME_LENGTH];
	char		strTarget[dNAME_LENGTH];
	WORD		wRank;
};

struct WORLDtoSERVERS_USER_CHANGERANK_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strTarget[dNAME_LENGTH];
	WORD		wRank;
};

struct	WORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	WORD		wSkill,wLevel,wGuildPoint;
};

//
//	Result of Increase Guild Skill Level
enum
{
	eRIGSL_OK,	//	º∫∞ÅE	
	eRIGSL_SKILL_POINT_LACKS,	//	Ω∫≈≥∆˜¿Œ∆Æ∞° ∫Œ¡∑«œ¥Ÿ.
	eRIGSL_LEVEL_SKILL_IS_MAX,	//	Ω∫≈≥∑π∫ß¿Ã ∏∆Ω∫¥Ÿ.
};

//
//	±ÊµÅE∑π∫ß ¡ı∞° Ω«∆–. ¥ÅEÛ¿∫ «ÿ¥ÅE±ÊµÂ¿« ±Ê∏∂(
struct	WORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wResult;
	WORD		wGuildSerial;
};

struct	WORLDtoSERVERS_BOOKING_GUILD_BATTLE_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wResult;
};

struct	WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wTimeOrder;
	WORD						wFieldOrder;
	WORD						awBattleGuild[2];	//	¥ÅEÅE“ µŒ ±ÊµÅE	
	WORD						wGuildSerial;
};

/////////////////////////////////////////////////////////////////////////////
//	∏µÅEº≠πˆø°∞‘ ∫∏≥ª¡ÅE±ÊµÅE¡§∫∏µÅE.
/////////////////////////////////////////////////////////////////////////////
struct	WORLDtoSERVERS_GUILD_CREATE
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	CGuildAdvanceInfo			info;
};

struct	WORLDtoSERVERS_GUILD_JOIN
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strGuildName[dGUILD_NAME_LENGTH];
	CGuildMan	member;
};

struct	WORLDtoSERVERS_GUILD_EXIT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strGuildName[dGUILD_NAME_LENGTH];
	char		strMemberName[dNAME_LENGTH];
};

class	CGuildExpInfoIncludeLevel
{
public:
	WORD		m_wGuildSerial;
	WORD		m_wLevel;
	WORD		m_wGuildPoint;
	WORD		m_wExpTaxRate;
	DWORD		m_dwUpdateExp;
};

struct	WORLDtoSERVERS_GUILD_UPDATE_EXP
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wCount;
	CGuildExpInfoIncludeLevel	aExpData[dUPDATE_GUILD_EXP_BUFFER_SIZE];

};

struct WORLDtoSERVERS_GUILD_NOTICE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	char		strNotice[dGUILD_NOTICE_LENGTH];
};

struct WORLDtoSERVERS_GUILD_EXPRATE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGuildSerial;
	WORD		wRateExp;
};

struct WORLDtoSERVERS_UPDATE_GUILD_HALL_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wCount,wIsIsBuildSiegeWarfareSchedule;
	cGuildHallInfo				aHallList[c_iGuildHallCount];
};


//¶≠	PACKET Guild
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠	PACKET Party

struct	SERVERStoWORLD_CREATE_UNION_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD						wAskerPartySerial;
	WORD						wReplayerPartySerial;
};

struct	SERVERStoWORLD_CREATE_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wFieldSerial;		//	¿Ø¥œ≈© « µÅEΩ√∏ÆæÅE	
	int		iMasterZoneSerial,iTargetZoneSerial;
	int		iMasterDuelVP,iTargetDuelVP;

	DWORD	bf10MasterLevel					:	10;
	DWORD	bf8MasterJob					:	8;
	DWORD	bf8MasterRemainHP				:	8;
	DWORD	bf4MasterSecretDungeonPlayCount	:	4;

	DWORD	bf10TargetLevel					:	10;
	DWORD	bf8TargetJob					:	8;
	DWORD	bf8TargetRemainHP				:	8;
	DWORD	bf4TargetSecretDungeonPlayCount	:	4;

	char	strName[dNAME_LENGTH];
	char	strTName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_JOIN_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;	//	∆ƒ∆º Ω√∏ÆæÅE	
	WORD	wFieldSerial;	//	¿Ø¥œ≈© « µÅEΩ√∏ÆæÅE	
	int		iZoneSerial,iDuelVP;

	WORD	bf7RemainHP				:	8;
	WORD	bf4SecretDungeonPlayCount:	4;

	DWORD	bf10Level					:	10;
	DWORD	bf8Job						:	8;
	DWORD	bf10InstanceField			:	10;
	DWORD	bf4InstanceFieldFloor		:	4;
	DWORD	bf1IsReconnect				:	1;
	DWORD	bf1IsByJoinPartyApplication	:	1;

	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_PARTY_WORK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;			//	∆ƒ∆º Ω√∏ÆæÅE-> ∆ƒ∆º ¿Ã∏ß¿∏∑Œ ±≥√º«“ ∞Õ !!
	DWORD	dwUniqueSerial;
	WORD	wWork;

	char	strName[dNAME_LENGTH];	//	≈ª≈«“ ªÁ∂˜¿« ¿Ã∏ß 
};

struct	SERVERStoWORLD_UNION_PARTY_WORK					//ø¨«’ ∆ƒ∆ºø° ∞ÅEÅE∏ﬁºº¡ˆ∏¶ ø˘µÂ∑Œ ¿ÅEﬁ«—¥Ÿ.
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD						wPartySerial;			//	∆ƒ∆º Ω√∏ÆæÅE-> ∆ƒ∆º ¿Ã∏ß¿∏∑Œ ±≥√º«“ ∞Õ !!
	WORD						wUnionPartySerial;
	WORD						wWork;
	
	char						strName[dNAME_LENGTH];	//	≈ª≈«“ ªÁ∂˜¿« ¿Ã∏ß 
};

struct	SERVERStoWORLD_DISCONNECT_PARTY_MEMBER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;			//	∆ƒ∆º Ω√∏ÆæÅE-> ∆ƒ∆º ¿Ã∏ß¿∏∑Œ ±≥√º«“ ∞Õ !!

	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];	//	≈ª≈«“ ªÁ∂˜¿« ¿Ã∏ß 
};

struct	SERVERStoWORLD_PARTY_ENTER_IF
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;
	char	strPartyName[dPARTYNAME_LENGTH];
	WORD	wGateField,wGateArea;
};

enum
{
	eJPA_OK,
	eJPA_DENY,
	eJPA_FAILED,
};

enum
{
	eGMJPA_DENY, // ∞≈¿˝
	eGMJPA_OK, // ø¿ƒ…¿Ã
	eGMJPA_HAVE_NO_TARGET, //	 ¥ÅEÛ¿Ã æ¯¥Ÿ.
	eGMJPA_TARGET_IS_IN_PARTY,	// ¥ÅEÛ¿Ã ∆ƒ∆º∞° ¿÷¥Ÿ.
	eGMJPA_PARTY_IS_FULL,		//	 ∆ƒ∆º∞° «Æ¿Ãø°øÅE	
	eGMJPA_FAILED,			//	 Ω«∆–?
};

struct	SERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wAskerJob,wAskerLevel;

	char	strTargetName[dNAME_LENGTH];
	char	strAskerName[dNAME_LENGTH];
};



struct	WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wAskerJob,wAskerLevel;

	char	strTargetName[dNAME_LENGTH];
	char	strAskerName[dNAME_LENGTH];
};


struct	SERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY_ANSWER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wAnswer;

	char	strTargetName[dNAME_LENGTH];
	char	strAskerName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY_ANSWER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wAnswer;

	char	strTargetName[dNAME_LENGTH];
	char	strAskerName[dNAME_LENGTH];

};

struct	SERVERStoWORLD_JOIN_PARTY_APPLICATION
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wAskerJob,wAskerLevel;

	char	strLeaderName[dNAME_LENGTH];
	char	strAskerName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_JOIN_PARTY_APPLICATION_ANSWER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wAnswer;
	WORD	wPartySerial;

	char	strLeaderName[dNAME_LENGTH];
	char	strAskerName[dNAME_LENGTH];
	char	strPartyName[dPARTYNAME_LENGTH];
};

struct	WORLDtoSERVERS_JOIN_PARTY_APPLICATION
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wAskerLevel,wAskerJob;

	char	strLeaderName[dNAME_LENGTH];
	char	strAskerName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_JOIN_PARTY_APPLICATION_ANSWER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wAnswer;
	WORD	wPartySerial;

	char	strLeaderName[dNAME_LENGTH];
	char	strAskerName[dNAME_LENGTH];
	char	strPartyName[dPARTYNAME_LENGTH];
};

struct	SERVERStoWORLD_RENAME_PARTYNAME
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;				//	∆ƒ∆º Ω√∏ÆæÅE
	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dPARTYNAME_LENGTH];	//	∆ƒ∆º ¿Ã∏ß
};

struct	SERVERStoWORLD_ASK_PARTY_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD	dwSerial;					//	πﬁ¿ª ¿Ø¿˙¿« Ω√∏ÆæÅE	
	WORD	isNext;						//	0 - ¿ÅE∆‰¿Ã¡ÅE, 1 - ¥Ÿ¿Ω ∆‰¿Ã¡ÅE	
	WORD	wPartyIndex;				//	πﬁ¿ª ±‚¡ÿ¿Ã µ«¥¬ ∆ƒ∆º¿« ¿Œµ¶Ω∫
};

struct SERVERStoWORLD_ASK_PARTYINFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;
	char	strPartyName[dPARTYNAME_LENGTH];
};

struct SERVERStoWORLD_CHANGE_PARTY_STATUS
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;
	DWORD	dwUniqueSerial;

	DWORD	bf1IsChangeSetting		:	1;
	DWORD	bf3GoldShareMethod		:	3;
	DWORD	bf3ItemShareMethod		:	3;
	DWORD	bf3PartyObject			:	3;	//∆ƒ∆º ∏Ò¿ÅE	
	DWORD	bf1IsOpenParty			:	1;	//∏‚πˆ∏¶ ∞Ëº” ∏¡˝«—¥Ÿ.

	DWORD	bf3LastReceiveGoldMember:	3;
	DWORD	bf3LastReceiveItemMember:	3;
};

struct WORLDtoSERVERS_CHANGE_PARTY_STATUS
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;
	DWORD	dwUniqueSerial;

	DWORD	bf1IsChangeSetting		:	1;
	DWORD	bf3GoldShareMethod		:	3;
	DWORD	bf3ItemShareMethod		:	3;
	DWORD	bf3PartyObject			:	3;	//∆ƒ∆º ∏Ò¿ÅE	
	DWORD	bf1IsOpenParty			:	1;	//∏‚πˆ∏¶ ∞Ëº” ∏¡˝«—¥Ÿ.

	DWORD	bf3LastReceiveGoldMember:	3;
	DWORD	bf3LastReceiveItemMember:	3;

};

struct SERVERStoWORLD_CHANGE_PARTY_MEMBER_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;
	DWORD	dwUniqueSerial;
	DWORD	bf10Level	:	10;
	DWORD	bf4PSC		:	4;
	DWORD	bf8Job		:	8;
	DWORD	bf3GDPC		:	3;

	char	strName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_CHECK_PARTY_INFO_BY_ACTOR
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;

	char	strName[dNAME_LENGTH];
};

struct	SERVERStoWORLD_MOVE_FIELD_PARTY_MEMBER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];
	int		iZoneSerial				:	31;
	int		bf1IsLeave				:	1;	//	±◊ « µÂø°º≠ ∂∞≥™¥¬∞≈≥ƒ?
	WORD	bf10InstanceField		:	10;
	WORD	bf4InstanceFieldFloor	:	4;

	WORD	wDestFieldSerial;	//	∏Ò¿˚¡ÅE};
};

#define	dJOINPARTY_RESULT_SUCCESS		0x01
#define	dJOINPARTY_RESULT_FAIL			0x02

struct	WORLDtoSERVERS_JOIN_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	short	sResult;
	WORD	wPartySerial;
	int		iZoneSerial;

	WORD	wFieldSerial;
	WORD	wRemainHP;

	DWORD	bf10Level					:	10;
	DWORD	bf8Job						:	8;
	DWORD	bf10InstanceField			:	10;
	DWORD	bf4InstanceFieldFloor		:	4;

	WORD	bf4SecretDungeonPlayCount	:	4;
	WORD	bf3GuildDungeonPlayCount	:	4;

	int		iDuelVP;	//	∞·≈ÅEΩ¬¡°

	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_PARTY_WORK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;				//	±ÊµÅE¿Œµ¶Ω∫ - ¿⁄Ω≈¿« ±ÊµÂ¿Œ¡ÅE.
	DWORD	dwUniqueSerial;
	WORD	wWork;
	char	strName[dNAME_LENGTH];	//	ªË¡¶µ… ¿Ø¿˙¿« æ∆¿ÃµÅE
};

struct	WORLDtoSERVERS_DISCONNECT_PARTY_MEMBER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;				//	±ÊµÅE¿Œµ¶Ω∫ - ¿⁄Ω≈¿« ±ÊµÂ¿Œ¡ÅE.
	WORD	wIsBanish;
	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];	//	ªË¡¶µ… ¿Ø¿˙¿« æ∆¿ÃµÅE
};

struct	WORLDtoSERVERS_UPDATE_PARTYINFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wIsCreate;		//	¿Ã¡¶∏∑ ∏∏µÈæ˚›ÅE∆ƒ∆º¥Ÿ.
	
	CSimplePartyInfo			partyInfo;
};

struct	WORLDtoSERVERS_UPDATE_UNIONPARTYINFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD						wIsCreate;					//	¿Ã¡¶∏∑ ∏∏µÈæ˚›ÅE∆ƒ∆º¥Ÿ.
	char						strName[dNAME_LENGTH];		//	ø¨«’ ∆ƒ∆º ¿Ã∏ß.
	DWORD						m_dwUniqueSerial;			//	¿Ø¥œ≈© Ω√∏ÆæÅE
	
	CSimpleUnionPartyInfo		unionPartyInfo;
};

struct	WORLDtoSERVERS_VALID_PARTY	
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wPartyIndex;
	char						strName[dPARTYNAME_LENGTH];	
};

struct	WORLDtoSERVERS_FAILED_TO_CREATE_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						sReason;
	char						strName[dNAME_LENGTH];
};

struct WORLDtoSERVERS_DISSOLUTION_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;
	char						strPartyName[dPARTYNAME_LENGTH];
};

struct WORLDtoSERVERS_RENAME_PARTYNAME
{
	cMSG_BASE_TYPE_FORWORLD		base;

	short	sResult;
	WORD	wPartySerial;				//	∆ƒ∆º Ω√∏ÆæÅE	
	char	strName[dPARTYNAME_LENGTH];	//	∆ƒ∆º ¿Ã∏ß
	char	strChangeName[dPARTYNAME_LENGTH];	//	∆ƒ∆º ¿Ã∏ß
};

struct	WORLDtoSERVERS_PARTY_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD						dwSerial;	//	πﬁ¿ª ¿Ø¿˙¿« Ω√∏ÆæÅE	
	WORD						wPartySIndex,wPartyEIndex;	//	πﬁ¿ª ±‚¡ÿ¿Ã µ«¥¬ ∆ƒ∆º¿« ¿Œµ¶Ω∫
	WORD						wPartyCount;				//	≥—æ˚Ã• ∞πºÅE
	CSimplePartyInfo			pInfo[dPARTYLIST_COUNT];
};

struct WORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;
	DWORD	dwUniqueSerial;
	char	strName[dNAME_LENGTH];

	DWORD	bf10Level	:	10;
	DWORD	bf4PSC		:	4;
	DWORD	bf8Job		:	8;
	DWORD	bf3GDPC		:	4;
};

struct	WORLDtoSERVERS_MOVE_FIELD_PARTY_MEMBER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;
	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];
	int		iZoneSerial;
	int		bf1IsLeave			:	1;	//	±◊ « µÂø°º≠ ∂∞≥™¥¬∞≈≥ƒ?
	WORD	bf10IFSerial		:	10;
	WORD	bf4IFFloor			:	4;

	WORD	wDestFieldSerial;	//	∏Ò¿˚¡ÅE};
};

//¶≠	PACKET Party
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠æ∆¿Ã≈€ √≥∏Æ

//	Game server -> World server
//	« µÂø° ∂≥æ˚“ﬂ∏Æ¥¬ æ∆¿Ã≈€ ª˝º∫
struct SERVERStoWORLD_CREATE_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wCount;//æ∆¿Ã≈€ ºÅE	
	WORD						wFieldSerial;//« µÅEΩ√∏ÆæÅE	
	cCREATE_ITEM_INFO			aCreateItem[dDROPPING_ITEM_COUNT+1];//ª˝º∫«“ æ∆¿Ã≈€
};	//	struct SERVERStoWORLD_CREATE_ITEM

//	World server -> Game server
//	« µÂø° ∂≥æ˚“ﬂ∏Æ¥¬ æ∆¿Ã≈€ ª˝º∫ ∞·∞˙∏¶ πﬁ¿Ω
struct WORLDtoSERVERS_CREATE_ITEM_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wCount;//æ∆¿Ã≈€ ºÅE	
	WORD						wFieldSerial;//« µÅEΩ√∏ÆæÅE	
	cRESULT_CREATE_ITEM_INFO	aCreateItemResult[dDROPPING_ITEM_COUNT+1];
};	//	struct WORLDtoSERVERS_CREATE_ITEM_RESULT

//	Game server -> World server
//	ªÛ¡°ø°º≠ ∆«∏≈«œ¥¬ æ∆¿Ã≈€ ª˝º∫
struct SERVERStoWORLD_CREATE_SELL_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int							iSerialInServer;
	DWORD						dwPrice;	//	∞°∞›
	char						strName[dNAME_LENGTH];
	CItemDefine					item;	//	∆«∏≈«“ æ∆¿Ã≈€
	WORD						wCount;
	WORD						wWhere;
	WORD						wIsDuelPointShop;
	WORD						wIsTokenShop;
};	//	struct SERVERStoWORLD_CREATE_SELL_ITEM

//	World server -> Game server
//	ªÛ¡°ø°º≠ ∆«∏≈«œ¥¬ æ∆¿Ã≈€ ª˝º∫ ∞·∞ÅE
struct WORLDtoSERVERS_CREATE_SELL_ITEM_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char						strName[dNAME_LENGTH];
	int							iSerialInServer;
	DWORD						dwPrice;	//	∞°∞›
	DWORD						dwSerial;
	WORD						wCount,wWhere;
	CItemDefine					item;	//	∆«∏≈«“ æ∆¿Ã≈€
	WORD						wIsDuelPointShop;
	WORD						wIsTokenShop;
};	//	struct SERVERStoWORLD_CREATE_SELL_ITEM

//	Game server -> World server
//	æ∆¿Ã≈€ ª˝º∫
struct SERVERStoWORLD_ADD_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wClient;//	∞˙Ã¥ -o-
	CItemDefine					item;	//	∆«∏≈«“ æ∆¿Ã≈€
};	//	struct SERVERStoWORLD_ADD_ITEM

//	World server -> Game server
//	æ∆¿Ã≈€ ª˝º∫
struct WORLDtoSERVERS_ADD_ITEM_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wClient;//	∞˙Ã¥ -o-
	CItemDefine					item;	//	∆«∏≈«“ æ∆¿Ã≈€
};	//	struct WORLDtoSERVERS_ADD_ITEM

//	Game server -> World server
//	æ∆¿Ã≈€¿« ¿Ø¥œ≈© Ω√∏ÆæÛ¿ª ø‰±∏
struct SERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int							iSerialInServer;//	∞˙Ã¥ -o-
};	//	struct SERVERStoWORLD_ADD_ITEM

//	World server -> Game server
//	æ∆¿Ã≈€¿« ¿Ø¥œ≈© Ω√∏ÆæÛ¿ª ø‰±∏
struct WORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int							iSerialInServer;//	∞˙Ã¥ -o-
	DWORD						dwSerial;
};	//	struct WORLDtoSERVERS_ASK_UNIQUE_SERIAL

struct SERVERStoWORLD_INCREASE_PACK_ITEM_COUNT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPackSerial,wItemIndex;
};

struct SERVERStoWORLD_RESET_DROP_PACK_ITEM_COUNT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPackSerial;
};

struct WORLDtoSERVERS_RESET_DROP_PACK_ITEM_COUNT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPackSerial;
};

struct WORLDtoSERVERS_CARROT_SHOP_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wIndex,wCount;
	WORD	wStatus;	//	0 Ω√¿€,1 µµ¡ﬂ,2 ∏∂¡ˆ∏∑

	cCarrotShopCategoryInfo2	categoryInfo;	//	µ•¿Ã≈Õ ªÁ¿Ã¡˚Ã° ∞˙’´¡Ÿ¿Ã±ÅE∂ßπÆø° ¿ßƒ° ∫Ø∞Ê«œ∏ÅEæ»µ 
	cCarrotShopCategoryInfo1	data;
};

struct WORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPackSerial,wItemIndex,wCount;
};

//¶≠æ∆¿Ã≈€ √≥∏Æ
//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
//¶≠	PACKET FOR ADMIN

struct	WORLDtoSERVERS_ITEM_PACK_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	cItemPackInfo	info;
};

struct	WORLDtoSERVERS_ITEM_PACK_LIST_SERIAL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wCount;
	WORD	awList[dMAX_ITEM_PACK_COUNT];
};

struct	WORLDtoSERVERS_LOTTO_EVENT_DATA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD				wCount;
	cLottoItemDropEvent	aEventList[dLOTTO_EVENT_COUNT];
};

struct	SERVERStoWORLD_ASK_ITEM_PACK_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPackSerial;
};

struct SERVERStoWORLD_ASK_NEXT_ITEM_PACK_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wCurrentPack;
};

struct SERVERStoWORLD_ASK_CARROT_SHOP_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

struct SERVERStoWORLD_UPDATE_CARROT_SHOP_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wIndex,wCount;
	WORD	wStatus;	//	0 Ω√¿€,1 ¡¯«ÅE2 ¡æ∑ÅE
	cCarrotShopCategoryInfoDefine	data;
};

struct	SERVERStoWORLD_UPDATE_ITEM_PACK_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD			wSerial;
	cItemPackInfo	info;
};

struct	SERVERStoWORLD_ITEM_PACK_INFO_CHECKSUM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD	dwCheckSum;
};

struct	SERVERStoWORLD_ASK_GR_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
};
struct	SERVERStoWORLD_UPDATE_GR_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	UTime	begin,end;
	WORD	wDropChance,wBoostExp;

	char	strBeginComment[256];
	char	strEndComment[256];
};

struct	WORLDtoSERVERS_GR_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	UTime	begin,end;
	WORD	wDropChance,wBoostExp;

	char	strBeginComment[256];
	char	strEndComment[256];
};

struct	WORLDtoSERVERS_GUILD_NAME_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	bf1IsBegin	:	1;
	WORD	bf1IsLast	:	1;
	WORD	bf14Count	:	14;
	int		iServerIndex,iLastGuild;

	char	strGuildNameList[100][dGUILD_NAME_LENGTH];
};

struct	WORLDtoSERVERS_CHANGE_GUILD_BATTLE_REPORT_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wResult;
};

struct	WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wResult;
};

struct	WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH];
	WORD	wField,wTime;
};

struct	WORLDtoSERVERS_GUILD_NAME
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wGuildSerial;
	char	strGuildName[dGUILD_NAME_LENGTH];
};

struct	WORLDtoSERVERS_ASK_ALL_ITEM_PACK_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
};


struct	WORLDtoSERVERS_SENDINFOTOADMIN
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		wGameSrvCount;							//	¡¢º”µ» ∞‘¿”º≠πÅEºÅE.. 
	DWORD		wUserCount,wMaxUserCount;				//	µø¡¢ºÅE
};

struct	SERVERStoWORLD_ALL_MSG
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strMessage[dCHAT_LENGTH];	//	∏ﬁΩ√¡ÅE
};

struct	SERVERStoWORLD_ISCONNECT_USER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strId[dID_LENGTH];
};

struct	SERVERStoWORLD_OFFSERVER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strMsg[16];
};

struct	SERVERStoWORLD_GETGUILDFIGHTLIST
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

struct	SERVERStoWORLD_APPOINTMENT_QUIZ_EVENT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wBeginYear,wBeginMonth,wBeginDay,wBeginHour,wBeginMinute;
	WORD	wLimitPlayerCount;
	WORD	wIsTest;
	WORD	wType;
	char	strEventName[128];
};

struct	SERVERStoWORLD_OX_QUIZ_LOSER_REVIVE_BATTLE
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

struct	SERVERStoWORLD_PRODUCE_OX_QUIZ
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strQuestion[256];
	WORD	wSolution;
	WORD	wCount;
};

struct	SERVERStoWORLD_PRODUCE_WORD_QUIZ
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strQuestion[256];
	char	strSolution[32];
	WORD	wCount;
};

struct	SERVERStoWORLD_CHANGE_WORD_QUIZ_ROUND
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

struct	SERVERStoWORLD_SET_EVENT_WORD
{	// ∏∂¡ˆ∏∑ ºˆ¡§¿œ : 09.10.09
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strWordOrString[256];
	
	DWORD	bf16EventKind : 16;		// 16
	DWORD	bf16BuffSerial : 16;	//32
	DWORD	dwTimeInfo;
	WORD	wBeginTimeYear;				//Ω√¿€ Ω√∞£...
	WORD	wBeginTimeMonth;
	WORD	wBeginTimeDay;
	WORD	wBeginTimeHour;
	WORD	wBeginTimeMinite;
	WORD	wEndTimeYear;				//¡æ∑ÅEΩ√∞£...
	WORD	wEndTimeMonth;
	WORD	wEndTimeDay;
	WORD	wEndTimeHour;
	WORD	wEndTimeMinite;	
	char	strBeginMent[256];
	char	strEndMent[256];
	char	strActiveMent[256];
};


struct	SERVERStoWORLD_CHANGE_MSG_OF_LEDERS_BELL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strMsg[128];
};


struct	SERVERStoWORLD_BUILD_RESULT_OF_GUILD_BATTLE_PROLOG
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD	wIsWantOnlyTest;
};

struct	SERVERStoWORLD_SYNC_CHANGEDNAME
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char		strID[dID_LENGTH];
	char		oldName[dNAME_LENGTH];
	char		newName[dNAME_LENGTH];
	WORD		guildSerial;	
};

struct	SERVERStoWORLD_MODIFY_LOTTO_EVENT_DATA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD				wCount;
	cLottoItemDropEvent	aEventList[dLOTTO_EVENT_COUNT];
};

struct	SERVERStoWORLD_MODIFY_EVENT_TIME
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD				wCount;
	WORD				wIsByLoginServer;
	cEventTimeDefine	aEventList[dMAX_EVENT_TIME_COUNT];
};

struct WORLDtoSERVERS_SYNC_CHANGEDNAME
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char		strID[dID_LENGTH];
	char		oldName[dNAME_LENGTH];
	char		newName[dNAME_LENGTH];
	WORD		guildSerial;	
};

struct	SERVERStoWORLD_END_QUIZ_EVENT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wType;
};

struct	SERVERStoWORLD_CANCEL_QUIZ_EVENT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wType;
};

struct	SERVERStoWORLD_CHANGE_FIXCHECK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	short	wWeek,wHour,wMin;
	char	strNoticeMsg[98];	//	%d∫–»ƒ º≠πˆ∞° ¡æ∑·«’¥œ¥Ÿ.æ’ø° ø√∏ª..
};

struct	SERVERStoWORLD_GET_FIXCHECK
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

struct	SERVERStoWORLD_GET_GUILDLIST
{
	cMSG_BASE_TYPE_FORWORLD		base;
};

struct	SERVERStoWORLD_GET_GUILDINFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wGuildSerial;
	char	strGuildName[dGUILD_NAME_LENGTH];
};

struct	SERVERStoWORLD_ASK_GUILD_NAME_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	int	iFirstGuild;
	int	iServerIndex;
};

struct	SERVERStoWORLD_CHANGE_GUILD_BATTLE_REPORT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strGuildMaster[dNAME_LENGTH];
	char	strGuildName[dGUILD_NAME_LENGTH];

	WORD	wGuildSerial;
	WORD	wVictoryPoint;
	WORD	wThisWeekBattleCount;
	WORD	wWinCount;
	WORD	wDrawCount;
	WORD	wDefeatCount;
	WORD	wCVCount;
};

struct	SERVERStoWORLD_BEGIN_TEST_GUILD_BATTLE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strGuild1[dGUILD_NAME_LENGTH],strGuild2[dGUILD_NAME_LENGTH];
	WORD	wField,wTime;

};

struct	SERVERStoWORLD_ASK_GUILD_NAME
{
	cMSG_BASE_TYPE_FORWORLD		base;
	WORD	wGuildSerial;
};

struct	SERVERStoWORLD_GET_GUILDMEMBERLIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wGuildSerial;
	char	strGuildName[dGUILD_NAME_LENGTH];
};

struct	SERVERStoWORLD_BANISH_USER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wIsForced;	//	∞≠¡¶∑Œ ¶iæ∆≥Ω¥Ÿ.
	char	strName[dNAME_LENGTH];
	char	strReason[256];
};

struct	SERVERStoWORLD_ADD_RENEW_ITEM_EXPIRE_TIME
{
	cMSG_BASE_TYPE_FORWORLD		base;

	UTime	time;
	DWORD	dwRenewMinute;
	WORD	wIsForOnlyYahoo;
	WORD	wIsForOnlyRedgem;
};

struct	SERVERStoWORLD_ASK_RENEW_ITEM_EXPIRE_TIME_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;
};


struct	WORLDtoSERVERS_ALL_MSG
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strMessage[dCHAT_LENGTH];	//	∏ﬁΩ√¡ÅE
};

	struct	WORLDtoSERVERS_GET_FIXCHECK
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	wWeek, wHour, wMin;
		char	strNoticeMsg[98];
	};

	typedef struct
	{
		WORD	wGuildSerial;
		char	strGuildName[dGUILD_NAME_LENGTH];
	}stGuildList;

	struct	WORLDtoSERVERS_GET_GUILDLIST
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD		wCount;
		stGuildList	list[dGUILD_MAX_COUNT];
	};

	struct	WORLDtoSERVERS_GET_GUILDINFO
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	wGuildSerial;
		char	strGuildName[dGUILD_NAME_LENGTH];
		CGuildAdvanceInfo			info;
	};

	struct	WORLDtoSERVERS_ISCONNECT_USER
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char			strId[dID_LENGTH];
		char			strName[dNAME_LENGTH];
		WORD			wfieldUniqueIndex;
	};

	struct	WORLDtoSERVERS_DELETE_AVATAR
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char			strID[dID_LENGTH];
		char			strName[dNAME_LENGTH];
		WORD			wAvatarIndex;
	};

	struct	WORLDtoSERVERS_GETGUILDFIGHTLIST
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		CGuildBattleSchedule		today;
	};

	struct	WORLDtoSERVER_EVENT_WORD_INFO
	{
		cMSG_BASE_TYPE_FORWORLD		base;
		WORD						bIsActiveEvent;
		char						strEventWord[256];
		DWORD						dwTimeInfo;
		DWORD						bf16EventKind : 16;		// 16
		DWORD						bf16BuffSerial : 16;	//32
		WORD						wBeginTimeYear;				//Ω√¿€ Ω√∞£...
		WORD						wBeginTimeMonth;
		WORD						wBeginTimeDay;
		WORD						wBeginTimeHour;
		WORD						wBeginTimeMinite;
		WORD						wEndTimeYear;				//¡æ∑ÅEΩ√∞£...
		WORD						wEndTimeMonth;
		WORD						wEndTimeDay;
		WORD						wEndTimeHour;
		WORD						wEndTimeMinite;
		char						strBeginMent[256];
		char						strEndMent[256];
		char						strActiveMent[256];
	};

	struct	WORLDtoSERVERS_WORLD_VALUE_INFO_ALL
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		int							aiWouldValue[dMAX_WORLD_VALUE_MAX];
	};

	struct	WORLDtoSERVERS_WORLD_VALUE_INFO
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD						wIndex;
		int							iValue;
	};



	struct	WORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	wBeginYear, wBeginMonth, wBeginDay, wBeginHour, wBeginMinute;
		WORD	wLimitPlayerCount;
		WORD	wType, wIsTest, wGateVillage;
		char	strEventName[128];
	};

	struct	WORLDtoSERVERS_PRODUCE_OX_QUIZ
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char	strQuestion[256];
		WORD	wSolution;
		WORD	wCount;
	};

	struct	WORLDtoSERVERS_PRODUCE_WORD_QUIZ
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char	strQuestion[256];
		char	strSolution[32];
		WORD	wCount;
	};

	struct	WORLDtoSERVERS_CHANGE_WORD_QUIZ_ROUND
	{
		cMSG_BASE_TYPE_FORWORLD		base;
	};

	struct	WORLDtoSERVERS_END_QUIZ_EVENT
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	wType;
	};

	struct	WORLDtoSERVERS_CANCEL_QUIZ_EVENT
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	wType;
	};

	struct	WORLDtoSERVERS_OX_QUIZ_LOSER_REVIVE_BATTLE
	{
		cMSG_BASE_TYPE_FORWORLD		base;
	};

	struct	WORLDtoSERVERS_CHAGEGUILDBATTLE
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	isTime;			//	∏˚’¯¬∞ Ω√∞£¿Œ∞°.
		WORD	wField;			//	∏˚’¯¬∞ « µÂ¿Œ∞°..(10∞≥¡ﬂ «—∞≥)

		char	strGuildName1[dGUILD_NAME_LENGTH];
		char	strGuildName2[dGUILD_NAME_LENGTH];
	};

	struct	WORLDtoSERVERS_BANISH_USER
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	wIsForced;	//	∞≠¡¶∑Œ ¶iæ∆≥Ω¥Ÿ.
		char	strName[dNAME_LENGTH];
		char	strReason[256];
	};

	struct	WORLDtoSERVERS_BANISH_USER_RESULT
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char	strName[dNAME_LENGTH];
	};

	class	cRenewItemExpireTimeInfo
	{
	public:
		WORD	m_bf14Index : 14;
		WORD	m_bf1IsForOnlyYahoo : 1;
		WORD	m_bf1IsForOnlyRedGem : 1;
		UTime	m_time;
		DWORD	m_dwRenewMinuite;
	};

#define	dRENEW_ITEM_EXPIRE_TIME_INFO_BUFFER_SIZE	100

	struct	WORLDtoSERVERS_RENEW_ITEM_EXPIRE_TIME_LIST
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	wCount, wFullCount, wIsLast;

		cRenewItemExpireTimeInfo	aList[dRENEW_ITEM_EXPIRE_TIME_INFO_BUFFER_SIZE];
	};
	//¶≠	PACKET  FOR ADMIN
	//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

	//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
	//¶≠	PACKET 

	struct	WORLDtoSERVERS_CHECKTIME
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD		wType;	//	1 - ¥›æ∆∂ÅE 0 - ¥ŸΩ√ ø≠æ˚“ÅE
	};

	//¶≠	PACKET 
	//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

	//¶Æ¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨
	//¶≠	PACKET 

	struct WORLDtoSERVERS_PRIZE_PLAYER_RESULT
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char						strID[20];//	ID∏˙‹∏∑Œ ¿Ø¿ÅE»Æ¿Œ
		char						strName[20];//	ƒ…∏Ø≈Õ∏˙‹∏∑Œ ¿Ø¿ÅE»Æ¿Œ
		WORD						wIsPrizePlayer;
	};	//	struct WORLDtoSERVERS_ADD_ITEM

	//	Game server -> World server
	//	æ∆¿Ã≈€¿« ¿Ø¥œ≈© Ω√∏ÆæÛ¿ª ø‰±∏
	struct SERVERStoWORLD_ASK_PRIZE_PLAYER
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char						strID[20];//	ID∏˙‹∏∑Œ ¿Ø¿ÅE»Æ¿Œ
		char						strName[20];//	ƒ…∏Ø≈Õ∏˙‹∏∑Œ ¿Ø¿ÅE»Æ¿Œ
	};	//	struct SERVERStoWORLD_ADD_ITEM

	//struct
	//{
	//	cMSG_BASE_TYPE_FORWORLD		base;
	//};

	//¶≠	PACKET 
	//¶±¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨¶¨

	//////////////////////////////////////////////////////////////////////////
	//	≈ÅEË∏¶ ¿ß«— ∑Œ±◊ ±‚∑œ
	struct SERVERStoWORLD_LOG_INOUT
	{
		cMSG_BASE_TYPE_FORWORLD	base;

		BOOL	isIn;	//	0 ∑Œ±◊æ∆øÅE1 ∑Œ±◊¿Œ
		short	lev;
		short	job;
		char	strID[20];
		char	strName[20];
		char	worldName[20];
	};

	struct SERVERStoWORLD_LOG_QUEST
	{
		cMSG_BASE_TYPE_FORWORLD	base;

		BOOL	isStartQuest;	//	0 Complete 1 Get 2 Cancel
		short	questIdx;
		short	lev;
		short	job;
		char	strID[20];
		char	strName[20];
		char	worldName[20];
	};


	//	≈ÅEË∏¶ ¿ß«— ∑Œ±◊ ±‚∑œ
	//////////////////////////////////////////////////////////////////////////


#pragma pack()
}
#endif	//	_classPACKET_WORLDSERVER_H