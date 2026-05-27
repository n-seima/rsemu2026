#ifndef _classPieceItem_h
#define	_classPieceItem_h

#include "cITEM.H"

class	cPieceItem	:	public cPieceItemDefine
{
public:
	void			reset();
	void			copy(cPieceItem *_lpItem);
	BOOL			generateItem(cITEM	*_lpItem);
};

class	cPieceItemManager
{
public:
	cDEN<cPieceItem>		m_den;

							cPieceItemManager()
							{
								init();
								reset();
							}

	inline	void			init()			{m_den.init(dMAX_PIECE_ITEM_COUNT);}
	inline	void			reset()			{m_den.reset();}
	inline	int				getCount()		{return m_den.getCount();}
	inline	int				getMaxCount()	{return	m_den.getMaxCount();}
	inline	cPieceItem*		getBay()		{return	m_den.getList();}
	inline	void			rebuild()		{m_den.rebuild();}

	void					build();
	int						add(cPieceItem *_lpPiece);
	void					replace(int _iSerial,cPieceItem *_lpPiece);
	cPieceItem*				getByName(char *_lpstrName);

	inline	int				getLastIndex()	{return	m_den.getLastIndex();}
	inline	BOOL			remove(int _iIndex){return	m_den.remove(_iIndex);}
	inline	cPieceItem*		get(int _iIndex){return	m_den.get(_iIndex);}
	inline	cPieceItem*		getFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);

		return	m_den.getNext();
	}
	inline	void			setFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);
	}
	inline	cPieceItem*		getNext()
	{
		return	m_den.getNext();
	}

};

extern	cPieceItemManager	g_pieceItemManager;

#endif