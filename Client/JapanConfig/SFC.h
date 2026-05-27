//////////////////////////////////////////////////////////////////////////////////********
//	SFC.H

//	몇가지 유용한 루틴과 몇가지 유용한 매크로
//	MMX레지스터와 FPU레지스터를 이용한 메모리 전송.

//	작성 날짜 : 1999.02.22
//	1차	 갱신 : 1999.03.15	-	* MMX레지스터와FPU레지스터를 이용한 메모리 전송추가
//								* UTIL.H -> GLOBAL.H 로 바꿈.
//	2차	 갱신 : 1999.03.24	-	* cRECT global class를 추가함을 시작으로 전역 클래스 작성.
//	3차  갱신 : 1999.05.14  -   * sine,cosine 테이블 추가
//	4차  갱신 : 2000.06.09	-	* sine,cosine 테이블 제거

//								* cSTRING,cSTRINGS,cFOLDER,cFILE,cRECT,cRECTS,cRECT_SIZE,cRECTS_SIZE,CPos,cPOINTS class
//								추가 및 교정

//								* 압축 클래스 cNUX 추가

//								* _ms,_exportFileName,_exportFolderName,_exportExt,_setExt,_fileSize,IsFolder
//								등의 전역 함수 추가및 교정

//								* 전역 변수 g_strLogFolder 추가(프로그램이 실행된 폴더)

//								* GLOBAL 을 SFC로 바꿈

//	5차  갱신 : 2000.06.22	-	* _fileSize 제거
//								* cFILE를 cFILES로 바꿈
//								* cFILE 화일 제어 클래스 추가
//								* cNUX 화일 병합 기능 추가
//								* IsFile 추가
//								* cFILES 클래스 제거. -.- cFOLDER과 합쳐 버렸다.


/////////////////////////////////////////////////////////////////////////////////*********/

#ifndef _classSFC_H
#define _classSFC_H

#include <winsock2.H>
#include <windows.h>
#include <stdio.h>
#include <dxerr8.h>

#ifdef _DEBUG
#define	dDEBUG_MODE		1
#endif

//#define	dDEBUG_MODE				1

#ifdef dDEBUG_MODE			//	디버깅 모드냐?
#define	TEST(err,method)	if (err) method;				//	err이 체크 되면 method를 실행
#define	CRY(err,message)	if (err) ERRMSG(message);		//	err이 체크 되면 method를 실행
#define	EXPEL(err)			if (err) goto LABEL_EXILE;
#define	DEBUG_ERRMSG		cMSG::Error
#else
#define	TEST(a,b)			(void(0))
#define	CRY(a,b)			(void(0))
#define	EXPEL(err)			(void(0))
#define	DEBUG_ERRMSG		(FALSE)
#endif

#define	WM_DEBUGTEXT			(WM_USER+2)

#define	dGET_FILE_NAME			0x480
#define	dGET_FILE_EXT			0x470

#define	d1MEGA					1048576

#define pKILL(buff) if (buff) {delete [] buff	;buff=NULL;}	// 포인터 삭제..
#define KILL(buff)	if (buff) {delete buff		;buff=NULL;}	// 클래스 삭제..
#undef  ATTEMPT
#define ATTEMPT(x)	{ if (FAILED(x)) goto ERROR_LABEL; }		//	에러 처리..
#undef  RELEASE
#define RELEASE(x)	{ if (x != NULL) {x->Release(); x = NULL;} }//	DX개체 삭제..

#define	MSGOUT		cMSG::Output
#define	DXMSGOUT	cMSG::DxOutput
#define	ERRMSG		cMSG::Error
#define	ERRORED		Errored()
#define	SAFETY		Safety()

template <class A> inline void swap(A &a, A &b)
{   A   c;
    c = a;
    a = b;
    b = c;
}


//	전역 변수들
extern	char		g_strLogFolder[1024];					//	프로그램 실행 폴더
extern	BYTE		*g_pTemplateBuffer;
extern	int			g_iTemplateBufferSize;
extern	int			StringToNumber(char *_strText,int _iLimitValue,int _iDecimal);

#define	dDEFAULT_TEMPLATE_BUFFER_SIZE	(1024*40)	//	40Kb

inline	void
AllocTemplateBuffer(int _iSize)
{
	if (g_iTemplateBufferSize	>=	_iSize)	return;

	_iSize	=	max(_iSize,dDEFAULT_TEMPLATE_BUFFER_SIZE);

	pKILL(g_pTemplateBuffer);

	g_pTemplateBuffer		=	new BYTE [_iSize];

	g_iTemplateBufferSize	=	_iSize;
}

inline	void
FreeTemplateBuffer()
{
	pKILL(g_pTemplateBuffer);
	g_iTemplateBufferSize	=	0;
}

