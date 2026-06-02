int			l_iVersionControl							=	1324;

#include "cGAME_TITLE.H"
#include "cJOB.H"
#include "CActor.H"
#include "CAgent.H"
#include "CSound.H"
#include "CGamePlayInterface.H"
#include "CGamePlay.H"
#include "kls.H"
#include "CMessage.H"
#include "CWindowInterface.h"
#include "itemSet.h"
#include "console.h"
#include "mess_sign.h"
#include "CLoadTextCode.h"
#include "version.h"

#ifdef _USE_XTRAP
#include "XTrap/Xtrap_C_Interface.h"
#endif

char		cGAME_TITLE::s_strID[dID_LENGTH];
char		cGAME_TITLE::s_strPass[dPASS_LENGTH];
char		cGAME_TITLE::s_strFocusServerName[dNAME_LENGTH];
BOOL		cGAME_TITLE::s_isWaitWorldServerConnectMessage	=	FALSE;
int			cGAME_TITLE::s_iTimeOfConnectTry	=	0;//연결을 시도한 시간
cSPRITE		cGAME_TITLE::s_sprSprite;
cSPRITE		cGAME_TITLE::s_sprTitle;
WORD		*cGAME_TITLE::s_pPannel;
DWORD		cGAME_TITLE::s_dwWaitForSafeDisconnectTime	=	0;
cRECT		cGAME_TITLE::s_rectServerListBorder;
cRECT		cGAME_TITLE::s_rectFocusWorld;
int			cGAME_TITLE::s_iFocusServer					=	-1;

enum
{
	eCSWI_CARROT	=	33,
	eCSWI_TITLE		=	67,
	eCSWI_FIRST_BAR	=	72,
};

#ifndef	_DEBUG
BOOL		l_bIsUseKls	=	TRUE;
#else
BOOL		l_bIsUseKls	=	TRUE;
#endif

cSPLINE		l_spline;

#define	dPBM_LOGIN_DISCONNECT_CONNECT_ID	1000
#define	dPBM_LOGIN_CANCEL					1001
#define	dPBM_LOGIN_SUCCESS_CHANGE_PASSWORD	1002
#define	dPBM_LOGIN_MOTP_CAMPAIGN_YES		1004
#define	dPBM_LOGIN_MOTP_CAMPAIGN_NO			1005
#define	dPBM_USE_PERSCOM_GUARD_CAMPAIGN_YES	1006
#define	dPBM_USE_PERSCOM_GUARD_CAMPAIGN_NO	1007

#define	dLOGIN_MBB_CANCEL_CONNECT			1003
#define	dSAFE_DISCONNECT_TIME				15

#define	dTM_LOGIN			1000	//	로그인
#define	dTM_CREATE_ID		1001	//	로그인
#define	dTM_OPTION			1002	//	옵션
#define	dTM_CREDIT			1003	//	크레딧
#define	dTM_EXIT			1004	//	종료
#define	dTM_REMEMBER_ID		1005	//	종료

//
//	Login Screen Image
enum
{
	eLSI_LOGIN,

	eLSI_OK_NORMAL,
	eLSI_OK_DISABLE,
	eLSI_OK_ACTIVE,
	eLSI_OK_PRESSED,

	eLSI_NEW_NORMAL,
	eLSI_NEW_ACTIVE,
	eLSI_NEW_PRESSED,

	eLSI_EXIT_NORMAL,
	eLSI_EXIT_ACTIVE,
	eLSI_EXIT_PRESSED,

	eLSI_BAR_NORMAL,
	eLSI_BAR_ACTIVE,
	eLSI_BAR_CHECK,

