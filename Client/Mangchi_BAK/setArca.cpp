#include "setArca.h"
#include "resource.h"
#include "cDRAW.h"
#include "cTEXT.h"
#include "CItem_Effect.h"
#include "trapSkill.h"
#include "setItem.h"
#include "pieceItem.h"
#define WM_MOUSEWHEEL       0x020A	// 09.08.11 
cLISTCONTROL	cSetArca::s_listArca;	//	드롭 아이템 리스트
cLISTCONTROL	cSetArca::s_listDropItemList;	//	드롭 아이템 리스트
HWND			cSetArca::s_hWnd;
cArcaInfo		cSetArca::s_makeArca;
cArcaInfo		cSetArca::s_clipArca;

BOOL
cSetArca::Open(HWND _hWnd)
{
	if (DialogBox( s_hInst, "ARCA_LIST",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cSetArca::InitData(HWND hDlg)
{
	int		i;
	HWND	hDropItemList,hArcaList,hTrapTarget;
	HWND	hTrapSkillList1,hTrapSkillList2;

	s_hWnd			=	hDlg;

	hArcaList			=	GetDlgItem(hDlg,IDC_ARCA_LIST);			//	보물상자 리스트
	hDropItemList		=	GetDlgItem(hDlg,IDC_DROP_ITEM_LIST);	//	떨어뜨리는 아이템 리스트
	hTrapTarget			=	GetDlgItem(hDlg,IDC_TRAP_TARGET);		//	함정 타겟
	hTrapSkillList1		=	GetDlgItem(hDlg,IDC_TRAP_SKILL1);
	hTrapSkillList2		=	GetDlgItem(hDlg,IDC_TRAP_SKILL2);

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

	//	바닥에 딸어지는 아이템 데이터/리스트
	{
		s_listDropItemList.Init		(hDropItemList	,4		,FALSE	,ProcDropItemList);		//	2개의 컬럼
		s_listDropItemList.AddColumn("분류"			,6*16	);
		s_listDropItemList.AddColumn(dMSG_NAME		,6*28	);
		s_listDropItemList.AddColumn(dMSG_FREQUENCY	,6*14	);
		s_listDropItemList.AddColumn(dMSG_COUNT		,6*7	);
	}

	//	보물 상자 리스트
	{
		s_listArca.Init			(hArcaList			,2		,FALSE	,ProcArcaList);	//	2개의 컬럼
		s_listArca.AddColumn	("No"				,6*6	);
		s_listArca.AddColumn	(dMSG_NAME			,6*30	);
	}

	int			iSelect	=	0;
	cArcaInfo	*lpArca	=	g_arcaManager.getFirst();

	if (lpArca)
	{
		iSelect	=	lpArca->m_wSerial;
		s_makeArca.copy(lpArca);
	}

	UpdateArcaList(hDlg,iSelect);

	FillArcaInfo(hDlg,FALSE);

	Draw(hDlg);
}

void
cSetArca::Draw(HWND hDlg)
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

	if (s_listArca.m_iSelect==	0xffff)	
	{
		EnableWindow(GetDlgItem(hDlg,IDC_UPDATE),FALSE);
		EnableWindow(GetDlgItem(hDlg,IDC_REMOVE),FALSE);
//		EnableWindow(GetDlgItem(hDlg,IDC_COPY),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(hDlg,IDC_UPDATE),TRUE);
		EnableWindow(GetDlgItem(hDlg,IDC_REMOVE),TRUE);
//		EnableWindow(GetDlgItem(hDlg,IDC_COPY),TRUE);
	}

//	if (s_clipArca.m_wSerial	==	0xffff)
//		EnableWindow(GetDlgItem(hDlg,IDC_PASTE),FALSE);
//	else
//		EnableWindow(GetDlgItem(hDlg,IDC_PASTE),TRUE);

	s_listDropItemList.Draw();
	s_listArca.Draw();
}

BOOL
cSetArca::DecodeTextScriptFile(char *_lpstrFn)
{
	cDECODER	decoder;

	if (!decoder.Upload(_lpstrFn,NULL))
		return	FALSE;

	int	iOverlappedCount	=	0;

	g_arcaManager.reset();

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream(13);

		if (!strOneLine)
			break;

		int	iPoint	=	0;

		if (strOneLine[0]	==	10)
			iPoint++;

		if (DecodeTextScript(strOneLine+iPoint))
		{
			int	iSerial	=	g_arcaManager.add(&s_makeArca);
			
			if (iSerial	==	0xffff)
			{
				ERRMSG("보물 상자 이름이 중복 됩니다.","%s",s_makeArca.m_strName);
				iOverlappedCount++;
			}
		}
	}

	cMSG::Put("데이터 화일 분석 결과","%d개의 보물 상자를 추가 했습니다.\n\n%d개의 보물 상자 이름이 중복 되었습니다.",g_arcaManager.getCount(),iOverlappedCount);

	return	TRUE;
}

BOOL
cSetArca::DecodeTextScript(char *_lpstrText)
{
	cDECODER	decoder;

	if (strlen(_lpstrText)	<=	5)
		return	FALSE;

	s_makeArca.reset();

	decoder.UploadBuffer(_lpstrText,NULL);

	char	*lpStream;
	int		iValidCount		=	0;
	int		iGrade			=	0;
	int		iItemCount		=	0;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		iGrade++;

		if (!lpStream)
		{
			iValidCount++;
		
			if (iValidCount	>=	20)
				break;

			continue;
		}

		int	iIndex		=	iGrade;
		int	iItemBorder	=	20;

		if (iIndex		>=	iItemBorder)
		{
			cItemInArca	*lpItem		=	&s_makeArca.m_aItem[iItemCount];

			lpItem->m_bf1IsItemPiece=	FALSE;

			if (((iIndex-iItemBorder)	%	3)	==	0)
			{
				BOOL	bIsCustomItem	=	decoder.isNumber(lpStream);
				
				if (bIsCustomItem)
				{
					lpItem->m_bf1IsCustomItem	=	TRUE;
					lpItem->m_wItemType			=	atoi(lpStream);
				}
				else
				{
					lpItem->m_bf1IsCustomItem	=	FALSE;
					lpItem->m_wItemType			=	cBASIC_ITEM::GetItemKind(lpStream);

					if (lpItem->m_wItemType	==	0xffff)
						return	ERRMSG("error","[%s] 타입의 아이템은 없다!",lpStream);
				}
			}
			if (((iIndex-iItemBorder)	%	3)	==	1)
			{
				lpItem->m_dwDroppingRate	=	StringToNumber(lpStream,100,2);
			}
			if (((iIndex-iItemBorder)	%	3)	==	2)
			{
				lpItem->m_bf8Count			=	atoi(lpStream);

				iItemCount++;
			}
		}
		else
		switch(iIndex)
		{
			case	1	:
			{
				strcpy(s_makeArca.m_strName,lpStream);
				break;
			}
			case	2	:
			{
				cDECODER	dec;

				dec.UploadBuffer(lpStream,NULL);

				int	iHour	=	dec.GetNumber();
				int	iMinute	=	dec.GetNumber();

				s_makeArca.m_dwRegenPeriod	=	iHour*60+iMinute;
				break;
			}

			case	3	:
			{
				s_makeArca.m_wRegenChance	=	atoi(lpStream);
				
				break;
			}

			case	4	:
			{
				s_makeArca.m_iDurability	=	atoi(lpStream);
				
				break;
			}
			case	5	:
			{
				s_makeArca.m_iResistance	=	atoi(lpStream);
				
				break;
			}
			case	6	:
			{
				s_makeArca.m_bf10DestroyAbleLevel	=	atoi(lpStream);
				
				break;
			}
			case	7	:
			{
				s_makeArca.m_bf1IsDestroyAble		=	atoi(lpStream);
				
				break;
			}
			case	8	:
			{
				s_makeArca.m_bSecretLevel			=	atoi(lpStream);
				
				break;
			}
			case	9	:
			{
				s_makeArca.m_bLockLevel				=	atoi(lpStream);
				
				break;
			}
			case	10	:
			{
				s_makeArca.m_wDetectTrapLevel		=	atoi(lpStream);
				
				break;
			}
			case	11	:
			{
				s_makeArca.m_wDisarmTrapLevel		=	atoi(lpStream);
				
				break;
			}
			case	12	:
			{
				s_makeArca.m_wTrapLevel				=	atoi(lpStream);
				
				break;
			}
			case	13	:
			{
				if (stricmp(lpStream,"타겟")==0)
					s_makeArca.m_bf2TrapTarget		=	eTT_TARGET;
				else
					s_makeArca.m_bf2TrapTarget		=	eTT_RANGE;
				
				break;
			}
			case	14	:
			{
				cTrapSkill	*lpSkill	=	g_trapSkillManager.get(lpStream);
				if (!lpSkill)
					return	ERRMSG("에러!!","'%s' 라는 이름의 함정은 없어!!",lpStream);

				s_makeArca.m_wTrap1Skill	=	lpSkill->m_wSerial;
				
				break;
			}
			case	15	:
			{
				s_makeArca.m_wTrap1ActiveChance		=	atoi(lpStream);
				
				break;
			}
			case	16	:
			{
				cTrapSkill	*lpSkill	=	g_trapSkillManager.get(lpStream);
				if (!lpSkill)
					return	ERRMSG("에러!!","'%s' 라는 이름의 함정은 없어!!",lpStream);

				s_makeArca.m_wTrap2Skill	=	lpSkill->m_wSerial;
				
				break;
			}
			case	17	:
			{
				s_makeArca.m_wTrap2ActiveChance		=	atoi(lpStream);
				
				break;
			}
			case	18	:
			{
				s_makeArca.m_dwOwnGold				=	atoi(lpStream);
				
				break;
			}
			case	19	:
			{
				s_makeArca.m_wItemDropLevel			=	atoi(lpStream);
				
				break;
			}
		}
	}

	return	TRUE;
}

