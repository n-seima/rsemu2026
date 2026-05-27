#ifndef	_classGLOBAL_MESSAGE_H
#define	_classGLOBAL_MESSAGE_H

#include "cNEMO.H"
#include "rsComm.H"

extern	char	**g_pstrCommonMessage;
extern	char	**g_pstrRegistServerMessage;
extern	char	**g_pstrSladerFilter;
extern	char	**g_pstrCorrectMagicResistance;
extern	char	**g_pstrTip;
extern	char	**g_pstrEnableAllignment;	//	착용/사용 가능한 성향
extern	char	**g_pstrEquipUseLimitContents;
extern	char	**g_pstrEquipmentName;	//	장비 이름
extern	char	**g_pstrGuildRank;
extern	char	**g_pstrHeroJobName;
extern	char	**g_pstrNpcName;
extern	char	**g_pstrMonsterGrade;
extern	char	**g_pstrMonsterType;
extern	char	**g_pstrSummonBeastName;
extern	char	**g_pstrNameFilter;
extern	char	**g_pstrExtraItemGrade;
extern	char	**g_pstrServerComment;
extern	char	**g_pstrStatusTooltip;
extern	char	**g_pstrMiniPetVoice;

extern  char	**g_pstrMonsterDicMessage;	//JBC	몬스터 도감 관련  08-07-28
extern	char	**g_pstrGreateGodGrace;		//JBC	거신의 은총관련 텍스트.	08-10-22

extern	char	g_strFontName[64];
extern	char	g_strMessageBoxTitleFontName[64];
extern	char	g_strCheckFontName[64];
extern	char	g_strPitchmanShopTextName[64];
extern	char	*g_lpstrBannerType[20];
extern	char	*g_lpstrGuildGuardianName[5];
extern	char	*g_lpstrGuildPetName[5];
extern	char	*g_lpstrRedStoneReceiver[3];

extern	char	*g_alpstrStateName[dSTATE_TYPE_COUNT];


#define	dMSG_CHARACTER_ANIMATION_DATA_LOAD_ERROR						g_pstrCommonMessage[0]
#define	dMSG_IS_HERO													g_pstrCommonMessage[1]
#define	dMSG_PROJECT_LOADING_ERROR										g_pstrCommonMessage[2]
#define	dMSG_CAN_NOT_OPEN_FOLLOW_FILE									g_pstrCommonMessage[3]
#define	dMSG_CAN_NOT_FIND_FOLLOW_FILE									g_pstrCommonMessage[4]
#define	dMSG_ITEM_INVENTORY_FULLED										g_pstrCommonMessage[5]

#define	dMSG_THIS_ITEM_DISABLE_PICK										g_pstrCommonMessage[6]
#define	dMSG_DURABILITY_ZERO											g_pstrCommonMessage[7]
#define	dMSG_SHIELD_DURABILITY_ZERO										g_pstrCommonMessage[8]
#define	dMSG_WEAPON_DURABILITY_ZERO										g_pstrCommonMessage[9]
#define	dMSG_CONNECT_TIME_OVERED										g_pstrCommonMessage[10]

#define	dMSG_CAN_NOT_CONNECT_TO_SEVER									g_pstrCommonMessage[11]
#define	dMSG_DISCONNECT_WITH_SERVER										g_pstrCommonMessage[12]
#define	dMSG_OPEN_PROJECT_ERROR											g_pstrCommonMessage[13]
#define	dMSG_CAN_NOT_FIND_ITEM_DATA_FILE								g_pstrCommonMessage[14]
#define	dMSG_CAN_NOT_FIND_JOB_DATA_FILE									g_pstrCommonMessage[15]

#define	dMSG_CAN_NOT_FIND_PROJECT_FOLDER								g_pstrCommonMessage[16]
#define	dMSG_MESSAGE													g_pstrCommonMessage[17]
#define	dMSG_CREATE_ID_OK												g_pstrCommonMessage[18]
#define	dMSG_OK															g_pstrCommonMessage[19]
#define	dMSG_CREATE_ID_ALREADY_EXIST_ID									g_pstrCommonMessage[20]

#define	dMSG_FAILED_UNKNOWN_REASON										g_pstrCommonMessage[21]
#define	dMSG_INCORRECT_CDKEY											g_pstrCommonMessage[22]
#define	dMSG_CDKEY_USED													g_pstrCommonMessage[23]
#define	dMSG_CDKEY_EXPIRED												g_pstrCommonMessage[24]
#define	dMSG_CREATE_ID_ID_FULLED										g_pstrCommonMessage[25]

#define	dMSG_YES														g_pstrCommonMessage[26]
#define	dMSG_NO															g_pstrCommonMessage[27]
#define	dMSG_LOG_IN_NOT_EXIST_ID										g_pstrCommonMessage[28]
#define	dMSG_LOG_IN_USER_FULLED											g_pstrCommonMessage[29]
#define	dMSG_LOG_IN_ALREADY_CONNECTED									g_pstrCommonMessage[30]

#define	dMSG_WAIT_LOGIN_RESULT											g_pstrCommonMessage[31]
#define	dMSG_WAIT_CREATE_ID_RESULT										g_pstrCommonMessage[32]
#define	dMSG_SUCCESS_CHANGE_PASSWORD									g_pstrCommonMessage[33]
#define	dMSG_INCORRECT_ORIGINAL_PASSWORD								g_pstrCommonMessage[34]
#define	dMSG_ID_CONNECTED												g_pstrCommonMessage[35]

#define	dMSG_NOT_EXIST_ID												g_pstrCommonMessage[36]
#define	dMSG_WAIT_CHANGE_PASSWORD_RESULT								g_pstrCommonMessage[37]
#define	dMSG_INCORRECT_PASSWORD											g_pstrCommonMessage[38]
#define	dMSG_ERROR														g_pstrCommonMessage[39]
#define	dMSG_CAN_NOT_CONNECT_TO_GAME_SERVER								g_pstrCommonMessage[40]

#define	dMSG_CONNECT_TO_GAME_SERVER										g_pstrCommonMessage[41]
#define	dMSG_CANCEL														g_pstrCommonMessage[42]
#define	dMSG_CAN_NOT_CONNECT_TO_LOGIN_SERVER							g_pstrCommonMessage[43]
#define	dMSG_CONNECT_TO_LOGIN_SERVER									g_pstrCommonMessage[44]
#define	dMSG_DO_YOU_REMOVE_SELECT_AVATAR								g_pstrCommonMessage[45]

#define	dMSG_REQUIRE_PATCH												g_pstrCommonMessage[46]
#define	dMSG_DO_PATCH													g_pstrCommonMessage[47]
#define	dMSG_SUCCESS_CREATE_AVATAR										g_pstrCommonMessage[48]
#define	dMSG_ALREADY_EXIST_SAME_NAME_CHARACTER							g_pstrCommonMessage[49]
#define	dMSG_CAN_NOT_CREATE_MORE_CHARACTER								g_pstrCommonMessage[50]

#define	dMSG_WAIT_RESULT_OF_CREATE_AVATAR								g_pstrCommonMessage[51]
#define	dMSG_REMOVE_SELECT_AVATAR										g_pstrCommonMessage[52]
#define	dMSG_FAILED_TO_REMOVE_AVATAR									g_pstrCommonMessage[53]
#define	dMSG_WAIT_PLAYER_DATA_AND_FIELD_DATA							g_pstrCommonMessage[54]
#define	dMSG_FIELD_FULL													g_pstrCommonMessage[55]

#define	dMSG_REALLY_EXIT												g_pstrCommonMessage[56]
#define	dMSG_CHATTING_METHOD											g_pstrCommonMessage[57]
#define	dMSG_SELECT														g_pstrCommonMessage[58]
#define	dMSG_TRANS_RUN													g_pstrCommonMessage[59]
#define	dMSG_TRANS_WALK													g_pstrCommonMessage[60]

#define	dMSG_TRANSFORMATION_FORM										g_pstrCommonMessage[61]
#define	dMSG_JOB_CHANGE_FORM											g_pstrCommonMessage[62]
#define	dMSG_LOGIN_FAILED												g_pstrCommonMessage[63]
#define	dMSG_FAILED_TO_JOIN_GAME										g_pstrCommonMessage[64]
#define	dMSG_CONNECT_OK_AND_WAIT_JOIN_RESULT							g_pstrCommonMessage[65]

#define	dMSG_EXP														g_pstrCommonMessage[66]
#define	dMSG_HP															g_pstrCommonMessage[67]
#define	dMSG_CP															g_pstrCommonMessage[68]
#define	dMSG_DEFENSE_POWER												g_pstrCommonMessage[69]
#define	dMSG_ATTACK_POWER												g_pstrCommonMessage[70]

#define	dMSG_STRENGTH													g_pstrCommonMessage[71]
#define	dMSG_HEALTH														g_pstrCommonMessage[72]
#define	dMSG_WISDOM														g_pstrCommonMessage[73]
#define	dMSG_CHARISMA													g_pstrCommonMessage[74]
#define	dMSG_AGILITY													g_pstrCommonMessage[75]

#define	dMSG_INTELLIGENCE												g_pstrCommonMessage[76]
#define	dMSG_LUCK														g_pstrCommonMessage[77]
#define	dMSG_PROGRAM_BEGINING_ERROR										g_pstrCommonMessage[78]
#define	dMSG_CAN_NOT_FIND_REGISTRY_DATA									g_pstrCommonMessage[79]
#define	dMSG_CAN_NOT_FIND_REGIST_FOLDER									g_pstrCommonMessage[80]

#define	dMSG_DODGE_CHANCE												g_pstrCommonMessage[81]
#define	dMSG_GOLD														g_pstrCommonMessage[82]
#define	dMSG_LEVEL														g_pstrCommonMessage[83]
#define	dMSG_ITEM_BASIC_INFO											g_pstrCommonMessage[84]
#define	dMSG_ITEM_SPECIAL_ABILITY										g_pstrCommonMessage[85]

#define	dMSG_COMMENT													g_pstrCommonMessage[86]
#define	dMSG_REQUIRE_ABILITY											g_pstrCommonMessage[87]
#define	dMSG_POINT														g_pstrCommonMessage[88]
#define	dMSG_REMAIN_EXP_FOR_LEVEL_UP									g_pstrCommonMessage[89]
#define	dMSG_ATTACK_SPEED												g_pstrCommonMessage[90]

#define	dMSG_SKILL_POINT												g_pstrCommonMessage[91]
#define	dMSG_INFORMATION												g_pstrCommonMessage[92]
#define	dMSG_REMAIN_POINT_FOR_LEVEL_UP									g_pstrCommonMessage[93]
#define	dMSG_SECOND														g_pstrCommonMessage[94]
#define	dMSG_REQUIRE_SKILL												g_pstrCommonMessage[95]

#define	dMSG_SKILL_LEVEL_UP_INFORMATION									g_pstrCommonMessage[96]
#define	dMSG_SKILL_DIFFICULTY											g_pstrCommonMessage[97]
#define	dMSG_WEAPON_ATTACK_POWER										g_pstrCommonMessage[98]
#define	dMSG_SPENT_CP													g_pstrCommonMessage[99]
#define	dMSG_GET_CP														g_pstrCommonMessage[100]

#define	dMSG_USE_ITEM													g_pstrCommonMessage[101]
#define	dMSG_DROP_ITEM													g_pstrCommonMessage[102]
#define	dMSG_CURRENT_EXP												g_pstrCommonMessage[103]
#define	dMSG_SKILL_EXP													g_pstrCommonMessage[104]
#define	dMSG_USEABLE_JOB												g_pstrCommonMessage[105]

#define	dMSG_MAN_EXCLUSIVE_ITEM											g_pstrCommonMessage[106]
#define	dMSG_WOMAN_EXCLUSIVE_ITEM										g_pstrCommonMessage[107]
#define	dMSG_ATTACK_RANGE												g_pstrCommonMessage[108]
#define	dMSG_NOT_TRANSFORMATION_JOB										g_pstrCommonMessage[109]
#define	dMSG_IS_NOT_TRANSFORMATION_TIME									g_pstrCommonMessage[110]

#define	dMSG_SITDOWN													g_pstrCommonMessage[111]
#define	dMSG_STATUS_INFO_WINDOW											g_pstrCommonMessage[112]
#define	dMSG_SKILL_INTERFACE											g_pstrCommonMessage[113]
#define	dMSG_QUEST_MENU													g_pstrCommonMessage[114]
#define	dMSG_PARTY_MENU													g_pstrCommonMessage[115]

#define	dMSG_SYSTEM_MENU												g_pstrCommonMessage[116]
#define	dMSG_NEXT_SKILL_SET												g_pstrCommonMessage[117]
#define	dMSG_PREVIOUS_SKILL_SET											g_pstrCommonMessage[118]
#define	dMSG_BELT_ITEM_RELOAD_TOOLTIP									g_pstrCommonMessage[119]
#define	dMSG_CAN_NOT_SITDOWN_IN_BATTLE									g_pstrCommonMessage[120]

#define	dMSG_ITEM_DATA_NOT_MATCHED_PLEASE_RETRY							g_pstrCommonMessage[121]
#define	dMSG_PICK_ITEM_FORM												g_pstrCommonMessage[122]
#define	dMSG_PICK_ITEM_INVENTORY_FULL									g_pstrCommonMessage[123]
#define	dMSG_PICK_ITEM_IS_OWNER											g_pstrCommonMessage[124]
#define	dMSG_PICK_ITEM_IS_VALID											g_pstrCommonMessage[125]

#define	dMSG_GET_EXP_FORM												g_pstrCommonMessage[126]
#define	dMSG_PICK_GOLD_FORM												g_pstrCommonMessage[127]
#define	dMSG_GOLD_FORM													g_pstrCommonMessage[128]
#define	dMSG_ITEMS_FORM													g_pstrCommonMessage[129]
#define	dMSG_CAN_NOT_USE_SKILL											g_pstrCommonMessage[130]

#define	dMSG_MOVE_FIELD_WRONG_AREA										g_pstrCommonMessage[131]
#define	dMSG_FILED_FULL													g_pstrCommonMessage[132]
#define	dMSG_ALREADY_EXIST_SAME_PLAYER									g_pstrCommonMessage[133]
#define	dMSG_THAT_FIELD_EXCLUSIVE_FOR_GUILD								g_pstrCommonMessage[134]
#define	dMSG_INCORRECT_CLIENT_VERSION									g_pstrCommonMessage[135]

#define	dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM								g_pstrCommonMessage[136]
#define	dMSG_MOVE_ITEM_COUNT											g_pstrCommonMessage[137]
#define	dMSG_OUT_OF_SKILL_RANGE_TARGET									g_pstrCommonMessage[138]
#define	dMSG_WEAPON_ATTACK_SPEED										g_pstrCommonMessage[139]
#define	dMSG_SKILL_ATTACK_SPEED_FORM									g_pstrCommonMessage[140]

#define	dMSG_RETURN_TO_VILLAGE											g_pstrCommonMessage[141]
#define	dMSG_RETURN_TO_LOGIN_MENU										g_pstrCommonMessage[142]
#define	dMSG_RETURN_TO_SELECT_AVATAR_MENU								g_pstrCommonMessage[143]
#define	dMSG_EXIT_TO_WINDOW												g_pstrCommonMessage[144]
#define	dMSG_OPTION														g_pstrCommonMessage[145]

#define	dMSG_CAN_NOT_TRANSFORMATION_BY_LOW_CP							g_pstrCommonMessage[146]
#define	dMSG_CAN_NOT_TRANSFORMATION_BY_SECOND_JOB_SKILL_NOT_FOUND		g_pstrCommonMessage[147]
#define	dMSG_CAN_NOT_TRANSFORMATION_BY_NOT_AVAIL_JOB					g_pstrCommonMessage[148]
#define	dMSG_CAN_NOT_TRANSFORMATION_BY_ETC								g_pstrCommonMessage[149]
#define	dMSG_TOO_FAR_DISTANCE_TARGET									g_pstrCommonMessage[150]

#define	dMSG_BUY														g_pstrCommonMessage[151]
#define	dMSG_SELL														g_pstrCommonMessage[152]
#define	dMSG_SELECT_ITEM_NOT_FOUND										g_pstrCommonMessage[153]
#define	dMSG_PRICE														g_pstrCommonMessage[154]
#define	dMSG_BUY_ITEM_FORM												g_pstrCommonMessage[155]

#define	dMSG_LACK_OF_GOLD												g_pstrCommonMessage[156]
#define	dMSG_INVENTORY_FULL												g_pstrCommonMessage[157]
#define	dMSG_CAN_NOT_BUY_ITEM_THIS_TIME									g_pstrCommonMessage[158]
#define	dMSG_SELL_PRICE_FORM											g_pstrCommonMessage[159]
#define	dMSG_CAN_NOT_SELL_ITEM_THIS_TIME								g_pstrCommonMessage[160]

#define	dMSG_SELL_ITEM_FORM												g_pstrCommonMessage[161]
#define	dMSG_BUY_ITEM_FAILED											g_pstrCommonMessage[162]
#define	dMSG_SELL_ITEM_FAILED											g_pstrCommonMessage[163]
#define	dMSG_BUY_ITEM													g_pstrCommonMessage[164]
#define	dMSG_SELL_ITEM													g_pstrCommonMessage[165]

#define	dMSG_OWN_MONEY													g_pstrCommonMessage[166]
#define	dMSG_GET_SKILL_EXP_FORM											g_pstrCommonMessage[167]
#define	dMSG_TOOLTIP_EMPTY_SKILL_SLOT									g_pstrCommonMessage[168]
#define	dMSG_FAILED														g_pstrCommonMessage[169]
#define	dMSG_ACTIVE_REACTION_FULL										g_pstrCommonMessage[170]

#define	dMSG_ACTIVATE_COUNTER_SKILL_FORM								g_pstrCommonMessage[171]
#define	dMSG_TURN_ON_COMPLETE_PROTECTION								g_pstrCommonMessage[172]
#define	dMSG_TURN_OFF_COMPLETE_PROTECTION								g_pstrCommonMessage[173]
#define	dMSG_CAN_NOT_USE_SKILL_LOWER_CP									g_pstrCommonMessage[174]
#define	dMSG_CAN_NOT_USE_SKILL_REQUIRE_WEAPON							g_pstrCommonMessage[175]

#define	dMSG_CAN_NOT_USE_SKILL_REQUIRE_SHIELD							g_pstrCommonMessage[176]
#define	dMSG_CAN_NOT_USE_SKILL_REQUIRE_BULLET							g_pstrCommonMessage[177]
#define	dMSG_IS_ACTION_TO_TARGET_SKILL									g_pstrCommonMessage[178]
#define	dMSG_THERE_IS_OUT_OF_ATTACK_RANGE								g_pstrCommonMessage[179]
#define	dMSG_THAT_SKILL_NOT_PLACED_TAHT_SKILL_SLOT						g_pstrCommonMessage[180]

#define	dMSG_NOT_LEARNED_SKILL											g_pstrCommonMessage[181]
#define	dMSG_NOT_THIS_JOB_SKILL											g_pstrCommonMessage[182]
#define	dMSG_THAT_SLOT_ARE_SUPPORT_SKILL_SLOT							g_pstrCommonMessage[183]
#define	dMSG_THAT_SKILL_NOT_LEFT_BUTTON_SKILL							g_pstrCommonMessage[184]
#define	dMSG_THAT_SKILL_NOT_RIGHT_BUTTON_SKILL							g_pstrCommonMessage[185]

#define	dMSG_CAN_NOT_USE_SKILL_TARGET									g_pstrCommonMessage[186]
#define	dMSG_ACTIVATE_SUPPORT_SKILL_FORM								g_pstrCommonMessage[187]
#define	dMSG_STANDUP													g_pstrCommonMessage[188]
#define	dMSG_CAN_NOT_TRANSFORMATION_IN_SITDOWN_STATUS					g_pstrCommonMessage[189]
#define	dMSG_IS_REQUIRE_TRANSFORMATION_CP_FORM							g_pstrCommonMessage[190]

#define	dMSG_REQUIRE_SECOND_JOB_SKILL_FORM								g_pstrCommonMessage[191]
#define	dMSG_RELEASE_TRANSFORMATION_BY_LOW_CP							g_pstrCommonMessage[192]
#define	dMSG_CAN_NOT_FIND_DEST_USER_FORM								g_pstrCommonMessage[193]
#define	dMSG_DEST_USER_IS_IGNORE_SAY_STATUS_FORM						g_pstrCommonMessage[194]
#define	dMSG_ITS_NOT_EQUIPABLE_ITEM										g_pstrCommonMessage[195]

#define	dMSG_TOOLTIP_FOR_EMPTY_BELT_SLOT								g_pstrCommonMessage[196]
#define	dMSG_NOT_EXIST_RELOAD_ABLE_ITEM									g_pstrCommonMessage[197]
#define	dMSG_RELOAD_BELT_ITEM											g_pstrCommonMessage[198]
#define	dMSG_ALREADY_RELOADING_BELT_SLOT								g_pstrCommonMessage[199]
#define	dMSG_RELOAD_BELT_ITEM_CANCEL									g_pstrCommonMessage[200]

#define	dMSG_EQUIP_WRONG_PLACE											g_pstrCommonMessage[201]
#define	dMSG_BELT_NOT_FOUND												g_pstrCommonMessage[202]
#define	dMSG_CAN_NOT_EQUIP_BELT_ITEM									g_pstrCommonMessage[203]
#define	dMSG_JOIN_FAIELD_BY_FIELD_FULL									g_pstrCommonMessage[204]
#define	dMSG_JOIN_FAIELD_BY_UNKNOWN_REASON								g_pstrCommonMessage[205]

#define	dMSG_REQUIRE_CP													g_pstrCommonMessage[206]
#define	dMSG_WEAPON_DAMAGE												g_pstrCommonMessage[207]
#define	dMSG_PARTY_JOIN_REQUEST											g_pstrCommonMessage[208]
#define	dMSG_BAN_PARTY													g_pstrCommonMessage[209]
#define	dMSG_RECEIVE_PARTY_JOIN_REQUEST									g_pstrCommonMessage[210]

#define	dMSG_ARE_YOU_JOIN_PARTY_FORM									g_pstrCommonMessage[211]
#define	dMSG_ADD_PARTYMEMBER_FORM										g_pstrCommonMessage[212]
#define	dMSG_CREATE_PARTY_MESSAGE										g_pstrCommonMessage[213]
#define	dMSG_JOIN_PARTY_FORM											g_pstrCommonMessage[214]
#define	dMSG_CREATE_PARTY												g_pstrCommonMessage[215]

#define	dMSG_YOU_LEAVE_PARTY											g_pstrCommonMessage[216]
#define	dMSG_LEAVE_PARTY_FORM											g_pstrCommonMessage[217]
#define	dMSG_CHANGE_PARTY_NAME_FORM										g_pstrCommonMessage[218]
#define	dMSG_FIRE_DAMAGE												g_pstrCommonMessage[219]
#define	dMSG_WATER_DAMAGE												g_pstrCommonMessage[220]

#define	dMSG_WIND_DAMAGE												g_pstrCommonMessage[221]
#define	dMSG_EARTH_DAMAGE												g_pstrCommonMessage[222]
#define	dMSG_LIGHT_DAMAGE												g_pstrCommonMessage[223]
#define	dMSG_DARK_DAMAGE												g_pstrCommonMessage[224]
#define	dMSG_ATTACK_POINT_COMMENT										g_pstrCommonMessage[225]

#define	dMSG_PHYSICAL_DAMAGE											g_pstrCommonMessage[226]
#define	dMSG_DISSOLUTION_PARTY											g_pstrCommonMessage[227]
#define	dMSG_CHANGE_PARTY_LEADER_FORM									g_pstrCommonMessage[228]
#define	dMSG_LEAVE_PARTY												g_pstrCommonMessage[229]
#define	dMSG_BANISH_MEMBER												g_pstrCommonMessage[230]

#define	dMSG_ASK_JOIN_PARTY												g_pstrCommonMessage[231]
#define	dMSG_NEXT_PARTY_LIST											g_pstrCommonMessage[232]
#define	dMSG_PREVIOUS_PARTY_LIST										g_pstrCommonMessage[233]
#define	dMSG_PLEASE_SELECT_PROPOSE_PARTY_PLAYER							g_pstrCommonMessage[234]
#define	dMSG_PARTY_LIST_COMMENT											g_pstrCommonMessage[235]

#define	dMSG_PROPOSE_PARTY_FORM											g_pstrCommonMessage[236]
#define	dMSG_PARTY_LEADER_AND_MEMBER_COUNT_FORM							g_pstrCommonMessage[237]
#define	dMSG_CHANGE_PARTY_PROSPECTUS_TEXT_KEYWORD						g_pstrCommonMessage[238]
#define	dMSG_PARTY_OBJECT_LENGTH_RULE									g_pstrCommonMessage[239]
#define	dMSG_ONLY_LEADER_CAN_CHNAGE_PARTY_OBJECT						g_pstrCommonMessage[240]

#define	dMSG_PM_TARGET_DEATH_OR_NOT_EXIST								g_pstrCommonMessage[241]	//	대상이 죽었거나 존재하지 않는다.
#define	dMSG_PM_TARGET_BELONG_TO_ALREADY_AT_DIFFERENT_PARTY				g_pstrCommonMessage[242]	//	이미 다른 파티에 소속해 있다.
#define	dMSG_PM_TARGET_NOT_BELONG_TO_PARTY								g_pstrCommonMessage[243]	//	그 녀석.. 우리 파티원이 아니다. -_-
#define	dMSG_PM_TARGET_NOT_EXIST										g_pstrCommonMessage[244]	//	그런넘 존재하지 않는다.
#define	dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE		g_pstrCommonMessage[245]	//	파티 리더만 초청 메시지를 보낼수 있습니다.

#define	dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_BANISH_MESSAGE			g_pstrCommonMessage[246]	//	파티 리더만 초청 메시지를 보낼수 있습니다.
#define	dMSG_PM_ONLY_PARTY_LEADER_CAN_SEND_AN_CHANGE_PARTY_NAME_MESSAGE	g_pstrCommonMessage[247]
#define	dMSG_PM_YOU_ARE_NOT_BELONG_TO_PARTY								g_pstrCommonMessage[248]	//	넌 파티에 소속되어 있지 않아!!
#define	dMSG_PM_ALREADY_FULL_PARTY										g_pstrCommonMessage[249]	//	풀 파티라서 더 이상 멤버를 받을 수 없다.
#define	dMSG_PM_ASKER_NOT_EXIST											g_pstrCommonMessage[250]	//	요청자가 없어졌다.

#define	dMSG_PM_DENY_INVITE_MESSAGE										g_pstrCommonMessage[251]	//	초청을 거부했다.
#define	dMSG_PM_DEST_PARTY_ARE_FULL										g_pstrCommonMessage[252]	//	풀 파티가 되어 버렸다.
#define	dMSG_PM_DEST_ACTOR_NOT_LEADER									g_pstrCommonMessage[253]	//	초청한넘... 리더 아냐-_-
#define	dMSG_PM_FAILED_TO_CREATE_PARTY									g_pstrCommonMessage[254]	//	파티 생성 실패
#define	dMSG_PM_FAILED_TO_JOIN_PARTY									g_pstrCommonMessage[255]	//	파티 참가에 실패

#define	dMSG_ARE_YOU_REALLY_LEAVE_PARTY									g_pstrCommonMessage[256]
#define	dMSG_ARE_YOU_REALLY_BANISH_SELECT_MEMBER						g_pstrCommonMessage[257]
#define	dMSG_NAME														g_pstrCommonMessage[258]
#define	dMSG_JOB														g_pstrCommonMessage[259]
#define	dMSG_INVENTORY_MENU												g_pstrCommonMessage[260]

#define	dMSG_GUILD_MENU													g_pstrCommonMessage[261]
#define	dMSG_FRIEND_MENU												g_pstrCommonMessage[262]
#define	dMSG_SELL_COUNT													g_pstrCommonMessage[263]
#define	dMSG_SELL_PRICE													g_pstrCommonMessage[264]
#define	dMSG_CAN_NOT_SELL_EQUIPPED_ITEM									g_pstrCommonMessage[265]

#define	dMSG_PREPARE_SELL_ITEM											g_pstrCommonMessage[266]
#define	dMSG_ITEM_COUNT_FORM											g_pstrCommonMessage[267]
#define	dMSG_WAIT_AVATAR_LIST											g_pstrCommonMessage[268]
#define	dMSG_AID_EFFECT													g_pstrCommonMessage[269]
#define	dMSG_INFORMATION_AFTER_LEVEL_UP									g_pstrCommonMessage[270]

#define	dMSG_BLOCKED_OBJECT_EXIST										g_pstrCommonMessage[271]
#define	dMSG_BGM														g_pstrCommonMessage[272]
#define	dMSG_EFFECT_SOUND												g_pstrCommonMessage[273]
#define	dMSG_PANNEL_HALF_BLENDING										g_pstrCommonMessage[274]
#define	dMSG_VIEW_SKILL_RANGE											g_pstrCommonMessage[275]

#define	dMSG_TOOLTIP_STATUS												g_pstrCommonMessage[276]
#define	dMSG_MINIMAP_OUTPUT_METHOD										g_pstrCommonMessage[277]
#define	dMSG_MINIMAP_NORMAL												g_pstrCommonMessage[278]
#define	dMSG_MINIMAP_HALFBLENDING										g_pstrCommonMessage[279]
#define	dMSG_MINIMAP_AUTO1												g_pstrCommonMessage[280]

#define	dMSG_MINIMAP_AUTO2												g_pstrCommonMessage[281]
#define	dMSG_WRONG_RING_PLACE											g_pstrCommonMessage[282]
#define	dMSG_LOW_POWER_FOR_EQUIP										g_pstrCommonMessage[283]
#define	dMSG_NOT_EQUIPMENT_ITEM											g_pstrCommonMessage[284]
#define	dMSG_NOT_EMBODED_SKILL											g_pstrCommonMessage[285]

#define	dMSG_CAN_NOT_USE_ITEM											g_pstrCommonMessage[286]
#define	dMSG_TARGET_TOO_FAR												g_pstrCommonMessage[287]
#define	dMSG_NOT_EMBODY_ITEM											g_pstrCommonMessage[288]
#define	dMSG_ROTTERY_TICKET_KWANG										g_pstrCommonMessage[289]
#define	dMSG_ID															g_pstrCommonMessage[290]

#define	dMSG_PASSWORD													g_pstrCommonMessage[291]
#define	dMSG_SERVER_STATUS												g_pstrCommonMessage[292]
#define	dMSG_SERVER_ON													g_pstrCommonMessage[293]
#define	dMSG_SERVER_OFF													g_pstrCommonMessage[294]
#define	dMSG_ROTTERY_TICKET_PRIZE_WINNING								g_pstrCommonMessage[295]

#define	dMSG_INCORRECT_CHARACTER_NAME									g_pstrCommonMessage[296]
#define	dMSG_REMOVE_CHARACTER											g_pstrCommonMessage[297]
#define	dMSG_CREATE_CHARACTER											g_pstrCommonMessage[298]
#define	dMSG_TOO_MANY_PLAYER											g_pstrCommonMessage[299]
#define	dMSG_ITEM_POWER_UP_METHOD										g_pstrCommonMessage[300]

#define	dMSG_CAN_NOT_SHOUT_REASON_BY_TIME								g_pstrCommonMessage[301]
#define	dMSG_PICK_PARTY_GOLD_FORM										g_pstrCommonMessage[302]
#define	dMSG_DEATH_MESSAGE1												g_pstrCommonMessage[303]
#define	dMSG_WAIT_SERVER_MESSAGE										g_pstrCommonMessage[304]
#define	dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_DATA						g_pstrCommonMessage[305]

#define	dMSG_THAT_SKILL_NOT_SUB_SKILL									g_pstrCommonMessage[306]
#define	dMSG_TOOLTIP_LEFT_SKILL_SLOT									g_pstrCommonMessage[307]
#define	dMSG_TOOLTIP_RIGHT_SKILL_SLOT									g_pstrCommonMessage[308]
#define	dMSG_TOOLTIP_SUB_SKILL_SLOT										g_pstrCommonMessage[309]
#define	dMSG_TOOLTIP_SUPPORT_SKILL_SLOT									g_pstrCommonMessage[310]

#define	dMSG_TOOLTIP_QUICK_SLOT_FORM									g_pstrCommonMessage[311]
#define	dMSG_ID_BLOCK_FORM												g_pstrCommonMessage[312]
#define	dMSG_TRADE														g_pstrCommonMessage[313]
#define	dMSG_PLEASE_SELECT_PROPOSE_TRADE_PLAYER							g_pstrCommonMessage[314]
#define	dMSG_PROPOSE_TRADE_FORM											g_pstrCommonMessage[315]

#define	dMSG_REQUEST_TRADE_FORM											g_pstrCommonMessage[316]
#define	dMSG_RECEIVE_TRADE_REQUEST										g_pstrCommonMessage[317]
#define	dMSG_READY_TRADE												g_pstrCommonMessage[318]
#define	dMSG_TRADE_BOX_COMMENT_FORM										g_pstrCommonMessage[319]
#define	dMSG_TRADE_BOX_CONFIRM_MESSAGE									g_pstrCommonMessage[320]

#define	dMSG_TRADE_WINDOW_FORM											g_pstrCommonMessage[321]
#define	dMSG_TM_DENY_TRADE												g_pstrCommonMessage[322]
#define	dMSG_TM_TOO_FAR_TARGET											g_pstrCommonMessage[323]
#define	dMSG_TM_NOT_EXIST												g_pstrCommonMessage[324]
#define	dMSG_TM_ALREADY_TRADE											g_pstrCommonMessage[325]

#define	dMSG_TM_DEATH_CHARACTER											g_pstrCommonMessage[326]
#define	dMSG_TM_TOO_MANY_TRADE											g_pstrCommonMessage[327]
#define	dMSG_TM_CANCEL													g_pstrCommonMessage[328]
#define	dMSG_TM_PLEASE_PERMIT_FORM										g_pstrCommonMessage[329]
#define	dMSG_TM_WAIT_PERMIT_FORM										g_pstrCommonMessage[330]