	eLSI_BAR_12,

};

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
cGAME_TITLE::UpdateInit()
{

#ifdef	_DEBUG
	if	(IsFile("console.txt"))
		OpenColsole("red stone console");	//	콘솔 열기
#endif
	CGame::s_bIsFirstLogin = FALSE;

	cINPDEV::Destroy();	//	다이렉트 인풋 초기화

	l_spline.init(0,0,g_iScreenWidth-200,g_iScreenHeight-1,0.01f);
	l_spline.update();
	s_agent.reset();
	
	//
	s_unionPartyInfo.reset();

	if	(s_iCurrentLoginServer	>=	s_iLoginServerLocalCount)
		s_iCurrentLoginServer	=	0;

	memcpy(s_strLoginServerAddress,s_aLoginServerIPList[s_iCurrentLoginServer].m_strIP,64);

	CGamePlay::ChangeCursorShape(s_hNormalCursor);

/*	WORD	awBuffer[34*34];

	cDRAW::Active(awBuffer,34,34);

	for (int i=0;i<dBASIC_ITEM_COUNT;i++)
	{
		if (g_aBasicItem[i].m_iSerial >= 2048 || g_aBasicItem[i].m_iSerial < 0)
			continue;

		g_smiIconItem.put(g_aBasicItem[i].m_wIconShape);

		char	strFileName[256];

		sprintf(strFileName,"[%.4d] %s.tga",i,g_aBasicItem[i].m_strName);
		cTGA::Save(strFileName,awBuffer,34,34,TRUE);
	}
*/
#ifndef	_USE_HS
	if	(g_bIsPortalUser == FALSE)
		if	(l_bIsUseKls)	
			UnsetShield();
#endif

//	이미지 데이터 로딩
	{
		s_mcuData.reset();

		if (!s_sprSprite.Load("data/interface/title.sd",FALSE,NULL,&s_mcuData))		return	FALSE;
		if (!s_sprTitle.Load("data/interface/titleAnm.sd",FALSE,NULL,&s_mcuData))	return	FALSE;

		int	iWdith	=	g_iScreenWidth;

		if	(iWdith%4!=0)
			iWdith	=	iWdith/4*4+4;

		s_pPannel	=	_New(WORD,iWdith*g_iScreenHeight,&s_mcuData,"pannel");

		cDRAW::Active(s_pPannel,iWdith,g_iScreenHeight);
		cDRAW::Clear();
		s_sprTitle.Put(0,0,0);
	}

	strcpy(s_strSelectServerName,"");

	int	x	=	10;
	int	y	=	10;
	int	ys	=	24;

	y		=	480;
	x		=	g_iScreenWidth/2 - 40;

//	버튼
	s_bmMenu.init(&s_sprSprite,10,FALSE);

#ifdef	_OGP_SERVICE
		s_bmMenu.addImageBar("",dTM_LOGIN		,534+g_iCorrectBI_X,461+g_iCorrectBI_Y,eLSI_OK_NORMAL		,eLSI_OK_ACTIVE		,eLSI_OK_PRESSED	,0xffff,eLSI_OK_DISABLE);
#else
	if	(g_bIsPortalUser)
		s_bmMenu.addImageBar("",dTM_LOGIN		,534+g_iCorrectBI_X,461+g_iCorrectBI_Y,eLSI_OK_NORMAL		,eLSI_OK_ACTIVE		,eLSI_OK_PRESSED	,0xffff,eLSI_OK_DISABLE);
	else
	{
		s_bmMenu.addImageBar("",dTM_LOGIN		,534+g_iCorrectBI_X,398+g_iCorrectBI_Y,eLSI_OK_NORMAL		,eLSI_OK_ACTIVE		,eLSI_OK_PRESSED	,0xffff,eLSI_OK_DISABLE);
		s_bmMenu.addImageBar("",dTM_CREATE_ID	,534+g_iCorrectBI_X,461+g_iCorrectBI_Y,eLSI_NEW_NORMAL	,eLSI_NEW_ACTIVE	,eLSI_NEW_PRESSED);
	}
#endif
	
	s_bmMenu.addImageBar("",dTM_EXIT		,534+g_iCorrectBI_X,524+g_iCorrectBI_Y,eLSI_EXIT_NORMAL	,eLSI_EXIT_ACTIVE	,eLSI_EXIT_PRESSED);
	s_bmMenu.setKey(dTM_LOGIN,0x0D);
	s_bmMenu.disable(dTM_LOGIN);

	s_bmCheckBox.init(&g_sprInterface,20,dBAR_CHECK,eBTOM_BAR_RIGHT);

	int	iSize	=	s_text.getPixelSize(dMSG_REMEMBER_ID);
//	s_bmCheckBox.addImageBar(dMSG_REMEMBER_ID	,dTM_REMEMBER_ID,g_iScreenWidth-iSize-40,294+g_iCorrectBI_Y,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);

	if	(g_config.m_bf1IsRememberId	)	
		s_bmCheckBox.check(dTM_REMEMBER_ID);

//	입력 박스 아이디/패스워드
#ifndef	_OGP_SERVICE
	if (g_bIsPortalUser == FALSE)
	{
		{
			int	iSize	=	s_text.getPixelSize(dMSG_REMEMBER_ID);
			s_bmCheckBox.addImageBar(dMSG_REMEMBER_ID	,dTM_REMEMBER_ID,g_iScreenWidth-iSize-40,294+g_iCorrectBI_Y,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX,eSMWI_CHECK_BOX_CHECKED);

			if	(g_config.m_bf1IsRememberId	)	
				s_bmCheckBox.check(dTM_REMEMBER_ID);
		}

		int		iX	=	534+64+10+g_iCorrectBI_X;

		s_inputBox.init(s_hWnd,s_hInst,&s_text,3);
#ifdef	_FOR_INTER
		s_inputBox.set(iX+4,326+g_iCorrectBI_Y,0,iX,326+g_iCorrectBI_Y,180,13,dID_LENGTH-2,s_strLastID,ES_AUTOHSCROLL);
#else
		s_inputBox.set(iX+4,326+g_iCorrectBI_Y,0,iX,326+g_iCorrectBI_Y,180,13,dID_LENGTH-2,s_strLastID);
#endif
		s_inputBox.set(iX+4,352+g_iCorrectBI_Y,1,iX,352+g_iCorrectBI_Y,180,12,dPASS_LENGTH-2,"",ES_PASSWORD);

		s_inputBox.setColor(WHITE,0);
		s_inputBox.setHideCtrl(2,TRUE);

		//JBC 아이디 체크 창에 기억하기가 체크 되어있으면 포커스를 페스워드로 이동. 08-07-01
//		if	(g_config.m_bf1IsRememberId	)	
//			s_inputBox.active(1,TRUE);
//		else
			s_inputBox.active(0,TRUE); 
		
		s_inputBox.disable();
	}
#endif
	
	BookingStep(dSTEP_READY,dFADE_IN);

	s_timer.resetFrameSkip();

	return	SAFETY;
}

