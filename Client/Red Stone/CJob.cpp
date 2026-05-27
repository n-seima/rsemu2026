#include "cJOB.H"
#include "cMESSAGE.H"
#include "CImageData.H"
#include "CMain.H"

CJobManager			g_jm;

void
CJobInfo::setData(cJOB*	_lpJob)
{
	strcpy(m_strName,_lpJob->m_strName);

	m_bf1IsZinMonster	=	FALSE;
	m_bf1IsExMonster	=	FALSE;

	int	iNameLength	=	strlen(m_strName);

	if	(iNameLength > 4)
	{
		char	strName[4];

		strcpy(strName,m_strName+iNameLength-3);

		strName[3]	=	NULL;

		if	(stricmp(strName,"Zin")	==	0)
			m_bf1IsZinMonster	=	TRUE;

		strcpy(strName,m_strName+iNameLength-2);

		strName[2]	=	NULL;

		if	(stricmp(strName,"Ex")	==	0)
			m_bf1IsExMonster	=	TRUE;
	}

	memset(m_awOwnedSkill,0xff,sizeof(m_awOwnedSkill));

	for	(int i=0;i<dMAX_MONSTER_ABILITY_COUNT;i++)
		m_awOwnedSkill[i]	=	_lpJob->m_aAbility[i].m_wSkill;

	m_iBody		=	_lpJob->m_wBody;

	memcpy(m_abBodyShape,_lpJob->m_abBodyShape,sizeof(m_abBodyShape));

	m_iVariation=	_lpJob->m_wVariation;
	m_wPalette	=	_lpJob->m_wPalette;
	m_wType		=	_lpJob->m_wType;//	영웅,NPC,몬스터
	m_wKind		=	_lpJob->m_wKind;
	m_wRank		=	_lpJob->m_wMonsterLevel;
	m_bf1IsEnableTameMonster	=	_lpJob->m_bf1IsEnableTameMonster;

	if	(m_wPalette	==	0xffff)
		m_wPalette	=	0;
}

CJobInfo*
CJobManager::getJob(char *_lpstrName)
{
	int	i;

	for	(i=0;i<dMAX_JOB_COUNT;i++)
	{
		if	(STRICMP(m_aJob[i].m_strName,_lpstrName)	==	0)
			return	&m_aJob[i];
	}

	return	NULL;	
}

