#ifndef __CPACKET_GAMESERVER_H
#define __CPACKET_GAMESERVER_H

#include	"definePacketData.h"
#include	"cPACKET_BASE.h"
#include	"cPACKET_LOGINSERVER.h"
#include	"cPACKET_DBSERVER.H"
#include	"CGuildDefine.h"
#include	"cPetDefine.h"
#include	"CPitchmanShopDefine.h"
#include	"doorDefine.h"
#include	"arcaDefine.h"
#include	"carrotShopDefine.h"
#include	"bannerDefine.h"
#include	"eventDefine.h"

#define	dDEFAULT_TARGET_COUNT					32
#define dMAX_WORLD_VALUE_MAX					128
#define dMAX_PARTY_NAME_LENGTH					22






#define dCG_CONNECT_GAME						0x1021
#define dCG_JOIN_GAME							0x1022
#define dCG_MOVE								0x1023
#define dCG_REPORT_CURRENT_POS					0x10b1
#define dCG_STOP								0x1024
#define dCG_ACTION_TO_OBJECT					0x10b3
#define dCG_ACTION_TO_ACTOR						0x1025
#define dCG_ACTION_TO_GROUND					0x1026
#define dCG_ACTION_QUICK						0x1038
#define dCG_SET_MOVE_ABILITY					0x1027
#define dCG_TRANSFORMATION						0x103a
#define dCG_ASK_INFO_ACTOR_LIST					0x1028
#define dCG_OBSCURITY_ACTOR_LIST				0x1099
#define	dCG_MISPLACED_ACTOR_LIST				0x1045
#define	dCG_ASK_PLAYER_INFO						0x10a1
#define	dCG_CHECK_PLAYER_CHECK_SUM				0x10a2
#define	dCG_ASK_ACTOR_ENCHANT_INFO				0x10be
#define dCG_SETTING_SEASON_VARIABLE				0x12f3
#define dCG_SETTING_SEASON_VARIABLE_SUCCESS		0x12f3

#define dCG_ENGAGE_BATTLE						0x102a
#define dCG_STOP_BATTLE							0x102b
#define dCG_INCREASE_STATE						0x1036
#define dCG_INCREASE_ABILITY_LEVEL				0x1037
#define dCG_ASK_ACTOR_INFO						0x1039
#define dCG_TOGGLE_SITDOWN						0x103c
#define dCG_MOVE_FIELD							0x103b
#define dCG_WARP_TO_NEAR_VILLAGE				0x109a	
#define	dCG_ENTER_PORTAL						0x109b
#define dCG_SET_REACTION_STATUS					0x1043
#define dCG_TURN_ON_PASSIVE_SKILL				0x1044
#define	dCG_RETURN_TO_VILLAGE					0x104f
#define	dCG_EXIT_GAME							0x1065
#define	dCG_HACKING_LOG							0x10ab
#define	dCG_GG_AUTH_DATA						0x10ac
#define	dCG_HS_GUID_REPLY						0x10c0
#define	dCG_HS_ACK_MSG_REPLY					0x10c1

#define dCG_WARP_FIELD_BY_GATE_GLOVE			0x10bb
#define dCG_WARP_FIELD_BY_NET_CAFE				0x10c6


#define	dCG_PICK_ITEM							0x102c
#define	dCG_EQUIP_ITEM							0x102d
#define	dCG_RELOAD_BELT_ITEM					0x102e
#define	dCG_USE_ITEM							0x102f
#define	dCG_USE_ITEM_TO_ITEM					0x1066
#define	dCG_USE_BELT_ITEM						0x1030
#define	dCG_ASK_ITEM_DATA						0x1031
#define	dCG_ASK_INVENTORY_DATA					0x1032
#define	dCG_ASK_EQUIPMENT_DATA					0x1033
#define	dCG_DROP_ITEM							0x1034
#define	dCG_CHANGE_ITEM_PLACE					0x1035
#define	dCG_STRIP_EQUIPMENT						0x104e
#define	dCG_DESTROY_ITEM						0x1082
#define	dCG_REPAIR_ITEM							0x1098
#define	dCG_ASK_ITEM_PACK_NAME					0x10ad
#define	dCG_REMOVE_PREFIX						0x10b7
#define	dCG_JUDGE_TO_ITEM						0x10b8
#define	dCG_MERGE_ITEM							0x10b9
#define	dCG_USE_SOUND_OF_LEADERS_BELL			0x10c7


#define	dCG_REQUEST_TRADE						0x1050
#define	dCG_TRADE_REPLY							0x1051
#define	dCG_CANCEL_TRADE						0x1052
#define	dCG_ADD_TRADE_ITEM						0x1053
#define	dCG_REMOVE_TRADE_ITEM					0x1054
#define	dCG_SET_TRADE_GOLD						0x1055
#define	dCG_READY_TRADE							0x1056
#define	dCG_PERMIT_TRADE						0x1057
#define	dCG_CHANGE_TRADE_ITEM_DATA				0x1058


#define	dCG_STORE_ITEM_TO_THE_BANK				0x105d
#define	dCG_STORE_GOLD_TO_THE_BANK				0x105e
#define	dCG_WITHDRAW_ITEM_FROM_THE_BANK			0x105f
#define	dCG_WITHDRAW_GOLD_FROM_THE_BANK			0x1060
#define	dCG_MOVE_BANK_ITEM						0x1061
#define	dCG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	0x1062
#define	dCG_FINISH_BANK_TRANSACTION				0x1063
#define	dCG_CANCEL_BANK_TRANSACTION				0x1064


#define	dCG_CART_OPEN							0x107b
#define	dCG_WITHDRAW_ITEM_FROM_THE_CART			0x107c
#define	dCG_CLOSE_CART							0x107d
#define	dCG_DESTROY_CART_ITEM					0x1081
#define	dCG_OPEN_CARROT_SHOP					0x10ba


#define	dCG_ASK_OPEN_THE_PITCHMAN_SHOP			0x108d
#define	dCG_ADD_PITCHMAN_SHOP_ITEM				0x108f
#define	dCG_REMOVE_PITCHMAN_SHOP_ITEM			0x1090
#define	dCG_CHANGE_PITCHMAN_SHOP_INFO			0x1091
#define	dCG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		0x1094
#define	dCG_CLOSE_PITCHMAN_SHOP					0x1095
#define	dCG_BUY_PITCHMAN_SHOP_ITEM				0x1092
#define	dCG_ASK_PITCHMAN_SHOP_INFO				0x1093
#define	dCG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST	0x1097


#define	dCG_SELECT_NPC							0x103f
#define	dCG_SELECT_SPEECH						0x1040
#define	dCG_BUY_ITEM							0x1041
#define	dCG_SELL_ITEM							0x1042
#define	dCG_CANCEL_QUEST						0x106a
#define	dCG_BUY_CARROT_SHOP_ITEM				0x10a5
#define	dCG_CHANGE_DOOR_STATUS					0x10aa
#define	dCG_TOUCH_EVENT_AREA					0x10ae
#define	dCG_ARCA_WORK							0x10af
#define	dCG_ASK_ARCA_INFO						0x10b0
#define	dCG_DISARM_FIELD_TRAP					0x10b2
#define	dCG_ASK_DOOR_INFO						0x10b4
#define	dCG_PUT_BANNER							0x10bc
#define	dCG_ASK_BANNER_TEXT						0x10bd
#define	dCG_SEARCH_RELAY_STATION_ITEM			0x10c4
#define	dCG_SUMMON_EVENT_MONSTER				0x10c8



#define dCG_CHAT								0x1029

#define	dCG_ASK_PARTY_LIST						0x1046
#define	dCG_ASK_PARTY_INFO						0x1048
#define	dCG_ASK_JOIN_PARTY						0x1049
#define	dCG_ASK_JOIN_PARTY_REPLY				0x104a
#define	dCG_LEAVE_PARTY							0x104b
#define	dCG_PARTY_LEADER_WORK					0x104c
#define	dCG_CHANGE_PARTY_NAME					0x104d
#define	dCG_ACCUSE								0x105a
#define	dCG_OPERATOR_COMMAND					0x105b
#define	dCG_FIND_USER							0x105c
#define	dCG_REQUEST_ADD_FRIEND					0x1067
#define	dCG_REPLY_FOR_ADD_FRIEND				0x1068
#define	dCG_CHANGE_COMMUNITY_STATUS				0x1069
#define	dCG_REMOVE_NAME_IN_TARGET_FRIEND_LIST	0x10a6
#define	dCG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT	0x10a7

#define	dCG_CHANGE_PARTY_STATUS					0x10b5


#define	dCG_CREATE_GUILD						0x106d
#define	dCG_ASK_GUILD_INFO						0x106e
#define	dCG_ASK_JOIN_GUILD						0x1070
#define	dCG_REPLY_ASK_JOIN_GUILD				0x1071
#define	dCG_ASK_CHANGE_GUILD_NOTICE				0x1072
#define	dCG_ASK_CHANGE_GUILD_EXP_TAX			0x1073
#define	dCG_ASK_DISMISS_SUB_GUILD_MASTER		0x1074
#define	dCG_ASK_APPOINTMENT_SUB_GUILD_MASTER	0x1075
#define	dCG_ASK_DISMISS_GUILD_CONGRESS			0x108a
#define	dCG_ASK_APPOINTMENT_GUILD_CONGRESS		0x108b
#define	dCG_ASK_GUILD_MEMBER_LIST				0x108c
#define	dCG_ASK_DISSOLUTION_GUILD				0x1096
#define	dCG_GUILD_MEMBER_WORK					0x10c2

#define	dCG_ASK_BANISH_GUILD_MEMBER				0x1076
#define	dCG_ASK_LEAVE_GUILD						0x1077
#define	dCG_ASK_CHANGE_GUILD_MASTER				0x1078
#define	dCG_REGIST_REGULAR_GUILD_MEMBER			0x1079
#define	dCG_READY_TO_MAKE_GUILD					0x107a
#define	dCG_INCREASE_GUILD_SKILL_LEVEL			0x1083
#define	dCG_ASK_GUILD_BATTLE_SCHEDULE			0x1084
#define	dCG_REQUEST_GUILD_BATTLE				0x1085
#define	dCG_CLOSE_GUILD_BATTLE_SCHEDULER		0x1086
#define	dCG_CANCEL_GUILD_BATTLE					0x1087	
#define	dCG_SELECT_GUILD_MARK					0x1088
#define	dCG_ASK_GUILD_MARK_INFO					0x109c
#define	dCG_ASK_SIMPLE_GUILD_INFO				0x10a8
#define	dCG_GUILD_CHECK_SUM						0x10a9



#define	dCG_BAD_USER							0x1059
#define	dCG_DEBUG_MESSAGE						0x106f
#define	dCG_CALL_MAGIC_CARPET					0x107e
#define	dCG_REMEMBER_ORB_LOCATION				0x107f
#define	dCG_TELEPORT_TO_ORB_LOCATION			0x1080
#define	dCG_RECOVER_DEATH_PENELTY				0x109d
#define	dCG_CHANGE_PET_NAME						0x109e
#define	dCG_PARTING_WITH_PET					0x109f
#define	dCG_CHANGE_EMBLEM_SHAPE					0x10a0
#define	dCG_ETC_WORK							0x10bf
#define	dCG_XTRAP_PACKET						0x10c5
 

#define	dBW_MAKE_DUMMY							0x1300
#define dBW_EXIT_GAME							0x1301
#define dBW_DEATH_ACTOR							0x1302
#define dBW_READY_TO_EXIT						0x1303


#define dBW_MOVE_FIELD							0x1304
#define dBW_EXIT_GAME_FOR_MOVE_FIELD			0x1305
#define	dBW_DISPLACEMENT						0x1306
#define	dBW_CHARMING							0x1307
#define dBW_MONSTER_TAME						0x1308
#define dBW_GENERATE_PET						0x1309
#define dBW_GENERATE_SUMMON_BEAST				0x130a
#define dBW_INTERVAL_SHOOTER					0x130b
#define dBW_MAKE_SKILL_ZONE						0x130c
#define dBW_COPY_ACTOR							0x130d
#define dBW_MAKE_ILLUSION						0x130e


#define dCG_CONNECT_STRESS_CLIENT				0x2000
#define dCG_STRESSDATA							0x2001
#define dCG_MODIFY_STRESS_CLIENT				0x2002
#define dCG_REGIST_STRESS_CLIENT				0x2003











#define dSG_RESULT_CONNECT						0x1120
#define dSG_RESULT_JOIN							0x1121
#define dSG_LOGOUT								0x12ff
#define dSG_FAILED_TO_JOIN						0x128e
#define dSG_BCS_INFO							0x1270
#define dSG_UPDATE_PLAYER_STATUS_DATA			0x114a
#define dSG_JOIN_PLAYER							0x1122
#define dSG_MOVE								0x1123
#define dSG_MOVE_FOR_PARTY_MEMBER				0x125a
#define dSG_MOVE_RESULT							0x1129
#define dSG_MOVE_FAILED							0x118a
#define dSG_STOP								0x1124
#define dSG_OUTER_ACTORS_POS_UPDATE				0x1177
#define dSG_ACTION_TO_ACTOR						0x1125
#define dSG_ACTION_TO_GROUP						0x1259
#define dSG_ACTION_TO_GROUND					0x1126
#define dSG_ACTION_QUICK						0x114d
#define	dSG_USE_SKILL_RESULT					0x114c
#define dSG_CORRECT_PLAYER_POSITION				0x1127
#define dSG_PACKET_TRAILER						0x1128
#define dSG_MINIMUM_ACTOR_INFO_LIST				0x112a
#define dSG_OPERATOR_LIST						0x11fb
#define dSG_REFIT_ACTORS_POSITION				0x117b
#define dSG_EVENT_NPC_LIST						0x1178
#define dSG_SIMPLE_ACTOR_INFO_LIST				0x1179
#define dSG_SIMPLE_ACTOR_INFO					0x123a
#define dSG_VERY_SIMPLE_ACTOR_INFO_LIST			0x1215
#define dSG_PLAYER_OUT							0x112b
#define dSG_SET_MOVE_ABILITY					0x112d
#define dSG_REGEN_MONSTER						0x112e
#define dSG_BASIC_ATTACK						0x1130
#define dSG_CONTINUOUS_HIT_BY_CHARACTER			0x112f
#define dSG_CONTINUOUS_HIT_BY_VALUE				0x1188
#define dSG_LAND_MARKER							0x11b4
#define dSG_STRIKE_INTERVAL_SHOOTER				0x11b5
#define dSG_CAST_INTERVAL_SHOOTER				0x11b6
#define dSG_BUNSHINE_ATTACK						0x11ad
#define dSG_SCIMITAR_CUTTING					0x11ae
#define dSG_HIT_CHANED_DAMAGE					0x1271
#define dSG_GUARDIAN_POST						0x1201
#define dSG_FIRST_AID							0x122c
#define dSG_REVIVE								0x122e
#define dSG_UPDATE_BOOST_COUNT					0x122f
#define dSG_PLAYER_INFO							0x123d
#define dSG_SUCCESS_TO_JOIN_GAME				0x123e

#define dSG_QUICK_ACTION						0x1151
#define	dSG_KILL								0x1132
#define	dSG_UPDATE_EXP							0x1146
#define	dSG_UPDATE_BASIC_INFORMATION			0x1147
#define	dSG_LEVEL_UP							0x1148
#define	dSG_LEVEL_UP_OTHER						0x1159
#define dSG_UPDATE_STATUS						0x119e
#define	dSG_DEATH_PENELTY_SECOND				0x1211
#define	dSG_QUESTION_FOR_RECOVER_DEATH_PENELTY	0x1225

#define	dSG_CHANGE_STATE_RESULT					0x1149
#define	dSG_INCREASE_ABILITY_LEVEL				0x114b
#define	dSG_VERY_SIMPLE_ACTOR_INFO				0x114e
#define	dSG_REMOVE_ACTOR						0x114f
#define dSG_TRANSFORMATION						0x1152
#define dSG_TRANSFORMATION_FAILED				0x115b
#define dSG_MOVE_FIELD_RESULT					0x1155
#define dSG_MOVE_FIELD_NOW						0x11e4
#define dSG_TOGGLE_SITDOWN						0x1156
#define dSG_SITDOWN_RESULT						0x1157
#define dSG_REDISTRIBUTE						0x1223
#define dSG_STOP_BATTLE							0x1239
#define dSG_GG_AUTH_DATA						0x124b
#define dSG_WINNING_PRIZE_LOTTO					0x124c
#define	dSG_HS_GUID								0x1273
#define	dSG_HS_REQ_MSG							0x1274

#define dSG_ARCA_LIST							0x124f
#define dSG_ADD_ARCA							0x1250
#define dSG_ATTACK_TO_OBJECT					0x1251
#define dSG_ARCA_WORK							0x1252
#define dSG_BROKEN_ARCA_ITEM_BY_ATTACK			0x1253
#define dSG_CLOSE_RANGE_ARCA_WORK				0x1255
#define	dSG_FIELD_TRAP_WORK						0x1256
#define	dSG_CLOSE_RANGE_FIELD_TRAP_WORK			0x1258		

#define	dSG_ENTER_TO_THE_SECRET_DUNGEON			0x125b
#define	dSG_SECRET_DUNGEON_MESSAGE				0x125c
#define	dSG_ETC_WORK							0x125d
#define	dSG_PLAY_SOUND							0x125e
#define	dSG_CHECK_RANGE_ETC_WORK				0x125f
#define	dSG_SET_ANM								0x1260
#define	dSG_DUEL_RECORD							0x1289
#define	dSG_DUEL_RESULT							0x128a
#define	dSG_DUEL_RANKING						0x128b
#define	dSG_TRY_DUEL_BATTLE						0x128c
#define	dSG_DUEL_MSG							0x128d


#define dSG_PET_INFO							0x1226
#define dSG_CHANGE_PET_COMMAND					0x1227
#define dSG_SYNC_PET_INFO						0x1228
#define dSG_PET_WAIT_COMMAND					0x1229
#define dSG_MAKE_BREEDING_RECORD_BOOK			0x122b
#define	dSG_UPDATE_PET_EXP						0x122d
#define	dSG_CHANGE_PET_NAME						0x1230
#define	dSG_PARTING_WITH_PET					0x1232
#define dSG_MINI_PET_INFO						0x1286


#define dSG_SUMMON_BEAST_INFO					0x1231
#define dSG_RIDING_DOG							0x1233


#define	dSG_WIDE_AREA_DAMAGE					0x1150
#define	dSG_WIDE_AREA_EFFECT					0x11ac
#define	dSG_WIDE_HEAL							0x126c
#define	dSG_STRIKE_LIGHTNING_WINDER				0x126d
#define	dSG_WIDE_AID_ATTACK_RESULT				0x1272
#define	dSG_STRIKE_MULTI_MISSILE				0x129a

#define dSG_SET_REACTION_STATUS					0x1164
#define dSG_TURN_ON_PASSIVE_SKILL				0x116f
#define	dSG_HIT_THORN_DAMAGE					0x1165
#define	dSG_ABSORB_DAMAGE						0x119d
#define	dSG_CHANGE_SPECIAL_SKILL_FIELD			0x1166
#define	dSG_RELEASE_SHIMMERING_SHIELD			0x1167
#define	dSG_SET_SHIMMERING_SHIELD				0x1168
#define	dSG_CURE_ACTOR							0x1169
#define	dSG_CURE_ACTOR_BY_POTION				0x11e2
#define	dSG_CHARGE_CP							0x1187
#define	dSG_HIT_AURA_SKILL						0x118c
#define	dSG_DISPLACEMENT						0x1216
#define	dSG_FLOAT_MACHINE						0x116b
#define	dSG_RELEASE_MACHINE						0x116c
#define	dSG_SHOOT_MACHINE_MISSILE				0x116d
#define	dSG_FIRE_TAIL_CHASER					0x116e
#define	dSG_MAKE_ACTION_FORM					0x1170
#define	dSG_THRUST_ACTORS						0x1171
#define	dSG_USE_SKILL_TO_ACTOR					0x1172
#define	dSG_USE_SKILL_TO_GROUND					0x1173
#define	dSG_CREATE_DUMMY						0x1175
#define	dSG_COPY_ACTOR							0x126b
#define	dSG_RELEASE_DUMMY						0x1174
#define	dSG_RECEIVE_AID_MAGIC					0x1186
#define dSG_THRUST								0x11a6
#define dSG_RUSH								0x11a9
#define dSG_ACTIVE_REACTION_SKILL				0x11aa
#define dSG_ICY_STALAGMITE						0x11af
#define	dSG_ACTOR_ENCHANTED_STATUS				0x11b0
#define	dSG_TELEPORT							0x11b1
#define	dSG_APPLY_SKILL_EXTRA_EFFECT			0x11b2
#define	dSG_FIND_USER_RESULT					0x11b7
#define	dSG_ACTIVE_SKILL_EFFECT					0x11b3
#define	dSG_ACTIVE_MIRROR_TOWER					0x1222
#define	dSG_SIMPLE_HIT							0x11bb
#define	dSG_HIT_DAMAGE							0x1213
#define	dSG_REGEN_HP_BY_AID_SKILL				0x11bc
#define	dSG_RECOVER_HP_BY_AID_SKILL				0x121c
#define dSG_HIT_ACTION							0x11d1
#define dSG_DETECTING_EVIL						0x121a
#define dSG_MISS								0x121b
#define dSG_READY_TO_WARP_TO_NEAREST_VILLAGE	0x121d
#define dSG_REMOVE_PORTAL						0x121f
#define dSG_ADD_PORTAL							0x1220
#define dSG_PORTAL_LIST							0x1221
#define dSG_SKILL_EFFECT						0x1238
#define dSG_PLAY_SKILL_SOUND					0x123c
#define dSG_EXIST_FIRE_WALL_LIST				0x1276
#define	dSG_ACTION_TO_GROUP_STICKED_BIT			0x129d

#define	dSG_DROPPING_ITEM						0x1133
#define	dSG_STRIP_EQUIPMENT						0x1153
#define	dSG_HIDE_EQUIPMENT						0x11ab
#define	dSG_STRIP_EQUIPMENT_RESULT				0x1189
#define	dSG_REMOVE_DROPPED_ITEMS				0x1134
#define	dSG_REMOVE_DROPPED_ITEM					0x1135
#define	dSG_CHANGE_DROPPED_ITEM_INFO			0x1136
#define	dSG_DROPPING_ITEMS						0x1137
#define	dSG_PICK_ITEM_RESULT					0x1138
#define	dSG_EQUIP_ITEM_RESULT					0x1139
#define	dSG_USE_ITEM_RESULT						0x113b
#define	dSG_USE_ITEM_TO_ITEM_RESULT				0x11c4
#define	dSG_USE_BELT_ITEM_RESULT				0x113c
#define	dSG_USE_ITEM							0x113d
#define	dSG_LOTTERY_TICKET_PRIZE_WINNING		0x118b
#define	dSG_ITEM_DATA							0x113e
#define	dSG_INVENTORY_DATA						0x1142
#define	dSG_EQUIPMENT_DATA						0x1143
#define	dSG_CHANGE_APPEARANCE					0x113f
#define	dSG_DROP_ITEM_RESULT					0x1141
#define	dSG_RELOAD_BELT_ITEM					0x113a
#define	dSG_BELT_ITEM_RELOAD_RESULT				0x1144
#define	dSG_CHANGE_ITEM_PLACE_RESULT			0x1145
#define dSG_DECREASE_ITEM_COUNT					0x11a1
#define dSG_ADD_ITEM							0x11a2
#define dSG_ADD_GOLD							0x11a3
#define	dSG_SET_ITEM_UNIQUE_SERIAL				0x11b9
#define	dSG_UPDATE_ITEM_DATA					0x11c5
#define	dSG_REMOVE_ITEM							0x11cb
#define	dSG_UPDATE_WAY_POINT_INFO				0x11e3
#define	dSG_EXPIRED_ITEM						0x11e5
#define	dSG_EXPIRED_ITEM_LIST					0x11e6
#define	dSG_USE_ORB_RESULT						0x11e7
#define	dSG_DESTROY_ITEM_RESULT					0x11e9
#define	dSG_UPDATE_ITEM_DURABILITY				0x1212
#define	dSG_REPAIR_ITEM							0x1217
#define	dSG_ITEM_WORK_RESULT					0x1218
#define	dSG_OPEN_REPAIR_ITEM_WINDOW				0x1219
#define	dSG_ITEM_PACK_NAME						0x124d
#define	dSG_REMOVE_PREFIX						0x1262
#define	dSG_OTHER_PLAYER_ITEM_DATA				0x1275
#define	dSG_ITEM_PACK_DATA						0x1285
#define	dSG_OPEN_DISJOINTING_DATA				0x12e0





#define	dSG_REQUEST_TRADE						0x1190
#define	dSG_REQUEST_TRADE_RESULT				0x1191
#define	dSG_BEGIN_TRADE							0x1192
#define	dSG_ADD_TRADE_ITEM_BY_ME				0x1193
#define	dSG_ADD_TRADE_ITEM_BY_TRADE_MATE		0x1194
#define	dSG_CHANGE_TRADE_ITEM_DATA_BY_ME			0x119b
#define	dSG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE	0x119c
#define	dSG_SET_TRADE_GOLD						0x1195
#define	dSG_REMOVE_TRADE_ITEM					0x1196
#define	dSG_READY_TRADE							0x1197
#define	dSG_PERMIT_TRADE						0x1198
#define	dSG_TRADE								0x1199
#define	dSG_TRADE_MESSAGE						0x119a


#define dSG_CHAT								0x1158
#define dSG_CHAT_FAILED							0x118d
#define dSG_INFO								0x1154
#define dSG_OUT									0x1140
#define dSG_SAY_RESULT							0x1176
#define dSG_EVENT_NOTICE						0x11ba
#define dSG_NOTICE								0x123b

#define	dSG_PARTY_LIST							0x117c
#define	dSG_CHANGE_PARTY_STATUS					0x1261

#define	dSG_PARTY_INFO							0x117d
#define dSG_ASK_JOIN_PARTY						0x117e
#define	dSG_JOIN_PARTY_MEMBER					0x117f
#define	dSG_PARTY_WORK							0x1180
#define	dSG_UPDATE_PARTY_MEMBER_HP				0x126e
#define	dSG_UPDATE_PARTY_MEMBER_INFO			0x1181
#define	dSG_PARTY_MESSAGE						0x1182
#define	dSG_CHANGE_PARTY_NAME					0x1183
#define	dSG_DISSOLUTION_PARTY					0x1185
#define	dSG_PCIK_PARTY_ITEM						0x118e
#define	dSG_PCIK_PARTY_GOLD						0x118f
#define	dSG_MESSAGE								0x11a4
#define dSG_REGIST_MESSAGE						0x11cf
#define	dSG_REMAIN_HP							0x11a5
#define	dSG_REQUEST_ADD_FRIEND					0x11c8
#define	dSG_REPLY_FOR_ADD_FRIEND				0x11c9
#define dSG_EXIT_GAME_SUCCESS					0x11d0
#define dSG_PICK_PARTY_ITEM						0x123f
#define dSG_ASK_REMOVE_FRIEND					0x1244
#define dSG_ASK_REMOVE_FRIEND_RESULT			0x1245
#define	dSG_GAME_SERVER_MESSAGE					0x124e


#define dSG_ASK_JOIN_UNION_PARTY				0x1310
#define dSG_REPLAY_JOIN_UNION_PARTY				0x1311
#define dSG_UNION_PARTY_INFO					0x1312
#define dSG_UNION_PARTY_MEMBER_INFO				0x1313
#define dSG_UNION_PARTY_MEMBER_HP_STATE			0x1314
#define dSG_DISSOLUTION_UNION_PARTY				0x1315



#define	dSG_READY_TO_MAKE_GUILD					0x11ca
#define	dSG_OPEN_GUILD_BATTLE_SCHEDULER			0x11eb
#define	dSG_OPEN_GUILD_MARK_COMPOSER			0x11f2
#define	dSG_CHANGE_GUILD_BATTLE_SCHEDULE		0x11ec
#define	dSG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME	0x11ed
#define	dSG_MAKE_GUILD_RESULT					0x11cd
#define	dSG_BASE_GUILD_INFO						0x11ce
#define	dSG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER	0x11ef
#define	dSG_READY_TO_NEXT_GUILD_BATTLE			0x11f0
#define	dSG_GUILD_BATTLE_NOTICE					0x1210
#define	dSG_GUILD_BATTLE_KILL_NOTICE			0x1269
#define	dSG_GUILD_BATTLE_INFO					0x126a
#define	dSG_DISSOLUTION_GUILD					0x11d2
#define	dSG_JOIN_GUILD							0x11d3
#define	dSG_UPDATE_GUILD_MEMBER					0x1203
#define	dSG_REMOVE_GUILD_MEMBER					0x11dd
#define	dSG_EXIT_GUILD							0x11d4
#define	dSG_ASK_JOIN_GUILD_RESULT				0x11d5
#define	dSG_ASK_JOIN_GUILD						0x11d6
#define	dSG_UPDATE_GUILD_LEVEL					0x11d7
#define	dSG_GUILD_WORK_MESSAGE					0x11d8
#define	dSG_CHANGE_GUILD_TAX_RATE				0x11da
#define	dSG_CHANGE_GUILD_NOTICE					0x11db
#define	dSG_CHANGE_RANK_IN_GUILD				0x11dc
#define	dSG_UPDATE_GUILD_SKILL_LEVEL			0x11ea
#define	dSG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK	0x11ee
#define dSG_GUILD_BATTLE_RESULT					0x11f1
#define dSG_CHANGE_GUILD_MARK					0x11f3
#define dSG_GUILD_MEMBER_LIST					0x1202
#define dSG_GUILD_MARK_INFO						0x1224
#define dSG_SIMPLE_GUILD_INFO					0x1246
#define	dSG_OPEN_GUILD_HALL_LIST				0x1279
#define	dSG_OPEN_GUILD_INVENTORY				0x127a
#define	dSG_STORE_GUILD_INVENTORY_ITEM			0x127b
#define	dSG_REMOVE_GUILD_INVENTORY_ITEM			0x127c
#define	dSG_STORE_GUILD_INVENTORY_GOLD			0x127d
#define	dSG_REMOVE_GUILD_INVENTORY_GOLD			0x127e
#define	dSG_CHANGE_GUILD_INVENTORY_ITEM_PLACE	0x127f
#define	dSG_SIEGE_WARFARE_NOTICE				0x1280
#define	dSG_GUILD_PET_AND_GUARDIAN_EXP			0x1281
#define	dSG_OPEN_GUILD_SHOP						0x1282
#define	dSG_OPEN_RELATED_PLACE_LIST				0x1283
#define	dSG_GUILD_HONOR_POINT_LIST				0x1284
#define	dSG_GUILD_MASTER_GRACE_INFO				0x129f	
#define	dSG_GVG_WORK_SUCCESS					0x129c


