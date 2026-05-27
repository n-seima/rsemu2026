#ifndef _classItemDefine_h
#define _classItemDefine_h

#include "SFC.H"
#include "CItemEffectDefine.H"
#include "book_define.H"
#include "playerJobCommon.H"

#pragma pack(2)

#define	dITEM_NAME_LENGTH				50
#define	dNAME_LENGTH					18

#define	dITEM_UNIQUE_DATA_COUNT			6			//	유니크 데이터 수
#define	dITEM_UNIQUE_DATA_VALUE_COUNT	8
#define	dGENERATE_ITEM_DATA_COUNT		4
#define	dGENERATE_ITEM_DATA_VALUE_COUNT	4

#define	dITEM_KIND_ALL					0xffff		//	모든 종류의 아이템
#define	dITEM_KIND_WASTING				0xfffe		//	모든 종류의 아이템
#define	dMAX_ITEM_COUNT_BY_KIND			400

#define	dOWN_ITEM_COUNT					42
#define	dMONSTER_ITEM_COUNT				10			//	몬스터가 가지고 있을 수 있는 아이템의 최대 수

#define	dITEM_PREFIX_COUNT				3

#define	dNOT_QUEST_ITEM					9999

#define	dBORDER_OF_ITEM_AND_EQUIPMENT	100
#define	dDEFAULT_ITEM_SLOT_COUNT		30

#define	dITEM_SHAPE_HEIGHT				34
#define	dITEM_SHAPE_WIDTH				34

#define	dOWN_ITEM_COUNT							42
#define	dEQUIPMENT_PART_COUNT					23
#define	dEQUIPMENT_SKILL_SLOT				200			// 10.01.06 스킬슬롯에 착용하는 아이템 구분
#define	dLIMIT_BADGE_ITEM_COUNT_IN_INVENTORY	5 // 20240904 rekunn 3
#define	dDX_ITEM_REPAIR_FACTOR					36
#define	dULTIMATE_ITEM_REPAIR_FACTOR			36

#define	dBASIC_ITEM_COUNT						8192
#define	dLIMIT_DROP_GOLD						50000  // 20240904 rekunn 10000to50000
#define	dITEM_LITTLE_GOOD_PREFIX_DROP_LEVEL		5

#define	dITEM_FULL_CHARGE_TIME					(60*24*10)

#define	dVALID_PLAYER_WAYPOINT			0xf
#define	dITEM_INDEX_GOLD_BAR			4237			// 09.08.19
#define	dITEM_MAX_GOLD_BAR_STACK		100				// 09.10.08
#define	dMAX_GOLD_BAR_STACK_FOR_JAPAN	10			// 일본 최대 금괴 스택제한은 10개이다.
#define	dITEM_INDEX_ATTACH_PREFIX			4437			// 접두사 붙여넣기아이템
#define	dITEM_INDEX_ATTACH_PREFIX_CAN_NOT_TRADE			4459			// 접두사 붙여넣기아이템 거래불가.


#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B1			3958			// 길드던전 b1 층 인덱스
#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B2			3959			// 길드던전 b2 층 인덱스
#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B3			3960			// 길드던전 b3 층 인덱스
#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B4			3961			// 길드던전 b4 층 인덱스
#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B5			3962			// 길드던전 b5 층 인덱스
#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B6			3963			// 길드던전 b6 층 인덱스
#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B7			3964			// 길드던전 b7 층 인덱스
#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B8			3965			// 길드던전 b8 층 인덱스
#define	dITEM_INDEX_GUILD_DUNGEON_TICKET_B9			3966			// 길드던전 b9 층 인덱스

const	int	c_iFireWorkKindCount	=	10;

#define	dDROPPING_GOLD_RANGE			10

#define	dDROP_ITEM_SEAL_TIME			300		//	300초면 6분
#define	dITEM_MONEY						0	//	돈 -o-
#define	dPICK_ABLE_RANGE				(4*110*110) // 2024 0904 rekunn (110*110)

#define	dDEFENSIVE_EQUIPMENT_COUNT		5
#define	dMAX_ITEM_ADDITIONAL_DATA		500
#define	dCOUNT_OF_ITEM_KIND				60

#define	dRANGE_OF_USE_ITEM				(200*200)

#define	dDROP_ITEM_PATTERN_COUNT		10
#define	dDEFAULT_SHIELD_RIGIDITY_TIME	4		//	디폴트 방패 경직 시간(방패로 블럭 했을때 그 자세 그대로 굳어-_-져있는 시간)
#define	dBELT_SLOT_COUNT				5

#define	dVALID_ITEM						0xffffffff

#define	dEQUIP_WEAPON					0			//	무기
#define	dEQUIP_SHIELD_BULLET			1			//	방패/탄환
#define	dEQUIP_ARMOR					2			//	갑옷
#define	dEQUIP_GLOVE					3			//	장갑
#define	dEQUIP_HELM						4			//	헬멧 종류
#define	dEQUIP_EARING					5			//	귀거리
#define	dEQUIP_NECKLACE					6			//	목걸이
#define	dEQUIP_BELT						7			//	벨트
#define	dEQUIP_SHOES					8			//	신발
#define	dEQUIP_RING						9			//	반지
#define	dEQUIP_RING1					9			//	반지
#define	dEQUIP_RING2					10			//	반지
#define	dEQUIP_RING3					11			//	반지
#define	dEQUIP_RING4					12			//	반지
#define	dEQUIP_RING5					13			//	반지
#define	dEQUIP_RING6					14			//	반지
#define	dEQUIP_RING7					15			//	반지
#define	dEQUIP_RING8					16			//	반지
#define	dEQUIP_WEAPON_2					17			//	무기2
#define	dEQUIP_BELT_SLOT1				18			//	벨트 슬롯 1
#define	dEQUIP_BELT_SLOT2				19			//	벨트 슬롯 2
#define	dEQUIP_BELT_SLOT3				20			//	벨트 슬롯 3
#define	dEQUIP_BELT_SLOT4				21			//	벨트 슬롯 4
#define	dEQUIP_BELT_SLOT5				22			//	벨트 슬롯 5
#define	dEQUIPMENT_PART_COUNT			23
#define	dITEM_INDEX_STONEOFMYSTERY		3955			// 09.08.26 신비석
#define	dITEM_INDEX_STONEOFMYSTERY_PIECE	3954		// 09.09.07 신비석 조각 
#define	dITEM_INDEX_TANTILLESS			3957			// 09.08.26 탄틸리스 유물
#define	dITEM_INDEX_TANTILLESS_PIECE		3956			// 09.08.26 탄틸리스 유물 조각	
#define dITEM_INDEX_CRYSTAL				3828			// 09.08.26 결정석
#define	dETERNAL_ITEM_MAX_LEVEL			10		// 이터널아이템 최고레벨	09.08.25
#define	dLIMIT_EQUIP_RING_OF_INFINITY_COUNT	1			//	 무한의 반지 착용 제한 전생 회차당 개수.
#define	dCARVING_LUCK_FACTOR		0.0005
//
//	CBIP - Compute Basic Item Price
enum
{
	eCBIP_PRICE,	//	가격으로 처리
	eCBIP_VALUE1,	//	수치 1으로 처리
	eCBIP_PRICE_MUL_VALUE1,	//	가격 * 수치1
	eCBIP_PRICE_MUL_VALUE2,	//	가격 * 수치2
	eCBIP_PRICE_MUL_VALUE1_PLUS1,	//	가격 * (수치1 +1)
	eCBIP_VALUE1_MUL_VALUE2_DIV_PRICE,	//	수치1*수치2/가격
	eCBIP_PRICE_DIV_10,	//	가격 / 10
	eCBIP_VALUE1_MUL_VALUE2_MUL_PRICE,	//	수치1*수치2*가격
	eCBIP_LEVEL_PER_GOLD
};

//
//	CIPP - Compute Item Price by Prefix
enum
{
	eCIPP_PRICE,	//	가격으로 처리
	eCIPP_VALUE1,	//	수치1
	eCIPP_VALUE1_MUL_PRICE,	//	가격 * 수치1
	eCIPP_VALUE1_DIV_PRICE,	//	수치1 / 가격
	eCIPP_VALUE1_MUL_PRICE_DIV_100,	//	수치1*가격/100
	eCIPP_PRICE_MUL_VALUE2_DIV_VALUE1,//가격*수치2/수치1
};

