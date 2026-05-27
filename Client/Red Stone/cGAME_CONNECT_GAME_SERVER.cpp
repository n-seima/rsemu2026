#include "cGAME_CONNECT_GAME_SERVER.H"
#include "CAgent.H"
#include "cJOB.H"
#include "CActor.H"
#include "CMessage.H"
#include "rscomm.H"

#define	IDM_CANCEL_TO_CONNECT_GAME_SERVER	2000

BOOL	cGAME_CONNECT_GAME_SERVER::s_isWaitGameServerConnectMessage	=	FALSE;

int		l_iGameServerPort	=	0;

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////


//
//	초기화
BOOL
cGAME_CONNECT_GAME_SERVER::UpdateInit()
{
	int		iPort		=	dGAMESERVER_PORT;

#ifdef	_FOR_CHINA
	if	(!s_agent.connectToServer(s_strGameServerAddress,iPort,20,FALSE))
#else

	if	(!s_agent.connectToServer(s_strGameServerAddress,iPort,20,FALSE))
#endif
	{
		g_msgBox.cPopup(NULL,dMSG_CONNECT_FAILED,dMSG_CANCEL);

		g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
		g_msgBox.setExclusive(FALSE);

		return	FALSE;
	}

	s_isWaitGameServerConnectMessage	=	TRUE;

	{
		g_msgBox.cPopup(NULL,_ms("%s",dMSG_CONNECT_TO_GAME_SERVER),dMSG_CANCEL);
//		g_msgBox.cPopup(NULL,_ms("%s\n[%s]",dMSG_CONNECT_TO_GAME_SERVER,s_strGameServerAddress),dMSG_CANCEL);
		g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
		g_msgBox.setExclusive(FALSE);
	}

	BookingStep(dSTEP_READY,dFADE_IN);

	return	SAFETY;
}

BOOL
cGAME_CONNECT_GAME_SERVER::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}

//
//	패킷 처리
BOOL
cGAME_CONNECT_GAME_SERVER::OperatePacket()
{
//	패킷 처리
	if	(s_agent.isDisconnected())
	{
		s_isWaitGameServerConnectMessage	=	FALSE;

		g_msgBox.cPopup(NULL,s_agent.m_strDisconnectText,dMSG_OK);
		g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
		s_agent.reset();

		return TRUE;
	}

	if	(!s_agent.packetHandler())
		return	FALSE;

//	게임 연결 결과
	if	(s_agent.isReceivedPacket(dSG_RESULT_CONNECT))
	{
		switch(GetConnectResult())
		{
			case dRESULT_CONNECT_SUCCESS	://게임 접속 성공!! 게임 플레이 모드로 전환!!
				g_msgBox.close();
				BookingMode(eGAME_PLAY,dSTEP_FINISH,dFADE_OUT);
				s_timer.resetFrameSkip();
				break;

			case dRESULT_CONNECT_FAIL		://게임 접속 실패!! 로그인 화면으로 돌아가자 -_-a
//				cMSG::Put("에러","게임 접속 실패 1");
				s_agent.reset();
				g_msgBox.cPopup(NULL,dMSG_CAN_NOT_CONNECT_TO_GAME_SERVER,dMSG_OK);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				break;

			case dRESULT_CONNECT_FULL		://게임 접속 실패!! 로그인 화면으로 돌아가자 -_-a
				s_agent.reset();
				g_msgBox.cPopup(NULL,dMSG_TOO_MANY_PLAYER,dMSG_OK);
				g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
				break;
		}
	}

	return	FALSE;
}

//
//	메인 업데이트
BOOL
cGAME_CONNECT_GAME_SERVER::OperateMessageBox()
{
//	메시지 박스 처리
	if (!s_isOperatedMsgbox)	return	FALSE;
	if	(g_msgBox.isNothing())	return	TRUE;

	switch(g_msgBox.getResult())
	{
		case	eMBI_RETURN_TO_TITLE	:		//	로그인 화면으로 돌아가기
		case	eMBI_RETURN_TO_TITLE_BY_CANCEL	:
			BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
			break;

	}

	return	FALSE;
}

//
//	서버 연결 메시지를 기다림
BOOL
cGAME_CONNECT_GAME_SERVER::WaitConnectMessage()
{
	if	(!s_isWaitGameServerConnectMessage)
		return	FALSE;

	int	iResult	=	s_agent.getConnectServerResult();

	switch(iResult)
	{
		case	dCONNECT_STATUS_CONNECT_OK		:	//접속 완료. 게임 접속 메시지를 날리자.
		{
			s_isWaitGameServerConnectMessage	=	FALSE;

			s_agent.sendConnectGame(s_strLastID,s_strSelectAvatarName,s_iConnectType);//게임 연결 메시지 전송
			break;
		}

		case	dCONNECT_STATUS_CONNECT_FAILED	:
		case	dCONNECT_STATUS_TIME_OVER		:
//			cMSG::Put("에러","게임 접속 실패 2");
			s_isWaitGameServerConnectMessage	=	FALSE;
			s_agent.reset();
			g_msgBox.cPopup(NULL,dMSG_CAN_NOT_CONNECT_TO_GAME_SERVER,dMSG_OK);
			g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
			return	TRUE;

		case	dCONNECT_STATUS_TRYING			:
			if (s_iConnectType	==	eCONNECT_TYPE_FIRST)	//	처음 접속 하는거라면 상자가 뜬다.
				g_msgBox.setText(_ms("%s\n\n%d",dMSG_CONNECT_TO_GAME_SERVER,s_agent.getRemianConnectServerTime()));
			return	TRUE;
	}

	return	FALSE;
}

//
//	메인 업데이트
BOOL
cGAME_CONNECT_GAME_SERVER::UpdateMain()
{
	WaitConnectMessage();
	OperatePacket();

	if (OperateMessageBox())	return	TRUE;

	return	SAFETY;
}

BOOL
cGAME_CONNECT_GAME_SERVER::UpdateFinish()
{
	if (g_canvas.isCompleteFadeOut())	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}

//
//
BOOL
cGAME_CONNECT_GAME_SERVER::UpdateClose()
{
	return	SAFETY;
}

//
//	Run!!!
BOOL
cGAME_CONNECT_GAME_SERVER::Run()
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
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_CONNECT_GAME_SERVER::DrawInit()
{
	DrawMain();
}

void
cGAME_CONNECT_GAME_SERVER::DrawReady()
{
	DrawMain();
}

//
//	그려주기.. 메인 루프-_-?
void
cGAME_CONNECT_GAME_SERVER::DrawMain()
{
	cDRAW::Clear();
}

void
cGAME_CONNECT_GAME_SERVER::DrawFinish()
{
	DrawMain();
}

void
cGAME_CONNECT_GAME_SERVER::DrawClose()
{
	cDRAW::Clear();
}

BOOL
cGAME_CONNECT_GAME_SERVER::Draw()
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