#define	dMSG_TM_TRADE_PERMIT											g_pstrCommonMessage[331]
#define	dMSG_TM_CANCEL_BY_INVENTORY_DATA_CHANGE							g_pstrCommonMessage[332]
#define	dMSG_TM_CANCEL_BY_INCORRECT_STATUS								g_pstrCommonMessage[333]
#define	dMSG_TM_TRADE_CANCEL											g_pstrCommonMessage[334]
#define	dMSG_LACK_ABILITY_ITEM_TO_USE									g_pstrCommonMessage[335]

#define	dMSG_RESTORE_EXP_FORM											g_pstrCommonMessage[336]
#define	dMSG_LOSE_EXP_FORM												g_pstrCommonMessage[337]
#define	dMSG_ACCUSE														g_pstrCommonMessage[338]
#define	dMSG_ALREADY_ACCUSED											g_pstrCommonMessage[339]
#define	dMSG_ACCUSE_COMMENT												g_pstrCommonMessage[340]

#define	dMSG_WRITE_CHAT_LOG												g_pstrCommonMessage[341]
#define	dMSG_SAY_METHOD													g_pstrCommonMessage[342]
#define	dMSG_ACCUSE_METHOD												g_pstrCommonMessage[343]
#define	dMSG_ADMIN_ICON_EXPLAIN											g_pstrCommonMessage[344]
#define	dMSG_ADMIN_ICON													g_pstrCommonMessage[345]

#define	dMSG_CAN_NOT_SAY_TO_OPERATOR									g_pstrCommonMessage[346]
#define	dMSG_EASY_SAY													g_pstrCommonMessage[347]
#define	dMSG_HATE_DOBAE													g_pstrCommonMessage[348]
#define	dMSG_TM_CAN_NOT_ASK_TRADE_TO_OPERATOR							g_pstrCommonMessage[349]
#define	dMSG_BAD_NAME_NOTICE											g_pstrCommonMessage[350]

#define	dMSG_DISPLAY_SHOT_KEY											g_pstrCommonMessage[351]
#define	dMSG_SAY														g_pstrCommonMessage[352]
#define	dMSG_SHOUT														g_pstrCommonMessage[353]
#define	dMSG_GUILD														g_pstrCommonMessage[354]
#define	dMSG_PARTY														g_pstrCommonMessage[355]

#define	dMSG_COMMUNITY_OPTION											g_pstrCommonMessage[356]
#define	dMSG_NOT_ALLOWED_SHOUT											g_pstrCommonMessage[357]
#define	dMSG_NOT_ALLOWED_SAY											g_pstrCommonMessage[358]
#define	dMSG_FIST_RING_LIMIT											g_pstrCommonMessage[359]
#define	dMSG_CAN_NOT_DROP_OR_TRADE_ITEM									g_pstrCommonMessage[360]

#define	dMSG_CAN_NOT_MAKE_NAME_BY_SLANDER_NAME							g_pstrCommonMessage[361]
#define	dMSG_PLEASE_USE_GOOD_WORD										g_pstrCommonMessage[362]
#define	dMSG_NOT_EXIST_PROCESS_QUEST									g_pstrCommonMessage[363]
#define	dMSG_RESET_STATE_POINT_COMMENT									g_pstrCommonMessage[364]
#define	dMSG_RESET_SKILL_POINT_COMMENT									g_pstrCommonMessage[365]

#define	dMSG_REMOVE_ID_WARNING											g_pstrCommonMessage[366]
#define	dMSG_REMAIN_TIME												g_pstrCommonMessage[367]
#define	dMSG_POSITION													g_pstrCommonMessage[368]
#define	dMSG_IS_TEST_SKILL												g_pstrCommonMessage[369]
#define	dMSG_IS_COPY_ITEM												g_pstrCommonMessage[370]

#define	dMSG_PLEASE_PICK_THROW_WEAPON									g_pstrCommonMessage[371]
#define	dMSG_PICK_THROW_WEAPON											g_pstrCommonMessage[372]
#define	dMSG_RETURN_THROW_WEAPON										g_pstrCommonMessage[373]
#define	dMSG_TEST_SERVER_NOTICE											g_pstrCommonMessage[374]
#define	dMSG_CAN_NOT_FIND_USER_FORM										g_pstrCommonMessage[375]

#define	dMSG_FIND_USER_RESULT_FORM										g_pstrCommonMessage[376]
#define	dMSG_COPY_DATA_TO_TEST_SERVER									g_pstrCommonMessage[377]
#define	dMSG_COMPLETE_COPY_DATA_TO_TEST_SERVER_WORK						g_pstrCommonMessage[378]
#define	dMSG_BASIC_FRIEND_GROUP											g_pstrCommonMessage[379]
#define	dMSG_ADD_FRIEND													g_pstrCommonMessage[380]

#define	dMSG_REMOVE_FRIEND												g_pstrCommonMessage[381]
#define	dMSG_FIND_FRIEND												g_pstrCommonMessage[382]
#define	dMSG_NORMAL_CHAT												g_pstrCommonMessage[383]
#define	dMSG_PARTY_CHAT													g_pstrCommonMessage[384]
#define	dMSG_GUILD_CHAT													g_pstrCommonMessage[385]

#define	dMSG_WHISPER_CHAT												g_pstrCommonMessage[386]
#define	dMSG_PLAYER_ARE_ALLREADY_FRIEND									g_pstrCommonMessage[387]
#define	dMSG_TOO_MANY_FRIEND											g_pstrCommonMessage[388]
#define	dMSG_ARE_YOU_REALLY_REMOVE_THAT_FRIEND_FORM						g_pstrCommonMessage[389]
#define	dMSG_PLEASE_SELECT_ADD_TO_FRIEND_CHARACTER						g_pstrCommonMessage[390]

#define	dMSG_DO_WHISPER													g_pstrCommonMessage[391]
#define	dMSG_PM_YOU_ARE_NOT_BELONG_TO_GUILD								g_pstrCommonMessage[392]
#define	dMSG_FRIEND_ICON												g_pstrCommonMessage[393]
#define	dMSG_FRIEND_ICON_EXPLAIN										g_pstrCommonMessage[394]
#define	dMSG_WAIT_BANK_DATA												g_pstrCommonMessage[395]

#define	dMSG_CAN_NOT_STORE_ITEM_TO_USED_SLOT							g_pstrCommonMessage[396]
#define	dMSG_NO_MORE_STORE_GOLD											g_pstrCommonMessage[397]
#define	dMSG_CAN_NOT_POSSESS_NO_MORE_THE_GOLD							g_pstrCommonMessage[398]
#define	dMSG_WAIT_TO_BANK_OPEN_PERIOD									g_pstrCommonMessage[399]
#define	dMSG_TOO_MANY_BANK_TRANSACTION									g_pstrCommonMessage[400]

#define	dMSG_CANCELED_BY_NOT_MATCHED_DATA_WITH_SERVER					g_pstrCommonMessage[401]
#define	dMSG_COMPLETE_BANK_TRANSACTION									g_pstrCommonMessage[402]
#define	dMSG_BANK_TRANSACTION_FAILED									g_pstrCommonMessage[403]
#define	dMSG_RECEIVE_BANK_DATA											g_pstrCommonMessage[404]
#define	dMSG_FINISH_BANK_TRANSACTION									g_pstrCommonMessage[405]

#define	dMSG_PLEASE_SELECT_ITEM_USE_CHARACTER							g_pstrCommonMessage[406]
#define	dMSG_PLEASE_SELECT_ITEM_USE_ITEM								g_pstrCommonMessage[407]
#define	dMSG_CAN_NOT_USE_ITEM_ON_DEST_ITEM								g_pstrCommonMessage[408]
#define	dMSG_CAN_NOT_USE_TO_EQUIPPED_ITEM								g_pstrCommonMessage[409]
#define	dMSG_ITEM_USE_FAILED_BY_EVENT20031101							g_pstrCommonMessage[410]

#define	dMSG_GET_TITLE_FORM												g_pstrCommonMessage[411]
#define	dMSG_REMOVED_TITLE_FORM											g_pstrCommonMessage[412]
#define	dMSG_TITLE_EFFECT												g_pstrCommonMessage[413]
#define	dMSG_MINIMUM_REQUIRE_LEVEL_FOR_TITLE_EFFECT						g_pstrCommonMessage[414]
#define	dMSG_CAN_NOT_ADAPTIVE_EFFECT_CAUSED_TOO_LOW_LEVEL				g_pstrCommonMessage[415]

#define	dMSG_LIMIT														g_pstrCommonMessage[416]
#define	dMSG_REAL_ADAPTIVE_LEVEL										g_pstrCommonMessage[417]
#define	dMSG_REQUIRE_LEVEL												g_pstrCommonMessage[418]
#define	dMSG_NEED_BANK_CLIENT_TITLE										g_pstrCommonMessage[419]
#define	dMSG_TARGET_DENY_COMMUNITY_STATUS								g_pstrCommonMessage[420]

#define	dMSG_DENY_ADD_FRIEND_MESSAGE									g_pstrCommonMessage[421]
#define	dMSG_CAN_NOT_FIND_TARGET										g_pstrCommonMessage[422]
#define	dMSG_REQEUST_ADD_FRIEND_FORM									g_pstrCommonMessage[423]
#define	dMSG_REQEUST_ADD_FRIEND_MESSAGE									g_pstrCommonMessage[424]
#define	dMSG_DENY_COMMUNITY												g_pstrCommonMessage[425]

#define	dMSG_RECEIVE_GOLD_BY_QUEST_FORM									g_pstrCommonMessage[426]
#define	dMSG_REMOVE_GOLD_BY_QUEST_FORM									g_pstrCommonMessage[427]
#define	dMSG_RECEIVE_ITEM_BY_QUEST_FORM									g_pstrCommonMessage[428]
#define	dMSG_REMOVE_ITEM_BY_QUEST_FORM									g_pstrCommonMessage[429]
#define	dMSG_CAN_NOT_SELL_ITEM											g_pstrCommonMessage[430]

#define	dMSG_GUILD_NAME													g_pstrCommonMessage[431]
#define	dMSG_SUB_GUILD_MASTER											g_pstrCommonMessage[432]
#define	dMSG_SUB_GUILD_MASTER_COMMENT									g_pstrCommonMessage[433]
#define	dMSG_QUEST_CLIENT_INFO											g_pstrCommonMessage[434]
#define	dMSG_QUEST_INFO													g_pstrCommonMessage[435]

#define	dMSG_ARE_YOU_REALLY_CANCEL_THAT_QUEST							g_pstrCommonMessage[436]
#define	dMSG_PROCESS_STATUS												g_pstrCommonMessage[437]
#define	dMSG_CANCEL_QUEST_FORM											g_pstrCommonMessage[438]
#define	dMSG_CANCEL_QUEST_FAILED										g_pstrCommonMessage[439]
#define	dMSG_REMOVE_QUEST_ITEM_FORM										g_pstrCommonMessage[440]

#define	dMSG_CAN_NOT_TRANSFORMATION_BY_YET_NOT_COMPLETE_ACTION			g_pstrCommonMessage[441]
#define	dMSG_DECISION													g_pstrCommonMessage[442]
#define	dMSG_PLEASE_INPUT_GUILD_NAME									g_pstrCommonMessage[443]
#define	dMSG_PLEASE_SELECT_SUB_GUILD_MASTER								g_pstrCommonMessage[444]
#define	dMSG_CREATE_GUILD_COMMENT										g_pstrCommonMessage[445]

#define	dMSG_HACKING_PROGRAM_FOUND										g_pstrCommonMessage[446]
#define	dMSG_CHECK_SYSTEM												g_pstrCommonMessage[447]
#define	dMSG_BANK_TRANSACTION_CHARGE_FORM								g_pstrCommonMessage[448]
#define	dMSG_BANK_TRANSACTION_LACK_CHARGE_FORM							g_pstrCommonMessage[449]
#define	dMSG_REDSTONE_EXCUTER_ERROR										g_pstrCommonMessage[450]

#define	dMSG_ALREADY_RUNNING_REDSTONE									g_pstrCommonMessage[451]
#define	dMSG_CAN_NOT_FIND_SOME_FILE										g_pstrCommonMessage[452]
#define	dMSG_REASON_OF_CAN_NOT_INCREASE_SKILL_POINT						g_pstrCommonMessage[453]
#define	dMSG_WAIT_FOR_BANK_TRANSACTION_RESULT							g_pstrCommonMessage[454]
#define	dMSG_REQUIRE_DIRECT_X											g_pstrCommonMessage[455]

#define	dMSG_CAN_NOT_CLOSE_ATTACK_ON_THIS_PLACE							g_pstrCommonMessage[456]
#define	dMSG_PICK_ERROR_COPY_ITEM										g_pstrCommonMessage[457]
#define	dMSG_PICK_ERROR_QUEST_ITEM										g_pstrCommonMessage[458]
#define	dMSG_QUEST_ITEM													g_pstrCommonMessage[459]
#define	dMSG_CAN_NOT_DROP_ITEM_FIELD_FULL								g_pstrCommonMessage[460]

#define	dMSG_CAN_NOT_EXIT_GAME_DURING_BATTLE_FORM						g_pstrCommonMessage[461]
#define	dMSG_FOUND_SPEED_HACK											g_pstrCommonMessage[462]
#define	dMSG_IGNORE_ENEMY_ATTACK										g_pstrCommonMessage[463]
#define	dMSG_WAIT_GUILD_INFO											g_pstrCommonMessage[464]
#define	dMSG_NOT_OWNED_GUILD											g_pstrCommonMessage[465]

#define	dMSG_CAN_NOT_STRIP_OR_EQUIP_EQUIPMENT_WHEN_ACTION				g_pstrCommonMessage[466]
#define	dMSG_FAILED_TO_JOIN_GAME_BY_EXIST_SAME_PLAYER					g_pstrCommonMessage[467]
#define	dMSG_EXPIRED_DAY_FORM											g_pstrCommonMessage[468]
#define	dMSG_CAN_NOT_CANCEL_QUEST										g_pstrCommonMessage[469]
#define	dMSG_DISSOLUTION_GUILD											g_pstrCommonMessage[470]

#define	dMSG_JOIN_GUILD_FORM											g_pstrCommonMessage[471]
#define	dMSG_LEAVE_GUILD_FORM											g_pstrCommonMessage[472]
#define	dMSG_GUILD_CREATE_FORM											g_pstrCommonMessage[473]
#define	dMSG_ASK_GUILD_JOIN												g_pstrCommonMessage[474]
#define	dMSG_PLEASE_SELECT_GUILD_MASTER									g_pstrCommonMessage[475]

#define	dMSG_ALREADY_GUILD_MEMBER										g_pstrCommonMessage[476]
#define	dMSG_TARGET_NOT_EXIST											g_pstrCommonMessage[477]
#define	dMSG_TARGET_NOT_GUILD_MASTER									g_pstrCommonMessage[478]
#define	dMSG_REQEUST_JOIN_GUILD_FORM									g_pstrCommonMessage[479]
#define	dMSG_REQEUST_JOIN_GUILD											g_pstrCommonMessage[480]

#define	dMSG_FULL_MEMBER_GUILD											g_pstrCommonMessage[481]
#define	dMSG_FAILED_TO_JOIN_GUILD										g_pstrCommonMessage[482]
#define	dMSG_GUILD_LEVEL_UP_FORM										g_pstrCommonMessage[483]
#define	dMSG_ADD_QUEST_FORM												g_pstrCommonMessage[484]
#define	dMSG_CAN_NOT_MOVE_FIELD_DURING_BATTLE_FORM						g_pstrCommonMessage[485]

#define	dMSG_GUILD_NOTICE												g_pstrCommonMessage[486]	//	길드 공지
#define	dMSG_GUILD_TAX													g_pstrCommonMessage[487]	//	길드 세율
#define	dMSG_LEAVE_GUILD												g_pstrCommonMessage[488]	//	길드 탈퇴
#define	dMSG_BANISH_GUILD_MEMBER										g_pstrCommonMessage[489]	//	길드원 추방
#define	dMSG_GUILD_CREATE_ERROR_EXIST_SAME_NAME_GUILD					g_pstrCommonMessage[490]

#define	dMSG_NOT_READY_TO_MAKE_GUILD									g_pstrCommonMessage[491]
#define	dMSG_GUILD_CREATE_ERROR_ALREADY_GUILD_MEMBER					g_pstrCommonMessage[492]
#define	dMSG_GUILD_CREATE_ERROR_LOW_POW									g_pstrCommonMessage[493]
#define	dMSG_CAN_NOT_CREATE_MORE_GUILD									g_pstrCommonMessage[494]
#define	dMSG_GUILD_CREATE_FAILED										g_pstrCommonMessage[495]

#define	dMSG_APPOINTMENT_SUB_GUILD_MASTER								g_pstrCommonMessage[496]
#define	dMSG_DISMISS_SUB_GUILD_MASTER									g_pstrCommonMessage[497]
#define	dMSG_ONLY_GUILD_LEADER_CHANGE_GUILD_NOTICE						g_pstrCommonMessage[498]
#define	dMSG_GUILD_NOTICE_RULE											g_pstrCommonMessage[499]
#define	dMSG_ONLY_GUILD_MASTER_APPOINTMENT_OR_DISMISS_SUB_GUILD_MASTER	g_pstrCommonMessage[500]

#define	dMSG_INCORRECT_NAME_LENGTH										g_pstrCommonMessage[501]
#define	dMSG_APPOINTMENT_SUB_GUILD_MASTER_FORM							g_pstrCommonMessage[502]
#define	dMSG_DISMISS_SUB_GUILD_MASTER_FORM								g_pstrCommonMessage[503]
#define	dMSG_ONLY_GUILD_MASTER_CHANGE_GUILD_TAX							g_pstrCommonMessage[504]
#define	dMSG_REALLY_CHANGE_GUILD_EXP_TAX_FORM							g_pstrCommonMessage[505]

#define	dMSG_ONLY_GUILD_MEMBER_LEAVE_GUILD								g_pstrCommonMessage[506]
#define	dMSG_ARE_YOU_LEAVE_GUILD										g_pstrCommonMessage[507]
#define	dMSG_ONLY_GUILD_MASTER_BANISH_MEMBER							g_pstrCommonMessage[508]
#define	dMSG_ARE_YOU_REALLY_BANISH_FOLLOW_GUILD_MEMBER					g_pstrCommonMessage[509]
#define	dMSG_YOU_ARE_NOT_GUILD_LEADER									g_pstrCommonMessage[510]

#define	dMSG_TOO_LONG_GUILD_NOTICE_LENTH								g_pstrCommonMessage[511]
#define	dMSG_YOU_ARE_NOT_GUILD_MASTER									g_pstrCommonMessage[512]
#define	dMSG_INCORRECT_GUILD_MEMBER_NAME_LENGTH							g_pstrCommonMessage[513]
#define	dMSG_USER_NOT_GUILD_MEMBER										g_pstrCommonMessage[514]
#define	dMSG_USER_NOT_SUB_GUILD_MASTER									g_pstrCommonMessage[515]

#define	dMSG_GUILD_MASTER_CAN_NOT_LEAVE_GUILD							g_pstrCommonMessage[516]
#define	dMSG_USER_ALREADY_SUB_GUILD_MASTER								g_pstrCommonMessage[517]
#define	dMSG_CHANGE_GUILD_MASTER										g_pstrCommonMessage[518]
#define	dMSG_ONLY_GUILD_MASTER_CHANGE_GUILD_MASTER						g_pstrCommonMessage[519]
#define	dMSG_CHANGE_GUILD_MASTER_FORM									g_pstrCommonMessage[520]

#define	dMSG_COMPLETE_ASK_GUILD_WORK									g_pstrCommonMessage[521]
#define	dMSG_CHANGE_GUILD_EXP_TAX_FORM									g_pstrCommonMessage[522]
#define	dMSG_CHANGED_GUILD_NOTICE										g_pstrCommonMessage[523]
#define	dMSG_CHANGED_GUILD_RANK_FORM									g_pstrCommonMessage[524]
#define	dMSG_FAILED_ASK_GUILD_WORK										g_pstrCommonMessage[525]

#define	dMSG_REGIST_REGULAR_GUILD_MEMBER								g_pstrCommonMessage[526]
#define	dMSG_ONLY_GUILD_MASTER_REGIST_REGULAR_GUILD						g_pstrCommonMessage[527]
#define	dMSG_REGIST_REGULARMEMBER_FORM									g_pstrCommonMessage[528]
#define	dMSG_READY_TO_MAKE_GUILD_ARE_YOU_MAKE_GUILD						g_pstrCommonMessage[529]
#define	dMSG_CAN_NOT_MOVE_BY_PREMIUM_ZONE								g_pstrCommonMessage[530]

#define	dMSG_YOU_ARE_NOT_READY_TO_MAKE_GUILD							g_pstrCommonMessage[531]
#define	dMSG_INCORRECT_GUILD_NAME										g_pstrCommonMessage[532]
#define	dMSG_DISMISS_GUILD_CONGRESS										g_pstrCommonMessage[533]
#define	dMSG_APPOINTMENT_GUILD_CONGRESS									g_pstrCommonMessage[534]
#define	dMSG_INCORRECT_TAX_RATE_FORM									g_pstrCommonMessage[535]

#define	dMSG_SPENT_HP													g_pstrCommonMessage[536]
#define	dMSG_LIGHT_DAMAGE_POINT											g_pstrCommonMessage[537]
#define	dMSG_CLOSE_SHOPPING_CART										g_pstrCommonMessage[538]
#define	dMSG_CLOSED_SHOPPING_CART_BY_NOT_MATCHED_DATA_WITH_SERVER		g_pstrCommonMessage[539]
#define	dMSG_CAN_NOT_WITHDRAW_ITEM_BY_FULL_INVENTORY					g_pstrCommonMessage[540]

#define	dMSG_FAILED_WITHDRAW_ITEM_BY_FULL_INVENTORY						g_pstrCommonMessage[541]
#define	dMSG_EXIST_LOAD_FAILED_PREMIUM_ITEM								g_pstrCommonMessage[542]
#define	dMSG_PLEASE_WAIT_FOR_NEXT_OPEN_CART								g_pstrCommonMessage[543]
#define	dMSG_ALREADY_OPENED_CART										g_pstrCommonMessage[544]
#define	dMSG_WAIT_FOR_CART_DATA											g_pstrCommonMessage[545]

#define	dMSG_NOT_OPENED_CART											g_pstrCommonMessage[546]
#define	dMSG_EXIST_ITEM_INVENTORY_SLOT									g_pstrCommonMessage[547]
#define	dMSG_TOO_MANY_OPENED_CART										g_pstrCommonMessage[548]
#define	dMSG_LOAD_FAILED_CART_PREMIUM_ITEM								g_pstrCommonMessage[549]
#define	dMSG_TRADE_COMMENT												g_pstrCommonMessage[550]

#define	dMSG_SHOPPING_CART_COMMENT										g_pstrCommonMessage[551]
#define	dMSG_ARE_YOU_OPEN_SHOPPING_CART									g_pstrCommonMessage[552]
#define	dMSG_RECEIVE_SHOPPING_CART_INFO									g_pstrCommonMessage[553]
#define	dMSG_NOT_SETTING_WAY_POINT										g_pstrCommonMessage[554]
#define	dMSG_WAY_POINT_INFO_FORM										g_pstrCommonMessage[555]

#define	dMSG_IS_CAN_NOT_USE_ITEM										g_pstrCommonMessage[556]
#define	dMSG_NOT_CONSUME_ITEM											g_pstrCommonMessage[557]
#define	dMSG_IS_NOT_CORRECT_STATUS_FOR_USE_ITEM							g_pstrCommonMessage[558]
#define	dMSG_RESURRECTION_BY_PERFECT_RESSURECTION_SCROLL				g_pstrCommonMessage[559]
#define	dMSG_RESURRECTION_BY_RESSURECTION_SCROLL						g_pstrCommonMessage[560]

#define	dMSG_UIR_INCORRECT_TARGET										g_pstrCommonMessage[561]
#define	dMSG_UIR_INCORRECT_TIMING										g_pstrCommonMessage[562]
#define	dMSG_CALL_MAGIC_CARPET											g_pstrCommonMessage[563]
#define	dMSG_ARE_YOU_REALLY_CALL_MAGIC_CARPET							g_pstrCommonMessage[564]
#define	dMSG_ARE_YOU_REALLY_RETURN_TO_THE_VILLAGE						g_pstrCommonMessage[565]

#define	dMSG_ONLY_ONE_ITEM_IN_INVENTORY									g_pstrCommonMessage[566]
#define	dMSG_WAIT_FOR_DISCONNECT_AND_SAFE_DATA_SAVE_FORM				g_pstrCommonMessage[567]
#define	dMSG_ARE_YOU_REALLY_RETURN_TO_TITLE								g_pstrCommonMessage[568]
#define	dMSG_ARE_YOU_REALLY_EXIT_GAME									g_pstrCommonMessage[569]
#define	dMSG_THAT_PORTAL_IS_ONE_WAY_PORTAL								g_pstrCommonMessage[570]

#define	dMSG_WAIT_FIELD_MOVE_RESULT										g_pstrCommonMessage[571]
#define	dMSG_ARE_YOU_REALLY_CANCEL_TO_MOVE_FIELD						g_pstrCommonMessage[572]
#define	dMSG_TRADE_WARNING_FORM											g_pstrCommonMessage[573]
#define	dMSG_WRITE_TRADE_LOG											g_pstrCommonMessage[574]
#define	dMSG_SCREEN_SHOT												g_pstrCommonMessage[575]

#define	dMSG_TOO_MANY_CART_ITEM_COUNT									g_pstrCommonMessage[576]
#define	dMSG_YET_REMAIN_CONNECT_USER_DATA								g_pstrCommonMessage[577]
#define	dMSG_ALREADY_EXIST_MORE_HIGH_LEVEL_ORB							g_pstrCommonMessage[578]
#define	dMSG_REMOVE_EXPIRED_ITEM_FORM									g_pstrCommonMessage[579]
#define	dMSG_REMEMBER_THIS_PLACE_TO_SLOT1								g_pstrCommonMessage[580]

#define	dMSG_REMEMBER_THIS_PLACE_TO_SLOT2								g_pstrCommonMessage[581]
#define	dMSG_WARP_TO_WAYPOINT_FORM										g_pstrCommonMessage[582]
#define	dMSG_INCORRECT_COMMAND											g_pstrCommonMessage[583]
#define	dMSG_LOTTERY_EVENT_COMMENT										g_pstrCommonMessage[584]
#define	dMSG_WAIT_FOR_PREVIOUS_CART_WORK_RESULT							g_pstrCommonMessage[585]

#define	dMSG_PREMIUM_ITEM_EVENT_COMMENT									g_pstrCommonMessage[586]
#define	dMSG_KOREA_NEW_YEAR_EVENT_COMMENT								g_pstrCommonMessage[587]
#define	dMSG_CAN_NOT_STORE_IN_BANK_ITEM									g_pstrCommonMessage[588]
#define	dMSG_BANK_CLIENT_LEVEL_LIMIT									g_pstrCommonMessage[589]
#define	dMSG_DESTROY_CART_ITEM											g_pstrCommonMessage[590]

#define	dMSG_ARE_YOU_REALLY_DESTROY_CART_ITEM_FORM						g_pstrCommonMessage[591]
#define	dMSG_CAN_NOT_DESTROY_NOT_USED_CART_ITEM							g_pstrCommonMessage[592]
#define	dMSG_DESTROY_ITEM												g_pstrCommonMessage[593]
#define	dMSG_ARE_YOU_REALLY_DESTROY_ITEM_FORM							g_pstrCommonMessage[594]
#define	dMSG_CAN_NOT_DESTROY_ITEM										g_pstrCommonMessage[595]

#define	dMSG_CAN_NOT_STRIP_WING											g_pstrCommonMessage[596]
#define	dMSG_CAN_NOT_USE_ORB_DURING_BATTLE_FORM							g_pstrCommonMessage[597]
#define	dMSG_UJMER_OK													g_pstrCommonMessage[598]
#define	dMSG_UJMER_ITEM_BROKEN											g_pstrCommonMessage[599]
#define	dMSG_UJMER_ITEM_DESTROYED										g_pstrCommonMessage[600]

#define	dMSG_UJMER_FAILED												g_pstrCommonMessage[601]
#define	dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_BY_PREFIX_FULL					g_pstrCommonMessage[602]
#define	dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM					g_pstrCommonMessage[603]
#define	dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_ITEM							g_pstrCommonMessage[604]
#define	dMSG_UJMER_TOO_LOW_LEVEL										g_pstrCommonMessage[605]

#define	dMSG_UJMER_IS_EXCLUSIVE_NOT_UNIQUE_ITEM							g_pstrCommonMessage[606]
#define	dMSG_UJMER_IS_EXCLUSIVE_UNIQUE_ITEM								g_pstrCommonMessage[607]
#define	dMSG_UEUIR_OK													g_pstrCommonMessage[608]
#define	dMSG_UEUIR_ITEM_BROKEN											g_pstrCommonMessage[609]
#define	dMSG_UEUIR_ITEM_DESTROYED										g_pstrCommonMessage[610]

#define	dMSG_UEUIR_CAN_NOT_ENCHANT_BY_PREFIX_FULL						g_pstrCommonMessage[611]
#define	dMSG_UEUIR_CAN_NOT_UPGRADE_PREFIX								g_pstrCommonMessage[612]
#define	dMSG_UEUIR_CAN_NOT_ATTACH_PREFIX_AT_BROKEN_ITEM					g_pstrCommonMessage[613]
#define	dMSG_UEUIR_CAN_NOT_ATTACH_PREFIX_ITEM							g_pstrCommonMessage[614]
#define	dMSG_UEUIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM							g_pstrCommonMessage[615]

#define	dMSG_UEUIR_IS_EXCLUSIVE_UNIQUE_ITEM								g_pstrCommonMessage[616]
#define	dMSG_RIR_OK														g_pstrCommonMessage[617]
#define	dMSG_RIR_OK_BUT_REMOVE_PREFIX									g_pstrCommonMessage[618]
#define	dMSG_RIR_FAILED													g_pstrCommonMessage[619]
#define	dMSG_RIR_USELESS_ITEM											g_pstrCommonMessage[620]

#define	dMSG_RIR_IS_EXCLUSIVE_NOT_UNIQUE_ITEM							g_pstrCommonMessage[621]
#define	dMSG_RIR_IS_EXCLUSIVE_UNIQUE_ITEM								g_pstrCommonMessage[622]
#define	dMSG_UEUIR_CAN_NOT_ATTACH_THAT_PREFIX_ITEM						g_pstrCommonMessage[623]
#define	dMSG_UJMER_CAN_NOT_ATTACH_THAT_PREFIX_ITEM						g_pstrCommonMessage[624]
#define	dMSG_IMPROVE_ITEM												g_pstrCommonMessage[625]

#define	dMSG_ARE_YOU_ENCHANT_ITEM_FORM									g_pstrCommonMessage[626]
#define	dMSG_ARE_YOU_ENCHANT_RING_ITEM_FORM								g_pstrCommonMessage[627]
#define	dMSG_BROKEN														g_pstrCommonMessage[628]
#define	dMSG_CAN_NOT_IMPROVE											g_pstrCommonMessage[629]
#define	dMSG_CURSED														g_pstrCommonMessage[630]

#define	dMSG_UJMER_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM					g_pstrCommonMessage[631]
#define	dMSG_UEUIR_CAN_NOT_ATTACH_PREFIX_AT_CURSED_ITEM					g_pstrCommonMessage[632]
#define	dMSG_RIR_IS_NOT_INCORRECT_ITEM									g_pstrCommonMessage[633]
#define	dMSG_REPAIR_ITEM												g_pstrCommonMessage[634]
#define	dMSG_ARE_YOU_REALLY_REPAIR_BROKEN_ITEM_BY_UNSKILLED_REPAIR_KIT	g_pstrCommonMessage[635]

#define	dMSG_GUILD_INFO													g_pstrCommonMessage[636]
#define	dMSG_GUILD_SKILL												g_pstrCommonMessage[637]
#define	dMSG_GUILD_ITEM													g_pstrCommonMessage[638]
#define	dMSG_MASTER														g_pstrCommonMessage[639]
#define	dMSG_GUILD_LEVEL												g_pstrCommonMessage[640]

#define	dMSG_GUILD_HALL													g_pstrCommonMessage[641]
#define	dMSG_NOTHING													g_pstrCommonMessage[642]
#define	dMSG_GUILD_EXP_POLICY											g_pstrCommonMessage[643]
#define	dMSG_NOT_GAIN_GUILD_EXP											g_pstrCommonMessage[644]
#define	dMSG_GUILD_EXP_DONATION_FORM									g_pstrCommonMessage[645]

#define	dMSG_GUILD_CAPITAL												g_pstrCommonMessage[646]
#define	dMSG_GUILD_MEMBER												g_pstrCommonMessage[647]
#define	dMSG_GUILD_MEMBER_COUNT_FORM									g_pstrCommonMessage[648]
#define	dMSG_GUILD_SUB_MASTER											g_pstrCommonMessage[649]
#define	dMSG_GUILD_SUB_MASTER_COUNT_FORM								g_pstrCommonMessage[650]

#define	dMSG_GUILD_CONGRESS_MAN											g_pstrCommonMessage[651]
#define	dMSG_GUILD_CONGRESS_MAN_COUNT_FORM								g_pstrCommonMessage[652]
#define	dMSG_TARGET_GUILD												g_pstrCommonMessage[653]
#define	dMSG_INVATION_GUILD												g_pstrCommonMessage[654]
#define	dMSG_EXIST														g_pstrCommonMessage[655]

#define	dMSG_CAN_NOT_OWN_SAME_ITEM										g_pstrCommonMessage[656]
#define	dMSG_TOO_MANY_BADGE_ITEM_FORM									g_pstrCommonMessage[657]
#define	dMSG_TRADE_MATE_TOO_MANY_BADGE_ITEM_FORM						g_pstrCommonMessage[658]
#define	dMSG_TRADE_MATE_CAN_NOT_OWN_SAME_ITEM							g_pstrCommonMessage[659]
#define	dMSG_GUILD_MEMBER_MINIMUM_LEVEL									g_pstrCommonMessage[660]

