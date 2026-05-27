#include "CGamePlay.H"
#include "CHero.H"
#include "CActor.H"
#include "CSound.H"
#include "CMessage.H"
#include "CItemEffect.H"
#include "CEvent.H"
#include "secretDungeonDefine.H"
#include "CGuild.H"
#include "carrotShop.h"		// insu add .. 당근상점 관련

CPos	CGamePlay::s_posSystemMenu;
int		CGamePlay::s_iFocusedSystemMenu	=	0xffff;
int		l_iMinimapMethodTextPos				=	0;
int		l_iChatFilterTextPos				=	0;

#define	dOPTION_TOP_POS						143
#define	dSYSTEM_MENU_BAR_LINE_GAP			28
#define	dMAX_VOLUME							1000
#define	dMIN_VOLUME							0
//
//	System Menu
enum
{
	eSM_RETURN_TO_VILLAGE				,
	eSM_RETURN_TO_LOGIN_MENU			,
	eSM_RETURN_TO_SELECT_AVATAR_MENU	,
	eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL	,
	eSM_RESSURECTION_BY_RESSURECTION_SCROLL			,
	eSM_EXIT_GAME						,

	eSM_BGM								,
	eSM_SOUND							,

	eSM_PANNEL_HALF_BLENDING			,
	eSM_VIEW_SKILL_RANGE				,
	eSM_SHOW_TOOLTIP					,

	eSM_MINIMAP_NORMAL					,
	eSM_MINIMAP_HALFBLENDING			,
	eSM_MINIMAP_AUTO1					,
	eSM_MINIMAP_AUTO2					,

	eSM_MUSIC_VOLUME_MINUS				,
	eSM_MUSIC_VOLUME_PLUS				,
	eSM_SOUND_VOLUME_MINUS				,
	eSM_SOUND_VOLUME_PLUS				,

	eSM_CHATTING_WINDOW_HB				,
	eSM_DISPLAY_SHOT_KEY				,
	eSM_OLD_STYLE_SHOT_KEY				,

	eSM_CHAT_ALLOW_SAY					,
	eSM_CHAT_ALLOW_SHOUT				,
	eSM_DENY_COMMUNITY					,
	eSM_HIDE_PITCHMAN_SHOP				,

	eSM_VIEW_TIP						,	

	eSM_HIDE_PARTY_ITEM_MESSAGE			,	
	eSM_RETURN_TO_GUILD_HALL			,
	eSM_MINIPET_SOUND_OFF				,
	eSM_SHAKE_OFF						,	// 화면 흔들림 끄기
	eSM_SHOW_DROP_ITEM					,	// 드랍아이템 보기
	eSM_OPEN_CARROT_SHOP				,	// insu add 당근 상점 열기
	eSM_NEW_CARROT_SHOP					,	// insu add 새로운당근상점? 체크박스
	eSM_BODY_EFFECT_OFF					,	// 몸에 붙는 이팩트 끄기
};

//
//시스템 메뉴 열기

void
CGamePlay::PopupSystemMenu()
{
	if (s_iPopupInterface	==	ePIW_SYSTEM)
	{
		ClosePopupWindow();

		return;
	}

	ClosePopupWindow();

	s_iPopupInterface		=	ePIW_SYSTEM;

	int	iWidth				=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_INVENTORY);

	s_posSystemMenu.x		=	g_iScreenWidth-iWidth;
	s_posSystemMenu.y		=	iTitleBarHeight+g_iCorrectBI_Y/2;
	int	iX					=	s_posSystemMenu.x;
	int	iY					=	s_posSystemMenu.y;

	s_rectInterfaceSize.set(g_iScreenWidth-iWidth, g_iCorrectBI_Y/2, g_iScreenWidth, g_iCorrectBI_Y/2+iTitleBarHeight+g_sprInterface.getSpriteHeight(eWINDOW));

	setSystemMenu();

	InitPopupWindowCloseButton();
}

void
CGamePlay::setSystemMenu()
{

	s_iPopupInterface		=	ePIW_SYSTEM;

	int	iWidth				=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_INVENTORY);

	//s_iPopupInterFaceWidth	=	iWidth;	//	인터페이스 넓이
	s_posSystemMenu.x		=	g_iScreenWidth-iWidth;
	s_posSystemMenu.y		=	iTitleBarHeight+g_iCorrectBI_Y/2;
	int	iX					=	s_posSystemMenu.x;
	int	iY					=	s_posSystemMenu.y;

	int iAddX				=	40;
	int iAddXGap			=	70;
#ifdef	_FOR_JAPAN
	iAddX					=	28;
	iAddXGap				=	67;
