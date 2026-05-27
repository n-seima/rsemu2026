// BufferForPacket.cpp: implementation of the CBufferForPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "debugCode.h"
#include "RSComm.h"
#include "BufferForPacket.h"
#include "GameDLL.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBufferForPacket::CBufferForPacket(int _iLength /*= dBUFFERLENGTH*/)
{
	FPInitCS(&m_stCS);

	m_iLength = _iLength;
	m_iCurLength = 0;
	m_iCurPos = 0;
	m_iAddPos = 0;
	m_isCanGet = 1;
	m_pBuffer = new char[m_iLength];
	if(!m_pBuffer)
		m_iLength = 0;
}

CBufferForPacket::~CBufferForPacket()
{
	if(m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
	DeleteCriticalSection(&m_stCS);
}

void	CBufferForPacket::Reset()
{
	CCritical	cs(&m_stCS);
	m_iCurLength = 0;
	m_iCurPos = 0;
	m_iAddPos = 0;
	m_isCanGet = 1;
	memset(m_pBuffer,0,m_iLength);
}

int		CBufferForPacket::AddData(char * _pData, int size)		//	return succ or fail
{
	CCritical	cs(&m_stCS);
#ifdef _DEBUG_FOR_BUFFER_SAFETY
	m_dwLastRecvTime = timeGetTime();
#endif

	if	(m_iCurLength+size > m_iLength)
	{
		//	임시 테스트.
#ifdef _FOR_KOREA
//		_log(" ERRORERRORERRORERROR : ****************\nERROR : m_iCurLength(%d)+size(%d) > m_iLength(%d)",m_iCurLength,size , m_iLength);
#endif
		return 0;
	}

	if	(m_iAddPos+size > m_iLength)
	{
		int iFSize = m_iLength - m_iAddPos;

		memcpy(m_pBuffer + m_iAddPos,_pData,iFSize);
		m_iAddPos = 0;

		int iLSize = size - iFSize;

		memcpy(m_pBuffer + m_iAddPos,_pData+iFSize,iLSize);
		m_iAddPos += iLSize;
	}
	else
	{
		memcpy(m_pBuffer + m_iAddPos,_pData,size);
		m_iAddPos += size;
	}

	m_iCurLength += size;

	if	(m_iAddPos>=m_iLength)
	{
		m_iAddPos -= m_iLength;
	}

	return size;
}

int		CBufferForPacket::GetData(char * _pData, int _isDelete)	//	return size
{
	CCritical	cs(&m_stCS);

	if	(!m_isCanGet)
	{
#ifdef _DEBUG_FOR_BUFFER_SAFETY
#ifdef _FOR_KOREA
		if(abs(timeGetTime() - m_dwLastSendCheckTime) > eGAP_FOR_ISCANGET)
		{
//			_log(" ERROR : _DEBUG_FOR_BUFFER_SAFETY  %d",abs(timeGetTime() - m_dwLastSendCheckTime)); 
		}
#endif
#endif
		return 0;
	}

	if	(m_iCurLength<4)
		return 0;
#ifdef _DEBUG_FOR_BUFFER_SAFETY
	m_dwLastSendTime = timeGetTime();
#endif
	WORD wSize;

	if	(m_iCurPos == m_iLength-1)
	{
		memcpy((char *)&wSize,m_pBuffer + m_iCurPos,1);
		memcpy(((char *)&wSize+1),m_pBuffer,1);
	}
	else
		memcpy(&wSize,m_pBuffer + m_iCurPos,sizeof(WORD));


	if	(m_iCurLength == 0 || m_iCurLength < wSize)
	{
#ifdef _FOR_KOREA
		_log("Warning 1: m_iCurLength %d , wSize %d",m_iCurLength,wSize);
#endif
		m_iCurPos		=	0;
		m_iCurLength	=	0;
		m_iAddPos		=	0;
//	m_iCurLength < wSize : 클라이언트가 보낸 패킷의 사이즈 (wSize)가 현재 가진 패킷의 사이즈보다 크다. 
//	m_iAddPos = 0으로 세팅하고 return 을 0으로 하면 다시 패킷의 왕래는 가능하지만 정상적인 진행이 불가능 할 수도 있다.
		return 1;	//	이런 문제가 발생하면 자르자.  2005/11/23 이지현 주임
	}

	if(wSize == 0 || wSize >=dBUFFERLENGTH)
	{
#ifdef _FOR_KOREA
		if(wSize>0)
			_log("Warning 2: m_iCurLength %d,m_iCurPos %d, m_iAddPos %d,m_isCanGet =%d,wSize = %d",m_iCurLength,m_iCurPos,m_iAddPos,m_isCanGet,wSize);
#endif
		m_iCurLength = 0;
		m_iCurPos = 0;
		m_iAddPos = 0;
		m_isCanGet = 1;
		memset(m_pBuffer,0,m_iLength);
		return 0;
	}

	if(m_iCurPos+wSize > m_iLength)
	{
		int	iFSize = m_iLength - m_iCurPos;
		memcpy(_pData , m_pBuffer + m_iCurPos , iFSize);
		memcpy(_pData+iFSize, m_pBuffer, wSize - iFSize);
	}else
		memcpy(_pData , m_pBuffer + m_iCurPos , wSize);

	m_iCurPos += wSize;
	if(m_iCurPos>=m_iLength)
		m_iCurPos -= m_iLength;

	m_iCurLength -= wSize;
	if(m_iCurLength<0)
	{
#ifdef _FOR_KOREA
		_log("ERROR 3: m_iCurLength %d",m_iCurLength);
#endif
		m_iCurLength = 0;
	}
	m_isCanGet = _isDelete;
	return (int)wSize;
}

int		CBufferForPacket::Next()		//	delete 1st data
{
	CCritical	cs(&m_stCS);
#ifdef _DEBUG_FOR_BUFFER_SAFETY
	m_dwLastSendCheckTime = timeGetTime();
#endif
	m_isCanGet  = 1;
	return 1;
}