#define	dMSG_REQUIRE_INFO_FOR_INCREASE_GUILD_SKILL						g_pstrCommonMessage[661]
#define	dMSG_REQUIRE_GOLD												g_pstrCommonMessage[662]
#define	dMSG_REAL_APPLY_LEVEL											g_pstrCommonMessage[663]
#define	dMSG_REQUIRE_TITLE												g_pstrCommonMessage[664]
#define	dMSG_LITMIT_LEVEL												g_pstrCommonMessage[665]

#define	dMSG_GWM_YOU_ARE_CAN_NOT_CHANGE_SKILL_LEVEL						g_pstrCommonMessage[666]
#define	dMSG_GWM_INCORRECT_SKILL_INDEX									g_pstrCommonMessage[667]
#define	dMSG_GWM_CAN_NOT_INCREASE_LEVEL_SKILL							g_pstrCommonMessage[668]
#define	dMSG_GWM_NOT_REMAIN_GUILD_SKILL_POINT							g_pstrCommonMessage[669]
#define	dMSG_GUILD_BATTLE_SCHEDULE_FORM									g_pstrCommonMessage[670]

#define	dMSG_BATTLE_TIME												g_pstrCommonMessage[671]
#define	dMSG_NUMBER														g_pstrCommonMessage[672]
#define	dMSG_FIELD_NAME													g_pstrCommonMessage[673]
#define	dMSG_GUILD_MARK													g_pstrCommonMessage[674]
#define	dMSG_BATTLE_GUILD_NAME											g_pstrCommonMessage[675]

#define	dMSG_REQUEST_GUILD_BATTLE										g_pstrCommonMessage[676]
#define	dMSG_CLOSE														g_pstrCommonMessage[677]
#define	dMSG_NOT_EXIST_BATTLE_GUILD										g_pstrCommonMessage[678]
#define	dMSG_REQUEST_BATTLE												g_pstrCommonMessage[679]
#define	dMSG_REQEUST_GUILD_BATTLE_FORM									g_pstrCommonMessage[680]

#define	dMSG_GUILD_BATTLE_FORM											g_pstrCommonMessage[681]
#define	dMSG_GUILD_BATTLE												g_pstrCommonMessage[682]
#define	dMSG_SIEGE_WARFARE_OF_GUILD										g_pstrCommonMessage[683]
#define	dMSG_BATTLE_TIME_FORM											g_pstrCommonMessage[684]
#define	dMSG_AM															g_pstrCommonMessage[685]

#define	dMSG_PM															g_pstrCommonMessage[686]
#define	dMSG_HOUR														g_pstrCommonMessage[687]
#define	dMSG_MINUTE														g_pstrCommonMessage[688]
#define	dMSG_READY_TO_NEXT_GUILD_BATTLE_FORM							g_pstrCommonMessage[689]
#define	dMSG_CAN_NOT_USE_THIS_FIELD_ITEM								g_pstrCommonMessage[690]

#define	dMSG_PM_TARGET_NOT_OWN_FORCE									g_pstrCommonMessage[691]
#define	dMSG_WIN														g_pstrCommonMessage[692]
#define	dMSG_LOSE														g_pstrCommonMessage[693]
#define	dMSG_DRAW														g_pstrCommonMessage[694]
#define	dMSG_GUILD_BATTLE_RESULT_TEXT_FORM								g_pstrCommonMessage[695]

#define	dMSG_GUILD_BATTLE_RESULT										g_pstrCommonMessage[696]
#define	dMSG_GUILD_BATTLE_CONTINUE_WIN_FORM								g_pstrCommonMessage[697]
#define	dMSG_GUILD_BATTLE_GET_VICTORY_POINT_FORM						g_pstrCommonMessage[698]
#define	dMSG_GUILD_BATTLE_RESULT_FORM									g_pstrCommonMessage[699]
#define	dMSG_GUILD_BATTLE_SCHEDULE_CHANGE_RULE							g_pstrCommonMessage[700]

#define	dMSG_TEST_PORTAL_FOR_TEST										g_pstrCommonMessage[701]
#define	dMSG_GUILD_BATTLE_RESULT_BRIEFLY_TEXT_FORM						g_pstrCommonMessage[702]
#define	dMSG_IS_MOVE_BY_END_OF_GUILD_BATTLE								g_pstrCommonMessage[703]
#define	dMSG_UOR_CAN_NOT_WRITTEN_FIELD									g_pstrCommonMessage[704]
#define	dMSG_UOR_CAN_NOT_USE_AT_THIS_FIELD								g_pstrCommonMessage[705]

#define	dMSG_GUILD_MARK_COMPOSE											g_pstrCommonMessage[706]
#define	dMSG_MIXED_GUILD_MARK_SHAPE										g_pstrCommonMessage[707]
#define	dMSG_GUILD_MARK_ICON_SHAPE										g_pstrCommonMessage[708]
#define	dMSG_GUILD_MARK_ICON_COLOR_PATTERN								g_pstrCommonMessage[709]
#define	dMSG_GUILD_MARK_BASE_SHAPE										g_pstrCommonMessage[710]

#define	dMSG_GUILD_MARK_BASE_COLOR_PARTITION							g_pstrCommonMessage[711]
#define	dMSG_GUILD_MARK_BASE_COLOR_PARTTERN								g_pstrCommonMessage[712]
#define	dMSG_GUILD_MARK_OUTLINE_COLOR_PARTTERN							g_pstrCommonMessage[713]
#define	dMSG_HANGUL_INITIAL												g_pstrCommonMessage[714]
#define	dMSG_ENGLISH_INITIAL											g_pstrCommonMessage[715]

#define	dMSG_HANJA_INITIAL												g_pstrCommonMessage[716]
#define	dMSG_ITEM_SHAPE													g_pstrCommonMessage[717]
#define	dMSG_ANIMAL_SHAPE												g_pstrCommonMessage[718]
#define	dMSG_SYMBOL_PATTERN												g_pstrCommonMessage[719]
#define	dMSG_UNIQUE1													g_pstrCommonMessage[720]

#define	dMSG_UNIQUE2													g_pstrCommonMessage[721]
#define	dMSG_CHANGE_GUILD_MARK											g_pstrCommonMessage[722]
#define	dMSG_SELECT_GUILD_MARK_BY_YOU									g_pstrCommonMessage[723]
#define	dMSG_GUILD_MARK_BASIC_RULE										g_pstrCommonMessage[724]
#define	dMSG_GWM_ONLY_GUILD_MASTER_CAN_SELECT_GUILD_MARK				g_pstrCommonMessage[725]

#define	dMSG_GMC_RQUIRE_GUILD_MARK_EXPAND_PACK							g_pstrCommonMessage[726]
#define	dMSG_GMC_RQUIRE_GUILD_MARK_SPECIAL_PACK							g_pstrCommonMessage[727]
#define	dMSG_GMC_RQUIRE_GUILD_MARK_LUXURY_PACK							g_pstrCommonMessage[728]
#define	dMSG_GMC_RQUIRE_GUILD_MARK_HANGUL_PACK							g_pstrCommonMessage[729]
#define	dMSG_GMC_RQUIRE_GUILD_MARK_ENGLISH_PACK							g_pstrCommonMessage[730]

#define	dMSG_GMC_RQUIRE_GUILD_MARK_HANMUN_PACK							g_pstrCommonMessage[731]
#define	dMSG_GMC_RQUIRE_GUILD_MARK_ITEM_PACK							g_pstrCommonMessage[732]
#define	dMSG_GMC_RQUIRE_GUILD_MARK_ANIMAL_PACK							g_pstrCommonMessage[733]
#define	dMSG_GMC_OR														g_pstrCommonMessage[734]
#define	dMSG_GUILD_MARK_SELECT_CAN_NOT_SELECT_ABLE_SYMBOL				g_pstrCommonMessage[735]

#define	dMSG_GUILD_MARK_SELECT_CAN_NOT_SELECT_ABLE_BASE_SHAPE			g_pstrCommonMessage[736]
#define	dMSG_GUILD_MARK_SELECT_CAN_NOT_SELECT_ABLE_BASE_PARTITION		g_pstrCommonMessage[737]
#define	dMSG_IS_MOVE_BY_NOT_GUILD_BATTLE_TIME							g_pstrCommonMessage[738]
#define	dMSG_IS_MOVE_BY_ALREADY_OX_QUIZ_PROCESS							g_pstrCommonMessage[739]
#define	dMSG_IS_MOVE_BY_NOT_OX_QUIZ_TIME								g_pstrCommonMessage[740]

#define	dMSG_DISAPPEAR_ITEM_FORM										g_pstrCommonMessage[741]
#define	dMSG_ANNOUNCE_QUIZ_EVENT_FORM									g_pstrCommonMessage[742]
#define	dMSG_OX_QUIZ_RESULT_FORM										g_pstrCommonMessage[743]
#define	dMSG_END_OX_QUIZ_FORM											g_pstrCommonMessage[744]
#define	dMSG_TM_MINUMUM_LEVEL											g_pstrCommonMessage[745]

#define	dMSG_WAIT_MESSAGE_FROM_SERVER									g_pstrCommonMessage[746]
#define	dMSG_CANCEL_OX_QUIZ_FORM										g_pstrCommonMessage[747]
#define	dMSG_CANCEL_OX_QUIZ_IN_OX_QUIZ_FIELD_FORM						g_pstrCommonMessage[748]
#define	dMSG_YOU_ARE_LOSE_TO_QUIZ_EVENT									g_pstrCommonMessage[749]
#define	dMSG_IS_MOVE_BY_END_OF_OX_QUIZ									g_pstrCommonMessage[750]

#define	dMSG_AJGR_REQUIRE_CITIZEN_TITLE									g_pstrCommonMessage[751]
#define	dMSG_OX_QUIZ_TAIL												g_pstrCommonMessage[752]
#define	dMSG_SURVIVER													g_pstrCommonMessage[753]
#define	dMSG_LOSER														g_pstrCommonMessage[754]
#define	dMSG_QUIZ_EVENT_FIELD_KEY_HELP									g_pstrCommonMessage[755]

#define	dMSG_QUIZ_EVENT_QUESTION_FORM									g_pstrCommonMessage[756]
#define	dMSG_IS_TEST_EVENT												g_pstrCommonMessage[757]
#define	dMSG_FRSI_OWN_TOO_MANY_GOLD										g_pstrCommonMessage[758]
#define	dMSG_REQUIRE_CARTOGRAPHER_LEVEL_FORM							g_pstrCommonMessage[759]
#define	dMSG_CAN_NOT_INCREASE_MINIMAP_SIZE_REQUIRE_MORE_HIGH_CARTOGRAPHER_TITLE	g_pstrCommonMessage[760]

#define	dMSG_CLOSE_SHOP_BY_ITEM_PRICE									g_pstrCommonMessage[761]
#define	dMSG_BEGIN_REVIVE_OX_QUIZ_LOSER_BATTLE							g_pstrCommonMessage[762]
#define	dMSG_FOR_OX_QUIZ_WINNER_WHEN_BEGIN_REVIVE_OX_QUIZ_LOSER_BATTLE	g_pstrCommonMessage[763]
#define	dMSG_WINNER														g_pstrCommonMessage[764]
#define	dMSG_PROCESS_OX_QUIZ											g_pstrCommonMessage[765]

#define	dMSG_PROCESS_REVIVE_OX_QUIZ_LOSER_BATTLE_FORM					g_pstrCommonMessage[766]
#define	dMSG_STATUS														g_pstrCommonMessage[767]
#define	dMSG_FOR_OX_QUIZ_LOSER_WHEN_BEGIN_REVIVE_OX_QUIZ_LOSER_BATTLE	g_pstrCommonMessage[768]
#define	dMSG_BEGIN_OX_QUIZ												g_pstrCommonMessage[769]
#define	dMSG_WAIT_BEGIN_OX_QUIZ											g_pstrCommonMessage[770]

#define	dMSG_COMER														g_pstrCommonMessage[771]
#define	dMSG_DO_YOU_REALLY_RETURN_TO_THE_VILLAGE_STOP_THE_EVENT			g_pstrCommonMessage[772]
#define	dMSG_NOT_APT_USED_SKILL_TO_TARGET								g_pstrCommonMessage[773]
#define	dMSG_CREATE_CHARACTER_CHOICE_JOB								g_pstrCommonMessage[774]
#define	dMSG_CREATE_CHARACTER_NAME										g_pstrCommonMessage[775]

#define	dMSG_REMOVE_EXPIRED_ITEM_IN_BANK								g_pstrCommonMessage[776]
#define	dMSG_OVER_TIME_ITEM_TERM_OF_VALIDATE_IN_BANK					g_pstrCommonMessage[777]
#define	dMSG_ACROSS_THE_BLOCKED_OBJECT_TO_JUMP							g_pstrCommonMessage[778]
#define	dMSG_TOO_CLOSE_TO_TARGET										g_pstrCommonMessage[779]
#define	dMSG_ONLY_GUILD_MASTER_APPOINTMENT_OR_DISMISS_GUILD_CONGRESS	g_pstrCommonMessage[780]

#define	dMSG_APPOINTMENT_GUILD_CONGRESS_FORM							g_pstrCommonMessage[781]
#define	dMSG_DISMISS_GUILD_CONGRESS_FORM								g_pstrCommonMessage[782]
#define	dMSG_GWM_USER_ALREADY_GUILD_CONGRESS							g_pstrCommonMessage[783]
#define	dMSG_GWM_USER_NOT_GUILD_CONGRESS								g_pstrCommonMessage[784]
#define	dMSG_GWM_TOO_MANY_SUB_GUILD_MASTER								g_pstrCommonMessage[785]

#define	dMSG_GWM_TOO_MANY_GUILD_CONGRESS								g_pstrCommonMessage[786]
#define	dMSG_MEMBER_LIST												g_pstrCommonMessage[787]
#define	dMSG_MANAGE_TOOL												g_pstrCommonMessage[788]
#define	dMSG_RANK_IN_GUILD												g_pstrCommonMessage[789]
#define	dMSG_SORT_METHOD												g_pstrCommonMessage[790]

#define	dMSG_CHANGE_GUILD_NOTICE										g_pstrCommonMessage[791]
#define	dMSG_CHANGE_GUILD_EXP_TAX_RATE									g_pstrCommonMessage[792]
#define	dMSG_PLEASE_INPUT_GUILD_NOTICE									g_pstrCommonMessage[793]
#define	dMSG_CHANGE_GUILD_NOTICE_FORM									g_pstrCommonMessage[794]
#define	dMSG_CHANGE_GUILD_EXP_TAX_RATE_FORM								g_pstrCommonMessage[795]

#define	dMSG_READY_TO_OPEN_SHOP											g_pstrCommonMessage[796]
#define	dMSG_SELLING_PITCHMAN_SHOP										g_pstrCommonMessage[797]
#define	dMSG_BEGIN_PITCHMAN_SHOP										g_pstrCommonMessage[798]
#define	dMSG_STOP_PITCHMAN_SHOP											g_pstrCommonMessage[799]
#define	dMSG_CLOSE_PITCHMAN_SHOP										g_pstrCommonMessage[800]

#define	dMSG_REQUIRE_DEALER_TITLE_FOR_OPEN_PITCHMAN_SHOP				g_pstrCommonMessage[801]
#define	dMSG_EXIST_ANOTHER_PITCHMAN_SHOP								g_pstrCommonMessage[802]
#define	dMSG_TOO_MANY_PITCHMAN_SHOP										g_pstrCommonMessage[803]
#define	dMSG_FAILED_OPEN_PITCHMAN_SHOP									g_pstrCommonMessage[804]
#define	dMSG_CAN_NOT_ADD_ITEM_BY_ALREADY_ADDED_PITCHMAN_SHOP_ITEM		g_pstrCommonMessage[805]

#define	dMSG_CAN_NOT_ADD_ITEM_BY_VALID_INVENTORY_SLOT					g_pstrCommonMessage[806]
#define	dMSG_CAN_NOT_REMOVE_ITEM_BY_VALID_SLOT							g_pstrCommonMessage[807]
#define	dMSG_NOT_OPENED_PITCHMAN_SHOP									g_pstrCommonMessage[808]
#define	dMSG_FAILED_BUY_ITEM_BY_CHANGE_SHOP_INFO						g_pstrCommonMessage[809]
#define	dMSG_INSUFFICIENT_GOLD_TO_BY_PITCHMAN_SHOP_ITEM					g_pstrCommonMessage[810]

#define	dMSG_READY_TO_OPEN_PITCHMAN_SHOP								g_pstrCommonMessage[811]
#define	dMSG_SHOP_NAME													g_pstrCommonMessage[812]
#define	dMSG_SIGN_BOARD_SHAPE											g_pstrCommonMessage[813]
#define	dMSG_BOLD_TEXT													g_pstrCommonMessage[814]
#define	dMSG_NORMAL_TEXT												g_pstrCommonMessage[815]

#define	dMSG_APPLY														g_pstrCommonMessage[816]
#define	dMSG_ITS_NOT_INFINITY_BULLET									g_pstrCommonMessage[817]
#define	dMSG_BROKEN_ITEM_COMMENT										g_pstrCommonMessage[818]
#define	dMSG_CAN_NOT_IMPROVE_ITEM_COMMENT								g_pstrCommonMessage[819]
#define	dMSG_CURSED_ITEM_COMMENT										g_pstrCommonMessage[820]

#define	dMSG_PITCHMAN_SHOP_NAME_FORM									g_pstrCommonMessage[821]
#define	dMSG_A_UNIT_PRICE												g_pstrCommonMessage[822]
#define	dMSG_SELL_ITEM_COUNT											g_pstrCommonMessage[823]
#define	dMSG_ITEM_COUNT_FOR_SELL										g_pstrCommonMessage[824]
#define	dMSG_CLOSE_PITCHMAN_SHOP_BY_SHOP_KEEPER							g_pstrCommonMessage[825]

#define	dMSG_SELLING_ITEM_COUNT_ZERO									g_pstrCommonMessage[826]
#define	dMSG_PITCHMAN_SHOP_NAME_RULE									g_pstrCommonMessage[827]
#define	dMSG_PLEASE_STOP_SHOP_FOR_ADD_SELLING_ITEM						g_pstrCommonMessage[828]
#define	dMSG_PITCHMAN_SHOP_BUTTON_COMMENT								g_pstrCommonMessage[829]
#define	dMSG_FAILED_BUY_ITEM_BY_FULL_INVENTORY							g_pstrCommonMessage[830]

#define	dMSG_TOO_MANY_GOLD												g_pstrCommonMessage[831]
#define	dMSG_PITCHMAN_SHOP_ITEM_SOLD_FORM								g_pstrCommonMessage[832]
#define	dMSG_PITCHMAN_SHOP_ITEM_BUY_FORM								g_pstrCommonMessage[833]
#define	dMSG_REMAIN_TIME_TO_BEGIN_GUILD_BATTLE_FORM						g_pstrCommonMessage[834]
#define	dMSG_REMAIN_TIME_TO_END_GUILD_BATTLE_FORM						g_pstrCommonMessage[835]

#define	dMSG_BEGUN_GUILD_BATTLE											g_pstrCommonMessage[836]
#define	dMSG_CANCEL_GUILD_BATTLE										g_pstrCommonMessage[837]
#define	dMSG_CANCEL_GUILD_BATTLE_FORM									g_pstrCommonMessage[838]
#define	dMSG_BOOKED_GUILD_BATTLE_COMMENT_FORM							g_pstrCommonMessage[839]
#define	dMSG_ENGAGED_GUILD_BATTLE_COMMENT_FORM							g_pstrCommonMessage[840]

#define	dMSG_REQUIRE_PREMIUM_ITEM_TO_USE_THIS_MENU						g_pstrCommonMessage[841]
#define	dMSG_CHANGE_PITCHMAN_SHOP_SIGNBOARD_SHAPE_COMMENT				g_pstrCommonMessage[842]
#define	dMSG_CHANGE_PITCHMAN_SHOP_TEXT_COLOR_COMMENT					g_pstrCommonMessage[843]
#define	dMSG_CHANGE_PITCHMAN_SHOP_TEXT_BOLD_COMMENT						g_pstrCommonMessage[844]
#define	dMSG_CHANGE_PITCHMAN_SHOP_EFFECT_COMMENT						g_pstrCommonMessage[845]

#define	dMSG_CHANGE_PITCHMAN_SHOP_DOUMI_COMMENT							g_pstrCommonMessage[846]
#define	dMSG_DO_YOU_OPEN_PITCHMAN_SHOP									g_pstrCommonMessage[847]
#define	dMSG_DO_YOU_STOP_PITCHMAN_SHOP									g_pstrCommonMessage[848]
#define	dMSG_PITCHMAN_SHOP_TEXT_SHADOW									g_pstrCommonMessage[849]
#define	dMSG_CALL_MAGIC_CARPET_BUTTON_COMMENT							g_pstrCommonMessage[850]

#define	dMSG_MILITARY_RECORD											g_pstrCommonMessage[851]
#define	dMSG_MILITARY_RECORD_FORM										g_pstrCommonMessage[852]
#define	dMSG_CONTINUE_WIN_FORM											g_pstrCommonMessage[853]
#define	dMSG_DEATH_PENELTY_FORM											g_pstrCommonMessage[854]
#define	dMSG_ARE_YOU_REALLY_BUY_SELECT_ITEM_FORM						g_pstrCommonMessage[855]

#define	dMSG_CLOSE_PITCHMAN_SHOP_BY_SOLD_ALL_ITEM						g_pstrCommonMessage[856]
#define	dMSG_PLEASE_INPUT_TARGET_PLAYER_NAME							g_pstrCommonMessage[857]
#define	dMSG_PLEASE_INPUT_LEVEL											g_pstrCommonMessage[858]
#define	dMSG_PLEASE_INPUT_GOLD											g_pstrCommonMessage[859]
#define	dMSG_PLEASE_INPUT_MOVE_LOCATE									g_pstrCommonMessage[860]

#define	dMSG_PLEASE_SELECT_MOVE_FIELD									g_pstrCommonMessage[861]
#define	dMSG_OPERATOR_MENU_COMMENT										g_pstrCommonMessage[862]
#define	dMSG_DISSOLUTION_GUILD_MENU										g_pstrCommonMessage[863]
#define	dMSG_DO_YOU_WANT_DISSOLUTION_GUILD								g_pstrCommonMessage[864]
#define	dMSG_ONLY_GUILD_MASTER_DISSOLUTION_GUILD						g_pstrCommonMessage[865]

#define	dMSG_DISSOLUTION_GUILD_CONDITION_COMMENT						g_pstrCommonMessage[866]
#define	dMSG_SKILL_LEVEL												g_pstrCommonMessage[867]
#define	dMSG_ITEM_USE_TO_ITEM_COMMENT									g_pstrCommonMessage[868]
#define	dMSG_ADMINISTRATOR												g_pstrCommonMessage[869]
#define	dMSG_OPERATOR													g_pstrCommonMessage[870]

#define	dMSG_CHANGE_QUEST_INFO_NOTICE_FORM								g_pstrCommonMessage[871]
#define	dMSG_DURABILITY													g_pstrCommonMessage[872]
#define	dMSG_REPAIR														g_pstrCommonMessage[873]
#define	dMSG_REPAIR_PRICE												g_pstrCommonMessage[874]
#define	dMSG_REPAIR_ITEM_QUESTION_COMMENT_FORM							g_pstrCommonMessage[875]

#define	dMSG_REPAIR_ITEM_COMMENT										g_pstrCommonMessage[876]
#define	dMSG_BROKEN_ITEM_REQUIRE_REAPIR									g_pstrCommonMessage[877]
#define	dMSG_BREAKDOWN_ITEM_FORM										g_pstrCommonMessage[878]
#define	dMSG_REPAIR_BREAKDOWN_ITEM_FORM									g_pstrCommonMessage[879]
#define	dMSG_NOT_EXIST_BREAKDOWN_ITEM_COMMENT							g_pstrCommonMessage[880]

#define	dMSG_MANY_GOLD													g_pstrCommonMessage[881]
#define	dMSG_CAN_NOT_MEMORY_FIELD										g_pstrCommonMessage[882]
#define	dMSG_REQUIRE_SKILL_MASTERY_TITLE								g_pstrCommonMessage[883]
#define	dMSG_GWM_LEAVE_GUILD_LIMIT_BY_JOIN_DATE							g_pstrCommonMessage[884]
#define	dMSG_ADD_NEW_GUILD_MARK_PACK									g_pstrCommonMessage[885]

#define	dMSG_RECOVER_ALL_DETH_PENELTY									g_pstrCommonMessage[886]
#define	dMSG_RECOVER_SOME_DETH_PENELTY									g_pstrCommonMessage[887]
#define	dMSG_QUESTION_FOR_RECOVER_DEATH_PENELTY_FORM					g_pstrCommonMessage[888]
#define	dMSG_YOU_ARE_NOT_EXIST_DEATH_PENELTY							g_pstrCommonMessage[889]
#define	dMSG_LACK_GOLD_FOR_RECOVER_DEATH_PENELTY_FORM					g_pstrCommonMessage[890]

#define	dMSG_POISON_DAMAGE_SKILL_COMMENT_FORM							g_pstrCommonMessage[891]
#define	dMSG_HIT_CHANCE													g_pstrCommonMessage[892]
#define	dMSG_MOVE_TOWN_PORTAL_QUESTION_FORM								g_pstrCommonMessage[893]
#define	dMSG_REALLY_RESET_STATE_POINT									g_pstrCommonMessage[894]
#define	dMSG_REALLY_RESET_SKILL_POINT									g_pstrCommonMessage[895]

#define	dMSG_UNDEAD_TYPE												g_pstrCommonMessage[896]
#define	dMSG_HUMAN_TYPE													g_pstrCommonMessage[897]
#define	dMSG_DEVIL_TYPE													g_pstrCommonMessage[898]
#define	dMSG_ANIMAL_TYPE												g_pstrCommonMessage[899]
#define	dMSG_HOLY_BEAST_TYPE											g_pstrCommonMessage[900]

#define	dMSG_BEGINNER_ITEM_COMMENT										g_pstrCommonMessage[901]
#define	dMSG_GWM_JOIN_GUILD_LIMIT_BY_JOIN_DATE							g_pstrCommonMessage[902]
#define	dMSG_GWM_CHANGE_GUILD_RANK_FORM									g_pstrCommonMessage[903]
#define	dMSG_ALLPY_LIMIT_TARGET											g_pstrCommonMessage[904]
#define	dMSG_APPLY_HP_LIMIT_TOOL_TIP_FORM								g_pstrCommonMessage[905]

#define	dMSG_APPLY_LEVEL_LIMIT_TOOL_TIP_FORM							g_pstrCommonMessage[906]
#define	dMSG_APPLY_SPECIFIC_MONSTER_TYPE_LEVEL_LIMIT_TOOL_TIP_FORM		g_pstrCommonMessage[907]
#define	dMSG_SUCCESS_TO_TAME_MONSTER									g_pstrCommonMessage[908]
#define	dMSG_CAN_NOT_USE_SKILL_REQUIRE_PET								g_pstrCommonMessage[909]
#define	dMSG_MAKE_BREEDING_RECORD_BOOK									g_pstrCommonMessage[910]

#define	dMSG_SUMMON_BEAST												g_pstrCommonMessage[911]
#define	dMSG_PET														g_pstrCommonMessage[912]
#define	dMSG_LOYALTY													g_pstrCommonMessage[913]
#define	dMSG_FEEDING_RED_PEPPER_COUNT_FORM								g_pstrCommonMessage[914]
#define	dMSG_BREEDING_TIME												g_pstrCommonMessage[915]

#define	dMSG_BREEDING_TIME_OVER_ONE_MONTH								g_pstrCommonMessage[916]
#define	dMSG_BREEDING_TIME_FORM											g_pstrCommonMessage[917]
#define	dMSG_PLEASE_INPUT_WANT_PET_NAME									g_pstrCommonMessage[918]
#define	dMSG_PET_NAME_RULE												g_pstrCommonMessage[919]
#define	dMSG_CHANGE_PET_NAME_KEYWORD									g_pstrCommonMessage[920]

#define	dCHANGE_NAME													g_pstrCommonMessage[921]
#define	dMSG_CAN_NOT_TAME_MONSTER_BY_LOW_LEVEL							g_pstrCommonMessage[922]
#define	dMSG_PARTING_WITH_PET											g_pstrCommonMessage[923]
#define	dMSG_DO_YOU_REALLY_PARTING_WITH_THAT_PET_FORM					g_pstrCommonMessage[924]
#define	dMSG_PARTING_WITH_PET_FORM										g_pstrCommonMessage[925]

#define	dMSG_MAX_TAME_MONSTER_COUNT										g_pstrCommonMessage[926]
#define	dMSG_PET_NUMBERS												g_pstrCommonMessage[927]
#define	dMSG_HEAL_POINT													g_pstrCommonMessage[928]
#define	dMSG_HEAL_PERCENTAGE_BASED_REMAIN_HP_FORM						g_pstrCommonMessage[929]
#define	dMSG_FIRST_AID													g_pstrCommonMessage[930]

#define	dMSG_FIRST_AID_RULE												g_pstrCommonMessage[931]
#define	dMSG_RESTORE_PET_EXP_PERCENTAGE_FORM							g_pstrCommonMessage[932]
#define	dMSG_DECREASE_DEATH_PENELTY_TIME_FORM							g_pstrCommonMessage[933]
#define	dMSG_CAN_NOT_USE_SKILL_REQUIRE_SUMMON_BEAST						g_pstrCommonMessage[934]
#define	dMSG_RELEASE_SUMMONED_BEAST										g_pstrCommonMessage[935]

#define	dMSG_DO_YOU_REALLY_RELEASE_THAT_SUMMON_BEAST_FORM				g_pstrCommonMessage[936]
#define	dMSG_SEE_INCLINE_ATTACK_POWER_POWER_PERCENTAGE_FOR_PET_COMMAND_FORM	g_pstrCommonMessage[937]
#define	dMSG_SEE_INCLINE_ATTACK_POWER_POWER_PERCENTAGE_FOR_SB_COMMAND_FORM	g_pstrCommonMessage[938]
#define	dMSG_SEE_INCLINE_ATTACK_POWER_POWER_PERCENTAGE_FOR_PET_AND_SB_COMMAND_FORM	g_pstrCommonMessage[939]
#define	dMSG_SEE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_COMMAND_FORM	g_pstrCommonMessage[940]

#define	dMSG_SEE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_SB_COMMAND_FORM	g_pstrCommonMessage[941]
#define	dMSG_SEE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_AND_SB_COMMAND_FORM	g_pstrCommonMessage[942]
#define	dMSG_SEE_INCLINE_WEAPON_ATTACK_POWER_FORM						g_pstrCommonMessage[943]
#define	dMSG_SEE_INCLINE_ARMOR_DEFENSE_POWER_FORM						g_pstrCommonMessage[944]
#define	dMSG_SEE_INCREASE_LEVEL_FORM									g_pstrCommonMessage[945]

#define	dMSG_SEE_BONUS_EXP_FORM											g_pstrCommonMessage[946]
#define	dMSG_SEE_PET_BOOST_FORM											g_pstrCommonMessage[947]
#define	dMSG_SEE_PHYSICAL_DAMAGE_BASED_REMAIN_HP_FORM					g_pstrCommonMessage[948]
#define	dMSG_SEE_SET_TRAP_FORM											g_pstrCommonMessage[949]
#define	dMSG_SUMMON_BEAST_NAME_FORM										g_pstrCommonMessage[950]

#define	dMSG_HIDE_PICHMAN_SHOP											g_pstrCommonMessage[951]
#define	dMSG_CAN_NOT_OPEN_PITCHMAN_SHOP_WHEN_HIDE_PICHMAN_SHOP_STATUS	g_pstrCommonMessage[952]
#define	dMSG_REMEMBER_ID												g_pstrCommonMessage[953]
#define	dMSG_OLD_STYLE_SHOT_KEY											g_pstrCommonMessage[954]
#define	dMSG_NO_MANER_POINT												g_pstrCommonMessage[955]

#define	dMSG_PET_CLASS													g_pstrCommonMessage[956]
#define	dMSG_GRADE														g_pstrCommonMessage[957]
#define	dMSG_INSTANCE_HEAL												g_pstrCommonMessage[958]
#define	dMSG_GRADE_DAMAGE_FORM											g_pstrCommonMessage[959]
#define	dMSG_WAD_CASTER													g_pstrCommonMessage[960]

#define	dMSG_WAD_SHOOT_RANGE											g_pstrCommonMessage[961]
#define	dMSG_WAD_MAX_EXPLOSION_RANGE									g_pstrCommonMessage[962]
#define	dMSG_WAD_EXPLOSION_RANGE										g_pstrCommonMessage[963]
#define	dMSG_ATTACK_COUNT												g_pstrCommonMessage[964]
#define	dMSG_CHARGE_MANA_POWER											g_pstrCommonMessage[965]

#define	dMSG_PERCENTAGE													g_pstrCommonMessage[966]
#define	dMSG_EXTRA_ITEM_EFFICIENCY										g_pstrCommonMessage[967]
#define	dMSG_IS_NOT_EXIST_SKILL_SUMMONED_BEAST							g_pstrCommonMessage[968]
#define	dMSG_OPERATE_SUMMONED_BEAST										g_pstrCommonMessage[969]
#define	dMSG_HUNDREAD_MILLION											g_pstrCommonMessage[970]

#define	dMSG_TEN_THOUND													g_pstrCommonMessage[971]
#define	dMSG_EXTRA_ITEM													g_pstrCommonMessage[972]
#define	dMSG_EXTRA_ITEM_CHARGE_OK										g_pstrCommonMessage[973]
#define	dMSG_UCIR_DEST_ITEM_NOT_EXTRA_ITEM								g_pstrCommonMessage[974]
#define	dMSG_UCIR_DEST_ITEM_CAN_NOT_CHARGEABLE							g_pstrCommonMessage[975]

