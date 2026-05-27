#include "cPROJECT.H"
#include "cMAPWND.H"
#include "cAREA.H"
#include "cOBJECT.H"
#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cDRAW.H"
#include "cVALUE.H"
#include "cQUEST.H"
#include "cITEM.H"
#include "cITEM_PREFIX.H"
#include "cTOOLWND.H"
#include "cMAP.H"
#include "resource.h"
#include "cJOB.h"
#include "arca.h"
#include "setItem.h"
#include "pieceItem.h"
#include "secretDungeon.h"
#include "book.h"
#include "mainQuest.h"
#include	"cSET.H"		// 10.01.15
#include	"mess_sign.H"		
#include	"commonClass.H"		
#include	"localize.H"		

char	cPROJECT::s_strName[128];
char	cPROJECT::s_strFileName[128];
char	cPROJECT::s_strMapFolder[256]	=	"";	//	마지막으로 저장하거나 불로온 맵 화일의 폴더

CKarmaContainer	cPROJECT::s_kcMonsterDeath;
CKarmaContainer	cPROJECT::s_kcMonsterGen;

//
//	새로운 프로젝트
BOOL
cPROJECT::NewProject(HINSTANCE hinst,HWND hwnd,BOOL isFirst)
{
	int	result	=	DialogBox( hinst,"NEW_PROJECT",hwnd,(DLGPROC)s_procNewProject);

	SetLogFolder();

	return	TRUE;
}	//	cPROJECT::NewProject(HINSTANCE hinst,HWND hwnd,BOOL isFirst)

BOOL
cPROJECT::SetTempMapFolder(char *_lpstrDestFolder)			// 09.07.24 추가
{
	SetMapFolder();

	char	strFolderName[512];

	if	(_lpstrDestFolder)
		sprintf(strFolderName,"%s/%s/data/Scenario/%s/map/temp",_logFolder,_lpstrDestFolder,s_strName);
	else
		sprintf(strFolderName,"%s/data/Scenario/%s/map/temp",_logFolder,s_strName);

	if	(!SetCurrentDirectory(strFolderName))
	{
		if	(_lpstrDestFolder)
			return	ERRMSG("can not find follow folder",strFolderName);

		SetMapFolder();

		cFOLDER::Create("Temp");

		return	SetCurrentDirectory(_ms("%s/data/Scenario/%s/map/temp",_logFolder,s_strName));
	}

	return	TRUE;
}

//
//	시나리오 폴더로 이동
BOOL
cPROJECT::LoadBlockedTileInfo()
{
	int			iTileSet=0;
	cDECODER	decoder;	

	while(1)
	{
		if (stricmp(g_strTileSet[iTileSet*2],"end")==0)	break;

		SetCurrentDirectory("data/Mapset");
		SetCurrentDirectory(g_strTileSet[iTileSet*2]);

		if (decoder.Upload("crash_info.txt",NULL))
		{
			g_aBlockedInfo[iTileSet].m_iBlockedInfoCount	=	0;
			g_aBlockedInfo[iTileSet].m_iCrashedInfoCount	=	0;

			BOOL	bIsLoop	=	TRUE;

			while(bIsLoop)
			{
				char	*strWord	=	decoder.GetWord();

				if (stricmp(strWord,"crash")==0)
				{
					int	iBegin	=	decoder.GetNumber();
					int	iEnd	=	decoder.GetNumber();

					g_aBlockedInfo[iTileSet].addCrashedInfo(iBegin,iEnd);
				}
				else
				if (stricmp(strWord,"block")==0)
				{
					int	iBegin	=	decoder.GetNumber();
					int	iEnd	=	decoder.GetNumber();

					g_aBlockedInfo[iTileSet].addBlockedInfo(iBegin,iEnd);
				}
				else	bIsLoop	=	FALSE;
			}
		}

		iTileSet++;

		SetLogFolder();
	}
	return	TRUE;
}	//	cPROJECT::SetProjectFolder()

//
//	프로젝트 정보를 읽어 들인다.
BOOL
cPROJECT::GetProject(HWND hdlg)
{
	char	folder[128];
	GetDlgItemText(hdlg,IDC_NAME,folder,sizeof(folder));

	SetCurrentDirectory("data\\Scenario");

	//if	(IsFolder(folder))	//	이미 같은 이름의 프로젝트 폴더가 존재한다.
	//	return	cMSG::Put(dMSG_NEW_PROJECT_ERROR,dMSG_ERROR_EXIST_PROJECT);

	if	(!cFOLDER::Create(folder,TRUE))	//	프로젝트 폴더 생성에 실패했다.
		return	cMSG::Put(dMSG_NEW_PROJECT_ERROR,dMSG_ERROR_CAN_NOT_CREATE_PROJECT_FOLDER);

	cMSG::Put("cPROJECT::GetProject",s_strName);

	strcpy(s_strName,folder);

	SetProjectFolder();
	cFOLDER::Create("Map");

	SetLogFolder();

	return	TRUE;
}

//
//	시나리오 폴더로 이동
BOOL
cPROJECT::SetScenarioFolder()
{
	if (!SetCurrentDirectory(_ms("%s\\data\\Scenario",_logFolder)))
	{
		if (!SetCurrentDirectory(_ms("%s\\data",_logFolder)))
			return	FALSE;

		cFOLDER::Create("Scenario");

		return	SetCurrentDirectory(_ms("%s\\data\\Scenario",_logFolder));
	}

	return	TRUE;
}

//
//	프로젝트 폴더로 이동
BOOL
cPROJECT::SetProjectFolder()
{
	if (!SetCurrentDirectory(_ms("%s\\data\\Scenario\\%s",_logFolder,s_strName)))
	{
		SetScenarioFolder();

		cMSG::Put("SetProjectFolder",s_strName);

		cFOLDER::Create(s_strName);

		return	SetCurrentDirectory(_ms("%s\\data\\Scenario\\%s",_logFolder,s_strName));
	}

	return	TRUE;
}

//
//	_lpstrFolder에 맵 폴더를 담아준다.
void
cPROJECT::GetMapFolder(char *_lpstrFolder)
{
	strcpy(_lpstrFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));
}

//
//	_lpstrFolder에 던젼 폴더를 담아준다.
void
cPROJECT::GetDungeonFolder(char *_lpstrFolder)
{
	strcpy(_lpstrFolder,_ms("%s\\data\\Scenario\\%s\\dungeon",_logFolder,s_strName));
}

//
//	맵 폴더로 이동
BOOL
cPROJECT::SetMapFolder(char *_lpstrDestFolder)
{
	char	strFolderName[512];

	if	(_lpstrDestFolder)
		sprintf(strFolderName,"%s/%s/data/Scenario/%s/map",_logFolder,_lpstrDestFolder,s_strName);
	else
		sprintf(strFolderName,"%s/data/Scenario/%s/map",_logFolder,s_strName);

	if	(!SetCurrentDirectory(strFolderName))
	{
		if	(_lpstrDestFolder)
			return	ERRMSG("can not find follow folder",strFolderName);

		SetProjectFolder();

		cFOLDER::Create("Map");

		return	SetCurrentDirectory(_ms("%s/data/Scenario/%s/map",_logFolder,s_strName));
	}

	return	TRUE;
}

//
//	던젼 폴더로 이동
BOOL
cPROJECT::SetDungeonFolder()
{
	if	(!SetCurrentDirectory(_ms("%s\\data\\Scenario\\%s\\dungeon",_logFolder,s_strName)))
	{
		SetProjectFolder();

		cFOLDER::Create("Dungeon");

		return	SetCurrentDirectory(_ms("%s\\data\\Scenario\\%s\\dungeon",_logFolder,s_strName));
	}

	return	TRUE;
}	//	cPROJECT::SetProjectFolder()

//
//	_strFn에서 프로젝트 맵 폴더를 제외한 맵 이름을 뽑아낸다.
char*
cPROJECT::ExportMapFileName(char *_strFn)
{
	static	char	s_strMapName[512];

	char	str[256];
	char	strMapFolder[256];

	strcpy(str,_strFn);
	strcpy(strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));
	if (stricmp(_strFn,strMapFolder)	==	0)	return	NULL;	//	폴더 없음

	str[strlen(strMapFolder)]	=	0;

	if (stricmp(str,strMapFolder)	==	0)
			strcpy(s_strMapName,str+strlen(strMapFolder)+1);
	else	strcpy(s_strMapName,_strFn);

	return	s_strMapName;
}

//
//
//
BOOL
cPROJECT::LoadProject()
{
	char	fn[256];

	strcpy(fn,_ms("%s.rpd",s_strName));

	return	LoadProject(fn);
}

//
//
BOOL
cPROJECT::SaveProject()
{
	char	fn[256];

	strcpy(fn,_ms("%s.rpd",s_strName));

	return	SaveProject(fn);
}

//
//	아이템 데이터 저장하기
BOOL
cPROJECT::SaveItem(char *_lpstrFileName,CCustomItemManager *_lpPremiumItem,CCustomItemManager *_lpCustomItem,
					cITEM_PREFIX_MANAGER *_lpIPM,cArcaManager *_lpAM,cItemSetManager *_lpISM,cPieceItemManager *_lpPIM,cBASIC_ITEM *_lpBIL,BOOL _bIsLocalize)

