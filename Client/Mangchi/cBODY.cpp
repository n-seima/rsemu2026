#include "cBODY.H"
#include "cJOB.H"
#include "cDRAW.H"

cANMQ			**g_pBody;
cANMQ			*g_apMonsterVariation[dBODY_COUNT][dMONSTER_VARIATION_COUNT];

cBODY_LIST		g_BodyList;
CPalette		g_aMonsterPalette[dBODY_COUNT];


cBODY_DATA	g_aNPCBodyData[dBODY_COUNT/3]	=
/*{
	{"수녀"						,"NUN.sad"							,dBODY_NPC_NUN						},//	수녀					기존
	{"노점상 주인(남)"			,"PITCHMAN_M.sad"					,dBODY_NPC_PITCHMAN_M				},//	노점상 주인 (남)					
	{"노점상 주인(여)"			,"PITCHMAN_F.sad"					,dBODY_NPC_PITCHMAN_F				},//	노점상 주인 (여)					
	{"노점상 주인(남노)"		,"PITCHMAN_GRAND_FATHER.sad"		,dBODY_NPC_PITCHMAN_GRAND_FATHER	},//	노점상 주인 (남노)					
	{"노점상 주인(여노)"		,"PITCHMAN_GRAND_MOTHER.sad"		,dBODY_NPC_PITCHMAN_GRAND_MOTHER	},//	노점상 주인 (여노)					

	{"end"						,"end"								,0xffff								},
};*/
{
	{"Mr. (Commoner) 1" ,"MAN1.sad" ,dBODY_NPC_MAN1 },
	{"Mr. (Commoner) 2" ,"MAN2.sad" ,dBODY_NPC_MAN2 },
	{"Mrs. (Commoner) 1" ,"WOMAN1.sad" ,dBODY_NPC_WOMAN1 },
	{"Mrs. (Commoner) 2" ,"WOMAN2.sad" ,dBODY_NPC_WOMAN2 },
	{"Young man 1 (commoner)" ,"YOUNG_MAN1.sad" ,dBODY_NPC_YOUNG_MAN1 },
	{"Young man 2 (commoner)" ,"YOUNG_MAN2.sad" ,dBODY_NPC_YOUNG_MAN2 },
	{"Lady (Commoner) 1" ,"LADY1.sad" ,dBODY_NPC_LADY1 },
	{"Lady (Commoner) 2" ,"LADY2.sad" ,dBODY_NPC_LADY2 },
	{"Grandfather (Commoner)" ,"GRAND_FATHER.sad" ,dBODY_NPC_GRAND_FATHER },
	{"Grandmother (Commoner)" ,"GRAND_MOTHER.sad" ,dBODY_NPC_GRAND_MOTHER },
	{"Boy (Commoner) 1" ,"BOY1.sad" ,dBODY_NPC_BOY1 },
	{"Boy (Commoner) 2" ,"BOY2.sad" ,dBODY_NPC_BOY2 },
	{"Girl (Commoner) 1" ,"GIRL1.sad" ,dBODY_NPC_GIRL1 },
	{"Girl (Commoner) 2" ,"GIRL2.sad" ,dBODY_NPC_GIRL2 },
	{"Noble Man 1" ,"NOBLE_MAN1.sad" ,dBODY_NPC_NOBLE_MAN1 },
	{"Noble Man 2" ,"NOBLE_MAN2.sad" ,dBODY_NPC_NOBLE_MAN2 },
	{"Noble Woman 1" ,"NOBLE_WOMAN1.sad" ,dBODY_NPC_NOBLE_WOMAN1 },
	{"Noble Woman 2" ,"NOBLE_WOMAN2.sad" ,dBODY_NPC_NOBLE_WOMAN2 },
	{"Boy (Noble)" ,"NOBLE_BOY.sad" ,dBODY_NPC_NOBLE_BOY },
	{"Girl (Noble)" ,"NOBLE_GIRL.sad" ,dBODY_NPC_NOBLE_GIRL },
	{"Gypsy (Male) 1" ,"GYPSY_MAN1.sad" ,dBODY_NPC_GYPSY_MAN1 },
	{"Gypsy (Male) 2" ,"GYPSY_MAN2.sad" ,dBODY_NPC_GYPSY_MAN2 },
	{"Gypsy (Woman) 1" ,"GYPSY_WOMAN1.sad" ,dBODY_NPC_GYPSY_WOMAN1 },
	{"Gypsy (female) 2" ,"GYPSY_WOMAN2.sad" ,dBODY_NPC_GYPSY_WOMAN2 },
	{"Gypsy (boy) 1" ,"GYPSY_BOY1.sad" ,dBODY_NPC_GYPSY_BOY1 },
	{"Gypsy (boy) 2" ,"GYPSY_BOY2.sad" ,dBODY_NPC_GYPSY_BOY2 },
	{"Gypsy (Girl) 1" ,"GYPSY_GIRL1.sad" ,dBODY_NPC_GYPSY_GIRL1 },
	{"Gypsy (girl) 2" ,"GYPSY_GIRL2.sad" ,dBODY_NPC_GYPSY_GIRL2 },
	{"Farmer 1" ,"FARMER1.sad" ,dBODY_NPC_FARMER1 },
	{"Farmer 2" ,"FARMER2.sad" ,dBODY_NPC_FARMER2 },
	{"Woodcutter 1" ,"WOODCUTTER1.sad" ,dBODY_NPC_WOODCUTTER1 },
	{"Woodcutter 2" ,"WOODCUTTER2.sad" ,dBODY_NPC_WOODCUTTER2 },
	{"Miner" ,"MINER.sad" ,dBODY_NPC_MINER },
	{"Sailor" ,"SAILER.sad" ,dBODY_NPC_SAILER },
	{"Axeman" ,"HALBERD_MAN.sad" ,dBODY_NPC_HALBERD_MAN },
	{"Paladin (Male)" ,"TEMPLE_KNIGHT_M.sad" ,dBODY_NPC_TEMPLE_KNIGHT_M },
	{"Paladin (Female)" ,"TEMPLE_KNIGHT_F.sad" ,dBODY_NPC_TEMPLE_KNIGHT_F },
	{"Bishop" ,"BISHOP.sad" ,dBODY_NPC_BISHOP },
	{"Priest" ,"PRIEST.sad" ,dBODY_NPC_PRIEST },
	{"Nun" ,"NUN.sad" ,dBODY_NPC_NUN },
	{"Mercenary (Male)" ,"MERCENARY_SOLDIER_M.sad" ,dBODY_NPC_MERCENARY_SOLDIER_M },
	{"Mercenary (Female)" ,"MERCENARY_SOLDIER_F.sad" ,dBODY_NPC_MERCENARY_SOLDIER_F },
	{"Beggar" ,"BAGGER.sad" ,dBODY_NPC_BAGGER },
	{"Desert Dancer (Female)" ,"DANCING_GIRL.sad" ,dBODY_NPC_DANCING_GIRL },
	{"Military Academy Male Inst" ,"INSTRUCTOR_M.sad" ,dBODY_NPC_INSTRUCTOR_M },
	{"Cadet Female" ,"INSTRUCTOR_F.sad" ,dBODY_NPC_INSTRUCTOR_F },
	{"Cadet 1" ,"CADET1.sad" ,dBODY_NPC_CADET1 },
	{"Cadet 2" ,"CADET2.sad" ,dBODY_NPC_CADET2 },
	{"Wizard (Old Man)" ,"WIZARD.sad" ,dBODY_NPC_WIZARD },
	{"Magician (man)" ,"MAGICIAN_M.sad" ,dBODY_NPC_MAGICIAN_M },
	{"Magician (Female)" ,"MAGICIAN_F.sad" ,dBODY_NPC_MAGICIAN_F },
	{"Weapons Shop Owner (Male)" ,"SHOPKEEPER_WEAPON.sad" ,dBODY_NPC_SHOPKEEPER_WEAPON },
	{"General Store Owner (Male)" ,"GROCER_M.sad" ,dBODY_NPC_GROCER_M },
	{"General Store Owner (Female)" ,"GROCER_F.sad" ,dBODY_NPC_GROCER_F },
	{"Street Vendor (Male)" ,"PITCHMAN_M.sad" ,dBODY_NPC_PITCHMAN_M },
	{"Street vendor (female)" ,"PITCHMAN_F.sad" ,dBODY_NPC_PITCHMAN_F },
	{"Street Vendor (Male)" ,"PITCHMAN_GRAND_FATHER.sad",dBODY_NPC_PITCHMAN_GRAND_FATHER},
	{"Stall Owner (Yono)" ,"PITCHMAN_GRAND_MOTHER.sad",dBODY_NPC_PITCHMAN_GRAND_MOTHER},
	{"Angel" ,"ANGEL.sad" ,dBODY_NPC_ANGEL },
	{"end"					,"end"						,0xffff							},
};

