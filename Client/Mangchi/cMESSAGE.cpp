#include "cMESSAGE.H"
#include "cAREA.H"

///////////////////////////////////////////////////////////////
//	각종 문장을 관리한다.
//	여기 이외에 문장이 있는 화일
//
//	cITEM_ADDITIONAL_DATA.CPP
//
//
///////////////////////////////////////////////////////////////

//	길드 연고지 리스트
char	*g_strRelatedPlaceOfGuildList[]	=
{
"Godov Brunnensteig","0",
"Mining City Hanov","36",
"Port city Strasserat","375",
"The oasis city of Ariane","193",
"Magic City of Smug","214",
"Holy City of Augusta","71",
"New Kingdom Bigapple","373",
"Logging village Brantil","347",
"Big Village Baliat","310",
"Port City Bridgehead","74",
	"end",
};



char	*g_strMessage[]	=
{
// 0~5
"No more maps can be added.",
"The horizontal or vertical size of the image is not a multiple of 64*32.\n\nSome images that exceed the specifications will be cropped.",
"_Field Entire",
"Image data to be used as a map has not been set.",
"Exiting the editor.",
"There is unsaved data in the map.\n\nDo you want to save the data?",

// 6~10
"There was a problem loading the map",
"There was a problem loading the map image.",
"The whole map image must be 256 colors.",
"Error closing map",
"The last remaining map cannot be closed.",

// 11~15
"Reset map information",
"The size of the map you are trying to reset is smaller than the existing map. Some information will be lost.\n\nDo you want to continue?",
"There are no registered tasks.",
"Karma addition error!!",
"Karma is full.",

// 16~20
"Add new karma",
"Error adding trigger!!",
"Trigger is full.",
"Error adding action!!",
"Action is full.",

// 21~25
"Job setting error!!",
"The default occupation for Job 1 has not been set.",
"The name is empty.",
"Add Hero",
"Remove the hero",

// 26~30
"Edit hero data",
"Project setting error!!",
"A project with the same name already exists.",
"A project folder with that name cannot be created.",
"The project list folder cannot be changed arbitrarily.",

// 31~35
"Project loading error!!",
"Error saving project!!",
"map name",
"map size",
"tile set",

// 36~40
"barrel map",
"A file with the same name already exists.",
"Do you want to overwrite the next file?",
"Map file",
"Load map file",

// 41~45
"Load",
"The file does not exist.",
"Add map file",
"Load project",
"Save project",

// 46~50
"Project file",
"eliminate",
"Remove file",
"Are you sure you want to remove the selected files?",
"File deletion error!!",

// 51~55
"The selected file cannot be deleted.",
"Project initial setup",
"Default Karma Group",
"All Heroes",
"All party members",

// 56~60
"hero",
"All Characters",
"All Monsters",
"Something",
"_screen",

// 61~65
"You do not have the minimum data required to establish this karma.",
"There was a problem adding a new karma value.",
"A karma value with the same name already exists.",
"No more karma value can be added.",
"name",


// 66~70
"default value",
"Global Karma Value",
"Local Karma Value",
"Set karma value",
" * Global variable settings. Applies to the entire project. Variables that affect the entire project, not just the current map, can be registered and managed here.",

// 71~75
" * Local variable settings. Applies only to the current map. Variables that are only needed for the current map, not the entire project, can be managed by registering them here.",
"The default karma value is empty.",
"The name of the karma value is empty.",
"Quest List",
"Quest addition error!!",

// 76~80
"Quest name is empty.",
"Quest description is empty.",
"A quest with the same name already exists.",
"Quest is full",
"correction",

// 81~85
"cancellation",
"Clear selected area",
"Edit zone information",
"Do you want to continue?",
"Clear the selected area.",

// 86~90
"*Entrance type",
"* detail",
"* Inquiry Status",
"*State of the box",
"It’s not a door.",


// 91~95
"check",
"cancellation",
"Project Karma Set",
"Local Karma Set",
"Sets a local karma set.",

// 96~100
"Set up the project karma set.",
"Details of",
"Karma",
"Removes selected karma.",
"Copy",

// 101~105
"cut",
"paste",
"Remove selected reactions.",
"Remove selected triggers.",
"Area setting error!!",

// 106~110
"A zone with the same name already exists.",
"Area addition error!!",
"Map settings change error!!",
"The settings cannot be changed because this map is a bridge map.",
"This map is a bridge map.",

// 111~115
"Map loading error!!",
"The map is already loaded.",
"A map with the same title already exists.",
"Error adding new map!!",
"This map is a bridge map. Only karma settings work.",

// 116~120
"Choose a career",
"The two hero professions are the same.",
	"A character with the same name already exists.",
	"Add Monster",
	"Monster Fix",

	// 121~125
	"Eliminate monsters",
"Error adding monster",
	"name",
	"job",
"No job 2",

	// 126~130
"Add special area",
	"classification",
"Common technology",
"Unique technology",
	"correction",

	// 131~135
	"level",
	"Deletes the selected job.",
"Are you sure you want to delete the selected job ?",
"Hero Data Settings",
	"part",

	// 136~140
	"equipment",
	"Additional Effects",
"Application figures",
	"method",
	"hour",

	// 141~145
	"explanation",
	"Numerical range",
"Both numbers / percentages can be applied",
"Add to existing figures",
"Apply as a percentage to existing figures",

	// 146~150
	"duration",
	"technology",
	"Abilities",
"Success Rate",
	"range",

	// 151~155
"Basic job setting error !!",
"I already have the same job.",
	"doesn't exist",
"All types of equipment / items",
"There are no items set.",

	// 156~160
	"No restrictions",
"Error adding basic item !!",
"Please select the type of item first",
	"There is a problem setting up some data.",
"An item with the same name already exists.",

	// 161~165
	"The item name is empty.",
	"No description",
	"Icon Shape",
"Shape that fell on the field",
"Appearance when worn",

	// 166~170
	"No change",
	"price",
	"stamina",
	"damage",
"Defense",

	// 171~175
"Restrictions on wear / use",
"tendency",
	"strength",
	"agility",
	"constitution",

	// 176-180
	"wisdom",
	"knowledge",
	"charisma",
	"luck",
	"weight",

	// 181~185
	"attack speed",
	"Basic data",
"Error adding item effect !!",
	"Only 6 or fewer effects can be set for one item.",
	"Do you want to remove the following item effects?",

	// 186~190
	"effect",
	"Do you want to remove the following items?",
	"No side effects",
	"Error adding technology",
	"Technology is full",

	// 191~195
"Wearable Jobs",
"All Jobs",
"Attack Distance",
"You are not wearing any equipment.",
"Magic &&Technology",

	// 196~200
"Monster job data settings",
"NPC Occupation Data Settings",
	"monster",
"No job",
"Error adding new job !!",

	// 201~205
	"A job with the same name already exists.\n\nTemporarily set to a job with the following name.",
"Number of registered heroes",
"Number of registered monsters",
"Number of registered NPCs",
	"frequency",

	// 206~210
	"quantity",
	"Add NPC",
	"Error writing file",
	"The following file cannot be opened",
"Error opening file !!",

	// 211~215
	"The project folder cannot be found.",
"Save Map",
"Dungeon save error !!",
	"The first map is not set.",
	"Demon folder was not specified.",

	// 216~220
	"The dungeon name is empty.",
	"No starting map has been set.",
	"Save dungeon data.",
	"The folder where dungeon data will be saved must not be changed.",
	"Select the folder where the dungeon maps are saved.",

	// 221~225
"Map folder selection error !!",
	"You must select a subfolder (where the map is saved) of the project map folder.",
	"Select a starting map.",
"Map selection error !!",
	"First, select the folder where the dungeon maps are stored.\n\nThe folder is incorrectly specified or not set up.",

	// 226~230
	"The following file is not a Red Jewel map file.",
	"There is modified data.",
	"A folder with the following name could not be found under the project map folder.",
	"\"001.rmd\"File not found.\n\nAt least one file must exist to save the dungeon.\n\nex) 001.rmd 002.rmd ...",
	"Select a map.",

	// 231~235
	"movement",
	"entrance",
	"Shape of the doorway",
"The entrance in the next direction has already been set.",
"A starting area is already set up.",

// 236~240
"The realm name is empty.",
"No monster was selected.",
"Limited number of monsters created",
"Select all",
"Select all characters",

// 241~245
"Select all objects",
"Select all buildings",
"select",
"Cancel Edit",
"Character data settings",

// 246~250
"doesn't exist",
"Block rate",
"Rare level",
"link",
"Generation level",

// 251~255
"Too many monsters are linked.",
"Remove Link",
"leader",
"factor",
"Monster activity area",

// 256~260
"_Do not move",
"Set as leader",
"Set as factor",
"Number of areas",
"Number of monsters/NPCs",

// 261~265
"There is a strange value. Set it to the default or existing value.",
"shadow",
"Are you sure you want to remove the following item data?",
"Error adding item prefix!",
"Duplicate prefix or identification code.",

// 266~270
"The prefix is empty.",
"every",
"Are you sure you want to remove the following prefix data?",
"The price is empty. Please check with the price calculation formula.",
"Conversation from '%s'",

// 271~275
"Add selection condition for starting conversation with '%s'",
"Modify selection conditions for starting conversation with '%s'",
"Add conversation set with '%s'",
"Modify conversation set with '%s'",
"Add dialogue with '%s'",

// 276~280
"Modify dialogue with '%s'",
"The conditions for being selected first are not registered.",
"会話NPC",
"ショップNPC",
"The movement area is incorrect.",

// 281~285
"There is no connected area.",
"Set shadow on selected stationary object",
"Remove shadows from selected fixed objects",
"Event Information",
"Information upon death",

// 286~290
"Remove the selected events.",
"Flayer",
"party",
"Event Settings",
"Select all the same objects in the field",

// 291~295
"Select all the same objects on the screen",
"The prefix '%s' cannot be attached to the selected item.\n\nDo you want to continue?",
"There was a problem processing the operation.",
"This item does not overlap more than %d times.",
"The first choice responds without dialogue",

// 296
"Copy selected area",
"Paste selected area",
"Select all characters like the selected character",
};

