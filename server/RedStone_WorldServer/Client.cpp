// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////
#include <winsock2.h>
#include "Client.h"
#include "cPACKET_WORLDSERVER.h"
#include "cSRVUTIL.h"
#include "secretDungeonDefine.h"

#if defined(_FOR_USA) && defined(_FOR_INTER)
	#define dGAPLIMIT		1000000
#else
	#define dGAPLIMIT		90000
#endif

static	int		basesize = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL	Avatar_Data::isWaste()
{	
	if(!bMovingMap)	return FALSE;
	if((timeGetTime() - time)>=0)
	{
		DWORD	test = timeGetTime();
		if( (test - time) >= dGAPLIMIT)
		{	
			return TRUE;	
		}
	}else
	if( (0xffffffff-time) >=dGAPLIMIT)
	{	
		return TRUE;	
	}

	return FALSE;
}

void	Avatar_Data::SetTime(){	time = timeGetTime();	}

//////////////////////////////////////////////////////////////////////
//

CClient::CClient(int	buffersize)
{	
	isSendPacket = TRUE;
	MAX_LENGTH		=	buffersize;
	isUsed			=	FALSE;
	isClose			=	TRUE;
	sock			=	NULL;
	timegap			=	timeGetTime();
	sendpacket_size =	0;
	serial			=	0xffff;
	isReadyToPlay	=	FALSE;
	m_wCurrentPlayerCount = 0;

	rBuffer = new CPacketBuffer(dMAX_PACKET_LENGTH_SERVER);
	sBuffer = new CPacketBuffer(dMAX_PACKET_LENGTH_SERVER);
	InitializeCriticalSection(&csUSER);
}

CClient::~CClient()
{
	delete rBuffer;
	delete sBuffer;

	Kill();	
	DeleteCriticalSection(&csUSER);
}

void	CClient::Open()
{
	UsedUser();
	rBuffer->Reset();
	sBuffer->Reset();
	serial			=	0xffff;
	isSendPacket = TRUE;
	isReadyToPlay	=	FALSE;
	isUsed			=	TRUE;
	isClose			=	FALSE;
	timegap			=	timeGetTime();
	memset(&rOL.OL,0,sizeof(OVERLAPPED));
	rOL.buf.buf = rOL.Buffer;
	rOL.buf.len = dMAX_PACKET_LENGTH_SERVER;

	memset(&sOL.OL,0,sizeof(OVERLAPPED));
	sOL.buf.buf = sOL.Buffer;
	sOL.buf.len = dMAX_PACKET_LENGTH_SERVER;
	sOL.IOState = 0;
	NotUsedUser();
}

void	CClient::PreClose()
{
	if (INVALID_SOCKET != sock) 
	{ 
		closesocket(sock); sock = NULL;
	} 
	isClose	= TRUE;
}

void	CClient::Close()
{	
	isSendPacket = FALSE;
	if (INVALID_SOCKET != sock) 
	{ 
		shutdown(sock,1);
		closesocket(sock); sock = NULL;
	}
	isUsed			=	FALSE;
	isClose			=	TRUE;
	isReadyToPlay	=	FALSE;
	timegap			=	0;
	serial			=	0xffff;
	GameClientMagager.Init();
}

void	CClient::Kill()
{
	if (INVALID_SOCKET != sock) 
	{ 
		shutdown(sock,1);
		closesocket(sock); sock = NULL;
	} 

	isUsed			=	FALSE;
	isClose			=	TRUE;
	isReadyToPlay	=	FALSE;
	isSendPacket	=	FALSE;
	serial			=	0xffff;
}

void		CClient::SetSocket(SOCKET	soc)
{	
	if(sock)
	{ 
		closesocket(sock); sock=NULL;
	}
	sock = soc;

	//	Addr 설정
	struct sockaddr_in name; int len;
	getpeername(sock,(struct sockaddr *)&name,&len);
	memset(addr,0,16);
	memcpy(addr,inet_ntoa(name.sin_addr),16);
}

void		CClient::SetIsUsed(BOOL isused)
{	
	UsedUser();
	isUsed = isused;
	isClose = isused ^ 1;
	NotUsedUser();
}