/*
cBODY_DATA	g_aMonsterBodyData[dBODY_COUNT/3]	=
{
	{"닭전사"			,	"CockFighter.sad"				,	dBODY_MONSTER_COCK_FIGHTER		,},
	{"켄타우로스"		,	"Centaurs.sad"					,	dBODY_MONSTER_CENTAURS			,},
	{"자이언트"			,	"Giant.sad"						,	dBODY_MONSTER_GIANT				,},
	{"미이라"			,	"Mummy.sad"						,	dBODY_MONSTER_MUMMY				,},
	{"오우거"			,	"Ogre.sad"						,	dBODY_MONSTER_OGRE				,},
	{"랩타일"			,	"Reptile.sad"					,	dBODY_MONSTER_REPTILE			,},
	{"가고일"			,	"Gargoyle.sad"					,	dBODY_MONSTER_GARGOYLE			,},
	{"데몬"				,	"Demon.sad"						,	dBODY_MONSTER_DEMON				,},
	{"터틀 드래곤"		,	"TurtleDragon.sad"				,	dBODY_MONSTER_TURTLE_DRAGON		,},
	{"자이언트 웜"		,	"GiantWorm.sad"					,	dBODY_MONSTER_GIANT_WORM		,},
	{"해골기사"			,	"SkeletonKnight.sad"			,	dBODY_MONSTER_SKELETON_KNIGHT	,},
	{"괴물 두더지"		,	"BigMole.sad"					,	dBODY_MONSTER_BIG_MOLE			,},
	{"머맨 나이트"		,	"MermanKnight.sad"				,	dBODY_MONSTER_MERMAN_KNIGHT		,},

	{"팀버맨"			,	"TimberMan.sad"					,	dBODY_MONSTER_TIMBER_MAN		,},
	{"해파리"			,	"JellyFish.sad"					,	dBODY_MONSTER_JELLYFISH			,},
	{"고스트 아머"		,	"GhostArmor.sad"				,	dBODY_MONSTER_GHOST_ARMOR		,},


	{"end"				,	"end"							,	0xffff							,},
};
*/
cBODY_DATA	g_aMonsterBodyData[dBODY_COUNT/3]	=
{
	{"Skeleton Warrior (Axe)" , "SkeletonAxe.sad",dBODY_MONSTER_SKELETON_AXE},//	
	{"Skeleton Warrior (Twin Swords)" , "SkeletonSword.sad",dBODY_MONSTER_SKELETON_SWORD},//	
	{"Skeleton Knight" , "SkeletonKnight.sad",dBODY_MONSTER_SKELETON_KNIGHT},//	
	{"Zombie" , "Zombie.sad",dBODY_MONSTER_ZOMBIE},//	
	{"Living Dead" , "LivingDead.sad",dBODY_MONSTER_LIVINNG_DEAD},//	
	{"Ghost" , "Ghost.sad",dBODY_MONSTER_GHOST},//	
	{"Mummy" , "Mummy.sad",dBODY_MONSTER_MUMMY},//	
	{"Ghost Armor" , "GhostArmor.sad",dBODY_MONSTER_GHOST_ARMOR},//	
	{"Vampire" , "Vampire.sad",dBODY_MONSTER_VAMPIRE},//	
	{"Lich" , "Lich.sad",dBODY_MONSTER_LICH},//	

	{"Black Elf" , "DarkElf.sad",dBODY_MONSTER_DARK_ELF},//	
	{"Assassin" , "Assassin.sad",dBODY_MONSTER_ASSASSIN},//	
	{"Native" , "AggressiveNative.sad",dBODY_MONSTER_AGGRESSIVE_NATIVE},//	
	{"Fallen Wizard" , "FallenWizard.sad",dBODY_MONSTER_FALLEN_WIZARD},//	
	{"Court Knight" , "TempleKnight.sad",dBODY_MONSTER_TEMPLE_KNIGHT},//	
	{"Dark Priest" , "DarkPriest.sad",dBODY_MONSTER_DARK_PRIEST},//	
	{"Dark Summoner" , "Conjuerer.sad",dBODY_MONSTER_CONJUERER},//	
	{"Giant" , "Giant.sad",dBODY_MONSTER_GIANT},//	
	{"Elf King" , "ElfKing.sad",dBODY_MONSTER_ELF_KING},//	
	{"Archmage" , "Archmage.sad",dBODY_MONSTER_ARCHMAGE},//	

	{"Demi Beholder" , "DemiBeholder.sad",dBODY_MONSTER_DEMI_BEHOLDER},//	
	{"Reptile" , "Reptile.sad",dBODY_MONSTER_REPTILE},//	
	{"Chicken Warrior" , "CockFighter.sad",dBODY_MONSTER_COCK_FIGHTER},//	
	{"Lizard Warrior" , "LizardWarrior.sad",dBODY_MONSTER_LIZARD_WARRIOR},//	
	{"Lizard Rider" , "LizardRider.sad",dBODY_MONSTER_LIZARD_RIDER},//	
	{"Rat Fighter" , "RatFighter.sad",dBODY_MONSTER_RAT_FIGHTER},//	
	{"Demon" , "Demon.sad",dBODY_MONSTER_DEMON},//	
	{"Ogre" , "Ogre.sad",dBODY_MONSTER_OGRE},//	
	{"Bahomat" , "Bahomate.sad",dBODY_MONSTER_BAHOMATE},//	
	{"Red Demon" , "RedDemon.sad",dBODY_MONSTER_RED_DEMON},//	

	{"Turtle Dragon" , "TurtleDragon.sad",dBODY_MONSTER_TURTLE_DRAGON},//	
	{"Spider" , "Spider.sad",dBODY_MONSTER_SPIDER},//	
	{"Scorpion" , "Scorpion.sad",dBODY_MONSTER_SCORPION},//	
	{"Jellyfish" , "JellyFish.sad",dBODY_MONSTER_JELLYFISH},//	
	{"Worm" , "GiantWorm.sad",dBODY_MONSTER_WORM},//	
	{"King Crab" , "KingCrab.sad",dBODY_MONSTER_KING_CRAB},//	
	{"Insect Swarm" , "InsectSwarm.sad",dBODY_MONSTER_INSECT_SWARM},//	
	{"Wolf" , "Wolf.sad",dBODY_MONSTER_WOLF},//	
	{"Big Mole" , "BigMole.sad",dBODY_MONSTER_BIG_MOLE},//	
	{"Mad Bear" , "MadBear.sad",dBODY_MONSTER_MAD_BEAR},//	

	{"Firebogey" , "FireBogy.sad",dBODY_MONSTER_FIRE_BOGY},//	
	{"TongueEye" , "TongueEye.sad",dBODY_MONSTER_TONGUE_EYE},//	
	{"Gargoyle" , "Gargoyle.sad",dBODY_MONSTER_GARGOYLE},//	
	{"Metal Golem" , "MetalGolem.sad",dBODY_MONSTER_METAL_GOLEM},//	
	{"Centaurs" , "Centaurs.sad",dBODY_MONSTER_CENTAURS},//	
	{"Merman Knight" , "MermanKnight.sad",dBODY_MONSTER_MERMAN_KNIGHT},//	
	{"Timberman" , "TimberMan.sad",dBODY_MONSTER_TIMBER_MAN},//	
	{"Saramanda" , "Salamander.sad",dBODY_MONSTER_SALAMANDER},//	
	{"Dark Fire" , "DarkFire.sad",dBODY_MONSTER_DARK_FIRE},//	
	{"White Shadow" , "WhiteShadow.sad",dBODY_MONSTER_WHITE_SHADOW},//	

	{"Skeleton Santa" , "SkeletonSanta.sad",dBODY_MONSTER_SKELETON_SANTA},//	

	{"Sage" , "sage.sad",dBODY_MONSTER_SAGE},//	
	{"Sage Master" , "sage_master.sad",dBODY_MONSTER_SAGE_MASTER},
	{"Tiamath (Left Tentacle)","tiamath_left_feeler.sad",dBODY_MONSTER_TIAMATH_FEELER},
	{"Tiamath (Body)" , "tiamath.sad",dBODY_MONSTER_TIAMATH},
	{"Tiamath (right tentacle)", "tiamath_right_feeler.sad",dBODY_MONSTER_TIAMATH_BOTTOM_FEELER},
	{"Water Dragon" , "water_dragon.sad",dBODY_MONSTER_WATER_DRAGON},
	{"Arch Devil" , "arch_devil.sad",dBODY_MONSTER_ARCH_DEVIL},
	{"Draco Lich" , "draco_lich.sad",dBODY_MONSTER_DRACO_LICH},
	{"Draco Lich Orb" , "draco_lich_orb.sad",dBODY_MONSTER_DRACO_LICH_ORB},

	{"Giant Skull" , "Giant_Skull.sad",dBODY_MONSTER_Giant_Skull },
	{"Skull Magician" , "Undead_Magician.sad",dBODY_MONSTER_Undead_Magician },
	{"Bone Chimera" , "Bone_Chimera.sad",dBODY_MONSTER_Bone_Chimera },
	{"High Elf" , "High_Elf.sad",dBODY_MONSTER_High_Elf },
	{"Gangster" , "Gangster.sad",dBODY_MONSTER_Gangster },
	{"Sikhs" , "Sikhs.sad",dBODY_MONSTER_Sikhs },
	{"Escapee" , "Escapee.sad",dBODY_MONSTER_Escapee },
	{"Madman" , "Mad_Demon.sad",dBODY_MONSTER_Mad_Demon },
	{"Reaper" , "God_of_Death.sad",dBODY_MONSTER_God_of_Death },
	{"Succubus" , "Succubus.sad",dBODY_MONSTER_Succubus },
	{"Mantis Warrior" , "Mantis_Warrior.sad",dBODY_MONSTER_Mantis_Warrior },
	{"Pumpkinhead" , "Pumpkin_Head.sad",dBODY_MONSTER_Pumpkin_Head },
	{"Great Monkey" , "Monkey_King.sad",dBODY_MONSTER_Monkey_King },
	{"Giant Frog" , "Giant_Frog.sad",dBODY_MONSTER_Giant_Frog },
	{"Tongue Monster" , "Tongue_Monster.sad",dBODY_MONSTER_Tongue_Monster },
	{"Dark Buffalo" , "Dark_Buffalo.sad",dBODY_MONSTER_Dark_Buffalo },
	{"Unicorn" , "Unicorn.sad",dBODY_MONSTER_Unicorn },
	{"Apostle" , "Apostles.sad",dBODY_MONSTER_Apostles },
	{"Cherubim" , "Cherubim.sad",dBODY_MONSTER_Cherubim },

	{"Guild Crest" , "guild_crest.sad",dBODY_MONSTER_Guild_Crest},
	{"Shield Craft" , "shield_craft.sad",dBODY_MONSTER_ShieldCraft },
	{"Elemental Post" , "elemantal_post.sad",dBODY_MONSTER_ElelementalPost},
	{"Ether Shell" , "ether_shell.sad",dBODY_MONSTER_EtherShell },
	{"Hollow Knight" , "hollow_knight.sad",dBODY_MONSTER_HollowKnight },
	{"Strategy Flag" , "guild_flag.sad",dBODY_MONSTER_GuildFlag },

	{"end"				,	"end"							,	0xffff},
};