char* g_strActorPowerLimit = "Level 1 ~ 1000\nExperience 0 ~ 4294967295\n\nHealth 10 ~ 10000\nPower Gauge 1 ~ 5000\n\nStrength 5 ~ 1000\nAgility 5 ~ 1000\nHealth 5 ~ 1000 \nWisdom 5 ~ 1000\nKnowledge 5 ~ 1000\nCharisma 5 ~ 1000\nLuck 5 ~ 1000\n\nAttack power 1 ~ 50000\nDefense power 1 ~ 50000\n\nAccuracy rate 5% ~ 95%\n Evasion rate 5% ~ 95%\nBlock rate 0% ~ 95%\nCritical hit 0% ~ 95%\n\nFire resistance 0% ~ 100%\nWater resistance 0% ~ 100%\nWind resistance 0% ~ 100 %\nEarth resistance 0% ~ 100%\nLight resistance 0% ~ 100%\nDark resistance 0% ~ 100%\n\nParalysis correction value 0% ~ 100%\nPoison correction value 0% ~ 100%\n Freeze correction value 0% ~ 100%\nMovement speed correction value 10%~ 200%\nAttack speed correction value 10% ~ 200%\n";

///////////////////////////////////////////////////// /////////////////////////////////////////////////////
///////////////////////////////////////////////////// /////////////////////////////////////////////////////
// Declared in the cACTOR.H class.
///////////////////////////////////////////////////// /////////////////////////////////////////////////////
///////////////////////////////////////////////////// /////////////////////////////////////////////////////

