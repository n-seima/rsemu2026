#include "cCompareBox.h"
#include "resource.h"

HWND					cCompareBox::s_hWnd;
HINSTANCE				cCompareBox::s_hInst;
char					cCompareBox::s_strSelectedFirstFilePath[MAX_PATH];		//	첫번째로 선택한 파일 경로
char					cCompareBox::s_strSelectedSecondFilePath[MAX_PATH];		//	두번째로 선택한 파일 경로
int						cCompareBox::s_iCompareKind;							//	비교할 종류?? 

cBASIC_ITEM*			cCompareBox::s_lpFirstBasicItem;

CCustomItemManager*		cCompareBox::s_lpFirstPremiumItem;			
CCustomItemManager*		cCompareBox::s_lpFirstCustomItem;			
cITEM_PREFIX_MANAGER*	cCompareBox::s_lpFirstItemPrefixManager;
cArcaManager*			cCompareBox::s_lpFirstArcaManager;			
cItemSetManager*		cCompareBox::s_lpFirstItemSetManager;		
cPieceItemManager*		cCompareBox::s_lpFirstPieceItemManager;		

cBASIC_ITEM*			cCompareBox::s_lpSecondBasicItem;			

CCustomItemManager*		cCompareBox::s_lpSecondPremiumItem;			
CCustomItemManager*		cCompareBox::s_lpSecondCustomItem;			
cITEM_PREFIX_MANAGER*	cCompareBox::s_lpSecondItemPrefixManager;	
cArcaManager*			cCompareBox::s_lpSecondArcaManager;			
cItemSetManager*		cCompareBox::s_lpSecondItemSetManager;		
cPieceItemManager*		cCompareBox::s_lpSecondPieceItemManager;	

cMAP*					cCompareBox::s_lpFirstMap;
cMAP*					cCompareBox::s_lpSecondMap;


void
cCompareBox::reset()
{
	s_iCompareKind	=	eCOMPARE_NONE;

	strcpy(s_strSelectedFirstFilePath,"NONE");
	strcpy(s_strSelectedSecondFilePath,"NONE");
	SetDlgItemText(s_hWnd,IDC_STATUS ,"");
	SetDlgItemText(s_hWnd,IDC_RESULT ,"");
}

void
cCompareBox::compareFile()
{


	SetDlgItemText(s_hWnd,IDC_STATUS ,"");
	SetDlgItemText(s_hWnd,IDC_RESULT ,"");



	addTextToEdit(IDC_STATUS,"StartComPare!\r\n");

	switch(s_iCompareKind)
	{

	case	eCOMPARE_NONE		:

		break;
	case	eCOMPARE_MAP_FILE	:
		compareMapFile();
		break;
	case	eCOMPARE_RPD_FILE	:
		compareRPDFile();
		break;
	case	eCOMPARE_ITEM_FILE	:
		compareItemFile();
		break;
	default						:
		cMSG::Error("What?????");

	}
	addTextToEdit(IDC_STATUS, "CompareItemFile End!!\r\n");

}

void
cCompareBox::addTextToEdit(int _iEditID, char* _strText)
{
	int iLen = GetWindowTextLength(GetDlgItem(s_hWnd,_iEditID));
	
	SendDlgItemMessage(s_hWnd, _iEditID, EM_SETSEL, iLen, iLen);
	
	SendDlgItemMessage(s_hWnd, _iEditID, EM_REPLACESEL, FALSE, (LPARAM)_strText);
}



int
cCompareBox::compareMapBasicInfo()
{

	addTextToEdit(IDC_STATUS, "Map BasicInfo Compare Start!!\r\n");

	int iNotSameBasicInfo	= 0;
	

	if(s_lpFirstMap->m_iWidth != s_lpSecondMap->m_iWidth)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_iWidth[%d] Second : m_iWidth[%d]!!\r\n",s_lpFirstMap->m_iWidth,s_lpSecondMap->m_iWidth));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_iHeight != s_lpSecondMap->m_iHeight)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_iHeight[%d] Second : m_iHeight[%d]!!\r\n",s_lpFirstMap->m_iHeight,s_lpSecondMap->m_iHeight));
		++iNotSameBasicInfo;
	}

	if(strcmp(s_lpFirstMap->m_strName, s_lpSecondMap->m_strName) != 0)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_strName[%s] Second : m_strName[%s]!!\r\n",s_lpFirstMap->m_strName,s_lpSecondMap->m_strName));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_iTileSet != s_lpSecondMap->m_iTileSet)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_iTileSet[%d] Second : m_iTileSet[%d]!!\r\n",s_lpFirstMap->m_iTileSet,s_lpSecondMap->m_iTileSet));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf4FieldType != s_lpSecondMap->m_bf4FieldType)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf4FieldType[%d] Second : m_bf4FieldType[%d]!!\r\n",s_lpFirstMap->m_bf4FieldType,s_lpSecondMap->m_bf4FieldType));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsPremiumZone != s_lpSecondMap->m_bf1IsPremiumZone)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsPremiumZone[%d] Second : m_bf1IsPremiumZone[%d]!!\r\n",s_lpFirstMap->m_bf1IsPremiumZone,s_lpSecondMap->m_bf1IsPremiumZone));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsGuildBattleZone != s_lpSecondMap->m_bf1IsGuildBattleZone)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsGuildBattleZone[%d] Second : m_bf1IsGuildBattleZone[%d]!!\r\n",s_lpFirstMap->m_bf1IsGuildBattleZone,s_lpSecondMap->m_bf1IsGuildBattleZone));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsGuildBattleLobby != s_lpSecondMap->m_bf1IsGuildBattleLobby)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsGuildBattleLobby[%d] Second : m_bf1IsGuildBattleLobby[%d]!!\r\n",s_lpFirstMap->m_bf1IsGuildBattleLobby,s_lpSecondMap->m_bf1IsGuildBattleLobby));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsLocked != s_lpSecondMap->m_bf1IsLocked)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsLocked[%d] Second : m_bf1IsLocked[%d]!!\r\n",s_lpFirstMap->m_bf1IsLocked,s_lpSecondMap->m_bf1IsLocked));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsEventField != s_lpSecondMap->m_bf1IsEventField)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsEventField[%d] Second : m_bf1IsEventField[%d]!!\r\n",s_lpFirstMap->m_bf1IsEventField,s_lpSecondMap->m_bf1IsEventField));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsOXQuizGateVillage != s_lpSecondMap->m_bf1IsOXQuizGateVillage)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsOXQuizGateVillage[%d] Second : m_bf1IsOXQuizGateVillage[%d]!!\r\n",s_lpFirstMap->m_bf1IsOXQuizGateVillage,s_lpSecondMap->m_bf1IsOXQuizGateVillage));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsHiddenPortal != s_lpSecondMap->m_bf1IsHiddenPortal)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsHiddenPortal[%d] Second : m_bf1IsHiddenPortal[%d]!!\r\n",s_lpFirstMap->m_bf1IsHiddenPortal,s_lpSecondMap->m_bf1IsHiddenPortal));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf4MinimapDisplayLevel != s_lpSecondMap->m_bf4MinimapDisplayLevel)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf4MinimapDisplayLevel[%d] Second : m_bf4MinimapDisplayLevel[%d]!!\r\n",s_lpFirstMap->m_bf4MinimapDisplayLevel,s_lpSecondMap->m_bf4MinimapDisplayLevel));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsCanNotMemoryZone != s_lpSecondMap->m_bf1IsCanNotMemoryZone)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsCanNotMemoryZone[%d] Second : m_bf1IsCanNotMemoryZone[%d]!!\r\n",s_lpFirstMap->m_bf1IsCanNotMemoryZone,s_lpSecondMap->m_bf1IsCanNotMemoryZone));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsHalfSize != s_lpSecondMap->m_bf1IsHalfSize)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsHalfSize[%d] Second : m_bf1IsHalfSize[%d]!!\r\n",s_lpFirstMap->m_bf1IsHalfSize,s_lpSecondMap->m_bf1IsHalfSize));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsWordQuizField != s_lpSecondMap->m_bf1IsWordQuizField)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsWordQuizField[%d] Second : m_bf1IsWordQuizField[%d]!!\r\n",s_lpFirstMap->m_bf1IsWordQuizField,s_lpSecondMap->m_bf1IsWordQuizField));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsWordQuizVillage != s_lpSecondMap->m_bf1IsWordQuizVillage)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsWordQuizVillage[%d] Second : m_bf1IsWordQuizVillage[%d]!!\r\n",s_lpFirstMap->m_bf1IsWordQuizVillage,s_lpSecondMap->m_bf1IsWordQuizVillage));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsBlockToWarpField != s_lpSecondMap->m_bf1IsBlockToWarpField)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsBlockToWarpField[%d] Second : m_bf1IsBlockToWarpField[%d]!!\r\n",s_lpFirstMap->m_bf1IsBlockToWarpField,s_lpSecondMap->m_bf1IsBlockToWarpField));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsBossZone != s_lpSecondMap->m_bf1IsBossZone)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsBossZone[%d] Second : m_bf1IsBossZone[%d]!!\r\n",s_lpFirstMap->m_bf1IsBossZone,s_lpSecondMap->m_bf1IsBossZone));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsGuildHall != s_lpSecondMap->m_bf1IsGuildHall)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsGuildHall[%d] Second : m_bf1IsGuildHall[%d]!!\r\n",s_lpFirstMap->m_bf1IsGuildHall,s_lpSecondMap->m_bf1IsGuildHall));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1Is1LevelGuildHall != s_lpSecondMap->m_bf1Is1LevelGuildHall)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1Is1LevelGuildHall[%d] Second : m_bf1Is1LevelGuildHall[%d]!!\r\n",s_lpFirstMap->m_bf1Is1LevelGuildHall,s_lpSecondMap->m_bf1Is1LevelGuildHall));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsGuildPointBattleField != s_lpSecondMap->m_bf1IsGuildPointBattleField)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsGuildPointBattleField[%d] Second : m_bf1IsGuildPointBattleField[%d]!!\r\n",s_lpFirstMap->m_bf1IsGuildPointBattleField,s_lpSecondMap->m_bf1IsGuildPointBattleField));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsSiegeWarefareField != s_lpSecondMap->m_bf1IsSiegeWarefareField)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsSiegeWarefareField[%d] Second : m_bf1IsSiegeWarefareField[%d]!!\r\n",s_lpFirstMap->m_bf1IsSiegeWarefareField,s_lpSecondMap->m_bf1IsSiegeWarefareField));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsCanNotCallCarpetField != s_lpSecondMap->m_bf1IsCanNotCallCarpetField)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsCanNotCallCarpetField[%d] Second : m_bf1IsCanNotCallCarpetField[%d]!!\r\n",s_lpFirstMap->m_bf1IsCanNotCallCarpetField,s_lpSecondMap->m_bf1IsCanNotCallCarpetField));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsCanNotUseCallingSkill != s_lpSecondMap->m_bf1IsCanNotUseCallingSkill)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsCanNotUseCallingSkill[%d] Second : m_bf1IsCanNotUseCallingSkill[%d]!!\r\n",s_lpFirstMap->m_bf1IsCanNotUseCallingSkill,s_lpSecondMap->m_bf1IsCanNotUseCallingSkill));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsUseFindWay != s_lpSecondMap->m_bf1IsUseFindWay)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsUseFindWay[%d] Second : m_bf1IsUseFindWay[%d]!!\r\n",s_lpFirstMap->m_bf1IsUseFindWay,s_lpSecondMap->m_bf1IsUseFindWay));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsSetBossMap != s_lpSecondMap->m_bf1IsSetBossMap)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsSetBossMap[%d] Second : m_bf1IsSetBossMap[%d]!!\r\n",s_lpFirstMap->m_bf1IsSetBossMap,s_lpSecondMap->m_bf1IsSetBossMap));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsGuildDungeonMap != s_lpSecondMap->m_bf1IsGuildDungeonMap)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsGuildDungeonMap[%d] Second : m_bf1IsGuildDungeonMap[%d]!!\r\n",s_lpFirstMap->m_bf1IsGuildDungeonMap,s_lpSecondMap->m_bf1IsGuildDungeonMap));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsNotUseSkillBeforeTrans != s_lpSecondMap->m_bf1IsNotUseSkillBeforeTrans)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsNotUseSkillBeforeTrans[%d] Second : m_bf1IsNotUseSkillBeforeTrans[%d]!!\r\n",s_lpFirstMap->m_bf1IsNotUseSkillBeforeTrans,s_lpSecondMap->m_bf1IsNotUseSkillBeforeTrans));
		++iNotSameBasicInfo;
	}

	if(s_lpFirstMap->m_bf1IsNotOpenPitchManShop != s_lpSecondMap->m_bf1IsNotOpenPitchManShop)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsNotOpenPitchManShop[%d] Second : m_bf1IsNotOpenPitchManShop[%d]!!\r\n",s_lpFirstMap->m_bf1IsNotOpenPitchManShop,s_lpSecondMap->m_bf1IsNotOpenPitchManShop));
		++iNotSameBasicInfo;
	}

	addTextToEdit(IDC_STATUS, "Map BasicInfo Compare End!!\r\n");


	return	iNotSameBasicInfo;
}