cMonsterBodyLoadInfo	g_aMonsterBodyLoadInfo[100]	=
{
	1,	1,//	언데드	해골전사 도끼	Skeleton, Axe		175㎝
	1,	1,//	언데드	해골전사 쌍칼	Skeleton, Sword		175㎝
	2,	10,//	언데드	해골기사	Skeleton Knight		190㎝	,
	1,	1,//	언데드	좀비	Zombie		170㎝		기존
	1,	1,//	언데드	산송장	Living Dead		155㎝		기존
	0,	1,//	언데드	유령	Ghost		180㎝		기존
	1,	1,//	언데드	미이라	Mummy		200㎝		기존
	0,	1,//	언데드	고스트 아머	Ghost Armor		250㎝		
	2,	20,//	언데드	뱀파이어	Vampire		180㎝	흡혈귀, ,
	0,	1,//	언데드	리치	Lich		210㎝	공중 부양, 허
	
	1,	1,//	인간형	블랙 엘프	Black Elf		160㎝		기
	1,	1,//	인간형	어새신	Assassin		165㎝	도적 스타일
	1,	1,//	인간형	원주민			80㎝	부쉬맨 스타일의 작
	0,	1,//	인간형	타락한 마법사	Fallen Wizard		145㎝
	1,	1,//	인간형	궁정 기사단	Temple Knight		195㎝	장
	0,	1,//	인간형	다크 프리스트	Dark Priest		160㎝	회
	1,	1,//	인간형	어둠의 소환사	Conjuerer		165㎝	채
	2,	20,//	인간형	자이언트	Giant		230㎝		세미 보,
	2,	20,//	인간형	엘프 킹	Elf King		175㎝	칼과 롱보우,
	0,	1,//	인간형	대마술사	Archmage		180㎝	장신의 
	
	0,	1,//	악마형	데미 비홀더	Demi Beholder		90㎝
	1,	1,//	악마형	렙타일	Reptile		120㎝		기존
	1,	1,//	악마형	닭 전사	Cock Fighter		175㎝	
	1,	1,//	악마형	리자드 워리어	Lizard Warrior		15
	0,	1,//	악마형	리자드 라이더	Lizard Rider		18
	1,	1,//	악마형	랫 파이터	Rat Fighter		140㎝	
	0,	1,//	악마형	데몬	Demon		185㎝		세미 
	2,	20,//	악마형	오우거	Ogre		210㎝		세미 ,
	2,	50,//	악마형	바호매트			210㎝	염소 파충,
	0,	1,//	악마형	붉은 악마	Red Demon		300㎝	최

	2,	10,//	동물형	터틀 드래곤	Turtle Dragon		19
	1,	1,//	동물형	거미	Spider		130㎝		기
	1,	1,//	동물형	전갈	Scorpion		180㎝	
	1,	1,//	동물형	해파리			170㎝		기존
	1,	1,//	동물형	웜	Worm		170㎝(길이)		기
	1,	1,//	동물형	킹 크랩	King Crab		50㎝	키
	0,	1,//	동물형	곤충 떼	Insect Swarm		120㎝
	1,	1,//	동물형	늑대	Wolf		50㎝	길이는
	0,	1,//	동물형	큰 두더지	Big Moul		400㎝(
	2,	30,//	동물형	미친 곰	Mad Bear		250㎝(길이
	
	1,	1,//	신수형	불도깨비			150㎝	불의 신수
	0,	1,//	신수형	혓바닥눈			200㎝	물의 신수
	1,	1,//	신수형	가고일	Gargoyle		155㎝	바람의
	2,	20,//	신수형	메탈 골렘	Metal Golem		220㎝	땅,
	2,	25,//	신수형	켄타우로스	Centaurs		210㎝	바,
	2,	30,//	신수형	머맨 나이트	Merman Knight		220㎝,
	0,	1,//	신수형	팀버맨	Timber Man		230㎝	땅의 
	0,	1,//	신수형	사라만다	Salamander		100㎝	불
	2,	40,//	신수형	다크 파이어	Dark Fire		120㎝	어,
	1,	1,//	신수형	화이트 섀도우	White Shadow		14
};

