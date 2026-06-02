#ifndef _classPACKET_WORLDSERVER_H
#define _classPACKET_WORLDSERVER_H

#include "cPACKET_BASE.h"
#include "definePacketData.h"
#include "CGuildDefine.h"
#include "actorDefine.h"
#include "carrotShopDefine.h"
#include "eventDefine.h"
#include "cPACKET_GAMESERVER.h"
//	모�E유저의 데이타를 가지�E있다.

//	게임 서버에서 요청하는 작업을 해준다.
//	게임 서버의 정보, 아이템 생성, 유저들간의 커뮤니티(세이, 길�E 친구), 서버간 이동

//	패킷의 기본구성
//	type,size

#pragma pack(1)

class	cMSG_BASE_TYPE_FORWORLD
{
public:
	WORD	wSize,				//	패킷 사이�E			
			wType;				//	패킷 타입
	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

/////////////////////////////////////////////////////////
//	PACKET TYPE
/////////////////////////////////////////////////////////
//	About In / Out
#define	dSERVERStoWORLD_LOGIN				0x8001//	자신(게임 서�E의 정보를 넘겨주�E연결을 유지한다.
#define	dSERVERStoWORLD_GETAVAS				0x8002//	아바타의 정보들을 보내달�E
#define	dSERVERStoWORLD_CREATEAVA			0x8003//	아바타를 새롭게 만들다.
#define	dSERVERStoWORLD_DELETEAVA			0x8004//	아바타를 삭제한다.
#define	dSERVERStoWORLD_LOADAVA				0x8005//	누군가 들엉怯거니�E. 아바타 정보 불러둬�E
#define	dSERVERStoWORLD_SAVEAVA				0x8006//	저장한다. 데이타... -0-
#define	dSERVERStoWORLD_INFO				0x8007//	엉隙 소켓으로 받을�E알려준다.
#define	dSERVERStoWORLD_LOGOUT				0x8008//	�E종료할 예정이니�E.. 더이�E패킷을 보내�E마셔유 ~~ 
#define	dSERVERStoWORLD_JOIN_COMPLETE		0x8009//	조인 완�E- 정보를 정상적으로 사�E究셀E

#define	dSERVERStoWORLD_USER_LOGIN			0x8011//	이 유저가 자신에게 들엉輩다.
#define	dSERVERStoWORLD_USER_LOGOUT			0x8012//	이 유저가 자신에게서 나갔다.
#define	dSERVERStoWORLD_GET_AVADATA			0x8013//	아바타 데이타를 요청.. 두가�E타입이 존재한다.
#define	dSERVERStoWORLD_COPY_DATA			0x8014//	테스트 서버에 사�E� 캐릭터를 복사한다. 바로 DBC에 넣을것!!

#define	dSERVERStoWORLD_ACCOUNT_INFO		0x8015
#define	dSERVERStoWORLD_CONFIRM_ENTER_FIELD	0x8016//
#define	dSERVERStoWORLD_PLAYER_COUNT		0x8017//
#define	dSERVERStoWORLD_LIFE_SIGN			0x8018//


#define	dWORLDtoSERVERS_LOGIN				0x8701//	guild정보깩�E다 넘겨줬으니 이제 알아서해!!
#define	dWORLDtoSERVERS_GETAVAS				0x8702//	아바타의 정보들을 보낸다.
#define	dWORLDtoSERVERS_CREATEAVA			0x8703//	아바타 만�E결�E
#define	dWORLDtoSERVERS_DELETEAVA			0x8704//	아바타 삭제 결�E
#define	dWORLDtoSERVERS_LOADAVA				0x8705//	아바타 인�E결�E
#define	dWORLDtoSERVERS_USER_LOGIN			0x8711//	인증만 확인하�E
#define	dWORLDtoSERVERS_DIS					0x8712
#define	dWORLDtoSERVERS_GET_AVADATA			0x8713//	데이타만 요청.. - 일반적인 요청�E첫 로그인을 위한 요청
#define	dWORLDtoSERVERS_USER_LOGOUT			0x8714//	정상적으로 로그아�E했다.
#define	dWORLDtoSERVERS_DISCONNECT_SERVER	0x8715//	게임서버가 죽었다.
#define	dWORLDtoSERVERS_RENEW_PREMIUM_ITEM	0x8716

#define	dWORLDtoSERVERS_ACCOUNT_INFO		0x8717
#define	dWORLDtoSERVERS_CONFIRM_ENTER_FIELD	0x8718	//
#define	dWORLDtoSERVERS_SIMPLE_WORK			0x8719	//

#define	dWORLDtoSERVERS_DUEL_MATCH_INFO		0x871a
#define	dWORLDtoSERVERS_LIFE_SIGN			0x871b
#define	dD2W_SIMPLE_WORK					0x871c


//	About Map & User
#define	dSERVERStoWORLD_USER_MOVEMAP		0x8101//	자신이 가지�E있�E않은 맵으로 유저가 이동하려 할때
#define	dSERVERStoWORLD_USER_MOVEMAP_INFO	0x8104//	맵이동에 필요한 정보를 월드에게 알린다.
#define	dSERVERStoWORLD_USER_RECALL			0x8107//	이쪽으로 이동시켜주~~~
#define	dSERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE		0x8108//	OX 퀴�E시작 마을
#define	dSERVERStoWORLD_CALLING				0x810a
#define	dSERVERStoWORLD_BOOST_POWER_BY_EVENT	0x8128
#define	dSERVERStoWORLD_CHECK_USER_MOVEMAP	0x8129	//	자신이 가지�E있�E않은 맵으로 유저가 이동하려 할때
													//	SERVERStoWORLD_USER_MOVEMAP 에 타입만 바꿔서 사�E磯�.
#define	dSERVERStoWORLD_SIMPLE_WORK			0x812a	//
#define	dSERVERStoWORLD_SOUND_OF_LEADERS_BELL	0x812c	
#define	dSERVERStoWORLD_CHANGE_MSG_OF_LEDERS_BELL	0x812d


#define	dWORLDtoSERVERS_USER_MOVEMAP		0x8801
#define	dWORLDtoSERVERS_USER_RECALL			0x8807//	알긋다. 가�E~~~ 
#define	dWORLDtoSERVERS_CALLING				0x8808
#define	dWORLDtoSERVERS_BOOST_POWER_BY_EVENT				0x8809
#define	dWORLDtoSERVERS_CHANGE_DAY			0x880a
#define	dWORLDtoSERVERS_CHECK_USER_MOVEMAP	0x882d	//	필�E이동 체크 결과를 날린다.
													//	SERVERStoWORLD_USER_MOVEMAP 패킷에 타입만 바�E사�E磯�.
#define	dWORLDtoSERVERS_SOUND_OF_LEADERS_BELL	0x882e
#define dWORLDtoSERVERS_SYNC_CHANGEDNAME	0x882f


//	About Admin
#define	dSERVERStoWORLD_ADMIN_LOGIN			0x8102//	Login 정보를 받아서 Admin일경�E시스템 뫄譚엉濤 사�E〈構�..
#define	dSERVERStoWORLD_ALL_MSG				0x8103//	�E�� 모드가 WORLD에게 .. 모�E유저들에게 �E瀕� 메세�E
#define	dSERVERStoWORLD_ISCONNECT_USER		0x8105//	이 계정의 유저가 현�E있냐?
#define	dSERVERStoWORLD_OFFSERVER			0x8109//	월드를 종료한다. (모�E게임서버가 종료될때깩�E기다렸다가..)
#define	dSERVERStoWORLD_APPOINTMENT_QUIZ_EVENT		0x810b//	OX 퀴�E예�E
#define	dSERVERStoWORLD_PRODUCE_OX_QUIZ		0x810c
#define	dSERVERStoWORLD_END_QUIZ_EVENT		0x810d
#define	dSERVERStoWORLD_CANCEL_QUIZ_EVENT		0x810e//	OX 퀴�E�E�
#define	dSERVERStoWORLD_OX_QUIZ_LOSER_REVIVE_BATTLE	0x810f//	OX 퀴�E패자 부활�E
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



#define	dWORLDtoSERVERS_SENDINFOTOADMIN		0x8802//	Admin에게 보내줄 각종 게임 서�E정보 
#define	dWORLDtoSERVERS_ALL_MSG				0x8803//	WORLD가 각 게임 서버에게 �E� 
#define	dWORLDtoSERVERS_DELETE_AVATAR		0x8804//	새로�E캐릭터가 생성되�E이 캐릭터의 save파일을 삭제시킨다.
#define	dWORLDtoSERVERS_ISCONNECT_USER		0x8805//	접속중인 캐릭터 이름�E같이 돌려준다. 맵 이름이�E 번호�E
#define	dWORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT		0x880b//	OX 퀴�E예�E
#define	dWORLDtoSERVERS_PRODUCE_OX_QUIZ		0x880c//	OX 퀴�E제�E
#define	dWORLDtoSERVERS_END_QUIZ_EVENT			0x880d//	OX 퀴�E종�E
#define	dWORLDtoSERVERS_CANCEL_QUIZ_EVENT		0x880e//	OX 퀴�E�E�
#define	dWORLDtoSERVERS_OX_QUIZ_LOSER_REVIVE_BATTLE	0x880f//	OX 퀴�E패자 부활�E
#define dWORLDtoSERVERS_CHAGEGUILDBATTLE	0x8810//	admin툴에서 길드�E예약상황을 바꾼다.
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
#define	dWORLDtoSERVERS_UPDATE_WORD_COMPARE					0x8360		//ADMIN에서 보낸정보를 월드를 거쳐서 게임 서버로 보낸다.
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

#define	dG2W_ASK_GVG_AVATAR_ORG_DATA						0x846f	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청
#define	dW2L_TOSS_ASK_GVG_AVATAR_ORG_DATA					0x8470	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷 토스
#define	dL2W_TOSS_ASK_GVG_AVATAR_ORG_DATA					0x8471	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷 토스
#define	dW2D_ASK_GVG_AVATAR_ORG_DATA						0x8472	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
#define	dD2W_GVG_AVATAR_ORG_DATA							0x8473	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
#define	dW2L_GVG_AVATAR_ORG_DATA							0x8474	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
#define	dL2W_GVG_AVATAR_ORG_DATA							0x8475	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
#define	dW2G_GVG_AVATAR_ORG_DATA							0x8476	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷

#define	dW2G_ASK_GVG_AVATAR_ORG_DATA_RESULT					0x8477
#define	dL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT					0x8478
#define	dW2L_ASK_GVG_AVATAR_ORG_DATA_RESULT					0x8479
#define	dD2W_ASK_GVG_AVATAR_ORG_DATA_RESULT					0x847a

//	About Party
#define	dSERVERStoWORLD_CREATE_PARTY				0x8401
#define	dSERVERStoWORLD_JOIN_PARTY					0x8402
#define	dSERVERStoWORLD_PARTY_WORK					0x8403
#define	dSERVERStoWORLD_RENAME_PARTYNAME			0x8404	//	파티 이름 바꾸�E
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
#define	dWORLDtoSERVERS_RENAME_PARTYNAME			0x8b03	//	파티 이름 바꾸�E- 필요한 게임 서버에게만.
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

//	파티 연합 �E�.
#define	dSERVERStoWORLD_CREATE_UNION_PARTY			0x84a0
#define	dWORLDtoSERVERS_UPDATE_UNION_PARTYINFO		0x84a1
#define	dWORLDtoSERVERS_DISSOLUTION_UNION_PARTY		0x84a2
#define	dWORLDtoSERVERS_CREATE_UNION_PARTY_RESULT	0x84a3

//	아이템 처리
#define	dSERVERStoWORLD_CREATE_ITEM					0x8501	//	필드에 떨굴 아이템 생성
#define	dSERVERStoWORLD_CREATE_SELL_ITEM			0x8502	//	판매할 아이템 생성
#define	dSERVERStoWORLD_ADD_ITEM					0x8503	//	
#define	dSERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL		0x8504	//	유니크 시리�E요구
#define	dSERVERStoWORLD_INCREASE_PACK_ITEM_COUNT	0x8505


#define	dWORLDtoSERVERS_CREATE_ITEM_RESULT		0x8c01	//	필드에 떨굴 아이템 생성 결�E
#define	dWORLDtoSERVERS_CREATE_SELL_ITEM_RESULT	0x8c02	//	판매할 아이템 생성 결�E
#define	dWORLDtoSERVERS_ADD_ITEM_RESULT			0x8c03	//	판매할 아이템 생성 결�E
#define	dWORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL	0x8c04	//	유니크 시리�E요구
#define	dWORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT	0x8c05	

//	서버간 처리 
#define	dWORLDtoSERVERS_CHECKTIME				0x8d01	//	서�E점검 시간
#define dWORLDtoSERVERS_CHEKCPACKET				0x8d02	//	테스트�E.
#define dWORLDtoSERVERS_WORLDNAME				0x8d03	//	각 서버로 월�E이름을 보낸다.

//////////////////////////////////////////////////////////////////////////
//	�E甕� 위한 로그 기록
typedef  enum
{
	dSERVERStoWORLD_LOG_INOUT				=	0x8f11,
	dSERVERStoWORLD_LOG_QUEST,
};
//	�E甕� 위한 로그 기록
//////////////////////////////////////////////////////////////////////////

//	엉珞민�E이벤트 단엉洑 비트 마스크.

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

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	PACKET In / Out

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
	WORD		wIndex;							//맵의 과鼈 인덱스
	WORD		wType;							//맵의 종�E	
	char		strMapName[dMAPNAME_LENGTH];	//자신이 �E�求� 맵이름을 따遝한다.
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
	char		strId[dID_LENGTH];	//	이사람의 데이타를 정상적으로 사�E究셀E
};

