#include "setSetItem.h"
#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cSET.H"
#include "cDRAW.H"
#include "cITEM.H"
#include "cTEXT.H"
#include "cITEM_PREFIX_EFFECT.H"
#include "cITEM_EFFECT.H"
#include "cPROJECT.H"
#include "resource.h"
#include "CSetCustomItem.h"

HWND					cSetItemSet::s_hWnd;
cDIBWND					cSetItemSet::s_dibSetEffectData;
cDIBWND					cSetItemSet::s_dibSetItemList;
cLISTCONTROL			cSetItemSet::s_listItemSet;
cSCROLL_BAR				cSetItemSet::s_sbSetItemEffectList;
cItemSet				cSetItemSet::s_makeGroup;

int						cSetItemSet::s_iSelectSetGroup		=	0;
int						cSetItemSet::s_iSelectSetCount		=	0;
int						cSetItemSet::s_iSelectSetEffect	=	0;
int						cSetItemSet::s_iSelectSetItem		=	0;
cRECT					cSetItemSet::s_rectSetItemList;

int						cSetItemSet::s_iSetEffectDlgHeight;
int						cSetItemSet::s_iDecodeSetItemIndex	=	0;

BOOL
cSetItemSet::Open(HWND _hWnd)//	시작
{
	if (DialogBox(s_hInst, "SETITEM_GROUP_LIST",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cSetItemSet::InitDlg(HWND _hDlg)
{
	s_hWnd	=	_hDlg;
	
	HWND	hSetItemList				=	GetDlgItem(_hDlg,IDC_SET_ITEM_LIST);

	s_dibSetItemList.lpszRegister		=	"SET_ITEM_LIST_WINDOW";
	s_dibSetItemList.Init(hSetItemList,(WNDPROC)ProcSetItemListDlg);

	InitSetEffectListWnd(_hDlg);

//	그룹 리스트
	{
		HWND	hGroupList	=	GetDlgItem(_hDlg,IDC_SET_ITEM_GROUP_LIST);

		s_listItemSet.Init(hGroupList,2,FALSE,ProcItemSetListWnd);		//	3개의 컬럼

		s_listItemSet.AddColumn("No"		,6*5);
		s_listItemSet.AddColumn("이름"		,6*32);

		ResetItemSetListBox(_hDlg);
	}
	
	Draw(_hDlg);
}

//
//	
void
cSetItemSet::ResetItemSetListBox(HWND hwnd)
{
	s_listItemSet.ResetItems();
	
	if	(g_itemSetManager.getCount() <= 1)
		return;

	int		iIndex;

	for (iIndex=1;iIndex<g_itemSetManager.getMaxCount();iIndex++)
	{
		cItemSet	*lpGroup	=	g_itemSetManager.get(iIndex);

		if	(iIndex	==	106)
			iIndex	=	iIndex;

		if	(!lpGroup)
			continue;

		s_listItemSet.AddItem(0,_ms("%-.4d",iIndex),iIndex);
		s_listItemSet.AddItem(1,lpGroup->m_strName);
		s_listItemSet.IncreaseLineCounter();
	}

	s_listItemSet.Sort(0);

	if (!s_listItemSet.selectByData(0,s_iSelectSetGroup))
	{
		if (!s_listItemSet.Select(0))
			s_iSelectSetGroup	=	0xffff;
		else
			s_iSelectSetGroup	=	s_listItemSet.GetData(0,0);
	}
}

void
cSetItemSet::InitSetEffectListWnd(HWND _hWnd)
{
	HWND	hSetEffectDataWnd		=	GetDlgItem(_hWnd,IDC_EFFECT_LIST);

	s_dibSetEffectData.lpszRegister	=	"Red Stone Set Effect DibWnd";
	s_dibSetEffectData.Init	(hSetEffectDataWnd,(WNDPROC)ProcSetEffectDlg);

	s_sbSetItemEffectList.Init(hSetEffectDataWnd);		//	스크롤바 초기화
	s_sbSetItemEffectList.InitHeight();

	s_iSetEffectDlgHeight		=	s_dibSetEffectData.height();	//	아이템 리스트 높이

	s_sbSetItemEffectList.SetHeight(s_iSetEffectDlgHeight,s_dibSetEffectData.height());
}

void
cSetItemSet::Draw(HWND _hDlg)
{
	cItemSet	*lpGroup	=	g_itemSetManager.get(s_iSelectSetGroup);

	if (lpGroup)
		SetDlgItemInt(s_hWnd,IDC_EQUIP_LEVEL,lpGroup->m_wEqiupLevel,FALSE);
	else
		SetDlgItemInt(s_hWnd,IDC_EQUIP_LEVEL,0,FALSE);

	DrawItemSetListWnd(s_hWnd);
	DrawSetItemListWnd(s_hWnd);
	DrawSetEffect(s_hWnd);
}

void
cSetItemSet::DrawItemSetListWnd(HWND _hDlg)
{
	if (g_itemSetManager.getCount() <= 1)
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_EDIT),TRUE);
		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE),TRUE);
	}

	if (g_itemSetManager.getCount() == g_itemSetManager.getMaxCount())
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD),TRUE);
	else
		EnableWindow(GetDlgItem(_hDlg,IDC_ADD),TRUE);
	
	s_listItemSet.Draw();
}

