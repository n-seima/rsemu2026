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

BOOL
cMAIN::compareLocalizeText(HWND hwnd,BOOL _bIsWantCheck)
{
	if	(!selectLocalizeFolder(hwnd))
		return	FALSE;

	char		strFileName[512];
	int			i;
	cDECODER	decoder;
	cFILE		file;

	SetLogFolder();

	char	strCheckFolder[512];
	char	strCheckResultFolder[512];
	char	strSourceFolder[512];
	char	strMapFolder[512];

	sprintf(strMapFolder,"%s/localize/%s/data/scenario/red stone/map",_logFolder,m_strLocalizeFolder);
	sprintf(strCheckFolder,"localize/%s/lastest",m_strLocalizeFolder);
	strcpy(strSourceFolder,"old");
	strcpy(strCheckResultFolder,"update");
	
	if	(_bIsWantCheck)
	{
		strcpy(strSourceFolder,"_modify result");
		strcpy(strCheckResultFolder,"_check result");
	}

	if	(!IsFolder(strMapFolder))
		return	ERRMSG("can not find follow folder",strMapFolder);
	if	(!IsFolder(strCheckFolder))
		return	ERRMSG("can not find follow folder","%s/%s",_logFolder,strCheckFolder);
	if	(!IsFolder(_ms("localize/%s/%s",m_strLocalizeFolder,strSourceFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/%s",_logFolder,m_strLocalizeFolder,strSourceFolder);
	if	(!IsFolder(_ms("localize/%s/%s",m_strLocalizeFolder,strCheckResultFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/%s",_logFolder,m_strLocalizeFolder,strCheckResultFolder);

	if	(SetCurrentDirectory(_ms("localize/%s/%s",m_strLocalizeFolder,strCheckResultFolder)))
	{
		cFOLDER::DeleteAllFile();
	}

	if (cMSG::YESNO("compare all maps text data","do you want continue this work?")	==	IDYES)
	{
		SetCurrentDirectory(strMapFolder);

		cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

		SetLogFolder();

		if	(!lpFiles)
			return	ERRMSG("error!!","can not find map list!");

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);

			SetCurrentDirectory(strMapFolder);
			char	strName[128];

			strcpy(strName,_exportFileName(lpFile->String));

			if	(cMAP::CompareText(strName,FALSE,_bIsWantCheck)	==	FALSE)
			{
				char	strSource[512],strDest[512];

				SetLogFolder();

				sprintf(strSource,"%s/%s.txt",strCheckFolder,strName);
				sprintf(strDest,"localize/%s/%s/%s.txt",m_strLocalizeFolder,strCheckResultFolder,strName);
				CopyFile(strSource,strDest,FALSE);
			}

			SetLogFolder();
			Draw();
			Sleep(1);
		}

		KILL(lpFiles);

		SetCurrentDirectory(strMapFolder);
		SetCurrentDirectory("expansion");

		lpFiles	=	cFOLDER::GetFiles("*.rmd");

		SetLogFolder();

		if	(!lpFiles)
			return	ERRMSG("error!!","can not find map list!");

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);

			SetCurrentDirectory(strMapFolder);
			SetCurrentDirectory("expansion");

			char	strName[128];

			strcpy(strName,_exportFileName(lpFile->String));

			if	(cMAP::CompareText(strName,TRUE,_bIsWantCheck)	==	FALSE)
			{
				char	strSource[512],strDest[512];

				SetLogFolder();

				sprintf(strSource,"%s/(expansion)%s.txt",strCheckFolder,strName);
				sprintf(strDest,"localize/%s/%s/(expansion)%s.txt",m_strLocalizeFolder,strCheckResultFolder,strName);
				CopyFile(strSource,strDest,FALSE);
			}

			SetLogFolder();
			Draw();
			Sleep(1);
		}
	}

	SetLogFolder();

	BOOL	bIsValidFile	=	FALSE;

	while(1)
	{
		char	strSrcFileName[512],strDestFileName[512];

		sprintf(strSrcFileName,"%s/%s/mainQuest.txt",_logFolder,strCheckFolder);

		if	(!uploadMainQuestText(&l_srcMainQuestText,strSrcFileName))
		{
			cMSG::Error("can not find follow main quest text file",strFileName);
			break;
		}

		sprintf(strDestFileName,"%s/localize/%s/%s/mainQuest.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		if	(!uploadMainQuestText(&l_destMainQuestText,strDestFileName))
		{
			sprintf(strDestFileName,"%s/localize/%s/%s/mainQuest.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);
			CopyFile(strSrcFileName,strDestFileName,FALSE);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/%s/mainQuest.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);
		file.Open(strFileName,"wb");

		for (i=0;i<c_iMaxChapterCount;i++)
		{
			cMainQuestChapterText	*lpSrcChapter	=	&l_srcMainQuestText.m_aChapterList[i];
			cMainQuestChapterText	*lpDestChapter	=	&l_destMainQuestText.m_aChapterList[i];

			BOOL	bIsUpdate	=	FALSE;

			if	(lpSrcChapter->m_iNameCheckSum		==	0x12345678)
				continue;

			if	(lpSrcChapter->m_iNameCheckSum	!=	lpDestChapter->m_iNameCheckSum	)
			{
				file.writeStreamCR	("#mainQuestName	[%d] %d \"%s\"",i,GetTextCheckSum(lpSrcChapter->m_strName),lpSrcChapter->m_strName);
				bIsUpdate	=	TRUE;
			}

			if	(lpSrcChapter->m_iCommentCheckSum	!=	lpDestChapter->m_iCommentCheckSum	)
			{
				file.writeStreamCR	("#mainQuestComment	[%d] %d \"%s\"",i,GetTextCheckSum(lpSrcChapter->m_strComment),lpSrcChapter->m_strComment);
				bIsUpdate	=	TRUE;
			}
			if	(lpSrcChapter->m_iEpilogCommentCheckSum	!=	lpDestChapter->m_iEpilogCommentCheckSum	)
			{
				file.writeStreamCR	("#mainQuestEpilogComment	[%d] %d \"%s\"",i,GetTextCheckSum(lpSrcChapter->m_strEpilogComment),lpSrcChapter->m_strEpilogComment);
				bIsUpdate	=	TRUE;
			}

			for	(int iHighQuest=0;iHighQuest<c_iMaxHighQuestCount;iHighQuest++)
			{
				if	(lpSrcChapter->m_aiHiQuestNameChecksum[iHighQuest]	!=	lpDestChapter->m_aiHiQuestNameChecksum[iHighQuest]	)
				{
					file.writeStreamCR	("#highQuestName	[%d,%d] %d \"%s\"",i,iHighQuest,GetTextCheckSum(lpSrcChapter->m_astrHighQuest[iHighQuest]),lpSrcChapter->m_astrHighQuest[iHighQuest]);

					bIsUpdate	=	TRUE;
				}
			}

			if	(bIsUpdate)
				file.writeStreamCR("  ");
		}

		bIsValidFile	=	FALSE;

		if	(file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if	(bIsValidFile)
			DeleteFile(strFileName);

		break;
	}

	while(1)
	{
		char	strSrcFileName[512],strDestFileName[512];

		sprintf(strSrcFileName,"%s/%s/project.txt",_logFolder,strCheckFolder);
		if	(!uploadProjectKarmaText(&l_srcProjectKarma,strSrcFileName))
		{
			cMSG::Error("can not find follow project text file",strSrcFileName);
			break;
		}

		sprintf(strDestFileName,"%s/localize/%s/%s/project.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		if	(!uploadProjectKarmaText(&l_destProjectKarma,strDestFileName))
		{
			sprintf(strDestFileName,"%s/localize/%s/%s/project.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);
			CopyFile(strSrcFileName,strDestFileName,FALSE);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/%s/project.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);
		file.Open(strFileName,"wb");

		{
			for	(int iKarma=0;iKarma<1024;iKarma++)
			{
				CKarmaText	*lpSrcKT	=	&l_srcProjectKarma.m_aMD_Karma[iKarma];
				CKarmaText	*lpDestKT	=	&l_destProjectKarma.m_aMD_Karma[iKarma];

				if	(lpSrcKT->m_iCheckSum	==	0x12345678)
					continue;

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKT->m_apReaction[iReaction];
					CReactionText	*lpDestReaction	=	lpDestKT->m_apReaction[iReaction];

					if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
						continue;

					if	(lpDestReaction && lpSrcReaction->m_iCheckSum	==	lpDestReaction->m_iCheckSum)
						continue;

					file.writeStreamCR("#md_karma_reaction	[%d] %d,%d \"%s\"\n",GetTextCheckSum(lpSrcReaction->m_string.String),lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
				}
			}
		}
		{
			for	(int iKarma=0;iKarma<1024;iKarma++)
			{
				CKarmaText	*lpSrcKT	=	&l_srcProjectKarma.m_aMG_Karma[iKarma];
				CKarmaText	*lpDestKT	=	&l_destProjectKarma.m_aMG_Karma[iKarma];

				if	(lpSrcKT->m_iCheckSum	==	0x12345678)
					continue;

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKT->m_apReaction[iReaction];
					CReactionText	*lpDestReaction	=	lpDestKT->m_apReaction[iReaction];

					if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
						continue;

					if	(lpDestReaction && lpSrcReaction->m_iCheckSum	==	lpDestReaction->m_iCheckSum)
						continue;

					file.writeStreamCR("#mg_karma_reaction	[%d] %d,%d \"%s\"\n",GetTextCheckSum(lpSrcReaction->m_string.String),lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
				}
			}
		}

		bIsValidFile	=	FALSE;

		if	(file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if	(bIsValidFile)
			DeleteFile(strFileName);

		break;
	}

	while(1)
	{
		char	strSrcFileName[512],strDestFileName[512];

		sprintf(strSrcFileName,"%s/%s/book.txt",_logFolder,strCheckFolder);

		if	(!uploadBookText(&l_srcBookText,strSrcFileName))
		{
			cMSG::Error("can not find follow main book text file",strFileName);
			break;
		}

		sprintf(strDestFileName,"%s/localize/%s/%s/book.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		if	(!uploadBookText(&l_destBookText,strDestFileName))
		{
			sprintf(strDestFileName,"%s/localize/%s/%s/book.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);
			CopyFile(strSrcFileName,strDestFileName,FALSE);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/%s/book.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);
		file.Open(strFileName,"wb");

		for (int iBook=0;iBook<c_iBookCount;iBook++)
		{
			cBookText	*lpSrcBook,*lpDestBook;

			lpSrcBook	=	&l_srcBookText.m_aBook[iBook];
			lpDestBook	=	&l_destBookText.m_aBook[iBook];

			BOOL	bIsUpdate	=	FALSE;

			if	(lpSrcBook->m_iCheckSum	==	0x12345678)
				continue;

			if	(lpSrcBook->m_iCheckSum	!=	lpDestBook->m_iCheckSum	)
			{
				file.writeStreamCR	("#bookName	[%d] %d \"%s\"",iBook,GetTextCheckSum(lpSrcBook->m_strBookName),lpSrcBook->m_strBookName);

				bIsUpdate	=	TRUE;
			}

			if	(lpSrcBook->m_iCommentCheckSum	!=	lpDestBook->m_iCommentCheckSum	)
			{
				file.writeStreamCR	("#bookItemComment	[%d] %d \"%s\"",iBook,GetTextCheckSum(lpSrcBook->m_strItemComment.String),lpSrcBook->m_strItemComment.String);
				bIsUpdate	=	TRUE;
			}

			for (int iCategory=0;iCategory<c_iMaxBookCategoryCount;iCategory++)
			{
				cBookCategoryText	*lpSrcCategory,*lpDestCategory;

				lpSrcCategory	=	&lpSrcBook->m_aCategory[iCategory];
				lpDestCategory	=	&lpDestBook->m_aCategory[iCategory];

				if	(lpSrcCategory->m_iCategoryCheckSum	==	0x12345678)
					continue;

				if	(lpSrcCategory->m_iCategoryCheckSum	!=	lpDestCategory->m_iCategoryCheckSum)
				{
					file.writeStreamCR	("#bookCategory	[%d,%d] %d \"%s\"",iBook,iCategory,GetTextCheckSum(lpSrcCategory->m_strCategoryName),lpSrcCategory->m_strCategoryName);

					bIsUpdate	=	TRUE;
				}

				for (int iPage=0;iPage<c_iMaxBookPageCount;iPage++)
				{
					cBookPageText	*lpSrcPage,*lpDestPage;

					lpSrcPage	=	&lpSrcCategory->m_aPage[iPage];
					lpDestPage	=	&lpDestCategory->m_aPage[iPage];

					if	(lpSrcPage->m_iCheckSum	==	0x12345678)
						continue;
					if	(lpSrcPage->m_iCheckSum	!=	lpDestPage->m_iCheckSum)
					{
						file.writeStreamCR	("#bookPage	[%d,%d,%d] %d \"%s\"",iBook,iCategory,iPage,GetTextCheckSum(lpSrcPage->m_strText.String),lpSrcPage->m_strText.String);
						bIsUpdate	=	TRUE;
					}
				}
			}

			if	(bIsUpdate)
				file.writeStreamCR("  ");
		}

		bIsValidFile	=	FALSE;

		if	(file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if	(bIsValidFile)
			DeleteFile(strFileName);

		break;
	}


//	퀘스트 
	while(1)
	{
		int			iGroup;

		sprintf(strFileName,"%s/%s/quest.txt",_logFolder,strCheckFolder);
		if (!uploadQuestText(&l_srcQuest,strFileName))
		{
			cMSG::Error("can not find follow quest text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/%s/quest.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		uploadQuestText(&l_destQuest,strFileName);

		sprintf(strFileName,"%s/localize/%s/%s/quest.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);

		file.Open(strFileName,"wb");

		for (iGroup=0;iGroup<2;iGroup++)
		{
			CQuestTextItem	*lpSrcQuestGroup,*lpDestQuestGroup;
			char			*lpstrHeader;

			if (iGroup	==	0)
			{
				lpSrcQuestGroup	=	l_srcQuest.m_aQuest;
				lpDestQuestGroup=	l_destQuest.m_aQuest;
				lpstrHeader		=	"quest";
			}
			else
			{
				lpSrcQuestGroup	=	l_srcQuest.m_aArbeit;
				lpDestQuestGroup=	l_destQuest.m_aArbeit;
				lpstrHeader		=	"arbeit";
			}

			for (int iQuest=0;iQuest<dQUEST_COUNT;iQuest++)
			{
				CQuestTextItem	*lpSrcQuest,*lpDestQuest;

				lpSrcQuest	=	&lpSrcQuestGroup[iQuest];
				lpDestQuest	=	&lpDestQuestGroup[iQuest];

				if	(lpSrcQuest->isDifferentCheckSum(lpDestQuest))
				{
					file.writeStreamCR	("#%s [%d] %d \"%s\"",lpstrHeader,GetTextCheckSum(lpSrcQuest->m_strName),iQuest,lpSrcQuest->m_strName);
					file.writeStreamCR	("    #client [%d] \"%s\"",GetTextCheckSum(lpSrcQuest->m_strQuestClientInfo),lpSrcQuest->m_strQuestClientInfo);
					file.writeStreamCR	("    #beginClient [%d] \"%s\"",GetTextCheckSum(lpSrcQuest->m_strBeginNPCName),lpSrcQuest->m_strBeginNPCName);
					file.writeStream	("    #value  [%d] \"",GetTextCheckSum(lpSrcQuest->m_strQuestValueComment));


					file.Write			(lpSrcQuest->m_strQuestValueComment,strlen(lpSrcQuest->m_strQuestValueComment));
					file.writeStreamCR	("\"");

					for (int j=0;j<dQUEST_STATUS_COUNT;j++)
					{
						if	(lpSrcQuest->m_iExplainCheckSum[j]	==	0x12345678)
							continue;

						int	iCheckSum	=	GetTextCheckSum(lpSrcQuest->m_astrExplain[j]);

						if	(iCheckSum)
							file.writeStreamCR("    #explain [%d] %d \"%s\"",iCheckSum,j,lpSrcQuest->m_astrExplain[j]);
					}

					file.writeStreamCR(" ");
				}
			}
		}

		bIsValidFile	=	FALSE;

		if (file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if	(bIsValidFile)
			DeleteFile(strFileName);

		break;
	}

//	아이템
	while(1)
	{
		sprintf(strFileName,"%s/%s/_L_item.txt",_logFolder,strCheckFolder);

		if (!upload2KindText(&l_srcItem,strFileName))
		{
			cMSG::Error("can not find follow item text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/%s/_L_item.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		upload2KindText(&l_destItem,strFileName);

		sprintf(strFileName,"%s/localize/%s/%s/_L_item.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);

		int		iItem;

		file.Open(strFileName,"wb");

		for (iItem=0;iItem<dBASIC_ITEM_COUNT;iItem++)
		{
			C2KindTextItem	*lpSrcItem	=	&l_srcItem.m_aItems[iItem];
			C2KindTextItem	*lpDestItem	=	&l_destItem.m_aItems[iItem];

			if (lpSrcItem->isDifferentCheckSum(lpDestItem))
				file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
					iItem,GetTextCheckSum(lpSrcItem->m_strName.String),GetTextCheckSum(lpSrcItem->m_strExplain.String),
					lpSrcItem->m_strName.String,lpSrcItem->m_strExplain.String);
		}


		bIsValidFile	=	FALSE;

		if (file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if (bIsValidFile)
			DeleteFile(strFileName);

		sprintf(strFileName,"%s/%s/_L_itemPrefix.txt",_logFolder,strCheckFolder);
		if (!upload2KindText(&l_srcItem,strFileName))
		{
			cMSG::Error("can not find follow item prefix text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/%s/_L_itemPrefix.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		upload2KindText(&l_destItem,strFileName);

		sprintf(strFileName,"localize/%s/%s/_L_itemPrefix.txt",m_strLocalizeFolder,strCheckResultFolder);
		file.Open(strFileName,"wb");

		for (iItem=0;iItem<dMAX_ITEM_PREFIX_COUNT;iItem++)
		{
			C2KindTextItem	*lpSrcItem	=	&l_srcItem.m_aItems[iItem];
			C2KindTextItem	*lpDestItem	=	&l_destItem.m_aItems[iItem];

			if (lpSrcItem->m_iNameCheckSum	==	0x12345678)
				continue;

			if (lpSrcItem->isDifferentCheckSum(lpDestItem))
				file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
					iItem,GetTextCheckSum(lpSrcItem->m_strName.String),GetTextCheckSum(lpSrcItem->m_strExplain.String),
					lpSrcItem->m_strName.String,lpSrcItem->m_strExplain.String);
		}

		bIsValidFile	=	FALSE;

		if (file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if (bIsValidFile)
			DeleteFile(strFileName);

		break;
	}

//	세트 아이템
	while(1)
	{
		sprintf(strFileName,"%s/%s/_L_item_set.txt",_logFolder,strCheckFolder);

		if	(!uploadSetItemKindText(&l_srcItemSet,strFileName))
		{
			cMSG::Error("can not find follow item set text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/%s/_L_item_set.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		uploadSetItemKindText(&l_destItemSet,strFileName);

		sprintf(strFileName,"%s/localize/%s/%s/_L_item_set.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);

		int		iItemSet;

		file.Open(strFileName,"wb");

		for (iItemSet=0;iItemSet<dMAX_SET_ITEM_GROUP_COUNT;iItemSet++)
		{
			cItemSetTextItem	*lpSrcItemSet	=	&l_srcItemSet.m_aSetItem[iItemSet];
			cItemSetTextItem	*lpDestItemSet	=	&l_destItemSet.m_aSetItem[iItemSet];

			if	(lpSrcItemSet->m_iNameCheckSum	==	0x12345678)
				continue;

			if	(lpSrcItemSet->m_iNameCheckSum	!=	lpDestItemSet->m_iNameCheckSum)
				file.writeStreamCR("\nset %3d [%d] \"%s\"",iItemSet,GetTextCheckSum(lpSrcItemSet->m_strName.String),lpSrcItemSet->m_strName.String);

			if	(lpSrcItemSet->m_iExplainCheckSum!=	lpDestItemSet->m_iExplainCheckSum)
				file.writeStreamCR("set_explain %3d [%d] \"%s\"",iItemSet,GetTextCheckSum(lpSrcItemSet->m_strExplain.String),lpSrcItemSet->m_strExplain.String);

			for	(int iSlot=0;iSlot<dMAX_ITEM_COUNT_IN_SET;iSlot++)
			{
				if	(lpSrcItemSet->m_aData[iSlot].m_iNameCheckSum	==	0x12345678)
					break;

				if	(lpSrcItemSet->m_aData[iSlot].m_iNameCheckSum	!=	lpDestItemSet->m_aData[iSlot].m_iNameCheckSum)
					file.writeStreamCR("set_item_name %d,%d [%d] \"%s\"",iItemSet,iSlot,GetTextCheckSum(lpSrcItemSet->m_aData[iSlot].m_strName.String),lpSrcItemSet->m_aData[iSlot].m_strName.String);

				if	(lpSrcItemSet->m_aData[iSlot].m_iExplainCheckSum!=	lpDestItemSet->m_aData[iSlot].m_iExplainCheckSum)
					file.writeStreamCR("set_item_explain %d,%d [%d] \"%s\"",iItemSet,iSlot,GetTextCheckSum(lpSrcItemSet->m_aData[iSlot].m_strExplain.String),lpSrcItemSet->m_aData[iSlot].m_strExplain.String);
			}
		}

		bIsValidFile	=	FALSE;

		if (file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if (bIsValidFile)
			DeleteFile(strFileName);

		break;
	}
	
	while(1)
	{
		sprintf(strFileName,"%s/%s/_L_skill.txt",_logFolder,strCheckFolder);
		if (!uploadSkillText(&l_srcSkill,strFileName))
		{
			cMSG::Error("can not find follow skill text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/%s/_L_skill.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		uploadSkillText(&l_destSkill,strFileName);

		sprintf(strFileName,"%s/localize/%s/%s/_L_skill.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);

		int		iSkill;

		file.Open(strFileName,"wb");

		for (iSkill=0;iSkill<dMAX_SKILL;iSkill++)
		{
			CSkillTextItem	*lpSrcSkill	=	&l_srcSkill.m_aSkill[iSkill];
			CSkillTextItem	*lpDestSkill=	&l_destSkill.m_aSkill[iSkill];

			if (lpSrcSkill->m_iNameCheckSum	==	0x12345678)
				continue;

			if (lpSrcSkill->isDifferentCheckSum(lpDestSkill))
				file.writeStreamCR("%3d  [%d,%d,%d]\n     \"%s\"\n     \"%s\"\n     \"%s\"\n",
					iSkill,GetTextCheckSum(lpSrcSkill->m_strName.String),GetTextCheckSum(lpSrcSkill->m_strPowerUp.String),GetTextCheckSum(lpSrcSkill->m_strExplain.String),
					lpSrcSkill->m_strName.String,lpSrcSkill->m_strPowerUp.String,lpSrcSkill->m_strExplain.String);
		}

		bIsValidFile	=	FALSE;

		if (file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if (bIsValidFile)
			DeleteFile(strFileName);

		break;
	}

	while(1)
	{
		sprintf(strFileName,"%s/%s/_L_title.txt",_logFolder,strCheckFolder);
		if (!upload2KindText(&l_srcItem,strFileName))
		{
			cMSG::Error("can not find follow title text file",strFileName);
			break;
		}
		sprintf(strFileName,"%s/localize/%s/%s/_L_title.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		upload2KindText(&l_destItem,strFileName);

		sprintf(strFileName,"%s/localize/%s/%s/_L_title.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);

		int		iTitle;

		file.Open(strFileName,"wb");

		for (iTitle=0;iTitle<256;iTitle++)
		{
			C2KindTextItem	*lpSrcTitle=	&l_srcItem.m_aItems[iTitle];
			C2KindTextItem	*lpDestTitle=	&l_destItem.m_aItems[iTitle];

			if (lpSrcTitle->m_iNameCheckSum	==	0x12345678)
				continue;

			if (lpSrcTitle->isDifferentCheckSum(lpDestTitle))
				file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
					iTitle,GetTextCheckSum(lpSrcTitle->m_strName.String),GetTextCheckSum(lpSrcTitle->m_strExplain.String),
					lpSrcTitle->m_strName.String,lpSrcTitle->m_strExplain.String);
		}

		bIsValidFile	=	FALSE;

		if (file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if (bIsValidFile)
			DeleteFile(strFileName);

		break;
	}

	while(1)
	{
		sprintf(strFileName,"%s/%s/_L_guildSkill.txt",_logFolder,strCheckFolder);
		if (!upload2KindText(&l_srcItem,strFileName))
		{
			cMSG::Error("can not find follow guild skill text file",strFileName);
			break;
		}
		sprintf(strFileName,"%s/localize/%s/%s/_L_guildSkill.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		upload2KindText(&l_destItem,strFileName);

		sprintf(strFileName,"%s/localize/%s/%s/_L_guildSkill.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);

		int		iSkill;

		file.Open(strFileName,"wb");

		for (iSkill=0;iSkill<256;iSkill++)
		{
			C2KindTextItem	*lpSrcSkill=	&l_srcItem.m_aItems[iSkill];
			C2KindTextItem	*lpDestSkill=	&l_destItem.m_aItems[iSkill];

			if (lpSrcSkill->m_iNameCheckSum	==	0x12345678)
				continue;

			if (lpSrcSkill->isDifferentCheckSum(lpDestSkill))
				file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
					iSkill,GetTextCheckSum(lpSrcSkill->m_strName.String),GetTextCheckSum(lpSrcSkill->m_strExplain.String),
					lpSrcSkill->m_strName.String,lpSrcSkill->m_strExplain.String);
		}

		bIsValidFile	=	FALSE;

		if (file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if (bIsValidFile)
			DeleteFile(strFileName);

		break;
	}

	while(1)
	{
		sprintf(strFileName,"%s/%s/_L_job.txt",_logFolder,strCheckFolder);
		if (!upload2KindText(&l_srcItem,strFileName,TRUE))
		{
			cMSG::Error("can not find follow job text file",strFileName);
			break;
		}
		sprintf(strFileName,"%s/localize/%s/%s/_L_job.txt",_logFolder,m_strLocalizeFolder,strSourceFolder);
		upload2KindText(&l_destItem,strFileName,TRUE);

		sprintf(strFileName,"%s/localize/%s/%s/_L_job.txt",_logFolder,m_strLocalizeFolder,strCheckResultFolder);

		int		iJob;

		file.Open(strFileName,"wb");

		for (iJob=0;iJob<dMAX_JOB_COUNT;iJob++)
		{
			C2KindTextItem	*lpSrcSkill=	&l_srcItem.m_aItems[iJob];
			C2KindTextItem	*lpDestSkill=	&l_destItem.m_aItems[iJob];

			if (lpSrcSkill->m_iNameCheckSum	==	0x12345678)
				continue;

			if (lpSrcSkill->isDifferentCheckSum(lpDestSkill))
				file.writeStreamCR("%3d  [%d,%d] \"%s\" \" \"",
					iJob,GetTextCheckSum(lpSrcSkill->m_strName.String),GetTextCheckSum(" "),
					lpSrcSkill->m_strName.String);
		}

		bIsValidFile	=	FALSE;

		if (file.Locate()	<=	4)
			bIsValidFile	=	TRUE;

		file.Close();

		if (bIsValidFile)
			DeleteFile(strFileName);

		break;
	}


	char	*astrText[]	=	
	{
		"itemExplain.txt",
		"itemPrefix.txt",
		"title.txt",
		"common.txt",
		"npcName.txt",
		"_L_secret_dungeon.txt",
		"_L_PremiumItem.txt",
		"GreateGodGrace.txt",
		"end"
	};


	for (int iTF=0;;iTF++)
	{
		if (stricmp(astrText[iTF],"end")	==	0)
			break;

		while (1)
		{
			sprintf(strFileName,"%s/%s/%s",_logFolder,strCheckFolder,astrText[iTF]);
			if (!upload1KindText(&l_src1KindText,strFileName))
			{
				cMSG::Error("can not find follow text file",strFileName);
				break;
			}
			sprintf(strFileName,"%s/localize/%s/%s/%s",_logFolder,m_strLocalizeFolder,strSourceFolder,astrText[iTF]);
			upload1KindText(&l_dest1KindText,strFileName);

			sprintf(strFileName,"%s/localize/%s/%s/%s",_logFolder,m_strLocalizeFolder,strCheckResultFolder,astrText[iTF]);

			int		iJob;

			file.Open(strFileName,"wb");

			for (iJob=0;iJob<dMAX_COMPARE_TEXT_COUNT;iJob++)
			{
				C1KindTextItem	*lpSrc	=	&l_src1KindText.m_aItems[iJob];
				C1KindTextItem	*lpDest	=	&l_dest1KindText.m_aItems[iJob];

				if (lpSrc->m_iCheckSum	==	0x12345678)
					continue;

				if (lpSrc->isDifferentCheckSum(lpDest))
					file.writeStreamCR("%3d  [%d] \"%s\"",iJob,GetTextCheckSum(lpSrc->m_strText.String),lpSrc->m_strText.String);
			}

			bIsValidFile	=	FALSE;

			if (file.Locate()	<=	4)
				bIsValidFile	=	TRUE;

			file.Close();

			if (bIsValidFile)
				DeleteFile(strFileName);

			break;
		}

	}


	cMSG::Put("compare all game text data","complete");

	return	TRUE;
}