#define dSG_COMPLEX_SPEECH						0x115c
#define dSG_EVENT_FAILED						0x115d
#define dSG_END_DIAOLG							0x115e
#define dSG_OPEN_SHOP							0x115f
#define dSG_BUY_ITEM							0x1160
#define dSG_FAILED_BUY_ITEM						0x1161
#define dSG_SELL_ITEM							0x1162
#define dSG_FAILED_SELL_ITEM					0x1163
#define dSG_QUEST_INFO							0x11a0
#define dSG_CHANGE_QUEST_INFO					0x11bd
#define	dSG_CANCEL_QUEST						0x11cc
#define dSG_DAMAGE_TO_ACTOR_BY_KARMA			0x11b8
#define	dSG_GET_TITLE							0x11c6
#define	dSG_REMOVE_TITLE						0x11c7
#define	dSG_ANNOUNCE_QUIZ_EVENT					0x11f4
#define	dSG_START_COUNTDOWN						0x11f5
#define	dSG_QUIZ_EVENT_QUESTION					0x11f7
#define	dSG_OX_QUIZ_RESULT						0x11f6
#define	dSG_END_QUIZ_EVENT						0x11f8
#define	dSG_QUIZ_EVENT_LOSER_LIST				0x11f9
#define	dSG_CANCEL_QUIZ_EVENT					0x11fa
#define	dSG_OX_QUIZ_SURVIVOR_COUNT				0x11fc
#define	dSG_OX_QUIZ_LOSER_REVIVE_BATTLE			0x11fd
#define	dSG_OX_QUIZ_WINNER_LIST					0x11fe
#define	dSG_BEGIN_QUIZ_EVENT					0x11ff
#define	dSG_BUY_CARROT_SHOP_ITEM_RESULT			0x1242
#define	dSG_CHANGE_DOOR_STATUS					0x1247
#define	dSG_DOOR_MESSAGE						0x1248
#define	dSG_DOORS_STATUS_IN_FIELD				0x1249
#define	dSG_BANNER_INFO							0x1265
#define	dSG_BANNER_TEXT							0x1266
#define	dSG_BANNER_LIST							0x1267
#define	dSG_BOOST_POWER_BY_EVENT				0x1268
#define	dSG_WORD_QUIZ_RESULT					0x126f
#define	dSG_EVENT_SCREEN_MOVE					0x1278
#define	dSG_SOUND_OF_LEADERS_BELL				0x1299
#define	dSG_EVENT_TIME_UPDATE					0x129e


#define dSG_OPEN_BANK							0x11a7
#define dSG_BANK_MESSAGE						0x11a8
#define	dSG_STORE_ITEM_TO_THE_BANK				0x11be
#define	dSG_STORE_GOLD_TO_THE_BANK				0x11bf
#define	dSG_WITHDRAW_ITEM_FROM_THE_BANK			0x11c0
#define	dSG_WITHDRAW_GOLD_FROM_THE_BANK			0x11c1
#define	dSG_MOVE_BANK_ITEM						0x11c2
#define	dSG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	0x11c3


#define dSG_OPEN_CART							0x11de
#define dSG_CART_MESSAGE						0x11df
#define	dSG_WITHDRAW_ITEM_FROM_THE_CART			0x11e0
#define	dSG_LOAD_FAILED_CART_PREMIUM_ITEM		0x11e1
#define	dSG_DESTROY_CART_ITEM_RESULT			0x11e8
#define	dSG_CARROT_SHOP_INFO					0x1263
#define	dSG_OPEN_CARROT_SHOP					0x1264


#define dSG_OPEN_PITCHMAN_SHOP					0x1204
#define dSG_ADD_PITCHMAN_SHOP_ITEM				0x1205
#define dSG_REMOVE_PITCHMAN_SHOP_ITEM			0x1206
#define dSG_CHANGE_PITCHMAN_SHOP_INFO			0x1207
#define dSG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		0x120f
#define	dSG_BUY_PITCHMAN_SHOP_ITEM				0x1208
#define	dSG_SOLD_PITCHMAN_SHOP_ITEM				0x1209
#define dSG_ADD_PITCHMAN_SHOP					0x120a
#define dSG_REMOVE_PITCHMAN_SHOP				0x120b
#define dSG_PITCHMAN_SHOP_LIST					0x120c
#define dSG_PITCHMAN_SHOP_INFO					0x120d
#define dSG_PITCHMAN_SHOP_MESSAGE				0x120e
#define dSG_SIMPLE_PITCHMAN_SHOP_INFO_LIST		0x1214
#define dSG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION	0x1288


#define dSG_ADD_TRAP							0x1234
#define dSG_REMOVE_TRAP							0x1235
#define dSG_EXPLOSION_TRAP						0x1236
#define dSG_EXIST_TRAP_LIST						0x1237
#define dSG_EXPLOSION_TRAP_SKILL				0x1254
#define	dSG_EXIST_ACTIVE_FIELD_TRAP_LIST		0x1257


#define dSG_ADD_GGG_LEVEL_UP					0x1290		
#define dSG_ADD_GGG_SKILL_UP					0x1291		
#define dSG_ADD_GGG_PARTY						0x1292		
#define dSG_GGG_LEVEL_UP_TIME_END				0x1293		
#define dSG_GGG_SKILL_UP_TIME_END				0x1294
#define dSG_ADD_GGG								0x1295
#define dSG_REMOVE_GGG							0x1296
#define dSG_PLAY_GGG_EFFECT						0x1297
#define dSG_GGG_INFO							0x1298

#define	dSG_WARNING_MESSAGE						0x117a
#define	dSG_CONTROL_MONSTER						0x119f
#define	dSG_OCCUR_EFFECT						0x122a
#define	dSG_ACCOUNT_INFO						0x1243
#define	dSG_PENELTY_TIME						0x124a
#define	dSG_TITLE_EFFECT						0x1287
#define	dSG_XTRAP_PACKET						0x128f
#define	dSG_UPDATE_PACKET_ENCRYT_CODE			0x129b


#define dSG_SENDINFOTOADMIN						0x1201
#define dSG_CLOSESERVER							0x1202


#define dSG_STRESSCLIENT						0x2200
#define dSG_STRESS_SERVER_INFO					0x2201


#define dSG_RESULT_PRIZE_PLAYER					0x2202






#define dEXIST_ACTOR_COUNT_FOR_ROOKIE				200
#define dVISIBLE_ACTOR_COUNT_FOR_ROOKIE				50
#define dEXIST_ACTOR_COUNT							10










enum
{
	eCONNECT_TYPE_FIRST		,
	eCONNECT_TYPE_MOVE_FIELD,
};



struct	CG_CONNECT_GAME
{
	cMSG_BASE_TYPE	base;

	WORD			wType;
	char			strId[dID_LENGTH];
	DWORD			dwCode;
	char			strName[dNAME_LENGTH];
	char			strMacAddress[64];
	char			strClientCode[12];
#ifdef	_OGP_SERVICE
	char			strToken[dTOKEN_LENGTH];
#endif
};



struct	CG_RESULT_PRIZE_PLAYER
{
	cMSG_BASE_TYPE	base;
	WORD			wSerialInField;
	WORD			wResult;
};



struct	CG_JOIN_GAME
{
	cMSG_BASE_TYPE	base;
};


struct	CG_SETTING_SEASON_VALIABLE
{
	cMSG_BASE_TYPE	base;
	WORD			wSeasonValiable;
};



struct	CG_MOVE
{
	cMSG_BASE_TYPE		base;

	WORD				wPosX,wPosY;
	WORD				wDestPosX,wDestPosY;
};



struct	CG_REPORT_CURRENT_POS
{
	cMSG_BASE_TYPE		base;

	WORD				wPosX,wPosY;
	WORD				wDestPosX,wDestPosY;
};



struct	CG_STOP
{
	cMSG_BASE_TYPE	base;

	WORD			wDirect;
	WORD			wPosX,wPosY;
};

#define	dMOVE_ABILITY_WALK		0	
#define	dMOVE_ABILITY_RUN		1


struct	CG_SET_MOVE_ABILITY
{
	cMSG_BASE_TYPE	base;

	WORD			wState;
};



struct	CG_TRANSFORMATION
{
	cMSG_BASE_TYPE	base;

	WORD			wPosX,wPosY;
};

struct	CG_ACTION_TO_OBJECT
{
	cMSG_BASE_TYPE	base;

	BYTE			bObjectType,bAbility;
	WORD			wObjectSerial;
};


struct	CG_ACTION_TO_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wTargetSerial;
	BYTE			bAbility,bEngageAbility,bSubAbility;
};



struct	CG_ACTION_TO_GROUND
{
	cMSG_BASE_TYPE	base;

	WORD			wDestPosX,wDestPosY;
	WORD			wAbility;
};



struct	CG_ACTION_QUICK
{
	cMSG_BASE_TYPE	base;

	WORD			wAbility;
	WORD			wPosX,wPosY;
};



struct	CG_ENGAGE_BATTLE
{
	cMSG_BASE_TYPE	base;

	WORD			wTargetSerial;
	WORD			wAbility;
};



struct	CG_STOP_BATTLE
{
	cMSG_BASE_TYPE	base;

};

enum
{
	eCHARACTER_BASIC_STATE_STRENGTH		,
	eCHARACTER_BASIC_STATE_CONSTITUTION	,
	eCHARACTER_BASIC_STATE_AGILITY		,
	eCHARACTER_BASIC_STATE_INTELLIGENCE	,
	eCHARACTER_BASIC_STATE_WISDOM		,
	eCHARACTER_BASIC_STATE_CHARISMA		,
	eCHARACTER_BASIC_STATE_LUCK			,
	eCHARACTER_BASIC_STATE_FAILED		,
};



struct	CG_INCREASE_STATE
{
	cMSG_BASE_TYPE	base;

	WORD			wState;
	WORD			wPoint;
};



struct	CG_INCREASE_ABILITY_LEVEL
{
	cMSG_BASE_TYPE	base;

	WORD			wAbility;
	WORD			wPoint;
	WORD			wAimLevel;
};



struct	CG_ASK_ACTOR_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
};


struct	CG_TOGGLE_SITDOWN
{
	cMSG_BASE_TYPE	base;
	WORD			wPosX,wPosY;
};



struct	CG_MOVE_FIELD
{
	cMSG_BASE_TYPE	base;

};

struct	CG_WARP_TO_NEAR_VILLAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wVillage;
};

struct	CG_ENTER_PORTAL
{
	cMSG_BASE_TYPE	base;

	WORD			wPortal;
};



struct	CG_SET_REACTION_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD			wAbility;
	WORD			wIsOn;
};



struct	CG_TURN_ON_PASSIVE_SKILL
{
	cMSG_BASE_TYPE	base;

	BYTE			aPassiveSkill[6];
};



enum
{
	eRTV_BY_REVIVE,
	eRTV_BY_ITEM,
	eRTV_BY_END_OF_GUILD_BATTLE,
	eRTV_BY_END_OF_EVENT,
};


struct	CG_RETURN_TO_VILLAGE
{
	cMSG_BASE_TYPE	base;
	
	WORD			wWork;
};



enum
{
	eEGV_RETURN_TO_TITLE,
	eEGV_RETURN_TO_SELECT_AVATAR,
	eEGV_EXIT_TO_WINDOW,
};

struct	CG_EXIT_GAME
{
	cMSG_BASE_TYPE	base;
	
	WORD			wValue;
};

struct	CG_HACKING_LOG
{
	cMSG_BASE_TYPE	base;

	WORD			wCode;
	char			strHackName[32];
};

struct	CG_GG_AUTH_DATA
{
	cMSG_BASE_TYPE	base;

	char			aBuffer[16];
};

struct	CG_HS_GUID_REPLY
{
	cMSG_BASE_TYPE	base;

	BYTE			abGuidAckMsg[20];
};

struct	CG_HS_ACK_MSG_REPLY
{
	cMSG_BASE_TYPE	base;

	BYTE			abAckMsg[56];
};

struct	CG_WARP_FIELD_BY_GATE_GLOVE
{
	cMSG_BASE_TYPE	base;

	WORD			wDestField;
	int				iItemSlot;
};











struct	CG_PICK_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wItemSerial;
};



struct	CG_EQUIP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wItemSlot;
	WORD			wBaseItem;
	WORD			wPart;
};



struct	CG_RELOAD_BELT_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
};



struct	CG_USE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wItemSlot;
	WORD			wBaseItem;
	WORD			wWho;
};



struct	CG_USE_ITEM_TO_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wBaseItem;
	BYTE			bItemSlot,bTargetItem;
};



struct	CG_USE_BELT_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wBeltSlot;
	WORD			wBaseItem;
	WORD			wTarget;
};



struct	CG_ASK_ITEM_DATA
{
	cMSG_BASE_TYPE	base;
};



struct	CG_ASK_INVENTORY_DATA
{
	cMSG_BASE_TYPE	base;
};



struct	CG_ASK_EQUIPMENT_DATA
{
	cMSG_BASE_TYPE	base;
};



struct	CG_DROP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wPosX,wPosY;
	WORD			wItemSlot;
	DWORD			dwCount;
	WORD			wBaseItem;
};



struct	CG_CHANGE_ITEM_PLACE
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wSrcItemSlot,wDestItemSlot;
};



struct	CG_STRIP_EQUIPMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wPart;
};

struct	CG_DESTROY_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wSlot;
};

struct	CG_REPAIR_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCheckSum;
	WORD			wSlot;
	WORD			wNpcSerial;
};

struct	CG_ASK_ITEM_PACK_NAME
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			awPackIndex[32];
};

struct	CG_REMOVE_PREFIX
{
	cMSG_BASE_TYPE	base;

	WORD			wPrefixIndex;
	WORD			wUseItem,wDestItem;
};

struct	CG_JUDGE_TO_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wNpc;
};

struct	CG_MERGE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wNpc;
};












struct	CG_REQUEST_TRADE
{
	cMSG_BASE_TYPE	base;
	WORD			wTradeTarget;
	char			strTargetName[dNAME_LENGTH];
};



enum
{
	eRTR_OK,
	eRTR_DENY,
};



struct	CG_TRADE_REPLY
{
	cMSG_BASE_TYPE	base;

	char			strAsker[dNAME_LENGTH];
	WORD			wReply;
};



struct	CG_CANCEL_TRADE
{
	cMSG_BASE_TYPE	base;
};



struct	CG_ADD_TRADE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wCount;
};



struct	CG_CHANGE_TRADE_ITEM_DATA
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wTradeSlot;
	WORD			wCount;
};



struct	CG_REMOVE_TRADE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wTradeSlot;
};



struct	CG_SET_TRADE_GOLD
{
	cMSG_BASE_TYPE	base;

	int				iGold;
};



struct	CG_READY_TRADE
{
	cMSG_BASE_TYPE	base;
};



struct	CG_PERMIT_TRADE
{
	cMSG_BASE_TYPE	base;
	int				iCheckSum;
};








struct	CG_STORE_ITEM_TO_THE_BANK
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wInventorySlot,wBankSlot;
	WORD			wBankSerial;
};

struct	CG_STORE_GOLD_TO_THE_BANK
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	int				iGold;
	WORD			wBankSerial;
};

struct	CG_WITHDRAW_ITEM_FROM_THE_BANK
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wInventorySlot,wBankSlot;
	WORD			wBankSerial;
};

struct	CG_WITHDRAW_GOLD_FROM_THE_BANK
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	int				iGold;
	WORD			wBankSerial;
};

struct	CG_MOVE_BANK_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wPos1,wPos2;
	DWORD			dwCheckSum;
	WORD			wBankSerial;
};

struct	CG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION
{
	cMSG_BASE_TYPE	base;

	WORD			wPos1,wPos2;
	DWORD			dwCheckSum;
	WORD			wBankSerial;
};

struct	CG_FINISH_BANK_TRANSACTION
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wBankSerial;
};

struct	CG_CANCEL_BANK_TRANSACTION
{
	cMSG_BASE_TYPE	base;
};








struct	CG_CART_OPEN
{
	cMSG_BASE_TYPE	base;
};

struct	CG_CLOSE_CART
{
	cMSG_BASE_TYPE	base;
};

struct	CG_DESTROY_CART_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCartSerial;
	WORD			wCartSlot;
	DWORD			dwCheckSum;
};

struct	CG_WITHDRAW_ITEM_FROM_THE_CART
{
	cMSG_BASE_TYPE	base;

	WORD			wCartSerial;
	WORD			wCartSlot;
	WORD			wInventorySlot;
	DWORD			dwCheckSum;
};

struct	CG_OPEN_CARROT_SHOP
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
};









struct	CG_ASK_OPEN_THE_PITCHMAN_SHOP
{
	cMSG_BASE_TYPE	base;

	WORD			wPosX,wPosY;
};


struct	CG_ADD_PITCHMAN_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wInventorySlot,wShopSlot;
	DWORD			dwPrice;
	BYTE			bDenomination;
};


struct	CG_REMOVE_PITCHMAN_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot,wSerial;
};


struct	CG_CHANGE_PITCHMAN_SHOP_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wTextColor;
	WORD			bf1IsBold			: 1;
	WORD			bf1IsShadowText		: 1;
	WORD			bf2Status			: 2;
	WORD			bf10SignboardShape	: 10;

	char			strName[dPITCHMAN_SHOP_NAME_LENGTH];
};

struct	CG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wSlot1,wSlot2;
};

struct	CG_CLOSE_PITCHMAN_SHOP
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
};


struct	CG_BUY_PITCHMAN_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wPitchmanSerial;
	WORD			wSlot;
	DWORD			dwPrice;
	BYTE			bDenomination;
	CItemDefine		item;
};


struct	CG_ASK_PITCHMAN_SHOP_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wPitchmanSerial;
};

#define	dMAX_ASK_SIMPLE_PITCHMAN_SHOP_INFO_COUNT	10

struct	CG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			awList[dMAX_ASK_SIMPLE_PITCHMAN_SHOP_INFO_COUNT];
};











struct	CG_SELECT_NPC
{
	cMSG_BASE_TYPE	base;

	WORD			wNpcSerial;
};



struct	CG_SELECT_SPEECH
{
	cMSG_BASE_TYPE	base;

	WORD			wSpeaker;
	WORD			wSpeechIndex;
	WORD			wSelectSpeech;
	WORD			wEventArea;
};



struct	CG_BUY_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	WORD			wShopSerial;
	WORD			wSelectItem;
	WORD			wIsGuildShop;
	WORD			wBuyCount;
	WORD			wIsTokenShop;
	WORD			bf1IsBadgeOfTraderShop :1;
	WORD			:0;
};


struct	CG_SELL_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wShopKeeper;
	WORD			wShopSerial;
	WORD			wSellItemSlot;
	WORD			wCheckSum;
	WORD			wBaseItem;
	WORD			wCount;
	WORD			bf1IsBadgeOfTraderShop :1;
	WORD			:0;
};



struct	CG_CANCEL_QUEST
{
	cMSG_BASE_TYPE	base;

	WORD			wQuestSlot;
	WORD			wQuestIndex;
};



enum
{
	eCDSM_OPEN_DOOR,
	eCDSM_CLOSE_DOOR,
	eCDSM_ATTACK_DOOR,
	eCDSM_DISARM_DOOR,
	eCDSM_UNLOCK_DOOR,
	eCDSM_UNLOCK_DOOR_BY_KEY,
	eCDSM_ENTER_TO_THE_SECRET_DUNGEON,
	eCDSM_COUNT
};

struct	CG_CHANGE_DOOR_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD	wArea;

	WORD	wMethod;
};

struct	CG_TOUCH_EVENT_AREA
{
	cMSG_BASE_TYPE	base;

	WORD	wArea;
};

enum
{
	eAAW_OPEN,
	eAAW_DISARM_TRAP,
	eAAW_UNLOCK,
	eAAW_UNLOCK_BY_USE_KEY,
	eAAW_ATTACK,
};

struct	CG_ARCA_WORK
{
	cMSG_BASE_TYPE	base;

	WORD	wWork,wArca;
	WORD	wValue1;
};

struct	CG_ASK_ARCA_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			awArcaIndex[32];
};

struct	CG_DISARM_FIELD_TRAP
{
	cMSG_BASE_TYPE	base;

	WORD			wFieldTrap;
};

struct	CG_ASK_DOOR_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wDoorArea;
};

struct	CG_PUT_BANNER
{
	cMSG_BASE_TYPE	base;

	WORD	wX,wY;
	WORD	wBannerItemSlot;

	char	strTitle[dBANNER_TITLE_LENGTH];
	char	strComment[dBANNER_COMMENT_LENGTH];
};

struct	CG_ASK_BANNER_TEXT
{
	cMSG_BASE_TYPE	base;

	WORD	wBannerSerial;
};

struct	CG_SEARCH_RELAY_STATION_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD	wItemType;
	WORD	wLevelRange;
	WORD	wFilterJob;
	WORD	wLastShop,wLastSlot;

	WORD	bf1IsSearchReverse	:	1;
	WORD	bf1IsORCondition:	1;
	WORD	bf1MaskUnique	:	1;
	WORD	bf1MaskMagic	:	1;
	WORD	bf1MaskDX		:	1;
	WORD	bf1MaskUltimate	:	1;

	char	strFilter[64];

};

struct	CG_BUY_CARROT_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wCategory,wItemIndex;
};









#define	dMAX_REQUEST_ACTOR_DATA_COUNT		50


struct	CG_ASK_INFO_ACTOR_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			aAskInfoActorList[dMAX_REQUEST_ACTOR_DATA_COUNT];
};


struct	CG_OBSCURITY_ACTOR_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			aObscurityActor[dMAX_REQUEST_ACTOR_DATA_COUNT];
};

enum
{
	eAEI_SHAKLE_SKILL,
	eAEI_BITE_HANGER,
	eAEI_THE_OTHER_SELF_COUNT,
};

#define	dMAX_ASK_ACTOR_ENCHANT_INFO_COUNT	50

struct	CG_ASK_ACTOR_ENCHANT_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			awData[dMAX_ASK_ACTOR_ENCHANT_INFO_COUNT][2];
};

#define	dMAX_MISPLACED_ACTOR_COUNT			100

struct	CG_MISPLACED_ACTOR_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			aMisplacedActorList[dMAX_MISPLACED_ACTOR_COUNT];
};

struct	CG_ASK_PLAYER_INFO
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
};

struct	CG_CHECK_PLAYER_CHECK_SUM
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
};











#define	dCHAT_CHAT						0
#define	dCHAT_PARTY						1
#define	dCHAT_SAY						2
#define	dCHAT_GUILD						3
#define	dCHAT_SHOUT						4
#define dCHAT_ADMIN						5
#define dCHAT_DUMMY						6



struct	CG_CHAT
{
	cMSG_BASE_TYPE	base;

	WORD			wChatType;
	char			strDestName[dNAME_LENGTH];
	char			strMessage[dCHAT_LENGTH];
};




enum
{
	eAPLD_PREVIOUS,
	eAPLD_NEXT,
};



struct	CG_ASK_PARTY_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wPartyIndex;
	WORD			wDirection;
};



struct	CG_ASK_PARTY_INFO
{
	cMSG_BASE_TYPE	base;
};



struct	CG_ASK_JOIN_PARTY
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsJoinPartyApplication	:	1;
	WORD			bf1IsUnionPartyApplication	:	1;
	WORD			bf1IsGuildMemberJoinParty		:	1;

	char			strName[dNAME_LENGTH];
	
};



struct	CG_ASK_JOIN_PARTY_REPLY
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsForJoinPartyApplication	:	1;
	WORD			bf1IsUnionPartyApplication		:	1;
	WORD			bf1IsGuildMemberJoinParty		:	1;

	WORD			wReply;

	char			strAskerName[dNAME_LENGTH];
};

struct	CG_CHANGE_PARTY_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD	bf3GoldShareMethod	:	3;
	WORD	bf3ItemShareMethod	:	3;
	WORD	bf3PartyObject		:	3;
	WORD	bf1IsOpenParty		:	1;
};



struct	CG_LEAVE_PARTY
{
	cMSG_BASE_TYPE	base;
};

enum
{
	ePLW_BANISH_MEMBER,
	ePLW_CHANGE_LEADER,
};


struct	CG_PARTY_LEADER_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	char			strName[dNAME_LENGTH];
};



struct	CG_CHANGE_PARTY_NAME
{
	cMSG_BASE_TYPE	base;

	char			strName[dPARTYNAME_LENGTH];
};



struct	CG_ACCUSE
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum1,dwCheckSum2;
	DWORD			dwSerial;
};



enum
{
	eOC_RETURN_TO_VILLAGE	,
	eOC_CHANGE_LOCATE		,
	eOC_MOVE_FIELD			,
	eOC_RECALL				,
	eOC_WARP				,
	eOC_BANISH				,
	eOC_JAIL				,
	eOC_DRAWING_ROOM		,
	eOC_VILLAGE				,
	eOC_CONTROL_MONSTER		,
	eOC_TRANS_TO_EVENT_MOB	,
	eOC_REGEN_FIELD_MOB		,
	eOC_SPRINKLE_GIFT		,
	eOC_HIDE				,
	eOC_GET_ITEM			,
	eOC_SUMMON_MONSTER		,
};



struct	CG_OPERATOR_COMMAND
{
	cMSG_BASE_TYPE	base;

	WORD			wCommand;
	WORD			wX,wY;
	char			strName[dNAME_LENGTH];
};



struct	CG_FIND_USER
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
};



struct	CG_REQUEST_ADD_FRIEND
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];

	WORD			bf1IsRecoverListWork	:	1;
	WORD			bf8Job					:	8;
	WORD			wCheckSign;
};



enum
{
	eRFA_OK,
	eRFA_DENY,
	eRFA_CAN_NOT_FIND,
	eRFA_STATUS_DENY_COMMUNITY,
	eRFA_AUTO_ADD_BY_ADDED,
};



struct	CG_REPLY_FOR_ADD_FRIEND
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	char			strName[dNAME_LENGTH];
};



struct	CG_CHANGE_COMMUNITY_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsDenyCommunity		:	1;
	WORD			bf1IsHidePitchmanShop	:	1;
};

struct	CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST
{
	cMSG_BASE_TYPE	base;

	char	strTargetName[dNAME_LENGTH];
};

enum
{
	eRNITFLR_SUCCESS,
	eRNITFLR_NOT_EXIST,
};

struct	CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT
{
	cMSG_BASE_TYPE	base;

	char	strRequestUserName[dNAME_LENGTH];
	WORD	wResult;
};



struct	CG_CREATE_GUILD
{
	cMSG_BASE_TYPE	base;

	char			strGuildName[dGUILD_NAME_LENGTH];
	char			strSubGuildMasterName[dNAME_LENGTH];
};

struct	CG_ASK_GUILD_INFO
{
	cMSG_BASE_TYPE	base;

	WORD	wIsIgnoreSkillData;
};

struct	CG_ASK_JOIN_GUILD
{
	cMSG_BASE_TYPE	base;

	WORD			wTargetSerial;
};



enum
{
	eAJGR_DENY		,
	eAJGR_ACCEPT	,
	eAJGR_BUSY		,
};

struct	CG_REPLY_ASK_JOIN_GUILD
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wAskerSerial;
	char			strAskerName[dNAME_LENGTH];
};


struct	CG_ASK_CHANGE_GUILD_NOTICE
{
	cMSG_BASE_TYPE	base;

	char			strNotice[dGUILD_NOTICE_LENGTH];
};


struct	CG_ASK_CHANGE_GUILD_EXP_TAX
{
	cMSG_BASE_TYPE	base;

	WORD			wTax;
};


struct	CG_ASK_DISMISS_SUB_GUILD_MASTER
{
	cMSG_BASE_TYPE	base;

	char			strSubGuildMaster[dNAME_LENGTH];
};


struct	CG_ASK_APPOINTMENT_SUB_GUILD_MASTER
{
	cMSG_BASE_TYPE	base;

	char			strSubGuildMaster[dNAME_LENGTH];
};


struct	CG_ASK_DISMISS_GUILD_CONGRESS
{
	cMSG_BASE_TYPE	base;

	char			strGuildCongress[dNAME_LENGTH];
};


struct	CG_ASK_APPOINTMENT_GUILD_CONGRESS
{
	cMSG_BASE_TYPE	base;

	char			strGuildCongress[dNAME_LENGTH];
};

struct	CG_ASK_GUILD_MEMBER_LIST
{
	cMSG_BASE_TYPE	base;

	BOOL			bIsinit;
};

enum
{
	eGMW_RENT_GUILD_PET,
	eGMW_RETRIEVAL_RENTED_GUILD_PET,
};

struct	CG_GUILD_MEMBER_WORK
{
	cMSG_BASE_TYPE	base;

	WORD	wWork,awValue[6];
	char	strMember[dNAME_LENGTH];
};

struct	CG_ASK_DISSOLUTION_GUILD
{
	cMSG_BASE_TYPE	base;
};


struct	CG_ASK_BANISH_GUILD_MEMBER
{
	cMSG_BASE_TYPE	base;

	char			strMember[dNAME_LENGTH];
};


struct	CG_ASK_LEAVE_GUILD
{
	cMSG_BASE_TYPE	base;
};

struct	CG_ASK_CHANGE_GUILD_MASTER
{
	cMSG_BASE_TYPE	base;

	char			strMaster[dNAME_LENGTH];
};

struct	CG_REGIST_REGULAR_GUILD_MEMBER
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
};

struct	CG_READY_TO_MAKE_GUILD
{
	cMSG_BASE_TYPE	base;
};

struct	CG_INCREASE_GUILD_SKILL_LEVEL
{
	cMSG_BASE_TYPE	base;
	WORD			wSkill;
};

struct	CG_ASK_GUILD_BATTLE_SCHEDULE
{
	cMSG_BASE_TYPE	base;

	WORD			wTimeOrder;
};

struct	CG_REQUEST_GUILD_BATTLE
{
	cMSG_BASE_TYPE	base;

	WORD			wTimeOrder,wFieldOrder;
};

struct	CG_CANCEL_GUILD_BATTLE
{
	cMSG_BASE_TYPE	base;
	
	WORD			wTimeOrder;
};

struct	CG_SELECT_GUILD_MARK
{
	cMSG_BASE_TYPE	base;

	CGuildMarkInfo	guildMarkInfo;
};

struct	CG_ASK_GUILD_MARK_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wGuildSerial;
};

struct	CG_ASK_SIMPLE_GUILD_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wGuildSerial;
};

struct	CG_GUILD_CHECK_SUM
{
	cMSG_BASE_TYPE	base;

	int				iCheckSum;
};

struct	CG_CLOSE_GUILD_BATTLE_SCHEDULER
{
	cMSG_BASE_TYPE	base;
};



enum
{
	eBUT_SPEED_HACK,
};



struct	CG_BAD_USER
{
	cMSG_BASE_TYPE	base;

	WORD			wType;
};

#define	dDEBUG_MESSAGE_LENGTH	256

struct	CG_DEBUG_MESSAGE
{
	cMSG_BASE_TYPE	base;

	char			strMessage[dDEBUG_MESSAGE_LENGTH];
};

struct	CG_CALL_MAGIC_CARPET
{
	cMSG_BASE_TYPE	base;
};

struct	CG_REMEMBER_ORB_LOCATION
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
};

struct	CG_TELEPORT_TO_ORB_LOCATION
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
};

struct	CG_RECOVER_DEATH_PENELTY
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
};

struct	CG_CHANGE_PET_NAME
{
	cMSG_BASE_TYPE	base;