void		CClient::SetID(char *nid){	strcpy(id,nid);	}

void
CClient::setGameServer()//	이 클라이언트를 게임서버로 설정
{
	m_wIsGameServer	=	TRUE;

	GameClientMagager.allocAvatarDataBuffer();
}

BOOL	CClient::isWaste()
{
//	if( (abs(timeGetTime() - GetTimeGap()) > dGAPLIMIT+1000) && (abs(timeGetTime() - GetTimeGap()) < dGAPLIMIT*2) )
//		return TRUE;
	return FALSE;
}


int		CClient::AddRecvPacket(char *data,int size)
{
	timegap = timeGetTime();
	return rBuffer->AddData(data,size);
}

int		CClient::AddRecvPacket(int size){	return AddRecvPacket(rOL.buf.buf,size);}
int		CClient::GetRecvPacket(char * recvpacket)
{	
	int	ret = rBuffer->GetData(recvpacket,1);
	if(ret && *(WORD *)recvpacket==0 || *(WORD *)recvpacket>=65535)
	{
		_log("Error...2");
		return 0;
	}
	return ret;
}

int		CClient::AddSendPacket(char *data,int size)
{
	if(*(WORD *)data==0 || *(WORD *)data>=65535)
	{
		_log("Error...3");
		return 0;
	}
	return sBuffer->AddData(data,size);
}

int		CClient::CompleteSendPacket(int size)
{
	if(sendpacket_size<size)	//	하나의 데이터가 완전하게 보내지지 않았다면..
	{
		char	tmpPacket[dMAX_PACKET_LENGTH];memset(tmpPacket,0,dMAX_PACKET_LENGTH);
		memcpy(tmpPacket,sendpacket+size,sendpacket_size-size);
		memset(sendpacket,0,dMAX_PACKET_LENGTH);
		sendpacket_size -= size;
		memcpy(sendpacket,tmpPacket,sendpacket_size);
		return sendpacket_size;
	}
	sBuffer->Next();
	return 0;
}

int		CClient::GetMapIndex(int proserial)
{
	for(int i=0;i<dGAME_MAX_FIELD_COUNT;i++)
	{
		if	(cGSInfo[i].wIndex==proserial)
			return i;
	}

	return -1;
}
//****************************************************************************************
//	Class	Client Manager
//****************************************************************************************
CClientManager::CClientManager()
{	InitializeCriticalSection(&csDeque);	
	Init();
}

CClientManager::~CClientManager()
{	DeleteCriticalSection(&csDeque);
}

CClientManager & CClientManager::GetInstance()
{	static CClientManager cmanager;
	return cmanager;
}

WORD	CClientManager::NextNext()
{	for(int _next = m_wNext;_next<dWORLD_MAX_USER_COUNT;_next++)
	{	if(User[_next].GetSerial()== 0xffff && !User[_next].GetIsUsed())
		{
			m_wNext = _next;
			return m_wNext;
		}
	}
	int end = m_wNext;
	for(_next = 0;_next<end;_next++)
	{	if(User[_next].GetSerial() == 0xffff && !User[_next].GetIsUsed())
		{	m_wNext = _next;
			return m_wNext;
		}
	}
	_log("NEXTNEXT ERROR ERROR ERROR ERROR");
	m_wNext = 0xffff;
	return 0xffff;
}
void	CClientManager::SetBaseSize(int bs){	basesize = bs;	}

int		CClientManager::GetAllUserCount()
{
	int result=0;

	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if	(User[i].GetIsUsed())
			result	+=	User[i].GameClientMagager.Size();
	}

	return result;
}

void	CClientManager::Init()
{	m_wNext = m_wCount = m_wGSCount = 0;
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		User[i].Close();
	}
	m_wBCSIndex = 0xffff;
}
void	CClientManager::End(){	Init();	}

int		CClientManager::Add()
{	CCritical	CS(&csDeque);

	WORD	wResult = 0xffff;
	if(m_wCount == dWORLD_MAX_USER_COUNT)	return wResult;
	if(m_wNext == 0xffff)					return wResult;
	wResult = m_wNext;
	User[m_wNext].SetIsUsed(TRUE);
	m_wCount++;
	NextNext();
	return wResult;
}

