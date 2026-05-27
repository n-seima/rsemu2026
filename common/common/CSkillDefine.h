#ifndef	_classSKILL_DEFINE_H
#define	_classSKILL_DEFINE_H

#ifndef __AFXWIN_H__
#ifndef _WINDOWS_
#include <windows.h>
#endif
#endif

#define		dSKILL_COMMENT_LENGTH				256
#define		dSKILL_POWER_UP_COMMENT_LENGTH		64
#define		dSKILL_NAME_LENGTH					32
#define		dSKILL_SOUND_FILE_LENGTH			32

#define		dLIMIT_SKILL_LEVEL					50

#define		dMAX_SKILL							2048
#define		dMAX_ACTIVE_SKILL					256
#define		dVALID_BLOCKER_CODE					dMAX_ACTIVE_SKILL


#define		dABILITY_COUNT						52
#define		dHERO_ABILITY_COUNT					52
#define		dMONSTER_ABILITY_COUNT				10

#define		dACTIVE_REACTION_COUNT				4
#define		dPLAYER_ACTIVE_REACTION_LIMIT		4
#define		dSKILL_SPARE_VALUE_COUNT			20	//	¿©ºĞÀÇ »óÅÂÄ¡
#define		dMAX_REQUIRE_SKILL_COUNT			5
#define		dEXTRA_EFFECT_COUNT					10

#define		dSKILL_ACTION_TO_OBJECT				0xfff9	//	º¸¹° »óÀÚ
#define		dSKILL_BASIC_ATTACK_TO_ARCA			0xfffa	//	º¸¹° »óÀÚ
#define		dSKILL_BASIC_ATTACK_TO_DOOR			0xfffb	//	¹® °ø°İ
#define		dSKILL_BORDER_OF_ATTACK_TO_OBJECT	0xfffb	//	¹® °ø°İ
#define		dSKILL_BASIC_ATTACK					0xfffe	//	±âº»°ø°İ
#define		dSKILL_CONTINUOUS_HIT_ATTACK		0xfffd	//	¿¬¼Ó Å¸°İ °ø°İ
#define		dSKILL_ILLUSION_ATTACK				0xfffc	//	µô·¹ÀÌ ¾ûáÃ
#define		dSKILL_INREGULAR_SKILL				0xfff0	//	±âº»°ø°İ
#define		dSKILL_SERIAL_HOLYCROSS				144		// È¦¸®Å©·Î½º½Ã¸®¾ó¹øÈ£ 09.09.24 
#define		dSKILL_SERIAL_PLOT_OF_SHADOW		357		// À½¸ğÀÇ ±×¸²ÀÚ ½Ã¸®¾E¹øÈ£ 09.09.29
#define		dSKILL_HELL_PRISON								381
#define		dJOB_MONSTER_START					201
#define		dBREAK_EQUIPMENT_FACTOR				2
#define		dNEED_STATE_EMPTY					0x3f	// ÇÊ¿ä»óÅÂ 

#define		dPASSIVE_SKILL_COUNT				5		//	ÆĞ½ÃºEºÅ³ ÃÖ´E¡..


#define		dMAX_PHYSICAL_DAMAGE					20000000
#define		dMAX_MAGICAL_DAMAGE						20000000

const	int	c_iCoolTimeSkillCount	=	10;

#define		dSKILL_TRANSFORMATION_BOW_ATTACK				504
#define		dSKILL_TRANSFORMATION_SWORD_ATTACK				500
#define		dSKILL_TRANSFORMATION_FIRE_BOLT					545

#define		dMAX_BIT_STICK_ACTOR							10
#define		dMAX_CAN_USE_BIT_TO_ONE							2	
enum
{
	eATTACK_OBJECT_DOOR,
	eATTACK_OBJECT_ARCA,
};

enum
{
	eTARGET_OBJECT_TRAP	=	1,
	eTARGET_OBJECT_FIELD_TRAP,
	eTARGET_OBJECT_ARCA,
	eTARGET_OBJECT_DOOR,
};
enum
{
	eBUNSHIN_RUSH_ATTACK = 1,		// ºĞ½Å ·¯½¬ °ø°İ
	eBUNSHIN_ROUND_ATTACK,			// ºĞ½Å ÁÖÀ§µ¹¸é¼­ °ø°İ

};

//
//	Reaction Skill Active Trigger
enum
{
	eRSAT_NONE,			//	¾ø¾E	
	eRSAT_STRIKE,		//	´©±º°¡¿¡°Ô »ç¿Eß´Ù.
	eRSAT_BLOCKING,		//	ºúÓ°Å·Çß´Ù.
	eRSAT_HIT,			//	¸Â¾Ò´Ù.
	eRSAT_DETECTING,	//	Å½ÁöÇÒ¶§
	eRSAT_DAMAGE_MAGIC_SKILL_SB,	//	¸¶¹ı °ø°İ ´çÇÒ¶§
	eRSAT_HIT_DAMAGE_PHYSICAL,	//	½ÇÁ¦ ´EÌÁö¸¦ ÀÔ±EÀE¡ ¹°¸® ´EÌÁö¸¦ ÀÔÀ½ ¹ßµ¿
	eRSAT_HIT_CLOSE_RANGE_ATTACK_COUNTER,	//	±ÙÁ¢ °ø°İÀ» ¹Ş¾ÒÀ» ¶§ Ä«¿ûáÍ
	eRSAT_HIT_MISSILE_ATTACK,	//	¿ø°Å¸® °ø°İÀ» ¹Ş¾ÒÀ» ¶§
	eRSAT_HIT_CLOSE_RANGE_ATTACK_DODGE_COUNTER,	//	±ÙÁ¢ °ø°İÀ» ¹Ş¾ÒÀ» ¶§ È¸ÇÇ Ä«¿ûáÍ
	eRSAT_REST	,		//	Á¤Áö»óÅÂ
	eRSAT_DAMAGED,		//	´EÌÁEÀÔÀ»¶§
	eRSAT_JUDGED_BEATEN_TIMING,	//	¸Â¾Ò´Ù°EÆÇÁ¤ ³µÀ»¶§
	eRSAT_HIT_PHYSICAL_DAMAGE_TIME_COUNTER,	//	¹°¸®°ø°İ ¹Ş¾ÒÀ»¶§ Ä«¿ûáÍ
	eRSAT_HIT_STRANGE_STATUS_ATTACK,		//	»óÅÂ ÀÌ»E°ø°İÀ» ¹Ş¾ÒÀ»¶§ Ä«¿ûáÍ
	eRSAT_PHYSICAL_DAMAGED,	//	½ÇÁ¦ ´EÌÁö¸¦ ÀÔ±EÀE¡ ¹°¸® ´EÌÁö¸¦ ÀÔÀ½ ¹ßµ¿
};
//
//	Skill Type
enum
{
	eST_NORMAL,
	eST_PASSIVE,
	eST_REACTION,
	eST_SUPPORT,
	eST_REAL_PASSIVE,
};

//
//	Aid Magic
enum
{
	eAM_ADD_FIRE_DAMAGE,
	eAM_ADD_WATER_DAMAGE,
	eAM_ADD_WIND_DAMAGE,
	eAM_ADD_EARTH_DAMAGE,
	eAM_ADD_LIGHT_DAMAGE,
	eAM_ADD_DARK_DAMAGE,
};

enum
{//ADD Hit Type
	eAHT_POISON,
	eAHT_DARK,
	eAHT_BLEEDING,
	eAHT_ELECTRIC_SHOCK,
};

//
//	CE
enum
{
	eCE_STRANGE_STATUS_CURE			=	0x01,	//ÀÌ»ó°èÅEóÅÂÀÌ»ó¾Æ±ºÄ¡·E	
	eCE_DECLINE_POWER_STATUS_CURE	=	0x02,	//ÀúÇÏ°èÅEóÅÂÀÌ»ó¾Æ±ºÄ¡·E	
	eCE_CURSE_STATUS_CURE			=	0x04,	//ÀúÁÖ°èÅEóÅÂÀÌ»ó¾Æ±ºÄ¡·E	
	eCE_ALL_STRANGE_STATUS_CURE		=	0x08,	//¸ğµç»óÅÂÀÌ»ó¾Æ±ºÄ¡·E	
	eCE_RESURRECTION				=	0x10,	//Çàµ¿ºÒ´É¾Æ±ºÈ¸º¹
};

//
//	¼Ò¼öÁ¡ Ã³¸®
enum
{
	eDECIMAL_METHOD_LEAVE,	//	¹ö¸²
	eDECIMAL_METHOD_ROUND_OFF,	//	¹İ ¿Ã¸²
	eDECIMAL_METHOD_UP,			//	¿Ã¸²
};

//
//	»ç¿E´EE
enum
{
	eSKILL_CAST_AT_ENEMY				=	0x00000001,
	eSKILL_CAST_AT_PLAYER				=	0x00000002,
	eSKILL_CAST_AT_DEATH_PLAYER			=	0x00000004,
	eSKILL_CAST_AT_GROUND				=	0x00000008,
	eSKILL_CAST_QUICK					=	0x00000010,
	eSKILL_CAST_AT_CASTER				=	0x00000020,
	eSKILL_CAST_ON_DEATH_SUMMON_BEAST	=	0x00000040,
	eSKILL_CAST_ON_PARTY_MEMBER			=	0x00000080,
	eSKILL_CAST_ON_PET					=	0x00000100,
	eSKILL_CAST_ON_ENEMY_PET			=	0x00000200,
	eSKILL_CAST_ON_DEATH_PET			=	0x00000400,
	eSKILL_CAST_ON_SUMMON_BEAST			=	0x00000800,
	eSKILL_CAST_ON_KELBY				=	0x00001000,
	eSKILL_CAST_ON_TRAP					=	0x00002000,
	eSKILL_CAST_ON_ARCA					=	0x00004000,
	eSKILL_CAST_ON_DOOR					=	0x00008000,
	eSKILL_CAST_ON_USER_PARTY_MEMBER	=	0x00010000,
};

//
//	µ¥¹ÌÁEÅ¸ÀÔ
enum
{
	eDAMAGE_TYPE_PHYSICAL	=	0x01,
	eDAMAGE_TYPE_FIRE		=	0x02,
	eDAMAGE_TYPE_WATER		=	0x04,
	eDAMAGE_TYPE_WIND		=	0x08,
	eDAMAGE_TYPE_EARTH		=	0x10,
	eDAMAGE_TYPE_LIGHT		=	0x20,
	eDAMAGE_TYPE_DARK		=	0x40
};

enum
{
	eMDA_FIRE	,
	eMDA_WATER	,
	eMDA_WIND	,
	eMDA_EARTH	,
	eMDA_LIGHT	,
	eMDA_DARK	
};

//
//	½ºÅ³ Á¾·E
enum
{
	eSKILL_TYPE_NORMAL_HIT					,	//	°¡ÀEÀÏ¹İÀûÀÎ ¹°¸® °ø°İ
	eSKILL_TYPE_CONTINUOUS_HIT_BY_CHARACTER	,	//	Ä³¸¯ÅÍÀÇ ´EÌÁEÇÁ·¹ÀÓÀ» Ã¼Å©ÇØ¼­ 2´EÀÌ»E¶§¸°´Ù.
	eSKILL_TYPE_CONTINUOUS_HIT_VALUE		,	//	Æ®¸®°Å ÇÁ·¹ÀÓ ÀÌÈÄ¿¡ ¼öÄ¡¿¡ ÀÇ°ÅÇØ¼­ ¼øÂ÷ÀûÀ¸·Î ´EÌÁö¸¦ ÀÔÈù´Ù.
	eSKILL_TYPE_CONTINUOUS_HIT_BY_ILLUSION	,	//	ÀÜ»ó¿¡ ÀÇÇÑ ¿¬¼Ó Å¸°İ
	eSKILL_TYPE_ATTACK_BY_BUNSHIN			,	//	ºĞ½Å¿¡ ÀÇÇÑ °ø°İ
	eSKILL_TYPE_SCIMITAR_CUTTING			,	//	½Ã¹ÌÅÍ Ä¿ÆÃ
	eSKILL_TYPE_RUSH						,	//	·¯½¬

	eSKILL_TYPE_JUMP_ATTACK					,	//	Á¡ÇÁ °ø°İ
	eSKILL_TYPE_WILD_STAMP					,	//	¿ÍÀÏµE½ºÅÆÇÁ
	eSKILL_TYPE_ALTERING_HITTER				,	//	¿ÃÅÍ¸µ È÷ÅÍ
	eSKILL_TYPE_IMPACT						,	//	¶§¸®°EÁÖÀ§¿¡ Ãæ°İÆÄ°¡ »ı±ä´Ù.
	eSKILL_TYPE_WHIRLWIND					,	//	È¸¿À¸®ÇEÀÚ±â¸¦ Áß½ÉÀ¸·Î ÀÏÁ¤ ¿µ¿ªÀ» °ø°İÇÑ´Ù.)
	eSKILL_TYPE_WHIRLTHRUSTING				,	//	µ¹·ÁÂûÔ£±E	
	eSKILL_TYPE_SURPRISING_RAID				,	//	¼­ÇÁ¶óÀÌÂ¡·¹ÀÌµE	
	eSKILL_TYPE_WHIRL_RUNNING				,	//	ÈÙ·¯´×
	eSKILL_TYPE_GUARDIAN_POST				,	//	{	"°¡µğ¾ğÆ÷½ºÆ®"			,
	eSKILL_TYPE_RADIAL_ARC					,	//	{	"·¹ÀÌµğ¾ó¾ÆÅ©"			,
	eSKILL_TYPE_AWESOME_FORTRESS			,	//	¿À¿E¶ Æ÷Æ®¸®½º
	eSKILL_TYPE_BITE_HANGING				,	//	¹ÙÀÌÆ® ÇàÀ×
	eSKILL_TYPE_BOUNCING_LINEAR				,

