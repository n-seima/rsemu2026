#include "setMakeItem.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "CItem_Effect.h"

cITEM					cSetMakeItem::s_itemEdit;	//	편집중인 상점

HWND					cSetMakeItem::s_hWnd;
int						cSetMakeItem::s_iPrefix1Kind;
int						cSetMakeItem::s_iPrefix2Kind;
int						cSetMakeItem::s_iPrefix3Kind;
int						cSetMakeItem::s_iItemKind;

cITEM*
cSetMakeItem::Open(HWND _hWnd,cITEM *_lpEditItem)
{
	s_itemEdit.reset();

	if (_lpEditItem)
		memcpy(&s_itemEdit,_lpEditItem,sizeof(cITEM));

	if (DialogBox( s_hInst, "MAKE_ITEM",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	NULL;

	return	&s_itemEdit;
}
//
//
void
cSetMakeItem::InitBox(HWND _hDlg)
{
	s_hWnd	=	_hDlg;

	int		iIndex,i;
	HWND	hCombo;

	for (int iPrefix=0;iPrefix<3;iPrefix++)
	{
		if (iPrefix	==	0)
			hCombo	=	GetDlgItem(_hDlg,IDC_PREFIX1_KIND);
		if (iPrefix	==	1)
			hCombo	=	GetDlgItem(_hDlg,IDC_PREFIX2_KIND);
		if (iPrefix	==	2)
			hCombo	=	GetDlgItem(_hDlg,IDC_PREFIX3_KIND);

		cCOMBOBOX::Add(hCombo,"ALL",-1);

		for (i=0;;i++)
		{
			if (g_aItemPrefixType[i].m_wType	==	0xffff)
				break;

			iIndex	=	cCOMBOBOX::Add(hCombo,g_aItemPrefixType[i].m_strName,g_aItemPrefixType[i].m_wType);
		}

		cCOMBOBOX::SelectByData(hCombo,0);
	}

	{
		hCombo	=	GetDlgItem(_hDlg,IDC_ITEM_KIND);
		cCOMBOBOX::Add(hCombo,"ALL",-1);

		for (i=0;;i++)
		{
			if (stricmp(g_strItemCategory[i],"end")	==	0)
				break;

			cCOMBOBOX::Add(hCombo,g_strItemCategory[i],i);
		}

		cCOMBOBOX::SelectByData(hCombo,0);
	}

	s_iPrefix1Kind	=	0;
	s_iPrefix2Kind	=	0;
	s_iPrefix3Kind	=	0;
	s_iItemKind		=	0;

	FillPrefixCombo(_hDlg,0);
	FillPrefixCombo(_hDlg,1);
	FillPrefixCombo(_hDlg,2);
	FillItemCombo(_hDlg);

	hCombo			=	GetDlgItem(_hDlg,IDC_ITEM_LIST);
	int		iItem	=	cCOMBOBOX::GetData(hCombo);

	SelchangeItem(_hDlg,iItem);

/*
	OnSelchangeItemList();
	// TODO: Add extra initialization here
*/	
	FillItemData(_hDlg);
}

BOOL
cSetMakeItem::GetItemInfo(HWND _hDlg,cITEM *_lpItem)
{
	// TODO: Add extra validation here
	BOOL	bTemp;
	int		iOption1		=	GetDlgItemInt(_hDlg,IDC_OPTION1,&bTemp,FALSE);
	int		iOption2		=	GetDlgItemInt(_hDlg,IDC_OPTION2,&bTemp,FALSE);
	int		iPrefix11		=	0;
	int		iPrefix12		=	0;
	int		iPrefix21		=	0;
	int		iPrefix22		=	0;
	int		iPrefix31		=	0;
	int		iPrefix32		=	0;

	int		iCount			=	GetDlgItemInt(_hDlg,IDC_ITEM_COUNT,&bTemp,FALSE);
	int		iBaseItem		=	cCOMBOBOX::GetData(GetDlgItem(_hDlg,IDC_ITEM_LIST));
	int		iPrefix1		=	cCOMBOBOX::GetData(GetDlgItem(_hDlg,IDC_PREFIX1));
	int		iPrefix2		=	cCOMBOBOX::GetData(GetDlgItem(_hDlg,IDC_PREFIX2));
	int		iPrefix3		=	cCOMBOBOX::GetData(GetDlgItem(_hDlg,IDC_PREFIX3));

	cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[iBaseItem];
	
	if (iOption1	<	lpBasicItem->m_aValue[0][0] || iOption1	>	lpBasicItem->m_aValue[0][1])
	{
		cMSG::Put("incorrect option 1 value","error");
		return FALSE;
	}

	if (iOption2	<	lpBasicItem->m_aValue[1][0] || iOption2	>	lpBasicItem->m_aValue[1][1])
	{
		cMSG::Put("incorrect option 2 value","error");
		return FALSE;
	}

	if (iCount	>	lpBasicItem->m_wStackLimit)
	{
		cMSG::Put("incorrect item count","error");
		return FALSE;
	}

	if (iPrefix1	!=	-1)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iPrefix1);

		iPrefix11		=	GetDlgItemInt(_hDlg,IDC_PREFIX_1,&bTemp,FALSE);
		iPrefix12		=	GetDlgItemInt(_hDlg,IDC_PREFIX_1_2,&bTemp,FALSE);

		if (iPrefix11	<	lpPrefix->m_aValue[0][0] || iPrefix11>	lpPrefix->m_aValue[0][1])
		{
			cMSG::Put("incorrect prefix 1 value 1","error");
			return FALSE;
		}
		if (iPrefix12	<	lpPrefix->m_aValue[1][0] || iPrefix12>	lpPrefix->m_aValue[1][1])
		{
			cMSG::Put("incorrect prefix 1 value 2","error");
			return FALSE;
		}
	}

	if (iPrefix2	!=	-1)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iPrefix2);

		iPrefix21		=	GetDlgItemInt(_hDlg,IDC_PREFIX_2,&bTemp,FALSE);
		iPrefix22		=	GetDlgItemInt(_hDlg,IDC_PREFIX_2_2,&bTemp,FALSE);

		if (iPrefix21	<	lpPrefix->m_aValue[0][0] || iPrefix21>	lpPrefix->m_aValue[0][1])
		{
			cMSG::Put("incorrect prefix 2 value 1","error");
			return FALSE;
		}
		if (iPrefix22	<	lpPrefix->m_aValue[1][0] || iPrefix22>	lpPrefix->m_aValue[1][1])
		{
			cMSG::Put("incorrect prefix 2 value 2","error");
			return FALSE;
		}
	}

	if (iPrefix3	!=	-1)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iPrefix3);

		iPrefix31		=	GetDlgItemInt(_hDlg,IDC_PREFIX_3,&bTemp,FALSE);
		iPrefix32		=	GetDlgItemInt(_hDlg,IDC_PREFIX_3_2,&bTemp,FALSE);

		if (iPrefix31	<	lpPrefix->m_aValue[0][0] || iPrefix31>	lpPrefix->m_aValue[0][1])
		{
			cMSG::Put("incorrect prefix 31 value 1","error");
			return FALSE;
		}
		if (iPrefix32	<	lpPrefix->m_aValue[1][0] || iPrefix32>	lpPrefix->m_aValue[1][1])
		{
			cMSG::Put("incorrect prefix 3 value 2","error");
			return FALSE;
		}
	}

	_lpItem->reset();

	_lpItem->m_wBaseItem				=	iBaseItem;
	_lpItem->m_bCount					=	iCount;
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

	for (int j=0;j<dITEM_PREFIX_COUNT;j++)
	{
		for (int i=0;i<dITEM_PREFIX_COUNT-1;i++)
			if (_lpItem->m_aPrefix[i].m_wPrefix	==	0xffff)
			{
				memcpy(&_lpItem->m_aPrefix[i],&_lpItem->m_aPrefix[i+1],sizeof(CItemPrefixInfo));

				_lpItem->m_aPrefix[i+1].m_wPrefix	=	0xffff;
				_lpItem->m_aPrefix[i+1].m_aValue[0]	=	0;
				_lpItem->m_aPrefix[i+1].m_aValue[1]	=	0;
			}
	}

	return	TRUE;
}