//	전역 함수들
extern	void		memset16(WORD *dest,WORD value,unsigned int size);	//	16비트 용..
extern	inline int	random(int range);									//	랜덤함수
extern	inline BOOL	SafeRange(int value,int minValue,int maxValue);		//	범위 체크
extern	inline void	RotateValue(int &value,int maxValue);				//	크기를 체크해서 크면 0으로..
extern	inline void	ClipValue(int &value,int minValue,int maxValue);	//	크기를 체크해서 자른다.
extern	inline BOOL	SetLogFolder(){return	SetCurrentDirectory(g_strLogFolder);}
extern	char*		GetTimeString(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute,int _iSecond);	//	시간 문자열 얻기

extern	BOOL		_makeShotcut(char* src, char* lnkName, char* lnkPath);	//	숏컷을 만든다.
inline	char* _cdecl _ms(char *str,...)
{	
	static	char	s_tempStr[1024];
	va_list argptr;
	va_start(argptr, str);
	vsprintf(s_tempStr,str,argptr);
	va_end(argptr);

	return s_tempStr;
}

extern	char		*_exportFileName(char *str);			//	화일 이름만 뽑아 낸다.
extern	char		*_exportName(char *str);
extern	char		*_exportFolderName(char *str);			//	폴더 이름만 뽑아 낸다.
extern	char		*_exportExt(char *str);					//	확장자만 뽑아 낸다.
extern	void		_setExt(char *srcStr,char *destStr);	//	확장자를 설정한다.
extern	BOOL		IsFolder(char *fn);						//	이 녀석이 폴더냐?
extern	BOOL		IsFile(char *fn);						//	이 녀석 화일이냐?
extern	BOOL		Safety();								//	안전하다!!
extern	BOOL		Errored();								//	에러가 났다!!
extern	void		EXIT(char *title,char *main,...);					//	걍 끝낸다.
extern	BOOL		ustrncpy(char *dest,char *src,int size);

inline	int
GetRange(int x1,int y1,int x2,int y2)
{	
	x1	-=	x2;
	y1	-=	y2;

	if (x1 >= 0x7fff || x1 <= -0x7fff)	return	0x7ffffff;
	if (y1 >= 0x7fff || y1 <= -0x7fff)	return	0x7ffffff;

	return	(x1*x1+y1*y1);
}

inline	int
GetOvalRange(int x1,int y1,int x2,int y2)
{	
	x1	-=	x2;
	y1	-=	y2;
	y1	*=	2;

	if (x1 >= 0x7fff || x1 <= -0x7fff)	return	0x7ffffff;
	if (y1 >= 0x7fff || y1 <= -0x7fff)	return	0x7ffffff;

	return	(x1*x1+y1*y1);
}

class	cINDEX
{
public:
	int 	Value;
	int 	Index;
};

extern	int Compare( const void *a, const void *b);

class CPos
{
public:
	int		x,y;

	void	Set(int a,int b)	{x =a,y =b;}
	void	Move(int a,int b)	{x+=a,y+=b;}
	BOOL	Match(CPos *pos);
	BOOL	Match(int a,int b);

	void	AdjustScreen(HWND hwnd);
	void	SetMousePos(HWND hwnd);
	void	SetMousePos();

	CPos	operator=(const CPos & );
	CPos	operator+(const CPos & );
	CPos	operator-(const CPos & );
};

class c4POINT : public CPos
{
public:
	int		mx,my;
};

class cPOINTS
{
public:
	short	x,y;

	void	Set(short a,short b)	{x =a,y =b;}
	void	Move(short a,short b)	{x+=a,y+=b;}
	BOOL	Match(CPos *pos);
	BOOL	Match(short a,short b);

	void	AdjustScreen(HWND hwnd);
	void	SetMousePos(HWND hwnd);
	void	SetMousePos();

	cPOINTS	operator=(const cPOINTS & );
	cPOINTS	operator+(const cPOINTS & );
	cPOINTS	operator-(const cPOINTS & );
};

class cRECT
{
public:
	int		x1,y1,x2,y2;

	cRECT	operator=(const cRECT &src)
	{	x1	=	src.x1;
		y1	=	src.y1;
		x2	=	src.x2;
		y2	=	src.y2;

		return src;
	}

	inline	BOOL	isIn(int x,int y)
	{
		if (x < x1) return FALSE;
		if (x > x2) return FALSE;
		if (y < y1) return FALSE;
		if (y > y2) return FALSE;
		
		return TRUE;
	}

	inline	BOOL	isCrash(cRECT *compare)
	{	if (compare->x1 > x2) return FALSE;
		if (compare->x2 < x1) return FALSE;
		if (compare->y1 > y2) return FALSE;
		if (compare->y2 < y1) return FALSE;

		return TRUE;
	}

	inline	void	set(int a1,int b1,int a2,int b2)
	{	x1	=	a1;
		y1	=	b1;
		x2	=	a2;
		y2	=	b2;
	}

	inline	void	set(cRECT *_lpRect)
	{	
		memcpy(this,_lpRect,sizeof(cRECT));
	}

