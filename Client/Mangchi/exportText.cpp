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
#include "secretDungeon.h"
#include "mainQuest.h"
#include "book.h"

BOOL
cMAIN::exportAllText(HWND hwnd,BOOL _bIsWantSomeMap)
{
	int			i;
	char		strFileName[512];
	cFILE		file;

	SetLogFolder();

	if	(IsFolder("localize/lastest") == FALSE)
	{
		CreateDirectory("localize",NULL);
		SetCurrentDirectory("localize");
		CreateDirectory("lastest",NULL);
		SetLogFolder();
	}

	if	(_bIsWantSomeMap)
		if	(IsFolder("localize/some_map") == FALSE)
		{
			CreateDirectory("localize",NULL);
			SetCurrentDirectory("localize");
			CreateDirectory("some_map",NULL);

			cFOLDER::DeleteAllFile();
				
			SetLogFolder();
		}

	if	(cMSG::YESNO("export all maps text data","do you want continue this work?")	==	IDYES)
	{
		cPROJECT::SetMapFolder();

		if	(_bIsWantSomeMap)
		{
			SetCurrentDirectory("..");

			if	(!SetCurrentDirectory("update_map"))
			{
				SetLogFolder();
				return	ERRMSG("error!!","can not find update_map folder");
			}
		}

		cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

		if	(!lpFiles)
			return	ERRMSG("error!!","can not find map list!");

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);

			cPROJECT::SetMapFolder();

			if	(_bIsWantSomeMap)
			{
				SetCurrentDirectory("..");
				SetCurrentDirectory("update_map");
			}

			cMAP::Load(lpFile->String,0);
			g_aMap[0].exportText(FALSE,_bIsWantSomeMap);
			SetLogFolder();
			Draw();
			Sleep(1);
		}

		KILL(lpFiles);

		cPROJECT::SetMapFolder();

		if	(_bIsWantSomeMap)
		{
			SetCurrentDirectory("..");

			if	(!SetCurrentDirectory("update_map"))
			{
				SetLogFolder();
				return	ERRMSG("error!!","can not find update_map folder");
			}
		}

		if	(SetCurrentDirectory("expansion"))
		{
			lpFiles	=	cFOLDER::GetFiles("*.rmd");

			if	(lpFiles)
			{
				for (i=0;i<lpFiles->Count;i++)
				{
					cSTRING	*lpFile	=	lpFiles->Pick(i);

					cPROJECT::SetMapFolder();
					if	(_bIsWantSomeMap)
					{
						SetCurrentDirectory("..");
						SetCurrentDirectory("update_map");
					}

					SetCurrentDirectory("expansion");
					cMAP::Load(lpFile->String,0);
					g_aMap[0].exportText(TRUE,_bIsWantSomeMap);

					SetLogFolder();
					Draw();
					Sleep(1);
				}
			}
			else
			{
				if	(_bIsWantSomeMap	==	FALSE)
					return	ERRMSG("error!!","can not find map list!");
			}
		}

		KILL(lpFiles);
	}

	SetLogFolder();

	if	(_bIsWantSomeMap)
	{
		cMSG::Put("export some filed text data","complete");
		return	TRUE;
	}

	{
		sprintf(strFileName,"%s/localize/lastest/project.txt",_logFolder);

		file.Open(strFileName,"wb");

		{
			CKarmaContainer	*lpkc	=	&cPROJECT::s_kcMonsterDeath;

			for (int iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
			{
				CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];

				for (int iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
				{
					CKarmaItem	*lpKI	=	&lpKarma->m_pReaction[iReaction];

					if (lpKI->m_wStringSize)
					{
						RemoveQuotationMark(lpKI->m_strString);
						file.writeStreamCR("#md_karma_reaction [%d] %d,%d \"%s\"",GetTextCheckSum(lpKI->m_strString),iKarma,iReaction,lpKI->m_strString);
					}
				}
			}
		}

		{
			CKarmaContainer	*lpkc	=	&cPROJECT::s_kcMonsterGen;

			for (int iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
			{
				CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];

				for (int iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
				{
					CKarmaItem	*lpKI	=	&lpKarma->m_pReaction[iReaction];

					if (lpKI->m_wStringSize)
					{
						RemoveQuotationMark(lpKI->m_strString);
						file.writeStreamCR("#mg_karma_reaction [%d] %d,%d \"%s\"",GetTextCheckSum(lpKI->m_strString),iKarma,iReaction,lpKI->m_strString);
					}
				}
			}
		}

		file.Close();
	}

//	퀘스트 
	{
		sprintf(strFileName,"%s/localize/lastest/quest.txt",_logFolder);

		file.Open(strFileName,"wb");

		for (int iGroup=0;iGroup<2;iGroup++)
		{
			char	*lpstrHeader;
			CQuest	*lpQuestGroup;

			if (iGroup	==	0)
			{
				lpstrHeader	=	"quest";
				lpQuestGroup=	&g_quest;
			}
			else
			{
				lpstrHeader	=	"arbeit";
				lpQuestGroup=	&g_questArbeit;
			}
			
			for (i=0;i<dQUEST_COUNT;i++)
			{
				CQuestItem	*lpQuest	=	&lpQuestGroup->m_aQuest[i];

				if (lpQuest->m_wSerial	==	0xffff)
					continue;
				
				file.writeStreamCR	("#%s [%d] %d \"%s\"",lpstrHeader,GetTextCheckSum(lpQuest->m_strName),i,lpQuest->m_strName);
				file.writeStreamCR	("    #client [%d] \"%s\"",GetTextCheckSum(lpQuest->m_strQuestClientInfo),lpQuest->m_strQuestClientInfo);
				file.writeStreamCR	("    #beginClient [%d] \"%s\"",GetTextCheckSum(lpQuest->m_strBeginNPCName),lpQuest->m_strBeginNPCName);
				file.writeStream	("    #value  [%d] \"",GetTextCheckSum(lpQuest->m_strQuestValueComment));
				file.Write			(lpQuest->m_strQuestValueComment,strlen(lpQuest->m_strQuestValueComment));


				file.writeStreamCR	("\"");

				for (int j=0;j<dQUEST_STATUS_COUNT;j++)
				{
					int	iCheckSum	=	GetTextCheckSum(lpQuest->m_astrExplain[j]);

					if	(iCheckSum)
						file.writeStreamCR("    #explain [%d] %d \"%s\"",iCheckSum,j,lpQuest->m_astrExplain[j]);
				}

				file.writeStreamCR(" ");
			}	
		}

		file.Close();
	}

//	메인 퀘스트
	{
		sprintf(strFileName,"%s/localize/lastest/mainQuest.txt",_logFolder);

		file.Open(strFileName,"wb");

		for (int iQuest=0;iQuest<c_iMaxChapterCount;iQuest++)
		{
			cChapterInfo	*lpChapterInfo	=	g_chapterManager.get(iQuest);

			if	(!lpChapterInfo)
				continue;

			file.writeStreamCR	("#mainQuestName	[%d] %d \"%s\"",iQuest,GetTextCheckSum(lpChapterInfo->m_strName),lpChapterInfo->m_strName);
			file.writeStreamCR	("#mainQuestComment	[%d] %d \"%s\"",iQuest,GetTextCheckSum(lpChapterInfo->m_strComment),lpChapterInfo->m_strComment);
			file.writeStreamCR	("#mainQuestEpilogComment	[%d] %d \"%s\"",iQuest,GetTextCheckSum(lpChapterInfo->m_strEpilogComment),lpChapterInfo->m_strEpilogComment);

			for	(int iHighQuest=0;iHighQuest<lpChapterInfo->m_iHighQuestCount;iHighQuest++)
				file.writeStreamCR	("#highQuestName	[%d,%d] %d \"%s\"",iQuest,iHighQuest,GetTextCheckSum(lpChapterInfo->m_strHighQuest[iHighQuest]),lpChapterInfo->m_strHighQuest[iHighQuest]);

			file.writeStreamCR(" ");
		}

		file.Close();
	}

//	책
	{
		sprintf(strFileName,"%s/localize/lastest/book.txt",_logFolder);

		file.Open(strFileName,"wb");

		for (int iBook=0;iBook<c_iBookCount;iBook++)
		{
			cBookInfo	*lpBook	=	g_book.getBook(iBook);

			if	(!lpBook)
				continue;

			RemoveQuotationMark(lpBook->m_strBookName);
			RemoveQuotationMark(lpBook->m_pstrText);

			file.writeStreamCR	("#bookName	[%d] %d \"%s\"",iBook,GetTextCheckSum(lpBook->m_strBookName),lpBook->m_strBookName);
			file.writeStreamCR	("#bookItemComment	[%d] %d \"%s\"",iBook,GetTextCheckSum(lpBook->m_pstrText),lpBook->m_pstrText);

			for (int iCategory=0;iCategory<lpBook->m_wCategoryCount;iCategory++)
			{
				cBookCategory	*lpCategory	=	g_book.getCategory(iBook,iCategory);

				RemoveQuotationMark(lpCategory->m_strCategoryName);
				file.writeStreamCR	("#bookCategory	[%d,%d] %d \"%s\"",iBook,iCategory,GetTextCheckSum(lpCategory->m_strCategoryName),lpCategory->m_strCategoryName);
				
				for (int iPage=0;iPage<lpCategory->m_iPageCount;iPage++)
				{
					cBookPageInfo	*lpPage	=	g_book.getPage(iBook,iCategory,iPage);

					char	*lpstrPageText		=	lpPage->getContents();

					if	(lpstrPageText)
					{
						RemoveQuotationMark(lpstrPageText);
						file.writeStreamCR	("#bookPage	[%d,%d,%d] %d \"%s\"",iBook,iCategory,iPage,GetTextCheckSum(lpstrPageText),lpstrPageText);
					}
				}
			}

			file.writeStreamCR(" ");
		}

		file.Close();
	}

//	비밀 던전
	{
		SetLogFolder();

		file.Open("localize/lastest/_L_secret_dungeon.txt","wb");

		for (i=0;i<g_sdManager.getMaxCount();i++)
		{
			cSecretDungeon	*lpDungeon	=	g_sdManager.get(i);

			if	(!lpDungeon)
				continue;

			file.writeStreamCR("%3d  [%d]  \"%s\"",i,GetTextCheckSum(lpDungeon->m_strName),lpDungeon->m_strName);
		}

		file.Close();
	}

//	아이템
	{
		SetLogFolder();

		file.Open("localize/lastest/_L_item.txt","wb");

		for (i=0;i<dBASIC_ITEM_COUNT;i++)
		{
			if (g_aBasicItem[i].m_iSerial	==	0xffffffff)
				continue;

			char	*lpstrComment	=	g_aBasicItem[i].m_pItemComment;

			file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
				g_aBasicItem[i].m_iSerial,GetTextCheckSum(g_aBasicItem[i].m_strName),GetTextCheckSum(lpstrComment),
				g_aBasicItem[i].m_strName,lpstrComment);
		}
		file.Close();
	}

//	프리미엄 아이템
	{
		SetLogFolder();

		file.Open("localize/lastest/_L_PremiumItem.txt","wb");

		int			iCount			=	g_premiumItem.getLastIndex();
		CCustomItem	*lpCustomItemBay=	g_premiumItem.getItemBay();

		for (int i=0;i<g_premiumItem.getLastIndex();i++)
		{
			CCustomItem	*lpItem		=	g_premiumItem.getItem(i);

			if	(!lpItem)
				continue;

			file.writeStreamCR("%3d  [%d]	\"%s\"",i,GetTextCheckSum(lpItem->m_strName),lpItem->m_strName);
		}
	}

//	세트 아이템
	{
		SetLogFolder();

		file.Open("localize/lastest/_L_item_set.txt","wb");

		for (i=0;i<g_itemSetManager.getMaxCount();i++)
		{
			cItemSet	*lpSet	=	g_itemSetManager.get(i);

			if	(!lpSet)
				continue;


			file.writeStreamCR("set %3d [%d] \"%s\"",i,GetTextCheckSum(lpSet->m_strName),lpSet->m_strName);
			file.writeStreamCR("set_explain %3d [%d] \"%s\"",i,GetTextCheckSum(lpSet->m_pItemComment),lpSet->m_pItemComment);

			for	(int j=0;j<dMAX_ITEM_COUNT_IN_SET;j++)
			{
				if	(lpSet->m_aItem[j].m_wBaseItem	==	0xffff)
					continue;

				if	(i	==	9	&&	j	==	2)
					i	=	i;

				file.writeStreamCR("set_item_name %d,%d [%d] \"%s\"",i,j,GetTextCheckSum(lpSet->m_aItem[j].m_strName),lpSet->m_aItem[j].m_strName);
				file.writeStreamCR("set_item_explain %d,%d [%d] \"%s\"",i,j,GetTextCheckSum(lpSet->m_aItem[j].m_pItemComment),lpSet->m_aItem[j].m_pItemComment);
			}

			file.writeStreamCR(" ");
		}
		file.Close();
	}

//	직업
	{
		SetLogFolder();

		file.Open("localize/lastest/_L_job.txt","wb");

		for (i=0;i<dMAX_JOB_COUNT;i++)
		{
			cJOB*	lpJob	=	g_denJob.getJob(i);

			if (!lpJob)
				continue;

			file.writeStreamCR("%3d [%d,%d] \"%s\" \" \"",i,GetTextCheckSum(lpJob->m_strName),GetTextCheckSum(" "),lpJob->m_strName);
		}
		file.Close();
	}

//	아이템 접두사
	{
		SetLogFolder();
		file.Open("localize/lastest/_L_itemPrefix.txt","wb");

		for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
		{
			cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.m_den.get(i);

			if (!lpPrefix	||	lpPrefix->m_wSerial	==	0xffff)
				continue;

			file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
				lpPrefix->m_wSerial,GetTextCheckSum(lpPrefix->m_str1stPrefix),GetTextCheckSum(lpPrefix->m_str2stPrefix),
				lpPrefix->m_str1stPrefix,lpPrefix->m_str2stPrefix);
		}
		file.Close();
	}

