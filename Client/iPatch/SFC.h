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

								* _ms,_exportFileName,_exportFolderName,_exportExt,s_strFname,_fileSize,IsFolder
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

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define	dDEBUG_MODE				1			//		디버깅 모드 이다. 조건문이 많아 지는 대신 에러검색이 쉬워쥔다.준다.
#define	WM_DEBUGTEXT		(WM_USER+2)
#ifndef WIN32_LEAN_AND_MEAN
#define	WIN32_LEAN_AND_MEAN
#endif

#define	dGET_FILE_NAME			0x480
#define	dGET_FILE_EXT			0x470
#define	d1MEGA					1048576

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
#define ERRORHANDLER(x)	{ if (x) goto ERROR_LABEL; }		//	에러 처리..

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

extern	BOOL		bACTIVEAPP;
extern	char		_logFolder[1024];						//	프로그램 실행 폴더

extern	int			_PixelFormat;
#define dRGB565		0 
#define dRGB555		1
#define dBGR565		2
extern	WORD		(*RGBmix  )(int r,int g,int b );
extern	WORD		RGB565mix(int r,int g,int b );
extern	WORD		RGB555mix(int r,int g,int b );
extern	WORD		BGR565mix(int r,int g,int b );

extern	void		memset16(WORD *dest,WORD value,unsigned int size);	//	16비트 용..
extern	inline int	random(int range);

extern	char *		GetExcuteFileName();
extern	char *		GetCommandLine(int index);

extern	BOOL		_makeShotcut(char* src, char* lnkName, char* lnkPath,char *commandLine=NULL);
extern	char* __cdecl	_ms(char *str,...);					//	문장을 만든다.
extern	char		*_exportFileName(char *str);			//	화일 이름만 뽑아 낸다.
extern	char		*_exportName(char *str);
extern	char		*_exportFolderName(char *str);			//	폴더 이름만 뽑아 낸다.
extern	char		*_exportExt(char *str);					//	확장자만 뽑아 낸다.
extern	void		_setExt(char *srcStr,char *destStr);	//	확장자를 설정한다.
extern	BOOL		IsFolder(char *fn);						//	이 녀석이 폴더냐?
extern	BOOL		IsFile(char *fn);
extern	BOOL		Safety();
extern	BOOL		Errored();
extern	BOOL		ustrncpy(char *dest,char *src,int size);
extern	BOOL		GetProgrameFilesFolder( LPSTR strBuffer, int buffersize) ;

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
extern	int	CompareInteger(const int *a, const int *b);


class	cDATA_STRING_32
{
public:
	char	m_strString[32];
	int		m_iData;
};

class	cDATA_STRING_64
{
public:
	char	m_strString[64];
	int		m_iData;
};

class	cDATA_STRING_128
{
public:
	char	m_strString[128];
	int		m_iData;
};


class cPOINT
{
public:
	int		x,y;

			cPOINT()			{x=0,y=0;}
			cPOINT(int a,int b) {x=a,y=b;}

	void	Set(POINT pos)		{x =x,y =y;}
	void	Set(int a,int b)	{x =a,y =b;}
	void	Add(int a,int b)	{x+=a,y+=b;}
	BOOL	isSame(cPOINT *pos);
	BOOL	isSame(POINT *pos);
	BOOL	isSame(int a,int b);

	void	toScreen(HWND hwnd);
	void	MousePos(HWND hwnd);
	void	MousePos();

	cPOINT	operator=(const cPOINT & );
	cPOINT	operator+(const cPOINT & );
	cPOINT	operator-(const cPOINT & );
};