	inline	void	clientFrame(HWND hwnd)
	{	GetClientRect(hwnd,(RECT *)this);
	}

	inline	void	outFrame(HWND hwnd)
	{	GetWindowRect(hwnd,(RECT *)this);
	}

	inline	void	move(int x,int y)
	{	x2	+=	(x-x1);
		y2	+=	(y-y1);
		x1	=	x;
		y1	=	y;
	}

	inline	void	add(int x,int y)
	{	x1	+=	x;
		y1	+=	y;
		x2	+=	x;
		y2	+=	y;
	}

	inline	void	reform()
	{	if (x1 > x2 ) swap(x1,x2);
		if (y1 > y2 ) swap(y1,y2);
	}

	inline	void	expand(int _iXS,int _iYS)	//	사이즈 키우기
	{
		x1	-=	_iXS,y1	-=	_iYS,x2	+=	_iXS,y2	+=	_iYS;
	}

	inline	BOOL	isSame(cRECT *_lpRect)
	{
		if (x1==_lpRect->x1 && y1==_lpRect->y1 && x2==_lpRect->x2 && y2==_lpRect->y2) return TRUE;

		return	FALSE;
	}

	inline	int		getLeft()					{return min(x1,x2);}
	inline	int		getRight()					{return max(x1,x2);}
	inline	int		getTop()					{return min(y1,y2);}
	inline	int		getBottom()					{return max(y1,y2);}

	inline	DWORD	getWidth()					{return getRight()-getLeft()+1;}
	inline	DWORD	getHeight()					{return getBottom()-getTop()+1;}
	inline	DWORD	getSize()					{return getWidth()*getHeight();}

	inline	void	getCenter(int &a,int &b)	{a=getLeft()+getWidth()/2,b=getTop()+getHeight()/2;}
	inline	void	getCenter(CPos	*_lpPos)	{_lpPos->x=getLeft()+getWidth()/2,_lpPos->y=getTop()+getHeight()/2;}
};

class cRECTS
{
public:
	short	x1,y1,x2,y2;

	inline	BOOL	isIn(int x,int y)
	{
		if (x < x1) return FALSE;
		if (x > x2) return FALSE;
		if (y < y1) return FALSE;
		if (y > y2) return FALSE;
		
		return TRUE;
	}

	inline	BOOL	isCrash(cRECTS *compare)
	{	if (compare->x1 > x2) return FALSE;
		if (compare->x2 < x1) return FALSE;
		if (compare->y1 > y2) return FALSE;
		if (compare->y2 < y1) return FALSE;

		return TRUE;
	}

	inline	void	set(short a1,short b1,short a2,short b2)
	{	x1	=	a1;
		y1	=	b1;
		x2	=	a2;
		y2	=	b2;

		if (x1 > x2) swap(x1,x2);
		if (y1 > y2) swap(x1,x2);
	}

	inline	void	move(short x,short y)
	{	x2	+=	(x-x1);
		y2	+=	(y-y1);
		x1	=	x;
		y1	=	y;
	}

	inline	void	add(short x,short y)
	{	x1	+=	x;
		y1	+=	y;
		x2	+=	x;
		y2	+=	y;
	}

	inline	void	reform()
	{	if (x1 > x2 ) swap(x1,x2);
		if (y1 > y2 ) swap(y1,y2);
	}

	inline	cRECTS	operator=(const cRECTS &src)
	{	x1	=	src.x1;
		y1	=	src.y1;
		x2	=	src.x2;
		y2	=	src.y2;

		return src;
	}

	inline	short	getLeft()					{return min(x1,x2);}
	inline	short	getRight()					{return max(x1,x2);}
	inline	short	getTop()					{return min(y1,y2);}
	inline	short	getBottom()					{return max(y1,y2);}

	inline	DWORD	getWidth()					{return getRight()-getLeft()+1;}
	inline	DWORD	getHeight()					{return getBottom()-getTop()+1;}
	inline	DWORD	getSize()					{return getWidth()*getHeight();}

	inline	void	getCenter(short &a,short &b)	{a=getLeft()+(getRight()-getLeft())/2,b=getTop()+(getBottom()-getTop())/2;}
};

class cSTRING
{
public:
	char			*String;
	int				Length;					//	문장의 사이즈
	int				Value;
	cSTRING			*pPRE,*pNEXT;			//	이전,이후

					cSTRING();
					cSTRING(char *str);
					~cSTRING();

	void			Reset();
	
	char*	_cdecl	Set(char *str,...);
	char*	_cdecl	Add(char *str,...);
	char*	_cdecl	Insert(char *str,...);
	BOOL			Match(char *str);
	void			Minus(char *minus);
	void			SetValue(int value);

	cSTRING			operator=(const cSTRING & );
	cSTRING			operator+(const cSTRING & );
	cSTRING			operator-(const cSTRING & );

	static	char*	Minus(char *str,char *minus);
	static	char*	UpperCase(char *str);
	static	char*	LowCase(char *str);
};

