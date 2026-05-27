#include "cGAME.H"
#include "cFIELD.H"
#include "cPROJECT.H"
#include "STDAFX.H"
#include "cDECODER.H"
#include "time.H"
#include "cPACKET_GAMESERVER.h"
#include "CImageData.h"
#include "Client.h"
#include "CGuild.h"
#include "CDataSave.h"
#include "debugCode.h"
#include "secretDungeon.h"
#include "packetManager.h"
#include "instance_guild_battle_manager.h"

//
//	월드에 로그아웃 메시지 전송
void
cGAME::sendWPUserLogoutForMoveField(cACTOR *_lpActor)
{
	SERVERStoWORLD_USER_LOGOUT	cLogout;

	cLogout.base.set(sizeof(SERVERStoWORLD_USER_LOGOUT),dSERVERStoWORLD_USER_LOGOUT);
	cLogout.type			=	1;
	cLogout.iSerialInServer	=	_lpActor->m_iZoneSerial;
	strcpy(cLogout.strID,_lpActor->m_strId);
	strcpy(cLogout.strName,_lpActor->m_strName);

	CClient	*lpClient		=	_lpActor->getClient();

	if	(lpClient)
	{
		strcpy(cLogout.strIP,lpClient->ip);
		lpClient->SetStateFlag(CClient::eSTATE_MOVING);
//		_log( "HYUN!!! : id[%s] eSTATE_MOVING \n", lpClient->id);
	}
	else
		strcpy(cLogout.strIP,"EMPTY");

	g_socketWorld.SendPacket((char *)&cLogout,cLogout.base.wSize);
}


//
//	월드에 로그아웃 메시지 전송
void
cGAME::sendWPUserLogoutById(char *_lpstrId,char *_lpstrIP)
{
	SERVERStoWORLD_USER_LOGOUT	cLogout;

	cLogout.base.set(sizeof(SERVERStoWORLD_USER_LOGOUT),dSERVERStoWORLD_USER_LOGOUT);
	cLogout.type	=	0;
	strcpy(cLogout.strID,_lpstrId);
	strcpy(cLogout.strIP,_lpstrIP);

	g_socketWorld.SendPacket((char *)&cLogout,cLogout.base.wSize);
}

void
cGAME::sendWPGuildMemberJoinParty(cACTOR *_lpAsker,char *_lpstrTargetName)
{
	SERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY);

	packet.wAskerJob	=	_lpAsker->m_wJob;
	packet.wAskerLevel	=	_lpAsker->m_iLevel;

	strcpy(packet.strAskerName,_lpAsker->m_strName);
	memcpy(packet.strTargetName,_lpstrTargetName,dNAME_LENGTH);

	packet.strTargetName[dNAME_LENGTH-1]	=	NULL;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPGuildMemberJoinPartyAnswer(char *_lpstrAnswerName,char *_lpstrAskerName,int _iAnswer)
{
	SERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY_ANSWER	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_GUILD_MEMBER_JOIN_PARTY_ANSWER);

	packet.wAnswer		=	_iAnswer;

	memcpy(packet.strTargetName,_lpstrAnswerName,dNAME_LENGTH);
	memcpy(packet.strAskerName,_lpstrAskerName,dNAME_LENGTH);

	packet.strTargetName[dNAME_LENGTH-1]	=	NULL;
	packet.strAskerName[dNAME_LENGTH-1]		=	NULL;

	SEND_WOLRD_PACKET(&packet);
}
//
//	이벤트 메시지 전송?
void
cGAME::sendWPEventMessage(char *_lpstrText,int _iChatType)
{
	SERVERStoWORLD_EVENT_MESSAGE	packet;

	packet.base.set(sizeof(SERVERStoWORLD_EVENT_MESSAGE),dSERVERStoWORLD_EVENT_MESSAGE);
	packet.wType	=	_iChatType;

	strncpy(packet.strMessage,_lpstrText,sizeof(packet.strMessage)-2);

	g_socketWorld.SendPacket((char *)&packet,packet.base.wSize);
}

//
//	파티 메시지 전송
void
cGAME::sendPartyMessage(CClient *_lpClient,int _iMessage)
{
	if	(!_lpClient)
		return;

	SG_PARTY_MESSAGE	packet;

	packet.base.set(sizeof(SG_PARTY_MESSAGE),dSG_PARTY_MESSAGE);

	packet.wMessage	=	_iMessage;
	
	g_userPM.add(_lpClient->wSerial,&packet,packet.base.wSize);
}

//
//	파티 정보 요청
void
cGAME::sendWPAskPartyInfo(int _iPartySerial,char *_lpstrPartyName)
{
	if (_iPartySerial	==	0xffff)
		return;

	SERVERStoWORLD_ASK_PARTYINFO	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ASK_PARTYINFO);
	packet.wPartySerial		=	_iPartySerial;

	if	(_lpstrPartyName)
		strcpy(packet.strPartyName,_lpstrPartyName);
	else
		strcpy(packet.strPartyName,"");

	SEND_WOLRD_PACKET(&packet);
}

//
//	필드를 이동해도 유지되어야 할 데이터 전송
void
cGAME::sendWPUpkeepPlayerData(cACTOR *_lpActor)
{
	SERVERStoWORLD_USER_MOVEMAP_INFO	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_MOVEMAP_INFO);
	packet.dwSerial		=	CM.GET_WORLDSERIAL(_lpActor->m_iClientSerial);

	if	(packet.dwSerial	==	0xffffffff)
		return;

	packet.wIsNetCafeUser=	FALSE;

	CClient	*lpClient	=	_lpActor->getClient();

	if	(lpClient)
		packet.wIsNetCafeUser	=	lpClient->GetIsNetCafeUser();

	_lpActor->getUpkeepStatus(&packet.upkeepStatus);
	memcpy(&packet.upkeepPlayerData,_lpActor->getUpkeepPlayerDataPointer(),sizeof(CUpkeepPlayerData));

	packet.upkeepPlayerData.m_wMachine	=	0xffff;

	SEND_WOLRD_PACKET(&packet);
}


