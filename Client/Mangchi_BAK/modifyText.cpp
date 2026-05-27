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
cMAIN::modifyText(HWND hwnd)
{
	if (!selectLocalizeFolder(hwnd))
		return	FALSE;

	char		strFileName[512];
	char		strMapFolder[512];
	int			i;
	cDECODER	decoder;
	cFILE		file;

	SetLogFolder();

	sprintf(strMapFolder,"%s/localize/%s/data/scenario/red stone/map",_logFolder,m_strLocalizeFolder);

	if	(!IsFolder(strMapFolder))
		return	ERRMSG("can not find follow folder",strMapFolder);
	if	(!IsFolder(_ms("localize/%s/old",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/old",_logFolder,m_strLocalizeFolder);
	if	(!IsFolder(_ms("localize/%s/_modify result",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/_modify result",_logFolder,m_strLocalizeFolder);
	if	(!IsFolder(_ms("localize/%s/_want modify",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/_want modify",_logFolder,m_strLocalizeFolder);

	if (cMSG::YESNO("modify all maps text data","do you want continue this work?")	==	IDYES)
	{
		SetCurrentDirectory(strMapFolder);

		cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

		SetLogFolder();

		if (!lpFiles)
			return	ERRMSG("error!!","can not find map list!");

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);
			SetCurrentDirectory(strMapFolder);
			strcpy(strFileName,_exportFileName(lpFile->String));

			if	(cMAP::ModifyText(strFileName)	==	FALSE)
			{
				char	strSource[512],strDest[512];

				SetLogFolder();

				sprintf(strSource,"localize/%s/old/%s.txt",m_strLocalizeFolder,strFileName);

				if(IsFile(strSource) == FALSE)
					sprintf(strSource,"localize/%s/_want modify/%s.txt",m_strLocalizeFolder,strFileName);

				if	(IsFile(strSource))
				{
					sprintf(strDest,"localize/%s/_modify result/%s.txt",m_strLocalizeFolder,strFileName);

					cMAP::UploadTextForCompare(strSource,NULL);//	파일 복사 하기 전에 텍스트 검증 한다.

					CopyFile(strSource,strDest,FALSE);
				}
				else
				{
					cMSG::Put("이 파일이 없네..",strFileName);
				}
			}

			SetLogFolder();
			Draw();
			Sleep(1);
		}

		SetLogFolder();
		KILL(lpFiles);

		SetCurrentDirectory(strMapFolder);
		SetCurrentDirectory("expansion");
		lpFiles	=	cFOLDER::GetFiles("*.rmd");

		SetLogFolder();

		if (!lpFiles)
			return	ERRMSG("error!!","can not find map list!");

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);
			SetCurrentDirectory(strMapFolder);
			SetCurrentDirectory("expansion");
			strcpy(strFileName,_exportFileName(lpFile->String));

			if (cMAP::ModifyText(strFileName,TRUE)	==	FALSE)
			{
				char	strSource[512],strDest[512];

				SetLogFolder();

				sprintf(strSource,"localize/%s/old/(expansion)%s.txt",m_strLocalizeFolder,strFileName);

				if	(IsFile(strSource) == FALSE)
					sprintf(strSource,"localize/%s/_want modify/(expansion)%s.txt",m_strLocalizeFolder,strFileName);

				if	(IsFile(strSource) == FALSE)
				{
					cMSG::Put("얼래? 파일이 없네;;",strFileName);
				}
				else
				{
					cMAP::UploadTextForCompare(strSource,NULL);//	파일 복사 하기 전에 텍스트 검증 한다.
					sprintf(strDest,"localize/%s/_modify result/(expansion)%s.txt",m_strLocalizeFolder,strFileName);
					CopyFile(strSource,strDest,FALSE);
				}
			}

			SetLogFolder();
			Draw();
			Sleep(1);
		}

		SetLogFolder();
		KILL(lpFiles);
	}

	SetLogFolder();

//	프로젝트 카르마
	while(1)
	{
		sprintf(strFileName,"localize/%s/old/project.txt",m_strLocalizeFolder);

		if	(uploadProjectKarmaText(&l_srcProjectKarma,strFileName)	==	FALSE)
		{
			char	strModifyFileName[512];
			sprintf(strModifyFileName,"localize/%s/_want modify/project.txt",m_strLocalizeFolder);
			sprintf(strFileName,"localize/%s/_modify result/project.txt",m_strLocalizeFolder);
			CopyFile(strModifyFileName,strFileName,FALSE);

//			cMSG::Error("can not find follow project text file",strFileName);
			break;
		}

		sprintf(strFileName,"localize/%s/_want modify/project.txt",m_strLocalizeFolder);
		uploadProjectKarmaText(&l_destProjectKarma,strFileName);

		sprintf(strFileName,"localize/%s/_modify result/project.txt",m_strLocalizeFolder);

		file.Open(strFileName,"wb");

		{
			for	(int iKarma=0;iKarma<1024;iKarma++)
			{
				CKarmaText	*lpSrcKarma		=	&l_srcProjectKarma.m_aMD_Karma[iKarma];
				CKarmaText	*lpDestKarma	=	&l_destProjectKarma.m_aMD_Karma[iKarma];

				if	(lpSrcKarma->m_iCheckSum	==	0x12345678	&&	lpDestKarma->m_iCheckSum	==	0x12345678)
					continue;

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
					CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

					if	(!lpSrcReaction && !lpDestReaction)
						continue;

					if	(lpDestReaction && lpDestReaction->m_iCheckSum	!=	0x12345678)
						lpSrcReaction	=	lpDestReaction;

					file.writeStreamCR("#md_karma_reaction [%d] %d,%d \"%s\"\n",lpSrcReaction->m_iCheckSum,lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
				}
			}
		}

		{
			for	(int iKarma=0;iKarma<1024;iKarma++)
			{
				CKarmaText	*lpSrcKarma		=	&l_srcProjectKarma.m_aMG_Karma[iKarma];
				CKarmaText	*lpDestKarma	=	&l_destProjectKarma.m_aMG_Karma[iKarma];

				if	(lpSrcKarma->m_iCheckSum	==	0x12345678	&&	lpDestKarma->m_iCheckSum	==	0x12345678)
					continue;

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
					CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

					if	(!lpSrcReaction && !lpDestReaction)
						continue;

					if	(lpDestReaction && lpDestReaction->m_iCheckSum	!=	0x12345678)
						lpSrcReaction	=	lpDestReaction;

					file.writeStreamCR("#mg_karma_reaction [%d] %d,%d \"%s\"\n",lpSrcReaction->m_iCheckSum,lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
				}
			}
		}

		file.Close();

		break;
	}

//	퀘스트 
	while(1)
	{
		int			iGroup;

		sprintf(strFileName,"localize/%s/old/quest.txt",m_strLocalizeFolder);
		if	(uploadQuestText(&l_srcQuest,strFileName)	==	FALSE)
		{
			cMSG::Error("can not find follow quest text file",strFileName);
			break;
		}
		sprintf(strFileName,"localize/%s/_want modify/quest.txt",m_strLocalizeFolder);
		uploadQuestText(&l_destQuest,strFileName);

		sprintf(strFileName,"localize/%s/_modify result/quest.txt",m_strLocalizeFolder);

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

				if (lpDestQuest->m_iNameCheckSum	!=	0x12345678)
					lpSrcQuest	=	lpDestQuest;

				if (lpSrcQuest->m_iNameCheckSum		==	0x12345678)
					continue;

				file.writeStreamCR	("#%s [%d] %d \"%s\"",lpstrHeader,lpSrcQuest->m_iNameCheckSum,iQuest,lpSrcQuest->m_strName);
				file.writeStreamCR	("    #client [%d] \"%s\"",lpSrcQuest->m_iClientCheckSum,lpSrcQuest->m_strQuestClientInfo);
				file.writeStreamCR	("    #beginClient [%d] \"%s\"",lpSrcQuest->m_iBeginClientCheckSum,lpSrcQuest->m_strBeginNPCName);
				file.writeStream	("    #value  [%d] \"",lpSrcQuest->m_iInfoCheckSum);

				file.Write			(lpSrcQuest->m_strQuestValueComment,strlen(lpSrcQuest->m_strQuestValueComment));
				file.writeStreamCR	("\"");

				for (int j=0;j<dQUEST_STATUS_COUNT;j++)
				{
					if	(iGroup	==	1	&&	iQuest	==	218	&&	j	==	4)
						j	=	4;

					if (lpSrcQuest->m_iExplainCheckSum[j]	==	0x12345678)
						continue;

					int	iCheckSum	=	lpSrcQuest->m_iExplainCheckSum[j];

					if (iCheckSum)
						file.writeStreamCR("    #explain [%d] %d \"%s\"",iCheckSum,j,lpSrcQuest->m_astrExplain[j]);
				}

				file.writeStreamCR(" ");
			}
		}

		file.Close();

		break;
	}

//	책 
	while(1)
	{
		sprintf(strFileName,"localize/%s/old/book.txt",m_strLocalizeFolder);

		if	(uploadBookText(&l_srcBookText,strFileName)	==	FALSE)
		{
			char	strSrcFileName[512],strDestFileName[512];

			sprintf(strDestFileName	,"localize/%s/_modify result/book.txt",m_strLocalizeFolder);
			sprintf(strSrcFileName	,"localize/%s/_want modify/book.txt",m_strLocalizeFolder);
			CopyFile(strSrcFileName	,strDestFileName,FALSE);

			break;
		}

		sprintf(strFileName,"localize/%s/_want modify/book.txt",m_strLocalizeFolder);
		uploadBookText(&l_destBookText,strFileName);

		sprintf(strFileName,"localize/%s/_modify result/book.txt",m_strLocalizeFolder);

		file.Open(strFileName,"wb");

		for (int iBook=0;iBook<c_iBookCount;iBook++)
		{
			cBookInfo	*lpBook	=	g_book.getBook(iBook);
			
			if	(!lpBook)
				continue;

			cBookText	*lpSrcBook,*lpDestBook;

			lpSrcBook	=	&l_srcBookText.m_aBook[iBook];
			lpDestBook	=	&l_destBookText.m_aBook[iBook];

			if	(lpDestBook->m_iCheckSum	!=	0x12345678)
				file.writeStreamCR	("#bookName	[%d] %d \"%s\"",iBook,lpDestBook->m_iCheckSum,lpDestBook->m_strBookName);
			else
			if	(lpSrcBook->m_iCheckSum		!=	0x12345678)
				file.writeStreamCR	("#bookName	[%d] %d \"%s\"",iBook,lpSrcBook->m_iCheckSum,lpSrcBook->m_strBookName);
			else
				continue;

			if	(lpDestBook->m_iCommentCheckSum	!=	0x12345678)
				file.writeStreamCR	("#bookItemComment	[%d] %d \"%s\"",iBook,lpDestBook->m_iCommentCheckSum,lpDestBook->m_strItemComment.String);
			else
			if	(lpSrcBook->m_iCommentCheckSum	!=	0x12345678)
				file.writeStreamCR	("#bookItemComment	[%d] %d \"%s\"",iBook,lpSrcBook->m_iCommentCheckSum,lpSrcBook->m_strItemComment.String);

			for (int iCategory=0;iCategory<lpBook->m_wCategoryCount;iCategory++)
			{
				cBookCategory		*lpCategory		=	g_book.getCategory(iBook,iCategory);
				cBookCategoryText	*lpSrcCategory	=	&lpSrcBook->m_aCategory[iCategory];
				cBookCategoryText	*lpDestCategory	=	&lpDestBook->m_aCategory[iCategory];

				if	(lpDestCategory->m_iCategoryCheckSum	!=	0x12345678)
					file.writeStreamCR	("#bookCategory	[%d,%d] %d \"%s\"",iBook,iCategory,lpDestCategory->m_iCategoryCheckSum,lpDestCategory->m_strCategoryName);
				else
				if	(lpSrcCategory->m_iCategoryCheckSum		!=	0x12345678)
					file.writeStreamCR	("#bookCategory	[%d,%d] %d \"%s\"",iBook,iCategory,lpSrcCategory->m_iCategoryCheckSum,lpSrcCategory->m_strCategoryName);

				for (int iPage=0;iPage<lpCategory->m_iPageCount;iPage++)
				{
					cBookPageInfo	*lpPage		=	g_book.getPage(iBook,iCategory,iPage);
					cBookPageText	*lpSrcPage	=	&lpSrcCategory->m_aPage[iPage];
					cBookPageText	*lpDestPage	=	&lpDestCategory->m_aPage[iPage];

					if	(lpDestPage->m_iCheckSum!=	0x12345678)
						file.writeStreamCR	("#bookPage	[%d,%d,%d] %d \"%s\"",iBook,iCategory,iPage,lpDestPage->m_iCheckSum,lpDestPage->m_strText.String);
					else
					if	(lpSrcPage->m_iCheckSum!=	0x12345678)
						file.writeStreamCR	("#bookPage	[%d,%d,%d] %d \"%s\"",iBook,iCategory,iPage,lpSrcPage->m_iCheckSum,lpSrcPage->m_strText.String);
				}
			}

			file.writeStreamCR(" ");
		}

		file.Close();

		break;
	}

//	메인 퀘스트 
	while(1)
	{
		sprintf(strFileName,"localize/%s/old/mainQuest.txt",m_strLocalizeFolder);

		if	(uploadMainQuestText(&l_srcMainQuestText,strFileName)	==	FALSE)
		{
			char	strSrcFileName[512],strDestFileName[512];

			sprintf(strDestFileName	,"localize/%s/_modify result/mainQuest.txt",m_strLocalizeFolder);
			sprintf(strSrcFileName	,"localize/%s/_want modify/mainQuest.txt",m_strLocalizeFolder);
			CopyFile(strSrcFileName	,strDestFileName,FALSE);

			break;
		}

		sprintf(strFileName,"localize/%s/_want modify/mainQuest.txt",m_strLocalizeFolder);
		uploadMainQuestText(&l_destMainQuestText,strFileName);

		sprintf(strFileName,"localize/%s/_modify result/mainQuest.txt",m_strLocalizeFolder);

		file.Open(strFileName,"wb");

		for (i=0;i<c_iMaxChapterCount;i++)
		{
			cMainQuestChapterText	*lpSrcChapter	=	&l_srcMainQuestText.m_aChapterList[i];
			cMainQuestChapterText	*lpDestChapter	=	&l_destMainQuestText.m_aChapterList[i];
			cChapterInfo			*lpChapterInfo	=	g_chapterManager.get(i);

			if	(lpDestChapter->m_iNameCheckSum		!=	0x12345678)
				file.writeStreamCR	("#mainQuestName	[%d] %d \"%s\"",i,lpDestChapter->m_iNameCheckSum,lpDestChapter->m_strName);
			else
			if	(lpSrcChapter->m_iNameCheckSum		!=	0x12345678)
				file.writeStreamCR	("#mainQuestName	[%d] %d \"%s\"",i,lpSrcChapter->m_iNameCheckSum,lpSrcChapter->m_strName);
			else
				continue;

			if	(lpDestChapter->m_iCommentCheckSum	!=	0x12345678)
				file.writeStreamCR	("#mainQuestComment	[%d] %d \"%s\"",i,lpDestChapter->m_iCommentCheckSum,lpDestChapter->m_strComment);
			else
			if	(lpSrcChapter->m_iCommentCheckSum	!=	0x12345678)
				file.writeStreamCR	("#mainQuestComment	[%d] %d \"%s\"",i,lpSrcChapter->m_iCommentCheckSum,lpSrcChapter->m_strComment);

			if	(lpDestChapter->m_iEpilogCommentCheckSum	!=	0x12345678)
				file.writeStreamCR	("#mainQuestEpilogComment	[%d] %d \"%s\"",i,lpDestChapter->m_iEpilogCommentCheckSum,lpDestChapter->m_strEpilogComment);
			else
			if	(lpSrcChapter->m_iEpilogCommentCheckSum	!=	0x12345678)
				file.writeStreamCR	("#mainQuestEpilogComment	[%d] %d \"%s\"",i,lpSrcChapter->m_iEpilogCommentCheckSum,lpSrcChapter->m_strEpilogComment);

			for	(int iHighQuest=0;iHighQuest<lpChapterInfo->m_iHighQuestCount;iHighQuest++)
			{
				if	(lpDestChapter->m_aiHiQuestNameChecksum[iHighQuest]	!=	0x12345678)
					file.writeStreamCR	("#highQuestName	[%d,%d] %d \"%s\"",i,iHighQuest,lpDestChapter->m_aiHiQuestNameChecksum[iHighQuest],lpDestChapter->m_astrHighQuest[iHighQuest]);
				else
				if	(lpSrcChapter->m_aiHiQuestNameChecksum[iHighQuest]	!=	0x12345678)
					file.writeStreamCR	("#highQuestName	[%d,%d] %d \"%s\"",i,iHighQuest,lpSrcChapter->m_aiHiQuestNameChecksum[iHighQuest],lpSrcChapter->m_astrHighQuest[iHighQuest]);
			}

			file.writeStreamCR(" ");
		}

		file.Close();

		break;
	}

	while(1)
	{
		sprintf(strFileName,"localize/%s/old/_L_item.txt",m_strLocalizeFolder);
		if (!upload2KindText(&l_srcItem,strFileName))
		{
			cMSG::Error("can not find follow item text file",strFileName);
			break;
		}
		sprintf(strFileName,"localize/%s/_want modify/_L_item.txt",m_strLocalizeFolder);
		upload2KindText(&l_destItem,strFileName);

		sprintf(strFileName,"localize/%s/_modify result/_L_item.txt",m_strLocalizeFolder);

		int		iItem;

		file.Open(strFileName,"wb");

		for (iItem=0;iItem<dBASIC_ITEM_COUNT;iItem++)
		{
			C2KindTextItem	*lpSrcItem	=	&l_srcItem.m_aItems[iItem];
			C2KindTextItem	*lpDestItem	=	&l_destItem.m_aItems[iItem];

			if (lpDestItem->m_iNameCheckSum	!=	0x12345678)
				lpSrcItem	=	lpDestItem;

			if (lpSrcItem->m_iNameCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
				iItem,lpSrcItem->m_iNameCheckSum,lpSrcItem->m_iExplainCheckSum,
				lpSrcItem->m_strName.String,lpSrcItem->m_strExplain.String);
		}

		file.Close();

		sprintf(strFileName,"localize/%s/old/_L_itemPrefix.txt",m_strLocalizeFolder);
		if (!upload2KindText(&l_srcItem,strFileName))
		{
			cMSG::Error("can not find follow item prefix text file",strFileName);
			break;
		}
		sprintf(strFileName,"localize/%s/_want modify/_L_itemPrefix.txt",m_strLocalizeFolder);
		upload2KindText(&l_destItem,strFileName);

		file.Open(_ms("localize/%s/_modify result/_L_itemPrefix.txt",m_strLocalizeFolder),"wb");

		for (iItem=0;iItem<dMAX_ITEM_PREFIX_COUNT;iItem++)
		{
			C2KindTextItem	*lpSrcItem	=	&l_srcItem.m_aItems[iItem];
			C2KindTextItem	*lpDestItem	=	&l_destItem.m_aItems[iItem];

			if (lpDestItem->m_iNameCheckSum	!=	0x12345678)
				lpSrcItem	=	lpDestItem;

			if (lpSrcItem->m_iNameCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
				iItem,lpSrcItem->m_iNameCheckSum,lpSrcItem->m_iExplainCheckSum,
				lpSrcItem->m_strName.String,lpSrcItem->m_strExplain.String);
		}

		file.Close();

		break;
	}

	while (1)
	{
		sprintf(strFileName,"%s/localize/%s/old/_L_skill.txt",_logFolder,m_strLocalizeFolder);
		if (!uploadSkillText(&l_srcSkill,strFileName))
		{
			cMSG::Error("can not find follow skill text file",strFileName);
			break;
		}
		sprintf(strFileName,"%s/localize/%s/_want modify/_L_skill.txt",_logFolder,m_strLocalizeFolder);
		uploadSkillText(&l_destSkill,strFileName);

		sprintf(strFileName,"%s/localize/%s/_modify result/_L_skill.txt",_logFolder,m_strLocalizeFolder);

		int		iSkill;

		file.Open(strFileName,"wb");

		for (iSkill=0;iSkill<dMAX_SKILL;iSkill++)
		{
			CSkillTextItem	*lpSrcSkill	=	&l_srcSkill.m_aSkill[iSkill];
			CSkillTextItem	*lpDestSkill=	&l_destSkill.m_aSkill[iSkill];

			if (lpDestSkill->m_iNameCheckSum	!=	0x12345678)
				lpSrcSkill	=	lpDestSkill;
			if (lpSrcSkill->m_iNameCheckSum		==	0x12345678)
				continue;

			file.writeStreamCR("%3d  [%d,%d,%d]\n     \"%s\"\n     \"%s\"\n     \"%s\"\n",
				iSkill,lpSrcSkill->m_iNameCheckSum,lpSrcSkill->m_iPowerUpCheckSum,lpSrcSkill->m_iExplainCheckSum,
				lpSrcSkill->m_strName.String,lpSrcSkill->m_strPowerUp.String,lpSrcSkill->m_strExplain.String);
		}

		file.Close();

		break;
	}

	while (1)
	{
		sprintf(strFileName,"%s/localize/%s/old/_L_item_set.txt",_logFolder,m_strLocalizeFolder);
		uploadSetItemKindText(&l_srcItemSet,strFileName);

		sprintf(strFileName,"%s/localize/%s/_want modify/_L_item_set.txt",_logFolder,m_strLocalizeFolder);
		uploadSetItemKindText(&l_destItemSet,strFileName);

		sprintf(strFileName,"%s/localize/%s/_modify result/_L_item_set.txt",_logFolder,m_strLocalizeFolder);

		int		iItemSet;

		file.Open(strFileName,"wb");

		for (iItemSet=0;iItemSet<dMAX_SET_ITEM_GROUP_COUNT;iItemSet++)
		{
			cItemSetTextItem	*lpSrcItemSet	=	&l_srcItemSet.m_aSetItem[iItemSet];
			cItemSetTextItem	*lpDestItemSet	=	&l_destItemSet.m_aSetItem[iItemSet];
			cItemSetTextItem	*lpSelect		=	lpSrcItemSet;

			if	(lpDestItemSet->m_iNameCheckSum	!=	0x12345678)
				lpSelect		=	lpDestItemSet;
			else
				lpSelect		=	lpSrcItemSet;

			if	(lpSelect->m_iNameCheckSum		==	0x12345678)
				continue;

			file.writeStreamCR("set %3d [%d] \"%s\"",iItemSet,lpSelect->m_iNameCheckSum,lpSelect->m_strName.String);

			if	(lpDestItemSet->m_iExplainCheckSum	!=	0x12345678)
				lpSelect		=	lpDestItemSet;
			else
				lpSelect		=	lpSrcItemSet;

			file.writeStreamCR("set_explain %3d [%d] \"%s\"",iItemSet,lpSelect->m_iExplainCheckSum,lpSelect->m_strExplain.String);

			for	(int iSlot=0;iSlot<dMAX_ITEM_COUNT_IN_SET;iSlot++)
			{
				if	(lpDestItemSet->m_aData[iSlot].m_iNameCheckSum	!=	0x12345678)
					lpSelect		=	lpDestItemSet;
				else
					lpSelect		=	lpSrcItemSet;

				if	(lpSelect->m_aData[iSlot].m_iNameCheckSum		==	0x12345678)
					break;

				file.writeStreamCR("set_item_name %d,%d [%d] \"%s\"",iItemSet,iSlot,lpSelect->m_aData[iSlot].m_iNameCheckSum,lpSelect->m_aData[iSlot].m_strName.String);

				if	(lpDestItemSet->m_aData[iSlot].m_iExplainCheckSum	!=	0x12345678)
					lpSelect		=	lpDestItemSet;
				else
					lpSelect		=	lpSrcItemSet;

				file.writeStreamCR("set_item_explain %d,%d [%d] \"%s\"",iItemSet,iSlot,lpSelect->m_aData[iSlot].m_iExplainCheckSum,lpSelect->m_aData[iSlot].m_strExplain.String);
			}

			file.writeStreamCR(" ");
		}

		file.Close();

		break;
	}

	while (1)
	{
		sprintf(strFileName,"localize/%s/old/_L_guildSkill.txt",m_strLocalizeFolder);
		if (!upload2KindText(&l_srcItem,strFileName))
		{
			cMSG::Error("can not find follow guild skill text file",strFileName);
			break;
		}
		sprintf(strFileName,"localize/%s/_want modify/_L_guildSkill.txt",m_strLocalizeFolder);
		upload2KindText(&l_destItem,strFileName);

		sprintf(strFileName,"localize/%s/_modify result/_L_guildSkill.txt",m_strLocalizeFolder);

		int		iSkill;

		file.Open(strFileName,"wb");

		for (iSkill=0;iSkill<256;iSkill++)
		{
			C2KindTextItem	*lpSrcSkill=	&l_srcItem.m_aItems[iSkill];
			C2KindTextItem	*lpDestSkill=	&l_destItem.m_aItems[iSkill];

			if (lpDestSkill->m_iNameCheckSum	!=	0x12345678)
				lpSrcSkill	=	lpDestSkill;
			if (lpSrcSkill->m_iNameCheckSum		==	0x12345678)
				continue;

			file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
				iSkill,lpSrcSkill->m_iNameCheckSum,lpSrcSkill->m_iExplainCheckSum,
				lpSrcSkill->m_strName.String,lpSrcSkill->m_strExplain.String);
		}

		file.Close();

		break;
	}

	while (1)
	{
		sprintf(strFileName,"localize/%s/old/_L_title.txt",m_strLocalizeFolder);
		if (!upload2KindText(&l_srcItem,strFileName))
		{
			cMSG::Error("can not find follow title text file",strFileName);
			break;
		}
		sprintf(strFileName,"localize/%s/_want modify/_L_title.txt",m_strLocalizeFolder);
		upload2KindText(&l_destItem,strFileName);

		sprintf(strFileName,"localize/%s/_modify result/_L_title.txt",m_strLocalizeFolder);

		int		iTitle;

		file.Open(strFileName,"wb");

		for (iTitle=0;iTitle<256;iTitle++)
		{
			C2KindTextItem	*lpSrcTitle=	&l_srcItem.m_aItems[iTitle];
			C2KindTextItem	*lpDestTitle=	&l_destItem.m_aItems[iTitle];

			if (lpDestTitle->m_iNameCheckSum!=	0x12345678)
				lpSrcTitle	=	lpDestTitle;
			if (lpSrcTitle->m_iNameCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
				iTitle,lpSrcTitle->m_iNameCheckSum,lpSrcTitle->m_iExplainCheckSum,
				lpSrcTitle->m_strName.String,lpSrcTitle->m_strExplain.String);
		}

		file.Close();
		
		break;
	}

	while (1)
	{
		sprintf(strFileName,"localize/%s/old/_L_job.txt",m_strLocalizeFolder);

		if (!upload2KindText(&l_srcItem,strFileName,TRUE))
		{
			cMSG::Error("can not find follow text file",strFileName);
			break;
		}
		sprintf(strFileName,"localize/%s/_want modify/_L_job.txt",m_strLocalizeFolder);
		upload2KindText(&l_destItem,strFileName,TRUE);

		sprintf(strFileName,"localize/%s/_modify result/_L_job.txt",m_strLocalizeFolder);

		int		iJob;

		file.Open(strFileName,"wb");

		for (iJob=0;iJob<dMAX_JOB_COUNT;iJob++)
		{
			C2KindTextItem	*lpSrcTitle=	&l_srcItem.m_aItems[iJob];
			C2KindTextItem	*lpDestTitle=	&l_destItem.m_aItems[iJob];

			if (lpDestTitle->m_iNameCheckSum!=	0x12345678)
				lpSrcTitle	=	lpDestTitle;
			if (lpSrcTitle->m_iNameCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR("%3d  [%d,%d] \"%s\" \" \"",
				iJob,lpSrcTitle->m_iNameCheckSum,lpSrcTitle->m_iExplainCheckSum,
				lpSrcTitle->m_strName.String);
		}

		file.Close();
		
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
			sprintf(strFileName,"localize/%s/old/%s",m_strLocalizeFolder,astrText[iTF]);

			if	(!upload1KindText(&l_src1KindText,strFileName))
				l_src1KindText.reset();

			sprintf(strFileName,"localize/%s/_want modify/%s",m_strLocalizeFolder,astrText[iTF]);
			upload1KindText(&l_dest1KindText,strFileName);

			sprintf(strFileName,"localize/%s/_modify result/%s",m_strLocalizeFolder,astrText[iTF]);

			int		iJob;

			file.Open(strFileName,"wb");

			for (iJob=0;iJob<dMAX_COMPARE_TEXT_COUNT;iJob++)
			{
				C1KindTextItem	*lpSrc	=	&l_src1KindText.m_aItems[iJob];
				C1KindTextItem	*lpDest	=	&l_dest1KindText.m_aItems[iJob];

				if (lpDest->m_iCheckSum	!=	0x12345678)
					lpSrc	=	lpDest;
				if (lpSrc->m_iCheckSum	==	0x12345678)
					continue;

				file.writeStreamCR("%3d  [%d] \"%s\"",
					iJob,lpSrc->m_iCheckSum,lpSrc->m_strText.String);
			}

			file.Close();
			
			break;
		}

	}


	cMSG::Put("modify all game text data","complete");

	return	TRUE;
}
