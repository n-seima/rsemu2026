#include "cAGENT.H"

cQUE_PACKET::cQUE_PACKET()
{
	reset();

	InitializeCriticalSection(&m_csOperateQue);
}

cQUE_PACKET::~cQUE_PACKET()
{

	DeleteCriticalSection(&m_csOperateQue);
}

//
//	초기화
void
cQUE_PACKET::reset()
{
	m_iFirstQue	=	0;
	m_iLastQue	=	0;
	m_iCount	=	0;
}	//	cQUE_PACKET::reset()

//
//	패킷 추가
BOOL
cQUE_PACKET::addPacket(SERVER_PACKETS *_lpPacket)
{
	EnterCriticalSection(&m_csOperateQue);

	if (m_iCount	>=	dMAX_PACKET_QUE_COUNT)
	{
		LeaveCriticalSection(&m_csOperateQue);
		return	FALSE;
	}

	memcpy(&m_aPacket[m_iLastQue],_lpPacket,_lpPacket->base.wSize);

	m_iLastQue++;
	m_iCount++;

	if (m_iLastQue	>=	dMAX_PACKET_QUE_COUNT)		m_iLastQue=0;	//	처음으로 돌아간다.

	LeaveCriticalSection(&m_csOperateQue);

	return	TRUE;
}	//	cQUE_PACKET::addPacket(SERVER_PACKETS *_lpPacket,int _iPacketSize)

//
//	받은 패킷이냐?
BOOL
cQUE_PACKET::isReceivedPacket(int _iPacket)
{
	int		i;

	if (m_iCount	<=	0			)	return	FALSE;

	if (m_iFirstQue	<	m_iLastQue	)
	{
		for (i=m_iFirstQue;i<=m_iLastQue;i++)
			if (m_aPacket[i].base.wType	==	_iPacket)	return	TRUE;
	}
	else
	{
		for (i=m_iFirstQue;i<dMAX_PACKET_QUE_COUNT;i++)
			if (m_aPacket[i].base.wType	==	_iPacket)	return	TRUE;

		for (i=0;i<=m_iLastQue;i++)
			if (m_aPacket[i].base.wType	==	_iPacket)	return	TRUE;
	}

	return	FALSE;
}	//	cQUE_PACKET::isReceivedPacket(int _iPacket)

//
//	큐에서 패킷을 꺼낸다.
SERVER_PACKETS*
cQUE_PACKET::popPacket()
{
	EnterCriticalSection(&m_csOperateQue);

	if (m_iCount	<=	0			)
	{
		LeaveCriticalSection(&m_csOperateQue);
		return	NULL;
	}

	int	iReturnQue	=	m_iFirstQue;

	m_iFirstQue++;
	m_iCount--;

	if (m_iFirstQue	>=	dMAX_PACKET_QUE_COUNT)	m_iFirstQue=0;	//	처음으로 돌아간다.

//	MSGOUT("Que : First[%d] Last[%d] Count[%d]",m_iFirstQue,m_iLastQue,m_iCount);

	LeaveCriticalSection(&m_csOperateQue);

	return	&m_aPacket[iReturnQue];
}	//	cQUE_PACKET::popPacket()