	WORD			wPetIndex;
	WORD			wIsMiniPet;
	char			strPetName[dPET_NAME_LENGTH+1];
};

struct	CG_PARTING_WITH_PET
{
	cMSG_BASE_TYPE	base;

	WORD	bf10PetIndex	:	11;
	WORD	bf6Type			:	6;
};

struct	CG_CHANGE_EMBLEM_SHAPE
{
	cMSG_BASE_TYPE	base;

	WORD	wShape;
};

enum
{
	eCEW_USE_TITLE,
	eCEW_BUY_PIGEON_POST_ITEM,
	eCEW_USE_COUPLE_RING,
	eCEW_FIRE_FIRE_WORK,
	eCEW_CHANGE_ISPI_SETTING,
	eCEW_USE_BOTTOMLESS_BOX,
	eCEW_REMOVE_TITLE,
	eCEW_SELECT_GUILD_MARK,
	eCEW_DISCONNECT_BCS,
	eCEW_ASK_GUILD_BATTLE_SCHEDULE,
	eCEW_END_EVENT,
	eCEW_DISARM_TRAP,
	eCEW_CHANGE_MONSTER_INFO,
	eCEW_CLOSE_GUILD_HALL_LIST,

	eCEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_ITEM_COUNT	,
	eCEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_GOLD		,
	eCEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE				,

	eCEW_STORE_GUILD_INVENTORY_ITEM			,
	eCEW_WITHDRAW_GUILD_INVENTORY_ITEM		,
	eCEW_STORE_GUILD_INVENTORY_GOLD			,
	eCEW_WITHDRAW_GUILD_INVENTORY_GOLD		,
	eCEW_CHANGE_GUILD_INVENTORY_ITEM_PLACE	,
	eCEW_SELECT_GUILD_HALL,
	
	eCEW_TRY_TO_GUILD_POINT_BATTLE,
	eCEW_RETURN_TO_GUILD_HALL_BY_END_BATTLE,
	eCEW_SUMMON_GUILD_GUARDIAN,
	eCEW_UNSUMMON_GUILD_GUARDIAN,
	eCEW_FEED_ITEM_TO_GUILD_PET	,
	eCEW_RETURN_TO_THE_GUILD_HALL_BY_END_GUILD_POINT_BATTLE,

	eCEW_CHANGE_OPEN_GUILD_HALL_STATUS,
	eCEW_CLOSE_RELATED_PLACE_LIST,
	eCEW_SELECT_RELATED_PLACE,

	eCEW_ADD_RELATED_PLACE_OF_GUILD,
	eCEW_ASK_ITEM_LIST_IN_ITEM_PACK,
	eCEW_SELECT_ITEM_IN_PACK,

	eCEW_ACTIVATE_MINI_PET,
	eCEW_FEED_ITEM_TO_MINIPET,

	eCEW_INCREASE_MINIPET_SKILL_LEVEL,
	eCEW_USE_MINIPET_CHARGE_SKILL,
	eCEW_USE_BLOCK_LIGHT_POCKET,

	eCEW_USE_POLISHER,
	eCEW_RESET_MINIPET_NAME,

	eCEW_CHANGE_MINIPET_PLACE,
	eCEW_INCREASE_TITLE_LEVEL_BY_CH5,
	eCEW_CH5_RELEASE_ITEM_REVERSION,
	eCEW_CH5_RESET_ONE_STATE,
	eCEW_CH5_RESET_ONE_SKILL,
	eCEW_CH5_NORMAL_ITEM_PREFIX_LEVEL_UP,
	eCEW_CH5_UNIQUE_ITEM_PREFIX_LEVEL_UP,
	eCEW_CH5_NORMAL_ITEM_UPGRADE,
	eCEW_CH5_UNIQUE_ITEM_UPGRADE,
	eCEW_CH5_SELECT_ALLIGNMENT,
	
	eCEW_USE_BARD_TITLE,
	eCEW_ASK_DUEL_RECORD_INFO,
	eCEW_CANCEL_TRY_DUEL,
	eCEW_ASK_DUEL_RANKING,
	
	eCEW_CHANGE_MINIPET_SKILL_ON_OFF,
	eCEW_USE_MINIPET_AWAKEN_ITEM,
	eCEW_TRY_TO_GUILD_DUNGEN,
	eCEW_ACTIVE_GUILD_MASTER_GRACE,
	eCEW_DISJOINTING_ITEM,
	eCEW_FEED_ALL_ITEM_TO_MINIPET,

	eCEW_RESET_PET_NAME,
	eCEW_MIX_MINIPET,
	eCEW_RESTRAINT_MINIPET,
	eCEW_BUY_GOLD_BAR,
	eCEW_ETERNAL_ITEM_UPGRADE,
	eCEW_REALLY_ITEM_UPGRADE,
	eCEW_DRAG_ITEM_DIVIDE,
	eCEW_USE_SHOP,
	eCEW_USE_BANK,
	eCEW_COPY_PREFIX,
	eCEW_CUT_PREFIX,
	eCEW_MINIPET_FEED,
	eCEW_RESET_MINIPET_SKILL,
	eCEW_SET_MINIPET_TYPE,

	eCEW_TRANS_AVATAR_TO_GVG_SERVER,
	eCEW_TRANS_GUILD_TO_GVG_SERVER,
};

struct	CG_ETC_WORK
{
	cMSG_BASE_TYPE	base;

	WORD	wWork;
	WORD	awValue[12];
};

struct	CG_XTRAP_PACKET
{
	cMSG_BASE_TYPE	base;
	
	char	aBuffer[128];
};











struct	BW_COPY_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wLord;
	WORD			wTarget;
	WORD			wUpkeepTime;
	int				iHP;
	int				iX,iY;
};



struct	BW_MAKE_DUMMY
{
	cMSG_BASE_TYPE	base;

	WORD			wLord;
	WORD			wUpkeepTime,wAddHPPercentage;
	int				iX,iY;
};



struct	BW_MAKE_ILLUSION
{
	cMSG_BASE_TYPE	base;

	DWORD			dwLordHashCode;
	WORD			wLord;
	WORD			wUpkeepTime;
	WORD			wIsFirstTarget;
	WORD			wJob;
	WORD			wSkill;

	int				iHP;
	int				iX,iY;
};


struct	BW_DISPLACEMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wTarget;
	WORD			wIsRelease;
};

struct	BW_CHARMING
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wTarget,wTime;
	char			strCasterName[dNAME_LENGTH],strTargetName[dNAME_LENGTH];
	WORD			wIsRelease;
};

struct	BW_INTERVAL_SHOOTER
{
	cMSG_BASE_TYPE	base;

	WORD			wSkillIndex,wCaster,wTarget;
	DWORD			dwCasterNameHashCode,dwTargetNameHashCode;
};



struct	BW_EXIT_GAME
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	char			strName[dNAME_LENGTH];
};



struct	BW_READY_TO_EXIT
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	char			strName[dNAME_LENGTH];
};



struct	BW_DEATH_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial,wKiller;
	int				iZoneSerial;

	WORD			bf1IsReleaseSummon	:	1;
	WORD			bf1IsByKarma		:	1;
};

#define	eCEW_ACTION_SKILL_TO_GROUND	29324

struct	BW_MONSTER_TAME
{
	cMSG_BASE_TYPE	base;

	char			strCaster[dNAME_LENGTH];
	WORD			wTamedLevel,wTamedLoyalty,wTargetFieldSerial,wTargetZoneSerial;
};

struct BW_GENERATE_PET
{
	cMSG_BASE_TYPE	base;

	char			strTamer[dNAME_LENGTH];
	WORD			bf4PetIndex	:	4;
	WORD			bf1IsUnseal	:	1;
};

struct BW_GENERATE_SUMMON_BEAST
{
	cMSG_BASE_TYPE	base;

	char			strTamer[dNAME_LENGTH];
	int				iRemainHP;
	
	WORD			bf10Level	:	10;
	WORD			bf2Type		:	2;
	WORD			bf2Grade	:	2;
	WORD			bf1IsSummon	:	1;
	WORD			wX,wY;
};

struct BW_MAKE_SKILL_ZONE
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster;
	DWORD			dwCasterNameHashCode;
	WORD			wSkill,wSkillLevel;
};


struct BW_EXIT_GAME_FOR_MOVE_FIELD
{
	cMSG_BASE_TYPE		base;
	WORD				wSerial;
	char				strName[dNAME_LENGTH];
};









#define dRESULT_CONNECT_SUCCESS						0x000
#define dRESULT_CONNECT_FAIL						0x001
#define dRESULT_CONNECT_FULL						0x002

#define	dMAP_FILE_NAME_SIZE							64
#define	dIP_LENGTH									16

#define	dENCRYPT_TABLE_SIZE							93

struct SG_RESULT_CONNECT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	UTime			serverTime;
	WORD			wFieldSerial,wXorCode,wPacketSerial;
	DWORD			dwSeasonVariable;
	char			strMapName[dMAP_FILE_NAME_SIZE];
	BYTE			abEncryptTable[dENCRYPT_TABLE_SIZE];

	char			strGuildName[dGUILD_NAME_LENGTH];
};

#define dRESULT_JOIN_FIELD_SUCCESS									0x000
#define dRESULT_JOIN_FIELD_FULL										0x001
#define dRESULT_JOIN_FIELD_FAILED									0x002
#define dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA					0x003
#define dRESULT_JOIN_FIELD_FAILED_BY_EXIST_PLAYER					0x004
#define dRESULT_JOIN_FIELD_FAILED_BY_NOT_SECRET_DUNGEON_MEMBER		0x005
#define dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_CONNECT				0x006
#define dRESULT_JOIN_FIELD_FAILED_BY_TOO_MANY_CONNECTOR				0x007
#define dRESULT_JOIN_FIELD_FAILED_BY_TRY_CONNECT_TO_INCORRECT_FIELD	0x008
#define dRESULT_JOIN_FIELD_FAILED_BY_PROCESS_OFFLINE_TOURNAMENT		0x009

#define dLIMIT_RECEIVE_EXIST_ACTOR_COUNT_FOR_ROOKIE					50
#define dLIMIT_RECEIVE_EXIST_ACTOR_COUNT_FOR_JOINER					10

struct	SG_RESULT_JOIN
{
	cMSG_BASE_TYPE						base;

	WORD								wResult;
	WORD								wSerialInField;
	DWORD								dwEnchantedMask;

	DWORD								bf1IsGuildBattleField	: 1;
	DWORD								bf1IsOXQuizField		: 1;
	DWORD								bf1IsVillage			: 1;
	DWORD								bf4MinimapDisplayLevel	: 4;
	DWORD								bf3Team					: 3;
	DWORD								bf3OperatorLevel		: 3;
	DWORD								bf12DeathPeneltySecond	: 12;
	DWORD								bf1IsProgressEvent		: 1;
	DWORD								bf1IsReadyToSelectGuildMark	: 1;
	DWORD								bf1IsBossZone			: 1;
	DWORD								bf1IsSiegeWarfareField	: 1;
	DWORD								bf1IsGuildPointBattleField	: 1;	
	DWORD								bf1IsNetCafeUser		: 1;
	DWORD								bf1IsWaitOrgAvatarDataResult	: 1;

	WORD								bf4RemainGuildStategyPointCount	:	4;
	WORD								bf7DeathPenelty					:	7;
	WORD								bf1IsDuelField					:	1;
	WORD								m_bf3GreateGodGrace				:	3;

	WORD								m_wCurrentField;
	BYTE								m_bf4GuildHallLevel : 4;
	BYTE								m_bf4Weather		: 4;
	BYTE								m_bGuildHallShape;

	WORD								m_wRemainSecondForUseGateGlove;
	int									m_iXPos,m_iYPos;
	WORD								m_wLastVillage;

	WORD								m_wBoostExpSecondByTreasureMapEventInServer;
	WORD								m_wBoostGoldSecondByTreasureMapEventInServer;
	WORD								m_wPlayerBoostExpSecondByTreasureMapEvent;
	WORD								m_wPlayerBoostGoldSecondByTreasureMapEvent;
	WORD								m_wRedStonePoint,m_wReversalPowerTime;
	int									m_iHeavenRedStoneCount,m_iHellRedStoneCount,m_iRedDevilRedStoneCount;

	CUpkeepPlayerData					upkeepPlayerData;
	WORD								m_wServerSign;
	WORD								m_awRelatedPlaceOfGuild[dMAX_GUILD_VILLAGE_COUNT];
	WORD								m_awSkillCoolTime[c_iCoolTimeSkillCount];
};

struct	SG_FAILED_TO_JOIN
{
	cMSG_BASE_TYPE						base;

	WORD	wResult;
	char	strText[256];
};

struct	SG_PLAYER_INFO
{
	cMSG_BASE_TYPE						base;
	CPlayerSaveDataForClientDefine		playerData;

	WORD								m_wExtraInventorySize;
	BYTE								m_aCompleteQuest[dQUEST_BUFFER_SIZE];
	BYTE								m_aCompleteQuest2[dQUEST_BUFFER_EXTEND_SIZE];
	BYTE								m_aCompleteArbeit[dARBEIT_BUFFER_SIZE];
	BYTE								m_aCompleteArbeit2[dARBEIT_BUFFER_SIZE];
	cMiniPetCommon						m_aMiniPet[c_iMiniPetCount];
	WORD								m_awItemCoolTime[dITEM_COOL_TIME_BUFFER_SIZE];

	CItemDefine							m_aExtraInventory[dEXTRA_INVENTORY_SIZE];
	WORD								m_wBonusStatePoint;
	WORD								m_awQuestCountDownTimer[dMAX_PROCESS_QUEST_COUNT];
	WORD								m_wProcessChapterIndex;
	WORD								m_wProcessHighQuestIndex;

	UINT								m_bf5ActiveMiniPet	:	5;
	UINT								m_bf5ActiveMiniPet2	:	5;
	UINT								m_reversalFrameYear	:	5;
	UINT								m_reversalFrameMonth:	4;
	UINT								m_reversalFrameDay	:	5;
	UINT								m_reversalFrameHour	:	5;
	UINT								m_reversalFrameMinute:	6;


	WORD								m_bf4CH5StateBonusCount	:	4;
	WORD								m_bf4CH5SkillBonusCount	:	4;
	WORD								m_bf4CH5HPBonusCount	:	4;
	WORD								m_bf4CH5CPBonusCount	:	4;

	WORD								m_bf4CH5PeneltyHPBonusCount		:	4;
	WORD								m_bf4CH5PeneltyCPBonusCount		:	4;

	WORD								m_bf3GreateGodGrace				:	3;
	WORD								m_bf3GuildDungeonPlayTime				:	3;

	UINT								m_bf4EatFireMiniPetCount	:	4;
	UINT								m_bf4EatWaterMiniPetCount	:	4;
	UINT								m_bf4EatWindMiniPetCount	:	4;
	UINT								m_bf4EatEarthMiniPetCount	:	4;
	UINT								m_bf4EatLightMiniPetCount	:	4;
	UINT								m_bf4EatDarkMiniPetCount	:	4;
	UINT								m_bf4RebirthCount			:	4;
	UINT								m_bf4UseSealSphereOfDawnCount:	4;
};

struct	SG_SUCCESS_TO_JOIN_GAME
{
	cMSG_BASE_TYPE						base;
};

struct	SG_BCS_INFO
{
	cMSG_BASE_TYPE		base;

	WORD				wIndexForBCS;
	char				strBCSIp[dIP_LENGTH];
};



struct	SG_UPDATE_PLAYER_STATUS_DATA
{
	cMSG_BASE_TYPE				base;

};


struct	SG_JOIN_PLAYER
{
	cMSG_BASE_TYPE				base;

	CSimplePlayerInfo			actorInfo;

	WORD						bf1IsByDeath	:	1;
	WORD						bf1IsByTeleport	:	1;
};

#define	dEXIST_ACTOR_COUNT_FOR_EASL	100



struct	SG_MINIMUM_ACTOR_INFO_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	CMinimumActorInfo			aInfoList[dEXIST_ACTOR_COUNT_FOR_EASL];
};

#define	dEXIST_OPERATOR_COUNT_IN_FIELD	20



struct	SG_OPERATOR_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	COperatorInfo				aOperatorList[dEXIST_OPERATOR_COUNT_IN_FIELD];
};




struct	SG_REFIT_ACTORS_POSITION
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	CActorInfoForOuterActor		aInfoList[20];
};



struct	SG_EVENT_NPC_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	cEventNpcInfo				aInfoList[dEXIST_ACTOR_COUNT_FOR_EASL];
};



struct	SG_SIMPLE_ACTOR_INFO_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	char						aBuffer[256];
};



struct	SG_SIMPLE_ACTOR_INFO
{
	cMSG_BASE_TYPE				base;

	char						aBuffer[256];
};



struct	SG_VERY_SIMPLE_ACTOR_INFO_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	char						aBuffer[256];
};



struct	SG_REGEN_MONSTER
{
	cMSG_BASE_TYPE				base;

	CSimpleMonsterInfo			monsterInfo;
};


struct	SG_PLAYER_OUT
{
	cMSG_BASE_TYPE				base;

	WORD						wSerial;
};








struct	SG_SUMMON_BEAST_INFO
{
	cMSG_BASE_TYPE				base;
	
	DWORD						bf4Index	:	4;
	DWORD						bf10Serial	:	10;
	DWORD						bf10Level	:	10;
	DWORD						bf2Grade	:	2;
	DWORD						bf2Type		:	2;
	DWORD						bf1IsUpgrade:	1;
};

struct	SG_RIDING_DOG
{
	cMSG_BASE_TYPE				base;

	WORD						wPosX,wPosY;
	WORD						bf2DogGrade	:	2;
	WORD						bf1IsRiding	:	1;
};





struct	SG_CHANGE_PET_COMMAND
{
	cMSG_BASE_TYPE				base;
	
	WORD						bf2PetIndex		:	2;
	WORD						bf4Command		:	4;
	WORD						bf10ProtectActor:	10;
	WORD						wX,wY;
};

struct	SG_PET_INFO
{
	cMSG_BASE_TYPE				base;
	
	WORD						bf4PetIndex :	4;
	WORD						bf1IsTame	:	1;
	WORD						wPetSerial;
	cPetDefine					petInfo;
	WORD						wMinAttackPower,wMaxAttackPower;
	int							iDefensePower;
};

struct	SG_MAKE_BREEDING_RECORD_BOOK
{
	cMSG_BASE_TYPE				base;
	WORD						wPetIndex;
};

struct	SG_SYNC_PET_INFO
{
	cMSG_BASE_TYPE				base;

	DWORD	bf29MaxHP		:	29;
	DWORD	bf2PetIndex		:	2;
	DWORD	bf1IsSummonBeast:	1;
	WORD	wStrength,wIntelligence;
	WORD	wRemainHP,wMinAttackPower,wMaxAttackPower;
	int		iDefensePower;
};

struct	SG_PET_WAIT_COMMAND
{
	cMSG_BASE_TYPE				base;

	WORD	wPetIndex;
};







class	cP_HIT_INFO
{
public:
	WORD			m_wPhysicalDamage,m_wMagicDamage;
	DWORD			m_dwResultField;
};

typedef struct 
{
	WORD isRun			:	1;
	WORD isContinue		:	1;
	WORD wPPS			:	14;
} tsMoveInfo;



struct	SG_MOVE
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wPosX,wPosY;
	WORD			wDestPosX,wDestPosY;
	tsMoveInfo		moveInfo;
};

enum
{
	eMOVE_RESULT_SUCCESS					,
	eMOVE_RESULT_MOVE_SLAVE					,
	eMOVE_RESULT_FAILED_BY_ACTION			,
	eMOVE_RESULT_FAILED_BY_BLOCKED_POS		,
	eMOVE_RESULT_FAILED_BY_INCORRECT_POS	,
	eMOVE_RESULT_FAILED_BY_ICY_STALAGMITE	,
	eMOVE_RESULT_FAILED_BY_STATUS			,
};



struct	SG_MOVE_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			isRun			: 1;
	WORD			wPPS			: 15;

	WORD			bf14HP				:	14;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;

	short			sCP;
};



struct	SG_MOVE_FAILED
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsRun	:	1;
	WORD			bf10PPS		:	10;
	WORD			bf5Result	:	5;
	WORD			wX,wY;
};



struct	SG_CORRECT_PLAYER_POSITION
{
	cMSG_BASE_TYPE	base;

	WORD			bf15Serial			:	15;
	WORD			bf1IsInnerTeleport	:	1;
	WORD			wPosX,wPosY;
};



struct	SG_STOP
{
	cMSG_BASE_TYPE	base;

	WORD			bf12Serial				: 12;
	WORD			bf3Direct				: 3;
	WORD			bf1IsChangeOwnerDirect	: 1;

	WORD			wPosX,wPosY;
};

#define	dMAX_UPDATE_ACTOR_POS_COUNT		50

struct	SG_OUTER_ACTORS_POS_UPDATE
{
	cMSG_BASE_TYPE				base;

	WORD						wCount;
	CMinimumActorInfo			aInfoList[dMAX_UPDATE_ACTOR_POS_COUNT];
};



struct	SG_SET_MOVE_ABILITY
{
	cMSG_BASE_TYPE	base;

	WORD			isRun			: 1;
	WORD			wPPS			: 15;
};



struct	SG_ACTION_TO_ACTOR
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial		:	11;
	DWORD			bf11TargetSerial:	11;
	DWORD			bf7Level		:	7;
	DWORD			bf1FPS			:	1;


	WORD			bf11Skill		:	11;
	WORD			bf5FPS			:	5;


	WORD			wRange;
	WORD			wPosX,wPosY;

	UINT			iPhysicalDamage;
	UINT			iMagicDamage;

	DWORD			dwResultField;
};

#define		dMAX_CONTINUOUS_TARGET_COUNT			12

class	cSimpleDamageInfo
{
public:
	WORD			wActor;
	
	UINT			iPhysicalDamage;
	UINT			iMagicDamage;
	DWORD			dwResultField;
};

class	cContinuousDamageInfo	: public	cSimpleDamageInfo
{
public:
	DWORD			m_bf6ContinuousShotCount	:	6;
	DWORD			m_bf6MissCount				:	6;
	DWORD			m_bf6CriticalCount			:	6;
	DWORD			m_bf6DoubleCriticalCount	:	6;
	DWORD			m_bf6HardBlowCount			:	6;
	DWORD			:0;
};
struct	SG_ACTION_TO_GROUP_STICKED_BIT
{
	cMSG_BASE_TYPE		base;

	DWORD				bf11Serial				:	11;
	DWORD				bf11Skill				:	11;
	DWORD				bf6FPS					:	6;
	DWORD				bf4TargetCount			:	4;

	WORD				bf7SkillLevel			:	7;
	WORD				:0;
	WORD				wPosX,wPosY,wRange;

	cContinuousDamageInfo	aData[dMAX_CONTINUOUS_TARGET_COUNT];
};

#define	dACTION_TO_GROUP_TARGET_COUNT	32

struct	SG_ACTION_TO_GROUP
{
	cMSG_BASE_TYPE		base;

	DWORD				bf11Serial				:	11;
	DWORD				bf11Skill				:	11;
	DWORD				bf6FPS					:	6;
	DWORD				bf4TargetCount			:	4;


	DWORD				bf7ContinuousShotCount	:	7;
	DWORD				bf7MissCount			:	7;
	DWORD				bf7CriticalCount		:	7;
	DWORD				bf7DoubleCriticalCount	:	7;
	DWORD				bf1MiniPetCharge1		:	1;
	DWORD				bf1MiniPetCharge2		:	1;
	DWORD				:0;

	WORD				bf7SkillLevel			:	7;
	WORD				bf7HardBlowCount		:	7;
	WORD				:0;
	WORD				wPosX,wPosY,wRange;

	cSimpleDamageInfo	aData[dACTION_TO_GROUP_TARGET_COUNT];
};



struct	SG_ACTION_TO_GROUND
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wPosX,wPosY;
	WORD			wDestPosX,wDestPosY;
	WORD			wLevel;
	WORD			wFPS;
	WORD			wRange;

	WORD			bf12Skill			:	12;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;
};



struct	SG_ACTION_QUICK
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wLevel;

	WORD			bf12Skill			:	12;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;
};



struct	SG_BASIC_ATTACK
{
	cMSG_BASE_TYPE	base;

	WORD			wPosX,wPosY;

	WORD			bf6FPS				:	6;
	WORD			bf7Level			:	7;
	WORD			bf1Skill			:	1;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;

	DWORD			bf11Serial			:	11;
	DWORD			bf11TargetSerial	:	11;
	DWORD			bf10Skill			:	10;

	UINT			iMagicDamage	;
	UINT			iPhysicalDamage	;

	DWORD			dwResultField;
};



struct	SG_QUICK_ACTION
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial	:	11;
	DWORD			bf7Level	:	7;	
	DWORD			bf4Direct	:	4;
	DWORD			bf6FPS		:	6;


	WORD			bf11Skill			:	11;
	WORD			bf1MiniPetCharge1	:	1;
	WORD			bf1MiniPetCharge2	:	1;

	WORD			wPosX,wPosY;
};



enum
{
	eAR_NOT_LEARNED_SKILL			,
	eAR_OK							,
	eAR_BUSY						,
	eAR_CAN_NOT_CLOSE_ATTACK_ON_HILL,
	eAR_LOW_CP						,
	eAR_TOO_DISTANCE				,
	eAR_IS_CAN_NOT_ATTACK_TARGET	,
	eAR_FAILED						,
	eAR_IS_UNDER_TARGET_ULTIMATE_BARRIER	,
	eAR_REQUIRE_WEAPON				,
	eAR_REQUIRE_SHIELD				,
	eAR_REQUIRE_BULLET				,
	eAR_NOT_EXIST_SKILL				,
	eAR_TARGET_DEATH				,
	eAR_IS_INREGULAR_SKILL			,
	eAR_NOT_APT_USED_SKILL_TO_TARGET,
	eAR_REQUIRE_PET					,	
	eAR_REQUIRE_SUMMON_BEAST		,
	eAR_ALREADY_OPEN_ARCA			,
	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_CONFUSE	,
	eAR_CAN_NOT_USE_HIGH_LEVEL_SKILL_BY_BERSERK	,
	eAR_CAN_NOT_USE_SKILL_TO_TARGET_OBJECT,
	eAR_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE,
	eAR_CAN_NOT_APT_SKILL_TO_THIS_STATUS,
	eAR_BLOCKED_OBJECT_EXIST,

	eAR_REQUIRE_POTION				,
	eAR_REQUIRE_FLOWER				,
	eAR_REQUIRE_CANDY				,
	eAR_REQUIRE_DURG				,
	eAR_REQUIRE_TARGET_SKILL		,

	eAR_IS_COOL_TIME				,

	eAR_BORDER_SPECIAL_SKILL		,
	eAR_SET_SPECIAL_SKILL_FIELD		,
	eAR_CONTINUOUS_ATTACK_BY_VALUE	,
	eAR_LAND_MARKER					,
	eAR_INTERVAL_SHOOTER			,
	eAR_BUNSHINE_ATTACK				,
	eAR_SCIMITAR_CUTTING			,
	eAR_BOUNCING_LINEAR				,
	eAR_ICY_STALAGMITE				,
	eAR_DOUBLE_TARGET				,
	eAR_CAN_NOT_USE_IN_DUEL_FIELD_SKILL	,
	eAR_GROUP_CONTINUOUS_BY_BIT		,
	eAR_BOUNCING_MISSILE				,
	eAR_WIDE_MISSILE				,
};



struct	SG_USE_SKILL_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	short			sRemainCP;
};



struct	SG_CONTINUOUS_HIT_BY_CHARACTER
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial				:	11;
	DWORD			bf11TargetSerial		:	11;
	DWORD			bf6FPS					:	6;
	DWORD			bf4MissCount			:	4;
												

	DWORD			bf7Level				:	7;
	DWORD			bf5AttackCount			:	5;
	DWORD			bf4CriticalCount		:	4;
	DWORD			bf4DoubleCriticalCount	:	4;
	DWORD			bf1IsCrushBlow			:	1;
	DWORD			bf11Skill				:	11;
													

	WORD			wPosX,wPosY;
	int				iPhysicalDamage,iMagicDamage;
	WORD			wMinPhysicalDamage,wMinMagicDamage;
	
	DWORD			bf4DancingBlockCount	:	4;
	DWORD			bf4NoActionBlockingCount:	4;
	DWORD			bf4BlockingCount		:	4;
	DWORD			bf1IsInstanceKill		:	1;
	DWORD			bf1IsAttackToShakle		:	1;
	DWORD			bf4HardBlowCount		:	4;
	DWORD			:	0;
};



struct	SG_CONTINUOUS_HIT_BY_VALUE
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial				:	11;
	DWORD			bf11TargetSerial		:	11;
	DWORD			bf6FPS					:	6;
	DWORD			bf4MissCount			:	4;


	DWORD			bf7Level				:	7;
	DWORD			bf5AttackCount			:	5;
	DWORD			bf4CriticalCount		:	4;
	DWORD			bf4DoubleCriticalCount	:	4;
	DWORD			bf1IsCrushBlow			:	1;
	DWORD			bf11Skill				:	11;

												
	WORD			wPosX,wPosY;
	int				iPhysicalDamage,iMagicDamage;
	WORD			wMinPhysicalDamage,wMinMagicDamage;

	DWORD			bf4DancingBlockCount	:	4;
	DWORD			bf4NoActionBlockingCount:	4;
	DWORD			bf4BlockingCount		:	4;
	DWORD			bf1IsInstanceKill		:	1;
	DWORD			bf1IsAttackToShakle		:	1;
	DWORD			bf4HardBlowCount		:	4;
	DWORD			:	0;
};




struct	SG_BUNSHINE_ATTACK
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial				:	11;
	DWORD			bf11TargetSerial		:	11;
	DWORD			bf6FPS					:	6;
	DWORD			bf4CriticalCount		:	4;


	DWORD			bf7Level				:	7;
	DWORD			bf5AttackCount			:	5;
	DWORD			bf4MissCount			:	4;
	DWORD			bf4DoubleCriticalCount	:	4;
	DWORD			bf1IsCrushBlow			:	1;
	DWORD			bf11Skill				:	11;


	WORD			wPosX,wPosY;
	int				iPhysicalDamage,iMagicDamage;
	WORD			wMinPhysicalDamage,wMinMagicDamage;
	DWORD			bf4DancingBlockCount	:	4;
	DWORD			bf4NoActionBlockingCount:	4;
	DWORD			bf4BlockingCount		:	4;
	DWORD			bf1IsInstanceKill		:	1;
	DWORD			bf1IsAttackToShakle		:	1;
	DWORD			bf4HardBlowCount		:	4;
	DWORD		:0;					
};


struct	SG_SCIMITAR_CUTTING
{
	cMSG_BASE_TYPE	base;

	WORD			wPosX,wPosY;
	WORD			wRange;
	int				iPhysicalDamage,iMagicDamage;

	DWORD			bf11Serial				:	11;
	DWORD			bf11TargetSerial		:	11;
	DWORD			bf6FPS					:	6;
	DWORD			bf4MissCount			:	4;


