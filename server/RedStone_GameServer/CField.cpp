#include	"cFIELD.H"
#include	"cGAME.H"
#include	"instance_guild_battle_manager.H"
#include	<time.H>
#include	<math.H>
#include	"CPitchmanShop.h"
#include	"debugCode.h"
#include	"secretDungeon.h"
#include	"cParty.h"
#include	"cProject.h"
#include	"field_common.h"
#include	"cEvent.h"
#include "GameDLL.H"

CFieldInfoManager	g_fieldInfo;

#define	dCHECK_PERIOD_FOR_TREASURE_MAP	15

int	cFIELD::s_iFileVersion	=	eVUI_CURRENT_VERSION;

cFIELD::cFIELD()
{
	int		i;

	m_pLocalPartition	=	NULL;
	m_pPartLinkInfo		=	NULL;
	m_pDoors			=	NULL;

	m_pWordActorInfo	=	NULL;
	m_pSortWordQuizActor=	NULL;
	m_pCPP				=	NULL;
	m_iDuelSerial		=	0xffffffff;

	m_pArea				=	new cArea ();
	
	for (i=0;i<dMAX_NPC_COUNT;i++)
		m_apNpcGenerateData[i]	=	NULL;

	reset();
}

cFIELD::~cFIELD()
{
	DeleteCriticalSection(&m_csPacketManager);
	DeleteCriticalSection(&m_csActorManager);

	KILL(m_pArea);

	close();
}

//
//	데이터 초기화
void
cFIELD::init()
{
	m_droppedItem.init();

	ZeroMemory(&m_aDuelTeam[0] , sizeof(m_aDuelTeam[0]));
	ZeroMemory(&m_aDuelTeam[1] , sizeof(m_aDuelTeam[1]));

	reset();
}

void
cFIELD::close()
{
	m_bIsReadyToClose	=	TRUE;
	int	i,iActor;

	for (i=0;i<dMAX_ARCA_COUNT_IN_FIELD;i++)
		if (m_awArcaList[i]	!=	0xffff)
			g_arcaManager.remove(m_awArcaList[i]);

	for (i=0;i<dMAX_TRAP_COUNT_IN_FIELD;i++)
		if (m_awTrapList[i]	!=	0xffff)
			g_pTrapManager->remove(m_awTrapList[i]);

	for (i=0;i<dMAX_BANNER_COUNT_IN_FIELD;i++)
		if	(m_awBannerList[i]	!=	0xffff)
			g_bannerManager.remove(m_awBannerList[i]);

	for (i=0;i<dTEAM_COUNT;i++)
	{
		for (iActor=m_aiActorCount[i];iActor;)
		{
			--iActor;

			cACTOR	*lpActor	=	m_alpActor[m_awActorList[i][iActor]];

			if	(!lpActor)
				continue;

			g_game.removeActor(lpActor->m_iZoneSerial);
		}
	}

	m_kc.reset();
	KILL(m_pCPP);
	
	pKILL(m_pLocalPartition);
	pKILL(m_pPartLinkInfo);
	pKILL(m_pMap);
	pKILL(m_pSortWordQuizActor);
	pKILL(m_pWordActorInfo);

	KILL(m_pDoors);


	for (i=0;i<dMAX_NPC_COUNT;i++)
		KILL(m_apNpcGenerateData[i]);
}

//
//	데이터 리셋
void
cFIELD::reset()
{
	int	i;

	for (i=0;i<dCHARACTER_COUNT		;i++)
		m_aCharacter[i].reset();

	for (i=0;i<dMAX_ACTOR_IN_FIELD	;i++)
		m_alpActor[i]	=	NULL;

	for (i=0;i<dMAX_NPC_COUNT		;i++)
		if (m_apNpcGenerateData[i])
			m_apNpcGenerateData[i]->reset();//필드에 몹이나 NPC 생성 정보 초기화

	m_wDuelMatchStep	=	eDUEL_MATCH_STEP_IDLE;

	memset(m_aiFieldValue,0,sizeof(m_aiFieldValue));

	m_queReceivePacket.reset();
	m_queSendPacket.reset();
	m_droppedItem.reset();
	m_shop.reset();
	m_customItemList.reset();
	m_portal.reset();

	memset(m_abIsReadyToRebirthActor,0,sizeof(m_abIsReadyToRebirthActor));

	memset(m_awArcaList,0xff,sizeof(m_awArcaList));
	m_wArcaCount					=	0;

	m_wHallOwnGuild					=	0xffff;
	m_bIs1LevelGuildHall			=	FALSE;
	m_bIsReadyToClose				=	FALSE;
	m_iRookieActor					=	0;
	m_iActorCount					=	0;
	m_iCharacterCount				=	0;
	m_iSerial						=	0xffffffff;
	m_dwBookedSecretDungeonKeyParty	=	0xffffffff;
	m_iUniqueSerial					=	-1;
	m_iFrameCounter					=	0;
	m_iUpdateTime					=	0;
	m_iUpdatePlayerIndex			=	0;
	m_iBookedRemoveActorCount		=	0;
	m_wIsVillage					=	FALSE;
	m_iRealPlayerCount				=	0;
	m_iReadyToRebirthActorCount		=	0;
	m_wTrapCount					=	0;
	m_iBookedReadyToRemoveActorCount=	0;
	m_bIsProcessEvent				=	FALSE;
	m_bIsBlockBattle				=	FALSE;
	m_wRemainMinuteForCheckSecretDungeonParty	=	1;
	m_wIsGBS_Field					=	FALSE;
	m_bIsProcessSiegeWarfare		=	FALSE;
	m_iRemainStategyPoint			=	0;
	
	m_wGuildBattleFinished			=	FALSE;
	m_wIsIncorrectGuildBattle		=	FALSE;
	m_bIsOperateGuildBattle			=	FALSE;
	m_wGateField					=	0xffff;
	m_wIsRegenMonster				=	TRUE;
	m_wCorrectExperience			=	100;
	m_wLinkSecretDungeon			=	0xffff;
	m_wRemainMinuteForRemoveSecretDungeonKeyCount	=	0;
	m_iLimitPlayerCount				=	dMAX_PLAYER_IN_FIELD;
	m_wWeather						=	eWEATHER_FINE;			// 날씨~~
	m_iWeatherSecond				=	0;		// 날씨 지속시간.
	m_wBannerCount					=	0;
	memset(m_awBannerList	,0xff,sizeof(m_awBannerList));

	m_oxQuizStatus.m_wStatus		=	eOIPS_SLEEP;
	m_oxQuizStatus.m_wQuestionCount	=	0;
	memset(m_aBattleGuild,0xff,sizeof(m_aBattleGuild));

	memset(m_aiActorCount,0,sizeof(m_aiActorCount));
	memset(m_aBattleGuild,0xff,sizeof(m_aBattleGuild));
	memset(m_awArcaList,0xff,sizeof(m_awArcaList));
	memset(m_awTrapList,0xff,sizeof(m_awTrapList));
}

//
//	_strHeader을 기준으로 이 맵 화일의 버젼을 얻어온다.
int
cFIELD::getMapDataVersion(char *_strHeader)
{
	s_iFileVersion	=	GetMapDataVersion(_strHeader);;

	return	s_iFileVersion;
}

BOOL
cFIELD::checkIncorrectInfo()
{
	for (int i=m_aiActorCount[0];i;--i)
	{
		cACTOR	*lpPlayer	=	m_alpActor[m_awActorList[0][i-1]];

		lpPlayer->checkIncorrectInfo();
	}

	return	TRUE;
}

//
//	카르마 저장하기
void
cFIELD::ReadKarmaContainer(cFILE *_lpFile,CKarmaContainer *_lpKC,BOOL _bIsDecodeKaramData)
{
	_lpFile->Read(&_lpKC->m_wKarmaCount,2);
	_lpFile->Read(&_lpKC->m_wTitleLength,2);

	_lpKC->m_wSerial	=	0xffff;

	if (_lpKC->m_wKarmaCount)
		_lpKC->m_pKarma		=	new CKarma	[_lpKC->m_wKarmaCount];

	if	(_lpKC->m_wTitleLength)
	{
		_lpKC->m_pstrTitle	=	new char	[_lpKC->m_wTitleLength];

		_lpFile->Read(_lpKC->m_pstrTitle,_lpKC->m_wTitleLength);
		
		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(_lpKC->m_pstrTitle,_lpKC->m_wTitleLength);
	}

	for (int iKarma=0;iKarma<_lpKC->m_wKarmaCount;iKarma++)
		ReadKarma(_lpFile,&_lpKC->m_pKarma[iKarma],_bIsDecodeKaramData);

	_lpKC->resetToFirstStatus();
}

//
//	카르마 저장하기
void
cFIELD::ReadKarma(cFILE *_lpFile,CKarma *_lpKarma,BOOL _bIsDecodeKaramData)
{
	int	i;

	if	(s_iFileVersion	>=	eVUI_26_ADD_DUNGEON_VALUE_AND_KARMA_VALUE)
		_lpFile->Read(_lpKarma,sizeof(CKarmaBasicInfo));
	else
	if	(s_iFileVersion	>=	eVUI_20_ADD_KARMA_OCCUR_CHANCE)
		_lpFile->Read(_lpKarma,sizeof(CKarmaBasicInfo)-4);
	else	
		_lpFile->Read(_lpKarma,sizeof(CKarmaBasicInfo)-2-4);

	if	(_bIsDecodeKaramData)
		cMessSign::DecodeDataBuffer2(_lpKarma,sizeof(CKarmaBasicInfo));

	_lpKarma->allocBuffer();

	if	(_lpKarma->m_wTitleLength	>	0)
	{
		_lpFile->Read(_lpKarma->m_pstrTitle,_lpKarma->m_wTitleLength);

		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(_lpKarma->m_pstrTitle,_lpKarma->m_wTitleLength);
	}

	for (i=0;i<_lpKarma->m_wTriggerCount;i++)
		ReadKarmaItem(_lpFile,&_lpKarma->m_pTrigger[i],_bIsDecodeKaramData);

	for (i=0;i<_lpKarma->m_wReactionCount;i++)
		ReadKarmaItem(_lpFile,&_lpKarma->m_pReaction[i],_bIsDecodeKaramData);
}

//
//	카르마 읽기
void
cFIELD::ReadKarmaItem(cFILE *_lpFile,CKarmaItem *_lpKarma,BOOL _bIsDecodeKaramData)
{
	_lpFile->Read(_lpKarma,sizeof(CKarmaItemBasicInfo));

	if	(_bIsDecodeKaramData)
		cMessSign::DecodeDataBuffer2(_lpKarma,sizeof(CKarmaItemBasicInfo));

	if	(_lpKarma->m_wStringSize	>	0)
	{
		_lpKarma->m_strString	=	new char [_lpKarma->m_wStringSize];

		_lpFile->Read(_lpKarma->m_strString,_lpKarma->m_wStringSize);

		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(_lpKarma->m_strString,_lpKarma->m_wStringSize);
	}
}

