#include "CGamePlay.H"
#include "cJOB.H"
#include "CActor.H"
#include "CAgent.H"
#include "CWindowInterface.H"
#include "CMessage.H"
#include "CGamePlayInterface.H"
#include "bm_event.h"
#include "dialog_box.H"
#include "duel.H"
#include "book_MonsterDictionary.h"

int		g_iFieldScreenHeight	=	516;

cTHREAD			CGamePlay::s_thLoading;
int				CGamePlay::s_iPreparationsForGameStatus	=	dPREPARATION_FOR_GAME_STATUS_INIT;
int				CGamePlay::s_iDataLoadingStatus			=	dDATA_LOADING_STATUS_PROCESS;
CFieldTitle		CGamePlay::s_fieldTitle;
CFieldTitle		CGamePlay::s_UpgradeFieldTitle;				// 09.09.02 강화 타이틀
CShop			CGamePlay::s_shop;

BOOL			CGamePlay::s_bIsFullScreenInterface		=	FALSE;
BOOL			CGamePlay::s_bIsOpenEventWindow			=	FALSE;
BOOL			CGamePlay::s_bIsSecretDungeon			=	FALSE;
BOOL			CGamePlay::s_bIsBossZone				=	FALSE;
BOOL			CGamePlay::s_bIsMoveFieldAtSameServer	=	FALSE;
BOOL			CGamePlay::s_bIsFailedToJoinGame		=	FALSE;
BOOL			CGamePlay::s_bIsMoveField				=	FALSE;
BOOL			CGamePlay::s_bIsProgressEvent			=	FALSE;
tuPropose		CGamePlay::s_propose;
int				CGamePlay::s_iBookedChangeGuildTax;
int				CGamePlay::s_iLoadingImage				=	-1;
int				CGamePlay::s_iPlayerMovePacketPeriod;
char			CGamePlay::s_strBookedUserName[dNAME_LENGTH];
SKILLITEM		CGamePlay::m_sQuickSlotItem[10][10][2];

int				g_iHeavenRedStoneCount=-1,g_iHellRedStoneCount=-1,g_iRedDevilRedStoneCount=-1;
BOOL			g_bIsWaitGetGVGAvatarOrgData	=	FALSE;

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

//
//	게임시작하기 전에 필요한 데이터들을 로딩한다.
void
CGamePlay::LoadData(void* _lpData)
{
	if	(LoadField(s_strBookingToMoveFieldFileName))
		s_iDataLoadingStatus	=	dDATA_LOADING_STATUS_COMPLETE;
	else
		s_iDataLoadingStatus	=	dDATA_LOADING_STATUS_FAILED;
}

//
//	필드 불러오기
BOOL
CGamePlay::LoadField(char *_lpstrMapName)
{
	if	(!g_map.load(_lpstrMapName,NULL)					)
		return	FALSE;	//	이건 계속 바뀌는거
	if	(!g_map.loadFieldImageData(FALSE,NULL,&s_mcuFieldImage))
		return	FALSE;	//	이건 설정에 따라 다름
	if	(!g_jm.loadLocalBody(NULL,&s_mcuBodyImage)		)
		return	FALSE;	//	이것 역시..

	g_gwBookMonDic.reset();

	return	TRUE;
}

//
//	게임 참가 실패
void
CGamePlay::FailedToJoinGame(char *_lpstrReason)
{
	g_msgBox.cPopup(NULL,_lpstrReason,dMSG_OK);
	g_msgBox.setButtonId(eMBI_CANCEL_TO_ENTER_GAME);

	s_bIsFailedToJoinGame			=	TRUE;
}

//
//	게임 플레이를 위한 초기화
void
CGamePlay::InitGamePlay()
{
	Reset();

	strcpy(g_duelManager.m_strTrialGameName," ");

	s_iPreparationsForGameStatus	=	dPREPARATION_FOR_GAME_STATUS_DATA_LOADING;
	s_iDataLoadingStatus			=	dDATA_LOADING_STATUS_PROCESS;
	s_bIsMoveField					=	FALSE;
	s_iLoadingImage					=	-1;
	g_map.m_iTileSet				=	-1;
	g_bIsPutSceneWhenLoading		=	FALSE;
	g_duelManager.reset();
	g_gwGuild.reset();

	g_canvas.fadeIn();
	s_thLoading.Create(s_hWnd,LoadData);	//	데이터 로딩
}

void
CGamePlay::BookingMoveField(char *_strIP,char *_lpstrMapName)
{
	strcpy(s_strBookingToMoveFieldFileName,_lpstrMapName);
	strcpy(s_strBookingToMoveFieldServeAddress,_strIP);
}

