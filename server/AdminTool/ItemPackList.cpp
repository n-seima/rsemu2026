// ItemPackList.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "ItemPackList.h"
#include "cMessage.h"
#include "cPACKET_WORLDSERVER.h"
#include "cMakeItem.h"
#include "cDECODER.h"
#include "MSGDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cItemPackList dialog


cItemPackList::cItemPackList(CWnd* pParent /*=NULL*/)
	: CPropertyPage(cItemPackList::IDD)
{
	m_bIsWantCurrentInfo	=	FALSE;
	m_bIsWantAllItemPackList=	FALSE;
	//{{AFX_DATA_INIT(cItemPackList)
	//}}AFX_DATA_INIT
}


void cItemPackList::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cItemPackList)
	DDX_Control(pDX, IDC_LINK_ITEM, m_cbLinkItem);
	DDX_Control(pDX, IDC_PACK_LIST, m_itemPackLlist);
	DDX_Control(pDX, IDC_EDIT_PACK_NAME, m_ebPackName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cItemPackList, CPropertyPage)
	//{{AFX_MSG_MAP(cItemPackList)
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_ASK_PACK_LIST, OnAskPackList)
	ON_BN_CLICKED(IDC_REMOVE_PACK, OnRemovePack)
	ON_BN_CLICKED(IDC_CHANGE_NAME, OnChangeName)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PACK_LIST, OnItemchangedPackList)
	ON_BN_CLICKED(IDC_ADD_ITEM, OnAddItem)
	ON_BN_CLICKED(IDC_APPLY, OnApplyList)
	ON_BN_CLICKED(IDC_REMOVE_ITEM, OnRemoveItem)
	ON_BN_CLICKED(IDC_MODIFY_DATA, OnModifyData)
	ON_BN_CLICKED(IDC_RESET_CONTENTS, OnResetContents)
	ON_BN_CLICKED(IDC_ASK_SELECT_SERVER_PACK_INFO, OnAskSelectServerPackInfo)
	ON_BN_CLICKED(IDC_EDIT_ITEM, OnEditItem)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_CBN_SELCHANGE(IDC_LINK_ITEM, OnSelchangeLinkItem)
	ON_BN_CLICKED(IDC_SAVE_TO_FILE, OnSaveToFile)
	ON_BN_CLICKED(IDC_LOAD_FROM_FILE, OnLoadFromFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cItemPackList message handlers

BOOL
cItemPackList::check(int _iLimitLevel,BOOL _bIsCheckReceivedList)
{
	if	(m_bIsWantAllItemPackList)
	{
		MessageBox("wait to Receive all item pack list","warning!");
		return	FALSE;
	}

	if	(!CheckOperatorLevel(_iLimitLevel))
		return FALSE;

//	if	(!isConnected())
//		return FALSE;
/*
	if	(_bIsCheckReceivedList	&&	m_bIsReceiveData	==	FALSE)
	{
		MessageBox("require press 'Ask pack List'","error!",MB_ICONERROR);

		return	FALSE;
	}
*/
	return	TRUE;
}

BOOL
cItemPackList::isConnected()
{
	return	IsConnectedAllServer(TRUE,TRUE);
}

void
cItemPackList::reset()
{
	for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
		m_aPacks[i].reset();

	m_iSelectedPack	=	-1;
	m_iPackCount	=	0;
}

void
cItemPackList::updateList(int _iSelect,BOOL _bIsWantSelect)
{
	if	(_iSelect	!=	-1)
		m_iSelectedPack	=	_iSelect;

	m_listItemPack.reset();

	if	(m_iPackCount	==	0)
	{
		m_listItemPack.addItem(0,0,"",0);
		m_listItemPack.setItem(0,1,"invalid List");
		m_iSelectedPack	=	-1;

		return;
	}

	int	iCount	=	0;

	for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
	{
		if	(m_aPacks[i].m_wLinkItem	==	0xffff)
			continue;

		m_listItemPack.addItem(iCount,0,_ms("%.3d",i),i);
		m_listItemPack.setItem(iCount,1,m_aPacks[i].m_strName);

		iCount++;
	}

	if	(_bIsWantSelect)
		m_listItemPack.selectItem(m_iSelectedPack);

	updateItemList();
}

void
cItemPackList::updateItemList()
{
	m_listItem.reset();

	if	(m_iSelectedPack	==	-1)
		return;

	for (int i=0;i<m_editPack.m_wCount;i++)
	{
		char		*strOX[]	=	{"X","O"};
		char		strItemName[128];

		cItemInPack	*lpItem	=	&m_editPack.m_aItems[i];

		strcpy(strItemName,((cITEM*)lpItem)->getName());

		m_listItem.addItem(i,0,_ms("%.3d",i),i);
		m_listItem.setItem(i,1,strItemName);
		m_listItem.setItem(i,2,_ms("1/%d",lpItem->m_dwFrequency));
		m_listItem.setItem(i,3,_ms("%d",lpItem->m_wLimitCountOfServer));
		m_listItem.setItem(i,4,_ms("%s",strOX[lpItem->m_bf1IsPrizeWinningItem]));
		m_listItem.setItem(i,5,_ms("%s",strOX[lpItem->m_bf1IsBlockToTrade]));
	}

	CheckDlgButton(IDC_SELECT_ABLE,m_editPack.m_bf1IsSelectOneItem);
	CheckDlgButton(IDC_SELECT_ONE_PREFIX,m_editPack.m_bf1IsSelectOnePrefix);

	if (m_editPack.m_wLinkItem	==	0xffff)
		m_cbLinkItem.SetCurSel(0);
	else
		m_cbLinkItem.SelectString(0,g_aBasicItem[m_editPack.m_wLinkItem].m_strName);
}

void
cItemPackList::receiveResetDropPackItemCount(int _iPackSerial)
{
	cItemPackInfo	*lpPackInfo	=	&m_aPacks[_iPackSerial];

	for (int i=0;i<lpPackInfo->m_wCount;i++)
		lpPackInfo->m_aItems[i].m_wCurrentCount	=	0;

	m_currentInfo.updateItemList();
}

void
cItemPackList::savePackList()
{
	FILE	*fp	=	fopen(_ms("%s/data/___itemPackList.ipd",g_strGameFolder),"wb");

	if	(fp)
	{
		fwrite(m_aPacks,sizeof(m_aPacks),1,fp);
		fclose(fp);
	}
}

void
cItemPackList::sendItemPackInfo(int _iWorldIndex)
{
	CMSGDLG		cProcessingDLG;
	char		strProcessingMsg[1024];
	int			iMaxProgress = dMAX_ITEM_PACK_COUNT;
	int			iCurProgress = 0;

	cProcessingDLG.Create(IDD_MSG_DIALOG);
	cProcessingDLG.m_ctlProgress.SetRange(0,iMaxProgress);
	cProcessingDLG.ShowWindow(SW_SHOWNORMAL);

	sprintf(strProcessingMsg,"Update ... Wait a minute~!");

	cProcessingDLG.m_ctlMsg.SetWindowText(strProcessingMsg);

	iCurProgress	=	0;
	cProcessingDLG.m_ctlProgress.SetPos(iCurProgress);

	{
		if	(!g_db.Connect(	sList[_iWorldIndex].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
		{
			MessageBox(g_strToolMessage[38]);
			return;
		}

		for(int iPack = dMAX_ITEM_PACK_COUNT;iPack;)
		{
			--iPack;
			m_aPacks[iPack].m_wSerial	=	iPack;
			g_db.spEvent_ItemPack_Create(iPack,&m_aPacks[iPack]);
			iCurProgress++;
			cProcessingDLG.m_ctlProgress.SetPos(iCurProgress);
			Sleep(1);
		}

		g_db.Close();
	}

}

BOOL cItemPackList::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SetLogFolder();
	CreateDirectory("Export item pack",NULL);
	SetLogFolder();

	reset();

	m_bIsReceiveData	=	FALSE;
/*
	FILE	*fp	=	fopen(_ms("%s/data/itemPackList.dat",g_strGameFolder),"rb");

	if (fp)
	{
		fread(m_aPacks,sizeof(m_aPacks),1,fp);
		fclose(fp);
	}
*/
	m_iPackCount	=	0;

	for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
	{
		if	(m_aPacks[i].m_wLinkItem	==	0xffff)
			continue;

		m_iPackCount++;
	}

	m_listItemPack.init(GetDlgItem(IDC_PACK_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);

	m_listItemPack.addColumn(0,30,"NO");
	m_listItemPack.addColumn(1,150,"Name");
	m_listItemPack.reset();

	updateList();
//	m_listItemPack.addItem(0,0,"",0);
//	m_listItemPack.setItem(0,1,"please ask info");

	m_editPack.reset();

	fillLinkItemCombo();

	m_listItem.init(GetDlgItem(IDC_ITEM_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	m_listItem.addColumn(0,30,"NO");
	m_listItem.addColumn(1,280,"Name");
	m_listItem.addColumn(2,75,"Frequency");
	m_listItem.addColumn(3,60,"Limit");
	m_listItem.addColumn(4,24,"W");
	m_listItem.addColumn(5,24,"B");

	updateItemList();

	m_iSelectedPack	=	-1;

	return TRUE;
}

void cItemPackList::OnCreate() 
{
	if	(!check(eAL_OPERATOR,FALSE))
		return;

	int		iValidPack	=	0xffff;
	char	strName[32];

	m_ebPackName.GetWindowText(strName,sizeof(strName)-2);

	if	(strlen(strName) <= 0)
		return;

	for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
	{
		if	(iValidPack	==	0xffff	&&	m_aPacks[i].m_wLinkItem	==	0xffff)
			iValidPack	=	i;

		if	(m_aPacks[i].m_wLinkItem	==	0xffff)
			continue;

		if	(stricmp(strName,m_aPacks[i].m_strName)	==	0)
		{
			MessageBox(_ms("already exist same name pack.\n\nindex : %d",i),"error");

			return;
		}
	}

	if	(iValidPack	==	0xffff)
		return;

	m_aPacks[iValidPack].reset();
	m_aPacks[iValidPack].m_wSerial	=	iValidPack;

	strcpy(m_aPacks[iValidPack].m_strName,strName);
	
	{
		int	iLinkItem					=	m_cbLinkItem.GetCurSel();
		m_aPacks[iValidPack].m_wLinkItem=	(WORD)m_cbLinkItem.GetItemData(iLinkItem);
	}

	m_iPackCount++;

	BOOL	bIsOK;

	int		iSerial	=	-1;

	if	(m_iSelectedPack	>=	0)
	{
		iSerial	=	m_listItemPack.getLParam(m_iSelectedPack,0,&bIsOK);

		if	(bIsOK	==	FALSE)
			iSerial	=	-1;
	}

	if	(iSerial	==	-1)
	{
		iSerial		=	iValidPack;
		memcpy(&m_editPack,&m_aPacks[iValidPack],sizeof(m_editPack));
	}

	if	(iSerial	!=	-1)
	{
		int	iCount	=	0;

		for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
		{
			if	(m_aPacks[i].m_wLinkItem	==	0xffff)
				continue;

			if	(i	==	iSerial)
				break;

			iCount++;
		}

		m_iSelectedPack	=	iCount;
	}

	updateList(-1);

	savePackList();
}

void cItemPackList::OnAskPackList() 
{
	if	(!check(eAL_OPERATOR,FALSE))
		return;

	int	iSelectServer			=	g_iSelectedServer;

	m_bIsWantCurrentInfo		=	FALSE;

	if	(iSelectServer	==	-1)
	{
		cMSG::Put("error!","please select world");
		return;
	}

	reset();

	m_bIsWantAllItemPackList	=	TRUE;

	askItemPackList(0);
}

void 
cItemPackList::askItemPackList(int _iPackIndex)
{
	CMSGDLG cProcessingDLG;

	char	strProcessingMsg[1024];
	int		iMaxProgress	=	dMAX_ITEM_PACK_COUNT;
	int		iCurProgress	=	0;
	
	cProcessingDLG.Create(IDD_MSG_DIALOG);	
	cProcessingDLG.m_ctlProgress.SetRange(0,iMaxProgress);
	cProcessingDLG.ShowWindow(SW_SHOWNORMAL);

	sprintf(strProcessingMsg,"Downloading ... Wait a minute~!");

	cProcessingDLG.m_ctlMsg.SetWindowText(strProcessingMsg);

	{
		if	(!g_db.Connect(	sList[g_iSelectedServer].strDBIP , strAvatarDBCId,strAvatarDBCPw,"RedStone_Avatar_Source",1))
		{
			MessageBox(g_strToolMessage[38]);
			return;
		}

		m_iPackCount	=	0;

		for(int i = dMAX_ITEM_PACK_COUNT;i;)
		{
			--i;

			g_db.spEvent_ItemPack_Get(i,&m_aPacks[i]);
			iCurProgress++;
			cProcessingDLG.m_ctlProgress.SetPos(iCurProgress);

			if	(m_aPacks[i].m_wLinkItem	!=	0xffff)
			{
				memcpy(&m_editPack,&m_aPacks[i],sizeof(cItemPackInfo));
				m_iPackCount++;
			}

			Sleep(1);
		}

		g_db.Close();
	}

	m_bIsReceiveData		=	TRUE;
	m_bIsWantAllItemPackList=	FALSE;

	updateList();

	MessageBox("receive item pack list","work complete");
}

void cItemPackList::OnRemovePack() 
{
	if	(!check(eAL_OPERATOR))
		return;

	// TODO: Add your control notification handler code here
	BOOL	bIsOk;
	int		iSelectedPack	=	m_listItemPack.getSelectedItem();

	if	(iSelectedPack	==	-1)
		return;

	int		iSerial			=	m_listItemPack.getLParam(iSelectedPack,0,&bIsOk);

	if	(bIsOk	==	FALSE)
		return;

	if	(cMSG::YESNO("remove select item pack","really?")	==	IDNO)
		return;

	m_listItemPack.delItem(iSelectedPack);
	m_iSelectedPack	=	iSelectedPack;

	m_aPacks[iSerial].reset();
	m_iPackCount--;

	if	(m_listItemPack.getLineCount()==	0	||	m_iPackCount	==	0)
	{
		m_iPackCount	=	0;

		updateList();
	}

	if	(m_iSelectedPack>=	m_iPackCount)
		m_iSelectedPack	=	m_iPackCount-1;

	m_editPack.reset();

	if	(m_iSelectedPack>=	0)
	{
		iSerial			=	m_listItemPack.getLParam(m_iSelectedPack,0,&bIsOk);

		memcpy(&m_editPack,&m_aPacks[iSerial],sizeof(m_editPack));

		m_listItemPack.selectItem(m_iSelectedPack);
	}

	updateItemList();
	savePackList();
}

void cItemPackList::OnChangeName() 
{
	if	(!check(eAL_OPERATOR))
		return;

	BOOL	bIsOk;
	int		iSelectedPack	=	m_listItemPack.getSelectedItem();

	if (iSelectedPack	==	-1)
		return;

	int		iSerial			=	m_listItemPack.getLParam(iSelectedPack,0,&bIsOk);

	if (bIsOk	==	FALSE)
		return;

	char	strName[32];

	m_ebPackName.GetWindowText(strName,sizeof(strName)-2);

	for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
	{
		if	(m_aPacks[i].m_wLinkItem	==	0xffff)
			continue;

		if (stricmp(strName,m_aPacks[i].m_strName)	==	0)
		{
			MessageBox(_ms("already exist same name pack.\n\nindex : %d",i),"error");

			return;
		}
	}

	strcpy(m_editPack.m_strName,strName);
//	strcpy(m_aPacks[iSerial].m_strName,strName);

	savePackList();
	
	m_listItemPack.setItem(iSelectedPack,1,strName);

	lostVirgin();
}

void cItemPackList::OnItemchangedPackList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;

	if	(pNMListView->uOldState	==	0)
	{
		BOOL	bIsOk;
		int		iSelectPack	=	m_listItemPack.getLParam(m_iSelectedPack,0,&bIsOk);

		if	(m_iSelectedPack	!=	-1)
		{
			iSelectPack	=	m_listItemPack.getLParam(m_iSelectedPack,0,&bIsOk);

			if	(m_editPack.getCheckSum()	!=	m_aPacks[iSelectPack].getCheckSum())
			{
				int	iResult	=	cMSG::YESNO("modifyed some item data","do you want apply this?");

				if	(iResult==	IDYES)
				{
					int	iLinkItem			=	m_cbLinkItem.GetCurSel();
					m_editPack.m_wLinkItem	=	(WORD)m_cbLinkItem.GetItemData(iLinkItem);

					memcpy(&m_aPacks[iSelectPack],&m_editPack,sizeof(m_editPack));

					m_aPacks[iSelectPack].m_wSerial	=	iSelectPack;

					savePackList();
				}
			}

			setVirgin();
		}

		if	(m_iPackCount)
		{
			iSelectPack	=	m_listItemPack.getLParam(pNMListView->iItem,0,&bIsOk);
			memcpy(&m_editPack,&m_aPacks[iSelectPack],sizeof(m_editPack));

			m_iSelectedPack	=	pNMListView->iItem;

			m_ebPackName.SetWindowText(m_editPack.m_strName);

			updateItemList();
		}
	}
}

void cItemPackList::OnAddItem() 
{
/*	cDECODER	decoder;

	if (!decoder.Upload(_ms("%s/santa.txt",g_strLogFolder),NULL))
		return;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		BOOL		bIsOK;
		cItemInPack	item;

		item.reset();

		if (stricmp(lpstrWord,"normal")==0)
		{
			item.m_bCount				=	decoder.GetNumber(&bIsOK);
			item.m_wBaseItem			=	decoder.GetNumber(&bIsOK);

			cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[item.m_wBaseItem];

			item.m_aOption[0]			=	(BYTE)lpBasicItem->m_aValue[0][1];
			item.m_aOption[1]			=	(BYTE)lpBasicItem->m_aValue[1][1];

			item.m_dwFrequency			=	decoder.GetNumber(&bIsOK);
			item.m_wLimitCountOfServer	=	decoder.GetNumber(&bIsOK);

			if (item.m_dwFrequency)
			{
				item.m_dwFrequency		/=	20;
				item.m_dwFrequency		=	max(item.m_dwFrequency,1);
			}

			memcpy(&m_editPack.m_aItems[m_editPack.m_wCount],&item,sizeof(item));

			m_editPack.m_wCount++;

			continue;
		}
		if (stricmp(lpstrWord,"custom")==0)
		{
			item.m_bCount					=	decoder.GetNumber(&bIsOK);
			item.m_wBaseItem				=	decoder.GetNumber(&bIsOK);
			item.m_dwSerial					=	decoder.GetNumber(&bIsOK);
			item.m_bCount					=	decoder.GetNumber(&bIsOK);
			item.m_bDurability				=	decoder.GetNumber(&bIsOK);

			item.m_aOption[0]				=	decoder.GetNumber(&bIsOK);
			item.m_aOption[1]				=	decoder.GetNumber(&bIsOK);

			item.m_aPrefix[0].m_wPrefix		=	decoder.GetNumber(&bIsOK);
			item.m_aPrefix[0].m_aValue[0]	=	decoder.GetNumber(&bIsOK);
			item.m_aPrefix[0].m_aValue[1]	=	decoder.GetNumber(&bIsOK);

			item.m_aPrefix[1].m_wPrefix		=	decoder.GetNumber(&bIsOK);
			item.m_aPrefix[1].m_aValue[0]	=	decoder.GetNumber(&bIsOK);
			item.m_aPrefix[1].m_aValue[1]	=	decoder.GetNumber(&bIsOK);

			item.m_aPrefix[2].m_wPrefix		=	decoder.GetNumber(&bIsOK);
			item.m_aPrefix[2].m_aValue[0]	=	decoder.GetNumber(&bIsOK);
			item.m_aPrefix[2].m_aValue[1]	=	decoder.GetNumber(&bIsOK);

			decoder.GetNumber(&bIsOK);

			item.setBitFieldValue(60);

			item.m_dwFrequency				=	decoder.GetNumber(&bIsOK);
			item.m_wLimitCountOfServer		=	decoder.GetNumber(&bIsOK);

			if (item.m_dwFrequency)
			{
				item.m_dwFrequency		/=	20;
				item.m_dwFrequency		=	max(item.m_dwFrequency,1);
			}

			memcpy(&m_editPack.m_aItems[m_editPack.m_wCount],&item,sizeof(item));

			m_editPack.m_wCount++;

			continue;
		}

		if (stricmp(lpstrWord,"end")==0)
			break;
	}

	memcpy(&m_aPacks[0],&m_editPack,sizeof(m_editPack));

	m_editPack.m_wLinkItem	=	0;

	updateItemList();

	savePackList();

	return;
*/
	if	(!check(eAL_OPERATOR))
		return;

	int		iSelectedPack	=	m_listItemPack.getSelectedItem();

	if (iSelectedPack	==	-1)
	{
		MessageBox("please select Item pack");
		return;
	}

	if (m_editPack.m_wCount	>=	dMAX_ITEM_IN_ITEM_PACK)
	{
		MessageBox("pack is full");
		return;
	}

	// TODO: Add your control notification handler code here
	cMakeItem	m_dlgMakeItem;

	m_dlgMakeItem.m_makeItem.reset();

	if (m_dlgMakeItem.DoModal()	==	IDCANCEL)
		return;

	cItemInPack	*lpItem	=	&m_dlgMakeItem.m_makeItem;

	memcpy(&m_editPack.m_aItems[m_editPack.m_wCount],lpItem,sizeof(cItemInPack));

	char	strItemName[128];
	strcpy(strItemName,((cITEM*)lpItem)->getName());

	char	*strOX[]	=	{"X","O"};

	m_listItem.addItem(m_editPack.m_wCount,0,_ms("%.3d",m_editPack.m_wCount),m_editPack.m_wCount);
	m_listItem.setItem(m_editPack.m_wCount,1,strItemName);
	m_listItem.setItem(m_editPack.m_wCount,2,_ms("1/%d",lpItem->m_dwFrequency));
	m_listItem.setItem(m_editPack.m_wCount,3,_ms("%d",lpItem->m_wLimitCountOfServer));
	m_listItem.setItem(m_editPack.m_wCount,4,_ms("%s",strOX[lpItem->m_bf1IsBlockToTrade]));
	m_listItem.setItem(m_editPack.m_wCount,5,_ms("%s",strOX[lpItem->m_bf1IsPrizeWinningItem]));

	m_editPack.m_wCount++;

	lostVirgin();
}

void cItemPackList::OnApplyList() 
{
	if	(!check(eAL_OPERATOR))
		return;

	int		iSelectedPack	=	m_listItemPack.getSelectedItem();
	BOOL	bIsOk;
	
	if	(iSelectedPack	==	-1)
	{
		MessageBox("please select Item pack");
		return;
	}

	int		iSerial			=	m_listItemPack.getLParam(iSelectedPack,0,&bIsOk);

	if	(cMSG::YESNO("Apply this item list on select item pack","Do you want continue this work?")	==	IDNO)
		return;

	int	iLinkItem			=	m_cbLinkItem.GetCurSel();
	m_editPack.m_wLinkItem	=	(WORD)m_cbLinkItem.GetItemData(iLinkItem);
	m_editPack.m_bf1IsSelectOneItem	=	IsDlgButtonChecked(IDC_SELECT_ABLE);
	m_editPack.m_bf1IsSelectOnePrefix=	IsDlgButtonChecked(IDC_SELECT_ONE_PREFIX);

	memcpy(&m_aPacks[iSerial],&m_editPack,sizeof(m_editPack));

	m_aPacks[iSerial].m_wSerial	=	iSerial;

	savePackList();

	setVirgin();
}

void cItemPackList::OnRemoveItem() 
{
	if	(!check(eAL_OPERATOR))
		return;

	int		iSelectedItem	=	m_listItem.getSelectedItem();

	if (iSelectedItem	==	-1)
	{
		MessageBox("please select item");
		return;
	}

	for (int i=iSelectedItem;i<m_editPack.m_wCount-1;i++)
		memcpy(&m_editPack.m_aItems[i],&m_editPack.m_aItems[i+1],sizeof(cItemInPack));

	m_editPack.m_wCount--;

	m_editPack.m_aItems[m_editPack.m_wCount].reset();

	updateItemList();

	savePackList();

	lostVirgin();
}

void cItemPackList::OnModifyData() 
{
	if	(!check(eAL_MASTER_OPERATER))
	{
		savePackList();
		return;
	}

	if	(g_iSelectedServer	==	-1)
	{
		MessageBox("please select server"," ");
		return;
	}

	if	(cMSG::YESNO("warning","this work modify all item pack info\n\nDo you want continue this work?")	==	IDNO)
		return;

	if	(isVirgin()	==	FALSE)
	{
		int		iSelectedPack	=	m_listItemPack.getSelectedItem();
		BOOL	bIsOk;

		if (iSelectedPack	!=	-1)
		{
			int	iSelectPack	=	m_listItemPack.getLParam(m_iSelectedPack,0,&bIsOk);
			int	iResult		=	cMSG::YESNO("modifyed some item data","do you want apply this?");

			if (iResult	==	IDYES)
			{
				int	iLinkItem			=	m_cbLinkItem.GetCurSel();
				m_editPack.m_wLinkItem	=	(WORD)m_cbLinkItem.GetItemData(iLinkItem);

				memcpy(&m_aPacks[iSelectPack],&m_editPack,sizeof(m_editPack));
				m_aPacks[iSelectPack].m_wSerial	=	iSelectPack;
			}
		}
	}

	savePackList();

	sendItemPackInfo(g_iSelectedServer);

	MessageBox("complete modify all item pack list","message");
}

void cItemPackList::OnResetContents() 
{
	if	(!check(eAL_OPERATOR))
		return;

	if	(cMSG::YESNO("warning","this work remove all item in select item pack\n\nDo you want continue this work?")	==	IDNO)
		return;

	m_editPack.m_wCount	=	0;

	updateItemList();

	lostVirgin();
}

void
cItemPackList::fillLinkItemCombo()
{
	int	iIndex,i;

	m_cbLinkItem.ResetContent();

	for (i=0;i<cITEM::s_iCount;i++)
	{
		if (g_aBasicItem[i].m_wKind	!=	eIK_ITEM_PACK)
			continue;

		iIndex	=	m_cbLinkItem.AddString(g_aBasicItem[i].m_strName);

		m_cbLinkItem.SetItemData(iIndex,i);
	}

	m_cbLinkItem.SetCurSel(0);
}

void cItemPackList::OnAskSelectServerPackInfo() 
{
	// TODO: Add your control notification handler code here
	if	(!check(eAL_OPERATOR))
		return;

	if (g_iSelectedServer	==	-1)
	{
		MessageBox("please select server"," ");
		return;
	}

	int	iSerial	=	-1;

	if	(m_iSelectedPack	>=	0)
	{
		BOOL	bIsOK;

		iSerial	=	m_listItemPack.getLParam(m_iSelectedPack,0,&bIsOK);

		if	(bIsOK	==	FALSE)
			iSerial	=	-1;
	}

	if	(iSerial	==	-1)
	{
		MessageBox("please select item pack"," ");
		return;
	}

	m_currentInfo.m_iSelectServer		=	g_iSelectedServer;

	memcpy(&m_currentInfo.m_editPack,&m_aPacks[iSerial],sizeof(m_currentInfo.m_editPack));

	m_currentInfo.DoModal();
}

void cItemPackList::OnEditItem() 
{
	// TODO: Add your control notification handler code here
	if (!check(eAL_OPERATOR))
		return;

	int		iSelectedPack	=	m_listItemPack.getSelectedItem();

	if (iSelectedPack	==	-1)
	{
		MessageBox("please select Item pack");
		return;
	}

	int		iSelectedItem	=	m_listItem.getSelectedItem();

	if (iSelectedItem	==	-1)
	{
		MessageBox("please select item");
		return;
	}

	// TODO: Add your control notification handler code here
	cMakeItem	m_dlgMakeItem;

	memcpy(&m_dlgMakeItem.m_makeItem,&m_editPack.m_aItems[iSelectedItem],sizeof(cItemInPack));

	if (m_dlgMakeItem.DoModal()	==	IDCANCEL)
		return;

	cItemInPack	*lpItem	=	&m_dlgMakeItem.m_makeItem;

	memcpy(&m_editPack.m_aItems[iSelectedItem],lpItem,sizeof(cItemInPack));

	updateItemList();

	m_listItem.selectItem(iSelectedItem);

	lostVirgin();

}

void cItemPackList::OnExport() 
{
	// TODO: Add your control notification handler code here
	int		iSelectedPack	=	m_listItemPack.getSelectedItem(),iSelectPack;
	BOOL	bIsOk;

	if (iSelectedPack	==	-1)
	{
		MessageBox("please select Item pack");
		return;
	}

	iSelectPack	=	m_listItemPack.getLParam(m_iSelectedPack,0,&bIsOk);

	if (m_editPack.getCheckSum()	!=	m_aPacks[iSelectPack].getCheckSum())
	{
		int	iResult	=	cMSG::YESNO("modifyed some item data","do you want apply this?");

		if (iResult	==	IDYES)
		{
			int	iLinkItem			=	m_cbLinkItem.GetCurSel();
			m_editPack.m_wLinkItem	=	(WORD)m_cbLinkItem.GetItemData(iLinkItem);

			memcpy(&m_aPacks[iSelectPack],&m_editPack,sizeof(m_editPack));
			m_aPacks[iSelectPack].m_wSerial	=	iSelectPack;
		}
	}

	OPENFILENAME	ofn;

	char	dir[512];
	char	strName[256]	=	"";

	SetCurrentDirectory(g_strLogFolder);

	sprintf(dir,"%s\\export item pack",g_strLogFolder);

	SetCurrentDirectory(dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.eid";
	ofn.lpstrTitle		=	"export item pack data";
	ofn.lpstrFile		=	strName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"eid";
	ofn.lpstrInitialDir	=	dir;

	if (!GetSaveFileName(&ofn))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	cFILE	file;

	file.Open(strName,"wb");

	file.Write(&m_aPacks[iSelectPack],sizeof(m_aPacks[iSelectPack]));

	file.Close();

	SetCurrentDirectory(g_strLogFolder);
}

void cItemPackList::OnImport() 
{
	int		iSelectedPack	=	m_listItemPack.getSelectedItem(),iSelectPack;
	BOOL	bIsOk;

	if (iSelectedPack	==	-1)
	{
		MessageBox("please select Item pack");
		return;
	}

	iSelectPack	=	m_listItemPack.getLParam(m_iSelectedPack,0,&bIsOk);

	OPENFILENAME	ofn;

	char	dir[512];
	char	strName[256]	=	"";

	SetCurrentDirectory(g_strLogFolder);

	sprintf(dir,"%s\\export item pack",g_strLogFolder);

	SetCurrentDirectory(dir);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow()->m_hWnd;
	ofn.lpstrFilter		=	"*.eid";
	ofn.lpstrTitle		=	"export item pack data";
	ofn.lpstrFile		=	strName;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING;
	ofn.lpstrDefExt		=	"eid";
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	cFILE	file;

	file.Open(strName,"rb");

	strcpy(strName,m_editPack.m_strName);
	file.Read(&m_editPack,sizeof(m_editPack));
	strcpy(m_editPack.m_strName,strName);

	file.Close();

	SetCurrentDirectory(g_strLogFolder);

	updateItemList();
}

void cItemPackList::OnSelchangeLinkItem() 
{
	// TODO: Add your control notification handler code here
	int	iLinkItem			=	m_cbLinkItem.GetCurSel();

	if	(iLinkItem	==	-1)
		return;
	
	m_editPack.m_wLinkItem	=	(WORD)m_cbLinkItem.GetItemData(iLinkItem);
}

void cItemPackList::OnSaveToFile() 
{
	// TODO: Add your control notification handler code here
	char	dir[512];
	char	strName[256]	=	"";

	strcpy(dir,g_strLogFolder);

	SetCurrentDirectory(dir);

	if	(!cFILE::SaveBox(strName,"item pack data {*.ipd}\0*.ipd\0\0","ipd",dir,"please select file"))//GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}


	FILE	*fp	=	fopen(strName,"wb");

	if	(fp)
	{
		fwrite(m_aPacks,sizeof(m_aPacks),1,fp);
		fclose(fp);
	}
}

void cItemPackList::OnLoadFromFile() 
{
	char	dir[512];
	char	strName[256]	=	"";

	strcpy(dir,g_strLogFolder);

	SetCurrentDirectory(dir);

	if	(!cFILE::LoadBox(strName,"item pack data {*.ipd}\0*.ipd\0\0","ipd",dir,"please select file"))//GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(g_strLogFolder);

		return;
	}

	FILE	*fp	=	fopen(strName,"rb");

	if	(fp)
	{
		fread(m_aPacks,sizeof(m_aPacks),1,fp);
		fclose(fp);
	}

	m_iSelectedPack	=	-1;

	{
		int	iCount				=	0;
		int	iSelectPackSerial	=	0;

		m_iPackCount			=	0;

		for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
		{
			if	(m_aPacks[i].m_wLinkItem	==	0xffff)
				continue;

			if	(m_iPackCount	==	0)
			{
				iSelectPackSerial	=	i;
				m_iSelectedPack		=	0;
			}

			m_iPackCount++;
		}

		if	(m_iSelectedPack	!=	-1)
			memcpy(&m_editPack,&m_aPacks[iSelectPackSerial],sizeof(m_editPack));
	}

	updateList();
}