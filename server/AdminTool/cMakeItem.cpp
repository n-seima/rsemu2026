// cMakeItem.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "cMakeItem.h"
#include "cItem.h"
#include "cItem_Prefix.h"
#include "cMessage.h"
#include "cDECODER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cMakeItem dialog


cMakeItem::cMakeItem(CWnd* pParent /*=NULL*/)
	: CDialog(cMakeItem::IDD, pParent)
{
	m_iPrefix1Kind	=	-1;
	m_iPrefix2Kind	=	-1;
	m_iPrefix3Kind	=	-1;
	m_iItemKind		=	-1;

	m_makeItem.reset();

	cITEM::ReadItemData();

	SetLogFolder();
	//{{AFX_DATA_INIT(cMakeItem)
	//}}AFX_DATA_INIT
}


void cMakeItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cMakeItem)
	DDX_Control(pDX, IDC_ITEM_COUNT, m_editCount);
	DDX_Control(pDX, IDC_PREFIX_3_2, m_editPrefix3_2);
	DDX_Control(pDX, IDC_PREFIX_2_2, m_editPrefix2_2);
	DDX_Control(pDX, IDC_PREFIX_1_2, m_editPrefix1_2);
	DDX_Control(pDX, IDC_PREFIX_3_VALUE2, m_stPrefix3Value2);
	DDX_Control(pDX, IDC_PREFIX_2_VALUE2, m_stPrefix2Value2);
	DDX_Control(pDX, IDC_PREFIX_1_VALUE2, m_stPrefix1Value2);
	DDX_Control(pDX, IDC_ITEM_CODE, m_editItemCode);
	DDX_Control(pDX, IDC_FREQUENCY, m_editFrequency);
	DDX_Control(pDX, IDC_LIMIT_COUNT, m_editLimitCount);
	DDX_Control(pDX, IDC_PREFIX_3, m_editPrefix3);
	DDX_Control(pDX, IDC_PREFIX_2, m_editPrefix2);
	DDX_Control(pDX, IDC_PREFIX_1, m_editPrefix1);
	DDX_Control(pDX, IDC_OPTION2, m_editOption2);
	DDX_Control(pDX, IDC_OPTION1, m_editOption1);
	DDX_Control(pDX, IDC_COUNT, m_stCount);
	DDX_Control(pDX, IDC_PREFIX_3_VALUE, m_stPrefix3Value);
	DDX_Control(pDX, IDC_PREFIX_2_VALUE, m_stPrefix2Value);
	DDX_Control(pDX, IDC_PREFIX_1_VALUE, m_stPrefix1Value);
	DDX_Control(pDX, IDC_OPTION_2_VALUE, m_stOption2Value);
	DDX_Control(pDX, IDC_OPTION_1_VALUE, m_stOption1Value);
	DDX_Control(pDX, IDC_ITEM_KIND, m_cbItemKind);
	DDX_Control(pDX, IDC_PREFIX3_KIND, m_cbPrefix3Kind);
	DDX_Control(pDX, IDC_PREFIX2_KIND, m_cbPrefix2Kind);
	DDX_Control(pDX, IDC_PREFIX1_KIND, m_cbPrefix1Kind);
	DDX_Control(pDX, IDC_ITEM_LIST, m_cbItemList);
	DDX_Control(pDX, IDC_PREFIX3, m_cbPrefix3);
	DDX_Control(pDX, IDC_PREFIX2, m_cbPrefix2);
	DDX_Control(pDX, IDC_PREFIX1, m_cbPrefix1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cMakeItem, CDialog)
	//{{AFX_MSG_MAP(cMakeItem)
	ON_CBN_SELCHANGE(IDC_PREFIX1_KIND, OnSelchangePrefix1Kind)
	ON_CBN_SELCHANGE(IDC_PREFIX2_KIND, OnSelchangePrefix2Kind)
	ON_CBN_SELCHANGE(IDC_PREFIX3_KIND, OnSelchangePrefix3Kind)
	ON_CBN_SELCHANGE(IDC_ITEM_KIND, OnSelchangeItemKind)
	ON_CBN_SELCHANGE(IDC_ITEM_LIST, OnSelchangeItemList)
	ON_CBN_SELCHANGE(IDC_PREFIX1, OnSelchangePrefix1)
	ON_CBN_SELCHANGE(IDC_PREFIX2, OnSelchangePrefix2)
	ON_CBN_SELCHANGE(IDC_PREFIX3, OnSelchangePrefix3)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_SET_CATEGORY, OnSetCategory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cMakeItem message handlers