//
//	Npc의 대화 데이터 읽기
void
cFIELD::readCC(cFILE *_lpFile,CConversationContainer *_lpCC,int _iSpeechCount,BOOL _bIsDecodeKaramData)
{
	CConversationContainer	*lpDialog=	_lpCC;
	
	lpDialog->m_wConversationCount	=	_iSpeechCount;
	lpDialog->m_pConversation		=	new CConversation [_iSpeechCount];

	_lpFile->Read(&lpDialog->m_wDialogBoxSize,2);
	_lpFile->Read(&lpDialog->m_wIsRandomChoiceDialg,2);

	for (int j=0;j<lpDialog->m_wConversationCount;j++)
	{
		CConversation	*lpSpeech	=	&lpDialog->m_pConversation[j];
		
		_lpFile->Read(lpSpeech,sizeof(CConversationBasicInfo));//	기본 정보

		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(lpSpeech,sizeof(CConversationBasicInfo));

		lpSpeech->init();	//	메모리 할당

		_lpFile->Read(lpSpeech->m_pstrSpeech,lpSpeech->m_wSpeechLength);//대사 읽기

		if	(_bIsDecodeKaramData)
			cMessSign::DecodeDataBuffer2(lpSpeech->m_pstrSpeech,lpSpeech->m_wSpeechLength);

		for (int iTrigger=0;iTrigger<lpSpeech->m_wTriggerCount;iTrigger++)
			ReadKarmaItem(_lpFile,&lpSpeech->m_pTrigger[iTrigger],_bIsDecodeKaramData);

		for (int iContent=0;iContent<lpSpeech->m_wContentsCount;iContent++)
		{
			CKarmaContainer	*lpkc	=	&lpSpeech->m_pContents[iContent];

			ReadKarmaContainer(_lpFile,lpkc,_bIsDecodeKaramData);
		}
	}
}

//	길드 홀 로딩
BOOL
cFIELD::loadGuildHall(CGuild	*_lpGuild)
{
	int	iHallLevel		=	_lpGuild->m_wHallLevel;
	int	iShape			=	_lpGuild->m_wGuildHallShape;
	int	iUniqueSerial	=	m_iUniqueSerial;
	int	iSerial			=	m_iSerial;
	
	if	(iHallLevel	==	0)
		return	FALSE;

	close();

	char	strFieldName[512];

	sprintf(strFieldName,"%s/GH%d_%.2d.rmd",g_project.m_strMapFolder,iHallLevel,iShape);

	BOOL	bResult		=	load(strFieldName,iUniqueSerial);

	m_iSerial			=	iSerial;

	return	bResult;
}

//	길드 홀 로딩
BOOL
cFIELD::loadGuildHall(int _iHallLevel)
{
	int	iHallLevel		=	_iHallLevel;
	int	iShape			=	0;
	int	iUniqueSerial	=	m_iUniqueSerial;
	int	iSerial			=	m_iSerial;

	close();

	char	strFieldName[512];

	sprintf(strFieldName,"%s/GH%d_%.2d.rmd",g_project.m_strMapFolder,_iHallLevel,iShape);

	BOOL	bResult		=	load(strFieldName,iUniqueSerial);

	m_iSerial			=	iSerial;

	return	bResult;
}

//
//	맵 데이터를 읽어 들인다.
BOOL
cFIELD::load(char *fn,int _iUniqueSerial,int _iGateField,int _iDuelType)
{// 마지막 수정일 : 09.10.26
	m_wDuelType					=	_iDuelType;
	m_bIsWantPrintTimeKarmaInfo	=	FALSE;

//	몇가지 초기화
	{
		init();

		FPInitCS(&m_csPacketManager);	//	크리티컬 섹션 초기화
		FPInitCS(&m_csActorManager);	//	크리티컬 섹션 초기화

	}	//	몇가지 초기화

	int		iFileSize,i,j,iKarmaInfoLocate;
	char	strHeader[60];
	cFILE	file;

	if	(!file.Open(fn,"rb"))
		return ERRMSG("Error in cFIELD::Load","'%s' file not found",fn);

	if	(m_wIsVillage)
		m_tradeManager.init(100);	//	트레이드 관리자
	else
		m_tradeManager.init(10);	//	트레이드 관리자

	strncpy(m_strFileName,_exportFileName(fn),sizeof(m_strFileName)-2);

	if	(_iUniqueSerial	==	-1)
	{
		m_strFileName[5]	=	NULL;
		m_iUniqueSerial		=	atoi(m_strFileName+1);

		strncpy(m_strFileName,_exportFileName(fn),sizeof(m_strFileName)-2);
	}
	else
		m_iUniqueSerial	=	_iUniqueSerial;

	m_wGateField		=	_iGateField;

	file.Read(&iFileSize			,4);		//	화일 사이즈
	file.Read(&m_iTextDataLocate	,4);		//	텍스트 데이터의 위치
	file.Read(strHeader,56);				//	헤더
	file.Read(&iKarmaInfoLocate		,4);				//	헤더

	getMapDataVersion(strHeader);
//	reset();									//	데이터 초기화

	BOOL	bIsDecodeKaramData	=	FALSE;

	if	(s_iFileVersion	>=	eVVI_49_ENCRYT_MORE)
		bIsDecodeKaramData	=	TRUE;

////////////////////////////////////////////////////////////////////////////////////////////////
//	맵 정보 읽어옴	
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		cMAP_INFO	mapInfo;

		if (s_iFileVersion	>=	eVUI_30_ADD_CORECT_MONSTER_RESISTANCE)
			file.Read(&mapInfo,sizeof(cMAP_INFO));	//	cMAP_INFO 인자만 읽어 들인다.
		else
		{
			memset(&mapInfo,0,sizeof(mapInfo));
			file.Read(&mapInfo,sizeof(cMapInfo050527));	//	cMAP_INFO 인자만 읽어 들인다.
		}

		if	(s_iFileVersion	>=	eVVI_51_ENCRYT2)
		{
			int		iNation;
			
			file.Read(&iNation,4);				//	헤더
			cMessSign::SetDataEncodeTable(iNation);
		}
		else
			cMessSign::SetDataEncodeTable(-1);

		m_iWidth					=	mapInfo.m_iWidth;
		m_iHeight					=	mapInfo.m_iHeight;
		m_iPixelWidth				=	m_iWidth*dTILE_XS;
		m_iPixelHeight				=	m_iHeight*dTILE_YS;		//	맵의 가로 세로 사이즈(픽셸단위)
		m_wIsPremiumZone			=	mapInfo.m_bf1IsPremiumZone;
		m_bIsGuildBattleField		=	mapInfo.m_bf1IsGuildBattleZone;
		m_wIsGuildBattleLobby		=	mapInfo.m_bf1IsGuildBattleLobby;
		m_wIsCanNotMemoryField		=	mapInfo.m_bf1IsCanNotMemoryField;
		m_wIsOXQuizField			=	mapInfo.m_bf1IsOXQuizField;
		m_wIsOXQuizVillage			=	mapInfo.m_bf1IsOXQuizGateVillage;
		m_wIsWordQuizField			=	mapInfo.m_bf1IsWordQuizField;
		m_wIsWordQuizVillage		=	mapInfo.m_bf1IsWordQuizVillage;
		m_wFieldType				=	mapInfo.m_bf4FieldType;
		m_wMinimapDisplayLevel		=	mapInfo.m_bf4MinimapDisplayLevel;	//	미니맵 표시 레벨
		m_wIsBlockToWarpField		=	mapInfo.m_bf1IsBlockToWarpField;

		if	(m_wFieldType	==	eFT_VILLAGE)
			m_wIsVillage			=	TRUE;

		if	(m_strFileName[5] == 'T')
			if	(strlen(m_strFileName) == 12)	
				m_wIsVillage			=	TRUE;

		m_wTileSet					=	mapInfo.m_iTileSet;

		m_wLinkSecretDungeon		=	mapInfo.m_wLinkSecretDungeon;
		m_wIsExistSecretDungeonGate	=	mapInfo.m_bf1IsExistSecretDungeonGate;
		m_wIsBossZone				=	mapInfo.m_bf1IsBossZone;
		m_wIsGuildHall				=	mapInfo.m_bf1IsGuildHall;
		m_bIs1LevelGuildHall		=	mapInfo.m_bf1Is1LevelGuildHall;
		m_bIsGuildPointBattleField	=	mapInfo.m_bf1IsGuildPointBattleField;
		m_bIsSiegeWarfareField		=	mapInfo.m_bf1IsSiegeWarfareField;
		m_bIsCanNotCallCarpetField	=	mapInfo.m_bf1IsCanNotCallCarpetField;
		m_bIsCanNotUseCalling		=	mapInfo.m_bf1IsCanNotUseCallingSkill;
		m_bIsUseFindWay				=	mapInfo.m_bf1IsUseFindWay;
		m_bIsSetBossMap				=	mapInfo.m_bf1IsSetBossMap;
		m_bIsGuildDungeonMap		=	mapInfo.m_bf1IsGuildDungeonMap;
		m_bIsNotUseSkillBeforeTrans		=	mapInfo.m_bf1IsNotUseSkillBeforeTrans;
		m_bIsNotOpenPitchManShop		=	mapInfo.m_bf1IsNotOpenPitchManShop;
		m_bWeatherType					=	mapInfo.m_bf3WeatherType;
		m_bIsNight						=	mapInfo.m_bf1IsNight;
		if	(m_bIsSiegeWarfareField)
			m_bIsGuildBattleField	=	TRUE;

		if	(m_wIsBossZone)
			m_iBossZoneResistenceLimit	=	c_iBossZoneResistenceLimit;
		else
			m_iBossZoneResistenceLimit	=	100;


		m_sCorrectFireResistance	=	mapInfo.m_sCorrectFireResistance;
		m_sCorrectWaterResistance	=	mapInfo.m_sCorrectWaterResistance;
		m_sCorrectWindResistance	=	mapInfo.m_sCorrectWindResistance;
		m_sCorrectEarthResistance	=	mapInfo.m_sCorrectEarthResistance;
		m_sCorrectLightResistance	=	mapInfo.m_sCorrectLightResistance;
		m_sCorrectDarkResistance	=	mapInfo.m_sCorrectDarkResistance;

		m_sCorrectMonsterFireResistance	=	mapInfo.m_sCorrectMonsterFireResistance;
		m_sCorrectMonsterWaterResistance=	mapInfo.m_sCorrectMonsterWaterResistance;
		m_sCorrectMonsterWindResistance	=	mapInfo.m_sCorrectMonsterWindResistance;
		m_sCorrectMonsterEarthResistance=	mapInfo.m_sCorrectMonsterEarthResistance;
		m_sCorrectMonsterLightResistance=	mapInfo.m_sCorrectMonsterLightResistance;
		m_sCorrectMonsterDarkResistance	=	mapInfo.m_sCorrectMonsterDarkResistance;

		m_sCorrectMonsterFireResistance	=	mapInfo.m_sCorrectMonsterFireResistance;			//	불 저항력 0% 100.00%  
		m_sCorrectMonsterWaterResistance=	mapInfo.m_sCorrectMonsterWaterResistance;			//	물 저항력 0% 100.00%  
		m_sCorrectMonsterWindResistance	=	mapInfo.m_sCorrectMonsterWindResistance;			//	바람 저항력 0% 100.00%  
		m_sCorrectMonsterEarthResistance=	mapInfo.m_sCorrectMonsterEarthResistance;			//	대지 저항력 0% 100.00%  
		m_sCorrectMonsterLightResistance=	mapInfo.m_sCorrectMonsterLightResistance;			//	빛 저항력 0% 100.00%
		m_sCorrectMonsterDarkResistance	=	mapInfo.m_sCorrectMonsterDarkResistance;			//	어둠 저항력 0% 100.00%

		m_sCorrectMonsterBlindResistance	=	mapInfo.m_sCorrectMonsterBlindResistance;			//	장님 저항
		m_sCorrectMonsterPoisonResistance	=	mapInfo.m_sCorrectMonsterPoisonResistance;			//	중독 저항
		m_sCorrectMonsterSleepResistance	=	mapInfo.m_sCorrectMonsterSleepResistance;			//	잠 저항
		m_sCorrectMonsterColdResistance		=	mapInfo.m_sCorrectMonsterColdResistance;			//	콜드 저항
		m_sCorrectMonsterFreezeResistance	=	mapInfo.m_sCorrectMonsterFreezeResistance;			//	프리즈 저항
		m_sCorrectMonsterStunResistance		=	mapInfo.m_sCorrectMonsterStunResistance;			//	경직 저항
		m_sCorrectMonsterStoneResistance	=	mapInfo.m_sCorrectMonsterStoneResistance;			//	석화 저항
		m_sCorrectMonsterConfuseResistance	=	mapInfo.m_sCorrectMonsterConfuseResistance;		//	혼란 저항
		m_sCorrectMonsterCharmingResistance	=	mapInfo.m_sCorrectMonsterCharmingResistance;		//	챠밍 저항
		m_sCorrectMonsterCriticalResistance	=	mapInfo.m_sCorrectMonsterCriticalResistance;
		m_sCorrectMonsterCrushResistance	=	mapInfo.m_sCorrectMonsterCrushResistance;

		m_sCorrectMonsterBadStatusResistance	=	mapInfo.m_sCorrectMonsterBadStatusResistance;		//	상태 이상공격에 대한 저항 0% 100.00%
		m_sCorrectMonsterDeclinePowerResistance	=	mapInfo.m_sCorrectMonsterDeclinePowerResistance;	//	능력치 저하계에 대한 저항 0% 100.00%
		m_sCorrectMonsterCurseResistance		=	mapInfo.m_sCorrectMonsterCurseResistance;			//	저주에 대한 저항 0% 100.00%

		strcpy(m_strName,mapInfo.m_strName);

		if	(m_wLinkSecretDungeon	==	0)
			m_wLinkSecretDungeon	=	0xffff;

		if	(m_wLinkSecretDungeon	==	0xffff)
			m_wIsExistSecretDungeonGate	=	FALSE;

		if	(m_wDuelType	!=	0xffff)
			m_bIsGuildBattleField	=	TRUE;

		//길드 던젼은 GBS 필드에 포함되지 않는다.(이속 제한 없음.)
		if	(m_bIsGuildBattleField	||	m_wIsBossZone	||	m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
		{
			m_wIsGBS_Field	=	TRUE;
			if(m_bIsGuildDungeonMap)
				m_wIsGBS_Field	=	FALSE;
		}
	}	//	맵 정보 읽어옴	

	file.Skip(4);	//	영역 정보 위치

////////////////////////////////////////////////////////////////////////////////////////////////
//	맵 데이터 스킵(클라이언트와 툴에서 사용.)
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		file.Skip(m_iWidth*m_iHeight*6);					//	타일/높이/건물,오브젝트,블럭 정보
	}	//	맵 데이터 스킵(클라이언트와 툴에서 사용.)

