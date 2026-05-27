#include "cPROJECT.H"
#include "cMESSAGE.H"
#include "cJOB.H"
#include "CItem.H"
#include "CItemPrefix.H"
#include "CSkill.H"
#include "CQuest.H"
#include "CMain.H"
#include "CLoadTextCode.h"
#include "arca.h"
#include "itemSet.h"
#include "pieceItem.h"
#include "secretDungeon.h"
#include "mainQuest.h"
#include "book.h"
#include "book_MonsterDictionary.h"
#include	"commonClass.H"		
#include	"mess_sign.H"		

cPROJECT	g_project;

//
//	프로젝트 데이터 초기화
void
cPROJECT::init(cNUX *_lpNux)
{
	m_lpNux	=	_lpNux;
}

//
//	프로젝트 폴더로 들어간다.
BOOL
cPROJECT::setProjectFolder()
{
	SetLogFolder();

	if (!SetCurrentDirectory(_ms("data/scenario/%s",m_strName))	)
		return	FALSE;

	return	TRUE;
}

//
//	맵 폴더로 들어간다.
BOOL
cPROJECT::setMapFolder()
{
	SetLogFolder();

	if (!SetCurrentDirectory(_ms("data/scenario/%s/map",m_strName))	)
		return	FALSE;

	return	TRUE;
}

BOOL
cPROJECT::readCloseQuestList()
{
	cFILE	file;

#ifdef	_IS_DEV_CLIENT
#ifdef	_FOR_KOREA

	char	*strClosemapListScriptFile[]	=	
	{
		"dev data/Scripter/closeQuestList.txt",
		"dev data/Scripter/closeQuestListForJapan.txt",
		"dev data/Scripter/closeQuestListForChina.txt",
		"end",
	};
	char	*strClosemapListFile[]	=
	{
		"data/closeQuestList.dat",
		"../Red stone for Japan/data/closeQuestListForJapan.dat",
		"../Red stone for China/data/closeQuestListForChina.dat"
	};

	CDecoder	decoder;
	int			iLanguage	=	0;

	while(1 && g_bIsDevPC)
	{
		m_iCloseQuestCount	=	0;

		if (STRICMP(strClosemapListScriptFile[iLanguage],"end")==0)
			break;

		if (decoder.Upload(strClosemapListScriptFile[iLanguage]))
		{
			while(1)
			{
				char		*lpstrName	=	decoder.getStream();
				CQuestItem	*lpQuest;

				if (STRICMP(lpstrName,"stream not found!!")==0)
					break;

				int			iQuestIndex	=	0;

				lpQuest	=	g_quest.getQuestByName(lpstrName);

				if (!lpQuest)
				{
					lpQuest		=	g_questArbeit.getQuestByName(lpstrName);

					iQuestIndex	+=	1000;
				}

				if (!lpQuest)
				{
					ERRMSG("error","can not find quest '%s'",lpstrName);
					continue;
				}

				iQuestIndex		+=	lpQuest->m_wSerial;

				m_awCloseQuestList[m_iCloseQuestCount++]	=	iQuestIndex;
			}
		}

		if (file.Open(strClosemapListFile[iLanguage],"wb"))
		{
			file.Write(&m_iCloseQuestCount,4,1);
			file.Write(m_awCloseQuestList,m_iCloseQuestCount*2);

			file.Close();
		}

		iLanguage++;
	}
#endif
#endif


#ifdef	_FOR_CHINA
	char	*lpstrClosemapListFile		=	"data/ChinaCloseQuestList.dat";
#elif	_FOR_JAPAN
	char	*lpstrClosemapListFile		=	"data/JapanCloseQuestList.dat";
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

	return	TRUE;
}

