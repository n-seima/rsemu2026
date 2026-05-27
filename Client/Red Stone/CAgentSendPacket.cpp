#include "CAgent.H"
#include "CActor.H"
#include "assert.H"


//
//	클라이언트 타입 전송
BOOL
CAgent::sendClientType(int _iClientType)
{
	CL_CLIENT_TYPE	packet;

	packet.base.set(sizeof(CL_CLIENT_TYPE),dCL_CLIENT_TYPE);
	packet.wType	=	_iClientType;

	return	sendPacket((char *)&packet,packet.base.wSize);
}



#pragma pack(1)
struct	CL_LOGIN_old
{
	cMSG_BASE_TYPE	base;
	WORD			wVersion;
	char			strId[dID_LENGTH];
	char			strPassword[dPASS_LENGTH];
	char			strSelectServer[dNAME_LENGTH];
	char			strMacAddress[64];

	WORD			b1IsYahoo				:	1;
	WORD			b1IsDisconnectExistID	:	1;

	WORD			wSecurityCode1,wSecurityCode2;
};	//	struct	CL_LOGIN
#pragma pack()
//
//	로그인
BOOL
CAgent::sendLogIn(char *_strId,char *_strPass,char *_strSelectServer,DWORD _dwClientVersion,BOOL _isDisconnectExistID,BOOL _isPortal,BOOL _bIsUseTokken)
{
	CL_LOGIN	packet;


	char		strMacAddress[1024];

	if	(m_strClientCode[0]	==	NULL)
	{
		srand(timeGetTime());

		for (int i=0;i<10;i++)
			m_strClientCode[i]	=	'0'+random(10);
	}

	strcpy(strMacAddress,getMacAddress());

 	packet.base.set(sizeof(packet),dCL_LOGIN);

	strcpy(packet.strId,_strId);

	if	(_bIsUseTokken)
	{
		strcpy(packet.strPassword,"");
		strcpy(packet.strToken,_strPass);
	}
	else
		strcpy(packet.strPassword,_strPass);

	strcpy(packet.strSelectServer,_strSelectServer);
	memcpy(packet.strMacAddress,strMacAddress,64);
#ifndef	dPRE_TEST
	strcpy(packet.strClientCode,m_strClientCode);
#endif

	packet.strMacAddress[63]	=	NULL;

	{
		packet.wSecurityCode1	=	s_wPacketSecurityCode;
		packet.wSecurityCode2	=	4845;//random(s_wPacketSecurityCode);

		EncodeBuffer(packet.wSecurityCode1,(BYTE *)packet.strId,sizeof(packet.strId));
		EncodeBuffer(packet.wSecurityCode2,(BYTE *)packet.strPassword,sizeof(packet.strPassword));
	}

	packet.wVersion					=	(WORD)_dwClientVersion;
	packet.b1IsDisconnectExistID	=	_isDisconnectExistID;
	packet.b1IsYahoo				=	_isPortal;

	BOOL	bIsSuccess	=	sendPacket((char *)&packet,packet.base.wSize);

	memset(&packet,0,sizeof(packet));

	return	bIsSuccess;
}

