#include "setArca.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "CItem_Effect.h"
#include "trapSkill.h"
#include "setItem.h"
#include "pieceItem.h"

cLISTCONTROL	cMakeArca::s_listDropItemList;	//	드롭 아이템 리스트
cLISTCONTROL	cMakeArca::s_listAddItemList;	//	추가 가능 아이템 리스트
HWND			cMakeArca::s_hWnd;
cArcaInfo		cMakeArca::s_makeArca;
int				cMakeArca::s_iSelectItemSet;
int				cMakeArca::s_iSelectPieceItem;

BOOL
cMakeArca::Open(HWND _hWnd,cArcaInfo *_lpEdit)
{
	if (_lpEdit)
		s_makeArca.copy(_lpEdit);
	else
		s_makeArca.reset();

	if (DialogBox( s_hInst, "ARCA_MAKE",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cMakeArca::InitData(HWND hDlg)
{
	int		i;
	HWND	hDropItemList,hAddItemList,hArcaList,hCustomItem,hTrapTarget;
	HWND	hTrapSkillList1,hTrapSkillList2;
	HWND	hItemSetList,hSetItemList;
	HWND	hPieceItem,hItemPiece;

	s_hWnd			=	hDlg;

	hArcaList			=	GetDlgItem(hDlg,IDC_ARCA_LIST);			//	보물상자 리스트
	hDropItemList		=	GetDlgItem(hDlg,IDC_DROP_ITEM_LIST);	//	떨어뜨리는 아이템 리스트
	hAddItemList		=	GetDlgItem(hDlg,IDC_ADD_ITEM_LIST);		//	추가 가능한 아이템 리스트
	hCustomItem			=	GetDlgItem(hDlg,IDC_CUSTOM_ITEM);		//	커스텀 아이템
	hTrapTarget			=	GetDlgItem(hDlg,IDC_TRAP_TARGET);		//	함정 타겟
	hTrapSkillList1		=	GetDlgItem(hDlg,IDC_TRAP_SKILL1);
	hTrapSkillList2		=	GetDlgItem(hDlg,IDC_TRAP_SKILL2);
	hItemSetList	=	GetDlgItem(hDlg,IDC_SET_ITEM_GROUP_LIST);
	hSetItemList		=	GetDlgItem(hDlg,IDC_SET_ITEM_LIST);
	hPieceItem			=	GetDlgItem(hDlg,IDC_PIECE_ITEM);
	hItemPiece			=	GetDlgItem(hDlg,IDC_ITEM_PIECE);

	{
		cCOMBOBOX::Reset(hTrapTarget);

		for (i=0;;i++)
		{
			if (stricmp(g_strTrapTarget[i],"end")==0)
				break;

			cCOMBOBOX::Add(hTrapTarget,g_strTrapTarget[i],i);
		}

		cCOMBOBOX::Select(hTrapTarget,0);
	}

	{
		cCOMBOBOX::Reset(hTrapSkillList1);

		cCOMBOBOX::Add(hTrapSkillList1,"valid",0xffff);

		for (i=0;i<g_trapSkillManager.m_iCount;i++)
		{
			cTrapSkill	*lpSkill	=	&g_trapSkillManager.m_aTrapSkill[i];

			cCOMBOBOX::Add(hTrapSkillList1,lpSkill->m_strName,i);
		}

		cCOMBOBOX::Select(hTrapSkillList1,0);

		cCOMBOBOX::Reset(hTrapSkillList2);

		cCOMBOBOX::Add(hTrapSkillList2,"valid",0xffff);
		for (i=0;i<g_trapSkillManager.m_iCount;i++)
		{
			cTrapSkill	*lpSkill	=	&g_trapSkillManager.m_aTrapSkill[i];

			cCOMBOBOX::Add(hTrapSkillList2,lpSkill->m_strName,i);
		}

		cCOMBOBOX::Select(hTrapSkillList2,0);
	}

	{
		cCOMBOBOX::Reset(hCustomItem);

		if (g_customItem.getCount() == 0)
			cCOMBOBOX::Add(hCustomItem,"valid",-1);
		else
		{
			for (i=0;i<g_customItem.getCount();i++)
			{
				CCustomItem	*lpItem	=	g_customItem.getItem(i);

				if (lpItem)
					cCOMBOBOX::Add(hCustomItem,lpItem->m_strName,i);
			}
		}

		cCOMBOBOX::Select(hCustomItem,0);
	}

	{
		cCOMBOBOX::Reset(hItemSetList);

		if (g_itemSetManager.getCount() <= 1)
			cCOMBOBOX::Add(hItemSetList,"valid",-1);
		else
		{
			for (i=1;i<g_itemSetManager.getCount();i++)
			{
				cItemSet	*lpGroup	=	g_itemSetManager.get(i);

				if (lpGroup)
					cCOMBOBOX::Add(hItemSetList,lpGroup->m_strName,i);
			}
		}

		cCOMBOBOX::Select(hItemSetList,0);

		cCOMBOBOX::Reset(hSetItemList);

		s_iSelectItemSet	=	cCOMBOBOX::GetData(hItemSetList);

		if (s_iSelectItemSet	==	-1)
			cCOMBOBOX::Add(hSetItemList,"valid",-1);
		else
		{
			cItemSet	*lpGroup	=	g_itemSetManager.get(s_iSelectItemSet);

			if (lpGroup && lpGroup->getItemCount())
			{
				for (i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
				{
					cSetItem	*lpItem	=	lpGroup->getItem(i);

					if (lpItem)
						cCOMBOBOX::Add(hSetItemList,g_aBasicItem[lpItem->m_wBaseItem].m_strName,i);
				}
			}
			else
				cCOMBOBOX::Add(hSetItemList,"valid",-1);
		}

		cCOMBOBOX::Select(hSetItemList,0);
	}

	{
		cCOMBOBOX::Reset(hPieceItem);
		cCOMBOBOX::Reset(hItemPiece);

		if (g_pieceItemManager.getCount() <= 1)
		{
			cCOMBOBOX::Add(hPieceItem,"valid",-1);
			cCOMBOBOX::Add(hItemPiece,"valid",-1);
		}
		else
		{
			for (i=1;i<g_pieceItemManager.getMaxCount();i++)
			{
				cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(i);

				if (!lpPieceItem)
					continue;

				cCOMBOBOX::Add(hPieceItem,lpPieceItem->m_strName,i);
			}

			cPieceItem	*lpPieceItem	=	g_pieceItemManager.getFirst(1);

			for (i=0;i<lpPieceItem->m_wPieceCount;i++)
				cCOMBOBOX::Add(hItemPiece, _ms("%.2d piece", i + 1), i);
		}
		cCOMBOBOX::Select(hPieceItem,0);
		cCOMBOBOX::Select(hItemPiece,0);

		s_iSelectPieceItem	=	cCOMBOBOX::GetData(hPieceItem);
	}

	//	바닥에 딸어지는 아이템 데이터/리스트
	{
		s_listDropItemList.Init		(hDropItemList	,4		,FALSE	,ProcDropItemList);		//	2개의 컬럼
		s_listDropItemList.AddColumn("classification"			,6*16	);
		s_listDropItemList.AddColumn(dMSG_NAME		,6*28	);
		s_listDropItemList.AddColumn(dMSG_FREQUENCY	,6*14	);
		s_listDropItemList.AddColumn(dMSG_COUNT		,6*7	);
	}

	//	바닥에 딸어지는 아이템 데이터
	{
		s_listAddItemList.Init		(hAddItemList	,1		,FALSE	);	//	2개의 컬럼
		s_listAddItemList.AddColumn	(dMSG_NAME		,6*30	);
		s_listAddItemList.HideColumn();

		UpdateAddItemList(hDlg);
	}

	FillArcaInfo(hDlg,FALSE);

	Draw(hDlg);
}

//
//	아이템 리스트 업데이트
void
cMakeArca::UpdateAddItemList(HWND hDlg)
{
	int		iIndex	=	0;

	s_listAddItemList.ResetItems();

	while(1)
	{
		if (strcmp(g_strItemCategory[iIndex],"end")==0)
			break;

		s_listAddItemList.AddItem(0,g_strItemCategory[iIndex],iIndex);
		s_listAddItemList.IncreaseLineCounter();

		iIndex++;
	}

	s_listAddItemList.Select(0);

	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT	||	s_listAddItemList.m_iLineCounter	<=	0)
		EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);
	else
		EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),TRUE);
}

