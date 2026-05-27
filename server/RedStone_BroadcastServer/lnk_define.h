//
//	제작 일시	2005.11.30
//
//	목적		공통이 되는 아주 기본적인 선언
//
//	제작		이영찬
//

#ifndef	_lnk_common_define_h
#define	_lnk_common_define_h

#ifdef	WIN32
#include	<wtypes.h>
#endif

#ifndef	pKILL
	#define pKILL(p) if (p) {delete [] p	;p=NULL;}	// 포인터 삭제..
#endif

#ifndef	KILL
	#define KILL(p)	if (p) {delete p		;p=NULL;}	// 클래스 삭제..
#endif

#ifndef	NULL
	#define NULL	0
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

typedef signed	 char	s1;
typedef unsigned char	u1;
typedef signed	 short	s2;
typedef unsigned short	u2;
typedef signed	 int	s4;
typedef unsigned int	u4;

#ifdef	WIN32
typedef LONGLONG		s8;
typedef ULONGLONG		u8;
#else
typedef signed	 long	s8;
typedef unsigned long	u8;
#endif

//	클래스 초기화 클래스 소멸자를 불러 온다.
template<class T>
void
ClearClass(T* _lpInstance)
{
	_lpInstance->T::~T();

    new(_lpInstance) T();
};

#ifndef	dIS_REDSTONE_ONE

//	연월일시분초를 4바이트에 때려 넣음 유효기간 2000~2063년
typedef union _union_time
{
    struct 
	{
		u4	year		:	6;	//	연 한계 수치 63년. 2063 이후에는 못 쓴다. 그때까지 쓸리도 없고 쓴다고 해도 내나이 80이 넘는다. 알아서 고치겠지. 우하하
		u4	month		:	4;	//	월
		u4	day			:	5;	//	일
		u4	hour		:	5;	//	시
		u4	minute		:	6;	//	분
		u4	second		:	6;	//	초
    };

    struct 
	{
	    u4	m_dwValue;
    };

	inline	void	reset()
	{
		m_dwValue	=	0;
	}
	inline	void	copy(_union_time *_lpTime)
	{
		m_dwValue	=	_lpTime->m_dwValue;
	}
}	unTime;

//	swap을 어디선가 사용하는게 있어서 Swap으로 사용 -_-
template <class A> inline void Swap(A &a, A &b)
{
	A   c;

	c = a;
	a = b;
	b = c;
}
#endif

#endif