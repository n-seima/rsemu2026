// cInventory.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "cInventory.h"
#include "CharacterInfo.h"
#include "cMakeItem.h"
#include "cDECODER.h"
#include "cMESSAGE.h"
#include "AdminLogCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_FOR_KOREA
char	*l_strEquipmentName[]	=	
{
"무기 1",
"방패/탄환",
"갑옷",
"장갑",
"헬멧 종류",
"귀거리",
"목걸이",
"벨트",
"신발",
"반지 1",
"반지 2",
"반지 3",
"반지 4",
"반지 5",
"반지 6",
"반지 7",
"반지 8",
"무기 2",
"벨트 1",
"벨트 2",
"벨트 3",
"벨트 4",
"벨트 5",
"end"
};

#else

char	*l_strEquipmentName[]	=	
{
"Weapon 1",
"Shield/Bullet",
"Armor",
"Glove",
"Helm",
"Earing",
"Necklace",
"Belt",
"Shoes",
"Ring 1",
"Ring 2",
"Ring 3",
"Ring 4",
"Ring 5",
"Ring 6",
"Ring 7",
"Ring 8",
"Wapon 2",
"Belt 1",
"Belt 2",
"Belt 3",
"Belt 4",
"Belt 5",
"end"
};

#endif

/////////////////////////////////////////////////////////////////////////////
// cInventory dialog