	eSKILL_TYPE_FIRE_BALL					,	//	ÆÄÀÌ¾Eº¼
	eSKILL_TYPE_MISSILE						,	//	¹Ì»çÀÏ
	eSKILL_TYPE_CONTINUOUS_SHOOT			,	//	¿¬¼Ó ¹ß»E	
	eSKILL_TYPE_EXPLOSION_MISSILE			,	//	Æø¹ß¹Ì»çÀÏ
	eSKILL_TYPE_PIERCING_MISSILE			,	//	°EEÌ»çÀÏ
	eSKILL_TYPE_ILLUSION_MISSILE			,	//	È¯»ó¹Ì»çÀÏ
	eSKILL_TYPE_SPREAD_ARROW				,	//	½ºÇÁ·¹µE¾Ö·Î¿E	
	eSKILL_TYPE_BIT_GLIDER					,	//	ºñÆ® ±Û¶óÀÌ´E	
	eSKILL_TYPE_BOOMERANG					,	//	ºÎ¸Ş¶E	
	eSKILL_TYPE_LASER						,	//	·¹ÀÌÀE	
	eSKILL_TYPE_EXPLOSION					,	//	Æø¹ßÇE	
	eSKILL_TYPE_EXPLOSION_DEPEND_ON_IMAGE	,	//	ÀÌ¹ÌÁöÀÇÁ¸Æø¹ßÇE	
	eSKILL_TYPE_LINER_CHAIN_EXPLOSION		,	//	Áø¼±ÇE¬¼âÆø¹ß... -_-?
	eSKILL_TYPE_LAND_MARKER					,	//	·£µå¸¶Ä¿
	eSKILL_TYPE_INTERVAL_SHOOTER			,	//	ÀÎÅÍ¹E½´ÅÍ
	eSKILL_TYPE_WATER_FALL					,	//	¿öÅÍ ÆE	
	eSKILL_TYPE_TORNADO						,	//	Åä³×ÀÌµµ
	eSKILL_TYPE_DROP_ON_HEAD				,	//	¸Ó¸® À§·Î ¶³¾ûİø´Ù.
	eSKILL_TYPE_MACHINE_GUN					,	//	¸Ó½®°Ç
	eSKILL_TYPE_MACHINE_MISSILE				,	//	¸Ó½®ÀÇ ¹Ì»çÀÏ
	eSKILL_TYPE_SPECIAL_MISSILE				,	//	Æ¯¼E¹Ì»çÀÏ
	eSKILL_TYPE_TAIL_CHASER					,	//	Å×ÀÏ Ã¼ÀÌ¼­
	eSKILL_TYPE_SPIN_ARISING				,	//	µ¹·ÁÀÏ¾ûÏª±E
	eSKILL_TYPE_ARMOR_DISASSEMBLER			,	//	¾Æ¸Óµğ½º¾ûØÀºúÓ¯
	eSKILL_TYPE_AID_ATTACK					,	//	º¸Á¶ °ø°İ
	eSKILL_TYPE_DOUBLE_TARGET				,	//	´õºEÅ¸°Ù
	eSKILL_TYPE_GAS							,	//	°¡½º
	eSKILL_TYPE_FLORENCE_SPECIAL			,	//	ÇÃ·Î·»½º ½ºÆä¼È Å¸ÀÔ
	eSKILL_TYPE_ROLLING_LOG					,	//	µ¥±¼µ¥±¼ÅEª¹«
	eSKILL_TYPE_BIG_BULLET					,	//	°Å´EºÈ¯
	eSKILL_TYPE_THROW_ITEM					,	//	¾ÆÀÌÅÛ ´øÁö±E	
	eSKILL_TYPE_LIGHTNING_WINDER			,	//	¶óÀÌÆ®´× ¿ÍÀÎ´E	
	eSKILL_TYPE_MIRROR_IMAGE				,	//	"¹Ì·¯ÀÌ¹ÌÁE			,
	eSKILL_TYPE_CHAIN_LIGHTNING				,	//	"Ã¼ÀÎ¶óÀÌÆ®´×"			,
	eSKILL_TYPE_FIRE_WALL					,	//	"ÆÄÀÌ¾ûÛE				,
	eSKILL_TYPE_GIGA_LIGHTNING				,	//	"±â°¡¶óÀÌÆ®´×"			,
	eSKILL_TYPE_CONTINUOUS_WHIRLWIND		,	//  Áö¼ÓÇEó¿ûÑE.Ãß°¡..
	eSKILL_TYPE_MULTI_MISSILE				,	// ¸ÖÆ¼¹Ì»çÀÏ.

//////////////////////////////////////////////////////
//	ÀEõÀûÀÎ ½ºÅ³ °æ°E	
	eSKILL_TYPE_AGRESSIVE_SKILL_BORDER		,	//	ÀEõÀûÀÎ ½ºÅ³ °æ°E//	ÀEõÀûÀÎ ½ºÅ³ °æ°E//////////////////////////////////////////////////////
	eSKILL_TYPE_JUMP						,	//	Á¡ÇÁ
	eSKILL_TYPE_PASSIVE_REACTION			,	//	Áö¼ÓÇE®¾×¼Ç
	eSKILL_TYPE_ACTIVITY_REACTION			,	//	¹ßµ¿ÇE®¾×¼Ç
	eSKILL_TYPE_ICY_STALAGMITE				,	//	¾ÆÀÌ½Ã½ºÅ»¶ó±×¸¶ÀÌÆ®
	eSKILL_TYPE_CHARGE						,	//	ÃæÀE	
	eSKILL_TYPE_HEAL						,	//	Ä¡·E	
	eSKILL_TYPE_DANCING_BLOCKER				,	//	´úÙÌ ºúÓÎÄ¿
	eSKILL_TYPE_MAGIC_SHIELD				,	//	¸¶¹ı¹æÆĞ
	eSKILL_TYPE_ADVANCED_BLOCKING			,	//	ºúÓ°·EÇâ»E	
	eSKILL_TYPE_AURA						,	//	¿À¶E	
	eSKILL_TYPE_BLOCKING					,	//	¸·±E	
	eSKILL_TYPE_DODGE						,	//	È¸ÇÇ
	eSKILL_TYPE_COUNTER						,	//	Ä«¿ûáÍ
	eSKILL_TYPE_TELEPORT					,	//	ÅÚ·¹Æ÷Æ®

	eSKILL_TYPE_AID_MAGIC					,	//	º¸Á¶ ¸¶¹ı
	eSKILL_TYPE_COMPLETE_PROTECTION			,	//	ÄÄÇÃ¸®Æ® ÇÁ·ÎÅØ¼Ç
	eSKILL_TYPE_SHIMMERING_SHIELD			,	//	½Ã¸Ó¸µ ½ÇµE	
	eSKILL_TYPE_MIRROR_TOWER				,	//	¹Ì·¯ Å¸¿E	
	eSKILL_TYPE_DISPLACEMENT				,	//	µğ½ºÇÃ·¹ÀÌ½º¸ÕÆ®

	eSKILL_TYPE_PET_COMMAND					,
	eSKILL_TYPE_ONE_BITE					,
	eSKILL_TYPE_MINIPET_COMBI				,


//////////////////////////////////////////////////////
//	ÀÓ½Ã ½ºÅ³ °æ°E	
//////////////////////////////////////////////////////

	eSKILL_TYPE_TRAP						,
	eSKILL_TYPE_CONTINUOUS_HIT_OF_DOPPELGANGER	,		// ºĞ½ÅÀÇ ¿¬¼Ó°ø°İ
	eSKILL_TYPE_PASSIVE						,	// ÆĞ½ÃºE½ºÅ³ 
	eSKILL_TYPE_GROUP_CONTINUOUS_BY_BIT				,	//	ºñÆ®¿¡ÀÇÇÑ ±×·E¿¬Å¸!
	eSKILL_TYPE_BOUNCING_MISSILE,						//	ÆÃ±â´Â ¹Ì»çÀÏ
	eSKILL_TYPE_FLY_BOUNCING_MISSILE,						//	³¯¶ó°¡´Â ÆÃ±â´Â ¹Ì»çÀÏ
	eSKILL_TYPE_WIDE_MISSILE,						//¹E§ ¹Ì»çÀÏ
	eSKILL_TYPE_ARC_MISSILE,						//Æ÷¹°¼± ¹Ì»çÀÏ

};

enum
{	// ÆĞ½ÃºE¿°E	
	eSKILL_PEE_INCREASE_CONCENTRATION,	// ÁıÁß·Â
	eSKILL_PEE_INCREASE_HARD_BLOW,		// °­Å¸
	eSKILL_PEE_INCREASE_MAX_HP	,	// ÃÖ´E¼·ÂÁõ°¡
	eSKILL_PEE_INCREASE_STRANGE_STATUS_RESISTANCE	,	// »óÅÂÀÌ»óÀúÇ×
	eSKILL_PEE_INCREASE_RANGE				,	// »ç°Å¸®Áõ°¡
	eSKILL_PEE_INCREASE_MAGIC_RESISTANCE,		//	¸¶¹ıÀúÇ×Áõ°¡.
	eSKILL_PEE_CORRECT_LIGHT_MIN_DAMAGE,				//	ºûÃÖ¼Ò´EÌÁEº¸Á¤	
	eSKILL_PEE_ADD_BIT_CHANCE,				//	ºñÆ®ºÎÂøÈ®·EÁõ°¡
	eSKILL_PEE_ADD_BIT_CHANCE_WHEN_BE_HIT,		//	¸ÂÀ»¶§ »ó´EEºñÆ®ºÎÂøÈ®·EÁõ°¡
	eSKILL_PEE_INCREASE_HIT_RANGE,			//	Å¸°İ¹E§Áõ°¡

};

//
//	½ºÅ³ »ç¿E» À§ÇØ ÇÊ¿äÇÑ ÀåºE

enum
{
	eREQUIRE_EQUIPMENT_FOR_SKILL_NONE			=	0,
	eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON			=	0x0001,
	eREQUIRE_EQUIPMENT_FOR_SKILL_SHIELD			=	0x0002,
	eREQUIRE_EQUIPMENT_FOR_SKILL_BULLET			=	0x0004,
	eREQUIRE_EQUIPMENT_FOR_SKILL_WEAPON2		=	0x0008,
	eREQUIRE_EQUIPMENT_FOR_SKILL_PET			=	0x0010,
	eREQUIRE_EQUIPMENT_FOR_SKILL_SUMMON_BEAST	=	0x0020,
	eREQUIRE_EQUIPMENT_FOR_SKILL_PET_OR_BEAST	=	0x0040,
	eREQUIRE_EQUIPMENT_FOR_SKILL_POTION			=	0x0080,
	eREQUIRE_EQUIPMENT_FOR_SKILL_FLOWER			=	0x0100,
	eREQUIRE_EQUIPMENT_FOR_SKILL_DRUG			=	0x0200,
	eREQUIRE_EQUIPMENT_FOR_SKILL_CANDY			=	0x0400,
};	//	½ºÅ³ »ç¿E» À§ÇØ ÇÊ¿äÇÑ ÀåºE
enum
{
	ePC_ATTACK,
	ePC_DEFENSE,
	ePC_PATROL,
	ePC_SPECIAL_ACTION,
	ePC_USE_SKILL,
};

//
//	CP¸¦ ¾ò¾ûÛÀ´Â Å¸ÀÌ¹Ö
enum
{
	eGET_CP_HIT,//È÷Æ® ¼ø°£
	eGET_CP_FIRST_HIT,//Ã¹¹øÂ° È÷Æ®
	eGET_CP_DEFENSE,//¸·´Â ¼ø°£
	eGET_CP_COUNTER,//º£´Â ¼ø°£
	eGET_ACTION_FINISH,//Çàµ¿Á¾·E	
	eGET_ACTION,//¹ßµ¿ ¼ø°£
	eGET_LAND,//ÂøÁö¼ø°£
	eGET_EVERY_HIT,//¸Å¹EÈ÷Æ®½Ã
	eGET_ENNEMY_IN_ATTACK_RANGE,//»çÁ¤±Ç ³» ÀûÀÌ µé¾ûÛÂ ¼ø°£
	eGET_DEFENSE_DODGE,//¸·´Â ¼ø°£,È¸ÇÇ ¼ø°£
	eGET_PARTY_ATTACKED,//ÁÖÀ§ ¾Æ±º »óÅÂÀÌ»E¸¶¹ı °ø°İ ¹Ş¾ÒÀ» ½Ã
	eGET_FIND_ENEMY,//ÀEÇÏ³ª ¹ß°ß ÇÒ ¶§ ¸¶´Ù
	eGET_ENEMY_DEATH,//ÀEÇÏ³ª ÆÄ¸EÇÒ ¶§ ¸¶´Ù
//ÀûÀ» ÆÇº°ÇÑ ¼ø°£
//¼³Ä¡ ¿Ï·E¼ø°£
//ÇØÃ¼ ¿Ï·E¼ø°£
//Å½ÁöÇÑ ¼ø°£
//±â¼E¼º°øÇÑ ¼ø°£
//ÇÑ ¹EÃ¼·Â È¸º¹ ÇÒ ¶§¸¶´Ù
//¸¶Áö¸· È÷Æ®
//¾Æ±ºÀÌ ¾ûÑÒ °ø°İ ¹ŞÀ»¶§¸¶´Ù
//¾Æ±ºÀÌ ºE°ø°İ ¹ŞÀ»¶§¸¶´Ù
//¾Æ±ºÀÌ ºÒ,¹Ù¶E°ø°İ¹ŞÀ»¶§¸¶´Ù
//¾Æ±ºÀÌ ¹°,¶¥ °ø°İ¹ŞÀ»¶§¸¶´Ù
//¾Æ±ºÀÌ ¸¶¹ı°ø°İ ¹ŞÀ»¶§¸¶´Ù
//Àû±º ÇÏ³ª È÷Æ®½Ã¸¶´Ù
//¾Æ±º ÇÑ ¸EÀû¿EÃ¸¶´Ù
//ÇÑ °³ ¹ß°ß½Ã¸¶´Ù
};

enum
{	// ÇÊ¿ä»óÅÂ.
	eNEED_STATE_CONSENSUS,		// ±³°¨.
	eNEED_STATE_FIGHTING_SPIRIT	,	// ÅõÁE
};

//
//	Extra Effext Adaptive Target
enum
{
	eEEAT_ALL			,
	eEEAT_ENEMY			,
	eEEAT_OWN_FORCE		,
	eEEAT_SELF			,
	eEEAT_PARTY_MEMBER	,
	eEEAT_PET			,
	eEEAT_ENEMY_PET		,
	eEEAT_SUMMON_BEAST	,
	eEEAT_PET_AND_BEAST	,
	eEEAT_SELF_AND_ENEMY,
	eEEAT_USER_PARTY_MEMBER	,
	eEEAT_ALL_EXCEPT_SELF,
	eEEAT_ENEMY_MONSTER,
	eEEAT_ACTIVE_SKILL_AREA,
};

//
//	½ºÅ³ÀÇ ºÎ°¡ È¿°E
enum
{
	eSKILL_EE_CONFUSE,	//	È¥¶E	
	eSKILL_EE_BREAK_ARMOR,	//	°©¿ÊÆÄ±«
	eSKILL_EE_BREAK_WEAPON,	//	¹«±âÆÄ±«
	eSKILL_EE_STUN,	//	°æÁE	
	eSKILL_EE_FREEZE,	//	ÇÁ¸®ÁE	
	eSKILL_EE_COLD,	//	ÄİµE	
	eSKILL_EE_BLIND,	//	Àå´Ô

	eSKILL_EE_DECLINE_HIT_CHANCE,	//	¸úİß·EÀúÇÏ
	eSKILL_EE_DECLINE_DODGE_CHANCE,	//	È¸ÇÇÀ² ÀúÇÏ

	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE_FOR_PET_COMMAND,
	eSKILL_EE_INCLINE_DEFENSE_POWER_PERCENTAGE_FOR_PET_COMMAND,
	eSKILL_EE_RESET_ATTACK_DEFENSE_POWER_FOR_PET_COMMAND,

	eSKILL_EE_DECLINE_ATTACK_POWER,	//	°ø°İ·Â ÀúÇÏ
	eSKILL_EE_DECLINE_ATTACK_POWER_PERCENTAGE,
	eSKILL_EE_DECLINE_HIT_CHANCE_BY_LIGHT,	//	ºû¿¡ ÀÇÇÑ ¸úİß·EÀúÇÏ
	eSKILL_EE_DECLINE_DODGE_CHANCE_BY_LIGHT,	//	ºû¿¡ ÀÇÇÑ È¸ÇÇÀ² ÀúÇÏ
	eSKILL_EE_BLIND_BY_LIGHT,	//	ºû¿¡ ÀÇÇÑ Àå´Ô
	eSKILL_EE_LIGHT_ATTACK_ADDITIONAL_EFFECT,	//	ºE°ø°İ ºÎ°¡È¿°E	
	eSKILL_EE_CURSE_AMPLIFICATION,
	eSKILL_EE_CREATE_DUMMY,	//	´õ¹Ì »ı¼º
	eSKILL_EE_DECLINE_MOVE_SPEED,
	eSKILL_EE_DECLINE_ATTACK_SPEED,

