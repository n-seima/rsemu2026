/*****************************************************************************************

	SFC		-	Sonaki Foundation Class (I'm Hate MFC... 실은 무서워 한다. -.-;;

*****************************************************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <shlobj.h>
#include <io.h>

#define	dDEBUG_MODE
#include "SFC.H"

//	FPU레지스터를 이용한 전송을 위해서 필요한 연속되는 메모리(쉽게 말해 16바이트씩 끊는다는 얘기다.. ==;
#define FPU_ALIGN	16 /* 8, 32 */

//	MMX레지스터를 이용한 전송을 위해서 필요한 연속되는 메모리(쉽게 말해 8 바이트씩 끊는다는 얘기다.. ==;
#define MMX_ALIGN	32 /*16, 32 */	// 16바이트로 설정 되어 있었는데.. 왜 그랬을까.. 이유를 아시는 분을
									//	갈켜 주세요..
// FPU혹은 MMX 의 특수한 전송코드를 사용하기 위한 최소한의 정렬된 바이트수 이보다 작음 memcpy를 써버린다.
#define THIN_THRESHOLD 128

// CPU 타입
static DWORD	gProcessorType;
static BOOL		gIsPentiumProcessor		= FALSE;		//	이건 펜티엄이다.
static BOOL		gIsPentiumProProcessor	= FALSE;		//	이건 펜티엄 프로다..
static BOOL		gIsPentiumIIProcessor	= FALSE;		//	이건 펜티엄 II다.
static BOOL		gHasMMXTechnology		= FALSE;		//	이건 MMX가 된다. 이게 젤 중요함..
static BOOL		gFloatingEmulation		= FALSE;		//	FPU!!?
static BOOL		gSupportsCPUID			= TRUE;			//	CPUID를 지원한다.
static char		*gCpuSignature			="Unknown CPU";	//	CPU이름..

// needed since CPUID isn't part of the VC++ 5.0 inline assembler
#define CPUID __asm _emit 0x0F __asm _emit 0xA2

static void *	fpucpy(void *pDest ,const void *pSrc,unsigned int size);
static void *	mmxcpy(void *pDest ,const void *pSrc,unsigned int size);
static DWORD	GetProcessorType(void);
static BOOL		CheckMMXTechnology(void);
static void		EnumerateProcessorType(void);

char	_logFolder[512];						//	프로그램 실행 폴더

void* (*fastcpy)(void *pDest ,const void *pSrc,unsigned int size)=memcpy;

class cSFC
{
public:
		cSFC();
		~cSFC();
};

cSFC temp;

cSFC::cSFC()
{	srand( (unsigned)time( NULL ) );
	EnumerateProcessorType();

	GetCurrentDirectory(sizeof(_logFolder),_logFolder);
#ifdef dDEBUG_MODE
	char time[128];
	FILE *fp;

	_strtime( time );

	fp = fopen("Result.txt", "wb" );
	if (fp)
	{	fprintf( fp, "[%s] Message : This Computer cpu Type - %s\n",time,gCpuSignature);
		if (gIsPentiumProcessor		)	fprintf( fp, "[%s] Message : This Computer equiped Pentium Processor\n",time);
		if (gIsPentiumProProcessor	)	fprintf( fp, "[%s] Message : This Computer equiped Pentium Pro Processor\n",time);
		if (gIsPentiumIIProcessor	)	fprintf( fp, "[%s] Message : This Computer equiped Pentium II Processor\n",time);
		if (gHasMMXTechnology		)	fprintf( fp, "[%s] Message : This Computer Has MMX Technology\n",time);
		if (gFloatingEmulation		)	fprintf( fp, "[%s] Message : This Computer Has Floationg Emulation\n",time);
		fprintf( fp, "\n");
		fclose( fp );
	}
#endif

	if (gHasMMXTechnology		)	fastcpy	=	mmxcpy;
	if (gFloatingEmulation		)	fastcpy	=	fpucpy;

	if (strcmp(gCpuSignature,"Unknown CPU"		)==0)	fastcpy=memcpy;
	if (strcmp(gCpuSignature,"Intel Pentium II"	)==0)	fastcpy=memcpy;
}

cSFC::~cSFC()
{
#ifdef dDEBUG_MODE
	SetCurrentDirectory(_logFolder);
	char time[128];
	FILE *fp;

	_strtime( time );
	fp = fopen("Result.txt", "a+" );
	if (fp)
	{	fprintf( fp, "\n[%s] Message : - 프로그램 정상종료. -\n",time);
		fclose( fp );
	}
#endif
}

int Compare( const void *a, const void *b)
{	return (((tINDEX *)a)->Value-((tINDEX *)b)->Value);
}

void
memset(WORD *dest,WORD value,unsigned int size)
{	UINT *dest32=(UINT *)dest;
	unsigned int width=size;

	__asm
	{	mov edi, dword ptr [dest32]
		mov	ax,value
		ror	eax,16
		mov	ax,value
		mov	ecx,width
		shr	ecx,1
		rep	stosd
		mov	 edx, width
		test edx, 1
		jne __makeworddata1
		jmp	__endmakedata1

__makeworddata1:
		stosw
__endmakedata1:
	}
}

inline int	
random(int range)
{	if (range==0) return 0;
	if (range<0) return -(rand()%(-range));
	return (rand()%range);
}

/************************************************************************************************

	FPU 레지스터를 이용한 메모리 전송과 MMX레지스터를 이용한 메모리 전송 루틴..

************************************************************************************************/


// --------------------------------------------------------------------------
// return values for GetProcessorType
//	Type (bits 13-12), Family (bits 11-8), Model (bits 7-4), Stepping (bits 3-0)
//
//	T = 00, F = 0101, M = 0001, for Pentium Processors (60, 66 MHz)
//	T = 00, F = 0101, M = 0010, for Pentium Processors (75, 90, 100, 120, 133, 150, 166, 200 MHz)
//	T = 00, F = 0101, M = 0100, for Pentium Processors with MMX technology
//
//	T = 00, F = 0110, M = 0001, for Pentium Pro Processor
//	T = 00, F = 0110, M = 0011, for Pentium II Processor
//
//	T = 00 for original OEM processor
//	T = 01 for Intel OverDrive Processor
//	T = 10 for dual processor
//	T = 11 is reserved
static DWORD GetProcessorType(void)
{	volatile DWORD retval;

	__try 
	{	_asm 
		{	mov eax, 1		// set up CPUID to return processor version and features
							//	0 = vendor string, 1 = version info, 2 = cache info
			CPUID			// code bytes = 0fh,  0a2h
			and eax, 03fffh	// type, family, model, stepping returned in eax
			mov retval, eax
		}
	} __except(EXCEPTION_EXECUTE_HANDLER)
	{	retval = 0;
	}

	return retval;
}

