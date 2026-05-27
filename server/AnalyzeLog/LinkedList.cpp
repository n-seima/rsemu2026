#include "cPACKET_LOGSERVER.h"
#include "LinkedList.h"

CLinkedList::CLinkedList()
{
	m_pFirstNode	= NULL;
}

CLinkedList::~CLinkedList()
{
	Reset();
}

void CLinkedList::Reset()
{
	while (m_pFirstNode)
	{
		CNode *pTemp = m_pFirstNode;
		m_pFirstNode = m_pFirstNode->pNext;
		delete pTemp;
	}
}

ALL_LOGMSG * CLinkedList::GetFirst()
{
	CNode **ppNode = &m_pFirstNode;	// get the address of your linked list head
	return &(*ppNode)->data;
}

ALL_LOGMSG * CLinkedList::GetLast()
{
	CNode *pNode = m_pFirstNode;
	while (pNode->pNext)
		pNode = pNode->pNext;
	return &pNode->data;
}

ALL_LOGMSG * CLinkedList::Get(int index)
{
	CNode *pNode = m_pFirstNode;

	while(index>0)
	{
		--index;
		pNode = pNode->pNext;
	}

	return &pNode->data;
}

CNode*
CLinkedList::AddNode(ALL_LOGMSG nData)
{
	if	(!m_pFirstNode)				// first case, when the head of the linked list is Null
	{
		m_pFirstNode	= new CNode;

		if	(!m_pFirstNode)
			return false;

		// work with your newly created node
		memcpy(&m_pFirstNode->data,&nData,sizeof(ALL_LOGMSG));
		m_lpTailNode	=	m_pFirstNode;

		return m_pFirstNode;
	}

	m_lpTailNode->pNext	=	new CNode;		// you have to create your linked list in the pNext variable

	if	(!m_lpTailNode->pNext)
		return NULL;

	CNode	*lpRookieNode	=	m_lpTailNode->pNext;			// this step is to go to the actual node to work with it
	
	// work with your newly created node
	memcpy(&lpRookieNode->data,&nData,sizeof(ALL_LOGMSG));

	m_lpTailNode			=	lpRookieNode;

	return lpRookieNode;
}

bool
CLinkedList::DeleteNode(ALL_LOGMSG * nData)
{
	if	(nData == NULL)	
		return false;

	if	(!m_pFirstNode)	// first case to check the list empty
		return false;

	if	(&m_pFirstNode->data == nData)	// second case to check for the deleted node if it is the head
	{
		CNode *pConnectAgain = m_pFirstNode->pNext;	// you need to hold on to your pNext
		delete m_pFirstNode;
		m_pFirstNode	=	pConnectAgain;					// you need to connect it again.

		return true;
	}

	CNode *pNode	=	m_pFirstNode;

	while(pNode->pNext)					// third case, your traversing would always be in terms of the pNext
	{
		if	(&(pNode->pNext)->data == nData)	// your checking would always be in terms of the pNext
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

bool
CLinkedList::NewDeleteNode(ALL_LOGMSG * nData)
{
	if(nData == NULL)		return false;
	CNode **ppNode = &m_pFirstNode;	// get the address of your linked list head
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
/*	if (!m_pFirstNode)	// first case to check the list empty
		return false;

	if(index == 0)
	{
		CNode *pConnectAgain = m_pFirstNode->pNext;	// you need to hold on to your pNext
		delete m_pFirstNode;
		m_pFirstNode = pConnectAgain;					// you need to connect it again.
		return true;
	}

	CNode *pNode = m_pFirstNode;
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
}