int
cCompareBox::compareMapBaseInfo20031111()
{

	addTextToEdit(IDC_STATUS, "Map Info CMapBaseInfo20031111 Start!!\r\n");

	int iNotSameMapBaseInfo20031111		=	0;

	if(memcmp(s_lpFirstMap->m_aBgmList,s_lpSecondMap->m_aBgmList, sizeof(WORD) * dMAX_BGM_COUNT_IN_MAP) != 0)
	{
		addTextToEdit(IDC_RESULT, "NotSame  m_aBgmList");
		++iNotSameMapBaseInfo20031111;
	}

	if(s_lpFirstMap->m_wLinkSecretDungeon != s_lpSecondMap->m_wLinkSecretDungeon)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_wLinkSecretDungeon[%d] Second : m_wLinkSecretDungeon[%d]!!\r\n",s_lpFirstMap->m_wLinkSecretDungeon,s_lpSecondMap->m_wLinkSecretDungeon));
		++iNotSameMapBaseInfo20031111;
	}

	if(s_lpFirstMap->m_bf1IsExistSecretDungeonGate != s_lpSecondMap->m_bf1IsExistSecretDungeonGate)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsExistSecretDungeonGate[%d] Second : m_bf1IsExistSecretDungeonGate[%d]!!\r\n",s_lpFirstMap->m_bf1IsExistSecretDungeonGate,s_lpSecondMap->m_bf1IsExistSecretDungeonGate));
		++iNotSameMapBaseInfo20031111;
	}

	if(s_lpFirstMap->m_bf3WeatherType != s_lpSecondMap->m_bf3WeatherType)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf3WeatherType[%d] Second : m_bf3WeatherType[%d]!!\r\n",s_lpFirstMap->m_bf3WeatherType,s_lpSecondMap->m_bf3WeatherType));
		++iNotSameMapBaseInfo20031111;
	}

	if(s_lpFirstMap->m_bf1IsNight != s_lpSecondMap->m_bf1IsNight)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsNight[%d] Second : m_bf1IsNight[%d]!!\r\n",s_lpFirstMap->m_bf1IsNight,s_lpSecondMap->m_bf1IsNight));
		++iNotSameMapBaseInfo20031111;
	}

	addTextToEdit(IDC_STATUS, "Map Info CMapBaseInfo20031111 End!!\r\n");

	return	iNotSameMapBaseInfo20031111;
}

int
cCompareBox::compareMapBaseInfo20050527()
{

	addTextToEdit(IDC_STATUS, "Map Info cMapBaseInfo20050527 Start!!\r\n");
	
	int iNotSameMapBaseInfo2005052	=	0;

	if(s_lpFirstMap->m_sCorrectFireResistance != s_lpSecondMap->m_sCorrectFireResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectFireResistance[%d] Second : m_sCorrectFireResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectFireResistance,s_lpSecondMap->m_sCorrectFireResistance));
		++iNotSameMapBaseInfo2005052;
	}

	if(s_lpFirstMap->m_sCorrectWaterResistance != s_lpSecondMap->m_sCorrectWaterResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectWaterResistance[%d] Second : m_sCorrectWaterResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectWaterResistance,s_lpSecondMap->m_sCorrectWaterResistance));
		++iNotSameMapBaseInfo2005052;
	}

	
	if(s_lpFirstMap->m_sCorrectWindResistance != s_lpSecondMap->m_sCorrectWindResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectWindResistance[%d] Second : m_sCorrectWindResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectWindResistance,s_lpSecondMap->m_sCorrectWindResistance));
		++iNotSameMapBaseInfo2005052;
	}
	
	if(s_lpFirstMap->m_sCorrectEarthResistance != s_lpSecondMap->m_sCorrectEarthResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectEarthResistance[%d] Second : m_sCorrectEarthResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectEarthResistance,s_lpSecondMap->m_sCorrectEarthResistance));
		++iNotSameMapBaseInfo2005052;
	}
		
	if(s_lpFirstMap->m_sCorrectLightResistance != s_lpSecondMap->m_sCorrectLightResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectLightResistance[%d] Second : m_sCorrectLightResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectLightResistance,s_lpSecondMap->m_sCorrectLightResistance));
		++iNotSameMapBaseInfo2005052;
	}

	if(s_lpFirstMap->m_sCorrectDarkResistance != s_lpSecondMap->m_sCorrectDarkResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectDarkResistance[%d] Second : m_sCorrectDarkResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectDarkResistance,s_lpSecondMap->m_sCorrectDarkResistance));
		++iNotSameMapBaseInfo2005052;
	}

	addTextToEdit(IDC_STATUS, "Map Info cMapBaseInfo20050527 End!!\r\n");

	return	iNotSameMapBaseInfo2005052;

}


int
cCompareBox::compareMapInfo()
{


	addTextToEdit(IDC_STATUS, "Compare cMAP_INFO Start!!\r\n");

	int iNotSameMapInfo		=	0;

	if(s_lpFirstMap->m_sCorrectMonsterFireResistance != s_lpSecondMap->m_sCorrectMonsterFireResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterFireResistance[%d] Second : m_sCorrectMonsterFireResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterFireResistance,s_lpSecondMap->m_sCorrectMonsterFireResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterWaterResistance != s_lpSecondMap->m_sCorrectMonsterWaterResistance)
	{
	addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterWaterResistance[%d] Second : m_sCorrectMonsterWaterResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterWaterResistance,s_lpSecondMap->m_sCorrectMonsterWaterResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterWindResistance != s_lpSecondMap->m_sCorrectMonsterWindResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterWindResistance[%d] Second : m_sCorrectMonsterWindResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterWindResistance,s_lpSecondMap->m_sCorrectMonsterWindResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterEarthResistance != s_lpSecondMap->m_sCorrectMonsterEarthResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterEarthResistance[%d] Second : m_sCorrectMonsterEarthResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterEarthResistance,s_lpSecondMap->m_sCorrectMonsterEarthResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterLightResistance != s_lpSecondMap->m_sCorrectMonsterLightResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterLightResistance[%d] Second : m_sCorrectMonsterLightResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterLightResistance,s_lpSecondMap->m_sCorrectMonsterLightResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterDarkResistance != s_lpSecondMap->m_sCorrectMonsterDarkResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterDarkResistance[%d] Second : m_sCorrectMonsterDarkResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterDarkResistance,s_lpSecondMap->m_sCorrectMonsterDarkResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterBlindResistance != s_lpSecondMap->m_sCorrectMonsterBlindResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterBlindResistance[%d] Second : m_sCorrectMonsterBlindResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterBlindResistance,s_lpSecondMap->m_sCorrectMonsterBlindResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterPoisonResistance != s_lpSecondMap->m_sCorrectMonsterPoisonResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterPoisonResistance[%d] Second : m_sCorrectMonsterPoisonResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterPoisonResistance,s_lpSecondMap->m_sCorrectMonsterPoisonResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterSleepResistance != s_lpSecondMap->m_sCorrectMonsterSleepResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterSleepResistance[%d] Second : m_sCorrectMonsterSleepResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterSleepResistance,s_lpSecondMap->m_sCorrectMonsterSleepResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterColdResistance != s_lpSecondMap->m_sCorrectMonsterColdResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterColdResistance[%d] Second : m_sCorrectMonsterColdResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterColdResistance,s_lpSecondMap->m_sCorrectMonsterColdResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterFreezeResistance != s_lpSecondMap->m_sCorrectMonsterFreezeResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterFreezeResistance[%d] Second : m_sCorrectMonsterFreezeResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterFreezeResistance,s_lpSecondMap->m_sCorrectMonsterFreezeResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterStunResistance != s_lpSecondMap->m_sCorrectMonsterStunResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterStunResistance[%d] Second : m_sCorrectMonsterStunResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterStunResistance,s_lpSecondMap->m_sCorrectMonsterStunResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterStoneResistance != s_lpSecondMap->m_sCorrectMonsterStoneResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterStoneResistance[%d] Second : m_sCorrectMonsterStoneResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterStoneResistance,s_lpSecondMap->m_sCorrectMonsterStoneResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterConfuseResistance != s_lpSecondMap->m_sCorrectMonsterConfuseResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterConfuseResistance[%d] Second : m_sCorrectMonsterConfuseResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterConfuseResistance,s_lpSecondMap->m_sCorrectMonsterConfuseResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterCharmingResistance != s_lpSecondMap->m_sCorrectMonsterCharmingResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterCharmingResistance[%d] Second : m_sCorrectMonsterCharmingResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterCharmingResistance,s_lpSecondMap->m_sCorrectMonsterCharmingResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterStoneResistance != s_lpSecondMap->m_sCorrectMonsterStoneResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterStoneResistance[%d] Second : m_sCorrectMonsterStoneResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterStoneResistance,s_lpSecondMap->m_sCorrectMonsterStoneResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterCriticalResistance != s_lpSecondMap->m_sCorrectMonsterCriticalResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterCriticalResistance[%d] Second : m_sCorrectMonsterCriticalResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterCriticalResistance,s_lpSecondMap->m_sCorrectMonsterCriticalResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterCrushResistance != s_lpSecondMap->m_sCorrectMonsterCrushResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterCrushResistance[%d] Second : m_sCorrectMonsterCrushResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterCrushResistance,s_lpSecondMap->m_sCorrectMonsterCrushResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterBadStatusResistance != s_lpSecondMap->m_sCorrectMonsterBadStatusResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterBadStatusResistance[%d] Second : m_sCorrectMonsterBadStatusResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterBadStatusResistance,s_lpSecondMap->m_sCorrectMonsterBadStatusResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterDeclinePowerResistance != s_lpSecondMap->m_sCorrectMonsterDeclinePowerResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterDeclinePowerResistance[%d] Second : m_sCorrectMonsterDeclinePowerResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterDeclinePowerResistance,s_lpSecondMap->m_sCorrectMonsterDeclinePowerResistance));
		++iNotSameMapInfo;
	}

	if(s_lpFirstMap->m_sCorrectMonsterCurseResistance != s_lpSecondMap->m_sCorrectMonsterCurseResistance)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_sCorrectMonsterCurseResistance[%d] Second : m_sCorrectMonsterCurseResistance[%d]!!\r\n",s_lpFirstMap->m_sCorrectMonsterCurseResistance,s_lpSecondMap->m_sCorrectMonsterCurseResistance));
		++iNotSameMapInfo;
	}

	addTextToEdit(IDC_STATUS, "Compare cMAP_INFO End!!\r\n");

	return		iNotSameMapInfo;

}



