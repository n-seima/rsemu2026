#include "stdafx.h"
#include "operate_packet.h"
#include "TemporaryListForEntry.h"
#include "userCustomDataDefine.h"
#include "cSRVUTIL.h"
#include "party.h"
#include "guild.h"
#include "util.h"
#include "field.h"
#include "ChattingDataBase.h"
#include "backup.h"
#include "FriendsListUpdater.h"

inline	BOOL
Is2ByteChar(char _char)
{
#ifdef	_FOR_JAPAN
	if	((BYTE)_char	>=	0xa1 && (BYTE)_char <=0xdf)
		return	FALSE;
#endif

#ifdef	_FOR_THAI
	return	FALSE;
#endif

	if	(_char	<	0)
		return	TRUE;

	return	FALSE;
}

inline	BOOL
Is2ByteChar(char *_lpstrText,int _iPoint)
{
	int	iPoint	=	0;

	while(_lpstrText[iPoint])
	{
		if (Is2ByteChar(_lpstrText[iPoint]))
		{
			if (_iPoint	==	iPoint || _iPoint	==	iPoint+1)
				return	TRUE;

			iPoint++;
		}

		iPoint++;
	}

	return	FALSE;
}

int
FindSpecialCodePoint(char *_lpString,int _iRange)
{
	int		iPoint	=	0;

	while(_lpString[iPoint])
	{
		if	(Is2ByteChar(_lpString[iPoint]))
			iPoint++;
		else
		{
			if	(_lpString[iPoint]	==	0x0d	||	_lpString[iPoint]	==	0x0a)
				return	iPoint;

			if	(_lpString[iPoint]	==	'<')
			{
				if	(_lpString[iPoint+1]	==	's')
				{
					if	(_lpString[iPoint+2]	==	':')
						return	iPoint;
				}

				if (_lpString[iPoint+1]	==	'c')
				{
					if (_lpString[iPoint+2]	==	':')
						return	iPoint;
				}
				else
				if (_lpString[iPoint+1]	==	'l')
				{
					if	(_lpString[iPoint+2]	==	'>')
						return	iPoint;
					if	(_lpString[iPoint+2]	==	':')
						return	iPoint;
				}
				else
				if (_lpString[iPoint+1]	==	'n')
				{
					if (_lpString[iPoint+2]	==	'>')
						return	iPoint;
				}
				else
				if (_lpString[iPoint+1]	==	'r'|| _lpString[iPoint+1]	==	'b'|| _lpString[iPoint+1]	==	's')
				{
					if (_lpString[iPoint+2]	==	'>')
						return	iPoint;
				}
			}
		}

		iPoint++;

		if (iPoint	>=	_iRange)	
			return	-1;
	}

	return	-1;
}


cSOCKET	* g_cpAvatarDBC;

/////////////////////////////////////////////////////////////////////////////
//
void	CheckString(char * _str,int _size, char * _where,int _line)
{
	int i;

	for(i=0; i<_size;i++)
	{
		if(_str[i]==NULL)
			break;
	}

	if	(i==_size)
	{
		char * tmpStr = new char[_size+1];
		memcpy(tmpStr,_str,_size);
		tmpStr[_size] = 0;
		_log("Error String : where[%s,%d] value[%s]",_where,_line,tmpStr);
		delete [] tmpStr;
	}
}
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
void
FillChattingBuffer(SG_CHAT *_lpstrPacket,char *_lpstrName,char *_lpstrMessage)
{
	int	iNameLength		=	min(strlen(_lpstrName)+1,dNAME_LENGTH);
	int	iMessageLength	=	min(strlen(_lpstrMessage)+1,dCHAT_LENGTH-1);

	mRCI();
	strcpy(_lpstrPacket->strMessage,_lpstrName);
	mRCI();
	strncpy(_lpstrPacket->strMessage+iNameLength,_lpstrMessage,dCHAT_LENGTH-1);
	mCRCI();

	_lpstrPacket->base.wSize	=	sizeof(SG_CHAT)-sizeof(_lpstrPacket->strMessage)+iNameLength+iMessageLength;
	_lpstrPacket->base.wType	=	dSG_CHAT;
}
//
/////////////////////////////////////////////////////////////////////////////