	DWORD			bf7Level				:	7;
	DWORD			bf5AttackCount			:	5;
	DWORD			bf4CriticalCount		:	4;
	DWORD			bf4DoubleCriticalCount	:	4;
	DWORD			bf1IsCrushBlow			:	1;
	DWORD			bf11Skill				:	11;


	WORD			wMinPhysicalDamage,wMinMagicDamage;
	DWORD			bf4DancingBlockCount	:	4;
	DWORD			bf4NoActionBlockingCount:	4;
	DWORD			bf4BlockingCount		:	4;
	DWORD			bf1IsInstanceKill		:	1;
	DWORD			bf1IsAttackToShakle		:	1;
	DWORD			bf4HardBlowCount		:	4;
	DWORD		:0;	
};



struct	SG_HIT_CHANED_DAMAGE
{
	cMSG_BASE_TYPE	base;


	DWORD			bf5AttackCount			:	5;
	DWORD			bf5MissCount			:	5;
	DWORD			bf5CriticalCount		:	5;
	DWORD			bf5DoubleCriticalCount	:	5;
	DWORD			bf5DancingBlockCount	:	5;
	DWORD			bf4NoActionBlockingCount:	4;

	DWORD			bf1IsCrushBlow			:	1;
	DWORD			bf1IsAttackToShakle		:	1;
	DWORD			bf1IsInstanceKill		:	1;


	DWORD			bf11Skill				:	11;
	DWORD			bf5BlockingCount		:	5;
	DWORD			bf5ShotGap				:	5;
	DWORD			bf11TargetSerial		:	11;


	WORD			bf5HardBlowCount				:5;
	WORD			:0;
	int				iPhysicalDamage,iMagicDamage;
	WORD			wMinPhysicalDamage,wMinMagicDamage;
	WORD			wCaster;
};



struct	SG_LAND_MARKER
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wPosX,wPosY;
	WORD			wTargetPosX,wTargetPosY;
	WORD			wSkill,wLevel;
	BYTE			bFPS,bShotCount;
};


struct	SG_STRIKE_INTERVAL_SHOOTER
{
	cMSG_BASE_TYPE	base;

	WORD			wTargetSerial;
	WORD			wPosX,wPosY;
	WORD			wSkill;
	WORD			wShotCount;
	int				iPhysicalDamage,iMagicDamage;
};

struct	SG_CAST_INTERVAL_SHOOTER
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wPosX,wPosY;
	WORD			wSkill,wLevel;
	BYTE			bFPS,bShotCount;
};

class	cWideHealE
{
public:
	WORD			wActor;
	WORD			wHealPoint;
};

struct	SG_WIDE_HEAL
{
public:
	cMSG_BASE_TYPE	base;

	WORD			wCount,wSkill;

	cWideHealE		aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

class	cWIDE_AREA_DAMAGE_E
{
public:
	WORD			wActor;
	WORD			wPhysicalDamage,wMagicDamage;
	DWORD			dwResultField;
};

class	CWideAreaDamageInfo
{
public:
	cWIDE_AREA_DAMAGE_E	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};



struct	SG_WIDE_AREA_DAMAGE
{
	cMSG_BASE_TYPE		base;

	WORD				bf6Count		: 6;
	WORD				bf1IsDamagePool	: 1;
	WORD				bf7FPS			: 7;

	DWORD				bf12Caster		: 12;
	DWORD				bf12Skill		: 12;
	DWORD				bf8Level		: 8;

	short				sRemainCP;

	cWIDE_AREA_DAMAGE_E	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

struct	SG_TITLE_EFFECT
{
	cMSG_BASE_TYPE		base;

	WORD				wTitle;
	WORD				wTargetCount;
	WORD				awTargetList[100];
};

struct	SG_XTRAP_PACKET
{
	cMSG_BASE_TYPE		base;
	
	char				aBuffer[128];
};

struct SG_UPDATE_PACKET_ENCRYT_CODE
{
	cMSG_BASE_TYPE	base;

	WORD			wNewTable;
	BYTE			abEncryptTable[dENCRYPT_TABLE_SIZE];
};

class	cWideAideAttackResultE
{
public:
	WORD				m_bf12Target	:	12;
	WORD				m_bf1IsHit		:	1;
};

struct	SG_WIDE_AID_ATTACK_RESULT
{

	cMSG_BASE_TYPE			base;

	DWORD					bf6Count		: 6;
	DWORD					bf12Skill		: 12;
	DWORD					bf12Caster		: 12;

	short					sRemainCP;

	cWideAideAttackResultE	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

struct	SG_GUARDIAN_POST
{
	cMSG_BASE_TYPE		base;

	DWORD				bf12Caster	: 12;
	DWORD				bf12Skill	: 12;
	DWORD				bf8Level	: 8;
	WORD				wRegSerial;
	WORD				wX,wY;
	WORD				wImage;
	WORD				wTargetCount;
	short				sCP;

	cWIDE_AREA_DAMAGE_E	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

struct	SG_FIRST_AID
{
	cMSG_BASE_TYPE		base;

	WORD				wSerial;
	int					iFirstAidHP;
};

struct	SG_REVIVE
{
	cMSG_BASE_TYPE		base;

	WORD				wSerial;
};

struct	SG_UPDATE_BOOST_COUNT
{
	cMSG_BASE_TYPE		base;

	WORD				bf3UseBoostCount		:	3;
	WORD				bf5RemainPet1BoostCount	:	5;
	WORD				bf5RemainPet2BoostCount	:	5;
};


#define	dMAX_TARGET_FOR_DAMAGE_TO_ACTOR_BY_KARMA	64

class	CDamageToActorByKarmaE
{
public:
	WORD			wActor,wDamage;
};



struct	SG_DAMAGE_TO_ACTOR_BY_KARMA
{
	cMSG_BASE_TYPE			base;

	WORD					wCount;
	WORD					wDamageType;

	CDamageToActorByKarmaE	aData[dMAX_TARGET_FOR_DAMAGE_TO_ACTOR_BY_KARMA];
};

#define	dMAX_TARGET_FOR_WIDE_AREA_EFFECT	32



struct	SG_GET_TITLE
{
	cMSG_BASE_TYPE			base;

	WORD					wSlot;
	BYTE					bTitle,bLevel;
};



struct	SG_REMOVE_TITLE
{
	cMSG_BASE_TYPE			base;

	WORD					wSlot;
	BYTE					bTitle,bLevel;
};

struct	SG_START_COUNTDOWN
{
	cMSG_BASE_TYPE			base;
	WORD					wTime;
};


struct	SG_BEGIN_QUIZ_EVENT
{
	cMSG_BASE_TYPE			base;
	WORD	wType;
	WORD	wCommerCount;

};

struct	SG_EVENT_TIME_UPDATE
{
	cMSG_BASE_TYPE	base;

	DWORD	adwEventTimeActiveBuffer[dEVENT_TIME_BUFFER_SIZE];
	int	iCount;
};


struct	SG_ANNOUNCE_QUIZ_EVENT
{
	cMSG_BASE_TYPE			base;

	WORD	wYear,wMonth,wDay,wHour,wMinute;
	WORD	wQuizGateField,wIsTest;
	WORD	wType;
	char	strQuizEventName[128];
};

struct	SG_OX_QUIZ_RESULT
{
	cMSG_BASE_TYPE			base;

	WORD					wSolution;
	WORD					wLoserCount;
	WORD					wSurvivorCount;
};

struct	SG_WORD_QUIZ_RESULT
{
	cMSG_BASE_TYPE			base;

	WORD					wLoserCount;
	WORD					wSurvivorCount;
	WORD					wTop10Count;
	WORD					awTop10Score[10];
	char					strSolution[16];
	char					strTop10List[dNAME_LENGTH*10];
};

#define	dQUIZ_EVENT_LOSER_SLOT_SIZE	128

struct	SG_QUIZ_EVENT_LOSER_LIST
{
	cMSG_BASE_TYPE			base;

	WORD					bf8LoserCount		:	8;
	WORD					bf4QuizType			:	4;
	WORD					bf1IsFinishEvent	:	1;

	WORD					wRound;

	WORD					awLoserList[dQUIZ_EVENT_LOSER_SLOT_SIZE];
};


struct	SG_QUIZ_EVENT_QUESTION
{
	cMSG_BASE_TYPE			base;
	WORD					wWaitCount;
	WORD					bf10QuestionCounter	:	10;
	WORD					bf4QuizType			:	4;

	char					strQuestion[256];
};

struct	SG_END_QUIZ_EVENT
{
	cMSG_BASE_TYPE			base;

	WORD	wIsReturnToVillage;
	WORD	wWinnerCount,wComerCount;
	WORD	wType;
};

struct	SG_CANCEL_QUIZ_EVENT
{
	cMSG_BASE_TYPE			base;

	WORD	wQuizType;
	WORD	wYear,wMonth,wDay,wHour,wMinute;
};

struct	SG_OX_QUIZ_SURVIVOR_COUNT
{
	cMSG_BASE_TYPE			base;

	WORD					wSurvivorCount,wLoserCount;
};

struct	SG_OX_QUIZ_LOSER_REVIVE_BATTLE
{
	cMSG_BASE_TYPE			base;
	
	WORD					wBattleCount,wWinnerCount,wRevivorCount;
};

struct	SG_OX_QUIZ_WINNER_LIST
{
	cMSG_BASE_TYPE			base;

	WORD					wWinnerCount;
	WORD					awWinnerList[100];
};

enum
{
	eBCIR_SUCCESS,
	eBCIR_FAILED,
	eBCIR_LOW_CARROT,
	eBCIR_FULL_INVENTORY,
};

struct	SG_BUY_CARROT_SHOP_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	int				iRemainCarrotCount;
	CItemDefine		item;
};

class	cFieldDoorStatus
{
public:
	WORD	bf10Area			:	10;
	WORD	bf1IsSleep			:	1;
	WORD	bf1IsHide			:	1;
	WORD	bf1IsLocked			:	1;
	WORD	bf1IsTrap			:	1;
	WORD	bf1IsClose			:	1;
	WORD	bf1IsActiveByKarma	:	1;
};

struct	SG_DOORS_STATUS_IN_FIELD
{
	cMSG_BASE_TYPE	base;

	WORD				wDoorCount;
	cFieldDoorStatus	aDoors[dMAX_DOOR_IN_MAP];
};

struct	SG_CHANGE_DOOR_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD	bf10Area			:	10;
	WORD	bf1IsSleep			:	1;
	WORD	bf1IsHide			:	1;
	WORD	bf1IsLocked			:	1;
	WORD	bf1IsTrap			:	1;
	WORD	bf1IsClose			:	1;
	WORD	bf1IsActiveByKarma	:	1;
};



enum
{
	eDWM_UNLOCK_BY_SKILL,
	eDWM_UNLOCK_BY_KEY,
	eDWM_UNLOCK_BY_SKILL_FAILED,
	eDWM_UNLOCK_BY_ATTACK,
	eDWM_UNLOCK_BY_KARMA,

	eDWM_DISARM,
	eDWM_DISARM_FAILED,
	eDWM_DISARM_FAILED_AND_EXPLOSION_TRAP,

	eDWM_IS_LOCKED,
	eDWM_IS_UNLOCKED,
	eDWM_IS_CLOSED,
	eDWM_IS_OPENED,
	eDWM_IS_DISARMED,
	eDWM_IS_SLEEP_HIDE,

	eDWM_IS_HIDE_DOOR,
	eDWM_IS_ARMED_TRAP,

	eDWM_DO_YOU_WANT_UNLOCK_DOOR_USE_KEY,
	eDWM_CAN_NOT_UNLOCK_DOOR_BY_NOT_EXIST_AVAIL_KEY,
	eDWM_NOT_EXIST_AVAIL_KEY_OR_SKILL_FOR_UNLOCK_DOOR,
	eDWM_CAN_NOT_UNLOCK_DOOR_BY_LOW_UNLOCK_LEVEL,

	eDWM_TRY_DISARM_FAILED_BY_LOW_CP,
	eDWM_TRY_DISARM_FAILED_BY_BUSY,
	eDWM_TRY_DISARM_FAILED_BY_LOW_SKILL_LEVEL,
	eDWM_CAN_NOT_DISARM_TRAP,
	eDWM_EXPLOSION_TRAP_BY_CAN_NOT_DETECT_TRAP,
	eDWM_EXPLOSION_TRAP_BY_ATTACK,
	eDWM_TRY_DISARM_BY_SKILL,

	eDWM_TRY_UNLOCK_BY_SKILL,
	eDWM_TRY_UNLOCK_FAILED_BY_LOW_CP,
	eDWM_TRY_UNLOCK_FAILED_BY_BUSY,

	eDWM_OPEN,
	eDWM_IS_REQURE_PARTY,
	eDWM_IS_FULL_SECRET_DUNGEON,
	eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON,
	eDWM_ALREADY_USE_SECRET_DUNGEON,
	eDWM_FAILED_TO_ENTER_THE_SECRET_DUNGEON_BY_LIMIT_LEVEL,
	eDWM_DUMMY,
	eDWM_CAN_NOT_DESTROY_DOOR,
};

struct	SG_DOOR_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wArea;
	WORD			wKey,wSkill;
	WORD			wMessage;

	char			strCasterName[dNAME_LENGTH];
};

struct	SG_BANNER_TEXT
{
	cMSG_BASE_TYPE	base;

	WORD			wBannerSerial;
	char			strText[dBANNER_COMMENT_LENGTH];
};

struct	SG_BANNER_INFO
{
	cMSG_BASE_TYPE		base;

	cBannerSimpleInfo	bannerInfo;
};

struct	SG_BANNER_LIST
{
	cMSG_BASE_TYPE	base;

	BYTE			abBuffer[512];
};

struct	CG_USE_SOUND_OF_LEADERS_BELL
{
	cMSG_BASE_TYPE	base;

	int		iItemSlot;
	WORD	wIsDisplayName;
	WORD	wTest;
	char	strMessage[64];
};

enum
{
	eSoundOfLeadersBellWork_begin,
	eSoundOfLeadersBellWork_move_field,
	eSoundOfLeadersBellWork_end,
};

struct	SG_SOUND_OF_LEADERS_BELL
{
	cMSG_BASE_TYPE	base;

	int		iUpkeepSecond;
	int		iBoostExp,iBoostItemDropChance;
	WORD	wWork,wIsDisplayNameWithSoundOfLeadersBell;

	char	strCaster[dNAME_LENGTH];
	char	strMessage[128];
};

struct	SG_BOOST_POWER_BY_EVENT
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wSecond;
	char			strName[dNAME_LENGTH];
};



enum
{
	eGSM_TOUCH_EVENT_AREA_SUCCESS,		
	eGSM_TOUCH_EVENT_AREA_FAILED,		
};

struct	SG_GAME_SERVER_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD	wMessage;
};

struct	SG_STRIKE_LIGHTNING_WINDER
{
	cMSG_BASE_TYPE	base;

	WORD	wSkill,wCaster,wTarget,wPhysicalDamage,wMagicDamage,wAbsorbDamage,wRemainTargetHP;
	DWORD	dwResultField;
};
struct	SG_STRIKE_MULTI_MISSILE
{
	cMSG_BASE_TYPE	base;

	WORD	wSkill,wCaster,wTarget;
	DWORD	dwPhysicalDamage,dwMagicDamage;
	DWORD	dwResultField;
	WORD	wAbsorbDamage,wRemainTargetHP;
};


struct	SG_WIDE_AREA_EFFECT
{
	cMSG_BASE_TYPE		base;

	WORD				wCount,wSkill;

	WORD				aData[dMAX_TARGET_FOR_WIDE_AREA_EFFECT];
};




struct SG_SCREEN_EVENT
{
	cMSG_BASE_TYPE		base;

	WORD				waValue[6];
};



struct	SG_KILL
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			bf12DeathPeneltySecond	:	12;
	WORD			bf1IsInstanceKill		:	1;
	WORD			bf1IsReleaseSummon		:	1;
};



struct	SG_UPDATE_EXP
{
	cMSG_BASE_TYPE	base;

	int				iExp;
	WORD			wSkillExp;
	WORD			bf10Level		:	10;
	WORD			bf1IsRestore	:	1;
};



struct	SG_UPDATE_PET_EXP
{
	cMSG_BASE_TYPE	base;

	WORD			bf10Level	:	10;
	WORD			bf4PetIndex	:	4;
	int				iExp;
};

struct	SG_CHANGE_PET_NAME
{
	cMSG_BASE_TYPE	base;

	WORD			wPetIndex;
	WORD			wIsMiniPet;
	char			strPetName[dPET_NAME_LENGTH+1];
	int				iResetItemIndex;
};

struct	SG_PARTING_WITH_PET
{
	cMSG_BASE_TYPE	base;

	WORD			wPetIndex;
	WORD			wIsMiniPet;

	WORD			bf1IsRequireMessage	:	1;
};

enum
{
	eUMIM_CREATE,
	eUMIM_UPDATE,
	eUMIM_FEEDING,
	eUMIM_INCREASE_SKILL_LEVEL,
	eUMIM_SEAL_MINIPET,
	eUMIM_UNSEAL_MINIPET,
	eUMIM_USE_POLISHER,
	eUMIM_RESET_MINIPET_NAME,
	eUMIM_EXPIRED,
	eUMIM_ACTIVATE,
	eUMIM_DISACTIVATE,
	eUMIM_FEED_AWAKEN_ITEM,
	eUMIM_MIXED,
	eUMIM_RESET_SKILL,
	eUMIM_FAIL_RESET_SKILL,
	eUMIM_CHANGE_TYPE,
};
struct	SG_MINI_PET_INFO
{
	cMSG_BASE_TYPE	base;

	cMiniPetCommon	miniPet;
	WORD			wPetIndex;
	WORD			wMethod;
	WORD			wIncreaseExp;

	DWORD			bf5ActivePet	:	5;
	WORD			bf5ActivePet2	:	5;
	DWORD			bf1IsEvolution	:	1;
	DWORD			bf4FeedType		:	4;
	DWORD			bf1Charge1		:	1;
	DWORD			bf1Charge2		:	1;
	DWORD			bf1IsUseMiniPetAwakenItem50		:	1;
	DWORD			bf1IsUseMiniPetAwakenItem100	:	1;
};

struct	SG_UPDATE_BASIC_INFORMATION
{
	cMSG_BASE_TYPE	base;

	WORD			wHP;
	short			sCP;
	WORD			wSerial;
};



struct	SG_LEVEL_UP
{
	cMSG_BASE_TYPE	base;

	WORD			wLevel;
	int				iExp;
	DWORD			dwSkillExp;

	short			sStrength;
	short			sAgility;
	short			sConstitution;
	short			sWisdom;
	short			sIntelligence;
	short			sCharisma;
	short			sLuck;
	WORD			wLevelPoint;
};



struct	SG_LEVEL_UP_OTHER
{
	cMSG_BASE_TYPE	base;

	WORD			wWho;
	WORD			wLevel;
};



struct	SG_CHANGE_STATE_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wType;
	short			sValue;
	WORD			wLevelPoint;
};



struct	SG_INCREASE_ABILITY_LEVEL
{
	cMSG_BASE_TYPE	base;

	WORD			wAbility,wLevel;
	DWORD			dwSkillExperience;
};





struct	SG_VERY_SIMPLE_ACTOR_INFO
{
	cMSG_BASE_TYPE	base;

	BYTE			aBuffer[100];
};



struct	SG_REMOVE_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			bf15Actor	:	15;
	WORD			bf1IsHide	:	1;
};

enum
{
	eTR_ASK_OF_CLIENT		,
	eTR_LOW_CP				,
	eTR_BY_SKILL			,
};



struct	SG_TRANSFORMATION
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			bf8TransformationJob		:	8;
	WORD			bf4Reason					:	4;
	WORD			bf1IsRequreOnlyAfterMotion	:	1;

	WORD			wSpentCP;
	WORD			wPosX,wPosY;

	BYTE			bWeapon,bShield,bArmor,bWeaponEffect;
	WORD			bf6FPS						:	6;
	WORD			:0;
};

enum
{
	eTRANSFORMATION_SUCCESS,
	eTRANSFORMATION_FAILED_BY_LOW_CP_LEVEL,
	eTRANSFORMATION_FAILED_BY_YET_NOT_COMPLETE_ACTION,
	eTRANSFORMATION_FAILED_BY_SECOND_JOB_SKILL_NOT_FOUND,
	eTRANSFORMATION_FAILED_BY_NOT_AVAIL_JOB,
	eTRANSFORMATION_FAILED_BY_ETC,
};



struct	SG_TRANSFORMATION_FAILED
{
	cMSG_BASE_TYPE	base;

	WORD			wReason;
	short			sCP;
};



struct	SG_TOGGLE_SITDOWN
{
	cMSG_BASE_TYPE	base;

	WORD			bf1IsSitDown : 1;
	WORD			bf15Serial   : 15;
	WORD			wPosX,wPosY;
};

enum
{
	eSITDOWN_RESULT_OK				,
	eSITDOWN_RESULT_FAILED_BY_BATTLE,
};



struct	SG_SITDOWN_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wHP;
	short			sCP;
	WORD			wValue;
};

enum
{
	eREDISTRIBUTE_SKILL,
	eREDISTRIBUTE_STATE,
	eREDISTRIBUTE_SKILL_BY_CH5,
	eREDISTRIBUTE_STATE_BY_CH5,
	eREDISTRIBUTE_FOR_DUEL_SERVER,
};

struct	SG_REDISTRIBUTE
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
};

struct	SG_STOP_BATTLE
{
	cMSG_BASE_TYPE	base;
	
	WORD			bf1IsReleaseFocus	:	1;
};

struct	SG_GG_AUTH_DATA
{
	cMSG_BASE_TYPE	base;

	char	aBuffer[16];
};

struct	SG_WINNING_PRIZE_LOTTO
{
	cMSG_BASE_TYPE	base;

	WORD	wItem;
};

struct	SG_HS_GUID
{
	cMSG_BASE_TYPE	base;

	BYTE	abGuidReqMsg[20];
};

struct	SG_HS_REQ_MSG
{
	cMSG_BASE_TYPE	base;

	BYTE	abReqMsg[160];
};

struct	SG_ARCA_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			bf8Count	:	8;
	WORD			bf1IsFirst	:	1;

	cArcaSimpleInfo	aArcaList[dMAX_ARCA_COUNT_IN_FIELD];
};

struct	SG_ADD_ARCA
{
	cMSG_BASE_TYPE	base;
	cArcaSimpleInfo	arcaInfo;
};

struct	SG_ATTACK_TO_OBJECT
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Attacker		:	11;
	DWORD			bf4ObjectType		:	4;
	DWORD			bf8RemainDurability	:	8;
	DWORD			bf6FPS				:	6;

	WORD			wObjectIndex;
	WORD			wPhysicalDamage,wMagicDamage;
	WORD			wSkill;
	WORD			wPosX,wPosY;
};

enum
{
	eFTW_CATCH,
	eFTW_EXPLOSION,
	eFTW_DETECTED,
	eFTW_ACTIVE,
	eFTW_DISACTIVE,
	eFTW_FAILED_BY_BAD_STATUS,
	eFTW_FAILED_BY_VALID_TRAP,
	eFTW_FAILED_DISARM_BY_RANGE,
	eFTW_FAILED_BY_IS_CAN_NOT_DETECT,	
	eFTW_FAILED_BY_IS_DISACTIVE,	
	eFTW_FAILED_BY_IS_CAN_NOT_DISARM_TRAP_BY_LOW_DISARM_LEVEL,
	eFTW_FAILED_BY_NOT_USE_ABLE_SKILL,
	eFTW_DISARM_FAILED_AND_EXPLOSION_TRAP,
	eFTW_DISARM_BY_SKILL_FAILED,
	eFTW_DISARMED_BY_SKILL,
	eFTW_TRY_DISARM,
	eFTW_TRY_DISARM_FAILED_BY_IS_LOW_CP,
	eFTW_TRY_DISARM_FAILED_BY_BUSY,
};

struct	SG_FIELD_TRAP_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wTrapIndex;
	WORD			wValue1;
};

struct	SG_CLOSE_RANGE_FIELD_TRAP_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wTrapIndex;
	WORD			wValue1;
	char			strName[dNAME_LENGTH];
};

struct	SG_ENTER_TO_THE_SECRET_DUNGEON
{
	cMSG_BASE_TYPE	base;

	WORD			wGateArea;
	WORD			wGateField;
};



enum
{
	eEW_SUCCESS_AID_ATTACK,
	eEW_LEVEL_DRAIN,
	eEW_TORTURE_DAMAGE,
	eEW_MAKE_SKILL_ZONE,
	eEW_MAKE_SHAKLE,
	eEW_DAMAGED_BY_SHAKLE,
	eEW_SHAKLE_SKILL,
	eEW_BITE_HANGER,
	eEW_THE_OTHER_SELF_COUNT,
	eEW_MAKE_THE_OTHER_SELF,
	eEW_CRUSH_MAGIC_BOX,
	eEW_SHUT_IN_MAGIC_BOX,
	eEW_CAUGHT,
	eEW_TRANS,
	eEW_DRAIN_BLOOD,
	eEW_MULTIPLE_DAMAGE,
	eEW_SET_MARIONETTE_STATUS,
	eEW_USE_ACTION,
	eEW_CHANGE_SAGE_BOOK_COLOR,
	eEW_MINIPET_ACTION,

	eEW_ARRAY_INDEX_ZERO_IS_ACTOR_SERIAL_BORDER,

	eEW_SHAKE_SCREEN,
	eEW_FAILED_TO_CREATE_SECRET_DUNGEON_BY_LIMIT_LEVEL,
	eEW_PERMISSION_LOGOUT,
	eEW_TRY_LOGOUT,
	eEW_CAN_NOT_EXIT_GAME_DURING_BATTLE,
	eEW_CANCEL_LOGOUT_TIMER_BY_ACTION,
	eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_NOT_EXIST_KEY,
	eEW_NOTIFICATION_OF_DEATH_COUNTER,
	eEW_STEAL_GOLD,
	eEW_END_OF_REMAIN_SECOND_FOR_USE_GATE_GLOVE,
	eEW_USE_GATE_GLOVE_REMAIN_SECOND,
	eEW_JUDGE_ITEM,
	eEW_MERGE_ITEM,
	eEW_OPEN_JUDGE_OF_ITEM_WINDOW,
	eEW_OPEN_MERGE_ITEM_PIECE_WINDOW,
	eEW_QUEST_COMPLETE,
	eEW_REMOVE_ITEM_PREFIX,
	eEW_WITHDRAW_CARROT_AGREEMENT_FAILED,
	eEW_WITHDRAW_CARROT_FAILED_BY_OWNED_OVER_COUNT,
	eEW_OPEN_CARROT_SHOP,
	eEW_FAILED_TO_PUT_BANNER,
	eEW_FAILED_TO_BANNER_FULL,
	eEW_SUCCESS_TO_PUT_BANNER,
	eEW_REMOVE_BANNER,
	eEW_FAILED_TO_PUT_BANNER_BY_TO_CLOSE,
	eEW_FAILED_TO_PUT_BANNER_BY_INCORRECT_FIELD,
	eEW_TREASURE_MAP_DESTROYED_WHEN_USE,
	eEW_OPEN_TREASURE_BOX,
	eEW_BOOST_EXP_SECOND_BY_TREASURE_MAP_EVENT,
	eEW_BOOST_GOLD_SECOND_BY_TREASURE_MAP_EVENT,
	eEW_BOOST_EXP_BY_TREASER_MAP_EVENT_TIME_OVER,
	eEW_BOOST_GOLD_BY_TREASER_MAP_EVENT_TIME_OVER,
	eEW_DESTROY_ITEM_BY_MOVE_FIELD,
	eEW_DESTROY_TREASURE_MAP_BY_MOVE_FIELD,
	eEW_FAILED_TO_USE_TREASURE_MAP,
	eEW_FOUND_GOLD_IN_TREASURE_BOX,

	eEW_BERSERK_COUNT_OVER,
	eEW_ALREADY_BERSERK_STATUS,
	eEW_USE_BERSERK_TITLE,
	eEW_TIME_OVER_BERSERK_TIME,
	
	eEW_CHANGE_MAGIC_RESISTANCE,
	eEW_CHANGE_FIRE_RESISTANCE,
	eEW_CHANGE_WATER_RESISTANCE,
	eEW_CHANGE_EARTH_RESISTANCE,
	eEW_CHANGE_WIND_RESISTANCE,
	eEW_CHANGE_LIGHT_RESISTANCE,
	eEW_CHANGE_DARK_RESISTANCE,
	eEW_OPEN_PIGEON_POST_SHOP,
	eEW_CHANGE_NONAME_COUPLE_RING_NAME_TO_LAST_TRADER,
	eEW_TARGET_IS_IN_PREMIUM_ZONE,
	eEW_TARGET_IS_PLAY_GUILD_BATTLE,
	eEW_END_FIRE_WORK_TIME,
	eEW_FIRE_FIRE_WORK,
	eEW_USE_FIRE_WORK,
	eEW_CHANGE_ISPI_SETTING,
	eEW_USE_BOTTOMLESS_BOX_FAILED,
	eEW_USE_BOTTOMLESS_BOX_SUCCESS,
	eEW_SET_CHAPTER,
	eEW_CAN_NOT_RETURN_TO_VILLAGE_DURING_BATTLE,
	eEW_CAN_NOT_TELEPORT_DURING_BATTLE,
	eEW_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE,
	eEW_CAN_NOT_FIRE_CRACKER_BY_ALREADY_USE_FIRE_CRACKER,
	eEW_FAILED_TO_ENTER_SECRET_DUNGEON_BY_LIMIT_LEVEL,
	eEW_TARGET_INVENTORY_FULL,
	eEW_TARGET_LEVEL_TOO_LOW,
	eEW_TRANS_TO_WEAPON,
	eEW_FINISH_LIGHTNING_WINDER,
	eEW_SET_COOL_TIME,
	eEW_IS_LIMIT_ACTIVE_COUNT_SKILL	,
	eEW_LIMIT_COPY_ACTOR_COUNT,
	eEW_DISPLAY_MAIN_QUEST_EPILOG,

	eEW_CAN_NOT_TRANS_TARGET_TO_FROG,
	eEW_LIMIT_SHUT_IN_MAGIC_BOX_ACTOR_COUNT,

	eEW_WORDQUIZ_SURVIVOR_COUNT,
	eEW_WORD_QUIZ_WINNER_COUNT,
	eEW_ANSWER_RESULT_TO_WORD_QUIZ,
	eEW_SET_REVERSAL_POWER_TIME,
	eEW_CAN_NOT_CREATE_MORE_ILLUSION,

	eEW_SHARE_DAMAGE_BY_DAMAGE_POOL,
	eEW_SET_IMPULSE_TIME,
	eEW_ANIMATE_PARTNER_TIME,
	eEW_FAILED_TO_USE_POS_BY_ALREADY_EXIST_TARGET,
	
	eEW_INLINE_STRENGTH,
	eEW_INLINE_AGILITY,
	eEW_INLINE_CONSTITUTION,
	eEW_INLINE_WISDOM,
	eEW_INLINE_INTELLIGENCE,
	eEW_INLINE_LUCK,

