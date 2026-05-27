#include "cITEM.H"
#include <atlbase.h>
#include <stdio.h>
#include "cITEM_PREFIX.h"
#include "CMessage.h"
#include "arcaDefine.h"
#include "book.h"
#include "mess_sign.h"

int		cITEM::s_iCount	=	0;
int		cITEM::s_iPrefixCount;
int		cITEM::s_iPremiumItemCount;
int		cITEM::s_iCustomItemCount;
BOOL	cITEM::s_bIsFindIncorrectPrefix	=	FALSE;

cBASIC_ITEM		g_aBasicItem[dBASIC_ITEM_COUNT];
cITEM_PREFIX	g_aItemPrefix[dMAX_ITEM_PREFIX_COUNT];
CCustomItem		g_aPremiumItem[1024];
CCustomItem		g_aCustomItem[1024];
cItemSetManager	g_itemSetManager;

BOOL
cITEM::ReadItemData()
{
	if	(s_iCount)
		return	TRUE;

	char		strDataFolder[512];

	sprintf(strDataFolder,"%s/data/scenario/red stone",g_strGameFolder);

	if (!SetCurrentDirectory(strDataFolder))
		return	FALSE;

	FILE	*fp;
	int		iCount;

	fp	=	fopen(dITEM_FILE_NAME,"rb");

	if (!fp)
		return	FALSE;

	int	iVersion	=	-1,iNation;

	{
		fseek(fp,-8,SEEK_END);

		int	iAddVersionChecker	=	dITEM_VERSION_CHECKER;

		fread(&iAddVersionChecker,sizeof(iAddVersionChecker),1,fp);

		if	(iAddVersionChecker	==	dITEM_VERSION_CHECKER)
			fread(&iVersion,sizeof(iVersion),1,fp);

		fseek(fp,0,SEEK_SET);
	}

	fread(&iNation,sizeof(iNation),1,fp);
	cMessSign::SetDataEncodeTable(iNation);

	fread(&s_iCount,4,1,fp);

	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		cMessSign::DecodeDataBuffer2(&s_iCount,4);

	fseek(fp,4,SEEK_CUR);
	fread(g_aBasicItem,sizeof(cBASIC_ITEM)*s_iCount,1,fp);

	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		for(int i=0;i<s_iCount;i++)
			cMessSign::DecodeDataBuffer2(g_aBasicItem+i,sizeof(cBASIC_ITEM));
		
	fread(&s_iPrefixCount,4,1,fp);

	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		cMessSign::DecodeDataBuffer2(&s_iPrefixCount,4);

	int	i;

	for (i=0;i<s_iPrefixCount;i++)
	{
		cITEM_PREFIX	prefix;

		fread(&prefix,sizeof(cITEM_PREFIX),1,fp);//	아이템 접두사

		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
			cMessSign::DecodeDataBuffer2(&prefix,sizeof(cITEM_PREFIX));

		memcpy(&g_aItemPrefix[prefix.m_wSerial],&prefix,sizeof(cITEM_PREFIX));
	}

	fread(&s_iPremiumItemCount,4,1,fp);
	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		cMessSign::DecodeDataBuffer2(&s_iPremiumItemCount,4);

	fread(g_aPremiumItem,s_iPremiumItemCount*sizeof(CCustomItem),1,fp);
	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		for (i=0;i<s_iPremiumItemCount;i++)
			cMessSign::DecodeDataBuffer2(g_aPremiumItem+i,sizeof(CCustomItem));

	fread(&s_iCustomItemCount,4,1,fp);
	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		cMessSign::DecodeDataBuffer2(&s_iCustomItemCount,4);
	fread(g_aCustomItem,s_iCustomItemCount*sizeof(CCustomItem),1,fp);
	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		for (i=0;i<s_iCustomItemCount;i++)
			cMessSign::DecodeDataBuffer2(g_aCustomItem+i,sizeof(CCustomItem));

	fread(&iCount,4,1,fp);
	if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
		cMessSign::DecodeDataBuffer2(&iCount,4);

	if	(iCount)	
		fseek(fp,sizeof(cArcaDefine)*iCount,SEEK_CUR);

	fread(&iCount,4,1,fp);

//	세트 아이템 로딩
	{
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

	//	조각 아이템 로딩
// 		fread(&iCount,4,1,fp);
// 
// 		if	(iVersion	>=	eITEMDATAVERSION_ENCRYPT)
// 			cMessSign::DecodeDataBuffer(&iCount,4);
// 
// 		if (iCount)	
// 		{
// 			g_pieceItemManager.init(iCount);
// 
// 			cPieceItem*	lpPieceItemBay	=	g_pieceItemManager.getBay();
// 
// 			file.Read(lpPieceItemBay,sizeof(cPieceItem)*iCount);
// 			g_pieceItemManager.build();
// 		}

	}

	fclose(fp);
	return FALSE;
}