int
cCompareBox::compareMap()
{
	addTextToEdit(IDC_STATUS, "Compare Map Start!!\r\n");
	int iNotSameMap		=	0;

	iNotSameMap		+=	compareObjectInMap();
	iNotSameMap		+=	compareValueInMap();
	iNotSameMap		+=	compareShopManagerInMap();
	iNotSameMap		+=	compareCustomItemManagerInMap();
	iNotSameMap		+=	comparePatrolRoadManagerInMap();
	iNotSameMap		+=	compareCharacterPatternPackInMap();
	iNotSameMap		+=	compareKarmaContainerInMap();
	iNotSameMap		+=	compareCharacterPatternPackInMap();
	iNotSameMap		+=	compareBuildingInMap();
	iNotSameMap		+=	compareAreaInMap();
	iNotSameMap		+=	compareCharacterSetInMap();
	iNotSameMap		+=	compareActorSetInMap();
	iNotSameMap		+=	compareEtcInMap();

	addTextToEdit(IDC_STATUS, "Compare Map End!!\r\n");
	return iNotSameMap;
}
int
cCompareBox::compareEtcInMap()
{
	addTextToEdit(IDC_STATUS, "Compare Etc Start!!\r\n");

	int iNotSameEtcCount	=	0;

	if(s_lpFirstMap->m_iRealWidth != s_lpSecondMap->m_iRealWidth)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_iRealWidth[%d] Second : m_iRealWidth[%d]!!\r\n",s_lpFirstMap->m_iRealWidth,s_lpSecondMap->m_iRealWidth));
		++iNotSameEtcCount;
	}

	if(s_lpFirstMap->m_iRealHeight != s_lpSecondMap->m_iRealHeight)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_iRealHeight[%d] Second : m_iRealHeight[%d]!!\r\n",s_lpFirstMap->m_iRealHeight,s_lpSecondMap->m_iRealHeight));
		++iNotSameEtcCount;
	}

	for(int iFieldValue = 0;iFieldValue<50;++iFieldValue)
	{
		if(strcmp(s_lpFirstMap->m_astrFieldValue[iFieldValue],s_lpFirstMap->m_astrFieldValue[iFieldValue]) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("[%d] first : m_astrFieldValue[%s] Second : m_astrFieldValue[%s]\r\n",iFieldValue,s_lpFirstMap->m_astrFieldValue[iFieldValue],s_lpFirstMap->m_astrFieldValue[iFieldValue]));
			++iNotSameEtcCount;
		}
	}

	addTextToEdit(IDC_STATUS, "Compare Etc End!!\r\n");

	return iNotSameEtcCount;
}

int
cCompareBox::compareObjectInMap()
{

	addTextToEdit(IDC_STATUS, "Compare ObjectInMap Start!!\r\n");

	int iNotSameObjectCount	=	0;

	for(int iTinyObject = 0;iTinyObject < dMAX_OBJECT_COUNT;++iTinyObject)
	{
		cTINY_OBJECT_INFO* lpFirstTinyObjectInfo	=	s_lpFirstMap->m_object.getTinyObject(iTinyObject);
		cTINY_OBJECT_INFO* lpSecondTinyObjectInfo	=	s_lpSecondMap->m_object.getTinyObject(iTinyObject);

		if(!lpFirstTinyObjectInfo || !lpSecondTinyObjectInfo)
		{
			if((lpFirstTinyObjectInfo && !lpSecondTinyObjectInfo) || (!lpFirstTinyObjectInfo && lpSecondTinyObjectInfo))
			{
				
				addTextToEdit(IDC_RESULT, _ms("Not Same Tiny[%d]!!\r\n",iTinyObject));
				++iNotSameObjectCount;
			}
			continue;
		}
		if(lpFirstTinyObjectInfo->m_wObject != lpSecondTinyObjectInfo->m_wObject)
		{
			
			addTextToEdit(IDC_RESULT, _ms("Tiny[%d] First : m_wObject[%d] Second : m_wObject[%d]!!\r\n",iTinyObject,lpFirstTinyObjectInfo->m_wObject, lpSecondTinyObjectInfo->m_wObject));
			++iNotSameObjectCount;				
		}
		if(lpFirstTinyObjectInfo->m_wSerial != lpSecondTinyObjectInfo->m_wSerial)
		{
			
			addTextToEdit(IDC_RESULT, _ms("Tiny[%d] First : m_wSerial[%d] Second : m_wSerial[%d]!!\r\n",iTinyObject,lpFirstTinyObjectInfo->m_wSerial, lpSecondTinyObjectInfo->m_wSerial));
			++iNotSameObjectCount;				
		}
		if(memcmp(&lpFirstTinyObjectInfo->m_pos, &lpSecondTinyObjectInfo->m_pos,sizeof(cPOINT)) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("not Same Tiny[%d] m_pos!!\r\n",iTinyObject));
			++iNotSameObjectCount;				
		}
	}


	
	for(int iFixedObject = 0;iFixedObject < dMAX_OBJECT_COUNT;++iFixedObject)
	{
		cFIXED_OBJECT_INFO* lpFirstFixedObjectInfo	=	s_lpFirstMap->m_object.getFixedObject(iFixedObject);
		cFIXED_OBJECT_INFO* lpSecondFixedObjectInfo	=	s_lpSecondMap->m_object.getFixedObject(iFixedObject);

		if(!lpFirstFixedObjectInfo || !lpSecondFixedObjectInfo)
		{
			if((lpFirstFixedObjectInfo && !lpSecondFixedObjectInfo) || (!lpFirstFixedObjectInfo && lpSecondFixedObjectInfo))
			{

				addTextToEdit(IDC_RESULT, _ms("First : m_iCount[%d] Second : m_iCount[%d]!!\r\n",s_lpFirstMap->m_Value.m_iCount, s_lpSecondMap->m_Value.m_iCount));

				++iNotSameObjectCount;
			}
			continue;
		}
		if(lpFirstFixedObjectInfo->m_wSerial != lpSecondFixedObjectInfo->m_wSerial)
		{
			
			addTextToEdit(IDC_RESULT, _ms("Fixed[%d] First : m_wSerial[%d] Second : m_wSerial[%d]!!\r\n",iTinyObject,lpFirstFixedObjectInfo->m_wSerial, lpSecondFixedObjectInfo->m_wSerial));
			++iNotSameObjectCount;				
		}
		if(lpFirstFixedObjectInfo->m_wSerial != lpSecondFixedObjectInfo->m_wSerial)
		{
			
			addTextToEdit(IDC_RESULT, _ms("Fixed[%d] First : m_wSerial[%d] Second : m_wSerial[%d]!!\r\n",iFixedObject,lpFirstFixedObjectInfo->m_wSerial, lpSecondFixedObjectInfo->m_wSerial));
			++iNotSameObjectCount;				
		}
		if(memcmp(&lpFirstFixedObjectInfo->m_pos, &lpSecondFixedObjectInfo->m_pos,sizeof(cPOINT)) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("not Same Fixed[%d] m_pos!!\r\n",iFixedObject));
			++iNotSameObjectCount;				
		}
		if(lpFirstFixedObjectInfo->m_wObject != lpSecondFixedObjectInfo->m_wObject)
		{
			
			addTextToEdit(IDC_RESULT, _ms("Fixed[%d] First : m_wObject[%d] Second : m_wObject[%d]!!\r\n",iFixedObject,lpFirstFixedObjectInfo->m_wObject, lpSecondFixedObjectInfo->m_wObject));
			++iNotSameObjectCount;				
		}
		if(lpFirstFixedObjectInfo->m_wIsPutShadow != lpSecondFixedObjectInfo->m_wIsPutShadow)
		{
			
			addTextToEdit(IDC_RESULT, _ms("Fixed[%d] First : m_wIsPutShadow[%d] Second : m_wIsPutShadow[%d]!!\r\n",iFixedObject,lpFirstFixedObjectInfo->m_wIsPutShadow, lpSecondFixedObjectInfo->m_wIsPutShadow));
			++iNotSameObjectCount;				
		}

		for(int iAddOnObject =0;iAddOnObject<dMAX_ADD_ON_OBJECT_COUNT;++iAddOnObject)
		{
			if(!lpFirstFixedObjectInfo->m_aAddonObject[iAddOnObject].isSame(&lpSecondFixedObjectInfo->m_aAddonObject[iAddOnObject]))
			{
				addTextToEdit(IDC_RESULT, _ms("Fixed[%d] First : m_aAddonObject[%d] Second : m_wIsPutShadow[%d]!!\r\n",iFixedObject,iAddOnObject));
				++iNotSameObjectCount;		
			}
		}
	}

	addTextToEdit(IDC_STATUS, "Compare ObjectInMap End!!\r\n");

	return iNotSameObjectCount;
}

int
cCompareBox::compareValueInMap()
{

	addTextToEdit(IDC_STATUS, "Compare ValueInMap Start!!\r\n");

	int iNotSameValueCount	=	0;

	if(s_lpFirstMap->m_Value.m_iCount != s_lpSecondMap->m_Value.m_iCount)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_iCount[%d] Second : m_iCount[%d]!!\r\n",s_lpFirstMap->m_Value.m_iCount, s_lpSecondMap->m_Value.m_iCount));
		++iNotSameValueCount;
	}
	for(int i=0;i<dVALUE_COUNT;++i)
	{
		if(s_lpFirstMap->m_Value.m_aValue[i].m_iValue != s_lpSecondMap->m_Value.m_aValue[i].m_iValue)
		{
			addTextToEdit(IDC_RESULT, _ms("First : Value[%d].m_iValue[%d]  Second : Value[%d].m_iValue[%d]!!\r\n",i,s_lpFirstMap->m_Value.m_aValue[i].m_iValue,i,s_lpSecondMap->m_Value.m_aValue[i].m_iValue));
			++iNotSameValueCount;
		}
		if(strcmp(s_lpFirstMap->m_Value.m_aValue[i].m_strName,s_lpSecondMap->m_Value.m_aValue[i].m_strName) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("First : Value[%d].m_strName[%s]  Second : Value[%d].m_strName[%s]!!\r\n",i,s_lpFirstMap->m_Value.m_aValue[i].m_strName,i,s_lpSecondMap->m_Value.m_aValue[i].m_strName));
			++iNotSameValueCount;
		}
	}
	
	addTextToEdit(IDC_STATUS, "Compare ValueInMap End!!\r\n");
	return iNotSameValueCount;
}