struct SERVERStoWORLD_GETAVAS
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD		loginSerial;			//	login 서버에서의 시리�E
	DWORD		worldSerial;			//	world 서버의 유�E시리�E	
	char		strId[dID_LENGTH];
};

struct SERVERStoWORLD_CREATEAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD		loginSerial;
	DWORD		worldSerial;			//	world 서버의 유�E시리�E	
	char		strID[dID_LENGTH];
	char		strName[dNAME_LENGTH];
	char		strIP[dIP_SIZE];
	WORD		wJob;	//	직�E};
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

	WORD						wSocketIndex;		//	0�E1
	WORD						wCount;				//	index로 몰아넣을 유저�E
};

struct	SERVERStoWORLD_USER_LOGIN			//	이 유저가 자신에게 들엉輩다.
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			type;				//	0 - 일반적인 로그인 , 1 - 맵의 이동 
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
	WORD			type;					//	0 - 일반적인 데이타만 요청 , 1 - 로그인을 위한 요청
	DWORD			serial;
	DWORD			dwIndex;
	char			strId[dID_LENGTH];
	WORD			badtime;

#ifdef	_OGP_SERVICE
	char			strToken[dTOKEN_LENGTH];
#endif
};

struct SERVERStoWORLD_COPY_DATA				//	테스트 서버에 사�E� 캐릭터를 복사한다. 바로 DBC에 넣을것!!
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

	DWORD		serial;						//	게임서버가 보낸 유저의 시리�E- 그�E� 돌려주�E된다.
	int			iIndex;						//	따遝된 시리얼을 돌려준다. 만�E-1이�E따遝 실패로 유�E자른다.
	WORD		wFieldidx;
	WORD		bf10IFSerial	:	10;
	WORD		bf4IFFloor		:	4;
	WORD		wPartyIndex;
	WORD		badtime;
	WORD		oper;						//	웝滂자인가??
	WORD		isOperInAccount;			//	계정이 웝滂자로 설정되�E있나?
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
	char			strCause[128];		//	죽이는 이유 -_x
};