class c4POINT : public cPOINT
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

	int		width()				{return right()-left()+1;}
	int		height()			{return bottom()-top()+1;}
	int  	centerX()			{return left()+ width()/2 ;}
	int  	centerY()			{return top() + height()/2;}
	DWORD	size()				{return width()*height();}

	void	Move(int x,int y)	{x1+=x,x2+=x,y1+=y,y2+=y;}
	void	MoveTo(int x,int y)	{x2=x+width(),x1=x,y2=y+height(),y1=y;}
	void	Adjust(int X1,int Y1,int X2,int Y2) {x1+=X1,y1+=Y1,x2-=X2,y2-=Y2;}

	BOOL	isIN(int x,int y);
	BOOL	Crash(cRECT *rect);
	void	Refit();

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

	BOOL	isIN(int x,int y);
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
	int				Length;					//	문장의 사이즈
	int				m_iSerial;
	int				m_iValue;
	char			*String;
	cSTRING			*pPRE,*pNEXT;			//	이전,이후

					cSTRING();
					cSTRING(char *str);
					~cSTRING();

	void			Reset();

	char*	_cdecl	Set(char *str);
	char*	_cdecl	sprint(char *str,...);
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

#define	dSORT_STRING_SIZE	64

class	cSORT_STRING_ITEM
{
public:
	char	m_strString[dSORT_STRING_SIZE];
	int 	m_iValue;
	int 	m_iSerial;
};

class cSTRINGS
{
public:
	int			maxString,Count;	//	한계 문장수와 현재 문장수
	cSTRING		*pHEAD,*pTAIL;		//	처음과 마지막
	int			CurString;			//	현재 문장

				cSTRINGS	(int count = 0xffff);
				~cSTRINGS	();

	void		Init		(int count);
	void		Reset		();

	BOOL		Add			(char *str,BOOL check=FALSE);
	BOOL		AddAndSort	(char *str,BOOL check=FALSE);
	BOOL		AddAndIndex	(char *str,int index);
	BOOL		AddAndData	(char *str,int data,int _iSerial=0xffff);

	BOOL		Add			(cSTRINGS *str);
	BOOL		Add			(cSTRING *str);
	BOOL		Remove		(int index);
	BOOL		Remove		(char *str);
	cSTRING*	Pick		(int index=0);
	char*		GetStr		(int index=0);
	int			GetData		(int index=0);
	int			GetIndex	(char *str);

	BOOL		Sort		(int term=dSORT_ASC);

	cSTRINGS		operator=(const cSTRINGS & );

	static	int	_cdecl	Compare( const void *a, const void *b);
};

#define	dMAX_FILE_LEN		64

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

class cThread
{
public:
	HANDLE		m_hThread;	// 스레드 핸들
	DWORD		m_dwThreadAddr;     // 스레드 주소
	BOOL		m_bActive;

				cThread();
				~cThread();

	void		close();
	void		create(HWND hwnd,void (__cdecl *func)() );
	BOOL		isAvail();
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
	BOOL				Seek(int pos,int seekTerm=SEEK_CUR);
	DWORD				Locate();
	BOOL				Read(void *buffer,DWORD size,DWORD items=1);
	BOOL				Write(void *buffer,DWORD size,DWORD items=1);
	BOOL				Write4(int _iWrite);
	BOOL				Rewind();
	BOOL				End();
	BOOL				IsOpen();
	BOOL				Set(cFILE *file,DWORD size);
	BOOL				Copy(char *fn);
	BOOL				Append(char *fn);
	BOOL	_cdecl		writeStream(char *str,...);
	BOOL	_cdecl		writeStreamCR(char *str,...);
	

	static	DWORD		Size(char *fn);
	static	DWORD		Size(FILE *tfp);
	static	BOOL		Burn(char *fn,char *buff,DWORD size);
	static	BOOL		SaveBox(char *fname,char *filter,char *defaultExt,char *folder=NULL,char *title="Save data",DWORD _dwStyle=0);
	static	BOOL		LoadBox(char *fname,char *filter,char *defaultExt,char *folder=NULL,char *title="Load data",DWORD _dwStyle=0);
	static	BOOL		LoadBoxHook(char *fname,char *filter,char *defaultExt,LPOFNHOOKPROC _procHook,
									char *folder=NULL,char *title="Load data",DWORD _dwStyle=0);
	static	BOOL		LoadBoxTemplate(char *fname,char *filter,char *defaultExt,char *_strTemplateName,
										LPOFNHOOKPROC _procHook,HINSTANCE _hInst,char *folder=NULL,char *title="Load data",DWORD _dwStyle=0);

};

