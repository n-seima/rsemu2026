#include "CGamePlay.H"
#include "cMAP.H"
#include "CSkill.H"
#include "CHitEffect.H"
#include "effectManager.H"
#include "cSHAKE.H"
#include "CFieldItem.H"
#include "cMESSAGE.H"
#include "CAgent.H"
#include "CHero.H"
#include "CActor.H"
#include "CSound.H"
#include "CWindowInterface.H"
#include "CGame_Title.H"
#include "bm_event.h"
#include "dialog_box.h"
#include "commonUI.h"
#include "book_MonsterDictionary.h"

cBARMENU			CGamePlay::s_bmBottomMenu;//퀵스킬/퀵 아이템 버튼,메뉴 버튼
cBARMENU			CGamePlay::s_bmToolButton;
cBARMENU			CGamePlay::s_bmButton;
cBARMENU			CGamePlay::s_bmButtonI2;
cBARMENU			CGamePlay::s_bmCheckBox;
cBARMENU			CGamePlay::s_bmRadioBox;
cBARMENU			CGamePlay::s_bmMiddleWindowButton;
CTextList			CGamePlay::s_tlSystem;		//	시스템 메시지
cBARMENU			CGamePlay::s_bmCategory;
int					CGamePlay::s_iCategoryIndex = eSM_CATEGORY_SCREEN;

cRECT				CGamePlay::s_rectTooltip;
int					CGamePlay::s_iPopupInterface		=	ePIW_NONE;
int					CGamePlay::s_iDragItem				=	0xffff;	//	드래그 중인 아이템
int					CGamePlay::s_iDragSkill				=	0xffff;	//	드래그 중인 스킬
int					CGamePlay::s_iDragEquipment			=	0xffff;	//	드래그 중인 장비
CImageScrollBar		CGamePlay::s_sbPopupWindow;
CImageScrollBar		CGamePlay::s_sbMiddleWindow;
BOOL				CGamePlay::s_bDragSkillItem				= FALSE;	// 10.01.07   스킬창에서 드래그한 아이템
BOOL				CGamePlay::s_bIsCanNotControlInterface	= FALSE;	// 인터페이스 조작 불가
cRECT				CGamePlay::s_rectInterfaceSize;		// insu add .. 10.9.15
cBARMENU			CGamePlay::s_bmPlusState;

int					g_iCorrectBI_X	=	0;
int					g_iCorrectBI_Y	=	0;

CGameOption			g_config;

