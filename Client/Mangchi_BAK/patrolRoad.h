#ifndef	_patrolRoad_h
#define	_patrolRoad_h

#include "SFC.H"

#define	dMAX_PATROL_ROAD_NODE_COUNT	20
#define	dMAX_PATROL_ROAD_COUNT		256

class	cPatrolRoadNode
{
public:
	WORD	x,y;
};

class	cPatrolRoad
{
public:
	WORD			m_wSerial;
	WORD			m_wStayTime;
	WORD			m_wIsReturnRoad;
	WORD			m_wNodeCount;
	cPatrolRoadNode	m_aNode[dMAX_PATROL_ROAD_NODE_COUNT];
	char			m_strName[32];

					cPatrolRoad();

	void			reset();
	void			copy(cPatrolRoad *_lpRoad);
	void			draw(BOOL _bIsEditRoad=FALSE,BOOL _bIsSelectRoad=FALSE);
	BOOL			addNode(int _iX,int _iY);
	void			putNode(int _iIndex,BOOL _bIsEditRoad,BOOL _bIsSelectRoad);
};

class	cPatrolRoadManager
{
public:
	int						m_iSelect;
	cDEN<cPatrolRoad>		m_den;

							cPatrolRoadManager();

	inline	void			init()				{m_den.init(dMAX_PATROL_ROAD_COUNT);}
	inline	void			reset()				{m_den.reset();}
	inline	int				getCount()			{return m_den.getCount();}
	inline	int				getMaxCount()		{return	m_den.getMaxCount();}
	inline	cPatrolRoad*	getBay()			{return	m_den.getList();}
	inline	void			rebuild()			{m_den.rebuild();}
	inline	int				getLastIndex()		{return	m_den.getLastIndex();}

	inline	cPatrolRoad*	getFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);

		return	m_den.getNext();
	}
	inline	void			setFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);
	}
	inline	cPatrolRoad*	getNext()
	{
		return	m_den.getNext();
	}

	cPatrolRoad*			get(int _iIndex=-1);
	BOOL					remove(int _iSerial=-1);
	void					build();
	void					draw();
	int						add(cPatrolRoad *_lpRoad,BOOL _bIsCheckName=FALSE);
	BOOL					replace(int _iSerial,cPatrolRoad *_lpRoad);
};

extern	cPatrolRoad	g_editRoad;

#endif