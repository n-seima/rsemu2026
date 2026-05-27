#include "CNemo.h"
#include "CTitle.h"
#include "CJob.h"
#include "CMessage.h"
#include "CLoadTextCode.h"

CTitleManager	g_titleManager;

#ifdef	_IS_DEV_CLIENT
CDecoderKeyword	l_aTitleEffect[]	=
{
	{"스킬마스터"		,eTE_SKILL_MASTER},		
	{"짐꾼"				,eTE_PORTER},
	{"은행고객"			,eTE_BANK_CLIENT},
	{"선구자"			,eTE_FRONTIER},
	{"시민"				,eTE_CITIZEN},

	{"거래인"			,eTE_DEALER				},
	{"음유 시인"		,eTE_BARD				},
	{"사냥 경험치 보너스",eTE_HUNTING_BONUS		},
	{"길잡이"			,eTE_PATHFINDER			},	//	
	{"버서커"			,eTE_BERSERKER			},	//	
	{"연금술"			,eTE_ALCHEMY			},	//	
	{"변별자"			,eTE_DISTINGUISHER		},	//	
	{"지도 제작자"		,eTE_CARTOGRAPHER		},	//	
	{"생존본능"			,eTE_SURVIVAL_INSTINCTS	},	//	
	{"시티 워커"		,eTE_CITY_WALKER		},	//	
	{"캠핑 마스터"		,eTE_CAMPING_MASTER		},	//	
	{"몬스터 감별사"	,eTE_MONSTER_DISCERNER	},	//	
	{"애교"				,eTE_COURTESY			},	//	
	{"약사"				,eTE_DRUGGIST			},	//	
	{"밀교"				,eTE_ESOTERIC			},	//	
	{"함정 회피"		,eTE_TRAP_EVADE			},
	{"자물쇠 명인"		,eTE_LOCK_MASTER		},	
	{"트레져 헌터"		,eTE_TREASURE_HUNTER	},	
	{"엘레멘탈 마스터"	,eTE_ELEMENTAL_MASTER	},	
	{"모험가"			,eTE_ADVENTURER			},	
	{"능력치 증가"		,eTE_INCLINE_POWER		},	
	{"마법 저항 증가"	,eTE_INCLINE_MAGIC_RESISTANCE	},
	{"천적"				,eTE_NATURAL_ENEMY		},	//	천적
	{"영웅"				,eTE_HERO				},	//
	{"반영웅"			,eTE_ANTI_HERO			},	//

	{"end"				,dCODE_END}
};
#endif
//
//	리쉣
void
CTitleManager::reset()
{
	m_iTitleCount	=	0;

	for (int i =0;i<dMAX_TITLE_COUNT;i++)	m_aTitles[i].reset();
}	//	CTitleManager::reset()

