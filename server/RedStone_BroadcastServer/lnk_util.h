//
//	제작 일시	2006.07.22
//
//	목적		각종 유틸리티,클래스 단위가 아닌 자잘한 함수들을 모았다.
//
//	제작		이영찬
//

#ifndef	_lnk_common_util_h
#define	_lnk_common_util_h

#include	"lnk_define.h"

#ifndef	dIS_REDSTONE_ONE
extern	inline u4	random(u4	_u4Range);	//	랜덤 함수 0x40000000 이상의 수치는 지원하지 않는다.
#endif

#ifdef	WIN32
#include	<windows.h>

#ifndef	mCS
#define	mCS(cs)	cCriticalSection c_s(&cs)	//	크리티컬 섹션 시작
#endif

class	cCriticalSection
{
public:
	CRITICAL_SECTION	*m_lpCs;

	cCriticalSection(CRITICAL_SECTION * _lpCs)
	{
		m_lpCs	=	_lpCs;
		EnterCriticalSection(m_lpCs);
	}

	~cCriticalSection()
	{
		LeaveCriticalSection(m_lpCs);
	}
};
#endif

#endif