//
//	필드를 이동한다.
void
CGamePlay::MoveField()
{
	s_bIsMoveField	=	TRUE;
	
	g_msgBox.close();
	s_agent.blocking();

	s_bIsBookedDisableChattingBox	=	TRUE;

	g_hero.m_wWaitMoveAreaResultTime=	0;

	if	(STRICMP(s_strBookingToMoveFieldServeAddress,"")==0)
	{
		s_bIsMoveFieldAtSameServer	=	TRUE;

		BookingMode(eGAME_PLAY,dSTEP_FINISH,dFADE_OUT);
	}
	else
	{
		s_bIsMoveFieldAtSameServer	=	FALSE;
		strcpy(s_strGameServerAddress,s_strBookingToMoveFieldServeAddress);
		s_iConnectType				=	eCONNECT_TYPE_MOVE_FIELD;

		BookingMode(eGAME_CONNECT_GAME_SERVER,dSTEP_FINISH,dFADE_OUT);
	}
}

//
//	초기화
BOOL
CGamePlay::UpdateInit()
{
//	BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
	mBeginTLog();

	g_eh.addLog("gp:ui %d",s_iPreparationsForGameStatus);

	OperateMessageBox();	//	메시지 박스 처리

	if	(s_agent.isDisconnected())
	{
		g_msgBox.cPopup(NULL,s_agent.m_strDisconnectText,dMSG_OK);
		g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
		s_agent.reset();

		return TRUE;
	}

	s_agent.packetHandler();

	switch(s_iPreparationsForGameStatus)
	{
		case	dPREPARATION_FOR_GAME_STATUS_INIT				:
			s_bIsFailedToJoinGame	=	FALSE;
			InitGamePlay();
			break;

//	데이터를 로딩하는 중이다.
		case	dPREPARATION_FOR_GAME_STATUS_DATA_LOADING		:
			if	(s_iDataLoadingStatus	==	dDATA_LOADING_STATUS_COMPLETE)
			{
				g_eh.addLog("gp:ui dDATA_LOADING_STATUS_COMPLETE 1");

				for (int i=0;g_map.m_strFileName[i];i++)
					g_map.m_strFileName[i]	=	tolower(g_map.m_strFileName[i]);
				
				if	(strstr(g_map.m_strFileName,"expansion") || strstr(g_map.m_strFileName,"duel"))
					PlayBgm(g_map.m_aBgmList[0],FALSE);
				else
					PlayBgm(g_map.m_iSerial);

				s_iPreparationsForGameStatus=	dPREPARATION_FOR_GAME_STATUS_WAIT_JOIN_RESULT;

				s_agent.sendJoinGame();
				g_msgBox.cPopup(NULL,dMSG_WAIT_SERVER_MESSAGE,dMSG_CANCEL);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				g_msgBox.setWaitPacket(dSG_SUCCESS_TO_JOIN_GAME);
				
				//길드 정보를 기다림.
				//if(g_iSeasonVariable >= 3)
// 				{			
// 					s_agent.sendAskGuildMemberList();
// 					g_msgBox.cPopup(NULL,"길드 정보를 기다립니다.",dMSG_CANCEL);
// 					g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
// 					g_msgBox.setWaitPacket(dSG_GUILD_MEMBER_LIST);
// 				}
				
				g_eh.addLog("gp:ui dDATA_LOADING_STATUS_COMPLETE 2");
			}

			if	(s_iDataLoadingStatus	==	dDATA_LOADING_STATUS_FAILED)
			{
				g_eh.addLog("gp:ui dDATA_LOADING_STATUS_FAILED 1");
				g_msgBox.cPopup(NULL,dMSG_FAILED_TO_JOIN_GAME,dMSG_OK);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				s_bIsFailedToJoinGame		=	TRUE;
				s_iPreparationsForGameStatus=	dPREPARATION_FOR_GAME_STATUS_WAIT_JOIN_RESULT;
				BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
				g_eh.addLog("gp:ui dDATA_LOADING_STATUS_FAILED 2");
			}
			break;

//	게임 참가 결과를 기다린다.
		case	dPREPARATION_FOR_GAME_STATUS_WAIT_JOIN_RESULT	:
			if	(s_agent.isReceivedPacket(dSG_SUCCESS_TO_JOIN_GAME))//게임 참가 왔다.
			{
				g_eh.addLog("gp:ui dSG_SUCCESS_TO_JOIN_GAME 1");

				int	iFireResistanceLevel	=	g_map.getCorrectFireResistanceLevel();
				int	iWaterResistanceLevel	=	g_map.getCorrectWaterResistanceLevel();
				int	iWindResistanceLevel	=	g_map.getCorrectWindResistanceLevel();
				int	iEarthResistanceLevel	=	g_map.getCorrectEarthResistanceLevel();
				int	iLightResistanceLevel	=	g_map.getCorrectLightResistanceLevel();
				int	iDarkResistanceLevel	=	g_map.getCorrectDarkResistanceLevel();

				if	(s_bIsObserverMode	==	FALSE)
				{
					if	(iFireResistanceLevel)
						CGamePlay::AddSystemMessage(CTBLUE,g_pstrCorrectMagicResistance[iFireResistanceLevel-1]);
					if	(iWaterResistanceLevel)
						CGamePlay::AddSystemMessage(CTBLUE,g_pstrCorrectMagicResistance[11+iWaterResistanceLevel-1]);
					if	(iWindResistanceLevel)
						CGamePlay::AddSystemMessage(CTBLUE,g_pstrCorrectMagicResistance[22+iWindResistanceLevel-1]);
					if	(iEarthResistanceLevel)
						CGamePlay::AddSystemMessage(CTBLUE,g_pstrCorrectMagicResistance[33+iEarthResistanceLevel-1]);
					if	(iLightResistanceLevel)
						CGamePlay::AddSystemMessage(CTBLUE,g_pstrCorrectMagicResistance[44+iLightResistanceLevel-1]);
					if	(iDarkResistanceLevel)
						CGamePlay::AddSystemMessage(CTBLUE,g_pstrCorrectMagicResistance[55+iDarkResistanceLevel-1]);
				}

				s_iPreparationsForGameStatus	=	dPREPARATION_FOR_GAME_FINISH;
				BookingStep(dSTEP_READY,dFADE_IN);
				g_am.setFocus();
				s_iStep	=	dSTEP_READY;

// 				else
// 				{
// 					s_iPreparationsForGameStatus	=	dPREPARATION_FOR_GAME_STATUS_GUILD_DATA_LOADING;
// 				}
// 				BookingStep(dSTEP_READY,dFADE_IN);
// 				g_am.setFocus();
// 				s_iStep	=	dSTEP_READY;


				g_eh.addLog("gp:ui dSG_SUCCESS_TO_JOIN_GAME 2");
				break;
			}
			if	(s_agent.isReceivedPacket(dSG_RESULT_JOIN))//게임 참가 왔다.
			{
				g_eh.addLog("gp:ui dSG_RESULT_JOIN 1");

				switch(GetJoinResult())
				{
					case	dRESULT_JOIN_FIELD_SUCCESS		:	//	성공
						break;

					case	dRESULT_JOIN_FIELD_FULL			://더이상 넣을수 없다. 실제로는 이런 메세지는 나오기 힘들꺼~~ 얼 -_-a
						g_msgBox.cPopup(NULL,dMSG_FIELD_FULL,dMSG_OK);
						g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
						break;

					case	dRESULT_JOIN_FIELD_FAILED		://걍.. 실패.. -_-a
					case	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA	:
						g_msgBox.cPopup(NULL,dMSG_FAILED_TO_JOIN_GAME,dMSG_OK);
						g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
						break;

					case	dRESULT_JOIN_FIELD_FAILED_BY_EXIST_PLAYER	:
						g_msgBox.cPopup(NULL,dMSG_FAILED_TO_JOIN_GAME_BY_EXIST_SAME_PLAYER,dMSG_OK);
						g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
						break;
/*
					case	dRESULT_JOIN_FIELD_FAILED_BY_INCORRECT_DATA	:
						g_msgBox.cPopup(NULL,dMSG_FAILED_TO_JOIN_GAME_BY_INCORRECT_DATA,dMSG_OK);
						g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
						break;*/
				}
				g_eh.addLog("gp:ui dSG_RESULT_JOIN 2");
			}
			break;
			//길드 데이터 로딩.
		case	dPREPARATION_FOR_GAME_STATUS_GUILD_DATA_LOADING		:
				s_iPreparationsForGameStatus=	dPREPARATION_FOR_GAME_STATUS_GUILD_DATA_LOADING_WAIT;

				//길드 정보를 기다림.
				s_agent.sendAskGuildMemberList(TRUE);
				g_msgBox.cPopup(NULL,dMSG_WAIT_GUILD_INFO,dMSG_CANCEL);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				g_msgBox.setWaitPacket(dSG_GUILD_MEMBER_LIST);
			break;

//	게임 참가 결과를 기다린다.
		case	dPREPARATION_FOR_GAME_STATUS_GUILD_DATA_LOADING_WAIT	:
			if	(s_agent.isReceivedPacket(dSG_GUILD_MEMBER_LIST))				//게임 참가 왔다.
			{
				s_iPreparationsForGameStatus	=	dPREPARATION_FOR_GAME_FINISH ;
				BookingStep(dSTEP_READY,dFADE_IN);
				g_am.setFocus();
				s_iStep	=	dSTEP_READY;	
			}

		break;
	}

	return	SAFETY;
}