struct WORLDtoSERVERS_GET_AVADATA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD							type;					//	0 - 일반적인 데이타만 요청 , 1 - 로그인을 위한 요청
	DWORD							serial;
	CPlayerSaveDataForServerDefine	aInfo;
	CUpkeepPlayerData				upkeepPlayerData;		//	필드를 이동해도 유지해야 하는 데이터
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
//	월드에서 게임서버로 �E�
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
	eSWWG_CONFIRM_USE_SOUND_OF_LEADERS_BELL,	//	인도자의 종소리 사�E가능
	eSWWG_CHANGE_TRY_GUILD_DUNGEN_COUNT	,
	eSWWG_SMS_ALARM_ERROR	,
	eSWWG_UPDATE_GUILD_BATTLE_SCORE,
	eSWWG_RESET_GUILD_SIEGEWARFARE_INFO,	//	길�E공성�E정보 초기화
	eSWWG_RESET_1DAY_GUILD_INFO		,
	eSWWG_RESET_1WEEK_GUILD_INFO	,

//	게임 서버에서 월드로 �E�
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
	eSWGW_INCREASE_BONUS_GUILD_POINT,	//	보너스 길�E포인트 증가
	eSWGW_RECOVER_BONUS_GUILD_POINT_PENELTY	,
	eSWGW_NAME_STONE_INDEX,
	eSWGW_ACTIVE_GUILD_MASTER_GRACE,
	eSWGW_INCREASE_TRY_GUILD_DUNGEON_COUNT	,
	eSWGW_CHANGE_SYSTEM_TIME	,
	eSWGW_CHANGE_GUILD_PET_EXP			,		// 길드�E뭔㎈�E	
	eSWGW_RESET_GUILD_BATTLE_SCHEDULE	,		// 길드�E스케�E초기화 

	eSWAW_BEGIN		=	0x2000,	//	엉珞민�E>월�E	
	eSWAW_CHANGE_GUILD_MARK_TWINKLE_STATUS,
	eSWAW_CHANGE_GUILD_HALL_LEVEL,
	eSWAW_CHANGE_GUILD_VILLAGE_INFO,
	eSWAW_CHANGE_GUILD_MASTER_GRACE,
	eSWAW_CHANGE_GUILD_INFO_ETC_BY_ADMINTOOL,
	eSWAW_RESET_SOUND_OF_LEDERS_BELL_COOLTIME	,	//	인도자의 종소리 초기화
	eSWAW_ASK_SOUND_OF_LEDERS_BELL_EVENT_INFO,
	eSWAW_CHANGE_GUILD_POINT_BATTLE_COUNT	,
	eSWAW_ASK_LOTTO_EVENT_DATA,
	eSWAW_ASK_EVENT_TIME_DATA,
	eSWAW_UPDATE_CARROT_SHOP_INFO,
	eSWAW_CHANGE_TODAY_GUILD_BATTLE_COUNT,
	eSWAW_CHANGE_THIS_WEEK_GUILD_BATTLE_COUNT,

	eSWLW_BEGIN		=	0x3000,						//	로그인 서버에서 월드로
	eSWLW_SMS_ALARM_ERROR	,
	eSWLW_UPDATE_CARROT_SHOP_INFO	,

	eSWWA_BEGIN		=	0x4000,		//	월�E>엉珞민�E	
	eSWWA_CAN_NOT_FOUND_LOGIN_SERVER,

	eSWWL_BEGIN		=	0x5000,						//	월�E서버에서 로그인 서버로
	eSWWL_UPDATE_CARROT_SHOP_INFO	,
	
	eSWDW_BEGIN		=	0x6000,		//	DBC->월�E
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

