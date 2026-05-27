#include "cPROJECT.H"
#include "cItem.H"
#include "cJOB.H"
#include "CQuest.H"
#include "arca.H"
#include "itemSet.H"
#include "pieceItem.H"
#include "secretDungeon.H"
#include "book.H"
#include "cFIELD.H"
#include "mess_sign.H"
#include "commonClass.H"
#include "GameDLL.H"

BOOL		cPROJECT::s_isExist		=	FALSE;

cPROJECT	g_project;

BOOL
cPROJECT::setProjectFolder()
{
	return	FPSetCurrentFolder(m_strProjectFolder);
}	//	프로젝트 폴더

BOOL
cPROJECT::setMapFolder()
{
	return	FPSetCurrentFolder(m_strMapFolder);
}	//	프로젝트의 맵들이 저장된 폴더

BOOL
cPROJECT::setDungeonFolder(char *_strDungeonMapFolder)
{
	if (_strDungeonMapFolder)
		return	FPSetCurrentFolder(_ms("%s\\%s",m_strDungeonFolder,_strDungeonMapFolder));

	return	FPSetCurrentFolder(m_strDungeonFolder);
}	//	프로젝트의 던젼설정들이 저장된 폴더

BOOL
cPROJECT::loadProject(char *fn)
{
	FPSetCurrentFolder(_ms("%s\\data\\Scenario",g_strLogFolder));		//	시나리오 폴더

	cFILE	file,textFile;
	DWORD	size;
	int		i=0,j=0,iCount=0,iTextSize=0,iBufferSize=1024,iNation;
	int		iVersion	=	0;
	char	strHeader[70];
//	char	*pBuffer	=	NULL;

	if	(!file.Open(fn,"rb"))
	{	
		SetLogFolder();

		return ERRMSG("Error in cDATA::LoadProject","can not open file '%s'",fn);
	}

//	프로젝트 화일 기본 데이터 로딩
	{
		file.Read(&size		,4	);		//	화일 사이즈

		file.Read(strHeader	,60	);		//	헤더

		for (i=0;;i++)
		{
			if	(stricmp(g_strProjectHeader[i],"end"		)==0)
				return	FALSE;

			if (stricmp(g_strProjectHeader[i],strHeader	)==0)
			{
				iVersion	=	i;
				break;
			}
		}

		file.Read(m_strName,sizeof(m_strName));	//	프로젝트 이름
		
		strcpy(m_strProjectFolder,_ms("%s\\data\\Scenario\\%s",g_strLogFolder,m_strName));

		file.Read(&iNation,sizeof(int));	//	프로젝트 이름
	}	//	프로젝트 화일 기본 데이터 로딩

	BOOL	bIsDecodeKaramData	=	FALSE;

	if	(iVersion	>=	ePVUI_13_ENCRYPT_MORE)
		bIsDecodeKaramData	=	TRUE;

//	프로젝트 폴더로 들어가기
	{
		if	(!setProjectFolder())
			return	ERRMSG("project loading error!","can not find project folder");
	}	//	프로젝트 폴더로 들어가기

	int	iQuestCount;
	int	iValue	=	cMessSign::MakeCheckTableResult("checkData");

	{
		CQuestManager	*lpQuest;

#ifdef	_FOR_JAPAN
		iNation	=	eNationJapan;
#endif

		if	(iVersion	>=	ePVUI_12_ENCRYPT)
			cMessSign::SetDataEncodeTable(iNation);

		for (int iQuestType=0;iQuestType<2;iQuestType++)
		{
			if (iQuestType==0)
				lpQuest	=	&g_quest;
			if (iQuestType==1)
				lpQuest	=	&g_questArbeit;

			file.Read(&iQuestCount,4);	//	프로젝트 이름

			if	(iVersion	>=	ePVUI_12_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&iQuestCount,4);

			lpQuest->m_iCount	=	iQuestCount;

			CQuestItem	saveQuest;

			for (int i=0;i<iQuestCount;i++)
			{
				file.Read(&saveQuest,sizeof(CQuestItem));

				if	(iVersion	>=	ePVUI_12_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&saveQuest,sizeof(CQuestItem));

				lpQuest->setQuestInfo(i,&saveQuest);
			}
		}
	}

	{
		int	iValue;
		file.Read(&iValue,4);	//	프로젝트 이름

		if (iValue	==	0x12345678)
		{
			int		iCount;

			file.Read(&iCount,4);

			if	(iVersion	>=	ePVUI_12_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&iCount,sizeof(iCount));

			if (iCount)
			{
				g_pSdManager->init(iCount);

				cSecretDungeon	*lpBay	=	g_pSdManager->getBay();

				file.Read(lpBay,sizeof(cSecretDungeon)*iCount);

				if	(iVersion	>=	ePVUI_12_ENCRYPT)
					for (int i=0;i<iCount;i++)
						cMessSign::DecodeDataBuffer2(lpBay+i,sizeof(cSecretDungeon));
		
				g_pSdManager->rebuild();
			}
		}
		else
			file.Seek(-4,SEEK_CUR);
	}

	//	던젼 수치 관련 내용 스킵~
	{
		file.Seek(4);
		file.Seek(dMAX_SECRET_DUNGEON_COUNT*dMAX_DUNGEON_VALUE_COUNT*dDUNGEON_VALUE_TEXT_LENGTH,SEEK_CUR);
	}

//	
	{
		file.Seek(4);

		const	int	c_iMainQuestDataSize	=	205192;

		file.Seek(c_iMainQuestDataSize,SEEK_CUR);
	}

	{
		file.Read(&g_book.m_iBookCount,4);
		
		printf("book count %d\n",g_book.m_iBookCount);

		for(int iBook=0;iBook<g_book.m_iBookCount;iBook++)
		{
			cBookInfoDefine	*lpBook	=	&g_book.m_aBook[iBook];

			file.Read(lpBook,sizeof(cBookSaveDataDefine));
			file.Skip(lpBook->m_wTextLength);

			for(int iCategory=0;iCategory<lpBook->m_wCategoryCount;iCategory++)
			{
				cBookCategoryDefine	*lpCategory	=	&lpBook->m_aCategory[iCategory];

				file.Read(lpCategory,sizeof(cBookCategorySaveDataDefine));

				for(int iPage=0;iPage<lpCategory->m_iPageCount;iPage++)
				{
					cBookPageInfoDefine	*lpPage	=	&lpCategory->m_aPage[iPage];

					file.Read(lpPage,sizeof(cBookPageSaveDataDefine));
					file.Seek(lpPage->m_wTextLength,SEEK_CUR);
				}
			}
		}
	}

//	카르마 읽기
	{
		cFIELD::ReadKarmaContainer(&file,&m_kcMonsterDeath,bIsDecodeKaramData);
		cFIELD::ReadKarmaContainer(&file,&m_kcMonsterGen,bIsDecodeKaramData);
	}

	file.Close();
	
//	각종 데이터 화일 불러오기
	{
		cFILE	dataFile;

	//	텍스트 화일 오픈
		{
//			if (!textFile.Open("textData.dat","rb"))
//				return	ERRMSG("file open error!",_ms("can not open follow file\n\n\"textData.dat\""));

//			pBuffer	=	new char [iBufferSize];		//	텍스트 데이터를 읽어들일 버퍼
		}	//	텍스트 화일 오픈

//	"item.dat" 기본 아이템 데이터 아이템 불러오기
		{
			if (dataFile.Open(dITEM_FILE_NAME,"rb"))
			{
				{
					dataFile.Seek(-8,SEEK_END);

					int	iAddVersionChecker	=	dITEM_VERSION_CHECKER;

					dataFile.Read(&iAddVersionChecker,sizeof(iAddVersionChecker));

					if	(iAddVersionChecker	==	dITEM_VERSION_CHECKER)
						dataFile.Read(&iVersion,sizeof(iVersion));

					dataFile.Seek(0,SEEK_SET);
				}

				dataFile.Read(&iNation,4);						//	저장된 아이템의 수
				dataFile.Read(&g_im.m_iItemCount,4);						//	저장된 아이템의 수

#ifdef	_FOR_JAPAN
				iNation	=	eNationJapan;
#endif

				cMessSign::SetDataEncodeTable(iNation);
				
				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&g_im.m_iItemCount,4);
				dataFile.Skip(4);									//	텍스트 위치

				printf("item count %d\n",g_im.m_iItemCount);

				dataFile.Read(g_aBasicItem,sizeof(cBasicItem)*g_im.m_iItemCount);

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					for(int i=0;i<g_im.m_iItemCount;i++)
						cMessSign::DecodeDataBuffer2(g_aBasicItem+i,sizeof(cBasicItem));

#ifdef	_FOR_JAPAN
				g_aBasicItem[dITEM_INDEX_GOLD_BAR].m_wStackLimit = dMAX_GOLD_BAR_STACK_FOR_JAPAN;	// 일본용 금괴 스택 제한은 10개로 한다.
#endif
			}
			else
				cMSG::Error("error!!","can not find 'item.dat' file");

			g_im.build();	//	빌드~

		//	아이템 접두사 로딩
			{
				dataFile.Read(&g_itemPrefix.m_iCount,4);

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&g_itemPrefix.m_iCount,4);

				printf("prefix count %d\n",g_itemPrefix.m_iCount);

				for (int i=0;i<g_itemPrefix.m_iCount;i++)
				{
					cITEM_PREFIX	itemPrefix;

					dataFile.Read(&itemPrefix,sizeof(cITEM_PREFIX));

					if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
						cMessSign::DecodeDataBuffer2(&itemPrefix,sizeof(cITEM_PREFIX));
					
					memcpy(&g_aItemPrefix[itemPrefix.m_wSerial],&itemPrefix,sizeof(itemPrefix));
				}
			}//	아이템 접두사 로딩
			
			{
				dataFile.Read(&g_premiumItemManager.m_iCount,4);

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&g_premiumItemManager.m_iCount,4);

				printf("premium item count %d\n",g_premiumItemManager.m_iCount);

				g_premiumItemManager.init(g_premiumItemManager.m_iCount);
				dataFile.Read(g_premiumItemManager.m_pItems,g_premiumItemManager.m_iCount*sizeof(CCustomItem));

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					for (int i=0;i<g_premiumItemManager.m_iCount;i++)
						cMessSign::DecodeDataBuffer2(g_premiumItemManager.m_pItems+i,sizeof(CCustomItem));

				g_premiumItemManager.build();
			}

			//	커스텀 아이템 정보 로딩
			{
				dataFile.Read(&g_customItem.m_iCount,4);

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&g_customItem.m_iCount,4);

				printf("custom item count %d\n",g_customItem.m_iCount);

				if (g_customItem.m_iCount)
				{
					g_customItem.init(g_customItem.m_iCount);
					dataFile.Read(g_customItem.m_pItems,g_customItem.m_iCount*sizeof(CCustomItem));
				}

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					for (int i=0;i<g_customItem.m_iCount;i++)
						cMessSign::DecodeDataBuffer2(g_customItem.m_pItems+i,sizeof(CCustomItem));

				g_customItem.build();
			}

		//	보물 상자 정보 로딩
			{
				int			iCount;

				dataFile.Read(&iCount,4);

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&iCount,4);

				printf("arca count %d\n",iCount);

				g_arcaManager.alloc(iCount);

				if	(iCount)	
					dataFile.Read(g_arcaManager.m_pArcaList,sizeof(cArcaInfo)*iCount);

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					for (int i=0;i<iCount;i++)
						cMessSign::DecodeDataBuffer2(g_arcaManager.m_pArcaList+i,sizeof(cArcaInfo));
			}

	//	아이템 세트
			int	iValue;

			dataFile.Read(&iValue,4);

			if (iValue	==	0x12345674)
			{
			//	세트 아이템 로딩
				dataFile.Read(&iCount,4);

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&iCount,4);

				printf("item set count%d\n",iCount);

				if (iCount)
				{
					g_itemSetManager.init(iCount);

					cItemSet*	lpBay		=	g_itemSetManager.getBay();

					dataFile.Read(lpBay,sizeof(cItemSet)*iCount);

					g_itemSetManager.build();
				}

		//	조각 아이템 로딩
				dataFile.Read(&iCount,4);

				if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&iCount,4);

				printf("item piece count%d\n",iCount);

				if (iCount)
				{
					g_pieceItemManager.init(iCount);

					cPieceItem*	lpPieceItemBay	=	g_pieceItemManager.getBay();

					dataFile.Read(lpPieceItemBay,sizeof(cPieceItem)*iCount);

					if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
						for (int i=0;i<iCount;i++)
							cMessSign::DecodeDataBuffer2(lpPieceItemBay+i,sizeof(cPieceItem));

					g_pieceItemManager.build();
				}
			}


			dataFile.Close();
		}	//	"item.dat" 기본 아이템 데이터 아이템 불러오기

