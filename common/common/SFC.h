//////////////////////////////////////////////////////////////////////////////////********
//	SFC.H

//	뫈瞼�E유�E� 루틴�E뫈瞼�E유�E� 매크로
//	MMX레지스터와 FPU레지스터를 이�E� 메모리 �E�.

//	작성 날짜 : 1999.02.22
//	1�E 갱신 : 1999.03.15	-	* MMX레지스터와FPU레지스터를 이�E� 메모리 �E倍煞�
//								* UTIL.H -> GLOBAL.H 로 바꿈.
//	2�E 갱신 : 1999.03.24	-	* cRECT global class를 추가함을 시작으로 �E� 클래스 작성.
//	3�E 갱신 : 1999.05.14  -   * sine,cosine 테이�E추가
//	4�E 갱신 : 2000.06.09	-	* sine,cosine 테이�E제거

//								* cSTRING,cSTRINGS,cFOLDER,cFILE,cRECT,cRECTS,cRECT_SIZE,cRECTS_SIZE,cPOINT,cPOINTS class
//								추가 및 교정

//								* 압�E클래스 cNUX 추가

//								* _ms,_exportFileName,_exportFolderName,_exportExt,_setExt,_fileSize,IsFolder
//								따洑 �E� 함�E추가및 교정

//								* �E� 변�Eg_strLogFolder 추가(프로그램이 실행된 폴�E

//								* GLOBAL 을 SFC로 바꿈

//	5�E 갱신 : 2000.06.22	-	* _fileSize 제거
//								* cFILE를 cFILES로 바꿈
//								* cFILE 화일 제�E클래스 추가
//								* cNUX 화일 병합 기능 추가
//								* IsFile 추가
//								* cFILES 클래스 제거. -.- cFOLDER�E합쳐 버렸다.


/////////////////////////////////////////////////////////////////////////////////*********/

#ifndef _classSFC_H
#define _classSFC_H

#include <winsock2.h>
//#ifndef _WINDOWS_
#include <windows.h>
//#endif
#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
#define	dDEBUG_MODE		1
#endif

//#define	dDEBUG_MODE				1

#define	WM_DEBUGTEXT			(WM_USER+2)

#define	dGET_FILE_NAME			0x480
#define	dGET_FILE_EXT			0x470

const	unsigned int	d1MEGA	=	1024*1024;

#define pKILL(buff) if (buff) {delete [] buff	;buff=NULL;}	// 포인터 삭제..
#define KILL(buff)	if (buff) {delete buff		;buff=NULL;}	// 클래스 삭제..
#undef  ATTEMPT
#define ATTEMPT(x)	{ if (FAILED(x)) goto ERROR_LABEL; }		//	에러 처리..
#undef  RELEASE
#define RELEASE(x)	{ if (x != NULL) {x->Release(); x = NULL;} }//	DX개체 삭제..

#define	MSGOUT		cMSG::Output
#define	ERRMSG		cMSG::Error

#define	ERRORED		Errored()
#define	SAFETY		Safety()

namespace	nsSfc
{

template <class A> inline void swap(A &a, A &b)
{   A   c;
    c = a;
    a = b;
    b = c;
}

};


//	�E� 변수�E
extern	BOOL		bRUNAPPLICATION,bACTIVEAPP;				//	엉柴리케이션의 상태 
extern	char		g_strLogFolder[1024];					//	프로그램 실�E폴�E
extern	int			g_iScreenWidth,g_iScreenHeight;			//	화�E사이�E
//	�E� 함수�E
extern	void		memset16(WORD *dest,WORD value,unsigned int size);	//	16비트 �E.
extern	int			random(int range);									//	랜덤함�E
extern	int			largeRandom(int range);									//	랜덤함�E
extern	inline BOOL	SafeRange(int value,int minValue,int maxValue);		//	�E� 체크
extern	inline void	RotateValue(int &value,int maxValue);				//	크기를 체크해서 크�E0으로..
extern	inline void	ClipValue(int &value,int minValue,int maxValue);	//	크기를 체크해서 자른다.
extern	inline BOOL	SetLogFolder(){return	SetCurrentDirectory(g_strLogFolder);}
extern	BOOL		CheckName(char *_strName,int _iNameLimit=4);
extern	int			GetLastDay(int _iYear,int _iMonth);


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
extern	char		*_exportFolderName(char *str);			//	폴�E이름만 뽑아 낸다.
extern	char		*_exportExt(char *str);					//	확장자만 뽑아 낸다.
extern	void		_setExt(char *srcStr,char *destStr);	//	확장자를 설정한다.
extern	BOOL		IsFolder(char *fn);						//	이 녀석이 폴더냐?
extern	BOOL		IsFile(char *fn);						//	이 녀석 화일이냐?
extern	BOOL		Safety();								//	안�E求�!!
extern	BOOL		Errored();								//	에러가 났다!!
extern	void		EXIT(char *title,char *main,...);					//	걍 끝낸다.
extern	BOOL		ustrncpy(char *dest,char *src,int size);