const	int	c_iaCarvingData[][2] =
{	// 각인 단계별 업그레이드 비용
	// 돈 x만 , 결정석 개수
	{50, 0},
	{100, 0},
	{150, 0},
	{200, 0},
	{250, 0},

	{300, 0},
	{400, 1},
	{500, 1},
	{600, 1},
	{700, 1},
//10
	{800, 1},
	{900, 1},
	{1000, 1},
	{1200, 1},
	{1400, 1},

	{1600, 1},
	{1800, 1},
	{2000, 1},
	{2200, 1},
	{2400, 1},
//20
	{2600, 1},
	{2800, 1},
	{3000, 1},
	{3500, 1},
	{4000, 1},

	{4500, 1},
	{5000, 1},
	{5500, 1},
	{6000, 1},

};


const	int	c_aItemEquipPlace[]	=
{
	dEQUIP_HELM,//"헬멧",
	dEQUIP_HELM,//"관",
	dEQUIP_GLOVE,//"장갑",
	dEQUIP_GLOVE,//"장갑대용",
	dEQUIP_GLOVE,//"발톱",
	dEQUIP_GLOVE,//"팔찌",
	dEQUIP_BELT,//"벨트",
	dEQUIP_SHOES,//"부츠",
	dEQUIP_NECKLACE,//"목걸이",
	dEQUIP_RING,//"반지",
	dEQUIP_EARING,//"귀걸이",
	dEQUIP_EARING,//"망토",
	dEQUIP_SHIELD_BULLET,//"브로치",
	dEQUIP_GLOVE,//"팔 문신",
	dEQUIP_SHIELD_BULLET,//"어깨 문신",
	dEQUIP_SHIELD_BULLET,//"십자가",
	dEQUIP_ARMOR,//"공용갑옷",
	dEQUIP_ARMOR,//"전용갑옷",
	dEQUIP_WEAPON,//"한손검",
	dEQUIP_SHIELD_BULLET,//"방패",
	dEQUIP_WEAPON,//"양손검",
	dEQUIP_WEAPON,//"지팡이",
	dEQUIP_WEAPON,//"이빨",
	dEQUIP_WEAPON,//"메이스",
	dEQUIP_WEAPON,//"날개",
	dEQUIP_WEAPON,//"단도",
	dEQUIP_WEAPON,//"활",
	dEQUIP_SHIELD_BULLET,//"화살",
	dEQUIP_WEAPON,//"창",
	dEQUIP_WEAPON,//"피리",
	dEQUIP_WEAPON,//"슬링",
	dEQUIP_SHIELD_BULLET,//"탄환",
	dEQUIP_WEAPON,//"마술봉",
	dEQUIP_WEAPON,//"채찍",
	dITEM_KIND_WASTING,//"보석",
	dITEM_KIND_WASTING,//"체력 포션",
	dITEM_KIND_WASTING,//"차징 포션",
	dITEM_KIND_WASTING,//"스탯 향상 포션",
	dITEM_KIND_WASTING,//"성능 향상 포션",
	dITEM_KIND_WASTING,//"치료약",
	dITEM_KIND_WASTING,//"상태 회복제",
	0xffff,//"열쇠",
	dITEM_KIND_WASTING,//"포탈 스톤",
	0xffff,//"필살기 두루말이",
	dITEM_KIND_WASTING,//"먹을 것/기타",
	0xffff,//"능력향상제",
	0xffff,//"강화액",
	0xffff,//"마법 보석",
	0xffff,//"특수 아이템",

	0xffff,//"퀘스트 아이템",
	0xffff,//"프리미엄 아이템",
	0xffff,//"인챈트 아이템",
	0xffff,//"아이템팩",
	
	0xffff,//"모든 무기",
	dEQUIP_WEAPON,	// 낫
	dEQUIP_WEAPON,	// 암웨폰
	dEQUIP_WEAPON,	// 책
};

//
//	Magic Carpet Shape
enum
{
	eMCS_NORMAL		=	1,
	eMCS_WILD_CAT	,
	eMCS_TURTLE		,
	eMCS_SQUIRREL	,
	eMCS_DRAKE		,
	eMCS_GEAR		,
	eMCS_RED_DRAKE	,
};

enum
{
	eMIT_NORMAL,
	eMIT_RARE,
	eMIT_UNIQUE,
	eMIT_LITTLE_GOOD,
	eMIT_SUPER_UNIQUE,
	eMIT_PRINCESS_WEAPON,
};

enum
{
eIK_HELM						,//"헬멧",
eIK_CROWN						,//"관",
eIK_GLOVE						,//"장갑",
eIK_GLOVE_STYLE					,//"장갑대용",
eIK_CLAW						,//"발톱",
eIK_BRACELET					,//"팔찌",
eIK_BELT						,//"벨트",
eIK_BOOTS						,//"부츠",
eIK_NECKLACE					,//"목걸이",
eIK_RING						,//"반지",
eIK_EARING						,//"귀걸이",
eIK_CLOAK						,//"망토",
eIK_BROOCH						,//"브로치",
eIK_ARM_TATOO					,//"팔문신",
eIK_SHOLDER_TATOO				,//"어깨문신",
eIK_CROSS						,//"십자가",
eIK_ARMOR						,//"공용갑옷",
eIK_EXCLUSIVE_ARMOR				,//"전용갑옷",
eIK_ONEHANDED_SWORD				,//"한손검",
eIK_SHIELD						,//"방패",
eIK_TWOHANDED_SWORD				,//"양손검",
eIK_STAFF						,//"지팡이",
eIK_TOOTH						,//"이빨",
eIK_MACE						,//"메이스",
eIK_WING						,//"날개",
eIK_KNIFE						,//"단도",
eIK_BOW							,//"활",
eIK_ARROW						,//"화살",
eIK_SPEAR						,//"창",
eIK_FLUTE						,//"피리",
eIK_SLING						,//"슬링",
eIK_BULLET						,//"탄환",
eIK_MAGICAL_ROD					,//"마술봉",
eIK_WHIP						,//"채찍",
eIK_JEWEL						,//"보석",
eIK_HEAL_POTION					,//"체력 포션",
eIK_CHARGING_POTION				,//"차징 포션",
eIK_STATE_IMPROVE_POTION		,//"스탯 향상 포션",
eIK_IMPROVE_PERFORMANCE_POTION	,//"성능 향상 포션",
eIK_DRUG						,//"치료약",
eIK_STATUS_DRUG					,//"상태 회복제",
eIK_KEY							,//"열쇠",
eIK_PORTAL_STONE				,//"포탈 스톤",
eIK_SKILL_SCROLL				,//"필살기 두루말이",
eIK_FOOD_AND_ETC				,//"먹을 것/기타",
eIK_IMPROVE_STATE				,//"능력향상제",
eIK_IMPROVE_PERFORMANCE			,//"강화액",
eIK_MAGIC_JEWEL					,//"마법 보석",
eIK_SPECIAL						,//"특수 아이템",

eIK_QUEST_ITEM					,//"퀘스트 아이템",
eIK_PREMIUM_ITEM				,//"프리미엄 아이템",
eIK_ENCHANT_ITEM				,//"인챈트 아이템",
eIK_ITEM_PACK					,//"아이템팩",

eIK_ALL_WEAPON					,//"모든 무기",
eIK_SCYTHE						,	// 낫
eIK_ARM_WEAPON					,		// 암웨폰
eIK_BOOK						,		// 책
eIK_END,							// 마지막..

};


#define	dITEM_KIND_COUNT	eIK_ALL_WEAPON

class	cUniqueData
{
public:
	WORD	m_wEffect;
	short	m_aValue[dITEM_UNIQUE_DATA_VALUE_COUNT];

			cUniqueData()	
			{
				m_wEffect	=	0xffff;
			}

	void	reset()	
			{
				memset(this,0,sizeof(cUniqueData));
				m_wEffect	=	0xffff;
			}

	char*	getComment(BOOL _bSpecial=FALSE);
	BOOL	isSame(cUniqueData* _lpUniqueData)
	{
		if(m_wEffect != _lpUniqueData->m_wEffect)
			return FALSE;
		
		if(memcmp(m_aValue, _lpUniqueData->m_aValue , sizeof(short) * dITEM_UNIQUE_DATA_VALUE_COUNT) != 0)
			return FALSE;

		return TRUE;
	}
};

class	cITEM_GENERATE_DATA
{
public:
	WORD					m_wEffect;
	WORD					m_aValue[dGENERATE_ITEM_DATA_VALUE_COUNT];

	void					reset()
	{
		m_wEffect	=	0xffff;
	}
};