//
// VF - NOTE
//
//		Added the 'volatile' keyword to these Intel sources because
//		the compiler was clobbering retval in Release builds with Maximize Speed
//
static BOOL CheckMMXTechnology(void)
{	volatile BOOL retval = TRUE;
	volatile DWORD RegEDX;

	__try 
	{	_asm 
		{	mov eax, 1		// set up CPUID to return processor version and features
							//	0 = vendor string, 1 = version info, 2 = cache info
			CPUID           // code bytes = 0fh,  0a2h
			mov RegEDX, edx	// features returned in edx
	   	}
   	} __except(EXCEPTION_EXECUTE_HANDLER)
	{
		retval = 0;
	}

	if (retval == 0)
	{	gSupportsCPUID=FALSE;
		return FALSE;        	// processor does not support CPUID
	}

	if (RegEDX & 0x800000) 		// bit 23 is set for MMX technology
	{	__try { _asm emms } 	// try executing the MMX instruction "emms"
		__except(EXCEPTION_EXECUTE_HANDLER) { retval = FALSE; }
		return retval;
	}
   	else return FALSE;        	// processor supports CPUID but does not support MMX technology

	// if retval == 0 here, it means the processor has MMX technology but
	// floating-point emulation is on; so MMX technology is unavailable
	gFloatingEmulation=TRUE;

	return retval;
}

//	CPU를 확인하고 프로세서의 환경과 능력을 나타내는 전역 변수들을 초기화한다..
static void EnumerateProcessorType( void )
{	DWORD type, family, model, stepping, signature;

	gProcessorType		= GetProcessorType();
	gHasMMXTechnology	= CheckMMXTechnology();

	type     = (gProcessorType>>12) & 0x3;
	family   = (gProcessorType>>8)  & 0xf;
	model    = (gProcessorType>>4)  & 0xf;
	stepping =  gProcessorType      & 0xf;
	signature=  gProcessorType      & 0x0ff0 ;

	if (family == 5)
	{	gIsPentiumProcessor = TRUE;
		gCpuSignature="Intel Pentium";
	}
	else if (family == 6 && model == 1)
	{	gIsPentiumProProcessor = TRUE;
		gCpuSignature="Intel Pentium Pro";
	}
	else if (family == 6 && model == 3)
	{	gIsPentiumIIProcessor = TRUE;
		gCpuSignature="Intel Pentium II";
	}
	else
	{	switch( signature )
		{	case 0x0500: gCpuSignature="AMD K5 Model 0"; break;
			case 0x0510: gCpuSignature="AMD K5 Model 1"; break;
			case 0x0520: gCpuSignature="AMD K5 Model 2"; break;
			case 0x0530: gCpuSignature="AMD K5 Model 3"; break;
			case 0x0560: gCpuSignature="AMD K6"; break;
			case 0x0400: gCpuSignature="AMD 486/Am5x86"; break;
		}
	}
}

// fpu를 이용해 16바이트 전송을 한다. mmx 8바이트 보다 느리다.. 
static void * fpucpy(void *pDest ,const void *pSrc,unsigned int size)
{	unsigned int	prebytes,qdwords,postbyte=size;

	if( postbyte	>=	THIN_THRESHOLD )
	{	prebytes	= FPU_ALIGN-(((long)pDest)%FPU_ALIGN);
		if( prebytes>postbyte ) prebytes=postbyte;
		postbyte	= postbyte-prebytes;
		qdwords		= postbyte/16;
		postbyte	= postbyte-qdwords*16;

		__asm
		{	MOV		ESI,[pSrc]
			MOV		EDI,[pDest]

			MOV		ECX,[prebytes]
			JCXZ	QDWORDSTART
		REP	MOVS	[EDI],[ESI]

QDWORDSTART:

			MOV		ECX,[qdwords]
			JCXZ	POSTBYTES

ALIGN 16
QDWORDLOOP:
			FILD    QWORD PTR [ESI]        //	이거 뭔지 아시는분 좀 갈켜 주세요.. FPU 전용인가??
			FILD    QWORD PTR [ESI+8]        
			FXCH
			FISTP   QWORD PTR [EDI]
			FISTP   QWORD PTR [EDI+8]

			ADD     ESI, 16
			ADD     EDI, 16
			LOOP	QDWORDLOOP

POSTBYTES:
			MOV		ECX,[postbyte]
			JCXZ	DONE
		REP	MOVS	[EDI],[ESI]

DONE:
		}
	}
	else	memcpy( pDest, pSrc, postbyte );

	return NULL;
}

static void * mmxcpy(void *pDest ,const void *pSrc,unsigned int size)
{	unsigned int	prebytes,qdwords,postbyte=size;

	if( postbyte	>=	THIN_THRESHOLD )
	{	prebytes	= MMX_ALIGN-(((long)pDest)%MMX_ALIGN);
		postbyte	= postbyte-prebytes;
		qdwords		= postbyte/8;
		postbyte	= postbyte-qdwords*8;


		__asm
		{	MOV		ESI,[pSrc]
			MOV		EDI,[pDest]

			MOV		ECX,[prebytes]
			JCXZ	QWORDSTART
		REP	MOVS	[EDI],[ESI]

QWORDSTART:

			MOV		ECX,[qdwords]
			JCXZ	POSTBYTES

ALIGN 16
QWORDLOOP:
			MOVQ    MM0		 ,[ESI]
			MOVQ    [EDI	],MM0
			ADD     ESI		 ,8
			ADD     EDI		 ,8
			LOOP	QWORDLOOP

POSTBYTES:
			MOV		ECX,[postbyte]
			JCXZ	DONE
		REP	MOVS	[EDI],[ESI]

DONE:
			EMMS
		}
	}
	else	memcpy( pDest, pSrc, postbyte );

	return NULL;
}

/*******************************************************************************************
	class cPOINT
		:	쩜을 다룬다.
*******************************************************************************************/

cPOINT
cPOINT::operator=(const cPOINT & pos)
{	Set(pos.x,pos.y);
	return pos;
}

cPOINT	
cPOINT::operator+(const cPOINT & pos)
{	Set(x+pos.x,y+pos.y);
	return pos;
}

cPOINT
cPOINT::operator-(const cPOINT & pos)
{	Set(x-pos.x,y-pos.y);
	return pos;
}

void
cPOINT::SetMousePos(HWND hwnd)
{	GetCursorPos((POINT *)this);
	ScreenToClient(hwnd,(POINT *)this);
}

void
cPOINT::SetMousePos()
{	GetCursorPos((POINT *)this);
}

void
cPOINT::AdjustScreen(HWND hwnd)
{	ClientToScreen(hwnd,(POINT *)this);
}