void
cSetArca::Loading(HWND hDlg)
{
	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	if (!cFILE::LoadBox(strFn,"텍스트 화일 {*.txt}\0*.txt\0\0","txt",NULL,"보물상자 스크립트 데이터 파일 불러오기"))
	{
		SetLogFolder();
		return;
	}

	DecodeTextScriptFile(strFn);

	UpdateArcaList(hDlg,0);

	FillArcaInfo(hDlg,TRUE);

	Draw(hDlg);

	SetLogFolder();
}

void
cSetArca::FillArcaInfo(HWND hDlg,BOOL _bIsClear)
{
	hDlg			=	s_hWnd;
	cArcaInfo	arca;

	if (_bIsClear	==	TRUE)
		s_makeArca.reset();
	
	arca.copy(&s_makeArca);

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
//	보물 상자 리스트 업데이트
void
cSetArca::UpdateArcaList(HWND hDlg,int _iSelect)
{
	int	iCount	=	0;

	s_listArca.ResetItems();

	for (int i = 0;i < g_arcaManager.getMaxCount() &&  iCount<g_arcaManager.getCount();i++)
	{
		cArcaInfo	*lpArca	=	g_arcaManager.get(i);

		if (!lpArca)
			continue;

		iCount++;

		s_listArca.AddItem(0,_ms("%d",i),i);
		s_listArca.AddItem(1,lpArca->m_strName);
		s_listArca.IncreaseLineCounter();
	}

	s_listArca.selectByData(0,_iSelect);
}

//
//	아이템 리스트 업데이트
void
cSetArca::UpdateGenItemList(HWND hDlg,int _iSelect)
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

			s_listDropItemList.AddItem(0,"아이템 조각",i);
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

			s_listDropItemList.AddItem(0,"세트 아이템",i);
			s_listDropItemList.AddItem(1,_ms("%s:%s",lpGroup->m_strName,g_aBasicItem[lpSetItem->m_wBaseItem].m_strName),i);
		}
		else
		if (lpItem->m_bf1IsCustomItem)
		{
			CCustomItem	*lpCustomItem	=	g_customItem.getItem(lpItem->m_wItemType);
			if(!lpCustomItem)			// 09.08.17
				continue;
			s_listDropItemList.AddItem(0,"커스텀 아이템",i);
			s_listDropItemList.AddItem(1,lpCustomItem->m_strName,i);
		}
		else
		{
			s_listDropItemList.AddItem(0,"아이템 종류",i);
			s_listDropItemList.AddItem(1,g_strItemCategory[lpItem->m_wItemType],i);
		}

		int	iValue	=	lpItem->m_dwDroppingRate;

		s_listDropItemList.AddItem(2,_ms("%d.%.2d％",iValue/100,iValue%100));
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
cSetArca::AddArca(HWND hDlg)
{
	if (!cMakeArca::Open(hDlg,NULL))
		return;

	int	iSerial	=	g_arcaManager.add(&cMakeArca::s_makeArca);

	UpdateArcaList(hDlg,iSerial);
}