typedef struct 
{
	DWORD isCanNotDropItem				:	1;	//	버릴수 없는 아이템
	DWORD isDurabilityToUseCount		:	1;	//	내구력을 사용 횟수로 변환하는 아이템
	DWORD isUseToItem					:	1;	//	아이템에 사용하는 아이템
	DWORD isUseToCharacter				:	1;	//	캐릭터에 사용하는 아이템
	DWORD isCanNotSelllItem				:	1;	//	팔 수 없는 아이템
	DWORD isCanUseCorpseStatus			:	1;	//	죽은 후에도 사용가능한 아이템
	DWORD isUseToCorpse					:	1;	//	시체에 사용하는 아이템
	DWORD isEquipableToBelt				:	1;	//	벨트에 착용 가능한 아이템
	DWORD isCanNotUseableItem			:	1;	//	사용할 수 없는 아이템
	DWORD premiumLevel					:	3;	//	프리미엄 레벨
	DWORD isCanNotStoreBank				:	1;	//	은행에 보관할 수 없는 아이템
	DWORD isCanNotDestroyItem			:	1;	//	파괴 불가 아이템
	DWORD isCanNotTradeItem				:	1;	//	거래할수 없는 아이템
	DWORD isExclusiveToNormalItem		:	1;	//	보통 아이템에만 사용할 수 있는 아이템
	DWORD isExclusiveToUniqueItem		:	1;	//	유니크 아이템에만 사용할 수 있는 아이템
	DWORD isDestroyRingAtFailedEnchant	:	1;	//	인챈트에 실패한 반지는 뽀갠다.
	DWORD isUpgradePrefixLevel			:	1;	//	접두사의 레벨을 올려주는 인챈트이다.
	DWORD isBadge						:	1;	//	배지 아이템이다.
	DWORD isEffectInInventoryItem		:	1;	//	인벤토리 안에서도 효과가 있는 아이템
	DWORD isCanNotOwnSameItem			:	1;	//	같은 아이템을 가질수 없다.
	DWORD isGuildBattleItem				:	1;	//	길드전 아이템이다.
	DWORD isRequireSaveUseLog			:	1;	//	사용한 로그를 남길 필요가 있다.
	DWORD isDXItem						:	1;
	DWORD isBeginnerItem				:	1;	//	초보자를 위한 아이템
	DWORD isExtraItem					:	1;	//	엑스트라 아이템
	DWORD isMagicCarpetEmblem			:	1;	//	양탄자 엠블럼이다.
	DWORD isMergeToCarpetEmblem			:	1;	//	사용하면 양탄자 엠블럼에 병합된다.
	DWORD isUseAbleBulletSubstituteArrow:	1;	//	사용하면 양탄자 엠블럼에 병합된다.	//	30
	DWORD isUseSpecialPrice				:	1;
	DWORD isUseToMiniPet				:	1;

} tsBasicItemAttribute;

const	BOOL	const_aIsShapeChangeItem[]	=	
{
0,//"헬멧",
0,//"관",
0,//"장갑",
0,//"장갑대용",
0,//"발톱",
0,//"팔찌",
0,//"벨트",
0,//"부츠",
0,//"목걸이",
0,//"반지",
0,//"귀걸이",
0,//"망토",
0,//"브로치",
0,//"팔 문신",
0,//"어깨 문신",
0,//"십자가",
1,//"공용갑옷",
1,//"전용갑옷",
1,//"한손검",
1,//"방패",
1,//"양손검",
1,//"지팡이",
0,//"이빨",
1,//"메이스",
1,//"날개",
1,//"단도",
1,//"활",
0,//"화살",
1,//"창",
1,//"피리",
1,//"슬링",
0,//"탄환",
1,//"마술봉",
1,//"채찍",
0,//"보석",
0,//"체력 포션",
0,//"차징 포션",
0,//"스탯 향상 포션",
0,//"성능 향상 포션",
0,//"치료약",
0,//"상태 회복제",
0,//"열쇠",
0,//"포탈 스톤",
0,//"필살기 두루말이",
0,//"먹을 것/기타",
0,//"능력향상제",
0,//"강화액",
0,//"마법 보석",
0,//"특수 아이템",
0,//"모든 무기",
1,	// 낫.
1,	// 암웨폰
1,	// 책
};

class	CBasicItemData
{
public:
	int						m_iSerial;
	char					m_strName[dITEM_NAME_LENGTH];		//	이름
	char					m_strOwnerGuild[dNAME_LENGTH];	//	소유길드
	DWORD					m_dwCommentAddress;					//	아이템 설명의 주소(텍스트는 따로 모아서 보관한다. 너무 많다. --)
	WORD					m_wKind;							//	종류
	BYTE					m_aEnableJob[18];					//	착용 가능한 직업

	DWORD					m_dwPrice;							//	가격
	WORD					m_wPriceComputeMethod;				//	가격 계산식

	WORD					m_wRange;							//	공격 거리
	WORD					m_wDamageRange;						//	데미지 범위
	WORD					m_wSpeed;							//	공격 속도

	WORD					m_wMinDamage,m_wMaxDamage;			//	공격력
	WORD					m_wDurability;						//	내구력

	WORD					m_wEquipUseLimitValue;
	WORD					m_wEquipUseLimitContents;
	WORD					m_wEquipUseLimitMethod;
	WORD					m_wRequireLevel;					//	사용/장비를 위한 최소 레벨
	WORD					m_wRequireStrength;					//	사용/장비를 위한 최소 힘
	WORD					m_wRequireDexterity;				//	사용/장비를 위한 최소 민첩성
	WORD					m_wRequireConstitution;				//	사용/장비를 위한 최소 체질
	WORD					m_wRequireWisdom;					//	사용/장비를 위한 최소 지혜
	WORD					m_wRequireIntelligence;				//	사용/장비를 위한 최소 지식
	WORD					m_wRequireCharisma;					//	사용/장비를 위한 최소 카리스마
	WORD					m_wRequireLuck;						//	사용/장비를 위한 최소 운
	WORD					m_wRequireAllignment;				//	사용/장비를 위한 성향

	WORD					m_wIconShape;						//	상태창에서의 모양
	WORD					m_wFieldShape;						//	필드에서의 모양
	WORD					m_wEquippedShape;					//	착용 했을때의 모양
	WORD					m_questKind : 2;
	WORD					m_questIndex: 14;
	WORD					m_wStackLimit;						//	특정 퀘스트에 필요한 아이템이다.
	WORD					m_wDropLevel;						//	아이템 드랍 레벨

	WORD					m_aValue[2][2];						//	아이템 생성될때 결정되는 수치
	cITEM_GENERATE_DATA		m_aGenerateData[dGENERATE_ITEM_DATA_COUNT];	//	아이템 생성될때 결정되는 수치
	cUniqueData				m_aUniqueData[dITEM_UNIQUE_DATA_COUNT];		//
	WORD					m_wPriceValue;
	tsBasicItemAttribute	m_attr;
	WORD					m_wExpireYear,m_wExpireMonth,m_wExpireDay,m_wExpireHour;

	WORD					m_wEnchantMinChance,m_wEnchantMaxChance;
	WORD					m_wEnchantLimitPrefixDiscernmentCode;
	WORD					m_wPaletteIndex;
	WORD					m_wBoostDurability;					//	내구력 증폭
	WORD					m_wCorrectDropChance;
	WORD					m_wBaseItem;
	WORD					m_wExtraGrade;
	WORD					m_wPremiumLevel;
	WORD					m_wIsIDPublicItem;
	
	DWORD					m_bf1IsDestroyWhenMoveField		:	1;
	DWORD					m_bf1IsBuyOnlyGuildMaster		:	1;	//	길드 마스터만 구매 가능
	DWORD					m_bf1IsUseOnlyGuildMaster		:	1;
	DWORD					m_bf1IsUseAbleByThrowPotionSkill:	1;
	DWORD					m_bf1IsUseAbleByThrowFlowerSkill:	1;
	DWORD					m_bf1IsUseAbleByThrowDrugSkill	:	1;
	DWORD					m_bf1IsUseAbleByThrowCandySkill	:	1;
	DWORD					m_bf1IsUltimate					:	1;
	DWORD					m_bf1IsBlockToEnchant			:	1;
	DWORD					m_bf1IsBlockToFeedPet			:	1;	//	펫에게 먹이기 금지
	DWORD					m_bf1IsFreeTeleport				:	1;	//	무료 타운 포탈.
	DWORD					m_bf1IsRequestSummonCarpet		:	1;	//	양탄자 소환 의뢰.
	DWORD					m_bf1IsCanSummonMagicCarpet		:	1;	//	양탄자 소환 가능하게 하는 아이템.
	DWORD					m_bf1RemeberPlace0				:	1;	//	0번 슬롯에 장소 기억
	DWORD					m_bf1RemeberPlace1				:	1;	//	1번 슬롯에 장소 기억
	DWORD					m_bf1RemeberPlace2				:	1;	//	2번 슬롯에 장소 기억	16
	DWORD					m_bf1AddEntryGuildDungeon		:	1;	//	17
	DWORD					m_bf1IsAddGetMysticStone		:	1;	//	신비석 추가 획득 18
	DWORD					m_bf1IsAddGetTantalissRelic		:	1;	//	탄탈리스 유물 추가 획득 19
	DWORD					m_bf1IsIgnoreGuildHallLevelForEntryTantalisExile	:	1;	//	길드홀 레벨 제한 없이 탄탈리스 유적 입장 20
	DWORD					m_bf1IsInfinitySeriousUpgradeChanceUp	:	1;	//	21	강화 확률 상승
	DWORD					m_bf1IsRequireLogWithGuildInfo	:	1;	//	22
	DWORD					m_bf1IsFreePastPortal			:	1;	//	23	공짜 패스트 포탈
	DWORD					m_bf1IsHalfPastPortal			:	1;	//	24	반값 패스트 포탈
	DWORD					m_bf1IsExcludeExpBonus			:	1;	//	25	스피어 종류 아이템인데 경험치 보너스가 없다.
	DWORD					m_bf3WearLimit					:	3;	//	28	아이템 착용 갯수 제한. 09.08.11
  	DWORD					m_bf1IsEternalItem				:	1;	//	29	이터널 아이템		09.08.25
  	DWORD					m_bf1EternalItemUpgradeNotPenalty		:	1;	//	30	이터널아이템 강화 패널티 제거~	09.08.25
	DWORD					m_bf1IsUpgradeMaterials			:	1;	//31 강화제 아이템 09.08.26
  	DWORD					m_bf1IsItemNotUsePet			:	1;	//32 펫에게 아이템 사용 불가..	09.09.04
  	