	eEW_DECLINE_RECOVER_HP_EFFICIENT,
	eEW_INTERRUPTING_ARMOR,
	eEW_CAN_NOT_RECALL_ACTOR_IN_JAIL,
	eEW_TARGET_ALREADY_OWNED_PRINCSS_WEAPON,
	eEW_CHANGE_DEFENSIVE_STATUS,

	eEW_HWABYUNG_TIME,
	eEW_PLOT_OF_SHADOW_TIME,
	eEW_LUCKY_SAVE_THROW_ITEM,
	eEW_SLEEP_TARGET,
	
	eEW_PLAY_SKILL_SOUND,
	eEW_DISCONNECT_BY_HACKSHIELD,
	eEW_BEGIN_SERCH_RANDOM_PLACE_BY_GIGA_LIGHTNING,
	eEW_LOCK_ON_BY_GIGA_LIGHTNING,
	eEW_FIRE_GIGA_LIGHTNING,

	eEW_FAILED_DISARM_ACTOR_TRAP_BY_RANGE,
	eEW_FAILED_DISARM_ACTOR_TRAP_BY_IS_CAN_NOT_DETECT,
	eEW_FAILED_DISARM_ACTOR_TRAP_BY_SAME_TEAM,
	eEW_FAILED_DISARM_ACTOR_TRAP,
	eEW_TRY_DISARM_ACTOR_TRAP,
	eEW_TRY_DISARM_ACTOR_TRAP_FAILED_BY_LOW_CP,
	eEW_TRY_DISARM_ACTOR_TRAP_FAILED_BY_BUSY,

	eEW_DISARM_ACTOR_TRAP_FAILED_BY_IS_CAN_NOT_DISARM_TRAP_BY_LOW_DISARM_LEVEL,
	eEW_DISARM_ACTOR_TRAP_FAILED_BY_SKILL_FAILED,

	eEW_MAKE_SMALL_FLAME,
	eEW_MAKE_FIRE_WALL,
	eEW_REMOVE_FIRE_WALL,

	eEW_BOSS_BATTLE_EVENT	,
	eEW_SET_FOCUS			,
	eEW_CHANGE_GUILD_HALL_LEVEL	,

	eEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_ITEM_COUNT,
	eEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_GOLD,
	eEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE		,

	eEW_CHANGE_TODAY_WITHDRAW_GUILD_INVENTORY_ITEM_COUNT,
	eEW_CHANGE_TODAY_WITHDRAW_GUILD_INVENTORY_GOLD,

	eEW_ADD_RELATED_PLACE_OF_GUILD,
	eEW_REMOVE_RELATED_PLACE_OF_GUILD,

	eEW_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,
	eEW_DO_YOU_WANT_REALLY_BEGIN_GUILD_POINT_BATTLE,

	eEW_RECORD_GUILD_HONOR_POINT,

	eEW_TOO_MANY_SUMMON_GUILD_GUARDIAN,
	eEW_EXIST_GUILD_GUARDIAN_OR_CREST_IN_TO_CLOSE_RANGE,

	eEW_SGG_IS_USE_ABLE_IN_GUILD_BATTLE,
	eEW_SGG_IS_USE_ABLE_DEFENSE_GUILD,
	eEW_RENT_GUILD_PET_COUNT_OVER,

	eEW_CAN_FIND_GUILD_GUARDIAN_IN_SIGHT,
	eEW_REMOVE_NEAREST_GUILD_GUARDIAN	,
	eEW_SUMMON_GUILD_GUARDIAN,

	eEW_OPEN_FEED_GUILD_GUARDIAN_AND_PET_WINDOW,

	eEW_FEED_ITEM_TO_GUILD_PET	,
	eEW_CAN_NOT_FEED_ITEM_BY_OVER_MAX_GUILD_PET_EXP,
	eEW_CAN_NOT_OPEN_GUILD_SHOP_BY_ITEM_ZERO,
	eEW_CHANGE_REMAIN_GUILD_STATEGY_POINT_COUNT,
	eEW_END_GUILD_POINT_BATTLE,
	eEW_CHANGE_OPEN_GUILD_HALL_STATUS,

	eEW_QUESTION_FOR_ADD_RELATED_PLACE_OF_GUILD,
	eEW_CAN_NOT_RETURN_TO_GUILD_HALL_DURING_BATTLE,
	eEW_INCREASE_HUNT_EXP,
	eEW_INCREASE_ITEM_DROP_CHANCE,

	eEW_SWALLOW,
	eEW_ZOMBIE_BOMB,
	eEW_READY_TO_FIRE_CHARGE_MINIPET_SKILL,
	eEW_ADD_MINIPET_SPARE_SKILL,
	eEW_CHANGE_MINIPET_PLACE,
	eEW_EXTEND_REVERSIBLE_VALIDATE,


	eEW_INCREASE_ALLIGNMENT,
	eEW_DECREASE_ALLIGNMENT,
	eEW_SET_ALLIGNMENT,
	eEW_INCREASE_BONUS_GUILD_POINT,
	eEW_CH5_REVERSE_ITEM_PENELTY,
	eEW_CH5_RECOVER_REVERSE_PENELTY,
	eEW_RECOVER_BONUS_GUILD_POINT_PENELTY,
	eEW_CH5_POINT_BONUS,
	eEW_GUILD_EXP,
	eEW_CH5_RELEASE_ITEM_REVERSION,

	eEW_CH5_RESET_ONE_STATE,
	eEW_CH5_RESET_ONE_SKILL,

	eEW_CH5_ITEM_PREFIX_LEVEL_UP,
	eEW_CH5_UPGRADE_ITEM,

	eEW_UPDATE_RED_STONE_COUNT,
	eEW_UPDATE_BARD_TITLE_COOL_TIME,
	eEW_CANCEL_CAST_BARD_TITLE,
	eEW_USE_BARD_TITLE,
	eEW_REQUIRE_PORTAL_ITEM_TO_USE_THAT_TITLE,

	eEW_OPEN_RELAY_STATION,
	eEW_DUEL_MATCH_APPLICANT_INFO,
	eEW_CANCEL_TO_TRY_DUEL_BY_CHANGE_PARTY_MEMBER,
	eEW_OPERATE_DUEL_MATCH,








	eEW_DUEL_KILL,
	eEW_OPEN_DUEL_RANKING_WINDOW,
	eEW_REMAIN_TIME_FOR_USE_DUEL_BUFF,
	eEW_END_DUEL_BATTLE_BY_OUT_ALL_TARGET_TEAM_MEMBER,
	eEW_EAT_DUEL_FIELD_ARCA_ITEM,
	eEW_RESET_WEEKLY_DUEL_RECORD	,

	eEW_CAN_NOT_MOVE_FIELD_DURING_BATTLE	,
	eEW_CAN_NOT_PRINCSS_WEAPON_CHANGE		,
	eEW_SCREEN_EVENT						,
	eEW_INSTANCE_GUILD_BATTLE	,

	eEW_DO_YOU_WANT_REALLY_BEGIN_GUILD_DUNGEN,
	eEW_SCREEN_START_EVENT,
	eEW_DISJOINTING_ITEM_RESULT_FLASE,
	eEW_SET_ITEM_COOL_TIME,
	eEW_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL,
	eEW_REBIRTH	,
	eEW_RESTRAINT_MINIPET	,
	eEW_UNDER_EFFECT_TWO_MOUTH_ONE_SOUND_EVENT,
	eEW_UNDER_EFFECT_GOLD_RUSH_EVENT,
	eEW_CONFIRM_USE_SOUND_OF_LEADERS_BELL,

	eEW_CHANGE_REBIRTH_COUNT,
	eEW_CHANGE_USE_SEAL_SPHERE_OF_DAWN_COUNT,
	eEW_BUY_GOLD_BAR,
	eEW_BUY_GOLD_BAR_ERR,
	eEW_BUY_GOLD_BAR_MESSAGE,
	eEW_OPEN_UPGRADE_SHOP,
	eEW_UPGRADE_MATERIALS_ERR,
	eEW_UPGRADE_SUCCESS,
	eEW_UPGRADE_FAILED,
	eEW_REALLY_ITEM_UPGRADE,
	eEW_DRAG_ITEM_DIVIDE,
	eEW_DISJOINTING_RESULT,
	eEW_CHANGE_WEATHER,
	eEW_CHANGE_CONSENSUS,
	eEW_CHANGE_FLEE,
	eEW_ADD_EXTRA_EFFECT_IMAGE,
	eEW_OPEN_COPY_OR_CUT_PREFIX_WINDOW,
	eEW_CHANGE_FIGHTING_SPIRIT			,
	eEW_JUMP							,
	eEW_DISPELL							,
	eEW_END_TRANSFORMATION_MONSTER			,
	eEW_SUCCESS_INCREASE_CARVING_LEVEL,
	eEW_RESULT_CARVE_LEVEL_MESSAGE			,
	eEW_ATTACK_DOPPELLGANGER,
	eEW_DRAW_IMAGE,
	eEW_RESET_ITEM_COOLTIME,
	eEW_SUMMON_EVENT_MONSTER,
	eEW_OFF_DRAWING_IMAGE,
	eEW_OPEN_INTERFACE,

	eEW_EXTRA_BAG_FULL,
	eEW_CHANGE_EXTRA_BAG_SIZE,
	eEW_SEND_GOLD_LOG,
	eEW_TRANS_AVATAR_TO_GVG_RESULT	,
	eEW_TRANS_GUILD_TO_GVG_RESULT	,
	eEW_ASK_GVG_AVATAR_ORG_DATA_RESULT	,
	eEW_ADD_BIT				,
	eEW_REMOVE_BIT					,
	eEW_OPEN_FAST_PORTAL_WINDOW		,
};



struct	CG_SUMMON_EVENT_MONSTER
{
	cMSG_BASE_TYPE	base;

	WORD	wIndex;
	WORD	wDropItemIndex;


	DWORD	bf10MinLevel	: 10;
	DWORD	bf10MaxLevel	: 10;
	DWORD	bf5Count		: 5;
	DWORD	bf7DropItemCount	:	7;

	DWORD	bf8ScaleX				: 8;
	DWORD	bf8ScaleY				: 8;
	DWORD	bf8MagicResistance		: 8;
	DWORD	bf8StrangeResistance	: 8;
	
	DWORD	bf10CorrectAP			:10;
	DWORD	bf10CorrectHP			:10;
	DWORD	bf10CorrectEXP			:10;
	DWORD	m_bf1IsImmunePhysicalDamage	:	1;
	DWORD	m_bf1IsImmuneMagicDamage	:	1;


	DWORD	bf8CorrectFinalDamage : 8;
	DWORD	bf8DropItemChance		:8;
	DWORD	bf4DropKind				:4;
	DWORD	bf1IsDefaultItemDrop	:1;
	DWORD	bf1IsDefaultGoldDrop	:1;
	DWORD	bf1IsOwner				:1;
	DWORD	:0;

	int		aiOption[4];
	char	strName[dNAME_LENGTH];
};

struct	SG_ETC_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	int				aiValue[6];
};

struct	SG_CHECK_RANGE_ETC_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			awValue[6];
};

struct	SG_PLAY_SOUND
{
	cMSG_BASE_TYPE	base;

	char	strSoundFile[128];
};

#define	dMAX_SET_ANM_PACKET_ACTOR_COUNT	64

class	cSetAnmPacketActorInfo
{
public:
	DWORD	m_bf12Actor		:	12;
	DWORD	m_bf5Anm		:	5;
	DWORD	m_bf4Direct		:	4;
	DWORD	m_bf15ReplayCount	:	15;

	void	set(int _iActor,int _iAnm,int _iDirect)
	{
		m_bf12Actor		=	_iActor;
		m_bf5Anm		=	_iAnm;
		m_bf4Direct		=	_iDirect;
	}
};

struct	SG_DUEL_RECORD
{
	cMSG_BASE_TYPE	base;

	cDuelRecord	record;

	WORD		wWeeklyRank,wDailyRank,wTrialGameRank;
};

class	cDuelBattlerInfoForResult
{
public:
	char	m_strName[dNAME_LENGTH];
	int		m_iTrialGameVP;
	int		m_iWeeklyVP;
	int		m_iDuelPoint;
	WORD	m_wKillCount,m_wDeathCount;
};

struct	SG_DUEL_RESULT
{
	cMSG_BASE_TYPE	base;

	cDuelBattlerInfoForResult	m_aBattler[2][5];

	int		iTrialGameVPForWinner;
	int		iVPForWinner;
	int		iDuelPointForWinner;
	int		iVPForDefeater;
	int		iTrialGameVPForDefeater;
	int		iDuelPointForDefeater;
	WORD	wWinTeam;

	WORD	awKillCount[2];
	char	strTrialGameName[32];
};

class	cDuelRankingInfoForClient
{
public:
	char	m_strName[dNAME_LENGTH];
	WORD	m_wJob;

	int		m_iWinPoint;
	WORD	m_wBattleCount;
	WORD	m_wWinCount;
	WORD	m_wDefeatCount;
	WORD	m_wDrawCount;
};

struct	SG_TRY_DUEL_BATTLE
{
	cMSG_BASE_TYPE	base;

	WORD			wApplicantCount,wType;
	char			strTrialGameName[32];
};

struct	SG_DUEL_RANKING
{
	cMSG_BASE_TYPE	base;
	
	WORD			bf10Count		:	10;
	WORD			bf2RankType		:	2;
	char			strTrialGameName[32];
	
	cDuelRankingInfoForClient	aList[100];
};

enum
{
	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY_PARTY,
	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_SEASON,
	eDMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY,
};

struct	SG_DUEL_MSG
{
	cMSG_BASE_TYPE	base;

	WORD	wMessage,wOutputType;

	char	strText[256];
};

struct	SG_SET_ANM
{
	cMSG_BASE_TYPE	base;

	WORD	wCount;
	cSetAnmPacketActorInfo	aActorList[dMAX_SET_ANM_PACKET_ACTOR_COUNT];
};

enum
{
	eSDM_WARNING_DISAPEAR_DUNGEON,
	eSDM_BEGIN_COUNTDOWN_TIMER,
	eSDM_BEGIN_EVENT,
	eSDM_END_EVENT,
	eSDM_FOCUS_ON_GROUND,
	eSDM_FOCUS_ON_CHARACTER,
	eSDM_RELEASE_FOCUS,
	eSDM_STRESS_MINIMAP,
	eSDM_RELEASE_STRESS_MINIMAP,
	eSDM_COMPLETE,
	eSDM_DISSOLUTION_PARTY,

	eSDM_GET_GUILD_HONOR_POINT,
	eSDM_GUILD_DUNGEON_COMPLETE,
	eSDM_WARNING_DISAPEAR_GUILD_DUNGEON,
};

struct	SG_SECRET_DUNGEON_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessage;
	int				iValue1,iValue2;
};

enum
{
	eAW_OPEN,
	eAW_IS_HIDE,
	eAW_IS_TRAP,
	eAW_IS_LOCKED,
	eAW_EXPLOSION_TRAP,
	eAW_IS_FAILED,
	eAW_FULL_INVENTORY,
	eAW_FULL_GOLD,
	eAW_REMOVE,
	eAW_CAN_NOT_UNLOCK,
	eAW_TRY_UNLOCK_BY_SKILL,
	eAW_UNLOCK_BY_SKILL,
	eAW_UNLOCK_BY_KEY,
	eAW_DO_YOU_WANT_UNLOCK_ARCA_BY_USE_KEY,
	eAW_UNLOCK_FAILED,
	eAW_UNLOCK_FAILED_BY_ALREADY_UNLOCKED,
	eAW_UNLOCK_FAILED_BY_LOW_UNLOCK_LEVEL,
	eAW_TRY_UNLOCK_FAILED_BY_LOW_CP,
	eAW_TRY_UNLOCK_FAILED_BY_BUSY,
	eAW_ATTACK_ARCA_FAILED,
	eAW_ATTACK_ARCA_FAILED_BY_LOW_LEVEL,
	eAW_ATTACK_ARCA_FAILED_BY_INVINCIBLE,
	eAW_ATTACK_ARCA,
	eAW_UNLOCK_BY_ATTACK,
	eAW_ATTACK_FAILED_BY_ALREADY_UNLOCKED,

	eAW_FAILED_BY_BAD_STATUS,
	eAW_DISARM_FAILED_BY_ALREADY_DISARMED,
	eAW_DISARM_FAILED_BY_NOT_EXIST_DISARM_SKILL,
	eAW_DISARM_FAILED_AND_EXPLOSION_TRAP,
	eAW_TRY_DISARM_FAILED_BY_LOW_CP,
	eAW_TRY_DISARM_FAILED_BY_BUSY,
	eAW_TRY_DISARM_BY_SKILL,
	eAW_FAILED_DISARM_BY_LOW_DISARM_LEVEL,
	eAW_FAILED_DISARM,
	eAW_DISARMED_BY_SKILL,

	eAW_EXPLOSION_TRAP_BY_NOT_DETECTED,
	eAW_IS_OWNED_OTHER,
};

struct	SG_ARCA_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wArcaIndex;
	WORD			wValue1;
};

struct	SG_CLOSE_RANGE_ARCA_WORK
{
	cMSG_BASE_TYPE	base;

	WORD			wWork;
	WORD			wArcaIndex;
	WORD			wValue1;
	char			strName[dNAME_LENGTH];
};

struct	SG_BROKEN_ARCA_ITEM_BY_ATTACK
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	CItemBaseInfo	aItem[dMAX_ITEM_COUNT_IN_ARCA];
};


enum
{
	eRRS_OK,
	eRRS_TOO_MANY_REACTION,
	eRRS_FAILED,
	eRRS_NOT_USE_REACTION_SKILL_TIME,
};


struct	SG_SET_REACTION_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wAbility;
	WORD			wIsOn;
};



struct	SG_TURN_ON_PASSIVE_SKILL
{
	cMSG_BASE_TYPE	base;

	BYTE			aPassiveSkill[6];
};



struct	SG_HIT_THORN_DAMAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wDamage;
	WORD			wTarget;
	short			sRemainCP;
	WORD			wAttacker;
	BYTE			bIsDiplayMagicDamage;
};



struct	SG_ABSORB_DAMAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	int				iAbsorbHp;
};



struct	SG_CHANGE_SPECIAL_SKILL_FIELD
{
	cMSG_BASE_TYPE	base;

	WORD			wSpecialSkillField;
	CAbilityDefine	ability;
};

struct	SG_RELEASE_SHIMMERING_SHIELD
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wReceiver;
};

struct	SG_SET_SHIMMERING_SHIELD
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wReceiver;

	WORD			bf6ShieldShape : 6;
	WORD			bf1IsShield    : 1;

	short			sRemainCP;
};


struct	SG_CURE_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wSkill;
	WORD			wActor;
	int				iHealPoint;
};


struct	SG_CURE_ACTOR_BY_POTION
{
	cMSG_BASE_TYPE	base;

	WORD			wItem;
	WORD			wActor;
	int				iHealPoint;
};



struct	SG_CHARGE_CP
{
	cMSG_BASE_TYPE	base;

	short			sCP;
	WORD			wSkill;
};


#define	dMAX_AURA_SKILL_HIT_ACTOR_COUNT		40



struct SG_HIT_AURA_SKILL
{
	cMSG_BASE_TYPE	base;

	WORD			wSkill;
	WORD			bf10Count	:	10;
	WORD			bf6HitImage	:	6;
	WORD			wCaster;
	WORD			wRemainCP;
	WORD			aActorList[dMAX_AURA_SKILL_HIT_ACTOR_COUNT];
};



enum
{
	eDM_DISPLACEMENT,
	eDM_RELEASE,
};

struct SG_DISPLACEMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wX,wY;
	WORD			wIsDisplacement;
	WORD			wTarget;
};



struct SG_FLOAT_MACHINE
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
	WORD			wSkill;
};



struct SG_RELEASE_MACHINE
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
};
	


struct	SG_SHOOT_MACHINE_MISSILE
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster;
	WORD			wTarget;
	WORD			bf12Skill		:	12;
	WORD			m_bf2MinpetOrder:	2;

	cP_HIT_INFO		hitInfo;
};



struct	SG_FIRE_TAIL_CHASER
{
	cMSG_BASE_TYPE	base;

	WORD			wSkill;
	WORD			wCaster,wTarget;
	WORD			wDamage,wShotCount,wShotPeriod,wCorrectDamage;
	short			sRemainCP;
};



struct	SG_MAKE_ACTION_FORM
{
	cMSG_BASE_TYPE	base;

	CAbilityDefine	ability;
	cP_HIT_INFO		hitInfo;
	WORD			wCaster,wTarget;
	short			sRemainCP;
	WORD			wPosX,wPosY;
};


#define	dMAX_TARGET_FOR_THRUST_ATTACK	64



struct	SG_THRUST_ACTORS
{
	cMSG_BASE_TYPE	base;

	WORD				wCount;
	WORD				wSkill;
	WORD				wAxisXPos,wAxisYPos;
	CMinimumActorInfo	aActor[dMAX_TARGET_FOR_THRUST_ATTACK];
};



struct SG_THRUST
{
	cMSG_BASE_TYPE		base;

	WORD				bf15Serial				:	15;
	WORD				bf1IsBlockedByObject	:	1;
	WORD				wPosX,wPosY;
	WORD				bf1IsChangeTargetDirect		:	1;
	WORD		:0;
};



enum
{
	eRR_ARRIVE,
	eRR_CRUSH,
	eRR_BLOCKED,
	eRR_CRUSH_AND_BLOCKED,
};

struct	SG_RUSH
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wArrivePosX,wArrivePosY;
	WORD			wSkill,wLevel;
	WORD			wResult,wFrame;
};

struct	SG_ACTIVE_REACTION_SKILL
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster;
	WORD			wTarget;
	short			sRemainCP;
	WORD			wSkill;
};



enum
{
	eRUIS_CAST,
	eRUIS_CANCEL,
	eRUIS_DESTROY,
	eRUIS_RELEASE,
};

struct	SG_ICY_STALAGMITE
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wStatus;
	WORD			wSkill,wLevel;
	short			sRemainCP;
};




#define	dAMEM_INVINCIBLE				0x0000000000000001
#define	dAMEM_SHIMMERING_SHIELD			0x0000000000000002
#define	dAMEM_FOUNTAIN_BARRIER			0x0000000000000004
#define	dAMEM_MISTIC_FOG				0x0000000000000008
#define	dAMEM_TORNADO_SHIELD			0x0000000000000010
#define	dAMEM_ROCK_BOUNDING				0x0000000000000020
#define	dAMEM_LEVITATE					0x0000000000000040
#define	dAMEM_STONED					0x0000000000000080
#define	dAMEM_COLD						0x0000000000000100
#define	dAMEM_HOT_ENCHANTING			0x0000000000000200
#define	dAMEM_BLIND						0x0000000000000400
#define	dAMEM_BERSERK					0x0000000000000800
#define	dAMEM_SLEEP						0x0000000000001000
#define	dAMEM_CONFUSE					0x0000000000002000
#define	dAMEM_CHARMING					0x0000000000004000
#define	dAMEM_STUN						0x0000000000008000
#define	dAMEM_INVISIBLE					0x0000000000010000
#define	dAMEM_BLUR						0x0000000000020000
#define	dAMEM_FREEZE					0x0000000000040000
#define	dAMEM_BLESS						0x0000000000080000
#define	dAMEM_HEDGER_HEDGING			0x0000000000100000
#define	dAMEM_IS_SITDOWN				0x0000000000200000
#define	dAMEM_PROTECTING_EVIL			0x0000000000400000
#define	dAMEM_NIGHTMARE					0x0000000000800000
#define	dAMEM_PARALYSIS					0x0000000001000000
#define	dAMEM_SPEAR_BLOCKER				0x0000000002000000
#define	dAMEM_DISPLACEMENT				0x0000000004000000
#define	dAMEM_SANCTUARY					0x0000000008000000
#define	dAMEM_HOLD_PERSON				0x0000000010000000
#define	dAMEM_HOLD_MONSTER				0x0000000020000000
#define	dAMEM_EVANGELISM				0x0000000040000000
#define	dAMEM_BOOST						0x0000000080000000
#define	dAMEM_IGNORE_ATTACK_BY_GGG		0x0000000100000000
#define	dAMEM_NEW_BLIND_VERY_SMALL		0x0000000200000000
#define	dAMEM_NEW_BLIND_SMALL			0x0000000400000000
#define	dAMEM_NEW_BLIND_MIDDLE			0x0000000800000000
#define	dAMEM_NEW_BLIND_LARGE			0x0000001000000000
#define	dAMEM_NEW_BLIND_VERY_LARGE		0x0000002000000000
#define	dAMEM_BLACK						0x0000004000000000


struct	SG_UPDATE_STATUS
{
public:
	cMSG_BASE_TYPE			base;

	WORD					wTarget;
	CActorStatusBitField	status;
};

struct	SG_ACTOR_ENCHANTED_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	LONGLONG		dwMask;
	uEnchantedImage	enchantedImage;
	WORD			wItemEffect;
	WORD			bf7MagicCarpetShape :	7;
	WORD			bf3CurrentDuelBuff	:	3;
	WORD			bf3CurrentGGGBuff	:	3;
};

enum
{
	eTM_TELEPORT,
	eTM_JUMP,
};

struct	SG_TELEPORT
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wSkill;
	WORD			wX,wY;
	WORD			wMethod;
};

struct	SG_APPLY_SKILL_EXTRA_EFFECT
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wSkill,wLevel;
	DWORD			dwKeepTime;
};

enum
{
	eFUR_NOT_FIND,
	eFUR_FIND,
	eFUR_TARGET_USER_NOT_OWNED_COUPLE_RING,
};

struct	SG_FIND_USER_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wField;
	WORD			wJob;
	char			strTargetName[dNAME_LENGTH];
};

struct	SG_ACTIVE_SKILL_EFFECT
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wTarget;
	WORD			wSkill;
	short			sRemainCP;
	WORD			wPhysicalDamage,wMagicDamage;
};

struct	SG_ACTIVE_MIRROR_TOWER
{
	cMSG_BASE_TYPE	base;

	WORD			wCaster,wTarget,wTargetRecoverHP;
	WORD			wSkill;
	short			sRemainCP;
	WORD			wMagicDamage;
};

struct	SG_SIMPLE_HIT
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
	int			iPhysicalDamage;
	int			iMagicDamage;
	WORD			bf15RemainHP	:	15;
	WORD			:0;
	WORD			bf7AddHitType	:	7;
	WORD			:0;
};


enum
{
	eSE_VALID,
	eSE_HIT,
	eSE_SKILL,
	eSE_HEAL,
	eSE_CAST,
	eSE_AID_SKILL,
	eSE_CASTER_HIT,
	eSE_EXPLOSION,
};

struct	SG_HIT_DAMAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget,wCaster;
	int				iPhysicalDamage;
	int				iMagicDamage;
	DWORD			dwResultField;
	WORD			wRemainHP;

	WORD			bf11Skill	:	11;
	WORD			bf5ImageType:	5;
};

struct	SG_SKILL_EFFECT
{
	cMSG_BASE_TYPE	base;
	WORD			wTarget;
	WORD			wSkill;
	WORD			bf8Level	: 8;
	WORD			bf8ImageType: 8;

};

struct	SG_PLAY_SKILL_SOUND
{
	cMSG_BASE_TYPE	base;

	WORD	wSkill;
	WORD	wSound;
};

struct	SG_REGEN_HP_BY_AID_SKILL
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
	WORD			wRemainHP;
	WORD			wRegenHP;
};

struct	SG_RECOVER_HP_BY_AID_SKILL
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget,wSkill;
	int				iRemainHP;
	WORD			wRecoverHP;
};




struct	SG_USE_SKILL_TO_ACTOR
{
	cMSG_BASE_TYPE	base;

	WORD			wActor,wTarget;
	short			sRemainCP;
	WORD			wPosX,wPosY;
	cP_HIT_INFO		hitInfo;
	CAbilityDefine	ability;
};



struct	SG_USE_SKILL_TO_GROUND
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
	WORD			wPosX,wPosY;
	CAbilityDefine	ability;
};



struct	SG_CREATE_DUMMY
{
	cMSG_BASE_TYPE		base;

	CSimplePlayerInfo	actorInfo;
};



struct	SG_COPY_ACTOR
{
	cMSG_BASE_TYPE		base;

	CSimplePlayerInfo	actorInfo;
};




struct	SG_RELEASE_DUMMY
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
};



struct SG_RECEIVE_AID_MAGIC
{
	cMSG_BASE_TYPE		base;

	WORD				wType;
	short				sValue;
	WORD				wTarget;
	WORD				wSkill;
};

#define	dMAX_VILLAGE_COUNT	50

struct SG_READY_TO_WARP_TO_NEAREST_VILLAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wVillageCount;
	WORD				wIsCreatePortal;
	WORD				wSkill;
	WORD				awVillage[dMAX_VILLAGE_COUNT];
};

class	CSimplePortalInfo
{
public:
	DWORD		m_bf7Serial		: 7;
	DWORD		m_bf12Village	: 12;
	DWORD		m_bf10Owner		: 10;
	DWORD		m_bf1IsTownPortal: 1;
	WORD		m_wX,m_wY;
};

struct	SG_REMOVE_PORTAL
{
	cMSG_BASE_TYPE	base;

	WORD			wPortalSerial;
};

struct	SG_ADD_PORTAL
{
	cMSG_BASE_TYPE	base;

	WORD			wPortalSerial;
	WORD			wX,wY,wField;
	WORD			bf12Owner		:	12;
	WORD			bf1IsTownPortal	:	1;
};

#define	dMAX_PORTAL_IN_FIELD	100

struct	SG_PORTAL_LIST
{
	cMSG_BASE_TYPE	base;

	WORD				wPortalCount;
	CSimplePortalInfo	aPortalList[dMAX_PORTAL_IN_FIELD];
};

#define	dMAP_FILE_NAME_SIZE			64
#define	dMOVE_FIELD_RESULT_FAIELD	0

struct	SG_MOVE_FIELD_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	char			strIP[dIP_SIZE];
	char			strMapName[dMAP_FILE_NAME_SIZE];
	WORD			wFloor,wFieldSerial;
	BYTE			bf4GuildHallLevel : 4;
	BYTE			bf4Weather : 4;				
	BYTE			bGuildHallShape;
	char			strGuildName[dGUILD_NAME_LENGTH];
};

struct	SG_MOVE_FIELD_NOW
{
	cMSG_BASE_TYPE	base;
};











struct	SG_REMOVE_DROPPED_ITEMS
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			aRemoveItem[dMAX_OPERATE_DROPPED_ITEM];
};



struct	SG_REMOVE_DROPPED_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wRemoveItem;
};



struct	SG_CHANGE_DROPPED_ITEM_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wFieldItem;
	DWORD			dwCount;
};



struct	SG_DROPPING_ITEMS
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	cP_DROPPED_ITEM	aDroppedItem[dMAX_OPERATE_DROPPED_ITEM];
};



struct	SG_DROPPING_ITEM
{
	cMSG_BASE_TYPE	base;

	cP_DROPPED_ITEM	droppedItem;
};



struct	SG_STRIP_EQUIPMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wPart;
};



