// Server.cpp : Defines the entry point for the application.
//
/**********************************************************************************
	Log 서버 
**********************************************************************************/

#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <iostream>
#include <io.h>
#include <tchar.h>

#include "SFC.h"
#include "Client.h"
#include "cScript.h"
#include "cSRVUTIL.h"
#include "Logger.h"
#include "cSOCKET.h"
#include "Hardwareinfo.h"

#define dUSE_SERVERSTATUS_REPORT

#ifdef _FOR_THAI
#undef dUSE_SERVERSTATUS_REPORT
#endif

#ifdef dUSE_SERVERSTATUS_REPORT
	#include "cServerStatus.h"

#endif


#define dCONFIGFILENAME				"config-toollog.txt"

static int socketcount=0;
static char mainPath[512];
static char	subpath[512];


CClientManager UserManager;

CRITICAL_SECTION csRECVPacket;
CRITICAL_SECTION csSENDPacket;
CRITICAL_SECTION csRemoveUser;
CRITICAL_SECTION csLogFile;

static DWORD	checkTime = 0;
static BOOL		isFirst	=TRUE;
static BOOL		isReady =FALSE;
static BOOL		g_bThreadLoop = TRUE;
static DWORD	dwPacketCount = 0;
HANDLE hAcceptThread,hAdditionThread,hWorkerThread[dMAX_WORKER_COUNT];

HANDLE			g_hCompletionPort;	//	IOCP에 사용되는 포트
SOCKET			lisSock;			//	listen 소켓
cSOCKET			logServer;

int				nTimer;
cScript			scp;

BOOL InitSocket();
BOOL InitCompletionPort();		//	IOCP 초기화
void WorkerThread(LPVOID lpParameter); 
void AdditionThread(LPVOID lpParameter); 
void PressThread(LPVOID lpParameter);
void ProcessPressThread(LPVOID lpParameter);
void ProcessSearchThread(LPVOID lpParameter);
void __cdecl SendErrorResult(DWORD _dwSerial,char * ,char * msg,...);

BOOL RecvPacket(int serial);
BOOL SendPacket(int serial);
void SendPacket_Add(int serial);

void	ProcessRecvPacket(int serial);
BOOL	ProcessCommand(char command);
HANDLE			hRecvThread;
void RecvThread(LPVOID lpParameter);
typedef struct 
{
//	CClient			* user;
	DWORD			serial;
	PACKET_SEARCH	searchItem;
}stParam;
//	method for packet send
void	DisUser(int serial);
//	end define

//	내부 테스트를 위한 
BOOL	l_bIsTestDB		=	FALSE;


//////////////////////////////////////
//	기타 기능을 위한 플래그 
BOOL	isSend_UserCount	=	FALSE;

DebugTracer	*	cLOG;
Logger			logger;
void RecvThread(LPVOID lpParameter)
{
/*	if(!l_bIsTestDB)
	{
		while(g_bThreadLoop)
		{
			Sleep(1);
			for(int count=0;count<dLOG_MAX_USER_COUNT;count++)
			{	
				CClient * user = UserManager.GetClientPnt(count);
				if(!user || user->GetSerial()==0xffff)	continue;
				ProcessRecvPacket(user->GetSerial());
			}
		}
	}
*/	_log("End RecvThread... ");
	return;
}

void main()
{
#ifdef dUSE_SERVERSTATUS_REPORT
	g_SS.init("ToolLog", "Server");
#endif

	FILE * testsfp	=	fopen("isInnerTestServer","rb");
	if (testsfp)
	{
		l_bIsTestDB	=	TRUE;

		MessageBox(NULL,"for inner test server","WARNING!!",MB_OK);

		fclose(testsfp);testsfp = NULL;
	}

	//	필요한 정보를 읽어두고
	scp.Read(dCONFIGFILENAME);
	strcpy(mainPath,scp.getData("MAINDIR"));
	CreateDirectory(mainPath,NULL);
	sprintf(subpath,"%s/TOOLLOG/",mainPath);
	CreateDirectory(subpath,NULL);
	logger.initialize(subpath);

	//	시작전에 log파일 검사해서 수정할 내용들을 먼저 수정해라.
	hAcceptThread = hAdditionThread = NULL;
	for(int jj=0;jj<dMAX_WORKER_COUNT;jj++)	hWorkerThread[jj] = NULL;
	//	윈도우 끝나면 나머지 초기화 
	//	스택 할당하고...
	InitializeCriticalSection(&csRECVPacket);
	InitializeCriticalSection(&csSENDPacket);
	InitializeCriticalSection(&csRemoveUser);
	InitializeCriticalSection(&csLogFile);

	cLOG = new DebugTracer(DTType_ALL,"RedStone_LOGServer");

	if(!InitSocket())
	{
		_log("Error InitSocket \n");
		return;
	}
	_log("Start LOG Server ... day : %d - time : %d \n",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes() );
	DWORD Flags = 0;
	SOCKET		clientsock = INVALID_SOCKET;
	int index=0;
	while(1)
	{
		if(UserManager.Size()>=dLOG_MAX_USER_COUNT){	_log("UserManager.Size()>=dLOG_MAX_USER_COUNT");	Sleep(10);	continue;}
		if((clientsock = accept(lisSock,NULL,NULL)) != INVALID_SOCKET)
		{
			int buf=dMAX_PACKET_LENGTH;
			if(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_SNDBUF,(const char*)&buf,sizeof(buf)))
			{
				closesocket(clientsock);	clientsock = 0;	continue;
			}
			if(SOCKET_ERROR ==setsockopt(clientsock,SOL_SOCKET ,SO_RCVBUF,(const char*)&buf,sizeof(buf)))
			{
				closesocket(clientsock);	clientsock = 0;	continue;
			}
			LINGER	lingerStruct; lingerStruct.l_onoff = 1; lingerStruct.l_linger = 0; 
			if(SOCKET_ERROR ==setsockopt(clientsock, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct)))
			{
				closesocket(clientsock);	clientsock = 0;	continue;
			}
			index = UserManager.Add();
			if(index==-1 || index==0xffff)
			{
				_log("Fail Add User!!!");
				UserManager.Init();
				continue;
			}

			CClient *newUser = UserManager.GetClientPnt(index);
			newUser->Open();
			newUser->SetSocket(clientsock);
			newUser->SetSerial(index);

			_logc("[%d]Add User.. %d \n",cSRVUTIL::GetTimes(),UserManager.Size());
			if(!CreateIoCompletionPort((HANDLE)newUser->GetSocket(),g_hCompletionPort,(DWORD)index,0))
			{
				UserManager.Del(newUser->GetSerial());
				_log("Error CreateIoCompletionPort !! - %d",GetLastError());
				continue;
			}
			RecvPacket(index);
		}
		Sleep(1);
	}
	DeleteCriticalSection(&csRECVPacket);
	DeleteCriticalSection(&csSENDPacket);
	DeleteCriticalSection(&csRemoveUser);
	DeleteCriticalSection(&csLogFile);

	_log(" LOG Server is Exit \n");
}