void
cMakeArca::Draw(HWND hDlg)
{
	hDlg		=	s_hWnd;

	int	iSelect	=	s_listDropItemList.m_iSelect;

	if (iSelect	!=	0xffff)
	{
		EnableWindow(GetDlgItem(hDlg,IDC_DROP_RATE),TRUE);
		EnableWindow(GetDlgItem(hDlg,IDC_ITEM_COUNT),TRUE);

		int	iValue	=	s_makeArca.m_aItem[iSelect].m_dwDroppingRate;

		SetDlgItemText(hDlg,IDC_DROP_RATE,_ms("%d.%.2d",iValue/100,iValue%100));
		SetDlgItemInt(hDlg,IDC_ITEM_COUNT,s_makeArca.m_aItem[iSelect].m_bf8Count,FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(hDlg,IDC_DROP_RATE),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_ITEM_COUNT),FALSE);
	}

//	if (s_clipArca.m_wSerial	==	0xffff)
//		EnableWindow(GetDlgItem(hDlg,IDC_PASTE),FALSE);
//	else
//		EnableWindow(GetDlgItem(hDlg,IDC_PASTE),TRUE);

	s_listDropItemList.Draw();
	s_listAddItemList.Draw();
}

void
cMakeArca::FillArcaInfo(HWND hDlg,BOOL _bIsClear)
{
	hDlg			=	s_hWnd;
	cArcaInfo	arca;

	if (_bIsClear	==	TRUE)
		s_makeArca.reset();
	
	arca.copy(&s_makeArca);

	SetDlgItemText(hDlg,IDC_NAME					,arca.m_strName);
	SetDlgItemInt(hDlg,IDC_REGEN_MINUTE				,arca.m_dwRegenPeriod/60,FALSE);
	SetDlgItemInt(hDlg,IDC_REGEN_SECOND				,arca.m_dwRegenPeriod%60,FALSE);
	SetDlgItemInt(hDlg,IDC_REGEN_CHANCE				,arca.m_wRegenChance,FALSE);
	SetDlgItemInt(hDlg,IDC_DURABILTY				,arca.m_iDurability,FALSE);
	SetDlgItemInt(hDlg,IDC_RESISTANCE				,arca.m_iResistance,FALSE);

	CheckDlgButton(hDlg,IDC_CAN_DESTROY				,arca.m_bf1IsDestroyAble);

	SetDlgItemInt(hDlg,IDC_DESTROY_ABLE_LEVEL		,arca.m_bf10DestroyAbleLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_SECRET_LEVEL				,arca.m_bSecretLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_LOCK_LEVEL				,arca.m_bLockLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DETECT_TRAP_LEVEL		,arca.m_wDetectTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DISARM_TRAP_LEVEL		,arca.m_wDisarmTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_TRAP_LEVEL				,arca.m_wTrapLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_TRAP_SKILL1_ACTIVE_CAHNCE,arca.m_wTrap1ActiveChance,FALSE);
	SetDlgItemInt(hDlg,IDC_TRAP_SKILL2_ACTIVE_CAHNCE,arca.m_wTrap2ActiveChance,FALSE);
	SetDlgItemInt(hDlg,IDC_DROP_GOLD				,arca.m_dwOwnGold,FALSE);
	SetDlgItemInt(hDlg,IDC_DROP_LEVEL				,arca.m_wItemDropLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_DROP_ITEM_COUNT			,arca.m_wItemCount,FALSE);

	UpdateGenItemList(hDlg,0);

	cCOMBOBOX::SelectByData(GetDlgItem(hDlg,IDC_TRAP_SKILL1),arca.m_wTrap1Skill);
	cCOMBOBOX::SelectByData(GetDlgItem(hDlg,IDC_TRAP_SKILL2),arca.m_wTrap2Skill);
	cCOMBOBOX::SelectByData(GetDlgItem(hDlg,IDC_TRAP_TARGET),arca.m_bf2TrapTarget);
}

