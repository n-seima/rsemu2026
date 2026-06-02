#include "CGamePlay.H"
#include "CActor.H"
#include "CMessage.H"
#include "CWindowInterface.H"
#include "CLoadTextCode.H"
#include "firecracker.h"
#include "bm_event.h"
#include "cMESSAGE.h"
#include "CGamePlayInterface.h"

#define	dQUICK_SLOT_DELAY		500

int		CGamePlay::s_iBloodStoneWidth	=	0;

char	*l_astrQuickSlotShotKey			=	"QWERTASDFG";
enum
{	// BottomInterface
	eBI_NUMBER_ZERO = 195,
};
enum
{
	// 스킬 상태..
	eNOT_USE_SKILL,
	eUSE_SKILL_NOT_TARGET,
	eUSE_SKILL,

};
cRECT	CGamePlay::s_arectSkillSlot[]	=
{
	{8  ,510,8  +34,510+34},	//	좌
	{758,510,758+34,510+34},	//	우
	{22 ,559,22 +34,559+34},	//	패시브
	{744,559,744+34,559+34},	//	보조

	{86 ,524,86 +34,524+34},	//	Quick 1
	{122,524,122+34,524+34},
	{158,524,158+34,524+34},
	{194,524,194+34,524+34},
	{230,524,230+34,524+34},

	{86 ,562,86 +34,562+34},
	{122,562,122+34,562+34},
	{158,562,158+34,562+34},
	{194,562,194+34,562+34},
	{230,562,230+34,562+34},
	{0xffff}
};

cRECT	l_rectQuickSkillSlot	=	{86 ,524,230+34,562+34};
int		l_iBottomTipPosY		=	498;

BOOL	l_bIsRevisedPos			=	FALSE;


cRECT	CGamePlay::s_arectBeltSlot[]	=
{
	{534,524,534+34,524+34},	//	좌
	{570,524,570+34,524+34},	//	우
	{606,524,606+34,524+34},	//	패시브
	{642,524,642+34,524+34},	//	보조
	{678,524,678+34,524+34},	//	Quick 1
	{0xffff}
};

enum
{
	eBII_MAIN,

//	이동 방법
	eBII_MOVE_WALK_NORMAL,
	eBII_MOVE_WALK_ACTIVE,
	eBII_MOVE_WALK_PRESSED,
	eBII_MOVE_RUN_NORMAL,
	eBII_MOVE_RUN_ACTIVE,
	eBII_MOVE_RUN_PRESSED,

//	변신관련
	eBII_CHANGE_JOB_NORMAL,
	eBII_CHANGE_JOB_ACTIVE,
	eBII_CHANGE_JOB_PRESSED,
	eBII_CHANGE_JOB_DISABLE,

//	앉기
	eBII_STAND_NORMAL,
	eBII_STAND_ACTIVE,
	eBII_STAND_PRESSED,
	eBII_STAND_DISABLE,

	eBII_SITDOWN_NORMAL,
	eBII_SITDOWN_ACTIVE,
	eBII_SITDOWN_PRESSED,
	eBII_SITDOWN_DISABLE,

//	메뉴
	eBII_MENU_STATUS_ACTIVE,
	eBII_MENU_STATUS_PRESSED,
	eBII_MENU_STATUS_CHECK,

	eBII_MENU_INVENTORY_ACTIVE,
	eBII_MENU_INVENTORY_PRESSED,
	eBII_MENU_INVENTORY_CHECK,

	eBII_MENU_SKILL_ACTIVE,
	eBII_MENU_SKILL_PRESSED,
	eBII_MENU_SKILL_CHECK,

	eBII_MENU_PARTY_ACTIVE,
	eBII_MENU_PARTY_PRESSED,
	eBII_MENU_PARTY_CHECK,

	eBII_MENU_QUEST_ACTIVE,
	eBII_MENU_QUEST_PRESSED,
	eBII_MENU_QUEST_CHECK,

	eBII_MENU_GUILD_ACTIVE,
	eBII_MENU_GUILD_PRESSED,
	eBII_MENU_GUILD_CHECK,

	eBII_MENU_FRIEND_ACTIVE,
	eBII_MENU_FRIEND_PRESSED,
	eBII_MENU_FRIEND_CHECK,

	eBII_MENU_SYSTEM_ACTIVE,
	eBII_MENU_SYSTEM_PRESSED,
	eBII_MENU_SYSTEM_CHECK,

	eBII_QUICK_SKILL_SLOT0,
	eBII_QUICK_SKILL_SLOT1,
	eBII_QUICK_SKILL_SLOT2,
	eBII_QUICK_SKILL_SLOT3,
	eBII_QUICK_SKILL_SLOT4,
	eBII_QUICK_SKILL_SLOT5,
	eBII_QUICK_SKILL_SLOT6,
	eBII_QUICK_SKILL_SLOT7,
	eBII_QUICK_SKILL_SLOT8,
	eBII_QUICK_SKILL_SLOT9,
	eBII_QUICK_SKILL_UP_ACTIVE,
	eBII_QUICK_SKILL_UP_PRESSED,
	eBII_QUICK_SKILL_DOWN_ACTIVE,
	eBII_QUICK_SKILL_DOWN_PRESSED,

	eBII_RELOAD_ACTIVE,
	eBII_RELOAD_PRESSED,

	eBII_EXP_GAUGE,
	eBII_SKILL_EXP_GAUGE,

	eBII_RED_STONE_01,
	eBII_RED_STONE_02,
	eBII_RED_STONE_03,
	eBII_RED_STONE_04,
	eBII_RED_STONE_05,
	eBII_RED_STONE_06,
	eBII_RED_STONE_07,
	eBII_RED_STONE_08,
	eBII_RED_STONE_09,
	eBII_RED_STONE_10,
	eBII_RED_STONE_11,
	eBII_RED_STONE_12,
	eBII_RED_STONE_13,
	eBII_RED_STONE_14,
	eBII_RED_STONE_15,
	eBII_RED_STONE_16,

	eBII_CP_LEVEL_MINUS_01,
	eBII_CP_LEVEL_MINUS_02,
	eBII_CP_LEVEL_MINUS_03,
	eBII_CP_LEVEL_MINUS_04,
	eBII_CP_LEVEL_MINUS_05,
	eBII_CP_LEVEL_MINUS_06,
	eBII_CP_LEVEL_MINUS_07,
	eBII_CP_LEVEL_MINUS_08,
	eBII_CP_LEVEL_MINUS_09,
	eBII_CP_LEVEL_MINUS_10,
	eBII_CP_LEVEL_MINUS_11,
	eBII_CP_LEVEL_MINUS_12,
	eBII_CP_LEVEL_MINUS_13,
	eBII_CP_LEVEL_MINUS_14,
	eBII_CP_LEVEL_MINUS_15,
	eBII_CP_LEVEL_MINUS_16,

	eBII_CP_LEVEL_MINUS_01_LEFT,
	eBII_CP_LEVEL_MINUS_02_LEFT,
	eBII_CP_LEVEL_MINUS_03_LEFT,
	eBII_CP_LEVEL_MINUS_04_LEFT,
	eBII_CP_LEVEL_MINUS_05_LEFT,
	eBII_CP_LEVEL_MINUS_06_LEFT,
	eBII_CP_LEVEL_MINUS_07_LEFT,
	eBII_CP_LEVEL_MINUS_08_LEFT,
	eBII_CP_LEVEL_MINUS_09_LEFT,
	eBII_CP_LEVEL_MINUS_10_LEFT,
	eBII_CP_LEVEL_MINUS_11_LEFT,
	eBII_CP_LEVEL_MINUS_12_LEFT,
	eBII_CP_LEVEL_MINUS_13_LEFT,
	eBII_CP_LEVEL_MINUS_14_LEFT,
	eBII_CP_LEVEL_MINUS_15_LEFT,
	eBII_CP_LEVEL_MINUS_16_LEFT,

	eBII_CP_LEVEL_MINUS_ACTIVE_1,
	eBII_CP_LEVEL_MINUS_ACTIVE_2,
	eBII_CP_LEVEL_MINUS_ACTIVE_3,

	eBII_CP_LEVEL_1_01,
	eBII_CP_LEVEL_1_02,
	eBII_CP_LEVEL_1_03,
	eBII_CP_LEVEL_1_04,
	eBII_CP_LEVEL_1_05,
	eBII_CP_LEVEL_1_06,
	eBII_CP_LEVEL_1_07,
	eBII_CP_LEVEL_1_08,
	eBII_CP_LEVEL_1_09,
	eBII_CP_LEVEL_1_10,
	eBII_CP_LEVEL_1_11,
	eBII_CP_LEVEL_1_12,
	eBII_CP_LEVEL_1_13,
	eBII_CP_LEVEL_1_14,
	eBII_CP_LEVEL_1_15,
	eBII_CP_LEVEL_1_16,

	eBII_CP_LEVEL_1_01_LEFT,
	eBII_CP_LEVEL_1_02_LEFT,
	eBII_CP_LEVEL_1_03_LEFT,
	eBII_CP_LEVEL_1_04_LEFT,
	eBII_CP_LEVEL_1_05_LEFT,
	eBII_CP_LEVEL_1_06_LEFT,
	eBII_CP_LEVEL_1_07_LEFT,
	eBII_CP_LEVEL_1_08_LEFT,
	eBII_CP_LEVEL_1_09_LEFT,
	eBII_CP_LEVEL_1_10_LEFT,
	eBII_CP_LEVEL_1_11_LEFT,
	eBII_CP_LEVEL_1_12_LEFT,
	eBII_CP_LEVEL_1_13_LEFT,
	eBII_CP_LEVEL_1_14_LEFT,
	eBII_CP_LEVEL_1_15_LEFT,
	eBII_CP_LEVEL_1_16_LEFT,

	eBII_CP_LEVEL_1_ACTIVE_1,
	eBII_CP_LEVEL_1_ACTIVE_2,
	eBII_CP_LEVEL_1_ACTIVE_3,

	eBII_CP_LEVEL_2_01,
	eBII_CP_LEVEL_2_02,
	eBII_CP_LEVEL_2_03,
	eBII_CP_LEVEL_2_04,
	eBII_CP_LEVEL_2_05,
	eBII_CP_LEVEL_2_06,
	eBII_CP_LEVEL_2_07,
	eBII_CP_LEVEL_2_08,
	eBII_CP_LEVEL_2_09,
	eBII_CP_LEVEL_2_10,
	eBII_CP_LEVEL_2_11,
	eBII_CP_LEVEL_2_12,
	eBII_CP_LEVEL_2_13,
	eBII_CP_LEVEL_2_14,
	eBII_CP_LEVEL_2_15,
	eBII_CP_LEVEL_2_16,

	eBII_CP_LEVEL_2_01_LEFT,
	eBII_CP_LEVEL_2_02_LEFT,
	eBII_CP_LEVEL_2_03_LEFT,
	eBII_CP_LEVEL_2_04_LEFT,
	eBII_CP_LEVEL_2_05_LEFT,
	eBII_CP_LEVEL_2_06_LEFT,
	eBII_CP_LEVEL_2_07_LEFT,
	eBII_CP_LEVEL_2_08_LEFT,
	eBII_CP_LEVEL_2_09_LEFT,
	eBII_CP_LEVEL_2_10_LEFT,
	eBII_CP_LEVEL_2_11_LEFT,
	eBII_CP_LEVEL_2_12_LEFT,
	eBII_CP_LEVEL_2_13_LEFT,
	eBII_CP_LEVEL_2_14_LEFT,
	eBII_CP_LEVEL_2_15_LEFT,
	eBII_CP_LEVEL_2_16_LEFT,

	eBII_CP_LEVEL_2_ACTIVE_1,
	eBII_CP_LEVEL_2_ACTIVE_2,
	eBII_CP_LEVEL_2_ACTIVE_3,

