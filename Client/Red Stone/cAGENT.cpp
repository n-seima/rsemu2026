#include "CAgent.H"
#include "cMESSAGE.H"
#include "CActor.H"
#include "CPartyWindow.H"
#include "console.H"
#include "CFriendWindow.h"
#include "cguild.h"
#include "mess_sign.h"
#include "mac.h"
#include <nb30.h>
#pragma comment(lib, "netapi32")
#include <nb30.h>
#include <wsipx.h>
#include <wsnwlink.h>

void	(*CAgent::fpPacketHandler[dMAX_SERVER_PACKET_COUNT])(SERVER_PACKETS*);


CAgent::CAgent()
{
	m_iGameServerPort	=	-1;
	m_wIsUseSSL			=	FALSE;

	memset(m_strClientCode,0,sizeof(m_strClientCode));
}

CAgent::~CAgent()
{

}

void
CAgent::disconnect()
{
	m_socket.close();
}

//	브로드 캐스트 서버 끊기
void
CAgent::disconnectBroadCastServer()
{
	m_socketBCS.close();
}


BOOL
CAgent::isDisconnected()
{
	if	(m_isEventDisconnect)
	{
		if	(m_strDisconnectText[0]	==	0)
		{
			g_eh.addStaticLog("CAgent::isDisconnected m_isEventDisconnect");
			strcpy(m_strDisconnectText,dMSG_DISCONNECT_WITH_SERVER);
		}

		return	TRUE;
	}
	
	if	(m_socket.isDisconnected())	//	디스 됐냐?
	{
		if	(m_strDisconnectText[0]	==	0)
		{
			g_eh.addStaticLog("CAgent::isDisconnected m_socket.isDisconnected()");

			strcpy(m_strDisconnectText,dMSG_DISCONNECT_WITH_SERVER);
		}

		return	TRUE;
	}

	return	FALSE;
}

//
//	초기화
void
CAgent::reset()
{
	m_isConnected				=	FALSE;
	m_isEventDisconnect			=	FALSE;
	m_isActive					=	FALSE;
	m_strDisconnectText[0]		=	NULL;

	m_dwReturnPacketSpeed		=	0;
	m_dwSumOfReturnPacketSpeed	=	0;
	m_wReturnPacketCount		=	0;

	CPacketStatus::reset();	//	초기화
	clearReceivedPacketCheck();	//	네트워크 메시지 초기화
	m_quePacket.reset();		//	패킷 큐 초기화
	disconnect();				//	연결을 끊고..
}

//
//	게임 서버에 연걸(게임 서버 주소를 받았어야 한다.
BOOL
CAgent::connectToGameServer()
{
	if	(m_iGameServerPort==	-1)
		return	FALSE;

	BOOL	bIsResult	=	connectToServer(m_strGameServerIp,m_iGameServerPort,TRUE);

	return	bIsResult;
}

//
//	게임 서버에 연걸(게임 서버 주소를 받았어야 한다.
BOOL
CAgent::connectToBroadCastServer(char *_lpstrIP,int _iPortNumber,int _iIndex)
{
	if	(m_socketBCS.isConnected()	&&	m_wIndexForBCS	==	_iIndex)
		return	FALSE;

	if	(!m_socketBCS.connect(_lpstrIP,_iPortNumber))
		return	FALSE;

	m_wIndexForBCS		=	_iIndex;
	m_bIsConnected2BCS	=	FALSE;

	return	TRUE;
}

//
//	서버 연결까지 남은 시간.
int
CAgent::getRemianConnectServerTime()
{
	return	(m_iLimitConnectTime - (timeGetTime()-m_iTryConnectTime))/1000;
}

//
//	연결 결과를 리턴한다.
int	
CAgent::getConnectServerResult()
{
		if	(timeGetTime() - m_iTryConnectTime > (DWORD)m_iLimitConnectTime)	//	20초가 넘으면 연결을 끊는다.
		return	dCONNECT_STATUS_TIME_OVER;

	if	(m_bIsWaitHandShakeData || m_bIsWaitSSLAuthenticationCode)
		return	dCONNECT_STATUS_TRYING;

	if	(m_socket.isConnected())
	{
		if	(!sendDummyMessage())
			return	dCONNECT_STATUS_CONNECT_FAILED;

		m_isConnected	=	TRUE;

		return	dCONNECT_STATUS_CONNECT_OK;
	}

	return	dCONNECT_STATUS_TRYING;
}

