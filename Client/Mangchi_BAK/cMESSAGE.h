#ifndef	_classGLOBAL_MESSAGE_H
#define	_classGLOBAL_MESSAGE_H

#include "SFC.H"

#define	dMSG_TOO_MANY_MAP								g_strMessage[0]			//	맵이 너무 많다.
#define	dMSG_TONG_MAP_WRONG_SIZE						g_strMessage[1]			//	통맵 사이즈가 맞지 않는다.
#define	dMSG_WHOLE_AREA									g_strMessage[2]			//	전체 영역
#define	dMSG_TONGMAP_FILE_NAME_VALID					g_strMessage[3]			//	통맵 화일 이름이 비어 있다
#define	dMSG_EXIT_MANGCHI								g_strMessage[4]			//	망치를 끝낸다.
#define	dMSG_DO_YOU_WANT_A_SAVE							g_strMessage[5]			//	세이브 할껴?

#define	dMSG_MAP_LOAD_ERROR								g_strMessage[6]			//	맵을 로딩 하는데 문제가 있습니다.
#define	dMSG_TONG_MAP_LOAD_ERROR						g_strMessage[7]			//	통맵 로딩 하는데 문제가..
#define	dMSG_TONG_MAP_BPP_ERROR							g_strMessage[8]			//	BPP가 안 맞는다.
#define	dMSG_CLOSE_MAP_ERROR							g_strMessage[9]
#define	dMSG_DONT_CLOSE_LAST_MAP						g_strMessage[10]

#define	dMSG_RESET_MAP									g_strMessage[11]		//	맵 정보 재설정
#define	dMSG_WARNING_LOST_DATA							g_strMessage[12]		//	기존의 맵 사이즈보다 작기때문에 일부 정보가 사라질 수 있다!!
#define	dMSG_THERE_ARE_NO_KARMA							g_strMessage[13]		//	기존의 맵 사이즈보다 작기때문에 일부 정보가 사라질 수 있다!!
#define	dMSG_ADD_KARMA_ERROR							g_strMessage[14]
#define	dMSG_KARMA_FULL									g_strMessage[15]

#define	dMSG_ADD_NEW_KARMA								g_strMessage[16]
#define	dMSG_ADD_TRIGGER_ERROR							g_strMessage[17]
#define	dMSG_TRIGGER_FULL								g_strMessage[18]
#define	dMSG_ADD_REACTION_ERROR							g_strMessage[19]
#define	dMSG_REACTION_FULL								g_strMessage[20]

#define	dMSG_ADD_HERO_ERROR								g_strMessage[21]
#define	dMSG_BASE_HERO_JOB_VALID						g_strMessage[22]
#define	dMSG_VALID_JOB_NAME								g_strMessage[23]
#define	dMSG_ADD_HERO									g_strMessage[24]
#define	dMSG_REMOVE_HERO								g_strMessage[25]

#define	dMSG_EDIT_HERO									g_strMessage[26]
#define	dMSG_NEW_PROJECT_ERROR							g_strMessage[27]
#define	dMSG_ERROR_EXIST_PROJECT						g_strMessage[28]
#define	dMSG_ERROR_CAN_NOT_CREATE_PROJECT_FOLDER		g_strMessage[29]
#define	dMSG_ERROR_PROJECT_FOLDER_CHANGED				g_strMessage[30]

#define	dMSG_ERROR_LOAD_PROJECT							g_strMessage[31]
#define	dMSG_ERROR_SAVE_PROJECT							g_strMessage[32]
#define	dMSG_MAP_NAME									g_strMessage[33]
#define	dMSG_MAP_SIZE									g_strMessage[34]
#define	dMSG_MAP_TILE_SET								g_strMessage[35]

#define	dMSG_TONG_MAP									g_strMessage[36]
#define	dMSG_EXIST_FILE									g_strMessage[37]
#define	dMSG_QUESTION_OVERITE							g_strMessage[38]
#define	dMSG_MAP_FILE_NAME								g_strMessage[39]
#define	dMSG_LOAD_MAP									g_strMessage[40]