	eBII_CP_LEVEL_3_01,
	eBII_CP_LEVEL_3_02,
	eBII_CP_LEVEL_3_03,
	eBII_CP_LEVEL_3_04,
	eBII_CP_LEVEL_3_05,
	eBII_CP_LEVEL_3_06,
	eBII_CP_LEVEL_3_07,
	eBII_CP_LEVEL_3_08,
	eBII_CP_LEVEL_3_09,
	eBII_CP_LEVEL_3_10,
	eBII_CP_LEVEL_3_11,
	eBII_CP_LEVEL_3_12,
	eBII_CP_LEVEL_3_13,
	eBII_CP_LEVEL_3_14,
	eBII_CP_LEVEL_3_15,
	eBII_CP_LEVEL_3_16,

	eBII_CP_LEVEL_3_01_LEFT,
	eBII_CP_LEVEL_3_02_LEFT,
	eBII_CP_LEVEL_3_03_LEFT,
	eBII_CP_LEVEL_3_04_LEFT,
	eBII_CP_LEVEL_3_05_LEFT,
	eBII_CP_LEVEL_3_06_LEFT,
	eBII_CP_LEVEL_3_07_LEFT,
	eBII_CP_LEVEL_3_08_LEFT,
	eBII_CP_LEVEL_3_09_LEFT,
	eBII_CP_LEVEL_3_10_LEFT,
	eBII_CP_LEVEL_3_11_LEFT,
	eBII_CP_LEVEL_3_12_LEFT,
	eBII_CP_LEVEL_3_13_LEFT,
	eBII_CP_LEVEL_3_14_LEFT,
	eBII_CP_LEVEL_3_15_LEFT,
	eBII_CP_LEVEL_3_16_LEFT,

	eBII_CP_LEVEL_3_ACTIVE_1,
	eBII_CP_LEVEL_3_ACTIVE_2,
	eBII_CP_LEVEL_3_ACTIVE_3,

	eBII_CP_LEVEL_4_01,
	eBII_CP_LEVEL_4_02,
	eBII_CP_LEVEL_4_03,
	eBII_CP_LEVEL_4_04,
	eBII_CP_LEVEL_4_05,
	eBII_CP_LEVEL_4_06,
	eBII_CP_LEVEL_4_07,
	eBII_CP_LEVEL_4_08,
	eBII_CP_LEVEL_4_09,
	eBII_CP_LEVEL_4_10,
	eBII_CP_LEVEL_4_11,
	eBII_CP_LEVEL_4_12,
	eBII_CP_LEVEL_4_13,
	eBII_CP_LEVEL_4_14,
	eBII_CP_LEVEL_4_15,
	eBII_CP_LEVEL_4_16,

	eBII_CP_LEVEL_4_01_LEFT,
	eBII_CP_LEVEL_4_02_LEFT,
	eBII_CP_LEVEL_4_03_LEFT,
	eBII_CP_LEVEL_4_04_LEFT,
	eBII_CP_LEVEL_4_05_LEFT,
	eBII_CP_LEVEL_4_06_LEFT,
	eBII_CP_LEVEL_4_07_LEFT,
	eBII_CP_LEVEL_4_08_LEFT,
	eBII_CP_LEVEL_4_09_LEFT,
	eBII_CP_LEVEL_4_10_LEFT,
	eBII_CP_LEVEL_4_11_LEFT,
	eBII_CP_LEVEL_4_12_LEFT,
	eBII_CP_LEVEL_4_13_LEFT,
	eBII_CP_LEVEL_4_14_LEFT,
	eBII_CP_LEVEL_4_15_LEFT,
	eBII_CP_LEVEL_4_16_LEFT,

	eBII_CP_LEVEL_4_ACTIVE_1,
	eBII_CP_LEVEL_4_ACTIVE_2,
	eBII_CP_LEVEL_4_ACTIVE_3,

	eBII_CP_LEVEL_5_01,
	eBII_CP_LEVEL_5_02,
	eBII_CP_LEVEL_5_03,
	eBII_CP_LEVEL_5_04,
	eBII_CP_LEVEL_5_05,
	eBII_CP_LEVEL_5_06,
	eBII_CP_LEVEL_5_07,
	eBII_CP_LEVEL_5_08,
	eBII_CP_LEVEL_5_09,
	eBII_CP_LEVEL_5_10,
	eBII_CP_LEVEL_5_11,
	eBII_CP_LEVEL_5_12,
	eBII_CP_LEVEL_5_13,
	eBII_CP_LEVEL_5_14,
	eBII_CP_LEVEL_5_15,
	eBII_CP_LEVEL_5_16,

	eBII_CP_LEVEL_5_01_LEFT,
	eBII_CP_LEVEL_5_02_LEFT,
	eBII_CP_LEVEL_5_03_LEFT,
	eBII_CP_LEVEL_5_04_LEFT,
	eBII_CP_LEVEL_5_05_LEFT,
	eBII_CP_LEVEL_5_06_LEFT,
	eBII_CP_LEVEL_5_07_LEFT,
	eBII_CP_LEVEL_5_08_LEFT,
	eBII_CP_LEVEL_5_09_LEFT,
	eBII_CP_LEVEL_5_10_LEFT,
	eBII_CP_LEVEL_5_11_LEFT,
	eBII_CP_LEVEL_5_12_LEFT,
	eBII_CP_LEVEL_5_13_LEFT,
	eBII_CP_LEVEL_5_14_LEFT,
	eBII_CP_LEVEL_5_15_LEFT,
	eBII_CP_LEVEL_5_16_LEFT,

	eBII_CP_LEVEL_5_ACTIVE_1,
	eBII_CP_LEVEL_5_ACTIVE_2,
	eBII_CP_LEVEL_5_ACTIVE_3,

};	//	하단 인터페이스


enum
{
	eBM_PLUS_STRENGTH		=	1000,
		eBM_PLUS_LUCK			,
		eBM_PLUS_WISDOM			,
		eBM_PLUS_CHARISMA		,
		eBM_PLUS_CONSTITUTION	,
		eBM_PLUS_AGILITY		,
		eBM_PLUS_INTELLIGENCE	
};

#define dGGG_ICON_START_INDEX				6

//
//하단 인터페이스 초기화
void
CGamePlay::InitBottomInterface()
{
	int		i;

	if	(l_bIsRevisedPos	==	FALSE)
	{
		l_bIsRevisedPos	=	TRUE;
		l_iBottomTipPosY+=	g_iCorrectBI_Y;

		for (i=0;i<5;i++)
		{
			s_arectBeltSlot[i].x1	+=	g_iCorrectBI_X;
			s_arectBeltSlot[i].y1	+=	g_iCorrectBI_Y;
			s_arectBeltSlot[i].x2	+=	g_iCorrectBI_X;
			s_arectBeltSlot[i].y2	+=	g_iCorrectBI_Y;
		}

		for (i=0;i<14;i++)
		{
			s_arectSkillSlot[i].x1	+=	g_iCorrectBI_X;
			s_arectSkillSlot[i].y1	+=	g_iCorrectBI_Y;
			s_arectSkillSlot[i].x2	+=	g_iCorrectBI_X;
			s_arectSkillSlot[i].y2	+=	g_iCorrectBI_Y;
		}

		l_rectQuickSkillSlot.x1	+=	g_iCorrectBI_X;
		l_rectQuickSkillSlot.y1	+=	g_iCorrectBI_Y;
		l_rectQuickSkillSlot.x2	+=	g_iCorrectBI_X;
		l_rectQuickSkillSlot.y2	+=	g_iCorrectBI_Y;
	}


	s_bmBottomMenu.init(&g_sprGameBottomInterface,40,dBAR_CHECK);//하단 인터페이스에 각종 버튼들

	//
	s_bmBottomMenu.addImageBar("",eBIM_RUN				,325+g_iCorrectBI_X,514+g_iCorrectBI_Y,eBII_MOVE_WALK_NORMAL,eBII_MOVE_WALK_ACTIVE,eBII_MOVE_WALK_PRESSED,0xffff,0xffff);
	s_bmBottomMenu.addImageBar("",eBIM_TRANS			,377+g_iCorrectBI_X,514+g_iCorrectBI_Y,eBII_CHANGE_JOB_NORMAL,eBII_CHANGE_JOB_ACTIVE,eBII_CHANGE_JOB_PRESSED,0xffff,eBII_CHANGE_JOB_DISABLE);
	s_bmBottomMenu.addImageBar("",eBIM_SITDOWN			,429+g_iCorrectBI_X,514+g_iCorrectBI_Y,eBII_STAND_NORMAL,eBII_STAND_ACTIVE,eBII_STAND_PRESSED,0xffff,eBII_STAND_DISABLE);
	s_bmBottomMenu.setKey(eBIM_TRANS,KEY_X);
	s_bmBottomMenu.setKey(eBIM_SITDOWN,KEY_C);

	s_bmBottomMenu.addImageBar("",eBIM_MENU_STATUS		,529+g_iCorrectBI_X,562+g_iCorrectBI_Y,0xffff,eBII_MENU_STATUS_ACTIVE,eBII_MENU_STATUS_PRESSED,eBII_MENU_STATUS_CHECK,0xffff,dMSG_STATUS_INFO_WINDOW);	//	12,13,14
	s_bmBottomMenu.addImageBar("",eBIM_MENU_INVENTORY	,576+g_iCorrectBI_X,562+g_iCorrectBI_Y,0xffff,eBII_MENU_INVENTORY_ACTIVE,eBII_MENU_INVENTORY_PRESSED,eBII_MENU_INVENTORY_CHECK,0xffff,dMSG_INVENTORY_MENU);	//	12,13,14
	s_bmBottomMenu.addImageBar("",eBIM_MENU_SKILL		,623+g_iCorrectBI_X,562+g_iCorrectBI_Y,0xffff,eBII_MENU_SKILL_ACTIVE,eBII_MENU_SKILL_PRESSED,eBII_MENU_SKILL_CHECK,0xffff,dMSG_SKILL_INTERFACE);
	s_bmBottomMenu.addImageBar("",eBIM_MENU_PARTY		,670+g_iCorrectBI_X,562+g_iCorrectBI_Y,0xffff,eBII_MENU_PARTY_ACTIVE,eBII_MENU_PARTY_PRESSED,eBII_MENU_PARTY_CHECK,0xffff,dMSG_PARTY_MENU);
	s_bmBottomMenu.addImageBar("",eBIM_MENU_QUEST		,529+g_iCorrectBI_X,583+g_iCorrectBI_Y,0xffff,eBII_MENU_QUEST_ACTIVE,eBII_MENU_QUEST_PRESSED,eBII_MENU_QUEST_CHECK,0xffff,dMSG_QUEST_MENU);
	s_bmBottomMenu.addImageBar("",eBIM_MENU_GUILD		,576+g_iCorrectBI_X,583+g_iCorrectBI_Y,0xffff,eBII_MENU_GUILD_ACTIVE,eBII_MENU_GUILD_PRESSED,eBII_MENU_GUILD_CHECK,0xffff,dMSG_GUILD_MENU);
	s_bmBottomMenu.addImageBar("",eBIM_MENU_FRIEND		,623+g_iCorrectBI_X,583+g_iCorrectBI_Y,0xffff,eBII_MENU_FRIEND_ACTIVE,eBII_MENU_FRIEND_PRESSED,eBII_MENU_FRIEND_CHECK,0xffff,dMSG_FRIEND_MENU);
	s_bmBottomMenu.addImageBar("",eBIM_MENU_SYSTEM		,670+g_iCorrectBI_X,583+g_iCorrectBI_Y,0xffff,eBII_MENU_SYSTEM_ACTIVE,eBII_MENU_SYSTEM_PRESSED,eBII_MENU_SYSTEM_CHECK,0xffff,dMSG_SYSTEM_MENU);

	int	iIsOldStyle		=	g_config.m_bf1IsUseOldStyleShotKey;

	s_bmBottomMenu.setKey(eBIM_MENU_STATUS			,KEY_F2-iIsOldStyle);
	s_bmBottomMenu.setKey(eBIM_MENU_INVENTORY		,KEY_F3-iIsOldStyle);
	s_bmBottomMenu.setKey(eBIM_MENU_SKILL			,KEY_F4-iIsOldStyle);
	s_bmBottomMenu.setKey(eBIM_MENU_PARTY			,KEY_F5-iIsOldStyle);
	s_bmBottomMenu.setKey(eBIM_MENU_QUEST			,KEY_F6-iIsOldStyle);
	s_bmBottomMenu.setKey(eBIM_MENU_GUILD			,KEY_F7-iIsOldStyle);
	s_bmBottomMenu.setKey(eBIM_MENU_FRIEND			,KEY_F8-iIsOldStyle);
	s_bmBottomMenu.setKey(eBIM_MENU_SYSTEM			,KEY_F9-iIsOldStyle);

	s_bmBottomMenu.addImageBar("",eBIM_QUICK_SKILL_UP	,265+g_iCorrectBI_X,523+g_iCorrectBI_Y,0xffff,eBII_QUICK_SKILL_UP_ACTIVE,eBII_QUICK_SKILL_UP_PRESSED,0xffff,0xffff,dMSG_NEXT_SKILL_SET);
	s_bmBottomMenu.addImageBar("",eBIM_QUICK_SKILL_DOWN	,265+g_iCorrectBI_X,560+g_iCorrectBI_Y,0xffff,eBII_QUICK_SKILL_DOWN_ACTIVE,eBII_QUICK_SKILL_DOWN_PRESSED,0xffff,0xffff,dMSG_PREVIOUS_SKILL_SET);

	s_bmBottomMenu.addImageBar("",eBIM_QUICK_ITEM_RELOAD	,501+g_iCorrectBI_X,519+g_iCorrectBI_Y,0xffff,eBII_RELOAD_ACTIVE,eBII_RELOAD_PRESSED,0xffff,0xffff,dMSG_BELT_ITEM_RELOAD_TOOLTIP);
#ifdef	_FOR_JAPAN
	s_bmBottomMenu.setKey(eBIM_QUICK_ITEM_RELOAD	,KEY_6);
#else
	s_bmBottomMenu.setKey(eBIM_QUICK_ITEM_RELOAD	,KEY_GRAVE);
#endif

	int	aQuickSkillHotKey[]	=	{KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G};

	for (i=0;i<4;i++)
	{
		s_bmBottomMenu.addImageBar("",eBIM_LEFT_SKILL1+i	,s_arectSkillSlot[i].x1,s_arectSkillSlot[i].y1);
		s_bmBottomMenu.setSize(eBIM_LEFT_SKILL1+i,s_arectSkillSlot[i].x1,s_arectSkillSlot[i].y1,s_arectSkillSlot[i].getWidth(),s_arectSkillSlot[i].getHeight());
	}

	for (i=0;i<10;i++)
	{
		s_bmBottomMenu.addImageBar("",eBIM_QUICK_SKILL1+i	,s_arectSkillSlot[i+4].x1,s_arectSkillSlot[i+4].y1);
		s_bmBottomMenu.setSize(eBIM_QUICK_SKILL1+i,s_arectSkillSlot[i+4].x1,s_arectSkillSlot[i+4].y1,s_arectSkillSlot[i+4].getWidth(),s_arectSkillSlot[i+4].getHeight());
		s_bmBottomMenu.setKey(eBIM_QUICK_SKILL1+i,aQuickSkillHotKey[i]);
	}

//	퀵 아이템
	for (i=0;i<5;i++)
	{
		s_bmBottomMenu.addImageBar("",eBIM_QUICK_ITEM1+i	,s_arectBeltSlot[i].x1,s_arectBeltSlot[i].y1);
		s_bmBottomMenu.setSize(eBIM_QUICK_ITEM1+i,s_arectBeltSlot[i].x1,s_arectBeltSlot[i].y1,s_arectBeltSlot[i].getWidth(),s_arectBeltSlot[i].getHeight());
		s_bmBottomMenu.setKey(eBIM_QUICK_ITEM1+i,KEY_1+i);
	}
}

