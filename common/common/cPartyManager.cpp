// cPartyManager.cpp: implementation of the cPartyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "cPartyManager.h"
#include <stdio.h>

//****************************************************************************************
//	Class	Party
//****************************************************************************************
cParty::cParty()
{
	InitializeCriticalSection(&csParty);
	init();
}

cParty::~cParty()
{
	DeleteCriticalSection(&csParty);
}

void	cParty::GetNextEmpty()
{
	for(WORD l_wCount = 0;l_wCount<dPARTY_MAXPLAYER;l_wCount++)
		if(m_stackpp[l_wCount].wSerial == 0xffff)
			break;
	if(l_wCount<dPARTY_MAXPLAYER)	m_wNext = l_wCount;
	else							m_wNext = 0xffff;

}

void	cParty::open(char *name,WORD serial)
{
	CCritical	CS(&csParty);
	init();
	wSerial = serial;
	strcpy(strName,name);
	m_isOpenParty = TRUE;
}

void	cParty::close()
{
	CCritical	CS(&csParty);

	init();
}

WORD	cParty::add(char *name,WORD gss,WORD ws,WORD gs)
{
	CCritical	CS(&csParty);
	WORD	wResult = 0xffff;
	GetNextEmpty();
	if(m_wNext==0xffff)		return 0xffff;
	wResult = m_wNext;
	m_stackpp[wResult].bMoveMap = FALSE;
	m_stackpp[wResult].wGameServerSerial = gss;
	m_stackpp[wResult].wWorldSerial = ws;
	m_stackpp[wResult].wSerialInGame = gs;
	m_stackpp[wResult].wSerial = wResult;
	strcpy(m_stackpp[wResult].strName,name);
	m_wCount++;

	if(m_wCount == 1)		m_wMaster = wResult;
	if(m_wMaster>wResult)	m_wMaster = wResult;

	return wResult;
}

BOOL	cParty::remove(WORD idx,char * _strName)
{
	CCritical	CS(&csParty);
	BOOL	bResult = FALSE;

	if(idx<dPARTY_MAXPLAYER)
	{
		m_stackpp[idx].close();
		m_wCount--;
		bResult = TRUE;
	}else if(_strName)
	{
		for(WORD l_wCount = 0;l_wCount<dPARTY_MAXPLAYER;l_wCount++)
		{
			if(STRICMP(m_stackpp[l_wCount].strName,_strName)==0)
			{
				idx = l_wCount;
				m_stackpp[l_wCount].close();
				m_wCount--;
				bResult = TRUE;
				break;
			}
		}
	}
	if(bResult)
	{
		if(idx == m_wMaster)
		{	m_wMaster = 0xffff;
			for(WORD l_wCount = 0;l_wCount<dMAX_PARTY_COUNT;l_wCount++)
			{
				if(m_stackpp[l_wCount].wSerial!=0xffff)
				{
					m_wMaster = l_wCount;
					break;
				}
			}
		}
	}
	return bResult;
}

cPartyPlayer *	cParty::GetPP(WORD idx)
{
	CCritical	CS(&csParty);
	if(idx==0xffff)							return NULL;
	if(m_stackpp[idx].wSerial != 0xffff)	return &m_stackpp[idx];
	else									return NULL;
}

cPartyPlayer *  cParty::GetPP(char * name)
{
	CCritical	CS(&csParty);

	for(WORD l_wCount = 0;l_wCount<dPARTY_MAXPLAYER;l_wCount++)
	{
		if(STRICMP(m_stackpp[l_wCount].strName,name)==0)
		{
			return &m_stackpp[l_wCount];
		}
	}
	return NULL;
}

void	cParty::SetName(char *name)
{	
	CCritical	CS(&csParty);

	memcpy(&strName,name,dPARTYNAME_LENGTH);
}

void	cParty::SetIsOpen(BOOL	open)
{	
	CCritical	CS(&csParty);

	m_isOpenParty = open;
}

WORD	cParty::GetClientSerial(WORD idx)
{	
	CCritical	CS(&csParty);

	if(m_stackpp[idx].wSerial == 0xffff)	return 0xffff;
	else									return m_stackpp[idx].wSerialInGame;
}

WORD	cParty::GetMasterSerial()
{
	CCritical	CS(&csParty);

	return m_wMaster;
}

//****************************************************************************************
//	Class	Party	Manager
//****************************************************************************************
cPartyManager::cPartyManager()
{	
	InitializeCriticalSection(&csPartyManager);
	Init();
}

cPartyManager::~cPartyManager()
{
	DeleteCriticalSection(&csPartyManager);
}

cPartyManager & cPartyManager::GetInstance()
{	static cPartyManager cmanager;
	return cmanager;
}

void	cPartyManager::Init()
{	
	CCritical	CS(&csPartyManager);

	for(int i = 0;i < dMAX_PARTY_COUNT; i++)
		m_stack[i].init();
	m_wNext = 0;
	m_wCount = 0;
}

void	cPartyManager::GetNextEmpty()
{
	for(WORD l_wCount = 0;l_wCount<dMAX_PARTY_COUNT;l_wCount++)
		if(m_stack[l_wCount].wSerial == 0xffff)
			break;
	if(l_wCount<dMAX_PARTY_COUNT)	m_wNext = l_wCount;
	else							m_wNext = 0xffff;

}

WORD	cPartyManager::Add(char *name)
{
	CCritical	CS(&csPartyManager);
	GetNextEmpty();
	WORD	wResult = m_wNext;
	if(wResult ==  0xffff)		return 0xffff;

	m_stack[wResult].open(name,wResult);
	m_wCount++;

	return wResult;
}

BOOL	cPartyManager::Del(WORD idx)
{
	CCritical	CS(&csPartyManager);
	
	if(m_stack[idx].wSerial!=0xffff)
	{
		m_stack[idx].close();
		m_wCount--;
		return TRUE;
	}else
		return FALSE;
}

BOOL	cPartyManager::Del(char *name)
{
	CCritical	CS(&csPartyManager);
	
	for(WORD l_wCount = 0;l_wCount<dMAX_PARTY_COUNT;l_wCount++)
		if(strcmp(m_stack[l_wCount].strName,name)==0)
		{
			m_stack[l_wCount].close();
			m_wCount--;
			return TRUE;
		}
	return FALSE;
}

cParty		*cPartyManager::GetPartyPnt(char *name)	//	ID로 cParty 포인터 얻기
{
	CCritical	CS(&csPartyManager);

	for(WORD l_wCount = 0;l_wCount<dMAX_PARTY_COUNT;l_wCount++)
		if(strcmp(m_stack[l_wCount].strName,name)==0)
		{
			return &m_stack[l_wCount];
		}
	return NULL;
}
