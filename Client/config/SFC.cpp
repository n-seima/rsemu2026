///////////////////////////////////////////////////////////////////////////////////

//	SFC		-	Sonaki Foundation Class (I'm Hate MFC... 실은 무서워 한다. -.-;;

///////////////////////////////////////////////////////////////////////////////////

#include "SFC.H"
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
#include <process.h>
#include <iostream.h>
#include "zlib.h"

#define	dLOGMODE

static char		tempStr[1024];				//	cSTRING	클래스를 위한 템플릿 스트링..
char			g_strLogFolder[1024];		//	프로그램 실행 폴더
static	char	ReportFile[1024]=	"";
static	int		ErrorCount		=	0;

BYTE			*g_pTemplateBuffer		=	NULL;
int				g_iTemplateBufferSize	=	0;

class cSFC
{
public:
	static	BOOL	s_isOne;

					cSFC();
					~cSFC();
};

BOOL	cSFC::s_isOne	=	FALSE;

cSFC tempsfc;

cSFC::cSFC()
{
	if (s_isOne)
	{
		ERRMSG("SFC!!","nooooooooooooooooooo!!!!");
		return;
	}

	CoInitialize(NULL);

	s_isOne	=	TRUE;

	srand( (unsigned)time( NULL ) );

	GetCurrentDirectory(sizeof(g_strLogFolder),g_strLogFolder);

	if (stricmp(_exportFileName(g_strLogFolder),"debug")==0)	strcpy(g_strLogFolder,_exportFolderName(g_strLogFolder));
	if (stricmp(_exportFileName(g_strLogFolder),"release")==0)	strcpy(g_strLogFolder,_exportFolderName(g_strLogFolder));

	SetCurrentDirectory(g_strLogFolder);
}

cSFC::~cSFC()
{
	if (!s_isOne)	return;

	s_isOne	=	FALSE;

	CoUninitialize();

	SetCurrentDirectory(g_strLogFolder);
	FreeTemplateBuffer();
}

int Compare( const void *a, const void *b)
{	return (((cINDEX *)a)->Value-((cINDEX *)b)->Value);
}

void
memset16(WORD *dest,WORD value,unsigned int size)
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

inline BOOL
SafeRange(int value,int minValue,int maxValue)
{	if (value < minValue) return FALSE;
	if (value > maxValue) return FALSE;

	return TRUE;
}

inline void	
RotateValue(int &value,int maxValue)				//	크기를 체크해서 크면 0으로..
{	if (value > maxValue) value = 0;
}

inline void
ClipValue(int &value,int minValue,int maxValue)		//	크기를 체크해서 자른다.
{	if (value<minValue)	{value = minValue;	return;	}
	if (value>maxValue)	{value = maxValue;	return;	}
}

BOOL
ustrncpy(char *dest,char *src,int size)
{
	int	index	=	0;
	int	len		=	strlen(src);

	while(1)
	{
		dest[index]	=	src[index];

		if (dest[index]	== NULL) break;

		if (dest[index] & 0x80)
		{
			if (index >= size || index >= len)
			{
				dest[index] =	NULL;
				break;
			}

			index++;

			dest[index]		=	src[index];
		}

		index++;

		if (index	>= size)
		{	dest[index]	=	NULL;
			break;
		}
	}

	return TRUE;
}

void
EXIT(char *headmsg,char *mainmsg,...)
{
	va_list		argptr;

	if (!mainmsg)
		ERRMSG(headmsg);
	else
	{	char	msg[256];

		va_start(argptr, mainmsg);
		vsprintf(msg,mainmsg,argptr);
		va_end(argptr);

		ERRMSG(headmsg,msg);
	}

	PostQuitMessage(0);
}

///////////////////////////////////////////////////////////////////////////////////

//	class CPos
//		:	쩜을 다룬다.

///////////////////////////////////////////////////////////////////////////////////

CPos
CPos::operator=(const CPos & pos)
{
	Set(pos.x,pos.y);
	return pos;
}

CPos	
CPos::operator+(const CPos & pos)
{
	Set(x+pos.x,y+pos.y);
	return pos;
}

CPos
CPos::operator-(const CPos & pos)
{
	Set(x-pos.x,y-pos.y);
	return pos;
}

void
CPos::SetMousePos(HWND hwnd)
{
	GetCursorPos((POINT *)this);
	ScreenToClient(hwnd,(POINT *)this);
}

void
CPos::SetMousePos()
{
	GetCursorPos((POINT *)this);
}

void
CPos::AdjustScreen(HWND hwnd)
{
	ClientToScreen(hwnd,(POINT *)this);
}

BOOL	
CPos::Match(CPos *pos)
{
	if (x!=pos->x)	return FALSE;
	if (y!=pos->y)	return FALSE;

	return TRUE;
}