/////////////////////////////////////////////////////////
//	정적 멤버
/////////////////////////////////////////////////////////

//
//	몸통 리스트
cBODY_LIST::cBODY_LIST()	
{
	m_iBodyCount		=	0;
	m_iNpcBodyCount		=	0;
	m_iMonsterBodyCount	=	0;
	m_iHeroBodyCount	=	0;
}	//	cBODY_LIST::cBODY_LIST()

cBODY_LIST::~cBODY_LIST()	
{
	for (int i	=	0;i<dBODY_COUNT;i++)
	{
		KILL(g_pBody[i]);

		for (int j=0;j<dMONSTER_VARIATION_COUNT;j++)
			KILL(g_apMonsterVariation[i][j]);
	}

	pKILL(g_pBody);
}	//	cBODY_LIST::cBODY_LIST()

//
//	몸통-_-추가
BOOL
cBODY_LIST::addBody(int _iBodyIndex,cBODY_DATA *_lpBodyDatas,int _iBody,char *_strBodyName)
{
	m_aBody[m_iBodyCount].m_iIndex		=	_iBodyIndex;
	m_aBody[m_iBodyCount].m_lpBodyList	=	_lpBodyDatas;
	m_aBody[m_iBodyCount].m_iBody		=	_iBody;
	strcpy(m_aBody[m_iBodyCount].m_strName,_strBodyName);

	m_iBodyCount++;

	return	TRUE;
}	//	cBODY_LIST::addBody(int _iBodyIndex,cBODY *_lpBodyDatas,int _iBody,char *_strBodyName)

