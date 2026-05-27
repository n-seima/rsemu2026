#include "cMAIN.H"
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "cTEXT.H"
#include "cDRAW.H"
#include "cMAPWND.H"
#include "cTOOLWND.H"
#include "cOBJECT.H"
#include "cBUILDING.H"
#include "cAREA.H"
#include "cSET.H"
#include "cMESSAGE.H"
#include "cKARMA.H"
#include "cPROJECT.H"
#include "cACTOR.H"
#include "cSKILL.H"
#include "CSetShop.H"
#include "CSetDialogList.H"
#include "CSetCustomItem.H"
#include "CTitle.H"
#include "localize.H"
#include "CLoadTextCode.h"
#include "CArea.h"
#include "secretDungeon.h"

BOOL					g_bIsJapan	=	FALSE;
CQuestText				l_srcQuest,l_destQuest;
C1KindText				l_src1KindText,l_dest1KindText;
C2KindText				l_srcItem,l_destItem;
CSkillText				l_srcSkill,l_destSkill;
cItemSetText			l_srcItemSet,l_destItemSet;
cMainQuestText			l_srcMainQuestText,l_destMainQuestText;
cBookTextPack			l_srcBookText,l_destBookText;
CProjectKarmaText		l_srcProjectKarma,l_destProjectKarma;

CQuest					l_localWorkQuest,l_localWorkarbeit;
cBASIC_ITEM				l_aBasicItem[dBASIC_ITEM_COUNT];
cSKILL					l_aSkill[dMAX_SKILL];
CMapSimpleInfo			l_aMapSimpleInfo[dMAX_MAP_COUNT];
cJOB_DEN				jobDen;
cSecretDungeonManager	l_sdManager;
cChapterManager			l_chapterManager;
cBookManager			l_bookManager;

CCustomItemManager		l_premiumItem,l_customItem;
cITEM_PREFIX_MANAGER	l_itemPrefixManager;
cArcaManager			l_arcaManager;
cItemSetManager			l_itemSetManager;
cPieceItemManager		l_pieceItemManager;
CKarmaContainer			l_kcMonsterDeath;
CKarmaContainer			l_kcMonsterGen;

int						g_iSeleceLocalizeNation	=	0;
int						g_iLocalizeNation		=	0;

int	l_aConvertMapList[dMAX_MAP_COUNT]	=	
{
-1,
};

BOOL
cMAIN::selectLocalizeFolder(HWND _hWnd)
{
	if	(m_strLocalizeFolder[0])
	{
		SetLogFolder();
		return	TRUE;
	}

	if	(DialogBox( hINST,"SELECT_TARGET_LANGUAGE",_hWnd,(DLGPROC)s_procSelectTargetLanguage)	==	IDCANCEL)
		return	FALSE;

	SetLogFolder();

	return	TRUE;
}

class	cLocalizeTemp
{
public:
	cLocalizeTemp()
	{
		g_bIsLocalizing	=	TRUE;
	}

	~cLocalizeTemp()
	{
		g_bIsLocalizing	=	FALSE;
	}
};