//	스킬
	{
		SetLogFolder();
		file.Open("localize/lastest/_L_skill.txt","wb");

		for (i=0;i<dMAX_SKILL;i++)
		{
			if (g_aSkill[i].m_wSerial	==	0xffff)
				continue;

			file.writeStreamCR("%3d  [%d,%d,%d]\n     \"%s\"\n     \"%s\"\n     \"%s\"\n",
				g_aSkill[i].m_wSerial,GetTextCheckSum(g_aSkill[i].m_strName),GetTextCheckSum(g_aSkill[i].m_strComment),GetTextCheckSum(g_aSkill[i].m_strPowerup),
				g_aSkill[i].m_strName,g_aSkill[i].m_strComment,g_aSkill[i].m_strPowerup);
		}
		file.Close();
	}

//	길드 스킬
	{
		SetLogFolder();

		if	(!file.Open("localize/lastest/_L_guildSkill.txt","wb"))
		{
			return	ERRMSG("error!!","can not open follow file\n\nlocalize/lastest/_L_guildSkill.txt");
		}

		for (i=0;i<dMAX_GUILD_SKILL_COUNT;i++)
		{
			CGuildSkill	*lpSkill	=	&g_gsManager.m_aSkills[i];

			if (lpSkill->m_wSerial	==	0xffff)
				continue;

			file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
					lpSkill->m_wSerial,GetTextCheckSum(lpSkill->m_strName),GetTextCheckSum(lpSkill->m_strComment),
					lpSkill->m_strName,lpSkill->m_strComment);
		}

		file.Close();
	}

//	타이틀
	{
		SetLogFolder();
		file.Open("localize/lastest/_L_title.txt","wb");

		for (int i=0;i<dMAX_TITLE_COUNT;i++)
		{
			if (g_titleManager.m_aTitles[i].m_wSerial	==	0xffff)
				continue;

			file.writeStreamCR("%3d  [%d,%d]\n     \"%s\"\n     \"%s\"\n",
				g_titleManager.m_aTitles[i].m_wSerial,GetTextCheckSum(g_titleManager.m_aTitles[i].m_strName),GetTextCheckSum(g_titleManager.m_aTitles[i].m_strExplain),
				g_titleManager.m_aTitles[i].m_strName,g_titleManager.m_aTitles[i].m_strExplain);
		}

		file.Close();
	}

//	textData.dat
	{
		g_loaderText.exportText("localize/lastest");
	}


	SetLogFolder();

	cMSG::Put("export all game text data","complete");

	return	TRUE;
}
