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

#include "cSOCKET.h"
#include "Client.h"
#include "cScript.h"
#include "cSRVUTIL.h"
#include "Logger.h"
#include "Hardwareinfo.h"
#include "reg_common.h"
#include "inputConsoleCommand.h"
#include "../buildVersion.h"	//	이건 공유 안하고 쓴다.
							//	없으면 파일을 생성한다.(공유 하지 말것!!)
							//	안에는
							//	#pragma	once
							//	#define	dBUILDER_NAME	"young"
							//	와 같이 선언 되어 있다. 뒤에 이름은 프로그래머 이름을 적을것


//#define dUSE_SERVERSTATUS_REPORT

#ifdef _FOR_THAI
#undef dUSE_SERVERSTATUS_REPORT
#endif

#ifdef dUSE_SERVERSTATUS_REPORT
	#include "cServerStatus.h"
#endif

#define dCONFIGFILENAME				"config-log.txt"
#define dMAXSERVERCOUNT				12

typedef struct 
{
	char	name[32];
	char	serverCount;
	char	IPs[dMAXSERVERCOUNT][16];
}_ServerInfo;

char			strToolLogServerIp[16];	//	TOOLLOGSERVERIP
int				svrCount;
_ServerInfo		svrInfo[dMAXSERVERCOUNT];
static int		socketcount=0;
static char		mainPath[512];
static char		configPath[1024];

CRITICAL_SECTION csRECVPacket;
CRITICAL_SECTION csSENDPacket;
CRITICAL_SECTION csRemoveUser;
CRITICAL_SECTION csLogFile;

static DWORD	checkTime = 0;
static BOOL		isFirst	=TRUE;
static BOOL		isReady =FALSE;
static BOOL		g_bThreadLoop = TRUE;
static DWORD	dwPacketCount = 0;
HANDLE hCommandThread,hAcceptThread,hAdditionThread,hWorkerThread[dMAX_WORKER_COUNT];
HANDLE			hPressThread;

HANDLE			g_hCompletionPort;	//	IOCP에 사용되는 포트
SOCKET			lisSock;			//	listen 소켓

int			nTimer;
cScript		scp;

CClientManager & UserManager  = CClientManager::GetInstance();

BOOL InitSocket();
BOOL InitCompletionPort();		//	IOCP 초기화
void WorkerThread(LPVOID lpParameter); 
void AdditionThread(LPVOID lpParameter); 
void PressThread(LPVOID lpParameter);
void ProcessPressThread(LPVOID lpParameter);
void CommandThread(LPVOID lpParameter);

BOOL RecvPacket(int serial);
BOOL SendPacket(int serial);
void SendPacket_Add(int serial);

void	ProcessRecvPacket(int serial);
BOOL	ProcessCommand(char command);

//	내부 테스트를 위한 
BOOL	l_bIsTestDB		=	FALSE;


HANDLE			hRecvThread;
void RecvThread(LPVOID lpParameter);

//	method for packet send
void	DisUser(int serial);
//	end define

//////////////////////////////////////
//	기타 기능을 위한 플래그 
BOOL	isSend_UserCount	=	FALSE;
BOOL	l_bIsActiveMainLoop			=	TRUE;

DebugTracer		* cLOG;

Logger	*	l_pLogger;		//	서버별로 각각 하나씩.. 로그인 서버의 순서대로...0번은 테스트 서버..