//
//	아이템 리스트 업데이트
void
cMakeArca::UpdateGenItemList(HWND hDlg,int _iSelect)
{
	s_listDropItemList.ResetItems();

	for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
	{
		cItemInArca	*lpItem		=	&s_makeArca.m_aItem[i];

		if (lpItem->m_wItemType	==	0xffff)
			continue;

		if (lpItem->m_bf1IsItemPiece)
		{
			cDroppingItemForItemPiece	*lpDropInfo	=	(cDroppingItemForItemPiece	*)lpItem;
			cPieceItem					*lpPieceItem=	g_pieceItemManager.get(lpDropInfo->m_wPieceItem);

			if (!lpPieceItem)
				continue;

			s_listDropItemList.AddItem(0,"Item fragment",i);
			s_listDropItemList.AddItem(1,_ms("%s[%d]",lpPieceItem->m_strName,lpDropInfo->m_bf4ItemPiece+1));
		}
		else
		if (lpItem->m_bf1IsSetItem)
		{
			cDroppingItemForSetItem	*lpDropInfo	=	(cDroppingItemForSetItem	*)lpItem;
			cItemSet				*lpGroup	=	g_itemSetManager.get(lpDropInfo->wItemSet);

			if (!lpGroup)
				continue;

			cSetItem	*lpSetItem	=	lpGroup->getItem(lpDropInfo->m_bf4SetItemIndex);

			if (!lpSetItem)
				continue;

			s_listDropItemList.AddItem(0,"Set Item",i);
			s_listDropItemList.AddItem(1,_ms("%s:%s",lpGroup->m_strName,g_aBasicItem[lpSetItem->m_wBaseItem].m_strName),i);
		}
		else
		if (lpItem->m_bf1IsCustomItem)
		{
			CCustomItem	*lpCustomItem	=	g_customItem.getItem(lpItem->m_wItemType);
			if(!lpCustomItem)			// 09.08.17
				continue;
			s_listDropItemList.AddItem(0,"Custom Item",i);
			s_listDropItemList.AddItem(1,lpCustomItem->m_strName,i);
		}
		else
		{
			s_listDropItemList.AddItem(0,"Item type",i);
			s_listDropItemList.AddItem(1,g_strItemCategory[lpItem->m_wItemType],i);
		}

		int	iValue	=	lpItem->m_dwDroppingRate;

		s_listDropItemList.AddItem(2,_ms("%d.%.2d%",iValue/100,iValue%100));
		s_listDropItemList.AddItem(3,_ms("%d",lpItem->m_bf8Count));
		s_listDropItemList.IncreaseLineCounter();
	}

	s_listDropItemList.Select(_iSelect);

	if (s_listDropItemList.m_iSelect==	0xffff)	
		EnableWindow(GetDlgItem(hDlg,IDC_REMOVE_ITEM),FALSE);
	else
		EnableWindow(GetDlgItem(hDlg,IDC_REMOVE_ITEM),TRUE);
}