#define	dMSG_UCIR_MISMATCH_EX_GRADE										g_pstrCommonMessage[976]
#define	dMSG_UMCIR_ALREADY_EXIST_SHAPE									g_pstrCommonMessage[977]
#define	dMSG_MC_RELEASE_TO_SUMMON_SCROLL								g_pstrCommonMessage[978]
#define	dMSG_MC_SUMMON_WILD_CAT											g_pstrCommonMessage[979]
#define	dMSG_MC_SUMMON_TURTLE											g_pstrCommonMessage[980]

#define	dMSG_MC_SUMMON_SQUIRREL											g_pstrCommonMessage[981]
#define	dMSG_MC_SUMMON_DRAKE											g_pstrCommonMessage[982]
#define	dMSG_MC_SUMMON_GEAR												g_pstrCommonMessage[983]
#define	dMSG_MC_SUMMON_WILD_CAT_COMMENT									g_pstrCommonMessage[984]
#define	dMSG_MC_SUMMON_TURTLE_COMMENT									g_pstrCommonMessage[985]

#define	dMSG_MC_SUMMON_SQUIRREL_COMMENT									g_pstrCommonMessage[986]
#define	dMSG_MC_SUMMON_DRAKE_COMMENT									g_pstrCommonMessage[987]
#define	dMSG_MC_SUMMON_GEAR_COMMENT										g_pstrCommonMessage[988]
#define	dMSG_MC_NON_ACTIVE_EMBLEM										g_pstrCommonMessage[989]
#define	dMSG_MC_WILD_CAT_ACTIVATED										g_pstrCommonMessage[990]

//	991~995
#define	dMSG_MC_TURTLE_ACTIVATED										g_pstrCommonMessage[991]
#define	dMSG_MC_SQUIRREL_ACTIVATED										g_pstrCommonMessage[992]
#define	dMSG_MC_GEAR_ACTIVATED											g_pstrCommonMessage[993]
#define	dMSG_MC_DRAKE_ACTIVATED											g_pstrCommonMessage[994]
#define	dMSG_MC_DISABLE_EMBLEM											g_pstrCommonMessage[995]

#define	dMSG_CONNECT_FAILED												g_pstrCommonMessage[996]
#define	dMSG_PLEASE_SELECT_SERVER										g_pstrCommonMessage[997]
#define	dMSG_VIEW_TIP													g_pstrCommonMessage[998]
#define	dMSG_OWN_GOLD_LIMIT												g_pstrCommonMessage[999]
#define	dMSG_OWN_BANK_GOLD_LIMIT										g_pstrCommonMessage[1000]

#define	dMSG_OPEN_SKILL_TREE											g_pstrCommonMessage[1001]
#define	dMSG_OPEN_SKILL_TREE_HELP										g_pstrCommonMessage[1002]
#define	dMSG_PLUS_SKILL_TIP_ABOUT_SHIFT									g_pstrCommonMessage[1003]
#define	dMSG_PLEASE_SELECT_CORPSE_PLAYER_FOR_WANT_REVIVE				g_pstrCommonMessage[1004]
#define	dWM_MENU_MESSAGE_BEGIN											1005
#define	dMSG_WM_FIELD													g_pstrCommonMessage[1005]

#define	dMSG_WM_PARTY													g_pstrCommonMessage[1006]
#define	dMSG_WM_QUEST													g_pstrCommonMessage[1007]
#define	dMSG_WM_GUILD													g_pstrCommonMessage[1008]
#define	dMSG_WM_FRIEND													g_pstrCommonMessage[1009]
#define	dMSG_WM_CUCKOO													g_pstrCommonMessage[1010]

#define	dMSG_WM_RETURN_TO_GAME											g_pstrCommonMessage[1011]
#define	dMSG_WM_FIELD_INFO												g_pstrCommonMessage[1012]
#define	dMSG_WM_VALID_LINKED_DUNGEON									g_pstrCommonMessage[1013]
#define	dMSG_WM_PARTY_MEMBER_LIST										g_pstrCommonMessage[1014]
#define	dMSG_WM_VALID_PARTY												g_pstrCommonMessage[1015]

#define	dMSG_WM_PARTY_INFO												g_pstrCommonMessage[1016]
#define	dMSG_WM_PROCESS_QUEST											g_pstrCommonMessage[1017]
#define	dMSG_WM_QUEST_INFO												g_pstrCommonMessage[1018]
#define	dMSG_WM_NOT_EXIST_PROCESS_QUEST									g_pstrCommonMessage[1019]
#define	dMSG_CAN_NOT_WITHDRAW_ITEM_BY_EXTRA_BAG_SIZE_FULL				g_pstrCommonMessage[1020]

#define	dMSG_PICK_PARTY_ITEM_FORM										g_pstrCommonMessage[1021]
#define	dMSG_RDIR_SUCCESS												g_pstrCommonMessage[1022]
#define	dMSG_RDIR_FAILED												g_pstrCommonMessage[1023]
#define	dMSG_RDIR_CAN_NOT_REPAIR_DEST_ITEM_BY_HIGH_DURABILITY			g_pstrCommonMessage[1024]
#define	dMSG_RDIR_DEST_ITEM_NOT_DX_ITEM									g_pstrCommonMessage[1025]

#define	dMSG_CAN_NOT_STACK_ON_EXPAND_INVETORY_ITEM						g_pstrCommonMessage[1026]
#define	dMSG_EXAPND_INVENTORY											g_pstrCommonMessage[1027]
#define	dMSG_WARNING_WITHDRAW_CARROT									g_pstrCommonMessage[1028]
#define	dMSG_WARNING_WITHDRAW_MAGIC_BAG									g_pstrCommonMessage[1029]
#define	dMSG_FAILED_TO_BUY_CARROT_SHOP_ITEM								g_pstrCommonMessage[1030]

#define	dMSG_LOW_CARROT_TO_BUY_CARROT_SHOP_ITEM							g_pstrCommonMessage[1031]
#define	dMSG_BUY_CARROT_SHOP_ITEM_FORM									g_pstrCommonMessage[1032]
#define	dMSG_DO_YOU_WANT_BUY_FOLLOW_CARROT_SHOP_ITEM_FORM				g_pstrCommonMessage[1033]
#define	dMSG_WAIT_FOR_RESULT_OF_BUY_ITEM								g_pstrCommonMessage[1034]
#define	dMSG_INCREASE_DODGE_CHANCE										g_pstrCommonMessage[1035]

#define	dMSG_INCREASE_BLOCKING_CHANCE									g_pstrCommonMessage[1036]
#define	dMSG_GET_PARTY_GOLD_FORM										g_pstrCommonMessage[1037]
#define	dMSG_INCREASE_DEFENSIVE_POWER									g_pstrCommonMessage[1038]
#define	dMSG_CAN_NOT_DROP_ITEM											g_pstrCommonMessage[1039]
#define	dMSG_CAN_NOT_TRADE_ITEM											g_pstrCommonMessage[1040]

#define	dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST_COMMENT				g_pstrCommonMessage[1041]
#define	dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST						g_pstrCommonMessage[1042]
#define	dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST_RULE_FORM				g_pstrCommonMessage[1043]
#define	dMSG_NOT_EXIST_MY_NAME_IN_TARGET_FRIEND_LIST_FORM				g_pstrCommonMessage[1044]
#define	dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST_SUCCESS_FORM			g_pstrCommonMessage[1045]

#define	dMSG_REMOVE_FRIEND_BY_ASK_FORM									g_pstrCommonMessage[1046]
#define	dMSG_OUTPUT_PARTY_ITEM_MESSAGE									g_pstrCommonMessage[1047]
#define	dMSG_JUMP_ABLE_VILLAGE_COUNT_FROM								g_pstrCommonMessage[1048]
#define	dMSG_JUMP_ABLE_DISTANCE_FROM									g_pstrCommonMessage[1049]
#define	dMSG_DIFF_PLUS_FORM												g_pstrCommonMessage[1050]

#define	dMSG_DIFF_MINUS_FORM											g_pstrCommonMessage[1051]
#define	dWM_DIFF_MESSAGE_BEGIN											1052
#define	dMSG_DIFF_HP													g_pstrCommonMessage[1052]
#define	dMSG_DIFF_CP													g_pstrCommonMessage[1053]
#define	dMSG_DIFF_AP1													g_pstrCommonMessage[1054]
#define	dMSG_DIFF_AP2													g_pstrCommonMessage[1055]

#define	dMSG_DIFF_DP													g_pstrCommonMessage[1056]
#define	dMSG_DIFF_ST													g_pstrCommonMessage[1057]
#define	dMSG_DIFF_DX													g_pstrCommonMessage[1058]
#define	dMSG_DIFF_CT													g_pstrCommonMessage[1059]
#define	dMSG_DIFF_CH													g_pstrCommonMessage[1060]

#define	dMSG_DIFF_IT													g_pstrCommonMessage[1061]
#define	dMSG_DIFF_WS													g_pstrCommonMessage[1062]
#define	dMSG_DIFF_LK													g_pstrCommonMessage[1063]
//#define	dMSG_DIFF_LK													g_pstrCommonMessage[1063]
//#define	dMSG_DIFF_LK													g_pstrCommonMessage[1064]

//#define	dMSG_DIFF_WS													g_pstrCommonMessage[1066]
//#define	dMSG_DIFF_LK													g_pstrCommonMessage[1067]
//#define	dMSG_DIFF_LK													g_pstrCommonMessage[1068]
//#define	dMSG_DIFF_LK													g_pstrCommonMessage[1069]
//#define	dMSG_DIFF_LK													g_pstrCommonMessage[1070]

#define	dMSG_AFTER_EQUIP_INFO											g_pstrCommonMessage[1071]
#define	dMSG_UNLOCK_DOOR_BY_SKILL_FORM									g_pstrCommonMessage[1072]
#define	dMSG_UNLOCK_DOOR_BY_KEY_FORM									g_pstrCommonMessage[1073]
#define	dMSG_DISARM_TRAP_BY_SKILL_FORM									g_pstrCommonMessage[1074]
#define	dMSG_CAN_NOT_UNLOCK_DOOR_BY_NOT_EXIST_AVAIL_KEY					g_pstrCommonMessage[1075]

#define	dMSG_CAN_NOT_UNLOCK_DOOR_BY_NOT_EXIST_AVAIL_SKILL				g_pstrCommonMessage[1076]
#define	dMSG_OPEN_DOOR													g_pstrCommonMessage[1077]
#define	dMSG_DESTROY_DOOR_LOCK											g_pstrCommonMessage[1078]
#define	dMSG_UNLOCK_DOOR_BY_SKILL										g_pstrCommonMessage[1079]
#define	dMSG_UNLOCK_DOOR_BY_KEY											g_pstrCommonMessage[1080]

#define	dMSG_DISARM_DOOR_TRAP											g_pstrCommonMessage[1081]
#define	dMSG_NOT_EXIST_SKILL_OR_KEY_TO_UNLOCK_TARGET_DOOR				g_pstrCommonMessage[1082]
#define	dMSG_PENELTY_TIME_FORM											g_pstrCommonMessage[1083]
#define	dMSG_GGM_ALREADY_ACTIVE_GG_PLEASE_RESTART_GAME					g_pstrCommonMessage[1084]
#define	dMSG_GGM_DOUBLE_START_GAME										g_pstrCommonMessage[1085]

#define	dMSG_GGM_GG_INIT_ERROR											g_pstrCommonMessage[1086]
#define	dMSG_GGM_INCORRECT_GG_FILE										g_pstrCommonMessage[1087]
#define	dMSG_GGM_INCORRECT_WINDOW_SYSTEM_FILE							g_pstrCommonMessage[1088]
#define	dMSG_GGM_FAILED_TO_BEGIN_GG										g_pstrCommonMessage[1089]
#define	dMSG_GGM_FIND_HACK												g_pstrCommonMessage[1090]

#define	dMSG_GGM_CANCEL_GG_UPDATE										g_pstrCommonMessage[1091]
#define	dMSG_GGM_FAILED_TO_CONNECT_GG_UPDATE_SERVER						g_pstrCommonMessage[1092]
#define	dMSG_GGM_CAN_NOT_COMPLETE_TO_GG_UPDATE							g_pstrCommonMessage[1093]
#define	dMSG_GGM_FAILED_TO_LOADING_MODULE_FOR_DETECT_HACK				g_pstrCommonMessage[1094]
#define	dMSG_GGM_FAILED_TO_BEGIN_GG2									g_pstrCommonMessage[1095]

//	1096~1100
#define	dMSG_GGM_GG_ERROR_FORM											g_pstrCommonMessage[1096]
#define	dMSG_GGM_FAILED_TO_INIT_GG_FORM									g_pstrCommonMessage[1097]
#define	dMSG_GGM_FIND_SPEED_HACK										g_pstrCommonMessage[1098]
#define	dMSG_GGM_FIND_GAME_HACK											g_pstrCommonMessage[1099]
#define	dMSG_GGM_CHANGE_GAME_OR_GG										g_pstrCommonMessage[1100]

#define	dMSG_CMD_ASK_JOIN_PARTY											g_pstrCommonMessage[1101]
#define	dMSG_CMD_ASK_ADD_FRIEND											g_pstrCommonMessage[1102]
#define	dMSG_LINK_QUEST_MONSTER											g_pstrCommonMessage[1103]
#define	dMSG_LINK_QUEST_ITEM											g_pstrCommonMessage[1104]
#define	dMSG_LINK_QUEST_NPC												g_pstrCommonMessage[1105]

#define	dMSG_WINNING_PRIZE_LOTTO_FORM									g_pstrCommonMessage[1106]
#define	dMSG_IS_TRAP_ARCA_DO_YOU_WANT_DISARM_THIS_ARCA					g_pstrCommonMessage[1107]
#define	dMSG_IS_TRAP_ARCA_DO_YOU_WANT_UNLOCK_THIS_ARCA					g_pstrCommonMessage[1108]
#define	dMSG_CAN_NOT_UNLOCK_ARCA										g_pstrCommonMessage[1109]
#define	dMSG_ATTACK														g_pstrCommonMessage[1110]

#define	dMSG_BROKEN_ARCA_ITEM_BY_ATTACK_FORM							g_pstrCommonMessage[1111]
#define	dMSG_ARCA_ATTACK_FAILED_BY_ALREADY_UNLOCKED						g_pstrCommonMessage[1112]
#define	dMSG_CAN_NOT_DESTROY_LOCK_BY_LOW_LEVEL_FORM						g_pstrCommonMessage[1113]
#define	dMSG_CAN_NOT_DESTROY_LOCK										g_pstrCommonMessage[1114]
#define	dMSG_UNLOCK_BY_DESTROY_LOCK										g_pstrCommonMessage[1115]

#define	dMSG_GET_ARCA_ITEM_FAILED_BY_INVENTORY_FULL						g_pstrCommonMessage[1116]
#define	dMSG_GET_ARCA_GOLD_FAILED_BY_GOLD_FULL							g_pstrCommonMessage[1117]
#define	dMSG_CAN_NOT_ACTION_BY_BAD_STATUS								g_pstrCommonMessage[1118]
#define	dMSG_CAN_NOT_MOVE_BY_BAD_STATUS									g_pstrCommonMessage[1119]
#define	dMSG_CAN_NOT_ACTION_BY_CONFUSE									g_pstrCommonMessage[1120]

#define	dMSG_UNLOCK_BY_KEY_FORM											g_pstrCommonMessage[1121]
#define	dMSG_UNLOCK_BY_SKILL_FORM										g_pstrCommonMessage[1122]
#define	dMSG_EXPLOSION_ARCA_TRAP										g_pstrCommonMessage[1123]
#define	dMSG_CAN_NOT_ACTION_BY_BERSERK									g_pstrCommonMessage[1124]
#define	dMSG_WARNING_CAUGHT_BY_TRAP										g_pstrCommonMessage[1125]

#define	dMSG_DETECT_TRAP_DO_YOU_WANT_TRY_DISARM_THIS_TRAP				g_pstrCommonMessage[1126]
#define	dMSG_CAN_NOT_DISARM_BY_LOW_CP									g_pstrCommonMessage[1127]
#define	dMSG_CAN_NOT_DISARM_TRAP										g_pstrCommonMessage[1128]
#define	dMSG_LEFT_PUNCH_SKILL											g_pstrCommonMessage[1129]
#define	dMSG_RIGHT_PUNCH_SKILL											g_pstrCommonMessage[1130]

#define	dMSG_LEFT_KICK_SKILL											g_pstrCommonMessage[1131]
#define	dMSG_RIGHT_KICK_SKILL											g_pstrCommonMessage[1132]
#define	dMSG_DOWN_KICK_SKILL											g_pstrCommonMessage[1133]
#define	dMSG_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE						g_pstrCommonMessage[1134]
#define	dMSG_FTW_FAILED_DISARM_BY_RANGE									g_pstrCommonMessage[1135]

#define	dMSG_FAILED_DISARM_BY_LOW_DISARM_LEVEL							g_pstrCommonMessage[1136]
#define	dMSG_NOT_EXIST_DISARM_SKILL										g_pstrCommonMessage[1137]
#define	dMSG_DISARM_FAILED_AND_EXPLOSION_TRAP							g_pstrCommonMessage[1138]
#define	dMSG_DISARM_TRAP_FAILED											g_pstrCommonMessage[1139]
#define	dMSG_TRY_DISARM_TRAP											g_pstrCommonMessage[1140]

#define	dMSG_TRY_DISARM_TRAP_FAILED_BY_LOW_CP							g_pstrCommonMessage[1141]
#define	dMSG_UNLOCK_ARCA_BY_SKILL_FORM									g_pstrCommonMessage[1142]
#define	dMSG_TRY_UNLOCK_BY_SKILL										g_pstrCommonMessage[1143]
#define	dMSG_UNLOCK_FAILED_BY_LOW_UNLOCK_LEVEL							g_pstrCommonMessage[1144]
#define	dMSG_UNLOCK_FAILED_BY_LOW_CP									g_pstrCommonMessage[1145]

#define	dMSG_DISARM_FAILED_BY_ALREADY_DISARMED							g_pstrCommonMessage[1146]
#define	dMSG_UNLOCK_FAILED												g_pstrCommonMessage[1147]
#define	dMSG_IS_ARMED_DO_YOU_WANT_DISARM_THIS_TRAP						g_pstrCommonMessage[1148]
#define	dMSG_EXPLOSION_TRAP												g_pstrCommonMessage[1149]
#define	dMSG_NOT_EXIST_AVAIL_SKILL_OR_KEY_TO_UNLOCK						g_pstrCommonMessage[1150]

#define	dMSG_DO_YOU_WANT_UNLOCK_USE_KEY_FORM							g_pstrCommonMessage[1151]
#define	dMSG_ALREADY_STEAL_GOLD_MONSTER									g_pstrCommonMessage[1152]
#define	dMSG_ALREADY_STEAL_ITEM_MONSTER									g_pstrCommonMessage[1153]
#define	dMSG_SET_TRAP_SPEED_FORM										g_pstrCommonMessage[1154]
#define	dMSG_USE_SKILL_TO_TARGET_FAIELD_BY_LOW_LEVEL					g_pstrCommonMessage[1155]

#define	dMSG_SET														g_pstrCommonMessage[1156]
#define	dMSG_ITEM_PIECE_FORM											g_pstrCommonMessage[1157]
#define	dMSG_UNKNOWN_ITEM												g_pstrCommonMessage[1158]
#define	dMSG_DO_YOU_WANT_USE_NPGL										g_pstrCommonMessage[1159]
#define	dMSG_ITEM_PIECE													g_pstrCommonMessage[1160]

#define	dMSG_UNKNOWN_ITEM_COMMENT										g_pstrCommonMessage[1161]
#define	dMSG_IS_OWNED_OTHER_ARCA										g_pstrCommonMessage[1162]
#define	dMSG_ENTER_TO_SECRET_DUNGEON_FAILED_BY_NOT_PARTY				g_pstrCommonMessage[1163]
#define	dMSG_FULL_SECRET_DUNGEON										g_pstrCommonMessage[1164]
#define	dMSG_SECRET_DUNGEON												g_pstrCommonMessage[1165]

#define	dMSG_FAILED_TO_JOIN_GAME_BY_NOT_SECRET_DUNGEON_MEMBER			g_pstrCommonMessage[1166]
#define	dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_CONNECT					g_pstrCommonMessage[1167]
#define	dMSG_FAILED_TO_JOIN_GAME_BY_TOO_MANY_CONNECTOR					g_pstrCommonMessage[1168]
#define	dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_FIELD						g_pstrCommonMessage[1169]
#define	dMSG_WARNING_DISAPEAR_DUNGEON_FORM								g_pstrCommonMessage[1170]

#define	dMSG_FAILED_QUEST_FORM											g_pstrCommonMessage[1171]
#define	dMSG_FAILED_QUEST												g_pstrCommonMessage[1172]
#define	dMSG_ALREADY_USE_SECRET_DUNGEON_PARTY							g_pstrCommonMessage[1173]
#define	dMSG_BASIC_ATTACK_SPEED											g_pstrCommonMessage[1174]
#define	dMSG_CAN_NOT_BANISH_CONNECT_MEMBER_IN_SECRET_DUNGEON			g_pstrCommonMessage[1175]

#define	dMSG_HOUR_MINUTE_SECOND_FORM									g_pstrCommonMessage[1176]
#define	dMSG_MINUTE_SECOND_FORM											g_pstrCommonMessage[1177]
#define	dMSG_SECOND_FORM												g_pstrCommonMessage[1178]
#define	dMSG_SDM_DISSOLUTION_PARTY										g_pstrCommonMessage[1179]
#define	dMSG_SDM_COMPLETE_FORM											g_pstrCommonMessage[1180]

#define	dMSG_FAILED_TO_CREATE_SECRET_DUNGEON_BY_LIMIT_LEVEL_FORM		g_pstrCommonMessage[1181]
#define	dMSG_PM_ASK_JOIN_FAILED_BY_TARGET_LEVEL_OVER_THE_SECRET_DUNGEON_LIMIT_LEVEL	g_pstrCommonMessage[1182]
#define	dMSG_ARE_YOU_WANT_FORCED_DISCONNECT_GAME						g_pstrCommonMessage[1183]
#define	dMSG_SECRET_LOGOUT_FORM											g_pstrCommonMessage[1184]
#define	dMSG_CANCEL_LOGOUT_TIMER_BY_ACTION								g_pstrCommonMessage[1185]

#define	dMSG_HUNTING													g_pstrCommonMessage[1186]
#define	dMSG_EXPLORATION_SECRET_DUNGEON									g_pstrCommonMessage[1187]
#define	dMSG_OPERATE_QUEST												g_pstrCommonMessage[1188]
#define	dMSG_FREE_ACQUISITION											g_pstrCommonMessage[1189]
#define	dMSG_EQUALITY_SHARING											g_pstrCommonMessage[1190]

#define	dMSG_ORDER_ACQUISITION											g_pstrCommonMessage[1191]
#define	dMSG_RANDOM_ACQUISITION											g_pstrCommonMessage[1192]
#define	dMSG_FIRST_JOB_RANDOM_ACQUISITION								g_pstrCommonMessage[1193]
#define	dMSG_FIRST_JOB_ORDER_ACQUISITION								g_pstrCommonMessage[1194]
#define	dMSG_RAISE_PARTY_MEMBER											g_pstrCommonMessage[1195]

#define	dMSG_SHARING_GOLD_METHOD										g_pstrCommonMessage[1196]
#define	dMSG_SHARING_ITEM_METHOD										g_pstrCommonMessage[1197]
#define	dMSG_PARTY_OBJECT												g_pstrCommonMessage[1198]
#define	dMSG_CHANGE_ITEM_SHARING_METHOD_FORM							g_pstrCommonMessage[1199]
#define	dMSG_CHANGE_GOLD_SHARING_METHOD_FORM							g_pstrCommonMessage[1200]

#define	dMSG_CHANGE_PARTY_OBJECT_OBJECT_FORM							g_pstrCommonMessage[1201]
#define	dMSG_PARTY_STATUS												g_pstrCommonMessage[1202]
#define	dMSG_DO_NOT_RAISE_PARTY_MEMBER									g_pstrCommonMessage[1203]
#define	dMSG_CHANGE_PARTY_STATUS_FORM									g_pstrCommonMessage[1204]
#define	dMSG_THAT_IS_CLOSE_SECRET_DUNGEON								g_pstrCommonMessage[1205]

#define	dMSG_TOO_MANY_PLAYER_ENTER_SECRET_DUNGEON						g_pstrCommonMessage[1206]
#define	dMSG_OVER_THE_SECRET_DUNGEON_PLAY_COUNT_PER_DAY					g_pstrCommonMessage[1207]
#define	dMSG_EXPLORATION_COUNT_FORM										g_pstrCommonMessage[1208]
#define	dMSG_CHANGE_PARTY_LEADER										g_pstrCommonMessage[1209]
#define	dMSG_QUESTION_CHANGE_PARTY_LEADER_FORM							g_pstrCommonMessage[1210]

#define	dMSG_CAN_NOT_DESTROY_DOOR										g_pstrCommonMessage[1211]
#define	dMSG_STEAL_GOLD_FORM											g_pstrCommonMessage[1212]
#define	dMSG_CAN_NOT_APT_SKILL_TO_THIS_STATUS							g_pstrCommonMessage[1213]
#define	dMSG_TM_TARGET_GOLD_FULL										g_pstrCommonMessage[1214]
#define	dMSG_CAN_NOT_STRIP_SHIELD_STATUS								g_pstrCommonMessage[1215]

#define	dMSG_CAN_NOT_STRIP_WEAPON_STATUS								g_pstrCommonMessage[1216]
#define	dMSG_CAN_NOT_CHANGE_SHIELD_STATUS								g_pstrCommonMessage[1217]
#define	dMSG_CAN_NOT_CHANGE_WEAPON_STATUS								g_pstrCommonMessage[1218]
#define	dMSG_QUEST_COMPLETE_FORM										g_pstrCommonMessage[1219]
#define	dMSG_RIPR_NOT_EXIST_PREFIX_ITEM									g_pstrCommonMessage[1220]

#define	dMSG_RIPR_IS_EXCLUSIVE_NORTMAL_ITEM								g_pstrCommonMessage[1221]
#define	dMSG_RIPR_IS_EXCLUSIVE_UNIQUE_ITEM								g_pstrCommonMessage[1222]
#define	dMSG_REMOVE_PREFIX												g_pstrCommonMessage[1223]
#define	dMSG_REMOVE_PREFIX_LIST_FORM									g_pstrCommonMessage[1224]
#define	dMSG_CONFIRM_REMOVE_PREFIX_FORM									g_pstrCommonMessage[1225]

#define	dMSG_REMOVE_PREFIX_RESULT										g_pstrCommonMessage[1226]
#define	dMSG_REMOVE_PREFIX_RESULT_FORM									g_pstrCommonMessage[1227]
#define	dMSG_CAN_NOT_FIND_PIECE_ITEM_IN_INVENTORY						g_pstrCommonMessage[1228]
#define	dMSG_MAKE														g_pstrCommonMessage[1229]
#define	dMSG_MERGE_ITEM_FORM											g_pstrCommonMessage[1230]

#define	dMSG_CAN_NOT_FIND_UNKNOWN_ITEM_IN_INVENTORY						g_pstrCommonMessage[1231]
#define	dMSG_JUDGE_TO_ITEM_FORM											g_pstrCommonMessage[1232]
#define	dMSG_LACK_MONEY_FOR_JUDGE_ITEM									g_pstrCommonMessage[1233]
#define	dMSG_LACK_MONEY_FOR_MERGE_ITEM									g_pstrCommonMessage[1234]
#define	dMSG_RESULT_OF_MERGE_ITEM										g_pstrCommonMessage[1235]

#define	dMSG_RESULT_OF_JUDGE_ITEM										g_pstrCommonMessage[1236]
#define	dMSG_RESULT_OF_MERGE_ITEM_FORM									g_pstrCommonMessage[1237]
#define	dMSG_RESULT_OF_JUDGE_ITEM_PIECE_FORM							g_pstrCommonMessage[1238]
#define	dMSG_RESULT_OF_JUDGE_ITEM_FORM									g_pstrCommonMessage[1239]
#define	dMSG_JUDGE_ITEM													g_pstrCommonMessage[1240]

#define	dMSG_IS_CAN_NOT_TRADE_ITEM										g_pstrCommonMessage[1241]
#define	dMSG_IS_REVERSION_WHEN_EQUIP									g_pstrCommonMessage[1242]
#define	dMSG_PIECE_ITEM_MERGE_CONDITION_FORM							g_pstrCommonMessage[1243]
#define	dMSG_CAN_NOT_ATTACK_TARGET_BY_IS_UNDER_TARGET_ULTIMATE_BARRIER	g_pstrCommonMessage[1244]
#define	dMSG_TM_LOW_EMPTY_INVENTORY_SLOT_COUNT							g_pstrCommonMessage[1245]

#define	dMSG_CAN_NOT_PICK_ITEM_UNDER_DEATH_STATUS						g_pstrCommonMessage[1246]
#define	dMSG_CARROT_SHOP_COMMENT										g_pstrCommonMessage[1247]
#define	dMSG_CAN_NOT_DEPOSIT_CARROT_CHECKED_BY_LIMIT_OWN_COUNT			g_pstrCommonMessage[1248]
#define	dMSG_USE_GATE_GLOVE_FAILED_BY_REMAIN_SECOND_FORM				g_pstrCommonMessage[1249]
#define	dMSG_REMAIN_TIME_FOR_USE_GATE_GLOVE_FORM						g_pstrCommonMessage[1250]

#define	dMSG_PLEASE_SELECT_TARGET_FIELD_FOR_GATE_GLOVE					g_pstrCommonMessage[1251]
#define	dMSG_USE_GATE_GLOVE_CONFIRM_FORM								g_pstrCommonMessage[1252]
#define	dMSG_USE_GATE													g_pstrCommonMessage[1253]
#define	dMSG_GAME_CARROT_AGREEMENT_FAILED								g_pstrCommonMessage[1254]
#define	dMSG_BANNER_BASIC_TYPE											g_pstrCommonMessage[1255]

#define	dMSG_HORIZON_BANNER_FORM										g_pstrCommonMessage[1256]
#define	dMSG_VERTICAL_BANNER_FORM										g_pstrCommonMessage[1257]
#define	dMSG_PUT_BANNER													g_pstrCommonMessage[1258]
#define	dMSG_BANNER_TITLE												g_pstrCommonMessage[1259]
#define	dMSG_BANNER_COMMNET												g_pstrCommonMessage[1260]

#define	dMSG_PLEASE_INPUT_BANNER_TITLE									g_pstrCommonMessage[1261]
#define	dMSG_PLEASE_INPUT_BANNER_COMMENT								g_pstrCommonMessage[1262]
#define	dMSG_EDIT														g_pstrCommonMessage[1263]
#define	dMSG_PLEASE_INPUT_BANNER_TEXT									g_pstrCommonMessage[1264]
#define	dMSG_BANNER_TEXT_MIN_LENGTH										g_pstrCommonMessage[1265]

#define	dMSG_FAILED_TO_PUT_BANNER										g_pstrCommonMessage[1266]
#define	dMSG_FAILED_TO_BANNER_FULL										g_pstrCommonMessage[1267]
#define	dMSG_SUCCESS_TO_PUT_BANNER										g_pstrCommonMessage[1268]
#define	dMSG_FAILED_TO_PUT_BANNER_BY_TO_CLOSE							g_pstrCommonMessage[1269]
#define	dMSG_FAILED_TO_PUT_BANNER_BY_INCORRECT_FIELD					g_pstrCommonMessage[1270]

#define	dMSG_CAN_NOT_BANISH_MEMBER_UNDER_DROP_PARTY_ITEM_TIME			g_pstrCommonMessage[1271]
#define	dMSG_TREASURE_MAP_DESTROYED_WHEN_USE							g_pstrCommonMessage[1272]
#define	dMSG_FOUND_TREASURE_BOX											g_pstrCommonMessage[1273]
#define	dMSG_CAN_NOT_FIND_RED_STONE_BUT_FEEL_RED_STONES_AIR				g_pstrCommonMessage[1274]
#define	dMSG_UNSEAL_RED_STONE_AIR_BY_SPECIFIC_PLAYER_FORM				g_pstrCommonMessage[1275]

#define	dMSG_UNSEAL_RED_STONE_AIR										g_pstrCommonMessage[1276]
#define	dMSG_TREASURE_MAP_DESTROYED										g_pstrCommonMessage[1277]
#define	dMSG_DESTROY_ITEM_BY_MOVE_FIELD									g_pstrCommonMessage[1278]
#define	dMSG_SUCCESS_TO_USE_TREASURE_MAP								g_pstrCommonMessage[1279]
#define	dMSG_VALID_TREASURE_BOX											g_pstrCommonMessage[1280]

#define	dMSG_FOUND_GOLD_IN_TREASURE_BOX									g_pstrCommonMessage[1281]
#define	dMSG_BERSERK_COUNT_OVER											g_pstrCommonMessage[1282]
#define	dMSG_ALREADY_BERSERK_STATUS_FORM								g_pstrCommonMessage[1283]
#define	dMSG_CAN_NOT_BANISH_GUILD_MASTER								g_pstrCommonMessage[1284]
#define	dMSG_CAN_NOT_BANISH_GUILD_ELDER									g_pstrCommonMessage[1285]

#define	dMSG_CAN_NOT_BANISH_GUILD_MEMBER_BY_SUB_MASTER					g_pstrCommonMessage[1286]
#define	dMSG_LEAVE_GUILD_MEMBER_FORM									g_pstrCommonMessage[1287]
#define	dMSG_UIR_FAILED_BY_VALID_INVENTORY_SLOT							g_pstrCommonMessage[1288]
#define	dMSG_ASK_FRIEND_FORM											g_pstrCommonMessage[1289]
#define	dMSG_SEND_WHISPER_FORM											g_pstrCommonMessage[1290]