//
//	아이템 리스트 윈도우 그리기
void
cSetItemSet::DrawSetItemListWnd(HWND _hDlg)
{
	int	x=4,y=5;

	s_dibSetItemList.Active();

	s_rectSetItemList.Set(0,0,cDRAW::WIDTH,cDRAW::HEIGHT);

	cItemSet	*lpGroup	=	g_itemSetManager.get(s_iSelectSetGroup);

	if (!lpGroup	||	lpGroup->getItemCount()	<=	0)
	{
		cDRAW::Fill(_SKYBLUE);

		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,dMSG_ITEM_NOT_EXIST);
		cTEXT::PopShadow();

		EnableWindow(GetDlgItem(_hDlg,IDC_REMOVE_ITEM),FALSE);

		s_dibSetItemList.Draw();

		return;
	}

	cDRAW::Fill(_SKYBLUE);

	int		iCount		=	0;
	int		iWidth		=	s_dibSetItemList.width();

	for (int i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		cSetItem	*lpSetItem		=	(cSetItem	*)&lpGroup->m_aItem[i];

		if (lpSetItem->m_wBaseItem	==	0xffff)
			break;

		cITEM	item;

		CSetCustomItem::GenerateItemBySetItem(&item,lpSetItem);

		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpSetItem->m_wBaseItem];

		iCount++;

		if (i==	s_iSelectSetItem)
		{
			cDRAW::Fill		(0			,x  ,y+2,iWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_BLUE		,x-1,y+1,iWidth-3,y+dICON_SHAPE_HEIGHT+5);
			cDRAW::Box		(_DEEPGRAY	,x-1,y+1,iWidth-3,y+dICON_SHAPE_HEIGHT+5);

			g_smiIconItem.put(lpItem->m_wIconShape,x+1,y+4);

			cTEXT::Put		(x+1+36	,y+4+2	,WHITE,_ms("[%.2d] %s",i+1,lpSetItem->m_strName));
			cTEXT::Put		(x+1+36	,y+4+18	,WHITE,g_strItemCategory[lpItem->m_wKind]);
		}
		else
		{
			cDRAW::Fill		(0			,x  ,y+2,iWidth-2,y+dICON_SHAPE_HEIGHT+6);
			cDRAW::Fill		(_DEEPGRAY	,x-2,y  ,iWidth-4,y+dICON_SHAPE_HEIGHT+4);
			cDRAW::Box		(0			,x-2,y  ,iWidth-4,y+dICON_SHAPE_HEIGHT+4);

			g_smiIconItem.put(lpItem->m_wIconShape,x	,y+3);

			cTEXT::Put		(x+36	,y+3+2	,LTGRAY,_ms("[%.2d] %s",i+1,lpSetItem->m_strName));
			cTEXT::Put		(x+36	,y+3+18	,LTGRAY,g_strItemCategory[lpItem->m_wKind]);
		}

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	s_dibSetItemList.Draw();
}

