#include "CGamePlay.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CWindowInterface.H"
#include "CMessage.H"
#include "CGuildSkill.H"
#include "cHERO.h"

CGuildWindow	g_gwGuild;

#define			dVISIBLE_GUILD_SKILL_COUNT_IN_GUILD_SKILL_WINDOW	22
#define			dMAX_MEMBER_COUNT_IN_GUILD_WINDOW					22
#define			dGUILD_MEMBER_NAME_LENGTH							87
//
//	Guild Window Tab Button
enum
{
	eGWTB_GUILD_INFO,
	eGWTB_GUILD_SKILL,
	eGWTB_GUILD_ITEM,
	eGWTB_MEMBER_LIST,
	eGWTB_TOOL,
	eGWB_ASK_JOIN_GUILD,
	eGWB_CHANGE_NOTICE,
	eGWB_CHANGE_GUILD_EXP_TAX_RATE,
	eGWB_LEAVE_GUILD,
	eGWB_DISSOULTION_GUILD,
	eGWB_CHNAGE_GUILD_HALL_OPEN_STATUS,

	eGWB_SUMMON_GUILD_GUARDIAN_SHIELD_CRAFT	,
	eGWB_SUMMON_GUILD_GUARDIAN_ELEMENTAL_POST	,
	eGWB_SUMMON_GUILD_GUARDIAN_ETHER_SHELL		,
	eGWB_SUMMON_GUILD_GUARDIAN_HOLLOW_KNIGHT	,

	eGWB_UNSUMMON_NEAREST_GUILD_GUARDIAN,
	eGWB_ACTIVE_GUILD_MASTER_GRACE,

	eGWB_TRANS_AVATAR_TO_GVG_SERVER	,
	eGWB_TRANS_GUILD_TO_GVG_SERVER	,
};

enum
{
	eGUILD_PET_ICON_OBITIAN	=		148,
	eGUILD_PET_ICON_DASH_BLADE	,
	eGUILD_PET_ICON_ASTRO_BOW	,
	eGUILD_PET_ICON_CRISTAL_WATER,
	eGUILD_RANK_ICON_ROOKIE			=	189,
	eGUILD_RANK_ICON_MEMBER	,
	eGUILD_RANK_ICON_PARLIAMENT	,
	eGUILD_RANK_ICON_SUBMASTER	,
	eGUILD_RANK_ICON_ELDER	,
	eGUILD_RANK_ICON_MASTER	,
};


void
CGuildWindow::reset()
{
	m_iSelectTab	=	eGWTB_GUILD_INFO;
}

//
//열기
void
CGuildWindow::open()
{
	if (s_iPopupInterface	==	ePIW_GUILD)
	{
		ClosePopupWindow();
		return;
	}

	if	(g_hero.m_bf1IsReadyToMakeGuild)
	{
		if	(g_iWorldServerType	==	eSERVER_TYPE_GVG)
		{
			char	strGuildName[dGUILD_NAME_LENGTH];
			memcpy(strGuildName,g_hero.m_aWayPoint,dGUILD_NAME_LENGTH);
			g_msgBox.cPopup(NULL,_ms(dMSG_ARE_YOU_MAKE_GVG_SERVER_GUILD,strGuildName),dMSG_YES,dMSG_NO);
		}
		else
			g_msgBox.cPopup(NULL,dMSG_READY_TO_MAKE_GUILD_ARE_YOU_MAKE_GUILD,dMSG_YES,dMSG_NO);

		g_msgBox.setButtonId(eMBI_MAKE_GUILD);
		s_bmBottomMenu.check(eBIM_MENU_GUILD,FALSE);

		return;
	}

	if	(g_iWorldServerType	==	eSERVER_TYPE_GVG && g_hero.m_wGuildSerial == 0xffff)
	{
		char	strGuildName[dGUILD_NAME_LENGTH];

		memcpy(strGuildName,g_hero.m_aWayPoint,dGUILD_NAME_LENGTH);

		if	(strGuildName[0]	!=	-1	&&	strGuildName[1]	!=	-1	)
		{
			s_agent.sendRequestJoinGuild(0xffff);

			return;
		}
		g_msgBox.cPopup(NULL,dMSG_PM_YOU_ARE_NOT_BELONG_TO_GUILD,dMSG_OK);
		return;
	}

	if	(g_hero.m_bf1IsReadyToSelectGuildMark)
	{
		g_msgBox.cPopup(NULL,dMSG_ARE_YOU_RETRY_SELECT_GUILD_MARK,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_SELECT_GUILD_MARK);
		s_bmBottomMenu.check(eBIM_MENU_GUILD,FALSE);

		return;
	}

	s_propose.m_work.isReadyAskJoinGuild	=	FALSE;
	m_iGuildSkillWindowScrollBarPos			=	0;
	m_iGuildMemberListWindowScrollBarPos	=	0;

	ClosePopupWindow();

	if	(g_guild.m_wSerial	==	0xffff)
		m_iSelectTab	=	eGWTB_GUILD_INFO;

	if	(m_iSelectTab	!=	eGWTB_GUILD_INFO)
		s_iPopupInterface		=	ePIW_GUILD;

	changeTab(m_iSelectTab,FALSE);

	s_iPopupInterface		=	ePIW_GUILD;
	s_bmBottomMenu.check(eBIM_MENU_GUILD,TRUE);

	int	iWindowWidth		=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight		=	g_sprInterface.getSpriteHeight(eWINDOW);
	int	iTitleBarHeight		=	g_sprInterface.getSpriteHeight(eWINDOW_TITLE_FRIEND);
	int	iBarHeight			=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
	int	iBarWidth			=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);

	s_rectInterfaceSize.set(g_iScreenWidth-iWindowWidth, g_iCorrectBI_Y/2, g_iScreenWidth, g_iCorrectBI_Y/2+iWindowHeight+iTitleBarHeight);

	m_pos.x					=	g_iScreenWidth-iWindowWidth;
	m_pos.y					=	iTitleBarHeight+g_iCorrectBI_Y/2;

	int	iX					=	m_pos.x;
	int	iY					=	m_pos.y+80;

//	탭 버튼 초기화
	{
		if (m_bmTab.m_lpSprite	==	NULL)	
			m_bmTab.init(&g_sprInterface,10,dBAR_RADIO);

		int	iTabWidth	=	g_sprInterface.getSpriteWidth(eGW_TAB_ACTIVE);
		int	iIconSize	=	g_sprInterface.getSpriteWidth(eGW_INFO_ACTIVE);

		m_bmTab.reset();

		m_bmTab.addImageBar("",eGWTB_GUILD_INFO	,m_pos.x+8				,m_pos.y+3,eGW_TAB_NORMAL,0xffff,0xffff,eGW_TAB_ACTIVE,eGW_TAB_NORMAL,dMSG_GUILD_INFO);
		m_bmTab.addImageBar("",eGWTB_GUILD_SKILL,m_pos.x+8+iTabWidth	,m_pos.y+3,eGW_TAB_NORMAL,0xffff,0xffff,eGW_TAB_ACTIVE,eGW_TAB_NORMAL,dMSG_GUILD_SKILL);
		m_bmTab.addImageBar("",eGWTB_GUILD_ITEM	,m_pos.x+8+iTabWidth*2	,m_pos.y+3,eGW_TAB_NORMAL,0xffff,0xffff,eGW_TAB_ACTIVE,eGW_TAB_NORMAL,dMSG_GUILD_STATUE);
		m_bmTab.addImageBar("",eGWTB_MEMBER_LIST,m_pos.x+8+iTabWidth*3	,m_pos.y+3,eGW_TAB_NORMAL,0xffff,0xffff,eGW_TAB_ACTIVE,eGW_TAB_NORMAL,dMSG_MEMBER_LIST);
		m_bmTab.addImageBar("",eGWTB_TOOL		,m_pos.x+8+iTabWidth*4	,m_pos.y+3,eGW_TAB_NORMAL,0xffff,0xffff,eGW_TAB_ACTIVE,eGW_TAB_NORMAL,dMSG_MANAGE_TOOL);

		m_bmTab.check(eGWTB_GUILD_INFO+m_iSelectTab);
		m_bmTab.setClickTextAllBarMargin(1,1);
	}

	iX	=	m_pos.x+(iWindowWidth-6)/2-iBarWidth/2;
	iY	=	m_pos.y+iWindowHeight-9-iBarHeight;

	s_bmButton.reset();

	if	(g_iWorldServerType	!=	eSERVER_TYPE_GVG)
		s_bmButton.addImageBar(dMSG_ASK_GUILD_JOIN,eGWB_ASK_JOIN_GUILD,iX,iY,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,eBUTTON_DISABLE);

	s_bmButton.setClicktAllBarMargin(1,1);
	s_bmButton.setAllBarTextColor(0,FALSE);
	InitPopupWindowCloseButton();

	if	(g_hero.m_wGuildSerial	!=	0xffff)
	{
		g_guild.resetGBS();
		s_agent.sendEtcWork(eCEW_ASK_GUILD_BATTLE_SCHEDULE);

		if	(g_hero.m_bf1IsWaitGuildInfo	==	TRUE)
		{
			g_hero.m_bf1IsWaitGuildInfo		=	FALSE;
			s_agent.sendAskGuildInfo(TRUE);
		}
		else
			s_agent.sendGuildCheckSum(g_guild.getCheckSum());
	}
	else
		g_hero.m_bf1IsWaitGuildInfo		=	FALSE;
}

