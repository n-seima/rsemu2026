#include "resource.H"
#include "cSET.H"
#include "cMAPWND.H"
#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cPROJECT.H"
#include "CSoundEffect.H"
#include "secretDungeon.H"

/////////////////////////////////////////////////////////////////////////////////////////////////
//	새로운 맵 설정
/////////////////////////////////////////////////////////////////////////////////////////////////
cMAP_INFO	cNEWMAP::_MapInfo;
char		cNEWMAP::_strFieldName[512];
char		cNEWMAP::_strOverlapName[512];

BOOL
cNEWMAP::Init(HWND hwnd)
{
	int		i,index=cMAP::GetValidMap();

	if (!_isAddMap)	index	=	g_iSelectMap;

	SetDlgItemText	(hwnd,IDC_NAME,_ms("Noname Map %.2d",index));
	SetDlgItemInt	(hwnd,IDC_MAP_WIDTH	,100,FALSE);
	SetDlgItemInt	(hwnd,IDC_MAP_HEIGHT,100,FALSE);
	SetDlgItemInt	(hwnd,IDC_LIMIT_REGEN_MONSTER_COUNT,256,FALSE);

	i	=	0;

	while(1)
	{
		if (strcmp(g_strTileSet[i*2],"end")	==	0)	break;

		cCOMBOBOX::Add(GetDlgItem(hwnd,IDC_TILESET),g_strTileSet[i*2+1]);

		i++;
	}

	cCOMBOBOX::Select(GetDlgItem(hwnd,IDC_TILESET),0);

	return TRUE;
}


BOOL
cNEWMAP::Setting(HWND hwnd)
{
	BOOL	isResult;

	GetDlgItemText	(hwnd,IDC_NAME			,_MapInfo.m_strName,sizeof(_MapInfo.m_strName));

	char	*lpDestMapFN	=	cPROJECT::GetFile(_MapInfo.m_strName);

	if (lpDestMapFN)
		return	ERRMSG(dMSG_ERROR_ADD_NEW_MAP,dMSG_ALREADY_EXIST_SAME_TILE);

	_MapInfo.m_iTileSet					=	cCOMBOBOX::GetSelect(GetDlgItem(hwnd,IDC_TILESET));
	_MapInfo.m_iWidth					=	GetDlgItemInt(hwnd,IDC_MAP_WIDTH,&isResult,FALSE);
	_MapInfo.m_iHeight					=	GetDlgItemInt(hwnd,IDC_MAP_HEIGHT,&isResult,FALSE);

	cCHARACTER_SET		tempCharacterSet;

	if (IsDlgButtonChecked(hwnd,IDC_COPY_CHARACTER_SET))
		memcpy(&tempCharacterSet,&g_lpActmap->m_setCharacter,sizeof(cCHARACTER_SET));

	if (_isAddMap)	isResult	=	cMAP::Add(&_MapInfo);
	else			isResult	=	cMAP::Reset(&_MapInfo,g_iSelectMap);

	if (isResult	==	TRUE)
		if (IsDlgButtonChecked(hwnd,IDC_COPY_CHARACTER_SET))
			memcpy(&g_lpActmap->m_setCharacter,&tempCharacterSet,sizeof(cCHARACTER_SET));


	return isResult;
}

LRESULT CALLBACK 
cNEWMAP::WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG	:
			cNEWMAP::Init(hWnd);
			break;

		case WM_COMMAND :
			switch(LOWORD(wParam))
			{	case	IDOK				:
					if (Setting(hWnd))	EndDialog(hWnd,IDOK);
					break;

				case	IDCANCEL			:
					EndDialog(hWnd,IDCANCEL);
					break;

				default	:
					break;
			}
			break;
			
		
		default :	return (FALSE);
	}
	return (TRUE);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//	기존의 맵 변경 - 현재는 이름이랑 사이즈 바꾸는 수준... 그나마 통맵은 사이즈도 못 바꾼다.