void
cMakeArca::RemoveGenItem(HWND hDlg)
{
	int	iItem		=	s_listDropItemList.GetSelectData();		//	현재 선택한 아이템

	for	(int	i=iItem;i<dMAX_ITEM_COUNT_IN_ARCA;i++)
		memcpy(&s_makeArca.m_aItem[i],&s_makeArca.m_aItem[i+1],sizeof(s_makeArca.m_aItem[i]));

	s_makeArca.m_aItem[dMAX_ITEM_COUNT_IN_ARCA-1].reset();

	int iSelect		=	s_listDropItemList.m_iSelect;

	if (iSelect	>=	s_listDropItemList.m_iLineCounter -1)
		iSelect--;

	UpdateGenItemList(hDlg,iSelect);

	if (s_listDropItemList.m_iLineCounter	<=	0)
		EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);
	if (s_listDropItemList.m_iLineCounter	<	dDROPPING_ITEM_COUNT)
		EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),TRUE);

	Draw(hDlg);
}

void
cMakeArca::AddItemPieceToGenItem(HWND hDlg)
{
	HWND	hPieceItem			=	GetDlgItem(hDlg,IDC_PIECE_ITEM);
	HWND	hItemPiece			=	GetDlgItem(hDlg,IDC_ITEM_PIECE);

	int		iPieceItem			=	cCOMBOBOX::GetData(hPieceItem);
	int		iItemPiece			=	cCOMBOBOX::GetData(hItemPiece);

	if (iPieceItem	==	-1	||	iItemPiece	==	-1)
		return;

	int		iSlot				=	-1;

	for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
	{
		cItemInArca	*lpItem	=	&s_makeArca.m_aItem[i];

		if (s_makeArca.m_aItem[i].m_wItemType	==	0xffff)
		{	
			iSlot	=	i;
			break;
		}
	}

	if (iSlot	==	-1)
		return;

	cDroppingItemForItemPiece	*lpSlot	=	(cDroppingItemForItemPiece	*)&s_makeArca.m_aItem[iSlot];

	lpSlot->m_wPieceItem		=	iPieceItem;
	lpSlot->m_bf4ItemPiece		=	iItemPiece;
	lpSlot->m_bf1IsItemPiece	=	TRUE;

	UpdateGenItemList(hDlg,iSlot);

	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT)
		EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);

	Draw(hDlg);
}