  	BOOL					m_bIsExceptionItem;
  	WORD					m_wPremiumItemLevelForWithdraw;
  	BYTE					m_bCompulsionColorizeEffect;
  	BYTE					m_bCoolTimeIndex;
  	WORD					m_wCoolTimeSecond;
  	
  	WORD					m_wNextEternalItemSerial;							//  이터널 아이템 다음단계 아이템 시리얼 09.08.25
  
  	WORD					m_bf4EternalItemCurrentLevel	:	4;				//4	이터널 아이템 현재 단계 09.08.25
  	WORD					m_bf1IsMaleJobEquipAble			:	1;	//5	남성 직업 착용 가능
  	WORD					m_bf1IsFemaleJobEquipAble		:	1;	//6	여성 직업 착용 가능  
	WORD					m_bf7UpgradeBaseSuccessPercent	:	7;	//13 이터널아이템 강화 확률 % 옵션  09.08.25
	WORD					m_bf1IsOccurEatPotionEffect		:	1;	//14 사용시 포션 먹는 효과 발생  09.09.11
	WORD					m_bf1IsUseShop					:	1;	// 15 상점 이용하기	
	WORD					m_bf1IsUseBank					:	1;	// 16 은행 이용하기	

	UINT					m_uiEquipAbleJobMask;
	WORD					m_wUpgradePenaltyItemSerial;				// 강화패널티 아이템 시리얼 09.08.26

	WORD					m_bf7UpgradeMaterialsSuccessPercent	:	7;		//7	강화제 강화확률 %옵션	09.08.26
	WORD					m_bf7UpgradeCosmicPowerSuccessPercent	:	7;		//14 코스믹파워 강화확률 %옵션		09.08.26
	WORD					m_bf1IsNotConsume						:	1;		//15	 사용시 소모안됨.
	WORD					m_bf1IsUseItemAfterTransformation		:	1;		//16	몬스터 변신후 사용

	WORD					m_bf4ItemRebirthCount					:	4;		//4 아이템 전생회차
	WORD					m_bf5LimitUpgradeRingOfInfinity			:	5;		//9 무한의 반지 업그레이드 제한 수치
	WORD					m_bf1IsRingOfInfinity					:	1;		//10	무한의반지?
	WORD					m_bf1IsUseToEnemy						:	1;		//11	 적에게 사용
	WORD					m_bf1IsUseAfterWear						:	1;		//12	착용후에 기능 사용
	WORD					m_bf1IsUsePremiumInventory				:	1;		//13	유료아이템 전용 인벤토리 사용
	WORD					m_bf1IsNotApplyCopyEffect				:	1;		//14	복사 효과 적용안됨
	WORD					m_bf1IsNotApplyReversionEffect			:	1;		//15	거래불가해제 효과 적용안됨
	WORD					m_bf1IsNotGetUsedSkill					:	1;		//16	스킬로 줍기 불가

	WORD					m_wMoveFieldSerial;						//	이동할 필드 번호..  사용하려면 아이템 옵션에 필드 이동옵션을 추가 시킨다.
	WORD					m_wRequireMaxLevel;					//	사용/장비를 위한 최대 레벨

	WORD					m_wItemEffect;

	WORD					m_bf1IsNotUseToGVG						:	1;	//1	GVG 에서 사용 불가
	WORD					:	0;
	BYTE					m_aDummyData[50];					//	이후로 추가될 데이터에 대비해서

