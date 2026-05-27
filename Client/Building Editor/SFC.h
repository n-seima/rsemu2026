/******************************************************************************************
	SFC.H

	몇가지 유용한 루틴과 몇가지 유용한 매크로
	MMX레지스터와 FPU레지스터를 이용한 메모리 전송.

	작성 날짜 : 1999.02.22
	1차	 갱신 : 1999.03.15	-	* MMX레지스터와FPU레지스터를 이용한 메모리 전송추가
								* UTIL.H -> GLOBAL.H 로 바꿈.
	2차	 갱신 : 1999.03.24	-	* cRECT global class를 추가함을 시작으로 전역 클래스 작성.
	3차  갱신 : 1999.05.14  -   * sine,cosine 테이블 추가
	4차  갱신 : 2000.06.09	-	* sine,cosine 테이블 제거

								* cSTRING,cSTRINGS,cFOLDER,cFILE,cRECT,cRECTS,cRECT_SIZE,cRECTS_SIZE,cPOINT,cPOINTS class
								추가 및 교정

								* 압축 클래스 cNUX 추가

								* _ms,_exportFileName,_exportFolderName,_exportExt,_setExt,_fileSize,IsFolder
								등의 전역 함수 추가및 교정

								* 전역 변수 _logFolder 추가(프로그램이 실행된 폴더)

								* GLOBAL 을 SFC로 바꿈

	5차  갱신 : 2000.06.22	-	* _fileSize 제거
								* cFILE를 cFILES로 바꿈
								* cFILE 화일 제어 클래스 추가
								* cNUX 화일 병합 기능 추가
								* IsFile 추가
								* cFILES 클래스 제거. -.- cFOLDER과 합쳐 버렸다.


*******************************************************************************************/

#ifndef _classSFC_H
#define _classSFC_H

#include <windows.h>
#include <stdio.h>

#define	dDEBUG_MODE			1			//		디버깅 모드 이다. 조건문이 많아 지는 대신 에러검색이 쉬워쥔다.준다.
#define	WM_CONSOLETEXT		(WM_USER+2)
#define	WIN32_LEAN_AND_MEAN	1

#define	dGET_FILE_NAME		0x480
#define	dGET_FILE_EXT		0x470
#define	d1MEGA				1048576

#define	dPUT_SOFT_DODGE			0	//	불효과
#define	dPUT_DIFFERENT			1	//	중독
#define	dPUT_DODGE_BURN			2	//	불에 그을린 효과
#define	dPUT_SOFT				3	//	연기..
#define	dPUT_DODGE				4	//	빛계열 마법
#define	dPUT_LIGHT				5	//	다중 투영효과..
#define	dPUT_EXTREAM_LIGHT		6	//	무쟈게 밝게..
#define	dPUT_NORMAL				7	//	걍.. 찍는다.
#define	dPUT_HALF_BLENDING		8	//	반투명
#define	dPUT_COLOR				9	//	특정 컬러로 찍는다.
#define	dPUT_ALPHA_BLENDING		10	//	알파 블렌딩

#define pKILL(buff) if (buff) {delete [] buff	;buff=NULL;}	// 포인터 삭제..
#define KILL(buff)	if (buff) {delete buff		;buff=NULL;}	// 클래스 삭제..
#undef  ATTEMPT
#define ATTEMPT(x)	{ if (FAILED(x)) goto ERROR_LABEL; }		//	에러 처리..
#undef  RELEASE
#define RELEASE(x)	{ if (x != NULL) {x->Release(); x = NULL;} }//	DX개체 삭제..

#define	MSGOUT		cMSG::Output
#define	PUTMSG		cMSG::Put
#define	ERRMSG		cMSG::Error
#define	ERRORED		Errored()
#define	SAFETY		Safety()


template <class A> inline void swap(A &a, A &b)
{   A   c;
    c = a;
    a = b;
    b = c;
}

extern	char		_logFolder[1024];						//	프로그램 실행 폴더

extern	int			_PixelFormat;
#define dRGB565		0 
#define dRGB555		1
#define dBGR565		2
extern	WORD		(*RGBmix  )(int r,int g,int b );
extern	WORD		RGB565mix(int r,int g,int b );
extern	WORD		RGB555mix(int r,int g,int b );
extern	WORD		BGR565mix(int r,int g,int b );

extern	void		memset(WORD *dest,WORD value,unsigned int size);	//	16비트 용..
extern	inline int	random(int range);

