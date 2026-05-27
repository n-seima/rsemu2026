#include "stdafx.h"
#include <winsock2.h>
#include <atlbase.h>
#include <stdio.h>
#include "cITEM.H"
#include "CItemPrefix.h"
#include "CMessage.h"
#include "book.h"
#include	"ArcaDefine.h"
#include	"mess_sign.h"

int		cITEM::s_iCount			=	0;
int		cITEM::s_iPrefixCount	=	0;
int		g_iBreedingRecordBook	=	-1;

cBASIC_ITEM		g_aBasicItem[dBASIC_ITEM_COUNT];

BOOL
cBASIC_ITEM::isAbvailJobItem()
{
	if (	m_aEnableJob[0	]	+
		m_aEnableJob[1	]		+
		m_aEnableJob[2	]		+
		m_aEnableJob[3	]		+
		m_aEnableJob[4	]		+
		m_aEnableJob[5	]	+
		m_aEnableJob[8	]	+
		m_aEnableJob[9	]	==	0)
		return	FALSE;

	return	TRUE;
}

BOOL
cITEM::ReadItemData()
{
	if	(s_iCount)
		return	TRUE;

	CRegKey		reg;

	char		strLogFolder[512],strDataFolder[512];

	if	(reg.Open( g_hRegKey, g_lpszRegistry) == ERROR_SUCCESS)
	{
		DWORD	count	=	sizeof(strLogFolder);

		if (reg.QueryValue((LPTSTR )strLogFolder,"Path",&count) == ERROR_SUCCESS)
		{	
			if	(count	<=	0)
				return FALSE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;

	sprintf(strDataFolder,"%s/data/scenario/red stone",strLogFolder);

	if	(!SetCurrentDirectory(strDataFolder))
		return	FALSE;

	FILE	*fp;

	fp	=	fopen(dITEM_FILE_NAME,"rb");

	if	(!fp)
		return	FALSE;

	int	iVersion			=	-1;

	{
		fseek(fp,-8,SEEK_END);

		int	iAddVersionChecker	=	dITEM_VERSION_CHECKER;

		fread(&iAddVersionChecker,sizeof(iAddVersionChecker),1,fp);

		if	(iAddVersionChecker	==	dITEM_VERSION_CHECKER)
			fread(&iVersion,sizeof(iVersion),1,fp);

		fseek(fp,0,SEEK_SET);
	}

	int	iNation;

	fread(&iNation,4,1,fp);
	cMessSign::SetDataEncodeTable(iNation);

	fread(&s_iCount,4,1,fp);

	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		cMessSign::DecodeDataBuffer2(&s_iCount,4);

	fseek(fp,4,SEEK_CUR);
	fread(g_aBasicItem,sizeof(cBASIC_ITEM)*s_iCount,1,fp);

	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
	{
		for (int i=0;i<s_iCount;i++)
		{
			cMessSign::DecodeDataBuffer2(g_aBasicItem+i,sizeof(cBASIC_ITEM));

			if	(g_aBasicItem[i].m_aGenerateData[0].m_wEffect	==	eIE_BREEDING_RECORD)
				g_iBreedingRecordBook	=	i;
		}
	}

	fread(&s_iPrefixCount,4,1,fp);
	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		cMessSign::DecodeDataBuffer2(&s_iPrefixCount,4);

	fread(g_aItemPrefix,s_iPrefixCount*sizeof(cITEM_PREFIX),1,fp);

	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		for (int i=0;i<s_iPrefixCount;i++)
			cMessSign::DecodeDataBuffer2(g_aItemPrefix+i,sizeof(cITEM_PREFIX));
	
	//	프리미엄 아이템 정보 로딩
	{
		int	iCount;
		fread(&iCount,4,1,fp);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);
		fseek(fp,iCount*sizeof(CCustomItemDefine),SEEK_CUR);
	}


	//	커스텀 아이템 정보 스킵~
	{
		int	iCount;
		fread(&iCount,4,1,fp);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);

		fseek(fp,iCount*sizeof(CCustomItemDefine),SEEK_CUR);
	}

	//	보물 상자 정보 스킵~
	{
		int	iCount;
		fread(&iCount,4,1,fp);

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&iCount,4);

		fseek(fp,iCount*sizeof(cArcaDefine),SEEK_CUR);

	}

	//	그룹 아이템
	{
		int	iValue;

		fread(&iValue,4,1,fp);

		if (iValue	==	0x12345674)
		{
			int	iCount;
		//	세트 아이템 로딩
			fread(&iCount,4,1,fp);

			if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
				cMessSign::DecodeDataBuffer2(&iCount,4);

			if (iCount)
			{
				g_itemSetManager.init(iCount);

				cItemSet*	lpBay		=	g_itemSetManager.getBay();

				fread(lpBay,sizeof(cItemSet)*iCount,1,fp);

				g_itemSetManager.build();
			}
		}
	}

	fclose(fp);
	return TRUE;
}