int
cCompareBox::compareShopManagerInMap()
{
	addTextToEdit(IDC_STATUS, "Compare ShopManager Start!!\r\n");
	int iNotSameShopManagerCount	=	0;

	for(int i=0;i<dMAX_SHOP;++i)
	{

		CShop	*lpFirstShop	=	s_lpFirstMap->m_shop.get(i);
		CShop	*lpSecondShop	=	s_lpSecondMap->m_shop.get(i);

		if(!lpFirstShop || !lpSecondShop)
		{
			if((lpFirstShop && !lpSecondShop) || (!lpFirstShop && lpSecondShop))
			{
				addTextToEdit(IDC_RESULT, _ms("Not Same ShopManager In Map[%d]",i));
				++iNotSameShopManagerCount;
			}
			continue;
		}

		if(lpFirstShop->m_wSerial != lpSecondShop->m_wSerial)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wSerial[%d] Second : m_wSerial[%d]!!\r\n",lpFirstShop->m_wSerial, lpSecondShop->m_wSerial));
			++iNotSameShopManagerCount;
		}
		if(lpFirstShop->m_wPriceFactor != lpSecondShop->m_wPriceFactor)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wPriceFactor[%d] Second : m_wPriceFactor[%d]!!\r\n",lpFirstShop->m_wPriceFactor, lpSecondShop->m_wPriceFactor));
			++iNotSameShopManagerCount;
		}
		if(strcmp(lpFirstShop->m_strName , lpSecondShop->m_strName) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_strName[%s] Second : m_strName[%s]!!\r\n",lpFirstShop->m_strName, lpSecondShop->m_strName));
			++iNotSameShopManagerCount;
		}
		if(lpFirstShop->m_bf1IsDuelPointShop != lpSecondShop->m_bf1IsDuelPointShop)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsDuelPointShop[%d] Second : m_bf1IsDuelPointShop[%d]!!\r\n",lpFirstShop->m_bf1IsDuelPointShop, lpSecondShop->m_bf1IsDuelPointShop));
			++iNotSameShopManagerCount;
		}
		if(lpFirstShop->m_bf1IsLevelPerGoldShop != lpSecondShop->m_bf1IsLevelPerGoldShop)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsLevelPerGoldShop[%d] Second : m_bf1IsLevelPerGoldShop[%d]!!\r\n",lpFirstShop->m_bf1IsLevelPerGoldShop, lpSecondShop->m_bf1IsLevelPerGoldShop));
			++iNotSameShopManagerCount;
		}
		if(lpFirstShop->m_bf15LevelPerGoldValue != lpSecondShop->m_bf15LevelPerGoldValue)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf15LevelPerGoldValue[%d] Second : m_bf15LevelPerGoldValue[%d]!!\r\n",lpFirstShop->m_bf15LevelPerGoldValue, lpSecondShop->m_bf15LevelPerGoldValue));
			++iNotSameShopManagerCount;
		}
		if(lpFirstShop->m_bf1IsNotBuyItem != lpSecondShop->m_bf1IsNotBuyItem)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsNotBuyItem[%d] Second : m_bf1IsNotBuyItem[%d]!!\r\n",lpFirstShop->m_bf1IsNotBuyItem, lpSecondShop->m_bf1IsNotBuyItem));
			++iNotSameShopManagerCount;
		}
		if(lpFirstShop->m_dwLimitPrice != lpSecondShop->m_dwLimitPrice)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_dwLimitPrice[%d] Second : m_dwLimitPrice[%d]!!\r\n",lpFirstShop->m_dwLimitPrice, lpSecondShop->m_dwLimitPrice));
			++iNotSameShopManagerCount;
		}
		if(lpFirstShop->m_TokkenShop != lpSecondShop->m_TokkenShop)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_TokkenShop[%d] Second : m_TokkenShop[%d]!!\r\n",lpFirstShop->m_TokkenShop, lpSecondShop->m_TokkenShop));
			++iNotSameShopManagerCount;
		}

		for(int j=0;j<dMAX_SHOP_ITEM_COUNT;++j)
		{

			CShopItem*	lpFirstShopItem		=	lpFirstShop->get(j);
			CShopItem*	lpSecondShopItem	=	lpSecondShop->get(j);
			if(!lpFirstShopItem || !lpSecondShopItem)
			{
				if((lpFirstShopItem && !lpSecondShopItem) || (!lpFirstShopItem && lpSecondShopItem))
				{
					addTextToEdit(IDC_RESULT, _ms("Not Same ShopItem In Map[%d][%d]",i,j));
					++iNotSameShopManagerCount;
				}
				continue;
			}
			if(!lpFirstShopItem->isSame(lpSecondShopItem))
			{
				addTextToEdit(IDC_RESULT, _ms("Not Same ShopItem In Map[%d][%d]",i,j));
				++iNotSameShopManagerCount;
			}

		}
	}


	addTextToEdit(IDC_STATUS, "Compare ShopManager End!!\r\n");
	return iNotSameShopManagerCount;
}

int
cCompareBox::compareCustomItemManagerInMap()
{
	addTextToEdit(IDC_STATUS, "Compare CustomItemManager Start!!\r\n");
	int iNotSameCustomItemManagerCount	=	0;

	for(int iCustomItem;iCustomItem<dMAX_CUSTOM_ITEM_COUNT;++iCustomItem)
	{
		CCustomItem	*lpFirstCustomItem	=	s_lpFirstMap->m_customItem.getItem(iCustomItem);
		CCustomItem	*lpSecondCustomItem	=	s_lpSecondMap->m_customItem.getItem(iCustomItem);


		if(!lpFirstCustomItem || !lpSecondCustomItem)
		{
			if((lpFirstCustomItem && !lpSecondCustomItem) || (!lpFirstCustomItem && lpSecondCustomItem))
			{
				addTextToEdit(IDC_RESULT, _ms("Not Same CustomItem In Map[%d]",iCustomItem));
				++iNotSameCustomItemManagerCount;
			}
			continue;
		}
		if(!lpFirstCustomItem->isSame(lpSecondCustomItem))
		{
			addTextToEdit(IDC_RESULT, _ms("Not Same CustomItem In Map[%d]",iCustomItem));
			++iNotSameCustomItemManagerCount;
		}
	}

	addTextToEdit(IDC_STATUS, "Compare CustomItemManager End!!\r\n");
	return iNotSameCustomItemManagerCount;
}

int
cCompareBox::comparePatrolRoadManagerInMap()
{
	addTextToEdit(IDC_STATUS, "Compare PatrolRoadManager Start!!\r\n");
	int iNotSamePatrolRoadManagerCount	=	0;

	for(int i=0;i<dMAX_PATROL_ROAD_COUNT;++i)
	{

		cPatrolRoad	*lpFirstPatrolRoad	=	s_lpFirstMap->m_prManager.get(i);
		cPatrolRoad	*lpSecondPatrolRoad	=	s_lpSecondMap->m_prManager.get(i);

		if(!lpFirstPatrolRoad || !lpSecondPatrolRoad)
		{
			if((lpFirstPatrolRoad && !lpSecondPatrolRoad) || (!lpFirstPatrolRoad && lpSecondPatrolRoad))
			{
				addTextToEdit(IDC_RESULT, _ms("Not Same PatrolRoadManager In Map[%d]",i));
				++iNotSamePatrolRoadManagerCount;
			}
			continue;
		}

		if(lpFirstPatrolRoad->m_wSerial != lpSecondPatrolRoad->m_wSerial)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wSerial[%d] Second : m_wSerial[%d]!!\r\n",lpFirstPatrolRoad->m_wSerial, lpSecondPatrolRoad->m_wSerial));
			++iNotSamePatrolRoadManagerCount;
		}
		if(lpFirstPatrolRoad->m_wStayTime != lpSecondPatrolRoad->m_wStayTime)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wStayTime[%d] Second : m_wStayTime[%d]!!\r\n",lpFirstPatrolRoad->m_wStayTime, lpSecondPatrolRoad->m_wStayTime));
			++iNotSamePatrolRoadManagerCount;
		}
		if(lpFirstPatrolRoad->m_wIsReturnRoad != lpSecondPatrolRoad->m_wIsReturnRoad)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wIsReturnRoad[%d] Second : m_wIsReturnRoad[%d]!!\r\n",lpFirstPatrolRoad->m_wIsReturnRoad, lpSecondPatrolRoad->m_wIsReturnRoad));
			++iNotSamePatrolRoadManagerCount;
		}
		if(lpFirstPatrolRoad->m_wNodeCount != lpSecondPatrolRoad->m_wNodeCount)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wNodeCount[%d] Second : m_wNodeCount[%d]!!\r\n",lpFirstPatrolRoad->m_wNodeCount, lpSecondPatrolRoad->m_wNodeCount));
			++iNotSamePatrolRoadManagerCount;
		}
		if(strcmp(lpFirstPatrolRoad->m_strName,lpSecondPatrolRoad->m_strName) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_strName[%s] Second : m_strName[%d]!!\r\n",lpFirstPatrolRoad->m_strName, lpSecondPatrolRoad->m_strName));
			++iNotSamePatrolRoadManagerCount;
		}
		
		for(int j=0;j<dMAX_PATROL_ROAD_NODE_COUNT;++j)
		{
			if(memcmp(&lpFirstPatrolRoad->m_aNode[j],&lpSecondPatrolRoad->m_aNode[j], sizeof(cPatrolRoadNode)) != 0)
			{
				addTextToEdit(IDC_RESULT, _ms("[%d] not Same m_aNode [%d]",i,j));
				++iNotSamePatrolRoadManagerCount;
			}
		}

	}

	addTextToEdit(IDC_STATUS, "Compare PatrolRoadManager End!!\r\n");
	return iNotSamePatrolRoadManagerCount;
}

int
cCompareBox::compareCharacterPatternPackInMap()
{
	addTextToEdit(IDC_STATUS, "Compare CharacterPatternPackInMap Start!!\r\n");

	int iNotSameCharacterPatternPackCount	=	0;

	if(s_lpFirstMap->m_cpp.m_wCount != s_lpSecondMap->m_cpp.m_wCount)
	{
		addTextToEdit(IDC_RESULT, _ms("First : m_wCount[%d] Second : m_wCount[%d]!!\r\n",s_lpFirstMap->m_cpp.m_wCount,s_lpSecondMap->m_cpp.m_wCount));
		++iNotSameCharacterPatternPackCount;
	}

	for(int i = 0;i<c_iCharacterPatternCountInField;++i)
	{
		if(s_lpFirstMap->m_cpp.m_aPatterns[i].m_wSerial != s_lpSecondMap->m_cpp.m_aPatterns[i].m_wSerial)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_aPatterns[%d].m_wSerial[%d] Second : m_aPatterns[%d].m_wSerial[%d]!!\r\n",i,s_lpFirstMap->m_cpp.m_aPatterns[i].m_wSerial,i,s_lpSecondMap->m_cpp.m_aPatterns[i].m_wSerial));
			++iNotSameCharacterPatternPackCount;
		}
		if(s_lpFirstMap->m_cpp.m_aPatterns[i].m_wCount != s_lpSecondMap->m_cpp.m_aPatterns[i].m_wCount)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_aPatterns[%d].m_wCount[%d] Second : m_aPatterns[%d].m_wCount[%d]!!\r\n",i,s_lpFirstMap->m_cpp.m_aPatterns[i].m_wCount,i,s_lpSecondMap->m_cpp.m_aPatterns[i].m_wCount));
			++iNotSameCharacterPatternPackCount;
		}
		if(strcmp(s_lpFirstMap->m_cpp.m_aPatterns[i].m_strName,s_lpSecondMap->m_cpp.m_aPatterns[i].m_strName) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_aPatterns[%d].m_strName[%s] Second : m_aPatterns[%d].m_strName[%s]!!\r\n",i,s_lpFirstMap->m_cpp.m_aPatterns[i].m_strName,i,s_lpSecondMap->m_cpp.m_aPatterns[i].m_strName));
			++iNotSameCharacterPatternPackCount;
		}

		for(int j=0;j<c_iCharacterPatternPieceCount;++j)
		{
			if(s_lpFirstMap->m_cpp.m_aPatterns[i].m_aPattern[j].m_wPattern != s_lpSecondMap->m_cpp.m_aPatterns[i].m_aPattern[j].m_wPattern)
			{
			addTextToEdit(IDC_RESULT, _ms("First : m_aPatterns[%d].m_aPattern[%d].m_wPattern[%d] Second : m_aPatterns[%d].m_aPattern[%d].m_wPattern[%d]!!\r\n",
				i,j,s_lpFirstMap->m_cpp.m_aPatterns[i].m_aPattern[j].m_wPattern,i,j,s_lpSecondMap->m_cpp.m_aPatterns[i].m_aPattern[j].m_wPattern));				
				++iNotSameCharacterPatternPackCount;
			}
			if(memcmp(s_lpFirstMap->m_cpp.m_aPatterns[i].m_aPattern[j].m_aiValue,s_lpSecondMap->m_cpp.m_aPatterns[i].m_aPattern[j].m_aiValue, sizeof(int) * c_iCharacterPatternValueCount) != 0)
			{
			addTextToEdit(IDC_RESULT, _ms("not Same m_aPatterns[%d].m_aPattern[%d].m_aiValue",i,j));						
				++iNotSameCharacterPatternPackCount;
			}
		}
	}
	addTextToEdit(IDC_STATUS, "Compare CharacterPatternPackInMap End!!\r\n");

	return iNotSameCharacterPatternPackCount;
}
int
cCompareBox::compareKarmaContainerInMap()
{
	int iNotSameKarmaContainerCount	=	0;

	if(!s_lpFirstMap->m_karma.isSame(&s_lpSecondMap->m_karma))
	{
			addTextToEdit(IDC_RESULT, "Not Same m_karma In Map");
		++iNotSameKarmaContainerCount;
	}

	return iNotSameKarmaContainerCount;
}

