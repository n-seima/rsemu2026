#include "cGAME_SELECT_AVATAR.H"
#include "CAgent.H"
#include "cJOB.H"
#include "CActor.H"
#include "CMessage.H"
#include "CSound.H"
#include "secretDungeonDefine.H"
#include "console.h"

#ifdef _USE_XTRAP
#include "XTrap/Xtrap_C_Interface.h"
#endif

#define	IDM_CREATE_AVATAR						1000
#define	IDM_REMOVE_AVATAR						1001

#define	IDM_COPY_CHARACTER_DATA_TO_TEST_SERVER	3000

#define	IDM_CHARACTER1							0
#define	IDM_CHARACTER2							1
#define	IDM_CHARACTER3							2
#define	IDM_CHARACTER4							3

#define	IDM_CANCEL_TO_CONNECT_GAME_SERVER		2000
#define	IDM_GAME_START							2001

#define	dAVATAR_HEIGHT							90
#define	dAVATAR_DEFAULT_HEIGHT					20

int		l_iRemainTime;
int		l_iSelectAvatar	=	0;
//
//	Select Avatar Screen Image
enum
{
	eSASI_SELECT_CHARACTER,

	eSASI_START_NORMAL,
	eSASI_START_DISABLE,
	eSASI_START_ACTIVE,
	eSASI_START_PRESSED,

	eSASI_CANCEL_NORMAL,
	eSASI_CANCEL_ACTIVE,
	eSASI_CANCEL_PRESSED,

	eSASI_REMOVE_NORMAL,
	eSASI_REMOVE_ACTIVE,
	eSASI_CREATE_NORMAL,
	eSASI_CREATE_ACTIVE,

	eSASI_BORDER,
};

int			cGAME_SELECT_AVATAR::s_iAvatarCount						=	0;
int			cGAME_SELECT_AVATAR::s_iModelFrame						=	0;
cSPRITE		cGAME_SELECT_AVATAR::s_sprSprite;
CActor		cGAME_SELECT_AVATAR::s_aModel[dMAX_AVATAR_COUNT];//¸ð¨.. -_-a
cANM		cGAME_SELECT_AVATAR::s_anmLight;

cGAME_UTIL::cLIGHT	cGAME_SELECT_AVATAR::s_aLight[dMAX_LIGHT];
BOOL		cGAME_SELECT_AVATAR::s_isWaitWorldServerConnectMessage	=	FALSE;	//	¼­¹ ³ØÆ® ¸Þ½ ±´¸®´ß
BOOL		cGAME_SELECT_AVATAR::s_bIsWaitAvatarList				=	TRUE;
int			cGAME_SELECT_AVATAR::s_iTimeOfConnectTry				=	FALSE;				//	³ØÆ® ½ ½°
char		cGAME_SELECT_AVATAR::s_astrAvatarField[dMAX_AVATAR_COUNT][256];

