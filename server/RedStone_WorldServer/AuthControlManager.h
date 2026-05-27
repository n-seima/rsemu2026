#pragma once

#include <map>

template<class T>
class cSinglton			
{
public:
	static T & GetInstance()	{	
		static T cMng;	return cMng;
	}

};


class cKeyNameString
{
public:
	std::string		m_key;

	cKeyNameString(const char * _name)	{	m_key = _name;	}

	//	map의 키는 비교 연산자 필요
    bool operator<(const cKeyNameString& other) const
    {
		return m_key.c_str()[0] < other.m_key.c_str()[0];
    };

};

class cAuthInfo
{
	enum
	{
		eGAPLIMIT	 = 60000,
	};
public:
	char	strID[dID_LENGTH];
	DWORD	checkTime;

	cAuthInfo(){}
	cAuthInfo(const char * _id){	strncpy( strID, _id, dID_LENGTH-1); checkTime = timeGetTime();	}
	BOOL	isGabage()
	{
		if((timeGetTime() - checkTime)>=0)
		{
			if( (timeGetTime() - checkTime) >= eGAPLIMIT)
			{	
				return TRUE;	
			}
		}else
		if( (0xffffffff-checkTime) >=eGAPLIMIT)
		{	
			return TRUE;	
		}

		return FALSE;
	}
};

class cAuthControlManager	:	public cSinglton<cAuthControlManager>
{
private:
	std::map<cKeyNameString , cAuthInfo>	m_mapAuth;	//	느리긴 하지만 일단 해결해두자.

public:
	void	add(const char * id, const char * name)
	{
		
		cAuthInfo	info(id);
		m_mapAuth[ cKeyNameString(name) ]  = info;
	}

	BOOL	find(const char * id, const char * name)
	{
		std::map<cKeyNameString , cAuthInfo>::iterator it = m_mapAuth.find( cKeyNameString(name) );
		if(it == m_mapAuth.end())
			return FALSE;
		if(it->second.isGabage())
		{
			m_mapAuth.erase( it );
			return FALSE;
		}

		if(strncmp(it->second.strID , id, dID_LENGTH-1) == 0)
		{
			m_mapAuth.erase( it );
			return TRUE;
		}
		m_mapAuth.erase( it );
		return FALSE;
	}

};

#define dAuthCtlMng		cAuthControlManager::GetInstance()