BOOL
cITEM::isIncorrectItem()
{
	if (m_wBaseItem	==	0	)
	{
		reset();

		return	FALSE;
	}

	if (isBreedingRecordBook())
		return	FALSE;

	BOOL	bIsIncorrectData	=	FALSE;
	int		i;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix == 0xffff)
			continue;

		if (m_aPrefix[i].m_wPrefix	>=	cITEM::s_iPrefixCount)
		{
			memset(&m_aPrefix[i],0xff,sizeof(CItemPrefixInfo));

			bIsIncorrectData	=	TRUE;
		}
	}

	for (i=0;i<dITEM_PREFIX_COUNT-1;i++)
	{
		if (m_aPrefix[i].m_wPrefix == 0xffff)
		{
			if (m_aPrefix[i+1].m_wPrefix != 0xffff)
			{
				memcpy(&m_aPrefix[i],&m_aPrefix[i+1],sizeof(CItemPrefixInfo));
				memset(&m_aPrefix[i+1],0xff,sizeof(CItemPrefixInfo));
			}
		}
	}

	return	bIsIncorrectData;
}

BOOL
IsImportantPrefix(int _iCode)
{
	if (_iCode >= 1001 && _iCode <= 1020)
		return	TRUE;
	if (_iCode >= 1401 && _iCode <= 1406)
		return	TRUE;
	if (_iCode >= 1417 && _iCode <= 1423)
		return	TRUE;
	if (_iCode >= 1503 && _iCode <= 1510)
		return	TRUE;
	if (_iCode >= 1601 && _iCode <= 2304)
		return	TRUE;

	return	FALSE;
}

int
cITEM::getIncorrectPrefixCount()
{
	if (m_wBaseItem	==	0xffff)
		return	0;

	if (isBreedingRecordBook())
		return	0;

//	if (getRequireLevelForEquip()	<=	200)
//		return	0;
//	if (getRequireLevelByPrefix()	<=	50)
//		return	0;

	BOOL	iIncorrectPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)	
			break;

		if (s_iPrefixCount <= m_aPrefix[i].m_wPrefix)
			return	0;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];
		cBASIC_ITEM		*lpBasicItem=	getBasicItem();

		if (!lpBasicItem)
			return	0;

		if (lpBasicItem->isAbvailJobItem())
			return	0;

		if (lpPrefix->m_wDiscernmentCode>=	10000)
			continue;

//		if (lpPrefix->m_wDropLevel	<	80)
//			if (IsImportantPrefix(lpPrefix->m_wDiscernmentCode) == FALSE)
//				continue;

		if (lpPrefix->m_aStickableItem[lpBasicItem->m_wKind] == FALSE)
			iIncorrectPrefixCount++;
	}

	return	iIncorrectPrefixCount;
}

int
cITEM::getRequireLevelForEquip()
{
	cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)
		return	0;

	return	getRequireLevelByPrefix()+lpBasicItem->m_wRequireLevel;
}