char *g_strBattleInclination[] =
{
"Passive/not paying attention to surroundings.",
"Passive/Help someone if they are being attacked.",
"Aggressive/not paying attention to surroundings.",
"Aggressive/Helps someone if they are being attacked.",
"end"
};

char *g_strMonsterGrade[] =
{
"General Stage 1",
"General Stage 2",
"General Stage 3",
"General Stage 4",
"Semi-boss stage 1",
"Semi-boss stage 2",
"Semi-boss level 3",
"Boss Stage 1",
"Boss Stage 2",
"Boss Stage 3",
"end"
};

char *g_strLinkType[] =
{
"No link",
"two-way link",
"one-way link",
};

///////////////////////////////////////////////////// /////////////////////////////////////////////////////
///////////////////////////////////////////////////// /////////////////////////////////////////////////////
// Declared in CCharacter.H class.
///////////////////////////////////////////////////// /////////////////////////////////////////////////////
///////////////////////////////////////////////////// /////////////////////////////////////////////////////

char *g_astrNpcSkin[] =
{
"doesn't exist",
"Apprentice Knight",
"Warrior",
"Wizard",
"werewolf",
"Priests",
"Fallen angel",
"thief",
"Martial Artist",
"Magic Spear Swordsman",
"Magic Archer",
"breaker",
"Summoner",
"princess",
"Transforming Girl",
"Spiritualist",
"devil",
"Operator (male angel)",
"Operator (Female Angel)",
"Spiritualist",
"projection",
"History of Optics",
"Suin",
"end",
};