void
nsReceiveFromClient::LogInClient(CClient *_lpClient,CB_LOGIN *_lpPacket)
{
	stCLIENTDATA * _stData = cTempList.GetDataPnt(_lpPacket->wIndex);
	
//	_log("LogInClient : idx [%d], [%s],[%s],[%d]",_lpPacket->wIndex,_lpPacket->strName,_stData->strName,_stData->iDBIndex);
	DWORD testDBIndex = 0xffffffff;
	BC_LOGIN_RESULT	packet;
	packet.base.set(sizeof(BC_LOGIN_RESULT),dBC_LOGIN_RESULT);

	if	(!_stData || STRICMP(_lpPacket->strName,_stData->strName)!=0)	//	 fail
	{
		packet.wResult = 0;
		packet.base.wSize -= sizeof(cUserCustomDataDefine);
		if(!_stData)
			extWriteConnectionInfo(_lpPacket->strId,"(LogInClient) !_stData\n");
		else
			extWriteConnectionInfo(_lpPacket->strId,"(LogInClient) wIndex %d, [%s]!=_stData->strName\n",_lpPacket->wIndex,_lpPacket->strName);
	}
	else
	{	
		testDBIndex = _stData->iDBIndex;
#ifdef _FOR_KOREA
		CheckString(_stData->strId,sizeof(_stData->strId),"LogInClient",__LINE__);
		CheckString(_stData->strName,sizeof(_stData->strName),"LogInClient",__LINE__);
#endif
		mRCI();
		strcpy(_lpClient->m_strId,_stData->strId);
		mRCI();
		strcpy(_lpClient->m_strName,_stData->strName);
		mCRCI();

		memcpy(&_lpClient->m_clPersonalData,&_stData->cData,sizeof(cUserCustomDataDefine));
		_lpClient->m_clPersonalData.resetCurrentFriendListOnlineStatus();

		_lpClient->m_dwHashIdCode	=	cSRVUTIL::GetHashCode((BYTE *)_lpClient->m_strId);
		_lpClient->m_dwHashNameCode =	cSRVUTIL::GetHashCode((BYTE *)_lpClient->m_strName);
		_lpClient->m_wLevel			=	_stData->wCurrentLevel;
		_lpClient->setPartySerial(_stData->wPartySerial);
		_lpClient->setFieldSerial(_stData->wCurrentField);
		_lpClient->setGuildSerial(_stData->wGuildSerial);// , _lpPacket->wGuildDisplayIndex);
		_lpClient->setAvatarIndex(_stData->wAvatarIndex);
		_lpClient->m_wMyGSSerial	=	_stData->wMyGSSerial;
		_lpClient->m_wGuildRank		=	_stData->wGuildRank;
		_lpClient->m_wOper			=	_stData->wOper;
		_lpClient->m_wGSSerial		=	_stData->wGSSerial;
		_lpClient->m_iDBIndex		=	_stData->iDBIndex;

		if(testDBIndex != _lpClient->m_iDBIndex)
		{
#ifdef _FOR_KOREA
			CheckString(_lpClient->m_strId,sizeof(_lpClient->m_strId),"LogInClient",__LINE__);
			CheckString(_lpClient->m_strName,sizeof(_lpClient->m_strName),"LogInClient",__LINE__);
#endif
			_log("========================\nREGIST Trouble : [%s,%s , %d->%d]\n========================\n",_lpClient->m_strId,_lpClient->m_strName,testDBIndex,_lpClient->m_iDBIndex);
		}

		packet.wResult				=	BC_LOGIN_RESULT::eREQUIRE_UPDATE_DATA;
		packet.wBCSerial			=	_lpClient->m_wSerial;

		for(int i=0;i<dMAX_FRIEND_COUNT;i++)
		{
			if	(!_lpClient->m_clPersonalData.m_aFriendList[i].m_strName[0])
				break;
			CClient * pFriend = UserManager.GetClientPnt(_lpClient->m_clPersonalData.m_aFriendList[i].m_strName,1);
			if	(!pFriend)
				_lpClient->m_clPersonalData.updateFriendOnlineStatus(i,false);
			else
				_lpClient->m_clPersonalData.updateFriendOnlineStatus(i,true);			
		}
		//	클라이언트에 저장된 데이터의 CRC32와 현재CRC32와 비교해 같으면 데이터 전송을 안한다.
		{
			DWORD	dwCRC32;

			if	(_lpClient->m_clPersonalData.getUserCustomDataCRC32(dwCRC32))
			{
				if	(dwCRC32	==	_lpPacket->dwCRC32)
				{
					packet.wResult	=	BC_LOGIN_RESULT::eDO_NOT_REQUIRE_UPDATE_DATA;
					packet.base.wSize=	sizeof(packet)-sizeof(packet.data);
				}
			}
		}

		if	(packet.wResult			==	BC_LOGIN_RESULT::eREQUIRE_UPDATE_DATA)
		{
			packet.wFriendCount		=	_lpClient->m_clPersonalData.getFriendCount();
			memcpy(&packet.data,&_lpClient->m_clPersonalData,sizeof(cUserCustomDataDefine));
		}
	}

	_lpClient->AddSendPacket((char *)&packet,packet.base.wSize);
	cTempList.Del(_lpPacket->wIndex);

	nsSendToClient::SendConnectionInfoToMyFriends(_lpClient, TRUE);	// 접속을 했다고 친구들에게 보낸다.
	//	정상적으로 등록되었으며, 게임서버에 새롭게 갱신된 인덱스 번호를 저장해주자.
	if	(packet.wResult			!=	BC_LOGIN_RESULT::eFAILED)
	{
		BG_REG_CLIENT_SERIAL	spacket;
		spacket.base.set(sizeof(BG_REG_CLIENT_SERIAL),dBG_REG_CLIENT_SERIAL);

		CClient					*clientGS	= mCM.GetClientPnt(_lpClient->m_wMyGSSerial);

		if	(!clientGS)//BC서버를 재시작할 경우 문제가 발생할 수 있다.
		{
			_log("nsReceiveFromClient::LogInClient - mCM.GetClientPnt  %d",_lpClient->m_wMyGSSerial);
			_log("Game Server Info\n==================================\n");

			for	(int i=dBC_MAX_USER_COUNT;i;)
			{
				--i;

				CClient * pClient = UserManager.GetClientPnt(i);

				if	(!pClient || !pClient->m_isGameServer)
					continue;
#ifdef _FOR_KOREA
				CheckString(pClient->m_strIp,sizeof(pClient->m_strIp),"LogInClient",__LINE__);
#endif
				_log("[%d] [%s] ",i,pClient->m_strIp);
			}
			_log("End\n===============================================\n");
			spacket.wRealIndex = 0xffff;
			spacket.wSerial = 0xffff;
		}
		else
		{
			spacket.wRealIndex	=	_lpClient->m_wSerial;
			spacket.wSerial		=	_lpClient->m_wGSSerial;
			clientGS->AddSendPacket((char *)&spacket,spacket.base.wSize);

			cGuildInfo* lpGuild = g_guildManager.get(_lpClient->m_wGuildSerial);
			if(lpGuild)
			{	
				lpGuild->m_bIsChangeConnetState = TRUE;
				g_cGuildMemberListUpdater.SetLoginUserIndex(_lpClient->m_wGuildSerial);			// 여기선 유저 시리얼이 아닌..길드 시리얼 넣자~!
			}
			g_cFriendsListUpdater.SetLoginUserIndex(_lpClient->m_wSerial);

		}
		extWriteConnectionInfo(_lpPacket->strId,"(LogInClient) wRealIndex %d, wSerial %d\n",spacket.wRealIndex,spacket.wSerial);
	}else
	{
		extWriteConnectionInfo(_lpPacket->strId,"(LogInClient) FAIL wResult %d\n",packet.wResult);
	}
}

