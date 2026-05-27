#include "CGuildBattleScheduler.h"
#include "CMessage.h"
#include "CHero.h"
#include "CMap.h"

CGuildBattleScehduler	g_gwGuildBattleScheduler;

#define	dGBI_FILED_LINE_GAB			39
#define	dGBI_FILED_SECOND_LINE_POS	18

const	int	c_iGBS_Time				=	140;
const	int	c_iGBS_FieldName		=	240;
const	int	c_iGBS_GuildName		=	278;
const	int	c_iGBS_FieldOrerGapTime	=	5;

enum
{
	eGBSBM_CLOSE	=	1000,
};

CGuildBattleScehduler::CGuildBattleScehduler()
{
	m_iSelectTime	=	0;
}

//
//	열렸냐?
BOOL
CGuildBattleScehduler::isOpened()
{
	if	(s_iPopupInterface	==	ePIW_GUILD_BATTLE_SCHEDULER)
		return	TRUE;

	return	FALSE;
}

void
CGuildBattleScehduler::open(int _iBeginHour,int _iBeginMinute,int _iBattlePeriod,int _iBattleCount)
{
	m_wBeginHour	=	_iBeginHour;
	m_wBeginMinute	=	_iBeginMinute;
	m_wBattlePeriod	=	_iBattlePeriod;
	m_wBattleCount	=	_iBattleCount;

	if	(s_iPopupInterface	==	ePIW_GUILD_BATTLE_SCHEDULER)
	{
		ClosePopupWindow();
		return;
	}

	ClosePopupWindow();

	s_bmToolButton.reset();

	s_iPopupInterface			=	ePIW_GUILD_BATTLE_SCHEDULER;
	s_rectInterfaceSize.x1 = g_iScreenWidth-(g_iScreenWidth/3+12);
	s_rectInterfaceSize.x2 = g_iScreenWidth;

	s_agent.sendAskGuildBattleInfoByTimeOrder(m_iSelectTime);
	g_hero.m_bf1IsWaitGuildCommandResult	=	TRUE;

	{
		m_bmButton.init(&g_sprInterface);

		int	iBarWidth	=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);
		int	iBarHeight	=	g_sprInterface.getSpriteHeight(eBUTTON_NORMAL);
		int	iX			=	g_iScreenWidth/3+12+c_iGBS_GuildName+256/2-iBarWidth/2-g_msgBox.getFrame()->m_iLeftBorderWidth,iY=63,i;

		for (i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
		{
			m_bmButton.addImageBar(dMSG_REQUEST_BATTLE	,i		,iX,iY+i*dGBI_FILED_LINE_GAB+dGBI_FILED_LINE_GAB/2-iBarHeight/2,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE);
			m_bmButton.addImageBar(dMSG_REQUEST_BATTLE	,i+100	,iX,iY+i*dGBI_FILED_LINE_GAB+dGBI_FILED_SECOND_LINE_POS,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE);
			m_bmButton.addImageBar(dMSG_CANCEL			,i+200	,iX,iY+i*dGBI_FILED_LINE_GAB+dGBI_FILED_SECOND_LINE_POS,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE);
			m_bmButton.hide(i);
			m_bmButton.hide(i+100);
			m_bmButton.hide(i+200);
		}
 
		cRECT	rect;

		rect.set(g_iScreenWidth/3+12,6,g_iScreenWidth-6,g_iFieldScreenHeight-30);

		m_bmButton.addImageBar(dMSG_CLOSE	,eGBSBM_CLOSE	,
								rect.x1+rect.getWidth()/2-iBarWidth/2,
								rect.y2-23,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE);

		m_bmButton.setAllBarTextColor(0,FALSE);
		m_bmButton.setClicktAllBarMargin(1,1);
	}
}