BOOL InitSocket()
{
	SOCKADDR_IN serverAddress;
	WSADATA wsadata;
	if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{	_log("Fail WSAStartup !! \n");
		return FALSE;
	}

	if((lisSock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{	_log("Fail listen !!");
		return FALSE;
	}

	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(dTOOLLOGSERVER_PORT);

	_logc("[Port : %d] OPEN",dTOOLLOGSERVER_PORT);

	if(bind(lisSock,(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) == SOCKET_ERROR)
	{	_log("Fail bind !!\n");
		return FALSE;
	}

	listen(lisSock,5);

	if(!InitCompletionPort())
	{	_log("Fail Init IOCP Port !!");
		return FALSE;
	}

	if(!logServer.init(atoi(scp.getData("LOGPORT")),scp.getData("LOGIP"),"LOGSERVER",4))
	{
		_log("Can't connect to LOG SERVER");
		return FALSE;
	}

	hAdditionThread = (HANDLE)_beginthread(AdditionThread, 0, NULL); 
	if(hAdditionThread==NULL)
	{	_log("ERROR : addition thread create \n");
		return FALSE;
	}
	HANDLE hPressThread = (HANDLE)_beginthread(PressThread, 0, NULL); 
	if(hPressThread == NULL)
	{	_log("ERROR : Press thread create \n");
		return FALSE;
	}
	hRecvThread = (HANDLE)_beginthread(RecvThread,0,NULL);
	if(hRecvThread == NULL)
	{	_log("ERROR : Recv thread create \n");
		return FALSE;
	}

	return TRUE;
}

BOOL InitCompletionPort()
{
	SYSTEM_INFO sys_info;
	DWORD dwThreadNum;

	g_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);
	if(g_hCompletionPort==NULL)
	{
		_log("Fail IOCP Port Create !!");
		return FALSE;
	}

	GetSystemInfo(&sys_info);
	dwThreadNum = sys_info.dwNumberOfProcessors * 2 + 1;

	for(int i=0;i<(int)dwThreadNum;i++)
	{
		hWorkerThread[i] = (HANDLE)_beginthread(WorkerThread, 0,(LPVOID)i); 
		if(hWorkerThread[i]==NULL)
		{
			_log("Fail Worker [%d] Thread begin !!",i);
			return FALSE;
		}
	}
	return TRUE;
}

void WorkerThread(LPVOID lpParameter)
{
	LP_IO_DATA	lpOV;
	DWORD dwRead,dwIOCPKey;
	int	threadSerial = (int)lpParameter;

	_log("START WorkerThread[%d] ...",threadSerial);
	BOOL retVal = TRUE;
	while(g_bThreadLoop)
	{
		dwIOCPKey = 0xffffffff;
		retVal = GetQueuedCompletionStatus(g_hCompletionPort,&dwRead,&dwIOCPKey,(LPOVERLAPPED *)&lpOV,dwIOCPKey);
		if( retVal == TRUE && dwIOCPKey < dLOG_MAX_USER_COUNT && dwRead != 0 && lpOV != 0 )
		{
			CClient	* user = UserManager.GetClientPnt(dwIOCPKey);
			if(!user)
			{
				continue;
			}

			if(lpOV->IOState == 0)
			{
				user->AddRecvPacket(dwRead);
				RecvPacket(dwIOCPKey);
				ProcessRecvPacket(dwIOCPKey);
			}else if(lpOV->IOState == 1)
			{
				int	result_send = user->CompleteSendPacket(dwRead);
				if(result_send>0)	SendPacket_Add(dwIOCPKey);
				else				SendPacket(dwIOCPKey);
			}
		}else{
			if(dwRead == 0 )
			{
				if(dwIOCPKey >= dLOG_MAX_USER_COUNT)	continue;
				if(!lpOV)	_log("SERROR : getqueue... lpOV = NULL");

				CClient	* user = UserManager.GetClientPnt((WORD)dwIOCPKey);
				if(!user)	continue;
				DisUser(dwIOCPKey);
			}else{
				DWORD dwError = GetLastError();
				_log("SERROR GetQueue Error : %d ",dwError);
				if(dwError==ERROR_NETNAME_DELETED)	_log("dwError ==ERROR_NETNAME_DELETED");
				if(dwError != ERROR_IO_PENDING && dwError!=ERROR_OPERATION_ABORTED && dwIOCPKey < dGAME_MAX_USER_COUNT)
				{
					_log("%d Worker thread error GetQueueCompletionStatus... error [%d]",threadSerial,dwError);
					DisUser(dwIOCPKey);
				}
			}
		}
	}
	_log("END WorkerThread[%d] ...",threadSerial);
	return;
}

BOOL RecvPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())		return FALSE;
	DWORD	lpRecv;
	DWORD	Flags = 0;
	user->rOL.IOState = 0;

	if(WSARecv(user->GetSocket(), &user->rOL.buf ,1,&lpRecv,&Flags,&user->rOL.OL ,NULL) ==SOCKET_ERROR)
	{
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			DisUser(serial);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL SendPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);

	if	(!user || user->GetIsClose())
		return FALSE;

	WORD iSendSize = (WORD)user->sBuffer->GetData(user->sendpacket,0);
	if(!iSendSize)
	{
		return TRUE;
	}

	user->sOL.IOState = 1;
	DWORD	lpSend = 0;	
	WSABUF	buf;

	user->sendpacket_size = iSendSize;

	buf.len	=	user->sendpacket_size;
	buf.buf	=	user->sendpacket;

	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if	(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			_log("SEND ERROR ... ERROR %d",dwError);
			user->sBuffer->Next();
			DisUser(serial);
			return FALSE;
		}
	}

	return TRUE;
}