//
//	초기화
BOOL
cBODY_LIST::init()
{
	int		iBodyIndex	=	0;
	int		i;

	g_pBody	=	new cANMQ* [dBODY_COUNT];

	for (i	=	0;i<dBODY_COUNT;i++)
	{
		g_pBody[i]	=	NULL;

		for (int j=0;j<dMONSTER_VARIATION_COUNT;j++)
			g_apMonsterVariation[i][j]	=	NULL;
	}

	m_iBodyCount		=	0;
	m_iNpcBodyCount		=	0;
	m_iMonsterBodyCount	=	0;
	m_iHeroBodyCount	=	0;

	while(1)
	{
		if	(g_aNPCBodyData[m_iNpcBodyCount].m_iBody			==	0xffff)
			break;
		m_iNpcBodyCount++;
	}
/*
	while(1)
	{
		if (g_aHeroBodyData[m_iHeroBodyCount].m_iBody		==	0xffff)	break;
		m_iHeroBodyCount++;
	}
*/
	while(1)
	{
		if (g_aMonsterBodyData[m_iMonsterBodyCount].m_iBody	==	0xffff)
			break;
		m_iMonsterBodyCount++;
	}

	if (!loadBodies())	return	FALSE;

/*
	while(1)
	{
		if (g_aHeroBodyData[iBodyIndex].m_iBody	==	0xffff)	break;
		g_BodyList.addBody(iBodyIndex,g_aHeroBodyData,g_aHeroBodyData[iBodyIndex].m_iBody,g_aHeroBodyData[iBodyIndex].m_strName);
		iBodyIndex++;
	}
*/

	iBodyIndex	=	0;
	while(1)
	{
		if (g_aMonsterBodyData[iBodyIndex].m_iBody	==	0xffff)	break;
		g_BodyList.addBody(iBodyIndex,g_aMonsterBodyData,g_aMonsterBodyData[iBodyIndex].m_iBody,g_aMonsterBodyData[iBodyIndex].m_strName);
		iBodyIndex++;
	}

	iBodyIndex	=	0;
	while(1)
	{
		if (g_aNPCBodyData[iBodyIndex].m_iBody	==	0xffff)	break;
		g_BodyList.addBody(iBodyIndex,g_aNPCBodyData,g_aNPCBodyData[iBodyIndex].m_iBody,g_aNPCBodyData[iBodyIndex].m_strName);
		iBodyIndex++;
	}


	return	TRUE;
}	//	cJOB::Init()