//
//
BOOL
CGamePlay::UpdateReady()
{
	s_iPreparationsForGameStatus	=	dPREPARATION_FOR_GAME_STATUS_INIT;

	BookingStep(dSTEP_MAIN);

	{
		CG_STOP	packet;

		packet.base.set(sizeof(CG_STOP),dCG_STOP);

		packet.wDirect	=	0;
		packet.wPosX	=	0;
		packet.wPosY	=	0;

		s_agent.sendPacket((char*)&packet,packet.base.wSize);
	}

#ifdef _DEBUG
	if	(s_bIsGuildHall)
		s_fieldTitle.init(_ms("Guild Hall : %s[%.4d]",s_strGuildHallOwnerGuildName,s_iFieldSerial));
	else
	if	(g_aMapSimpleInfo[s_iFieldSerial].m_bf1IsPremiumZone)
		s_fieldTitle.init(_ms("%s[%.4d P]",g_map.m_strName,s_iFieldSerial));
	else
		s_fieldTitle.init(_ms("%s[%.4d]",g_map.m_strName,s_iFieldSerial));
#else
	if	(s_bIsGuildHall)
		s_fieldTitle.init(_ms("Guild Hall : %s",s_strGuildHallOwnerGuildName));
	else
	if	(g_aMapSimpleInfo[s_iFieldSerial].m_bf1IsPremiumZone)
		s_fieldTitle.init(_ms("%s[P]",g_map.m_strName));
	else
		s_fieldTitle.init(g_map.m_strName);
#endif

	s_iFrameCounter	=	0;

	return	SAFETY;
}

