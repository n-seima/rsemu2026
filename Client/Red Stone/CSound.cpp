#include "CSound.H"


CSoundEffectInfo		g_aEffectSound[dMAX_EFFECT_SOUND_COUNT]	=	
{
	{eES_BLOCKING	,TRUE,"blocking.wav"	,"막기"},
	{eES_DODGE		,TRUE,"dodge.wav"		,"회피"},
	{eES_SWORD_HIT	,TRUE,"sword_hit.wav"	,"칼 명중"},
	{eES_SPEAR_HIT	,TRUE,"spear_hit.wav"	,"창 명중"},
	{eES_BLUNT_HIT	,TRUE,"Blunt_hit.wav"	,"둔기 명중"},
	{eES_ARROW_SHOT	,TRUE,"arrow_shot.wav"	,"화살 발사"},
	{eES_ARROW_HIT	,TRUE,"sword_hit.wav"	,"화살 명중"},
	{0xffff},
};

char	*l_strOldBgmFile[]	=	
{
	"grassland.ogg",
	"Dungeon.ogg",
	"Theme.ogg",
	"Mountain Village.ogg",
	"Cave.ogg",
	"Mountain.ogg",
	"Mine.ogg",
	"Brunenstig.ogg",
	"mola.ogg",
	"end",
};


char	*l_strNewBgmFile[]	=	
{
	"03 Grassland-Echo of Wind.ogg",
	"07 Dungeon-Cold Spirits.ogg",
	"01 Title-Legend of Red Stone.ogg",
	"05 Mountain Village-My Sweety Home.ogg",
	"04 Cave-Impression of Adventure.ogg",
	"06 Mountain-Teeth of the Earth.ogg",
	"08 Mine-Dark Stream.ogg",
	"02 Brunenstig-Old City Brunenstig.ogg",
	"09 Liberation Team-Sorrow of Pure White.ogg",
	"end",
};

char	*g_strBgmFile[]	=	
{
	"01 Title-Legend of Red Stone.ogg",
	"02 Brunenstig-Old City Brunenstig.ogg",
	"03 Grassland-Echo of Wind.ogg",
	"04 Cave-Impression of Adventure.ogg",
	"05 Mountain Village-My Sweety Home.ogg",
	"06 Mountain-Teeth of the Earth.ogg",
	"07 Dungeon-Cold Spirits.ogg",
	"08 Mine-Dark Stream.ogg",
	"09 Liberation Team-Sorrow of Pure White.ogg",
	"10 Desert-Yellow Sand, Oasis, and Life.ogg",
	"11 Dessert Village-Cactus.ogg",
	"12 Ruined City-Scar of Brick.ogg",
	"13 Savanna-Beat of Root.ogg",
	"14 Tower-Dancing Gear.ogg",
	"15 Small Town-Incongruity.ogg",
	"16 Temple-Rose Window.ogg",
	"end",
};

class	cMonsterSoundInfo
{
public:
	int		m_iSoundCount;
	char	m_strAttackSound[64];
	char	m_strDeathSound[64];
};