extern	inline		BOOL
IsNumberText(char *_lpstrText)
{
	int	iLength	=	strlen(_lpstrText);

	if (iLength	==	0)
		return	FALSE;

	for (;iLength;iLength--)
	{
		if ((BYTE)_lpstrText[iLength-1] > '9' || (BYTE)_lpstrText[iLength-1] < '0')
			return	FALSE;
	}

	return	TRUE;
}

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

typedef union UNION_LARGE_INTEGER 
{
    struct 
	{
        DWORD	low;
        int		high;
    };
    LONGLONG	value;
}	uLI;

class	cIndexForQSort
{
public:
	int 	m_iValue;
	int 	m_iIndex;
};

extern	int Compare( const void *a, const void *b);
extern	int DesCompare( const void *a, const void *b);

class cPOINT
{
public:
	int		x,y;

	void	Set(int a,int b)	{x =a,y =b;}
	void	Move(int a,int b)	{x+=a,y+=b;}
	BOOL	Match(cPOINT *pos);
	BOOL	Match(int a,int b);

	void	AdjustScreen(HWND hwnd);
	void	SetMousePos(HWND hwnd);
	void	SetMousePos();

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

	cRECT	operator=(const cRECT & );

	void	Set(int a1,int b1,int a2,int b2);
	void	OutFrame(HWND hwnd);
	void	ClientFrame(HWND hwnd);

	void	Move(int x,int y);
	void	Add(int x,int y);

	int		getRange(int _iX,int _iY,BOOL _bIsOval);
	
	inline	BOOL	Isin(int x,int y)
			{
				if (x < x1) return FALSE;
				if (x > x2) return FALSE;
				if (y < y1) return FALSE;
				if (y > y2) return FALSE;
				
				return TRUE;
			}

	inline	BOOL	Crash(cRECT *compare)
			{	if (compare->x1 > x2) return FALSE;
				if (compare->x2 < x1) return FALSE;
				if (compare->y1 > y2) return FALSE;
				if (compare->y2 < y1) return FALSE;

				return TRUE;
			}

	inline	void	expand(int _iXS,int _iYS)	//	사이�E키�EE
	{
		x1	-=	_iXS,y1	-=	_iYS,x2	+=	_iXS,y2	+=	_iYS;
	}
	
	void	Reform();

	int		Left()					{return min(x1,x2);}
	int		Right()					{return max(x1,x2);}
	int		Top()					{return min(y1,y2);}
	int		Bottom()				{return max(y1,y2);}

	DWORD	Width()					{return Right()-Left()+1;}
	DWORD	Height()				{return Bottom()-Top()+1;}
	DWORD	Size()					{return Width()*Height();}

	void	Center(int &a,int &b)	{a=Left()+Width()/2,b=Top()+Height()/2;}
	void	adjustScreen(HWND hwnd);
	inline	void	getRandomPosInRect(int* _iX,int* _iY)
	{
		*_iX	=	x1+random(x2-x1+1);
		*_iY	=	y1+random(y2-y1+1);
	}
};

class cRECTS
{
public:
	short	x1,y1,x2,y2;

	cRECTS	operator=(const cRECTS & );

	void	Set(short a1,short b1,short a2,short b2);
	void	OutFrame(HWND hwnd);
	void	ClientFrame(HWND hwnd);

	void	Move(short x,short y);
	void	Add(short x,short y);