#endif
	s_rectInterfaceSize.set(g_iScreenWidth-iWidth, g_iCorrectBI_Y/2, g_iScreenWidth, g_iCorrectBI_Y/2+iTitleBarHeight+g_sprInterface.getSpriteHeight(eWINDOW));

	s_bmCategory.reset();
	s_bmCategory.addImageBar(dMSG_SYSTEM_CATEGORY_SCREEN,   eSM_CATEGORY_SCREEN,    iX+iAddX,  iY+dOPTION_TOP_POS+1, eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
	iAddX += iAddXGap;
	s_bmCategory.addImageBar(dMSG_SYSTEM_CATEGORY_SOUND,   eSM_CATEGORY_SOUND,     iX+iAddX, iY+dOPTION_TOP_POS+1, eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
	iAddX += iAddXGap;
	s_bmCategory.addImageBar(dMSG_SYSTEM_CATEGORY_COMMUNITY,   eSM_CATEGORY_COMMUNITY, iX+iAddX, iY+dOPTION_TOP_POS+1, eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
	
	s_bmCategory.check(s_iCategoryIndex,TRUE);


	s_bmBottomMenu.resetChecker();
	s_bmBottomMenu.check(eBIM_MENU_SYSTEM,TRUE);

	s_bmMenu.reset();
	s_bmMenu.addImageBar(dMSG_RETURN_TO_VILLAGE				,eSM_RETURN_TO_VILLAGE				,iX	+	36	,iY	+14								,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
	s_bmMenu.addImageBar(dMSG_RETURN_TO_GUILD_HALL			,eSM_RETURN_TO_GUILD_HALL			,iX	+	36	,iY	+14								,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
	
	s_bmMenu.addImageBar(dMSG_EXIT_TO_WINDOW				,eSM_EXIT_GAME						,iX	+	36	,iY	+14+dSYSTEM_MENU_BAR_LINE_GAP*2	,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
	s_bmMenu.addImageBar(dMSG_RETURN_TO_LOGIN_MENU			,eSM_RETURN_TO_LOGIN_MENU			,iX	+	36	,iY	+14+dSYSTEM_MENU_BAR_LINE_GAP	,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
	s_bmMenu.addImageBar(dMSG_RESURRECTION_BY_PERFECT_RESSURECTION_SCROLL,eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL	,iX	+	36	,iY	+14+dSYSTEM_MENU_BAR_LINE_GAP	,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
	s_bmMenu.addImageBar(dMSG_RESURRECTION_BY_RESSURECTION_SCROLL		,eSM_RESSURECTION_BY_RESSURECTION_SCROLL			,iX	+	36	,iY	+14+dSYSTEM_MENU_BAR_LINE_GAP*2	,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);

	if	(g_lpHero->isDeath())
	{
		s_bmMenu.show(eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL	);
		s_bmMenu.show(eSM_RESSURECTION_BY_RESSURECTION_SCROLL			);
		s_bmMenu.correctPos(eSM_EXIT_GAME,iX+36	,iY	+14+dSYSTEM_MENU_BAR_LINE_GAP*3);
		s_bmMenu.hide(eSM_RETURN_TO_LOGIN_MENU							);
	}
	else
	{
		s_bmMenu.correctPos(eSM_EXIT_GAME,iX+36	,iY	+14+dSYSTEM_MENU_BAR_LINE_GAP*2);
		s_bmMenu.hide(eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL	);
		s_bmMenu.hide(eSM_RESSURECTION_BY_RESSURECTION_SCROLL			);
	}

	if (s_iCategoryIndex == eSM_CATEGORY_SOUND)
	{
		iY+= 36;
		s_bmMenu.addImageBar("",eSM_MUSIC_VOLUME_MINUS	,iX+121 ,iY+dOPTION_TOP_POS+1,eSTATE_DOWN_NORMAL,eSTATE_DOWN_ACTIVE,eSTATE_DOWN_PRESSED,0xffff,eSTATE_DOWN_DISABLE);
		s_bmMenu.addImageBar("",eSM_MUSIC_VOLUME_PLUS	,iX+233 ,iY+dOPTION_TOP_POS+1,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
		s_bmMenu.addImageBar("",eSM_SOUND_VOLUME_MINUS	,iX+121 ,iY+dOPTION_TOP_POS+16+1, eSTATE_DOWN_NORMAL,eSTATE_DOWN_ACTIVE,eSTATE_DOWN_PRESSED,0xffff,eSTATE_DOWN_DISABLE);
		s_bmMenu.addImageBar("",eSM_SOUND_VOLUME_PLUS	,iX+233 ,iY+dOPTION_TOP_POS+16+1, eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);
	}
	

	s_bmMenu.setAllBarTextColor(BLACK,FALSE);
	s_bmMenu.setClickTextAllBarMargin(0,1);
	s_bmMenu.setClickImageMargin(eSM_MUSIC_VOLUME_MINUS	,0,0);
	s_bmMenu.setClickImageMargin(eSM_MUSIC_VOLUME_PLUS	,0,0);
	s_bmMenu.setClickImageMargin(eSM_SOUND_VOLUME_MINUS	,0,0);
	s_bmMenu.setClickImageMargin(eSM_SOUND_VOLUME_PLUS	,0,0);

	s_bmCheckBox.reset();

	if (s_iCategoryIndex == eSM_CATEGORY_SOUND)
	{
		s_bmCheckBox.addImageBar(dMSG_BGM			,eSM_BGM	,iX+36,iY+dOPTION_TOP_POS,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		s_bmCheckBox.addImageBar(dMSG_EFFECT_SOUND	,eSM_SOUND	,iX+36,iY+dOPTION_TOP_POS+16,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);	
	}

	int	iVP			=	iY+dOPTION_TOP_POS+36;
	int	iLineGab	=	16;

	s_bmRadioBox.reset();
	if (s_iCategoryIndex == eSM_CATEGORY_SCREEN)
	{
		s_bmCheckBox.addImageBar(dMSG_PANNEL_HALF_BLENDING	,eSM_PANNEL_HALF_BLENDING	,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_VIEW_SKILL_RANGE		,eSM_VIEW_SKILL_RANGE		,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_TOOLTIP_STATUS		,eSM_SHOW_TOOLTIP			,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_CHATTING_METHOD		,eSM_CHATTING_WINDOW_HB		,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_DISPLAY_SHOT_KEY		,eSM_DISPLAY_SHOT_KEY		,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
#ifdef	_FOR_KOREA
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_OLD_STYLE_SHOT_KEY	,eSM_OLD_STYLE_SHOT_KEY		,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
#endif
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_VIEW_TIP				,eSM_VIEW_TIP				,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		
		s_bmCheckBox.addImageBar(dMSG_SHAKE_OFF,eSM_SHAKE_OFF,iX+36,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_SHOW_DROP_ITEM,eSM_SHOW_DROP_ITEM,iX+36,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);

#ifndef	_FOR_JAPAN
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_USE_NEW_CARROT_SHOP, eSM_NEW_CARROT_SHOP, iX+36, iVP, eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
#endif

		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_BODY_EFFECT_OFF, eSM_BODY_EFFECT_OFF, iX+36, iVP, eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);

		iVP+=35;
		l_iMinimapMethodTextPos	=	iVP-14;		
		// 138
		
		s_bmRadioBox.addImageBar(dMSG_MINIMAP_NORMAL		,eSM_MINIMAP_NORMAL			,iX+36	,iVP,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmRadioBox.addImageBar(dMSG_MINIMAP_HALFBLENDING	,eSM_MINIMAP_HALFBLENDING	,iX+36	,iVP,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmRadioBox.addImageBar(dMSG_MINIMAP_AUTO1			,eSM_MINIMAP_AUTO1			,iX+36	,iVP,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmRadioBox.addImageBar(dMSG_MINIMAP_AUTO2			,eSM_MINIMAP_AUTO2			,iX+36	,iVP,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		/**/

		iVP+=35;
	}
	if (s_iCategoryIndex == eSM_CATEGORY_SOUND)
	{
		s_bmCheckBox.addImageBar(dMSG_MINIPET_SOUND_OFF,eSM_MINIPET_SOUND_OFF,iX+36,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
	}

	if (s_iCategoryIndex == eSM_CATEGORY_COMMUNITY)
	{
		l_iChatFilterTextPos	=	iVP-14;
		s_bmCheckBox.addImageBar(dMSG_SAY					,eSM_CHAT_ALLOW_SAY			,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_SHOUT					,eSM_CHAT_ALLOW_SHOUT		,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_DENY_COMMUNITY		,eSM_DENY_COMMUNITY			,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
		s_bmCheckBox.addImageBar(dMSG_HIDE_PICHMAN_SHOP		,eSM_HIDE_PITCHMAN_SHOP		,iX+36	,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;

		s_bmCheckBox.addImageBar(dMSG_OUTPUT_PARTY_ITEM_MESSAGE,eSM_HIDE_PARTY_ITEM_MESSAGE,iX+36,iVP,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);
		iVP+=iLineGab;
	}

		if (g_config.m_bIsMusicOn				)	s_bmCheckBox.check(eSM_BGM);
		if (g_config.m_bIsSoundOn				)	s_bmCheckBox.check(eSM_SOUND);
		if (g_config.m_bf1IsViewSkillRange		)	s_bmCheckBox.check(eSM_VIEW_SKILL_RANGE);	//	기술 사정거리 표시
		if (g_config.m_wIsTooltipOn				)	s_bmCheckBox.check(eSM_SHOW_TOOLTIP);	//	툴팁 표시 (켜기, 끄기)
		if (g_config.m_bIsInfoWindowHalfblending)	s_bmCheckBox.check(eSM_PANNEL_HALF_BLENDING);//	정보창 표시 방법 (반투명, 불투명)
		if (g_config.m_bIsChatWindowHalfblending)	s_bmCheckBox.check(eSM_CHATTING_WINDOW_HB);
		if (g_config.m_bIsDisplayShotKey		)	s_bmCheckBox.check(eSM_DISPLAY_SHOT_KEY);
		if (g_config.m_bf1IsHideTip	==	FALSE	)	s_bmCheckBox.check(eSM_VIEW_TIP);
		if (g_config.m_bf1IsHidePartyItemMessage==	FALSE	)	s_bmCheckBox.check(eSM_HIDE_PARTY_ITEM_MESSAGE);

	#ifdef	_FOR_KOREA
		if (g_config.m_bf1IsUseOldStyleShotKey	)	s_bmCheckBox.check(eSM_OLD_STYLE_SHOT_KEY);
	#endif

		if (g_config.m_wIsAllowWhisper			)	s_bmCheckBox.check(eSM_CHAT_ALLOW_SAY	);
		if (g_config.m_wIsAllowShout			)	s_bmCheckBox.check(eSM_CHAT_ALLOW_SHOUT);
		if (g_hero.m_bf1IsDenyCommunity			)	s_bmCheckBox.check(eSM_DENY_COMMUNITY);
		if (g_hero.m_bf1IsHidePitchmanShop		)	s_bmCheckBox.check(eSM_HIDE_PITCHMAN_SHOP);

		if (g_config.m_bf1IsMiniPetSoundOff		)	s_bmCheckBox.check(eSM_MINIPET_SOUND_OFF);
		if (g_config.m_bf1IsShakeOff			)	s_bmCheckBox.check(eSM_SHAKE_OFF);			// 화면 흔들림
		if (g_config.m_bf1IsShowDropItem			)	s_bmCheckBox.check(eSM_SHOW_DROP_ITEM);			// 드랍 아이템 보기
		s_bmRadioBox.check(eSM_MINIMAP_NORMAL+g_config.m_iMinimapOutputMethod);

#ifndef	_FOR_JAPAN
		if (g_config.b_bIsNewCarrotShop)		s_bmCheckBox.check(eSM_NEW_CARROT_SHOP);
#endif
		if (g_config.m_bf1IsBodyEffectOff)		s_bmCheckBox.check(eSM_BODY_EFFECT_OFF);

	if	((g_lpHero->isDeath() || s_bIsEventField)	&&	g_hero.m_wCurrentField	!=	433	 &&  g_bIsDuelServer == FALSE)
		s_bmMenu.enable(eSM_RETURN_TO_VILLAGE);
	else
		s_bmMenu.disable(eSM_RETURN_TO_VILLAGE);

	if	(g_lpHero->isDeath()	&&	s_bIsGuildPointBattleField)
	{
		s_bmMenu.show(eSM_RETURN_TO_GUILD_HALL	);
		s_bmMenu.hide(eSM_RETURN_TO_VILLAGE);
	}
	else
		s_bmMenu.hide(eSM_RETURN_TO_GUILD_HALL);
}

//
//창 닫아
void
CGamePlay::CloseSystemMenu()
{
	cINPDEV::ReleaseExclusive();
	s_bmBottomMenu.check(eBIM_MENU_SYSTEM,FALSE);
}

//
//스테이터스 인터페이스 그리기
void
CGamePlay::DrawSystemMenu()
{
	if	(s_iPopupInterface	!=	ePIW_SYSTEM)
		return;

	int		iX,iY;

	iX		=	s_posSystemMenu.x;
	iY		=	s_posSystemMenu.y;

#ifdef	_FOR_ENG
//	iX	-=	50;
//	iY	+=	10;
#endif

	if (g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+266-4,iY+482-8);

	g_sprInterface.Put(iX+8,iY+6	,eSMWI_MENU_FRAME);
#ifdef	_FOR_ENG
	g_sprInterface.Put(iX+8,iY+122	,eSMWI_OPTION_FRAME);
#else
	g_sprInterface.Put(iX+8,iY+136	,eSMWI_OPTION_FRAME);
#endif

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface.Put(iX,g_iCorrectBI_Y/2 ,eWINDOW_TITLE_SYSTEM);

	if (s_iCategoryIndex == eSM_CATEGORY_SOUND)	
	{
		iY += 35;
		int iXRate			=	60;
		int iWidth				= 185 * iXRate/100;
		g_sprInterface.Put(iX+120,iY+dOPTION_TOP_POS,eSMWI_GAUGE,iXRate);
		g_sprInterface.Put(iX+120,iY+dOPTION_TOP_POS+16,eSMWI_GAUGE,iXRate);		// insu add .. 22->16

		int	iMusicVolume	=	max(min(float(g_config.m_iMusicVolume)/dMAX_VOLUME*iWidth,iWidth),0);
		int	iSoundVolume	=	max(min(float(g_config.m_iSoundVolume)/dMAX_VOLUME*iWidth,iWidth),0);
		if	(g_config.m_bIsMusicOn)
			g_sprInterface.putClipedImage(iX+133,iY+dOPTION_TOP_POS,eSMWI_FILLED_GAUGE,iMusicVolume);
		if	(g_config.m_bIsSoundOn)
			g_sprInterface.putClipedImage(iX+133,iY+dOPTION_TOP_POS+16,eSMWI_FILLED_GAUGE,iSoundVolume);		// insu add .. 22->16
	}

	s_bmMenu.draw();
	s_bmCheckBox.draw();
	s_bmRadioBox.draw();
	s_bmCategory.draw();
	cDRAW::Line(RGBmix(132, 123, 123), iX+29, s_posSystemMenu.y+165, iX+259, s_posSystemMenu.y+165);
	cDRAW::Line(RGBmix(132, 123, 123), iX+29, s_posSystemMenu.y+166, iX+259, s_posSystemMenu.y+166);

	if (s_iCategoryIndex == eSM_CATEGORY_SCREEN)		
		s_text.put(iX+36,l_iMinimapMethodTextPos,WHITE,dMSG_MINIMAP_OUTPUT_METHOD);
}



void
CGamePlay::AskExitGame(int _iWhere,BOOL _bIsNow,BOOL _bIsByServer)
{
	if (_bIsNow)
	{
		if	(_bIsByServer	==	FALSE)
		{
			if	(g_hero.m_wCurrentField	>=	dINSTANCE_FIELD_FLAG	||	s_bIsGuildBattleField	||	(g_bIsDuelServer && g_guildBattleInfo.m_bIsBeginBattle))
			{
				if	(g_hero.m_wRestraintTimeByLogout	==	0xffff)
				{
					g_hero.m_wLogoutWork	=	_iWhere;
					s_agent.sendExitGame(_iWhere);
					return;
				}

				switch(_iWhere)
				{
					case	eEGV_RETURN_TO_TITLE			:
						g_msgBox.cPopup("",dMSG_ARE_YOU_WANT_FORCED_DISCONNECT_GAME,dMSG_YES,dMSG_NO);
						g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_FOCED);
						break;

					case	eEGV_EXIT_TO_WINDOW				:
						g_msgBox.cPopup("",dMSG_ARE_YOU_WANT_FORCED_DISCONNECT_GAME,dMSG_YES,dMSG_NO);
						g_msgBox.setButtonId(eMBI_EXIT_TO_WINDOW_BY_FOCED);
						break;
				}

				return;
			}
		}

		switch(_iWhere)
		{
			case	eEGV_RETURN_TO_TITLE			:
				BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
				g_hero.m_GGG.ResetGGGSkill();
				break;

			case	eEGV_EXIT_TO_WINDOW				:
				BookingMode(eGAME_CLOSE,dSTEP_FINISH,dFADE_OUT);
				g_hero.m_GGG.ResetGGGSkill();
				break;
		}

		s_agent.reset();	//	연결 바로 끊자!!
		ClosePopupWindow();
		return;
	}

	if (g_hero.m_wRestraintTimeByAttackAction && !g_lpHero->isDeath())
	{
		char	strMessage[256];

		sprintf(strMessage,dMSG_CAN_NOT_EXIT_GAME_DURING_BATTLE_FORM,g_hero.m_wRestraintTimeByAttackAction);
		CGamePlay::WarningMessage(strMessage);
		return;
	}

	switch(_iWhere)
	{
		case	eEGV_RETURN_TO_TITLE			:
			g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_RETURN_TO_TITLE,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_LOGOUT);
			break;

		case	eEGV_EXIT_TO_WINDOW				:
			g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_EXIT_GAME,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_EXIT_TO_WINDOW);
			break;
	}
}

//
//
void
CGamePlay::AskUsePerfectResurrectionScroll()
{
	int iItemSlot	=	g_hero.findSpecificEffectItem(eIE_PERFECT_RESSURECTION,TRUE);

	if	(iItemSlot	==	0xffff)
		return;

	if	(iItemSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		s_agent.sendUseBeltItem(iItemSlot-dBORDER_OF_ITEM_AND_EQUIPMENT-dEQUIP_BELT_SLOT1,g_lpHero->m_wSerial);
	else
		s_agent.sendUseItemToCharacter(iItemSlot,g_lpHero->m_wSerial);
}

//
//
void
CGamePlay::AskUseResurrectionScroll()
{
	int iItemSlot	=	g_hero.findSpecificEffectItem(eIE_SELF_RESURRECTION,TRUE);

	if	(iItemSlot	==	0xffff)
		return;

	if	(iItemSlot	>=	dBORDER_OF_ITEM_AND_EQUIPMENT)
		s_agent.sendUseBeltItem(iItemSlot-dBORDER_OF_ITEM_AND_EQUIPMENT-dEQUIP_BELT_SLOT1,g_lpHero->m_wSerial);
	else
		s_agent.sendUseItemToCharacter(iItemSlot,g_lpHero->m_wSerial);
}

//
//시스템 메뉴 업데이트

void
CGamePlay::UpdateSystemMenu()
{
	if	(s_iPopupInterface	!=	ePIW_SYSTEM)
		return;

	int		iX,iY;

	iX		=	s_posSystemMenu.x;
	iY		=	s_posSystemMenu.y;

#ifdef	_FOR_ENG
//	iX	-=	50;
//	iY	+=	10;
#endif

	if	(g_lpHero->isDeath())
	{
		if	(g_hero.m_wCurrentField	!=	433 && g_guildBattleInfo.m_bIsBeginBattle == FALSE)
			s_bmMenu.enable(eSM_RETURN_TO_VILLAGE);

		if	(s_bIsGuildPointBattleField	||	s_bIsSiegeWarfareField)
		{
			s_bmMenu.show(eSM_RETURN_TO_GUILD_HALL	);
			s_bmMenu.hide(eSM_RETURN_TO_VILLAGE);
		}
		else
		{
			s_bmMenu.show(eSM_RETURN_TO_VILLAGE);
			s_bmMenu.hide(eSM_RETURN_TO_GUILD_HALL);
		}

		s_bmMenu.show(eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL);
		s_bmMenu.show(eSM_RESSURECTION_BY_RESSURECTION_SCROLL);
		s_bmMenu.correctPos(eSM_EXIT_GAME,iX+36	,iY	+14+dSYSTEM_MENU_BAR_LINE_GAP*3);
		s_bmMenu.hide(eSM_RETURN_TO_LOGIN_MENU);

		if	(g_hero.findSpecificEffectItem(eIE_PERFECT_RESSURECTION,TRUE) != 0xffff)
			s_bmMenu.enable(eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL);
		else
			s_bmMenu.disable(eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL);

		if	(g_hero.findSpecificEffectItem(eIE_SELF_RESURRECTION,TRUE) != 0xffff)
			s_bmMenu.enable(eSM_RESSURECTION_BY_RESSURECTION_SCROLL);
		else
			s_bmMenu.disable(eSM_RESSURECTION_BY_RESSURECTION_SCROLL);
	}
	else
	{
		if	(s_bIsGuildPointBattleField	||	s_bIsSiegeWarfareField)
		{
			s_bmMenu.show(eSM_RETURN_TO_GUILD_HALL	);
			s_bmMenu.hide(eSM_RETURN_TO_VILLAGE);
		}
		else
		{
			s_bmMenu.show(eSM_RETURN_TO_VILLAGE);
			s_bmMenu.hide(eSM_RETURN_TO_GUILD_HALL);

			if	(s_bIsEventField)
				s_bmMenu.enable(eSM_RETURN_TO_VILLAGE);
			else
				s_bmMenu.disable(eSM_RETURN_TO_VILLAGE);
		}

		if	(g_bIsDuelServer &&	g_guildBattleInfo.m_bIsBeginBattle)
			s_bmMenu.disable(eSM_RETURN_TO_LOGIN_MENU);

		s_bmMenu.show(eSM_RETURN_TO_LOGIN_MENU);

		s_bmMenu.hide(eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL);
		s_bmMenu.hide(eSM_RESSURECTION_BY_RESSURECTION_SCROLL);
		s_bmMenu.correctPos(eSM_EXIT_GAME,iX+36	,iY	+14+dSYSTEM_MENU_BAR_LINE_GAP*2);
	}

	if (g_config.m_bIsMusicOn)
	{
		s_bmMenu.enable(eSM_MUSIC_VOLUME_MINUS);
		s_bmMenu.enable(eSM_MUSIC_VOLUME_PLUS	);
	}
	else
	{
		s_bmMenu.disable(eSM_MUSIC_VOLUME_MINUS);
		s_bmMenu.disable(eSM_MUSIC_VOLUME_PLUS	);
	}

	if (g_config.m_bIsSoundOn)
	{
		s_bmMenu.enable(eSM_SOUND_VOLUME_MINUS);
		s_bmMenu.enable(eSM_SOUND_VOLUME_PLUS	);
	}
	else
	{
		s_bmMenu.disable(eSM_SOUND_VOLUME_MINUS);
		s_bmMenu.disable(eSM_SOUND_VOLUME_PLUS	);
	}

	DWORD	dwSelectMenu;
	// 카테고리버튼 처리
	{
		dwSelectMenu	=	s_bmCategory.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);
		switch(dwSelectMenu)
		{
			case eSM_CATEGORY_SCREEN:
				s_iCategoryIndex = eSM_CATEGORY_SCREEN;
				setSystemMenu();
				break;
			case eSM_CATEGORY_SOUND:
				s_iCategoryIndex = eSM_CATEGORY_SOUND;
				setSystemMenu();
				break;
			case eSM_CATEGORY_COMMUNITY:
				s_iCategoryIndex = eSM_CATEGORY_COMMUNITY;
				setSystemMenu();
				break;
		}
	}
//	체크 버튼들 처리
	{
		dwSelectMenu	=	s_bmCheckBox.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

		switch(dwSelectMenu)
		{
			// insu add
			case	eSM_NEW_CARROT_SHOP			:
				g_config.b_bIsNewCarrotShop				= 1-g_config.b_bIsNewCarrotShop;
				break;
			// insu add end
			case	eSM_BGM						:
				g_config.m_bIsMusicOn					=	1-g_config.m_bIsMusicOn;
				PlayBgm();
				break;

			case	eSM_SOUND					:
				g_config.m_bIsSoundOn					=	1-g_config.m_bIsSoundOn;
				g_esm.setActive(g_config.m_bIsSoundOn);
				break;

			case	eSM_PANNEL_HALF_BLENDING	:
				g_config.m_bIsInfoWindowHalfblending	=	1-g_config.m_bIsInfoWindowHalfblending;
				break;

			case	eSM_VIEW_SKILL_RANGE		:
				g_config.m_bf1IsViewSkillRange			=	1-g_config.m_bf1IsViewSkillRange;
				break;

			case	eSM_SHOW_TOOLTIP			:
				g_config.m_wIsTooltipOn					=	1-g_config.m_wIsTooltipOn;
				s_ttCommon.setEnable(g_config.m_wIsTooltipOn);
				break;

			case	eSM_CHATTING_WINDOW_HB		:
				g_config.m_bIsChatWindowHalfblending	=	1-g_config.m_bIsChatWindowHalfblending;
				break;

			case	eSM_VIEW_TIP				:
				g_config.m_bf1IsHideTip					=	1-g_config.m_bf1IsHideTip;
				break;

			case	eSM_HIDE_PARTY_ITEM_MESSAGE	:
				g_config.m_bf1IsHidePartyItemMessage	=	1-g_config.m_bf1IsHidePartyItemMessage;
				RebuildChatMessageBox();
				break;

			case	eSM_DISPLAY_SHOT_KEY		:
				g_config.m_bIsDisplayShotKey			=	1-g_config.m_bIsDisplayShotKey;
				break;

#ifdef	_FOR_KOREA
			case	eSM_OLD_STYLE_SHOT_KEY		:
				g_config.m_bf1IsUseOldStyleShotKey		=	1-g_config.m_bf1IsUseOldStyleShotKey;
				break;
#endif
			case	eSM_CHAT_ALLOW_SAY			:
				g_config.m_wIsAllowWhisper				=	1-g_config.m_wIsAllowWhisper;
				RebuildChatMessageBox();
				break;

			case	eSM_CHAT_ALLOW_SHOUT		:
				g_config.m_wIsAllowShout				=	1-g_config.m_wIsAllowShout;
				break;

			case	eSM_DENY_COMMUNITY			:
				g_hero.m_bf1IsDenyCommunity				=	1-g_hero.m_bf1IsDenyCommunity;
				s_agent.sendChangCommunityStatus(g_hero.m_bf1IsDenyCommunity,g_hero.m_bf1IsHidePitchmanShop);
				break;

			case	eSM_HIDE_PITCHMAN_SHOP		:
				g_hero.m_bf1IsHidePitchmanShop			=	1-g_hero.m_bf1IsHidePitchmanShop;
				s_agent.sendChangCommunityStatus(g_hero.m_bf1IsDenyCommunity,g_hero.m_bf1IsHidePitchmanShop);
				break;
			case	eSM_MINIPET_SOUND_OFF		:
				g_config.m_bf1IsMiniPetSoundOff		=	1-g_config.m_bf1IsMiniPetSoundOff;
				break;
			case	eSM_SHAKE_OFF		:
				g_config.m_bf1IsShakeOff		=	1-g_config.m_bf1IsShakeOff;
				break;
			case	eSM_SHOW_DROP_ITEM		:
				g_config.m_bf1IsShowDropItem		=	1-g_config.m_bf1IsShowDropItem;
				break;
			case	eSM_BODY_EFFECT_OFF		:
				g_config.m_bf1IsBodyEffectOff		=	1-g_config.m_bf1IsBodyEffectOff;
				break;
/*
			case	eSM_CHAT_ALLOW_PARTY		:
				g_config.m_wIsAllowPartyChat			=	1-g_config.m_wIsAllowPartyChat;
				RebuildChatMessageBox();
				break;
*/
		}
	}

//	라디오 버튼 처리
	{
		dwSelectMenu	=	s_bmRadioBox.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

		switch(dwSelectMenu)
		{
			case	eSM_MINIMAP_NORMAL			:
				g_config.m_iMinimapOutputMethod	=	0;
				break;
			case	eSM_MINIMAP_HALFBLENDING	:
				g_config.m_iMinimapOutputMethod	=	1;
				break;
			case	eSM_MINIMAP_AUTO1			:
				g_config.m_iMinimapOutputMethod	=	2;
				break;
			case	eSM_MINIMAP_AUTO2			:
				g_config.m_iMinimapOutputMethod	=	3;
				break;
		}
	}

//	시스템 메뉴
	dwSelectMenu	=	s_bmMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

	switch(dwSelectMenu)
	{
		case	eSM_RETURN_TO_VILLAGE							:
		{
			if	(s_bIsEventField	&& g_eventOxQuiz.m_bIsEndEvent == FALSE)
			{
				g_msgBox.cPopup("",dMSG_DO_YOU_REALLY_RETURN_TO_THE_VILLAGE_STOP_THE_EVENT,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_OX_QUIZ_EVENT);
			}
			else	
				s_agent.sendReturnToVillage(eRTV_BY_REVIVE);
			break;
		}

		case	eSM_RETURN_TO_GUILD_HALL						:
		{
			g_msgBox.cPopup("",dMSG_DO_YOU_WANT_RETURN_TO_GUILD_HALL_END_GUILD_POINT_BATTLE,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_RETURN_TO_THE_GUILD_HALL_BY_END_GUILD_POINT_BATTLE);
			break;
		}

		case	eSM_RETURN_TO_LOGIN_MENU						:
		{
			AskExitGame(eEGV_RETURN_TO_TITLE);
			break;
		}

		case	eSM_RESSURECTION_BY_PERFECT_RESSURECTION_SCROLL	:
			AskUsePerfectResurrectionScroll();
			break;

		case	eSM_RESSURECTION_BY_RESSURECTION_SCROLL			:
			AskUseResurrectionScroll();
			break;

		case	eSM_EXIT_GAME									:
			AskExitGame(eEGV_EXIT_TO_WINDOW);
			break;
	}

//	볼륨들
	dwSelectMenu	=	s_bmMenu.updatePressed(s_posMouse.x,s_posMouse.y,s_aTouched,s_isLeftButtonPressed);

	switch(dwSelectMenu)
	{
		case	eSM_MUSIC_VOLUME_PLUS		:
			g_config.m_iMusicVolume	+=	5;
			g_config.m_iMusicVolume	=	min(g_config.m_iMusicVolume,dMAX_VOLUME);
			s_oggPlayer.setVolume(g_config.m_iMusicVolume*10);
			break;

		case	eSM_MUSIC_VOLUME_MINUS		:
			g_config.m_iMusicVolume	-=	5;
			g_config.m_iMusicVolume	=	max(g_config.m_iMusicVolume,dMIN_VOLUME);
			s_oggPlayer.setVolume(g_config.m_iMusicVolume*10);
			break;

		case	eSM_SOUND_VOLUME_MINUS		:
			g_config.m_iSoundVolume	-=	5;
			g_config.m_iSoundVolume	=	max(g_config.m_iSoundVolume,dMIN_VOLUME);
			s_soundPlayer.setDefaultVolume(g_config.m_iSoundVolume*10);
			break;

		case	eSM_SOUND_VOLUME_PLUS		:
			g_config.m_iSoundVolume	+=	5;
			g_config.m_iSoundVolume	=	min(g_config.m_iSoundVolume,dMAX_VOLUME);
			s_soundPlayer.setDefaultVolume(g_config.m_iSoundVolume*10);
			break;
	}

//	볼륨 조절
	if (s_bmMenu.isPressed() || s_bmCheckBox.isPressed() || s_bmRadioBox.isPressed())	return;

	if (s_iCategoryIndex == eSM_CATEGORY_SOUND && s_isLeftButtonPressed)
	{
		int	iHeight			=	g_sprInterface.getSpriteHeight(eSMWI_FILLED_GAUGE);
		int	iButtonWidth	=	g_sprInterface.getSpriteWidth(eSTATE_UP_NORMAL);

		int iXRate			=	60;
		int	iWidth			=	185 * iXRate/100;
		cRECT	rect;
		iY += 36;
		rect.set(iX+133,iY+dOPTION_TOP_POS-1,iX+133+iWidth,iY+dOPTION_TOP_POS+iHeight-1);

		if (g_config.m_bIsMusicOn)
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				int	iPos	=	max(s_posMouse.x-(iX+133),0);
				iPos		=	min(iPos,iWidth);
				iPos		=	iPos*dMAX_VOLUME/iWidth;

				g_config.m_iMusicVolume	=	max(iPos,dMIN_VOLUME);
				g_config.m_iMusicVolume	=	min(iPos,dMAX_VOLUME);

				s_oggPlayer.setVolume(g_config.m_iMusicVolume*10);
			}

		rect.set(iX+133,iY+dOPTION_TOP_POS-1+22,iX+133+iWidth,iY+dOPTION_TOP_POS-1+22+iHeight);

		if (g_config.m_bIsSoundOn)
			if (rect.isIn(s_posMouse.x,s_posMouse.y))
			{
				int	iPos	=	max(s_posMouse.x-(iX+133),0);
				iPos		=	min(iPos,iWidth);
				iPos		=	iPos*dMAX_VOLUME/iWidth;

				g_config.m_iSoundVolume	=	max(iPos,dMIN_VOLUME);
				g_config.m_iSoundVolume	=	min(iPos,dMAX_VOLUME);

				s_soundPlayer.setDefaultVolume(g_config.m_iSoundVolume*10);
			}
	}
}