void	CClientManager::Del(int idx,char *id)
{	CCritical	CS(&csDeque);

	if(idx>=dWORLD_MAX_USER_COUNT)	return;
	if(User[idx].GetSerial() == 0xffff)	return;

	User[idx].Close();
	if(m_wCount==0)	_log("ERROR CClientManager::Del m_wCount = 0");
	else			m_wCount--;
	if(m_wNext == 0xffff)	m_wNext = idx;
}

CClient		*CClientManager::GetClientPnt(char *id)	//	ID로 CClient 포인터 얻기
{	
	for(int i = 0; i < dWORLD_MAX_USER_COUNT; i++)
	{
		if(!User[i].GetIsUsed())	continue;
		if(STRICMP(id,User[i].GetID()) == 0  && User[i].GetSocket()>0)
			return &User[i];
	}
	return NULL;
}

CClient		*CClientManager::GetClientPnt(WORD idx,BOOL	isSerial)	//	serial로 CClient 포인터 얻기
{	
	if(idx >=dWORLD_MAX_USER_COUNT)
		return NULL;

	if(User[idx].GetIsUsed())
		return &User[idx];

	return NULL;
}

Avatar_Data	*CClientManager::GetGClientPnt(char *strId,int flag)	//	게임서버에서 유저 찾기
{
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if	(User[i].m_wIsGameServer	==	FALSE)
			continue;
		if	(User[i].GetSerial()==0xffff || !User[i].GetIsUsed())
			continue;

		if	(STRICMP(User[i].GetID(),"#login") == 0)
			continue;

		if	(STRICMP(User[i].GetID(),"#Admin") == 0 )
			continue;

		Avatar_Data * ava = NULL;

		if(!flag)
			ava = User[i].GameClientMagager.GetAvatarPnt(strId);
		else
			ava = User[i].GameClientMagager.GetAvatarPnt(strId,1);

		if	(ava)
			return ava;
	}

	return NULL;
}

CClient	*CClientManager::GetClientOfField(int properserial,WORD	_wPrimium,WORD & _wResult)
{
	if (properserial	>=	dINSTANCE_FIELD_FLAG)
		properserial	-=	dINSTANCE_FIELD_FLAG;

	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if	(User[i].m_wIsGameServer	==	FALSE)
			continue;
		if	(User[i].GetSerial()==0xffff || !User[i].GetIsUsed())
			continue;

		if	(STRICMP(User[i].GetID(),"#login") == 0)
			continue;

		if	(STRICMP(User[i].GetID(),"#Admin") == 0 )
			continue;

		for(int j=0;j<User[i].wMapCount;j++)
		{
			if(User[i].cGSInfo[j].wIndex== properserial)
			{
				if(User[i].cGSInfo[j].wType ==	eFT_PREMIUM_ZONE && !_wPrimium)
				{	
					_wResult = 6;	
					goto _FIELD_END1;	
				}

				return &User[i];
			}
		}
	}
_FIELD_END1:
	return NULL;
}

//
//	이거 길드전 필드냐?
BOOL
CClientManager::isGuildBattleField(int _iFieldSerial)
{
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if(User[i].GetIsUsed() && STRICMP(User[i].GetID(),"#login")!=0 && STRICMP(User[i].GetID(),"#Admin")!=0)
		{	
			for(int j=0;j<User[i].wMapCount;j++)
			{
				if(User[i].cGSInfo[j].wIndex	== _iFieldSerial)
				{
					if(User[i].cGSInfo[j].wType ==	eFT_GUILD_BATTLE_FIELD)	return	TRUE;
				}
			}
		}
	}
	return FALSE;
}


CClient	*CClientManager::GetClientOfField(char *mn,int & properserial,WORD	_wPrimium,WORD & _wResult)
{
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if(User[i].GetIsUsed() && STRICMP(User[i].GetID(),"#login")!=0 && STRICMP(User[i].GetID(),"#Admin")!=0)
		{	
			for(int j=0;j<User[i].wMapCount;j++)
			{
				if(STRICMP(User[i].cGSInfo[j].strMapName,mn)==0)
				{
					if(User[i].cGSInfo[j].wType ==eFT_PREMIUM_ZONE && !_wPrimium){	_wResult = 6;	goto _FIELD_END2;	}
					properserial = User[i].cGSInfo[j].wIndex;
					return &User[i];
				}
			}
		}
	}