////////////////////////////////////////////////////////////////////////////////////////////////
//	¾÷Æ®
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cGAME_SELECT_AVATAR::UpdateInit()
{
	int		i;

	for (i=0;i<dMAX_AVATAR_COUNT;i++)
		s_astrAvatarField[i][0]	=	0;

	ReadLastPlayAvatarName(s_strLastID,s_strLastSelectAvatarName);	//	¸¶¸·¸· ·¹ ¾Æ¹¸ ¸§ ¾¾¿±

	strcpy(s_strSelectAvatarName,s_strLastSelectAvatarName);

	if	(s_bIsWaitAvatarList)
		if	(s_iPreMode	!=	eGAME_CREATE_AVATAR)
			memset(s_aAvatarList,0xff,sizeof(s_aAvatarList));

	l_iRemainTime						=	dSYNC_FPS*2;
	l_iSelectAvatar++;

	if	(l_iSelectAvatar	>=	4)
		l_iSelectAvatar	=	0;

	g_map.m_pos.x						=	0;
	g_map.m_pos.y						=	0;
	s_iModelFrame						=	0;
	s_isWaitWorldServerConnectMessage	=	FALSE;
	s_iTimeOfConnectTry					=	FALSE;
	s_iLoadedHeroImageJob				=	-1;

	s_iSelectAvatar						=	0xffff;

	for (i=0;i<dMAX_AVATAR_COUNT;i++)
		s_aModel[i].m_wSerial	=	0xffff;

//	¹  ·
	{
		s_mcuData.reset();

		char	strDirectory[512];

		GetCurrentDirectory(512,strDirectory);

		if	(!s_sprSprite.Load("data/interface/select_avatar.sd",FALSE,NULL,&s_mcuData))
			return	FALSE;
		if	(!s_anmLight.Load("data/interface/light.sad",FALSE,NULL,&s_mcuData))
			return	FALSE;
	}	//	¹  ·

	for (i=0;i<dMAX_LIGHT;i++)
	{
		int		iAnm	=	random(s_anmLight.m_iAnmCount);
		s_aLight[i].init(iAnm,s_anmLight.GetFrameCount(iAnm),-200,-200,g_iScreenWidth+200,g_iScreenHeight+200,0.002f);
	}

//	¹Æ°
	{
		s_bmMenu.init(&s_sprSprite,10,dBAR_BUTTON,eBTOM_BAR_RIGHT);

		s_bmMenu.addImageBar(dMSG_CREATE_CHARACTER,IDM_CREATE_AVATAR	,0  ,463,eSASI_CREATE_NORMAL,eSASI_CREATE_ACTIVE,eSASI_CREATE_ACTIVE,0xffff,0xffff);
		s_bmMenu.addImageBar(dMSG_REMOVE_CHARACTER,IDM_REMOVE_AVATAR	,20  ,526+g_iCorrectBI_Y,eSASI_REMOVE_NORMAL,eSASI_REMOVE_ACTIVE,eSASI_REMOVE_ACTIVE,0xffff,0xffff);
		s_bmMenu.addImageBar("",IDM_GAME_START		,534+g_iCorrectBI_X*9/10,462+g_iCorrectBI_Y,eSASI_START_NORMAL,eSASI_START_ACTIVE,eSASI_START_PRESSED,0xffff,eSASI_START_DISABLE);
		s_bmMenu.addImageBar("",eMBI_RETURN_TO_TITLE,534+g_iCorrectBI_X*9/10,525+g_iCorrectBI_Y,eSASI_CANCEL_NORMAL,eSASI_CANCEL_ACTIVE,eSASI_CANCEL_PRESSED,0xffff,0xffff);
		s_bmMenu.disable(IDM_GAME_START);

		s_bmMenu.setClickMargin(IDM_CREATE_AVATAR,0,1);
		s_bmMenu.setClickMargin(IDM_REMOVE_AVATAR,0,1);

		s_bmMenu.hide(IDM_CREATE_AVATAR);

		s_bmButton.init(&g_sprInterface,2,dBAR_BUTTON);
//		s_bmButton.addImageBar(dMSG_COPY_DATA_TO_TEST_SERVER,IDM_COPY_CHARACTER_DATA_TO_TEST_SERVER	,0  ,463,eBUTTON_NORMAL,eBUTTON_ACTIVE,eBUTTON_ACTIVE,0xffff,0xffff);

//		s_bmButton.hide(IDM_COPY_CHARACTER_DATA_TO_TEST_SERVER);
		s_bmButton.setClicktAllBarMargin(1,1);
		s_bmButton.setAllBarTextColor(0,FALSE);
	}

	BookingStep(dSTEP_READY,dFADE_IN);

	CGamePlay::PlayBgm("01 Title-Legend of Red Stone.ogg");
	s_timer.resetFrameSkip();

	if	(g_bIsDuelServer)
		g_sm.loadData("data/skill2.dat");
	else
#ifdef	_FOR_KOREA
		g_sm.loadData("data/skill.dat");
#else
		g_sm.loadData("data/skill2.dat");
#endif

	return	SAFETY;
}

BOOL
cGAME_SELECT_AVATAR::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

