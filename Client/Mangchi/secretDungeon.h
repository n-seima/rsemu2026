#ifndef	_classSecretDungeon_h
#define	_classSecretDungeon_h

#include "SFC.H"
#include "secretDungeonDefine.h"

class	cSecretDungeon	:	public	cSecretDungeonDefine
{
public:
	void					reset();
	void					copy(cSecretDungeon *_lpDungeon);
};
class	cSecretDungeonValue
{
public:
	char					m_strDungeonValue[dMAX_DUNGEON_VALUE_COUNT][dDUNGEON_VALUE_TEXT_LENGTH];

	void					reset()	
	{
		for (int i=0;i<dMAX_DUNGEON_VALUE_COUNT;i++)
			sprintf(m_strDungeonValue[i],"value #%.2d",i+1);
	}
	void					copy(cSecretDungeonValue *_lpDungeonValue)
	{
		memcpy(this,_lpDungeonValue,sizeof(cSecretDungeonValue));
	}
};

class	cSecretDungeonManager
{
public:
	cDEN<cSecretDungeon>			m_den;
	cSecretDungeonValue				m_aValueString[dMAX_SECRET_DUNGEON_COUNT];

									cSecretDungeonManager();

	inline	cSecretDungeonValue*	getString(int _iIndex)
	{
		if	(_iIndex	<=0 || _iIndex >= dMAX_SECRET_DUNGEON_COUNT)
			return	NULL;

		return	&m_aValueString[_iIndex];
	}
	inline	void					init()				
	{
		m_den.init(dMAX_SECRET_DUNGEON_COUNT);

		reset();
	}
	inline	void					reset()				
	{
		m_den.reset();
		for (int i=0;i<dMAX_SECRET_DUNGEON_COUNT;i++)
			m_aValueString[i].reset();
	}
	inline	BOOL					remove(int _iSerial){return	m_den.remove(_iSerial);}
	inline	int						getCount()			{return m_den.getCount();}
	inline	int						getMaxCount()		{return	m_den.getMaxCount();}
	inline	cSecretDungeon*			getBay()			{return	m_den.getList();}
	inline	cSecretDungeon*			get(int _iIndex)	{return	m_den.get(_iIndex);}
	inline	void					rebuild()			{m_den.rebuild();}
	inline	int						getLastIndex()		{return	m_den.getLastIndex();}
	void							build();

	inline	cSecretDungeon*			getFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);

		return	m_den.getNext();
	}
	inline	void					setFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);
	}
	inline	cSecretDungeon*			getNext()
	{
		return	m_den.getNext();
	}

	int								add(cSecretDungeon *_lpDungeon,BOOL _bIsCheckNameAndIDCode=FALSE);
	BOOL							replace(int _iSerial,cSecretDungeon *_lpDungeon);
	void							copy(cSecretDungeonManager *_lpSDM);
};

extern	cSecretDungeonManager	g_sdManager;

#endif