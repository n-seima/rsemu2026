#include "cFIELD.H"
#include "cGAME.H"
#include "Client.H"
#include "packetManager.H"

//
//	플레이어에게 CP충전했다고 메시지 전송
void
cFIELD::sendChargeCP(cACTOR *_lpActor,int _iCP,int _iSkill)
{
	SG_CHARGE_CP	packet;

	packet.base.set(sizeof(packet),dSG_CHARGE_CP);
	packet.sCP		=	_iCP/100;
	packet.wSkill	=	_iSkill;
	
	g_userPM.add(_lpActor->m_iClientSerial,&packet,packet.base.wSize);
}

//
//	스훼셜 스킬 휠드가 변경되었다.
void
cFIELD::sendChangeSpecialSkillField(cACTOR *_lpCaster,cAbility *_lpAbility)
{
	SG_CHANGE_SPECIAL_SKILL_FIELD	packet;

	packet.base.set(sizeof(SG_CHANGE_SPECIAL_SKILL_FIELD),dSG_CHANGE_SPECIAL_SKILL_FIELD);
	packet.wSpecialSkillField	=	_lpCaster->m_wSpecialSkillField;

	if	(_lpAbility)
		memcpy(&packet.ability,_lpAbility,sizeof(cAbility));

	g_userPM.add(_lpCaster->m_iClientSerial,&packet,packet.base.wSize);
}

// 모든 운영자에게 패킷 전송
BOOL
cFIELD::sendPacketToAllOperator(ALL_MSG *_lpPacket,int _iLevel)
{
	_iLevel	=	max(_iLevel,1);

	for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		for (int i=0;i<m_aiActorCount[iTeam];i++)
		{
			cACTOR	*lpPlayer	=	getActor(m_awActorList[iTeam][i]);

			if	(!lpPlayer || !lpPlayer->isPlayer())	
				continue;

			if	(lpPlayer->m_wOperatorLevel < _iLevel)
				continue;

			g_userPM.add(lpPlayer->m_iClientSerial,_lpPacket,_lpPacket->base.wSize);
		}

	return	TRUE;
}

void
cFIELD::sendUseItemResult(int _iClientSerial, WORD _wSerialInField,WORD _wResult)
{
	// 아이템사용결과
	SG_USE_ITEM_RESULT	sgPacket;
	sgPacket.base.set(sizeof(sgPacket),dSG_USE_ITEM_RESULT);
	sgPacket.wTarget	=	_wSerialInField;
	sgPacket.wResult	=	_wResult;
	g_userPM.add(_iClientSerial,&sgPacket,sgPacket.base.wSize);
}



// 모든 플레이어에게 패킷 전송
BOOL
cFIELD::sendPacketToAllPlayer(void *_lpPacket)
{
	cMSG_BASE_TYPE	*lpPacket	=	(cMSG_BASE_TYPE	*)_lpPacket;

	for (int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
		for (int i=0;i<m_aiActorCount[iTeam];i++)
		{
			cACTOR	*lpPlayer	=	getActor(m_awActorList[iTeam][i]);

			if	(!lpPlayer || !lpPlayer->isPlayer())	
				continue;

			g_userPM.add(lpPlayer->m_iClientSerial,lpPacket,lpPacket->wSize);
		}

	return	TRUE;
}

void
cFIELD::sendPacketToSameLocalPlayer(int _iLocal,cRECT *_lpRect,void *_lpPacket,int _iPacketSize)
{
	for(int iTeam=0;iTeam<1+m_bIsGuildBattleField;iTeam++)
	{
		for (int i=m_pLocalPartition[_iLocal].m_aiActorCount[iTeam];i;i--)
		{
			int		iSerial	=	m_pLocalPartition[_iLocal].m_apActorList[iTeam][i-1];

			cACTOR	*lpActor=	getTestedActor(iSerial);

			if	(!lpActor	||	!lpActor->isPlayer())
				continue;

			if	(_lpRect->Isin(lpActor->m_pos.x,lpActor->m_pos.y))
				g_userPM.add(lpActor->m_iClientSerial,_lpPacket,_iPacketSize);
		}
	}
}