BOOL cMakeItem::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int	iIndex,i;

	for (int iPrefix=0;iPrefix<3;iPrefix++)
	{
		CComboBox	*lpCombo;

		if (iPrefix	==	0)
			lpCombo	=	&m_cbPrefix1Kind;
		if (iPrefix	==	1)
			lpCombo	=	&m_cbPrefix2Kind;
		if (iPrefix	==	2)
			lpCombo	=	&m_cbPrefix3Kind;

		lpCombo->AddString("ALL");
		lpCombo->SetItemData(0,-1);

		for (i=0;;i++)
		{
			if (g_aItemPrefixType[i].m_wType	==	0xffff)
				break;

			iIndex	=	lpCombo->AddString(g_aItemPrefixType[i].m_strName);
			lpCombo->SetItemData(iIndex,g_aItemPrefixType[i].m_wType);
		}

		lpCombo->SetCurSel(0);
	}

	{
		m_cbItemKind.AddString("ALL");
		m_cbItemKind.SetItemData(0,-1);

		for (i=0;;i++)
		{
			if (stricmp(g_strItemCategory[i],"end")	==	0)
				break;

			iIndex	=	m_cbItemKind.AddString(g_strItemCategory[i]);
			m_cbItemKind.SetItemData(iIndex,i);
		}

		m_cbItemKind.SetCurSel(0);
	}

	fillPrefixCombo(0);
	fillPrefixCombo(1);
	fillPrefixCombo(2);
	fillItemCombo();

	m_editPrefix1.SetWindowText("0");
	m_editPrefix1_2.SetWindowText("0");
	m_editPrefix2.SetWindowText("0");
	m_editPrefix2_2.SetWindowText("0");
	m_editPrefix3.SetWindowText("0");
	m_editPrefix3_2.SetWindowText("0");
	m_editOption1.SetWindowText("0");
	m_editOption2.SetWindowText("0");
	m_editCount.SetWindowText("1");

	m_editLimitCount.SetWindowText("0");
	m_editFrequency.SetWindowText("10000");

	OnSelchangeItemList();
	fillItemData();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void
cMakeItem::fillPrefixCombo(int _iIndex)
{
	int	iIndex,i;

	CComboBox	*lpCombo;
	int			iKind;

	if (_iIndex	==	0)
	{
		lpCombo	=	&m_cbPrefix1;
		iKind	=	m_iPrefix1Kind;
	}
	if (_iIndex	==	1)
	{
		lpCombo	=	&m_cbPrefix2;
		iKind	=	m_iPrefix2Kind;
	}
	if (_iIndex	==	2)
	{
		lpCombo	=	&m_cbPrefix3;
		iKind	=	m_iPrefix3Kind;
	}

	lpCombo->ResetContent();
	lpCombo->AddString("  ");
	lpCombo->SetItemData(0,-1);

	for (i=0;i<cITEM::s_iPrefixCount;i++)	
	{
		if (iKind	!=	-1)
			if (g_aItemPrefix[i].m_wType	!=	iKind)
				continue;

		iIndex	=	lpCombo->AddString(g_aItemPrefix[i].m_str1stPrefix);
		lpCombo->SetItemData(iIndex,i);
	}

	lpCombo->SetCurSel(0);
}

void
cMakeItem::fillItemCombo() 
{
	int	iIndex,i;

	m_cbItemList.ResetContent();

	for (i=0;i<cITEM::s_iCount;i++)
	{
		if	(m_iItemKind	!=	-1)
			if	(g_aBasicItem[i].m_wKind	!=	m_iItemKind)
				continue;

		if	(i	==	0)
			continue;

		iIndex	=	m_cbItemList.AddString(g_aBasicItem[i].m_strName);
		m_cbItemList.SetItemData(iIndex,i);
	}

	m_cbItemList.SetCurSel(0);
}

void cMakeItem::OnSelchangePrefix1Kind() 
{
	int	iSelect		=	m_cbPrefix1Kind.GetCurSel();

	m_iPrefix1Kind	=	m_cbPrefix1Kind.GetItemData(iSelect);

	fillPrefixCombo(0);
	// TODO: Add your control notification handler code here
}