//
//	유니크 아이템 데이터
void
cSetItemSet::DrawSetEffect(HWND _hWnd)
{
//	아이템 효과
	s_dibSetEffectData.Active();

	cDRAW::Fill(_SKYBLUE);

	int		x =4,y=4-s_sbSetItemEffectList.m_iHeightPos;

	int		iWidth			=	s_dibSetEffectData.width() - 34;
	int		iLineHeight		=	14;
	int		iCount			=	0;

	s_iSetEffectDlgHeight	=	0;
	s_iSetEffectDlgHeight	=	4;

	cItemSet	*lpGroup	=	g_itemSetManager.get(s_iSelectSetGroup);

	if (!lpGroup || lpGroup->getSetEffectCount() <= 0)
	{
		cTEXT::PushShadow(FALSE);
		cTEXT::Put(5,5,0,"설정된 세트 효과 없음");
		cTEXT::PopShadow();
		s_dibSetEffectData.Draw();

		return;
	}

	for (int iSetItemCount=0;iSetItemCount<dMAX_ITEM_COUNT_IN_SET;iSetItemCount++)
	{
		BOOL	bIsPutSetItemCount	=	FALSE;

		int		iCurrentCount		=	0;

		for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
		{
			cUniqueData	*lpEffect	=	&lpGroup->m_aSetEffect[iSetItemCount][i];

			if (lpEffect->m_wEffect	==	0xffff)
				break;

			iCount++;
			iCurrentCount++;

			char	*lpComment	=	lpEffect->getComment(TRUE);
			int		iLineCount	=	cTEXT::GetLines(iWidth,lpComment);

			x	=	5;

			if (bIsPutSetItemCount	==	FALSE)
			{
				cDRAW::Fill(0,x-2,y-2,x+10+6,y+iLineHeight);
				cTEXT::Put(x,y,LTGREEN,_ms("%.2d",iSetItemCount+1));

				bIsPutSetItemCount	=	TRUE;
			}

			x	=	25;

			if (i	==	s_iSelectSetEffect && iSetItemCount	==	s_iSelectSetCount)
			{
				cDRAW::Fill		(_LTBLUE,x-2,y-2,x+iWidth+4,y+iLineCount*iLineHeight);
				cDRAW::Fill		(0,x-2,y+iLineCount*iLineHeight+1,x+iWidth+4,y+iLineCount*iLineHeight+1);
				cTEXT::FitPut	(x+2,y,iWidth,iLineCount*8*2,LTYELLOW,lpComment);
			}
			else
			{	
				if (iSetItemCount	==	s_iSelectSetCount)
				{
					cDRAW::Fill		(0 ,x-2,y-1,x+iWidth+4,y+iLineCount*iLineHeight+2);
					cTEXT::FitPut	(x+2,y+1,iWidth,iLineCount*8*2,WHITE,lpComment);
				}
				else
				{
					cDRAW::Fill		(_DEEPGRAY,x-2,y-2,x+iWidth+4,y+iLineCount*iLineHeight+2);
					cTEXT::FitPut	(x+2,y+1,iWidth,iLineCount*8*2,WHITE,lpComment);
				}
			}

			y						+=	iLineCount*iLineHeight+2;
			s_iSetEffectDlgHeight	+=	iLineCount*iLineHeight+2;
		}

		if (iCurrentCount)
		{
			y						+=	4;
			s_iSetEffectDlgHeight	+=	4;
		}
	}

	s_sbSetItemEffectList.SetHeight(s_iSetEffectDlgHeight,s_dibSetEffectData.height());

	s_dibSetEffectData.Draw();
}

//
//	
int
cSetItemSet::SelectSetItem(int _iX,int _iY)
{
	int	x=5,y=5;

	cItemSet	*lpGroup	=	g_itemSetManager.get(s_iSelectSetGroup);

	if (!lpGroup || lpGroup->getItemCount()	<=	0)
		return	0xffff;

	int		iIndex		=	s_sbSetItemEffectList.m_iHeightPos;
	int		iCount		=	0;

	if (!s_rectSetItemList.isIN(_iX,_iY))
		return	0xffff;

	for (int i=iIndex;i<dMAX_ITEM_COUNT_IN_SET;i++)
	{
		cSetItem	*lpSetItem		=	(cSetItem	*)&lpGroup->m_aItem[i];

		if (lpSetItem->m_wBaseItem	==	0xffff)
			break;

		cBASIC_ITEM	*lpItem	=	&g_aBasicItem[lpSetItem->m_wBaseItem];

		iIndex++;
		iCount++;

		if (_iY	>=	y	&&	_iY	<=	y	+	dICON_SHAPE_HEIGHT+8)
			return	i;

		y	+=	(dICON_SHAPE_HEIGHT+8);
	}

	return	0xffff;
}