{
	char					*lpstrFileName;
	int						iValue	=	0x12345678;

	if (_lpstrFileName)
		lpstrFileName		=	_lpstrFileName;
	else
		lpstrFileName		=	"item.dat";

	if	(_lpPremiumItem	==	NULL)
		_lpPremiumItem	=	&g_premiumItem;
	if	(_lpCustomItem	==	NULL)
		_lpCustomItem	=	&g_customItem;
	if	(_lpIPM			==	NULL)
		_lpIPM			=	&g_itemPrefix;
	if	(_lpAM			==	NULL)
		_lpAM			=	&g_arcaManager;
	if	(_lpISM			==	NULL)
		_lpISM			=	&g_itemSetManager;
	if	(_lpPIM			==	NULL)
		_lpPIM			=	&g_pieceItemManager;
	if	(_lpBIL			==	NULL)
		_lpBIL			=	g_aBasicItem;
	
	if	(!SetProjectFolder())
		return	FALSE;

	{
		char	strItemBackUp[512];

		sprintf(strItemBackUp,"backup_item1.dat",lpstrFileName);
		SetFileAttributes(strItemBackUp,FILE_ATTRIBUTE_ARCHIVE);

		CopyFile(lpstrFileName,strItemBackUp,FALSE);
	}

	cFILE	file,textFile;

	if	(!file.Open("_________tempItem.tmp","wb"))
	{
		SetLogFolder();

		return	ERRMSG(dMSG_WRITE_FILE_ERROR,_ms("%s\n\n'%s'",dMSG_CAN_NOT_OPEN_FOLLOW_FILE,"_________tempItem.tmp"));
	}

	textFile.Open("____itemText.tmp","wb");

	{
		int		iLocalizeNation	= eNationJapan;
		
		if	(_bIsLocalize)
			iLocalizeNation	=	g_iLocalizeNation;
		
		file.Write(&iLocalizeNation,sizeof(iLocalizeNation));
	}

	int	iLastIndex	=	cBASIC_ITEM::GetLastIndex(_lpBIL)+1;		//	마지막으로 저장된 아이템을 구한다.
	int	iSaveIndex	=	iLastIndex;

	cMessSign::SetDataEncodeTable(eNationJapan);

	if	(_bIsLocalize)
		cMessSign::SetDataEncodeTable(g_iLocalizeNation);

	cMessSign::EncodeDataBuffer2(&iSaveIndex,4);
	file.Write(&iSaveIndex,4);								//	저장할 아이템의 수

	file.Write(&iLastIndex,4);								//	텍스트 데이터가 들어가는 위치

	for (int i = 0 ; i < iLastIndex;i++)
	{
		if (_lpBIL[i].m_attr.isExtraItem)
		{
			_lpBIL[i].m_attr.isCanNotDestroyItem			=	FALSE;	//	파괴 불가 아이템
			_lpBIL[i].m_attr.isCanNotSelllItem				=	FALSE;	//	판매 불가 아이템
		}

		if (_lpBIL[i].m_iSerial	!=	dVALID_ITEM)
		{
			_lpBIL[i].m_dwCommentAddress	=	textFile.Locate();

			RemoveQuotationMark(_lpBIL[i].m_strName);
			RemoveQuotationMark(_lpBIL[i].m_pItemComment);
			textFile.Write(&_lpBIL[i].m_dwCommentSize,4);	//	아이템 설명 사이즈 저장
			textFile.Write(_lpBIL[i].m_pItemComment,_lpBIL[i].m_dwCommentSize);	//	아이템 설명 저장
		}

		if (_lpBIL[i].m_wStackLimit	>	1)
			if (_lpBIL[i].isUnique() && !_lpBIL[i].isInfinityBullet())
				_lpBIL[i].addInfinityBulletOption();

		cBASIC_ITEM	saveItem;
		
		memcpy(&saveItem,&_lpBIL[i],sizeof(cBASIC_ITEM)-8);
		cMessSign::EncodeDataBuffer2(&saveItem,sizeof(cBASIC_ITEM)-8);

		file.Write(&saveItem,sizeof(cBASIC_ITEM)-8);
	}

//	아이템 접두사 저장
	{
		int	iCount		=	_lpIPM->getCount();
		int	iSaveIndex	=	iCount;

		cMessSign::EncodeDataBuffer2(&iSaveIndex,4);
		file.Write(&iSaveIndex,4);								//	저장할 아이템의 수

		for (int i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
		{
			cITEM_PREFIX	*lpPrefix	=	_lpIPM->get(i);

			if	(!lpPrefix)
				break;

			cITEM_PREFIX	saveData;

			memcpy(&saveData,lpPrefix,sizeof(cITEM_PREFIX));

			cMessSign::EncodeDataBuffer2(&saveData,sizeof(cITEM_PREFIX));

			file.Write(&saveData,sizeof(cITEM_PREFIX));
		}
	}

//	프리미엄 아이템 정보 저장
	{
		int			iCount			=	_lpPremiumItem->getLastIndex();
		CCustomItem	*lpCustomItemBay=	_lpPremiumItem->getItemBay();
		int			iSaveCount		=	iCount;

		cMessSign::EncodeDataBuffer2(&iSaveCount,sizeof(iSaveCount));

		file.Write(&iSaveCount,4);

		for (int i=0;i<iCount;i++)
		{
			CCustomItem	saveData;

			memcpy(&saveData,lpCustomItemBay+i,sizeof(CCustomItem));

			cMessSign::EncodeDataBuffer2(&saveData,sizeof(CCustomItem));

			file.Write(&saveData,sizeof(CCustomItem));
		}
	}

//	커스텀 아이템 정보 저장
	{
		int			iCount			=	_lpCustomItem->getLastIndex();
		CCustomItem	*lpCustomItemBay=	_lpCustomItem->getItemBay();

		int			iSaveCount		=	iCount;

		cMessSign::EncodeDataBuffer2(&iSaveCount,sizeof(iSaveCount));

		file.Write(&iSaveCount,4);

		for (int i=0;i<iCount;i++)
		{
			CCustomItem	saveData;

			memcpy(&saveData,lpCustomItemBay+i,sizeof(CCustomItem));

			cMessSign::EncodeDataBuffer2(&saveData,sizeof(CCustomItem));

			file.Write(&saveData,sizeof(CCustomItem));
		}
	}

//	보물 상자 저장
	{
		int			iCount			=	_lpAM->getLastIndex()+1;
		cArcaInfo*	lpArcaBay		=	_lpAM->getBay();

		int			iSaveCount		=	iCount;

		cMessSign::EncodeDataBuffer2(&iSaveCount,sizeof(iSaveCount));

		file.Write(&iSaveCount,4);

		for (int i=0;i<iCount;i++)	
		{
			cArcaInfo	arca;
			
			memcpy(&arca,lpArcaBay+i,sizeof(cArcaInfo));

			cMessSign::EncodeDataBuffer2(&arca,sizeof(cArcaInfo));

			file.Write(&arca,sizeof(cArcaInfo));
		}
	}

	iValue	=	0x12345674;

	file.Write(&iValue,4);

//	세트 아이템 저장
	{
		int			iCount	=	_lpISM->getLastIndex()+1;
		cItemSet*	lpBay	=	_lpISM->getBay();

		int			iSaveCount		=	iCount;

		cMessSign::EncodeDataBuffer2(&iSaveCount,sizeof(iSaveCount));

		file.Write(&iSaveCount,4);

		for	(int	i=0;i<iCount;i++)
		{
			cItemSet	*lpSet	=	&lpBay[i];

			if	(lpSet->m_wSerial	!=	0xffff)
			{
				lpSet->m_dwCommentAddress	=	textFile.Locate();

				RemoveQuotationMark(lpSet->m_strName);
				RemoveQuotationMark(lpSet->m_pItemComment);
				textFile.Write(&lpSet->m_wCommentSize,2);	//	아이템 설명 사이즈 저장
				textFile.Write(lpSet->m_pItemComment,lpSet->m_wCommentSize);	//	아이템 설명 저장

				for	(int	j=0;j<dMAX_ITEM_COUNT_IN_SET;j++)
				{
					cSetItem	*lpItem	=	(cSetItem	*)&lpSet->m_aItem[j];

					if	(lpSet->m_aItem[j].m_wBaseItem	!=	0xffff)
					{
						lpItem->m_dwCommentAddress	=	textFile.Locate();

						RemoveQuotationMark(lpItem->m_strName);
						RemoveQuotationMark(lpItem->m_pItemComment);
						textFile.Write(&lpItem->m_wCommentSize,2);	//	아이템 설명 사이즈 저장
						textFile.Write(lpItem->m_pItemComment,lpItem->m_wCommentSize);	//	아이템 설명 저장
					}
				}
			}

			file.Write(lpSet,sizeof(cItemSetDefine));
		}
	}

//	조각 아이템 저장
	{
		int			iCount			=	_lpPIM->getLastIndex()+1;
		cPieceItem*	lpBay			=	_lpPIM->getBay();

		int			iSaveCount		=	iCount;

		cMessSign::EncodeDataBuffer2(&iSaveCount,sizeof(iSaveCount));

		file.Write(&iSaveCount,4);

		for	(int	i=0;i<iCount;i++)
		{
			cPieceItem	saveData;
			
			memcpy(&saveData,lpBay+i,sizeof(cPieceItem));
			cMessSign::EncodeDataBuffer2(&saveData,sizeof(cPieceItem));
			file.Write(&saveData,sizeof(cPieceItem));
		}
	}

	textFile.Close();

	int	iTextLocate	=	file.Locate();

	file.Seek(8,SEEK_SET);
	file.Write(&iTextLocate,4);
	file.Seek(iTextLocate,SEEK_SET);

	file.Append("____itemText.tmp");

	int	iTempFileLocate	=	file.Locate();

	{
		file.Seek(0,SEEK_END);

		int	iAddVersionChecker	=	dITEM_VERSION_CHECKER;
		int	iVersion			=	dLAST_ITEM_DATA_VERSION;

		file.Write(&iAddVersionChecker,sizeof(iAddVersionChecker));
		file.Write(&iVersion,sizeof(iVersion));
	}

	file.Close();							//	닫는다.

	int	iTempFileSize	=	cFILE::Size("_________tempItem.tmp");

	SetProjectFolder();
	DeleteFile("____itemText.tmp");

	SetFileAttributes(lpstrFileName,FILE_ATTRIBUTE_ARCHIVE);
	SetFileAttributes("_________tempItem.tmp",FILE_ATTRIBUTE_ARCHIVE);

	{
		char	strItemBackUp[512];

		sprintf(strItemBackUp,"backup_item.dat",lpstrFileName);
		SetFileAttributes(strItemBackUp,FILE_ATTRIBUTE_ARCHIVE);

		DeleteFile(strItemBackUp);
		MoveFile(lpstrFileName,strItemBackUp);

		if	(MoveFile("_________tempItem.tmp",lpstrFileName))
		{
			if	(cFILE::Size(lpstrFileName)	<	10*1024)
				cMSG::Put("Warning!!","There is a problem saving item data!! Case 3\n\nPlease contact the coolest guy in LNK!!\n\n%d,%d",iTempFileLocate,iTempFileSize) ;
			else
				DeleteFile(strItemBackUp);

			if	(IsFile(lpstrFileName)	==	FALSE)
				cMSG::Put("Warning!!","There was a problem saving item data!! Case 1\n\nPlease contact the coolest guy in LNK!!");
		}
		else
			cMSG::Put("Warning!!","There was a problem saving item data!! Case 2\n\nPlease contact the coolest guy in LNK!!");

	}

	SetLogFolder();

	return	TRUE;
}

//
//	아이템 데이터 불러오기
BOOL
cPROJECT::LoadItem(char *_lpstrFileName,CCustomItemManager *_lpPremiumItem,CCustomItemManager *_lpCustomItem,cITEM_PREFIX_MANAGER *_lpIPM,cArcaManager *_lpAM,
				   cItemSetManager *_lpISM,cPieceItemManager *_lpPIM,cBASIC_ITEM *_lpBIL)
{	//마지막 수정일 : 09.10.13
	if	(!SetProjectFolder())
		return	FALSE;

	cFILE	file,textFile;
	int		iLastIndex,iTextDataLocate,iBufferSize=1024;
	BOOL	bIsByLocal	=	FALSE;

	if	(_lpPremiumItem	==	NULL)
		_lpPremiumItem	=	&g_premiumItem;
	if	(_lpCustomItem	==	NULL)
		_lpCustomItem	=	&g_customItem;
	if	(_lpIPM			==	NULL)
		_lpIPM			=	&g_itemPrefix;
	if	(_lpAM			==	NULL)
		_lpAM			=	&g_arcaManager;
	if	(_lpISM			==	NULL)
		_lpISM			=	&g_itemSetManager;
	if	(_lpPIM			==	NULL)
		_lpPIM			=	&g_pieceItemManager;
	if	(_lpBIL			==	NULL)
		_lpBIL			=	g_aBasicItem;

	cBASIC_ITEM::Reset(_lpBIL);

	_lpPremiumItem->init(dMAX_INIT_CUSTOM_ITEM_COUNT);
	_lpCustomItem->init(dMAX_INIT_CUSTOM_ITEM_COUNT);

	if	(_lpstrFileName	==	NULL)
		_lpstrFileName	=	"item.dat";
	else
		bIsByLocal		=	TRUE;

	if	(!file.Open(_lpstrFileName,"rb"))
	{
		SetLogFolder();

		return	TRUE;
	}

	int	iVersion			=	-1;

	{
		file.Seek(-8,SEEK_END);

		int	iAddVersionChecker	=	dITEM_VERSION_CHECKER;

		file.Read(&iAddVersionChecker,sizeof(iAddVersionChecker));

		if	(iAddVersionChecker	==	dITEM_VERSION_CHECKER)
			file.Read(&iVersion,sizeof(iVersion));

		file.Seek(0,SEEK_SET);
	}
	
	char*	pBuffer;
	pBuffer	=	new char [iBufferSize];

	textFile.Open(_lpstrFileName,"rb");

	int	iLocalizeNation	= eNationJapan;
	if	(iVersion	>=	eITEMDATAVERSION_LAST)
	{	//잉 ? 아이템버전이 마지막버전보다 크다고?? 이상함..-,.-;
		pKILL(pBuffer);
		return FALSE;
	}
	else
	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT2)
	{
		file.Read(&iLocalizeNation,sizeof(iLocalizeNation));
		cMessSign::SetDataEncodeTable(iLocalizeNation);
	}
	else
		cMessSign::SetDataEncodeTable(-1);

	file.Read(&iLastIndex,4);
	file.Read(&iTextDataLocate,4);	

	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		cMessSign::DecodeDataBuffer2(&iLastIndex,4);

	if(iLastIndex >= dBASIC_ITEM_COUNT)
	{
		// 뭐징 마지막인덱스가... 아이템 최대 인덱스보다 크당??
		pKILL(pBuffer);
		return FALSE;
	}
	for (int i = 0 ; i < iLastIndex;i++)
	{
		file.Read(&_lpBIL[i],sizeof(CBasicItemData));

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&_lpBIL[i],sizeof(CBasicItemData));

		CBasicItemData	*lpBID	=	&_lpBIL[i];

		if	(iVersion	<	eITEMDATAVERSION_RENEWAL_EQUIPABLE_JOB_INFO)
		{
			lpBID->m_uiEquipAbleJobMask	=	0;

			int	iHitCount	=	0;

			for (int iJob=0;iJob<16;iJob++)
			{
				if	(lpBID->m_aEnableJob[iJob])
				{
					lpBID->m_uiEquipAbleJobMask	+=	(1<<iJob);
					iHitCount++;
				}
			}

			if	(iHitCount	==	16)
				lpBID->m_uiEquipAbleJobMask	=	0xffffffff;

			if	(lpBID->m_aEnableJob[16])
				lpBID->m_bf1IsMaleJobEquipAble		=	TRUE;

			if	(lpBID->m_aEnableJob[17])
				lpBID->m_bf1IsFemaleJobEquipAble	=	TRUE;

			memset(lpBID->m_aEnableJob,0,sizeof(lpBID->m_aEnableJob));
		}

		if (_lpBIL[i].m_iSerial	>=	dBASIC_ITEM_COUNT)
			_lpBIL[i].m_iSerial	=	dVALID_ITEM;

//		if	(_lpBIL[i].m_attr.premiumLevel	&&	_lpBIL[i].m_wPremiumLevel	==	0)			09.10.13 
//			_lpBIL[i].m_wPremiumLevel	=	_lpBIL[i].m_attr.premiumLevel;					 프리미엄등급을 강제로 변경못하게 주석처리함..

		if (_lpBIL[i].m_questIndex	!=	dNOT_QUEST_ITEM)
		{
			if (_lpBIL[i].m_questIndex>	dNOT_QUEST_ITEM)
				_lpBIL[i].m_questIndex=	dNOT_QUEST_ITEM;

			_lpBIL[i].m_wKind			=	eIK_QUEST_ITEM;
		}

		if (_lpBIL[i].m_wBoostDurability	==	0)
			_lpBIL[i].m_wBoostDurability	=	100;

		if (_lpBIL[i].m_questKind	>	eQT_ARBEIT)	
			_lpBIL[i].m_questKind	=	eQT_NORMAL;

		if	(_lpBIL[i].m_wStackLimit	>	1)
			if	(_lpBIL[i].isUnique() && !_lpBIL[i].isInfinityBullet())
			{
				if (!_lpBIL[i].addInfinityBulletOption())
					ERRMSG("아이템 데이터 로딩 에러!!","유니크 아이템에 무한 탄환 옵션을 추가 할 수 없습니다.");
			}

		if	(_lpBIL[i].m_iSerial	!=	dVALID_ITEM)
		{
			int	iTextSize;

			textFile.Seek(_lpBIL[i].m_dwCommentAddress+iTextDataLocate,SEEK_SET);
			textFile.Read(&iTextSize,4);

			if (iTextSize	> iBufferSize)
			{
				iBufferSize	=	iTextSize;

				pKILL(pBuffer);

				pBuffer		=	new char [iBufferSize];
			}

			textFile.Read(pBuffer,iTextSize);

			RemoveQuotationMark(pBuffer);
			_lpBIL[i].setComment(pBuffer);	//	스트림 설정.

			if (_lpBIL[i].m_wKind		>=	eIK_END)
				_lpBIL[i].m_iSerial	=	dVALID_ITEM;
		}
	}