	eSKILL_EE_INCLINE_HIT_CHANCE,	//	¸úİß·E»ó½Â
	eSKILL_EE_INCLINE_DODGE_CHANCE,	//	È¸ÇÇÀ² Áõ°¡
	eSKILL_EE_INCLINE_MOVE_SPEED,	//	ÀÌµ¿ ¼Óµµ Áõ°¡
	eSKILL_EE_INCLINE_ATTACK_SPEED,	//	°ø°İ ¼Óµµ Áõ°¡
	eSKILL_EE_LEVITATE,	//	°øÁßºÎ¾E
	eSKILL_EE_INCLINE_ATTACK_POWER,	//	°ø°İ·Â Áõ°¡
	eSKILL_EE_INCLINE_ATTACK_POWER_PERCENTAGE,	//	°ø°İ·Â ÆÛ¼¾Æ¼ÁEÁõ°¡

	eSKILL_EE_DECREASE_FINAL_DEFENSE_POWER_PERCENTAGE,
	eSKILL_EE_DECREASE_DEFENSE_POWER,	//	¹æ¾ûÓÂ Áõ°¡
	eSKILL_EE_DECREASE_DEFENSE_POWER_PERCENTAGE,	//	¹æ¾ûÓÂ ÆÛ¼¾Æ¼ÁE°¨¼Ò
	eSKILL_EE_INCREASE_DEFENSE_POWER,	//	¹æ¾ûÓÂ Áõ°¡
	eSKILL_EE_INCREASE_DEFENSE_POWER_PERCENTAGE,	//	¹æ¾ûÓÂ ÆÛ¼¾Æ¼ÁEÁõ°¡

	eSKILL_EE_INCLINE_STRANGE_STATUS_RESISTANCE,	//	ÀÌ»ó»óÅÂ ÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_DECLINE_POWER_RESISTANCE,	//	´É·ÂÄ¡ ÀúÇÏ ÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_CURSE_RESISTANCE,	//	ÀúÁÖ ÀúÇ× Áõ°¡

	eSKILL_EE_DECLINE_MAGIC_RESISTANCE,
	eSKILL_EE_DECLINE_TARGET_MAGIC_RESISTANCE,	//	Å¸°Ù¸¶¹ıÀúÇ×°¨¼Ò

	eSKILL_EE_DECLINE_FIRE_RESISTANCE,	//	ºÒÀúÇ× °¨¼Ò
	eSKILL_EE_DECLINE_WATER_RESISTANCE,	//	¹°ÀúÇ× °¨¼Ò
	eSKILL_EE_DECLINE_WIND_RESISTANCE,	//	¹Ù¶÷ÀúÇ× °¨¼Ò
	eSKILL_EE_DECLINE_EARTH_RESISTANCE,	//	´EöÀúÇ× °¨¼Ò
	eSKILL_EE_DECLINE_LIGHT_RESISTANCE,	//	ºûÀúÇ× °¨¼Ò
	eSKILL_EE_DECLINE_DARK_RESISTANCE,	//	¾ÏÈæÀúÇ× °¨¼Ò

	eSKILL_EE_INCLINE_MAGIC_RESISTANCE,	//	¸¶¹ı ÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_FIRE_RESISTANCE,	//	ºÒÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_WATER_RESISTANCE,	//	¹°ÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_WIND_RESISTANCE,	//	¹Ù¶÷ÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_EARTH_RESISTANCE,	//	´EöÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_LIGHT_RESISTANCE,	//	ºûÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_DARK_RESISTANCE,	//	¾ÏÈæÀúÇ× Áõ°¡
	eSKILL_EE_INCLINE_4ELEMENT_RESISTANCE,

	eSKILL_EE_INCLINE_CONCENTRATION,	//	ÁıÁß·Â Áõ°¡
	eSKILL_EE_INCLINE_CONCENTRATION_BY_STELLAR_PEER,
	eSKILL_EE_DECLINE_CONCENTRATION,	//	ÁıÁß·Â °¨¼Ò

	eSKILL_EE_INCLINE_POWER,	//	ÈûÁõ°¡"	
	eSKILL_EE_INCLINE_AGILITY,	//	¹ÎÃ¸¼ºÁõ°¡"
	eSKILL_EE_INCLINE_CONSTITUTION,	//	°Ç°­Áõ°¡"	
	eSKILL_EE_INCLINE_CHARISMA,	//	°Ç°­Áõ°¡"	
	eSKILL_EE_INCLINE_INTELIGENCE,	//	Áö½ÄÁõ°¡"	
	eSKILL_EE_INCLINE_WISDOM,	//	ÁöÇıÁõ°¡"	
	eSKILL_EE_INCLINE_LUCK,	//	¿ûİõ°¡"	

	eSKILL_EE_INCLINE_MAX_CP,	//	CPÁõ°¡
	eSKILL_EE_INCLINE_MAX_HP,	//	Ã¼·ÂÁõ°¡"	
	eSKILL_EE_INCLINE_MAX_HP_PERCENTAGE,	//	Ã¼·ÂÁõ°¡"	
	eSKILL_EE_REGEN_HP,	//	Ã¼·ÂÀÚµ¿È¸º¹"	

	eSKILL_EE_NOCK_BACK,	//	³EE
	eSKILL_EE_CAN_NOT_MOVE_BY_EARTH,	//	ÀÌµ¿ºÒ°¡ by ´EE	
	eSKILL_EE_CAN_NOT_MOVE,	//	ÀÌµ¿ºÒ°¡
	eSKILL_EE_DECLINE_MOVE_SPEED_BY_EARTH,	//	ÀÌµ¿¼Óµµ ÀúÇÏ by ´EE	
	eSKILL_EE_STUN_BY_EARTH,	//	°æÁEby ´EE	
	eSKILL_EE_STONE_BY_EARTH,	//	¼®È­ by ´EE	
	eSKILL_EE_POISON,	//	Áßµ¶
	eSKILL_EE_CURE_POISON,	//	Áßµ¶ Ä¡·E
	eSKILL_EE_INCLINE_BY_OWNER_FORCE,	//	µ¿·á¿¡ÀÇÇÑ°ø°İ·Â»ó½Â
	eSKILL_EE_CHANGE_ALLIGNMENT,	//	¼ºÇEº¯°E
	eSKILL_EE_ADDITIONAL_FIRE_DAMAGE,	//	Ãß°¡ºÒ´EÌÁE		,
	eSKILL_EE_ADDITIONAL_WATER_DAMAGE,	//	Ãß°¡¹°´EÌÁE		,
	eSKILL_EE_ADDITIONAL_WIND_DAMAGE,	//	Ãß°¡¹Ù¶÷´EÌÁE	,
	eSKILL_EE_ADDITIONAL_EARTH_DAMAGE,	//	Ãß°¡´Eö´EÌÁE	,
	eSKILL_EE_ADDITIONAL_LIGHT_DAMAGE,	//	Ãß°¡ºû´EÌÁE		,
	eSKILL_EE_ADDITIONAL_DARK_DAMAGE,	//	Ãß°¡¾ûÑÒ´EÌÁE	,

	eSKILL_EE_FIRE_DAMAGE,	//	ºÒ´EÌÁE		,
	eSKILL_EE_WATER_DAMAGE,	//	¹°´EÌÁE		,
	eSKILL_EE_WIND_DAMAGE,	//	¹Ù¶÷´EÌÁE	,
	eSKILL_EE_EARTH_DAMAGE,	//	´Eö´EÌÁE	,
	eSKILL_EE_LIGHT_DAMAGE,	//	ºû´EÌÁE		,
	eSKILL_EE_DARK_DAMAGE,	//	¾ûÑÒ´EÌÁE	,

	eSKILL_EE_FIRE_DAMAGE_SHIELD,	//	ºÒ´EÌÁE½ÇµE	
	eSKILL_EE_WATER_DAMAGE_SHIELD,	//	¹°´EÌÁE½ÇµE	
	eSKILL_EE_WIND_DAMAGE_SHIELD,	//	¹Ù¶E´EÌÁE½ÇµE	
	eSKILL_EE_EARTH_DAMAGE_SHIELD,	//	´EE´EÌÁE½ÇµE	
	eSKILL_EE_LIGHT_DAMAGE_SHIELD,	//	ºE´EÌÁE½ÇµE	
	eSKILL_EE_DARK_DAMAGE_SHIELD,	//	¾ûÑÒ ´EÌÁE½ÇµE

	eSKILL_EE_FLEE,
	eSKILL_EE_FLEE_BY_TURNUNDEAD,	//µµ¸ÁbyÅÏ¾ğµ¥µE	
	eSKILL_EE_STUN_BY_TURNUNDEAD,	//¸¶ºñbyÅÏ¾ğµ¥µE	
	eSKILL_EE_INSTANCE_KILL_TURNUNDEAD,	//ÁEçbyÅÏ¾ğµ¥µE	
	eSKILL_EE_DESTROYING_UNDEAD,	//	µğ½ºÆ®·ÎÀ×¾ğµ¥µE,
	eSKILL_EE_DESTROYING_UNHOLY,	//	µğ½ºÆ®·ÎÀ×¾ğÈ¦¸®	,

	eSKILL_EE_LIFE_DRAIN,	//Ã¼·ÂÈúØE	
	eSKILL_EE_RECOVER_HP,	//Ã¼·ÂÈ¸º¹
	eSKILL_EE_SLEEP,	//ÀE
	eSKILL_EE_CURE_STRANGE_STATUS,	//ÀÌ»ó°èÅEóÅÂÀÌ»ó¾Æ±ºÄ¡·E	
	eSKILL_EE_CURE_CURSE,	//ÀúÁÖ°èÅEóÅÂÀÌ»ó¾Æ±ºÄ¡·E	
	eSKILL_EE_CURE_DECLINE_POWER_STATUS,	//ÀúÇÏ°èÅEóÅÂÀÌ»ó¾Æ±ºÄ¡·E	
	eSKILL_EE_DISPLACEMENT,	//	ºùÀÇ
	eSKILL_EE_DETECTING,	//	µğÅØÆÃ
	eSKILL_EE_DETECTING_EVIL,	//	µğÅØÆÃ ÀÌºE
	eSKILL_EE_DISPELLING,	//	µğ½ºÆç¸µ
	eSKILL_EE_REDUCE_CP,	//	CP°¨¼Ò
	eSKILL_EE_REDUCE_CP_PERCENTAGE,	//	CP°¨¼Ò(ÆÛ¼¾Æ¼ÁE
	eSKILL_EE_CHARMING,	//	Ã­¹Ö
	eSKILL_EE_SANCTUARY,
	eSKILL_EE_WARP_NEAREST_VILLAGE,
	eSKILL_EE_CALLING,
	eSKILL_EE_CREATE_TOWN_PORTAL,
	eSKILL_EE_LEVEL_DRAIN,
	eSKILL_EE_INCREASE_LEVEL,
	eSKILL_EE_DECREASE_BLOCKING_CHANCE,

	eSKILL_EE_CHARMING_RESISTANCE,
	eSKILL_EE_TAME,
	eSKILL_EE_NONE_TARGET,	//	Å¸°Ù Á¦¿Ü »óÅÂ
	eSKILL_EE_REAIM_TARGET,	//	Å¸°Ù ÀE¼³Á¤
	eSKILL_EE_FIRST_AID,	//	ÀÀ±ŞÃ³Ä¡
	eSKILL_EE_BREEDING_RECORD,	//	»çÀ°±â·Ï
	eSKILL_EE_BONUS_EXP,	//	°æÇèÄ¡º¸³Ê½º

	eSKILL_EE_PET_BOOST,	//
	eSKILL_EE_BURST,	//

	eSKILL_EE_STRENGTHENING_FIRE_DAMAGE,
	eSKILL_EE_STRENGTHENING_WATER_DAMAGE,
	eSKILL_EE_STRENGTHENING_WIND_DAMAGE,
	eSKILL_EE_STRENGTHENING_EARTH_DAMAGE,
	eSKILL_EE_STRENGTHENING_LIGHT_DAMAGE,
	eSKILL_EE_STRENGTHENING_DARK_DAMAGE,
	eSKILL_EE_STRENGTHENING_MAGIC_DAMAGE,

	eSKILL_EE_DECLINE_CHARMING_RESISTANCE,
	eSKILL_EE_RALLY,
	eSKILL_EE_CHARMING_PET,

	eSKILL_EE_CALL_SUMMON_BEAST,
	eSKILL_EE_POWER_UP_SUMMON_BEAST,
	eSKILL_EE_RIDING_DOG,
	eSKILL_EE_INCREASE_SKILL_LEVEL,
	eSKILL_EE_SET_TRAP,
	eSKILL_EE_ADD_THORN,
	eSKILL_EE_INCLINE_CRITICAL_CHANCE,
	eSKILL_EE_PHYSICAL_DAMAGE_BASED_REMAIN_HP,
	eSKILL_EE_FINDING_TEASURE,
	eSKILL_EE_ITEM_COLLECTOR,
	eSKILL_EE_GOLD_HOLDER,

	eSKILL_EE_DECLINE_WEAPON_ATTACK_POWER_PERCENTAGE,
	eSKILL_EE_INCLINE_WEAPON_ATTACK_POWER,
	eSKILL_EE_INCLINE_ARMOR_DEFENSE_POWER,

	eSKILL_EE_TAUNT,
	eSKILL_EE_PET_TAUNT,
	eSKILL_EE_INCREASE_STUN_RESISTANCE,
	eSKILL_EE_DECREASE_PHYSICAL_DAMAGE,
	eSKILL_EE_INVINCIBLE,
	eSKILL_EE_DODGE_THIS_TIME_ATTACK,

	eSKILL_EE_REDUCE_STRANGE_STATUS_TIME,
	eSKILL_EE_REDUCE_CURSE_TIME,
	eSKILL_EE_REDUCE_DECLINE_POWER_STATUS_TIME,

	eSKILL_EE_MAKE_THE_OTHER_SELF,

	eSKILL_EE_RESISTANCE_KNOCK_BACK,
	eSKILL_EE_RESISTANCE_KNOCK_OUT,	//	³E¾Æ¿E¹öÆ¼±E
	eSKILL_EE_INCREASE_RIGHT_PUNCH_SKILL_LEVEL,
	eSKILL_EE_INCREASE_LEFT_PUNCH_SKILL_LEVEL,
	eSKILL_EE_INCREASE_RIGHT_KICK_SKILL_LEVEL,
	eSKILL_EE_INCREASE_LEFT_KICK_SKILL_LEVEL,
	eSKILL_EE_INCREASE_DOWN_KICK_SKILL_LEVEL,

	eSKILL_EE_NEUTRALIZE_POISON,
	eSKILL_EE_NEUTRALIZE_FREEZE_STUN,

	eSKILL_EE_DETECTING_MONSTER,
	eSKILL_EE_INVISIBLE,
	eSKILL_EE_INSTANCE_KILL,

