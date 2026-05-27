#include "quest.h"
#include "CMessage.h"
#include "secretDungeonDefine.h"
#include "book.h"
#include "mess_sign.h"
#include "commonClass.h"

class	cChapterInfo
{
public:
	int		m_iHighQuestCount;
	char	m_strName[c_iChapterNameLength];
	char	m_strComment[c_iChapterCommentLength];
	char	m_strEpilogComment[c_iChapterEpilogCommentLength];

	char	m_strHighQuest[c_iMaxHighQuestCount][c_iHighQuestNameLength];
};

CQuestItem*
CQuest::GetQuest(int _iType,int _iIndex)
{
	if	(_iType	==	eQT_NORMAL)
		return	g_quest.getQuest(_iIndex);	

	return	g_questArbeit.getQuest(_iIndex);	
}

CQuestItem*
CQuest::GetQuest(int _iIndex)
{
	if	 (_iIndex	>=	dQUEST_AND_ARBEIT_BORDER_VALUE)
	{
		_iIndex	-=	dQUEST_AND_ARBEIT_BORDER_VALUE;

		return	g_questArbeit.getQuest(_iIndex);
	}

	return	g_quest.getQuest(_iIndex);	
}

BOOL
CQuest::Load()
{
	cFILE	file;
	int		i,iSize,iVersion,iNation;
	char	strFileName[512];

	sprintf(strFileName,"%s/data/scenario/red stone.rpd",g_strGameFolder);

	if (!file.Open(strFileName,"rb"))
	{
		return	ERRMSG("error!","can not find project data file\n\n'red stone.rpd'");
	}

	{
		char	strHeader[60];

		file.Read(&iSize				,4	);		//	화일 사이즈

		file.Read(strHeader	,60	);		//	헤더

		for (i=0;;i++)
		{
			if	(stricmp(g_strProjectHeader[i],"end"		)==0)
				return	FALSE;

			if	(stricmp(g_strProjectHeader[i],strHeader	)==0)
			{
				iVersion	=	i;
				break;
			}
		}

		file.Skip(128);	//	프로젝트 이름
		file.Read(&iNation,sizeof(int));	//	프로젝트 이름
	}	//	프로젝트 기본 데이터 불러오기

	cMessSign::SetDataEncodeTable(iNation);

	{
		int	iQuestCount;

		file.Read(&iQuestCount,4);

		if	(iVersion	>=	ePVUI_12_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iQuestCount,4);

		g_quest.init(iQuestCount);

		CQuestItemSaveData	saveQuestData;

		for (i=0;i<iQuestCount;i++)
		{
			file.Read(&saveQuestData,sizeof(saveQuestData));

			if	(iVersion	>=	ePVUI_12_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&saveQuestData,sizeof(saveQuestData));

			g_quest.setQuestInfo(i,&saveQuestData);

			for (int iStatus=0;iStatus<dQUEST_STATUS_COUNT;iStatus++)
				g_quest.setExplain(i,iStatus,saveQuestData.m_astrExplain[iStatus]);
		}

		file.Read(&iQuestCount,4);
		if	(iVersion	>=	ePVUI_12_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iQuestCount,4);

		g_questArbeit.init(iQuestCount);

		for (i=0;i<iQuestCount;i++)
		{
			file.Read(&saveQuestData,sizeof(saveQuestData));

			if	(iVersion	>=	ePVUI_12_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&saveQuestData,sizeof(saveQuestData));

			g_questArbeit.setQuestInfo(i,&saveQuestData);

			for (int iStatus=0;iStatus<dQUEST_STATUS_COUNT;iStatus++)
				g_questArbeit.setExplain(i,iStatus,saveQuestData.m_astrExplain[iStatus]);
		}
	}

//	비밀 던전 정보
	{
		int	iValue;
		file.Read(&iValue,4);	//

		if (iValue	==	0x12345678)
		{
			int		iCount;

			file.Read(&iCount,4);

			if	(iVersion	>=	ePVUI_12_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&iCount,sizeof(iCount));

			if	(iCount)
				file.Seek(sizeof(cSecretDungeonDefine)*iCount);
		}
		else
			file.Seek(-4,SEEK_CUR);

	}

//	비밀 던젼 수치
	{
		file.Seek(dMAX_SECRET_DUNGEON_COUNT*dMAX_DUNGEON_VALUE_COUNT*dDUNGEON_VALUE_TEXT_LENGTH+4);
	}

//	메인 퀘스트 정보
	{
		file.Seek(c_iMaxMainQuestValueCount*c_iMainQuestValueNameLength+c_iMaxMainQuestSwitchCount*c_iMainQuestSwitchNameLength+4);
		file.Seek(4+4+sizeof(cChapterInfo)*c_iMaxChapterCount);
	}

//	책 정보
	{
		file.Read(&g_book.m_iBookCount,4);
		
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

	file.Close();

	return	TRUE;
}


CQuest	g_quest;
CQuest	g_questArbeit;

//
//	초기화
void
CQuest::init(int _iCount)
{
	m_iCount	=	_iCount;
	pKILL(m_pQuest);

	m_pQuest	=	new CQuestItem [m_iCount];
}

CQuestItem*
CQuest::getQuestByName(char *_lpstrName)
{
	for (int i=0;i<m_iCount;i++)
	{
		if (stricmp(m_pQuest[i].m_strName,_lpstrName) == 0)
			return	&m_pQuest[i];
	}

	return	NULL;
}


void
CQuest::setQuestInfo(int _iQeustIndex,CQuestItemSaveData *_lpQuest)
{
	strcpy(m_pQuest[_iQeustIndex].m_strName				,_lpQuest->m_strName);
	strcpy(m_pQuest[_iQeustIndex].m_strQuestClientInfo	,_lpQuest->m_strQuestClientInfo);
	strcpy(m_pQuest[_iQeustIndex].m_strQuestValueComment,_lpQuest->m_strQuestValueComment);

	m_pQuest[_iQeustIndex].m_wSerial							=	_iQeustIndex;
	m_pQuest[_iQeustIndex].m_isEnableCancel						=	_lpQuest->m_isEnableCancel						;
	m_pQuest[_iQeustIndex].m_isRemoveQuestItemByCancelQuest		=	_lpQuest->m_isRemoveQuestItemByCancelQuest		;

}