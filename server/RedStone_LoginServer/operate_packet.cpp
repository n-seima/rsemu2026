#include	"operate_packet.h"
#include	"life_code.h"
#include	"cSOCKET.h"
#include	"Client.h"
#include	"server.h"

void
SendSimpleWork(cSOCKET *_lpServer,int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)	//	공성전 시작 예고
{
	SERVERStoWORLD_SIMPLE_WORK	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_SIMPLE_WORK);

	packet.wWork	=	_iWork;
	packet.aiValue[0]=	_iValue1;
	packet.aiValue[1]=	_iValue2;
	packet.aiValue[2]=	_iValue3;
	packet.aiValue[3]=	_iValue4;
	packet.aiValue[4]=	_iValue5;
	packet.aiValue[5]=	_iValue6;

	SendToWorld(&packet,_lpServer);
}

void
OperateSimpleWorldWork(cSOCKET *_lpSocket,WORLDtoSERVERS_SIMPLE_WORK *_lpPacket)
{
	switch(_lpPacket->wWork)
	{
		case	eSWWL_UPDATE_CARROT_SHOP_INFO	:
		{
			SendSimpleWork(NULL,eSWLW_UPDATE_CARROT_SHOP_INFO);
			break;
		}
	}
}

void
OperateWorldLifeSignPacket(cSOCKET *_lpSocket,WORLDtoSERVERS_LIFE_SIGN *_lpPacket)
{
	int		iServerIndex	=	_lpPacket->iServerIndex;
	char	*lpstrWorldName	=	NULL;

	cLifeCode_ServerInfo	*lpServer	=	NULL;

	if	(iServerIndex	==	eLC_UserDBCServer)
		lpServer	=	&g_lifeCodeLogServerMgr.m_userDbc;
	else
	{
		cLifeCode_WorldInfo	*lpWorldInfo	=	g_lifeCodeLogServerMgr.getWorld(_lpSocket->name);

		if	(!lpWorldInfo)	//	아직 등록이 안된 월드다
		{
			g_lifeCodeLogServerMgr.registWorld(_lpSocket->name,_lpPacket->iGameServerCount);

			lpWorldInfo	=	g_lifeCodeLogServerMgr.getWorld(_lpSocket->name);
		}

		if	(!lpWorldInfo)
			return;

		lpstrWorldName	=	lpWorldInfo->m_strWorldName;
		lpServer		=	&lpWorldInfo->m_aServers[iServerIndex];
	}

	switch(_lpPacket->wSign)
	{
		case	eBeginLifeSign	:
		case	eUpdateLifeSign	:	//	사인 받았다.
			g_lifeCodeLogServerMgr.lifeSign(eLCS_Life,lpServer,lpstrWorldName,iServerIndex,_lpPacket->strIP);
			break;

		case	eEndLifeSign	:	//	정상적으로 오프됨
		{
			g_lifeCodeLogServerMgr.lifeSign(eLCS_End,lpServer,lpstrWorldName,iServerIndex,_lpPacket->strIP);
			break;
		}

		case	eIncorrectStatusSign	:
		{
			g_lifeCodeLogServerMgr.lifeSign(eLCS_IncorrectStatusSign,lpServer,lpstrWorldName,iServerIndex,_lpPacket->strIP,_lpPacket->strMsg);
			break;
		}
	}
}

void
OperateTransAvatarToGVGServerPacket(cSOCKET *_lpSocket,WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_SERVER *_lpPacket)
{
	cSOCKET	*lpGVGServerSocket	=	GetGVGServerSocket();

	if	(lpGVGServerSocket	==	NULL)
	{
		SERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT	packet;

		packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT);
		
		packet.wResult		=	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_CANNOT_FIND_GVG_SERVER;
		strcpy(packet.strName,_lpPacket->avatarData.m_strName);

		_lpSocket->SendPacket((char *)&packet,packet.base.wSize);

		return;
	}

	SERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_SERVER);

	memcpy(&packet.avatarData,&_lpPacket->avatarData,sizeof(CPlayerSaveDataForServerDefine));
	packet.iWorldIndex	=	_lpPacket->iWorldIndex;

	lpGVGServerSocket->SendPacket((char *)&packet,packet.base.wSize);

