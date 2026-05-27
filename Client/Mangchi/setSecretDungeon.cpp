#include "setSecretDungeon.h"
#include "resource.h"
#define	 WM_MOUSEWHEEL	0x020A		/// 09.08.11
HWND					cSetSecretDungeon::s_hWnd;
cLISTCONTROL			cSetSecretDungeon::s_listDungeon;	//	던젼 리스트
cLISTCONTROL			cSetSecretDungeon::s_listFloor;	//	층별 정보 리스트

BOOL
cSetSecretDungeon::Open(HWND _hWnd)	//	시작
{
	if (DialogBox( s_hInst, "SECRET_DUNGEON",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cSetSecretDungeon::InitData(HWND _hDlg)
{
	HWND	hDungeonList,hFloorList;

	hDungeonList	=	GetDlgItem(_hDlg,IDC_ID_LIST);
	hFloorList		=	GetDlgItem(_hDlg,IDC_FLOOR_LIST);

	s_hWnd			=	_hDlg;

	//	바닥에 딸어지는 아이템 데이터/리스트
	{
		s_listDungeon.Init		(hDungeonList	,6		,FALSE	,ProcDungeonList);		//	2개의 컬럼
		s_listDungeon.AddColumn("Index"			,6*7	,eLCOM_CENTER);
		s_listDungeon.AddColumn("이름[ID Code]"	,6*58	,eLCOM_CENTER);
		s_listDungeon.AddColumn("생성제한"		,6*10	,eLCOM_CENTER);
		s_listDungeon.AddColumn("유지 시간"		,6*12	,eLCOM_CENTER);
		s_listDungeon.AddColumn("제한 레벨"		,6*12	,eLCOM_CENTER);
		s_listDungeon.AddColumn("테스트중"		,6*10	,eLCOM_CENTER);

		cSecretDungeon	*lpDungeon		=	g_sdManager.getFirst(1);

		if (lpDungeon)
			UpdateDungeonList(_hDlg,lpDungeon->m_wSerial);
		else
			UpdateDungeonList(_hDlg,0);
	}

	{
		s_listFloor.Init		(hFloorList		,3		,FALSE	,NULL);		//	2개의 컬럼
		s_listFloor.AddColumn	("층"			,6*7	,eLCOM_CENTER);
		s_listFloor.AddColumn	("경험치 보정"	,6*13	,eLCOM_CENTER);
		s_listFloor.AddColumn	("몬스터 리젠"	,6*13	,eLCOM_CENTER);
	}

	ChangeDungeon(_hDlg,s_listDungeon.GetSelectData());

	Draw(_hDlg);
}

void
cSetSecretDungeon::Draw(HWND hDlg)
{
	s_listDungeon.Draw();
	s_listFloor.Draw();
}

void
cSetSecretDungeon::AddDungeon(HWND hDlg)
{
	if (!cMakeSecretDungeon::Open(hDlg,NULL))
		return;

	int	iSerial	=	g_sdManager.add(&cMakeSecretDungeon::s_makeDungeon);

	if (iSerial	==	0xffff)
	{
		while(1)
		{
			if (!cMakeSecretDungeon::Open(hDlg,&cMakeSecretDungeon::s_makeDungeon))
				return;

			iSerial	=	g_sdManager.add(&cMakeSecretDungeon::s_makeDungeon);

			if (iSerial	!=	0xffff)
				break;
		}
	}

	UpdateDungeonList(hDlg,iSerial);
	ChangeDungeon(hDlg,iSerial);

	Draw(s_hWnd);
}

void
cSetSecretDungeon::EditDungeon(HWND hDlg)
{
	if (g_sdManager.getCount() == 0)
		return;

	int					iSelect		=	s_listDungeon.GetSelectData();

	cSecretDungeon	*lpDungeon	=	g_sdManager.get(iSelect);

	if(!lpDungeon)
		return;

	if (!cMakeSecretDungeon::Open(hDlg,lpDungeon))
		return;

	if (g_sdManager.replace(lpDungeon->m_wSerial,&cMakeSecretDungeon::s_makeDungeon)	==	FALSE)
	{
		while(1)
		{
			if (!cMakeSecretDungeon::Open(hDlg,&cMakeSecretDungeon::s_makeDungeon))
				return;

			if (g_sdManager.replace(lpDungeon->m_wSerial,&cMakeSecretDungeon::s_makeDungeon))
				break;
		}
	}

	UpdateDungeonList(hDlg,iSelect);
	ChangeDungeon(hDlg,iSelect);

	Draw(s_hWnd);
}

void
cSetSecretDungeon::RemoveDungeon(HWND hDlg)
{
	if (g_sdManager.getCount() == 0)
		return;

	int					iSelect		=	s_listDungeon.GetSelectData();
	cSecretDungeon	*lpDungeon	=	g_sdManager.get(iSelect);

	if(!lpDungeon)
		return;

	if (cMSG::YESNO("비밀 던젼 제거","선택한 비밀 던젼을 제거하시겠습니까?")	==	IDNO)
		return;

	g_sdManager.remove(iSelect);

	lpDungeon		=	g_sdManager.getFirst(iSelect);

	if (!lpDungeon)
		lpDungeon	=	g_sdManager.getFirst(1);

	if (lpDungeon)
	{
		UpdateDungeonList(hDlg,lpDungeon->m_wSerial);
		ChangeDungeon(hDlg,lpDungeon->m_wSerial);
	}
	else
	{
		UpdateDungeonList(hDlg,0);
		ChangeDungeon(hDlg,0);
	}

	Draw(s_hWnd);
}

void
cSetSecretDungeon::ChangeDungeon(HWND hDlg,int _iDungeon)
{
	hDlg	=	s_hWnd;
	
	cSecretDungeon	*lpDungeon	=	g_sdManager.get(_iDungeon);

	s_listFloor.ResetItems();

	if (!lpDungeon)
		return;

	for (int i = 1; i<=lpDungeon->m_bf8FloorCount;i++)
	{
		cSecretDungeonFloorDefine	*lpFloor	=	&lpDungeon->m_aFloor[i-1];

		if (lpFloor	==	NULL)
			continue;

		s_listFloor.AddItem(0,_ms("%d층",i),i);
		s_listFloor.AddItem(1,_ms("%d％",lpFloor->m_wCorrectExperience));
		if (lpFloor->m_wIsRegenMonster)
			s_listFloor.AddItem(2,"o");
		else
			s_listFloor.AddItem(2,"x");
		
		s_listFloor.IncreaseLineCounter();
	}
}

void
cSetSecretDungeon::UpdateDungeonList(HWND hDlg,int _iSelect)
{
	s_listDungeon.ResetItems();


	for (int i = 1; i<g_sdManager.getMaxCount();i++)
	{
		cSecretDungeon	*lpDungeon	=	g_sdManager.get(i);;

		if (lpDungeon	==	NULL)
			continue;

		s_listDungeon.AddItem(0,_ms("%.3d",i),i);
		s_listDungeon.AddItem(1,_ms("%s[%s]",lpDungeon->m_strName,lpDungeon->m_strIDCode));
		s_listDungeon.AddItem(2,_ms("%d 개",lpDungeon->m_wLimitCount));
		s_listDungeon.AddItem(3,_ms("%d시간 %d분",lpDungeon->m_wUpkeepTime/60,lpDungeon->m_wUpkeepTime%60));
		s_listDungeon.AddItem(4,_ms("%d~%d",lpDungeon->m_wMinLevel,lpDungeon->m_wMaxLevel));

		if	(lpDungeon->m_bf1IsTestDungeon)
			s_listDungeon.AddItem(5,"O");
		else
			s_listDungeon.AddItem(5,"X");

		s_listDungeon.IncreaseLineCounter();
	}

	s_listDungeon.selectByData(0,_iSelect);
}

LRESULT CALLBACK
cSetSecretDungeon::ProcDlg(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

				case	IDC_ADD				:
					AddDungeon(hDlg);
					break;

				case	IDC_EDIT			:
					EditDungeon(hDlg);
					break;

				case	IDC_REMOVE			:
					RemoveDungeon(hDlg);
					break;

				default		:
					break;
			}
			break;
	}

	return (0L);
}



LRESULT CALLBACK
cSetSecretDungeon::ProcDungeonList(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_LBUTTONDOWN		:
		{
			int	iSelect	=	s_listDungeon.GetSelectData();

			if (iSelect	==	0x7fffffff)
				break;

			ChangeDungeon(hDlg,iSelect);
			Draw(hDlg);
			SetFocus(hDlg);
			break;
		}
		case WM_MOUSEWHEEL :
			{
				
				int zDelta = ((short)HIWORD(wParam));
				
				
				if	(zDelta>=0)
				{
					s_listDungeon.VScroll(SB_LINEUP);
				}
				else{
					s_listDungeon.VScroll(SB_LINEDOWN);
				}
				
				Draw(hDlg);
				break;
			}
	}

	return (0L);
}