//
//	ÆÐ¶ ³¸®
BOOL
cGAME_SELECT_AVATAR::OperatePacket()
{
//	ÆÐ¶ ³¸®
	if	(s_agent.isDisconnected())
	{
		g_msgBox.cPopup(NULL,s_agent.m_strDisconnectText,dMSG_OK);
		g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
		s_agent.reset();

		return	FALSE;
	}

	if	(!s_agent.packetHandler())	
		return	FALSE;

//	·±× °°
	if	(s_agent.isReceivedPacket(dSL_RESULT_LOGIN))
	{
		switch(GetLogInResult())
		{
			case	dRESULT_LOGIN_SUCCESS			:	//	OK
			case	dRESULT_LOGIN_SUCCESS_PERSONALCOM:
				break;

			case	dRESULT_LOGIN_FAIL_PERSONALCOM	:
				g_msgBox.cPopup(NULL,dMSG_LOGIN_FAILED,dMSG_CANCEL);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				break;

			case	dRESULT_LOGIN_USEDID			:	//	¹ ¼ß ¾Æð´.
			case	dRESULT_LOGIN_FAILED			:	//	¾Æð°¡ ¸  ¾°³ª ÆÐ½º¿°¡ Æ²·´.
			case	dRESULT_LOGIN_RETRY				:	//	¼ ²÷¾¸´± ´½ ¼´.
				g_msgBox.cPopup(NULL,dMSG_LOGIN_FAILED,dMSG_CANCEL);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				break;

			case	dRESULT_LOGIN_NOT_SAME_VERSION	:
				g_msgBox.cPopup(NULL,dMSG_INCORRECT_CLIENT_VERSION,dMSG_OK);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				break;

			default									:
			{
				DWORD	dwDate	=	(DWORD) GetLogInResult();
				int		iHour	=	dwDate%100;
				int		iDay	=	dwDate%10000/100;
				int		iMonth	=	dwDate%1000000/10000;
				int		iYear	=	dwDate/1000000;
				char	strText[512];

				sprintf(strText,dMSG_ID_BLOCK_FORM,iYear,iMonth,iDay,iHour,s_strMessage);

				g_msgBox.cPopup(NULL,strText,dMSG_OK);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				break;
			}

		}
	}

//	¾Æ¹¸ ¼± °°
	if	(s_agent.isReceivedPacket(dSL_RESULT_SELECT_AVATAR))
	{
		switch(GetSelectAvatarResult())
		{
			case	eDLMSG_SELECTAVATAR_SUCCESS	:	//	¼± ¼º°ø
//				cMSG::Put("","¼± °° ¹Þ½");
				s_agent.reset();
				strcpy(s_strGameServerAddress,s_aAvatarList[s_iSelectAvatar].m_strIP);
				s_iTimeOfConnectTry	=	FALSE;
				s_iConnectType		=	eCONNECT_TYPE_FIRST;
				s_isEnterenceGame	=	TRUE;
				BookingMode(eGAME_CONNECT_GAME_SERVER,dSTEP_FINISH,dFADE_OUT);
				break;

			case	eDLMSG_EXIST_CONNECT_USER	:	//	´±º°¡ ¼Ø ´.
//				cMSG::Put("","¼± °° ¹® ½");
				g_msgBox.cPopup(NULL,dMSG_YET_REMAIN_CONNECT_USER_DATA,dMSG_OK);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				break;

			default								:	//	½ÆÐ
//				cMSG::Put("","¼± °° ¹® ½ÆÐ");
				g_msgBox.cPopup(NULL,dMSG_FAILED_TO_JOIN_GAME,dMSG_OK);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				break;
		}
	}
//	¾Æ¹¸ ° °°
	if	(s_agent.isReceivedPacket(dSL_RESULT_REMOVE_AVATAR))
	{
		switch(GetRemoveAvatarResult())
		{
			case	dRESULT_REMOVE_AVATAR_SUCESS			:	//	° ¼º°ø
				s_aAvatarList[s_iSelectAvatar].m_wJob	=	0xffff;
				s_iSelectAvatar							=	0xffff;
				UpdateAvatarList(TRUE);
				break;

			case	dRESULT_REMOVE_AVATAR_FAILED			:	//	½ÆÐ
				g_msgBox.cPopup(NULL,dMSG_FAILED_TO_REMOVE_AVATAR,dMSG_OK);
				break;
		}
	}

//	¾Æ¹¸ ¸®½ºÆ®°¡ øß´.
	if	(s_agent.isReceivedPacket(dSL_AVATAR_LIST))
		UpdateAvatarList(TRUE);

	return	FALSE;
}

//
//	¸Þ ¾÷Æ®
BOOL
cGAME_SELECT_AVATAR::OperateMessageBox()
{
//	¸Þ½ ¹½º ³¸®
	if (!s_isOperatedMsgbox)
		return	FALSE;

	if	(g_msgBox.isNothing())
		return	TRUE;

	switch(g_msgBox.getResult())
	{
		case	IDM_REMOVE_AVATAR	:
			s_agent.sendRemoveAvatar(s_aAvatarList[s_iSelectAvatar].m_strName);
			g_msgBox.cPopup(NULL,dMSG_REMOVE_SELECT_AVATAR,dMSG_CANCEL);
			g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
			g_msgBox.setWaitPacket(dSL_RESULT_REMOVE_AVATAR);
			break;

		case	eMBI_RETURN_TO_TITLE			:		//	·±× ­¸¸· ¹¾Æ°¡±
		case	eMBI_RETURN_TO_TITLE_BY_CANCEL	:
			s_agent.reset();
			BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
			break;
	}

	return	FALSE;
}