void
cMakeArca::AddSetItemToGenItem(HWND hDlg)
{
	HWND	hSetItemList		=	GetDlgItem(hDlg,IDC_SET_ITEM_LIST);
	HWND	hItemSetList	=	GetDlgItem(hDlg,IDC_SET_ITEM_GROUP_LIST);

	int		iSelectItem			=	cCOMBOBOX::GetData(hSetItemList);
	int		iSelectItemSet	=	cCOMBOBOX::GetData(hItemSetList);

	if (iSelectItem	==	-1	||	iSelectItemSet	==	-1)
		return;

	int		iSlot				=	-1;

	for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
	{
		cItemInArca	*lpItem	=	&s_makeArca.m_aItem[i];

		if (s_makeArca.m_aItem[i].m_wItemType	==	0xffff)
		{	
			iSlot	=	i;
			break;
		}
	}

	if (iSlot	==	-1)
		return;

	cDroppingItemForSetItem	*lpSlot	=	(cDroppingItemForSetItem	*)&s_makeArca.m_aItem[iSlot];

	lpSlot->wItemSet		=	iSelectItemSet;
	lpSlot->m_bf4SetItemIndex		=	iSelectItem;
	lpSlot->m_bf1IsSetItem			=	TRUE;

	UpdateGenItemList(hDlg,iSlot);

	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT)
		EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);

	Draw(hDlg);
}

void
cMakeArca::AddCustomItemKindToGenItem(HWND hDlg)
{
	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT	||	s_listAddItemList.m_iLineCounter	<=	0)
		return;

	int	iItem	=	-1,iCount	=	0;

	for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
	{
		cItemInArca	*lpItem	=	&s_makeArca.m_aItem[i];

		if (s_makeArca.m_aItem[i].m_wItemType	==	0xffff)
		{	
			iItem	=	i;
			break;
		}

		iCount++;
	}

	if (iItem	==	-1)
		return;

	HWND	hCombo		=	GetDlgItem(hDlg,IDC_CUSTOM_ITEM);
	int		iCustomItem	=	cCOMBOBOX::GetData(hCombo);

	if	(iCustomItem	==	-1)
		return;

	s_makeArca.m_aItem[iItem].reset();

	s_makeArca.m_aItem[iItem].m_wItemType		=	iCustomItem;
	s_makeArca.m_aItem[iItem].m_bf1IsCustomItem	=	TRUE;

	UpdateGenItemList(hDlg,iCount);

	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT)
		EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);

	Draw(hDlg);
}

void
cMakeArca::AddItemKindToGenItem(HWND hDlg)
{
	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT	||	s_listAddItemList.m_iLineCounter	<=	0)
		return;

	int	iItem	=	0,iCount	=	0;

	for (int i = 0; i<dDROPPING_ITEM_COUNT;i++)
	{
		cItemInArca	*lpItem	=	&s_makeArca.m_aItem[i];

		iItem		=	s_makeArca.m_aItem[i].m_wItemType;

		if (iItem	==	0xffff)
		{	
			iItem	=	i;
			break;
		}

		iCount++;
	}

	s_makeArca.m_aItem[iItem].reset();

	s_makeArca.m_aItem[iItem].m_wItemType		=	s_listAddItemList.GetSelectData();
	s_makeArca.m_aItem[iItem].m_bf1IsCustomItem	=	FALSE;

	UpdateGenItemList(hDlg,iCount);

	if (s_listDropItemList.m_iLineCounter	>=	dDROPPING_ITEM_COUNT)
		EnableWindow(GetDlgItem(hDlg,IDC_ADD_ITEM),FALSE);

	Draw(hDlg);
}

