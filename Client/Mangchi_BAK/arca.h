#ifndef	_classArca_h
#define	_classArca_h

#include "cITEM.H"
#include "arcaDefine.H"

class	cArcaInfo	:	public cArcaDefine
{
public:
				cArcaInfo()
				{
					reset();
				}

	void		copy(cArcaInfo *_lpInfo);
	void		reset();
	BOOL		isSame(cArcaInfo *_lpArca);
};

class	cArcaManager
{
public:
	cDEN<cArcaInfo>		m_den;

						cArcaManager()
						{
							init();
						}
	inline	void		init()			{m_den.init(dMAX_ARCA_COUNT);}
	inline	void		reset()			{m_den.reset();}
	inline	int			getCount()		{return m_den.getCount();}
	inline	int			getMaxCount()	{return	m_den.getMaxCount();}
	inline	cArcaInfo*	getBay()		{return	m_den.getList();}
	inline	void		rebuild()		{m_den.rebuild();}

	int					add(cArcaInfo *_lpArca);
	void				replace(int _iSerial,cArcaInfo *_lpArca);
	cArcaInfo*			getByName(char *_lpstrName);

	inline	int			getLastIndex()	{return	m_den.getLastIndex();}
	inline	BOOL		remove(int _iIndex){return	m_den.remove(_iIndex);}
	inline	cArcaInfo*	get(int _iIndex){return	m_den.get(_iIndex);}
	inline	cArcaInfo*	getFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);

		return	m_den.getNext();
	}
	inline	void		setFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);
	}
	inline	cArcaInfo*	getNext()
	{
		return	m_den.getNext();
	}

};

extern	cArcaManager	g_arcaManager;

#endif