char *g_strMonsterEventCheckCondition[] =
{
"Killed by the Flayer.",
"The battle has begun.",
"I killed the flayer.",
"Always check",
"end",
};

///////////////////////////////////////////////////// /////////////////////////////////////////////////////
///////////////////////////////////////////////////// /////////////////////////////////////////////////////
// Declared in cJOB.H class.
///////////////////////////////////////////////////// /////////////////////////////////////////////////////
///////////////////////////////////////////////////// /////////////////////////////////////////////////////

#include "cJOB.H"


char *g_aDefaultJobName[] =
{
"Apprentice Knight",
"Warrior",
"Wizard",
"werewolf",
"Priests",
"Fallen angel",
"thief",
"Martial Artist",
"Magic Spear Swordsman",
"Magic Archer",
"breaker",
"Summoner",
"princess",
"Transforming Girl",
"Spiritualist",
"devil",
"Spiritualist",
"projection",
"History of Optics",
"Suin",
"end",
};

// tendency
char *g_strAllignment[] = {"No restrictions", "Absolute evil", "Evil", "Neutral", "Good", "Absolute good"};

// Classification of occupation
char *g_strJobCategory[] =
{
"Undead type",
"humanoid",
"Devil type",
"animal type",
"Shin Soo-hyung",
"end"
};

//
// NPC type
char *g_strNpcCategorty[] =
{
"プレイヤー",
"通常NPC",
"モンスター",

"武器商店",
"防具商店",
"雑貨商店",
"アクセサリー商店",
"露店(雑貨商店を使用)",
"銀行",
"スキルマスター",

"一般クエスト",
"称号関連クエスト",
"ギルドクエスト",
"メインストーリークエスト",
"ヘルパー",
"テレポーター",
"治療師",
"クエスト案内人",
"鍛冶屋",
"初心者ヘルパー",
"進行中クエスト依頼人",
"進行中クエスト関係者表示",
"討伐対象",
"剣士/戦士 武器商人",
"ウィザード/ウルフマン 武器商人",
"ビショップ/追放天使 武器商人",
"シーフ/武道家 武器商人",
"ランサー/アーチャー 武器商人",
"ビーストテイマー/サマナー 武器商人",
"プリンセス/リトルウィッチ 武器商人",
"ネクロマンサー/悪魔 武器商人",
"決闘場報酬",
"霊術師/闘士 武器商人",
"ギルドホールテレポーター",
"イベント案内人",
"冒険者協会",
"光奏師/獣人 武器商人",
	"end"
};	//	char *g_strNpcCategorty[]


char	*g_strMonsterLevel[]	=
{
"General 1",
"General 2",
"General 3",
"General 4",
"Semi-Boss 1",
"Semi-Boss 2",
"Semi-Boss 3",
"Boss 1",
"Boss 2",
"Boss 3",
	"end"
};
char* g_strSeasonKind[] =
{
"Season 0",
"Season 1",
"Season 2",
"Season 3",
"Season 4",
"Season 5",
"Six Seasons",
"Seven Seasons",
"Season 8",
"Season 9",
"10 Seasons",
"Season 11",
"12 Seasons",
"Season 13",
"Season 14",
"Season 15",
"Season 16",
"Season 17",
"Season 18",
"Season 19",
"20 Seasons",
"end",
};
char* g_strSwapSlotKind[] =
{
"No",
"0 slot",
"1 slot",
"2 slots",
"3 slots",
"4 slots",
"5 slots",
"6 slots",
"7 slots",
"8 slots",
"9 slots",
"end",
};