class	CStream
{
public:
	char			*m_pBuffer;
	DWORD			m_dwBufferSize;
	DWORD			m_dwBufferPoint;

					CStream()
					{
						m_pBuffer	=	NULL;
					}

					~CStream()
					{
						close();
					}

	void			init(DWORD _dwSize = 1024*10);
	void			reset();
	void			close();

	BOOL			addStringP(char *_lpstrText,...);
	BOOL			addString(char *_lpstrText);
	BOOL			writeToFile(char *_lpstrFileName,BOOL _bIsReset);
};

#define	dSORT_BY_EXT	0
#define	dSORT_BY_NAME	1
#define	dSORT_ASC		0
#define	dSORT_DESC		1
#define	dSORT_NOT		2

class cSTRINGS
{
public:
	int			maxString,Count;	//	한계 문장수와 현재 문장수
	cSTRING		*pHEAD,*pTAIL;		//	처음과 마지막
	int			CurString;			//	현재 문장

				cSTRINGS(int count = 0xffff);
				~cSTRINGS();

	void		Init(int count);
	void		Reset();

	BOOL		Add		(char *str,int Value=0,BOOL check=FALSE);
	BOOL		Add		(cSTRINGS *str);
	BOOL		Remove	(int index);
	BOOL		Remove	(char *str);
	cSTRING*	Pick	(int index=0);
	char*		GetStr	(int index=0);
	int			GetIndex(char *str);

	BOOL		Sort	(int term=dSORT_ASC);

	static	int	_cdecl	Compare( const void *a, const void *b);
};

#define	dMAX_FILE_LEN		60

class	cFILENAME
{
public:
	char	Name[dMAX_FILE_LEN];
	int		m_iValue;
	BYTE	isFolder;

			~cFILENAME()	{};
	void	Set(char *name,BOOL folder)
			{	strcpy(Name,name);
				isFolder	=	folder;
			}
};

class	cFILE
{
public:
	FILE				*fp;
	DWORD				frontPos,endPos,Length;
	BOOL				isMerge;
	WORD				m_wIsFFlush;

						cFILE();
						~cFILE();

	BOOL				Open(char *fn,char *mode,BOOL _bIsFFlush=FALSE);
	BOOL				Close();
	BOOL				Seek(int pos,int seekTerm=SEEK_CUR);
	DWORD				Locate();
	BOOL				Read(void *buffer,DWORD size,DWORD itemSize=1);
	BOOL				Write(void *buffer,DWORD size,DWORD itemSize=1);
	BOOL	_cdecl		writeStreamCR(char *str,...);
	BOOL	_cdecl		writeStream(char *str,...);
	BOOL				writeStreamNP(char *str,BOOL _bIsPutReturn=TRUE);
	BOOL				Skip(int skip);
	BOOL				Rewind();			//	화일 처음으로
	BOOL				End();				//	화일 끝으로
	BOOL				IsOpen();
	BOOL				Set(cFILE *file,DWORD size);
	BOOL				Copy(char *fn);

	static	DWORD		Size(char *fn);
	static	DWORD		Size(FILE *tfp);
	static	BOOL		Burn(char *fn,char *buff,DWORD size);
};

class	cFOLDER
{
public:
	static	WORD		SortTerm;

	static	void		SetSort			(WORD	sort)
						{	SortTerm	=	sort;
							if (SortTerm < 0        ) SortTerm = 0;
							if (SortTerm > dSORT_NOT) SortTerm = dSORT_NOT;
						}

	static	BOOL		Delete			(char *delDir);				//	폴더의 화일을 비롯한 모든걸 지운다.(지울수 없는 거면 속성을 바꿔서 지운다.)
	static	BOOL		Create			(char *folder,BOOL changeFolder=FALSE);				//	폴더를 만든다.
	static	BOOL		Change			(char *folder);										//	폴더를 이동한다.

	static	BOOL		getFolders		(char *root,cSTRINGS *folders);		//	폴더들을 얻어 온다.
	static	cSTRINGS*	GetFolders		(char *root);		//	폴더들을 얻어 온다.
	static	cSTRINGS*	GetFiles		(char *ext="*.*",char *root=NULL,BOOL includeFolder=FALSE,WORD sort=dSORT_BY_EXT);//	화일들을 얻어 온다.
	static	cSTRINGS*	GetFiles		(WORD sort);																//	화일들을 얻어 온다.

	static	DWORD		GetFileCount	(char *ext="*.*",char *folder=NULL,BOOL includeFolder=FALSE,BOOL includeSub=FALSE);	//	폴더안에 화일의 수
	static	DWORD		GetFolderCount	(char *folder=NULL);										//	폴더의 수
	static	DWORD		GetFolderSize	(char *ext="*.*",char *root=NULL,BOOL includeSub=FALSE);	//	폴더안에 화일들의 사이즈

	static	int	_cdecl	Compare			(const void *a, const void *b);
	static	char		*Tree(HWND hwnd,BOOL change,char *title,char *folder=NULL,char *dest=NULL);	//	폴더트리 쉘
};