//
//	아이템 효과 선택
int
cSetItemSet::SelectSetItemEffect(int _iX,int _iY)
{
	int		x =4,y=4-s_sbSetItemEffectList.m_iHeightPos;

	int		iWidth			=	s_dibSetEffectData.width() - 34;
	int		iLineHeight		=	14;
	int		iCount			=	0;

	cItemSet	*lpGroup	=	g_itemSetManager.get(s_iSelectSetGroup);

	if (!lpGroup)
		return	0xffff;

	for (int iSetItemCount=0;iSetItemCount<dMAX_ITEM_COUNT_IN_SET;iSetItemCount++)
	{
		int		iCurrentCount		=	0;

		for (int i=0;i<dMAX_SET_ITEM_EFFECT_COUNT;i++)
		{
			cUniqueData	*lpEffect	=	&lpGroup->m_aSetEffect[iSetItemCount][i];

			if (lpEffect->m_wEffect	==	0xffff)
				break;

			iCount++;
			iCurrentCount++;

			char	*lpComment	=	lpEffect->getComment(TRUE);
			int		iLineCount	=	cTEXT::GetLines(iWidth,lpComment);

			x	=	5;

			if (_iY	>=	y	&&	_iY	<=	y+iLineCount*iLineHeight+4)
			{
				if (s_iSelectSetCount	!=	iSetItemCount)
				{
					int	iSetCount	=	cCOMBOBOX::SelectByData(GetDlgItem(s_hWnd,IDC_SET_ITEM_COUNT),iSetItemCount);

					s_iSelectSetCount	=	iSetItemCount;
				}

				return	i;
			}

			y						+=	iLineCount*iLineHeight+2;
		}

		if (iCurrentCount)
			y						+=	4;
	}

	return	0xffff;
}

enum
{
	eDSITSR_FAILED,
	eDSITSR_READ_SET_ITEM,
	eDSITSR_READ_ITEM_SET_END,
};

BOOL
cSetItemSet::DecodeTextScriptFile(char *_lpstrFn)
{
	cDECODER	decoder;

	if (!decoder.Upload(_lpstrFn,NULL))
		return	FALSE;

	int	iOverlappedCount	=	0;

	g_itemSetManager.reset();
	g_itemSetManager.build();

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream(13);

		if (!strOneLine)
			break;

		int	iPoint	=	0;

		if (strOneLine[0]	==	10)
			iPoint++;


		int	iResult	=	DecodeTextScript(strOneLine+iPoint);

		if (iResult	==	eDSITSR_FAILED)
			return	FALSE;

		if (iResult	==	eDSITSR_READ_SET_ITEM)	
			continue;

		if (iResult	==	eDSITSR_READ_ITEM_SET_END)
		{
			int	iSerial	=	g_itemSetManager.add(&s_makeGroup);

			if (iSerial	==	0xffff)
			{
				ERRMSG("The item set name has a duplicate name.","%s",s_makeGroup.m_strName);
				iOverlappedCount++;
			}
		}
	}

	cMSG::Put("Data file analysis results", "%d item sets were added.\n\n%d item set names were duplicated.", g_itemSetManager.getCount() - 1, iOverlappedCount);

	return	TRUE;
}