//
//	하단 인터페이스의 슬롯들 툴팁
void
CGamePlay::OperateBottomInterfaceTooltip()
{
	char	*strComment;
	cRECT	rect;

//	패시브 스킬들 툴팁
	if (s_inputChatting.isEnable() == FALSE)
	{
		int	iXPos					=	696+g_iCorrectBI_X;
		int	iYPos					=	l_iBottomTipPosY-4;
		int	iReactionSlot			=	g_hero.m_wJob%2;
		int	iScale					=	65;
		int	iSize					=	dITEM_SHAPE_WIDTH*iScale/100;
		int	iActiveReactionCount	=	0;

		for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
		{
			BOOL	bIsActivate	=	FALSE;
			int		iAbility	=	g_hero.m_aActivePassiveSkill[iReactionSlot][i];

			if (iAbility		>=	dABILITY_COUNT)
				continue;

			cAbility *lpAbility	=	&g_hero.m_aAbility[iAbility];
			CSkill	 *lpSkill	=	lpAbility->getSkill();

			if	(!lpSkill)
				continue;

			cRECT	rect;

			rect.set(iXPos,iYPos,iXPos+iSize,iYPos+iSize);

			iXPos	-=	(iSize+2);

			if (!rect.isIn(s_posMouse.x,s_posMouse.y))
				continue;

			if	(lpSkill->m_wCastMethod	==	eST_PASSIVE)
				bIsActivate	=	lpAbility->isActivatePassiveSkill(g_hero.m_iCP);
			if	(lpSkill->m_wCastMethod	==	eST_REACTION)
				bIsActivate	=	lpAbility->isActivateReactionSkill(GetCPLevel(g_hero.m_iCP));

			char	strText[512];

			if	(bIsActivate)
				sprintf(strText,"<c:LTYELLOW>%s<n><Lv %d>",lpSkill->m_strName,lpAbility->m_wLevel+g_hero.getCorrectSkillLevel(lpAbility));
			else
				sprintf(strText,"<c:GRAY>%s<n><Lv %d>",lpSkill->m_strName,lpAbility->m_wLevel);

			s_rectTooltip.set(&rect);
			s_rectTooltip.add(0,-6);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strText,eTFM_BOTTOM);

			WORD	*lpImage	=	g_smiIconSkill.get16Image(lpSkill->m_wIconIndex);
			s_ttCommon.setImage(lpImage);

			return;
		}
	}

//	GGG 툴팁
	{
		cRECT					GGGRect;
		BOOL					bIsActiveLevelAndSkill = FALSE;
		GGGRect.set(120+g_iCorrectBI_X , 497+g_iCorrectBI_Y , 137+g_iCorrectBI_X , 514+g_iCorrectBI_Y);
		int GGGImageIconindex = g_hero.m_bf3GreateGodGrace - 1;

		if(g_hero.m_GGG.m_wGGGTimeForSkillUp)	//스킬업 툴팁
		{
			if	(g_hero.m_bf3GreateGodGrace && GGGRect.isIn(s_posMouse.x,s_posMouse.y))
			{
				cSTRING	string;

				string.Add("<c:LTYELLOW>%s<n>\n\n",g_pstrGreateGodGrace[dMSG_GGG_SKILLUP_EFFECT_NAME_START_INDEX + GGGImageIconindex]);
				string.Add("<<c:LTCYAN>%s<n>>\n",dMSG_INFORMATION);
				string.Add("- %s\n",g_pstrGreateGodGrace[GGGImageIconindex]);
				string.Add("- %s\n",_ms(dMSG_GGG_LEVELUP_EFFECT_COMMENT_VALUE1_TOOLTIP_FORM, GetGGGSkillUpTableValue(g_hero.m_bf3GreateGodGrace , eINCRECE_VALUE1)));
				string.Add("- %s\n",_ms(dMSG_GGG_ATTACK_SPEED,GetGGGSkillUpTableValue(g_hero.m_bf3GreateGodGrace , eINCRECE_VALUE2)));
				string.Add("> %s\n\n",_ms(dMSG_GGG_REMAIN_TIME, g_hero.m_GGG.m_wGGGTimeForSkillUp));
				
				string.Add("<c:LTCYAN>%s<n>\n",dMSG_COMMENT);
				string.Add("- %s",_ms(dMSG_GGG_TOOLTIP_COMMENT_SKILL, g_pstrGreateGodGrace[GGGImageIconindex]));
				
				s_ttCommon.popupTip(&GGGRect,-1,-1,&s_text,WHITE,string.String,eTFM_LEFT);
				
				WORD	*lpImage	=	g_smiIconGGG.get16Image(GGGImageIconindex);
				s_ttCommon.setImage(lpImage);
			}
		}
	}

