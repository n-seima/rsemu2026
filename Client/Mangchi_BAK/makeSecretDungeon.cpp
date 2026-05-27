#include "setSecretDungeon.h"
#include "resource.h"

cSecretDungeon		cMakeSecretDungeon::s_makeDungeon;
HWND				cMakeSecretDungeon::s_hWnd;
cLISTCONTROL		cMakeSecretDungeon::s_listFloor;	//	드롭 아이템 리스트
int					cMakeSecretDungeon::s_iSelectFloor;

BOOL
cMakeSecretDungeon::Open(HWND _hWnd,cSecretDungeon *_lpEdit)	//	시작
{
	if (_lpEdit)
		s_makeDungeon.copy(_lpEdit);
	else
		s_makeDungeon.reset();

	if (DialogBox( s_hInst, "SECRET_DUNGEON_MAKE",_hWnd,(DLGPROC)ProcDlg)==IDCANCEL)
		return	FALSE;

	return	TRUE;
}

void
cMakeSecretDungeon::InitData(HWND _hDlg)
{
	int		i;
	HWND	hFloorList,hFloorCount ,hSecretDungeonSerial;

	hFloorList		=	GetDlgItem(_hDlg,IDC_FLOOR_LIST);
	hFloorCount		=	GetDlgItem(_hDlg,IDC_FLOOR_COUNT);
	hSecretDungeonSerial	=	GetDlgItem(_hDlg,IDC_GUILD_DUNGEON_SERIAL);
	
	s_hWnd			=	_hDlg;

	{
		s_listFloor.Init		(hFloorList		,3		,FALSE	,NULL);		//	2개의 컬럼
		s_listFloor.AddColumn	("층"			,6*4	,eLCOM_CENTER);
		s_listFloor.AddColumn	("경험치 보정"	,6*13	,eLCOM_CENTER);
		s_listFloor.AddColumn	("몬스터 리젠"	,6*13	,eLCOM_CENTER);
	}

	{
		cCOMBOBOX::Reset(hFloorCount);

		for (i=1;i<=dMAX_FLOOR_COUNT_FOR_SECRET_DUNGEON;i++)
			cCOMBOBOX::Add(hFloorCount,_ms("%2d층",i),i);
		
		cCOMBOBOX::Reset(hSecretDungeonSerial);

		for (i=1;i<=dMAX_SERIAL_FOR_GUILD_DUNGEON;i++)
			cCOMBOBOX::Add(hSecretDungeonSerial,_ms("%2d번째 던젼",i),i);
	}

	FillInfo(_hDlg,0);

	Draw(_hDlg);

}

void
cMakeSecretDungeon::Draw(HWND hDlg)
{
	s_listFloor.Draw();
}

void
cMakeSecretDungeon::ChangeGuildDungeon(HWND hDlg,int _iCount)
{
	s_makeDungeon.m_bf5GuildDungenSerial	=	_iCount;
	
	//UpdateFloorList();
	Draw(s_hWnd);
}

void
cMakeSecretDungeon::ChangeFloorCount(HWND hDlg,int _iCount)
{
	s_makeDungeon.m_bf8FloorCount	=	_iCount;

	UpdateFloorList();
	Draw(s_hWnd);
}

BOOL
cMakeSecretDungeon::FinishWork(HWND _hDlg)
{
	BOOL	bIsTemp;

	GetDlgItemText(_hDlg,IDC_NAME,s_makeDungeon.m_strName,sizeof(s_makeDungeon.m_strName)-2);
	GetDlgItemText(_hDlg,IDC_ID_CODE,s_makeDungeon.m_strIDCode,sizeof(s_makeDungeon.m_strIDCode)-2);

	s_makeDungeon.m_wLimitCount			=	GetDlgItemInt(_hDlg,IDC_LIMIT_COUNT,&bIsTemp,FALSE);

	s_makeDungeon.m_bf1IsTestDungeon	=	IsDlgButtonChecked(_hDlg,IDC_IS_TEST);
	s_makeDungeon.m_bf1IsTowerOfOrdeal	=	IsDlgButtonChecked(_hDlg,IDC_IS_TOWER_OF_ORDEAL);
	s_makeDungeon.m_bf1IsGuildDungen	=	IsDlgButtonChecked(_hDlg,IDC_IS_GUILD_DUNGEN);

	int	iUpkeepHour					=	GetDlgItemInt(_hDlg,IDC_LIMIT_HOUR,&bIsTemp,FALSE);
	int	iUpkeepMinute				=	GetDlgItemInt(_hDlg,IDC_LIMIT_MINUTE,&bIsTemp,FALSE);
	int	iMinLevel					=	GetDlgItemInt(_hDlg,IDC_LIMIT_LOW_LEVEL,&bIsTemp,FALSE);
	int	iMaxLevel					=	GetDlgItemInt(_hDlg,IDC_LIMIT_HIGH_LEVEL,&bIsTemp,FALSE);

	s_makeDungeon.m_wUpkeepTime		=	iUpkeepHour*60+iUpkeepMinute;
	s_makeDungeon.m_wMinLevel		=	iMinLevel;
	s_makeDungeon.m_wMaxLevel		=	iMaxLevel;

	return	TRUE;
}