	inline	BOOL	Isin(int x,int y)
			{
				if (x < x1) return FALSE;
				if (x > x2) return FALSE;
				if (y < y1) return FALSE;
				if (y > y2) return FALSE;
				
				return TRUE;
			}

	inline	BOOL	Crash(cRECTS *compare)
			{	if (compare->x1 > x2) return FALSE;
				if (compare->x2 < x1) return FALSE;
				if (compare->y1 > y2) return FALSE;
				if (compare->y2 < y1) return FALSE;

				return TRUE;
			}


	void	Reform();

	short	Left()					{return min(x1,x2);}
	short	Right()					{return max(x1,x2);}
	short	Top()					{return min(y1,y2);}
	short	Bottom()				{return max(y1,y2);}

	DWORD	Width()					{return Right()-Left()+1;}
	DWORD	Height()				{return Bottom()-Top()+1;}
	DWORD	Size()					{return Width()*Height();}

	void	Center(short &a,short &b)	{a=Left()+(Right()-Left())/2,b=Top()+(Bottom()-Top())/2;}

};

class	cCRC32
{
	static	inline	void	CalcCrc32(const BYTE byte, DWORD &dwCrc32);
public:
	static	BOOL			StringToCrc32(char *_lpstrString,DWORD &_dwCrc32);
	static	BOOL			BufferToCrc32(void *_lpBuffer,int _iBufferSize,DWORD &_dwCrc32);
	static	BOOL			FileToCrc32(char *_lpstrFileName,DWORD &dwCrc32);

};

class cSTRING
{
public:
	char			*String;
	int				Length;					//	문장의 사이�E	
	int				Value;
	cSTRING			*pPRE,*pNEXT;			//	이�E이후

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

#define	dSORT_BY_EXT	0
#define	dSORT_BY_NAME	1
#define	dSORT_ASC		0
#define	dSORT_DESC		1
#define	dSORT_NOT		2

class cSTRINGS
{
public:
	int			maxString,Count;	//	한�E문장수와 현�E문장�E	
	cSTRING		*pHEAD,*pTAIL;		//	처음�E마지막
	int			CurString;			//	현�E문�E
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

	BOOL				writeString(char *_lpstrString);
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
	static	BOOL		LoadBox(char *fname,char *filter,char *defaultExt,char *folder=NULL,char *title="Load data",DWORD _dwStyle=0);
	static	BOOL		SaveBox(char *fname,char *filter,char *defaultExt,char *folder=NULL,char *title="Save data",DWORD _dwStyle=0);

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

	static	BOOL		Delete			(char *delDir);				//	폴더의 화일을 비롯한 모든걸 지웝芩.(지�EE없는 거�E속성을 바꿔서 지웝芩.)
	static	BOOL		Create			(char *folder,BOOL changeFolder=FALSE);				//	폴더를 만든다.
	static	BOOL		Change			(char *folder);										//	폴더를 이동한다.

	static	BOOL		getFolders		(char *root,cSTRINGS *folders);		//	폴더들을 얻�E온다.
	static	cSTRINGS*	GetFolders		(char *root);		//	폴더들을 얻�E온다.
	static	cSTRINGS*	GetFiles		(char *ext="*.*",char *root=NULL,BOOL includeFolder=FALSE,WORD sort=dSORT_BY_EXT);//	화일들을 얻�E온다.
	static	cSTRINGS*	GetFiles		(WORD sort);																//	화일들을 얻�E온다.

	static	DWORD		GetFileCount	(char *ext="*.*",char *folder=NULL,BOOL includeFolder=FALSE,BOOL includeSub=FALSE);	//	폴더안에 화일의 �E	
	static	DWORD		GetFolderCount	(char *folder=NULL);										//	폴더의 �E	
	static	DWORD		GetFolderSize	(char *ext="*.*",char *root=NULL,BOOL includeSub=FALSE);	//	폴더안에 화일들의 사이�E
	static	int	_cdecl	Compare			(const void *a, const void *b);
	static	char		*Tree(HWND hwnd,BOOL change,char *title,char *folder=NULL,char *dest=NULL);	//	폴더트리 쉘
};

//	압�E화일 병합 �E�..
#define	dMAX_MERGE_FILE	2046

class cMERGEINFO
{
public:
	char	FName[40];
	DWORD	Locate;
	DWORD	Size;
};

class	cNUX		//	라틴엉踏 호두라는 �E사�E� cNUX.cpp를 링크해야 한다. 
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
	static	char		CurrentFile[128];		//	현�E압축하�E있는 화일 혹은 압축을 풀�E있는 화일
	static	char		ArcName[128];			//	압�E화일
	static	BOOL		s_bIsMerge,s_bIsMergeFolder;

