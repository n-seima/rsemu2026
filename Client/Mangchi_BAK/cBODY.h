#ifndef _classBODY_H
#define _classBODY_H

#include "SFC.H"
#include "cIMAGE.H"

#define	dBODY_COUNT									300
#define	dMONSTER_VARIATION_COUNT					4

#define	dBODY_NPC_START								101


#define	dBODY_NPC_MAN1								101		//	아저씨 (평민) 1				평범한 결혼한 남자 스타일. 범용.	기존
#define	dBODY_NPC_MAN2								102		//	아저씨 (평민) 2				평범한 결혼한 남자 스타일. 옷 모양이 좀 다르다. 범용.	
#define	dBODY_NPC_WOMAN1							103		//	부인 (평민) 1				평범한 결혼한 여자. 범용. 옷 모양이 좀 다름	기존
#define	dBODY_NPC_WOMAN2							104		//	부인 (평민) 2				평범한 결혼한 여자. 범용. 옷 모양이 좀 다름	
#define	dBODY_NPC_YOUNG_MAN1						105		//	청년 1 (평민)				결혼 안 한 청년	기존
#define	dBODY_NPC_YOUNG_MAN2						106		//	청년 2 (평민)				결혼 안 한 청년	기존
#define	dBODY_NPC_LADY1								107		//	아가씨 (평민) 1				결혼 안 한 처녀.	기존
#define	dBODY_NPC_LADY2								108		//	아가씨 (평민) 2				결혼 안 한 처녀. 옷이 다름	
#define	dBODY_NPC_GRAND_FATHER						109		//	할아버지 (평민)				노인네.	기존
#define	dBODY_NPC_GRAND_MOTHER						110		//	할머지 (평민)				나이든 여인.	
#define	dBODY_NPC_BOY1								111		//	남아 (평민) 1				남자 아이. 중산층	기존
#define	dBODY_NPC_BOY2								112		//	남아 (평민) 2				남자 아이. 중산층	
#define	dBODY_NPC_GIRL1								113		//	여아 (평민) 1				여자 아이. 중산층	
#define	dBODY_NPC_GIRL2								114		//	여아 (평민) 2				여자 아이. 중산층	
#define	dBODY_NPC_NOBLE_MAN1						115		//	귀족 남자 1				귀족 남자. 나이가 좀 들었음	기존
#define	dBODY_NPC_NOBLE_MAN2						116		//	귀족 남자 2				귀족 남자. 젋은 나이	기존
#define	dBODY_NPC_NOBLE_WOMAN1						117		//	귀족 부인 1				우아한 귀족 부인	기존
#define	dBODY_NPC_NOBLE_WOMAN2						118		//	귀족 부인 2				우아한 귀족 부인. 나이가 약간 들었음.	
#define	dBODY_NPC_NOBLE_BOY							119		//	남아 (귀족)				귀족 자녀. 잘 입었음	기존
#define	dBODY_NPC_NOBLE_GIRL						120		//	여아 (귀족)				귀족 자녀. 잘 입었음	
#define	dBODY_NPC_GYPSY_MAN1						121		//	집시 (남) 1				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
#define	dBODY_NPC_GYPSY_MAN2						122		//	집시 (남) 2				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
#define	dBODY_NPC_GYPSY_WOMAN1						123		//	집시 (여) 1				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
#define	dBODY_NPC_GYPSY_WOMAN2						124		//	집시 (여) 2				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
#define	dBODY_NPC_GYPSY_BOY1						125		//	집시 (남아) 1				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
#define	dBODY_NPC_GYPSY_BOY2						126		//	집시 (남아) 2				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
#define	dBODY_NPC_GYPSY_GIRL1						127		//	집시 (여아) 1				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
#define	dBODY_NPC_GYPSY_GIRL2						128		//	집시 (여아) 2				거지 같은 느낌이 아닌 집시 전통의 화려한 의상을 입었음	
#define	dBODY_NPC_FARMER1							129		//	농부 1				밀짚 모자에 쟁기를 든 모습	
#define	dBODY_NPC_FARMER2							130		//	농부 2				팔을 걷어 붙이고 있는 농부의 모습	
#define	dBODY_NPC_WOODCUTTER1						131		//	나무꾼 1				도끼든 모습	
#define	dBODY_NPC_WOODCUTTER2						132		//	나무꾼 2				나무짐을 지고 있음	
#define	dBODY_NPC_MINER								133		//	광부					기존
#define	dBODY_NPC_SAILER							134		//	선원					기존
#define	dBODY_NPC_HALBERD_MAN						135		//	도끼창병					기존
#define	dBODY_NPC_TEMPLE_KNIGHT_M					136		//	성기사 (남)					기존
#define	dBODY_NPC_TEMPLE_KNIGHT_F					137		//	성기사 (여)					기존
#define	dBODY_NPC_BISHOP							138		//	주교					기존
#define	dBODY_NPC_PRIEST							139		//	신부					기존
#define	dBODY_NPC_NUN								140		//	수녀					기존
#define	dBODY_NPC_MERCENARY_SOLDIER_M				141		//	용병 (남)					기존
#define	dBODY_NPC_MERCENARY_SOLDIER_F				142		//	용병 (여)					기존
#define	dBODY_NPC_BAGGER							143		//	거지					기존
#define	dBODY_NPC_DANCING_GIRL						144		//	사막 무희 (여자)					기존
#define	dBODY_NPC_INSTRUCTOR_M						145		//	사관학교 남교관					
#define	dBODY_NPC_INSTRUCTOR_F						146		//	사관학교 여교관					기존
#define	dBODY_NPC_CADET1							147		//	사관 생도 1					기존
#define	dBODY_NPC_CADET2							148		//	사관 생도 2					
#define	dBODY_NPC_WIZARD							149		//	마법사 (남자 노인)				뾰족 모자 쓴 전형적인 노마법사, 레드 아이 단원 겸용	
#define	dBODY_NPC_MAGICIAN_M						150		//	마법사 (남자)				젊은 마법사, 레드 아이 단원 겸용	
#define	dBODY_NPC_MAGICIAN_F						151		//	마법사 (여자)				젊은 마법사, 좀 노출이 있게, 레드 아이 단원 겸용	
#define	dBODY_NPC_SHOPKEEPER_WEAPON					152		//	무기점 주인 (남)				우락부락한 대장장이 느낌	
#define	dBODY_NPC_GROCER_M							153		//	잡화점 주인 (남)				전형적인 상점 주인	
#define	dBODY_NPC_GROCER_F							154		//	잡화점 주인 (여)				전형적인 상점 주인	
#define	dBODY_NPC_PITCHMAN_M						155		//	노점상 주인 (남)					
#define	dBODY_NPC_PITCHMAN_F						156		//	노점상 주인 (여)					
#define	dBODY_NPC_PITCHMAN_GRAND_FATHER				157		//	노점상 주인 (남노)					
#define	dBODY_NPC_PITCHMAN_GRAND_MOTHER				158		//	노점상 주인 (여노)					
#define	dBODY_NPC_ANGEL								159		//	천사					