//
//	월드에 파티 이탈 메시지 보내기
void
cGAME::sendWPPartyWork(int _iPartySerial,int _iPartyWork,char *_lpstrMemberName)
{
	cParty	*lpParty;

	lpParty	=	g_pPartyManager->get(_iPartySerial);

	if (!lpParty)
		return;

	if (!lpParty->getMemberInfoByName(_lpstrMemberName))
		return;

	SERVERStoWORLD_PARTY_WORK	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_PARTY_WORK);

	packet.wPartySerial		=	_iPartySerial;
	packet.wWork			=	_iPartyWork;
	packet.dwUniqueSerial	=	lpParty->m_dwUniqueSerial;

	strcpy(packet.strName,_lpstrMemberName);

	SEND_WOLRD_PACKET(&packet);
}

//
//	월드에 파티 멤버 디스 컨넥트 메시지 보내기
void
cGAME::sendWPDisconnectPartyMember(int _iPartySerial,char *_lpstrPartyName,char *_lpstrMemberName)
{
	cParty	*lpParty;

	if (_lpstrPartyName)
		lpParty	=	g_pPartyManager->get(_iPartySerial,_lpstrPartyName);
	else
		lpParty	=	g_pPartyManager->get(_iPartySerial);

	if (!lpParty)
		return;

	if (!lpParty->getMemberInfoByName(_lpstrMemberName))
		return;

	SERVERStoWORLD_DISCONNECT_PARTY_MEMBER	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_DISCONNECT_PARTY_MEMBER);

	packet.wPartySerial		=	_iPartySerial;

	if (_lpstrPartyName)
		strcpy(packet.strPartyName,_lpstrPartyName);
	else
		packet.strPartyName[0]	=	NULL;

	strcpy(packet.strName,_lpstrMemberName);

	SEND_WOLRD_PACKET(&packet);
}

//
//
void
cGAME::sendWPEnterIFParty(int _iPartySerial,char *_lpstrPartyName,int _iGateField,int _iGateArea)
{
	cParty	*lpParty;

	if (_lpstrPartyName)
		lpParty	=	g_pPartyManager->get(_iPartySerial,_lpstrPartyName);
	else
		lpParty	=	g_pPartyManager->get(_iPartySerial);

	if (!lpParty)
		return;

	SERVERStoWORLD_PARTY_ENTER_IF	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_PARTY_ENTER_IF);

	packet.wPartySerial		=	_iPartySerial;

	if (_lpstrPartyName)
		strcpy(packet.strPartyName,_lpstrPartyName);
	else
		packet.strPartyName[0]	=	NULL;

	packet.wGateField		=	_iGateField;
	packet.wGateArea		=	_iGateArea;

	SEND_WOLRD_PACKET(&packet);
}


//
//	아이템 추가
void
cGAME::sendWPAddItem(cACTOR *_lpActor,cItem *_lpItem)
{
}

//
//	유저 찾기
void
cGAME::sendWPFindUser(char *_lpstrClientName,char *_lpstrTargetName,int _iField,int _iWork)
{
	SERVERStoWORLD_USER_FIND	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_FIND);

	if	(strlen(_lpstrTargetName) >= dNAME_LENGTH)
		return;

	strcpy(packet.strName,_lpstrClientName);
	strcpy(packet.strFindName,_lpstrTargetName);
	packet.wWork			=	_iWork;
	packet.wField			=	_iField;

	SEND_WOLRD_PACKET(&packet);
}

BOOL
cGAME::sendWPMoveField(cACTOR *_lpActor,char *_lpstrMapname,int _iField,int _iGateIndex,int _iX,int _iY,BOOL _bIsInnerMove,BOOL _bIsMoveNow)
{
	cFIELD	*lpField			=	_lpActor->m_lpField;
	int		iTargetFieldSerial	=	0xffff;

	if	(_lpstrMapname	&&	_bIsInnerMove	==	FALSE	&&	lpField->m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
	{
		cInstanceField	*lpIF		=	lpField->getMasterInstanceField();

		int				iFloorIndex	=	lpIF->getFloorIndex(_lpstrMapname);

		if	(iFloorIndex	!=	-1)
		{
			_lpActor->enterToTheInstanceField(lpIF->m_wSerial,iFloorIndex,-1,_iGateIndex);

			return	TRUE;
		}
	}

	if	(_lpstrMapname && _lpstrMapname[0])
		iTargetFieldSerial	=	atoi(_lpstrMapname+1);

	if	(_bIsInnerMove)
		iTargetFieldSerial	=	_iField;
	else
	if	(_iField	==	0xffff)
		_iField		=	iTargetFieldSerial;

	if	(isCloseMap(_iField))
	{
		MessageBox(NULL,"isCloseMap","",MB_OK);
		_lpActor->sendMoveFieldResult(dMOVEMAP_RESULT_NOTFIND);
		return	FALSE;
	}

	if	(lpField  && lpField->m_iUniqueSerial == _iField)
	{
		if	(_iX == -1 || _iY == -1)
		{
			cAreaInfo	*lpArea	=	lpField->m_pArea->getStartArea();
			cPOINT		pos;

			if	(!lpArea)
				lpArea	=	lpField->m_pArea->getFirstPortal();

			if	(lpArea)
			{
				lpArea->getCenterPos(&pos);

				_iX	=	pos.x;
				_iY	=	pos.y;
			}
		}

		if	(_iX != -1 && _iY != -1)
		{
			if	(_lpActor->setActorPos(_iX,_iY,TRUE,TRUE))
			{
				_lpActor->sendCurrentPosition(TRUE);
				_lpActor->sendMoveFieldResult(10000);

				return	TRUE;
			}
		}
	}

	DWORD	dwWorldSerial=	CM.GET_WORLDSERIAL(_lpActor->m_iClientSerial);

	if	(dwWorldSerial	==	0xffffffff)
	{
		_lpActor->sendMoveFieldResult(dMOVEMAP_RESULT_UNKNOWN_ERROR);

		return	FALSE;
	}

	if	(_lpActor->m_wWaitBankDataSaveResultTime || _lpActor->m_wWaitBuyCarrotShopItemResultTime)	//	은행 거래중에는 필드 이동 안돼
		return	FALSE;

	CClient	*lpClient	=	_lpActor->getClient();

	if	(_bIsMoveNow)
	{
		_lpActor->m_bIsReadyToExit	=	TRUE;
		_lpActor->immediatelySendSaveDataToDBMemory(TRUE);

//		if	(lpClient)
//			lpClient->SetIsUsed(eISUSED_SERVERStoWORLD_USER_MOVEMAP);
	}
	
	SERVERStoWORLD_USER_MOVEMAP	packetMove;

	packetMove.base.set(sizeof(SERVERStoWORLD_USER_MOVEMAP),dSERVERStoWORLD_USER_MOVEMAP);

	if	(_bIsMoveNow	==	FALSE)
		packetMove.base.set(sizeof(SERVERStoWORLD_USER_MOVEMAP),dSERVERStoWORLD_CHECK_USER_MOVEMAP);

	packetMove.iWorldSerial		=	dwWorldSerial;				//	해당 게임 서버에서의 시리얼
	packetMove.iSerialInServer	=	_lpActor->m_iZoneSerial;	//	존 시리얼
	packetMove.wFieldSerial		=	_iField;					//	필드의 고유 시리얼
	packetMove.wGateIndex		=	_iGateIndex;
	packetMove.wXPos			=	_iX;
	packetMove.wYPos			=	_iY;
	//JBC	미니 게이트 글로브와 포탈스피어 중복시 이동 문제.	08-09-04
	packetMove.wIsPremiumMember =	_lpActor->getMaxPremiumItemLevel();

	if	(_lpstrMapname	&&	_lpstrMapname[0])
		strcpy(	packetMove.strMapName,_lpstrMapname);
	else
		packetMove.strMapName[0]	=	NULL;

	if	(_lpActor->m_wOperatorLevel >= eAL_NORMAL || g_bIsTestServer)
		packetMove.wIsPremiumMember	=	TRUE;		

	SEND_WOLRD_PACKET(&packetMove);
//	CLOG("MoveMap",_ms("sendWPMoveField - [%s] field[%d : %s]",_lpActor->m_strName,_iField, _lpstrMapname));
	return	TRUE;
}

void
cGAME::sendWPMoveFieldPartyMember(char *_lpstrPartyname,char *_lpstrName,int _iDestField,int _iZoneSerial,BOOL _bIsLeave,int _iDestInstanceField,int _iDestInstanceFieldFloor)
{
	SERVERStoWORLD_MOVE_FIELD_PARTY_MEMBER	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_MOVE_FIELD_PARTY_MEMBER);

	strcpy(packet.strPartyName,_lpstrPartyname);
	strcpy(packet.strName,_lpstrName);

	packet.bf1IsLeave				=	_bIsLeave;	//	그 필드에서 떠나는거냐?
	packet.wDestFieldSerial			=	_iDestField;	//	목적지
	packet.iZoneSerial				=	_iZoneSerial;
	packet.bf10InstanceField		=	_iDestInstanceField;
	packet.bf4InstanceFieldFloor	=	_iDestInstanceFieldFloor;

	SEND_WOLRD_PACKET(&packet);
}

