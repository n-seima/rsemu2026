#include "Client.h"

cLoginInfoList::cLoginInfoList()
{
	m_mapList.clear();
}

cLoginInfoList::~cLoginInfoList()
{
}

void cLoginInfoList::release()
{
	while(!m_mapList.empty())
	{
		m_mapList.erase( m_mapList.begin() );
	}
}

BOOL cLoginInfoList::add(DWORD _serial, char * _strId)
{
	cLoginInfo	info(_serial, _strId);

	int size = m_mapList.size();
	m_mapList[ info.m_dwHashIdCode ] = info;
	size = m_mapList.size();

	return TRUE;
}

BOOL cLoginInfoList::remove(DWORD _hash, BOOL isWriteLog)
{
	if(isWriteLog)
	{
		_log("LIL : id[%s] , login time [%d] , SPType [0X%x, %d] , RPType [0X%x, %d]",m_mapList[ _hash ].m_strId , m_mapList[ _hash ].m_dwRegTime , 
			m_mapList[ _hash ].m_wSendPacketType , m_mapList[ _hash ].m_dwSendTime,
			m_mapList[ _hash ].m_wRecvPacketType , m_mapList[ _hash ].m_dwRecvTime
			);
	}
	int size = m_mapList.size();
	m_mapList.erase( _hash );
	size = m_mapList.size();
	return TRUE;
}

BOOL cLoginInfoList::setData(DWORD _hash, ePACKETTYPE _type, WORD _wType)
{
	if(_type == eSEND_DATA)
		m_mapList[ _hash ].setSendData(_wType);
	else
		m_mapList[ _hash ].setRecvData(_wType);

	return TRUE;
}