struct WORLDtoSERVERS_RENEW_PREMIUM_ITEM	//	프리미�E아이템 보�E정보
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
	DWORD			loginSerial;			//	login 서버에서의 시리�E	
	DWORD			worldSerial;			//	world 서버의 유�E시리�E	
	cAVATAR_INFO	aAvatarList[dMAX_AVATAR_COUNT];
};

struct WORLDtoSERVERS_CREATEAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strID[dID_LENGTH];
	DWORD			loginSerial;
	DWORD			worldSerial;			//	world 서버의 유�E시리�E	
	short			wResult;		//	결�E	
	WORD			wIndex;			//	뫈礫째 아바타에 추가되었는�E..
	cAVATAR_INFO	aCharacter;		//	데이타도 보내준다. 
};

struct WORLDtoSERVERS_DELETEAVA
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD			loginSerial;
	char			strID[dID_LENGTH];
	short			wResult;		//	결�E
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
	int		iServerIndex;	//	-1이�E월�E	
	int		iGameServerCount;	//	워드에서만 쓰는 값. -1이�E월드가 아닌곳에서 보낸 값으로 인식하�E무시해 버린다.

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

struct	SERVERStoWORLD_USER_LOGOUT			//	이 유저가 자신에게서 나갔다.
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		type;						//	 0 -logout , 1 - 다른 서버로 이동
	int			iSerialInServer;
	char		strID[dID_LENGTH];
	char		strName[dNAME_LENGTH];
	char		strIP[dIP_SIZE];
};

//┃	PACKET In / Out
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	PACKET Map & User

struct	SERVERStoWORLD_USER_MOVEMAP
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int			iSerialInServer;	//	해�E게임 서버에서의 시리�E
	int			iWorldSerial;	//	월드에서 �E�求� 유�E시리�E- 게임서버에 로그인을 성공하�E월드가 보내준다. 
	WORD		wFieldSerial;	//	필드의 과鼈 시리�E	
	WORD		wIsPremiumMember;
	char		strMapName[dMAPNAME_LENGTH];
	WORD		wGateIndex;
	WORD		wXPos,wYPos;
	WORD		wCheckResult;	//	패킷 타입이 dSERVERStoWORLD_CHECK_USER_MOVEMAP인 경�E이 값에 결과를 담아
								//	패킷 타입을 dWORLDtoSERVERS_CHECK_USER_MOVEMAP로 바�E게임 서버에 �E徘磯�.
};

struct SERVERStoWORLD_USER_MOVEMAP_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	DWORD						dwSerial;				//	해�E클라이언트의 월�E시리�E
	CUpkeepPlayerData			upkeepPlayerData;		//	필드를 이동해도 유지해야 하는 데이터
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
	WORD	wWork;	//	0 이벤트 시작,1 �E� �EE이벤트 종�E2 리�E�EE이벤트 종�E3 메시�E변�E
	WORD	wBeginYear,wBeginMonth,wBeginDay,wBeginHour,wBeginMinute;	//	시작한 시간(엉珞민툴에서 만 씀. 게임 서버에는 쓰레�E값)
	WORD	wCoolTimeMinute;	//	남은 쿨타임(엉珞민툴에서 만 씀. 게임 서버에는 쓰레�E값)

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