	eSKILL_EE_DISARM_TRAP,	//	"ÇÔÁ¤ÇØÃ¼"			,,
	eSKILL_EE_DETECT_TRAP,	//	"ÇÔÁ¤Å½ÁE			,,
	eSKILL_EE_DETECT_SECRET_OBJECT,	//	"ºñ¹Ğ¿ÀºE§Æ®Å½ÁE	,,
	eSKILL_EE_UNLOCK_ARCA,	//	"Àá±ä»óÀÚ¿­±E		,,
	eSKILL_EE_UNLOCK_DOOR,	//	"Àá±ä¹®¿­±E		,,
	eSKILL_EE_PICKPOCKET,	//	"¼Ò¸ÅÄ¡±E			,,
	eSKILL_EE_ROBBERY,	//	"¾ÆÀÌÅÛ°­Å»"		,,

	eSKILL_NOTIFICATION_OF_DEATH,	//	Á×À½ÀÇ ¿¹°úÜE	
	eSKILL_EE_STEAL_MONEY,
	eSKILL_EE_MIN_DAMAGE_FOR_THIS_TIME_ATTACK,
	eSKILL_EE_ANIMATE_PARTNER,
	eSKILL_EE_TORTURE,
	eSKILL_EE_MAKE_SKILL_ZONE,
	eSKILL_EE_MAKE_SHAKLE,	//	Á·¼E»ı¼º
	eSKILL_EE_PERIODICAL_DARK_DAMAGE_BY_SLEEP,	//	ÁÖ±âÀûÀ¸·Î ¾ûÑÒ ´EÌÁEÀÔÈE	
	eSKILL_EE_ULTIMATE_BARRIER,
	eSKILL_EE_PIGEON_POST,
	eSKILL_EE_BERSERK,
	eSKILL_EE_DANCE,
	eSKILL_EE_MAGIC_BOX,
	eSKILL_EE_COPY,

	eSKILL_EE_TRANS_FROG,	//	{	"°³±¸¸®º¯½Å"		
	eSKILL_EE_TRANS_FROG2,	//	{	"°³±¸¸®º¯½Å"		
	eSKILL_EE_TRANS_FAT_GIRL,	//	{	"ºñ¸¸ÁE			
	eSKILL_EE_TRANS_TO_WEAPON,	//	{	"¹«±âº¯½Å"			
	eSKILL_EE_TRANS_TO_RABBIT,	//	{	"Åä³¢º¯½Å"			
	eSKILL_EE_JUMP_JUMP,	//	{	"±øÃÑ°Å¸®±E		
	eSKILL_EE_FEIGN_DEATH,	//	{	"Á×ÀºÃ´ÇÏ±E		
	eSKILL_EE_ATTACK_UNDER_FEIGN_DATH,	//	{	"Á×ÀºÃ´ÇÏ¸é¼­°ø°İÇÏ±E
	eSKILL_EE_BOUNCE,	//	Æ¨±â±E
	eSKILL_EE_DECREASE_ALL_RESISTANCE,	//	¸ğµEÀúÇ× °¨¼Ò

	eSKILL_EE_EXPLOSION_BOTTLE,
	eSKILL_EE_CHARGE_CP,
	eSKILL_EE_MARIONETTE,	//	¸¶¸®¿À³×¶ß

	eSKILL_EE_TRANS_POWER,	//	´É·ÂÄ¡º¯È¯"	
	eSKILL_EE_MAKE_ILLUSION,
	eSKILL_EE_DAMAGE_POOL,
	eSKILL_EE_IMPULSE,
	eSKILL_EE_BLOOD_DRAIN,
	eSKILL_EE_SELF_REPROOF,
	eSKILL_EE_DEGENERATE,
	eSKILL_EE_PHANTOM_IMPULSE,
	eSKILL_EE_TERRIBLE_DREAMS,
	eSKILL_EE_HWABYUNG,
	eSKILL_EE_ATTACK_COMMAND,
	eSKILL_EE_PLOT_OF_SHADOW,
	eSKILL_EE_SMELL_OF_DEATH,

	eSKILL_EE_DRAW_BODY,
	eSKILL_EE_CHAIN_DAMAGE,
	eSKILL_EE_CHAIN_FIRE_DAMAGE,
	eSKILL_EE_CHAIN_WATER_DAMAGE,
	eSKILL_EE_CHAIN_WIND_DAMAGE,
	eSKILL_EE_CHAIN_EARTH_DAMAGE,
	eSKILL_EE_CHAIN_LIGHT_DAMAGE,
	eSKILL_EE_CHAIN_DARK_DAMAGE,

	eSKILL_EE_CONTRACT,
	eSKILL_EE_DECLINE_RECOVER_HP_EFFICIENT,
	eSKILL_EE_INTERRUPTING_REVIVE,
	eSKILL_EE_BLOODY_CROSS,
	eSKILL_EE_UNFAIR_COVENANT,

	eSKILL_EE_SOUL_OATH,
	eSKILL_EE_BLOOD_COMPACT,
	eSKILL_EE_ANNULMENT_CONTRACT,

	eSKILL_EE_IMMUNE_STRANGE_STAUS,
	eSKILL_EE_IMMUNE_PHYSICAL_ATTACK,
	eSKILL_EE_IMMUNE_MAGIC_ATTACK,

	eSKILL_EE_INCREASE_FINAL_PHYSICAL_ATTACK_POWER,	//	ÃÖÁ¾¹°¸®°ø°İ·ÂÁõ°¡",

	eSKILL_EE_DODGE_MAGIC_DAMAGE,
	eSKILL_EE_FLY,
	eSKILL_EE_REGEN_MONSTER,
	eSKILL_EE_MONSTER_BOMB,

	eSKILL_EE_SWALLOW,	//	»E°±E	
	eSKILL_EE_INHALE_ENEMY,
	eSKILL_EE_DECREASE_SWALLOW_CORPES,	//	»E² ½ÃÃ¼¼E°¨¼Ò
	eSKILL_EE_CONVERSION_ICE,	//	¸ğµE¸¶¹ı¼Ó¼º °ø°İ ¹° ¼Ó¼ºÀ¸·Î ÀE¯
	eSKILL_EE_FOG_FORM,
	eSKILL_EE_NO_MORE_FIGHT,
	eSKILL_EE_ONE_BITE,	//	ÇÑÀÔ!
	eSKILL_EE_ZOMBIE_BOMB,	//	Á»ºEÆøÅº

	eSKILL_EE_INCREASE_MAGIC_ITEM_DROP_CHANCE,
	eSKILL_EE_INCREASE_GOLD_DROP_CHANCE,

	eSKILL_EE_CORRECT_FIELD_FIRE_PENELTY,
	eSKILL_EE_CORRECT_FIELD_WATER_PENELTY,
	eSKILL_EE_CORRECT_FIELD_WIND_PENELTY,
	eSKILL_EE_CORRECT_FIELD_EARTH_PENELTY,
	eSKILL_EE_CORRECT_FIELD_LIGHT_PENELTY,
	eSKILL_EE_CORRECT_FIELD_DARK_PENELTY,
	eSKILL_EE_CORRECT_ITEM_EQUIP_LEVEL,
	eSKILL_EE_INTERRUPTING_WEAPON,					// ¹«±âÇØÁ¦
	eSKILL_EE_DECREASE_FINAL_ATTACK_POWER_PERCENTAGE, // ÃÖÁ¾°ø°İ·Â°¨¼ÒÆÛ¼¾Æ¼ÁE
	eSKILL_EE_NOT_USE_ACTIVITY_REACTION_SKILL,		// ¹ßµ¿ÇE®¾×¼Ç»ç¿EİÁE	
	eSKILL_EE_CONSENSUS,							// ±³°¨»óÅÂ.
	eSKILL_EE_INCREASE_ALL_STATUS,		/// ¸ğµç´É·ÂÄ¡Áõ°¡.
	eSKILL_EE_MADNESS,						// ±¤¶E	
	eSKILL_EE_INCLINE_CRITICAL_DAMAGE_PERCENTAGE,	// Ä¡¸úá¸´EÌÁöÁõ°¡ÆÛ¼¾Æ¼ÁE	
	eSKILL_EE_DRAIN_CP,			//CPÈúØE
	eSKILL_EE_SOUL_CONVERSION,		// ¼Ò¿EÁ¹öÀE
	eSKILL_EE_SOUL_BURN,			// ¼Ò¿EE
	eSKILL_EE_RAGE_SOUL,				// Å¸¿À¸£´Â¿µÈ¥..
	eSKILL_EE_BINDING_WORDS,			// ¾ğ¾ûÍ¸¼Ó.
	eSKILL_EE_FIGHT_ENERGY,				// Åõ±E
	eSKILL_EE_RUSH,						// µ¹ÁE	
	eSKILL_EE_FIGHTING_SPIRIT,			// ÅõÁE
	eSKILL_EE_IGNORE_NEED_STATE,		// ÇÊ¿ä»óÅÂ¹«½Ã
	eSKILL_EE_IGNORE_BLOCKING,			// ºúÓ°Å·¹«½ÃÈ®·E	
	eSKILL_EE_RETURN_MISSILE_DAMAGE,	//	¹Ì»çÀÏ´EÌÁö¸®ÅÏ
	eSKILL_EE_INCREASE_HARD_BLOW,		// °­Å¸È®·Eõ°¡
	eSKILL_EE_JUMP,		// Á¡ÇÁ
	eSKILL_EE_ATTACK_DOPPELGANGER,	//	ºĞ½Å°ø°İ
	eSKILL_EE_ADD_BIT,	//	ºñÆ®Ãß°¡
	eSKILL_EE_REMOVE_BIT,	//	ºñÆ®Á¦°Å
	eSKILL_EE_BLEEDING,	//	ÃâÇE	
	eSKILL_EE_ELECTRIC_SHOCK,	//	°¨ÀE	
	eSKILL_EE_INSTANCE_HEAL_EFFECT,	//	ÁEÃÈúÈ¿°E	
	eSKILL_EE_MAKE_TYPE_UNDEAD,	//	¾ğµ¥µåÈ­
	eSKILL_EE_BLOOD_DONATION,	//	ÇåÇE

};

enum
{	// Åõ±âÈ¿°E	
	eSKILL_FEE_NONE,		// ¾øÀ½..
	eSKILL_FEE_ADD_ABSOLUTE_ATTACK_POWER,	//	ÆÄ±« ( Àı´E°ø°İ·Â ¼öÄ¡ Ãß°¡)
	eSKILL_FEE_INCREASE_ATTACK_COUNT,		//	ÁúÇ³ ( °ø°İ È½¼EÁõ°¡)
	eSKILL_FEE_INCREASE_GET_CP,		//	ÀıÁ¦ (È¹µECP Áõ°¡)
	eSKILL_FEE_INCREASE_ABSOLUTE_CRITICAL_PERCENT,		//	¾ûÍİ´Ï ( Àı´EÅ©¸®Æ¼ÄÃ% Áõ°¡)
	eSKILL_FEE_DECREASE_COOL_TIME,		//	±â´E( ÄğÅ¸ÀÓ °¨¼Ò )

};

//
//	special skill effect
	enum
{
	eSKILL_SE_SNIPE_SHOT				=	0x0001,	//¼­ÀÖ´Â Å¸°Ù 100% ¸úİß
	eSKILL_SE_SKULL_SHOT				=	0x0002,	//¼­ÀÖ´Â Å¸°Ù ÁEE	
	eSKILL_SE_NOT_INTERRUPT				=	0x0004,	//ÀûÀÇ °ø°İÀ» ¸Â¾Æµµ ²à²àÀÌ ¹öÆ¾´Ù. -_-;;
	eSKILL_SE_THROW_WEAPON				=	0x0008,	//¹«±â´øÁö±Eo-
	eSKILL_SE_CHANGE_JOB				=	0x0010,	//Á÷¾÷¹Ù²Ù±E	
	eSKILL_SE_HIDE_SHIELD				=	0x0020,	//¹æÆĞ´øÁö±Eo-
	eSKILL_SE_REMOVE_MISSLE				=	0x0040,	//¹Ì»çÀÏ Á¦°Å
	eSKILL_SE_DEFENSE_KNOCKOUT			=	0x0080,	//³EÆ¿E¹æ¾E	
	eSKILL_SE_TAUNT						=	0x0100,	//Å¸¿ûâ®
	eSKILL_SE_THROW_OTHER_JOB_WEAPON	=	0x0200,	//¼¼ÄÁÀE¹«±â´øÁö±Eo-

};

//	½ºÅ³ »ç¿E°á°E
enum
{
	eSKILL_FIRE_RESULT_OK,							//	¼º°E
	eSKILL_FIRE_RESULT_FAILED,						//	½ÇÆĞ
	eSKILL_FIRE_RESULT_NOT_APT_SKILL_TO_TARGET	,	//	Å¸°Ù¿¡°Ô ÀûÀıÄ¡ ¾ÊÀº ½ºÅ³ÀÌ´Ù.
	eSKILL_FIRE_RESULT_TOO_MANY_USE_THAT_SKILL	,	//	±× ½ºÅ³À» ³Ê¹« ¸¹ÀÌ »ç¿EÇß¾û›
	eSKILL_FIRE_RESULT_BLOCKED_POINT,				//	Á¡ÇÁ Á¾·E½ºÅ³À» »ç¿EÏ·Á Çß´Âµ¥ ÇØ´EÁöÁ¡ÀÌ ºúÓ°µÇ¾EÀÖ´Ù.
	eSKILL_FIRE_RESULT_TOO_CLOSE,					//	Á¡ÇÁ Á¾·E½ºÅ³À» »ç¿EÏ·Á Çß´Âµ¥ ÇØ´EÁöÁ¡ÀÌ ³Ê¹« °¡±õ´Ù.
	eSKILL_FIRE_RESULT_BLOCKED_OBJECT,
	eSKILL_FIRE_RESULT_CAN_NOT_FIND_TARGET_IN_ATTACK_RANGE,							//	¼º°E
	eSKILL_FIRE_RESULT_NOT_APT_SKILL_TO_THIS_STATUS,
	eSKILL_FIRE_RESULT_BY_BUSY	,					//	¹Ù»µ!!

};


enum
{
	eUSR_SUCCESS,
	eUSR_FAULT_SKILL,
	eUSR_WEAPON_NOT_FOUND,
	eUSR_SHIELD_NOT_FOUND,
	eUSR_BULLET_NOT_FOUND,
	eUSR_LOWER_CP,
	eUSR_VALID_SKILL,
	eUSR_CANO_NOT_CLOSE_ATTACK_ON_HILL,
	eUSR_PET_NOT_FOUND,
	eUSR_SUMMON_BEAST_NOT_FOUND,
	eUSR_REQUIRE_POTION,
	eUSR_REQUIRE_DURG,
	eUSR_REQUIRE_FLOWER,
	eUSR_REQUIRE_CANDY,
	eUSR_IS_COOL_TIME,
	eUSR_NOT_NEED_STATE,		// ÇÊ¿ä»óÅÂ..
	eUSR_BINDING_WORDS,	/// ¾ğ¾ûÍ¸¼Ó»óÅÂ..

};


//
//	Skill Ai Check Status