//
//
BOOL
CGamePlay::UpdateFinish()
{
	if	(s_bIsFailedToJoinGame)
		BookingStep(dSTEP_CLOSE);

	if	(g_canvas.isCompleteFadeOut())
		BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

//
//	닫기
BOOL
CGamePlay::UpdateClose()
{
	s_agent.releaseBlock();

	if	(s_bIsFailedToJoinGame	==	FALSE)
		g_hero.saveUserData();
	
	ClosePopupWindow();
	cINPDEV::ReleaseExclusive();

	s_iPreparationsForGameStatus	=	dPREPARATION_FOR_GAME_STATUS_INIT;

	if	(s_iBookingMode	==	eGAME_TITLE)
	{
		s_bmBottomMenu.close();
		s_bmButton.close();
		s_bmPlusState.close();

		s_agent.reset();//	연결을 끊는다.
		g_map.close();	//	맵 초기화
		g_am.close();
		s_partyInfo.reset();
		s_agent.disconnectBroadCastServer();
	}

	return	SAFETY;
}

//
//	돌려라 돌려~
BOOL
CGamePlay::Run()
{
	DWORD	dwCurrentTime	=	timeGetTime();
	DWORD	dwPastSecond;

	if	(dwCurrentTime	>	s_dwReceiveServerTime)
		dwPastSecond	=	(dwCurrentTime-s_dwReceiveServerTime)/1000;
	else
		dwPastSecond	=	((0xffffffff-s_dwReceiveServerTime)+dwCurrentTime)/1000;

	memcpy(&s_currentTime,&s_serverTime,sizeof(CTimeInfo));
	if(CGame::s_bIsFirstLogin == FALSE)
	{
		CGame::s_bIsFirstLogin = TRUE;
		s_dwLoginTime = 0;
	}
	
	s_currentTime.increaseSecond(dwPastSecond);
	
	switch(s_iStep)
	{
		case	dSTEP_INIT		:
			return	UpdateInit();

		case	dSTEP_READY		:
			return	UpdateReady();

		case	dSTEP_MAIN		:
			return	UpdateMain();

		case	dSTEP_FINISH	:
			return	UpdateFinish();

		case	dSTEP_CLOSE		:
			return	UpdateClose();
	}

	return	TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
void
CGamePlay::DrawInit()
{
	if	(g_bIsTerminalPC)
	{
		cDRAW::Clear();

		if	(timeGetTime()	%	1000 < 500)
			s_text.put(5,g_iScreenHeight-20,WHITE,"아무튼... 로딩중.. ( ㅡ.-)す~~");
		else
			s_text.put(5,g_iScreenHeight-20,WHITE,"아무튼... 로딩중.. ( ㅡ.-)す~");

		return;
	}

	if	(g_map.m_iTileSet	==	-1)
		return;

	int iRateX = 100;
	int iRateY = 100;
	int iAddY = 0;
	if (g_iScreenWidth != 800)
	{
		iRateX = (int)(g_iScreenWidth*100)/800;
		iRateY = (int)(g_iScreenHeight*100)/600;
		iAddY = g_iScreenHeight-600;
	}

	if	(s_iLoadingImage	==	-1)
	{
		int		aiNonImage[]	=	{8,9,16,17,18,19,20,-1};

		g_bIsPutSceneWhenLoading	=	TRUE;

		for(int i=0;;i++)
		{
			if	(aiNonImage[i]		==	-1)
				break;
			if	(g_map.m_iTileSet	==	aiNonImage[i])
			{
				g_bIsPutSceneWhenLoading	=	FALSE;
				break;
			}
		}

		if	(g_bIsPutSceneWhenLoading==	FALSE)
		{
			s_iLoadingImage	=	timeGetTime()%g_sprLoadingPannel.m_iCount;

			cDRAW::Clear();

			g_sprLoadingPannel.Put(g_iScreenWidth/2,g_iScreenHeight/2,s_iLoadingImage, iRateX, iRateY);
		}
		else
		{
			g_smiLoading.putScale(g_map.m_iTileSet,/*g_iCorrectBI_X/2*/0, /*g_iCorrectBI_Y/2*/0, iRateX);
		}
	}

	if	(g_canvas.getFadeStatus()	!=	dFADE_NONE)
	{
		if	(g_bIsPutSceneWhenLoading==	FALSE)
			g_sprLoadingPannel.Put(g_iScreenWidth/2,g_iScreenHeight/2,s_iLoadingImage, iRateX, iRateY);
		else
			g_smiLoading.putScale(g_map.m_iTileSet,/*g_iCorrectBI_X/2*/0,/*g_iCorrectBI_Y/2*/0, iRateX);			
	}

	DrawLoadingText();
	
}

//
//
void
CGamePlay::DrawReady()
{
	DrawMain();
//	cDRAW::FillAB(0,s_iScreenFadeValue);
//	cDRAW::Clear();
}

//
//
void
CGamePlay::DrawFinish()
{
	if	(g_canvas.isCompleteFadeOut())
		cDRAW::Clear();
	else
		DrawMain();
}

//
//	그리기 - 종료
void
CGamePlay::DrawClose()
{
	cDRAW::Clear();
}

//
//	그려주기
BOOL
CGamePlay::Draw()
{
	if (s_bIsFailedToJoinGame)
	{
		cDRAW::Clear();
		return	TRUE;
	}

	switch(s_iStep)
	{
		case	dSTEP_INIT		:	DrawInit();break;
		case	dSTEP_READY		:	DrawReady();break;
		case	dSTEP_MAIN		:	DrawMain();break;
		case	dSTEP_FINISH	:	DrawFinish();break;
		case	dSTEP_CLOSE		:	DrawClose();break;
	}

	return	TRUE;
}

//
//	열려진 이벤트
BOOL
CGamePlay::IsOpenedEventWindow()
{
	return	g_dialogBox.isTalking()+g_gwShop.isOpened()+IsOpenTradeBox()+g_gwGuildBattleScheduler.isOpened()+ g_gwGuildMarkComposer.isOpened()+g_gwBank.isOpened()+
			g_gwMakeGuild.isOpened()+g_gwCart.isOpened()+g_msgBox.isPoped()+s_bIsMoveField+g_gwPitchmanShop.isOpened()+g_gwRepairItem.isOpened()+
			g_gwSelectVillage.isOpened()+g_gwWorldMap.isOpened()+g_gwHelp.isOpened()+g_itemCountWindow.m_bIsOpend+g_gwPutBannerWnd.isOpened()+g_gwMainQuestEvent.isOpened()+
			g_gwBook.isOpened()+g_gwBookMonDic.isOpened()+g_bossMonsterEvent.isAvail()+g_gwGuildHallList.isOpened()+g_gwGuildInventory.isOpened()+g_gwFeedGuildPet.isOpened()+
			g_gwGuildRelatedPlaceList.isOpened()+g_gwHonorPointList.isOpened()+g_gwSelectItemInPack.isOpened() + g_gwUpgradeShop.isOpened();
}