BOOL
cSetItemSet::DecodeTextScript(char *_lpstrText)
{
	cDECODER	decoder;

	if (strlen(_lpstrText)	<=	5)
		return	FALSE;

	decoder.UploadBuffer(_lpstrText,NULL);

	char			*lpStream;
	int				iValidCount		=	0;
	int				iGrade			=	0;
	int				iItemCount		=	0;
	int				iSetEffectIndex	=	0;

	cSetItemDefine	*lpEditSetItem	=	NULL;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		iGrade++;

		if (iGrade	>=	33+20)
			break;

		if (!lpStream)
		{
			iValidCount++;
		
			if (iValidCount	>=	30)
				return	eDSITSR_READ_ITEM_SET_END;

			continue;
		}

		iValidCount		=	0;

		int	iIndex		=	iGrade;
		int	iValue		=	0;

		if (iIndex		>=	8	&&	iIndex	<=	13)
		{
			int				iCase		=	(iIndex-8)%2;
			int				iPrefixIndex=	(iIndex-8)/2;
			CItemPrefixInfo	*lpPrefixInfo=	&lpEditSetItem->m_aPrefix[iPrefixIndex];

			switch(iCase)
			{
				case	0	:
				{
					cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.getPrefix(lpStream);

					if (!lpPrefix)
						return	ERRMSG("Error!!","The prefix '%s' does not exist.",lpStream);

					lpPrefixInfo->m_wPrefix		=	lpPrefix->m_wSerial;
					break;
				}

				case	1	:
				{
					cDECODER	dec;

					dec.UploadBuffer(lpStream,NULL);

					lpPrefixInfo->m_aValue[0]	=	dec.GetNumber();
					lpPrefixInfo->m_aValue[1]	=	dec.GetNumber(FALSE);

					break;						
				}
			}
		}
		else
		if (iIndex	>=	15	&&	iIndex	<=	26)
		{
			int				iCase		=	(iIndex-15)%2;
			int				iEffectIndex=	(iIndex-15)/2;
			cUniqueData		*lpSetEffect=	&s_makeGroup.m_aSetEffect[iSetEffectIndex][iEffectIndex];

			switch(iCase)
			{
				case	0	:
				{
					lpSetEffect->m_wEffect		=	g_itemPrefix.getPrefixType(lpStream);

					if (lpSetEffect->m_wEffect	==	0xffff)
						return	ERRMSG("Error!!","The item '%s' has no effect.",lpStream);
					break;
				}

				case	1	:
				{
					cDECODER	dec;

					dec.UploadBuffer(lpStream,NULL);

					lpSetEffect->m_aValue[0]	=	dec.GetNumber(FALSE);
					lpSetEffect->m_aValue[1]	=	dec.GetNumber(FALSE);
					lpSetEffect->m_aValue[2]	=	dec.GetNumber(FALSE);
					lpSetEffect->m_aValue[3]	=	dec.GetNumber(FALSE);
					lpSetEffect->m_aValue[4]	=	dec.GetNumber(FALSE);
					lpSetEffect->m_aValue[5]	=	dec.GetNumber(FALSE);
					lpSetEffect->m_aValue[6]	=	dec.GetNumber(FALSE);
					lpSetEffect->m_aValue[7]	=	dec.GetNumber(FALSE);

					break;						
				}
			}
		}
		else
		if(iIndex	>=	35	&&	iIndex	<	35+16)
		{
			int		iJob		=	iIndex-35;
			UINT	uiJobMask	=	1<<iJob;

			if	(stricmp(lpStream,"o") == 0)
			{
				lpEditSetItem->m_uiEnableJobMask	+=	uiJobMask;
			}
		}
		else
		switch(iIndex)
		{
			case	1	:
			{
				iValue	=	atoi(lpStream);

				s_makeGroup.reset();
				s_makeGroup.m_wSerial	=	iValue;
				s_iDecodeSetItemIndex	=	0;
				break;
			}
			case	2	:
			{
				iValue	=	atoi(lpStream);

				s_makeGroup.m_wEqiupLevel	=	iValue;
				break;
			}

			case	3	:
			{
				strcpy(s_makeGroup.m_strName,lpStream);
				break;
			}

			case	4	:
			{
				s_makeGroup.setComment(lpStream);
				break;
			}

			case	5	:
			{
				lpEditSetItem						=	&s_makeGroup.m_aItem[s_iDecodeSetItemIndex];
				lpEditSetItem->reset();

				lpEditSetItem->m_bf1IsCustomItem	=	TRUE;

				cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::Get(lpStream);

				if (!lpItem)
					return	ERRMSG("Error!!","There is no item called '%s'.",lpStream);

				CCustomItem		customItem;

				customItem.m_wBaseItem			=	lpItem->m_iSerial;
				customItem.m_bCount				=	1;
				customItem.m_wGenerateQuality	=	100;

				CSetCustomItem::GenerateItemByCustomItem((cITEM *)lpEditSetItem,&customItem);
				break;
			}

			case	6	:
			{
				lpEditSetItem						=	&s_makeGroup.m_aItem[s_iDecodeSetItemIndex];
				strcpy(lpEditSetItem->m_strName,lpStream);
				break;
			}

			case	7	:
			{
				lpEditSetItem						=	&s_makeGroup.m_aItem[s_iDecodeSetItemIndex];
				lpEditSetItem->setComment(lpStream);
				break;
			}

			case	14	:
			{
				iSetEffectIndex		=	atoi(lpStream)-1;
				break;
			}

			case	27	:
			{
				lpEditSetItem->m_wDropChance	=	StringToNumber(lpStream,100,2);
				break;
			}

			case	28	:
			{
				if (stricmp(lpStream,"o")==0)
					lpEditSetItem->m_bf1IsDropInQuest				=	TRUE;
				break;
			}

			case	29	:
			{
				if (stricmp(lpStream,"o")==0)
					lpEditSetItem->m_bf1IsDropInTempDungeonMonster	=	TRUE;
				break;
			}

			case	30	:
			{
				if (stricmp(lpStream,"o")==0)
					lpEditSetItem->m_bf1IsDropInTempDungeonArca		=	TRUE;
				break;
			}

			case	31	:
			{
				if (stricmp(lpStream,"o")==0)
					lpEditSetItem->m_bf1IsDropInNormalFieldMonster	=	TRUE;
				break;
			}

			case	32:
			{
				if (stricmp(lpStream,"o")==0)
					lpEditSetItem->m_bf1IsDropInNormalFieldArca		=	TRUE;
				break;
			}

			case	33	:
			{
				lpEditSetItem->m_wDropLevel	=	atoi(lpStream);
				break;
			}

			case	34	:
			{
				if (stricmp(lpStream,"x")==0)
					lpEditSetItem->m_bf4ReversionType	=	eIRT_REVERSION_WHEN_GET;
				break;
			}
		}
	}

	s_iDecodeSetItemIndex++;

	return	eDSITSR_READ_SET_ITEM;
}