void
cSetArca::EditArca(HWND hDlg)
{
	if (g_arcaManager.getCount() <= 0)
		return;

	int	iSelect	=	s_listArca.GetSelectData();

	if (iSelect	==	0x7fffffff)
		return;

	cArcaInfo	*lpArca	=	g_arcaManager.get(iSelect);

	if (!cMakeArca::Open(hDlg,lpArca))
		return;

	g_arcaManager.replace(lpArca->m_wSerial,&cMakeArca::s_makeArca);

	UpdateArcaList(hDlg,lpArca->m_wSerial);
}

void
cSetArca::RemoveArca(HWND hDlg)
{
	int	iSelect	=	s_listArca.GetSelectData();

	if (iSelect	==	0x7fffffff)
		return;

	if (cMSG::YESNO("선택한 보물 상자 정보를 지웁니다","정말로 지우시겠습니까?")==IDNO)
		return;

	g_arcaManager.remove(iSelect);

	cArcaInfo	*lpArca	=	g_arcaManager.getFirst(iSelect);

	if (!lpArca)
		lpArca	=	g_arcaManager.getFirst(0);

	if (lpArca)
	{
		s_makeArca.copy(lpArca);
		iSelect	=	lpArca->m_wSerial;
		FillArcaInfo(hDlg,FALSE);
	}

	UpdateArcaList(hDlg,iSelect);
	Draw(hDlg);
}