//	스킬들의 툴팁
	{
		int			iIndex,iFocusSlot=0xffff,iFocusAbility=0xffff;
		cAbility	*lpAbility	=	NULL;

		for (iIndex=0;s_arectSkillSlot[iIndex].x1!=0xffff;iIndex++)
		{
			if (!s_arectSkillSlot[iIndex].isIn(s_posMouse.x,s_posMouse.y))
				continue;

			s_rectTooltip.set(&s_arectSkillSlot[iIndex]);

			switch(iIndex)
			{
				case	0	:
					iFocusSlot		=	iIndex;
					iFocusAbility	=	iIndex;
					s_rectTooltip.add(dITEM_SHAPE_WIDTH+4,0);
					lpAbility	=	g_hero.getLeftAbility();
					if	(!lpAbility)
						s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,dMSG_TOOLTIP_LEFT_SKILL_SLOT);
					break;
				case	1	:
					iFocusSlot		=	iIndex;
					iFocusAbility	=	iIndex;
					s_rectTooltip.add(-4,-4);
					lpAbility	=	g_hero.getRightAbility();
					if	(!lpAbility)
						s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,dMSG_TOOLTIP_RIGHT_SKILL_SLOT,eTFM_LEFT);
					break;
				case	2	:
					iFocusSlot		=	iIndex;
					iFocusAbility	=	iIndex;
					s_rectTooltip.add(dITEM_SHAPE_WIDTH+4,0);
					lpAbility	=	g_hero.getSubAbility();
					if	(!lpAbility)
						s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,dMSG_TOOLTIP_SUB_SKILL_SLOT);
					break;
				case	3	:
					iFocusSlot		=	iIndex;
					iFocusAbility	=	iIndex;
					s_rectTooltip.add(-4,0);
					lpAbility	=	g_hero.getSupportAbility();
					if	(!lpAbility)
						s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,dMSG_TOOLTIP_SUPPORT_SKILL_SLOT,eTFM_LEFT);
					break;

				case	4	:
				case	5	:
				case	6	:
				case	7	:
				case	8	:
				case	9	:
				case	10	:
				case	11	:
				case	12	:
				case	13	:
				{
					iFocusSlot		=	iIndex;
					iFocusAbility	=	iIndex;
					lpAbility		=	g_hero.getQuickSkill(iIndex-4);
					// 10.01.07 스킬슬롯에 있는 아이템 툴팁
					int iItemIndex = g_hero.m_aQuickSkill[g_hero.m_wCurrentQuickSkillSlot][iIndex-4][g_hero.m_wJob%2];
					
					if( iItemIndex >= dEQUIPMENT_SKILL_SLOT && iItemIndex <= 400)
					{
						if	(s_isLeftButtonPressed	==	FALSE)
						{
							if( iItemIndex == 0xffff)
								return;

							cItem	*lpItem	=	g_hero.getInventoryItem(iItemIndex - dEQUIPMENT_SKILL_SLOT);

							if(!lpItem)
								return;

							if( iItemIndex >= 300)
							{
								cItem	*lpEquipment=	&s_lpInventoryWindowEquipmentList[iItemIndex - 300];
								
								if(!lpEquipment)
									return;

								char	*strComment	=	lpEquipment->getItemTooltip();
								s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_BOTTOM);
							}
							else
							{
								// 10.01.22 
								char		*lpStrComment = NULL;
								cItem		itemFocus;

								if(iItemIndex - dEQUIPMENT_SKILL_SLOT < dOWN_ITEM_COUNT)
								{
									lpStrComment	=	s_lpInventoryWindowItemList[iItemIndex - dEQUIPMENT_SKILL_SLOT].getItemTooltip(-1,-1,TRUE);
									memcpy(&itemFocus,&s_lpInventoryWindowItemList[iItemIndex - dEQUIPMENT_SKILL_SLOT],sizeof(cItem));			
								}
								else
								{
									lpStrComment	=	g_hero.m_aExtraInventory[iItemIndex - dEQUIPMENT_SKILL_SLOT-dOWN_ITEM_COUNT].getItemTooltip(-1,-1,TRUE);
									memcpy(&itemFocus,&g_hero.m_aExtraInventory[iItemIndex - dEQUIPMENT_SKILL_SLOT-dOWN_ITEM_COUNT],sizeof(cItem));
								}

								lpStrComment		=	g_hero.getAfterEquipInfo(lpStrComment,&itemFocus);
								s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,lpStrComment,eTFM_BOTTOM);
								
								if	(itemFocus.isItemPiece() && itemFocus.isUnknownItem() == FALSE)
								{
									cItem	pieceItem;
									
									if	(itemFocus.generatePieceItem(&pieceItem))
									{
										cBasicItem	*lpBasicItem	=	pieceItem.getBasicItem();
										WORD		*lpImage		=	g_smiIconItem.get16Image(lpBasicItem->m_wIconShape);
										
										s_ttCommon.setImage(lpImage);
									}
								}
							}
						}

						if	(s_iDragItem	==	0xffff	&&	s_iDragSkill	==	0xffff && s_bIsDragMouse &&	iFocusAbility	!=	0xffff	&&	iFocusAbility	!=	2)
						{
							s_iDragItem	 = iItemIndex - dEQUIPMENT_SKILL_SLOT;
							s_bDragSkillItem = TRUE;
							cINPDEV::SetExclusive(eMOUSE_STATE_ITEM_DRAG);

						}
						return;
					}
					if	(!lpAbility	&&	s_isLeftButtonPressed	==	FALSE)
						s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,_ms(dMSG_TOOLTIP_QUICK_SLOT_FORM,l_astrQuickSlotShotKey[iIndex-4]),eTFM_BOTTOM);

					break;
				}
			}
		}

		if (lpAbility)
		{
			if	(s_isLeftButtonPressed	==	FALSE)
			{
				cAbility	ability;

				ability.copy(lpAbility);
				ability.m_wLevel	+=	g_hero.getCorrectSkillLevel(lpAbility);

				strComment			=	ability.getTooltip(g_hero.getCorrectSkillLevel(lpAbility));

				if	(iFocusSlot	==	eSKILL_SLOT_RIGHT	||	iFocusSlot	==	eSKILL_SLOT_SUPPORT_MAGIC)
					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);
				else
				if (iFocusSlot	>=	4	&&	iFocusSlot	<=	13)
				{
					s_rectTooltip.add(0,-4);
					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_BOTTOM);
				}
				else
					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment);
			}

			if	(s_iDragItem	==	0xffff	&&	s_iDragSkill	==	0xffff && s_bIsDragMouse &&	iFocusAbility	!=	0xffff	&&	iFocusAbility	!=	2)
			{
				s_iDragSkill			=	iFocusAbility+dBORDER_OF_BOTTOM_SKILL;

				cINPDEV::SetExclusive(eMOUSE_STATE_SKILL_DRAG);
			}

			return;
		}
	}	//	스킬들의 툴팁

//	벨트 슬롯
	{
		int			iIndex,iFocusSlot=0xffff;
		cItem		*lpItem	=	NULL;

		for (iIndex=0;s_arectBeltSlot[iIndex].x1!=0xffff;iIndex++)
		{
			if (!s_arectBeltSlot[iIndex].isIn(s_posMouse.x,s_posMouse.y))
				continue;

			iFocusSlot	=	iIndex;

			s_rectTooltip.set(&s_arectBeltSlot[iIndex]);

			switch(iIndex)
			{
				case	0	:
				case	1	:
				case	2	:
				case	3	:
				case	4	:
					lpItem		=	g_hero.getBeltItem(iIndex);
					break;
			}
		}

		if (s_isLeftButtonPressed	==	FALSE)
			s_iFocusBeltSlot=	0xffff;

		if (iFocusSlot	!=	0xffff)
		{
			if (lpItem->m_wBaseItem !=	0xffff)
			{
				if (s_isLeftButtonPressed	==	FALSE)
				{
					s_rectTooltip.add(-4,0);
					strComment			=	lpItem->getItemTooltip();
					strComment			=	g_hero.getAfterEquipInfo(strComment,lpItem);

					s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);
					s_iFocusBeltSlot	=	iFocusSlot;
				}

				OperateDragItem(FALSE);
				return;
			}

			if (s_isLeftButtonPressed	==	FALSE)
			{
				s_rectTooltip.add(0,-40);

				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,dMSG_TOOLTIP_FOR_EMPTY_BELT_SLOT);
			}

			return;
		}
	}	//	벨트 슬롯

//	보석의 툴팁
	if	(s_inputChatting.isEnable()	==	FALSE)
	{
		if	(g_hero.m_wJewelTime		&&	s_isLeftButtonPressed	==	FALSE)
		{
			cRECT	rect;

			rect.set(84,l_iBottomTipPosY-4,110,512);

			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				cItem	*lpJewel	=	(cItem *)&g_hero.m_jewel;
				rect.add(0,-46);
				s_rectTooltip.set(&rect);

				strComment			=	lpJewel->getJewelTooltip(g_hero.m_wJewelTime);
				s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);
				return;
			}
		}
	}

//	각종 메뉴들의 툴팁
	{
		cBAR	*lpFocusedBar	=	s_bmBottomMenu.getFocusMenu(s_posMouse.x,s_posMouse.y);

		if (lpFocusedBar)
		{
			rect.set(&lpFocusedBar->m_rectCrash);
			rect.add(0,-40);

			switch(lpFocusedBar->m_dwMenu)
			{
				case	eBIM_RUN		:
					strComment	=	dMSG_TRANS_RUN;
					break;

				case	eBIM_WALK	:
					strComment	=	dMSG_TRANS_WALK;
					break;

				case	eBIM_TRANS	:
				{
					int	iCTR		=	g_hero.isAbleToTransformation();
					int	iAnotherJob	=	g_hero.getAnotherJob();

					if (iCTR	==	eCTR_OK)
					{
						if (iAnotherJob/2 == 1	||	iAnotherJob/2 == 2||	iAnotherJob/2 == 6||	iAnotherJob/2 == 7)
							strComment	=	_ms(dMSG_TRANSFORMATION_FORM,g_pstrHeroJobName[iAnotherJob]);
						else
							strComment	=	_ms(dMSG_JOB_CHANGE_FORM,g_pstrHeroJobName[iAnotherJob]);
					}
					else
					{
						switch(iCTR)
						{
							case	eCTR_IS_NOT_TRANSFORMATION_JOB					:
								strComment	=	dMSG_NOT_TRANSFORMATION_JOB;
								break;
							case	eCTR_IS_NOT_TRANSFORMATION_TIME					:
								strComment	=	dMSG_IS_NOT_TRANSFORMATION_TIME;
								break;
							case	eCTR_CAN_NOT_TRANSFORMATION_IN_SITDOWN_STATUS	:
								strComment	=	dMSG_CAN_NOT_TRANSFORMATION_IN_SITDOWN_STATUS;
								break;
							case	eCTR_IS_REQUIRE_TRANSFORMATION_CP				:
								strComment	=	_ms(dMSG_IS_REQUIRE_TRANSFORMATION_CP_FORM,c_aMinimumCP[dLOW_CP_LEVEL_FOR_TRANSFORMATIION-1]/100);
								break;

							case	eCTR_REQUIRE_SECOND_JOB_SKILL					:
								strComment	=	_ms(dMSG_REQUIRE_SECOND_JOB_SKILL_FORM,g_pstrHeroJobName[iAnotherJob]);
								break;
						}

					}
					break;
				}

				case	eBIM_SITDOWN	:
					strComment	=	dMSG_SITDOWN;
					break;

				case	eBIM_STAND_UP:
					strComment	=	dMSG_STANDUP;
					break;

				default				:
					strComment	=	lpFocusedBar->getTooltip();
					break;
			}

			if	(strComment	&&	s_isLeftButtonPressed	==	FALSE)
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strComment);

			return;
		}
	}	//	각종 메뉴들의 툴팁

//	HP
	{
		rect.set(329+g_iCorrectBI_X,543+g_iCorrectBI_Y,477+g_iCorrectBI_X,579+g_iCorrectBI_Y);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			int	iHP	=	max(g_hero.m_iHP/100,1);

			if	(g_hero.m_iHP	==	0)
				iHP	=	0;

			char	strHPText[256];

			if	(g_lpHero->m_iFirstAidHP)
				sprintf(strHPText,"HP <c:LTPINK>%d<n>[<c:LTGREEN>%d<c:LTRED>+%d<n>]/<c:LTYELLOW>%d<n>",g_lpHero->m_iFirstAidHP/100+iHP,iHP,g_lpHero->m_iFirstAidHP/100,g_hero.getMaxHP()/100);
			else
				sprintf(strHPText,"HP <c:LTGREEN>%d<n>/<c:LTYELLOW>%d<n>",iHP,g_hero.getMaxHP()/100);
			int	iAddPoint	=	(g_hero.m_bf4CH5HPBonusCount -g_hero.m_bf4CH5PeneltyHPBonusCount)*10;

			if	(g_hero.m_bf4CH5HPBonusCount && g_hero.m_bf4CH5PeneltyHPBonusCount)
				strcat(strHPText,_ms(dMSG_RECEIVE_CH5_HP_POINT_BONUS_PENELTY_COUNT_FORM,g_hero.m_bf4CH5HPBonusCount,g_hero.m_bf4CH5PeneltyHPBonusCount,iAddPoint));
			else
			if	(g_hero.m_bf4CH5HPBonusCount)
				strcat(strHPText,_ms(dMSG_RECEIVE_CH5_HP_POINT_BONUS_COUNT_FORM,g_hero.m_bf4CH5HPBonusCount,iAddPoint));
			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strHPText);

			return;
		}
	}

//	CP
	{
		rect.set(329+g_iCorrectBI_Y,583+g_iCorrectBI_X,514+g_iCorrectBI_X,598+g_iCorrectBI_Y);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y) && g_hero.m_bf4CH5CPBonusCount)
		{
			char	strCPText[256];

			int	iAddPoint	=	(g_hero.m_bf4CH5CPBonusCount - g_hero.m_bf4CH5PeneltyCPBonusCount)*10;

			if	(g_hero.m_bf4CH5CPBonusCount && g_hero.m_bf4CH5PeneltyCPBonusCount)
				sprintf(strCPText,_ms(dMSG_RECEIVE_CH5_CP_POINT_BONUS_PENELTY_COUNT_FORM,g_hero.m_bf4CH5CPBonusCount,g_hero.m_bf4CH5PeneltyCPBonusCount,iAddPoint));
			else
			if	(g_hero.m_bf4CH5CPBonusCount)
				sprintf(strCPText,_ms(dMSG_RECEIVE_CH5_CP_POINT_BONUS_COUNT_FORM,g_hero.m_bf4CH5CPBonusCount,iAddPoint));

			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,strCPText);

			return;
		}
	}