//
//
BOOL
cGAME_TITLE::UpdateReady()
{
	UpdateInit();
	CGamePlay::PlayBgm("01 Title-Legend of Red Stone.ogg");
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

//
//	서버 연결 메시지를 기다림
BOOL
cGAME_TITLE::WaitConnectMessage()
{
	if	(!s_isWaitWorldServerConnectMessage)
		return	FALSE;

	int	iConnectResult	=	s_agent.getConnectServerResult();

	switch(iConnectResult)
	{
		case	dCONNECT_STATUS_CONNECT_OK		:	//	접속 완료. 로그인 메시지 날리자
		{
			s_agent.sendClientType(eCT_GAME);
			s_isWaitWorldServerConnectMessage	=	FALSE;
			break;
		}

		case	dCONNECT_STATUS_CONNECT_FAILED	:
		case	dCONNECT_STATUS_TIME_OVER		:
			s_isWaitWorldServerConnectMessage	=	FALSE;
			s_iTimeOfConnectTry	=	timeGetTime();
			s_agent.disconnect();	//	연결을 끊고..
			return	TRUE;

		case	dCONNECT_STATUS_TRYING			:
			return	TRUE;
	}

	return	FALSE;
}

//
//	 메시지 박스 처리
BOOL
cGAME_TITLE::OperateMessageBox()
{
//	메시지 박스 메시지 처리
	if	(!s_isOperatedMsgbox)
		return	FALSE;
	if	(g_msgBox.isNothing())
		return	TRUE;

	switch(g_msgBox.getResult())
	{
		case	dPBM_LOGIN_MOTP_CAMPAIGN_YES	:
			s_agent.disconnect();	//	연결을 끊고..
			s_inputBox.setStr(1,"");
			s_inputBox.active(0);
			UnsetShield();
			
			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://members.redsonline.jp/otk/index.asp",NULL,NULL,SW_SHOWNORMAL);
			break;

		case	dPBM_LOGIN_MOTP_CAMPAIGN_NO		:
			strcpy(s_strLastID,s_strID);
			strcpy(s_strLastSelectServerName,s_strSelectServerName);

			#ifdef _USE_XTRAP
			XTrap_C_SetUserInfoEx(s_strLastID,s_strLastSelectServerName," "," ",0,NULL);
			#endif

			BookingMode(eGAME_SELECT_AVATAR,dSTEP_FINISH,dFADE_OUT);
			s_inputBox.close();
			cINPDEV::Init(s_hWnd,s_hInst);	//	다이렉트 인풋 초기화
			break;

		case	dPBM_USE_PERSCOM_GUARD_CAMPAIGN_YES		:
			s_agent.disconnect();	//	연결을 끊고..

			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://members.redsonline.jp/security",NULL,NULL,SW_SHOWNORMAL);
			break;

		case	dPBM_USE_PERSCOM_GUARD_CAMPAIGN_NO		:
			s_agent.disconnect();	//	연결을 끊고..
			BookingMode(eGAME_CLOSE,dSTEP_FINISH,dFADE_OUT);
			break;

		case	dPBM_LOGIN_DISCONNECT_CONNECT_ID	:
		{
			g_msgBox.cPopup(NULL,_ms(dMSG_WAIT_FOR_DISCONNECT_AND_SAFE_DATA_SAVE_FORM,dSAFE_DISCONNECT_TIME));
			s_dwWaitForSafeDisconnectTime	=	timeGetTime()+1000*dSAFE_DISCONNECT_TIME;

#ifdef	_DEBUG	//	디버그 모드 일때는 1초만 기다려!
			s_dwWaitForSafeDisconnectTime	=	timeGetTime()+1000;
#endif

			char	strPassword[80];

			if	(g_bIsPortalUser)
			{
				memcpy(strPassword,s_strLastPassword,dTOKEN_LENGTH);
				DecodeBuffer(0x2fea,(BYTE*)strPassword,dTOKEN_LENGTH);
			}
			else
			{
				if	(l_bIsUseKls)
				{
					if	(s_inputBox.getActiveBox() == 1)	
						UnsetShield();

					int	iCount	=	GetInputChars(strPassword);

					if	(s_inputBox.getActiveBox() == 1)
					{
						if	(!SetShield(s_hWnd, GetModuleHandle(NULL)))
							l_bIsUseKls	=	FALSE;
					}

					strPassword[iCount]	=	'\0';
				}
				else	
					s_inputBox.getStr(1,strPassword,sizeof(strPassword));
			}

			s_agent.sendLogIn(s_strID,strPassword,s_strSelectServerName,g_dwClientVersion,TRUE,g_bIsPortalUser,g_iServiceID);
			memset(strPassword,0,sizeof(strPassword));
			break;
		}

		case	dPBM_LOGIN_CANCEL					:
			s_agent.reset();
			break;
	}

	return	FALSE;
}

//
//	패킷 처리
BOOL
cGAME_TITLE::OperatePacket()
{
//	패킷 처리
	if (!s_agent.packetHandler())
		return	FALSE;

	if (s_agent.isReceivedPacket(dSL_LOGIN_RESULT_OTP))
	{
		switch(GetOTPLogInResult())
		{
			case	0	:
				strcpy(s_strLastID,s_strID);
				strcpy(s_strLastSelectServerName,s_strSelectServerName);

				#ifdef _USE_XTRAP
				XTrap_C_SetUserInfoEx(s_strLastID,s_strLastSelectServerName," "," ",0,NULL);
				#endif

				BookingMode(eGAME_SELECT_AVATAR,dSTEP_FINISH,dFADE_OUT);
				s_inputBox.close();
				cINPDEV::Init(s_hWnd,s_hInst);	//	다이렉트 인풋 초기화
				break;

			case	dOTP_RESULT_INCORRECT	:
			case	dOTP_RESULT_EXPIRE	:
			case	dOTP_RESULT_FAIL		:
				g_msgBox.cPopup(NULL,dMSG_LOGIN_FAILED,dMSG_OK);
				break;
		}
	}

	if (s_agent.isReceivedPacket(dSL_RESULT_LOGIN))
	{
		switch(GetLogInResult())
		{
			case	dRESULT_LOGIN_OTP				:
			{
				s_inputBox.active(0);
				UnsetShield();
				s_inputBox.setHideCtrl(0,TRUE);
				s_inputBox.setHideCtrl(1,TRUE);
				g_itemCountWindow.popup(g_iScreenWidth/2,+g_iScreenHeight/2,0,NULL,eICWR_RETURN_MOTP_CODE,100,0xffff,&s_inputBox,2);

				break;
			}

			case	dRESULT_LOGIN_SUCCESS_BUT_NOT_USE_OTP	:
				strcpy(s_strLastID,s_strID);
				strcpy(s_strLastSelectServerName,s_strSelectServerName);

#ifdef _USE_XTRAP
				XTrap_C_SetUserInfoEx(s_strLastID,s_strLastSelectServerName," "," ",0,NULL);
#endif

				BookingMode(eGAME_SELECT_AVATAR,dSTEP_FINISH,dFADE_OUT);
				s_inputBox.close();
				cINPDEV::Init(s_hWnd,s_hInst);
				break;

			case	dRESULT_LOGIN_FAIL_PERSONALCOM	:
				g_msgBox.cPopup(NULL,dMSG_PERSCOM_GUARD_FAILED,dMSG_YES,dMSG_NO);	//	OTP 신청 할껴?
				g_msgBox.setButtonId(dPBM_USE_PERSCOM_GUARD_CAMPAIGN_YES,dPBM_USE_PERSCOM_GUARD_CAMPAIGN_NO);
				break;
				break;

			case	dRESULT_LOGIN_SUCCESS			:	//	OK
			case	dRESULT_LOGIN_SUCCESS_PERSONALCOM:
			{
				strcpy(s_strLastID,s_strID);
				strcpy(s_strLastSelectServerName,s_strSelectServerName);

#ifdef _USE_XTRAP
				XTrap_C_SetUserInfoEx(s_strLastID,s_strLastSelectServerName," "," ",0,NULL);
#endif

				BookingMode(eGAME_SELECT_AVATAR,dSTEP_FINISH,dFADE_OUT);
				s_inputBox.close();
				cINPDEV::Init(s_hWnd,s_hInst);	//	다이렉트 인풋 초기화
				break;
			}

			case	dRESULT_LOGIN_USEDID			:	//	이미 접속중인 아이디이다.
			{
				g_msgBox.cPopup(NULL,dMSG_LOG_IN_ALREADY_CONNECTED,dMSG_YES,dMSG_NO);	//	끊고 접속할껴-_-?
				g_msgBox.setButtonId(dPBM_LOGIN_DISCONNECT_CONNECT_ID);
				break;
			}

			case	dRESULT_LOGIN_FAILED			:	//	아이디가 존재 하지 않거나 패스워드가 틀렸다.
				s_inputBox.setStr(1,"");
				ClearInputChars();

				g_msgBox.cPopup(NULL,dMSG_LOGIN_FAILED,dMSG_OK);
				break;

			case	dRESULT_LOGIN_RETRY				:	//	접속 끊었으니까 다시 접속한다.
				break;

			case	dRESULT_LOGIN_NOT_SAME_VERSION	:
				g_msgBox.cPopup(NULL,dMSG_INCORRECT_CLIENT_VERSION,dMSG_OK);
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
				break;
			}
		}
	}

	if (s_agent.isReceivedPacket(dSL_SERVER_LIST))
		UpdateServerList();

	return	FALSE;
}