BOOL	
cPOINT::Match(cPOINT *pos)
{	if (x!=pos->x)	return FALSE;
	if (y!=pos->y)	return FALSE;

	return TRUE;
}

BOOL
cPOINT::Match(int a,int b)
{	if (x!=a) return FALSE;	
	if (y!=b) return FALSE;

	return TRUE;
}

/*
	class cRECT		
		:	rect	스트럭쳐의 확장판이다. 
			박스겹침,영역확인,좌표교정등을 알아서 처리해준다.
*/

void
cRECT::Set(int a1,int b1,int a2,int b2)
{	x1	=	a1;
	y1	=	b1;
	x2	=	a2;
	y2	=	b2;

	if (x1 > x2) swap(x1,x2);
	if (y1 > y2) swap(x1,x2);
}

BOOL
cRECT::Isin(int x,int y)
{	if (x >= x1 && x <= x2 && y >= y1 && y <= y2) return TRUE;

	return FALSE;
}

BOOL
cRECT::Crash(cRECT *compare)
{	if (compare->x1 > x2) return FALSE;
	if (compare->x2 < x1) return FALSE;
	if (compare->y1 > y2) return FALSE;
	if (compare->y2 < y1) return FALSE;

	return TRUE;
}

void
cRECT::Move(int x,int y)
{	x2	+=	(x-x1);
	y2	+=	(y-y1);
	x1	=	x;
	y1	=	y;
}

void
cRECT::Add(int x,int y)
{	x1	+=	x;
	y1	+=	y;
	x2	+=	x;
	y2	+=	y;
}


cRECT
cRECT::operator=(const cRECT &src)
{	Set(src.x1,src.y1,src.x2,src.y2);

	return src;
}

//	cRECT 클래스의 short형 버젼

void
cRECTS::Set(short a1,short b1,short a2,short b2)
{	x1	=	a1;
	y1	=	b1;
	x2	=	a2;
	y2	=	b2;

	if (x1 > x2) swap(x1,x2);
	if (y1 > y2) swap(x1,x2);
}

BOOL
cRECTS::Isin(short x,short y)
{	if (x >= x1 && x <= x2 && y >= y1 && y <= y2) return TRUE;

	return FALSE;
}

BOOL
cRECTS::Crash(cRECTS *compare)
{	if (compare->x1 > x2) return FALSE;
	if (compare->x2 < x1) return FALSE;
	if (compare->y1 > y2) return FALSE;
	if (compare->y2 < y1) return FALSE;

	return TRUE;
}

void
cRECTS::Move(short x,short y)
{	x2	+=	(x-x1);
	y2	+=	(y-y1);
	x1	=	x;
	y1	=	y;
}

void
cRECTS::Add(short x,short y)
{	x1	+=	x;
	y1	+=	y;
	x2	+=	x;
	y2	+=	y;
}

cRECTS
cRECTS::operator=(const cRECTS &src)
{	Set(src.x1,src.y1,src.x2,src.y2);

	return src;
}

//	cRECT 클래스에서 사이즈를 기억한다.
void
cRECT_SIZE::Set(int a,int b,int as,int bs)
{	x	=	a;
	y	=	b;
	xs	=	as;
	ys	=	bs;
}

void
cRECT_SIZE::AdjustScreen(HWND hwnd)
{	cPOINT	pos;

	pos.Set(x,y);
	pos.AdjustScreen(hwnd);

	x=	pos.x;
	y=	pos.y;
}

void
cRECT_SIZE::Add(int a,int b)
{	x	+=	a;
	y	+=	b;
}

void
cRECT_SIZE::Move(int a,int b)
{	x	=	a;
	y	=	b;
}

void
cRECT_SIZE::ClientFrame(HWND hwnd)
{	GetClientRect(hwnd,(RECT *)this);
}

void
cRECT_SIZE::OutFrame(HWND hwnd)
{	GetWindowRect(hwnd,(RECT *)this);

	xs	=	xs-x;
	ys	=	ys-y;
}

BOOL
cRECT_SIZE::Isin(int a,int b)
{	if (x >= a && x+xs-1 <= a && y >= b && y+ys-1 <= b) return TRUE;

	return FALSE;
}

BOOL
cRECT_SIZE::Crash(cRECT_SIZE *compare)
{	if (compare->x			> Right()	) return FALSE;
	if (compare->Right()	< x			) return FALSE;
	if (compare->y			> Bottom()	) return FALSE;
	if (compare->Bottom()	< y			) return FALSE;

	return TRUE;
}

cRECT_SIZE
cRECT_SIZE::operator=(const cRECT_SIZE &src)
{	Set(src.x,src.y,src.xs,src.ys);

	return src;
}

//	cRECT 클래스에서 사이즈를 기억한다.
void
cRECTS_SIZE::Set(short a,short b,short as,short bs)
{	x	=	a;
	y	=	b;
	xs	=	as;
	ys	=	bs;
}

void
cRECTS_SIZE::AdjustScreen(HWND hwnd)
{	cPOINT	pos;

	pos.Set(x,y);
	pos.AdjustScreen(hwnd);

	x=	pos.x;
	y=	pos.y;
}

void
cRECTS_SIZE::Add(short a,short b)
{	x	+=	a;
	y	+=	b;
}

void
cRECTS_SIZE::Move(short a,short b)
{	x	=	a;
	y	=	b;
}

void
cRECTS_SIZE::ClientFrame(HWND hwnd)
{	GetClientRect(hwnd,(RECT *)this);
}

void
cRECTS_SIZE::OutFrame(HWND hwnd)
{	RECT rect;

	GetWindowRect(hwnd,&rect);

	x	=	(short)rect.left;
	y	=	(short)rect.top;
	xs	=	(short)rect.right;
	ys	=	(short)rect.bottom;
	xs	=	xs-x;
	ys	=	ys-y;
}

BOOL
cRECTS_SIZE::Isin(short a,short b)
{	if (x >= a && x+xs-1 <= a && y >= b && y+ys-1 <= b) return TRUE;

	return FALSE;
}

BOOL
cRECTS_SIZE::Crash(cRECTS_SIZE *compare)
{	if (compare->x			> Right()	) return FALSE;
	if (compare->Right()	< x			) return FALSE;
	if (compare->y			> Bottom()	) return FALSE;
	if (compare->Bottom()	< y			) return FALSE;

	return TRUE;
}

cRECTS_SIZE
cRECTS_SIZE::operator=(const cRECTS_SIZE &src)
{	Set(src.x,src.y,src.xs,src.ys);

	return src;
}

/*******************************************************************************************
	class cSTRING
		:	문자열을 다룬다.
*******************************************************************************************/

static char		tempStr[1024];			//	cSTRING	클래스를 위한 템플릿 스트링..

