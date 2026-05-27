#include "CImageData.H"
#include "cJOB.H"
#include "cMESSAGE.H"
#include "CGame.H"
#include "firecracker.H"
#include "cMap.H"
#include "mess_sign.H"

enum
{
	eANM_LM_NORMAL,
	eANM_LM_ALWAYS,
	eANM_LM_QUICK,
};

CImageManager		g_im;	//	이미지 데이터 메니져
cSPRITE				g_sprFieldItem,g_sprInterface,g_sprInterface2,g_sprNameBar,g_sprHitText;
cSPRITE				g_sprRegionInterface;
cSPRITE				g_sprGameBottomInterface,g_sprLoading,g_sprLoading2;
cSPRITE				g_sprBook;
cSMI				g_smiIconItem,g_smiIconSkill,g_smiIconGuildSkill,g_smiLoading,g_smiIconGGG;//	아이콘들
cSPRITE				g_sprGuildIconShape[dGUILD_MARK_ICON_SHAPE_PATTERN_COUNT],g_sprGuildHallLevel,g_sprGuildIconOutLine,g_sprGuildIconBaseShape;
cSPRITE				g_sprMonsterDiscerner;
cSPRITE				g_asprPitchmanShop[c_iPitchmanShopShapeCount];
cSPRITE				g_sprWorldMap;
cSPRITE				g_sprTitle;			// 10.02.18
cSPRITE				g_sprArca,g_sprBanner,g_sprMainQuestInterface,g_sprMiniPetIcon;
cSPRITEQ			g_sprLoadingPannel,g_sprMainQuestEnding;
cSMI				g_smiGuildIconBaseColorPartition;
cANMQ				g_anmMainQuestImage;
cANM				g_aDuelBuff[4];
cANM				g_aGGGBuff[4];

cANM_BASE			*g_apEffect[dEFFECT_IMAGE_DATA_COUNT];
cANM_BASE			*g_apBody[dJOB_IMAGE_DATA_COUNT];
cANM_BASE			*g_apMonsterVariation[dJOB_IMAGE_DATA_COUNT][dMONSTER_VARIATION_COUNT];
cANM_BASE			*g_alpBody[dJOB_IMAGE_DATA_COUNT];
cANM				*g_apIBody[dJOB_IMAGE_DATA_COUNT];
cANM				*g_apIMonsterVariation[dJOB_IMAGE_DATA_COUNT][dMONSTER_VARIATION_COUNT];
cANM				g_aMiniPetBody[nsMiniPetType::Count];
cANM_BASE			*g_apMonsterParts[dJOB_IMAGE_DATA_COUNT][dMONSTER_VARIATION_COUNT];

cANMQ				g_aQBody[100];
cANM				g_aBody[100];
cANM				g_aTransNPC[6];
cANM				g_anmHelp,g_anmItemType;
cANM				g_anmSageStone,g_anmGuildFlag,g_anmGuildMarkGlow,g_anmMagicalShield;

cANM_BASE			*g_apWeapon[dPLAYER_JOB_COUNT][dEQUIPMENT_SHAPE_VARIATION];
cANM				g_aShield[dPLAYER_JOB_COUNT][dEQUIPMENT_SHAPE_VARIATION];
cANM				g_anmEtc,g_anmGate,g_anmNpcType,g_anmJewel,g_anmGuildBattlePortal,g_anmQuestNpcMark;
cANM				g_aAnmMagicCarpet[dMAX_MAGIC_CARPET_SHAPE_COUNT];
cANM				g_aJobIcon[dPLAYER_JOB_COUNT];	//	직업 에니 아이콘
cANM				g_anmGameManager;
cANM				g_anmLongRain, g_anmShortRain, g_anmBombRain;				// 10.02.10 추가
CImagePalette		g_aPalette[dJOB_IMAGE_DATA_COUNT];
CGuildMarkPalette	g_guildMarkPalette;
CGuildMarkImage		g_guildMark;
cANM				g_aGuildPet[dGUILD_PET_TYPE_COUNT];
BOOL				g_bIsPutSceneWhenLoading	=	FALSE;
WORD*				g_pRedDrakePalette			=	NULL;
WORD*				g_pRebirthPalette			=	NULL;
WORD*				g_pRebirthBallPalette		=	NULL;

char				l_strMonsterNames[300][32];

int		g_iWorldMapJobIconIndex[dPLAYER_JOB_COUNT] = 
{		// 월드맵 캐릭터 아이콘 인덱스...
1,
2,
3,
4,
5,
6,
7,
8,
9,
10,
11,
12,
13,
14,
15,
16,
58,
59,
60,
};

cMonsterBodyLoadInfo	g_aMonsterBodyLoadInfo[100]	=
{
	1,	1,	//	언데드	해골전사 도끼	Skeleton, Axe		175㎝		기존
	1,	1,//	언데드	해골전사 쌍칼	Skeleton, Sword		175㎝		기존
	2,	10,//	언데드	해골기사	Skeleton Knight		190㎝		기존
	1,	1,//	언데드	좀비	Zombie		170㎝		기존
	1,	1,//	언데드	산송장	Living Dead		155㎝		기존
	0,	1,//	언데드	유령	Ghost		180㎝		기존
	1,	1,//	언데드	미이라	Mummy		200㎝		기존
	2,	1,	//	언데드	고스트 아머	Ghost Armor		250㎝		기존//	0,	1,
	2,	20,//	언데드	뱀파이어	Vampire		180㎝	흡혈귀, 세미보스다운 귀족적 스타일	세미보스
	0,	1,//	언데드	리치	Lich		210㎝	공중 부양, 허리 밑은 없음. 머리부터 허리까지는 100㎝

	1,	1,//	인간형	블랙 엘프	Black Elf		160㎝		기존
	1,	1,//	인간형	어새신	Assassin		165㎝	도적 스타일	기존
	1,	1,//	인간형	원주민			80㎝	부쉬맨 스타일의 작은 원주민, 무기는 몽둥이	
	1,	1,//	인간형	타락한 마법사	Fallen Wizard		145㎝	구부정한 곱사등 스타일, 구불구불한 스
	1,	1,//	인간형	궁정 기사단	Temple Knight		195㎝	장신, 멋진 풀 플레이트 메일에 할버드	
	0,	1,//	인간형	다크 프리스트	Dark Priest		160㎝	회복과 보조 마법 전문의 성직자	
	1,	1,//	인간형	어둠의 소환사	Conjuerer		165㎝	채찍으로 무장	
	2,	20,//	인간형	자이언트	Giant		230㎝		세미 보스, 기존
	2,	20,//	인간형	엘프 킹	Elf King		175㎝	칼과 롱보우 소지	보스
	0,	1,//	인간형	대마술사	Archmage		180㎝	장신의 마법사, 레드아이 보스로 등장 예정	

	0,	1,//	악마형	데미 비홀더	Demi Beholder		90㎝		기존
	1,	1,//	악마형	렙타일	Reptile		120㎝		기존
	1,	1,//	악마형	닭 전사	Cock Fighter		175㎝		기존
	1,	1,//	악마형	리자드 워리어	Lizard Warrior		155㎝		기존
	0,	1,//	악마형	리자드 라이더	Lizard Rider		180㎝		기존
	1,	1,//	악마형	랫 파이터	Rat Fighter		140㎝		기존
	0,	1,//	악마형	데몬	Demon		185㎝		세미 보스, 기존
	eANM_LM_QUICK,	20,//	악마형	오우거	Ogre		210㎝		세미 보스, 기존
	eANM_LM_QUICK,	50,//	악마형	바호매트			210㎝	염소 파충류라는 것.	세미 보스, 기존
	0,	1,//	악마형	붉은 악마	Red Demon		300㎝	최종 보스 다운 거대함과 위용을 보여줄 것	

	2,	10,//	동물형	터틀 드래곤	Turtle Dragon		190㎝		기존
	1,	1,//	동물형	거미	Spider		130㎝		기존
	1,	1,//	동물형	전갈	Scorpion		180㎝		기존
	1,	1,//	동물형	해파리			170㎝		기존
	1,	1,//	동물형	웜	Worm		170㎝(길이)		기존
	1,	1,//	동물형	킹 크랩	King Crab		50㎝	키는 작지만 그 넓이까지 합치면 꽤 크다. 옆으로 걸
	0,	1,//	동물형	곤충 떼	Insect Swarm		120㎝	곤충 떼. 점으로만 이루어져도 될 듯	
	1,	1,//	동물형	늑대	Wolf		50㎝	길이는 150㎝~170㎝ 정도 된다. 거대한 늑대	
	0,	1,//	동물형	큰 두더지	Big Moul		400㎝(팔길이)		세미보스, 기존
	eANM_LM_QUICK,	30,//	동물형	미친 곰	Mad Bear		250㎝(길이)	이동시에는 네 발로 걷고 공격할 때는 서서 앞 

	1,	1,//	신수형	불도깨비			150㎝	불의 신수	기존
	0,	1,//	신수형	혓바닥눈			200㎝	물의 신수	기존
	1,	1,//	신수형	가고일	Gargoyle		155㎝	바람의 신수	기존
	eANM_LM_QUICK,	20,//	신수형	메탈 골렘	Metal Golem		220㎝	땅의 신수	기존
	eANM_LM_QUICK,	25,//	신수형	켄타우로스	Centaurs		210㎝	바람의 신수	기존
	eANM_LM_QUICK,	30,//	신수형	머맨 나이트	Merman Knight		220㎝	물의 신수	기존
	0,	1,//	신수형	팀버맨	Timber Man		230㎝	땅의 신수	기존
	0,	1,//	신수형	사라만다	Salamander		100㎝	불의 신수, 불 도마뱀	
	eANM_LM_QUICK,	40,//	신수형	다크 파이어	Dark Fire		120㎝	어둠의 신수, 검은 불로 둘러싸인 괴물 분위기	
	1,	1,//	신수형	화이트 섀도우	White Shadow		140㎝	빛의 신수, 몸집이 작은 어린 아이 같은

	1,	1,//	해골 산타

	0,	1,	//	세이지//	세이지
	0,	1,	//	세이지 마스터	,//	세이지 마스터
	0,	1,	//	티아메스(위 촉수),//	티아메스(위 촉수)
	0,	1,	//	티아메스(본체)	,//	티아메스(본체)
	0,	1,	//	티아메스(아래 촉수)//	티아메스(아래 촉수)
	0,	1,	//	워터 드래곤		,//	워터 드래곤
	0,	1,	//	아크 데빌		,//	아크 데빌
	0,	1,	//	드라코 리치		,//	드라코 리치
	0,	1,	//	드라코 리치 오브	,//	드라코 리치 오브

	eANM_LM_QUICK,	1,	//		{"자이언트해골"
	eANM_LM_QUICK,	1,	//		{"해골 마법사"
	eANM_LM_QUICK,	1,	//		{"본 키메라"
	eANM_LM_QUICK,	1,	//		{"하이 엘프"
	eANM_LM_QUICK,	1,	//		{"깡패"
	eANM_LM_QUICK,	1,	//		{"시크 교도"
	eANM_LM_QUICK,	1,	//		{"탈주자"
	eANM_LM_QUICK,	1,	//		{"광마"
	eANM_LM_QUICK,	1,	//		{"사신"
	eANM_LM_QUICK,	1,	//		{"서큐버스"
	eANM_LM_QUICK,	1,	//		{"맨티스 전사"
	eANM_LM_QUICK,	1,	//		{"펌프킨헤드"
	eANM_LM_QUICK,	1,	//		{"대왕 원숭이"
	eANM_LM_QUICK,	1,	//		{"거대 개구리"
	eANM_LM_QUICK,	1,	//		{"혀괴수"
	eANM_LM_QUICK,	1,	//		{"다크 버팔로"
	eANM_LM_QUICK,	1,	//		{"유니콘"
	eANM_LM_QUICK,	1,	//		{"사도"
	eANM_LM_QUICK,	1,	//		{"케루빔"

	0,	1,	//		{//길드 문장"

	0,	1,	//실드 크래프트"
	0,	1,	//엘레멘탈 포스트"
	0,	1,	//에테르 쉘"
	0,	1,	//할로우 나이트"
	0,	1,	//길드 깃발

	0xffff,
};

char	l_strJobIconFileName[][30]	=
{
	"job_icon_knight.sad",
	"job_icon_warrior.sad",
	"job_icon_wizard.sad",
	"job_icon_werewolf.sad",
	"job_icon_priest.sad",
	"job_icon_fallen_angel.sad",
	"job_icon_rogue.sad",
	"job_icon_fighter.sad",
	"job_icon_lancer.sad",
	"job_icon_archer.sad",
	"job_icon_tamer.sad",
	"job_icon_summoner.sad",
	"job_icon_princess.sad",
	"job_icon_magical_girl.sad",
	"job_icon_necromancer.sad",
	"job_icon_devil.sad",
	"job_icon_soul.sad",
	"job_icon_champion.sad",
	"job_icon_opticalist.sad",
	"job_icon_beast_man.sad",
};