int
cCompareBox::compareBuildingInMap()
{
	addTextToEdit(IDC_STATUS, "Compare BuildingInMap Start!!\r\n");

	int iNotSameBuildingCount	=	0;

	for(int iBuildIndex	= 0;iBuildIndex<dMAX_BUILDING_COUNT_IN_MAP;++iBuildIndex)
	{
		cBUILDING_INFO	*lpFirstBuilding	=	s_lpFirstMap->getBuilding(iBuildIndex);
		cBUILDING_INFO	*lpSecondBuilding	=	s_lpSecondMap->getBuilding(iBuildIndex);


		if(!lpFirstBuilding || !lpSecondBuilding)
		{
			if((lpFirstBuilding && !lpSecondBuilding) || (!lpFirstBuilding && lpSecondBuilding))
			{
				addTextToEdit(IDC_RESULT, _ms("buildingInfo[%d]\r\n",iBuildIndex));	
				++iNotSameBuildingCount;
			}
			continue;
		}
		if(!lpFirstBuilding->isSame(lpSecondBuilding))
		{
			addTextToEdit(IDC_RESULT, _ms("buildingInfo[%d]\r\n",iBuildIndex));	
			++iNotSameBuildingCount;	
		}
	}

	addTextToEdit(IDC_STATUS, "Compare BuildingInMap End!!\r\n");

	return	iNotSameBuildingCount;
}

int
cCompareBox::compareAreaInMap()
{
	addTextToEdit(IDC_STATUS, "Compare AreaInMap Start!!\r\n");
	int iNotSameAreaCount	=	0;

	for(int iAreaIndex	= 0 ; iAreaIndex < dMAX_AREA ; ++iAreaIndex)
	{
		if(!s_lpFirstMap->m_pArea || !s_lpSecondMap->m_pArea)
		{
			if((s_lpFirstMap->m_pArea && !s_lpSecondMap->m_pArea) || (!s_lpFirstMap->m_pArea && s_lpSecondMap->m_pArea))
			{
				
				++iNotSameAreaCount;
				break;
			}
			break;
		}

		cAREA_INFO*	lpFirstAreaInfo		=	&s_lpFirstMap->m_pArea->m_aArea[iAreaIndex];
		cAREA_INFO*	lpSecondAreaInfo	=	&s_lpSecondMap->m_pArea->m_aArea[iAreaIndex];

		if(!lpFirstAreaInfo->isSame(lpSecondAreaInfo))
		{
			addTextToEdit(IDC_RESULT, _ms("m_pArea[%d]\r\n",iAreaIndex));	
			++iNotSameAreaCount;
		}
	}

	addTextToEdit(IDC_STATUS, "Compare AreaInMap End!!\r\n");
	return iNotSameAreaCount;
}


int
cCompareBox::compareCharacterSetInMap()
{
	addTextToEdit(IDC_STATUS, "Compare compareCharacterSetInMap Start!!\r\n");

	int iNotSameCharacterSetCount	=	0;

	for(int i=0;i<dMAX_CHARACTER;++i)
	{

		cCHARACTER* lpFirstCharacter	= &s_lpFirstMap->m_setCharacter.m_aCharacter[i];
		cCHARACTER* lpSecondCharacter	= &s_lpSecondMap->m_setCharacter.m_aCharacter[i];
		
		if(lpFirstCharacter->m_wSerial != lpSecondCharacter->m_wSerial)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wSerial[%d] Second : m_wSerial[%d]!!\r\n",lpFirstCharacter->m_wSerial,lpSecondCharacter->m_wSerial));
			++iNotSameCharacterSetCount;		
		}

		if(lpFirstCharacter->m_wBaseJob != lpSecondCharacter->m_wBaseJob)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wBaseJob[%d] Second : m_wBaseJob[%d]!!\r\n",lpFirstCharacter->m_wBaseJob,lpSecondCharacter->m_wBaseJob));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_wMinLevel != lpSecondCharacter->m_wMinLevel)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wMinLevel[%d] Second : m_wMinLevel[%d]!!\r\n",lpFirstCharacter->m_wMinLevel,lpSecondCharacter->m_wMinLevel));
			++iNotSameCharacterSetCount;		
		}
		if(strcmp(lpFirstCharacter->m_strName,lpSecondCharacter->m_strName) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_strName[%s] Second : m_strName[%s]!!\r\n",lpFirstCharacter->m_strName,lpSecondCharacter->m_strName));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_wCorrectExp != lpSecondCharacter->m_wCorrectExp)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wCorrectExp[%d] Second : m_wCorrectExp[%d]!!\r\n",lpFirstCharacter->m_wCorrectExp,lpSecondCharacter->m_wCorrectExp));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_wCorrectHP != lpSecondCharacter->m_wCorrectHP)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wCorrectHP[%d] Second : m_wCorrectHP[%d]!!\r\n",lpFirstCharacter->m_wCorrectHP,lpSecondCharacter->m_wCorrectHP));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_wCorrectAP != lpSecondCharacter->m_wCorrectAP)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wCorrectAP[%d] Second : m_wCorrectAP[%d]!!\r\n",lpFirstCharacter->m_wCorrectAP,lpSecondCharacter->m_wCorrectAP));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_wCorrectFinalDamage != lpSecondCharacter->m_wCorrectFinalDamage)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wCorrectFinalDamage[%d] Second : m_wCorrectFinalDamage[%d]!!\r\n",lpFirstCharacter->m_wCorrectFinalDamage,lpSecondCharacter->m_wCorrectFinalDamage));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_wScaleX != lpSecondCharacter->m_wScaleX)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wScaleX[%d] Second : m_wScaleX[%d]!!\r\n",lpFirstCharacter->m_wScaleX,lpSecondCharacter->m_wScaleX));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_wScaleY != lpSecondCharacter->m_wScaleY)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wScaleY[%d] Second : m_wScaleY[%d]!!\r\n",lpFirstCharacter->m_wScaleY,lpSecondCharacter->m_wScaleY));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_wMaxLevel != lpSecondCharacter->m_wMaxLevel)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wMaxLevel[%d] Second : m_wMaxLevel[%d]!!\r\n",lpFirstCharacter->m_wMaxLevel,lpSecondCharacter->m_wMaxLevel));
			++iNotSameCharacterSetCount;		
		}
		
		if(lpFirstCharacter->m_bf1IsInvincible != lpSecondCharacter->m_bf1IsInvincible)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsInvincible[%d] Second : m_bf1IsInvincible[%d]!!\r\n",lpFirstCharacter->m_bf1IsInvincible,lpSecondCharacter->m_bf1IsInvincible));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsNotDropGold != lpSecondCharacter->m_bf1IsNotDropGold)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsNotDropGold[%d] Second : m_bf1IsNotDropGold[%d]!!\r\n",lpFirstCharacter->m_bf1IsNotDropGold,lpSecondCharacter->m_bf1IsNotDropGold));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsNotDropItem != lpSecondCharacter->m_bf1IsNotDropItem)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsNotDropItem[%d] Second : m_bf1IsNotDropItem[%d]!!\r\n",lpFirstCharacter->m_bf1IsNotDropItem,lpSecondCharacter->m_bf1IsNotDropItem));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsNotExp != lpSecondCharacter->m_bf1IsNotExp)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsNotExp[%d] Second : m_bf1IsNotExp[%d]!!\r\n",lpFirstCharacter->m_bf1IsNotExp,lpSecondCharacter->m_bf1IsNotExp));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf8CorrectUseSkillChance != lpSecondCharacter->m_bf8CorrectUseSkillChance)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf8CorrectUseSkillChance[%d] Second : m_bf8CorrectUseSkillChance[%d]!!\r\n",lpFirstCharacter->m_bf8CorrectUseSkillChance,lpSecondCharacter->m_bf8CorrectUseSkillChance));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsKeyMonster != lpSecondCharacter->m_bf1IsKeyMonster)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsKeyMonster[%d] Second : m_bf1IsKeyMonster[%d]!!\r\n",lpFirstCharacter->m_bf1IsKeyMonster,lpSecondCharacter->m_bf1IsKeyMonster));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf8Skin != lpSecondCharacter->m_bf8Skin)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf8Skin[%d] Second : m_bf8Skin[%d]!!\r\n",lpFirstCharacter->m_bf8Skin,lpSecondCharacter->m_bf8Skin));
			++iNotSameCharacterSetCount;		
		}
		
		if(lpFirstCharacter->m_bf1IsImmuneStun != lpSecondCharacter->m_bf1IsImmuneStun)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsImmuneStun[%d] Second : m_bf1IsImmuneStun[%d]!!\r\n",lpFirstCharacter->m_bf1IsImmuneStun,lpSecondCharacter->m_bf1IsImmuneStun));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsImmuneStone != lpSecondCharacter->m_bf1IsImmuneStone)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsImmuneStone[%d] Second : m_bf1IsImmuneStone[%d]!!\r\n",lpFirstCharacter->m_bf1IsImmuneStone,lpSecondCharacter->m_bf1IsImmuneStone));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsImmuneCharming != lpSecondCharacter->m_bf1IsImmuneCharming)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsImmuneCharming[%d] Second : m_bf1IsImmuneCharming[%d]!!\r\n",lpFirstCharacter->m_bf1IsImmuneCharming,lpSecondCharacter->m_bf1IsImmuneCharming));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsImmuneFreeze != lpSecondCharacter->m_bf1IsImmuneFreeze)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsImmuneFreeze[%d] Second : m_bf1IsImmuneFreeze[%d]!!\r\n",lpFirstCharacter->m_bf1IsImmuneFreeze,lpSecondCharacter->m_bf1IsImmuneFreeze));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsImmuneKnockBack != lpSecondCharacter->m_bf1IsImmuneKnockBack)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsImmuneKnockBack[%d] Second : m_bf1IsImmuneKnockBack[%d]!!\r\n",lpFirstCharacter->m_bf1IsImmuneKnockBack,lpSecondCharacter->m_bf1IsImmuneKnockBack));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsReviseIntelligence != lpSecondCharacter->m_bf1IsReviseIntelligence)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsReviseIntelligence[%d] Second : m_bf1IsReviseIntelligence[%d]!!\r\n",lpFirstCharacter->m_bf1IsReviseIntelligence,lpSecondCharacter->m_bf1IsReviseIntelligence));
			++iNotSameCharacterSetCount;		
		}
		
		if(lpFirstCharacter->m_bf1IsImmunePhysicalDamage != lpSecondCharacter->m_bf1IsImmunePhysicalDamage)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsImmunePhysicalDamage[%d] Second : m_bf1IsImmunePhysicalDamage[%d]!!\r\n",lpFirstCharacter->m_bf1IsImmunePhysicalDamage,lpSecondCharacter->m_bf1IsImmunePhysicalDamage));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsImmuneMagicDamage != lpSecondCharacter->m_bf1IsImmuneMagicDamage)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsImmuneMagicDamage[%d] Second : m_bf1IsImmuneMagicDamage[%d]!!\r\n",lpFirstCharacter->m_bf1IsImmuneMagicDamage,lpSecondCharacter->m_bf1IsImmuneMagicDamage));
			++iNotSameCharacterSetCount;		
		}
		
		if(lpFirstCharacter->m_bf1IsImmuneConfuse != lpSecondCharacter->m_bf1IsImmuneConfuse)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsImmuneConfuse[%d] Second : m_bf1IsImmuneConfuse[%d]!!\r\n",lpFirstCharacter->m_bf1IsImmuneConfuse,lpSecondCharacter->m_bf1IsImmuneConfuse));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsSetBoss != lpSecondCharacter->m_bf1IsSetBoss)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsSetBoss[%d] Second : m_bf1IsSetBoss[%d]!!\r\n",lpFirstCharacter->m_bf1IsSetBoss,lpSecondCharacter->m_bf1IsSetBoss));
			++iNotSameCharacterSetCount;		
		}
		if(lpFirstCharacter->m_bf1IsBlockToTame != lpSecondCharacter->m_bf1IsBlockToTame)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsBlockToTame[%d] Second : m_bf1IsBlockToTame[%d]!!\r\n",lpFirstCharacter->m_bf1IsBlockToTame,lpSecondCharacter->m_bf1IsBlockToTame));
			++iNotSameCharacterSetCount;		
		}
		
		if(memcmp(lpFirstCharacter->m_asAbsorbMagicDamage,lpSecondCharacter->m_asAbsorbMagicDamage,sizeof(short)*7*2) != 0)
		{
			addTextToEdit(IDC_RESULT, "First : m_asAbsorbMagicDamage Second : m_asAbsorbMagicDamage!!\r\n");
			++iNotSameCharacterSetCount;		
		}

		for(int iEvent=0;iEvent<eMECC_COUNT;++iEvent)
		{

			CKarmaContainer	*lpFirstkc	= lpFirstCharacter->m_pEvent[iEvent];
			CKarmaContainer	*lpSecondkc = lpSecondCharacter->m_pEvent[iEvent];

			if(!lpFirstkc || !lpSecondkc)
			{
				if(!lpFirstkc && lpSecondkc || lpFirstkc && !lpSecondkc)	
				{
					addTextToEdit(IDC_RESULT, _ms("Event[%d]!!\r\n",iEvent));
					++iNotSameCharacterSetCount;	
				}
			
				continue;
			}
			if(!lpFirstkc->isSame(lpSecondkc))
			{
				addTextToEdit(IDC_RESULT, _ms("Event[%s][%d]!!\r\n",lpFirstkc->m_pstrTitle,iEvent));
				++iNotSameCharacterSetCount;
			}
		}
		
	}
	
	addTextToEdit(IDC_STATUS, "Compare compareCharacterSetInMap End!!\r\n");
	return iNotSameCharacterSetCount;
}


