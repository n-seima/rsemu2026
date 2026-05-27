#include "cFIELD.H"
#include "cGAME.H"
#include "debugCode.H"
#include "CGuild.H"
#include "packetManager.H"
#include "booking_work.H"
#include "secretDungeon.H"
#include "debugCode.H"
#include "cDECODER.H"
#include "actor_mini_pet.H"
#include "field_OC.H"
#include "CEvent.h"
#include "cPARTY.h"
//
//	채팅
void
cFIELD::chat(CG_CHAT *_lpChatPacket,int _iSerial)
{
	cACTOR	*lpChatter		=	getTestedActor(_iSerial);

	if	(!lpChatter)
		return;

	lpChatter->updateLastActionTime();

	if	(_lpChatPacket->wChatType	==	dCHAT_DUMMY)
		return;

	if	(((cField_OC *)this)->operateSpecialChat(_lpChatPacket,lpChatter)	==	TRUE)
		return;
/*	if (m_wIsVillage			||	m_iUniqueSerial	==	64	||	
		m_iUniqueSerial	==	116	||	m_iUniqueSerial	==	117	||	
		m_iUniqueSerial	==	118	||	m_iUniqueSerial	==	202	)
		CLOG(_ms("__chat [%3d]",m_iUniqueSerial),"%.2d/%.2d %.2d:%.2d:%.2d [%s/%s] %s",g_currentTime.m_wMonth,g_currentTime.m_wDay,g_currentTime.m_wHour,g_currentTime.m_wMinute,g_currentTime.m_wSecond,lpChatter->m_strId,lpChatter->m_strName,_lpChatPacket->strMessage);
*/
#ifdef	_FOR_CHINA
	DWORD	dwLastChatHashCode		=	GetHashCode(_lpChatPacket->strMessage);

	if	(dwLastChatHashCode	==	lpChatter->m_dwLastChatHashCode)
		return;

	lpChatter->m_dwLastChatHashCode	=	dwLastChatHashCode;
#endif

	_lpChatPacket->strMessage[dCHAT_LENGTH-1]	=	NULL;

	switch(_lpChatPacket->wChatType)
	{
		case	dCHAT_CHAT		:
		{
			SG_CHAT	sgChat;

			if	(strlen(_lpChatPacket->strMessage) >= dCHAT_LENGTH)
				break;

			if (lpChatter->m_wOperatorLevel < eAL_BOSS)
				if (lpChatter->m_dwLastChatTime + dCHAT_PERIOD > g_dwCurrentTime && lpChatter->m_dwLastChatTime < g_dwCurrentTime)
					break;

			lpChatter->m_dwLastChatTime	=	g_dwCurrentTime;

			if (g_eventManager.isWordCompareEventTime())
			{
				if(g_eventManager.isEventWordCompare(_lpChatPacket->strMessage))
				{
					g_eventManager.operateWordEventPlayer(lpChatter);
				}
			}

			FillChattingBuffer(&sgChat,lpChatter->m_strName,_lpChatPacket->strMessage);

			sgChat.bf11Serial	=	_iSerial;
			sgChat.bf6ChatType	=	eCT_TALK;

			if (lpChatter->m_wOperatorLevel >= eAL_BOSS)
			{
				if (lpChatter->m_wLordOperator == 0xffff)
					sgChat.bf6ChatType	+=	0x20;
				else
				{
					cACTOR	*lpSlave	=	getActor(lpChatter->m_wLordOperator);

					FillChattingBuffer(&sgChat,lpSlave->m_strName,_lpChatPacket->strMessage);
					sgChat.bf11Serial		=	lpChatter->m_wLordOperator;
				}
			}

			addSendPacket((ALL_MSG*)&sgChat,_iSerial);
			break;
		}

		case	dCHAT_SHOUT		:
		{
			SG_CHAT	sgChat;

#ifdef	_FOR_JAPAN
			if	(lpChatter->m_iLevel	<=	4)
				break;
#endif

			if	(strlen(_lpChatPacket->strMessage) >= dCHAT_LENGTH)
				break;

			if	(lpChatter->m_wOperatorLevel < eAL_BOSS)
			{
				if	(lpChatter->m_wShoutCount	>=	dSHOUT_COUNT_BY_ONE_PERIOD )
				{
					if	(lpChatter->m_dwLastShoutTime+dSHOUT_PERIOD*1000	>	g_dwCurrentTime)
					{
						SG_CHAT_FAILED	packet;

						packet.base.set(sizeof(SG_CHAT_FAILED),dSG_CHAT_FAILED);
						packet.wReason	=	eRCF_CAN_NOT_SHOUT_BY_TIME;
						g_userPM.add(lpChatter->m_iClientSerial,&packet,packet.base.wSize);
						return;
					}

					lpChatter->m_wShoutCount	=	0;
				}

				if	(lpChatter->m_dwLastShoutTime+dSHOUT_PERIOD*1000	<	g_dwCurrentTime)	
					lpChatter->m_wShoutCount	=	0;
			}

			lpChatter->m_dwLastShoutTime	=	g_dwCurrentTime;
			lpChatter->m_wShoutCount++;

			sgChat.bf11Serial	=	_iSerial;
			sgChat.bf6ChatType	=	eCT_SHOUT;

			if	(lpChatter->m_wOperatorLevel >= eAL_BOSS)
				sgChat.bf6ChatType	+=	0x20;

			FillChattingBuffer(&sgChat,lpChatter->m_strName,_lpChatPacket->strMessage);

			addSendPacket((ALL_MSG*)&sgChat,_iSerial);
			break;
		}

		case	dCHAT_SAY		:
		{
			SERVERStoWORLD_USER_SAY	packet;

			packet.base.set(sizeof(SERVERStoWORLD_USER_SAY),dSERVERStoWORLD_USER_SAY);

			packet.dwSenderSerial	=	lpChatter->m_iZoneSerial;

			if	(strlen(_lpChatPacket->strMessage) >= dCHAT_LENGTH)
				break;

			if	(strlen(_lpChatPacket->strDestName) >= dNAME_LENGTH)
				break;

			strcpy(packet.strSendName	,lpChatter->m_strName);
			strcpy(packet.strRecvName	,_lpChatPacket->strDestName);
			strcpy(packet.strMsg		,_lpChatPacket->strMessage);

			if	(lpChatter->m_wOperatorLevel	>=	2)
				packet.wIsByOperator	=	TRUE;
			else
				packet.wIsByOperator	=	FALSE;

			packet.base.wSize	=	sizeof(packet)-dCHAT_LENGTH+strlen(packet.strMsg)+1;

			SEND_WOLRD_PACKET(&packet);
			break;
		}

		case	dCHAT_PARTY		:
		{
			if	(lpChatter->getPartySerial()	==	0xffff)
				break;

			cParty	*lpParty	=	lpChatter->getParty();

			if	(!lpParty)
			{
				CLOG("party","dCHAT_PARTY");
				break;
			}

#ifndef	_FOR_JAPAN
//			if	(g_game.sendBPPartyChat(lpChatter,_lpChatPacket->strMessage))
//				break;
#endif

			{
				SERVERStoWORLD_PARTY_CHAT	packet;

				packet.base.set(sizeof(SERVERStoWORLD_PARTY_CHAT),dSERVERStoWORLD_PARTY_CHAT);
				packet.wPartySerial	=	lpChatter->getPartySerial();

				strcpy(packet.strName		,lpChatter->m_strName);
				strcpy(packet.strMsg		,_lpChatPacket->strMessage);

				SEND_WOLRD_PACKET(&packet);
			}
			break;
		}

		case	dCHAT_GUILD		:
		{
			if	(g_bIsEnableGuildChat	==	FALSE)
				break;

			if	(lpChatter->m_wGuildSerial == 0xffff)
				break;

			g_game.sendBPGuildChat(lpChatter,_lpChatPacket->strMessage);
			break;
		}

		case	dCHAT_ADMIN		:
			break;
	}

	if	(lpChatter->isTester()	&&	_lpChatPacket->wChatType	!=	dCHAT_SAY)
	{
		int	iType	=	_lpChatPacket->wChatType;

		if	(_lpChatPacket->wChatType	==	dCHAT_PARTY)
			iType	=	eCT_PARTY;

		lpChatter->sendOperatorChatLog(iType,FALSE,"normal chat",_lpChatPacket->strMessage);
	}
}

