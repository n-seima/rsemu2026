#include "operatePacket.h"
#include "cPLAYER_DATA.h"
#include "Client.h"
#include "CSaveData.h"
#include "cSRVUTIL.h"
#include "cPACKET_BASE.h"

void
SendSimpleWork(CClient *_lpServer,int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)	//	공성전 시작 예고
{
	D2W_SIMPLE_WORK	packet;

	packet.base.set(sizeof(packet),dD2W_SIMPLE_WORK);

	packet.wWork	=	_iWork;
	packet.aiValue[0]=	_iValue1;
	packet.aiValue[1]=	_iValue2;
	packet.aiValue[2]=	_iValue3;
	packet.aiValue[3]=	_iValue4;
	packet.aiValue[4]=	_iValue5;
	packet.aiValue[5]=	_iValue6;

	_lpServer->AddSendPacket((char*)&packet,packet.base.wSize);
}

void	
OperateTransAvatarToGVGPacket(CClient *_lpWorldServer,WDMSG_TRANS_AVATAR_TO_GVG *_lpPacket)
{
	cPLAYER_DATA	player;

	memcpy(&player,&_lpPacket->avatarData,sizeof(CPlayerSaveDataForServerDefine));

// 	if	(player.m_bf4RebirthCount)
// 		player.m_iLevel	=	600+(player.m_bf4RebirthCount-1)*100+player.m_iLevel*100/600;

	int	iNameLength	=	strlen(player.m_strName);

	if	(iNameLength >= dNAME_LENGTH-2)
	{
		player.m_wProcessChapterIndex	=	1;
		player.m_strName[iNameLength]	=	'A'+_lpPacket->iWorldIndex;
		player.m_strName[iNameLength+1]	=	NULL;
	}
	else
	{
		player.m_wProcessChapterIndex	=	0;
		player.m_strName[iNameLength]	=	'_';
		player.m_strName[iNameLength+1]	=	'A'+_lpPacket->iWorldIndex;
		player.m_strName[iNameLength+2]	=	NULL;
	}

//	printf("[GVG] %s %d",player.m_strName,player.m_wProcessChapterIndex);

	player.m_wAvatarIndex	=	0;	//	0번에만 생성 가능
	player.m_wCurrentField	=	801;
	player.m_wLastVillage	=	801;
	player.m_wGuildSerial	=	0xffff;
	player.m_iXPos			=	-1;
	player.m_iYPos			=	-1;

//	해당 인덱스에 이미 있다면 얻어온다.
	DWORD	dwDbIndex		=	0x7fffffff;
	DWORD	dwTableIndex	=	g_Im.getTableIndexByAvaIndex(player.m_strId,0);

	DWMSG_TRANS_AVATAR_TO_GVG_RESULT	resultPacket;

	resultPacket.base.set(sizeof(resultPacket),dDWMSG_TRANS_AVATAR_TO_GVG_RESULT);
	resultPacket.wWorldServerIndex	=	_lpPacket->iWorldIndex;

	strcpy(resultPacket.strName,_lpPacket->avatarData.m_strName);
	strcpy(resultPacket.strNewName,player.m_strName);

	if	(dwTableIndex < 0xffffffff)	//	이미 캐릭터가 존재한다.
	{	//	얻어온다
		WORD			wGuildIndex	=	0xffff;
		cPLAYER_DATA	existAvatarData;
		WORD			wOperLevel;
		int				iResult		=	-1;

#ifdef	_FOR_KOREA
		printf("GVG 기존 아바타 체크 %d\n",dwTableIndex);
#endif

		g_adoAvatar.spGetAvaInfo(g_Im.idxTable[dwTableIndex].strName,g_Im.idxTable[dwTableIndex].strId,&existAvatarData,wOperLevel);

		if	(existAvatarData.m_wAvatarIndex	<	dMAX_AVATAR_COUNT)
		{
			if	(stricmp(player.m_strName,existAvatarData.m_strName) !=	0)
			{
				resultPacket.wResult	=	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_ALREADY_EXIST_DIFF_AVATAR;

				_lpWorldServer->AddSendPacket((char*)&resultPacket,resultPacket.base.wSize);

				return;				
			}

			player.m_dwSaveCount	=	existAvatarData.m_dwSaveCount+1;
			strcpy(player.m_strName,existAvatarData.m_strName);

			player.m_wGuildSerial	=	existAvatarData.m_wGuildSerial;
			player.m_wCurrentField	=	existAvatarData.m_wCurrentField;
			player.m_wLastVillage	=	existAvatarData.m_wLastVillage;
			player.m_iXPos			=	existAvatarData.m_iXPos;
			player.m_iYPos			=	existAvatarData.m_iYPos;
			player.m_wGuildRank		=	existAvatarData.m_wGuildRank;

			player.m_wBoostExpSecondByTreasureMapEvent	=	0;
			player.m_wBoostGoldSecondByTreasureMapEvent	=	0;
			player.m_wIncreaseFinalDamageTime			=	0;
			player.m_wIncreaseFinalDamage				=	0;

			iResult		=	g_adoAvatar.spSaveAva(&player,TRUE);
		}

		if	(iResult	==	eSAVE_SUCCESS_DB)	//	저장 성공
		{
			strcpy(resultPacket.strNewName,player.m_strName);

			resultPacket.wResult	=	eTRAN_AVATAR_TO_GVG_RESULT_SUCCESS;
			_lpWorldServer->AddSendPacket((char*)&resultPacket,resultPacket.base.wSize);

			return;
		}

#ifdef	_FOR_KOREA
		printf("GVG 아바타 저장 실패\n");
#endif
		//	저장 실패

		resultPacket.wResult	=	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_SAVE_AVATAR;

		_lpWorldServer->AddSendPacket((char*)&resultPacket,resultPacket.base.wSize);

		return;
	}

	int	iAvaIndex	=	g_adoAvatar.spCreateAva(dwDbIndex,&player,g_Im.getBlankPos(player.m_strId));

	if	(iAvaIndex	<	0)	//	생성 실패
	{
		if	(iAvaIndex	==	-3)
		{
			WORD	wGuild	=	0xffff;
			int		iResult	=	g_adoAvatar.spDeleteAva(player.m_strId,player.m_strName,wGuild);

			iResult	=	iResult;
		}

		resultPacket.wResult	=	eTRAN_AVATAR_TO_GVG_RESULT_FAILED_BY_CREATE_AVATAR;

		_lpWorldServer->AddSendPacket((char*)&resultPacket,resultPacket.base.wSize);
		
		return;
	}
	else
	{
		if	(dwDbIndex==0x7fffffff)
		{
			g_Im.removeAll(player.m_strId);

			_DBINDEXTABLE	avaList;
			g_adoAvatar.GetDBIndex(player.m_strId,avaList);

			for(int i = 0;i<dMAX_AVATAR_COUNT;i++)
			{
				if	(avaList.wAvatarIndex[i]==0xffff)
					continue;

				int	iIndex	=	g_Im.getIndex(avaList.strName[i],player.m_strId);

				if	(iIndex	==	0x7fffffff)
				{
					DWORD ret = g_Im.add(avaList.dwIndex[i],avaList.strName[i],player.m_strId,avaList.wAvatarIndex[i]);
					_log(" RE ADD DBINDEX : [%d] [%s] [%s] [%d],ret %d",avaList.dwIndex[i],avaList.strName[i],player.m_strId,avaList.wAvatarIndex[i],ret);
				}
			}
		}
		else
		{
			DWORD ret = g_Im.add(dwDbIndex,player.m_strName,player.m_strId,player.m_wAvatarIndex);

			_log(" ADD DBINDEX : [%d] [%s] [%s] [%d],ret %d",dwDbIndex,player.m_strName,player.m_strId,player.m_wAvatarIndex,ret);
		}

		g_sharedMemory.beginProcess();
		g_lpCdm->removeByIdAndIndex(player.m_strId,iAvaIndex);
		g_sharedMemory.endProcess();
	}

	resultPacket.wResult	=	eTRAN_AVATAR_TO_GVG_RESULT_SUCCESS;	//	성공

	strcpy(resultPacket.strNewName,player.m_strName);

	_lpWorldServer->AddSendPacket((char*)&resultPacket,resultPacket.base.wSize);
}

