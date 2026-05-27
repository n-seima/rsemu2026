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
#include <ddraw.h>
#include <dsound.h>
#include <d3drm.h>

#include "SFC.H"

#ifdef dDEBUG_MODE
#define	dLOGMODE
#else
#define	dLOGMODE
#endif

static char		tempStr[1024];			//	cSTRING	클래스를 위한 템플릿 스트링..
static char		_logFolder[1024];		//	프로그램 실행 폴더

static	char	ReportFile[1024];
static	int		ErrorCount		=	0;

int		_PixelFormat	=	dRGB555;

WORD	(*RGBmix  )(int r,int g,int b ) = RGB555mix;

WORD RGB565mix(int r,int g,int b)
{	return 	(WORD) (  (((WORD)(r<<11))) + (((WORD)(g<<5))) + ((WORD)b)  );
}

WORD RGB555mix(int r,int g,int b)
{	g>>=1;
	return 	(WORD) (  (((WORD)(r<<10))) + (((WORD)(g<<5))) + ((WORD)b)  );
}

WORD BGR565mix(int r,int g,int b)
{	return 	(WORD) (  (((WORD)(b<<11))) + (((WORD)(g<<5))) + ((WORD)r)  );
}

class cSFC
{
public:
		cSFC();
		~cSFC();
};

cSFC tempsfc;

cSFC::cSFC()
{	srand( (unsigned)time( NULL ) );

	GetCurrentDirectory(sizeof(_logFolder),_logFolder);
	if (stricmp(_exportFileName(_logFolder),"debug")==0)	strcpy(_logFolder,_exportFolderName(_logFolder));
	if (stricmp(_exportFileName(_logFolder),"release")==0)	strcpy(_logFolder,_exportFolderName(_logFolder));
	SetCurrentDirectory(_logFolder);

	strcpy(ReportFile,_ms("%s\\result.txt",_logFolder));
	DeleteFile(ReportFile);

	cMSG::Init();

	char time[128];
	_strtime( time );

	MSGOUT("\n************* Application Started at [%s] *************\n",time);
}

cSFC::~cSFC()
{
	char time[128];
	_strtime( time );

	MSGOUT("\n************* Application Closed  at [%s] *************\n\n",time);

#ifdef dLOGMODE
	SetCurrentDirectory(_logFolder);

//	ShellExecute(NULL,"open",ReportFile,NULL,NULL,SW_SHOW);
#endif
}

int Compare( const void *a, const void *b)
{	return (((tINDEX *)a)->Value-((tINDEX *)b)->Value);
}

