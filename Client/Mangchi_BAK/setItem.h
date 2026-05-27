#ifndef _classSetItem_h
#define	_classSetItem_h

#include "cITEM.H"

class	cSetItem	:	public cSetItemDefine
{
public:
							cSetItem()
							{
								m_pItemComment	=	NULL;

								setComment("valid");
							}
							~cSetItem()
							{
								pKILL(m_pItemComment);
							}
};

class	cItemSet	:	public cItemSetDefine
{
public:
	WORD			m_wCommentSize;					//	아이템 설명의 사이즈(텍스트는 따로 모아서 보관한다. 너무 많다. --)
	char			*m_pItemComment;
	
	cItemSet()
	{
		m_pItemComment	=	NULL;
	}

	~cItemSet()
	{
		pKILL(m_pItemComment);
	}

	void			reset();
	void			copy(cItemSet *_lpGroup);


	cSetItem*		getItem(int _iItemIndex);
	cUniqueData*	getSetEffect(int _iSetCount,int _iEffectIndex);
	BOOL			isAvailSetEffect(int _iSetCount,int _iEffectIndex);

	BOOL			isFullSetEffectCount(int _iSetCount);
	BOOL			isFullItem();
	int				getSetEffectCount(int _iSetCount=-1);
	int				getItemCount();

	void			replaceItem(int _iIndex,cSetItem *_lpItem);
	BOOL			addItem(cSetItem *_lpItem);
	BOOL			removeItem(int _iIndex);
	void			setName(char *_lpstrName);
	void			replaceSetEffect(int _iCount,int _iEffectIndex,cUniqueData *_lpEffect);
	BOOL			addSetEffect(int _iCount,cUniqueData *_lpEffect);
	BOOL			removeSetEffect(int _iCount,int _iIndex);

	BOOL			isDefault();
	BOOL			isSame(cItemSet *_lpGroup, BOOL bIsByteCompare = TRUE);

	void			setComment(char *_strComment)
	{
		pKILL(m_pItemComment);

		if (!_strComment)
		{
			m_wCommentSize	=	strlen("valid")+1;

			m_pItemComment	=	new char [m_wCommentSize];

			strcpy(m_pItemComment,"valid");

			return;
		}

		m_wCommentSize	=	strlen(_strComment)+1;
		m_pItemComment	=	new char [m_wCommentSize];

		strcpy(m_pItemComment,_strComment);
	}
};

class	cItemSetManager
{
public:
	cDEN<cItemSet>			m_den;

							cItemSetManager()
							{
								init();
								reset();
							}
	inline	void			init()			{m_den.init(dMAX_SET_ITEM_GROUP_COUNT);}
	inline	void			reset()			{m_den.reset();}
	inline	int				getCount()		{return m_den.getCount();}
	inline	int				getMaxCount()	{return	m_den.getMaxCount();}
	inline	cItemSet*		getBay()		{return	m_den.getList();}
	inline	void			rebuild()		{m_den.rebuild();}

	void					copy(cItemSetManager *_lpData);
	void					build();
	int						add(cItemSet *_lpItemSet);
	void					replace(int _iSerial,cItemSet *_lpItemSet);

	cItemSet*				getByName(char *_lpstrName);
	inline	int				getLastIndex()	{return	m_den.getLastIndex();}
	inline	BOOL			remove(int _iIndex){return	m_den.remove(_iIndex);}
	inline	cItemSet*		get(int _iIndex){return	m_den.get(_iIndex);}
	inline	cItemSet*		getFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);

		return	m_den.getNext();
	}
	inline	void			setFirst(int _iSerial = 0)
	{
		m_den.setFirst(_iSerial);
	}
	inline	cItemSet*		getNext()
	{
		return	m_den.getNext();
	}
};

extern	char	*g_strItemReversionMethod[];

extern	cItemSetManager	g_itemSetManager;

#endif