BOOL _makeShotcut(char* src, char* lnkPath, char* lnkName)
{	int			length;
	HRESULT		hres;
	IShellLink	*psl;
	char		posdancuk[256];

	CoInitialize(NULL);

	hres	=	CoCreateInstance(CLSID_ShellLink, NULL,CLSCTX_INPROC_SERVER,IID_IShellLink, (LPVOID*)&psl);

	if(SUCCEEDED(hres))
	{	IPersistFile *ppf;
		hres=psl->QueryInterface(IID_IPersistFile, (void **)&ppf);

		if(SUCCEEDED(hres))
		{	WORD wsz[256];
			psl->SetPath(src);
			psl->SetArguments("");

			char Drive[MAX_PATH];

			for(length=0; length<256; length++)
			{	if(src[length] != '\\')	Drive[length]=src[length];
				else
				{	Drive[length]='\0';
					break;
				}
			}

			int maxn = strlen(src)-1;
		
			
			psl->SetWorkingDirectory(_exportFolderName(src));
			psl->SetIconLocation(NULL, 1);
			psl->SetHotkey(0);
			psl->SetShowCmd(0);

			strcpy(posdancuk,_ms("%s\\%s.lnk",lnkPath,lnkName));

			MultiByteToWideChar(CP_ACP, 0, posdancuk, -1,(LPWSTR)wsz,MAX_PATH);
			hres = ppf->Save( (LPWSTR)wsz, TRUE);
			if (! SUCCEEDED(hres)) 	return hres;
			ppf->Release();
		}
		psl->Release();
	}

	CoUninitialize();

	return TRUE;
}

char	*_exportFileName(char *str)
{	int dest=0;
	strcpy(tempStr,str);

	for (UINT i=0;i<strlen(str);i++)	if (str[i]=='\\') dest=i+1;

	return &tempStr[dest];
}

char	*_exportFolderName(char *str)
{	strcpy(tempStr,str);

	int size = (int)strlen(str)-1;

	for (int i=size;i>=0;i--)	
		if (str[i]=='\\') 
		{	tempStr[i]	=	0;
			return tempStr;
		}

	return NULL;
}

char	*_exportExt(char *str)
{	int dest=0xffff;

	for (UINT i=0;i<strlen(str);i++)	if (str[i]=='.') dest=i+1;

	if (dest==0xffff) return NULL;

	return &str[dest];
}

void	_setExt(char *srcStr,char *destStr)
{	int		dest=0xffff;
	UINT	i,srcLen=strlen(srcStr),destLen=strlen(destStr);

	if (srcLen == 0) return;

	for (i=0;i<srcLen;i++	)	if (srcStr[i]=='.') dest=i+1;
	if	(dest==0xffff		)	srcStr[srcLen]='.',dest=srcLen+1;

	for (i=0;i<destLen;i++	)	srcStr[dest+i]	=	destStr[i];

	srcStr[dest+destLen]	=	NULL;
}

char	*_ms(char *str,...)
{	va_list argptr;
	va_start(argptr, str);
	vsprintf(tempStr,str,argptr);
	va_end(argptr);

	return tempStr;
}

cSTRING::cSTRING()
{	pPRE	=	NULL;
	pNEXT	=	NULL;
	String	=	NULL;
	Length	=	0;
}

cSTRING::cSTRING(char *str,...)
{	va_list argptr;
	va_start(argptr, str);
	vsprintf(tempStr,str,argptr);
	va_end(argptr);

	pPRE	=	NULL;
	pNEXT	=	NULL;
	String	=	NULL;
	Length	=	0;

	Set(tempStr);
}

cSTRING::~cSTRING()	
{	pKILL(String);
	Length	=	0;	
}

char *
cSTRING::Set(char *str,...)
{	pKILL(String);

	va_list argptr;
	va_start(argptr, str);
	vsprintf(tempStr,str,argptr);
	va_end(argptr);

	int len	=	strlen(tempStr);

	String	=	new char [len+1];	
	strcpy(String,tempStr);
	Length	=	strlen(String);

	return String;
}

char *
cSTRING::Add(char *str,...)
{	va_list argptr;
	va_start(argptr, str);
	vsprintf(tempStr,str,argptr);
	va_end(argptr);

	char *temp;

	int len	=	strlen(tempStr);
	if (len==0) return String;

	temp	=	new char [len+Length+1];

	if (!String) strcpy(temp,tempStr);
	else
	{	strcpy(temp,String);
		strcat(temp,tempStr);
	}
	pKILL(String);
	String	=	new char [len+Length+1];
	strcpy(String,temp);
	Length	=	strlen(String);

	return String;
}

char *
cSTRING::Insert(char *str,...)
{	va_list argptr;
	va_start(argptr, str);
	vsprintf(tempStr,str,argptr);
	va_end(argptr);

	char *temp;

	int len	=	strlen(tempStr);
	if (len==0) return String;

	temp	=	new char [len+Length+1];

	if (!String) strcpy(temp,tempStr);
	else
	{	strcpy(temp,tempStr);
		strcat(temp,String);
	}
	Length=len+Length;
	String	=	new char [len+Length+1];
	strcpy(String,temp);
	Length	=	strlen(String);

	return String;
}

BOOL
cSTRING::Match(char *str)
{	if (strcmp(String,str)==0) return TRUE;

	return FALSE;
}

char*
cSTRING::UpperCase(char *str)
{	int count	=	0;

	while(1)
	{	char ch	=	str[count];
		if (ch == NULL) break;
		if (ch>='a' && ch<= 'z')	ch -= 32;
		tempStr[count]	=	ch;
		count++;
	}
	tempStr[count]	=	NULL;

	return tempStr;
}

char*
cSTRING::LowCase(char *str)
{	int count	=	0;

	while(1)
	{	char ch	=	str[count];
		if (ch == NULL) break;
		if (ch>='A' && ch<= 'Z')	ch += 32;
		tempStr[count]	=	ch;
		count++;
	}

	return tempStr;
}

cSTRING
cSTRING::operator=(const cSTRING & str)
{	Set(str.String);
	return cSTRING(str.String);
}

cSTRING
cSTRING::operator+(const cSTRING & str)
{	strcpy(tempStr,String);
	strcat(tempStr,str.String);
	Set(tempStr);
	return cSTRING(tempStr);
}

void
cSTRING::Minus(char *minus)
{	char *src = String;

	while(1)
	{	if (src==NULL) break;

		if (*src!=*minus)
		{	strcpy(tempStr,src);
			strcpy(String,tempStr);
			return;
		}

		src++;
		minus++;

	}

	String	=	NULL;
}