void cMakeItem::OnSelchangePrefix2Kind() 
{
	int	iSelect		=	m_cbPrefix2Kind.GetCurSel();

	m_iPrefix2Kind	=	m_cbPrefix2Kind.GetItemData(iSelect);

	fillPrefixCombo(1);
}

void cMakeItem::OnSelchangePrefix3Kind() 
{
	int	iSelect		=	m_cbPrefix3Kind.GetCurSel();

	m_iPrefix3Kind	=	m_cbPrefix3Kind.GetItemData(iSelect);

	fillPrefixCombo(2);
}

void cMakeItem::OnSelchangeItemKind() 
{
	int	iSelect		=	m_cbItemKind.GetCurSel();

	m_iItemKind		=	m_cbItemKind.GetItemData(iSelect);

	fillItemCombo();

	OnSelchangeItemList();
}

void cMakeItem::OnSelchangeItemList()
{
	// TODO: Add your control notification handler code here

	int	iSelect	=	m_cbItemList.GetCurSel();
	int	iItem	=	m_cbItemList.GetItemData(iSelect);

	cBASIC_ITEM	*lpBasicItem=	&g_aBasicItem[iItem];

	char	strOptionText[64];

	sprintf(strOptionText,"Option1Value [%d~%d]",lpBasicItem->m_aValue[0][0],lpBasicItem->m_aValue[0][1]);
	m_stOption1Value.SetWindowText(strOptionText);

	sprintf(strOptionText,"Option2Value [%d~%d]",lpBasicItem->m_aValue[1][0],lpBasicItem->m_aValue[1][1]);
	m_stOption2Value.SetWindowText(strOptionText);

	m_editOption1.SetWindowText(_ms("%d",lpBasicItem->m_aValue[0][1]));
	m_editOption2.SetWindowText(_ms("%d",lpBasicItem->m_aValue[1][1]));

	sprintf(strOptionText,"Count [1~%d]",lpBasicItem->m_wStackLimit);
	m_stCount.SetWindowText(strOptionText);
	m_editCount.SetWindowText(_ms("%d",lpBasicItem->m_wStackLimit));
}

void cMakeItem::OnSelchangePrefix1()
{
	int	iSelect					=	m_cbPrefix1.GetCurSel();
	int	iPrefix					=	m_cbPrefix1.GetItemData(iSelect);

	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

	char	strOptionText[64];

	sprintf(strOptionText,"Prefix1Value [%d~%d]",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]);
	m_stPrefix1Value.SetWindowText(strOptionText);

	sprintf(strOptionText,"[%d~%d]",lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]);
	m_stPrefix1Value2.SetWindowText(strOptionText);

	m_editPrefix1.SetWindowText(_ms("%d",lpPrefix->m_aValue[0][1]));
	m_editPrefix1_2.SetWindowText(_ms("%d",lpPrefix->m_aValue[1][1]));
}

void cMakeItem::OnSelchangePrefix2() 
{
	int	iSelect					=	m_cbPrefix2.GetCurSel();
	int	iPrefix					=	m_cbPrefix2.GetItemData(iSelect);

	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

	char	strOptionText[64];

	sprintf(strOptionText,"Prefix2Value [%d~%d]",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]);
	m_stPrefix2Value.SetWindowText(strOptionText);

	sprintf(strOptionText,"[%d~%d]",lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]);
	m_stPrefix2Value2.SetWindowText(strOptionText);

	m_editPrefix2.SetWindowText(_ms("%d",lpPrefix->m_aValue[0][1]));
	m_editPrefix2_2.SetWindowText(_ms("%d",lpPrefix->m_aValue[1][1]));
}

void cMakeItem::OnSelchangePrefix3() 
{
	// TODO: Add your control notification handler code here

	int	iSelect					=	m_cbPrefix3.GetCurSel();
	int	iPrefix					=	m_cbPrefix3.GetItemData(iSelect);

	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix];

	char	strOptionText[64];

	sprintf(strOptionText,"Prefix3Value [%d~%d]",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]);
	m_stPrefix3Value.SetWindowText(strOptionText);

	sprintf(strOptionText,"[%d~%d]",lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]);
	m_stPrefix3Value2.SetWindowText(strOptionText);

	m_editPrefix3.SetWindowText(_ms("%d",lpPrefix->m_aValue[0][1]));
	m_editPrefix3_2.SetWindowText(_ms("%d",lpPrefix->m_aValue[1][1]));
}