void
cSetItemSet::LoadingTextScript(HWND _hDlg)
{
	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	if (!cFILE::LoadBox(strFn, "Text file {*.txt}\0*.txt\0\0", "txt", NULL, "Load set item text script data file"))
	{
		SetLogFolder();
		return;
	}

	DecodeTextScriptFile(strFn);

	SetLogFolder();

	ResetItemSetListBox(_hDlg);
	Draw(_hDlg);
}

void
cSetItemSet::AddGroup(HWND _hDlg)
{
	if	(!cMakeItemSet::Open(_hDlg))
		return;

	int		iSerial		=	g_itemSetManager.add(&cMakeItemSet::s_makeItemSet);

	if(iSerial	==	0xffff)
		return;

	s_iSelectSetGroup	=	iSerial;

	ResetItemSetListBox(_hDlg);
	Draw(_hDlg);
}

void
cSetItemSet::EditGroup(HWND _hDlg)
{
	if(g_itemSetManager.getCount() <= 1 || s_iSelectSetGroup<=0)
		return;

	cItemSet	*lpGroup	=	g_itemSetManager.get(s_iSelectSetGroup);

	if (!lpGroup)
		return;

	if	(!cMakeItemSet::Open(_hDlg,lpGroup))
		return;

	g_itemSetManager.replace(lpGroup->m_wSerial,&cMakeItemSet::s_makeItemSet);

	ResetItemSetListBox(_hDlg);
	Draw(_hDlg);
}

void
cSetItemSet::RemoveGroup(HWND _hDlg)
{
	cItemSet	*lpGroup=	g_itemSetManager.get(s_iSelectSetGroup);

	if (!lpGroup)
		return;

	if (cMSG::YESNO("Remove set item", "Are you sure you want to remove the selected set item?") == IDNO)
		return;

	g_itemSetManager.remove(s_iSelectSetGroup);

	ResetItemSetListBox(_hDlg);
	Draw(_hDlg);
}

void
cSetItemSet::ChangeSetGroup(HWND _hDlg,int _iGroup)
{
	cItemSet	*lpGroup=	g_itemSetManager.get(_iGroup);

	if (!lpGroup)
		return;

	s_iSelectSetGroup		=	_iGroup;

	s_iSelectSetCount		=	0;
	s_iSelectSetEffect		=	0;
	s_iSelectSetItem		=	0;

	Draw(_hDlg);
}

