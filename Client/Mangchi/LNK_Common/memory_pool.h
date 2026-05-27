//	------------------------------------------------------------------------------------------
//
//	제작 일시	2005.11.30
//
//	목적		메모리 관리자(메모리 풀)
//
//	제작		이영찬
//
//	기본적으로 풀 형태의 메모리이다. 여기서 쌓기만 하는 메모리의 경우 낭비가 심해 전위 스택,후위 스택이 가능하게 해준다.
//	전위 스택 : 앞에서 부터 차곡차곡 쌓이는 메모리 구조
//	후위 스택 : 뒤에서 부터 쌓이는 메모리 구조

//	주의!! malloc을 사용하기 때문에 클래스 생성자나 소멸자가 동작하지 않음.
//	개선책을 찾다가 시간 관계상 냅둠. 적절한 방법 아시는분은 개선해 주세요.
//	------------------------------------------------------------------------------------------

//	------------------------------------------------------------------------------------------
//	사용 예
//
//	초기화 부분..
//	...
//	
//	-----------------------일반적인 사용
//	cMemoryPool	mp;
//
//	mp.init(1024*1024*10,"오브젝트 이미지");	//	메모리를 설정하고 이름을 설정해 준다.
//	char	*pMem	=	mp.alloc(1024);	메모리 할당
//	이렇게 할당된 메모리는 개별 초기화는 안되고 한번에 해제 하는것만 가능하다.
//	
//	mp.reset();	//	할당된 메모리를 모두 초기화 한다.
//	mp.close();	//	메모리를 더 이상 쓸일이 없으면 해제한다.
//	...
//
//	-----------------------전위와 후위를 나눠서 사용
//	...
//	
//
//	cMemoryPool	mpField,lpmpObject,lpmpMonster;	//	오브젝트와 몬스터는 mpField의 메모리를 참조만 하기 때문에 앞에 lp를 붙였다.

//	mpField.init(1024*1024*10,"몬스터&오브젝트");
//	lpmpObject.init(&mp,cMemoryPool::eStackFront,"몬스터");
//	lpmpMonster.init(&mp,cMemoryPool::eStackRear,"건물,오브젝트 이미지");

//	mpObject는 mp의 메모리 풀에서 앞에서 부터 메모리를 사용하고
//	mpMonster는 mp의 메모리 풀에서 뒤에서 부터 메모리를 사용하게 된다.
//	붉은 보석의 경우 필드 이동시 mpField를 초기화 하고 오브젝트와 몬스터를 따로 관리한다.
//	일반적으로 전투 필드와 마을은 필요한 내용이 다르기 때문에 이런식의 할당이 유용하다.

#ifndef	_lnk_common_memory_pool_h
#define	_lnk_common_memory_pool_h

#include	"lnk_define.h"

class cMemoryPool		//	Memory Control Unit
{
public:
	enum
	{
		eStackFront		=	0	,
		eStackRear				,
		eClientHead	=	0xa5c2b4d3,
		eClientTail	=	0xaec2f4c3,
		eNameLength	=	32	,	//	이름 길이
		eMemoryAllign=	8	//	메모리 정렬
	};

protected:
	s4					m_s4StackMethod;	//	스택 방법
	u4					m_u4FullSize;	//	전체 사이즈
	u4					m_u4FrontPointer,m_u4RearPointer;	//	전위 스택 포인트,후위 스택 포인트
	u4					m_u4TempAlloc;

	char				*m_pPool;		//	실제 메모리 풀
	cMemoryPool			*m_lpReference;	//	

	char				m_strName[eNameLength];	//	메모리 관리자 이름

public:
						cMemoryPool();
	virtual				~cMemoryPool();

	bool				init(u4 _u4Size,char *_lpstrName=NULL);
	//	메모리 풀 초기화
	bool				init(cMemoryPool *_lpMcu,s4 _s4StackMethod,char *_lpstrName=NULL);
	//	전위 스택,후위 스택을 좀 더 편하게 쓰기 위해 사용
	void				reset();	//	초기화,반드시!! 먼저 메모리 풀을 참조한 포인터들을 초기화 시켜 줄것
	void				close();	//	메모리 완전 해제

	bool				checkMemory(void *_lpPtr);

#ifdef	dUSE_SAFE_MEMORY
	void*				alloc(u4 _u4Size,char *_lpstrName=NULL,s4 _s4StackMethod=eStackFront);
//	_s4StackMethod 는 init(cMemoryPool *_lpMcu... 로 설정한 경우 무시하고 설정된 값으로 처리한다.
	void*				tempAlloc(u4 _u4Size,char *_lpstrName=NULL);	//	잠깐 쓰고 버리는 메모리. 한번에 하나밖에 할당 할 수 없다.
	void				check(u4 address);
#else
	void*				alloc(u4 _u4Size,s4 _s4StackMethod=eStackFront);
//	_s4StackMethod 는 init(cMemoryPool *_lpMcu... 로 설정한 경우 무시하고 설정된 값으로 처리한다.
	void*				tempAlloc(u4 _u4Size);
#endif
	void				tempFree();

	u4				getRemainMemorySize();	//	남은 메모리
	u4				getUsedMemorySize();	//	사용한 메모리
};

#endif