	inline	BOOL			isEquipAbleJob(int _iJob)
	{
		UINT	uiMask	=	1<<_iJob;

		if	(m_uiEquipAbleJobMask & uiMask)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isEnableJob(int _iJob)
	{
		if	(!m_bf1IsMaleJobEquipAble && c_aIsMale[_iJob])
			return	FALSE;
		if	(!m_bf1IsFemaleJobEquipAble && c_aIsMale[_iJob] == FALSE)
			return	FALSE;

		return	isEquipAbleJob(_iJob);
	}

	inline	BOOL			isChargeExtraItem()
	{
		if (m_aGenerateData[0].m_wEffect	==	eIE_CHARGE_EXTRA_ITEM_POWER)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isManExclusiveItem()	//	남성 전용 아이템
	{
		if	(m_bf1IsMaleJobEquipAble	&&	!m_bf1IsFemaleJobEquipAble)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isWomanExclusiveItem()	//	여성 전용 아이템
	{
		if	(!m_bf1IsMaleJobEquipAble	&&	m_bf1IsFemaleJobEquipAble)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL			isLimitEquipItem()
	{
		if	(m_wRequireAllignment+m_wRequireLevel+m_wRequireStrength+m_wRequireDexterity+
			m_wRequireConstitution+m_wRequireWisdom+m_wRequireIntelligence+m_wRequireCharisma+m_wRequireLuck + m_wRequireMaxLevel)
			return	TRUE;

		if	((1-isAllJobItem()) + isManExclusiveItem() + isWomanExclusiveItem())
			return	TRUE;

		return	FALSE;
	}


	inline	BOOL			isAllJobItem()	//	모든 직업이 사용 가능한 아이템
	{
		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
		{
			if	(isEquipAbleJob(i)	==	FALSE)
				return	FALSE;
		}

		//	남성전용 혹은 여성 전용 체크 할까?

		return	TRUE;
	}

	inline BOOL			isGoldBar()		// 09.08.19
	{
		if	(m_iSerial == dITEM_INDEX_GOLD_BAR)			// 인덱스 4237은 금괴이다.
			return TRUE;
		
		return FALSE;
	}

	inline	int				getDurability()
	{
		return	m_wDurability;
	}

	inline	BOOL			isEquipmentItem()		//추가 .
	{	// 장비 아이템인가?
		if	(c_aItemEquipPlace[m_wKind]	>=	dEQUIP_WEAPON && c_aItemEquipPlace[m_wKind]	<=	dEQUIP_RING )
			return	TRUE;
		
		return FALSE;
	}
	inline	BOOL			isUnique()			// 수정
	{
		if	(isEquipmentItem() == FALSE)	//  소모품은 무시
			return	FALSE;

		if	(m_aUniqueData[0].m_wEffect != 0xffff)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isAttachAblePrefix()
	{	// 접두사를  붙일 수 있는 아이템인가?
		if(isEquipmentItem() || m_wKind == eIK_JEWEL )
			return TRUE;

		return FALSE;

	}

	inline	BOOL			isAppearanceChangeItem()	//	착용 하면 캐릭터 외형이 변하는 아이템인가?
	{
		return	const_aIsShapeChangeItem[m_wKind];
	}
	inline int			getNextEternalItemSerial()			// 다음단계아이템 시리얼		09.08.25
	{
		return m_wNextEternalItemSerial;
	}
	inline int			getUpgradePenaltyItemSerial()			// 강화패널티아이템 시리얼		09.08.26
		{
		return m_wUpgradePenaltyItemSerial;
	}
	
	inline int			getCurrentEternalItemLevel()		// 현재 단계					09.08.25
		{
		return m_bf4EternalItemCurrentLevel;
	}
	inline BOOL			isEternalItem()					// 이터널아이템??	09.08.25
		{
		return m_bf1IsEternalItem;
	}
	inline BOOL			isCosmicPower()					// 코스믹파워옵션???	09.08.25
		{
		return m_bf1IsInfinitySeriousUpgradeChanceUp;
	}
	
	inline BOOL			isUpgradeEternalItemNotPenalty()	// 업그레이드 패널티 제거옵션??	09.08.25
		{
		return m_bf1EternalItemUpgradeNotPenalty;
	}
	inline BOOL			isUpgradeMaterialsItem()			// 강화제 아이템 ? ? ?  09.08.26
		{
		return m_bf1IsUpgradeMaterials;
	}
	inline int			getUpgradeMaterialsItemSuccessChance()	// 강화제 강화 확률	09.08.26
		{
		return m_bf7UpgradeMaterialsSuccessPercent;
	}
	inline int			getUpgradeCosmicPowerSuccessChance()	// 코스믹파워 강화 성공 확률	09.08.26
		{
		return m_bf7UpgradeCosmicPowerSuccessPercent;
	}
	inline int			getUpgradeBaseSuccessChance()			// 기본 강화 성공 확률	09.08.26
		{
		return m_bf7UpgradeBaseSuccessPercent;
	}
	inline int	getCarveNeedMoney(int _iLevel)
	{
		return	c_iaCarvingData[_iLevel][0];
	}
	inline int	getCarveNeedCristalCount(int _iLevel)
	{
		return	c_iaCarvingData[_iLevel][1];
	}
};

#pragma pack()

#pragma pack(1)

class	CItemPrefixInfo
{
public:
	WORD		m_wPrefix;
	BYTE		m_aValue[2];
	BOOL		isSame(CItemPrefixInfo* lpPrefixInfo)
	{
		if(m_wPrefix != lpPrefixInfo->m_wPrefix)
			return FALSE;

		if(memcmp(m_aValue,lpPrefixInfo->m_aValue,sizeof(BYTE)*2) != 0)
			return FALSE;

		return TRUE;
	}
};

class	CItemBaseInfo
{
public: 
	DWORD				m_dwSerial;

	WORD				m_wBaseItem;							//	아이템 인덱스
	BYTE				m_bCount;								//	숫자
	BYTE				m_bDurability;							//	내구력
	BYTE				m_aOption[2];							//	생성될때 붙은 옵션

	CItemPrefixInfo		m_aPrefix[dITEM_PREFIX_COUNT];	//	추가효과 3개
	BOOL				isSame(CItemBaseInfo* _lpItemBaseInfo)
	{
		if(m_dwSerial != _lpItemBaseInfo->m_dwSerial)
			return FALSE;


	if(m_wBaseItem != _lpItemBaseInfo->m_wBaseItem)
		return FALSE;

	if(m_bCount != _lpItemBaseInfo->m_bCount)
		return FALSE;						

	if(m_bDurability != _lpItemBaseInfo->m_bDurability)
		return FALSE;

	if(memcmp(m_aOption, m_aOption, sizeof(BYTE)*2) != 0)
		return FALSE;					
	
	
	for(int i=0;i<dITEM_PREFIX_COUNT;++i)
	{
		if(!m_aPrefix[i].isSame(&_lpItemBaseInfo->m_aPrefix[i]))
			return FALSE;
	}
	
		return TRUE;
	}
};

class	CCustomItemDefine	: public CItemBaseInfo
{
public:
	char			m_strName[30];
	DWORD			m_bf20TermOfValidate	:	20;	//	분단위
	DWORD			m_bf1IsSellInCarrotShop	:	1;	//	분단위
	DWORD			m_bf1IsCustomItem		:	1;	//	커스텀 아이템(퀄리티를 무시하고 CItemBaseInfo 정보대로 세팅)
	DWORD			m_bf2ItemInArcaType		:	2;
	
	WORD			m_wIsAttachRandomPrefix,m_wDropLevel;
	WORD			m_wGenerateQuality,m_aPrefixGenerateQuality[dITEM_PREFIX_COUNT];
};

class	cCoupleRingDefine
{
public:
	DWORD				m_dwSerial;
	WORD				m_wBaseItem;						//	아이템 인덱스

	WORD				m_bf1IsCanNotTradeItem	:	1;		//	1
	WORD				m_year			: 5;	//6	유효기간
	WORD				m_month			: 4;	//10
	WORD				m_day			: 5;	//15
	WORD				:0;

	BYTE				m_aOption[2];							//	생성될때 붙은 옵션

	char				m_strCoupleName[dNAME_LENGTH-2];

	void				setCoupleName(char *_lpstrName)
	{
		memcpy(m_strCoupleName,_lpstrName,dNAME_LENGTH-2);
	}
	char*				getCoupleName()
	{
		static	char	strCoupleName[dNAME_LENGTH];

		memcpy(strCoupleName,m_strCoupleName,dNAME_LENGTH-2);

		strCoupleName[dNAME_LENGTH-2]	=	NULL;

		return	strCoupleName;
	}
};

enum
{
	eIRT_NONE,
	eIRT_REVERSION_WHEN_GET,
	eIRT_REVERSION_WHEN_EQUIP,
};

class	cBookItemDefine
{
public:
	DWORD				m_dwSerial;
	WORD				m_wBaseItem;			//	아이템 인덱스

	WORD				m_wBookIndex;
	BYTE				m_aCategoryInfo[c_iMaxBookCategoryCount];
};

class	cBlockLightPocket
{
public:
	int				m_iVitalPoint;
	WORD			m_wBaseItem;			//	아이템 인덱스

	UINT			m_bf3SkillLevel1:	3;
	UINT			m_bf3SkillLevel2:	3;
	UINT			m_bf3SkillLevel3:	3;
	UINT			m_bf3SkillLevel4:	3;
	UINT			m_bf3SkillLevel5:	3;
	UINT			m_bf3SkillLevel6:	3;
	UINT			m_bf3SkillLevel7:	3;
	UINT			m_bf3SkillLevel8:	3;
	UINT			m_bf3SkillLevel9:	3;
	UINT			m_bf3SkillLevel10:	3;	//	30
	UINT			m_bf1isUseAwakenItem50	:		1;	//	31
	UINT			m_bf1isUseAwakenItem100	:		1;	//	32

	UINT			m_bf7Level		:	7;	//	레벨
	UINT			m_bf5Type		:	5;	//	타입
	UINT			m_bf16Exp		:	16;	//	경험치
	UINT			m_bf3SkillLevel11:	3;	//	31

	int				m_iEnergyPoint,m_iMineralPoint;

	UINT			m_bf7SkillPoint	:	7;
	UINT			m_bf4ExpireMonth:	4;
	UINT			m_bf5ExpireYear	:	5;
	UINT			m_bf5ExpireDay	:	5;
	UINT			m_bf5ExpireHour	:	5;
	UINT			m_bf6ExpireMinute:	6;	//	32
};

class	cPrincessWeaponDefine	
{
public:
	DWORD				m_dwSerial;
	WORD				m_wBaseItem;			//	아이템 인덱스
	WORD				m_wBasicItem;			//	아이템 인덱스

	BYTE				m_aOption[2];			//	생성될때 붙은 옵션

	BYTE				m_bFireDamage;
	BYTE				m_bWaterDamage;
	BYTE				m_bWindDamage;
	BYTE				m_bEarthDamage;
	BYTE				m_bLightDamage;
	BYTE				m_bDarkDamage;

	WORD				m_wAddDamagePercentage;

	DWORD				m_dwPrincessNameHashCode;
	WORD				m_wPrincessSerial;
};

class	cNameStone
{
public:
	DWORD				m_dwSerial;
	WORD				m_wBaseItem;			//	아이템 인덱스

	WORD				m_wTargetWorld;
	char				m_strName[dNAME_LENGTH];
};

class	cBookCategoryItemDefine
{
public:
	DWORD				m_dwSerial;
	WORD				m_wBaseItem;			//	아이템 인덱스

	WORD				m_wBookIndex;
	WORD				m_wBeginCategory,m_wEndCategory;
};

class	cBookPageItemDefine
{
public:
	DWORD				m_dwSerial;
	WORD				m_wBaseItem;			//	아이템 인덱스

	WORD				m_wBookIndex;
	WORD				m_wCategory;
	WORD				m_wBeginPage,m_wEndPage;
};

class	cSetItemDefine	: public CItemBaseInfo
{
public:
	WORD			m_bf1IsCustomItem				:	1;	//	커스텀 아이템(퀄리티를 무시하고 CItemBaseInfo 정보대로 세팅)
	WORD			m_bf1IsDropInTempDungeonMonster	:	1;
	WORD			m_bf1IsDropInTempDungeonArca	:	1;
	WORD			m_bf1IsDropInNormalFieldMonster	:	1;
	WORD			m_bf1IsDropInNormalFieldArca	:	1;
	WORD			m_bf1IsDropInQuest				:	1;
	WORD			m_bf4ReversionType				:	4;
	WORD			m_bf6Dummy						:	6;

	WORD			m_wDummyData;	//	워래 직업 설정 있던 자리다

	WORD			m_wDropChance,m_wDropLevel;
	WORD			m_wGenerateQuality,m_aPrefixGenerateQuality[dITEM_PREFIX_COUNT];

	char			m_strName[30];
	DWORD			m_dwCommentAddress;
	WORD			m_wCommentSize;					//	아이템 설명의 사이즈(텍스트는 따로 모아서 보관한다. 너무 많다. --)

	UINT			m_uiEnableJobMask;	

	char			*m_pItemComment;

	cSetItemDefine()
	{
		m_pItemComment	=	NULL;
	}
	~cSetItemDefine()
	{
		pKILL(m_pItemComment);
	}

	void			reset()
	{
		memset(this,0,sizeof(cSetItemDefine)-4);
		memset(m_aPrefix,0xff,sizeof(m_aPrefix));

		m_wBaseItem			=	0xffff;
		m_wGenerateQuality	=	100;
		m_wDropLevel		=	100;
		m_wDropChance		=	100;
		strcpy(m_strName,"valid");
	}

	void			copy(cSetItemDefine *_lpData)
	{
		memcpy(this,_lpData,sizeof(cSetItemDefine)-4);
		setComment(_lpData->m_pItemComment);
	}

	void					setComment(char *_strComment)
	{
		pKILL(m_pItemComment);

		if (!_strComment)
		{
			m_wCommentSize	=	strlen("valid")+1;

			m_pItemComment	=	new char [m_wCommentSize];

			strcpy(m_pItemComment,"valid");

			return;
		}

		m_wCommentSize	=	strlen(_strComment)+1;
		m_pItemComment	=	new char [m_wCommentSize];

		strcpy(m_pItemComment,_strComment);
	}

	inline	BOOL	isEnableJob(int _iJob)
	{
		if	(m_uiEnableJobMask	==	0)
			return	TRUE;

		UINT	uiMask	=	1<<_iJob;

		if	(m_uiEnableJobMask & uiMask)
			return	TRUE;

		return	FALSE;
	}

	BOOL			isValidEnableJobMaskInfo()
	{
		if	(m_uiEnableJobMask)
			return	TRUE;

		return	FALSE;
	}
	BOOL			isSame(cSetItemDefine* _lpSetItem)
	{

		if(!((CItemBaseInfo*) this)->isSame(((CItemBaseInfo*)_lpSetItem)))
			return FALSE;

		if(m_bf1IsCustomItem != _lpSetItem->m_bf1IsCustomItem)
			return FALSE;

		if(m_bf1IsDropInTempDungeonMonster != _lpSetItem->m_bf1IsDropInTempDungeonMonster)
			return FALSE;

		if(m_bf1IsDropInTempDungeonArca != _lpSetItem->m_bf1IsDropInTempDungeonArca)
			return FALSE;

		if(m_bf1IsDropInNormalFieldMonster != _lpSetItem->m_bf1IsDropInNormalFieldMonster)
			return FALSE;

		if(m_bf1IsDropInNormalFieldArca != _lpSetItem->m_bf1IsDropInNormalFieldArca)
			return FALSE;

		if(m_bf1IsDropInQuest != _lpSetItem->m_bf1IsDropInQuest)
			return FALSE;

		if(m_bf4ReversionType != _lpSetItem->m_bf4ReversionType)
			return FALSE;

		if(m_bf6Dummy != _lpSetItem->m_bf6Dummy)
			return FALSE;
			
		if(m_wDummyData != _lpSetItem->m_wDummyData)
			return FALSE;
			
		if(m_wDropChance != _lpSetItem->m_wDropChance)
			return FALSE;

		if(m_wDropLevel != _lpSetItem->m_wDropLevel)
			return FALSE;

		if(m_wGenerateQuality != _lpSetItem->m_wGenerateQuality)
			return FALSE;

		if(memcmp(m_aPrefixGenerateQuality,_lpSetItem->m_aPrefixGenerateQuality,sizeof(WORD) *dITEM_PREFIX_COUNT) != 0)
			return FALSE;
			
		if(strcmp(m_strName, _lpSetItem->m_strName) != 0)
			return FALSE;

		if(m_dwCommentAddress != _lpSetItem->m_dwCommentAddress)
			return FALSE;
		if(m_wCommentSize != _lpSetItem->m_wCommentSize)
			return FALSE;
			
		if(m_uiEnableJobMask != _lpSetItem->m_uiEnableJobMask)
			return FALSE;

		return	TRUE;
	}

};

#define	dMAX_ITEM_COUNT_IN_SET				10
#define	dMAX_SET_ITEM_COUNT					4096
#define	dMAX_SET_ITEM_GROUP_COUNT			1024
#define	dMAX_SET_ITEM_EFFECT_GRADE_COUNT	10
#define	dMAX_SET_ITEM_EFFECT_COUNT			6

class	cItemSetDefine
{
public:
	WORD			m_wSerial;
	WORD			m_wEqiupLevel;

	char			m_strName[30];

	cUniqueData		m_aSetEffect[dMAX_SET_ITEM_EFFECT_GRADE_COUNT][dMAX_SET_ITEM_EFFECT_COUNT];		//
	DWORD			m_dwCommentAddress;

	cSetItemDefine	m_aItem[dMAX_ITEM_COUNT_IN_SET];
	BOOL			isSame(cItemSetDefine* _lpItemSet)
	{
		if(m_wSerial != _lpItemSet->m_wSerial)
			return FALSE;
		if(m_wEqiupLevel != _lpItemSet->m_wEqiupLevel)
			return FALSE;
		
		if(strcmp(m_strName,_lpItemSet->m_strName) != 0)
			return FALSE;
		
		cUniqueData		m_aSetEffect[dMAX_SET_ITEM_EFFECT_GRADE_COUNT][dMAX_SET_ITEM_EFFECT_COUNT];		//
		for(int i=0;i<dMAX_SET_ITEM_EFFECT_GRADE_COUNT;++i)
		{
			for(int j=0;j<dMAX_SET_ITEM_EFFECT_COUNT;++j)
			{
				if(!m_aSetEffect[i][j].isSame(&_lpItemSet->m_aSetEffect[i][j]))
					return FALSE;
			}
			
		}
		if(m_dwCommentAddress != m_dwCommentAddress)
			return FALSE;
		
		for(int k=0;k<dMAX_ITEM_COUNT_IN_SET;++k)
		{
			if(!m_aItem[k].isSame(&_lpItemSet->m_aItem[k]))
				return FALSE;
		}
		
		return TRUE;
	}
};

#define	dDROPPING_ITEM_COUNT				10		//	떨어드리는 아이템 패턴

class	cDroppingItemForItemPiece
{
public:
	WORD		m_wPieceItem	;

	WORD		m_bf4ItemPiece		:	4;
	WORD		m_bf8Count			:	10;	//	갯수
	WORD		m_bf1IsCustomItem	:	1;
	WORD		m_bf1IsSetItem		:	1;

	WORD		m_bf1IsItemPiece	:	1;
	WORD		m_bf1IsUnknown		:	1;

	DWORD		m_dwDroppingRate;	//	떨어뜨릴 확률
};

class	cDroppingItemForSetItem
{
public:
	WORD		wItemSet;

	WORD		m_bf4SetItemIndex	:	4;
	WORD		m_bf8Count			:	10;	//	갯수
	WORD		m_bf1IsCustomItem	:	1;
	WORD		m_bf1IsSetItem		:	1;

	WORD		m_bf1IsItemPiece	:	1;
	WORD		m_bf1IsUnknown		:	1;

	DWORD		m_dwDroppingRate;	//	떨어뜨릴 확률
};

class	cDroppingItem
{
public:
	WORD		m_wItemType	;			//	아이템

	WORD		m_bf8Count			:	10;	//	갯수
	WORD		m_bf1IsCustomItem	:	1;
	WORD		m_bf1IsSetItem		:	1;
	WORD		m_bf1IsItemPiece	:	1;
	WORD		m_bf1IsUnknown		:	1;

	DWORD		m_dwDroppingRate;	//	떨어뜨릴 확률

				cDroppingItem()	{reset();}

	inline void	reset()
	{
		memset(this,0,sizeof(cDroppingItem));

		m_wItemType		=	0xffff;
		m_bf8Count		=	1;
		m_bf1IsCustomItem=	FALSE;
		m_dwDroppingRate=	100;
	}
};

class	CItemDefineForValue : public CItemBaseInfo
{
public: 
    DWORD	m_dwValue;
};

#define	dMAX_PIECE_ITEM_PIECE_COUNT		25
#define	dMAX_PIECE_ITEM_COUNT			2048

class	cItemPieceDefine
{
public:
	WORD	m_wDropChance;
	WORD	m_wDropLevel;

	WORD	m_bf1IsDropInTempDungeonMonster	:	1;
	WORD	m_bf1IsDropInTempDungeonArca	:	1;
	WORD	m_bf1IsDropInNormalFieldMonster	:	1;
	WORD	m_bf1IsDropInNormalFieldArca	:	1;
	WORD	m_bf1IsDropInQuest				:	1;
	WORD	m_bf1IsReversion				:	1;

			cItemPieceDefine()
			{
				reset();
			}

	void	reset()
	{
		memset(this,0,sizeof(cItemPieceDefine));
		m_wDropLevel	=	100;
		m_wDropChance	=	100;
		m_bf1IsReversion=	TRUE;
	}
	BOOL	isSame(cItemPieceDefine* _lpPieceItem)
	{
		if(m_wDropChance != _lpPieceItem->m_wDropChance)
			return FALSE;
		
		if(m_wDropLevel != _lpPieceItem->m_wDropLevel)
			return FALSE;
		
		if(m_bf1IsDropInTempDungeonMonster != _lpPieceItem->m_bf1IsDropInTempDungeonMonster)
			return FALSE;
		
		if(m_bf1IsDropInTempDungeonArca != _lpPieceItem->m_bf1IsDropInTempDungeonArca)
			return FALSE;
		
		if(m_bf1IsDropInNormalFieldMonster != _lpPieceItem->m_bf1IsDropInNormalFieldMonster)
			return FALSE;
		
		if(m_bf1IsDropInNormalFieldArca != _lpPieceItem->m_bf1IsDropInNormalFieldArca)
			return FALSE;
		
		if(m_bf1IsDropInQuest != _lpPieceItem->m_bf1IsDropInQuest)
			return FALSE;
		
		if(m_bf1IsReversion != _lpPieceItem->m_bf1IsReversion)
			return FALSE;

		return TRUE;
	}
};

class	cPieceItemDefine
{
public:
	char					m_strName[30];
	WORD					m_wSerial;
	WORD					m_wEquipLevel;
	WORD					m_wPieceCount;

	CItemBaseInfo			m_item;
	WORD					m_bf1IsSetItem		:	1;
	WORD					m_bf11SetGroupIndex	:	11;
	WORD					m_bf4SetItemIndex	:	4;

	cItemPieceDefine		m_aPiece[dMAX_PIECE_ITEM_PIECE_COUNT];
	BOOL	isSame(cPieceItemDefine* _lpPieceItem)
	{	
		
		if(strcmp(m_strName, _lpPieceItem->m_strName) != 0)
			return FALSE;
		
		if(m_wSerial != _lpPieceItem->m_wSerial)
			return FALSE;
		
		if(m_wEquipLevel != _lpPieceItem->m_wEquipLevel)
			return FALSE;
		
		if(m_wPieceCount != _lpPieceItem->m_wPieceCount)
			return FALSE;
		
		if(!m_item.isSame(&_lpPieceItem->m_item))
			return FALSE;
		
		if(m_bf1IsSetItem != _lpPieceItem->m_bf1IsSetItem)
			return FALSE;
		
		if(m_bf11SetGroupIndex != _lpPieceItem->m_bf11SetGroupIndex)
			return FALSE;
		
		if(m_bf4SetItemIndex != _lpPieceItem->m_bf4SetItemIndex)
			return FALSE;
		
		for(int i=0;i<dMAX_PIECE_ITEM_PIECE_COUNT;++i)
		{
			if(!m_aPiece[i].isSame(&_lpPieceItem->m_aPiece[i]))
				return FALSE;
		}
		
		return TRUE;
	}
};

class	cExtraItemDefine
{
public: 
	DWORD				m_dwSerial;
	WORD				m_wBaseItem;			//	아이템 인덱스
	short				m_sCurrentEfficiency;	//	현재 효율
	BYTE				m_aOption[2];			//	생성될때 붙은 옵션

	CItemPrefixInfo		m_aPrefix[dITEM_PREFIX_COUNT];	//	추가효과 3개

	DWORD				m_isBroken		: 1;	//	부숴진 아이템
	DWORD				m_isCursed		: 1;	//	저주 받은 아이템
	DWORD				m_wayPointIndex	: 4;	//	웨이 포인트 인덱스
	DWORD				m_isCanNotAttachPrefixItem	: 1;	//	더 이상 접두사를 붙일 수 없는 아이템

	DWORD				m_year			: 4;	//	유효기간
	DWORD				m_month			: 5;
	DWORD				m_day			: 5;
	DWORD				m_hour			: 5;
	DWORD				m_minute		: 6;
	
	BOOL				updateCurrentEfficency();
	int					getRemainTimeEfficency();
};

class	cMagicCarpetEmblemDefine
{
public: 
	DWORD				m_dwSerial;
	WORD				m_wBaseItem;			//	아이템 인덱스

	WORD				m_bf1IsActiveGearCarpet		:	1;
	WORD				m_bf1IsTurtleCarpet			:	1;
	WORD				m_bf1IsSquirrelCarpet		:	1;
	WORD				m_bf1IsDrakeCarpet			:	1;
	WORD				m_bf1IsGearCarpet			:	1;
	WORD				m_bf1IsWildCat				:	1;
	WORD				m_bf1IsRedDrakeCarpet		:	1;

	BYTE				m_aOption[2];			//	생성될때 붙은 옵션

	CItemPrefixInfo		m_aPrefix[dITEM_PREFIX_COUNT];	//	추가효과 3개

	DWORD				m_isBroken					: 1;	//	부숴진 아이템
	DWORD				m_isCursed					: 1;	//	저주 받은 아이템
	DWORD				m_wayPointIndex				: 4;	//	웨이 포인트 인덱스
	DWORD				m_isCanNotAttachPrefixItem	: 1;	//	더 이상 접두사를 붙일 수 없는 아이템

	DWORD				m_year						: 4;	//	유효기간
	DWORD				m_month						: 5;
	DWORD				m_day						: 5;
	DWORD				m_hour						: 5;
	DWORD				m_minute					: 6;
};

class	cSpecialItemDefine	: public CItemBaseInfo
{
public:
	DWORD				m_isBroken						:	1;	//	부숴진 아이템
	DWORD				m_bf1IsSpecialItem				:	1;	//	

	DWORD				m_bf1IsReversionItem			:	1;	//	귀속된 아이템
	DWORD				m_bf1IsReversionItemWhenEquip	:	1;	//	착용하면 귀속 되는 아이템
	DWORD				m_bf1IsSetItem					:	1;	//	세트 아이템이다.
	DWORD				m_bf1IsUnknown					:	1;	//	무슨 아이템인지 모르겠다.

	DWORD				m_bf11SValue					:	11;	//	세트 아이템이나 조각 아이템 인덱스
	DWORD				m_bf10EquipLevel				:	10;	//	착용 제한 레벨
	DWORD				m_bf1IsFreeUseItem				:	1;	//	사용이나 착용에 제한이 없는 아이템 28

};

class	CItemDefine : public CItemBaseInfo	//	26바이트
{
public: 
	DWORD				m_isBroken			: 1;	//	부숴진 아이템
	DWORD				m_bf1IsSpecialItem	: 1;	//	
	DWORD				m_wayPointIndex		: 4;	//	웨이 포인트 인덱스

	DWORD				m_isCanNotAttachPrefixItem	: 1;	//	더 이상 접두사를 붙일 수 없는 아이템
	DWORD				m_year				: 4;	//	유효기간
	DWORD				m_month				: 5;
	DWORD				m_day				: 5;
	DWORD				m_hour				: 5;
	DWORD				m_minute			: 6;

	inline	void		reset()
	{
		memset(this,0,sizeof(CItemDefine));

		m_wayPointIndex	=	dVALID_PLAYER_WAYPOINT;
		m_wBaseItem		=	0xffff;

		memset(m_aPrefix,0xff,sizeof(m_aPrefix));
	}

	DWORD				getBitFieldValue()
	{
		return	((CItemDefineForValue *)this)->m_dwValue;
	}

	void				setBitFieldValue(DWORD _dwValue)
	{
		((CItemDefineForValue *)this)->m_dwValue = _dwValue;
	}
	inline	BOOL	isExpirationData()	// 유효기간 유무 체크..
	{	
		if(m_year != 0 || m_month != 0 || m_day != 0 || m_hour != 0 || m_minute != 0)
			return TRUE;
		return FALSE;

	}

	inline	BOOL	isMoneyItem()	//	골드거나.. 금괴
	{
		if	(m_wBaseItem == dITEM_INDEX_GOLD_BAR || m_wBaseItem == dITEM_MONEY)
			return	TRUE;

		return	FALSE;
		
	}
};

class	cItemDataForRevise	:	CItemDefine	
{
public: 
	WORD	m_wSlot;

	void	reset()
	{
		m_wSlot	=	0xffff;
	}
};

class	CPlayerEquipmentDataDefine
{
public:
	CItemDefine			m_aEquip[dEQUIPMENT_PART_COUNT];
	CItemDefine			m_aItems[dOWN_ITEM_COUNT];
};

#pragma pack()

//
//	Prefix Output Effect
enum
{
ePOE_PHYSICAL_DAMAGE_1TH,
ePOE_PHYSICAL_DAMAGE_2TH,
ePOE_PHYSICAL_DAMAGE_3TH,
ePOE_FIRE_DAMAGE_1TH,
ePOE_FIRE_DAMAGE_2TH,
ePOE_FIRE_DAMAGE_3TH,
ePOE_WATER_DAMAGE_1TH,
ePOE_WATER_DAMAGE_2TH,
ePOE_WATER_DAMAGE_3TH,
ePOE_WIND_DAMAGE_1TH,
ePOE_WIND_DAMAGE_2TH,
ePOE_WIND_DAMAGE_3TH,
ePOE_EARTH_DAMAGE_1TH,
ePOE_EARTH_DAMAGE_2TH,
ePOE_EARTH_DAMAGE_3TH,
ePOE_LIGHT_DAMAGE_1TH,
ePOE_LIGHT_DAMAGE_2TH,
ePOE_LIGHT_DAMAGE_3TH,
ePOE_DARK_DAMAGE_1TH,
ePOE_DARK_DAMAGE_2TH,
ePOE_DARK_DAMAGE_3TH,
ePOE_DEFENSIVE_POWER_1TH,
ePOE_DEFENSIVE_POWER_2TH,
ePOE_DEFENSIVE_POWER_3TH,
ePOE_BASIC_POWER_1TH,
ePOE_BASIC_POWER_2TH,
ePOE_BASIC_POWER_3TH,
ePOE_INCREASE_HP_1TH,
ePOE_INCREASE_HP_2TH,
ePOE_INCREASE_HP_3TH,
ePOE_INCREASE_CP_1TH,
ePOE_INCREASE_CP_2TH,
ePOE_INCREASE_CP_3TH,
ePOE_ELEMENTS_RESISTANCE_1TH,
ePOE_ELEMENTS_RESISTANCE_2TH,
ePOE_ELEMENTS_RESISTANCE_3TH,
ePOE_STRANGE_STATUS_RESISTANCE_1TH,
ePOE_STRANGE_STATUS_RESISTANCE_2TH,
ePOE_STRANGE_STATUS_RESISTANCE_3TH,
ePOE_THORN_1TH,
ePOE_THORN_2TH,
ePOE_THORN_3TH,

ePOE_PRINCESS_WEAPON	=	200,
};


#define	dITEM_PREFIX_STICK_ABLE_ITEM_COUNT	44

class cITEM_PREFIX
{
public:
	class cTYPE
	{
	public:
		int			m_wType;
		char		m_strName[128];
	};

	WORD			m_wSerial;
	WORD			m_wDiscernmentCode;
	WORD			m_wType;
	short			m_aValue[2][2];//수치 1,2의 최대 최소치
	short			m_sValue3;//3번째 수치
	char			m_str1stPrefix[20],m_str2stPrefix[20];
	WORD			m_wDropLevel;
	WORD			m_wPrefixForm;//접두사 타입 0은 명사,1은 형용사

	DWORD			m_dwPrice;//가격
	WORD			m_wComputePriceMethod;	//	가격 계산 방법
	WORD			m_wPriceFactor;	//	곱할 것인가 더할 것인가

	BYTE			m_aEnableSet[4];	//	처음,두번째,세번째,덤-_-
	BYTE			m_aStickableItem[dITEM_PREFIX_STICK_ABLE_ITEM_COUNT];
	BYTE			m_bWeaponEffect,m_bArmorEffect;
	WORD			m_wIsDXPrefix;
	WORD			m_wCorrectDropChance;
	short			m_saCorrectQualityPrefix[2];
	WORD			m_wIsUltimatePrefix;
	WORD			m_bf1IsStickableItemScythe :1;		// 1 낫..
	WORD			m_bf1IsStickAbleItemArmWeapon :1;		//2 암웨폰..
	WORD			m_bf1IsStickAbleItemBook		:1;		//3 책 
	WORD			:0;
	BYTE			m_aTempValue[30];	// 32 -> 30

					cITEM_PREFIX()	{m_wSerial	=	0xffff;}

	void			reset();
	void			copy(cITEM_PREFIX *_lpPrefix);	//	복사
	void			getMaxValue(short *_lpValues);
	char*			getPrefix(int _iPos);
	DWORD			getPriceValue(int _iValue1,int _iValue2);	//	가격 인자 얻기

	inline	BOOL	isStickAbleItem(int _iKind)
	{
		if (_iKind >= dITEM_PREFIX_STICK_ABLE_ITEM_COUNT)
		{
			switch(_iKind)
			{
			case eIK_SCYTHE :
				return m_bf1IsStickableItemScythe;
			case eIK_ARM_WEAPON :
				return m_bf1IsStickAbleItemArmWeapon;
			case eIK_BOOK :
				return m_bf1IsStickAbleItemBook;
			default:
				return FALSE;
			}
		}
		return	m_aStickableItem[_iKind];
	}
	BOOL			isSame(cITEM_PREFIX* _lpItemPrefix);
};
#define	dMAX_ITEM_IN_ITEM_PACK	100

class	cItemInPack : public CItemDefine
{
public:
	WORD			m_wLimitCountOfServer	:	14;
	WORD			m_bf1IsPrizeWinningItem	:	1;
	WORD			m_bf1IsBlockToTrade		:	1;

	WORD			m_wCurrentCount;
	DWORD			m_dwFrequency;

	DWORD			getCheckSum()
	{
		DWORD	dwCheckSum	=	0;
		BYTE	*lpData		=	(BYTE *)this;

		for (int i=0;i<sizeof(cItemInPack);i++)
			dwCheckSum	+=	lpData[i];

		return	dwCheckSum;
	}

	void			reset()
	{
		CItemDefine::reset();

		m_bf1IsPrizeWinningItem	=	FALSE;
		m_bf1IsBlockToTrade		=	FALSE;
		m_wCurrentCount			=	0;
		m_wLimitCountOfServer	=	0;
		m_dwFrequency			=	10000;
		m_dwSerial				=	0;
	}
};

class	cItemPackInfo
{
public:
	WORD		m_wSerial;
	WORD		m_wCount;
	WORD		m_wLinkItem;
	char		m_strName[28];
	UINT		m_bf1IsSelectOneItem	:	1;	//	이걸 사용하면 유저는 이 아이템중 하나를 선택할 수 있다.
	UINT		m_bf1IsSelectOnePrefix	:	1;	//	여기서 아이템을 사용하면 유저는 아이템의 접두사들 중 하나만 선택할 수 있다.

	cItemInPack	m_aItems[dMAX_ITEM_IN_ITEM_PACK];

	DWORD		getCheckSum()
	{
		DWORD	dwCheckSum	=	m_wCount+m_wSerial+m_wLinkItem+GetHashCode(m_strName);

		for (int i=0;i<dMAX_ITEM_IN_ITEM_PACK;i++)
		{
			if (m_aItems[i].m_wBaseItem	==	0xffff)
				break;

			dwCheckSum	+=	m_aItems[i].getCheckSum();
		}

		return	dwCheckSum;
	}

	void		reset()
	{
		memset(this,0,sizeof(cItemPackInfo));

		m_wCount	=	0;
		m_wSerial	=	0xffff;
		m_wLinkItem	=	0xffff;

		for (int i=0;i<dMAX_ITEM_IN_ITEM_PACK;i++)
			m_aItems[i].reset();
	}
};

#define	dMAX_ITEM_PACK_COUNT				256
#define	dMAX_CARROT_SHOP_CATEGORY_COUNT		20
#define	dMAX_CARROT_SHOP_ITEM_COUNT			99

#define	dITEM_VERSION_CHECKER				0x12345678

enum
{
	eITEMDATAVERSION_ADD_ITEM_COOL_TIME	=	0,
	eITEMDATAVERSION_RENEWAL_EQUIPABLE_JOB_INFO	,
	eITEMDATAVERSION_ENCRYPT,
	eITEMDATAVERSION_ENCRYPT2,
	eITEMDATAVERSION_LAST,
};

const	int c_iItemDataLastVersion	=	eITEMDATAVERSION_LAST-1;
#define	dLAST_ITEM_DATA_VERSION	c_iItemDataLastVersion	//	VA에서 색깔 표시 하려고..--

#define	dITEM_FILE_NAME	"item.dat"

#endif