void
nsReceiveFromClient::UpdateUserAllInfoPacket(CClient *_lpClient,CB_UPDATE_USER_ALLINFO *_lpPacket)
{
	cUserCustomDataDefine	*lpInfo	=	&_lpClient->m_clPersonalData;

	if	(lpInfo->m_bf1IsSaveAllDataByClient)
		return;

	memcpy(lpInfo,&_lpPacket->data,sizeof(cUserCustomDataDefine));
	
	for	(int i=_lpPacket->wFriendCount;i<dMAX_FRIEND_COUNT;i++)
		lpInfo->m_aFriendList[i].reset();

	nsSendToDB::SavePersonalData(_lpClient , 1);
}

void
nsReceiveFromClient::ChangeUserSkillSetting(CClient *_lpClient,CB_CHANGE_USER_SKILL_SETTING *_lpPacket)
{
	if	(!_lpClient)
		return;

	cUserCustomDataDefine	*lpInfo	=	&_lpClient->m_clPersonalData;

	lpInfo->changeSkillInfo((cUserCustomDataDefine::eUSER_SKILL_SET)_lpPacket->wWhat,_lpPacket->wValue1,_lpPacket->wValue2,_lpPacket->wValue3,_lpPacket->wValue4);

	nsSendToDB::SavePersonalData(_lpClient);
}

void
nsReceiveFromClient::ChangeFriendGroupName(CClient *_lpClient,CB_CHANGE_FRIEND_GROUP_NAME *_lpPacket)
{
	if	(!_lpClient)
		return;

	cUserCustomDataDefine	*lpInfo	=	&_lpClient->m_clPersonalData;

	lpInfo->changeFriendGroupName(_lpPacket->wGroup,_lpPacket->strName);

	nsSendToDB::SavePersonalData(_lpClient);
}

void
nsReceiveFromClient::ChangeFriendGroup(CClient *_lpClient,CB_CHANGE_FRIEND_GROUP *_lpPacket)
{
	if	(!_lpClient)
		return;

	cUserCustomDataDefine	*lpInfo	=	&_lpClient->m_clPersonalData;

	lpInfo->changeFriendGroup(_lpPacket->wChangeGroup,_lpPacket->strName);

	nsSendToDB::SavePersonalData(_lpClient);
}

