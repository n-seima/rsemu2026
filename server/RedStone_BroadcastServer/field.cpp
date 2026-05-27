#include	"field.h"
#include	"operate_packet.H"

BOOL		cFieldManager::s_bIsExist	=	FALSE;

cFieldManager	g_fieldManager;

void
cFieldInfo::reset()
{
	m_wSerial			=	0xffff;
	m_wActiveUserCount	=	0;
}

BOOL
cFieldInfo::addUser(int _iSerial)
{
	for	(int i=0;i<m_wActiveUserCount;i++)
	{
		if	(m_awUserSerial[i]	==	_iSerial)
			return	TRUE;
	}
	
	if	(m_wActiveUserCount	>=	c_iMaxUserCountInField)
		return	FALSE;

	m_awUserSerial[m_wActiveUserCount++	]	=	_iSerial;

	return	FALSE;
}

void
cFieldInfo::removeUser(int _iSerial)
{
	for	(int i=m_wActiveUserCount;i;)
	{
		--i;

		if	(m_awUserSerial[i]	==	_iSerial)
		{
			for	(int j=i;j<m_wActiveUserCount-1;j++)
				m_awUserSerial[j]	=	m_awUserSerial[j+1];

			m_awUserSerial[m_wActiveUserCount-1]	=	0xffff;
			m_wActiveUserCount--;

			return;
		}
	}
}

void
cFieldInfo::copy(cFieldInfo *_lpInfo)
{
	memcpy(this,_lpInfo,sizeof(cFieldInfo));
}

void
cFieldInfo::sendPacketToClient(void *_lpPacket)
{
	int		iSize	=	((cMSG_BASE_TYPE *)_lpPacket)->wSize;
	int		iRemoveActorCount	=	0;
	WORD	awRemoveActorList[c_iMaxUserCountInField];
	int		i;

	for(i=m_wActiveUserCount;i;)
	{
		--i;

		CClient	*lpUser	= mCM.GetClientPnt(m_awUserSerial[i]);

		if	(!lpUser	||	lpUser->m_wSerial == 0xffff || lpUser->m_isGameServer)
			continue;

		if	(lpUser->m_wCurrentField	!=	m_wSerial)
		{
			lpUser->setFieldSerial(lpUser->m_wCurrentField);

			awRemoveActorList[iRemoveActorCount++]	=	m_awUserSerial[i];

			continue;
		}

		lpUser->AddSendPacket((char *)_lpPacket,iSize);
	}

	for(i=iRemoveActorCount;i;)
	{
		--i;

		removeUser(awRemoveActorList[i]);
	}
}

cFieldManager::cFieldManager()
{
	if	(s_bIsExist)
	{
		ERRMSG("error!!","cFieldManager::cFieldManager()");

		return;
	}

	m_denField.init(c_iMaxFieldCount);

	//	파티 리스트 전부를 받기 끝
	s_bIsExist					=	TRUE;

	InitializeCriticalSection(&m_csFieldWork);

	reset();

}

cFieldManager::~cFieldManager()
{
	DeleteCriticalSection(&m_csFieldWork);
}

cFieldInfo*
cFieldManager::get(int _iSerial)
{
	if	(_iSerial	<	0	||	_iSerial	>=	c_iMaxFieldCount)
		return	NULL;

	return	m_denField.get(_iSerial);
}

void
cFieldManager::reset()
{
	m_denField.reset();

	for (int i=0;i<dGUILD_MAX_COUNT;i++)
	{
		cFieldInfo	fieldInfo;

		fieldInfo.reset();
		fieldInfo.m_wSerial	=	i;

		m_denField.add(&fieldInfo);
	}
}

void
cFieldManager::addUser(int _iField,int _iClient)
{
	CCriticalSection	cs(&m_csFieldWork);

	cFieldInfo			*lpField	=	get(_iField);

	if	(lpField)
		lpField->addUser(_iClient);
}

void
cFieldManager::removeUser(int _iField,int _iClient)
{
	CCriticalSection	cs(&m_csFieldWork);

	cFieldInfo			*lpField	=	get(_iField);

	if	(lpField)
		lpField->removeUser(_iClient);
}

void
cFieldManager::sendPacketToClient(int _iField,void *_lpPacket)
{
	CCriticalSection	cs(&m_csFieldWork);

	cFieldInfo			*lpField	=	get(_iField);

	if	(lpField)
		lpField->sendPacketToClient(_lpPacket);
}