BOOL
cMakeArca::FinishWork(HWND hDlg)
{
	BOOL	temp;

	memcpy(s_makeArca.m_aItem,s_makeArca.m_aItem,sizeof(s_makeArca.m_aItem));

	GetDlgItemText(hDlg,IDC_NAME	,s_makeArca.m_strName,sizeof(s_makeArca.m_strName)-2);

	if (strlen(s_makeArca.m_strName)	<	2)
		return	ERRMSG("보물 상자 설정","에러!! 보물 상자 이름이 너무 짧습니다.");

	cArcaInfo	*lpArca					=	g_arcaManager.getByName(s_makeArca.m_strName);

	if (lpArca)
		if (s_makeArca.m_wSerial	!=	lpArca->m_wSerial)
			return	ERRMSG("보물 상자 설정","에러!! 이미 동일한 이름의 보물 상자가 있습니다.");

	int	iRegenMinute					=	GetDlgItemInt(hDlg,IDC_REGEN_MINUTE				,&temp,FALSE);
	int	iRegenSecond					=	GetDlgItemInt(hDlg,IDC_REGEN_SECOND				,&temp,FALSE);

	s_makeArca.m_dwRegenPeriod			=	iRegenMinute*60 + iRegenSecond;

	s_makeArca.m_wRegenChance			=	GetDlgItemInt(hDlg,IDC_REGEN_CHANCE				,&temp,FALSE);
	s_makeArca.m_iDurability			=	GetDlgItemInt(hDlg,IDC_DURABILTY				,&temp,FALSE);
	s_makeArca.m_iResistance			=	GetDlgItemInt(hDlg,IDC_RESISTANCE				,&temp,FALSE);

	s_makeArca.m_bf10DestroyAbleLevel	=	GetDlgItemInt(hDlg,IDC_DESTROY_ABLE_LEVEL		,&temp,FALSE);
	s_makeArca.m_bSecretLevel			=	GetDlgItemInt(hDlg,IDC_SECRET_LEVEL				,&temp,FALSE);
	s_makeArca.m_bLockLevel				=	GetDlgItemInt(hDlg,IDC_LOCK_LEVEL				,&temp,FALSE);
	s_makeArca.m_wDetectTrapLevel		=	GetDlgItemInt(hDlg,IDC_DETECT_TRAP_LEVEL		,&temp,FALSE);
	s_makeArca.m_wDisarmTrapLevel		=	GetDlgItemInt(hDlg,IDC_DISARM_TRAP_LEVEL		,&temp,FALSE);
	s_makeArca.m_wTrapLevel				=	GetDlgItemInt(hDlg,IDC_TRAP_LEVEL				,&temp,FALSE);
	s_makeArca.m_wTrap1ActiveChance		=	GetDlgItemInt(hDlg,IDC_TRAP_SKILL1_ACTIVE_CAHNCE,&temp,FALSE);
	s_makeArca.m_wTrap2ActiveChance		=	GetDlgItemInt(hDlg,IDC_TRAP_SKILL2_ACTIVE_CAHNCE,&temp,FALSE);
	s_makeArca.m_dwOwnGold				=	GetDlgItemInt(hDlg,IDC_DROP_GOLD				,&temp,FALSE);
	s_makeArca.m_wItemDropLevel			=	GetDlgItemInt(hDlg,IDC_DROP_LEVEL				,&temp,FALSE);
	s_makeArca.m_wItemCount				=	GetDlgItemInt(hDlg,IDC_DROP_ITEM_COUNT			,&temp,FALSE);
	s_makeArca.m_wTrap1Skill			=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_TRAP_SKILL1));
	s_makeArca.m_wTrap2Skill			=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_TRAP_SKILL2));
	s_makeArca.m_bf2TrapTarget			=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_TRAP_TARGET));

	s_makeArca.m_bf1IsDestroyAble		=	IsDlgButtonChecked(hDlg,IDC_CAN_DESTROY	);

	return	TRUE;
}