//
//	아이템 이름
BOOL
cITEM::isHighLevelMagicOptionItem(int _iLevel)
{
	if	(isBreedingRecordBook() || isCoupleRing() || isExceptionItem())
		return	FALSE;

	int	i,iPrefix;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		iPrefix		=	m_aPrefix[i].m_wPrefix;

		if (iPrefix	==	0xffff)	return	FALSE;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

		if (lpPrefix->m_wDropLevel	>=	_iLevel)	return	TRUE;
	}

	return	FALSE;
}

BOOL
cITEM::isPremiumItem()
{
	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	FALSE;

	cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	if	(lpBasicItem->m_wKind	==	eIK_PREMIUM_ITEM	||	isExtraItem())
		return	TRUE;

	return	FALSE;
}

void
cITEM::increaseMinute(int _iMinute)
{
	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return;

	if	(isSpecialItem())
		return;

	for (int iTime=0;iTime<_iMinute;iTime++)
	{
		m_minute++;

		if (m_minute	>=	60)
		{
			m_minute	=	0;
			m_hour++;

			if (m_hour	>=	24)
			{
				m_hour	=	0;
				
				int	iLastDay=	GetLastDay(m_year,m_month);

				if (m_day+1	>	(DWORD)iLastDay)
				{
					m_day	=	1;
					m_month++;

					if (m_month	>	12)
					{
						m_month	=	1;
						m_year++;
					}
				}
				else	m_day++;
			}
		}
	}
}

BOOL
cITEM::isDXItem()
{
	cBASIC_ITEM	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	FALSE;

	if	(lpItem->m_attr.isDXItem == TRUE)
		return	TRUE;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if	(lpPrefix->m_wIsDXPrefix)
			return	TRUE;
	}

	return	FALSE;
}

//
//
char*
cITEM::getExpireDate()
{
	static	char	s_strExpireDate[512];

	strcpy(s_strExpireDate,"../../..");

	if	(isPremiumItem()	&&	m_year)
		sprintf(s_strExpireDate,"%.2d/%.2d/%.2d",m_year,m_month,m_day);

	if	(isCoupleRing())
	{
		cCoupleRingDefine	*lpCoupleRing	=	(cCoupleRingDefine	*)this;

		if	(lpCoupleRing->m_year)
			sprintf(s_strExpireDate,"%.2d/%.2d/%.2d",lpCoupleRing->m_year,lpCoupleRing->m_month,lpCoupleRing->m_day);
	}

	return	s_strExpireDate;
}

//
//	아이템 이름
char*
cITEM::getName()
{
	s_bIsFindIncorrectPrefix	=	FALSE;

	static	char	s_strItemName[512];

	char	strString[512];

	s_strItemName[0]	=	NULL;

	cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)
		return	NULL;

	if	(isBreedingRecordBook()  || isCoupleRing())
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

	if	(isExceptionItem() && lpBasicItem->isEternalItem() == FALSE)
		return	lpBasicItem->m_strName;

	if	(!isRareItem() && !isUniqueItem())
	{
		if	(m_isBroken					)
			sprintf(s_strItemName,"%s %s",dMSG_BROKEN,lpBasicItem->m_strName);
		else
		if	(isSpecialItem() == FALSE && m_isCanNotAttachPrefixItem	)
			sprintf(s_strItemName,"%s %s",dMSG_CAN_NOT_IMPROVE,lpBasicItem->m_strName);
		else
			strcpy(s_strItemName,lpBasicItem->m_strName);

		return	s_strItemName;
	}

			if (isUniqueItem() && isRareItem())	strcpy(s_strItemName,"(S) ");
	else	if (isUniqueItem())	strcpy(s_strItemName,"(U) ");
	else	if (isRareItem())	strcpy(s_strItemName,"(R) ");

	if	(m_isBroken					)
		strcat(s_strItemName,dMSG_BROKEN);
	else
	if	(isSpecialItem() == FALSE && m_isCanNotAttachPrefixItem	)
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

		sprintf(strString,"%s ",lpPrefix->m_str1stPrefix);
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
}