#define	dMOVEMAP_RESULT_SUCCESS			0x01	//	이동가능
#define	dMOVEMAP_RESULT_NOTFIND			0x02	//	존재하�E않거나.. 현�E접속 불가능 지역입니다.
#define	dMOVEMAP_RESULT_FULL			0x03	//	더이�E입장이 불가능합니다.
#define	dMOVEMAP_RESULT_USEDNAME		0x04	//	같은 이름의 아이디가 있다 -_-;
#define	dMOVEMAP_RESULT_UNKNOWN_ERROR	0x05	//	알�E없는 문제 발생.. -_-a
#define	dMOVEMAP_RESULT_PREMIUM_ZONE	0x06	//	프리미�E존인데 당신은 프리미�E사�E微� 아냐!!
#define	dMOVEMAP_RESULT_ONE_WAY_PORTAL	0x07	//	일방�EE포탈이다.
#define	dMOVEMAP_RESULT_TEST_PORTAL		0x08	//	테스트 포탈입니다.
#define	dMOVEMAP_RESULT_MOVE_TO_IF		0x09	//	인스턴스 던젼으로 입�E
#define	dMOVEMAP_RESULT_CAN_NOT_MOVE_FIELD_STATUS	0x0a	//	필�E이동을 할 �E없는 상태이다.

	//	기타 필요할지도 모르는 결�E
#define	dMOVEMAP_RESULT_NOTGUILD		0x11	//	특정길드만이 입�E가능합니다.


struct WORLDtoSERVERS_USER_MOVEMAP
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD		Result;
	int			iSerialInServer;
	WORD		wUniqueFieldSerial;	//	필드의 과鼈 시리�E	
	char		strServerAddr[dIP_SIZE];
	char		strMapName[dMAPNAME_LENGTH];
	WORD		wGateIndex;
	WORD		wXPos,wYPos;
};

//┃	PACKET Map & User
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━



//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	PACKET Community

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

	WORD				wIsDailyRanking;	//	0 : 주간 랭킹,1 : 일간 랭킹
	WORD				wWeeklyRankerCount,wTrialGameRankerCount;	//	주간 랭커 �E예선�E랭커 �E	
	cDuelRankingInfo	aList[200];	//	예선�E랭킹 + 주간 랭킹
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
	eBPBE_TM_GOLD,	//	보물 지도 이벤트를 인한 골�E4�E	
	eBPBE_TM_EXP,	//	보물 지도 이벤트로 인한 경험치 2�E
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
	eROFS_CAN_NOT_FIND_TARGET		,	//	타겟을 못찾겠는디유.. -o-;;
	eROFS_TARGET_DENIED_SAY_MESSAGE	,	//	수신 거부 상태
	eROFS_TARGET_IS_OPERATOR		,	//	�E瓚� 웝滂자다
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

#define	dFIND_RESULT_NOTFIND		0x00	//	접속하�E않았습니다.
#define	dFIND_RESULT_SUCCESS		0x01
#define	dFIND_NOT_EXIST_COUPLE_RING	0x02	//	접속하�E않았습니다.

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

//┃	PACKET Community
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	PACKET Guild

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
	char							strMasterName[dNAME_LENGTH];	//	길�E마스터 이름
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
	char							strMasterName[dNAME_LENGTH];	//	길�E마스터 이름
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

struct G2W_ASK_GVG_AVATAR_ORG_DATA						//	0x846f	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wOrgServer;
	WORD	wNotUseUnderBarName;	//	이름이 길�E밑줄(_)을 �E붙이�E바로 알파벳을 붙인 이름이다.
	char	strName[dNAME_LENGTH];	//	
	char	strId[dID_LENGTH];	//	
};

struct W2L_TOSS_ASK_GVG_AVATAR_ORG_DATA					//	0x8470	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷 토스
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wOrgServer;
	WORD	wNotUseUnderBarName;	//	이름이 길�E밑줄(_)을 �E붙이�E바로 알파벳을 붙인 이름이다.
	char	strName[dNAME_LENGTH];	//	
	char	strId[dID_LENGTH];	//	
};

struct L2W_TOSS_ASK_GVG_AVATAR_ORG_DATA					//	0x8471	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷 토스
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wOrgServer;
	WORD	wNotUseUnderBarName;	//	이름이 길�E밑줄(_)을 �E붙이�E바로 알파벳을 붙인 이름이다.
	char	strName[dNAME_LENGTH];	//	
	char	strId[dID_LENGTH];	//	
};

struct W2D_ASK_GVG_AVATAR_ORG_DATA						//	0x8472	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
{
	cMSG_BASE_TYPE_FORDB		base;

	WORD	wNotUseUnderBarName;	//	이름이 길�E밑줄(_)을 �E붙이�E바로 알파벳을 붙인 이름이다.
	char	strId[dID_LENGTH];	//	
	char	strName[dNAME_LENGTH];	//	
};

struct D2W_GVG_AVATAR_ORG_DATA							//	0x8473	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strName[dNAME_LENGTH];	//	
	CPlayerSaveDataForServerDefine	avatarData;
};

struct W2L_GVG_AVATAR_ORG_DATA							//	0x8474	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strName[dNAME_LENGTH];	//	
	CPlayerSaveDataForServerDefine	avatarData;
};