extern	BOOL		_makeShotcut(char* src, char* lnkName, char* lnkPath);
extern	char* __cdecl	_ms(char *str,...);					//	문장을 만든다.
extern	char		*_exportFileName(char *str);			//	화일 이름만 뽑아 낸다.
extern	char		*_exportFolderName(char *str);			//	폴더 이름만 뽑아 낸다.
extern	char		*_exportExt(char *str);					//	확장자만 뽑아 낸다.
extern	void		_setExt(char *srcStr,char *destStr);	//	확장자를 설정한다.
extern	BOOL		IsFolder(char *fn);						//	이 녀석이 폴더냐?
extern	BOOL		IsFile(char *fn);
extern	BOOL		Safety();
extern	BOOL		Errored();

typedef struct 
{	int 	Value;
	int 	Index;
}	tINDEX;

extern	int Compare( const void *a, const void *b);

class cPOINT
{
public:
	int		x,y;

			cPOINT()			{x=0,y=0;}
			cPOINT(int a,int b) {x=a,y=b;}

	void	Set(POINT pos)		{x =x,y =y;}
	void	Set(int a,int b)	{x =a,y =b;}
	void	Add(int a,int b)	{x+=a,y+=b;}
	BOOL	match(cPOINT pos);
	BOOL	match(POINT pos);
	BOOL	match(int a,int b);

	void	toScreen(HWND hwnd);
	void	MousePos(HWND hwnd);
	void	MousePos();

	cPOINT	operator=(const cPOINT & );
	cPOINT	operator+(const cPOINT & );
	cPOINT	operator-(const cPOINT & );
};

class cPOINTS
{
public:
	short	x,y;

	void	Set(short a,short b)	{x =a,y =b;}
	void	Move(short a,short b)	{x+=a,y+=b;}
	BOOL	Match(cPOINT *pos);
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
	static	RECT	rect;

			cRECT()								{x1=0,y1=0,x2=0,y1=0;}
			cRECT(int X1,int Y1,int X2,int Y2)	{x1=X1,y1=Y1,x2=X2,y2=Y2;}
			cRECT(HWND hwnd)					{Set(hwnd);}
			cRECT(RECT r)						{Set(r.left,r.top,r.right,r.bottom);}

	void	Set(int X1,int Y1,int X2,int Y2)	{x1=X1,y1=Y1,x2=X2,y2=Y2;}
	void	Set(HWND hwnd);
	void	Set(RECT rect)						{Set(rect.left,rect.top,rect.right,rect.bottom);}
	void	Set(RECT *rect)						{rect->left=x1,rect->top=y1,rect->right=x2,rect->bottom=y2;}
	void	OutFrame(HWND hwnd);
	void	toScreen(HWND hwnd);

	cRECT	operator=(const cRECT & );

	int		left()				{return min(x1,x2);}
	int		right()				{return max(x1,x2);}
	int		top()				{return min(y1,y2);}
	int		bottom()			{return max(y1,y2);}

	void	addLeft(int x)		{x1+=x;}
	void	addRight(int x)		{x2+=x;}
	void	addTop(int y)		{y1+=y;}
	void	addBottom(int y)	{y2+=y;}

	void	left(int x)			{x1=x;}
	void	right(int x)		{x2=x;}
	void	top(int y)			{y1=y;}
	void	bottom(int y)		{y2=y;}

	int		width()				{return right()-left();}
	int		height()			{return bottom()-top();}
	int  	centerX()			{return left()+ width()/2 ;}
	int  	centerY()			{return top() + height()/2;}
	DWORD	size()				{return width()*height();}

	void	move(int x,int y)	{x1+=x,x2+=x,y1+=y,y2+=y;}
	void	moveTo(int x,int y)	{x2=x+width(),x1=x,y2=y+height(),y1=y;}
	void	adjust(int X1,int Y1,int X2,int Y2) {x1+=X1,y1+=Y1,x2-=X2,y2-=Y2;}

	BOOL	isIN(int x,int y);
	BOOL	crash(cRECT rect);
};

class cRECT_SIZE
{
public:
	int		x,y,xs,ys;

	cRECT_SIZE	operator=(const cRECT_SIZE & );

	void	Set(int a,int b,int as,int bs);
	void	OutFrame(HWND hwnd);
	void	ClientFrame(HWND hwnd);
	void	AdjustScreen(HWND hwnd);

	void	Move(int x,int y);
	void	Add(int x,int y);

	BOOL	Isin(int x,int y);
	BOOL	Crash(cRECT_SIZE *rect);

	int		Right()					{return x+xs-1;}
	int		Bottom()				{return y+ys-1;}
	void	Resize(int a,int b)		{xs=a,ys=b;}

	DWORD	Size()					{return xs*ys;}

	void	Center(int &a,int &b)	{a=x+xs/2,b=y+ys/2;}
};

class cRECTS
{
public:
	short	x1,y1,x2,y2;

	cRECTS	operator=(const cRECTS & );

	void	Set(short a1,short b1,short a2,short b2);
	void	OutFrame(HWND hwnd);
	void	ClientFrame(HWND hwnd);
	void	AdjustScreen(HWND hwnd);