void
cSetMakeItem::FillPrefixCombo(HWND _hDlg,int _iIndex)
{
	int		iIndex,i;

	HWND	hCombo;
	int		iKind;

	if (_iIndex	==	0)
	{
		hCombo	=	GetDlgItem(_hDlg,IDC_PREFIX1);
		iKind	=	s_iPrefix1Kind;
	}
	if (_iIndex	==	1)
	{
		hCombo	=	GetDlgItem(_hDlg,IDC_PREFIX2);
		iKind	=	s_iPrefix2Kind;
	}
	if (_iIndex	==	2)
	{
		hCombo	=	GetDlgItem(_hDlg,IDC_PREFIX3);
		iKind	=	s_iPrefix3Kind;
	}

	cCOMBOBOX::Reset(hCombo);

	cCOMBOBOX::Add(hCombo," ",-1);

	for (i=0;i<g_itemPrefix.getCount();i++)
	{
		cITEM_PREFIX	*lpPrefix=	g_itemPrefix.get(i);

		if (iKind	!=	-1)
			if (lpPrefix->m_wType	!=	iKind)
				continue;

		if (lpPrefix->m_wIsDXPrefix)
			iIndex	=	cCOMBOBOX::Add(hCombo,_ms("[DX] %s",lpPrefix->m_str1stPrefix),i);
		else
			iIndex	=	cCOMBOBOX::Add(hCombo,lpPrefix->m_str1stPrefix,i);
	}

	cCOMBOBOX::Select(hCombo,0);
	
	SelchangePrefix(_hDlg,-1,_iIndex);
}