//	압축&화일 병합 관련..
#define	dMAX_MERGE_FILE	2046

class cMERGEINFO
{
public:
	char	FName[40];
	DWORD	Locate;
	DWORD	Size;
};

class	cNUX		//	라틴어로 호두라는 뜻,사용시 cNUX.cpp를 링크해야 한다. 
{
public:
	cFILE				File;
	cMERGEINFO			*INDEX;
	DWORD				AppendPos,Count;

						cNUX();
						~cNUX();

	BOOL				Load(char *fn);		//	연다.
	BOOL				Close();

	cFILE*				Open(char *fn);
	cFILE*				Open(DWORD index);

	BOOL				Export(char *fn);
	inline	cMERGEINFO	*Seek(char *fn);

	static	BOOL		bSTOP;

	static	void		(*ProcessExtract)();
	static	void		(*ProcessPress)();
	static	int	_cdecl	Compare_Name( const void *a, const void *b);

	static	DWORD		TotalSize,FileSize,FileCount,ProcessRate,ProcessSize,FileRate;
	static	char		CurrentFile[128];		//	현재 압축하고 있는 화일 혹은 압축을 풀고 있는 화일
	static	char		ArcName[128];			//	압축 화일

	static	BOOL		PressCreate(char *fn);															//	새로운 압축화일 생성
	static	BOOL		PressAppend(char *fn);
	static	BOOL		PressFile(char *fn);															//	화일 추가
	static	void		PressAllFile();																	//	현재 폴더에 모든 화일을 추가한다.
	static	BOOL		PressFolder(char *fn,BOOL includeCurrent=FALSE,BOOL includeSubFolder=FALSE);	//	폴더안에 모든 화일을 추가
	static	DWORD		PressClose();																	//	압축화일 닫기
	static	void		Stop();																			//	작업을 중지한다.
	static	BOOL		PressExtract(char *fn,char *destFolder,cSTRINGS *files=NULL);					//	압축화일 풀기

	//	비 압축,헤더 8Byte(화일 수와 헤더 위치)
	static	BOOL		MergeCreate(char *fn);														
	static	BOOL		MergeAppend(char *fn);
	static	BOOL		MergeFile(char *fn);
	static	BOOL		MergeClose();
	static	BOOL		MergeExtract(char *fn,char *destFolder);

	static	void		SetFunc(void (*press)(),void (*extract)());
};

#pragma comment(lib,"ZipArchive.lib")

extern	int	ZipCompress(unsigned char * _lpSourcedata, unsigned char *_lpTargetData,int _iDataSize);
extern	int	ZipUncompress(unsigned char * _lpSourcedata, unsigned char *_lpTargetData,int _iDataSize,int _iSourceSize);

class cMSG
{
private:
	static	char				msgBuffer[1024];
	static	char*				DXERROR(HRESULT hErr);
	static	HWND				hDEBUGWND;

public:

	static	int _cdecl	YESNO(char *headmsg,char *mainmsg,...);
	static	int _cdecl	YESNOCANCEL(char *headmsg,char *mainmsg,...);
	static	void		Test(int num=-1);

	static	void		Init();
	static	void		Close();
	static	BOOL _cdecl	Put	 (HWND hwnd,char *msg);
	static	BOOL _cdecl	Put  (char *headmsg,char *mainmsg=NULL,...);
	static	BOOL _cdecl	Output(char *msg,...);
	static	BOOL		Output(HRESULT result){return Output(DXERROR(result));}

	static	BOOL		Output(HRESULT result,char *where)
						{	return Output(_ms("%s-%s",where,DXERROR(result)));
						}
	static	BOOL		DxOutput(char *where,HRESULT result)
						{	return Output(_ms("%s-%s",where,DXERROR(result)));
						}

	static	BOOL _cdecl Error(char *headmsg,char *mainmsg=NULL,...);
	static	BOOL		Error(HRESULT result){return Error(DXERROR(result));}
	static	BOOL		Error(HRESULT result,char *where)
						{	return Error(where,DXERROR(result));
						}
	static	BOOL		Error(char *where,HRESULT result)
						{	return Error(where,DXERROR(result));
						}
};


class CTimeInfo
{
public:
	WORD	m_wYear,m_wMonth,m_wDay,m_wHour,m_wMinute,m_wSecond;

			CTimeInfo();

	void	update();
};


//////////////////////////////////////////////////////////////////////////////////

//	class	cTIMER	:	타이머 처리

//////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib,"winmm.lib")

class cTIMER
{
	static	int			s_iTps;			//	tps 초당 몇번이나 튀는가!!??
	static	int			s_iFps,s_iFrameCounter,s_iLastTime;	//	FPS 측정을 위한 수치

	int					m_iTick;		//	이름 그대로.. Frame Per Second
	int					m_iOldTime;		//	프레임 스키핑을 위한.. 시간..
	int					m_iNewTime;
	int					m_iOverTick;
	int					m_iSkipCount;	//	프레임 스킵한 횟수.