////////////////////////////////////////////////////////////////////////////////////////////
//	몬스터
////////////////////////////////////////////////////////////////////////////////////////////
#define	dBODY_MONSTER_START							201

#define	dBODY_MONSTER_SKELETON_AXE					201		//	언데드	해골전사 도끼	Skeleton, Axe		175㎝		기존
#define	dBODY_MONSTER_SKELETON_SWORD				202		//	언데드	해골전사 쌍칼	Skeleton, Sword		175㎝		기존
#define	dBODY_MONSTER_SKELETON_KNIGHT				203		//	언데드	해골기사	Skeleton Knight		190㎝		기존
#define	dBODY_MONSTER_ZOMBIE						204		//	언데드	좀비	Zombie		170㎝		기존
#define	dBODY_MONSTER_LIVINNG_DEAD					205		//	언데드	산송장	Living Dead		155㎝		기존
#define	dBODY_MONSTER_GHOST							206		//	언데드	유령	Ghost		180㎝		기존
#define	dBODY_MONSTER_MUMMY							207		//	언데드	미이라	Mummy		200㎝		기존
#define	dBODY_MONSTER_GHOST_ARMOR					208		//	언데드	고스트 아머	Ghost Armor		250㎝		기존
#define	dBODY_MONSTER_VAMPIRE						209		//	언데드	뱀파이어	Vampire		180㎝	흡혈귀, 세미보스다운 귀족적 스타일	세미보스
#define	dBODY_MONSTER_LICH							210		//	언데드	리치	Lich		210㎝	공중 부양, 허리 밑은 없음. 머리부터 허리까지는 100㎝	보스