void
cMakeItem::getItemInfo(cItemInPack *_lpItem)
{
	// TODO: Add extra validation here
	int	iSelect;

	int		iOption1			=	GetDlgItemInt(IDC_OPTION1);
	int		iOption2			=	GetDlgItemInt(IDC_OPTION2);
	int		iPrefix11			=	0;
	int		iPrefix12			=	0;
	int		iPrefix21			=	0;
	int		iPrefix22			=	0;
	int		iPrefix31			=	0;
	int		iPrefix32			=	0;
	BOOL	bIsPrizeWinningItem	=	FALSE;
	BOOL	bIsBlockToTrade		=	FALSE;

	int	iCount			=	GetDlgItemInt(IDC_ITEM_COUNT);

	int	iLimitCount		=	GetDlgItemInt(IDC_LIMIT_COUNT);
	int	iFrequency		=	GetDlgItemInt(IDC_FREQUENCY);

	iSelect				=	m_cbItemList.GetCurSel();
	int	iBaseItem		=	m_cbItemList.GetItemData(iSelect);

	iSelect				=	m_cbPrefix1.GetCurSel();
	int	iPrefix1		=	m_cbPrefix1.GetItemData(iSelect);

	iSelect				=	m_cbPrefix2.GetCurSel();
	int	iPrefix2		=	m_cbPrefix2.GetItemData(iSelect);

	iSelect				=	m_cbPrefix3.GetCurSel();
	int	iPrefix3		=	m_cbPrefix3.GetItemData(iSelect);

	bIsPrizeWinningItem	=	IsDlgButtonChecked(IDC_PRIZE_WINNING_ITEM);
	bIsBlockToTrade		=	IsDlgButtonChecked(IDC_BLOCK_TO_TRADE);

	cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[iBaseItem];
	
	if (iOption1	<	lpBasicItem->m_aValue[0][0] || iOption1	>	lpBasicItem->m_aValue[0][1])
	{
		MessageBox("incorrect option 1 value","error");
		return;
	}

	if (iOption2	<	lpBasicItem->m_aValue[1][0] || iOption2	>	lpBasicItem->m_aValue[1][1])
	{
		MessageBox("incorrect option 2 value","error");
		return;
	}

	if (iCount	>	lpBasicItem->m_wStackLimit)
	{
		MessageBox("incorrect item count","error");
		return;
	}

	if (iPrefix1	!=	-1)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix1];

		iPrefix11		=	GetDlgItemInt(IDC_PREFIX_1);
		iPrefix12		=	GetDlgItemInt(IDC_PREFIX_1_2);

		if (iPrefix11	<	lpPrefix->m_aValue[0][0] || iPrefix11>	lpPrefix->m_aValue[0][1])
		{
			MessageBox("incorrect prefix 1 value 1","error");
			return;
		}
		if (iPrefix12	<	lpPrefix->m_aValue[1][0] || iPrefix12>	lpPrefix->m_aValue[1][1])
		{
			MessageBox("incorrect prefix 1 value 2","error");
			return;
		}
	}

	if (iPrefix2	!=	-1)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix2];

		iPrefix21		=	GetDlgItemInt(IDC_PREFIX_2);
		iPrefix22		=	GetDlgItemInt(IDC_PREFIX_2_2);

		if (iPrefix21	<	lpPrefix->m_aValue[0][0] || iPrefix21>	lpPrefix->m_aValue[0][1])
		{
			MessageBox("incorrect prefix 2 value 1","error");
			return;
		}
		if (iPrefix22	<	lpPrefix->m_aValue[1][0] || iPrefix22>	lpPrefix->m_aValue[1][1])
		{
			MessageBox("incorrect prefix 2 value 2","error");
			return;
		}
	}

	if (iPrefix3	!=	-1)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[iPrefix3];

		iPrefix31		=	GetDlgItemInt(IDC_PREFIX_3);
		iPrefix32		=	GetDlgItemInt(IDC_PREFIX_3_2);

		if (iPrefix31	<	lpPrefix->m_aValue[0][0] || iPrefix31>	lpPrefix->m_aValue[0][1])
		{
			MessageBox("incorrect prefix 31 value 1","error");
			return;
		}
		if (iPrefix32	<	lpPrefix->m_aValue[1][0] || iPrefix32>	lpPrefix->m_aValue[1][1])
		{
			MessageBox("incorrect prefix 3 value 2","error");
			return;
		}
	}

	_lpItem->reset();

	_lpItem->m_wBaseItem				=	iBaseItem;
	_lpItem->m_bCount					=	iCount;
	_lpItem->m_bDurability				=	(BYTE)lpBasicItem->m_wDurability;
	_lpItem->m_aOption[0]				=	iOption1;
	_lpItem->m_aOption[1]				=	iOption2;

	_lpItem->m_aPrefix[0].m_wPrefix		=	iPrefix1;
	_lpItem->m_aPrefix[0].m_aValue[0]	=	iPrefix11;
	_lpItem->m_aPrefix[0].m_aValue[1]	=	iPrefix12;

	_lpItem->m_aPrefix[1].m_wPrefix		=	iPrefix2;
	_lpItem->m_aPrefix[1].m_aValue[0]	=	iPrefix21;
	_lpItem->m_aPrefix[1].m_aValue[1]	=	iPrefix22;

	_lpItem->m_aPrefix[2].m_wPrefix		=	iPrefix3;
	_lpItem->m_aPrefix[2].m_aValue[0]	=	iPrefix31;
	_lpItem->m_aPrefix[2].m_aValue[1]	=	iPrefix32;

	_lpItem->m_dwFrequency				=	iFrequency;
	_lpItem->m_wLimitCountOfServer		=	iLimitCount;
	_lpItem->m_bf1IsPrizeWinningItem	=	bIsPrizeWinningItem;
	_lpItem->m_bf1IsBlockToTrade		=	bIsBlockToTrade;

}