//	아이템 접두사 로딩
	{
		int	iCount;

		_lpIPM->reset();

		file.Read(&iCount,4);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);

		if(iCount >= dMAX_ITEM_PREFIX_COUNT)
		{// 접두사 최대 갯수 보다 크다궁?
			pKILL(pBuffer);
			return FALSE;
		}
		for (int i=0;i<iCount;i++)
		{
			cITEM_PREFIX		itemPrefix;

			file.Read(&itemPrefix,sizeof(cITEM_PREFIX));

			if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&itemPrefix,sizeof(cITEM_PREFIX));

			_lpIPM->insert(itemPrefix.m_wSerial,&itemPrefix);
		}
	}//	아이템 접두사 로딩

//	프리미엄 아이템 정보 로딩
	{
		int			iCount;
		CCustomItem	*lpCustomItemBay=	_lpPremiumItem->getItemBay();

		file.Read(&iCount,4);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);

		if (iCount)
			file.Read(lpCustomItemBay,sizeof(CCustomItem)*iCount);

		if(iCount >= dMAX_INIT_CUSTOM_ITEM_COUNT)
		{//	 최대치보다 커??
			pKILL(pBuffer);
			return FALSE;
		}
		for	(int	i=0;i<iCount;i++)
		{
			if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&lpCustomItemBay[i],sizeof(CCustomItem));

			lpCustomItemBay[i].m_strName[29]	=	NULL;
		}

		_lpPremiumItem->rebuild();
	}