void
cSetMakeItem::FillItemCombo(HWND _hDlg) 
{
	int		i;
	
	HWND	hCombo	=	GetDlgItem(_hDlg,IDC_ITEM_LIST);

	cCOMBOBOX::Reset(hCombo);

	for (i=0;i<cBASIC_ITEM::s_iCount;i++)
	{
		if (s_iItemKind	!=	-1)
			if (g_aBasicItem[i].m_wKind	!=	s_iItemKind)
				continue;

		if (s_iItemKind	!=	-1 && i	==	0)
			continue;

		char	strName[128];

		if (g_aBasicItem[i].isDXUnique())
			sprintf(strName,"[DU] %s",g_aBasicItem[i].m_strName);
		else
		if (g_aBasicItem[i].isUnique())
			sprintf(strName,"[U] %s",g_aBasicItem[i].m_strName);
		else
		if (g_aBasicItem[i].isDX())
			sprintf(strName,"[DX] %s",g_aBasicItem[i].m_strName);
		else
		if (g_aBasicItem[i].isExtraItem())
			sprintf(strName,"[EX] %s",g_aBasicItem[i].m_strName);
		else
			strcpy(strName,g_aBasicItem[i].m_strName);
			
		cCOMBOBOX::Add(hCombo,strName,i);
	}

	cCOMBOBOX::Select(hCombo,0);
}