BOOL
cMAIN::localizing(HWND hwnd,BOOL _bIsWantPart)
{
	cLocalizeTemp	localize;

	if	(!selectLocalizeFolder(hwnd))
		return	FALSE;

	int			i;
	char		strFileName[512],strPath[512];
	char		strDataFileName[512];
	cDECODER	decoder;

	SetLogFolder();

	if	(!IsFolder(_ms("localize/%s/import",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/import",_logFolder);

	if	(_bIsWantPart)
	{
		if	(!IsFolder(_ms("localize/%s/import/_update",m_strLocalizeFolder)))
			return	ERRMSG("can not find follow folder","localize/%s/import/_update",m_strLocalizeFolder);
		if	(!IsFolder(_ms("localize/%s/import/_update/expansion",m_strLocalizeFolder)))
			return	ERRMSG("can not find follow folder","localize/%s/import/_update/expansion",m_strLocalizeFolder);
	}

	sprintf(strPath,"localize/%s/result data/scenario/%s/map",m_strLocalizeFolder,cPROJECT::s_strName);

	if	(!IsFolder(strPath))
		return	ERRMSG("can not find follow folder",strPath);

	sprintf(strPath,"localize/%s/result data/scenario/%s/map/expansion",m_strLocalizeFolder,cPROJECT::s_strName);

	if	(!IsFolder(strPath))
		return	ERRMSG("can not find follow folder",strPath);

	char	strSrcMapFolder[1024];

	sprintf(strSrcMapFolder,"localize/%s",m_strLocalizeFolder);

	if	(cMSG::YESNO("localize all maps text data","do you want continue this work?")	==	IDYES)
	{
		for (i=0;i<dMAX_MAP_COUNT;i++)
		{
			strcpy(l_aMapSimpleInfo[i].m_strName,"valid");
			l_aMapSimpleInfo[i].m_wSerial	=	0xffff;
		}

		cSTRINGS	*lpFiles;
		int			iWorkMapCount	=	0;

		{
			cPROJECT::SetMapFolder(strSrcMapFolder);

			lpFiles	=	cFOLDER::GetFiles("*.rmd");

			SetLogFolder();

			if (!lpFiles)
				return	ERRMSG("error!!","can not find map list!");

			for (i=0;i<lpFiles->Count;i++)
			{
				cSTRING	*lpFile	=	lpFiles->Pick(i);

				char	strIndex[10];

				strncpy(strIndex,lpFile->String+1,3);
				strIndex[3]	=	NULL;

				int	iIndex	=	atoi(strIndex);

				if	(_bIsWantPart)
				{
					char	strFileName[512],strRmdFileName[512];

					sprintf(strFileName,"%s/localize/%s/import/_update/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_exportFileName(lpFile->String));
					sprintf(strRmdFileName,"%s/localize/%s/import/_update/%s",_logFolder,MAIN.m_strLocalizeFolder,_exportFileName(lpFile->String));

					if	(IsFile(strFileName)	==	FALSE	&&	IsFile(strRmdFileName)	==	FALSE	)
						continue;
				}

				if	(!IsConvertMap(iIndex))
					continue;

				cPROJECT::SetMapFolder(strSrcMapFolder);

				cMAP::Load(lpFile->String,0);

				if	(g_aMap[0].importText(FALSE,_bIsWantPart))
				{
					sprintf(strFileName,"%s/localize/%s/result data/scenario/%s/map/%s",_logFolder,m_strLocalizeFolder,cPROJECT::s_strName,lpFile->String);

					cPROJECT::SaveMap(g_aMap[0].m_strFName,0,strFileName,TRUE);
				}

				l_aMapSimpleInfo[iIndex].m_wWidth			=	g_aMap[0].m_iWidth;
				l_aMapSimpleInfo[iIndex].m_wHeight			=	g_aMap[0].m_iHeight;
				strcpy(l_aMapSimpleInfo[iIndex].m_strName,g_aMap[0].m_strName);
				strcpy(l_aMapSimpleInfo[iIndex].m_strFileName,lpFile->String);

				l_aMapSimpleInfo[iIndex].m_wSerial			=	iIndex;
				l_aMapSimpleInfo[iIndex].m_bf8Type			=	g_aMap[0].m_bf4FieldType;
				l_aMapSimpleInfo[iIndex].m_bf1IsPremiumZone	=	g_aMap[0].m_bf1IsPremiumZone;

				SetLogFolder();
				Draw();
				Sleep(1);

				iWorkMapCount++;
			}

			SetLogFolder();

			if	(_bIsWantPart	==	FALSE)
			{
				cFILE	file;

				file.Open(_ms("localize/%s/result data/mapList.dat",m_strLocalizeFolder),"wb");
				file.Write(l_aMapSimpleInfo,sizeof(l_aMapSimpleInfo));
				file.Close();
			}

			KILL(lpFiles);
		}

		{
			cPROJECT::SetMapFolder(strSrcMapFolder);

			SetCurrentDirectory("expansion");

			lpFiles	=	cFOLDER::GetFiles("*.rmd");

			SetLogFolder();

			if	(!lpFiles)
				return	ERRMSG("error!!","can not find map list!");

			for (i=0;i<lpFiles->Count;i++)
			{
				cSTRING	*lpFile	=	lpFiles->Pick(i);

				cPROJECT::SetMapFolder(strSrcMapFolder);
				SetCurrentDirectory("expansion");

				if	(_bIsWantPart)
				{
					char	strFileName[512],strRmdFileName[512];

					sprintf(strFileName,"%s/localize/%s/import/_update/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_exportFileName(lpFile->String));
					sprintf(strRmdFileName,"%s/localize/%s/import/_update/expansion/%s",_logFolder,MAIN.m_strLocalizeFolder,_exportFileName(lpFile->String));

					if	(IsFile(strFileName)	==	FALSE	&&	IsFile(strRmdFileName)	==	FALSE	)
						continue;
				}

				cMAP::Load(lpFile->String,0);

				if	(g_aMap[0].importText(TRUE,_bIsWantPart))
				{
					sprintf(strFileName,"%s/localize/%s/result data/scenario/%s/map/expansion/%s",_logFolder,m_strLocalizeFolder,cPROJECT::s_strName,lpFile->String);

					cPROJECT::SaveMap(g_aMap[0].m_strFName,0,strFileName,TRUE);
				}

				SetLogFolder();
				Draw();
				Sleep(1);

				iWorkMapCount++;
			}

			SetLogFolder();

			KILL(lpFiles);
		}

		cMSG::Put("map work","complete [%d] numbers map data",iWorkMapCount);
	}

	SetLogFolder();

	sprintf(strFileName,"%s/localize/%s/data/scenario/red stone.rpd",_logFolder,m_strLocalizeFolder);

	cPROJECT::LoadProject(strFileName,&l_localWorkQuest,&l_localWorkarbeit,&l_sdManager,&l_chapterManager,&l_bookManager,&l_kcMonsterDeath,&l_kcMonsterGen,TRUE);

	while(1)
	{
		CKarmaContainer	*lpkc;

//	몬스터 죽음 카르마
		sprintf(strFileName,"localize/%s/import/project.txt",m_strLocalizeFolder);

		if	(!decoder.Upload(strFileName,NULL))
		{
			ERRMSG("can not find follow file",strFileName);

			break;
		}

		while(1)
		{
			char	*lpstrKeyWord	=	decoder.GetWord();
			char	*lpstrStream;

			if	(stricmp(lpstrKeyWord,"md_karma_reaction")==0)
			{
				decoder.GetNumber();

				int	iKarma			=	decoder.GetNumber();
				int	iReaction		=	decoder.GetNumber();

				lpstrStream			=	decoder.GetStream();

				lpkc				=	&l_kcMonsterDeath;

				if	(iKarma			>=	lpkc->m_wKarmaCount)
					continue;

				if	(iReaction		>=	lpkc->m_pKarma[iKarma].m_wReactionCount)
					continue;

				CKarmaItem	*lpKI	=	&lpkc->m_pKarma[iKarma].m_pReaction[iReaction];

				lpKI->SetString(lpstrStream);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"mg_karma_reaction")==0)
			{
				decoder.GetNumber();

				int	iKarma			=	decoder.GetNumber();
				int	iReaction		=	decoder.GetNumber();

				lpstrStream			=	decoder.GetStream();

				lpkc				=	&l_kcMonsterGen;

				if	(iKarma			>=	lpkc->m_wKarmaCount)
					continue;

				if	(iReaction		>=	lpkc->m_pKarma[iKarma].m_wReactionCount)
					continue;

				CKarmaItem	*lpKI	=	&lpkc->m_pKarma[iKarma].m_pReaction[iReaction];

				lpKI->SetString(lpstrStream);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"isEndCode")==0)
				break;

			ERRMSG("error in cMAIN::localizing","can not find follow keyword in project.txt",lpstrKeyWord);

			break;

		}

		break;
	}

	while(1)
	{
//	메인 퀘스트
		sprintf(strFileName,"localize/%s/import/mainQuest.txt",m_strLocalizeFolder);

		if	(!decoder.Upload(strFileName,NULL))
		{
			ERRMSG("can not find follow file",strFileName);

			break;
		}

		while(1)
		{
			char			*lpstrKeyWord	=	decoder.GetWord();
			cChapterInfo	*lpChapter;

			if	(stricmp(lpstrKeyWord,"mainQuestName")==0)
			{
				int		iIndex		=	decoder.GetNumber();
				int		iCheckSum	=	decoder.GetNumber();
				char	*lpstrName	=	decoder.GetStream();

				lpChapter			=	l_chapterManager.get(iIndex);

				if	(!lpChapter)
					continue;

				ustrncpy(lpChapter->m_strName,lpstrName,c_iChapterNameLength-2,g_bIsJapan);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"mainQuestComment")==0)
			{
				int		iIndex		=	decoder.GetNumber();
				int		iCheckSum	=	decoder.GetNumber();
				char	*lpstrComment=	decoder.GetStream();

				lpChapter			=	l_chapterManager.get(iIndex);

				if	(!lpChapter)
					continue;

				ustrncpy(lpChapter->m_strComment,lpstrComment,c_iChapterCommentLength-2,g_bIsJapan);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"mainQuestEpilogComment")==0)
			{
				int		iIndex		=	decoder.GetNumber();
				int		iCheckSum	=	decoder.GetNumber();
				char	*lpstrComment=	decoder.GetStream();

				lpChapter			=	l_chapterManager.get(iIndex);

				if	(!lpChapter)
					continue;

				ustrncpy(lpChapter->m_strEpilogComment,lpstrComment,c_iChapterEpilogCommentLength-2,g_bIsJapan);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"highQuestName")==0)
			{
				int		iIndex		=	decoder.GetNumber();
				int		iHighQuest	=	decoder.GetNumber();
				int		iCheckSum	=	decoder.GetNumber();
				char	*lpstrName	=	decoder.GetStream();

				lpChapter			=	l_chapterManager.get(iIndex);

				if	(!lpChapter)
					continue;

				ustrncpy(lpChapter->m_strHighQuest[iHighQuest],lpstrName,c_iHighQuestNameLength-2,g_bIsJapan);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"isEndCode")==0)
				break;

			ERRMSG("can not find follow keyword in mainQuest.txt",lpstrKeyWord);

			break;
		}

		break;
	}

	while(1)
	{
//	책
		sprintf(strFileName,"localize/%s/import/book.txt",m_strLocalizeFolder);

		if	(!decoder.Upload(strFileName,NULL))
		{
			ERRMSG("can not find follow file",strFileName);

			break;
		}

		while(1)
		{
			char			*lpstrKeyWord	=	decoder.GetWord();
			cBookInfo		*lpBook;

			if	(stricmp(lpstrKeyWord,"bookName")==0)
			{
				int		iIndex		=	decoder.GetNumber();
				int		iCheckSum	=	decoder.GetNumber();
				char	*lpstrName	=	decoder.GetStream();

				lpBook				=	l_bookManager.getBook(iIndex);

				if	(!lpBook)
					continue;

				ustrncpy(lpBook->m_strBookName,lpstrName,c_iBookNameLength-2,g_bIsJapan);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"bookItemComment")==0)
			{
				int		iIndex		=	decoder.GetNumber();
				int		iCheckSum	=	decoder.GetNumber();
				char	*lpstrComment=	decoder.GetStream();

				lpBook				=	l_bookManager.getBook(iIndex);

				if	(!lpBook)
					continue;

				lpBook->setItemComment(lpstrComment);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"bookCategory")==0)
			{
				int		iIndex		=	decoder.GetNumber();
				int		iCategory	=	decoder.GetNumber();
				int		iCheckSum	=	decoder.GetNumber();
				char	*lpstrName	=	decoder.GetStream();

				lpBook				=	l_bookManager.getBook(iIndex);

				if	(!lpBook)
					continue;

				ustrncpy(lpBook->m_aCategory[iCategory].m_strCategoryName,lpstrName,c_iBookCategoryNameLength-2,g_bIsJapan);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"bookPage")==0)
			{
				int		iIndex		=	decoder.GetNumber();
				int		iCategory	=	decoder.GetNumber();
				int		iPage		=	decoder.GetNumber();
				int		iCheckSum	=	decoder.GetNumber();
				char	*lpstrText	=	decoder.GetStream();

				cBookPageInfo	*lpPage	=	l_bookManager.getPage(iIndex,iCategory,iPage);

				if	(!lpPage)
					continue;

				lpPage->setText(lpstrText);

				continue;
			}

			if	(stricmp(lpstrKeyWord,"isEndCode")==0)
				break;

			ERRMSG("can not find follow keyword in book.txt",lpstrKeyWord);

			break;
		}

		break;
	}

	{
//	퀘스트 
		sprintf(strFileName,"localize/%s/import/quest.txt",m_strLocalizeFolder);

//	비밀 던전
		{
			cDECODER	decoder;
			char		*lpstrName;

			if	(!decoder.Upload(_ms("localize/%s/import/_L_secret_dungeon.txt",m_strLocalizeFolder),NULL))
				return	ERRMSG("can not find follow file","localize/%s/import/_L_secret_dungeon.txt",m_strLocalizeFolder);

			while(1)
			{
				int	iSerial	=	decoder.GetNumber();

				if	(iSerial==	-1 || iSerial	==	0x7fffffff)
					break;

				cSecretDungeon	*lpDungeon	=	l_sdManager.get(iSerial);

				if	(!lpDungeon)
					continue;

				lpstrName	=	decoder.GetStream();

				ustrncpy(lpDungeon->m_strName,lpstrName,dSECRET_DUNGEON_NAME_LENGTH-2,g_bIsJapan);
			}
		}

		decoder.Upload(strFileName,NULL);

		CQuestItem	*lpQuest;

		int	iQuest	=	-1;
		int	iArbeit	=	-1;

		while(1)
		{
			char	*lpstrWord	=	decoder.GetWord();
			char	*lpstrStream;

			if (stricmp(lpstrWord,"quest")==0)
			{
				decoder.GetNumber();
				int	iSerial	=	decoder.GetNumber();
				iQuest		=	iSerial;
				iArbeit		=	-1;

				lpstrStream	=	decoder.GetStream();

				lpQuest		=	l_localWorkQuest.getQuest(iSerial);

				if	(lpQuest)
					strncpy(lpQuest->m_strName,lpstrStream,dQUEST_NAME_SIZE-2);

				continue;
			}
			
			if (stricmp(lpstrWord,"arbeit")==0)
			{
				decoder.GetNumber();
				int	iSerial	=	decoder.GetNumber();
				iArbeit		=	iSerial;
				iQuest		=	-1;

				lpstrStream	=	decoder.GetStream();

				lpQuest		=	l_localWorkarbeit.getQuest(iSerial);

				if	(lpQuest)
					strncpy(lpQuest->m_strName,lpstrStream,dQUEST_NAME_SIZE-2);

				continue;
			}
			
			if (stricmp(lpstrWord,"client")==0)
			{
				decoder.GetNumber();
				lpstrStream	=	decoder.GetStream();
				if	(lpQuest)
					strcpy(lpQuest->m_strQuestClientInfo,lpstrStream);

				continue;
			}
			if (stricmp(lpstrWord,"beginClient")==0)
			{
				decoder.GetNumber();
				lpstrStream	=	decoder.GetStream();
				if	(lpQuest)
					strcpy(lpQuest->m_strBeginNPCName,lpstrStream);

				continue;
			}
			if (stricmp(lpstrWord,"value")==0)
			{
				decoder.GetNumber();
				lpstrStream	=	decoder.GetStream();
				if	(lpQuest)
					strcpy(lpQuest->m_strQuestValueComment,lpstrStream);

				continue;
			}
			if (stricmp(lpstrWord,"explain")==0)
			{
				decoder.GetNumber();
				int	iIndex	=	decoder.GetNumber();
				lpstrStream	=	decoder.GetStream();
				if	(lpQuest)
					strcpy(lpQuest->m_astrExplain[iIndex],lpstrStream);

				continue;
			}

			if	(stricmp(lpstrWord,"isEndCode")==0)
				break;
			
			ERRMSG("quest.txt","quest %d,arbeit %d\n%s",iQuest,iArbeit,lpstrWord);

			break;
		}

		sprintf(strPath,"%s/localize/%s/result data/scenario",_logFolder,m_strLocalizeFolder);
		sprintf(strFileName,"%s.rpd",cPROJECT::s_strName);

		cPROJECT::SaveProject(strFileName,strPath,&l_localWorkQuest,&l_localWorkarbeit,&l_sdManager,&l_chapterManager,&l_bookManager,&l_kcMonsterDeath,&l_kcMonsterGen,TRUE);
	}