char* g_strAreaKind[] =
{
"General Area",
"notice board",
"door",
"entrance",
"Monster activity area",
"Start Area",
"Name display area",
"Guild Battle Area",
"OX Quiz Area (O)",
"OX Quiz Area (X)",
"Place of Resurrection",
"trap",
"Event Area",
"treasure box",
"Guild Crest",
"Guild Object",
"Bidun Exit",
"Hunting ground", // insu add
"end",
};

char* g_strPortalShape[] =
{
"Move in the field", "Move in the building", "One-way traffic", "Invisible", "Move in the field", "Move in the field (hidden)",
"Guild hall entrance", "Guild hall->Village", "Dungeon movement", "Tower movement", "Movement within the building", "end",
};
char* g_strPortalDirect[] =
{
"North","Northeast","East","Southeast","South","Southwest","West","Northwest","end",
};

char* g_str8Direct[] =
{
"north",
"northeast",
"dong",
"southeast",
"other",
"southwest",
"west",
"northwest",
"random",
};
// 09.12.09 추가 몬스터 컨셉
char *g_strMonsterConcept[] =
{
	" ", "G", "N", "B"
};



////////////////////////////////////////////////////////////////////////////////////////////////
//	setItem.h
////////////////////////////////////////////////////////////////////////////////////////////////
#include	"setItem.h"

char	*g_strItemReversionMethod[]	=
{
"doesn't exist",
"Vested upon acquisition",
"Bind when worn",
	"end",
};



////////////////////////////////////////////////////////////////////////////////////////////////
//	cITEM.H
////////////////////////////////////////////////////////////////////////////////////////////////

#include	"cITEM.H"

char	*g_strPrefixOutputEffect[]	=	
{
"Primary physical damage",
"Secondary physical damage",
"Physical damage 3rd",
"First fire damage",
"Secondary fire damage",
"Fire damage 3rd time",
"Primary water damage",
"Secondary water damage",
"Water Damage 3rd",
"Primary wind damage",
"Secondary wind damage",
"Wind damage 3rd",
"Earth damage 1st",
"Second Earth Damage",
"Earth damage 3rd",
"Primary light damage",
"Secondary light damage",
"Light damage 3rd",
"Primary darkness damage",
"Second Dark Damage",
"Darkness damage 3rd",
"Defense 1st",
"Second Defense",
"3rd defense",
"Basic ability level 1",
"Basic ability level 2",
"Basic ability level 3",
"First stamina increase",
"2nd stamina increase",
"3rd stamina increase",
"CP increase 1st time",
"CP Increase 2nd",
"CP increase 3rd time",
"Elemental Resistance Primary",
"Elemental Resistance Secondary",
"Elemental Resistance 3rd",
"Ideal resistance 1st order",
"Secondary resistance resistance",
"Ideal Resistance 3rd Order",
"Spikes 1st",
"Thorns 2nd",
"Third of Thorns",
		"end",				
};

char	*g_strPremiumServiceLevel[]	=
{
	"No rating",
	"Lite Lite",
	"Lite Premium",
	"Lite Platinum",
	"Lite",
	"Premium",
	"Platinum",
"Acquire additional experience",
"Portal excluding experience points",
	"end",
};