struct	SG_HIDE_EQUIPMENT
{
	cMSG_BASE_TYPE	base;

	WORD			wPart;
	WORD			wMethod;
};



struct	SG_STRIP_EQUIPMENT_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wPart;
	WORD			wResult;
};



struct	SG_PICK_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	CItemDefine		item;
};



struct	SG_EQUIP_ITEM_RESULT
{
	cMSG_BASE_TYPE		base;

	WORD				wResult;
	WORD				wItemSlot;
	WORD				wPart;
};



enum
{
	eUIR_OK								,
	eUIR_DEATH_BY_REACTION				,
	eUIR_ROTTERY_TICKET_KWANG			,
	eUIR_UPGRADE_FAILED					,
	eUIR_SUCCESS_TO_WRITE_BOOK_CATEGORY	,
	eUIR_SUCCESS_TO_WRITE_BOOK_PAGE		,

	eCIR_CH5_REWARD_BROKEN				,
	eCIR_CH5_REWARD_COPY_ITEM			,
	eUIR_SUCCESS_TO_USE_SEAL_SPHERE_OF_DAWN	,




	eUIR_DECREASE_ITEM_COUNT_BORDER		,

	eUIR_ROTTERY_TICKET_OK				,
	eUIR_FAILED							,
	eUIR_NO_MORE_ATTACH_PREFIX			,
	eUIR_IS_NOT_WEAPON					,
	eUIR_NO_MORE_IMPROVE_WEAPON_DAMAGE	,
	eUIR_IS_NOT_ARMOR					,
	eUIR_NO_MORE_IMPROVE_ARMOR_DEFENSE	,
	eUIR_CAN_NOT_USE_ITEM				,
	eUIR_CAN_NOT_USE_ON_DEST_ITEM		,
	eUIR_LACK_ABILITY					,
	eUIR_STRANGE_OPTION_ITEM			,
	eUIR_IS_NOT_CONSUME_ITEM			,
	eUIR_IS_NOT_CORRECT_STATUS			,
	eUIR_USELESS_STATUS_TO_USE_ITEM		,
	eUIR_CAN_NOT_USE_THIS_FIELD			,
	eUIR_INCORRECT_TARGET				,
	eUIR_INCORRECT_TIMING				,
	eUIR_NOT_EMBODY_ITEM				,
	eUIR_WRONG_ITEM_DATA				,
	eUIR_TOO_FAR						,
	eUIR_FAILED_EVENT20031101			,
	eUIR_CAN_NOT_MEMORY_FIELD			,
	eUIR_FAILED_BY_VALID_INVENTORY_SLOT	,


	eUJMER_OK									,
	eUJMER_ITEM_BROKEN							,
	eUJMER_ITEM_DESTROYED						,
	eUJMER_FAILED								,
	eUJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL	,
	eUJMER_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	,
	eUJMER_CAN_NOT_ATTACH_PREFIX_ITEM			,
	eUJMER_CAN_NOT_ATTACH_THAT_PREFIX_ITEM		,
	eUJMER_TOO_LOW_LEVEL						,
	eUJMER_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			,
	eUJMER_IS_EXCLUSIVE_UNIQUE_ITEM				,
	eUJMER_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM	,


	eUEUIR_OK									,
	eUEUIR_ITEM_BROKEN							,
	eUEUIR_ITEM_DESTROYED						,
	eUEUIR_CAN_NOT_ENCHANT_BY_PREFIX_FULL		,
	eUEUIR_CAN_NOT_UPGRADE_PREFIX				,
	eUEUIR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	,
	eUEUIR_CAN_NOT_ATTACH_PREFIX_ITEM			,
	eUEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM		,
	eUEUIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			,
	eUEUIR_IS_EXCLUSIVE_UNIQUE_ITEM				,
	eUEUIR_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM	,


	eRIR_OK										,
	eRIR_OK_BUT_REMOVE_PREFIX					,
	eRIR_FAILED									,
	eRIR_IS_NOT_INCORRECT_ITEM					,
	eRIR_IS_USELESS_ITEM						,
	eRIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM			,
	eRIR_IS_EXCLUSIVE_UNIQUE_ITEM				,


	eUPR_PICK_ITEM_IS_COPY_ITEM			,
	eUPR_PICK_ITEM_IS_VALID_ITEM		,
	eUPR_CAN_NOT_PICK_ITEM_UNDER_DEATH_STATUS	,

	eUPR_PICK_ITEM_IS_PRIVATE_ITEM		,
	eUPR_PICK_THROW_WEAPON1				,
	eUPR_PICK_THROW_WEAPON2				,
	eUPR_PICK_ITEM_IS_PARTY_ITEM		,
	eUPR_INVENTORY_FULL					,
	eUPR_CAN_NOT_OWN_MORE_GOLD			,
	eUPR_QUEST_ITEM						,

	eUPR_PICK_ITEM_BY_PARTY_MEMBER		,
	eUPR_FAILED_BY_BAD_STATUS			,



	eUCIR_SUCCESS						,
	eUCIR_FAILED						,
	eUCIR_DEST_ITEM_NOT_EXTRA_ITEM		,
	eUCIR_DEST_ITEM_CAN_NOT_CHARGEABLE	,
	eUCIR_MISMATCH_EX_GRADE				,


	eBWR_INCORRECT_BOOK_DATA			,
	eBWR_DEST_ITEM_IS_NOT_BOOK			,
	eBWR_IS_NOT_SAME_BOOK				,
	eBWR_IS_NOT_DEFIEND_BOOK			,
	eBWR_SOURCE_ITEM_IS_NOT_BOOK_CATEGORY	,
	eBWR_SOURCE_ITEM_IS_EXIST_BOOK_CATEGORY	,
	eBWR_SOURCE_ITEM_IS_NOT_BOOK_PAGE	,
	eBWR_SOURCE_ITEM_IS_EXIST_BOOK_PAGE	,
	

	eRDIR_SUCCESS						,
	eRDIR_FAILED						,
	eRDIR_CAN_NOT_REPAIR_DEST_ITEM_BY_HIGH_DURABILITY	,
	eRDIR_DEST_ITEM_NOT_DX_ITEM			,


	eUMCIR_ALREADY_EXIST_SHAPE			,
	eUMCIR_OK							,


	eRIPR_OK							,
	eRIPR_NOT_EXIST_PREFIX_ITEM			,
	eRIPR_IS_EXCLUSIVE_NORTMAL_ITEM		,
	eRIPR_IS_EXCLUSIVE_UNIQUE_ITEM		,


	eUAIR_CAN_NOT_OWN_SAME_ITEM			,
	eUAIR_TOO_MANY_BADGE_ITEM			,

	eUER_PICK_WEAPON					,
	eUER_LACK_ABILITY					,
	eUER_NOT_EQUIPMENT_ITEM				,
	eUER_NOT_RING_PLACE					,
	eUER_NOT_FIST_RING_PLACE			,
	eUER_BELT_NOT_FOUND					,
	eUER_INVENTORY_FULL					,
	eUER_CAN_NOT_EQUIP_EQUIPMENT_WHEN_ACTION,
	eUER_WRONG_PLACE					,

	eUBR_RESULT_FAILED					,

	eUSR_PLEASE_PICK_WEAPON				,
	eUSR_CAN_NOT_STRIP_EQUIPMENT_WHEN_ACTION	,
	eUSR_CAN_NOT_STRIP_SHIELD_STATUS	,
	eUSR_CAN_NOT_STRIP_WEAPON_STATUS	,
	eUSR_CAN_NOT_CHANGE_SHIELD_STATUS	,
	eUSR_CAN_NOT_CHANGE_WEAPON_STATUS	,

	eUDR_FIELD_FULL						,
	eUDR_CAN_NOT_DROP_ITEM				,
	eUDR_PLEASE_PICK_WEAPON				,
	eUDR_CAN_NOT_STRIP_EQUIPMENT_WHEN_ACTION	,

	eUDIR_CAN_NOT_DESTROY_ITEM			,
	eUDIR_VAILD_ITEM					,

	eRIWR_ITEM_SYNC_FAILED				,
	eRIWR_FAILED						,

	eURR_FAILED							,

	eUSGG_FAILED						,
	eUIR_FAILED_BY_ITEM_COOLTIME		,


	eCIR_FAILED_BY_NOT_EQUIPMENT		,
	eUER_WEAR_LIMIT						,


	eCAPR_OK									,
	eCAPR_FAILED								,
	eCAPR_EXIST_SAME_PREFIX						,
	eCAPR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM	,
	eCAPR_CAN_NOT_ATTACH_PREFIX_ITEM			,
	eCAPR_PREFIX_FULL							,
	eCAPR_CAN_NOT_ATTACH_PREFIX					,
	eCAPR_STRANGE_ITEM							,

	ePCR_OK										,
	ePCR_INVENTORY_FULL							,

	ePCOPR_OK									,
	ePCOPR_FAILED								,
	ePCOPR_STRANGE_ITEM							,
	eUIR_BE_MANY_CHARACTER_IN_FIELD				,
	eUIR_CAN_NOT_USE_IN_FIELD					,
	eCLR_CAN_NOT_CARVE_TO_ITEM					,
	eCLR_NOT_ENOUGH_TO_METERIALS				,
	eCLR_INCREASE_SUCCESS						,
	eCLR_INCREASE_FAILED						,
	eCLR_SET_SUCCESS							,
	eCLR_SET_FAILED								,
};



struct	SG_USE_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wItemSlot;
	WORD			wTarget;
};



struct	SG_USE_ITEM_TO_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	BYTE			bItemSlot,bTargetItemSlot;
};



struct	SG_LOTTERY_TICKET_PRIZE_WINNING
{
	cMSG_BASE_TYPE	base;

	CItemDefine		itemPrize;
	WORD			wItemSlot;
	WORD			bf1IsBySelect	:	1;
};



struct	SG_USE_BELT_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wItemSlot;
	WORD			wTarget;
};



struct	SG_USE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wWho;

	CItemBaseInfo	item;

	WORD			bf1IsUseEffectToHero	:	1;
	WORD			bf1IsDisplayMsg			:	1;
	WORD			:	14;

	char			strCaster[dNAME_LENGTH];

};



struct	SG_ITEM_DATA
{
	cMSG_BASE_TYPE		base;

	int					iGold;
	WORD				wExtraInventorySize;
	WORD				wCount;

	cItemDataForRevise	m_aItems[dEQUIPMENT_PART_COUNT+dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];
};

struct	SG_ITEM_PACK_DATA
{
	cMSG_BASE_TYPE		base;

	WORD				wItem;
	cItemPackInfo		packInfo;
};

struct	SG_OTHER_PLAYER_ITEM_DATA
{
	cMSG_BASE_TYPE		base;

	int					iGold;
	WORD				bf6ExtraInventorySize	:	6;
	WORD				bf8ItemCount			:	8;
	char				strId[dID_LENGTH];
	char				strName[dNAME_LENGTH];

	cItemDataForRevise	m_aItems[dEQUIPMENT_PART_COUNT+dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];
};




struct	SG_INVENTORY_DATA
{
	cMSG_BASE_TYPE	base;

	CItemDefine		m_aItems[dOWN_ITEM_COUNT];
};



struct	SG_EQUIPMENT_DATA
{
	cMSG_BASE_TYPE	base;

	CItemDefine		m_aEquip[dEQUIPMENT_PART_COUNT];
};

struct	SG_CHANGE_APPEARANCE
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	DWORD			bf4Weapon		: 4;
	DWORD			bf8WeaponEffect : 8;
	DWORD			bf4Shield		: 4;
	DWORD			bf4Armor		: 4;
	DWORD			bf3ArmorPalette : 3;
};

struct	SG_HIT_ACTION
{
	cMSG_BASE_TYPE	base;

	WORD			wActor;
};

class	CDetectingEvilTargetInfo
{
public:
	DWORD			bf16X		:	16;
	DWORD			bf15Y		:	15;
	DWORD			bf1IsMonster:	1;
};

#define	dMAX_DETECTING_EVIL_TARGET_COUNT	100

struct	SG_DETECTING_EVIL
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;

	CDetectingEvilTargetInfo	aTarget[dMAX_DETECTING_EVIL_TARGET_COUNT];
};

struct	SG_MISS
{
	cMSG_BASE_TYPE	base;
	WORD			wActor,wSkill;
};

struct	SG_DROP_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wItemSlot;
	DWORD			dwItemCount;
};

struct	SG_BELT_ITEM_RELOAD_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
};

struct	SG_CHANGE_ITEM_PLACE_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wSrcSlot;
	WORD			wDestSlot;
};



struct	SG_DECREASE_ITEM_COUNT
{
	cMSG_BASE_TYPE	base;

	WORD			wItemSlot;
	WORD			wCount;
};

enum
{
	eIAR_NORMAL,
	eIAR_WITHDRAW_FROM_GUILD_INVENTORY,
	eIAR_CH5_REWARD_COPY,
};



struct	SG_ADD_ITEM
{
	cMSG_BASE_TYPE	base;

	CItemDefine		item;
	WORD			wReason;
};

enum
{
	eRIR_NORMAL,
	eRIR_STORE_TO_GUILD_INVENTORY,
	eRIR_DISJOINTING
};


struct	SG_REMOVE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wItem;
	WORD			bf14Count				:	14;
	WORD			bf1IsSlot				:	1;
	WORD			bf1IsIncludeEquipment	:	1;
	WORD			wReason;
};

enum
{
	eAGR_NORMAL,
	eAGR_STORE_TO_GUILD_INVENTORY,
	eAGR_WITHDRAW_FROM_GUILD_INVENTORY,
};


struct	SG_ADD_GOLD
{
	cMSG_BASE_TYPE	base;

	int				iAddGold;
	WORD			wReason;
};



struct	SG_SET_ITEM_UNIQUE_SERIAL
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
	DWORD			dwSerial;
};



struct	SG_UPDATE_ITEM_DATA
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot,wIsTwinkle;
	CItemDefine		item;
};

struct	SG_UPDATE_WAY_POINT_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wField,wXPos,wYPos,wWayPointSlot;
};

struct	SG_EXPIRED_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot,wItem;
};

struct	SG_EXPIRED_ITEM_LIST
{
	cMSG_BASE_TYPE	base;

	WORD			wItemCount;
	WORD			awItemList[dOWN_ITEM_COUNT+dEQUIPMENT_PART_COUNT+dEXTRA_INVENTORY_SIZE][2];
};



enum
{
	eUOR_SUCCESS,
	eUOR_SUCCESS_WRITTEN,
	eUOR_INCORRECT_COMMAND,
	eUOR_NOT_WRITTEN_ORB,
	eUOR_CAN_NOT_WRITTEN_FIELD,
	eUOR_CAN_NOT_USE_AT_THIS_FIELD,
};

struct	SG_USE_ORB_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wSlot,wRememberField,wRememberXPos,wRememberYPos;
};

struct	SG_DESTROY_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wSlot;
};

struct	SG_UPDATE_ITEM_DURABILITY
{
	cMSG_BASE_TYPE	base;

	BYTE			bPart,bDurability;
};

struct	SG_REPAIR_ITEM
{
	cMSG_BASE_TYPE	base;

	BYTE			bPart,bDurability;
	DWORD			dwRepairPrice;
};

struct	SG_OPEN_REPAIR_ITEM_WINDOW
{
	cMSG_BASE_TYPE	base;

	WORD			wNpcSerial;
};

struct	SG_OPEN_DISJOINTING_ITEM_WINDOW
{
	cMSG_BASE_TYPE	base;
	
	WORD			wNpcSerial;
};

struct	SG_ITEM_WORK_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
};


struct	SG_RELOAD_BELT_ITEM
{
	cMSG_BASE_TYPE	base;
};

struct	SG_ITEM_PACK_NAME
{
	cMSG_BASE_TYPE	base;

	WORD	wPackIndex;
	char	strPackName[32];
};

struct	SG_REMOVE_PREFIX
{
	cMSG_BASE_TYPE	base;

	WORD			wPrefixIndex;
	WORD			wUseItem,wDestItem;
};











struct	SG_REQUEST_TRADE
{
	cMSG_BASE_TYPE	base;

	char			strAsker[dNAME_LENGTH];
};

#define	dRANGE_OF_ENABLE_TRADE		(400*400)


enum
{
	eRRT_DENY,
	eRRT_TOO_FAR,
	eRRT_NOT_EXIST_TARGET,
	eRRT_ALREADY_TRADE,
	eRRT_DEATH_CHARACTER,
	eRRT_TOO_MANY_TRADE_BOX,
	eRRT_CAN_NOT_ASK_TRADE_TO_OPERATOR,
	eRRT_TARGET_DENY_COMMUNITY_STATUS,
	eRRT_LOW_LEVEL,
	eRRT_CAN_NOT_TRADE_STATE,
};



struct	SG_REQUEST_TRADE_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wReply;
};



struct	SG_BEGIN_TRADE
{
	cMSG_BASE_TYPE	base;

	WORD			wTarget;
	WORD			wTradeBoxSerial;
	char			strTarget[dNAME_LENGTH];
	WORD			wTargetLevel;
};



struct	SG_ADD_TRADE_ITEM_BY_ME
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot,wCount;
};



struct	SG_ADD_TRADE_ITEM_BY_TRADE_MATE
{
	cMSG_BASE_TYPE	base;

	CItemDefine		item;
};



struct	SG_CHANGE_TRADE_ITEM_DATA_BY_ME
{
	cMSG_BASE_TYPE	base;

	WORD			wTradeSlot,wSlot,wCount;
};



struct	SG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE
{
	cMSG_BASE_TYPE	base;

	WORD			wTradeSlot;
	CItemDefine		item;
};



struct	SG_SET_TRADE_GOLD
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	int				iGold;
};



struct	SG_REMOVE_TRADE_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	WORD			wSlotIndex;
};



struct	SG_READY_TRADE
{
	cMSG_BASE_TYPE	base;

	WORD			wTrader;
};



struct	SG_PERMIT_TRADE
{
	cMSG_BASE_TYPE	base;

	WORD			wTrader;
};
#define	dTRADE_ITEM_COUNT	18

class	CTradeItemInfo1
{
public:
	WORD	m_wItemSlot;
	WORD	m_wTradeCount;
};



struct	SG_TRADE
{
	cMSG_BASE_TYPE	base;

	WORD			wReceiveItemCount;
	DWORD			dwReceiveGold,dwGiveGold;
	WORD			wFirstWorkTrader;
	CTradeItemInfo1	aGiveItems[dTRADE_ITEM_COUNT];
	CItemDefine		aReceiveItems[dTRADE_ITEM_COUNT];
};



enum
{
	eTM_TARGET_INVENTORY_FULL,
	eTM_OWNER_INVENTORY_FULL,
	eTM_VALID_INVENTORY_ITEM,
	eTM_ALREADY_ADDED_INVENTORY_ITEM,
	eTM_NOT_TRADE_STATUS,
	eTM_INCORRECT_TRADE_SLOT,
	eTM_TRADE_BOX_FULL	,
	eTM_INCORRECT_TRADE_ITEM_COUNT,
	eTM_INCORRECT_TRADE_ITEM_SLOT	,
	eTM_LOW_EMPTY_INVENTORY_SLOT_COUNT,
	eTM_LOW_GOLD		,
	eTM_CAN_NOT_OWN_SAME_ITEM	,
	eTM_TOO_MANY_BADGE_ITEM		,

	eTM_TRADE_CANCEL_BY_TRADE_MATE,
	eTM_TRADE_CANCEL_BY_ME,
	eTM_TRADE_CANCEL_BY_INVENTORY_DATA_CHANGE,
	eTM_TRADE_CANCEL_BY_INCORRECT_STATUS,

	eTM_TARGET_GOLD_FULL,
	eTM_TARGET_GOLD_BAR_FULL,

};



struct	SG_TRADE_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessage;
};











enum
{
	eCT_TALK							,
	eCT_SAY								,
	eCT_PARTY							,
	eCT_GUILD							,
	eCT_SHOUT							,
	eCT_ADMIN							,
	eCT_RETURN_SAY						,
	eCT_EVENT_SHOUT						,
	eCT_EVENT_CHAT						,
	eCT_EVENT_NOTICE_IN_CHAT_BOX		,
	eCT_EVENT_NOTICE_IN_SYSTEM_MESSAGE	,
	eCT_EVENT_NOTICE_IN_NOTICE_BOX		,
	eCT_EVENT_OX_QUIZ					,
	eCT_SYSTEM_MESSAGE					,
	eCT_GET_PARTY_ITEM_MESSAGE			,
	eCT_ETERNAL_UPGRADE_MESSAGE			,
};


#define	dCHAT_PERIOD			900






struct	SG_EVENT_NOTICE
{
	cMSG_BASE_TYPE	base;

	WORD			wNoticeType;
	WORD			wTime;
	WORD			wEffectflickeringly;
	char			strMessage[128];
};

struct	SG_CHAT
{
	cMSG_BASE_TYPE	base;

	DWORD			bf11Serial		:	11;
	DWORD			bf11PGSerial	:	11;
	DWORD			bf6ChatType		:	6;

	char			strMessage[dNAME_LENGTH+dCHAT_LENGTH];
};

struct	SG_GUILD_MEMBER_CONNETION_STATE
{
	cMSG_BASE_TYPE	base;
	



	
	char			strMessage[c_iMaxGuildMemberCount];
};

struct	SG_NOTICE
{
	cMSG_BASE_TYPE	base;

	char			strMessage[256];
};



enum
{
	eRCF_CAN_NOT_SHOUT_BY_TIME,
};

struct	SG_CHAT_FAILED
{
	cMSG_BASE_TYPE	base;

	WORD			wReason;
};

struct	SG_SAY_RESULT
{
	cMSG_BASE_TYPE	base;
	WORD			wResult;

	char			strName[dNAME_LENGTH];
	char			strMessage[dCHAT_LENGTH];
};


#define dINFOTYPE_PARTY		0
#define dINFOTYPE_GUILD		1
#define dINFOTYPE_ADMIN		2

struct SG_INFO
{
	cMSG_BASE_TYPE		base;

	WORD				wInfoType;
	char				strMessage[dCHAT_LENGTH];
};

struct	SG_CHANGE_PARTY_STATUS
{
	cMSG_BASE_TYPE	base;

	WORD	bf3GoldShareMethod	:	3;
	WORD	bf3ItemShareMethod	:	3;
	WORD	bf3PartyObject		:	3;
	WORD	bf1IsOpenParty		:	1;
};



struct	SG_PARTY_LIST
{
	cMSG_BASE_TYPE		base;

	WORD				wCount;
	WORD				wPartySIndex;
	WORD				wPartyEIndex;

	CPartyInfoForList	aPartyList[dPARTYLIST_COUNT];
};


struct	SG_PARTY_INFO
{
	cMSG_BASE_TYPE		base;

	CPartyInfoInPacket	partyInfo;
};


struct	SG_ASK_JOIN_UNION_PARTY
{
	cMSG_BASE_TYPE		base;
	
	char				strAskerName[dNAME_LENGTH];
	WORD				wAskerJob,wAskerLevel;

	WORD				wAskerPartyCount;
	WORD				wAskerPartyAverageLevel;
	WORD				wAskerPartyPurpose;
	char				strPartyName[dMAX_PARTY_NAME_LENGTH];
};


struct	SG_ASK_JOIN_PARTY
{
	cMSG_BASE_TYPE		base;

	WORD				bf1IsJoinPartyApplication	:	1;
	WORD				bf1IsGuildMemberJoinParty	:	1;
	WORD	:0;
	char				strAskerName[dNAME_LENGTH];
	WORD				wAskerJob,wAskerLevel;
};


struct	SG_JOIN_PARTY_MEMBER
{
	cMSG_BASE_TYPE		base;

	char				strName[dNAME_LENGTH];
	WORD				wLevel;
	WORD				wJob;
	WORD				wSerialInField;
	WORD				wIndex;
	WORD				wField;
	int					iDuelVP;

	DWORD				bf10IFSerial	:	10;
	DWORD				bf4IFFloor		:	4;
	DWORD				bf4SPC			:	4;
};


struct	SG_PARTY_WORK
{
	cMSG_BASE_TYPE		base;

	WORD				wWork;
	WORD				wMemberIndex;
};


struct	SG_UPDATE_PARTY_MEMBER_HP
{
	cMSG_BASE_TYPE				base;

	WORD						wRemainHPRate,wActor;
	DWORD						dwMaxHP;
};


struct	SG_UPDATE_PARTY_MEMBER_INFO
{
	cMSG_BASE_TYPE				base;

	BYTE						bIndex,bRestHp;
	WORD						wLevel;
	WORD						wField;

	DWORD						bf10InstanceField			:	10;
	DWORD						bf4InstanceFieldFloor		:	4;
	DWORD						bf1IsDisconnected			:	1;
	DWORD						bf4SecretDungeonPlayCount	:	4;
	DWORD						bf8Job						:	8;
	int							iDuelVP;

};

enum
{
	ePM_TARGET_DEATH_OR_NOT_EXIST								,
	ePM_TARGET_BELONG_TO_ALREADY_AT_DIFFERENT_PARTY				,
	ePM_TARGET_NOT_BELONG_TO_PARTY								,
	ePM_TARGET_NOT_EXIST										,
	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE		,
	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_BANISH_MESSAGE			,
	ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_CHANGE_PARTY_NAME_MESSAGE	,
	ePM_YOU_ARE_NOT_BELONG_TO_PARTY								,

	ePM_ALREADY_FULL_PARTY										,
	ePM_ASKER_NOT_EXIST											,
	ePM_DENY_INVITE_MESSAGE										,
	ePM_DEST_PARTY_ARE_FULL										,
	ePM_DEST_ACTOR_NOT_LEADER									,
	ePM_FAILED_TO_CREATE_PARTY									,
	ePM_FAILED_TO_JOIN_PARTY									,
	ePM_TARGET_DENY_COMMUNITY_STATUS							,
	ePM_TARGET_NOT_OWN_FORCE									,

	ePM_CAN_NOT_BANISH_CONNECT_MEMBER_IN_SECRET_DUNGEON			,
	ePM_ASK_JOIN_FAILED_BY_TARGET_LEVEL_OVER_THE_SECRET_DUNGEON_LIMIT_LEVEL	,
	ePM_CAN_NOT_BANISH_MEMBER_UNDER_DROP_PARTY_ITEM_TIME		,

	ePM_ALREADY_UNION_PARTY										,
	ePM_TARGET_IS_NOT_PARTY_LEADER								,
};



struct	SG_PARTY_MESSAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wMessage;
};



struct	SG_CHANGE_PARTY_NAME
{
	cMSG_BASE_TYPE		base;

	char				strPartyName[dPARTYNAME_LENGTH];
};



struct	SG_DISSOLUTION_PARTY
{
	cMSG_BASE_TYPE		base;
};



struct	SG_PCIK_PARTY_ITEM
{
	cMSG_BASE_TYPE		base;

	CItemDefine			item;
	WORD				wReceiver;
};

struct	SG_DEATH_PENELTY_SECOND
{
	cMSG_BASE_TYPE		base;

	WORD				wDeathPenelty;

	WORD				bf14PeneltySecond	: 14;
	WORD				bf1IsDeath			: 1;
	WORD				bf1IsRecover		: 1;
};

struct	SG_QUESTION_FOR_RECOVER_DEATH_PENELTY
{
	cMSG_BASE_TYPE		base;

	WORD				wRecoverTime;
	DWORD				dwPrice;
};


struct	SG_PCIK_PARTY_GOLD
{
	cMSG_BASE_TYPE		base;

	int					iGold;
};

#define	dSERVER_MESSAGE_SIZE	256



enum
{
	eSM_SYSTEM,
	eSM_CHATTING,
	eSM_NOTICE,
	eSM_MSG_BOX,
	eSM_WARNING,
};


struct	SG_MESSAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wType;
	char				strMessage[dSERVER_MESSAGE_SIZE];
};



enum
{
	eRSM_DUMMY,
	eRSM_CAN_NOT_EXIT_GAME_DURING_BATTLE,
	eRSM_CAN_NOT_CALL_MAGIC_CARPET_FIELD,
	eRSM_FIND_COPIED_ITEM,
	eRSM_TEST_QUEST,
	eRSM_PLEASE_RELEASE_PARTY_FOR_GUILD_BATTLE,
	eRSM_PROTECTED_BY_PREVENT_STEAL,
	eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_BATTLE_SCHEDULER,
	eRSM_GUILD_ALREADY_BOOKING_GUILD_BATTLE,
	eRSM_GUILD_ALREADY_FINISH_GUILD_BATTLE_FOR_THIS_WEEK,
	eRSM_GUILD_ALREADY_BOOKED_SCHEDULE,
	eRSM_GUILD_INCORRECT_DATA_PLEASE_RETRY,
	eRSM_GUILD_IS_CHECKED_BY_GUILD_BATTLE_DAY_RULE,
	eRSM_GUILD_IS_RECENTLY_BATTLE_GUILD,
	eRSM_GUILD_TOO_MANY_DIFFERENT_VICTORY_POINT,
	eRSM_GUILD_ONLY_GUILD_MASTER_CAN_REQUEST_GUILD_BATTLE,
	eRSM_GUILD_ONLY_GUILD_MASTER_CAN_CANCEL_GUILD_BATTLE,
	eRSM_GUILD_TOO_LATE_TO_CANCEL_GUILD_BATTLE,
	eRSM_GUILD_NOT_EXIST_BOOKED_GUILD_BATTLE,
	eRSM_GUILD_INCORRECT_SOME_DATA_PLEASE_RECONNECT,
	eRSM_GUILD_TOO_LOW_LEVEL_TO_GUILD_BATTLE,
	eRSM_TOO_LOW_GOLD_TO_GUILD_BATTLE,
	eRSM_GUILD_ONLY_GUILD_MASTER_CAN_OPEN_GUILD_MARK_COMPOSER,
	eRSM_OVER_TIME_ITEM_TERM_OF_VALIDATE_IN_INVENTORY,
	eRSM_TOO_MANY_PORTAL_IN_THIS_FIELD,
	eRSM_REMOVE_PORTAL,
	eRSM_TOO_FAR_PORTAL,
	eRSM_LACK_MONEY_FOR_RECOVER_DEATH_PENELTY,

	eRSM_CAN_NOT_RECEIVE_ITEM_BY_INVENTORY_FULL	,
	eRSM_NOTICE_FOR_SPEEDHACK_USER_FORM,
	eRSM_NOTICE_FOR_INCORRECT_PLAYER_USER,
	eRSM_NOTICE_FOR_RELEASE_PENELTY_TIME,
	eRSM_GAME_SERVER_CLOSE,

	eRSM_IS_CAN_NOT_TAME_THAT_MONSTER_BY_THIS_SKILL,
	eRSM_IS_CAN_NOT_TAME_NO_MORE_MONSTER_BY_THIS_SKILL,
	eRSM_IS_CAN_NOT_TAME_MONSTER,
	eRSM_IS_CAN_NOT_TAME_NO_MORE_MONSTER,
	eRSM_NOT_EXIST_CONTROL_PET,
	eRSM_NOT_EXIST_OWNED_SPECIAL_ATTACK_PET,
	eRSM_THAT_SKILL_REQUIRE_VALID_SLOT,