char*
cSTRING::Minus(char *src,char *minus)
{	while(1)
	{	if (*src!=*minus)
		{	strcpy(tempStr,src);
			return tempStr;
		}

		src++;
		minus++;
	}
}

/*********************************************************************************
		문장들(더블 링크드 리스트다.)
*********************************************************************************/

int
cSTRINGS::Compare( const void *a, const void *b)
{
	return stricmp(((cFILENAME *)a)->Name,((cFILENAME *)b)->Name);
		
}

cSTRINGS::cSTRINGS(int limit)
{	Count			=	0;
	CurString		=	0;
	maxString		=	limit;
	pHEAD			=	NULL;
	pTAIL			=	NULL;			//	처음과 마지막
}

cSTRINGS::~cSTRINGS()
{	maxString		=	1000;
	Reset();
}

void
cSTRINGS::Init(int limit)
{	maxString	=	limit;

	Reset();
}

void
cSTRINGS::Reset()
{	Count			=	0;
	CurString		=	0;

	cSTRING	*sen;

	sen	=	pHEAD;

	while(sen	!=pTAIL && sen!=NULL)
	{	sen		=	sen->pNEXT;
		cKILL(sen->pPRE);
	}

	cKILL(sen);

	pHEAD	=	NULL;
	pTAIL	=	NULL;
}

BOOL
cSTRINGS::Add(char *str,BOOL check)
{	if (check)	if (GetIndex(str)!=-1) return FALSE;

	if (!pHEAD)
	{	pHEAD		=	new cSTRING();
		pTAIL		=	new cSTRING();
		pHEAD->pNEXT=	pTAIL;
		pTAIL->pPRE	=	pHEAD;
	}

	cSTRING	*item;

	item				=	new cSTRING();
	item->Set(str);
	
	pTAIL->pPRE->pNEXT	=	item;
	item->pPRE			=	pTAIL->pPRE;
	item->pNEXT			=	pTAIL;
	pTAIL->pPRE			=	item;

	Count++;

	if (Count >= maxString)	Remove(0);

	CurString			=	Count-1;

	return TRUE;
}

BOOL
cSTRINGS::Add(cSTRINGS *strs)
{	if (!strs) return FALSE;

	cSTRING *str=strs->pHEAD->pNEXT;

	while(str->String)
	{	Add(str->String);
		str	=	str->pNEXT;
	}

	return TRUE;
}

BOOL
cSTRINGS::Sort(int term)
{	cFILENAME	*Files;
	int			i,fc=0;

	fc		=	Count;
	Files	=	new cFILENAME [fc];

	for (i=0;i<fc;i++)	strcpy(Files[i].Name,GetStr(i));

	qsort((void *)Files,fc, sizeof(cFILENAME) ,cSTRINGS::Compare);

	Reset();

	for (i=0;i<fc;i++)	Add(Files[i].Name);
	
	pKILL(Files);

	return TRUE;
}

BOOL
cSTRINGS::Remove(int index)
{	if (index >= Count) return FALSE;

	cSTRING	*talk	=	Pick(index);

	if (!talk) return FALSE;

	talk->pPRE->pNEXT	=talk->pNEXT;
	talk->pNEXT->pPRE	=talk->pPRE;

	Count--;

	return TRUE;
}

BOOL
cSTRINGS::Remove(char *str)
{	int index = GetIndex(str);

	if (index!=-1) return Remove(index);

	return FALSE;
}

cSTRING*
cSTRINGS::Pick(int index)
{	if (index >= Count || index < 0) return FALSE;

	int i	=	0;

	cSTRING	*talk	=	pHEAD->pNEXT;

	while(talk)
	{	if (i	==	index) return talk;
		talk	=	talk->pNEXT;
		i++;
	}

	return NULL;
}

char*
cSTRINGS::GetStr(int index)
{	cSTRING	*talk = Pick(index);

	if (!talk) return NULL;

	return talk->String;
}

int
cSTRINGS::GetIndex(char *str)
{	int i	=	0;

	if (!pHEAD) return -1;

	cSTRING	*talk	=	pHEAD->pNEXT;

	while(talk->String)
	{	if (strcmp(talk->String,str)==0)	return i;

		talk	=	talk->pNEXT;
		i++;
	}

	return -1;
}

/****************************************************************************************
	화일 제어 클레스..
****************************************************************************************/

