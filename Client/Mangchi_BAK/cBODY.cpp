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
	{"아저씨 (평민) 1"		,"MAN1.sad"					,dBODY_NPC_MAN1					},
	{"아저씨 (평민) 2"		,"MAN2.sad"					,dBODY_NPC_MAN2					},
	{"부인 (평민) 1"		,"WOMAN1.sad"				,dBODY_NPC_WOMAN1				},
	{"부인 (평민) 2"		,"WOMAN2.sad"				,dBODY_NPC_WOMAN2				},
	{"청년 1 (평민)"		,"YOUNG_MAN1.sad"			,dBODY_NPC_YOUNG_MAN1			},
	{"청년 2 (평민)"		,"YOUNG_MAN2.sad"			,dBODY_NPC_YOUNG_MAN2			},
	{"아가씨 (평민) 1"		,"LADY1.sad"				,dBODY_NPC_LADY1				},
	{"아가씨 (평민) 2"		,"LADY2.sad"				,dBODY_NPC_LADY2				},
	{"할아버지 (평민)"		,"GRAND_FATHER.sad"			,dBODY_NPC_GRAND_FATHER			},
	{"할머지 (평민)"		,"GRAND_MOTHER.sad"			,dBODY_NPC_GRAND_MOTHER			},
	{"남아 (평민) 1"		,"BOY1.sad"					,dBODY_NPC_BOY1					},
	{"남아 (평민) 2"		,"BOY2.sad"					,dBODY_NPC_BOY2					},
	{"여아 (평민) 1"		,"GIRL1.sad"				,dBODY_NPC_GIRL1				},
	{"여아 (평민) 2"		,"GIRL2.sad"				,dBODY_NPC_GIRL2				},
	{"귀족 남자 1"			,"NOBLE_MAN1.sad"			,dBODY_NPC_NOBLE_MAN1			},
	{"귀족 남자 2"			,"NOBLE_MAN2.sad"			,dBODY_NPC_NOBLE_MAN2			},
	{"귀족 부인 1"			,"NOBLE_WOMAN1.sad"			,dBODY_NPC_NOBLE_WOMAN1			},
	{"귀족 부인 2"			,"NOBLE_WOMAN2.sad"			,dBODY_NPC_NOBLE_WOMAN2			},
	{"남아 (귀족)"			,"NOBLE_BOY.sad"			,dBODY_NPC_NOBLE_BOY			},
	{"여아 (귀족)"			,"NOBLE_GIRL.sad"			,dBODY_NPC_NOBLE_GIRL			},
	{"집시 (남) 1"			,"GYPSY_MAN1.sad"			,dBODY_NPC_GYPSY_MAN1			},
	{"집시 (남) 2"			,"GYPSY_MAN2.sad"			,dBODY_NPC_GYPSY_MAN2			},
	{"집시 (여) 1"			,"GYPSY_WOMAN1.sad"			,dBODY_NPC_GYPSY_WOMAN1			},
	{"집시 (여) 2"			,"GYPSY_WOMAN2.sad"			,dBODY_NPC_GYPSY_WOMAN2			},
	{"집시 (남아) 1"		,"GYPSY_BOY1.sad"			,dBODY_NPC_GYPSY_BOY1			},
	{"집시 (남아) 2"		,"GYPSY_BOY2.sad"			,dBODY_NPC_GYPSY_BOY2			},
	{"집시 (여아) 1"		,"GYPSY_GIRL1.sad"			,dBODY_NPC_GYPSY_GIRL1			},
	{"집시 (여아) 2"		,"GYPSY_GIRL2.sad"			,dBODY_NPC_GYPSY_GIRL2			},
	{"농부 1"				,"FARMER1.sad"				,dBODY_NPC_FARMER1				},
	{"농부 2"				,"FARMER2.sad"				,dBODY_NPC_FARMER2				},
	{"나무꾼 1"				,"WOODCUTTER1.sad"			,dBODY_NPC_WOODCUTTER1			},
	{"나무꾼 2"				,"WOODCUTTER2.sad"			,dBODY_NPC_WOODCUTTER2			},
	{"광부"					,"MINER.sad"				,dBODY_NPC_MINER				},
	{"선원"					,"SAILER.sad"				,dBODY_NPC_SAILER				},
	{"도끼창병"				,"HALBERD_MAN.sad"			,dBODY_NPC_HALBERD_MAN			},
	{"성기사 (남)"			,"TEMPLE_KNIGHT_M.sad"		,dBODY_NPC_TEMPLE_KNIGHT_M		},
	{"성기사 (여)"			,"TEMPLE_KNIGHT_F.sad"		,dBODY_NPC_TEMPLE_KNIGHT_F		},
	{"주교"					,"BISHOP.sad"				,dBODY_NPC_BISHOP				},
	{"신부"					,"PRIEST.sad"				,dBODY_NPC_PRIEST				},
	{"수녀"					,"NUN.sad"					,dBODY_NPC_NUN					},
	{"용병 (남)"			,"MERCENARY_SOLDIER_M.sad"	,dBODY_NPC_MERCENARY_SOLDIER_M	},
	{"용병 (여)"			,"MERCENARY_SOLDIER_F.sad"	,dBODY_NPC_MERCENARY_SOLDIER_F	},
	{"거지"					,"BAGGER.sad"				,dBODY_NPC_BAGGER				},
	{"사막 무희 (여자)"		,"DANCING_GIRL.sad"			,dBODY_NPC_DANCING_GIRL			},
	{"사관학교 남교관"		,"INSTRUCTOR_M.sad"			,dBODY_NPC_INSTRUCTOR_M			},
	{"사관학교 여교관"		,"INSTRUCTOR_F.sad"			,dBODY_NPC_INSTRUCTOR_F			},
	{"사관 생도 1"			,"CADET1.sad"				,dBODY_NPC_CADET1				},
	{"사관 생도 2"			,"CADET2.sad"				,dBODY_NPC_CADET2				},
	{"마법사 (남자 노인)"	,"WIZARD.sad"				,dBODY_NPC_WIZARD				},
	{"마법사 (남자)"		,"MAGICIAN_M.sad"			,dBODY_NPC_MAGICIAN_M			},
	{"마법사 (여자)"		,"MAGICIAN_F.sad"			,dBODY_NPC_MAGICIAN_F			},
	{"무기점 주인 (남)"		,"SHOPKEEPER_WEAPON.sad"	,dBODY_NPC_SHOPKEEPER_WEAPON	},
	{"잡화점 주인 (남)"		,"GROCER_M.sad"				,dBODY_NPC_GROCER_M				},
	{"잡화점 주인 (여)"		,"GROCER_F.sad"				,dBODY_NPC_GROCER_F				},
	{"노점상 주인 (남)"		,"PITCHMAN_M.sad"			,dBODY_NPC_PITCHMAN_M			},
	{"노점상 주인 (여)"		,"PITCHMAN_F.sad"			,dBODY_NPC_PITCHMAN_F			},
	{"노점상 주인 (남노)"	,"PITCHMAN_GRAND_FATHER.sad",dBODY_NPC_PITCHMAN_GRAND_FATHER},
	{"노점상 주인 (여노)"	,"PITCHMAN_GRAND_MOTHER.sad",dBODY_NPC_PITCHMAN_GRAND_MOTHER},
	{"천사"					,"ANGEL.sad"				,dBODY_NPC_ANGEL				},
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
	{"해골전사(도끼)"	,	"SkeletonAxe.sad",dBODY_MONSTER_SKELETON_AXE},//	
	{"해골전사(쌍칼)"	,	"SkeletonSword.sad",dBODY_MONSTER_SKELETON_SWORD},//	
	{"해골기사"			,	"SkeletonKnight.sad",dBODY_MONSTER_SKELETON_KNIGHT},//	
	{"좀비"				,	"Zombie.sad",dBODY_MONSTER_ZOMBIE},//	
	{"산송장"			,	"LivingDead.sad",dBODY_MONSTER_LIVINNG_DEAD},//	
	{"유령"				,	"Ghost.sad",dBODY_MONSTER_GHOST},//	
	{"미이라"			,	"Mummy.sad",dBODY_MONSTER_MUMMY},//	
	{"고스트 아머"		,	"GhostArmor.sad",dBODY_MONSTER_GHOST_ARMOR},//	
	{"뱀파이어"			,	"Vampire.sad",dBODY_MONSTER_VAMPIRE},//	
	{"리치"				,	"Lich.sad",dBODY_MONSTER_LICH},//	

	{"블랙 엘프"		,	"DarkElf.sad",dBODY_MONSTER_DARK_ELF},//	
	{"어새신"			,	"Assassin.sad",dBODY_MONSTER_ASSASSIN},//	
	{"원주민"			,	"AggressiveNative.sad",dBODY_MONSTER_AGGRESSIVE_NATIVE},//	
	{"타락한 마법사"	,	"FallenWizard.sad",dBODY_MONSTER_FALLEN_WIZARD},//	
	{"궁정 기사단"		,	"TempleKnight.sad",dBODY_MONSTER_TEMPLE_KNIGHT},//	
	{"다크 프리스트"	,	"DarkPriest.sad",dBODY_MONSTER_DARK_PRIEST},//	
	{"어둠의 소환사"	,	"Conjuerer.sad",dBODY_MONSTER_CONJUERER},//	
	{"자이언트"			,	"Giant.sad",dBODY_MONSTER_GIANT},//	
	{"엘프 킹"			,	"ElfKing.sad",dBODY_MONSTER_ELF_KING},//	
	{"대마술사"			,	"Archmage.sad",dBODY_MONSTER_ARCHMAGE},//	

	{"데미 비홀더"		,	"DemiBeholder.sad",dBODY_MONSTER_DEMI_BEHOLDER},//	
	{"렙타일"			,	"Reptile.sad",dBODY_MONSTER_REPTILE},//	
	{"닭 전사"			,	"CockFighter.sad",dBODY_MONSTER_COCK_FIGHTER},//	
	{"리자드 워리어"	,	"LizardWarrior.sad",dBODY_MONSTER_LIZARD_WARRIOR},//	
	{"리자드 라이더"	,	"LizardRider.sad",dBODY_MONSTER_LIZARD_RIDER},//	
	{"랫 파이터"		,	"RatFighter.sad",dBODY_MONSTER_RAT_FIGHTER},//	
	{"데몬"				,	"Demon.sad",dBODY_MONSTER_DEMON},//	
	{"오우거"			,	"Ogre.sad",dBODY_MONSTER_OGRE},//	
	{"바호매트"			,	"Bahomate.sad",dBODY_MONSTER_BAHOMATE},//	
	{"붉은 악마"		,	"RedDemon.sad",dBODY_MONSTER_RED_DEMON},//	

	{"터틀 드래곤"		,	"TurtleDragon.sad",dBODY_MONSTER_TURTLE_DRAGON},//	
	{"거미"				,	"Spider.sad",dBODY_MONSTER_SPIDER},//	
	{"전갈"				,	"Scorpion.sad",dBODY_MONSTER_SCORPION},//	
	{"해파리"			,	"JellyFish.sad",dBODY_MONSTER_JELLYFISH},//	
	{"웜"				,	"GiantWorm.sad",dBODY_MONSTER_WORM},//	
	{"킹 크랩"			,	"KingCrab.sad",dBODY_MONSTER_KING_CRAB},//	
	{"곤충 떼"			,	"InsectSwarm.sad",dBODY_MONSTER_INSECT_SWARM},//	
	{"늑대"				,	"Wolf.sad",dBODY_MONSTER_WOLF},//	
	{"큰 두더지"		,	"BigMole.sad",dBODY_MONSTER_BIG_MOLE},//	
	{"미친 곰"			,	"MadBear.sad",dBODY_MONSTER_MAD_BEAR},//	

	{"불도깨비"			,	"FireBogy.sad",dBODY_MONSTER_FIRE_BOGY},//	
	{"혓바닥눈"			,	"TongueEye.sad",dBODY_MONSTER_TONGUE_EYE},//	
	{"가고일"			,	"Gargoyle.sad",dBODY_MONSTER_GARGOYLE},//	
	{"메탈 골렘"		,	"MetalGolem.sad",dBODY_MONSTER_METAL_GOLEM},//	
	{"켄타우로스"		,	"Centaurs.sad",dBODY_MONSTER_CENTAURS},//	
	{"머맨 나이트"		,	"MermanKnight.sad",dBODY_MONSTER_MERMAN_KNIGHT},//	
	{"팀버맨"			,	"TimberMan.sad",dBODY_MONSTER_TIMBER_MAN},//	
	{"사라만다"			,	"Salamander.sad",dBODY_MONSTER_SALAMANDER},//	
	{"다크 파이어"		,	"DarkFire.sad",dBODY_MONSTER_DARK_FIRE},//	
	{"화이트 섀도우"	,	"WhiteShadow.sad",dBODY_MONSTER_WHITE_SHADOW},//	

	{"해골 산타"		,	"SkeletonSanta.sad",dBODY_MONSTER_SKELETON_SANTA},//	

	{"세이지"			,	"sage.sad",dBODY_MONSTER_SAGE},//	
	{"세이지 마스터"	,	"sage_master.sad",dBODY_MONSTER_SAGE_MASTER},
	{"티아메스(왼쪽위 촉수)",	"tiamath_left_feeler.sad",dBODY_MONSTER_TIAMATH_FEELER},
	{"티아메스(본체)"	,	"tiamath.sad",dBODY_MONSTER_TIAMATH},
	{"티아메스(오른쪽 촉수)",	"tiamath_right_feeler.sad",dBODY_MONSTER_TIAMATH_BOTTOM_FEELER},
	{"워터 드래곤"		,	"water_dragon.sad",dBODY_MONSTER_WATER_DRAGON},
	{"아크 데빌"		,	"arch_devil.sad",dBODY_MONSTER_ARCH_DEVIL},
	{"드라코 리치"		,	"draco_lich.sad",dBODY_MONSTER_DRACO_LICH},
	{"드라코 리치 오브"	,	"draco_lich_orb.sad",dBODY_MONSTER_DRACO_LICH_ORB},

	{"자이언트해골"		,	"Giant_Skull.sad",dBODY_MONSTER_Giant_Skull		},
	{"해골 마법사"		,	"Undead_Magician.sad",dBODY_MONSTER_Undead_Magician	},
	{"본 키메라"		,	"Bone_Chimera.sad",dBODY_MONSTER_Bone_Chimera		},
	{"하이 엘프"		,	"High_Elf.sad",dBODY_MONSTER_High_Elf			},
	{"깡패"				,	"Gangster.sad",dBODY_MONSTER_Gangster			},
	{"시크 교도"		,	"Sikhs.sad",dBODY_MONSTER_Sikhs				},
	{"탈주자"			,	"Escapee.sad",dBODY_MONSTER_Escapee			},
	{"광마"				,	"Mad_Demon.sad",dBODY_MONSTER_Mad_Demon			},
	{"사신"				,	"God_of_Death.sad",dBODY_MONSTER_God_of_Death		},
	{"서큐버스"			,	"Succubus.sad",dBODY_MONSTER_Succubus			},
	{"맨티스 전사"		,	"Mantis_Warrior.sad",dBODY_MONSTER_Mantis_Warrior	},
	{"펌프킨헤드"		,	"Pumpkin_Head.sad",dBODY_MONSTER_Pumpkin_Head		},
	{"대왕 원숭이"		,	"Monkey_King.sad",dBODY_MONSTER_Monkey_King		},
	{"거대 개구리"		,	"Giant_Frog.sad",dBODY_MONSTER_Giant_Frog		},
	{"혀괴수"			,	"Tongue_Monster.sad",dBODY_MONSTER_Tongue_Monster	},
	{"다크 버팔로"		,	"Dark_Buffalo.sad",dBODY_MONSTER_Dark_Buffalo		},
	{"유니콘"			,	"Unicorn.sad",dBODY_MONSTER_Unicorn			},
	{"사도"				,	"Apostles.sad",dBODY_MONSTER_Apostles			},
	{"케루빔"			,	"Cherubim.sad",dBODY_MONSTER_Cherubim			},

	{"길드 문장"		,	"guild_crest.sad",dBODY_MONSTER_Guild_Crest},
	{"실드 크래프트"	,	"shield_craft.sad",		dBODY_MONSTER_ShieldCraft		},
	{"엘레멘탈 포스트"	,	"elemantal_post.sad",dBODY_MONSTER_ElelementalPost	},
	{"에테르 쉘"		,	"ether_shell.sad",dBODY_MONSTER_EtherShell		},
	{"할로우 나이트"	,	"hollow_knight.sad",dBODY_MONSTER_HollowKnight	},	
	{"전략 깃발"		,	"guild_flag.sad",dBODY_MONSTER_GuildFlag	},	

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