BOOL
CTitleManager::init()
{
#ifdef	_IS_DEV_CLIENT
	reset();

	CDecoder	reader;
	CTitle		title;
	int			iValue,iTitleEffectCount;
	BOOL		bIsLooping	=	TRUE;
	
	if (!reader.Upload("dev data/scripter/title.txt",NULL))
		return	loadData();

	while(bIsLooping)
	{
		char	*str;
		char	*strKeyword	=	reader.getWord(TRUE);
		BOOL	bIsCorrect;

		switch(reader.analCode(strKeyword))
		{
			case	dCODE_CLOSE		:
			{
				addTitle(&title);
				continue;
			}

			case	dCODE_FINISH	:
			case	dCODE_OPEN		:
				continue;

			case	dCODE_END		:
				bIsLooping	=	FALSE;
				continue;
		}

		if (STRICMP("타이틀",strKeyword)==0)
		{
			title.reset();
			title.m_wSerial		=	reader.getNumber(&bIsCorrect);
			iTitleEffectCount	=	0;

			if	(!bIsCorrect)
				EXIT("error in CTitleManager::init()","타이틀 인덱스를 정의할 수 없습니다.'");

			str					=	reader.getStream();

			strcpy(title.m_strName,str);

			continue;
		}

		if (STRICMP("가능직업",strKeyword)==0)
		{
			str				=	reader.getStream();

			if	(STRICMP("모두",str)==0)
			{
				title.m_uiEnableJobMask	=	0xffffffff;
			}
			else
			{
				int	iEnableJob	=	0xffff;

				for (int iJob=0;iJob<dPLAYER_JOB_COUNT;iJob++)
					if (STRICMP(g_pstrHeroJobName[iJob],str) == 0)
						iEnableJob	=	iJob;

				if	(iEnableJob	==	0xffff)
					EXIT("error in CTitleManager::init()","%s 타이틀의 직업을 정의할 수 없습니다.'",title.m_strName);

				title.m_uiEnableJobMask	+=	(1<<iEnableJob);
			}
			continue;
		}

		if (STRICMP("필요레벨계산방법",strKeyword)==0)
		{
			str				=	reader.getWord();

			if (STRICMP("곱하기",str)==0)	title.m_wComputeRequireLevelMethod	=	eCRLM_MULTIPLE_VALUE;
			else
			if (STRICMP("설정",str)==0	)	title.m_wComputeRequireLevelMethod	=	eCRLM_LEVEL_VALUE;
			else
			if (STRICMP("제곱",str)==0	)	title.m_wComputeRequireLevelMethod	=	eCRLM_SQUARE_VALUE;
			continue;
		}

		if (STRICMP("필요레벨수치",strKeyword)==0)
		{
			title.m_wRequireLevelValue	=	reader.getNumber(&bIsCorrect);

			if (!bIsCorrect)
				EXIT("error in CTitleManager::init()","'%s'의 필요레벨수치를 정의 할 수 없습니다.'",title.m_strName);

			continue;
		}

		if (STRICMP("삭제가능",strKeyword)==0)
		{
			title.m_bf1IsRemoveAble	=	TRUE;
			continue;
		}
		if (STRICMP("챕터5보상예외",strKeyword)==0)
		{
			title.m_bf1ExceptCH5Reward	=	TRUE;
			continue;
		}
		if (STRICMP("타이틀효과",strKeyword)==0)
		{
			CTitleEffectInfo	*lpInfo	=	&title.m_aEffect[iTitleEffectCount++];

			int	iEffectValueIndex	=	0;
			str						=	reader.getStream();
			iValue					=	CDecoder::AnalCode(l_aTitleEffect,str);

			if (iValue		==	dCODE_UNDEFINE)
				EXIT("error in CTitleManager::init()","타이틀효과 '%s'를 정의 할 수 없습니다.'",str);

			lpInfo->m_wEffect=	iValue;

			reader.findOpenCode();	//	오픈코드
			
			while(1)
			{
				str				=	reader.getWord();

				DWORD	dwCode	=	reader.analCode(str);
				if	(dwCode	==	dCODE_CLOSE	)	break;
				if	(dwCode	==	dCODE_END	||	dwCode	==	dCODE_FINISH||	dwCode	==	dCODE_OPEN)	continue;

				if (STRICMP("수치",str)==0)
				{
					iValue		=	reader.getNumber(&bIsCorrect);

					if	(strstr(title.m_strName,"크로매틱")	&&	iValue	==	7)
						iValue	=	7;

					if (!bIsCorrect)
						EXIT("error in CTitleManager::init()","타이틀효과-수치-수치 인덱스를 정의할 수 없습니다.");

					iEffectValueIndex	=	iValue-1;

					for (int iTitleLevel=0;iTitleLevel<dMAX_TITLE_LEVEL;iTitleLevel++)
					{
						iValue	=	reader.getNumber(&bIsCorrect);

						if (!bIsCorrect)
							EXIT("error in CTitleManager::init()","타이틀효과-수치-수치 인덱스를 정의할 수 없습니다.");
						lpInfo->m_aiValue[iTitleLevel][iEffectValueIndex]	=	iValue;
					}

					continue;
				}

				break;
			}
			continue;
		}

		if (STRICMP("디폴트보유직업",strKeyword)==0)
		{
			str				=	reader.getStream();

			if	(STRICMP("모두",str)==0)
				title.m_uiDefaultOwnJobMask	=	0xffffffff;
			else
			{
				int	iEnableJob	=	0xffff;

				for (int iJob=0;iJob<dPLAYER_JOB_COUNT;iJob++)
					if (STRICMP(g_pstrHeroJobName[iJob],str) == 0)
						iEnableJob	=	iJob;

				if (iJob	==	0xffff)
					EXIT("error in CTitleManager::init()","%s 타이틀의 직업을 정의할 수 없습니다.'",title.m_strName);

				title.m_uiDefaultOwnJobMask	+=	1<<iEnableJob;
			}
			continue;
		}

		if (STRICMP("설명",strKeyword)==0)
		{
			str				=	reader.getStream();
			
			strcpy(title.m_strExplain,str);
			continue;
		}

		break;
	}

	return	saveData();
#else
	return	loadData();
#endif
}	//	CTitleManager::init()