void
cMakeItem::OnOK() 
{
	getItemInfo(&m_makeItem);

	int	iIncorrectPrefixCount			=	((cITEM*)(&m_makeItem))->getIncorrectPrefixCount();

	if ( iIncorrectPrefixCount	>	0)
	{
		if (cMSG::YESNO("Warning make Item!!",g_strToolMessage[147],iIncorrectPrefixCount) == IDNO)
			return;
	}

	if ( iIncorrectPrefixCount	<	0)
		return;
	
	for (int i=0;i<dITEM_PREFIX_COUNT;i++)
	{
		if (m_makeItem.m_aPrefix[i].m_wPrefix	!=	0xffff)
			continue;

		for (int j=i+1;j<dITEM_PREFIX_COUNT;j++)
		{
			if (m_makeItem.m_aPrefix[j].m_wPrefix	!=	0xffff)
			{
				memcpy(&m_makeItem.m_aPrefix[i],&m_makeItem.m_aPrefix[j],sizeof(m_makeItem.m_aPrefix[i]));
				memset(&m_makeItem.m_aPrefix[j],0xff,sizeof(m_makeItem.m_aPrefix[j]));

				break;
			}
		}
	}

	CDialog::OnOK();
}

void cMakeItem::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void cMakeItem::OnApply() 
{
	// TODO: Add your control notification handler code here
	char strItemInfo[512];

	m_editItemCode.GetWindowText(strItemInfo,sizeof(strItemInfo)-2);

	if (strlen(strItemInfo) < 5)
	{
		cItemInPack	item;

		getItemInfo(&item);

		sprintf(strItemInfo,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
							item.m_wBaseItem,item.m_dwSerial,item.m_bCount,item.m_bDurability,
							item.m_aOption[0],item.m_aOption[1],
							item.m_aPrefix[0].m_wPrefix,item.m_aPrefix[0].m_aValue[0],item.m_aPrefix[0].m_aValue[1],
							item.m_aPrefix[1].m_wPrefix,item.m_aPrefix[1].m_aValue[0],item.m_aPrefix[1].m_aValue[1],
							item.m_aPrefix[2].m_wPrefix,item.m_aPrefix[2].m_aValue[0],item.m_aPrefix[2].m_aValue[1],
							item.getBitFieldValue()	);

		m_editItemCode.SetWindowText(strItemInfo);

		return;
	}

	cDECODER	decoder;

	if (!decoder.UploadBuffer(strItemInfo,NULL))
		return;

	int			iProcessIndex	=	0,iCount=0;
	cITEM		item;

	item.reset();

	for (iProcessIndex =0;iProcessIndex<22;iProcessIndex++)
	{
		BOOL	bIsSuccess;
		DWORD	dwValue		=	decoder.GetNumber(&bIsSuccess);

		if (!bIsSuccess)
			break;

		if (iProcessIndex	==	0)	item.m_wBaseItem				=	(WORD)dwValue;
		if (iProcessIndex	==	1)
		{
			item.m_dwSerial	=	(DWORD)dwValue;
			if (item.m_dwSerial	<=	1)
				item.m_dwSerial	=	largeRandom(0x7fffffff);
		}
		if (iProcessIndex	==	2)	item.m_bCount					=	(BYTE)dwValue;
		if (iProcessIndex	==	3)	item.m_bDurability				=	(BYTE)dwValue;

		if (iProcessIndex	==	4)	item.m_aOption[0]				=	(BYTE)dwValue;
		if (iProcessIndex	==	5)	item.m_aOption[1]				=	(BYTE)dwValue;

		if (iProcessIndex	==	6)	item.m_aPrefix[0].m_wPrefix		=	(WORD)dwValue;
		if (iProcessIndex	==	7)	item.m_aPrefix[0].m_aValue[0]	=	(BYTE)dwValue;
		if (iProcessIndex	==	8)	item.m_aPrefix[0].m_aValue[1]	=	(BYTE)dwValue;

		if (iProcessIndex	==	9)	item.m_aPrefix[1].m_wPrefix		=	(WORD)dwValue;
		if (iProcessIndex	==	10)	item.m_aPrefix[1].m_aValue[0]	=	(BYTE)dwValue;
		if (iProcessIndex	==	11)	item.m_aPrefix[1].m_aValue[1]	=	(BYTE)dwValue;

		if	(iProcessIndex	==	12)	item.m_aPrefix[2].m_wPrefix		=	(WORD)dwValue;
		if	(iProcessIndex	==	13)	item.m_aPrefix[2].m_aValue[0]	=	(BYTE)dwValue;
		if	(iProcessIndex	==	14)	item.m_aPrefix[2].m_aValue[1]	=	(BYTE)dwValue;

		if	(iProcessIndex	==	15)
			item.setBitFieldValue((DWORD)dwValue);
	}

	memcpy(&m_makeItem,&item,sizeof(item));

	fillItemData();
}