int
cCompareBox::compareActorSetInMap()
{

	addTextToEdit(IDC_STATUS, "Compare compareActorSetInMap Start!!\r\n");

	int iNotSameActorSetCount	=	0;

	for(int i=0;i<dMAX_ACTOR_COUNT;++i)
	{
		cACTOR* lpFirstActor	= s_lpFirstMap->m_setActor.getActor(i);
		cACTOR* lpSecondActor	= s_lpSecondMap->m_setActor.getActor(i);
		if(!lpFirstActor || !lpSecondActor)
		{
			if(!lpFirstActor && lpSecondActor || lpFirstActor && !lpSecondActor)	
			{
				addTextToEdit(IDC_RESULT, _ms("Actor[%d]!!\r\n",i));
				++iNotSameActorSetCount;	
			}
			
			continue;
		}

		if(lpFirstActor->m_iSerial != lpSecondActor->m_iSerial)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_iSerial[%d] Second : m_iSerial[%d]!!\r\n",lpFirstActor->m_iSerial,lpSecondActor->m_iSerial));
			++iNotSameActorSetCount;
		}
			
		if(lpFirstActor->m_wCharacter != lpSecondActor->m_wCharacter)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wCharacter[%d] Second : m_wCharacter[%d]!!\r\n",lpFirstActor->m_wCharacter,lpSecondActor->m_wCharacter));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wType != lpSecondActor->m_wType)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wType[%d] Second : m_wType[%d]!!\r\n",lpFirstActor->m_wType,lpSecondActor->m_wType));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wDirect != lpSecondActor->m_wDirect)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wDirect[%d] Second : m_wDirect[%d]!!\r\n",lpFirstActor->m_wDirect,lpSecondActor->m_wDirect));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_isChangeDirect != lpSecondActor->m_isChangeDirect)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_isChangeDirect[%d] Second : m_isChangeDirect[%d]!!\r\n",lpFirstActor->m_isChangeDirect,lpSecondActor->m_isChangeDirect));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_dwRegenerationCycle != lpSecondActor->m_dwRegenerationCycle)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_dwRegenerationCycle[%d] Second : m_dwRegenerationCycle[%d]!!\r\n",lpFirstActor->m_dwRegenerationCycle,lpSecondActor->m_dwRegenerationCycle));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wMoveArea != lpSecondActor->m_wMoveArea)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wMoveArea[%d] Second : m_wMoveArea[%d]!!\r\n",lpFirstActor->m_wMoveArea,lpSecondActor->m_wMoveArea));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wWanderCycle != lpSecondActor->m_wWanderCycle)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wWanderCycle[%d] Second : m_wWanderCycle[%d]!!\r\n",lpFirstActor->m_wWanderCycle,lpSecondActor->m_wWanderCycle));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wPatrolRoute != lpSecondActor->m_wPatrolRoute)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wPatrolRoute[%d] Second : m_wPatrolRoute[%d]!!\r\n",lpFirstActor->m_wPatrolRoute,lpSecondActor->m_wPatrolRoute));
			++iNotSameActorSetCount;
		}

		if(lpFirstActor->m_wBattleInclination != lpSecondActor->m_wBattleInclination)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wBattleInclination[%d] Second : m_wBattleInclination[%d]!!\r\n",lpFirstActor->m_wBattleInclination,lpSecondActor->m_wBattleInclination));
			++iNotSameActorSetCount;
		}

		if(lpFirstActor->m_bf1IsLeader != lpSecondActor->m_bf1IsLeader)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsLeader[%d] Second : m_bf1IsLeader[%d]!!\r\n",lpFirstActor->m_bf1IsLeader,lpSecondActor->m_bf1IsLeader));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_bf1IsGuildCrest != lpSecondActor->m_bf1IsGuildCrest)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsGuildCrest[%d] Second : m_bf1IsGuildCrest[%d]!!\r\n",lpFirstActor->m_bf1IsGuildCrest,lpSecondActor->m_bf1IsGuildCrest));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_bf1IsGuildObject != lpSecondActor->m_bf1IsGuildObject)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsGuildObject[%d] Second : m_bf1IsGuildObject[%d]!!\r\n",lpFirstActor->m_bf1IsGuildObject,lpSecondActor->m_bf1IsGuildObject));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_bf1IsHideName != lpSecondActor->m_bf1IsHideName)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_bf1IsHideName[%d] Second : m_bf1IsHideName[%d]!!\r\n",lpFirstActor->m_bf1IsHideName,lpSecondActor->m_bf1IsHideName));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_isWard != lpSecondActor->m_isWard)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_isWard[%d] Second : m_isWard[%d]!!\r\n",lpFirstActor->m_isWard,lpSecondActor->m_isWard));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wLinkMonsterCount != lpSecondActor->m_wLinkMonsterCount)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wLinkMonsterCount[%d] Second : m_wLinkMonsterCount[%d]!!\r\n",lpFirstActor->m_wLinkMonsterCount,lpSecondActor->m_wLinkMonsterCount));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wLinkType != lpSecondActor->m_wLinkType)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wLinkType[%d] Second : m_wLinkType[%d]!!\r\n",lpFirstActor->m_wLinkType,lpSecondActor->m_wLinkType));
			++iNotSameActorSetCount;
		}
		if(memcmp(lpFirstActor->m_awLinkMonster,lpSecondActor->m_awLinkMonster,sizeof(WORD) * dMAX_LINK_MONSTER_COUNT) != 0)
		{
			addTextToEdit(IDC_RESULT, "First : m_awLinkMonster Second : m_awLinkMonster!!\r\n");
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wGenerateAroundPosX1 != lpSecondActor->m_wGenerateAroundPosX1)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wGenerateAroundPosX1[%d] Second : m_wGenerateAroundPosX1[%d]!!\r\n",lpFirstActor->m_wGenerateAroundPosX1,lpSecondActor->m_wGenerateAroundPosX1));
			++iNotSameActorSetCount;
		}						
		if(lpFirstActor->m_wGenerateAroundPosY1 != lpSecondActor->m_wGenerateAroundPosY1)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wGenerateAroundPosY1[%d] Second : m_wGenerateAroundPosY1[%d]!!\r\n",lpFirstActor->m_wGenerateAroundPosY1,lpSecondActor->m_wGenerateAroundPosY1));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wGenerateAroundPosX2 != lpSecondActor->m_wGenerateAroundPosX2)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wGenerateAroundPosX2[%d] Second : m_wGenerateAroundPosX2[%d]!!\r\n",lpFirstActor->m_wGenerateAroundPosX2,lpSecondActor->m_wGenerateAroundPosX2));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wGenerateAroundPosY2 != lpSecondActor->m_wGenerateAroundPosY2)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wGenerateAroundPosY2[%d] Second : m_wGenerateAroundPosY2[%d]!!\r\n",lpFirstActor->m_wGenerateAroundPosY2,lpSecondActor->m_wGenerateAroundPosY2));
			++iNotSameActorSetCount;
		}
		if(lpFirstActor->m_wGenerateMethod != lpSecondActor->m_wGenerateMethod)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wGenerateMethod[%d] Second : m_wGenerateMethod[%d]!!\r\n",lpFirstActor->m_wGenerateMethod,lpSecondActor->m_wGenerateMethod));
			++iNotSameActorSetCount;
		}

		if(!lpFirstActor->m_pos.isSame(&lpSecondActor->m_pos))
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_pos[%d %d] Second : m_pos[%d %d]!!\r\n",lpFirstActor->m_pos.x,lpFirstActor->m_pos.y,lpSecondActor->m_pos.x,lpSecondActor->m_pos.y));
			++iNotSameActorSetCount;
		}

		if(strcmp(lpFirstActor->m_strName,lpSecondActor->m_strName) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_strName[%s] Second : m_strName[%s]!!\r\n",lpFirstActor->m_strName,lpSecondActor->m_strName));
			++iNotSameActorSetCount;
		}

		if(lpFirstActor->m_wIsBlockToAutoRegen != lpSecondActor->m_wIsBlockToAutoRegen)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wIsBlockToAutoRegen[%d] Second : m_wIsBlockToAutoRegen[%d]!!\r\n",lpFirstActor->m_wIsBlockToAutoRegen,lpSecondActor->m_wIsBlockToAutoRegen));
			++iNotSameActorSetCount;
		}

		if(lpFirstActor->m_wJob != lpSecondActor->m_wJob)
		{
			addTextToEdit(IDC_RESULT, _ms("First : m_wJob[%d] Second : m_wJob[%d]!!\r\n",lpFirstActor->m_wJob,lpSecondActor->m_wJob));
			++iNotSameActorSetCount;
		}

		if(!lpFirstActor->m_pCC || !lpSecondActor->m_pCC)
		{
			if(!lpFirstActor->m_pCC && lpSecondActor->m_pCC || lpFirstActor->m_pCC&& !lpSecondActor->m_pCC)	
			{
				addTextToEdit(IDC_RESULT, "Not Same ConversationContainer!!\r\n");
				++iNotSameActorSetCount;	
			}
		}
		else
		{
			if(!lpFirstActor->m_pCC->isSame(lpSecondActor->m_pCC))
			{
				addTextToEdit(IDC_RESULT, "Not Same ConversationContainer!!\r\n");
				++iNotSameActorSetCount;	
			}
		}
	}

	addTextToEdit(IDC_STATUS, "Compare compareActorSetInMap End!!\r\n");

	return iNotSameActorSetCount;
}