BOOL isFolder(WIN32_FIND_DATA *fd)
{	if(fd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{	if (strcmp(fd->cFileName,".")==0) return FALSE;
		if (strcmp(fd->cFileName,"..")==0) return FALSE;
		return TRUE;
	}

	return FALSE;
}

BOOL IsFolder(char *fn)
{	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;

	hFind=FindFirstFile(fn,&fd);

	while(hFind == INVALID_HANDLE_VALUE)	return FALSE;

	FindClose(hFind);

	return isFolder(&fd);
}

BOOL isFile(WIN32_FIND_DATA *fd)
{	if(fd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) return FALSE;

	return TRUE;
}

BOOL IsFile(char *fn)
{	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;

	hFind	=	FindFirstFile(fn,&fd);

	while(hFind == INVALID_HANDLE_VALUE)	return FALSE;

	FindClose(hFind);

	return isFile(&fd);
}

cFILE::cFILE()
{	fp			=	NULL;
	FrontPos	=	0;
	EndPos		=	0;
	Length		=	0;
	isMerge		=	FALSE;
}

cFILE::~cFILE()
{	Close();
}

BOOL
cFILE::Open(char *fn,char *mode)
{	fp		=	fopen(fn,mode);
	if (!fp) return FALSE;

	isMerge		=	FALSE;
	FrontPos	=	0;
	EndPos		=	cFILE::Size(fp);
	Length		=	EndPos;

	return TRUE;
}

BOOL
cFILE::Close()
{	if (!fp) return FALSE;

	fclose(fp);

	fp	=	NULL;

	return TRUE;
}

BOOL
cFILE::Seek(int pos,int seekTerm)
{	if (!fp) return FALSE;

	if (isMerge)
	{	switch(seekTerm)
		{	case	SEEK_SET	:
				fseek(fp,pos+FrontPos,seekTerm);
				break;

			case	SEEK_CUR	:
				fseek(fp,pos,seekTerm);
				break;

			case	SEEK_END	:
				fseek(fp,pos+EndPos,SEEK_SET);
				break;
		}
	}
	else fseek(fp,pos,seekTerm);

	return TRUE;
}

BOOL
cFILE::Read(void *buffer,DWORD size)
{	if (!fp) return FALSE;

	fread(buffer,size,1,fp);

	return TRUE;
}

BOOL
cFILE::Write(void *buffer,DWORD size)
{	if (!fp) return FALSE;

	fwrite(buffer,size,1,fp);

	return TRUE;
}

BOOL
cFILE::Rewind()
{	return Seek(FrontPos,SEEK_SET);
}

BOOL
cFILE::End()
{	return Seek(EndPos,SEEK_SET);
}

BOOL
cFILE::IsOpen()
{	if (fp) return TRUE;

	return FALSE;
}

DWORD
cFILE::GetPos()
{	return ftell(fp);
}

BOOL
cFILE::Set(cFILE *file,DWORD size)
{	fp			=	file->fp;
	Length		=	size;
	FrontPos	=	ftell(file->fp);
	EndPos		=	FrontPos+size;

	return TRUE;
}

BOOL
cFILE::Copy(char *fn)
{	if (!fp) return FALSE;

	cFILE	file;

	if (!file.Open(fn,"wb")) return FALSE;

	char	*temp	=	new char [d1MEGA];

	for (DWORD i=0;i<Length/d1MEGA;i++)
	{	Read(temp,d1MEGA);
		file.Write(temp,d1MEGA);
	}

	Read		(temp,Length%d1MEGA);
	file.Write	(temp,Length%d1MEGA);

	file.Close();

	return TRUE;
}

BOOL
cFILE::Burn(char *fn,char *buff,DWORD size)
{	cFILE	file;

	if (!file.Open(fn,"wb")) return FALSE;

	file.Write(buff,size);

	return TRUE;
}

//	정적 멤버
DWORD
cFILE::Size(char *fn)
{	FILE *tfp;

	tfp	=	fopen(fn,"rb");

	if (!tfp) return 0;

	DWORD size = filelength(fileno(tfp));

	fclose(tfp);

	return size;
}

DWORD
cFILE::Size(FILE *tfp)
{	return filelength(fileno(tfp));
}

BOOL
cFILE::SaveBox(char *fname,char *filter,char *defaultExt,char *folder,char *title,DWORD _dwStyle)
{	OPENFILENAME	ofn;

	char	dir[512];

	if (!folder)	GetCurrentDirectory(511,dir);
	else			strcpy(dir,folder);


	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrTitle		=	title;
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING | _dwStyle;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrInitialDir	=	dir;

	if (!GetSaveFileName(&ofn)) return FALSE;

	return TRUE;
}

BOOL
cFILE::LoadBox(char *fname,char *filter,char *defaultExt,char *folder,char *title,DWORD _dwStyle)
{	OPENFILENAME	ofn;

	char	dir[512];

	if (!folder)	GetCurrentDirectory(511,dir);
	else
	{
		strcpy(dir,folder);
		SetCurrentDirectory(dir);
	}

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrTitle		=	title;
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING | _dwStyle;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn)) return FALSE;

	return TRUE;
}

BOOL
cFILE::LoadBoxHook(char *fname,char *filter,char *defaultExt,LPOFNHOOKPROC _procHook,char *folder,char *title,DWORD _dwStyle)
{
	OPENFILENAME	ofn;

	char	dir[512];

	if (!folder)	GetCurrentDirectory(511,dir);
	else			strcpy(dir,folder);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_FILEMUSTEXIST|OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK |
							OFN_EXPLORER | OFN_ENABLEINCLUDENOTIFY | OFN_ENABLESIZING | _dwStyle;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrTitle		=	title;
	ofn.lpfnHook		=	_procHook;
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn))	return FALSE;
	
	return	TRUE;
}

BOOL
cFILE::LoadBoxTemplate(char *fname,char *filter,char *defaultExt,char *_strTemplateName,
					   LPOFNHOOKPROC _procHook,HINSTANCE _hInst,char *folder,char *title,DWORD _dwStyle)
{
	OPENFILENAME	ofn;

	char	dir[512];

	if (!folder)	GetCurrentDirectory(511,dir);
	else			strcpy(dir,folder);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_FILEMUSTEXIST|OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | 
							OFN_EXPLORER | OFN_ENABLEINCLUDENOTIFY | OFN_ENABLESIZING | _dwStyle;
	ofn.lpTemplateName	=	_strTemplateName;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.hInstance		=	_hInst;
	ofn.lpstrTitle		=	title;
	ofn.lpfnHook		=	_procHook;
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn))	return FALSE;

	return	TRUE;
}


/****************************************************************************************
	디렉토리 제어 클레스..
****************************************************************************************/
static	char	tempFolder[512];
WORD	cFOLDER::SortTerm;
char	*destFolder		=	NULL;

int CALLBACK	BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
{	char szDir[255];

	switch(uMsg) 
	{	case BFFM_INITIALIZED	:
		{	if (GetCurrentDirectory(255,szDir)) 
				SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
			if (tempStr[0])
				SetWindowText(GetDlgItem(hwnd,0x3744),tempStr);		//	에디트 박스

			int WIDTH	=	GetSystemMetrics(SM_CXSCREEN);
			int HEIGHT	=	GetSystemMetrics(SM_CYSCREEN);

			cRECT_SIZE rect;

			rect.OutFrame(hwnd);

			MoveWindow(hwnd,(WIDTH-rect.xs)/2,(HEIGHT-rect.ys)/2,rect.xs,rect.ys,TRUE);
			break;
        }

        case BFFM_SELCHANGED	: 
		{	if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir)) 
			{	if (destFolder)
				{	int size = strlen(szDir);

					if (szDir[size-1] == '\\')	szDir[size-1]	=	NULL;

					SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)_ms("%s\\%s",szDir,destFolder));
					SetWindowText(GetDlgItem(hwnd,0x3744),_ms("%s\\%s",szDir,destFolder));		//	에디트 박스
				}
				else
				{	SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
					SetWindowText(GetDlgItem(hwnd,0x3744),szDir);		//	에디트 박스
				}
			}
			break;
        }

		case BFFM_VALIDATEFAILEDA	:
			cFOLDER::Create((char *)lp,TRUE);
			strcpy(tempFolder,(char *)lp);
			break;

        default:	break;
    }
	return 0;
}

void
cFOLDER::Refit(char *folder)
{	int		size	=	(int)strlen(folder);
	int		count	=	0,index	=	0;

	char	dir[512];

	for (int i=0;i<size+1;i++)
	{	if (folder[i]=='\\')	count++;
		else
		{	if (count==1)	dir[index++]	=	'\\';
			count=0;
		}
		if (count <3)	dir[index++]	=	folder[i];
	}

	dir[index++]	=	NULL;

	strcpy(folder,dir);
}