void
nsReceiveFromClient::AddFriend(CClient *_lpClient,GB_ADD_FRIEND *_lpPacket)
{
	if	(!_lpClient || !_lpClient->m_isGameServer)
		return;

	CClient	*lpUser1=NULL,*lpUser2=NULL;

	lpUser1	=	mCM.GetClientPnt(_lpPacket->aInfo[0].m_strName,1);

	if	(_lpPacket->wIsRecoverList	==	FALSE)
	{
		lpUser2	=	mCM.GetClientPnt(_lpPacket->aInfo[1].m_strName,1);

		if	(!lpUser1	||	!lpUser2)
			return;
	}
		
	if	(lpUser1)
	{
		lpUser1->m_clPersonalData.addFriend(&_lpPacket->aInfo[1]);
		nsSendToDB::SavePersonalData(lpUser1);
	}

	if	(lpUser2)
	{
		lpUser2->m_clPersonalData.addFriend(&_lpPacket->aInfo[0]);
		nsSendToDB::SavePersonalData(lpUser2);
	}
}

void
nsReceiveFromClient::ForcedRemoveFriend(CClient *_lpClient,GB_FORCED_REMOVE_FRIEND *_lpPacket)
{
	if	(!_lpClient || !_lpClient->m_isGameServer)
		return;

	CClient	*lpOwner	=	mCM.GetClientPnt(_lpPacket->strOwnerName,1);

	if	(!lpOwner)
		return;

	if	(lpOwner->m_clPersonalData.removeFriend(_lpPacket->strFriendName))
		nsSendToDB::SavePersonalData(lpOwner);
}

void
nsReceiveFromClient::AskItemInfo(CClient *_lpClient,GB_ASK_ITEM_INFO *_lpPacket)
{
	if	(!_lpClient || !_lpClient->m_isGameServer)
		return;

	CClient	*lpTarget		=	mCM.GetClientPnt(_lpPacket->strTargetName,1);
	CClient	*lpAsker		=	mCM.GetClientPnt(_lpPacket->strAskerName,1);

	if	(!lpTarget	||	!lpAsker)
		return;

	CClient	*lpServer		=	mCM.GetClientPnt(lpTarget->m_wMyGSSerial);

	if	(!lpServer)
		return;

	BG_ASK_ITEM_INFO	packet;

	packet.base.set(sizeof(packet),dBG_ASK_ITEM_INFO);
#ifdef _FOR_KOREA
	CheckString(_lpPacket->strTargetName,sizeof(_lpPacket->strTargetName),__FILE__,__LINE__);
#endif
	mRCI();
	strcpy(packet.strTargetName,_lpPacket->strTargetName);
	mCRCI();
	packet.wAskerBCSerial	=	lpAsker->m_wSerial;

	lpServer->AddSendPacket((char *)&packet,packet.base.wSize);
}

//	정보 업데이트
void
nsReceiveFromClient::UpdateInfo(CClient *_lpClient,GB_UPDATE_INFO *_lpPacket)
{
	if	(!_lpClient || !_lpClient->m_isGameServer)
		return;

	CClient	*lpOwner	=	mCM.GetClientPnt(_lpPacket->wBCSerial);

	if	(!lpOwner	||	STRICMP(lpOwner->m_strName,_lpPacket->strName)	!=	0)
	{
		lpOwner	=	mCM.GetClientPnt(_lpPacket->strName,1);

		if	(!lpOwner	||	STRICMP(lpOwner->m_strName,_lpPacket->strName)	!=	0)
			return;
	}

	lpOwner->m_wMyGSSerial	=	_lpClient->m_wSerial;
	lpOwner->m_wLevel		=	_lpPacket->wLevel;
	lpOwner->m_wGuildRank	=	_lpPacket->wGuildRank;
	lpOwner->m_wOper		=	_lpPacket->wOper;

	lpOwner->setFieldSerial(_lpPacket->wCurrentField);
	lpOwner->setGuildSerial(_lpPacket->wGuildSerial);
	lpOwner->setPartySerial(_lpPacket->wPartySerial);
	lpOwner->setAvatarIndex(_lpPacket->wAvatarIndex);
}

void
nsReceiveFromClient::RemoveFriend(CClient *_lpClient,CB_REMOVE_FRIEND *_lpPacket)
{
	if	(!_lpClient)
		return;

	cUserCustomDataDefine	*lpInfo	=	&_lpClient->m_clPersonalData;

	if	(lpInfo->removeFriend(_lpPacket->strName))
		nsSendToDB::SavePersonalData(_lpClient);
}

//	길드 채팅
void
nsReceiveFromClient::GuildChat(CClient *_lpClient,GB_GUILD_CHAT *_lpPacket)
{
	if	(!_lpClient)
		return;

	CClient	*lpChatter	=	mCM.GetClientPnt(_lpPacket->wBCSerial);

	if	(!lpChatter)
		return;

	nsSendToClient::SendGuildChat(lpChatter,_lpPacket->strMessage);
}