void
cCompareBox::compareMapFile()
{

	if(!allocMapDataMemory())
	{
		killMapDataMamory();
		return;
	}

	addTextToEdit(IDC_STATUS, "allocMapDataMemory End!!\r\n");
	

	addTextToEdit(IDC_STATUS, "FisrtMapLoad Start!!\r\n");

	if(!cMAP::Load(s_strSelectedFirstFilePath,0,s_lpFirstMap))
	{
		killMapDataMamory();
		return;
	}

	addTextToEdit(IDC_STATUS, "FisrtMapLoad End!!\r\n");
	

	addTextToEdit(IDC_STATUS, "SecondMapLoad Start!!\r\n");	

	if(!cMAP::Load(s_strSelectedSecondFilePath,0,s_lpSecondMap))
	{
		killMapDataMamory();
		return;
	}

	addTextToEdit(IDC_STATUS, "SecondMapLoad End!!\r\n");

	compareMapBasicInfo();
	compareMapBaseInfo20031111();
	compareMapBaseInfo20050527();
	compareMapInfo();
	compareMap();




	killMapDataMamory();

}


void
cCompareBox::compareRPDFile()
{
	/*
	CQuest *_lpQuest
	
	CQuest *_lpArbeit
	
	cSecretDungeonManager *_lpSDM
	
	cChapterManager *_lpCM
	
	cBookManager *_lpBM
	
	CKarmaContainer	*_lpKC
	
	CKarmaContainer *_lpKCMopnsterGen

	cPROJECT::LoadProject(
*/
}

BOOL
cCompareBox::allocMapDataMemory()
{
	addTextToEdit(IDC_STATUS, "allocMapDataMemory Start!!\r\n");

	s_lpFirstMap	= new cMAP;
	s_lpSecondMap	= new cMAP;

	BOOL bResult	=	TRUE;

	if(	!s_lpFirstMap || !s_lpSecondMap)
		bResult =  FALSE;

	addTextToEdit(IDC_STATUS, "allocMapDataMemory End!!\r\n");

	return bResult;
}

void
cCompareBox::killMapDataMamory()
{
	addTextToEdit(IDC_STATUS, "MapDataMemoryKill Start!!\r\n");	

	if(s_lpFirstMap)
		s_lpFirstMap->Reset();

	if(s_lpSecondMap)
		s_lpSecondMap->Reset();

	KILL(s_lpFirstMap);
	KILL(s_lpSecondMap);

	addTextToEdit(IDC_STATUS, "MapDataMemoryKill End!!\r\n");
}


BOOL
cCompareBox::allocItemDataMemory()
{
	addTextToEdit(IDC_STATUS, "allocItemDataMemory Start!!\r\n");
	

	BOOL	bIsResult			=	TRUE;

	
	s_lpFirstBasicItem			=	new 	cBASIC_ITEM[dBASIC_ITEM_COUNT];
	
	s_lpFirstPremiumItem		=	new		CCustomItemManager;
	s_lpFirstCustomItem			=	new		CCustomItemManager;
	s_lpFirstItemPrefixManager	=	new		cITEM_PREFIX_MANAGER;
	s_lpFirstArcaManager		=	new		cArcaManager;
	s_lpFirstItemSetManager		=	new		cItemSetManager;
	s_lpFirstPieceItemManager	=	new		cPieceItemManager;
	
	s_lpSecondBasicItem			=	new 	cBASIC_ITEM[dBASIC_ITEM_COUNT];
	
	s_lpSecondPremiumItem		=	new		CCustomItemManager;
	s_lpSecondCustomItem		=	new		CCustomItemManager;
	s_lpSecondItemPrefixManager	=	new		cITEM_PREFIX_MANAGER;
	s_lpSecondArcaManager		=	new		cArcaManager;
	s_lpSecondItemSetManager	=	new		cItemSetManager;
	s_lpSecondPieceItemManager	=	new		cPieceItemManager;


	if	(!s_lpFirstBasicItem		||	
		!s_lpFirstPremiumItem		||	
		!s_lpFirstCustomItem		||		
		!s_lpFirstItemPrefixManager	||
		!s_lpFirstArcaManager		||
		!s_lpFirstItemSetManager	||	
		!s_lpFirstPieceItemManager	||
		!s_lpSecondBasicItem		||	
		!s_lpSecondPremiumItem		||
		!s_lpSecondCustomItem		||
		!s_lpSecondItemPrefixManager||	
		!s_lpSecondArcaManager		||
		!s_lpSecondItemSetManager	||
		!s_lpSecondPieceItemManager)
	{
		bIsResult = FALSE;
	}
		
	addTextToEdit(IDC_STATUS, "allocItemDataMemory End!!\r\n");

	return bIsResult;
}
void
cCompareBox::killItemDataMamory()
{
	addTextToEdit(IDC_STATUS, "killItemDataMamory Start!!\r\n");

	pKILL(s_lpFirstBasicItem);
	
	KILL(s_lpFirstPremiumItem);
	KILL(s_lpFirstCustomItem);		
	KILL(s_lpFirstItemPrefixManager);
	KILL(s_lpFirstArcaManager);	
	KILL(s_lpFirstItemSetManager);	
	KILL(s_lpFirstPieceItemManager);
	
	pKILL(s_lpSecondBasicItem);		
	
	KILL(s_lpSecondPremiumItem);	
	KILL(s_lpSecondCustomItem);		
	KILL(s_lpSecondItemPrefixManager);
	KILL(s_lpSecondArcaManager);	
	KILL(s_lpSecondItemSetManager);
	KILL(s_lpSecondPieceItemManager);

	addTextToEdit(IDC_STATUS, "killItemDataMamory End!!\r\n");
}


int
cCompareBox::compareBasicItem()
{
	addTextToEdit(IDC_STATUS, "BasicItem Compare Start!!\r\n");

	int iNotSameBasicItemCount	=	0;

	for(int iBIIndex=0 ; iBIIndex < dBASIC_ITEM_COUNT ; ++iBIIndex)
	{
		if(!s_lpFirstBasicItem[iBIIndex].isSame(&s_lpSecondBasicItem[iBIIndex]))
		{
			addTextToEdit(IDC_RESULT, _ms("BasicItem[%d]\r\n",iBIIndex));
			++iNotSameBasicItemCount;
		}
	}

	addTextToEdit(IDC_STATUS, "BasicItem Compare End!!\r\n");

	return iNotSameBasicItemCount;
}



int
cCompareBox::comparePremiumItem()
{
	addTextToEdit(IDC_STATUS, "PremiumItem Compare Start!!\r\n");

	int iNotSamePremiumItemCount	= 0;

	for(int iPIIndex = 0 ; iPIIndex<dMAX_INIT_CUSTOM_ITEM_COUNT ; ++iPIIndex )
	{
		CCustomItem* lpFirstPremiumItem		=	s_lpFirstPremiumItem->getItem(iPIIndex);
		CCustomItem* lpSecondPremiumItem	=	s_lpSecondPremiumItem->getItem(iPIIndex);

		if(!lpFirstPremiumItem || !lpSecondPremiumItem)
		{
			if((lpFirstPremiumItem && !lpSecondPremiumItem) || (!lpFirstPremiumItem && lpSecondPremiumItem))
			{
				addTextToEdit(IDC_RESULT, _ms("PremiumItem[%d]\r\n",iPIIndex));		
				++iNotSamePremiumItemCount;
			}
			continue;
		}

		if(!lpFirstPremiumItem->isSame(lpSecondPremiumItem))
		{
			addTextToEdit(IDC_RESULT, _ms("PremiumItem[%d]\r\n",iPIIndex));		
			++iNotSamePremiumItemCount;
		}
	}
	
	addTextToEdit(IDC_STATUS, "PremiumItem Compare End!!\r\n");


	return iNotSamePremiumItemCount;
}



int
cCompareBox::compareCustomItem()
{
	addTextToEdit(IDC_STATUS, "CustomItem Compare Start!!\r\n");

	int iNotSameCustomItemCount		= 0;

	for(int iCIIndex = 0 ; iCIIndex<dMAX_INIT_CUSTOM_ITEM_COUNT ; ++iCIIndex )
	{
		CCustomItem* lpFirstCustomItem		=	s_lpFirstCustomItem->getItem(iCIIndex);
		CCustomItem* lpSecondCustomItem		=	s_lpSecondCustomItem->getItem(iCIIndex);

		if(!lpFirstCustomItem || !lpSecondCustomItem)
		{
			if((lpFirstCustomItem && !lpSecondCustomItem) || (!lpFirstCustomItem && lpSecondCustomItem))
			{
				addTextToEdit(IDC_RESULT, _ms("CustomItem[%d]\r\n",iCIIndex));		
				++iNotSameCustomItemCount;
			}
			continue;
		}
		if(!lpFirstCustomItem->isSame(lpSecondCustomItem))
		{
			addTextToEdit(IDC_RESULT, _ms("CustomItem[%d]\r\n",iCIIndex));		
			++iNotSameCustomItemCount;
		}
	}
	
	addTextToEdit(IDC_STATUS, "CustomItem Compare End!!\r\n");

	return iNotSameCustomItemCount;
}