cMonsterSoundInfo	l_aMonsterSoundInfo[60]	=
{
	{2,	"SkeletonAxe",	"SkeletonAxe_die"},
	{2,	"SkeletonSword",	"SkeletonSword_die"},
	{2,	"SkeletonKnight",	"SkeletonKnight_die"},
	{2,	"Zombie",	"Zombie_die"},
	{2,	"LivingDead",	"LivingDead_die"},
	{2,	"Ghost",	"Ghost_die"},
	{2,	"Mummy",	"Mummy_die"},
	{2,	"GhostArmor",	"GhostArmor_die"},
	{2,	"Vampire",	"Vampire_die"},
	{2,	"Lich",	"Lich_die"},
	{2,	"DarkElf",	"DarkElf_die"},
	{2,	"Assassin",	"Assassin_die"},
	{2,	"AggressiveNative",	"AggressiveNative_die"},
	{2,	"FallenWizard",	"FallenWizard_die"},
	{2,	"TempleKnight",	"TempleKnight_die"},
	{2,	"DarkPriest",	"DarkPriest_die"},
	{2,	"Conjuerer",	"Conjuerer_die"},
	{2,	"Giant",	"Giant_die"},
	{2,	"ElfKing",	"ElfKing_die"},
	{2,	"Archmage",	"Archmage_die"},
	{2,	"DemiBeholder",	"DemiBeholder_die"},
	{2,	"Reptile",	"Reptile_die"},
	{2,	"CockFighter",	"CockFighter_die"},
	{2,	"LizardWarrior",	"LizardWarrior_die"},
	{2,	"LizardRider",	"LizardRider_die"},
	{2,	"RatFighter",	"RatFighter_die"},
	{2,	"Demon",	"Demon_die"},
	{2,	"Ogre",	"Ogre_die"},
	{2,	"Bahomate",	"Bahomate_die"},
	{2,	"RedDemon",	"RedDemon_die"},
	{2,	"TurtleDragon",	"TurtleDragon_die"},
	{2,	"Spider",	"Spider_die"},
	{2,	"Scorpion",	"Scorpion_die"},
	{2,	"JellyFish",	"JellyFish_die"},
	{2,	"GiantWorm",	"GiantWorm_die"},
	{2,	"KingCrab",	"KingCrab_die"},
	{2,	"InsectSwarm",	"InsectSwarm_die"},
	{2,	"Wolf",	"Wolf_die"},
	{2,	"BigMole",	"BigMole_die"},
	{2,	"MadBear",	"MadBear_die"},
	{2,	"FireBogy",	"FireBogy_die"},
	{2,	"TongueEye",	"TongueEye_die"},
	{2,	"Gargoyle",	"Gargoyle_die"},
	{2,	"MetalGolem",	"MetalGolem_die"},
	{2,	"Centaurs",	"Centaurs_die"},
	{2,	"MermanKnight",	"MermanKnight_die"},
	{2,	"TimberMan",	"TimberMan_die"},
	{2,	"Salamander",	"Salamander_die"},
	{2,	"DarkFire",	"DarkFire_die"},
	{2,	"WhiteShadow",	"WhiteShadow_die"},
};

CSoundManager		g_esm;

BOOL
CSoundManager::initForDev()
{
#ifdef	_IS_DEV_CLIENT
	if	(!g_bIsDevPC)
		return	TRUE;

	CDecoder	reader;

	if (!reader.Upload("dev data/scripter/bgm.txt",NULL))
		return FALSE;

	while(1)
	{
		BOOL	bIsSuccess;

		if (reader.findCharOver('[')	==	FALSE)
			break;

		int		iIndex	=	reader.getNumber(&bIsSuccess);
		int		iBgm	=	reader.getNumber(&bIsSuccess);

		if (bIsSuccess	==	FALSE	||	iIndex	>=	1024)
			break;

		m_awBgm[iIndex]	=	iBgm;
	}

	cFILE	file;

	file.Open("data/bgm.dat","wb");
	file.Write(m_awBgm,sizeof(m_awBgm));
	file.Close();
#endif

	return	TRUE;
}

BOOL
CSoundManager::init()
{
	m_iCount			=	0;
	m_iStaticSoundCount	=	0;

	SetCurrentDirectory("data/Bgm");

	int	i=0;

	while(1)
	{
		if (STRICMP(l_strOldBgmFile[i],"end")	==	0)
			break;

		MoveFile(l_strOldBgmFile[i],l_strNewBgmFile[i]);

		i++;
	}

	SetLogFolder();

	memset(m_awBgm,0,sizeof(m_awBgm));

	cFILE	file;

	if (file.Open("data/bgm.dat","rb"))
	{
		file.Read(m_awBgm,sizeof(m_awBgm));
		file.Close();
	}

	return	TRUE;
}

//
//	_iSerial 사운드를 찾는다.
int
CSoundManager::findSound(int _iSerial)
{
	for (int i=0;i<m_iCount;i++)
		if (g_aEffectSound[i].m_wSerial	==	_iSerial)
			return	i;

	return	0xffff;
}

//
//	_lpstrName를 찾는다.
int
CSoundManager::findSound(char *_lpstrName)
{
	for (int i=0;i<m_iCount;i++)
	{
		if (STRICMP(g_aEffectSound[i].m_strName,_lpstrName) ==	0)
			return	i;
	}

	return	0xffff;
}

//
//	액션 사운드 연주
void
CSoundManager::playActionSound(CSkill *_lpSkill)
{
	play(_lpSkill->m_sound.m_strAction);
}

