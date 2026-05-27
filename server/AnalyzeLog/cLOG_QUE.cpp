#include "cPACKET_LOGSERVER.h"

//
//	초기화
void
cLOG_QUE::reset()
{
	m_iFirstQue	=	0;
	m_iLastQue	=	0;
	m_iCount	=	0;
	m_dwTotalPacketSize = 0;
}	//	cLOG_QUE::reset()

//
//	패킷 추가
BOOL
cLOG_QUE::addPacket(ALL_LOGMSG *_lpPacket)
{
	CCritical	CS(&cs);
	if (m_iCount	>=	dMAX_MSG_COUNT)	return	FALSE;

	if (_lpPacket->base.wSize > sizeof(ALL_LOGMSG))
	{
		printf("패킷사이즈오버 \n");
		return	FALSE;
	}
	memcpy(&m_aPacket[m_iLastQue],_lpPacket,_lpPacket->base.wSize);

	m_iLastQue++;
	m_iCount++;
	m_dwTotalPacketSize += _lpPacket->base.wSize;

	if (m_iLastQue	>=	dMAX_MSG_COUNT)		m_iLastQue=0;	//	처음으로 돌아간다.

	return	TRUE;
}	//	cLOG_QUE::addPacket(ALL_MSG *_lpPacket,int _iSerial)

//
//	받은 패킷이냐?
BOOL
cLOG_QUE::isReceivedPacket(int _iPacket)
{
	CCritical	CS(&cs);
	int		i;

	if (m_iCount	<=	0			)	return	FALSE;

	if (m_iFirstQue	<	m_iLastQue	)
	{
		for (i=m_iFirstQue;i<=m_iLastQue;i++)
			if (m_aPacket[i].base.wType	==	_iPacket){		return	TRUE;	}
	}
	else
	{
		for (i=m_iFirstQue;i<dMAX_MSG_COUNT;i++)
			if (m_aPacket[i].base.wType	==	_iPacket){		return	TRUE;	}

		for (i=0;i<=m_iLastQue;i++)
			if (m_aPacket[i].base.wType	==	_iPacket){		return	TRUE;	}
	}
	
	return	FALSE;
}	//	cALL_LOGMSG_QUE::isReceivedPacket(int _iPacket)

//
//	큐에서 패킷을 꺼낸다.
ALL_LOGMSG*
cLOG_QUE::popPacket()
{
	CCritical	CS(&cs);
	if (m_iCount	<=	0			)	return	NULL;
	int	iReturnQue	=	m_iFirstQue;

	m_iFirstQue++;
	m_iCount--;
	if(m_dwTotalPacketSize >= m_aPacket[iReturnQue].base.wSize)
		m_dwTotalPacketSize -= m_aPacket[iReturnQue].base.wSize;
	else
		reset();

	if (m_iFirstQue	>=	dMAX_MSG_COUNT)	m_iFirstQue=0;	//	처음으로 돌아간다.

//	MSGOUT("Que : First[%d] Last[%d] Count[%d]",m_iFirstQue,m_iLastQue,m_iCount);
	return	&m_aPacket[iReturnQue];
}	//	cLOG_QUE::popPacket()


//
//	큐에서 패킷을 꺼낸다.
ALL_LOGMSG*
cLOG_QUE::getOne()
{
	CCritical	CS(&cs);
	if (m_iCounter	<=	0)	return	NULL;
	int	iReturnQue	=	m_iCurrentQue;

	m_iCurrentQue++;
	m_iCounter--;
	if(m_dwTotalPacketSize >= m_aPacket[iReturnQue].base.wSize)
		m_dwTotalPacketSize -= m_aPacket[iReturnQue].base.wSize;
	else
		reset();
	
	if (m_iCurrentQue	>=	dMAX_MSG_COUNT)	m_iCurrentQue=0;	//	처음으로 돌아간다.

//	MSGOUT("Que : First[%d] Last[%d] Count[%d]",m_iFirstQue,m_iLastQue,m_iCount);
	return	&m_aPacket[iReturnQue];
}	//	cLOG_QUE::getOne()