//
//	몸통-_-을 불러 온다.
BOOL
cBODY_LIST::loadBody(int index,char *fn)
{
	if	(!g_pBody[index])
		g_pBody[index]	=	new cANMQ();

	cANMQ	*lpBody	=	g_pBody[index];

	cANM_INDEX	Anms[8];

	if	(!lpBody->Load(fn))
		return	FALSE;

	int		iDirectCount	=	lpBody->GetDirectCount(dLOAD_ACTION);
	int		iMul			=	max(iDirectCount/8,1);
	int		iDiv			=	max(8/iDirectCount,1);

	for (int i=0;i<8;i++)
	{
		Anms[i].m_iAnm		=	dLOAD_ACTION;
		Anms[i].m_iDirect	=	(i/iDiv)*iMul;
		Anms[i].m_iFrame	=	0;
	}

	if	(index	==	266)
		index	=	266;

	if	(!lpBody->Read(Anms,8))
		return FALSE;

	pKILL(lpBody->m_pANM);

	for (i=0;i<dMONSTER_VARIATION_COUNT;i++)
	{
		char	strMV_file_name[512];

		sprintf(strMV_file_name,"%s_%.2d.sad",_exportName(fn),i+1);

		if	(!IsFile(strMV_file_name))
			break;

		g_apMonsterVariation[index][i]	=	new cANMQ();

		lpBody	=	g_apMonsterVariation[index][i];

		if	(!lpBody->Load(strMV_file_name))
			break;

		int		iDirectCount	=	lpBody->GetDirectCount(dLOAD_ACTION);
		int		iMul			=	max(iDirectCount/8,1);
		int		iDiv			=	max(8/iDirectCount,1);

		for (int j=0;j<8;j++)
		{
			Anms[j].m_iAnm		=	dLOAD_ACTION;
			Anms[j].m_iDirect	=	(j/iDiv)*iMul;
			Anms[j].m_iFrame	=	0;
		}

		if	(!lpBody->Read(Anms,8))
			return FALSE;

		pKILL(lpBody->m_pANM);
	}

	return	TRUE;
}

