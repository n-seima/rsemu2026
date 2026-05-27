#include "packetManager.h"
#include "cPACKET_GAMESERVER.h"
#include "Client.h"
#include "cACTOR.h"
#include "cGame.h"

cPacketManager	g_userPM;	//	user packet manager

#define			dPACKET_TRAILER_INFO_DATA_LENGTH	12

cCFPM::cCFPM()
{
	inlineReset();
}

cCFPM::~cCFPM()
{
}

BOOL
cCFPM::init()
{
	inlineReset();

	return	TRUE;
}

BOOL
cCFPM::add(void *_lpPacket,int	_iPacketSize)
{
	if	(base.wSize + _iPacketSize >= c_iPacketTrailerDataSize)
		return	FALSE;

	char	*lpBuffer=	m_aData+base.wSize;

	memcpy(lpBuffer,_lpPacket,_iPacketSize);

	base.wSize		+=	_iPacketSize;
	m_iCount++;

	return	TRUE;
}

BOOL
cCFPM::send(int _iClient)
{
	base.wSize	+=	dPACKET_TRAILER_INFO_DATA_LENGTH;

	g_game.operateSendPacketDataStaticsWork(base.wSize);

	CM.SEND_PACKET(_iClient,&base,base.wSize);

	inlineReset();

	return	TRUE;
}

cPacketManager::cPacketManager()
{
	m_pClient	=	NULL;
}

cPacketManager::~cPacketManager()
{
	pKILL(m_pClient);
}

BOOL
cPacketManager::init()
{
	pKILL(m_pClient);

	m_pClient	=	new cCFPM [dGAME_MAX_USER_COUNT];

	for	(int i=0;i<dGAME_MAX_USER_COUNT;i++)
		m_pClient[i].init();

	return	TRUE;
}

BOOL
cPacketManager::add(int _iClientSerial,void *_lpPacket,int _iPacketSize)
{
	if	(_iClientSerial	<	0	||	_iClientSerial	>=	dGAME_MAX_USER_COUNT)
		return	FALSE;

	if	(!m_pClient[_iClientSerial].add(_lpPacket,_iPacketSize))
	{
		CClient	*lpClient	=	CM.mGETCLIENT(_iClientSerial);

		if	(lpClient)
		{
			int		iSerial	=	lpClient->GetActorSerialInServer();

			cACTOR	*lpActor=	g_game.getActor(iSerial);

			if	(lpActor)
				CLOG("packetSizeOver","[%d]%s",lpActor->m_wCurrentField,lpActor->m_lpField->m_strName);
		}

		m_pClient[_iClientSerial].send(_iClientSerial);

		m_pClient[_iClientSerial].add(_lpPacket,_iPacketSize);
	}

	return	TRUE;
}

void
cPacketManager::operate()
{
	for	(int	iClient=dGAME_MAX_USER_COUNT;iClient;)
	{
		--iClient;

		cCFPM	*lpClientPacket	=	&m_pClient[iClient];

		if	(lpClientPacket->m_iCount)
			lpClientPacket->send(iClient);
	}
}
