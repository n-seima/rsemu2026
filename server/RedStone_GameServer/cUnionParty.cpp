//////////////////////////////////////////////////////////////////////////
//	09-02-02	--파티 연합 때문에 추가된 함수들 입니다.--		-JBC-	//
//	구조는 기본적으로 파티 시스템의 구조를 따릅니다.
//	연합 파티 생성(직접 걸어서하는)을 제외한 다른 것들은 월드 서버에서 처리한다 
//	
//////////////////////////////////////////////////////////////////////////

#include "cParty.H"
#include "cGAME.H"
#include "CServer.H"
#include "debugCode.H"
#include "packetManager.H"
#include "GameDLL.H"

cUnionPartyManager*		g_pUnionpartyManager;


BOOL
cUnionParty::CreateUnionParty(cParty* _AskerParty , cParty* _ReplayerParty)
{
	if(!_AskerParty)
		return FALSE;
	
	if(!_ReplayerParty)
		return FALSE;
	
	//걍 포인터를 넣는다 따라서 파티 연합 파티 정보는 파티정보를 참조 하도록 한다.
	m_apMemberParty[0] = _AskerParty;
	m_apMemberParty[1] = _ReplayerParty;
	
	return TRUE;
}

// void
// cUnionParty::syncMemberStatus(cACTOR *_lpActor)
// {
// 
// }

cUnionPartyManager::cUnionPartyManager()
{
	FPInitCS(&m_csUnionPartyWork);
	m_den.init(dMAX_UNION_PARTY_COUNT);
}


cUnionPartyManager::~cUnionPartyManager()
{
	m_den.close();
	DeleteCriticalSection(&m_csUnionPartyWork);
}

BOOL				
cUnionPartyManager::makeUnionParty(DWORD _dwUniqueSerial , char* _strName ,WORD _wIndex, WORD* _pwPartySerialList)
{
	cUnionParty	UnionParty;
	
	UnionParty.init(_wIndex,_dwUniqueSerial,_strName);
	
	if (!m_den.insert(_wIndex,&UnionParty))
	{
		m_den.remove(_wIndex);
		m_den.insert(_wIndex,&UnionParty);
		
		CLOG("Unionparty","음..연합 파티 생성 실패 했었었어.. -o-");
	}

	UnionParty.m_wMemberPartyCount = 0; 

	if (_pwPartySerialList)
	{
		cParty*	lpParty = g_pPartyManager->m_den.getList();
		
		for (int i =0 ;  i < dUNION_PARTY_COUNT ; i++)
		{
			if(_pwPartySerialList[i] == 0xffff)
			{
				UnionParty.m_wMemberPartyCount = i;
				break;
			}
			
			//연합 파티는 파티들의 포인터를 가지고 있는다.
			memcpy(&UnionParty.m_apMemberParty[i] , &lpParty[i] , sizeof(cParty));
			
			lpParty[i].setUnionMember();
		}
	}
		
	return	TRUE;
	
}

void
cUnionPartyManager::updateUnionPartyInfo(WORLDtoSERVERS_UPDATE_UNIONPARTYINFO* _lpPacket)
{
	mCS(m_csUnionPartyWork);
	cParty				*lpParty			=	NULL;
	BOOL				bIsCreateParty		=	_lpPacket->wIsCreate;
	
	if (bIsCreateParty)
	{
		WORD		wIndex			= 0xffff;
		cUnionParty	*lpUnionParty	=	getFreeDenIndex(&wIndex);

		lpUnionParty->reset();
		
		if (lpUnionParty == NULL)
			CLOG("Union party","연합 파티를 생성하려는데 숫자가 부족하군요. '%s'",lpUnionParty->m_strUnionPartyName);		
 		else
 			makeUnionParty(_lpPacket->m_dwUniqueSerial , _lpPacket->strName , wIndex  , 
			_lpPacket->unionPartyInfo.wPartySerialList);	//	생성되는 거냐?
	}
}

cUnionParty*
cUnionPartyManager::getFreeDenIndex(WORD* _wIndex)
{
	for (int i = 0 ; i < dMAX_UNION_PARTY_COUNT ; i++)
	{
		cUnionParty* lpUnionParty = m_den.get(i);

		if(lpUnionParty == NULL)
		{
			if(!_wIndex)
				(*_wIndex) = i;

			return lpUnionParty;
		}
	}
	
	return NULL;
}