////////////////////////////////////////////////////////////////////////////////////////////////
//	문 데이터
////////////////////////////////////////////////////////////////////////////////////////////////
	if (s_iFileVersion	>=	eVUI_29_ADD_DOOR_LIST)
	{
		int	iDoorCount;

		file.Read(&iDoorCount,4);

		KILL(m_pDoors);

		if (iDoorCount)
		{
			m_pDoors	=	new cDoorManager ();

			m_pDoors->m_iDoorCount	=	iDoorCount;

			for (int i=0;i<m_pDoors->m_iDoorCount;i++)
			{
				cDoorObjectInfo	*lpDoor	=	&m_pDoors->m_aDoorList[i];

				file.Read(lpDoor,sizeof(cDoorObjectSimpleInfo));

				lpDoor->m_iPosValue	=	lpDoor->m_wX+lpDoor->m_wY*m_iWidth;
				lpDoor->m_wStatus	=	eDS_CLOSE;
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	충돌 맵 정보 로딩
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		m_pMap	=	new BYTE [m_iWidth*m_iHeight];

		file.Read(m_pMap,m_iWidth*m_iHeight);				//	블럭 정보
	}	//	충돌 맵 정보 로딩

////////////////////////////////////////////////////////////////////////////////////////////////
//	커스텀 아이템 로딩
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		CCustomItem	aCustomItem[dMAX_CUSTOM_ITEM_COUNT];
		int			iCustomItemCount=	0;

		file.Read(&i,4);	//	스킵 데이터 위치
		file.Read(aCustomItem,dMAX_CUSTOM_ITEM_COUNT*sizeof(CCustomItem));

		for (i=dMAX_CUSTOM_ITEM_COUNT;i;i--)
		{
			if (aCustomItem[i-1].m_wBaseItem	!=	0xffff)
			{
				iCustomItemCount	=	i;
				break;
			}
		}

		m_customItemList.init(iCustomItemCount);

		CCustomItem	*lpItems		=	m_customItemList.getItemBay();

		if (iCustomItemCount)	memcpy(lpItems,aCustomItem,iCustomItemCount*sizeof(CCustomItem));
	}

////////////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터 데이터
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		file.Read(&m_iCharacterCount	,	4);		//	캐릭터 데이터의 수

		if	(s_iFileVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&m_iCharacterCount,4);

		file.Skip(m_iCharacterCount*2);				//	직업 리스트
		file.Skip(4);								//	캐릭터 정보가 끝나는곳 위치

		int	iDataSize	=	-1;

		if	(s_iFileVersion	>=	eVVI_45_ADD_MORE_CHARACTER_DATA)
			file.Read(&iDataSize	,	4);		//	캐릭터 데이터의 사이즈

		for (i=0;i<m_iCharacterCount;i++)
		{
			cCHARACTER	saveCharacter,*lpCharacter;
			int			iSaveEventConditionCount;

			memset(&saveCharacter,0,sizeof(CCharacterBaseInfo));

			saveCharacter.reset();
		
			if	(s_iFileVersion	>=	eVVI_45_ADD_MORE_CHARACTER_DATA)
				file.Read(&saveCharacter,iDataSize);
			else
			{
				if	(s_iFileVersion	>=	eVUI_27_ADD_CHARACTER_DATA)
					file.Read(&saveCharacter,sizeof(CCharacterBaseInfo_old_before090717));
				else
					file.Read(&saveCharacter,sizeof(CCharacterBaseInfo_old_before090717)-4);

				saveCharacter.m_bf1IsBlockToTame	=	FALSE;
			}

			if	(s_iFileVersion	>=	eVVI_48_ENCRYT)
				cMessSign::DecodeDataBuffer2(&saveCharacter,iDataSize);

			if	(s_iFileVersion	<	eVVI_46_REVISE_BLOCK_TO_TAME)
				saveCharacter.m_bf1IsBlockToTame	=	FALSE;
		

//			if	(s_iFileVersion	<	eVUI_41_ADD_REVISE_INT)
//				saveCharacter.m_bf1IsRevisIntCalc	=	FALSE;

			if	(s_iFileVersion	<	eVUI_40_ADD_DAMAGE_IMMUNE)
			{
				saveCharacter.m_bf1IsImmuneMagicDamage	=	FALSE;
				saveCharacter.m_bf1IsImmunePhysicalDamage=	FALSE;
			}

			
			if	(s_iFileVersion	<	eVUI_38_ADD_CORRECT_CHARACTER_POWER)
			{
				saveCharacter.m_wCorrectHP	=	100;
				saveCharacter.m_wCorrectAP	=	100;
				saveCharacter.m_wCorrectExp	=	100;
			}

			if	(s_iFileVersion	<	eVUI_35_ADD_BOSS_ZONE)
			{
				saveCharacter.m_wCorrectFinalDamage	=	10000;
				saveCharacter.m_bf1IsImmuneStun		=	FALSE;
				saveCharacter.m_bf1IsImmuneConfuse	=	FALSE;
				saveCharacter.m_bf1IsImmuneStone	=	FALSE;
				saveCharacter.m_bf1IsImmuneCharming	=	FALSE;
				saveCharacter.m_bf1IsImmuneFreeze	=	FALSE;
				saveCharacter.m_bf1IsImmuneKnockBack=	FALSE;
			}

			lpCharacter	=	&m_aCharacter[saveCharacter.m_wSerial];

			memcpy(lpCharacter,&saveCharacter,sizeof(CCharacterBaseInfo));

			if (lpCharacter->m_wScaleX	<=	10	||	lpCharacter->m_wScaleY	<=	10	||	
				lpCharacter->m_wScaleX	>	200	||	lpCharacter->m_wScaleY	>	200)
			{
				lpCharacter->m_wScaleX	=	100;
				lpCharacter->m_wScaleY	=	100;
			}

			file.Read(&iSaveEventConditionCount,4);

			for (j=0;j<iSaveEventConditionCount;j++)
			{
				int	iEvent;

				file.Read(&iEvent,4);

				lpCharacter->m_pEvent[iEvent]	=	new CKarmaContainer();

				ReadKarmaContainer(&file,lpCharacter->m_pEvent[iEvent],bIsDecodeKaramData);
			}
		}
	}
