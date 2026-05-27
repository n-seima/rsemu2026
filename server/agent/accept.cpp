#include "stdafx.h"
#include "accept.h"
#include "client.h"
#include "AgentDlg.h"

#define	dMAX_PACKET_LENGTH_SERVER	256000

void
AcceptThread(void * _pParam)
{
	CAgentDlg	*lpMainDlg	=	(CAgentDlg	*)_pParam;

	while(g_bIsMainLoop)
	{
		Sleep(1);

		SOCKET	hClientsock	=	accept(lpMainDlg->getListenSocket(),NULL,NULL);

		if	(hClientsock ==	 INVALID_SOCKET)
			continue;

		if	(g_bIsMainLoop==	FALSE)
			continue;

		int iBufSize	=	dMAX_PACKET_LENGTH_SERVER;

		if	(SOCKET_ERROR	==	setsockopt(hClientsock,SOL_SOCKET ,SO_SNDBUF,(const char*)&iBufSize,sizeof(iBufSize)))
		{
			shutdown(hClientsock,1);
			closesocket(hClientsock);
			hClientsock = 0;
			continue;
		}

		if	(SOCKET_ERROR	==	setsockopt(hClientsock,SOL_SOCKET ,SO_RCVBUF,(const char*)&iBufSize,sizeof(iBufSize)))
		{
			shutdown(hClientsock,1);
			closesocket(hClientsock);
			hClientsock = 0;
			continue;
		}

		LINGER	lingerStruct;
		
		lingerStruct.l_onoff = 1;
		lingerStruct.l_linger = 0; 

		if	(SOCKET_ERROR	==	setsockopt(hClientsock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
		{
			shutdown(hClientsock,1);
			closesocket(hClientsock);
			hClientsock	=	0;
			continue;
		}

		WORD	wIndex	=	UserManager.Add();

		if	(wIndex==0xffff)
		{
			shutdown(hClientsock,1);
			closesocket(hClientsock); 
			continue;
		}

		CClient		*lpNewUser		=	UserManager.GetClientPnt(wIndex);
		int			iSocketAddrSize	=	sizeof(sockaddr);
		SOCKADDR_IN	saddr;

		memset(&saddr,0,sizeof(saddr));

		getpeername(hClientsock,(SOCKADDR *)&saddr,&iSocketAddrSize);

		lpNewUser->Open(wIndex,hClientsock,inet_ntoa(saddr.sin_addr));

		{
			char		strIp[20];
			SOCKADDR_IN	saddr;

			memset(&saddr,0,sizeof(saddr));
			int tmp	=	sizeof(sockaddr);
			getpeername(hClientsock,(SOCKADDR *)&saddr,&tmp);

			strcpy(strIp,inet_ntoa(saddr.sin_addr));

			strcpy(lpNewUser->m_strIp,strIp);
		}

		lpMainDlg->PrintMainMessage("accept %s",lpNewUser->m_strIp);

		if	(!CreateIoCompletionPort((HANDLE)lpNewUser->m_hSock,lpMainDlg->getCompletionPort(),(DWORD)wIndex,0))
		{
			lpMainDlg->DisUser(wIndex,"accept error");
			continue;
		}

		lpMainDlg->RecvPacket(wIndex);
	}

	return;
}