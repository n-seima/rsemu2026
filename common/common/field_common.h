#pragma once

#define	dMAP_FILE_HEADER_FORM	"Red Stone Scenario File %d.%d beta"
// extern	char	g_astrMapHeader[100][50]
// {
// 	"Red Stone Scenario File 1.0 beta",
// 	"Red Stone Scenario File 1.1 beta",
// 	"Red Stone Scenario File 1.2 beta",
// 	"Red Stone Scenario File 1.3 beta",
// 	"Red Stone Scenario File 1.4 beta",
// 	"Red Stone Scenario File 1.5 beta",
// 	"Red Stone Scenario File 1.6 beta",
// 	"Red Stone Scenario File 1.7 beta",
// 	"Red Stone Scenario File 1.8 beta",
// 	"Red Stone Scenario File 1.9 beta",
// 	"Red Stone Scenario File 2.0 beta",
// 	"Red Stone Scenario File 2.1 beta",
// 	"Red Stone Scenario File 2.2 beta",
// 	"Red Stone Scenario File 2.3 beta",
// 	"Red Stone Scenario File 2.4 beta",
// 	"Red Stone Scenario File 2.5 beta",
// 	"Red Stone Scenario File 2.6 beta",
// 	"Red Stone Scenario File 2.7 beta",
// 	"Red Stone Scenario File 2.8 beta",
// 	"Red Stone Scenario File 2.9 beta",
// 	"Red Stone Scenario File 3.0 beta",
// 	"Red Stone Scenario File 3.1 beta",
// 	"Red Stone Scenario File 3.2 beta",
// 	"Red Stone Scenario File 3.3 beta",
// 	"Red Stone Scenario File 3.4 beta",
// 	"Red Stone Scenario File 3.5 beta",
// 	"Red Stone Scenario File 3.6 beta",
// 	"Red Stone Scenario File 3.7 beta",
// 	"Red Stone Scenario File 3.8 beta",
// 	"Red Stone Scenario File 3.9 beta",
// 	"Red Stone Scenario File 4.0 beta",
// 	"Red Stone Scenario File 4.1 beta",
// 	"Red Stone Scenario File 4.2 beta",
// 	"Red Stone Scenario File 4.3 beta",
// 	"Red Stone Scenario File 4.4 beta",
// 	"Red Stone Scenario File 4.5 beta",
// 	"Red Stone Scenario File 4.6 beta",
// 	"Red Stone Scenario File 4.7 beta",
// 	"Red Stone Scenario File 4.8 beta",
// 	"Red Stone Scenario File 4.9 beta",
// 	"Red Stone Scenario File 5.0 beta",
// 	"Red Stone Scenario File 5.1 beta",
// 	"Red Stone Scenario File 5.2 beta",
// 	"Red Stone Scenario File 5.3 beta",
// 	"Red Stone Scenario File 5.4 beta",
// 	"Red Stone Scenario File 5.5 beta",
// 	"Red Stone Scenario File 5.6 beta",
// 	"Red Stone Scenario File 5.7 beta",
// 	"Red Stone Scenario File 5.8 beta",
// 	"Red Stone Scenario File 5.9 beta",
// 	"Red Stone Scenario File 6.0 beta",
// 	"Red Stone Scenario File 6.1 beta",
// 	"Red Stone Scenario File 6.2 beta",
// 	"end"
// };

enum
{
//	VUI -> VERSION UPDATE INFO
	eVUI_02_SHOP_INFO			=	2	,
	eVUI_03_ACTOR_SPEECH_DATA			,
	eVUI_04_SAVE_ACTOR_DATA_SKIP_POINT	,
	eVUI_05_CHANGE_SHOP_DATA_SAVE_LOCATE,
	eVUI_06_SAVE_SHOP_DATA_SKIP_POINT	,
	eVUI_07_SAVE_SHOP_PRICE_FACTOR		,
	eVUI_08_ADD_DBOX_SIZE_AND_NO_SPEECH	,
	eVUI_09_ADD_ACTOR_NAME				,
	eVUI_10_ADD_BGM						,
	eVUI_11_ADD_MOVE_GATE_NAME			,
	eVUI_12_ADD_CURRENT_MAP_POS			,
	eVUI_13_CHANGE_DIALG_SELECT_SPEECH	,
	eVUI_14_ADD_RANDOM_CHOICE_DIALOG	,
	eVUI_15_ADD_OBJECT_SHADOW_CHECK		,
	eVUI_16_ADD_CHARACTER_EVENT			,
	eVUI_17_ADD_AREA_SAVE_POINT			,
	eVUI_18_ADD_CHARACTER_JOB_LIST		,
	eVUI_19_ADD_CUSTOM_ITEM				,
	eVUI_20_ADD_KARMA_OCCUR_CHANCE		,
	eVUI_21_ADD_BGM_LIST				,
	eVUI_22_ADD_TRIGGER_ACTIVE_CONDITION,
	eVUI_23_ADD_CUSTOM_ITEM_NAME		,
	eVUI_24_ADD_CORRECT_MAGIC_RESISTANCE,
	eVUI_25_ADD_KARMA					,
	eVUI_26_ADD_DUNGEON_VALUE_AND_KARMA_VALUE	,
	eVUI_27_ADD_CHARACTER_DATA			,
	eVUI_28_EXPAND_AREA_DATA			,
	eVUI_29_ADD_DOOR_LIST				,
	eVUI_30_ADD_CORECT_MONSTER_RESISTANCE,
	eVUI_31_ADD_FIELD_KARMA				,
	eVUI_32_ADD_AREA_CC					,
	eVUI_33_REMOVE_DUNGEON_VALUE		,
	eVUI_34_ADD_CHARACTER_SKIN			,
	eVUI_35_ADD_BOSS_ZONE				,
	eVUI_36_ADD_CHARACTER_PATTERN		,
	eVUI_37_ADD_BLOCK_TO_AUTO_REGEN		,
	eVUI_38_ADD_CORRECT_CHARACTER_POWER	,
	eVUI_39_ADD_FIELD_VALUE				,
	eVUI_40_ADD_DAMAGE_IMMUNE			,
	eVUI_41_ADD_REVISE_INT				,
	eVUI_42_ADD_DUEL_SERVER				,
	eVVI_43_ADD_SEASON_VARIABLE			,
	eVVI_44_ADD_TOKEN_SHOP				,
	eVVI_45_ADD_MORE_CHARACTER_DATA		,
	eVVI_46_REVISE_BLOCK_TO_TAME		,
	eVVI_47_ADD_HIDE_NAME_BAR_OPTION	,
	eVVI_48_ENCRYT						,
	eVVI_49_ENCRYT_MORE					,
	eVVI_50_ADD_GENERATE_METHOD			,	// insu add
	eVVI_51_ENCRYT2						,
	eVUI_CURRENT_VERSION				,
};

#define	dCURRENT_MAP_VERSION			(eVUI_CURRENT_VERSION-1)

inline	int
GetMapDataVersion(char *_strHeader)
{
	char	strHeader[256];

	for (int i=0;i<100;i++)
	{
		sprintf(strHeader,dMAP_FILE_HEADER_FORM,(i/10)+1,i%10);

		if	(strcmp(strHeader,_strHeader)	==	0)
		{
			return	i;
		}
	}

	return	-1;
}


inline	char*
GetMapDataVersion(int _iVersion=-1)
{
	if	(_iVersion	==	-1)
		_iVersion	=	dCURRENT_MAP_VERSION;

	static	char	strHeader[256];

	sprintf(strHeader,dMAP_FILE_HEADER_FORM,(_iVersion/10)+1,_iVersion%10);

	return	strHeader;
}