//
//	아이템의 유니크 시리얼을 요구
void
cGAME::sendWPAskItemUniqueSerial(cACTOR *_lpActor)
{
	SERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL	packet;

	packet.base.set(sizeof(SERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL),dSERVERStoWORLD_ASK_ITEM_UNIQUE_SERIAL);
	packet.iSerialInServer	=	_lpActor->m_iZoneSerial;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPSetGuildHallRoenGoodWillInfo(CGuild *_lpGuild,int _wGoodWill)
{
	SERVERStoWORLD_SET_GUILD_HALL_ROEN_GOOD_WILL	packet;
	
	packet.base.set(sizeof(packet),dSERVERStoWORLD_SET_GUILD_HALL_ROEN_GOOD_WILL);
	
	packet.wGoodWill	=	_wGoodWill;
	strcpy(packet.strName,_lpGuild->m_strName);
	
	SEND_WOLRD_PACKET(&packet);
}


void
cGAME::sendWPSetGuildHallLevel(CGuild	*_lpGuild,int _iLevel)
{
	SERVERStoWORLD_SET_GUILD_HALL_LEVEL	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_SET_GUILD_HALL_LEVEL);

	packet.wLevel	=	_iLevel;
	strcpy(packet.strName,_lpGuild->m_strName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangeGuildInventoryPermitWithdrawItemCount(int _iGuild,int _iStorage,int _iRank,int _iCount)
{
	SERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_ITEM		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_ITEM);

	packet.wStorage		=	_iStorage;
	packet.iGuild		=	_iGuild;
	packet.wRank		=	_iRank;
	packet.wItemCount	=	_iCount;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangeGuildInventoryPermitWithdrawGold(int _iGuild,int _iRank,int _iGold)
{
	SERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_GOLD		packet;	

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_GI_PERMIT_WITHDRAW_GOLD);

	packet.iGuild		=	_iGuild;
	packet.wRank		=	_iRank;
	packet.iGold		=	_iGold;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangeGuildInventoryStorageSize(int _iGuild,int _iCount1,int _iCount2,int _iCount3,int _iCount4)
{
	SERVERStoWORLD_CHANGE_GI_STORAGE_SIZE				packet;
	
	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_GI_STORAGE_SIZE);

	packet.iGuild			=	_iGuild;
	packet.abStorageSize[0]	=	_iCount1;
	packet.abStorageSize[1]	=	_iCount2;
	packet.abStorageSize[2]	=	_iCount3;
	packet.abStorageSize[3]	=	_iCount4;

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드 생성
void
cGAME::sendWPCreateGuild(cACTOR *_lpMaster,char *_lpstrGuildName,char *_lpstrSubMasterName)
{
	SERVERStoWORLD_USER_CREATEGUILD	packet;

	packet.base.set(sizeof(SERVERStoWORLD_USER_CREATEGUILD),dSERVERStoWORLD_USER_CREATEGUILD);

	strcpy(packet.strName,_lpstrGuildName);
	strcpy(packet.strMasterId,_lpMaster->m_strId);
	strcpy(packet.strMasterName,_lpMaster->m_strName);
	strcpy(packet.strSubMasterName,_lpstrSubMasterName);

	packet.wMasterJob	=	_lpMaster->m_wJob;
	packet.wMasterLevel	=	_lpMaster->m_iLevel;

	SEND_WOLRD_PACKET(&packet);
}

//
//	월드에 길드 참가 신청 메시지 전송
void
cGAME::sendWPJoinGuild(char *_lpstrGuildName,char *_lpstrMasterName,cACTOR *_lpAsker,int _iRank)
{
	SERVERStoWORLD_USER_JOINGUILD	packet;
	
	strcpy(packet.strName		,_lpstrGuildName);
	strcpy(packet.strMasterName	,_lpstrMasterName);
	strcpy(packet.strTargetId	,_lpAsker->m_strId);
	strcpy(packet.strTargetName	,_lpAsker->m_strName);
	
	packet.wJob		=	_lpAsker->m_wJob;
	packet.wLevel	=	_lpAsker->m_iLevel;
	packet.wRank	=	_iRank;

	packet.base.set(sizeof(SERVERStoWORLD_USER_JOINGUILD),dSERVERStoWORLD_USER_JOINGUILD);

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드 스킬레벨 증가
void
cGAME::sendWPIncreaseGuildSkillLevel(int _iGuild,int _iSkill)
{
	SERVERStoWORLD_INCREASE_GUILD_SKILL_LEVEL	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_INCREASE_GUILD_SKILL_LEVEL);
	packet.wGuildSerial		=	_iGuild;
	packet.wSkill			=	_iSkill;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPAskDeleteGuild(cACTOR *_lpActor,char *_lpstrGuildName,int _iGuildSerial)
{
	SERVERStoWORLD_USER_DELETEGUILD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_DELETEGUILD);

	packet.wGuildSerial	=	_iGuildSerial;
	strcpy(packet.strName,_lpstrGuildName);
	strcpy(packet.strRequestMan,_lpActor->m_strName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangeGuildNotice(cACTOR *_lpActor,char *_lpstrNotice)
{
	SERVERStoWORLD_UPDATE_NOTICE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_NOTICE);
	
	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	strcpy(packet.strMaster,_lpActor->m_strName);
	packet.wType	=	0;
	strcpy(packet.strNotice,_lpstrNotice);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangeGuildExpTax(cACTOR *_lpActor,int _iTax)
{
	SERVERStoWORLD_UPDATE_EXPRATE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_EXPRATE);
	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	strcpy(packet.strMaster,_lpActor->m_strName);
	packet.wRateExp	=	_iTax;

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드 경험치 세율
void
cGAME::sendWPChangeGuildExpTax(CGuild *_lpGuild,int _iTax)
{
	SERVERStoWORLD_UPDATE_EXPRATE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_EXPRATE);
	packet.wGuildSerial	=	_lpGuild->m_wSerial;
	strcpy(packet.strMaster,_lpGuild->m_strMasterName);
	packet.wRateExp		=	_iTax;

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드 경험치 세율
void
cGAME::sendWPMakeGuildLevel(CGuild *_lpGuild,int _iLevel)
{
	SERVERStoWORLD_MAKE_GUILD_LEVEL	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_MAKE_GUILD_LEVEL);
	packet.wGuildSerial	=	_lpGuild->m_wSerial;
	strcpy(packet.strMaster,_lpGuild->m_strMasterName);
	packet.wLevel		=	_iLevel;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPDismissSubGuildMaster(cACTOR *_lpActor,char *_lpstrName)
{
	SERVERStoWORLD_USER_CHANGERANK		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);

	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	strcpy(packet.strMaster,_lpActor->m_strName);
	strcpy(packet.strTarget,_lpstrName);

	packet.wType	=	dGUILD_DISMISSAL;
	packet.wRank	=	dGUILD_CLASS_SUBMASTER;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPAppointmentSubGuildMaster(cACTOR *_lpActor,char *_lpstrName)
{
	SERVERStoWORLD_USER_CHANGERANK		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);

	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	strcpy(packet.strMaster,_lpActor->m_strName);
	strcpy(packet.strTarget,_lpstrName);

	packet.wType	=	dGUILD_APPONT;
	packet.wRank	=	dGUILD_CLASS_SUBMASTER;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPDismissGuildCongress(cACTOR *_lpActor,char *_lpstrName)
{
	SERVERStoWORLD_USER_CHANGERANK		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);

	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	strcpy(packet.strMaster,_lpActor->m_strName);
	strcpy(packet.strTarget,_lpstrName);

	packet.wType	=	dGUILD_DISMISSAL;
	packet.wRank	=	dGUILD_CLASS_PARLIAMENT;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPAppointmentGuildCongress(cACTOR *_lpActor,char *_lpstrName)
{
	SERVERStoWORLD_USER_CHANGERANK		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);

	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	strcpy(packet.strMaster,_lpActor->m_strName);
	strcpy(packet.strTarget,_lpstrName);

	packet.wType	=	dGUILD_APPONT;
	packet.wRank	=	dGUILD_CLASS_PARLIAMENT;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangeGuildMaster(cACTOR *_lpActor,char *_lpstrName)
{
	SERVERStoWORLD_USER_CHANGERANK		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);

	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	strcpy(packet.strMaster,_lpActor->m_strName);
	strcpy(packet.strTarget,_lpstrName);

	packet.wType	=	dGUILD_APPONT;
	packet.wRank	=	dGUILD_CLASS_MASTER;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangeGuildRank(CGuild	*_lpGuild,char *_lpstrMemberName,int _iNewRank)
{
	SERVERStoWORLD_USER_CHANGERANK		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);

	packet.wGuildSerial	=	_lpGuild->m_wSerial;

	strcpy(packet.strMaster,_lpGuild->m_strMasterName);
	strcpy(packet.strTarget,_lpstrMemberName);

	packet.wType	=	dGUILD_APPONT;
	packet.wRank	=	_iNewRank;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPRegistRegular(cACTOR *_lpActor,char *_lpstrName)
{
	SERVERStoWORLD_USER_CHANGERANK		packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_CHANGERANK);

	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;
	strncpy(packet.strMaster,_lpActor->m_strName,dNAME_LENGTH);
	strncpy(packet.strTarget,_lpstrName,dNAME_LENGTH);

	packet.wType	=	dGUILD_APPONT;
	packet.wRank	=	dGUILD_CLASS_MEMBER;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPAskGuildInfo(int _iSerial)
{
	SERVERStoWORLD_ASK_GUILD_INFO	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ASK_GUILD_INFO);
	packet.wGuildSerial	=	_iSerial;

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드전 스케쥴 요청
void
cGAME::sendWPAskGuildBattleSchedule()
{
	SERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ASK_GUILD_BATTLE_SCHEDULE);

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드전 예약 요청
void
cGAME::sendWPRequestGuildBattle(cACTOR *_lpActor,int _iTimeOrder,int _iFieldOrder)
{
	SERVERStoWORLD_BOOKING_GUILD_BATTLE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_BOOKING_GUILD_BATTLE);

	packet.wGuildSerial			=	_lpActor->m_wGuildSerial;
	packet.wTimeOrder			=	_iTimeOrder;
	packet.wFieldOrder			=	_iFieldOrder;
	packet.wCurrentBattleOrder	=	g_gbsToday.getCurrentBattleOrder(_iFieldOrder);

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드전 예약 요청
void
cGAME::sendWPRemoveGuildGold(int _iGuild,int _iGold)
{
	SERVERStoWORLD_ADD_GUILD_GOLD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ADD_GUILD_GOLD);

	packet.wGuildSerial	=	_iGuild;
	packet.iGold		=	-_iGold;

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드전 예약 요청
void
cGAME::sendWPAddGuildGold(int _iGuild,int _iGold)
{
	SERVERStoWORLD_ADD_GUILD_GOLD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ADD_GUILD_GOLD);

	packet.wGuildSerial	=	_iGuild;
	packet.iGold		=	_iGold;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPCancelGuildBattle(int _iGuild,int _iTimeOrder,int _iFieldOrder)	//	길드전 취소 요청
{
	SERVERStoWORLD_CANCEL_GUILD_BATTLE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CANCEL_GUILD_BATTLE);

	packet.wGuildSerial	=	_iGuild;
	packet.wTimeOrder	=	_iTimeOrder;
	packet.wFieldOrder	=	_iFieldOrder;

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드전 취소 요청
void
cGAME::sendWPCancelGuildBattle(cACTOR *_lpActor,int _iTimeOrder,int _iFieldOrder)
{
	int	iGuild	=	0xffff;

	if	(_lpActor)
		iGuild	=	_lpActor->m_wGuildSerial;

	sendWPCancelGuildBattle(iGuild,_iTimeOrder,_iFieldOrder);//	길드전 취소 요청
}

//
//	특정 시간에 길드전 정보를 요청한다.
void
cGAME::sendWPAskGuildBattleInfo(int _iTimeOrder,int _iFieldOrder)
{
	SERVERStoWORLD_ASK_GUILD_BATTLE_INFO	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ASK_GUILD_BATTLE_INFO);

	packet.wTimeOrder	=	_iTimeOrder;
	packet.wFieldOrder	=	_iFieldOrder;

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드전 결과
void
cGAME::sendWPGuildBattleResult(int _iGuild1,int _iGuild2,BOOL _bIsWinGuild1,BOOL _bIsWinGuild2)
{
	SERVERStoWORLD_GUILD_BATTLE_RESULT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_GUILD_BATTLE_RESULT);
	
	CGuild	*lpGuild1		=	g_guildManager.getBySerial(_iGuild1);
	CGuild	*lpGuild2		=	g_guildManager.getBySerial(_iGuild2);

	if (!lpGuild1 || !lpGuild2)
		return;

	packet.abWin[0]			=	_bIsWinGuild1;
	packet.abWin[1]			=	_bIsWinGuild2;
	packet.awGuildSerial[0]	=	_iGuild1;
	packet.awGuildSerial[1]	=	_iGuild2;

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드 마크 선택
void
cGAME::sendWPSelectGuildMark(int _iSerial,CGuildMarkInfo *_lpMarkInfo,BOOL _bIsUseGuildMarkPack)
{
	SERVERStoWORLD_SELECT_GUILD_MARK	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_SELECT_GUILD_MARK);
	packet.wGuildSerial				=	_iSerial;
	packet.bf1IsUseGuildMarkPack	=	_bIsUseGuildMarkPack;

	memcpy(&packet.guildMarkInfo,_lpMarkInfo,sizeof(CGuildMarkInfo));

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPUpdateGuildMemberLevel(int _iSerial,char *_lpstrName,int _iLevel)
{
	SERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_UPDATE_GUILD_MEMBER_LEVEL);
	packet.wGuildSerial	=	_iSerial;
	strcpy(packet.strName,_lpstrName);
	packet.wLevel		=	_iLevel;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPBanishGuildMember(cACTOR *_lpActor,char *_lpstrName)
{
	SERVERStoWORLD_USER_EXITGUILD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_EXITGUILD);

	CGuild	*lpGuild	=	_lpActor->getGuild();

	if	(!lpGuild)
		return;

	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;

	strcpy(packet.strMasterName,_lpActor->m_strName);
	strcpy(packet.strTargetName,_lpstrName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPLeaveGuild(cACTOR *_lpActor)
{
	SERVERStoWORLD_USER_EXITGUILD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_USER_EXITGUILD);

	CGuild	*lpGuild	=	_lpActor->getGuild();

	if	(!lpGuild)
		return;

	packet.wGuildSerial	=	_lpActor->m_wGuildSerial;

	strcpy(packet.strMasterName,_lpActor->m_strName);
	strcpy(packet.strTargetName,_lpActor->m_strName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPCheckGuildData(WORD *_lpwCheckGuild,int *_lpiCheckSum,int _iCount,DWORD _dwTodayCheckSum)
{
	SERVERStoWORLD_CHECK_GUILD_CHECKSUM	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHECK_GUILD_CHECKSUM);

	packet.wGuildCount				=	_iCount;
	packet.dwTodayGBS_CheckSum		=	_dwTodayCheckSum;
	packet.dwGuildHallCheckSum		=	g_guildManager.getGuildHallCheckSum();

	memcpy(packet.awCheckGuild,_lpwCheckGuild,sizeof(packet.awCheckGuild));
	memcpy(packet.aiCheckSum,_lpiCheckSum,sizeof(packet.aiCheckSum));

	SEND_WOLRD_PACKET(&packet);
}

//
//	길드 스킬의 상태 변경
void
cGAME::sendWPUpdateGuildSkillStatus(int _iGuild,int _iSkill,BOOL _bIsDisable)
{
/*	SERVERStoWORLD_CHANGE_GUILD_SKILL_STATUS	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_GUILD_SKILL_STATUS);

	packet.wGuildSerial	=	_iGuild;
	packet.wSkill		=	_iSkill;
	packet.wIsDisable	=	_bIsDisable;

	SEND_WOLRD_PACKET(&packet);
*/
}


void
cGAME::sendWPStoreGuildInventoryItem(int _iGuild,int _iGuildInventorySlot,cItem *_lpItem,char *_lpstrMemberName,int _iInventorySlot)//	길드에 인벤에 아이템 넣기
{
	SERVERStoWORLD_STORE_GUILD_INVENTORY_ITEM	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_STORE_GUILD_INVENTORY_ITEM);

	packet.iGuild				=	_iGuild;
	packet.iGuildInventorySlot	=	_iGuildInventorySlot;
	packet.wInventorySlot		=	_iInventorySlot;
	strcpy(packet.strMemberName,_lpstrMemberName);
	memcpy(&packet.item,_lpItem,sizeof(cItem));

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPAddRelatedPlaceOfGuild(int _iGuild,int _iFieldSerial,int _iGold,BOOL _bIsAdd,char *_lpstrName)
{
	SERVERStoWORLD_ADD_RELATED_PLACE_OF_GUILD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ADD_RELATED_PLACE_OF_GUILD);

	packet.iGuild		=	_iGuild;
	packet.iFieldSerial	=	_iFieldSerial;
	packet.iGold		=	_iGold;
	packet.wIsAdd		=	_bIsAdd;
	strcpy(packet.strMemberName,_lpstrName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPRecordGuildHonorPoint(int _iGuild,int _iGuildHonorPoint)
{
	SERVERStoWORLD_RECORD_GUILD_HONOR_POINT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_RECORD_GUILD_HONOR_POINT);

	packet.iGuild			=	_iGuild;
	packet.iGuildHonorPoint	=	_iGuildHonorPoint;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPWithdrawGuildInventoryItem(int _iGuild,int _iGuildInventorySlot,char *_lpstrName,cItem *_lpItem)//	길드 인벤에서 아이템 꺼내기
{
	SERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_ITEM	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_ITEM);

	packet.iGuild				=	_iGuild;
	packet.iGuildInventorySlot	=	_iGuildInventorySlot;
	memcpy(&packet.item,_lpItem,sizeof(CItemDefine));
	strcpy(packet.strMemberName,_lpstrName);
	
	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPStoreGuildInventoryGold(int _iGuild,int _iGold,char *_lpstrMemberName)//	길드 인벤에 골드 넣기
{
	SERVERStoWORLD_STORE_GUILD_INVENTORY_GOLD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_STORE_GUILD_INVENTORY_GOLD);

	packet.iGuild				=	_iGuild;
	packet.iGold				=	_iGold;
	strcpy(packet.strMemberName,_lpstrMemberName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPWithdrawGuildInventoryGold(int _iGuild,int _iGold,char *_lpstrName)//	길드 인벤에서 골드 꺼내기
{
	SERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_GOLD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_WITHDRAW_GUILD_INVENTORY_GOLD);

	packet.iGuild				=	_iGuild;
	packet.iGold				=	_iGold;
	strcpy(packet.strMemberName,_lpstrName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangeGuildInventoryItemPlace(int _iGuild,int _iSlot1,int _iSlot2,char *_lpstrName)//	길드 인벤 안에 아이템 위치 변경
{
	SERVERStoWORLD_CHANGE_GUILD_INVENTORY_ITEM_PLACE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_GUILD_INVENTORY_ITEM_PLACE);

	packet.iGuild	=	_iGuild;
	packet.iSlot1	=	_iSlot1;
	packet.iSlot2	=	_iSlot2;
	strcpy(packet.strMemberName,_lpstrName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPCheckPartyInfoByActor(cACTOR *_lpActor)
{
	SERVERStoWORLD_CHECK_PARTY_INFO_BY_ACTOR	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHECK_PARTY_INFO_BY_ACTOR);
	packet.wPartySerial	=	_lpActor->getPartySerial();
	strcpy(packet.strName,_lpActor->m_strName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPChangePartyStatus(cParty *_lpParty,BOOL _bIsChangeSetting)
{
	SERVERStoWORLD_CHANGE_PARTY_STATUS	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_PARTY_STATUS);

	packet.wPartySerial				=	_lpParty->m_wSerial;
	packet.dwUniqueSerial			=	_lpParty->m_dwUniqueSerial;

	packet.bf1IsChangeSetting		=	_bIsChangeSetting;
	packet.bf3GoldShareMethod		=	_lpParty->m_bf3GoldShareMethod;
	packet.bf3ItemShareMethod		=	_lpParty->m_bf3ItemShareMethod;
	packet.bf3PartyObject			=	_lpParty->m_bf3PartyObject;
	packet.bf1IsOpenParty			=	_lpParty->m_bf1IsOpenParty;
	packet.bf3LastReceiveGoldMember	=	_lpParty->m_bf3LastReceiveGoldMember;
	packet.bf3LastReceiveItemMember	=	_lpParty->m_bf3LastReceiveItemMember;

	SEND_WOLRD_PACKET(&packet);
}
//
//	월드에 파티 생성 메시지 보내기
void
cGAME::sendWPCreateParty(cACTOR *_lpLeader,cACTOR *_lpMember)
{
	SERVERStoWORLD_CREATE_PARTY	packet;

	packet.base.set(sizeof(SERVERStoWORLD_CREATE_PARTY),dSERVERStoWORLD_CREATE_PARTY);

	CClient	*lpLeaderClient	=	CM.mGETCLIENT(_lpLeader->m_iClientSerial);
	CClient	*lpMemberClient	=	CM.mGETCLIENT(_lpMember->m_iClientSerial);

	if	(!lpLeaderClient	||	!lpMemberClient)
		return;	//	둥중 한넘이 맛탱이 갔다.

	packet.bf10MasterLevel	=	_lpLeader->m_iLevel;
	packet.bf8MasterJob		=	_lpLeader->m_wJob;
	packet.bf8MasterRemainHP=	_lpLeader->m_iHP*100/_lpLeader->getMaxHP();
	packet.iMasterZoneSerial=	_lpLeader->m_iZoneSerial;
	packet.bf4MasterSecretDungeonPlayCount	=	_lpLeader->m_bf4SecretDungeonPlayCount;
	packet.iMasterDuelVP	=	_lpLeader->m_duelRecord.m_iWeeklyWinPoint;
	strcpy(packet.strName,_lpLeader->m_strName);

	packet.bf10TargetLevel	=	_lpMember->m_iLevel;
	packet.bf8TargetJob		=	_lpMember->m_wJob;
	packet.bf8TargetRemainHP=	_lpMember->m_iHP*100/_lpMember->getMaxHP();
	packet.iTargetZoneSerial=	_lpMember->m_iZoneSerial;
	packet.bf4TargetSecretDungeonPlayCount	=	_lpMember->m_bf4SecretDungeonPlayCount;
	packet.iTargetDuelVP	=	_lpMember->m_duelRecord.m_iWeeklyWinPoint;
	strcpy(packet.strTName,_lpMember->m_strName);

	packet.wFieldSerial		=	_lpLeader->m_wCurrentField;

	SEND_WOLRD_PACKET(&packet);
}

//
//	월드에 파티 참가 메시지 보내기
void
cGAME::sendWPJoinParty(cParty *_lpParty,cACTOR *_lpActor,BOOL _bIsReconnect)
{
	SERVERStoWORLD_JOIN_PARTY	packet;

	packet.base.set(sizeof(SERVERStoWORLD_JOIN_PARTY),dSERVERStoWORLD_JOIN_PARTY);

	CClient	*lpJoinerClient	=	CM.mGETCLIENT(_lpActor->m_iClientSerial);

	if	(!lpJoinerClient)
		return;

	packet.wPartySerial					=	_lpParty->m_wSerial;	//	파티 시리얼
	packet.bf1IsReconnect				=	_bIsReconnect;
	packet.bf1IsByJoinPartyApplication	=	FALSE;

	packet.wFieldSerial					=	_lpActor->m_wCurrentField;
	packet.iZoneSerial					=	_lpActor->m_iZoneSerial;
	packet.bf8Job						=	_lpActor->m_wJob;
	packet.bf10Level					=	_lpActor->m_iLevel;
	packet.bf10InstanceField			=	_lpActor->m_bf8InstanceField;
	packet.bf4InstanceFieldFloor		=	_lpActor->m_bf4InstanceFieldFloor;
	packet.bf7RemainHP					=	_lpActor->m_iHP*100/_lpActor->getMaxHP();
	packet.bf4SecretDungeonPlayCount	=	_lpActor->m_bf4SecretDungeonPlayCount;
	packet.iDuelVP						=	_lpActor->m_duelRecord.m_iWeeklyWinPoint;

	strcpy(packet.strName,_lpActor->m_strName);
	strcpy(packet.strPartyName,_lpParty->m_strPartyName);

	SEND_WOLRD_PACKET(&packet);
}

//
//	월드에 파티 참가 메시지 보내기
void
cGAME::sendWPJoinPartyByApplication(char *_lpstrPartyName,int _iPartySerial,cACTOR *_lpActor)
{
	SERVERStoWORLD_JOIN_PARTY	packet;

	packet.base.set(sizeof(SERVERStoWORLD_JOIN_PARTY),dSERVERStoWORLD_JOIN_PARTY);

	CClient	*lpJoinerClient	=	CM.mGETCLIENT(_lpActor->m_iClientSerial);

	if	(!lpJoinerClient)
		return;

	packet.bf1IsReconnect				=	FALSE;
	packet.bf1IsByJoinPartyApplication	=	TRUE;

	packet.wFieldSerial					=	_lpActor->m_wCurrentField;
	packet.iZoneSerial					=	_lpActor->m_iZoneSerial;
	packet.bf8Job						=	_lpActor->m_wJob;
	packet.bf10Level					=	_lpActor->m_iLevel;
	packet.bf10InstanceField			=	_lpActor->m_bf8InstanceField;
	packet.bf4InstanceFieldFloor		=	_lpActor->m_bf4InstanceFieldFloor;
	packet.bf7RemainHP					=	_lpActor->m_iHP*100/_lpActor->getMaxHP();
	packet.bf4SecretDungeonPlayCount	=	_lpActor->m_bf4SecretDungeonPlayCount;
	strcpy(packet.strName,_lpActor->m_strName);

	packet.wPartySerial					=	_iPartySerial;	//	파티 시리얼
	strcpy(packet.strPartyName,_lpstrPartyName);

	SEND_WOLRD_PACKET(&packet);
}

//
//	월드에 파티이름 변경 메시지를 전송한다.
void
cGAME::sendWPChangePartyName(cParty *_lpParty,char *_lpstrName)
{
	SERVERStoWORLD_RENAME_PARTYNAME	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_RENAME_PARTYNAME);
	packet.wPartySerial	=	_lpParty->m_wSerial;	//	파티 시리얼
//	packet.dwCheckSum	=	_lpParty->getCheckSum();
	strcpy(packet.strPartyName,_lpParty->m_strPartyName);

	strcpy(packet.strName,_lpstrName);		//	파티 이름

	SEND_WOLRD_PACKET(&packet);
}

//
//	OX 퀴즈 마을이다.
void
cGAME::sendWPQuizVillage(int _iOXQuizVillage,int _iWordQuizVillage)
{
	SERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_EVENT_QUIZ_GATE_VILLAGE);

	packet.wOXQuizVillage	=	_iOXQuizVillage;
	packet.wWordQuizVillage	=	_iWordQuizVillage;

	if	(_iOXQuizVillage	==	0xffffffff)
		packet.wOXQuizVillage	=	0xffff;
	if	(_iWordQuizVillage	==	0xffffffff)
		packet.wWordQuizVillage	=	0xffff;

	SEND_WOLRD_PACKET(&packet);
}

//	이사람 커뮤니티 거부 상태다.
void
cGAME::sendWPIsCommunityDenyStatus(char *_lpstrSender,char *_lpstrReceiver)
{
	SERVERStoWORLD_IS_DENY_COMMUNITY_TARGET	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_IS_DENY_COMMUNITY_TARGET);

	strcpy(packet.strSenderName,_lpstrSender);
	strcpy(packet.strReceiverName,_lpstrReceiver);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPCalling(cACTOR *_lpActor,int _iRange,int _iSkill)
{
	SERVERStoWORLD_CALLING	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CALLING);

	packet.wCasterField		=	_lpActor->m_wCurrentField;
	packet.wParty			=	_lpActor->getPartySerial();
	packet.wCaterXPos		=	_lpActor->m_pos.x;
	packet.wCaterYPos		=	_lpActor->m_pos.y;
	packet.wLimitRange		=	_iRange;
	packet.wSkill			=	_iSkill;
	packet.dwNameHashCode	=	_lpActor->m_dwNameHashCode;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPIncreasePackItemCount(int _iPackSerial,int _iItemIndex)
{
	SERVERStoWORLD_INCREASE_PACK_ITEM_COUNT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_INCREASE_PACK_ITEM_COUNT);

	packet.wPackSerial	=	_iPackSerial;
	packet.wItemIndex	=	_iItemIndex;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPAskDuelRecord(cACTOR *_lpActor)
{
	SERVERStoWORLD_ASK_DUEL_RECORD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ASK_DUEL_RECORD);

	strcpy(packet.strId,_lpActor->m_strId);

	SEND_WOLRD_PACKET(&packet);
}


void
cGAME::sendWPChangePartyMemberInfo(cParty *_lpParty,char *_lpstrName,int _iLevel,int _iJob,int _iPSC,int _iGDPC)
{
	SERVERStoWORLD_CHANGE_PARTY_MEMBER_INFO	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CHANGE_PARTY_MEMBER_INFO);

	packet.wPartySerial	=	_lpParty->m_wSerial;
	packet.dwUniqueSerial=	_lpParty->m_dwUniqueSerial;
	strcpy(packet.strName,_lpstrName);
	packet.bf10Level	=	_iLevel;
	packet.bf8Job		=	_iJob;
	packet.bf4PSC		=	_iPSC;
	packet.bf3GDPC		=	_iGDPC;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPRemoveFriendInTargetList(char *_lpstrClient,char *_lpstrTarget)
{
	SERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST);

	strncpy(packet.strClientName,_lpstrClient,dNAME_LENGTH);
	strncpy(packet.strTargetName,_lpstrTarget,dNAME_LENGTH);
	
	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPRemoveFriendInTargetListResult(char *_lpstrClient,char *_lpstrTarget,int _iResult)
{
	SERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST_RESULT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_REMOVE_FRIEND_IN_TARGET_LIST_RESULT);

	strncpy(packet.strClientName,_lpstrClient,dNAME_LENGTH);
	strncpy(packet.strTargetName,_lpstrTarget,dNAME_LENGTH);
	packet.wResult	=	_iResult;
	
	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPPlayerCount(int _iServerIndex,int _iPlayerCount)
{
	SERVERStoWORLD_ACCOUNT_INFO	packet;

	packet.wMaxLevel	=	0;

	memset(packet.awPlayerCountPerLevel,0,sizeof(packet.awPlayerCountPerLevel));

	for (int i=0;i<m_iMaxActorCount;i++)
	{
		if	(m_pActor[i].m_iZoneSerial == 0xffffffff	)
			continue;

		if	(!m_pActor[i].isPlayer()					)
			continue;

		if	(m_pActor[i].isDummy()						)
			continue;

		int	iLevel	=	m_pActor[i].m_iLevel;

		if	(iLevel < 0 || iLevel >= 2000)
			continue;

		packet.wMaxLevel	=	max(packet.wMaxLevel,iLevel);

		packet.awPlayerCountPerLevel[iLevel]++;
	}

	packet.base.set(sizeof(packet),dSERVERStoWORLD_ACCOUNT_INFO);

	packet.iServerIndex	=	_iServerIndex;
	packet.iPlayerCount	=	_iPlayerCount;
	packet.base.wSize	=	sizeof(packet)-sizeof(packet.awPlayerCountPerLevel)+sizeof(WORD)*packet.wMaxLevel;

	SEND_WOLRD_PACKET(&packet);
}

//	단순 작업
void
cGAME::sendWPSimpleWork(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	SERVERStoWORLD_SIMPLE_WORK	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_SIMPLE_WORK);

	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;
	
	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPConfirmEnterField(cACTOR *_lpActor)
{
	SERVERStoWORLD_CONFIRM_ENTER_FIELD	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_CONFIRM_ENTER_FIELD);

	packet.wField	=	_lpActor->m_wCurrentField;
	strcpy(packet.strId,_lpActor->m_strId);
	strcpy(packet.strName,_lpActor->m_strName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPBoostPowerByEvent(int _iWork,char *_lpstrName)
{
	SERVERStoWORLD_BOOST_POWER_BY_EVENT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_BOOST_POWER_BY_EVENT);

	packet.wWork	=	_iWork;
	strcpy(packet.strName,_lpstrName);

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPJoinPartyApplication(cACTOR *_lpAsker,char *_lpstrLeaderName)
{
	SERVERStoWORLD_JOIN_PARTY_APPLICATION	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_JOIN_PARTY_APPLICATION);

	packet.wAskerJob	=	_lpAsker->m_wJob;
	packet.wAskerLevel	=	_lpAsker->m_iLevel;

	strcpy(packet.strAskerName,_lpAsker->m_strName);
	memcpy(packet.strLeaderName,_lpstrLeaderName,dNAME_LENGTH);

	packet.strLeaderName[dNAME_LENGTH-1]	=	NULL;

	SEND_WOLRD_PACKET(&packet);
}

void
cGAME::sendWPJoinPartyApplicationAnswer(char *_lpstrAnswerName,char *_lpstrAskerName,int _iAnswer,cParty *_lpParty)
{
	SERVERStoWORLD_JOIN_PARTY_APPLICATION_ANSWER	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_JOIN_PARTY_APPLICATION_ANSWER);

	packet.wAnswer		=	_iAnswer;

	if	(_lpParty)
	{
		packet.wPartySerial	=	_lpParty->m_wSerial;
		strcpy(packet.strPartyName,_lpParty->m_strPartyName);
	}

	memcpy(packet.strLeaderName,_lpstrAnswerName,dNAME_LENGTH);
	memcpy(packet.strAskerName,_lpstrAskerName,dNAME_LENGTH);

	packet.strLeaderName[dNAME_LENGTH-1]	=	NULL;
	packet.strAskerName[dNAME_LENGTH-1]		=	NULL;

	SEND_WOLRD_PACKET(&packet);
}

//
//월드에 연합 파티 생성을 의뢰 한다.
void
cGAME::SendWPCreateUnionParty(WORD _wAskerPartySerial , WORD _wReplayerPartySerial)
{
	SERVERStoWORLD_CREATE_UNION_PARTY	packet;
	
	packet.base.set(sizeof(packet),dSERVERStoWORLD_CREATE_UNION_PARTY);

	packet.wAskerPartySerial	=	_wAskerPartySerial;
	packet.wReplayerPartySerial	=	_wReplayerPartySerial;
	
	SEND_WOLRD_PACKET(&packet);
}