#define	dBODY_MONSTER_DARK_ELF						211		//	인간형	블랙 엘프	Black Elf		160㎝		기존
#define	dBODY_MONSTER_ASSASSIN						212		//	인간형	어새신	Assassin		165㎝	도적 스타일	기존
#define	dBODY_MONSTER_AGGRESSIVE_NATIVE				213		//	인간형	원주민			80㎝	부쉬맨 스타일의 작은 원주민, 무기는 몽둥이	
#define	dBODY_MONSTER_FALLEN_WIZARD					214		//	인간형	타락한 마법사	Fallen Wizard		145㎝	구부정한 곱사등 스타일, 구불구불한 스태프 소지	
#define	dBODY_MONSTER_TEMPLE_KNIGHT					215		//	인간형	궁정 기사단	Temple Knight		195㎝	장신, 멋진 풀 플레이트 메일에 할버드	
#define	dBODY_MONSTER_DARK_PRIEST					216		//	인간형	다크 프리스트	Dark Priest		160㎝	회복과 보조 마법 전문의 성직자	
#define	dBODY_MONSTER_CONJUERER						217		//	인간형	어둠의 소환사	Conjuerer		165㎝	채찍으로 무장	
#define	dBODY_MONSTER_GIANT							218		//	인간형	자이언트	Giant		230㎝		세미 보스, 기존
#define	dBODY_MONSTER_ELF_KING						219		//	인간형	엘프 킹	Elf King		175㎝	칼과 롱보우 소지	보스
#define	dBODY_MONSTER_ARCHMAGE						220		//	인간형	대마술사	Archmage		180㎝	장신의 마법사, 레드아이 보스로 등장 예정	보스

#define	dBODY_MONSTER_DEMI_BEHOLDER					221		//	악마형	데미 비홀더	Demi Beholder		90㎝		기존
#define	dBODY_MONSTER_REPTILE						222		//	악마형	렙타일	Reptile		120㎝		기존
#define	dBODY_MONSTER_COCK_FIGHTER					223		//	악마형	닭 전사	Cock Fighter		175㎝		기존
#define	dBODY_MONSTER_LIZARD_WARRIOR				224		//	악마형	리자드 워리어	Lizard Warrior		155㎝		기존
#define	dBODY_MONSTER_LIZARD_RIDER					225		//	악마형	리자드 라이더	Lizard Rider		180㎝		기존
#define	dBODY_MONSTER_RAT_FIGHTER					226		//	악마형	랫 파이터	Rat Fighter		140㎝		기존
#define	dBODY_MONSTER_DEMON							227		//	악마형	데몬	Demon		185㎝		세미 보스, 기존
#define	dBODY_MONSTER_OGRE							228		//	악마형	오우거	Ogre		210㎝		세미 보스, 기존
#define	dBODY_MONSTER_BAHOMATE						229		//	악마형	바호매트			210㎝	염소 파충류라는 것.	세미 보스, 기존
#define	dBODY_MONSTER_RED_DEMON						230		//	악마형	붉은 악마	Red Demon		300㎝	최종 보스 다운 거대함과 위용을 보여줄 것	최종 보스

#define	dBODY_MONSTER_TURTLE_DRAGON					231		//	동물형	터틀 드래곤	Turtle Dragon		190㎝		기존
#define	dBODY_MONSTER_SPIDER						232		//	동물형	거미	Spider		130㎝		기존
#define	dBODY_MONSTER_SCORPION						233		//	동물형	전갈	Scorpion		180㎝		기존
#define	dBODY_MONSTER_JELLYFISH						234		//	동물형	해파리			170㎝		기존
#define	dBODY_MONSTER_WORM							235		//	동물형	웜	Worm		170㎝(길이)		기존
#define	dBODY_MONSTER_KING_CRAB						236		//	동물형	킹 크랩	King Crab		50㎝	키는 작지만 그 넓이까지 합치면 꽤 크다. 옆으로 걸을 것	
#define	dBODY_MONSTER_INSECT_SWARM					237		//	동물형	곤충 떼	Insect Swarm		120㎝	곤충 떼. 점으로만 이루어져도 될 듯	
#define	dBODY_MONSTER_WOLF							238		//	동물형	늑대	Wolf		50㎝	길이는 150㎝~170㎝ 정도 된다. 거대한 늑대	
#define	dBODY_MONSTER_BIG_MOLE						239		//	동물형	큰 두더지	Big Moul		400㎝(팔길이)		세미보스, 기존
#define	dBODY_MONSTER_MAD_BEAR						240		//	동물형	미친 곰	Mad Bear		250㎝(길이)	이동시에는 네 발로 걷고 공격할 때는 서서 앞 발로.	세미보스