void
cSetItemSet::FilterSetItem(HWND _hDlg)		// 09.08.18
{

	char strFilter[256] = "";
	GetDlgItemText(_hDlg,IDC_EDIT_SETITEM_FILTER,strFilter,256);
	int iFilterNameLen	= strlen(strFilter);
	
	s_listItemSet.ResetItems();
	
	if(iFilterNameLen<=0)
	{
		InitDlg(_hDlg);
		return;
	}

	int	iCount	=	0;

	for (int i=0;i<g_itemSetManager.getMaxCount();++i)
	{

		cItemSet	*lpGroup	=	g_itemSetManager.get(i);
		if	(!lpGroup)
			continue;
		++iCount;
		int iItemNameLen	= strlen(lpGroup->m_strName);
		int iLenGap			= iItemNameLen - iFilterNameLen;
		if(iLenGap<0)
			continue;
		
		for(int iName=0;iName<=iLenGap;++iName)
		{
			if(strnicmp(strFilter,&lpGroup->m_strName[iName], iFilterNameLen) == 0 )
			{
				s_listItemSet.AddItem(0,_ms("%-.4d",i),i);
				s_listItemSet.AddItem(1,lpGroup->m_strName);
				s_listItemSet.IncreaseLineCounter();
				break;		

			}
		}
		
	}
	Draw(_hDlg);
}


LRESULT CALLBACK
cSetItemSet::ProcItemSetListWnd(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)		// 09.08.11
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN	:
		{
			s_iSelectSetGroup	=	s_listItemSet.GetSelectData();
			
			Draw(hDlg);
			SetFocus(hDlg);
			break;
		}
	case WM_MOUSEWHEEL :
		{
			
			int zDelta = ((short)HIWORD(wParam));
			
			if	(zDelta>=0){
				
				s_listItemSet.VScroll(SB_LINEUP);
				
			}else{
				s_listItemSet.VScroll(SB_LINEDOWN);
			}
			
			Draw(hDlg);
			break;
		}
	}

	return (0L);
}

LRESULT CALLBACK
cSetItemSet::ProcSetItemListDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{
			PAINTSTRUCT ps;

			BeginPaint( hDlg, &ps );

			s_dibSetItemList.Active();
			s_dibSetItemList.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hDlg, &ps);
			break;
		}


		case WM_LBUTTONDOWN	:
		{
			int item		=	SelectSetItem(LOWORD(lParam),HIWORD(lParam));

			if(item			==	0xffff		)
				break;
			if(item			==	s_iSelectSetItem)
				break;

			s_iSelectSetItem=	item;

			DrawSetItemListWnd(s_hWnd);
//			DrawSelectSetItemInfoWnd(s_hWnd);

			break;
		}

		default				:
			return( DefWindowProc( hDlg, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//
//	
LRESULT CALLBACK
cSetItemSet::ProcSetEffectDlg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	
			PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibSetEffectData.Active();
			s_dibSetEffectData.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN	:
		{
			int item	=	SelectSetItemEffect(LOWORD(lParam),HIWORD(lParam));

			if	(item	==	0xffff)
				break;

			s_iSelectSetEffect	=	item;

			DrawSetEffect(hwnd);
			break;
		}

		case WM_VSCROLL			:
			if (s_sbSetItemEffectList.VScroll(wParam))
				DrawSetEffect(s_hWnd);
			break;

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}

//
//	다얄로그 박스 프로시져
LRESULT CALLBACK
cSetItemSet::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG	:
			InitDlg(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;

		case	WM_COMMAND	:
			switch( LOWORD( wParam ) )
            {
				case	IDOK					:
				{
					EndDialog(hDlg,IDOK);
					break;
				}

				case	IDCANCEL				:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_LOADING	:
				{
					LoadingTextScript(hDlg);
					break;
				}
				case	IDC_ADD		:
				{
					AddGroup(hDlg);
					break;
				}
				case	IDC_EDIT	:
				{
					EditGroup(hDlg);
					break;
				}
				case	IDC_REMOVE	:
				{
					RemoveGroup(hDlg);
					break;
				}
				case	IDC_EDIT_SETITEM_FILTER :		// 09.08.18
					{
						if	(HIWORD(wParam) != EN_CHANGE)
							break;
						FilterSetItem(hDlg);
						break;
					}


			}
			break;
	}

	return (0L);
}