int
cITEM::getRequireLevelByPrefix()//	접두사로 인해 착용/사용에 필요한 레벨
{
	if (isBreedingRecordBook())
		return	0;

	int	iPrefixLevel	=	0,i;
	int	aPrefixLevel[3]	=	{0,0,0};
	int	iMaxPrefixIndex	=	0;
	int	iMaxPerfexLevel	=	0;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		CItemPrefixInfo	*lpPrefixInfo	=	&m_aPrefix[i];

		if (lpPrefixInfo->m_wPrefix	==	0xffff)	break;

		cITEM_PREFIX		*lpPrefix		=	&g_aItemPrefix[lpPrefixInfo->m_wPrefix];

		iPrefixLevel	=	max(lpPrefix->m_wDropLevel,iPrefixLevel);
		aPrefixLevel[i]	=	lpPrefix->m_wDropLevel;
		iMaxPerfexLevel	=	max(iPrefixLevel,iMaxPerfexLevel);
		iMaxPrefixIndex	=	i;
	}

	int	iFirst,iSecond;

	if (iMaxPrefixIndex	==	0)	iFirst	=	1,iSecond	=	2;
	if (iMaxPrefixIndex	==	1)	iFirst	=	0,iSecond	=	2;
	if (iMaxPrefixIndex	==	2)	iFirst	=	0,iSecond	=	1;

	if (aPrefixLevel[iFirst] && aPrefixLevel[iSecond])
	{
		if (aPrefixLevel[iFirst] > aPrefixLevel[iSecond])
				iPrefixLevel	=	iPrefixLevel+aPrefixLevel[iFirst]*2/3+aPrefixLevel[iSecond]/3;
		else	iPrefixLevel	=	iPrefixLevel+aPrefixLevel[iSecond]*2/3+aPrefixLevel[iFirst]/3;
	}

	if (aPrefixLevel[iFirst] && aPrefixLevel[iSecond]==0)
	{
		iPrefixLevel	=	iPrefixLevel+	aPrefixLevel[iFirst]*2/3+aPrefixLevel[iSecond]/3;
	}

	return	iMaxPerfexLevel;//iPrefixLevel;
}

//
//	아이템 이름
char*
cITEM::getName()
{
	static	char	s_strItemName[512];

	char	strString[512];

	s_strItemName[0]	=	NULL;

	cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)
		return	NULL;

	if	(isBreedingRecordBook()  || isCoupleRing() || checkFirstEffect(eIE_MINIPET_POUCH))
		return	lpBasicItem->m_strName;

	cSetItem	*lpSetItem	=	getSetItem();

	if	(lpSetItem)
	{
		sprintf(s_strItemName,"(T) %s",lpSetItem->m_strName);

		return	s_strItemName;
	}

	if	(isBook())
	{
		sprintf(s_strItemName,"(B) %s",getBookName());

		return	s_strItemName;
	}

	if	(isAddPageToBookItem())
	{
		int	iBeginPage	=	getBeginPageInBook()+1;
		int	iEndPage	=	getEndPageInBook()+1;

		if	(iBeginPage	==	iEndPage)
			sprintf(s_strItemName,"(B) %s[Page %d]",getBookName(),iBeginPage);
		else
			sprintf(s_strItemName,"(B) %s[Page %d~%d]",getBookName(),iBeginPage),iEndPage;

		return	s_strItemName;
	}

	if	(isSecretDungeonKey()		)
	{
		strcpy(s_strItemName,"Secret dungeon Key");

		return	s_strItemName;
	}

	if (isExtraItem())
	{
		sprintf(s_strItemName,"(E) %s",lpBasicItem->m_strName);

		return	s_strItemName;
	}

	if	(lpBasicItem->m_bIsExceptionItem && lpBasicItem->isEternalItem() == FALSE)
		return	lpBasicItem->m_strName;

	if (!isRareItem() && !isUniqueItem())
	{
		if (m_isBroken					)
			sprintf(s_strItemName,"%s %s",dMSG_BROKEN,lpBasicItem->m_strName);
		else
		if (m_bf1IsSpecialItem == FALSE && m_isCanNotAttachPrefixItem	)
			sprintf(s_strItemName,"%s %s",dMSG_CAN_NOT_IMPROVE,lpBasicItem->m_strName);
		else								strcpy(s_strItemName,lpBasicItem->m_strName);

		return	s_strItemName;
	}

			if (isUniqueItem() && isRareItem())	strcpy(s_strItemName,"(S) ");
	else	if (isUniqueItem())	strcpy(s_strItemName,"(U) ");
	else	if (isRareItem())	strcpy(s_strItemName,"(R) ");

	if	(m_isBroken					)
		strcat(s_strItemName,dMSG_BROKEN);
	else
	if	(m_bf1IsSpecialItem == FALSE && m_isCanNotAttachPrefixItem	)
		strcat(s_strItemName,dMSG_CAN_NOT_IMPROVE);