//
//	¸Ð½Ð±Ð
void
cGAME_SELECT_AVATAR::UpdateAvatarList(BOOL _isCheck)
{
	s_iAvatarCount	=	0;

	for (int i=0;i<dMAX_AVATAR_COUNT;i++)
	{
		if (s_aAvatarList[i].m_wJob	==	0xffff)	
		{
			s_aModel[i].m_wSerial	=	0xffff;
			continue;
		}

		if (s_astrAvatarField[i][0]	==	0)
		{
			char	*lpstrFieldName;
			
			if	(g_bIsDuelServer	&&	s_aAvatarList[i].m_wLastField	<	c_iDuelServerLobyIndex)
				lpstrFieldName	=	dMSG_DUEL_FIELD;
			else
			if	(s_aAvatarList[i].m_wLastField	>=	dINSTANCE_FIELD_FLAG)
				lpstrFieldName	=	dMSG_SECRET_DUNGEON;
			else
				lpstrFieldName	=	cMAP::GetFieldName(s_aAvatarList[i].m_wLastField);

			if (lpstrFieldName)
				strcpy(s_astrAvatarField[i],lpstrFieldName);
		}

		if	(s_aModel[i].m_wSerial	==	0xffff)
			if	(s_aModel[i].setDataByAvatar(&s_aAvatarList[i])	==	FALSE)
				s_aModel[i].m_wJob	=	0xffff;	//	

		s_iAvatarCount++;
	}

	if (s_iAvatarCount	==	0)
		s_iSelectAvatar	=	0xffff;

	if (_isCheck)
		for (int i=0;i<dMAX_AVATAR_COUNT;i++)
			if	(s_aAvatarList[i].m_wJob	!=	0xffff	&&	s_aModel[i].m_wJob	!=	0xffff)
				if (STRICMP(s_strSelectAvatarName,s_aAvatarList[i].m_strName)	==	0)
				{
					s_iSelectAvatar	=	i;
					s_aModel[i].stop();
					break;
				}

	if (s_iSelectAvatar	==	0xffff)
		for (int i=0;i<dMAX_AVATAR_COUNT;i++)
			if (s_aAvatarList[i].m_wJob	!=	0xffff	&&	s_aModel[i].m_wJob	!=	0xffff)
			{
				s_iSelectAvatar	=	i;
				s_aModel[i].stop();
				break;
			}

	if (s_iSelectAvatar	==	0xffff)
	{	
		s_bmMenu.disable(IDM_GAME_START);
		s_bmMenu.hide(IDM_REMOVE_AVATAR);
	}
	else
	{
		s_bmMenu.enable(IDM_GAME_START);
		s_bmMenu.show(IDM_REMOVE_AVATAR);
	}

	if (s_isOperatedMsgbox)	return;

	CPos	pos;

	pos.Set(570+g_iCorrectBI_X*9/10,100);

	for (i=0;i<dMAX_AVATAR_COUNT;i++)
	{

		cAVATAR_INFO	*lpAvatar	=	&s_aAvatarList[i];
		cRECT	rectCrash;

		if (lpAvatar->m_wJob	==	0xffff)	continue;

		int iHeight = dAVATAR_DEFAULT_HEIGHT;

		if (i	==	s_iSelectAvatar)
			iHeight	=	dAVATAR_HEIGHT;

		rectCrash.set(pos.x-20,pos.y,g_iScreenWidth,pos.y+iHeight);
		if (s_isTouchLeftButton)
		{
			if (rectCrash.isIn(s_posMouse.x,s_posMouse.y))
			{
				s_iSelectAvatar	=	i;
				s_aModel[i].stop();
			}
		}
		pos.y	+=	iHeight;
	}


	//JBC °·¸· ¸ Æ÷¿½º º°æ °¡´° ¼¤ 08-08-01
	if(s_aClicked[KEY_DOWN])
	{			
		if(s_iSelectAvatar == 0xffff)		//Æ÷¿½º°¡ ´· ¾°¡¶§.
			s_iSelectAvatar = 0;
		else		//Æ÷¿½º°¡ ´· °¡¶§.
		{
			int		iSaveSelectAvatar = s_iSelectAvatar;
			BOOL	bMove = FALSE;
			for(int i = s_iSelectAvatar + 1; i < dMAX_AVATAR_COUNT ; i++)
			{
				if(s_aAvatarList[i].m_wJob == 0xffff)
					continue;

				bMove = TRUE;
				s_iSelectAvatar = i;
				break;
			}
		}
	}

	if(s_aClicked[KEY_UP])
	{	
		if(s_iSelectAvatar == 0xffff)
			s_iSelectAvatar = 0;
		else 
		{
			int		iSaveSelectAvatar = s_iSelectAvatar;
			BOOL	bMove = FALSE;
			for(int i = s_iSelectAvatar - 1; i >= 0 ; i--)
			{
				if(s_aAvatarList[i].m_wJob == 0xffff)
					continue;

				bMove = TRUE;
				s_iSelectAvatar = i;
				break;
			}
		}
	}

	if (s_iAvatarCount	>=	dMAX_AVATAR_COUNT	||	!s_agent.isActive()	||	s_bIsWaitAvatarList || g_iWorldServerType	==	eSERVER_TYPE_GVG)
		s_bmMenu.hide(IDM_CREATE_AVATAR);
	else
	{
		int iY = s_iAvatarCount*dAVATAR_DEFAULT_HEIGHT+110;

		if(s_iSelectAvatar != 0xffff)
			iY+= dAVATAR_HEIGHT-dAVATAR_DEFAULT_HEIGHT;

		s_bmMenu.correctPos(IDM_CREATE_AVATAR,pos.x-20,iY);
		s_bmMenu.show(IDM_CREATE_AVATAR);
	}
}