char	l_strWeaponFileName[][20][40]	=
{
	{
		"knight - short sword.sad",
		"knight - long sword.sad",
		"knight - bastard sword.sad",
		"knight - broad sword.sad",
		"knight - Scimitar.sad",
		"knight - Falchion.sad",
		"knight - Sword Stick.sad",
		"knight - Cutlass.sad",
		"knight - Kris.sad",
		"knight - Cinquedea.sad",
		"knight - Hand Axe.sad",
		"end",
	},
	{
		"warrior - Two handed Sword.sad",
		"warrior - Zweihander.sad",
		"warrior - Bastard Sword.sad",
		"warrior - Flamberge.sad",
		"warrior - Executioner's Sword.sad",
		"warrior - Large Axe.sad",
		"warrior - Battle Axe.sad",
		"end",
	},
	{
		"wizard - Staff.sad",
		"wizard - Short Staff.sad",
		"wizard - Long Staff.sad",
		"wizard - Alpenstock.sad",
		"wizard - Magical Staff.sad",
		"end",
	},
	{"end"},	//	늑대인간용 이빨
	{
		"priest - Club.sad",
		"priest - Mace.sad",
		"priest - Hammer.sad",
		"priest - War Hammer.sad",
		"priest - Morning Star.sad",
		"priest - Scepter.sad",
		"priest - Flail.sad",
		"end",
	},
	{
		"fallen angel - Copper Wing.sad",
		"end",
	},
	{
		"rogue - Dart.sad",
		"rogue - Dagger.sad",
		"rogue - Handed Dagger.sad",
		"rogue - Boomerang.sad",
		"rogue - Bola.sad",
		"rogue - Tomahawk.sad",
		"rogue - Chakram.sad",
		"end",
	},
	{"end"},	//	무도가용 무기
	{
		"lancer - Short Spear.sad",
		"lancer - Long Spear.sad",
		"lancer - Winged Spear.sad",
		"lancer - Pike.sad",
		"lancer - Lance.sad",
		"lancer - Trident.sad",
		"lancer - Fork.sad",
		"lancer - Halberd.sad",
		"lancer - Glaive.sad",
		"lancer - Javelin.sad",
		"end",
	},
	{
		"archer - Short Bow.sad",
		"archer - Long Bow.sad",
		"archer - Composite Bow.sad",
		"end",
	},
	{
		"tammer - Flute.sad",
		"tammer - Clarinet.sad",
		"tammer - Oboe.sad",
		"tammer - Recorder.sad",
		"tammer - Grass Harp.sad",
		"tammer - Bamboo Flute.sad",
		"end",
	},
	{
		"useSameWeapon",
		"useSameWeapon",
		"useSameWeapon",
		"useSameWeapon",
		"useSameWeapon",
		"useSameWeapon",
		"end"},	//	소환사용 무기
	{
		"princess - Sling.sad",
		"princess - Chained Sling.sad",
		"end",
	},
	{
		"magical girl - Rod.sad",
		"magical girl - Baton.sad",
		"magical girl - Wand.sad",
		"magical girl - Mike Rod.sad",
		"magical girl - Magical Rod.sad",
		"#05",
		"end",
	},
	{"end"},	//	강신술사용 무기
	{
		"devil - Leather Whip.sad",
		"devil - Chained Whip.sad",
		"devil - Flame Whip.sad",
		"end",
	},
	{
		"SoulBringer - scythe.sad",			//  영술사 무기..
		"SoulBringer - Bird Scythe.sad",
		"SoulBringer - Crow Scythe.sad",
		"SoulBringer - Diamond Scythe.sad",
		"SoulBringer - Giant Scythe.sad",
		"SoulBringer - Hook Scythe.sad",
		"SoulBringer - Horse Slasher.sad",
		"SoulBringer - Mower.sad",
		"SoulBringer - Semicircle Scythe.sad",
		"SoulBringer - Sickle Spear.sad",
		"SoulBringer - Silver Scythe.sad",
		"SoulBringer - Steel Scythe.sad",
		"SoulBringer - Thick Scythe.sad",
		"end",
	},
	{// 투사 무기

		"Champion - Hit Gloves.sad",
		"Champion - Blade Hit Gloves.sad",
		"Champion - Iron Hit Gloves.sad",
		"Champion - Circle Blade Hit Gloves.sad",
		"Champion - Spear Blade Hit Gloves.sad",
		"Champion - Snake Form Hit Gloves.sad",
		"Champion - Hawk Form Hit Gloves.sad",
		"Champion - Bear Form Hit Gloves.sad",
		"Champion - Puma Form Hit Gloves.sad",
		"Champion - Spiral Hit Claw.sad",
		"Champion - Silver-Plate Hit Gloves.sad",
		"Champion - Gold-Plate Hit Gloves.sad",

		"end",
	},
	{// 광학사 무기
		"Opticalist - Ancient Book.sad",
		"Opticalist - Thick Book.sad",
		"Opticalist - Thin Book.sad",
		"Opticalist - Heavenly Book.sad",
		"Opticalist - Tartarean Book.sad",
		"Opticalist - Under Seal Book.sad",
		"Opticalist - Huge Book.sad",
		"Opticalist - Archaic Parchment.sad",
		"Opticalist - Woodblock.sad",
		"Opticalist - Slate.sad",
		"end",
	},
};


char	l_strShieldFileName[][20][40]	=
{
	{
		"knight - Buckler.sad",
		"knight - Large Shield.sad",
		"knight - Kite Shield.sad",
		"knight - Tower Shield.sad",
		"knight - Spiked Shield.sad",
		"knight - Main Gauche.sad",
		"end",
	},
	{"end"},	//	전사용 방패
	{"end"},	//	마법사용 방패
	{"end"},	//	늑대인간용 방패
	{
		"priest - Buckler.sad",
		"priest - Large Shield.sad",
		"priest - Kite Shield.sad",
		"priest - Tower Shield.sad",
		"priest - Spiked Shield.sad",
		"priest - Main Gauche.sad",
		"end",
	},
	{"end"},	//	타락천사용 방패
	{"end"},	//	도둑용 방패
	{"end"},	//	무도가용 방패
	{"end"},	//	궁수용 방패
	{"end"},	//	창병 방패
	{"end"},	//	조련사용 방패
	{"end"},	//	소환사용 방패
	{"end"},	//	공주용 방패
	{"end"},	//	변신소녀용 방패
	{"end"},	//	강신술사용 방패
	{"end"},	//	악마용 방패
	{"end"},	//  영술사용 방패
	{"end"},	//	투사용 방패
	{"end"},	//  광학사 방패
	{"end"},	//	수인 방패
};

char	*l_strHeroBody[]	=
{
	"Knight01"					,
	"Knight02"					,
	"Knight03"					,
	"Warrior01"					,
	"Warrior02"					,
	"Warrior03"					,
	"Wizard01"					,
	"Wizard02"					,
	"Wizard03"					,
	"Werewolf01"				,
	"Werewolf02"				,
	"Werewolf03"				,
	"Priest01"					,
	"Priest02"					,
	"Priest03"					,
	"FallenAngel01"				,
	"FallenAngel02"				,
	"FallenAngel03"				,
	"Rogue01"					,
	"Rogue02"					,
	"Rogue03"					,
	"null"						,
	"null"						,
	"null"						,
	"Lancer01"					,
	"Lancer02"					,
	"Lancer03"					,
	"Archer01"					,
	"Archer02"					,
	"Archer03"					,
	"BeastTamer01"				,
	"null"						,
	"BeastTamer03"				,
	"null"							,
	"null"							,
	"null"							,
	"Princess01"				,
	"Princess02"				,
	"Princess03"				,
	"MagicalGirl01"				,
	"MagicalGirl02"				,
	"MagicalGirl03"				,
	"NecroMancer01"				,
	"NecroMancer02"				,
	"NecroMancer03"				,
	"Devil01"					,
	"Devil02"					,
	"Devil03"					,
	"SoulBringer01"				,
	"SoulBringer02"					,
	"SoulBringer03"					,
	"Champion01"					,
	"Champion02"					,
	"Champion03"					,
	"Opticalist01"					,
	"Opticalist02"					,
	"Opticalist03"					,
	"BeastMan01"					,
	"BeastMan02"					,
	"BeastMan03"					,
	"end"							,
};

char *l_strMonsterFileName[]	=
{
	"SkeletonAxe.sad",		//	언데드	해골전사 도끼	Skeleton, Axe		175㎝		기존
	"SkeletonSword.sad",	//	언데드	해골전사 쌍칼	Skeleton, Sword		175㎝		기존
	"SkeletonKnight.sad",	//	언데드	해골기사	Skeleton Knight		190㎝		기존
	"Zombie.sad",			//	언데드	좀비	Zombie		170㎝		기존
	"LivingDead.sad",		//	언데드	산송장	Living Dead		155㎝		기존
	"Ghost.sad",			//	언데드	유령	Ghost		180㎝		기존
	"Mummy.sad",			//	언데드	미이라	Mummy		200㎝		기존
	"GhostArmor.sad",		//	언데드	고스트 아머	Ghost Armor		250㎝		기존
	"Vampire.sad",			//	언데드	뱀파이어	Vampire		180㎝	흡혈귀, 세미보스다운 귀족적 스타일	세미보스
	"Lich.sad",				//	언데드	리치	Lich		210㎝	공중 부양, 허리 밑은 없음. 머리부터 허리까지는 100㎝	보스
	"DarkElf.sad",			//	인간형	블랙 엘프	Black Elf		160㎝		기존
	"Assassin.sad",			//	인간형	어새신	Assassin		165㎝	도적 스타일	기존
	"AggressiveNative.sad",	//	인간형	원주민			80㎝	부쉬맨 스타일의 작은 원주민, 무기는 몽둥이	
	"FallenWizard.sad",		//	인간형	타락한 마법사	Fallen Wizard		145㎝	구부정한 곱사등 스타일, 구불구불한 스태프 소지	
	"TempleKnight.sad",		//	인간형	궁정 기사단	Temple Knight		195㎝	장신, 멋진 풀 플레이트 메일에 할버드	
	"DarkPriest.sad",		//	인간형	다크 프리스트	Dark Priest		160㎝	회복과 보조 마법 전문의 성직자	
	"Conjuerer.sad",		//	인간형	어둠의 소환사	Conjuerer		165㎝	채찍으로 무장	
	"Giant.sad",			//	인간형	자이언트	Giant		230㎝		세미 보스, 기존
	"ElfKing.sad",			//	인간형	엘프 킹	Elf King		175㎝	칼과 롱보우 소지	보스
	"Archmage.sad",			//	인간형	대마술사	Archmage		180㎝	장신의 마법사, 레드아이 보스로 등장 예정	보스
	"DemiBeholder.sad",		//	악마형	데미 비홀더	Demi Beholder		90㎝		기존
	"Reptile.sad",			//	악마형	렙타일	Reptile		120㎝		기존
	"CockFighter.sad",		//	악마형	닭 전사	Cock Fighter		175㎝		기존
	"LizardWarrior.sad",	//	악마형	리자드 워리어	Lizard Warrior		155㎝		기존
	"LizardRider.sad",		//	악마형	리자드 라이더	Lizard Rider		180㎝		기존
	"RatFighter.sad",		//	악마형	랫 파이터	Rat Fighter		140㎝		기존
	"Demon.sad",			//	악마형	데몬	Demon		185㎝		세미 보스, 기존
	"Ogre.sad",				//	악마형	오우거	Ogre		210㎝		세미 보스, 기존
	"Bahomate.sad",			//	악마형	바호매트			210㎝	염소 파충류라는 것.	세미 보스, 기존
	"RedDemon.sad",			//	악마형	붉은 악마	Red Demon		300㎝	최종 보스 다운 거대함과 위용을 보여줄 것	최종 보스
	"TurtleDragon.sad",		//	동물형	터틀 드래곤	Turtle Dragon		190㎝		기존
	"Spider.sad",			//	동물형	거미	Spider		130㎝		기존
	"Scorpion.sad",			//	동물형	전갈	Scorpion		180㎝		기존
	"JellyFish.sad",		//	동물형	해파리			170㎝		기존
	"GiantWorm.sad",		//	동물형	웜	Worm		170㎝(길이)		기존
	"KingCrab.sad",			//	동물형	킹 크랩	King Crab		50㎝	키는 작지만 그 넓이까지 합치면 꽤 크다. 옆으로 걸을 것	
	"InsectSwarm.sad",		//	동물형	곤충 떼	Insect Swarm		120㎝	곤충 떼. 점으로만 이루어져도 될 듯	
	"Wolf.sad",				//	동물형	늑대	Wolf		50㎝	길이는 150㎝~170㎝ 정도 된다. 거대한 늑대	
	"BigMole.sad",			//	동물형	큰 두더지	Big Moul		400㎝(팔길이)		세미보스, 기존
	"MadBear.sad",			//	동물형	미친 곰	Mad Bear		250㎝(길이)	이동시에는 네 발로 걷고 공격할 때는 서서 앞 발로.	세미보스
	"FireBogy.sad",			//	신수형	불도깨비			150㎝	불의 신수	기존
	"TongueEye.sad",		//	신수형	혓바닥눈			200㎝	물의 신수	기존
	"Gargoyle.sad",			//	신수형	가고일	Gargoyle		155㎝	바람의 신수	기존
	"MetalGolem.sad",		//	신수형	메탈 골렘	Metal Golem		220㎝	땅의 신수	기존
	"Centaurs.sad",			//	신수형	켄타우로스	Centaurs		210㎝	바람의 신수	기존
	"MermanKnight.sad",		//	신수형	머맨 나이트	Merman Knight		220㎝	물의 신수	기존
	"TimberMan.sad",		//	신수형	팀버맨	Timber Man		230㎝	땅의 신수	기존
	"Salamander.sad",		//	신수형	사라만다	Salamander		100㎝	불의 신수, 불 도마뱀	
	"DarkFire.sad",			//	신수형	다크 파이어	Dark Fire		120㎝	어둠의 신수, 검은 불로 둘러싸인 괴물 분위기	세미 보스
	"WhiteShadow.sad",		//	신수형	화이트 섀도우	White Shadow		140㎝	빛의 신수, 몸집이 작은 어린 아이 같은 분위기	보스
	"SkeletonSanta.sad",

	"sage.sad",
	"sage_master.sad",
	"tiamath_left_feeler.sad",
	"tiamath.sad",
	"tiamath_right_feeler.sad",
	"water_dragon.sad",
	"arch_devil.sad",
	"draco_lich.sad",
	"draco_lich_orb.sad",

	"Giant_Skull.sad",
	"Undead_Magician.sad",
	"Bone_Chimera.sad",
	"High_Elf.sad",
	"Gangster.sad",
	"Sikhs.sad",
	"Escapee.sad",
	"Mad_Demon.sad",
	"God_of_Death.sad",
	"Succubus.sad",
	"Mantis_Warrior.sad",
	"Pumpkin_Head.sad",
	"Monkey_King.sad",
	"Giant_Frog.sad",
	"Tongue_Monster.sad",
	"Dark_Buffalo.sad",
	"Unicorn.sad",
	"Apostles.sad",
	"Cherubim.sad",
	"guild_crest.sad",
	"shield_craft.sad",
	"elemantal_post.sad",
	"ether_shell.sad",
	"hollow_knight.sad",
	"guild_flag.sad",

	"end",
};