//	형용사 2차 + 형용사 2차 + 형용사 1차 + 아이템
//	형용사 2차 + 형용사 1차 + 명사 1차 + 아이템
//	형용사 1차 + 명사 2차 + 명사 1차 + 아이템
//	명사 2차 + 명사 2차 + 명사 1차 + 아이템
	int	iPrefixCount	=	getPrefixCount();
	int	iNounCount		=	getNounPrefixCount();

//	접두사가 하나 일때
	if (iPrefixCount	==	1)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[0].m_wPrefix];

		if(strcmp(lpPrefix->m_str1stPrefix,"")==0)
		{
			sprintf(strString,"ERROR",lpPrefix->m_str1stPrefix);
		}else
		{
			sprintf(strString,"%s ",lpPrefix->m_str1stPrefix);
		}
		strcat(s_strItemName,strString);
	}

//	접두사가 둘 일때
	if (iPrefixCount	==	2)
	{
		cITEM_PREFIX	*lpPrefix1	=	&g_aItemPrefix[m_aPrefix[0].m_wPrefix];
		cITEM_PREFIX	*lpPrefix2	=	&g_aItemPrefix[m_aPrefix[1].m_wPrefix];

		if (iNounCount	==	0)	//	둘다 형용사
		{
			sprintf(strString,"%s %s ",lpPrefix1->m_str2stPrefix,lpPrefix2->m_str1stPrefix);
			strcat(s_strItemName,strString);
		}

		if (iNounCount	==	1)
		{
			if (lpPrefix1->m_wPrefixForm	==	0)	//	명사
			{
				sprintf(strString,"%s %s ",lpPrefix2->m_str1stPrefix,lpPrefix1->m_str1stPrefix);
				strcat(s_strItemName,strString);
			}
			else	//	형용사
			{
				sprintf(strString,"%s %s ",lpPrefix1->m_str1stPrefix,lpPrefix2->m_str1stPrefix);
				strcat(s_strItemName,strString);
			}
		}

		if (iNounCount	==	2)	//	둘다 명사
		{
			sprintf(strString,"%s %s ",lpPrefix1->m_str2stPrefix,lpPrefix2->m_str1stPrefix);
			strcat(s_strItemName,strString);
		}
	}