void
CGuildWindow::changeTab(int _iTab,BOOL _bIsCheckSameWork)
{
	if	(s_iPopupInterface	!=	ePIW_GUILD)
		return;

	if	(_iTab	==	-1)
	{
		_iTab				=	m_iSelectTab;
		_bIsCheckSameWork	=	FALSE;
	}

	if	(_iTab	>	eGWTB_TOOL	)
		return;

	if (g_hero.m_wGuildSerial	==	0xffff	)
	{
		m_iSelectTab	=	eGWTB_GUILD_INFO;

		return;
	}

	if (_bIsCheckSameWork && _iTab == m_iSelectTab)
		return;


	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	
	s_bmRadioBox.reset();
	s_bmMenu.reset();

	if	(_iTab	==	eGWTB_GUILD_SKILL)
	{
		s_sbPopupWindow.set(m_pos.x+iWindowWidth-16,m_pos.y+34,iWindowHeight-61);
		s_sbPopupWindow.setSize(g_gsManager.m_iNormalSkillCount,dVISIBLE_GUILD_SKILL_COUNT_IN_GUILD_SKILL_WINDOW);
		m_iGuildSkillWindowScrollBarPos	=	0;
		s_sbPopupWindow.setPos(m_iGuildSkillWindowScrollBarPos	);

		m_iSelectTab	=	_iTab;
		resetSkillPlusButtonsPos();
	}

	if	(_iTab	==	eGWTB_GUILD_ITEM)
	{
		s_sbPopupWindow.set(m_pos.x+iWindowWidth-16,m_pos.y+34,iWindowHeight-61);
		s_sbPopupWindow.setSize(g_gsManager.m_iStatueSkillCount,dVISIBLE_GUILD_SKILL_COUNT_IN_GUILD_SKILL_WINDOW);
		m_iGuildSkillWindowScrollBarPos	=	0;
		s_sbPopupWindow.setPos(m_iGuildSkillWindowScrollBarPos);

		m_iSelectTab	=	_iTab;
		resetSkillPlusButtonsPos();
	}

	if	(_iTab	==	eGWTB_TOOL)
	{
		int	iX	=	m_pos.x+30;
		int	iY	=	m_pos.y+46;

		s_bmMenu.addImageBar(dMSG_CHANGE_GUILD_NOTICE		,eGWB_CHANGE_NOTICE				,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
		iY	+=	28;

		s_bmMenu.addImageBar(dMSG_CHANGE_GUILD_EXP_TAX_RATE	,eGWB_CHANGE_GUILD_EXP_TAX_RATE	,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
		iY	+=	28;

		if	(g_hero.isGuildMaster() && g_iSeasonVariable >= dSEASON_VARIABLE_GUILD_REFORM)
		{
			s_bmMenu.addImageBar(dMSG_GUILD_MASTER_GRACE_GUILD_MENU	,eGWB_ACTIVE_GUILD_MASTER_GRACE			,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
			iY	+=	28;
		}

		if	(g_iWorldServerType	!=	eSERVER_TYPE_GVG)
		{
			s_bmMenu.addImageBar(dMSG_LEAVE_GUILD				,eGWB_LEAVE_GUILD				,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
			iY	+=	28;

			s_bmMenu.addImageBar(dMSG_DISSOLUTION_GUILD_MENU	,eGWB_DISSOULTION_GUILD			,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
			iY	+=	28;

			if	(g_guild.m_wHallLevel	>=	2)
			{
				s_bmMenu.addImageBar(dMSG_CHANGE_OPEN_GUILD_HALL_STATUS	,eGWB_CHNAGE_GUILD_HALL_OPEN_STATUS,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
				iY	+=	28;
			}
		}

		if	(s_bIsSiegeWarfareField	&&	g_hero.isGuildMaster()	&&	g_hero.m_wTeam	==	0)
		{
			iY	+=	15;

			for	(int i=0;i<dGUILD_GUARDIAN_TYPE_COUNT;i++)
			{
				s_bmMenu.addImageBar(_ms(dMSG_SUMMON_GUILD_GUARDIAN_MENU_FORM,g_lpstrGuildGuardianName[eGUILD_GUARDIAN_SHIELD_CRAFT+i]),
									eGWB_SUMMON_GUILD_GUARDIAN_SHIELD_CRAFT+i	,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
				iY	+=	26;
			}

			s_bmMenu.addImageBar(dMSG_UNSUMMON_GUILD_GUARDIAN,eGWB_UNSUMMON_NEAREST_GUILD_GUARDIAN
								,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
		}

		if	(g_iWorldServerType	!=	eSERVER_TYPE_GVG)
		{
			if	(g_hero.isGuildMaster())
			{
				s_bmMenu.addImageBar(dMSG_TRANS_GUILD_TO_GVG_SERVER	,eGWB_TRANS_GUILD_TO_GVG_SERVER,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
				iY	+=	28;
			}

			{
				s_bmMenu.addImageBar(dMSG_TRANS_AVATAR_TO_GVG_SERVER	,eGWB_TRANS_AVATAR_TO_GVG_SERVER,iX,iY,eSMWI_BAR_NORMAL,eSMWI_BAR_ACTIVE,eSMWI_BAR_PRESSED,0xffff,eSMWI_BAR_DISABLE);
				iY	+=	28;
			}

		}

		s_bmMenu.setAllBarTextColor(BLACK,FALSE);
		s_bmMenu.setClickTextAllBarMargin(0,1);
	}

	if	(_iTab	==	eGWTB_MEMBER_LIST)
	{
		int	iX	=	m_pos.x+62;
		int	iY	=	m_pos.y+38;

		s_bmRadioBox.addImageBar(dMSG_RANK_IN_GUILD	,eGMLSM_BY_RANK	,iX		,iY,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		s_bmRadioBox.addImageBar(dMSG_NAME			,eGMLSM_BY_NAME	,iX+=50	,iY,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		s_bmRadioBox.addImageBar(dMSG_LEVEL			,eGMLSM_BY_LEVEL,iX+=50	,iY,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		s_bmRadioBox.addImageBar(dMSG_JOB			,eGMLSM_BY_JOB	,iX+=50	,iY,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX,eSMWI_RADIO_BOX_CHECKED);
		s_bmRadioBox.check(g_guild.m_wMemberSortMethod);

		if (g_guild.m_wIsNotReceivedMemberList && g_hero.m_bf1IsWaitGuildInfo == FALSE)
		{
			g_hero.m_bf1IsWaitGuildInfo	=	TRUE;

			s_agent.sendAskGuildMemberList();
		}


		s_sbPopupWindow.set(m_pos.x+iWindowWidth-16,m_pos.y+54,iWindowHeight-61);
		s_sbPopupWindow.setSize(g_guild.m_memberList.m_wCount,dMAX_MEMBER_COUNT_IN_GUILD_WINDOW);
		s_sbPopupWindow.setPos(m_iGuildMemberListWindowScrollBarPos);
	}

	m_iSelectTab	=	_iTab;
}

//
//창 닫기
void
CGuildWindow::close()
{
	cINPDEV::ReleaseExclusive();

	s_bmBottomMenu.check(eBIM_MENU_GUILD,FALSE);

	s_bmButton.reset();
	s_propose.m_work.isReadyAskJoinGuild	=	FALSE;
}

//
//	스킬 윈도우의 버튼들을 초기화
void
CGuildWindow::resetSkillPlusButtons()
{
	s_bmPlusState.reset();

	if	(!g_hero.isGuildMaster())
		return;

	int	i,iCount=0;

	for (i=0;iCount < g_gsManager.m_iSkillCount && i < dMAX_GUILD_SKILL_COUNT;i++)
	{
		CGuildSkill		*lpGuildSkill	=	&g_gsManager.m_aSkills[i];

		if	(lpGuildSkill->m_wSerial	==	0xffff)
			continue;

		if	(m_iSelectTab	==	eGWTB_GUILD_SKILL	&&	lpGuildSkill->m_bf1IsStatueSkill	==	TRUE)
			continue;

		if	(m_iSelectTab	==	eGWTB_GUILD_ITEM	&&	lpGuildSkill->m_bf1IsStatueSkill	==	FALSE)
			continue;

		if	(iCount	<	m_iGuildSkillWindowScrollBarPos)
		{
			iCount++;
			continue;
		}

		s_bmPlusState.addImageBar("",i,-100,0,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED,0xffff,eSTATE_UP_DISABLE);

		iCount++;

		if	(iCount-m_iGuildSkillWindowScrollBarPos>=	dVISIBLE_GUILD_SKILL_COUNT_IN_GUILD_SKILL_WINDOW)
			break;
	}
}

//
//	길드 윈도우의 버튼들 위치 설정
void
CGuildWindow::resetSkillPlusButtonsPos()
{
	if	(s_iPopupInterface	!=	ePIW_GUILD)
		return;

	int	i,iX,iY,iCount= 0 ;

	iX		=	m_pos.x+33;
	iY		=	m_pos.y+37;

	resetSkillPlusButtons();

	if	(!g_hero.isGuildMaster())
		return;

	for (i=0;iCount < g_gsManager.m_iSkillCount && i < dMAX_GUILD_SKILL_COUNT;i++)
	{
		CGuildSkill		*lpGuildSkill	=	&g_gsManager.m_aSkills[i];

		if	(lpGuildSkill->m_wSerial	==	0xffff)
			continue;

		if	(m_iSelectTab	==	eGWTB_GUILD_SKILL	&&	lpGuildSkill->m_bf1IsStatueSkill	==	TRUE)
			continue;

		if	(m_iSelectTab	==	eGWTB_GUILD_ITEM	&&	lpGuildSkill->m_bf1IsStatueSkill	==	FALSE)
			continue;

		if	(iCount < m_iGuildSkillWindowScrollBarPos)
		{
			iCount++;
			continue;
		}

		int		iPutX	=	iX,iSkillNameWidth;
		int		iPutY	=	iY+(iCount-m_iGuildSkillWindowScrollBarPos)*19;

		if	(lpGuildSkill->m_wTopLevel && g_guild.m_wGuildPoint && g_guild.isUpgradeAbleSkill(i))
		{
			if	(g_guild.m_abGuildSkill[i] && g_guild.m_abGuildSkill[i] <= dMAX_GUILD_SKILL_LEVEL)
				iSkillNameWidth	=	s_text.getPixelSize(_ms("%s(Lv %d)",lpGuildSkill->m_strName,g_guild.m_abGuildSkill[i]));
			else
				iSkillNameWidth	=	s_text.getPixelSize(lpGuildSkill->m_strName);

			s_bmPlusState.resetImageBar(i,"",i,iPutX+iSkillNameWidth,iPutY,eSTATE_UP_NORMAL,eSTATE_UP_ACTIVE,eSTATE_UP_PRESSED);

			if	(!s_bmPlusState.isShow(i))	
				s_bmPlusState.show(i);
		}

		iCount++;

		if	(iCount-m_iGuildSkillWindowScrollBarPos	>=	dVISIBLE_GUILD_SKILL_COUNT_IN_GUILD_SKILL_WINDOW)
			break;
	}
}

//
//	길드 정보 출력
void
CGuildWindow::drawGuildInfo()
{
	int		i;
	int		iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int		iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);
	int		iX,iY;
	BOOL	bWarninigForLevelDown	=	FALSE;

	iX		=	m_pos.x+34;
	iY		=	m_pos.y+33;

	int	iContentsWidth	=	70;
	int	iRightFitWidth	=	64;

	g_sprInterface.Put(m_pos.x+8,iY	,eGW_FRAME);

	iY		+=	6;

	CTimeInfo	curTime;

	curTime.update();

	if	(curTime.m_wWeekDay	==	c_iWarningForDownGuildPetGuardianLevelWeekDay1	||	
		curTime.m_wWeekDay	==	c_iWarningForDownGuildPetGuardianLevelWeekDay2)
	{
		for	(int iPet=eGUILD_PET_OBITIAN;iPet<=eGUILD_PET_CRISTAL_WATER;iPet++)
		{
			CGuildPetInfo	*lpPet	=	g_guild.getPet(iPet);

			int	iPetLevel					=	g_guild.getPetLevel(iPet);
			int	iPetOldExp					=	max(lpPet->m_dwExperience-lpPet->m_wExpDuringWeek,0);
			int	iOldLevel					=	g_guild.getPetLevelByExp(iPetOldExp);
			int	iRequireExpForUpkeepLevel	=	iOldLevel*c_iDownGuildPetGuardianLevelFactor;
			int	iExpDuringWeek				=	lpPet->m_wExpDuringWeek;

			if	(iOldLevel	<	c_iMinLevelForCheckUpkeepGuildPetAndGuardianLevel)
				iRequireExpForUpkeepLevel	=	0;

			if	(iExpDuringWeek	<	iRequireExpForUpkeepLevel)
				bWarninigForLevelDown		=	TRUE;
		}

		for	(int iGuardian=eGUILD_GUARDIAN_SHIELD_CRAFT;iGuardian<=eGUILD_GUARDIAN_HOLLOW_KNIGHT;iGuardian++)
		{
			CGuildPetInfo	*lpGuardian		=	g_guild.getGuardian(iGuardian);

			int	iGuardianLevel				=	g_guild.getGuardianLevel(iGuardian);
			int	iGuardianOldExp				=	max(lpGuardian->m_dwExperience-lpGuardian->m_wExpDuringWeek,0);
			int	iOldLevel					=	g_guild.getPetLevelByExp(iGuardianOldExp);
			int	iRequireExpForUpkeepLevel	=	iOldLevel*c_iDownGuildPetGuardianLevelFactor;
			int	iExpDuringWeek				=	lpGuardian->m_wExpDuringWeek;

			if	(iOldLevel	<	c_iMinLevelForCheckUpkeepGuildPetAndGuardianLevel)
				iRequireExpForUpkeepLevel	=	0;

			if	(iExpDuringWeek	<	iRequireExpForUpkeepLevel)
				bWarninigForLevelDown		=	TRUE;
		}

		if	(bWarninigForLevelDown)
		{
			cRECT	rect;

			rect.set(m_pos.x-200,456,m_pos.x-10,490);

			int	iLineCount	=	s_text.getLineCount(rect.getWidth(),dMSG_WARNING_FOR_LEVEL_DOWN_GUILD_PET_AND_GUARDIAN);
			int	iTextHeight	=	iLineCount*s_text.getLineThick()-s_text.m_iLineGap;

			rect.y1			=	rect.y2-iTextHeight;

			s_ttCommon.drawBox(&rect,0,16,TRUE,TRUE);
			s_text.put(rect.x1,rect.y1,rect.getWidth(),iTextHeight,0,WHITE,dMSG_WARNING_FOR_LEVEL_DOWN_GUILD_PET_AND_GUARDIAN);
		}
	}
	

//	기본 정보
	{
#ifdef	_FOR_ENG
		iY		+=	4;
#endif
		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_NAME,g_guild.m_strName);
		s_text.put(iX+iContentsWidth,iY,LTGREEN,g_guild.m_strName);
		iY		+=	14;
		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_MASTER,g_guild.m_strMasterName);
		s_text.put(iX+iContentsWidth,iY,LTGREEN,g_guild.m_strMasterName);

//	레벨
		iY		+=	18;
		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_LEVEL);
		s_text.put(iX+iContentsWidth,iY,LTGREEN,"%d",g_guild.m_wLevel);

		iY		+=	14;

#ifdef	_FOR_ENG
		iY		-=	2;
#endif

		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_HALL);

		if (g_guild.m_wHallLevel	==	0)
			s_text.put(iX+iContentsWidth,iY,GRAY,dMSG_NOTHING);
		else
			s_text.put(iX+iContentsWidth,iY,LTGREEN,"Level %d",g_guild.m_wHallLevel);

		iY		+=	18;
#ifdef	_FOR_ENG
		iY		-=	7;
#endif
//	경험치 세율
		s_text.putRF(iX	+iRightFitWidth,iY,WHITE,dMSG_GUILD_EXP_POLICY);

		if	(g_guild.m_wIsGetGuildExp)
			s_text.put(iX+iContentsWidth,iY,LTGREEN,dMSG_NOT_GAIN_GUILD_EXP);
		else
			s_text.put(iX+iContentsWidth,iY,WHITE,dMSG_GUILD_EXP_DONATION_FORM,g_guild.m_wExpTaxRate/100,g_guild.m_wExpTaxRate%100);

		iY		+=	14;
#ifdef	_FOR_ENG
		iY		+=	2;
#endif

		if	(g_guild.m_bf1IsPermitVisitor)
			s_text.put(iX,iY,LTGREEN,dMSG_PERMIT_GUILD_HALL_VISTORS);
		else
			s_text.put(iX,iY,LTYELLOW,dMSG_DENY_GUILD_HALL_VISTORS);

		CGuildMarkInfo	*lpInfo	=	&g_guild.m_guildMarkInfo;

		g_guildMark.putMark(iX+195,iY-40,lpInfo->m_wIcon,lpInfo->m_bIconColor,lpInfo->m_bBaseShape,lpInfo->m_bBaseColorPartition,lpInfo->m_bBaseColor1,lpInfo->m_bBaseColor2,lpInfo->m_bBaseLineColor,0,0,250);
	}

//	재원
	{
		iY		+=	26;
#ifdef	_FOR_ENG
		iY		+=	4;
#endif
		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_CAPITAL);
		s_text.put(iX+iContentsWidth,iY,GOLDYELLOW,"<c:LTYELLOW>%s Gold",GetMoneyString(g_guild.m_llGuildGold));

		iY		+=	20;
#ifdef	_FOR_ENG
		iY		-=	3;
#endif
		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_MEMBER);
		s_text.put(iX+iContentsWidth,iY,WHITE,dMSG_GUILD_MEMBER_COUNT_FORM,g_guild.m_wMemberCount,g_guild.m_wMaxMember);
		iY		+=	12;
		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_SUB_MASTER);
		s_text.put(iX+iContentsWidth,iY,WHITE,dMSG_GUILD_SUB_MASTER_COUNT_FORM,g_guild.m_wSubMasterCount,g_guild.m_wMaxSubMaster);
		iY		+=	12;
		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_CONGRESS_MAN);
		s_text.put(iX+iContentsWidth,iY,WHITE,dMSG_GUILD_CONGRESS_MAN_COUNT_FORM,g_guild.m_wCongressManCount,g_guild.m_wMaxCongressMan);
		iY		+=	12;
		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_GUARDIAN);
		s_text.put(iX+iContentsWidth,iY,WHITE,dMSG_GUILD_GUARDIAN_FROM,g_guild.m_wMaxGuardianCount);
	}

//	길드전
	{
		iY		+=	22;

#ifdef	_FOR_ENG
		iY		+=	7;
#endif
		CGuildBattleInfoForMember	*lpInfo	=	&g_guild.m_battleInfo;

		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_SIEGE_WARFARE_OF_GUILD);

		if	(lpInfo->m_bBSW_Hour	==	0xff)
			s_text.put(iX+iContentsWidth,iY,GRAY,dMSG_NOTHING);
		else
		{
			char	*lpstrTimeString	=	GetTimeStringHM(lpInfo->m_bBSW_Hour,lpInfo->m_bBSW_Minute);
			s_text.put(iX+iContentsWidth,iY,LTGREEN,lpstrTimeString);
		}

		iY		+=	14;
#ifdef	_FOR_ENG
		iY		-=	2;
#endif

		s_text.putRF(iX+iRightFitWidth,iY,WHITE,dMSG_GUILD_BATTLE);

		if	(!g_guild.isBookedIGB())
			s_text.put(iX+iContentsWidth,iY,GRAY,dMSG_NOTHING);
		else
		{
			char	*lpstrTimeString	=	GetTimeStringHM(lpInfo->m_bIGB_Hour,lpInfo->m_bIGB_Minute);
			s_text.put(iX+iContentsWidth,iY,LTGREEN,lpstrTimeString);
		}

#ifdef	_FOR_ENG
		iY		+=	2;
#endif

		iY		+=	20;
		s_text.putRF(iX+iRightFitWidth	,iY,WHITE,dMSG_GUILD_HONOR_POINT);
		s_text.put(iX+iContentsWidth	,iY,LTGREEN,"%d",g_guild.m_iHonorPoint);
	}

//	연고지
	{
		iY		+=	28;

		int	iConnectionVillageCount	=	0;

		for (i=0;i<dMAX_GUILD_VILLAGE_COUNT;i++)
		{
			int	iFieldSerial	=	g_guild.m_awGuildVillage[i];

			if	(iFieldSerial	==	0xffff)	
				continue;

			char *lpstrFieldName=	cMAP::GetFieldName(iFieldSerial);

			if	(lpstrFieldName	==	NULL)	
				continue;

			s_text.put(iX	,iY+iConnectionVillageCount*16,BTBLUE,lpstrFieldName);
			iConnectionVillageCount++;
		}

		iY		+=	(dMAX_GUILD_VILLAGE_COUNT*16);
	}

	iY		+=	14;
	{
		int		iNoticeWidth	=	g_iScreenWidth-iX-8;

		s_text.put(iX,iY,iNoticeWidth,60,0,GOLDYELLOW,g_guild.m_strNotice);
//		s_text.cPut(iX,iY,iNoticeWidth,60,0,LTYELLOW,dFITTEXT_HEIGHT_CENTER,g_guild.m_strNotice);
		int		iLineCount		=	s_text.getLineCount(iNoticeWidth,g_guild.m_strNotice);

		iY		+=	(16*iLineCount);
	}
}


//
//	길드 스킬

void
CGuildWindow::drawGuildSkill()
{
	int	i,iX,iY,iCount= 0 ;

	iX		=	m_pos.x;
	iY		=	m_pos.y;

	g_sprInterface.Put(iX+7,469+g_iCorrectBI_Y/2		,eWINDOW_SEPERATE_BAR);	//	경계

	iY		+=	35;
//		m_abGuildSkill
	for (i=0;iCount < g_gsManager.m_iSkillCount && i < dMAX_GUILD_SKILL_COUNT;i++)
	{
		CGuildSkill		*lpGuildSkill	=	&g_gsManager.m_aSkills[i];

		if	(lpGuildSkill->m_wSerial	==	0xffff)	
			continue;

		if	(m_iSelectTab	==	eGWTB_GUILD_SKILL	&&	lpGuildSkill->m_bf1IsStatueSkill	==	TRUE)
			continue;

		if	(m_iSelectTab	==	eGWTB_GUILD_ITEM	&&	lpGuildSkill->m_bf1IsStatueSkill	==	FALSE)
			continue;

		if	(iCount	<	m_iGuildSkillWindowScrollBarPos)
		{
			iCount++;
			continue;
		}

		BOOL	bIsShowSerial	=	FALSE;

#ifdef	_DEBUG
		if	(i	>=	c_iGuildStatueSkillBeginIndex)
			bIsShowSerial		=	TRUE;
#endif

		int		iPutX	=	iX;
		int		iPutY	=	iY+(iCount-m_iGuildSkillWindowScrollBarPos)*19;
		DWORD	dwColor	=	WHITE;

		//아이콘 출력
		g_smiIconGuildSkill.putScale(lpGuildSkill->m_wIconIndex,iPutX+10,iPutY,50);

		if	(bIsShowSerial)
		{
			if	(g_guild.m_abGuildSkill[i])
				s_text.put(iPutX+30,iPutY+2,dwColor	,"%s[<c:LTGREEN>Lv %d<n>]:%d",lpGuildSkill->m_strName,g_guild.m_abGuildSkill[i],i);
			else
				s_text.put(iPutX+30,iPutY+2,GRAY	,"%s:%d",lpGuildSkill->m_strName,i);
		}
		else
		{
			if	(g_guild.m_abGuildSkill[i])
				s_text.put(iPutX+30,iPutY+2,dwColor	,"%s[<c:LTGREEN>Lv %d<n>]",lpGuildSkill->m_strName,g_guild.m_abGuildSkill[i]);
			else
				s_text.put(iPutX+30,iPutY+2,GRAY	,lpGuildSkill->m_strName);
		}

		iCount++;

		if	(iCount-m_iGuildSkillWindowScrollBarPos>=	dVISIBLE_GUILD_SKILL_COUNT_IN_GUILD_SKILL_WINDOW)
			break;
	}

	s_sbPopupWindow.draw();
	s_bmPlusState.draw();

	if	(m_iSelectTab	==	eGWTB_GUILD_SKILL)
	{
		CPos	posGuildSkill;
		posGuildSkill.x = iX+10;
		posGuildSkill.y = 477+g_iCorrectBI_Y/2;
		if	(g_guild.m_bf6BonusGuildPoint)
		{
			if	(g_guild.m_bf6PeneltyBonusGuildPoint)
				s_text.put(posGuildSkill.x,posGuildSkill.y,WHITE	,dMSG_BONUS_PENELTY_GUILD_POINT_FORM,g_guild.m_wGuildPoint,g_guild.m_bf6BonusGuildPoint,g_guild.m_bf6PeneltyBonusGuildPoint);
			else
				s_text.put(posGuildSkill.x,posGuildSkill.y,WHITE	,dMSG_BONUS_GUILD_POINT_FORM,g_guild.m_wGuildPoint,g_guild.m_bf6BonusGuildPoint);
		}
		else
			s_text.put(posGuildSkill.x,posGuildSkill.y,WHITE	,"%s <c:LTYELLOW>%d<n>",dMSG_SKILL_POINT,g_guild.m_wGuildPoint);
	}
}

//
//	길드 아이템
void
CGuildWindow::drawGuildItem()
{
}

//
//	길드 관리 도구
void
CGuildWindow::drawGuildManageTool()
{
	s_bmMenu.draw();
}

//
//	길드 아이템
void
CGuildWindow::drawGuildMemberList()
{
	s_bmRadioBox.draw();
	s_sbPopupWindow.draw();

	int	i,iX,iY,iCount= 0 ;

	iX		=	m_pos.x+10;
	iY		=	m_pos.y+72;

	s_text.put(iX+2,m_pos.y+38,WHITE,dMSG_SORT_METHOD);

	CSimpleGuildMemberList	*lpList	=	&g_guild.m_memberList;

	{
		int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW)-12;

		iX	+=	8;
		g_sprInterface2.Put(iX,iY-16+5,eGUILD_PET_ICON_OBITIAN);
		s_text.put(iX+10,iY-16,WHITE,"[<c:LTGREEN>%d<n>/%d]",g_guild.getRentedPetCount(eGUILD_PET_OBITIAN),g_guild.m_wObitianCount);
		iX	+=	iWindowWidth/4;

		g_sprInterface2.Put(iX,iY-16+5,eGUILD_PET_ICON_DASH_BLADE);
		s_text.put(iX+10,iY-16,WHITE,"[<c:LTGREEN>%d<n>/%d]",g_guild.getRentedPetCount(eGUILD_PET_DASH_BLADE),g_guild.m_wDashBladeCount);
		iX	+=	iWindowWidth/4;

		g_sprInterface2.Put(iX,iY-16+5,eGUILD_PET_ICON_ASTRO_BOW);
		s_text.put(iX+10,iY-16,WHITE,"[<c:LTGREEN>%d<n>/%d]",g_guild.getRentedPetCount(eGUILD_PET_ASTRO_BOW),g_guild.m_wAstralBowCount);
		iX	+=	iWindowWidth/4;

		g_sprInterface2.Put(iX,iY-16+5,eGUILD_PET_ICON_CRISTAL_WATER);
		s_text.put(iX+10,iY-16,WHITE,"[<c:LTGREEN>%d<n>/%d]",g_guild.getRentedPetCount(eGUILD_PET_CRISTAL_WATER),g_guild.m_wCristalWaterCount);
		iX	+=	iWindowWidth/4;
	}

	iX		=	m_pos.x+10;

	for (i=0;i<lpList->m_wCount;i++)
	{
		CSimpleGuildMemberInfo	*lpMember	=	&lpList->m_aMembers[i+m_iGuildMemberListWindowScrollBarPos];

		if	(i+m_iGuildMemberListWindowScrollBarPos	==	m_iFocusedMember)
			cDRAW::FillHB(_LTGREEN,iX-1,iY-3,g_iScreenWidth-18,iY+17-3);

		int	iFPS		=	g_aJobIcon[lpMember->m_bf8Job].GetFPS(0);
		int	iFrameCount	=	g_aJobIcon[lpMember->m_bf8Job].GetFrameCount(0);
		cRECT rectGuildRank;
		int	iGuildRankIconWidth		=	g_sprInterface2.getSpriteWidth(eGUILD_PET_ICON_OBITIAN);
		int	iGuildRankIconHeight	=	g_sprInterface2.getSpriteHeight(eGUILD_PET_ICON_OBITIAN);
		int iGuildRankIconX			=	iX+8;
		int iGuildRankIconY			=	iY+5;
		int iGuildRankIconIndex		=	eGUILD_RANK_ICON_ROOKIE;
		
		rectGuildRank.set(iX,iY , iX+iGuildRankIconWidth, iY+ iGuildRankIconHeight);

		switch(lpMember->m_bf4Rank)
		{
			
		case	dGUILD_CLASS_ROOKIE			:
			iGuildRankIconIndex = eGUILD_RANK_ICON_ROOKIE;
			break;
		case	dGUILD_CLASS_MEMBER			:
			iGuildRankIconIndex = eGUILD_RANK_ICON_MEMBER;
			break;
		case	dGUILD_CLASS_PARLIAMENT		:
			iGuildRankIconIndex = eGUILD_RANK_ICON_PARLIAMENT;
			break;
		case	dGUILD_CLASS_SUBMASTER		:
			iGuildRankIconIndex = eGUILD_RANK_ICON_SUBMASTER;
			break;
		case	dGUILD_CLASS_ELDER			:
			iGuildRankIconIndex = eGUILD_RANK_ICON_ELDER;
			break;
		case	dGUILD_CLASS_MASTER			:
			iGuildRankIconIndex = eGUILD_RANK_ICON_MASTER;
			break;
		}

		g_sprInterface2.Put(iGuildRankIconX,iGuildRankIconY,iGuildRankIconIndex);

		cANM	*lpIcon			=	&g_aJobIcon[lpMember->m_bf8Job];
		WORD	*lpPalette		=	lpIcon->getOrgPalette();
		WORD	awPlt[256];
		int iJobIconX = iGuildRankIconX + 10;
		int iJobIconY = iY - 2;
		
		if(lpMember->m_bf1IsOnline )
		{
			lpIcon->PutReg(iJobIconX,iJobIconY,0,0,((s_iFrameCounter+iY)*iFPS/dSYNC_FPS)%iFrameCount);
		}
		else
		{
			EffectPalette(dPLT_EFFECT_GRAY,lpPalette,awPlt,80,0);
			lpIcon->setPalette(awPlt);
			lpIcon->PutReg(iJobIconX,iJobIconY,0,0,0);
			lpIcon->m_sprite.restorePalette();
		}


		int iNameX = iJobIconX + 32;
		int iNameY = iY;
		char	strName[dNAME_LENGTH];

		strcpy(strName,lpMember->getName());
		
		s_text.put(iNameX,iNameY,WHITE,"%s(<c:LTYELLOW>%d<n>)",strName,lpMember->m_bf11Level);

		int iReservePetX = iX+g_sprInterface.getSpriteWidth(eWINDOW)*0.85;
		int iReservePetY = iY+5;

		switch(lpMember->m_bf4ReservePet)
		{
		case	eGUILD_PET_OBITIAN		:
			g_sprInterface2.Put(iReservePetX,iReservePetY,eGUILD_PET_ICON_OBITIAN);
			break;
		case	eGUILD_PET_DASH_BLADE	:
			g_sprInterface2.Put(iReservePetX,iReservePetY,eGUILD_PET_ICON_DASH_BLADE);
			break;
		case	eGUILD_PET_ASTRO_BOW	:
			g_sprInterface2.Put(iReservePetX,iReservePetY,eGUILD_PET_ICON_ASTRO_BOW);
			break;
		case	eGUILD_PET_CRISTAL_WATER:
			g_sprInterface2.Put(iReservePetX,iReservePetY,eGUILD_PET_ICON_CRISTAL_WATER);
			break;
		}

		if(rectGuildRank.isIn(s_posMouse.x , s_posMouse.y))
			s_ttCommon.popupTip(&rectGuildRank,s_posMouse.x+8,s_posMouse.y,&s_text,WHITE,g_pstrGuildRank[lpMember->m_bf4Rank]);

		iY	+=	18;
		iCount++;
		if	(iCount	>=	dMAX_MEMBER_COUNT_IN_GUILD_WINDOW)
			break;
	}
}
//
//그리기
void
CGuildWindow::draw()
{
	if	(s_iPopupInterface!=	ePIW_GUILD)
		return;
	if	(s_propose.m_work.isReadyAskJoinGuild)
		return;

	int	iWindowWidth	=	g_sprInterface.getSpriteWidth(eWINDOW);
	int	iWindowHeight	=	g_sprInterface.getSpriteHeight(eWINDOW);

	int		iX,iY;

	iX		=	m_pos.x;
	iY		=	m_pos.y;

	if	(g_config.m_bIsInfoWindowHalfblending)
		cDRAW::FillHB(0,iX+8,iY+6,iX+266-4,iY+482-8);
	else
		cDRAW::Fill	 (0,iX+8,iY+6,iX+266-4,iY+482-8);

	if (g_hero.m_bf1IsWaitGuildInfo)
	{
		g_sprInterface.Put(iX,iY,eWINDOW);
		g_sprInterface.Put(iX,g_iCorrectBI_Y/2  ,eWINDOW_TITLE_GUILD);

		s_text.put(iX+14,iY+10,LTYELLOW,dMSG_WAIT_GUILD_INFO);

		return;
	}

	if (g_hero.m_wGuildSerial == 0xffff)
	{
		g_sprInterface.Put(iX,iY,eWINDOW);
		g_sprInterface.Put(iX,g_iCorrectBI_Y/2 ,eWINDOW_TITLE_GUILD);

		s_text.put(iX+14,iY+10,LTYELLOW,dMSG_NOT_OWNED_GUILD);

		s_bmButton.draw();

		return;
	}

	m_bmTab.draw();

	g_sprInterface.Put(iX,iY,eWINDOW);
	g_sprInterface.Put(iX,g_iCorrectBI_Y/2 ,eWINDOW_TITLE_GUILD);

	switch(m_iSelectTab)
	{
		case	eGWTB_GUILD_INFO	:
			drawGuildInfo();
			break;
		case	eGWTB_GUILD_SKILL	:
		case	eGWTB_GUILD_ITEM	:
			drawGuildSkill();
			break;

		case	eGWTB_TOOL			:
			drawGuildManageTool();
			break;
		case	eGWTB_MEMBER_LIST	:
			drawGuildMemberList();
			break;
	}

	{
		m_bmTab.draw();
		g_sprInterface.Put(m_pos.x+7,m_pos.y+26	,eWINDOW_SEPERATE_BAR);	//	경계

		int	iTabWidth	=	g_sprInterface.getSpriteWidth(eGW_TAB_ACTIVE);
		int	iX			=	m_pos.x+8+5+2;
		int	iY			=	m_pos.y+4;

		for (int i=0;i<m_bmTab.m_wCount;i++)
		{
			int	iPos	=	iX+i*iTabWidth;
			int	iImage	=	0xffff;

			if (m_bmTab.isCheck(i))
				iImage	=	eGW_INFO_ACTIVE+i;
			else
			if (m_bmTab.isFocusedMenu(i))
				iImage	=	eGW_INFO_FOCUS+i;
			else	
				iImage	=	eGW_INFO_NORMAL+i;

			g_sprInterface.Put(iPos,iY,iImage);
		}
	}
}	//	CGuildWindow::draw()