//
//	¸ð¨-_- ¾÷Æ®
void
cGAME_SELECT_AVATAR::UpdateModel()
{
	if (s_iAvatarCount	<=	0)	return;

	s_iModelFrame++;

	for (int i=0;i<dMAX_AVATAR_COUNT;i++)
	{
		if	(s_aModel[i].m_wSerial	==	0xffff)
			continue;
		if	(i	!=	s_iSelectAvatar)
			s_aModel[i].setAnm(dACT_SITDOWN);

		s_aModel[i].updateForModel();
	}

	if	(s_iSelectAvatar	==	0xffff)
		return;

	if	((s_iModelFrame	%(2*dSYNC_FPS))	==	0)
	{
		int	aAction[7]	=	{dACT_WALK,dACT_READY,dACT_RUN,dACT_ACTION_1,dACT_ACTION_2,dACT_ACTION_3,dACT_ACTION_4};
		int	iAction		=	aAction[random(7)];

		if	(s_aModel[s_iSelectAvatar].m_wJob		==	dJOB_ROGUE)
			iAction		=	random(5)+8;
		if	(s_aModel[s_iSelectAvatar].m_wJob		==	dJOB_FIGHTER)
			iAction		=	random(11)+13;

		cANM	*lpBody	=	s_aModel[s_iSelectAvatar].getBody();

		if	(iAction	>=	lpBody->m_iAnmCount)
			iAction	=	lpBody->m_iAnmCount-1;

		if (!s_aModel[s_iSelectAvatar].isExclusiveAction())
		{
			s_aModel[s_iSelectAvatar].setAnm(iAction);

			if (iAction	>=	dACT_ACTION_1)
				s_aModel[s_iSelectAvatar].setExclusiveAction(TRUE);
		}
	}
}



//
//	º-o- ¾÷Æ®
void
cGAME_SELECT_AVATAR::UpdateLight()
{
	for (int i=0;i<dMAX_LIGHT;i++)
	{
		s_aLight[i].updateSpline();
		s_aLight[i].updateFrame();
	}
}

void
cGAME_SELECT_AVATAR::CopySelectAvatarDataToDuelServer()
{
	s_agent.sendCopySelectAvatarDataToDuelServer(&s_aAvatarList[s_iSelectAvatar],s_iSelectServer);
}

//
//	¾Æ¹¸ ¸ ×½ºÆ® ¼­¹¿¡ º¹
void
cGAME_SELECT_AVATAR::CopySelectAvatarDataToTestServer()
{
	s_agent.sendCopySelectAvatarDataToTestServer(&s_aAvatarList[s_iSelectAvatar]);
}