struct L2W_GVG_AVATAR_ORG_DATA							//	0x8475	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
{
	cMSG_BASE_TYPE_FORDB		base;

	char	strName[dNAME_LENGTH];	//	
	CPlayerSaveDataForServerDefine	avatarData;
};

struct W2G_GVG_AVATAR_ORG_DATA							//	0x8476	//	G.V.G 서버에서 본서�E본래 캐릭터 데이터 요청 패킷
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

struct	SERVERStoWORLD_USER_DELETEGUILD	//	길�E마스터 & 현�E접속중인 길드�E& 모�E길드원의 정보 갱신
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
	eRBGB_ALREADY_EXIST_SCHEDULE	,	//	이미 신청 했잖아!!
	eRBGB_ALREADY_BOOKED			,	//	이미 예약되�E있다.
	eRBGB_OVER_BATTLE_FOR_THIS_WEEK	,	//	이번주 �E醮� 끝났다.

	eRBGB_NOT_EXIST_GUILD			,	//	존재하�E않는 길드다.
	eRBGB_INCORRECT_DATA			,	//	데이터가 잘�E되었다.
	eRBGB_FULL_REQUEST_IN_THAT_TIME	,	//	그 시간에 신청이 종료되었다.

	eRBGB_OVER_BATTLE_FOR_THIS_DAY	,	//	오늘 �E醮� 끝났다.
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
	eRCGB_SUCCESS									,	//	성�E	
	eRCGB_VALID										,	//	예약된거 없다.
	eRCGB_CAN_NOT_CANCEL_BATTLE_BY_ALREADY_ENGAGED	,	//	�E老� �E없다.
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
//	길�E하나의 정보 업데이트
struct	WORLDtoSERVERS_UPDATE_GUILD_INFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wGuildSerial;
	WORD						wIsRequireResetMemberInfo;
	CGuildAdvanceInfo			info;	//dGUILD_COUNT_FORPACKET
};

//
//	길드�E정보
struct	WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE
{
	cMSG_BASE_TYPE_FORWORLD		base;

	CGuildBattleSchedule		todaySchedule;
};

//
//	길드�E결�E
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
//	존재하�E않는 길드다.
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
#define	dCREATEGUILD_RESULT_FULL		0x02		//	더이�E만들�E없습니다.
#define	dCREATEGUILD_RESULT_SAMENAME	0x03		//	같은 이름의 길드가 존재한다.
#define	dCREATEGUILD_RESULT_HAVEGUILD	0x04		//	마스터가 다른 길드가 있잖아 -_-a
#define	dCREATEGUILD_RESULT_FAIL		0x05		//	다른 이유로 실패

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
#define	dJOINGUILD_RESULT_NOTFIND		0x02	//	그런 길드는 없는데 .. ;
#define	dJOINGUILD_RESULT_FULL			0x03	//	푸 ~~ �E
#define	dJOINGUILD_RESULT_JOINED		0x04	//	이미 다른 길드에 조인되�E있다.
#define	dJOINGUILD_RESULT_NEEDRANK		0x05
#define	dJOINGUILD_RESULT_FAIL			0x06	//	뭘깩~~

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
#define	dEXITGUILD_RESULT_NOTFIND		0x02	//	그런 길드는 없는데 .. ;
#define	dEXITGUILD_RESULT_NOTJOIN		0x03	//	넌 길�E없스 ~~
#define	dEXITGUILD_RESULT_NOTACOUNT		0x04	//	정보가 일치하�E않습니다.
#define	dEXITGUILD_RESULT_FAIL			0x05	//	뭘깩~~
#define dEXITGUILD_RESULT_NEEDRANK		0x06	//	뭘깩~~
#define dEXITGUILD_RESULT_FAILED_BY_TARGET_IS_GUILD_MASTER	0x07	//	뭘깩~~
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
#define	dDELETEGUILD_RESULT_FAIL		0x02	//	뭔 이유가 있을�E-_-a

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
#define	dCHANGERANK_RESULT_NOTFIND		0x02	//	그런 길드는 없는데 .. ;
#define	dCHANGERANK_RESULT_NOTACOUNT	0x03	//	정보가 일치하�E않습니다.
#define	dCHANGERANK_RESULT_FAIL			0x04	//	뭘깩~~

struct WORLDtoSERVERS_USER_CHANGERANK			//	성공했을 경�E�E서버에게...
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
	eRIGSL_OK,	//	성�E	
	eRIGSL_SKILL_POINT_LACKS,	//	스킬포인트가 부족하다.
	eRIGSL_LEVEL_SKILL_IS_MAX,	//	스킬레벨이 맥스다.
};

//
//	길�E레벨 증가 실패. �E瓚� 해�E길드의 길마(
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
	WORD						awBattleGuild[2];	//	�EE� 두 길�E	
	WORD						wGuildSerial;
};

/////////////////////////////////////////////////////////////////////////////
//	모�E서버에게 보내�E길�E정보�E.
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


//┃	PACKET Guild
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	PACKET Party

struct	SERVERStoWORLD_CREATE_UNION_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD						wAskerPartySerial;
	WORD						wReplayerPartySerial;
};

struct	SERVERStoWORLD_CREATE_PARTY
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wFieldSerial;		//	유니크 필�E시리�E	
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

	WORD	wPartySerial;	//	파티 시리�E	
	WORD	wFieldSerial;	//	유니크 필�E시리�E	
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

	WORD	wPartySerial;			//	파티 시리�E-> 파티 이름으로 교체할 것 !!
	DWORD	dwUniqueSerial;
	WORD	wWork;

	char	strName[dNAME_LENGTH];	//	탈퇴할 사람의 이름 
};