//
//	파티리스트를 요청하는 메시지를 받았다.
void
cFIELD::receiveAskPartyListMessage(CG_ASK_PARTY_LIST *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if	(!checkActor(lpActor))
		return;

	CLOG("packetHacking","[%.3d] [%s/%s] hacking CG_ASK_PARTY_LIST",lpActor->m_wCurrentField,lpActor->m_strId,lpActor->m_strName);

	lpActor->setPeneltyTime(600,ePPPTR_PACKET_HACKING);

	return;

	SERVERStoWORLD_ASK_PARTY_LIST	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ASK_PARTY_LIST);

	packet.dwSerial		=	lpActor->m_iClientSerial;	//	받을 유저의 시리얼
	packet.isNext		=	_lpPacket->wDirection;		//	0 - 전 페이지 , 1 - 다음 페이지
	packet.wPartyIndex	=	_lpPacket->wPartyIndex;		//	받을 기준이 되는 파티의 인덱스

	SEND_WOLRD_PACKET(&packet);
}

//
//	유저 찾기
void
cFIELD::receiveFindUserMessage(CG_FIND_USER *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	_lpPacket->strName[dNAME_LENGTH-1]	=	NULL;

	g_game.sendWPFindUser(lpActor->m_strName,_lpPacket->strName,lpActor->m_wCurrentField,eFUW_FIND);
}