void
CGuildWindow::updateGuildInfo()
{
	cRECT	rectGuildBattleFrame;
	char	strText[512];

	CGuildBattleInfoForMember	*lpInfo;

	lpInfo	=	&g_guild.m_battleInfo;

	if	(lpInfo->m_bBSW_Hour	!=	0xff)
	{
		rectGuildBattleFrame.set(m_pos.x+30,236,g_iScreenWidth,236+16);

		cDRAW::Box(0xffff,&rectGuildBattleFrame);

		if	(rectGuildBattleFrame.isIn(s_posMouse.x,s_posMouse.y))
		{
			if	(lpInfo->m_bf1IsDefense	==	FALSE)
				sprintf(strText,dMSG_EXIST_SIEGE_WARFARE_FOR_CAPTURE_GUILD_HALL_FORM,lpInfo->m_bBSW_Hour,lpInfo->m_bBSW_Minute,lpInfo->m_strSW_TargetGuildName);
			else
				sprintf(strText,dMSG_EXIST_SIEGE_WARFARE_FOR_DEFENSE_GUILD_HALL_FORM,lpInfo->m_bBSW_Hour,lpInfo->m_bBSW_Minute,lpInfo->m_strSW_TargetGuildName);

			s_rectTooltip.set(&rectGuildBattleFrame);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strText,eTFM_BOTTOM);
		}
	}

	{
		rectGuildBattleFrame.set(m_pos.x+30,236+16,g_iScreenWidth,236+16+16);

		if	(rectGuildBattleFrame.isIn(s_posMouse.x,s_posMouse.y))
		{
			cSTRING	string;
			char	strText[256];

			if	(g_guild.isBookedIGB())
			{
				CGuildBattleInfoForMember	*lpInfo			=	&g_guild.m_battleInfo;
				char*						lpstrFieldName	=	cMAP::GetFieldName(lpInfo->m_wIGB_FieldSerial);

				if	(g_guild.isEngagedIGB())
					sprintf(strText,dMSG_ENGAGED_GUILD_BATTLE_COMMENT_FORM,GetTimeStringHM(lpInfo->m_bIGB_Hour,lpInfo->m_bIGB_Minute),lpstrFieldName,lpInfo->m_strIGB_TargetGuildName);
				else
					sprintf(strText,dMSG_BOOKED_GUILD_BATTLE_COMMENT_FORM,GetTimeStringHM(lpInfo->m_bIGB_Hour,lpInfo->m_bIGB_Minute),lpstrFieldName);

				s_rectTooltip.set(&rectGuildBattleFrame);

				string.Add(strText);
				string.Add("\n\n");
			}

			s_rectTooltip.set(&rectGuildBattleFrame);
//			s_rectTooltip.add(0,-20);

			if	(g_guild.m_wVictoryContinue >= 2)
			{
				string.Add(dMSG_GUILD_BATTLE_CONTINUE_WIN_FORM,g_guild.m_wVictoryContinue);
				string.Add("\n");
			}

			string.Add(dMSG_GUILD_BATTLE_RESULT_FORM,g_guild.m_wVictoryCount,g_guild.m_wDefeatCount,g_guild.m_wDrawCount,g_guild.m_iVictoryPoint);

			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,string.String,eTFM_BOTTOM);
		}
	}
}

