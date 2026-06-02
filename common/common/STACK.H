#ifndef __STACK_H
#define __STACK_H

#include <process.h>

//	T에 사용할 '그것'은 close(), copy(T *), wSerial을 지원해야 한다.

template <class T>
class	cSTACK
{
protected:
	T					*m_pStack;
	WORD				m_wMax;		//	최대 가질수 있는 갯수
	WORD				m_wLast;	//	마지막으로 추가된 번호
	WORD				m_wNext;	//	다음에 추가될 번호 
	WORD				m_wCount;	//	몇개가 사용중일까?
	CRITICAL_SECTION	cs;
		
public:
	cSTACK()
	{
		InitializeCriticalSection(&cs);
		m_pStack		=	NULL;
		m_wMax			=	0;
		m_wLast			=	0;
		m_wNext			=	0;
		m_wCount		=	0;
	}
	~cSTACK(){	close();	DeleteCriticalSection(&cs);	}
	inline	WORD	getCount(){	return m_wCount;	}
	inline	T *		get(WORD c)
	{	if(c>=m_wMax)	return NULL;
		else			return &m_pStack[c];	
	}

	void	init(WORD _max)
	{
		EnterCriticalSection(&cs);
		if(m_pStack){	delete [] m_pStack;	m_pStack = NULL;	}
		m_pStack		=	new T[_max];
		m_wMax			=	_max;
		m_wLast			=	0;
		m_wNext			=	0;
		m_wCount		=	0;
		LeaveCriticalSection(&cs);
	}

	void	close()
	{
		EnterCriticalSection(&cs);
		if(m_pStack){	delete [] m_pStack;	m_pStack = NULL;	}
		m_wMax			=	0;
		m_wLast			=	0;
		m_wNext			=	0;
		m_wCount		=	0;
		LeaveCriticalSection(&cs);
	}

	inline void reset()
	{
		EnterCriticalSection(&cs);
		for(WORD i=0;i<m_wMax;i++)	m_pStack[i].close();
		m_wLast			=	0;
		m_wNext			=	0;
		m_wCount		=	0;
		LeaveCriticalSection(&cs);
	}

	WORD	add(T * _lpData)
	{
		EnterCriticalSection(&cs);
		if(m_wCount>=m_wMax || m_wNext==0xffff){	LeaveCriticalSection(&cs);	return 0xffff;	}
		//	class  T에 추가 작업을 한다.
		WORD	wStart = 0; 
		m_pStack[m_wNext].copy(_lpData);
		m_pStack[m_wNext].wSerial = wStart = m_wNext;
		m_wCount++;	//if(m_wCount> m_iMax){	}
		m_wNext = 0xffff;
		//	다음 next를 찾는다.
		WORD	l_iCount = 0;
		for(l_iCount = wStart+1;l_iCount<m_wMax;l_iCount++)
			if(m_pStack[l_iCount].wSerial >= 0xffff){	m_wNext = l_iCount;		break;	}
		if(m_wNext == 0xffff)
		{
			for(l_iCount = 0;l_iCount<wStart;l_iCount++)
				if(m_pStack[l_iCount].wSerial >= 0xffff){	m_wNext = l_iCount; break;	}
		}
		LeaveCriticalSection(&cs);
		return (WORD)m_pStack[wStart].wSerial;
	}

	BOOL	remove(WORD	_wSerial,BOOL	_isCheck = TRUE)
	{
		EnterCriticalSection(&cs);
		if(_wSerial>=m_wMax){	LeaveCriticalSection(&cs);	return FALSE;	}
		if(_isCheck && m_pStack[_wSerial].wSerial >= 0xffff){	LeaveCriticalSection(&cs);	return FALSE;	}
		m_wCount--;
		m_pStack[_wSerial].close();
		if(m_wNext == 0xffff)	m_wNext = _wSerial;
		LeaveCriticalSection(&cs);
		return TRUE;
	}
};

#endif	//	__STACK_H