cUnionParty*
cUnionPartyManager::getDenIndex(WORD _wIndex)
{
	if(_wIndex == 0xffff)
		return NULL;

	cUnionParty* lpUnionParty = m_den.get(_wIndex);
			
	return lpUnionParty;
}


BOOL
cUnionPartyManager::CreateUnionPartyWork(cACTOR* _lpReplayer , cACTOR* _lpAsker)
{
	mCS(m_csUnionPartyWork);
	
	cParty	*lpParty				=	_lpAsker->getParty();
	cParty	*lpReplayerParty		=	_lpReplayer->getParty();
	
 	if	(lpParty	==	NULL)				//갸아악 파티 없다.
		return	FALSE;

	if	(lpReplayerParty	==	NULL)		//갸아악 파티 없다.
		return	FALSE;

	g_game.SendWPCreateUnionParty(_lpAsker->getParty()->m_wSerial , _lpReplayer->getParty()->m_wSerial);

	/*
	WORD wFreeIndex = 0;

	getFreeDenIndex(&wFreeIndex);

	char	strUnionPartyName[dUNION_PARTYNAME_LENGTH];
	
	for(int i=0;i<1000;i++)
	{
		sprintf(strUnionPartyName,"%s_%.3d",_lpAsker->m_strName,i);
		
		if	(getByName(strUnionPartyName)	==	NULL)
			break;
	}

	makeUnionParty(dwUniqueUnionSerialCount , strUnionPartyName , wFreeIndex);

	if(!joinUnionPartyMember(wFreeIndex , lpParty))				//초대한 사람을 입력하고.
		return FALSE;

	lpParty->setUnionMember();
	getFreeDenIndex(&wFreeIndex);						//빈곳을 찾는다.

	if(!joinUnionPartyMember(wFreeIndex , lpReplayerParty))		//응답한 사람을 입력한다.
		return FALSE;

	lpReplayerParty->setUnionMember();

	//여기까지 오면 파티연합은 결성 된거다...-_-;

	//sendWPUnionPartyInfo()

	//dSG_REPLAY_JOIN_UNION_PARTY 패킷 종류 , lpParty 가입자의 파티 , TRUE 승락 여부 , 
	sendUnionPartyEtcInfo(dSG_REPLAY_JOIN_UNION_PARTY , lpParty , TRUE , 2);
	sendUnionPartyEtcInfo(dSG_REPLAY_JOIN_UNION_PARTY , lpReplayerParty , TRUE , 2);

	//연합 파티원의 정보를 전달합니다.
	for (i = 0 ; i < lpParty->getMemberCount() ; i++)
	{
		cACTOR	*lpActor = lpParty->getActorByMemberIndex(i);

		if (!lpActor)
			break;

		//자신의 파티원 정보 전송.
		sendUnionPartyInfo(wFreeIndex , lpParty->m_wUnionPartySerial , lpParty->getMemberCount() , lpActor->m_iClientSerial);
		sendUnionPartyInfo(wFreeIndex , lpReplayerParty->m_wUnionPartySerial , lpReplayerParty->getMemberCount() , lpActor->m_iClientSerial);
	}

	for (i = 0 ; i < lpReplayerParty->getMemberCount() ; i++)
	{
		cACTOR	*lpActor = lpReplayerParty->getActorByMemberIndex(i);
		
		if (!lpActor)
			break;
		
		sendUnionPartyInfo(wFreeIndex , lpParty->m_wUnionPartySerial , lpParty->getMemberCount() , lpActor->m_iClientSerial);
		sendUnionPartyInfo(wFreeIndex , lpReplayerParty->m_wUnionPartySerial , lpReplayerParty->getMemberCount() , lpActor->m_iClientSerial);
	}
	*/

	return TRUE;
}


BOOL
cUnionPartyManager::sendUnionPartyMemberInfoToAllUnionParty(int _iUnionPartySerial , int _iUnionPartyIndex , int _iSendPartyMemberIndex)
{
	cUnionParty*		lpUnionParty	=	m_den.notCheckedget(_iUnionPartySerial);

	if(!lpUnionParty)
		return FALSE;

	for (int i = 0 ; i < lpUnionParty->getPartyCount() ; i++)
	{
		cParty* lpParty	=	lpUnionParty->m_apMemberParty[i];

		if(!lpParty)
			break;

		for (int j = 0 ; j < lpParty->getMemberCount() ; j++)
		{
			cACTOR	*lpActor = lpParty->getActorByMemberIndex(i);

			if(!lpActor)
				break;

			sendUnionPartyMemberInfo(_iUnionPartySerial , _iUnionPartyIndex , _iSendPartyMemberIndex , lpActor->m_iClientSerial);
		}
	}

	return TRUE;
}