//
//	¸Þ ¾÷Æ®
BOOL
cGAME_SELECT_AVATAR::UpdateMain()
{
	UpdateAvatarList();
	OperatePacket();
	UpdateLight();
	UpdateModel();

	if	(OperateMessageBox())
		return	TRUE;

	DWORD	dwSelectMenu	=	s_bmMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

	//JBC ¿¸ ¡¸ Æ÷¿½º¸ ¹Þ° ´ ¸ ¼±¾ ³¾°´. 08-07-01
	if(s_aClicked[KEY_RETURN] || s_aClicked[KEY_NUMPADENTER])
		dwSelectMenu = IDM_GAME_START;

//	l_iRemainTime--;

	if (l_iRemainTime<=0)
	{
		l_iRemainTime	=	1000000;
		dwSelectMenu	=	IDM_GAME_START;
		s_iSelectAvatar	=	l_iSelectAvatar;
	}

	switch(dwSelectMenu)
	{
		case	IDM_GAME_START		:
		{
			if	(s_iSelectAvatar	==	0xffff)	
				break;

			if	(STRICMP(s_aAvatarList[s_iSelectAvatar].m_strIP,"")==0)
			{
				g_msgBox.cPopup(NULL,dMSG_GAME_SERVER_NOT_OPEND,dMSG_CANCEL);
				break;
			}

#ifdef _USE_XTRAP
			XTrap_C_SetUserInfoEx(s_strLastID,s_strLastSelectServerName,
									s_aAvatarList[s_iSelectAvatar].m_strName,
									g_pstrHeroJobName[s_aAvatarList[s_iSelectAvatar].m_wJob],
									0,NULL);
#endif

			strcpy(s_strSelectAvatarName,s_aAvatarList[s_iSelectAvatar].m_strName);
			strcpy(s_strLastSelectAvatarName,s_strSelectAvatarName);

			s_agent.setPlayerName(s_strLastID,s_strSelectAvatarName);
			s_agent.sendSelectAvatar(s_strSelectAvatarName);

			g_msgBox.cPopup(NULL,dMSG_WAIT_SERVER_MESSAGE,dMSG_CANCEL);
			g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
			g_msgBox.setWaitPacket(dSL_RESULT_SELECT_AVATAR);

			break;
		}

		case	eMBI_RETURN_TO_TITLE	:		//	·±× ­¸¸· ¹¾Æ°¡±
		case	eMBI_RETURN_TO_TITLE_BY_CANCEL	:
			s_agent.reset();
			BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
			break;

		case	IDM_CREATE_AVATAR	:
			BookingMode(eGAME_CREATE_AVATAR,dSTEP_FINISH,dFADE_OUT);
			break;

		case	IDM_REMOVE_AVATAR	:
			if (s_iSelectAvatar	==	0xffff)	break;

			g_msgBox.cPopup(NULL,_ms(dMSG_DO_YOU_REMOVE_SELECT_AVATAR,s_aAvatarList[s_iSelectAvatar].m_strName),dMSG_YES,dMSG_NO);
			g_msgBox.setButtonId(IDM_REMOVE_AVATAR);
			break;
	}

	dwSelectMenu	=	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

	switch(dwSelectMenu)
	{
		case	IDM_COPY_CHARACTER_DATA_TO_TEST_SERVER	:	//	³¸  º¹
			CopySelectAvatarDataToTestServer();
			break;
	}

	return	SAFETY;
}