//
//	서버리스트 업데이트
void
cGAME_TITLE::UpdateServerList()
{
	int		iSelectServer	=	-1;

	if	(!s_agent.isConnected())
	{
		s_iWorldServerCount	=	0;//	이미 연결된 상태이다.
		s_iSelectServer		=	-1;

		return;
	}

	if	(STRICMP(s_strSelectServerName,"")==0)
		strcpy(s_strSelectServerName,s_strLastSelectServerName);

	for (int i=0;i<s_iWorldServerCount;i++)
	{
//		s_aServerList[i].m_wType	=	eSERVER_TYPE_NORMAL;

		if	(STRICMP(s_aServerList[i].m_strName,s_strSelectServerName)	==	0	)
		{
			iSelectServer	=	i;

			if	(s_iSelectServer	==	-1)
				s_iSelectServer		=	s_aServerList[i].m_wServerPlaceInList;
		}
	}

	if	(iSelectServer		==	-1)
		s_iSelectServer		=	-1;
}

//
//	컨넥트 상태를 유지한다.
BOOL
cGAME_TITLE::HoldConnectState()
{
	int	iRetryPeriod	=	1000*5;

	if	(s_agent.isConnected()	&&	s_iWorldServerCount)
	{
		BOOL	bExistDisableWorld	=	FALSE;

		for (int j=0;j<s_iWorldServerCount;j++)
		{
#ifndef _DEBUG
			if	(s_aServerList[j].m_wType	>=	eSERVER_TYPE_INNER_TEST)
			{
				bExistDisableWorld	=	TRUE;
				break;
			}
#else
			if	(s_aServerList[j].m_wType	==	eSERVER_TYPE_OFF)
			{
				bExistDisableWorld	=	TRUE;
				break;
			}
#endif
		}

		if	(bExistDisableWorld)
			iRetryPeriod	=	1000*10;
		else
		{
			s_isWaitWorldServerConnectMessage	=	FALSE;

			return	FALSE;	//	이미 연결된 상태이다.
		}
	}
	else
		s_iWorldServerCount	=	0;	//	연결되지 않았다면 서버 카운트를 0으로 설정한다.

	if	(s_isWaitWorldServerConnectMessage)
		return	FALSE;	//	서버와 접속 메시지를 기다리는중

	if	(timeGetTime()	<	(DWORD)s_iTimeOfConnectTry+iRetryPeriod)
		return	FALSE;	//	접속 시도한지 얼마 안되었다.

	s_iTimeOfConnectTry	=	timeGetTime();

	int		iPort			=	dLOGINSERVER_PORT;
	char	strLoginIP[64];

	memcpy(strLoginIP,s_strLoginServerAddress,64);

	DecodeAddrBuffer(0x1eaf,(BYTE*)strLoginIP,64);

	if	(s_agent.isConnected())
	{
		s_agent.sendClientType(eCT_GAME);
		s_agent.m_iTryConnectTime	=	timeGetTime();
	}
	else
	if	(!s_agent.connectToServer(strLoginIP,iPort))
	{
		s_agent.disconnect();	//	연결을 끊고..

		s_iTimeOfConnectTry	=	0;

		return	FALSE;
	}

	memset(strLoginIP,0,sizeof(strLoginIP));

	s_isWaitWorldServerConnectMessage	=	TRUE;

	return	FALSE;
}