//	"job.dat" 직업 데이터 불러오기
#ifdef	_FOR_KOREA		
		if (dataFile.Open("job.dat","rb"))
#else
		if (dataFile.Open("job2.dat","rb"))
#endif
		{
			int	iCount,iVersion=-1,iVersionCheck,iVersionCheckCode=0x12345678;

			dataFile.Read(&iVersionCheck,4);

			if	(iVersionCheck	!=	iVersionCheckCode)
			{
				dataFile.Rewind();
				dataFile.Read(&iCount,4);
			}
			else
			{
				dataFile.Read(&iVersion,4);
				dataFile.Read(&iCount,4);
			}

			printf("job count%d\n",iCount);
			dataFile.Read(g_aJob,sizeof(cJOB)*iCount);	//	시리얼과 어빌러티

			if	(iVersion	<	eJobDataFileVersion_begin)
				for (int i=0;i<iCount;i++)
					g_aJob[i].m_bf3RequireRebirthCountForTame	=	0;

			dataFile.Close();
		}	//	직업 데이터 로딩
	}	//	각종 데이터 화일 불러오기

//	프로젝트 화일 로딩 완료
//	textFile.Close();
//	pKILL(pBuffer);

	SetLogFolder();

	strcpy(m_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",g_strLogFolder,m_strName));
	strcpy(m_strDungeonFolder,_ms("%s\\data\\Scenario\\%s\\dungeon",g_strLogFolder,m_strName));

	{
		cFILE	file;

#ifdef	_FOR_CHINA
		char	*lpstrClosemapListFile		=	"data/closeQuestListForChina.dat";
#elif	_FOR_JAPAN
		char	*lpstrClosemapListFile		=	"data/closeQuestListForJapan.dat";
#elif	_FOR_USA
		char	*lpstrClosemapListFile		=	"data/closeQuestListForUsa.dat";
#else
		char	*lpstrClosemapListFile		=	"data/closeQuestList.dat";
#endif

		m_iCloseQuestCount	=	0;

		if (file.Open(lpstrClosemapListFile,"rb"))
		{
			file.Read(&m_iCloseQuestCount,4,1);
			file.Read(m_awCloseQuestList,m_iCloseQuestCount*2);

			file.Close();
		}

		if (m_iCloseQuestCount)
		{
			cMSG::Put("close quest info","close quest count %d",m_iCloseQuestCount);

			g_quest.checkCloseQuest(0);
			g_questArbeit.checkCloseQuest(1000);
		}
	}


	return	TRUE;
}

BOOL
cPROJECT::isCloseQuest(int _iQuestSerial)
{
	for	(int i=0;i<m_iCloseQuestCount;i++)
	{
		if (m_awCloseQuestList[i]	==	_iQuestSerial)
			return	TRUE;
	}

	return	FALSE;
}