void 
cSetMakeItem::FillItemData(HWND _hDlg)
{
	if (s_itemEdit.m_wBaseItem	==	0xffff)
		return;

	// TODO: Add your control notification handler code here
	cBASIC_ITEM	*lpBasicItem	=	&g_aBasicItem[s_itemEdit.m_wBaseItem];

	s_iItemKind	=	lpBasicItem->m_wKind;

	HWND	comboItem,comboPrefix1,comboPrefix2,comboPrefix3,comboItemKind;
	HWND	comboPrefix1Kind,comboPrefix2Kind,comboPrefix3Kind;

	comboItemKind	=	GetDlgItem(_hDlg,IDC_ITEM_KIND);
	comboItem		=	GetDlgItem(_hDlg,IDC_ITEM_LIST);
	comboPrefix1	=	GetDlgItem(_hDlg,IDC_PREFIX1);
	comboPrefix2	=	GetDlgItem(_hDlg,IDC_PREFIX2);
	comboPrefix3	=	GetDlgItem(_hDlg,IDC_PREFIX3);
	comboPrefix1Kind=	GetDlgItem(_hDlg,IDC_PREFIX1_KIND);
	comboPrefix2Kind=	GetDlgItem(_hDlg,IDC_PREFIX2_KIND);
	comboPrefix3Kind=	GetDlgItem(_hDlg,IDC_PREFIX3_KIND);

	cCOMBOBOX::SelectByData(comboItemKind,lpBasicItem->m_wKind);
	FillItemCombo(_hDlg);

	cCOMBOBOX::SelectByData(comboItem,lpBasicItem->m_iSerial);

	if (s_itemEdit.m_aPrefix[0].m_wPrefix	!=	0xfffF)
	{
		s_iPrefix1Kind	=	g_itemPrefix.get(s_itemEdit.m_aPrefix[0].m_wPrefix)->m_wType;
		cCOMBOBOX::SelectByData(comboPrefix1Kind,s_iPrefix1Kind);
		FillPrefixCombo(_hDlg,0);
		cCOMBOBOX::Select(comboPrefix1,g_itemPrefix.get(s_itemEdit.m_aPrefix[0].m_wPrefix)->m_str1stPrefix);
	}
	else
		cCOMBOBOX::Select(comboPrefix1,0);

	if (s_itemEdit.m_aPrefix[1].m_wPrefix	!=	0xfffF)
	{
		s_iPrefix2Kind	=	g_itemPrefix.get(s_itemEdit.m_aPrefix[1].m_wPrefix)->m_wType;
		cCOMBOBOX::SelectByData(comboPrefix2Kind,s_iPrefix2Kind);
		FillPrefixCombo(_hDlg,1);
		cCOMBOBOX::Select(comboPrefix2,g_itemPrefix.get(s_itemEdit.m_aPrefix[1].m_wPrefix)->m_str1stPrefix);
	}
	else
		cCOMBOBOX::Select(comboPrefix2,0);
	
	if (s_itemEdit.m_aPrefix[2].m_wPrefix	!=	0xfffF)
	{
		s_iPrefix3Kind	=	g_itemPrefix.get(s_itemEdit.m_aPrefix[2].m_wPrefix)->m_wType;
		cCOMBOBOX::SelectByData(comboPrefix3Kind,s_iPrefix3Kind);
		FillPrefixCombo(_hDlg,1);
		cCOMBOBOX::Select(comboPrefix3,g_itemPrefix.get(s_itemEdit.m_aPrefix[2].m_wPrefix)->m_str1stPrefix);
	}
	else
		cCOMBOBOX::Select(comboPrefix3,0);

	SelchangeItem(_hDlg,s_itemEdit.m_wBaseItem);
	SelchangePrefix(_hDlg,s_itemEdit.m_aPrefix[0].m_wPrefix,0);
	SelchangePrefix(_hDlg,s_itemEdit.m_aPrefix[1].m_wPrefix,1);
	SelchangePrefix(_hDlg,s_itemEdit.m_aPrefix[2].m_wPrefix,2);

	SetDlgItemText(_hDlg,IDC_ITEM_COUNT,_ms("%d",s_itemEdit.m_bCount));
	
	s_itemEdit.m_aOption[0]	=	(BYTE)max(lpBasicItem->m_aValue[0][0],s_itemEdit.m_aOption[0]);
	s_itemEdit.m_aOption[0]	=	(BYTE)min(lpBasicItem->m_aValue[0][1],s_itemEdit.m_aOption[0]);
	s_itemEdit.m_aOption[1]	=	(BYTE)max(lpBasicItem->m_aValue[1][0],s_itemEdit.m_aOption[1]);
	s_itemEdit.m_aOption[1]	=	(BYTE)min(lpBasicItem->m_aValue[1][1],s_itemEdit.m_aOption[1]);

	SetDlgItemText(_hDlg,IDC_OPTION1,_ms("%d",s_itemEdit.m_aOption[0]));
	SetDlgItemText(_hDlg,IDC_OPTION2,_ms("%d",s_itemEdit.m_aOption[1]));

	if (s_itemEdit.m_aPrefix[0].m_wPrefix	!=	0xffff)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_itemEdit.m_aPrefix[0].m_wPrefix);

		s_itemEdit.m_aPrefix[0].m_aValue[0]	=	(BYTE)max(lpPrefix->m_aValue[0][0],s_itemEdit.m_aPrefix[0].m_aValue[0]);
		s_itemEdit.m_aPrefix[0].m_aValue[0]	=	(BYTE)min(lpPrefix->m_aValue[0][1],s_itemEdit.m_aPrefix[0].m_aValue[0]);
		s_itemEdit.m_aPrefix[0].m_aValue[1]	=	(BYTE)max(lpPrefix->m_aValue[1][0],s_itemEdit.m_aPrefix[0].m_aValue[1]);
		s_itemEdit.m_aPrefix[0].m_aValue[1]	=	(BYTE)min(lpPrefix->m_aValue[1][1],s_itemEdit.m_aPrefix[0].m_aValue[1]);

		SetDlgItemText(_hDlg,IDC_PREFIX_1,_ms("%d",s_itemEdit.m_aPrefix[0].m_aValue[0]));
		SetDlgItemText(_hDlg,IDC_PREFIX_1_2,_ms("%d",s_itemEdit.m_aPrefix[0].m_aValue[1]));
	}
	else
	{
		SetDlgItemText(_hDlg,IDC_PREFIX_1,"0");
		SetDlgItemText(_hDlg,IDC_PREFIX_1_2,"0");
	}

	if (s_itemEdit.m_aPrefix[1].m_wPrefix	!=	0xffff)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_itemEdit.m_aPrefix[1].m_wPrefix);

		s_itemEdit.m_aPrefix[1].m_aValue[0]	=	(BYTE)max(lpPrefix->m_aValue[0][0],s_itemEdit.m_aPrefix[1].m_aValue[0]);
		s_itemEdit.m_aPrefix[1].m_aValue[0]	=	(BYTE)min(lpPrefix->m_aValue[0][1],s_itemEdit.m_aPrefix[1].m_aValue[0]);
		s_itemEdit.m_aPrefix[1].m_aValue[1]	=	(BYTE)max(lpPrefix->m_aValue[1][0],s_itemEdit.m_aPrefix[1].m_aValue[1]);
		s_itemEdit.m_aPrefix[1].m_aValue[1]	=	(BYTE)min(lpPrefix->m_aValue[1][1],s_itemEdit.m_aPrefix[1].m_aValue[1]);

		SetDlgItemText(_hDlg,IDC_PREFIX_2,_ms("%d",s_itemEdit.m_aPrefix[1].m_aValue[0]));
		SetDlgItemText(_hDlg,IDC_PREFIX_2_2,_ms("%d",s_itemEdit.m_aPrefix[1].m_aValue[1]));
	}
	else
	{
		SetDlgItemText(_hDlg,IDC_PREFIX_2,"0");
		SetDlgItemText(_hDlg,IDC_PREFIX_2_2,"0");
	}

	if (s_itemEdit.m_aPrefix[2].m_wPrefix	!=	0xffff)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_itemEdit.m_aPrefix[2].m_wPrefix);

		s_itemEdit.m_aPrefix[2].m_aValue[0]	=	(BYTE)max(lpPrefix->m_aValue[0][0],s_itemEdit.m_aPrefix[2].m_aValue[0]);
		s_itemEdit.m_aPrefix[2].m_aValue[0]	=	(BYTE)min(lpPrefix->m_aValue[0][1],s_itemEdit.m_aPrefix[2].m_aValue[0]);
		s_itemEdit.m_aPrefix[2].m_aValue[1]	=	(BYTE)max(lpPrefix->m_aValue[1][0],s_itemEdit.m_aPrefix[2].m_aValue[1]);
		s_itemEdit.m_aPrefix[2].m_aValue[1]	=	(BYTE)min(lpPrefix->m_aValue[1][1],s_itemEdit.m_aPrefix[2].m_aValue[1]);

		SetDlgItemText(_hDlg,IDC_PREFIX_3,_ms("%d",s_itemEdit.m_aPrefix[2].m_aValue[0]));
		SetDlgItemText(_hDlg,IDC_PREFIX_3_2,_ms("%d",s_itemEdit.m_aPrefix[2].m_aValue[1]));
	}
	else
	{
		SetDlgItemText(_hDlg,IDC_PREFIX_3,"0");
		SetDlgItemText(_hDlg,IDC_PREFIX_3_2,"0");
	}
}