//	프리미엄 아이템 정보 로딩
	{
		int			iCount;
		CCustomItem	*lpCustomItemBay=	_lpCustomItem->getItemBay();

		file.Read(&iCount,4);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);

		if (iCount)
			file.Read(lpCustomItemBay,sizeof(CCustomItem)*iCount);

		if(iCount >= dMAX_INIT_CUSTOM_ITEM_COUNT)
		{//	 최대치보다 커??
			pKILL(pBuffer);
			return FALSE;
		}

		for	(int	i=0;i<iCount;i++)
		{
			if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&lpCustomItemBay[i],sizeof(CCustomItem));
		}

		_lpCustomItem->rebuild();
	}

//	보물 상자 정보 로딩
	{
		int			iCount;
		cArcaInfo*	lpArcaBay		=	_lpAM->getBay();

		file.Read(&iCount,4);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);

		if	(iCount >=	dMAX_ARCA_COUNT)
		{//	 최대치보다 커??
			pKILL(pBuffer);
			return FALSE;
		}
		if	(iCount)	
			file.Read(lpArcaBay,sizeof(cArcaInfo)*iCount);

		for	(int	i=0;i<iCount;i++)
		{
			if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&lpArcaBay[i],sizeof(cArcaInfo));
		}

		_lpAM->rebuild();
	}

	int	iValue;
	file.Read(&iValue,4);

	_lpISM->reset();
	_lpPIM->reset();

	if	(iValue	==	0x12345674)
//	세트 아이템 로딩
	{
		int			iCount,i;
		cItemSet*	lpBay		=	_lpISM->getBay();

		file.Read(&iCount,4);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);

		if	(iCount >=	dMAX_SET_ITEM_COUNT)
		{//	 최대치보다 커??
			pKILL(pBuffer);
			return FALSE;
		}
		for	(i=0;i<iCount;i++)
		{
			cItemSet	*lpSet	=	&lpBay[i];

			if	(iVersion	<	eITEMDATAVERSION_RENEWAL_EQUIPABLE_JOB_INFO)
			{
				file.Read(lpSet,sizeof(cItemSetDefine)-sizeof(lpSet->m_aItem));

				for (int iSetItem=0;iSetItem<dMAX_ITEM_COUNT_IN_SET;iSetItem++)
				{
					cSetItemDefine	*lpSetItem	=	&lpSet->m_aItem[iSetItem];

					file.Read(lpSetItem,sizeof(cSetItemDefine)-4);

					lpSetItem->m_uiEnableJobMask	=	lpSetItem->m_wDummyData;
					lpSetItem->m_wDummyData			=	0;
					lpSetItem->m_pItemComment		=	NULL;
				}

				lpSet->m_pItemComment	=	NULL;
			}
			else
				file.Read(lpSet,sizeof(cItemSetDefine));

			{
				WORD	wTextSize;

				textFile.Seek(lpSet->m_dwCommentAddress+iTextDataLocate,SEEK_SET);
				textFile.Read(&wTextSize,2);

				if	(wTextSize	> iBufferSize)
				{
					iBufferSize	=	wTextSize;

					pKILL(pBuffer);

					pBuffer		=	new char [iBufferSize];
				}

				textFile.Read(pBuffer,wTextSize);

				lpSet->setComment(pBuffer);	//	스트림 설정.
			}

			for	(int	j=0;j<dMAX_ITEM_COUNT_IN_SET;j++)
			{
				cSetItem	*lpSetItem	=	(cSetItem	*)&lpSet->m_aItem[j];

				lpSetItem->m_pItemComment=	NULL;

				if	(lpSetItem->m_wBaseItem	==	0xffff)
					continue;

				WORD	wTextSize;

				textFile.Seek(lpSetItem->m_dwCommentAddress+iTextDataLocate,SEEK_SET);
				textFile.Read(&wTextSize,2);

				if	(wTextSize	> iBufferSize)
				{
					iBufferSize	=	wTextSize;

					pKILL(pBuffer);

					pBuffer		=	new char [iBufferSize];
				}

				textFile.Read(pBuffer,wTextSize);

				lpSetItem->setComment(pBuffer);	//	스트림 설정.
			}
		}

//	조각 아이템 로딩
		cPieceItem*	lpPieceItemBay	=	_lpPIM->getBay();

		file.Read(&iCount,4);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);

		if	(iCount >=	dMAX_PIECE_ITEM_COUNT)
		{//	 최대치보다 커??
			pKILL(pBuffer);
			return FALSE;
		}

		if (iCount)	
			file.Read(lpPieceItemBay,sizeof(cPieceItem)*iCount);

		for (i=0;i<iCount;i++)	
		{
			if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
				cMessSign::DecodeDataBuffer2(lpPieceItemBay+i,sizeof(cPieceItem));
		}

	}

	_lpISM->build();
	_lpPIM->build();

	textFile.Close();

	{
// 		file.Seek(-8,SEEK_END);
// 
// 		int	iAddVersionChecker;
// 		int	iVersion	=	-1;
// 
// 		file.Read(&iAddVersionChecker,sizeof(iAddVersionChecker));
// 
// 		if	(iAddVersionChecker	==	dITEM_VERSION_CHECKER)
// 			file.Read(&iVersion,sizeof(iVersion));
		cBASIC_ITEM::Arrange(_lpBIL,iVersion);
	}

	file.Close();

	if	(iVersion	!=	dLAST_ITEM_DATA_VERSION && bIsByLocal == FALSE)
	{
		SaveItem();

		cMSG::Put("Item data conversion","%s\n\nThe file is an old version, so it was updated to a new version and saved.",_lpstrFileName);
	}

	SetLogFolder();

	pKILL(pBuffer);

	return	TRUE;
}


BOOL
cPROJECT::ReadCorrectJobData(cJOB_DEN *_lpDen)
{
	cFILE	file;

	char	*strCorrectMonsterExpScriptFile[]	=	
	{
//		"data/Scripter/correctMonsterExp.txt",
		"data/Scripter/correctMonsterExp - for Japan.txt",
//		"data/Scripter/correctMonsterExp - for China.txt",
		"end",
	};

	cDECODER	decoder;
	int			iLanguage	=	0;

	while(1)
	{
		if	(stricmp(strCorrectMonsterExpScriptFile[iLanguage],"end")==0)
			break;

		if	(!decoder.Upload(strCorrectMonsterExpScriptFile[iLanguage],NULL))
			return	FALSE;

		while(1)
		{
			char		*lpstrName	=	decoder.GetStream();

			if	(stricmp(lpstrName,"end")	==	0)
				break;

			cJOB		*lpJob		=	_lpDen->getJob(lpstrName);

			if	(!lpJob)
			{
				SetLogFolder();

				return	FALSE;
			}

			int	iExp				=	decoder.GetNumber();

			lpJob->m_dwDeathExp		=	iExp;
		}

		iLanguage++;
	}

	return	TRUE;
}

//
//	직업 데이터 저장하기
BOOL
cPROJECT::SaveJob(cJOB_DEN *_lpJobDen,char *_lpstrFn)
{
	if	(!SetProjectFolder())
		return	FALSE;

	if	(_lpJobDen	==	NULL)
		_lpJobDen	=	&g_denJob;

	{
		for (int iLoop=0;iLoop<10;iLoop++)
			for (int i=0;i<dMAX_JOB_COUNT;i++)
			{
				cJOB	*lpJob	=	_lpJobDen->getJob(i);
				
				if	(!lpJob)
					continue;
				
				for (int k=0;k<dMAX_MONSTER_ABILITY_COUNT;k++)
				{
					if	(lpJob->m_aAbility[k].m_wSkill	==	0xffff)
					{
						for (int j=k;j<dMAX_MONSTER_ABILITY_COUNT-1;j++)
						{
							lpJob->m_aAbility[j].m_wSkill	=	lpJob->m_aAbility[j+1].m_wSkill;
							lpJob->m_aAbility[j].m_wLevel	=	lpJob->m_aAbility[j+1].m_wLevel;
						}
						
						lpJob->m_aAbility[dMAX_MONSTER_ABILITY_COUNT-1].m_wSkill	=	0xffff;
						lpJob->m_aAbility[dMAX_MONSTER_ABILITY_COUNT-1].m_wLevel	=	0;
					}
				}
			}
	}

	if	(_lpstrFn	==	NULL)
		_lpstrFn	=	"job.dat";

	cFILE	file;

	if	(!file.Open(_lpstrFn,"wb"))
	{
		SetLogFolder();

		return	ERRMSG(dMSG_WRITE_FILE_ERROR,_ms("%s\n\n\"job.dat\"",dMSG_CAN_NOT_OPEN_FOLLOW_FILE));
	}

	int	iVersion=dJobDataFileCurrentVerion,iVersionSignCode=0x12345678;

	file.Write(&iVersionSignCode,4);
	file.Write(&iVersion,4);
	file.Write(&_lpJobDen->m_iCount,4);

	for (int i=0;i<dMAX_JOB_COUNT;i++)
	{
		cJOB	*lpJob	=	_lpJobDen->getJob(i);

		if	(lpJob)	
			file.Write(lpJob,sizeof(cJOB));
	}

	file.Close();

	SetLogFolder();


	return	TRUE;
}