_FIELD_END2:
	return NULL;
}

WORD		CClientManager::GetClientOfGClientSerial(char *strId,int flag)	//	한 유저가 속한 게임서버의 시리얼
{
	for(WORD i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if(STRICMP(User[i].GetID(),"#login")!=0 && STRICMP(User[i].GetID(),"#Admin")!=0 && User[i].GetIsUsed())
		{
			Avatar_Data * ava = NULL;
			if(!flag)	ava = User[i].GameClientMagager.GetAvatarPnt(strId);
			else		ava = User[i].GameClientMagager.GetAvatarPnt(strId,1);
			if(ava)		return i;
		}
	}
	return -1;
}

CClient	*	CClientManager::GetClientOfGClientPnt(char *strId , int _iFindByName)	//	한 유저가 속한 게임서버의 시리얼
{
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if	(STRICMP(User[i].GetID(),"#login")!=0 && STRICMP(User[i].GetID(),"#Admin")!=0 &&  User[i].GetIsUsed())
		{
			Avatar_Data * ava = NULL;

			if	(_iFindByName)
				ava = User[i].GameClientMagager.GetAvatarPnt(strId,1);
			else
				ava = User[i].GameClientMagager.GetAvatarPnt(strId);

			if	(ava)
				return &User[i];
		}
	}
	return NULL;
}

CClient	*	CClientManager::GetClientOfGClientPntForDelete(char *strId , int flag)	//	삭제를 위한 검색. 맵 이동중인거 안가져 온다.
{
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if(User[i].GetIsUsed() && STRICMP(User[i].GetID(),"#login")!=0 && STRICMP(User[i].GetID(),"#Admin")!=0)
		{
			Avatar_Data * ava = NULL;
			if(!flag)	ava = User[i].GameClientMagager.GetAvatarPnt(strId);
			else		ava = User[i].GameClientMagager.GetAvatarPnt(strId,1);
			if(ava)		return &User[i];
		}
	}
	return NULL;
}

char	*	CClientManager::GetFieldName(int fieldindex)
{
	for(int i=0;i<dWORLD_MAX_USER_COUNT;i++)
	{
		if(User[i].GetIsUsed() && STRICMP(User[i].GetID(),"#login")!=0 && STRICMP(User[i].GetID(),"#Admin")!=0)
		{	for(int j=0;j<User[i].wMapCount;j++)
			{
				if(User[i].cGSInfo[j].wIndex == fieldindex)
					return User[i].cGSInfo[j].strMapName;
			}
		}
	}
	return NULL;
}

int CClientManager::SendToBCS(char * _cData, WORD _size)
{
	if(m_wBCSIndex == 0xffff)	return 0;

	CClient * pcBcs = GetClientPnt(m_wBCSIndex);
	if(!pcBcs)					return 0;

	pcBcs->AddSendPacket((char *)_cData,_size);
	return 1;
}
//****************************************************************************************
//	Class	Game in Client Manager
//****************************************************************************************
CGClientManager::CGClientManager()
{	
	InitializeCriticalSection(&csDeque);

	m_pStack	=	NULL;
	Init();
}

CGClientManager::~CGClientManager()
{	
	End();

	pKILL(m_pStack);

	DeleteCriticalSection(&csDeque);
}

void
CGClientManager::allocAvatarDataBuffer()
{
	pKILL(m_pStack);

	m_pStack	=	new Avatar_Data [dGAME_MAX_USER_COUNT];
}


