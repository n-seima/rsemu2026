// cListManage.h: interface for the cListManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLISTMANAGE_H__6582597B_FFC6_450B_A59C_E06D2FAC9E1F__INCLUDED_)
#define AFX_CLISTMANAGE_H__6582597B_FFC6_450B_A59C_E06D2FAC9E1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream.h>

const int defaultSize = 100;

template <class T>
class cListManage  
{
private:
	T *pType;
	int	size;

public:
	cListManage(int defaultsize=defaultSize);
	cListManage(const cListManage &rhs);
	~cListManage(){	delete [] pType; pType = NULL; }

	cListManage& operator=(const cListManage&);
	T& operator[](int offset){	return pType[offset];	}
	const T& operator[](int offset) const {	return pType[offset];	}

	int		getSize() const {return size;}
};

#endif // !defined(AFX_CLISTMANAGE_H__6582597B_FFC6_450B_A59C_E06D2FAC9E1F__INCLUDED_)