void
cSetArca::CopyArca(HWND hDlg)
{
	int	iSelect	=	s_listArca.GetSelectData();

	if (iSelect	!=	0x7fffffff)
	{
		cArcaInfo	*lpArca	=	g_arcaManager.get(iSelect);

		s_clipArca.copy(lpArca);

		Draw(hDlg);
	}
}

void
cSetArca::FilterArca(HWND _hDlg)		// 09.08.17
{

		
	char strFilter[256] = "";
	GetDlgItemText(_hDlg,IDC_EDIT_ARCA_FILTER,strFilter,256);
	int iFilterNameLen	= strlen(strFilter);
	if(iFilterNameLen<=0)
	{
		InitData(_hDlg);
		return;

	}

	int	iCount	=	0;

	s_listArca.ResetItems();
	
	for (int i = 0;i < g_arcaManager.getMaxCount() && iCount<g_arcaManager.getCount();++i)
	{
		
		cArcaInfo	*lpArca	=	g_arcaManager.get(i);
		
		if (!lpArca)
			continue;
		++iCount;
		int iItemNameLen	= strlen(lpArca->m_strName);
		int iLenGap			= iItemNameLen - iFilterNameLen;
		if(iLenGap<0)
			continue;
		
		for(int iName=0;iName<=iLenGap;++iName)
		{
			if(strnicmp(strFilter,&lpArca->m_strName[iName], iFilterNameLen) == 0 )
			{
				s_listArca.AddItem(0,_ms("%d",i),i);
				s_listArca.AddItem(1,lpArca->m_strName);
				s_listArca.IncreaseLineCounter();
			}
		}

	}
	s_listArca.Select(0);
	Draw(_hDlg);

}

LRESULT CALLBACK
cSetArca::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)		// 09.08.17
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
				case	IDCANCEL	:
					EndDialog(hDlg,IDOK);
					break;

				case	IDC_LOAD_TEXT		:
					Loading(hDlg);
					break;

				case	IDC_ADD				:
					AddArca(hDlg);
					break;

				case	IDC_EDIT			:
					EditArca(hDlg);
					break;

//				case	IDC_PASTE			:
//					PasteArca(hDlg);
//					break;
				case	IDC_REMOVE			:
					RemoveArca(hDlg);
					break;
//				case	IDC_COPY			:
//					CopyArca(hDlg);
//					break;

				case IDC_EDIT_ARCA_FILTER	:
					{
						if	(HIWORD(wParam) != EN_CHANGE)
							break;
						FilterArca(hDlg);
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
cSetArca::ProcDropItemList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)		// 09.08.11
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			Draw(hDlg);
			SetFocus(hDlg);
			break;
		}

	}

	return (0L);
}

LRESULT CALLBACK
cSetArca::ProcArcaList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)	// 09.08.11
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSelect	=	s_listArca.GetSelectData();

			if (iSelect	==	0x7fffffff)
				break;

			s_makeArca.copy(g_arcaManager.get(iSelect));
			FillArcaInfo(hDlg,FALSE);
			Draw(hDlg);
			SetFocus(hDlg);
			break;
		}
		case WM_MOUSEWHEEL :
			{
				
				int zDelta = ((short)HIWORD(wParam));
				
				
				if	(zDelta>=0)
				{
					s_listArca.VScroll(SB_LINEUP);
				}
				else{
					s_listArca.VScroll(SB_LINEDOWN);
				}
				
			Draw(hDlg);
				break;
			}
	}

	return (0L);
}