////////////////////////////////////////////////////////////////////////////////////////////////
//	액터
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		file.Read(&m_iSavedActorCount	,4);//	읽기 싫을때 점프할 위치
		file.Read(&m_iSavedActorCount	,4);//	액터의 수

		if	(s_iFileVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&m_iSavedActorCount,4);

		m_iSaveMonsterCount	=	0;
		m_iSaveNPCCount		=	0;

		for (i=0;i<m_iSavedActorCount;i++)
		{
			cBASE_NPC_INFO	existActor;
			WORD			wSpeechCount;

			file.Read(&existActor,sizeof(cBASE_NPC_INFO));	//	액터 데이터 로딩

			file.Read(&wSpeechCount,2);	//	액터 데이터 로딩

			if	(s_iFileVersion	>=	eVVI_48_ENCRYT)
				cMessSign::DecodeDataBuffer2(&existActor,sizeof(cBASE_NPC_INFO));

			m_apNpcGenerateData[existActor.m_iSerial]	=	new cNPC();
			
			memcpy(m_apNpcGenerateData[existActor.m_iSerial],&existActor,sizeof(cBASE_NPC_INFO));
			
			if	(existActor.m_wActorKind	==	eAK_MONSTER)	
				m_iSaveMonsterCount++;
			else
			{
				m_iSaveNPCCount++;
				m_apNpcGenerateData[existActor.m_iSerial]->m_wBattleInclination	=	dBI_PASSIVE_SOLO;
			}

			if	(wSpeechCount	>	0)
			{
				m_apNpcGenerateData[existActor.m_iSerial]->m_pSpeech	=	new CConversationContainer();

				readCC(&file,m_apNpcGenerateData[existActor.m_iSerial]->m_pSpeech,wSpeechCount,bIsDecodeKaramData);
			}

			if	(s_iFileVersion	<	eVVI_47_ADD_HIDE_NAME_BAR_OPTION)
				m_apNpcGenerateData[existActor.m_iSerial]->m_bf1IsHideName			=	FALSE;

			if	(s_iFileVersion	<	eVUI_37_ADD_BLOCK_TO_AUTO_REGEN)
				m_apNpcGenerateData[existActor.m_iSerial]->m_wIsBlockToAutoRegen	=	FALSE;

			if	(m_apNpcGenerateData[existActor.m_iSerial]->m_wIsBlockToAutoRegen	==	0xcccc)
				m_apNpcGenerateData[existActor.m_iSerial]->m_wIsBlockToAutoRegen	=	FALSE;
		}
	}	//	액터

////////////////////////////////////////////////////////////////////////////////////////////////
//	영역 불러오기
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		m_pArea->reset();	//	초기화

		file.Read(&m_pArea->m_iCount,4);//	영역의 수

		if	(s_iFileVersion	>=	eVVI_48_ENCRYT)
			cMessSign::DecodeDataBuffer2(&m_pArea->m_iCount,4);

		for (i=0;i<m_pArea->m_iCount;i++)
		{
			cAreaInfo				area,*lpArea;
			cAreaSaveData			areaSave;
			int						iLocate;

			if	(s_iFileVersion	<	eVUI_28_EXPAND_AREA_DATA)
			{
				file.Read(&areaSave,sizeof(cAREA_OLD_SAVE_DATA));
				memset(areaSave.m_aBuffer,0,sizeof(areaSave.m_aBuffer));
			}
			else
			{
				if	(s_iFileVersion	<	eVVI_43_ADD_SEASON_VARIABLE)
				{
					file.Read(&areaSave,sizeof(cAreaSaveData) - 2);
					areaSave.m_wSeasonVariable = 0;
				}
				else
				{
					file.Read(&areaSave,sizeof(cAreaSaveData));
				}

				file.Read(&iLocate,4);//	카르마 저장하고 난후의 위치
			}

			if	(s_iFileVersion	>=	eVVI_48_ENCRYT)
				cMessSign::DecodeDataBuffer2(&areaSave,sizeof(cAreaSaveData));

			lpArea	=	m_pArea->getArea(areaSave.m_wSerial,TRUE,areaSave.m_wKind);

			lpArea->set(&areaSave);

			lpArea->m_cc.reset();
			
			if (s_iFileVersion	>=	eVUI_28_EXPAND_AREA_DATA)
			{
				if (s_iFileVersion	<	eVUI_32_ADD_AREA_CC)
					file.Seek(iLocate,SEEK_SET);
				else
				{
					WORD	wCount;

					file.Read(&wCount,2);

					if (wCount	>	0)
						readCC(&file,&lpArea->m_cc,wCount,bIsDecodeKaramData);
				}
			}

			file.Read(&iLocate,4);//텍스트의 위치

			if(iLocate	!=	0xffffffff)
			{
				iLocate	+=	m_iTextDataLocate;

				int	iTextSize;
				int	iCurLocate	=	file.Locate();	//	현재 위치

				file.Seek(iLocate,SEEK_SET);		//	문장의 위치로 이동
				file.Read(&iTextSize,4);					//	사이즈

				lpArea->m_pstrText	=	new char [iTextSize+1];

				file.Read(lpArea->m_pstrText,iTextSize);
				file.Seek(iCurLocate,SEEK_SET);
			}
		}

		m_pArea->m_pArea[0]->x1	=	dTILE_XS+dTILE_XS/2;
		m_pArea->m_pArea[0]->y1	=	dTILE_YS+dTILE_YS/2;
		m_pArea->m_pArea[0]->x2	=	m_iPixelWidth-dTILE_XS-dTILE_XS/2;
		m_pArea->m_pArea[0]->y2	=	m_iPixelHeight-dTILE_YS-dTILE_YS/2;

		m_pArea->build(this);	//영역 정보 설정
	}	//	영역 불러오기

////////////////////////////////////////////////////////////////////////////////////////////////
//	상점
////////////////////////////////////////////////////////////////////////////////////////////////
	{
		WORD	wShopCount;

		file.Seek(4,SEEK_CUR);	//	스킵정보
		file.Read(&wShopCount,2);//	상점의 수

		m_shop.m_iCount	=	wShopCount;

		if	(wShopCount)
		{
			m_shop.m_pShop	=	new CShop [wShopCount];

			for (int i=0;i<wShopCount;i++)
			{
				CShop	*lpShop	=	&m_shop.m_pShop[i];
				cShopNameInfo	shopName;

				file.Read(&shopName,sizeof(shopName));	//	상점 이름
				
				strcpy(lpShop->m_strName,shopName.m_strName);	//	상점 이름

				if	(s_iFileVersion	<	eVUI_42_ADD_DUEL_SERVER)
				{
					shopName.m_bf1IsDuelPointShop		=	0;
					shopName.m_bf1IsLevelPerGoldShop	=	0;
					shopName.m_bf15LevelPerGoldValue	=	0;
					shopName.m_bf1IsNotBuyItem			=	FALSE;
				}

				lpShop->m_bf1IsDuelPointShop		=	shopName.m_bf1IsDuelPointShop;
				lpShop->m_bf1IsLevelPerGoldShop		=	shopName.m_bf1IsLevelPerGoldShop;
				lpShop->m_bf15LevelPerGoldValue		=	shopName.m_bf15LevelPerGoldValue;
				lpShop->m_bf1IsNotBuyItem			=	shopName.m_bf1IsNotBuyItem;		// 상점 구입 불가.

				file.Read(&lpShop->m_dwLimitPrice,4);	//	아이템 구입 최고가
				file.Read(&lpShop->m_wSerial,2);		//	상점 시리얼
				file.Read(&lpShop->m_wPriceFactor,2);	//	상점 시리얼

				if	(s_iFileVersion < eVVI_44_ADD_TOKEN_SHOP)
					lpShop->m_TokenShop = 0xffff;
				else
					file.Read(&lpShop->m_TokenShop,2);		//	토큰 샵

				file.Read(&lpShop->m_wOrgItemCount,2);		//	판매 아이템의 수

				lpShop->m_wItemCount	=	lpShop->m_wOrgItemCount;
				lpShop->m_dwLimitPrice	=	max(lpShop->m_dwLimitPrice,5000);
				lpShop->m_dwLimitPrice	=	min(lpShop->m_dwLimitPrice,100000);

				lpShop->alloc(lpShop->m_wItemCount);

#ifdef	_FOR_JAPAN
				if	(m_iUniqueSerial	==	193)
				{
					if	(lpShop->m_wSerial	==	9)
					{
						lpShop->m_wPriceFactor	=	10;
						lpShop->m_dwLimitPrice	=	100000;
					}
				}
#endif

				for (j=0;j<lpShop->m_wItemCount;j++)
				{
					CShopGenerateItemInfo	*lpItem	=	&lpShop->m_pGenerateItemInfo[j];

					file.Read(lpItem,sizeof(CShopGenerateItemInfo));

					if	(s_iFileVersion	<	eVUI_42_ADD_DUEL_SERVER)
					{
						lpItem->m_bf10ChangeLimitLevel	=	0;
					}

					if	(lpItem->m_bCount	<=	0)
						lpItem->m_bCount	=	1;
				}
			}

			m_shop.generateItem();
		}
	}

	if (s_iFileVersion	>=	eVUI_31_ADD_FIELD_KARMA)
	{
		file.Seek(iKarmaInfoLocate,SEEK_SET);
		ReadKarmaContainer(&file,&m_kc,bIsDecodeKaramData);
	}
	
	if	(s_iFileVersion	>=	eVUI_36_ADD_CHARACTER_PATTERN)
	{
		WORD	wPatternCount;
		
		file.Read(&i,4);
		file.Read(&wPatternCount,2);

		if	(wPatternCount)
		{
			m_pCPP	=	new cCharacterPatternPack();

			for	(i=0;i<wPatternCount;i++)
			{
				WORD	wSerial,wCount;

				file.Read(&wSerial,2);
				file.Read(&wCount,2);
				file.Seek(c_iCharacterPatternNameLength);

				int	iReadBufferSize				=	sizeof(cCharacterPatternPiece)*wCount;

				m_pCPP->m_apPatterns[wSerial]	=	new cCharacterPattern();
				cCharacterPattern	*lpPattern	=	m_pCPP->m_apPatterns[wSerial];

				lpPattern->m_pPattern			=	new cCharacterPatternPiece [wCount];

				file.Read(lpPattern->m_pPattern,iReadBufferSize);
				lpPattern->m_wSerial			=	wSerial;
				lpPattern->m_wCount				=	wCount;
			}
		}
	}

	if	(s_iFileVersion	>=	eVUI_39_ADD_FIELD_VALUE)
	{
		int	iWriteSkipSizeLocate;

		file.Read(&iWriteSkipSizeLocate,4);
		file.Seek(iWriteSkipSizeLocate,SEEK_SET);
	}

	file.Close();

	strcpy(s_strServerVersion,"2010-08-10 18:16");

	srand( (unsigned)time( NULL ) );