#define	dMSG_LOAD										g_strMessage[41]
#define	dMSG_NOT_EXIST_FILE								g_strMessage[42]
#define	dMSG_ADD_MAP									g_strMessage[43]
#define	dMSG_LOAD_PROJECT								g_strMessage[44]
#define	dMSG_SAVE_PROJECT								g_strMessage[45]

#define	dMSG_PROJECT_FILE_NAME							g_strMessage[46]
#define	dMSG_REMOVE										g_strMessage[47]
#define	dMSG_REMOVE_FILE								g_strMessage[48]
#define	dMSG_QUESTION_REMOVE_SELECT_FILE				g_strMessage[49]
#define	dMSG_REMOVE_FILE_ERROR							g_strMessage[50]

#define	dMSG_CAN_NOT_REMOVE_SELECT_FILE					g_strMessage[51]
#define	dMSG_PROJECT_BEGIN_KARMA_GROUP					g_strMessage[52]
#define	dMSG_DEFAULT_KARMA_GROUP						g_strMessage[53]
#define	dMSG_ALL_HERO									g_strMessage[54]
#define	dMSG_ALL_PARTY									g_strMessage[55]

#define	dMSG_HERO										g_strMessage[56]
#define	dMSG_ALL_CHARACTER								g_strMessage[57]
#define	dMSG_ALL_MONSTER								g_strMessage[58]
#define	dMSG_SOMETHING									g_strMessage[59]
#define	dMSG_SCREEN_AREA								g_strMessage[60]

#define	dMSG_LACK_KARMA_DATA							g_strMessage[61]
#define	dMSG_ERROR_ADD_VALUE							g_strMessage[62]
#define	dMSG_EXIT_SAME_VALUE							g_strMessage[63]
#define	dMSG_TOO_MANY_VALUE								g_strMessage[64]
#define	dMSG_VALUE_NAME									g_strMessage[65]

#define	dMSG_VALUE_DEFUALT								g_strMessage[66]
#define	dMSG_GLOBAL_KARMA_VALUE							g_strMessage[67]
#define	dMSG_LOCAL_KARMA_VALUE							g_strMessage[68]
#define	dMSG_SET_KARMA_VALUE							g_strMessage[69]
#define	dMSG_GLOBAL_KARMA_VALUE_EXPLAIN					g_strMessage[70]

#define	dMSG_LOCAL_KARMA_VALUE_EXPLAIN					g_strMessage[71]
#define	dMSG_CAN_NOT_READ_VALUE							g_strMessage[72]
#define	dMSG_CAN_NOT_READ_VALUE_NAME					g_strMessage[73]
#define	dMSG_QUEST_LIST									g_strMessage[74]
#define	dMSG_ADD_QUEST_ERROR							g_strMessage[75]

#define	dMSG_VALID_QUEST_NAME							g_strMessage[76]
#define	dMSG_VALID_QUEST_OBJECT							g_strMessage[77]
#define	dMSG_EXIST_SAME_NAME_QUEST						g_strMessage[78]
#define	dMSG_QUEST_FULL									g_strMessage[79]
#define	dMSG_REVISE_QUEST								g_strMessage[80]

#define	dMSG_REVISE_QUEST_CANCEL						g_strMessage[81]
#define	dMSG_REMOVE_AREA								g_strMessage[82]
#define	dMSG_EDIT_AREA									g_strMessage[83]
#define	dMSG_QUESTION_CONTINUE							g_strMessage[84]
#define	dMSG_REMOVE_SELECT_AREA							g_strMessage[85]

#define	dMSG_PORTAL_KIND								g_strMessage[86]
#define	dMSG_CONTENTS									g_strMessage[87]
#define	dMSG_DOOR_STATUS								g_strMessage[88]
#define	dMSG_BOX_STATUS									g_strMessage[89]
#define	dMSG_ITS_NOT_DOOR								g_strMessage[90]

#define	dMSG_OK											g_strMessage[91]
#define	dMSG_CANCEL										g_strMessage[92]
#define	dMSG_GLOBAL_KARMA								g_strMessage[93]
#define	dMSG_LOCAL_KARMA								g_strMessage[94]
#define	dMSG_SET_LOCAL_KARMA_SET						g_strMessage[95]

