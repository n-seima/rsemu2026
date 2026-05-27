#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "winsock2.h"
#include "cPACKET_TOOLLOGSERVER.h"

struct CNode
{
	PACKET_COMMON		data;		//	필요한 데이타 형으로 수정할 것 .!!
	CNode *	pNext;
	CNode() { pNext = NULL; }
};

class CLinkedList
{
public:	
	CLinkedList();
	~CLinkedList();
	bool AddNode(PACKET_COMMON nData);
	bool NewAddNode(PACKET_COMMON nData);
	bool DeleteNode(PACKET_COMMON * nData);
	bool NewDeleteNode(PACKET_COMMON * nData);
	bool DeleteNode(int index);
	bool NewDeleteNode(int index);
	void Reset();

	PACKET_COMMON * GetFirst();
	PACKET_COMMON * GetLast();
	PACKET_COMMON * Get(int index);
	
protected:
	CNode *m_pLinkedList;
};

#endif	// __LINKEDLIST_H