#define	dMSG_ASK_PARTY_FORM												g_pstrCommonMessage[1291]
#define	dMSG_INCORRECT_AVATAR_DATA_PLEASE_CONTACT_TO_US					g_pstrCommonMessage[1292]
#define	dMSG_GUILD_KILL_NOTICE_FORM										g_pstrCommonMessage[1293]
#define	dMSG_WINNING_POINT												g_pstrCommonMessage[1294]
#define	dMSG_CAN_NOT_SELL_ITEM_SHOP										g_pstrCommonMessage[1295]

#define	dMSG_CURRENT_SET_EFFECT											g_pstrCommonMessage[1296]
#define	dMSG_AFTER_EQUIP_SET_EFFECT										g_pstrCommonMessage[1297]
#define	dMSG_SET_ITEM_INFO												g_pstrCommonMessage[1298]
#define	dMSG_CAN_NOT_ENTER_TO_PORTAL_BY_IS_OWNED_DIFFERENT_PARTY		g_pstrCommonMessage[1299]
#define	dMSG_MOVE_PORTAL_QUESTION_FORM									g_pstrCommonMessage[1300]

#define	dMSG_TO_USE_COUPLERING_SET_REQUIRE_TWO_VALID_INVENTORY_SLOT		g_pstrCommonMessage[1301]
#define	dMSG_TARGET_EXIST_IN_PREMIUM_ZONE								g_pstrCommonMessage[1302]
#define	dMSG_TARGET_IS_NOT_CONNECTED_GAME								g_pstrCommonMessage[1303]
#define	dMSG_TARGET_IS_PLAY_GUILD_BATTLE								g_pstrCommonMessage[1304]
#define	dMSG_ARE_YOU_REALLY_USE_COUPLE_RING_FROM						g_pstrCommonMessage[1305]

#define	dMSG_ISPI_STRENGTH												g_pstrCommonMessage[1306]
#define	dMSG_ISPI_AGILITY												g_pstrCommonMessage[1307]
#define	dMSG_ISPI_CONSTITUTION											g_pstrCommonMessage[1308]
#define	dMSG_ISPI_CHARISMA												g_pstrCommonMessage[1309]
#define	dMSG_ISPI_INTELLIGENCE											g_pstrCommonMessage[1310]

#define	dMSG_ISPI_WISDOM												g_pstrCommonMessage[1311]
#define	dMSG_ISPI_LUCK													g_pstrCommonMessage[1312]
#define	dMSG_ISPI_COMMENT_FORM											g_pstrCommonMessage[1313]
#define	dMSG_COUPLE_RING_COMMENT_FORM									g_pstrCommonMessage[1314]
#define	dMSG_NONAME_COUPLE_RING											g_pstrCommonMessage[1315]

#define	dMSG_WAIT_FOR_FIRST_FIREWORK									g_pstrCommonMessage[1316]
#define	dMSG_FIREWORK_REMAIN_TIME_FORM									g_pstrCommonMessage[1317]
#define	dMSG_FAILED_TO_USE_COUPLE_RING_BY_TARGET_IS_NOT_OWNED_COUPLE_RING_FROM	g_pstrCommonMessage[1318]
#define	dMSG_USE_COUPLE_RING											g_pstrCommonMessage[1319]
#define	dMSG_MIX_TWO_ITEM_BY_BOTTOMLESS_BOX								g_pstrCommonMessage[1320]

#define	dMSG_CNUB_BY_VALID_SLOT											g_pstrCommonMessage[1321]
#define	dMSG_CNUB_BY_MISMATCH_ITEM_TYPE									g_pstrCommonMessage[1322]
#define	dMSG_CNUB_BY_IS_ABLE_ONLY_EQUIPPMENT							g_pstrCommonMessage[1323]
#define	dMSG_CNUB_BY_PREFIX_ZERO										g_pstrCommonMessage[1324]
#define	dMSG_CNUB_BY_CAN_NOT_MIX_ITEM_COMMENT							g_pstrCommonMessage[1325]

#define	dMSG_DO_YOU_REALLY_MIX_TWO_ITEM_FORM							g_pstrCommonMessage[1326]
#define	dMSG_CAN_NOT_RETURN_TO_VILLAGE_DURING_BATTLE_FORM				g_pstrCommonMessage[1327]
#define	dMSG_CAN_NOT_TELEPORT_DURING_BATTLE_FORM						g_pstrCommonMessage[1328]
#define	dMSG_CAN_NOT_USE_GATE_GLOVE_DURING_BATTLE_FORM					g_pstrCommonMessage[1329]
#define	dMSG_CONTINUOUS_SKILL_ATTACK_SPEED_FORM							g_pstrCommonMessage[1330]

#define	dMSG_PSM_FAILED_BY_INCORRECT_NAME								g_pstrCommonMessage[1331]
#define	dMSG_PROCESS_MAIN_QUEST											g_pstrCommonMessage[1332]
#define	dMSG_CHAPTER													g_pstrCommonMessage[1333]
#define	dMSG_PART														g_pstrCommonMessage[1334]
#define	dMSG_NORMAL_QUEST												g_pstrCommonMessage[1335]

#define	dMSG_MAIN_QUEST													g_pstrCommonMessage[1336]
#define	dMSG_PAGE														g_pstrCommonMessage[1337]
#define	dMSG_COMPLETE_PERCENTAGE_FORM									g_pstrCommonMessage[1338]
#define	dMSG_BWR_DEST_ITEM_IS_NOT_BOOK									g_pstrCommonMessage[1339]
#define	dMSG_BWR_IS_NOT_SAME_BOOK										g_pstrCommonMessage[1340]

#define	dMSG_BWR_SOURCE_ITEM_IS_EXIST_BOOK_PAGE							g_pstrCommonMessage[1341]
#define	dMSG_READ_BOOK													g_pstrCommonMessage[1342]
#define	dMSG_FAILED_TO_ENTER_THE_SECRET_DUNGEON_BY_LIMIT_LEVEL_FORM		g_pstrCommonMessage[1343]
#define	dMSG_ARE_YOU_REALLY_USE_TITLE_FORM								g_pstrCommonMessage[1344]
#define	dMSG_GAME_SERVER_NOT_OPEND										g_pstrCommonMessage[1345]

#define	dMSG_USR_REQUIRE_POTION											g_pstrCommonMessage[1346]
#define	dMSG_USR_REQUIRE_DURG											g_pstrCommonMessage[1347]
#define	dMSG_USR_REQUIRE_FLOWER											g_pstrCommonMessage[1348]
#define	dMSG_USR_REQUIRE_CANDY											g_pstrCommonMessage[1349]
#define	dMSG_PRINCESS_TRANS_WEAPON										g_pstrCommonMessage[1350]

#define	dMSG_PTW_FIRE_DAMAGE_FORM										g_pstrCommonMessage[1351]
#define	dMSG_PTW_WATER_DAMAGE_FORM										g_pstrCommonMessage[1352]
#define	dMSG_PTW_WIND_DAMAGE_FORM										g_pstrCommonMessage[1353]
#define	dMSG_PTW_EARTH_DAMAGE_FORM										g_pstrCommonMessage[1354]
#define	dMSG_PTW_LIGHT_DAMAGE_FORM										g_pstrCommonMessage[1355]

#define	dMSG_PTW_DARK_DAMAGE_FORM										g_pstrCommonMessage[1356]
#define	dMSG_PTW_INCRASE_DAMAGE_PERCENTAGE_FORM							g_pstrCommonMessage[1357]
#define	dMSG_BUY_PIGEON_SHOP_ITEM_QUESTION_FORM							g_pstrCommonMessage[1358]
#define	dMSG_AR_IS_COOL_TIME											g_pstrCommonMessage[1359]
#define	dMSG_LIMIT_ACTIVE_COUNT_SKILL_FORM								g_pstrCommonMessage[1360]

#define	dMSG_CAN_NOT_COPY_MORE_ACTOR									g_pstrCommonMessage[1361]
#define	dMSG_REMAIN_COOL_TIME_FOR_USE_SKILL_FORM						g_pstrCommonMessage[1362]
#define	dMSG_CONVERT_ANIMATION_DATA										g_pstrCommonMessage[1363]
#define	dMSG_TREASURE_ARCA												g_pstrCommonMessage[1364]
#define	dMSG_EW_CAN_NOT_TRANS_TARGET_TO_FROG_BY_BOSS					g_pstrCommonMessage[1365]

#define	dMSG_EW_CAN_NOT_TRANS_TARGET_TO_FROG_BY_HP						g_pstrCommonMessage[1366]
#define	dMSG_EW_CAN_NOT_TRANS_TARGET_TO_FROG_FORM						g_pstrCommonMessage[1367]
#define	dMSG_EW_LIMIT_SHUT_IN_MAGIC_BOX_ACTOR_COUNT						g_pstrCommonMessage[1368]
#define	dMSG_USE_ABLE_POTION_FORM										g_pstrCommonMessage[1369]
#define	dMSG_OX_QUIZ													g_pstrCommonMessage[1370]

#define	dMSG_WORD_QUIZ													g_pstrCommonMessage[1371]
#define	dMSG_BEGIN_WORD_QUIZ											g_pstrCommonMessage[1372]
#define	dMSG_IS_INCORRECT_SOLUTION										g_pstrCommonMessage[1373]
#define	dMSG_IS_CORRECT_SOLUTION_AND_GET_SCORE_FORM						g_pstrCommonMessage[1374]
#define	dMSG_WQ_CHANGE_ROUND_FORM										g_pstrCommonMessage[1375]

#define	dMSG_CORRECT_SOLUTION_FORM										g_pstrCommonMessage[1376]
#define	dMSG_RANK_FORM													g_pstrCommonMessage[1377]
#define	dMSG_SCORE_FORM													g_pstrCommonMessage[1378]
#define	dMSG_MY_SCORE_FORM												g_pstrCommonMessage[1379]
#define	dMSG_WORD_QUIZ_SOLUTION_LENGTH									g_pstrCommonMessage[1380]

#define	dMSG_END_WORD_QUIZ_FORM											g_pstrCommonMessage[1381]
#define	dMSG_FINAL_RANK													g_pstrCommonMessage[1382]
#define	dMSG_SEND_USER_DATA_FORM										g_pstrCommonMessage[1383]
#define	dMSG_SAVE_USER_DATA												g_pstrCommonMessage[1384]
#define	dMSG_ADD_YOU_BY_FRIEND_FROM										g_pstrCommonMessage[1385]

#define	dMSG_CHANGE_FRIEND_GROUP_FORM									g_pstrCommonMessage[1386]
#define	dMSG_CHANGE_FRIEND_GROUP_NAME									g_pstrCommonMessage[1387]
#define	dMSG_CHANGE_FRIEND_GROUP_NAME_COMMAND							g_pstrCommonMessage[1388]
#define	dMSG_CHANGE_FRIEND_GROUP_NAME_COMMAND_FORM						g_pstrCommonMessage[1389]
#define	dMSG_DISCONNECTED_TO_BROAD_CAST_SERVER							g_pstrCommonMessage[1390]

#define	dMSG_NOT_SAVE_THIS_SETTING_DATA									g_pstrCommonMessage[1391]
#define	dMSG_PITCHMAN_SHOP												g_pstrCommonMessage[1392]
#define	dMSG_FOLDER_AND_FILE_FORM										g_pstrCommonMessage[1393]
#define	dMSG_TRANS_TO_WEAPON_UPKEEP_TIME_FORM							g_pstrCommonMessage[1394]
#define	dMSG_LIMIT_LEVEL												g_pstrCommonMessage[1395]

#define	dMSG_REMOVE_TITLE_FORM											g_pstrCommonMessage[1396]
#define	dMSG_ARE_YOU_REALLY_REMOVE_TITLE_FORM							g_pstrCommonMessage[1397]
#define	dMSG_CONNECT_TO_BC_SERVER										g_pstrCommonMessage[1398]
#define	dMSG_DISCONNECT_TO_BC_SERVER									g_pstrCommonMessage[1399]
#define	dMSG_REQUEST_TIME_IS_MAINTENANCE_TIME							g_pstrCommonMessage[1400]

#define	dMSG_ARE_YOU_RETRY_SELECT_GUILD_MARK							g_pstrCommonMessage[1401]
#define	dMSG_RECOVER_LIST												g_pstrCommonMessage[1402]
#define	dMSG_RECOVER_FRIEND_LIST_FORM									g_pstrCommonMessage[1403]
#define	dMSG_NOT_EXIST_RECOVER_FRIEND_LIST								g_pstrCommonMessage[1404]
#define	dMSG_RECOVER_FRIEND_LIST										g_pstrCommonMessage[1405]

#define	dMSG_ADD_FRIEND_FORM											g_pstrCommonMessage[1406]
#define	dMSG_FAILED_TO_CONNECT_BC_RETRY_CONNECT							g_pstrCommonMessage[1407]
#define	dMSG_CAN_NOT_CREATE_MORE_ILLUSION								g_pstrCommonMessage[1408]
#define	dMSG_FAILED_TO_USE_SKILL_BY_LIMIT_SAME_TIME_CASTING_TARGET_FORM	g_pstrCommonMessage[1409]
#define	dMSG_EXIST_TRAFFIC_OBJECT_TO_OPEN_PORTAL						g_pstrCommonMessage[1410]

#define	dMSG_FAILED_TO_CREATE_CHARACTER_BY_INCLUDE_INCORRECT_CHAR		g_pstrCommonMessage[1411]
#define	dMSG_TARGET_IN_JAIL												g_pstrCommonMessage[1412]
#define	dMSG_TARGET_ALREADY_OWNED_PRINCSS_WEAPON						g_pstrCommonMessage[1413]
#define	dMSG_TARGET_IS_AT_BLOCKED_FIELD_TO_USE_THAT_ITEM				g_pstrCommonMessage[1414]
#define	dMSG_CAN_NOT_USE_THAT_ITEM_IN_THIS_FIELD						g_pstrCommonMessage[1415]

#define	dMSG_IS_ONLY_ACTIVE_UNDER_MINUS_CP								g_pstrCommonMessage[1416]
#define	dMSG_STT_HIT_DARK_DAMAGE_TO_TARGET_FORM							g_pstrCommonMessage[1417]
#define	dMSG_STT_SOD_REDUCE_RESISTANSE_FORM								g_pstrCommonMessage[1418]
#define	dMSG_STT_SOD_ADD_DARK_DAMAGE_FORM								g_pstrCommonMessage[1419]
#define	dMSG_STT_TERRIBLE_DREAMS_FROM									g_pstrCommonMessage[1420]

#define	dMSG_STT_HWABYUNG_FROM											g_pstrCommonMessage[1421]
#define	dMSG_STT_PLOT_OF_SHADOW_TARGET_DAMAGE_FORM						g_pstrCommonMessage[1422]
#define	dMSG_STT_PLOT_OF_SHADOW_CASTER_DAMAGE_FORM						g_pstrCommonMessage[1423]
#define	dMSG_STT_IMPULSE_DAMAGE_FORM									g_pstrCommonMessage[1424]
#define	dMSG_STT_SELF_REPROOF_DAMAGE_FORM								g_pstrCommonMessage[1425]

#define	dMSG_STT_TORTURE_DAMAGE_FORM									g_pstrCommonMessage[1426]
#define	dMSG_TIME														g_pstrCommonMessage[1427]
#define	dMSG_AM_ICON													g_pstrCommonMessage[1428]
#define	dMSG_PM_ICON													g_pstrCommonMessage[1429]
#define	dMSG_STT_DAMAGED_FIRST_AID_FORM									g_pstrCommonMessage[1430]

#define	dMSG_STT_DAMAGE_POOL_FORM										g_pstrCommonMessage[1431]
#define	dMSG_SKILL_LIMIT_LEVEL_FROM										g_pstrCommonMessage[1432]
#define	dMSG_STT_MARIONETTE_FORM										g_pstrCommonMessage[1433]
#define	dMSG_STT_DEGENERATE_FORM										g_pstrCommonMessage[1434]
#define	dMSG_STT_ANIMATE_UNDEAD_FORM									g_pstrCommonMessage[1435]

#define	dMSG_STT_OVERLAPPING_INCLINE_POWER_FORM							g_pstrCommonMessage[1436]
#define	dMSG_STT_INCLINE_POWER_FORM										g_pstrCommonMessage[1437]
#define	dMSG_STT_CURE_POISON_FORM										g_pstrCommonMessage[1438]
#define	dMSG_STT_SHAKLE_FORM											g_pstrCommonMessage[1439]
#define	dMSG_STT_NOTIFICATION_OF_DEATH_FORM								g_pstrCommonMessage[1440]

#define	dMSG_STT_ATTR_DAMAGE_WITH_REDUCE_RESISTENCE_FORM				g_pstrCommonMessage[1441]
#define	dMSG_STT_ATTR_DAMAGE_FORM										g_pstrCommonMessage[1442]
#define	dWM_ATTR_MESSAGE_BEGIN											1443
#define	dWM_DAMAGE_ATTR_MESSAGE_BEGIN									1449
#define	dMSG_STT_PERIODICAL_DARK_DAMAGE_BY_SLEEP_FROM					g_pstrCommonMessage[1455]

#define	dMSG_STT_DECLINE_RECOVER_HP_EFFICIENT_FORM						g_pstrCommonMessage[1456]
#define	dMSG_STT_INTERRUPTING_REVIVE_FROM								g_pstrCommonMessage[1457]
#define	dMSG_STT_CONTRACT_FORM											g_pstrCommonMessage[1458]
#define	dMSG_STT_SOUL_OATH_FORM											g_pstrCommonMessage[1459]
#define	dMSG_STT_BLOOD_COMPACT											g_pstrCommonMessage[1460]

#define	dMSG_HS_END_CLIENT_BY_FOUND_HACKING_TOOL_FORM					g_pstrCommonMessage[1461]
#define	dMSG_HS_FOUND_TRY_DEBUGGING_FORM								g_pstrCommonMessage[1462]
#define	dMSG_HS_OCCUR_PROBLEM_IN_DEFEND_HACKING_FORM					g_pstrCommonMessage[1463]
#define	dMSG_HS_INCORRECT_DATA_PLEASE_REINSTALL							g_pstrCommonMessage[1464]
#define	dMSG_HS_FOUND_DEBUGGER_STOP_DEBUGER_AND_RESTART					g_pstrCommonMessage[1465]

//	1466~1470
#define	dMSG_HS_ERR_NEED_ADMIN_RIGHTS									g_pstrCommonMessage[1466]
#define	dMSG_HS_ERR_COMPATIBILITY_MODE_RUNNING							g_pstrCommonMessage[1467]
#define	dMSG_ARE_YOU_RECEIVE_JOIN_PARTY_ASK_FORM						g_pstrCommonMessage[1468]
#define	dMSG_RECEIVE_PARTY_JOIN_ASK										g_pstrCommonMessage[1469]
#define	dMSG_PREFERENCE_JOB												g_pstrCommonMessage[1470]

#define	dMSG_PARTY_PROSPECTUS_FORM										g_pstrCommonMessage[1471]
#define	dMSG_PARTY_MEMBER_INFO_SUMMARY_FORM								g_pstrCommonMessage[1472]
#define	dMSG_RECRUIT_PARTY_MEMBER_LIMIT_LEVEL							g_pstrCommonMessage[1473]
#define	dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE					g_pstrCommonMessage[1474]
#define	dMSG_PARTY_EXPLAIN												g_pstrCommonMessage[1475]

#define	dMSG_RECRUIT_PARTY_MEMBER_LEVEL_RANGE_FORM						g_pstrCommonMessage[1476]
#define	dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE_KEYWORD			g_pstrCommonMessage[1477]
#define	dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE_COMMENT			g_pstrCommonMessage[1478]
#define	dMSG_CHANGE_PARTY_PROSPECTUS_TEXT_COMMENT						g_pstrCommonMessage[1479]
#define	dMSG_CHANGE														g_pstrCommonMessage[1480]

#define	dMSG_NOT_EXIST_A_JOIN_ABLE_PARTY								g_pstrCommonMessage[1481]
#define	dMSG_PL_PARTY_MEMBER_INFO										g_pstrCommonMessage[1482]
#define	dMSG_PL_JOIN_PARTY_REQUEST_FORM									g_pstrCommonMessage[1483]
#define	dMSG_JPRUL_ACCEPT_JOIN_PARTY_REQUEST_FORM						g_pstrCommonMessage[1484]
#define	dMSG_JPRUL_REMOVE_USER_FROM_JOIN_PARTY_USER_LIST_FORM			g_pstrCommonMessage[1485]

#define	dMSG_JPRUL_RESET_JOIN_PARTY_USER_LIST							g_pstrCommonMessage[1486]
#define	dMSG_REQUIRE_SETTING_PARTY_EXPLAIN_BEFOR_OPEN_PARTY				g_pstrCommonMessage[1487]
#define	dMSG_REQUEST_JOIN_PARTY_USER_LIST								g_pstrCommonMessage[1488]
#define	dMSG_UER_WRONG_PLACE_ITEM										g_pstrCommonMessage[1489]
#define	dMSG_RSM_CAN_NOT_TELEPORT_FIELD									g_pstrCommonMessage[1490]

#define	dMSG_STT_CHANIN_MAGIC_DAMAGE_FORM								g_pstrCommonMessage[1491]
#define	dMSG_CASTING_SPEED												g_pstrCommonMessage[1492]
#define	dMSG_FOUND_TRY_HACKING											g_pstrCommonMessage[1493]
#define	dMSG_MAKE_GUILD_MARK											g_pstrCommonMessage[1494]
#define	dMSG_MAKE_GUILD_BY_SELECT_CONTENTS_CONTINUE						g_pstrCommonMessage[1495]


#define	dMSG_ABLE_TO_TAME_MONSTER_GRADE_FORM							g_pstrCommonMessage[1496]
#define	dMSG_REQUEST_GUILD_COUNT										g_pstrCommonMessage[1497]
#define	dMSG_REQUEST_GUILD_SCRAMBLE_FORM								g_pstrCommonMessage[1498]
#define	dMSG_REQUEST_GUILD_SCRAMBLE_CONFIRM_FORM						g_pstrCommonMessage[1499]
#define	dMSG_RSM_ALREADY_REQUEST_GUILD_SCRAMBLE							g_pstrCommonMessage[1500]


#define	dMSG_ARE_YOU_CANCEL_REQUEST_GUILD_SCRAMBLE						g_pstrCommonMessage[1501]
#define	dMSG_CAN_NOT_CANCEL_REQUEST_GUILD_SCRAMBLE						g_pstrCommonMessage[1502]
#define	dMSG_ALREADY_REQEUST_ANOTHER_TIME_GUILD_SCRAMBLE				g_pstrCommonMessage[1503]
#define	dMSG_ENABLE_TO_CANCEL											g_pstrCommonMessage[1504]
#define	dMSG_REQUESTED_GUILD_SCRAMBLE									g_pstrCommonMessage[1505]

#define	dMSG_BLOCK_TO_REQUEST_GUILD_SCRAMBLE							g_pstrCommonMessage[1506]
#define	dMSG_ENABLE_TO_REQUEST_GUILD_SCRAMBLE							g_pstrCommonMessage[1507]
#define	dMSG_EXIST_SIEGE_WARFARE_FOR_CAPTURE_GUILD_HALL_FORM			g_pstrCommonMessage[1508]
#define	dMSG_EXIST_SIEGE_WARFARE_FOR_DEFENSE_GUILD_HALL_FORM			g_pstrCommonMessage[1509]
#define	dWM_ABSORB_MAGIC_DAMAGE_MESSAGE_BEGIN							1510
#define	dMSG_ABSORB_FIRE_DAMAGE											g_pstrCommonMessage[1510]

#define	dMSG_ABSORB_WATER_DAMAGE										g_pstrCommonMessage[1511]
#define	dMSG_ABSORB_WIND_DAMAGE											g_pstrCommonMessage[1512]
#define	dMSG_ABSORB_EARTH_DAMAGE										g_pstrCommonMessage[1513]
#define	dMSG_ABSORB_LIGHT_DAMAGE										g_pstrCommonMessage[1514]
#define	dMSG_ABSORB_DARK_DAMAGE											g_pstrCommonMessage[1515]

#define	dWM_INCREASE_MAGIC_RESISTANCE_MESSAGE_BEGIN						1516

#define	dMSG_INCREASE_FIRE_RESISTANCE									g_pstrCommonMessage[1516]
#define	dMSG_INCREASE_WATER_RESISTANCE									g_pstrCommonMessage[1517]
#define	dMSG_INCREASE_WIND_RESISTANCE									g_pstrCommonMessage[1518]
#define	dMSG_INCREASE_EARTH_RESISTANCE									g_pstrCommonMessage[1519]
#define	dMSG_INCREASE_LIGHT_RESISTANCE									g_pstrCommonMessage[1520]

#define	dMSG_INCREASE_DARK_RESISTANCE									g_pstrCommonMessage[1521]
#define	dMSG_CRUSH_RESISTANCE											g_pstrCommonMessage[1522]
#define	dMSG_CURSE_RESISTANCE											g_pstrCommonMessage[1523]
#define	dMSG_STRANGE_STATUS_RESISTANCE									g_pstrCommonMessage[1524]
#define	dMSG_DECLINE_POWER_RESISTANCE									g_pstrCommonMessage[1525]

#define	dMSG_PHYSICAL_MAGIC_DAMAGE_RESISTANCE							g_pstrCommonMessage[1526]
#define	dMSG_DEFENSIVE_POWER											g_pstrCommonMessage[1527]
#define	dMSG_MOVE_SPEED													g_pstrCommonMessage[1528]
#define	dMSG_GUILD_PET_OBITIAN_COUNT									g_pstrCommonMessage[1529]
#define	dMSG_GUILD_PET_CRISTAL_WATER_COUNT								g_pstrCommonMessage[1530]

#define	dMSG_GUILD_PET_ASTRAL_BOW_COUNT									g_pstrCommonMessage[1531]
#define	dMSG_GUILD_PET_DASH_BLADE_COUNT									g_pstrCommonMessage[1532]
#define	dMSG_GUILD_GUARDIAN_COUNT										g_pstrCommonMessage[1533]
#define	dMSG_GUILD_GUARDIAN_SIGHT										g_pstrCommonMessage[1534]
#define	dMSG_BUILD_ABLE_GUILD_STRUCTURE_COUNT							g_pstrCommonMessage[1535]

#define	dMSG_INCREASE_GUILD_STRUCTURE_HP								g_pstrCommonMessage[1536]
#define	dMSG_INCREASE_EFFCIENT_FALG_OF_STRATEGY							g_pstrCommonMessage[1537]
#define	dMSG_INCREASE_EFFCIENT_FALG_OF_CHEER							g_pstrCommonMessage[1538]
#define	dMSG_INCREASE_EFFCIENT_FALG_OF_QUICKNESS						g_pstrCommonMessage[1539]
#define	dMSG_INCREASE_EFFCIENT_FALG_OF_ASSUALT							g_pstrCommonMessage[1540]

#define	dMSG_INCREASE_EFFCIENT_FALG_OF_VICTORY							g_pstrCommonMessage[1541]
#define	dMSG_INCREASE_EFFCIENT_FALG_OF_APPEAR							g_pstrCommonMessage[1542]
#define	dMSG_DECREASE_FIELD_FIRE_RESISTANCE_PENELTY						g_pstrCommonMessage[1543]
#define	dMSG_DECREASE_FIELD_WATER_RESISTANCE_PENELTY					g_pstrCommonMessage[1544]
#define	dMSG_DECREASE_FIELD_WIND_RESISTANCE_PENELTY						g_pstrCommonMessage[1545]

#define	dMSG_DECREASE_FIELD_EARTH_RESISTANCE_PENELTY					g_pstrCommonMessage[1546]
#define	dMSG_DECREASE_FIELD_DARK_RESISTANCE_PENELTY						g_pstrCommonMessage[1547]
#define	dMSG_DECREASE_FIELD_LIGHT_RESISTANCE_PENELTY					g_pstrCommonMessage[1548]
#define	dMSG_SELL_MAGIC_ENCHANT_ITEM									g_pstrCommonMessage[1549]
#define	dMSG_DECREASE_STRUCTURE_PRICE									g_pstrCommonMessage[1550]

#define	dMSG_DECREASE_DUNGEON_MAGIC_ATTR_PENELTY						g_pstrCommonMessage[1551]
#define	dMSG_DECREASE_DEATH_PENELTY										g_pstrCommonMessage[1552]
#define	dMSG_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY		g_pstrCommonMessage[1553]
#define	dMSG_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY		g_pstrCommonMessage[1554]
#define	dMSG_STORAGE													g_pstrCommonMessage[1555]

#define	dMSG_RANK_IN_GUILD2												g_pstrCommonMessage[1556]
#define	dMSG_CHANGE_STORAGE_SIZE										g_pstrCommonMessage[1557]
#define	dMSG_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY			g_pstrCommonMessage[1558]
#define	dMSG_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY			g_pstrCommonMessage[1559]
#define	dMSG_COUNT_FORM													g_pstrCommonMessage[1560]

#define	dMSG_KW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE						g_pstrCommonMessage[1561]
#define	dMSG_KW_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY	g_pstrCommonMessage[1562]
#define	dMSG_KW_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY	g_pstrCommonMessage[1563]
#define	dMSG_PLEASE_INPUT_COUNT											g_pstrCommonMessage[1564]
#define	dMSG_ENABLE_WIDTHDRAW_GOLD_LIMIT_RULE_FOR_GUILD_INVENTORY		g_pstrCommonMessage[1565]

#define	dMSG_ENABLE_WIDTHDRAW_ITEM_LIMIT_RULE_FOR_GUILD_INVENTORY		g_pstrCommonMessage[1566]
#define	dMSG_CHANGE_GUILD_INVENTORY_STORAGE_SIZE_RULE					g_pstrCommonMessage[1567]
#define	dMSG_PLEASE_INPUT_CHANGE_GUILD_INVENTORY_STORAGE_SIZE			g_pstrCommonMessage[1568]
#define	dMSG_CHANGED_GUILD_INVENTORY_STORAGE_SIZE						g_pstrCommonMessage[1569]
#define	dMSG_COMPLETE_REQUEST_WORK										g_pstrCommonMessage[1570]

#define	dMSG_ADD_ITEM_ONLY_GUILD_INVENTORY_EMPTY_SLOT					g_pstrCommonMessage[1571]
#define	dMSG_FAILED_COMMAND_WORK										g_pstrCommonMessage[1572]
#define	dMSG_WITHDRAW_ITEM_FROM_GUILD_INVENTORY_FORM					g_pstrCommonMessage[1573]
#define	dMSG_STORE_ITEM_TO_GUILD_INVENTORY_FORM							g_pstrCommonMessage[1574]
#define	dMSG_WITHDRAW_GOLD_FROM_GUILD_INVENTORY_FORM					g_pstrCommonMessage[1575]

#define	dMSG_STORE_GOLD_TO_GUILD_INVENTORY_FORM							g_pstrCommonMessage[1576]
#define	dMSG_ALREADY_EXIST_SAME_VILLAGE_IN_GUILD_RELATED_PLACE_LIST		g_pstrCommonMessage[1577]
#define	dMSG_FULL_VILLAGE_IN_GUILD_RELATED_PLACE_COUNT					g_pstrCommonMessage[1578]	//	더 이상 연고지를 추가 할 수 없다.
#define	dMSG_LOW_GOLD_FOR_ADD_VILLAGE_IN_GUILD_RELATED_PLACE_LIST_FORM	g_pstrCommonMessage[1579]	//	연고지 추가할 돈이 모자른다.
#define	dMSG_NOT_EXIST_VILLAGE_IN_GUILD_RELATED_PLACE_LIST				g_pstrCommonMessage[1580]	//	제거 하려는 마을이 연고지에 없다.

#define	dMSG_MINIMUM_COUNT_RULE_OF_GUILD_RELATED_PLACE_LIST				g_pstrCommonMessage[1581]	//	최소 하나 이상의 길드 연고지가 있어야 한다.
#define	dMSG_ADD_RELATED_PLACE_OF_GUILD_FROM							g_pstrCommonMessage[1582]
#define	dMSG_REMOVE_RELATED_PLACE_OF_GUILD_FROM							g_pstrCommonMessage[1583]
#define	dMSG_TODAY_GUILD_SCRAMBLE										g_pstrCommonMessage[1584]
#define	dMSG_REQUEST_GUILD_SCRAMBLE										g_pstrCommonMessage[1585]

#define	dMSG_DO_YOU_WANT_MOVE_TO_SELECT_GUILD_HALL_FORM					g_pstrCommonMessage[1586]
#define	dMSG_WARP_TO_RELATE_PLACE_OF_GUILD_FORM							g_pstrCommonMessage[1587]
#define	dMSG_DO_YOU_WANT_WARP_TO_RELATE_PLACE_OF_GUILD_FORM				g_pstrCommonMessage[1588]
#define	dMSG_COUNT_OVER_TO_TRY_GUILD_POINT_BATTLE						g_pstrCommonMessage[1589]
#define	dMSG_DO_YOU_WANT_TRY_GUILD_POINT_BATTLE							g_pstrCommonMessage[1590]

#define	dMSG_TO_TRY_GUILD_POINT_BATTLE_GATHER_PARTY_MEMBER_IN_SAME_FIELD	g_pstrCommonMessage[1591]
#define	dMSG_TO_MANY_GUILD_TO_TRY_GUILD_POINT_BATTLE					g_pstrCommonMessage[1592]
#define	dMSG_FAILED_TO_TRY_GUILD_POINT_BATTLE							g_pstrCommonMessage[1593]
#define	dMSG_RSM_CAN_NOT_WARP_OWN_GUILD_HALL_BY_VALID					g_pstrCommonMessage[1594]
#define	dMSG_RSM_FAILED_TO_ENTER_GUILD_BATTLE_FIELD_BY_NOT_BATTLE_TIME	g_pstrCommonMessage[1595]

#define	dMSG_SUCCESS_TO_CAPTURE_GUILD_HALL								g_pstrCommonMessage[1596]
#define	dMSG_SUCCESS_TO_DEFENSE_GUILD_HALL								g_pstrCommonMessage[1597]
#define	dMSG_FAILED_TO_CAPTURE_GUILD_HALL								g_pstrCommonMessage[1598]
#define	dMSG_FAILED_TO_DEFENSE_GUILD_HALL								g_pstrCommonMessage[1599]
#define	dMSG_EW_TOO_MANY_SUMMON_GUILD_GUARDIAN							g_pstrCommonMessage[1600]

