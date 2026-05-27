//
//	제작 일시	2005.11.30
//
//	목적		공통이 되는 아주 기본적인 선언
//
//	제작		이영찬
//

#pragma	once


#define	_lnk_common_define_h

#ifdef	WIN32
#include	<wtypes.h>
#endif
#include	<tchar.h>
#include	<assert.h>

//	DEV 모드 이상에서 작동하는 매크로
#ifdef	_DEV
	#ifndef  _DEBUG
		#ifdef  __cplusplus
		extern "C" {
		#endif

		_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);

		#ifdef  __cplusplus
		}
		#endif
	#endif

	#define dev_assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
	#define dev_print(_Expression) dPrint(_Expression)

#else
	#ifndef  _DEBUG
		#define	dev_assert(_Expression)	((void)0)
		#define dev_print(_Expression) ((void)0)
	#else
		#define	dev_assert	assert
		#define dev_print(_Expression) dPrint(_Expression)
	#endif
#endif

//유니코드 프로젝트를 처리하기 위해서
#ifdef UNICODE 
	#define dL	L
	#define dT(a)	_T(a)
#else
	#define dL
	#define dT(a)	a
#endif

#ifdef	_LNK_CC
#pragma warning( disable : 4996 ) 
#pragma warning( disable : 4819 ) 
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

//	update.txt파일이 없어서 이곳에 기록한다. written by jihyun, 4th, Sep 2006 , modified by jihyun 18, Sep 2006
//	win64는 long을 4byte로 유지한다. 하여.. 64또한 8byte자료형은 LONGLONG으로 선언한다.
#if	defined(WIN32) || defined(WIN64)
	typedef signed long		sl8;
	typedef unsigned long	ul8;
	typedef LONGLONG		s8;
	typedef ULONGLONG		u8;
#else
	typedef signed int		sl8;
	typedef unsigned int	ul8;
	typedef signed	 long	s8;
	typedef unsigned long	u8;
#endif

//	클래스 소멸자를 불러 온다.
template<class T>
void
ClearClass(T* _lpInstance)
{
	_lpInstance->T::~T();

    new(_lpInstance) T();
};

//	클래서 생성자를 불러 온다.
template<class T>
void
InitClass(T* _lpInstance)
{
    new(_lpInstance) T();
};

//	객체를 0으로 초기화. 이름은 ZeroMemory가 더 좋은데.. -_-
template<class T>
void
ClearMemory(T* _lpInstance)
{
	ZeroMemory(_lpInstance,sizeof(T));
};

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
	inline void getSystemTime(SYSTEMTIME *_pDest)
	{
		_pDest->wYear = year + 2000;
		_pDest->wMonth = month;
		_pDest->wDay = day;
		_pDest->wHour = hour;
		_pDest->wMinute = minute;
		_pDest->wSecond = second;
	}
	inline void setSystemTime(SYSTEMTIME *_pSource)
	{
		if(_pSource->wYear < 2063){
			year = _pSource->wYear % 100;
		}else{
			year = 0;
		}
		month = _pSource->wMonth;
		day = _pSource->wDay;
		hour = _pSource->wHour;
		minute = _pSource->wMinute;
		second = _pSource->wSecond;
	}
}unTime;

//	연월일시분초를 4바이트에 때려 넣음 유효기간 2000~2063년
typedef union _union_minute_time
{
    struct 
	{
		u4	year		:	12;	//	연 한계 수치 63년. 2063 이후에는 못 쓴다. 그때까지 쓸리도 없고 쓴다고 해도 내나이 80이 넘는다. 알아서 고치겠지. 우하하
		u4	month		:	4;	//	월
		u4	day			:	5;	//	일
		u4	hour		:	5;	//	시
		u4	minute		:	6;	//	분
    };

    struct 
	{
	    u4	m_dwValue;
    };

	inline	void	reset()
	{
		m_dwValue	=	0;
	}
	inline	void	copy(_union_minute_time *_lpTime)
	{
		m_dwValue	=	_lpTime->m_dwValue;
	}
}	unTimeMinute;

//	swap을 어디선가 사용하는게 있어서 Swap으로 사용 -_-
template <class A> inline void Swap(A &a, A &b)
{
	A   c;

	c = a;
	a = b;
	b = c;
}

#ifdef	UNICODE
#define	STRICMP		_tcsicmp
#define	STRCPY		_tcscpy_s
#define	STRCAT		_tcscat_s
#define	STRNCPY		_tcsncpy_s
#define	STRCMP		_tcscmp
#define	SPRINTF		_stprintf_s
#define	STRLEN		_tcslen
#define	VSPRINTF_S	_vstprintf_s
#define	STRSTR		_tcsstr
#else
#define	STRICMP		stricmp
#define	STRCPY		strcpy
#define	STRNCPY		strncpy
#define	STRCMP		strcmp
#define	SPRINTF		sprintf
#define	STRLEN		strlen
#define	VSPRINTF_S	vsprintf_s
#define	STRCAT		strcat
#define	STRSTR		strstr
#endif

template <class T>
class	_cStaticData
{
public:
	void	_resetStaticData()
	{
		memset(this,0,sizeof(T));
	}
	void	_copyStaticData(const T* _lpData)
	{
		memcpy(this,((T*)_lpData)->_getStaticDataPtr(),sizeof(T));
	}
	void*	_getStaticDataPtr()
	{
		return	this;
	}
};

typedef struct _D3DVECTOR_POS {
    float x;
    float y;
    float z;
} D3DVECTOR_POS;

typedef struct _D3DMATRIX_LNK {
	float m[4][4];
} D3DMATRIX_LNK;

typedef struct _D3DQUATERNION_LNK {
	float x, y, z, w;
} D3DQUATERNION_LNK;


#include	<string>
#include	<map>
#include	<vector>

// 유니코드에 대한 STL의 string
#if defined _UNICODE  ||  defined UNICODE
	typedef std::wstring	tstring;
#else
	typedef std::string		tstring;
#endif

typedef	std::map<int,tstring>	MapIntString;	//	일반적으로 많이 쓰는 Int,스트링 맵
typedef	std::pair<int,tstring>	PairIntString;	//
typedef	MapIntString::iterator	IterIntString;	//

typedef	std::map<tstring,int>	MapStringInt;	//	일반적으로 많이 쓰는 스트링,Int 맵
typedef	std::pair<tstring,int>	PairStringInt;
typedef	MapStringInt::iterator	IterStringInt;	

typedef	std::map<int,int>		MapIntInt;		//	일반적으로 많이 쓰는 Int 맵
typedef	std::pair<int,int>		PairIntInt;
typedef	MapIntInt::iterator		IterIntInt;	

typedef	std::vector<int>		VecInt;
typedef	VecInt::iterator		IterInt;

typedef	std::vector<tstring>	VecString;
typedef	VecString::iterator		IterString;

// Release / Delete 매크로
#ifndef	SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p); (p)=NULL; } }
#endif

#ifndef	SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete [] (p); (p)=NULL; } }
#endif

#ifndef	SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#endif