enum
{
	eSACS_ORDERING,							//	¿E±¼øÀ§
	eSACS_IN_SIGHT_OWNER_FORCE_HP,			//	½Ã¾ß³»¾Æ±ºÀÇHP
	eSACS_RANGE_TO_TARGET,					//	Å¸°Ù°úÀÇ°Å¸®
	eSACS_BUFF_ON_OWNER,					//	ÀÚ½Å¿¡°Ô¹öÇÁµÊ
	eSACS_BUFF_ON_TARGET,					//	Å¸°Ù¿¡°Ô¹öÇÁµÊ
	eSACS_OWNER_FORCE_COUNT_IN_SIGHT,		//	½Ã¾ß³»µ¿·áÀÇ¼E	
	eSACS_ENEMY_COUNT_IN_SIGHT,				//	½Ã¾ß³»ÀûÀÇ¼E	
	eSACS_ENEMY_COUNT_IN_TARGET_AROUND,		//	Å¸°ÙÁÖÀ§¿¡ÀûÀÇ¼E	
	eSACS_DEATH_OWNER_FORCE_IN_SIGHT,		//	½Ã¾ß³»¾Æ±ºÀÇÁ×À½
	eSACS_ON_BATTLE,						//	ÀEõ»óÈ²
	eSACS_ENABLE_CURSE_AMPLIFY,				//	ÀúÁÖ·ÎÁõÆø°¡´É
	eSACS_TARGET_IS_PET,					//	Å¸°ÙÀÌÆEÌ´Ù
	eSACS_ACTIVE_CHANCE,					//	¹ßµ¿È®·E	
	eSACS_CURRENT_HP,						//	ÇöÀçHP
	eSACS_ATTACKED,							//	°ø°İ´çÇÔ
	eSACS_CHECK_STATUS,						//	»óÅÂÃ¼Å©
	eSACS_CHECK_ANIMATE_PARTNER_COUNT,		//	¿¡´Ï¸ŞÀÌÆ® ÆÄÆ®³Ê È½¼EÃ¼Å©
	eSACS_CHECK_TRAP_COUNT		,			//	ÇÔÁ¤°³¼öÃ¼Å©
	eSACS_CHECK_SWALLOW_CORPES_COUNT	,	//	»E²½ÃÃ¼¼öÃ¼Å©

};

//
//	status
enum
{
	eSTATUS_INVISIBLE,
};

enum
{
	eSage_book_color_normal		,
	eSage_book_color_red		,
	eSage_book_color_white_blue	,
	eSage_book_color_blue		,
};

//
//	Fire Ball Animation
enum
{
	eFBA_FLY,
	eFBA_FLOAT,
	eFBA_APPEARANCE
};

//
//	¹Ì»çÀÏÀÌ ³¯¶ó°¡´Â ¹æÇE
enum
{
	eFLY_MISSILE_DIRECT_NE,
	eFLY_MISSILE_DIRECT_SE,
	eFLY_MISSILE_DIRECT_SW,
	eFLY_MISSILE_DIRECT_NW,
};

//
//	ÀÜ»EÀÌ¹ÌÁEÅ¸ÀÔ
enum
{
eSKILL_AFTER_IMAGE_TYPE_NONE,
eSKILL_AFTER_IMAGE_SELF_COPY,
eSKILL_AFTER_IMAGE_NEXT_ANIMATION,
eSKILL_AFTER_IMAGE_INCREASE_FRAME,
eSKILL_AFTER_IMAGE_WATER_CANON_STYLE,
};	//	ÀÜ»EÀÌ¹ÌÁEÅ¸ÀÔ


//	HEOP	-	Hit Effect Ouput Part
enum
{
	eHEOP_HIT_ZONE,	//	Å¸°İ ¿µ¿ª(sadÈ­ÀÏ¿¡ ¼³Á¤µÇ¾EÀÖ´Ù.)
	eHEOP_FOOT,		//	¹ß ¹Ø¿¡..
	eHEOP_SHOULDER,	//	¾ûÍú¿¡..
	eHEOP_ON_THE_HEAD,	//	¸Ó¸® ÂÊ¿¡
};

//
//	After Image Type
enum
{
	eCAIT_NONE,
	eCAIT_DELAY,
};

//
//	Explosion Missile Status
enum
{
	eEMS_FLY,
	eEMS_EXPLOSION,
};

//
//	Shake Timing
enum
{
	eST_EXPLOSION,
	eST_CAST,
};

class	cREQUIRE_SKILL
{
public:
	WORD	m_wSkill;
	WORD	m_wLevel;

			cREQUIRE_SKILL()
			{
				m_wSkill	=	0xffff;
			}
};

//
//	½ºÅ³ÀÇ ºÎ°¡ È¿°E
class	CSkillExtraEffect
{
public:
	class	cInterruptingArmor
	{
	public:
		DWORD	m_bf7BeginLevel					:	7;
		DWORD	m_bf7EndLevel					:	7;
		DWORD	m_bf1InterruptingPartEaring		:	1;
		DWORD	m_bf1InterruptingPartNecklace	:	1;
		DWORD	m_bf1InterruptingPartBoots		:	1;
		DWORD	m_bf1InterruptingPartHelm		:	1;
		DWORD	m_bf1InterruptingPartGauntlet	:	1;
		DWORD	m_bf1InterruptingPartArmor		:	1;
		DWORD	m_bf1InterruptingPartRing		:	1;
		DWORD	m_bf1InterruptingPartShield		:	1;
	};

	WORD			m_wEffect;
	short			m_sActiveChance,m_sActiveChancePerLevel;
	short			m_sUpkeepTime,m_sUpkeepTimePerLevel;
	int				m_aiValue[10];
	WORD			m_wLimitActiveChance,m_wLimitUpkeepTime;

	WORD			m_bf1IsUpkeepTimeByFrame: 1;		//1

	WORD			m_bf4TargetMethod		: 4;		//5
	WORD			m_bf1TargetUndead		: 1;		//6
	WORD			m_bf1TargetHuman		: 1;		//7
	WORD			m_bf1TargetDemon		: 1;		//8
	WORD			m_bf1TargetAnimal		: 1;		//9
	WORD			m_bf1TargetHolyBeast	: 1;		//10
	WORD			m_bf1IsActiveChaceInversePropotionByRemainHP				:	1;	//11	¹ßµ¿È®·E²ÀºÃ¼·Â¿¡¹İºñ·Ê
	WORD			m_bf1IsActiveChaceInversePropotionByRemainHPxMonsterGrade1	:	1;	//12	¹ßµ¿È®·E(³²ÀºÃ¼·ÂºñÀ²x¸ó½ºÅÍµûÍŞ(1:1,2:2,3:4,4:8)
	WORD			m_bf1IsStop				:	1;	//13	ÀÌ°Å ¼º°øÇÏ¸EºêÏ!!
	WORD			m_bf1IsCalcUpkeepTimeRoundOff	:	1;	//14	Áö¼Ó½Ã°£ ¹İ¿Ã¸²À¸·Î °è»E	
	WORD			:0;
	WORD			m_wExtraEffectImage ;		// ºÎ°¡È¿°EÀÌ¹ÌÁE
	WORD			m_wExtraEffectOutputPart;	// ºÎ°¡È¿°EÀÌ¹ÌÁEÀ§Ä¡.
					CSkillExtraEffect(){reset();}

	inline	void	reset()
	{
		memset(this,0,sizeof(CSkillExtraEffect));
		
		m_bf1TargetUndead		=	TRUE;
		m_bf1TargetHuman		=	TRUE;
		m_bf1TargetDemon		=	TRUE;
		m_bf1TargetAnimal		=	TRUE;
		m_bf1TargetHolyBeast	=	TRUE;

		m_wEffect				=	0xffff;
		m_wLimitUpkeepTime		=	0xffff;
		m_wLimitActiveChance	=	10000;
		m_wExtraEffectImage		=	0xffff;		// ºÎ°¡È¿°EÀÌ¹ÌÁE
		m_wExtraEffectOutputPart	=	eHEOP_FOOT;	// ºÎ°¡È¿°EÀÌ¹ÌÁEÀ§Ä¡.

	}
};	//	class	CSkillExtraEffect

class	CSkillExtraEffectInfo
{
public:
	WORD			m_wEffect;
	short			m_sActiveChance;
	int				m_iUpkeepTime;
	int				m_aiValue[10];

	inline	int		getNockBackDistance(int _iLevel)
	{
		return	(m_aiValue[0]+m_aiValue[1]*_iLevel)/100;
	}
};

#pragma	pack(2)
typedef union _union_skill_enchanted_image
{
    struct 
	{
		DWORD	m_bf1InclineAttackPower	:	1;
		DWORD	m_bf1InclineDefensePower:	1;
		DWORD	m_bf1InclineHitChance	:	1;
		DWORD	m_bf1InclineDodgeChance	:	1;
		DWORD	m_bf1InclineAttackSpeed	:	1;
		DWORD	m_bf1StrengthenMagicDamage:	1;
		DWORD	m_bf1StrengthenResistance:	1;
		DWORD	m_bf1InclineMoveSpeed	:	1;

		DWORD	m_bf1DeclineAttackPower	:	1;
		DWORD	m_bf1DeclineDefensePower:	1;
		DWORD	m_bf1DeclineHitChance	:	1;
		DWORD	m_bf1DeclineDodgeChance	:	1;
		DWORD	m_bf1DeclineAttackSpeed	:	1;
		DWORD	m_bf1WeaknessMagicRegistence:	1;
		DWORD	m_bf1WeaknessResistance	:	1;
		DWORD	m_bf1DeclineMoveSpeed	:	1;
		DWORD	m_bf1IsTorture			:	1;	//	17
		DWORD	m_bf1IsBloodDrain		:	1;	//
		DWORD	m_bf1IsReversalPower	:	1;	//	
		DWORD	m_bf1IsPlotOfShadow		:	1;	//	20
		DWORD	m_bf1IsBreakArmor		:	1;	//	
		DWORD	m_bf1IsBreakWeapon		:	1;	//	22

//	¿ä±â±ûİEÀÌ¸§Ç¥ À§¿¡ Ç¥½ÃµÊ
///////////////////////////////////////////////////////////////////////////////////
		DWORD	m_bf1EIFlameRing		:	1;
		DWORD	m_bf1Incinerate			:	1;
		DWORD	m_bf1VaccumPoint		:	1;
		DWORD	m_bf1HotSkin			:	1;
		DWORD	m_bf1DarkWeapon			:	1;
		DWORD	m_bf1CancerHall			:	1;	//
		DWORD	m_bf1UltimateBarrier	:	1;	//

		DWORD	m_bf1IsExistTheOtherSelf:	1;
		DWORD	m_bf1IsExistBiter		:	1;	//	31
		DWORD	m_bf1IsLevelDown		:	1;	//	32
///////////////////////////////////////////////////////////////////////////////////

		DWORD	m_bf1IsNotificationOfDeath:	1;	//
		DWORD	m_bf1IsAnimatePartner	:	1;	//	
		DWORD	m_bf1IsBlockToMove		:	1;	//

		DWORD	m_bf1IsBindByShakle		:	1;	//	1
		DWORD	m_bf1IsHiding			:	1;
		DWORD	m_bf1IsDisableInvisibleByAttack	:	1;
		DWORD	m_bf1IsRabbitRush		:	1;
		DWORD	m_bf1IsDance			:	1;
		DWORD	m_bf1IsShutInMagicBox	:	1;
		DWORD	m_bf1IsFeignDeath		:	1;
		DWORD	m_bf1IsRabbit			:	1;
		DWORD	m_bf1IsPlump			:	1;
		DWORD	m_bf1IsFrog				:	1;
		DWORD	m_bf1IsJumppingSkipping	:	1;	//	
		DWORD	m_bf1IsTransToWeapon	:	1;	//	
		DWORD	m_bf1IsImpulse			:	1;	//	
		DWORD	m_bf1IsHwabyung			:	1;	//

		DWORD	m_bf1IsSmellOfDeath		:	1;	//	18
		DWORD	m_bf1IsInterruptingArmor:	1;	//	19
		DWORD	m_bf1IsContract			:	1;	//	20
		DWORD	m_bf1IsBloodCompact		:	1;	//	21
		DWORD	m_bf1IsSoulOath			:	1;	//	22
		DWORD	m_bf1IsUseSpecialAttack	:	1;	//	23
		DWORD	m_bf1IsTaunted			:	1;	//	24
		DWORD	m_bf1IsLaziness			:	1;	//	25

		DWORD	m_bf1IsLightningBarrier	:	1;	//	26
		DWORD	m_bf1IsConversionIce	:	1;	//	27
		DWORD	m_bf1IsFogForm			:	1;	//	28
		DWORD	m_bf1IsNoMoreFight		:	1;	//	29
		DWORD	m_bf1IsZombieBomb		:	1;	//	30
		DWORD	m_bf1IsInterruptingWeapon : 1; // 31  ¹«±âÇØÁ¦
		DWORD	m_bf1IsNotUseActivityReactionSkill : 1; // 32 ¹ßµ¿ÇE®¾×¼Ç»ç¿EİÁE

		DWORD	m_bf1IsConsensus			:	1;	//1 ±³°¨ »óÅÂ.
		DWORD	m_bf1IsIncreaseAllStatus:1;		//2		¸ğµç´É·ÂÄ¡Áõ°¡.
		DWORD	m_bf1IsMadness:1;		//3		±¤¶E
		DWORD	m_bf1IsStrengtheningCriticalDamage :1;// 4 Ä¡¸úá¸´EÌÁöÁõ°¡.
		DWORD	m_bf1IsSoulBurn:1;					// 5 ¼Ò¿EE
		DWORD	m_bf1IsRageSoul:1;					//6 Å¸¿À¸£´Â¿µÈ¥.
		DWORD	m_bf1IsBindingWords:1;				// 7	¾ğ¾ûÍ¸¼Ó.
		DWORD	m_bf1IsFlee		:1	;				// 8	°øÆE.
		DWORD	m_bf1IsAddAbsoluteAttackPower				:	1;				//	9	Àı´Eª°ø°İ·ÂÃß°¡
		DWORD	m_bf1IsIncreaseAttackCount					:	1;				//	10	°ø°İÈ½¼öÁõ°¡
		DWORD	m_bf1IsIncreaseGetCP						:	1;				//	11	È¹µæCPÁõ°¡
		DWORD	m_bf1IsIncreaseAbsoluteCriticalPercent		:	1;				//	12	Àı´EªÅ©¸®Æ¼ÄÃ%Áõ°¡
		DWORD	m_bf1IsDecreaseCoolTime						:	1;				//	13	ÄğÅ¸ÀÓ°¨¼Ò
		DWORD	m_bf1IsRushTime								:	1;				//	14	µ¹ÁE		
		DWORD	m_bf1IsFightingSpirit						:	1;				//	15	ÅõÁE		
		DWORD	m_bf1IsIgnoreBlocking						:	1;				//	16	ºúÓ°Å·¹«½Ã
		DWORD	m_bf1IsReturnMissileDamage					:	1;				//	17	¹Ì»çÀÏ´EÌÁö¸®ÅÏ
		DWORD	m_bf1IsIncreaseHardBlow						:	1;				//	18	°­Å¸È®·Eõ°¡
		DWORD	m_bf1IsTransformationHunter					:	1;				//	19	»ç³É²Û ¸ó½ºÅÍ º¯½Å
		DWORD	m_bf1IsTransformationThief					:	1;				//	20	Á»µµµÏ ¸ó½ºÅÍº¯½Å
		DWORD	m_bf1IsTransformationMagician				:	1;				//	21	¸¶¹ı»E¸ó½ºÅÍº¯½Å
		DWORD	m_bf1IsTransformationReptile				:	1;				//	22	ÆÛ¹Ğ¸®¾E¸ó½ºÅÍº¯½Å
		DWORD	m_bf1IsImmunePhysicalDamage					:	1;				//	23	¹°¸®´EÌÁöÀÌ¹Ã
		DWORD	m_bf1IsGreamTant							:	1;				//	24	ºûÀÇÀå¸·
		DWORD	m_bf1IsPowerOfVitalization					:	1;				//	25	È°¼ºÈ­ÀÇ±Ç´É
		DWORD	m_bf1IsMakeTypeUndead						:	1;					//	26	¾ğµ¥µåÈ­
		DWORD	m_bf1IsElectricShock						:	1;					//	27	°¨ÀE		
		DWORD	:0;

    };

    struct 
	{
		DWORD	m_value1;
		DWORD	m_value2;
		DWORD	m_value3;
	};

	inline	void	orEffect(_union_skill_enchanted_image *_lpEffect)
	{
		m_value1	|=	_lpEffect->m_value1;
		m_value2	|=	_lpEffect->m_value2;
		m_value3	|=	_lpEffect->m_value3;
	}

	inline	void	reset()
	{
		m_value1	=	0;
		m_value2	=	0;
		m_value3	=	0;
	}

	inline	BOOL	isSame(_union_skill_enchanted_image *_lpEffect)
	{
		if	(m_value1	==	_lpEffect->m_value1	&&	m_value2	==	_lpEffect->m_value2	&&	m_value3	==	_lpEffect->m_value3)
			return	TRUE;

		return	FALSE;
	}

	inline	void	copy(_union_skill_enchanted_image *_lpEffect)
	{
		m_value1	=	_lpEffect->m_value1;
		m_value2	=	_lpEffect->m_value2;
		m_value3	=	_lpEffect->m_value3;
	}
	inline	BOOL	isExitEffect()
	{
		if	(m_value1+m_value2+m_value3)
			return	TRUE;

		return	FALSE;
	}

}	uEnchantedImage;
#pragma	pack()