	eRSM_THAT_PET_ALREADY_USE_ALL_BOOST_POWER_FOR_TODAY,
	eRSM_TOO_MANY_PET_CAN_NOT_UNSEAL_BREEDING_RECORD_BOOK,
	eRSM_ALREADY_NAMED_PET,
	eRSM_CAN_NOT_SUMMON_MORE_SUMMON_BEAST,
	eRSM_FOR_UNSEAL_PET_REQUIRE_MORE_POWERFUL_TAME_SKILL,
	eRSM_BREEDING_RECORD_UNSEAL_ONLY_SEAL_PLAYER,
	eRSM_NOT_EXIST_SUMMON_BEAST_USE_THAT_SKILL,

	eRSM_TOO_SHORT_TIME_WHEN_SEAL_PET,
	eRSM_CAN_NOT_SET_NO_MORE_THAT_SKILL_TRAP,
	eRSM_RELEASE_SUMMON_BEAST_BY_LOW_CP,
	eRSM_SUMMON_BEAST_GRADE_DOWN_BY_LOW_CP,
	eRSM_CAN_NOT_SUMMON_BY_DEATH_RULE,
	eRSM_REVISE_INCORRECT_PREFIX_IN_BANK,
	eRSM_REVISE_INCORRECT_PREFIX,


	eRSM_COMMON_MESSAGE_BORDER,


	eRSM_OWN_GOLD_LIMIT,
	eRSM_OWN_BANK_GOLD_LIMIT,
	eRSM_ALREADY_STEAL_GOLD_MONSTER,
	eRSM_ALREADY_STEAL_ITEM_MONSTER,

	eRSM_CAN_NOT_ADD_SECRET_DUNGEON_KEY_BY_FULL_INVENTORY,
	eRSM_THAT_IS_CLOSE_SECRET_DUNGEON,
	eRSM_TOO_MANY_PLAYER_ENTER_SECRET_DUNGEON,
	eRSM_OVER_THE_SECRET_DUNGEON_PLAY_COUNT_PER_DAY,
	eRSM_CAN_NOT_ENTER_TO_PORTAL_BY_IS_OWNED_DIFFERENT_PARTY,

	eRSM_LACK_MONEY_FOR_JUDGE_ITEM,
	eRSM_LACK_MONEY_FOR_MERGE_ITEM,
	eRSM_TO_USE_COUPLERING_SET_REQUIRE_TWO_VALID_INVENTORY_SLOT,
	eRSM_REQUEST_TIME_IS_MAINTENANCE_TIME,
	eRSM_EXIST_TRAFFIC_OBJECT_TO_OPEN_PORTAL,

	eRSM_CAN_NOT_USE_THAT_ITEM_IN_THIS_FIELD,
	eRSM_TARGET_IS_AT_BLOCKED_FIELD_TO_USE_THAT_ITEM,
	eRSM_CAN_NOT_TELEPORT_FIELD,
	eRSM_ALREADY_REQUEST_GUILD_SCRAMBLE,
	eRSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID,
	eRSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID_FIELD_SERIAL,
	eRSM_CAN_NOT_WITHDRAW_GUILD_ITEM_BY_CHECK_PERMIT_COUNT,
	eRSM_CAN_NOT_WITHDRAW_GUILD_GOLD_BY_CHECK_PERMIT_GOLD,

	eRSM_FAILED_COMMAND_WORK,
	eRSM_ALREADY_EXIST_SAME_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,
	eRSM_FULL_VILLAGE_IN_GUILD_RELATED_PLACE_COUNT,
	eRSM_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,
	eRSM_NOT_EXIST_VILLAGE_IN_GUILD_RELATED_PLACE_LIST,
	eRSM_MINIMUM_COUNT_RULE_OF_GUILD_RELATED_PLACE_LIST	,
	eRSM_COUNT_OVER_TO_TRY_GUILD_POINT_BATTLE,
	eRSM_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD,
	eRSM_TO_MANY_GUILD_TO_TRY_GUILD_POINT_BATTLE,
	eRSM_FAILED_TO_TRY_GUILD_POINT_BATTLE,
	eRSM_FAILED_TO_ENTER_GUILD_BATTLE_FIELD_BY_NOT_BATTLE_TIME,
	eRSM_TARGET_GUILD_HALL_IS_CLOSE_TO_STRANGER,

	eRSM_CAN_NOT_STORE_ITEM_IN_GUILD_INVENTORY,

	eRSM_PERMIT_ONLY_GUILD_MASTER_WORK			,
	eRSM_VALID_GUILD_SKILL_LEVEL	,
	eRSM_TEMP_LIMIT_GOLD_IN_GUILD_INVENTORY,
	eRSM_FAILED_BY_INSTANCE_GUILD_BATTLE_TIME_RULE,
	eRSM_CAN_NOT_SUMMON_MINI_PET_BY_FULL_COUNT,
	eRSM_ITS_USE_TO_ENEMY_SKILL,
	eRSM_TOO_FAR_TO_USE_SKILL,
	eRSM_CAN_NOT_FIND_ATTACK_ABLE_TARGET,
	eRSM_SEAL_MINIPET,
	eRSM_UNSEAL_MINIPET,
	eRSM_IS_NOT_ABLE_TO_TRY_GUILD_POINT_BATTLE_TIME,
	eRSM_CAN_NOT_ACTIVATE_MINIPET_BY_IS_EXPIRED_MINIPET,
	eRSM_EXPIRED_REVERSAL_FRAME_TIME,
	eRSM_MISSILE_BLOCKED_LINE,
	eRSM_PLEASE_SELECT_MINIPET_FOR_CHANGE_NAME,
	eRSM_REQUIRE_MINIPET_FOR_USE_THAT_ITEM,
	eRSM_DUEL_1VS1_RULE,
	eRSM_DUEL_CAN_NOT_TRY_BY_TO_MANY_BATTLER,
	eRSM_DUEL_CAN_NOT_TRY_BY_BAD_PARTY_STATUS,
	eRSM_DUEL_FIELD_IS_FULL,
	eRSM_DUEL_EXCEPTION_TIME_RULE,
	eRSM_DUEL_CAN_NOT_TRY_BY_YOU_ARE_NOT_PARTY_LEADER,
	eRSM_DUEL_CAN_NOT_TRY_BY_MISMATCH_PARTY_MEMBER_COUNT,
	eRSM_MINI_PET_CANT_USE_AWAKEN_ITEM_LOWER,

	eRSM_GUILD_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD,
	eRSM_COUNT_OVER_TO_TRY_GUILD_DUNGEN,
	eRSM_TO_MANY_GUILD_TO_TRY_GUILD_DUNGEN,
	eRSM_FAILED_TO_TRY_GUILD_DUNGEN,

	eRSM_EXIST_COUNT_OVER_MEMBER_TO_TRY_SECRET_DUNGEN,
	eRSM_SECRET_DUNGEN_ERROR_GATHER_PARTY_MEMBER_IN_SAME_FIELD,
	eRSM_TO_MANY_PARTY_TO_TRY_SECRET_DUNGEN,
	eRSM_TOO_MANY_BANK_TRANSACTION_WITH_REBIRTH,
	eRSM_TOO_MANY_GOLD_BAR,





	eRSM_OUTPUT_MESSAGE_BORDER		=	9999,

	eRSM_TOUCH_EVENT_AREA_SUCCESS	=	10000,
	eRSM_TOUCH_EVENT_AREA_FAILED,


};


struct	SG_REGIST_MESSAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wMessage;
	WORD				wType;
};

struct	SG_EXIT_GAME_SUCCESS
{
	cMSG_BASE_TYPE		base;

	WORD				wValue;
};

struct	SG_PICK_PARTY_ITEM
{
	cMSG_BASE_TYPE		base;

	char		strPickerName[dNAME_LENGTH];
	WORD		bf1IsSharingGold	:	1;

	CItemDefine	item;
};

struct	SG_REMAIN_HP
{
	cMSG_BASE_TYPE		base;

	int					iMaxHP;
	WORD				wRemainHPRate;
	WORD				wActor;
	WORD				bf1IsSyncUnionParty : 1;
	WORD				bf1IsGuildObject : 1;
	WORD				:0;
};

struct	SG_GUILD_PACKET_FORM
{
	cMSG_BASE_TYPE		base;
	WORD				wGuildSerial;
};


struct	SG_REQUEST_ADD_FRIEND
{
	cMSG_BASE_TYPE		base;

	char				strName[dNAME_LENGTH];
};

struct	SG_REPLY_FOR_ADD_FRIEND
{
	cMSG_BASE_TYPE		base;

	WORD				wResult;
	WORD				bf8Job			:	8;
	WORD				bf1IsRecover	:	1;
	char				strName[dNAME_LENGTH];
};


struct	SG_ASK_REMOVE_FRIEND
{
	cMSG_BASE_TYPE		base;

	char				strName[dNAME_LENGTH];
};

enum
{
	eRFR_SUCCESS,
	eRFR_NOT_EXIST_NAME_IN_TARGET_FRIEND_LIST,
	eRFR_TARGET_NOT_CONNECTED,
};

struct	SG_ASK_REMOVE_FRIEND_RESULT
{
	cMSG_BASE_TYPE		base;

	char				strName[dNAME_LENGTH];
	WORD				wResult;
};

struct	SG_READY_TO_MAKE_GUILD
{
	cMSG_BASE_TYPE		base;

	char				strMemberList[dPARTY_MAXPLAYER][dNAME_LENGTH];
};

struct	SG_OPEN_GUILD_BATTLE_SCHEDULER
{
	cMSG_BASE_TYPE			base;

	WORD					wBeginHour,wBeginMinute,wBattlePeriod,wBattleCount;
};

struct	SG_OPEN_GUILD_MARK_COMPOSER
{
	cMSG_BASE_TYPE			base;
};

struct	SG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME
{
	cMSG_BASE_TYPE						base;

	WORD								wTimeOrder;
	WORD								wYear,wMonth,wDay;
	CGuildBattleInFieldInfoForClient	data;
};

struct	SG_OPEN_GUILD_HALL_LIST
{
	cMSG_BASE_TYPE		base;

	WORD	wCount;
	char	abBuffer[dGUILD_NAME_LENGTH*c_iGuildHallCount];
};

struct	SG_GUILD_HONOR_POINT_LIST
{
	cMSG_BASE_TYPE		base;

	class	cGuildHonorPointInfoForPacket
	{
	public:
		int		m_iHonorPoint;
		char	m_strGuildName[dGUILD_NAME_LENGTH];
	};
	
	WORD	wCount,wHallLevel;
	cGuildHonorPointInfoForPacket	aList[30];
};

struct	SG_OPEN_RELATED_PLACE_LIST
{
	cMSG_BASE_TYPE		base;

	WORD	wCount;
	WORD	awPlaceList[dMAX_GUILD_VILLAGE_COUNT];
};

struct	SG_OPEN_GUILD_INVENTORY
{
	cMSG_BASE_TYPE		base;

	WORD							wCount;
	int								aiWithdrawGoldLimitPerDay[5];
	BYTE							aWithdrawItemCountPerDay[4][5];
	BYTE							abInventorySize[4];
	LONGLONG						llGuildGold;

	cGuildInventoryItemForPacket	aItems[c_iGuildInventorySize];
};

struct	SG_STORE_GUILD_INVENTORY_ITEM
{
	cMSG_BASE_TYPE		base;

	WORD		wGuildInventorySlot;
	CItemDefine	item;
};

struct	SG_REMOVE_GUILD_INVENTORY_ITEM
{
	cMSG_BASE_TYPE		base;

	WORD		wGuildInventorySlot;
	CItemDefine	item;
};

struct	SG_STORE_GUILD_INVENTORY_GOLD
{
	cMSG_BASE_TYPE		base;

	int		iGold;
};

struct	SG_REMOVE_GUILD_INVENTORY_GOLD
{
	cMSG_BASE_TYPE		base;

	int		iGold;
};

struct	SG_CHANGE_GUILD_INVENTORY_ITEM_PLACE
{
	cMSG_BASE_TYPE		base;

	WORD	wSlot1,wSlot2;
};

enum
{
	eSW_BEGIN_NOTICE,
	eSW_END_NOTICE	,
	eSW_BEGIN		,
	eSW_END			,

	eSW_SUCCESS_TO_DEFENSE	,
	eSW_SUCCESS_TO_INVADE	,
	eSW_FAILED_TO_DEFENSE	,
	eSW_FAILED_TO_INVADE	,
};

enum
{
	eGMG_ACTIVE_GUILD_MASTER_GRACE			,
	eGMG_ACTIVE_GUILD_MASTER_GRACE_END		,
	eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR1	,
	eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR2	,
	eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR3	,
	eGMG_ACTIVE_GUILD_MASTER_GRACE_ERROR4	,
};

struct	SG_GVG_WORK_SUCCESS
{
	cMSG_BASE_TYPE		base;

	WORD	wResult;
	WORD	wIsTransAvatar;
	char	strName[dGUILD_NAME_LENGTH];
};

struct	SG_GUILD_MASTER_GRACE_INFO
{
	cMSG_BASE_TYPE		base;
	
	WORD	wWork,wValue;
};

struct	SG_SIEGE_WARFARE_NOTICE
{
	cMSG_BASE_TYPE		base;

	WORD	wWork,wValue;
	char	strGuildName[dGUILD_NAME_LENGTH];
};

struct	SG_GUILD_PET_AND_GUARDIAN_EXP
{
	cMSG_BASE_TYPE		base;

	CGuildPetInfo	aPetInfo[dMAX_GUILD_PET_COUNT];
	CGuildPetInfo	aGuardianInfo[dMAX_GUILD_GUARDIAN_COUNT];
};

struct	SG_OPEN_GUILD_SHOP
{
	cMSG_BASE_TYPE		base;

	WORD	wSellItemCount;
	WORD	wDC_Factor;
	WORD	awSellItemList[c_iGuildHallSellEnchantItemCount];
};

struct	SG_CHANGE_GUILD_BATTLE_SCHEDULE
{
	cMSG_BASE_TYPE				base;

	WORD						wTimeOrder,wFieldOrder;
	CGuildBattleInfoForClient	info;
};



enum
{
	eMGR_SUCCESS				,
	eMGR_EXIST_SAME_NAME_GUILD	,
	eMGR_NOT_READY_TO_MAKE_GUILD,
	eMGR_ALREADY_HAVEAGUILD		,
	eMGR_LOW_POWER				,
	eMGR_FULL					,
	eMGR_FAILED					,
};

struct	SG_MAKE_GUILD_RESULT
{
	cMSG_BASE_TYPE		base;

	WORD				wResult;
	char				strGuildName[dGUILD_NAME_LENGTH];
};



struct	SG_GUILD_BATTLE_RESULT
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	char						strTargetGuild[dGUILD_NAME_LENGTH];
	CGuildBattleResultInfo		battleResult;
};

#pragma	pack(1)
class	CSimpleGuildMemberInfo
{
protected:
	char	m_strName[dNAME_LENGTH-1];
public:

	char*	getName()
	{
		static	char	strName[dNAME_LENGTH];
		
		memcpy(strName,m_strName,dNAME_LENGTH-1);

		strName[dNAME_LENGTH-1]	=	NULL;

		return	strName;
	}
	void	setName(char *_lpstrName)
	{
		memcpy(m_strName,_lpstrName,dNAME_LENGTH-1);
	}

	WORD	m_bf8Job			:	8;
	WORD	m_bf4ReservePet		:	4;

	WORD	m_bf4Rank			:	4;

	WORD	m_bf11Level			:	11;
	WORD	m_bf1IsConnected	:	1;
	WORD	m_bf1IsOnline		:	1;
};

#pragma	pack()

class	CSimpleGuildMemberList
{
public:
	WORD					m_wCount;

	CSimpleGuildMemberInfo	m_aMembers[dGUILD_MAX_PLAYER];
};

#define	dGUILD_MEMBER_COUNT_FOR_MEMBER_LIST_PACKET	50

struct	SG_GUILD_MEMBER_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						bf15Count	:	15;
	WORD						bf1IsAppend	:	1;

	CSimpleGuildMemberInfo		m_aMember[dGUILD_MEMBER_COUNT_FOR_MEMBER_LIST_PACKET];
};

struct	SG_GUILD_MARK_INFO
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	WORD						bf1IsTwinkle	:	1;
	WORD						bf8Level		:	8;
	WORD						bf3HallLevel	:	3;
	CGuildMarkInfo				markInfo;
};

struct	SG_CHANGE_GUILD_MARK
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	WORD						bf4Result				:	4;
	WORD						bf1IsUseGuildMarkPack	:	1;
	CGuildMarkInfo				guildMarkInfo;
};



struct	SG_BASE_GUILD_INFO
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	CGuildBaseInfo				guildInfo;
};



struct	SG_SIMPLE_GUILD_INFO
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	BYTE				abGuildStatueStatus[6];
	WORD				wGuildSkillCount;
	WORD				bf3HallLevel	:	3;
	WORD				bf8Level		:	8;
	WORD				bf1IsTwinkle	:	1;
	CGuildMarkInfo		guildMarkInfo;
	char				strGuildName[dGUILD_NAME_LENGTH];
	BYTE				abGuildSkill[dMAX_GUILD_SKILL_COUNT];
};

struct	SG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER
{
	cMSG_BASE_TYPE				base;

	WORD						wGuildSerial;
	CGuildBattleInfoForMember	data;
};


struct	SG_READY_TO_NEXT_GUILD_BATTLE
{
	cMSG_BASE_TYPE				base;
	WORD						wGuildSerial;
	char						strTargetGuildName[dGUILD_NAME_LENGTH];
	BYTE						bHour,bMinute;
};

struct	SG_GUILD_BATTLE_NOTICE
{
	cMSG_BASE_TYPE				base;
	BYTE						bRemainTimeToBeginBattle,bRemainTimeToEndBattle;
};

struct	SG_GUILD_BATTLE_INFO
{
	cMSG_BASE_TYPE				base;
	char						strTeam1Name[dGUILD_NAME_LENGTH],strTeam2Name[dGUILD_NAME_LENGTH];
	int							iGuild1KillPoint,iGuild2KillPoint;
	WORD						wIsTrialGame;
};

struct	SG_GUILD_BATTLE_KILL_NOTICE
{
	cMSG_BASE_TYPE				base;
	WORD						wKillerTeam,wGetPoint,wIsDuel;
	int							iGuild1KillPoint,iGuild2KillPoint;
	char						strKiller[dNAME_LENGTH],strTarget[dNAME_LENGTH];
};

struct	SG_DISSOLUTION_GUILD
{
	cMSG_BASE_TYPE		base;

	char				strGuildName[dGUILD_NAME_LENGTH];
};

struct	SG_JOIN_GUILD
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	char				strGuildName[dGUILD_NAME_LENGTH];
};

struct	SG_UPDATE_GUILD_MEMBER
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	char				strMemberName[dNAME_LENGTH];
	WORD				m_bf4Rank			:	4;
	WORD				m_bf11Level			:	11;
	WORD				m_bf1IsConnected	:	1;

	WORD				m_bf8Job		:	8;
	WORD				m_bf4ReservePet	:	4;
	WORD				:0;

	BYTE				bMemberCount,bCongressManCount,bSubMasterCount;
};
struct	SG_REMOVE_GUILD_MEMBER
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	char				strMemberName[dNAME_LENGTH];
	BYTE				bMemberCount,bCongressManCount,bSubMasterCount;
};

struct	SG_UPDATE_GUILD_LEVEL
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	WORD				wGuildLevel;
	DWORD				dwGuildExp;
	WORD				wGuildPoint,wExpTaxRate;
};



enum
{
	eGWM_YOU_ARE_NOT_GUILD_LEADER,
	eGWM_TOO_LONG_GUILD_NOTICE_LENGTH,
	eGWM_YOU_ARE_NOT_GUILD_MASTER,
	eGWM_MEMBER_COUNT_NOT_ONE_AT_DISSOLUTION_GUILD,
	eGWM_INCORRECT_MEMBER_NAME_LENGTH,
	eGWM_TOO_MANY_SUB_GUILD_MASTER,
	eGWM_TOO_MANY_GUILD_CONGRESS,
	eGWM_USER_NOT_GUILD_MEMBER,
	eGWM_USER_NOT_SUB_GUILD_MASTER,
	eGWM_GUILD_MASTER_CAN_NOT_LEAVE_GUILD,
	eGWM_LEAVE_GUILD_LIMIT_BY_JOIN_DATE,
	eGWM_JOIN_GUILD_LIMIT_BY_JOIN_DATE,
	eGWM_USER_ALREADY_SUB_GUILD_MASTER,
	eGWM_USER_ALREADY_GUILD_CONGRESS,
	eGWM_USER_NOT_GUILD_CONGRESS,
	eGWM_COMPLETE_ASK_WORK,
	eGWM_FAILED_ASK_WORK,
	eGWM_YOU_ARE_NOT_READY_TO_MAKE_GUILD,
	eGWM_INCORRECT_GUILD_NAME	,
	eGWM_INCORRECT_TAX_RATE		,

	eGWM_YOU_ARE_CAN_NOT_CHANGE_SKILL_LEVEL,
	eGWM_INCORRECT_SKILL_INDEX			,
	eGWM_CAN_NOT_INCREASE_LEVEL_SKILL	,
	eGWM_NOT_REMAIN_GUILD_SKILL_POINT	,
	eGWM_GUILD_BATTLE_SLEEP_TIME		,
	eGWM_YOU_ARE_ALREADY_GUILD_MEMBER	,
	eGWM_ONLY_GUILD_MASTER_CAN_SELECT_GUILD_MARK,
	eGWM_CHANGE_RANK_IN_GUILD,
	
	eGWM_CAN_NOT_BANISH_GUILD_MASTER,
	eGWM_CAN_NOT_BANISH_GUILD_ELDER,
	eGWM_CAN_NOT_BANISH_GUILD_CONGRESS,

	eGWM_FAILED_REQUEST_GUILD_BATTLE_BY_LOW_HALL_LEVEL	,
	eGWM_FAILED_REQUEST_GUILD_BATTLE_BY_FULL_REQUEST		,
	eGWM_FAILED_CANCEL_GUILD_BATTLE_BY_ALREADY_SETTING_BATTLE_SCHEDULE	,

	eGWM_LACK_INCREASE_MEMBER_COUNT_SKILL_LEVEL	,

};


struct	SG_GUILD_WORK_MESSAGE
{
	cMSG_BASE_TYPE		base;

	WORD				wMessage;
	WORD				wValue1,wValue2;
	char				strName[dNAME_LENGTH];
};

struct	SG_CHANGE_GUILD_TAX_RATE
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	WORD				wTaxRate;
};

struct	SG_UPDATE_GUILD_SKILL_LEVEL
{
	cMSG_BASE_TYPE	base;

	WORD			wGuildSerial,wSkill,wLevel,wRemainSkillPoint;
};

struct	SG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK
{
	cMSG_BASE_TYPE	base;

	WORD			wGuildSerial;
	WORD			wBattleCountAtWeek;
};

struct	SG_CHANGE_GUILD_NOTICE
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	char				strGuildNotice[dGUILD_NOTICE_LENGTH];
};

struct	SG_CHANGE_RANK_IN_GUILD
{
	cMSG_BASE_TYPE		base;

	WORD				wRank;
};


enum
{
	eREG_LEAVE,
	eREG_BANISH,
};

struct	SG_EXIT_GUILD
{
	cMSG_BASE_TYPE		base;

	WORD				wGuildSerial;
	WORD				wReason;
	char				strGuildName[dGUILD_NAME_LENGTH];
};



enum
{
	eAJGR_ALREADY_GUILD_MEMBER,
	eAJGR_TARGET_NOT_EXIST,
	eAJGR_TARGET_NOT_GUILD_MASTER,
	eAJGR_TARGET_NOT_GUILD_LEADER,
	eAJGR_TARGET_DENY_COMMUNITY,
	eAJGR_TARGET_DENY,
	eAJGR_TARGET_ACCEPT,
	eAJGR_TARGET_BUSY,
	eAJGR_GUILD_MEMBER_FULL,
	eAJGR_REQUIRE_CITIZEN_TITLE,
	eAJGR_GUILD_NOT_EXIST,
	eAJGR_FAILED,
};

struct	SG_ASK_JOIN_GUILD_RESULT
{
	cMSG_BASE_TYPE		base;

	WORD				wResult;
};

struct	SG_ASK_JOIN_GUILD
{
	cMSG_BASE_TYPE		base;

	WORD				wAskerSerial;
	char				strAskerName[dNAME_LENGTH];
	WORD				wLevel,wJob;
};











struct	SG_OUT
{
	cMSG_BASE_TYPE	base;

	char			strCause[128];
};








#define	dMAX_SPEECH_LENGTH	2048

struct	SG_COMPLEX_SPEECH
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	WORD			wEventArea;
	WORD			wContentsCount;
	WORD			wSpeechIndex;
	WORD			wDialogBoxSize;

	char			strText[dMAX_SPEECH_LENGTH];
};




enum
{
	eREF_NOT_OWNED_EVENT	,
	eREF_TOO_FAR			,
};



struct	SG_EVENT_FAILED
{
	cMSG_BASE_TYPE	base;
	WORD			wOwner;
	WORD			wReason;
};



struct	SG_END_DIAOLG
{
	cMSG_BASE_TYPE	base;
	WORD			wOwner;
};

#define	dMAX_ITEM_IN_SHOP	50

#define	dSHOP_NAME_LENGTH	32



struct	SG_OPEN_SHOP
{
	cMSG_BASE_TYPE	base;

	WORD			wOwner;
	WORD			wShopSerial;
	WORD			bf15SellItemCount	: 15;
	WORD			bf1IsDuelPointShop	: 1;
	WORD			bf1IsLevelPerGoldShop	:	1;
	WORD			bf15LevelPerGoldValue	:	15;
	WORD			wPriceFactor;
	DWORD			dwLimitPrice;
	WORD			wTokenItem;
	WORD			bf1IsNotBuyItem		:1;
	WORD			bf1IsBadgeOfTraderShop : 1;
	WORD			:0;
	char			strName[dSHOP_NAME_LENGTH];
	CItemDefine		aItem[dMAX_ITEM_IN_SHOP];
};




struct	SG_BUY_ITEM
{
	cMSG_BASE_TYPE	base;

	DWORD			dwPrice				;
	DWORD			bf1IsPigeonPostItem	:	1;
	DWORD			bf1IsDuelPointItem	:	1;
	WORD			wTokenShop;
	int				iRemainGold;
	WORD			wCount;

	CItemDefine		aBuyItem[dOWN_ITEM_COUNT+dEXTRA_INVENTORY_SIZE];
};



enum
{
	eFRBI_TOO_FAR_DISTANCE		,
	eFRBI_NPC_NOT_OWN_THAT_SHOP	,
	eFRBI_NOT_EXIST_SHOP		,
	eFRBI_NOT_EXIST_ITEM		,
	eFRBI_LACK_OF_GOLD			,
	eFRBI_INVENTORY_FULL		,
	eFRBI_CAN_NOT_OWN_SAME_ITEM	,
	eFRBI_TOO_MANY_BADGE_ITEM	,
	eFRBI_UNKONWN				,
	eFRBI_LACK_OF_DUEL_POINT	,
	eFRBI_LACK_OF_TOKEN		,
};


struct	SG_FAILED_BUY_ITEM
{
	cMSG_BASE_TYPE	base;

	int				iGold;
	WORD			wReason;
};




struct	SG_SELL_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wSellItemSlot;
	WORD			wCount;
	int				iPrice;
	int				iRemainGold;
};


enum
{
	eFRSI_TOO_FAR_DISTANCE		,
	eFRSI_NPC_NOT_OWN_THAT_SHOP	,
	eFRSI_NOT_EXIST_SHOP		,
	eFRSI_NOT_EXIST_ITEM		,
	eFRSI_NOT_MATCHED_ITEM_COUNT,
	eFRSI_CAN_NOT_SELL_ITEM		,
	eFRSI_OWN_TOO_MANY_GOLD		,
	eFRSI_CHANGE_ITEM_PRICE		,
	eFRSI_UNKONWN				,
};


struct	SG_FAILED_SELL_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wReason;
};



struct	SG_QUEST_INFO
{
	cMSG_BASE_TYPE		base;

	tsProcessQuestField	m_aProcessQuest[dMAX_PROCESS_QUEST_COUNT];
};



struct	SG_CHANGE_QUEST_INFO
{
	cMSG_BASE_TYPE		base;

	WORD				bf4ProcessSlot			:	4;
	WORD				bf1IsForQuestComplete	:	1;
	tsProcessQuestField	processQuest;
	WORD				wCountDownTime;
};



enum
{
	eCQR_SUCCESS,
	eCQR_FAILED,
};

struct	SG_CANCEL_QUEST
{
	cMSG_BASE_TYPE		base;

	WORD				wQuestSlot;
	WORD				wResult;
};










struct	SG_OPEN_BANK
{
	cMSG_BASE_TYPE	base;

	CBankInfo		bankInfo;
	WORD			wBankSerial,wBankSize,wBankCharge;
};

struct	SG_STORE_ITEM_TO_THE_BANK		
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wInventorySlot,wBankSlot;
};

struct	SG_STORE_GOLD_TO_THE_BANK		
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	int				iGold;
};

struct	SG_WITHDRAW_ITEM_FROM_THE_BANK	
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	WORD			wInventorySlot,wBankSlot;
};

struct	SG_WITHDRAW_GOLD_FROM_THE_BANK	
{
	cMSG_BASE_TYPE	base;

	DWORD			dwCheckSum;
	int				iGold;
};

struct	SG_MOVE_BANK_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wPos1,wPos2;
	DWORD			dwCheckSum;
};

struct	SG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION
{
	cMSG_BASE_TYPE	base;

	WORD			wPos1,wPos2;
	DWORD			dwCheckSum;
};



enum
{
	eMOB_WAIT_BANK_DATA,
	eMOB_WAIT_TO_BANK_OPEN_PERIOD_MESSAGE,
	eMOB_TOO_MANY_BANK_TRANSACTION,
	eMOB_CANCELED_BY_NOT_MATCHED_DATA_WITH_SERVER,
	eMOB_TRANSACTION_FINISH,
	eMOB_FAILED,
	eMOB_NOT_BANK_CLIENT,
	eMOB_CAN_NOT_STORE_ITEM,
	eMOB_CAN_OWN_SAME_ITEM,
	eMOB_TOO_MANY_BADGE_ITEM,
	eMOB_REMOVE_EXPIRED_ITEM,
	eMOB_OVER_TIME_ITEM_TERM_OF_VALIDATE,
	eMOB_TOO_MANY_BANK_TRANSACTION_WITH_REBIRTH,
	eMOB_IS_NOT_OWN_ITEM,
	eMOB_TOO_MANY_GOLD_BAR,
	eMOB_PLAYER_HAVE_TOO_MANY_GOLD_BAR,
};

struct	SG_BANK_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessage;
};









struct	SG_OPEN_CART
{
	cMSG_BASE_TYPE	base;

	WORD			wCartSerial;
	WORD			wItemCount;
	tsBuyLst		aItemList[dMAX_BUYLIST];
};