//
//	업데이트
BOOL
cGAME_TITLE::UpdateMain()
{
/*	if (g_gwGuildMarkComposer.isOpened() == FALSE)
		g_gwGuildMarkComposer.open();

	g_gwGuildMarkComposer.update();

	if (s_isTouchRightButton)
	{
		g_guildMarkPalette.init();
		g_msgBox.cPopup("","길드 마크 팔레트를 다시 불러 들였습니다.",dMSG_OK);
	}

	return TRUE;
*/
#ifdef	_DEBUG
	if	(s_aKey[VK_F5])
		s_agent.sendClientType(eCT_GAME);
#endif

	char	strPassword[80];
	int		iCount	=	0;

	l_spline.update();

	s_iFocusLoginServer	=	GetFocusLoginServer();

	if	(s_isClickedLeftButton && s_iFocusLoginServer != 0xffff)
	{
		if	(s_iCurrentLoginServer	!=	s_iFocusLoginServer)
		{
			s_iCurrentLoginServer	=	s_iFocusLoginServer;

			memcpy(s_strLoginServerAddress,s_aLoginServerIPList[s_iFocusLoginServer].m_strIP,64);

			s_agent.reset();
			s_iTimeOfConnectTry					=	0;
			s_isWaitWorldServerConnectMessage	=	FALSE;

			cFILE	file;

			file.Open("data/@@lastConnectLoginServer.dat","wb");

			file.Write(&s_iCurrentLoginServer,4);
			file.Close();
		}
	}

	if	(g_bIsPortalUser == FALSE && l_bIsUseKls)
	{
		int	iActiveBox	=	s_inputBox.getActivateBox();

		if	(iActiveBox	== 1)
		{
			s_inputBox.setStr(1,"");

			ClearInputChars();

			if	(!SetShield(s_hWnd, GetModuleHandle(NULL)))
				l_bIsUseKls	=	FALSE;
		}

		if	(iActiveBox	== 0)
			UnsetShield();
	}

	if	(s_dwWaitForSafeDisconnectTime)
	{
		int	iRemainTime	=	s_dwWaitForSafeDisconnectTime -	timeGetTime();

		if	(iRemainTime	>	0)
			g_msgBox.cPopup(NULL,_ms(dMSG_WAIT_FOR_DISCONNECT_AND_SAFE_DATA_SAVE_FORM,iRemainTime/1000));

		if	(s_dwWaitForSafeDisconnectTime <	timeGetTime() || iRemainTime > dSAFE_DISCONNECT_TIME*1000)
		{
			s_dwWaitForSafeDisconnectTime	=	0;
			g_msgBox.close();
		}
	}

	{
		int	iInputCodeResult	=	g_itemCountWindow.update();

		if	(iInputCodeResult	!=	eICWR_NONE)
		{
			if	(iInputCodeResult	==	eICWR_RETURN_MOTP_CODE)
			{
				char	strCode[64];

				s_inputBox.getStr(2,strCode,64);

				CL_LOGIN_OTP	packet;

				packet.base.set(sizeof(packet),dCL_LOGIN_OTP);
				strcpy(packet.strOTPCode,strCode);

				s_agent.sendPacket((char*)&packet,sizeof(packet));
			}

			s_inputBox.setHideCtrl(0,FALSE);
			s_inputBox.setHideCtrl(1,FALSE);
			s_inputBox.setHideCtrl(2,TRUE);
			s_inputBox.setStr(2,"");
			s_inputBox.setCorrectPos(2,-10000,-10000);

			s_inputBox.active(0);

			s_sprTitle.Put(0,0,0);
		}
	}

	OperatePacket();//패킷 처리
	WaitConnectMessage();//컨넥트 메시지 기다림
	HoldConnectState();//컨넥트 상태 유지

	if	(OperateMessageBox() || g_itemCountWindow.m_bIsOpend)
		return	TRUE;

	UpdateServerList();

	s_iFocusServer	=	GetSelectWorld();

	if	(s_iWorldServerCount	==	0)
		strcpy(s_strSelectServerName,"");

//	아이디와 패스워드
	if	(g_bIsPortalUser)
	{
		if	(s_agent.isConnected() && s_iWorldServerCount > 0 && STRICMP(s_strSelectServerName,"")!=0)
			s_bmMenu.enable(dTM_LOGIN);
		else
			s_bmMenu.disable(dTM_LOGIN);
	}
	else
	{
		s_inputBox.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,s_aTouched[9]);
		s_inputBox.getStr(0,s_strID,sizeof(s_strID));
		s_inputBox.getStr(1,strPassword,sizeof(strPassword));

		if	(CheckID(s_strID) && strlen(strPassword) >= 4 && s_agent.isConnected()	&&	s_iWorldServerCount > 0)
			s_bmMenu.enable(dTM_LOGIN);
		else
			s_bmMenu.disable(dTM_LOGIN);

		memset(strPassword,0,sizeof(strPassword));
	}

	DWORD	dwSelectMenu=	s_bmMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

	if	(dwSelectMenu	==	0xffffffff)
		dwSelectMenu	=	s_bmCheckBox.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

	if	(s_isClickedLeftButton	&&	s_iFocusServer != -1)
	{
		s_iSelectServer	=	s_aServerList[s_iFocusServer].m_wServerPlaceInList;

		strcpy(s_strSelectServerName,s_strFocusServerName);
	}
	else
	if	(s_iFocusServer	!=	-1)
	{
		if	(s_aServerList[s_iFocusServer].m_wMessage != 0xffff	&&	s_aServerList[s_iFocusServer].m_wMessage < g_loaderText.m_iServerCommentCount)
		{
			char	*lpstrComment	=	g_pstrServerComment[s_aServerList[s_iFocusServer].m_wMessage];	
			
			if	(strlen(lpstrComment)	>	5)
			{
				cRECT	rect;
				
				rect.set(&s_rectFocusWorld);
				
				s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,lpstrComment,eTFM_LEFT);
			}
		}
	}