int
CGuildWindow::getFocusedSkill()
{
	int		i,iX,iY,iCount= 0 ,iFocusSkill=0xffff;
	cRECT	rect;

	iX		=	m_pos.x+10;
	iY		=	m_pos.y+35;

	for (i=0;iCount < g_gsManager.m_iSkillCount && i < dMAX_GUILD_SKILL_COUNT;i++)
	{
		CGuildSkill		*lpGuildSkill	=	&g_gsManager.m_aSkills[i];

		if	(lpGuildSkill->m_wSerial	==	0xffff)
			continue;

		if	(m_iSelectTab	==	eGWTB_GUILD_SKILL	&&	lpGuildSkill->m_bf1IsStatueSkill	==	TRUE)
			continue;

		if	(m_iSelectTab	==	eGWTB_GUILD_ITEM	&&	lpGuildSkill->m_bf1IsStatueSkill	==	FALSE)
			continue;

		if	(iCount	<	m_iGuildSkillWindowScrollBarPos	)
		{
			iCount++;
			continue;
		}

		int		iPutX	=	iX;
		int		iPutY	=	iY+(iCount-m_iGuildSkillWindowScrollBarPos)*19;
		int		iSkillNameWidth;

		
		if (g_guild.m_abGuildSkill[i] && g_guild.m_abGuildSkill[i])
				iSkillNameWidth	=	s_text.getPixelSize(_ms("%s[Lv %d]",lpGuildSkill->m_strName,g_guild.m_abGuildSkill[i]));
		else	iSkillNameWidth	=	s_text.getPixelSize(lpGuildSkill->m_strName);

		rect.set(iPutX,iPutY,iPutX+iSkillNameWidth+20,iPutY+16);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			s_rectTooltip.set(&rect);
			s_rectTooltip.add(-2,-4);
			iFocusSkill	=	i;
			break;
		}

		iCount++;

		if (iCount-m_iGuildSkillWindowScrollBarPos>=	dVISIBLE_GUILD_SKILL_COUNT_IN_GUILD_SKILL_WINDOW)	break;
	}

	return	iFocusSkill;
}

