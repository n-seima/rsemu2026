#include "cShop.h"
#include "cNemo.h"

cPegeionPostShop	g_pigeonPostShop;

BOOL
cPegeionPostShop::initForDev()
{
#ifdef	_IS_DEV_CLIENT

	CDecoder	decoder;

	if	(!decoder.Upload("dev data/scripter/pigeonPostShop.txt",NULL))
		return	FALSE;

	m_iItemCount	=	0;
	
	while(1)
	{
		char	*lpstrWord	=	decoder.getStream();

		if	(STRICMP(lpstrWord,"end")	==	0)
			break;

		cPegeionPostShopItem	*lpItem	=	&m_aItemList[m_iItemCount];
		int						iItem	=	cItem::GetItemIndexByName(lpstrWord);

		if	(iItem	==	0xffff)
			return	ERRMSG("에러!!","cPegeionPostShop::init\n\n'%s'라는 아이템을 찾을 수 없다.",lpstrWord);

		lpItem->m_wBasicItem		=	iItem;
		lpItem->m_wCount			=	decoder.getNumber();
		lpItem->m_iPrice			=	decoder.getNumber();
		lpItem->m_awOption[0]		=	decoder.getNumber();
		lpItem->m_awOption[1]		=	decoder.getNumber();

		m_iItemCount++;

		cBasicItem	*lpBasicItem	=	&g_aBasicItem[iItem];

		for (int j=0;j<dGENERATE_ITEM_DATA_COUNT;j++)
		{
			cITEM_GENERATE_DATA	*lpGenerateData	=	&lpBasicItem->m_aGenerateData[j];

			if	(lpGenerateData->m_wEffect	==	0xffff)
				break;

			int	iReferenceIndex,iMinValue,iMaxValue,iRange;

			iReferenceIndex	=	lpGenerateData->m_aValue[0];
			iMinValue		=	min(lpBasicItem->m_aValue[iReferenceIndex][0],255);
			iMaxValue		=	min(lpBasicItem->m_aValue[iReferenceIndex][1],255);
			iMaxValue		=	max(iMaxValue,iMinValue);

			iRange			=	iMaxValue-iMinValue+1;

			if	(lpItem->m_awOption[iReferenceIndex]	>	iMaxValue	||	lpItem->m_awOption[iReferenceIndex]	<	iMinValue)
			{
				cMSG::Put("잘못된 아이템 수치","[%d]아이템 %s\n\n범위 %d~%d\n\n수치 %d",iReferenceIndex,lpBasicItem->m_strName,iMinValue,iMaxValue,lpItem->m_awOption[iReferenceIndex]);
				
			}
		}
	}

	cFILE	file;

	file.Open("data/pigeionPostShop.dat","wb");
	file.Write(&g_pigeonPostShop,sizeof(g_pigeonPostShop));
	file.Close();

#endif
	return	TRUE;
}

BOOL
cPegeionPostShop::init()
{
	cFILE	file;

	if	(!file.Open("data/pigeionPostShop.dat","rb"))
		return	FALSE;

	file.Read(&g_pigeonPostShop,sizeof(g_pigeonPostShop));
	file.Close();

	return	TRUE;
}

cPegeionPostShopItem*
cPegeionPostShop::get(int _iIndex)
{
	if	(_iIndex	<	0	||	_iIndex	>=	m_iItemCount)
		return	NULL;

	if	(m_aItemList[_iIndex].m_wBasicItem	==	0xffff)
		return	NULL;

	return	&m_aItemList[_iIndex];
}

BOOL
cPegeionPostShop::getItemForShop(cItem *_lpItem,int _iIndex,int _iCharge)
{
	cPegeionPostShopItem*	lpPostItem	=	get(_iIndex);

	if	(!lpPostItem)
		return	FALSE;

	_lpItem->reset();
	_lpItem->m_wBaseItem	=	lpPostItem->m_wBasicItem;
	_lpItem->m_dwSerial		=	lpPostItem->m_iPrice+lpPostItem->m_iPrice*_iCharge/100;
	_lpItem->m_aOption[0]	=	lpPostItem->m_awOption[0];
	_lpItem->m_aOption[1]	=	lpPostItem->m_awOption[1];
	_lpItem->m_bCount		=	lpPostItem->m_wCount;
	_lpItem->m_bDurability	=	1;

	return	TRUE;
}