//	아이템
	{
		sprintf(strFileName,"%s/localize/%s/data/scenario/%s/item.dat",_logFolder,m_strLocalizeFolder,cPROJECT::s_strName);

		if	(!cPROJECT::LoadItem(strFileName,&l_premiumItem,&l_customItem,&l_itemPrefixManager,&l_arcaManager,&l_itemSetManager,&l_pieceItemManager,l_aBasicItem))
			return	ERRMSG("localize item data error!!","!cPROJECT::LoadItem");

		//	프리미엄 아이템
		{
			sprintf(strFileName,"localize/%s/import/_L_PremiumItem.txt",m_strLocalizeFolder);

			if	(!decoder.Upload(strFileName,NULL))
				return	ERRMSG("can not find follow file","localize/import/_L_PremiumItem.txt");

			while(1)
			{
				int	iSerial		=	decoder.GetNumber();

				if (iSerial	==	0x7fffffff || iSerial	==	-1)
					break;

				char	*lpstrName=	decoder.GetStream();

				CCustomItem	*lpItem	=	l_premiumItem.getItem(iSerial);

				if	(!lpItem)
					continue;

				ustrncpy(lpItem->m_strName,lpstrName,30-2,g_bIsJapan);
			}
		}

		//	세트 아이템
		{
			sprintf(strFileName,"localize/%s/import/_L_item_set.txt",m_strLocalizeFolder);

			if	(!decoder.Upload(strFileName,NULL))
				return	ERRMSG("can not find follow file","localize/import/_L_item_set.txt");

			while(1)
			{
				char		*lpstrKeyWord	=	decoder.GetWord();
				cItemSet	*lpItemSet;

				if	(stricmp(lpstrKeyWord,"set")==0)
				{
					
					int		iSet		=	decoder.GetNumber();
					int		iCheckSum	=	decoder.GetNumber();
					char	*lpstrName	=	decoder.GetStream();

					lpItemSet			=	l_itemSetManager.get(iSet);

					if	(!lpItemSet)
						continue;

					ustrncpy(lpItemSet->m_strName,lpstrName,30-2,g_bIsJapan);

					continue;
				}

				if	(stricmp(lpstrKeyWord,"set_explain")==0)
				{
					int		iSet		=	decoder.GetNumber();
					int		iCheckSum	=	decoder.GetNumber();
					char	*lpstrExplain=	decoder.GetStream();

					lpItemSet			=	l_itemSetManager.get(iSet);

					if	(!lpItemSet)
						continue;

					lpItemSet->setComment(lpstrExplain);

					continue;
				}

				if	(stricmp(lpstrKeyWord,"set_item_name")==0)
				{
					int		iSet		=	decoder.GetNumber();
					int		iSlot		=	decoder.GetNumber();
					int		iCheckSum	=	decoder.GetNumber();
					char	*lpstrName	=	decoder.GetStream();

					lpItemSet			=	l_itemSetManager.get(iSet);

					if	(!lpItemSet)
						continue;

					ustrncpy(lpItemSet->m_aItem[iSlot].m_strName,lpstrName,30-2,g_bIsJapan);

					continue;
				}

				if	(stricmp(lpstrKeyWord,"set_item_explain")==0)
				{
					int		iSet		=	decoder.GetNumber();
					int		iSlot		=	decoder.GetNumber();
					int		iCheckSum	=	decoder.GetNumber();
					char	*lpstrExplain=	decoder.GetStream();

					lpItemSet			=	l_itemSetManager.get(iSet);

					if	(!lpItemSet)
						continue;

					lpItemSet->m_aItem[iSlot].setComment(lpstrExplain);

					continue;
				}

				break;
			}
		}

		//	아이템
		{
			sprintf(strFileName,"localize/%s/import/_L_item.txt",m_strLocalizeFolder);

			if	(!decoder.Upload(strFileName,NULL))
				return	ERRMSG("can not find follow file","localize/import/_L_item.txt");

			char	*lpstrName,*lpstrComment;
			int		iItemCount	=	0,iSerial;

			while(1)
			{
				iSerial		=	decoder.GetNumber();

				if (iSerial	==	0x7fffffff || iSerial	==	-1)
					break;

				lpstrName	=	decoder.GetStream();
				ustrncpy(l_aBasicItem[iSerial].m_strName,lpstrName,dITEM_NAME_LENGTH-2,g_bIsJapan);

				lpstrComment=	decoder.GetStream();
				l_aBasicItem[iSerial].setComment(lpstrComment);
				iItemCount++;

				l_aBasicItem[iSerial].m_aDummyData[0]	=	1;
			}

			for (i=0;i<dBASIC_ITEM_COUNT;i++)
			{
				if	(l_aBasicItem[i].m_iSerial	<	dBASIC_ITEM_COUNT	&&	l_aBasicItem[i].m_iSerial	>=	0)
				{
					if	(l_aBasicItem[i].m_aDummyData[0]	!=	1)
						cMSG::Put("fault in text file script","serial [%d] item text not found",i);

					l_aBasicItem[iSerial].m_aDummyData[0]	=	0;
				}
			}
		}

		//	아이템 접두사
		{
			sprintf(strFileName,"localize/%s/import/_L_itemPrefix.txt",m_strLocalizeFolder);

			if (!decoder.Upload(strFileName,NULL))
				return	ERRMSG("can not find follow file","localize/%s/import/_L_itemPrefix.txt",m_strLocalizeFolder);

			char	*lpstrPrefix1,*lpstrPrefix2;

			while(1)
			{
				int	iSerial	=	decoder.GetNumber();

				if (iSerial	==	0x7fffffff || iSerial	==	-1)
					break;

				cITEM_PREFIX	*lpPrefix	=	l_itemPrefixManager.get(iSerial);

				lpstrPrefix1	=	decoder.GetStream();

				if	(strlen(lpstrPrefix1)	>=	20)
					MSGOUT(lpstrPrefix1);

				ustrncpy(lpPrefix->m_str1stPrefix,lpstrPrefix1,20-2,g_bIsJapan);

				lpstrPrefix2	=	decoder.GetStream();
				ustrncpy(lpPrefix->m_str2stPrefix,lpstrPrefix2,20-2,g_bIsJapan);
			}
		}

		sprintf(strFileName,"%s/localize/%s/result data/scenario/%s/item.dat",_logFolder,m_strLocalizeFolder,cPROJECT::s_strName);

		cPROJECT::SaveItem(strFileName,&l_premiumItem,&l_customItem,&l_itemPrefixManager,&l_arcaManager,&l_itemSetManager,&l_pieceItemManager,l_aBasicItem,TRUE);
	}