WORD	CGClientManager::NextNext()
{
	if	(!m_pStack)
	{
		_log("!m_pStack NextNext()");

		return	0xffff;
	}

	for(int _next = m_wNext;_next<dGAME_MAX_USER_COUNT;_next++)
	{
		if	(m_pStack[_next].wSerial == 0xffff)
		{
			m_wNext = _next;
			return m_wNext;
		}
			
	}
	int end = m_wNext;
	for(_next = 0;_next<end;_next++)
	{
		if(m_pStack[_next].wSerial == 0xffff)
		{
			m_wNext = _next;
			return m_wNext;
		}
	}
	_log("NEXTNEXT ERROR ERROR ERROR ERROR");

	return 0xffff;
}

void	CGClientManager::Init()	
{
	m_wNext = m_wCount = 0;

	if	(m_pStack)
	{
		for	(int i=0;i<dGAME_MAX_USER_COUNT;i++)
			m_pStack[i].close();

		pKILL(m_pStack);
	}
}

void	CGClientManager::End()
{
	Init();	
}

int		CGClientManager::Add(char *_strId)
{
	if	(!m_pStack)
	{
		_log("!m_pStack CGClientManager::Add");
		return	-1;
	}

	CCritical	CS(&csDeque);

	int		wResult = 0xffff;

	if	(m_wCount >= dGAME_MAX_USER_COUNT)
		return -1;

	if	(m_wNext == 0xffff)
		return wResult;

	DWORD	hash = GetHashCode((BYTE *)_strId);

	for(WORD l_wCount=0;l_wCount<dGAME_MAX_USER_COUNT;l_wCount++)
	{
		if	(m_pStack[l_wCount].wSerial == 0xffff)
			continue;

		if	(hash == m_pStack[l_wCount].hashIdCode && STRICMP(m_pStack[l_wCount].strMaster,_strId)==0)
		{
//			return -2;
			m_pStack[l_wCount].wSerial = wResult;
			m_pStack[l_wCount].wState  = 1;
			m_pStack[l_wCount].time = timeGetTime();
			m_pStack[l_wCount].min = cSRVUTIL::GetMin();
			m_pStack[l_wCount].playtime = 0;
			m_pStack[l_wCount].isOper = 0;
			m_pStack[l_wCount].hashIdCode = hash;

			return l_wCount;
		}
	}

	wResult = m_wNext;
	m_wCount++;

	m_pStack[wResult].wSerial = wResult;
	m_pStack[wResult].wState  = 1;
	m_pStack[wResult].time = timeGetTime();
	m_pStack[wResult].min = cSRVUTIL::GetMin();
	m_pStack[wResult].playtime = 0;
	m_pStack[wResult].isOper = 0;
	m_pStack[wResult].hashIdCode = hash;

	NextNext();

	return wResult;
}

void	CGClientManager::writeData(WORD _wSerial ,CPlayerSaveDataForServerDefine * data,WORD  gsserial,WORD oper,WORD _isNetCafeUser,WORD s)
{
	if	(!m_pStack)
	{
		_log("!m_pStack CGClientManager::writeData1");
		return;
	}

	if	(_wSerial>=dGAME_MAX_USER_COUNT)
		return;
	
	memcpy(&m_pStack[_wSerial].data,data,sizeof(CPlayerSaveDataForServerDefine));
	memcpy(m_pStack[_wSerial].strMaster,data->m_strId,dID_LENGTH);
	m_pStack[_wSerial].hashIdCode = GetHashCode((BYTE *)data->m_strId);
	m_pStack[_wSerial].wState = s;
	m_pStack[_wSerial].SetTime();
	m_pStack[_wSerial].wGameSerial = gsserial;
	m_pStack[_wSerial].isOper = oper;
	m_pStack[_wSerial].isNetCafeUser = _isNetCafeUser;

	if(m_pStack[_wSerial].isOper>6)
	{
		_log("ERROR OPER (in WRITEDATA)is = %d ,[%s][%s]",m_pStack[_wSerial].isOper,m_pStack[_wSerial].data.m_strId,m_pStack[_wSerial].data.m_strName);
		m_pStack[_wSerial].isOper = 0;
	}
}