#define	dMSG_EW_EXIST_GUILD_GUARDIAN_OR_CREST_IN_TO_CLOSE_RANGE			g_pstrCommonMessage[1601]
#define	dMSG_EW_SGG_IS_USE_ABLE_IN_GUILD_BATTLE							g_pstrCommonMessage[1602]
#define	dMSG_EW_SGG_IS_USE_ABLE_DEFENSE_GUILD							g_pstrCommonMessage[1603]
#define	dMSG_GUILD_STATUE												g_pstrCommonMessage[1604]
#define	dMSG_RENT_GUILD_PET_FORM										g_pstrCommonMessage[1605]

#define	dMSG_RETRIEVAL_GUILD_PET										g_pstrCommonMessage[1606]
#define	dMSG_GUILD_PET_OBITIAN											g_pstrCommonMessage[1607]
#define	dMSG_GUILD_PET_DASH_BLADE										g_pstrCommonMessage[1608]
#define	dMSG_GUILD_PET_ASTRO_BOW										g_pstrCommonMessage[1609]
#define	dMSG_GUILD_PET_CRISTAL_WATER									g_pstrCommonMessage[1610]

#define	dMSG_EW_RENT_GUILD_PET_COUNT_OVER_FORM							g_pstrCommonMessage[1611]
#define	dMSG_SUMMON_GUILD_GUARDIAN_MENU_FORM							g_pstrCommonMessage[1612]
#define	dMSG_GUILD_GUARDIAN_SHIELD_CRAFT								g_pstrCommonMessage[1613]
#define	dMSG_GUILD_GUARDIAN_ELEMENTAL_POST								g_pstrCommonMessage[1614]
#define	dMSG_GUILD_GUARDIAN_ETHER_SHELL									g_pstrCommonMessage[1615]

#define	dMSG_GUILD_GUARDIAN_HOLLOW_KNIGHT								g_pstrCommonMessage[1616]
#define	dMSG_UNSUMMON_GUILD_GUARDIAN									g_pstrCommonMessage[1617]
#define	dMSG_CAN_FIND_GUILD_GUARDIAN_IN_SIGHT							g_pstrCommonMessage[1618]
#define	dMSG_REMOVE_NEAREST_GUILD_GUARDIAN								g_pstrCommonMessage[1619]
#define	dMSG_SUMMON_GUILD_GUARDIAN_FORM									g_pstrCommonMessage[1620]

#define	dMSG_REMAIN_SUMMON_ABLE_GUILD_GUARDIAN_COUNT_FORM				g_pstrCommonMessage[1621]
#define	dMSG_GUILD_GUARDIAN_COUNT_FULL_IN_FIELD							g_pstrCommonMessage[1622]
#define	dMSG_ATTACK_DOOR_RESULT_FORM									g_pstrCommonMessage[1623]
#define	dMSG_INCREASE_GUILD_GUARDIAN_EXP_BY_FEED_ITEM_FORM				g_pstrCommonMessage[1624]
#define	dMSG_INCREASE_GUILD_PET_EXP_BY_FEED_ITEM_FORM					g_pstrCommonMessage[1625]

#define	dMSG_INCREASE_GUILD_GUARDIAN_LEVEL_FORM							g_pstrCommonMessage[1626]
#define	dMSG_INCREASE_GUILD_PET_LEVEL_FORM								g_pstrCommonMessage[1627]
#define	dMSG_INCORRECT_ITEM_FOR_FEED_GUILD_PET_FORM						g_pstrCommonMessage[1628]
#define	dMSG_DO_YOU_WANT_FEED_ITEM_TO_GUILD_PET_FORM					g_pstrCommonMessage[1629]
#define	dMSG_TOOLTIP_FOR_OBITIAN_FORM									g_pstrCommonMessage[1630]

#define	dMSG_TOOLTIP_FOR_DASH_BLADE_FORM								g_pstrCommonMessage[1631]
#define	dMSG_TOOLTIP_FOR_ASTRO_BOW_FORM									g_pstrCommonMessage[1632]
#define	dMSG_TOOLTIP_FOR_CRISTAL_WATER_FORM								g_pstrCommonMessage[1633]
#define	dMSG_TOOLTIP_FOR_SHIELD_CRAFT_FORM								g_pstrCommonMessage[1634]
#define	dMSG_TOOLTIP_FOR_ELEMENTAL_POST_FORM							g_pstrCommonMessage[1635]

#define	dMSG_TOOLTIP_FOR_ETHER_SHELL_FORM								g_pstrCommonMessage[1636]
#define	dMSG_TOOLTIP_FOR_HOLLOW_KNIGHT_FORM								g_pstrCommonMessage[1637]
#define	dMSG_SDM_END_FORM												g_pstrCommonMessage[1638]
#define	dMSG_GUILD_HONOR_POINT											g_pstrCommonMessage[1639]
#define	dMSG_RRT_CAN_NOT_TRADE_STATE									g_pstrCommonMessage[1640]

#define	dMSG_EW_FAILED_DISARM_ACTOR_TRAP_BY_SAME_TEAM					g_pstrCommonMessage[1641]
#define	dMSG_CM_FAILED_BY_EXPIRED_ITEM									g_pstrCommonMessage[1642]
#define	dMSG_TRY_UNLOCK_DOOR_BY_SKILL									g_pstrCommonMessage[1643]
#define	dMSG_EW_CAN_NOT_OPEN_GUILD_SHOP_BY_ITEM_ZERO					g_pstrCommonMessage[1644]
#define	dMSG_DESTROYED_GUILD_STATEGY_POINT_BY_ENEMY						g_pstrCommonMessage[1645]

#define	dMSG_DESTROY_GUILD_STATEGY_POINT								g_pstrCommonMessage[1646]
#define	dMSG_REDUCE_GUILD_CREST_HP										g_pstrCommonMessage[1647]
#define	dMSG_REMOVE_HP_BONUS_BY_GUILD_STATEGY_POINT						g_pstrCommonMessage[1648]
#define	dMSG_INCREASE_ATTACK_DEFENSE_POWER								g_pstrCommonMessage[1649]
#define	dMSG_REDUCE_GUILD_HONOR_POINT_FORM								g_pstrCommonMessage[1650]

#define	dMSG_COMPLETE_TO_GUILD_POINT_BATTLE_FORM						g_pstrCommonMessage[1651]
#define	dMSG_FAILED_TO_GUILD_POINT_BATTLE_FORM							g_pstrCommonMessage[1652]
#define	dMSG_RETURN_TO_GUILD_HALL										g_pstrCommonMessage[1653]
#define	dMSG_DO_YOU_WANT_RETURN_TO_GUILD_HALL_END_GUILD_POINT_BATTLE	g_pstrCommonMessage[1654]
#define	dMSG_CHANGE_OPEN_GUILD_HALL_STATUS								g_pstrCommonMessage[1655]

#define	dMSG_PERMIT_GUILD_HALL_VISTORS									g_pstrCommonMessage[1656]
#define	dMSG_DENY_GUILD_HALL_VISTORS									g_pstrCommonMessage[1657]
#define	dMSG_PERMIT_ONLY_GUILD_MASTER_WORK								g_pstrCommonMessage[1658]
#define	dMSG_PERMIT_ONLY_GUILD_LEADER_WORK								g_pstrCommonMessage[1659]
#define	dMSG_EXP_FOR_UPKEEP_LEVEL										g_pstrCommonMessage[1660]

#define	dMSG_WARNING_FOR_LEVEL_DOWN_GUILD_PET_AND_GUARDIAN				g_pstrCommonMessage[1661]
#define	dMSG_GUILD_GUARDIAN												g_pstrCommonMessage[1662]
#define	dMSG_GUILD_GUARDIAN_FROM										g_pstrCommonMessage[1663]
#define	dMSG_GUILD_HALL_LEVEL											g_pstrCommonMessage[1664]
#define	dMSG_RELATED_PLACE_COUNT_OF_GUILD								g_pstrCommonMessage[1665]

#define	dMSG_RSM_TARGET_GUILD_HALL_IS_CLOSE_TO_STRANGER					g_pstrCommonMessage[1666]
#define	dMSG_MOVE_TO_GUILD_HALL											g_pstrCommonMessage[1667]
#define	dMSG_RSM_CAN_NOT_WITHDRAW_GUILD_ITEM_BY_CHECK_PERMIT_COUNT		g_pstrCommonMessage[1668]
#define	dMSG_RSM_CAN_NOT_WITHDRAW_GUILD_GOLD_BY_CHECK_PERMIT_GOLD		g_pstrCommonMessage[1669]
#define	dMSG_RSM_CAN_NOT_STORE_ITEM_IN_GUILD_INVENTORY					g_pstrCommonMessage[1670]

#define	dMSG_DO_YOU_WANT_MOVE_TO_SELECT_PLACE_FORM						g_pstrCommonMessage[1671]
#define	dMSG_RSM_VALID_GUILD_SKILL_LEVEL								g_pstrCommonMessage[1672]
#define	dMSG_GWM_LACK_INCREASE_MEMBER_COUNT_SKILL_LEVEL					g_pstrCommonMessage[1673]
#define	dMSG_RESET_GUILD_SKILL_LEVEL									g_pstrCommonMessage[1674]
#define	dMSG_INCREASE_DAMAGE_UNDER_SIEGE_WARFARE						g_pstrCommonMessage[1675]

#define	dMSG_REDUCE_DAMAGE_UNDER_SIEGE_WARFARE							g_pstrCommonMessage[1676]
#define	dMSG_INCREASE_GUILD_PET_SIGHT									g_pstrCommonMessage[1677]
#define	dMSG_INCREASE_GUILD_CREST_EFFICIENT								g_pstrCommonMessage[1678]
#define	dMSG_INCREASE_GUILD_FLAG_EFFICIENT								g_pstrCommonMessage[1679]
#define	dMSG_INCREASE_ASTRO_BOW_DAMAGE									g_pstrCommonMessage[1680]

#define	dMSG_INCREASE_GUILD_DOOR_EFFICIENT								g_pstrCommonMessage[1681]
#define	dMSG_INCREASE_GUILD_PET_ATTACK_SPEED							g_pstrCommonMessage[1682]
#define	dMSG_DECREASE_GUILD_SHOP_ITEM_PRICE								g_pstrCommonMessage[1683]
#define	dMSG_INCREASE_DASH_BLADE_DAMAGE									g_pstrCommonMessage[1684]
#define	dMSG_INCREASED_GUILD_SKILL_LEVEL_FORM							g_pstrCommonMessage[1685]

#define	dMSG_INCREASED_GUILD_STATUE_SKILL_LEVEL_FORM					g_pstrCommonMessage[1686]
#define	dMSG_GUILD_MEMBER_COUNT											g_pstrCommonMessage[1687]
#define	dMSG_SUB_GUILD_MASTER_COUNT										g_pstrCommonMessage[1688]
#define	dMSG_GUILD_GONGRESS_COUNT										g_pstrCommonMessage[1689]
#define	dMSG_GUILD_INVENTORY_SIZE										g_pstrCommonMessage[1690]

#define	dMSG_QUESTION_FOR_ADD_RELATED_PLACE_OF_GUILD_FORM				g_pstrCommonMessage[1691]
#define	dMSG_RSM_TEMP_LIMIT_GOLD_IN_GUILD_INVENTORY						g_pstrCommonMessage[1692]
#define	dMSG_LIST_OF_GUILD_HALL_LEVEL_FORM								g_pstrCommonMessage[1693]
#define	dMSG_GUILD_HONOR_POINT_FORM										g_pstrCommonMessage[1694]
#define	dMSG_RSM_FAILED_BY_INSTANCE_GUILD_BATTLE_TIME_RULE				g_pstrCommonMessage[1695]

#define	dMSG_CAN_NOT_RETURN_TO_GUILD_HALL_DURING_BATTLE_FORM			g_pstrCommonMessage[1696]
#define	dMSG_INCREASE_EXP_FORM											g_pstrCommonMessage[1697]
#define	dMSG_INCREASE_ITEM_DROP_CHANCE_FORM								g_pstrCommonMessage[1698]
#define	dWM_MINIPET_TYPE_NAME_BEGIN										1699

#define	dWM_MINIPET_TYPE_NAME_END										1722
#define	dMSG_CAN_NOT_SUMMON_MORE_MINIPET								g_pstrCommonMessage[1723]
#define	dMSG_CREATE_MINIPET_FORM										g_pstrCommonMessage[1724]
#define	dMSG_MINIPET_EVOLUTION_GRADE									g_pstrCommonMessage[1725]

#define	dMSG_MINIPET_GROW_RATE											g_pstrCommonMessage[1726]
#define	dMSG_MINIPET_BIO												g_pstrCommonMessage[1727]
#define	dMSG_MINIPET_ENERGY												g_pstrCommonMessage[1728]
#define	dMSG_MINIPET_MINERAL											g_pstrCommonMessage[1729]
#define	dMSG_ACTIVATE													g_pstrCommonMessage[1730]

#define	dMSG_DISACTIVATE												g_pstrCommonMessage[1731]
#define	dMSG_BLOCK_LIGHT_POCKET											g_pstrCommonMessage[1732]
#define	dMSG_RELEASE_MINIPET_WARNING									g_pstrCommonMessage[1733]
#define	dMSG_MINIPET_VALIDATE_TIME_FORM									g_pstrCommonMessage[1734]
#define	dMSG_CHANGE_MINIPET_NAME_KEYWORD								g_pstrCommonMessage[1735]

#define	dMSG_INCORRECT_ITEM_FOR_FEED_MINIPET							g_pstrCommonMessage[1736]
#define	dMSG_DO_YOU_WANT_FEED_ITEM_TO_MINI_PET_FORM						g_pstrCommonMessage[1737]
#define	dMSG_INCREASE_MINIPET_LEVEL										g_pstrCommonMessage[1738]
#define	dMSG_INCREASE_MINIPET_EXP_FORM									g_pstrCommonMessage[1739]
#define	dMSG_EVLOUTION_MINIPET_FORM										g_pstrCommonMessage[1740]

#define	dMSG_CHARGE_MINIPET_SKILL_FORM									g_pstrCommonMessage[1741]
#define	dMSG_ITS_TO_USE_ENEMY_SKILL										g_pstrCommonMessage[1742]
#define	dMSG_CAN_NOT_FIND_ATTACK_ABLE_TARGET							g_pstrCommonMessage[1743]
#define	dMSG_STT_INCLINE_POWER_BY_MP_FORM								g_pstrCommonMessage[1744]
#define	dMSG_STT_DECREASE_FILED_MAGIC_PENELTY_FORM						g_pstrCommonMessage[1745]

#define	dMSG_STT_INCREASE_PLAYER_FINAL_PHYSICAL_ATTACK_POWER_FORM		g_pstrCommonMessage[1746]
#define	dMSG_STT_DECLINE_TARGET_MAGIC_RESISTANCE_FORM					g_pstrCommonMessage[1747]
#define	dMSG_STT_INCLINE_DODGE_CHANCE_FORM								g_pstrCommonMessage[1748]
#define	dMSG_STT_INCLINE_HIT_CHANCE_FORM								g_pstrCommonMessage[1749]
#define	dMSG_STT_STRENGTHENING_MAGIC_DAMAGE_FORM						g_pstrCommonMessage[1750]

#define	dMSG_STT_CORRECT_ITEM_EQUIP_LEVEL_FORM							g_pstrCommonMessage[1751]
#define	dMSG_STT_INCREASE_SKILL_LEVEL_FORM								g_pstrCommonMessage[1752]
#define	dMSG_STT_INCREASE_DEFENSE_POWER_PERCENTAGE_FORM					g_pstrCommonMessage[1753]
#define	dMSG_STT_CURE_STRANGE_STATUS_FORM								g_pstrCommonMessage[1754]
#define	dMSG_STT_GOLD_HOLDER_FORM										g_pstrCommonMessage[1755]

#define	dMSG_STT_ITEM_COLLECTOR_FORM									g_pstrCommonMessage[1756]
#define	dMSG_STT_INCLINE_ATTACK_SPEED_FORM								g_pstrCommonMessage[1757]
#define	dMSG_STT_INCLINE_MOVE_SPEED_FORM								g_pstrCommonMessage[1758]
#define	dMSG_STT_RECOVER_HP_WITH_CHANCE_FORM							g_pstrCommonMessage[1759]
#define	dMSG_STT_RECOVER_HP_WITHOUT_CHANCE_FORM							g_pstrCommonMessage[1760]

#define	dMSG_STT_INCLINE_MAX_CP_FORM									g_pstrCommonMessage[1761]
#define	dMSG_STT_INCLINE_MAX_HP_FORM									g_pstrCommonMessage[1762]
#define	dMSG_STT_CHARGE_CP_FORM											g_pstrCommonMessage[1763]
#define	dMSG_STT_EFFECT_ACTIVE_CHANCE_AND_PERIOD_FORM					g_pstrCommonMessage[1764]
#define	dMSG_STT_INCLINE_ALL_MAGIC_RESISTANCE_FORM						g_pstrCommonMessage[1765]

#define	dMSG_STT_INCLINE_MAGIC_RESISTANCE_FORM							g_pstrCommonMessage[1766]
#define	dMSG_MSTT_RECOVER_USE_CP_FORM									g_pstrCommonMessage[1767]
#define	dMSG_MSTT_USE_CHARGE_PORION_FORM								g_pstrCommonMessage[1768]
#define	dMSG_MSTT_HealBonusByHealSkill_FORM								g_pstrCommonMessage[1769]
#define	dMSG_MSTT_IncreaseDropItemChance_FORM							g_pstrCommonMessage[1770]

#define	dMSG_MSTT_IncreaseDropGoldChance_FORM							g_pstrCommonMessage[1771]
#define	dMSG_MSTT_UseHealPotion_FORM									g_pstrCommonMessage[1772]
#define	dMSG_MSTT_IgnoreTargetMagicResistanceUnderAllBossBattle_FORM	g_pstrCommonMessage[1773]
#define	dMSG_MSTT_IgnoreTargetMagicResistanceUnderBossBattle_FORM		g_pstrCommonMessage[1774]
#define	dMSG_MSTT_PowerUpUnderBattleToAllBoss_FORM						g_pstrCommonMessage[1775]

#define	dMSG_MSTT_PowerUpUnderBattleToBoss_FORM							g_pstrCommonMessage[1776]
#define	dMSG_MSTT_ActionWhenDamaged_FORM								g_pstrCommonMessage[1777]
#define	dMSG_MSTT_IncreaeUnlockLevel_FORM								g_pstrCommonMessage[1778]
#define	dMSG_MSTT_DecreaseResurrectionPenelty_FORM						g_pstrCommonMessage[1779]
#define	dMSG_MSTT_Resurrection_FORM										g_pstrCommonMessage[1780]

#define	dMSG_MSTT_AvoidTrap_FORM										g_pstrCommonMessage[1781]
#define	dMSG_MSTT_IncreaseActiveChanceUnderAllBossBattle_FORM			g_pstrCommonMessage[1782]
#define	dMSG_MSTT_IncreaseActiveChanceUnderBossBattle_FORM				g_pstrCommonMessage[1783]
#define	dMSG_MSTT_DecreaseChargeCount_FORM								g_pstrCommonMessage[1784]
#define	dMSG_MSTT_AttackTheObject_FORM									g_pstrCommonMessage[1785]

#define	dMSG_MSTT_INCREASE_ACTIVE_CHANCE_FORM							g_pstrCommonMessage[1786]
#define	dMSG_MSTT_INCREASE_HIT_RANGE_FORM								g_pstrCommonMessage[1787]
#define	dMSG_MSTT_INCREASE_DAMAGE_FORM									g_pstrCommonMessage[1788]
#define	dMSG_HIT_RANGE_FORM												g_pstrCommonMessage[1789]
#define	dMSG_MSTT_CHARGE_AND_FIRE_FORM									g_pstrCommonMessage[1790]

#define	dMSG_USE_BLOCK_LIGHT_POCKET										g_pstrCommonMessage[1791]
#define	dMSG_REALLY_USE_POLISHER										g_pstrCommonMessage[1792]
#define	dMSG_REALLY_USE_MASSIVE_POLISHER								g_pstrCommonMessage[1793]
#define	dMSG_IS_NOT_ABLE_TO_TRY_GUILD_POINT_BATTLE_TIME					g_pstrCommonMessage[1794]
#define	dMSG_CAN_NOT_GROUW_UP_MINIPET									g_pstrCommonMessage[1795]

#define	dMSG_DO_YOU_WANT_CHANGE_SELECT_MINI_PET_NAME					g_pstrCommonMessage[1796]
#define	dMSG_RESET_SELECTED_MINIPET_NAME_THAN_ABLE_TO_CHANGE_NAME		g_pstrCommonMessage[1797]
#define	dMSG_CAN_NOT_ACTIVATE_MINIPET_BY_IS_EXPIRED_MINIPET				g_pstrCommonMessage[1798]
#define	dMSG_CURRENT_MINIPET_GOTO_REST_BY_EXPIRED						g_pstrCommonMessage[1799]
#define	dMSG_SKILL_INFO													g_pstrCommonMessage[1800]

#define	dMSG_MINIPET_UNSEAL_EXPIRE_TIME_RULE							g_pstrCommonMessage[1801]
#define	dMSG_TOGGLE_MINIPET_WINDOW_COMMENT								g_pstrCommonMessage[1802]
#define	dMSG_MINIPET_FEED_TYPE_BIO										g_pstrCommonMessage[1803]
#define	dMSG_MINIPET_FEED_TYPE_ENERGY									g_pstrCommonMessage[1804]
#define	dMSG_MINIPET_FEED_TYPE_MINERAL									g_pstrCommonMessage[1805]

#define	dMSG_MINIPET_FEED_TYPE											g_pstrCommonMessage[1806]
#define	dMSG_EXPIRED_REVERSAL_FRAME_TIME								g_pstrCommonMessage[1807]
#define	dMSG_FOWORD_LEFT												g_pstrCommonMessage[1808]
#define	dMSG_FOWORD_RIGHT												g_pstrCommonMessage[1809]
#define	dMSG_FOWORD_UP													g_pstrCommonMessage[1810]

#define	dMSG_FOWORD_DOWN												g_pstrCommonMessage[1811]
#define	dMSG_REVERSIBLE_FRAME_VALIDATE_FORM								g_pstrCommonMessage[1812]
#define	dMSG_INVALID_REVERSIBLE_FRAME									g_pstrCommonMessage[1813]
#define	dMSG_EXTENDED_REVERSAL_FRAME_VALIDATE							g_pstrCommonMessage[1814]
#define	dMSG_INVALID_MINPET_COMBI_SKILL									g_pstrCommonMessage[1815]

#define	dMSG_REVERSIBLE_FRAME_COMMENT_FORM								g_pstrCommonMessage[1816]
#define	dMSG_MINIPET_COMBI_SKILLCOMMENT_FORM							g_pstrCommonMessage[1817]
#define	dMSG_INCREASE_PET_SKILL_DAMAGE_FORM								g_pstrCommonMessage[1818]
#define	dMSG_INCREASE_PET_SKILL_ACTIVE_CAHNCE_FORM						g_pstrCommonMessage[1819]
#define	dMSG_INCREASE_BASIC_PET_SKILL_LEVEL_FORM						g_pstrCommonMessage[1820]

#define	dMSG_INCREASE_PET_SKILL_LEVEL_FORM								g_pstrCommonMessage[1821]
#define	dMSG_EVOLUTION_BONUS											g_pstrCommonMessage[1822]
#define	dMSG_BUY_COUNT													g_pstrCommonMessage[1823]
#define	dMSG_RESET_ALLIGNMENT_POINT										g_pstrCommonMessage[1824]
#define	dMSG_INCREASE_ALLIGNMENT_POINT_FORM								g_pstrCommonMessage[1825]

#define	dMSG_DECREASE_ALLIGNMENT_POINT_FORM								g_pstrCommonMessage[1826]
#define	dMSG_SET_ALLIGNMENT_POINT_FORM									g_pstrCommonMessage[1827]
#define	dMSG_INCREASE_GUILD_POINT										g_pstrCommonMessage[1828]
#define	dMSG_RESET_CH5_HP_CP_BONUS										g_pstrCommonMessage[1829]
#define	dMSG_RECOVER_CH5_HP_BONUS_PENELTY								g_pstrCommonMessage[1830]

#define	dMSG_RECOVER_CH5_CP_BONUS_PENELTY								g_pstrCommonMessage[1831]
#define	dMSG_RECOVER_CH5_BONUS_GUILD_POINT_PENELTY						g_pstrCommonMessage[1832]
#define	dMSG_INCREASE_CH5_STATE_BONUS									g_pstrCommonMessage[1833]
#define	dMSG_INCREASE_CH5_SKILL_BONUS									g_pstrCommonMessage[1834]
#define	dMSG_INCREASE_CH5_HP_BONUS										g_pstrCommonMessage[1835]

#define	dMSG_INCREASE_CH5_CP_BONUS										g_pstrCommonMessage[1836]
#define	dMSG_RECEIVE_CH5_SKILL_POINT_COUONT_FORM						g_pstrCommonMessage[1837]
#define	dMSG_RECEIVE_CH5_STATE_POINT_COUONT_FORM						g_pstrCommonMessage[1838]
#define	dMSG_YMDHM_FORM													g_pstrCommonMessage[1839]
#define	dMSG_RECEIVE_CH5_HP_POINT_BONUS_PENELTY_COUNT_FORM				g_pstrCommonMessage[1840]

#define	dMSG_RECEIVE_CH5_HP_POINT_BONUS_COUNT_FORM						g_pstrCommonMessage[1841]
#define	dMSG_RECEIVE_CH5_CP_POINT_BONUS_PENELTY_COUNT_FORM				g_pstrCommonMessage[1842]
#define	dMSG_RECEIVE_CH5_CP_POINT_BONUS_COUNT_FORM						g_pstrCommonMessage[1843]
#define	dMSG_COMPLETE													g_pstrCommonMessage[1844]
#define	dMSG_CH5_UPGRADE_TITLE_BOTTOM_FORM								g_pstrCommonMessage[1845]

#define	dMSG_INCREASE_LEVEL_TITLE										g_pstrCommonMessage[1846]
#define	dMSG_INCREASE_GUILD_EXP_FORM									g_pstrCommonMessage[1847]
#define	dMSG_LETS_TRANS													g_pstrCommonMessage[1848]
#define	dMSG_SELECT_UNSEAL_ITEM											g_pstrCommonMessage[1849]
#define	dMSG_RELEASE_ITEM_REVERSION_FORM								g_pstrCommonMessage[1850]

#define	dMSG_SELECT_RESET_STATE											g_pstrCommonMessage[1851]
#define	dMSG_RESET_STATE_POINT_FORM										g_pstrCommonMessage[1852]
#define	dMSG_RESET_SKILL_POINT_FORM										g_pstrCommonMessage[1853]
#define	dMSG_SELECT_RESET_SKILL											g_pstrCommonMessage[1854]
#define	dMSG_THERE_ARE_NOTHING_TO_RESET_ABLE_SKILL						g_pstrCommonMessage[1855]

#define	dMSG_THERE_ARE_NOTHING_TO_RESET_ABLE_STATE						g_pstrCommonMessage[1856]
#define	dMSG_CHANGE_SKILL_LEVEL_FORM									g_pstrCommonMessage[1857]
#define	dMSG_RESET_STATE_FORM											g_pstrCommonMessage[1858]
#define	dMSG_WHEN_RESET_STATE_BONUS_POINT_TT_FORM						g_pstrCommonMessage[1859]
#define	dMSG_WHEN_RESET_SKILL_BONUS_POINT_TT_FORM						g_pstrCommonMessage[1860]

#define	dMSG_BROKEN_ITEM_BY_CH5_REWARD									g_pstrCommonMessage[1861]
#define	dMSG_COPY_ITEM_BY_CH5_REWARD_FORM								g_pstrCommonMessage[1862]
#define	dMSG_FAILED_TO_ITEM_ITS_NOT_EQUIPMENT							g_pstrCommonMessage[1863]
#define	dMSG_THERE_ARE_NOTHING_TO_LEVELUP_ABLE_ITEM						g_pstrCommonMessage[1864]
#define	dMSG_UPGRADE_ITEM												g_pstrCommonMessage[1865]

#define	dMSG_UPGRADE_PREFIX_FORM										g_pstrCommonMessage[1866]
#define	dMSG_UPGRADE_PREFIX_RESULT_FORM									g_pstrCommonMessage[1867]
#define	dMSG_PREFIX_LEVEL_UP_FORM										g_pstrCommonMessage[1868]
#define	dMSG_DO_YOU_WANT_COPY_THIS_ITEM_FORM							g_pstrCommonMessage[1869]
#define	dMSG_UPGRADE_ITEM_RESULT_FORM									g_pstrCommonMessage[1870]

#define	dMSG_HEAVEN_GAE													g_pstrCommonMessage[1871]
#define	dMSG_HELL_GAE													g_pstrCommonMessage[1872]
#define	dMSG_RED_DEVIL													g_pstrCommonMessage[1873]
#define	dMSG_INCREASE_HEAVEN_HELL_RED_STONE_FORM						g_pstrCommonMessage[1874]
#define	dMSG_INCREASE_RED_DEVILS_RED_STONE_FORM							g_pstrCommonMessage[1875]

#define	dMSG_RED_STONE_COUNT_FORM										g_pstrCommonMessage[1876]
#define	dMSG_RESISTANCE_BONUS_FORM										g_pstrCommonMessage[1877]
#define	dMSG_RESISTANCE_PENELTY_FORM									g_pstrCommonMessage[1878]
#define	dMSG_EXP_BONUS_FORM												g_pstrCommonMessage[1879]
#define	dMSG_EXP_PENELTY_FORM											g_pstrCommonMessage[1880]

#define	dMSG_TRADE_BONUS_FORM											g_pstrCommonMessage[1881]
#define	dMSG_TRADE_PENELTY_FORM											g_pstrCommonMessage[1882]
#define	dMSG_ITEM_DROP_LEVEL_BONUS_FORM									g_pstrCommonMessage[1883]
#define	dMSG_ITEM_DROP_CHANCE_BONUS_FORM								g_pstrCommonMessage[1884]
#define	dMSG_RED_DEVIL_FACTOR_FORM										g_pstrCommonMessage[1885]

#define	dMSG_ALLIGNMENT_BONUS_INFO										g_pstrCommonMessage[1886]
#define	dMSG_BARD_TITLE_CAN_USE_UNDER_SITDOWN							g_pstrCommonMessage[1887]
#define	dMSG_WAIT_TO_USE_TITLE_COOLTIME_FORM							g_pstrCommonMessage[1888]
#define	dMSG_WAIT_TO_END_SONG_THEN_ACTIVE_EFFECT						g_pstrCommonMessage[1889]
#define	dMSG_CANCEL_CASTING_BARD_TITLE									g_pstrCommonMessage[1890]

#define	dMSG_REQUIRE_PORTAL_ITEM_TO_USE_THAT_TITLE						g_pstrCommonMessage[1891]
#define	dMSG_PLEASE_SELECT_WANT_VALUE									g_pstrCommonMessage[1892]
#define	dMSG_BONUS_GUILD_POINT_FORM										g_pstrCommonMessage[1893]
#define	dMSG_BONUS_PENELTY_GUILD_POINT_FORM								g_pstrCommonMessage[1894]
#define	dMSG_PREFIX_UPGRADE_FORM										g_pstrCommonMessage[1895]

#define	dMSG_PREFIX_2VALUE_UPGRADE_FORM									g_pstrCommonMessage[1896]
#define	dMSG_PLEASE_SELECT_MINIPET_FOR_CHANGE_NAME						g_pstrCommonMessage[1897]
#define	dMSG_REQUIRE_MINIPET_FOR_USE_THAT_ITEM							g_pstrCommonMessage[1898]
#define	dMSG_DUEL_SERVER												g_pstrCommonMessage[1899]
#define	dMSG_COMPLETE_COPY_DATA_TO_DUEL_SERVER_WORK						g_pstrCommonMessage[1900]

#define	dMSG_DUEL_VP													g_pstrCommonMessage[1901]
#define	dMSG_DUEL_POINT													g_pstrCommonMessage[1902]
#define	dMSG_WCG_VP														g_pstrCommonMessage[1903]
#define	dMSG_DUEL_RECORD_FORM											g_pstrCommonMessage[1904]
#define	dMSG_DUEL_RECORD												g_pstrCommonMessage[1905]

#define	dMSG_ASKING_DUEL_RECORD											g_pstrCommonMessage[1906]
#define	dMSG_MC_RED_DRAKE_ACTIVATED										g_pstrCommonMessage[1907]
#define	dMSG_MC_SUMMON_RED_DRAKE										g_pstrCommonMessage[1908]
#define	dMSG_MC_SUMMON_RED_DRAKE_COMMENT								g_pstrCommonMessage[1909]
#define	dMSG_FAILED_DATA_COPY_TO_TEST_SERVER_WORK						g_pstrCommonMessage[1910]

#define	dMSG_FAILED_DATA_COPY_TO_DUEL_SERVER_WORK						g_pstrCommonMessage[1911]
#define	dMSG_DUEL_APPLICANT_FORM										g_pstrCommonMessage[1912]
#define	dMSG_WAIT_FOR_DUEL_APPLICANT_COUNT								g_pstrCommonMessage[1913]
#define	dMSG_CANCEL_TO_TRY_DUEL_BY_CHANGE_PARTY_MEMBER					g_pstrCommonMessage[1914]
#define	dMSG_REMAIN_TIME_TO_REBIRTH_FORM								g_pstrCommonMessage[1915]

#define	dMSG_DECIDED_DUEL_MATCH_TARGET_AND_MOVE_TO_DUEL_FIELD			g_pstrCommonMessage[1916]
#define	dMSG_READY_TO_BATTLE_REMAIN_SOMETIME_FORM						g_pstrCommonMessage[1917]
#define	dMSG_BEGIN_DUEL													g_pstrCommonMessage[1918]
#define	dMSG_END_DUEL_AFTER_SOMETIME_FORM								g_pstrCommonMessage[1919]
#define	dMSG_END_DUEL													g_pstrCommonMessage[1920]