int
cITEM::getPrefixCount()
{
	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	0;

	int	iCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(m_aPrefix[i].m_wPrefix == 0xffff)
			break;

		iCount++;
	}

	return	iCount;
}

void
cITEM::repairItemIncorrectInfo()
{
	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if	(m_aPrefix[i].m_wPrefix == 0xffff)
		{
			memset(&m_aPrefix[i],0xff,sizeof(CItemPrefixInfo)*(dITEM_PREFIX_COUNT-i));
			break;
		}
	}
}

//
//	명사의 수
int
cITEM::getNounPrefixCount()
{
	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	0;

	int	iPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)		break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_wPrefixForm	==	0)	
			iPrefixCount++;
	}

	return	iPrefixCount;
}

BOOL
cITEM::isIncorrectItem()
{
	if	(m_wBaseItem	==	0	)
	{
		reset();

		return	FALSE;
	}

	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
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

//
//	형용사의 수
int
cITEM::getAdjectivePrefixCount()
{
	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	0;

	int	iPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)		break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];

		if (lpPrefix->m_wPrefixForm	==	1)	iPrefixCount++;
	}

	return	iPrefixCount;
}

//
//
BOOL
cITEM::refitItemData()
{
	if (m_wBaseItem	==	0xffff)
		return FALSE;

	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	0;

	BOOL	bIsRefit	=	FALSE;
	int		iRefitCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];
		cBASIC_ITEM		*lpBasicItem=	getBasicItem();

		if (lpPrefix->m_aStickableItem[lpBasicItem->m_wKind] == TRUE)
		{
			i++;
			continue;
		}

		bIsRefit	=	TRUE;
		iRefitCount++;

		if (i<dITEM_PREFIX_COUNT-1)
		{
			memcpy(&m_aPrefix[i],&m_aPrefix[i+1],sizeof(CItemPrefixInfo)*(dITEM_PREFIX_COUNT-i-1));

			memset(&m_aPrefix[dITEM_PREFIX_COUNT-1],0,sizeof(CItemPrefixInfo));	//	마지막에 있는거 초기화
			m_aPrefix[dITEM_PREFIX_COUNT-1].m_wPrefix	=	0xffff;
		}
		else
		{
			memset(&m_aPrefix[i],0,sizeof(CItemPrefixInfo));	//	초기화
			m_aPrefix[i].m_wPrefix	=	0xffff;
		}
	}

	return	bIsRefit;
}

//
//
int
cITEM::getIncorrectPrefixCount()
{
	if	(m_wBaseItem	==	0xffff)
		return	-1;

	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	0;

	BOOL	iIncorrectPrefixCount	=	0;

	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)		break;

		if (cITEM::s_iPrefixCount <= m_aPrefix[i].m_wPrefix)
		{
#ifdef	_FOR_KOREA
			cMSG::Put("얼래?","아이템 접두사가 문제 있어효. -o-");
#else
			cMSG::Put("error!!","incorrect item prefix!");
#endif
			return	-1;
		}
		
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[m_aPrefix[i].m_wPrefix];
		cBASIC_ITEM		*lpBasicItem=	getBasicItem();

		if (!lpBasicItem)
		{
#ifdef	_FOR_KOREA
			cMSG::Put("얼래?","아이템 기본 인덱스가 문제 있어효. -o-");
#else
			cMSG::Put("error!!","incorrect base item!");
#endif
			return	-1;
		}

		if (lpPrefix->m_aStickableItem[lpBasicItem->m_wKind] == FALSE)
		{
			iIncorrectPrefixCount++;

#ifdef	_FOR_KOREA
			cMSG::Put("얼래?","'%s'에는 접두사 %s[인덱스 %d]를 붙일 수 없어요.",lpBasicItem->m_strName,lpPrefix->m_str1stPrefix,m_aPrefix[i].m_wPrefix);
#else
			cMSG::Put("warning","can not attatch prefix '%s'[%d] front on item '%s'",lpPrefix->m_str1stPrefix,m_aPrefix[i].m_wPrefix,lpBasicItem->m_strName);
#endif
		}
	}

	return	iIncorrectPrefixCount;
}