void
cMakeArca::ChangeItemSet(HWND hDlg,int _iGroup)
{
	if (s_iSelectItemSet	==	_iGroup)
		return;

	s_iSelectItemSet		=	_iGroup;

	cItemSet	*lpGroup	=	g_itemSetManager.get(s_iSelectItemSet);

	HWND	hSetItemList		=	GetDlgItem(hDlg,IDC_SET_ITEM_LIST);

	cCOMBOBOX::Reset(hSetItemList);

	if (lpGroup && lpGroup->getItemCount())
	{
		for (int i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
		{
			cSetItem	*lpItem	=	lpGroup->getItem(i);

			if (lpItem)
				cCOMBOBOX::Add(hSetItemList,g_aBasicItem[lpItem->m_wBaseItem].m_strName,i);
		}
	}
	else
		cCOMBOBOX::Add(hSetItemList,"valid",-1);

	cCOMBOBOX::Select(hSetItemList,0);
}

void
cMakeArca::ChangePieceItem(HWND hDlg,int _iPieceItem)
{
	if (s_iSelectPieceItem		==	_iPieceItem)
		return;

	s_iSelectPieceItem			=	_iPieceItem;

	cPieceItem	*lpPieceItem	=	g_pieceItemManager.get(s_iSelectPieceItem);
	HWND		hItemPiece		=	GetDlgItem(hDlg,IDC_ITEM_PIECE);

	cCOMBOBOX::Reset(hItemPiece);

	if (lpPieceItem)
	{
		for (int i=0;i<lpPieceItem->m_wPieceCount;i++)
			cCOMBOBOX::Add(hItemPiece,_ms("%.2d번째 조각",i+1),i);
	}
	else
		cCOMBOBOX::Add(hItemPiece,"valid",-1);

	cCOMBOBOX::Select(hItemPiece,0);
}

LRESULT CALLBACK
cMakeArca::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	
		case WM_INITDIALOG	:
			InitData(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			break;

		case WM_COMMAND		:
			switch( LOWORD( wParam ) )
            {
				case	IDOK		:
					if (FinishWork(hDlg))
						EndDialog(hDlg,IDOK);
					break;
				case	IDCANCEL	:
					EndDialog(hDlg,IDCANCEL);
					break;
				case	IDC_ADD_ITEM_PIECE	:
					AddItemPieceToGenItem(hDlg);
					break;
				case	IDC_ADD_SET_ITEM	:
					AddSetItemToGenItem(hDlg);
					break;
				case	IDC_ADD_CUSTOM_ITEM	:
					AddCustomItemKindToGenItem(hDlg);
					break;
				case	IDC_ADD_ITEM		:
					AddItemKindToGenItem(hDlg);
					break;
				case	IDC_REMOVE_ITEM		:
					RemoveGenItem(hDlg);
					break;

				case	IDC_SET_ITEM_GROUP_LIST	:
				{
					if (HIWORD(wParam)	!=	CBN_SELCHANGE)
						break;

					int	iGroup	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_SET_ITEM_GROUP_LIST));

					ChangeItemSet(hDlg,iGroup);
					break;
				}
				case	IDC_PIECE_ITEM			:
				{
					if (HIWORD(wParam)	!=	CBN_SELCHANGE)
						break;

					int	iPieceItem	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_PIECE_ITEM));

					ChangePieceItem(hDlg,iPieceItem);
					break;
				}
				case	IDC_ITEM_COUNT		:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							int		iIndex	=	s_listDropItemList.GetSelectData();

							BOOL	temp;
							DWORD	iValue	=	GetDlgItemInt(hDlg,IDC_ITEM_COUNT,&temp,FALSE);

							if (temp)
							{	
								s_makeArca.m_aItem[iIndex].m_bf8Count	=	(BYTE)min(iValue,0xff);
								s_listDropItemList.SetSelectItemStr(3,_ms("%d",s_makeArca.m_aItem[iIndex].m_bf8Count));
								s_listDropItemList.Draw();
							}
							break;
						}
					}
					break;
				}

				case	IDC_DROP_RATE		:
				{
					switch (HIWORD(wParam))
                    {
						case	EN_CHANGE	:
						{
							int	iIndex	=	s_listDropItemList.GetSelectData();

							char	strRate[20];
							int		iValue;
							GetDlgItemText(hDlg,IDC_DROP_RATE,strRate,sizeof(strRate));

							iValue	=	StringToNumber(strRate,100,2);

							if (iValue	==	0)
								iValue	=	1;

							s_makeArca.m_aItem[iIndex].m_dwDroppingRate	=	iValue;

							s_listDropItemList.SetSelectItemStr(2,_ms("%d.%.2d％",iValue/100,iValue%100));
							s_listDropItemList.Draw();
							break;
						}
					}
					break;
				}


				default		:
					break;
			}
			break;
	}

	return (0L);
}

LRESULT CALLBACK
cMakeArca::ProcDropItemList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			Draw(hDlg);
			break;
		}
	}

	return (0L);
}