//	스킬
	{
		memcpy(l_aSkill,g_aSkill,sizeof(l_aSkill));

		if	(cSKILL::Init(_ms("localize/%s/data/skill2.dat",m_strLocalizeFolder)))
		{
			char		*lpstrName,*lpstrPowerUp,*lpstrComment;
			cDECODER	decoder;

			if	(!decoder.Upload(_ms("localize/%s/import/_L_skill.txt",m_strLocalizeFolder),NULL))
				return	ERRMSG("can not find follow file","localize/%s/import/_L_skill.txt",m_strLocalizeFolder);

			int	iCount	=	0;

			while(1)
			{
				int	iSerial	=	decoder.GetNumber();

				if (iSerial	==	-1	||	iSerial	==	0x7fffffff)
					break;

				iCount++;

				lpstrName	=	decoder.GetStream();
				ustrncpy(g_aSkill[iSerial].m_strName,lpstrName,dSKILL_NAME_LENGTH-2,g_bIsJapan);

				lpstrComment=	decoder.GetStream();
				ustrncpy(g_aSkill[iSerial].m_strComment,lpstrComment,dSKILL_COMMENT_LENGTH-2,g_bIsJapan);

				lpstrPowerUp=	decoder.GetStream();
				ustrncpy(g_aSkill[iSerial].m_strPowerup,lpstrPowerUp,dSKILL_POWER_UP_COMMENT_LENGTH-2,g_bIsJapan);
			}

			cSKILL::SaveData(_ms("localize/%s/result data/skill2.dat",m_strLocalizeFolder));

			memcpy(g_aSkill,l_aSkill,sizeof(l_aSkill));
		}
	}