int
cFOLDER::Compare( const void *a, const void *b)
{	cFILENAME *first	=	(cFILENAME *)a;
	cFILENAME *second	=	(cFILENAME *)b;

	if (SortTerm		==	dSORT_BY_NAME	)	return stricmp(first->Name,second->Name);
	if (first->isFolder != second->isFolder	)	return second->isFolder - first->isFolder;

	char *fe	=	_exportExt(first->Name);
	char *se	=	_exportExt(second->Name);

	if (!fe || !se)
	{	if (!fe && se) return -1;
		if (fe && !se) return 1;

		return stricmp(first->Name,second->Name);
	}

	int result = strcmp(fe,se);

	if (result==0)	return stricmp(first->Name,second->Name);

	return result;
}

char*
cFOLDER::Tree(HWND hwnd,BOOL change,char *title,char *folder,char *dest)
{	char	curDir[512];

	destFolder	=	dest;

	tempStr[0]	=	0;

	if (folder)
	{	Change(folder);
		strcpy(tempStr,folder);
	}

	GetCurrentDirectory(sizeof(curDir),curDir);			//	현재 폴더 혹인

	BROWSEINFO		binfo;
	LPITEMIDLIST	pidl;

	binfo.hwndOwner			=	hwnd;
	binfo.pidlRoot			=	0;
	binfo.pszDisplayName	=	0;
	binfo.lpszTitle			=	title;
	binfo.ulFlags			=	BIF_EDITBOX | BIF_VALIDATE;
	binfo.lpfn				=	BrowseCallbackProc; 

	tempFolder[0]			=	NULL;

	pidl					=	SHBrowseForFolder(&binfo);

	if (pidl)
	{	SHGetPathFromIDList(pidl,tempFolder);
		SetCurrentDirectory(tempFolder);

		if (!change) SetCurrentDirectory(curDir);

		return tempFolder;
	}

	SetCurrentDirectory(curDir);

	if (tempFolder[0] && change)
	{	SetCurrentDirectory(tempFolder);
		return tempFolder;
	}

	return NULL;
}