void cMakeItem::fillItemData()
{
	if (m_makeItem.m_wBaseItem	==	0xffff)
		return;

	// TODO: Add your control notification handler code here
	cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[m_makeItem.m_wBaseItem];

	m_iItemKind	=	lpBasicItem->m_wKind;
	m_cbItemKind.SetCurSel(lpBasicItem->m_wKind+1);

	fillItemCombo();
	m_cbItemList.SelectString(0,lpBasicItem->m_strName);

	if	(m_makeItem.m_bf1IsPrizeWinningItem)
		CheckDlgButton(IDC_PRIZE_WINNING_ITEM,TRUE);
	else
		CheckDlgButton(IDC_PRIZE_WINNING_ITEM,FALSE);

	if	(m_makeItem.m_bf1IsBlockToTrade)
		CheckDlgButton(IDC_BLOCK_TO_TRADE,TRUE);
	else
		CheckDlgButton(IDC_BLOCK_TO_TRADE,FALSE);

	if	(m_makeItem.m_aPrefix[0].m_wPrefix	!=	0xfffF)
	{
		m_iPrefix1Kind	=	g_aItemPrefix[m_makeItem.m_aPrefix[0].m_wPrefix].m_wType;
		m_cbPrefix1Kind.SetCurSel(m_iPrefix1Kind+1);
		fillPrefixCombo(0);
		m_cbPrefix1.SelectString(0,g_aItemPrefix[m_makeItem.m_aPrefix[0].m_wPrefix].m_str1stPrefix);
	}
	else
		m_cbPrefix1.SetCurSel(0);
	
	if (m_makeItem.m_aPrefix[1].m_wPrefix	!=	0xfffF)
	{
		m_iPrefix2Kind	=	g_aItemPrefix[m_makeItem.m_aPrefix[1].m_wPrefix].m_wType;
		m_cbPrefix2Kind.SetCurSel(m_iPrefix2Kind+1);
		fillPrefixCombo(1);
		m_cbPrefix2.SelectString(0,g_aItemPrefix[m_makeItem.m_aPrefix[1].m_wPrefix].m_str1stPrefix);
	}
	else
		m_cbPrefix2.SetCurSel(0);

	if (m_makeItem.m_aPrefix[2].m_wPrefix	!=	0xfffF)
	{
		m_iPrefix3Kind	=	g_aItemPrefix[m_makeItem.m_aPrefix[2].m_wPrefix].m_wType;
		m_cbPrefix3Kind.SetCurSel(m_iPrefix3Kind+1);
		m_cbPrefix3.SelectString(0,g_aItemPrefix[m_makeItem.m_aPrefix[2].m_wPrefix].m_str1stPrefix);
		fillPrefixCombo(2);
	}
	else
		m_cbPrefix3.SetCurSel(0);

	OnSelchangeItemList();
	OnSelchangePrefix1();
	OnSelchangePrefix2();
	OnSelchangePrefix3();

	m_editCount.SetWindowText(_ms("%d",m_makeItem.m_bCount));
	m_editOption1.SetWindowText(_ms("%d",m_makeItem.m_aOption[0]));
	m_editOption2.SetWindowText(_ms("%d",m_makeItem.m_aOption[1]));

	if (m_makeItem.m_aPrefix[0].m_wPrefix	!=	0xffff)
	{
		m_editPrefix1.SetWindowText(_ms("%d",m_makeItem.m_aPrefix[0].m_aValue[0]));
		m_editPrefix1_2.SetWindowText(_ms("%d",m_makeItem.m_aPrefix[0].m_aValue[1]));
	}

	if (m_makeItem.m_aPrefix[1].m_wPrefix	!=	0xffff)
	{
		m_editPrefix2.SetWindowText(_ms("%d",m_makeItem.m_aPrefix[1].m_aValue[0]));
		m_editPrefix2_2.SetWindowText(_ms("%d",m_makeItem.m_aPrefix[1].m_aValue[1]));
	}

	if (m_makeItem.m_aPrefix[2].m_wPrefix	!=	0xffff)
	{
		m_editPrefix3.SetWindowText(_ms("%d",m_makeItem.m_aPrefix[2].m_aValue[0]));
		m_editPrefix3_2.SetWindowText(_ms("%d",m_makeItem.m_aPrefix[2].m_aValue[1]));
	}

	m_editLimitCount.SetWindowText(_ms("%d",m_makeItem.m_wLimitCountOfServer));
	m_editFrequency.SetWindowText(_ms("%d",m_makeItem.m_dwFrequency));
}