//	cUnionPartyManager의 _iUnionPartyIndex 인덱스로 cUnionParty를 구하고 
//	cUnionParty의 _iUnionPartyMemberIndex 인덱스로 cParty를 구하고 
//	cParty의 _iSendPartyMemberIndex 인덱스로 Member의 정보를 구한다. -_-; 미안...
BOOL
cUnionPartyManager::sendUnionPartyMemberInfo(int _iUnionPartySerial , int _iUnionPartyIndex , int _iSendPartyMemberIndex , int _iClientSerial)
{
	cUnionParty*	lpUnionParty = m_den.get(_iUnionPartySerial);

	if(!lpUnionParty)
		return FALSE;

	cParty* lpParty		=	lpUnionParty->m_apMemberParty[_iUnionPartyIndex];

	if(!lpParty)
		return FALSE;

	cACTOR*	lpActor		=	lpParty->getActorByMemberIndex(_iSendPartyMemberIndex);

	if(!lpParty)
		return FALSE;

	//cPARTY_MEMBER
	SG_UNION_PARTY_MEMBER_INFO					packet;
	ZeroMemory(&packet , sizeof(SG_UNION_PARTY_MEMBER_INFO));

	packet.base.set(sizeof(packet),dSG_UNION_PARTY_MEMBER_INFO);

	packet.wUnionPartySerial		= _iUnionPartySerial;
	packet.wUnionPartyIndex			= _iUnionPartyIndex;
	packet.wPartyMemberIndex		= _iSendPartyMemberIndex;

	strcpy(packet.Info.m_strName , lpParty->m_aMember[_iSendPartyMemberIndex].m_strName);
	packet.Info.m_wHP				=	lpActor->m_iHP * 100 / lpActor->getMaxHP();
	packet.Info.m_dwState			=	lpParty->m_aMember[_iSendPartyMemberIndex].getInfoForUnionPartyState();
	packet.Info.m_wUnionPartySerial	=	lpUnionParty->m_wSerial;

	g_userPM.add(_iClientSerial ,&packet,packet.base.wSize);

	return TRUE;
}


BOOL
cUnionPartyManager::sendUnionPartyInfo(int _iUnionPartyIndex , int _iUnionPartyMemberIndex ,int _iEffectivenessCount, int _iTargetClientSerial)
{
	cUnionParty*	lpUnionParty = g_pUnionpartyManager->m_den.get(_iUnionPartyIndex);

	if(!lpUnionParty)
		return FALSE;
	
	//cParty* lpParty		=	lpUnionParty->m_apMemberParty[_iUnionPartyMemberIndex];

// 	if(!lpParty)
// 		return FALSE;

	if(lpUnionParty->m_apMemberParty[_iUnionPartyMemberIndex]->m_wSerial == 0xffff)
		return FALSE;

	SG_UNION_PARTY_INFO						packet;

	ZeroMemory(&packet , sizeof(SG_UNION_PARTY_MEMBER_INFO));
	
	packet.base.set(sizeof(packet),dSG_UNION_PARTY_INFO);
	
	packet.wUnionPartySerial		= _iUnionPartyIndex;
	packet.wUnionPartyIndex			= _iUnionPartyMemberIndex;
	packet.wEffectivenessCount		= _iEffectivenessCount;
	
	for (int i = 0 ; i < _iEffectivenessCount ; i++)
	{
		strcpy(packet.Info[i].m_strName ,		lpUnionParty->m_apMemberParty[_iUnionPartyMemberIndex]->m_aMember[i].m_strName);
		packet.Info[i].m_wHP				=	lpUnionParty->m_apMemberParty[_iUnionPartyMemberIndex]->m_aMember[i].m_bf8RestHP;
		packet.Info[i].m_dwState			=	lpUnionParty->m_apMemberParty[_iUnionPartyMemberIndex]->m_aMember[i].getInfoForUnionPartyState();
		packet.Info[i].m_wUnionPartySerial	=	_iUnionPartyMemberIndex;				//연합 파티 시리얼이라기 보다는 자기 자신의 위치를 나타낸다.
	}
	
	g_userPM.add(_iTargetClientSerial ,&packet,packet.base.wSize);
	
	return TRUE;
}