#define	dMSG_SET_PROJECT_KARMA_SET						g_strMessage[96]
#define	dMSG_KARMA_DETAIL_CONTENTS						g_strMessage[97]
#define	dMSG_KARMA										g_strMessage[98]
#define	dMSG_REMOVE_SELECT_KARMA						g_strMessage[99]
#define	dMSG_COPY										g_strMessage[100]

#define	dMSG_CUT										g_strMessage[101]
#define	dMSG_PASTE										g_strMessage[102]
#define	dMSG_REMOVE_SELECT_REACTION						g_strMessage[103]
#define	dMSG_REMOVE_SELECT_TRIGGER						g_strMessage[104]
#define	dMSG_ERROR_SET_AREA								g_strMessage[105]

#define	dMSG_EXIST_SAME_NAME_AREA						g_strMessage[106]
#define	dMSG_ERROR_ADD_AREA								g_strMessage[107]
#define	dMSG_ERROR_CHANGE_MAP_SETTING					g_strMessage[108]
#define	dMSG_CAN_NOT_CHANGE_SETTING						g_strMessage[109]
#define	dMSG_IS_BRIDGE_MAP								g_strMessage[110]

#define	dMSG_ERROR_LOAD_MAP								g_strMessage[111]
#define	dMSG_ALREADY_OPENED								g_strMessage[112]
#define	dMSG_ALREADY_EXIST_SAME_TILE					g_strMessage[113]
#define	dMSG_ERROR_ADD_NEW_MAP							g_strMessage[114]
#define	dMSG_IS_BRIDGE_MAP_FOR_MAP_WND					g_strMessage[115]

#define	dMSG_SELECT_JOB									g_strMessage[116]
#define	dMSG_EQUAL_HERO_TWO_JOB							g_strMessage[117]
#define	dMSG_EXIST_SAME_NAME_JOB						g_strMessage[118]
#define	dMSG_ADD_MONSTER								g_strMessage[119]
#define	dMSG_EDIT_MONSTER								g_strMessage[120]

#define	dMSG_REMOVE_MONSTER								g_strMessage[121]
#define	dMSG_ADD_MONSTER_ERROR							g_strMessage[122]
#define	dMSG_NAME										g_strMessage[123]
#define	dMSG_JOB										g_strMessage[124]
#define	dMSG_JOB2_NOT									g_strMessage[125]

#define	dMSG_ADD_SPECIAL_AREA							g_strMessage[126]
#define	dMSG_KIND										g_strMessage[127]
#define	dMSG_COMMON_SKILL								g_strMessage[128]
#define	dMSG_EXCLUSIVE_SKILL							g_strMessage[129]
#define	dMSG_EDIT										g_strMessage[130]

#define	dMSG_LEVEL										g_strMessage[131]
#define	dMSG_REMOVE_SELECT_JOB							g_strMessage[132]
#define	dMSG_DO_YOU_WNAT_REMOVE_SELECT_JOB				g_strMessage[133]
#define	dMSG_SET_HERO_DATA								g_strMessage[134]
#define	dMSG_PART										g_strMessage[135]

#define	dMSG_EQUIPMENT									g_strMessage[136]
#define	dMSG_ADD_POWER									g_strMessage[137]
#define	dMSG_VALUE										g_strMessage[138]
#define	dMSG_TERM										g_strMessage[139]
#define	dMSG_TIME										g_strMessage[140]

#define	dMSG_COMMENT									g_strMessage[141]
#define	dMSG_VALUE_RANGE								g_strMessage[142]
#define	dMSG_VALUE_APPLY_ALL							g_strMessage[143]
#define	dMSG_VALUE_APPLY_VALUE							g_strMessage[144]
#define	dMSG_VALUE_APPLY_PERCENTAGE						g_strMessage[145]

#define	dMSG_DURATION									g_strMessage[146]
#define	dMSG_SKILL										g_strMessage[147]
#define	dMSG_POWER										g_strMessage[148]
#define	dMSG_HIT_RATE									g_strMessage[149]
#define	dMSG_RANGE										g_strMessage[150]

