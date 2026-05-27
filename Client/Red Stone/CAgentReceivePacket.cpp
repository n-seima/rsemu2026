#include "CAgent.H"
#include "CActor.H"
#include "CGamePlay.H"
#include "cGAME_SELECT_AVATAR.H"
#include "CMessage.H"

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃로그인 서버

//
//	로그인 결과
void
cPACKET_HANDLER::ResultLogin(SERVER_PACKETS *_lpPacket)
{
	SetLogInResult(_lpPacket->mSL_RESULT_LOGIN.dwResult);

	s_dwPasswordAuthenticationCode	=	_lpPacket->mSL_RESULT_LOGIN.dwCode;
	s_wPacketSecurityCode			=	_lpPacket->mSL_RESULT_LOGIN.wSecurityCode;

	if	(_lpPacket->mSL_RESULT_LOGIN.dwResult	>	dRESULT_LOGIN_NOT_SAME_VERSION)
		strcpy(CGame::s_strMessage,_lpPacket->mSL_RESULT_LOGIN.strWhy);

	cGAME_SELECT_AVATAR::s_bIsWaitAvatarList	=	TRUE;
}

void
cPACKET_HANDLER::ResultLoginOTP(SERVER_PACKETS *_lpPacket)
{
	SL_LOGIN_RESULT_OTP	*lpPacet	=	(SL_LOGIN_RESULT_OTP	*)_lpPacket;

	SetOTPLogInResult(lpPacet->iResult);
	cGAME_SELECT_AVATAR::s_bIsWaitAvatarList	=	TRUE;
}

//
//	로그인 결과
void
cPACKET_HANDLER::ServerList(SERVER_PACKETS *_lpPacket)
{
	s_wPacketSecurityCode	=	_lpPacket->mSL_SERVER_LIST.wSecurityCode;
	s_iWorldServerCount		=	_lpPacket->mSL_SERVER_LIST.wCount;

	memcpy(s_aServerList,_lpPacket->mSL_SERVER_LIST.aServerList,sizeof(cSERVER_INFO)*s_iWorldServerCount);

	for (int i=0;i<s_iWorldServerCount;i++)
	{
#ifndef _DEBUG
		if	(s_aServerList[i].m_bf1IsInnerOpen || s_aServerList[i].m_wType	==	eSERVER_TYPE_INNER_TEST)
			memset(s_aServerList[i].m_strName,0, sizeof(char) * dWORLD_NAME_LENGTH);
#endif
	}

//	g_eh.addStaticLog("%s[%d]",s_aServerList[0].m_strName,s_aServerList[0].m_wType);
//	g_eh.addStaticLog("%s[%d]",s_aServerList[1].m_strName,s_aServerList[1].m_wType);
//	g_eh.addStaticLog("%s[%d]",s_aServerList[2].m_strName,s_aServerList[2].m_wType);
//	g_eh.addStaticLog("%s[%d]",s_aServerList[3].m_strName,s_aServerList[3].m_wType);
}

//
// 아바타리스트를 받는다.
void
cPACKET_HANDLER::AvatarList(SERVER_PACKETS *_lpPacket)
{
	SL_AVATAR_LIST	*lpPacket	=	&_lpPacket->mSL_AVATAR_LIST;

	s_wPacketSecurityCode	=	lpPacket->wSecurityCode;

	int	i;

	memset(s_aAvatarList,0xff,sizeof(s_aAvatarList));

	for (i=0;i<dMAX_AVATAR_COUNT;i++)
		strcpy(s_aAvatarList[i].m_strName,"EMPTY");

	for (i=0;i<dMAX_AVATAR_COUNT;i++)
	{
		int	iAvatarIndex	=	_lpPacket->mSL_AVATAR_LIST.aAvatarList[i].m_wAvatarIndex;

		if	(iAvatarIndex	==	0xffff)
			continue;

		memcpy(&s_aAvatarList[iAvatarIndex].m_wAvatarIndex,&_lpPacket->mSL_AVATAR_LIST.aAvatarList[i],sizeof(cAVATAR_INFO));
	}

	cGAME_SELECT_AVATAR::s_bIsWaitAvatarList	=	FALSE;
}
//
// 아바타 생성 결과를 받는다.
void
cPACKET_HANDLER::ResultCreateAvatar(SERVER_PACKETS *_lpPacket)
{
	SL_RESULT_CREATE_AVATAR	*lpPacket	=	&_lpPacket->mSL_RESULT_CREATE_AVATAR;

	s_wPacketSecurityCode	=	lpPacket->wSecurityCode;

	SetCreateAvatarResult(lpPacket->wResult);	//	결과

	if (lpPacket->wResult	==	dRESULT_CREATE_AVATAR_SUCESS)
	{
		memcpy(&s_aAvatarList[lpPacket->wIndex],&lpPacket->avatarInfo,sizeof(cAVATAR_INFO));	//	아바타 정보

	}
}

//
// 아바타 제거 결과를 받는다.
void
cPACKET_HANDLER::ResultRemoveAvatar(SERVER_PACKETS *_lpPacket)
{
	SL_RESULT_REMOVE_AVATAR	*lpPacket	=	&_lpPacket->mSL_RESULT_REMOVE_AVATAR;

	s_wPacketSecurityCode	=	lpPacket->wSecurityCode;

	SetRemoveAvatarResult(lpPacket->wResult);	//	결과
}

//
// 아바타 제거 결과를 받는다.
void
cPACKET_HANDLER::ReceiveSelectAvatarResult(SERVER_PACKETS *_lpPacket)
{
	SL_RESULT_SELECT_AVATAR	*lpPacket	=	&_lpPacket->mSL_RESULT_SELECT_AVATAR;

	s_wPacketSecurityCode	=	lpPacket->wSecurityCode;

	SetSelectAvatarResult(lpPacket->wResult);	//	결과
}

void
cPACKET_HANDLER::ResultOfCopyAvatar(SERVER_PACKETS *_lpPacket)
{
	SL_RESULT_OF_COPY_AVATAR	*lpPacket	=	(SL_RESULT_OF_COPY_AVATAR	*)_lpPacket;

	s_wPacketSecurityCode	=	lpPacket->wSecurityCode;

	switch(lpPacket->wServerType)
	{
		case	eSERVER_TYPE_DUEL	:
			if	(lpPacket->wResult	==	1)
				g_msgBox.cPopup(NULL,dMSG_COMPLETE_COPY_DATA_TO_DUEL_SERVER_WORK,dMSG_OK);
			else
				g_msgBox.cPopup(NULL,dMSG_FAILED_DATA_COPY_TO_DUEL_SERVER_WORK,dMSG_OK);
			break;
		case	eSERVER_TYPE_TEST	:
			if	(lpPacket->wResult	==	1)
				g_msgBox.cPopup(NULL,dMSG_COMPLETE_COPY_DATA_TO_TEST_SERVER_WORK,dMSG_OK);
			else
				g_msgBox.cPopup(NULL,dMSG_FAILED_DATA_COPY_TO_TEST_SERVER_WORK,dMSG_OK);
			break;
	}
}

//┃로그인 서버
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// SSL 인증 패킷 받음
void
cPACKET_HANDLER::ReceiveSSLAuthenticationCode(SERVER_PACKETS *_lpPacket)
{
	SG_COMPLETE_SSL	*lpPacket	=	(SG_COMPLETE_SSL	*)_lpPacket;

	if (lpPacket->wResult	==	0)
		s_agent.disconnect();

	s_agent.receiveSSLAuthenticationCode();
}