	static	BOOL		PressCreate(char *fn,BOOL _bIsMerge=FALSE);															//	새로�E압축화일 생성
	static	BOOL		PressAppend(char *fn,BOOL _bIsMerge=FALSE);
	static	BOOL		PressFile(char *fn);															//	화일 추가
	static	void		PressAllFile();																	//	현�E폴더에 모�E화일을 추가한다.
	static	BOOL		PressFolder(char *fn,BOOL includeCurrent=FALSE,BOOL includeSubFolder=FALSE);	//	폴더안에 모�E화일을 추가
	static	DWORD		PressClose();																	//	압축화일 닫�E	
	static	void		Stop();																			//	작업을 중지한다.
	static	BOOL		PressExtract(char *fn,char *destFolder,cSTRINGS *files=NULL);					//	압축화일 풀�E
	//	�E압�E�EE8Byte(화일 수와 �EE위치)
	static	BOOL		MergeCreate(char *fn);														
	static	BOOL		MergeAppend(char *fn);
	static	BOOL		MergeFile(char *fn);
	static	BOOL		MergeClose();
	static	BOOL		MergeExtract(char *fn,char *destFolder,cSTRINGS *files=NULL);
	static	void		MergeAllFile();
	static	BOOL		MergeFolder(char *fn,BOOL includeCurrent=FALSE,BOOL includeSubFolder=FALSE);
	static	void		mergeFolder(char *fn);

	static	void		SetFunc(void (*press)(),void (*extract)());
};

class	CLogFile
{
public:
	char				m_strFileName[64];
	DWORD				m_dwNameHashCode;
	cFILE				m_file;
	CRITICAL_SECTION	m_csLogFile;

	void				init(char *_lpstrFileName,char *_lpstrLogFolder=NULL,BOOL _bIsStandAloneFolder=FALSE);
	void				close();
	void	_cdecl		writeLog(char *_lpstrMsg,...);
	void	_cdecl		writeLogWithoutTime(char *_lpstrMsg,...);
	void				_writeLog(char *_lpstrMsg,BOOL _bIsWantCarraigeReturn=TRUE);
	void				_writeLogWithoutTime(char *_lpstrMsg,BOOL _bIsWantCarraigeReturn=TRUE);
};

#define	dMAX_LOG_FILE_COUNT	100

class	CLogManager
{
	BOOL				m_bIsStandAlone;	//	시작 폴�E무시
	int					getLogFile(char *_lpstrFileName);
	char				*m_lpstrLogFile;
public:
	int					m_iLogFileCount,m_iCommonLog;
	CLogFile			m_aLogFile[dMAX_LOG_FILE_COUNT];
	char				m_strLogFolder[32];

						CLogManager();
						~CLogManager();

	void				setLogFolderName(char *_lpstrFolder,BOOL _bIsStandAlone=FALSE);
	int		_cdecl		writeLogWithoutTime(char *_lpstrFileName,char *_lpstrMsg,...);
	int		_cdecl		writeLog(char *_lpstrFileName,char *_lpstrMsg,...);
	void	_cdecl		writeLog(int _iFileIndex,char *_lpstrMsg,...);
	int		_cdecl		writeLog2(char *_lpstrFileName,char *_lpstrMsg,...);
	int					add(char *_lpstrFileName);
};

extern	CLogManager	g_log;

#define	CLOG	g_log.writeLog
#define	CLOGWT	g_log.writeLogWithoutTime
#define	CTLOG	g_log.writeLog2

class cMSG
{
public:
	static	char				msgBuffer[1024];
	static	char				s_strLogFile[1024];
	static	char*				DXERROR(HRESULT hErr);
	static	HWND				hDEBUGWND;
	static	cFILE				s_fileLog;
	static	CRITICAL_SECTION	s_csLogFile;
	static	cFILE				s_fileCLog;
	static	BOOL				s_bIsActive;

public:

	static	void		Disable();

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

	static	BOOL _cdecl Error(char *headmsg,char *mainmsg=NULL,...);
	static	BOOL		Error(HRESULT result){return Error(DXERROR(result));}
	static	BOOL		Error(HRESULT result,char *where)
						{	return Error(where,DXERROR(result));
						}
};

//////////////////////////////////////////////////////////////////////////////////
//	class	cTIMER	:	타이머 처리
//////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib,"winmm.lib")

class CTimeInfo
{
public:
	static	char	s_strText[60];
	static	char	s_strText2[60];
	WORD			m_wYear,m_wMonth,m_wDay,m_wHour,m_wMinute,m_wSecond,m_wWeekDay;

					CTimeInfo();

	void			reset()
	{
		memset(this,0,sizeof(CTimeInfo));
	}
	void			update();

	void			increaseSecond(int _iSecond);
	void			increaseMinute(int _iMinute);
	void			increaseHour(int _iHour);
	void			increaseDay(int _iDay);

	void			decreaseSecond(int _iSecond);
	void			decreaseMinute(int _iMinute);
	void			decreaseHour(int _iHour);
	void			decreaseDay(int _iDay);

	double			getTimeValue();
	int				getDayOfWeek();
	
	int				getGapDay(CTimeInfo *_lpTime);
	int				getMinuteValue(int _iCorrectYear=2000);	//	=2000을 넣�E싶지만... 이제 와서 넣으�E무슨 사과瞼 터질�E몰�E안 넣는다.
	int				getDayValue(int _iCorrectYear=2000);

	inline	int		getSecondValue(int _iCorrectYear=2000)
	{
		return	getMinuteValue(_iCorrectYear)*60+m_wSecond;
	}
	char*			getTime(BOOL _bIsYear=TRUE,BOOL _bIsMonth=TRUE,BOOL _bIsDay=TRUE,BOOL _bIsHour=TRUE,BOOL _bIsMinute=TRUE,BOOL _bIsSecond=TRUE);

	static	char*	GetTime(BOOL _bIsYear=TRUE,BOOL _bIsMonth=TRUE,BOOL _bIsDay=TRUE,BOOL _bIsHour=TRUE,BOOL _bIsMinute=TRUE,BOOL _bIsSecond=TRUE);
	static	char*	GetTime2();
	static	char*	GetTimeForLog();
};

class cTIMER
{
	static	int			s_iTps;			//	tps 초�E뫈礫이나 튀는가!!??
	static	LONGLONG	s_llTps;
	static	int			s_iFps,s_iFrameCounter,s_iLastTime;	//	FPS 측정을 위한 수치

	int					m_iTick;		//	이름 그�E�.. Frame Per Second
	int					m_iOldTime;		//	프레임 스키핑을 위한.. 시간..
	int					m_iOverTick;
	int					m_iSkipCount;	//	프레임 스킵한 횟�E
	int					m_iOverTime;

	int					m_iTimerStartTime;
	LONGLONG			m_llOldTime;
	LONGLONG			m_llOverTime;
	LONGLONG			m_llTick;
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

	inline	void		resetOverTime()
	{
		m_llOverTime=	0;
		m_iOverTime	=	0;
	}

	BOOL				setFPS(int fps);	//	초�E뫈時레임으로 돌릴깩�E?
	BOOL				isSkipFrame();
	void				resetFrameSkip();
	BOOL				syncFrame();

	static	inline	LONGLONG	getMiliSec(LONGLONG _llTime)
	{
		_llTime	=	_llTime*1000000/s_iTps;

		return _llTime;
	}

	static	inline	int	GetTime()
	{
		LARGE_INTEGER	time;

		QueryPerformanceCounter( &time );

		return (int)(time.LowPart>>2);	//	32비트 풀로 받으�E계�E歐E짱나니�E. 30비트로 바꾼다..
										//	사인�E플래기가 엉氷구 저쩌구.. 궁시렁..
	}
	static	inline	int	GetFPS(){return s_iFps;}
	static	int			UpdateFPS();
};

//////////////////////////////////////////////////////////////////////////////////

//	class	cMCU	:	메모리 �E�

//////////////////////////////////////////////////////////////////////////////////
extern	DWORD	GetTotalMemory();
extern	DWORD	GetAvailMemory();

class cMCU
{
	DWORD			m_dwFullSize,m_dwPointer,m_dwTempAlloc;
	WORD			m_wIsSharedMemory;
	char*			m_pMemory;
	char			m_strName[64];
#ifdef	dDEBUG_MODE
	DWORD			m_dwLog[102400],m_dwLogCount;
#endif

public:

					cMCU();
					~cMCU();

	BOOL			init(DWORD size,char *name);
	BOOL			initBySharedMemory(void *_lpMemory,DWORD size,char *name);

#ifdef	dDEBUG_MODE
	void*			alloc(DWORD size,char *name=NULL);
	void*			tempAlloc(DWORD size,char *name);
	void			check(DWORD address);
#else
	void*			alloc(DWORD size);
	void*			tempAlloc(DWORD size,char *name);
#endif
	void			tempFree();
	DWORD			getRemainMemorySize();
	DWORD			getFilledMemorySize();

	void			reset();
};

//////////////////////////////////////////////////////////////////////////////////
//	class	cRANDOM	:	예�E가능한 난�E만들�E//////////////////////////////////////////////////////////////////////////////////
class	cRANDOM
{
private:
	DWORD	m_dwGen1,m_dwGen2,m_dwSeed;

public:
	BOOL	ready(DWORD _dwGen1,DWORD _dwSeed)
	{
		if	(_dwGen1	>=	0xffffffff/2)	return	FALSE;
		if	(_dwSeed	>=	_dwGen1		)	return	FALSE;

		m_dwGen1	=	_dwGen1;
		m_dwGen2	=	_dwGen1*2;
		m_dwSeed	=	_dwSeed;

		return	TRUE;
	}

