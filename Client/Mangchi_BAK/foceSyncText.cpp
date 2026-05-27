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
cMAIN::forceSyncCommonText(HWND hwnd)
{
	if (!selectLocalizeFolder(hwnd))
		return	FALSE;

	char		strFileName[512];
	cDECODER	decoder;
	cFILE		file;

	if	(!IsFolder(_ms("localize/%s/force sync text",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/force sync text",_logFolder,m_strLocalizeFolder);
	if	(!IsFolder(_ms("localize/%s/import",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/import",_logFolder,m_strLocalizeFolder);
	if	(!IsFolder(_ms("localize/%s/lastest",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/lastest",_logFolder,m_strLocalizeFolder);

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
			sprintf(strFileName,"localize/%s/lastest/%s",m_strLocalizeFolder,astrText[iTF]);

			if (!upload1KindText(&l_src1KindText,strFileName))
			{
				cMSG::Error("can not find follow text file",strFileName);
				break;
			}

			sprintf(strFileName,"localize/%s/import/%s",m_strLocalizeFolder,astrText[iTF]);
			upload1KindText(&l_dest1KindText,strFileName,FALSE);

			sprintf(strFileName,"localize/%s/force sync text/%s",m_strLocalizeFolder,astrText[iTF]);

			int		iJob;

			file.Open(strFileName,"wb");

			for (iJob=0;iJob<dMAX_COMPARE_TEXT_COUNT;iJob++)
			{
				C1KindTextItem	*lpSrc	=	&l_src1KindText.m_aItems[iJob];
				C1KindTextItem	*lpDest	=	&l_dest1KindText.m_aItems[iJob];

				if (lpSrc->m_iCheckSum	==	0x12345678)
					continue;
				if (lpDest->m_iCheckSum	==	0x12345678)
					continue;

				file.writeStreamCR("%3d  [%d] \"%s\"",iJob,lpSrc->m_iCheckSum,lpDest->m_strText.String);
			}

			file.Close();
			
			break;
		}
	}

	return	TRUE;
}

BOOL
cMAIN::forceSyncText(HWND hwnd)
{
	if (!selectLocalizeFolder(hwnd))
		return	FALSE;

	char		strFileName[512];
	int			i;
	cDECODER	decoder;
	cFILE		file;

	if (!IsFolder(_ms("localize/%s/force sync text",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/force sync text",_logFolder,m_strLocalizeFolder);
	if (!IsFolder(_ms("localize/%s/import",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/import",_logFolder,m_strLocalizeFolder);
	if (!IsFolder(_ms("localize/%s/lastest",m_strLocalizeFolder)))
		return	ERRMSG("can not find follow folder","%s/localize/%s/lastest",_logFolder,m_strLocalizeFolder);

	if (cMSG::YESNO("compare all maps text data","do you want continue this work?")	==	IDYES)
	{
		cPROJECT::SetMapFolder();

		cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

		SetLogFolder();

		if (!lpFiles)
			return	ERRMSG("error!!","can not find map list!");

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);
			char	strName[128];

			strcpy(strName,_exportFileName(lpFile->String));

			if (cMAP::ForceSyncText(strName)	==	FALSE)
			{
				SetLogFolder();
				return	FALSE;
			}

			SetLogFolder();
			Sleep(1);
		}

		KILL(lpFiles);

		cPROJECT::SetMapFolder();
		SetCurrentDirectory("expansion");

		lpFiles	=	cFOLDER::GetFiles("*.rmd");

		SetLogFolder();

		if (!lpFiles)
			return	ERRMSG("error!!","can not find map list!");

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);
			char	strName[128];

			strcpy(strName,_exportFileName(lpFile->String));

			if (cMAP::ForceSyncText(strName,TRUE)	==	FALSE)
			{
				SetLogFolder();
				return	FALSE;
			}

			SetLogFolder();
			Sleep(1);
		}

		KILL(lpFiles);

	}

	SetLogFolder();

//	½ºÆ® 
	while(1)
	{
		int			iGroup;

		sprintf(strFileName,"%s/localize/%s/lastest/quest.txt",_logFolder,m_strLocalizeFolder);

		if (!uploadQuestText(&l_srcQuest,strFileName))
		{
			cMSG::Error("can not find follow quest text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/import/quest.txt",_logFolder,m_strLocalizeFolder);
		uploadQuestText(&l_destQuest,strFileName);

		sprintf(strFileName,"%s/localize/%s/force sync text/quest.txt",_logFolder,m_strLocalizeFolder);

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

				if (lpSrcQuest->m_iNameCheckSum	==	0x12345678)
					continue;

				file.writeStreamCR	("#%s [%d] %d \"%s\"",lpstrHeader,lpSrcQuest->m_iNameCheckSum,iQuest,lpDestQuest->m_strName);
				file.writeStreamCR	("    #client [%d] \"%s\"",lpSrcQuest->m_iClientCheckSum,lpDestQuest->m_strQuestClientInfo);
				file.writeStreamCR	("    #beginClient [%d] \"%s\"",GetTextCheckSum(lpSrcQuest->m_strBeginNPCName),lpSrcQuest->m_strBeginNPCName);
				file.writeStream	("    #value  [%d] \"",lpSrcQuest->m_iInfoCheckSum);

				file.Write			(lpDestQuest->m_strQuestValueComment,strlen(lpDestQuest->m_strQuestValueComment));
				file.writeStreamCR	("\"");

				for (int j=0;j<dQUEST_STATUS_COUNT;j++)
				{
					if (lpSrcQuest->m_iExplainCheckSum[j]	==	0x12345678)
						continue;

					int	iCheckSum	=	GetTextCheckSum(lpSrcQuest->m_astrExplain[j]);

					if (iCheckSum)
						file.writeStreamCR("    #explain [%d] %d \"%s\"",iCheckSum,j,lpDestQuest->m_astrExplain[j]);
				}

				file.writeStreamCR(" ");
			}
		}

		file.Close();

		break;
	}

//	¸Þ ½ºÆ®
	while(1)
	{
		sprintf(strFileName,"%s/localize/%s/lastest/mainQuest.txt",_logFolder,m_strLocalizeFolder);

		if	(!uploadQuestText(&l_srcMainQuestText,strFileName))
		{
			cMSG::Error("can not find follow main quest text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/import/mainQuest.txt",_logFolder,m_strLocalizeFolder);
		if	(!uploadQuestText(&l_destMainQuestText,strFileName))
		{
			cMSG::Error("can not find follow main quest text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/force sync text/mainQuest.txt",_logFolder,m_strLocalizeFolder);

		file.Open(strFileName,"wb");

		for (int iQuest=0;iQuest<c_iMaxChapterCount;iQuest++)
		{
			cMainQuestChapterText	*lpSrcQuest,*lpDestQuest;

			lpSrcQuest	=	&l_srcMainQuestText.m_aChapterList[iQuest];
			lpDestQuest	=	&l_destMainQuestText.m_aChapterList[iQuest];

			if	(lpSrcQuest->m_iNameCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR	("#mainQuestName	[%d] %d \"%s\"",iQuest,lpSrcQuest->m_iNameCheckSum,lpDestQuest->m_strName);
			file.writeStreamCR	("#mainQuestComment	[%d] %d \"%s\"",iQuest,lpSrcQuest->m_iCommentCheckSum,lpDestQuest->m_strComment);
			file.writeStreamCR	("#mainQuestEpilogComment	[%d] %d \"%s\"",iQuest,GetTextCheckSum(lpSrcQuest->m_strEpilogComment),lpDestQuest->m_strEpilogComment);

			for	(int iHighQuest=0;iHighQuest<c_iMaxHighQuestCount;iHighQuest++)
			{
				if	(lpSrcQuest->m_aiHiQuestNameChecksum[iHighQuest]	==	0x12345678)
					continue;

				file.writeStreamCR	("#highQuestName	[%d,%d] %d \"%s\"",iQuest,iHighQuest,lpSrcQuest->m_aiHiQuestNameChecksum[iHighQuest],lpDestQuest->m_astrHighQuest[iHighQuest]);
			}

			file.writeStreamCR(" ");
		}

		file.Close();

		break;
	}

//	
	while(1)
	{
		sprintf(strFileName,"%s/localize/%s/lastest/book.txt",_logFolder,m_strLocalizeFolder);

		if	(!uploadBookText(&l_srcBookText,strFileName))
		{
			cMSG::Error("can not find follow book text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/import/book.txt",_logFolder,m_strLocalizeFolder);
		if	(!uploadQuestText(&l_destBookText,strFileName))
		{
			cMSG::Error("can not find follow book text file",strFileName);
			break;
		}

		sprintf(strFileName,"%s/localize/%s/force sync text/book.txt",_logFolder,m_strLocalizeFolder);

		file.Open(strFileName,"wb");

		for (int iBook=0;iBook<c_iBookCount;iBook++)
		{
			cBookText	*lpSrcBook,*lpDestBook;

			lpSrcBook	=	&l_srcBookText.m_aBook[iBook];
			lpDestBook	=	&l_destBookText.m_aBook[iBook];

			if	(lpSrcBook->m_iCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR	("#bookName	[%d] %d \"%s\"",iBook,lpSrcBook->m_iCheckSum,lpDestBook->m_strBookName);
			file.writeStreamCR	("#bookItemComment	[%d] %d \"%s\"",iBook,lpSrcBook->m_iCommentCheckSum,lpDestBook->m_strItemComment.String);

			for (int iCategory=0;iCategory<c_iMaxBookCategoryCount;iCategory++)
			{
				cBookCategoryText	*lpSrcCategory,*lpDestCategory;

				lpSrcCategory	=	&lpSrcBook->m_aCategory[iCategory];
				lpDestCategory	=	&lpDestBook->m_aCategory[iCategory];

				if	(lpSrcCategory->m_iCategoryCheckSum	==	0x12345678)
					continue;

				file.writeStreamCR	("#bookCategory	[%d,%d] %d \"%s\"",iBook,iCategory,lpSrcCategory->m_iCategoryCheckSum,lpDestCategory->m_strCategoryName);

				for (int iPage=0;iPage<c_iMaxBookPageCount;iPage++)
				{
					cBookPageText	*lpSrcPage,*lpDestPage;

					lpSrcPage	=	&lpSrcCategory->m_aPage[iPage];
					lpDestPage	=	&lpDestCategory->m_aPage[iPage];

					if	(lpSrcPage->m_iCheckSum	==	0x12345678)
						continue;

					file.writeStreamCR	("#bookPage	[%d,%d,%d] %d \"%s\"",iBook,iCategory,iPage,lpSrcPage->m_iCheckSum,lpDestPage->m_strText.String);
				}
			}

			file.writeStreamCR(" ");
		}

		file.Close();

		break;
	}


	cMSG::Put("compare all game text data","complete");

	return	TRUE;

}