#define	dMSG_SECOND_JOB_SETTING_ERROR					g_strMessage[151]
#define	dMSG_EQUAL_FIRST_JOB							g_strMessage[152]
#define	dMSG_NOT_EXIST									g_strMessage[153]
#define	dMSG_ALL_KIND_ITEM								g_strMessage[154]
#define	dMSG_ITEM_NOT_EXIST								g_strMessage[155]

#define	dMSG_NO_LIMIT									g_strMessage[156]
#define	dMSG_BASE_ITEM_ADD_ERROR						g_strMessage[157]
#define	dMSG_SELECT_ITEM_KIND							g_strMessage[158]
#define	dMSG_SOME_DATA_VALID							g_strMessage[159]
#define	dMSG_ALREADY_EXIST_SAME_ITEM					g_strMessage[160]

#define	dMSG_ITEM_NAME_VALID							g_strMessage[161]
#define	dMSG_VALID_COMMENT								g_strMessage[162]
#define	dMSG_ICON_SHAPE									g_strMessage[163]
#define	dMSG_FIELD_SHAPE								g_strMessage[164]
#define	dMSG_EQUIPPED_SHAPE								g_strMessage[165]

#define	dMSG_NO_CHANGE									g_strMessage[166]
#define	dMSG_PRICE										g_strMessage[167]
#define	dMSG_DURABILITY									g_strMessage[168]
#define	dMSG_DAMAGE										g_strMessage[169]
#define	dMSG_DEFENSE_POWER								g_strMessage[170]

#define	dMSG_EQUIP_USE_LIMIT							g_strMessage[171]
#define	dMSG_ALLIGNMENT									g_strMessage[172]
#define	dMSG_STRENGTH									g_strMessage[173]
#define	dMSG_AGILITY									g_strMessage[174]
#define	dMSG_CONSTITUTE									g_strMessage[175]

#define	dMSG_WISDOM										g_strMessage[176]
#define	dMSG_INTELLIGENCE								g_strMessage[177]
#define	dMSG_CHARISMA									g_strMessage[178]
#define	dMSG_LUCK										g_strMessage[179]
#define	dMSG_WEIGHT										g_strMessage[180]

#define	dMSG_ATTACK_SPEED								g_strMessage[181]
#define	dMSG_BASIC_DATA									g_strMessage[182]
#define	dMSG_ERROR_ADD_ITEM_EFFECT						g_strMessage[183]
#define	dMSG_MAX_ITEM_EFFECT_COUNT						g_strMessage[184]
#define	dMSG_REALLY_REMOVE_FOLLOW_ITEM_EFFECT			g_strMessage[185]

#define	dMSG_EFFECT										g_strMessage[186]
#define	dMSG_REALLY_REMOVE_FOLLOW_ITEM					g_strMessage[187]
#define	dMSG_NO_EFFECT									g_strMessage[188]
#define	dMSG_ERROR_ADD_SKILL							g_strMessage[189]
#define	dMSG_FULL_SKILL									g_strMessage[190]

#define	dMSG_EQUIP_ABLE_JOB								g_strMessage[191]
#define	dMSG_ALL_JOB									g_strMessage[192]
#define	dMSG_ATTACK_RANGE								g_strMessage[193]
#define	dMSG_NO_ITEM_EQUIPPED							g_strMessage[194]
#define	dMSG_MAGIC_AND_SKILL							g_strMessage[195]

#define	dMSG_SET_MONSTER_JOB							g_strMessage[196]
#define	dMSG_SET_NPC_JOB								g_strMessage[197]
#define	dMSG_MONSTER									g_strMessage[198]
#define	dMSG_JOB2_VALID									g_strMessage[199]
#define	dMSG_ERROR_NEW_JOB								g_strMessage[200]

#define	dMSG_ALREADYEXIST_FOLLOW_NAME_JOB				g_strMessage[201]
#define	dMSG_REGIST_HERO_COUNT							g_strMessage[202]
#define	dMSG_REGIST_MONSTER_COUNT						g_strMessage[203]
#define	dMSG_REGIST_NPC_COUNT							g_strMessage[204]
#define	dMSG_FREQUENCY									g_strMessage[205]

