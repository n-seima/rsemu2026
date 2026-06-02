// PacketBuffer.h: interface for the CPacketBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETBUFFER_H__4D0216DC_367B_4F34_AE5D_3424F8505B1F__INCLUDED_)
#define AFX_PACKETBUFFER_H__4D0216DC_367B_4F34_AE5D_3424F8505B1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
///////////////////////////////////////////////////
//	CPacketBuffer
//	:	데이터를 담을 버퍼 클래스이다.
//		memcpy등을 최대한 자제하고, loop형식으로 데이터를 저장한다.
//		암호화없이 일반적인 패킷 처리할 때 사용하자.
//	:	2005.09.24 (토)  / 이지현 / 
//	:	<조건> 데이터의 상위 2바이트는 무조건 그 데이터의 사이즈를 뜻한다.
//	:	<조건> m_iLength보다 큰 데이터은 존재하지 않아야 한다.

#define dBUFFERLENGTH	8196

class CPacketBuffer  
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
	CPacketBuffer(int _iLength = dBUFFERLENGTH);
	virtual ~CPacketBuffer();

	void	Reset();
	int		ReSize(int _iLength);
	int		AddData(char * _pData, int size);		//	return succ or fail
	int		GetData(char * _pData, int _isDelete);	//	return size
	int		Next();		//	delete 1st data
	inline int		GetSize(){	return m_iCurLength;	}
	void	DumpData(char * _strPath);
};

#endif // !defined(AFX_PACKETBUFFER_H__4D0216DC_367B_4F34_AE5D_3424F8505B1F__INCLUDED_)