//	파티 채팅
void
nsReceiveFromClient::PartyChat(CClient *_lpClient,GB_PARTY_CHAT *_lpPacket)
{
	if	(!_lpClient)
		return;

	CClient	*lpChatter	=	mCM.GetClientPnt(_lpPacket->wBCSerial);

	if	(!lpChatter)
		return;

	nsSendToClient::SendPartyChat(lpChatter,_lpPacket->strMessage);
}

void
nsReceiveFromClient::PartyPacketDeliver(CClient *_lpClient,GB_PARTY_PACKET_DELIVER *_lpPacket)
{
	if	(!_lpClient)
		return;

	cPartyInfo	*lpParty	=	g_partyManager.get(_lpPacket->wPartySerial);

	if	(!lpParty)
		return;
	
	lpParty->sendPacketToClient(_lpPacket->aBuffer,_lpPacket->wFieldSerial);
}

void
nsReceiveFromClient::PacketDeliver(CClient *_lpServer,GB_PACKET_DELIVER *_lpPacket)
{
	void	*lpPacket			=	_lpPacket->aBuffer;
	int		iPacketSize			=	((cMSG_BASE_TYPE *)lpPacket)->wSize;
	int		iPacketType			=	((cMSG_BASE_TYPE *)lpPacket)->wType;
	int		iDeliverTargetCount	=	_lpPacket->wDeliverTargetCount;
	WORD	*lpClientList		=	(WORD *)(_lpPacket->aBuffer+iPacketSize);
	CClient	*lpPacketOwner		=	mCM.GetClientPnt(_lpPacket->wPacketOwner);

	if	(_lpPacket->wTargetField!=	0xffff)
	{
		cFieldInfo	*lpField	=	g_fieldManager.get(_lpPacket->wTargetField);

		if	(!lpField)
			return;

		lpField->sendPacketToClient(lpPacket);

		return;
	}

	for	(;iDeliverTargetCount;)
	{
		--iDeliverTargetCount;

		CClient	*lpClient	=	mCM.GetClientPnt(lpClientList[iDeliverTargetCount]);

		if	(!lpClient)
		{
			nsSendToGS::AskClientInfo(_lpServer,lpClientList[iDeliverTargetCount]);
			continue;
		}

		lpClient->AddSendPacket((char *)lpPacket,iPacketSize);
	}
}
//	내 친구들에게 나의 접속 정보를 보낸다.
void
nsSendToClient::SendConnectionInfoToMyFriends(CClient* _lpClient, BOOL _bConnection)
{
	for(int i=0;i<dMAX_FRIEND_COUNT;i++)
	{
		if	(!_lpClient->m_clPersonalData.m_aFriendList[i].m_strName[0])
			break;
		
		CClient * pFriend = UserManager.GetClientPnt(_lpClient->m_clPersonalData.m_aFriendList[i].m_strName,1);
		
		if(pFriend)
		{
			for(int j=0;j<dMAX_FRIEND_COUNT;++j)
			{
				if	(!pFriend->m_clPersonalData.m_aFriendList[j].m_strName[0])
					break;
				DWORD	dwFriendHashCode = cSRVUTIL::GetHashCode((BYTE *)pFriend->m_clPersonalData.m_aFriendList[j].m_strName);
				
				if(_lpClient->m_dwHashNameCode == dwFriendHashCode)
				{
					pFriend->m_clPersonalData.updateFriendOnlineStatus(j,_bConnection);
					if(pFriend->m_clPersonalData.m_bf1IsChangeFriendListOnlineStatus)
						nsSendToClient::ChangeFriendOnlineStatus(pFriend);
					break;
				}	
			}
		}	
	}
}
void
nsReceiveFromClient::AskOpenPartyList(CClient *_lpClient,CB_ASK_OPEN_PARTY_LIST *_lpPacket)
{
	BC_OPEN_PARTY_LIST	packet;

	packet.wCount	=	0;

	int	iPartyIndex	=	_lpPacket->bf10LastReceiveParty;

	if	(iPartyIndex>=	dPARTY_COUNT)
		iPartyIndex	=	0;

	for	(int i=0;i<dPARTY_COUNT;i++)
	{
		cPartyInfo	*lpInfo	=	g_partyManager.get(iPartyIndex);

		if	(_lpPacket->bf1IsWantNext)
		{
			iPartyIndex++;

			if	(iPartyIndex>=	dPARTY_COUNT)
				iPartyIndex	=	0;
		}
		else
		{
			--iPartyIndex;

			if	(iPartyIndex	<	0)
				iPartyIndex	=	dPARTY_COUNT-1;
		}

		if	(!lpInfo	||	lpInfo->m_bf1IsOpenParty	==	FALSE)
			continue;

		if	(!lpInfo->isJoinAbleLevelParty(_lpClient->m_wLevel))
			continue;
		
		if	(lpInfo->m_bf3MemberCount	==	0)
			continue;

		lpInfo->getInfoForPL(&packet.aList[packet.wCount++]);

		if	(packet.wCount	>=	c_iPartyBufferSizeForPartyList)
			break;
	}

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.aList)+sizeof(cPartyInfoForPL)*packet.wCount;
	packet.base.set(sizeof(packet),dBC_OPEN_PARTY_LIST);

	_lpClient->AddSendPacket((char *)&packet,packet.base.wSize);
}