	int					m_iTimerStartTime;
public:

						cTIMER();

	inline	void		startTimer()	{m_iTimerStartTime	=	GetTime();}
	inline	int			stopTimer()
	{
		if (m_iTimerStartTime == 0) return 0;

		long	iTimeGab	=	GetTime()-m_iTimerStartTime;

		if (iTimeGab	<	0) iTimeGab+=	0x40000000;

		m_iTimerStartTime	=	0;

		return iTimeGab;
	}


	BOOL				setFPS(int fps);	//	초당 몇프레임으로 돌릴까요??
	BOOL				isSkipFrame();
	void				resetFrameSkip();

	static	inline	int	getMiliSec(LONGLONG _llTime)
	{
		_llTime	=	_llTime*1000000/s_iTps;

		return (int)_llTime;
	}

	static	inline	int	GetTime()
	{
		LARGE_INTEGER	time;

		QueryPerformanceCounter( &time );

		return (int)(time.LowPart>>2);	//	32비트 풀로 받으면 계산하기 짱나니까.. 30비트로 바꾼다..
										//	사인드 플래기가 어쩌구 저쩌구.. 궁시렁..
	}
	static	inline	int	GetFPS(){return s_iFps;}
	static	int			UpdateFPS();
};

//////////////////////////////////////////////////////////////////////////////////

//	class	cMCU	:	메모리 관리

//////////////////////////////////////////////////////////////////////////////////
extern	DWORD	GetTotalMemory();
extern	DWORD	GetAvailMemory();

class cMCU
{
public:

	DWORD			m_dwFullSize,m_dwFrontPointer,m_dwRearPointer,m_dwTempAlloc;
	BOOL			m_isRearStack;//뒤에서 부터 쌓이는 넘이다.
	char			*m_pMemory;
	cMCU			*m_lpReference;

	char			m_strName[64];
#ifdef	dDEBUG_MODE
	DWORD			m_dwaLog[102400],m_dwLogCount;
#endif

					cMCU();
					~cMCU();

	BOOL			init(DWORD size,char *name);
	BOOL			init(cMCU *_lpMcu,char *name,BOOL _isRearStack);
	void			close();							//	메모리를 작살-_- 낸다.
	void			reset();

#ifdef	dDEBUG_MODE
	void*			alloc(DWORD size,char *_lpstrName,BOOL _isRearStack=FALSE);
	void*			tempAlloc(DWORD size,char *name);
	void			check(DWORD address);
#else
	void*			alloc(DWORD size,BOOL _isRearStack=FALSE);
	void*			tempAlloc(DWORD size,char *name="temp");
#endif
	void			tempFree();
	DWORD			getRemainMemorySize();
	DWORD			getFilledMemorySize();

};

//////////////////////////////////////////////////////////////////////////////////
//	class	cDen	:	소굴-_-
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//	class	cDen	:	소굴-_-
//////////////////////////////////////////////////////////////////////////////////
template <class T>
class	cDen	//	시리얼이 16비트다.
{
	T					*m_pList;
	T					*m_lpList;
	WORD				*m_pOrderingBuffer;
	int					m_iLimitCount;
	int					m_iCount,m_iRookie;

public:
						cDen()
						{
							m_pList				=	NULL;
							m_iLimitCount		=	0;
							m_iCount			=	0;
							m_iRookie			=	0;
							m_pOrderingBuffer	=	NULL;
						}

						~cDen()
						{
							close();
						}

	void				init(int _iCount,BOOL _bIsOrdering=FALSE)//초기설정
	{
		close();

		m_pList			=	new T [_iCount];
		m_lpList		=	m_pList;
		m_iLimitCount	=	_iCount;

		if (_bIsOrdering)
		{
			m_pOrderingBuffer=	new WORD [_iCount];

			memset(m_pOrderingBuffer,0xff,sizeof(WORD)*_iCount);
		}
	}

	void				init(T *_lpList,int _iCount,BOOL _bIsOrdering=FALSE)//초기설정
	{
		close();

		m_lpList		=	_lpList;
		m_iLimitCount	=	_iCount;

		if (_bIsOrdering)
		{
			m_pOrderingBuffer=	new WORD [_iCount];

			memset(m_pOrderingBuffer,0xff,sizeof(WORD)*_iCount);
		}
	}

	inline	void		reset()//리셋
	{
		m_iCount		=	0;
		m_iRookie		=	0;

		if (m_pList)
			for (int i=0;i<m_iLimitCount;i++)
				m_lpList[i].reset();
	}

	void				close()//닫기
	{
		pKILL(m_pList);
		pKILL(m_pOrderingBuffer);

		m_lpList		=	NULL;
		reset();
	}