//
//	직업 로딩
BOOL
cPROJECT::LoadJob(cJOB_DEN *_lpJobDen,char *_lpstrFileName)
{
	if	(_lpstrFileName	==	NULL)
	{
		_lpstrFileName	=	"job.dat";

		if	(!SetProjectFolder())
			return	FALSE;
	}

	if	(_lpJobDen	==	NULL)
		_lpJobDen	=	&g_denJob;

	cFILE	file;

	//	"job.dat" 직업 데이터 불러오기
	if	(!file.Open(_lpstrFileName,"rb"))
	{
		SetLogFolder();

		return	TRUE;
	}

	int	iCount,iVersion=-1,iVersionSign,iVersionSignCode=0x12345678;

	file.Read(&iVersionSign,4);

	if	(iVersionSign	!=	iVersionSignCode)
	{
		file.Rewind();
		file.Read(&iCount,4);
	}
	else
	{
		file.Read(&iVersion,4);
		file.Read(&iCount,4);
	}

	for (int i=0;i<iCount;i++)
	{
		cJOB	tempJob,*lpJob;

		file.Read(&tempJob,sizeof(cJOB));		//	직업 인덱스

		int	iIndex	=	tempJob.m_wBody-dBODY_MONSTER_START;

		if	(iVersion	<	eJobDataFileVersion_begin)
			tempJob.m_bf3RequireRebirthCountForTame	=	0;

		lpJob		=	&_lpJobDen->m_aJobList[tempJob.m_iSerial];

		if	(strlen(tempJob.m_strName) >= 24)
			tempJob.m_strName[0]	=	tempJob.m_strName[0];

		memcpy(lpJob,&tempJob,sizeof(cJOB));
	}

	_lpJobDen->rebuild();
	file.Close();

	SetLogFolder();

	return	TRUE;
}

class	cOldBookSaveDataDefine
{
public:
	int					m_wCategoryCount;
	char				m_strBookName[c_iBookNameLength];
};


void
cPROJECT::LoadBook(cFILE *_lpFile,BOOL _bIsOldData,cBookManager *_lpBM)
{
	if	(_lpBM	==	NULL)
		_lpBM	=	&g_book;
	
	_lpBM->resetBookManager();

	_lpFile->Read(&_lpBM->m_iBookCount,4);

	for(int iBook=0;iBook<_lpBM->m_iBookCount;iBook++)
	{
		cBookInfo	*lpBook	=	&_lpBM->m_aBook[iBook];

		if	(_bIsOldData)
		{
			cOldBookSaveDataDefine	data;
			_lpFile->Read(&data,sizeof(cOldBookSaveDataDefine));

			memcpy(lpBook,&data,sizeof(data));
			lpBook->setItemComment(" ");
		}
		else
		{
			_lpFile->Read(lpBook,sizeof(cBookSaveDataDefine));

			pKILL(lpBook->m_pstrText);
			lpBook->m_pstrText	=	new char [lpBook->m_wTextLength];

			if	(lpBook->m_pstrText)
				_lpFile->Read(lpBook->m_pstrText,lpBook->m_wTextLength);
			else
				_lpFile->Seek(lpBook->m_wTextLength);
		}

		for(int iCategory=0;iCategory<lpBook->m_wCategoryCount;iCategory++)
		{
			cBookCategory	*lpCategory	=	lpBook->getCategory(iCategory);

			_lpFile->Read(lpCategory,sizeof(cBookCategorySaveDataDefine));

			for(int iPage=0;iPage<lpCategory->m_iPageCount;iPage++)
			{
				cBookPageInfo	*lpPage	=	lpCategory->getPage(iPage);

				_lpFile->Read(lpPage,sizeof(cBookPageSaveDataDefine));

				pKILL(lpPage->m_pstrText);
				lpPage->m_pstrText		=	new char [lpPage->m_wTextLength];

				if	(lpPage->m_pstrText)
					_lpFile->Read(lpPage->m_pstrText,lpPage->m_wTextLength);
			}
		}
	}
}

void
cPROJECT::SaveBook(cFILE *_lpFile,cBookManager *_lpBM)
{
	if	(_lpBM	==	NULL)
		_lpBM	=	&g_book;

	_lpFile->Write(&_lpBM->m_iBookCount,4);

	for(int iBook=0;iBook<_lpBM->m_iBookCount;iBook++)
	{
		cBookInfo	*lpBook	=	&_lpBM->m_aBook[iBook];

		lpBook->m_dwTextPos	=	_lpFile->Locate()+sizeof(cBookSaveDataDefine);
		_lpFile->Write(lpBook,sizeof(cBookSaveDataDefine));
		_lpFile->Write(lpBook->m_pstrText,lpBook->m_wTextLength);

		for(int iCategory=0;iCategory<lpBook->m_wCategoryCount;iCategory++)
		{
			cBookCategory	*lpCategory	=	lpBook->getCategory(iCategory);

			_lpFile->Write(lpCategory,sizeof(cBookCategorySaveDataDefine));

			for(int iPage=0;iPage<lpCategory->m_iPageCount;iPage++)
			{
				cBookPageInfo	*lpPage	=	lpCategory->getPage(iPage);

				lpPage->m_dwTextPos		=	_lpFile->Locate()+sizeof(cBookPageSaveDataDefine);
				_lpFile->Write(lpPage,sizeof(cBookPageSaveDataDefine));
				_lpFile->Write(lpPage->m_pstrText,lpPage->m_wTextLength);
			}
		}
	}
}