void
nsReceiveFromClient::AskPartyProstpectus(CClient *_lpClient,CB_ASK_PARTY_PROSPECTUS *_lpPacket)
{
	if	(_lpPacket->wPartySerial	>=	dPARTY_COUNT)
		return;

	cPartyInfo			*lpParty	=		g_partyManager.get(_lpPacket->wPartySerial);

	if	(!lpParty)
	{
		nsSendToWorld::AskPartyInfo(_lpPacket->wPartySerial);
		nsSendToClient::SendMessage(_lpClient,BC_SERVER_MESSAGE::eINCORRECT_PARTY_INFO_PLEASE_RETRY_WORK);

		return;
	}

	BC_UPDATE_PARTY_PROSPECTUS	packet;

	packet.base.set(sizeof(packet),dBC_UPDATE_PARTY_PROSPECTUS);

	packet.bf1IsIncludeText				=	TRUE;
	packet.wPartySerial					=	_lpPacket->wPartySerial;
	packet.dwPreferenceJobMask			=	lpParty->m_dwPreferenceJobMask;			// 수정
	packet.bf10RecruitMemberLevelRange	=	lpParty->m_wRecruitMemberLevelRange;
#ifdef _FOR_KOREA
	CheckString(lpParty->m_strPartyProspectus,sizeof(lpParty->m_strPartyProspectus),__FILE__,__LINE__);
#endif
	mRCI();
	memset(packet.strPartyProspectus+c_iPartyProspectusLength-2,0,2);
	memcpy(packet.strPartyProspectus,lpParty->m_strPartyProspectus,c_iPartyProspectusLength);
	mCRCI();

	packet.base.wSize					=	sizeof(packet)-c_iPartyProspectusLength + strlen(packet.strPartyProspectus)+1;

	_lpClient->AddSendPacket((char *)&packet,packet.base.wSize);
}


void
nsReceiveFromClient::ChangePartyProstpectus(CClient *_lpClient,CB_CHANGE_PARTY_PROSPECTUS *_lpPacket)
{
	if	(_lpPacket->wPartySerial	>=	dPARTY_COUNT)
		return;

	cPartyInfo			*lpParty	=		g_partyManager.get(_lpPacket->wPartySerial);

	if	(!lpParty	||	lpParty->isPartyLeader(_lpClient->m_strName) == FALSE)
	{
		nsSendToWorld::AskPartyInfo(_lpPacket->wPartySerial);
		nsSendToClient::SendMessage(_lpClient,BC_SERVER_MESSAGE::eINCORRECT_PARTY_INFO_PLEASE_RETRY_WORK);

		return;
	}

	if	(FindSpecialCodePoint(_lpPacket->strPartyProspectus,strlen(_lpPacket->strPartyProspectus))	!=	-1)
		return;

	lpParty->m_dwPreferenceJobMask		=	_lpPacket->dwPreferenceJobMask;		// 수정
	lpParty->m_wRecruitMemberLevelRange	=	_lpPacket->bf10RecruitMemberLevelRange;

	if	(_lpPacket->bf1IsChangeText)
		lpParty->changePartyProspectusText(_lpPacket->strPartyProspectus);

	{
		BC_UPDATE_PARTY_PROSPECTUS	packet;

		packet.base.set(sizeof(packet),dBC_UPDATE_PARTY_PROSPECTUS);

		if	(_lpPacket->bf1IsChangeText	==	FALSE)
			packet.base.wSize	=	sizeof(packet)-sizeof(packet.strPartyProspectus);
		else
		{
			memcpy(packet.strPartyProspectus,lpParty->m_strPartyProspectus,c_iPartyProspectusLength);
			packet.strPartyProspectus[c_iPartyProspectusLength-2]	=	NULL;
			packet.base.wSize	=	sizeof(packet)-c_iPartyProspectusLength + strlen(packet.strPartyProspectus)+1;
		}

		packet.wPartySerial					=	_lpPacket->wPartySerial;
		packet.bf1IsIncludeText				=	_lpPacket->bf1IsChangeText;
		packet.dwPreferenceJobMask			=	_lpPacket->dwPreferenceJobMask;		//	수정
		packet.bf10RecruitMemberLevelRange	=	_lpPacket->bf10RecruitMemberLevelRange;

		lpParty->sendPacketToClient(&packet);
	}
}