/////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cSETMAP::Init(HWND hwnd)
{
	SetDlgItemText	(hwnd,IDC_NAME						,g_lpActmap->m_strName);
	SetDlgItemInt	(hwnd,IDC_MAP_WIDTH					,g_lpActmap->m_iWidth ,FALSE);
	SetDlgItemInt	(hwnd,IDC_MAP_HEIGHT				,g_lpActmap->m_iHeight,FALSE);
	SetDlgItemInt	(hwnd,IDC_X							,0,FALSE);
	SetDlgItemInt	(hwnd,IDC_Y							,0,FALSE);
	SetDlgItemInt	(hwnd,IDC_CORRECT_FIRE_RESISTANCE	,g_lpActmap->m_sCorrectFireResistance	,TRUE);
	SetDlgItemInt	(hwnd,IDC_CORRECT_WATER_RESISTANCE	,g_lpActmap->m_sCorrectWaterResistance	,TRUE);
	SetDlgItemInt	(hwnd,IDC_CORRECT_WIND_RESISTANCE	,g_lpActmap->m_sCorrectWindResistance	,TRUE);
	SetDlgItemInt	(hwnd,IDC_CORRECT_EARTH_RESISTANCE	,g_lpActmap->m_sCorrectEarthResistance	,TRUE);
	SetDlgItemInt	(hwnd,IDC_CORRECT_LIGHT_RESISTANCE	,g_lpActmap->m_sCorrectLightResistance	,TRUE);
	SetDlgItemInt	(hwnd,IDC_CORRECT_DARK_RESISTANCE	,g_lpActmap->m_sCorrectDarkResistance	,TRUE);

    SetDlgItemInt(hwnd,IDC_BAD_STATUS_REGISTANCE	,g_lpActmap->m_sCorrectMonsterBadStatusResistance		,TRUE	);
    SetDlgItemInt(hwnd,IDC_DECLINE_POWER_REGISTANCE	,g_lpActmap->m_sCorrectMonsterDeclinePowerResistance	,TRUE	);
	SetDlgItemInt(hwnd,IDC_CURSE_REGISTANCE			,g_lpActmap->m_sCorrectMonsterCurseResistance			,TRUE	);

    SetDlgItemInt(hwnd,IDC_BLIND					,g_lpActmap->m_sCorrectMonsterBlindResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_POISON					,g_lpActmap->m_sCorrectMonsterPoisonResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_SLEEP					,g_lpActmap->m_sCorrectMonsterSleepResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_COLD						,g_lpActmap->m_sCorrectMonsterColdResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_FREEZE					,g_lpActmap->m_sCorrectMonsterFreezeResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_STUN						,g_lpActmap->m_sCorrectMonsterStunResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_STONE					,g_lpActmap->m_sCorrectMonsterStoneResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_CONFUSE					,g_lpActmap->m_sCorrectMonsterConfuseResistance		,TRUE	);
    SetDlgItemInt(hwnd,IDC_CHARMING					,g_lpActmap->m_sCorrectMonsterCharmingResistance		,TRUE	);
    SetDlgItemInt(hwnd,IDC_CRITICAL_RESISTANCE		,g_lpActmap->m_sCorrectMonsterCriticalResistance		,TRUE	);
    SetDlgItemInt(hwnd,IDC_CRUSH_RESISTANCE			,g_lpActmap->m_sCorrectMonsterCrushResistance			,TRUE	);

    SetDlgItemInt(hwnd,IDC_FIRE_REGISTANCE			,g_lpActmap->m_sCorrectMonsterFireResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_WATER_REGISTANCE			,g_lpActmap->m_sCorrectMonsterWaterResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_WIND_REGISTANCE			,g_lpActmap->m_sCorrectMonsterWindResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_LIGHT_REGISTANCE			,g_lpActmap->m_sCorrectMonsterLightResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_EARTH_REGISTANCE			,g_lpActmap->m_sCorrectMonsterEarthResistance			,TRUE	);
    SetDlgItemInt(hwnd,IDC_DARK_REGISTANCE			,g_lpActmap->m_sCorrectMonsterDarkResistance			,TRUE	);

	if (g_lpActmap->m_bf1IsPremiumZone)
		CheckDlgButton(hwnd,IDC_IS_PREMIUM_ZONE			,TRUE);

	if (g_lpActmap->m_bf1IsGuildBattleZone)	
		CheckDlgButton(hwnd,IDC_IS_GUILD_BATTLE_ZONE	,TRUE);

	if (g_lpActmap->m_bf1IsGuildBattleLobby)
		CheckDlgButton(hwnd,IDC_IS_GUILD_BATTLE_LOBBY	,TRUE);

	if (g_lpActmap->m_bf1IsEventField)		
		CheckDlgButton(hwnd,IDC_IS_EVENT_FIELD			,TRUE);

	if (g_lpActmap->m_bf1IsOXQuizGateVillage)
		CheckDlgButton(hwnd,IDC_IS_OX_QUIZ_VILLAGE		,TRUE);

	if (g_lpActmap->m_bf1IsLocked)			
		CheckDlgButton(hwnd,IDC_IS_LOCKED_MAP			,TRUE);

	if (g_lpActmap->m_bf1IsCanNotMemoryZone)
		CheckDlgButton(hwnd,IDC_DISABLE_ORB_MEMORY		,TRUE);

	if (g_lpActmap->m_bf1IsExistSecretDungeonGate)
		CheckDlgButton(hwnd,IDC_SECRET_DUNGEON_GATE		,TRUE);

	if (g_lpActmap->m_bf1IsHalfSize)		
		CheckDlgButton(hwnd,IDC_HALF_SIZE				,TRUE);

	if (g_lpActmap->m_bf1IsBossZone)	
		CheckDlgButton(hwnd,IDC_BOSS_ZONE				,TRUE);

	if (g_lpActmap->m_bf1IsGuildHall)	
		CheckDlgButton(hwnd,IDC_IS_GUILD_HALL			,TRUE);

	if (g_lpActmap->m_bf1Is1LevelGuildHall)	
		CheckDlgButton(hwnd,IDC_IS_1LEVEL_GUILD_HALL	,TRUE);
	
	
	if (g_lpActmap->m_bf1IsWordQuizField)
		CheckDlgButton(hwnd,IDC_WORD_QUIZ_MAP			,TRUE);

	if (g_lpActmap->m_bf1IsWordQuizVillage)	
		CheckDlgButton(hwnd,IDC_WORD_QUIZ_VILLAGE		,TRUE);

	if (g_lpActmap->m_bf1IsBlockToWarpField)
		CheckDlgButton(hwnd,IDC_BLOCK_TO_WARP			,TRUE);

	if (g_lpActmap->m_bf1IsGuildPointBattleField)
		CheckDlgButton(hwnd,IDC_GUILD_POINT_BATTLE	,TRUE);

	if (g_lpActmap->m_bf1IsSiegeWarefareField)
		CheckDlgButton(hwnd,IDC_GUILD_SIEGE_WAREFARE	,TRUE);

	if (g_lpActmap->m_bf1IsCanNotCallCarpetField)
		CheckDlgButton(hwnd,IDC_IS_CAN_NOT_CALL_CARPET_FIELD,TRUE);

	if (g_lpActmap->m_bf1IsCanNotUseCallingSkill)
		CheckDlgButton(hwnd,IDC_BLOCK_TO_USE_CALLING,TRUE);

	if (g_lpActmap->m_bf1IsUseFindWay)	
		CheckDlgButton(hwnd,IDC_IS_FIND_WAY,TRUE);

	if (g_lpActmap->m_bf1IsSetBossMap)	
		CheckDlgButton(hwnd,IDC_IS_SET_BOSS_MAP,TRUE);

	if (g_lpActmap->m_bf1IsGuildDungeonMap)
		CheckDlgButton(hwnd,IDC_IS_GUILD_DUNGEON_MAP,TRUE);

	if (g_lpActmap->m_bf1IsNotUseSkillBeforeTrans)
		CheckDlgButton(hwnd,IDC_IS_NOT_USE_SKILL_BEFORE_TRANS,TRUE);
	
	if (g_lpActmap->m_bf1IsNotOpenPitchManShop)
		CheckDlgButton(hwnd,IDC_IS_NOT_OPEN_PITCHMAN_SHOP,TRUE);

	if (g_lpActmap->m_bf1IsNight)
		CheckDlgButton(hwnd,IDC_IS_NIGHT,TRUE);

	{
		HWND	hLinkSecretDungeon	=	GetDlgItem(hwnd,IDC_LINK_SECRET_DUNGEON);

		cCOMBOBOX::Reset(hLinkSecretDungeon);

		cCOMBOBOX::Add(hLinkSecretDungeon,"없음",0);

		for (int i=1;i<g_sdManager.getMaxCount();i++)
		{
			cSecretDungeon	*lpDungeon	=	g_sdManager.get(i);

			if (!lpDungeon)
				continue;

			cCOMBOBOX::Add(hLinkSecretDungeon,lpDungeon->m_strName,i);
		}

		if (cCOMBOBOX::SelectByData(hLinkSecretDungeon,g_lpActmap->m_wLinkSecretDungeon) == CB_ERR)
			cCOMBOBOX::Select(hLinkSecretDungeon,0);

	}
	
//	if (g_lpActmap->m_bf1IsHiddenPortal)		CheckDlgButton(hwnd,IDC_IS_HIDDEN_PORTAL		,TRUE);

	int	i	=	0;

	while(1)
	{
		if (strcmp(g_aBgm[i].m_strFileName,"end")	==	0)
			break;

		cCOMBOBOX::Add(GetDlgItem(hwnd,IDC_BGM_LIST),g_aBgm[i].m_strTitle,i);

		i++;
	}

	cCOMBOBOX::Select(GetDlgItem(hwnd,IDC_BGM_LIST),0);

	{
		for (i=0;;i++)
		{
			if (stricmp(g_strMapType[i],"end") == 0) break;

			cCOMBOBOX::Add(GetDlgItem(hwnd,IDC_FIELD_TYPE_LIST),g_strMapType[i],i);
		}

		cCOMBOBOX::Select(GetDlgItem(hwnd,IDC_FIELD_TYPE_LIST),g_lpActmap->m_bf4FieldType);
	}

	{
		HWND	hMinimapDisplayLevelList	=	GetDlgItem(hwnd,IDC_MINIMAP_DISPLAY_LEVEL);

		for (i=0;i<10;i++)
			cCOMBOBOX::Add(hMinimapDisplayLevelList,_ms("Level %d",i),i);

		cCOMBOBOX::Select(hMinimapDisplayLevelList,g_lpActmap->m_bf4MinimapDisplayLevel);
	}

	HWND	hBgmList	=	GetDlgItem(hwnd,IDC_BGM);

	for (i=0;;i++)
	{
		int	iBgm	=	g_lpActmap->m_aBgmList[i];

		if (iBgm == 0xffff)	break;

		cLISTBOX::Add(hBgmList,g_aBgm[iBgm].m_strTitle,iBgm);
	}
	// 날씨 10.02.10 추가
	{	
		HWND	hWeatherType	=	GetDlgItem(hwnd,IDC_FIELD_WEATHER);
		cCOMBOBOX::Reset(hWeatherType);
		
		for (i=0;;i++)
		{
			if (stricmp(g_strWeatherType[i],"end") == 0) break;
			cCOMBOBOX::Add(hWeatherType,g_strWeatherType[i],i);
		}
		
		if (cCOMBOBOX::SelectByData(hWeatherType,g_lpActmap->m_bf3WeatherType) == CB_ERR)
			cCOMBOBOX::Select(hWeatherType,0);
	}
	return TRUE;
}