// 	printf("try Trans Avatar to G.V.G [%s]\n",packet.avatarData.m_strName);
}

void
OperateTransAvatarToGVGResultPacket(WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT *_lpPacket)	//	아바타 전송 결과 처리
{
//  	printf("receive Trans Avatar to G.V.G Result[World %d]\n",_lpPacket->wWorldServerIndex);
	cSOCKET	*lpWorldServerSocket	=	GetWorldServerSocket(_lpPacket->wWorldServerIndex);

	if	(lpWorldServerSocket	==	NULL)	//	이건뭐.. 어떻게 처리 할수가 없다.. ;;
		return;
//  	printf("receive Trans Avatar to G.V.G Result[%s][%s]\n",_lpPacket->strName,lpWorldServerSocket->name);
	SERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_AVATAR_TO_GVG_RESULT);

	packet.wResult	=	_lpPacket->wResult;

	strcpy(packet.strName,_lpPacket->strName);
	memcpy(packet.strNewName,_lpPacket->strNewName,sizeof(_lpPacket->strNewName));

//	SendToAllWorld(&packet);
	lpWorldServerSocket->SendPacket((char *)&packet,packet.base.wSize);
}

void
OperateTransGuildToGVGServerPacket(cSOCKET *_lpSocket,WORLDtoSERVERS_TRANS_GUILD_TO_GVG_SERVER *_lpPacket)
{
	cSOCKET	*lpGVGServerSocket	=	GetGVGServerSocket();

	if	(lpGVGServerSocket	==	NULL)
	{
		SERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT	packet;

		packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT);
		packet.wResult		=	eTRAN_GUILD_TO_GVG_RESULT_FAILED_BY_CANNOT_FIND_GVG_SERVER;

		memcpy(packet.strMasterName,_lpPacket->strMasterName,sizeof(packet.strMasterName));

		_lpSocket->SendPacket((char *)&packet,packet.base.wSize);
		
		return;
	}

	SERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_GUILD_TO_GVG_SERVER);

	packet.iWorldIndex	=	_lpPacket->iWorldIndex;
	memcpy(&packet.guildData,&_lpPacket->guildData,sizeof(packet.guildData));
	memcpy(packet.strMasterName,_lpPacket->strMasterName,sizeof(packet.strMasterName));
	memcpy(packet.strMasterId,_lpPacket->strMasterId,sizeof(packet.strMasterId));

	packet.wMasterJob	=	_lpPacket->wMasterJob;
	packet.wMasterLevel	=	_lpPacket->wMasterLevel;

	lpGVGServerSocket->SendPacket((char *)&packet,packet.base.wSize);
}

void
OperateTransGuildToGVGResultPacket(WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT *_lpPacket)	//	길드 전송 결과 처리
{
	cSOCKET	*lpWorldServerSocket	=	GetWorldServerSocket(_lpPacket->wWorldServerIndex);

	if	(lpWorldServerSocket	==	NULL)	//	이건뭐.. 어떻게 처리 할수가 없다.. ;;
		return;

	SERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT);

	packet.wResult				=	_lpPacket->wResult;
	memcpy(packet.strMasterName,_lpPacket->strMasterName,sizeof(packet.strMasterName));
	memcpy(packet.strNewMasterName,_lpPacket->strNewMasterName,sizeof(packet.strNewMasterName));
	memcpy(packet.strNewGuildName,_lpPacket->strNewGuildName,sizeof(packet.strNewGuildName));
	memcpy(packet.strGuildName,_lpPacket->strGuildName,sizeof(packet.strGuildName));

//	SendToAllWorld(&packet);
	lpWorldServerSocket->SendPacket((char *)&packet,packet.base.wSize);
}

