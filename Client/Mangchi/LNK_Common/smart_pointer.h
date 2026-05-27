//
//	제작 일시	2005.12.03
//
//	목적		스마트 포인터
//
//	제작		이영찬
//
//	... GPG에서 퍼오다 시피 함.
//	기본 개념은 new 로 할당된 후 delete를 안 해줘도 스스로 소멸하는 클래스

// 사용예
//	cTest 클래스
//	class cTest	: public cSmartPointer
//	{
//		DECLARE_SMART_POINTER(cTest);
//		//	클래스 내용
//	};
//
//	cTest::SP	test	=	new cTest;
//

#ifndef	_lnk_common_smart_pointer_h
#define	_lnk_common_smart_pointer_h

#include	"lnk_define.h"

/// 스마트 포인터 최상위 클래스
class cSmartPointer
{
public:
			cSmartPointer()
			:m___iRefCount(0)
			{
			}

	virtual ~cSmartPointer()
			{
			}

//	직접 호출 절대 금지!!
	void	__addRef()	//	이름이 중복되지 않게 하기 위해 __ 삽입
	{
		++m___iRefCount;
	}
	void	__release()	//	이름이 중복되지 않게 하기 위해 __ 삽입
	{
		if	(m___iRefCount	>=	0	&&	--m___iRefCount	<=	0)
			delete this;
	}

private:
	int				m___iRefCount;
	//	참조 횟수 - 이녀석이 0이 되면 자폭한다.(상속 받아도 다른 멤버와 겹치지 않게 이렇게 작명함)
};

#define DECLARE_SMART_POINTER(_self) \
public: \
	typedef ctSmartPointer<_self> SP;	//	작명 규칙에 어긋나기는 하는데, 사용할때 편의를 생각해 이렇게 정의함

// <주의 사항>
//      1) 반드시 동적 생성된 개체만을 가리킬 것
//      2) class T는 __addRef()와 __release()를 반드시 구현할 것(cSmartPointer 상속 받아도 되고 만들어도 됨)
//      3) 다중 스레딩 환경에서는 신중히 사용할 것
//
// <올바른 사용>
//		class T { void __addRef(); void __release(); };
//
//      1) 동적 생성된 개체를 가리킴
//         ctSmartPointer<T> p1(new T), p2 = new T;
//
//      2) 여러개의 포인터가 동적 생성된 개체의 주소를 공유
//         ctSmartPointer<T> p1, p2;
//         T* p = new T;
//         p1 = p2 = p;
//
//         ctSmartPointer<T> p1, p2;
//         T* p = new T;
//         p1 = p;
//         p2 = p;
//
// <잘못된 사용>
//			class T {};
//
//      1) 스택 상의 정적인 개체를 가리키는 경우 에러!
//         T t;
//         ctSmartPointer<T> p1(&t), p2 = &t;
//
//      2) 배열을 가리키는 경우 메모리 누수 발생!
//         ctSmartPointer<T> p = new T[100];
template<typename T>
class ctSmartPointer
{
public:
	ctSmartPointer();
	ctSmartPointer(const ctSmartPointer<T>& _rp);
	ctSmartPointer(T* _ptr);
	~ctSmartPointer();

	ctSmartPointer<T>& operator=(const ctSmartPointer<T>& _rp);
	ctSmartPointer<T>& operator=(T* _ptr);

	T&		operator*() const;
	T*		operator->() const;
			operator T*() const;

	bool	operator==(const ctSmartPointer<T>& _rp) const;
	bool	operator==(const T* _lpPtr) const;
	bool	operator!=(const ctSmartPointer<T>& _rp) const;
	bool	operator!=(const T* _lpPtr) const;
	bool	operator!( void ) const ;

private:
	T*	m_pPtr;
};

template<typename T>
inline
ctSmartPointer<T>::ctSmartPointer()
{
	m_pPtr	=	0;
}

template<typename T>
inline
ctSmartPointer<T>::ctSmartPointer(const ctSmartPointer<T>& _rp)
{
	m_pPtr	=	_rp.m_pPtr;

	if	(m_pPtr)
		m_pPtr->__addRef();
}

template<typename T>
inline
ctSmartPointer<T>::ctSmartPointer(T* _lpPtr)
{
	if	(_lpPtr)
	{
		m_pPtr	=	_lpPtr;
		m_pPtr->__addRef();
	}
	else
		m_pPtr	=	0;
}

template<typename T>
inline
ctSmartPointer<T>::~ctSmartPointer()
{
	if	(m_pPtr)
		m_pPtr->__release();
}

template<typename T>
inline
ctSmartPointer<T>& ctSmartPointer<T>::operator=(const ctSmartPointer<T>& _rp)
{
	if	(m_pPtr == _rp.m_pPtr)
		return *this;

	if	(m_pPtr)
		m_pPtr->__release();
	m_pPtr	=	_rp.m_pPtr;

	if	(m_pPtr)
		m_pPtr->__addRef();

	return *this;
}

template<typename T>
inline
ctSmartPointer<T>& ctSmartPointer<T>::operator=(T* _lpPtr)
{
	if	(m_pPtr == _lpPtr)
		return *this;
	if	(m_pPtr)
		m_pPtr->__release();
	if	(_lpPtr)
	{	
		m_pPtr = _lpPtr;
		m_pPtr->__addRef();
	}
	else
		m_pPtr = 0;

	return *this;
}

template<typename T>
inline T&
ctSmartPointer<T>::operator*() const
{
	return *m_pPtr;
}

template<typename T>
inline T*
ctSmartPointer<T>::operator->() const
{
	return m_pPtr;
}

template<typename T>
inline
ctSmartPointer<T>::operator T*() const
{
	return m_pPtr;
}

template<typename T>
inline bool
ctSmartPointer<T>::operator==(const ctSmartPointer<T>& _rp) const
{
	return m_pPtr == _rp.m_pPtr;
}

template<typename T>
inline bool
ctSmartPointer<T>::operator==(const T* _lpPtr) const
{
	return m_pPtr == _lpPtr;
}

template<typename T>
inline bool
ctSmartPointer<T>::operator!=(const ctSmartPointer<T>& _rp) const
{
	return m_pPtr	!=	_rp.m_pPtr;
}

template<typename T>
inline bool
ctSmartPointer<T>::operator!=(const T* _lpPtr) const
{
	return m_pPtr	!=	_lpPtr;
}

template<typename T>
inline bool
ctSmartPointer<T>::operator!( void ) const 
{
	return !m_pPtr;
}

#endif