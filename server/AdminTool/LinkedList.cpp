#include "LinkedList.h"

CLinkedList::CLinkedList()
{
	m_pLinkedList	= NULL;
}

CLinkedList::~CLinkedList()
{
	Reset();
}

void CLinkedList::Reset()
{
	while (m_pLinkedList)
	{
		CNode *pTemp = m_pLinkedList;
		m_pLinkedList = m_pLinkedList->pNext;
		delete pTemp;
	}
}

PACKET_COMMON * CLinkedList::GetFirst()
{
	CNode **ppNode = &m_pLinkedList;	// get the address of your linked list head
	return &(*ppNode)->data;
}

PACKET_COMMON * CLinkedList::GetLast()
{
	CNode *pNode = m_pLinkedList;
	while (pNode->pNext)
		pNode = pNode->pNext;
	return &pNode->data;
}

PACKET_COMMON * CLinkedList::Get(int index)
{
	CNode *pNode = m_pLinkedList;
	while(index>0)
	{
		--index;
		pNode = pNode->pNext;
	}
	return &pNode->data;
}

bool CLinkedList::AddNode(PACKET_COMMON nData)
{
	if (!m_pLinkedList)				// first case, when the head of the linked list is Null
	{
		m_pLinkedList	= new CNode;
		if (!m_pLinkedList)
			return false;
		// work with your newly created node
		memcpy(&m_pLinkedList->data,&nData,sizeof(PACKET_COMMON));
		return true;
	}
	
	CNode *pNode = m_pLinkedList;	// second case, where you search for the first pNext
	// that is Null to fill it in.
	while (pNode->pNext)
		pNode = pNode->pNext;
	
	pNode->pNext = new CNode;		// you have to create your linked list in the pNext variable
	if (!pNode->pNext)
		return false;
	pNode = pNode->pNext;			// this step is to go to the actual node to work with it
	
	// work with your newly created node
	memcpy(&pNode->data,&nData,sizeof(PACKET_COMMON));

	return true;
}

bool CLinkedList::NewAddNode(PACKET_COMMON nData)
{
	CNode **ppNode = &m_pLinkedList;	// get the address of your linked list head
	while (*ppNode)
		ppNode = &(*ppNode)->pNext;		// traverse by moving the address to the next pNext
	
	*ppNode = new CNode();				// create the node right in the pNext, without worrying 
										// about your conventional two cases described above
	if (!*ppNode)
		return false;
	
	// work with your newly created node, without the conventional step of going to the pNext pointer
	memcpy(&(*ppNode)->data,&nData,sizeof(PACKET_COMMON));

	return true;
}

bool CLinkedList::DeleteNode(PACKET_COMMON * nData)
{
	if(nData == NULL)		return false;
	if (!m_pLinkedList)	// first case to check the list empty
		return false;

	if (&m_pLinkedList->data == nData)	// second case to check for the deleted node if it is the head
	{
		CNode *pConnectAgain = m_pLinkedList->pNext;	// you need to hold on to your pNext
		delete m_pLinkedList;
		m_pLinkedList = pConnectAgain;					// you need to connect it again.
		return true;
	}

	CNode *pNode = m_pLinkedList;

	while (pNode->pNext)					// third case, your traversing would always be in terms of the pNext
	{
		if (&(pNode->pNext)->data == nData)	// your checking would always be in terms of the pNext
		{
			CNode *pDeleteNode = pNode->pNext;
			pNode->pNext = pNode->pNext->pNext;	// alot of pNext pointers, don't you think ?
			delete pDeleteNode;
			return true;
		}
		pNode = pNode->pNext;
	}

	return false;
}

bool CLinkedList::NewDeleteNode(PACKET_COMMON * nData)
{
	if(nData == NULL)		return false;
	CNode **ppNode = &m_pLinkedList;	// get the address of your linked list head
	while (*ppNode)
	{
		if (&(*ppNode)->data == nData)	// you work with your current node and not confusing pNext nodes
		{
			CNode *pDeleteNode = (*ppNode);
			(*ppNode) = (*ppNode)->pNext;	// you don't need to work with pNext->pNext
			delete pDeleteNode;
			return true;
		}
		ppNode = &((*ppNode)->pNext);
	}
	// you have only one case here, compared with conventional 3 cases for the normal linked list delete !
	return false;
}


bool CLinkedList::DeleteNode(int index)
{
	return DeleteNode(Get(index));
/*	if (!m_pLinkedList)	// first case to check the list empty
		return false;

	if(index == 0)
	{
		CNode *pConnectAgain = m_pLinkedList->pNext;	// you need to hold on to your pNext
		delete m_pLinkedList;
		m_pLinkedList = pConnectAgain;					// you need to connect it again.
		return true;
	}

	CNode *pNode = m_pLinkedList;
	while(index>1 && pNode->pNext)
	{
		--index;	pNode = pNode->pNext;
	}
	if(index!=0)	return false;
	CNode *pDeleteNode = pNode->pNext;
	pNode->pNext = pNode->pNext->pNext;
	delete pDeleteNode;
	return true;
*/
}

bool CLinkedList::NewDeleteNode(int index)
{
	return NewDeleteNode(Get(index));
/*	CNode **ppNode = &m_pLinkedList;	// get the address of your linked list head
	while (*ppNode && index>1)
	{
		--index;
		ppNode = &((*ppNode)->pNext);
	}
	if(index!=0)	return false;
	CNode *pDeleteNode = (*ppNode);
	(*ppNode) = (*ppNode)->pNext;	// you don't need to work with pNext->pNext
	delete pDeleteNode;
	return true;
*/
}