enum
{
	eWAD_METHOD_CASTER		=	1,
	eWAD_MAX_EXPLOSION_RANGE,
	eWAD_EXPLOSION_RANGE	,
	eWAD_SHOOT_RANGE	,
	eWAD_TARGET_NON_TARGET,
};

//	CSkillDefine::m_wSkillUniqueSpecialFeature
enum
{
	eSUSF_IS_DISARM		=	1,
	eSUSF_IS_UNLOCK_DOOR,
	eSUSF_IS_UNLOCK_ARCA,
};


#define		dMAX_SKILL_AI_PATTERN				8
#define		dMAX_SKILL_AI_PATTERN_VALUE_COUNT	5

//
//	Skill Ai Target
enum
{
	eSAT_OWN_FORCE,
	eSAT_ENEMY,
	eSAT_SELF,
	eSAT_OWN_FORCE_CORPSE,
	eSAT_OWN_FORCE_SAME_ORDER_JOB_CORPSE,
};

class	cSkillAiPatternInfo
{
public:
	WORD	m_wTrigger;
	short	m_asValue[dMAX_SKILL_AI_PATTERN_VALUE_COUNT];
};

enum
{
	eSSD_CASTING,
	eSSD_ACTION,
	eSSD_HIT,
	eSSD_CREATE,
	eSSD_EXPLOSION,
	eSSD_MISS,

	eSSD_EE_SUCCESS,
	eSSD_EE_MISS,
};

class	CSkillDefine
{
public:
	class	CSoundInfo
	{
	public:
		char				m_strCasting[dSKILL_SOUND_FILE_LENGTH];
		char				m_strAction[dSKILL_SOUND_FILE_LENGTH];
		char				m_strHit[dSKILL_SOUND_FILE_LENGTH];
		char				m_strCreate[dSKILL_SOUND_FILE_LENGTH];
		char				m_strExplosion[dSKILL_SOUND_FILE_LENGTH];
		char				m_strMiss[dSKILL_SOUND_FILE_LENGTH];
		WORD				m_wSEE_Success,m_wSEE_Miss;

							CSoundInfo()	{reset();}

		void				reset()
		{
			m_strCasting[0]		=	0;
			m_strAction[0]		=	0;
			m_strHit[0]			=	0;
			m_strCreate[0]		=	0;
			m_strMiss[0]		=	0;
			m_wSEE_Success		=	0xffff;
			m_wSEE_Miss			=	0xffff;

		}
	};

	typedef union _union_check_status
	{
		struct 
		{
			DWORD	m_bf1IsPoison	:	1;
			DWORD	m_bf1IsContract	:	1;
			DWORD	m_bf1IsMinusCP	:	1;
		};

		struct 
		{
			DWORD	m_dwValue1;
		};

	}	uCheckStatus;

	WORD				m_wSerial;							//	½Ã¸®¾E	
	WORD				m_wIconIndex;						//	¾ÆÀÌÄÜ ÀÎµ¦½º
	WORD				m_wType;							//	Å¸ÀÔ
	WORD				m_wAction,m_wAction2;				//	±â¼E¹ßµ¿½Ã »ç¿EÏ´Â ¾×¼Ç,¾×¼ÇÀÌ 2°³ ÀÖÀ»¼öµµ ÀÖ´Ù. ÀÌ¸¦Å×¸EÆÄÀÌ¾ûÖ¼ ÁØºE´Ü°è°¡ ÀÖ°E¹ß»E´Ü°è°¡ ÀÖ´Ù.
	WORD				m_wOverlapAction,m_wOverlapAction2;	//	¿À¹ö·¦µÇ´Â ¾×¼Ç
	WORD				m_wReiterationDamageCountSyncWithOverlapAction;	//	¿À¹ö·¦µÇ´Â ¾×¼Ç°E½ÌÅ©ÇØ¼­ Ãâ·ÂÇÒ ´EÌÁEÈ½¼E	
	WORD				m_wEnableJob;						//	»ç¿E°¡´ÉÇÑ Á÷¾E0xffff¸E¸ğµEÁ÷¾÷ÀÌ °¡´É
	WORD				m_wSpeed,m_wCorrectFPS;							//	¼Óµµ

	char				m_strName[dSKILL_NAME_LENGTH];
	WORD				m_wDifficultyLevel;					//	±â¼E³­ÀÌµµ
	WORD				m_wSkillGroup;						//	±â¼E±×·E	
	WORD				m_wCastMethod;						//	¾×¼Ç,Áö¼ÓÇE¹ßµ¿ÇE¼­Æ÷Æ®,ÃæÀE
	WORD				m_wDamageAttribute;					//	´EÌÁE¼Ó¼º
	WORD				m_wTargetTypeLimit;						//	´EEÁ¦ÇÑ
	DWORD				m_dwTargetMethod;

	WORD				m_wSpentHPPercentage,m_wSpentHPPercentageBasedBloodWing;				//	¼Ò¸EHP ÆÛ¼¾Æ¼ÁE	
	WORD				m_wSpentCP,m_wSpentCPPerLevel;		//	¼Ò¸ECP
	WORD				m_wGetCP,m_wGetCPPerLevel,m_wGetCPTiming;	//	È¹µECP,CP È¹µEÅ¸ÀÌ¹Ö

	WORD				m_wMissileDustRange;					//	ÇÑ¹E»ç¿EÏ°E´Ù½Ã »ç¿EÏ¸E²¨ÁE	
	WORD				m_wIsNotSyncCP;						//	CP¼Ò¸ğ¸¦ ¼­¹ö¿Í ½ÌÅ© ½ÃÅ°ÁE¾Ê´Â´Ù.
	WORD				m_wIsMagicDamageBasedLastHitDamage;	//	¸¶¹ı ´EÌÁö°¡ ¹«±E°ø°İ·Â ±â¹İÀÌ ¾Æ´Ï°E¸¶Áö¸·¿¡ ¸Â°E¶§¸°°ÅÀÇ ´õÇÑ°Å ÇÕÀÌ´Ù.
	DWORD				m_bf1IsIgnoreWeaponDamage			:	1;	//	¹«±E´EÌÁE¹«½Ã
	DWORD				m_bf1IsPenetrationRush				:	1;	//	°EE¯½¬?
	DWORD				m_bf1IsLoopRushAni					:	1;	//	·¯½¬ ¿¡´Ï°¡ ·çÇÁÇÑ´Ù.
	DWORD				m_bf1IsAttackOnlyTargetRush			:	1;
	DWORD				m_bf1IsWhirlRunningStyleRush		:	1;
	DWORD				m_bf9IsParallellRushAngle			:	9;
	DWORD				m_bf8ParallellRushGab				:	8;
	DWORD				m_bf1IsShimmeringShieldByWeapon		:	1;
	DWORD				m_bf1IsIgnoreTargetDodgeCorrectValue:	1;	//	Å¸°ÙÀÇ È¸ÇÇ ¸ğÁ¤ ¹«½Ã
	DWORD				m_bf1IsSyncShieldShape				:	1;	//	¹æÆĞ ¸ğ¾E½ÌÅ©
	DWORD				m_bf1IsNotWhirlTornado				:	1;
	DWORD				m_bf1SetRushImageByTriggerNextFrame	:	1;
	DWORD				m_bf1BeastBerserker					:	1;
	DWORD				m_bf1IsGlareSkill					:	1;	//	29
	DWORD				m_bf1NotRevenge						:	1;
	DWORD				m_bf1IsSummonBeastCommand			:	1;	//	¼ÒÈ¯¼E¸úÓÉ 
	DWORD				m_bf1IsSummonedBeastSkill			:	1;	//	¼ÒÈ¯¼E½ºÅ³ÀÌ´Ù.

	cSkillAiPatternInfo	m_aAi[dMAX_SKILL_AI_PATTERN];
	WORD				m_wAI_Target;
	WORD				m_wLimitCrushChance;
	WORD				m_wShootRangeCorrect;
	WORD				m_wLimitShotCount;
	WORD				m_wSkillUniqueSpecialFeature;
	WORD				m_wImageRadius;
	WORD				m_wOutputEffect;
	WORD				m_wLimitActiveCount;
	WORD				m_wMissileFollowImage;						//	¹Ì½º ³´À»¶§ ÀÌ¹ÌÁE	
	BYTE				m_aTempBuffer[2];
	
	DWORD				m_bf1IsHideMissImage					:	1;
	DWORD				m_bf1IsRightPunchSkill					:	1;
	DWORD				m_bf1IsLeftPunchSkill					:	1;
	DWORD				m_bf1IsRightKickSkill					:	1;
	DWORD				m_bf1IsLeftKickSkill					:	1;
	DWORD				m_bf1IsDownKickSkill					:	1;
	DWORD				m_bf1IsCancelKnockBackSkill				:	1;
	DWORD				m_bf1IsDecreaseWeaponCount				:	1;
	DWORD				m_bf1IsInstanceApplyAura				:	1;
	DWORD				m_bf1IsDefendOnImageTrap				:	1;
	DWORD				m_bf1IsMonsterCounter					:	1;
	DWORD				m_bf1IsApplyHealPointByPercentage		:	1;
	DWORD				m_bf1IsIgnoreDodgeBlockReaction			:	1;
	DWORD				m_bf1IsAttackDamageBasedAttackerRemainHPRate	:	1;
	DWORD				m_bf1IsSelfDestructionSkill				:	1;	//	15
	DWORD				m_bf1IsRefitImageSizeByHitRange			:	1;
	DWORD				m_bf1IsCatchAndThrowingTypeAttackPower	:	1;
	DWORD				m_bf1IsTagetingToBattleWithPartyMember	:	1;
	DWORD				m_bf1IsPutHitDamage						:	1;
	DWORD				m_bf1IsNonAggressiveSkill				:	1;
	DWORD				m_bf1DefensePowerByCurseResistance		:	1;
	DWORD				m_bf1DarkDamageAttackByDamagePoint		:	1;	//	22
	DWORD				m_bf1IsPigeonPostItemSkill				:	1;
	DWORD				m_bf1IsLoopLastFrame					:	1;
	DWORD				m_bf1IsCastOnDestPosSkill				:	1;
	DWORD				m_bf1PutTrasAfterAnm					:	1;
	DWORD				m_bf1IsIllusionAttack					:	1;
	DWORD				m_bf1IsResistKnockBackByCurse			:	1;
	DWORD				m_bf1IsRequireFeignDeathStatus			:	1;
	DWORD				m_bf1IsStopWhenFailedFirstSE			:	1;
	DWORD				m_bf1IsToggleSkill						:	1;
	DWORD				m_bf1IsCastImageOnReleasPos				:	1;	//	32

	WORD				m_wDamageToHP,m_wDamageToHPPerLevel;	//	ÀÔÈE´EÌÁEÈúØE	
	WORD				m_wWADDamageMethod,m_wWADDamageGradeCount,m_wWADDamageValue,m_wWADMaxDamage,m_wWADMinDamage;

	WORD				m_bf1IsIgnoreTargetBlockingChance		:	1;	//	Å¸°ÙÀÇ ¹æÆĞ ºúÓ°·E¹«½Ã
	WORD				m_bf1IsRandomDustImageFrame				:	1;	//	Å¸°ÙÀÇ ¹æÆĞ ºúÓ°·E¹«½Ã
	WORD				m_bf1IsOnOffSkill						:	1;
	WORD				m_bf1IsGroundMissile					:	1;	//	¶¥Å¸°E³¯¾Æ°¡´Â ¹Ì»çÀÏ
	WORD				m_bf1IsApplyPhysicalDamageOnlyTarget	:	1;	//	Å¸°Ù¿¡°Ô¸¸¹°¸®´EÌÁöÀû¿E	
	WORD				m_bf1IsParallelAfterImage				:	1;			//	¼öÆEºĞ½Å
	WORD				m_bf1IsSyncSkillExtraEffectWithPlayer	:	1;//	ÇÃ·¹ÀÌ¾ûÛÍ ½ºÅ³ ºÎ°¡È¿°E½ÌÅ© ½ÃÅ´
	WORD				m_bf1IsMagicDamageBasedRemainHP			:	1;
	WORD				m_bf1IsZoomInEffectSkill				:	1;
	WORD				m_bf1IsHpSyncSkill						:	1;					//	¾ó··¶×¶¥ HP¸¦ ½ÌÅ© ½ÃÅ°´Â ½ºÅ³
	WORD				m_bf1IsTestSkill						:	1;	//	Å×½ºÆ®ÁßÀÎ ½ºÅ³
	WORD				m_bf1IsUsHealEffectToHitImageForAuraSkill:	1;	//	
	WORD				m_bf1IsNotIncreaseByActiveAura			:	1;
	WORD				m_bf1IsGetCPGasSkillByCasting			:	1;	//	14
	WORD				m_bf1IsRequireMissImage					:	1;	//	15	¹Ì½º ÀÌ¹ÌÁEÇÊ¿E
	WORD				m_bf1IsBlockInDuelField					:	1;	//	16	°áÅEÇÊµå¿¡¼­ »ç¿EºÒ°¡

	WORD				m_wCoolTimeIndex;		//	Å¸°Ù¿¡°Ô¸¸¹°¸®´EÌÁöÀû¿E	
	WORD				m_wCoolTime;			//	¼öÆEºĞ½Å
	WORD				m_wCoolTimePerLevel;	//	ÇÃ·¹ÀÌ¾ûÛÍ ½ºÅ³ ºÎ°¡È¿°E½ÌÅ© ½ÃÅ´
	WORD				m_wMinimumCoolTime;
	WORD				m_wCorrectTargetDefensivePower;
	WORD				m_wPetCommand;
	short				m_sIncreasePetPowerPerLevel;