BOOL
cSETMAP::Setting(HWND hwnd)
{
	BOOL	isTemp;
	int		iWidth,iHeight,iLimitRegenMonsterCount;
	int		iX, iY;
	char	strName[dMAP_NAME_LENGTH];
	char	*lpMapFN,*lpDestMapFN;

	lpMapFN		=	g_lpActmap->m_strFName;

	GetDlgItemText	(hwnd,IDC_NAME	,strName	,sizeof(strName));

	lpDestMapFN	=	cPROJECT::GetFile(strName);

	if (lpDestMapFN)
		if (stricmp(lpMapFN,lpDestMapFN)	!=	0)
			return	ERRMSG(dMSG_ERROR_CHANGE_MAP_SETTING,dMSG_ALREADY_EXIST_SAME_TILE);

	iWidth					=	GetDlgItemInt(hwnd,IDC_MAP_WIDTH				,&isTemp,FALSE);
	iHeight					=	GetDlgItemInt(hwnd,IDC_MAP_HEIGHT				,&isTemp,FALSE);
	iX						=	GetDlgItemInt(hwnd,IDC_X				,&isTemp,FALSE);
	iY						=	GetDlgItemInt(hwnd,IDC_Y				,&isTemp,FALSE);

	iLimitRegenMonsterCount	=	GetDlgItemInt(hwnd,IDC_LIMIT_REGEN_MONSTER_COUNT,&isTemp,FALSE);

	g_lpActmap->m_sCorrectFireResistance	=	GetDlgItemInt(hwnd,IDC_CORRECT_FIRE_RESISTANCE,&isTemp,TRUE);
	g_lpActmap->m_sCorrectWaterResistance	=	GetDlgItemInt(hwnd,IDC_CORRECT_WATER_RESISTANCE,&isTemp,TRUE);
	g_lpActmap->m_sCorrectWindResistance	=	GetDlgItemInt(hwnd,IDC_CORRECT_WIND_RESISTANCE,&isTemp,TRUE);
	g_lpActmap->m_sCorrectEarthResistance	=	GetDlgItemInt(hwnd,IDC_CORRECT_EARTH_RESISTANCE,&isTemp,TRUE);
	g_lpActmap->m_sCorrectLightResistance	=	GetDlgItemInt(hwnd,IDC_CORRECT_LIGHT_RESISTANCE,&isTemp,TRUE);
	g_lpActmap->m_sCorrectDarkResistance	=	GetDlgItemInt(hwnd,IDC_CORRECT_DARK_RESISTANCE,&isTemp,TRUE);

	g_lpActmap->m_sCorrectMonsterBadStatusResistance	=	GetDlgItemInt(hwnd,IDC_BAD_STATUS_REGISTANCE	,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterDeclinePowerResistance	=	GetDlgItemInt(hwnd,IDC_DECLINE_POWER_REGISTANCE	,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterCurseResistance		=	GetDlgItemInt(hwnd,IDC_CURSE_REGISTANCE			,&isTemp,TRUE	);

	g_lpActmap->m_sCorrectMonsterBlindResistance		=	GetDlgItemInt(hwnd,IDC_BLIND					,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterPoisonResistance		=	GetDlgItemInt(hwnd,IDC_POISON					,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterSleepResistance		=	GetDlgItemInt(hwnd,IDC_SLEEP					,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterColdResistance			=	GetDlgItemInt(hwnd,IDC_COLD						,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterFreezeResistance		=	GetDlgItemInt(hwnd,IDC_FREEZE					,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterStunResistance			=	GetDlgItemInt(hwnd,IDC_STUN						,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterStoneResistance		=	GetDlgItemInt(hwnd,IDC_STONE					,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterConfuseResistance		=	GetDlgItemInt(hwnd,IDC_CONFUSE					,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterCharmingResistance		=	GetDlgItemInt(hwnd,IDC_CHARMING					,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterCriticalResistance		=	GetDlgItemInt(hwnd,IDC_CRITICAL_RESISTANCE		,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterCrushResistance		=	GetDlgItemInt(hwnd,IDC_CRUSH_RESISTANCE			,&isTemp,TRUE	);

	g_lpActmap->m_sCorrectMonsterFireResistance			=	GetDlgItemInt(hwnd,IDC_FIRE_REGISTANCE			,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterWaterResistance		=	GetDlgItemInt(hwnd,IDC_WATER_REGISTANCE			,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterWindResistance			=	GetDlgItemInt(hwnd,IDC_WIND_REGISTANCE			,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterLightResistance		=	GetDlgItemInt(hwnd,IDC_LIGHT_REGISTANCE			,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterEarthResistance		=	GetDlgItemInt(hwnd,IDC_EARTH_REGISTANCE			,&isTemp,TRUE	);
	g_lpActmap->m_sCorrectMonsterDarkResistance			=	GetDlgItemInt(hwnd,IDC_DARK_REGISTANCE			,&isTemp,TRUE	);

	g_lpActmap->m_bf4FieldType							=	cCOMBOBOX::GetData(GetDlgItem(hwnd,IDC_FIELD_TYPE_LIST));
	g_lpActmap->m_bf4MinimapDisplayLevel				=	cCOMBOBOX::GetData(GetDlgItem(hwnd,IDC_MINIMAP_DISPLAY_LEVEL));
	g_lpActmap->m_wLinkSecretDungeon					=	cCOMBOBOX::GetData(GetDlgItem(hwnd,IDC_LINK_SECRET_DUNGEON));
	// 날씨 10.02.10 추가
	g_lpActmap->m_bf3WeatherType						=	cCOMBOBOX::GetData(GetDlgItem(hwnd,IDC_FIELD_WEATHER));
	if (IsDlgButtonChecked(hwnd,IDC_IS_PREMIUM_ZONE))
		g_lpActmap->m_bf1IsPremiumZone			=	TRUE;
	else
		g_lpActmap->m_bf1IsPremiumZone			=	FALSE;

	if (IsDlgButtonChecked(hwnd,IDC_IS_GUILD_BATTLE_ZONE))
		g_lpActmap->m_bf1IsGuildBattleZone		=	TRUE;
	else
		g_lpActmap->m_bf1IsGuildBattleZone		=	FALSE;

	if (IsDlgButtonChecked(hwnd,IDC_IS_GUILD_BATTLE_LOBBY))
		g_lpActmap->m_bf1IsGuildBattleLobby		=	TRUE;
	else
		g_lpActmap->m_bf1IsGuildBattleLobby		=	FALSE;

	if (IsDlgButtonChecked(hwnd,IDC_IS_EVENT_FIELD))
		g_lpActmap->m_bf1IsEventField			=	TRUE;
	else
		g_lpActmap->m_bf1IsEventField			=	FALSE;

	if (IsDlgButtonChecked(hwnd,IDC_IS_OX_QUIZ_VILLAGE))
		g_lpActmap->m_bf1IsOXQuizGateVillage	=	TRUE;
	else
		g_lpActmap->m_bf1IsOXQuizGateVillage	=	FALSE;

	if (IsDlgButtonChecked(hwnd,IDC_IS_LOCKED_MAP))
		g_lpActmap->m_bf1IsLocked				=	TRUE;
	else
		g_lpActmap->m_bf1IsLocked				=	FALSE;

	if (IsDlgButtonChecked(hwnd,IDC_DISABLE_ORB_MEMORY))
		g_lpActmap->m_bf1IsCanNotMemoryZone	=	TRUE;
	else
		g_lpActmap->m_bf1IsCanNotMemoryZone	=	FALSE;

	if (IsDlgButtonChecked(hwnd,IDC_SECRET_DUNGEON_GATE))
		g_lpActmap->m_bf1IsExistSecretDungeonGate	=	TRUE;
	else
		g_lpActmap->m_bf1IsExistSecretDungeonGate	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_HALF_SIZE))
		g_lpActmap->m_bf1IsHalfSize	=	TRUE;
	else
		g_lpActmap->m_bf1IsHalfSize	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_BOSS_ZONE))
		g_lpActmap->m_bf1IsBossZone	=	TRUE;
	else
		g_lpActmap->m_bf1IsBossZone	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_GUILD_HALL))
		g_lpActmap->m_bf1IsGuildHall	=	TRUE;
	else
		g_lpActmap->m_bf1IsGuildHall	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_1LEVEL_GUILD_HALL))
		g_lpActmap->m_bf1Is1LevelGuildHall	=	TRUE;
	else
		g_lpActmap->m_bf1Is1LevelGuildHall	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_WORD_QUIZ_MAP))
		g_lpActmap->m_bf1IsWordQuizField	=	TRUE;
	else
		g_lpActmap->m_bf1IsWordQuizField	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_WORD_QUIZ_VILLAGE))
		g_lpActmap->m_bf1IsWordQuizVillage	=	TRUE;
	else
		g_lpActmap->m_bf1IsWordQuizVillage	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_BLOCK_TO_WARP))
		g_lpActmap->m_bf1IsBlockToWarpField	=	TRUE;
	else
		g_lpActmap->m_bf1IsBlockToWarpField	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_GUILD_POINT_BATTLE))
		g_lpActmap->m_bf1IsGuildPointBattleField	=	TRUE;
	else
		g_lpActmap->m_bf1IsGuildPointBattleField	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_GUILD_SIEGE_WAREFARE))
		g_lpActmap->m_bf1IsSiegeWarefareField	=	TRUE;
	else
		g_lpActmap->m_bf1IsSiegeWarefareField	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_CAN_NOT_CALL_CARPET_FIELD))
		g_lpActmap->m_bf1IsCanNotCallCarpetField	=	TRUE;
	else
		g_lpActmap->m_bf1IsCanNotCallCarpetField	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_BLOCK_TO_USE_CALLING))
		g_lpActmap->m_bf1IsCanNotUseCallingSkill	=	TRUE;
	else
		g_lpActmap->m_bf1IsCanNotUseCallingSkill	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_FIND_WAY))
		g_lpActmap->m_bf1IsUseFindWay	=	TRUE;
	else
		g_lpActmap->m_bf1IsUseFindWay	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_SET_BOSS_MAP))
		g_lpActmap->m_bf1IsSetBossMap	=	TRUE;
	else
		g_lpActmap->m_bf1IsSetBossMap	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_GUILD_DUNGEON_MAP))
		g_lpActmap->m_bf1IsGuildDungeonMap	=	TRUE;
	else
		g_lpActmap->m_bf1IsGuildDungeonMap	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_NOT_USE_SKILL_BEFORE_TRANS))
		g_lpActmap->m_bf1IsNotUseSkillBeforeTrans	=	TRUE;
	else
		g_lpActmap->m_bf1IsNotUseSkillBeforeTrans	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_NOT_OPEN_PITCHMAN_SHOP))
		g_lpActmap->m_bf1IsNotOpenPitchManShop	=	TRUE;
	else
		g_lpActmap->m_bf1IsNotOpenPitchManShop	=	FALSE;

	if	(IsDlgButtonChecked(hwnd,IDC_IS_NIGHT))
		g_lpActmap->m_bf1IsNight	=	TRUE;
	else
		g_lpActmap->m_bf1IsNight	=	FALSE;