//
//	프로젝트 데이터를 저장한다.
BOOL
cPROJECT::SaveProject(char *fn,char *_lpstrDestFolder,CQuest *_lpQuest,CQuest *_lpArbeit,cSecretDungeonManager *_lpSDM,cChapterManager *_lpCM,cBookManager *_lpBM,
					CKarmaContainer	*_lpKC,CKarmaContainer	*_lpKCMonsterGen,BOOL _bIsLocalize)
{
	cFILE			file,TextFile;
	DWORD			size;
	int				i=0,j=0,iCount=0,iTextSize=0,iTextLocate=0,iLastIndex=0;

	CQuest					*lpQM,*lpAM;
	cSecretDungeonManager	*lpSDM;
	cChapterManager			*lpCM;
	cBookManager			*lpBM;
	CKarmaContainer			*lpKC,*lpKCMonsterGen;

	if	(_lpKC)
		lpKC	=	_lpKC;
	else
		lpKC	=	&s_kcMonsterDeath;

	if	(_lpKCMonsterGen)
		lpKCMonsterGen	=	_lpKCMonsterGen;
	else
		lpKCMonsterGen	=	&s_kcMonsterGen;

	if	(_lpQuest)
		lpQM	=	_lpQuest;
	else
		lpQM	=	&g_quest;

	if	(_lpArbeit)
		lpAM	=	_lpArbeit;
	else
		lpAM	=	&g_questArbeit;

	if	(_lpSDM)
		lpSDM	=	_lpSDM;
	else
		lpSDM	=	&g_sdManager;

	if	(_lpCM)
		lpCM	=	_lpCM;
	else
		lpCM	=	&g_chapterManager;

	if	(_lpBM)
		lpBM	=	_lpBM;
	else
		lpBM	=	&g_book;

	if (_lpstrDestFolder)
	{
		if(!SetCurrentDirectory(_lpstrDestFolder))
			return ERRMSG("can not find follow folder",_lpstrDestFolder);
	}
	else
	if (!SetScenarioFolder())	
		return ERRMSG("can not find follow folder","%s\\data\\Scenario",_logFolder);

	if (!file.Open(fn,"wb"))
	{
		SetLogFolder();

		return ERRMSG("Error in cPROJECT::SaveProject","'%s' file write Error!!",fn);
	}

	//	프로젝트 폴더 안으로 들어간다.
	//	프로젝트 화일을 제외한 데이터는 전부 프로젝트 폴더 안으로 들어가야 한다.
	//	나중에... 하나로 합치더라도.. -_-a
	{
		if(!SetProjectFolder())	
			return	ERRMSG(dMSG_ERROR_LOAD_PROJECT,dMSG_CAN_NOT_FIND_PROJECT_FOLDER);
	}	//	프로젝트 폴더 안으로 들어간다.

//	프로젝트 기본 데이터 저장
	{
		file.Write(&size,4);									//	화일 사이즈
		file.Write(g_strProjectHeader[dCURRENT_PROJECT_VERSION],60);	//	헤더
		file.Write(s_strName,sizeof(s_strName));							//	프로젝트 이름
	}	//	프로젝트 기본 데이터 저장

	{
		int		iLocalizeNation	= eNationJapan;
		
		if	(_bIsLocalize)
			iLocalizeNation	=	g_iLocalizeNation;
		
		file.Write(&iLocalizeNation,sizeof(iLocalizeNation));
	}

	cMessSign::SetDataEncodeTable(eNationJapan);

	if	(_bIsLocalize)
		cMessSign::SetDataEncodeTable(g_iLocalizeNation);

	{
		CQuest	*lpQuest=lpQM,*lpArbeit=lpAM;

		for (int iQuestKind=0;iQuestKind<2;iQuestKind++)
		{
			CQuest	*lpActiveQuest;

			if (iQuestKind	==	0)
				lpActiveQuest	=	lpQuest;
			else
				lpActiveQuest	=	lpArbeit;

			for (int i=0;i<dQUEST_COUNT;i++)
			{
				if (lpActiveQuest->m_aQuest[i].m_wSerial	!=	0xffff)
				{
					RemoveQuotationMark(lpActiveQuest->m_aQuest[i].m_strName);
					RemoveQuotationMark(lpActiveQuest->m_aQuest[i].m_strQuestClientInfo);
					RemoveQuotationMark(lpActiveQuest->m_aQuest[i].m_strQuestValueComment);

					for (int j=0;j<dQUEST_STATUS_COUNT;j++)
						RemoveQuotationMark(lpActiveQuest->m_aQuest[i].m_astrExplain[j]);
				}
			}
		}

		int	iQuestCount	=	lpQuest->m_iCount;
		int	iArbeitCount=	lpArbeit->m_iCount;
		
		cMessSign::EncodeDataBuffer2(&iQuestCount,4);
		cMessSign::EncodeDataBuffer2(&iArbeitCount,4);

		file.Write(&iQuestCount,4);					//	저장할 퀘스트 정보의 수

		if	(lpQuest->m_iCount	>	0)
		{
			CQuestItem	quest;

			for (int i=0;i<lpQuest->m_iCount;i++)
			{
				memcpy(&quest,&lpQuest->m_aQuest[i],sizeof(CQuestItem));

				cMessSign::EncodeDataBuffer2(&quest,sizeof(CQuestItem));
				file.Write(&quest,sizeof(CQuestItem));
			}
		}

		file.Write(&iArbeitCount,4);					//	저장할 아르바이트 정보의 수

		if (lpArbeit->m_iCount	>	0)
		{
			CQuestItem	quest;

			for (int i=0;i<lpArbeit->m_iCount;i++)
			{
				memcpy(&quest,&lpArbeit->m_aQuest[i],sizeof(CQuestItem));

				cMessSign::EncodeDataBuffer2(&quest,sizeof(CQuestItem));
				file.Write(&quest,sizeof(CQuestItem));
			}
		}
	}	//	퀘스트 저장

	cSecretDungeonManager	*lpSD	=	lpSDM;

	{
		size	=	0x12345678;

		file.Write(&size,4);

		int				iCount	=	lpSD->getLastIndex()+1;
		cSecretDungeon	*lpBay	=	lpSD->getBay();

		int				iSaveCount=	iCount;
		cMessSign::EncodeDataBuffer2(&iSaveCount,4);

		file.Write(&iSaveCount,4);

		for (int i=0;i<iCount;i++)
		{
			cSecretDungeon	saveData;

			memcpy(&saveData,lpBay+i,sizeof(cSecretDungeon));

			cMessSign::EncodeDataBuffer2(&saveData,sizeof(cSecretDungeon));
			file.Write(&saveData,sizeof(cSecretDungeon));
		}
	}

	{
		size	=	0x12345678;

		file.Write(&size,4);
		file.Write(lpSD->m_aValueString,sizeof(lpSD->m_aValueString));
	}

	{
		size	=	0x12345676;

		file.Write(&size,4);
		file.Write(g_strMainQuestValue,sizeof(g_strMainQuestValue));
		file.Write(g_strMainQuestSwitch,sizeof(g_strMainQuestSwitch));
		file.Write(lpCM,sizeof(g_chapterManager));

		SaveBook(&file,lpBM);
	}

	cMAP::SaveKarmaContainer(&file,lpKC);
	cMAP::SaveKarmaContainer(&file,lpKCMonsterGen);

//	맵 정보 저장
	{
		for (i=0;i<dMAX_MAP;i++)
		{
			char	fn[128];

			strcpy(fn,"");

			if (strcmp(g_aMap[i].m_strFName,"")!=0)
				strcpy(fn,g_aMap[i].m_strFName);

			file.Write(fn,sizeof(fn));
			file.Write(&g_aMap[i].m_pos,sizeof(cPOINT));
		}

		file.Write(&g_iSelectMap,4);	//	선택된 맵 저장
		
	}	//	맵 정보 저장


	file.Close();		//	프로젝트 화일을 닫는다.
	TextFile.Close();	//	텍스트 화일을 닫는다.

////////////////////////////////////////////////////////////////////////////////////////////////
//	프로젝트 화일 외의 화일들 저장
////////////////////////////////////////////////////////////////////////////////////////////////
	SaveItem();
	SaveJob();
//	ReadCorrectJobDataAndSave();

///////////////////////////////////////////////////////////////////////////////////
//	각종 마무리
///////////////////////////////////////////////////////////////////////////////////
	SetLogFolder();

	return	TRUE;
}