//
//	
BOOL
CAgent::sendDummyMessage()
{
	if	(m_bIsWaitHandShakeData || m_bIsWaitSSLAuthenticationCode)
		return	TRUE;

	CM_DUMMY	packet;

	packet.base.set(sizeof(CM_DUMMY),dCM_DUMMY);

	if	(isConnectedBCServer())
		sendBCSPacket(&packet);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	서버에 연결한다.
BOOL
CAgent::connectToServer(char *_strIp,int _iPortNumber,int _iLimitTime,BOOL _bIsUseSSL)
{
	reset();

	if	(_iPortNumber	==	0xffff)
		_iPortNumber	=	dPORT_DUMMY_SERVER;

	m_isEventDisconnect				=	FALSE;	//	디스컨넥트 이벤트 초기화
	m_isActive						=	TRUE;
	m_iLimitConnectTime				=	_iLimitTime*1000;
	m_iTryConnectTime				=	timeGetTime();
	m_wIsUseSSL						=	_bIsUseSSL;
	m_bIsWaitHandShakeData			=	FALSE;
	m_bIsWaitSSLAuthenticationCode	=	FALSE;

	if	(m_wIsUseSSL)
	{
		m_bIsWaitHandShakeData			=	TRUE;
		m_bIsWaitSSLAuthenticationCode	=	TRUE;
	}

	if	(!m_socket.connect(_strIp,_iPortNumber))
		return	FALSE;

	return	TRUE;
}

//
//	FD_CONNECT 메시지를 받아서 처리
BOOL
CAgent::connect(HWND _hWnd,SOCKET _socket)
{
	if	(m_socket.getSocket()	==	_socket)
		return	m_socket.connect();

	if	(m_socketBCS.getSocket()	==	_socket)
	{
		if	(m_socketBCS.connect())
		{
			CB_LOGIN	packet;

			packet.base.set(sizeof(packet),dCB_LOGIN);
			packet.wIndex				=	m_wIndexForBCS;
			packet.wGuildIndex			=	g_hero.m_wGuildSerial;
			packet.wGuildDisplayIndex	=	g_hero.getGuildDisplayIndex();

			if	(!g_hero.getUserCustomDataCRC32(packet.dwCRC32))
				packet.dwCRC32	=	0;

			strcpy(packet.strId,m_strUserId);
			strcpy(packet.strName,m_strUserName);

			sendBCSPacket(&packet);
		}
	}

	return	FALSE;
}

//
//	FD_CLOSE 메시지를 받아서 처리
BOOL
CAgent::close(SOCKET _socket)
{
	if	(m_socket.getSocket()	==	_socket)
	{
		m_isEventDisconnect	=	TRUE;

		return	m_socket.close();
	}

	if	(m_socketBCS.getSocket()	==	_socket)
	{
		if	(m_bIsConnected2BCS)
			sendEtcWork(eCEW_DISCONNECT_BCS);

		CGamePlay::AddSystemMessage(LTYELLOW,dMSG_DISCONNECT_TO_BC_SERVER);

		m_bIsConnected2BCS	=	FALSE;

		return	m_socketBCS.close();
	}

	return	FALSE;
}

BOOL
CAgent::stackPacket(SERVER_PACKETS* _lpPacket)
{
	cMSG_BASE_TYPE	*lpBaseInfo	=	(cMSG_BASE_TYPE	*)_lpPacket;

	if	(m_bIsWaitSSLAuthenticationCode)
	{
		if	(lpBaseInfo->wType	==	dSM_COMPLETESSL)
			ReceiveSSLAuthenticationCode(_lpPacket);
		else
		{
			disconnect();	//	연결을 끊고
			m_isEventDisconnect	=	TRUE;
			g_eh.addStaticLog("CAgent::stackPacket");

			strcpy(m_strDisconnectText,dMSG_DISCONNECT_WITH_SERVER);
		}

		return	TRUE;
	}

	if	(lpBaseInfo->wType	==	dSG_PACKET_TRAILER)	//	패킷 트레일러다.
	{
		SG_PACKET_TRAILER	*lpPacketTrailer	=	(SG_PACKET_TRAILER	*)_lpPacket;

		char	*lpCurrentBufferPoint	=	lpPacketTrailer->m_aData;
		int		iCount					=	lpPacketTrailer->m_iCount,iPoint	=	0;

		for (int i=0;i<iCount;i++)
		{
			SERVER_PACKETS*	lpPacket	=	(SERVER_PACKETS*)(lpCurrentBufferPoint+iPoint);

			m_quePacket.addPacket(lpPacket);

			iPoint		+=	lpPacket->base.wSize;
		}

		return	TRUE;
	}

	m_quePacket.addPacket(_lpPacket);

	return	TRUE;
}

//
//	FD_READ 메시지를 받아서 처리
BOOL
CAgent::receive(SOCKET _socket)
{
	if	(m_socketBCS.getSocket() ==	_socket)
	{
		if	(!m_socketBCS.receive())	//	제대로 왔다. 처리해 주자.
		{
			m_bIsConnected2BCS	=	FALSE;

			m_socketBCS.close();	//	연결을 끊고

			return	FALSE;
		}

		char	*lpBuffer		=	m_socketBCS.getReceiveBuffer();
		DWORD	dwReceiveMsgSize=	m_socketBCS.getReceivedMessageSize();
		DWORD	dwMsgPos		=	0;

		while(dwMsgPos	<	dwReceiveMsgSize)		//	한번에 여러개 뭉쳐서 올수도 있다.
		{
			SERVER_PACKETS*	_lpPacket=	(SERVER_PACKETS*)(lpBuffer+dwMsgPos);

			int	iRemainBufferSize	=	dwReceiveMsgSize-dwMsgPos;	//	남은 메시지 버퍼 사이즈

			if (iRemainBufferSize	<	_lpPacket->base.wSize)	//	남은 메시지 버퍼 사이즈가 지금 처리 할 패킷 사이즈 보다 작다.
			{
				m_socketBCS.setReceiveBufferPoint(dwMsgPos,iRemainBufferSize);	//	리시브 버퍼 비우기

				return	TRUE;
			}

			dwMsgPos		+=	_lpPacket->base.wSize;

			if	(!stackPacket(_lpPacket))
				return	FALSE;
		}

		m_socketBCS.emptyReceiveBuffer();	//	리시브 버퍼 비우기

		return	TRUE;
	}

	if	(m_socket.getSocket() ==	_socket)
	{
		m_wIsRequireReceivePacket	=	FALSE;

		if	(m_bIsWaitHandShakeData)
		{
	//	SSL Handshake 과정
			int		iRetVal,iReceiveSize,iSendSize,iHandShakeError;

			iReceiveSize	=	recv(m_socket.m_socket,m_aSSLReceiveBuffer,dPACKET_BUFFER_SIZE,0);
			iHandShakeError	=	GetLastError();
			if (iHandShakeError)
				g_eh.addStaticLog("recv error %d",iHandShakeError);

			iRetVal			=	gen_handshake(&m_ssl,m_aSSLSendBuffer,dPACKET_BUFFER_SIZE);
			iHandShakeError	=	GetLastError();
			if (iHandShakeError)
				g_eh.addStaticLog("gen_handshake error %d",iHandShakeError);

			iSendSize		=	send(m_socket.m_socket,m_aSSLSendBuffer,iRetVal,0);
			iHandShakeError	=	GetLastError();
			if (iHandShakeError)
				g_eh.addStaticLog("send Error %d",iHandShakeError);

			iRetVal			=	process_handshake(&m_ssl,m_aSSLReceiveBuffer,iReceiveSize);
			iHandShakeError	=	GetLastError();
			if (iHandShakeError)
				g_eh.addStaticLog("process_handshake %d",iHandShakeError);

			m_bIsWaitHandShakeData	=	FALSE;

			return	TRUE;
		}

		if	(!m_socket.receive())	//	제대로 왔다. 처리해 주자.
		{
			disconnect();	//	연결을 끊고
			m_isEventDisconnect	=	TRUE;
			g_eh.addStaticLog("CAgent::receive");

			strcpy(m_strDisconnectText,dMSG_DISCONNECT_WITH_SERVER);

			return	FALSE;
		}

		char	*lpBuffer		=	m_socket.getReceiveBuffer();
		DWORD	dwReceiveMsgSize=	m_socket.getReceivedMessageSize();
		DWORD	dwMsgPos		=	0;

		if	(m_wIsUseSSL)
		{
			while(dwMsgPos	<	dwReceiveMsgSize)
			{
				DWORD	dwPacketSize		=	*(DWORD *)(lpBuffer+dwMsgPos+6);
				int		iRemainBufferSize	=	dwReceiveMsgSize-dwMsgPos;	//	남은 메시지 버퍼 사이즈
				int		iCurrentPacketSize	=	dwPacketSize+10;

				if (dwReceiveMsgSize	<	(DWORD)iCurrentPacketSize)	//	패킷이 중간에 짤렸다.
				{
					m_socket.setReceiveBufferPoint(dwMsgPos,iRemainBufferSize);	//	리시브 버퍼 비우기

					return	TRUE;
				}

				int	iReceiveSize		=	decrypt_msg(&m_ssl,m_aSSLReceiveBuffer,lpBuffer+dwMsgPos,dPACKET_BUFFER_SIZE,iCurrentPacketSize,FALSE);

				SERVER_PACKETS*	_lpPacket=	(SERVER_PACKETS*)m_aSSLReceiveBuffer;

				stackPacket(_lpPacket);

				dwMsgPos				+=	iCurrentPacketSize;
			}
		}
		else
		{
			while(dwMsgPos	<	dwReceiveMsgSize)		//	한번에 여러개 뭉쳐서 올수도 있다.
			{
				SERVER_PACKETS*	_lpPacket=	(SERVER_PACKETS*)(lpBuffer+dwMsgPos);

				int	iRemainBufferSize	=	dwReceiveMsgSize-dwMsgPos;	//	남은 메시지 버퍼 사이즈

				if (iRemainBufferSize	<	_lpPacket->base.wSize)	//	남은 메시지 버퍼 사이즈가 지금 처리 할 패킷 사이즈 보다 작다.
				{
					m_socket.setReceiveBufferPoint(dwMsgPos,iRemainBufferSize);	//	리시브 버퍼 비우기

					return	TRUE;
				}

				dwMsgPos		+=	_lpPacket->base.wSize;

				if	(!stackPacket(_lpPacket))
					return	FALSE;
			}
		}

		m_socket.emptyReceiveBuffer();	//	리시브 버퍼 비우기

		return	TRUE;
	}

	return	TRUE;
}

//
//	이벤트 핸들러..
BOOL
CAgent::handleWindowMessage(HWND _hWnd,int _iEvent,SOCKET _socket)
{
	switch(_iEvent)
	{	
		case FD_CONNECT		:	// --------------------------- 연결요청 메시지
			return	connect(_hWnd,_socket);

		case FD_CLOSE		:	// --------------------------- 소켓 종료 메시지
		{
//			cMSG::Put("에러","소켓이 종료되었습니다.");
			return	close(_socket);
		}

		case FD_READ		:	// --------------------------- 데이터 수신 메시지
			return	receive(_socket);

		default:
			break;
	}

	return	TRUE;
}

//
//	등록
void
CAgent::registerHandler(int _iPacketType,void (*_lpFp)(SERVER_PACKETS*))
{
	fpPacketHandler[_iPacketType-dPACKET_INDEX_BEGIN]	=	_lpFp;
}

void
CAgent::initPacketOperator()
{
	for (int i=0;i<dMAX_SERVER_PACKET_COUNT;i++)	fpPacketHandler[i]	=	NULL;

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃로그인 서버
	registerHandler(dSL_RESULT_LOGIN			,ResultLogin);
	registerHandler(dSL_LOGIN_RESULT_OTP		,ResultLoginOTP);
	
	registerHandler(dSL_SERVER_LIST				,ServerList);
	registerHandler(dSL_AVATAR_LIST				,AvatarList);
	registerHandler(dSL_RESULT_CREATE_AVATAR	,ResultCreateAvatar);
	registerHandler(dSL_RESULT_REMOVE_AVATAR	,ResultRemoveAvatar);
	registerHandler(dSL_RESULT_SELECT_AVATAR	,ReceiveSelectAvatarResult);
	registerHandler(dSL_OUT						,Disconnect);
	registerHandler(dSL_RESULT_OF_COPY_AVATAR	,ResultOfCopyAvatar);
	
//┃로그인 서버
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃게임 서버
	registerHandler(dSG_RESULT_CONNECT			,ResultGameConnect);
	registerHandler(dSG_BCS_INFO				,ReceiveBCSInfo);
	registerHandler(dSG_RESULT_JOIN				,ResultJoin);
	registerHandler(dSG_FAILED_TO_JOIN			,ReceiveFailedToJoin);

	registerHandler(dSG_PLAYER_INFO				,ReceivePlayerInfo);
	registerHandler(dSG_SUCCESS_TO_JOIN_GAME	,ReceiveSuccessToJoinGame);
	registerHandler(dSG_ENTER_TO_THE_SECRET_DUNGEON	,ReceiveEnterToTherSecretDungeon);

	registerHandler(dSG_OUT						,Disconnect);
	registerHandler(dSG_JOIN_PLAYER				,ReceiveJoinPlayerMessage);

	registerHandler(dSG_SIMPLE_ACTOR_INFO_LIST	,ReceiveSimpleActorInfoList);
	registerHandler(dSG_VERY_SIMPLE_ACTOR_INFO_LIST,ReceiveVerySimpleActorInfoList);
	registerHandler(dSG_SIMPLE_ACTOR_INFO		,ReceiveSimpleActorInfo);
	registerHandler(dSG_VERY_SIMPLE_ACTOR_INFO	,ReceiveVerySimpleActorInfo);

	registerHandler(dSG_MINIMUM_ACTOR_INFO_LIST	,ReceiveMinimumActorInfoList);
	registerHandler(dSG_OPERATOR_LIST			,ReceiveOperatorList);
	registerHandler(dSG_REFIT_ACTORS_POSITION	,ReceiveRefitActorsPostion);

	registerHandler(dSG_OUTER_ACTORS_POS_UPDATE	,ReceiveOuterActorsPosUpdate);
	registerHandler(dSG_EVENT_NPC_LIST			,ReceiveEventNpcList);

	registerHandler(dSG_MOVE					,Move);
	registerHandler(dSG_MOVE_FOR_PARTY_MEMBER	,Move);
	registerHandler(dSG_MOVE_RESULT				,MoveResult);
	registerHandler(dSG_MOVE_FAILED				,ReceiveMoveFailedMessage);
	registerHandler(dSG_CORRECT_PLAYER_POSITION	,CorrectPlayerPosition);
	registerHandler(dSG_STOP					,Stop);
	registerHandler(dSG_PLAYER_OUT				,ReceivePlayerOutMessage);
	registerHandler(dSG_SET_MOVE_ABILITY		,SetMoveAbility);
	registerHandler(dSG_TRANSFORMATION			,Transformation);
	registerHandler(dSG_TRANSFORMATION_FAILED	,TransformationFailed);
	registerHandler(dSG_TOGGLE_SITDOWN			,ToggleSitdown);
	registerHandler(dSG_SITDOWN_RESULT			,SitdownResult);

	registerHandler(dSG_REGEN_MONSTER			,RegenMonster);
	registerHandler(dSG_BASIC_ATTACK			,BasicAttack);
	registerHandler(dSG_LAND_MARKER				,ReceiveLandMarkerAttack);
	registerHandler(dSG_STRIKE_INTERVAL_SHOOTER	,ReceiveStrikeIntervalShooter);
	registerHandler(dSG_CAST_INTERVAL_SHOOTER	,ReceiveCastIntervalShooter);
	registerHandler(dSG_GUARDIAN_POST			,ReceiveGuardianPost);

	registerHandler(dSG_HIT_CHANED_DAMAGE		,ReceiveHitChanedDamage);
	registerHandler(dSG_CONTINUOUS_HIT_BY_CHARACTER,ContinuousHitAttack);
	registerHandler(dSG_CONTINUOUS_HIT_BY_VALUE	,ReceiveContinuousHitAttackByValue);


	registerHandler(dSG_ACTION_TO_ACTOR			,ActionToActor);
	registerHandler(dSG_ACTION_TO_GROUP			,ReceiveActionToGroup);
	registerHandler(dSG_ACTION_TO_GROUP_STICKED_BIT		,ReceiveActionToGroupStickedBit);
	registerHandler(dSG_ACTION_TO_GROUND		,ActionToGround);
	registerHandler(dSG_QUICK_ACTION			,QuickAction);
	registerHandler(dSG_USE_SKILL_RESULT		,UseSkillResult);
	registerHandler(dSG_MISS					,ReceiveMiss);
	registerHandler(dSG_WIDE_AREA_DAMAGE		,WideAreaDamage);
	registerHandler(dSG_WIDE_AID_ATTACK_RESULT	,WideAidAttackResult);
	
	registerHandler(dSG_WIDE_HEAL				,WideHeal);
	
	registerHandler(dSG_KILL					,Kill);
	registerHandler(dSG_UPDATE_EXP				,UpdateExp);
	registerHandler(dSG_UPDATE_PET_EXP			,UpdatePetExp);
	registerHandler(dSG_UPDATE_BASIC_INFORMATION,UpdateBasicInformation);
	registerHandler(dSG_REMOVE_ACTOR			,RemoveActor);
	registerHandler(dSG_LEVEL_UP				,LevelUp);
	registerHandler(dSG_LEVEL_UP_OTHER			,LevelUpOther);
	registerHandler(dSG_CHANGE_STATE_RESULT		,ChangeStatePower);
	registerHandler(dSG_INCREASE_ABILITY_LEVEL	,IncreaseAbilityLevel);
	registerHandler(dSG_MOVE_FIELD_RESULT		,MoveFieldResult);
	registerHandler(dSG_MOVE_FIELD_NOW			,ReceiveMoveFieldNow);
	registerHandler(dSG_SET_REACTION_STATUS		,ReceiveSetReactionStatus);
	registerHandler(dSG_HIT_THORN_DAMAGE		,ReceiveHitThornDamage);
	registerHandler(dSG_ABSORB_DAMAGE			,ReceiveAbsorbDamage);
	registerHandler(dSG_CHANGE_SPECIAL_SKILL_FIELD,ReceiveChangeSpecialSkillField);
	registerHandler(dSG_REVIVE					,ReceiveRevive);
	registerHandler(dSG_FIRST_AID				,ReceiveFirstAid);
	
	registerHandler(dSG_UPDATE_BOOST_COUNT		,ReceiveUpdateBoostCount);
	
	registerHandler(dSG_SET_SHIMMERING_SHIELD	,ReceiveSetShimmeringShield);
	registerHandler(dSG_RELEASE_SHIMMERING_SHIELD,ReceiveReleaseShimmeringShield);
	registerHandler(dSG_CURE_ACTOR				,ReceiveCureActor);
	registerHandler(dSG_DEATH_PENELTY_SECOND	,ReceiveDeathPeneltySecond);	//	길드전 관련 공지
	registerHandler(dSG_QUESTION_FOR_RECOVER_DEATH_PENELTY	,ReceiveQuestionForRecoverDeathPenelty);	//	길드전 관련 공지


	registerHandler(dSG_ADD_PORTAL				,ReceiveAddPortal);
	registerHandler(dSG_REMOVE_PORTAL			,ReceiveRemovePortal);
	registerHandler(dSG_PORTAL_LIST				,ReceivePortalList);
	registerHandler(dSG_CHANGE_DOOR_STATUS		,ReceiveChangeDoorStatus);
	registerHandler(dSG_DOOR_MESSAGE			,ReceiveDoorMessage);
	registerHandler(dSG_DOORS_STATUS_IN_FIELD	,ReceiveDoorsStatusInField);

	registerHandler(dSG_REDISTRIBUTE			,ReceiveRedistribute);
	registerHandler(dSG_STOP_BATTLE				,ReceiveStopBattle);


	registerHandler(dSG_FLOAT_MACHINE			,ReceiveFloatMachineMessage);
	registerHandler(dSG_RELEASE_MACHINE			,ReceiveReleaseMachineMessage);
	registerHandler(dSG_SHOOT_MACHINE_MISSILE	,ReceiveShootMachineMessage);
	registerHandler(dSG_FIRE_TAIL_CHASER		,ReceiveTailChaser);
	registerHandler(dSG_TURN_ON_PASSIVE_SKILL	,ReceiveTurnOnPassiveSkill);
	registerHandler(dSG_DISPLACEMENT			,ReceiveDisplacement);

	registerHandler(dSG_THRUST_ACTORS			,ReceiveThrustActors);
	registerHandler(dSG_MAKE_ACTION_FORM		,ReceiveMakeActionForm);
	registerHandler(dSG_USE_SKILL_TO_ACTOR		,ReceiveUseSkillToActor);
	registerHandler(dSG_USE_SKILL_TO_GROUND		,ReceiveUseSkillToGround);

	registerHandler(dSG_CREATE_DUMMY			,ReceiveCreateDummy);
	registerHandler(dSG_COPY_ACTOR				,ReceiveCreateDummy);	//	같은걸 사용함
	registerHandler(dSG_RELEASE_DUMMY			,ReceiveReleaseDummy);
	registerHandler(dSG_RECEIVE_AID_MAGIC		,ReceiveReceiveAidMagic);
	registerHandler(dSG_CHARGE_CP				,ReceiveChargeCP);
	registerHandler(dSG_HIT_AURA_SKILL			,ReceiveHitAuraSkill);
	registerHandler(dSG_MESSAGE					,ReceiveMessage);
	registerHandler(dSG_REMAIN_HP				,ReceiveRemainHP);
	registerHandler(dSG_THRUST					,ReceiveThrust);
	registerHandler(dSG_RUSH					,ReceiveRush);
	registerHandler(dSG_ACTIVE_REACTION_SKILL	,ReceiveActiveReactionSkill);
	registerHandler(dSG_HIDE_EQUIPMENT			,ReceiveHideEquipment);
	registerHandler(dSG_BUNSHINE_ATTACK			,ReceiveBunshineAttack);
	registerHandler(dSG_SCIMITAR_CUTTING		,ReceiveScimitarCutting);
	registerHandler(dSG_ICY_STALAGMITE			,ReceiveIcyStalagmite);
	registerHandler(dSG_ACTOR_ENCHANTED_STATUS	,ReceiveActorEnchantedStatus);
	registerHandler(dSG_TELEPORT				,ReceiveTeleport);
	registerHandler(dSG_APPLY_SKILL_EXTRA_EFFECT,ReceiveApplyExtraSkillEffect);
	registerHandler(dSG_ACTIVE_SKILL_EFFECT		,ReceiveActiveSkillEffect);
	registerHandler(dSG_ACTIVE_MIRROR_TOWER		,ReceiveActiveMirrorTower);
	registerHandler(dSG_FIND_USER_RESULT		,ReceiveFindUserResult);
	registerHandler(dSG_SIMPLE_HIT				,ReceiveSimpleHit);
	registerHandler(dSG_HIT_DAMAGE				,ReceiveHitDamage);
	registerHandler(dSG_SKILL_EFFECT			,ReceiveSkillEffect);
	registerHandler(dSG_PLAY_SKILL_SOUND		,ReceivePlaySkillSound);
	registerHandler(dSG_STRIKE_LIGHTNING_WINDER	,ReceiveStrikeLightningWinder);
	registerHandler(dSG_STRIKE_MULTI_MISSILE	,ReceiveStrikeMultiMissile);	// 멀티미사일.

	registerHandler(dSG_REGEN_HP_BY_AID_SKILL	,ReceiveRegenHPByAidSkill);
	registerHandler(dSG_RECOVER_HP_BY_AID_SKILL	,ReceiveRecoverHPByAidSkill);
	registerHandler(dSG_EXIT_GAME_SUCCESS		,ReceiveExitGameSuccess);
	registerHandler(dSG_REGIST_MESSAGE			,ReceiveRegistMessage);
	registerHandler(dSG_HIT_ACTION				,ReceiveHitAction);
	registerHandler(dSG_DETECTING_EVIL			,ReceiveDetectingEvil);
	registerHandler(dSG_READY_TO_WARP_TO_NEAREST_VILLAGE	,ReceiveReadyToWarpToNearestVillage);

	registerHandler(dSG_ACCOUNT_INFO			,ReceiveAccountInfo);
	registerHandler(dSG_GG_AUTH_DATA			,ReceiveGGAuthCode);
	registerHandler(dSG_HS_GUID					,ReceiveHSGuid);
	registerHandler(dSG_HS_REQ_MSG				,ReceiveHSReqMsg);

	
	registerHandler(dSG_WINNING_PRIZE_LOTTO		,ReceiveWinningPrizeLotto);
	registerHandler(dSG_ARCA_LIST				,ReceiveArcaList);

	registerHandler(dSG_ADD_ARCA				,ReceiveAddArca);
	registerHandler(dSG_ATTACK_TO_OBJECT		,ReceiveAttackToObject);
	registerHandler(dSG_ARCA_WORK				,ReceiveArcaWork);
	registerHandler(dSG_CLOSE_RANGE_ARCA_WORK	,ReceiveArcaWork);
	registerHandler(dSG_BROKEN_ARCA_ITEM_BY_ATTACK	,ReceiveBrokenArcaItemByAttack);

	registerHandler(dSG_PLAY_SOUND				,ReceivePlaySound);
	registerHandler(dSG_ETC_WORK				,ReceiveEtcWork);
	registerHandler(dSG_CHECK_RANGE_ETC_WORK	,ReceiveEtcWork);
	registerHandler(dSG_SET_ANM					,ReceiveSetAnm);

	registerHandler(dSG_DUEL_RESULT				,ReceiveDuelResult);
	registerHandler(dSG_DUEL_RANKING			,ReceiveDuelRanking);
	registerHandler(dSG_DUEL_RECORD				,ReceiveDuelRecord);	
	registerHandler(dSG_TRY_DUEL_BATTLE			,ReceiveTryDuelBattle);	
	registerHandler(dSG_DUEL_MSG				,ReceiveDuelMsg);	
	

//━펫 관련
	registerHandler(dSG_PARTING_WITH_PET		,ReceivePartingWithPet);
	registerHandler(dSG_PET_INFO				,ReceivePetInfo);
	registerHandler(dSG_CHANGE_PET_COMMAND		,ReceiveChangePetCommand);
	registerHandler(dSG_SYNC_PET_INFO			,ReceiveSyncPetInfo);
	registerHandler(dSG_PET_WAIT_COMMAND		,ReceivePetWaitCommand);
	registerHandler(dSG_MAKE_BREEDING_RECORD_BOOK,ReceiveMakeBreedingRecordBook);
	registerHandler(dSG_CHANGE_PET_NAME			,ReceiveChangePetName);
	registerHandler(dSG_SUMMON_BEAST_INFO		,ReceiveSummonBeastInfo);
	registerHandler(dSG_RIDING_DOG				,ReceiveRidingDog);

//━거신의 은총 관련
	registerHandler(dSG_GGG_INFO				,ReceiveGGGInfo);
	registerHandler(dSG_ADD_GGG_SKILL_UP		,ReceiveGGGSkillUp);
	registerHandler(dSG_GGG_SKILL_UP_TIME_END	,ReceiveGGGSkillUpTimeEnd);
	registerHandler(dSG_ADD_GGG					,ReceiveGGG);
	registerHandler(dSG_REMOVE_GGG				,ReceiveRemoveGGG);
	registerHandler(dSG_PLAY_GGG_EFFECT			,ReceiveGGGEffectPlay);

//━함정관련
	registerHandler(dSG_REMOVE_TRAP				,ReceiveRemoveTrap);
	registerHandler(dSG_ADD_TRAP				,ReceiveAddTrap);
	registerHandler(dSG_EXPLOSION_TRAP			,ReceiveExplosionTrap);
	registerHandler(dSG_EXPLOSION_TRAP_SKILL	,ReceiveExplosionTrapSkill);
	registerHandler(dSG_EXIST_TRAP_LIST			,ReceiveExistTrapList);
	registerHandler(dSG_EXIST_FIRE_WALL_LIST	,ReceiveExistFireWallList);

	
	registerHandler(dSG_EXIST_ACTIVE_FIELD_TRAP_LIST,ReceiveExistActiveFieldTrapList);
	registerHandler(dSG_FIELD_TRAP_WORK			,ReceiveFieldTrapWork);
	registerHandler(dSG_CLOSE_RANGE_FIELD_TRAP_WORK,ReceiveFieldTrapWork);
	
	registerHandler(dSG_SECRET_DUNGEON_MESSAGE	,ReceiveSecretDungeonMessage);

//━커뮤니티
	registerHandler(dSG_CHAT					,Chat);
	registerHandler(dSG_CHAT_FAILED				,ReceiveChatFailed);
	registerHandler(dSG_SAY_RESULT				,ReceiveSayResult);

	//파티 연합 관련
	registerHandler(dSG_ASK_JOIN_UNION_PARTY		,ReceiveAskUnionJoinParty);//	연합 파티 참가 요청
	registerHandler(dSG_REPLAY_JOIN_UNION_PARTY		,ReceiveReplayUnionJoinParty);//	연합 파티 참가 결과
	registerHandler(dSG_UNION_PARTY_MEMBER_INFO		,ReceiveUnionPartyMemberInfo);//	연합 파티 참가 결과
	registerHandler(dSG_UNION_PARTY_INFO			,ReceiveUnionPartyInfo);//	연합 파티 참가 결과
	registerHandler(dSG_DISSOLUTION_UNION_PARTY		,ReceiveUnionPartyDissolution);

	registerHandler(dSG_CHANGE_PARTY_STATUS		,ReceiveChangePartyStatus);//	파티리스트 전송
	registerHandler(dSG_PARTY_LIST				,ReceivePartyList);//	파티리스트 전송
	registerHandler(dSG_PARTY_INFO				,ReceivePartyInfo);//	파티멤버에게 보내는 파티 정보(파티원 리스트나 기타등등)
	registerHandler(dSG_ASK_JOIN_PARTY			,ReceiveAskJoinParty);//	파티 참가 요청
	registerHandler(dSG_JOIN_PARTY_MEMBER		,ReceiveJoinPartyMember);//	파티 멤버 추가
	registerHandler(dSG_PARTY_WORK				,ReceivePartyWork);//	누군가 파티에서 탈퇴 했다.(거나 쫏겨 났거나.. -_-a)
	registerHandler(dSG_UPDATE_PARTY_MEMBER_INFO,ReceiveUpdatePartyMemberInfo);//	파티 멤버 정보를 업데이트(체력정도..)
	registerHandler(dSG_UPDATE_PARTY_MEMBER_HP	,ReceiveUpdatePartyMemberHP);//	파티 멤버 정보를 업데이트(체력정도..)
	registerHandler(dSG_CHANGE_PARTY_NAME		,ReceiveChangePartyName);//	파티 이름 변경
	registerHandler(dSG_PARTY_MESSAGE			,ReceivePartyMessage);//	파티와 관련된 이런저런 메시지(넌 리더가 아냐!!,넌 파티에 속해 있지 않아!!,파티 풀~ 등등등..)
	registerHandler(dSG_DISSOLUTION_PARTY		,ReceiveDissolutionParty);//	파티와 관련된 이런저런 메시지(넌 리더가 아냐!!,넌 파티에 속해 있지 않아!!,파티 풀~ 등등등..)

	registerHandler(dSG_REQUEST_ADD_FRIEND		,ReceiveRequestAddFriend);//	친구 추가 요청 메시지
	registerHandler(dSG_REPLY_FOR_ADD_FRIEND	,ReceiveReplyForAddFriend);//	친구 추가 요청 메시지에 대한 응답
	registerHandler(dSG_PICK_PARTY_ITEM			,ReceivePickPartyItem);//	친구 추가 요청 메시지에 대한 응답

	registerHandler(dSG_ASK_REMOVE_FRIEND		,ReceiveAskRemoveFriend);//	친구 목록에서 제거 요청 메시지
	registerHandler(dSG_ASK_REMOVE_FRIEND_RESULT,ReceiveAskRemoveFriendResult);//	친구 목록에서 제거 요청의 결과

//━노점상 관련
	registerHandler(dSG_OPEN_PITCHMAN_SHOP					,ReceiveOpenPitchmanShop);			//	노점상 오픈
	registerHandler(dSG_ADD_PITCHMAN_SHOP_ITEM				,ReceiveAddPitchmanShopItem);//	노점상에 아이템 추가
	registerHandler(dSG_REMOVE_PITCHMAN_SHOP_ITEM			,ReceiveRemovePitchmanShopItem);//	노점상에 아이템 제거
	registerHandler(dSG_CHANGE_PITCHMAN_SHOP_INFO			,ReceiveChangePitchmanShopInfo);//	노점상 상태 변경
	registerHandler(dSG_CHANGE_PITCHMAN_SHOP_ITEM_PLACE		,ReceiveChangePitchmanShopItemPlace);	//	노점상 아이템 위치 변경

	registerHandler(dSG_BUY_PITCHMAN_SHOP_ITEM				,ReceiveBuyPitchmanShopItem);//	노점상에서 아이템 구입
	registerHandler(dSG_SOLD_PITCHMAN_SHOP_ITEM				,ReceiveSoldPitchmanShopItem);//	노점상에서 아이템이 팔렸다.
	registerHandler(dSG_ADD_PITCHMAN_SHOP					,ReceiveAddPitchmanShop);//	노점상 추가
	registerHandler(dSG_REMOVE_PITCHMAN_SHOP				,ReceiveRemovePitchmanShop);//	노점상 제거
	registerHandler(dSG_PITCHMAN_SHOP_LIST					,ReceivePitchmanShopList);//	노점상 리스트
	registerHandler(dSG_SIMPLE_PITCHMAN_SHOP_INFO_LIST		,ReceiveSimplePitchmanShopInfoList);//	노점상 리스트
	
	registerHandler(dSG_PITCHMAN_SHOP_INFO					,ReceivePitchmanShopInfo);//	노점상 정보
	registerHandler(dSG_PITCHMAN_SHOP_MESSAGE				,ReceivePitchmanShopMessage);//	노점상 메시지
	registerHandler(dSG_RESULT_OF_SEARCH_ITEM_IN_RELAY_STATION	,ReceiveResultOfSearchItemInRelayStation);//	노점상 메시지

//━길드 관련

	
	registerHandler(dSG_GUILD_PET_AND_GUARDIAN_EXP			,ReceiveGuildGuardianAndPetExp);
	registerHandler(dSG_SIEGE_WARFARE_NOTICE				,ReceiveSiegeWarfareNotice);
	registerHandler(dSG_STORE_GUILD_INVENTORY_ITEM			,ReceiveStoreGuildInventoryItem);
	registerHandler(dSG_REMOVE_GUILD_INVENTORY_ITEM			,ReceiveRemoveGuildInventoryItem);
	registerHandler(dSG_STORE_GUILD_INVENTORY_GOLD			,ReceiveStoreGuildInventoryGold);
	registerHandler(dSG_REMOVE_GUILD_INVENTORY_GOLD			,ReceiveRemoveGuildInventoryGold);
	registerHandler(dSG_CHANGE_GUILD_INVENTORY_ITEM_PLACE	,ReceiveChangeGuildInventoryItemPlace);

	registerHandler(dSG_OPEN_GUILD_INVENTORY		,ReceiveOpenGuildInventory);	//	길드 스케쥴러 오픈
	registerHandler(dSG_OPEN_GUILD_HALL_LIST		,ReceiveOpenGuildHallList);	//	길드 스케쥴러 오픈
	registerHandler(dSG_OPEN_RELATED_PLACE_LIST		,ReceiveOpenRelatedPlaceList);	//	길드 스케쥴러 오픈
	registerHandler(dSG_GUILD_HONOR_POINT_LIST		,ReceiveGuildHonorPointList);	//	길드 스케쥴러 오픈
	registerHandler(dSG_GUILD_MASTER_GRACE_INFO		,ReceiveGuildMasterGraceInfo);	//	길드 마스터 은총 부여
	registerHandler(dSG_GVG_WORK_SUCCESS			,ReceiveGVGWorkSuccess);	//	길드 마스터 은총 부여

	registerHandler(dSG_OPEN_GUILD_BATTLE_SCHEDULER	,ReceiveOpenGuildScheduler);	//	길드 스케쥴러 오픈
	registerHandler(dSG_OPEN_GUILD_MARK_COMPOSER	,ReceiveOpenGuildMarkComposer);	//	길드 마크 조합 인터페이스 오픈
	registerHandler(dSG_GUILD_BATTLE_SCHEDULE_IN_SAME_TIME,ReceiveGuildBattleScheduleInSameTime);	//	길드 스케쥴러 오픈
	registerHandler(dSG_CHANGE_GUILD_BATTLE_SCHEDULE,ReceiveChangeGuildSchedule);	//	길드 스케쥴 변경됨
	registerHandler(dSG_UPDATE_GUILD_BATTLE_COUNT_AT_THIS_WEEK,ReceiveUpdateGuildBattleCountAtThisWeek);	//	한주동안 싸운 횟수 업데이트
	registerHandler(dSG_READY_TO_NEXT_GUILD_BATTLE	,ReceiveReadyToNextGuildBattle);	//	다음 길드전 준비 메시지 받음
	registerHandler(dSG_GUILD_BATTLE_RESULT			,ReceiveGuildBattleResult);	//	다음 길드전 준비 메시지 받음
	registerHandler(dSG_GUILD_BATTLE_NOTICE			,ReceiveGuildBattleNotice);	//	길드전 관련 공지
	registerHandler(dSG_GUILD_BATTLE_INFO			,ReceiveGuildBattleInfo);	//	길드전 관련 공지
	registerHandler(dSG_GUILD_BATTLE_KILL_NOTICE	,ReceiveGuildBattleKillNotice);	//	길드전 관련 공지

	registerHandler(dSG_READY_TO_MAKE_GUILD		,ReceiveReadyToMakeGuild);	//	길드 맹글기 준비!!
	registerHandler(dSG_MAKE_GUILD_RESULT		,ReceiveCreateGuildResult);	//	길드 맹글기 결과
	registerHandler(dSG_BASE_GUILD_INFO			,ReceiveBaseGuildInfo);		//	길드 맹글기 결과
	registerHandler(dSG_SIMPLE_GUILD_INFO		,ReceiveSimpleGuildInfo);		//	길드 맹글기 결과
	registerHandler(dSG_UPDATE_GUILD_BATTLE_INFO_FOR_MEMBER,ReceiveGuildBattleInfoForMember);	//	길드전 정보 받기
	registerHandler(dSG_GUILD_MARK_INFO			,ReceiveGuildMarkInfo);	//	길드마크 정보 받기

	registerHandler(dSG_DISSOLUTION_GUILD		,ReceiveGuildDissolution);	//	길드 해체
	registerHandler(dSG_ASK_JOIN_GUILD_RESULT	,ReceiveAskJoinGuildResult);//	길드 참가신청 결과
	registerHandler(dSG_ASK_JOIN_GUILD			,ReceiveAskJoinGuild);		//	길드 참가신청

	registerHandler(dSG_JOIN_GUILD				,ReceiveJoinGuild);			//	길드 참가?
	registerHandler(dSG_EXIT_GUILD				,ReceiveLeaveGuild);		//	길드 탈퇴
	registerHandler(dSG_UPDATE_GUILD_LEVEL		,ReceiveUpdateGuildLevel);	//	길드 레벨 업데이트
	registerHandler(dSG_GUILD_WORK_MESSAGE		,ReceiveGuildWorkMessage);	//	길드 메시지
	registerHandler(dSG_CHANGE_GUILD_TAX_RATE	,ReceiveChangeGuildExpTax);//	경험치 세율 변경
	registerHandler(dSG_CHANGE_GUILD_NOTICE		,ReceiveChangeGuildNotice);//	길드 공지 변경
	registerHandler(dSG_CHANGE_RANK_IN_GUILD	,ReceiveChangeRankInGuild);//	길드 멤버 랭크 변경
	registerHandler(dSG_GUILD_MEMBER_LIST		,ReceiveGuildMemberList);		//	길드 멤버리스트 받기

	registerHandler(dSG_UPDATE_GUILD_MEMBER		,ReceiveUpdateGuildMemberInfo);//	길드 멤버 정보 업데이트
	registerHandler(dSG_REMOVE_GUILD_MEMBER		,ReceiveRemoveGuildMember);//	길드 멤버 제거

	registerHandler(dSG_UPDATE_GUILD_SKILL_LEVEL,ReceiveUpdateGuildSkillLevel);
	registerHandler(dSG_CHANGE_GUILD_MARK		,ReceiveChangeGuildMark);


	
//━아이템 관련
	registerHandler(dSG_DROPPING_ITEMS			,DroppingItems);
	registerHandler(dSG_REPAIR_ITEM				,ReceiveRepairItem);
	registerHandler(dSG_OPEN_REPAIR_ITEM_WINDOW	,ReceiveOpenRepairItemWindow);
	registerHandler(dSG_OPEN_DISJOINTING_DATA	,ReceiveOpenItemDisjointingWindow);//아이템 분해
	registerHandler(dSG_ITEM_WORK_RESULT		,ReceiveItemWorkResult);
	registerHandler(dSG_DROPPING_ITEM			,DroppingItem);
	registerHandler(dSG_STRIP_EQUIPMENT			,StripEquipment);
	registerHandler(dSG_REMOVE_DROPPED_ITEMS	,RemoveDroppedItems);
	registerHandler(dSG_REMOVE_DROPPED_ITEM		,RemoveDroppedItem);
	registerHandler(dSG_CHANGE_DROPPED_ITEM_INFO,ChangeDroppedItemInfo);
	registerHandler(dSG_PCIK_PARTY_GOLD			,PickItemResult);
	registerHandler(dSG_PCIK_PARTY_ITEM			,ReceivePickPartyGold);
	registerHandler(dSG_UPDATE_ITEM_DURABILITY	,ReceiveUpdateItemDurability);//아이템 내구력

	registerHandler(dSG_PICK_ITEM_RESULT		,PickItemResult);
	registerHandler(dSG_EQUIP_ITEM_RESULT		,EquipItemResult);
	registerHandler(dSG_RELOAD_BELT_ITEM		,ReloadBeltItem);
	registerHandler(dSG_BELT_ITEM_RELOAD_RESULT	,ReceiveBeltItemReloadResult);
	registerHandler(dSG_USE_ITEM_RESULT			,UseItemResult);
	registerHandler(dSG_LOTTERY_TICKET_PRIZE_WINNING,ReceiveRotteryTicketPrizeWinning);
	registerHandler(dSG_USE_BELT_ITEM_RESULT	,UseBeltItemResult);
	registerHandler(dSG_USE_ITEM_TO_ITEM_RESULT	,ReceiveUseItemToItemResult);
	registerHandler(dSG_USE_ITEM				,UseItem);
	registerHandler(dSG_ITEM_PACK_DATA			,OpenSelectItemInPackWindow);
	
	registerHandler(dSG_OTHER_PLAYER_ITEM_DATA	,OtherPlayerItemData);
	registerHandler(dSG_ITEM_DATA				,ItemData);
	registerHandler(dSG_EQUIPMENT_DATA			,EquipmentData);
	registerHandler(dSG_CHANGE_APPEARANCE		,ChangeAppearance);
	registerHandler(dSG_DROP_ITEM_RESULT		,DropItemResult);
	registerHandler(dSG_CHANGE_ITEM_PLACE_RESULT,ChangeItemPlaceResult);
	registerHandler(dSG_STRIP_EQUIPMENT_RESULT	,ReceiveStripEquipmentResult);

	registerHandler(dSG_DECREASE_ITEM_COUNT		,ReceiveDecreaseItemCount);
	registerHandler(dSG_ADD_ITEM				,ReceiveAddItem);
	registerHandler(dSG_REMOVE_ITEM				,ReceiveRemoveItem);
	
	registerHandler(dSG_ADD_GOLD				,ReceiveAddGold);
	registerHandler(dSG_SET_ITEM_UNIQUE_SERIAL	,ReceiveSetItemUniqueSerial);
	registerHandler(dSG_UPDATE_ITEM_DATA		,ReceiveUpdateItemData);
	registerHandler(dSG_UPDATE_WAY_POINT_INFO	,ReceiveUpdateWayPointInfo);
	registerHandler(dSG_CURE_ACTOR_BY_POTION	,ReceiveCureActorByPotion);

	registerHandler(dSG_EXPIRED_ITEM			,ReceiveExpiredItem);
	registerHandler(dSG_EXPIRED_ITEM_LIST		,ReceiveExpiredItemList);
	registerHandler(dSG_USE_ORB_RESULT			,ReceiveUseOrbResult);
	registerHandler(dSG_DESTROY_ITEM_RESULT		,ReceiveDestroyItemResult);
	registerHandler(dSG_ITEM_PACK_NAME			,ReceiveItemPackName);	//	프리미엄 아이템 가격 정보

	registerHandler(dSG_REMOVE_PREFIX			,ReceivePrefix);	//	프리미엄 아이템 가격 정보

	registerHandler(dSG_MINI_PET_INFO			,ReceiveMiniPetInfo);	
	

//━트레이드 관련
	registerHandler(dSG_REQUEST_TRADE						,ReceiveRequestTrade);	//		0x1190	//	트레이드 요청
	registerHandler(dSG_REQUEST_TRADE_RESULT				,ReceiveRequestTradeResult);	//		0x1191	//	트레이드 요청 결과
	registerHandler(dSG_BEGIN_TRADE							,ReceiveBeginTrade);	//	0x1192	//	트레이드 시작
	registerHandler(dSG_ADD_TRADE_ITEM_BY_ME				,ReceiveAddTradeItemByMe);	//	0x1193	//	트레이드 아이템 추가
	registerHandler(dSG_ADD_TRADE_ITEM_BY_TRADE_MATE		,ReceiveAddTradeItemByTradeMate);	//	0x1194	//	트레이드 아이템 추가
	registerHandler(dSG_CHANGE_TRADE_ITEM_DATA_BY_ME			,ReceiveChangeTradeItemDataByMe);	//	0x1193	//	트레이드 아이템 추가
	registerHandler(dSG_CHANGE_TRADE_ITEM_DATA_BY_TRADE_MATE	,ReceiveChangeTradeItemDataByTradeMate);	//	0x1194	//	트레이드 아이템 추가
	registerHandler(dSG_SET_TRADE_GOLD						,ReceiveSetTradeGold);	//	0x1195	//	트레이드 아이템 추가
	registerHandler(dSG_REMOVE_TRADE_ITEM					,ReceiveRemoveTradeItem);	//	0x1196	//	트레이드 아이템 제거 트레이드 아이템 제거 결과
	registerHandler(dSG_READY_TRADE							,ReceiveReadyTrade);	//	0x1197	//	트레이드 준비
	registerHandler(dSG_PERMIT_TRADE						,ReceivePermitTrade);	//	0x1198	//	트레이드 수락
	registerHandler(dSG_TRADE								,ReceiveTrade);	//	0x1199	//	트레이드
	registerHandler(dSG_TRADE_MESSAGE						,ReceiveTradeMessage);	//	0x119a	//	트레이드 메시지


//━은행 관련
	registerHandler(dSG_OPEN_BANK							,ReceiveOpenBank);
	registerHandler(dSG_BANK_MESSAGE						,ReceiveBankMessage);
	registerHandler(dSG_STORE_ITEM_TO_THE_BANK				,ReceiveStoreItemToTheBank);
	registerHandler(dSG_WITHDRAW_ITEM_FROM_THE_BANK			,ReceiveWithdrawItemFromTheBank);
	registerHandler(dSG_STORE_GOLD_TO_THE_BANK				,ReceiveStoreGoldToTheBank);
	registerHandler(dSG_WITHDRAW_GOLD_FROM_THE_BANK			,ReceiveWithdrawGoldFromTheBank);
	registerHandler(dSG_MOVE_BANK_ITEM						,ReceiveMoveBankItem);
	registerHandler(dSG_MOVE_INVENTORY_ITEM_WHEN_BANK_TRANSACTION	,ReceiveMoveInventoryItemWhenBankTransaction);


//━쇼핑 카트 관련
	registerHandler(dSG_OPEN_CART							,ReceiveOpenCart);	//	쇼핑 카트 열기
	registerHandler(dSG_CART_MESSAGE						,ReceiveCartMessage);	//	쇼핑 카트 열기
	registerHandler(dSG_WITHDRAW_ITEM_FROM_THE_CART			,ReceiveWithdrawItemFromCart);	//	쇼핑 카트 열기
	registerHandler(dSG_LOAD_FAILED_CART_PREMIUM_ITEM		,ReceiveLoadFailedCartPremiumItem);	//	쇼핑 카트 열기
	registerHandler(dSG_DESTROY_CART_ITEM_RESULT			,ReceiveDestroyCartItemResult);	//	쇼핑 카트에 아이템 제거
	registerHandler(dSG_CARROT_SHOP_INFO					,ReceiveCarrotShopInfo);	//	당근 상점 정보 받음

//━이벤트
	registerHandler(dSG_COMPLEX_SPEECH						,ReceiveComplexSpeech);
	registerHandler(dSG_EVENT_FAILED						,ReceiveEventFailed);
	registerHandler(dSG_END_DIAOLG							,ReceiveEndDialog);
	registerHandler(dSG_OPEN_SHOP							,ReceiveOpenShop);
	registerHandler(dSG_OPEN_GUILD_SHOP						,ReceiveOpenGuildShop);

	registerHandler(dSG_BUY_ITEM							,ReceiveBuyItem);
	registerHandler(dSG_FAILED_BUY_ITEM						,ReceiveFailedBuyItem);
	registerHandler(dSG_SELL_ITEM							,ReceiveSellItem);
	registerHandler(dSG_FAILED_SELL_ITEM					,ReceiveFailedSellItem);
	registerHandler(dSG_QUEST_INFO							,ReceiveQuestInfo);
	registerHandler(dSG_CHANGE_QUEST_INFO					,ReceiveChangeQuestInfo);
	registerHandler(dSG_DAMAGE_TO_ACTOR_BY_KARMA			,ReceiveDamageToActorByKarma);
	registerHandler(dSG_EVENT_NOTICE						,ReceiveEventNotice);
	registerHandler(dSG_NOTICE								,ReceiveNotice);
	registerHandler(dSG_GET_TITLE							,ReceiveGetTitle);
	registerHandler(dSG_REMOVE_TITLE						,ReceiveRemoveTitle);
	registerHandler(dSG_CANCEL_QUEST						,ReceiveCancelQuest);

	registerHandler(dSG_BEGIN_QUIZ_EVENT					,ReceiveBeginQuizEvent);
	registerHandler(dSG_ANNOUNCE_QUIZ_EVENT					,ReceiveAnnounceQuizEvent);
	registerHandler(dSG_OX_QUIZ_RESULT						,ReceiveOXQuizResult);
	registerHandler(dSG_START_COUNTDOWN						,ReceiveCountDownStart);
	registerHandler(dSG_QUIZ_EVENT_QUESTION					,ReceiveQuizEventQuestion);
	registerHandler(dSG_END_QUIZ_EVENT						,ReceiveEndQuizEvent);
	registerHandler(dSG_QUIZ_EVENT_LOSER_LIST				,ReceiveQuizEventLoserList);
	registerHandler(dSG_CANCEL_QUIZ_EVENT					,ReceiveCancelQuizEvent);
	registerHandler(dSG_OX_QUIZ_SURVIVOR_COUNT				,ReceiveOXQuizSurvivorCount);
	registerHandler(dSG_OX_QUIZ_LOSER_REVIVE_BATTLE			,ReceiveReviveOXQuizLoserBattle);
	registerHandler(dSG_OX_QUIZ_WINNER_LIST					,ReceiveOXQuizWinnerList);
	registerHandler(dSG_WORD_QUIZ_RESULT					,ReceiveWordQuizResult);
	registerHandler(dSG_SOUND_OF_LEADERS_BELL				,ReceiveSoundOfLeadersBell);
	
	registerHandler(dSG_BUY_CARROT_SHOP_ITEM_RESULT			,ReceiveBuyCarrotShopItemResult);
	registerHandler(dSG_BANNER_INFO							,ReceiveBannerInfo);
	registerHandler(dSG_BANNER_TEXT							,ReceiveBannerText);
	registerHandler(dSG_BANNER_LIST							,ReceiveBannerList);
	registerHandler(dSG_BOOST_POWER_BY_EVENT				,ReceiveBoostPowerByEvent);
	registerHandler(dSG_EVENT_SCREEN_MOVE					,ReceiveScreenMove);
	registerHandler(dSG_EVENT_TIME_UPDATE					,ReceiveEventTimeUpdate);

//━기타
	registerHandler(dSG_CONTROL_MONSTER						,ReceiveControlMonster);
	registerHandler(dSG_OCCUR_EFFECT						,ReceiveOccurEffect);
	registerHandler(dSG_PENELTY_TIME						,ReceivePeneltyTime);
	registerHandler(dSG_TITLE_EFFECT						,ReceiveTitleEffect);
	registerHandler(dSG_XTRAP_PACKET						,ReceiveXtrapPacket);
	registerHandler(dSG_UPDATE_PACKET_ENCRYT_CODE			,ReceiveUpdatePacketEncryptCode);

//┃게임 서버
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
}

//
//	에이전트 초기화
BOOL
CAgent::init(HWND _hWnd,DWORD _dwLifecodeCycle)
{
	if	(!cSOCKET::Init(_hWnd))
		return	FALSE;		//	윈속 초기화

	reset();

	m_dwLifecodeCycle	=	_dwLifecodeCycle;
	m_dwLastLifecodeTime=	timeGetTime()/1000;

	m_quePacket.reset();							//	패킷 큐 초기화

	initPacketOperator();

	return	TRUE;
}

//
//	생존 코드 전송
void
CAgent::sendLifeCode()
{
	if	(!isConnected())
		return;

	DWORD	dwTime	=	timeGetTime()/1000;

	if	(m_dwLastLifecodeTime	+	m_dwLifecodeCycle	<	dwTime)
	{
		sendDummyMessage();

		m_dwLastLifecodeTime	=	dwTime;
	}
}

const	char*
CAgent::getMacAddress() 
{
	m_strMacAddress	=	GetMacAddressByAdapter();

	if	(m_strMacAddress.length())
		return	m_strMacAddress.c_str();

	m_strMacAddress	=	GetMacAddressFromSock();

	if	(m_strMacAddress.length())
		return	m_strMacAddress.c_str();

	m_strMacAddress	=	GetMacAddressFromNetBIOS();

	return	m_strMacAddress.c_str();
}

//
//	닫기
void
CAgent::close()
{
	reset();
	disconnectBroadCastServer();

	cSOCKET::Close();
}

//
//	패킷 타임 줄이기
void
CAgent::reducePacketTime(int *_iTime,int _iPeriod)
{
	if	(*_iTime	+	dSYNC_FPS	>	s_iFrameCounter)
		*_iTime	=	*_iTime-((_iPeriod-1)*dSYNC_FPS);
	else
		*_iTime	=	0;

	if	(*_iTime < 0)
		*_iTime	=	0;
}

//
//	패킷 핸들러
BOOL
CAgent::packetHandler()
{
	SERVER_PACKETS *lpPacket;

	clearReceivedPacketCheck();
	ResetNetworkEvent();

	while(1)
	{
		lpPacket	=	popPacket();

		if	(!lpPacket)
			return	TRUE;

		int	iPacketType	=	lpPacket->base.wType;

		if	(iPacketType	>=	dBC_LOGIN_RESULT)
		{
			switch(iPacketType)
			{
				case	dBC_LOGIN_RESULT	:
				{
					BC_LOGIN_RESULT	*lpResult	=	(BC_LOGIN_RESULT	*)lpPacket;

					if	(lpResult->wResult	==	BC_LOGIN_RESULT::eREQUIRE_UPDATE_DATA)
						g_hero.setCustomData(&lpResult->data,lpResult->wFriendCount);

					g_hero.m_wBCS_Serial	=	lpResult->wBCSerial;
					m_wIndexForBCS			=	lpResult->wBCSerial;

					if	(lpResult->wResult	==	BC_LOGIN_RESULT::eFAILED)
					{
						sendEtcWork(eCEW_DISCONNECT_BCS);
#ifdef	_DEBUG
						CGamePlay::AddSystemMessage(LTGREEN,dMSG_FAILED_TO_CONNECT_BC_RETRY_CONNECT);
#endif
					}
					else
					{
						m_bIsConnected2BCS	=	TRUE;
						CGamePlay::AddSystemMessage(LTGREEN,dMSG_CONNECT_TO_BC_SERVER);
					}

					g_hero.correctCustomData();

					break;
				}

				case	dBC_UPDATE_FRIEND_ONLINE_STATUS	:
				{
					BC_UPDATE_FRIEND_ONLINE_STATUS	*lpResult	=	(BC_UPDATE_FRIEND_ONLINE_STATUS	*)lpPacket;

					g_hero.setFriendListOnlineStatus(&lpResult->onlineStatus);
					break;
				}

				case	dBC_SERVER_MESSAGE				:	//	서버 메시지
				{
					break;
				}

				case	dBC_UPDATE_PARTY_PROSPECTUS		:	//	파티 모집요강
				{
					BC_UPDATE_PARTY_PROSPECTUS	*lpProspectus	=	(BC_UPDATE_PARTY_PROSPECTUS	*)lpPacket;

					if	(g_hero.getPartySerial()	!=	lpProspectus->wPartySerial)
						break;

					CGamePlay::s_partyInfo.updatePartyProspectus(lpProspectus->bf10RecruitMemberLevelRange,lpProspectus->dwPreferenceJobMask);

					if	(lpProspectus->bf1IsIncludeText)
						CGamePlay::s_partyInfo.changePartyProspectusText(lpProspectus->strPartyProspectus);
						
					break;
				}

				case	dBC_INFO_GUILD_MEMBER_CONNECTION_LIST	:
				{
					BC_INFO_GUILD_MEMBER_CONNECTION_LIST	*lpConnectionList	=	(BC_INFO_GUILD_MEMBER_CONNECTION_LIST	*)lpPacket;
				
					CSimpleGuildMemberList	*lpList	=	&g_guild.m_memberList;
					if(!lpList)
						break;
					for(int i=0;i<lpList->m_wCount;++i)
					{
						lpList->m_aMembers[i].m_bf1IsOnline = FALSE;
						for(int j=0;j<lpConnectionList->wCount;++j)
						{
							
							if(lpConnectionList->adwHashNameCode[j] == GetHashCode(lpList->m_aMembers[i].getName()))
							{
								lpList->m_aMembers[i].m_bf1IsOnline = TRUE;
								break;
							}
						}

					}
					break;
 				}

				case	dBC_OPEN_PARTY_LIST				:
				{
					BC_OPEN_PARTY_LIST	*lpList	=	(BC_OPEN_PARTY_LIST	*)lpPacket;

					g_gwParty.receivePartyList(lpList->wCount,lpList->aList);
					break;
				}

				default	:
				{
					m_socket.emptyReceiveBuffer();
					m_socketBCS.emptyReceiveBuffer();
					m_quePacket.reset();

					CRY(1,_ms("cry in packetHandler1()- type 0x%x",lpPacket->base.wType));

					return	FALSE;
				}
			}
		}
		else
		{
			iPacketType			-=	dPACKET_INDEX_BEGIN;

			if	(iPacketType	>=	dMAX_SERVER_PACKET_COUNT || iPacketType < 0	||	fpPacketHandler[iPacketType]==NULL)
			{
				m_socket.emptyReceiveBuffer();
				m_socketBCS.emptyReceiveBuffer();
				m_quePacket.reset();

				CRY(1,_ms("cry in packetHandler2()- type 0x%x",lpPacket->base.wType));

				return	FALSE;
			}

			checkReceivedPacket(lpPacket->base.wType);	//	이벤트 체크		

			fpPacketHandler[iPacketType](lpPacket);
		}
	}

	return	FALSE;
}

//
//
BOOL
CAgent::sendPacket(char *_lpPacketData,int _iSize)
{
	if	(s_bIsSpeedHackUser)
		return	FALSE;

	int	iSendsize	=	_iSize;

	if	(m_bIsWaitSSLAuthenticationCode+m_bIsWaitHandShakeData)
		return	FALSE;

	{

		cMSG_BASE_TYPE	*lpBase	=	(cMSG_BASE_TYPE	*)_lpPacketData;

		// 패킷 암호화		

		if	(lpBase->wType	!=	dCG_CONNECT_GAME	&&	lpBase->wType	!=	dCL_CLIENT_TYPE&&	lpBase->wType	!=	dCM_DUMMY)
		{
			if	(lpBase->wType	<	dCG_CONNECT_GAME )	//	로그인 서버 패킷
				EncryptData(_lpPacketData+6,_iSize-6,lpBase->wType);
 			else
			{
				m_iPacketSerial			=	g_gsEncTable.getNextPacketSerial(m_iPacketSerial);
				lpBase->wProtectType	=	g_gsEncTable.makeProtectCode(m_iPacketSerial,m_iGameServerEncTable,m_wXorCode);

 				g_gsEncTable.encrypt(_lpPacketData+6,_iSize-6,m_abEncTable);
			}
		}

	}

	if	(m_isActive && m_socket.isBlockedToSend()	==	FALSE)
	{
		m_dwLastSendPacketTime	=	timeGetTime();

		if	(m_wIsUseSSL)	//	사실상 안하고 있다
		{
			iSendsize		=	encrypt_msg(&m_ssl,m_aSSLSendBuffer,_lpPacketData,dPACKET_BUFFER_SIZE,_iSize,AES_ENCRYPT);//XOR_ENCRYPT

			int	iSendResult	=	m_socket.send(m_aSSLSendBuffer,iSendsize);

			return	iSendResult;
		}

		return	m_socket.send(_lpPacketData,iSendsize);
	}

	return	FALSE;
}

BOOL
CAgent::sendBCSPacket(void *_lpPacket)
{
	cMSG_BASE_TYPE	*lpPacket	=	(cMSG_BASE_TYPE	*)_lpPacket;

	if	(m_isActive && m_socketBCS.isBlockedToSend()	==	FALSE)
		if	(!m_socketBCS.send((char*)_lpPacket,lpPacket->wSize))
			sendEtcWork(eCEW_DISCONNECT_BCS);

	return	FALSE;
}


void
CAgent::sendCheckPacketSpeedPacket()
{
	m_dwSendCheckReturnPacketSpeedTime	=	timeGetTime();
}

void
CAgent::receiveCheckPacketSpeedPacket()
{
	m_dwCurrentReturnPacketSpeed	=	timeGetTime()-m_dwSendCheckReturnPacketSpeedTime;
	m_dwSumOfReturnPacketSpeed		+=	m_dwCurrentReturnPacketSpeed;

	m_wReturnPacketCount++;

	if (m_wReturnPacketCount	>=	10)
	{
		m_dwReturnPacketSpeed		=	m_dwSumOfReturnPacketSpeed/10;
		m_dwSumOfReturnPacketSpeed	=	0;
		m_wReturnPacketCount		=	0;
	}
}

void
CAgent::setOperateItemData(BOOL _bStatus)
{
	m_isOperateItemData			=	_bStatus;

	m_dwBeginOperateItemTime	=	timeGetTime();
}

BOOL
CAgent::isOperateItemData()
{
	if (m_isOperateItemData)
	{
		if (m_dwBeginOperateItemTime+1000	<	timeGetTime())
			m_isOperateItemData			=	FALSE;
	}

	return	m_isOperateItemData;
}

void
CAgent::setPlayerName(char *_lpstrId,char *_lpstrName)
{
	strcpy(m_strUserId,_lpstrId);
	strcpy(m_strUserName,_lpstrName);
}

BOOL
CAgent::checkChangePosPacketSendCount()
{
	DWORD	dwTime				=	timeGetTime();

	if (m_wIsRequireReceivePacket)
	{
		if (m_dwLastSendPacketTime+1000	<	dwTime)
			sendAskActorInfo(g_lpHero->m_wSerial);

		return	TRUE;
	}

	if (m_dwLastSendChangePosPacket+1000<	dwTime)
	{
		m_wSendChangePosPacketCounter	=	0;
		m_dwLastSendChangePosPacket		=	dwTime;
	}

	if (s_bIsGuildBattleField)
	{
		if (m_wSendChangePosPacketCounter	>=	6)
			return	TRUE;
	}
	else
	if (m_wSendChangePosPacketCounter	>=	4)
		return	TRUE;

	m_wIsRequireReceivePacket	=	TRUE;
	m_wSendChangePosPacketCounter++;

	return	FALSE;
}