#define	dMSG_COUNT										g_strMessage[206]
#define	dMSG_ADD_NPC									g_strMessage[207]
#define	dMSG_WRITE_FILE_ERROR							g_strMessage[208]
#define	dMSG_CAN_NOT_OPEN_FOLLOW_FILE					g_strMessage[209]
#define	dMSG_FILE_OPEN_ERROR							g_strMessage[210]

#define	dMSG_CAN_NOT_FIND_PROJECT_FOLDER				g_strMessage[211]
#define	dMSG_SAVE_MAP									g_strMessage[212]
#define	dMSG_ERROR_SAVE_DUNGEON							g_strMessage[213]
#define	dMSG_NOT_SETTING_FIRST_MAP						g_strMessage[214]
#define	dMSG_VALID_DUNGEON_FOLDER						g_strMessage[215]

#define	dMSG_VALID_DUNGEON_NAME							g_strMessage[216]
#define	dMSG_VALID_DUNGEON_START_MAP					g_strMessage[217]
#define	dMSG_SAVE_DUNGEON_DATA							g_strMessage[218]
#define	dMSG_CAN_NOT_CHANGE_DUNGEON_FOLDER				g_strMessage[219]
#define	dMSG_SELECT_DUNGEON_MAP_DATA_FOLDER				g_strMessage[220]

#define	dMSG_ERROR_SELECT_MAP_FOLDER					g_strMessage[221]
#define	dMSG_PLEASE_SELECT_PERMIT_MAP_FOLDER			g_strMessage[222]
#define	dMSG_SELECT_START_MAP							g_strMessage[223]
#define	dMSG_ERROR_SELECT_MAP							g_strMessage[224]
#define	dMSG_PLEASE_FIRST_SELECT_DUNGEON_MAP_FOLDER		g_strMessage[225]

#define	dMSG_FOLLOW_FILE_NOT_REDSTONE_MAP_FILE			g_strMessage[226]
#define	dMSG_EXIST_CHANGE_DATA							g_strMessage[227]
#define	dMSG_CAN_NOT_FIND_FOLLOW_FOLDER					g_strMessage[228]
#define	dMSG_CAN_NOT_FIND_FIRST_MAP						g_strMessage[229]
#define	dMSG_SELECT_MAP									g_strMessage[230]

#define	dMSG_MOVE_LEVEL									g_strMessage[231]
#define	dMSG_PORTAL										g_strMessage[232]
#define	dMSG_DIRECT										g_strMessage[233]
#define	dMSG_ALREADY_EXIST_FOLLOW_DIRECT_PORTAL			g_strMessage[234]
#define	dMSG_ALREADY_EXIST_START_AREA					g_strMessage[235]

#define	dMSG_VALID_AREA_NAME							g_strMessage[236]
#define	dMSG_MONSTER_NOT_SELECTED						g_strMessage[237]
#define	dMSG_LIMIT_REGEN_MONSTER_COUNT					g_strMessage[238]
#define	dMSG_SELECT_ALL									g_strMessage[239]
#define	dMSG_SELECT_CHARACTER							g_strMessage[240]

#define	dMSG_SELECT_ALL_OBJECT							g_strMessage[241]
#define	dMSG_SELECT_ALL_BUILDING						g_strMessage[242]
#define	dMSG_SELECT										g_strMessage[243]
#define	dMSG_CANCEL_EDIT								g_strMessage[244]
#define	dMSG_ACTOR_SETTING								g_strMessage[245]

#define	dMSG_VALID										g_strMessage[246]
#define	dMSG_BLOCK_RATE									g_strMessage[247]
#define	dMSG_RARE_LEVEL									g_strMessage[248]
#define	dMSG_LINK_MONSTER								g_strMessage[249]
#define	dMSG_GENERATE_LEVEL								g_strMessage[250]

#define	dMSG_TOO_MANY_LINK_MONSTER						g_strMessage[251]
#define	dMSG_REMOVE_LINK								g_strMessage[252]
#define	dMSG_LEADER										g_strMessage[253]
#define	dMSG_WARD										g_strMessage[254]
#define	dMSG_MONSTER_ACTIVE_AREA						g_strMessage[255]