void
nsReceiveFromClient::OperateGuildWorkBus(CClient *_lpClient,GB_GUILD_WORK_BUS *_lpPacket)
{
	g_guildManager.sendPacketToClient(_lpPacket->wGuildSerial,_lpPacket->aBuffer);
}

void
nsSendToDB::SavePersonalData(CClient *_lpClient, BOOL _isSave)
{
	BOOL	bIsSaved	=	FALSE;
	int iTime = timeGetTime() - _lpClient->m_dwSaveGap;
	if	(_isSave || (abs(iTime) > 6000) )
	{
		cUserCustomDataDefine	*lpInfo	=	&_lpClient->m_clPersonalData;

		_lpClient->m_dwSaveGap = timeGetTime();

		GDMSG_SAVE_PERSONALDATA sPacket;
		sPacket.base.set(sizeof(GDMSG_SAVE_PERSONALDATA),dGDMSG_SAVE_PERSONALDATA);
		sPacket.wIndex = _lpClient->m_wSerial;
		sPacket.iDBIndex = _lpClient->m_iDBIndex;

		if	(strcmp(_lpClient->m_strId,"")==0)
		{
#ifdef _FOR_KOREA
			CheckString(_lpClient->m_strId,sizeof(_lpClient->m_strId),__FILE__,__LINE__);
			CheckString(_lpClient->m_strName,sizeof(_lpClient->m_strName),__FILE__,__LINE__);
#endif
			_log(" ERROR SavePersonalData : id [%s] , name [%s] ",_lpClient->m_strId,_lpClient->m_strName);
			return ;
		}
		
		bIsSaved	=	TRUE;
#ifdef _FOR_KOREA
		CheckString(_lpClient->m_strId,sizeof(_lpClient->m_strId),__FILE__,__LINE__);
		CheckString(_lpClient->m_strName,sizeof(_lpClient->m_strName),__FILE__,__LINE__);
#endif
		mRCI();
		strcpy(sPacket.strId,_lpClient->m_strId);
		mRCI();
		strcpy(sPacket.strName,_lpClient->m_strName);
		mCRCI();
		memcpy(&sPacket.data,lpInfo,sizeof(cUserCustomDataDefine));
		g_cpAvatarDBC->SendPacket((char *)&sPacket,sPacket.base.wSize);
	}
	
	g_bud.save(_lpClient,bIsSaved);
}

void
nsSendToClient::SendMessage(CClient *_lpClient,int _iMessage)
{
	BC_SERVER_MESSAGE	packet;

	packet.base.set(sizeof(packet),dBC_SERVER_MESSAGE);
	packet.wMessage	=	_iMessage;

	_lpClient->AddSendPacket((char *)&packet,packet.base.wSize);
}

//	친구 목록의 온라인 상태 갱신
void
nsSendToClient::ChangeFriendOnlineStatus(CClient *_lpClient)
{
	BC_UPDATE_FRIEND_ONLINE_STATUS		packet;
	
	packet.base.set(sizeof(packet),dBC_UPDATE_FRIEND_ONLINE_STATUS);

	_lpClient->m_clPersonalData.getFriendListOnlineStatus(&packet.onlineStatus);

	_lpClient->AddSendPacket((char *)&packet,packet.base.wSize);
}

//	길드 채팅 전송
void
nsSendToClient::SendGuildChat(CClient *_lpClient,char *_lpstrMessage)
{
	if	(_lpClient->m_wGuildSerial	==	0xffff)
		return;

	SG_CHAT		sPacket;

	sPacket.bf11PGSerial	=	_lpClient->m_wGuildSerial;
	sPacket.bf6ChatType		=	eCT_GUILD;
	FillChattingBuffer(&sPacket,_lpClient->m_strName,_lpstrMessage);

	cGuildInfo	*lpGuild	=	g_guildManager.get(_lpClient->m_wGuildSerial);

	if	(lpGuild)
	{
		lpGuild->sendPacketToClient(&sPacket);

		return;
	}

	for(int i=dBC_MAX_USER_COUNT;i;)
	{
		--i;

		CClient	*lpUser	= mCM.GetClientPnt(i);

		if	(!lpUser	||	lpUser->m_wSerial == 0xffff || lpUser->m_isGameServer)
			continue;
		if	(lpUser->m_wGuildSerial	!=	_lpClient->m_wGuildSerial)
			continue;

		lpUser->AddSendPacket((char *)&sPacket,sPacket.base.wSize);
	}
}

//	파티 채팅 전송
void
nsSendToClient::SendPartyChat(CClient *_lpClient,char *_lpstrMessage)
{
	if	(_lpClient->m_wPartySerial	==	0xffff)
		return;

	cPartyInfo			*lpParty	=		g_partyManager.get(_lpClient->m_wPartySerial);

	if	(!lpParty)
	{
		nsSendToWorld::AskPartyInfo(_lpClient->m_wPartySerial);

		return;
	}

	SG_CHAT		sPacket;

	sPacket.bf11PGSerial	=	_lpClient->m_wPartySerial;
	sPacket.bf6ChatType		=	eCT_PARTY;
	FillChattingBuffer(&sPacket,_lpClient->m_strName,_lpstrMessage);

	lpParty->sendPacketToClient(&sPacket);
}

