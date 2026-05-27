// carrotShop.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "carrotShop.h"
#include "cITEM.h"
#include "cPACKET_WORLDSERVER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

cCarrotShopItem	*
cCarrotShopCategoryInfo::getItem(int _iIndex)
{
	return	(cCarrotShopItem	*)&m_aItemList[_iIndex];
}

void
cCarrotShopCategoryInfo::reset()
{
	m_wItemCount	=	0;
}

void
cCarrotShopInfo::reset()
{
	m_wCategoryCount	=	0;
}

BOOL
cCarrotShopInfo::isFull()
{
	if	(m_wCategoryCount	>=	dMAX_CARROT_SHOP_CATEGORY_COUNT)
		return	TRUE;

	return	FALSE;
}


cCarrotShopCategoryInfo*
cCarrotShopInfo::getByName(char *_lpstrName)
{
	for (int i=0;i<m_wCategoryCount;i++)
	{
		if	(stricmp(m_aData[i].m_strName,_lpstrName)	==	0)
			return	(cCarrotShopCategoryInfo*)&m_aData[i];
	}

	return	NULL;	
}

cCarrotShopCategoryInfo*
cCarrotShopInfo::getCategory(int _iIndex)
{
	return	(cCarrotShopCategoryInfo*)&m_aData[_iIndex];
}

/////////////////////////////////////////////////////////////////////////////
// cCarrotShop property page

IMPLEMENT_DYNCREATE(cCarrotShop, CPropertyPage)