//	Npc 몽뚱아리 화일 이름
char *l_strNpcFileName[]	=
{
	"MAN1.sad",					//	아저씨 (평민) 1				평범한 결혼한 남자 스타일. 범용.	기존
	"MAN2.sad",					//	아저씨 (평민) 2				평범한 결혼한 남자 스타일. 옷 모양이 좀 다르다. 범용.	
	"WOMAN1.sad",				//	부인 (평민) 1				평범한 결혼한 여자. 범용. 옷 모양이 좀 다름	기존
	"WOMAN2.sad",				//	부인 (평민) 2				평범한 결혼한 여자. 범용. 옷 모양이 좀 다름	
	"YOUNG_MAN1.sad",			//	청년 1 (평민)				결혼 안 한 청년	기존
	"YOUNG_MAN2.sad",			//	청년 2 (평민)				결혼 안 한 청년	기존
	"LADY1.sad",				//	아가씨 (평민) 1				결혼 안 한 처녀.	기존
	"LADY2.sad",				//	아가씨 (평민) 2				결혼 안 한 처녀. 옷이 다름	
	"GRAND_FATHER.sad",			//	할아버지 (평민)				노인네.	기존
	"GRAND_MOTHER.sad",			//	할머지 (평민)				나이든 여인.	
	"BOY1.sad",					//	남아 (평민) 1				남자 아이. 중산층	기존
	"BOY2.sad",					//	남아 (평민) 2				남자 아이. 중산층	
	"GIRL1.sad",				//	여아 (평민) 1				여자 아이. 중산층	
	"GIRL2.sad",				//	여아 (평민) 2				여자 아이. 중산층	
	"NOBLE_MAN1.sad",			//	귀족 남자 1				귀족 남자. 나이가 좀 들었음	기존
	"NOBLE_MAN2.sad",			//	귀족 남자 2				귀족 남자. 젋은 나이	기존
	"NOBLE_WOMAN1.sad",			//	귀족 부인 1				우아한 귀족 부인	기존
	"NOBLE_WOMAN2.sad",			//	귀족 부인 2				우아한 귀족 부인. 나이가 약간 들었음.	
	"NOBLE_BOY.sad",			//	남아 (귀족)				귀족 자녀. 잘 입었음	기존
	"NOBLE_GIRL.sad",			//	여아 (귀족)				귀족 자녀. 잘 입었음	
	"GYPSY_MAN1.sad",			//	집시 (남) 1				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
	"GYPSY_MAN2.sad",			//	집시 (남) 2				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
	"GYPSY_WOMAN1.sad",			//	집시 (여) 1				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
	"GYPSY_WOMAN2.sad",			//	집시 (여) 2				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
	"GYPSY_BOY1.sad",			//	집시 (남아) 1				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
	"GYPSY_BOY2.sad",			//	집시 (남아) 2				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
	"GYPSY_GIRL1.sad",			//	집시 (여아) 1				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
	"GYPSY_GIRL2.sad",			//	집시 (여아) 2				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
	"FARMER1.sad",				//	농부 1				밀짚 모자에 쟁기를 든 모습	
	"FARMER2.sad",				//	농부 2				팔을 걷어 붙이고 있는 농부의 모습	
	"WOODCUTTER1.sad",			//	나무꾼 1				도끼든 모습	
	"WOODCUTTER2.sad",			//	나무꾼 2				나무짐을 지고 있음	
	"MINER.sad",				//	광부					기존
	"SAILER.sad",				//	선원					기존
	"HALBERD_MAN.sad",			//	도끼창병					기존
	"TEMPLE_KNIGHT_M.sad",		//	성기사 (남)					기존
	"TEMPLE_KNIGHT_F.sad",		//	성기사 (여)					기존
	"BISHOP.sad",				//	주교					기존
	"PRIEST.sad",				//	신부					기존
	"NUN.sad",					//	수녀					기존
	"MERCENARY_SOLDIER_M.sad",	//	용병 (남)					기존
	"MERCENARY_SOLDIER_F.sad",	//	용병 (여)					기존
	"BAGGER.sad",				//	거지					기존
	"DANCING_GIRL.sad",			//	사막 무희 (여자)					기존
	"INSTRUCTOR_M.sad",			//	사관학교 남교관					
	"INSTRUCTOR_F.sad",			//	사관학교 여교관					기존
	"CADET1.sad",				//	사관 생도 1					기존
	"CADET2.sad",				//	사관 생도 2					
	"WIZARD.sad",				//	마법사 (남자 노인)				뾰족 모자 쓴 전형적인 노마법사, 레드 아이 단원 겸용	
	"MAGICIAN_M.sad",			//	마법사 (남자)				젊은 마법사, 레드 아이 단원 겸용	
	"MAGICIAN_F.sad",			//	마법사 (여자)				젊은 마법사, 좀 노출이 있게, 레드 아이 단원 겸용	
	"SHOPKEEPER_WEAPON.sad",	//	무기점 주인 (남)				우락부락한 대장장이 느낌	
	"GROCER_M.sad",				//	잡화점 주인 (남)				전형적인 상점 주인	
	"GROCER_F.sad",				//	잡화점 주인 (여)				전형적인 상점 주인	
	"PITCHMAN_M.sad",			//	노점상 주인 (남)					
	"PITCHMAN_F.sad",			//	노점상 주인 (여)					
	"PITCHMAN_GRAND_FATHER.sad",//	노점상 주인 (남노)					
	"PITCHMAN_GRAND_MOTHER.sad",//	노점상 주인 (여노)					
	"ANGEL.sad",				//	천사					
	"end",
};

char *l_strMinipetFileName[]	=
{
	"SpritOfFire.sad",
	"SpritOfWater.sad",
	"SpritOfWind.sad",
	"SpritOfEarth.sad",
	"SpritOfLight.sad",
	"SpritOfDark.sad",

	"Salamander.sad",
	"Undine.sad",
	"Silf.sad",
	"Nom.sad",
	"Wisp.sad",
	"Shadow.sad",

	"Hwa.sad",
	"Soo.sad",
	"Poong.sad",
	"Ji.sad",
	"Vit.sad",
	"Am.sad",

	"Haoring.sad",
	"Mioring.sad",
	"Sooring.sad",
	"Daoring.sad",
	"Chaoring.sad",
	"Goring.sad",
};

CDecoderKeyword	l_aEffectKeyword[]	=
{
	{	"qanm"		,1},
	{	"anm"		,2},
	{	"end"		,dCODE_END}
};

inline	cANM*
_GetBody(int _iBody)
{
//	return	(cANM *)g_apBody[_iBody];
	return	(cANM *)g_alpBody[_iBody];
}

inline	cANM*
_GetMonsterVariation(int _iBody,int _iVariation)
{
	return	(cANM *)g_apMonsterVariation[_iBody][_iVariation];
}

inline	cANM*
GetBody(int _iBody)
{
	if	(g_alpBody[_iBody] && g_alpBody[_iBody]->m_pAnmData)
		return	(cANM *)g_alpBody[_iBody];

//	if	(g_apBody[_iBody] && g_apBody[_iBody]->m_pAnmData)
//		return	(cANM *)g_apBody[_iBody];

	return	g_apIBody[_iBody];
}

inline	cANM*
GetMonsterVariation(int _iBody,int _iVariation,int _iParts)
{
	if	(g_apMonsterVariation[_iBody][_iVariation] && g_apMonsterVariation[_iBody][_iVariation]->m_pAnmData)
		return	(cANM *)g_apMonsterVariation[_iBody][_iVariation];

	return	g_apIMonsterVariation[_iBody][_iVariation];
}

void
CImageManager::resetIMonsterBody()//	임시 몬스터 이미지 초기화
{
	int		i,j;

	for (i=0;i<dJOB_IMAGE_DATA_COUNT;i++)
	{
		KILL(g_apIBody[i]);
	}

	for (i=0;i<dJOB_IMAGE_DATA_COUNT;i++)
		for (j=0;j<dMONSTER_VARIATION_COUNT;j++)
			KILL(g_apIMonsterVariation[i][j]);
}

CImageManager::CImageManager()
{
	int		i,j;

	m_iLoadedBodyCount			=	0;	//	현재 맵에서 불러온 몬스터/NPC 수
	m_iEffectCount			=	0;	//	이펙트 수
	m_pPalette				=	NULL;
	m_wPaletteCount			=	0;
	m_pEfectInfo			=	NULL;
	m_bIsSlimMemoryMode		=	FALSE;
	m_bIsSuperSlimMemoryMode=	FALSE;

	for (i=0;i<dEFFECT_IMAGE_DATA_COUNT;i++)
		g_apEffect[i]	=	NULL;

	for (i=0;i<dPLAYER_JOB_COUNT;i++)
		for (j=0;j<dPLAYER_JOB_COUNT;j++)
			g_apWeapon[i][j]	=	NULL;

	for (i=0;i<dJOB_IMAGE_DATA_COUNT;i++)
	{
		g_apBody[i]	=	NULL;
		g_apIBody[i]=	NULL;
		g_alpBody[i]=	NULL;
	}

	for (i=0;i<dJOB_IMAGE_DATA_COUNT;i++)
		for (j=0;j<dMONSTER_VARIATION_COUNT;j++)
		{
			g_apMonsterVariation[i][j]	=	NULL;
			g_apIMonsterVariation[i][j]	=	NULL;
		}

	for (i=dBODY_HERO_JOB_START;i<dBODY_HERO_JOB_END;i++)
		g_apBody[i]	=	NULL;

	for (i=dBODY_KELBY_1;i<=dBODY_HEDGER_3;i++)
		g_apBody[i]	=	NULL;

	for (i=dBODY_OPERATOR_ANGEL_MALE;i<=dBODY_OPERATOR_ANGEL_FEMALE;i++)
	{
		g_apBody[i]	=	new cANMQ();
		g_alpBody[i]=	g_apBody[i];
	}

	for (i=dJOB_NPC_START;i<dJOB_NPC_END;i++)
	{
		g_apBody[i]	=	new cANMQ();
		g_alpBody[i]=	g_apBody[i];
	}

	for (i=dJOB_NPC_FAT_GIRL;i<=dJOB_NPC_RABBIT;i++)
	{
//		g_apBody[i]	=	&g_aTransNPC[i-dJOB_NPC_FAT_GIRL];
		g_alpBody[i]=	&g_aTransNPC[i-dJOB_NPC_FAT_GIRL];
	}
}