//	if	(IsDlgButtonChecked(hwnd,IDC_IS_HIDDEN_PORTAL))
//		g_lpActmap->m_bf1IsHiddenPortal			=	TRUE;
//	else
//		g_lpActmap->m_bf1IsHiddenPortal			=	FALSE;

	{
		HWND	hBgmList	=	GetDlgItem(hwnd,IDC_BGM);

		int		iBgmCount	=	cLISTBOX::GetCount(hBgmList),iBgm,i;

		memset(g_lpActmap->m_aBgmList,0xff,sizeof(g_lpActmap->m_aBgmList));
		for (i=0;i<iBgmCount;i++)
		{
			iBgm	=	cLISTBOX::GetData(hBgmList,i);
			g_lpActmap->m_aBgmList[i]	=	iBgm;
		}
	}

	g_lpActmap->LostVirgin();

	if (!(iX + iY) && iWidth == g_lpActmap->m_iWidth && iHeight == g_lpActmap->m_iHeight)
	{
		strcpy(g_lpActmap->m_strName,strName);

		char	name[32];

		ustrncpy(name,g_lpActmap->m_strName,20);

		g_bmMapList.Set(g_iSelectMap,name);

		cMAP::Select(g_iSelectMap);

		return TRUE;
	}

	isTemp				=	g_lpActmap->Resize(strName,iX, iY,iWidth,iHeight,iLimitRegenMonsterCount);

	return isTemp;
}