void
CGClientManager::writeKeepData(WORD _wSerial,CUpkeepPlayerData * upkeepdata,BOOL isMoveField)
{
	if	(!m_pStack)
	{
		_log("!m_pStack CGClientManager::writeData2");
		return;
	}

	if	(_wSerial>=dGAME_MAX_USER_COUNT)
		return;
	
	memcpy(&m_pStack[_wSerial].keepData,upkeepdata,sizeof(CUpkeepPlayerData));
	m_pStack[_wSerial].bMovingMap = isMoveField;
	m_pStack[_wSerial].time = timeGetTime();
}

void
CGClientManager::writeKeepStatus(WORD _wSerial,cActorStatusUpkeep * _lpUpkeepStatus)
{
	if	(!m_pStack)
	{
		_log("!m_pStack CGClientManager::writeKeepStatus");
		return;
	}

	if	(_wSerial>=dGAME_MAX_USER_COUNT)
		return;

	memcpy(&m_pStack[_wSerial].upkeepStatus,_lpUpkeepStatus,sizeof(cActorStatusUpkeep));
}

BOOL	CGClientManager::Del(WORD _serial,char *str)
{
	CCritical	CS(&csDeque);

	if	(!m_pStack)
	{
		_log("!m_pStack CGClientManager::Del");

		return	FALSE;
	}

	if	(_serial>=dGAME_MAX_USER_COUNT)
		return FALSE;

	if	(m_pStack[_serial].wSerial == 0xffff)
		return FALSE;

//	_logf("[%s(%s) - %d] Delete User Data  -> [%s]\n",m_pStack[_serial].data.m_strId,m_pStack[_serial].data.m_strName,m_pStack[_serial].data.m_dwSaveCount,str);

	m_pStack[_serial].close();

	if(m_wCount==0)
		_log("ERROR CClientManager::Del m_wCount = 0");
	else
		m_wCount--;
	if(m_wNext == 0xffff)	m_wNext = _serial;

	return TRUE;
}

BOOL	CGClientManager::Del(char * id,char *str)
{
	if	(!m_pStack)
	{
		_log("!m_pStack CGClientManager::Del");

		return	FALSE;
	}

	DWORD	hashcode  = GetHashCode((BYTE *)id);

	for(WORD l_wCount=0;l_wCount<dGAME_MAX_USER_COUNT;l_wCount++)
	{
		if	(m_pStack[l_wCount].wSerial == 0xffff)
			continue;

		if	(m_pStack[l_wCount].hashIdCode == hashcode && STRICMP(m_pStack[l_wCount].strMaster,id)==0)
			return Del(m_pStack[l_wCount].wSerial,str);
	}
//	_logf("ERROR [%s]데이타 삭제 실패  -> [%s]\n",id,str);
	return FALSE;
}

Avatar_Data	*CGClientManager::GetAvatarPnt(char *id, int flag)	//	ID로 CClient 포인터 얻기
{
	if	(!m_pStack)
	{
//		_log("!m_pStack CGClientManager::GetAvatarPnt");
		return	NULL;
	}

	DWORD	hashcode = GetHashCode((BYTE *)id);

	if	(flag)
	{
		for(WORD l_wCount=0;l_wCount<dGAME_MAX_USER_COUNT;l_wCount++)
		{
			if(m_pStack[l_wCount].wSerial == 0xffff)
				continue;

			if(STRICMP(m_pStack[l_wCount].data.m_strName,id)==0)
				return &m_pStack[l_wCount];
		}
	}
	else
	{
		for(WORD l_wCount=0;l_wCount<dGAME_MAX_USER_COUNT;l_wCount++)
		{
			if(m_pStack[l_wCount].wSerial == 0xffff)
				continue;

			if(m_pStack[l_wCount].hashIdCode == hashcode && STRICMP(m_pStack[l_wCount].strMaster,id)==0)
				return &m_pStack[l_wCount];
		}
	}
	return NULL;
}

Avatar_Data	*CGClientManager::GetAvatarPnt(int idx,BOOL	isSerial)	//	serial로 CClient 포인터 얻기
{	
	if	(!m_pStack)
	{
//		_log("!m_pStack - GetAvatarPnt");
		return	NULL;
	}

	if	(idx>=dGAME_MAX_USER_COUNT)	
		return NULL;

	if	(m_pStack[idx].wSerial == 0xffff)
		return NULL;

	return &m_pStack[idx];
}