CImageManager::~CImageManager()
{
}

BOOL
CImageManager::convertImageData051116()
{
	cANM	convertor;
	int		i;
	
	{
		SetCurrentDirectory("data/interface/game");
		
		for (i=0;i<dMAX_MAGIC_CARPET_SHAPE_COUNT;i++)
			if	(convertor.convert051115(_ms("magic_carpet%.2d.sad",i)))
				break;

		if	(!convertor.convert051115("gateAnm.sad"))
			return	ERRMSG(dMSG_ERROR,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"gateAnm.sad");
		if	(!convertor.convert051115("effect_game_manager.sad"))
			return	ERRMSG(dMSG_ERROR,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"effect_game_manager.sad");

		SetLogFolder();
	}

	{
		cFILE	file;
		int		i;

		if	(!file.Open("data/effect_data.dat","rb"))
			return	ERRMSG(dMSG_ERROR,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"data/effect_data.dat");

		SetLogFolder();
		SetCurrentDirectory("data/Effects");

		file.Read(&i,4);
		file.Read(this,sizeof(CClientSaveImageData));

		for (i=0;i<m_iEffectCount;i++)
		{
			CEffectDataInfo	effectInfo;

			file.Read(&effectInfo,sizeof(CEffectDataInfo));

			if	(!convertor.convert051115(effectInfo.m_strImageFileName))
				return	ERRMSG("error in CImageManager::loadEffectByFile",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,effectInfo.m_strImageFileName);
		}
		file.Close();
		SetLogFolder();
	}
	{
		SetCurrentDirectory("data/equipment");

		int	iJob,iEquipment;

		for (iJob=0;iJob<dPLAYER_JOB_COUNT;iJob++)
		{
			iEquipment	=	0;

			for(iEquipment=0;;iEquipment++)
			{
				if (strcmp(l_strWeaponFileName[iJob][iEquipment],"end")	==	0)	
					break;

				if (strcmp(l_strWeaponFileName[iJob][iEquipment],"useSameWeapon")	==	0)	
					continue;
				
				convertor.convert051115(l_strWeaponFileName[iJob][iEquipment]);
			}

			for(iEquipment=0;;iEquipment++)
			{
				if	(strcmp(l_strShieldFileName[iJob][iEquipment],"end")	==	0)
					break;

				convertor.convert051115(l_strShieldFileName[iJob][iEquipment]);
			}
		}

		SetLogFolder();
	}

//	주인공 데이터 로딩
	{
		SetCurrentDirectory("data/heros");

		for (i=0;;i++)
		{
			if (STRICMP(l_strHeroBody[i],"end")	==	0)
				break;
			if (STRICMP(l_strHeroBody[i],"null")==	0)
				continue;

			if	(!convertor.convert051115(_ms("%s.sad",l_strHeroBody[i])))
			{
				SetLogFolder();
				return	ERRMSG(dMSG_ERROR,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,l_strHeroBody[i]);
			}
		}
		SetLogFolder();
	}	//	주인공 데이터 로딩

	{
		SetCurrentDirectory(_ms("%s/data/Monsters",g_strLogFolder));

		for (i=0;;i++)
		{
			if	(STRICMP(l_strMonsterFileName[i],"end") == 0)
				break;

			if	(!convertor.convert051115(l_strMonsterFileName[i]))
				return	ERRMSG(dMSG_ERROR,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,l_strMonsterFileName[i]);
		}

		SetLogFolder();
	}

	{
		SetCurrentDirectory(_ms("%s/data/NPC",g_strLogFolder));

		for (i=0;;i++)
		{
			if	(STRICMP(l_strNpcFileName[i],"end")	==	0)
				break;
			
			if	(!convertor.convert051115(l_strNpcFileName[i]))
				return	ERRMSG(dMSG_ERROR,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,l_strNpcFileName[i]);
		}

		SetLogFolder();
	}

	return	TRUE;
}