void
OperateW2D_ASK_GVG_AVATAR_ORG_DATA(CClient *_lpWorldServer,W2D_ASK_GVG_AVATAR_ORG_DATA *_lpPacket)
{
//	printf("[GVG] receive W2D_ASK_GVG_AVATAR_ORG_DATA\n");

	char	strOrgName[dNAME_LENGTH];

	int		iNameLength	=	strlen(_lpPacket->strName);

	memcpy(strOrgName,_lpPacket->strName,dNAME_LENGTH);

	if	(_lpPacket->wNotUseUnderBarName)
		strOrgName[iNameLength-1]	=	NULL;
	else
		strOrgName[iNameLength-2]	=	NULL;

//	printf("[GVG] %s %d\n",strOrgName,_lpPacket->wNotUseUnderBarName);

	cPLAYER_DATA	existAvatarData;

	WORD			wOperLevel;
	int				iResult		=	-1;

	g_adoAvatar.spGetAvaInfo(strOrgName,_lpPacket->strId,&existAvatarData,wOperLevel);

	if	(existAvatarData.m_wAvatarIndex	>=	dMAX_AVATAR_COUNT)	//	없네?
	{
		D2W_ASK_GVG_AVATAR_ORG_DATA_RESULT	packet;

		packet.base.set(sizeof(packet),dD2W_ASK_GVG_AVATAR_ORG_DATA_RESULT);

		memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
		packet.wResult	=	eGET_GVG_AVATAR_ORG_DATA_RESULT_FAILED_CAN_NOT_FIND_ORG_AVATAR;

		_lpWorldServer->AddSendPacket((char *)&packet,packet.base.wSize);

		return;
	}

	D2W_GVG_AVATAR_ORG_DATA		packet;

	packet.base.set(sizeof(packet),dD2W_GVG_AVATAR_ORG_DATA);

	memcpy(packet.strName,_lpPacket->strName,dNAME_LENGTH);
	memcpy(&packet.avatarData,&existAvatarData,sizeof(packet.avatarData));

	CM_COMPRESS		compPacket;
	WORD			wCompSize	=	cSRVUTIL::MakeCompressPacket(&compPacket,&packet,packet.base.wSize);

	if	(wCompSize == 0)
	{
		while(!_lpWorldServer->AddSendPacket((char *)&packet,packet.base.wSize))
		{
			Sleep(50);

			if	(_lpWorldServer->GetIsClose())
				break;
		}
	}
	else
	{
		while(!_lpWorldServer->AddSendPacket((char *)&compPacket,compPacket.wSize))
		{
			Sleep(50);

			if	(_lpWorldServer->GetIsClose())
				break;
		}
	}
}