//	EXP
	{
		rect.set(328+g_iCorrectBI_X,534+g_iCorrectBI_Y,400+g_iCorrectBI_X,542+g_iCorrectBI_Y);	//	EXP

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			rect.add(0,-40);

			if	(g_hero.m_iExperience < 0)
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,_ms("%s <c:LTRED>%d<n>",dMSG_CURRENT_EXP,g_hero.m_iExperience));
			else
			{
				float	fExpPercent	=	100 - g_hero.getRemainExpForLevelUp() / float(g_hero.getExpForLevelUp()) * 100;

				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,_ms("%s <c:LTYELLOW>%.4f%s<n> (%d/%d)",dMSG_CURRENT_EXP, fExpPercent,dMSG_PERCENT,g_hero.m_iExperience,g_hero.getExpForLevelUp()));
			}

			return;
		}
	}

//	SKILL EXP
	{
		rect.set(406+g_iCorrectBI_X,534+g_iCorrectBI_Y,478+g_iCorrectBI_X,542+g_iCorrectBI_Y);	//	SKILL EXP

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			rect.add(0,-40);

			if (g_hero.getSkillPointSum() > g_hero.getSkillPointSumByLevel())
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,LTRED,dMSG_REASON_OF_CAN_NOT_INCREASE_SKILL_POINT);
			else
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,_ms("%s <c:LTYELLOW>%d<n>",dMSG_SKILL_POINT,g_hero.m_dwSkillExperience));
			return;
		}
	}

}

//
//버튼들 업데이트
BOOL
CGamePlay::UpdateBottomInterface()
{
	if	(s_bmBottomMenu.isPressed()	==	FALSE	&&	s_posMouse.y	<	g_iFieldScreenHeight)
		ReleaseExclusive(TRUE,eMOUSE_STATE_BOTTOM_INTERFACE);

	if	(g_gwWorldMap.isOpened() || g_gwHelp.isOpened() || g_gwMainQuestEvent.isOpened() || s_bIsFullScreenInterface	||	g_bossMonsterEvent.isAvail())
		return	TRUE;

	if	(s_bIsEventField)//	&&	g_hero.isOperator() == FALSE)
	{
		if	(s_inputChatting.isEnable() == FALSE)
			if	(s_aTouched[KEY_C])
				g_hero.sendSitdown();

		if	(s_aTouched[KEY_F1])
			g_gwHelp.open();

		return TRUE;
	}

	if	(s_bIsHideInterface)
		return	TRUE;
//	주인공 상태 관련 업뎃
	{
		//	뛰기/걷기
		if	(g_hero.m_bf1IsRunning)
		{
			if (s_bmBottomMenu.getIndex(eBIM_WALK)==0xffff) s_bmBottomMenu.resetImageBar(eBIM_RUN,"",eBIM_WALK,325+g_iCorrectBI_X,514+g_iCorrectBI_Y,eBII_MOVE_RUN_NORMAL,eBII_MOVE_RUN_ACTIVE,eBII_MOVE_RUN_PRESSED);
		}
		else
		{
			if (s_bmBottomMenu.getIndex(eBIM_RUN) ==0xffff) s_bmBottomMenu.resetImageBar(eBIM_WALK,"",eBIM_RUN,325+g_iCorrectBI_X,514+g_iCorrectBI_Y,eBII_MOVE_WALK_NORMAL,eBII_MOVE_WALK_ACTIVE,eBII_MOVE_WALK_PRESSED);
		}

		//	앉기/서기
		if (g_lpHero && g_lpHero->isSitdown())
		{
			if (s_bmBottomMenu.getIndex(eBIM_STAND_UP)==0xffff)
				s_bmBottomMenu.resetImageBar(eBIM_SITDOWN,"",eBIM_STAND_UP,429+g_iCorrectBI_X,514+g_iCorrectBI_Y,eBII_SITDOWN_NORMAL,eBII_SITDOWN_ACTIVE,eBII_SITDOWN_PRESSED,0xffff,eBII_SITDOWN_DISABLE);
		}
		else
		{
			if (s_bmBottomMenu.getIndex(eBIM_SITDOWN) ==0xffff)
				s_bmBottomMenu.resetImageBar(eBIM_STAND_UP,"",eBIM_SITDOWN,429+g_iCorrectBI_X,514+g_iCorrectBI_Y,eBII_STAND_NORMAL,eBII_STAND_ACTIVE,eBII_STAND_PRESSED,0xffff,eBII_STAND_DISABLE);
		}

		//	변신 가능?
		if (g_hero.isAbleToTransformation()	==	eCTR_OK)
			s_bmBottomMenu.enable(eBIM_TRANS);
		else
			s_bmBottomMenu.disable(eBIM_TRANS);

		if (g_hero.isAbleToSitdown())
			s_bmBottomMenu.enable(eBIM_SITDOWN);
		else
			s_bmBottomMenu.disable(eBIM_SITDOWN);

		s_bmBottomMenu.setKey(eBIM_TRANS,KEY_X);
		s_bmBottomMenu.setKey(eBIM_SITDOWN,KEY_C);
		s_bmBottomMenu.setKey(eBIM_STAND_UP,KEY_C);
	}

	if	(s_bIsHideInterface	==	FALSE)
		OperateBottomInterfaceTooltip();

	if	(IsOpenedEventWindow()								)
		return	FALSE;

	if (g_config.m_bf1IsUseOldStyleShotKey)
	{
		if (s_aKey[KEY_F9])
			g_gwHelp.open();
	}
	else
	{
		if (s_aKey[KEY_F1])
			g_gwHelp.open();
	}

	DWORD	dwBottomButtonMenu = 0xffffffff;

	int	iIsOldStyle		=	g_config.m_bf1IsUseOldStyleShotKey;

	{
		s_bmBottomMenu.setKey(eBIM_MENU_STATUS			,KEY_F2-iIsOldStyle);
		s_bmBottomMenu.setKey(eBIM_MENU_INVENTORY		,KEY_F3-iIsOldStyle);
		s_bmBottomMenu.setKey(eBIM_MENU_SKILL			,KEY_F4-iIsOldStyle);
		s_bmBottomMenu.setKey(eBIM_MENU_PARTY			,KEY_F5-iIsOldStyle);
		s_bmBottomMenu.setKey(eBIM_MENU_QUEST			,KEY_F6-iIsOldStyle);
		s_bmBottomMenu.setKey(eBIM_MENU_GUILD			,KEY_F7-iIsOldStyle);
		s_bmBottomMenu.setKey(eBIM_MENU_FRIEND			,KEY_F8-iIsOldStyle);
		s_bmBottomMenu.setKey(eBIM_MENU_SYSTEM			,KEY_F9-iIsOldStyle);
	}

	if (s_inputChatting.isEnable())
	{
		dwBottomButtonMenu	=	s_bmBottomMenu.update(s_posMouse.x,s_posMouse.y,NULL,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (s_aTouched[KEY_F2-iIsOldStyle])	dwBottomButtonMenu	=	eBIM_MENU_STATUS;
		if (s_aTouched[KEY_F3-iIsOldStyle])	dwBottomButtonMenu	=	eBIM_MENU_INVENTORY;
		if (s_aTouched[KEY_F4-iIsOldStyle])	dwBottomButtonMenu	=	eBIM_MENU_SKILL;
		if (s_aTouched[KEY_F5-iIsOldStyle])	dwBottomButtonMenu	=	eBIM_MENU_PARTY;
		if (s_aTouched[KEY_F6-iIsOldStyle])	dwBottomButtonMenu	=	eBIM_MENU_QUEST;
		if (s_aTouched[KEY_F7-iIsOldStyle])	dwBottomButtonMenu	=	eBIM_MENU_GUILD;
		if (s_aTouched[KEY_F8-iIsOldStyle])	dwBottomButtonMenu	=	eBIM_MENU_FRIEND;
		if (s_aTouched[KEY_F9-iIsOldStyle])	dwBottomButtonMenu	=	eBIM_MENU_SYSTEM;
	}
	else if(!s_pmCommon.isPopuped() &&!s_aKey[KEY_LCONTROL])
		dwBottomButtonMenu	=	s_bmBottomMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	if	(g_gwShop.isOpened()+IsOpenTradeBox()+g_gwGuildBattleScheduler.isOpened() + g_gwMakeGuild.isOpened() + g_gwGuildMarkComposer.isOpened()+
		g_gwGuildHallList.isOpened()+g_gwGuildRelatedPlaceList.isOpened())
		if (dwBottomButtonMenu	>=	eBIM_MENU_STATUS	&&	dwBottomButtonMenu	<=	eBIM_MENU_SYSTEM)
		{
			dwBottomButtonMenu	=	0xffffffff;
			for(int i=0;i<8;i++)
				s_bmBottomMenu.check(eBIM_MENU_STATUS+i,FALSE);
		}

	if (dwBottomButtonMenu	!=	0xffffffff)
		switch(dwBottomButtonMenu)
		{
			case	eBIM_RUN				:
			case	eBIM_WALK				:
				g_hero.changeMoveAbility();
				break;

			case	eBIM_MENU_STATUS		:
				OpenStatusWindow();
				break;

			case	eBIM_MENU_INVENTORY		:	//	12,13,14
				OpenInventoryWindow();
				break;

			case	eBIM_MENU_SKILL			:
				g_gwSkill.open();
				break;

			case	eBIM_MENU_QUEST			:
				g_gwQuest.open();
				break;

			case	eBIM_MENU_FRIEND		:
				g_gwFriend.open();
				break;

			case	eBIM_MENU_GUILD			:
				g_gwGuild.open();
				break;

			case	eBIM_MENU_PARTY			:
				g_gwParty.open();
				break;

			case	eBIM_MENU_SYSTEM			:
				PopupSystemMenu();
				break;

			case	eBIM_TRANS				:
				g_hero.sendTransformation();
				break;

			case	eBIM_SITDOWN				:
			case	eBIM_STAND_UP			:
				g_hero.sendSitdown();
				break;

			case	eBIM_QUICK_SKILL1		:
			case	eBIM_QUICK_SKILL2		:
			case	eBIM_QUICK_SKILL3		:
			case	eBIM_QUICK_SKILL4		:
			case	eBIM_QUICK_SKILL5		:
			case	eBIM_QUICK_SKILL6		:
			case	eBIM_QUICK_SKILL7		:
			case	eBIM_QUICK_SKILL8		:
			case	eBIM_QUICK_SKILL9		:
			case	eBIM_QUICK_SKILL10		:
				{

					if(s_bmBottomMenu.m_bIsActionedByHotKey	==	FALSE	&&	!s_bmBottomMenu.isClickedPos(s_posMouse.x,s_posMouse.y))	
						break;
					if(g_hero.m_dwQuickSlotDelayTime <= timeGetTime())
					{
						BOOL bIsOk = FALSE;
						bIsOk = g_hero.useQuickSlotSkill(dwBottomButtonMenu-eBIM_QUICK_SKILL1);
						if(bIsOk)
							g_hero.m_dwQuickSlotDelayTime = timeGetTime() + dQUICK_SLOT_DELAY;

					}
					break;
				}
			case	eBIM_QUICK_SKILL_UP		:
				g_hero.m_wCurrentQuickSkillSlot++;
				if	(g_hero.m_wCurrentQuickSkillSlot	>=	10)
					g_hero.m_wCurrentQuickSkillSlot	=	0;
				break;
			case	eBIM_QUICK_SKILL_DOWN	:
				if	(g_hero.m_wCurrentQuickSkillSlot	<=	0)
					g_hero.m_wCurrentQuickSkillSlot	=	9;
				else
					g_hero.m_wCurrentQuickSkillSlot--;
				break;
			case	eBIM_LEFT_SKILL1			:
				break;
			case	eBIM_LEFT_SKILL2			:
				g_hero.m_bf1IsSubAttackAbilityOn	=	1-	g_hero.m_bf1IsSubAttackAbilityOn;
				break;
			case	eBIM_RIGHT_SKILL1			:
				break;
			case	eBIM_RIGHT_SKILL2			:
				break;

			case	eBIM_QUICK_ITEM1			:
			case	eBIM_QUICK_ITEM2			:
			case	eBIM_QUICK_ITEM3			:
			case	eBIM_QUICK_ITEM4			:
			case	eBIM_QUICK_ITEM5			:
			{
				if	(s_bmBottomMenu.m_bIsActionedByHotKey	==	FALSE	&&	!s_bmBottomMenu.isClickedPos(s_posMouse.x,s_posMouse.y))
					break;

				if	(!s_aKey[KEY_LCONTROL]) 
					g_hero.sendUseBeltItem(dwBottomButtonMenu-eBIM_QUICK_ITEM1); //KH_2
				break;
			}

			case	eBIM_QUICK_ITEM_RELOAD	:
				g_hero.sendBeltItemReload();
				break;
		}

	if (s_iWheelValue	&&	l_rectQuickSkillSlot.isIn(s_posMouse.x,s_posMouse.y))
	{
		if	(s_iWheelValue	<	0)
		{
			if	(g_hero.m_wCurrentQuickSkillSlot	==	0)
				g_hero.m_wCurrentQuickSkillSlot	=	9;
			else
				g_hero.m_wCurrentQuickSkillSlot--;
		}
		if (s_iWheelValue	>	0)
		{
			g_hero.m_wCurrentQuickSkillSlot++;
			if (g_hero.m_wCurrentQuickSkillSlot	>=	10)
				g_hero.m_wCurrentQuickSkillSlot	=	0;
		}
	}

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_BOTTOM_INTERFACE))	
		return	FALSE;