	void	Move(short x,short y);
	void	Add(short x,short y);

	BOOL	Isin(short x,short y);
	BOOL	Crash(cRECTS *rect);

	short	Left()					{return min(x1,x2);}
	short	Right()					{return max(x1,x2);}
	short	Top()					{return min(y1,y2);}
	short	Bottom()				{return max(y1,y2);}
	void	Left(short x)			{x1=x;}
	void	Right(short x)			{x2=x;}
	void	Top(short y)			{y1=y;}
	void	Bottom(short y)			{y2=y;}

	DWORD	Width()					{return Right()-Left();}
	DWORD	Height()				{return Bottom()-Top();}
	DWORD	Size()					{return Width()*Height();}

	void	Center(short &a,short &b)	{a=Left()+(Right()-Left())/2,b=Top()+(Bottom()-Top())/2;}

};

class cRECTS_SIZE
{
public:
	short	x,y,xs,ys;

	cRECTS_SIZE	operator=(const cRECTS_SIZE & );

	void	Set(short a,short b,short as,short bs);
	void	OutFrame(HWND hwnd);
	void	ClientFrame(HWND hwnd);
	void	AdjustScreen(HWND hwnd);

	void	Move(short x,short y);
	void	Add(short x,short y);

	BOOL	Isin(short x,short y);
	BOOL	Crash(cRECTS_SIZE *rect);

	short	Right()						{return x+xs-1;}
	short	Bottom()					{return y+ys-1;}
	void	Resize(short a,short b)		{xs=a,ys=b;}

	DWORD	Size()						{return xs*ys;}

	void	Center(short &a,short &b)	{a=x+xs/2,b=y+ys/2;}
};

class cSTRING
{
public:
	char			*String;
	int				Length;					//	문장의 사이즈
	cSTRING			*pPRE,*pNEXT;			//	이전,이후

					cSTRING();
					cSTRING(char *str);
					~cSTRING();

	char*	_cdecl	Set(char *str,...);
	char*	_cdecl	Add(char *str,...);
	char*	_cdecl	Insert(char *str,...);
	BOOL			Match(char *str);
	void			Minus(char *minus);

	cSTRING			operator=(const cSTRING & );
	cSTRING			operator+(const cSTRING & );
	cSTRING			operator-(const cSTRING & );

	static	char*	Minus(char *str,char *minus);
	static	char*	UpperCase(char *str);
	static	char*	LowCase(char *str);
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

	BOOL		Add		(char *str,BOOL check=FALSE);
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

						cFILE();
						~cFILE();

	BOOL				Open(char *fn,char *mode);
	BOOL				Close();
	BOOL				Seek(int pos,int seekTerm);
	DWORD				Locate();
	BOOL				Read(void *buffer,DWORD size);
	BOOL				Write(void *buffer,DWORD size);
	BOOL				Rewind();
	BOOL				End();
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

	FILE*				Open(char *fn);
	FILE*				Open(DWORD index);

	BOOL				Open(char *fn,cFILE *file);
	BOOL				Open(DWORD index,cFILE *file);
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

class cMSG
{
private:
	static	char		msgBuffer[256];
	static	char*		DXERROR(HRESULT hErr);

public:
	static	HANDLE		hCONSOLE;
	static	HWND		hCONSOLEWND;

	static	void		InitConsole(int width = 120, int height = 40);

	static	int _cdecl	YESNO(char *headmsg,char *mainmsg,...);
	static	int _cdecl	YESNOCANCEL(char *headmsg,char *mainmsg,...);
	static	void		Test(int num=-1);

	static	BOOL _cdecl	Put	 (HWND hwnd,char *msg);
	static	BOOL _cdecl	Put  (char *headmsg,char *mainmsg=NULL,...);
	static	BOOL _cdecl	Output(char *msg,...);
	static	BOOL		Output(HRESULT result){return Output(DXERROR(result));}

	static	BOOL _cdecl Error(char *headmsg,char *mainmsg=NULL,...);
	static	BOOL		Error(HRESULT result){return Error(DXERROR(result));}
};


/**********************************************************************************

	class	cTIMER	:	타이머 처리

**********************************************************************************/

#pragma comment(lib,"winmm.lib")

class cTIMER
{	static	long	TPS;			//	tps 초당 몇번이나 튀는가!!??
	static	int		Tick;			//	이름 그대로.. Frame Per Second

	static	long	getCurrentTime();

			long	m_lStartTime;

public:
	static	int		FPS;

					cTIMER();

			void	Start();
			DWORD	Stop();

	//	적적 멤버들
	static	BOOL	Init(int fps);				//	초당 몇프레임으로 돌릴까요??
	static	BOOL	FrameSkip();
	static	int		UpdateFPS();
	static	BOOL	isNOW(int lasttime,int delay);
};

#endif