	inline	T*			get(int _iSerial)//_iIndex번째 멤버 리턴
	{
		if (_iSerial						>=	m_iLimitCount	)
			return	NULL;
		if (m_lpList[_iSerial].m_wSerial	==	0xffff			)
			return	NULL;

		return	&m_lpList[_iSerial];
	}
	inline	T*			getByOrdering(int _iIndex)	//	순차적으로 모든걸 검색할때 사용
	{
		if (_iIndex	>=	m_iCount	||	_iIndex	<	0)
			return	NULL;

		return	&m_lpList[m_pOrderingBuffer[_iIndex]];
	}
	inline	T*			notCheckedget(int _iIndex)	//_iIndex번째 멤버 리턴
	{
		return	&m_lpList[_iIndex];
	}
	inline	T*			getList()			{return	m_lpList;}//리스트 자체를 리턴
	inline	int			getCount()			{return	m_iCount;}//현재 카운트
	inline	int			getMaxCount()		{return	m_iLimitCount;}//최대 인원수

	int					add(T* _lpData)//멤버 추가
	{
		if (m_iCount	>=	m_iLimitCount)
			return	0xffff;

		int	iIndex	=	m_iRookie;

		m_lpList[m_iRookie].copy(_lpData);
		m_lpList[m_iRookie].m_wSerial	=	m_iRookie;

		if (m_pOrderingBuffer)
			m_pOrderingBuffer[m_iCount]	=	m_iRookie;

		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				return	iIndex;
			}