	WORD				m_wLimitPhysicalDamage,m_wLimitMagicDamage;
	short				m_sLimitPhysicalDamagePerLevel,m_sLimitMagicDamagePerLevel;
	WORD				m_wCircleRange;						//	ºùºùµµ´Â½ºÅ³ÀÇ ¹İ°E
	short				m_sAuraActivePeriod,m_sAuraActivePeriodPerLevel,m_sAuraActivePeriodDecimalMethod;	//	¿À¶E½ºÅ³ÀÇ ¹ßµ¿ ÁÖ±E	
	WORD				m_wCureEffectField;					//	Ä¡·EÈ¿°EÇÊµE	
	short				m_sApplyLimitHPPercentage,m_sApplyLimitHPPercentagePerLevel;
	WORD				m_wLimitApplyLimitHPPercentage;
	short				m_sTargetLevelLimitType1;
	short				m_sHumanTargetLevelLimitType1,m_sAnimalTargetLevelLimitType1,m_sHolyBeastTargetLevelLimitType1,m_sDemonTargetLevelLimitType1,m_sUndeadTargetLevelLimitType1;
	short				m_sHealPercentageBasedCurrentHP,m_sHealPercentageBasedCurrentHPperLevel;
	WORD				m_wHealPoint,m_wHealPointPerLevel;	//	Ä¡·E¼öÄ¡
	short				m_sHideWeaponTime,m_sHideWeaponTimePerLevel;

	WORD				m_wWideAreaAttackDamagePercent,m_wWideAreaAttackDamagePercentPerLevel;
	WORD				m_wBlackBluesFactor,m_wBlackBluesFactorPerLevel,m_wBlackBluesFactorLimit;
	short				m_sAttackPoint,m_sAttackPointPerLevel,m_sAttackPointRange,m_sAttackPointRangePerLevel,m_sCorrectAttackPoint;	//	°ø°İ·Â
	short				m_sAttackPercentage,m_sAttackPercentagePerLevel;
	short				m_sDefensePoint,m_sDefensePointPerLevel;	//	¹æ¾ûÓÂ
	short				m_sDefensePercentage,m_sDefensePercentagePerLevel;

	short				m_sFireDamage,m_sFireDamagePerLevel,m_sFireDamageRange,m_sFireDamageRangePerLevel;	//	ºÒ ´EÌÁE	
	short				m_sFireDamagePercentage,m_sFireDamagePercentagePerLevel,m_sFireDamagePercentageLimit;
	short				m_sWaterDamage,m_sWaterDamagePerLevel,m_sWaterDamageRange,m_sWaterDamageRangePerLevel;	//	¹° ´EÌÁE	
	short				m_sWaterDamagePercentage,m_sWaterDamagePercentagePerLevel,m_sWaterDamagePercentageLimit;
	short				m_sWindDamage,m_sWindDamagePerLevel,m_sWindDamageRange,m_sWindDamageRangePerLevel;	//	¹Ù¶E´EÌÁE	
	short				m_sWindDamagePercentage,m_sWindDamagePercentagePerLevel,m_sWindDamagePercentageLimit;
	short				m_sEarthDamage,m_sEarthDamagePerLevel,m_sEarthDamageRange,m_sEarthDamageRangePerLevel;	//	´EE´EÌÁE	
	short				m_sEarthDamagePercentage,m_sEarthDamagePercentagePerLevel,m_sEarthDamagePercentageLimit;
	short				m_sLightDamage,m_sLightDamagePerLevel,m_sLightDamageRange,m_sLightDamageRangePerLevel;	//	ºE´EÌÁE	
	short				m_sLightDamagePercentage,m_sLightDamagePercentagePerLevel,m_sLightDamagePercentageLimit;
	short				m_sDarkDamage,m_sDarkDamagePerLevel,m_sDarkDamageRange,m_sDarkDamageRangePerLevel;	//	¾ûÑÒ ´EÌÁE	
	short				m_sDarkDamagePercentage,m_sDarkDamagePercentagePerLevel,m_sDarkDamagePercentageLimit;

	short				m_sPiercingChance,m_sPiercingChancePerLevel;

	WORD				m_wSpecialEffect;
	CSkillExtraEffect	m_aExtraEffect[dEXTRA_EFFECT_COUNT];
	WORD				m_wApplyExtraEffectCount;
	WORD				m_wActivateTrigger;
	short				m_sNockbackDistance,m_sNockbackDistancePerLevel;	//	³EE°Å¸®

	short				m_sCastingTime;		//	Ä³½ºÆÃ Å¸ÀÓ 
	short				m_sAttackSpeed,m_sAttackSpeedPerLevel,m_sLimitAttackSpeed;		//	°ø°İ ¼Óµµ(1/16 ÇÁ·¹ÀÓ ´ÜÀ§ * 100)
	short				m_sCorrectAttackSpeed,m_sCorrectAttackSpeedPerLevel;

	WORD				m_wMinimumShootRange;					//	ÃÖ¼Ò »çÁ¤ °Å¸®
	WORD				m_wShootRange,m_wShootRangePerLevel;	//	»çÁ¤ °Å¸®
	WORD				m_wWeaponShootRangeCorrect,m_wWeaponShootRangeCorrectPerLevel;
	
	WORD				m_wMinimumHitRange,m_wHitRange,m_wHitRangePerLevel;		//	 Å¸°İ ¹E§
	WORD				m_wWeaponHitRangeCorrect;
	WORD				m_wAngle;							//	ºÎÃ¤²Ã ´EÌÁöÀÌ´Ù.

	short				m_sHitChance,m_sHitChancePerLevel,m_sFixHitChance;	//	¸úİß·Eº¸Á¤,·¹º§´E¸úİß·Eº¸Á¤,¸úİß·E	
	short				m_sDodgeChance,m_sDodgeChancePerLevel,m_sFixDodgeCahnce;	//	È¸ÇÇ·Eº¸Á¤,·¹º§´EÈ¸ÇÇ·Eº¸Á¤,È¸ÇÇ·E	
	short				m_sCriticalChance,m_sCriticalChancePerLevel,m_sFixCriticalChance;	//	Ä¡¸úá¸
	short				m_sCrushChance,m_sCrushChancePerLevel,m_sFixCrushChance;	//	Å©·¯½¬¼¦
	short				m_sCriticalToUndead,m_sCriticalToUndeadPerLevel,m_sCriticalToDemon,m_sCriticalToDemonPerLevel,m_sCriticalToAnimal,m_sCriticalToAnimalPerLevel;
	short				m_sCriticalToHuman,m_sCriticalToHumanPerLevel,m_sCriticalToHolyAnimal,m_sCriticalToHolyAnimalPerLevel;//°¢Á¾ ¸ó½ºÅÍ¿¡ ´EÑ Å©¸®Æ¼ÄÃ º¸Á¤Ä¡
	WORD				m_wInstanceKillMethod;	//	ÁEE°è»E¹æ¹ı
	short				m_sInstanceKillToUndead,m_sInstanceKillToDemon,m_sInstanceKillToAnimal,m_sInstanceKillToHuman,m_sInstanceKillToHolyAnimal;//°¢Á¾ ¸ó½ºÅÍ¿¡ ´EÑ ÁEEº¸Á¤Ä¡
	short				m_sInstanceKillToUndeadPerLevel,m_sInstanceKillToDemonPerLevel,m_sInstanceKillToAnimalPerLevel,m_sInstanceKillToHumanPerLevel,m_sInstanceKillToHolyAnimalPerLevel;//°¢Á¾ ¸ó½ºÅÍ¿¡ ´EÑ ÁEEº¸Á¤Ä¡
	short				m_sBlockingChance,m_sBlockingChancePerLevel,m_sMaximumBlockingChance,m_sFixBlockingChance;	//	ºúÓ°Å· º¸Á¤,·¹º§´EºúÓ°Å· º¸Á¤,ºúÓ°Å·
	short				m_sConcentration,m_sConcentrationPerLevel;	//	ÁıÁß·Â º¸Á¤,·¹º§´EÁıÁß·Â º¸Á¤,ÁıÁß·Â

	short				m_sStunResistance,m_sStunResistancePerLevel;	//	¸¶ºEÀúÇ×
	short				m_sStrangeStatusResistance,m_sStrangeStatusResistancePerLevel;	//	ÀÌ»ó°EÀúÇ×
	short				m_sDeclineResistance,m_sDeclineResistancePerLevel;	//	ÀúÇÏ°EÀúÇ×
	short				m_sCurseResistance,m_sCurseResistancePerLevel;	//	ÀúÁÖ°EÀúÇ×

	short				m_sFireResistance,m_sFireResistancePerLevel;	//	ºÒ ÀúÇ×
	short				m_sWaterResistance,m_sWaterResistancePerLevel;	//	¹° ÀúÇ×
	short				m_sWindResistance,m_sWindResistancePerLevel;	//	¹Ù¶EÀúÇ×
	short				m_sEarthResistance,m_sEarthResistancePerLevel;	//	´EEÀúÇ×
	short				m_sLightResistance,m_sLightResistancePerLevel;	//	ºEÀúÇ×
	short				m_sDarkResistance,m_sDarkResistancePerLevel;	//	¾ûÑÒ ÀúÇ×

	short				m_sBulletCount,m_sBulletCountPerLevel,m_sBulletLimitCount,m_wBulletDecimalMethod;
	short				m_sShotCount,m_sShotCountPerLevel,m_wShotCountDecimalMethod;	//	µ¿½Ã °ø°İ(?) È½¼E ¼Ò¼öÁ¡ Ã³¸® ¸Ş¼ÒµE	
	short				m_sContiniousShotMaxCount,m_sContiniousShotCount,m_sContiniousShotCountPerLevel,m_sContiniousShotPeriod,m_wContiniousShotDecimalMethod;//¿¬¼Ó »ç°İ
	short				m_wIncreaseActionAnmCount;

	short				m_sCorrectTargetHitChance,m_sCorrectTargetHitChancePerLevel;	//	ÀE¸úİß·Eº¸Á¤

	WORD				m_wUpkeepTime,m_wUpkeepTimePerLevel;	//	À¯ÁE½Ã°£

	short				m_sTargetFleeChance,m_sTargetStunChance;
	short				m_sInstanceKillChance,m_sInstanceKillChancePerLevel;	//	Å¸°ÙÀÌ µµ¸Á °¡°Å³ª ½ºÅÏµÇ°Å³ª ÁEE_-
	short				m_sTargetDecreaseHitChance,m_sTargetDecreaseHitChancePerLevel;	//	¸ÂÀº³ÑÀº ¸úİß·EÀúÇÏ

	short				m_isExemptToDemonType,m_isExemptToHumanType,m_isExemptToAnimalType,m_isExemptToHolyAnimalType,m_isExemptToUndeadType;	//	¿µÇâÀ» ¾È ¹Ş´Â ¸ó½ºÅÍ
	short				m_sIntelligencePerLevel,m_sStrengthPerLevel,m_sLuckPerLevel,m_sAgilityPerLevel,m_sWisdomPerLevel,m_sCharismaPerLevel;

	short				m_sOccurActionPeriod;	//	º¸Á¶¸¶¹ı¿¡¼­ Àû¿EÁÖ±E
	short				m_sHealPoint,m_sHealPointPerLevel;	//	Ã¼·Â È¸º¹ ¼öÄ¡
	short				m_sFirstAidPoint,m_sFirstAidPointPerLevel,m_sFirstAidUpkeepTime;	//	ÀÀ±Ş Á¶Ä¡
	short				m_sDisarmLevel,m_sDisarmLevelPerLevel,m_sDetectLevelPerLevel,m_sDetectLevel;	//	ÇØÃ¼ ·¹º§,Å½ÁE·¹º§
	short				m_sActiveChance,m_sActiveChancePerLevel;		//	¹ßµ¿È®·E¹ßµ¿È®·E·¹º§ º¸Á¤
	WORD				m_wActiveChanceByInRangeEnemyCount;
	WORD				m_wLimitActiveChance;

	WORD				m_wRequireEquipment;							//	ÇÊ¿EÀåºE	
	cREQUIRE_SKILL		m_aRequireSkill[dMAX_REQUIRE_SKILL_COUNT];

	CSoundInfo			m_sound;

	short				m_aValue[dSKILL_SPARE_VALUE_COUNT];		//	¿©ºĞÀÇ ¼öÄ¡..

//	½ºÅ³ ±¸ÇöÀ» À§ÇØ ÇÊ¿äÇÑ µ¥ÀÌÅÍ
	WORD				m_wImageScale;
	WORD				m_wTargetMarkImage;
	WORD				m_wShootImage;						//	³¯¾Æ°¥¶§ ÀÌ¹ÌÁE	
	WORD				m_wMissileHeadImage;				//	³¯¾Æ°¡´Â ¹«±E¸Ó¸®¿¡ ÂEEÀÌ¹ÌÁE	
	WORD				m_wMachineImage;					//	¸Ó½Å ÀÌ¹ÌÁE	
	WORD				m_wExplosionImage;					//	Æø¹ßÇÒ¶§ ÀÌ¹ÌÁE	
	WORD				m_wMissImage;						//	¹Ì½º ³´À»¶§ ÀÌ¹ÌÁE
	WORD				m_wAidAttackImage,m_wAidAttackImageOutputPart,m_wAidAttackImageEffect;
	WORD				m_wHitImage,m_wHitImageOutputPart,m_wHitImageEffect;
	WORD				m_wHealImage,m_wHealImageOutputPart,m_wHealImageEffect;
	WORD				m_wAddHitImage,m_wAddHitImageOutputPart,m_wAddHitImageEffect;
	WORD				m_wCastImage,m_wCastImageOutputPart,m_wCastImageEffect;
	WORD				m_wAidSkillCastingImage,m_wAidSkillCastingImageOutputPart,m_wAidSkillCastingImageEffect;	//	»ç¿EÒ¶§ ½ÃÀEÚ¿¡°Ô ³ªÅ¸³ª´Â ÀÌ¹ÌÁE	
	WORD				m_wSkillImage,m_wSkillImageOutputPart,m_wSkillImageEffect;						//	½ºÅ³ ÀÌ¹ÌÁE	
	WORD				m_wCasterHitImage,m_wCasterHitImageOutputPart,m_wCasterHitImageEffect;		//	Å¸°İÀÌ¹ÌÁEÃâ·Â ºÎÀ§
	WORD				m_wSwingImage,m_wSwingImageEffect;						//	¹«±â¸¦ ÈÖµÎ¸¦¶§ ³ªÅ¸³ª´Â ÀÌ¹ÌÁE	
	WORD				m_wBottomImage;	//	¹Ù´Ú ÀÌ“G

	WORD				m_wDodgeAngle;						//	È¸ÇÇ °¢
	WORD				m_wHitAngleRange,m_wHitAngleRangePerLevel;	//	Å¸°İ°¢µµ¹E§
	WORD				m_wDodgeDistance;					//	È¸ÇÇ °Å¸®
	WORD				m_wPaletteIndex;