//
//	초기화
BOOL
CImageManager::init(cNUX *_lpNux,cMCU *_lpMcu)
{
	int i;

	if	(isSuperSlimMemoryMode())
	{
		for (i=dBODY_HERO_JOB_START;i<dBODY_HERO_JOB_END;i++)
		{
			g_apBody[i]	=	new cANMQ();
			g_alpBody[i]=	g_apBody[i];
		}

		for (i=dBODY_KELBY_1;i<=dBODY_HEDGER_3;i++)
		{
			g_apBody[i]	=	new cANMQ();
			g_alpBody[i]=	g_apBody[i];
		}
	}
	else
	{
		for (i=dBODY_HERO_JOB_START;i<dBODY_HERO_JOB_END;i++)
		{
			g_apBody[i]	=	new cANM();
			g_alpBody[i]=	g_apBody[i];
		}
		for (i=dBODY_KELBY_1;i<=dBODY_HEDGER_3;i++)
		{
			g_apBody[i]	=	new cANM();
			g_alpBody[i]=	g_apBody[i];
		}
	}

	memset(m_abImageOutputMethod,0,sizeof(m_abImageOutputMethod));

	{
		char	*astrGuildPetFileName[]	=	
		{
			"data/npc/obitian.sad",
			"data/npc/dash_blade.sad",
			"data/npc/astro_bow.sad",
			"data/npc/cristal_water.sad",
		};

		for	(i=0;i<dGUILD_PET_TYPE_COUNT;i++)
			if	(!g_aGuildPet[i].Load(astrGuildPetFileName[i],TRUE,_lpNux,_lpMcu)) 
			{
				g_eh.addStaticLog(dMSG_CAN_NOT_FIND_FOLLOW_FILE,astrGuildPetFileName[i]);

				return	FALSE;
			}
	}

	{
		for	(i=0;i<nsMiniPetType::Count;i++)
			if	(!g_aMiniPetBody[i].Load(_ms("data/miniPet/%s",l_strMinipetFileName[i]),TRUE,_lpNux,_lpMcu)) 
			{
				g_eh.addStaticLog(dMSG_CAN_NOT_FIND_FOLLOW_FILE,l_strMinipetFileName[i]);

				return	FALSE;
			}
	}

	{
		char	*astrDuelBuffFileName[]	=	
		{
			"data/effects/duel_buff_fire.sad",
			"data/effects/duel_buff_water.sad",
			"data/effects/duel_buff_wind.sad",
			"data/effects/duel_buff_earth.sad",
		};

		for (int i=0;i<4;i++)
		{
			if	(!g_aDuelBuff[i].Load(astrDuelBuffFileName[i],TRUE,_lpNux,_lpMcu)) 
				return FALSE;
		}

		char	*astrGGGBuffFileName[]	=	
		{
			"data/effects/GGG_skillbuff_active_fire.sad",
			"data/effects/GGG_skillbuff_active_water.sad",
			"data/effects/GGG_skillbuff_active_wind.sad",
			"data/effects/GGG_skillbuff_active_earth.sad",
		};
		
		for (i=0;i<4;i++)
		{
			if	(!g_aGGGBuff[i].Load(astrGGGBuffFileName[i],TRUE,_lpNux,_lpMcu)) 
				return FALSE;
		}
	}

	if (!g_anmEtc.Load("data/interface/game/etc_anm.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if (!g_anmHelp.Load("data/interface/game/help.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if (!g_anmGate.Load("data/interface/game/gateAnm.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if (!g_anmNpcType.Load("data/interface/game/shopIcon.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if (!g_anmQuestNpcMark.Load("data/interface/game/questNpcMark.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if (!g_anmJewel.Load("data/interface/game/jewel.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if (!g_anmItemType.Load("data/interface/game/itemType.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if (!g_anmGuildFlag.Load("data/interface/game/guild_flag.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if	(!g_anmGuildMarkGlow.Load("data/interface/game/guild_mark_glow.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if	(!g_anmMagicalShield.Load("data/effects/magical_shield.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	// 비   10.02.10 추가	
	if	(!g_anmBombRain.Load("data/interface/game/effect_rain1.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if	(!g_anmLongRain.Load("data/interface/game/effect_rain2.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if	(!g_anmShortRain.Load("data/interface/game/effect_rain3.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	for (i=0;i<dMAX_MAGIC_CARPET_SHAPE_COUNT;i++)
		if	(!g_aAnmMagicCarpet[i].Load(_ms("data/interface/game/magic_carpet%.2d.sad",i),TRUE,_lpNux,_lpMcu))
			break;

	if	(!g_anmGuildBattlePortal.Load("data/interface/game/portal_guild_battle.sad",TRUE,_lpNux,_lpMcu)) 
		return FALSE;

#ifdef	_FOR_THAI
	if	(!g_anmGameManager.Load("data/interface/game/effect_game_master.sad",TRUE,_lpNux,_lpMcu)) 
#else
		if	(!g_anmGameManager.Load("data/interface/game/effect_game_manager.sad",TRUE,_lpNux,_lpMcu)) 
#endif
			return FALSE;

	if	(!g_sprNameBar.Load("data/interface/game/name_bar.sd",TRUE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprGameBottomInterface.Load("data/interface/game/bottom.sd",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if	(!g_sprBook.Load("data/interface/game/book.sd",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if	(!g_sprFieldItem.Load("data/misc/ItemFieldShape.sd",TRUE,_lpNux,_lpMcu)) 
		return FALSE;
	if	(!g_sprHitText.Load("data/interface/hitText.sd",TRUE,_lpNux,_lpMcu)) 
		return FALSE;

	if	(!g_smiIconItem.load("data/misc/iconItem.smi",_lpNux,_lpMcu)) 
		return FALSE;	//	아이템 아이콘
	if	(!g_smiIconSkill.load("data/misc/iconSkill.smi",_lpNux,_lpMcu)) 
		return FALSE;	//	아이템 아이콘
	if	(!g_smiIconGuildSkill.load("data/misc/iconGuildSkill.smi",_lpNux,_lpMcu)) 
		return FALSE;	//	아이템 아이콘
	if	(!g_smiIconGGG.load("data/misc/iconGGG.smi",_lpNux,_lpMcu)) 
		return FALSE;	//	GGG 아이콘
	if	(!g_smiLoading.iLoad("data/misc/loading.smi",_lpNux,_lpMcu)) 
		return FALSE;	//	아이템 아이콘
	// 10.02.18 Title
	if	(!g_sprTitle.Load("data/interface/game/designation_icon.sd",TRUE,_lpNux,_lpMcu))
		return FALSE;	//	타이틀 아이콘

	if	(!g_guildMarkPalette.init())	
		return	FALSE;


	for (i=0;i<dGUILD_MARK_ICON_SHAPE_PATTERN_COUNT;i++)
	{
		char	*lpstrFileName	=	_ms("data/interface/game/guild_icon_shape%.2d.sd",i);

		if	(!IsFile(lpstrFileName))
			break;

		if	(!g_sprGuildIconShape[i].Load(lpstrFileName,TRUE,_lpNux,_lpMcu))
			return	FALSE;
	}

	if	(!g_sprGuildHallLevel.Load("data/interface/game/guild_hall_level.sd",FALSE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprGuildIconOutLine.Load("data/interface/game/guild_icon_outline.sd",FALSE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprGuildIconBaseShape.Load("data/interface/game/guild_icon_base_shape.sd",FALSE,_lpNux,_lpMcu))
		return FALSE;

	for	(i=0;i<c_iPitchmanShopShapeCount;i++)
		if	(!g_asprPitchmanShop[i].Load(_ms("data/interface/game/personal_shop_%.2d.sd",i),TRUE,_lpNux,_lpMcu))
			return	FALSE;

	if	(!g_sprMonsterDiscerner.Load("data/interface/game/monsterDiscerner.sd",FALSE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprWorldMap.Load("data/interface/game/worldMap.sd",FALSE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprArca.Load("data/misc/arca.sd",TRUE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprBanner.Load("data/misc/banner.sd",TRUE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprMiniPetIcon.Load("data/interface/game/miniPetIcon.sd",TRUE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprLoadingPannel.Load("data/interface/game/loading.sd",FALSE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_sprMainQuestEnding.Load("data/interface/game/main_quest_ending.sd",FALSE,_lpNux,_lpMcu))
		return FALSE;
	if	(!g_smiGuildIconBaseColorPartition.load("data/interface/game/guild_icon_base_color_partition.smi",_lpNux,_lpMcu)) 
		return FALSE;	//	아이템 아이콘

//	이미지 데이터 로딩
	if	(!loadEffect("dev data/scripter/effect_file.txt",_lpNux,_lpMcu))
	{
		SetLogFolder();

		return	FALSE;
	}//	이미지 데이터 로딩

//	직업 데이터 로딩
	if	(!loadHeroImageData(_lpNux,_lpMcu))
	{
		SetLogFolder();

		return	FALSE;
	}//	직업 데이터 로딩

//	직업 데이터 로딩
	if	(!loadStaticMonsterImageData(_lpNux,_lpMcu))
	{
		SetLogFolder();

		return	FALSE;
	}//	직업 데이터 로딩

	{
		cFILE	file;

		if	(!file.Open("data/interface/game/red_drake.plt","rb"))
		{
			SetLogFolder();

			return	FALSE;
		}

		g_pRedDrakePalette	= new WORD [256];

		file.Read(g_pRedDrakePalette,256*2);

		if	(g_iPixelFormat != dRGB565)
			for (i=0;i<256;i++)	
				g_pRedDrakePalette[i]	=	RGB565ToRGB555(g_pRedDrakePalette[i]);
	}

	{
		g_pRebirthPalette		=	new WORD[256*5];
		g_pRebirthBallPalette	=	new WORD[256*5];

		for (int i=0;i<5;i++)
		{
			cFILE	file;

			if	(!file.Open(_ms("data/effects/rebirth%.2d.plt",i+1),"rb"))
			{
				SetLogFolder();

				return	cMSG::Error("loading error!!","can not find rebirth palette");
			}

			file.Read(&g_pRebirthPalette[i*256],256*sizeof(WORD));
			file.Close();

			if	(!file.Open(_ms("data/effects/rebirth_ball_%.2d.plt",i+1),"rb"))
			{
				SetLogFolder();

				return	cMSG::Error("loading error!!","can not find rebirth palette");
			}

			file.Read(&g_pRebirthBallPalette[i*256],256*sizeof(WORD));
			file.Close();
		}

		if	(g_iPixelFormat != dRGB565)
			for (i=0;i<256*5;i++)	
			{
				g_pRebirthPalette[i]	=	RGB565ToRGB555(g_pRebirthPalette[i]);
				g_pRebirthBallPalette[i]=	RGB565ToRGB555(g_pRebirthBallPalette[i]);
			}
	}

	if	(!loadPalette(_lpNux,_lpMcu))
	{
		SetLogFolder();

		return	FALSE;
	}//	직업 데이터 로딩

//	이미지 데이터 로딩
	if	(!loadPalette("data/effects/effect.plt",_lpNux,_lpMcu))
	{
		SetLogFolder();

		return	FALSE;
	}//	이미지 데이터 로딩

//	장비 데이터 로딩
	if	(!loadEquipmentImageData(_lpNux,_lpMcu))
	{
		SetLogFolder();

		return	FALSE;
	}//	직업 데이터 로딩

	return TRUE;
}

//
//	다기
void
CImageManager::close()
{
	g_sprFieldItem.close();
	g_sprNameBar.close();
	g_sprGameBottomInterface.close();

	int i;

	for (i=0;i<m_iEffectCount;i++)
	{
		KILL(g_apEffect[i]);
	}

	for (int iJob=0;iJob<dPLAYER_JOB_COUNT;iJob++)
	{
		int iEquipment	=	0;

		for(iEquipment=0;;iEquipment++)
		{
			if	(strcmp(l_strWeaponFileName[iJob][iEquipment],"end")	==	0)	
				break;

			if	(strcmp(l_strWeaponFileName[iJob][iEquipment],"useSameWeapon")	==	0)	
				continue;
			if	(strcmp(l_strWeaponFileName[iJob][iEquipment],"#05")	==	0)	
				continue;

			KILL(g_apWeapon[iJob][iEquipment]);
		}
	}

	for (i=0;i<dJOB_IMAGE_DATA_COUNT;i++)
		KILL(g_apBody[i]);

	for (i=0;;i++)
	{
		if	(STRICMP(l_strMonsterFileName[i],"end") == 0)
			break;

		if	(isSlimMemoryMode() || g_aMonsterBodyLoadInfo[i].m_wLoadMethodValue	>	eANM_LM_ALWAYS)
			continue;

		int		iJob	=	dJOB_MONSTER_START+i;

		for(int j=0;j<dMONSTER_VARIATION_COUNT;j++)
		{
			KILL(g_apMonsterVariation[iJob][j]);
		}
	}

	resetIMonsterBody();

	pKILL(m_pPalette);
	pKILL(m_pEfectInfo);
	pKILL(g_pRedDrakePalette);
	pKILL(g_pRebirthPalette);
	pKILL(g_pRebirthBallPalette);
}

//
//	
BOOL
CImageManager::loadPalette(char *fn,cNUX *p_nux,cMCU *p_mcu)
{
	int		i;

	cFILE	file;

	if	(!file.Open(fn,"rb"))
		return	FALSE;

	m_wPaletteCount	=	(WORD)file.Length / (256*5);

	if	((DWORD)m_wPaletteCount * 256*5 != file.Length)
		return	FALSE;

	m_pPalette		=	new WORD [m_wPaletteCount*256];

	for (i=0;i<m_wPaletteCount;i++)
	{
		file.Read(&m_pPalette[i*256],512);
		file.Seek(256*3,SEEK_CUR);
	}

	file.Close();

	if	(g_iPixelFormat != dRGB565)
		for (i=0;i<m_wPaletteCount*256;i++)	
			m_pPalette[i]	=	RGB565ToRGB555(m_pPalette[i]);

	return	TRUE;
}

//
//	이펙트 데이터 로딩
BOOL
CImageManager::loadEffectByFile(char *_lpstrFileName,cNUX *_lpNux,cMCU *_lpMcu)
{
	cFILE	file;
	int		i;

	char	*lpstrFileName	=	"data/effect_data.dat";
// 
// #ifdef	_FOR_JAPAN
// 	cMessSign	messSign;
// 
// 	lpstrFileName	=	messSign.makeDecodeFile(lpstrFileName);
// 
// #ifdef	_DEBUG
// 	if	(!messSign.m_iIsSuccess)
// 		Beep(4200,200);
// #endif
// 
// #endif

	if	(!file.Open(lpstrFileName,"rb"))
		return	ERRMSG(dMSG_ERROR,dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"data/effect_data.dat");

	SetLogFolder();
	SetCurrentDirectory("data/Effects");

	file.Read(&i,4);
	file.Read(this,sizeof(CClientSaveImageData));

	for (i=0;i<m_iEffectCount;i++)
	{
		CEffectDataInfo	effectInfo;

		file.Read(&effectInfo,sizeof(CEffectDataInfo));

		if	(effectInfo.m_wType	==	eEDF_QANM)
			g_apEffect[i]		=	new	cANMQ();

		if	(effectInfo.m_wType	==	eEDF_ANM)
			g_apEffect[i]		=	new	cANM();

		if	(!g_apEffect[i]->Load(effectInfo.m_strImageFileName,TRUE,_lpNux,_lpMcu))
			return	ERRMSG("error in CImageManager::loadEffectByFile",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,effectInfo.m_strImageFileName);
	}

	file.Close();

	SetLogFolder();

	return	TRUE;
}

//
//	효과 이미지들을 로딩한다.
BOOL
CImageManager::loadEffect(char *fn,cNUX *p_nux,cMCU *p_mcu)
{
#ifndef	_IS_DEV_CLIENT
	return	loadEffectByFile("effect_data.dat",p_nux,p_mcu);
#else

	CDecoder	clDecoder;	//	스크립트 디코더
	DWORD		code	=	0;
	char		*name,*keyword;

	if (!clDecoder.Upload(fn,l_aEffectKeyword))
		return	loadEffectByFile("effect_data.dat",p_nux,p_mcu);

	SetCurrentDirectory("data/Effects");

	m_iEffectCount	=	0;
	m_pEfectInfo	=	new CEffectDataInfo [dEFFECT_IMAGE_DATA_COUNT];

	while(code!=dCODE_END)
	{	
		keyword		=	clDecoder.getWord();
		code		=	clDecoder.analCode(keyword);

		if (code == 1 || code == 2)
		{
			keyword	=	clDecoder.getWord();

			if (STRICMP(keyword,"rape")==0)
				m_abImageOutputMethod[m_iEffectCount]	=	eEOM_RAPE;
			else
			if (STRICMP(keyword,"stand")==0)
				m_abImageOutputMethod[m_iEffectCount]	=	eEOM_STAND;
			else
			if (STRICMP(keyword,"float")==0)
				m_abImageOutputMethod[m_iEffectCount]	=	eEOM_FLOAT;
			else
				return	ERRMSG("error in CImageManager::loadEffect","can not judge follow effect type : %s",keyword);

			name						=	clDecoder.getStream();			//	이미지 이름
			strncpy(m_pEfectInfo[m_iEffectCount].m_strImageName,name,31);
			name						=	clDecoder.getStream();			//	이미지 파일 이름
			strncpy(m_pEfectInfo[m_iEffectCount].m_strImageFileName,name,64);

			if	(isSlimMemoryMode()	&&	cFILE::Size(name)	>	1024*500)
				code	=	1;
		}

		switch(code)
		{
			case	1			:
			{
				m_pEfectInfo[m_iEffectCount].m_wType	=	eEDF_QANM;

				g_apEffect[m_iEffectCount]				=	new	cANMQ();

				if	(g_apEffect[m_iEffectCount]->Load(name,TRUE,p_nux,p_mcu))	
					m_iEffectCount++;
				else
					return	ERRMSG("error in CImageManager::loadEffect",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,name);
				break;
			}

			case	2			:
			{
				strncpy(m_pEfectInfo[m_iEffectCount].m_strImageFileName,name,64);
				m_pEfectInfo[m_iEffectCount].m_wType	=	eEDF_ANM;

				g_apEffect[m_iEffectCount]	=	new	cANM();

				if (g_apEffect[m_iEffectCount]->Load(name,TRUE,p_nux,p_mcu))						
					m_iEffectCount++;
				else	
					return	ERRMSG("error in CImageManager::loadEffect",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,name);
				break;
			}

			case	dCODE_END		:
			case	dCODE_FINISH	:
			case	dCODE_CLOSE		:
			case	dCODE_OPEN		:
				break;

			default		:
			{	
				g_eh.addStaticLog("Undefined KEY WORD '%s'",keyword);
				break;
			}
		}
	}

	SetLogFolder();

	m_wNormalHit			=	getEffectIndex("일반타격");
	m_wImageMonsterEnterance=	getEffectIndex("몬스터 등장");
	m_wImageLevelUp			=	getEffectIndex("레벨업");
	m_wImageSkillLevelUp	=	getEffectIndex("스킬 레벨업");
	m_wInstanceKillEffect	=	getEffectIndex("즉사");	
	m_wDancingBlocker		=	getEffectIndex("댄싱 블로커");
	m_wDamageReturnEffect	=	getEffectIndex("DamageReturn");
	m_wDamageAbsorbEffect	=	getEffectIndex("AbsorbDamage");
	m_wCriticalHitEffect	=	getEffectIndex("CriticalEffect");
	m_wSoulOutEffect		=	getEffectIndex("SoulOut");
	m_wRushDustImage		=	getEffectIndex("Rush Dust");
	m_wCastDisplacementImage=	getEffectIndex("빙의");

	m_wStunImage			=	getEffectIndex("스턴효과");
	m_wSleepImage			=	getEffectIndex("잠효과");
	m_wLaziness				=	getEffectIndex("게으름");
	m_wConfuseImage			=	getEffectIndex("혼란효과");
	m_wBerserkImage			=	getEffectIndex("버서커효과");
	m_wDisplacementImage	=	getEffectIndex("빙의효과");
	m_wBlindImage			=	getEffectIndex("장님효과");
	m_wLevitateShadow		=	getEffectIndex("레비테이트");

	m_wBlockerImage			=	getEffectIndex("블로커 00");
	m_wBoomerangeImage		=	getEffectIndex("부메랑 00");
	m_wSpearBlockerImage	=	getEffectIndex("창 블로커");
	m_wHitPoisonEffect		=	getEffectIndex("중독hit");
	m_wHitDarkDamageEffect	=	getEffectIndex("어둠 대미지hit");
	m_wGuardianPostImage	=	getEffectIndex("가디언 포스트");
	m_wEatPotionImage		=	getEffectIndex("포션 먹기");
	m_wCharmedImage			=	getEffectIndex("유혹");
	m_wTownPortalImage		=	getEffectIndex("타운 포탈");
	m_wPortalImage			=	getEffectIndex("포탈");

	m_wBreedingRecordImage	=	getEffectIndex("사육기록");
	m_wPetAnalCommandImage	=	getEffectIndex("펫 명령 분석");
	m_wPetMark				=	getEffectIndex("펫 마크");
	m_wPetUnsealImage		=	getEffectIndex("품에 넣기");

	m_wEI_Taunt				=	getEffectIndex("타운트");

	m_wSummonBeastPowerUp1	=	getEffectIndex("소환수 파워 업1");
	m_wSummonBeastPowerUp2	=	getEffectIndex("소환수 파워 업2");
	m_wSummonningSummonBeast=	getEffectIndex("소환수 소환");
	m_wMiniPetSummon		=	getEffectIndex("1073_hit");

	m_wRebirthImage				=	getEffectIndex("rebirth");

	m_awRebirthMarkImage[0]	=	getEffectIndex("rebirth01");
	m_awRebirthMarkImage[1]	=	getEffectIndex("rebirth02");

	m_wSoundOfLeadersBellImage	=	getEffectIndex("big_bell");
	m_wSoundOfLeadersBellMarkImage	=	getEffectIndex("leders_bell_mask");


	m_wEI_FlameRing			=	getEffectIndex("플레임 링");
	m_wEI_Incinerate		=	getEffectIndex("인시너레이트");
	m_wEI_VaccumPoint		=	getEffectIndex("베이큠 포인트");
	m_wEI_InclineStatus		=	getEffectIndex("상태 상승");
	m_wEI_DeclineStatus		=	getEffectIndex("상태 저하");
	m_wEI_HotSkin			=	getEffectIndex("핫 스킨");
	m_wEI_DarkWeapon		=	getEffectIndex("다크 웨폰");
	m_wInnerTeleport		=	getEffectIndex("내부 텔레포트");
	m_wInvincible			=	getEffectIndex("무적");
	m_wLevelDrain			=	getEffectIndex("레벨 드레인");
	m_wLevelDown			=	getEffectIndex("레벨 다운");
	m_wDeathCounter			=	getEffectIndex("죽음의 카운트다운");
	m_wStealMoney			=	getEffectIndex("소매치기");

	m_wHitTortureDamageImage=	getEffectIndex("고문 대미지");
	m_wEI_Torture			=	getEffectIndex("고문 상태");
	m_wEI_CancerHall		=	getEffectIndex("캔서 홀");
	m_wEI_UntimateBarrier	=	getEffectIndex("얼티밋 배리어");
	m_wPigeonPost			=	getEffectIndex("전서구");
	m_wEI_RabbitRush		=	getEffectIndex("래빗 러쉬");
	m_wSetOffFirecrackerImage=	getEffectIndex("폭죽 발사");
	m_wMainQuestEagle		=	getEffectIndex("메인퀘스트전령");

	m_wEI_Contract			=	getEffectIndex("status_contract");
	m_wEI_BloodCompact		=	getEffectIndex("status_blood_compact");
	m_wEI_SoulOath			=	getEffectIndex("status_soul_oath");
	m_wEI_InterruptingArmor	=	getEffectIndex("status_Interrupting_Armor");
	m_wEI_HwaByung			=	getEffectIndex("status_hwabyung");
	m_wEI_BloodDrain		=	getEffectIndex("status_blood_drain");
	m_wEI_PlotOfShadow		=	getEffectIndex("status_plot_of_shadow");
	m_wEI_Marionette		=	getEffectIndex("status_marionette");
	m_wEI_ReversalPower		=	getEffectIndex("status_reversal_power");
	m_wEI_Impulse			=	getEffectIndex("status_impulse");
	m_wEI_LightningBarrier	=	getEffectIndex("status_lightning_barrier");
	m_wEI_NoMoreFight		=	getEffectIndex("815_hit");

	m_wUseSpecialAttack		=	getEffectIndex("status_use_special_attack");
	m_wRegenHPImage			=	0xffff;
	m_wShutInMagicBox		=	getEffectIndex("매직 박스");
	m_wRollinggLog			=	getEffectIndex("통나무");
	m_wTransImage			=	getEffectIndex("변신");

	m_wBreakArmor			=	getEffectIndex("status_break_armor");
	m_wBreakWeapon			=	getEffectIndex("status_break_weapon");

	m_wEI_SmellOfDeath		=	m_wEI_DarkWeapon;

	{
		for	(int i=0;i<10;i++)
			m_awFirecrackerImage[i]	=	getEffectIndex(_ms("폭죽%.2d",i));
	}
	m_wSnowImage			=	 	getEffectIndex("SnowImage");		// 눈이미지.	
	m_wEI_SC_Soul_Consensus		=	 	getEffectIndex("skill_cast_soul_consensus");	// 교감
	m_wEI_SC_Soul_WindmilSlash	=	 	getEffectIndex("skill_cast_soul_windmilslash");	// 풍차베기
	m_wEI_SC_Soul_BindingWords	=	 	getEffectIndex("skill_cast_soul_bindingwords");	// 언어구속
	m_wEI_SC_Soul_Blaze			=	 	getEffectIndex("skill_cast_soul_blaze");		// 소울블레이즈
	m_wEI_SC_Soul_TyphoonSlash	=	 	getEffectIndex("skill_cast_soul_typhoonslash");	// 대회전베기
	m_wEI_SC_Soul_Snatcher		=	 	getEffectIndex("skill_cast_soul_snatcher");		// 소울스내쳐
	m_wEI_SS_Soul_Blaze			=	 	getEffectIndex("skill_status_soul_blaze");		// 소울블레이즈(지속)
	m_wEI_SA_Soul_StrikeSlash		=	 	getEffectIndex("skill_action_soul_strikeslash");		// 내려 베기

	m_wEI_SS_Soul_CruelSoul		=		getEffectIndex("skill_status_soul_cruelsoul");	// 잔혹한영혼
	m_wEI_SS_Soul_Release		=		getEffectIndex("skill_status_soul_release");		// 해방.
	m_wEI_SC_Soul_Casting		=		getEffectIndex("skill_cast_soul_casting");		// 캐스팅
	m_wEI_SS_Soul_Consensus		=	 	getEffectIndex("skill_status_soul_consensus");	// 교감 상태..
	m_wEI_SS_Soul_InterruptingWeapon		=	 	getEffectIndex("SS_soul_Interruptingweapon");	// 무기해제
	m_wEI_SC_Soul_Release		=		getEffectIndex("skill_cast_soul_release");		// 해방캐스팅
	m_wEI_SS_Soul_BindingWords		=		getEffectIndex("skill_status_soul_bindingwords");		// 언어구속.
	m_wEI_SS_Soul_NotUseReaction		=		getEffectIndex("SS_soul_notusereaction");		// 리액션스킬사용금지
	m_wEI_SS_Cham_Bear			=		getEffectIndex("skill_status_bear");		// 투기상태 - 곰
	m_wEI_SS_Cham_Bull			=		getEffectIndex("skill_status_Bull");		// 투기상태 - 소
	m_wEI_SS_Cham_Hawk			=		getEffectIndex("skill_status_Hawk");		// 투기상태 - 독수리
	m_wEI_SS_Cham_Snake			=		getEffectIndex("skill_status_Snake");		// 투기상태 - 뱀
	m_wEI_SS_Cham_Puma			=		getEffectIndex("skill_status_Puma");		// 투기상태 - 푸마
	m_wEI_Hit_Hard_Blow			=		getEffectIndex("hit_hard_blow");			//	강타 히트!
	m_wEI_Hit_FightingSpirit	=		getEffectIndex("hit_fightingspirit");		// 투지 히트!
	m_wEI_SS_Hard_Blow			=		getEffectIndex("skill_status_hard_blow");			//	강타 효과
	m_wEI_Hit_Ignore_Block		=		getEffectIndex("hit_ignore_block");			//	블럭무시
	m_wEI_WhiteBit				=		getEffectIndex("white_bit");			//	하얀비트
	m_wEI_RedBit				=		getEffectIndex("red_bit");				//	레드비트
	m_wEI_GleamTant				=		getEffectIndex("빛의 장막");				//	빛의장막
	m_wEI_PowerOfVitalization	=		getEffectIndex("활성화의 권능");				//	활성화의권능
	m_wHitBleedingEffect		=		getEffectIndex("hit_bleeding");				//	출혈
	m_wHitElectricShockEffect	=		getEffectIndex("hit_electricshock");				//	감전
	m_wEI_MakeTypeUndead		=		getEffectIndex("status_maketypeundead");				//	언데드화
	m_wEI_ElectricShock			=		getEffectIndex("status_electricshock");				//	감전상태
	cFILE	file;

	file.Open("data/effect_data.dat","wb");
	int	iSkipSize			=	sizeof(CClientSaveImageData)-4;
	file.Write(&iSkipSize,4);
	file.Write(this,sizeof(CClientSaveImageData));
	file.Write(m_pEfectInfo,sizeof(CEffectDataInfo)*m_iEffectCount);
	file.Close();
#endif
	return TRUE;
}

//
//이펙트 인덱스 구하기
int
CImageManager::getEffectIndex(char *_strName)
{
	for (int i=0;i<m_iEffectCount;i++)
		if (STRICMP(m_pEfectInfo[i].m_strImageName,_strName)==0)
			return	i;

	return	0xffff;
}

BOOL
CImageManager::loadIMonsterBody(int _iBody,int _iVariation)
{
	int		iBody			=	_iBody;
	int		iVariation		=	_iVariation;
	char	*lpstrFileName	=	NULL;

	if	(iBody	<	dJOB_MONSTER_START)
		return	FALSE;

	lpstrFileName	=	l_strMonsterFileName[iBody-dJOB_MONSTER_START];

//	1번 속성은 항상 로드되어 있는 몹이다.
//	if	(g_aMonsterBodyLoadInfo[iBody-dJOB_MONSTER_START].m_wLoadMethodValue	==	eANM_LM_ALWAYS)
//	{
//		return	FALSE;
//	}

	SetCurrentDirectory(_ms("%s/data/Monsters",g_strLogFolder));

	if	(lpstrFileName)
	{
		if	(!g_apIBody[iBody])
			g_apIBody[iBody]	=	new cANM();

		if	(!g_apIBody[iBody]->Load(lpstrFileName,FALSE))
			return	SetLogFolder();

		if	(iVariation)
		{
			if	(!g_apIMonsterVariation[iBody][iVariation])
				g_apIMonsterVariation[iBody][iVariation]	=	new cANM();

			char	strFName[256];
			strcpy(strFName,_exportName(lpstrFileName));

			sprintf(strFName,"%s_%.2d.sad",strFName,iVariation);

			if	(!g_apIMonsterVariation[iBody][iVariation]->Load(strFName,FALSE))
				return	SetLogFolder();
		}
	}

	SetLogFolder();

	return	TRUE;
}

BOOL
CImageManager::loadStaticMonsterImageData(cNUX *_lpNux,cMCU *_lpMcu)
{
	int	i;

	for (i=0;;i++)
	{
		if	(STRICMP(l_strMonsterFileName[i],"end") == 0)
			break;

		if	(isSlimMemoryMode())
		{
			g_apBody[dJOB_MONSTER_START+i]	=	new cANMQ();		//	항상 로드는 아니지만 그거다.. - -
			g_alpBody[dJOB_MONSTER_START+i]	=	g_apBody[dJOB_MONSTER_START+i];
		}
		else
		if	(g_aMonsterBodyLoadInfo[i].m_wLoadMethodValue	<=	eANM_LM_ALWAYS)
		{
			int		iJob	=	dJOB_MONSTER_START+i;

			g_apBody[iJob]	=	new cANM();		//	항상 로드는 아니지만 그거다.. - -
			g_alpBody[iJob]	=	g_apBody[iJob];

			for(int j=0;j<dMONSTER_VARIATION_COUNT;j++)
				g_apMonsterVariation[iJob][j]	=	new cANM();		//	항상 로드는 아니지만 그거다.. - -
		}
	}

	SetCurrentDirectory(_ms("%s/data/Monsters",g_strLogFolder));

	for (i=0;;i++)
	{
		if	(STRICMP(l_strMonsterFileName[i],"end") == 0)
			break;

		if	(isSlimMemoryMode())
		{
			if (!g_apBody[dJOB_MONSTER_START+i]->Load(l_strMonsterFileName[i],FALSE,_lpNux,_lpMcu))
				return	FALSE;
		}
		else
		{
			if	(g_aMonsterBodyLoadInfo[i].m_wLoadMethodValue	==	eANM_LM_ALWAYS)
				if (!g_apBody[dJOB_MONSTER_START+i]->Load(l_strMonsterFileName[i],FALSE,_lpNux,_lpMcu))
					return	FALSE;

			if	(g_aMonsterBodyLoadInfo[i].m_wLoadMethodValue	==	eANM_LM_QUICK)
			{
				if (!g_aQBody[i].Load(l_strMonsterFileName[i],FALSE,_lpNux,_lpMcu))
					return	FALSE;

				g_alpBody[dJOB_MONSTER_START+i]	=	(cANM *)&g_aQBody[i];
//				g_apBody[dJOB_MONSTER_START+i]	=	(cANM *)&g_aQBody[i];
			}
		}
	}

	SetCurrentDirectory(_ms("%s/data/NPC",g_strLogFolder));

	for (i=0;;i++)
	{
		if	(STRICMP(l_strNpcFileName[i],"end")	==	0)
			break;

		if	(!_GetBody(dJOB_NPC_START+i)->Load(l_strNpcFileName[i],TRUE,_lpNux,_lpMcu))
			return	SetLogFolder();
	}

	SetLogFolder();

	return	TRUE;
}

//
//	직업 초기화
BOOL
CImageManager::loadHeroImageData(cNUX *_lpNux,cMCU *_lpMcu)
{
	int		i;

	SetCurrentDirectory("data/Heros");

//	주인공 데이터 로딩
	{
		for (i=0;;i++)
		{
			if (STRICMP(l_strHeroBody[i],"end")	==	0)
				break;
			if (STRICMP(l_strHeroBody[i],"null")==	0)
				continue;

			if (!_GetBody(i)->Load(_ms("%s/data/heros/%s.sad",g_strLogFolder,l_strHeroBody[i]),TRUE,_lpNux,_lpMcu))
			{
				char	folder[512];
				GetCurrentDirectory(512,folder);
				SetLogFolder();
				return	FALSE;//	로딩
			}
		}
	}	//	주인공 데이터 로딩

	SetLogFolder();
	SetCurrentDirectory("data/Heros");

//	소환수 데이터 로딩
	{
		int	iType,iGrade;

		for (iType=0;iType<dSUMMON_BEAST_TYPE_COUNT;iType++)
			for (iGrade=0;iGrade<dSUMMON_BEAST_GRADE_COUNT;iGrade++)
			{
				char	strFileName[128];
				int		iBody	=	dBODY_KELBY_1+iType*dSUMMON_BEAST_GRADE_COUNT+iGrade;

				sprintf(strFileName,"summon_beast_%.2d_%.2d.sad",iType+1,iGrade+1);

				if	(!_GetBody(iBody)->Load(strFileName,TRUE,_lpNux,_lpMcu))
				{
					SetLogFolder();
					return	FALSE;//	로딩
				}
			}
	}
//	주인공 데이터 로딩

	SetLogFolder();

	SetCurrentDirectory("data/NPC");

//	변신후 캐릭
	{
		if	(!_GetBody(dJOB_NPC_FAT_GIRL)->Load("fat_girl.sad",TRUE,_lpNux,_lpMcu))
			return	ERRMSG("error!!",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"data/npc/fat_girl.sad");
		if	(!_GetBody(dJOB_NPC_FROG)->Load("frog.sad",TRUE,_lpNux,_lpMcu))
			return	ERRMSG("error!!",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"data/npc/frog.sad");
		if	(!_GetBody(dJOB_NPC_RABBIT)->Load("rabbit.sad",TRUE,_lpNux,_lpMcu))
			return	ERRMSG("error!!",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"data/npc/rabbit.sad");
		if	(!_GetBody(dBODY_OPERATOR_ANGEL_MALE)->Load("operator_man.sad",TRUE,_lpNux,_lpMcu))
			return	ERRMSG("error!!",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"data/npc/operator_man.sad");
		if	(!_GetBody(dBODY_OPERATOR_ANGEL_FEMALE)->Load("operator_woman.sad",TRUE,_lpNux,_lpMcu))
			return	ERRMSG("error!!",dMSG_CAN_NOT_FIND_FOLLOW_FILE_FORM,"data/npc/operator_woman.sad");
	}

	SetLogFolder();

	SetCurrentDirectory("data/interface/game");

	for (i=0;i<dPLAYER_JOB_COUNT;i++)
	{
		if	(!g_aJobIcon[i].Load(l_strJobIconFileName[i],TRUE,_lpNux,_lpMcu))
		{
			SetLogFolder();
			return	FALSE;//	로딩
		}
	}

	SetLogFolder();
//	주인공/소환수/무기/방패 전부 로딩

	return	TRUE;
}

//
//	장비 데이터 로딩
BOOL
CImageManager::loadEquipmentImageData(cNUX *_lpNux,cMCU *_lpMcu)
{
	SetCurrentDirectory("data/equipment");

	DeleteFile("tammer - Recoder.sad");

	int	iJob,iEquipment;

	for (iJob=0;iJob<dPLAYER_JOB_COUNT;iJob++)
	{
		iEquipment	=	0;

		for(iEquipment=0;;iEquipment++)
		{
			if	(strcmp(l_strWeaponFileName[iJob][iEquipment],"end")	==	0)	
				break;

			if	(strcmp(l_strWeaponFileName[iJob][iEquipment],"useSameWeapon")	==	0)	
			{
				g_apWeapon[iJob][iEquipment]	=	g_apWeapon[iJob-1][iEquipment];
				continue;
			}
			if	(strcmp(l_strWeaponFileName[iJob][iEquipment],"#05")	==	0)	
			{
				g_apWeapon[iJob][iEquipment]	=	g_apWeapon[iJob][iEquipment-1];
				continue;
			}

			if	(isSuperSlimMemoryMode())
				g_apWeapon[iJob][iEquipment]	=	new	cANMQ ();
			else
				g_apWeapon[iJob][iEquipment]	=	new	cANM ();

			g_apWeapon[iJob][iEquipment]->Load(l_strWeaponFileName[iJob][iEquipment],TRUE,_lpNux,_lpMcu);

		}

		for(iEquipment=0;;iEquipment++)
		{
			if	(strcmp(l_strShieldFileName[iJob][iEquipment],"end")	==	0)
				break;

			g_aShield[iJob][iEquipment].Load(l_strShieldFileName[iJob][iEquipment],TRUE,_lpNux,_lpMcu);
		}
	}

	SetLogFolder();

	return	TRUE;
}

//
//	현재 필드에서만 사용하는 몸뚱아리들 로딩
BOOL
CImageManager::loadLocalBody(cNUX *_lpNux,cMCU *_lpMcu)
{
	if	(isSlimMemoryMode())
		return	TRUE;

	if	(g_bIsGuildBattleField)
		g_eh.addStaticLog("loadLocalBody");

	int	i;

//	일단 2번 속성은 전부 ANMQ로 설정
	for (i=0;;i++)
	{
		if	(STRICMP(l_strMonsterFileName[i],"end") == 0)
			break;

		if	(g_aMonsterBodyLoadInfo[i].m_wLoadMethodValue	==	eANM_LM_QUICK)
		{
			g_alpBody[i+dJOB_MONSTER_START]	=	(cANM *)&g_aQBody[i];
//			g_apBody[i+dJOB_MONSTER_START]	=	(cANM *)&g_aQBody[i];
		}

		if	(g_aMonsterBodyLoadInfo[i].m_wLoadMethodValue	==	eANM_LM_ALWAYS)
			continue;
	}

	BYTE	aLoaded[1024];

	memset(aLoaded,0,sizeof(aLoaded));

	for (i=0;i<m_iLoadedBodyCount;i++)
	{
		int		iBody			=	m_aLoadedBody[i][0];
		int		iVariation		=	m_aLoadedBody[i][1];
		char	*lpstrFileName	=	NULL;

		if	(iBody	>=	dJOB_MONSTER_START)
		{
			lpstrFileName	=	l_strMonsterFileName[iBody-dJOB_MONSTER_START];

//	1번 속성은 항상 로드되어 있는 몹이다.
			if	(g_aMonsterBodyLoadInfo[iBody-dJOB_MONSTER_START].m_wLoadMethodValue	==	eANM_LM_ALWAYS)
				continue;

			if	(!_lpNux)
				SetCurrentDirectory(_ms("%s/data/Monsters",g_strLogFolder));

//	혹시 이 필드에서 2번 속성 몹이 있다면 로딩한다.
			if	(g_aMonsterBodyLoadInfo[iBody-dJOB_MONSTER_START].m_wLoadMethodValue	==	eANM_LM_QUICK)
			{
				g_aBody[iBody-dJOB_MONSTER_START].Load(lpstrFileName,FALSE,_lpNux,_lpMcu);
				g_alpBody[iBody]	=	(cANM *)&g_aBody[iBody-dJOB_MONSTER_START];
//				g_apBody[iBody]	=	(cANM *)&g_aBody[iBody-dJOB_MONSTER_START];

				continue;
			}
		}

		if	(lpstrFileName)
		{
			if	(aLoaded[iBody]	==	FALSE)
				if	(!_GetBody(iBody)->Load(lpstrFileName,FALSE,_lpNux,_lpMcu))
					return	SetLogFolder();

			aLoaded[iBody]	=	TRUE;

			if	(iBody	==	dJOB_MONSTER_SAGE)
				if	(!g_anmSageStone.Load("sage_stone.sad",TRUE,_lpNux,_lpMcu))
					return	SetLogFolder();

			if	(iVariation)
			{
				cANM	*lpAnm	=	_GetMonsterVariation(iBody,iVariation);

				char	strFName[256];
				strcpy(strFName,_exportName(lpstrFileName));

				sprintf(strFName,"%s_%.2d.sad",strFName,iVariation);

				if	(!lpAnm->Load(strFName,FALSE,_lpNux,_lpMcu))
					return	SetLogFolder();
			}
		}
	}

	SetLogFolder();

	return	TRUE;
}

//
//	현재 필드에서만 사용하는 몸뚱아리들 로딩
BOOL
CImageManager::loadPalette(cNUX *_lpNux,cMCU *_lpMcu)
{
	int		iBody;
	char	*lpstrFileName;

	cFILE	file;

	SetCurrentDirectory(_ms("%s/data/Monsters",g_strLogFolder));

	for (iBody=dJOB_MONSTER_START;iBody<dJOB_IMAGE_DATA_COUNT;iBody++)
	{
		char	strFileName[256];

		lpstrFileName	=	l_strMonsterFileName[iBody-dJOB_MONSTER_START];

		if (STRICMP(lpstrFileName,"end")==0)	break;

		strcpy(strFileName,_exportName(lpstrFileName));

		if (!file.Open(_ms("%s.plt",strFileName),"rb"))	continue;

		file.Read(&g_aPalette[iBody].m_wCount,2);

		for (int i=0;i<g_aPalette[iBody].m_wCount;i++)
		{
			g_aPalette[iBody].m_apPalette[i]	=	new	WORD [256];
			file.Read(g_aPalette[iBody].m_apPalette[i],256*2);

			CONVERTPALETTE(g_aPalette[iBody].m_apPalette[i],256);
		}

		file.Close();
	}

	SetCurrentDirectory(_ms("%s/data/Heros",g_strLogFolder));

	for (iBody=0;;iBody++)
	{
		lpstrFileName	=	l_strHeroBody[iBody];

		if	(STRICMP(lpstrFileName,"end")==0)
			break;

		if	(!file.Open(_ms("%s.plt",lpstrFileName),"rb"))
			continue;

		g_aPalette[iBody].m_wCount	=	5;

		for (int i=0;i<g_aPalette[iBody].m_wCount;i++)
		{
			g_aPalette[iBody].m_apPalette[i]	=	new	WORD [256];
			file.Read(g_aPalette[iBody].m_apPalette[i],256*2);

			if	(g_iPixelFormat	==	dRGB555)
			{
				for (int j=0;j<256;j++)
					g_aPalette[iBody].m_apPalette[i][j]	=	RGB565ToRGB555(g_aPalette[iBody].m_apPalette[i][j]);
			}
		}

		file.Close();
	}

	SetLogFolder();

	for (iBody=0;iBody<dJOB_IMAGE_DATA_COUNT;iBody++)
	{
		SetLogFolder();

		if	(g_aPalette[iBody].m_wCount)
			continue;

		lpstrFileName=	NULL;

		if (iBody	>=	dJOB_MONSTER_START)
		{
			lpstrFileName	=	l_strMonsterFileName[iBody-dJOB_MONSTER_START];

			if	(STRICMP(lpstrFileName,"end")==0)
				break;

			if	(!_lpNux)
				SetCurrentDirectory(_ms("%s/data/Monsters",g_strLogFolder));
		}
		else
		if (iBody	>=	dJOB_NPC_START)
		{
			lpstrFileName	=	l_strNpcFileName[iBody-dJOB_NPC_START];

			if	(STRICMP(lpstrFileName,"end")==0)
			{
				iBody	=	dJOB_MONSTER_START-1;
				continue;
			}

			if	(!_lpNux)
				SetCurrentDirectory(_ms("%s/data/Npc",g_strLogFolder));
		}
		else
		{
			lpstrFileName	=	l_strHeroBody[iBody];

			if	(STRICMP(lpstrFileName,"end")==0)
			{
				iBody	=	dJOB_NPC_START-1;
				continue;
			}

			if	(!_lpNux)
				SetCurrentDirectory(_ms("%s/data/Heros",g_strLogFolder));
		}

		if	(lpstrFileName)
		{
			if	(!IsFile(lpstrFileName))
				continue;

			g_aPalette[iBody].m_wCount		=	1;
			g_aPalette[iBody].m_apPalette[0]=	new WORD [256];

			_GetBody(iBody)->LoadPlt(g_aPalette[iBody].m_apPalette[0],lpstrFileName);
		}
	}

	SetLogFolder();

	return	TRUE;
}

//
//	효과 출력
void
CImageManager::putEffect(int _iEffect,int _iX,int _iY,int _iFrameCounter,int _iAnm,int _iDirect,int _iHorzScale,int _iVertScale)
{
	cANM*	lpEffect	=	getEffect(_iEffect);

	if (!lpEffect)
		return;

	int	iMaxFrame	=	lpEffect->GetFrameCount(_iAnm);
	int	iFPS		=	lpEffect->GetFPS(0);

	lpEffect->PutReg(_iX,_iY,_iAnm,_iDirect,(_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame,_iHorzScale,_iVertScale);
}

//
//	출력
void
CImageManager::putShadow(int _iImage,int _iX,int _iY,int _iAnm,int _iDirect,int _iFrame,int _iHorzScale,int _iVertScale)
{
	cANM*	lpEffect		=	getEffect(_iImage);

	if	(!lpEffect)
		return;

	lpEffect->PutShadow(_iX,_iY,_iAnm,_iDirect,_iFrame,_iHorzScale,_iVertScale);
}

void
CImageManager::putWhichUsePalette(int _iImage,int _iX,int _iY,WORD *_lpPalette,int _iAnm,int _iDirect,int _iFrame,int _iHorzScale,int _iVertScale,int _iOutputEffect)
{
	cANM*	lpEffect		=	getEffect(_iImage);
	WORD	*lpBackupPalette=	NULL;

	if	(!lpEffect)
		return;

	if	(_lpPalette)
	{
		lpBackupPalette				=	lpEffect->m_sprite.m_lpPlt;
		lpEffect->m_sprite.m_lpPlt	=	_lpPalette;
	}

	if	(_iOutputEffect	==	0xffff)
		lpEffect->PutReg(_iX,_iY,_iAnm,_iDirect,_iFrame,_iHorzScale,_iVertScale);
	else
		lpEffect->Put(_iX,_iY,_iAnm,_iDirect,_iFrame,_iHorzScale,_iVertScale,_iOutputEffect);

	if	(lpBackupPalette)
		lpEffect->m_sprite.m_lpPlt	=	lpBackupPalette;
}

//
//	출력
void
CImageManager::putWhichUsePalette(int _iImage,int _iX,int _iY,int _iPaletteIndex,int _iAnm,int _iDirect,int _iFrame,int _iHorzScale,int _iVertScale,int _iOutputEffect)
{
	WORD	*lpPalette	=	NULL;

	if	(_iPaletteIndex	>= 0 && _iPaletteIndex	< m_wPaletteCount)
		lpPalette	=	&m_pPalette[_iPaletteIndex*256];

	putWhichUsePalette(_iImage,_iX,_iY,lpPalette,_iAnm,_iDirect,_iFrame,_iHorzScale,_iVertScale,_iOutputEffect);
}

void
CImageManager::put(int _iImage,int _iX,int _iY,int _iAnm,int _iDirect,int _iFrame,int _iHorzScale,int _iVertScale)
{
	cANM*	lpEffect		=	getEffect(_iImage);

	if (!lpEffect)
		return;

	lpEffect->PutReg(_iX,_iY,_iAnm,_iDirect,_iFrame,_iHorzScale,_iVertScale);
}

BOOL
CImageManager::isExistBody(int _iBody)
{
//	if	(g_apBody[_iBody]	==	NULL)
	if	(g_alpBody[_iBody]	==	NULL)
		return	FALSE;

	return	TRUE;
}

cANM*
CImageManager::getEffect(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>=	m_iEffectCount)
		return	NULL;

	return (cANM *)g_apEffect[_iIndex];
}

//
//	길드 마크 팔레트 초기화
BOOL
CGuildMarkPalette::init()
{
	cFILE	file;

	char	dir[256];

	GetCurrentDirectory(256,dir);

	if	(!file.Open("data/interface/game/guild_mark_palette.plt","rb"))
		return ERRMSG("data open error","can not find folow file\n\ndata/interface/game/guild_mark_palette.plt");	//	아이템 아이콘

	file.Read(m_awPalette,512);
	file.Close();

	if	(!file.Open("data/interface/game/guild_mark_base_color.plt","rb"))
		return ERRMSG("data open error","can not find folow file\n\ndata/interface/game/guild_mark_base_color.plt");	//	아이템 아이콘

	file.Read(m_awBaseColor,512);
	file.Close();
	
	if (g_iPixelFormat != dRGB565)
		for (int i=0;i<256;i++)	
		{
			m_awPalette[i]	=	RGB565ToRGB555(m_awPalette[i]);
			m_awBaseColor[i]=	RGB565ToRGB555(m_awBaseColor[i]);
		}

	return	TRUE;
}

//
//	길드 마크 아이콘만 출력
void
CGuildMarkImage::putIcon(int _iX,int _iY,int _iIndex,int _iColorPattern,int _iScale,BOOL _bIsTwinkle)
{
	if	(_iIndex	>=	0xffff)
		return;

	int	iKind	=	_iIndex/1000;
	int	iIndex	=	_iIndex%1000;

	if (iKind	>=	dGUILD_MARK_ICON_SHAPE_PATTERN_COUNT)
		return;

	if (iIndex	>=	g_sprGuildIconShape[iKind].m_iCount)
		return;

	WORD	*lpPalette	=	g_guildMarkPalette.getGradiationPalette(_iColorPattern);

	if (!lpPalette)	
		lpPalette	=	g_guildMarkPalette.getGradiationPalette(0);

	g_sprGuildIconShape[iKind].setPalette(lpPalette);

	if	(iKind	>=	6)
		g_sprGuildIconShape[iKind].restorePalette();

	WORD	awTempPlt[256];

	if	(_bIsTwinkle)
	{
		int	iIntensity	=	200;

		int	iValue		=	CGamePlay::s_iFrameCounter*300/100%iIntensity*2;

		if	(iValue	>	iIntensity)
			iValue	=	iIntensity*2-iValue;

		EffectPalette(dPLT_EFFECT_BRIGHT,g_sprGuildIconShape[iKind].m_lpPlt,awTempPlt,100+iValue,0);

		g_sprGuildIconShape[iKind].setPalette(awTempPlt);
	}

	g_sprGuildIconShape[iKind].Put(_iX,_iY,iIndex,_iScale,_iScale);
}

//
//	베이스 출력
void
CGuildMarkImage::putBase(int _iX,int _iY,int _iBaseShape,int _iColorPartition,int _iColor1,int _iColor2,int _iScale,BOOL _bIsTwinkle)
{
	_iX	-=	12;
	_iY	-=	19;

	BYTE	*lpBaseShape	=	g_sprGuildIconBaseShape.Get8(_iBaseShape);
	WORD	*lpPalette		=	g_guildMarkPalette.getBaseColorPalette(_iColor1,_iColor2);

	if (!lpBaseShape)
		return;

	if	(!lpPalette)
		lpPalette	=	g_guildMarkPalette.getBaseColorPalette(0,1);

	g_smiGuildIconBaseColorPartition.setPalette(lpPalette);

	WORD	awTempPlt[256];

	if	(_bIsTwinkle)
	{
		int	iIntensity	=	200;

		int	iValue		=	CGamePlay::s_iFrameCounter*300/100%iIntensity*2;

		if (iValue	>	iIntensity)
			iValue	=	iIntensity*2-iValue;

		EffectPalette(dPLT_EFFECT_BRIGHT,lpPalette,awTempPlt,100+iValue,0);
		g_smiGuildIconBaseColorPartition.setPalette(awTempPlt);
	}

	g_smiGuildIconBaseColorPartition.putBySpriteFilter(_iX,_iY,_iColorPartition,lpBaseShape,_iScale);
}

//
//	베이스 외곽선 출력
void
CGuildMarkImage::putBaseOutLine(int _iX,int _iY,int _iIndex,int _iColorPattern,int _iScale,BOOL _bIsTwinkle)
{
	if	(_iIndex	>=	g_sprGuildIconOutLine.m_iCount)
		return;

	WORD	*lpPalette	=	g_guildMarkPalette.getGradiationPalette(_iColorPattern);

	if (!lpPalette)
		lpPalette	=	g_guildMarkPalette.getGradiationPalette(0);

	g_sprGuildIconOutLine.setPalette(lpPalette);

	WORD	awTempPlt[256];

	if	(_bIsTwinkle)
	{
		int	iIntensity	=	200;

		int	iValue		=	CGamePlay::s_iFrameCounter*300/100%iIntensity*2;

		if (iValue	>	iIntensity)
			iValue	=	iIntensity*2-iValue;

		EffectPalette(dPLT_EFFECT_BRIGHT,lpPalette,awTempPlt,100+iValue,0);
		g_sprGuildIconOutLine.setPalette(awTempPlt);
	}

	g_sprGuildIconOutLine.Put(_iX,_iY,_iIndex,_iScale,_iScale);
}

//
//	길드 마크 출력
void
CGuildMarkImage::putMark(int _iX,int _iY,int _iIcon,int _iIconColorPattern,int _iBaseShape,int _iBaseColorPartition,int _iBaseColor1,int _iBaseColor2,int _iBaseOutLineColor,
						int _iGuildLevel,int _iHallLevel,int _iScale,BOOL _bIsTwinkle)
{
	if	(_bIsTwinkle)
	{
		int	iMaxFrame	=	g_anmGuildMarkGlow.GetFrameCount(0);
		int	iFPS		=	g_anmGuildMarkGlow.GetFPS(0);

		g_anmGuildMarkGlow.PutReg(_iX,_iY,0,0,(CGamePlay::s_iFrameCounter*iFPS/dSYNC_FPS)%iMaxFrame);
	}

	putBase(_iX,_iY,_iBaseShape,_iBaseColorPartition,_iBaseColor1,_iBaseColor2,_iScale,FALSE);
	putBaseOutLine(_iX,_iY,_iBaseShape,_iBaseOutLineColor,_iScale,FALSE);
	putIcon(_iX,_iY,_iIcon,_iIconColorPattern,_iScale,FALSE);

	if	(_iBaseShape	>=	0xff	&&	_iIcon	>=	10000)
		return;

	if	(_iGuildLevel	==	0)
		return;

	if	(_iHallLevel)
	{
		int		iKind		=	0;
		WORD	*lpPalette	=	g_guildMarkPalette.getGradiationPalette(15);

		if (!lpPalette)	
			lpPalette	=	g_guildMarkPalette.getGradiationPalette(0);

		g_sprGuildIconShape[iKind].setPalette(lpPalette);

		WORD	awTempPlt[256];

		int	iIntensity	=	200;

		int	iValue		=	CGamePlay::s_iFrameCounter*300/100%iIntensity*2;

		if	(iValue	>	iIntensity)
			iValue	=	iIntensity*2-iValue;

		EffectPalette(dPLT_EFFECT_BRIGHT,g_sprGuildIconShape[iKind].m_lpPlt,awTempPlt,100+iValue,0);

		g_sprGuildIconShape[iKind].setPalette(awTempPlt);

		g_sprGuildIconShape[iKind].Put(_iX,_iY,100+_iHallLevel-1,_iScale,_iScale);
	}

	if	(_iGuildLevel	>=	100)
	{
		g_sprInterface.Put(_iX-5,_iY+18,eSN_0+_iGuildLevel/100);
		g_sprInterface.Put(_iX,_iY+18,eSN_0+(_iGuildLevel%100)/10);
		g_sprInterface.Put(_iX+5,_iY+18,eSN_0+_iGuildLevel%10);
	}
	else
	if	(_iGuildLevel	>=	10)
	{
		g_sprInterface.Put(_iX-2,_iY+18,eSN_0+_iGuildLevel/10);
		g_sprInterface.Put(_iX+3,_iY+18,eSN_0+_iGuildLevel%10);
	}
	else
	{
		g_sprInterface.Put(_iX,_iY+18,eSN_0+_iGuildLevel);
	}
}