//	몬스터 활동 영역 재 설정
	int	iCount	=	0;

	for (i=0,iCount=0;i<dMAX_NPC_COUNT&&iCount<m_iSavedActorCount;i++)
	{
		cNPC	*lpNpc		=	m_apNpcGenerateData[i];

		if (!lpNpc	||	lpNpc->m_iSerial==	-1)
			continue;

		if (lpNpc->isNotMoveNpc())
			continue;

		int	iMoveArea		=	m_pArea->findAreaIndexBySerial(lpNpc->m_wMoveArea);

		if	(iMoveArea		==	-1)
			lpNpc->m_wMoveArea	=	0;

		iCount++;
	}

	if	(m_wIsWordQuizField)
	{
		m_pWordActorInfo	=	new cWordQuizActorInfo [dMAX_ACTOR_IN_FIELD];
		m_pSortWordQuizActor=	new cIndexForQSort [dMAX_ACTOR_IN_FIELD];
	}

	{
		cFILE	fileCheckLocation;

		if	(fileCheckLocation.Open(_ms("%s\\data\\_buffData.dat",g_strLogFolder),"rb"))
		{
			fileCheckLocation.Read(&g_wBuffUpkeepTime,2);
			fileCheckLocation.Close();

			g_wBuffUpkeepTime	=	0;
		}
	}


//	필드를 적절하게 지역 분활 해서 관리한다.
	{
		int	aTeamActorCount[dTEAM_COUNT];

		if	(m_bIsGuildBattleField  || m_iUniqueSerial	>= dINSTANCE_FIELD_FLAG	||	m_wIsBossZone)
		{
			m_wIsCanNotMemoryField	=	TRUE;
			aTeamActorCount[0]		=	400;
			aTeamActorCount[1]		=	400;
			aTeamActorCount[2]		=	m_iSaveMonsterCount+m_iSaveNPCCount+10;

			m_iLocalPartitionCount	=	1;

			m_pLocalPartition		=	new CLocalPartionInfo			[m_iLocalPartitionCount];
			m_pPartLinkInfo			=	new CLocalSmallPartionLinkInfo	[m_iLocalPartitionCount];
			m_pPartLinkInfo[0].init(1);
			m_pPartLinkInfo[0].addPart(0);
		}
		else
		if	(isSpecialField())
		{
			m_wIsCanNotMemoryField	=	TRUE;
			aTeamActorCount[0]		=	1024;
			aTeamActorCount[1]		=	m_iSaveMonsterCount+10;
			aTeamActorCount[2]		=	m_iSaveNPCCount+10;

			m_iLocalPartitionCount	=	1;

			m_pLocalPartition		=	new CLocalPartionInfo [m_iLocalPartitionCount];
			m_pPartLinkInfo			=	new CLocalSmallPartionLinkInfo	[m_iLocalPartitionCount];
			m_pPartLinkInfo[0].init(1);
			m_pPartLinkInfo[0].addPart(0);
		}
		else
		{
			aTeamActorCount[0]		=	dMAX_ACTOR_COUNT_IN_LOCAL_PART;
			aTeamActorCount[1]		=	dMAX_ACTOR_COUNT_IN_LOCAL_PART/2;
			aTeamActorCount[2]		=	m_iSaveNPCCount;

			int	iMaxPartCount		=	(dPARTITION_COUNT+2)*(dPARTITION_COUNT+2);

			m_iPartWidth			=	max(m_iPixelWidth/(dPARTITION_COUNT+2),800);
			m_iPartHeight			=	max(m_iPixelHeight/(dPARTITION_COUNT+2),600);
			m_iWidthPartCount		=	min(m_iPixelWidth/m_iPartWidth+1,dPARTITION_COUNT+2);
			m_iHeightPartCount		=	min(m_iPixelHeight/m_iPartHeight+1,dPARTITION_COUNT+2);

			if (m_iWidthPartCount < dPARTITION_COUNT+2 && m_iHeightPartCount == dPARTITION_COUNT+2)
			{
				int	iLocalPartCount	=	m_iWidthPartCount*m_iHeightPartCount;
				int	iSparePartCount	=	iMaxPartCount-iLocalPartCount;
				int	iSpareHeightCount=	iSparePartCount/m_iWidthPartCount;

				if (iSpareHeightCount)
				{
					m_iHeightPartCount	+=	iSpareHeightCount;

					m_iPartHeight		=	max(m_iPixelHeight/m_iHeightPartCount,600);
					m_iHeightPartCount	=	m_iPixelHeight/m_iPartHeight;
				}
			}

			if (m_iHeightPartCount < dPARTITION_COUNT+2 && m_iWidthPartCount == dPARTITION_COUNT+2)
			{
				int	iLocalPartCount	=	m_iWidthPartCount*m_iHeightPartCount;
				int	iSparePartCount	=	iMaxPartCount-iLocalPartCount;
				int	iSpareWidthCount=	iSparePartCount/m_iHeightPartCount;

				if (iSpareWidthCount)
				{
					m_iWidthPartCount	+=	iSpareWidthCount;

					m_iPartWidth		=	max(m_iPixelWidth/m_iWidthPartCount,800);
					m_iWidthPartCount	=	m_iPixelWidth/m_iPartWidth;
				}
			}

			if (m_iWidthPartCount	<	3)
			{
				m_iWidthPartCount	=	3;
				m_iPartWidth		=	max(m_iPixelWidth/m_iWidthPartCount,800);
			}

			if (m_iHeightPartCount	<	3)
			{
				m_iHeightPartCount	=	3;
				m_iPartHeight		=	max(m_iPixelHeight/m_iHeightPartCount,600);
			}

			int	iRealWidthPartCount	=	m_iWidthPartCount-2;
			int	iRealHeightPartCount=	m_iHeightPartCount-2;
			int	iSmallPartCount		=	m_iWidthPartCount*m_iHeightPartCount;

			m_iLocalPartitionCount	=	iRealWidthPartCount*iRealHeightPartCount;
			m_pLocalPartition		=	new CLocalPartionInfo [m_iLocalPartitionCount];
			m_pPartLinkInfo			=	new CLocalSmallPartionLinkInfo	[iSmallPartCount];

			int iX=g_wBuffUpkeepTime,iY=g_wBuffUpkeepTime;

			for (iY=1;iY<m_iHeightPartCount-1;iY++)
			{
				int	iHeightPart	=	iY-1;

				for (iX=1;iX<m_iWidthPartCount-1;iX++)
				{
					int	iWidthPart	=	iX-1;
					int	iPart		=	iWidthPart+iHeightPart*iRealWidthPartCount;
					int	iXPos		=	(iX-1)*m_iPartWidth;
					int	iYPos		=	(iY-1)*m_iPartHeight;

					m_pLocalPartition[iPart].m_rect.Set(iXPos,iYPos,iXPos+m_iPartWidth*3-1,iYPos+m_iPartHeight*3-1);

					if (iX	==	m_iWidthPartCount-2)
						m_pLocalPartition[iPart].m_rect.x2	=	m_iPixelWidth;

					if (iY	==	m_iHeightPartCount-2)
						m_pLocalPartition[iPart].m_rect.y2	=	m_iPixelHeight;
				}
			}

			for (iY=0;iY<m_iHeightPartCount;iY++)
			{
				for (iX=0;iX<m_iWidthPartCount;iX++)
				{
					int	iSmallPart		=	iX+iY*m_iWidthPartCount;
					int	iLinkPartCount	=	0;

					for (i=0;i<3;i++)
						for (j=0;j<3;j++)
							if (iX-i >= 0 && iY - j >= 0 && iX-i < m_iWidthPartCount-2 && iY-j < m_iHeightPartCount-2)
								iLinkPartCount++;

					m_pPartLinkInfo[iSmallPart].init(iLinkPartCount);

					for (i=0;i<3;i++)
						for (j=0;j<3;j++)
							if (iX-i >= 0 && iY - j >= 0 && iX-i < m_iWidthPartCount-2 && iY-j < m_iHeightPartCount-2)
							{
								int	iPart	=	iX-i+(iY-j)*(m_iWidthPartCount-2);

								m_pPartLinkInfo[iSmallPart].addPart(iPart);
							}
				}
			}
		}

		for (i=0;i<m_iLocalPartitionCount;i++)
			m_pLocalPartition[i].init(aTeamActorCount[0],aTeamActorCount[1],aTeamActorCount[2]);

		static	DWORD	dwSize	=	0;
		int		iSize			=	m_iLocalPartitionCount*((aTeamActorCount[0]+aTeamActorCount[1]+aTeamActorCount[2])*2+sizeof(cRECT)+24);

		dwSize	+=	iSize;
//		printf(" %d.%.3d MB",dwSize/(1024*1024),(dwSize%(1024*1024))/1024);
	}

	//linkMonsterInSameArea();
	generateAllNPC();	//	모든 NPC를 생성 시킨다.

	resetUserPlayStatics();
	
	return TRUE;
}

char	cFIELD::s_strServerVersion[64];