#define	dBODY_MONSTER_FIRE_BOGY						241		//	신수형	불도깨비			150㎝	불의 신수	기존
#define	dBODY_MONSTER_TONGUE_EYE					242		//	신수형	혓바닥눈			200㎝	물의 신수	기존
#define	dBODY_MONSTER_GARGOYLE						243		//	신수형	가고일	Gargoyle		155㎝	바람의 신수	기존
#define	dBODY_MONSTER_METAL_GOLEM					244		//	신수형	메탈 골렘	Metal Golem		220㎝	땅의 신수	기존
#define	dBODY_MONSTER_CENTAURS						245		//	신수형	켄타우로스	Centaurs		210㎝	바람의 신수	기존
#define	dBODY_MONSTER_MERMAN_KNIGHT					246		//	신수형	머맨 나이트	Merman Knight		220㎝	물의 신수	기존
#define	dBODY_MONSTER_TIMBER_MAN					247		//	신수형	팀버맨	Timber Man		230㎝	땅의 신수	기존
#define	dBODY_MONSTER_SALAMANDER					248		//	신수형	사라만다	Salamander		100㎝	불의 신수, 불 도마뱀	
#define	dBODY_MONSTER_DARK_FIRE						249		//	신수형	다크 파이어	Dark Fire		120㎝	어둠의 신수, 검은 불로 둘러싸인 괴물 분위기	세미 보스
#define	dBODY_MONSTER_WHITE_SHADOW					250		//	신수형	화이트 섀도우	White Shadow		140㎝	빛의 신수, 몸집이 작은 어린 아이 같은 분위기	보스
#define	dBODY_MONSTER_SKELETON_SANTA				251		//	해골 산타

#define	dBODY_MONSTER_SAGE							252		//	세이지
#define	dBODY_MONSTER_SAGE_MASTER					253		//	세이지 마스터	,
#define	dBODY_MONSTER_TIAMATH_FEELER				254		//	티아메스(위 촉수),
#define	dBODY_MONSTER_TIAMATH						255		//	티아메스(본체)	,
#define	dBODY_MONSTER_TIAMATH_BOTTOM_FEELER			256		//	티아메스(아래 촉수)
#define	dBODY_MONSTER_WATER_DRAGON					257		//	워터 드래곤		,
#define	dBODY_MONSTER_ARCH_DEVIL					258		//	아크 데빌		,
#define	dBODY_MONSTER_DRACO_LICH					259		//	드라코 리치		,
#define	dBODY_MONSTER_DRACO_LICH_ORB				260		//	드라코 리치 오브	,

#define	dBODY_MONSTER_Giant_Skull					261		//		{"자이언트해골"
#define	dBODY_MONSTER_Undead_Magician				262		//		{"해골 마법사"
#define	dBODY_MONSTER_Bone_Chimera					263		//		{"본 키메라"
#define	dBODY_MONSTER_High_Elf						264		//		{"하이 엘프"
#define	dBODY_MONSTER_Gangster						265		//		{"깡패"
#define	dBODY_MONSTER_Sikhs							266		//		{"시크 교도"
#define	dBODY_MONSTER_Escapee						267		//		{"탈주자"
#define	dBODY_MONSTER_Mad_Demon						268		//		{"광마"
#define	dBODY_MONSTER_God_of_Death					269		//		{"사신"
#define	dBODY_MONSTER_Succubus						270		//		{"서큐버스"
#define	dBODY_MONSTER_Mantis_Warrior				271		//		{"맨티스 전사"
#define	dBODY_MONSTER_Pumpkin_Head					272		//		{"펌프킨헤드"
#define	dBODY_MONSTER_Monkey_King					273		//		{"대왕 원숭이"
#define	dBODY_MONSTER_Giant_Frog					274		//		{"거대 개구리"
#define	dBODY_MONSTER_Tongue_Monster				275		//		{"혀괴수"
#define	dBODY_MONSTER_Dark_Buffalo					276		//		{"다크 버팔로"
#define	dBODY_MONSTER_Unicorn						277		//		{"유니콘"
#define	dBODY_MONSTER_Apostles						278		//		{"사도"
#define	dBODY_MONSTER_Cherubim						279		//		{"케루빔"
#define	dBODY_MONSTER_Guild_Crest					280		//		{"길드 문장"