void 
cSetMakeItem::SelchangeItem(HWND _hDlg,int _iItem)
{
	cBASIC_ITEM	*lpBasicItem=	&g_aBasicItem[_iItem];

	char	strOptionText[64];

	sprintf(strOptionText,"Option1Value [%d~%d]",lpBasicItem->m_aValue[0][0],lpBasicItem->m_aValue[0][1]);
	SetDlgItemText(_hDlg,IDC_OPTION_1_VALUE,strOptionText);

	sprintf(strOptionText,"Option2Value [%d~%d]",lpBasicItem->m_aValue[1][0],lpBasicItem->m_aValue[1][1]);
	SetDlgItemText(_hDlg,IDC_OPTION_2_VALUE,strOptionText);

	SetDlgItemInt(_hDlg,IDC_OPTION1,lpBasicItem->m_aValue[0][1],FALSE);
	SetDlgItemInt(_hDlg,IDC_OPTION2,lpBasicItem->m_aValue[1][1],FALSE);
	
	sprintf(strOptionText,"Count [1~%d]",lpBasicItem->m_wStackLimit);
	SetDlgItemText(_hDlg,IDC_ITEM_COUNT_TEXT,strOptionText);
	SetDlgItemInt(_hDlg,IDC_ITEM_COUNT,lpBasicItem->m_wStackLimit,FALSE);
}