//	인터페이스 하단 밑에서 마우스를 누르고 있으면서, 락이 안걸렸다. -_-a
	if	(s_bmBottomMenu.isPressed()	||	s_posMouse.y	>=	g_iFieldScreenHeight)
	{
		int	iFocusBelt	=	s_iFocusBeltSlot;

		ResetAllFocus();	//	모든 포커스 제거

		s_iFocusBeltSlot=	iFocusBelt;

		cINPDEV::SetExclusive(eMOUSE_STATE_BOTTOM_INTERFACE);
		return	TRUE;
	}

	if	(s_inputChatting.isEnable())
	{
		if	(s_posMouse.y	>=	g_iFieldScreenHeight-20)
		{
			ResetAllFocus();	//	모든 포커스 제거

			cINPDEV::SetExclusive(eMOUSE_STATE_BOTTOM_INTERFACE);
			return	TRUE;
		}
	}


	cINPDEV::ReleaseExclusive();

	return	FALSE;
}


void
CGamePlay::operateScreenEffect()
{
	if(g_hero.m_wBlackScreenCount)		//화면 흑백 효과가 나타나는 시간.
		cDRAW::PutScreenReferenceBlackEffect();

	if(g_hero.m_wTVScreenCount)			//TV 지지직 효과...(어따 쓸지는 모르겟지만.)
		cDRAW::PutScreenReferenceTvEffect(8);

	//cDRAW::putMagniFinder(s_posMouse.x , s_posMouse.y);
	//g_sprInterface2.Put(100 , 200, 187);
}

//
//하단 인터페이스 그려주기
void
CGamePlay::DrawBottomInterface()
{
	if	(s_bIsHideInterface)
		return;

	if	(g_gwWorldMap.isOpened())
		return;

	int i;

	if	(g_bossMonsterEvent.isAvail()	==	FALSE)
	{
		if	(g_fireCrackerManager.isActive())
		{

			if	(s_inputChatting.isEnable())
				s_bIsBookedDisableChattingBox	=	TRUE;
			else
			{
				int	iSize	=	g_iScreenWidth/2-50;
				
				cDRAW::FillHB(0,g_iScreenWidth/2-iSize,l_iBottomTipPosY-4,g_iScreenWidth/2+iSize,514);

				if	(g_fireCrackerManager.m_iRemainFireWorkTime	== 0 &&	g_fireCrackerManager.m_iUpkeepFireWorkTime)
					s_bText.cPut(g_iScreenWidth/2,l_iBottomTipPosY,WHITE,dMSG_WAIT_FOR_FIRST_FIREWORK);
				else
					s_bText.cPut(g_iScreenWidth/2,l_iBottomTipPosY,WHITE,dMSG_FIREWORK_REMAIN_TIME_FORM,g_fireCrackerManager.m_iRemainFireWorkTime,g_fireCrackerManager.m_iRemainFireWorkCount);
			}
		}
		else
		if	(!s_inputChatting.isEnable())
		{
			BOOL	bIsPutStatus	=	FALSE;

			if	(g_hero.m_wIsReadyToFireChargedMiniPetSkill+g_hero.m_wIsReadyToFireChargedMiniPetSkill2)
			{
				int	iValue	=	s_iFrameCounter%(dSYNC_FPS*2);

				int	iSize	=	g_iScreenWidth/2-50;

				cDRAW::FillHB(0,g_iScreenWidth/2-iSize,l_iBottomTipPosY-4,g_iScreenWidth/2+iSize,514);

				if	(iValue > dSYNC_FPS/3)
				{
					char	strText[128];

					if	(g_hero.m_wIsReadyToFireChargedMiniPetSkill==TRUE	&&	g_hero.m_wIsReadyToFireChargedMiniPetSkill2==TRUE)
						sprintf(strText,dMSG_CHARGE_MINIPET_SKILL_FORM,"<c:LTGREEN>[V],[B]<n>");
					if	(g_hero.m_wIsReadyToFireChargedMiniPetSkill==TRUE	&&	g_hero.m_wIsReadyToFireChargedMiniPetSkill2==FALSE)
						sprintf(strText,dMSG_CHARGE_MINIPET_SKILL_FORM,"<c:LTGREEN>[V]<c:GRAY>,[B]<n>");
					if	(g_hero.m_wIsReadyToFireChargedMiniPetSkill==FALSE	&&	g_hero.m_wIsReadyToFireChargedMiniPetSkill2==TRUE)
						sprintf(strText,dMSG_CHARGE_MINIPET_SKILL_FORM,"<c:GRAY>[V],<c:LTGREEN>[B]<n>");

					s_bText.cPut(g_iScreenWidth/2,l_iBottomTipPosY,WHITE,strText);
				}

				bIsPutStatus	=	TRUE;
			}

			if	(g_hero.m_wJewelTime)
			{
				int	iJewelImage	=	(s_iFrameCounter*g_anmJewel.GetFPS(0)/dSYNC_FPS)%g_anmJewel.GetFrameCount(0);

				int iAddX = 0;
				int iAddY = 0;
				if (g_iScreenWidth != 800)
				{
					iAddX = g_iScreenWidth-800;
					iAddY = g_iScreenHeight-600;
				}
				
				g_anmJewel.PutReg(98+iAddX,502+iAddY,0,0,iJewelImage);

				bIsPutStatus	=	TRUE;
			}

			if	(bIsPutStatus	==	FALSE	&&	g_config.m_bf1IsHideTip	==	FALSE && g_loaderText.m_iTipCount)
			{
				int	iValue	=	s_iFrameCounter/(dSYNC_FPS/4);

				if	((iValue%120) < 34)
				{
					int	iSize	=	g_iScreenWidth/2-50;

					if	((iValue%120) == 32)
						cDRAW::FillHB(0,g_iScreenWidth/2-iSize,l_iBottomTipPosY-4,g_iScreenWidth/2+iSize,514);
					else
					if	((iValue%120) == 33)
					{
						int	iAlpha	=	s_iFrameCounter%(dSYNC_FPS/4);

						iAlpha		=	16-16*iAlpha/(dSYNC_FPS/4);

						cDRAW::FillAB(0,g_iScreenWidth/2-iSize,l_iBottomTipPosY-4,g_iScreenWidth/2+iSize,514,iAlpha);
					}
					else
					{
						cDRAW::FillHB(0,g_iScreenWidth/2-iSize,l_iBottomTipPosY-4,g_iScreenWidth/2+iSize,514);

						if	((iValue%120)%8 > 0)
						{
							iValue	=	(iValue/120)%g_loaderText.m_iTipCount;

							s_bText.cPut(g_iScreenWidth/2+g_iCorrectBI_X/2,l_iBottomTipPosY,WHITE,g_pstrTip[iValue]);
						}
					}

				}
			}
		}
	}

	g_sprGameBottomInterface.Put(g_iCorrectBI_X,g_iScreenHeight,eBII_MAIN);

	if	(s_inputChatting.isEnable() == FALSE)
	{
		int	iXPos					=	696+g_iCorrectBI_X;
		int	iYPos					=	500+g_iCorrectBI_Y;
		int	iReactionSlot			=	g_hero.m_wJob%2;
		int	iScale					=	50;
		int	iSize					=	dITEM_SHAPE_WIDTH*iScale/100;
		int	iActiveReactionCount	=	0;


		for (int i=0;i<dACTIVE_REACTION_COUNT;i++)
		{
			BOOL	bIsActivate	=	FALSE;
			int		iAbility	=	g_hero.m_aActivePassiveSkill[iReactionSlot][i];

			if	(iAbility		>=	dABILITY_COUNT)
				continue;

			cAbility *lpAbility	=	&g_hero.m_aAbility[iAbility];
			CSkill	 *lpSkill	=	lpAbility->getSkill();

			if	(!lpSkill)
				continue;
			if	(lpSkill->m_wCastMethod	==	eST_PASSIVE)
				bIsActivate	=	lpAbility->isActivatePassiveSkill(g_hero.m_iCP);
			if	(lpSkill->m_wCastMethod	==	eST_REACTION)
				bIsActivate	=	lpAbility->isActivateReactionSkill(GetCPLevel(g_hero.m_iCP));

			if	(lpSkill->m_bf2RequireSummonBeastGrade)
			{
				cSummonBeast	*lpBeast	=	g_hero.getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast);

				if	(!lpBeast || lpBeast->m_bf2Grade	<	lpSkill->m_bf2RequireSummonBeastGrade-1)
					bIsActivate	=	FALSE;
			}
			
			g_smiIconSkill.putScale(lpSkill->m_wIconIndex	,iXPos,iYPos,iScale);
			
			if	(bIsActivate)
			{
				int	iAlpha	=	s_iFrameCounter/3%32;
				
				if (iAlpha	>=	16)	iAlpha	-=	(iAlpha%16)*2;
				
				cDRAW::FillAB(_WHITE,iXPos,iYPos,iXPos+iSize,iYPos+iSize,iAlpha);
			}
			else
				cDRAW::FillAB(0,iXPos,iYPos,iXPos+iSize,iYPos+iSize,16);
			
			if(iActiveReactionCount)
				cDRAW::YLine(_YELLOW,iYPos,iYPos+iSize,iXPos + iActiveReactionCount*iSize,2);
			iXPos	-=	(iSize+2);
			iActiveReactionCount++;
		}

		if (iActiveReactionCount)
		{
			cRECT	rect;
	
			int iX	=	iXPos+iSize-2;
			rect.set(iX,iYPos-3,iX+(iSize+2)*iActiveReactionCount+5,iYPos+iSize+2);
			s_ttCommon.drawFrame(&rect,FALSE);
		}

		//GGG 버프 그려주기.
		iXPos					=	120+g_iCorrectBI_X;
		iYPos					=	497+g_iCorrectBI_Y;
		int		iIsActiveGGG	=	0;

		if	(iIsActiveGGG)
			iXPos += 17;

		if(g_hero.m_GGG.m_wGGGTimeForSkillUp)
		{
			if	(g_hero.m_bf3GreateGodGrace)
			{
				g_smiIconGGG.putScale(dGGG_ICON_START_INDEX + (g_hero.m_bf3GreateGodGrace-1) * 3 + 1,iXPos,iYPos);
				//g_smiIconGGG.putScale(eGGG_ICON_FIRE_SKILL,iXPos,iYPos,100);
				
				int	iAlpha	=	s_iFrameCounter/3%32;
				
				if (iAlpha	>=	16)	iAlpha	-=	(iAlpha%16)*2;
				
				cDRAW::FillAB(_WHITE,iXPos,iYPos,iXPos+iSize,iYPos+iSize,iAlpha);

				iIsActiveGGG++;
			}
		}

		if(iIsActiveGGG)
		{
			cRECT	rect;
			
			rect.set(120 - 3+g_iCorrectBI_X ,iYPos -1+g_iCorrectBI_Y,120+iSize*(iIsActiveGGG) + 2,iYPos+iSize + 1);
			
			s_ttCommon.drawFrame(&rect,FALSE);
		}
	}

	s_bmBottomMenu.draw();

	if	(s_partyInfo.isExistNewRequestJoinPartyUser())
		if	((timeGetTime()%1000)	<	500)
			g_sprGameBottomInterface.Put(670,562,eBII_MENU_PARTY_ACTIVE);

	if	(g_gwHelp.isOpened())
		return;