//	접두사가 셋일때
	if (iPrefixCount	==	3)
	{
		cITEM_PREFIX	*lpPrefix1	=	&g_aItemPrefix[m_aPrefix[0].m_wPrefix];
		cITEM_PREFIX	*lpPrefix2	=	&g_aItemPrefix[m_aPrefix[1].m_wPrefix];
		cITEM_PREFIX	*lpPrefix3	=	&g_aItemPrefix[m_aPrefix[2].m_wPrefix];

		if (iNounCount	==	0)	//	셋 다 형용사
		{
			sprintf(strString,"%s %s %s ",lpPrefix1->m_str2stPrefix,lpPrefix2->m_str2stPrefix,lpPrefix3->m_str1stPrefix);
			strcat(s_strItemName,strString);
		}

		if (iNounCount	==	1)	//	명사가 하나
		{
			if (lpPrefix1->m_wPrefixForm	==	0)	//	명사
			{
				sprintf(strString,"%s %s %s ",lpPrefix3->m_str2stPrefix,lpPrefix2->m_str1stPrefix,lpPrefix1->m_str1stPrefix);
				strcat(s_strItemName,strString);
			}

			if (lpPrefix2->m_wPrefixForm	==	0)	//	명사
			{
				sprintf(strString,"%s %s %s ",lpPrefix3->m_str2stPrefix,lpPrefix1->m_str1stPrefix,lpPrefix2->m_str1stPrefix);
				strcat(s_strItemName,strString);
			}

			if (lpPrefix3->m_wPrefixForm	==	0)	//	명사
			{
				sprintf(strString,"%s %s %s ",lpPrefix1->m_str2stPrefix,lpPrefix2->m_str1stPrefix,lpPrefix3->m_str1stPrefix);
				strcat(s_strItemName,strString);
			}
		}

		if (iNounCount	==	2)	//	형용사가 하나
		{
			if (lpPrefix1->m_wPrefixForm	==	1)	//	형용사
			{
				sprintf(strString,"%s %s %s ",lpPrefix1->m_str1stPrefix,lpPrefix2->m_str2stPrefix,lpPrefix3->m_str1stPrefix);
				strcat(s_strItemName,strString);
			}

			if (lpPrefix2->m_wPrefixForm	==	1)	//	형용사
			{
				sprintf(strString,"%s %s %s ",lpPrefix2->m_str1stPrefix,lpPrefix1->m_str2stPrefix,lpPrefix3->m_str1stPrefix);
				strcat(s_strItemName,strString);
			}

			if (lpPrefix3->m_wPrefixForm	==	1)	//	형용사
			{
				sprintf(strString,"%s %s %s ",lpPrefix3->m_str1stPrefix,lpPrefix1->m_str2stPrefix,lpPrefix2->m_str1stPrefix);
				strcat(s_strItemName,strString);
			}
		}

		if (iNounCount	==	3)	//	셋 다 명사
		{
			sprintf(strString,"%s %s %s ",lpPrefix1->m_str2stPrefix,lpPrefix2->m_str2stPrefix,lpPrefix3->m_str1stPrefix);
			strcat(s_strItemName,strString);
		}
	}

	strcat(s_strItemName,lpBasicItem->m_strName);

	if (isDXItem() && lpBasicItem->m_attr.isDXItem	==	FALSE)
	{
		char	strName[256];

		sprintf(strName,"DX %s",s_strItemName);
		strcpy(s_strItemName,strName);
	}

	return	s_strItemName;
}	//	cITEM::getName()

//
//	명사의 수
int
cITEM::getNounPrefixCount()
{
	int	iPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	>=	s_iPrefixCount)		break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_wPrefixForm	==	0)	iPrefixCount++;
	}

	return	iPrefixCount;
}	//	cITEM::getNounPrefixCount()


//
//	형용사의 수
int
cITEM::getAdjectivePrefixCount()
{
	int	iPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	>=	s_iPrefixCount)		break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_wPrefixForm	==	1)	iPrefixCount++;
	}

	return	iPrefixCount;
}	//	cITEM::getAdjectivePrefixCount()

char*
cITEM::getPrefixComment(int _iPrefixIndex)
{
	if (_iPrefixIndex>=dITEM_PREFIX_COUNT)
	{
		ERRMSG("아이템 접두사 얻기 에러!!","%d 번째 접두사의 정보를 요청했습니다.\n\n제작자에게 문의해 주세요.",_iPrefixIndex);

		return	NULL;
	}

	CItemPrefixInfo	*lpPrefix	=	&m_aPrefix[_iPrefixIndex];

	if (lpPrefix->m_wPrefix	>=	s_iPrefixCount)
		return	NULL;

	return	g_itemPrefix.getComment(lpPrefix);
}