BOOL
cMakeSecretDungeon::EditFloor(HWND hDlg)
{
	int	iSelectFloor	=	s_listFloor.GetSelectData();

	if (iSelectFloor	==	0x7fffffff)
		return	FALSE;

	if (!cMakeFloor::Open(hDlg,&s_makeDungeon.m_aFloor[iSelectFloor]))
		return	FALSE;

	memcpy(&s_makeDungeon.m_aFloor[iSelectFloor],&cMakeFloor::s_makeFloor,sizeof(cSecretDungeonFloorDefine));

	UpdateFloorList();
	Draw(hDlg);

	return	TRUE;
}

void
cMakeSecretDungeon::UpdateFloorList()
{
	s_listFloor.ResetItems();

	for (int i = 0; i<s_makeDungeon.m_bf8FloorCount;i++)
	{
		cSecretDungeonFloorDefine	*lpFloor	=	&s_makeDungeon.m_aFloor[i];

		if (lpFloor	==	NULL)
			continue;

		s_listFloor.AddItem(0,_ms("%d층",i+1),i);
		s_listFloor.AddItem(1,_ms("%d％",lpFloor->m_wCorrectExperience));
		if (lpFloor->m_wIsRegenMonster)
			s_listFloor.AddItem(2,"o");
		else
			s_listFloor.AddItem(2,"x");

		s_listFloor.IncreaseLineCounter();
	}
}

void
cMakeSecretDungeon::FillInfo(HWND hDlg,BOOL _bIsClear)
{
	HWND	hFloorCount		=	GetDlgItem(hDlg,IDC_FLOOR_COUNT);
	HWND	hGuildDungeonSerial		=	GetDlgItem(hDlg,IDC_GUILD_DUNGEON_SERIAL);

	cCOMBOBOX::SelectByData(hFloorCount,s_makeDungeon.m_bf8FloorCount);
	cCOMBOBOX::SelectByData(hGuildDungeonSerial,s_makeDungeon.m_bf5GuildDungenSerial);

	SetDlgItemText(hDlg,IDC_ID_CODE,s_makeDungeon.m_strIDCode);
	SetDlgItemText(hDlg,IDC_NAME,s_makeDungeon.m_strName);
	SetDlgItemInt(hDlg,IDC_LIMIT_COUNT,s_makeDungeon.m_wLimitCount,FALSE);

	SetDlgItemInt(hDlg,IDC_LIMIT_HOUR	,s_makeDungeon.m_wUpkeepTime/60,FALSE);
	SetDlgItemInt(hDlg,IDC_LIMIT_MINUTE	,s_makeDungeon.m_wUpkeepTime%60,FALSE);

	SetDlgItemInt(hDlg,IDC_LIMIT_LOW_LEVEL	,s_makeDungeon.m_wMinLevel,FALSE);
	SetDlgItemInt(hDlg,IDC_LIMIT_HIGH_LEVEL	,s_makeDungeon.m_wMaxLevel,FALSE);

	CheckDlgButton(hDlg,IDC_IS_TEST,s_makeDungeon.m_bf1IsTestDungeon);
	CheckDlgButton(hDlg,IDC_IS_TOWER_OF_ORDEAL,s_makeDungeon.m_bf1IsTowerOfOrdeal);
	CheckDlgButton(hDlg,IDC_IS_GUILD_DUNGEN,s_makeDungeon.m_bf1IsGuildDungen);

	UpdateFloorList();
}

LRESULT CALLBACK
cMakeSecretDungeon::ProcDlg(HWND hDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
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
				case	IDOK			:
					if (FinishWork(hDlg))
						EndDialog(hDlg,IDOK);
					break;

				case	IDCANCEL		:
					EndDialog(hDlg,IDCANCEL);
					break;

				case	IDC_EDIT_FLOOR	:
					EditFloor(hDlg);
					break;

				case	IDC_FLOOR_COUNT		:
				{
					switch (HIWORD(wParam))
					{	
					case	CBN_SELCHANGE	:
						{
							int	iSetCount	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_FLOOR_COUNT));
							
							ChangeFloorCount(hDlg,iSetCount);
							Draw(hDlg);
							break;
						}
					}
					break;
				}

				case	IDC_GUILD_DUNGEON_SERIAL		:
					{
						switch (HIWORD(wParam))
						{	
						case	CBN_SELCHANGE	:
							{
								int	iSetCount	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_GUILD_DUNGEON_SERIAL));
								
								ChangeGuildDungeon(hDlg,iSetCount);
								Draw(hDlg);
								break;
							}
						}
						break;
				}


				default					:
					break;
			}
			break;
	}

	return (0L);
}