//
//	팝업 윈도우 닫기
BOOL
CGamePlay::ClosePopupWindow()
{
	if (g_map.m_bOptionState)
	{
		g_map.m_bOptionState = FALSE;
		return TRUE;
	}

	if (g_map.m_bQuestListState)
	{
		g_map.m_bQuestListState = FALSE;
		return TRUE;
	}

	if (g_map.m_bFogmapState)
	{
		g_map.m_bFogmapState = FALSE;
		return TRUE;
	}

	if	(g_gwGuildBattleScheduler.isOpened() + g_gwMakeGuild.isOpened() + g_gwGuildMarkComposer.isOpened()+g_gwMainQuestEvent.isOpened()+g_bossMonsterEvent.isAvail())
		return	TRUE;

	if	(g_gwPitchmanShop.isOpened() && g_gwPitchmanShop.m_iOwner == ePS_SHOP_KEEPER)
		return	TRUE;

	if	(g_gwBank.m_wIsWaitSaveResult					)
	{
		AddSystemMessage(WHITE,dMSG_WAIT_FOR_BANK_TRANSACTION_RESULT);	//	은행 거래 결과를 기다리는 중입니다.
		return	TRUE;
	}

	if(g_dialogBox.m_bDoNotCloseDialogBox == FALSE)
		g_dialogBox.closeComplexDialog();
	else
	{
		if(g_dialogBox.isTalking())
			return FALSE;
	}
	s_bmBottomMenu.resetChecker();

	if (s_iPopupInterface	==	ePIW_NONE					)	return	FALSE;
	if (s_iPopupInterface	==	ePIW_INVENTORY				)	CloseInventoryWindow();
	if (s_iPopupInterface	==	ePIW_SYSTEM					)
	{
		if	(g_lpHero->isDeath() && g_bIsDuelServer	== FALSE)
			return	TRUE;

		CloseSystemMenu();
	}
	if (s_iPopupInterface	==	ePIW_BOOK					)	g_gwBook.close();
	if (s_iPopupInterface	==	ePIW_BOOK_MONSTER_DICTIONARY)	g_gwBookMonDic.close();
	if (s_iPopupInterface	==	ePIW_PARTY					)	g_gwParty.close();
	if (s_iPopupInterface	==	ePIW_SHOP					)	g_gwShop.close();
	if (s_iPopupInterface	==	ePIW_REPAIR_ITEM			)	g_gwRepairItem.close();
	if (s_iPopupInterface	==	ePIW_SELECT_VILLAGE			)	g_gwSelectVillage.close();

#ifdef	_DEBUG
	if (s_iPopupInterface	==	ePIW_DEBUG					)	g_gwDebug.close();
#endif
	if (s_iPopupInterface	==	ePIW_SYSTEM					)	CloseSystemMenu();
	if (s_iPopupInterface	==	ePIW_TRADE_BOX				)	CloseTradeBox();
	if (s_iPopupInterface	==	ePIW_FRIEND					)	g_gwFriend.close();
	if (s_iPopupInterface	==	ePIW_QUEST					)	g_gwQuest.close();
	if (s_iPopupInterface	==	ePIW_BANK					)	g_gwBank.close();
	if (s_iPopupInterface	==	ePIW_GUILD_INVENTORY		)	g_gwGuildInventory.close();
	if (s_iPopupInterface	==	ePIW_GUILD_FEED_PET_WINDOW	)	g_gwFeedGuildPet.close();
	if (s_iPopupInterface	==	ePIW_GUILD					)	g_gwGuild.close();
	if (s_iPopupInterface	==	ePIW_WORLD_MAP				)	g_gwWorldMap.close();
	if (s_iPopupInterface	==	ePIW_HELP_SCREEN			)	g_gwHelp.close();
	if (s_iPopupInterface	==	ePIW_SHOPPING_CART			)	g_gwCart.close();
	if (s_iPopupInterface	==	ePIW_SKILL					)	g_gwSkill.close();
	if (s_iPopupInterface	==	ePIW_COMMON_LIST			)	g_gwCommonList.close();
	if (s_iPopupInterface	==	ePIW_COMMON_UI				)	g_commonUI.close();
	if (s_iPopupInterface	==	ePIW_GUILD_BATTLE_SCHEDULER	)	g_gwGuildBattleScheduler.close();
	if (s_iPopupInterface	==	ePIW_GUILD_HALL_LIST		)	g_gwGuildHallList.close();
	if (s_iPopupInterface	==	ePIW_GUILD_HONOR_POINT_LIST	)	g_gwHonorPointList.close();
	if (s_iPopupInterface	==	ePIW_GUILD_RELATED_PLACE_LIST)	g_gwGuildRelatedPlaceList.close();
	if (s_iPopupInterface	==	ePIW_GUILD_MARK_COMPOSER	)	g_gwGuildMarkComposer.close();
	if (s_iPopupInterface	==	ePIW_PITCHMAN_SHOP			)	g_gwPitchmanShop.close();
	if (s_iPopupInterface	==	ePIW_UPGRADE_SHOP			)	g_gwUpgradeShop.close();	//  닫기 09.08.26
	if (s_iPopupInterface	==	ePIW_STATUS					)	CloseStatusWindow();
	if (s_iPopupInterface	==	ePIW_CARROT_SHOP			)	g_gwCarrotShop.close();		// insu add .. 당근상점 개편

	s_iPopupInterface			=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);
	s_bIsCanNotControlInterface	=	FALSE;

	s_bmPlusState.reset();

	return	TRUE;
}


void
CGamePlay::openInterface(int iInterface, BOOL _bIsClose, BOOL _bIsCanNotControl)
{

		
	if(g_dialogBox.m_iTalker != -1)
	{
		g_dialogBox.m_bDoNotCloseDialogBox	= TRUE;
		g_dialogBox.m_posOldView = g_map.m_posView;
	}

	if(!_bIsClose)
	{
		s_bIsCanNotControlInterface		=	_bIsCanNotControl;

		switch(iInterface)
		{
			
		case	ePIW_INVENTORY				:
			{
				if(s_iPopupInterface != ePIW_INVENTORY)
					OpenInventoryWindow();
				break;
			}
		case	ePIW_STATUS					:
			{
				if(s_iPopupInterface != ePIW_STATUS)
					OpenStatusWindow();
				
				break;
			}
		case	ePIW_SKILL					:
			{
				if(s_iPopupInterface != ePIW_SKILL)
					g_gwSkill.open();
				
				break;
			}
		case	ePIW_QUEST					:
			{
				if(s_iPopupInterface != ePIW_QUEST)
					g_gwQuest.open();
				
				break;
			}
		case	ePIW_PARTY					:
			{
				if(s_iPopupInterface != ePIW_PARTY)
					g_gwParty.open();
				
				break;
			}
		case	ePIW_GUILD					:
			{
				if(s_iPopupInterface != ePIW_GUILD)
					g_gwGuild.open();
				
				break;
			}
		case	ePIW_FRIEND					:
			{
				if(s_iPopupInterface != ePIW_FRIEND)
					g_gwFriend.open();
				
				break;
			}
		case	ePIW_SYSTEM					:
			{
				if(s_iPopupInterface != ePIW_SYSTEM)
					PopupSystemMenu();
				
				break;
			}
			
		}
	}
	else
	{
		ClosePopupWindow();
	}

}