BOOL
cITEM::isExtraItem()
{
	cBASIC_ITEM	*lpItem	=	getBasicItem();

	if (!lpItem)	
		return	FALSE;

	if (isBreedingRecordBook())
		return	FALSE;

	if (lpItem->m_attr.isExtraItem == TRUE)
		return	TRUE;

	return	FALSE;
}

BOOL
cITEM::isDXItem()
{
	cBASIC_ITEM	*lpItem	=	getBasicItem();

	if (!lpItem)
		return	FALSE;

	if (isBreedingRecordBook())
		return	FALSE;

	if (lpItem->m_attr.isDXItem == TRUE)
		return	TRUE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	>=	s_iPrefixCount)
			break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];
		if (lpPrefix->m_wIsDXPrefix)
			return	TRUE;
	}

	return	FALSE;
}


cSetItem*
cITEM::getSetItem()
{
	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if	(!lpSpecialItem || lpSpecialItem->m_bf1IsSetItem == FALSE)
		return	NULL;

	cItemSet		*lpItemSet		=	g_itemSetManager.get(lpSpecialItem->m_bf11SValue);

	if	(!lpItemSet)
		return	NULL;

	return	lpItemSet->getItemByBaseItem(m_wBaseItem);
}


char*
cITEM::getBookName()
{
	cBookItemDefine			*lpBook		=	getBook();
	cBookPageItemDefine		*lpPage		=	getAddPageToBookItem();
	cBookCategoryItemDefine	*lpCategory	=	getAddCategoryToBookItem();

	int	iBookIndex	=	-1;

	if	(lpBook)
		iBookIndex	=	lpBook->m_wBookIndex;
	if	(lpPage)
		iBookIndex	=	lpPage->m_wBookIndex;
	if	(lpCategory)
		iBookIndex	=	lpCategory->m_wBookIndex;

	if	(iBookIndex	!=	-1)
		return	g_book.getBookName(iBookIndex);

	return	"";
}

cBookItemDefine*
cITEM::getBook()
{
	if	(isBook())
		return	(cBookItemDefine	*)this;

	return	NULL;
}


BOOL
cITEM::isBook()
{
	cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)	
		return	FALSE;

	if	(lpBasicItem->m_aGenerateData[0].m_wEffect	==	eIE_BOOK)
		return	TRUE;

	return	FALSE;
}

BOOL
cITEM::isAddPageToBookItem()
{
	cBASIC_ITEM	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_ADD_PAGE)
		return	TRUE;

	return	FALSE;
}

cBookPageItemDefine*
cITEM::getAddPageToBookItem()
{
	if	(isAddPageToBookItem())
		return	(cBookPageItemDefine	*)this;

	return	NULL;
}

BOOL
cITEM::isAddCategoryToBookItem()
{
	cBASIC_ITEM	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(lpItem->m_aGenerateData[0].m_wEffect	==	eIE_ADD_CATEGORY)
		return	TRUE;

	return	FALSE;

}

cBookCategoryItemDefine*
cITEM::getAddCategoryToBookItem()
{
	if	(isAddCategoryToBookItem())
		return	(cBookCategoryItemDefine	*)this;

	return	NULL;
}

int
cITEM::getBeginPageInBook()
{
	cBookPageItemDefine	*lpPage	=	getAddPageToBookItem();

	if	(!lpPage)
		return	0;

	return	g_book.getPageInBook(lpPage->m_wBookIndex,lpPage->m_wCategory,lpPage->m_wBeginPage);
}

int
cITEM::getEndPageInBook()
{
	cBookPageItemDefine	*lpPage	=	getAddPageToBookItem();

	if	(!lpPage)
		return	0;

	return	g_book.getPageInBook(lpPage->m_wBookIndex,lpPage->m_wCategory,lpPage->m_wEndPage);
}


BOOL
cITEM::isEnableJob(int _iJob)
{
	cSetItem		*lpSetItem		=	getSetItem();

	if	(!lpSetItem	||	lpSetItem->m_uiEnableJobMask == 0)
	{
		cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

		return	lpBasicItem->isEnableJob(_iJob);
	}

	return	lpSetItem->isEnableJob(_iJob);
}