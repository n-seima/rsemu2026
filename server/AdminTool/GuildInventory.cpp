// GuildInventory.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "GuildInventory.h"
#include "CGuildDefine.h"
#include "CharacterInfo.h"
#include "cMakeItem.h"
#include "cDECODER.h"
#include "cMESSAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuildInventory dialog


CGuildInventory::CGuildInventory(CWnd* pParent /*=NULL*/)
	: CDialog(CGuildInventory::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuildInventory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGuildInventory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuildInventory)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGuildInventory, CDialog)
	//{{AFX_MSG_MAP(CGuildInventory)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_REMOVE_BY_CODE, OnRemoveByCode)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_BN_CLICKED(IDC_ADD_GOLD, OnAddGold)
	ON_BN_CLICKED(IDC_REMOVE_GOLD, OnRemoveGold)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_GET_ITEM_CODE, OnGetItemCode)
	ON_BN_CLICKED(IDC_MAKE, OnMake)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuildInventory message handlers

BOOL CGuildInventory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.init(GetDlgItem(IDC_ITEM_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_ITEM_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_list.addColumn(0,90,"Slot");
	m_list.addColumn(1,46,"Count");
	m_list.addColumn(2,11*6,"End Date");
	m_list.addColumn(3,360,"Name");

	updateList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void
CGuildInventory::setGuild(CGuildAdvanceInfo	*_lpGuild)
{
	m_lpGuild	=	_lpGuild;
}

void
CGuildInventory::setItemInfoToList(cListView *_lpListView,int _iIndex,cITEM *_lpItem)
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
			int			iAvatarIndex	=	0,iTamerLevel		=	0;

			if	(lpBRBook->isAvailInfo(_lpItem->m_dwSerial))
			{
				lpBRBook->makePetInfo(&petInfo,_lpItem->m_dwSerial);
				iAvatarIndex	=	lpBRBook->m_bf4TamerSlot;
				iTamerLevel		=	lpBRBook->m_bf11TamerLevel;
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

				sprintf(strName,"%s Avatar Index %d Lv [%d,%d]",
								strPetName,iAvatarIndex,
								petInfo.m_bf10Level,petInfo.m_dwExperience);
			}
		}

		_lpListView->setItem(_iIndex,1,_ms("%d",iItemCount));
		_lpListView->setItem(_iIndex,2,_lpItem->getExpireDate());
		_lpListView->setItem(_iIndex,3,strName);
	}
}

void
CGuildInventory::updateList()
{
	char	strGoldInfo[128];
	char	strGold[128];

#ifdef	_FOR_KOREA
	{
		int	iUk	=	(int)(m_lpGuild->m_llGuildGold/100000000),iMan=(int)((m_lpGuild->m_llGuildGold/10000)%10000),iGold=(int)(m_lpGuild->m_llGuildGold%10000);

		if	(iUk && iMan && iGold)
		{
			sprintf(strGold,"%d%s %d%s %d",iUk,"억",iMan,"만",iGold);
		}
		else
		if	(iUk && iMan)
		{
			sprintf(strGold,"%d%s %d%s",iUk,"억",iMan,"만");
		}
		else
		if	(iUk && iGold)
		{
			sprintf(strGold,"%d%s %d",iUk,"억",iGold);
		}
		else
		if	(iUk)
		{
			sprintf(strGold,"%d%s",iUk,"억");
		}
		else
		if	(iMan && iGold)
		{
			sprintf(strGold,"%d%s %d",iMan,"만",iGold);
		}
		else
		if	(iMan)
		{
			sprintf(strGold,"%d%s",iMan,"만");
		}
		else
		{
			sprintf(strGold,"%d",iGold);
		}
	}
#else
	{
		int	iD3	=	(int)(m_lpGuild->m_llGuildGold/1000000000),iUk	=	(int)((m_lpGuild->m_llGuildGold/1000000)%1000);
		int	iMan=	(int)((m_lpGuild->m_llGuildGold/1000)%1000),iGold=(int)(m_lpGuild->m_llGuildGold%1000);

		if	(iD3)
		{
			sprintf(strGold,"%d,%.3d,%.3d,%.3d",iD3,iUk,iMan,iGold);
		}
		else
		if	(iUk)
		{
			sprintf(strGold,"%d,%.3d,%.3d",iUk,iMan,iGold);
		}
		else
		if	(iMan)
		{
			sprintf(strGold,"%d,%.3d",iMan,iGold);
		}
		else
			sprintf(strGold,"%d",iGold);
	}
#endif
	sprintf(strGoldInfo,"gold : %s",strGold);
	SetDlgItemText(IDC_STATIC_GOLD_INFO,strGoldInfo);

	int	i;

	m_list.reset();

	for (i=0;i<c_iGuildInventorySize;i++)
	{
		m_list.addItem(i,0,_ms("Inventory #%.2d",i+1),i);

		cITEM	*lpItem	=	(cITEM	*)&m_lpGuild->m_aInventory[i];

		setItemInfoToList(&m_list,i,lpItem);
	}
	
}