class	cFOLDER
{
public:
	static	WORD		SortTerm;
	static	char		s_strFolder[1024];
	static	char		s_destFolder[1024];
	static	BOOL		s_bOk;

	static	void		SetSort			(WORD	sort)
						{	SortTerm	=	sort;
							if (SortTerm < 0        ) SortTerm = 0;
							if (SortTerm > dSORT_NOT) SortTerm = dSORT_NOT;
						}

	static	BOOL		Delete			(char *delDir);				//	폴더의 화일을 비롯한 모든걸 지운다.(지울수 없는 거면 속성을 바꿔서 지운다.)
	static	BOOL		DeleteAllFile	(char *_lpstrFilter="*.*");			//	화일을 포함한 폴더를 지운다.

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
	static	char		*Tree(HWND hwnd,BOOL change,BOOL create,char *title,char *folder=NULL,char *dest=NULL);	//	폴더트리 쉘
	static	int CALLBACK	BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) ;

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
	static	BOOL		bMERGE;
	static	BOOL		bMERGEFOLDER;

	static	void		(*ProcessExtract)();
	static	void		(*ProcessPress)();
	static	int	_cdecl	Compare_Name( const void *a, const void *b);

	static	DWORD		TotalSize,FileSize,FileCount,ProcessRate,ProcessSize,FileRate;
	static	char		CurrentFile[128];		//	현재 압축하고 있는 화일 혹은 압축을 풀고 있는 화일
	static	char		ArcName[128];			//	압축 화일

	static	BOOL		PressCreate(char *fn,BOOL merge=FALSE);											//	새로운 압축화일 생성
	static	BOOL		PressAppend(char *fn,BOOL merge=FALSE);
	static	BOOL		PressFile(char *fn);															//	화일 추가
	static	void		PressAllFile();																	//	현재 폴더에 모든 화일을 추가한다.
	static	BOOL		PressFolder(char *fn,BOOL includeCurrent=FALSE,BOOL includeSubFolder=FALSE);	//	폴더안에 모든 화일을 추가
	static	DWORD		PressClose();																	//	압축화일 닫기
	static	void		Stop();																			//	작업을 중지한다.
	static	BOOL		PressExtract(char *fn,char *destFolder,char *strLog=NULL,cSTRINGS *files=NULL);					//	압축화일 풀기

	//	비 압축,헤더 8Byte(화일 수와 헤더 위치)
	static	BOOL		MergeCreate(char *fn);														
	static	BOOL		MergeAppend(char *fn);
	static	BOOL		MergeFile(char *fn);
	static	void		MergeAllFile();
	static	BOOL		MergeClose();
	static	BOOL		MergeFolder(char *fn,BOOL includeCurrent=FALSE,BOOL includeSubFolder=FALSE);
	static	BOOL		MergeExtract(char *fn,char *destFolder,char *strLog=NULL,cSTRINGS *files=NULL);
	static	void		mergeFolder(char *fn);

	static	void		SetExtractFunc(void (*extract)(void *lpInfo));
	static	void		SetPressFunc(void (*press)(void *lpInfo));

	static	void		Pause();
	static	void		Resume();
	static	void		Cancel();
	static	BOOL		IsCanceled();
	static	BOOL		IsPaused();

};

