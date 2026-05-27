#ifndef __SERVER_H
#define __SERVER_H

#include "cSOCKET.h"
#include "cPACKET_GAMESERVER.h"
#include "cPACKET_LOGSERVER.h"
#include "cPACKET_WORLDSERVER.h"
#include <string>

#pragma warning( push )
#pragma warning( disable : 4786 )		// 비표준 확장이 사용되었습니다. 정규화된 이름에 'enParts' 열거형을 사용했습니다.
#include <vector>

extern cSOCKET	g_socketWorld;
extern cSOCKET	g_socketDBC;

extern	void	DisUser(int serial,char *_lpstrLog);
extern	void	DisUser(char* _strid,char *_lpstrLog);

#define	SEND_WOLRD_PACKET(packet)	g_socketWorld.SendPacket((char *)packet,((cMSG_BASE_TYPE *)packet)->wSize)

extern	char		g_strServerFolder[512];
extern	char		g_strBuildVersion[512];
extern	int			g_aiWorldValue[dMAX_WORLD_VALUE_MAX];


class cConfigData		: public cSinglton<cConfigData>
{
public:
	std::string		m_strGameName;
	std::string		m_strWorldIp;
	std::string     m_strToWorldIP;
	DWORD			m_dwWorldPort;

	std::string		m_strMailServerIP;
	std::string		m_strWCGMailTitle;
	std::string		m_strWCGMailSender;
	std::string		m_strWCGMailSubject;
	std::string		m_strWCGMailRecipient;
	std::vector<std::string>	m_listWCGMailCCRecipient;
};

#pragma warning( pop)		// 비표준 확장이 사용되었습니다. 정규화된 이름에 'enParts' 열거형을 사용했습니다.
#endif //	__SERVER_H