struct	SERVERStoWORLD_UNION_PARTY_WORK					//연합 파티에 �EE메세지를 월드로 �E僿磯�.
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD						wPartySerial;			//	파티 시리�E-> 파티 이름으로 교체할 것 !!
	WORD						wUnionPartySerial;
	WORD						wWork;
	
	char						strName[dNAME_LENGTH];	//	탈퇴할 사람의 이름 
};

struct	SERVERStoWORLD_DISCONNECT_PARTY_MEMBER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;			//	파티 시리�E-> 파티 이름으로 교체할 것 !!

	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];	//	탈퇴할 사람의 이름 
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
	eGMJPA_DENY, // 거절
	eGMJPA_OK, // 오케이
	eGMJPA_HAVE_NO_TARGET, //	 �E瓚� 없다.
	eGMJPA_TARGET_IS_IN_PARTY,	// �E瓚� 파티가 있다.
	eGMJPA_PARTY_IS_FULL,		//	 파티가 풀이에�E	
	eGMJPA_FAILED,			//	 실패?
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

	WORD	wPartySerial;				//	파티 시리�E
	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dPARTYNAME_LENGTH];	//	파티 이름
};

struct	SERVERStoWORLD_ASK_PARTY_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD	dwSerial;					//	받을 유저의 시리�E	
	WORD	isNext;						//	0 - �E페이�E, 1 - 다음 페이�E	
	WORD	wPartyIndex;				//	받을 기준이 되는 파티의 인덱스
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
	DWORD	bf3PartyObject			:	3;	//파티 목�E	
	DWORD	bf1IsOpenParty			:	1;	//멤버를 계속 모집한다.

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
	DWORD	bf3PartyObject			:	3;	//파티 목�E	
	DWORD	bf1IsOpenParty			:	1;	//멤버를 계속 모집한다.

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
	int		bf1IsLeave				:	1;	//	그 필드에서 떠나는거냐?
	WORD	bf10InstanceField		:	10;
	WORD	bf4InstanceFieldFloor	:	4;

	WORD	wDestFieldSerial;	//	목적�E};
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

	int		iDuelVP;	//	결�E승점

	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];
};

struct	WORLDtoSERVERS_PARTY_WORK
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;				//	길�E인덱스 - 자신의 길드인�E.
	DWORD	dwUniqueSerial;
	WORD	wWork;
	char	strName[dNAME_LENGTH];	//	삭제될 유저의 아이�E
};

struct	WORLDtoSERVERS_DISCONNECT_PARTY_MEMBER
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPartySerial;				//	길�E인덱스 - 자신의 길드인�E.
	WORD	wIsBanish;
	char	strPartyName[dPARTYNAME_LENGTH];
	char	strName[dNAME_LENGTH];	//	삭제될 유저의 아이�E
};

struct	WORLDtoSERVERS_UPDATE_PARTYINFO
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wIsCreate;		//	이제막 만들엉�E파티다.
	
	CSimplePartyInfo			partyInfo;
};

struct	WORLDtoSERVERS_UPDATE_UNIONPARTYINFO
{
	cMSG_BASE_TYPE_FORWORLD		base;
	
	WORD						wIsCreate;					//	이제막 만들엉�E파티다.
	char						strName[dNAME_LENGTH];		//	연합 파티 이름.
	DWORD						m_dwUniqueSerial;			//	유니크 시리�E
	
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
	WORD	wPartySerial;				//	파티 시리�E	
	char	strName[dPARTYNAME_LENGTH];	//	파티 이름
	char	strChangeName[dPARTYNAME_LENGTH];	//	파티 이름
};

struct	WORLDtoSERVERS_PARTY_LIST
{
	cMSG_BASE_TYPE_FORWORLD		base;

	DWORD						dwSerial;	//	받을 유저의 시리�E	
	WORD						wPartySIndex,wPartyEIndex;	//	받을 기준이 되는 파티의 인덱스
	WORD						wPartyCount;				//	넘엉怯 갯�E
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
	int		bf1IsLeave			:	1;	//	그 필드에서 떠나는거냐?
	WORD	bf10IFSerial		:	10;
	WORD	bf4IFFloor			:	4;

	WORD	wDestFieldSerial;	//	목적�E};
};

//┃	PACKET Party
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃아이템 처리

//	Game server -> World server
//	필드에 떨엉磊리는 아이템 생성
struct SERVERStoWORLD_CREATE_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wCount;//아이템 �E	
	WORD						wFieldSerial;//필�E시리�E	
	cCREATE_ITEM_INFO			aCreateItem[dDROPPING_ITEM_COUNT+1];//생성할 아이템
};	//	struct SERVERStoWORLD_CREATE_ITEM

//	World server -> Game server
//	필드에 떨엉磊리는 아이템 생성 결과를 받음
struct WORLDtoSERVERS_CREATE_ITEM_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wCount;//아이템 �E	
	WORD						wFieldSerial;//필�E시리�E	
	cRESULT_CREATE_ITEM_INFO	aCreateItemResult[dDROPPING_ITEM_COUNT+1];
};	//	struct WORLDtoSERVERS_CREATE_ITEM_RESULT

//	Game server -> World server
//	상점에서 판매하는 아이템 생성
struct SERVERStoWORLD_CREATE_SELL_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int							iSerialInServer;
	DWORD						dwPrice;	//	가격
	char						strName[dNAME_LENGTH];
	CItemDefine					item;	//	판매할 아이템
	WORD						wCount;
	WORD						wWhere;
	WORD						wIsDuelPointShop;
	WORD						wIsTokenShop;
};	//	struct SERVERStoWORLD_CREATE_SELL_ITEM