BOOL
cPROJECT::loadItem()
{
//	"item.dat" 기본 아이템 데이터 아이템 불러오기
	int		iItemTextLocate,iLastIndex,iCount;
	cFILE	file;

#ifdef	_FOR_JAPAN
	char	strItemNewFileName[512];

	sprintf(strItemNewFileName,"data/scenario/%s/itemData.dat",m_strName);

	DeleteFile(strItemNewFileName);
#endif

	if (!file.Open(_ms("data/scenario/%s/%s",m_strName,dITEM_FILE_NAME),"rb"))
		return	ERRMSG(dMSG_OPEN_PROJECT_ERROR,dMSG_CAN_NOT_FIND_ITEM_DATA_FILE);

	int	iVersion			=	-1;

	{
		file.Seek(-8,SEEK_END);

		int	iAddVersionChecker	=	dITEM_VERSION_CHECKER;

		file.Read(&iAddVersionChecker,sizeof(iAddVersionChecker));

		if	(iAddVersionChecker	==	dITEM_VERSION_CHECKER)
			file.Read(&iVersion,sizeof(iVersion));

		file.Seek(0,SEEK_SET);
	}

	int	iNation;
	
	file.Read(&iNation,4);					//	국가
	file.Read(&iLastIndex,4);				//	저장 된 아이템의 수
	file.Read(&iItemTextLocate,4);			//	텍스트 데이터가 저장된 위치

#ifdef	_FOR_JAPAN
	iNation	=	eNationJapan;
#endif

	cMessSign::SetDataEncodeTable(iNation);
	cMessSign::DecodeDataBuffer2(&iLastIndex,4);

	g_iBasicItemCount	=	iLastIndex;

	file.Read(g_aBasicItem,(sizeof(cBasicItem))*iLastIndex);

	for(int i=0;i<iLastIndex;i++)
		cMessSign::DecodeDataBuffer2(g_aBasicItem+i,sizeof(cBasicItem));
	
#ifdef	_FOR_JAPAN
	g_aBasicItem[dITEM_INDEX_GOLD_BAR].m_wStackLimit = dMAX_GOLD_BAR_STACK_FOR_JAPAN;	// 일본용 금괴 스택 제한은 5개로 한다.
#endif

	file.Read(&g_itemPrefix.m_iCount,sizeof(int));//	아이템 접두사

	cMessSign::DecodeDataBuffer2(&g_itemPrefix.m_iCount,4);

	for (i=0;i<g_itemPrefix.m_iCount;i++)
	{
		cITEM_PREFIX	prefix;

		file.Read(&prefix,sizeof(cITEM_PREFIX));//	아이템 접두사

		cMessSign::DecodeDataBuffer2(&prefix,sizeof(cITEM_PREFIX));

		memcpy(&g_aItemPrefix[prefix.m_wSerial],&prefix,sizeof(cITEM_PREFIX));
	}

//	프리미엄 아이템 정보 로딩
	{
		file.Read(&g_premiumItem.m_iCount,4);

		cMessSign::DecodeDataBuffer2(&g_premiumItem.m_iCount,4);

		g_premiumItem.init(g_premiumItem.m_iCount);

		file.Read(g_premiumItem.m_pItems,g_premiumItem.m_iCount*sizeof(CCustomItem));

		for (int i=0;i<g_premiumItem.m_iCount;i++)
			cMessSign::DecodeDataBuffer2(&g_premiumItem.m_pItems[i],sizeof(CCustomItem));

	}


//	커스텀 아이템 정보 스킵~
	{
		file.Read(&iCount,4);

		cMessSign::DecodeDataBuffer2(&iCount,4);

		if	(iCount)
			file.Skip(sizeof(CCustomItem)*iCount);
	}

//	보물 상자 정보 스킵~
	{
		file.Read(&iCount,4);

		cMessSign::DecodeDataBuffer2(&iCount,4);

		if	(iCount)	
			file.Skip(sizeof(cArcaDefine)*iCount);
	}

//	그룹 아이템
	int	iValue;

	file.Read(&iValue,4);

	if (iValue	==	0x12345674)
	{
	//	세트 아이템 로딩
		file.Read(&iCount,4);

		cMessSign::DecodeDataBuffer2(&iCount,4);

		if (iCount)
		{
			g_itemSetManager.init(iCount);

			cItemSet*	lpBay		=	g_itemSetManager.getBay();

			file.Read(lpBay,sizeof(cItemSet)*iCount);

			g_itemSetManager.build();
		}

//	조각 아이템 로딩
		file.Read(&iCount,4);

		cMessSign::DecodeDataBuffer2(&iCount,4);

		if (iCount)	
		{
			g_pieceItemManager.init(iCount);

			cPieceItem*	lpPieceItemBay	=	g_pieceItemManager.getBay();

			file.Read(lpPieceItemBay,sizeof(cPieceItem)*iCount);

			for (int i=0;i<iCount;i++)
				cMessSign::DecodeDataBuffer2(lpPieceItemBay+i,sizeof(cPieceItem));

			g_pieceItemManager.build();
		}
	}

	file.Close();

	memcpy(g_aBackupItemPrefix,g_aItemPrefix,sizeof(g_aItemPrefix));

	g_itemGold.reset();

	g_itemGold.m_wBaseItem	=	0;
	g_itemGold.m_bCount		=	1;

	g_itemCarrot.reset();

	g_itemCarrot.m_wBaseItem	=	1834;
	g_itemCarrot.m_bCount		=	1;

	cItem::BuildData();

	return	TRUE;
}	//	"item.dat" 기본 아이템 데이터 아이템 불러오기

