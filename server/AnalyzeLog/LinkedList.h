#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

union ALL_LOGMSG;

struct CNode
{
	ALL_LOGMSG		data;		//	필요한 데이타 형으로 수정할 것 .!!
	CNode *	pNext;
	CNode() { pNext = NULL; }
};

class CLinkedList
{
public:	
	CLinkedList();
	~CLinkedList();
	CNode*	AddNode(ALL_LOGMSG nData);
	bool DeleteNode(ALL_LOGMSG * nData);
	bool NewDeleteNode(ALL_LOGMSG * nData);
	bool DeleteNode(int index);
	bool NewDeleteNode(int index);
	void Reset();

	ALL_LOGMSG * GetFirst();
	ALL_LOGMSG * GetLast();
	ALL_LOGMSG * Get(int index);
	
	CNode	*m_pFirstNode;
	CNode	*m_lpTailNode;
protected:
};

#endif	// __LINKEDLIST_H