void
CGuildWindow::updateGuildSkill()
{
	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SCROLL_BAR))
	{
		if	(s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);

		if	(m_iGuildSkillWindowScrollBarPos	!=	s_sbPopupWindow.m_iCurrentPos)
		{
			m_iGuildSkillWindowScrollBarPos		=	s_sbPopupWindow.m_iCurrentPos;
			resetSkillPlusButtonsPos();
		}
	}

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_PLUS_BUTTONS))
	{
		if	(g_hero.isGuildMaster())
		{
			DWORD	dwMenu	=	s_bmPlusState.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

			if (dwMenu	!=	0xffffffff && g_hero.m_bf1IsWaitGuildCommandResult == FALSE)
			{
				s_agent.sendIncreaseGuildSkillLevel(dwMenu);
				g_hero.m_bf1IsWaitGuildCommandResult	=	TRUE;
			}

			if	(s_bmPlusState.isPressed())
				cINPDEV::SetExclusive(eMOUSE_STATE_PLUS_BUTTONS);
			else
				cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_PLUS_BUTTONS);
		}
	}

	if	(cINPDEV::IsExclusive())
		return;

	m_iFocusedSkill	=	0xffff;

	if	(IsValidFocus())
		m_iFocusedSkill	=	getFocusedSkill();

	if	(m_iFocusedSkill		!=	0xffff)
	{
		CGuildSkill	*lpSkill=	g_gsManager.get(m_iFocusedSkill);
		char	*strComment	=	g_guild.getSkillComment(m_iFocusedSkill);

		if	(strComment && lpSkill)
		{
			s_ttCommon.popupTip(&s_rectTooltip,-1,-1,&s_text,WHITE,strComment,eTFM_LEFT);

			WORD	*lpImage	=	g_smiIconGuildSkill.get16Image(lpSkill->m_wIconIndex);
			s_ttCommon.setImage(lpImage);
		}
	}
}