//
//	업데이트
void
cFIELD::updateGame()
{
	BOOL	bIsIncorrectActorInfo	=	FALSE;
	int		iSerial;

	g_debugSign.m_iCurrentFieldStep	=	1;
	m_iFrameCounter++;		//	프레임 증가

//	operateBookedPlayerToJoin();		//	참가하기로 예약된 녀석들 처리
	if	(g_game.m_bIsRequireRebuildRedStonePointTitle)	//	붉은 보석 포인트 체크
		rebuildRedstonePointEffect();

	g_debugSign.m_iCurrentFieldStep	=	2;

	resetStandActorList();
	m_iRealPlayerCount				=	0;
	m_iAlivePlayerCount				=	0;

	if (m_bIsGuildBattleField)
	{
		g_debugSign.m_iCurrentFieldStep	=	3;

		for (g_debugSign.m_iCurrentPlayer=m_aiActorCount[0];g_debugSign.m_iCurrentPlayer;--g_debugSign.m_iCurrentPlayer)
		{
			iSerial	=	m_awActorList[0][g_debugSign.m_iCurrentPlayer-1];

			cACTOR	*lpPlayer	=	getTestedActor(iSerial);

			if (!lpPlayer)
			{
				bIsIncorrectActorInfo	=	TRUE;
				continue;
			}

			lpPlayer->updatePlayer();

			if	(lpPlayer->isPlayer())
			{
				++m_iRealPlayerCount;

				if	(lpPlayer->isDeath()	==	FALSE)
					++m_iAlivePlayerCount;
			}
		}

		g_debugSign.m_iCurrentFieldStep	=	4;

		for (g_debugSign.m_iCurrentMonster=m_aiActorCount[1];g_debugSign.m_iCurrentMonster;--g_debugSign.m_iCurrentMonster)
		{
			iSerial	=	m_awActorList[1][g_debugSign.m_iCurrentMonster-1];

			cACTOR	*lpPlayer	=	getTestedActor(iSerial);

			if (!lpPlayer)
			{
				bIsIncorrectActorInfo	=	TRUE;
				continue;
			}

			lpPlayer->updatePlayer();

			if	(lpPlayer->isPlayer())
				++m_iRealPlayerCount;
		}

		g_debugSign.m_iCurrentFieldStep	=	5;

		if (m_aiActorCount[0])	//	플레이어가 없다면 NPC를 업데이트 할 필요 없다.
			for (g_debugSign.m_iCurrentNpc=m_aiActorCount[2];g_debugSign.m_iCurrentNpc;--g_debugSign.m_iCurrentNpc)
			{
				iSerial	=	m_awActorList[2][g_debugSign.m_iCurrentNpc-1];

				cACTOR	*lpNPC	=	getTestedActor(iSerial);

				if (!lpNPC)
				{
					bIsIncorrectActorInfo	=	TRUE;
					continue;
				}

				lpNPC->updateMonster();
			}

		g_debugSign.m_iCurrentFieldStep	=	6;

	}
	else
	{
		
		g_debugSign.m_iCurrentFieldStep	=	7;
		g_game.beginProfile("플레이어 업데이트");
		for (g_debugSign.m_iCurrentPlayer=m_aiActorCount[0];g_debugSign.m_iCurrentPlayer;--g_debugSign.m_iCurrentPlayer)
		{
			iSerial	=	m_awActorList[0][g_debugSign.m_iCurrentPlayer-1];

			cACTOR	*lpPlayer	=	getTestedActor(iSerial);

			if (!lpPlayer)
			{
				bIsIncorrectActorInfo	=	TRUE;
				continue;
			}

			lpPlayer->updatePlayer();

			if	(lpPlayer->isPlayer())
			{
				++m_iRealPlayerCount;
				if	(lpPlayer->isDeath()	==	FALSE)
					++m_iAlivePlayerCount;
			}
		}
		g_game.endProfile("플레이어 업데이트");

		g_game.beginProfile("몬스터 업데이트");
		g_debugSign.m_iCurrentFieldStep	=	8;
		for (g_debugSign.m_iCurrentMonster=m_aiActorCount[1];g_debugSign.m_iCurrentMonster;--g_debugSign.m_iCurrentMonster)
		{
			iSerial	=	m_awActorList[1][g_debugSign.m_iCurrentMonster-1];

			cACTOR	*lpMonster	=	getTestedActor(iSerial);

			if (!lpMonster)
			{
				bIsIncorrectActorInfo	=	TRUE;
				continue;
			}

			if (lpMonster->isPlayer())
			{
				CLOG("monsterFailed","Monster" );
				bIsIncorrectActorInfo	=	TRUE;
				continue;
			}

			lpMonster->updateMonster();
		}

		g_game.endProfile("몬스터 업데이트");

		g_debugSign.m_iCurrentFieldStep	=	9;
		g_game.beginProfile("NPC 업데이트");

		if (m_aiActorCount[0])	//	플레이어가 없다면 NPC를 업데이트 할 필요 없다.
			for (g_debugSign.m_iCurrentNpc=m_aiActorCount[2];g_debugSign.m_iCurrentNpc;--g_debugSign.m_iCurrentNpc)
			{
				iSerial	=	m_awActorList[2][g_debugSign.m_iCurrentNpc-1];

				cACTOR	*lpNPC	=	getTestedActor(iSerial);

				if (!lpNPC)
				{
					bIsIncorrectActorInfo	=	TRUE;
					continue;
				}
				if (lpNPC->isPlayer())
				{
					CLOG("monsterFailed","NPC" );
					bIsIncorrectActorInfo	=	TRUE;
					continue;
				}

				lpNPC->updateNpc();
			}

		g_debugSign.m_iCurrentFieldStep	=	10;
		g_game.endProfile("NPC 업데이트");
	}

	if	(bIsIncorrectActorInfo)
	{
		g_debugSign.m_iCurrentFieldStep	=	101;

		rebuildActorList();

		g_debugSign.m_iCurrentFieldStep	=	102;
	}

	g_game.beginProfile("스킬 처리");
	m_skillManager.update();	//	스킬 업데이트
	g_game.endProfile("스킬 처리");
	g_debugSign.m_iCurrentFieldStep	=	11;

	operateReadyToRemoveActorList();

	for (;m_iBookedRemoveActorCount;--m_iBookedRemoveActorCount)
	{
		cBookedRemoveActorInfo	*lpInfo	=	&m_aBookedRemoveActor[m_iBookedRemoveActorCount-1];

		removeActor(lpInfo->m_wSerial,lpInfo->m_bf1IsSleep);
	}

	g_debugSign.m_iCurrentFieldStep	=	-1;

	return;
}


//
//	_iPartySerial 파티가 해제 되었다고 통보한다.
void
cFIELD::operateBreakParty(int _iPartySerial)
{
	int	i,iCount;

//	날 때린 넘-_- 변경
	for (i=0,iCount=0;i<dMAX_NPC_COUNT&&iCount<m_iSavedActorCount;i++)
	{
		if (m_apNpcGenerateData[i] == NULL || m_apNpcGenerateData[i]->m_wActorSerial	==0xffff)
			continue;

		iCount++;

		m_apNpcGenerateData[i]->operateBreakParty(_iPartySerial);
	}
}

//
//	몬스터의 타격 정보 업데이트
void
cFIELD::updateMonsterBeatInfo()
{
	int	i,iCount;

//	날 때린 넘-_- 변경
	for (i=0,iCount=0;i<dMAX_NPC_COUNT&&iCount<m_iSavedActorCount;i++)
	{
		if (m_apNpcGenerateData[i] == NULL || m_apNpcGenerateData[i]->m_wActorSerial	==0xffff)
			continue;

		iCount++;
		m_apNpcGenerateData[i]->updateBeatInfo(m_iFrameCounter);
	}

}

BOOL
cFIELD::isCanCallMagicCarpetField()
{
	int	iFileNameLength	=	strlen(m_strFileName);

	if	(STRICMP(m_strFileName,"[115]G26_D01.rmd") == 0)
		return	FALSE;
	if	(STRICMP(m_strFileName,"[352]T14_G01_F01.rmd") == 0)
		return	TRUE;
	if	(STRICMP(m_strFileName,"[378]T09_K01.rmd") == 0)
		return	FALSE;
	if	(iFileNameLength	>=	18)
		return	FALSE;
	if	(m_bIsCanNotCallCarpetField)
		return	FALSE;

	if	(m_bIsGuildBattleField)
		return	FALSE;

	return	TRUE;
}

//
//	안 막힌 위치 확인
BOOL
cFIELD::getFreePos(c4POINT *_lpPos)
{
	if ((_lpPos->x % dTILE_XS) > dTILE_XS/2)
	{
		if (!isBlocked(_lpPos->mx+1,_lpPos->my))
		{
			_lpPos->mx	=	(_lpPos->mx+1);
			_lpPos->x	=	dTILE_XS*_lpPos->mx+dTILE_XS/2;

			return	TRUE;
		}
	}
	else
	{
		if (!isBlocked(_lpPos->mx-1,_lpPos->my))
		{
			_lpPos->mx	=	(_lpPos->mx-1);
			_lpPos->x	=	dTILE_XS*_lpPos->mx+dTILE_XS/2;

			return	TRUE;
		}
	}

	if ((_lpPos->y % dTILE_YS) > dTILE_YS/2)
	{
		if (!isBlocked(_lpPos->mx,_lpPos->my+1))
		{
			_lpPos->my	=	(_lpPos->my+1);
			_lpPos->y	=	dTILE_YS*_lpPos->my+dTILE_YS/2;

			return	TRUE;
		}
	}
	else
	{
		if (!isBlocked(_lpPos->mx,_lpPos->my-1))
		{
			_lpPos->my	=	(_lpPos->my-1);
			_lpPos->y	=	dTILE_YS*_lpPos->my+dTILE_YS/2;

			return	TRUE;
		}
	}

	if ((_lpPos->x % dTILE_XS) >= dTILE_XS/2 && (_lpPos->y % dTILE_YS) >= dTILE_YS/2)
	{
		if (!isBlocked(_lpPos->mx+1,_lpPos->my+1))
		{
			_lpPos->mx	=	(_lpPos->mx+1);
			_lpPos->x	=	dTILE_XS*_lpPos->mx+dTILE_XS/2;
			_lpPos->my	=	(_lpPos->my+1);
			_lpPos->y	=	dTILE_YS*_lpPos->my+dTILE_YS/2;

			return	TRUE;
		}
	}

	if ((_lpPos->x % dTILE_XS) >= dTILE_XS/2 && (_lpPos->y % dTILE_YS) <= dTILE_YS/2)
	{
		if (!isBlocked(_lpPos->mx+1,_lpPos->my-1))
		{
			_lpPos->mx	=	(_lpPos->mx+1);
			_lpPos->x	=	dTILE_XS*_lpPos->mx+dTILE_XS/2;
			_lpPos->my	=	(_lpPos->my-1);
			_lpPos->y	=	dTILE_YS*_lpPos->my+dTILE_YS/2;

			return	TRUE;
		}
	}

	if ((_lpPos->x % dTILE_XS) <= dTILE_XS/2 && (_lpPos->y % dTILE_YS) >= dTILE_YS/2)
	{
		if (!isBlocked(_lpPos->mx-1,_lpPos->my+1))
		{
			_lpPos->mx	=	(_lpPos->mx-1);
			_lpPos->x	=	dTILE_XS*_lpPos->mx+dTILE_XS/2;
			_lpPos->my	=	(_lpPos->my+1);
			_lpPos->y	=	dTILE_YS*_lpPos->my+dTILE_YS/2;

			return	TRUE;
		}
	}

	if ((_lpPos->x % dTILE_XS) <= dTILE_XS/2 && (_lpPos->y % dTILE_YS) <= dTILE_YS/2)
	{
		if (!isBlocked(_lpPos->mx-1,_lpPos->my-1))
		{
			_lpPos->mx	=	(_lpPos->mx-1);
			_lpPos->x	=	dTILE_XS*_lpPos->mx+dTILE_XS;
			_lpPos->my	=	(_lpPos->my-1);
			_lpPos->y	=	dTILE_YS*_lpPos->my+dTILE_YS/2;

			return	TRUE;
		}
	}

	return	FALSE;
}