#define	dMSG_KILL_TARGET_FOR_DUEL_FORM									g_pstrCommonMessage[1921]
#define	dMSG_KILL_COUNT													g_pstrCommonMessage[1922]
#define	dMSG_DEATH_COUNT												g_pstrCommonMessage[1923]
#define	dMSG_WEEKLY_VP													g_pstrCommonMessage[1924]
#define	dMSG_ENEMY_TEAM													g_pstrCommonMessage[1925]

#define	dMSG_OUR_TEAM													g_pstrCommonMessage[1926]
#define	dMSG_RESET_PLAYER_INFO_FOR_DUEL_SERVER							g_pstrCommonMessage[1927]
#define	dMSG_RANKING													g_pstrCommonMessage[1928]
#define	dMSG_WEEKLY_RANKING												g_pstrCommonMessage[1929]
#define	dMSG_DAILY_RANKING												g_pstrCommonMessage[1930]

#define	dMSG_WCG_RANKING												g_pstrCommonMessage[1931]
#define	dMSG_WEEKLY_BATTLE_RECORD_FORM									g_pstrCommonMessage[1932]
#define	dMSG_TOTAL_BATTLE_RECORD_FORM									g_pstrCommonMessage[1933]
#define	dMSG_OUT_OF_RANKING												g_pstrCommonMessage[1934]
#define	dMSG_DUEL_CHARACTER_INFO										g_pstrCommonMessage[1935]

#define	dMSG_OUT_OF_DUEL_RECORD											g_pstrCommonMessage[1936]
#define	dMSG_IS_MOVE_BY_END_OF_DUEL										g_pstrCommonMessage[1937]
#define	dMSG_WEEKLY_DUEL_RANK											g_pstrCommonMessage[1938]
#define	dMSG_DAILY_DUEL_RANK											g_pstrCommonMessage[1939]
#define	dMSG_WCG_DUEL_RANK												g_pstrCommonMessage[1940]

#define	dMSG_REMAIN_TIME_FOR_USE_DUEL_BUFF_FORM							g_pstrCommonMessage[1941]
#define	dMSG_DUEL_FIELD													g_pstrCommonMessage[1942]
#define	dMSG_TEAM_NAME													g_pstrCommonMessage[1943]
#define	dMSG_TEAM_MEMBER_INFO											g_pstrCommonMessage[1944]
#define	dMSG_ASKINFO_INFO_TO_SERVER										g_pstrCommonMessage[1945]

#define	dMSG_END_DUEL_BATTLE_BY_OUT_ALL_TARGET_TEAM_MEMBER				g_pstrCommonMessage[1946]
#define	dMSG_AR_CAN_NOT_USE_IN_DUEL_FIELD_SKILL							g_pstrCommonMessage[1947]
#define	dMSG_RSM_DUEL_CAN_NOT_TRY_BY_BAD_PARTY_STATUS					g_pstrCommonMessage[1948]
#define	dMSG_RSM_DUEL_CAN_NOT_TRY_BY_TO_MANY_BATTLER					g_pstrCommonMessage[1949]
#define	dMSG_RSM_DUEL_1VS1_RULE											g_pstrCommonMessage[1950]

#define	dMSG_RSM_DUEL_FIELD_IS_FULL										g_pstrCommonMessage[1951]
#define	dMSG_EW_EAT_DUEL_FIELD_ARCA_ITEM_FORM							g_pstrCommonMessage[1952]
#define	dMSG_TRY_DUEL_BATTLE_FORM										g_pstrCommonMessage[1953]
#define	dMSG_IS_BLOCK_IN_DUEL_FIELD										g_pstrCommonMessage[1954]
#define	dMSG_LACK_OF_DUEL_POINT											g_pstrCommonMessage[1955]

#define	dMSG_BUY_DUEL_POINT_ITEM_FORM									g_pstrCommonMessage[1956]
#define	dMSG_1VS1_DUEL_APPLICANT_FORM									g_pstrCommonMessage[1957]
#define	dMSG_DMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_SEASON				g_pstrCommonMessage[1958]
#define	dMSG_DMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY_PARTY_FORM		g_pstrCommonMessage[1959]
#define	dMSG_PREFIX_VALUE_REFORM_RANGE_FORM								g_pstrCommonMessage[1960]

#define	dMSG_RESULT_OF_DUEL												g_pstrCommonMessage[1961]
#define	dMSG_RSM_DUEL_EXCEPTION_TIME_RULE								g_pstrCommonMessage[1962]
#define	dMSG_DMSG_CAN_NOT_TRY_TRIAL_BATTLE_BY_NOT_ENTRY_FORM			g_pstrCommonMessage[1963]
#define	dMSG_RSM_DUEL_CAN_NOT_TRY_BY_YOU_ARE_NOT_PARTY_LEADER			g_pstrCommonMessage[1964]
#define	dMSG_RSM_DUEL_CAN_NOT_TRY_BY_MISMATCH_PARTY_MEMBER_COUNT		g_pstrCommonMessage[1965]

#define	dMSG_EMPTY_BULLET_FORM											g_pstrCommonMessage[1966]
#define	dMSG_WARP_NET_CAFE_USER_CONFIRM_FORM							g_pstrCommonMessage[1967]
#define dMSG_USE_NET_CAFE_MENU_COMMENT									g_pstrCommonMessage[1968]
#define dMSG_CAN_NOT_PRINCSS_WEAPON_CHANGE								g_pstrCommonMessage[1969]
#define dMSG_QUEST_REMAIN_TIME											g_pstrCommonMessage[1970]

#define dMSG_GGG														g_pstrCommonMessage[1971]
#define dMSG_PARTY_INFO													g_pstrCommonMessage[1972]
#define dMSG_UNION_PARTY												g_pstrCommonMessage[1973]
#define dMSG_PARTY_LIST_COMMENT2										g_pstrCommonMessage[1974]
#define dMSG_PARTY_LIST_COMMENT3										g_pstrCommonMessage[1975]

#define dMSG_USE_BUFF_FORM												g_pstrCommonMessage[1976]
#define dMSG_WINNING_GUILD_FORM											g_pstrCommonMessage[1977]
#define dMSG_DRAW_GAME													g_pstrCommonMessage[1978]
#define dMSG_REALLY_USE_AWAKEN50										g_pstrCommonMessage[1979]
#define dMSG_REALLY_USE_AWAKEN100										g_pstrCommonMessage[1980]

#define dMSG_TARGET_MINI_PET											g_pstrCommonMessage[1981]
#define dMSG_MINI_PET_AWAKEN_ITEM50										g_pstrCommonMessage[1982]
#define dMSG_MINI_PET_AWAKEN_ITEM100									g_pstrCommonMessage[1983]
#define dMSG_USE_MINI_PET_AWAKEN_ITEM50									g_pstrCommonMessage[1984]
#define dMSG_USE_MINI_PET_AWAKEN_ITEM100								g_pstrCommonMessage[1985]

#define dMSG_MINI_PET_CANT_USE_AWAKEN_ITEM_LOWER						g_pstrCommonMessage[1986]
#define dMSG_PLASE_SELECT_UNION_PARTY_PLAYER							g_pstrCommonMessage[1987]
#define dMSG_INVITATION_UNION_PARTY										g_pstrCommonMessage[1988]
#define dMSG_DISSOLUTION_UNION_PARTY									g_pstrCommonMessage[1989]
#define dMSG_REMEMBER_THIS_PLACE_TO_SLOT3								g_pstrCommonMessage[1990]

#define dMSG_DO_YOU_WANT_TRY_GUILD_DUNGEN								g_pstrCommonMessage[1991]
#define dMSG_GUILD_MASTER_GRACE_GUILD_MENU								g_pstrCommonMessage[1992]
#define dMSG_GUILD_MASTER_GRACE_ACTIVE_2HOUR							g_pstrCommonMessage[1993]
#define dMSG_ARE_YOU_ACTIVE_GUILD_MASTER_GRACE							g_pstrCommonMessage[1994]
#define dMSG_GUILD_MASTER_GRACE_END										g_pstrCommonMessage[1995]

#define dMSG_GUILD_MASTER_GRACE_ERROR1_ONLY_MASTER_USE					g_pstrCommonMessage[1996]
#define dMSG_GUILD_MASTER_GRACE_ERROR2_ONLY_USE_ONCE_IN_WEEK			g_pstrCommonMessage[1997]
#define dMSG_GUILD_MASTER_GRACE_ERROR3_LOW_GUILD_HOLL_LEVEL				g_pstrCommonMessage[1998]
#define dMSG_GUILD_MASTER_GRACE_ERROR4_UNKNOWEN							g_pstrCommonMessage[1999]
#define dMSG_TO_TRY_GUILD_DUNGEN_GATHER_PARTY_MEMBER_IN_SAME_FIELD		g_pstrCommonMessage[2000]

#define dMSG_COUNT_OVER_TO_TRY_GUILD_DUNGEN								g_pstrCommonMessage[2001]
#define dMSG_TO_MANY_GUILD_TO_TRY_GUILD_DUNGEN							g_pstrCommonMessage[2002]
#define dMSG_FAILED_TO_TRY_GUILD_DUNGEN									g_pstrCommonMessage[2003]
#define dMSG_COMPLATE_BONUS												g_pstrCommonMessage[2004]
#define	dMSG_GUILD_DUNGEON_LOGOUT_FORM									g_pstrCommonMessage[2005]

#define	dMSG_GDM_COMPLETE_FORM											g_pstrCommonMessage[2006]
#define dMSG_GDM_END_FORM												g_pstrCommonMessage[2007]
#define dMSG_TOKKEN_SHOP_FORM											g_pstrCommonMessage[2008]
#define	dMSG_LACK_OF_TOKKEN												g_pstrCommonMessage[2009]
#define	dMSG_WARNING_DISAPEAR_GUILD_DUNGEON_FORM						g_pstrCommonMessage[2010]

#define	dMSG_DISJOINTING_ITEM_SUCCESS									g_pstrCommonMessage[2011]
#define	dMSG_DISJOINTING_ITEM_TOP_TEXT									g_pstrCommonMessage[2012]
#define	dMSG_DISJOINTING_ITEM_BUTTON_TEXT								g_pstrCommonMessage[2013]
#define	dMSG_DISJOINTING_ITEM_FAILED									g_pstrCommonMessage[2014]
#define	dMSG_DISJOINTING_ITEM_FAILED_INVENTORY							g_pstrCommonMessage[2015]

#define	dMSG_DISJOINTING_ITEM_FAILED_IS_NOT_UNIQUE						g_pstrCommonMessage[2016]
#define	dMSG_FEED_ALL_ITEM_BY_TYPE										g_pstrCommonMessage[2017]
#define	dMSG_REALLY_RESET_ONE_STATE_POINT_FORM							g_pstrCommonMessage[2018]
#define	dMSG_COOL_TIME													g_pstrCommonMessage[2019]
#define	dMSG_WARNING_ITEM_COOLTIME										g_pstrCommonMessage[2020]

#define	dMSG_DO_YOU_REALLY_RESET_PET_NAME_FORM							g_pstrCommonMessage[2021]
#define	dMSG_CAHNGED_PET_NAME											g_pstrCommonMessage[2022]
#define	dMSG_RESETED_PET_NAME											g_pstrCommonMessage[2023]
#define	dMSG_USE_SOUND_OF_LEADERS_BELL_FORM								g_pstrCommonMessage[2024]
#define	dMSG_UPKEEPING_SOUND_OF_LEADERS_BELL_FORM						g_pstrCommonMessage[2025]

#define	dMSG_INCREASE_EXP_AND_ITEM_DROP_CHANCE_BY_SOUND_OF_LEADERS_BELL	g_pstrCommonMessage[2026]
#define	dMSG_UNKWON_SOMEBODY											g_pstrCommonMessage[2027]
#define	dMSG_END_EFFECT_OF_SOUND_OF_LEADERS_BELL						g_pstrCommonMessage[2028]
#define	dMSG_PLEASE_WRITE_OUPUT_MESSAGE_WITH_SOUND_OF_LEADERS_BELL		g_pstrCommonMessage[2029]
#define	dMSG_DO_YOUR_WANT_A_DISPLAY_YOUR_NAME_WITH_SOUND_OF_LEADERS_BELL	g_pstrCommonMessage[2030]

#define	dMSG_PLEASE_CHECK_DOWN_CONTENTS									g_pstrCommonMessage[2031]
#define	dMSG_DO_YOU_WANT_USE_ITEM										g_pstrCommonMessage[2032]
#define	dMSG_CHARACTER_NAME												g_pstrCommonMessage[2033]
#define	dMSG_DISPLAY													g_pstrCommonMessage[2034]
#define	dMSG_NOT_DISPLAY												g_pstrCommonMessage[2035]

#define	dMSG_DISPLAY_MESSAGE_FORM										g_pstrCommonMessage[2036]
#define	dMSG_INCORRECT_MESSAGE_LENGTH									g_pstrCommonMessage[2037]
#define	dMSG_REQUIRE_MORE_TIME_FOR_USE_SOUND_OF_LEADERS_BELL_FORM		g_pstrCommonMessage[2038]
#define	dMSG_USE_ITEM_WITH_ACTOR_NAME_FORM								g_pstrCommonMessage[2039]
#define	dMSG_REMAIN_TIME_HOUR_MINUTE_FORM								g_pstrCommonMessage[2040]

#define	dMSG_REMAIN_TIME_MINUTE_FORM									g_pstrCommonMessage[2041]
#define	dMSG_REBIRTH_COUNT_FORM											g_pstrCommonMessage[2042]
#define	dMSG_LIMIT_OF_SEAL_SPHERE_OF_DAWN								g_pstrCommonMessage[2043]
#define	dMSG_ALREADY_OWN_THAT_EFFECT_ITEM								g_pstrCommonMessage[2044]
#define	dMSG_SUCCESS_TO_USE_ITEM										g_pstrCommonMessage[2045]

#define	dMSG_REBIRTH_TOOLTIP_FORM										g_pstrCommonMessage[2046]
#define	dMSG_REBIRTH_EXP_PENELTY_FORM									g_pstrCommonMessage[2047]
#define	dMSG_SUCCESS_TO_USE_SEAL_SPHERE_OF_DAWN							g_pstrCommonMessage[2048]
#define	dMSG_FAILE_BY_TARGET_ITEMS_LEVEL								g_pstrCommonMessage[2049]
#define	dMSG_EXLCUSIVE_TO_REBIRTH_CHARACTER_ITEM_FORM					g_pstrCommonMessage[2050]

#define	dMSG_PLEASE_SELECT_ONLY_ONE_MINIPET								g_pstrCommonMessage[2051]
#define	dMSG_PLEASE_SELECT_MANY_MINIPET_WITH_SHIFT						g_pstrCommonMessage[2052]
#define	dMSG_PLEASE_DISACTIVE_SELECTED_MINIPET							g_pstrCommonMessage[2053]
#define	dMSG_CONFIRM_MIX_MINIPET										g_pstrCommonMessage[2054]
#define	dMSG_CREATE_MINIPET_WITH_MIXER_FORM								g_pstrCommonMessage[2055]

#define	dMSG_PLEASE_SELECT_MINIPET_AND_RETRY							g_pstrCommonMessage[2056]
#define	dMSG_PLEASE_DISACTIVE_SELECTED_MINIPET_FOR_RESTRAINT			g_pstrCommonMessage[2057]
#define	dMSG_ARE_YOU_REALLY_RESTRAINT_MINIPET_FORM						g_pstrCommonMessage[2058]
#define	dMSG_CAN_NOT_RESTRAINT_MORE_MINIPET_WITH_THAT_KIND_FORM			g_pstrCommonMessage[2059]
#define	dMSG_ABSORB_MINIPET_AND_FEEL_SO_POWERFUL_FORM					g_pstrCommonMessage[2060]

#define	dMSG_MINIPET_RESTRAINT_COUNT_FOR_TT_FORM						g_pstrCommonMessage[2061]
#define	dMSG_ALREADY_UNDER_EFFECT_SOUND_OF_LEADERS_BELL_FORM			g_pstrCommonMessage[2062]
#define	dMSG_UNDER_EFFECT_TWO_MOUTH_ONE_SOUND_EVENT						g_pstrCommonMessage[2063]
#define	dMSG_UNDER_EFFECT_GOLD_RUSH_EVENT								g_pstrCommonMessage[2064]
#define	dMSG_USELESS_STATUS_TO_USE_ITEM									g_pstrCommonMessage[2065]


#define	dMSG_COUNT_OVER_TO_TRY_SECRET_DUNGEON							g_pstrCommonMessage[2066]
#define	dMSG_TOO_MAY_PARTY_TRY_TGUS_SECRET_DUNGEON						g_pstrCommonMessage[2067]
#define	dMSG_FAILED_TO_TRY_SECRET_DUNGEON								g_pstrCommonMessage[2068]
#define	dMSG_TO_TRY_SECRET_DUNGEON_GATHER_PARTY_MEMBER_IN_SAME_FIELD	g_pstrCommonMessage[2069]
#define	dMSG_TOO_MANY_BANK_TRANSACTION_WITH_REBIRTH						g_pstrCommonMessage[2070]

#define	dMSG_CAN_NOT_EQUIP_ITEM_WITH_CURRENT_JOB						g_pstrCommonMessage[2071]
#define	dMSG_USE_ITEM_TO_ITEM_FORM										g_pstrCommonMessage[2072]
#define	dMSG_WARNING_FOR_SEAL_SHPERE_OF_DAWN							g_pstrCommonMessage[2073]
#define	dMSG_DO_YOU_WANT_USE_THIS_ITEM									g_pstrCommonMessage[2074]
#define	dMSG_CAN_NOT_WEAR_ANYMORE										g_pstrCommonMessage[2075]		// 09.08.12

#define	dMSG_WEAR_LIMIT													g_pstrCommonMessage[2076]		// 09.08.12
#define	dMSG_ENG_GOLD													g_pstrCommonMessage[2077]		// Gold			09.08.18
#define	dMSG_GOLD_BAR													g_pstrCommonMessage[2078]		// 금괴			09.08.18
#define	dMSG_BUY_GOLD_BAR												g_pstrCommonMessage[2079]		// 금괴 판매내용09.08.19
#define	dMSG_BUY_GOLD_BAR_ERR											g_pstrCommonMessage[2080]		// 금괴 판매내용 에러09.08.19

#define	dMSG_SELL_ITEM_GOLD_OVER										g_pstrCommonMessage[2081]		// 금액 초과	09.08.19
#define dMSG_ARE_YOU_REALLY_BUY_SELECT_ITEM_FORM_GOLD_BAR				g_pstrCommonMessage[2082]		// 금괴로 사는 매세지~?09.08.19	
#define dMSG_PITCHMAN_SHOP_ITEM_SOLD_FORM_GOLD_BAR						g_pstrCommonMessage[2083]		// 금괴 판매 폼09.08.19	
#define dMSG_PITCHMAN_SHOP_ITEM_BUY_FORM_GOLD_BAR						g_pstrCommonMessage[2084]		// 금괴 구입 폼09.08.21	
#define dMSG_INSUFFICIENT_GOLD_BAR_TO_BY_PITCHMAN_SHOP_ITEM				g_pstrCommonMessage[2085]		// 금괴 부족09.08.21	

#define dMSG_PITCHMAN_SHOP_OWNER_NOT_SLOT								g_pstrCommonMessage[2086]		// 판매자 슬롯부족09.08.21	
#define dMSG_UPGRADE													g_pstrCommonMessage[2087]		// 강화				09.08.27
#define dMSG_SUCCESS_UPGRADE											g_pstrCommonMessage[2088]		// 강화성공!!		09.08.27
#define dMSG_FAILED_UPGRADE												g_pstrCommonMessage[2089]		// 강화실패.ㅠ.ㅠ	09.08.27
#define dMSG_NOT_MATERIALS												g_pstrCommonMessage[2090]		// 재료가..부족해요~ 09.08.27

#define dMSG_ITEM_STONEOFMYSTERY										g_pstrCommonMessage[2091]		// 신비석~ 09.08.27
#define dMSG_ITEM_TANTILLESS											g_pstrCommonMessage[2092]		// 탄틸리스 유물~ 09.08.27
#define dMSG_ITEM_CRYSTAL												g_pstrCommonMessage[2093]		// 결정석~ 09.08.27
#define dMSG_REALLY_UPGRADE_FORM										g_pstrCommonMessage[2094]		// 	"※ 강화 재료\n%s\n위의 재료를 소비하여 강화합니다.\n\n※ 실패시 패널티\n\n%d로 레벨이 하락합니다. 정말로 강화 하시겠습니까?", 09.08.27
#define dMSG_COUNT														g_pstrCommonMessage[2095]		// 개 09.09.01

#define dMSG_UPGRADE_TARGET_WEAPON_ITEM									g_pstrCommonMessage[2096]		// 강화할 아이템 09.09.01
#define dMSG_UPGRADE_METERIALS_ITEM										g_pstrCommonMessage[2097]		// 강화제 09.09.01
#define dMSG_UPGRADE_SUCCESS_ITEM										g_pstrCommonMessage[2098]		// 강화 성공아이템 09.09.01
#define dMSG_UPGRADE_COSMIC_POWER_ITEM									g_pstrCommonMessage[2099]		// 코스믹파워 09.09.01
#define dMSG_UPGRADE_NOT_EFFECT											g_pstrCommonMessage[2100]		// 효과없음 09.09.01

#define dMSG_UPGRADE_SUCCESS_CHANCE_UP									g_pstrCommonMessage[2101]		// 강화 성공확률상승 09.09.01
#define dMSG_UPGRADE_NOT_PENALTY										g_pstrCommonMessage[2102]		// 강화 패널티 없음 09.09.01
#define dMSG_REGISTRATION_UPGRADE_ITEM									g_pstrCommonMessage[2103]		// "이터널 웨폰즈를 등록하는 슬롯입니다.",09.09.02
#define dMSG_REGISTRATION_UPGRADE_MATERIALS_ITEM						g_pstrCommonMessage[2104]		// "일반 보조제를 등록하는 슬롯입니다.", 09.09.02
#define dMSG_REGISTRATION_UPGRADE_COSMIC_POWER_ITEM						g_pstrCommonMessage[2105]		// "코스믹 아이템이 자동 등록되는 슬롯으로 소지하고 있지 않으면 등록되지 않습니다."09.09.02

#define dMSG_REGISTRATION_UPGRADE_NEXT_LEVEL_ITEM						g_pstrCommonMessage[2106]		// "강화에 성공시 얻을 수 있는 아이템을 보여 줍니다." 09.09.02
#define dMSG_LARGE_PLUS													g_pstrCommonMessage[2107]		// "╋",	 09.09.02
#define dMSG_AN_INVERTED_TRIANGLE										g_pstrCommonMessage[2108]		// "▼", 09.09.02
#define dMSG_KOR_GOLD													g_pstrCommonMessage[2109]		// "골드", 09.09.02
#define dMSG_REGISTRATION_UPGRADE_PREMIUM_MATERIALS_ITEM				g_pstrCommonMessage[2110]		// "프리미엄 보조제를 등록하는 슬롯입니다.", 09.09.03

#define dMSG_UPGRADE_SUCCESS_FORM										g_pstrCommonMessage[2111]		// 	강화되었습니다.", 09.09.03
#define dMSG_UPGRADE_FAILED_FORM										g_pstrCommonMessage[2112]		// 하락되었습니다.", 09.09.03
#define dMSG_RIGHT_ARROW												g_pstrCommonMessage[2113]		//→ , 09.09.03
#define dMSG_COATING_ITEM_EVAPORATION									g_pstrCommonMessage[2114]		//"코팅제가 증발하였습니다.", , 09.09.03
#define dMSG_COATING_ITEM_EFFECT_GENERATION								g_pstrCommonMessage[2115]		//"코팅제의 효과로 레벨이 보존되었습니다.",, 09.09.03

#define dMSG_ITEM														g_pstrCommonMessage[2116]		//"아이템",, 09.09.03
#define dMSG_UPGRADE_SUCCESS											g_pstrCommonMessage[2117]		//강화 성공, 09.09.03
#define dMSG_UPGRADE_FAILED												g_pstrCommonMessage[2118]		// 강화 실패, 09.09.03
#define dMSG_ITEM_NOT_USE_PET											g_pstrCommonMessage[2119]		// 펫에서 사용할 수 없는 아이템입니다., 09.09.03
#define dMSG_REALLY_DISJOINTING_ITEM									g_pstrCommonMessage[2120]		// 정말로 분해 하시겠습니까?., 09.09.03

#define dMSG_RECEIVE_ITEM_COUNT_BY_QUEST_FORM							g_pstrCommonMessage[2121]		// 다음아이템 %d개를 얻었습니다. %s 09.09.07
#define dMSG_COATING_ITEM_EFFECT_GENERATION_LEVEL_DOWN					g_pstrCommonMessage[2122]		// 코팅제의 효과로 1레벨 하락되었습니다.. %s 09.09.08
#define dMSG_PERCENT													g_pstrCommonMessage[2123]		// ％ 09.09.09
#define dMSG_EXP_PENALTY												g_pstrCommonMessage[2124]		//	경험치 페널티 	09.09.28 
#define	dMSG_ETERNAL_UPGARDE_MAX_SUCCESS_FORM							g_pstrCommonMessage[2125]		//	%s님이 이터널 10단계 강화에 성공하셨습니다. 	

#define	dMSG_PLEASE_INPUT_CODE											g_pstrCommonMessage[2126]		//	"코드를 입력해 주세요",
#define	dMSG_DO_YOU_WANT_ASK_MOTP_SYSTEM								g_pstrCommonMessage[2127]		//	"더욱 안전한 원타임 패스워드의 사용을 추천 합니다.\n\n지금 사용 신청을 하시겠습니까?",
#define dMSG_USR_NOT_NEED_STATE											g_pstrCommonMessage[2128]		//	"스킬이 사용 가능한 상태가 아닙니다.",			
#define dMSG_USR_BINDING_WORDS											g_pstrCommonMessage[2129]		//	"언어구속상태입니다.",	
#define dMSG_NOT_USE_REACTION_SKILL_TIME								g_pstrCommonMessage[2130]		//	"리액션스킬을 사용할 수 없는 상태입니다.",	

#define	dMSG_EE_REDUCE_CP_FORM											g_pstrCommonMessage[2131]		//	"- <c:LTYELLOW>%d<n>초당 소모 CP <c:LTYELLOW>%d<n>",
#define	dMSG_IS_NOT_OWN_ITEM											g_pstrCommonMessage[2132]		//	"해당 직업으로 그 아이템을 찾을 수 없습니다.",
#define	dMSG_EE_DRAIN_CP_FORM											g_pstrCommonMessage[2133]		//	"- 탈취 CP <c:LTYELLOW>%d<n>\n",
#define	dMSG_EE_SOUL_BURN_FORM											g_pstrCommonMessage[2134]		//	"- 타겟이 마인드 디지즈에 걸리면 일정시간 동안 <c:LTYELLOW>%d<n>초마다 <c:RED>불꽃 대미지<n>를 입힘.\n",
#define	dMSG_EE_RAGE_SOUL_FORM											g_pstrCommonMessage[2135]		//	"- %d확률로 %f~%f의 <c:RED>불꽃 대미지<n>를 입힘.\n",

#define	dMSG_TARGET_NAME												g_pstrCommonMessage[2136]		//	"<target_name>",
#define	dMSG_USE_SHOP													g_pstrCommonMessage[2137]		//	"상점 이용하기",
#define	dMSG_ASK_USE_SHOP												g_pstrCommonMessage[2138]		//	"<c:LTYELLOW>상점<n> 을 이용하시겠습니까?",
#define	dMSG_USE_BANK													g_pstrCommonMessage[2139]		//	"은행 이용하기",
#define	dMSG_ASK_USE_BANK												g_pstrCommonMessage[2140]		//	"<c:LTYELLOW>은행<n> 을 이용하시겠습니까?",

#define	dMSG_TARGET_GOLD_BAR_FULL										g_pstrCommonMessage[2141]		//	"상대방이 금괴를 너무 많이 가지고 있습니다.",
#define dMSG_CHANGE_VIEW_TYPE											g_pstrCommonMessage[2142]		//	"보기 화면 변경",
#define	dMSG_EXIST_SAME_PREFIX											g_pstrCommonMessage[2143]		//		"같은 접두사는 붙일 수 없습니다."
#define	dMSG_COPY														g_pstrCommonMessage[2144]		//		"복사",
#define	dMSG_CUT														g_pstrCommonMessage[2145]		//		"잘라내기",

#define	dMSG_COPY_PREFIX_LIST_FORM										g_pstrCommonMessage[2146]		//	"<c:LTGREEN>%s<n>\n- %s\n- 복사 후 착용 레벨 <c:LTYELLOW>%d<n>",
#define	dMSG_CONFIRM_COPY_PREFIX_FORM									g_pstrCommonMessage[2147]		//	"<c:LTGREEN>%s<n>\n- %s\n\n- 복사 후 착용 레벨 <c:LTYELLOW>%d<n>\n\n이 마법 옵션을 복사 하시겠습니까?",
#define	dMSG_CUT_PREFIX_LIST_FORM										g_pstrCommonMessage[2148]		//	"<c:LTGREEN>%s<n>\n- %s\n- 잘라내기 후 착용 레벨 <c:LTYELLOW>%d<n>",
#define	dMSG_CONFIRM_CUT_PREFIX_FORM									g_pstrCommonMessage[2149]		//	"<c:LTGREEN>%s<n>\n- %s\n\n- 잘라내기 후 착용 레벨 <c:LTYELLOW>%d<n>\n\n이 마법 옵션을 잘라내시겠습니까?",
#define	dMSG_PASSIVE_EFFECT												g_pstrCommonMessage[2150]		//	"<지속효과>",

#define	dMSG_INCREASE_CONCENTRATION_FORM								g_pstrCommonMessage[2151]		//	"집중력 <c:LTYELLOW>%d％<n> 증가",
#define	dMSG_INCREASE_HARD_BLOW_FORM									g_pstrCommonMessage[2152]		//	"강타확률 <c:LTYELLOW>%.2f％<n> 증가",
#define	dMSG_MINIPET_SOUND_OFF											g_pstrCommonMessage[2153]		//	"미니펫 효과음 끄기",
#define	dMSG_INCREASE_MAX_HP_FORM										g_pstrCommonMessage[2154]		//	"- 최대체력 <c:LTYELLOW>%d<n> 증가",
#define	dMSG_INCREASE_STRANGE_STATUS_RESISTANCE_FORM					g_pstrCommonMessage[2155]		//	"- 상태이상저항 <c:LTYELLOW>%d％<n> 증가",

#define	dMSG_INCREASE_RANGE_FORM										g_pstrCommonMessage[2156]		//	"- 사정거리 <c:LTYELLOW>%d<n> 증가",
#define	dMSG_INCREASE_MAGIC_RESISTANCE_FORM								g_pstrCommonMessage[2157]		//	"- 마법저항 <c:LTYELLOW>%d<n> 증가",
#define	dMSG_SHAKE_OFF													g_pstrCommonMessage[2158]		//	"화면 흔들림 끄기",.
#define	dMSG_BE_MANY_CHARACTER_IN_FIELD									g_pstrCommonMessage[2159]		//	"현재 필드에 몬스터가 너무 많이 있습니다. 잠시 후에 다시 사용해 주십시오."
#define	dMSG_CARVING_MATERIALS											g_pstrCommonMessage[2160]		// 	"※ 각인 재료"

#define	dMSG_SUCCESS_CHANCE												g_pstrCommonMessage[2161]		// 		"성공 확률 %d％",
#define	dMSG_REALLY_IS_CARVE											g_pstrCommonMessage[2162]		// 		"정말로 각인하시겠습니까?",
#define	dMSG_CARVING_SUCCESS											g_pstrCommonMessage[2163]		// 		"각인에 성공하셨습니다.",
#define	dMSG_CARVING_FAILED												g_pstrCommonMessage[2164]		// 		"각인에 실패하셨습니다.",
#define	dMSG_HERR														g_pstrCommonMessage[2165]		// 		"님이",

#define	dMSG_PLEASE_TO_TARGET											g_pstrCommonMessage[2166]		// 			"대상을 선택해 주세요.",
#define	dMSG_FEE_FORM_ADD_ABSOLUTE_ATTACK_POWER							g_pstrCommonMessage[2167]		// 			"<c:LTYELLOW>%d<n> 초간 최종 공격력 <c:LTYELLOW>%d<n> 증가",
#define	dMSG_FEE_FORM_INCREASE_ATTACK_COUNT								g_pstrCommonMessage[2168]		// 			"<c:LTYELLOW>%d<n> 초간 공격횟수 <c:LTYELLOW>%d<n> 증가",
#define	dMSG_FEE_FORM_INCREASE_GET_CP									g_pstrCommonMessage[2169]		// 			"<c:LTYELLOW>%d<n> 초간 획득 CP <c:LTYELLOW>%d<n> 증가",
#define	dMSG_FEE_FORM_INCREASE_ABSOLUTE_CRITICAL_PERCENT				g_pstrCommonMessage[2170]		// 			"<c:LTYELLOW>%d<n> 초간 절대 크리티컬 <c:LTYELLOW>%d％<n> 증가",

#define	dMSG_FEE_FORM_FEE_DECREASE_COOL_TIME							g_pstrCommonMessage[2171]		// 			"<c:LTYELLOW>%d<n> 초간 쿨타임 <c:LTYELLOW>%d<n> 초 감소",
#define	dMSG_FORM_FEED_EFFECT_ITEM_TO_MINIPET							g_pstrCommonMessage[2172]		// 			"성장 형태 : <c:LTYELLOW>%s<n>\n경험치 : <c:LTYELLOW>%d<n> 만\n\n 대상 미니펫 [<c:LTYELLOW>%s<n>]에게 사용하시겠습니까?",
#define	dMSG_FORM_RESET_MINIPET_SKILL									g_pstrCommonMessage[2173]		// 			"대상 미니펫 [<c:LTYELLOW>%s<n>]의 스킬을 초기화시키겠습니까?",
#define	dMSG_RESET_MINIPET_SKILL										g_pstrCommonMessage[2174]		// 			"미니펫 스킬이 초기화되었습니다.",
#define	dMSG_CHANGE_MINIPET_TYPE										g_pstrCommonMessage[2175]		// 	"대상 미니펫 [<c:LTYELLOW>%s<n>]의 성장형태를 [<c:LTYELLOW>%s<n>] 100％로 변경하시겠습니까?\n※ 미니펫 스킬도 초기화됩니다.",

