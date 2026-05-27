// PacketBuffer.cpp: implementation of the CPacketBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "PacketBuffer.h"
#include "RSComm.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPacketBuffer::CPacketBuffer(int _iLength /*= dBUFFERLENGTH*/)
{
	InitializeCriticalSection(&m_stCS);
	m_iLength = _iLength;
	m_iCurLength = 0;
	m_iCurPos = 0;
	m_iAddPos = 0;
	m_isCanGet = 1;
	m_pBuffer = new char[m_iLength];
	if(!m_pBuffer)
		m_iLength = 0;
}

CPacketBuffer::~CPacketBuffer()
{
	if(m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
	DeleteCriticalSection(&m_stCS);
}

int	CPacketBuffer::ReSize(int _iLength)
{
	CCritical	cs(&m_stCS);

	int oldLength = m_iLength;
	m_iLength = _iLength;
	
	char * tmp = new char[oldLength];
	if(!tmp)
		return 0;	//	혹시 있을지도 모를 데이터가 손실되므로 무효 
	if(m_pBuffer)
	{
		if(_iLength < m_iLength)	//	혹시 있을지도 모를 데이터가 손실되므로 무효 
			return 0;

		if((m_iCurPos + m_iCurLength) >= oldLength)
		{
			int len = (oldLength - m_iCurPos);
			memcpy(tmp,m_pBuffer+m_iCurPos,len);
			int len2 = m_iCurLength - len;
			memcpy(tmp+len,m_pBuffer,len2);
		}else
			memcpy(tmp,m_pBuffer+m_iCurPos,m_iCurLength);

		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
	m_pBuffer = new char[m_iLength];
	if(!m_pBuffer)
	{
		delete [] tmp; tmp = NULL;
		return 0;
	}
	memset(m_pBuffer,0,m_iLength);
	if(m_iCurLength)
	{
		memcpy(m_pBuffer,tmp,m_iCurLength);
	}
	m_iCurPos = 0;
	m_iAddPos = m_iCurLength;
	m_isCanGet = 1;

	delete [] tmp;tmp = NULL;
	
	return 1;
}

void	CPacketBuffer::Reset()
{
	CCritical	cs(&m_stCS);
	m_iCurLength = 0;
	m_iCurPos = 0;
	m_iAddPos = 0;
	m_isCanGet = 1;
	memset(m_pBuffer,0,m_iLength);
}

int		CPacketBuffer::AddData(char * _pData, int size)		//	return succ or fail
{
	CCritical	cs(&m_stCS);
	if(m_iCurLength+size > m_iLength)
	{
//		_log("ERROR m_iCurLength(%d)+size(%d) > m_iLength(%d)",m_iCurLength, size ,m_iLength);
		return 0;
	}
	if(m_iAddPos+size > m_iLength)	//	
	{
		int iFSize = m_iLength - m_iAddPos;
		memcpy(m_pBuffer + m_iAddPos,_pData,iFSize);
		m_iAddPos = 0;
		int iLSize = size - iFSize;
		memcpy(m_pBuffer + m_iAddPos,_pData+iFSize,iLSize);
		m_iAddPos += iLSize;
	}else
	{
		memcpy(m_pBuffer + m_iAddPos,_pData,size);
		m_iAddPos += size;
	}
	m_iCurLength += size;
	if(m_iAddPos>=m_iLength)
	{
		m_iAddPos -= m_iLength;
	}
	return size;
}

int		CPacketBuffer::GetData(char * _pData, int _isDelete)
{
	CCritical	cs(&m_stCS);
	if(m_iCurLength<4)		return 0;
	if(!m_isCanGet)
	{
		return 0;
	}
	WORD wSize;

	if	(m_iCurPos == m_iLength-1)
	{	
		memcpy((char *)&wSize,m_pBuffer + m_iCurPos,1);
		memcpy(((char *)&wSize+1),m_pBuffer,1);
	}
	else
	{
		memcpy(&wSize,m_pBuffer + m_iCurPos,sizeof(WORD));
	}
	
	if	(m_iCurLength == 0 || m_iCurLength < wSize)
	{
		return 0;
	}

	if	(wSize == 0 || wSize >=0xcccc)
	{
//		_log("ERROR m_iCurLength %d,m_iCurPos %d, m_iAddPos %d,m_isCanGet =%d,wSize = %d",m_iCurLength,m_iCurPos,m_iAddPos,m_isCanGet,wSize);
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
	{
		memcpy(_pData , m_pBuffer + m_iCurPos , wSize);
	}
	m_iCurPos += wSize;
	if(m_iCurPos>=m_iLength)
	{
		m_iCurPos -= m_iLength;
	}
	if(*(WORD *)_pData ==0 || *(WORD *)_pData >=65535)
	{
//		printf("=-=");	//	size : 1478 , 1406
		return 0;
	}
	m_iCurLength -= wSize;
	m_isCanGet = _isDelete;
	return (int)wSize;
}

int		CPacketBuffer::Next()		//	delete 1st data
{
	CCritical	cs(&m_stCS);
	m_isCanGet = 1;
	return 1;
}

void	CPacketBuffer::DumpData(char * _strPath)
{
#ifdef _DEBUG
	CCritical	cs(&m_stCS);

	if(!m_iCurLength)
		return;

	char strFn[256];
	sprintf(strFn,"%s/DumpData_%d_size%d.dat",_strPath,timeGetTime(),m_iCurLength);
	FILE * fp = NULL;
	fp = fopen(strFn,"wb");
	if(!fp)
		return ;

	if( (m_iCurLength + m_iCurPos) > m_iLength)
	{
		int size = (m_iCurLength + m_iCurPos) - m_iLength;
		fwrite(m_pBuffer+m_iCurPos,size,1,fp);
		fwrite(m_pBuffer,m_iCurLength - size,1,fp);
	}else
		fwrite(m_pBuffer+m_iCurPos,m_iCurLength,1,fp);

	fclose(fp);
#endif
}
