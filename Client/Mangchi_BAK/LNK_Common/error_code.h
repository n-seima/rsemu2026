//
//	제작 일시	2005.11.30
//
//	목적		공통 에러 코드 관리자
//
//	제작		이영찬
//

//	사용 예
//

//	초기화 부분..
//	...
//	g_ecm.init(char *_lpstrErrorCodeFileName=NULL);	//	초기화를 한다. 아직 파일이름은 아무것도 안한다.
//	...

//	에러 발생!
//	...
//	관련 에러 코드가 있는지 확인해 보고 없다면 추가하자. 뒤에 메시지는 안 넣어도 된다.
//	g_ecm.addError(eEC_CouldNotFoundFile,"oops! can not found file '%s'",_lpstrFileName);	//	에러 추가(SetLastError도 사용)
//
//	귀찮으면 그냥 메시지만 추가하자. 다만, 한국에서만 쓸건 아니니 가급적이면 영문으로 하는게 좋다.
//	g_ecm.addError("oops! it's damaged pos4er!! '%s'",_lpstrDamagedPos4erName);	//	에러 코드 없이 에러 메시지만 추가 한다.
//

//	에러 체크. 메인 루프 안에 넣고 체크 하자.
//	s4	iErrorCount;
//	g_ecm.getErrorCount(&iErrorCount);	//	현재까지 에러 수를 얻어 온다.
//
//	에러가 있다면

//	while(iErrorCount)
//	{
//		u4	dwErrorCode	=	g_ecm.getLastError(iErrorCount);	//	에러를 얻는다.
//		--iErrorCount;
//	}
//	getLastErrorMessage();	//	에러 메시지를 얻는다. 인자가 없을 경우 마지막 에러 리턴


#ifndef	_lnk_common_error_code_h
#define	_lnk_common_error_code_h

#include	"lnk_define.h"

enum
{
//	일반적인 에러			10000~11000
	eEC_Valid							=	10000	,

//	파일 관련 에러			11001~12000
	eEC_CouldNotFoundFile				=	11001,

//	메모리 관련 에러		12001~13000
	eEC_Memory_DamageSafeMemoryHeader	=	12001,
	eEC_Memory_DamageSafeMemoryTail		,
};

class	cErrorCodeManager
{
public:
	enum
	{
		eMaxErrorCount			=	100,
		eMaxErrorMessageLength	=	256,
	};

protected:
	static	s4	s_s4IsExist;	//	싱글톤 구현-_-a

	class	cErrorCode
	{
	public:
		s4		m_s4Code;
		char	m_strMessage[eMaxErrorMessageLength];
	};

	s4			m_s4ErrorCount,m_s4BeginPos;	//	에러 수,에러 시작 위치
	cErrorCode	m_aErrorList[eMaxErrorCount];	//	에러 리스트
	char		m_strTextBuffer[1024];		//	에러 메시지를 추가하기 위한 텍스트 버퍼

	s4			_getErrorSlot(s4 _s4Index=-1);	//	실제 에러가 저장된 슬롯을 리턴
public:

				cErrorCodeManager();
	virtual		~cErrorCodeManager();

	bool		init(char *_lpstrErrorCodeFileName=NULL);	//	초기화를 한다.
	void		reset();

	void		addError(s4 _s4ErrorCode,char *_lpstrErrorMessage=NULL,...);	//	에러 추가(SetLastError도 사용)
	void		addError(char *_lpstrErrorMessage,...);	//	에러 추가

	s4			getErrorCount(s4 *_lps4TotalErrorCount=NULL);	//	현재까지 에러 수를 얻어 온다.
	s4			getLastError(s4 _s4Index=-1);	//	에러를 얻는다. 인자가 없을 경우 마지막 에러 리턴
	char*		getLastErrorMessage(s4 _s4Index=-1);//	에러 메시지를 얻는다. 인자가 없을 경우 마지막 에러 리턴
};

extern	cErrorCodeManager	g_ecm;	//	error code manager

#endif