#define	dMSG_NOT_MOVE									g_strMessage[256]
#define	dMSG_SET_LEADER									g_strMessage[257]
#define	dMSG_SET_WARD									g_strMessage[258]
#define	dMSG_AREA_COUNT									g_strMessage[259]
#define	dMSG_NPC_COUNT									g_strMessage[260]

#define	dMSG_SOME_VALUE_INCORRECT						g_strMessage[261]
#define	dMSG_SHADOW										g_strMessage[262]
#define	dMSG_REALLY_REMOVE_FOLLOW_ITEM_DATA				g_strMessage[263]
#define	dMSG_ADD_PREFIX_ERROR							g_strMessage[264]
#define	dMSG_PREFIX_OVERLAPPING							g_strMessage[265]

#define	dMSG_VALID_PREFIX								g_strMessage[266]
#define	dMSG_ALL										g_strMessage[267]
#define	dMSG_REALLY_REMOVE_FOLLOW_PREFIX				g_strMessage[268]
#define	dMSG_VALID_PRICE_VALUE							g_strMessage[269]
#define	dMSG_TALK_BY_FORM								g_strMessage[270]
	
#define	dMSG_SELECT_START_SPEECH_SET_FORM				g_strMessage[271]
#define	dMSG_EDIT_SELECT_START_SPEECH_SET_FORM			g_strMessage[272]
#define	dMSG_ADD_SPEECH_SET_FORM						g_strMessage[273]
#define	dMSG_EDIT_SPEECH_SET_FORM						g_strMessage[274]
#define	dMSG_ADD_SPEECH_FORM							g_strMessage[275]

#define	dMSG_EDIT_SPEECH_FORM							g_strMessage[276]
#define	dMSG_NOT_EXIST_TRIGGER_FOR_SELECT_FIRST_DIALOG	g_strMessage[277]
#define	dMSG_OWN_SPEECH_NPC								g_strMessage[278]
#define	dMSG_OWN_SHOP_NPC								g_strMessage[279]
#define	dMSG_WRONG_MOVE_AREA							g_strMessage[280]

#define	dMSG_NOT_EXIST_CONNECT_AREA						g_strMessage[281]
#define	dMSG_CHECK_OBJECT_SHADOW						g_strMessage[282]
#define	dMSG_REMOVE_OBJECT_SHADOW						g_strMessage[283]
#define	dMSG_EVENT_INFO									g_strMessage[284]
#define	dMSG_DEATH_INFO									g_strMessage[285]

#define	dMSG_REMOVE_SELECT_EVENT						g_strMessage[286]
#define	dMSG_TARGET_PLAYER								g_strMessage[287]
#define	dMSG_TARGET_PARTY								g_strMessage[288]
#define	dMSG_SET_EVENT									g_strMessage[289]
#define	dMSG_SELECT_SAME_OBJECT_IN_FIELD				g_strMessage[290]

#define	dMSG_SELECT_SAME_OBJECT_IN_SCREEN				g_strMessage[291]
#define	dMSG_SET_CUSTOM_ITEM_ERROR_FORM					g_strMessage[292]
#define	dMSG_OCCUR_PROBLEM								g_strMessage[293]
#define	dMSG_CURRENT_ITEM_STACK_ABLE_ITEM_COUNT_FORM	g_strMessage[294]
#define	dMSG_NOT_DIALOG_AND_REACT_FIRST_SELECT_SPEECH	g_strMessage[295]

#define	dMSG_SELECT_AREA_COPY							g_strMessage[296]
#define	dMSG_SELECT_AREA_PASTE							g_strMessage[297]
#define	dMSG_SELECT_SAME_KIND							g_strMessage[298]


extern	char	*g_strMessage[];
extern	char	*g_strAttackPart[];
extern	char	*g_strActorPowerLimit;
extern	char	*g_str8Direct[];
extern	char	*g_astrQuestCategory[];
extern	char	*g_strRelatedPlaceOfGuildList[];	//	길드 연고지 리스트

#endif