cInventory::cInventory(CWnd* pParent /*=NULL*/)
	: CDialog(cInventory::IDD, pParent)
{
	//{{AFX_DATA_INIT(cInventory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cInventory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cInventory)
	DDX_Control(pDX, IDC_PLAYER_LIST, m_cbPlayerList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cInventory, CDialog)
	//{{AFX_MSG_MAP(cInventory)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_BN_CLICKED(IDC_MAKE, OnMake)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_REMOVE_BY_CODE, OnRemoveByCode)
	ON_BN_CLICKED(IDC_GET_ITEM_CODE, OnGetItemCode)
	ON_BN_CLICKED(IDC_ADD_GOLD, OnAddGold)
	ON_BN_CLICKED(IDC_REMOVE_GOLD, OnRemoveGold)
	ON_BN_CLICKED(IDC_BANK, OnBank)
	ON_BN_CLICKED(IDC_INVENTORY, OnInventory)
	ON_CBN_SELCHANGE(IDC_PLAYER_LIST, OnSelchangePlayerList)
	ON_BN_CLICKED(IDC_RENEW_SELECT_ITEM_PERIOD, OnRenewSelectItemPeriod)
	ON_NOTIFY(NM_CLICK, IDC_ITEM_LIST2, OnClickItemList2)
	ON_NOTIFY(NM_CLICK, IDC_ITEM_LIST, OnClickItemList)
	ON_BN_CLICKED(IDC_GET_ITEM_CODE2, OnGetItemCode2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cInventory message handlers

void
cInventory::init(CAvatarManager	*_lpEditAvatar,int _iSelectAvatar,int _iSelectWorld)
{
	m_lpAM			=	_lpEditAvatar;
	m_iSelectAvatar	=	_iSelectAvatar;
	m_iSelectWorld	=	_iSelectWorld;

	m_lpSelectPlayer=	&m_lpAM->m_aData[_iSelectAvatar];
}

BOOL cInventory::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_iViewSlot	=	eVS_INVENTORY;

	m_aList[0].init(GetDlgItem(IDC_ITEM_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_ITEM_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_aList[0].addColumn(0,90,"Slot");
	m_aList[0].addColumn(1,46,"Count");
	m_aList[0].addColumn(2,11*6,"End Date");
	m_aList[0].addColumn(3,290,"Name");

	m_aList[1].init(GetDlgItem(IDC_ITEM_LIST2)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_ITEM_LIST2)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_aList[1].addColumn(0,90,"Slot");
	m_aList[1].addColumn(1,46,"Count");
	m_aList[1].addColumn(2,11*6,"End Date");
	m_aList[1].addColumn(3,290,"Name");

	{
		m_cbPlayerList.ResetContent();

		for (int i=0;i<m_lpAM->m_iLoadDataCount;i++)	
			m_cbPlayerList.AddString(m_lpAM->m_aData[i].m_strName);

		m_cbPlayerList.SetCurSel(m_iSelectAvatar);
	}

	char	strGoldInfo[128];

	sprintf(strGoldInfo,"[%s : %d gold],[Bank : %ud gold]",m_lpSelectPlayer->m_strName,m_lpSelectPlayer->m_iGold,m_lpAM->m_bank.m_dwGold);

	SetDlgItemText(IDC_STATIC_GOLD_INFO,strGoldInfo);

	CheckDlgButton(IDC_INVENTORY,TRUE);

	updateList();

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void
cInventory::setItemInfoToList(cListView *_lpListView,int _iIndex,cITEM *_lpItem)
{
	if	(_lpItem	==	NULL	||	_lpItem->m_wBaseItem	==	0xffff)
	{
		_lpListView->setItem(_iIndex,1,"0");
		_lpListView->setItem(_iIndex,2,"../../..");
		_lpListView->setItem(_iIndex,3,"empty");
	}
	else
	{
		int	iItemCount	=	_lpItem->m_bCount;

		if	(_lpItem->isExceptionItem())
			iItemCount	=	1;

		char	strName[512];

		strcpy(strName,_lpItem->getName());

		if	(_lpItem->isCoupleRing())
		{
			cCoupleRingDefine	*lpRing	=	(cCoupleRingDefine	*)_lpItem;

			char	strCoupleName[dNAME_LENGTH];
			
			memcpy(strCoupleName,lpRing->m_strCoupleName,dNAME_LENGTH-2);

			strCoupleName[dNAME_LENGTH-2]	=	NULL;

			sprintf(strName,"%s[%s]",_lpItem->getName(),strCoupleName);

		}
		if	(_lpItem->isBreedingRecordBook())
		{
			cBreedingRecordBookDefine	*lpBRBook	=	_lpItem->getBreedingRecordBookPetInfo();
			cPetDefine	petInfo;
			int			iAvatarIndex	=	0;
			int			iAvatarLevel	=	0;

			if	(lpBRBook->isAvailInfo(_lpItem->m_dwSerial))
			{
				lpBRBook->makePetInfo(&petInfo,_lpItem->m_dwSerial);
				iAvatarIndex	=	lpBRBook->m_bf4TamerSlot;
				iAvatarLevel	=	lpBRBook->m_bf11TamerLevel;
			}
			else
			{
				memcpy(&petInfo,&_lpItem->m_bCount,sizeof(cPetDefine)-4);
				petInfo.m_dwExperience		=	_lpItem->m_dwSerial;
				iAvatarIndex				=	petInfo.m_bf2TamerSlot;
			}

			{
				char	strPetName[dPET_NAME_LENGTH+2];

				memcpy(strPetName,petInfo.m_strName,dPET_NAME_LENGTH);
				strPetName[dPET_NAME_LENGTH]	=	NULL;

				sprintf(strName,"%s [Idx %d,Lv %d] Pet [Lv %d,Exp %d]",
								strPetName,iAvatarIndex,iAvatarLevel,
								petInfo.m_bf10Level,petInfo.m_dwExperience);
			}
		}

		_lpListView->setItem(_iIndex,1,_ms("%d",iItemCount));
		_lpListView->setItem(_iIndex,2,_lpItem->getExpireDate());
		_lpListView->setItem(_iIndex,3,strName);
	}
}

void
cInventory::updateInventoryList()
{
	int	i;

	m_aList[0].reset();
	m_aList[1].reset();

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		m_aList[0].addItem(i,0,_ms("Inventory #%.2d",i+1),i);

		cITEM	*lpItem	=	(cITEM	*)&m_lpSelectPlayer->m_aItems[i];

		setItemInfoToList(&m_aList[0],i,lpItem);
	}
	for (i=0;i<dEXTRA_INVENTORY_SIZE;i++)
	{
		m_aList[0].addItem(i+dOWN_ITEM_COUNT,0,_ms("MagicBag #%.2d",i+1),i+dOWN_ITEM_COUNT);

		cITEM	*lpItem	=	(cITEM	*)&m_lpSelectPlayer->m_aExtraInventory[i];

		if	(i	>=	m_lpSelectPlayer->m_bf6ExtraInventorySize)
			lpItem	=	NULL;

		setItemInfoToList(&m_aList[0],i+dOWN_ITEM_COUNT,lpItem);
	}

	for (i=0;i<dEQUIPMENT_PART_COUNT;i++)
	{
		m_aList[1].addItem(i,0,l_strEquipmentName[i],i);

		cITEM	*lpItem	=	(cITEM	*)&m_lpSelectPlayer->m_aEquip[i];

		setItemInfoToList(&m_aList[1],i,lpItem);
	}
}

void
cInventory::updateEquipmentList()
{
	int	iCount	=	0,i;

	m_aList[0].reset();

	for (i=0;i<dOWN_ITEM_COUNT;i++)
	{
		m_aList[0].addItem(i,0,_ms("%.2d",i),i);

		cITEM	*lpItem	=	(cITEM	*)&m_lpSelectPlayer->m_aItems[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			m_aList[0].addItem(i,2,"invalid");
		else
			m_aList[0].setItem(i,2,lpItem->getName());

		iCount++;
	}
}

void
cInventory::updateBankList()
{
	int	iCount	=	0,i,iList;

	for (iList=0;iList<2;iList++)
	{
		m_aList[iList].reset();

		for (i=0;i<dMAX_ITEM_IN_BANK/2;i++)
		{
			m_aList[iList].addItem(i,0,_ms("Bank #%.2d",iCount+1),iCount);

			cITEM	*lpItem	=	(cITEM	*)&m_lpAM->m_bank.m_aItems[iCount];

			if	(lpItem->m_wBaseItem	==	0)
				lpItem->reset();

			if	(lpItem->m_wBaseItem	==	0xffff)
			{
				m_aList[iList].setItem(i,1,"0");
				m_aList[iList].setItem(i,2,lpItem->getExpireDate());
				m_aList[iList].setItem(i,3,"empty");
			}
			else
			{
				m_aList[iList].setItem(i,1,_ms("%d",lpItem->m_bCount));
				m_aList[iList].setItem(i,2,lpItem->getExpireDate());
				m_aList[iList].setItem(i,3,lpItem->getName());
			}

			iCount++;
		}
	}
}

void
cInventory::updateList()
{
	char	strGoldInfo[128];

	sprintf(strGoldInfo,"[%s : %d gold],[Bank : %ud gold]",m_lpSelectPlayer->m_strName,m_lpSelectPlayer->m_iGold,m_lpAM->m_bank.m_dwGold);

	SetDlgItemText(IDC_STATIC_GOLD_INFO,strGoldInfo);

	switch(m_iViewSlot)
	{
		case	eVS_INVENTORY	:
			updateInventoryList();
			break;
		case	eVS_EQUIPMENT	:
			updateEquipmentList();
			break;
		case	eVS_BANK		:
			updateBankList();
			break;
	}
}

void cInventory::OnAdd() 
{
	cITEM		item;
	cDECODER	decoder;

	char		strScript[1024];
	char		*lpstrWord;
	BOOL		bIsSuccess;

	GetDlgItemText(IDC_ITEM_CODE,strScript,sizeof(strScript));

	if	(!decoder.UploadBuffer(strScript,NULL))
	{
		cMSG::Put("add item","please input item code");
		return;
	}

	lpstrWord	=	decoder.GetWord();

	if	(stricmp(lpstrWord,"cr")	==	0)
	{
		cCoupleRingDefine	*lpRing	=	(cCoupleRingDefine	*)&item;

		lpRing->m_wBaseItem		=	(WORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		lpRing->m_dwSerial		=	(DWORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}

		char		*lpstrName	=	decoder.GetStream();

		memcpy(lpRing->m_strCoupleName,lpstrName,dNAME_LENGTH-2);

		lpRing->m_bf1IsCanNotTradeItem	=	(WORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		lpRing->m_aOption[0]			=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		lpRing->m_aOption[1]			=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
	}
	else
	if	(stricmp(lpstrWord,"br")	==	0)
	{
		item.m_wBaseItem				=	(WORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		item.m_dwSerial					=	(DWORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}

		cBreedingRecordBookDefine	*lpBRBook	=	item.getBreedingRecordBookPetInfo();
		cPetDefine	petInfo;

		if	(!lpBRBook)
		{
			cMSG::Put("Error in Add Item!!","Incorrect Pet Info");
			return;
		}

		lpstrWord			=	decoder.GetWord();

		if	(stricmp(lpstrWord,"name")	!=	0)
		{	
			cMSG::Put("Error in Add Item!!","Incorrect Item Data");
			return;
		}

		char		*lpstrName	=	decoder.GetStream();

		memcpy(petInfo.m_strName,lpstrName,dPET_NAME_LENGTH);

		lpstrWord			=	decoder.GetWord();

		if	(stricmp(lpstrWord,"avatarIndex")	!=	0)
		{	
			cMSG::Put("Error in Add Item!!","Incorrect Item Data");
			return;
		}

		int	iAvatarIndex		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		if	(iAvatarIndex	>=	16	)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}

		lpstrWord			=	decoder.GetWord();

		if	(stricmp(lpstrWord,"code")	!=	0)
		{	
			cMSG::Put("Error in Add Item!!","Incorrect Item Data");
			return;
		}

		int	iBreedingTime	=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		int	iLoyalty		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		int	iLevel			=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		int	iCurrentHP		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		int	iJob			=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		int	iBoostCount		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		int	iTamerLevel		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	iTamerLevel		=	0;

		petInfo.m_bf2TamerSlot		=	0;
		petInfo.m_wBreedingTime	=	iBreedingTime;
		petInfo.m_bf14Loyalty	=	iLoyalty;
		petInfo.m_bf10Level		=	iLevel;
		petInfo.m_bf22CurrentHP	=	iCurrentHP;
		petInfo.setJobIndex(iJob);
		petInfo.m_bf5BoostCount	=	iBoostCount;

		item.m_dwSerial	=	lpBRBook->buildByPetInfo(&petInfo,iAvatarIndex,iTamerLevel);
	}
	else
	{
		decoder.rewind();

		item.m_wBaseItem				=	(WORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		item.m_dwSerial					=	(DWORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}

		int	iCount						=	(DWORD)decoder.GetNumber(&bIsSuccess);

		item.m_bCount					=	iCount;

		int	iDurability					=	(DWORD)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}
		item.m_bDurability				=	iDurability;

		item.m_aOption[0]				=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}
		item.m_aOption[1]				=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}

		item.m_aPrefix[0].m_wPrefix		=	(WORD)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}
		item.m_aPrefix[0].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}
		item.m_aPrefix[0].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}

		item.m_aPrefix[1].m_wPrefix		=	(WORD)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}
		item.m_aPrefix[1].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}
		item.m_aPrefix[1].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}

		item.m_aPrefix[2].m_wPrefix		=	(WORD)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}
		item.m_aPrefix[2].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return;}
		item.m_aPrefix[2].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");
			return;
		}

		if	(item.isExceptionItem()	==	FALSE)
		{
			if	(item.m_dwSerial	<= 1)
				item.m_dwSerial	=	largeRandom(0x7fffffff);

			if	(!item.isExtraItem())
				if	(iCount	<= 0 || iCount> 255)
				{
					cMSG::Put("command error!!","incorrect item count\n\n %d",iCount);

					return;
				}
		}

		DWORD	dwValue					=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");
			return;
		}
		
		item.setBitFieldValue(dwValue);

		char	*lpstrWord				=	decoder.GetWord();

		if	(stricmp(lpstrWord,"expireDate")	==	0)
		{
			int	iYear	=	(WORD)decoder.GetNumber(&bIsSuccess);
			int	iMonth	=	(WORD)decoder.GetNumber(&bIsSuccess);
			int	iDay	=	(WORD)decoder.GetNumber(&bIsSuccess);
			int	iHour	=	(WORD)decoder.GetNumber(&bIsSuccess);
			int	iMinute	=	(WORD)decoder.GetNumber(&bIsSuccess);

			item.m_year	=	iYear%2000;
			item.m_month=	iMonth;
			item.m_day	=	iDay;
			item.m_hour	=	iHour;
			item.m_minute=	iMinute;

		}
	}

	int	iResult	=	item.getIncorrectPrefixCount();

	if (iResult	==	-1)
	{
		cMSG::Put("ERROR ITEM ADDITION!!","Not Found  Empty Slot");

		return;
	}

	if	(iResult)
		if	(cMSG::YESNO("ERROR ITEM ADDITION!!",g_strToolMessage[147],iResult) == IDNO)
			return;

	if	(item.isExceptionItem() == FALSE)
		if	(item.getBasicItem()->m_wStackLimit	<= 1 || item.isInfinityBullet())
		{
			if	(m_lpAM->getItem(item.m_wBaseItem,item.m_dwSerial))
			{
				ERRMSG("command error","exist same item in Player Inventory");

				return;
			}

			if	(m_lpAM->getItemInBank(item.m_wBaseItem,item.m_dwSerial))
			{
				ERRMSG("command error","exist same item in Player Bank");

				return;
			}
		}

	CItemDefine	*lpSlot;

	if (m_iViewSlot	==	eVS_BANK)
	{
		if	(m_lpAM->m_bIsInvalidBankData)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","invalid Bank Data");

			return;
		}

		lpSlot	=	m_lpAM->getFreeSlotInBank();

		if	(!lpSlot)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","Not Found Empty Slot in Bank");

			return;
		}

		m_lpAM->m_bIsChangeBankData	=	TRUE;

		memcpy(lpSlot,&item,sizeof(item));

		_log("[%s] [id:%s] add item to bank : %s",strUserName,m_lpAM->m_strId,item.getItemCode());
		
		if	(!g_toolLog.addItemToBank(&item,m_lpSelectPlayer->m_strId,sList[m_iSelectWorld].strName))
			return;
	}
	else
	{
		lpSlot	=	m_lpSelectPlayer->getValidItem();

		if	(!lpSlot)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","Not Found  Empty Slot in Character");

			return;
		}

		memcpy(lpSlot,&item,sizeof(item));

		_log("[%s] [%s:%s] add item to inventory : %s",strUserName,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,item.getItemCode());

		if	(!g_toolLog.addItemToPlayer(&item,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,sList[m_iSelectWorld].strName))
			return;
	}

	updateList();

	cMSG::Put("ITEM ADDITION","Complete!!");
}