//
//	타격 효과음 연주
void
CSoundManager::playHitSound(CSkill *_lpSkill)
{
	play(_lpSkill->m_sound.m_strHit);
}

//
//	캐스팅 효과음 연주
void
CSoundManager::playCastingSound(CSkill *_lpSkill)
{
	play(_lpSkill->m_sound.m_strCasting);
}

//
//	생성 효과음 연주
void
CSoundManager::playCreateSound(CSkill *_lpSkill)
{
	play(_lpSkill->m_sound.m_strCreate);
}

//
//	폭발
void
CSoundManager::playExplosionSound(CSkill *_lpSkill)
{
	play(_lpSkill->m_sound.m_strExplosion);
}

void
CSoundManager::playMissSound(CSkill *_lpSkill)
{
	play(_lpSkill->m_sound.m_strMiss);
}

char*
CSoundManager::getSkillSound(CSkill *_lpSkill,int _iSound)
{
	switch(_iSound)
	{
		case	eSSD_CASTING:
			return	_lpSkill->m_sound.m_strCasting;
			
		case	eSSD_ACTION:
			return	_lpSkill->m_sound.m_strAction;
			
		case	eSSD_HIT:
			return	_lpSkill->m_sound.m_strHit;
			
		case	eSSD_CREATE:
			return	_lpSkill->m_sound.m_strCreate;
			
		case	eSSD_EXPLOSION:
			return	_lpSkill->m_sound.m_strExplosion;
			
		case	eSSD_MISS:
			return	_lpSkill->m_sound.m_strMiss;
	}
	
	return	NULL;
}

void
CSoundManager::playSEE_SuccessSound(CSkill *_lpSkill)
{
	char	*lpSoundFile	=	getSkillSound(_lpSkill,_lpSkill->m_sound.m_wSEE_Success);

	play(lpSoundFile);
}

void
CSoundManager::playSEE_MissSound(CSkill *_lpSkill)
{
	char	*lpSoundFile	=	getSkillSound(_lpSkill,_lpSkill->m_sound.m_wSEE_Miss);

	play(lpSoundFile);
}

//
//	_iSerial 사운드 연주
void
CSoundManager::play(char *_lpstrFileName)
{
	if	(!m_bIsActive	||	_lpstrFileName	==	NULL)
		return;

	if	(_lpstrFileName[0]	==	0)
		return;

	char	strFullPath[1024];

	sprintf(strFullPath,"data/sound/%s",_lpstrFileName);

	s_soundPlayer.play(strFullPath);

	SetLogFolder();
}

//
//	_iSerial 사운드 연주
void
CSoundManager::play(int _iSerial)
{
/*	if (!m_bIsActive)	return;

	int	iSoundIndex	=	findSound(_iSerial);

	if(iSoundIndex	==	0xffff)	return;

	SetCurrentDirectory("data/sound");

	if (g_aEffectSound[iSoundIndex].m_isStatic)	s_soundPlayer.playSound(g_aEffectSound[iSoundIndex].m_wStaticIndex);
	else										s_soundPlayer.playSound(g_aEffectSound[iSoundIndex].m_strFileName);

	SetLogFolder();
	*/
}	//	CSoundManager::play(int _iIndex)


void
CSoundManager::playMonsterAttackSound(int _iIndex)
{
	if (!m_bIsActive)
		return;

	if (random(100) > 20)
		return;

	char	strFullPath[1024];

	int	iValue	=	random(2);

	sprintf(strFullPath,"data/sound/monster/%s%d.wav",l_aMonsterSoundInfo[_iIndex].m_strAttackSound,iValue);

	if (!IsFile(strFullPath))
		sprintf(strFullPath,"data/sound/monster/%s0.wav",l_aMonsterSoundInfo[_iIndex].m_strAttackSound);

	s_soundPlayer.play(strFullPath);
}

void
CSoundManager::playMonsterDeathSound(int _iIndex)
{
	if (!m_bIsActive)
		return;

	char	strFullPath[1024];

	sprintf(strFullPath,"data/sound/monster/%s.wav",l_aMonsterSoundInfo[_iIndex].m_strDeathSound);

	s_soundPlayer.play(strFullPath);
}