//단순한 연합 관련 메세지는 여기서 보내도록 하자.
//연합 파티 관련 정보를 클라이언트에게 보낸다.
BOOL
cUnionPartyManager::sendUnionPartyEtcInfo(WORD _wWork ,cParty* _lpParty , WORD _Value0, WORD _Value1, WORD _Value2, WORD _Value3, 
									   WORD _Value4, WORD _Value5, BOOL _sendAllMember)
{
	SG_UNION_PARTY_WORK	packet;
	ZeroMemory(&packet , sizeof(packet));
	
	packet.base.set(sizeof(packet),_wWork);	//패킷 종료를 세팅한다.
	packet.wValue[0] =	_Value0;			
	packet.wValue[1] =	_Value1;			
	packet.wValue[2] =	_Value2;			
	packet.wValue[3] =	_Value3;			
	packet.wValue[4] =	_Value4;			
	packet.wValue[5] =	_Value5;			

	int iMemberCount = _lpParty->getMemberCount();	
	if (_sendAllMember)
	{
		for (int i = 0 ; i < iMemberCount;i++)
		{
			cACTOR	*lpActor	=	_lpParty->getActorByMemberIndex(i);
			
			if	(!lpActor)
				continue;
			
			g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
		}
	}
	else
	{
		//파티장에게만 보낸다.
		cACTOR	*lpActor	=	_lpParty->getActorByMemberIndex(0);

		if	(!lpActor)
			return FALSE;

		g_userPM.add(lpActor->m_iClientSerial,&packet,packet.base.wSize);
	}

	return TRUE;
}

//파티 연합 메니져에 맵버를 추가한다.
BOOL
cUnionPartyManager::joinUnionPartyMember(int _iUnionPartyIndex,cParty *_lpMember)
{
	if(!_lpMember)
		return FALSE;

	cUnionParty*	lpUnionParty = get(_iUnionPartyIndex);

	if(!lpUnionParty)
		return FALSE;

	//연합 파티의 시리얼을 세팅한다.
	_lpMember->m_wUnionPartyserial = lpUnionParty->insertParty(_lpMember);

	return TRUE;

}

cUnionParty*
cUnionPartyManager::getByName(char *_lpstrName)
{
	mCS(m_csUnionPartyWork);

	return	NULL;
}


BOOL
cUnionParty::isCorrectUnionParty()
{
	DWORD		dwAllMember		=	0;			//파티원수 파티수(디스 커넥트 유저는 치지 않는다.)
	DWORD		dwCorrectParty	=	0;			//적합한 파티수(2명 이상의 파티원을 보유한 파티들.)

	for (int i = 0 ; i < dUNION_PARTY_COUNT ; i++)
	{
		dwAllMember = 0;

		for (int j = 0 ; j < dPARTY_MAXPLAYER ; j++)
		{
			if(m_apMemberParty[i] == NULL)		//파티가 체크
				break;

			cACTOR* lpActor = m_apMemberParty[i]->getActorByMemberIndex(j);

			if(!lpActor)						// 파티원 체크
				continue;

			cPARTY_MEMBER		*lpMemberInfo	=	m_apMemberParty[i]->getMemberInfoByName(lpActor->m_strName);

			if(!lpMemberInfo)					//	멤버 정보 체크 
				continue;

			if(lpMemberInfo->m_bf1IsDisconnected)
				continue;

			dwAllMember++;
		}
	
		if(dwAllMember >= 2)				//멥버가 2명이 상이다.
			dwCorrectParty++;				//적합한 파티수 증가.
	}

	if(dwCorrectParty >= 2)				//멥버가 2명이 상이다.
		return TRUE;

	return FALSE;
}

void	
cUnionParty::syncUnionPartyMemberHP(cACTOR* _lpActor)
{	
	for (int i=0;i<dUNION_PARTY_COUNT;i++)
	{
		cParty	*lpParty	=	getPartyByIndex(i);
		
		if	(!lpParty)
			continue;

		for (int j = 0 ; j <dPARTY_MAXPLAYER ;j++)
		{
			cACTOR	*lpMember	=	lpParty->getActorByMemberIndex(j);

			if(!lpMember)
				continue;

			cParty	*lpTempParty	=	_lpActor->getParty();
			
			if(lpTempParty == NULL)
				return;

			g_pUnionpartyManager->sendUnionPartyMemberInfo(m_wSerial , _lpActor->getPartySerial() , 
				lpTempParty->getMemberIndex(_lpActor->m_strName) , lpMember->m_iClientSerial);	
		}
	}
	
}

