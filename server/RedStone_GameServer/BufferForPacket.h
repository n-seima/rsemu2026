// BufferForPacket.h: interface for the CBufferForPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFERFORPACKET_H__C63DD673_060E_492C_946D_AF332584E5AF__INCLUDED_)
#define AFX_BUFFERFORPACKET_H__C63DD673_060E_492C_946D_AF332584E5AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define dBUFFERLENGTH		8196

#define _DEBUG_FOR_BUFFER_SAFETY

///////////////////////////////////////////////////
//	CBufferForPacket
//	:	데이터를 담을 버퍼 클래스이다.
//		memcpy등을 최대한 자제하고, loop형식으로 데이터를 저장한다.
//	:	2005.09.22 (목)  / 이지현 / 
//	:	<조건> 데이터의 상위 2바이트는 무조건 그 데이터의 사이즈를 뜻한다.
//	:	<조건> m_iLength보다 큰 데이터은 존재하지 않아야 한다.

class CBufferForPacket  
{
private:
	int		m_iLength;	//	버퍼의 최대 길이
	int		m_iCurLength;//	현재 추가된 데이터의 길이
	char *	m_pBuffer;	//	버퍼
	int		m_iCurPos;	//	현재 처리해야 할 위치
	int		m_iAddPos;	//	추가되는 내용을 담을 위치
	int		m_isCanGet;	//	데이터를 가져갈 수 있나?
	CRITICAL_SECTION	m_stCS;

public:
	CBufferForPacket(int _iLength = dBUFFERLENGTH);
	virtual ~CBufferForPacket();

	void	Reset();
	int		AddData(char * _pData, int size);		//	return succ or fail
	int		GetData(char * _pData, int _isDelete);	//	return size	//	conState = eISUSED_ENDSSL
	int		Next();		//	delete 1st data

#ifdef _DEBUG_FOR_BUFFER_SAFETY
	inline int GetSize(){ return m_iCurLength; }
	DWORD	m_dwLastSendTime;
	DWORD	m_dwLastSendCheckTime;
	DWORD	m_dwLastRecvTime;

	enum
	{
		eGAP_FOR_ISCANGET = 3000,
	};
#endif
};

#endif // !defined(AFX_BUFFERFORPACKET_H__C63DD673_060E_492C_946D_AF332584E5AF__INCLUDED_)