BOOL
CPos::Match(int a,int b)
{
	if (x!=a) return FALSE;	
	if (y!=b) return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////

//	class cSTRING
//		:	문자열을 다룬다.

///////////////////////////////////////////////////////////////////////////////////

BOOL _makeShotcut(char* src, char* lnkPath, char* lnkName)
{
	int			length;
	HRESULT		hres;
	IShellLink	*psl;
	char		posdancuk[256];

	hres	=	CoCreateInstance(CLSID_ShellLink, NULL,CLSCTX_INPROC_SERVER,IID_IShellLink, (LPVOID*)&psl);

	if(SUCCEEDED(hres))
	{
		IPersistFile *ppf;

		hres	=	psl->QueryInterface(IID_IPersistFile, (void **)&ppf);

		if(SUCCEEDED(hres))
		{
			WORD wsz[256];
			psl->SetPath(src);
			psl->SetArguments("");

			char Drive[MAX_PATH];

			for(length=0; length<256; length++)
			{
				if(src[length] != '\\')	Drive[length]=src[length];
				else
				{
					Drive[length]='\0';
					break;
				}
			}

			int	maxn	=	strlen(src)-1;
			
			psl->SetWorkingDirectory(_exportFolderName(src));
			psl->SetIconLocation(NULL, 1);
			psl->SetHotkey(0);
			psl->SetShowCmd(0);

			strcpy(posdancuk,_ms("%s\\%s.lnk",lnkPath,lnkName));

			MultiByteToWideChar(CP_ACP, 0, posdancuk, -1,(LPWSTR)wsz,MAX_PATH);

			hres		=	ppf->Save( (LPWSTR)wsz, TRUE);

			if	(!SUCCEEDED(hres))	return	hres;

			ppf->Release();
		}

		psl->Release();
	}

	return TRUE;
}

char	*_exportFileName(char *str)
{
	int dest=0;
	strcpy(tempStr,str);

	for (UINT i=0;i<strlen(str);i++)	if (str[i]=='\\') dest=i+1;

	return &tempStr[dest];
}

char	*_exportName(char *str)
{
	int dest=0;
	strcpy(tempStr,str);

	for (UINT i=0;i<strlen(str)	;i++)	if (str[i]=='\\') dest		=i+1;
	for (i=dest;i<strlen(str)	;i++)	if (str[i]=='.'	) tempStr[i]=NULL;



	return &tempStr[dest];
}

char	*_exportFolderName(char *str)
{
	strcpy(tempStr,str);

	int size = (int)strlen(str)-1;

	for (int i=size;i>=0;i--)	
		if (str[i]=='\\') 
		{	tempStr[i]	=	0;
			return tempStr;
		}

	return NULL;
}

char	*_exportExt(char *str)
{
	int dest=0xffff;

	for (UINT i=0;i<strlen(str);i++)	if (str[i]=='.') dest=i+1;

	if (dest==0xffff) return NULL;

	return &str[dest];
}

void	_setExt(char *srcStr,char *destStr)
{
	int		dest=0xffff;
	UINT	i,srcLen=strlen(srcStr),destLen=strlen(destStr);

	if (srcLen == 0) return;

	for (i=0;i<srcLen;i++	)	if (srcStr[i]=='.') dest=i+1;
	if	(dest==0xffff		)	srcStr[srcLen]='.',dest=srcLen+1;

	for (i=0;i<destLen;i++	)	srcStr[dest+i]	=	destStr[i];

	srcStr[dest+destLen]	=	NULL;
}

cSTRING::cSTRING()
{
	pPRE	=	NULL;
	pNEXT	=	NULL;
	String	=	NULL;
	Length	=	0;
}

cSTRING::cSTRING(char *str)
{
	pPRE	=	NULL;
	pNEXT	=	NULL;
	String	=	NULL;
	Length	=	0;

	Set(str);
}

cSTRING::~cSTRING()	
{
	_Free(String);
	Length	=	0;	
}

char* _cdecl
cSTRING::Set(char *str,...)
{
	_Free(String);

	va_list argptr;
	va_start(argptr, str);
	vsprintf(tempStr,str,argptr);
	va_end(argptr);

	int len	=	strlen(tempStr);

	String	=	_New(char,len+1,NULL,"cSTRING::String");
	strcpy(String,tempStr);
	Length	=	strlen(String);

	return String;
}

void
cSTRING::Reset()
{
	_Free(String);
	Length	=	0;	
}

void
cSTRING::SetValue(int value)
{
	Value	=	value;
}

char* _cdecl
cSTRING::Add(char *str,...)
{
	if (str	==	NULL)
		return String;

	va_list argptr;
	va_start(argptr, str);
	vsprintf(tempStr,str,argptr);
	va_end(argptr);

	char *temp;

	int len	=	strlen(tempStr);
	if (len==0) return String;

	temp	=	_New(char,len+Length+1,NULL,"cSTRING::Add temp buffer");

	if (!String) strcpy(temp,tempStr);
	else
	{	strcpy(temp,String);
		strcat(temp,tempStr);
	}
	_Free(String);
	String	=	_New(char,len+Length+1,NULL,"String Buffer");
	strcpy(String,temp);
	Length	=	strlen(String);

	_Free(temp);

	return String;
}

char* _cdecl
cSTRING::Insert(char *str,...)
{
	if (str	==	NULL)	return String;

	va_list argptr;
	va_start(argptr, str);
	vsprintf(tempStr,str,argptr);
	va_end(argptr);

	char *temp;

	int len	=	strlen(tempStr);
	if (len==0) return String;

	temp	=	_New(char,len+Length+1,NULL,"temp buffer");

	if (!String) strcpy(temp,tempStr);
	else
	{	strcpy(temp,tempStr);
		strcat(temp,String);
	}
	Length=len+Length;
	String	=	_New(char,len+Length+1,NULL,"String Buffer");
	strcpy(String,temp);
	Length	=	strlen(String);

	_Free(temp);

	return String;
}

BOOL
cSTRING::Match(char *str)
{
	if (strcmp(String,str)==0)
		return TRUE;

	return FALSE;
}

char*
cSTRING::UpperCase(char *str)
{
	int count	=	0;

	while(1)
	{
		char ch	=	str[count];

		if (ch == NULL)
			break;

		if (ch>='a' && ch<= 'z')
			ch -= 32;

		tempStr[count]	=	ch;
		count++;
	}

	tempStr[count]	=	NULL;

	return tempStr;
}

char*
cSTRING::LowCase(char *str)
{
	int count	=	0;

	while(1)
	{
		char ch	=	str[count];

		if (ch == NULL)
			break;

		if (ch>='A' && ch<= 'Z')
			ch += 32;

		tempStr[count]	=	ch;
		count++;
	}

	return tempStr;
}

cSTRING
cSTRING::operator=(const cSTRING & str)
{
	Set(str.String);
	return cSTRING(str.String);
}

cSTRING
cSTRING::operator+(const cSTRING & str)
{
	strcpy(tempStr,String);
	strcat(tempStr,str.String);
	Set(tempStr);

	return cSTRING(tempStr);
}

void
cSTRING::Minus(char *minus)
{
	char *src = String;

	while(1)
	{
		if (src==NULL)
			break;

		if (*src!=*minus)
		{
			strcpy(tempStr,src);
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
{
	while(1)
	{
		if (*src!=*minus)
		{
			strcpy(tempStr,src);
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
{
	return stricmp(((cFILENAME *)a)->Name,((cFILENAME *)b)->Name);
//	return(stricmp(((cSTRING *)a)->String,((cSTRING *)b)->String));
}

cSTRINGS::cSTRINGS(int limit)
{
	Count			=	0;
	CurString		=	0;
	maxString		=	limit;
	pHEAD			=	NULL;
	pTAIL			=	NULL;			//	처음과 마지막
}

cSTRINGS::~cSTRINGS()
{
	maxString		=	1000;
	Reset();
}

void
cSTRINGS::Init(int limit)
{
	maxString	=	limit;

	Reset();
}

void
cSTRINGS::Reset()
{
	Count			=	0;
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
cSTRINGS::Add(char *str,int value,BOOL check)
{
	if (check)
		if (GetIndex(str)!=-1)
			return FALSE;

	if (!pHEAD)
	{	pHEAD		=	new cSTRING();
		pTAIL		=	new cSTRING();
		pHEAD->pNEXT=	pTAIL;
		pTAIL->pPRE	=	pHEAD;
	}

	cSTRING	*item;

	item				=	new cSTRING();
	item->Set(str);
	item->SetValue(value);

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
{
	if (!strs)
		return FALSE;

	cSTRING *str	=	strs->pHEAD->pNEXT;

	while(str->String)
	{
		Add(str->String);
		str	=	str->pNEXT;
	}

	return TRUE;
}

BOOL
cSTRINGS::Sort(int term)
{
	cFILENAME	*Files;
	int			i,fc=0;

	fc		=	Count;
	Files	=	_New(cFILENAME,fc,NULL,"cSTRINGS::Sort Files");// [fc];

	cSTRING	*lpStr	=	Pick();

	for (i=0;i<fc;i++)
	{
		strcpy(Files[i].Name,lpStr->String);

		Files[i].m_iValue	=	lpStr->Value;

		lpStr	=	lpStr->pNEXT;
	}

	qsort((void *)Files,fc, sizeof(cFILENAME) ,cSTRINGS::Compare);

	Reset();

	for (i=0;i<fc;i++)
		Add(Files[i].Name,Files[i].m_iValue);

	_Free(Files);

	return TRUE;
}

BOOL
cSTRINGS::Remove(int index)
{
	if (index >= Count)
		return FALSE;

	cSTRING	*talk	=	Pick(index);

	if (!talk)
		return FALSE;

	talk->pPRE->pNEXT	=talk->pNEXT;
	talk->pNEXT->pPRE	=talk->pPRE;

	Count--;

	return TRUE;
}

BOOL
cSTRINGS::Remove(char *str)
{
	int index = GetIndex(str);

	if (index!=-1)
		return Remove(index);

	return FALSE;
}

cSTRING*
cSTRINGS::Pick(int index)
{
	if (index >= Count || index < 0) 
		return FALSE;

	int i	=	0;

	cSTRING	*talk	=	pHEAD->pNEXT;

	while(talk)
	{
		if (i	==	index) 
			return talk;
		talk	=	talk->pNEXT;
		i++;
	}

	return NULL;
}

char*
cSTRINGS::GetStr(int index)
{
	cSTRING	*talk = Pick(index);

	if (!talk)
		return NULL;

	return talk->String;
}

int
cSTRINGS::GetIndex(char *str)
{
	int i	=	0;

	if (!pHEAD) 
		return -1;

	cSTRING	*talk	=	pHEAD->pNEXT;

	while(talk->String)
	{	
		if (strcmp(talk->String,str)==0)
			return i;

		talk	=	talk->pNEXT;
		i++;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////////

//	cFILE	화일 제어 클레스..

///////////////////////////////////////////////////////////////////////////////////

BOOL isFolder(WIN32_FIND_DATA *fd)
{
	if(fd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
		if (strcmp(fd->cFileName,".")==0)
			return FALSE;

		if (strcmp(fd->cFileName,"..")==0)
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOL IsFolder(char *fn)
{
	HANDLE				hFind;
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
{
	HANDLE				hFind;
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
cFILE::Open(char *fn,char *mode,BOOL _bIsFFlush)
{	Close();

	fp			=	fopen(fn,mode);
	if (!fp) return FALSE;

	isMerge		=	FALSE;
	frontPos	=	0;
	endPos		=	cFILE::Size(fp);
	Length		=	endPos;
	m_wIsFFlush	=	_bIsFFlush;

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
				if (fseek(fp,pos+frontPos,seekTerm)) return FALSE;
				break;

			case	SEEK_CUR	:
				if (fseek(fp,pos,seekTerm))	return FALSE;
				break;

			case	SEEK_END	:
				if (fseek(fp,pos+endPos,SEEK_SET)) return FALSE;
				break;
		}
	}
	else
	{	if (fseek(fp,pos,seekTerm)) return FALSE;
	}

	return TRUE;
}

DWORD
cFILE::Locate()
{	return ftell(fp);
}

BOOL
cFILE::Read(void *buffer,DWORD size,DWORD itemSize)
{	if (!fp) return FALSE;

	if (size*itemSize	<=	0)	return	FALSE;

	fread(buffer,size*itemSize,1,fp);

	return TRUE;
}

BOOL
cFILE::Write(void *buffer,DWORD size,DWORD itemSize)
{	if (!fp) return FALSE;

	if (size*itemSize	<=	0)	return	FALSE;

	fwrite(buffer,size*itemSize,1,fp);

	if (m_wIsFFlush)	fflush(fp);

	return TRUE;
}

BOOL
cFILE::writeStreamNP(char *_lpstrText,BOOL _bIsPutReturn)
{
	if (!fp) return FALSE;

	fprintf(fp,_lpstrText);

	if (_bIsPutReturn)	fprintf(fp,"%c%c",0x0d,0x0a);

	if (m_wIsFFlush)	fflush(fp);

	return TRUE;
}

BOOL
cFILE::writeStream(char *_lpstrText,...)
{
	if (!fp) return FALSE;

	static	char	strMessage[1024];

	va_list		argptr;

	va_start(argptr, _lpstrText);
	vsprintf(strMessage,_lpstrText,argptr);
	va_end(argptr);

	fprintf(fp,strMessage);

	if (m_wIsFFlush)	fflush(fp);

	return TRUE;
}

BOOL
cFILE::writeStreamCR(char *_lpstrText,...)
{
	if (!fp) return FALSE;

	static	char	strMessage[1024];

	va_list		argptr;

	va_start(argptr, _lpstrText);
	vsprintf(strMessage,_lpstrText,argptr);
	va_end(argptr);

	fprintf(fp,strMessage);
	fprintf(fp,"%c%c",0x0d,0x0a);

	if (m_wIsFFlush)	fflush(fp);

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
cFILE::Skip(int skip)
{	return	Seek(skip,SEEK_CUR);
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

	char	*temp	=	_New(char,d1MEGA,NULL,"cFILE::Copt temp");

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

	_Free(temp);

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

///////////////////////////////////////////////////////////////////////////////////

//	디렉토리 제어 클레스..

///////////////////////////////////////////////////////////////////////////////////
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

			cRECT	rect;

			rect.outFrame(hwnd);

			MoveWindow(hwnd,(WIDTH-rect.x2)/2,(HEIGHT-rect.y2)/2,rect.x2,rect.y2,TRUE);
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

	if (folders->Count <=0) KILL(folders);

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

		if (files->Count <= 0) KILL(files);
		SetCurrentDirectory(tempFolder);

		return files;
	}

	fc		=	GetFileCount(ext,NULL,includeFolder);

	if (fc==0)
	{	SetCurrentDirectory(tempFolder);
		return NULL;
	}

	Files	=	_New(cFILENAME,fc,NULL,"file names");

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

	_Free(Files);

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

///////////////////////////////////////////////////////////////////////////////////

//							병합된 화일을 관리한다.

///////////////////////////////////////////////////////////////////////////////////

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

	_Free(INDEX);
	INDEX	=	_New(cMERGEINFO,Count,NULL,_ms("merge file[%s] info",fn));

	File.Read(&temp,4);								//	합쳐진 화일의 사이즈(여기선 별루 쓸데가..
	File.Read(&temp,4);								//	화일 정보의 위치
	File.Seek(temp,SEEK_SET);						//	화일의 정보가 있는 곳으로
	File.Read(INDEX,Count*sizeof(cMERGEINFO));		//	화일 정보를 읽어온다.

	return TRUE;
}

BOOL
cNUX::Close()
{	File.Close();
	_Free(INDEX);

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
	{
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
{	
	if (index >= Count)
	{
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


///////////////////////////////////////////////////////////////////////////////////

//	cMSG 클래스 메시지창을 띄운다.

///////////////////////////////////////////////////////////////////////////////////

char				cMSG::msgBuffer[1024];
HWND				cMSG::hDEBUGWND	=	NULL;

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
{
	hDEBUGWND	=	FindWindow("DEBUGWINDOW","Debug Window");

	strcpy(ReportFile,_ms("%s\\result.txt",g_strLogFolder));
	DeleteFile(ReportFile);

	char time[128];
	_strtime( time );


	float total	=	(float)(((float ) GetTotalMemory()) /1024.0 / 1024.0);
	float avail	=	(float)(((float ) GetAvailMemory()) /1024.0 / 1024.0);
}

void
cMSG::Close()
{
	char time[128];
	_strtime( time );

	float total	=	(float)(((float ) GetTotalMemory()) /1024.0 / 1024.0);
	float avail	=	(float)(((float ) GetAvailMemory()) /1024.0 / 1024.0);

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
{
	if (ReportFile[0] == 0)	return	FALSE;

	if (hDEBUGWND)
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

		strcpy((char *)lpData,_ms("%s\n",msgBuffer));
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();

		SendMessage(hDEBUGWND,WM_DEBUGTEXT,0,0);
	}

#ifdef dLOGMODE

 	if (!hDEBUGWND)
	{	va_list		argptr;

		va_start(argptr, mainmsg);
		vsprintf(msgBuffer,mainmsg,argptr);
		va_end	(argptr);
		DWORD size = strlen(msgBuffer);

		msgBuffer[size]		=	0x0d;
		msgBuffer[size+1]	=	0x0a;
		msgBuffer[size+2]	=	NULL;
	}
	
	FILE	*fp = fopen(ReportFile, "ab+" );

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

	if (!mainmsg)
	{	MessageBox(GetActiveWindow(),headmsg,"Error Message",MB_ICONERROR|MB_TOPMOST);

		if (ReportFile[0])
		{
			FILE	*fp = fopen(ReportFile, "ab+" );

			if (fp)
			{	fprintf( fp, "Error Message - %s\n",headmsg);
				fclose( fp );
			}
		}
	}
	else
	{	va_start(argptr, mainmsg);
		vsprintf(msgBuffer,mainmsg,argptr);
		va_end(argptr);

		MessageBox(GetActiveWindow(),msgBuffer,headmsg,MB_ICONERROR|MB_TOPMOST);

		if (ReportFile[0])
		{
			FILE	*fp = fopen(ReportFile, "ab+" );

			if (fp)
			{	fprintf( fp, "%s - %s\n",headmsg,msgBuffer);
				fclose( fp );
			}
		}
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
        case D3DERR_SCENE_END_FAILED	: strcpy(returnMsg,"D3DERR_SCENE_END_FAILED"); break;
        case DD_OK						: strcpy(returnMsg,"OK"); break;

        default : strcpy(returnMsg,"Unknown Error"); 
    }

	return returnMsg;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//	class	CTimeInfo	:	시간 얻어오기
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

CTimeInfo::CTimeInfo()
{
	update();
}

void
CTimeInfo::update()
{
	time_t tTime;
	struct tm *tmTime;

	tTime		=	time(NULL);
	tmTime		=	localtime(&tTime);

	m_wYear		=	1900+tmTime->tm_year;
	m_wMonth	=	tmTime->tm_mon+1;
	m_wDay		=	tmTime->tm_mday;
	m_wHour		=	tmTime->tm_hour;
	m_wMinute	=	tmTime->tm_min;
	m_wSecond	=	tmTime->tm_sec;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//	class	cTIMER	:	타이머 관련 처리
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

int		cTIMER::s_iTps			=	0;			//	tps 초당 몇번이나 튀는가!!??
int		cTIMER::s_iFps			=	0;
int		cTIMER::s_iFrameCounter	=	0;
int		cTIMER::s_iLastTime		=	0;	//	FPS 측정을 위한 수치

cTIMER	templateTimer;

cTIMER::cTIMER()
{
	if (s_iTps	==	0)
	{	LARGE_INTEGER time;

		time.LowPart	=	0;
		time.HighPart	=	0;

		if(!QueryPerformanceCounter( &time ))
		{	ERRMSG("Error!! in cTIMER :: cTIMER","system not surpport QueryPerformanceCounter");
			return;
		}

		QueryPerformanceFrequency(&time);

		s_iTps		=	time.LowPart>>2;
	}

	m_iOldTime		=	0;
	m_iTick			=	0;	
	m_iNewTime		=	0;
	m_iOverTick		=	0;
	m_iSkipCount	=	0;	//	프레임 스킵한 횟수.
	m_iTimerStartTime=	0;
}

//
//	프레임 스킵 설정
BOOL
cTIMER::setFPS(int _iFps)
{
	if (_iFps <= 0 )
	{	
		m_iTick	=	0;
		return TRUE;
	}

	m_iTick	=	s_iTps/ _iFps;

	resetFrameSkip();

	return TRUE;
}	//	cTIMER::setFPS(int _iFps)

void
cTIMER::resetFrameSkip()
{
	m_iOldTime	=	GetTime();
}

//
//	이번 프레임 스킵할까?
BOOL 
cTIMER::isSkipFrame()
{
	if (!m_iTick)	
	{
		Sleep(1);

		return FALSE;
	}

	m_iNewTime		=	GetTime();
	int	iTimeGab	=	m_iNewTime - m_iOldTime;

	if( iTimeGab < 0 ) 
		iTimeGab += 0x40000000;

	m_iOverTick		=	iTimeGab - m_iTick + m_iOverTick;

	if (m_iOverTick < 0 )	//	기다린다.
	{
		while(m_iOverTick < 0 )
		{
			Sleep(1);
			m_iOldTime	=	m_iNewTime;
			m_iNewTime	=	GetTime();
			iTimeGab	=	m_iNewTime - m_iOldTime;
			if( iTimeGab< 0 ) iTimeGab += 0x40000000;
			m_iOverTick	+=	iTimeGab;
		}

		m_iOldTime		=	m_iNewTime;
		m_iSkipCount	=	0;

		return FALSE;
	}

	m_iSkipCount++;

	if (m_iSkipCount	>	16)		//	최대 8프레임만 스킵한다.
	{
		m_iOldTime		=	m_iNewTime;
		m_iOverTick		=	0;
		m_iSkipCount	=	0;

		return FALSE;
	}

	m_iOldTime	=	m_iNewTime;

	return TRUE;
}	//	cTIMER::isSkipFrame()

//
//	FPS측정
int
cTIMER::UpdateFPS()
{
	int		iNewTime	=	GetTime();
	int		iTimeGap	=	iNewTime-s_iLastTime;

	if	(iTimeGap		<	0		)	iTimeGap	+=	0x40000000;
	if	(iTimeGap		>=	s_iTps	)
	{
		s_iFps			=	s_iFrameCounter;
		s_iFrameCounter	=	0;
		s_iLastTime		=	iNewTime;

		return	s_iFps;
	}
	
	s_iFrameCounter++;

	return	s_iFps;
}	//	cTIMER::updateFPS()



///////////////////////////////////////////////////////////////////////////////////

//	class	cMCU	:	메모리 관리 (Memory Control Unit)

///////////////////////////////////////////////////////////////////////////////////

#define	dMEMORY_ALLIGN	8

//
//	생성자
cMCU::cMCU()
{	m_dwFullSize		=	0;
	m_dwFrontPointer	=	0;
	m_pMemory			=	NULL;
	m_dwTempAlloc		=	0xffffffff;
	m_lpReference		=	NULL;

#ifdef	dDEBUG_MODE
//	m_dwLogCount		=	0;
#endif

}	//	cMCU::cMCU()

//
//	디스트럭터
cMCU::~cMCU()
{
	close();
}	//	cMCU::~cMCU()

//
//	메모리 설정
BOOL
cMCU::init(DWORD size,char *name)
{	strncpy(m_strName,name,63);

	if (size%dMEMORY_ALLIGN!=0)	size+=(dMEMORY_ALLIGN-size%dMEMORY_ALLIGN);

	m_dwFullSize	=	size;

	reset();
	_Free(m_pMemory);

	m_pMemory		=	_New(char,m_dwFullSize,NULL,_ms("MCU %s's m_pMemory",m_strName));

	if (!m_pMemory)	return ERRMSG("Error in cMCU::init","Memory allocation Error!!");

	if ((DWORD)m_pMemory%dMEMORY_ALLIGN	!= 0)
		m_dwFrontPointer	+=	dMEMORY_ALLIGN	-	((DWORD)m_pMemory%dMEMORY_ALLIGN);

	m_dwRearPointer	=	m_dwFullSize;

	return	TRUE;
}	//	메모리 설정

//
//	레퍼런스 메모리 설정
BOOL
cMCU::init(cMCU *_lpMcu,char *name,BOOL _isRearStack)
{
	if (_lpMcu->m_lpReference)	return	ERRMSG("error in cMCU::init(cMCU *_lpMcu,char *name,BOOL _isRearStack)","%s's reference memory",name);

	m_lpReference	=	_lpMcu;
	m_isRearStack	=	_isRearStack;

	strncpy(m_strName,name,63);

	return	TRUE;
}	//	cMCU::init(cMCU *_lpMcu,char *name,BOOL _isRearStack)

//
//	작살-o- 낸다.
void
cMCU::close()
{
	reset();

	if (!m_lpReference)	_Free(m_pMemory);

	m_lpReference		=	NULL;
}	//	cMCU::destroy()

//
//	초기화 하고, 상처 입은 메모리가 있는지 확인한다.
void
cMCU::reset()
{
	if (m_lpReference)
	{
		if (m_isRearStack)	m_lpReference->m_dwRearPointer	=	m_lpReference->m_dwFullSize;
		else				m_lpReference->m_dwFrontPointer	=	0;
	}
	else
	{
		m_dwFrontPointer	=	0;
		m_dwRearPointer		=	m_dwFullSize;
		m_dwTempAlloc		=	0xffffffff;
	}

#ifdef	dDEBUG_MODE

	if (m_dwLogCount > 0 &&	!m_lpReference)
	{

		m_dwLogCount	=	0;
	}
#endif
}	//	cMCU::reset()

#ifdef	dDEBUG_MODE

//
//	데미지 입은 메모리가 없는지 확인
void
cMCU::check(DWORD address)
{	
	unsigned int	head,tail,size;

	char *ptr	=	m_pMemory+address;
	
	head		=	*(DWORD *)(ptr		);		//	헤더
	size		=	*(DWORD *)(ptr+4	);		//	헤더
	tail		=	*(DWORD *)(ptr+size	);		//	헤더

	if (head	!= dCHECK_MCU_HEAD)	ERRMSG(_ms("MCU %s's follow pointer HEAD offset damaged!!",m_strName),ptr+8);
	if (tail	!= dCHECK_MCU_TAIL)	ERRMSG(_ms("MCU %s's follow pointer TAIL offset damaged!!",m_strName),ptr+8);
}	//	cMCU::check(DWORD address)

//
//	메모리 할당
void*
cMCU::alloc(DWORD size,char *_lpstrName,BOOL _isRearStack)
{
	if (m_lpReference)
		return	m_lpReference->alloc(size,_lpstrName,m_isRearStack);

	size	+=	4+4+4+dPTR_STRING_SIZE;

	if (size%dMEMORY_ALLIGN!=0)	size+=(dMEMORY_ALLIGN-size%dMEMORY_ALLIGN);

	if (_isRearStack)
	{
		if (m_dwFrontPointer+size >= m_dwRearPointer)
		{
			ERRMSG("Error in cMCU::ALLOC","Memory allocation Error(Rear)!!");
			return NULL;
		}

		m_dwRearPointer			-=	size;

//		m_dwaLog[m_dwLogCount++]	=	m_dwRearPointer;

		char	*ptr			=	m_pMemory+m_dwRearPointer;

		*(DWORD *)(ptr)			=	dCHECK_MCU_HEAD;
		*(DWORD *)(ptr+4)		=	size-4;
		*(DWORD *)(ptr+size-4)	=	dCHECK_MCU_TAIL;

		strncpy(ptr+8,_ms("mcu %s's rear %d",m_strName,m_dwRearPointer),dPTR_STRING_SIZE-1);	//	이름

		*(DWORD *)(m_pMemory+m_dwRearPointer)	=	dCHECK_MCU_HEAD;

		return (void *)(m_pMemory+m_dwRearPointer+dPTR_STRING_SIZE+8);
	}
	
	if (m_dwTempAlloc!=0xffffffff)
	{	ERRMSG("Error in cMCU::alloc","There are Exist Template Memory");
		return NULL;
	}

	if (m_dwFrontPointer%dMEMORY_ALLIGN	!= 0)	m_dwFrontPointer	+=	dMEMORY_ALLIGN	-	(m_dwFrontPointer%dMEMORY_ALLIGN);

	if (m_dwFrontPointer+size >= m_dwRearPointer)
	{	ERRMSG("Error in cMCU::alloc","Memory allocation Error(Front)!!");
		return NULL;
	}

//	m_dwaLog[m_dwLogCount++]	=	m_dwFrontPointer;

	char	*ptr			=	m_pMemory+m_dwFrontPointer;

	*(DWORD *)(ptr)			=	dCHECK_MCU_HEAD;
	*(DWORD *)(ptr+4)		=	size-4;
	*(DWORD *)(ptr+size-4)	=	dCHECK_MCU_TAIL;

	strncpy(ptr+8,_ms("mcu %s's front %d",m_strName,m_dwFrontPointer),dPTR_STRING_SIZE-1);	//	이름

	m_dwFrontPointer		+=	size;

	return (void *)(m_pMemory+m_dwFrontPointer-size+dPTR_STRING_SIZE+8);
}

//
//	메모리 임시 할당(이동안 아무것도 못한다.)
void*
cMCU::tempAlloc(DWORD size,char *name)
{
	if (m_lpReference)	return	NULL;

	if (m_dwTempAlloc	!=	0xffffffff)
	{	ERRMSG("Error in cMCU::TempAlloc","There are Already Exist Template Memory");
		return NULL;
	}

	void *mem	=	alloc(size,name);

	if (!mem) return NULL;

	m_dwTempAlloc	=	m_dwFrontPointer;

	return mem;
}	//	cMCU::tempAlloc(DWORD size,char *name)

//
//	임시로 할당된 메모리 해제
void
cMCU::tempFree()
{
	if (m_dwTempAlloc	==	0xffffffff)
		return;
//	m_dwLogCount--;
//	check(m_dwaLog[m_dwLogCount]);

	m_dwFrontPointer=	m_dwTempAlloc;
	m_dwTempAlloc	=	0xffffffff;
}

#else	//	#ifdef dDEBUG_MODE

//
//	메모리 할당
void*
cMCU::alloc(DWORD size,BOOL _isRearStack)
{
	if (m_lpReference)
		return	m_lpReference->alloc(size,m_isRearStack);

	if (size%dMEMORY_ALLIGN	!=0	)
		size+=	(dMEMORY_ALLIGN-(size%dMEMORY_ALLIGN));	//	메모리 정렬

	size	+=	4;		//	MCU에서 할당 받은 포인터라는 낙인찍을 공간.

	if (_isRearStack)
	{
		if (m_dwFrontPointer+size >= m_dwRearPointer)
		{
			ERRMSG("Error in cMCU::ALLOC","Memory allocation Error(Rear)!!");
			return NULL;
		}

		m_dwRearPointer		-=	size;

		*(DWORD *)(m_pMemory+m_dwRearPointer)	=	dCHECK_MCU_HEAD;

		return (void *)(m_pMemory+m_dwRearPointer+4);
	}


	if (m_dwTempAlloc!=0xffffffff)
	{
		ERRMSG("Error in cMCU::ALLOC","There are Exist Template Memory");
		return NULL;
	}


	if (m_dwFrontPointer%dMEMORY_ALLIGN	!=0)	
		m_dwFrontPointer	+=	(dMEMORY_ALLIGN-(m_dwFrontPointer%dMEMORY_ALLIGN));

	if (m_dwFrontPointer+size >= m_dwFullSize)
	{
		ERRMSG("Error in cMCU::ALLOC","Memory allocation Error(Front)!!");
		return NULL;
	}

	*(DWORD *)(m_pMemory+m_dwFrontPointer)	=	dCHECK_MCU_HEAD;

	m_dwFrontPointer	+=	size;

	return (void *)(m_pMemory+m_dwFrontPointer-size	+	4);
}

//
//
void*
cMCU::tempAlloc(DWORD size,char *name)
{
	if (m_lpReference)	return	NULL;

	if (m_dwTempAlloc	!=	0xffffffff)
	{	ERRMSG("Error in cMCU::TempAlloc","There are Already Exist Template Memory");
		return NULL;
	}

	void *mem	=	alloc(size);

	if (!mem) return NULL;

	m_dwTempAlloc	=	m_dwFrontPointer;

	return mem;
}

//
//	
void
cMCU::tempFree()
{	if (m_dwTempAlloc	==	0xffffffff)	return;
	m_dwFrontPointer=	m_dwTempAlloc;
	m_dwTempAlloc	=	0xffffffff;
}

#endif	//	#ifdef dDEBUG_MODE

//
//	남은 메모리
DWORD
cMCU::getRemainMemorySize()
{
	if(m_lpReference)	return	m_lpReference->getRemainMemorySize();

	return	m_dwRearPointer - m_dwFrontPointer;
}	//	cMCU::getRemainMemorySize()

//
//	사용된 메모리
DWORD
cMCU::getFilledMemorySize()
{
	if(m_lpReference)	return	m_lpReference->getFilledMemorySize();

	return	m_dwFrontPointer+(m_dwFullSize-m_dwRearPointer);
}	//	cMCU::getFilledMemorySize()


DWORD
GetTotalMemory()
{	MEMORYSTATUS ms;
	ms.dwLength = sizeof( MEMORYSTATUS );
	GlobalMemoryStatus( &ms );

	return ms.dwTotalPhys;
}

DWORD
GetAvailMemory()
{	MEMORYSTATUS ms;
	ms.dwLength = sizeof( MEMORYSTATUS );
	GlobalMemoryStatus( &ms );

	return ms.dwAvailPhys;
}

///////////////////////////////////////////////////////////////////////////////////

//	class	cTHREAD	:	쓰레드 관리

///////////////////////////////////////////////////////////////////////////////////

cTHREAD::cTHREAD()
{	ThreadHandle	=	0; // 스레드 핸들
	ThreadAddr		=	0;          // 스레드 주소
}

cTHREAD::~cTHREAD()
{	Close();
}


void
cTHREAD::Create(HWND hwnd,void (__cdecl *func)(void *_data),void *_lpData)
{
	ThreadHandle = CreateThread( NULL, 0, (unsigned long (__stdcall *)(void *))func,_lpData, 0, &ThreadAddr );
}

void
cTHREAD::Close()
{
	if (!ThreadHandle)	return;

	CloseHandle(ThreadHandle);
	
	ThreadHandle	=	0;
}

//
//	초기화
void
CStream::init(DWORD _dwSize)
{
	m_pBuffer		=	new char [_dwSize];
	m_dwBufferSize	=	_dwSize;
	m_dwBufferPoint	=	0;
}	//	CStream::init(DWORD _dwSize)

void
CStream::reset()
{
	m_dwBufferPoint	=	0;
}	//	CStream::reset()

void
CStream::close()
{
	pKILL(m_pBuffer);
}	//	CStream::close()

BOOL
CStream::addStringP(char *_lpstrText,...)
{
	va_list		argptr;
	char		strText[1024];

	va_start(argptr, _lpstrText);
	vsprintf(strText,_lpstrText,argptr);
	va_end(argptr);

	DWORD		dwStringLength	=	strlen(strText);

	if (m_dwBufferPoint+dwStringLength >= m_dwBufferSize+3)	return	FALSE;

	memcpy(m_pBuffer+m_dwBufferPoint,strText,dwStringLength);

	m_dwBufferPoint				+=	dwStringLength;

	m_pBuffer[m_dwBufferPoint]	=	0x0d;
	m_pBuffer[m_dwBufferPoint+1]=	0x0a;
	m_dwBufferPoint				+=	2;

	m_pBuffer[m_dwBufferPoint]	=	NULL;

	return	TRUE;
}	//	CStream::addStringP(char *_lpstrText,...)

BOOL
CStream::addString(char *_lpstrText)
{
	DWORD		dwStringLength	=	strlen(_lpstrText);

	if (m_dwBufferPoint+dwStringLength >= m_dwBufferSize+3)	return	FALSE;

	memcpy(m_pBuffer+m_dwBufferPoint,_lpstrText,dwStringLength);

	m_dwBufferPoint				+=	dwStringLength;

	m_pBuffer[m_dwBufferPoint]	=	0x0d;
	m_pBuffer[m_dwBufferPoint+1]=	0x0a;
	m_dwBufferPoint				+=	2;

	m_pBuffer[m_dwBufferPoint]	=	NULL;

	return	TRUE;
}	//	CStream::addString(char *_lpstrText)

BOOL
CStream::writeToFile(char *_lpstrFileName,BOOL _bIsReset)
{
	char	strFileName[512];

	sprintf(strFileName,"%s/%s",g_strLogFolder,_lpstrFileName);

	FILE	*fp;

	fp	=	fopen(strFileName,"wb");

	if (!fp)	return	FALSE;

	fprintf(fp,m_pBuffer);
	fclose(fp);

	if (_bIsReset)	reset();

	return	TRUE;
}	//	CStream::writeToFile(char *_lpstrFileName,BOOL _bIsReset)


DWORD	EncodeBuffer(int _iSeedValue,BYTE *_lpBuffer,int _iBufferSize)
{
// 	srand(_iSeedValue);
// 
// 	int		iRandValue	=	_iSeedValue%g_uiLogInServerPacketEncodeSeedValue1+g_uiLogInServerPacketEncodeSeedValue2;
// 	DWORD	dwCheckSum	=	0;
// 
// 	for (int i=0;i<_iBufferSize;i++)
// 	{
// 		dwCheckSum		+=	_lpBuffer[i];
// 		_lpBuffer[i]	+=	random(iRandValue);
// 	}
// 	srand( (unsigned)time( NULL ) );

	return	0;
}

DWORD	DecodeBuffer(int _iSeedValue,BYTE *_lpBuffer,int _iBufferSize)
{
// 	srand(_iSeedValue);
// 
// 	int		iRandValue	=	_iSeedValue%g_uiLogInServerPacketEncodeSeedValue1+g_uiLogInServerPacketEncodeSeedValue2;
// 	DWORD	dwCheckSum	=	0;
// 
// 	for (int i=0;i<_iBufferSize;i++)
// 	{
// 		_lpBuffer[i]	-=	random(iRandValue);
// 		dwCheckSum		+=	_lpBuffer[i];
// 	}
// 	srand( (unsigned)time( NULL ) );

	return	0;
}

int
StringToNumber(char *_strText,int _iLimitValue,int _iDecimal)
{
	int		iLength			=	strlen(_strText),i;
	int		iDotCount		=	0;
	int		iDecimalCount	=	0,iNumberCount	=	0,iDecimal,iNumber;
	char	strNumber[11],strDecimal[11];

	for (i=0;i<iLength;i++)
	{
		if (_strText[i]	==	'.')
		{
			iDotCount++;

			if (iDotCount	>=	2)	break;
		}
		else
		if (_strText[i]	>=	'0'	&&	_strText[i]	<=	'9')
		{
			if (iDotCount)
			{
				strDecimal[iDecimalCount++]	=	_strText[i];
				if (iDecimalCount	>=	_iDecimal)	break;
			}
			else
			{
				strNumber[iNumberCount++]	=	_strText[i];
				if (iNumberCount	>=	10)	break;
			}
		}
		else	break;
	}

	strNumber[iNumberCount]		=	NULL;
	strDecimal[iDecimalCount]	=	NULL;

	iNumber	=	atoi(strNumber);
	iDecimal=	atoi(strDecimal);

	if (iNumber	>=	_iLimitValue)
	{	iNumber	=	_iLimitValue;
		iDecimal=	0;
	}

	for (i=0;i<_iDecimal;i++)				iNumber	*=	10;
	for (i=0;i<_iDecimal-iDecimalCount;i++)	iDecimal*=	10;

	return	iNumber+iDecimal;
}	//	StringToNumber(char *_strText,int _iDecimal)

int
ZipCompress(unsigned char * _lpSourcedata, unsigned char * _lpTargetData,int _iDataSize)
{
	WORD		iResultSize = 0;
	z_stream	zipStream;
	int			iFlush, iStatus , iMaxInBufferSize;

	iMaxInBufferSize = _iDataSize+1024;

	zipStream.zalloc= Z_NULL;
	zipStream.zfree	= Z_NULL;
	zipStream.opaque= Z_NULL;
	if (deflateInit(&zipStream, Z_DEFAULT_COMPRESSION) != Z_OK) 
	{
//		printf("deflateInit: %s\n", (zipStream.msg) ? zipStream.msg : "???");
		return 0; 
	}

	zipStream.avail_in = 0;
	zipStream.next_out = (unsigned char *)_lpTargetData;
	zipStream.avail_out = _iDataSize;

	iFlush = Z_NO_FLUSH;

	while (1)
	{
		if (zipStream.avail_in == 0)
		{
			zipStream.next_in	= (unsigned char * )_lpSourcedata;
			zipStream.avail_in	= _iDataSize;

    		if (zipStream.avail_in < (UINT)iMaxInBufferSize) 
				iFlush = Z_FINISH;
		}

		iStatus = deflate(&zipStream, iFlush);
		
		if (iStatus == Z_STREAM_END) break;
		if (iStatus != Z_OK)
		{
//			printf("deflate: %s\n", (zipStream.msg) ? zipStream.msg : "???");
			return 0;
		}
		if (zipStream.avail_out == 0)
		{
			zipStream.next_out = (unsigned char *)_lpTargetData;
			zipStream.avail_out = _iDataSize;
		}
	}
	iResultSize = _iDataSize - zipStream.avail_out;

	if(_iDataSize - iResultSize	<=	0)
		return	0;	//	압축한 데이터 사이즈가 더 작거나 같다.

	if (deflateEnd(&zipStream) != Z_OK)
	{
//		printf("deflateEnd: %s\n", (zipStream.msg) ? zipStream.msg : "???");
		return 0;
	}

	return iResultSize;
}

int
ZipUncompress(unsigned char * _lpSourcedata, unsigned char * _lpTargetData,int _iDataSize,int _iSourceSize)
{
	int			iResultSize = 0;
	z_stream	zipStream;
	int			iStatus, iMaxInBufferSize	=	 _iSourceSize;

	zipStream.zalloc	= Z_NULL;
	zipStream.zfree		= Z_NULL;
	zipStream.opaque	= Z_NULL;

	zipStream.next_in	= Z_NULL;
	zipStream.avail_in	= 0;
	
	if (inflateInit(&zipStream) != Z_OK) 
	{
//		printf("inflateInit: %s\n", (zipStream.msg) ? zipStream.msg : "???");
		return iResultSize;
	}

	zipStream.next_out	=	(unsigned char *)_lpTargetData;
	zipStream.avail_out	=	_iSourceSize;
	iStatus				=	Z_OK;

	while (iStatus != Z_STREAM_END) 
	{
		if (zipStream.avail_in	==	0) 
		{ 
			zipStream.next_in	=	(unsigned char *)_lpSourcedata;  
			zipStream.avail_in	=	_iDataSize; 
		}

		iStatus		=	inflate(&zipStream, Z_NO_FLUSH); 

		if (iStatus == Z_STREAM_END) 
			break;

		if (iStatus != Z_OK) 
		{
//			printf("inflate: %s\n", (zipStream.msg) ? zipStream.msg : "???");
			return iResultSize;
		}
		if (zipStream.avail_out == 0) 
		{ 
			zipStream.next_out	= (unsigned char *)_lpTargetData; 
			zipStream.avail_out = _iSourceSize; 
		}
	}

	iResultSize	=	_iSourceSize - zipStream.avail_out;

	if (inflateEnd(&zipStream) != Z_OK) 
	{
//		printf("inflateEnd: %s\n", (zipStream.msg) ? zipStream.msg : "???");
		return iResultSize;
	}

	return iResultSize;
}