void
RecvThread(LPVOID lpParameter)
{
	if(!l_bIsTestDB)
	{
/*		while(g_bThreadLoop)
		{
			Sleep(1);
			for(int count=0;count<dLOG_MAX_USER_COUNT;count++)
			{	
				CClient * user = UserManager.GetClientPnt(count,FALSE);
				if(!user || user->GetSerial()==0xffff)	continue;
				ProcessRecvPacket(user->GetSerial());
			}
		}
*/	}
	_log("End RecvThread... ");
	return;
}
void main()
{
	{
		char	strTitle[512];

#ifdef	_FOR_KOREA
		sprintf(strTitle,"RS Log server for korea. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_JAPAN
		sprintf(strTitle,"RS Log server for japan. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_THAI
		sprintf(strTitle,"RS Log server for thai. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#elif	_FOR_USA
		sprintf(strTitle,"RS Log server for USA. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#else
		sprintf(strTitle,"RS Log server. build time %s %s by %s",__DATE__,__TIME__,dBUILDER_NAME);
#endif

		SetConsoleTitle(strTitle);

		WriteServerInfoToRegistry(eST_LOG,"Log Server");
	}

	//	필요한 정보를 읽어두고
	GetCurrentDirectory(1024,configPath);

	sprintf(configPath,"%s/%s",configPath,dCONFIGFILENAME);
	printf("Read Scp : [%s]",configPath);
	scp.Read(configPath);
	char	mainPath[128];
	strcpy(mainPath,scp.getData("MAINDIR"));
	strcpy(strToolLogServerIp,scp.getData("TOOLLOGSERVERIP"));
	CreateDirectory(mainPath,NULL);
	svrCount = atoi(scp.getData("SERVERCOUNT"));

	l_pLogger = new Logger[dMAXSERVERCOUNT];

	for(int tc = 0;tc<svrCount;tc++)
	{
		char subpath[128];
		strcpy(svrInfo[tc].name,scp.getData("SERVERNAME",tc+1));
		svrInfo[tc].serverCount = atoi(scp.getData("SERVERGCOUNT",tc+1));
		sprintf(subpath,"%s/%s",mainPath,svrInfo[tc].name);
		CreateDirectory(subpath,NULL);
		l_pLogger[tc].initialize(subpath);

		char subName[20];
		sprintf(subName,"SERVERIP%02d",tc);

		for	(int gc = 0;gc<svrInfo[tc].serverCount;gc++)
			strcpy(svrInfo[tc].IPs[gc],scp.getData(subName,gc+1));
	}

	//	시작전에 log파일 검사해서 수정할 내용들을 먼저 수정해라.
	hAcceptThread = hAdditionThread = hPressThread = NULL;

	for	(int jj=0;jj<dMAX_WORKER_COUNT;jj++)
		hWorkerThread[jj] = NULL;

	hCommandThread = (HANDLE)_beginthread(CommandThread, 0, NULL); 

	if	(hCommandThread==NULL)
	{
		_log("Fail Create CommandThread");
		return;
	}

	//	윈도우 끝나면 나머지 초기화 
	//	스택 할당하고...
	InitializeCriticalSection(&csRECVPacket);
	InitializeCriticalSection(&csSENDPacket);
	InitializeCriticalSection(&csRemoveUser);
	InitializeCriticalSection(&csLogFile);

//	strcpy(strAvatarDBIp,scp.getData("IP"));

#ifdef dUSE_SERVERSTATUS_REPORT
// 	if(!g_SS.init("Log", "Server"))
// 	{
// 		MessageBox(NULL,"ERROR IN SS","WARNING!!",MB_OK);
// 		return;
// 	}
#endif

	cLOG = new DebugTracer(DTType_ALL,"RedStone_LOGServer");

	if	(!InitSocket())
	{
		_log("Error InitSocket \n");
		return;
	}

	_log("Start LOG Server ... day : %d - time : %d \n",cSRVUTIL::GetTime(),cSRVUTIL::GetTimes() );
	DWORD Flags = 0;
	SOCKET		clientsock = INVALID_SOCKET;
	int index=0;

	WriteServerStatusToRegistry(eST_LOG,eSERVER_STATUS_ON);

	while(l_bIsActiveMainLoop)
	{
		if	(UserManager.Size()>=dLOG_MAX_USER_COUNT)
		{	
			_log("UserManager.Size()>=dLOG_MAX_USER_COUNT");	
			Sleep(10);	
			continue;
		}

		if	((clientsock = accept(lisSock,NULL,NULL)) != INVALID_SOCKET)
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

			SOCKADDR_IN	saddr;		
			int tmp;
#ifdef _FOR_KOREA
 			char	_ip[16];
 			memset(&saddr,0,sizeof(saddr));
 			tmp = sizeof(sockaddr);
 			getpeername(clientsock,(SOCKADDR *)&saddr,&tmp);
			strcpy(_ip,inet_ntoa(saddr.sin_addr));
// // 			if(strncmp(_ip,"211.189",7)!=0 && strncmp(_ip,"211.235",11)!=0 && strncmp(_ip,"172.31.6",8)!=0 && strncmp(_ip,"58.150.56",8)!=0)
// // 			{
// // 				_log("엉뚱한 SERVER 출현 : %s",_ip);
// // 				closesocket(clientsock);	
// // 				clientsock = 0;	
// // 				continue;
// // 			}
#endif

			index = UserManager.Add();

			if	(index==-1)
			{
				_log("Fail Add User!!!");
				continue;
			}

			CClient *newUser = UserManager.GetClientPnt(index);

			newUser->Open();
			newUser->SetSocket(clientsock);
			newUser->SetSerial(index);

			memset(&saddr,0,sizeof(saddr));
			tmp = sizeof(sockaddr);
			getpeername(clientsock,(SOCKADDR *)&saddr,&tmp);
			char ip[16];
			strcpy(ip,inet_ntoa(saddr.sin_addr));

			newUser->wServerIndex = 0;

			if	(stricmp(ip,strToolLogServerIp)==0)
				newUser->wServerIndex = 0;
			else
			{
				for	(tc = 0;tc<svrCount;tc++)
				{
					for	(int gc = 0;gc<svrInfo[tc].serverCount;gc++)
					{
						if	(stricmp(ip,svrInfo[tc].IPs[gc])==0)
						{
							newUser->wServerIndex = tc;
							break;
						}
					}
				}
			}

			_logc("[%d]add user.. %d \n",cSRVUTIL::GetTimes(),UserManager.Size());

			if	(!CreateIoCompletionPort((HANDLE)newUser->GetSocket(),g_hCompletionPort,(DWORD)index,0))
			{
				UserManager.Del(newUser->GetSerial());
				_log("Error CreateIoCompletionPort !! - %d",GetLastError());
				continue;
			}

			RecvPacket(index);
		}
		Sleep(1);
	}

	//	모든 로그 닫는다.

	DeleteCriticalSection(&csRECVPacket);
	DeleteCriticalSection(&csSENDPacket);
	DeleteCriticalSection(&csRemoveUser);
	DeleteCriticalSection(&csLogFile);

	delete [] l_pLogger;

	WriteServerStatusToRegistry(eST_LOG,eSERVER_STATUS_OFF);

	_log(" LOG Server is Exit! \n");
}

BOOL InitSocket()
{
	SOCKADDR_IN serverAddress;
	WSADATA wsadata;

	if	(WSAStartup(MAKEWORD(2,2),&wsadata) != 0)
	{
		_log("Fail WSAStartup !! \n");
		return FALSE;
	}

	if	((lisSock = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{	
		_log("Fail listen !!");
		return FALSE;
	}

	memset(&serverAddress,0,sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(dLOGSERVER_PORT);

	if	(bind(lisSock,(LPSOCKADDR)&serverAddress,sizeof(serverAddress)) == SOCKET_ERROR)
	{	
		_log("Fail bind !!\n");
		return FALSE;
	}

	listen(lisSock,5);

	if(!InitCompletionPort())
	{	_log("Fail Init IOCP Port !!");
		return FALSE;
	}
	hAdditionThread = (HANDLE)_beginthread(AdditionThread, 0, NULL); 
	if(hAdditionThread==NULL)
	{	_log("ERROR : addition thread create \n");
		return FALSE;
	}

	hPressThread = (HANDLE)_beginthread(PressThread, 0, NULL); 
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
	if (dwThreadNum > dMAX_WORKER_COUNT)
		dwThreadNum = dMAX_WORKER_COUNT;

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
				continue;
			
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
			if(dwIOCPKey >= dLOG_MAX_USER_COUNT)	continue;
			if(!lpOV)	_log("SERROR : getqueue... lpOV = NULL");

			if(dwRead == 0 )
			{
				CClient	* user = UserManager.GetClientPnt(dwIOCPKey);
				if(!user)
				{
					_log("SERROR : getqueue... user==NULL || !user->GetIsUsed()");
					continue;	
				}
				user->PreClose();
				DisUser(dwIOCPKey);
			}
			else
			{
				DWORD dwError = GetLastError();
				_log("SERROR GetQueue Error : %d ",dwError);
				if(dwError==ERROR_NETNAME_DELETED)	_log("dwError ==ERROR_NETNAME_DELETED");
				if(dwError != ERROR_IO_PENDING && dwError!=ERROR_OPERATION_ABORTED && dwIOCPKey < dLOG_MAX_USER_COUNT)
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

	if	(!user || user->GetIsClose())
		return FALSE;

	DWORD	lpRecv;
	DWORD	Flags = 0;
	user->rOL.IOState = 0;

	if	(WSARecv(user->GetSocket(), &user->rOL.buf ,1,&lpRecv,&Flags,&user->rOL.OL ,NULL) ==SOCKET_ERROR)
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

	if	(!user || user->GetIsClose())
		return;

	WORD	type,recvsize;
	char	recvpacket[dMAX_PACKET_LENGTH_SERVER];
	int		resultGetRecvPacket=0;

	while(TRUE)
	{
		resultGetRecvPacket = user->GetRecvPacket(recvpacket);

		if	(resultGetRecvPacket==0)
			break;

		recvsize	=	*(WORD *)recvpacket;
		type		=	*(WORD *)(recvpacket+2);

		if	(type == dCM_COMPRESS)
		{
			CM_COMPRESS	comp;
			memcpy(&comp,recvpacket,recvsize);
			cSRVUTIL::Uncompress((unsigned char *)comp.data,(unsigned char *)recvpacket,comp.wSize - sizeof(WORD)*3,comp.wSourceSize);
			recvsize = *(WORD *)recvpacket;
			type = *(WORD *)(recvpacket+2);
		}

		switch(type)
		{
		case dLOG_FROMTOOLLOG:	//	로그툴에서 전달된 내용이다. 따로 처리하자.
			{
				LOG_FROMTOOLLOG		packet;
				memset(&packet,0,sizeof(LOG_FROMTOOLLOG));
				memcpy(&packet,recvpacket,recvsize);

				l_pLogger[packet.wServerIndex].write((char *)&packet.binData);
			}
			break;

		case dLOG_PACKAGE:
			{
				LOG_PACKAGE		packet;
				memset(&packet,0,sizeof(LOG_PACKAGE));
				memcpy(&packet,recvpacket,recvsize);
				
				int pos = 0;
				WORD	s=0,t=0;

				while(pos < packet.base.wSize-4)
				{
					memcpy(&s,packet.package+pos,sizeof(WORD));
					memcpy(&t,packet.package+pos+2,sizeof(WORD));
					
					if	(t<dLOG_BEGIN || t>dLOG_AVA_END_LOG)
					{
						_log("ERROR PACKAGE : pos = %d, size = %d, type = %d",pos,s,t);
						break;
					}

					if	(s==52428)
					{
						_log("ERROR PACKAGE : pos = %d, size = %d, type = %d",pos,s,t);
						break;
					}

					if (t	==	dLOG_ITEM_DROP					||
						t	==	dLOG_ITEM_PICK					||//	87
						t	==	dLOG_ITEM_SELL					||//	87
						t	==	dLOG_ITEM_DEL					||//	87
						t	==	dLOG_ITEM_CREATE_PREMIUM_ITEM	||
						t	==	dLOG_CREATE_ITEM				||
						t	==	dLOG_USE_ITEM					||
						t	==	dLOG_EXPIRED_ITEM				||
						t	==	dLOG_DOUBLE_ITEM				||
						t	==	dLOG_ENCHANT_ITEM				)
					{
						LOG_ITEM_DROP	*lpPacket	=	(LOG_ITEM_DROP *)(packet.package+pos);

						if	(lpPacket->cItem.m_wBaseItem	>=	8000)
							_log("ERROR PACKAGE : pos = %d, size = %d, type = %d - incorrect item index",pos,s,t);
					}

					l_pLogger[user->wServerIndex].write(packet.package+pos);	
					pos	+=	s;
					dwPacketCount++;

					printf(" Current Packet Count = %d , size = %d \n",dwPacketCount,s);
				}
				break;
			}

		case dLOG_GUILD_WORK					://	길드 관련 로그
		case dLOG_GUILD_BATTLE_WORK				://	길드전 관련 로그
		case dLOG_ITEM_DROP						://	87
		case dLOG_ITEM_PICK						://	87
		case dLOG_ITEM_SELL						://	87
		case dLOG_ITEM_DEL						://	87
		case dLOG_ITEM_TRADE					://	921
		case dLOG_ITEM_BANK_IN					:
		case dLOG_ITEM_BANK_OUT					:
		case dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	:
		case dLOG_ITEM_CREATE_PREMIUM_ITEM		:
		case dLOG_CREATE_ITEM					:
		case dLOG_USE_ITEM						:
		case dLOG_EXPIRED_ITEM					:
		case dLOG_DOUBLE_ITEM					:
		case dLOG_ENCHANT_ITEM					:
		case dLOG_EVENT							:
		case dLOG_PITCHMAN_SHOP_TRADE			:
		case dLOG_AVA_CREATE					:	//	63
		case dLOG_AVA_DELETE					:	//	63
		case dLOG_AVA_LOGIN						:	//	63
		case dLOG_AVA_LOGOUT					:	//	63
		case dLOG_AVA_SAVE_LOG					:
		case dLOG_PET_WORK						:
		case dLOG_SYSTEM						:
			{	//	알지 못하는 패킷은 버린다.
				l_pLogger[user->wServerIndex].write(recvpacket);
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
	//월드의 갯수만큼 처리한다.
	for (int _iloop = 0;_iloop<svrCount;_iloop++)
	{
		char	strPath[1024];

		DWORD	pressDay = cSRVUTIL::GetPrevTime();
		strcpy(strPath,l_pLogger[_iloop].m_strLogPath);
		
		char	Presspath[1024];
		sprintf(Presspath,"%s\\press",strPath);
		CreateDirectory(Presspath,NULL);

		//현재 폴더에서 확장자가 log인 파일 모두를 찾아낸다.
		cSTRINGS * files	=	cFOLDER::GetFiles("*.log",strPath);

		if	(!files)	//	log파일이 없다면 메세지를 남기고 다음 서버로 넘어간다.
		{
			_log("%d -> Not Found File for Compressing!!\n",cSRVUTIL::GetTimes());
			continue;
		}

		char	compareDate[16];
		sprintf(compareDate,"%d",pressDay);

		cSTRINGS * selectFiles = new cSTRINGS;

		{	//	해당 디렉토리에서 pressDay(어제 날짜)가 포함된 파일들을 찾는다.
			for(int i =0; i < files->Count ; i++)
			{
				char * fn = files->GetStr(i);

				if	(strstr(fn,compareDate))		
					selectFiles->Add(fn);
			}
		}

		KILL(files);//log파일의 리스트를 파괴한다.

		if	(selectFiles->Count == 0)//log파일이 없다면 메세지를 남기고 다음 서버로 넘어간다.
		{
			_log("%d -> Not Found File for Compressing\n",cSRVUTIL::GetTimes());
			KILL(selectFiles);
			continue;
		}

		BOOL	isAddFiles = TRUE;
		{	
			//	한개 이상일 경우 하나의 파일로 합친다.
			if	(selectFiles->Count>1)
			{
				DWORD	allSize = 0;
				int		i = 0;
				char	openFile[1024];

				for(i =0 ; i<selectFiles->Count ; i++ )
				{
					sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(i));

					FILE * fp = fopen(openFile,"r");

					if(!fp)
					{
						isAddFiles = FALSE;	
						break;
					}
					allSize += filelength(fileno(fp));
					fclose(fp);
					fp = NULL;
				}

				char	* allData = new char[allSize];
				DWORD	dwProcessLength = 0;

				for(i =0 ; i<selectFiles->Count ; i++ )
				{
					Sleep(1);
					sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(i));

					FILE * fp = fopen(openFile,"r");

					if(!fp)
					{
						isAddFiles = FALSE;
						break;
					}

					DWORD	_size = filelength(fileno(fp));

					fread(allData+dwProcessLength,_size,1,fp);
					dwProcessLength += _size;
					fclose(fp);

					fp = NULL;
				}

				if	(allSize != dwProcessLength)
				{
					isAddFiles = FALSE;
				}
				else
				{
					//	기존 이름을 다 바꾼다.
					char	newFile[1024];
					int j=0;
					for(j =0 ; j<selectFiles->Count ; j++ )
					{
						sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(j));
						sprintf(newFile,"%s\\%s.pre",strPath,selectFiles->GetStr(j));
						MoveFile(openFile,newFile);
					}

					sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(0));
					FILE * fp = fopen(openFile,"w");

					if(!fp)
					{
						isAddFiles = FALSE;
					}
					else
					{
						fwrite(allData,allSize,1,fp);
						fclose(fp);
						//	합해진 파일을 만들었다면 pre 는 다 지우고....
						for(j =0 ; j<selectFiles->Count ; j++ )
						{
							sprintf(newFile,"%s\\%s.pre",strPath,selectFiles->GetStr(j));
							DeleteFile(newFile);
						}
					}
				}
			}
		}

		//	해당파일을 압축한다.
		if	(selectFiles->Count>0)
		{
			char	strNuxFile[1024],strTxtFile[1024];;
			sprintf(strNuxFile,"%s\\press\\%s.nux",strPath,selectFiles->GetStr(0));

			if	(isAddFiles)
			{
				sprintf(strTxtFile,"%s\\%s",strPath,selectFiles->GetStr(0));
				cNUX::PressCreate(strNuxFile);
				cNUX::PressFile(strTxtFile);
				cNUX::PressClose();
				//	기존 파일 백업..
				char	openFile[1024];
				char	newFile[1024];
				sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(0));
				sprintf(newFile,"%s\\Backup\\%s.pre",strPath,selectFiles->GetStr(0));
				MoveFile(openFile,newFile);
//				DeleteFile(openFile);
			}
			else
			{
				cNUX::PressCreate(strNuxFile);
				int i = 0;

				for(i = 0; i<selectFiles->Count ;i++)
				{
					sprintf(strTxtFile,"%s\\%s",strPath,selectFiles->GetStr(i));
					cNUX::PressFile(strTxtFile);
				}

				cNUX::PressClose();

				char	openFile[1024];
				char	newFile[1024];

				for(i =0 ; i<selectFiles->Count ; i++ )
				{
					sprintf(openFile,"%s\\%s",strPath,selectFiles->GetStr(i));
					sprintf(newFile,"%s\\Backup\\%s.pre",strPath,selectFiles->GetStr(i));
					MoveFile(openFile,newFile);
//					DeleteFile(openFile);
				}
			}
		}
		KILL(selectFiles);
	}
	return; 
}

void
PressThread(LPVOID lpParameter)
{
	_log("START PressThread ...");

	static int	iCurrentHour = 1;	//cSRVUTIL::GetHour();

	BOOL	isSaveTime = FALSE;

	while(g_bThreadLoop)
	{
		Sleep(100);

		if	(iCurrentHour != 23 && iCurrentHour != cSRVUTIL::GetHour())
		{
			for (int _iloop = 0;_iloop<svrCount;_iloop++)
			{
				char	strPath[1024];
				DWORD	pressDay = cSRVUTIL::GetTime();

				strcpy(strPath,l_pLogger[_iloop].m_strLogPath);

				char	sourceFn[1024],pressFn[1024];

				sprintf(sourceFn,"%s\\%d.log",strPath,pressDay);
				sprintf(pressFn,"%s\\today.nux",strPath);

				if (!IsFile(sourceFn))
					continue;

				if	(IsFile(pressFn))
				{
					if	(DeleteFile(pressFn))	//	if (pressFn) exists, DELETE it
					{
//#ifdef _FOR_KOREA
						_log("기존 파일을 삭제할 수 없어서.. [%s]파일은 시간별 복사 포기",sourceFn);
//#endif
						continue;
					}
				}

				cNUX::PressCreate(pressFn);
				cNUX::PressFile(sourceFn);
				cNUX::PressClose();
			}

			iCurrentHour = cSRVUTIL::GetHour();
		}

		if	(!isSaveTime && cSRVUTIL::GetHour()!=3)
			isSaveTime = TRUE;
		else
		if	(cSRVUTIL::GetHour()==3 && isSaveTime)	//	매일 새벽 3시에 로그 압축
		{
			HANDLE hProcessPress = NULL;

			DWORD	strtime = timeGetTime();

			printf(" START Press... \n");

			hProcessPress = (HANDLE)_beginthread(ProcessPressThread, 0, NULL); 

			if	(hProcessPress == NULL)
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

void AdditionThread(LPVOID lpParameter)
{
	cHardwareInfo	hwInfo;

	_log("START AdditionThread ...");
	
	DWORD	dwSSCheckTime = timeGetTime();
	DWORD	dwLifeSignTime=	timeGetTime();
	
	while(g_bThreadLoop)
	{
		for	(int z = 0;z<UserManager.Size();z++)
		{
			CClient * user = UserManager.GetClientPnt(z,FALSE);

			if	(!user || user->GetIsClose())
				continue;

			SendPacket(user->GetSerial());
		}

		Sleep(1);

		DWORD	dwCurTime	=	timeGetTime();

		if	(dwCurTime	>	dwLifeSignTime	+5000	||	dwCurTime	<	dwLifeSignTime)
		{
			dwLifeSignTime	=	dwCurTime;
			WriteLifeSignToRegistry(eST_LOG,1);
		}

#ifdef dUSE_SERVERSTATUS_REPORT
// 		if	(dwCurTime > dwSSCheckTime + 30000 || dwSSCheckTime > dwCurTime)
// 		{	//	30초에 한번씩 DB에 현재 정보를 갱신해준다.
// 			if( g_SS.isReady() )
// 			{
// 				g_SS.update(cServerStatus::eERROR_TYPE_SERVERSTATUS,1,"No problem");
// 				dwSSCheckTime = dwCurTime;
// 			}
// 
// 			int error = hwInfo.checkHW();
// 			char strText[512];
// 			switch(error)
// 			{
// 			case eHW_WARNING_OVER_CPU_USAGE:
// 				{
// 					sprintf(strText,"eHW_WARNING_OVER_CPU_USAGE %d%% ",hwInfo.m_cCPUInfo.m_iTotalUsage);
// 					g_SS.update( eHW_WARNING_OVER_CPU_USAGE , hwInfo.m_cCPUInfo.m_iTotalUsage , strText);
// 				}
// 				break;
// 			case eHW_WARNING_OVER_MEMORY_USAGE:
// 				{
// 					sprintf(strText,"eHW_WARNING_OVER_MEMORY_USAGE %d/%d ",hwInfo.m_cMemoryInfo.m_lFreeSize,hwInfo.m_cMemoryInfo.m_lFreeSize);
// 					g_SS.update( eHW_WARNING_OVER_MEMORY_USAGE , 0 , strText);
// 				}
// 				break;
// 			case eHW_WARNING_HAVENOT_FREESIZE:
// 				{
// 					sprintf(strText,"eHW_WARNING_HAVENOT_FREESIZE [%s] %u/%u  ",hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_strHddName , hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_iFreeMB, hwInfo.m_dqHDDList.at(hwInfo.m_iErrorHddIndex).m_iTotalMB);
// 					g_SS.update( eHW_WARNING_HAVENOT_FREESIZE , 0 , strText);
// 				}
// 				break;
// 			}
// 		}
#endif

/*		{
			scp.Read(configPath);
			_log("Read Scp : [%s]",configPath);
			svrCount = atoi(scp.getData("SERVERCOUNT"));

			for(int tc = 0;tc<svrCount;tc++)
			{
				char subpath[128];
				strcpy(svrInfo[tc].name,scp.getData("SERVERNAME",tc+1));
				svrInfo[tc].serverCount = atoi(scp.getData("SERVERGCOUNT",tc+1));
				sprintf(subpath,"%s/%s",mainPath,svrInfo[tc].name);
				CreateDirectory(subpath,NULL);
				l_pLogger[tc].initialize(subpath);

				char subName[20];
				sprintf(subName,"SERVERIP%02d",tc);
				for(int gc = 0;gc<svrInfo[tc].serverCount;gc++)
					strcpy(svrInfo[tc].IPs[gc],scp.getData(subName,gc+1));
			}
		}
*/
	}
	_log("END AdditionThread ...");
	return ; 
}



//////////////////////////////////////////////
//	method for packet method
//////////////////////////////////////////////
void	DisUser(int serial)
{
	if	(serial<0 || serial>=dLOG_MAX_USER_COUNT)
		return;

	CCritical CS(&csRemoveUser);

	CClient * user	=	UserManager.GetClientPnt(serial);

	if	(!user)
	{
		LeaveCriticalSection(&csRemoveUser);
		return;
	}

	if	(user->GetIsUsed())
	{
		user->SetIsUsed(FALSE);
		_log("(%s)%d is Exit. - %d\n",user->GetID(),serial);
	}

	UserManager.Del(serial);
}

void
CommandThread(LPVOID lpParameter)
{
	cInputConsoleCommand	inputCommand;

	BOOL	bIsConfirmQuit	=	FALSE;

	while(g_bThreadLoop)
	{
		char	*lpstrCommand	=	inputCommand.getCommand(TRUE);

		if	(!lpstrCommand)
			continue;

		if	(stricmp(lpstrCommand,"q")==0)
		{
			printf(" Quit ? (y/n) ");

			bIsConfirmQuit	=	TRUE;

			continue;
		}
			
		if	(bIsConfirmQuit && stricmp(lpstrCommand,"y")==0)
		{
			l_bIsActiveMainLoop		=	FALSE;

			WriteServerStatusToRegistry(eST_LOG,eSERVER_STATUS_QUIT);

			shutdown(lisSock,1);
			closesocket(lisSock); 

			lisSock	=	NULL;

			Sleep(1000);

			break;
		}

		bIsConfirmQuit	=	FALSE;
	}

	return;
}