void
cFIELD::receiveRemoveNameInTargetFriendList(CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST* _lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	_lpPacket->strTargetName[dNAME_LENGTH-1]	=	NULL;

	g_game.sendWPRemoveFriendInTargetList(lpActor->m_strName,_lpPacket->strTargetName);
}

void
cFIELD::receiveRemoveNameInTargetFriendListResult(CG_REMOVE_NAME_IN_TARGET_FRIEND_LIST_RESULT* _lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if	(!lpActor)
		return;

	_lpPacket->strRequestUserName[dNAME_LENGTH-1]	=	NULL;

	g_game.sendWPRemoveFriendInTargetListResult(_lpPacket->strRequestUserName,lpActor->m_strName,_lpPacket->wResult);
}

//
//	친구 추가 요청
void
cFIELD::receiveRequestAddFriendMessage(CG_REQUEST_ADD_FRIEND *_lpPacket,int _iSerial)
{
	_lpPacket->strName[dNAME_LENGTH-1]	=	NULL;
	
	cACTOR	*lpActor	=	getTestedActor(_iSerial);
	cACTOR	*lpTarget	=	getPlayerByName(_lpPacket->strName);

	if	(!lpActor)
		return;

	if	(_lpPacket->bf1IsRecoverListWork)
	{
		WORD	wCheckValue	=	0;
		int		iLength		=	strlen(_lpPacket->strName);
		int		iServerSign	=	lpActor->m_wServerSign;

		for (int i=0;i<iLength;i++)
		{
			int	iValue		=	(BYTE)_lpPacket->strName[i];

			if	(iValue	<	'a')
				iValue		+=	'a'-'A';

			wCheckValue		+=	iValue+iServerSign;

			switch(i%4)
			{
				case	0	:
					iServerSign		+=	iValue/2;
					break;
				case	1	:
					iServerSign		+=	iValue/3;
					break;
				case	2	:
					iServerSign		+=	iValue/4;
					break;
				case	3	:
					iServerSign		+=	iValue/5;
					break;
			}
		}

		if	(stricmp(_lpPacket->strName,"test")==0)
		{
			CG_USE_ITEM	packet;

			packet.base.wSize	=	sizeof(cMSG_BASE_TYPE);
			packet.base.wType	=	0x1213+0x1324;

			for (int iActor=m_aiActorCount[0];iActor;)
			{
				--iActor;

				cACTOR	*lpActor	=	m_alpActor[m_awActorList[0][iActor]];

				if	(lpActor	==	NULL || !lpActor->isPlayer())
					continue;

				addReceivePacket((ALL_MSG*)&packet,lpActor->m_wSerialInField,TRUE);

				break;
			}
//		0x2537
		}

		if	(wCheckValue	!=	_lpPacket->wCheckSign)
			return;

		SG_REPLY_FOR_ADD_FRIEND	packet;

		packet.base.set(sizeof(packet),dSG_REPLY_FOR_ADD_FRIEND);

		packet.bf1IsRecover		=	TRUE;

		{
			packet.wResult		=	eRFA_OK;
			packet.bf8Job		=	_lpPacket->bf8Job;
			strcpy(packet.strName,_lpPacket->strName);

			g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}

		if	(lpTarget)
		{
			packet.wResult	=	eRFA_AUTO_ADD_BY_ADDED;
			packet.bf8Job	=	lpActor->m_wJob;
			strcpy(packet.strName,lpActor->m_strName);

			g_userPM.add(lpTarget->m_iClientSerial,&packet,packet.base.wSize);
		}

		g_game.sendBPAddFriend(lpActor,NULL,_lpPacket->strName,_lpPacket->bf8Job);

		return;
	}

	if	(!lpTarget)
	{
		SG_REPLY_FOR_ADD_FRIEND	packet;

		packet.base.set(sizeof(packet),dSG_REPLY_FOR_ADD_FRIEND);
		packet.wResult		=	eRFA_CAN_NOT_FIND;
		packet.bf1IsRecover	=	FALSE;
		strcpy(packet.strName,_lpPacket->strName);

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

	if	(lpTarget->isDenyCommunity()	==	TRUE)	//	커뮤니티 거부 상태다
	{
		SG_REPLY_FOR_ADD_FRIEND	packet;

		packet.base.set(sizeof(packet),dSG_REPLY_FOR_ADD_FRIEND);
		packet.wResult		=	eRFA_STATUS_DENY_COMMUNITY;
		packet.bf1IsRecover	=	FALSE;
		strcpy(packet.strName,_lpPacket->strName);

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		return;
	}

	SG_REQUEST_ADD_FRIEND	packet;

	packet.base.set(sizeof(packet),dSG_REQUEST_ADD_FRIEND);
	strcpy(packet.strName,lpActor->m_strName);
	
	lpTarget->m_wRequestFriendSerial	=	lpActor->m_wSerialInField;

	g_userPM.add(lpTarget->m_iClientSerial,&packet,packet.base.wSize);
}

//
//	친구 추가 요청에 대한 응답
void
cFIELD::receiveReplyForAddFriendMessage(CG_REPLY_FOR_ADD_FRIEND *_lpPacket,int _iSerial)
{
	_lpPacket->strName[dNAME_LENGTH-1]	=	NULL;
	
	cACTOR	*lpActor	=	getTestedActor(_iSerial);
	cACTOR	*lpTarget	=	getPlayerByName(_lpPacket->strName);

	if	(!lpActor)
		return;

	if	(!lpTarget)
		return;

	if	(lpActor->m_wRequestFriendSerial	!=	lpTarget->m_wSerialInField)
		return;

	lpActor->m_wRequestFriendSerial	=	0xffff;

	SG_REPLY_FOR_ADD_FRIEND	packet;

	packet.base.set(sizeof(packet),dSG_REPLY_FOR_ADD_FRIEND);
	packet.wResult		=	_lpPacket->wResult;
	packet.bf8Job		=	lpActor->m_wJob;
	packet.bf1IsRecover	=	FALSE;
	strcpy(packet.strName,lpActor->m_strName);

	g_userPM.add(lpTarget->m_iClientSerial,&packet,packet.base.wSize);

	if	(packet.wResult	==	eRFA_OK)
	{
		packet.wResult	=	eRFA_AUTO_ADD_BY_ADDED;
		packet.bf8Job	=	lpTarget->m_wJob;
		strcpy(packet.strName,lpTarget->m_strName);

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		g_game.sendBPAddFriend(lpActor,lpTarget);
	}
}

//
//	커뮤니티 상태 변경 메시지
void
cFIELD::receiveChangeCommunityStatus(CG_CHANGE_COMMUNITY_STATUS *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getTestedActor(_iSerial);

	if (!lpActor)
		return;

	lpActor->m_bf1IsDenyCommunity	=	_lpPacket->bf1IsDenyCommunity;
	lpActor->m_bf1IsHidePitchmanShop=	_lpPacket->bf1IsHidePitchmanShop;
}

//
//	파티 정보를 요청하는 메시지를 받았다.
void
cFIELD::receiveAskPartyInfoMessage(CG_ASK_PARTY_INFO *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if (!checkActor(lpActor))	return;

	cParty	*lpParty	=	lpActor->getParty();

	if (!lpParty || lpParty->m_wSerial	==	0xffff)
	{
		CLOG("party","receiveAskPartyInfoMessage");
		lpActor->sendPartyMessage(ePM_YOU_ARE_NOT_BELONG_TO_PARTY);	//	니는 파티에 소속해 있지 않데이.. -o-
		return;
	}

	sendPartyInfo(lpActor,FALSE);
}

void
cFIELD::receiveChangePartyStatus(CG_CHANGE_PARTY_STATUS *_lpPacket,int _iSerial)
{
	cACTOR	*lpAsker	=	getActor(_iSerial);

	if (!checkActor(lpAsker))
		return;

	cParty	*lpParty	=	lpAsker->getParty();

	if	(!lpParty)
		return;

	if	(!lpAsker->isPartyLeader())
		return;

	lpParty->m_bf3GoldShareMethod	=	_lpPacket->bf3GoldShareMethod;
	lpParty->m_bf3ItemShareMethod	=	_lpPacket->bf3ItemShareMethod;
	lpParty->m_bf3PartyObject		=	_lpPacket->bf3PartyObject;
	lpParty->m_bf1IsOpenParty		=	_lpPacket->bf1IsOpenParty;

	g_game.sendWPChangePartyStatus(lpParty);
}

//
//	파티 참가 요첨 메시지를 받았다.
void
cFIELD::receiveAskJoinPartyMessage(CG_ASK_JOIN_PARTY *_lpPacket,int _iSerial)
{
	cACTOR	*lpAsker	=	getActor(_iSerial);

	if	(!checkActor(lpAsker))
		return;

	if	(_lpPacket->bf1IsGuildMemberJoinParty)
	{	 
		g_game.sendWPGuildMemberJoinParty(lpAsker,_lpPacket->strName);
		return;

	}

	if	(_lpPacket->bf1IsJoinPartyApplication)
	{
		if	(lpAsker->getPartySerial()	!=	0xffff)
			return;

		cACTOR	*lpLeader	=	g_game.getPlayer(_lpPacket->strName);

		if	(lpLeader)
		{
			if	(!lpLeader->isPartyLeader())
			{
				lpAsker->sendPartyMessage(ePM_FAILED_TO_JOIN_PARTY);
				return;
			}
			if	(lpAsker->m_wTeam			!= lpLeader->m_wTeam)
			{
				lpAsker->sendPartyMessage(ePM_TARGET_NOT_OWN_FORCE);
				return;
			}
			if	(lpLeader->m_wOperatorLevel	 >= eAL_BOSS)
			{
				lpAsker->sendPartyMessage(ePM_FAILED_TO_JOIN_PARTY);
				return;
			}
			if	(lpLeader->isDenyCommunity()	==	TRUE)
			{
				lpAsker->sendPartyMessage(ePM_TARGET_DENY_COMMUNITY_STATUS);
				return;
			}
			if	(lpLeader->isDeath())	//	타겟이 죽거나 없다.
			{
				lpAsker->sendPartyMessage(ePM_TARGET_DEATH_OR_NOT_EXIST);
				return;
			}

			lpLeader->sendJoinParty(lpAsker->m_iLevel,lpAsker->m_wJob,lpAsker->m_strName,TRUE);
		}
		else
			g_game.sendWPJoinPartyApplication(lpAsker,_lpPacket->strName);

		return;
	}

	cACTOR	*lpTarget	=	getPlayerByName(_lpPacket->strName);

	if	(!checkActor(lpTarget))
		return;
	
	if	(lpTarget->m_wPitchmanShopSerial	!=	0xffff)
	{
		lpAsker->sendPartyMessage(ePM_FAILED_TO_JOIN_PARTY);
		return;
	}
	if	(lpAsker->m_wTeam != lpTarget->m_wTeam)
	{
		lpAsker->sendPartyMessage(ePM_TARGET_NOT_OWN_FORCE);
		return;
	}

	if	(lpAsker->m_wOperatorLevel >= eAL_BOSS)
	{
		lpAsker->sendPartyMessage(ePM_TARGET_NOT_EXIST);
		return;
	}

	if	(lpTarget->isDenyCommunity()	==	TRUE)
	{
		lpAsker->sendPartyMessage(ePM_TARGET_DENY_COMMUNITY_STATUS);
		return;
	}

	if	(lpTarget->getPartySerial()	!=	0xffff && !_lpPacket->bf1IsUnionPartyApplication)
	{
		lpAsker->sendPartyMessage(ePM_TARGET_BELONG_TO_ALREADY_AT_DIFFERENT_PARTY);
		return;
	}

	if	(lpAsker->getPartySerial()	!=	0xffff)	//	파티에 소속해 있다.
	{
		cParty	*lpParty	=	lpAsker->getParty();

		if (!lpParty || lpParty->m_wSerial	==	0xffff)
		{
			CLOG("party","receiveAskJoinPartyMessage");
			return;
		}

		//JBC	파티 연합인지를 체크 한다. 09-01-20
		if(_lpPacket->bf1IsUnionPartyApplication)
		{
			if	(!lpTarget->isPartyLeader())
			{
				lpAsker->sendPartyMessage(ePM_TARGET_IS_NOT_PARTY_LEADER);
				return;
			}

			if(lpParty->m_bf1IsUnionParty)
			{
				lpAsker->sendPartyMessage(ePM_ALREADY_UNION_PARTY);
			//	return;
			}

			lpTarget->sendUnionJoinParty(lpAsker);
			return;
		}
		else			//파티 연합 프로세스가 아니면.
		{
			if	(lpAsker->isPartyLeader())
			{			
				if	(lpParty->isFullParty())
				{
					lpAsker->sendPartyMessage(ePM_ALREADY_FULL_PARTY);
					return;
				}
				
				if	(lpParty->checkSecretDungeonLimitLevel(lpTarget->m_iLevel)==FALSE)
				{
					lpAsker->sendPartyMessage(ePM_ASK_JOIN_FAILED_BY_TARGET_LEVEL_OVER_THE_SECRET_DUNGEON_LIMIT_LEVEL);
					return;
				}
			}
			else //	하지만, 파티 리더는 아니다.
			{
				lpAsker->sendPartyMessage(ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_INVITATION_MESSAGE);
				return;
			}
			
			if	(lpTarget->getPartySerial()	!=	0xffff)	//	타겟이 이미 파티에 소속해 있다.
			{
				lpAsker->sendPartyMessage(ePM_TARGET_BELONG_TO_ALREADY_AT_DIFFERENT_PARTY);
				return;
			}
		}		
	}

	if	(!lpTarget || lpTarget->isDeath())	//	타겟이 죽거나 없다.
	{
		lpAsker->sendPartyMessage(ePM_TARGET_DEATH_OR_NOT_EXIST);
		return;
	}

	lpTarget->sendJoinParty(lpAsker->m_iLevel,lpAsker->m_wJob,lpAsker->m_strName);

}
//
//	파티 참가 요청 메시지에 대한 응답 메시지를 받았다.
void
cFIELD::receiveAskJoinPartyReplyMessage(CG_ASK_JOIN_PARTY_REPLY *_lpPacket,int _iSerial)
{
	cACTOR	*lpReplyer	=	getActor(_iSerial);
	cACTOR	*lpAskerForUnionParty	=	getPlayerByName(_lpPacket->strAskerName);

	if	(!checkActor(lpReplyer))
		return;
	
	if(strcmp(lpReplyer->m_strName ,_lpPacket->strAskerName) == 0)
		return;

	if(_lpPacket->bf1IsGuildMemberJoinParty)
	{
		g_game.sendWPGuildMemberJoinPartyAnswer(lpReplyer->m_strName,_lpPacket->strAskerName,_lpPacket->wReply);	
		return;
	}
 	if (_lpPacket->bf1IsUnionPartyApplication && g_iSeasonVariable >= dSEASON_VARIABLE_PARTY_UNION)
	{
 		g_pUnionpartyManager->CreateUnionPartyWork(lpReplyer , lpAskerForUnionParty);
		return;
	}

	if	(lpReplyer->m_dwRequestPartyPlayerNameHashCode	!=	GetHashCode(_lpPacket->strAskerName))
	{
		lpReplyer->m_dwRequestPartyPlayerNameHashCode	=	0xffffffff;

		lpReplyer->sendPartyMessage(ePM_FAILED_TO_JOIN_PARTY);
		return;
	}

	if	(_lpPacket->bf1IsForJoinPartyApplication)
	{
		cACTOR	*lpLeader	=	lpReplyer;

		int	iAnswer;

		if	(_lpPacket->wReply	==	0)
			iAnswer	=	eJPA_DENY;
		else
			iAnswer	=	eJPA_OK;

		lpLeader->acceptJoinPartyApplication(_lpPacket->strAskerName,iAnswer);

		return;
	}
	
	cACTOR	*lpAsker	=	getPlayerByName(_lpPacket->strAskerName);

	if	(!lpAsker)	//	파티 신청한 넘이 사라졌다.
	{
		lpReplyer->sendPartyMessage(ePM_ASKER_NOT_EXIST);
		return;
	}

	if	(_lpPacket->wReply	==	0)
	{
		lpAsker->sendPartyMessage(ePM_DENY_INVITE_MESSAGE);
		return;
	}

	if(_lpPacket->bf1IsUnionPartyApplication)
	{
		cParty	*lpParty	=	lpAsker->getParty();
	}
	else
	if	(lpAsker->getPartySerial()	!=	0xffff)	//	파티에 소속해 있다. 하지만, 파티 리더는 아니다.
	{
		if	(lpAsker->isPartyLeader())
		{
			cParty	*lpParty	=	lpAsker->getParty();

			if (!lpParty || lpParty->m_wSerial	==	0xffff)
			{
				CLOG("party","receiveAskJoinPartyReplyMessage");
				return;
			}

			if (lpParty->isFullParty())
			{
				lpReplyer->sendPartyMessage(ePM_DEST_PARTY_ARE_FULL);
				return;
			}

			if (lpParty->checkSecretDungeonLimitLevel(lpReplyer->m_iLevel)==FALSE)
			{
				lpAsker->sendPartyMessage(ePM_ASK_JOIN_FAILED_BY_TARGET_LEVEL_OVER_THE_SECRET_DUNGEON_LIMIT_LEVEL);
				return;
			}
		}
		else
		{
			lpReplyer->sendPartyMessage(ePM_DEST_ACTOR_NOT_LEADER);
			return;
		}
	}
	
	lpReplyer->m_dwRequestPartyPlayerNameHashCode	=	0xffffffff;

	cParty	*lpParty	=	lpAsker->getParty();

	if (!lpParty || lpParty->m_wSerial	==	0xffff)//	파티에 소속해 있지 않다.
	{
		g_game.sendWPCreateParty(lpAsker,lpReplyer);
		return;
	}

	g_game.sendWPJoinParty(lpParty,lpReplyer,FALSE);

}
//
//	파티에서 떠나겠어!! 라는 메시지 받았다.
void
cFIELD::receiveLeavePartyMessage(CG_LEAVE_PARTY *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if (!checkActor(lpActor))	
		return;
//	getParty에서 무결성 검사까지 다 한다.
	if (lpActor->getPartySerial()	==	0xffff)
		return;

	cParty	*lpParty	=	lpActor->getParty();	

	if (!lpParty)
	{
		CLOG("party","receiveLeavePartyMessage");
		return;
	}

	CLOG("party","receiveLeavePartyMessage [%s]",lpActor->m_strName);

	g_game.sendWPPartyWork(lpParty->m_wSerial,ePW_LEAVE_PARTY,lpActor->m_strName);
}

//
//	파티에서 쫏아낼껴!! 라는 메시지를 받았다.
void
cFIELD::receivePartyLeaderWork(CG_PARTY_LEADER_WORK *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if	(!checkActor(lpActor))
		return;

	if	(!lpActor->isPartyLeader())
	{
		lpActor->sendPartyMessage(ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_BANISH_MESSAGE);
		return;
	}

	cParty	*lpParty	=	lpActor->getParty();

	if	(!lpParty || lpParty->m_wSerial	==	0xffff)
	{
		CLOG("party","receivePartyLeaderWork");
		lpActor->sendPartyMessage(ePM_YOU_ARE_NOT_BELONG_TO_PARTY);
		return;
	}

	_lpPacket->strName[dNAME_LENGTH-1] = NULL;

	if	(STRICMP(lpActor->m_strName,_lpPacket->strName)==0)
		return;

	cPARTY_MEMBER	*lpMember	=	lpParty->getMemberInfoByName(_lpPacket->strName);

	if (lpMember	==	NULL)	//	그넘 울 파티 소속 아녀. -o-
	{
		lpActor->sendPartyMessage(ePM_TARGET_NOT_BELONG_TO_PARTY);
		return;
	}

	switch(_lpPacket->wWork)
	{
		case	ePLW_BANISH_MEMBER	:
		{
			if	(lpParty->m_dwLastTimeToDropItem+60*1000	>=	timeGetTime())
			{
				lpActor->sendPartyMessage(ePM_CAN_NOT_BANISH_MEMBER_UNDER_DROP_PARTY_ITEM_TIME);
				return;
			}

			if	(lpParty->m_wGateField	!=	0xffff)
				if (!lpMember->m_bf1IsDisconnected)
				{
					lpActor->sendPartyMessage(ePM_CAN_NOT_BANISH_CONNECT_MEMBER_IN_SECRET_DUNGEON);
					return;
				}

			g_game.sendWPPartyWork(lpParty->m_wSerial,ePW_BANISH_MEMBER,_lpPacket->strName);
			break;
		}

		case	ePLW_CHANGE_LEADER	:
		{
			if	(lpMember->m_bf1IsDisconnected)
				break;

			g_game.sendWPPartyWork(lpParty->m_wSerial,ePW_CHANGE_LEADER,_lpPacket->strName);
			break;
		}
	}
}

//
//	파티 이름을 바꿀껴!! 라는 메시지를 받았다.
void
cFIELD::receiveChangePartyNameMessage(CG_CHANGE_PARTY_NAME *_lpPacket,int _iSerial)
{
	cACTOR	*lpActor	=	getActor(_iSerial);

	if (!checkActor(lpActor))	return;

	if (!lpActor->isPartyLeader())
	{
		lpActor->sendPartyMessage(ePM_ONLY_PARTY_LEADER_CAN_SEND_AN_CHANGE_PARTY_NAME_MESSAGE);
		return;
	}

	cParty	*lpParty	=	lpActor->getParty();

	if (!lpParty || lpParty->m_wSerial	==	0xffff)
	{
		CLOG("party","receiveChangePartyNameMessage");

		lpActor->sendPartyMessage(ePM_YOU_ARE_NOT_BELONG_TO_PARTY);
		return;
	}

	g_game.sendWPChangePartyName(lpParty,_lpPacket->strName);
}