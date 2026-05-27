// cShareMemory.h: interface for the cShareMemory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSHAREMEMORY_H__8C4B9855_9433_4EB2_BAB1_8C8C79AFB12E__INCLUDED_)
#define AFX_CSHAREMEMORY_H__8C4B9855_9433_4EB2_BAB1_8C8C79AFB12E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
//#include <assert.h>

template<class T> class CShareMemory;

template<class T>
class CSMemoryNode {

friend class CShareMemory<T>;

private:
	T       item;
	DWORD   next;

public:
	CSMemoryNode(const T & itm = NULL)
	{
		next = NULL;
	}
};

template<class T>
class CShareMemory
{
protected:     
	//**** properties ****
	HANDLE      m_hMemMap;
	LPBYTE      m_pMapBuff;
	HANDLE      m_hMutex;
	BOOL        m_bCreate;

private:
	DWORD m_dwHead, m_dwTail;
	WORD m_nSize, m_nCount;
	CSMemoryNode<T> *GetNewNode();

public:
	//**** class method ****
	//**** constructor / desturctor 
	CShareMemory();
	~CShareMemory();

	BOOL Create(const char* name, WORD size = 1);
	BOOL Open(const char* name);

	//check the list
	int ItemCount();
	BOOL isEmpty();

	//traversal & Retrieve
	BOOL InsertItem(const T & itm);
	BOOL DeleteHead(T & itm);
	BOOL GetHead(T & itm);
	BOOL FindAndClear(const T & itm);
	BOOL Find(const T & itm);
	void ClearMemory();
};

/*
template<class T>
CSMemoryNode<T>::CSMemoryNode(const T & itm) : item(itm) 
{	next = NULL;
}
*/

#endif // !defined(AFX_CSHAREMEMORY_H__8C4B9855_9433_4EB2_BAB1_8C8C79AFB12E__INCLUDED_)