#define	dBODY_MONSTER_ShieldCraft					281		//		{"쉴드 크래프트"
#define	dBODY_MONSTER_ElelementalPost				282		//		{"엘레멘틀 포스트"
#define	dBODY_MONSTER_EtherShell					283		//		{"에테르 쉘"
#define	dBODY_MONSTER_HollowKnight					284		//		{"할로우 나이트"

#define	dBODY_MONSTER_GuildFlag						285		//		{"길드 깃발

#define	dBODY_VALID									0x7fff

#define	dLOAD_ACTION								2		//	툴에서 쓰기위해 로딩할 액션

class	cBODY_DATA
{
public:
	char			m_strName[32];						//	직업 이름
	char			m_strFileName[32];					//	화일 이름
	int				m_iBody;							//	인덱스
};

class	cBODY
{
public:
	int				m_iIndex;
	cBODY_DATA		*m_lpBodyList;
	char			m_strName[32];						//	직업 이름
	int				m_iBody;
	int				m_iCount;

					cBODY()
					{
						m_iIndex	=	0xffff;
						m_iBody		=	0xffff;
						m_iCount	=	0;
					}
};

class	CPaletteData
{
public:
	WORD			m_aData[256];
	char			m_strName[32];
};

class	CPalette
{
public:
	WORD			m_wSerial;
	WORD			m_wCount;
	CPaletteData	*m_pPalette;

					CPalette()
					{
						m_wSerial	=	0xffff;
						m_wCount	=	0;
						m_pPalette	=	NULL;
					}

					~CPalette()
					{
						pKILL(m_pPalette);
					}
};

//
//	인덱스가 일정치 않은 몸뚱아리 리스트를 관리하기 위해서..
class	cBODY_LIST
{
public:
	cBODY			m_aBody[dBODY_COUNT];
	int				m_iBodyCount;
	int				m_iNpcBodyCount,m_iMonsterBodyCount,m_iHeroBodyCount;

					cBODY_LIST();
					~cBODY_LIST();

	BOOL			init();	//	초기화
	BOOL			addBody(int _iBodyIndex,cBODY_DATA *_lpBodyDatas,int _iBody,char *_strBodyName);		//	몸통-_-추가
	BOOL			loadBody(int index,char *fn);		//	몸뚱아리 하나 불러오기
	BOOL			loadPalette(int _iIndex);
	BOOL			loadBodies();						//	몸뚱아리들을 몽땅 불러온다.
	int				getBodyIndex(int _iBody);			//	m_aBody에서 _iBody인 데이터의 인덱스를 리턴한다.
	void			arrangeData(int _iJobGroup);		//	데이터 정렬
	int				getUsedBodyCount();					//	직업에 등록된 몸뚱아리의 숫자를 리턴한다.
	int				getRegistJobCount();				//	등록된 직업수

	inline	int		getBody(int _iIndex)		{return	m_aBody[_iIndex].m_iBody;}	//	몸뚱아리 리턴
	inline	int		getJobCount(int _iJob)		{return	m_aBody[_iJob].m_iCount;}
	inline	char*	getName(int _iIndex)		{return	m_aBody[_iIndex].m_strName;}
};	//	class	cBODY_LIST


class	cMonsterBodyLoadInfo
{
public:
	WORD	m_wLoadMethodValue,m_wTameResistance;
};

extern	cMonsterBodyLoadInfo	g_aMonsterBodyLoadInfo[100];
extern	cANMQ					**g_pBody;
extern	cANMQ					*g_apMonsterVariation[dBODY_COUNT][dMONSTER_VARIATION_COUNT];

//extern	cBODY_DATA			g_aHeroBodyData[dBODY_COUNT/3];
extern	cBODY_DATA				g_aNPCBodyData[dBODY_COUNT/3];
extern	cBODY_DATA				g_aMonsterBodyData[dBODY_COUNT/3];
extern	cBODY_LIST				g_BodyList;
extern	CPalette				g_aMonsterPalette[dBODY_COUNT];

#endif