void 
cSetMakeItem::SelchangePrefix(HWND _hDlg,int _iPrefix,int _iPrefixIndex)
{
	char	strOptionText[64];
	int		iTextID1,iTextID2;
	int		iEditID1,iEditID2;

	if (_iPrefixIndex	==	0)
	{
		iTextID1	=	IDC_PREFIX_1_VALUE;
		iTextID2	=	IDC_PREFIX_1_VALUE2;
		iEditID1	=	IDC_PREFIX_1;
		iEditID2	=	IDC_PREFIX_1_2;
	}
	if (_iPrefixIndex	==	1)
	{
		iTextID1	=	IDC_PREFIX_2_VALUE;
		iTextID2	=	IDC_PREFIX_2_VALUE2;
		iEditID1	=	IDC_PREFIX_2;
		iEditID2	=	IDC_PREFIX_2_2;
	}
	if (_iPrefixIndex	==	2)
	{
		iTextID1	=	IDC_PREFIX_3_VALUE;
		iTextID2	=	IDC_PREFIX_3_VALUE2;
		iEditID1	=	IDC_PREFIX_3;
		iEditID2	=	IDC_PREFIX_3_2;
	}

	if (_iPrefix	==	-1	||	_iPrefix	==	0xffff)
	{
		sprintf(strOptionText,"Prefix%dValue 1",_iPrefixIndex+1);
		SetDlgItemText(_hDlg,iTextID1,strOptionText);

		strcpy(strOptionText,"value 2");
		SetDlgItemText(_hDlg,iTextID2,strOptionText);

		SetDlgItemInt(_hDlg,iEditID1,0,FALSE);
		SetDlgItemInt(_hDlg,iEditID2,0,FALSE);
		return;
	}

	cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(_iPrefix);

	sprintf(strOptionText,"Prefix%dValue [%d~%d]",_iPrefixIndex+1,lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]);
	SetDlgItemText(_hDlg,iTextID1,strOptionText);

	sprintf(strOptionText,"[%d~%d]",lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]);
	SetDlgItemText(_hDlg,iTextID2,strOptionText);

	SetDlgItemInt(_hDlg,iEditID1,lpPrefix->m_aValue[0][1],FALSE);
	SetDlgItemInt(_hDlg,iEditID2,lpPrefix->m_aValue[1][1],FALSE);
}