BOOL
CTitleManager::addTitle(CTitle *_lpTitle)
{
#ifdef	_IS_DEV_CLIENT
	if	(_lpTitle->m_wSerial				>=	dMAX_TITLE_COUNT)
		EXIT("error in CTitleManager::addTitle","추가하려는 타이틀 '%s'의 시리얼이 이상합니다.",_lpTitle->m_strName);

	if	(_lpTitle->m_strName[0]			==	0)
		EXIT("error in CTitleManager::addTitle","추가하려는 타이틀의 이름이 없습니다.");

	if	(_lpTitle->m_wRequireLevelValue	==	0)
		EXIT("error in CTitleManager::addTitle","추가하려는 타이틀 '%s'의 필요 레벨 수치가 이상합니다.",_lpTitle->m_strName);

	if	(_lpTitle->m_strExplain[0]		==	0)
		EXIT("error in CTitleManager::addTitle","추가하려는 타이틀 '%s'의 설명이 비어있습니다.",_lpTitle->m_strName);

//	if	(_lpTitle->m_aEffect[0].m_wEffect==	0xffff)
//		EXIT("error in CTitleManager::addTitle","추가하려는 타이틀 '%s'의 효과가 설정되어 있지 않습니다.",_lpTitle->m_strName);

	if	(_lpTitle->m_uiEnableJobMask	==	0)	
		EXIT("error in CTitleManager::addTitle","추가하려는 타이틀 '%s'의 가능 직업이 설정되어 있지 않습니다.",_lpTitle->m_strName);

	CTitle	*lpSlot			=	&m_aTitles[_lpTitle->m_wSerial];

	if (lpSlot->m_wSerial	!=	0xffff)
		EXIT("error in CTitleManager::addTitle","추가하려는 타이틀 '%s'의 시리얼이 이미 존재합니다.",_lpTitle->m_strName);

	memcpy(lpSlot,_lpTitle,sizeof(CTitle));

	m_iTitleCount++;

#endif

	return	TRUE;
}	//	CTitleManager::addTitle(CTitle *_lpTitle)

//
//	데이터 읽기
BOOL
CTitleManager::loadData()
{
	cFILE	file;

	if (!file.Open("data/title.dat","rb"))
		EXIT("error in CTitleManager::loadData","can not find 'data/title.dat' file.");

	file.Read(&g_titleManager.m_iTitleCount,4);
	file.Read(g_titleManager.m_aTitles,sizeof(g_titleManager.m_aTitles));
	file.Close();

	return	TRUE;
}

//
//	데이터 저장
BOOL
CTitleManager::saveData()
{
#ifdef	_IS_DEV_CLIENT

	cFILE	file;

	if (!file.Open("data/title.dat","wb"))	return	FALSE;

	file.Write(&g_titleManager.m_iTitleCount,4);
	file.Write(g_titleManager.m_aTitles,sizeof(g_titleManager.m_aTitles));
	file.Close();


#endif
	return	TRUE;
}	//	CTitleManager::saveData()