BOOL
cGAME_SELECT_AVATAR::UpdateFinish()
{
	if (g_canvas.isCompleteFadeOut())	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

//
//
BOOL
cGAME_SELECT_AVATAR::UpdateClose()
{
	if (s_iBookingMode	!=	eGAME_CREATE_AVATAR)	s_bIsWaitAvatarList	=	TRUE;

	s_sprSprite.close();
	s_anmLight.Reset();
	s_anmHero.Reset();
	s_anmHero2.Reset();

	return	SAFETY;
}

//
//	Run!!!
BOOL
cGAME_SELECT_AVATAR::Run()
{
	switch(s_iStep)
	{
		case	dSTEP_INIT					:	return	UpdateInit();
		case	dSTEP_READY					:	return	UpdateReady();
		case	dSTEP_MAIN					:	return	UpdateMain();
		case	dSTEP_FINISH				:	return	UpdateFinish();
		case	dSTEP_CLOSE					:	return	UpdateClose();
	}

	return	FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	­¸¿¡ ·
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_SELECT_AVATAR::DrawInit()
{
	DrawMain();
}

void
cGAME_SELECT_AVATAR::DrawReady()
{
	DrawMain();
}

//
//­³½Ð±.
void
cGAME_SELECT_AVATAR::DrawAvatarList(int _iDx,int _iDy)
{

	CPos	pos;

	pos.Set(565-_iDx+g_iCorrectBI_X*9/10,100);

//	s_bmButton.hide(IDM_COPY_CHARACTER_DATA_TO_TEST_SERVER);


	for (int i=0;i<dMAX_AVATAR_COUNT;i++)
	{
		cAVATAR_INFO	*lpAvatar	=	&s_aAvatarList[i];


		if	(lpAvatar->m_wJob	==	0xffff)
			continue;


		if (i	==	s_iSelectAvatar)
		{
			if	(s_aModel[i].m_wJob	==	0xffff)
			{
				s_text.put(pos.x+35,pos.y-42   ,WHITE,"%s <c:LTYELLOW>%s<n>",dMSG_NAME,s_aAvatarList[i].m_strName);
				s_text.put(pos.x+35,pos.y-42+16,g_iScreenWidth-(570+35+30)-5,g_iScreenHeight,0,WHITE,dMSG_INCORRECT_AVATAR_DATA_PLEASE_CONTACT_TO_US);
				pos.y += dAVATAR_HEIGHT;
				continue;
			}
		
			int	iReviseWidthPos	=	0;

			if	(g_bIsTestServer == FALSE && g_bIsDuelServer == FALSE && g_iWorldServerType	!=	eSERVER_TYPE_GVG)
			{
//				s_bmButton.show(IDM_COPY_CHARACTER_DATA_TO_TEST_SERVER);
//				s_bmButton.correctPos(IDM_COPY_CHARACTER_DATA_TO_TEST_SERVER,pos.x+35,pos.y-65);
			}

			int	iDy	=	0;

			if	(s_aAvatarList[i].m_wJob/2	==	0)
				iDy	=	10;

			s_aModel[i].putForModel(pos.x,pos.y-iDy+dAVATAR_HEIGHT-20,TRUE);
			int iAddHeight = 8;
			int iAddWidth	= 35;
			int iY			=	pos.y + 20;
			s_text.put(pos.x +iAddWidth,iY   ,WHITE,"%s <c:LTYELLOW>%s<n>",dMSG_NAME,s_aAvatarList[i].m_strName);
			iAddHeight += iAddHeight;
			s_text.put(pos.x +iAddWidth, iY+iAddHeight,WHITE,"%s <c:LTYELLOW>%s(Level %d)<n>",dMSG_JOB,g_pstrHeroJobName[s_aAvatarList[i].m_wJob],s_aAvatarList[i].m_wLevel);

			if	(s_astrAvatarField[i][0])
			{
				iAddHeight += iAddHeight;
				s_text.put(pos.x+iAddWidth , iY+iAddHeight,WHITE,dMSG_POSITION);
				
				int	iWidth	=	s_text.getPixelSize(dMSG_POSITION);
				iAddWidth += iWidth;
				s_text.put(pos.x +iAddWidth, iY+ iAddHeight,g_iScreenWidth-(570+35+30)-5,g_iScreenHeight,0,WHITE,"<c:LTYELLOW>%s<n>",s_astrAvatarField[i]);
			}
			pos.y += dAVATAR_HEIGHT;
			continue;
		}
		else
		{
			cRECT rectCrash;
			rectCrash.set(pos.x-20,pos.y,g_iScreenWidth,pos.y);
			rectCrash.y2 += dAVATAR_DEFAULT_HEIGHT;

			int iColor = LTGRAY;
			if (rectCrash.isIn(s_posMouse.x,s_posMouse.y))
			{
				iColor = LTYELLOW;
			}

			if	(s_aModel[i].m_wJob	==	0xffff)
			{
				s_text.put(pos.x+35,pos.y-55   ,LTGRAY,"%s %s",dMSG_NAME,s_aAvatarList[i].m_strName);
				s_text.put(pos.x+35,pos.y-42+16,g_iScreenWidth-(570+35+30)-5,g_iScreenHeight,0,WHITE,dMSG_INCORRECT_AVATAR_DATA_PLEASE_CONTACT_TO_US);
				pos.y += dAVATAR_DEFAULT_HEIGHT;
				continue;
			}
			cANM	*lpIcon			=	&g_aJobIcon[s_aModel[i].m_wJob];

			int	iFPS		=	lpIcon->GetFPS(0);
			int	iFrameCount	=	lpIcon->GetFrameCount(0);
			lpIcon->PutReg(pos.x,pos.y-2,0,0,((s_iFrameCounter+pos.y)*iFPS/dSYNC_FPS)%iFrameCount);			
			s_text.put(pos.x+35,pos.y   ,iColor,"%s(Level %d)",s_aAvatarList[i].m_strName,s_aAvatarList[i].m_wLevel);
			pos.y += dAVATAR_DEFAULT_HEIGHT;
		}


	}

	if	(s_iAvatarCount	<=	0		)
		return;
	if	(s_iSelectAvatar	==	0xffff	||	s_aModel[s_iSelectAvatar].m_wJob	==	0xffff)
		return;
	
	DrawHeroPicture(0,0,s_aAvatarList[s_iSelectAvatar].m_wJob);
}




//
//	º-o- ±×·±..
void
cGAME_SELECT_AVATAR::DrawLight()
{
	for (int i=0;i<dMAX_LIGHT;i++)
		cDRAW::Fill(_LTYELLOW,	s_aLight[i].m_spline.m_pos.x,s_aLight[i].m_spline.m_pos.y,
								s_aLight[i].m_spline.m_pos.x+2,s_aLight[i].m_spline.m_pos.y+2);
	//		s_anmLight.PutReg(s_aLight[i].m_spline.m_pos.x,s_aLight[i].m_spline.m_pos.y,s_aLight[i].m_iAnm,0,s_aLight[i].m_iFrame);
}

//
//	±×·±.. ¸Þ ·-_-?
void
cGAME_SELECT_AVATAR::DrawMain()
{
	int		iDeltaWidth;
	cRECT	rectCreateButton;
	
	cDRAW::Clear();
	
	DrawLight();
	
	iDeltaWidth	=	(200)*(32-g_canvas.getFadeScreenValue())/32;
	DrawAvatarList(-iDeltaWidth,0);
	
	s_sprSprite.Put(553+iDeltaWidth+g_iCorrectBI_X*9/10,4,eSASI_SELECT_CHARACTER);
	s_bmButton.draw();
	
	iDeltaWidth	=	(g_iScreenWidth - 558+g_iCorrectBI_X*9/10)*(32-g_canvas.getFadeScreenValue())/32;
	
	//	s_sprSprite.Put(558+iDeltaWidth,67,34);
	
	s_bmMenu.getRect(IDM_CREATE_AVATAR,&rectCreateButton);
	iDeltaWidth	=	(g_iScreenWidth - rectCreateButton.x2)*(32-g_canvas.getFadeScreenValue())/32;
	
	s_bmMenu.setCorrectPos(IDM_GAME_START		,iDeltaWidth,0);
	s_bmMenu.setCorrectPos(eMBI_RETURN_TO_TITLE	,iDeltaWidth,0);
	s_bmMenu.setCorrectPos(IDM_CREATE_AVATAR	,iDeltaWidth,0);
	s_bmMenu.setCorrectPos(IDM_REMOVE_AVATAR	,-iDeltaWidth,0);
	
	s_bmMenu.draw();
	
	if	(s_bIsWaitAvatarList)
		if	(timeGetTime()%2000 > 700)
		{
			s_textLarge32.setShadowMargin(2);
			s_textLarge32.cPut(cDRAW::WIDTH/2,cDRAW::HEIGHT/3,WHITE,dMSG_WAIT_AVATAR_LIST);
		}
}

void
cGAME_SELECT_AVATAR::DrawFinish()
{
	DrawMain();
}

void
cGAME_SELECT_AVATAR::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_SELECT_AVATAR::Draw()
{
	switch(s_iStep)
	{
	case	dSTEP_INIT		:DrawInit();break;
	case	dSTEP_READY		:DrawReady();break;
	case	dSTEP_MAIN		:DrawMain();break;
	case	dSTEP_FINISH	:DrawFinish();break;
	case	dSTEP_CLOSE		:DrawClose();break;
	}
	
	return	TRUE;
}