#ifdef	_USE_NPGL
	if (g_bIsUseNPGL)
		if (random(dSYNC_FPS*10) == 0)
			if (g_npgl->Check()	!=	NPGAMEMON_SUCCESS)
			{
				s_isRunApplication	=	FALSE;

				g_eh.outputStaticLog(_ms("%s/log.txt",g_strLogFolder));
			}
#endif

	switch(dwSelectMenu)
	{
		case	dTM_LOGIN		:
		{
			if	(s_iWorldServerCount	==	0)	
				break;

			int	iSelectServer	=	0xffff;

			for (int i=0;i<s_iWorldServerCount;i++)
				if (s_iSelectServer	== s_aServerList[i].m_wServerPlaceInList)
					iSelectServer	=	i;

			if	(iSelectServer == 0xffff)
			{
				g_msgBox.cPopup(NULL,dMSG_PLEASE_SELECT_SERVER,dMSG_OK);
				break;
			}

			if	(s_aServerList[iSelectServer].m_wType == eSERVER_TYPE_OFF)	
			{
				g_msgBox.cPopup(NULL,dMSG_PLEASE_SELECT_SERVER,dMSG_OK);
				break;
			}

#ifndef _DEBUG
			if	(s_aServerList[iSelectServer].m_wType == eSERVER_TYPE_INNER_TEST)	
			{
				g_msgBox.cPopup(NULL,dMSG_PLEASE_SELECT_SERVER,dMSG_OK);
				break;
			}
#endif
			g_msgBox.cPopup(NULL,dMSG_WAIT_LOGIN_RESULT,dMSG_CANCEL);
			g_msgBox.setButtonId(dPBM_LOGIN_CANCEL);
			g_msgBox.setWaitPacket(dSL_RESULT_LOGIN);

			g_bIsTestServer			=	FALSE;
			g_bIsInnerTestServer	=	FALSE;
			g_bIsDuelServer			=	FALSE;
			g_iWorldServerType		=	s_aServerList[iSelectServer].m_wType;

			if	(s_aServerList[iSelectServer].m_wType == eSERVER_TYPE_TEST	||
				s_aServerList[iSelectServer].m_wType == eSERVER_TYPE_INNER_TEST	)
				g_bIsTestServer	=	TRUE;

			if	(s_aServerList[iSelectServer].m_wType == eSERVER_TYPE_DUEL)
				g_bIsDuelServer	=	TRUE;

			if	(g_bIsPortalUser)
			{
				strcpy(s_strID,s_strLastID);

				memcpy(strPassword,s_strLastPassword,dTOKEN_LENGTH);
				DecodeBuffer(0x2fea,(BYTE*)strPassword,dTOKEN_LENGTH);
			}
			else
			{
#ifndef	_USE_HS
				if	(l_bIsUseKls)
				{
					if	(s_inputBox.getActiveBox() == 1)	
						UnsetShield();

					iCount	=	GetInputChars(strPassword);

					if	(s_inputBox.getActiveBox() == 1)
					{
						if	(!SetShield(s_hWnd, GetModuleHandle(NULL)))
							l_bIsUseKls	=	FALSE;
					}

					strPassword[iCount]	=	'\0';
				}
				else
#endif
					s_inputBox.getStr(1,strPassword,sizeof(strPassword));
			}

			s_agent.sendLogIn(s_strID,strPassword,s_strSelectServerName,g_dwClientVersion,FALSE,g_bIsPortalUser,g_iServiceID);
			memset(strPassword,0,sizeof(strPassword));
			break;
		}

		case	dTM_REMEMBER_ID	:
		{
			g_config.m_bf1IsRememberId	=	1-g_config.m_bf1IsRememberId;
			break;
		}

		case	dTM_CREATE_ID	:
		{
#ifdef	_FOR_KOREA
			if	(g_bIsPortalUser)
				break;

			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://www.redgem.co.kr",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_CHINA
			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://www.redgem.com.cn",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_JAPAN
			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://www.redsonline.jp",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_INTERNATIONAL
			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://redstone.lnklogickorea.com",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_USA
			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://playredstone.com/classes.php?nav=red",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_INTER
			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://redstone.gamengame.com",NULL,NULL,SW_SHOWNORMAL);
#elif	_FOR_THAI
			SetForegroundWindow(GetDesktopWindow());
			ShellExecute(NULL,"open","http://redstone.vplay.in.th/register.php",NULL,NULL,SW_SHOWNORMAL);
#endif
 			break;
		}

		case	dTM_CREDIT		:
			break;

		case	dTM_EXIT		:
		{
			if	(s_aKey[VK_SHIFT] && s_aKey[VK_CONTROL] && s_aKey[VK_F2])
			{
				cFILE	file;

				file.Open("v.txt","wt");
				file.writeStream("%d",l_iVersionControl);
				file.Close();
			}

			BookingMode(eGAME_CLOSE,dSTEP_FINISH,dFADE_OUT);
			s_inputBox.close();
			break;
		}
	}

	return	SAFETY;
}