void
CGuildWindow::updateGuildItem()
{
}

void
CGuildWindow::updateManageTool()
{
	DWORD	dwMenu	=	s_bmMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	switch(dwMenu)
	{
		case	eGWB_CHANGE_NOTICE				:
			if (!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			s_inputChatting.setStr(0,_ms("@%s %s",dMSG_GUILD_NOTICE,dMSG_PLEASE_INPUT_GUILD_NOTICE));

			s_inputChatting.setSelect(0,strlen(dMSG_GUILD_NOTICE)+2,-1);
			break;

		case	eGWB_CHANGE_GUILD_EXP_TAX_RATE	:
		{
			if	(!s_inputChatting.isEnable())
				EnableChattingBar(TRUE);

			int	iMinTaxRate	=	g_guild.getMinimumExpTax();
			int	iMaxTaxRate	=	g_guild.getMaximumExpTax();

			s_inputChatting.setStr(0,_ms(dMSG_CHANGE_GUILD_EXP_TAX_RATE_FORM,dMSG_GUILD_TAX,
								iMinTaxRate/100,iMinTaxRate%100,
								iMaxTaxRate/100,iMaxTaxRate%100,
								g_guild.m_wExpTaxRate/100,g_guild.m_wExpTaxRate%100));

			s_inputChatting.setSelect(0,strlen(dMSG_GUILD_TAX)+2,-1);

			break;
		}
			
		case	eGWB_LEAVE_GUILD				:
			if (!g_hero.isGuildMember())	//	길마
			{
				g_msgBox.cPopup("",dMSG_ONLY_GUILD_MEMBER_LEAVE_GUILD,dMSG_OK);
				return;
			}

			g_msgBox.cPopup("",dMSG_ARE_YOU_LEAVE_GUILD,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_LEAVE_GUILD);
			break;

		case	eGWB_CHNAGE_GUILD_HALL_OPEN_STATUS	:
		{
			if	(!g_hero.isGuildMaster())	//	길마
			{
				g_msgBox.cPopup("",dMSG_PERMIT_ONLY_GUILD_MASTER_WORK,dMSG_OK);

				return;
			}

			s_agent.sendEtcWork(eCEW_CHANGE_OPEN_GUILD_HALL_STATUS);
			break;
		}

		case	eGWB_TRANS_AVATAR_TO_GVG_SERVER		:
		{
			g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_TRANS_AVATAR_TO_GVG_SERVER,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_TRANS_AVATAR_TO_GVG_SERVER);
			break;
		}

		case	eGWB_TRANS_GUILD_TO_GVG_SERVER		:
		{
			g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_TRANS_GUILD_TO_GVG_SERVER,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_TRANS_GUILD_TO_GVG_SERVER);
			break;
		}

		case	eGWB_ACTIVE_GUILD_MASTER_GRACE	:			//길드 마스터 은총 발동.
		{
			g_msgBox.cPopup("",dMSG_ARE_YOU_ACTIVE_GUILD_MASTER_GRACE,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_ACTIVE_GUILD_MASTER_GRACE);
		}
			break;
		case	eGWB_DISSOULTION_GUILD			:
			if (!g_hero.isGuildMaster())	//	길마
			{
				g_msgBox.cPopup("",dMSG_ONLY_GUILD_MASTER_DISSOLUTION_GUILD,dMSG_OK);

				return;
			}
			if (g_guild.m_wMemberCount > 1)	//	길마
			{
				g_msgBox.cPopup("",dMSG_DISSOLUTION_GUILD_CONDITION_COMMENT,dMSG_OK);

				return;
			}

			g_msgBox.cPopup("",dMSG_DO_YOU_WANT_DISSOLUTION_GUILD,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_DISSOLUTION_GUILD);
			break;

		case	eGWB_SUMMON_GUILD_GUARDIAN_SHIELD_CRAFT		:
		case	eGWB_SUMMON_GUILD_GUARDIAN_ELEMENTAL_POST	:
		case	eGWB_SUMMON_GUILD_GUARDIAN_ETHER_SHELL		:
		case	eGWB_SUMMON_GUILD_GUARDIAN_HOLLOW_KNIGHT	:
		{
			int	iGuardianType	=	dwMenu-eGWB_SUMMON_GUILD_GUARDIAN_SHIELD_CRAFT+1;

			s_agent.sendEtcWork(eCEW_SUMMON_GUILD_GUARDIAN,iGuardianType);

			break;
		}

		case	eGWB_UNSUMMON_NEAREST_GUILD_GUARDIAN		:
			s_agent.sendEtcWork(eCEW_UNSUMMON_GUILD_GUARDIAN);
			break;
	}
}