BOOL cInventory::OnAddByEventList(char* _ItemScript ,int _InsertBank) 
{
	cITEM		item;
	cDECODER	decoder;

	char		*lpstrWord;
	BOOL		bIsSuccess;

	//GetDlgItemText(IDC_ITEM_CODE,strScript,sizeof(strScript));

	if	(!decoder.UploadBuffer(_ItemScript,NULL))
	{
		cMSG::Put("add item","please input item code");
		return FALSE;
	}

	lpstrWord	=	decoder.GetWord();

	if	(stricmp(lpstrWord,"cr")	==	0)
	{
		cCoupleRingDefine	*lpRing	=	(cCoupleRingDefine	*)&item;

		lpRing->m_wBaseItem		=	(WORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		lpRing->m_dwSerial		=	(DWORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}

		char		*lpstrName	=	decoder.GetStream();

		memcpy(lpRing->m_strCoupleName,lpstrName,dNAME_LENGTH-2);

		lpRing->m_bf1IsCanNotTradeItem	=	(WORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		lpRing->m_aOption[0]			=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		lpRing->m_aOption[1]			=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
	}
	else
	if	(stricmp(lpstrWord,"br")	==	0)
	{
		item.m_wBaseItem				=	(WORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		item.m_dwSerial					=	(DWORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}

		cBreedingRecordBookDefine	*lpBRBook	=	item.getBreedingRecordBookPetInfo();
		cPetDefine	petInfo;

		if	(!lpBRBook)
		{
			cMSG::Put("Error in Add Item!!","Incorrect Pet Info");
			return	FALSE;
		}

		lpstrWord			=	decoder.GetWord();

		if	(stricmp(lpstrWord,"name")	!=	0)
		{	
			cMSG::Put("Error in Add Item!!","Incorrect Item Data");
			return FALSE;
		}

		char		*lpstrName	=	decoder.GetStream();

		memcpy(petInfo.m_strName,lpstrName,dPET_NAME_LENGTH);

		lpstrWord			=	decoder.GetWord();

		if	(stricmp(lpstrWord,"avatarIndex")	!=	0)
		{	
			cMSG::Put("Error in Add Item!!","Incorrect Item Data");
			return FALSE;
		}

		int	iAvatarIndex	=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		if	(iAvatarIndex	>=	16	)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}

		lpstrWord			=	decoder.GetWord();

		if	(stricmp(lpstrWord,"code")	!=	0)
		{	
			cMSG::Put("Error in Add Item!!","Incorrect Item Data");
			return FALSE;
		}

		int	iBreedingTime	=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		int	iLoyalty		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		int	iLevel			=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		int	iCurrentHP		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		int	iJob			=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		int	iBoostCount		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		int	iTamerLevel		=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	iTamerLevel	=	0;

		petInfo.m_bf2TamerSlot	=	0;
		petInfo.m_wBreedingTime	=	iBreedingTime;
		petInfo.m_bf14Loyalty	=	iLoyalty;
		petInfo.m_bf10Level		=	iLevel;
		petInfo.m_bf22CurrentHP	=	iCurrentHP;
		petInfo.setJobIndex(iJob);
		petInfo.m_bf5BoostCount	=	iBoostCount;

		item.m_dwSerial	=	lpBRBook->buildByPetInfo(&petInfo,iAvatarIndex,iTamerLevel);
	}
	else
	{
		decoder.rewind();

		item.m_wBaseItem				=	(WORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}
		item.m_dwSerial					=	(DWORD)decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return FALSE;}

		int	iCount						=	(DWORD)decoder.GetNumber(&bIsSuccess);

		item.m_bCount					=	iCount;

		int	iDurability					=	(DWORD)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}
		item.m_bDurability				=	iDurability;

		item.m_aOption[0]				=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}
		item.m_aOption[1]				=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}

		item.m_aPrefix[0].m_wPrefix		=	(WORD)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}
		item.m_aPrefix[0].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}
		item.m_aPrefix[0].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}

		item.m_aPrefix[1].m_wPrefix		=	(WORD)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}
		item.m_aPrefix[1].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}
		item.m_aPrefix[1].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}

		item.m_aPrefix[2].m_wPrefix		=	(WORD)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}
		item.m_aPrefix[2].m_aValue[0]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)	{cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");return FALSE;}
		item.m_aPrefix[2].m_aValue[1]	=	(BYTE)decoder.GetNumber(&bIsSuccess);
		if (!bIsSuccess)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");
			return FALSE;
		}

		if	(item.isExceptionItem()	==	FALSE)
		{
			if	(item.m_dwSerial	<= 1)
				item.m_dwSerial	=	largeRandom(0x7fffffff);

			if	(!item.isExtraItem())
				if	(iCount	<= 0 || iCount> 255)
				{
					cMSG::Put("command error!!","incorrect item count\n\n %d",iCount);

					return FALSE;
				}
		}

		DWORD	dwValue					=	decoder.GetNumber(&bIsSuccess);

		if	(!bIsSuccess)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","Incorrect Data");
			return FALSE;
		}
		
		item.setBitFieldValue(dwValue);

		char	*lpstrWord				=	decoder.GetWord();

		if	(stricmp(lpstrWord,"expireDate")	==	0)
		{
			int	iYear	=	(WORD)decoder.GetNumber(&bIsSuccess);
			int	iMonth	=	(WORD)decoder.GetNumber(&bIsSuccess);
			int	iDay	=	(WORD)decoder.GetNumber(&bIsSuccess);
			int	iHour	=	(WORD)decoder.GetNumber(&bIsSuccess);
			int	iMinute	=	(WORD)decoder.GetNumber(&bIsSuccess);

			item.m_year	=	iYear%2000;
			item.m_month=	iMonth;
			item.m_day	=	iDay;
			item.m_hour	=	iHour;
			item.m_minute=	iMinute;

		}
	}

	int	iResult	=	item.getIncorrectPrefixCount();

	if (iResult	==	-1)
	{
		cMSG::Put("ERROR ITEM ADDITION!!","Not Found  Empty Slot");

		return FALSE;
	}

	if	(iResult)
		if	(cMSG::YESNO("ERROR ITEM ADDITION!!",g_strToolMessage[147],iResult) == IDNO)
			return FALSE;

	if	(item.isExceptionItem() == FALSE)
		if	(item.getBasicItem()->m_wStackLimit	<= 1 || item.isInfinityBullet())
		{
			if	(m_lpAM->getItem(item.m_wBaseItem,item.m_dwSerial))
			{
				ERRMSG("command error","exist same item in Player Inventory");

				return FALSE;
			}

			if	(m_lpAM->getItemInBank(item.m_wBaseItem,item.m_dwSerial))
			{
				ERRMSG("command error","exist same item in Player Bank");

				return FALSE;
			}
		}

	CItemDefine	*lpSlot;

	if (_InsertBank)
	{
		lpSlot	=	m_lpAM->getFreeSlotInBank();

		if	(!lpSlot)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","Not Found Empty Slot in Bank");

			return FALSE;
		}

		m_lpAM->m_bIsChangeBankData	=	TRUE;

		memcpy(lpSlot,&item,sizeof(item));

		_log("[%s] [id:%s] add item to bank : %s",strUserName,m_lpAM->m_strId,item.getItemCode());
		
		if	(!g_toolLog.addItemToBank(&item,m_lpSelectPlayer->m_strId,sList[m_iSelectWorld].strName))
			return FALSE;
	}
	else
	{
		lpSlot	=	m_lpSelectPlayer->getValidItem();

		if	(!lpSlot)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","Not Found  Empty Slot in Character");

			return FALSE;
		}

		memcpy(lpSlot,&item,sizeof(item));

		_log("[%s] [%s:%s] add item to inventory : %s",strUserName,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,item.getItemCode());

		if	(!g_toolLog.addItemToPlayer(&item,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,sList[m_iSelectWorld].strName))
			return FALSE;
	}

	return TRUE;
}