BOOL
cFOLDER::Delete(char *delDir)			//	화일을 포함한 폴더를 지운다.
{	char dirName[ 256];									// 재귀 호출을 사용한다.

	GetCurrentDirectory( 256, dirName);

	if( SetCurrentDirectory( delDir) == 0) 
	{	SetCurrentDirectory( dirName);
		return FALSE;
	}

	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;

	hFind	=	FindFirstFile("*.*",&fd);

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{	if (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{	if (strcmp(fd.cFileName,".")!=0 && strcmp(fd.cFileName,"..")!=0)
				Delete(fd.cFileName);
		}
		else
		{	SetFileAttributes(fd.cFileName,FILE_ATTRIBUTE_ARCHIVE);
			DeleteFile(fd.cFileName);
		}

		bRet=	FindNextFile(hFind,&fd);
	}

	FindClose(hFind);

	SetCurrentDirectory( dirName);

	if( RemoveDirectory( delDir) == 0) return FALSE;

	return TRUE;
}

BOOL
cFOLDER::getFolders(char *folder,cSTRINGS *folders)			//	화일을 포함한 폴더를 지운다.
{	char dirName[ 256];									// 재귀 호출을 사용한다.

	GetCurrentDirectory( 256, dirName);

	if( SetCurrentDirectory( folder) == 0) 
	{	SetCurrentDirectory( dirName);
		return FALSE;
	}

	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;

	hFind	=	FindFirstFile("*.*",&fd);

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{	if (isFolder(&fd))	getFolders(fd.cFileName,folders);

		bRet=	FindNextFile(hFind,&fd);
	}

	FindClose(hFind);

	GetCurrentDirectory( 512, tempFolder);
	folders->Add(tempFolder);
	SetCurrentDirectory(dirName);

	return TRUE;
}

cSTRINGS*
cFOLDER::GetFolders(char *folder)			//	화일을 포함한 폴더를 지운다.
{	cSTRINGS	*folders;

	folders	=	new cSTRINGS();

	getFolders(folder,folders);

	if (folders->Count <=0) cKILL(folders);

	return folders;
}

BOOL	//	폴더를 만든다. 
cFOLDER::Create(char *folder,BOOL changeFolder)
{	int	pos	=	0,count	=	0;

	char curDir[512];

	GetCurrentDirectory(512,curDir);

	while(1)
	{	tempStr[pos]	=	folder[count];
		if (tempStr[pos]==	NULL	||	tempStr[pos]==	'\\')
		{	tempStr[pos+1]=	NULL;
			if (SetCurrentDirectory(tempStr)==0)
			{	CreateDirectory(tempStr,NULL);
				if (SetCurrentDirectory(tempStr)==0)
				{	SetCurrentDirectory(curDir);
					return FALSE;
				}
			}
			if (folder[count]==	NULL)	break;
			pos	=	0;
		} else pos++;

		count++;
	}

	if (changeFolder)	SetCurrentDirectory(folder);
	else				SetCurrentDirectory(curDir);

	return TRUE;
}

BOOL	//	폴더를 이동한다.
cFOLDER::Change(char *folder)
{	int	pos	=	0,count	=	0;

	while(1)
	{	tempStr[pos]	=	folder[count];
		if (tempStr[pos]==	NULL	||	tempStr[pos]==	'\\')
		{	tempStr[pos+1]=	NULL;
			if (SetCurrentDirectory(tempStr)==0) return FALSE;
			if (folder[count]==	NULL)	return TRUE;
			pos	=	0;
		} else pos++;

		count++;
	}

	return FALSE;
}

cSTRINGS *
cFOLDER::GetFiles(char *ext,char *folder,BOOL includeFolder,WORD sort)
{	cFILENAME			*Files;
	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;
	cSTRINGS			*files	=	NULL;

	SetSort(sort);

	GetCurrentDirectory(512,tempFolder);

	if (folder)
		if (!SetCurrentDirectory(folder)) return NULL;

	if (SortTerm == dSORT_NOT)	//	소트할 필요가 없음 걍 돌린다.
	{	files	=	new cSTRINGS();

		hFind	=	FindFirstFile(ext,&fd);

		while(hFind != INVALID_HANDLE_VALUE && bRet)
		{	if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{	if (includeFolder )
					if (isFolder(&fd))
						files->Add(fd.cFileName);
			}
			else		files->Add(fd.cFileName);

			bRet=	FindNextFile(hFind,&fd);
		}
		FindClose(hFind);

		if (files->Count <= 0) cKILL(files);
		SetCurrentDirectory(tempFolder);

		return files;
	}

	fc		=	GetFileCount(ext,NULL,includeFolder);

	if (fc==0)
	{	SetCurrentDirectory(tempFolder);
		return NULL;
	}

	Files	=	new cFILENAME [fc];

	fc		=	0;

	hFind	=	FindFirstFile(ext,&fd);

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{	if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{	if (includeFolder )
				if (isFolder(&fd))
					Files[fc++].Set(fd.cFileName,TRUE);
		}
		else		Files[fc++].Set(fd.cFileName,FALSE);
		

		bRet=	FindNextFile(hFind,&fd);
	}

	FindClose(hFind);

	qsort((void *)Files,fc, sizeof(cFILENAME) ,Compare);

	files	=	new cSTRINGS();

	for (int i=0;i<fc;i++)	files->Add(Files[i].Name);

	SetCurrentDirectory(tempFolder);

	pKILL(Files);

	return files;
}

cSTRINGS *
cFOLDER::GetFiles(WORD sort)
{	return GetFiles("*.*",NULL,FALSE,sort);
}

int	fileCount(char *ext,BOOL includeFolder)
{	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;

	hFind=FindFirstFile(ext,&fd);
	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{	if(includeFolder)
		{	if (isFolder(&fd))	fc++;
			else				if ((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)	fc++;
		}
		else					if ((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)	fc++;

		bRet=FindNextFile(hFind,&fd);
	}
	FindClose(hFind);

	return fc;
}

DWORD
cFOLDER::GetFileCount(char *ext,char *folder,BOOL includeFolder,BOOL includeSub)
{	cSTRINGS	*strs	=	NULL;
	int			count	=	0;
	char		curDir[512];

	GetCurrentDirectory(512,curDir);

	if (!includeSub)
	{	if (folder)	if (!SetCurrentDirectory(folder)) return 0;

		count	=	fileCount(ext,includeFolder);

		SetCurrentDirectory(curDir);

		return count;
	}

	strs		=	GetFolders(folder);

	if (strs)
	{	cSTRING	*str	=	strs->Pick();

		while(str->pNEXT)
		{	SetCurrentDirectory(curDir);
			SetCurrentDirectory(str->String);

			count+=fileCount("*.*",FALSE);

			str	=	str->pNEXT;
		}

		cKILL(strs);
	}
	else count	=	fileCount("*.*",FALSE);

	SetCurrentDirectory(curDir);

	return count;
}

DWORD
cFOLDER::GetFolderCount(char *folder)
{	GetCurrentDirectory(512,tempFolder);

	if (folder)	if (!SetCurrentDirectory(folder)) return 0;

	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	DWORD				fc		=	0;

	hFind=FindFirstFile("*.*",&fd);
	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{	if(isFolder(&fd))	fc++;
		bRet=FindNextFile(hFind,&fd);
	}

	FindClose(hFind);

	return fc;
}

DWORD	getFolderSize(char *ext)
{	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	DWORD				size	=	0;

	hFind=FindFirstFile(ext,&fd);

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{	if ((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)	size+=cFILE::Size(fd.cFileName);
		bRet	=	FindNextFile(hFind,&fd);
	}

	FindClose(hFind);

	return size;
}

DWORD
cFOLDER::GetFolderSize(char *ext,char *folder,BOOL includeSub)
{	DWORD size	=	0;

	char	curDir[512];

	GetCurrentDirectory(512,curDir);

	if (!includeSub)
	{	if (folder)	if (!SetCurrentDirectory(folder)) return 0;
		size	=	getFolderSize(ext);
		SetCurrentDirectory(curDir);

		return size;
	}

	cSTRINGS *strs	=	GetFolders(folder);

	if (strs)
	{	cSTRING	*str	=	strs->Pick();

		while(str->String)
		{	SetCurrentDirectory(curDir);
			SetCurrentDirectory(str->String);

			size+=	getFolderSize(ext);
			str	=	str->pNEXT;
		}

		cKILL(strs);
	}
	else	size	=	getFolderSize(ext);

	SetCurrentDirectory(curDir);

	return size;
}

/**********************************************************************************
							병합된 화일을 관리한다.
**********************************************************************************/

cNUX::cNUX()
{	INDEX		=	NULL;
	AppendPos	=	0;
	Count		=	0;
}

cNUX::~cNUX()
{	Close();
}

int	
cNUX::Compare_Name( const void *a, const void *b)
{	return(stricmp(((cMERGEINFO *)a)->FName,((cMERGEINFO *)b)->FName));
}

BOOL
cNUX::Open(char *fn)
{	if (!File.Open(fn,"rb"))
	{	MessageBox(GetActiveWindow(),_ms("'%s' 화일을 찾울 수 없습니다.",fn),"Error!! in cNUX::Open",MB_ICONERROR|MB_TOPMOST);
		return FALSE;
	}

	DWORD	temp;

	File.Seek(-4,SEEK_END);
	File.Read(&temp,4);								//	합쳐진 화일인가?

	if (temp	!=	0xefefefef)						//	아니다.
	{	File.Close();
		MessageBox(GetActiveWindow(),_ms("'%s'화일의 형식을 알 수 없습니다.",fn),"Error!! in cNUX::Open",MB_ICONERROR|MB_TOPMOST);
		return FALSE;
	}

	File.Seek(-8,SEEK_END);							//	화일의 헤더 위치
	File.Read(&AppendPos,4);						//	를 읽는다.
	File.Seek(AppendPos,SEEK_SET);					//	화일의 헤더 위치
	File.Read(&Count,4);							//	화일 수

	pKILL(INDEX);
	INDEX	=	new	cMERGEINFO [Count];

	File.Read(&temp,4);								//	합쳐진 화일의 사이즈(여기선 별루 쓸데가..
	File.Read(&temp,4);								//	화일 정보의 위치
	File.Seek(temp,SEEK_SET);						//	화일의 정보가 있는 곳으로
	File.Read(INDEX,Count*sizeof(cMERGEINFO));		//	화일 정보를 읽어온다.

	return TRUE;
}

BOOL
cNUX::Close()
{	File.Close();
	pKILL(INDEX);

	return TRUE;
}

inline cMERGEINFO *
cNUX::Seek(char *fn)
{	return (cMERGEINFO *)bsearch( fn,INDEX,Count,sizeof(cMERGEINFO),Compare_Name);
}

BOOL
cNUX::Open(char *fn,cFILE *file)
{	cMERGEINFO *result = Seek(fn);

	if (!result) return FALSE;

	File.Seek(result->Locate,SEEK_SET);

	file->Set(&File,result->Size);

	return TRUE;
}

BOOL
cNUX::Open(DWORD index,cFILE *file)
{	if (index >= Count) return FALSE;

	File.Seek(INDEX[index].Locate,SEEK_SET);
	file->Set(&File,INDEX[index].Size);

	return TRUE;
}

BOOL
cNUX::Export(char *fn)
{	cMERGEINFO *result = Seek(fn);

	if (!result) return FALSE;

	File.Seek(result->Locate,SEEK_SET);

	char	*temp	=	new char [result->Size];

	File.Read(temp,result->Size);

	BOOL r = cFILE::Burn(result->FName,temp,result->Size);

	pKILL(temp);

	return r;
}