//	길드 스킬
	{
		CGuildSkillManager	gsManager;
		cDECODER			decoder;
		char				*lpstrName,*lpstrComment;

		if	(!gsManager.loadData(_ms("localize/%s/data/guildSkill.dat",m_strLocalizeFolder)))
			return	ERRMSG("can not find follow file","localize/%s/data/guildSkill.dat",m_strLocalizeFolder);

		if	(!decoder.Upload(_ms("localize/%s/import/_L_guildSkill.txt",m_strLocalizeFolder),NULL))
			return	ERRMSG("can not find follow file","localize/%s/import/_L_guildSkill.txt",m_strLocalizeFolder);

		while(1)
		{
			int	iSerial	=	decoder.GetNumber();

			if (iSerial	==	-1 || iSerial	==	0x7fffffff)
				break;

			lpstrName	=	decoder.GetStream();
			ustrncpy(gsManager.m_aSkills[iSerial].m_strName,lpstrName,dSKILL_NAME_LENGTH-2,g_bIsJapan);

			lpstrComment=	decoder.GetStream();
			ustrncpy(gsManager.m_aSkills[iSerial].m_strComment,lpstrComment,dSKILL_COMMENT_LENGTH-2,g_bIsJapan);
		}

		gsManager.saveData(_ms("localize/%s/result data/guildSkill.dat",m_strLocalizeFolder));
	}

//	직업
	{
		cDECODER			decoder;
		char				*lpstrName;

		sprintf(strDataFileName,"localize/%s/data/scenario/red stone/job.dat",m_strLocalizeFolder);

		if	(!cPROJECT::LoadJob(&jobDen,strDataFileName))
			return	ERRMSG("can not find follow file","%s",strDataFileName);

		cPROJECT::ReadCorrectJobData(&jobDen);

		if (!decoder.Upload(_ms("localize/%s/import/_L_job.txt",m_strLocalizeFolder),NULL))
			return	ERRMSG("can not find follow file","localize/%s/import/_L_job.txt",m_strLocalizeFolder);

		while(1)
		{
			int	iSerial	=	decoder.GetNumber();

			if (iSerial	==	-1 || iSerial	==	0x7fffffff)
				break;

			lpstrName	=	decoder.GetStream();
			ustrncpy(jobDen.m_aJobList[iSerial].m_strName,lpstrName,32-2,g_bIsJapan);

			decoder.GetStream();
		}

		jobDen.saveData(_ms("localize/%s/result data/scenario/red stone/job2.dat",m_strLocalizeFolder));
	}

//	타이틀
	{
		CTitleManager	titleManager;
		cDECODER		decoder;
		char			*lpstrName,*lpstrComment;

		sprintf(strDataFileName,"localize/%s/data/title.dat",m_strLocalizeFolder);

		if	(!titleManager.loadData(strDataFileName))
			return	FALSE;

		if	(!decoder.Upload(_ms("localize/%s/import/_L_title.txt",m_strLocalizeFolder),NULL))
			return	ERRMSG("can not find follow file","localize/%s/import/_L_title.txt",m_strLocalizeFolder);

		while(1)
		{
			int	iSerial	=	decoder.GetNumber();

			if (iSerial	==	-1	||	iSerial	==	0x7fffffff)
				break;

			lpstrName	=	decoder.GetStream();
			ustrncpy(titleManager.m_aTitles[iSerial].m_strName,lpstrName,dTITLE_NAME_LENGTH-2,g_bIsJapan);

			lpstrComment=	decoder.GetStream();
			ustrncpy(titleManager.m_aTitles[iSerial].m_strExplain,lpstrComment,dTITLE_EXPLAIN_LENGTH-2,g_bIsJapan);
		}

		titleManager.saveData(_ms("localize/%s/result data/title.dat",m_strLocalizeFolder));
	}

	g_loaderText.run(m_strLocalizeFolder);

	cMSG::Put("localize all game text data","complete");

	return	TRUE;
}

BOOL
cMAIN::exportLoclalizeMapList(HWND hwnd)
{
	if	(!selectLocalizeFolder(hwnd))
		return	FALSE;

	int			i;

	SetLogFolder();

	if	(!IsFolder(_ms("localize/%s/import",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/import",_logFolder);

	char	strSrcMapFolder[1024];

	sprintf(strSrcMapFolder,"localize/%s",m_strLocalizeFolder);

	for (i=0;i<dMAX_MAP_COUNT;i++)
	{
		strcpy(l_aMapSimpleInfo[i].m_strName,"valid");
		l_aMapSimpleInfo[i].m_wSerial	=	0xffff;
	}

	cSTRINGS	*lpFiles;
	int			iWorkMapCount	=	0;

	{
		cPROJECT::SetMapFolder(strSrcMapFolder);

		lpFiles	=	cFOLDER::GetFiles("*.rmd");

		SetLogFolder();

		if (!lpFiles)
			return	ERRMSG("error!!","can not find map list!");

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);

			if	(!lpFile)
				break;

			if	(lpFile->String[0]	!=	'[')
			{
				lpFile		=	lpFile->pNEXT;
				i++;
				continue;
			}

			char	strIndex[10];

			strncpy(strIndex,lpFile->String+1,3);
			strIndex[3]	=	NULL;

			int	iIndex	=	atoi(strIndex);

			if	(!IsConvertMap(iIndex))
				continue;

			cPROJECT::SetMapFolder(strSrcMapFolder);

			cMAP::Load(lpFile->String,0);

			g_aMap[0].importText(FALSE,FALSE);

			l_aMapSimpleInfo[iIndex].m_wWidth			=	g_aMap[0].m_iWidth;
			l_aMapSimpleInfo[iIndex].m_wHeight			=	g_aMap[0].m_iHeight;
			strcpy(l_aMapSimpleInfo[iIndex].m_strName,g_aMap[0].m_strName);
			strcpy(l_aMapSimpleInfo[iIndex].m_strFileName,lpFile->String);

			l_aMapSimpleInfo[iIndex].m_wSerial			=	iIndex;
			l_aMapSimpleInfo[iIndex].m_bf8Type			=	g_aMap[0].m_bf4FieldType;
			l_aMapSimpleInfo[iIndex].m_bf1IsPremiumZone	=	g_aMap[0].m_bf1IsPremiumZone;

			SetLogFolder();
			Draw();
			Sleep(1);

			iWorkMapCount++;
		}

		SetLogFolder();

		cFILE	file;

		file.Open(_ms("localize/%s/result data/mapList.dat",m_strLocalizeFolder),"wb");
		file.Write(l_aMapSimpleInfo,sizeof(l_aMapSimpleInfo));
		file.Close();

		KILL(lpFiles);
	}

	cMSG::Put("map work","complete [%d] numbers map data",iWorkMapCount);

	return	TRUE;
}

BOOL
cMAIN::uploadProjectKarmaText(void *_lpPK,char *_lpstrFileName)
{
	CProjectKarmaText	*lpPK	=	(CProjectKarmaText	*)_lpPK;
	cDECODER	decoder;

	lpPK->reset();

	if	(!decoder.Upload(_lpstrFileName,NULL))
		return FALSE;

	int	iType	=	0;
	int	iKarma;
	int	iReaction;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		if (stricmp(lpstrWord,"md_karma_reaction")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			iKarma				=	decoder.GetNumber();
			iReaction			=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n [md_karma_reaction] 문자열 시작이 없다!\nIndex %d",_lpstrFileName,iKarma,iReaction);

				return	FALSE;
			}

			char	*lpstrStream=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n [md_karma_reaction] 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iKarma,iReaction);

				return	FALSE;
			}

			lpPK->m_aMD_Karma[iKarma].m_apReaction[iReaction]	=	new	CReactionText();

			CReactionText	*lpReaction			=	lpPK->m_aMD_Karma[iKarma].m_apReaction[iReaction];

			lpReaction->m_iCheckSum				=	iCheckSum;
			lpReaction->m_iKarma				=	iKarma;
			lpReaction->m_string.Set(lpstrStream);
			lpPK->m_aMD_Karma[iKarma].m_iCheckSum	=	1;

			iType	=	0;

			continue;
		}

		if (stricmp(lpstrWord,"mg_karma_reaction")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			iKarma				=	decoder.GetNumber();
			iReaction			=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n [mg_karma_reaction] 문자열 시작이 없다!\nIndex %d",_lpstrFileName,iKarma,iReaction);

				return	FALSE;
			}

			char	*lpstrStream=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n [mg_karma_reaction] 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iKarma,iReaction);

				return	FALSE;
			}

			lpPK->m_aMG_Karma[iKarma].m_apReaction[iReaction]	=	new	CReactionText();

			CReactionText	*lpReaction			=	lpPK->m_aMG_Karma[iKarma].m_apReaction[iReaction];

			lpReaction->m_iCheckSum				=	iCheckSum;
			lpReaction->m_iKarma				=	iKarma;
			lpReaction->m_string.Set(lpstrStream);
			lpPK->m_aMG_Karma[iKarma].m_iCheckSum	=	1;

			iType	=	0;

			continue;
		}

		if	(stricmp(lpstrWord,"isEndCode")==0)
			break;

		ERRMSG(_lpstrFileName,"%s\nuploadProjectKarmaText : type %d,karma %d,reaction %d",lpstrWord,iType,iKarma,iReaction);

		break;
	}

	return	TRUE;
}


