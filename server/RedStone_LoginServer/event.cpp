#include "event.h"
#include "server.h"

CEventTimeManager	g_eventTimeManager;

CEventTimeManager::CEventTimeManager()
{
	memset(this,0,sizeof(CEventTimeManager));
}

void
CEventTimeManager::updateData(int _iCount,cEventTimeDefine *_lpData)
{
	m_iCount	=	_iCount;

	memcpy(m_aEventTimeList,_lpData,sizeof(cEventTimeDefine)*_iCount);

	save();

	sendEventData(NULL);
}

void
CEventTimeManager::sendEventData(cSOCKET *_lpSocket)
{
	SERVERStoWORLD_MODIFY_EVENT_TIME	packet;

	packet.wCount	=	m_iCount;
	memcpy(packet.aEventList,m_aEventTimeList,sizeof(cEventTimeDefine)*m_iCount);

	packet.base.wType		=	dSERVERStoWORLD_MODIFY_EVENT_TIME;
	packet.base.wSize		=	sizeof(packet)-sizeof(packet.aEventList)+sizeof(cEventTimeDefine)*m_iCount;
	packet.wIsByLoginServer	=	TRUE;

	SendToWorld(&packet,_lpSocket);
}

void
CEventTimeManager::save()
{
	cFILE	file;

	file.Open(_ms("%s/data/eventTime.dat",g_strLogFolder),"wb");

	file.Write(&m_iCount,sizeof(int));
	file.Write(m_aEventTimeList,sizeof(cEventTimeDefine)*m_iCount);
	file.Close();
}

void
CEventTimeManager::load()
{
	m_iCount	=	0;

	cFILE	file;

	if	(file.Open("data/eventTime.dat","rb"))
	{
		file.Read(&m_iCount,sizeof(int));
		file.Read(m_aEventTimeList,sizeof(cEventTimeDefine)*m_iCount);
		file.Close();
	}
}