BOOL
cGAME_TITLE::UpdateFinish()
{
	if (g_canvas.getFadeScreenStatus()	!=	dFADE_OUT || g_canvas.isCompleteFadeOut())
		BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

BOOL
cGAME_TITLE::UpdateClose()
{
	if	(g_bIsPortalUser == FALSE)
		s_inputBox.close();

	s_bmButton.close();
	s_sprSprite.close();
	s_sprTitle.close();

#ifndef	_USE_HS
	if	(l_bIsUseKls)
		UnsetShield();
#endif

	return	SAFETY;
}


BOOL
cGAME_TITLE::Run()
{
	switch(s_iStep)
	{
		case	dSTEP_INIT			:	return	UpdateInit();
		case	dSTEP_READY			:	return	UpdateReady();
		case	dSTEP_MAIN			:	return	UpdateMain();
		case	dSTEP_FINISH		:	return	UpdateFinish();
		case	dSTEP_CLOSE			:	return	UpdateClose();
	}

	return	FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_TITLE::DrawInit()
{
	DrawMain();
}

void
cGAME_TITLE::DrawReady()
{
	DrawMain();
}

void
cGAME_TITLE::DrawMain()
{
//	if	(g_gwGuildMarkComposer.isOpened())	{		g_gwGuildMarkComposer.draw();		return;	}
	cDRAW::Clear();

//	ExportNewMonsterImage();

	if	(!g_bIsTerminalPC)
	{
//		cDRAW::PushInfo();
//		cDRAW::Active(s_pPannel,g_iScreenWidth,g_iScreenHeight);
//		s_sprTitle.Put(400,300,3+(s_iFrameCounter/4%8));
//		s_sprTitle.Put(0,0,11);
//		cDRAW::PopInfo();

//		s_sprTitle.Put(0,0,0);
		s_sprTitle.putReferenceImage(l_spline.m_pos.x,l_spline.m_pos.y,1,s_pPannel);
	}

	int	iPannelDeltaWidth	=	(g_iScreenWidth - 517 + g_iCorrectBI_X)*(32-g_canvas.getFadeScreenValue())/32;

	s_bmButton.setCorrectPos(iPannelDeltaWidth,0);

	if (g_bIsPortalUser == FALSE)
	{
		if(s_iStep	==	dSTEP_MAIN	&&	g_canvas.getFadeScreenValue()	==	32)
		{
			if (s_inputBox.isEnable()	==	FALSE)
				s_inputBox.enable();
		}
	}

	s_sprSprite.Put(529+iPannelDeltaWidth+g_iCorrectBI_X,14,eLSI_LOGIN);

	int	iButtonDeltaWidth	=	(g_iScreenWidth - 534+g_iCorrectBI_X)*(32-g_canvas.getFadeScreenValue())/32;

	s_bmMenu.setCorrectPos(iButtonDeltaWidth,0);
	s_bmCheckBox.setCorrectPos(iButtonDeltaWidth,0);

	s_bmMenu.draw();
	s_bmCheckBox.draw();

	cDRAW::FillHB(0,700+g_iCorrectBI_X,578+g_iCorrectBI_Y,790+g_iCorrectBI_X,594+g_iCorrectBI_Y);

#ifdef	_DEBUG
	s_text.put(530+g_iCorrectBI_X,580+g_iCorrectBI_Y,WHITE,"<c:LTYELLOW>[F5]<n> Refresh server List");
#endif

#ifdef	_FOR_KOREA
	s_text.put(706+g_iCorrectBI_X,580+g_iCorrectBI_Y,WHITE,"Version <c:LTYELLOW>%d.%.4d<n>β",g_dwClientVersion/1000,g_dwClientVersion%1000);
#elif	_FOR_CHINA
	s_text.put(706+g_iCorrectBI_X,580+g_iCorrectBI_Y,WHITE,"Version <c:LTYELLOW>%d.%.4d<n>",g_dwClientVersion/1000+8,g_dwClientVersion%1000);
#else
	s_text.put(706+g_iCorrectBI_X,580+g_iCorrectBI_Y,WHITE,"Version <c:LTYELLOW>%d.%.4d<n>",g_dwClientVersion/1000,g_dwClientVersion%1000);
#endif
#ifdef	_DEBUG
	s_bText.put(10,520+g_iCorrectBI_Y,600,200,0,WHITE,"Build Date <c:LTYELLOW>%s<n>\nRevision <c:LTYELLOW>%d<n>",
															g_strBuildDate,g_iRevision);
#endif	
//	월드 리스트, 아이디 입력창 외곽 프레임 그려주기
	{
		s_rectServerListBorder.set(533+g_iCorrectBI_X,81 ,799+g_iCorrectBI_X,289);
		s_rectServerListBorder.add(iPannelDeltaWidth,0);
		s_ttFrame.drawBox(&s_rectServerListBorder);
	}

	DrawLoginServerList();
	DrawWorldList();
//	서버 리스트
//	s_bmButton.draw();

//	아이디 입력창
	if	(g_bIsPortalUser == FALSE)
	{
		cRECT	rectBorder;

		rectBorder.set(533+g_iCorrectBI_X,313+g_iCorrectBI_Y,799+g_iCorrectBI_X,379+g_iCorrectBI_Y);
		rectBorder.add(iPannelDeltaWidth,0);
		s_ttFrame.drawBox(&rectBorder,0,32);

		int		iX	=	534+64+iPannelDeltaWidth+g_iCorrectBI_X;

		s_bText.putRF(iX+6,328+g_iCorrectBI_Y,RGB(181,107,74),dMSG_ID);
		s_bText.putRF(iX+6,356+g_iCorrectBI_Y,RGB(181,107,74),dMSG_PASSWORD);

		cDRAW::XLine(RGB24To16(RGB(181,107,74)),iX+8,iX+4+180,344+g_iCorrectBI_Y);
		cDRAW::XLine(RGB24To16(RGB(181,107,74)),iX+8,iX+4+180,370+g_iCorrectBI_Y);

		s_inputBox.draw(WHITE);
	}

//	서버 상태
	{
		int	iColor,iCounter	=	s_iFrameCounter*2;

		if (iCounter%200>=	100)
			iColor=	255-(iCounter%100);
		else
			iColor=	155+(iCounter%100);

		char	strText[128];

		if	(s_agent.isConnected() && s_iWorldServerCount	>	0)
			sprintf(strText,"%s <c:%d,%d,0>%s<n>",dMSG_SERVER_STATUS,iColor,iColor,dMSG_SERVER_ON);
		else
			sprintf(strText,"%s <c:%d,%d,%d>%s<n>",dMSG_SERVER_STATUS,iColor,iColor,iColor,dMSG_SERVER_OFF);

		int	iX	=	g_iScreenWidth	-	s_text.getPixelSize(strText)-10+iPannelDeltaWidth;

		s_text.put(iX,60,WHITE,strText);
	}

	g_itemCountWindow.draw();
#ifdef	_FOR_KOREA

	int iWidth	=	g_sprRegionInterface.getSpriteWidth(eIK_RANK_12AGE);

	g_sprRegionInterface.Put(5,10,eIK_RANK_12AGE);
	g_sprRegionInterface.Put(5+iWidth,13,eIK_VIOLENCE);

#endif
#ifdef	_FOR_CHINA
	s_sprSprite.Put(0,g_iScreenHeight,15);
#elif _FOR_JAPAN
	s_sprSprite.Put(10,555+g_iCorrectBI_Y,15);
#else
	s_sprSprite.Put(10,565+g_iCorrectBI_Y,15);
#endif
}


void
cGAME_TITLE::DrawFinish()
{
	DrawMain();
}

void
cGAME_TITLE::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_TITLE::Draw()
{
	switch(s_iStep)
	{
		case	dSTEP_INIT	:	
			DrawInit();
			break;

		case	dSTEP_READY	:	
			DrawReady();
			break;

		case	dSTEP_MAIN	:	
			DrawMain();
			break;

		case	dSTEP_FINISH:	
			DrawFinish();
			break;

		case	dSTEP_CLOSE	:	
			DrawClose();
			break;
	}

	return	TRUE;
}