//	World server -> Game server
//	상점에서 판매하는 아이템 생성 결�E
struct WORLDtoSERVERS_CREATE_SELL_ITEM_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char						strName[dNAME_LENGTH];
	int							iSerialInServer;
	DWORD						dwPrice;	//	가격
	DWORD						dwSerial;
	WORD						wCount,wWhere;
	CItemDefine					item;	//	판매할 아이템
	WORD						wIsDuelPointShop;
	WORD						wIsTokenShop;
};	//	struct SERVERStoWORLD_CREATE_SELL_ITEM

//	Game server -> World server
//	아이템 생성
struct SERVERStoWORLD_ADD_ITEM
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wClient;//	과甄 -o-
	CItemDefine					item;	//	판매할 아이템
};	//	struct SERVERStoWORLD_ADD_ITEM

//	World server -> Game server
//	아이템 생성
struct WORLDtoSERVERS_ADD_ITEM_RESULT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD						wClient;//	과甄 -o-
	CItemDefine					item;	//	판매할 아이템
};	//	struct WORLDtoSERVERS_ADD_ITEM

//	Game server -> World server
//	아이템의 유니크 시리얼을 요구
struct SERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int							iSerialInServer;//	과甄 -o-
};	//	struct SERVERStoWORLD_ADD_ITEM

//	World server -> Game server
//	아이템의 유니크 시리얼을 요구
struct WORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL
{
	cMSG_BASE_TYPE_FORWORLD		base;

	int							iSerialInServer;//	과甄 -o-
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
	WORD	wStatus;	//	0 시작,1 도중,2 마지막

	cCarrotShopCategoryInfo2	categoryInfo;	//	데이터 사이짊瞼 과諾줄이�E때문에 위치 변경하�E안됨
	cCarrotShopCategoryInfo1	data;
};

struct WORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT
{
	cMSG_BASE_TYPE_FORWORLD		base;

	WORD	wPackSerial,wItemIndex,wCount;
};

//┃아이템 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃	PACKET FOR ADMIN

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
	WORD	wStatus;	//	0 시작,1 진�E2 종�E
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

	WORD		wGameSrvCount;							//	접속된 게임서�E�E.. 
	DWORD		wUserCount,wMaxUserCount;				//	동접�E
};

struct	SERVERStoWORLD_ALL_MSG
{
	cMSG_BASE_TYPE_FORWORLD		base;

	char			strMessage[dCHAT_LENGTH];	//	메시�E
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
{	// 마지막 수정일 : 09.10.09
	cMSG_BASE_TYPE_FORWORLD		base;

	char	strWordOrString[256];
	
	DWORD	bf16EventKind : 16;		// 16
	DWORD	bf16BuffSerial : 16;	//32
	DWORD	dwTimeInfo;
	WORD	wBeginTimeYear;				//시작 시간...
	WORD	wBeginTimeMonth;
	WORD	wBeginTimeDay;
	WORD	wBeginTimeHour;
	WORD	wBeginTimeMinite;
	WORD	wEndTimeYear;				//종�E시간...
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
	char	strNoticeMsg[98];	//	%d분후 서버가 종료합니다.앞에 올말..
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

	WORD	wIsForced;	//	강제로 �i아낸다.
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

	char			strMessage[dCHAT_LENGTH];	//	메시�E
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
		WORD						wBeginTimeYear;				//시작 시간...
		WORD						wBeginTimeMonth;
		WORD						wBeginTimeDay;
		WORD						wBeginTimeHour;
		WORD						wBeginTimeMinite;
		WORD						wEndTimeYear;				//종�E시간...
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

		WORD	isTime;			//	뫈礫째 시간인가.
		WORD	wField;			//	뫈礫째 필드인가..(10개중 한개)

		char	strGuildName1[dGUILD_NAME_LENGTH];
		char	strGuildName2[dGUILD_NAME_LENGTH];
	};

	struct	WORLDtoSERVERS_BANISH_USER
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD	wIsForced;	//	강제로 �i아낸다.
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
	//┃	PACKET  FOR ADMIN
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃	PACKET 

	struct	WORLDtoSERVERS_CHECKTIME
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		WORD		wType;	//	1 - 닫아�E 0 - 다시 열엉�E
	};

	//┃	PACKET 
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//┃	PACKET 

	struct WORLDtoSERVERS_PRIZE_PLAYER_RESULT
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char						strID[20];//	ID뫄棅로 유�E확인
		char						strName[20];//	케릭터뫄棅로 유�E확인
		WORD						wIsPrizePlayer;
	};	//	struct WORLDtoSERVERS_ADD_ITEM

	//	Game server -> World server
	//	아이템의 유니크 시리얼을 요구
	struct SERVERStoWORLD_ASK_PRIZE_PLAYER
	{
		cMSG_BASE_TYPE_FORWORLD		base;

		char						strID[20];//	ID뫄棅로 유�E확인
		char						strName[20];//	케릭터뫄棅로 유�E확인
	};	//	struct SERVERStoWORLD_ADD_ITEM

	//struct
	//{
	//	cMSG_BASE_TYPE_FORWORLD		base;
	//};

	//┃	PACKET 
	//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//////////////////////////////////////////////////////////////////////////
	//	�E甕� 위한 로그 기록
	struct SERVERStoWORLD_LOG_INOUT
	{
		cMSG_BASE_TYPE_FORWORLD	base;

		BOOL	isIn;	//	0 로그아�E1 로그인
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


	//	�E甕� 위한 로그 기록
	//////////////////////////////////////////////////////////////////////////


#pragma pack()
#endif	//	_classPACKET_WORLDSERVER_H