	DWORD				m_dwEnchantedEffectMask;			//	¸¶¾Æ½ºÅ©~
	WORD				m_wEnchantedImage;					//	ÀÎÃ¦Æ® ÀÌ¹ÌÁE	
	WORD				m_wDustImageRange;

	WORD				m_wShakeTiming;						//	¾ğÁ¦ Èçµé²¸?
	WORD				m_wShakeIntensity;					//	ÁöÁøÀÇ °­µµ
	WORD				m_wShakeTime;						//	ÁöÁøÀÌ À¯ÁöµÇ´Â ½Ã°£..

	BYTE				m_bCharacterAfterImageType;			//	¸ö¶×¾Æ¸® ÀÜ»EÅ¸ÀÔ
	BYTE				m_bCharacterAfterImageDelayTime;	//	µô·¹ÀÌ Ä³¸¯ÅÍ ÀÜ»Eµô·¹ÀÌ Å¸ÀÓ
	WORD				m_wStrikePeriod;
	short				m_sStrikePeriodPerLevel;
	WORD				m_wMinimumStrikePeriod;

	WORD				m_wAfterImageType;					//	ÀÜ»EÅ¸ÀÔ
	WORD				m_wAfterImageGap;					//	ÀÜ»E°£°İ
	WORD				m_wAfterImageFirstImageDistance;	//	µÎ¹øÂ° ÀÌ¹ÌÁEÀ§Ä¡
	WORD				m_wAfterImageFirstImageAlphaDepth,m_sAfterImageDecreaseAlphaDepthValue;	//	ÀÜ»EÃ¹¹øÂ° ÀÌ¹ÌÁö¿Í ½Ç ÀÌ¹ÌÁöÀÇ °£°İ
	WORD				m_wAfterImageCount;					//	ÀÜ»óÀÇ ¼E
	DWORD				m_bf1IsBlockOnlyMissilAttack	: 1;
	DWORD				m_bf1IsExclusiveAction			: 1;
	DWORD				m_bf5HitDamagePeriod			: 5;	//	´EÌÁEÀÔÈ÷´Â ÁÖ±E	
	DWORD				m_bf1IsRoundRappedBunshinAttack	: 1;	//	ºĞ½Å °ø°İÀÎµ¥... ÀûÀ» µÑ·¯½Ñ´Ù.
	DWORD				m_bf5BunshineAlphaDepth			: 5;	//	ºĞ½Å ³óµµ
	DWORD				m_bf3MaxPetCount				: 3;
	DWORD				m_bf2RequireSummonBeastGrade	: 2;
	DWORD				m_bf2OperateSummonBeast			: 2;
	DWORD				m_bf1IsPohibitAction			: 1;
	DWORD				m_bf1ShootRangeZeroSkill		: 1;
	DWORD				m_bf1IsTrap						: 1;
	DWORD				m_bf1IsFlatTrap					: 1;
	DWORD				m_bf1IsInstanceHeal				: 1;
	DWORD				m_bf1IsOnlySelfEnchantSkill		: 1;	//	ÀÚ±âÇÑÅ×¸¸ ÀÎÃ¦Æ® ÇÏ´Â ½ºÅ³ÀÌ´Ù.
	DWORD				m_bf1IsSecondJobMachine			: 1;
	DWORD				m_bf1IsRapeExplosionImage		: 1;	//	28
	DWORD				m_bf1IsLaser					: 1;	//	29
	DWORD				m_bf1AttackByDamagedDamage		: 1;	//	30
	DWORD				m_bf1PlayCastSoundByEnchantAidSkill	: 1;	//	31
	DWORD				m_bf1RootAttackPower			: 1;	//	32

	uEnchantedImage		m_enchantedImage;
	WORD				m_wCommandSkill;

	uCheckStatus		m_checkTargetStatus,m_checkCasterStatus;

	DWORD				m_bf1IsObitianSkill				:	1;
	DWORD				m_bf1IsDashBladeSkill			:	1;
	DWORD				m_bf1IsAstroBowSkill			:	1;
	DWORD				m_bf1IsCristalWaterSkill		:	1;
	DWORD				m_bf1IsApplyToPartyAura			:	1;
	DWORD				m_bf4ChangeDirect				:	4;
	DWORD				m_bf1IsCastSelfBuff				:	1;
	DWORD				m_bf1IsExplosionAtCastPos		:	1;	//	11
	DWORD				m_bf1InsInstanceWaterFall		:	1;
	DWORD				m_bf5MiniPetType				:	5;
	DWORD				m_bf1IsDefaultMiniPetSkill		:	1;	//	18
	DWORD				m_bf5MiniPetSkillType			:	5;	//	23
	DWORD				m_bf1IsXMiniPetLevel			:	1;	//	24
	DWORD				m_bf4PetSkillDamageExpressionType	:	4;	//	28
	DWORD				m_bf1IsNormalMiniPetSkill		:	1;	//	29
	DWORD				m_bf1IsDuelCoolTimeSkill		:	1;	//	30
	DWORD				m_bf1IsShadowHideSkill			:	1;	//	31	µµµÏ ±×¸²ÀÚ ¼û±E½ºÅ³ÀÌ³Ä?.
	DWORD				m_bf1IsApplyShadowHideEffect	:	1;	//	32	µµµÏ ±×¸²ÀÚ ¼û±E½ºÅ³È¿°EÀû¿E	
	DWORD				m_bf7DuelServerpenaltyDiv		:	7;	//	µà¾ó¼­¹E¿¡¼­ Æä³ÎÆ¼¸¦ ¹Ş´Â ±â¼E(³ª´©¾ûØ­ ¾´´Ù.)_ / 0 ~ 127 ±ûİE	
	DWORD				m_bf4PetSkillDamageExpressionTypeForAwaken	:	4;	//	11
	DWORD				m_bf6NeedState					:	6	;// 17		½ºÅ³À» »ç¿EÏ±EÀ§ÇÑ ÇÊ¿ä»óÅÂ
	DWORD				m_bf1IsCancelNeedState			:	1	; // 18  ÇÊ¿ä»óÅÂÇØÁ¦
	DWORD				m_bf1IsReactionLimitCount		:	1	; //19 ¸®¾×¼Ç¹ßµ¿¼öÁ¦ÇÑ...
	DWORD				m_bf1IsIgnoreOptionAttackSpeed	:	1	;// 20	°ø°İ¼Óµµ¾ÆÀÌÅÛ¿É¼Ç¹«½Ã.
	DWORD				m_bf1IsIgnoreLucky				:	1	;	// 21 ¿ûÕ«½Ã.
	DWORD				m_bf1IsRushAttack				:	1	;	//	 22 µ¹ÁE¾ûáÃ...
	DWORD				m_bf1IsIgnoreDelayAfterAttack	:	1	;	//	 23 °ø°İÈÄ µô·¹ÀÌ ¹«½Ã
	DWORD				m_bf1IsApplyEffectChanceToPlayer	:	1	;	//	 24 ÇÃ·¹ÀÌ¾EºÎ°¡È¿°EÈ®·Eû¿E	
	DWORD				m_bf1IsApplyFightingSpirit			:	1	;	//	 25 ÅõÁö¿µÇâ¹Ş´Â ½ºÅ³
	DWORD				m_bf1IsCanNotApplySameEffect			:	1	;	//	 26 °°Àº ºÎ°¡È¿°EÀû¿E¾ÈµÊ(ÀÏºÎ ´É·ÂÄ¡ »ó½Â ¹öÇÁ¸¸)
	DWORD				m_bf1IsTargetingToActorStickedBit		:	1	;	//	 27 ºñÆ®ºÙÀº Å¸°Ù
	DWORD				m_bf4SpendBit							:	4	;	//	 31 ¼Ò¸ğºñÆ®
	DWORD				m_bf1IsApplyCriticalEffectWhenBeLightEffect :	1;	//	 32	ºû¼Ó¼º°EÃ»óÅÂÀÌ»óÀÏ¶§ Å©¸®Æ¼ÄÃÈ¿°úÀû¿E	
	DWORD				:0;
	//JBC		¹Ì´ÏÆE3ÂE°¢¼º °¢¼ºÀÇ ¾¾¾Ñ ¿­¸Åµ¥¹ÌÁö¸¦ °è»EÒ¶§ ÇÊ¿äÇÑ º¯¼öµE09-01-07
	WORD				m_wAwakenSkillBonusActiveChance;			//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º ¹ßµ¿ È®·E°úİ¤).
	WORD				m_wAwakenSkillBonusActiveChancePerLevel;	//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º ¹ßµ¿ È®·E·¹º§´E.
	
	short				m_wAwakenSkillBonusDamage;					//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º µ¥¹ÌÁE°úİ¤).
	short				m_wAwakenSkillBonusDamagePerLevel;			//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º µ¥¹ÌÁE·¹º§´E.
	short				m_wAwakenSkillBonusDamageForOrigin;			//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º µ¥¹ÌÁEÁøÈ­ÀE¼ö½ÄÀÇ °úİ¤°ª)¿¡ Ãß°¡°ª.
	short				m_wAwakenSkillBonusDamagePerLevelForOrigin;	//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º µ¥¹ÌÁEÁøÈ­ÀE¼ö½ÄÀÇ ·¹º§´E°ª)¿¡ Ãß°¡°ª.
	short				m_wAwakenSkillBonusDamageValueRangeForOrigin;//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º µ¥¹ÌÁEÁøÈ­ÀE¼ö½ÄÀÇ +-°ª)¿¡ Ãß°¡°ª.
	short				m_wAwakenSkillBonusDamageValueRangePerLevelForOrigin;//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º µ¥¹ÌÁEÁøÈ­ÀE¼ö½ÄÀÇ ·¹º§´E+-°ª )¿¡ Ãß°¡°ª.
	
	//JBC		¹Ì´ÏÆE4ÂE°¢¼º °¢¼ºÀÇ ¾¾¾Ñ ¿­¸Å°­È­¿¡ Àû¿EÉ º¯¼öµE9-04-06
	short				m_wAwakenSkillBonusRange;					//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º »çÁ¤ °Å¸®(°úİ¤).
	short				m_wAwakenSkillBonusRangePerLevel;			//°¢¼ºÀÇ ¾¾¾Ñ º¸³Ê½º »çÁ¤ °Å¸®(·¹º§´E..
	
	short				m_wAwakenSkillBonusDamageForBoJoSkill;			//°¢¼ºÀÇ ¾¾¾Ñ º¸Á¶½ºÅ³À» À§ÇÑ º¸³Ê½º µ¥¹ÌÁE°úİ¤).
	short				m_wAwakenSkillBonusDamageForBoJoSkillPerLevel;	//°¢¼ºÀÇ ¾¾¾Ñ º¸Á¶½ºÅ³À» À§ÇÑ º¸³Ê½º µ¥¹ÌÁE·¹º§´E..
	
	short				m_wAwakenSkillBonusActiveChanceForBoJoSKill;					//°¢¼ºÀÇ ¾¾¾Ñ º¸Á¶½ºÅ³¿¡ ºÙÀÏ º¸³Ê½º ¹ßµ¿ È®·E°úİ¤).
	short				m_wAwakenSkillBonusActiveChanceForBoJoSKillPerLevel;			//°¢¼ºÀÇ ¾¾¾Ñ º¸Á¶½ºÅ³¿¡ ºÙÀÏ º¸³Ê½º ¹ßµ¿ È®·E·¹º§´E.
	
	WORD				m_wApplyEffectChanceToPlayer;
	WORD				m_wApplyEffectChancePerLevelToPlayer;
	WORD				m_wApplyEffectChanceLimitToPlayer;

	WORD				m_bf1IsApplyMinDamageWhenBeNotLightEffect			:1	;	// ºû°EÃ»óÅÂÀÌ»óÈ¿°ú°¡ ¾Æ´Ò¶§ ÃÖ¼Ò´EÌÁEÀû¿E	
	WORD				m_bf1IsApplyExplosionWhenHitTargetStickedBit		:1	;	//2 ºñÆ®ºÎÂøÁßÀÎ Å¸°Ù È÷Æ®½Ã Æø¹ßÀû¿E	
	WORD				m_bf1IsApplySameTarget								:1	;	//3 Áßº¹Å¸°ÙÀû¿E	
	WORD				m_bf11ArcHeight										:11	;	//14 Æ÷¹°¼±³ôÀÌ
	WORD				bf1IsAbleSkillWhenTransToWeapon						:1	;	//15 ¹«±âº¯½ÅÁß¿¡µµ»ç¿E¡´É
	WORD				:0;	

	short				m_sAttackPointPerActorLevel;
	short				m_sAddDamagePercent;

	WORD				m_bf5MagicDamageDiceCount						:	5;		//	5	¸¶¹ı´EÌÁEÁÖ»çÀ§±¼¸²È½¼E	
	WORD				m_bf5MagicDamageDiceCountLimit					:	5;		//	10	¸¶¹ı´EÌÁEÁÖ»çÀ§±¼¸²È½¼öÇÑ°E	
	WORD				:0;
	short				m_sMagicDamageDiceCountPerLevel;							//	¸¶¹ı´EÌÁEÁÖ»çÀ§±¼¸²per·¹º§	

	BYTE				m_abSpareSkillBuffer[12];

	char				m_strComment[dSKILL_COMMENT_LENGTH];	
	char				m_strPowerup[dSKILL_POWER_UP_COMMENT_LENGTH];
};


class	CAbilityDefine
{
public:
	WORD			m_wSkill;
	WORD			m_wLevel;
};

enum
{	// ÇÊ¿ä»óÅÂ.
	eDAMAGE_NORMAL,		// ÀÏ¹İ
	eDAMAGE_MIN	,	// ÃÖ¼Ò´EÌÁE	
	eDAMAGE_MAX	,	// ÃÖ´EEÌÁE
};



class	cFighterAttackInfoE
{
public:
	WORD	m_wLevel1,m_wLevel2;
	WORD	m_wMinAttackPower,m_wMaxAttackPower;
	WORD	m_wAttackSpeed,m_wAttackRange;
};

class	cFighterAttackInfo
{
public:
	int						m_iCount;
	cFighterAttackInfoE		m_aData[100];

	cFighterAttackInfoE*	get(int _iLevel)
	{
		for (int i=0;i<m_iCount;i++)
			if (_iLevel >= m_aData[i].m_wLevel1 && _iLevel <= m_aData[i].m_wLevel2)
				return	&m_aData[i];

		return	NULL;
	}

	int		getMinAttackPower(int _iLevel)
	{
		return	get(_iLevel)->m_wMinAttackPower;
	}
	int		getMaxAttackPower(int _iLevel)
	{
		return	get(_iLevel)->m_wMaxAttackPower;
	}
	int		getAttackSpeed(int _iLevel)
	{
		return	get(_iLevel)->m_wAttackSpeed;
	}
	int		getAttackRange(int _iLevel)
	{
		return	get(_iLevel)->m_wAttackRange;
	}
};

extern	cFighterAttackInfo	g_fighterAttackInfo;

enum
{
	eSKILL_DATA_VERSION_FIRST,
	eSKILL_DATA_VERSION_ENCRYPT = eSKILL_DATA_VERSION_FIRST,
	eSKILL_DATA_VERSION_ENCRYPT2,
		
	eSKILL_DATA_VERSION_LAST,
	eSKILL_DATA_VERSION_CURRENT	=	eSKILL_DATA_VERSION_LAST-1,
};

#define	dSKILL_DATA_HEADER_SIGN	0xabcd1234

#endif