char	g_strViewEquipmentName[80][20][40]	=
{
	{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},{"end"},
{
"Level 1",
"Step 2",
"Step 3",
"end"
},
{
"Level 1",
"Step 2",
"Step 3",
"end"
},
{
"Short Sword",
"Long Sword",
"Bastard Sword",
"Broadsword",
"Banwoldo",
"Unwoldo",
"sword stick",
"Cutlass",
"chris",
"Cinquedia",
"Hand Axe",
"end",
},
{
"circular shield",
"Great Shield",
"Kite Shield",
"Tower Shield",
"Spike Shield",
"Main Gauche",
"end",
},
{
"Two-handed sword",
"Zweihander",
"Bastard Sword",
"Flamberge",
"Executioner's Sword",
"tomahawk",
"battle axe",
"end",
},
{
"cane",
"little cane",
"long staff",
"Hiking stick",
"Magic Wand",
"end",
},
{"end"},
{
"club",
"mace",
"hammer",
"Battle Hammer",
"Morning Star",
"hall",
"flail",
"end",
},
{
"copper wings",
"end",
},
{
"dart",
"dagger",
"Recognition",
"boomerang",
"Bola",
"Tomahawk",
"Chakram",
"end",
},
{
"small bow",
"long bow",
"composite bow",
"end",
},
{
"end",
},
{
"pike",
"long spear",
"Wing Spear",
"halberd",
"Lance",
"trident",
"Lee Ji-chang",
"axe spear",
"Glave",
"javelin",
"end",
},
{
"Pipe",
"clarinet",
"oboe",
"recorder",
"Flute of Grass",
"Bamboo Flute",
"end",
},
{
"sling",
"Chain Sling",
"end",
},
{
"end",
},
{
"rod",
"baton",
"commanding rod",
"Bong Mike",
"Magic Wand",
"hall",
"end",
},
{
"kourbash",
"Chain Whip",
"Flame Whip",
"end",
},
	{"end"},//	"보석",
	{"end"},//"체력 포션",

	{"end"},//"차징 포션",
	{"end"},//"스탯 향상 포션",
	{"end"},//"성능 향상 포션",
	{"end"},//"치료약",
	{"end"},//"상태 회복제",
	{"end"},//"열쇠",
	{"end"},//"포탈 스톤",
	{"end"},//"필살기 두루말이",
	{"end"},//"먹을 것, 기타",
	{"end"},//"능력향상제",
	{"end"},//"강화액",
	{"end"},//"마법 보석",
	{"end"},//"이벤트 아이템",
	{"end"},//"퀘스트 아이템",
	{"end"},//"프리미엄 아이템",
	{"end"},//"인챈트 아이템",
	{"end"},//"아이템 팩",
	{"end"},//"모든 무기",
		{
			"scythe",
			"Bird Scythe",
			"Crow Scythe",
			"Diamond Scythe",
			"Giant Scythe",
			"Hook Scythe",
			"Horse Slasher",
			"Mower",
			"Semicircle Scythe",
			"Sickle Spear",
			"Silver Scythe",
			"Steel Scythe",
			"Thick Scythe",

	"end"},// 낫
	{

"LS basic model",
"LS blade type",
"LS nail type",
"LS3 blade type",
"LS prototype",
"HL snake type",
"HS brother-in-law",
"HS bear type",
"HL puma type",
"HS shield type",
"HL Dragon type",
"HL Dragon Type 2",

	"end"},	// 손톱무기
	{
					"Ancient Book",
					"Thick Book",
					"Thin Book",
					"Heavenly Book",
					"Tartarean Book",
					"Under Seal Book",
					"Huge Book",
					"Archaic Parchment",
					"Woodblock",
					"Slate",
					
	"end"}, // 책
	{"end"},
	{"end"},
	{"end"},
	{"end"},
	{"end"},

};


char* g_strPriceComputeMethod[] =
{
"Treat by price",
	"Number 1",
	"Price*Number 1",
	"Price*Number2",
"Price*(Number 1+1)",
"Number 1*Number 2/Price",
	"Price/10",
"Number 1*Number 2*Price",
	"end"
};

char* g_strEquipUseLimitMethod[] =
{
	"numerical value1 * ",
	"numerical value2 * ",
	"Number 1 / ",
	"Number 2 / ",
	"end"
};

char* g_strItemPrefixPriceComputeMethod[] =
{
"Treat by price",
	"Number 1",
"Number 1*Price",
	"Number 1/Price",
"Number 1*Price/100",
"Price*Number 2/Number 1",
	"end"
};

char* g_strEquipUseLimitContents[] =
{
	"level",
	"strength",
	"agility",
	"health",
	"charisma",
	"knowledge",
	"wisdom",
	"luck",
	"end"
};

