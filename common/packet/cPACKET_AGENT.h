#pragma once

#include "cPACKET_BASE.h"
#include "definePacketData.h"

#define	dIP_LENGTH	16

#pragma pack(1)

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Root Agent
//┃패킷 인덱스 정의
#define	dCR_STATUS						0x4001	//	로그인
//┃Client->Root Agent
//┃패킷 인덱스 정의
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Root Agent->Client
//┃패킷 인덱스 정의
//	━━필드 진입 전					0x1101 ~ 0x1200
#define	dRC_LOGIN_RESULT				0x4101	//	로그인 결과
//┃Root Agent->Client
//┃패킷 인덱스 정의
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃Client->Server
//┃로그인/로그아웃

class	cAgentServerInfo
{
public:
	WORD			m_wServerType;
	WORD			m_wServerStatus;
	int				m_iWorldIndex;
	int				m_iGameServerIndex;
	int				m_iServerVersion;
	char			m_strWorldIP[dIP_LENGTH];
	char			m_strName[dNAME_LENGTH];
	char			m_strIP[dIP_LENGTH];
};

struct	CR_STATUS
{
	cMSG_BASE_TYPE_INNER	base;

	cAgentServerInfo		m_serverInfo;
};

struct	RC_LOGIN
{
	cMSG_BASE_TYPE_INNER	base;

	WORD			wResult;	//	결과
};

#pragma pack()