void
OperateChangeGVGServerGuildMaster(WORLDtoSERVERS_CHANGE_GVG_SERVER_GUILD_MASTER *_lpPacket)
{
	cSOCKET	*lpGVGServerSocket	=	GetGVGServerSocket();

	if	(lpGVGServerSocket	==	NULL)
		return;

	SERVERStoWORLD_CHANGE_GVG_SERVER_GUILD_MASTER	packet;

	packet.base.set(sizeof(packet),dSERVERStoWORLD_TRANS_GUILD_TO_GVG_RESULT);

	strcpy(packet.strGuildName,_lpPacket->strGuildName);
	strcpy(packet.strNewMasterID,_lpPacket->strNewMasterID);

	lpGVGServerSocket->SendPacket((char *)&packet,packet.base.wSize);
}

void
OperateW2L_TOSS_ASK_GVG_AVATAR_ORG_DATA(cSOCKET *_lpGVGWorldSocket,W2L_TOSS_ASK_GVG_AVATAR_ORG_DATA *_lpPacket)
{
//	_log("[GVG] receive W2L_TOSS_ASK_GVG_AVATAR_ORG_DATA\n");

	cSOCKET	*lpWorldServerSocket	=	GetWorldServerSocket(_lpPacket->wOrgServer);

	if	(lpWorldServerSocket	==	NULL)	//	이건뭐.. 어떻게 처리 할수가 없다.. ;;
	{
		L2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	packet;

		packet.base.set(sizeof(packet),dL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT);

		memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
		packet.wResult	=	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_ORG_AVATAR;

		_lpGVGWorldSocket->SendPacket((char *)&packet,packet.base.wSize);

		return;
	}

	L2W_TOSS_ASK_GVG_AVATAR_ORG_DATA	packet;

	packet.base.set(sizeof(packet),dL2W_TOSS_ASK_GVG_AVATAR_ORG_DATA);

	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	memcpy(packet.strId,_lpPacket->strId,dID_LENGTH);
	packet.wOrgServer			=	_lpPacket->wOrgServer;
	packet.wNotUseUnderBarName	=	_lpPacket->wNotUseUnderBarName;

	SendToWorld(&packet,lpWorldServerSocket);
}

void
OperateW2L_ASK_GVG_AVATAR_ORG_DATA_RESULT(W2L_ASK_GVG_AVATAR_ORG_DATA_RESULT *_lpPacket)
{
//	_log("[GVG] receive W2L_ASK_GVG_AVATAR_ORG_DATA_RESULT\n");

	cSOCKET	*lpGVGServerSocket	=	GetGVGServerSocket();

	if	(lpGVGServerSocket	==	NULL)
		return;

	L2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	packet;

	packet.base.set(sizeof(packet),dL2W_ASK_GVG_AVATAR_ORG_DATA_RESULT);
	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	packet.wResult	=	_lpPacket->wResult;

	lpGVGServerSocket->SendPacket((char *)&packet,packet.base.wSize);
}

void
OperateW2L_GVG_AVATAR_ORG_DATA(W2L_GVG_AVATAR_ORG_DATA* _lpPacket)
{
//	_log("[GVG] receive W2L_GVG_AVATAR_ORG_DATA\n");

	cSOCKET	*lpGVGServerSocket	=	GetGVGServerSocket();

	if	(lpGVGServerSocket	==	NULL)
		return;

	L2W_GVG_AVATAR_ORG_DATA	packet;

	packet.base.set(sizeof(packet),dL2W_GVG_AVATAR_ORG_DATA);

	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	memcpy(&packet.avatarData,&_lpPacket->avatarData,sizeof(packet.avatarData));

	CM_COMPRESS		compPacket;
	WORD			wCompSize	=	cSRVUTIL::MakeCompressPacket(&compPacket,&packet,packet.base.wSize);

	if	(wCompSize == 0)
		lpGVGServerSocket->SendPacket((char *)&packet,packet.base.wSize);
	else
		lpGVGServerSocket->SendPacket((char *)&compPacket,compPacket.wSize);
}