		return	iIndex;
	}

	inline	T*			getRookie(int *_lpSerial)//루키
	{
		if (m_iCount	>=	m_iLimitCount)
			return	NULL;

		*_lpSerial	=	m_iRookie;

		return	&m_lpList[m_iRookie];
	}

	inline	int			getRookieSerial()//루키 시리얼 리턴
	{
		if (m_iCount	>=	m_iLimitCount)
			return	0xffff;

		return	m_iRookie;
	}

	inline	void		increaseCount()//카운트 증가
	{
		if (m_lpList[m_iRookie].m_wSerial	==	0xffff)
			return;

		if (m_pOrderingBuffer)
			m_pOrderingBuffer[m_iCount]	=	m_iRookie;

		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				break;
			}
	}

	BOOL				remove(int _iIndex)//멤버 제거
	{
		if (m_lpList[_iIndex].m_wSerial	==	0xffff)
			return	FALSE;

		m_lpList[_iIndex].reset();

		if (m_pOrderingBuffer)
			for (int i=0;i<m_iCount;i++)
				if	(m_pOrderingBuffer[i]	==	_iIndex)
				{
					m_pOrderingBuffer[i]	=	m_pOrderingBuffer[m_iCount-1];
					break;
				}

		if (_iIndex	<	m_iRookie)
			m_iRookie	=	_iIndex;

		m_iCount--;

		return	TRUE;
	}

	BOOL				insert(int _iSerial,T *_lpData)//특정 인덱스에 멤버 추가
	{
		if (m_lpList[_iSerial].m_wSerial	!=	0xffff)
			return	FALSE;

		m_lpList[_iSerial].copy(_lpData);
		m_lpList[_iSerial].m_wSerial	=	_iSerial;

		if (m_pOrderingBuffer)
			m_pOrderingBuffer[m_iCount]	=	_iSerial;

		m_iCount++;

		for (int i=m_iRookie;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				return	TRUE;
			}

		return	TRUE;
	}

	void			rebuild()
	{
		int	i;

		m_iRookie	=	-1;
		m_iCount	=	0;

		for (i=0;i<m_iLimitCount;i++)
		{
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				if	(m_iRookie	==	-1)
					m_iRookie	=	i;

				continue;
			}

			m_iCount++;
		}

		if (m_pOrderingBuffer)
		{
			m_iCount	=	0;

			for (i=0;i<m_iLimitCount;i++)
			{
				if (m_lpList[i].m_wSerial	==	0xffff)
					continue;

				m_pOrderingBuffer[m_iCount++]	=	i;
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////////////////
//	암호화
//////////////////////////////////////////////////////////////////////////////////

DWORD	EncodeBuffer(int _iSeedValue,BYTE *_lpBuffer,int _iBufferSize);
DWORD	DecodeBuffer(int _iSeedValue,BYTE *_lpBuffer,int _iBufferSize);

//////////////////////////////////////////////////////////////////////////////////
//	class	cTHREAD	:	쓰레드
//////////////////////////////////////////////////////////////////////////////////

class cTHREAD
{
public:
	HANDLE		ThreadHandle;	// 스레드 핸들
	DWORD		ThreadAddr;     // 스레드 주소

				cTHREAD();
				~cTHREAD();

	void		Create(HWND hwnd,void (__cdecl *func)(void *),void *_lpData=NULL);
	void		Close();

};

//////////////////////////////////////////////////////////////////////////////////

//	디버깅용 코드들. 좀 지저분 해도 매우 중요하니 참아라. --;;

//////////////////////////////////////////////////////////////////////////////////
#define	dCHECK_PTR_HEAD			0x12345678					//	포인터를 체크한다. 헤드
#define	dCHECK_PTR_TAIL			0x87654321					//	포인터를 체크한다. 마무리
#define	dCHECK_MCU_HEAD			dCHECK_PTR_TAIL				//	MCU의 포인터를 체크한다. 헤드
#define	dCHECK_MCU_TAIL			dCHECK_PTR_HEAD				//	MCU의 포인터를 체크한다. 마무리
#define	dPTR_STRING_SIZE		64							//	포인터 이름 사이즈

#ifdef dDEBUG_MODE			//	디버깅 모드냐?
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃#ifdef dDEBUG_MODE	디버깅 모드다.
#define	dCHECK_POINTER_SIZE		(64+12)						//	포인터 이름 사이즈

//	메모리를 설정 매크로. type 형,numbers 갯수,mcu cMCU 포인터 NULL이면 그냥 메모리에 설정한다. name 디버깅용 포인터 이름
#define	_New(type,numbers,mcu,name)		(type *)safeMalloc((numbers)*sizeof(type),mcu,name)

//	메모리를 해제 매크로. 일단 있는지(?) 확인하고 있으면 체크한 다음에 해제하고 포인터를 초기화 시킨다.
#define	_Free(ptr)								\
if (ptr)										\
{	_checkPtr((char *)ptr);						\
	free((char *)(ptr)-(dPTR_STRING_SIZE+8));	\
	ptr		=	NULL;							\
}

//	메모리를 설정 함수. type 형,numbers 갯수,mcu cMCU 포인터 NULL이면 그냥 메모리에 설정한다. name 디버깅용 포인터 이름
inline void*  safeMalloc(unsigned int size,cMCU *mcu,char *name)
{	if (mcu)
	{	void	*ptr	=	mcu->alloc(size,name);

		return	ptr;
	}

	char	*ptr	=	(char *)malloc(size+4+4+4+dPTR_STRING_SIZE);	//	size + head(4) + size(4) + str(64) + tail(4)

	*(unsigned int *)(ptr							)	=	dCHECK_PTR_HEAD;			//	헤더
	*(unsigned int *)(ptr+4							)	=	(size+8+dPTR_STRING_SIZE);	//	꼬리 위치
	*(unsigned int *)(ptr+8+dPTR_STRING_SIZE+size	)	=	dCHECK_PTR_TAIL;			//	꼬리
	strncpy((char *)(ptr+8),name,dPTR_STRING_SIZE-1);										//	이름

	ptr	+=	(dPTR_STRING_SIZE+8);

	return ptr;
};

//	메모리를 체크 함수. 메모리를 해제 하기 전에 p의 앞뒤를 검사해 깨지지 않았는지 확인한다.
inline void _checkPtr(char *p)
{	unsigned int	head,tail,size;

	char *ptr	=	p-(dPTR_STRING_SIZE+8);

	head		=	*(DWORD *)(ptr		);		//	헤더
	size		=	*(DWORD *)(ptr+4	);		//	헤더
	tail		=	*(DWORD *)(ptr+size	);		//	헤더

	if (head	!= dCHECK_PTR_HEAD)
	{	if (head	== dCHECK_MCU_HEAD)
				ERRMSG(_ms("Error!! pointer %s are cMCU's Client!!",ptr+8));
		else	ERRMSG(_ms("Error!! pointer %s's HEAD offset damaged!!",ptr+8));
	}

	if (tail	!= dCHECK_PTR_TAIL)	ERRMSG(_ms("Error!! pointer %s's TAIL offset damaged!!",ptr+8));
};
//┃#ifdef dDEBUG_MODE	디버깅 모드
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#else	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃#ifdef dDEBUG_MODE	디버깅 모드가 아니다.
#define	dCHECK_POINTER_SIZE		0						//	포인터 이름 사이즈

//	에러체크 안한다.
//	메모리 할당 매크로... mcu 유닛이 있다면 그 넘한테 설정하고 아니면... 걍 한다.
#define	_New(type,numbers,mcu,c)	(type *)safeMalloc((numbers)*sizeof(type),mcu)
inline void*  safeMalloc(unsigned int size,cMCU *mcu)
{	void*	ptr;

	if (mcu)
	{	ptr	=	mcu->alloc(size);

		return ptr;
	}

	ptr		=	(void *)malloc(size);

	return ptr;
}

//	메모리 해제 매크로... 이 메크로가 참조한 매크로인지 확인한다.
#define	_Free(ptr)											\
if (ptr)													\
{	if (*(DWORD*)((char*)(ptr)-4)	==	dCHECK_PTR_TAIL)	\
	{	ptr	=	NULL;										\
		ERRMSG("Error!! cMCU's Inheritor are destroy!!");	\
	}														\
	free(ptr);												\
	ptr	=	NULL;											\
}


#endif
//┃#ifdef dDEBUG_MODE	디버깅 모드가 아니다.
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#endif