//
//	월드에서 파티 리스트를 받는다.
void
nsReceiveFromWorld::UpdatePartyList(WB_PARTY_LIST *_lpPacket)
{
	if	(_lpPacket->bf1IsFirst)
		g_partyManager.beginReceivePartyList();
	if	(_lpPacket->bf1IsLast)
		g_partyManager.endReceivePartyList();

	for	(int i=0;i<(int)_lpPacket->bf8CurrentCount;i++)
	{
		cPartyInfo	*lpInfo	=	(cPartyInfo	*)&_lpPacket->aList[i];

		g_partyManager.updatePartyInfo(lpInfo,TRUE);
	}
}

//
//	월드에서 특정 파치 정보를 업데이트 했다. 추가/삭제/업데이트
void
nsReceiveFromWorld::UpdatePartyInfo(WB_UPDATE_PARTY_INFO *_lpPacket)
{
	g_partyManager.updatePartyInfo((cPartyInfo *)&_lpPacket->partyInfo);
}

void
nsSendToWorld::AskPartyList()
{
	BW_ASK_PARTY_LIST	packet;
	
	packet.base.set(sizeof(packet),dBW_ASK_PARTY_LIST);

	SendPacketToWS(&packet);
}

void
nsSendToWorld::AskPartyInfo(int _iPartySerial)
{
	BW_ASK_PARTY_INFO	packet;

	packet.base.set(sizeof(packet),dBW_ASK_PARTY_INFO);

	packet.wPartySerial	=	_iPartySerial;

	SendPacketToWS(&packet);
}

void
nsSendToGS::AskClientInfo(CClient *_lpServer,int _iSerial)
{
	if	(_iSerial	>=	dBC_MAX_USER_COUNT)
		return;

	BG_ASK_CLIENT_INFO	packet;

	packet.base.set(sizeof(BG_REG_CLIENT_RESULT),dBG_ASK_CLIENT_INFO);
	packet.wSerial	=	_iSerial;

	_lpServer->AddSendPacket((char *)&packet,packet.base.wSize);
}

cBinaryLog	g_binaryLog;

cBinaryLog::cBinaryLog()
{
	InitializeCriticalSection(&m_cs);

	m_iMaxLogFileSize	=	0;
}

cBinaryLog::~cBinaryLog()
{
	DeleteCriticalSection(&m_cs);
}

BOOL
cBinaryLog::init(char *_lpstrFileName,int _iMaxSize)
{
#ifndef	_FOR_KOREA
	return	TRUE;
#endif

	m_iLogFileCounter	=	0;
	m_uiCurrentFilePoint=	0;
	m_iMaxLogFileSize	=	_iMaxSize;

	strcpy(m_strLogName,_lpstrFileName);
	sprintf(m_strLogFileName,"%s_%.4d.sonaki_bld",_lpstrFileName,m_iLogFileCounter);

	if	(!m_file.Open(m_strLogFileName,"wb",TRUE))
		return	FALSE;

	m_iLogFileCounter++;

	return	TRUE;
}

BOOL
cBinaryLog::add(void *_lpData,int _iSize)
{
#ifndef	_FOR_KOREA
	return	TRUE;
#endif
	mCS(m_cs);

	if	(m_iMaxLogFileSize	<=	0)
		return	FALSE;

	if	(_iSize	>	m_iMaxLogFileSize)
	{
		cMSG::Put("error!!","cBinaryLog::add to big data");

		return	FALSE;
	}

	if	(_iSize	<=	0)
	{
		cMSG::Put("error!!","cBinaryLog::add to small data");

		return	FALSE;
	}

	m_uiCurrentFilePoint	+=	_iSize;

	if	(m_uiCurrentFilePoint	>=	m_iMaxLogFileSize)
	{
		m_file.Close();

		if	(m_iLogFileCounter	>=	3)
		{
			char	strDeleteFile[512];
			sprintf(strDeleteFile,"%s_%.4d.sonaki_bld",m_strLogName,m_iLogFileCounter-3);
			DeleteFile(strDeleteFile);
		}

		sprintf(m_strLogFileName,"%s_%.4d.sonaki_bld",m_strLogName,m_iLogFileCounter);

		if	(!m_file.Open(m_strLogFileName,"wb",TRUE))
		{
			cMSG::Put("error!!","cBinaryLog::add new file open error!");

			return	FALSE;
		}

		m_iLogFileCounter++;
		m_uiCurrentFilePoint	=	0;
	}

	m_file.Write(_lpData,_iSize);

	return	TRUE;
}