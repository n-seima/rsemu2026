#ifndef _classShop_h
#define	_classShop_h

#include "SFC.H"
#include "cItem.H"

#define	dSHOP_NAME_LENGTH		32
#define	dMAX_ITEM_IN_SHOP		50

//
//	가게-o-r~~
class	CShop
{
public:
	char		m_strName[dSHOP_NAME_LENGTH];//	가게 이름
	WORD		m_wItemCount;
	WORD		m_wOwner;
	WORD		m_wSerial;
	WORD		m_wPriceFactor;
	DWORD		m_dwLimitPrice;
	cItem		m_aSellItem[dMAX_ITEM_IN_SHOP];		//	판매 할 아이템 정보

				CShop()
				{
					m_wItemCount		=	0;
				}	//	CShop::CShop()

				~CShop()
				{
					reset();
				}	//	CShop::~CShop()

	void		reset()//초기화
	{
		m_wItemCount	=	0;
		strcpy(m_strName,"");
	}
};	//	class	CShop

class	cPegeionPostShopItem
{
public:
	WORD		m_wBasicItem;
	WORD		m_wCount;
	int			m_iPrice;
	WORD		m_awOption[2];

	cPegeionPostShopItem()
	{
		m_wBasicItem	=	0xffff;
	}
};

class	cPegeionPostShop
{
public:
	int						m_iItemCount;
	cPegeionPostShopItem	m_aItemList[100];

	BOOL					init();
	BOOL					initForDev();
	cPegeionPostShopItem*	get(int _iIndex);
	BOOL					getItemForShop(cItem *_lpItem,int _iIndex,int _iCharge);
};

extern	cPegeionPostShop	g_pigeonPostShop;

#endif