#include "Stdafx.h"
#include "Client.h"
/////////////////////////////////////
//	define extern functions and members

BOOL RecvPacket(DWORD _dwIdx);
BOOL SendPacket(DWORD _dwIdx);
BOOL SendPacket_Add(DWORD _dwIdx);

BOOL RecvPacket(DWORD _dwIdx)
{
	CClient	* user = UserManager.GetClientPnt((WORD)_dwIdx);
	if(!user || user->m_wSerial == 0xffff)	return FALSE;
	DWORD	lpRecv;
	DWORD	Flags = 0;
	user->m_stRecvOL.IOState = 0;

	if(WSARecv(user->m_hSock, &user->m_stRecvOL.buf ,1,&lpRecv,&Flags,&user->m_stRecvOL.OL ,NULL) ==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			DisUser((WORD)_dwIdx,"RECV ERROR");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL SendPacket(DWORD _dwIdx)
{
	CClient	* user = UserManager.GetClientPnt((WORD)_dwIdx);

	if	(!user || user->m_wSerial == 0xffff)
		return FALSE;

	WORD iSendSize = (WORD)user->m_pclSendBuffer->GetData(user->m_pcSendpacket,0);

	if	(!iSendSize)
		return TRUE;

	user->m_stSendOL.IOState = 1;
	DWORD	lpSend = 0;	
	WSABUF	buf;

	user->m_wSendpacket_size	=	iSendSize;

	buf.len	=	user->m_wSendpacket_size;
	buf.buf	=	user->m_pcSendpacket;

	if(WSASend(user->m_hSock, &buf,1,&lpSend,0,&user->m_stSendOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			user->m_pclSendBuffer->Next();
			DisUser((WORD)_dwIdx,"SEND ERROR");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL	SendPacket_Add(DWORD _dwIdx)
{
	CClient	* user = UserManager.GetClientPnt((WORD)_dwIdx);

	if	(!user || user->m_wSerial == 0xffff)	
		return FALSE;

	user->m_stSendOL.IOState = 1;

	DWORD	lpSend = 0;
	WSABUF	buf;

	buf.len = user->m_wSendpacket_size;
	buf.buf = user->m_pcSendpacket;

	if(WSASend(user->m_hSock, &buf,1,&lpSend,0,&user->m_stSendOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			user->m_pclSendBuffer->Next();
			DisUser((WORD)_dwIdx,"SEND_ADD ERROR");
			return FALSE;
		}
	}
	return TRUE;
}



