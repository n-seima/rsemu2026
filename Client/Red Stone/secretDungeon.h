#ifndef	_classSecretDungeon_h
#define	_classSecretDungeon_h

#include "SFC.h"
#include "secretDungeonDefine.h"

#define	dNAME_LENGTH								18
#define	dINSTANCE_FIELD_UPKEEPTIME_IN_VALID_PLAYER	3
#define	dMAX_IF_VALUE_COUNT							50
#define	dMAX_IF_COUNT								256
#define	dPARTY_MEMBER_COUNT							8
#define	dINSTANCE_FIELD_DEATH_COUNTER				30
#define	dINSTANCE_FIELD_COMPLETE_COUNTER			60

#define	dINSTANCE_FIELD_WARNING_STEP_COUNT			4

class	cSecretDungeon	:	public	cSecretDungeonDefine
{
public:
	void					reset(){}
	void					copy(cSecretDungeon *_lpDungeon){}
};

class	cSecretDungeonManager
{
public:
	cDen<cSecretDungeon>		m_den;

	inline	void				init(int _iCount)	{m_den.init(_iCount);}
	inline	void				reset()				{m_den.reset();}
	inline	int					getCount()			{return m_den.getCount();}
	inline	int					getMaxCount()		{return	m_den.getMaxCount();}
	inline	cSecretDungeon*		getBay()			{return	m_den.getList();}
	inline	cSecretDungeon*		get(int _iIndex)	{return	m_den.get(_iIndex);}
	inline	void				rebuild()			{m_den.rebuild();}
};


extern	cSecretDungeonManager	g_sdManager;

#endif