char* g_strEquipmentPart[] =
{
	"helmet",
	"Armor",
	"earring",
	"necklace",
	"belt",
	"shoes",
	"Gloves",
	"Ring 1",
	"Ring 2",
	"Ring 3",
	"Ring 4",
	"Ring 5",
	"Ring 6",
	"Ring 7",
	"Ring 8",
"Weapon (Profession 1)",
"Shield (Job 1)",
"Bullet (Job 1)",
"Weapon (Profession 2)",
"Shield (Job 2)",
"Bullet (Job 2)",
	"Belt 1",
	"Belt 2",
	"Belt 3",
	"Belt 4",
};

char* g_strItemCategory[] =
{
"helmet",
"coffin",
"Gloves",
"Substitute for gloves",
"toenail",
"bracelet",
"belt",
"Boots",
"necklace",
"ring",
"earring",
"Cloak",
"brooch",
"arm tattoo",
"shoulder tattoo",
"The Cross",
"Common Armor",
"Exclusive Armor",
"one-handed sword",
"shield",
"Two-handed sword",
"Magic Wand",
"teeth",
"mace",
"wing",
"dagger",
"bow",
"arrow",
"window",
"Pipe",
"sling",
"Sling Bullet",
"Magic Wand",
"whip",
"jewel",

"Stamina Potion",
"Charging Potion",
"Stat Enhancement Potion",
"Performance Enhancement Potion",
"cure",
"Status restorer",
"key",
"Portal Stone",
"Special Move Scroll",
"food, etc.",
"Performance enhancer",
"reinforcement liquid",
"Magic Jewel",
"Event Item",
"Quest Item",
"Premium Item",
"Enchant Item",
"Item Pack",

"All weapons",
"sickle",
"Nail weapon",
"book",
"end",
};


///////////////////////////////////////////////////// /////////////////////////////////////////////////
//cAI.H
///////////////////////////////////////////////////// /////////////////////////////////////////////////
#include "cAI.H"

char* g_strMovePattern[] =
{
"doesn't exist",
"Stick to your current position.",
"Wanders around and attacks enemies within sight.",
"It sticks to a specific area and wanders around to attack enemies that come into view.",
"end",
};

char* g_strBattleAI[] =
{
"doesn't exist",
"default",
"wandering",
"Tracking",
"Defense of a specific area",
"Protection of specific characters",
"assassination",
"Incomprehensible behavior",
"Flayer",
"end"
};

char* g_strAttackPart[] =
{
"entire",
"Top",
"stop",
"lower"
};

///////////////////////////////////////////////////// /////////////////////////////////////////////////
//CDialog.H
///////////////////////////////////////////////////// /////////////////////////////////////////////////
char* g_strDialogWindowSizeList[] =
{
"largest size",
"middle size",
"Balloon chat window",
"Right chat window",
"end",
};

char* g_strTrapTarget[] =
{
"target",
"range",
"end",
};

////////////////////////////////////////////////////////////////////////////////////////////////
//	cMAP.H
////////////////////////////////////////////////////////////////////////////////////////////////
#include "cMAP.H"

char* g_strTileSet[] =
{
"Grassland" ,"Grassland",
"Mountains" ,"Mountains",
"Desert" ,"desert",
"Savana" ,"Savana",
"Cave" ,"cave",
"Dungeon" ,"Dungeon",
"Tower" ,"Tower",
"Mine" ,"mine",
"Hell" ,"Underworld (Hell)",
"Heaven" ,"Heaven",
"Brunenstig" ,"Village - Brunenstig",
"Bigaple" ,"Village - Bigaple",
"Augusta" ,"Village - Augusta",
"Bridgehead" ,"Village - Bridgehead",
"Mountains Village" ,"Village - Mountain Village",
"Arian" ,"Village - Arian",
"Ruined City" ,"Village - ruins",
"FarmHouse" ,"Village - Farmhouse",
"Gypsy" ,"Village - Gypsy Village",
"Room" ,"Inside the house",
"GuildHall" ,"Guild Hall",

"end",
};

char* g_strMapType[] =
{
" ",
"Battle Field",
"Town",
"Inside the building",
"end",
}; // map type string
// Added weather 10.02.10
char* g_strWeatherType[] =
{
"Sunny",
"eye",
"rain",
"end",
};

///////////////////////////////////////////////////// /////////////////////////////////////////////////
//CQuest.H
///////////////////////////////////////////////////// /////////////////////////////////////////////////
char* g_strQuestKind[] =
{
"General quest (completion check)",
"Part-time job (this is also checked for completion)",
"end",
};