//
//	팔레트들 불러오기
BOOL
cBODY_LIST::loadPalette(int _iIndex)
{
	cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.bmp");
	cBMP		bmp;

	if (!lpFiles)
		return FALSE;
	
	g_aMonsterPalette[_iIndex].m_wSerial	=	_iIndex;
	g_aMonsterPalette[_iIndex].m_wCount		=	lpFiles->Count;
	g_aMonsterPalette[_iIndex].m_pPalette	=	new CPaletteData [lpFiles->Count];

	cSTRING	*lpFileName	=	lpFiles->Pick();

	for (int i=0;i<lpFiles->Count;i++)
	{
		CPaletteData	*lpPalette	=	&g_aMonsterPalette[_iIndex].m_pPalette[i];

		bmp.loadPalette(lpFileName->String);

		memcpy(lpPalette->m_aData	,bmp.m_pPLT,512		);
		strcpy(lpPalette->m_strName	,_exportName(lpFileName->String)	);

		lpFileName	=	lpFileName->pNEXT;
	}

	KILL(lpFiles);

	return	TRUE;
}

BOOL
cBODY_LIST::loadBodies()
{
	int		i,index;

	SetLogFolder();
	SetCurrentDirectory("data\\NPC");

	for (i =0 ;i<m_iNpcBodyCount;i++)
	{
		index	=	g_aNPCBodyData[i].m_iBody;
		loadBody(index,g_aNPCBodyData[i].m_strFileName);
	}

	SetLogFolder();
	SetCurrentDirectory("data\\monsters");

	for (i =0;i<m_iMonsterBodyCount;i++)
	{
		index	=	g_aMonsterBodyData[i].m_iBody;
		loadBody(index,g_aMonsterBodyData[i].m_strFileName);
	}

	SetLogFolder();

	BOOL	bDecodePaletteData	=	FALSE;

	if (bDecodePaletteData	==	FALSE)
	{
		SetCurrentDirectory("data/monsters");

		for (i =0;i<m_iMonsterBodyCount;i++)
		{
			index	=	g_aMonsterBodyData[i].m_iBody;

			CPalette	*lpPalette	=	&g_aMonsterPalette[index];

			char	strPaletteName[512];

			strcpy(strPaletteName,_exportName(g_aMonsterBodyData[i].m_strFileName));

			cFILE	file;

			file.Open(_ms("%s.plt",strPaletteName),"rb");
			file.Read(&lpPalette->m_wCount,2);

			lpPalette->m_wSerial	=	index;
			lpPalette->m_wCount		=	lpPalette->m_wCount;
			lpPalette->m_pPalette	=	new CPaletteData [lpPalette->m_wCount];

			for (int j=0;j<lpPalette->m_wCount;j++)
			{
				file.Read(lpPalette->m_pPalette[j].m_aData,512);
				sprintf(lpPalette->m_pPalette[j].m_strName,"%.2d",j);
			}

			file.Close();
		}
	}
	else
	{
		for (i =0;i<m_iMonsterBodyCount;i++)
		{
			SetLogFolder();
			SetCurrentDirectory("dev data/mangchi/palette");

			char	strPaletteName[512];

			index	=	g_aMonsterBodyData[i].m_iBody;

			strcpy(strPaletteName,_exportName(g_aMonsterBodyData[i].m_strFileName));

			if (!SetCurrentDirectory(strPaletteName))
				continue;

			loadPalette(index);

			SetLogFolder();
			SetCurrentDirectory("data/monsters");

			CPalette	*lpPalette	=	&g_aMonsterPalette[index];
			cFILE		file;

			file.Open(_ms("%s.plt",strPaletteName),"wb");
			file.Write(&lpPalette->m_wCount,2);

			for (int j=0;j<lpPalette->m_wCount;j++)
			{
				CONVERTPALETTE(lpPalette->m_pPalette[j].m_aData,256);
				file.Write(lpPalette->m_pPalette[j].m_aData,512);
			}
			file.Close();
		}
	}

	SetLogFolder();

	return	TRUE;
}