BOOL
cFIELD::isEventField()	//	이벤트 필드냐?
{
	if	(m_wIsOXQuizField+m_wIsWordQuizField)
		return	TRUE;

	return	FALSE;
}

BOOL
cFIELD::isSpecialField()//	특수 필드냐?
{
	if	(m_wIsOXQuizField+m_wIsWordQuizField+m_bIsGuildBattleField)
		return	TRUE;

	return	FALSE;
}

BOOL
cFIELD::isSecretDungeon()
{
	if	(m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
		return	TRUE;

	return	FALSE;
}

BOOL
cFIELD::isVillage()
{
	return	m_wIsVillage;
}

BOOL
cFIELD::isCanNotUseTeleportEffectField()
{
	if	(m_iUniqueSerial	==	27	||	m_wIsBlockToWarpField)
		return	TRUE;

	return	FALSE;	
}

BOOL
cFIELD::isBlockedToTeleportField()
{
	if	(m_iUniqueSerial	==	27	||	isSecretDungeon()	||	isSpecialField())
		return	TRUE;

	return	FALSE;	
}

BOOL
cFIELD::isExistTrafficObjectToOpenPitchmanShop(int _iX,int _iY)
{
	int	i;
	
	for (i=m_aiActorCount[2];i;--i)
	{
		cACTOR	*lpActor	=	getTestedActor(m_awActorList[2][i-1]);

		if (!lpActor	|| !lpActor->isNpc())
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if (!lpNpc		|| !lpNpc->isNotMoveNpc())
			continue;

		int	iRange	=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

		if (iRange	<=	dPITCHMAN_SHOP_SIZE*dPITCHMAN_SHOP_SIZE)
			return	TRUE;
	}

	for (i=dMAX_PORTAL_AREA_COUNT;i;--i)
	{
		int	iArea	=	m_pArea->m_aPortal[i-1];

		if (iArea	==	0xffffffff)
			continue;

		cAreaInfo	*lpArea	=	m_pArea->getArea(iArea);

		if (!lpArea)
			continue;

		cPOINT	pos;

		lpArea->getCenterPos(&pos);

		int	iRange	=	GetOvalRange(_iX,_iY,pos.x,pos.y);

		if (iRange	<=	dPITCHMAN_SHOP_SIZE*dPITCHMAN_SHOP_SIZE)
			return	TRUE;
	}

	return	g_pitchmanShopManager.isExistNearestShop(_iX,_iY,m_iUniqueSerial);
}

BOOL
cFIELD::isExistTrafficObjectToOpenPortal(int _iX,int _iY)
{
	const	int	c_iSelectPortalSize	=	300;

	int	i;

	for (i=m_aiActorCount[2];i;--i)
	{
		cACTOR	*lpActor	=	getTestedActor(m_awActorList[2][i-1]);

		if	(!lpActor	|| !lpActor->isNpc())
			continue;

		cNPC	*lpNpc	=	lpActor->getNpc();

		if	(!lpNpc		|| !lpNpc->isNotMoveNpc())
			continue;

		int	iRange	=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

		if	(iRange	<=	c_iSelectPortalSize*c_iSelectPortalSize)
			return	TRUE;
	}

	for (i=dMAX_PORTAL_AREA_COUNT;i;--i)
	{
		int	iArea	=	m_pArea->m_aPortal[i-1];

		if	(iArea	==	0xffffffff)
			continue;

		cAreaInfo	*lpArea	=	m_pArea->getArea(iArea);

		if	(!lpArea)
			continue;

		cPOINT	pos;

		lpArea->getCenterPos(&pos);

		int	iRange	=	GetOvalRange(_iX,_iY,pos.x,pos.y);

		if	(iRange	<=	c_iSelectPortalSize*c_iSelectPortalSize)
			return	TRUE;
	}

	return	FALSE;
}

//
//	_iRange안에 플레이어가 있냐?
BOOL
cFIELD::isExistDownrangeEnemy(int _iXPos,int _iYPos,int _iRange,int _iClientTeam)
{
	int	i;

	_iRange	=	_iRange*_iRange;

	for (int iTeam=0;iTeam<dTEAM_COUNT;iTeam++)
	{
		if (_iClientTeam	==	iTeam)
			continue;

		for (i=m_aiActorCount[iTeam];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[iTeam][i-1]);

			if (!lpActor || lpActor->isDeath()	)
				continue;
			
			int	iRange	=	GetOvalRange(_iXPos,_iYPos,lpActor->m_pos.x,lpActor->m_pos.y);

			if (iRange	<=	_iRange)	
				return	TRUE;
		}
	}

	return	FALSE;
}

//
//	1초에 한번씩 처리해야 할 작업.
void
cFIELD::operateOncePerSecondWork()
{		// 마지막 수정일 : 09.10.07
	g_debugSign.m_iTempStep	=	5;
	operateRegenNpc();
	g_debugSign.m_iTempStep	=	6;
	updateReadyToRebirthTimeActor();
	g_debugSign.m_iTempStep	=	7;
	updateMonsterBeatInfo();
	g_debugSign.m_iTempStep	=	8;
	updateTrap();
	g_debugSign.m_iTempStep	=	9;

	updateDuelField();

	m_pArea->operate1SecondWork();

	operateKarma();

	if(m_iWeatherSecond)		// 날씨 시간 감소+_+;
	{
		--m_iWeatherSecond;
		if(!m_iWeatherSecond)
		{
			m_wWeather = eWEATHER_FINE;
			for (int i=m_aiActorCount[0];i;i--)
			{
				cACTOR	*lpActor	=	getTestedActor(m_awActorList[0][i-1]);
				
				if	(!lpActor)
					continue;
				
				lpActor->sendEtcWork(eEW_CHANGE_WEATHER, m_wWeather);
			}
			
		}
		
	}
//	if	(m_wIsOXQuizField	||	m_wIsWordQuizField)
//		return;

	if	(g_bIsReceivedGuildBattleSchedule && g_bIsCompleteReceiveGuildDataWork)
	{
		checkExterminatedGuild();
		g_debugSign.m_iTempStep	=	10;
		operateGuildBattleWorkPerSecond();
	}

	g_debugSign.m_iTempStep	=	11;
	m_portal.update(this);
	g_debugSign.m_iTempStep	=	12;

	int	i;

	if (m_bIsGuildBattleField)
	{
		g_debugSign.m_iTempStep	=	13;

		for (i=m_aiActorCount[0];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[0][i-1]);

			if	(!lpActor)
				continue;

			lpActor->operateOncePerSecondWorkForPlayer();
		}

		g_debugSign.m_iTempStep	=	14;

		for (i=m_aiActorCount[1];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[1][i-1]);

			if	(!lpActor)
				continue;

			lpActor->operateOncePerSecondWorkForPlayer();
		}

		g_debugSign.m_iTempStep	=	15;

		for (i=m_aiActorCount[2];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[2][i-1]);

			if	(!lpActor)
				continue;
			if(lpActor->isDeath())
				continue;

			lpActor->operateOncePerSecondWorkForMonster();
		}
	}
	else
	{
		g_debugSign.m_iTempStep	=	16;

		for (i=m_aiActorCount[0];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[0][i-1]);

			if	(!lpActor)
				continue;

			lpActor->operateOncePerSecondWorkForPlayer();
		}

		g_debugSign.m_iTempStep	=	17;

		for (i=m_aiActorCount[1];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[1][i-1]);

			if	(!lpActor )
				continue;

			if(lpActor->isDeath())
				continue;

			if	(lpActor->isPlayer())
			{
				CLOG("monsterFailed","Monster");
				continue;
			}

			lpActor->operateOncePerSecondWorkForMonster();
		}
	}
	g_debugSign.m_iTempStep	=	18;
}

//
//	10초에 한번씩 처리해야 할 작업.
void
cFIELD::operateOncePer10SecondWork()
{
	operateDroppedItem();
	
	int		iRemoveArcaCount=0;
	WORD	awRemoveArca[100];

	for (int i=0,iCount=0;i<dMAX_ARCA_COUNT_IN_FIELD && iCount<m_wArcaCount;i++)
	{
		int		iArcaSerial	=	m_awArcaList[i];

		if	(iArcaSerial	==	0xffff)
			continue;

		iCount++;

		cArca	*lpArca	=	g_arcaManager.get(iArcaSerial);

		if	(!lpArca	||	lpArca->getField()	!=	this)
			continue;

		if	(lpArca->m_wLifeTime)
		{
			lpArca->m_wLifeTime--;

			if	(lpArca->m_wLifeTime	==	0)
			{
				if (iRemoveArcaCount	>=	100)
					lpArca->m_wLifeTime	=	1;
				else
					awRemoveArca[iRemoveArcaCount++]	=	m_awArcaList[i];
				continue;
			}
		}

		if	(lpArca->m_wSealedTime)
		{
			lpArca->m_wSealedTime--;

			if	(lpArca->m_wSealedTime	==	0)
			{
				lpArca->m_wOwner		=	0xffff;
				lpArca->m_wOwnerParty	=	0xffff;
			}
		}
	}

	for (i=0;i<iRemoveArcaCount;i++)
		g_arcaManager.remove(awRemoveArca[i]);
}

//
//	1분에 한번씩 처리해야 할 작업
void
cFIELD::operateOncePerMinuteWork()
{
	m_pArea->operate1MinuteWork();

	int	i,iTeam;

	for (iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		for (i=m_aiActorCount[iTeam];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[iTeam][i-1]);

			if	(!lpActor || lpActor->isDeath()	)
				continue;

			lpActor->operateOncePerMinuteWorkForPlayer();
		}

	for (i=0;i<dMAX_ACTOR_IN_FIELD;i++)
		if (m_alpActor[i])
			if (m_alpActor[i]->m_wSerialInField	==	0xffff)
				m_alpActor[i]	=	NULL;

	checkSecretDungeonParty();
	// 이벤트 타임 체크
	if(g_eventTimeManager.update())
		sendEventTimeUpdate();
}

void
cFIELD::operateOncePerHourWork()
{
	int	i,iTeam;

	for (iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		for (i=m_aiActorCount[iTeam];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[iTeam][i-1]);

			if (!lpActor || lpActor->isDeath()	)
				continue;

			lpActor->operateOncePerHourWorkForPlayer();
		}
}


