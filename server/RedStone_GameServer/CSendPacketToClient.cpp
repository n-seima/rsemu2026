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
#include "processThread.h"

//
//	 패킷을 모든 플레이어에게 전송
void
cGAME::sendPacketToAllPlayer(char *_lpPacket)
{
	for (int iField=m_iFieldCount;iField;iField--)
		m_pField[iField-1].addSendPacket((ALL_MSG*)_lpPacket,0xffff);

	g_pIfManager->sendPacketToAllPlayer(_lpPacket);
}

//
//	 패킷을 모든 운영자에게 전송
void
cGAME::sendPacketToAllOperator(char *_lpPacket,int _iLevel)
{
	for (int iField=m_iFieldCount;iField;iField--)
		m_pField[iField-1].sendPacketToAllOperator((ALL_MSG*)_lpPacket,_iLevel);

	g_pIfManager->sendPacketToAllPlayer(_lpPacket);
}


void
cGAME::notice(char *_lpstrNotice)
{
	SG_NOTICE	packet;

	packet.base.set(sizeof(packet),dSG_NOTICE);

	strncpy(packet.strMessage,_lpstrNotice,sizeof(packet.strMessage)-2);

	packet.base.wSize	=	sizeof(packet)-sizeof(packet.strMessage)+strlen(packet.strMessage)+1;

	sendPacketToAllPlayer((char *)&packet);
}

void
cGAME::sendEtcWorkPacketToAllPlayer(int _iWork,int _iValue1,int _iValue2,int _iValue3,int _iValue4,int _iValue5,int _iValue6)
{
	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ETC_WORK);
	packet.wWork		=	_iWork;
	packet.aiValue[0]	=	_iValue1;
	packet.aiValue[1]	=	_iValue2;
	packet.aiValue[2]	=	_iValue3;
	packet.aiValue[3]	=	_iValue4;
	packet.aiValue[4]	=	_iValue5;
	packet.aiValue[5]	=	_iValue6;

	sendPacketToAllPlayer((char *)&packet);
}


void
cGAME::sendDisconnectByHackShieldPacket(CClient *_lpClient)
{
#ifdef	_USE_HS	
	SG_ETC_WORK	packet;

	packet.base.set(sizeof(packet),dSG_ETC_WORK);
	packet.wWork	=	eEW_DISCONNECT_BY_HACKSHIELD;

	_lpClient->m_wAppointmentDisconnectTime	=	2;
	
	_lpClient->AddSendPacket((char *)&packet,packet.base.wSize);
	SendPacket(_lpClient->GetSerial());	
#endif
}