void cMakeItem::OnSetCategory() 
{
	int	iSelect,iItem;
	int	iPrefix1Kind=-1,iPrefix2Kind=-1,iPrefix3Kind=-1,iItemKind=-1;

	{
		iSelect	=	m_cbItemList.GetCurSel();
		iItem	=	m_cbItemList.GetItemData(iSelect);

		if	(iItem	>	0)
			iItemKind	=	g_aBasicItem[iItem].m_wKind;
	}

	{
		iSelect	=	m_cbPrefix1.GetCurSel();
		iItem	=	m_cbPrefix1.GetItemData(iSelect);

		if	(iItem	>=	0)
			iPrefix1Kind	=	g_aItemPrefix[iItem].m_wType;
	}

	{
		iSelect	=	m_cbPrefix2.GetCurSel();
		iItem	=	m_cbPrefix2.GetItemData(iSelect);

		if	(iItem	>=	0)
			iPrefix2Kind	=	g_aItemPrefix[iItem].m_wType;
	}

	{
		iSelect	=	m_cbPrefix3.GetCurSel();
		iItem	=	m_cbPrefix3.GetItemData(iSelect);

		if	(iItem	>=	0)
			iPrefix3Kind	=	g_aItemPrefix[iItem].m_wType;
	}

	if	(iItemKind		>=	0)
		m_cbItemKind.SetCurSel(iItemKind+1);

	if	(iPrefix1Kind	>=	0)
		m_cbPrefix1Kind.SetCurSel(iPrefix1Kind+1);

	if	(iPrefix2Kind	>=	0)
		m_cbPrefix2Kind.SetCurSel(iPrefix2Kind+1);

	if	(iPrefix3Kind	>=	0)
		m_cbPrefix3Kind.SetCurSel(iPrefix3Kind+1);
}