class cMSG
{
private:
	static	char		msgBuffer[1024];
	static	char*		DXERROR(HRESULT hErr);
	static	HWND		hDEBUGWND;

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

/**********************************************************************************

	class	cINPDEV	:	마우스와 키보드 처리

**********************************************************************************/

extern	cPOINT	MOUSE;				//	커서의 위치
extern	BYTE	KEY[256];			//	키보드 눌렸냐?
extern	BYTE	TOUCHKEY[256];		//	키보드 눌렸냐?(한번만)
extern	BYTE	LEFTDBLCLICK;
extern	BYTE	RIGHTDBLCLICK;
extern	BYTE	LBUTTON;
extern	BYTE	RBUTTON;
extern	BYTE	TOUCHLBUTTON;
extern	BYTE	TOUCHRBUTTON;

#define dKEY_LBUTTON        0x01
#define dKEY_RBUTTON        0x02
#define dKEY_CANCEL         0x03
#define dKEY_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define dKEY_BACK           0x08
#define dKEY_TAB            0x09

#define dKEY_CLEAR          0x0C
#define dKEY_ENTER			0x0D

#define dKEY_SHIFT          0x10
#define dKEY_CONTROL        0x11
#define dKEY_ALT			0x12
#define dKEY_PAUSE          0x13
#define dKEY_CAPITAL        0x14

#define dKEY_HANGUL         0x15
#define dKEY_JUNJA          0x17
#define dKEY_FINAL          0x18
#define dKEY_HANJA          0x19

#define dKEY_ESCAPE         0x1B

#define dKEY_CONVERT        0x1C
#define dKEY_NONCONVERT     0x1D
#define dKEY_ACCEPT         0x1E
#define dKEY_MODECHANGE     0x1F

#define dKEY_SPACE          0x20
#define dKEY_PGUP			0x21
#define dKEY_PGDOWN			0x22
#define dKEY_END            0x23
#define dKEY_HOME           0x24
#define dKEY_LEFT           0x25
#define dKEY_UP             0x26
#define dKEY_RIGHT          0x27
#define dKEY_DOWN           0x28
#define dKEY_SELECT         0x29
#define dKEY_PRINT          0x2A
#define dKEY_EXECUTE        0x2B
#define dKEY_SNAPSHOT       0x2C
#define dKEY_INSERT         0x2D
#define dKEY_DELETE         0x2E
#define dKEY_HELP           0x2F

/* dKEY_0 thru dKEY_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* dKEY_A thru dKEY_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

#define dKEY_LWIN           0x5B
#define dKEY_RWIN           0x5C
#define dKEY_APPS           0x5D

#define dKEY_NUMPAD0        0x60
#define dKEY_NUMPAD1        0x61
#define dKEY_NUMPAD2        0x62
#define dKEY_NUMPAD3        0x63
#define dKEY_NUMPAD4        0x64
#define dKEY_NUMPAD5        0x65
#define dKEY_NUMPAD6        0x66
#define dKEY_NUMPAD7        0x67
#define dKEY_NUMPAD8        0x68
#define dKEY_NUMPAD9        0x69
#define dKEY_MULTIPLY       0x6A
#define dKEY_ADD            0x6B
#define dKEY_SEPARATOR      0x6C
#define dKEY_SUBTRACT       0x6D
#define dKEY_DECIMAL        0x6E
#define dKEY_DIVIDE         0x6F
#define dKEY_F1             0x70
#define dKEY_F2             0x71
#define dKEY_F3             0x72
#define dKEY_F4             0x73
#define dKEY_F5             0x74
#define dKEY_F6             0x75
#define dKEY_F7             0x76
#define dKEY_F8             0x77
#define dKEY_F9             0x78
#define dKEY_F10            0x79
#define dKEY_F11            0x7A
#define dKEY_F12            0x7B
#define dKEY_F13            0x7C
#define dKEY_F14            0x7D
#define dKEY_F15            0x7E
#define dKEY_F16            0x7F
#define dKEY_F17            0x80
#define dKEY_F18            0x81
#define dKEY_F19            0x82
#define dKEY_F20            0x83
#define dKEY_F21            0x84
#define dKEY_F22            0x85
#define dKEY_F23            0x86
#define dKEY_F24            0x87

#define dKEY_NUMLOCK        0x90
#define dKEY_SCROLL         0x91


class cINPDEV
{
public:
	static	HWND	hWND;
	static	BOOL	Init(HWND hwnd=NULL);
	static	void	Update();
	static	void	Reset();
};

template <class T>
class	cDEN	//	시리얼이 16비트다.
{
	T					*m_pList;
	T					*m_lpList;
private:
	int					m_iLimitCount;
	int					m_iCount,m_iRookie;
	BOOL				m_bIsNotRemainValidItem;	//	리스트 중간에 빈자리를 남기지 않는다.(지우면 땡기라는 얘기.. -_-a)
	int					m_iCurrentIndex;

public:
						cDEN()
						{
							m_pList					=	NULL;
							m_iLimitCount			=	0;
							m_iCount				=	0;
							m_iRookie				=	0;
							m_bIsNotRemainValidItem	=	FALSE;	//	리스트 중간에 빈자리를 남기지 않는다.(지우면 땡기라는 얘기.. -_-a)
						}
						~cDEN(){close();}

	inline	void		setFirst(int _iFirst = 0)	{m_iCurrentIndex=	_iFirst;}
	inline	T*			getNext()
	{
		for (m_iCurrentIndex;m_iCurrentIndex<m_iLimitCount;m_iCurrentIndex++)
		{
			if (m_lpList[m_iCurrentIndex].m_wSerial	!=	0xffff			)
			{
				m_iCurrentIndex++;
				return	&m_lpList[m_iCurrentIndex-1];
			}
		}

		return	NULL;
	}
	inline	T*			get(int _iIndex)
	{
		if (_iIndex						>=	m_iLimitCount	)
			return	NULL;
		if (m_lpList[_iIndex].m_wSerial	==	0xffff			)
			return	NULL;

		return	&m_lpList[_iIndex];
	}

	inline	T*			getList()			{return	m_lpList;}

	inline	int			getCount()			{return	m_iCount;}
	inline	int			getMaxCount()		{return	m_iLimitCount;}

	inline	int			getLastIndex()
	{
		for (int i=m_iLimitCount-1;i;i--)
		{
			if (m_lpList[i-1].m_wSerial	!=	0xffff)
				return	i-1;
		}

		return	-1;
	}

	void				init(int _iCount,BOOL _bIsNorRemainValidItem=FALSE)
	{
		close();

		m_pList					=	new T [_iCount];
		m_lpList				=	m_pList;
		m_iLimitCount			=	_iCount;
		m_bIsNotRemainValidItem	=	_bIsNorRemainValidItem;
	}

	void				init(T *_lpList,int _iCount,BOOL _bIsNorRemainValidItem=FALSE)
	{
		close();

		m_lpList				=	_lpList;
		m_iLimitCount			=	_iCount;
		m_bIsNotRemainValidItem	=	_bIsNorRemainValidItem;
	}

	void				reset()
	{
		for (m_iCount=0;m_iCount<m_iLimitCount;m_iCount++)
			m_lpList[m_iCount].reset();

		m_iCount		=	0;
		m_iRookie		=	0;
	}	//	cDEN<T>::reset()

	void				close()
	{
		pKILL(m_pList);
		m_lpList		=	NULL;

		m_iLimitCount	=	0;
		m_iCount		=	0;
		m_iRookie		=	0;
	}	//	cDEN<T>::close()

	int					add(T* _lpData)
	{
		if (m_iCount	>=	m_iLimitCount)	return	0xffff;

		int	iIndex	=	m_iRookie;

		m_lpList[m_iRookie].copy(_lpData);
		m_lpList[m_iRookie].m_wSerial	=	m_iRookie;

		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				return	iIndex;
			}

		return	iIndex;
	}	//	cDEN<T>::add(T* _lpData)

	inline	T*			getRookie()
	{
		if (m_iCount	>=	m_iLimitCount)
			return	NULL;

		return	&m_lpList[m_iRookie];
	}

	inline	T*			getAvailItem(int _iIndex)
	{
		if (_iIndex	>=	m_iLimitCount)
			return	NULL;

		for(int i = _iIndex;i<m_iLimitCount;i++)
		{
			if (m_lpList[i].m_wSerial	!=	0xffff)	return	&m_lpList[i];
		}

		return	NULL;
	}

	inline	int			getRookieSerial()
	{
		if (m_iCount	>=	m_iLimitCount)
			return	0xffff;

		return	m_iRookie;
	}

	void				increaseCount()
	{
		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				break;
			}
	}

	BOOL				remove(int _iIndex)
	{
		if (m_lpList[_iIndex].m_wSerial	==	0xffff)
			return	FALSE;

		m_lpList[_iIndex].reset();

		m_iCount--;

		if (m_bIsNotRemainValidItem)
		{
			for (int i=_iIndex;i<m_iCount;i++)
			{
				m_lpList[i].copy(&m_lpList[i+1]);
				m_lpList[i].m_wSerial	=	i;
			}

			m_lpList[m_iCount].reset();
			m_iRookie	=	m_iCount;

		}
		else
		if (_iIndex	<	m_iRookie)
			m_iRookie	=	_iIndex;

		return	TRUE;
	}

	void				removeValidItem()
	{
		for (int i=0;i<m_iCount;i++)
		{
			if (m_lpList[i].m_wSerial	!=	0xffff)
				continue;

			T*	lpAvailItem	=	getAvailItem(i+1);

			if (!lpAvailItem)
				return;

			m_lpList[i].copy(lpAvailItem);
			m_lpList[i].m_wSerial	=	i;
			lpAvailItem->reset();
		}
	}

	BOOL				insert(int _iSerial,T *_lpData)
	{
		if (_iSerial	>=	m_iLimitCount	||	_iSerial < 0)
			return	FALSE;

		if (m_lpList[_iSerial].m_wSerial	!=	0xffff)
			return	FALSE;

		m_lpList[_iSerial].copy(_lpData);

		m_lpList[_iSerial].m_wSerial	=	_iSerial;
		m_iCount++;

		if (_iSerial	==	m_iRookie)
			for (int i=m_iRookie;i<m_iLimitCount;i++)
				if (m_lpList[i].m_wSerial	==	0xffff)
				{
					m_iRookie	=	i;

					return	TRUE;
				}

		return	TRUE;
	}

	BOOL				replace(int _iSerial,T *_lpData)
	{
		if (_iSerial	>=	m_iLimitCount	||	_iSerial < 0)
			return	FALSE;
		
		BOOL	bIsReplace	=	FALSE;

		if (m_lpList[_iSerial].m_wSerial	!=	0xffff)
			bIsReplace	=	TRUE;

		m_lpList[_iSerial].copy(_lpData);

		m_lpList[_iSerial].m_wSerial	=	_iSerial;

		if (bIsReplace	==	FALSE)
			m_iCount++;

		if (_iSerial	==	m_iRookie)
			for (int i=m_iRookie;i<m_iLimitCount;i++)
				if (m_lpList[i].m_wSerial	==	0xffff)
				{
					m_iRookie	=	i;

					return	TRUE;
				}

		return	TRUE;
	}

	void				rebuild()
	{
		m_iRookie	=	-1;
		m_iCount	=	0;

		for (int i=0;i<m_iLimitCount;i++)
		{
			if (m_lpList[i].m_wSerial	!=	0xffff)
				m_iCount++;
			else
			{
				if	(m_iRookie	==	-1)
					m_iRookie	=	i;
			}
		}
	}


};
/*
template <class T>
class	CDoubleLinkedList	//	더블 링크드 리스트
{
	T					*m_pList;
	T					*m_lpList;
private:
	int					m_iLimitCount;
	int					m_iCount,m_iRookie;

public:
						CDoubleLinkedList()
						{
							m_pList			=	NULL;
							m_iLimitCount	=	0;
							m_iCount		=	0;
							m_iRookie		=	0;
						}
						~CDoubleLinkedList(){close();}

	inline	T*			getHead()	{
	inline	T*			get(int _iIndex)
	{
		if (_iIndex						>=	m_iLimitCount	)	return	NULL;
		if (m_lpList[_iIndex].m_wSerial	==	0xffff			)	return	NULL;

		return	&m_lpList[_iIndex];
	}

	inline	T*			getList()	{return	m_lpList[_iIndex];}

	inline	int			getCount()			{return	m_iCount;}
	inline	int			getMaxCount()		{return	m_iLimitCount;}

	void				init(int _iCount)
	{
		close();

		m_pList			=	new T [_iCount];
		m_lpList		=	m_pList;
		m_iLimitCount	=	_iCount;
	}

	void				init(T *_lpList,int _iCount)
	{
		close();

		m_lpList		=	_lpList;
		m_iLimitCount	=	_iCount;
	}

	void				reset()
	{
		for (m_iCount=0;m_iCount<m_iLimitCount;m_iCount++)	m_lpList[m_iCount].reset();

		m_iCount		=	0;
		m_iRookie		=	0;
	}	//	cDEN<T>::reset()

	void				close()
	{
		pKILL(m_pList);
		m_lpList		=	NULL;

		m_iLimitCount	=	0;
		m_iCount		=	0;
		m_iRookie		=	0;
	}	//	cDEN<T>::close()

	int					add(T* _lpData)
	{
		if (m_iCount	>=	m_iLimitCount)	return	0xffff;

		int	iIndex	=	m_iRookie;

		memcpy(&m_lpList[m_iRookie],_lpData,sizeof(T));
		
		m_lpList[m_iRookie].m_wSerial	=	m_iRookie;

		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				return	iIndex;
			}

		return	iIndex;
	}	//	cDEN<T>::add(T* _lpData)

	inline	T*			getRookie()
	{
		if (m_iCount	>=	m_iLimitCount)	return	NULL;

		return	&m_lpList[m_iRookie];
	}

	inline	int			getRookieSerial()
	{
		if (m_iCount	>=	m_iLimitCount)	return	0xffff;

		return	m_iRookie;
	}

	void				increaseCount()
	{
		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				break;
			}
	}

	BOOL				remove(int _iIndex)
	{
		if (m_lpList[_iIndex].m_wSerial	==	0xffff)	return	FALSE;

		m_lpList[_iIndex].reset();

		if (_iIndex	<	m_iRookie)	m_iRookie	=	_iIndex;

		m_iCount--;

		return	TRUE;
	}

	BOOL				insert(int _iSerial,T *_lpData)
	{
		if (m_lpList[_iSerial].m_wSerial	!=	0xffff)	return	FALSE;

		memcpy(&m_lpList[_iSerial],_lpData,sizeof(T));

		m_lpList[_iSerial].m_wSerial	=	_iSerial;
		m_iCount++;

		for (int i=m_iRookie;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				return	TRUE;
			}

		return	TRUE;
	}


};
*/

//////////////////////////////////////////////////////////////////////////////////

//	class	cMCU	:	메모리 관리

//////////////////////////////////////////////////////////////////////////////////
extern	DWORD	GetTotalMemory();
extern	DWORD	GetAvailMemory();

#ifdef	dDEBUG_MODE
#define	mTEMPALLOC(a,b)	TempAlloc(a,b)
#else
#define	mTEMPALLOC(a,b)	TempAlloc(a)
#endif

class cMCU
{
	DWORD			FULLSIZE,POINTER,TEMPALLOC;
	char*			m_pMEMORY;
	char			m_strName[64];
#ifdef	dDEBUG_MODE
	DWORD			m_dwLog[102400],m_dwLogCount;
#endif

public:

					cMCU();
					~cMCU();

	BOOL			Init(DWORD size,char *name);

#ifdef	dDEBUG_MODE
	void*			ALLOC(DWORD size,char *name=NULL);
	void*			TempAlloc(DWORD size,char *name);
	void			Check(DWORD address);
#else
	void*			ALLOC(DWORD size);
	void*			TempAlloc(DWORD size);
#endif
	void			TempFree();

	void			Reset();
};


inline	void		SetLogFolder()
{
	SetCurrentDirectory(_logFolder);
}

extern	int			StringToNumber(char *_strText,int _iLimitValue,int _iDecimal);

//////////////////////////////////////////////////////////////////////////////////

//	디버깅용 코드들. 좀 지저분 해도 매우 중요하니 참아라. --;;

//////////////////////////////////////////////////////////////////////////////////
#define	dCHECK_PTR_HEAD			0x12345678					//	포인터를 체크한다. 헤드
#define	dCHECK_PTR_TAIL			0x87654321					//	포인터를 체크한다. 마무리
#define	dCHECK_MCU_HEAD			dCHECK_PTR_TAIL				//	MCU의 포인터를 체크한다. 헤드
#define	dCHECK_MCU_TAIL			dCHECK_PTR_HEAD				//	MCU의 포인터를 체크한다. 마무리
#define	dPTR_STRING_SIZE		64							//	포인터 이름 사이즈

#ifdef _DEBUG			//	디버깅 모드냐?

#define	TEST(err,method)	if (err) method;				//	err이 체크 되면 method를 실행
#define	CRY(err,message)	if (err) ERRMSG(message);		//	err이 체크 되면 method를 실행

//	메모리를 설정 매크로. type 형,numbers 갯수,mcu cMCU 포인터 NULL이면 그냥 메모리에 설정한다. name 디버깅용 포인터 이름
#define	_new(type,numbers,name)		(type *)__new((numbers)*sizeof(type),name)

//	메모리를 해제 매크로. 일단 있는지(?) 확인하고 있으면 체크한 다음에 해제하고 포인터를 초기화 시킨다.
#define	_free(ptr)								\
if (ptr)										\
{	_checkPtr((char *)ptr);						\
	free((char *)(ptr)-(dPTR_STRING_SIZE+8));	\
	ptr		=	NULL;							\
}

//	메모리를 설정 함수. type 형,numbers 갯수,mcu cMCU 포인터 NULL이면 그냥 메모리에 설정한다. name 디버깅용 포인터 이름
inline void*  __new(unsigned int size,char *name)
{	char	*ptr	=	(char *)malloc(size+4+4+4+dPTR_STRING_SIZE);	//	size + head(4) + size(4) + str(64) + tail(4)

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
		ERRMSG(_ms("Error!! pointer %s's HEAD offset damaged!!",ptr+8));

	if (tail	!= dCHECK_PTR_TAIL)	ERRMSG(_ms("Error!! pointer %s's TAIL offset damaged!!",ptr+8));
};

#else	//	#ifdef _DEBUG	디버깅 모드가 아니다.

//	에러체크 안한다.
#define	TEST(a,b)		(void(0))
#define	CRY(a,b)		(void(0))


//	메모리 할당 매크로... mcu 유닛이 있다면 그 넘한테 설정하고 아니면... 걍 한다.
#define	_new(type,numbers,c)	(type *)__new((numbers)*sizeof(type))
inline void*  __new(unsigned int size)
{	void*	ptr;

	ptr		=	(void *)malloc(size);

	return ptr;
}

//	메모리 해제 매크로... 이 메크로가 참조한 매크로인지 확인한다.
#define	_free(ptr)											\
if (ptr)													\
{	free(ptr);												\
	ptr	=	NULL;											\
}


#endif

inline	GetTextCheckSum(char *_lpstrText)
{
	int	iCheckSum	=	0;
	int	iStrsize	=	strlen(_lpstrText);

	for (int i=0;i<iStrsize;i++)
		iCheckSum	+=	_lpstrText[i];

	return	iCheckSum;	
}

inline	void	RemoveQuotationMark(char *_lpstrText)
{
	if (!_lpstrText)
		return;

	int	iLength	=	strlen(_lpstrText);

	while(iLength)
	{
		if (_lpstrText[iLength-1]	==	'\"')
			_lpstrText[iLength-1]	=	'\'';

		iLength--;
	}
}

inline unsigned long
GetHashCode(BYTE* _lpString)
{
	int	iResult=0,iXOR=0,iCH=0,i=0;

	for (;_lpString[i];i++)
	{
		iCH		=	_lpString[i];

		if (iCH >= 'a' && iCH <= 'z')
			iCH	-=	('a'-'A');

		iResult	=	( ( iResult << 8 ) | iCH ) % 16777213UL; // 16777213UL
		iXOR		^=	iCH;
	}

	return iResult | (iXOR<<24);
}

inline unsigned long
GetHashCode(char* _lpString)
{
	return	GetHashCode((BYTE*) _lpString);
}

#endif