//
//	아바타 지우기
BOOL
CAgent::sendRemoveAvatar(char *_strName)
{
	CL_REMOVE_AVATAR	packet;

	packet.base.set(sizeof(CL_REMOVE_AVATAR),dCL_REMOVE_AVATAR);
	
	strcpy(packet.strName,_strName);

	packet.wSecurityCode	=	s_wPacketSecurityCode;
	packet.uiSecurityCodeRet=	dLOGINSERVER_SECURITY_RET;
	EncodeBuffer(s_wPacketSecurityCode,(BYTE*)&packet.uiSecurityCodeRet,sizeof(UINT));


	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//아바타 선택하기
BOOL
CAgent::sendSelectAvatar(char *_strName)
{
	CL_SELECT_AVATAR	packet;

	packet.base.set(sizeof(CL_SELECT_AVATAR),dCL_SELECT_AVATAR);
 
	strcpy(packet.strName,_strName);

	packet.wSecurityCode	=	s_wPacketSecurityCode;
	packet.uiSecurityCodeRet=	dLOGINSERVER_SECURITY_RET;
	EncodeBuffer(s_wPacketSecurityCode,(BYTE*)&packet.uiSecurityCodeRet,sizeof(UINT));


	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	아바타 데이터 테섭에 복사
BOOL
CAgent::sendCopySelectAvatarDataToTestServer(cAVATAR_INFO *_lpInfo)
{
	CL_COPY_DATA	packet;

	packet.base.set(sizeof(packet),dCL_COPY_DATA);
	packet.wType	=	eSERVER_TYPE_TEST;
	packet.wWorld	=	0;

	packet.wSecurityCode	=	s_wPacketSecurityCode;
	packet.uiSecurityCodeRet=	dLOGINSERVER_SECURITY_RET;
	EncodeBuffer(s_wPacketSecurityCode,(BYTE*)&packet.uiSecurityCodeRet,sizeof(UINT));

	memcpy(&packet.info,_lpInfo,sizeof(cAVATAR_INFO));

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendCopySelectAvatarDataToDuelServer(cAVATAR_INFO *_lpInfo,int _iWorld)
{
	CL_COPY_DATA	packet;

	packet.base.set(sizeof(packet),dCL_COPY_DATA);
	packet.wType	=	eSERVER_TYPE_DUEL;
	packet.wWorld	=	_iWorld;

	memcpy(&packet.info,_lpInfo,sizeof(cAVATAR_INFO));

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	아바타 생성하기
BOOL
CAgent::sendCreateAvatar(char *_strName,int _iJob)
{
	CL_CREATE_AVATAR	packet;

	packet.base.set(sizeof(CL_CREATE_AVATAR),dCL_CREATE_AVATAR);

	strcpy(packet.strName,_strName);
	packet.wJob				=	_iJob;
	packet.wSecurityCode	=	s_wPacketSecurityCode;
	packet.uiSecurityCodeRet=	dLOGINSERVER_SECURITY_RET;

	EncodeBuffer(s_wPacketSecurityCode,(BYTE*)&packet.uiSecurityCodeRet,sizeof(UINT));

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//게임에 참가하기
BOOL
CAgent::sendConnectGame(char *_strID,char *_strAvatarName,int _iType)
{
	CG_CONNECT_GAME	packet;

	packet.base.set(sizeof(CG_CONNECT_GAME),dCG_CONNECT_GAME);

	if	(m_strClientCode[0]	==	NULL)
	{
		for (int i=0;i<10;i++)
			m_strClientCode[i]	=	'0'+random(10);
	}

	packet.wType	=	_iType;

	strcpy(packet.strId,_strID);
	packet.dwCode	=	s_dwPasswordAuthenticationCode;
	strcpy(packet.strName,_strAvatarName);
	memcpy(packet.strMacAddress,m_strMacAddress.c_str(),64);
	memcpy(packet.strClientCode,m_strClientCode,12);

#ifdef __OGP_SERVICE
	if	(g_iServiceID)
		strcpy(packet.strToken,g_strTokken);
	else
		packet.base.wSize	=	sizeof(packet)-sizeof(packet.strToken);
#endif
	packet.strClientCode[11]	=	NULL;
	packet.strMacAddress[63]	=	NULL;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//게임에 참가하기
BOOL
CAgent::sendJoinGame()
{
	CG_JOIN_GAME	packet;

	packet.base.set(sizeof(CG_JOIN_GAME),dCG_JOIN_GAME);

	return	sendPacket((char *)&packet,packet.base.wSize);
}


BOOL
CAgent::sendReportCurrentPos()
{
	return	TRUE;
	CG_REPORT_CURRENT_POS	packet;

	packet.base.set(sizeof(CG_REPORT_CURRENT_POS),dCG_REPORT_CURRENT_POS);

	packet.wPosX		=	g_lpHero->m_pos.x;
	packet.wPosY		=	g_lpHero->m_pos.y;
	packet.wDestPosX	=	g_hero.m_posDest.x;
	packet.wDestPosY	=	g_hero.m_posDest.y;

	return	sendPacket((char *)&packet,packet.base.wSize);
}
//
//	이동
BOOL
CAgent::sendMove(int _iXPos,int _iYPos)
{
	int	iMx	=	g_lpHero->m_pos.x/dTILE_XS;
	int	iMy	=	g_lpHero->m_pos.y/dTILE_YS;

	if	(g_lpHero->isBlocked(iMx,iMy))	
		return FALSE;	//	막힌 지형 아닌지 확인
	
	CG_MOVE	packet;

	packet.base.set(sizeof(CG_MOVE),dCG_MOVE);

	if	(checkChangePosPacketSendCount())
		return	FALSE;

	packet.wPosX		=	g_lpHero->m_pos.x;
	packet.wPosY		=	g_lpHero->m_pos.y;
	packet.wDestPosX	=	_iXPos;
	packet.wDestPosY	=	_iYPos;

	if	(_iXPos	==	-1)
	{
		packet.wDestPosX	=	g_lpHero->m_posDest.x;
		packet.wDestPosY	=	g_lpHero->m_posDest.y;
	}

	sendCheckPacketSpeedPacket();

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	정지
BOOL
CAgent::sendStop(BOOL _bIsCheckChangePosPacketSendCount)
{
	if (_bIsCheckChangePosPacketSendCount && checkChangePosPacketSendCount())
		return	FALSE;

	CG_STOP	packet;

	packet.base.set(sizeof(CG_STOP),dCG_STOP);

	int	iMx	=	g_lpHero->m_pos.x/dTILE_XS;
	int	iMy	=	g_lpHero->m_pos.y/dTILE_YS;

	if (g_lpHero->isBlocked(iMx,iMy))
		return FALSE;	//	막힌 지형 아닌지 확인

	packet.wDirect	=	g_lpHero->m_iDirect;
	packet.wPosX	=	g_lpHero->m_pos.x;
	packet.wPosY	=	g_lpHero->m_pos.y;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	이동 어빌러티 변경
BOOL
CAgent::sendSetMoveAbility()
{
	CG_SET_MOVE_ABILITY	packet;

	packet.base.set(sizeof(CG_SET_MOVE_ABILITY),dCG_SET_MOVE_ABILITY);

	packet.wState	=	1-g_lpHero->m_isRunning;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//본쉰!! -o-!!
BOOL
CAgent::sendTransformation()
{
	if (checkChangePosPacketSendCount())
		return	FALSE;

	CG_TRANSFORMATION	packet;

	packet.base.set(sizeof(CG_TRANSFORMATION),dCG_TRANSFORMATION);

	packet.wPosX	=	g_lpHero->m_pos.x;
	packet.wPosY	=	g_lpHero->m_pos.y;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//앉기
BOOL
CAgent::sendSitdown()
{
	if (checkChangePosPacketSendCount())
		return	FALSE;

	CG_TOGGLE_SITDOWN	packet;

	packet.base.set(sizeof(CG_TOGGLE_SITDOWN),dCG_TOGGLE_SITDOWN);

	packet.wPosX	=	g_lpHero->m_pos.x;
	packet.wPosY	=	g_lpHero->m_pos.y;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//
BOOL
CAgent::sendAskInfoActorList(WORD *_lpAskInfoActorList,int _iAskInfoActorCount)
{
	CG_ASK_INFO_ACTOR_LIST	packet;

	int	iSize	=	sizeof(CG_ASK_INFO_ACTOR_LIST)-sizeof(packet.aAskInfoActorList)+_iAskInfoActorCount*2;
	packet.base.set(iSize,dCG_ASK_INFO_ACTOR_LIST);

	packet.wCount	=	_iAskInfoActorCount;

	memcpy(packet.aAskInfoActorList,_lpAskInfoActorList,_iAskInfoActorCount*2);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	이동 어빌러티 변경
BOOL
CAgent::sendObscurityActorList(WORD *_lpObscurityActorList,int _iObscurityActorCount)
{
	CG_OBSCURITY_ACTOR_LIST	packet;

	int		iSize	=	sizeof(CG_OBSCURITY_ACTOR_LIST)-sizeof(packet.aObscurityActor)+_iObscurityActorCount*2;
	packet.base.set(iSize,dCG_OBSCURITY_ACTOR_LIST);

	packet.wCount	=	_iObscurityActorCount;

	memcpy(packet.aObscurityActor,_lpObscurityActorList,_iObscurityActorCount*2);


	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAskActorEnchantInfoList(WORD *_lpwActorList,int _iActorCount)
{
	CG_ASK_ACTOR_ENCHANT_INFO	packet;

	int	iSize	=	sizeof(packet)-sizeof(packet.awData)+_iActorCount*sizeof(WORD)*2;

	packet.base.set(iSize,dCG_ASK_ACTOR_ENCHANT_INFO);

	packet.wCount	=	_iActorCount;

	memcpy(packet.awData,_lpwActorList,_iActorCount*2*2);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	특정 액터들의 위치 정보 요청
BOOL
CAgent::sendMisplacedActorList(WORD *_lpMisplacedActorList,int _iMisplacedActorCount)
{
	CG_MISPLACED_ACTOR_LIST	packet;

	int	iSize	=	sizeof(packet)-sizeof(packet.aMisplacedActorList)+_iMisplacedActorCount*sizeof(WORD);

	packet.base.set(iSize,dCG_MISPLACED_ACTOR_LIST);

	packet.wCount	=	_iMisplacedActorCount;

	memcpy(packet.aMisplacedActorList,_lpMisplacedActorList,_iMisplacedActorCount*2);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//퀵 캐스트?
BOOL
CAgent::sendQuickAction(int _iAbility)
{
	if	(checkChangePosPacketSendCount())
		return	FALSE;

	if	(s_bIsBlockUseSkill)
		return	TRUE;

	CG_ACTION_QUICK	packet;

	packet.base.set(sizeof(CG_ACTION_QUICK),dCG_ACTION_QUICK);

	packet.wAbility	=	_iAbility;
	packet.wPosX	=	g_lpHero->m_pos.x;
	packet.wPosY	=	g_lpHero->m_pos.y;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendUseSkillToObject(int _iTargetType,int _iTarget,int _iAbility)
{
	CG_ACTION_TO_OBJECT	packet;

	packet.base.set(sizeof(packet),dCG_ACTION_TO_OBJECT);

	packet.bAbility		=	_iAbility;
	packet.bObjectType	=	_iTargetType;
	packet.wObjectSerial=	_iTarget;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//액터를 공격!!
BOOL
CAgent::sendUseSkillToTarget(int _iTarget,int _iAbility,int _iEngageAbility,int _iSubAbility)
{
	if	(s_bIsBlockUseSkill)
		return	TRUE;

	CG_ACTION_TO_ACTOR	packet;

	packet.base.set(sizeof(CG_ACTION_TO_ACTOR),dCG_ACTION_TO_ACTOR);
	packet.wTargetSerial	=	_iTarget;

	packet.bAbility			=	_iAbility;
	packet.bEngageAbility	=	_iEngageAbility;
	packet.bSubAbility		=	_iSubAbility;

	if (g_hero.m_bf1IsSubAttackAbilityOn	==	FALSE)
		packet.bSubAbility		=	0xff;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	_iX,_iY에 공격!!
BOOL
CAgent::sendUseSkillToGround(int _iX,int _iY,int _iAbility)
{
	if	(s_bIsBlockUseSkill)
		return	TRUE;

	CG_ACTION_TO_GROUND	packet;

	packet.base.set(sizeof(CG_ACTION_TO_GROUND),dCG_ACTION_TO_GROUND);

	packet.wDestPosX	=	_iX;
	packet.wDestPosY	=	_iY;
	packet.wAbility		=	_iAbility;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//액터를 공격!!
BOOL
CAgent::sendEngageBattle(int _iTarget,int _iAbility)
{
	CG_ENGAGE_BATTLE	packet;

	packet.base.set(sizeof(CG_ENGAGE_BATTLE),dCG_ENGAGE_BATTLE);

	packet.wTargetSerial=	_iTarget;
	packet.wAbility		=	_iAbility;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//전투 중지
BOOL
CAgent::sendStopBattle()
{
	CG_STOP_BATTLE	packet;

	packet.base.set(sizeof(CG_STOP_BATTLE),dCG_STOP_BATTLE);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//특정 스탯 수치 업!!
BOOL
CAgent::sendIncreaseState(int _iState,int _iPoint)
{
	CG_INCREASE_STATE	packet;

	packet.base.set(sizeof(CG_INCREASE_STATE),dCG_INCREASE_STATE);
	packet.wState	=	_iState;
	packet.wPoint	=	_iPoint;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//특정 어빌러티의 레벨 업!!
BOOL
CAgent::sendIncreaseAbilityLevel(int _iAbility,int _iPoint,int _iAimLevel)
{
	CG_INCREASE_ABILITY_LEVEL	packet;

	packet.base.set(sizeof(CG_INCREASE_ABILITY_LEVEL),dCG_INCREASE_ABILITY_LEVEL);

	packet.wAbility	=	_iAbility;
	packet.wPoint	=	_iPoint;
	packet.wAimLevel=	_iAimLevel;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//특정 몬스터 정보 요청
BOOL
CAgent::sendAskActorInfo(int _iActor)
{
	CG_ASK_ACTOR_INFO	packet;

	packet.base.set(sizeof(CG_ASK_ACTOR_INFO),dCG_ASK_ACTOR_INFO);

	packet.wTarget	=	_iActor;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	액터 찾기
BOOL
CAgent::sendFindActor(char *_lpstrName)
{
	CG_FIND_USER	packet;

	packet.base.set(sizeof(CG_FIND_USER),dCG_FIND_USER);
	strcpy(packet.strName,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	필드 이동 할껴!!
BOOL
CAgent::sendMoveField()
{
	CG_MOVE_FIELD	packet;

	packet.base.set(sizeof(CG_MOVE_FIELD),dCG_MOVE_FIELD);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendEnterPortal(int _iPortal)
{
	CG_ENTER_PORTAL	packet;

	packet.base.set(sizeof(CG_ENTER_PORTAL),dCG_ENTER_PORTAL);
	packet.wPortal	=	_iPortal;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendWarpToNearVillage(int _iVillage)
{
	CG_WARP_TO_NEAR_VILLAGE	packet;

	packet.base.set(sizeof(CG_WARP_TO_NEAR_VILLAGE),dCG_WARP_TO_NEAR_VILLAGE);
	packet.wVillage	=	_iVillage;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	마을로 돌아가기
BOOL
CAgent::sendReturnToVillage(int _iWork)
{
	CG_RETURN_TO_VILLAGE	packet;

	packet.base.set(sizeof(CG_RETURN_TO_VILLAGE),dCG_RETURN_TO_VILLAGE);
	packet.wWork	=	_iWork;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	리액션 켜기
BOOL
CAgent::sendSetReactionStatus(int _iAbility,BOOL _isOn)
{
	if (m_iSendSetReactionPacketTime)
		if (m_iSendSetReactionPacketTime+ dPOPS_DEFAULT*dSYNC_FPS > s_iFrameCounter)
			return	FALSE;

	if(g_hero.m_wTransformationTime)
		return FALSE;

	m_iSendSetReactionPacketTime	=	s_iFrameCounter;

	CG_SET_REACTION_STATUS	packet;

	packet.base.set(sizeof(CG_SET_REACTION_STATUS),dCG_SET_REACTION_STATUS);

	packet.wAbility	=	_iAbility;
	packet.wIsOn	=	_isOn;

	return	sendPacket((char *)&packet,packet.base.wSize);
}
//
//	패시브 스킬들 켜기
BOOL
CAgent::sendTurnOnPassiveSkill(CG_TURN_ON_PASSIVE_SKILL *_lpPacket)
{
	return	sendPacket((char *)_lpPacket,_lpPacket->base.wSize);
}

//
//	현재 시간 전송
BOOL
CAgent::sendExitGame(int _iType)
{
	CG_EXIT_GAME	packet;

	packet.base.set(sizeof(CG_EXIT_GAME),dCG_EXIT_GAME);
	packet.wValue		=	_iType;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	커맨드
BOOL
CAgent::sendOperatorCommand(CG_OPERATOR_COMMAND	*_lpPacket)
{
	return	sendPacket((char *)_lpPacket,_lpPacket->base.wSize);
}

//
//	친구 친구 메시지에 대한 응답
BOOL
CAgent::sendReplyAddFriend(int _iReply,char *_lpstrTarget)
{
	CG_REPLY_FOR_ADD_FRIEND	packet;

	packet.base.set(sizeof(packet),dCG_REPLY_FOR_ADD_FRIEND);

	packet.wResult	=	_iReply;
	strcpy(packet.strName,_lpstrTarget);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	친구 추가 요청 메시지
BOOL
CAgent::sendRequestAddFriend(char *_lpstrTarget,BOOL _bIsRecoverList,int _iServerSign,int _iJob)
{
	CG_REQUEST_ADD_FRIEND	packet;

	packet.base.set(sizeof(packet),dCG_REQUEST_ADD_FRIEND);

	strcpy(packet.strName,_lpstrTarget);

	packet.bf1IsRecoverListWork	=	_bIsRecoverList;
	packet.bf8Job				=	_iJob;
	packet.wCheckSign			=	_iServerSign;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	커뮤니티 거부 상태
BOOL
CAgent::sendChangCommunityStatus(BOOL _bIsDenyCommunity,BOOL _bIsHidePitchmanShop)
{
	CG_CHANGE_COMMUNITY_STATUS	packet;

	packet.base.set(sizeof(packet),dCG_CHANGE_COMMUNITY_STATUS);
	packet.bf1IsDenyCommunity	=	_bIsDenyCommunity;
	packet.bf1IsHidePitchmanShop=	_bIsHidePitchmanShop;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendRemoveMyNameInTargetFriendList(char *_lpstrTarget)
{
	CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST	packet;

	packet.base.set(sizeof(packet),dCG_REMOVE_NAME_IN_TARGET_FRIEND_LIST);

	strcpy(packet.strTargetName,_lpstrTarget);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendRemoveMyNameInTargetFriendListResult(char *_lpstrClient,int _iResult)
{
	CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT	packet;

	packet.base.set(sizeof(packet),dCG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT);

	strcpy(packet.strRequestUserName,_lpstrClient);
	packet.wResult	=	_iResult;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

//
//	
BOOL
CAgent::sendDebugMessage(char *_lpstrMessage)
{
	CG_DEBUG_MESSAGE			packet;

	packet.base.set(sizeof(CG_DEBUG_MESSAGE),dCG_DEBUG_MESSAGE);

	DWORD	dwMessageLength	=	strlen(_lpstrMessage);

	if	(dwMessageLength	>=	dDEBUG_MESSAGE_LENGTH)	return FALSE;

	strcpy(packet.strMessage,_lpstrMessage);

	packet.base.wSize	=	sizeof(CG_DEBUG_MESSAGE) - dDEBUG_MESSAGE_LENGTH+ dwMessageLength+1;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendCallMagicCarpet()
{
	CG_CALL_MAGIC_CARPET	packet;

	packet.base.set(sizeof(packet),dCG_CALL_MAGIC_CARPET);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendChangeMagicCarpetShape(int _iShape)
{
	CG_CHANGE_EMBLEM_SHAPE	packet;

	packet.base.set(sizeof(packet),dCG_CHANGE_EMBLEM_SHAPE);
	packet.wShape	=	_iShape;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendRememberOrbLocation(int _iSlot)
{
	CG_REMEMBER_ORB_LOCATION		packet;

	if (g_hero.m_bf1IsWaitTeleportResult)	return	FALSE;

	g_hero.m_bf1IsWaitTeleportResult	=	TRUE;

	packet.base.set(sizeof(CG_REMEMBER_ORB_LOCATION),dCG_REMEMBER_ORB_LOCATION);
	packet.wSlot	=	_iSlot;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendTeleportToOrbLocation(int _iSlot)
{
	CG_TELEPORT_TO_ORB_LOCATION		packet;

	if (g_hero.m_bf1IsWaitTeleportResult)	return	FALSE;

	g_hero.m_bf1IsWaitTeleportResult	=	TRUE;

	packet.base.set(sizeof(CG_TELEPORT_TO_ORB_LOCATION),dCG_TELEPORT_TO_ORB_LOCATION);
	packet.wSlot	=	_iSlot;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAskRecoverDeathPenelty()
{
	CG_RECOVER_DEATH_PENELTY	packet;

	packet.base.set(sizeof(CG_RECOVER_DEATH_PENELTY),dCG_RECOVER_DEATH_PENELTY);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendChangePetName(int _iIndex,char *_lpstrPetName,BOOL _bIsMiniPet)
{
	CG_CHANGE_PET_NAME	packet;

	packet.base.set(sizeof(CG_CHANGE_PET_NAME),dCG_CHANGE_PET_NAME);
	packet.wPetIndex	=	_iIndex;
	memcpy(packet.strPetName,_lpstrPetName,dPET_NAME_LENGTH);
	packet.strPetName[dPET_NAME_LENGTH]	=	NULL;
	packet.wIsMiniPet					=	_bIsMiniPet;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendPartingWithPet(int _iIndex,int _iType)
{
	CG_PARTING_WITH_PET	packet;

	packet.base.set(sizeof(CG_PARTING_WITH_PET),dCG_PARTING_WITH_PET);
	packet.bf10PetIndex		=	_iIndex;
	packet.bf6Type			=	_iType;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendAskPlayerInfo(char *_lpstrName)
{
	CG_ASK_PLAYER_INFO	packet;

	packet.base.set(sizeof(CG_ASK_PLAYER_INFO),dCG_ASK_PLAYER_INFO);
	strcpy(packet.strName,_lpstrName);

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendCheckHeroInfo(DWORD _dwCheckSum)
{
	CG_CHECK_PLAYER_CHECK_SUM	packet;

	packet.base.set(sizeof(CG_CHECK_PLAYER_CHECK_SUM),dCG_CHECK_PLAYER_CHECK_SUM);
	packet.dwCheckSum	=	_dwCheckSum;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendHackingLog(int _iCode,char *_lpstrHackName)
{
	CG_HACKING_LOG	packet;

	packet.base.set(sizeof(packet),dCG_HACKING_LOG);
	packet.wCode	=	_iCode;

	if (_lpstrHackName)
	{
		strncpy(packet.strHackName,_lpstrHackName,sizeof(packet.strHackName)-2);
		packet.strHackName[sizeof(packet.strHackName)-1]	=	NULL;
	}
	else
		packet.strHackName[0]	=	0;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendGGAuthData(void *_lpData)
{
#ifdef	_USE_NPGL
	CG_GG_AUTH_DATA	packet;

	packet.base.set(sizeof(packet),dCG_GG_AUTH_DATA);

	memcpy(packet.aBuffer,_lpData,sizeof(GG_AUTH_DATA));

	return	sendPacket((char *)&packet,packet.base.wSize);
#endif

	return	TRUE;
}

BOOL
CAgent::sendEtcWork(int _iWork,int _iCount,int *_lpiValue)
{
	CG_ETC_WORK	packet;

	assert(_iCount <= sizeof(packet.awValue)/sizeof(WORD));

	packet.base.set(sizeof(packet),dCG_ETC_WORK);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.awValue)+sizeof(WORD)*_iCount;
	packet.wWork		=	_iWork;

	for (int i=0;i<_iCount;i++)
	{
		assert(_lpiValue[i] < 0xffff);

		packet.awValue[i]	=	_lpiValue[i];
	}

	return	sendPacket((char *)&packet,packet.base.wSize);
}


BOOL
CAgent::sendEtcWork(int _iWork,int _iCount,WORD *_lpwValue)
{
	CG_ETC_WORK	packet;

	assert(_iCount <= sizeof(packet.awValue)/sizeof(WORD));

	packet.base.set(sizeof(packet),dCG_ETC_WORK);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.awValue)+sizeof(WORD)*_iCount;
	packet.wWork		=	_iWork;

	for (int i=0;i<_iCount;i++)
		packet.awValue[i]	=	_lpwValue[i];

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendEtcWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	CG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dCG_ETC_WORK);
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.awValue)+sizeof(WORD)*6;

	packet.wWork		=	_iWork;
	packet.awValue[0]	=	_iValue1;
	packet.awValue[1]	=	_iValue2;
	packet.awValue[2]	=	_iValue3;
	packet.awValue[3]	=	_iValue4;
	packet.awValue[4]	=	_iValue5;
	packet.awValue[5]	=	_iValue6;

	return	sendPacket((char *)&packet,packet.base.wSize);
}

BOOL
CAgent::sendGuildMemberWork(int _iWork,char *_lpstrName,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	CG_GUILD_MEMBER_WORK	packet;

	packet.base.set(sizeof(packet),dCG_GUILD_MEMBER_WORK);

	strcpy(packet.strMember,_lpstrName);

	packet.wWork		=	_iWork;
	packet.awValue[0]	=	_iValue1;
	packet.awValue[1]	=	_iValue2;
	packet.awValue[2]	=	_iValue3;
	packet.awValue[3]	=	_iValue4;
	packet.awValue[4]	=	_iValue5;
	packet.awValue[5]	=	_iValue6;

	return	sendPacket((char *)&packet,packet.base.wSize);
}