//
//	길드 아이템
int
CGuildWindow::getFocusedMember()
{
	int		i,iX,iY,iCount= 0 ;

	iX		=	m_pos.x+10;
	iY		=	m_pos.y+72;

	for (i=0;i<g_guild.m_memberList.m_wCount;i++)
	{
		cRECT	rect;

		rect.set(iX,iY-2,g_iScreenWidth-10,iY+16);

		if	(rect.isIn(s_posMouse.x,s_posMouse.y))
			return	i+m_iGuildMemberListWindowScrollBarPos;

		iY	+=	18;
		iCount++;

		if	(iCount	>=	dMAX_MEMBER_COUNT_IN_GUILD_WINDOW)
			break;
	}

	return	0xffff;
}

void
CGuildWindow::selectMemberListPopupMenu(int _iMenu)
{
	char					strMessage[256];
	CSimpleGuildMemberInfo	*lpMember	=	g_guild.getMember(m_iSelectMember);

	if (!lpMember)
		return;

	switch(_iMenu)
	{
		case	ePM_RENT_GUILD_PET_OBITIAN		:
		case	ePM_RENT_GUILD_PET_DASH_BLADE	:
		case	ePM_RENT_GUILD_PET_ASTRO_BOW	:
		case	ePM_RENT_GUILD_PET_CRISTAL_WATER:
		{
			int	iPetType	=	_iMenu-ePM_RENT_GUILD_PET_OBITIAN+1;
			s_agent.sendGuildMemberWork(eGMW_RENT_GUILD_PET,lpMember->getName(),iPetType);
			break;
		}

		case	ePM_RETRIEVAL_GUILD_PET			:
			s_agent.sendGuildMemberWork(eGMW_RETRIEVAL_RENTED_GUILD_PET,lpMember->getName());
			break;

		case	ePM_REGIST_REGULAR_GUILD_MEMBER	:
			sprintf(strMessage,dMSG_REGIST_REGULARMEMBER_FORM,lpMember->getName());

			g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_REGIST_REGULAR_MEMBER);

			strcpy(s_strBookedUserName,lpMember->getName());
			break;

		case	ePM_APPOINTMENT_SUB_GUILD_MASTER:
			char	strMessage[256];

			sprintf(strMessage,dMSG_APPOINTMENT_SUB_GUILD_MASTER_FORM,lpMember->getName());

			g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_APPOINTMENT_SUB_GUILD_MASTER);

			strcpy(s_strBookedUserName,lpMember->getName());
			break;

		case	ePM_DISMISS_SUB_GUILD_MASTER	:
			sprintf(strMessage,dMSG_DISMISS_SUB_GUILD_MASTER_FORM,lpMember->getName());

			g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_DISMISS_SUB_GUILD_MASTER);

			strcpy(s_strBookedUserName,lpMember->getName());
			break;

		case	ePM_DISMISS_PARLIAMENT			:
			sprintf(strMessage,dMSG_DISMISS_GUILD_CONGRESS,lpMember->getName());

			g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_DISMISS_GUILD_CONGRESS);

			strcpy(s_strBookedUserName,lpMember->getName());
			break;

		case	ePM_APPOINTMENT_GUILD_MASTER	:
			sprintf(strMessage,dMSG_CHANGE_GUILD_MASTER_FORM,lpMember->getName());

			g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_CHANGE_GUILD_MASTER);

			strcpy(s_strBookedUserName,lpMember->getName());
			break;

		case	ePM_BANISH_MEMBER				:
			sprintf(strMessage,dMSG_ARE_YOU_REALLY_BANISH_FOLLOW_GUILD_MEMBER,lpMember->getName());

			g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_BANISH_GUILD_MEMBER);

			strcpy(s_strBookedUserName,lpMember->getName());
			break;
	}
}