char* g_astrQuestCategory[] =
{
"General Quest",
"Skill Master",
"Title Quest",
"Main Story Quest",
"Guild Quest",
"end",
};

/*
AI의 기본은 그룹이다. 그룹의 AI에 따라서 행동을 함. 그룹이 없으면
(혹은 그룹이 와해되면) 캐릭터 각각의 이동 성향에 따라 이동하고,
전투시에는 전투 성향에 따라 전투를 한다.


이동 성향		-	
	기본 AI
		1> 서 있음 
			-	서 있다가 적이 오면 공격을 하고 사라지면, 그자리에 멈춰 선다.
				대사1> ... 심심하다. -o-

		2> 현재 위치 고수
			-	서 있다가 적이 오면 공격을 하고 사라지면, 현재 위치로 돌아 옴
				대사1> 뭔가... 한가하군.. -_/~~~

		3> 패트롤
			-	일정 영역 안을 순찰하다가, 적을 발견하면 공격하고 적이 사라지면
				다시 순찰을 한다.
				대사1> 한놈만 걸려라 -_-+

	예외 상황
		1> 대피
			-	적이 공격을 하고 있지만, 반격을 할 수 없을때 대피를 한다. 일정 시간이
				지나면 기본 AI에 의거해 행동함
				대사1> 아씨!! 치사한 녀석!!


전투 성향		-
	기본 AI
		1> 평화적
			-	공격해 오기 전에는 공격을 하지 않는다.
				대사1> 안녕하세요~

		2> 호전적
			-	눈에 보이는 넘은 무조건 공격한다.
				대사1> ... 딱 걸렸어!!

		3> 신중함
			-	전황 혹은 전력을 분석해서 만만하다고 판단되면 공격을 한다. -_-
				대사1> 훗~ 애송아 덤벼라!!

		4> 겁쟁이
			-	전력을 분석해서 자기 보다 강하다고 판단되면, 슬금슬금 피함
				대사1> 앗!! 저쪽에 별똥별이!!

				자기보다 약하다고 판단되면, 공격함
				대사1> 본좌의 힘을 보여 주마... 크하하하!!

				싸우다가 전력을 분석해서 많이 불리해지면 도망감 -_-
				대사1> 쿠.. 쿨럭... 갑자기 지병이... 두... 두고보자!!

		5> 짜증나는넘
			-	한대 때리고 도망간다. 다시 온다. 한대 때리고 도망간다. 다시 온다.  ....
				대사> 나 잡아 봐~~~라~~~ 
		

	예외 상황
		1> 분노
			-	호전적으로 변함. 일정 시간이 지나면 기본 AI로 돌아 온다.
				대사1> 크아아아!! 이제 못참아!! 너 일루와!!

		2> 위축
			-	도망갈때 AI이다. 무조건 도망간다. 시간이 지나면 기본 AI로 돌아 온다.
				대사1> 커헉!! 사... 살려주세요!! ㅠ.ㅜ

그룹 대형	-
	기본 대형
	1> 산개
		-	일정 거리 안에 이리저리 흩어져 있다가 전투가 일어나면 몰려와 다구리 친다. -_-
			

	2> 기본 대형
		-	특정 캐릭터를 보호하는 모양으로 뭉쳐 있는다.



그룹 전투 성향	-
	기본적으로 그룹이 설정되어 있으면 
	기본 AI
		1> 리더 보호
			-	리더를 중심으로 옹기종기 모여 있는다.

		2>	

전력 분석		- 자신과 상대의 레벨,공격력,체력,마력,방어력등을 고려해서 전력을 분석한다.
전황 분석		- 시야안에 혹은 특정 영역안에 전황을 분석한다.(전력 분석이 기본)
그룹 행동		- 
리더

적 선택			- 전력 분석,전황 분성,전투 성향을 기본으로 적을 선택한다.
공격 패턴 선택	- 공격력,마력,거리,속성등을 고려해 공격 패턴을 정한다.
도망 갈 조건	- 전력 분석,전황 분성,전투 성향을 기본으로 도망갈지 정한다.
*/