void cInventory::OnRemove() 
{
	int	aiSelectedItem[2][100],aiSelectedItemCount[2],i;

	for	(i=0;i<2;i++)
	{
		int	*lpiList	=	m_aList[i].getSelectedItem(&aiSelectedItemCount[i]);

		if	(aiSelectedItemCount[i]	&&	lpiList)
			memcpy(aiSelectedItem[i],lpiList,4*aiSelectedItemCount[i]);
	}

	if	(aiSelectedItemCount[0]+aiSelectedItemCount[1]	==	0)
	{
		cMSG::Put("remove Item","please select item");

		return;
	}

	if	(cMSG::YESNO("remove item","do you want remove %d count item",aiSelectedItemCount[0]+aiSelectedItemCount[1])	==	IDNO)
		return;

	cITEM	*lpItem;

	if	(m_iViewSlot	==	eVS_INVENTORY)
	{
		for	(i=0;i<2;i++)
		{
			for (int iSlot=0;iSlot<aiSelectedItemCount[i];iSlot++)
			{
				int	iRemoveSlot	=	aiSelectedItem[i][iSlot];

				if	(i==0)
				{
					if	(iRemoveSlot	>=	dOWN_ITEM_COUNT)
						lpItem	=	(cITEM *)&m_lpSelectPlayer->m_aExtraInventory[iRemoveSlot-dOWN_ITEM_COUNT];
					else
						lpItem	=	(cITEM *)&m_lpSelectPlayer->m_aItems[iRemoveSlot];
				}
				else
					lpItem	=	(cITEM *)&m_lpSelectPlayer->m_aEquip[iRemoveSlot];

				if	(lpItem->m_wBaseItem	!=	0xffff)
				{
					if	(!g_toolLog.removeItemFromPlayer(lpItem,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,sList[m_iSelectWorld].strName))
						return;

					_log("[%s] [%s:%s] remove item from inventory : %s",strUserName,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,lpItem->getItemCode());

					lpItem->reset();
				}
			}
		}
	}
	else
	{
		for	(i=0;i<2;i++)
		{
			for (int iSlot=0;iSlot<aiSelectedItemCount[i];iSlot++)
			{
				int	iRemoveSlot	=	aiSelectedItem[i][iSlot];

				if	(i==0)
					lpItem	=	(cITEM *)&m_lpAM->m_bank.m_aItems[iRemoveSlot];
				else
					lpItem	=	(cITEM *)&m_lpAM->m_bank.m_aItems[iRemoveSlot+dMAX_ITEM_IN_BANK/2];

				if	(lpItem->m_wBaseItem	!=	0xffff)
				{
					if	(!g_toolLog.removeItemFromBank(lpItem,m_lpSelectPlayer->m_strId,sList[m_iSelectWorld].strName))
						return;

					_log("[%s] [%s] remove item from bank : %s",strUserName,m_lpSelectPlayer->m_strId,lpItem->getItemCode());

					lpItem->reset();

					m_lpAM->m_bIsChangeBankData	=	TRUE;
				}
			}
		}
	}

	updateList();
}

