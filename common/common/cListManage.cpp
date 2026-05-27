// cListManage.cpp: implementation of the cListManage class.
//
//////////////////////////////////////////////////////////////////////

#include "cListManage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
template <class T>
cListManage<T>::cListManage(int defaultsize=defaultSize):
size(defaultsize)
{
	pType = new T[defaultsize];
	for(int i=0;i<defaultsize;i++)	pType[i] = 0;
}

template <class T>
cListManage<T>::cListManage(const cListManage &rhs)
{
	size = rhs.getSize();
	pType = new T[size];
	for(int i=0;i<size;i++)	pType[i] = rhs[i];
}

template <class T>
cListManage<T>& cListManage<T>::operator =(const cListManage &rhs)
{
	if(this == &rhs)	return *this;
	delete [] pType;	pType = NULL;
	size = rhs.getSize();
	pType = new T[size];
	for(int i=0;i<size;i++)	pType[i] = rhs[i];
	return *this;
}