//
//	프로젝트 데이터를 불러온다.
BOOL
cPROJECT::LoadProject(char *fn,CQuest *_lpQuest,CQuest *_lpArbeit,cSecretDungeonManager *_lpSDM,cChapterManager *_lpCM,cBookManager *_lpBM,CKarmaContainer	*_lpKC,CKarmaContainer *_lpKCMopnsterGen,BOOL _bIsForLocalize)
{
	cFILE	file,textFile;
	DWORD	size;
	int		i=0,j=0,iCount=0,iBufferSize=1024,iVersion=0;
	char	strHeader[70];

	CQuest					*lpQM,*lpAM;
	cSecretDungeonManager	*lpSDM;
	cChapterManager			*lpCM;
	cBookManager			*lpBM;
	CKarmaContainer			*lpKC,*lpKCMopnsterGen;

	if	(_lpKC)
		lpKC	=	_lpKC;
	else
		lpKC	=	&s_kcMonsterDeath;

	if	(_lpKCMopnsterGen)
		lpKCMopnsterGen	=	_lpKCMopnsterGen;
	else
		lpKCMopnsterGen	=	&s_kcMonsterGen;

	if	(_lpQuest)
		lpQM	=	_lpQuest;
	else
		lpQM	=	&g_quest;

	if	(_lpArbeit)
		lpAM	=	_lpArbeit;
	else
		lpAM	=	&g_questArbeit;

	if	(_lpSDM)
		lpSDM	=	_lpSDM;
	else
		lpSDM	=	&g_sdManager;

	if	(_lpCM)
		lpCM	=	_lpCM;
	else
		lpCM	=	&g_chapterManager;

	if	(_lpBM)
		lpBM	=	_lpBM;
	else
		lpBM	=	&g_book;

	if	(!SetScenarioFolder())
		return	FALSE;	//	프로젝트 리스트 폴더

	if	(!file.Open(fn,"rb"))
	{
		SetLogFolder();

		return cMSG::Put("Error in cPROJECT::LoadProject","can not open file '%s'",fn);
	}

	int		iLocalizeNation	= eNationJapan;

//	프로젝트 화일 기본 데이터 로딩
	{
		file.Read(&size		,4	);		//	화일 사이즈
		file.Read(strHeader	,60	);		//	헤더

		for (i=0;;i++)
		{
			if (stricmp(g_strProjectHeader[i],"end"		)==0)
				return	ERRMSG("프로젝트 로딩 에러!!","붉은보석의 프로젝트 데이터가 아닙니다.\n\n%s",fn);

			if (stricmp(g_strProjectHeader[i],strHeader	)==0)
			{
				iVersion	=	i;
				break;
			}
		}

		file.Read(s_strName,sizeof(s_strName));	//	프로젝트 이름

		if	(iVersion	>=	ePVUI_14_ENCRYPT2)
			file.Read(&iLocalizeNation,sizeof(iLocalizeNation));

	}	//	프로젝트 화일 기본 데이터 로딩

//	프로젝트 폴더로 들어가기
	{
		if	(!SetProjectFolder())
			return	cMSG::Put(dMSG_ERROR_LOAD_PROJECT,dMSG_CAN_NOT_FIND_PROJECT_FOLDER);
	}

//	각종 데이터 화일 불러오기
	LoadItem();
	LoadJob();

	SetProjectFolder();

#pragma	pack(2)

class	CQuestItem_old
{
public:
	WORD	m_wSerial;
	char	m_strName[dQUEST_NAME_SIZE];
	char	m_astrExplain[dQUEST_STATUS_COUNT][dQUEST_EXPLAIN_SIZE];

	WORD	m_wQuestType;	//	이벤트 퀘스트여?
	WORD	m_wBeginYear,m_wBeginMonth,m_wBeginDay;
	WORD	m_wExpireYear,m_wExpireMonth,m_wExpireDay;

	char	m_strQuestClientInfo[dQUEST_CLIENT_INFO_SIZE];
	WORD	m_wQuestKind;
	char	m_strBeginNPCName[dNAME_LENGTH];
	WORD	m_wBeginNPC_Field;

	WORD	m_wMinProprietyLevel,m_wMaxProprietyLevel;
	WORD	m_wMinLimitLevel,m_wMaxLimitLevel;
	char	m_strQuestValueComment[dQUEST_VALUE_COMMENT_SIZE];

	UINT	m_uiEnableJobMask;

	WORD	m_bf1IsMaleJobEquipAble			:	1;	//	남성 직업 가능
	WORD	m_bf1IsFemaleJobEquipAble		:	1;	//	여성 직업 가능
	WORD	m_isEnableCancel					: 1;
	WORD	m_isRemoveQuestItemByCancelQuest	: 1;
	WORD	m_isTestQuestInTestServer			: 1;
	WORD	m_dummy								: 13;
};
#pragma pack()

	if	(iVersion	>=	ePVUI_14_ENCRYPT2)
		cMessSign::SetDataEncodeTable(iLocalizeNation);
	else
		cMessSign::SetDataEncodeTable(-1);

	{
		CQuest	*lpQuest;

		for (int iQuestTyep=0;iQuestTyep<2;iQuestTyep++)
		{
			if (iQuestTyep==0)	
				lpQuest	=	lpQM;
			if (iQuestTyep==1)	
				lpQuest	=	lpAM;

			lpQuest->reset();

			file.Read(&lpQuest->m_iCount,4);					//	저장할 퀘스트 정보의 수

			if	(iVersion	>=	ePVUI_12_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&lpQuest->m_iCount,4);

			for (int i=0;i<lpQuest->m_iCount;i++)
			{
				CQuestItem	saveData;

				file.Read(&saveData,sizeof(CQuestItem));

				if	(iVersion	>=	ePVUI_12_ENCRYPT)
					cMessSign::DecodeDataBuffer2(&saveData,sizeof(CQuestItem));

				memcpy(lpQuest->m_aQuest+i,&saveData,sizeof(CQuestItem));
			}

			lpQuest->rebuild();
		}

		if	(lpQM->m_iCount	<	dEVENT_QUEST_COUNT)
		{
			lpQM->reset();
			lpQM->initEventQuest();
		}

		for (i=0;i<dEVENT_QUEST_COUNT;i++)	
			lpQM->m_aQuest[i].m_wQuestType	=	eQT_EVENT;

		for (int iQuestKind=0;iQuestKind<2;iQuestKind++)
		{
			CQuest	*lpQuest;

			if	(iQuestKind	==	0)
				lpQuest		=	lpQM;
			else
				lpQuest		=	lpAM;

			for (int i=0;i<dQUEST_COUNT;i++)
			{
				if (lpQuest->m_aQuest[i].m_wSerial	!=	0xffff)
				{
					RemoveQuotationMark(lpQuest->m_aQuest[i].m_strName);
					RemoveQuotationMark(lpQuest->m_aQuest[i].m_strQuestClientInfo);
					RemoveQuotationMark(lpQuest->m_aQuest[i].m_strQuestValueComment);

					for (int j=0;j<dQUEST_STATUS_COUNT;j++)
						RemoveQuotationMark(lpQuest->m_aQuest[i].m_astrExplain[j]);
				}
			}
		}
	}

	{
		file.Read(&size,4);

		if (size	==	0x12345678)
		{
			cSecretDungeon	*lpBay	=	lpSDM->getBay();
			int				iCount;

			file.Read(&iCount,4);

			if	(iVersion	>=	ePVUI_12_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&iCount,4);

			if	(iCount)
			{
				file.Read(lpBay,sizeof(cSecretDungeon)*iCount);

				if	(iVersion	>=	ePVUI_12_ENCRYPT)
					for(int i=0;i<iCount;i++)
						cMessSign::DecodeDataBuffer2(lpBay+i,sizeof(cSecretDungeon));
			}

			lpSDM->build();
		}
		else
			file.Seek(-4);
	}

	{
		file.Read(&size,4);

		if (size	==	0x12345678)
			file.Read(lpSDM->m_aValueString,sizeof(lpSDM->m_aValueString));
		else
			file.Seek(-4);
	}

	{
		file.Read(&size,4);

		if	(size	==	0x12345678 || size	==	0x12345677 || size	==	0x12345676)
		{
			int	iSize	=	sizeof(g_strMainQuestValue)+sizeof(g_strMainQuestSwitch)+sizeof(g_chapterManager);

			file.Read(g_strMainQuestValue,sizeof(g_strMainQuestValue));
			file.Read(g_strMainQuestSwitch,sizeof(g_strMainQuestSwitch));
			file.Read(lpCM,sizeof(g_chapterManager));

//  			cFILE	file2;
//  			
//  			file2.Open("d:\\mainQuestValue.dat","rb");
//  			file2.Read(g_strMainQuestValue,sizeof(g_strMainQuestValue));
//  			file2.Read(g_strMainQuestSwitch,sizeof(g_strMainQuestSwitch));
//  			file2.Close();
			
			if	(size	==	0x12345676)
				LoadBook(&file,FALSE,lpBM);
			else
				LoadBook(&file,TRUE,lpBM);
		}
		else
		{
			file.Seek(-4);

			lpCM->reset();
		}
	}

	s_kcMonsterDeath.reset();
	s_kcMonsterGen.reset();

	BOOL	bIsDecodeKarma	=	FALSE;

	if	(iVersion >= ePVUI_13_ENCRYPT_MORE)
		bIsDecodeKarma	=	TRUE;

	if	(iVersion >= ePVUI_09_ADD_PORJECT_KARMA)
		cMAP::ReadKarmaContainer(&file,lpKC,bIsDecodeKarma);

	if	(iVersion >= ePVUI_10_ADD_PORJECT_KARMA_MONSTER_GEN)
		cMAP::ReadKarmaContainer(&file,lpKCMopnsterGen,bIsDecodeKarma);

//	맵들 불러오기
	if	(_bIsForLocalize	==	FALSE)
	{
		int	iCount=0,iSelectMap=0;

		for (i=0;i<dMAX_MAP;i++)
		{
			char	fn[128];
			file.Read(fn,sizeof(fn));

			if (strcmp(fn,"")	!=	0)
			{	
				cPOINT	pos;
				file.Read(&pos,sizeof(cPOINT));

				if	(LoadMap(fn,iCount))
				{	
					g_aMap[iCount].setPos(pos.x,pos.y);
					iCount++;
				}
			}
			else
				file.Seek(sizeof(cPOINT),SEEK_CUR);
		}

		file.Read(&iSelectMap,4);	//	선택된 맵 저장
		cMAP::Select(iSelectMap);
	}

//	프로젝트 화일 로딩 완료
	file.Close();

	SetLogFolder();

	if	(iVersion	<	dCURRENT_PROJECT_VERSION)
	{
		SaveProject(fn);

		cMSG::Put("Project data conversion","[%s]\n\nThe file is an old version, so it was updated to a new version and saved.",fn);
	}

	return	TRUE;
}


LRESULT CALLBACK
cPROJECT::s_procNewProject(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			break;

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDC_LOAD			:
				{
					break;
				}

				case	IDOK				:
				{
					if (GetProject(hwnd))	EndDialog(hwnd,IDOK);

					break;
				}

			}
			break;
	}

	return (0L);
}

//
//	맵 저장
BOOL
cPROJECT::SaveMap(HINSTANCE hinst,HWND hwnd)
{
	strcpy(s_strFileName,"");

	if	(strcmp(s_strMapFolder,"")==0)			strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));
	if	(!SetCurrentDirectory(s_strMapFolder))	strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));

	if (!cFILE::SaveBox(s_strFileName,"red stone map file {*.rmd}\0*.rmd\0\0","rmd",s_strMapFolder,dMSG_SAVE_MAP))
	{
		SetLogFolder();

		return	FALSE;
	}

	GetCurrentDirectory(sizeof(s_strMapFolder),s_strMapFolder);

	strcpy(s_strFileName,ExportMapFileName(s_strFileName));

	BOOL	isResult	=	SaveMap(s_strFileName,g_iSelectMap);

	SetLogFolder();

	return	isResult;
}	//	cPROJECT::SaveMap(HINSTANCE hinst,HWND hwnd)


//
//	_lpstrFoder 이거... 프로젝트에 속한 맵 폴더 맞냐-_-?
BOOL
cPROJECT::IsMapFolder(char *_lpstrFoder)
{
	char	strMapFolder[512];
	char	strProjectMapFolder[512];

	GetMapFolder(strProjectMapFolder);	//	프로젝트 맵 폴더를 얻어온다.
	strcpy(strMapFolder,_lpstrFoder);

	strMapFolder[strlen(strProjectMapFolder)]	=	0;

	if (stricmp(strMapFolder,strProjectMapFolder)	!=	0)	return	FALSE;

	return	TRUE;
}	//	cPROJECT::IsMapFolder(char *_lpstrFoder)

//
//	현재 선택한 맵에 맵을 로딩
char *
cPROJECT::GetMapName(HINSTANCE hinst,HWND hwnd)
{
	static	char	strMapName[512];

	strcpy(strMapName,"");

	if	(strcmp(s_strMapFolder,"")==0)			strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));
	if	(!SetCurrentDirectory(s_strMapFolder))	strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));
	if	(!IsMapFolder(s_strMapFolder))			strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));

	if (!cFILE::LoadBoxTemplate(strMapName,"red stone map file {*.rmd}\0*.rmd\0\0","rmd","MAP_INFO",
								hookOpenMap,hinst,s_strMapFolder,dMSG_LOAD_MAP , OFN_ALLOWMULTISELECT))
	{
		SetLogFolder();
		return	NULL;
	}

	GetCurrentDirectory(sizeof(s_strMapFolder),s_strMapFolder);

	strcpy(strMapName,ExportMapFileName(strMapName));
	
	SetLogFolder();

	return	strMapName;
}	//	cPROJECT::LoadMap(HINSTANCE hinst,HWND hwnd)


//
//	현재 선택한 맵에 맵을 로딩
BOOL
cPROJECT::LoadMap(HINSTANCE hinst,HWND hwnd)
{
	char	*lpstrMapName	=	GetMapName(hinst,hwnd);
	
	if (!lpstrMapName)	return	FALSE;

	strcpy(s_strFileName,lpstrMapName);

	BOOL	isResult	=	LoadMap(s_strFileName);
	// 10.01.12
	cMemoWindow::SetMemo(&g_lpActmap->m_sMemoInfo);
	// 10.01.15
	cSET_AREA_EDIT::InitAreaDataBox();
	cSET_AREA_EDIT::ResetAreData();

	return	isResult;
}	//	cPROJECT::LoadMap(HINSTANCE hinst,HWND hwnd)