int
cCompareBox::compareItemPrefix()
{

	addTextToEdit(IDC_STATUS, "ItemPrefix Compare Start!!\r\n");

	int iNotSameItemPrefixCount		= 0;

	for(int iIPIndex = 0 ; iIPIndex < dMAX_ITEM_PREFIX_COUNT ; ++iIPIndex)
	{
		cITEM_PREFIX* lpFirstItemPrefix		=	s_lpFirstItemPrefixManager->get(iIPIndex);
		cITEM_PREFIX* lpSecondItemPrefix	=	s_lpSecondItemPrefixManager->get(iIPIndex);

		if(!lpFirstItemPrefix || !lpSecondItemPrefix)
		{
			if((lpFirstItemPrefix && !lpSecondItemPrefix) || (!lpFirstItemPrefix && lpSecondItemPrefix))
			{
				addTextToEdit(IDC_RESULT, _ms("ItemPrefix[%d]\r\n",iIPIndex));		
				++iNotSameItemPrefixCount;
			}
			continue;
		}

		if(memcmp(lpFirstItemPrefix,lpSecondItemPrefix	,sizeof(cITEM_PREFIX)) != 0)
		{
			addTextToEdit(IDC_RESULT, _ms("Prefix[%d]\r\n",iIPIndex));		
			++iNotSameItemPrefixCount;
		}
			
	}
	addTextToEdit(IDC_STATUS, "ItemPrefix Compare End!!\r\n");

	return iNotSameItemPrefixCount;

}

int
cCompareBox::compareArca()
{

	addTextToEdit(IDC_STATUS, "Arca Compare Start!!\r\n");

	int iNotSameArcaCount			= 0;

	for(int iAIIndex = 0 ; iAIIndex < dMAX_ARCA_COUNT ; ++iAIIndex)
	{
		cArcaInfo* lpFirstItemArca	=	s_lpFirstArcaManager->get(iAIIndex);
		cArcaInfo* lpSecondItemArca	=	s_lpSecondArcaManager->get(iAIIndex);

		if(!lpFirstItemArca || !lpSecondItemArca)
		{
			if((lpFirstItemArca && !lpSecondItemArca) || (!lpFirstItemArca && lpSecondItemArca))
			{
				addTextToEdit(IDC_RESULT, _ms("ItemPrefix[%d]\r\n",iAIIndex));		
				++iNotSameArcaCount;
			}
			continue;
		}

		if(!lpFirstItemArca->isSame(lpSecondItemArca))
		{
			addTextToEdit(IDC_RESULT, _ms("Arca[%d]\r\n",iAIIndex));		
			++iNotSameArcaCount;
		}
			
	}

	addTextToEdit(IDC_STATUS, "Arca Compare End!!\r\n");

	return iNotSameArcaCount;

}

int
cCompareBox::compareItemSet()
{

	addTextToEdit(IDC_STATUS, "ItemSet Compare Start!!\r\n");

	int iNotSameItemSetCount		=	0;

	cItemSet* lpFirstItemSet		=	s_lpFirstItemSetManager->getBay();
	cItemSet* lpSecondItemSet		=	s_lpSecondItemSetManager->getBay();
	int		iItemSetMaxCount		=	s_lpFirstItemSetManager->getMaxCount();

	for(int iISIndex = 0 ; iISIndex < dMAX_SET_ITEM_GROUP_COUNT ; ++iISIndex)
	{
		if(!lpFirstItemSet || !lpSecondItemSet)
		{
			if((lpFirstItemSet && !lpSecondItemSet) || (!lpFirstItemSet && lpSecondItemSet))
			{
				addTextToEdit(IDC_RESULT, _ms("ItemSet Data Compare!!\r\n"));		
				++iNotSameItemSetCount;
			}
			break;
		}

		if(!lpFirstItemSet[iISIndex].isSame(&lpSecondItemSet[iISIndex] , FALSE))
		{
			addTextToEdit(IDC_RESULT, _ms("ItemSet[%d]\r\n",iISIndex));		
			++iNotSameItemSetCount;
		}
	}

	addTextToEdit(IDC_STATUS, "ItemSet Compare End!!\r\n");

	return iNotSameItemSetCount;
}


int
cCompareBox::comparePieceItem()
{

	addTextToEdit(IDC_STATUS, "PieceItem Compare Start!!\r\n");

	int iNotSamePieceItemCount		= 0;

	for(int iPIndex = 0 ; iPIndex < dMAX_PIECE_ITEM_COUNT ; ++iPIndex)
	{
		cPieceItem* lpFirstPieceItem	=	s_lpFirstPieceItemManager->get(iPIndex);
		cPieceItem* lpSecondPieceItem	=	s_lpSecondPieceItemManager->get(iPIndex);

		if(!lpFirstPieceItem || !lpSecondPieceItem)
		{
			if((lpFirstPieceItem && !lpSecondPieceItem) || (!lpFirstPieceItem && lpSecondPieceItem))
			{
				addTextToEdit(IDC_RESULT, _ms("PieceItem[%d]\r\n",iPIndex));		
				++iNotSamePieceItemCount;
			}
			continue;
		}

		if(!lpFirstPieceItem->isSame(lpSecondPieceItem))
		{
			addTextToEdit(IDC_RESULT, _ms("PieceItem[%d]\r\n",iPIndex));		
			++iNotSamePieceItemCount;
		}
			
	}
	addTextToEdit(IDC_STATUS, "PieceItem Compare End!!\r\n");

	return iNotSamePieceItemCount;
}

void
cCompareBox::compareItemFile()
{

	if(!allocItemDataMemory())
	{
		killItemDataMamory();
		addTextToEdit(IDC_STATUS, "allocItemDataMemory Fail!!\r\n");
		return;
	}


	addTextToEdit(IDC_STATUS, "FirstFile LoadItem Start!!\r\n");

	if	(!cPROJECT::LoadItem(s_strSelectedFirstFilePath,s_lpFirstPremiumItem,s_lpFirstCustomItem,s_lpFirstItemPrefixManager,s_lpFirstArcaManager,s_lpFirstItemSetManager,s_lpFirstPieceItemManager,s_lpFirstBasicItem))
	{
		killItemDataMamory();
		addTextToEdit(IDC_STATUS, "FirstFile LoadItem Fail!!\r\n");
		return;
	}

	addTextToEdit(IDC_STATUS, "FirstFile LoadItem End!!\r\n");

	addTextToEdit(IDC_STATUS, "SecondFile LoadItem Start!!\r\n");

	if	(!cPROJECT::LoadItem(s_strSelectedSecondFilePath,s_lpSecondPremiumItem,s_lpSecondCustomItem,s_lpSecondItemPrefixManager,s_lpSecondArcaManager,s_lpSecondItemSetManager,s_lpSecondPieceItemManager,s_lpSecondBasicItem))
	{
		killItemDataMamory();
		addTextToEdit(IDC_STATUS, "SecondFile LoadItem Fail!!\r\n");
		return;
	}

	addTextToEdit(IDC_STATUS, "SecondFile LoadItem End!!\r\n");
	
	int iNotSameBasicItemCount		= compareBasicItem();
	int iNotSamePremiumItemCount	= comparePremiumItem();
	int iNotSameCustomItemCount		= compareCustomItem();
	int iNotSameItemPrefixCount		= compareItemPrefix();
	int iNotSameArcaCount			= compareArca();
	int iNotSameItemSetCount		= compareItemSet();
	int iNotSamePieceItemCount		= comparePieceItem();

	killItemDataMamory();

	cMSG::Put("Result!",_ms("BasicItem[%d] PremiumItem[%d] CostomItem[%d] Prefix[%d] Arca[%d] ItemSet[%d] PieceItem[%d]",
		iNotSameBasicItemCount,iNotSamePremiumItemCount,iNotSameCustomItemCount,iNotSameItemPrefixCount,iNotSameArcaCount,iNotSameItemSetCount,iNotSamePieceItemCount));

}



void
cCompareBox::updateBox()
{
	
	SetDlgItemText(s_hWnd,IDC_SELECTED_FIRST_FILE_PATH	,s_strSelectedFirstFilePath);
	SetDlgItemText(s_hWnd,IDC_SELECTED_SECOND_FILE_PATH	,s_strSelectedSecondFilePath);

}


void
cCompareBox::cancelSelectData()
{
	reset();
	updateBox();
}

BOOL
cCompareBox::open(HWND _hWnd,HINSTANCE _hInstance)
{

	s_hInst		=	_hInstance;
	reset();

	if (DialogBox( s_hInst, "COMPARE_BOX",_hWnd,(DLGPROC)procDlg)==IDCANCEL)
		return	FALSE;

	return TRUE;
}

void
cCompareBox::selectFile(int _iSelectFileKind)
{
	//	비교할 파일을 선택한당!
	char	strFirstFileName[MAX_PATH]	=	"";
	char	strSecondFileName[MAX_PATH] =	"";

	char	strFilter[64]				=	"";
	char	strExt[8]					=	"";

	switch(_iSelectFileKind)
	{
	case	eCOMPARE_MAP_FILE:
		strcpy(strFirstFileName,"*.rmd");
		strcpy(strSecondFileName,"*.rmd");
		strcpy(strFilter,"red stone map file {*.rmd}\0*.rmd\0\0");
		strcpy(strExt,"rmd");
		break;
	case	eCOMPARE_RPD_FILE:
		strcpy(strFirstFileName,"*.rpd");
		strcpy(strSecondFileName,"*.rpd");
		strcpy(strFilter,"red stone rpd file {*.rpd}\0*.rpd\0\0");
		strcpy(strExt,"rpd");
		break;

	case	eCOMPARE_ITEM_FILE:
		strcpy(strFirstFileName,"*.dat");
		strcpy(strSecondFileName,"*.dat");
		strcpy(strFilter,"red stone item file {*.dat}\0*.dat\0\0");
		strcpy(strExt,"dat");	
		break;

	default:
		cMSG::Error("??????");
		return;
	}


	if (!cFILE::LoadBox(strFirstFileName,strFilter,strExt,NULL,
		"Select First File", OFN_ALLOWMULTISELECT))
	{
		return;
	}

	if (!cFILE::LoadBox(strSecondFileName,strFilter,strExt,NULL,
		"Select Second File", OFN_ALLOWMULTISELECT))
	{
		return;
	}

	s_iCompareKind	=	_iSelectFileKind;
	strcpy(s_strSelectedFirstFilePath,strFirstFileName);
	strcpy(s_strSelectedSecondFilePath,strSecondFileName);

}

LRESULT CALLBACK		
cCompareBox::procDlg(HWND _hWnd, UINT _uMassage, WPARAM _wParam, LPARAM _lParam)
{


	switch(_uMassage)
	{
	case WM_INITDIALOG	:
		{
			s_hWnd		=	_hWnd;
			SendDlgItemMessage(s_hWnd, IDC_STATUS, EM_LIMITTEXT, 0, 0);
			SendDlgItemMessage(s_hWnd, IDC_RESULT, EM_LIMITTEXT, 0, 0);
			updateBox();
			break;
		}
	case WM_COMMAND			:
		{
			switch( LOWORD( _wParam ) )
			{
			case	IDC_SELECT_MAP_FILE			:
				{
					selectFile(eCOMPARE_MAP_FILE);
					updateBox();
					break;
				}
			case	IDC_SELECT_ITEM_FILE		:
				{
					selectFile(eCOMPARE_ITEM_FILE);
					updateBox();
					break;
				}
			case	IDC_SELECT_RPD_FILE			:
				{
					selectFile(eCOMPARE_RPD_FILE);
					updateBox();
					break;
				}
			case	IDC_CANCEL_SELECT_DATA		:
				{
					cancelSelectData();
					updateBox();
					break;
				}
			case	IDC_COMPARE					:
				{
					compareFile();
					break;
				}
				
			case	IDC_EXIT					:
				{
					EndDialog(_hWnd,IDC_EXIT);
					break;
				}
			}
			break;
		}
	}
	
	return	(0L);
}