//	스킬 관련
	{
		cAbility	*lpLeftAbility	=	g_hero.getLeftAbility();
		cAbility	*lpRightAbility	=	g_hero.getRightAbility();
		cAbility	*lpSupportAbility=	g_hero.getSupportAbility();
		cAbility	*lpSubAbility	=	g_hero.getSubAbility();

		if	(lpLeftAbility	&&	lpLeftAbility->getSkill()	==	NULL)
			lpLeftAbility	=	NULL;
		if	(lpRightAbility	&&	lpRightAbility->getSkill()	==	NULL)
			lpRightAbility	=	NULL;
		if	(lpSupportAbility&&	lpSupportAbility->getSkill()	==	NULL)
			lpSupportAbility=	NULL;
		if	(lpSubAbility	&&	lpSubAbility->getSkill()	==	NULL)
			lpSubAbility	=	NULL;

		if	(lpLeftAbility	)
		{
			if	(lpLeftAbility->m_wLevel	==	0)
				g_hero.setLeftAbility(0xffff);
			else
			{
				int		iX			=	s_arectSkillSlot[0].x1;
				int		iY			=	s_arectSkillSlot[0].y1;
				g_smiIconSkill.put(lpLeftAbility->getSkill()->m_wIconIndex	,s_arectSkillSlot[0].x1,s_arectSkillSlot[0].y1);

				if	(g_hero.isUseAbleSkill(g_hero.m_wLeftAbility)	!=	eUSR_SUCCESS)
				{
					cDRAW::FillAB(0,s_arectSkillSlot[0].x1,iY,iX+dITEM_SHAPE_WIDTH,iY+dITEM_SHAPE_WIDTH,16);
				}
			}
		}
		if	(lpRightAbility	)
		{
			if	(lpRightAbility->m_wLevel	==	0)
				g_hero.setRightAbility(0xffff);
			else
			{
				BOOL	bIsUseAble	=	FALSE;
				int		iX			=	s_arectSkillSlot[1].x1;
				int		iY			=	s_arectSkillSlot[1].y1;
				if	(g_hero.isUseAbleSkill(g_hero.m_wRightAbility)	==	eUSR_SUCCESS)
				{
					int	iRequireCP	=	0;

					cAbility	rightAbility;

					rightAbility.copy(lpRightAbility);
					rightAbility.m_wLevel	+=	g_hero.getCorrectSkillLevel(lpRightAbility);

					if	(g_hero.m_wJob==dJOB_WEREWOLF||g_hero.m_wJob==dJOB_FALLEN_ANGEL||g_hero.m_wJob==dJOB_MAGICAL_GIRL||g_hero.m_wJob==dJOB_DEVIL	)
						iRequireCP	=	rightAbility.getSpentCP();

					if	(g_hero.m_iCP	>=	iRequireCP+g_hero.getReleaseTrasformationCp() || g_hero.m_wMadnessTime)	// 폭주상태일때 사용가능체크..
						bIsUseAble	=	TRUE;
				}

				g_smiIconSkill.put(lpRightAbility->getSkill()->m_wIconIndex	,iX,iY);
				if	(bIsUseAble	==	FALSE)
					cDRAW::FillAB(0,iX,iY,iX+dITEM_SHAPE_WIDTH,iY+dITEM_SHAPE_WIDTH,16);
			}

		}
		if	(lpSubAbility	)
		{
			if	(lpSubAbility->m_wLevel	==	0)
				g_hero.setSubAbility(0xffff);
			else
			{
				int		iX			=	s_arectSkillSlot[2].x1;
				int		iY			=	s_arectSkillSlot[2].y1;
				BOOL	bIsUseAble	=	FALSE;

				if	(g_hero.isUseAbleSkill(g_hero.m_wSubAbility)	==	eUSR_SUCCESS)
				{
					int	iRequireCP	=	0;

					cAbility	subAbility;

					subAbility.copy(lpSubAbility);
					subAbility.m_wLevel	+=	g_hero.getCorrectSkillLevel(lpSubAbility);

					if	(g_hero.m_wJob==dJOB_WEREWOLF||g_hero.m_wJob==dJOB_FALLEN_ANGEL||g_hero.m_wJob==dJOB_MAGICAL_GIRL||g_hero.m_wJob==dJOB_DEVIL	)
						iRequireCP	=	subAbility.getSpentCP();

					if	(g_hero.m_iCP	>=	iRequireCP+g_hero.getReleaseTrasformationCp() || g_hero.m_wMadnessTime)	// 폭주상태일때 사용가능체크..
						bIsUseAble	=	TRUE;

				}

				g_smiIconSkill.put(lpSubAbility->getSkill()->m_wIconIndex	,iX,iY);

				if	(bIsUseAble	==	FALSE)
					cDRAW::FillAB(0,iX,iY,iX+dITEM_SHAPE_WIDTH,iY+dITEM_SHAPE_WIDTH,16);

				if	(g_hero.m_bf1IsSubAttackAbilityOn)
				{
					g_sprInterface.Put(s_arectSkillSlot[2].x1+23,s_arectSkillSlot[2].y1+23,eREACTION_SKILL_ON_S);

					if	(s_iFrameCounter%dSYNC_FPS < dSYNC_FPS/2)
						g_sprInterface.Put(s_arectSkillSlot[2].x1+23,s_arectSkillSlot[2].y1+23,eREACTION_SKILL_ON_S,100,100,dPUT_SOFT_DODGE);
				}
				else
					g_sprInterface.Put(s_arectSkillSlot[2].x1+23,s_arectSkillSlot[2].y1+23,eREACTION_SKILL_OFF_S);
			}
		}

		if	(lpSupportAbility)
		{
			if	(lpSupportAbility->m_wLevel	==	0)
				g_hero.setSupportAbility(0xffff);
			else
				g_smiIconSkill.put(lpSupportAbility->getSkill()->m_wIconIndex,s_arectSkillSlot[3].x1,s_arectSkillSlot[3].y1);
		}

		for (int i=0;i<10;i++)
		{

			// 10.01.07 스킬슬롯에 있는 아이템 출력
			int Index = g_hero.m_aQuickSkill[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2];

			if(Index >= dEQUIPMENT_SKILL_SLOT && Index <= 400 )
			{
				CPos	pos;
				pos.Set(s_arectSkillSlot[4+i].x1,s_arectSkillSlot[4+i].y1);
								
				if(g_hero.m_sQuickSlotItem[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2].IconShape <= dBASIC_ITEM_COUNT)
				{
					g_smiIconItem.put(g_hero.m_sQuickSlotItem[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2].IconShape,pos.x,pos.y);
					
					if(g_hero.m_sQuickSlotItem[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2].UmItem)
					{
						int		iFrame	=	(s_iFrameCounter*g_anmItemType.GetFPS(19)/dSYNC_FPS)%g_anmItemType.GetFrameCount(19);
						g_anmItemType.PutReg(pos.x+18,pos.y+24,19,0,iFrame);

					}
					else if(g_hero.m_sQuickSlotItem[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2].DxItem)
					{
						int		iFrame	=	(s_iFrameCounter*g_anmItemType.GetFPS(4)/dSYNC_FPS)%g_anmItemType.GetFrameCount(4);
						g_anmItemType.PutReg(pos.x+18,pos.y+24,4,0,iFrame);
					}

					// 10.01.28 추가
					cItem	*lpItem	= g_hero.getInventoryItem(Index - dEQUIPMENT_SKILL_SLOT);

					if(lpItem)
					{
						if(!g_hero.isUseAbleItem(lpItem))
							g_hero.m_sQuickSlotItem[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2].NotUseItem = TRUE;
						else
							g_hero.m_sQuickSlotItem[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2].NotUseItem = FALSE;
						
						if(g_hero.m_sQuickSlotItem[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2].NotUseItem)
							cDRAW::FillAB(_LTRED,pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_HEIGHT,12);
					}
					else
					{
						if(g_hero.m_sQuickSlotItem[g_hero.m_wCurrentQuickSkillSlot][i][(g_hero.m_wJob)%2].NotUseItem)
							cDRAW::FillAB(_LTRED,pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_HEIGHT,12);
					}
					g_sprInterface2.Put(pos.x,pos.y,2+i);
				}
				continue;
			}

			cAbility	*lpAbility	=	g_hero.getQuickSkill(i);

			if	(!lpAbility)
				continue;

			CSkill	*lpSkill	=	lpAbility->getSkill();

			if	(!lpSkill	||	lpAbility->m_wLevel	==	0)
			{
				g_hero.setQuickSlotAbility(0xffff,i);
				continue;
			}

			int		iAbility	=	g_hero.getAbilityIndexBySkill(lpAbility->m_wSkill);
			CPos	pos;

			pos.Set(s_arectSkillSlot[4+i].x1,s_arectSkillSlot[4+i].y1);

			g_smiIconSkill.put(lpSkill->m_wIconIndex,pos.x,pos.y);

			if	(lpSkill->isPassiveSkill())
			{
				if	(g_hero.isActiveReaction(iAbility))
					g_sprInterface.Put(pos.x+23,pos.y+23,eREACTION_SKILL_ON_S);
				else
					g_sprInterface.Put(pos.x+23,pos.y+23,eREACTION_SKILL_OFF_S);
			}


			if	(g_config.m_bIsDisplayShotKey)
			{
				///////////////////////////////////////////////////////////////////////
				///////////////////// KH_4  슬롯 상태//////////////////////////////////
				///////////////////////////////////////////////////////////////////////



				 BOOL bStateUseSkill = eNOT_USE_SKILL;
				
				if	((lpSkill->m_wDifficultyLevel	==	1	&&	lpAbility->getSpentCP() == 0)	||	g_hero.m_wMadnessTime || (g_hero.m_iCP/100) >= (c_aMinimumCP[lpSkill->m_wDifficultyLevel-1]/100) ) 					 
				{	// 폭주상태일때도..
					if( g_hero.isUseAbleSkill(iAbility) ==	eUSR_SUCCESS )
					{

						bStateUseSkill = eUSE_SKILL_NOT_TARGET;
						CActor	*lpActor	=	g_am.getTestedActor(s_iFocusActor,FALSE);
						cItem	*lpWeapon	=	g_hero.getWeapon();
						int		iAttackRange=	g_hero.getAttackRange(lpAbility) + g_lpHero->getBodySize();	//	공격범위 체크
						int		iMinRange	=	lpAbility->getMinimumAttackRange();	//	공격범위 체크

						//스킬 타입 분류 
						//나한테 쓰는 스킬,리액션스킬 
						if	(lpAbility->isQuickCastSkill()) 
							bStateUseSkill = eUSE_SKILL;
						//필드//나한테 쓰느 스킬 포커스 필요없다 
						else
						if	(lpAbility->isCastGroundSkill() && lpActor == NULL)
						{							
							if	(g_lpHero->isTouchAblePos(s_posCursor.x,s_posCursor.y,iAttackRange) == TRUE	&&
								!g_map.isMissileBlockedLine(g_lpHero->m_pos.x,g_lpHero->m_pos.y,s_posCursor.x,s_posCursor.y))
							{
								//소환수다!!
								if( lpSkill->m_bf2RequireSummonBeastGrade)
								{
									//소환수 떔시 체크 
									cSummonBeast	*lpBeast	=	g_hero.getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast);											

									if( lpBeast != NULL && (lpSkill->m_bf2RequireSummonBeastGrade <= lpBeast->m_bf2Grade+1))
										bStateUseSkill = eUSE_SKILL;
								}								
								else
									bStateUseSkill = eUSE_SKILL;
							}
						}

						//적 한테 쓰는 스킬 //플레이어 쓰는 스킬//포커스가 살아있따면 
						if	(lpActor != NULL || g_hero.m_wEngageTarget != 0xffff )
						{
							CActor	*lpTarget	=	&g_aActor[s_iFocusActor & g_hero.m_wEngageTarget];

							if	(lpTarget->isCastAbleSkill(lpAbility,TRUE))
							{
								if	(g_lpHero->isTouchAbleTarget(lpTarget,iAttackRange,iMinRange) == TRUE	&&
									!g_map.isMissileBlockedLine(g_lpHero->m_pos.x,g_lpHero->m_pos.y,lpTarget->m_pos.x,lpTarget->m_pos.y))
								{
									//소환수다!!
									if( lpSkill->m_bf2RequireSummonBeastGrade)
									{
										//소환수 떔시 체크 
										cSummonBeast	*lpBeast	=	g_hero.getSummonBeastByType(lpSkill->m_bf2OperateSummonBeast);

										if( lpBeast != NULL && (lpSkill->m_bf2RequireSummonBeastGrade <= lpBeast->m_bf2Grade+1))
											bStateUseSkill = eUSE_SKILL;
									}								
									else
										bStateUseSkill = eUSE_SKILL;
								}
							}
						}							 
					}
				}

				if( lpAbility->isReactionSkill())
				{
					bStateUseSkill = eUSE_SKILL;

					if(g_map.m_bf1IsNotUseSkillBeforeTrans)
					{
						if(!g_hero.m_wTransformationTime)
							bStateUseSkill = eNOT_USE_SKILL;
					}
				}
				if(g_hero.m_wTransformationTime)
					bStateUseSkill = eNOT_USE_SKILL;

				if(bStateUseSkill == eUSE_SKILL)
				{
					g_sprInterface2.Put(pos.x,pos.y,2+i);
				}
				else
					if(bStateUseSkill == eUSE_SKILL_NOT_TARGET)
					{
						g_sprInterface2.Put(pos.x,pos.y,2+i+20 );
					}
					else
					{
						WORD wCoolTime = g_hero.getCoolTime(lpAbility);
						if(wCoolTime > 0)
						{
							
							int	iWidth		=	g_sprInterface2.getSpriteWidth(eBI_NUMBER_ZERO);
							int  iCount = 0;
							int iMaxCoolTime = lpAbility->getCoolTime();
							
							
							//	cDRAW::FillHB(0,pos.x,pos.y,pos.x+(dITEM_SHAPE_WIDTH*wCoolTime/iMaxCoolTime),pos.y+dITEM_SHAPE_WIDTH);		
							
							//	cDRAW::FillHB(0,pos.x+dITEM_SHAPE_WIDTH*(iMaxCoolTime-wCoolTime)/iMaxCoolTime,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_WIDTH);
							
							//	cDRAW::FillHB(0,pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+(dITEM_SHAPE_WIDTH*wCoolTime/iMaxCoolTime));
							
							cDRAW::FillHB(0,pos.x,pos.y+dITEM_SHAPE_WIDTH*(iMaxCoolTime-wCoolTime)/iMaxCoolTime,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_WIDTH);
							
							if(wCoolTime == 1)
							{
								g_sprInterface.Put(pos.x,pos.y,eIWI_ITEM_SLOT_ANM_1+((s_iFrameCounter/8)%5));
							}
							for(int iDivision = 10000; iDivision != 0;iDivision /= 10)
							{
								lpAbility->getCoolTime();
								int  iNumber = wCoolTime/iDivision;
								wCoolTime -= iNumber*iDivision;
								if(iNumber == 0 && iCount == 0)
									continue;
								if(iNumber >= 10)
									break;
								g_sprInterface2.Put(pos.x + iWidth * iCount,pos.y+26,eBI_NUMBER_ZERO + iNumber);	//	빨간색
								++iCount;
							}
							
						}
						else
						{
							cDRAW::FillAB(0,pos.x,pos.y,pos.x+dITEM_SHAPE_WIDTH,pos.y+dITEM_SHAPE_WIDTH,16);
						}
						g_sprInterface2.Put(pos.x,pos.y,2+i+10);	//	빨간색
					}
					
			}
		}

	}