//
//	현재 선택한 맵에 맵을 로딩
char*
cPROJECT::SelectMap(HINSTANCE hinst,HWND hwnd)
{
	static	char	s_strMapFile[256];
	strcpy(s_strMapFile,"");

	if	(strcmp(s_strMapFolder,"")==0)			strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));
	if	(!SetCurrentDirectory(s_strMapFolder))	strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));
	if (!IsMapFolder(s_strMapFolder))			strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));

	if (!cFILE::LoadBoxTemplate(s_strMapFile,"red stone map file {*.rmd}\0*.rmd\0\0","rmd","MAP_INFO",
								hookOpenMap,hinst,s_strMapFolder,dMSG_SELECT_MAP, OFN_ALLOWMULTISELECT))
	{
		SetLogFolder();
		return	NULL;
	}

	GetCurrentDirectory(sizeof(s_strMapFolder),s_strMapFolder);

	strcpy(s_strMapFile,ExportMapFileName(s_strMapFile));
	
	SetLogFolder();

	return	s_strMapFile;
}	//	cPROJECT::LoadMap(HINSTANCE hinst,HWND hwnd)

//
//	맵을 추가
BOOL
cPROJECT::AddMap(HINSTANCE hinst,HWND hwnd)
{
	int availMap	=	cMAP::GetValidMap();

	if (availMap	==	0xffff)	return	ERRMSG(dMSG_MAP_LOAD_ERROR,dMSG_TOO_MANY_MAP);

	strcpy(s_strFileName,"");

	if	(strcmp(s_strMapFolder,"")==0)			strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));
	if	(!SetCurrentDirectory(s_strMapFolder))	strcpy(s_strMapFolder,_ms("%s\\data\\Scenario\\%s\\map",_logFolder,s_strName));

	if (!cFILE::LoadBoxTemplate(s_strFileName,"red stone map file {*.rmd}\0*.rmd\0\0","rmd","MAP_INFO",
								hookOpenMap,hinst,s_strMapFolder,dMSG_ADD_MAP, OFN_ALLOWMULTISELECT))
	{
		SetLogFolder();
		return	FALSE;
	}

	GetCurrentDirectory(sizeof(s_strMapFolder),s_strMapFolder);

	BOOL	isResult	=	AddMap(s_strFileName);

	SetLogFolder();
	// 10.01.12
	g_lpActmap->LoadMemoFile();
	cMemoWindow::SetMemo(&g_lpActmap->m_sMemoInfo);

	return	isResult;
}	//	cPROJECT::AddMap(HINSTANCE hinst,HWND hwnd)

//
//	프로젝트 로딩
BOOL
cPROJECT::LoadProject(HINSTANCE hinst,HWND hwnd)
{
	strcpy(s_strFileName,"");

	if (!cFILE::LoadBox(s_strFileName,"red stone project filefile {*.rpd}\0*.rpd\0\0","rpd",_ms("%s\\data\\Scenario",_logFolder),dMSG_ADD_MAP))
	{
		SetLogFolder();
		return	FALSE;
	}

	BOOL	isResult	=	LoadProject(s_strFileName);

	SetLogFolder();

	return	isResult;
}

//
//	프로젝트 저장
BOOL
cPROJECT::SaveProject(HINSTANCE hinst,HWND hwnd)
{
	strcpy(s_strFileName,"");

	if (!cFILE::SaveBox(s_strFileName,"red stone project file {*.rpd}\0*.rpd\0\0","rpd",
						_ms("%s\\data\\Scenario",_logFolder),dMSG_SAVE_PROJECT)
		)
	{
		SetLogFolder();
		return	FALSE;
	}

	BOOL	isResult	=	SaveProject(s_strFileName);

	SetLogFolder();

	return	isResult;
}	//	cPROJECT::SaveProject(HINSTANCE hinst,HWND hwnd)


//
//	맵 저장.
BOOL
cPROJECT::SaveMap(char *fn,int index,char *_lpstrDestFileName,BOOL _bIsLocalize)
{
	if	(!SetMapFolder())
		return	ERRMSG("can not find follow folder","%s\\data\\Scenario\\%s\\map",_logFolder,s_strName);

	char	FName[256];

	if	(fn)
		strcpy(FName,fn);

	if	(index	==	0xffff)
	{	
		index	=	g_iSelectMap;
		strcpy(FName,g_lpActmap->m_strFName);
	}

	if	(strcmp(FName,"")==0)
	{	
		if	(!SaveMap(MAIN.hINST,MAIN.hWND))
			return FALSE;

		return	TRUE;
	}

	BOOL	result	=	cMAP::Save(FName,index,_lpstrDestFileName,FALSE,NULL,_bIsLocalize);

	if	(index	==	g_iSelectMap)
		MAIN.Draw();

	SetLogFolder();

	return	result;
}

//
//	맵 불러오기
BOOL
cPROJECT::LoadMap(char *fn,int index)
{
	if	(IsOpenMap(ExportMapFileName(fn),index))
		return	cMSG::Put(dMSG_ERROR_LOAD_MAP,dMSG_ALREADY_OPENED);

	if	(!SetMapFolder())
		return FALSE;

	BOOL	result	=	cMAP::Load(fn,index);

	SetLogFolder();

	g_MAPWND.Draw();

	return	result;
}

//
//	맵 추가하기
BOOL
cPROJECT::AddMap(char *fn)
{
	int availMap	=	cMAP::GetValidMap();

	if	(IsOpenMap(ExportMapFileName(fn)))
		return	cMSG::Put(dMSG_ERROR_LOAD_MAP,dMSG_ALREADY_OPENED);

	if	(!SetMapFolder())
		return FALSE;

	BOOL	result	=	cMAP::Load(fn,availMap);

	SetLogFolder();

	g_MAPWND.Draw();
	TOOLWND.ChangeMode(g_iWorkMode);
	TOOLWND.Draw();

	return	result;
}


char*
cPROJECT::GetFile(char *str)
{
	int	i;

	SetProjectFolder();

	cSTRINGS	*lpFileList	=	cFOLDER::GetFiles("*.rmd");//	화일들을 얻어 온다.

	static	char	fn[64];

	if (lpFileList)
	{
		cSTRING	*lpFile	=	lpFileList->Pick();

		for (i=0;i<lpFileList->Count;i++)
		{
			cFILE		file;
			cMAP_INFO	MapInfo;

			if (file.Open(lpFile->String,"rb"))
			{
				file.Seek(68,SEEK_SET);
				file.Read(&MapInfo,8+dMAP_NAME_LENGTH);

				if (stricmp(str,MapInfo.m_strName)	==	0)
				{
					strcpy(fn,lpFile->String);

					SetLogFolder();

					return	fn;
				}

				file.Close();
			}

			lpFile	=	lpFile->pNEXT;
		}
	}

	SetLogFolder();

	return	NULL;
}


UINT APIENTRY
cPROJECT::hookOpenMap(HWND hdlg,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{	
		case	WM_NOTIFY		:
		{
			LPOFNOTIFY	lpInfo	=	(LPOFNOTIFY)lParam;
			switch( lpInfo->hdr.code)
			{
				case	CDN_SELCHANGE	:
				{
					HWND		hwnd	=	((NMHDR*)lParam)->hwndFrom;
					char		fn[256];
					GetDlgItemText(hwnd,dGET_FILE_NAME,fn,sizeof(fn));

					if	(strcmp(fn,"")==0)
						strcpy(fn," ");

					cMAP_INFO	mapInfo;

					HWND	hFrame			=	GetDlgItem(hdlg,IDC_FRAME);
					HWND	hStaticName		=	GetDlgItem(hdlg,IDC_STATIC_NAME);
					HWND	hStaticType		=	GetDlgItem(hdlg,IDC_STATIC_TYPE);
					HWND	hStaticSize		=	GetDlgItem(hdlg,IDC_STATIC_SIZE);
					HWND	hStaticTileSet	=	GetDlgItem(hdlg,IDC_STATIC_TILESET);

					HWND	hName			=	GetDlgItem(hdlg,IDC_NAME);
					HWND	hSize			=	GetDlgItem(hdlg,IDC_MAP_SIZE);
					HWND	hType			=	GetDlgItem(hdlg,IDC_TYPE);
					HWND	hTileSet		=	GetDlgItem(hdlg,IDC_TILESET);

					if	(strstr(fn,"rmd")	==	NULL)
						strcat(fn,".rmd");

					if (cMAP::GetMapInfo(fn,&mapInfo))
					{
						EnableWindow(hFrame,TRUE);
						EnableWindow(hStaticName,TRUE);
						EnableWindow(hStaticType,TRUE);
						EnableWindow(hStaticSize,TRUE);
						EnableWindow(hStaticTileSet,TRUE);

						SetWindowText(hName,mapInfo.m_strName);
						SetWindowText(hSize,_ms("%d x %d",mapInfo.m_iWidth,mapInfo.m_iHeight));
						SetWindowText(hTileSet,g_strTileSet[mapInfo.m_iTileSet*2+1]);
					}
					else
					{
						EnableWindow(hFrame,FALSE);
						EnableWindow(hStaticName,FALSE);
						EnableWindow(hStaticType,FALSE);
						EnableWindow(hStaticSize,FALSE);
						EnableWindow(hStaticTileSet,FALSE);

						SetWindowText(hName,"");
						SetWindowText(hSize,"");
						SetWindowText(hType,"");
						SetWindowText(hTileSet,"");
					}

					break;
				}
			}
			break;
		}

		default:
			return FALSE;

	}

	return FALSE;
}