void 
CGuildInventory::exportItemList(int _iBaseItem,DWORD _dwSerial)
{
	char	strExportDir[512],strExportFileName[512];
	int		i,iCount=0;

	sprintf(strExportDir,g_strToolMessage[110],g_strLogFolder);

	strcpy(strExportFileName,"guildItemList.txt");

	SetCurrentDirectory(strExportDir);

	cFILE	file;

	file.Open("guildItemList.txt","wt");
	cITEM	*lpItem;

	file.writeStreamCR("guild item list : %s",m_lpGuild->m_strName);
	file.writeStreamCR(g_strToolMessage[114],m_lpGuild->m_llGuildGold);

	iCount	=	0;
	for (i=0;i<c_iGuildInventorySize;i++)
	{
		lpItem	=	(cITEM *)&m_lpGuild->m_aInventory[i];

		if	(lpItem->m_wBaseItem	==	0xffff)
			continue;

		if	(_iBaseItem != 0 && lpItem->m_wBaseItem != _iBaseItem)
			continue;
		if	(_dwSerial  >  1 && lpItem->m_dwSerial  != _dwSerial)
			continue;

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
	
	if	(iCount	==	0)
		file.writeStreamCR("    ▷ No Item");
	else
	{
		file.writeStreamCR("    ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
		file.writeStreamCR(g_strToolMessage[116],iCount);
	}

	file.writeStreamCR(" ");

	file.Close();

	ShellExecute(NULL,"open",strExportFileName,NULL,NULL,SW_SHOWNORMAL);

	SetLogFolder();
	// TODO: Add your control notification handler code here
}

void CGuildInventory::OnAdd() 
{
/*	cITEM		item;
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

		cPetDefine	*lpPetInfo	=	item.getBreedingRecordBookPetInfo();

		if	(!lpPetInfo)
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

		memcpy(lpPetInfo->m_strName,lpstrName,dPET_NAME_LENGTH);

		lpstrWord			=	decoder.GetWord();

		if	(stricmp(lpstrWord,"avatarIndex")	!=	0)
		{	
			cMSG::Put("Error in Add Item!!","Incorrect Item Data");
			return;
		}

		int	iAvatarIndex	=	decoder.GetNumber(&bIsSuccess);
		if	(!bIsSuccess			)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}
		if	(iAvatarIndex	>=	4	)	{cMSG::Put("Error in Add Item!!","Incorrect Item Data");return;}

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

		lpPetInfo->m_bf2TamerSlot	=	iAvatarIndex;
		lpPetInfo->m_wBreedingTime	=	iBreedingTime;
		lpPetInfo->m_bf14Loyalty	=	iLoyalty;
		lpPetInfo->m_bf10Level		=	iLevel;
		lpPetInfo->m_bf22CurrentHP	=	iCurrentHP;
		lpPetInfo->m_bf10Job		=	iJob;
		lpPetInfo->m_bf5BoostCount	=	iBoostCount;
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

	lpSlot	=	m_lpSelectPlayer->getValidItem();

	if	(!lpSlot)
	{
		cMSG::Put("ERROR ITEM ADDITION!!","Not Found  Empty Slot in Character");

		return;
	}

	memcpy(lpSlot,&item,sizeof(item));

	if	(!g_toolLog.addItemToPlayer(&item,m_lpSelectPlayer->m_strId,m_lpSelectPlayer->m_strName,sList[m_iSelectWorld].strName))
		return;

	updateList();

	cMSG::Put("ITEM ADDITION","Complete!!");	
	*/
}

void CGuildInventory::OnRemove() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildInventory::OnRemoveByCode() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildInventory::OnFind() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildInventory::OnAddGold() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildInventory::OnRemoveGold() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildInventory::OnExport() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildInventory::OnGetItemCode() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildInventory::OnMake() 
{
	// TODO: Add your control notification handler code here
	
}

void CGuildInventory::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
