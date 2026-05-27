#include "cPACKET_GAMESERVER.H"
#include "SFC.H"
#include "debugCode.H"

//
//	초기화
void
cPACKET_QUE::reset()
{
	m_iFirstQue			=	0;
	m_iLastQue			=	0;
	m_iCount			=	0;
	m_iMaxPacketCount	=	0;
}

//
//	패킷 추가
BOOL
cPACKET_QUE::addPacket(ALL_MSG *_lpPacket,int _iSerial)
{
	if	(m_iCount	>=	dMAX_STORE_PACKET_COUNT)
		return	FALSE;

	if	(_lpPacket->base.wSize > sizeof(ALL_MSG))
	{
		if	(g_bIsTestServer)
			ERRMSG("packet size over","0x%x",_lpPacket->base.wType);

		CLOG("packet size over","cPACKET_QUE::addPacket - _lpPacket->base.wSize > sizeof(m_aPacket[m_iLastQue].m_packet)");

		return	FALSE;
	}

	memcpy(&m_aPacket[m_iLastQue].m_packet,_lpPacket,_lpPacket->base.wSize);

	m_aPacket[m_iLastQue].m_iOwner	=	_iSerial;

	m_iLastQue++;
	m_iCount++;

	if	(m_iCount	>	m_iMaxPacketCount)
		m_iMaxPacketCount	=	m_iCount;

	if	(m_iLastQue	>=	dMAX_STORE_PACKET_COUNT)
		m_iLastQue=0;	//	처음으로 돌아간다.

	return	TRUE;
}

//
//	받은 패킷이냐?
BOOL
cPACKET_QUE::isReceivedPacket(int _iPacket)
{
	int		i;

	if (m_iCount	<=	0			)	return	FALSE;

	if (m_iFirstQue	<	m_iLastQue	)
	{
		for (i=m_iFirstQue;i<=m_iLastQue;i++)
			if (m_aPacket[i].m_packet.base.wType	==	_iPacket)	return	TRUE;
	}
	else
	{
		for (i=m_iFirstQue;i<dMAX_STORE_PACKET_COUNT;i++)
			if (m_aPacket[i].m_packet.base.wType	==	_iPacket)	return	TRUE;

		for (i=0;i<=m_iLastQue;i++)
			if (m_aPacket[i].m_packet.base.wType	==	_iPacket)	return	TRUE;
	}

	return	FALSE;
}

//
//	큐에서 패킷을 꺼낸다.
cPACKET_INFO*
cPACKET_QUE::popPacket()
{
	if (m_iCount	<=	0			)	return	NULL;

	int	iReturnQue	=	m_iFirstQue;

	m_iFirstQue++;
	m_iCount--;

	if (m_iFirstQue	>=	dMAX_STORE_PACKET_COUNT)	m_iFirstQue=0;	//	처음으로 돌아간다.

//	MSGOUT("Que : First[%d] Last[%d] Count[%d]",m_iFirstQue,m_iLastQue,m_iCount);

	return	&m_aPacket[iReturnQue];
}


//
//	큐에서 패킷을 꺼낸다.
cPACKET_INFO*
cPACKET_QUE::getOne()
{
	if (m_iCounter	<=	0)	return	NULL;

	int	iReturnQue	=	m_iCurrentQue;

	m_iCurrentQue++;
	m_iCounter--;

	if (m_iCurrentQue	>=	dMAX_STORE_PACKET_COUNT)	m_iCurrentQue=0;	//	처음으로 돌아간다.

//	MSGOUT("Que : First[%d] Last[%d] Count[%d]",m_iFirstQue,m_iLastQue,m_iCount);

	return	&m_aPacket[iReturnQue];
}