enum
{
	eCM_CART_CLOSE_BY_INCORRECT_INFO,
	eCM_INVENTORY_FULL				,
	eCM_FAILED_BY_INVENTORY_FULL	,
	eCM_FAILED_BY_VALID_ITEM_INFO	,
	eCM_EXIST_FAILED_PREMIUM_ITEM	,
	eCM_FAILED_BY_TOO_FAST_REASK	,
	eCM_ALREADY_OPEN_CART			,
	eCM_TOO_MANY_OPEN_SHOPPING_CART	,
	eCM_WAIT_CART_DATA				,
	eCM_NOT_OPENED_CART				,
	eCM_CAN_NOT_LOAD_THAT_PLACE		,
	eCM_ONLY_ONE_ITEM_IN_INVENTORY	,
	eCM_TOO_MANY_ITEM_COUNT					,
	eCM_ALREADY_EXIST_MORE_HIGH_LEVEL_ORB	,
	eCM_WAIT_FOR_PREVIOUS_WORK_RESULT		,
	eCM_LOTTERY_EVENT_COMMENT				,
	eCM_MAKE_RICE_SOUP_PLACE				,
	eCM_CAN_NOT_OWN_SAME_ITEM				,
	eCM_TOO_MANY_BADGE_ITEM					,
	eCM_ALREADY_EXIST_SAME_EFFECT_IN_EMBLEM	,
	eCM_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL	,
	eCM_CAN_NOT_WITHDRAW_CARROT_BY_OWN_LIMIT	,
	eCM_FAILED_BY_EXPIRED_ITEM					,
};

struct	SG_CART_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wCartMessage;
};

struct	SG_WITHDRAW_ITEM_FROM_THE_CART	
{
	cMSG_BASE_TYPE	base;

	WORD			wCartSlot;
	WORD			wInventorySlot;
	CItemDefine		item;
};



enum
{
	eDCIR_SUCCESS,
	eDCIR_FAILED,
	eDCIR_CAN_NOT_DESTROY_NOT_USED_ITEM,
};

struct	SG_DESTROY_CART_ITEM_RESULT
{
	cMSG_BASE_TYPE	base;

	WORD			wResult;
	WORD			wCartSlot;
};

struct	SG_CARROT_SHOP_INFO
{
	cMSG_BASE_TYPE	base;

	WORD			wIsOK;
	WORD			wIndex,wStatus,wCount;
	int				iRemainCarrotCount;

	cCarrotShopCategoryInfo2	categoryInfo;
	cCarrotShopCategoryInfo1	data;
};

struct	SG_OPEN_CARROT_SHOP
{
	cMSG_BASE_TYPE	base;

	int				iRemainCarrotCount;
};

struct	SG_LOAD_FAILED_CART_PREMIUM_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wInventorySlot;
	CItemDefine		item;
};











enum
{
	ePSM_OK,
	ePSM_FAILED,

	ePSM_SETUP_FAILED_BY_REQUIRE_DEALER_TITLE,
	ePSM_SETUP_FAILED_BY_EXIT_ANOTHER_PITCHMAN_SHOP,
	ePSM_SETUP_FAILED_BY_TOO_MANY_PITCHMAN_SHOP,
	ePSM_SETUP_FAILED,

	ePSM_FAILED_REGIST_ITEM_BY_ALREADY_REGISTED,
	ePSM_FAILED_REGIST_ITEM_BY_VALID_INVENTORY_SLOT,
	ePSM_FAILED_SHOP_NOT_OPENED,

	ePSM_FAILED_REMOVE_ITEM_BY_VALID_SLOT,

	ePSM_FAILED_CLOSE_SHOP_BY_NOT_OPENED,

	ePSM_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO,
	ePSM_FAILED_BUY_ITEM_BY_INSUFFICIENT_GOLD,
	ePSM_FAILED_BUY_ITEM_BY_FULL_INVENTORY,
	ePSM_FAILED_BUY_ITEM_BY_FULL_GOLD,

	ePSM_FAILED_OPEN_SHOP_BY_NOT_OPENED,

	ePSM_FAILED_BY_VALID_SHOP_SERIAL	,

	ePSM_FAILED_BY_CAN_NOT_TRADE_ITEM	,
	ePSM_FAILED_BY_CAN_NOT_TRADE_ITEM_IS_REVERSION_ITEM	,
	ePSM_FAILED_BY_INCORRECT_NAME,

	ePSM_FAILED_BUY_ITEM_BY_INSUFFICIENT_GOLD_BAR,
	ePSM_FAILED_BUY_ITEM_BY_OWNER_NOT_SLOT,
	ePSM_FAILED_BUY_ITEM_BY_MANY_GOLD_BAR,
	ePSM_CAN_NOT_OPEN_THIS_FIELD,
};

struct	SG_PITCHMAN_SHOP_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessage;
};

struct	SG_OPEN_PITCHMAN_SHOP
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
	WORD			wX,wY;
};

struct	SG_ADD_PITCHMAN_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wInventorySlot,wShopSlot;
	DWORD			dwPrice;
	BYTE			bDenomination;
};

struct	SG_REMOVE_PITCHMAN_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot;
};

struct	SG_CHANGE_PITCHMAN_SHOP_INFO
{
	cMSG_BASE_TYPE	base;
	
	CSimplePitchmanShopInfo		info;
	char						strName[dPITCHMAN_SHOP_NAME_LENGTH];
};

struct	SG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE
{
	cMSG_BASE_TYPE	base;

	WORD			wSlot1,wSlot2;
};

struct	SG_BUY_PITCHMAN_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	WORD			wPitchmanSerial;
	WORD			wSlot;
	CItemDefine		item;
	DWORD			dwPrice;
	BYTE			bDenomination;
	char			strOwnerName[dNAME_LENGTH];
};

struct	SG_SOLD_PITCHMAN_SHOP_ITEM
{
	cMSG_BASE_TYPE	base;

	char			strName[dNAME_LENGTH];
	WORD			wSlot;
	DWORD			dwPrice;
	BYTE			bDenomination;
	
};

struct	SG_ADD_PITCHMAN_SHOP
{
	cMSG_BASE_TYPE				base;

	CSimplePitchmanShopInfo		info;
};

struct	SG_REMOVE_PITCHMAN_SHOP
{
	cMSG_BASE_TYPE	base;

	WORD			wSerial;
};

struct	SG_GGG_INFO
{
	cMSG_BASE_TYPE	base;
	
	WORD			wValue[6];
};

struct	SG_UNION_PARTY_WORK
{
	cMSG_BASE_TYPE	base;
	
	WORD			wValue[6];
};

#define	dUNION_PARTY_MEMBER_STATE_NORMAL		0x00000000
#define	dUNION_PARTY_MEMBER_STATE_DISCONNECT	0x00000001
#define	dUNION_PARTY_MEMBER_STATE_OTHERFILED	0x00000002

class	CUnionPartyMemberInfo
{
public:
	WORD						m_wUnionPartySerial;
	char						m_strName[dNAME_LENGTH];
	WORD						m_wHP;
	DWORD						m_dwState;








};

struct	SG_UNION_PARTY_MEMBER_INFO
{
	cMSG_BASE_TYPE				base;
	
	WORD						wUnionPartySerial;
	WORD						wUnionPartyIndex;
	WORD						wPartyMemberIndex;
	CUnionPartyMemberInfo		Info;
};	

struct	SG_UNION_PARTY_INFO
{
	cMSG_BASE_TYPE				base;
	
	WORD						wUnionPartySerial;
	WORD						wUnionPartyIndex;
	WORD						wEffectivenessCount;
	CUnionPartyMemberInfo		Info[dMAX_UNION_PARTY_COUNT];
};	

#define	dPITCHMAN_SHOP_COUNT_FOR_LIST_PACKET		100

struct	SG_PITCHMAN_SHOP_LIST
{
	cMSG_BASE_TYPE				base;

	WORD						bf15Count : 15;
	WORD						bf1IsFirst: 1;

	CMinimumPitchmanShopInfo	aList[dPITCHMAN_SHOP_COUNT_FOR_LIST_PACKET];
};

#define	dPITCHMAN_SHOP_COUNT_FOR_VERY_SIMPLE_INFO_PACKET	10

struct	SG_SIMPLE_PITCHMAN_SHOP_INFO_LIST
{
	cMSG_BASE_TYPE				base;
	WORD						wCount;

	char						aData[(dPITCHMAN_SHOP_NAME_LENGTH+10)*dPITCHMAN_SHOP_COUNT_FOR_VERY_SIMPLE_INFO_PACKET];
};

struct	SG_PITCHMAN_SHOP_INFO
{
	cMSG_BASE_TYPE				base;

	CPitchmanShopForClient		shopInfo;
};

#define	dRESULT_ITEM_COUNT_OF_SEARCH_ITEM_IN_RELAY_STATION	20

struct	SG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION
{
	cMSG_BASE_TYPE	base;

	WORD			wCount;
	WORD			wShopSerial;
	WORD			wLastSlot;
	WORD			wIsFinish;

	CPitchmanShopItemInfoForClient	m_aItemList[dRESULT_ITEM_COUNT_OF_SEARCH_ITEM_IN_RELAY_STATION];
};






struct	SG_ADD_TRAP
{
	cMSG_BASE_TYPE				base;

	WORD		wSkill,wSerial,wPosX,wPosY,wRange;
	WORD		bf3Team			:	3;
	WORD		bf8DetectLevel	:	8;
};

struct	SG_REMOVE_TRAP
{
	cMSG_BASE_TYPE				base;

	WORD		wSerial;
};



struct	SG_EXPLOSION_TRAP
{
	cMSG_BASE_TYPE		base;

	DWORD				bf6TargetCount	: 6;
	DWORD				bf12Skill		: 12;
	DWORD				bf8Level		: 8;
	DWORD				bf1IsFirst		: 1;

	WORD				wPosX,wPosY;

	cWIDE_AREA_DAMAGE_E	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};

class	cTrapSkillDamageE
{
public:
	WORD			wActor;
	WORD			wPhysicalDamage,wMagicDamage;
};



struct	SG_EXPLOSION_TRAP_SKILL
{
	cMSG_BASE_TYPE		base;

	DWORD				bf6TargetCount	: 6;
	DWORD				bf11Skill		: 11;

	cTrapSkillDamageE	aData[dMAX_TARGET_FOR_WIDE_AREA_ATTACK];
};


class	cSimpleTrapInfo
{
public:
	WORD				m_wSerial;
	WORD				m_wX,m_wY;
	WORD				m_wRange;
	WORD				m_wSkill;
	WORD				m_bf3Team			:	3;
	WORD				m_bf8DetectLevel	:	8;
};

#define	dMAX_TRAP_COUNT_IN_FIELD_FOR_PACKET	20

struct	SG_EXIST_TRAP_LIST
{
	cMSG_BASE_TYPE		base;

	WORD				wTrapCount;
	cSimpleTrapInfo		aTrapInfo[dMAX_TRAP_COUNT_IN_FIELD_FOR_PACKET];
};

class	cSimpleFireWallInfo
{
public:
	WORD	m_wSkill,m_wX1,m_wY1,m_wX2,m_wY2;
};

const	int	c_iMaxFireWallCountForPacket	=	100;

struct	SG_EXIST_FIRE_WALL_LIST
{
	cMSG_BASE_TYPE		base;

	WORD				wWallCount;
	cSimpleFireWallInfo	aWallList[c_iMaxFireWallCountForPacket];
};

#define	dMAX_ACTIVE_TRAP_COUNT_IN_PACKET	32

struct	SG_EXIST_ACTIVE_FIELD_TRAP_LIST
{
	cMSG_BASE_TYPE		base;

	WORD				wTrapCount;
	WORD				awTrapList[dMAX_ACTIVE_TRAP_COUNT_IN_PACKET];
};







struct	SG_WARNING_MESSAGE
{
	cMSG_BASE_TYPE	base;

	WORD			wMessageIndex;
	WORD			wMessageType;
};

struct	SG_CONTROL_MONSTER
{
	cMSG_BASE_TYPE	base;

	WORD			wLord;
	WORD			wSlave;
};

struct	SG_OCCUR_EFFECT
{
	cMSG_BASE_TYPE	base;

	WORD			wX,wY,wEffect,wScale;
};

struct	SG_ACCOUNT_INFO
{
	cMSG_BASE_TYPE	base;

	int		aPlayerCountPerServer[10];
};

struct	SG_PENELTY_TIME
{
	cMSG_BASE_TYPE	base;

	DWORD	dwPeneltyTime;
};








struct	SG_SENDINFOTOADMIN
{
	cMSG_BASE_TYPE		base;

	DWORD		wUserCount,wMaxUserCount;
};

struct	SG_CLOSESERVER
{
	cMSG_BASE_TYPE		base;

	char		strMsg[16];
};







struct	CG_STRESSCLIENT
{
	cMSG_BASE_TYPE		base;

	WORD				wUserCount;
	WORD				wSendSize;
};

struct	CG_REGIST_STRESS_CLIENT
{
	cMSG_BASE_TYPE		base;

	int					iUniqueSerial;
};


struct	CG_MODIFY_STRESS_CLIENT
{
	cMSG_BASE_TYPE		base;
};


struct	CG_STRESSDATA
{
	cMSG_BASE_TYPE		base;
};


struct	SG_STRESSCLIENT
{
	cMSG_BASE_TYPE		base;

	
};

struct	SG_STRESS_SERVER_INFO
{
	cMSG_BASE_TYPE		base;

	int					iConnectUserCount;
	int					iUpdateTime;
};










const	int		c_iPacketTrailerDataSize	=	dMAX_PACKET_LENGTH-20;

struct	SG_PACKET_TRAILER
{
	cMSG_BASE_TYPE	base;

	int				m_iCount;

	char			m_aData[c_iPacketTrailerDataSize];
};






union	ALL_MSG
{
	cMSG_BASE_TYPE							base;



	BW_MAKE_DUMMY							mBW_MAKE_DUMMY;
	BW_DISPLACEMENT							mBW_DISPLACEMENT;
	BW_CHARMING								mBW_CHARMING;
	BW_EXIT_GAME							mBW_EXIT_GAME;
	BW_READY_TO_EXIT						mBW_READY_TO_EXIT;
	BW_DEATH_ACTOR							mBW_DEATH_ACTOR;

	BW_EXIT_GAME_FOR_MOVE_FIELD				mBW_EXIT_GAME_FOR_MOVE_FIELD;






	
	CG_MOVE									mCG_MOVE;
	CG_STOP									mCG_STOP;
	CG_TOGGLE_SITDOWN						mCG_TOGGLE_SITDOWN;
	CG_ACTION_TO_ACTOR						mCG_ACTION_TO_ACTOR;
	CG_ACTION_TO_GROUND						mCG_ACTION_TO_GROUND;
	CG_ACTION_QUICK							mCG_ACTION_QUICK;
	CG_SET_MOVE_ABILITY						mCG_SET_MOVE_ABILITY;
	CG_ASK_INFO_ACTOR_LIST					mCG_ASK_INFO_ACTOR_LIST;
	CG_OBSCURITY_ACTOR_LIST					mCG_OBSCURITY_ACTOR_LIST;
	CG_MISPLACED_ACTOR_LIST					mCG_MISPLACED_ACTOR_LIST;
	CG_ENGAGE_BATTLE						mCG_ENGAGE_BATTLE;
	CG_STOP_BATTLE							mCG_STOP_BATTLE;	
	CG_INCREASE_STATE						mCG_INCREASE_STATE;	
	CG_INCREASE_ABILITY_LEVEL				mCG_INCREASE_ABILITY_LEVEL;
	CG_ASK_ACTOR_INFO						mCG_ASK_ACTOR_INFO;
	CG_TRANSFORMATION						mCG_TRANSFORMATION;
	CG_MOVE_FIELD							mCG_MOVE_FIELD;

	CG_PICK_ITEM							mCG_PICK_ITEM;
	CG_EQUIP_ITEM							mCG_EQUIP_ITEM;
	CG_RELOAD_BELT_ITEM						mCG_RELOAD_BELT_ITEM;
	CG_USE_ITEM								mCG_USE_ITEM;
	CG_USE_ITEM_TO_ITEM						mCG_USE_ITEM_TO_ITEM;
	CG_USE_BELT_ITEM						mCG_USE_BELT_ITEM;
	CG_ASK_ITEM_DATA						mCG_ASK_ITEM_DATA;
	CG_DROP_ITEM							mCG_DROP_ITEM;
	CG_CHANGE_ITEM_PLACE					mCG_CHANGE_ITEM_PLACE;
	CG_STRIP_EQUIPMENT						mCG_STRIP_EQUIPMENT;
	CG_DESTROY_ITEM							mCG_DESTROY_ITEM;
	CG_SET_REACTION_STATUS					mCG_SET_REACTION_STATUS;
	CG_TURN_ON_PASSIVE_SKILL				mCG_TURN_ON_PASSIVE_SKILL;
	CG_RETURN_TO_VILLAGE					mCG_RETURN_TO_VILLAGE;
	CG_OPERATOR_COMMAND						mCG_OPERATOR_COMMAND;
	CG_EXIT_GAME							mCG_EXIT_GAME;

	CG_REQUEST_TRADE						mCG_REQUEST_TRADE;
	CG_TRADE_REPLY							mCG_TRADE_REPLY;
	CG_CANCEL_TRADE							mCG_CANCEL_TRADE;
	CG_ADD_TRADE_ITEM						mCG_ADD_TRADE_ITEM;
	CG_CHANGE_TRADE_ITEM_DATA				mCG_CHANGE_TRADE_ITEM_DATA;
	CG_REMOVE_TRADE_ITEM					mCG_REMOVE_TRADE_ITEM;
	CG_SET_TRADE_GOLD						mCG_SET_TRADE_GOLD;
	CG_READY_TRADE							mCG_READY_TRADE;
	CG_PERMIT_TRADE							mCG_PERMIT_TRADE;

	CG_SELECT_NPC							mCG_SELECT_NPC;
	CG_SELECT_SPEECH						mCG_SELECT_SPEECH;
	CG_BUY_ITEM								mCG_BUY_ITEM;
	CG_SELL_ITEM							mCG_SELL_ITEM;
	CG_CANCEL_QUEST							mCG_CANCEL_QUEST;
	CG_SUMMON_EVENT_MONSTER					mCG_SUMMON_EVENT_MONSTER;

	CG_ASK_OPEN_THE_PITCHMAN_SHOP			mCG_ASK_OPEN_THE_PITCHMAN_SHOP;
	CG_ADD_PITCHMAN_SHOP_ITEM				mCG_ADD_PITCHMAN_SHOP_ITEM;
	CG_REMOVE_PITCHMAN_SHOP_ITEM			mCG_REMOVE_PITCHMAN_SHOP_ITEM;
	CG_CHANGE_PITCHMAN_SHOP_INFO			mCG_CHANGE_PITCHMAN_SHOP_INFO;
	CG_BUY_PITCHMAN_SHOP_ITEM				mCG_BUY_PITCHMAN_SHOP_ITEM;
	CG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		mCG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE;
	CG_CLOSE_PITCHMAN_SHOP					mCG_CLOSE_PITCHMAN_SHOP;
	CG_ASK_PITCHMAN_SHOP_INFO				mCG_ASK_PITCHMAN_SHOP_INFO;
	CG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST	mCG_ASK_SIMPLE_PITCHMAN_SHOP_INFO_LIST;

	CG_STORE_ITEM_TO_THE_BANK				mCG_STORE_ITEM_TO_THE_BANK;
	CG_STORE_GOLD_TO_THE_BANK				mCG_STORE_GOLD_TO_THE_BANK;
	CG_WITHDRAW_ITEM_FROM_THE_BANK			mCG_WITHDRAW_ITEM_FROM_THE_BANK;
	CG_WITHDRAW_GOLD_FROM_THE_BANK			mCG_WITHDRAW_GOLD_FROM_THE_BANK;
	CG_MOVE_BANK_ITEM						mCG_MOVE_BANK_ITEM;
	CG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	mCG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION;	
	CG_FINISH_BANK_TRANSACTION				mCG_FINISH_BANK_TRANSACTION;
	CG_CANCEL_BANK_TRANSACTION				mCG_CANCEL_BANK_TRANSACTION;


	CG_CHAT									mCG_CHAT;
	CG_ASK_PARTY_LIST						mCG_ASK_PARTY_LIST;
	CG_ASK_PARTY_INFO						mCG_ASK_PARTY_INFO;
	CG_ASK_JOIN_PARTY						mCG_ASK_JOIN_PARTY;
	CG_ASK_JOIN_PARTY_REPLY					mCG_ASK_JOIN_PARTY_REPLY;
	CG_LEAVE_PARTY							mCG_LEAVE_PARTY;
	CG_PARTY_LEADER_WORK					mCG_PARTY_LEADER_WORK;
	CG_CHANGE_PARTY_NAME					mCG_CHANGE_PARTY_NAME;
	CG_FIND_USER							mCG_FIND_USER;
	CG_REQUEST_ADD_FRIEND					mCG_REQUEST_ADD_FRIEND;
	CG_REPLY_FOR_ADD_FRIEND					mCG_REPLY_FOR_ADD_FRIEND;
	CG_CHANGE_COMMUNITY_STATUS				mCG_CHANGE_COMMUNITY_STATUS;
	CG_DEBUG_MESSAGE						mCG_DEBUG_MESSAGE;


	CG_CREATE_GUILD							mCG_CREATE_GUILD;
	CG_ASK_GUILD_INFO						mCG_ASK_GUILD_INFO;
	CG_ASK_GUILD_MEMBER_LIST				mCG_ASK_GUILD_MEMBER_LIST;
	CG_ASK_JOIN_GUILD						mCG_ASK_JOIN_GUILD;
	CG_REPLY_ASK_JOIN_GUILD					mCG_REPLY_ASK_JOIN_GUILD;
	CG_DESTROY_CART_ITEM					mCG_DESTROY_CART_ITEM;

	CG_ASK_CHANGE_GUILD_NOTICE				mCG_ASK_CHANGE_GUILD_NOTICE;
	CG_ASK_CHANGE_GUILD_EXP_TAX				mCG_ASK_CHANGE_GUILD_EXP_TAX;
	CG_ASK_DISMISS_SUB_GUILD_MASTER			mCG_ASK_DISMISS_SUB_GUILD_MASTER;
	CG_ASK_APPOINTMENT_SUB_GUILD_MASTER		mCG_ASK_APPOINTMENT_SUB_GUILD_MASTER;
	CG_ASK_APPOINTMENT_GUILD_CONGRESS		mCG_ASK_APPOINTMENT_GUILD_CONGRESS;
	CG_ASK_DISMISS_GUILD_CONGRESS			mCG_ASK_DISMISS_GUILD_CONGRESS;
	CG_ASK_BANISH_GUILD_MEMBER				mCG_ASK_BANISH_GUILD_MEMBER;
	CG_ASK_LEAVE_GUILD						mCG_ASK_LEAVE_GUILD;
	CG_ASK_CHANGE_GUILD_MASTER				mCG_ASK_CHANGE_GUILD_MASTER;
	CG_READY_TO_MAKE_GUILD					mCG_READY_TO_MAKE_GUILD;
	CG_REGIST_REGULAR_GUILD_MEMBER			mCG_REGIST_REGULAR_GUILD_MEMBER;
	CG_INCREASE_GUILD_SKILL_LEVEL			mCG_INCREASE_GUILD_SKILL_LEVEL;
	CG_ASK_GUILD_BATTLE_SCHEDULE			mCG_ASK_GUILD_BATTLE_SCHEDULE;
	CG_REQUEST_GUILD_BATTLE					mCG_REQUEST_GUILD_BATTLE;
	CG_CANCEL_GUILD_BATTLE					mCG_CANCEL_GUILD_BATTLE;
	
	CG_CART_OPEN							mCG_CART_OPEN;
	CG_WITHDRAW_ITEM_FROM_THE_CART			mCG_WITHDRAW_ITEM_FROM_THE_CART;
	CG_CLOSE_CART							mCG_CLOSE_CART;
	CG_CALL_MAGIC_CARPET					mCG_CALL_MAGIC_CARPET;
	CG_REMEMBER_ORB_LOCATION				mCG_REMEMBER_ORB_LOCATION;
	CG_TELEPORT_TO_ORB_LOCATION				mCG_TELEPORT_TO_ORB_LOCATION;

	CG_STRESSCLIENT							mCG_STRESSCLIENT;







	SG_MOVE									mSG_MOVE;
	SG_STOP									mSG_STOP;
	SG_THRUST								mSG_THRUST;
	SG_CORRECT_PLAYER_POSITION				mSG_CORRECT_PLAYER_POSITION;
	SG_ACTION_TO_ACTOR						mSG_ACTION_TO_ACTOR;
	SG_ACTION_TO_GROUND						mSG_ACTION_TO_GROUND;
	SG_PLAYER_OUT							mSG_PLAYER_OUT;
	SG_JOIN_PLAYER							mSG_JOIN_PLAYER;
	SG_SET_MOVE_ABILITY						mSG_SET_MOVE_ABILITY;
	SG_REGEN_MONSTER						mSG_REGEN_MONSTER;
	SG_BASIC_ATTACK							mSG_BASIC_ATTACK;
	SG_CONTINUOUS_HIT_BY_CHARACTER			mSG_CONTINUOUS_HIT_BY_CHARACTER;
	SG_CONTINUOUS_HIT_BY_VALUE				mSG_CONTINUOUS_HIT_BY_VALUE;
	SG_LAND_MARKER							mSG_LAND_MARKER;
	SG_BUNSHINE_ATTACK						mSG_BUNSHINE_ATTACK;
	SG_SCIMITAR_CUTTING						mSG_SCIMITAR_CUTTING;
	SG_QUICK_ACTION							mSG_QUICK_ACTION;
	SG_KILL									mSG_KILL;
	SG_OUT									mSG_OUT;
	SG_WIDE_AREA_DAMAGE						mSG_WIDE_AREA_DAMAGE;
	SG_WIDE_AREA_EFFECT						mSG_WIDE_AREA_EFFECT;
	SG_GUARDIAN_POST						mSG_GUARDIAN_POST;
	SG_TRANSFORMATION						mSG_TRANSFORMATION;
	SG_MOVE_FIELD_RESULT					mSG_MOVE_FIELD_RESULT;
	SG_MOVE_FIELD_NOW						mSG_MOVE_FIELD_NOW;
	SG_TOGGLE_SITDOWN						mSG_TOGGLE_SITDOWN;
	SG_LEVEL_UP_OTHER						mSG_LEVEL_UP_OTHER;
	SG_CONTROL_MONSTER						mSG_CONTROL_MONSTER;
	SG_HIT_THORN_DAMAGE						mSG_HIT_THORN_DAMAGE;
	SG_ABSORB_DAMAGE						mSG_ABSORB_DAMAGE;
	SG_RELEASE_SHIMMERING_SHIELD			mSG_RELEASE_SHIMMERING_SHIELD;
	SG_SET_SHIMMERING_SHIELD				mSG_SET_SHIMMERING_SHIELD;
	SG_REMOVE_ACTOR							mSG_REMOVE_ACTOR;
	SG_FLOAT_MACHINE						mSG_FLOAT_MACHINE;
	SG_RELEASE_MACHINE						mSG_RELEASE_MACHINE;
	SG_SHOOT_MACHINE_MISSILE				mSG_SHOOT_MACHINE_MISSILE;
	SG_FIRE_TAIL_CHASER						mSG_FIRE_TAIL_CHASER;
	SG_MAKE_ACTION_FORM						mSG_MAKE_ACTION_FORM;
	SG_THRUST_ACTORS						mSG_THRUST_ACTORS;
	SG_USE_SKILL_TO_ACTOR					mSG_USE_SKILL_TO_ACTOR;
	SG_USE_SKILL_TO_GROUND					mSG_USE_SKILL_TO_GROUND;
	SG_RELEASE_DUMMY						mSG_RELEASE_DUMMY;
	SG_CREATE_DUMMY							mSG_CREATE_DUMMY;
	SG_RECEIVE_AID_MAGIC					mSG_RECEIVE_AID_MAGIC;
	SG_HIT_AURA_SKILL						mSG_HIT_AURA_SKILL;
	SG_RUSH									mSG_RUSH;
	SG_ACTIVE_REACTION_SKILL				mSG_ACTIVE_REACTION_SKILL;
	SG_ICY_STALAGMITE						mSG_ICY_STALAGMITE;
	SG_ACTOR_ENCHANTED_STATUS				mSG_ACTOR_ENCHANTED_STATUS;
	SG_ACTIVE_SKILL_EFFECT					mSG_ACTIVE_SKILL_EFFECT;

	SG_REMOVE_DROPPED_ITEMS					mSG_REMOVE_DROPPED_ITEMS;
	SG_REMOVE_DROPPED_ITEM					mSG_REMOVE_DROPPED_ITEM;
	SG_CHANGE_DROPPED_ITEM_INFO				mSG_CHANGE_DROPPED_ITEM_INFO;
	SG_DROPPING_ITEM						mSG_DROPPING_ITEM;
	SG_DROPPING_ITEMS						mSG_DROPPING_ITEMS;
	SG_PICK_ITEM_RESULT						mSG_PICK_ITEM_RESULT;
	SG_EQUIP_ITEM_RESULT					mSG_EQUIP_ITEM_RESULT;
	SG_USE_ITEM_RESULT						mSG_USE_ITEM_RESULT;
	SG_USE_BELT_ITEM_RESULT					mSG_USE_BELT_ITEM_RESULT;
	SG_USE_ITEM								mSG_USE_ITEM;
	SG_CHANGE_APPEARANCE					mSG_CHANGE_APPEARANCE;
	SG_HIT_ACTION							mSG_HIT_ACTION;

	SG_WARNING_MESSAGE						mSG_WARNING_MESSAGE;
	SG_CHAT									mSG_CHAT;
	SG_ADD_TRAP								mSG_ADD_TRAP;
	SG_REMOVE_TRAP							mSG_REMOVE_TRAP;
	SG_EXPLOSION_TRAP						mSG_EXPLOSION_TRAP;


	SG_STRESSCLIENT							mSG_STRESSCLIENT;


	SG_COMPLETE_SSL							mSG_COMPLETE_SSL;





};


#define	dMAX_STORE_PACKET_COUNT		256

class	cPACKET_INFO
{
public:
	enum
	{
		eSendedPacket	=	0x7ffffffe,
	};

	int				m_iOwner;
	ALL_MSG			m_packet;
};


class cPACKET_QUE
{
	cPACKET_INFO	m_aPacket[dMAX_STORE_PACKET_COUNT];

	int				m_iFirstQue,m_iLastQue,m_iCurrentQue,m_iCount,m_iCounter;

public:
	int				m_iMaxPacketCount;
	
					cPACKET_QUE()	{	reset();	}

	void			reset();
	inline	int		getCount(){return	m_iCount;}
	BOOL			addPacket(ALL_MSG *_lpPacket,int _iSerial);
	BOOL			isReceivedPacket(int _iPacket);
	cPACKET_INFO*	popPacket();

	cPACKET_INFO*	getOne();
	void			setFront()
	{	
		m_iCounter		=	m_iCount;
		m_iCurrentQue	=	m_iFirstQue;
	}

};

#endif