BOOL
cITEM::isExceptionItem()
{
	cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

	if	(!lpBasicItem)
		return	FALSE;

	return	lpBasicItem->m_bIsExceptionItem;
}
//
//	무한 탄환이냐?
BOOL
cITEM::isInfinityBullet()
{
	int			i;
	cBASIC_ITEM	*lpBasicItem	=	getBasicItem();

	if (!lpBasicItem)
		return	FALSE;

	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	FALSE;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_aPrefix[i].m_wPrefix	==	0xffff)	continue;

		if (g_aItemPrefix[m_aPrefix[i].m_wPrefix].m_wType==	eIP_INFINITY_BULLET)	return	TRUE;
	}

	for (i=0;i<dGENERATE_ITEM_DATA_COUNT;i++)
		if (lpBasicItem->m_aGenerateData[i].m_wEffect	==	eIE_INFINITY_BULLET)	
			return	TRUE;

	for (i=0;i<dITEM_UNIQUE_DATA_COUNT;i++)
		if (lpBasicItem->m_aUniqueData[i].m_wEffect		==	eIP_INFINITY_BULLET)	
			return	TRUE;

	return	FALSE;
}

BOOL
cITEM::isExtraItem()
{
	cBASIC_ITEM	*lpItem	=	getBasicItem();

	if	(!lpItem)	
		return	FALSE;

	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	FALSE;

	if	(lpItem->m_attr.isExtraItem == TRUE)
		return	TRUE;

	return	FALSE;
}

//
//	접두사로 인해 착용/사용에 필요한 레벨
int
cITEM::getRequireLevelByPrefix()
{
	if	(isBreedingRecordBook()  || isCoupleRing() || isBook() || isExceptionItem())
		return	0;

	int	iPrefixLevel	=	0,i;
	int	aPrefixLevel[3]	=	{0,0,0};
	int	iMaxPrefixIndex	=	0;

	for (i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		CItemPrefixInfo	*lpPrefixInfo	=	&m_aPrefix[i];

		if (lpPrefixInfo->m_wPrefix	==	0xffff)
			break;

		cITEM_PREFIX		*lpPrefix		=	&g_aItemPrefix[lpPrefixInfo->m_wPrefix];

		if (lpPrefix->m_wDropLevel	>	iPrefixLevel)
		{
			iPrefixLevel	=	lpPrefix->m_wDropLevel;
			iMaxPrefixIndex	=	i;
		}

		aPrefixLevel[i]	=	lpPrefix->m_wDropLevel;
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

	return	iPrefixLevel;
}

int
cITEM::getRequireLevel()
{
	cBASIC_ITEM	*lpBasicItem		=	getBasicItem();

	if (!lpBasicItem)
		return	0;

	int				iRequireLevel	=	lpBasicItem->m_wRequireLevel+getRequireLevelByPrefix();

	cSpecialItem	*lpSpecialItem	=	getSpecialItem();

	if (!lpSpecialItem || lpSpecialItem->m_bf10EquipLevel == 0)
		return	iRequireLevel;

	return	lpSpecialItem->m_bf10EquipLevel;
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


cBreedingRecordBookDefine*
cITEM::getBreedingRecordBookPetInfo()
{
	if	(!isBreedingRecordBook())
		return	NULL;

	return	(cBreedingRecordBookDefine	*)(((char *)this)+6);
}

char*
cITEM::getItemCode(BOOL _bIsIncludeName)
{
	static	char	strItemCode[512];

	if	(_bIsIncludeName)
	{
		sprintf(strItemCode,"%s [%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
								getName(),m_wBaseItem,m_dwSerial,m_bCount,m_bDurability,
								m_aOption[0],m_aOption[1],
								m_aPrefix[0].m_wPrefix,m_aPrefix[0].m_aValue[0],m_aPrefix[0].m_aValue[1],
								m_aPrefix[1].m_wPrefix,m_aPrefix[1].m_aValue[0],m_aPrefix[1].m_aValue[1],
								m_aPrefix[2].m_wPrefix,m_aPrefix[2].m_aValue[0],m_aPrefix[2].m_aValue[1],
								getBitFieldValue());

	}
	else
	{
		sprintf(strItemCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
								m_wBaseItem,m_dwSerial,m_bCount,m_bDurability,
								m_aOption[0],m_aOption[1],
								m_aPrefix[0].m_wPrefix,m_aPrefix[0].m_aValue[0],m_aPrefix[0].m_aValue[1],
								m_aPrefix[1].m_wPrefix,m_aPrefix[1].m_aValue[0],m_aPrefix[1].m_aValue[1],
								m_aPrefix[2].m_wPrefix,m_aPrefix[2].m_aValue[0],m_aPrefix[2].m_aValue[1],
								getBitFieldValue());
	}
	return	strItemCode;
}