cCarrotShop::cCarrotShop() : CPropertyPage(cCarrotShop::IDD)
{
	//{{AFX_DATA_INIT(cCarrotShop)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

cCarrotShop::~cCarrotShop()
{
}

void cCarrotShop::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cCarrotShop)
	DDX_Control(pDX, IDC_PRICE, m_editPrice);
	DDX_Control(pDX, IDC_COUNT, m_editCount);
	DDX_Control(pDX, IDC_PREMIUM_ITEM_LIST, m_cbPremiumItem);
	DDX_Control(pDX, IDC_CATEGORY_NAME, m_editCategoryName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cCarrotShop, CPropertyPage)
	//{{AFX_MSG_MAP(cCarrotShop)
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_ADD_CARROT_ITEM, OnAddCarrotItem)
	ON_BN_CLICKED(IDC_REMOVE_CARROT_ITEM, OnRemoveCarrotItem)
	ON_BN_CLICKED(IDC_EDIT_CARROT_ITEM, OnEditCarrotItem)
	ON_BN_CLICKED(IDC_RESET_CARROT_ITEM_CONTENTS, OnResetCarrotItemContents)
	ON_BN_CLICKED(IDC_APPLY_CARROT_ITEM, OnApplyCarrotItem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CATEGORY, OnItemchangedCategory)
	ON_BN_CLICKED(IDC_REMOVE_PACK, OnRemovePack)
	ON_BN_CLICKED(IDC_CHANGE_NAME, OnChangeName)
	ON_BN_CLICKED(IDC_MOVE_UP_SELECT_CATEGORY, OnMoveUpSelectCategory)
	ON_BN_CLICKED(IDC_MOVE_DOWN_SELECT_CATEGORY, OnMoveDownSelectCategory)
	ON_BN_CLICKED(IDC_MOVE_UP_SELECT_ITEM, OnMoveUpSelectItem)
	ON_BN_CLICKED(IDC_MOVE_DOWN_SELECT_ITEM, OnMoveDownSelectItem)
	ON_BN_CLICKED(IDC_SAVE_TO_FILE, OnSaveToFile)
	ON_BN_CLICKED(IDC_LOAD_FROM_FILE, OnLoadFromFile)
	ON_BN_CLICKED(IDC_PASTE_CATEGORY, OnPasteCategory)
	ON_BN_CLICKED(IDC_COPY_CATEGORY, OnCopyCategory)
	ON_BN_CLICKED(IDC_ASK_CARROT_INFO, OnAskCarrotInfo)
	ON_BN_CLICKED(IDC_MODIFY_CARROT_SHOP_DATA, OnModifyCarrotShopData)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CARROT_ITEM_LIST, OnItemchangedCarrotItemList)
	ON_BN_CLICKED(IDC_APPLY_NOW, OnApplyNow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cCarrotShop message handlers

BOOL cCarrotShop::OnInitDialog() 
{
	int		i;

	CPropertyPage::OnInitDialog();

	m_shopInfo.reset();

	m_bIsReceiveData		=	FALSE;
	m_bIsWaitToReceiveData	=	FALSE;

	m_iSelectedCategory		=	-1;
	m_iSelectedItem			=	0;

	m_listCategory.init(GetDlgItem(IDC_CATEGORY)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	m_listCategory.addColumn(0,30,"NO");
	m_listCategory.addColumn(1,130,"Name");
	m_listCategory.addColumn(2,24,"T");
	m_listCategory.addColumn(3,24,"H");
	m_listCategory.addColumn(4,24,"R");
	m_listCategory.reset();

	m_listCarrotShopItem.init(GetDlgItem(IDC_CARROT_ITEM_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	m_listCarrotShopItem.addColumn(0,30,"NO");
	m_listCarrotShopItem.addColumn(1,50,"Index");
	m_listCarrotShopItem.addColumn(2,260,"Name");
	m_listCarrotShopItem.addColumn(3,50,"Count");
	m_listCarrotShopItem.addColumn(4,50,"Price");
	m_listCarrotShopItem.addColumn(5,24,"T");
	m_listCarrotShopItem.addColumn(6,24,"H");
	m_listCarrotShopItem.addColumn(7,24,"R");

	m_editCategory.reset();

	setVirgin();

	m_editCount.SetWindowText("1");
	m_editPrice.SetWindowText("10");

	for	(i=0;i<cITEM::s_iPremiumItemCount;i++)
	{
		CCustomItem	*lpCustomItem	=	&g_aPremiumItem[i];

		if	(lpCustomItem->m_dwSerial	==	0xffffffff)
			continue;

		int	iIndex	=	m_cbPremiumItem.AddString(g_aBasicItem[lpCustomItem->m_wBaseItem].m_strName);

		m_cbPremiumItem.SetItemData(iIndex,i);
	}

	m_cbPremiumItem.SetCurSel(0);
	
	updateCategoryList();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cCarrotShop::OnCreate() 
{
	char	strName[dCARROT_SHOP_CATEGORY_NAME_LENGTH];

	m_editCategoryName.GetWindowText(strName,sizeof(strName)-1);
	
	if	(strlen(strName) <= 0)
	{
		cMSG::Error("error in create new Category","invalid name");
		return;
	}

	if	(m_shopInfo.getByName(strName))
	{
		cMSG::Error("error in create new Category","exist same name category");
		return;
	}

	if	(m_shopInfo.isFull())
	{
		cMSG::Error("error in create new Category","category count full!!");

		return;
	}

	cCarrotShopCategoryInfo	*lpCategory	=	m_shopInfo.getCategory(m_shopInfo.m_wCategoryCount);

	strcpy(lpCategory->m_strName,strName);

	lpCategory->m_bf1IsTestCategory		=	IsDlgButtonChecked(IDC_TEST_CATE);
	lpCategory->m_bf1IsHotCategory		=	IsDlgButtonChecked(IDC_HOT_CATE);
	lpCategory->m_bf1IsRecommandCategory=	IsDlgButtonChecked(IDC_RECOMMAND_CATE);
	lpCategory->m_wItemCount			=	0;
	m_iSelectedCategory					=	m_shopInfo.m_wCategoryCount;

	m_shopInfo.m_wCategoryCount++;
	m_editCategory.reset();
	
	updateCategoryList();
}

void cCarrotShop::updateCategoryList()
{
	m_listCategory.reset();

	if	(m_shopInfo.m_wCategoryCount	==	0)
	{
		m_listCategory.addItem(0,0,"",0);
		m_listCategory.setItem(0,1,"invalid List");

		m_editCategory.reset();

		updateItemList();
		
		return;
	}

	int		iCount		=	0;
	char	*strOX[]	=	{"X" ,"O"};
	
	for (int i=0;i<m_shopInfo.m_wCategoryCount;i++)
	{
		m_listCategory.addItem(i,0,_ms("%.3d",i),i);
		m_listCategory.setItem(i,1,m_shopInfo.m_aData[i].m_strName);

		m_listCategory.setItem(i,2,strOX[m_shopInfo.m_aData[i].m_bf1IsTestCategory]);
		m_listCategory.setItem(i,3,strOX[m_shopInfo.m_aData[i].m_bf1IsHotCategory]);
		m_listCategory.setItem(i,4,strOX[m_shopInfo.m_aData[i].m_bf1IsRecommandCategory]);
	}

	m_listCategory.selectItem(m_iSelectedCategory);

	updateItemList();
}

void cCarrotShop::updateItemList()
{
	m_listCarrotShopItem.reset();

	if	(m_iSelectedCategory	==	-1	||	m_editCategory.m_wItemCount	==	0	||	m_shopInfo.m_wCategoryCount	==	0)
	{
		m_listCarrotShopItem.addItem(0,0,"");
		m_listCarrotShopItem.setItem(0,1,"");
		m_listCarrotShopItem.setItem(0,2,"invalid list");

		return;
	}

	char	*strOX[]	=	{"X" ,"O"};

	for (int i=0;i<m_editCategory.m_wItemCount;i++)
	{
		cCarrotShopItem	*lpCarrotShopItem	=	m_editCategory.getItem(i);
		int				iPremiumItemIndex	=	lpCarrotShopItem->m_wPremiunItemIndex;
		CCustomItem		*lpPremiumItem		=	&g_aPremiumItem[iPremiumItemIndex];

		m_listCarrotShopItem.addItem(i,0,_ms("%.3d",i),i);
		m_listCarrotShopItem.setItem(i,1,_ms("%d",iPremiumItemIndex));
		m_listCarrotShopItem.setItem(i,2,lpPremiumItem->m_strName);
		m_listCarrotShopItem.setItem(i,3,_ms("%d",lpCarrotShopItem->m_bf8ItemCount));
		m_listCarrotShopItem.setItem(i,4,_ms("%d",lpCarrotShopItem->m_wCarrotCount));

		m_listCarrotShopItem.setItem(i,5,strOX[lpCarrotShopItem->m_bf1IsTestItem]);
		m_listCarrotShopItem.setItem(i,6,strOX[lpCarrotShopItem->m_bf1IsHotItem]);
		m_listCarrotShopItem.setItem(i,7,strOX[lpCarrotShopItem->m_bf1IsRecommandItem]);
	}

	m_listCarrotShopItem.selectItem(m_iSelectedItem);

	CheckDlgButton(IDC_TEST_CATE,m_editCategory.m_bf1IsTestCategory);
	CheckDlgButton(IDC_HOT_CATE,m_editCategory.m_bf1IsHotCategory);
	CheckDlgButton(IDC_RECOMMAND_CATE,m_editCategory.m_bf1IsRecommandCategory);
}


void cCarrotShop::OnAddCarrotItem() 
{
	// TODO: Add your control notification handler code here
	if	(m_editCategory.m_wItemCount	>=	dMAX_CARROT_SHOP_ITEM_COUNT)
	{
		cMSG::Error("error in add new item","is full");
		return;
	}

	if	(m_iSelectedCategory	==	-1	||	m_shopInfo.m_wCategoryCount	==	0)
	{
		cMSG::Error("error in add new item","please select category");

		return;
	}
	
	cCarrotShopItem	*lpCarrotShopItem	=	m_editCategory.getItem(m_editCategory.m_wItemCount);
	
	CString	string;

	m_editCount.GetWindowText(string);
	int		iCount	=	atoi(string);

	m_editPrice.GetWindowText(string);
	int		iPrice	=	atoi(string);

	if	(iCount	==	0	||	iPrice	==	0)
	{
		cMSG::Error("error in add new item","please input Count and Price Info");
		return;
	}


	int	iSelect			=	m_cbPremiumItem.GetCurSel();
	int	iPremiumItem	=	m_cbPremiumItem.GetItemData(iSelect);

	m_cbPremiumItem.GetWindowText(string);

	if	(strlen(string)	<=	3)
	{
		if	(atoi(string)	<	cITEM::s_iPremiumItemCount)
		{
			iPremiumItem	=	atoi(string);

			m_cbPremiumItem.SelectString(0,g_aPremiumItem[iPremiumItem].m_strName);
		}
	}

	m_iSelectedItem							=	m_editCategory.m_wItemCount;

	m_editCategory.m_wItemCount++;

	lpCarrotShopItem->m_bf8ItemCount		=	iCount;
	lpCarrotShopItem->m_wPremiunItemIndex	=	iPremiumItem;
	lpCarrotShopItem->m_wCarrotCount		=	iPrice;
	lpCarrotShopItem->m_bf1IsTestItem		=	IsDlgButtonChecked(IDC_ITEM_TEST);
	lpCarrotShopItem->m_bf1IsHotItem		=	IsDlgButtonChecked(IDC_ITEM_HOT);
	lpCarrotShopItem->m_bf1IsRecommandItem	=	IsDlgButtonChecked(IDC_ITEM_RECOMMAND);

	updateItemList();

	lostVirgin();
}

void cCarrotShop::OnRemoveCarrotItem() 
{
	int		iSelectedItem	=	m_listCarrotShopItem.getSelectedItem();

	if (iSelectedItem	==	-1)
	{
		MessageBox("please select item");
		return;
	}

	for (int i=iSelectedItem;i<m_editCategory.m_wItemCount-1;i++)
		memcpy(&m_editCategory.m_aItemList[i],&m_editCategory.m_aItemList[i+1],sizeof(cCarrotShopItemDefine));

	m_editCategory.m_wItemCount--;

	m_iSelectedItem			=	min(iSelectedItem,m_editCategory.m_wItemCount-1);

	updateItemList();

	lostVirgin();
}

void cCarrotShop::OnEditCarrotItem() 
{
	int		iSelectedItem	=	m_listCarrotShopItem.getSelectedItem();

	if	(iSelectedItem	==	-1	||	m_shopInfo.m_wCategoryCount	==	0)
	{
		MessageBox("please select item");
		return;
	}

	m_iSelectedItem	=	iSelectedItem;

	cCarrotShopItem	*lpItem		=	m_editCategory.getItem(iSelectedItem);
	
	CString			string;

	m_editCount.GetWindowText(string);
	int		iCount	=	atoi(string);

	m_editPrice.GetWindowText(string);
	int		iPrice	=	atoi(string);

	if	(iCount	==	0	||	iPrice	==	0)
	{
		cMSG::Error("error in add new item","please input Count and Price Info");
		return;
	}

	int	iSelect			=	m_cbPremiumItem.GetCurSel();
	int	iPremiumItem	=	m_cbPremiumItem.GetItemData(iSelect);

	m_cbPremiumItem.GetWindowText(string);

	if	(strlen(string)	<=	3)
	{
		if	(atoi(string)	<	cITEM::s_iPremiumItemCount)
		{
			iPremiumItem	=	atoi(string);

			m_cbPremiumItem.SelectString(0,g_aPremiumItem[iPremiumItem].m_strName);
		}
	}

	lpItem->m_wCarrotCount		=	iPrice;
	lpItem->m_bf8ItemCount		=	iCount;
	lpItem->m_wPremiunItemIndex	=	iPremiumItem;
	lpItem->m_bf1IsTestItem		=	IsDlgButtonChecked(IDC_ITEM_TEST);
	lpItem->m_bf1IsHotItem		=	IsDlgButtonChecked(IDC_ITEM_HOT);
	lpItem->m_bf1IsRecommandItem=	IsDlgButtonChecked(IDC_ITEM_RECOMMAND);

	lostVirgin();

	updateItemList();
}

void cCarrotShop::OnResetCarrotItemContents() 
{
	// TODO: Add your control notification handler code here

	m_editCategory.reset();

	updateItemList();

	lostVirgin();
}

void cCarrotShop::OnApplyCarrotItem() 
{
	int		iSelectedCategory	=	m_listCategory.getSelectedItem();

	if	(iSelectedCategory		==	-1||	m_shopInfo.m_wCategoryCount	==	0)
	{
		MessageBox("please select cagtegory");
		return;
	}

	char	strName[dCARROT_SHOP_CATEGORY_NAME_LENGTH];

	m_editCategoryName.GetWindowText(strName,sizeof(strName)-2);
	
	if	(strlen(strName) <= 0)
	{
		cMSG::Error("error in change Category name","invalid name");
		return;
	}

	strcpy(m_shopInfo.m_aData[iSelectedCategory].m_strName,strName);
	strcpy(m_editCategory.m_strName,strName);

	m_editCategory.m_bf1IsTestCategory			=	IsDlgButtonChecked(IDC_TEST_CATE);
	m_editCategory.m_bf1IsHotCategory			=	IsDlgButtonChecked(IDC_HOT_CATE);
	m_editCategory.m_bf1IsRecommandCategory		=	IsDlgButtonChecked(IDC_RECOMMAND_CATE);

	cCarrotShopCategoryInfo	*lpSelectCategory	=	m_shopInfo.getCategory(iSelectedCategory);

	memcpy(lpSelectCategory,&m_editCategory,sizeof(cCarrotShopCategoryInfo));

	updateCategoryList();

	setVirgin();
}

void
cCarrotShop::lostVirgin()
{
	m_bIsVirgin	=	FALSE;
}

void
cCarrotShop::setVirgin()
{
	m_bIsVirgin	=	TRUE;
}

BOOL
cCarrotShop::isVirgin()
{
	return	m_bIsVirgin;
}

BOOL
cCarrotShop::isLostVirgin()
{
	return	1-m_bIsVirgin;
}

void cCarrotShop::OnItemchangedCategory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	if	(pNMListView->uOldState	==	0)
	{
		if	(isLostVirgin())
		{
			int	iResult	=	cMSG::YESNO("modified some item data","do you want apply this?");

			if (iResult	==	IDYES)
			{
				cCarrotShopCategoryInfo	*lpSelectCategory	=	m_shopInfo.getCategory(m_iSelectedCategory);

				memcpy(lpSelectCategory,&m_editCategory,sizeof(cCarrotShopCategoryInfo));
			}

			setVirgin();
		}

		m_iSelectedCategory			=	pNMListView->iItem;

		memcpy(&m_editCategory,&m_shopInfo.m_aData[m_iSelectedCategory],sizeof(m_editCategory));

		m_editCategoryName.SetWindowText(m_editCategory.m_strName);

		CheckDlgButton(IDC_TEST_CATE,m_editCategory.m_bf1IsTestCategory);
		CheckDlgButton(IDC_HOT_CATE,m_editCategory.m_bf1IsHotCategory);
		CheckDlgButton(IDC_RECOMMAND_CATE,m_editCategory.m_bf1IsRecommandCategory);

		updateItemList();
	}
}

void cCarrotShop::OnRemovePack() 
{
	int	iSelectedCategory	=	m_listCategory.getSelectedItem();

	if	(iSelectedCategory	==	-1)
	{
		MessageBox("please select category");
		return;
	}

	for (int i=iSelectedCategory;i<m_shopInfo.m_wCategoryCount-1;i++)
		memcpy(&m_shopInfo.m_aData[i],&m_shopInfo.m_aData[i+1],sizeof(cCarrotShopCategoryInfo));

	m_shopInfo.m_wCategoryCount--;

	if	(m_shopInfo.m_wCategoryCount	>	0)
	{
		m_iSelectedCategory	=	min(m_iSelectedCategory,m_shopInfo.m_wCategoryCount-1);

		memcpy(&m_editCategory,&m_shopInfo.m_aData[m_iSelectedCategory],sizeof(m_editCategory));

		m_editCategoryName.SetWindowText(m_editCategory.m_strName);
	}
	
	setVirgin();

	updateCategoryList();
}

void cCarrotShop::OnChangeName() 
{
	char	strName[dCARROT_SHOP_CATEGORY_NAME_LENGTH];

	m_editCategoryName.GetWindowText(strName,sizeof(strName)-2);
	
	if	(strlen(strName) <= 0)
	{
		cMSG::Error("error in change Category name","invalid name");
		return;
	}

	if	(m_shopInfo.getByName(strName))
	{
		cMSG::Error("error in change Category name","exist same name category");
		return;
	}

	int	iSelectedCategory	=	m_listCategory.getSelectedItem();

	if	(iSelectedCategory	==	-1)
	{
		MessageBox("please select category");
		return;
	}

	strcpy(m_shopInfo.m_aData[iSelectedCategory].m_strName,strName);
	strcpy(m_editCategory.m_strName,strName);

	updateCategoryList();
}

void cCarrotShop::OnMoveUpSelectCategory() 
{
	int	iSelectedCategory	=	m_listCategory.getSelectedItem();

	if	(iSelectedCategory	==	-1)
	{
		MessageBox("please select category");
		return;
	}

	if	(iSelectedCategory	==	0)
	{
		MessageBox("it's top");
		return;
	}

	nsSfc::swap(m_shopInfo.m_aData[iSelectedCategory],m_shopInfo.m_aData[iSelectedCategory-1]);

	m_iSelectedCategory	=	iSelectedCategory-1;

	updateCategoryList();
}

void cCarrotShop::OnMoveDownSelectCategory() 
{
	int	iSelectedCategory	=	m_listCategory.getSelectedItem();

	if	(iSelectedCategory	==	-1)
	{
		MessageBox("please select category");
		return;
	}
	if	(iSelectedCategory	==	m_shopInfo.m_wCategoryCount-1)
	{
		MessageBox("it's bottom");
		return;
	}

	nsSfc::swap(m_shopInfo.m_aData[iSelectedCategory],m_shopInfo.m_aData[iSelectedCategory+1]);

	m_iSelectedCategory	=	iSelectedCategory+1;

	updateCategoryList();
}

void cCarrotShop::OnMoveUpSelectItem() 
{
	int	iSelectedItem	=	m_listCarrotShopItem.getSelectedItem();

	if	(iSelectedItem	==	-1)
	{
		MessageBox("please select item");
		return;
	}

	if	(iSelectedItem	==	0)
	{
		MessageBox("it's top");
		return;
	}

	nsSfc::swap(m_editCategory.m_aItemList[iSelectedItem],m_editCategory.m_aItemList[iSelectedItem-1]);

	m_iSelectedItem	=	iSelectedItem-1;

	lostVirgin();

	updateItemList();
}

void cCarrotShop::OnMoveDownSelectItem() 
{
	int	iSelectedItem	=	m_listCarrotShopItem.getSelectedItem();

	if	(iSelectedItem	==	-1)
	{
		MessageBox("please select item");
		return;
	}

	if	(iSelectedItem	==	m_editCategory.m_wItemCount-1)
	{
		MessageBox("it's bottom");
		return;
	}

	nsSfc::swap(m_editCategory.m_aItemList[iSelectedItem],m_editCategory.m_aItemList[iSelectedItem+1]);

	m_iSelectedItem	=	iSelectedItem+1;

	lostVirgin();

	updateItemList();
}

void cCarrotShop::OnSaveToFile() 
{
	if	(isLostVirgin())
	{
		int	iResult	=	cMSG::YESNOCANCEL("modifyed some item data","do you want apply this?");

		if (iResult	==	IDCANCEL)
			return;

		if (iResult	==	IDYES)
		{
			cCarrotShopCategoryInfo	*lpSelectCategory	=	m_shopInfo.getCategory(m_iSelectedCategory);

			memcpy(lpSelectCategory,&m_editCategory,sizeof(cCarrotShopCategoryInfo));
		}

		setVirgin();
	}

	SetLogFolder();

	char	strFileName[256]	=	"";

	if	(!cFILE::SaveBox(strFileName,"carrot shop data {*.csd}\0*.csd\0\0","csd",g_strLogFolder,"Save carrot shop data"))
	{
		SetLogFolder();

		return;
	}

	cFILE	file;

	file.Open(strFileName,"wb");
	file.Write(&m_shopInfo,sizeof(m_shopInfo));
	file.Close();

	MessageBox("complete","save carrot shop data");
}

void cCarrotShop::OnLoadFromFile() 
{
	SetLogFolder();

	char	strFileName[256]	=	"";

	if	(!cFILE::LoadBox(strFileName,"carrot shop data {*.csd}\0*.csd\0\0","csd",g_strLogFolder,"Save carrot shop data"))
	{
		SetLogFolder();

		return;
	}

	cFILE	file;

	if	(!file.Open(strFileName,"rb"))
	{
		cMSG::Error("save carrot shop data","can not find '%s' file",strFileName);

		return;
	}

	file.Read(&m_shopInfo,sizeof(m_shopInfo));
	file.Close();

	setVirgin();

	m_iSelectedCategory	=	0;
	m_iSelectedItem		=	0;

	memcpy(&m_editCategory,&m_shopInfo.m_aData[0],sizeof(m_editCategory));

	updateCategoryList();

	MessageBox("complete","load carrot shop data");
}

void cCarrotShop::OnPasteCategory() 
{
	int	iSelectedCategory	=	m_listCategory.getSelectedItem();

	if	(iSelectedCategory	==	-1)
	{
		MessageBox("please select category");
		return;
	}

	if	(m_clipCategory.m_wItemCount	==	0)
	{
		MessageBox("invalid clip board");
		return;
	}

	char	strName[dCARROT_SHOP_CATEGORY_NAME_LENGTH];

	strcpy(strName,m_shopInfo.m_aData[iSelectedCategory].m_strName);

	memcpy(&m_shopInfo.m_aData[iSelectedCategory],&m_clipCategory,sizeof(m_clipCategory));

	strcpy(m_shopInfo.m_aData[iSelectedCategory].m_strName,strName);
}

void cCarrotShop::OnCopyCategory() 
{
	int	iSelectedCategory	=	m_listCategory.getSelectedItem();

	if	(iSelectedCategory	==	-1)
	{
		MessageBox("please select category");
		return;
	}

	memcpy(&m_clipCategory,&m_shopInfo.m_aData[iSelectedCategory],sizeof(m_clipCategory));
}

BOOL
cCarrotShop::check(int _iLimitLevel,BOOL _bIsCheckReceivedList)
{
	if	(m_bIsWaitToReceiveData)
	{
		MessageBox("wait to Receive carrot shop info","warning!");
		return	FALSE;
	}

	if	(!CheckOperatorLevel(_iLimitLevel))
		return FALSE;

	return	TRUE;
}

void
cCarrotShop::OnAskCarrotInfo() 
{
	// TODO: Add your control notification handler code here
	if	(!check(eAL_OPERATOR,FALSE))
		return;

	memset(&m_shopInfo,0xff,sizeof(m_shopInfo));
	g_DBAccount.GetCarrotShopInfo(&m_shopInfo);

	updateCategoryList();
	
	cMSG::Put("carrot shop","complete to receive carrot shop info");
}

void
cCarrotShop::receiveCarrotShopInfo(int _iIndex,int _iCount,int _iStatus,cCarrotShopCategoryInfo *_lpData)
{
/*	if	(_iStatus	==	2)
	{
		m_shopInfo.m_wCategoryCount	=	_iCount;
		m_bIsWaitToReceiveData		=	FALSE;
		m_bIsReceiveData			=	TRUE;

		m_iSelectedCategory			=	0;
		m_iSelectedItem				=	0;

		memcpy(&m_editCategory,&m_shopInfo.m_aData[0],sizeof(cCarrotShopCategoryInfo));

		updateCategoryList();

		cMSG::Put("receive shop info","complete");

		return;
	}

	if	(_iStatus	==	0)
		m_shopInfo.reset();

	memcpy(&m_shopInfo.m_aData[_iIndex],_lpData,sizeof(cCarrotShopCategoryInfo));
*/
}

void cCarrotShop::OnModifyCarrotShopData() 
{
	if	(!check(eAL_MASTER_OPERATER,TRUE))
		return;

	g_DBAccount.SetCarrotShopInfo(&m_shopInfo);

	cMSG::Put("carrot shop","complete to update carrot shop info");
}

void cCarrotShop::OnItemchangedCarrotItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	if	(pNMListView->uOldState	!=	0)
		return;

	int		iSelectItem	=	pNMListView->iItem;

	if	(iSelectItem	==	-1)
		return;

	m_iSelectedItem		=	iSelectItem;

	cCarrotShopItemDefine	*lpItem	=	&m_editCategory.m_aItemList[iSelectItem];
	
	m_editCount.SetWindowText(_ms("%d",lpItem->m_bf8ItemCount));
	m_editPrice.SetWindowText(_ms("%d",lpItem->m_wCarrotCount));

	m_cbPremiumItem.SelectString(0,g_aPremiumItem[lpItem->m_wPremiunItemIndex].m_strName);

	CheckDlgButton(IDC_ITEM_TEST,lpItem->m_bf1IsTestItem);
	CheckDlgButton(IDC_ITEM_HOT,lpItem->m_bf1IsHotItem);
	CheckDlgButton(IDC_ITEM_RECOMMAND,lpItem->m_bf1IsRecommandItem);
}

void cCarrotShop::OnApplyNow() 
{
	if	(!check(eAL_MASTER_OPERATER,TRUE))
		return;

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select server"," ");
		return;
	}

	if	(!IsConnectedServer(g_iSelectedServer,TRUE))
		return;

#ifdef	_FOR_KOREA
	if	(cMSG::YESNO("","모든 월드 서버에게 당근 상점 정보를 DB에서 새로 업데이트 받을것을 요청합니다.\n\n계속 하시겠습니까?")	==	IDNO)
		return;
#else
	if	(cMSG::YESNO("","Ask update all world inner game shop info\n\ncontinue?")	==	IDNO)
		return;
#endif

	if	(!SendWP_SimpleWork(g_iSelectedServer,eSWAW_UPDATE_CARROT_SHOP_INFO))
	{
		ERRMSG("Send Packet Error!");
		return;
	}

#ifdef	_FOR_KOREA
	MessageBox("모든 월드 서버의 정보가 업데이트 되었습니다."," ");
#else
	MessageBox("All servers are updated with information of the shop"," ");
#endif
}