void	
cUnionParty::dissolutionUnionParty(WORD _wBecause)
{
	for (int i = 0 ; i < getPartyCount() ; i++)
	{
		if(m_apMemberParty[i]->m_wSerial == 0xffff)
			break;

		m_apMemberParty[i]->m_wUnionPartyserial = 0xffff;
		m_apMemberParty[i]->releaseUnionParty();

		for (int j = 0 ; j < m_apMemberParty[i]->getMemberCount() ; j++)
		{
			cACTOR	*lpActor	=	g_game.getActor(m_apMemberParty[i]->m_aMember[j].m_iZoneSerial);	
		
			if(!lpActor)	
				break;

			lpActor->sendUnionPartyDissolution(_wBecause);
		}
	}
}


void	
cUnionPartyManager::dissolutionUnionParty(int _iPartySerial , WORD _wBecause)
{
	cUnionParty*	lpUnionparty	=	m_den.get(_iPartySerial);

	if(!lpUnionparty)
		return;

	lpUnionparty->dissolutionUnionParty(_wBecause);
}

void	
cUnionPartyManager::CreateUnionPartyResultWork(WORD _wUnionPartySerial , cParty* _lpAskerParty , cParty* _lpReplayerParty)
{
	cUnionParty* lpUnionParty = g_pUnionpartyManager->m_den.notCheckedget(_wUnionPartySerial);

// 	if(lpUnionParty != NULL)
// 		return;

	if(!_lpAskerParty)
		return;
	
	if(!_lpReplayerParty)
		return;

	lpUnionParty->CreateUnionParty(_lpAskerParty , _lpReplayerParty);

	_lpAskerParty->m_wUnionPartyIndex		=	0;				//Index는 자기 단위단의 목차이고 Serial은 상위 단위의 목차로 사용햇습니다.//파티연합에서는
	_lpReplayerParty->m_wUnionPartyIndex	=	1;

	lpUnionParty->m_wSerial = _wUnionPartySerial;
	_lpAskerParty->m_wUnionPartyserial = _wUnionPartySerial;
	_lpReplayerParty->m_wUnionPartyserial = _wUnionPartySerial;

	_lpAskerParty->setUnionMember();
	_lpReplayerParty->setUnionMember();
		  
	//여기까지 오면 파티연합은 결성 된거다...-_-;
				  
	//dSG_REPLAY_JOIN_UNION_PARTY 패킷 종류 , lpParty 가입자의 파티 , TRUE 승락 여부 , 
	sendUnionPartyEtcInfo(dSG_REPLAY_JOIN_UNION_PARTY , _lpAskerParty , TRUE , 2);
	sendUnionPartyEtcInfo(dSG_REPLAY_JOIN_UNION_PARTY , _lpReplayerParty , TRUE , 2);
					
	//연합 파티원의 정보를 전달합니다.
	for (int i = 0 ; i < _lpAskerParty->getMemberCount() ; i++)
	{
		cACTOR	*lpActor = _lpAskerParty->getActorByMemberIndex(i);
					  
		if (!lpActor)
			break;
						
		//자신의 파티원 정보 전송.
		sendUnionPartyInfo(_wUnionPartySerial , _lpAskerParty->m_wUnionPartyIndex , _lpAskerParty->getMemberCount() , lpActor->m_iClientSerial);
		sendUnionPartyInfo(_wUnionPartySerial , _lpReplayerParty->m_wUnionPartyIndex , _lpReplayerParty->getMemberCount() , lpActor->m_iClientSerial);
	}
						  
	for (i = 0 ; i < _lpReplayerParty->getMemberCount() ; i++)
	{
		cACTOR	*lpActor = _lpReplayerParty->getActorByMemberIndex(i);
							
		if (!lpActor)
			break;
							  
		sendUnionPartyInfo(_wUnionPartySerial , _lpAskerParty->m_wUnionPartyIndex , _lpAskerParty->getMemberCount() , lpActor->m_iClientSerial);
		sendUnionPartyInfo(_wUnionPartySerial , _lpReplayerParty->m_wUnionPartyIndex , _lpReplayerParty->getMemberCount() , lpActor->m_iClientSerial);
	}
	
}