//
//	캐릭터 생성시 부터 착용하고 있는 아이템 로딩
BOOL
CJobManager::readDefaultEquipmentData()
{
	cFILE	file;

#ifdef	_FOR_KOREA
	char	*strDataFileName	=	"data/jobData.dat";
#elif	_FOR_JAPAN
	char	*strDataFileName	=	"data/jobDataForJapan.dat";
#elif	_FOR_CHINA
	char	*strDataFileName	=	"data/JobDataForChina.dat";
#elif	_FOR_INTERNATIONAL
	char	*strDataFileName	=	"data/JobDataForInternational.dat";
#elif	_FOR_USA
	char	*strDataFileName	=	"data/JobDataForUSA.dat";
#elif	_FOR_INTER
	char	*strDataFileName	=	"data/JobDataForInter.dat";
#elif	_FOR_THAI
	char	*strDataFileName	=	"data/JobDataForThai.dat";
#endif

	if	(!file.Open(strDataFileName,"rb"))
		return	FALSE;

	file.Read(g_aPlayer1LvSetting,sizeof(g_aPlayer1LvSetting));
	file.Close();
	SetLogFolder();

#ifndef	_IS_DEV_CLIENT
	return	TRUE;
#endif

#ifndef	_FOR_KOREA
	return	TRUE;
#endif

	if	(g_bIsDevPC	==	FALSE)
		return	TRUE;

	SetLogFolder();

enum
{
	eDEFAULT_DATA	=	101,
	eDATA_WEAPON	=	102,
	eDATA_SHIELD	=	103,
	eDATA_GLOVE		=	104,
	eDATA_BULLET	=	105,
	eDATA_BELT		=	106,
	eDATA_ENABLE		,
	eDATA_DISABLE		,
	eDATA_TEST			,
	eDATA_BASIC_SKILL	,
	eDATA_SLOT0ITEM		,
};


	CDecoderKeyword	aKeyword[]	=
	{
		{	"default_data"			,eDEFAULT_DATA		},
		{	"weapon"				,eDATA_WEAPON		},
		{	"shield"				,eDATA_SHIELD		},
		{	"glove"					,eDATA_GLOVE		},
		{	"bullet"				,eDATA_BULLET		},
		{	"belt"					,eDATA_BELT			},
		{	"basic_skill"			,eDATA_BASIC_SKILL	},
		{	"slot0Item"				,eDATA_SLOT0ITEM	},
		{	"knight"				,dJOB_KNIGHT		},
		{	"warrior"				,dJOB_WARRIOR		},
		{	"wizard"				,dJOB_WIZARD		},
		{	"werewolf"				,dJOB_WEREWOLF		},
		{	"priest"				,dJOB_PRIEST		},
		{	"fallen_angel"			,dJOB_FALLEN_ANGEL	},
		{	"rogue"					,dJOB_ROGUE			},
		{	"fighter"				,dJOB_FIGHTER		},
		{	"magic_lancer"			,dJOB_MAGIC_LANCER	},
		{	"magic_archer"			,dJOB_MAGIC_ARCHER	},
		{	"beast_tamer"			,dJOB_BEAST_TAMER	},
		{	"summoner"				,dJOB_SUMMONER		},
		{	"princess"				,dJOB_PRINCESS		},
		{	"magical_girl"			,dJOB_MAGICAL_GIRL	},
		{	"necromancer"			,dJOB_NECROMANCER	},
		{	"devil"					,dJOB_DEVIL			},
		{	"SoulBringer"				,dJOB_SOUL_BRINGER	},
		{	"Champion"			,dJOB_CHAMPION	},
		{	"opticalist"			,dJOB_OPTICALIST	},
		{	"beast_man"				,dJOB_BEAST_MAN	},
		{	"enable"				,eDATA_ENABLE		},
		{	"disable"				,eDATA_DISABLE		},
		{	"test"					,eDATA_TEST			},

		{	"end"					,dCODE_END			}
	};



	char		*strScriptFileName[]	=
	{
		"dev data/scripter/job_default_data.txt",
		"dev data/scripter/job_default_data_for_japan.txt",
		"dev data/scripter/job_default_data_for_china.txt",
		"dev data/scripter/job_default_data_for_international.txt",
		"dev data/scripter/job_default_data_for_USA.txt",
		"dev data/scripter/job_default_data_for_Inter.txt",
		"dev data/scripter/job_default_data_for_Thai.txt",
		"end"
	};

	char		*strSaveFileName[]	=
	{
		"data/jobData.dat",
		"../Red stone for Japan/data/jobDataForJapan.dat",
		"../Red stone for China/data/jobDataForChina.dat",
		"../Red stone for Inter/data/jobDataForInternational.dat",
		"../Red stone for USA/data/jobDataForUSA.dat",
		"../Red stone for Inter/data/jobDataForInter.dat",
		"../Red stone for Thai/data/jobDataForThai.dat",
	};
		
	CDecoder	decoder;

	int			iIndex	=	0;

	while(1 && g_bIsDevPC)
	{
		if (STRICMP(strScriptFileName[iIndex],"end")	==	0)
			break;

		if	(!decoder.Upload(strScriptFileName[iIndex],aKeyword))
		{
			SetLogFolder();

			return	ERRMSG("에러!!","%s 화일이 없어요",strScriptFileName[iIndex]);
		}

		int		iJob	=	0;
		BOOL	bIsEnd	=	FALSE;

		while(bIsEnd==FALSE)
		{
			char	*strCode	=	decoder.getWord();

			switch(decoder.analCode(strCode))
			{
				case		eDEFAULT_DATA	:
				{
					char	*strJob	=	decoder.getWord();
					iJob			=	decoder.analCode(strJob);
					decoder.findOpenCode();
					break;
				}

				case		eDATA_SLOT0ITEM	:
				{
					int		iCount		=	decoder.getNumber();
					char	*strItem	=	decoder.getStream();
					int		iItem		=	cItem::GetItemIndexByName(strItem);
					
					g_aPlayer1LvSetting[iJob].m_wSlot0Item		=	iItem;
					g_aPlayer1LvSetting[iJob].m_wSlot0ItemCount	=	iCount;
					break;
				}

				case		eDATA_WEAPON	:
				{
					int		iIndex		=	decoder.getNumber();
					int		iCount		=	decoder.getNumber();
					char	*strWeapon	=	decoder.getStream();
					int		iWeapon		=	cItem::GetItemIndexByName(strWeapon);

					if (iWeapon	==	0xffff)
						return	ERRMSG("Error in Loading Default equipment weapon data","job index %d's %s not found",iJob,strWeapon);

					g_aPlayer1LvSetting[iJob].m_aWeaponCount[iIndex]	=	iCount;
					g_aPlayer1LvSetting[iJob].m_aWeapon[iIndex]		=	iWeapon;
					break;
				}

				case		eDATA_SHIELD	:
				{
					char	*strShield	=	decoder.getStream();
					int		iShield		=	cItem::GetItemIndexByName(strShield);

					if (iShield	==	0xffff)
						return	ERRMSG("Error in Loading Default equipment shield data","job index %d's %s not found",iJob,strShield);

					g_aPlayer1LvSetting[iJob].m_wShield	=	iShield;
					break;
				}

				case		eDATA_BELT		:
				{
					char	*strBelt	=	decoder.getStream();
					int		iBelt		=	cItem::GetItemIndexByName(strBelt);

					if (iBelt	==	0xffff)
						return	ERRMSG("Error in Loading Default equipment shield data","job index %d's %s not found",iJob,strBelt);

					g_aPlayer1LvSetting[iJob].m_wBelt	=	iBelt;
					break;
				}

				case		eDATA_GLOVE		:
				{
					char	*strGlove	=	decoder.getStream();
					int		iGlove		=	cItem::GetItemIndexByName(strGlove);

					if (iGlove	==	0xffff)
						return	ERRMSG("Error in Loading Default equipment glove data","job index %d's %s not found",iJob,strGlove);
					g_aPlayer1LvSetting[iJob].m_wGlove	=	iGlove;
					break;
				}

				case		eDATA_BULLET	:
				{
					int		iCount		=	decoder.getNumber();
					char	*strBullet	=	decoder.getStream();
					int		iBullet		=	cItem::GetItemIndexByName(strBullet);

					if (iBullet	==	0xffff)
						return	ERRMSG("Error in Loading Default equipment bullet data","job index %d's %s not found",iJob,strBullet);

					g_aPlayer1LvSetting[iJob].m_wBullet		=	iBullet;
					g_aPlayer1LvSetting[iJob].m_wBulletCount	=	iCount;
					break;
				}

				case	eDATA_BASIC_SKILL	:
				{
					char	*lpstrSkill		=	decoder.getStream();
					int		iBasicSkill		=	g_sm.getIndex(lpstrSkill,iJob);

					if (iBasicSkill		==	0xffff)
						return	ERRMSG("Error in Loading Default equipment bullet data","job index %d's default skill '%s' not found",iJob,lpstrSkill);

					g_aPlayer1LvSetting[iJob].m_wDefaultSkill	=	iBasicSkill;
					break;
				}

				case	eDATA_DISABLE		:
				{
					g_aPlayer1LvSetting[iJob].m_wStatus	=	0;
					break;
				}
				case	eDATA_ENABLE		:
				{
					g_aPlayer1LvSetting[iJob].m_wStatus	=	1;
					break;
				}
				case	eDATA_TEST			:
				{
					g_aPlayer1LvSetting[iJob].m_wStatus	=	2;
					break;
				}

				case	dCODE_CLOSE		:
				{
					break;
				}

				case		dCODE_END	:
				{
					cFILE	file;

					file.Open(strSaveFileName[iIndex],"wb");
					file.Write(g_aPlayer1LvSetting,sizeof(g_aPlayer1LvSetting));
					file.Close();

					bIsEnd	=	TRUE;
					break;
				}
			}
		}
		iIndex++;
	}

	SetLogFolder();

	return	TRUE;
}