BOOL
cMAIN::uploadSkillText(void *_lpSkill,char *_lpstrFileName)
{
	CSkillText	*lpSkillText	=	(CSkillText	*)_lpSkill;
	cDECODER	decoder;

	lpSkillText->reset();

	if (!decoder.Upload(_lpstrFileName,NULL))
		return FALSE;

	while(1)
	{
		int	iIndex	=	decoder.GetNumber();

		if (iIndex	==	-1 || iIndex	==	0x7fffffff)
			break;

		lpSkillText->m_aSkill[iIndex].m_iNameCheckSum		=	decoder.GetNumber();
		lpSkillText->m_aSkill[iIndex].m_iPowerUpCheckSum	=	decoder.GetNumber();
		lpSkillText->m_aSkill[iIndex].m_iExplainCheckSum	=	decoder.GetNumber();

		{
			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n [이름] 문자열 시작이 없다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			lpSkillText->m_aSkill[iIndex].m_strName.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n [이름] 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}
		}
		
		{
			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n [설명] 문자열 시작이 없다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			lpSkillText->m_aSkill[iIndex].m_strPowerUp.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n [설명] 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}
		}

		{
			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n [파워업] 문자열 시작이 없다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			lpSkillText->m_aSkill[iIndex].m_strExplain.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n [파워업]] 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}
		}
	}

	return	TRUE;
}

BOOL
cMAIN::upload1KindText(void *_lpItem,char *_lpstrFileName,BOOL _bIsReadCheckSum)
{
	C1KindText	*lpItemText	=	(C1KindText	*)_lpItem;
	cDECODER	decoder;

	lpItemText->reset();

	if	(!decoder.Upload(_lpstrFileName,NULL))
		return FALSE;

	int	iCount	=	0;
	int	iIndex	=	0;

	while(1)
	{
		char	*lpstr;

		if	(_bIsReadCheckSum)
		{
			iIndex	=	decoder.GetNumber();

			if (iIndex	==	-1 || iIndex	==	0x7fffffff)
				break;

			lpItemText->m_aItems[iIndex].m_iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n 코드 뒤에 뒤에 문자열이 없다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			lpstr		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}
		}
		else
		{
			iIndex	=	iCount;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n 코드 뒤에 뒤에 문자열이 없다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			lpstr	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			if	(stricmp(lpstr,"stream not Found!!")==	0 || stricmp(lpstr,"__end")==	0)
				break;

			lpItemText->m_aItems[iIndex].m_iCheckSum	=	0;
		}

		lpItemText->m_aItems[iIndex].m_strText.Set(lpstr);

		iCount++;
	}

	return	TRUE;
}
	
BOOL
cMAIN::uploadSetItemKindText(void *_lpSetItem,char *_lpstrFileName)
{
	cItemSetText	*lpItemText	=	(cItemSetText	*)_lpSetItem;
	cDECODER		decoder;

	lpItemText->reset();

	if	(!decoder.Upload(_lpstrFileName,NULL))
		return FALSE;

	int	iCount	=	0;

	while(1)
	{
		char	*lpstrKeyWord	=	decoder.GetWord();

		if	(stricmp(lpstrKeyWord,"set")==0)
		{
			int		iIndex		=	decoder.GetNumber();
			int		iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n set 코드 뒤에 뒤에 문자열이 없다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			char	*lpstrName	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n set 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			lpItemText->m_aSetItem[iIndex].m_strName.Set(lpstrName);
			lpItemText->m_aSetItem[iIndex].m_iNameCheckSum	=	iCheckSum;

			continue;
		}

		if	(stricmp(lpstrKeyWord,"set_explain")==0)
		{
			int		iIndex		=	decoder.GetNumber();
			int		iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n set_explain 코드 뒤에 뒤에 문자열이 없다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			char	*lpstrExplain=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n set_explain 문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}

			lpItemText->m_aSetItem[iIndex].m_strExplain.Set(lpstrExplain);
			lpItemText->m_aSetItem[iIndex].m_iExplainCheckSum	=	iCheckSum;

			continue;
		}

		if	(stricmp(lpstrKeyWord,"set_item_name")==0)
		{
			int		iSet		=	decoder.GetNumber();
			int		iSlot		=	decoder.GetNumber();
			int		iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n set_item_name 코드 뒤에 뒤에 문자열이 없다!\nIndex %d,%d",_lpstrFileName,iSet,iSlot);

				return	FALSE;
			}

			char	*lpstrName	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n set_item_name 문자열 뒤에 쓰레기 값이있다!\nIndex %d,%d",_lpstrFileName,iSet,iSlot);

				return	FALSE;
			}

			lpItemText->m_aSetItem[iSet].m_aData[iSlot].m_strName.Set(lpstrName);
			lpItemText->m_aSetItem[iSet].m_aData[iSlot].m_iNameCheckSum	=	iCheckSum;

			continue;
		}

		if	(stricmp(lpstrKeyWord,"set_item_explain")==0)
		{
			int		iSet		=	decoder.GetNumber();
			int		iSlot		=	decoder.GetNumber();
			int		iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n set_item_explain 코드 뒤에 뒤에 문자열이 없다!\nIndex %d,%d",_lpstrFileName,iSet,iSlot);

				return	FALSE;
			}

			char	*lpstrExplain=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n set_item_explain 문자열 뒤에 쓰레기 값이있다!\nIndex %d,%d",_lpstrFileName,iSet,iSlot);

				return	FALSE;
			}

			lpItemText->m_aSetItem[iSet].m_aData[iSlot].m_strExplain.Set(lpstrExplain);
			lpItemText->m_aSetItem[iSet].m_aData[iSlot].m_iExplainCheckSum	=	iCheckSum;

			continue;
		}

		break;
	}

	return	TRUE;
}