void
memset(WORD *dest,WORD value,unsigned int size)
{	UINT *dest32=(UINT *)dest;
	unsigned int width	=	size;

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
cPOINT::MousePos(HWND hwnd)
{	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(hwnd,&pos);
	x=pos.x,y=pos.y;
}

void
cPOINT::MousePos()
{	POINT pos;
	GetCursorPos(&pos);
	x=pos.x,y=pos.y;
}


void
cPOINT::toScreen(HWND hwnd)
{	POINT pos;
	pos.x	=	x,pos.y	=	y;

	ClientToScreen(hwnd,&pos);
	x=pos.x,y=pos.y;
}

BOOL	
cPOINT::match(cPOINT pos)
{	if (x!=pos.x || y!=pos.y) return FALSE;

	return TRUE;
}

BOOL
cPOINT::match(POINT pos)
{	if (x!=pos.x || y!=pos.y) return FALSE;

	return TRUE;
}

BOOL
cPOINT::match(int a,int b)
{	if (x!=a || y!=b) return FALSE;

	return TRUE;
}

/*
	class cRECT		
		:	rect	스트럭쳐의 확장판이다. 
			박스겹침,영역확인,좌표교정등을 알아서 처리해준다.
*/


void
cRECT::toScreen(HWND hwnd)
{	RECT	rect;
	GetClientRect(hwnd,&rect);
	Set(rect);

	cPOINT	posLT,posRD;

	posLT.Set(rect.left,rect.top);
	posRD.Set(rect.right,rect.bottom);

	posLT.toScreen(hwnd);
	posRD.toScreen(hwnd);

	Set(posLT.x,posLT.y,posRD.x,posRD.y);
}

void
cRECT::Set(HWND hwnd)
{	RECT	rect;
	GetClientRect(hwnd,&rect);
	Set(rect);
}

void
cRECT::OutFrame(HWND hwnd)
{	RECT	rect;
	GetWindowRect(hwnd,&rect);
	Set(rect);
}

BOOL
cRECT::isIN(int x,int y)
{	if (x >= x1 && x <= x2 && y >= y1 && y <= y2) return TRUE;
	return FALSE;
}

BOOL
cRECT::crash(cRECT compare)
{	if (compare.x1 > x2) return FALSE;
	if (compare.x2 < x1) return FALSE;
	if (compare.y1 > y2) return FALSE;
	if (compare.y2 < y1) return FALSE;

	return TRUE;
}

cRECT
cRECT::operator=(const cRECT &src)
{	Set(src.x1,src.y1,src.x2,src.y2);

	return src;
}
//	cRECT 클래스의 short형 버젼


/*******************************************************************************************
	class cSTRING
		:	문자열을 다룬다.
*******************************************************************************************/

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

char	*_exportName(char *str)
{	int dest=0;
	strcpy(tempStr,str);

	for (UINT i=0;i<strlen(str)	;i++)	if (str[i]=='\\') dest		=i+1;
	for (i=dest;i<strlen(str)	;i++)	if (str[i]=='.'	) tempStr[i]=NULL;



	return &tempStr[dest];
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

char* _cdecl _ms(char *str,...)
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

cSTRING::cSTRING(char *str)
{	pPRE	=	NULL;
	pNEXT	=	NULL;
	String	=	NULL;
	Length	=	0;

	Set(str);
}

cSTRING::~cSTRING()	
{	pKILL(String);
	Length	=	0;	
}

char* _cdecl
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

char* _cdecl
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

char* _cdecl
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

int _cdecl	
cSTRINGS::Compare( const void *a, const void *b)
{	return stricmp(((cFILENAME *)a)->Name,((cFILENAME *)b)->Name);
//	return(stricmp(((cSTRING *)a)->String,((cSTRING *)b)->String));
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
		KILL(sen->pPRE);
	}

	KILL(sen);

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

	cSTRING	*str	=	Pick(0);

	for (i=0;i<fc;i++)
	{	strcpy(Files[i].Name,str->String);
		str	=	str->pNEXT;
	}

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
	frontPos	=	0;
	endPos		=	0;
	Length		=	0;
	isMerge		=	FALSE;
}

cFILE::~cFILE()
{	Close();
}

BOOL
cFILE::Open(char *fn,char *mode)
{	Close();

	fp			=	fopen(fn,mode);
	if (!fp) return FALSE;

	isMerge		=	FALSE;
	frontPos	=	0;
	endPos		=	cFILE::Size(fp);
	Length		=	endPos;

	return TRUE;
}

BOOL
cFILE::Close()
{	if (!fp		)	return FALSE;
	if (isMerge	)	return TRUE;

	fclose(fp);
	fp			=	NULL;
	frontPos	=	0;
	endPos		=	0;
	Length		=	0;
	isMerge		=	FALSE;

	return TRUE;
}

BOOL
cFILE::Seek(int pos,int seekTerm)
{	if (!fp) return FALSE;

	if (isMerge)
	{	switch(seekTerm)
		{	case	SEEK_SET	:
				fseek(fp,pos+frontPos,seekTerm);
				break;

			case	SEEK_CUR	:
				fseek(fp,pos,seekTerm);
				break;

			case	SEEK_END	:
				fseek(fp,pos+endPos,SEEK_SET);
				break;
		}
	}
	else fseek(fp,pos,seekTerm);

	return TRUE;
}

DWORD
cFILE::Locate()
{	return ftell(fp);
}

BOOL
cFILE::Read(void *buffer,DWORD size,DWORD itemSize)
{	if (!fp) return FALSE;

	fread(buffer,size*itemSize,1,fp);

	return TRUE;
}

BOOL
cFILE::Write(void *buffer,DWORD size,DWORD itemSize)
{	if (!fp) return FALSE;

	fwrite(buffer,size*itemSize,1,fp);

	return TRUE;
}

BOOL
cFILE::Rewind()
{	return Seek(frontPos,SEEK_SET);
}

BOOL
cFILE::End()
{	return Seek(endPos,SEEK_SET);
}

BOOL
cFILE::Set(cFILE *file,DWORD size)
{	fp			=	file->fp;
	Length		=	size;
	frontPos	=	ftell(file->fp);
	endPos		=	frontPos+size;

	return TRUE;
}

BOOL
cFILE::Copy(char *fn)
{	if (!fp) return FALSE;

	cFILE	file;

	if (!file.Open(fn,"wb")) return FALSE;

	char	*temp	=	NEW(d1MEGA,char);

	DWORD	pos		=	Locate();
	Seek(0,SEEK_SET);

	for (DWORD i=0;i<Length/d1MEGA;i++)
	{	Read(temp,d1MEGA);
		file.Write(temp,d1MEGA);
	}

	Read		(temp,Length%d1MEGA);
	file.Write	(temp,Length%d1MEGA);

	file.Close();

	Seek(pos,SEEK_SET);

	FREE(temp);

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
cFILE::SaveBox(char *fname,char *filter,char *defaultExt,char *folder,char *title)
{	OPENFILENAME	ofn;

	char	dir[512];

	if (!folder)	GetCurrentDirectory(511,dir);
	else			strcpy(dir,folder);


	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrTitle		=	"데이터 저장.";
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrInitialDir	=	dir;

	if (!GetSaveFileName(&ofn)) return FALSE;

	return TRUE;
}

BOOL
cFILE::LoadBox(char *fname,char *filter,char *defaultExt,char *folder,char *title)
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
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrInitialDir	=	dir;

	if (!GetOpenFileName(&ofn)) return FALSE;

	return TRUE;
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

			cRECT rect;

			rect.OutFrame(hwnd);

			MoveWindow(hwnd,(WIDTH-rect.width())/2,(HEIGHT-rect.height())/2,rect.width(),rect.height(),TRUE);
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

int _cdecl	
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
cFOLDER::getFolders(char *folder,cSTRINGS *folders)
{	char dirName[ 512];									// 재귀 호출을 사용한다.

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
cFOLDER::GetFolders(char *folder,BOOL includeSub)			//	화일을 포함한 폴더를 지운다.
{	cSTRINGS	*folders;

	folders	=	new cSTRINGS();

	if( includeSub)	getFolders(folder,folders);
	else
	{	HANDLE				hFind;
		BOOL				bRet	=	TRUE;
		WIN32_FIND_DATA		fd;

		hFind	=	FindFirstFile("*.*",&fd);

		while(hFind != INVALID_HANDLE_VALUE && bRet)
		{	if (isFolder(&fd))	folders->Add(fd.cFileName);

			bRet=	FindNextFile(hFind,&fd);
		}

		FindClose(hFind);
	}

	if (folders->Count <=0)
	{	KILL(folders);
	}
	else	folders->Sort();

	return	folders;
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

		if (files->Count <= 0) KILL(files);
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

		KILL(strs);
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

		KILL(strs);
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

int	_cdecl	
cNUX::Compare_Name( const void *a, const void *b)
{	return(stricmp(((cMERGEINFO *)a)->FName,((cMERGEINFO *)b)->FName));
}

BOOL
cNUX::Load(char *fn)
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

cFILE*
cNUX::Open(char *fn)
{	cMERGEINFO *result = Seek(fn);

	if (!result)
	{	MSGOUT("Error in cNUX::Open - '%s' file not found",fn);
		return NULL;
	}

	File.Seek(result->Locate,SEEK_SET);

	cFILE	*file	=	new cFILE();
	file->Set(&File,result->Size);
	file->isMerge	=	TRUE;

	return file;
}

cFILE*
cNUX::Open(DWORD index)
{	if (index >= Count)
	{	MSGOUT("Error in cNUX::Open - out of range %d [0~%d]",index,Count);
		return NULL;
	}

	File.Seek(INDEX[index].Locate,SEEK_SET);

	cFILE	*file	=	new cFILE();
	file->Set(&File,INDEX[index].Size);
	file->isMerge	=	TRUE;

	return file;
}

BOOL
cNUX::Export(char *fn)
{	cMERGEINFO *result = Seek(fn);

	if (!result) return FALSE;

	File.Seek(result->Locate,SEEK_SET);

	cFILE *file = Open(fn);

	file->Copy(result->FName);

	KILL(file);

	return TRUE;
}

/**********************************************************************************
	cMSG 클래스 메시지창을 띄운다.
**********************************************************************************/

char	cMSG::msgBuffer[1024];
HWND	cMSG::hDEBUGWND;

BOOL
Safety()
{	if (ErrorCount > 0)
	{	cMSG::Error(_ms("%d Errors occured!!",ErrorCount),"Please Check \"%s\" File",ReportFile);

		return FALSE;
	}

	return TRUE;
}

BOOL
Errored()
{	if (ErrorCount > 0)	return TRUE;

	return FALSE;
}

void
cMSG::Init()
{	hDEBUGWND	=	FindWindow("DEBUGWINDOW","Debug Window");
}

void
cMSG::Close()
{
}

void
cMSG::Test(int num)
{	static int count=0;

	if (num==-1) count++;
	else count=num;

	Put("Message from Message Hadler","Test!! It's Check Point No.%d!!",count);
}

int _cdecl
cMSG::YESNO(char *headmsg,char *mainmsg,...)
{	if (!mainmsg) mainmsg="(null)";
	va_list		argptr;

	va_start(argptr, mainmsg);
	vsprintf(msgBuffer,mainmsg,argptr);
	va_end(argptr);

	return	MessageBox(GetActiveWindow(),msgBuffer,headmsg,MB_ICONQUESTION|MB_YESNO|MB_TOPMOST);
}

int _cdecl
cMSG::YESNOCANCEL(char *headmsg,char *mainmsg,...)
{	if (!mainmsg) mainmsg="(null)";
	va_list		argptr;

	va_start(argptr, mainmsg);
	vsprintf(msgBuffer,mainmsg,argptr);
	va_end(argptr);

	return MessageBox(GetActiveWindow(),msgBuffer,headmsg,MB_ICONQUESTION|MB_YESNOCANCEL|MB_TOPMOST);
}

BOOL _cdecl
cMSG::Output(char *mainmsg,...)
{	if (hDEBUGWND)
	{	HANDLE		hMem;
		LPVOID		lpData;
		va_list		argptr;

		va_start(argptr, mainmsg);
		vsprintf(msgBuffer,mainmsg,argptr);
		va_end	(argptr);

		OpenClipboard(NULL);
		EmptyClipboard();

		hMem	=	GlobalAlloc(GHND,1024);
		lpData	=	GlobalLock(hMem);

		strcpy((char *)lpData,_ms("%s%c%c",msgBuffer,0x0d,0x0a));
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();

		SendMessage(hDEBUGWND,WM_CONSOLETEXT,0,0);
	}

#ifdef dLOGMODE

	if (!hDEBUGWND)
	{	va_list		argptr;

		va_start(argptr, mainmsg);
		vsprintf(msgBuffer,mainmsg,argptr);
		va_end	(argptr);
		DWORD size = strlen(msgBuffer);

		strcpy(msgBuffer+size,"\n");
	}

	FILE	*fp = fopen(ReportFile, "a+" );

	OutputDebugString(msgBuffer);

	if (fp)
	{	fprintf( fp, "%s",msgBuffer);
		fclose( fp );
	}
#endif

	return FALSE;
}

BOOL _cdecl
cMSG::Put(char *headmsg,char *mainmsg,...)
{	va_list		argptr;

	if (!mainmsg)	MessageBox(GetActiveWindow(),headmsg,"Message",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	else
	{	va_start(argptr, mainmsg);
		vsprintf(msgBuffer,mainmsg,argptr);
		va_end(argptr);

		MessageBox(GetActiveWindow(),msgBuffer,headmsg,MB_OK|MB_TOPMOST|MB_ICONINFORMATION);
	}

	return FALSE;
}

BOOL _cdecl
cMSG::Error(char *headmsg,char *mainmsg,...)
{	va_list		argptr;

	if (!mainmsg)	MessageBox(GetActiveWindow(),headmsg,"Error Message!!",MB_ICONERROR|MB_TOPMOST);
	else
	{	va_start(argptr, mainmsg);
		vsprintf(msgBuffer,mainmsg,argptr);
		va_end(argptr);

		MessageBox(GetActiveWindow(),msgBuffer,headmsg,MB_ICONERROR|MB_TOPMOST);
	}

	ErrorCount++;

	return FALSE;
}

BOOL
cMSG::Put(HWND hwnd,char *msg)
{	MessageBox(hwnd,msg,"Message Handler Message!!",MB_OK|MB_TOPMOST);

	return FALSE;
}

char* 
cMSG::DXERROR(HRESULT hErr)
{	static	char	returnMsg[256];

	switch (hErr)
    {	//DirectDraw
		case DDERR_ALREADYINITIALIZED:
			strcpy(returnMsg,"This object is already initialized.\0");break;
		case DDERR_BLTFASTCANTCLIP:
			strcpy(returnMsg,"Return if a clipper object is attached to the source surface passed into a BltFast call.\0");break;
		case DDERR_CANNOTATTACHSURFACE:
			strcpy(returnMsg,"This surface can not be attached to the requested surface.\0");break;
		case DDERR_CANNOTDETACHSURFACE:
			strcpy(returnMsg,"This surface can not be detached from the requested surface.\0");break;
		case DDERR_CANTCREATEDC:
			strcpy(returnMsg,"Windows can not create any more DCs.\0");break;
		case DDERR_CANTDUPLICATE:
			strcpy(returnMsg,"Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.\0");break;
		case DDERR_CLIPPERISUSINGHWND:
			strcpy(returnMsg,"An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.\0");break;
		case DDERR_COLORKEYNOTSET:
			strcpy(returnMsg,"No src color key specified for this operation.\0");break;
		case DDERR_CURRENTLYNOTAVAIL:
			strcpy(returnMsg,"Support is currently not available.\0");break;
		case DDERR_DIRECTDRAWALREADYCREATED:
			strcpy(returnMsg,"A DirectDraw object representing this driver has already been created for this process.\0");break;
		case DDERR_EXCEPTION:
			strcpy(returnMsg,"An exception was encountered while performing the requested operation.\0");break;
		case DDERR_EXCLUSIVEMODEALREADYSET:
			strcpy(returnMsg,"An attempt was made to set the cooperative level when it was already set to exclusive.\0");break;
		case DDERR_GENERIC:
			strcpy(returnMsg,"Generic failure.\0");break;
		case DDERR_HEIGHTALIGN:
			strcpy(returnMsg,"Height of rectangle provided is not a multiple of reqd alignment.\0");break;
		case DDERR_HWNDALREADYSET:
			strcpy(returnMsg,"The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.\0");break;
		case DDERR_HWNDSUBCLASSED:
			strcpy(returnMsg,"HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.\0");break;
		case DDERR_IMPLICITLYCREATED:
			strcpy(returnMsg,"This surface can not be restored because it is an implicitly created surface.\0");break;
		case DDERR_INCOMPATIBLEPRIMARY:
			strcpy(returnMsg,"Unable to match primary surface creation request with existing primary surface.\0");break;
		case DDERR_INVALIDCAPS:
			strcpy(returnMsg,"One or more of the caps bits passed to the callback are incorrect.\0");break;
		case DDERR_INVALIDCLIPLIST:
			strcpy(returnMsg,"DirectDraw does not support the provided cliplist.\0");break;
		case DDERR_INVALIDDIRECTDRAWGUID:
			strcpy(returnMsg,"The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.\0");break;
		case DDERR_INVALIDMODE:
			strcpy(returnMsg,"DirectDraw does not support the requested mode.\0");break;
		case DDERR_INVALIDOBJECT:
			strcpy(returnMsg,"DirectDraw received a pointer that was an invalid DIRECTDRAW object.\0");break;
		case DDERR_INVALIDPARAMS:
			strcpy(returnMsg,"One or more of the parameters passed to the function are incorrect.\0");break;
		case DDERR_INVALIDPIXELFORMAT:
			strcpy(returnMsg,"The pixel format was invalid as specified.\0");break;
		case DDERR_INVALIDPOSITION:
			strcpy(returnMsg,"Returned when the position of the overlay on the destination is no longer legal for that destination.\0");break;
		case DDERR_INVALIDRECT:
			strcpy(returnMsg,"Rectangle provided was invalid.\0");break;
		case DDERR_LOCKEDSURFACES:
			strcpy(returnMsg,"Operation could not be carried out because one or more surfaces are locked.\0");break;
		case DDERR_NO3D:
			strcpy(returnMsg,"There is no 3D present.\0");break;
		case DDERR_NOALPHAHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no alpha accleration hardware present or available.\0");break;
		case DDERR_NOBLTHW:
			strcpy(returnMsg,"No blitter hardware present.\0");break;
		case DDERR_NOCLIPLIST:
			strcpy(returnMsg,"No cliplist available.\0");break;
		case DDERR_NOCLIPPERATTACHED:
			strcpy(returnMsg,"No clipper object attached to surface object.\0");break;
		case DDERR_NOCOLORCONVHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no color conversion hardware present or available.\0");break;
		case DDERR_NOCOLORKEY:
			strcpy(returnMsg,"Surface doesn't currently have a color key\0");break;
		case DDERR_NOCOLORKEYHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no hardware support of the destination color key.\0");break;
		case DDERR_NOCOOPERATIVELEVELSET:
			strcpy(returnMsg,"Create function called without DirectDraw object method SetCooperativeLevel being called.\0");break;
		case DDERR_NODC:
			strcpy(returnMsg,"No DC was ever created for this surface.\0");break;
		case DDERR_NODDROPSHW:
			strcpy(returnMsg,"No DirectDraw ROP hardware.\0");break;
		case DDERR_NODIRECTDRAWHW:
			strcpy(returnMsg,"A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.\0");break;
		case DDERR_NOEMULATION:
			strcpy(returnMsg,"Software emulation not available.\0");break;
		case DDERR_NOEXCLUSIVEMODE:
			strcpy(returnMsg,"Operation requires the application to have exclusive mode but the application does not have exclusive mode.\0");break;
		case DDERR_NOFLIPHW:
			strcpy(returnMsg,"Flipping visible surfaces is not supported.\0");break;
		case DDERR_NOGDI:
			strcpy(returnMsg,"There is no GDI present.\0");break;
		case DDERR_NOHWND:
			strcpy(returnMsg,"Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.\0");break;
		case DDERR_NOMIRRORHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no hardware present or available.\0");break;
		case DDERR_NOOVERLAYDEST:
			strcpy(returnMsg,"Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.\0");break;
		case DDERR_NOOVERLAYHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no overlay hardware present or available.\0");break;
		case DDERR_NOPALETTEATTACHED:
			strcpy(returnMsg,"No palette object attached to this surface.\0");break;
		case DDERR_NOPALETTEHW:
			strcpy(returnMsg,"No hardware support for 16 or 256 color palettes.\0");break;
		case DDERR_NORASTEROPHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no appropriate raster op hardware present or available.\0");break;
		case DDERR_NOROTATIONHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no rotation hardware present or available.\0");break;
		case DDERR_NOSTRETCHHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no hardware support for stretching.\0");break;
		case DDERR_NOT4BITCOLOR:
			strcpy(returnMsg,"DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.\0");break;
		case DDERR_NOT4BITCOLORINDEX:
			strcpy(returnMsg,"DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.\0");break;
		case DDERR_NOT8BITCOLOR:
			strcpy(returnMsg,"DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.\0");break;
		case DDERR_NOTAOVERLAYSURFACE:
			strcpy(returnMsg,"Returned when an overlay member is called for a non-overlay surface.\0");break;
		case DDERR_NOTEXTUREHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no texture mapping hardware present or available.\0");break;
		case DDERR_NOTFLIPPABLE:
			strcpy(returnMsg,"An attempt has been made to flip a surface that is not flippable.\0");break;
		case DDERR_NOTFOUND:
			strcpy(returnMsg,"Requested item was not found.\0");break;
		case DDERR_NOTLOCKED:
			strcpy(returnMsg,"Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.\0");break;
		case DDERR_NOTPALETTIZED:
			strcpy(returnMsg,"The surface being used is not a palette-based surface.\0");break;
		case DDERR_NOVSYNCHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.\0");break;
		case DDERR_NOZBUFFERHW:
			strcpy(returnMsg,"Operation could not be carried out because there is no hardware support for zbuffer blitting.\0");break;
		case DDERR_NOZOVERLAYHW:
			strcpy(returnMsg,"Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.\0");break;
		case DDERR_OUTOFCAPS:
			strcpy(returnMsg,"The hardware needed for the requested operation has already been allocated.\0");break;
		case DDERR_OUTOFMEMORY:
			strcpy(returnMsg,"DirectDraw does not have enough memory to perform the operation.\0");break;
		case DDERR_OUTOFVIDEOMEMORY:
			strcpy(returnMsg,"DirectDraw does not have enough memory to perform the operation.\0");break;
		case DDERR_OVERLAYCANTCLIP:
			strcpy(returnMsg,"The hardware does not support clipped overlays.\0");break;
		case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
			strcpy(returnMsg,"Can only have ony color key active at one time for overlays.\0");break;
		case DDERR_OVERLAYNOTVISIBLE:
			strcpy(returnMsg,"Returned when GetOverlayPosition is called on a hidden overlay.\0");break;
		case DDERR_PALETTEBUSY:
			strcpy(returnMsg,"Access to this palette is being refused because the palette is already locked by another thread.\0");break;
		case DDERR_PRIMARYSURFACEALREADYEXISTS:
			strcpy(returnMsg,"This process already has created a primary surface.\0");break;
		case DDERR_REGIONTOOSMALL:
			strcpy(returnMsg,"Region passed to Clipper::GetClipList is too small.\0");break;
		case DDERR_SURFACEALREADYATTACHED:
			strcpy(returnMsg,"This surface is already attached to the surface it is being attached to.\0");break;
		case DDERR_SURFACEALREADYDEPENDENT:
			strcpy(returnMsg,"This surface is already a dependency of the surface it is being made a dependency of.\0");break;
		case DDERR_SURFACEBUSY:
			strcpy(returnMsg,"Access to this surface is being refused because the surface is already locked by another thread.\0");break;
		case DDERR_SURFACEISOBSCURED:
			strcpy(returnMsg,"Access to surface refused because the surface is obscured.\0");break;
		case DDERR_SURFACELOST:
			strcpy(returnMsg,"Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.\0");break;
		case DDERR_SURFACENOTATTACHED:
			strcpy(returnMsg,"The requested surface is not attached.\0");break;
		case DDERR_TOOBIGHEIGHT:
			strcpy(returnMsg,"Height requested by DirectDraw is too large.\0");break;
		case DDERR_TOOBIGSIZE:
			strcpy(returnMsg,"Size requested by DirectDraw is too large, but the individual height and width are OK.\0");break;
		case DDERR_TOOBIGWIDTH:
			strcpy(returnMsg,"Width requested by DirectDraw is too large.\0");break;
		case DDERR_UNSUPPORTED:
			strcpy(returnMsg,"Action not supported.\0");break;
		case DDERR_UNSUPPORTEDFORMAT:
			strcpy(returnMsg,"FOURCC format requested is unsupported by DirectDraw.\0");break;
		case DDERR_UNSUPPORTEDMASK:
			strcpy(returnMsg,"Bitmask in the pixel format requested is unsupported by DirectDraw.\0");break;
		case DDERR_VERTICALBLANKINPROGRESS:
			strcpy(returnMsg,"Vertical blank is in progress.\0");break;
		case DDERR_WASSTILLDRAWING:
			strcpy(returnMsg,"Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.\0");break;
		case DDERR_WRONGMODE:
			strcpy(returnMsg,"This surface can not be restored because it was created in a different mode.\0");break;
		case DDERR_XALIGN:
			strcpy(returnMsg,"Rectangle provided was not horizontally aligned on required boundary.\0");break;
		case D3DERR_BADMAJORVERSION:
			strcpy(returnMsg,"D3DERR_BADMAJORVERSION\0");break;
		case D3DERR_BADMINORVERSION:
			strcpy(returnMsg,"D3DERR_BADMINORVERSION\0");break;
		case D3DERR_EXECUTE_LOCKED:
			strcpy(returnMsg,"D3DERR_EXECUTE_LOCKED\0");break;
		case D3DERR_EXECUTE_NOT_LOCKED:
			strcpy(returnMsg,"D3DERR_EXECUTE_NOT_LOCKED\0");break;
		case D3DERR_EXECUTE_CREATE_FAILED:
			strcpy(returnMsg,"D3DERR_EXECUTE_CREATE_FAILED\0");break;
		case D3DERR_EXECUTE_DESTROY_FAILED:
			strcpy(returnMsg,"D3DERR_EXECUTE_DESTROY_FAILED\0");break;
		case D3DERR_EXECUTE_LOCK_FAILED:
			strcpy(returnMsg,"D3DERR_EXECUTE_LOCK_FAILED\0");break;
		case D3DERR_EXECUTE_UNLOCK_FAILED:
			strcpy(returnMsg,"D3DERR_EXECUTE_UNLOCK_FAILED\0");break;
		case D3DERR_EXECUTE_FAILED:
			strcpy(returnMsg,"D3DERR_EXECUTE_FAILED\0");break;
		case D3DERR_EXECUTE_CLIPPED_FAILED:
			strcpy(returnMsg,"D3DERR_EXECUTE_CLIPPED_FAILED\0");break;
		case D3DERR_TEXTURE_NO_SUPPORT:
			strcpy(returnMsg,"D3DERR_TEXTURE_NO_SUPPORT\0");break;
		case D3DERR_TEXTURE_NOT_LOCKED:
			strcpy(returnMsg,"D3DERR_TEXTURE_NOT_LOCKED\0");break;
		case D3DERR_TEXTURE_LOCKED:
			strcpy(returnMsg,"D3DERR_TEXTURELOCKED\0");break;
		case D3DERR_TEXTURE_CREATE_FAILED:
			strcpy(returnMsg,"D3DERR_TEXTURE_CREATE_FAILED\0");break;
		case D3DERR_TEXTURE_DESTROY_FAILED:
			strcpy(returnMsg,"D3DERR_TEXTURE_DESTROY_FAILED\0");break;
		case D3DERR_TEXTURE_LOCK_FAILED:
			strcpy(returnMsg,"D3DERR_TEXTURE_LOCK_FAILED\0");break;
		case D3DERR_TEXTURE_UNLOCK_FAILED:
			strcpy(returnMsg,"D3DERR_TEXTURE_UNLOCK_FAILED\0");break;
		case D3DERR_TEXTURE_LOAD_FAILED:
			strcpy(returnMsg,"D3DERR_TEXTURE_LOAD_FAILED\0");break;
		case D3DERR_MATRIX_CREATE_FAILED:
			strcpy(returnMsg,"D3DERR_MATRIX_CREATE_FAILED\0");break;
		case D3DERR_MATRIX_DESTROY_FAILED:
			strcpy(returnMsg,"D3DERR_MATRIX_DESTROY_FAILED\0");break;
		case D3DERR_MATRIX_SETDATA_FAILED:
			strcpy(returnMsg,"D3DERR_MATRIX_SETDATA_FAILED\0");break;
		case D3DERR_SETVIEWPORTDATA_FAILED:
			strcpy(returnMsg,"D3DERR_SETVIEWPORTDATA_FAILED\0");break;
		case D3DERR_MATERIAL_CREATE_FAILED:
			strcpy(returnMsg,"D3DERR_MATERIAL_CREATE_FAILED\0");break;
		case D3DERR_MATERIAL_DESTROY_FAILED:
			strcpy(returnMsg,"D3DERR_MATERIAL_DESTROY_FAILED\0");break;
		case D3DERR_MATERIAL_SETDATA_FAILED:
			strcpy(returnMsg,"D3DERR_MATERIAL_SETDATA_FAILED\0");break;
		case D3DERR_LIGHT_SET_FAILED:
			strcpy(returnMsg,"D3DERR_LIGHT_SET_FAILED\0");break;
		case D3DRMERR_BADOBJECT:
			strcpy(returnMsg,"D3DRMERR_BADOBJECT\0");break;
		case D3DRMERR_BADTYPE:
			strcpy(returnMsg,"D3DRMERR_BADTYPE\0");break;
		case D3DRMERR_BADALLOC:
			strcpy(returnMsg,"D3DRMERR_BADALLOC\0");break;
		case D3DRMERR_FACEUSED:
			strcpy(returnMsg,"D3DRMERR_FACEUSED\0");break;
		case D3DRMERR_NOTFOUND:
			strcpy(returnMsg,"D3DRMERR_NOTFOUND\0");break;
		case D3DRMERR_NOTDONEYET:
			strcpy(returnMsg,"D3DRMERR_NOTDONEYET\0");break;
		case D3DRMERR_FILENOTFOUND:
			strcpy(returnMsg,"The file was not found.\0");break;
		case D3DRMERR_BADFILE:
			strcpy(returnMsg,"D3DRMERR_BADFILE\0");break;
		case D3DRMERR_BADDEVICE:
			strcpy(returnMsg,"D3DRMERR_BADDEVICE\0");break;
		case D3DRMERR_BADVALUE:
			strcpy(returnMsg,"D3DRMERR_BADVALUE\0");break;
		case D3DRMERR_BADMAJORVERSION:
			strcpy(returnMsg,"D3DRMERR_BADMAJORVERSION\0");break;
		case D3DRMERR_BADMINORVERSION:
			strcpy(returnMsg,"D3DRMERR_BADMINORVERSION\0");break;
		case D3DRMERR_UNABLETOEXECUTE:
			strcpy(returnMsg,"D3DRMERR_UNABLETOEXECUTE\0");break;
        case DSERR_ALLOCATED : strcpy(returnMsg,"DSERR_ALLOCATED"); 
        case DSERR_CONTROLUNAVAIL : strcpy(returnMsg,"DSERR_CONTROLUNAVAIL"); break;
        case DSERR_INVALIDCALL : strcpy(returnMsg,"DSERR_INVALIDCALL"); break;
        case DSERR_PRIOLEVELNEEDED : strcpy(returnMsg,"DSERR_PRIOLEVELNEEDED"); break;
        case DSERR_BADFORMAT : strcpy(returnMsg,"DSERR_BADFORMAT"); break;
        case DSERR_NODRIVER : strcpy(returnMsg,"DSERR_NODRIVER"); break;
        case DSERR_ALREADYINITIALIZED : strcpy(returnMsg,"DSERR_ALREADYINITIALIZED"); break;
        case DSERR_NOAGGREGATION : strcpy(returnMsg,"DSERR_NOAGGREGATION"); break;
        case DSERR_BUFFERLOST : strcpy(returnMsg,"DSERR_BUFFERLOST"); break;
        case DSERR_OTHERAPPHASPRIO : strcpy(returnMsg,"DSERR_OTHERAPPHASPRIO"); break;
        case DSERR_UNINITIALIZED : strcpy(returnMsg,"DSERR_UNINITIALIZED"); break;
        case DDERR_NODIRECTDRAWSUPPORT : strcpy(returnMsg,"DDERR_NODIRECTDRAWSUPPORT"); break;
        case DDERR_CANTLOCKSURFACE : strcpy(returnMsg,"DDERR_CANTLOCKSURFACE"); break;
        case DDERR_UNSUPPORTEDMODE : strcpy(returnMsg,"DDERR_UNSUPPORTEDMODE"); break;
        case DDERR_NOMIPMAPHW : strcpy(returnMsg,"DDERR_NOMIPMAPHW"); break;
        case DDERR_INVALIDSURFACETYPE : strcpy(returnMsg,"DDERR_INVALIDSURFACETYPE"); break;
        case DDERR_DCALREADYCREATED : strcpy(returnMsg,"DDERR_DCALREADYCREATED"); break;
        case DDERR_CANTPAGELOCK : strcpy(returnMsg,"DDERR_CANTPAGELOCK"); break;
        case DDERR_CANTPAGEUNLOCK : strcpy(returnMsg,"DDERR_CANTPAGEUNLOCK"); break;
        case DDERR_NOTPAGELOCKED : strcpy(returnMsg,"DDERR_NOTPAGELOCKED"); break;
        case DDERR_NOTINITIALIZED : strcpy(returnMsg,"DDERR_NOTINITIALIZED"); break;

        case D3DERR_TEXTURE_SWAP_FAILED : strcpy(returnMsg,"D3DERR_TEXTURE_SWAP_FAILED"); break;
        case D3DERR_TEXTURE_GETSURF_FAILED : strcpy(returnMsg,"D3DERR_TEXTURE_GETSURF_FAILED"); break;
        case D3DERR_MATRIX_GETDATA_FAILED : strcpy(returnMsg,"D3DERR_MATRIX_GETDATA_FAILED"); break;
        case D3DERR_MATERIAL_GETDATA_FAILED : strcpy(returnMsg,"D3DERR_MATERIAL_GETDATA_FAILED"); break;
        case D3DERR_SCENE_IN_SCENE : strcpy(returnMsg,"D3DERR_SCENE_IN_SCENE"); break;
        case D3DERR_SCENE_NOT_IN_SCENE : strcpy(returnMsg,"D3DERR_SCENE_NOT_IN_SCENE"); break;
        case D3DERR_SCENE_BEGIN_FAILED : strcpy(returnMsg,"D3DERR_SCENE_BEGIN_FAILED"); break;
        case D3DERR_SCENE_END_FAILED : strcpy(returnMsg,"D3DERR_SCENE_END_FAILED"); break;

        default : strcpy(returnMsg,"Unknown Error"); 
    }

	return returnMsg;
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//	class	cTIMER	:	타이머 관련 처리
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

long	cTIMER::TPS			=	0;	//	tps 초당 몇번이나 튀는가!!??
int		cTIMER::Tick		=	0;	
int		cTIMER::FPS			=	0;

cTIMER	templetetimer;

cTIMER::cTIMER()
{	if (TPS	==	0)
	{	LARGE_INTEGER time;

		time.LowPart	=	0;
		time.HighPart	=	0;

		if(!QueryPerformanceCounter( &time ))
		{	ERRMSG("Error!! in cTIMER :: cTIMER","system not surpport QueryPerformanceCounter");
			return;
		}

		QueryPerformanceFrequency(&time);

		TPS		=	time.LowPart>>2;
	}

	m_lStartTime	=	0;
}


void
cTIMER::Start()
{	m_lStartTime	=	getCurrentTime();
}

DWORD
cTIMER::Stop()
{	long	nt	=	getCurrentTime();
	long	gt	=	nt-m_lStartTime;

	if (gt<0) gt+=	0x40000000;

	return (DWORD)gt;
}

//-----------------------------------------------------------------------------------
//	정적 멤버들..
//-----------------------------------------------------------------------------------

BOOL
cTIMER::Init(int fps)
{	if (ERRORED) return FALSE;

	Tick	=	TPS/ fps;

	MSGOUT("** Frame skip. start!! engaged fps are %d **",fps);

	return TRUE;
}

long
cTIMER::getCurrentTime()
{	LARGE_INTEGER	time;

	QueryPerformanceCounter( &time );

	return (long)(time.LowPart>>2);	//	32비트 풀로 받으면 계산하기 짱나니까.. 30비트로 바꾼다..
									//	사인드 플래기가 어쩌구 저쩌구.. 궁시렁..
}

BOOL 
cTIMER::FrameSkip()					//	이번 프레임 스킵할까?
{	if (!Tick)	return FALSE;

	static	long	newTime=0,overTick=0,oldTime=getCurrentTime();
			long	timeGab;

	newTime		=	getCurrentTime();
	timeGab		=	newTime - oldTime;

	if( timeGab < 0 ) timeGab += 0x40000000;

	overTick	=	timeGab - Tick + overTick;

	if (overTick < 0 )
	{	while(overTick < 0 )
		{	oldTime	=	newTime;
			newTime =	getCurrentTime();
			timeGab	=	newTime - oldTime;
			if( timeGab	< 0 ) timeGab += 0x40000000;
			overTick+= timeGab;
		}

		oldTime	=	newTime;

		return FALSE;
	}

	oldTime = newTime;

	return TRUE;
}

int
cTIMER::UpdateFPS()
{	static	int		fps	=	0;
	static	long	ot	=	getCurrentTime(),nt,gt;

	fps++;
	nt	=	getCurrentTime();
	gt	=	nt-ot;

	if	(gt	<	0	)	gt	+=	0x40000000;
	if	(gt	>=	TPS	)	FPS	=	fps,fps=0,ot=nt;

	return	FPS;
}

BOOL
cTIMER::isNOW(int last,int delay)
{	if (timeGetTime()-last	>= (DWORD)delay	) return TRUE;

	return FALSE;
}