#define	dMSG_CAN_NOT_USE_TO_MINIPET_HAVE_NOT_EXP						g_pstrCommonMessage[2176]		// 			"먹이를 한 번도 먹이지 않은 미니펫에게는 사용할 수 없습니다.",
#define	dMSG_SHOW_DROP_ITEM												g_pstrCommonMessage[2177]		// 			"드랍 아이템 보기",
#define	dMSG_CAN_DO_NOT_PASSIVE_SKILL									g_pstrCommonMessage[2178]		// 			"패시브 스킬은 등록할 수 없습니다.",
#define	dMSG_EE_FORM_IGNORE_BLOCKING									g_pstrCommonMessage[2179]		// 			"- 초간 블럭 무시 <c:LTYELLOW>%d％<n>",
#define	dMSG_EE_FORM_ATTACK_DOPPELGANGER								g_pstrCommonMessage[2180]		// 			//	"- 분신이 <c:LTYELLOW>%.2f<n> 초마다 현재 공격력의 <c:LTYELLOW>%d％<n>의 공격력으로 <c:LTYELLOW>%d<n> 회 공격",

#define	dMSG_NORMAL_INVENTORY											g_pstrCommonMessage[2181]		// 			"일반 인벤토리",
#define	dMSG_PREMIUM_INVENTORY											g_pstrCommonMessage[2182]		// 			"프리미엄 인벤토리",
#define	dMSG_MOVE_ITEM_TO_NORMAL_INVENTORY								g_pstrCommonMessage[2183]		// 			"기본 인벤토리로 이동",
#define	dMSG_MOVE_ITEM_TO_PREMIUM_INVENTORY								g_pstrCommonMessage[2184]		// 			"유료아이템 전용 이벤토리로 이동",
#define	dMSG_HAVE_NOT_EMPTY_NORMAL_INVENTORY_SLOT						g_pstrCommonMessage[2185]		// 			"기본 인벤토리에 빈 공간이 없습니다.",

#define	dMSG_HAVE_NOT_EMPTY_PREMIUM_INVENTORY_SLOT						g_pstrCommonMessage[2186]		// 			"유료 아이템 전용 인벤토리에 빈 공간이 없습니다.",
#define	dMSG_AMPLIFICATION												g_pstrCommonMessage[2187]		// 			"증폭"
#define	dMSG_REGISTER_AMPLIFYING_ITEM									g_pstrCommonMessage[2188]		//			"증폭 할 아이템을 등록해 주세요",
#define	dMSG_REGISTER_SUBSTONE											g_pstrCommonMessage[2189]		//			"보조석 아이템을 등록해 주세요",
#define	dMSG_PERSCOM_GUARD_FAILED										g_pstrCommonMessage[2190]		// 		"퍼스컴 가드 등록 유저입니다.\n\n현재 PC는 등록되어 있지 않은 PC 입니다.",

#define	dMSG_EXTRA_BAG_SIZE_FULL										g_pstrCommonMessage[2191]		//	"더 이상 인벤토리 크기를 확장 시킬 수 없습니다.",
#define	dMSG_TRANS_AVATAR_TO_GVG_SERVER									g_pstrCommonMessage[2192]		//	GVG 서버로 캐릭터 전송
#define	dMSG_ARE_YOU_REALLY_TRANS_AVATAR_TO_GVG_SERVER					g_pstrCommonMessage[2193]
#define	dMSG_TRANS_GUILD_TO_GVG_SERVER									g_pstrCommonMessage[2194]		//	길드 정보 GVG 서버에 전송
#define	dMSG_ARE_YOU_REALLY_TRANS_GUILD_TO_GVG_SERVER					g_pstrCommonMessage[2195]

#define	dMSG_TRANS_AVATAR_TO_GVG_SERVER_RULE							g_pstrCommonMessage[2196]		//	"캐릭터 전송은 한달에 한번만 가능합니다.\n\n마지막 전송일 %4d/%.2d/%.2d",
#define	dMSG_TRANS_GUILD_TO_GVG_SERVER_RULE								g_pstrCommonMessage[2197]		//	"길드 전송은 한달에 한번만 가능합니다.\n\n마지막 전송일 %4d/%.2d/%.2d",
#define	dMSG_COMPLETE_TRANS_AVATAR_TO_GVG_SERVER						g_pstrCommonMessage[2198]		//	"캐릭터 정보의 전송이 완료되었습니다.\n\n전송된 캐릭터 이름 <c:LTGREEN>%s<n>",
#define	dMSG_COMPLETE_TRANS_GUILD_TO_GVG_SERVER							g_pstrCommonMessage[2199]		//	"길드 정보의 전송이 완료되었습니다.\n\n전송된 길드 이름 <c:LTGREEN>%s<n>",
#define	dMSG_TRANS_AVATAR_FAILED										g_pstrCommonMessage[2200]		//	"길드 정보의 전송이 완료되었습니다.\n\n전송된 길드 이름 <c:LTGREEN>%s<n>",

#define	dMSG_TRANS_GUILD_FAILED											g_pstrCommonMessage[2201]		//	"길드 정보의 전송이 완료되었습니다.\n\n전송된 길드 이름 <c:LTGREEN>%s<n>",
#define	dMSG_TRAN_GUILD_TO_GVG_RESULT_FAILED_BY_MISMATCH_LEVEL			g_pstrCommonMessage[2202]		//	
#define	dMSG_CAN_NOT_OPEN_PITCHMAN_SHOP_THIS_FIELD						g_pstrCommonMessage[2203]		//	"현재 필드에서는 노점상을 열 수 없습니다.",
#define	dMSG_WAIT_FOR_TRANS_AVATAR_RESULT								g_pstrCommonMessage[2204]		//	"캐릭터 전송 결과를 기다리는 중입니다.",
#define	dMSG_WAIT_FOR_TRANS_GUILD_RESULT								g_pstrCommonMessage[2205]		//	"길드 전송 결과를 기다리는 중입니다.",

#define	dMSG_CAN_NOT_TRANS_GUILD_IN_GVG_SERVER							g_pstrCommonMessage[2206]		//	"길드 전송을 위해서는 먼저 아바타를 전송해 GVG서버에서 길드를 생성해야 합니다.\n자세한 방법은 홈페이지를 참고해 주세요.",
#define	dMSG_ARE_YOU_MAKE_GVG_SERVER_GUILD								g_pstrCommonMessage[2207]		//	"길드 생성 준비가 되어 있습니다.\n길드명 [%s]\n\n지금 길드를 생성 하시겠습니까?"
#define dMSG_CARROTSHOP_ITEMBUY											g_pstrCommonMessage[2208]		//   "아이템 구매"
#define dMSG_CARROTSHOP_SHOPPINGBASKET									g_pstrCommonMessage[2209]		// 	 "장바구니"
#define dMSG_CARROTSHOP_BUYLIST											g_pstrCommonMessage[2210]		// 	 "구매내역"

#define dMSG_CARROTSHOP_CHARGE											g_pstrCommonMessage[2211]		// 	 "당근 충전소"
#define dMSG_CAN_HAVE_BEDGE_UNTIL_THREE									g_pstrCommonMessage[2212]		//	"배지류 아이템은 동시에 <c:LTYELLOW>세 개<n>만 소지할 수 있습니다. (편리성 배지 아이템 제외)",
#define dMSG_CAN_NOT_FOUND_TRANS_GUILD_FOR_GVG							g_pstrCommonMessage[2213]		//	"길드 마스터가 아직 [<c:LTYELLOW>%s<n>] 길드를 전송 하지 않았습니다.\n\n길드 마스터에게 문의해 주세요.",
#define dMSG_CAN_NOT_TRAS_TO_GVG_WITH_NOT_OWNED_GUILD					g_pstrCommonMessage[2214]		//	"길드에 가입된 상태에서만 전송이 가능합니다.",
#define dMSG_TRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_ALREADY_EXIST_DIFF_AVATAR	g_pstrCommonMessage[2215]		//	"이미 G.V.G 서버에 다른 캐릭터가 존재 합니다.\n새로 전송을 하시려면 G.V.G 서버에 기존 캐릭터를 삭제 하셔야 합니다.",

#define dMSG_CARROTSHOP_BUY												g_pstrCommonMessage[2216]		// 	 "구입"
#define dMSG_CARROTSHOP_PRESENT											g_pstrCommonMessage[2217]		// 	 "선물"
#define dMSG_CARROTSHOP_WISHLIST										g_pstrCommonMessage[2218]		// 	 "위시 리스트"
#define dMSG_CARROTSHOP_WISHLIST_DELETE									g_pstrCommonMessage[2219]		// 	 "모두 지우기"
#define dMSG_CARROTSHOP_REMAINCARROT									g_pstrCommonMessage[2220]		// 	 "현재 보유 당근 개수"

#define dMSG_CARROTSHOP_BILLCARROT										g_pstrCommonMessage[2221]		// 	 "결제 필요 당근 개수"
#define dMSG_CARROTSHOP_RECOMMENDSEARCH									g_pstrCommonMessage[2222]		// 	 "추천 검색!"
#define dMSG_READY														g_pstrCommonMessage[2223]		// 	 "       준비중입니다."
#define dMSG_CARROTSHOP_WISHLIST_FULL									g_pstrCommonMessage[2224]		//	 "위시리스트가 가득차서 더이상 담을 수 없습니다.",
#define dMSG_CARROTSHOP_CATEGORY_ALL									g_pstrCommonMessage[2225]		//	 "전체",

#define dMSG_CARROTSHOP_CATEGORY_HOT									g_pstrCommonMessage[2226]		//	 "신규",
#define dMSG_CARROTSHOP_BUY_SHORT_MENT									g_pstrCommonMessage[2227]		//	 "<c:LTYELLOW>%s %d개<n>\n"
#define dMSG_CARROTSHOP_BUY_LONG_MENT									g_pstrCommonMessage[2228]		//	 "%s \n위의 아이템들을 <c:LTYELLOW>당근 %d개<n>로 구입합니다. 한 번 구입한 아이템은 취소할 수 없습니다. 구입하시겠습니까?"
#define dMSG_USE_NEW_CARROT_SHOP										g_pstrCommonMessage[2229]		//	 "신규 당근상점 사용하기"
#define dMSG_WAIT_FOR_SYNC_GVG_AVATAR_DATA								g_pstrCommonMessage[2230]		//	 "캐릭터 정보와 동기화 중입니다.",

#define dMSG_COMPLETE_SYNC_GVG_AVATAR_DATA								g_pstrCommonMessage[2231]		//	 "캐릭터 정보와 동기화가 완료 되었습니다.",
#define dMSG_SYNC_GVG_AVATAR_DATA										g_pstrCommonMessage[2232]		//	 "캐릭터 정보를 동기화 시킵니다.",
#define dMSG_CAN_NOT_FIND_ORG_AVATAR									g_pstrCommonMessage[2233]		//	 "본래 캐릭터를 찾을 수 없습니다.",
#define dMSG_FAILED_SYNC_GVG_AVATAR_BY_INCORRECT_SERVER_STATUS_FORM		g_pstrCommonMessage[2234]		//	 "일부 서버에 문제가 있어 캐릭터를 동기화 시킬 수 없습니다.",
#define dMSG_CORRECT_LIGHT_MIN_DAMAGE_FORM								g_pstrCommonMessage[2235]		//	 "- 빛 최대 대미지<c:LTYELLOW>%d％<n>가 최소대미지로 보정"

#define dMSG_ADD_BIT_CHANCE_FORM										g_pstrCommonMessage[2236]		//	 "- 비트 부착 <c:LTYELLOW>%d％<n> 증가",
#define dMSG_ADD_BIT_CHANCE_WHEN_BE_HIT_FORM							g_pstrCommonMessage[2237]		//	 "- 피격시 공격자에게 <c:LTYELLOW>%d％<n> 로 비트 부착",
#define dMSG_INCREASE_HIT_RANGE_FORM									g_pstrCommonMessage[2238]		//	 "- 범위 스킬의 타격 범위 <c:LTYELLOW>%d<n> 증가",
#define dMSG_EE_FORM_ADD_BIT											g_pstrCommonMessage[2239]		//	"-  <c:LTYELLOW>%d％<n> 확률로 비트 부착",
#define dMSG_CARROTSHOP_NOTYET											g_pstrCommonMessage[2240]		//	 "아직 구현이 되지 않았습니다.\n홈페이지를 이용해서 충전해 주시기 바랍니다."

#define dMSG_CARROTSHOP_WISHLIST_ZERO									g_pstrCommonMessage[2241]		//	 "위시 리스트에 아이템을 추가해야 구입이 가능합니다."
#define dMSG_WORLD_UNLINKED_QUEST										g_pstrCommonMessage[2242]		//   "해당 필드에 관련된 퀘스트가 아닙니다."
#define dMSG_MINIMAP_SHOW_ALL											g_pstrCommonMessage[2243]		//   "모두 보기"
#define dMSG_TARGET_REBIRTH_LEVEL          								g_pstrCommonMessage[2244]		// "<target_rebirth_level>"
#define dMSG_MINIMAP_QUEST_PORTAL_TOOLTIP_REMAIN						g_pstrCommonMessage[2245]		//   "<c:LTORANGE>%s<n>\n  목적지까지 남은 필드 수 : <c:LTORANGE>%d<n>\n"

#define dMSG_MINIMAP_QUEST_PORTAL_TOOLTIP_CORRECT						g_pstrCommonMessage[2246]		//   "<c:LTORANGE>%s<n>\n  퀘스트 진행필드\n"
#define dMSG_SYSTEM_CATEGORY_SCREEN										g_pstrCommonMessage[2247]		//   "화    면"
#define dMSG_SYSTEM_CATEGORY_SOUND										g_pstrCommonMessage[2248]		//   "사 운 드"
#define dMSG_SYSTEM_CATEGORY_COMMUNITY									g_pstrCommonMessage[2249]		//   "커뮤니티"
#define dMSG_QUEST_HISTORY												g_pstrCommonMessage[2250]		// "퀘스트 전체 목록"

#define dMSG_TITLE														g_pstrCommonMessage[2251]		// "칭호"
#define dMSG_SKILL_MASTER												g_pstrCommonMessage[2252]		// "스킬 마스터"
#define dMSG_COMMON														g_pstrCommonMessage[2253]		// "공통"
#define dMSG_DO_NOT_COMPLETE											g_pstrCommonMessage[2254]		// "미완료"
#define dMSG_PROCESS													g_pstrCommonMessage[2255]		// "진행중"

#define dMSG_COMPLETE_RATE												g_pstrCommonMessage[2256]		// "달성률"
#define dMSG_AREA_COMMENT												g_pstrCommonMessage[2257]		// "관련 지역"
#define dMSG_POSSIBLE_LEVEL												g_pstrCommonMessage[2258]		// "수행 가능 레벨"
#define dMSG_ABOVE														g_pstrCommonMessage[2259]		// "이상"
#define dMSG_FILTER														g_pstrCommonMessage[2260]		// "필터"

#define	dMSG_NOT_SELECT													g_pstrCommonMessage[2261]		// "미선택"
#define dMSG_SELECT_COMPLETE											g_pstrCommonMessage[2262]		// "선택완료"
#define	dMSG_FORM_EE_BLEEDING											g_pstrCommonMessage[2263]		//	"- 대상을 <c:LTORANGE>%d<n> 초 동안 스킬 공격력의 <c:LTORANGE>%d％<n> 를 <c:LTORANGE>%d<n> 초 간격으로 입히는 출혈 상태로 만듦(걸릴 확률 <c:LTORANGE>%d％<n>)",
#define	dMSG_FORM_ATTACK_POINT_PER_ACTOR_LEVEL							g_pstrCommonMessage[2264]		//	"- 자신의 레벨에 비례한 공격력 <c:LTORANGE>%d<n> 추가",
#define	dMSG_FORM_EE_ELECTRIC_SHOCK										g_pstrCommonMessage[2265]		//	"- 대상을 <c:LTORANGE>%d<n> 초 동안 감전 상태로 만듦(걸릴 확률 <c:LTORANGE>%d％<n>)",

#define	dMSG_FORM_EE_MAKE_TYPE_UNDEAD									g_pstrCommonMessage[2266]		//	"- 대상을 <c:LTORANGE>%d<n> 초 동안 언데드 속성으로 변환(걸릴 확률 <c:LTORANGE>%d％<n>)",
#define	dMSG_FORM_EE_INSTANCE_HEAL_EFFECT								g_pstrCommonMessage[2267]		//	"- 대상을 <c:LTORANGE>%d<n> 초 동안 받는 회복 스킬의 치유량 중 <c:LTORANGE>%d％<n> 를 즉시 회복할 수 있는 상태로 만듦",
#define	dMSG_FORM_ADD_DAMAGE_PERCENT									g_pstrCommonMessage[2268]		//	"- 명중시 입힌 대미지의 <c:LTORANGE>%d％<n> 추가",
#define	dMSG_CAN_USE_SKILL_WHEN_TRANS_TO_WEAPON							g_pstrCommonMessage[2269]		//	"- 무기변신 중 사용 가능",
#define	dMSG_BODY_EFFECT_OFF											g_pstrCommonMessage[2270]		//	"지속형 이팩트 끄기",

#define	dMSG_STORE_ITEM_TO_CART											g_pstrCommonMessage[2271]
#define	dCARROT_SHOP_ITEM_TERM_WARNING									g_pstrCommonMessage[2272]		//	\n <c:LTYELLOW>구매와 동시에 사용기간이 적용 됩니다.<n>


//GGG Message
#define dMSG_GGG_NAME_START_INDEX									0
#define dMSG_GGG_NAME_FIRE											g_pstrGreateGodGrace[0]
#define dMSG_GGG_NAME_WATER											g_pstrGreateGodGrace[1]
#define dMSG_GGG_NAME_WIND											g_pstrGreateGodGrace[2]
#define dMSG_GGG_NAME_EARTH											g_pstrGreateGodGrace[3]
#define dMSG_GGG_NAME_LIGHT											g_pstrGreateGodGrace[4]
#define dMSG_GGG_NAME_DARK											g_pstrGreateGodGrace[5]

#define dMSG_GGG_COMMENT_START_INDEX								6
#define dMSG_GGG_COMMENT_FIRE										g_pstrGreateGodGrace[6]
#define dMSG_GGG_COMMENT_WATER										g_pstrGreateGodGrace[7]
#define dMSG_GGG_COMMENT_WIND										g_pstrGreateGodGrace[8]
#define dMSG_GGG_COMMENT_EARTH										g_pstrGreateGodGrace[9]
#define dMSG_GGG_EFFECT												g_pstrGreateGodGrace[10]

#define dMSG_GGG_LEVELUP_EFFECT_COMMENT_VALUE1_FORM					g_pstrGreateGodGrace[11]	//속성 공격
#define dMSG_GGG_LEVELUP_EFFECT_COMMENT_VALUE2_FORM					g_pstrGreateGodGrace[12]	//이속 증가
#define dMSG_GGG_LEVELUP_EFFECT_COMMENT_VALUE3_FORM					g_pstrGreateGodGrace[13]	//CP MAX
#define dMSG_GGG_SKILLUP_EFFECT_COMMENT_VALUE1_FORM					g_pstrGreateGodGrace[14]	//속성 공격
#define dMSG_GGG_SKILLUP_EFFECT_COMMENT_VALUE2_FORM					g_pstrGreateGodGrace[15]	//이속 증가

#define dMSG_GGG_LEVELUP_EFFECT_NAME_START_INDEX					16
#define dMSG_GGG_LEVELUP_EFFECT_NAME_FIRE							g_pstrGreateGodGrace[16]	
#define dMSG_GGG_LEVELUP_EFFECT_NAME_WATER							g_pstrGreateGodGrace[17]	
#define dMSG_GGG_LEVELUP_EFFECT_NAME_WIND							g_pstrGreateGodGrace[18]	
#define dMSG_GGG_LEVELUP_EFFECT_NAME_EARTH							g_pstrGreateGodGrace[19]
#define dMSG_GGG_LEVELUP_EFFECT_COMMENT_VALUE1_TOOLTIP_FORM			g_pstrGreateGodGrace[20]

#define dMSG_GGG_ATTACK_SPEED										g_pstrGreateGodGrace[21]
#define dMSG_GGG_REMAIN_TIME										g_pstrGreateGodGrace[22]
#define dMSG_GGG_TOOLTIP_COMMENT									g_pstrGreateGodGrace[23]
#define dMSG_GGG_SKILLUP_EFFECT_NAME_START_INDEX					24
#define dMSG_GGG_SKILLUP_EFFECT_NAME_FIRE							g_pstrGreateGodGrace[24]
#define dMSG_GGG_SKILLUP_EFFECT_NAME_WATER							g_pstrGreateGodGrace[25]

#define dMSG_GGG_SKILLUP_EFFECT_NAME_WIND							g_pstrGreateGodGrace[26]
#define dMSG_GGG_SKILLUP_EFFECT_NAME_EARTH							g_pstrGreateGodGrace[27]
#define dMSG_GET													g_pstrGreateGodGrace[28]
#define dMSG_REMOVE													g_pstrGreateGodGrace[29]
#define dMSG_GGG_TOOLTIP_COMMENT_SKILL								g_pstrGreateGodGrace[30]

#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_FIRE1						g_pstrGreateGodGrace[31]
#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_FIRE2						g_pstrGreateGodGrace[32]
#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_WATER1						g_pstrGreateGodGrace[33]
#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_WATER2						g_pstrGreateGodGrace[34]
#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_WIND1						g_pstrGreateGodGrace[35]

#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_WIND2						g_pstrGreateGodGrace[36]
#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_WIND3						g_pstrGreateGodGrace[37]
#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_EARTH1						g_pstrGreateGodGrace[38]
#define dMSG_GGG_TOOLTIP_COMMENT_SKILL_EARTH2						g_pstrGreateGodGrace[39]



#define	dNOTICE_COUNT		60
#define	dNOTICE_TIME		2000

#define	dMSG_KIND_NORMAL	0
#define	dMSG_KIND_BOLD		1

class	cNOTICE
{
	int				m_iMessageType;
	DWORD			m_dwTime;
	DWORD			m_dwColor;
	char			m_strMessage[64];

	inline	void	setMessage(char *_lpMessage,DWORD _dwColor,int _iType);
	void			update();
	void			put();

public:
	static	CTextDC		*s_lpText;
	static	cNOTICE		s_aMessages[dNOTICE_COUNT];
	static	cNOTICE		s_Notice;

	static	int			s_iCount;

	static	void		SetTextDevice(CTextDC *_lpText)	{s_lpText=_lpText;}
	static	void		Notify(char *_lpMessage,DWORD _dwColor);
	static	BOOL		AddNotify(char *_lpMessage,DWORD _dwColor);
	static	void		Update();
	static	void		Put();
};

//	팝업 상자
#define	dMSGBOX_NOTHING				0
#define	dMSGBOX_BUTTON1				1
#define	dMSGBOX_BUTTON2				2
#define	dMSGBOX_BUTTON3				3
#define	dMSGBOX_RECEIVE_WAIT_PACKET	4
#define	dMAX_MSG_BOX_BUTTON_COUNT	3
//
//	Message Box Type
enum
{
	eMBT_TYPE1,
	eMBT_TYPE2,
};

//
//	Message Box Icon Shape
enum
{
	eMBIS_NONE,
	eMBIS_IMPORTANT,
	eMBIS_WARNING,
};

class	cTextNode
{
public:
	int		x,y;
	WORD	m_wPutMethod;
	DWORD	m_dwTextColor;
	char	*m_pstrText;

	cTextNode()
	{
		m_pstrText	=	NULL;
	}
	~cTextNode()
	{
		pKILL(m_pstrText);
	}

	void	set(int _iX,int _iY,int _iPutMethod,DWORD _dwColor,char *_lpstrText)
	{
		x				=	_iX;
		y				=	_iY;
		m_wPutMethod	=	_iPutMethod;

		pKILL(m_pstrText);

		int	iLen		=	strlen(_lpstrText);

		m_pstrText		=	new char [iLen+1];
		m_dwTextColor	=	_dwColor;

		strcpy(m_pstrText,_lpstrText);
	}
};

class	CMsgBoxInfo
{
public:
	WORD			m_wTextNodeCount;
	WORD			m_wType;

//	버튼 관련
	WORD			m_wNormalButtonImage,m_wActiveButtonImage,m_wPushButtonImage,m_wDisableButtonImage;
	DWORD			m_dwBarTextColor;
	WORD			m_wIsPutBarTextShadow;
	WORD			m_wButtonMargin;

//	메시지 박스 이미지
	WORD			m_wBoxImage;
	WORD			m_wTitleBarImage,m_wBottomBarImage;
	WORD			m_wTitleBarLeftImage,m_wTitleBarRightImage;
	WORD			m_wBottomBarLeftImage,m_wBottomBarRightImage;

//	외곽 프레임
	WORD			m_wLeftTopImage,m_wRightTopImage,m_wLeftBottomImage,m_wRightBottomImage;
	WORD			m_wTopBorderImage,m_wBottomBorderImage,m_wLeftBorderImage,m_wRightBorderImage;
	WORD			m_wPannelColor;

//	아이콘 모양
	WORD			m_wIconImportant,m_wIconWarning;

//	타이틀바 관련 정보
	WORD			m_wTitleFontSize;
	char			m_strTitleFontName[32];
	DWORD			m_dwTitleFontStyle;

//	기타 위치 보정
	WORD			m_wLeftFrameMargin,m_wRightFrameMargin,m_wTopFrameMargin,m_wBottomFrameMargin;
	WORD			m_wTextBoxHorizonMargin,m_wShadowMargin;
	WORD			m_wTitleTextVerticalMargin;
	WORD			m_wBarMenuVerticalMargin;
	WORD			m_wTitleTextShadowMargin;

	void			reset()
	{
		memset(this,0xff,sizeof(CMsgBoxInfo));

		m_wTextNodeCount		=	0;
		m_wTextBoxHorizonMargin	=	0;
		m_wShadowMargin			=	0;
		m_wLeftFrameMargin		=	0;
		m_wRightFrameMargin		=	0;
		m_wTopFrameMargin		=	0;
		m_wBottomFrameMargin	=	0;
		m_wTextBoxHorizonMargin	=	0;
		m_wTitleTextVerticalMargin=	0;
		m_wBarMenuVerticalMargin=	0;
		m_wTitleTextShadowMargin=	1;

		m_wIsPutBarTextShadow	=	FALSE;
	}
};

const	int	c_iMaxTextNodeCount	=	256;

class	CMsgBox : public CMsgBoxInfo
{
	//	박스와 관련된 각종 이미지
	cSPRITE			*m_lpSprite;
	CTooltip		m_ttFrame;
	cTextNode		m_aTextNode[c_iMaxTextNodeCount];

	int				m_iTextNodeCount;

	BOOL			m_isPoped,m_bIsRookie,m_isExclusive,m_isFreeze;	//	떠있냐? 메시지 박스 중앙에 쎄린다.-_-  프리즈 상태:까만 화면에 메시지 박스만 덜렁..
	int				m_iAlignText;

	DWORD			m_dwWaitPacket;				//	기다리는 네트워크 패킷
	int				m_iIconShape;				//	아이콘 모양
	int				m_iResult;					//	결과
	CPos			m_pos,m_posClickedImageMargin;	//	박스의 위치,눌러진 버튼의 마진
	int				m_iBoxWidth,m_iBoxHeight;	//	박스의 크기
	int				m_iButtonCount;				//	버튼의 수
	int				m_iFocusButton,m_iPushButton;	//	마우스가 머물고 있는버튼,누르고 있는 버튼

	CTextDC			*m_lpText,*m_lpCurrentTextDC,m_textTitle;
	CTextDCFontInfo	m_fontInfo;
	cRECT			m_rectOutput,m_rectCurrent,m_rectFrame;

	char			m_strMessage[1024];	//	박스에 메시지
	char			m_strTitle[32];		//	타이틀 메시지
	char			m_strButton[3][12];	//	버튼 메시지
	cBARMENU		m_bmButton;
	
	BOOL			_popup(char *_strTitle,char *_strMessage,char *_strButton1,char *_strButton2,char *_strButton3, int _iButton1Key, int _iButton2Key, int _iButton3Key);

public:
					CMsgBox()
					{
						CMsgBoxInfo::reset();

						m_isPoped				=	FALSE;
						m_isExclusive			=	FALSE;
						m_bIsRookie				=	FALSE;
						m_iButtonCount			=	0;
						m_dwWaitPacket			=	0xffffffff;
						m_lpCurrentTextDC		=	NULL;
						m_iTextNodeCount		=	0;

						m_iResult				=	0xffffffff;
					}


	//	각종 이미지 설정
	BOOL			init(CTextDC *_lpText,cSPRITE *_lpSprite,int _iBox,int _iNormalButton,int _iActiveButton,int _iPushButton,cRECT *_lprectOutput);
	BOOL			init(CTextDC *_lpText,cSPRITE *_lpSprite,CMsgBoxInfo *_lpImageInfo,cRECT *_lprectOutput);
	CTooltip*		getFrame()	{return	&m_ttFrame;}

	bool			addTextNode(int _iX,int _iY,int _iPutMethod,DWORD _dwColor,char *_lpstrText);

	void			setClickedImageMargin(int _iDx,int _iDy){m_posClickedImageMargin.x=_iDx,m_posClickedImageMargin.y=_iDy;}
	void			close();

	int				update(int _iX,int _iY,BYTE *_lpKeyMap,BOOL _isTouchLeftButton,BOOL _isClickLeftButton,int _iClickButton=0xffff);	//	업데이트 한다.

	inline	void	setExclusive(BOOL _is)	{m_isExclusive	=	_is;}
	inline	BOOL	isPoped()		{return	m_isPoped;}	//	떠있냐-_-?
	inline	BOOL	isExclusive()	{return	m_isExclusive;}	//	떠있냐-_-?
	inline	BOOL	isWaitPacket()	{if (m_dwWaitPacket	==	0xffffffff)	return	FALSE;return	TRUE;}	//	기다리는 패킷이 있다.	
	inline	BOOL	isNothing()		{if (m_iResult	==	dMSGBOX_NOTHING)	return	TRUE;return	FALSE;}	//	기다리는 패킷이 있다.	
	inline	DWORD	getWaitPacket()	{return m_dwWaitPacket;}	//	기다리는 패킷
	inline	int		getResult()	{return m_iResult;}				//	결과
	inline	void	setResult(int _iResult)	{m_iResult	=	_iResult;}	//	결과 설정
	inline	void	setFreeze()	{m_isFreeze	=	TRUE;}			//	프리즈 상태 설정
	inline	BOOL	isFreeze()	{return	m_isFreeze;}			//	프리즈 상태 설정

	void			draw();
	void			drawNodeText();
	void			drawFrame(cRECT *_lpRect,BOOL _bIsDrawShadow,char *_lpstrTitle=NULL);


//	팝업 박스 띄움.
	void			setText(char *_strMessage)	{strcpy(m_strMessage,_strMessage);}
	BOOL			cWarning(char *_strTitle,char *_strMessage,char *_strButton1=NULL,char *_strButton2=NULL,char *_strButton3=NULL)
	{
		if	(!cPopup(_strTitle,_strMessage,_strButton1,_strButton2,_strButton3))
			return	FALSE;

		m_iIconShape	=	eMBIS_WARNING;

		return	TRUE;
	}
	BOOL			warning(char *_strTitle,char *_strMessage,char *_strButton1=NULL,char *_strButton2=NULL,char *_strButton3=NULL)
	{
		if	(!_popup(_strTitle,_strMessage,_strButton1,_strButton2,_strButton3,0xffffffff,0xffffffff,0xffffffff))
			return	FALSE;

		m_iIconShape	=	eMBIS_WARNING;

		return	TRUE;
	}
	BOOL			cPopup(char *_strTitle,char *_strMessage,char *_strButton1=NULL,char *_strButton2=NULL,char *_strButton3=NULL, int _iButton1Key = 0xffffffff, int _iButton2Key = 0xffffffff, int _iButton3Key = 0xffffffff);

	void			setTextDC(CTextDC *_lpTextDC)
	{
		m_lpCurrentTextDC	=	_lpTextDC;
	}

	void			setButtonId(int _iId1,int _iId2=0xffffffff,int _iId3=0xffffffff);
	inline	void	setWaitPacket(DWORD _dwWaitPacket){m_dwWaitPacket=_dwWaitPacket;}	//	기다리는 패킷 설정

	inline	void	alignTextByWidthCenter()	{m_iAlignText	=	dFITTEXT_WIDTH_CENTER;}
	inline	void	alignTextByHeightCenter()	{m_iAlignText	=	dFITTEXT_HEIGHT_CENTER;}
	inline	void	alignTextByCenter()			{m_iAlignText	=	dFITTEXT_CENTER;}
	inline	void	alignTextNone()				{m_iAlignText	=	0;}
	inline	void	setIcon(int _iIcon)			{m_iIconShape	=	_iIcon;}
	void			setSize(int _iWidth,int _iHeight);
	inline	cRECT	getrectOutput()	{return m_rectOutput;}
	inline	void	setrectOutput(cRECT* _rect)	{m_rectOutput.set(_rect);}
	inline	void	setrectOutput(int _x1, int _y1, int _x2, int _y2)	{m_rectOutput.set(_x1, _y1, _x2, _y2);}
};

extern	BOOL		CheckFilter(char *_lpstrText,char **_lpFilter,int _iFilterCount);
extern	BOOL		sfcStrstr(char *_lpstrText,char *_lpstrWord);
extern	CMsgBox		g_msgBox;						//	팝업 박스

#endif