//
//이 지역에서 사용하는 몸뚱아리 로딩
BOOL
CJobManager::loadLocalBody(cNUX *_lpNux,cMCU *_lpMcu)
{
	int	i;

	BOOL	isExistNewBody	=	FALSE;

	for (i=0;i<m_iCharacterDataCount;i++)
	{
		int	iBody		=	m_aJob[m_aJobList[i]+dJOB_BORDER].m_iBody;
		int	iVariation	=	m_aJob[m_aJobList[i]+dJOB_BORDER].m_iVariation;

		if	(g_im.isLoadedBody(iBody,iVariation))
			continue;

		isExistNewBody	=	TRUE;

		break;
	}

	g_im.resetIMonsterBody();

	if	(!isExistNewBody)
		return	TRUE;

	_lpMcu->reset();	//	만약에 새로운 몸뚱아리*-_-*가 존재한다면 다시 로딩해야 한다.

	g_im.m_iLoadedBodyCount	=	0;

	for (i=0;i<m_iCharacterDataCount;i++)
	{
		int	iBody		=	m_aJob[m_aJobList[i]+dJOB_BORDER].m_iBody;
		int	iVariation	=	m_aJob[m_aJobList[i]+dJOB_BORDER].m_iVariation;

		if	(!g_im.isLoadedBody(iBody,iVariation))
			g_im.addLoadedBody(iBody,iVariation);
	}

	if	(!g_im.loadLocalBody(_lpNux,_lpMcu))
		return	FALSE;

	return	TRUE;
}

//
//	제거
void
CJobManager::close()
{
}

//
//초기화
BOOL
CJobManager::init()
{
	int	i;

	for (i=0;i<=dMAX_JOB_COUNT	;i++)
		m_aJob[i].m_wPalette	=	0;

	return	TRUE;
}