void	SendPacket_Add(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())	return;
	user->sOL.IOState = 1;
	DWORD	lpSend = 0;	WSABUF	buf;
	buf.len = user->sendpacket_size;
	buf.buf = user->sendpacket;

	if(WSASend(user->GetSocket(), &buf,1,&lpSend,0,&user->sOL.OL ,NULL)==SOCKET_ERROR)
	{	
		DWORD dwError = WSAGetLastError();
		if(dwError!=ERROR_IO_PENDING && dwError!=ERROR_SUCCESS)
		{
			_log("SEND_ADD ERROR ... ERROR %d",dwError);
			user->sBuffer->Next();
			DisUser(serial);
			return;
		}
	}
}

void	ProcessRecvPacket(int serial)
{
	CClient	* user = UserManager.GetClientPnt(serial);
	if(!user || user->GetIsClose())	return;
	WORD	type,recvsize;
	char recvpacket[dMAX_PACKET_LENGTH_SERVER];
	int resultGetRecvPacket=0;
	while(TRUE)
	{
		resultGetRecvPacket = user->GetRecvPacket(recvpacket);
		if(resultGetRecvPacket==0)	break;
		recvsize = *(WORD *)recvpacket;
		type = *(WORD *)(recvpacket+2);
		switch(type)
		{
		case ePACKET_LOGIN:
			{
				PACKET_LOGIN * rPacket;
				rPacket = (PACKET_LOGIN *)recvpacket;

				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);
				
				sprintf(common.strMsg," (%s)is log in.",rPacket->strIp);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);
			
				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_SENDNOTICE:
			{
				PACKET_SENDNOTICE * rPacket;
				rPacket = (PACKET_SENDNOTICE *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," serves a notice to [%s].",rPacket->strServerName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_KILLWORLD:
			{
				PACKET_KILLWORLD * rPacket;
				rPacket = (PACKET_KILLWORLD *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," closes World [%s]",rPacket->strServerName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_LOGINSERVER:
			{
				PACKET_LOGINSERVER * rPacket;
				rPacket = (PACKET_LOGINSERVER *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				if(rPacket->wKill)
					sprintf(common.strMsg," closes LoginServer");
				else
					sprintf(common.strMsg," opens LoginServer");
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}

		case ePACKET_SETBADUSER:
			{
				PACKET_SETBADUSER * rPacket;
				rPacket = (PACKET_SETBADUSER *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," intercepts [%s] until [%d] becaues of [%s]",rPacket->strUserId,rPacket->dwBlockDate,rPacket->strWhy);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_DELID:
			{
				PACKET_DELID * rPacket;
				rPacket = (PACKET_DELID *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," deletes id [%s] ",rPacket->strUserId);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_DELAVA:
			{
				PACKET_DELAVA * rPacket;
				rPacket = (PACKET_DELAVA *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," deletes [%s]'s [%s]'s avatar [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_RECOVERID:
			{
				PACKET_RECOVERID * rPacket;
				rPacket = (PACKET_RECOVERID *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," recovers id [%s] to [%s]",rPacket->strOldUserId,rPacket->strNewUserId);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_RECOVERAVA:
			{
				PACKET_RECOVERAVA * rPacket;
				rPacket = (PACKET_RECOVERAVA *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," recovers [%s]'s avatar [%s]'s [%s]->[%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strOldUserName,rPacket->strNewUserName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGENAME:
			{
				PACKET_CHANGENAME * rPacket;
				rPacket = (PACKET_CHANGENAME *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," [%s]'s [%s] avatar's name [%s] changes to [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strOldUserName,rPacket->strNewUserName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_SETOPERATER:
			{
				PACKET_SETOPERATER * rPacket;
				rPacket = (PACKET_SETOPERATER *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," [%s]'s avatar [%s] is set operater [%d] level",rPacket->strServerName,rPacket->strUserName,rPacket->wOper);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}

		case ePACKET_DATASAVE:
			{
				PACKET_DATASAVE * rPacket;
				rPacket = (PACKET_DATASAVE *)recvpacket;

				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," saves data [%s] [%s]'s [%s] ",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGELEVEL:
			{
				PACKET_CHANGELEVEL * rPacket;
				rPacket = (PACKET_CHANGELEVEL *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," changes level [%s] [%s]'s [%s] [%d -> %d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOldLev,rPacket->wNewLev);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGELASTVILLAGE:
			{
				PACKET_CHANGELASTVILLAGE * rPacket;
				rPacket = (PACKET_CHANGELASTVILLAGE *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," changes village [%s] [%s]'s [%s] [%d -> %d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOldVillage,rPacket->wNewVillage);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGELASTFIELD:
			{
				PACKET_CHANGELASTFIELD * rPacket;
				rPacket = (PACKET_CHANGELASTFIELD *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," changes field [%s] [%s]'s [%s] [%d -> %d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOldField,rPacket->wNewField);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}

		case ePACKET_INSERTITEMTOBANK:
			{
				PACKET_INSERTITEMTOBANK * rPacket;
				rPacket = (PACKET_INSERTITEMTOBANK *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				char	strCode[512];

				CItemDefine	*lpItem	=	&rPacket->item;
				
				sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
										lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
										lpItem->m_aOption[0],lpItem->m_aOption[1],
										lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
										lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
										lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
										lpItem->getBitFieldValue());

				sprintf(common.strMsg," inserts item to bank [%s] [%s] [%s]",rPacket->strServerName,rPacket->strUserId,strCode);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_REMOVEITEM_FROM_BANK	:
			{
				PACKET_REMOVEITEM_FROM_BANK * rPacket;
				rPacket = (PACKET_REMOVEITEM_FROM_BANK *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				char	strCode[512];

				CItemDefine	*lpItem	=	&rPacket->item;
				
				sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
										lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
										lpItem->m_aOption[0],lpItem->m_aOption[1],
										lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
										lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
										lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
										lpItem->getBitFieldValue());

				sprintf(common.strMsg," removes bank item [%s] [%s] [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,strCode);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_INSERTITEM:
			{
				PACKET_INSERTITEM * rPacket;
				rPacket = (PACKET_INSERTITEM *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				char	strCode[512];

				CItemDefine	*lpItem	=	&rPacket->item;
				
				sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
										lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
										lpItem->m_aOption[0],lpItem->m_aOption[1],
										lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
										lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
										lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
										lpItem->getBitFieldValue());

				sprintf(common.strMsg," inserts item to ava [%s] [%s]'s [%s] [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,strCode);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_REMOVEITEM:
			{
				PACKET_REMOVEITEM * rPacket;
				rPacket = (PACKET_REMOVEITEM *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				char	strCode[512];

				CItemDefine	*lpItem	=	&rPacket->item;
				
				sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
										lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
										lpItem->m_aOption[0],lpItem->m_aOption[1],
										lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
										lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
										lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
										lpItem->getBitFieldValue());

				sprintf(common.strMsg," removes item [%s] [%s]'s [%s] [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,strCode);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_ADDEXP:
			{
				PACKET_ADDEXP * rPacket;
				rPacket = (PACKET_ADDEXP *)recvpacket;

				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," adds exp [%s] [%s]'s [%s] %d",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->iAddExp);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}

		case ePACKET_ADDGOLD:
			{
				PACKET_ADDGOLD * rPacket;
				rPacket = (PACKET_ADDGOLD *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," adds gold [%s] [%s]'s [%s] %d Gold",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->dwAddGold);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_ADDGOLDTOBANK:
			{
				PACKET_ADDGOLDTOBANK * rPacket;
				rPacket = (PACKET_ADDGOLDTOBANK *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," adds gold to bank [%s] [%s]'s [%s] %d Gold",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->dwAddGold);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_REMOVEGOLDTOBANK:
			{
				PACKET_REMOVEGOLDTOBANK * rPacket;
				rPacket = (PACKET_REMOVEGOLDTOBANK *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," removes gold to bank [%s] [%s]'s [%s] %d Gold",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->dwRemoveGold);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_REMOVEGOLD:
			{
				PACKET_REMOVEGOLD * rPacket;
				rPacket = (PACKET_REMOVEGOLD *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," removes gold [%s] [%s]'s [%s] %d Gold",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->dwRemoveGold);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_REMOVEITEM_BUTTON:
			{
				PACKET_REMOVEITEM_BUTTON * rPacket;
				rPacket = (PACKET_REMOVEITEM_BUTTON *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," ");
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}

		case ePACKET_RECOVERITEM_BUTTON:
			{
				PACKET_RECOVERITEM_BUTTON * rPacket;
				rPacket = (PACKET_RECOVERITEM_BUTTON *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," ");
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_RESETSKILLPOINT:
			{
				PACKET_RESETSKILLPOINT * rPacket;
				rPacket = (PACKET_RESETSKILLPOINT *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," resets skill point [%s] [%s]'s [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_RESETSTATPOINT:
			{
				PACKET_RESETSTATPOINT * rPacket;
				rPacket = (PACKET_RESETSTATPOINT *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," resets stat point [%s] [%s]'s [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_MOVETO:
			{
				PACKET_MOVETO * rPacket;
				rPacket = (PACKET_MOVETO *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				char strTarget[32];
				switch(rPacket->wPoint)
				{
				case 0:
					sprintf(strTarget,"Last Village");
					break;
				case 1:
					sprintf(strTarget,"send prison");
					break;
				default:
					sprintf(strTarget,"gate of current field");
					break;
				}
				sprintf(common.strMsg,"moves to [%s] [%s]'s [%s] to [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,strTarget);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_DATACOPY:
			{
				PACKET_DATACOPY * rPacket;
				rPacket = (PACKET_DATACOPY *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," copy data [%s(%s)] -> [%s] [%s]'s [%s]",rPacket->strSourceId,rPacket->strSourceName,rPacket->strServerName,rPacket->strTargetId,rPacket->strTargetName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}

		case ePACKET_SAVE_BANK_DATA			:	//	은행 데이터 저장
			{
				PACKET_SAVE_BANK_DATA * rPacket;
				rPacket = (PACKET_SAVE_BANK_DATA *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," save bank data [%s]",rPacket->strId);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_FAILED_WORK				:	//	로그를 보낸후에 무슨 작업을 했는데: 그 작업이 실패 했다.(ex 세이브 로그 보내고 세이브 했는데 무슨 이유로 세이브 실패!!)
			{
				PACKET_FAILED_WORK * rPacket;
				rPacket = (PACKET_FAILED_WORK *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				strcpy(common.strMsg," failed work");
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}

		case ePACKET_RENEW_PLAYER_ITEM		:	//	플레이어 아이템 기간 연장
			{
				PACKET_RENEW_PLAYER_ITEM * rPacket;
				rPacket = (PACKET_RENEW_PLAYER_ITEM *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				char	strCode[512];

				CItemDefine	*lpItem	=	&rPacket->item;
				
				sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
										lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
										lpItem->m_aOption[0],lpItem->m_aOption[1],
										lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
										lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
										lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
										lpItem->getBitFieldValue());

				sprintf(common.strMsg," renew item period [%s] [%s]'s [%s] [%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,strCode);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_RENEW_BANK_ITEM			:	//	은행 아이템 기간 연장
			{
				PACKET_RENEW_BANK_ITEM * rPacket;
				rPacket = (PACKET_RENEW_BANK_ITEM *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				char	strCode[512];

				CItemDefine	*lpItem	=	&rPacket->item;
				
				sprintf(strCode,"[%d:%d] [%d,%d] [%d,%d] [%d:%d,%d] [%d:%d,%d] [%d:%d,%d] %d",
										lpItem->m_wBaseItem,lpItem->m_dwSerial,lpItem->m_bCount,lpItem->m_bDurability,
										lpItem->m_aOption[0],lpItem->m_aOption[1],
										lpItem->m_aPrefix[0].m_wPrefix,lpItem->m_aPrefix[0].m_aValue[0],lpItem->m_aPrefix[0].m_aValue[1],
										lpItem->m_aPrefix[1].m_wPrefix,lpItem->m_aPrefix[1].m_aValue[0],lpItem->m_aPrefix[1].m_aValue[1],
										lpItem->m_aPrefix[2].m_wPrefix,lpItem->m_aPrefix[2].m_aValue[0],lpItem->m_aPrefix[2].m_aValue[1],
										lpItem->getBitFieldValue());

				sprintf(common.strMsg," renew item period [%s] [%s] [%s]",rPacket->strServerName,rPacket->strUserId,strCode);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_WORK_MESSAGE			:
			{
				PACKET_WORK_MESSAGE * rPacket;
				rPacket = (PACKET_WORK_MESSAGE *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," [%s] [%s] [%s/%s]",rPacket->strMessage,rPacket->strServerName,rPacket->strUserId,rPacket->strUserName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_LOAD_PLAYER_DATA		:	//	계정 혹은 플레이어 데이터를 로딩 했다.
			{
				PACKET_LOAD_PLAYER_DATA * rPacket;
				rPacket = (PACKET_LOAD_PLAYER_DATA *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," load player data [%s] [%s/%s]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}

		case ePACKET_CHANGE_AVATAR_INDEX		:	//	아바타 인덱스를 바꿈
			{
				PACKET_CHANGE_AVATAR_INDEX * rPacket;
				rPacket = (PACKET_CHANGE_AVATAR_INDEX *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," change avatar index [%s] [%s/%s] [%d->%d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOld,rPacket->wNew);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGE_CURRENT_FIELD	:	//	현재 필드를 바꿈
			{
				PACKET_CHANGE_AVATAR_INDEX * rPacket;
				rPacket = (PACKET_CHANGE_AVATAR_INDEX *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," change current field [%s] [%s/%s] [%d->%d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOld,rPacket->wNew);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGE_SECRET_DUNGEON_PLAY_COUNT:	//	비밀 던젼 플레이 수를 바꿈
			{
				PACKET_CHANGE_AVATAR_INDEX * rPacket;
				rPacket = (PACKET_CHANGE_AVATAR_INDEX *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," change secret dungeon play count [%s] [%s/%s] [%d->%d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOld,rPacket->wNew);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGE_EXTRA_INVENTORY_SIZE	:	//	인벤토리 사이즈를 바꿈
			{
				PACKET_CHANGE_AVATAR_INDEX * rPacket;
				rPacket = (PACKET_CHANGE_AVATAR_INDEX *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," change extra inventory size [%s] [%s/%s] [%d->%d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOld,rPacket->wNew);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGE_PENELTY_TIME	:		//	페널티 시간을 바꿈
			{
				PACKET_CHANGE_AVATAR_INDEX * rPacket;
				rPacket = (PACKET_CHANGE_AVATAR_INDEX *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," change penelty time [%s] [%s/%s] [%d->%d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOld,rPacket->wNew);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
		case ePACKET_CHANGE_DEATH_PENELTY_TIME	:	//	죽음 페널티 시간을 바꿈
			{
				PACKET_CHANGE_AVATAR_INDEX * rPacket;
				rPacket = (PACKET_CHANGE_AVATAR_INDEX *)recvpacket;
				
				PACKET_COMMON	common;
				common.dwtime = cSRVUTIL::GetTimes();
				strcpy(common.strId,rPacket->strId);

				sprintf(common.strMsg," change death penelty time [%s] [%s/%s] [%d->%d]",rPacket->strServerName,rPacket->strUserId,rPacket->strUserName,rPacket->wOld,rPacket->wNew);
				printf("%d %s %s \n",common.dwtime,common.strId,common.strMsg);

				common.base.wSize = 28 + strlen(common.strMsg);
				common.base.wType = ePACKET_COMMON;
				logger.write((char *)&common);
				break;
			}
				
		case ePACKET_SEARCH:
			{
				stParam	param;
				param.serial = user->GetSerial();
				memcpy(&param.searchItem,recvpacket,sizeof(PACKET_SEARCH));

				printf("START SEARCH...");
				HANDLE hThread = (HANDLE)_beginthread(ProcessSearchThread, 0,(void *)&param); 
				if(hThread==NULL)
				{	
					//	실패 PACKET_COMMON에 쓰레드 생성 실패 했다고 해서 넘겨준다.
					SendErrorResult( user->GetSerial(), param.searchItem.strId, " 너무 많은 검색이 이루어지고 있습니다. 잠시후에 다시 시도해 주십시요. ");
				}
				break;
			}
		default:
			{	//	알지 못하는 패킷은 버린다.
				break;
			}
		}
		SendPacket(serial);
	}
}

void ProcessPressThread(LPVOID lpParameter)
{
	char	strPath[1024];
	DWORD	pressDay = cSRVUTIL::GetPrevTime();
	strcpy(strPath,logger.strPath);

	char	Backuppath[1024],Presspath[1024];
	sprintf(Backuppath,"%s\\backup",strPath);	CreateDirectory(Backuppath,NULL);
	sprintf(Presspath,"%s\\press",strPath);		CreateDirectory(Presspath,NULL);

	cSTRINGS * files = cFOLDER::GetFiles("*.log",strPath);
	if(!files)
	{
		_log("%d -> Not Found file for compress\n",cSRVUTIL::GetTimes());
		return;
	}
	char	compareDate[16];	sprintf(compareDate,"%d",pressDay);
	cSTRINGS * selectFiles = new cSTRINGS;
	{	//	해당 디렉토리에서 pressDay가 포함된 파일들을찾는다.
		for(int i =0; i < files->Count ; i++)
		{
			char * fn = files->GetStr(i);
			if(strstr(fn,compareDate))		
				selectFiles->Add(fn);
		}
	}
	if(selectFiles->Count == 0)
	{
		_log("%d -> Not Found file for compress\n",cSRVUTIL::GetTimes());
		return;
	}

	BOOL	isAddFiles = TRUE;
	{	//	한개이상일 경우 하나의 파일로 합친다.
		if(selectFiles->Count>1)
		{
			DWORD	allSize = 0;
			int i = 0;
			char	openFile[1024];
			for(i =0 ; i<selectFiles->Count ; i++ )
			{
				sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(i));
				FILE * fp = fopen(openFile,"r");
				if(!fp)	{	isAddFiles = FALSE;	break;	}
				allSize += filelength(fileno(fp));
				fclose(fp);	fp = NULL;
			}
			char	* allData = new char[allSize];
			DWORD	dwProcessLength = 0;
			for(i =0 ; i<selectFiles->Count ; i++ )
			{
				Sleep(1);
				sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(i));
				_log("FILE NAME.. [%s]",openFile);
				FILE * fp = fopen(openFile,"r");
				if(!fp)	{	isAddFiles = FALSE;	break;	}
				DWORD	_size = filelength(fileno(fp));
				fread(allData+dwProcessLength,_size,1,fp);
				dwProcessLength += _size;
				fclose(fp);	fp = NULL;
			}
			if(allSize != dwProcessLength)	{ isAddFiles = FALSE; }
			else
			{
				//	기존 이름을 다 바꾼다.
				sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(0));
#ifdef _FOR_KOREA
				_log("기존 이름을 다 바꾼다. [%s]",openFile);
#endif
				FILE * fp = fopen(openFile,"w");
				if(!fp)	{	isAddFiles = FALSE;	}
				else
				{
					fwrite(allData,allSize,1,fp);
					fclose(fp);
				}
			}
		}
	}
	//	해당파일을 압축한다.
	if(selectFiles->Count>0)
	{
		char	openFile[1024],newFile[1024];
		sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(0));
		sprintf(newFile,"%s\\Backup\\%s",strPath,selectFiles->GetStr(0));
		MoveFile(openFile,newFile);
		_log(" FILE MOVE : [%s] -> [%s]",openFile,newFile);
		DeleteFile(openFile);
	}else{
		_log(" ERROR file count = %d",selectFiles->Count);
	}
	return; 
}

void AdditionThread(LPVOID lpParameter)
{
	_log("START AdditionThread ...");
	static int tmpCount = 0;
	DWORD	dwSSCheckTime = timeGetTime();
	cHardwareInfo	hwInfo;

	while(g_bThreadLoop)
	{	
		int a = UserManager.Size();
		for(int z = 0;z<a;z++)
		{
			CClient * user = UserManager.GetClientPnt(z);
			if(!user || user->GetIsClose())		continue;
			SendPacket(user->GetSerial());
			a = UserManager.Size();
		}
#ifdef dUSE_SERVERSTATUS_REPORT
		if(timeGetTime() - dwSSCheckTime >= 30000)
		{	//	30초에 한번씩 DB에 현재 정보를 갱신해준다.
			if( g_SS.isReady() )
			{
				g_SS.update(cServerStatus::eERROR_TYPE_SERVERSTATUS,1,"No problem");
				dwSSCheckTime = timeGetTime();
			}

			int error = hwInfo.checkHW();
			char strText[512];
			switch(error)
			{
			case eHW_WARNING_OVER_CPU_USAGE:
				{
					sprintf(strText,"eHW_WARNING_OVER_CPU_USAGE %d%% ",hwInfo.m_cCPUInfo.m_iTotalUsage);
					g_SS.update( eHW_WARNING_OVER_CPU_USAGE , hwInfo.m_cCPUInfo.m_iTotalUsage , strText);
				}
				break;
			case eHW_WARNING_OVER_MEMORY_USAGE:
				{
					sprintf(strText,"eHW_WARNING_OVER_MEMORY_USAGE %d/%d ",hwInfo.m_cMemoryInfo.m_lFreeSize,hwInfo.m_cMemoryInfo.m_lFreeSize);
					g_SS.update( eHW_WARNING_OVER_MEMORY_USAGE , 0 , strText);
				}
				break;
			case eHW_WARNING_HAVENOT_FREESIZE:
				{
					sprintf(strText,"eHW_WARNING_HAVENOT_FREESIZE [%s] %u/%u  ",hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_strHddName , hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_iFreeMB, hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_iTotalMB);
					g_SS.update( eHW_WARNING_HAVENOT_FREESIZE , 0 , strText);
				}
				break;
			}
		}
#endif

		Sleep(1);
	}
	_log("END AdditionThread ...");
	return ; 
}

//////////////////////////////////////////////
//	method for Command
//////////////////////////////////////////////
BOOL	ProcessCommand(char command)
{
	switch(command)
	{
	case 'h':	//	help
		{
			printf("┌─────────────────────┐\n");
			printf("│h : View Command List                     │\n");
			printf("│q : Exit Server                           │\n");
			printf("└─────────────────────┘\n");
			printf("\n\n\n");
			break;
		}
	case 'q':
		{
			char quit;
			printf(" Quit ? (y/n) ");
			cin >> quit;
			if(quit=='y' || quit=='Y')
				return FALSE;//	서버를 종료한다.
			break;
		}
	}
	return TRUE;
}

//////////////////////////////////////////////
//	method for packet method
//////////////////////////////////////////////
void	DisUser(int serial)
{
	if(serial<0 || serial>=dLOG_MAX_USER_COUNT)	return;
	EnterCriticalSection(&csRemoveUser);
	CClient * user  = UserManager.GetClientPnt(serial);
	if(!user){	LeaveCriticalSection(&csRemoveUser);	return;	}
	if(user->GetIsUsed())
	{
		user->isUsed = FALSE;
	}
	_log("(%s)%d is Exit - %d\n",user->GetID(),serial);
	UserManager.Del(serial);
	LeaveCriticalSection(&csRemoveUser);
}

void ProcessSearchThread(LPVOID lpParameter)
{
	stParam	* _param = (stParam * )lpParameter;
	printf(" START Search Thread... \n");
	//	조건에 문제가 없는지 확인한다.
	int	iCurYY = cSRVUTIL::GetYear()+1900;
	int	iCurMM = cSRVUTIL::GetMonth()+1;
	int iCurDD = cSRVUTIL::GetDay();
	int		iTestDate = _param->searchItem._yy*10000 + _param->searchItem._mm*100 + _param->searchItem._dd;
	int		iEndDate = _param->searchItem.yy*10000 + _param->searchItem.mm*100 + _param->searchItem.dd;
	char	fn[512];
	int		iCheckDataCount = 0;
	PACKET_COMMON		packet;

	//	백업중에는 검사 못하게 하고..
	int	iBackupCheckTime = cSRVUTIL::GetTimes();
	if( (iBackupCheckTime>=235500 && iBackupCheckTime<235959) || (iBackupCheckTime>=0 && iBackupCheckTime<003000))
	{
		SendErrorResult( _param->serial,_param->searchItem.strId, " Don't search. Now I work on backup.\n");
		return;
	}

	//////////////////////////////////////////////////
	//	검색 시작일이 미래라면 검색할 수 없다.
	if(_param->searchItem.yy > iCurYY)
	{
		SendErrorResult( _param->serial,_param->searchItem.strId, " 0 The search Items are incorrect. Correct the errors and try again please. \n");;
		return ;
	}
	if(_param->searchItem.yy == iCurYY && _param->searchItem.mm > iCurMM )
	{
		SendErrorResult( _param->serial,_param->searchItem.strId, " 1 The search Items are incorrect. Correct the errors and try again please. \n");
		return;
	}
	if(_param->searchItem.yy == iCurYY && _param->searchItem.mm == iCurMM && _param->searchItem.dd > iCurDD)
	{
		SendErrorResult( _param->serial,_param->searchItem.strId, " 2 The search Items are incorrect. Correct the errors and try again please. \n");
		return;
	}
	if(_param->searchItem.yy == iCurYY && _param->searchItem.mm == iCurMM && _param->searchItem.dd == iCurDD && _param->searchItem.hh > cSRVUTIL::GetHour())
	{
		SendErrorResult( _param->serial,_param->searchItem.strId, " 3 The search Items are incorrect. Correct the errors and try again please. \n");
		return;
	}
	//	검색 시작일이 미래라면 검색할 수 없다.
	//////////////////////////////////////////////////

	//	요일을 기준으로 파일을 하나씩 열어서 검사하고 조건이 만족했으면 바로 보내준다.
	//	당일 파일은 현재 사용중이므로 신청한 시간의 10분 이전까지만 read해서 검사한다.
	while(TRUE)
	{
		Sleep(1);
		DWORD	filesize = 0,curfilepos = 0;
		int		iSize =0, iType = 0; 
		int		iCheckHour = 0 , iCheckType = 0;
		if(_param->searchItem._yy == _param->searchItem.yy && _param->searchItem._mm == _param->searchItem.mm && _param->searchItem._dd != _param->searchItem.dd)
			iCheckHour = 1;

		if(iTestDate<cSRVUTIL::GetTime())
			sprintf(fn,"%sBackup\\%d.log",subpath,iTestDate);
		else
			sprintf(fn,"%s%d.log",subpath,iTestDate);

		printf("Searching files ~... [%s]\n",fn);
		FILE * fp = fopen(fn,"r");
		if(!fp)
		{
			printf("Not found the file [%s]\n",fn);
			goto GOTO_SEARCH_NEXTDATE;
		}

		filesize = filelength(fileno(fp));
		curfilepos  = 0;
		if(iCheckHour)
		{
			if((iTestDate%10000)/100 == _param->searchItem._mm && iTestDate%100 == _param->searchItem._dd)
			{
				//	검색조건의 시작일자와 동일하다면..
				iCheckType = 1;
			}else
			if((iTestDate%10000)/100 == _param->searchItem.mm && iTestDate%100 == _param->searchItem.dd)
			{
				//	검색조건의 마지막 일자와 동일하다면..
				iCheckType = 2;
			}else
			{
				//	시간 검사는 필요없다.
				iCheckType = 0;	//	시간 검사 필요없음.
			}
		}else
		{
			//	금일 데이터만 검색하면 된다.
			iCheckType = 3;
		}
		//	해당 파일을 하나씩 읽어서 조건에 맞으면 하나씩 보낸다.
		try{
			while(curfilepos<=filesize-4)
			{
				Sleep(1);
				memset(&packet,0,sizeof(PACKET_COMMON));
				if(!fread(&packet.base.wSize,sizeof(WORD),1,fp))	break;
				fread(&packet.base.wType,sizeof(WORD),1,fp);
				fread((char *)&packet+4,packet.base.wSize-4,1,fp);
				packet.strMsg[511] = 0;
				if(packet.base.wSize>sizeof(ALL_TOOLLOGMSG))	
				{
#ifdef _FOR_KOREA
					_log("최대 사이즈를 넘어선 잘못된 데이타가 존재합니다. [%s]",fn);
#endif
					break;
				}
				if(iCheckType>=2 && _param->searchItem._hh>=(iTestDate%100))
				{
					_log("  ");
					break;
				}
				BOOL	isSkip = 0;
				switch(iCheckType)
				{
				case 1:
					{
						//	검색일자의 시작일자일 경우 지정된 시간보다 적으면 무시~
						if((packet.dwtime/10000) < _param->searchItem._hh)
							isSkip =1;
						break;
					}
				case 2:
					{
						if(_param->searchItem._hh<(packet.dwtime/10000))
							isSkip  = 2;
						break;
					}
				case 3:
					{
						if(_param->searchItem._hh>(packet.dwtime/10000))
							isSkip = 1;
						if( _param->searchItem._hh<(packet.dwtime/10000))
							isSkip = 2;
						break;
					}
				}
				if(isSkip == 1)		continue;
				if(isSkip == 2)		break;
				if(strcmp(_param->searchItem.strId,"")!=0)
				{
					if(stricmp(_param->searchItem.strId,packet.strId)!=0)
					{
						continue;
					}
				}
				{
					CClient * user = UserManager.GetClientPnt(_param->serial);
					if(user)
						user->AddSendPacket((char *)&packet,packet.base.wSize);
				}
//				printf("SEND PACKET : [%d %s %s]\n",packet.dwtime,packet.strId,packet.strMsg);
				iCheckDataCount++;
				Sleep(1);
			}
		}catch(...)
		{
			//	읽기에 문제 발생.. 종료 메세지 보내고 끝낼것..
			SendErrorResult(_param->serial,_param->searchItem.strId,"Read Error\n");
			fclose(fp);
			return;
		}
		fclose(fp);

GOTO_SEARCH_NEXTDATE:
		Sleep(1);
		iTestDate = cSRVUTIL::GetNextTime(iTestDate);
		if(iEndDate<iTestDate)
			break;
		continue;
	}
	//	다 찾아서 보냈으면, 끝났다고 표시해서 한번 보낼것!!
	PACKET_COMMON	rIncorrectSearchItem;
	rIncorrectSearchItem.dwtime = cSRVUTIL::GetTimes();
	strcpy(rIncorrectSearchItem.strId,_param->searchItem.strId);
	rIncorrectSearchItem.dwtime = 0xffffffff;	//	다 찾았다.
	sprintf(rIncorrectSearchItem.strMsg," Found [%d] items!!",iCheckDataCount);
	rIncorrectSearchItem.base.wSize = 28 + strlen(rIncorrectSearchItem.strMsg);
	rIncorrectSearchItem.base.wType = ePACKET_COMMON;
	{
		CClient * user = UserManager.GetClientPnt(_param->serial);
		if(user)
			user->AddSendPacket((char *)&rIncorrectSearchItem,rIncorrectSearchItem.base.wSize);
	}
}

void __cdecl SendErrorResult(DWORD _dwSerial,char * _strId, char * msg,...)
{
	TCHAR buffer[256];
	va_list args;

	va_start( args, msg );
	_vstprintf( buffer, msg, args );
	va_end( args );

	PACKET_COMMON	rIncorrectSearchItem;
	rIncorrectSearchItem.dwtime = cSRVUTIL::GetTimes();
	strcpy(rIncorrectSearchItem.strId,_strId);
	rIncorrectSearchItem.dwtime = 0xfffffffe;
	strcpy(rIncorrectSearchItem.strMsg,buffer);
	rIncorrectSearchItem.base.wSize = 28 + strlen(rIncorrectSearchItem.strMsg);
	rIncorrectSearchItem.base.wType = ePACKET_COMMON;

	{
		CClient * user = UserManager.GetClientPnt(_dwSerial);
		if(user)
			user->AddSendPacket((char *)&rIncorrectSearchItem,rIncorrectSearchItem.base.wSize);
	}

	printf("SEND ERROR RESULT : [%s - %s]",_strId,buffer);
}


void PressThread(LPVOID lpParameter)
{
	_log("START PressThread ...");

	BOOL	isSaveTime = FALSE;
	while(g_bThreadLoop)
	{	
		Sleep(1);

		if(!isSaveTime && cSRVUTIL::GetHour()!=0)
			isSaveTime = TRUE;
		else if(cSRVUTIL::GetHour()==0 && isSaveTime)
		{
			logger.SetFileHandle();

			HANDLE hProcessPress = NULL;
			DWORD	strtime = timeGetTime();
			printf(" START Press... \n");
			hProcessPress = (HANDLE)_beginthread(ProcessPressThread, 0, NULL); 
			if(hProcessPress == NULL)
			{
				printf("ERROR : ProcessPress thread 생성 실패.. !!\n 지현이 바보~~\n");
				continue;
			}
			isSaveTime = FALSE;
			WaitForSingleObject(hProcessPress,INFINITE);
			DWORD	endTime = timeGetTime();
			_log(" ProcessPress time = %d.%d Sec\n",(endTime - strtime)/1000,(endTime - strtime)%1000);
		}
	}
}