//
//	포로젝트 로딩
BOOL
cPROJECT::load(char *_strName,cNUX *_lpNux)
{
	int		i,iSize,iNation;
	cFILE	*lpFile	=	NULL,File;

	if (_lpNux)
	{
		lpFile	=	_lpNux->Open(_strName);

		if (!lpFile)
			return	ERRMSG(dMSG_PROJECT_LOADING_ERROR,"%s\n\n%s",dMSG_CAN_NOT_OPEN_FOLLOW_FILE,_strName);
	}
	else
	{
		SetLogFolder();

		SetCurrentDirectory("data/scenario");
		if (!File.Open(_strName,"rb"))	
		{
			SetLogFolder();
			return	ERRMSG(dMSG_PROJECT_LOADING_ERROR,"%s\n\n%s",dMSG_CAN_NOT_FIND_FOLLOW_FILE,_strName);
		}

		SetLogFolder();

		lpFile	=	&File;
	}

	int	iVersion	=	-1;
//	프로젝트 기본 데이터 불러오기
	{
		lpFile->Read(&iSize				,4	);		//	화일 사이즈

		char	strHeader[60];
		
		lpFile->Read(strHeader	,60	);		//	헤더

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

		lpFile->Read(m_strName,sizeof(m_strName));	//	프로젝트 이름
		lpFile->Read(&iNation,sizeof(iNation));

#ifdef	_FOR_JAPAN
		iNation	=	eNationJapan;
#endif

		cMessSign::SetDataEncodeTable(iNation);
	}	//	프로젝트 기본 데이터 불러오기

	if	(!loadItem())
		return	FALSE;

	{
		int	iQuestCount;

		lpFile->Read(&iQuestCount,4);

		cMessSign::DecodeDataBuffer2(&iQuestCount,4);
		
		g_quest.init(iQuestCount);

		CQuestItemSaveData	saveQuestData;

		for (i=0;i<iQuestCount;i++)
		{
			lpFile->Read(&saveQuestData,sizeof(saveQuestData));

			cMessSign::DecodeDataBuffer2(&saveQuestData,sizeof(saveQuestData));

			g_quest.setQuestInfo(i,&saveQuestData);

			for (int iStatus=0;iStatus<dQUEST_STATUS_COUNT;iStatus++)
				g_quest.setExplain(i,iStatus,saveQuestData.m_astrExplain[iStatus]);
		}

		lpFile->Read(&iQuestCount,4);

		cMessSign::DecodeDataBuffer2(&iQuestCount,4);

		g_questArbeit.init(iQuestCount);

		for (i=0;i<iQuestCount;i++)
		{
			lpFile->Read(&saveQuestData,sizeof(saveQuestData));

			cMessSign::DecodeDataBuffer2(&saveQuestData,sizeof(saveQuestData));

			g_questArbeit.setQuestInfo(i,&saveQuestData);

			for (int iStatus=0;iStatus<dQUEST_STATUS_COUNT;iStatus++)
				g_questArbeit.setExplain(i,iStatus,saveQuestData.m_astrExplain[iStatus]);
		}

//	비밀 던전 정보
		{
			int	iValue;
			lpFile->Read(&iValue,4);	//

			if (iValue	==	0x12345678)
			{
				int		iCount;

				lpFile->Read(&iCount,4);

				cMessSign::DecodeDataBuffer2(&iCount,sizeof(iCount));

				if (iCount)
				{
					g_sdManager.init(iCount);

					cSecretDungeon	*lpBay	=	g_sdManager.getBay();

					lpFile->Read(lpBay,sizeof(cSecretDungeon)*iCount);

					for(int i=0;i<iCount;i++)
						cMessSign::DecodeDataBuffer2(lpBay+i,sizeof(cSecretDungeon));

					g_sdManager.rebuild();
				}
			}
			else
				lpFile->Seek(-4,SEEK_CUR);

		}

//	비밀 던젼 수치
		{
			lpFile->Seek(dMAX_SECRET_DUNGEON_COUNT*dMAX_DUNGEON_VALUE_COUNT*dDUNGEON_VALUE_TEXT_LENGTH+4);
		}

//	메인 퀘스트 정보
		{
			lpFile->Seek(c_iMaxMainQuestValueCount*c_iMainQuestValueNameLength+c_iMaxMainQuestSwitchCount*c_iMainQuestSwitchNameLength+4);
			lpFile->Read(&g_chapterManager,sizeof(g_chapterManager));
		}

//	책 정보
		{
			lpFile->Read(&g_book.m_iBookCount,4);
			
			for(int iBook=0;iBook<g_book.m_iBookCount;iBook++)
			{
				cBookInfoDefine	*lpBook	=	&g_book.m_aBook[iBook];

				lpFile->Read(lpBook,sizeof(cBookSaveDataDefine));
				lpFile->Skip(lpBook->m_wTextLength);

				for(int iCategory=0;iCategory<lpBook->m_wCategoryCount;iCategory++)
				{
					cBookCategoryDefine	*lpCategory	=	&lpBook->m_aCategory[iCategory];

					lpFile->Read(lpCategory,sizeof(cBookCategorySaveDataDefine));

					for(int iPage=0;iPage<lpCategory->m_iPageCount;iPage++)
					{
						cBookPageInfoDefine	*lpPage	=	&lpCategory->m_aPage[iPage];

						lpFile->Read(lpPage,sizeof(cBookPageSaveDataDefine));
						lpFile->Seek(lpPage->m_wTextLength,SEEK_CUR);
					}
				}
			}
		}
	}

//	마무리
	lpFile->Close();


///////////////////////////////////////////////////////////////////////////////////
//	프로젝트 외부 데이터 화일을 로딩한다.
///////////////////////////////////////////////////////////////////////////////////

//	프로젝트 폴더로 들어간다.
	if (!_lpNux)
	{
		if (!setProjectFolder())
			return	ERRMSG(dMSG_OPEN_PROJECT_ERROR,dMSG_CAN_NOT_FIND_PROJECT_FOLDER);
	}

//	"job.dat" 직업 데이터 불러오기
#ifdef	_FOR_KOREA
	if	(!lpFile->Open("job.dat","rb"))
#else
	if	(!lpFile->Open("job2.dat","rb"))
#endif
		return	FALSE;

	int	iValue	=	cMessSign::MakeCheckTableResult(g_strLogFolder);

//	직업 데이터 로딩
	{
		int	iCount=iValue,iVersion=-1,iVersionCheck,iVersionCheckCode=0x12345678;

		lpFile->Read(&iVersionCheck,4);

		if	(iVersionCheck	!=	iVersionCheckCode)
		{
			lpFile->Rewind();
			lpFile->Read(&iCount,4);
		}
		else
		{
			lpFile->Read(&iVersion,4);
			lpFile->Read(&iCount,4);
		}

		g_jm.init();

		for (i=0;i<iCount;i++)
		{
			cJOB		jobData;
			lpFile->Read(&jobData,sizeof(cJOB));	//	시리얼

			CJobInfo	*lpJobInfo	=	&g_jm.m_aJob[jobData.m_iSerial+dJOB_BORDER];

			lpJobInfo->m_wSerial	=	jobData.m_iSerial+dJOB_BORDER;

			lpJobInfo->setData(&jobData);

			g_gwBookMonDic.setData(&jobData , iCount);
		}

		lpFile->Close();

		for (i=dJOB_KELBY_1;i<=dJOB_HEDGER_3;i++)
		{
			int	iType	=	(i-dJOB_KELBY_1)/dSUMMON_BEAST_GRADE_COUNT;

			strcpy(g_jm.m_aJob[i].m_strName,g_pstrSummonBeastName[iType]);
		}
	}

	if (_lpNux)
		KILL(lpFile);

	SetLogFolder();

	return	TRUE;
}

//
//	포로젝트 로딩
void
cPROJECT::resetItemPrefixData()
{
	memcpy(g_aItemPrefix,g_aBackupItemPrefix,sizeof(g_aItemPrefix));
}