//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
cSetMakeItem::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	case WM_INITDIALOG	:
			InitBox(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;

		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case	IDOK					:
				{
					cITEM	item;

					if (GetItemInfo(hDlg,&item))
					{
						s_itemEdit.copy(&item);

						EndDialog(hDlg,IDOK);
					}
					break;
				}

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_ITEM_LIST			:
				{
					switch (HIWORD(wParam))
                    {	case	CBN_SELCHANGE	:
						{
							HWND	hCombo	=	GetDlgItem(hDlg,IDC_ITEM_LIST);

							int	iSelect		=	cCOMBOBOX::GetData(hCombo);

							SelchangeItem(hDlg,iSelect);

							break;
						}
					}
					break;
				}

				case	IDC_PREFIX1				:
				case	IDC_PREFIX2				:
				case	IDC_PREFIX3				:
				{
					switch (HIWORD(wParam))
                    {	case	CBN_SELCHANGE	:
						{
							HWND	hCombo	=	GetDlgItem(hDlg,LOWORD( wParam ) );
							int		iSelect	=	cCOMBOBOX::GetData(hCombo);
							int		iIndex	=	0;

							if (LOWORD( wParam )	==	IDC_PREFIX1)	iIndex		=	0;
							if (LOWORD( wParam )	==	IDC_PREFIX2)	iIndex		=	1;
							if (LOWORD( wParam )	==	IDC_PREFIX3)	iIndex		=	2;

							SelchangePrefix(hDlg,iSelect,iIndex);

							break;
						}
					}
					break;
				}

				case	IDC_PREFIX1_KIND		:
				case	IDC_PREFIX2_KIND		:
				case	IDC_PREFIX3_KIND		:
				{
					switch (HIWORD(wParam))
                    {	case	CBN_SELCHANGE	:
						{
							HWND	hCombo	=	GetDlgItem(hDlg,LOWORD( wParam ) );

							int	iSelect		=	cCOMBOBOX::GetData(hCombo);
							int	iIndex		=	0;

							if (LOWORD( wParam )	==	IDC_PREFIX1_KIND)
							{
								iIndex			=	0;
								s_iPrefix1Kind	=	iSelect;
							}
							if (LOWORD( wParam )	==	IDC_PREFIX2_KIND)
							{
								iIndex			=	1;
								s_iPrefix2Kind	=	iSelect;
							}
							if (LOWORD( wParam )	==	IDC_PREFIX3_KIND)
							{
								iIndex			=	2;
								s_iPrefix3Kind	=	iSelect;
							}

							FillPrefixCombo(hDlg,iIndex);

							break;
						}
					}
					break;
				}

				case	IDC_ITEM_KIND			:
				{
					switch (HIWORD(wParam))
                    {	case	CBN_SELCHANGE	:
						{
							HWND	hCombo	=	GetDlgItem(hDlg,IDC_ITEM_KIND);

							int		iSelect	=	cCOMBOBOX::GetData(hCombo);

							s_iItemKind		=	iSelect;

							FillItemCombo(hDlg);
							hCombo			=	GetDlgItem(hDlg,IDC_ITEM_LIST);
							int		iItem	=	cCOMBOBOX::GetData(hCombo);

							SelchangeItem(hDlg,iItem);

							break;
						}
					}
					break;
				}

			}
			break;
	}

	return (0L);
}