//
//	아이템 윈도우에 팝업 메뉴 처리
void 
CGuildWindow::operateGuildMemberListPopupMenu()
{
	if	(cINPDEV::IsExclusive(eMOUSE_STATE_POPUP_MENU))
		return;

	if	(!s_isClickedRightButton	)
		return;

	m_iFocusedMember	=	getFocusedMember();

	m_iSelectMember	=	m_iFocusedMember;

	CSimpleGuildMemberInfo	*lpMember	=	NULL;
	
	if(m_iSelectMember != 0xffff)
		lpMember = &g_guild.m_memberList.m_aMembers[m_iFocusedMember];

	if	(!lpMember || s_pmCommon.isPopuped())
	{
		s_pmCommon.reset();
		
		return;
	}
	
	s_pmCommon.popup(s_posMouse.x,s_posMouse.y+20);
	s_ttCommon.hide();


	if(m_iSelectMember != 0xffff)
	{
		s_pmCommon.addMenu(ePM_PRM_ASK_GUILD_MEMBER_JOIN_PARTY	,dMSG_ASK_JOIN_PARTY);
		strcpy(s_strBookedUserName, lpMember->getName());
	}

	if	(!g_hero.isGuildLeader()	||	m_iFocusedMember	==	0xffff	)
		return;

	if	(lpMember->m_bf4Rank	!=	dGUILD_CLASS_MASTER	&&	stricmp(lpMember->getName(),g_hero.m_strName)	!=	0)
		s_pmCommon.addMenu(ePM_BANISH_MEMBER	,dMSG_BANISH_GUILD_MEMBER);

	if	(lpMember->m_bf4Rank	==	dGUILD_CLASS_ROOKIE)
		s_pmCommon.addMenu(ePM_REGIST_REGULAR_GUILD_MEMBER	,dMSG_REGIST_REGULAR_GUILD_MEMBER);

	if	(g_hero.isGuildMaster())
		if	(lpMember->m_bf4Rank	==	dGUILD_CLASS_MEMBER	||	lpMember->m_bf4Rank	==	dGUILD_CLASS_PARLIAMENT)
			s_pmCommon.addMenu(ePM_APPOINTMENT_SUB_GUILD_MASTER	,dMSG_APPOINTMENT_SUB_GUILD_MASTER);

	if	(g_hero.isGuildMaster())
		if	(lpMember->m_bf4Rank	==	dGUILD_CLASS_PARLIAMENT)
			s_pmCommon.addMenu(ePM_DISMISS_PARLIAMENT	,dMSG_DISMISS_GUILD_CONGRESS);

	if	(g_hero.isGuildMaster())
		if	(lpMember->m_bf4Rank	==	dGUILD_CLASS_SUBMASTER)
		{
			s_pmCommon.addMenu(ePM_APPOINTMENT_GUILD_MASTER	,dMSG_CHANGE_GUILD_MASTER);
			s_pmCommon.addMenu(ePM_DISMISS_SUB_GUILD_MASTER	,dMSG_DISMISS_SUB_GUILD_MASTER);
		}

	if	(g_hero.isGuildMaster()	)
	{
		s_pmCommon.addMenu(ePM_RENT_GUILD_PET_OBITIAN,_ms(dMSG_RENT_GUILD_PET_FORM,dMSG_GUILD_PET_OBITIAN));
		s_pmCommon.addMenu(ePM_RENT_GUILD_PET_DASH_BLADE,_ms(dMSG_RENT_GUILD_PET_FORM,dMSG_GUILD_PET_DASH_BLADE));
		s_pmCommon.addMenu(ePM_RENT_GUILD_PET_ASTRO_BOW,_ms(dMSG_RENT_GUILD_PET_FORM,dMSG_GUILD_PET_ASTRO_BOW));
		s_pmCommon.addMenu(ePM_RENT_GUILD_PET_CRISTAL_WATER,_ms(dMSG_RENT_GUILD_PET_FORM,dMSG_GUILD_PET_CRISTAL_WATER));

		if	(lpMember->m_bf4ReservePet)
			s_pmCommon.addMenu(ePM_RETRIEVAL_GUILD_PET	,dMSG_RETRIEVAL_GUILD_PET);
	}



	if	(s_pmCommon.getCount())
		cINPDEV::SetExclusive(eMOUSE_STATE_POPUP_MENU);
	else
		s_pmCommon.reset();
}


void
CGuildWindow::updateMemberList()
{
	operateGuildMemberListPopupMenu();

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SCROLL_BAR))
	{
		if (s_sbPopupWindow.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
			cINPDEV::SetExclusive(eMOUSE_STATE_SCROLL_BAR);
		else
			cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_SCROLL_BAR);

		m_iGuildMemberListWindowScrollBarPos	=	s_sbPopupWindow.m_iCurrentPos;
	}

	if (!cINPDEV::IsExclusive(eMOUSE_STATE_PLUS_BUTTONS))
	{
		DWORD	dwMenu	=	s_bmRadioBox.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (dwMenu	!=	0xffffffff)
			g_guild.sortMemberList(dwMenu);

		if (s_bmRadioBox.isPressed())
			cINPDEV::SetExclusive(eMOUSE_STATE_PLUS_BUTTONS);
		else
			cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_PLUS_BUTTONS);
	}

	if	(cINPDEV::IsExclusive())
		return;

	m_iFocusedMember	=	0xffff;

	if	(IsValidFocus())	
		m_iFocusedMember	=	getFocusedMember();

}

//
//업데이트
void
CGuildWindow::update()
{
	if	(s_iPopupInterface	!=	ePIW_GUILD	)
		return;

	if	(s_propose.m_work.isReadyAskJoinGuild)
		return;

	s_rectInterfaceSize.x1 = g_iScreenWidth-g_sprInterface.getSpriteWidth(eWINDOW);
	s_rectInterfaceSize.x2 = g_iScreenWidth;

	DWORD	dwMenu;

	if	(g_hero.m_wGuildSerial == 0xffff		)
	{
		m_iSelectTab	=	eGWTB_GUILD_INFO;
		m_bmTab.check(m_iSelectTab);

		if	(g_iWorldServerType	!=	eSERVER_TYPE_GVG)
		{
			s_bmButton.show(eGWB_ASK_JOIN_GUILD);
			s_bmButton.show(eGWB_TRANS_AVATAR_TO_GVG_SERVER);
		}

		dwMenu		=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if	(s_bmButton.isPressed())
			cINPDEV::SetExclusive(eMOUSE_STATE_GUILD_WINDOW_BUTTONS);
		else
			cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_GUILD_WINDOW_BUTTONS);

		switch(dwMenu)
		{
			case	eGWB_ASK_JOIN_GUILD	:
			{
				if	(g_hero.m_iLevel < 10 && !g_hero.m_bf4RebirthCount)		// 전생자는 제외한다.
				{
					g_msgBox.cPopup("",dMSG_GUILD_MEMBER_MINIMUM_LEVEL,dMSG_OK);
					break;
				}
				s_propose.m_work.isReadyAskJoinGuild	=	TRUE;
				s_rectInterfaceSize.set(0, 0, 0, 0);

				cINPDEV::SetExclusive(eMOUSE_STATE_PROPOSE_TO_WORK);
				break;
			}

			case	eGWB_TRANS_AVATAR_TO_GVG_SERVER		:
			{
				g_msgBox.cPopup("",dMSG_ARE_YOU_REALLY_TRANS_AVATAR_TO_GVG_SERVER,dMSG_YES,dMSG_NO);
				g_msgBox.setButtonId(eMBI_TRANS_AVATAR_TO_GVG_SERVER);
				break;
			}

		}

		return;
	}

	if	(g_iWorldServerType	!=	eSERVER_TYPE_GVG)
	{
		s_bmButton.hide(eGWB_ASK_JOIN_GUILD);
		s_bmButton.hide(eGWB_TRANS_AVATAR_TO_GVG_SERVER);
	}

	if	(!cINPDEV::IsExclusive(eMOUSE_STATE_SCROLL_BAR))
	{
		if(s_rectInterfaceSize.isIn(s_posMouse.x,s_posMouse.y))
		{
			if (s_iWheelValue<0)
				s_sbPopupWindow.setPos(s_sbPopupWindow.m_iCurrentPos+1);
			if (s_iWheelValue>0)
				s_sbPopupWindow.setPos(s_sbPopupWindow.m_iCurrentPos-1);
		}
	}

	switch(m_iSelectTab)
	{
		case	eGWTB_GUILD_INFO	:
			updateGuildInfo();
			break;
		case	eGWTB_GUILD_SKILL	:
		case	eGWTB_GUILD_ITEM	:
			updateGuildSkill();
			break;
		case	eGWTB_MEMBER_LIST	:
			updateMemberList();
			break;
		case	eGWTB_TOOL			:
			updateManageTool();
			break;
	}

	if	(cINPDEV::IsExclusive(eMOUSE_STATE_GUILD_WINDOW_BUTTONS))
		return;

//	길드탭 업데이트
	{
		dwMenu	=	m_bmTab.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (dwMenu	!=	0xffffffff)
			changeTab(m_bmTab.getCheckedMenu());

		if	(m_bmTab.isPressed())
			cINPDEV::SetExclusive(eMOUSE_STATE_GUILD_WINDOW_BUTTONS);
		else
		{
			cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_GUILD_WINDOW_BUTTONS);
			
			cBAR	*lpFocusedBar	=	m_bmTab.getFocusMenu(s_posMouse.x,s_posMouse.y);

			if (lpFocusedBar)
			{
				cRECT	rect;

				rect.set(&lpFocusedBar->m_rectCrash);
				rect.add(0,26);

				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,lpFocusedBar->getTooltip());
			}
		}
	}	//	각종 메뉴들의 툴팁

}

//
//
void
CGuildWindow::askJoinGuild(char *_lpAskerName,int _iAskerSerial,int _iLevel,int _iJob)
{
	if	(g_msgBox.isPoped())
	{
		s_agent.sendReplyJoinGuild(eAJGR_BUSY,_lpAskerName,_iAskerSerial);

		return;
	}

	char	strMessage[256];

	strcpy(m_strJoinGuildAsker,_lpAskerName);
	m_wJoinGuildAskerSerial	=	_iAskerSerial;

	sprintf(strMessage,dMSG_REQEUST_JOIN_GUILD_FORM,_iLevel,g_pstrHeroJobName[_iJob],_lpAskerName);

	g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
	g_msgBox.setButtonId(eMBI_ACCEPT_JOIN_GUILD_REQUEST,eMBI_DECLINE_JOIN_GUILD_REQUEST);
}