void cInventory::OnFind() 
{
	cDECODER	decoder;
	BOOL		bIsSuccess;

	char		strScript[1024];

	GetDlgItemText(IDC_ITEM_CODE,strScript,sizeof(strScript));

	if	(!decoder.UploadBuffer(strScript,NULL))
		return;

	int		iBaseItem	=	decoder.GetNumber(&bIsSuccess);

	if	(!bIsSuccess)
	{
		cMSG::Put("ERROR ITEM DELETION!!","Incorrect Data\n\nplease input find item code");
		return;
	}

	DWORD	dwSerial	=	(DWORD)decoder.GetNumber(&bIsSuccess);

	if	(!bIsSuccess)
	{
		cMSG::Put("ERROR ITEM DELETION!!","Incorrect Data\n\nplease input find item code");
		return;
	}

	exportItemList(iBaseItem,dwSerial);
}

void cInventory::OnMake() 
{
	cMakeItem	m_dlgMakeItem;

	m_dlgMakeItem.m_makeItem.reset();

	if	(m_dlgMakeItem.DoModal()	==	IDCANCEL)
	{
		SetDlgItemText(IDC_ITEM_CODE,"");
		return;
	}

	char	strCode[512];

	cITEM	*lpItem	=	(cITEM	*)&m_dlgMakeItem.m_makeItem;
	
	sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
							lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
							lpItem->m_aOption[0],lpItem->m_aOption[1],
							lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
							lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
							lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
							lpItem->getBitFieldValue());

	SetDlgItemText(IDC_ITEM_CODE,strCode);
}

void cInventory::OnExport() 
{
	exportItemList(0,0);
}

void cInventory::OnRemoveByCode() 
{
	cITEM		item;
	cDECODER	decoder;
	BOOL		bIsSuccess;

	char		strScript[1024];

	GetDlgItemText(IDC_ITEM_CODE,strScript,sizeof(strScript));

	if	(!decoder.UploadBuffer(strScript,NULL))
		return;

	int		iBaseItem	=	(WORD)decoder.GetNumber(&bIsSuccess);

	if	(!bIsSuccess)
	{
		cMSG::Put("ERROR ITEM DELETION!!","Incorrect Data\n\nplease input remove item code");

		return;
	}

	DWORD	dwSerial	=	(DWORD)decoder.GetNumber(&bIsSuccess);

	if	(!bIsSuccess)
	{
		cMSG::Put("ERROR ITEM DELETION!!","Incorrect Data\n\nplease input remove item code");
		return;
	}

	int	iRemoveBankCount	=	0,iRemovePlayerCount	=	0;

	while(1)
	{
		cITEM	*lpItem	=	m_lpAM->getItemInBank(iBaseItem,dwSerial);

		if	(!lpItem)
			break;

		if	(cMSG::YESNO("ITEM DELETION!!",g_strToolMessage[144],iBaseItem,dwSerial,((cITEM*)lpItem)->getName()) == IDNO)
			return;

		if	(g_toolLog.removeItemFromBank(lpItem,m_lpSelectPlayer->m_strId,sList[m_iSelectWorld].strName))
		{
			_log("[%s] [%s] remove item from bank : %s",strUserName,m_lpSelectPlayer->m_strId,lpItem->getItemCode());

			iRemoveBankCount++;

			lpItem->reset();
			updateList();

			m_lpAM->m_bIsChangeBankData	=	TRUE;
		}
	}

	while(1)
	{
		cITEM	*lpItem	=	m_lpAM->getItem(iBaseItem,dwSerial);

		if	(!lpItem)
			break;

		if	(cMSG::YESNO("ITEM DELETION!!",g_strToolMessage[146],iBaseItem,dwSerial,((cITEM*)lpItem)->getName()) == IDNO)
			return;

		if	(g_toolLog.removeItemFromPlayer(lpItem,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,sList[m_iSelectWorld].strName))
		{
			_log("[%s] [%s:%s] remove item from inventory : %s",strUserName,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,lpItem->getItemCode());
	
			iRemovePlayerCount++;

			lpItem->reset();
			updateList();
		}
	}

	if	(iRemoveBankCount+iRemovePlayerCount	==	0)
	{
		cMSG::Put("ERROR ITEM DELETION!!!!",g_strToolMessage[145],iBaseItem,dwSerial);
		return;
	}

	cMSG::Put("ITEM DELETION!!","Complete!!");
}

void cInventory::OnGetItemCode() 
{
	SetDlgItemText(IDC_ITEM_CODE,"");

	cITEM	*lpItem	=	NULL;

	if	(m_iViewSlot	==	eVS_INVENTORY)
	{
		BOOL	bResult;
		int		iIndex	=	m_aList[0].getSelectedItemLParam(&bResult);

		if	(bResult	==	TRUE)
		{
			if	(iIndex	>=	dOWN_ITEM_COUNT)
				lpItem	=	(cITEM	*)&m_lpSelectPlayer->m_aExtraInventory[iIndex-dOWN_ITEM_COUNT];
			else
				lpItem	=	(cITEM	*)&m_lpSelectPlayer->m_aItems[iIndex];
		}
	}
	else
	{
		BOOL	bResult;
		int		iIndex	=	m_aList[0].getSelectedItemLParam(&bResult);

		if	(bResult	==	TRUE)
			lpItem	=	(cITEM	*)&m_lpAM->m_bank.m_aItems[iIndex];
	}

	if	(!lpItem	||	lpItem->m_wBaseItem	==	0xffff)
	{
		cMSG::Put("export item code","please select item");

		return;
	}

	char	strCode[512];

	if	(lpItem->isBreedingRecordBook())
	{
		cBreedingRecordBookDefine	*lpBRBook	=	lpItem->getBreedingRecordBookPetInfo();
		cPetDefine	petInfo;
		int			iAvatarIndex	=	0,iTamerLevel		=	0;

		if	(lpBRBook->isAvailInfo(lpItem->m_dwSerial))
		{
			lpBRBook->makePetInfo(&petInfo,lpItem->m_dwSerial);
			iAvatarIndex	=	lpBRBook->m_bf4TamerSlot;
			iTamerLevel		=	lpBRBook->m_bf11TamerLevel;
		}
		else
		{
			memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
			petInfo.m_dwExperience		=	lpItem->m_dwSerial;
			iAvatarIndex				=	petInfo.m_bf2TamerSlot;
		}

		char	strPetName[dPET_NAME_LENGTH+2];

		memcpy(strPetName,petInfo.m_strName,dPET_NAME_LENGTH);
		strPetName[dPET_NAME_LENGTH]	=	NULL;

		sprintf(strCode,"#br [%d:%d] name \"%s\" avatarIndex [%d] code [%d,%d,%d,%d,%d,%d,%d]",
			lpItem->m_wBaseItem,lpItem->m_dwSerial,strPetName,iAvatarIndex,
			petInfo.m_wBreedingTime,petInfo.m_bf14Loyalty,petInfo.m_bf10Level,petInfo.m_bf22CurrentHP,petInfo.getJobIndex(),petInfo.m_bf5BoostCount,iTamerLevel);
	}
	else
	if	(lpItem->isCoupleRing())
	{
		cCoupleRingDefine	*lpRing	=	(cCoupleRingDefine	*)lpItem;

		char	strCoupleName[dNAME_LENGTH];

		memcpy(strCoupleName,lpRing->m_strCoupleName,dNAME_LENGTH-2);
		strCoupleName[dNAME_LENGTH-2]	=	NULL;

		sprintf(strCode,"#cr [%d:%d] name \"%s\" [%d,%d,%d]",
				lpRing->m_wBaseItem,lpRing->m_dwSerial,strCoupleName,lpRing->m_bf1IsCanNotTradeItem,lpRing->m_aOption[0],lpRing->m_aOption[1]);
	}
	else
	{
		sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d expireDate %d/%d/%d %d:%d",
								lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
								lpItem->m_aOption[0],lpItem->m_aOption[1],
								lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
								lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
								lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
								lpItem->getBitFieldValue(),
								lpItem->m_year,lpItem->m_month,lpItem->m_day,lpItem->m_hour,lpItem->m_minute);
	}

	SetDlgItemText(IDC_ITEM_CODE,strCode);
}