//
//	팝업 윈도우 클로즈 버튼 초기화
void
CGamePlay::InitPopupWindowCloseButton()
{
	int	iToolButtonSize	=	g_sprInterface.getSpriteWidth(eWINDOW_CLOSE_ACTIVE);

	s_bmToolButton.reset();
	s_bmToolButton.addImageBar("",eTBM_CLOSE_WINDOW	,g_iScreenWidth-iToolButtonSize,g_iCorrectBI_Y/2	,0xffff,eWINDOW_CLOSE_ACTIVE,eWINDOW_CLOSE_PRESSED,0xffff,0xffff);
}

//
//	팝업 윈도우 클로즈 버튼 초기화
void
CGamePlay::UpdatePopupWindowCloseButton()
{
	if (cINPDEV::IsExclusive())	return;

	DWORD	dwMenu	=	s_bmToolButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	if (dwMenu	==	eTBM_CLOSE_WINDOW)
	{
		ClosePopupWindow();

		return;
	}
}

//
//	팝업 윈도우 클로즈 버튼 초기화
void
CGamePlay::DrawPopupWindowCloseButton()
{
	s_bmToolButton.draw();
}

//
//
void
CGamePlay::LoadConfigData()
{
	cFILE	fileConfig;

#ifdef	_FOR_CHINA
	char	*lpstrFileName	=	"data/@@lsaf_china.dat";

	if	(fileConfig.Open(lpstrFileName,"rb"))
	{
		int	iCount	=	0;

		fileConfig.Read(&s_iLoginServerLocalCount,4);
		fileConfig.Read(s_aLoginServerIPList,sizeof(s_aLoginServerIPList));
		fileConfig.Close();
	}
#endif

	if	(fileConfig.Open("data/@@lastConnectLoginServer.dat","rb"))
	{
		fileConfig.Read(&cGAME_TITLE::s_iCurrentLoginServer,4);
		fileConfig.Close();
	}

#ifndef	_FOR_CHINA
#ifndef	_DEBUG
	cGAME_TITLE::s_iCurrentLoginServer	=	0;
#endif	
#endif

	if	(!fileConfig.Open("config.cfg","rb"))
	{
		g_iScreenWidth	=	800;
		g_iScreenHeight	=	600;

		g_iCorrectBI_X	=	g_iScreenWidth-800;
		g_iCorrectBI_Y	=	g_iScreenHeight-600;

		g_iFieldScreenHeight	=	g_iScreenHeight-84;

		if	(g_iScreenWidth	==	0)
			g_iScreenWidth	=	800,g_iScreenHeight=	600;

		g_config.m_bf1IsWindowMode	=	FALSE;

		return;
	}

	fileConfig.Read(&g_config,sizeof(g_config));

	fileConfig.Close();

	int	iVersion	=	g_config.m_dwConfigVersion;

	if	(iVersion	<	eCFV_ADD_MINIMAP_DISPLAY_OPTION)
		g_config.m_iMinimapOutputMethod	=	0;

	if	(g_config.m_dwConfigVersion	<	eCFV_ADD_WINDOW_MODE)
		g_config.m_bf1IsWindowMode	=	FALSE;

	if	(iVersion	<	eCFV_ADD_MUSIC_ON)
	{
		g_config.m_bIsMusicOn	=	TRUE;
		g_config.m_bIsSoundOn	=	TRUE;
	}

	if (iVersion	<	eCFV_ADD_OUTPUT_DEVICE)
		g_config.m_iOutputDevice	=	0;

//	데이터 교정
	if (g_config.m_dwStaticMemorySize		<=	0)	g_config.m_dwStaticMemorySize	=	100*1000;
	if (g_config.m_iOutputDevice			> 2 || g_config.m_iOutputDevice				< 0)	g_config.m_iOutputDevice				=	1;
	if (g_config.m_bIsDisplayShotKey		> 1 || g_config.m_bIsDisplayShotKey			< 0)	g_config.m_bIsDisplayShotKey			=	1;

	if (g_config.m_bIsMusicOn				> 1 || g_config.m_bIsMusicOn				< 0)	g_config.m_bIsMusicOn					=	1;
	if (g_config.m_bIsSoundOn				> 1 || g_config.m_bIsSoundOn				< 0)	g_config.m_bIsSoundOn					=	1;
	if (g_config.m_iMusicVolume				< 0 || g_config.m_iMusicVolume				> 1000)	g_config.m_iMusicVolume					=	1000;
	if (g_config.m_iSoundVolume				< 0 || g_config.m_iSoundVolume				> 1000)	g_config.m_iSoundVolume					=	1000;

	if (g_config.m_wIsAllowShout			> 1 || g_config.m_wIsAllowShout				< 0)	g_config.m_wIsAllowShout				=	1;
	if (g_config.m_wIsAllowWhisper			> 1 || g_config.m_wIsAllowWhisper			< 0)	g_config.m_wIsAllowWhisper				=	1;
	if (g_config.m_wIsAllowPartyChat		> 1 || g_config.m_wIsAllowPartyChat			< 0)	g_config.m_wIsAllowPartyChat			=	1;
	if (g_config.m_wIsExclusiveMode			> 1 || g_config.m_wIsExclusiveMode			< 0)	g_config.m_wIsExclusiveMode				=	FALSE;
	if (g_config.m_bIsChatWindowHalfblending> 1 || g_config.m_bIsChatWindowHalfblending	< 0)	g_config.m_bIsChatWindowHalfblending	=	1;
	if (g_config.m_bIsInfoWindowHalfblending> 1 || g_config.m_bIsInfoWindowHalfblending	< 0)	g_config.m_bIsInfoWindowHalfblending	=	1;
	if (g_config.m_wIsTooltipOn				> 1 || g_config.m_wIsTooltipOn				< 0)	g_config.m_wIsTooltipOn					=	1;
	if (g_config.m_iMinimapOutputMethod		> 3 || g_config.m_iMinimapOutputMethod		< 0)	g_config.m_iMinimapOutputMethod			=	0;
	if (g_config.m_wIsViewLargeSkillIcon	> 1											   )	g_config.m_wIsViewLargeSkillIcon		=	1;
	
#ifndef	_FOR_KOREA
	g_config.m_bf1IsUseOldStyleShotKey	=	FALSE;
#endif	

	int	iChattingBoxScale	=	g_config.m_iChattingBoxScale;

	if	(iChattingBoxScale	> 0x8000				)	iChattingBoxScale				-=	0x8000;
	if	(iChattingBoxScale	>= dCHAT_BOX_MAX_SCALE	)	g_config.m_iChattingBoxScale	=	1;

	if	(g_config.m_iSelectAvatar	< 0 || g_config.m_iSelectAvatar >= dMAX_AVATAR_COUNT			)	g_config.m_iSelectAvatar	=	0;
	if	(strlen(g_config.m_strLastId)				>=	sizeof(g_config.m_strLastId)				)	g_config.m_strLastId[0] = 0;
	if	(strlen(g_config.m_strLastSelectServerName)	>=	sizeof(g_config.m_strLastSelectServerName)	)	g_config.m_strLastSelectServerName[0] = 0;

	if	(s_strLastID[0]	==	0)
		strcpy(s_strLastID,g_config.m_strLastId);

	if	(g_config.m_bf1IsRememberId	==	FALSE && g_bIsPortalUser == FALSE)
		strcpy(s_strLastID,"");

	strcpy(s_strLastSelectServerName,g_config.m_strLastSelectServerName);
	s_iSelectAvatar			=	g_config.m_iSelectAvatar;

	{
		if	(g_config.m_bf1IsUse1024X768)
		{
			g_iScreenWidth	=	1024;
		//	g_iScreenWidth	=	1366;
			g_iScreenHeight	=	768;
		}
		else
		{
			g_iScreenWidth	=	800;
			g_iScreenHeight	=	600;
		}

		g_iCorrectBI_X	=	g_iScreenWidth-800;
		g_iCorrectBI_Y	=	g_iScreenHeight-600;

		g_iFieldScreenHeight	=	g_iScreenHeight-84;

		if	(g_iScreenWidth	==	0)
			g_iScreenWidth	=	800,g_iScreenHeight=	600;
	}
}

//
//	
void
CGamePlay::SaveConfigData()
{
	{
		if (g_config.m_bf1IsRememberId)
			strcpy(g_config.m_strLastId,s_strLastID);
		else
			strcpy(g_config.m_strLastId,"");

		strcpy(g_config.m_strLastSelectServerName,s_strLastSelectServerName);

		g_config.m_iSelectAvatar	=	s_iSelectAvatar;
	}

	SetLogFolder();

	cFILE	fileConfig;

	fileConfig.Open("config.cfg","wb");

	g_config.m_dwCheckCode		=	0x12345678;
	g_config.m_dwConfigVersion	=	eCFV_CURRENT_VERSION-1;
	
	fileConfig.Write(&g_config,sizeof(g_config));
	fileConfig.Close();
}