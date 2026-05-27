#include "CImageData.H"
#include "cGAME.H"
#include "cDECODER.H"
#include "mess_sign.H"
#include "GameDLL.H"

cBODY					g_aBody[dBODY_COUNT];
cBODY					g_aEffect[dEFFECT_IMAGE_DATA_COUNT];
CImageDataManager		g_idm;

//
//	최초의 초기화
BOOL
CImageDataManager::init()
{
	if	(!loadJobBodyData())
		return	FALSE;
	if	(!loadEffect())
		return	FALSE;

	return	TRUE;
}

//
//	직업의 몸뚱아리 데이터 로딩
BOOL
CImageDataManager::loadJobBodyData()
{
	int		iIndex;

//	플레이어 몸뚱아리 불러오기
	{
		if (!g_game.setHeroBodyFolder())
			return	ERRMSG("error in CImageDataManager::loadJobBodyData()","can not find heros data folder");

		iIndex	=	0;

		while(1)
		{
			if	(STRICMP(FPGetHeroFileName(iIndex),"end")	==0)
				break;

			if	(!g_aBody[iIndex].load(FPGetHeroFileName(iIndex)))
				return	ERRMSG("error in CImageDataManager::loadJobBodyData()","can not find heros data\n%s",FPGetHeroFileName(iIndex));

			iIndex++;
		}
	}	//	플레이어 몸뚱아리 불러오기


//	소환수 데이터 로딩
	{
		int	iType,iGrade;

		for (iType=0;iType<dSUMMON_BEAST_TYPE_COUNT;iType++)
			for (iGrade=0;iGrade<dSUMMON_BEAST_GRADE_COUNT;iGrade++)
			{
				char	strFileName[128];
				int		iBody	=	dBODY_KELBY_1+iType*dSUMMON_BEAST_GRADE_COUNT+iGrade;

				sprintf(strFileName,"summon_beast_%.2d_%.2d.sad",iType+1,iGrade+1);

				if	(!g_aBody[iBody].load(strFileName))
					return	ERRMSG("error in CImageDataManager::loadJobBodyData()","can not load npc data\n\n%s",strFileName);
			}
	}

//	몬스터 몸뚱아리 불러오기
	{
		if (!g_game.setMonsterBodyFolder())
			return	ERRMSG("error in CImageDataManager::loadJobBodyData()","can not find monster data folder");

		iIndex	=	0;

		while(1)
		{
			if	(STRICMP(FPGetMonsterFileName(iIndex),"end")	==0)
				break;

			g_aBody[dBODY_MONSTER_START+iIndex].load(FPGetMonsterFileName(iIndex));

			iIndex++;
		}
	}	//	몬스터 몸뚱아리 불러오기

//	NPC 몸뚱아리 불러오기
	{
		if (!g_game.setNpcBodyFolder())
			return	ERRMSG("error in CImageDataManager::loadJobBodyData()","can not find npc data folder");

		iIndex	=	0;

		while(1)
		{
			if	(STRICMP(FPGetNpcFileName(iIndex),"end")	==0)
				break;

			g_aBody[dBODY_NPC_START+iIndex].load(FPGetNpcFileName(iIndex));

			iIndex++;
		}

		if	(!g_aBody[dBODY_OPERATOR_ANGEL_MALE].load("operator_man.sad"))
			return	ERRMSG("error in CImageDataManager::loadJobBodyData()","can not load npc data\n\noperator_man.sad");
		if	(!g_aBody[dBODY_OPERATOR_ANGEL_FEMALE].load("operator_woman.sad"))
			return	ERRMSG("error in CImageDataManager::loadJobBodyData()","can not load npc data\n\noperator_woman.sad");
	}	//	NPC 몸뚱아리 불러오기

//	
	SetLogFolder();

	return	TRUE;
}

//
//	효과 이미지들을 로딩한다.
BOOL
CImageDataManager::loadEffect()
{
	//
	//	Effect Data Format
	enum
	{
		eEDF_QANM,
		eEDF_ANM,
	};

	char	*lpstrFileName	=	"data/effect_data.dat";
// 
// #ifdef	_FOR_JAPAN
// 	cMessSign	messSign;
// 
// 	lpstrFileName	=	messSign.makeDecodeFile(lpstrFileName);
// 
// #ifdef	_DEBUG
// 	if	(!messSign.m_iIsSuccess)
// 		Beep(2200,200);
// #endif
// 
// #endif

	cFILE	file;
	int		i,iSkipSize;

	MSGOUT(" - 마법 효과 로딩 시작");

	if	(!file.Open(lpstrFileName,"rb"))
		return	ERRMSG("error!","can not find follow file\n\ndata/effect_data.dat");

	file.Read(&iSkipSize,4);
	file.Read(this,sizeof(CSaveImageIndexData));
	
	SetLogFolder();
	FPSetCurrentFolder("data/Effects");

	printf("image file size %d\n",file.Length);
	printf("image file count %d\n",m_iEffectCount);

	file.Seek(iSkipSize+8,SEEK_SET);

	for (i=0;i<m_iEffectCount;i++)
	{
		CEffectDataInfo	effectInfo;

		file.Read(&effectInfo,sizeof(CEffectDataInfo));

		printf("load %s ...\n",effectInfo.m_strImageFileName);

		if (!g_aEffect[i].load(effectInfo.m_strImageFileName))
			return	ERRMSG("error!!","can not find follow file\n\n%s",effectInfo.m_strImageFileName);
	}

	file.Close();

	SetLogFolder();

	return TRUE;
}

//
//	제거
void
CImageDataManager::close()
{
	int		i;

//	몸뚱아리 초기화
	for (i=0;i<dBODY_COUNT;i++)	g_aBody[i].reset();
	for (i=0;i<dEFFECT_IMAGE_DATA_COUNT;i++)	g_aEffect[i].reset();
}