void
CGuildBattleScehduler::close()
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_BATTLE_SCHEDULER)	
		return;

	cINPDEV::ReleaseExclusive();
	m_bmButton.reset();
	s_iPopupInterface		=	ePIW_NONE;
	s_rectInterfaceSize.set(0, 0, 0, 0);

	s_agent.sendCloseGuildBattleScheduler();
}
 
//
//	업데이트
void
CGuildBattleScehduler::updateGuildBattleSchedule(WORD _wTimeOrder,CGuildBattleInFieldInfoForClient *_lpData,int _iYear,int _iMonth,int _iDay)
{
	if (_wTimeOrder	!=	m_iSelectTime)
	{
		s_agent.sendAskGuildBattleInfoByTimeOrder(m_iSelectTime);
		g_hero.m_bf1IsWaitGuildCommandResult	=	TRUE;

		return;
	}

	m_wYear	=	_iYear;
	m_wMonth=	_iMonth;
	m_wDay	=	_iDay;

	memcpy(&m_schedule,_lpData,sizeof(CGuildBattleInFieldInfoForClient));
}

//
//	업데이트
void
CGuildBattleScehduler::updateGuildBattleSchedule(WORD _wTimeOrder,WORD _wFieldOrder,CGuildBattleInfoForClient *_lpData)
{
	if	(_wTimeOrder	!=	m_iSelectTime)
		return;

	if	(_wFieldOrder	==	0xffff)
	{
		for	(int i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
			memset(m_schedule.m_aBattleGuildInfo[i].m_awBattleGuild,0xff,sizeof(m_schedule.m_aBattleGuildInfo[i].m_awBattleGuild));
	}
	else
		memcpy(&m_schedule.m_aBattleGuildInfo[_wFieldOrder],_lpData,sizeof(CGuildBattleInfoForClient));
}

//
//	마우스 커서가 가르키고 있는 타임을 얻는다.
//	타임이 변경되면 TRUE 리턴~
BOOL
CGuildBattleScehduler::getFocusTime()
{
	int		iX=g_iScreenWidth/3+12-g_msgBox.getFrame()->m_iLeftBorderWidth,iY=0,i;
	cRECT	rect;

	m_iFocusTime	=	0xffff;

	for (i=0;i<m_wBattleCount;i++)
	{
		rect.set(iX+5,iY+62+i*17,iX+73,iY+62+(i+1)*17);

		if (rect.isIn(s_posMouse.x,s_posMouse.y))
		{
			m_iFocusTime= i;

			return TRUE;
		}
	}

	return	FALSE;
}	

void
CGuildBattleScehduler::update()
{
	int		iX=g_iScreenWidth/3+12-g_msgBox.getFrame()->m_iLeftBorderWidth,iY=0,i;

	if	(s_iPopupInterface	!=	ePIW_GUILD_BATTLE_SCHEDULER)
		return;

	if	(g_guild.isBookedIGB() || g_hero.m_bf1IsWaitGuildCommandResult)
	{
		for (i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
		{
			m_bmButton.hide(i);
			m_bmButton.hide(i+100);
			m_bmButton.hide(i+200);
		}
	}

	if	(g_hero.m_bf1IsWaitGuildCommandResult)
		return;

	if	(g_guild.isBookedIGB())
	{
//		if	(!g_guild.isEngagedIGB())
			for (i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
			{
				if (m_schedule.m_aBattleGuildInfo[i].m_awBattleGuild[0]	==	g_guild.m_wSerial && 
					m_schedule.m_aBattleGuildInfo[i].m_awBattleGuild[1]	==	0xffff				)
					m_bmButton.show(i+200);
			}
	}
	else
	{
		for (i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
		{
			m_bmButton.hide(i+200);

			if (m_schedule.m_aBattleGuildInfo[i].m_awBattleGuild[0]	==	0xffff)
			{
				m_bmButton.show(i);
				m_bmButton.hide(i+100);
			}
			else
			if (m_schedule.m_aBattleGuildInfo[i].m_awBattleGuild[1]	==	0xffff)
			{
				m_bmButton.hide(i);
				m_bmButton.show(i+100);
			}
			else
			{
				m_bmButton.hide(i);
				m_bmButton.hide(i+100);
			}
		}
	}

	if (cINPDEV::IsExclusive(eMOUSE_STATE_GUILD_BATTLE_SCHEDULER))
		return;

	getFocusTime();

	if (m_iFocusTime != 0xffff && m_iSelectTime != m_iFocusTime && s_isTouchLeftButton)
	{
		m_iSelectTime	=	m_iFocusTime;
		s_agent.sendAskGuildBattleInfoByTimeOrder(m_iSelectTime);
		g_hero.m_bf1IsWaitGuildCommandResult	=	TRUE;

		return;
	}

	DWORD	dwSelectMenu	=	m_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

	if	(m_bmButton.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_GUILD_BATTLE_SCHEDULER);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_GUILD_BATTLE_SCHEDULER);

	if	 (dwSelectMenu	!=	0xffffffff)
	{
		if	(dwSelectMenu	==	eGBSBM_CLOSE)
		{
			close();
			return;
		}

		char						strText[256],*lpstrFieldName;
		CTimeInfo					beginTime;
		CGuildBattleInfoForClient	*lpInfo;

		if	(dwSelectMenu	>=	200)	//	길드전 취소
			m_iSelectFieldOrder		=	dwSelectMenu-200;
		else
		if	(dwSelectMenu	>=	100)	//	길드전 취소
			m_iSelectFieldOrder		=	dwSelectMenu-100;
		else
			m_iSelectFieldOrder		=	dwSelectMenu;

		getBeginBattleTime(m_iSelectTime,m_iSelectFieldOrder,&beginTime);

		lpInfo				=	&m_schedule.m_aBattleGuildInfo[m_iSelectFieldOrder];
		lpstrFieldName		=	cMAP::GetFieldName(m_schedule.m_aBattleGuildInfo[m_iSelectFieldOrder].m_wFieldSerial);

		if	(dwSelectMenu	>=	200)
		{
			sprintf(strText,dMSG_CANCEL_GUILD_BATTLE_FORM,beginTime.m_wYear,beginTime.m_wMonth,beginTime.m_wDay,beginTime.m_wHour,beginTime.m_wMinute,lpstrFieldName);

			g_msgBox.cPopup(dMSG_CANCEL_GUILD_BATTLE,strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_CANCEL_GUILD_BATTLE,eMBI_CANCEL);
		}
		else
		{
			if	(lpInfo->m_astrGuild[0][0])
				sprintf(strText,dMSG_GUILD_BATTLE_FORM,beginTime.m_wYear,beginTime.m_wMonth,beginTime.m_wDay,beginTime.m_wHour,beginTime.m_wMinute,lpstrFieldName,lpInfo->m_astrGuild[0]);
			else
				sprintf(strText,dMSG_REQEUST_GUILD_BATTLE_FORM,beginTime.m_wYear,beginTime.m_wMonth,beginTime.m_wDay,beginTime.m_wHour,beginTime.m_wMinute,lpstrFieldName);

			g_msgBox.cPopup(dMSG_REQUEST_GUILD_BATTLE,strText,dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(eMBI_REQUEST_GUILD_BATTLE,eMBI_CANCEL);
		}
	}

//	대전 길드를 체크해서 기타등등..
}

//
//	전투 시작 시간 얻기
void
CGuildBattleScehduler::getBeginBattleTime(int _iTimeOrder,int _iFieldOrder,CTimeInfo *_lpTime)
{
	int		iBeginTime	=	m_wBeginHour*60+m_wBeginMinute+_iTimeOrder*m_wBattlePeriod+_iFieldOrder/c_iSameTimeFiledCountInGuildBattle*c_iGBS_FieldOrerGapTime;

	_lpTime->m_wHour	=	iBeginTime/60;
	_lpTime->m_wMinute	=	iBeginTime%60;

	if	(_lpTime->m_wHour	>=	24)
	{
		_lpTime->m_wHour	-=	24;
		_lpTime->increaseDay(1);
	}
}

void
CGuildBattleScehduler::getTimeText(int _iIndex,int _iCurrentTime,char *_lpstrText,BOOL _bIsNoAmPm,BOOL bIsWantPutAMPM)
{
	int	iTime		=	_iCurrentTime/60;

	if	(_bIsNoAmPm	==	FALSE)
	{
		if	(iTime	>=	24)	
			iTime	-=	24;
		if	(iTime	>=	13)	
			iTime	-=	12;
	}

	if (_iCurrentTime >= 12*60	&&	_iCurrentTime < 24*60	)	//	pm
	{
		sprintf(_lpstrText,"<b>%2d:%.2d<b>",iTime,_iCurrentTime%60);

		if	(_bIsNoAmPm)
			return;

		if	(bIsWantPutAMPM)
			sprintf(_lpstrText,"%s <b>%2d:%.2d<b>",dMSG_PM_ICON,iTime,_iCurrentTime%60);
	}
	else
	{
		sprintf(_lpstrText,"<b>%2d:%.2d<b>",iTime,_iCurrentTime%60);

		if	(_bIsNoAmPm)
			return;

		if	(bIsWantPutAMPM)
			sprintf(_lpstrText,"%s <b>%2d:%.2d<b>",dMSG_AM_ICON,iTime,_iCurrentTime%60);
	}
}

void
CGuildBattleScehduler::draw()
{
	if	(s_iPopupInterface	!=	ePIW_GUILD_BATTLE_SCHEDULER)
		return;

	int		iX=g_iScreenWidth/3+12,iY=0,i;
	cRECT	rect;

	rect.set(g_iScreenWidth/3+12,6,g_iScreenWidth-6,g_iFieldScreenHeight-30);
	g_msgBox.drawFrame(&rect,FALSE,_ms(dMSG_GUILD_BATTLE_SCHEDULE_FORM,m_wYear,m_wMonth,m_wDay));

	iX	-=	5;

	s_text.put(iX+15,iY+45,WHITE,dMSG_BATTLE_TIME);
	cDRAW::XLine(RGB24To16(154,93,47),iX+5 ,iX+73 ,iY+60);

	s_text.put(iX+84,iY+45,WHITE,dMSG_NUMBER);
	s_text.cPut(iX+c_iGBS_Time,iY+45,WHITE,dMSG_TIME);
	s_text.cPut(iX+c_iGBS_FieldName,iY+45,WHITE,dMSG_FIELD_NAME);
	s_text.cPut(iX+c_iGBS_GuildName+256/2,iY+45,WHITE,dMSG_BATTLE_GUILD_NAME);

	cDRAW::YLine(RGB24To16(160,91,67),iY+39,iY+453,iX+76 ,2);
	cDRAW::XLine(RGB24To16(154,93,47),iX+80,iX+514,iY+60);

	int		iBeginTime	=	m_wBeginHour*60+m_wBeginMinute;
	BOOL	bIsAm		=	2;

	for (i=0;i<m_wBattleCount;i++)
	{
		DWORD	dwTextColor	=	LTGREEN;
		int		iCurrentTime=	m_wBeginHour*60+m_wBeginMinute+i*m_wBattlePeriod;
		char	strTimeText[64];

		if	(m_iSelectTime == i)
			g_sprInterface.Put(iX+5,iY+62+i*17,eGBSW_SELECT_TIME_BAR);
		else
		{
			if	(m_iFocusTime == i)
				g_sprInterface.Put(iX+5,iY+62+i*17,eGBSW_ACTIVE_TIME_BAR);
			else
				g_sprInterface.Put(iX+5,iY+62+i*17,eGBSW_NORMAL_TIME_BAR);
		}

		BOOL	bIsChangeAMPM	=	FALSE;

		if	(iCurrentTime	>=	24*60)
			iCurrentTime	-=	24*60;

		if	(iCurrentTime >= 12*60)	//	pm
		{
			if	(bIsAm	!=	FALSE)
			{
				bIsChangeAMPM	=	TRUE;
				bIsAm			=	FALSE;
			}

			if	(m_iSelectTime == i || m_iFocusTime == i)
				dwTextColor	=	WHITE;
			else
				dwTextColor	=	RGB(193,100,0);

			getTimeText(i,iCurrentTime,strTimeText,FALSE,bIsChangeAMPM);
	
			s_text.putRF(iX+62,iY+64+i*17,dwTextColor,strTimeText);
		}
		else
		{
			if	(bIsAm	!=	TRUE)
			{
				bIsChangeAMPM	=	TRUE;
				bIsAm			=	TRUE;
			}

			if	(m_iSelectTime == i || m_iFocusTime == i)
				dwTextColor	=	RGB(2,212,76);
			else
				dwTextColor	=	RGB(1,151,54);

			getTimeText(i,iCurrentTime,strTimeText,FALSE,bIsChangeAMPM);

			s_text.putRF(iX+62,iY+64+i*17,dwTextColor,strTimeText);
		}
	}

	int	iBarWidth	=	g_sprInterface.getSpriteWidth(eBUTTON_NORMAL);

	for (i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME;i++)
	{
		int		iCurrentTime=	m_wBeginHour*60+m_wBeginMinute+m_iSelectTime*m_wBattlePeriod+i/c_iSameTimeFiledCountInGuildBattle*c_iGBS_FieldOrerGapTime;

		if	(iCurrentTime	>=	24*60)
			iCurrentTime	-=	24*60;

//	인덱스
		g_sprInterface.Put(iX+97,iY+93+i*39,eGBSW_1+i);	//	1,2,3,4...

//	시간
		{
			char	strTimeText[64];

			getTimeText(m_iSelectTime,iCurrentTime,strTimeText,TRUE);

			s_text.cPut(iX+c_iGBS_Time,iY+78+i*39,WHITE,strTimeText);
		}

//	필드 이름
		{
			CMapSimpleInfo	*lpMapInfo	=	&g_aMapSimpleInfo[m_schedule.m_aBattleGuildInfo[i].m_wFieldSerial];

			if	(lpMapInfo->m_wSerial	!=	0xffff)
			{
				s_text.cPut(iX+c_iGBS_FieldName,iY+70+i*39,WHITE,"%s",lpMapInfo->m_strName);
				s_text.cPut(iX+c_iGBS_FieldName,iY+86+i*39,WHITE,"[%d x %d]",lpMapInfo->m_wWidth,lpMapInfo->m_wHeight);
			}
		}

//	대전 예약된 길드 이름들. 있다면 말이지...
		if	(g_hero.m_bf1IsWaitGuildCommandResult == TRUE)
			continue;
		
		CPos	posPut;
		BOOL	bIsMyBattle	=	FALSE;
		int		aiGuildSerial[2];

		aiGuildSerial[0]	=	m_schedule.m_aBattleGuildInfo[i].m_awBattleGuild[0];
		aiGuildSerial[1]	=	m_schedule.m_aBattleGuildInfo[i].m_awBattleGuild[1];

		posPut.x	=	iX+c_iGBS_GuildName+256/2;
		posPut.y	=	67;

		if	(aiGuildSerial[0]	==	g_hero.m_wGuildSerial || aiGuildSerial[1]	==	g_hero.m_wGuildSerial)
			bIsMyBattle	=	TRUE;

		if	(aiGuildSerial[0]	!=	0xffff)
		{
			if (bIsMyBattle)
			{
				if (aiGuildSerial[0]	==	g_hero.m_wGuildSerial)
					s_text.cPut(posPut.x,posPut.y+i*dGBI_FILED_LINE_GAB							,LTGREEN,m_schedule.m_aBattleGuildInfo[i].m_astrGuild[0]);
				else	
					s_text.cPut(posPut.x,posPut.y+i*dGBI_FILED_LINE_GAB							,LTRED	,m_schedule.m_aBattleGuildInfo[i].m_astrGuild[0]);
			}
			else
				s_text.cPut(posPut.x,posPut.y+i*dGBI_FILED_LINE_GAB								,WHITE,m_schedule.m_aBattleGuildInfo[i].m_astrGuild[0]);
		}
		else
		if	(g_guild.isBookedIGB())
			s_text.cPut(posPut.x,posPut.y+2+i*dGBI_FILED_LINE_GAB								,LTGRAY,dMSG_NOT_EXIST_BATTLE_GUILD);

		if	(aiGuildSerial[1]	!=	0xffff)
		{
			if	(bIsMyBattle)
			{
				if	(aiGuildSerial[1]	==	g_hero.m_wGuildSerial)
					s_text.cPut(posPut.x,posPut.y+i*dGBI_FILED_LINE_GAB+dGBI_FILED_SECOND_LINE_POS	,LTGREEN,m_schedule.m_aBattleGuildInfo[i].m_astrGuild[1]);
				else
					s_text.cPut(posPut.x,posPut.y+i*dGBI_FILED_LINE_GAB+dGBI_FILED_SECOND_LINE_POS	,LTRED,m_schedule.m_aBattleGuildInfo[i].m_astrGuild[1]);
			}
			else
				s_text.cPut(posPut.x,posPut.y+i*dGBI_FILED_LINE_GAB+dGBI_FILED_SECOND_LINE_POS	,WHITE,m_schedule.m_aBattleGuildInfo[i].m_astrGuild[1]);
		}
		else
		if	(g_guild.isBookedIGB()	&&	bIsMyBattle	==	FALSE)
			s_text.cPut(posPut.x,posPut.y+i*dGBI_FILED_LINE_GAB+dGBI_FILED_SECOND_LINE_POS	,LTGRAY,dMSG_NOT_EXIST_BATTLE_GUILD);
	}
	
	for (i=0;i<dGUILD_BATTLE_COUNT_IN_SAME_TIME-1;i++)
		cDRAW::XLine(RGB24To16(72,35,8),iX+80,iX+514,iY+101+i*dGBI_FILED_LINE_GAB);

	m_bmButton.draw();

	if	(g_guild.isBookedIGB()	||	g_guild.isEngagedIGB())
	{
		char						strText[256];
		cRECT						rect;
		CGuildBattleInfoForMember	*lpInfo			=	&g_guild.m_battleInfo;
		char*						lpstrFieldName	=	cMAP::GetFieldName(lpInfo->m_wIGB_FieldSerial);

		if	(g_guild.isEngagedIGB())
			sprintf(strText,dMSG_ENGAGED_GUILD_BATTLE_COMMENT_FORM,GetTimeStringHM(lpInfo->m_bIGB_Hour,lpInfo->m_bIGB_Minute),lpstrFieldName,lpInfo->m_strIGB_TargetGuildName);
		else
			sprintf(strText,dMSG_BOOKED_GUILD_BATTLE_COMMENT_FORM,GetTimeStringHM(lpInfo->m_bIGB_Hour,lpInfo->m_bIGB_Minute),lpstrFieldName);

		rect.set(10,40,g_iScreenWidth/3-10,40);

		int	iLineCount	=	s_text.getLineCount(rect.getWidth(),strText);
		int	iFontHeight	=	max(s_text.getFontHeight(),12);

		rect.y2			=	rect.y1+iLineCount*(iFontHeight+2)-2;

		s_ttCommon.drawBox(&rect,0,16,TRUE,TRUE);

		s_text.cPut(rect.x1,rect.y1,rect.getWidth(),100,0,WHITE,strText);
	}
}