BOOL
cMAIN::upload2KindText(void *_lpItem,char *_lpstrFileName,BOOL _bIsStreamInOneLine)
{
	C2KindText	*lpItemText	=	(C2KindText	*)_lpItem;
	cDECODER	decoder;

	lpItemText->reset();

	if (!decoder.Upload(_lpstrFileName,NULL))
		return FALSE;

	int	iCount	=	0;

	while(1)
	{
		int	iIndex	=	decoder.GetNumber();

		if	(iIndex	==	-1 || iIndex	==	0x7fffffff)
			break;

		lpItemText->m_aItems[iIndex].m_iNameCheckSum	=	decoder.GetNumber();
		lpItemText->m_aItems[iIndex].m_iExplainCheckSum	=	decoder.GetNumber();

		if	(!decoder.findCharWithRange('"',5))
		{
			ERRMSG("","%s\n\n 코드 뒤에 뒤에 첫번째 문자열이 없다!\nIndex %d",_lpstrFileName,iIndex);

			return	FALSE;
		}

		lpItemText->m_aItems[iIndex].m_strName.Set(decoder.GetStream());

		if	(!decoder.isEmptyLine())
		{
			if	(_bIsStreamInOneLine	==	FALSE)
			{
				ERRMSG("","%s\n\n문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

				return	FALSE;
			}
		}

		if	(!decoder.findCharWithRange('"',5))
		{
			ERRMSG("","%s\n\n 두번째 문자열 시작이 없다!\nIndex %d",_lpstrFileName,iIndex);

			return	FALSE;
		}

		lpItemText->m_aItems[iIndex].m_strExplain.Set(decoder.GetStream());

		if	(!decoder.isEmptyLine())
		{
			ERRMSG("","%s\n\n문자열 뒤에 쓰레기 값이있다!\nIndex %d",_lpstrFileName,iIndex);

			return	FALSE;
		}

		iCount++;
	}

	return	TRUE;
}

BOOL
cMAIN::uploadMainQuestText(void *_lpText,char *_lpstrFileName)
{
	cMainQuestText	*lpMQT	=	(cMainQuestText	*)_lpText;
	cDECODER		decoder;
	char			*lpstrText;

	lpMQT->reset();

	if	(!decoder.Upload(_lpstrFileName,NULL))
		return FALSE;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		if (stricmp(lpstrWord,"mainQuestName")==0)
		{
			int	iQuest		=	decoder.GetNumber();
			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\nmainQuestName 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuest);

				return	FALSE;
			}

			lpstrText		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nmainQuestName 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuest);

				return	FALSE;
			}

			lpMQT->m_aChapterList[iQuest].m_iNameCheckSum	=	iCheckSum;

			strncpy(lpMQT->m_aChapterList[iQuest].m_strName,lpstrText,c_iChapterNameLength-2);

			continue;
		}

		if (stricmp(lpstrWord,"mainQuestComment")==0)
		{
			int	iQuest		=	decoder.GetNumber();
			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n mainQuestComment 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuest);

				return	FALSE;
			}

			lpstrText		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n mainQuestComment 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuest);

				return	FALSE;
			}

			lpMQT->m_aChapterList[iQuest].m_iCommentCheckSum	=	iCheckSum;

			strncpy(lpMQT->m_aChapterList[iQuest].m_strComment,lpstrText,c_iChapterCommentLength-2);

			continue;
		}
		if (stricmp(lpstrWord,"mainQuestEpilogComment")==0)
		{
			int	iQuest		=	decoder.GetNumber();
			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n mainQuestEpilogComment 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuest);

				return	FALSE;
			}

			lpstrText		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n mainQuestEpilogComment 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuest);

				return	FALSE;
			}

			lpMQT->m_aChapterList[iQuest].m_iEpilogCommentCheckSum	=	iCheckSum;

			strncpy(lpMQT->m_aChapterList[iQuest].m_strEpilogComment,lpstrText,c_iChapterEpilogCommentLength-2);

			continue;
		}

		if (stricmp(lpstrWord,"highQuestName")==0)
		{
			int	iQuest		=	decoder.GetNumber();
			int	iHighQuest	=	decoder.GetNumber();
			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n highQuestName 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuest);

				return	FALSE;
			}

			lpstrText		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n highQuestName 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuest);

				return	FALSE;
			}

			strncpy(lpMQT->m_aChapterList[iQuest].m_astrHighQuest[iHighQuest],lpstrText,c_iHighQuestNameLength-2);
			lpMQT->m_aChapterList[iQuest].m_aiHiQuestNameChecksum[iHighQuest]	=	iCheckSum;
			continue;
		}

		if (stricmp(lpstrWord,"isEndCode")==0)
			break;

		ERRMSG(_lpstrFileName,lpstrWord);
		break;
	}

	return	TRUE;
}

BOOL
cMAIN::uploadBookText(void *_lpText,char *_lpstrFileName)
{
	cBookTextPack	*lpBT	=	(cBookTextPack	*)_lpText;
	cDECODER		decoder;
	char			*lpstrText;

	lpBT->reset();

	if	(!decoder.Upload(_lpstrFileName,NULL))
		return FALSE;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		if (stricmp(lpstrWord,"bookName")==0)
		{
			int	iBook		=	decoder.GetNumber();
			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n bookName 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iBook);

				return	FALSE;
			}

			lpstrText		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n bookName 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iBook);

				return	FALSE;
			}

			lpBT->m_aBook[iBook].m_iCheckSum	=	iCheckSum;
			strncpy(lpBT->m_aBook[iBook].m_strBookName,lpstrText,c_iBookNameLength-2);

			continue;
		}
		if (stricmp(lpstrWord,"bookItemComment")==0)
		{
			int	iBook		=	decoder.GetNumber();
			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n bookItemComment 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iBook);

				return	FALSE;
			}

			lpstrText		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n bookItemComment 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iBook);

				return	FALSE;
			}

			lpBT->m_aBook[iBook].m_iCommentCheckSum	=	iCheckSum;
			lpBT->m_aBook[iBook].m_strItemComment.Set(lpstrText);

			continue;
		}

		if (stricmp(lpstrWord,"bookCategory")==0)
		{
			int	iBook		=	decoder.GetNumber();
			int	iCategory	=	decoder.GetNumber();
			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n bookCategory 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iBook);

				return	FALSE;
			}

			lpstrText		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n bookCategory 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iBook);

				return	FALSE;
			}

			cBookCategoryText	*lpCategory	=	&lpBT->m_aBook[iBook].m_aCategory[iCategory];

			lpCategory->m_iCategoryCheckSum	=	iCheckSum;
			strncpy(lpCategory->m_strCategoryName,lpstrText,c_iBookCategoryNameLength-2);

			continue;
		}

		if (stricmp(lpstrWord,"bookPage")==0)
		{
			int	iBook		=	decoder.GetNumber();
			int	iCategory	=	decoder.GetNumber();
			int	iPage		=	decoder.GetNumber();
			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n bookPage 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iBook);

				return	FALSE;
			}

			lpstrText		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n bookPage 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iBook);

				return	FALSE;
			}

			cBookPageText	*lpPage	=	&lpBT->m_aBook[iBook].m_aCategory[iCategory].m_aPage[iPage];

			lpPage->m_strText.Set(lpstrText);
			lpPage->m_iCheckSum		=	iCheckSum;
			continue;
		}

		if (stricmp(lpstrWord,"isEndCode")==0)
			break;

		ERRMSG(_lpstrFileName,lpstrWord);
		break;
	}

	return	TRUE;
}