	inline	DWORD	random(DWORD _dwLimit)
	{
		m_dwSeed	=	((m_dwGen1*m_dwSeed)+m_dwGen2)%_dwLimit;

		return	m_dwSeed;
	}
};


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

		reset();
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

		reset();
	}

	inline	void		reset()//리셋
	{
		m_iCount		=	0;
		m_iRookie		=	0;
		
		if	(m_lpList)
			for (int i=m_iLimitCount;i;)
			{
				--i;
				m_lpList[i].reset();
			}
	}

	void				close()//닫�E
	{
		reset();
		
		pKILL(m_pList);
		pKILL(m_pOrderingBuffer);

		m_lpList		=	NULL;
	}

	inline	T*			get(int _iSerial)//_iIndex번째 멤�E리턴
	{
		if	(_iSerial						>=	m_iLimitCount	)
			return	NULL;
		if	(m_lpList[_iSerial].m_wSerial	==	0xffff			)
			return	NULL;

		return	&m_lpList[_iSerial];
	}
	inline	T*			getByOrdering(int _iIndex)	//	순차적으로 모든걸 검색할때 사�E
	{
		if (_iIndex	>=	m_iCount	||	_iIndex	<	0)
			return	NULL;

		return	&m_lpList[m_pOrderingBuffer[_iIndex]];
	}
	inline	T*			notCheckedget(int _iIndex)	//_iIndex번째 멤�E리턴
	{
		return	&m_lpList[_iIndex];
	}
	inline	T*			getList()			{return	m_lpList;}//리스트 자체를 리턴
	inline	int			getCount()			{return	m_iCount;}//현�E카웝獸
	inline	int			getMaxCount()		{return	m_iLimitCount;}//최�E인원�E
	inline	BOOL		isFull()
	{
		if (getCount() == getMaxCount())
			return	TRUE;

		return	FALSE;
	}
	int					add(T* _lpData)//멤�E추가
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
		if	(m_iCount	>=	m_iLimitCount)
			return	NULL;

		*_lpSerial	=	m_iRookie;

		return	&m_lpList[m_iRookie];
	}

	inline	int			getRookieSerial()//루키 시리�E리턴
	{
		if	(m_iCount	>=	m_iLimitCount)
			return	0xffff;

		return	m_iRookie;
	}

	inline	void		increaseCount()//카웝獸 증가
	{
		if	(m_lpList[m_iRookie].m_wSerial	==	0xffff)
			return;

		if	(m_pOrderingBuffer)
			m_pOrderingBuffer[m_iCount]	=	m_iRookie;

		m_iCount++;

		for (int i=m_iRookie+1;i<m_iLimitCount;i++)
			if (m_lpList[i].m_wSerial	==	0xffff)
			{
				m_iRookie	=	i;

				break;
			}
	}

	BOOL				remove(int _iIndex)//멤�E제거
	{
		if	(_iIndex	<	0	||	_iIndex	>=	m_iLimitCount	||	m_lpList	==	NULL	||	m_lpList[_iIndex].m_wSerial	==	0xffff)
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

	BOOL				replace(int _iSerial,T *_lpData)	//특정 인덱스에 멤�E추가하거나 덮�E쓰�E
	{
		BOOL	bIsExist	=	FALSE;

		if	(m_lpList[_iSerial].m_wSerial	!=	0xffff)
			bIsExist	=	TRUE;

		m_lpList[_iSerial].copy(_lpData);
		m_lpList[_iSerial].m_wSerial	=	_iSerial;

		if	(bIsExist	==	FALSE)
		{
			if	(m_pOrderingBuffer)
				m_pOrderingBuffer[m_iCount]	=	_iSerial;
			
			m_iCount++;

			for (int i=m_iRookie;i<m_iLimitCount;i++)
				if (m_lpList[i].m_wSerial	==	0xffff)
				{
					m_iRookie	=	i;

					return	TRUE;
				}
		}

		return	TRUE;
	}

	BOOL				insert(int _iSerial,T *_lpData)//특정 인덱스에 멤�E추가
	{
		if	(m_lpList[_iSerial].m_wSerial	!=	0xffff)
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

//	class	cTHREAD	:	쓰레�E
//////////////////////////////////////////////////////////////////////////////////
class cTHREAD
{
public:
	HANDLE		ThreadHandle;	// 스레�E핸�E
	DWORD		ThreadAddr;     // 스레�E주소

				cTHREAD();
				~cTHREAD();

	void		Create(HWND hwnd,void (__cdecl *func)(void *) );
	void		Close();

};

#define	mCS(cs)	CCriticalSection c_s(&cs)	//	크리티컬 섹션 시작

class	CCriticalSection
{
public:
	CRITICAL_SECTION	*m_lpCs;

	CCriticalSection(CRITICAL_SECTION * _lpCs)
	{
		m_lpCs	=	_lpCs;
		EnterCriticalSection(m_lpCs);
	}

	~CCriticalSection()
	{
		LeaveCriticalSection(m_lpCs);
	}
};


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

//////////////////////////////////////////////////////////////////////////////////

//	디버�EE코드�E 좀 지저분 해도 매�E중요하니 �E틋E --;;

//////////////////////////////////////////////////////////////////////////////////
#define	dCHECK_PTR_HEAD			0x12345678					//	포인터를 체크한다. �EE
#define	dCHECK_PTR_TAIL			0x87654321					//	포인터를 체크한다. 마무리
#define	dCHECK_MCU_HEAD			dCHECK_PTR_TAIL				//	MCU의 포인터를 체크한다. �EE
#define	dCHECK_MCU_TAIL			dCHECK_PTR_HEAD				//	MCU의 포인터를 체크한다. 마무리
#define	dPTR_STRING_SIZE		64							//	포인터 이름 사이�E
#ifdef dDEBUG_MODE			//	디버�E모드냐?
#define	dCHECK_POINTER_SIZE		(64+12)						//	포인터 이름 사이�E
#define	TEST(err,method)	if (err) method;			//	err이 체크 되�Emethod를 실�E
#define	CRY(err,message)	if (err) ERRMSG(message);		//	err이 체크 되�Emethod를 실�E
#define	EXPEL(err)			if (err) goto LABEL_EXILE;

//	메모리를 설정 매크로. type �Enumbers 갯�Emcu cMCU 포인터 NULL이�E그냥 메모리에 설정한다. name 디버�EE포인터 이름

#define	New(type,numbers,mcu,name)		(type *)__new((numbers)*sizeof(type),mcu,name)

//	메모리를 해제 매크로. 일단 있는�E?) 확인하�E있으�E체크한 다음에 해제하�E포인터를 초기화 시킨다.
template <typename T>
void Free(T ptr)
{
	if (ptr)
	{
		_checkPtr((char*)ptr);
		free((char*)(ptr)-(dPTR_STRING_SIZE + 8));
		ptr = NULL;
	}
}

//	메모리를 설정 함�E type �Enumbers 갯�Emcu cMCU 포인터 NULL이�E그냥 메모리에 설정한다. name 디버�EE포인터 이름
inline void*  __new(unsigned int size,cMCU *mcu,char *name)
{	if (mcu)
	{	void	*ptr	=	mcu->alloc(size,name);

		return	ptr;
	}

	char	*ptr	=	(char *)malloc(size+4+4+4+dPTR_STRING_SIZE);	//	size + head(4) + size(4) + str(64) + tail(4)

	*(unsigned int *)(ptr							)	=	dCHECK_PTR_HEAD;			//	�EE	
	*(unsigned int *)(ptr+4							)	=	(size+8+dPTR_STRING_SIZE);	//	꼬리 위치
	*(unsigned int *)(ptr+8+dPTR_STRING_SIZE+size	)	=	dCHECK_PTR_TAIL;			//	꼬리
	strncpy((char *)(ptr+8),name,dPTR_STRING_SIZE-1);									//	이름

	ptr	+=	(dPTR_STRING_SIZE+8);

	return ptr;
};

//	메모리를 체크 함�E 메모리를 해제 하�E�E� p의 앞뒤를 검사해 깨지�E않았는�E확인한다.
inline void _checkPtr(char *p)
{	unsigned int	head,tail,size;

	char *ptr	=	p-(dPTR_STRING_SIZE+8);

	head		=	*(DWORD *)(ptr		);		//	�EE	
	size		=	*(DWORD *)(ptr+4	);		//	�EE	
	tail		=	*(DWORD *)(ptr+size	);		//	�EE
	if (head	!= dCHECK_PTR_HEAD)
	{	if (head	== dCHECK_MCU_HEAD)
				ERRMSG(_ms("Error!! pointer %s are cMCU's Client!!",ptr+8));
		else	ERRMSG(_ms("Error!! pointer %s's HEAD offset damaged!!",ptr+8));
	}

	if (tail	!= dCHECK_PTR_TAIL)	ERRMSG(_ms("Error!! pointer %s's TAIL offset damaged!!",ptr+8));
};

#else	//	#ifdef dDEBUG_MODE	디버�E모드가 아니다.
#define	dCHECK_POINTER_SIZE		0						//	포인터 이름 사이�E
//	에러체크 안한다.
#define	TEST(a,b)				(void(0))
#define	CRY(err,message)		(void(0))
#define	EXPEL(err)				(void(0))


//	메모리 할�E매크로... mcu 유닛이 있다�E그 넘한테 설정하�E아니�E.. 걍 한다.
#define	New(type,numbers,mcu,c)	(type *)__new((numbers)*sizeof(type),mcu)
inline void*  __new(unsigned int size,cMCU *mcu)
{	void*	ptr;

	if (mcu)
	{	ptr	=	mcu->alloc(size);

		return ptr;
	}

	ptr		=	(void *)malloc(size);

	return ptr;
}

//	메모리 해제 매크로... 이 메크로가 �E또� 매크로인�E확인한다.
#define	Free(ptr)											\
if (ptr)													\
{	if (*(DWORD*)((char*)(ptr)-4)	==	dCHECK_PTR_TAIL)	\
	{	ptr	=	NULL;										\
		ERRMSG("Error!! cMCU's Inheritor are destroy!!");	\
	}														\
	free(ptr);												\
	ptr	=	NULL;											\
}

#endif

#ifdef	_FOR_JAPAN
	#ifndef	STRICMP
		#define	STRICMP	lstrcmpi
	#endif	
#elif	_FOR_THAI
	#ifndef	STRICMP
		#define	STRICMP	lstrcmpi
	#endif	
#else
	#ifndef	STRICMP
		#define	STRICMP	stricmp
	#endif	
#endif

#define	STRCMP	STRICMP

extern	int		GetLastDay(int _iYear,int _iMonth);	

#endif