LRESULT CALLBACK 
cSETMAP::WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			Init(hWnd);
			break;

		case WM_COMMAND :
			switch(LOWORD(wParam))
			{	case	IDOK				:
					if (Setting(hWnd))	EndDialog(hWnd,IDOK);
					break;

				case	IDCANCEL			:
					EndDialog(hWnd,IDCANCEL);
					break;

				case	IDC_ADD_BGM			:
				{
					int		iBgm		=	cCOMBOBOX::GetData(GetDlgItem(hWnd,IDC_BGM_LIST));
					HWND	hBgmList	=	GetDlgItem(hWnd,IDC_BGM);

					cLISTBOX::Add(hBgmList,g_aBgm[iBgm].m_strTitle,iBgm);
					break;
				}

				case	IDC_REMOVE_BGM		:
				{
					HWND	hBgmList	=	GetDlgItem(hWnd,IDC_BGM);
					int		iIndex		=	cLISTBOX::GetSelect(hBgmList);

					cLISTBOX::Del(hBgmList,iIndex);
					break;
				}

				case	IDC_PLAY			:
				{
					int	iIndex	=	cCOMBOBOX::GetSelect(GetDlgItem(hWnd,IDC_BGM_LIST));

					g_bgPlayer.Play(_ms("data/bgm/%s",g_aBgm[iIndex].m_strFileName));
					break;
				}

				default	:
					break;
			}
			break;
			
		
		default :	return (FALSE);
	}
	return (TRUE);
}