//
//	플레이어의 게임 플레이시간을 증가 시켜준다.
void
cFIELD::increasePlayerPlayTime()
{
	int	i,iTeam;

	for (iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		for (i=m_aiActorCount[iTeam];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[iTeam][i-1]);

			if	(!lpActor)
				continue;

			lpActor->m_dwPlayTime++;

			if	(isSecretDungeon()	==	FALSE)
				if	(g_bIsInnerTestServer	||	(lpActor->m_dwPlayTime	%	dCHECK_PERIOD_FOR_TREASURE_MAP	==	0))
				{
					int	iChance	=	(int)(sqrt(m_iWidth*m_iHeight)*100/20);

					if	(g_bIsInnerTestServer)
						iChance		=	10000;

					if	(random(10000)	<	iChance)
						lpActor->m_bf1IsChanceToGetTreasureMap	=	TRUE;
				}

			if	(lpActor->isPet())
			{
				cACTOR	*lpTamer		=	lpActor->getTamer();

				if (lpTamer)
				{
					cAbility	*lpGlareAbility	=	lpTamer->getGlareSkill();
					int			iGlareSkillLevel=	0;

					if (lpGlareAbility)
						iGlareSkillLevel	=	lpGlareAbility->m_wLevel;

					if (lpActor->m_dwPlayTime%dPET_LOYALTY_UPDATE_PERIOD	==	0)
					{
//	충성도 증가량 = 조련사의 카리스마 * 0.05 + 조련사의 노려보기 스킬 레벨 * 0.05 (%)	
						int			iIncreaseLoyalty=	(lpTamer->m_charisma.m_sValue*5+iGlareSkillLevel*5)/10;

						lpActor->m_wPetLoyaltyValue	+=	iIncreaseLoyalty;
						lpActor->m_wPetLoyaltyValue = min(dPET_LOYALTY_MAX, lpActor->m_wPetLoyaltyValue);			// 펫충성도 증가 제한..200 까지.. 09.09.22
					}

					lpTamer->sendPetInfo(lpActor->m_wPetIndex,lpActor->m_wSerialInField,FALSE);
//펫 충성도는 0%~100% 사이의 값을 가진다. 단, 노려보기 스킬의 패시브 능력을 통해 최대치를 200%까지 증가시킬 수 있다.	
//	충성도 최대치 = 100% + 2% * 노려보기의 순수한 스킬 레벨(아이템이나 칭호 등 기타 부수적인 방법을 통해 올린 것 제외)

					lpTamer->m_aPet[lpActor->m_wPetIndex].m_bf14Loyalty		=	min(lpActor->m_wPetLoyaltyValue,1000+20*iGlareSkillLevel);
					lpTamer->m_aPet[lpActor->m_wPetIndex].m_wBreedingTime	=	(WORD)min(lpActor->m_dwPlayTime,60000);
				}
			}
		}
}

BOOL
cFIELD::addPortal(cACTOR *_lpCaster,int _iVillage,int _iRemainTime,int _iX,int _iY)
{
	DWORD	dwPartyUniqueSerial		=	0xffffffff;

	cParty	*lpParty	=	_lpCaster->getParty();

	if	(lpParty)
		dwPartyUniqueSerial	=	lpParty->m_dwUniqueSerial;

	if	(isExistTrafficObjectToOpenPitchmanShop(_lpCaster->m_pos.x,_lpCaster->m_pos.y))
	{
		_lpCaster->sendRegistServerMessage(eRSM_EXIST_TRAFFIC_OBJECT_TO_OPEN_PORTAL);
		return	FALSE;
	}

	int		iPortal	=	m_portal.addPortal(_lpCaster,_iVillage,_iRemainTime,_iX,_iY,dwPartyUniqueSerial);

	if	(iPortal	==	0xffff)
	{
		_lpCaster->sendRegistServerMessage(eRSM_TOO_MANY_PORTAL_IN_THIS_FIELD);
		return	FALSE;
	}

	BOOL bIsTownPortal	=	FALSE;

	if	(_iX == 0xffff && _iY == 0xffff)
		bIsTownPortal	=	TRUE;

	addPacketAddPortal(iPortal,_lpCaster->m_pos.x,_lpCaster->m_pos.y,_lpCaster->m_wSerialInField,_iVillage,bIsTownPortal);

	return	TRUE;
}

BOOL
CFieldInfoManager::isVillage(int _iField)
{
	if	(m_aField[_iField].m_wSerial	==	0xffff)
		return	FALSE;

	if	(m_aField[_iField].m_wType	==	eFT_VILLAGE)
		return	TRUE;

	return	FALSE;
}

int
CFieldInfoManager::getDistance(int _iField1,int _iField2)
{
	if (_iField1 == _iField2)
		return	0x7fffffff;

	if (m_aField[_iField1].m_wSerial	==	0xffff	||
		m_aField[_iField2].m_wSerial	==	0xffff)
		return	0x7fffffff;

	int	iRange;

	if (m_aField[_iField1].x==m_aField[_iField2].x&&m_aField[_iField1].y==m_aField[_iField2].y)
		iRange	=	abs(m_aField[_iField1].z-m_aField[_iField2].z);
	else
	{
		iRange	=	GetRange(m_aField[_iField1].x,m_aField[_iField1].y,m_aField[_iField2].x,m_aField[_iField2].y);
		iRange	=	(int)sqrt(iRange);
		iRange	+=	abs(m_aField[_iField1].z*20);
		iRange	+=	abs(m_aField[_iField2].z*20);
	}

	iRange		=	iRange*16;

	return	iRange;
}

int
cFIELD::getMasterInstanceFieldSerial()
{
	if (m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		return	0xffff;

	return	m_iUniqueSerial/dINSTANCE_FIELD_FLAG;
}

cInstanceField*
cFIELD::getMasterInstanceField()
{
	if (m_iUniqueSerial	<	dINSTANCE_FIELD_FLAG)
		return	NULL;

	int	iIF	=	m_iUniqueSerial/dINSTANCE_FIELD_FLAG;

	return	g_pIfManager->get(iIF);
}

void
cFIELD::checkSecretDungeonParty()
{
	if	(m_wIsExistSecretDungeonGate	==	FALSE	||	m_wLinkSecretDungeon	==	0xffff)
		return;

	cSecretDungeon	*lpDungeon	=	g_pSdManager->get(m_wLinkSecretDungeon);

	if	(!lpDungeon)
		return;

	if	(lpDungeon->m_bf1IsTestDungeon)
		if	(g_bIsTestServer	==	FALSE)
			return;

	if	(m_wRemainMinuteForRemoveSecretDungeonKeyCount)
	{
		m_wRemainMinuteForRemoveSecretDungeonKeyCount--;

		if	(m_wRemainMinuteForRemoveSecretDungeonKeyCount	==	0)
			if	(lpDungeon->m_wActiveKeyCount)
				lpDungeon->m_wActiveKeyCount--;
	}

	if	(m_wRemainMinuteForCheckSecretDungeonParty)
		m_wRemainMinuteForCheckSecretDungeonParty--;

	if	(g_pIfManager->isFullSecretDungeon(m_wLinkSecretDungeon,TRUE))
		return;

	if	(m_wRemainMinuteForCheckSecretDungeonParty	!=	0)
		return;

	m_dwBookedSecretDungeonKeyParty				=	0xffffffff;
	m_wRemainMinuteForCheckSecretDungeonParty	=	1;

	int		iPartyCount	=	0,i;
	int		iScoreSum	=	0;
	WORD	awPartyList[100];
	int		aiScore[100];

	for	(i=m_aiActorCount[0];i;i--)
	{
		cACTOR	*lpActor	=	getTestedActor(m_awActorList[0][i-1]);

		if	(!lpActor || lpActor->isDeath() || lpActor->isIdlePlayer())
			continue;

		cParty	*lpParty	=	lpActor->getParty();

		if	(!lpParty || lpParty->m_bf3PartyObject != ePO_EXPLORATION)
			continue;

		if	(g_bIsInnerTestServer	==	FALSE)
			if	(lpParty->m_wLeaderPlayMinuteOnThisFieldTime	<	10)
				continue;

		if	(lpParty->isLeader(lpActor->m_strName)	==	FALSE)
			continue;

		if	(lpParty->getItemByBasicItem(g_im.m_wSecretDungeonKey))
			continue;

		iScoreSum				+=	lpParty->getSecretDungeonScore();
		aiScore[iPartyCount]	=	iScoreSum;
		awPartyList[iPartyCount]=	lpActor->getPartySerial();
		iPartyCount++;

		if	(iPartyCount	>=	100)
			break;
	}

	if	(iPartyCount	==	0)
		return;

	int	iValue	=	random(iScoreSum);
	int	iIndex	=	-1;

	for (i=0;i<iPartyCount;i++)
	{
		if	(iValue	<	aiScore[i])
		{
			iIndex	=	i;
			break;
		}
	}

	cParty	*lpParty	=	g_pPartyManager->get(awPartyList[iIndex]);

	if	(!lpParty)
		return;

	m_wRemainMinuteForCheckSecretDungeonParty	=	dSERECT_DUNGEON_KEY_REGEN_PERIOD;

	m_dwBookedSecretDungeonKeyParty				=	lpParty->m_dwUniqueSerial;
}


//
//	1초에 한번씩 처리해야 할 작업.
void
cFIELD::rebuildRedstonePointEffect()
{
	int	i;

	if (m_bIsGuildBattleField)
	{
		for (i=m_aiActorCount[0];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[0][i-1]);

			if	(!lpActor)
				continue;

			lpActor->checkRedStonePoint();
		}

		for (i=m_aiActorCount[1];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[1][i-1]);

			if	(!lpActor)
				continue;

			lpActor->checkRedStonePoint();
		}
	}
	else
	{
		for (i=m_aiActorCount[0];i;i--)
		{
			cACTOR	*lpActor	=	getTestedActor(m_awActorList[0][i-1]);

			if	(!lpActor)
				continue;

			lpActor->checkRedStonePoint();
		}
	}
}

cCHARACTER*
cFIELD::getCharacterByJob(int _iBaseJob)
{
	for	(int i=0;i<dCHARACTER_COUNT;i++)
	{
		if	(m_aCharacter[i].m_wSerial	==	0xffff)
			continue;

		if	(g_aJob[m_aCharacter[i].m_wBaseJob].m_wBody	==	_iBaseJob)
			return	&m_aCharacter[i];
	}

	return	NULL;
}

void
cFIELD::exportSkillList()
{
	CLOGWT("active_skill_info","[%.3d]%s : count of active skill %d\n",m_iUniqueSerial,m_strName,m_skillManager.m_iActiveSkillCount);

	for	(int i=0;i<dMAX_ACTIVE_SKILL;i++)
	{
		CActiveSkill	*lpAS	=	m_skillManager.getActiveSkill(i);

		if	(!lpAS)
			continue;
		
		CSkill	*lpSkill	=	lpAS->m_ability.getSkill();

		if	(lpSkill)
			CLOGWT("active_skill_info","[%.3d] %s\n",i,lpSkill->m_strName);
		else
			CLOGWT("active_skill_info","[%.3d] %d\n",i,lpAS->m_ability.m_wSkill);
	}
}