//
//	List에서 _iBody인 데이터의 인덱스를 리턴한다.
int
cBODY_LIST::getBodyIndex(int _iBody)
{
	int	i;

	for (i=0;i<m_iBodyCount;i++)
		if	(m_aBody[i].m_iBody	==	_iBody)	
			return	i;

	return	0xffff;
}	//	cJOB::GetBodyIndex(int _iBody)

//
//	데이터 정렬
void
cBODY_LIST::arrangeData(int _iJobGroup)
{
	int	iJobCount	=	0,i;

	for (i=0;i<m_iBodyCount;i++)
		m_aBody[i].m_iCount	=	0;

	for (i=0;i<dMAX_JOB_COUNT;i++)
	{
		cJOB	*lpJob	=	g_denJob.getJob(i);

		if	(!lpJob)
			continue;

		if	(lpJob->m_wType	==	_iJobGroup)
		{
			iJobCount++;

			int	iIndex	=	getBodyIndex(lpJob->m_wBody);

			if	(iIndex	==	0xffff)
				continue;

			m_aBody[iIndex].m_iCount++;
		}
	}
}

//
//	직업에 등록된 몸뚱아리의 숫자를 리턴한다.
int
cBODY_LIST::getUsedBodyCount()
{
	int	i,iCount	=	0;

	for (i=0;i<m_iBodyCount;i++)	if (m_aBody[i].m_iCount	>	0	)	iCount++;

	return	iCount;
}	//	cBODY_LIST::getUsedBodyCount()

//
//	직업군 별 	
int
cBODY_LIST::getRegistJobCount()
{
	int	i,iCount	=	0;

	for (i=0;i<m_iBodyCount;i++)	iCount	+=	m_aBody[i].m_iCount;

	return	iCount;
}	//	cBODY_LIST::getAvailJobCount(int _iKind)