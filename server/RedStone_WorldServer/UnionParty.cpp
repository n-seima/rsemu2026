#include "party.h"
#include "Client.h"

cUnionPartyManager	g_UnionpartyManager;


BOOL
cUnionParty::CreateUnionParty(CPartyInfo* _AskerParty , CPartyInfo* _ReplayerParty)
{
	if(!_AskerParty)
		return FALSE;
	
	if(!_ReplayerParty)
		return FALSE;

	g_UnionpartyManager.getFreeDenIndex(&m_wSerial);

	_AskerParty->m_wUnionPartyserial		= m_wSerial;
	_ReplayerParty->m_wUnionPartyserial		= m_wSerial;

	//걍 포인터를 넣는다 따라서 파티 연합 파티 정보는 파티정보를 참조 하도록 한다.
	m_apMemberParty[0] = _AskerParty;
	m_apMemberParty[1] = _ReplayerParty;
	
	return TRUE;
}

void
cUnionParty::dissolutuionParty(int _iBecause)
{
	for (int i = 0 ; i < getMemberPartyCount() ; i++)
	{
		if(m_apMemberParty[i]->m_wSerial ==	0xffff)
			break;

		m_apMemberParty[i]->releaseUnionParty();
		m_apMemberParty[i]->m_wUnionPartyserial	=	0xffff;		
	}

	//모든 게임서버에 연합이 풀렷다고 통보해 준다.
	SG_UNION_PARTY_WORK					packet;	
	ZeroMemory(&packet , sizeof(SG_UNION_PARTY_WORK));

	packet.base.set(sizeof(packet) , dWORLDtoSERVERS_DISSOLUTION_UNION_PARTY);

	//0번은 해채하는 연합 파티 시리얼. 
	//1번은 이유. int _iBecause
	packet.wValue[0] = m_wSerial;
	packet.wValue[1] = _iBecause;

	SendDataAllGameServer((char*)&packet , packet.base.wSize);
	//mSEND_GSP(NULL , &packet);
	
	reset();
}



cUnionPartyManager::cUnionPartyManager()
{
	InitializeCriticalSection(&m_csUnionPartyWork);
	m_den.init(dMAX_UNION_PARTY_COUNT);
}


cUnionPartyManager::~cUnionPartyManager()
{
	m_den.close();
	DeleteCriticalSection(&m_csUnionPartyWork);
}



void
cUnionPartyManager::dissolutionUnionParty(WORD _wIndex , int _iBecause)
{
	CPartyInfo*		lpParty	=	g_partyManager.getBySerial(_wIndex);

	if(!lpParty)
		return;

	WORD wUnionPartySerial = lpParty->m_wUnionPartyserial;

	cUnionParty*	lpUnionParty	= m_den.notCheckedget(wUnionPartySerial);

	if(!lpUnionParty || lpUnionParty->m_wSerial == 0xffff)
		return;

	lpUnionParty->dissolutuionParty(_iBecause);
}



BOOL
cUnionPartyManager::isCorrectUnionParty(int _iSerial)
{
	return FALSE;

	CPartyInfo*	lpPartyInfo = g_partyManager.getBySerial(_iSerial);

	if(!lpPartyInfo)
		return FALSE;

	if(lpPartyInfo->m_wUnionPartyserial == 0xffff)
		return FALSE;

	cUnionParty* lpUnionParty = g_UnionpartyManager.m_den.get(lpPartyInfo->m_wUnionPartyserial);

	if(!lpUnionParty)
		return FALSE;

	int iCount = lpUnionParty->getMemberPartyCount();

	if(iCount >= 2)
		return FALSE;

	return TRUE;
}

BOOL
cUnionPartyManager::operateCreateUnionPartyPacket(void *_lpClient,char *_lpPacketData)
{
	SERVERStoWORLD_CREATE_UNION_PARTY	*lpPacket	=	(SERVERStoWORLD_CREATE_UNION_PARTY *)_lpPacketData;

	CPartyInfo *lpAskerParty = g_partyManager.getBySerial(lpPacket->wAskerPartySerial);

	if(!lpAskerParty)
		return FALSE;

	CPartyInfo *lpReplayerParty = g_partyManager.getBySerial(lpPacket->wReplayerPartySerial);

	if(!lpReplayerParty)
		return FALSE; 	

	WORD				wDenIndex = 0xffff;

	//wDenIndex에 값을 세팅해 준다.
	cUnionParty*		lpUnionParty	=	getFreeDenIndex(&wDenIndex);

	if(!lpUnionParty)
		return FALSE;

	SG_UNION_PARTY_WORK			packet;
	ZeroMemory(&packet , sizeof(SG_UNION_PARTY_WORK));

	packet.base.set(sizeof(packet) , dWORLDtoSERVERS_CREATE_UNION_PARTY_RESULT);

	if(lpUnionParty->CreateUnionParty(lpAskerParty , lpReplayerParty))
	{
		packet.wValue[0] = CREATE_UNION_PARTY_RESULT_OK;		//결과
		packet.wValue[1] = lpUnionParty->m_wSerial;				//연합 파티 시리얼.
		packet.wValue[2] = lpAskerParty->m_wSerial;				//요청자 시리얼
		packet.wValue[3] = lpReplayerParty->m_wSerial;			//응답자 시리얼.
	}
	else	
		packet.wValue[0] = CREATE_UNION_PARTY_RESULT_FAILED;

	CClient *Client = (CClient *)_lpClient;

	mSEND_GSP(Client , packet);

	return TRUE;
}

cUnionParty*
cUnionPartyManager::getFreeDenIndex(WORD* _wIndex)
{
	for (int i = 0 ; i < dMAX_UNION_PARTY_COUNT ; i++)
	{
		cUnionParty* lpUnionParty = m_den.notCheckedget(i);
		
		if(lpUnionParty->m_wSerial == 0xffff)
		{	
			(*_wIndex) = i;
			
			return lpUnionParty;
		}
	}
	
	return NULL;
}