void 
cInventory::exportItemList(int _iBaseItem,DWORD _dwSerial)
{
	char	strExportDir[512],strExportFileName[512];
	int		i,j,iCount=0;

	sprintf(strExportDir,g_strToolMessage[110],g_strLogFolder);

	if (_iBaseItem || _dwSerial >= 2)
		sprintf(strExportFileName,g_strToolMessage[111],m_lpAM->m_strId,sList[m_iSelectWorld].strName);
	else
		sprintf(strExportFileName,g_strToolMessage[112],m_lpAM->m_strId,sList[m_iSelectWorld].strName);

	SetCurrentDirectory(strExportDir);

	cFILE	file;

	file.Open(strExportFileName,"wb");
	cITEM	*lpItem;

	file.writeStreamCR(g_strToolMessage[113]);
	file.writeStreamCR(g_strToolMessage[114],m_lpAM->m_bank.m_dwGold);
	file.writeStreamCR(g_strToolMessage[115]);

	iCount	=	0;
	for (i=0;i<dMAX_ITEM_IN_BANK;i++)
	{
		lpItem	=	(cITEM *)&m_lpAM->m_bank.m_aItems[i];

		if (lpItem->m_wBaseItem	==	0xffff)	continue;

		if (_iBaseItem != 0 && lpItem->m_wBaseItem != _iBaseItem)	continue;
		if (_dwSerial  >  1 && lpItem->m_dwSerial  != _dwSerial)	continue;

		file.writeStreamCR("    Slot %2d %s IT [%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d       [%.2d/%.2d/%.2d] [%.2d:%.2d]",i,
							lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
							lpItem->m_aOption[0],lpItem->m_aOption[1],
							lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
							lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
							lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
							lpItem->getBitFieldValue()			,
							lpItem->m_year	,lpItem->m_month	,lpItem->m_day	,lpItem->m_hour	,lpItem->m_minute	);
		iCount++;
	}
	
	if (iCount	==	0)
		file.writeStreamCR("    ▷ No Item");
	else
	{
		file.writeStreamCR("    ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
		file.writeStreamCR(g_strToolMessage[116],iCount);
	}

	file.writeStreamCR(" ");

	iCount	=	0;
	for (i=0;i<m_lpAM->m_iLoadDataCount;i++)
	{
		file.writeStreamCR(g_strToolMessage[117],m_lpAM->m_aData[i].m_strName);

		iCount	=	0;
		file.writeStreamCR(g_strToolMessage[118]);
		for (j=0;j<dEQUIPMENT_PART_COUNT;j++)
		{
			lpItem	=	(cITEM *)&m_lpAM->m_aData[i].m_aEquip[j];

			if (lpItem->m_wBaseItem	==	0xffff)	continue;

			if (_iBaseItem != 0 && lpItem->m_wBaseItem != _iBaseItem)	continue;
			if (_dwSerial  >  1 && lpItem->m_dwSerial  != _dwSerial)	continue;

			file.writeStreamCR("    Slot %2d %s IT [%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d       [%.2d/%.2d/%.2d] [%.2d:%.2d]",j,
								lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
								lpItem->m_aOption[0],lpItem->m_aOption[1],
								lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
								lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
								lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
								lpItem->getBitFieldValue()			,
								lpItem->m_year	,lpItem->m_month	,lpItem->m_day	,lpItem->m_hour	,lpItem->m_minute	);

			iCount++;
		}
		if (iCount	==	0)	file.writeStreamCR("    ▷ no item");
		else
		{
			file.writeStreamCR("    ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
			file.writeStreamCR(g_strToolMessage[119],iCount);
		}

		iCount	=	0;
		file.writeStreamCR(" ");
		file.writeStreamCR("  Inventory");

		for (j=0;j<dOWN_ITEM_COUNT;j++)
		{
			lpItem	=	(cITEM *)&m_lpAM->m_aData[i].m_aItems[j];

			if (lpItem->m_wBaseItem	==	0xffff)	continue;

			if (_iBaseItem != 0 && lpItem->m_wBaseItem != _iBaseItem)	continue;
			if (_dwSerial  >  1 && lpItem->m_dwSerial  != _dwSerial)	continue;

			file.writeStreamCR("    Slot %2d %s IT [%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d       [%.2d/%.2d/%.2d] [%.2d:%.2d]",j,
								lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
								lpItem->m_aOption[0],lpItem->m_aOption[1],
								lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
								lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
								lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
								lpItem->getBitFieldValue()			,
								lpItem->m_year	,lpItem->m_month	,lpItem->m_day	,lpItem->m_hour	,lpItem->m_minute	);

			iCount++;
		}

		for (j=0;j<m_lpAM->m_aData[i].m_bf6ExtraInventorySize;j++)
		{
			lpItem	=	(cITEM *)&m_lpAM->m_aData[i].m_aExtraInventory[j];

			if (lpItem->m_wBaseItem	==	0xffff)
				continue;

			if (_iBaseItem != 0 && lpItem->m_wBaseItem != _iBaseItem)
				continue;
			if (_dwSerial  >  1 && lpItem->m_dwSerial  != _dwSerial)
				continue;

			file.writeStreamCR("    Ex Slot %2d %s IT [%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d       [%.2d/%.2d/%.2d] [%.2d:%.2d]",j,
								lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
								lpItem->m_aOption[0],lpItem->m_aOption[1],
								lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
								lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
								lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
								lpItem->getBitFieldValue()			,
								lpItem->m_year	,lpItem->m_month	,lpItem->m_day	,lpItem->m_hour	,lpItem->m_minute	);

			iCount++;
		}

		if (iCount	==	0)
			file.writeStreamCR("    ▷ no item");
		else				
		{
			file.writeStreamCR("     ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
			file.writeStreamCR(g_strToolMessage[120],iCount);
		}

		file.writeStreamCR("  ");
		file.writeStreamCR("  ");
	}


	file.Close();

	ShellExecute(NULL,"open",strExportFileName,NULL,NULL,SW_SHOWNORMAL);

	SetLogFolder();
	// TODO: Add your control notification handler code here
}

void cInventory::OnAddGold() 
{
	cDECODER	decoder;

	char		strScript[1024];

	GetDlgItemText(IDC_ITEM_CODE,strScript,sizeof(strScript));

	if	(!decoder.UploadBuffer(strScript,NULL))
	{
		ERRMSG("add gold","please input add gold");
		return;
	}

	BOOL	temp;
	DWORD	dwAddGold		=	decoder.GetNumber(&temp);

	if	(temp	==	FALSE)
	{
		ERRMSG("add gold","please input add gold");

		return;
	}

	if	(m_iViewSlot	==	eVS_INVENTORY)
	{
		LONGLONG	llGold	=	m_lpSelectPlayer->m_iGold;

		llGold	+=	dwAddGold;

		if	(llGold	>	2000000000)
		{
			ERRMSG("add gold","%s\n\nplease input add gold",dMSG_TOO_ADD_MANY_GOLD_FOR_BANK);

			return;
		}

		if	(cMSG::YESNO("add gold",dMSG_ADD_GOLD_FORM,dwAddGold) == IDNO)
			return;
		
		if	(!g_toolLog.addGoldToPlayer(dwAddGold,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,sList[m_iSelectWorld].strName))
			return;

		_log("[%s] [%s:%s] add gold to inventory : %d",strUserName,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,dwAddGold);
		
		m_lpSelectPlayer->m_iGold	+=	dwAddGold;
	}
	else
	{
		if	(m_lpAM->m_bIsInvalidBankData)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","invalid Bank Data");

			return;
		}

		LONGLONG	llGold	=	m_lpAM->m_bank.m_dwGold;

		llGold	+=	dwAddGold;

		if	(llGold	>	4000000000)
		{
			ERRMSG("add gold",dMSG_TOO_ADD_MANY_GOLD_FOR_BANK);

			return;
		}

		if	(cMSG::YESNO("add gold",dMSG_SAVE_GOLD_FORM,dwAddGold) == IDNO)
			return;

		if	(!g_toolLog.addGoldToBank(dwAddGold,m_lpSelectPlayer->m_strId,sList[m_iSelectWorld].strName))
			return;

		_log("[%s] [%s] add gold to bank : %d",strUserName,m_lpSelectPlayer->m_strId,dwAddGold);

		m_lpAM->m_bank.m_dwGold	+=	dwAddGold;
		m_lpAM->m_bIsChangeBankData	=	TRUE;
	}

	char	strGoldInfo[128];

	sprintf(strGoldInfo,"[%s : %d gold],[Bank : %ud gold]",m_lpSelectPlayer->m_strName,m_lpSelectPlayer->m_iGold,m_lpAM->m_bank.m_dwGold);

	SetDlgItemText(IDC_STATIC_GOLD_INFO,strGoldInfo);
}

void cInventory::OnRemoveGold() 
{
	cDECODER	decoder;

	char		strScript[1024];

	GetDlgItemText(IDC_ITEM_CODE,strScript,sizeof(strScript));

	if	(!decoder.UploadBuffer(strScript,NULL))
		return;

	BOOL	temp;
	int		iRemoveGold		=	decoder.GetNumber(&temp);

	if	(iRemoveGold	<=	0	||	temp	==	FALSE)
	{
#ifdef	_FOR_KOREA
		ERRMSG("remove gold","20억을 초과하는 금액을 제거 할 수는 없습니다.\n\n금액을 두번에 나눠서 제거해 주세요.");
#else
		ERRMSG("remove gold","can not remove over 200000000 gold\n\nplease input (1~2000000000) gold");
#endif
		return;
	}

	if	(m_iViewSlot	==	eVS_INVENTORY)
	{
		if	(cMSG::YESNO("remove gold",dMSG_DRAW_GOLD_FROM_PLAYER_FORM,iRemoveGold) == IDNO)
			return;

		if	(m_lpSelectPlayer->m_iGold	<	iRemoveGold)
		{
			ERRMSG("remove gold",dMSG_TOO_LOW_GOLD_IN_PLAYER);

			return;
		}

		if	(!g_toolLog.removeGoldFromPlayer(iRemoveGold,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,sList[m_iSelectWorld].strName))
			return;

		_log("[%s] [%s:%s] remove gold from inventory : %d",strUserName,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,iRemoveGold);

		m_lpSelectPlayer->m_iGold	-=	iRemoveGold;
	}
	else
	{
		if	(m_lpAM->m_bIsInvalidBankData)
		{
			cMSG::Put("ERROR ITEM ADDITION!!","invalid Bank Data");

			return;
		}

		if	(cMSG::YESNO("remove gold",dMSG_DRAW_GOLD_FROM_BANK_FORM,iRemoveGold) == IDNO)
			return;
		
		if	(m_lpAM->m_bank.m_dwGold	<	(DWORD)iRemoveGold)
		{
			ERRMSG("remove gold",dMSG_TOO_LOW_GOLD_IN_BANK);

			return;
		}

		if	(!g_toolLog.removeGoldFromBank(iRemoveGold,m_lpSelectPlayer->m_strId,sList[m_iSelectWorld].strName))
			return;

		_log("[%s] [%s] remove gold from bank : %d",strUserName,m_lpSelectPlayer->m_strId,iRemoveGold);

		m_lpAM->m_bank.m_dwGold		-=	iRemoveGold;
		m_lpAM->m_bIsChangeBankData	=	TRUE;
	}
	
	char	strGoldInfo[128];

	sprintf(strGoldInfo,"[%s : %d gold],[Bank : %ud gold]",m_lpSelectPlayer->m_strName,m_lpSelectPlayer->m_iGold,m_lpAM->m_bank.m_dwGold);

	SetDlgItemText(IDC_STATIC_GOLD_INFO,strGoldInfo);
}

void cInventory::OnBank() 
{
	if	(m_lpAM->m_bIsInvalidBankData)
	{
		cMSG::Put("ERROR ITEM ADDITION!!","invalid Bank Data");

		return;
	}
	
	m_iViewSlot	=	eVS_BANK;

	updateList();
}

void cInventory::OnInventory() 
{
	m_iViewSlot	=	eVS_INVENTORY;
	
	updateList();
}

void cInventory::OnSelchangePlayerList() 
{
	// TODO: Add your control notification handler code here
	int iSelectedList	=	m_cbPlayerList.GetCurSel();

	if	(iSelectedList	>=	m_lpAM->m_iLoadDataCount)
	{
		MessageBox("PLEASE, CONNECT TO LnkLogicKorea, in Korea");
		return;
	}

	m_iSelectAvatar	=	iSelectedList;
	m_lpSelectPlayer=	&m_lpAM->m_aData[m_iSelectAvatar];

	updateList();	
}

void cInventory::OnRenewSelectItemPeriod() 
{
	int	iExtendDay		=	GetDlgItemInt(IDC_EXTEND_DAY	);
	int	iExtendHour		=	GetDlgItemInt(IDC_EXTEND_HOUR	);
	int	iExtendMinute	=	GetDlgItemInt(IDC_EXTEND_MINUTE	);

	int	iExtendTime	=	iExtendDay*24*60+iExtendHour*60+iExtendMinute;

	if	(iExtendTime	<=	0	||	iExtendTime	>	60*24*30*12)
	{
		cMSG::Put("renew item preiod","please input time\n\nlimit 1 year");
		return;
	}

	int	aiSelectedItem[2][100],aiSelectedItemCount[2],i,iExtendItemCount=0;

	for	(i=0;i<2;i++)
	{
		int	*lpiList	=	m_aList[i].getSelectedItem(&aiSelectedItemCount[i]);

		if	(aiSelectedItemCount[i]	&&	lpiList)
			memcpy(aiSelectedItem[i],lpiList,4*aiSelectedItemCount[i]);
	}

	if	(aiSelectedItemCount[0]+aiSelectedItemCount[1]	==	0)
	{
		cMSG::Put("renew item preiod","please select item");

		return;
	}

	if	(cMSG::YESNO("renew item period","do you want renew %d count item's period\n\n%d day,%d hour,%d minute",aiSelectedItemCount[0]+aiSelectedItemCount[1],iExtendDay,iExtendHour,iExtendMinute)	==	IDNO)
		return;

	cITEM	*lpItem;

	if	(m_iViewSlot	==	eVS_INVENTORY)
	{
		for	(i=0;i<2;i++)
		{
			for (int iSlot=0;iSlot<aiSelectedItemCount[i];iSlot++)
			{
				int	iRemoveSlot	=	aiSelectedItem[i][iSlot];

				if	(i==0)
				{
					if	(iRemoveSlot	>=	dOWN_ITEM_COUNT)
						lpItem	=	(cITEM *)&m_lpSelectPlayer->m_aExtraInventory[iRemoveSlot-dOWN_ITEM_COUNT];
					else
						lpItem	=	(cITEM *)&m_lpSelectPlayer->m_aItems[iRemoveSlot];
				}
				else
					lpItem	=	(cITEM *)&m_lpSelectPlayer->m_aEquip[iRemoveSlot];

				if	(lpItem->m_wBaseItem	==	0xffff	||	!lpItem->isPremiumItem()	||	lpItem->m_year	==	0)
					continue;

				if	(!g_toolLog.renewPlayItemPeriod(lpItem,iExtendDay,iExtendHour,iExtendMinute,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,sList[m_iSelectWorld].strName))
					return;

				_log("[%s] [%s:%s] renew inventory item validate [%.2d days %.2d:%.2d] : %s",strUserName,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,lpItem->getItemCode(),iExtendDay,iExtendHour,iExtendMinute);

				iExtendItemCount++;
				lpItem->increaseMinute(iExtendTime);
			}
		}
	}
	else
	{
		for	(i=0;i<2;i++)
		{
			for (int iSlot=0;iSlot<aiSelectedItemCount[i];iSlot++)
			{
				int	iRemoveSlot	=	aiSelectedItem[i][iSlot];

				if	(i==0)
					lpItem	=	(cITEM *)&m_lpAM->m_bank.m_aItems[iRemoveSlot];
				else
					lpItem	=	(cITEM *)&m_lpAM->m_bank.m_aItems[iRemoveSlot+dMAX_ITEM_IN_BANK/2];

				if	(lpItem->m_wBaseItem	==	0xffff	||	!lpItem->isPremiumItem()	||	lpItem->m_year	==	0)
					continue;

				if	(!g_toolLog.renewBankItemPeriod(lpItem,iExtendDay,iExtendHour,iExtendMinute,m_lpSelectPlayer->m_strId,sList[m_iSelectWorld].strName))
					return;

				_log("[%s] [%s] renew bank item validate [%.2d days %.2d:%.2d] : %s",strUserName,m_lpSelectPlayer->m_strId,lpItem->getItemCode(),iExtendDay,iExtendHour,iExtendMinute);

				m_lpAM->m_bIsChangeBankData	=	TRUE;

				iExtendItemCount++;
				lpItem->increaseMinute(iExtendTime);
			}
		}
	}

	if	(iExtendItemCount)
		updateList();

	cMSG::Put("renew item period","renew %d count item's period",iExtendItemCount);
}


void cInventory::OnClickItemList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
}

void cInventory::OnClickItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void cInventory::OnGetItemCode2() 
{
	SetDlgItemText(IDC_ITEM_CODE,"");

	cITEM	*lpItem	=	NULL;

	if	(m_iViewSlot	==	eVS_INVENTORY)
	{
		BOOL	bResult;
		int		iIndex	=	m_aList[1].getSelectedItemLParam(&bResult);

		if	(bResult	==	TRUE)
			lpItem	=	(cITEM	*)&m_lpSelectPlayer->m_aEquip[iIndex];
	}
	else
	{
		BOOL	bResult;
		int		iIndex	=	m_aList[1].getSelectedItemLParam(&bResult);

		if	(bResult	==	TRUE)
			lpItem	=	(cITEM	*)&m_lpAM->m_bank.m_aItems[iIndex];
	}

	if	(!lpItem	||	lpItem->m_wBaseItem	==	0xffff)
	{
		cMSG::Put("export item code","please select item");

		return;
	}

	char	strCode[512];

	if	(lpItem->isBreedingRecordBook())
	{
		cBreedingRecordBookDefine	*lpBRBook	=	lpItem->getBreedingRecordBookPetInfo();
		cPetDefine	petInfo;
		int			iAvatarIndex	=	0,iTamerLevel		=	0;

		if	(lpBRBook->isAvailInfo(lpItem->m_dwSerial))
		{
			lpBRBook->makePetInfo(&petInfo,lpItem->m_dwSerial);
			iAvatarIndex	=	lpBRBook->m_bf4TamerSlot;
			iTamerLevel		=	lpBRBook->m_bf11TamerLevel;
		}
		else
		{
			memcpy(&petInfo,&lpItem->m_bCount,sizeof(cPetDefine)-4);
			petInfo.m_dwExperience		=	lpItem->m_dwSerial;
			iAvatarIndex				=	petInfo.m_bf2TamerSlot;
		}

		char	strPetName[dPET_NAME_LENGTH+2];

		memcpy(strPetName,petInfo.m_strName,dPET_NAME_LENGTH);
		strPetName[dPET_NAME_LENGTH]	=	NULL;

		sprintf(strCode,"#br [%d:%d] name \"%s\" avatarIndex [%d] code [%d,%d,%d,%d,%d,%d,%d]",
			lpItem->m_wBaseItem,lpItem->m_dwSerial,strPetName,iAvatarIndex,
			petInfo.m_wBreedingTime,petInfo.m_bf14Loyalty,petInfo.m_bf10Level,petInfo.m_bf22CurrentHP,petInfo.getJobIndex(),petInfo.m_bf5BoostCount,iTamerLevel);
	}
	else
	if	(lpItem->isCoupleRing())
	{
		cCoupleRingDefine	*lpRing	=	(cCoupleRingDefine	*)lpItem;

		char	strCoupleName[dNAME_LENGTH];

		memcpy(strCoupleName,lpRing->m_strCoupleName,dNAME_LENGTH-2);
		strCoupleName[dNAME_LENGTH-2]	=	NULL;

		sprintf(strCode,"#cr [%d:%d] name \"%s\" [%d,%d,%d]",
				lpRing->m_wBaseItem,lpRing->m_dwSerial,strCoupleName,lpRing->m_bf1IsCanNotTradeItem,lpRing->m_aOption[0],lpRing->m_aOption[1]);
	}
	else
	{
		sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d expireDate %d/%d/%d %d:%d",
								lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
								lpItem->m_aOption[0],lpItem->m_aOption[1],
								lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
								lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
								lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
								lpItem->getBitFieldValue(),
								lpItem->m_year,lpItem->m_month,lpItem->m_day,lpItem->m_hour,lpItem->m_minute);
	}

	SetDlgItemText(IDC_ITEM_CODE,strCode);
}

void cInventory::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