BOOL
cMAIN::uploadQuestText(void *_lpQuest,char *_lpstrFileName)
{
	CQuestText		*lpQuestText	=	(CQuestText	*)_lpQuest;
	cDECODER		decoder;
	CQuestTextItem	*lpItem;

	lpQuestText->reset();

	if	(!decoder.Upload(_lpstrFileName,NULL))
		return FALSE;

	int		iQuestIndex=-1,iArbeitIndex=-1;
	int		iExplainIndex=-1;
	char	strLastString[256];

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		if (stricmp(lpstrWord,"quest")==0)
		{
			int	iCheckSum			=	decoder.GetNumber();
			iQuestIndex				=	decoder.GetNumber();

			lpItem					=	&lpQuestText->m_aQuest[iQuestIndex];
			lpItem->m_iNameCheckSum	=	iCheckSum;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n quest 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			strcpy(lpItem->m_strName,decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n quest 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			strcpy(strLastString,lpstrWord);
			continue;
		}

		if (stricmp(lpstrWord,"arbeit")==0)
		{
			int	iCheckSum			=	decoder.GetNumber();
			iArbeitIndex			=	decoder.GetNumber();

			lpItem					=	&lpQuestText->m_aArbeit[iArbeitIndex];
			lpItem->m_iNameCheckSum	=	iCheckSum;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n arbeit 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			strcpy(lpItem->m_strName,decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n arbeit 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			strcpy(strLastString,lpstrWord);
			continue;
		}

		if (stricmp(lpstrWord,"client")==0)
		{
			lpItem->m_iClientCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n client 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			char	*lpstrStream	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n client 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			if (lpItem->m_iClientCheckSum	==	0)
				strcpy(lpItem->m_strQuestClientInfo,"");
			else
				strcpy(lpItem->m_strQuestClientInfo,lpstrStream);



			strcpy(strLastString,lpstrWord);
			continue;
		}
		if (stricmp(lpstrWord,"beginClient")==0)
		{
			lpItem->m_iBeginClientCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n beginClient 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			char	*lpstrStream	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n beginClient 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			if	(lpItem->m_iBeginClientCheckSum	==	0)
				strcpy(lpItem->m_strBeginNPCName,"");
			else
				strcpy(lpItem->m_strBeginNPCName,lpstrStream);

			strcpy(strLastString,lpstrWord);
			continue;
		}
		if (stricmp(lpstrWord,"value")==0)
		{
			lpItem->m_iInfoCheckSum		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n value 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			char	*lpstrStream	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n value 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			if (lpItem->m_iInfoCheckSum	==	0)
				strcpy(lpItem->m_strQuestValueComment,"");
			else
				strcpy(lpItem->m_strQuestValueComment,lpstrStream);

			strcpy(strLastString,lpstrWord);
			continue;
		}
		if (stricmp(lpstrWord,"explain")==0)
		{
			int	iCheckSum			=	decoder.GetNumber();
			int	iIndex				=	decoder.GetNumber();

			iExplainIndex			=	iIndex;

			lpItem->m_iExplainCheckSum[iIndex]	=	iCheckSum;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\n explain 코드 뒤에 뒤에 문자열이 없다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			char	*lpstrStream	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\n explain 문자열 뒤에 쓰레기 값이있다!\ncode %d",_lpstrFileName,iQuestIndex);

				return	FALSE;
			}

			if (iCheckSum	==	0)
				strcpy(lpItem->m_astrExplain[iIndex],"");
			else
				strcpy(lpItem->m_astrExplain[iIndex],lpstrStream);

			strcpy(strLastString,lpstrWord);
			continue;
		}

		if	(stricmp(lpstrWord,"isEndCode")==0)
			break;

		ERRMSG(_lpstrFileName,"Error Word %s\nLast Success Word %s\n\nLast Quest Index %d\nLast Arbeit Index %d\nLast Explain Index %d",lpstrWord,strLastString,iQuestIndex,iArbeitIndex,iExplainIndex);
		break;
	}

	return	TRUE;
}


LRESULT CALLBACK
cMAIN::s_procSelectTargetLanguage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG	:
		{
			cLISTBOX::Add(GetDlgItem(hwnd,IDC_LIST),"일본어",eNationJapan);
			cLISTBOX::Add(GetDlgItem(hwnd,IDC_LIST),"중국어",eNationChina);
			cLISTBOX::Add(GetDlgItem(hwnd,IDC_LIST),"한국어",eNationKorea);
			cLISTBOX::Add(GetDlgItem(hwnd,IDC_LIST),"International",eNationInter);
			cLISTBOX::Add(GetDlgItem(hwnd,IDC_LIST),"미국판",eNationUsa);
			cLISTBOX::Add(GetDlgItem(hwnd,IDC_LIST),"태국판",eNationThai);
			break;
		}

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
			{
				case	IDOK				:
				{
					int	iSelectLanguage	=	cLISTBOX::GetSelect(GetDlgItem(hwnd,IDC_LIST));

					iSelectLanguage		=	cLISTBOX::GetData(GetDlgItem(hwnd,IDC_LIST),iSelectLanguage);

					g_iLocalizeNation	=	iSelectLanguage;

					g_bIsJapan	=	FALSE;
					
					if (iSelectLanguage	==	eNationJapan)
					{
						strcpy(MAIN.m_strLocalizeFolder,"japan");
						g_bIsJapan	=	TRUE;
					}
					else
					if (iSelectLanguage	==	eNationChina)
						strcpy(MAIN.m_strLocalizeFolder,"china");
					else
					if (iSelectLanguage	==	eNationKorea)
						strcpy(MAIN.m_strLocalizeFolder,"korea");
					else
					if (iSelectLanguage	==	eNationInter)
						strcpy(MAIN.m_strLocalizeFolder,"international");
					else
					if (iSelectLanguage	==	eNationUsa)
						strcpy(MAIN.m_strLocalizeFolder,"USA");
					else
					if (iSelectLanguage	==	eNationThai)
						strcpy(MAIN.m_strLocalizeFolder,"thai");
					else
						EndDialog(hwnd,IDCANCEL);

					EndDialog(hwnd,IDOK);
					break;
				}

				case	IDCANCEL			:
					EndDialog(hwnd,IDCANCEL);
					break;
			}
			break;
	}

	return (0L);
}