//	퀵 아이템 관련
	for (i=0;i<5;i++)
	{
		cItem	*lpItem	=	g_hero.getBeltItem(i);

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		PutItem(lpItem,s_arectBeltSlot[i].x1,s_arectBeltSlot[i].y1,2);

		if	(g_config.m_bIsDisplayShotKey)
		{
			cDRAW::Fill(0,s_arectBeltSlot[i].x1,s_arectBeltSlot[i].y1,s_arectBeltSlot[i].x1+10,s_arectBeltSlot[i].y1+10);
			s_sText.put(s_arectBeltSlot[i].x1,s_arectBeltSlot[i].y1,WHITE,"<c:LTGREEN>%d<n>",i+1);
		}
	}	//	퀵 아이템 관련


//	현재 퀵스킬 슬롯 인덱스
	g_sprGameBottomInterface.Put(283+g_iCorrectBI_X,521+g_iCorrectBI_Y,eBII_QUICK_SKILL_SLOT0+g_hero.m_wCurrentQuickSkillSlot);


//	EXP바 그리기
	{
		int	iGaugeWidth=73;

		if	(g_hero.m_iExperience > 0)
		{
			LONGLONG	llExp	=	g_hero.getExpForLevelUp();
			if (llExp	>	0)
			{
				LONGLONG	llWidth	=	iGaugeWidth;
				llWidth				=	llWidth*g_hero.getCurrentLevelExp()/llExp;

				g_sprGameBottomInterface.putClipedImage(328+g_iCorrectBI_X,537+g_iCorrectBI_Y,eBII_EXP_GAUGE,llWidth,100);

//	스킬 포인트 EXP 게이지
				LONGLONG	llCurrentLevelMaxSkillPoint	=	min(g_hero.m_iLevel+1,100);
				if (llCurrentLevelMaxSkillPoint	>	0)
				{

					LONGLONG	llCurrentSkillPoint			=	llCurrentLevelMaxSkillPoint*g_hero.m_iExperience/llExp;
					LONGLONG	llNextSkillPoint			=	llCurrentSkillPoint+1;

					LONGLONG	llDestExp					=	llNextSkillPoint*llExp/llCurrentLevelMaxSkillPoint;
					LONGLONG	llBaseExp					=	llCurrentSkillPoint*llExp/llCurrentLevelMaxSkillPoint;
					LONGLONG	llExpGap					=	llDestExp-llBaseExp;
					LONGLONG	llCurrentExp				=	g_hero.m_iExperience-llBaseExp;

					if (llExpGap	>	0)
					{

						llWidth	=	iGaugeWidth*llCurrentExp/llExpGap;
						llWidth	=	min(llWidth,iGaugeWidth);

						g_sprGameBottomInterface.putClipedImage(406+iGaugeWidth-llWidth+g_iCorrectBI_X,537+g_iCorrectBI_Y,eBII_SKILL_EXP_GAUGE,llWidth,100);
					}
				}
			}
		}
	}	//	EXP바 그리기


//	체력 게이지
	{
		int	iWidth				=	g_sprGameBottomInterface.getSpriteWidth(eBII_RED_STONE_01);
		int	iBloodStoneWidth	=	g_hero.m_iHP*iWidth/g_hero.getMaxHP();
		int	iHPGaugeFrameCount	=	16;
		int	iBloodStoneImage	=	eBII_RED_STONE_01+((s_iFrameCounter/12)%iHPGaugeFrameCount);

		if	(g_lpHero->m_iFirstAidHP)
		{
			int	iFirstAidHP		=	g_hero.m_iHP+g_lpHero->m_iFirstAidHP;
			int	iStoneWidth		=	iFirstAidHP*iWidth/g_hero.getMaxHP();

			g_sprGameBottomInterface.putClipedImage(297+g_iCorrectBI_X,542+g_iCorrectBI_Y,iBloodStoneImage,iStoneWidth,100,100,dPUT_HALF_BLENDING);
		}

		g_sprGameBottomInterface.putClipedImage(297+g_iCorrectBI_X,542+g_iCorrectBI_Y,iBloodStoneImage,iBloodStoneWidth);
	}


//	CP바 그리기
	{
		int	iWidth				=	g_sprGameBottomInterface.getSpriteWidth(eBII_CP_LEVEL_MINUS_01);
		int	iMaxCP				=	g_hero.getMaxCP();
		int	iCP					=	g_hero.m_iCP;
		int	iCPLevel			=	max(GetCPLevel(iCP),0);
		int	iCPGaugeAnmFrameCount=	16;
		int	iCPGaugeFrameCount	=	16*2+3;
		int	iCPGaugeImage		=	eBII_CP_LEVEL_MINUS_01+iCPLevel*iCPGaugeFrameCount+((s_iFrameCounter/12)%iCPGaugeAnmFrameCount);
		int	iGaugeWidth			=	min(iCP*iWidth/iMaxCP,iWidth);

		if (iCPLevel	>	0)
			g_sprGameBottomInterface.putClipedImage(330+g_iCorrectBI_X,562+g_iCorrectBI_Y,iCPGaugeImage,iGaugeWidth);

		g_sprGameBottomInterface.Put(289+g_iCorrectBI_X,564+g_iCorrectBI_Y,iCPGaugeImage+iCPGaugeAnmFrameCount);

		s_sText.pushShadowStatus(FALSE);
		s_sText.cPut(403+g_iCorrectBI_X,586+g_iCorrectBI_Y,WHITE,"%d/%d",g_hero.m_iCP/100,g_hero.getMaxCP()/100);
		s_sText.setClipArea(0,0,330+iGaugeWidth,g_iScreenHeight);
		s_sText.cPut(403+g_iCorrectBI_X,586+g_iCorrectBI_Y,0,"%d/%d",g_hero.m_iCP/100,g_hero.getMaxCP()/100);
		s_sText.releaseClipArea();
		s_sText.popShadowStatus();
	}
	
	//파티 연합 상태의 파티원들의 체력 게이지 그려주기.
// 	for (int i = 0 ; i < s_partyInfo.m_wUnionPartyCount; i++)
// 	{
// 		putE
// 	}
}
